#include <array>
#include <iostream>
#include <stdexcept>
#include "../../../gta4-recomp/src/gta4_quicksave_policy.h"
#include "../../../gta4-recomp/src/gta4_phone_quicksave_profiles.h"
#ifdef GTA4_QUICKSAVE_STANDALONE
static size_t checks = 0;
#define CHECK_Q(...) do { ++checks; if (!(__VA_ARGS__)) throw std::runtime_error(#__VA_ARGS__); } while(false)
#else
#include <catch2/catch_test_macros.hpp>
#define CHECK_Q(...) REQUIRE(__VA_ARGS__)
#endif
namespace {
using namespace gta4::quicksave;
void Verify() {
  for (unsigned bits=0;bits<4096;++bits) {
    Conditions c{.enabled=true,.known_program=true,.player_ready=true,.signed_in=true};
    c.enabled=!(bits&1);c.known_program=!(bits&2);c.player_ready=!(bits&4);c.signed_in=!(bits&8);
    c.multiplayer=bits&16;c.mission=bits&32;c.activity=bits&64;c.in_vehicle=bits&128;
    c.busy=bits&256;c.phone_call=bits&512;c.wanted=bits&1024;c.transitioning=bits&2048;
    CHECK_Q((CanSave(c)==Denial::kNone)==(bits==0));
    CHECK_Q(!ReasonKey(CanSave(c)).empty());
    CHECK_Q(ReasonKey(CanSave(c)).size()<=8);
  }
  Session session{5,0x123456,1,0,0x83000000};
  for(bool success:{false,true}) {
    Transaction t;CHECK_Q(t.Begin(session,100));CHECK_Q(!t.Begin(session,100));
    Observation o{.session=session,.milliseconds=120};CHECK_Q(t.Poll(o)==Decision::kNone);
    o.phone_closed=true;CHECK_Q(t.Poll(o)==Decision::kOpenSave);CHECK_Q(t.phase()==Phase::kWaitingForUi);
    o.frontend_requested=true;o.milliseconds=300;CHECK_Q(t.Poll(o)==Decision::kNone);
    o.frontend_visible=true;CHECK_Q(t.Poll(o)==Decision::kNone);CHECK_Q(t.phase()==Phase::kSaveUi);
    o.frontend_visible=false;o.storage_busy=true;o.milliseconds=900000;
    CHECK_Q(t.Poll(o)==Decision::kNone);CHECK_Q(t.phase()==Phase::kSaveUi);
    o.storage_busy=false;o.frontend_requested=false;o.save_succeeded=success;
    CHECK_Q(t.Poll(o)==(success?Decision::kSaved:Decision::kCancelled));
    CHECK_Q(t.phase()==Phase::kIdle);CHECK_Q(t.Poll(o)==Decision::kNone);
  }
  for(unsigned which=0;which<5;++which) {
    Transaction t;CHECK_Q(t.Begin(session,0));Observation o{.session=session,.milliseconds=10};
    if(which==0)++o.session.epoch;if(which==1)++o.session.xuid;if(which==2)++o.session.episode;
    if(which==3)++o.session.player;if(which==4)++o.session.ped;
    CHECK_Q(t.Poll(o)==Decision::kAbandoned);CHECK_Q(t.phase()==Phase::kIdle);
  }
  for(auto denial:{Denial::kMission,Denial::kBusy,Denial::kPhone,Denial::kVehicle,Denial::kTransition}) {
    Transaction t;CHECK_Q(t.Begin(session,100));
    CHECK_Q(t.Poll({.session=session,.milliseconds=500,.eligibility=denial})==Decision::kAbandoned);
  }
  Transaction timeout;CHECK_Q(timeout.Begin(session,100));
  CHECK_Q(timeout.Poll({.session=session,.milliseconds=8101})==Decision::kAbandoned);
  Transaction handoff;CHECK_Q(handoff.Begin(session,100));
  CHECK_Q(handoff.Poll({.session=session,.milliseconds=101,.phone_closed=true})==Decision::kOpenSave);
  CHECK_Q(handoff.Poll({.session=session,.milliseconds=8200,.frontend_requested=true})==Decision::kAbandoned);
  // A prior completed save must not make a new rejected or cancelled request succeed.
  Transaction repeat;CHECK_Q(repeat.Begin(session,0));
  CHECK_Q(repeat.Poll({.session=session,.milliseconds=1,.phone_closed=true,.save_succeeded=true})==Decision::kOpenSave);
  CHECK_Q(repeat.Poll({.session=session,.milliseconds=2,.save_succeeded=false})==Decision::kNone);
  CHECK_Q(repeat.Poll({.session=session,.milliseconds=300,.save_succeeded=false})==Decision::kCancelled);
  for(const auto& p:kPhoneProfiles) {
    CHECK_Q(MatchPhoneProfile(p.code_size,p.sha256)==&p);
    CHECK_Q(!MatchPhoneProfile(p.code_size,"modified"));
    std::vector<uint8_t> v(p.code_size);
    std::array<uint8_t,5> append{uint8_t(p.menu_local+96),63,40,uint8_t(p.options_local),uint8_t(p.options_local>>8)};
    std::array<uint8_t,5> accept{uint8_t(p.menu_local+96),63,40,60,0};
    std::copy(append.begin(),append.end(),v.begin()+p.append_site);
    std::copy(accept.begin(),accept.end(),v.begin()+p.accept_site);
    v[p.append_function]=47;v[p.append_function+1]=p.append_args;
    v[p.set_state_function]=47;v[p.set_state_function+1]=1;v[p.accept_exit]=34;
    auto patched=BuildPhonePatch(v,p,0x90000000,0x90000010);
    CHECK_Q(patched.code.size()>v.size());CHECK_Q(patched.append_extension==v.size());
    for(size_t i=0;i<v.size();++i)
      if((i<p.append_site||i>=p.append_site+5)&&(i<p.accept_site||i>=p.accept_site+5))CHECK_Q(v[i]==patched.code[i]);
    CHECK_Q(bytecode::Read32(patched.code,p.append_site+1)==patched.append_extension);
    CHECK_Q(bytecode::Read32(patched.code,p.accept_site+1)==patched.accept_extension);
    for(unsigned failure=0;failure<5;++failure) {
      auto broken=p;auto bytes=v;
      if(failure==0)broken.append_site=UINT32_MAX;
      if(failure==1)broken.accept_site=p.code_size;
      if(failure==2)broken.append_args=9;
      if(failure==3)bytes[p.accept_site]^=1;
      bool threw=false;try{BuildPhonePatch(bytes,broken,failure==4?0:0x90000000,0x90000010);}catch(const std::exception&){threw=true;}
      CHECK_Q(threw);
    }
  }
}
}
#ifdef GTA4_QUICKSAVE_STANDALONE
int main(){try{Verify();std::cout<<"PASS "<<checks<<" Quicksave policy/patch assertions\n";}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
#else
TEST_CASE("Quicksave transactions and verified program extension", "[gta4-quicksave]"){Verify();}
#endif
