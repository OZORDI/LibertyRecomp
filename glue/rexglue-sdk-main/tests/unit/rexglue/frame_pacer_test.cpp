#include <catch2/catch_test_macros.hpp>
#include <rex/ui/frame_pacer.h>
#include <rex/ui/paint_wakeup_state.h>
#include <rex/ui/presentation_clock.h>
#include <array>
#include <future>
#include <thread>
#include <vector>
using namespace rex::ui;
using namespace std::chrono_literals;

TEST_CASE("Single pacer preserves rational cadence at every supported rate", "[frame-pacer]") {
  for (uint32_t fps : {30u,40u,60u,120u}) {
    FramePacer p;
    const uint64_t origin = 1'000'000'000;
    p.Configure(fps, origin);
    uint64_t now = origin;
    for (uint64_t frame = 0; frame < 1200; ++frame) {
      auto plan = p.Plan(now);
      if (plan.delay_ns) { now += plan.delay_ns; plan = p.Plan(now); }
      REQUIRE(plan.slot_ns == origin + frame * FramePacer::kSecond / fps);
      REQUIRE(plan.delay_ns == 0);
      REQUIRE(plan.display_target_ns == origin + (frame+1) * FramePacer::kSecond / fps);
      p.Queued(now);
    }
  }
}
TEST_CASE("Retries do not spend a slot and long stalls do not release a burst", "[frame-pacer]") {
  FramePacer p; p.Configure(40, 1'000'000'000);
  const auto initial = p.Plan(1'000'000'000);
  REQUIRE(p.Plan(1'001'000'000).slot_ns == initial.slot_ns);
  p.Queued(1'001'000'000);
  REQUIRE(p.Plan(1'002'000'000).delay_ns == 23'000'000);
  auto late = p.Plan(2'000'000'000);
  REQUIRE(late.delay_ns == 0);
  p.Queued(2'000'000'000);
  REQUIRE(p.Plan(2'000'000'001).delay_ns == 24'999'999);
}
TEST_CASE("Cap changes, unlocked, clock regression, and surface reset drop stale deadlines", "[frame-pacer]") {
  FramePacer p;
  for (auto fps : {30u,40u,60u,120u,0u,30u}) {
    const auto previous = p.generation();
    p.Configure(fps, 2'000'000'000);
    const auto a = p.Plan(2'000'000'000);
    REQUIRE(a.delay_ns == 0);
    REQUIRE(a.fps == fps);
    REQUIRE(a.generation > previous);
    p.Queued(2'000'000'000);
  }
  REQUIRE(p.Plan(1).delay_ns == 0);
  p.Reset(); REQUIRE(p.Plan(2).delay_ns == 0);
  p.Configure(1001,3); REQUIRE(p.fps() == 0);
}
TEST_CASE("Validated display phase is adopted only once per rate and surface epoch", "[frame-pacer]") {
  FramePacer p;p.Configure(40,1'000'000'000);p.Plan(1'000'000'000);p.Queued(1'001'000'000);
  REQUIRE_FALSE(p.ObservePhase(1'050'000'000,1'040'000'000));
  REQUIRE(p.ObservePhase(1'008'000'000,1'015'000'000));
  REQUIRE(p.Plan(1'015'000'000).slot_ns == 1'033'000'000);
  REQUIRE_FALSE(p.ObservePhase(1'009'000'000,1'015'000'000));
  p.Configure(60,1'040'000'000);REQUIRE_FALSE(p.phase_observed());
}
TEST_CASE("One display timeline absorbs wake and paint cost variation within the budget", "[frame-pacer]") {
  FramePacer p;p.Configure(40,1'000'000'000);
  p.Plan(1'000'000'000);p.Queued(1'001'000'000);
  REQUIRE(p.ObservePhase(1'005'000'000,1'010'000'000));
  uint64_t now = 1'010'000'000, previous_target = 0;
  for (unsigned i=0;i<600;++i) {
    auto plan=p.Plan(now);
    now += plan.delay_ns + (i%2 ? 5'000'000 : 0);
    plan=p.Plan(now);
    const uint64_t work = i%2 ? 9'000'000 : 1'000'000;
    REQUIRE(now+work < plan.display_target_ns);
    if(previous_target) REQUIRE(plan.display_target_ns-previous_target == 25'000'000);
    previous_target=plan.display_target_ns;
    p.Queued(now+work);now+=work;
  }
}
TEST_CASE("Deadline arithmetic saturates safely near the clock boundary", "[frame-pacer]") {
  FramePacer p;p.Configure(30,UINT64_MAX-10);auto a=p.Plan(UINT64_MAX-10);
  REQUIRE(a.display_target_ns==UINT64_MAX);p.Queued(UINT64_MAX-5);
  REQUIRE(p.Plan(UINT64_MAX-4).delay_ns==4);
  REQUIRE_FALSE(p.ObservePhase(1,UINT64_MAX-4));
}
TEST_CASE("Publication acknowledgement blocks only the producer and consumes the exact serial", "[frame-pacer]") {
  FramePublicationGate g;g.SetAvailable(true);
  auto first=g.Publish(40);g.Accept(first);
  REQUIRE(g.Wait(first));
  auto second=g.Publish(40);
  auto wait=std::async(std::launch::async,[&]{return g.Wait(second);});
  REQUIRE(wait.wait_for(5ms)==std::future_status::timeout);
  g.Accept(first);
  REQUIRE(wait.wait_for(5ms)==std::future_status::timeout);
  g.Accept(second+1);
  REQUIRE(wait.wait_for(5ms)==std::future_status::timeout);
  g.Accept(second); REQUIRE(wait.wait_for(100ms)==std::future_status::ready);REQUIRE(wait.get());
}
TEST_CASE("Producer handoff cancels on surface loss, shutdown, and unlocked mode", "[frame-pacer]") {
  for(int action=0;action<3;++action) {
    FramePublicationGate g;g.SetAvailable(true);auto serial=g.Publish(40);
    auto wait=std::async(std::launch::async,[&]{return g.Wait(serial);});
    REQUIRE(wait.wait_for(5ms)==std::future_status::timeout);
    if(action==0)g.SetAvailable(false);else if(action==1)g.Stop();else g.Publish(0);
    REQUIRE(wait.wait_for(100ms)==std::future_status::ready);REQUIRE(wait.get());
  }
}
TEST_CASE("Paint wake tickets preempt delayed timers without consuming newer work", "[frame-pacer]") {
  PaintWakeupState s;
  auto deferred=s.Request(1000);REQUIRE(deferred!=0);
  REQUIRE(s.Request(2000)==0);
  auto immediate=s.Request(100);REQUIRE(immediate!=0);
  REQUIRE_FALSE(s.IsCurrent(deferred));REQUIRE_FALSE(s.Complete(deferred));
  REQUIRE(s.IsCurrent(immediate));REQUIRE(s.Complete(immediate));
  auto newer=s.Request(300);REQUIRE_FALSE(s.Complete(immediate));REQUIRE(s.IsCurrent(newer));
  s.Stop();REQUIRE_FALSE(s.IsCurrent(newer));REQUIRE(s.Request(1)==0);
  PaintWakeupState reopened;auto fresh=reopened.Request(1);
  REQUIRE(fresh!=newer);REQUIRE_FALSE(reopened.Complete(newer));
}
TEST_CASE("Clock mapping correlates epochs and rejects unsafe measurements", "[frame-pacer]") {
  PresentationClockMapping m;
  REQUIRE(m.ToDriver(100)==0);
  REQUIRE_FALSE(m.Sample(100,0,101));REQUIRE_FALSE(m.Sample(101,100,100));
  REQUIRE_FALSE(m.Sample(100,100,300101));
  REQUIRE(m.Sample(1000000,5000000,1000200));
  REQUIRE(m.uncertainty()==100);REQUIRE(m.ToDriver(1000100)==5000000);
  REQUIRE(m.ToDriver(2000100)==6000000);REQUIRE(m.ToHost(6000000)==2000100);
  REQUIRE(m.ToHost(1)==0);
}
TEST_CASE("Feedback accepts only matching issued IDs, epoch data and plausible times", "[frame-pacer]") {
  PresentationFeedbackHistory h;
  auto id=h.NewID();h.Insert({id,12,40,3,100,200,7});
  auto good=h.Take(id,200,210,250);REQUIRE(good);REQUIRE(good->serial==7);
  REQUIRE_FALSE(h.Take(id,200,210,250));
  id=h.NewID();h.Insert({id,12,40,3,100,200,7});REQUIRE_FALSE(h.Take(id,201,210,250));
  id=h.NewID();h.Insert({id,12,40,3,100,200,7});REQUIRE_FALSE(h.Take(id,200,99,250));
  id=h.NewID();h.Insert({id,12,40,3,100,200,7});REQUIRE_FALSE(h.Take(id,200,4'000'000,250));
  id=h.NewID();h.Insert({id,12,40,3,100,200,7});h.Clear();REQUIRE_FALSE(h.Take(id,200,210,250));
  id=h.NewID();h.Insert({id,12,40,3,100,200,7});
  REQUIRE_FALSE(h.Take(id,200,210,3'000'000'000));
}
