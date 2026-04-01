#include "gta4_init.h"

PPC_FUNC_IMPL(__imp__sub_825D3DF0);
PPC_WEAK_FUNC(sub_825D3DF0) { __imp__sub_825D3DF0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D3DF0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stfd f29,-32(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f29.u64);
	// stfd f30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f30.u64);
	// stfd f31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.f31.u64);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f1,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f1.f64 = double(temp.f32);
	// lfs f31,0(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f31.f64 = double(temp.f32);
	// lfs f30,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f30.f64 = double(temp.f32);
	// lfs f29,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f29.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,5704(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 5704);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f1,f0
	ctx.cr6.compare(ctx.f1.f64, ctx.f0.f64);
	// bgt cr6,0x825d3e3c
	if (ctx.cr6.gt) goto loc_825D3E3C;
	// li r5,4
	ctx.r5.s64 = 4;
	// fmr f2,f30
	ctx.f2.f64 = ctx.f30.f64;
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// bl 0x82363040
	ctx.lr = 0x825D3E3C;
	sub_82363040(ctx, base);
loc_825D3E3C:
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// stfs f1,88(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// stfs f31,80(r1)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// fmr f1,f29
	ctx.f1.f64 = ctx.f29.f64;
	// stfs f30,84(r1)
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// bl 0x8225f970
	ctx.lr = 0x825D3E54;
	sub_8225F970(ctx, base);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f29,-32(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f29.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// lfd f30,-24(r1)
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// lfd f31,-16(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D3E70);
PPC_WEAK_FUNC(sub_825D3E70) { __imp__sub_825D3E70(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D3E70) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stfd f29,-32(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f29.u64);
	// stfd f30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f30.u64);
	// stfd f31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.f31.u64);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f1,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f1.f64 = double(temp.f32);
	// lfs f31,0(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f31.f64 = double(temp.f32);
	// lfs f30,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f30.f64 = double(temp.f32);
	// lfs f29,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f29.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,5704(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 5704);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f1,f0
	ctx.cr6.compare(ctx.f1.f64, ctx.f0.f64);
	// bgt cr6,0x825d3ebc
	if (ctx.cr6.gt) goto loc_825D3EBC;
	// li r5,4
	ctx.r5.s64 = 4;
	// fmr f2,f30
	ctx.f2.f64 = ctx.f30.f64;
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// bl 0x82363040
	ctx.lr = 0x825D3EBC;
	sub_82363040(ctx, base);
loc_825D3EBC:
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// stfs f1,88(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// stfs f31,80(r1)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// fmr f1,f29
	ctx.f1.f64 = ctx.f29.f64;
	// stfs f30,84(r1)
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// bl 0x8225f9d8
	ctx.lr = 0x825D3ED4;
	sub_8225F9D8(ctx, base);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f29,-32(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f29.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// lfd f30,-24(r1)
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// lfd f31,-16(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D3EF0);
PPC_WEAK_FUNC(sub_825D3EF0) { __imp__sub_825D3EF0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D3EF0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lbz r11,-26547(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + -26547);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D3F18);
PPC_WEAK_FUNC(sub_825D3F18) { __imp__sub_825D3F18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D3F18) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lbz r11,-1635(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + -1635);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D3F38);
PPC_WEAK_FUNC(sub_825D3F38) { __imp__sub_825D3F38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D3F38) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32065
	ctx.r10.s64 = -2101411840;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,-1636(r10)
	PPC_STORE_U8(ctx.r10.u32 + -1636, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D3F58);
PPC_WEAK_FUNC(sub_825D3F58) { __imp__sub_825D3F58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D3F58) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d3fa0
	if (ctx.cr6.eq) goto loc_825D3FA0;
	// bl 0x821fe708
	ctx.lr = 0x825D3F88;
	sub_821FE708(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,131(r3)
	PPC_STORE_U8(ctx.r3.u32 + 131, ctx.r11.u8);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_825D3FA0:
	// bl 0x821fe708
	ctx.lr = 0x825D3FA4;
	sub_821FE708(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// stb r11,131(r3)
	PPC_STORE_U8(ctx.r3.u32 + 131, ctx.r11.u8);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D3FC0);
PPC_WEAK_FUNC(sub_825D3FC0) { __imp__sub_825D3FC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D3FC0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// li r31,0
	ctx.r31.s64 = 0;
	// bl 0x821fe708
	ctx.lr = 0x825D3FE0;
	sub_821FE708(ctx, base);
	// lbz r11,135(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 135);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825d3ff8
	if (!ctx.cr6.eq) goto loc_825D3FF8;
	// bl 0x821fe708
	ctx.lr = 0x825D3FF0;
	sub_821FE708(ctx, base);
	// li r31,1
	ctx.r31.s64 = 1;
	// stb r31,135(r3)
	PPC_STORE_U8(ctx.r3.u32 + 135, ctx.r31.u8);
loc_825D3FF8:
	// lwz r11,0(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// clrlwi r10,r31,24
	ctx.r10.u64 = ctx.r31.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4020);
PPC_WEAK_FUNC(sub_825D4020) { __imp__sub_825D4020(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4020) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// lfs f1,12(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f1.f64 = double(temp.f32);
	// lfs f0,0(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f12.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f12,88(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// bl 0x822bca90
	ctx.lr = 0x825D4060;
	sub_822BCA90(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4070);
PPC_WEAK_FUNC(sub_825D4070) { __imp__sub_825D4070(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4070) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// lfs f1,12(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f1.f64 = double(temp.f32);
	// lfs f0,0(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f12.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f12,88(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// bl 0x822bca90
	ctx.lr = 0x825D40B0;
	sub_822BCA90(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D40C0);
PPC_WEAK_FUNC(sub_825D40C0) { __imp__sub_825D40C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D40C0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lfs f0,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f12.f64 = double(temp.f32);
	// srawi r9,r10,8
	ctx.xer.ca = (ctx.r10.s32 < 0) & ((ctx.r10.u32 & 0xFF) != 0);
	ctx.r9.s64 = ctx.r10.s32 >> 8;
	// lfs f1,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f1.f64 = double(temp.f32);
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f12,88(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// lwz r11,1792(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 1792);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r8,r8,r9
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r8.u32 + ctx.r9.u32);
	// cmpw cr6,r8,r10
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r10.s32, ctx.xer);
	// bne cr6,0x825d4124
	if (!ctx.cr6.eq) goto loc_825D4124;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r9
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r9.s32);
	// add r6,r11,r10
	ctx.r6.u64 = ctx.r11.u64 + ctx.r10.u64;
	// b 0x825d4128
	goto loc_825D4128;
loc_825D4124:
	// li r6,0
	ctx.r6.s64 = 0;
loc_825D4128:
	// li r7,0
	ctx.r7.s64 = 0;
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x822bca90
	ctx.lr = 0x825D4138;
	sub_822BCA90(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4148);
PPC_WEAK_FUNC(sub_825D4148) { __imp__sub_825D4148(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4148) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r7,1
	ctx.r7.s64 = 1;
	// li r6,0
	ctx.r6.s64 = 0;
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// lfs f1,12(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f1.f64 = double(temp.f32);
	// lfs f0,0(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f12.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f12,88(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// bl 0x822bca90
	ctx.lr = 0x825D4188;
	sub_822BCA90(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4198);
PPC_WEAK_FUNC(sub_825D4198) { __imp__sub_825D4198(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4198) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lfs f0,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f12.f64 = double(temp.f32);
	// srawi r9,r10,8
	ctx.xer.ca = (ctx.r10.s32 < 0) & ((ctx.r10.u32 & 0xFF) != 0);
	ctx.r9.s64 = ctx.r10.s32 >> 8;
	// lfs f1,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f1.f64 = double(temp.f32);
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f12,88(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// lwz r11,1792(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 1792);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r8,r8,r9
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r8.u32 + ctx.r9.u32);
	// cmpw cr6,r8,r10
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r10.s32, ctx.xer);
	// bne cr6,0x825d41fc
	if (!ctx.cr6.eq) goto loc_825D41FC;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r9
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r9.s32);
	// add r6,r11,r10
	ctx.r6.u64 = ctx.r11.u64 + ctx.r10.u64;
	// b 0x825d4200
	goto loc_825D4200;
loc_825D41FC:
	// li r6,0
	ctx.r6.s64 = 0;
loc_825D4200:
	// li r7,1
	ctx.r7.s64 = 1;
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x822bca90
	ctx.lr = 0x825D4210;
	sub_822BCA90(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4220);
PPC_WEAK_FUNC(sub_825D4220) { __imp__sub_825D4220(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4220) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// bl 0x82165060
	ctx.lr = 0x825D4238;
	sub_82165060(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4258);
PPC_WEAK_FUNC(sub_825D4258) { __imp__sub_825D4258(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4258) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// lfs f3,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f3.f64 = double(temp.f32);
	// lfs f4,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f4.f64 = double(temp.f32);
	// b 0x825d23b8
	sub_825D23B8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4278);
PPC_WEAK_FUNC(sub_825D4278) { __imp__sub_825D4278(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4278) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// lfs f3,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f3.f64 = double(temp.f32);
	// lfs f4,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f4.f64 = double(temp.f32);
	// b 0x825d2440
	sub_825D2440(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4298);
PPC_WEAK_FUNC(sub_825D4298) { __imp__sub_825D4298(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4298) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stfd f29,-32(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f29.u64);
	// stfd f30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f30.u64);
	// stfd f31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.f31.u64);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f1,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f1.f64 = double(temp.f32);
	// lfs f31,0(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f31.f64 = double(temp.f32);
	// lfs f30,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f30.f64 = double(temp.f32);
	// lfs f29,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f29.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,5704(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 5704);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f1,f0
	ctx.cr6.compare(ctx.f1.f64, ctx.f0.f64);
	// bgt cr6,0x825d42e4
	if (ctx.cr6.gt) goto loc_825D42E4;
	// li r5,4
	ctx.r5.s64 = 4;
	// fmr f2,f30
	ctx.f2.f64 = ctx.f30.f64;
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// bl 0x82363040
	ctx.lr = 0x825D42E4;
	sub_82363040(ctx, base);
loc_825D42E4:
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// stfs f1,88(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// stfs f31,80(r1)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// fmr f1,f29
	ctx.f1.f64 = ctx.f29.f64;
	// stfs f30,84(r1)
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// bl 0x82270100
	ctx.lr = 0x825D42FC;
	sub_82270100(ctx, base);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f29,-32(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f29.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// lfd f30,-24(r1)
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// lfd f31,-16(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4318);
PPC_WEAK_FUNC(sub_825D4318) { __imp__sub_825D4318(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4318) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f0,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f12.f64 = double(temp.f32);
	// lfs f11,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f11.f64 = double(temp.f32);
	// lfs f10,16(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f10.f64 = double(temp.f32);
	// lis r11,-32058
	ctx.r11.s64 = -2100953088;
	// stfs f0,-16(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -16, temp.u32);
	// stfs f13,-12(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + -12, temp.u32);
	// stfs f12,-8(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + -8, temp.u32);
	// addi r10,r1,-16
	ctx.r10.s64 = ctx.r1.s64 + -16;
	// stfs f11,-31744(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + -31744, temp.u32);
	// lis r11,-32058
	ctx.r11.s64 = -2100953088;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stfs f10,-31748(r11)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r11.u32 + -31748, temp.u32);
	// lis r11,-32058
	ctx.r11.s64 = -2100953088;
	// addi r11,r11,-29696
	ctx.r11.s64 = ctx.r11.s64 + -29696;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4368);
PPC_WEAK_FUNC(sub_825D4368) { __imp__sub_825D4368(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4368) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f0,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f12.f64 = double(temp.f32);
	// lfs f11,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f11.f64 = double(temp.f32);
	// lfs f10,16(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f10.f64 = double(temp.f32);
	// lis r11,-32058
	ctx.r11.s64 = -2100953088;
	// stfs f0,-16(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -16, temp.u32);
	// stfs f13,-12(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + -12, temp.u32);
	// stfs f12,-8(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + -8, temp.u32);
	// addi r10,r1,-16
	ctx.r10.s64 = ctx.r1.s64 + -16;
	// stfs f11,-31752(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + -31752, temp.u32);
	// lis r11,-32058
	ctx.r11.s64 = -2100953088;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stfs f10,-31756(r11)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r11.u32 + -31756, temp.u32);
	// lis r11,-32058
	ctx.r11.s64 = -2100953088;
	// addi r11,r11,-29680
	ctx.r11.s64 = ctx.r11.s64 + -29680;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D43B8);
PPC_WEAK_FUNC(sub_825D43B8) { __imp__sub_825D43B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D43B8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32058
	ctx.r10.s64 = -2100953088;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,-31096(r10)
	PPC_STORE_U8(ctx.r10.u32 + -31096, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D43D8);
PPC_WEAK_FUNC(sub_825D43D8) { __imp__sub_825D43D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D43D8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r10
	ctx.r11.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// b 0x8226fc70
	sub_8226FC70(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D43F8);
PPC_WEAK_FUNC(sub_825D43F8) { __imp__sub_825D43F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D43F8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// bl 0x8223cbb0
	ctx.lr = 0x825D4410;
	sub_8223CBB0(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// lwz r10,0(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4440);
PPC_WEAK_FUNC(sub_825D4440) { __imp__sub_825D4440(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4440) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// addi r11,r11,-24840
	ctx.r11.s64 = ctx.r11.s64 + -24840;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// bne cr6,0x825d4468
	if (!ctx.cr6.eq) goto loc_825D4468;
	// li r11,1
	ctx.r11.s64 = 1;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
loc_825D4468:
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// addi r11,r11,14644
	ctx.r11.s64 = ctx.r11.s64 + 14644;
	// lbz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 4);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4490);
PPC_WEAK_FUNC(sub_825D4490) { __imp__sub_825D4490(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4490) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// bl 0x8223cbc8
	ctx.lr = 0x825D44A8;
	sub_8223CBC8(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// lwz r10,0(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D44D8);
PPC_WEAK_FUNC(sub_825D44D8) { __imp__sub_825D44D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D44D8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32087
	ctx.r10.s64 = -2102853632;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,21630(r10)
	PPC_STORE_U8(ctx.r10.u32 + 21630, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4508);
PPC_WEAK_FUNC(sub_825D4508) { __imp__sub_825D4508(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4508) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lwz r9,24(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// lfs f3,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f3.f64 = double(temp.f32);
	// lfs f4,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f4.f64 = double(temp.f32);
	// lfs f5,16(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f5.f64 = double(temp.f32);
	// lfs f6,20(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	ctx.f6.f64 = double(temp.f32);
	// b 0x825d2ac8
	sub_825D2AC8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4530);
PPC_WEAK_FUNC(sub_825D4530) { __imp__sub_825D4530(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4530) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d24c8
	sub_825D24C8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4540);
PPC_WEAK_FUNC(sub_825D4540) { __imp__sub_825D4540(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4540) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lfs f0,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f12.f64 = double(temp.f32);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,7597(r10)
	PPC_STORE_U8(ctx.r10.u32 + 7597, ctx.r11.u8);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stfs f0,-576(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -576, temp.u32);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stfs f13,-580(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + -580, temp.u32);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stfs f12,-584(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + -584, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4578);
PPC_WEAK_FUNC(sub_825D4578) { __imp__sub_825D4578(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4578) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// bl 0x825fd800
	ctx.lr = 0x825D4590;
	sub_825FD800(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D45B0);
PPC_WEAK_FUNC(sub_825D45B0) { __imp__sub_825D45B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D45B0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r10,1
	ctx.r10.s64 = 1;
	// lwz r9,0(r3)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// slw r10,r10,r8
	ctx.r10.u64 = ctx.r8.u8 & 0x20 ? 0 : (ctx.r10.u32 << (ctx.r8.u8 & 0x3F));
	// and r11,r10,r11
	ctx.r11.u64 = ctx.r10.u64 & ctx.r11.u64;
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stw r11,0(r9)
	PPC_STORE_U32(ctx.r9.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D45E0);
PPC_WEAK_FUNC(sub_825D45E0) { __imp__sub_825D45E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D45E0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x825d2cf8
	sub_825D2CF8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4600);
PPC_WEAK_FUNC(sub_825D4600) { __imp__sub_825D4600(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4600) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// lwz r11,7488(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 7488);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// bgt cr6,0x825d4618
	if (ctx.cr6.gt) goto loc_825D4618;
	// li r11,0
	ctx.r11.s64 = 0;
loc_825D4618:
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4628);
PPC_WEAK_FUNC(sub_825D4628) { __imp__sub_825D4628(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4628) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// bl 0x821fe708
	ctx.lr = 0x825D4640;
	sub_821FE708(ctx, base);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x825d465c
	if (ctx.cr6.eq) goto loc_825D465C;
	// bl 0x821fe708
	ctx.lr = 0x825D464C;
	sub_821FE708(ctx, base);
	// lbz r11,133(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 133);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// bne cr6,0x825d4660
	if (!ctx.cr6.eq) goto loc_825D4660;
loc_825D465C:
	// li r11,0
	ctx.r11.s64 = 0;
loc_825D4660:
	// lwz r10,0(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4680);
PPC_WEAK_FUNC(sub_825D4680) { __imp__sub_825D4680(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4680) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// addi r11,r11,-24840
	ctx.r11.s64 = ctx.r11.s64 + -24840;
	// lwz r11,40(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 40);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D46A8);
PPC_WEAK_FUNC(sub_825D46A8) { __imp__sub_825D46A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D46A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// addi r11,r11,-24840
	ctx.r11.s64 = ctx.r11.s64 + -24840;
	// lwz r11,128(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 128);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D46C0);
PPC_WEAK_FUNC(sub_825D46C0) { __imp__sub_825D46C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D46C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lis r11,-32069
	ctx.r11.s64 = -2101673984;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,-1468(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + -1468);
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x82205180
	ctx.lr = 0x825D46E8;
	sub_82205180(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4708);
PPC_WEAK_FUNC(sub_825D4708) { __imp__sub_825D4708(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4708) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// bl 0x821f33c0
	ctx.lr = 0x825D4720;
	sub_821F33C0(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4740);
PPC_WEAK_FUNC(sub_825D4740) { __imp__sub_825D4740(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4740) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// bge cr6,0x825d4754
	if (!ctx.cr6.lt) goto loc_825D4754;
	// neg r11,r11
	ctx.r11.s64 = static_cast<int64_t>(-ctx.r11.u64);
loc_825D4754:
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4760);
PPC_WEAK_FUNC(sub_825D4760) { __imp__sub_825D4760(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4760) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lfs f0,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// fabs f0,f0
	ctx.f0.u64 = ctx.f0.u64 & ~0x8000000000000000;
	// stfs f0,0(r10)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4778);
PPC_WEAK_FUNC(sub_825D4778) { __imp__sub_825D4778(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4778) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// lfs f3,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f3.f64 = double(temp.f32);
	// lfs f4,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f4.f64 = double(temp.f32);
	// lfs f5,16(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f5.f64 = double(temp.f32);
	// lfs f6,20(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	ctx.f6.f64 = double(temp.f32);
	// bl 0x825d2e88
	ctx.lr = 0x825D47AC;
	sub_825D2E88(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D47D0);
PPC_WEAK_FUNC(sub_825D47D0) { __imp__sub_825D47D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D47D0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// lfs f3,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f3.f64 = double(temp.f32);
	// lfs f4,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f4.f64 = double(temp.f32);
	// lfs f5,16(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f5.f64 = double(temp.f32);
	// lfs f6,20(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	ctx.f6.f64 = double(temp.f32);
	// bl 0x825d2f18
	ctx.lr = 0x825D4804;
	sub_825D2F18(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4828);
PPC_WEAK_FUNC(sub_825D4828) { __imp__sub_825D4828(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4828) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// addi r12,r1,-16
	ctx.r12.s64 = ctx.r1.s64 + -16;
	// bl 0x82a01328
	ctx.lr = 0x825D483C;
	__savefpr_28(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// li r5,0
	ctx.r5.s64 = 0;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lfs f31,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f31.f64 = double(temp.f32);
	// lfs f30,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f30.f64 = double(temp.f32);
	// cntlzw r10,r10
	ctx.r10.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// lfs f29,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f29.f64 = double(temp.f32);
	// lfs f28,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f28.f64 = double(temp.f32);
	// rlwinm r11,r10,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d4880
	if (ctx.cr6.eq) goto loc_825D4880;
	// bl 0x8225cf80
	ctx.lr = 0x825D487C;
	sub_8225CF80(ctx, base);
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
loc_825D4880:
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// stfs f31,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f30,84(r1)
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// fmr f1,f28
	ctx.f1.f64 = ctx.f28.f64;
	// stfs f29,88(r1)
	temp.f32 = float(ctx.f29.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// bl 0x8229a540
	ctx.lr = 0x825D4898;
	sub_8229A540(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// addi r12,r1,-16
	ctx.r12.s64 = ctx.r1.s64 + -16;
	// bl 0x82a01374
	ctx.lr = 0x825D48B0;
	__restfpr_28(ctx, base);
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D48C0);
PPC_WEAK_FUNC(sub_825D48C0) { __imp__sub_825D48C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D48C0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r10,24(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// cntlzw r10,r10
	ctx.r10.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// lfs f3,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f3.f64 = double(temp.f32);
	// lfs f4,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f4.f64 = double(temp.f32);
	// rlwinm r10,r10,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// lfs f5,16(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f5.f64 = double(temp.f32);
	// lfs f6,20(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	ctx.f6.f64 = double(temp.f32);
	// xori r11,r10,1
	ctx.r11.u64 = ctx.r10.u64 ^ 1;
	// clrlwi r9,r11,24
	ctx.r9.u64 = ctx.r11.u32 & 0xFF;
	// bl 0x825d2600
	ctx.lr = 0x825D4908;
	sub_825D2600(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4928);
PPC_WEAK_FUNC(sub_825D4928) { __imp__sub_825D4928(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4928) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d4980
	if (ctx.cr6.eq) goto loc_825D4980;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// lfs f0,2612(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f0,84(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f0,88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// bl 0x82476708
	ctx.lr = 0x825D4970;
	sub_82476708(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_825D4980:
	// bl 0x82476758
	ctx.lr = 0x825D4984;
	sub_82476758(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4998);
PPC_WEAK_FUNC(sub_825D4998) { __imp__sub_825D4998(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4998) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// li r10,1
	ctx.r10.s64 = 1;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D49A8);
PPC_WEAK_FUNC(sub_825D49A8) { __imp__sub_825D49A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D49A8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// lwz r31,19924(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 19924);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82219ac0
	ctx.lr = 0x825D49D0;
	sub_82219AC0(ctx, base);
	// lbz r11,361(r31)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r31.u32 + 361);
	// lwz r10,0(r30)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D49F8);
PPC_WEAK_FUNC(sub_825D49F8) { __imp__sub_825D49F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D49F8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32003
	ctx.r10.s64 = -2097348608;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,7743(r10)
	PPC_STORE_U8(ctx.r10.u32 + 7743, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4A18);
PPC_WEAK_FUNC(sub_825D4A18) { __imp__sub_825D4A18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4A18) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lbz r11,7743(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 7743);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4A30);
PPC_WEAK_FUNC(sub_825D4A30) { __imp__sub_825D4A30(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4A30) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// addi r10,r10,-10284
	ctx.r10.s64 = ctx.r10.s64 + -10284;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,28(r10)
	PPC_STORE_U8(ctx.r10.u32 + 28, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4A58);
PPC_WEAK_FUNC(sub_825D4A58) { __imp__sub_825D4A58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4A58) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// addi r10,r10,-10284
	ctx.r10.s64 = ctx.r10.s64 + -10284;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,29(r10)
	PPC_STORE_U8(ctx.r10.u32 + 29, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4A80);
PPC_WEAK_FUNC(sub_825D4A80) { __imp__sub_825D4A80(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4A80) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r8,1
	ctx.r8.s64 = 1;
	// lwz r7,0(r3)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r11,r10,r11
	ctx.r11.u64 = ctx.r11.u64 - ctx.r10.u64;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// sraw r10,r9,r10
	temp.u32 = ctx.r10.u32 & 0x3F;
	if (temp.u32 > 0x1F) temp.u32 = 0x1F;
	ctx.xer.ca = (ctx.r9.s32 < 0) & (((ctx.r9.s32 >> temp.u32) << temp.u32) != ctx.r9.s32);
	ctx.r10.s64 = ctx.r9.s32 >> temp.u32;
	// slw r11,r8,r11
	ctx.r11.u64 = ctx.r11.u8 & 0x20 ? 0 : (ctx.r8.u32 << (ctx.r11.u8 & 0x3F));
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// and r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 & ctx.r10.u64;
	// stw r11,0(r7)
	PPC_STORE_U32(ctx.r7.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4AB8);
PPC_WEAK_FUNC(sub_825D4AB8) { __imp__sub_825D4AB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4AB8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82205c20
	sub_82205C20(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4AD8);
PPC_WEAK_FUNC(sub_825D4AD8) { __imp__sub_825D4AD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4AD8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r31,r11,1
	ctx.r31.u64 = ctx.r11.u64 ^ 1;
	// bl 0x821fe708
	ctx.lr = 0x825D4B00;
	sub_821FE708(ctx, base);
	// stb r31,137(r3)
	PPC_STORE_U8(ctx.r3.u32 + 137, ctx.r31.u8);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4B18);
PPC_WEAK_FUNC(sub_825D4B18) { __imp__sub_825D4B18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4B18) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r31,r11,1
	ctx.r31.u64 = ctx.r11.u64 ^ 1;
	// bl 0x821fe708
	ctx.lr = 0x825D4B40;
	sub_821FE708(ctx, base);
	// stb r31,144(r3)
	PPC_STORE_U8(ctx.r3.u32 + 144, ctx.r31.u8);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4B58);
PPC_WEAK_FUNC(sub_825D4B58) { __imp__sub_825D4B58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4B58) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x82247d78
	sub_82247D78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4B68);
PPC_WEAK_FUNC(sub_825D4B68) { __imp__sub_825D4B68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4B68) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x82244720
	ctx.lr = 0x825D4B8C;
	sub_82244720(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4BB0);
PPC_WEAK_FUNC(sub_825D4BB0) { __imp__sub_825D4BB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4BB0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82476798
	sub_82476798(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4BD0);
PPC_WEAK_FUNC(sub_825D4BD0) { __imp__sub_825D4BD0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4BD0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x8225c368
	sub_8225C368(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4BF0);
PPC_WEAK_FUNC(sub_825D4BF0) { __imp__sub_825D4BF0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4BF0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32086
	ctx.r10.s64 = -2102788096;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stw r11,6520(r10)
	PPC_STORE_U32(ctx.r10.u32 + 6520, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4C08);
PPC_WEAK_FUNC(sub_825D4C08) { __imp__sub_825D4C08(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4C08) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82a12628
	sub_82A12628(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4C28);
PPC_WEAK_FUNC(sub_825D4C28) { __imp__sub_825D4C28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4C28) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x824768e0
	sub_824768E0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4C48);
PPC_WEAK_FUNC(sub_825D4C48) { __imp__sub_825D4C48(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4C48) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r11,-24248
	ctx.r3.s64 = ctx.r11.s64 + -24248;
	// lbz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d4c78
	if (ctx.cr6.eq) goto loc_825D4C78;
	// li r11,1
	ctx.r11.s64 = 1;
	// b 0x825d4c8c
	goto loc_825D4C8C;
loc_825D4C78:
	// bl 0x8224e5d0
	ctx.lr = 0x825D4C7C;
	sub_8224E5D0(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
loc_825D4C8C:
	// lwz r10,0(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4CB0);
PPC_WEAK_FUNC(sub_825D4CB0) { __imp__sub_825D4CB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4CB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lbz r11,10577(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 10577);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4CC8);
PPC_WEAK_FUNC(sub_825D4CC8) { __imp__sub_825D4CC8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4CC8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32076
	ctx.r11.s64 = -2102132736;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,-27388(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -27388);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4CE0);
PPC_WEAK_FUNC(sub_825D4CE0) { __imp__sub_825D4CE0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4CE0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,19924(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 19924);
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x8221a0d0
	ctx.lr = 0x825D4D08;
	sub_8221A0D0(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4D28);
PPC_WEAK_FUNC(sub_825D4D28) { __imp__sub_825D4D28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4D28) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// bl 0x821fe708
	ctx.lr = 0x825D4D40;
	sub_821FE708(ctx, base);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x825d4d54
	if (ctx.cr6.eq) goto loc_825D4D54;
	// bl 0x821fe708
	ctx.lr = 0x825D4D4C;
	sub_821FE708(ctx, base);
	// lwz r11,24(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 24);
	// b 0x825d4d58
	goto loc_825D4D58;
loc_825D4D54:
	// li r11,0
	ctx.r11.s64 = 0;
loc_825D4D58:
	// lwz r10,0(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4D78);
PPC_WEAK_FUNC(sub_825D4D78) { __imp__sub_825D4D78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4D78) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d3898
	sub_825D3898(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4DA8);
PPC_WEAK_FUNC(sub_825D4DA8) { __imp__sub_825D4DA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4DA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// addi r11,r11,14644
	ctx.r11.s64 = ctx.r11.s64 + 14644;
	// lbz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 4);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4DC0);
PPC_WEAK_FUNC(sub_825D4DC0) { __imp__sub_825D4DC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4DC0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r4)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 8);
	// mr r10,r3
	ctx.r10.u64 = ctx.r3.u64;
	// lfs f12,8(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f12.f64 = double(temp.f32);
	// lfs f9,20(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	ctx.f9.f64 = double(temp.f32);
	// lfs f0,0(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f11,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f11.f64 = double(temp.f32);
	// lfs f10,16(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f10.f64 = double(temp.f32);
	// lfs f1,24(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	ctx.f1.f64 = double(temp.f32);
	// stfs f9,104(r1)
	temp.f32 = float(ctx.f9.f64);
	PPC_STORE_U32(ctx.r1.u32 + 104, temp.u32);
	// lwz r9,104(r1)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 104);
	// stfs f12,88(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// lwz r11,88(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// rldicr r6,r9,32,63
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r9.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// ld r3,80(r1)
	ctx.r3.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
	// stfs f11,96(r1)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r1.u32 + 96, temp.u32);
	// rldicr r4,r11,32,63
	ctx.r4.u64 = __builtin_rotateleft64(ctx.r11.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// stfs f10,100(r1)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r1.u32 + 100, temp.u32);
	// ld r5,96(r1)
	ctx.r5.u64 = PPC_LOAD_U64(ctx.r1.u32 + 96);
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
	// bctrl 
	ctx.lr = 0x825D4E28;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4E38);
PPC_WEAK_FUNC(sub_825D4E38) { __imp__sub_825D4E38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4E38) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stfd f31,-24(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f31.u64);
	// stwu r1,-176(r1)
	ea = -176 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r4)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 8);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lfs f31,56(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 56);
	ctx.f31.f64 = double(temp.f32);
	// lwz r10,60(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 60);
	// lfs f0,0(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f12.f64 = double(temp.f32);
	// lfs f11,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f11.f64 = double(temp.f32);
	// lfs f10,16(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f10.f64 = double(temp.f32);
	// stw r10,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r10.u32);
	// lfs f9,20(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	ctx.f9.f64 = double(temp.f32);
	// lfs f8,24(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	ctx.f8.f64 = double(temp.f32);
	// lfs f7,28(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	ctx.f7.f64 = double(temp.f32);
	// lfs f6,32(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 32);
	ctx.f6.f64 = double(temp.f32);
	// lfs f5,36(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	ctx.f5.f64 = double(temp.f32);
	// lfs f4,40(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 40);
	ctx.f4.f64 = double(temp.f32);
	// lfs f3,44(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 44);
	ctx.f3.f64 = double(temp.f32);
	// lfs f2,48(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 48);
	ctx.f2.f64 = double(temp.f32);
	// lfs f1,52(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 52);
	ctx.f1.f64 = double(temp.f32);
	// stfs f31,136(r1)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 136, temp.u32);
	// lwz r11,136(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 136);
	// stfs f12,120(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 120, temp.u32);
	// stfs f0,112(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 112, temp.u32);
	// stfs f13,116(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 116, temp.u32);
	// ld r3,112(r1)
	ctx.r3.u64 = PPC_LOAD_U64(ctx.r1.u32 + 112);
	// stfs f2,128(r1)
	temp.f32 = float(ctx.f2.f64);
	PPC_STORE_U32(ctx.r1.u32 + 128, temp.u32);
	// stw r11,88(r1)
	PPC_STORE_U32(ctx.r1.u32 + 88, ctx.r11.u32);
	// stfs f1,132(r1)
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r1.u32 + 132, temp.u32);
	// lwz r11,120(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 120);
	// stfs f9,120(r1)
	temp.f32 = float(ctx.f9.f64);
	PPC_STORE_U32(ctx.r1.u32 + 120, temp.u32);
	// stfs f11,112(r1)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r1.u32 + 112, temp.u32);
	// rldicr r4,r11,32,63
	ctx.r4.u64 = __builtin_rotateleft64(ctx.r11.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// lwz r11,120(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 120);
	// stfs f6,120(r1)
	temp.f32 = float(ctx.f6.f64);
	PPC_STORE_U32(ctx.r1.u32 + 120, temp.u32);
	// rldicr r6,r11,32,63
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r11.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// lwz r11,120(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 120);
	// stfs f3,120(r1)
	temp.f32 = float(ctx.f3.f64);
	PPC_STORE_U32(ctx.r1.u32 + 120, temp.u32);
	// rldicr r8,r11,32,63
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r11.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// lwz r11,120(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 120);
	// stfs f10,116(r1)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r1.u32 + 116, temp.u32);
	// ld r5,112(r1)
	ctx.r5.u64 = PPC_LOAD_U64(ctx.r1.u32 + 112);
	// rldicr r10,r11,32,63
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// ld r11,128(r1)
	ctx.r11.u64 = PPC_LOAD_U64(ctx.r1.u32 + 128);
	// stfs f8,112(r1)
	temp.f32 = float(ctx.f8.f64);
	PPC_STORE_U32(ctx.r1.u32 + 112, temp.u32);
	// stfs f7,116(r1)
	temp.f32 = float(ctx.f7.f64);
	PPC_STORE_U32(ctx.r1.u32 + 116, temp.u32);
	// ld r7,112(r1)
	ctx.r7.u64 = PPC_LOAD_U64(ctx.r1.u32 + 112);
	// stfs f5,112(r1)
	temp.f32 = float(ctx.f5.f64);
	PPC_STORE_U32(ctx.r1.u32 + 112, temp.u32);
	// stfs f4,116(r1)
	temp.f32 = float(ctx.f4.f64);
	PPC_STORE_U32(ctx.r1.u32 + 116, temp.u32);
	// ld r9,112(r1)
	ctx.r9.u64 = PPC_LOAD_U64(ctx.r1.u32 + 112);
	// std r11,80(r1)
	PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r11.u64);
	// mtctr r31
	ctx.ctr.u64 = ctx.r31.u64;
	// bctrl 
	ctx.lr = 0x825D4F1C;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// addi r1,r1,176
	ctx.r1.s64 = ctx.r1.s64 + 176;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f31,-24(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4F38);
PPC_WEAK_FUNC(sub_825D4F38) { __imp__sub_825D4F38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4F38) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,10120
	ctx.r3.s64 = ctx.r11.s64 + 10120;
	// b 0x825d4dc0
	sub_825D4DC0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4F48);
PPC_WEAK_FUNC(sub_825D4F48) { __imp__sub_825D4F48(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4F48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,12520
	ctx.r3.s64 = ctx.r11.s64 + 12520;
	// b 0x825d4e38
	sub_825D4E38(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4F58);
PPC_WEAK_FUNC(sub_825D4F58) { __imp__sub_825D4F58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4F58) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,19924(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 19924);
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x8221a300
	ctx.lr = 0x825D4F80;
	sub_8221A300(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D4FA0);
PPC_WEAK_FUNC(sub_825D4FA0) { __imp__sub_825D4FA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D4FA0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// lwz r7,8(r3)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,23772
	ctx.r10.s64 = 1557921792;
	// addi r11,r11,18316
	ctx.r11.s64 = ctx.r11.s64 + 18316;
	// ori r6,r10,64167
	ctx.r6.u64 = ctx.r10.u64 | 64167;
	// lwz r8,0(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// mulld r10,r8,r6
	ctx.r10.s64 = static_cast<int64_t>(ctx.r8.u64 * ctx.r6.u64);
	// lwz r8,0(r7)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
	// add r10,r10,r9
	ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
	// rotlwi r9,r10,0
	ctx.r9.u64 = __builtin_rotateleft32(ctx.r10.u32, 0);
	// rldicl r10,r10,32,32
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u64, 32) & 0xFFFFFFFF;
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// clrlwi r9,r9,9
	ctx.r9.u64 = ctx.r9.u32 & 0x7FFFFF;
	// stw r10,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// std r9,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r9.u64);
	// lfd f0,-16(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f0.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// fcfid f0,f0
	ctx.f0.f64 = double(ctx.f0.s64);
	// frsp f13,f0
	ctx.f13.f64 = double(float(ctx.f0.f64));
	// lfs f0,-27544(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -27544);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
	// stfs f0,0(r8)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D5000);
PPC_WEAK_FUNC(sub_825D5000) { __imp__sub_825D5000(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D5000) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// lwz r7,8(r3)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,23772
	ctx.r10.s64 = 1557921792;
	// addi r11,r11,18316
	ctx.r11.s64 = ctx.r11.s64 + 18316;
	// ori r6,r10,64167
	ctx.r6.u64 = ctx.r10.u64 | 64167;
	// lwz r8,0(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// mulld r10,r8,r6
	ctx.r10.s64 = static_cast<int64_t>(ctx.r8.u64 * ctx.r6.u64);
	// lwz r8,0(r7)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
	// add r10,r10,r9
	ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
	// rotlwi r9,r10,0
	ctx.r9.u64 = __builtin_rotateleft32(ctx.r10.u32, 0);
	// rldicl r10,r10,32,32
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u64, 32) & 0xFFFFFFFF;
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// clrlwi r9,r9,17
	ctx.r9.u64 = ctx.r9.u32 & 0x7FFF;
	// stw r10,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
	// stw r9,0(r8)
	PPC_STORE_U32(ctx.r8.u32 + 0, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D5048);
PPC_WEAK_FUNC(sub_825D5048) { __imp__sub_825D5048(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D5048) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f0,-680(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -680);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f13,3444(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3444);
	ctx.f13.f64 = double(temp.f32);
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// bge cr6,0x825d5078
	if (!ctx.cr6.lt) goto loc_825D5078;
	// lis r10,-32001
	ctx.r10.s64 = -2097217536;
	// lwz r10,-660(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -660);
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// blr 
	return;
loc_825D5078:
	// lis r10,-32001
	ctx.r10.s64 = -2097217536;
	// lwz r10,-664(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -664);
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D5088);
PPC_WEAK_FUNC(sub_825D5088) { __imp__sub_825D5088(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D5088) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32001
	ctx.r10.s64 = -2097217536;
	// lwz r9,8(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lfs f0,-680(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -680);
	ctx.f0.f64 = double(temp.f32);
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stfs f0,0(r9)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// lis r9,-32001
	ctx.r9.s64 = -2097217536;
	// lwz r9,-660(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + -660);
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// lwz r11,-664(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -664);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D50C0);
PPC_WEAK_FUNC(sub_825D50C0) { __imp__sub_825D50C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D50C0) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32057
	ctx.r11.s64 = -2100887552;
	// lwz r11,-15708(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -15708);
	// lwz r10,0(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D50D8);
PPC_WEAK_FUNC(sub_825D50D8) { __imp__sub_825D50D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D50D8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32057
	ctx.r11.s64 = -2100887552;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r11,r11,-15708
	ctx.r11.s64 = ctx.r11.s64 + -15708;
	// lfs f0,8(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D50F8);
PPC_WEAK_FUNC(sub_825D50F8) { __imp__sub_825D50F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D50F8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r9,23772
	ctx.r9.s64 = 1557921792;
	// addi r11,r11,18316
	ctx.r11.s64 = ctx.r11.s64 + 18316;
	// ori r6,r9,64167
	ctx.r6.u64 = ctx.r9.u64 | 64167;
	// lfs f0,0(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,4(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lwz r7,0(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// fsubs f12,f13,f0
	ctx.f12.f64 = double(float(ctx.f13.f64 - ctx.f0.f64));
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// mulld r9,r7,r6
	ctx.r9.s64 = static_cast<int64_t>(ctx.r7.u64 * ctx.r6.u64);
	// add r9,r9,r8
	ctx.r9.u64 = ctx.r9.u64 + ctx.r8.u64;
	// lwz r8,8(r10)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 8);
	// rotlwi r10,r9,0
	ctx.r10.u64 = __builtin_rotateleft32(ctx.r9.u32, 0);
	// rldicl r9,r9,32,32
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u64, 32) & 0xFFFFFFFF;
	// clrlwi r7,r10,9
	ctx.r7.u64 = ctx.r10.u32 & 0x7FFFFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// std r7,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r7.u64);
	// lfd f13,-16(r1)
	ctx.f13.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// fcfid f13,f13
	ctx.f13.f64 = double(ctx.f13.s64);
	// frsp f11,f13
	ctx.f11.f64 = double(float(ctx.f13.f64));
	// lfs f13,-27544(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -27544);
	ctx.f13.f64 = double(temp.f32);
	// fmuls f13,f11,f13
	ctx.f13.f64 = double(float(ctx.f11.f64 * ctx.f13.f64));
	// fmadds f0,f13,f12,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 * ctx.f12.f64 + ctx.f0.f64));
	// stfs f0,0(r8)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D5168);
PPC_WEAK_FUNC(sub_825D5168) { __imp__sub_825D5168(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D5168) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r9,23772
	ctx.r9.s64 = 1557921792;
	// addi r11,r11,18316
	ctx.r11.s64 = ctx.r11.s64 + 18316;
	// ori r6,r9,64167
	ctx.r6.u64 = ctx.r9.u64 | 64167;
	// lwz r7,0(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// mulld r9,r7,r6
	ctx.r9.s64 = static_cast<int64_t>(ctx.r7.u64 * ctx.r6.u64);
	// lwz r7,4(r10)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// lwz r6,8(r10)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r10.u32 + 8);
	// add r9,r9,r8
	ctx.r9.u64 = ctx.r9.u64 + ctx.r8.u64;
	// lwz r8,0(r10)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// rotlwi r10,r9,0
	ctx.r10.u64 = __builtin_rotateleft32(ctx.r9.u32, 0);
	// rldicl r9,r9,32,32
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u64, 32) & 0xFFFFFFFF;
	// clrldi r5,r10,33
	ctx.r5.u64 = ctx.r10.u64 & 0x7FFFFFFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// subf r11,r8,r7
	ctx.r11.u64 = ctx.r7.u64 - ctx.r8.u64;
	// extsw r11,r11
	ctx.r11.s64 = ctx.r11.s32;
	// mulld r11,r5,r11
	ctx.r11.s64 = static_cast<int64_t>(ctx.r5.u64 * ctx.r11.u64);
	// rldicl r11,r11,33,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u64, 33) & 0x1FFFFFFFF;
	// rotlwi r11,r11,0
	ctx.r11.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// add r11,r11,r8
	ctx.r11.u64 = ctx.r11.u64 + ctx.r8.u64;
	// stw r11,0(r6)
	PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D51D0);
PPC_WEAK_FUNC(sub_825D51D0) { __imp__sub_825D51D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D51D0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// li r8,4
	ctx.r8.s64 = 4;
	// li r7,0
	ctx.r7.s64 = 0;
	// addi r6,r1,80
	ctx.r6.s64 = ctx.r1.s64 + 80;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lwz r30,12(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// lfs f3,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f3.f64 = double(temp.f32);
	// bl 0x82363158
	ctx.lr = 0x825D520C;
	sub_82363158(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stfs f1,0(r30)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r30.u32 + 0, temp.u32);
	// lbz r10,80(r1)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r1.u32 + 80);
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D5238);
PPC_WEAK_FUNC(sub_825D5238) { __imp__sub_825D5238(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D5238) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825e7e18
	sub_825E7E18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D5248);
PPC_WEAK_FUNC(sub_825D5248) { __imp__sub_825D5248(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D5248) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f0,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lwz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lfs f11,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f11.f64 = double(temp.f32);
	// fsubs f0,f0,f11
	ctx.f0.f64 = double(float(ctx.f0.f64 - ctx.f11.f64));
	// lfs f13,0(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f12.f64 = double(temp.f32);
	// fsubs f13,f13,f12
	ctx.f13.f64 = double(float(ctx.f13.f64 - ctx.f12.f64));
	// fmuls f0,f0,f0
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f0.f64));
	// fmadds f0,f13,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 * ctx.f13.f64 + ctx.f0.f64));
	// fsqrts f0,f0
	ctx.f0.f64 = double(float(sqrt(ctx.f0.f64)));
	// stfs f0,0(r10)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D5280);
PPC_WEAK_FUNC(sub_825D5280) { __imp__sub_825D5280(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D5280) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f0,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lwz r10,24(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// lfs f13,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f12.f64 = double(temp.f32);
	// lfs f11,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f11.f64 = double(temp.f32);
	// lfs f10,16(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f10.f64 = double(temp.f32);
	// lfs f9,20(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	ctx.f9.f64 = double(temp.f32);
	// stfs f0,-32(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -32, temp.u32);
	// stfs f13,-28(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + -28, temp.u32);
	// stfs f12,-24(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + -24, temp.u32);
	// addi r11,r1,-32
	ctx.r11.s64 = ctx.r1.s64 + -32;
	// stfs f9,-8(r1)
	temp.f32 = float(ctx.f9.f64);
	PPC_STORE_U32(ctx.r1.u32 + -8, temp.u32);
	// stfs f11,-16(r1)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r1.u32 + -16, temp.u32);
	// stfs f10,-12(r1)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r1.u32 + -12, temp.u32);
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,-16
	ctx.r11.s64 = ctx.r1.s64 + -16;
	// lvx128 v13,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,-16
	ctx.r11.s64 = ctx.r1.s64 + -16;
	// vsubfp v0,v0,v13
	ctx.fpscr.enableFlushModeUnconditional();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_sub_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v13.f32)));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lfs f13,-12(r1)
	ctx.fpscr.disableFlushModeUnconditional();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + -12);
	ctx.f13.f64 = double(temp.f32);
	// fmuls f12,f13,f13
	ctx.f12.f64 = double(float(ctx.f13.f64 * ctx.f13.f64));
	// lfs f0,-16(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + -16);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,-8(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	ctx.f13.f64 = double(temp.f32);
	// fmadds f0,f0,f0,f12
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f0.f64 + ctx.f12.f64));
	// fmadds f0,f13,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 * ctx.f13.f64 + ctx.f0.f64));
	// fsqrts f0,f0
	ctx.f0.f64 = double(float(sqrt(ctx.f0.f64)));
	// stfs f0,0(r10)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D52F8);
PPC_WEAK_FUNC(sub_825D52F8) { __imp__sub_825D52F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D52F8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// lfs f3,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f3.f64 = double(temp.f32);
	// lfs f4,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f4.f64 = double(temp.f32);
	// b 0x825d29c8
	sub_825D29C8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D5318);
PPC_WEAK_FUNC(sub_825D5318) { __imp__sub_825D5318(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D5318) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// b 0x825d2a60
	sub_825D2A60(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D5330);
PPC_WEAK_FUNC(sub_825D5330) { __imp__sub_825D5330(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D5330) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r5,16(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lfs f1,12(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f1.f64 = double(temp.f32);
	// lwz r6,20(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lfs f0,0(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f12.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f12,88(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// bl 0x8226fbf8
	ctx.lr = 0x825D5374;
	sub_8226FBF8(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D5390);
PPC_WEAK_FUNC(sub_825D5390) { __imp__sub_825D5390(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D5390) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r10,32(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 32);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// cntlzw r10,r10
	ctx.r10.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// rlwinm r10,r10,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// xori r31,r10,1
	ctx.r31.u64 = ctx.r10.u64 ^ 1;
	// lwz r9,24(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// lwz r10,28(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stb r31,87(r1)
	PPC_STORE_U8(ctx.r1.u32 + 87, ctx.r31.u8);
	// bl 0x825d2568
	ctx.lr = 0x825D53DC;
	sub_825D2568(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D53F0);
PPC_WEAK_FUNC(sub_825D53F0) { __imp__sub_825D53F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D53F0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r9,1
	ctx.r9.s64 = 1;
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// slw r11,r9,r11
	ctx.r11.u64 = ctx.r11.u8 & 0x20 ? 0 : (ctx.r9.u32 << (ctx.r11.u8 & 0x3F));
	// lwz r9,0(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// or r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 | ctx.r9.u64;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D5418);
PPC_WEAK_FUNC(sub_825D5418) { __imp__sub_825D5418(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D5418) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r9,1
	ctx.r9.s64 = 1;
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// slw r11,r9,r11
	ctx.r11.u64 = ctx.r11.u8 & 0x20 ? 0 : (ctx.r9.u32 << (ctx.r11.u8 & 0x3F));
	// lwz r9,0(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// andc r11,r9,r11
	ctx.r11.u64 = ctx.r9.u64 & ~ctx.r11.u64;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D5440);
PPC_WEAK_FUNC(sub_825D5440) { __imp__sub_825D5440(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D5440) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x8235c370
	ctx.lr = 0x825D5460;
	sub_8235C370(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D5480);
PPC_WEAK_FUNC(sub_825D5480) { __imp__sub_825D5480(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D5480) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// addi r3,r11,-1640
	ctx.r3.s64 = ctx.r11.s64 + -1640;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x8221fd88
	ctx.lr = 0x825D54A8;
	sub_8221FD88(ctx, base);
	// li r4,1
	ctx.r4.s64 = 1;
	// bl 0x821ff5a8
	ctx.lr = 0x825D54B0;
	sub_821FF5A8(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D54D0);
PPC_WEAK_FUNC(sub_825D54D0) { __imp__sub_825D54D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D54D0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// bne cr6,0x825d5500
	if (!ctx.cr6.eq) goto loc_825D5500;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f1,2612(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2612);
	ctx.f1.f64 = double(temp.f32);
	// b 0x825d5508
	goto loc_825D5508;
loc_825D5500:
	// li r4,1
	ctx.r4.s64 = 1;
	// bl 0x821ff5a8
	ctx.lr = 0x825D5508;
	sub_821FF5A8(ctx, base);
loc_825D5508:
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D5528);
PPC_WEAK_FUNC(sub_825D5528) { __imp__sub_825D5528(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D5528) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x825d2d80
	ctx.lr = 0x825D554C;
	sub_825D2D80(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D5568);
PPC_WEAK_FUNC(sub_825D5568) { __imp__sub_825D5568(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D5568) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x825d2e00
	ctx.lr = 0x825D558C;
	sub_825D2E00(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D55A8);
PPC_WEAK_FUNC(sub_825D55A8) { __imp__sub_825D55A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D55A8) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// lwz r10,4(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825d55d0
	if (!ctx.cr6.eq) goto loc_825D55D0;
loc_825D55BC:
	// li r11,0
	ctx.r11.s64 = 0;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
loc_825D55D0:
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x825d55bc
	if (ctx.cr6.eq) goto loc_825D55BC;
loc_825D55D8:
	// lbz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// lbz r8,0(r10)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// subf r8,r8,r9
	ctx.r8.u64 = ctx.r9.u64 - ctx.r8.u64;
	// beq cr6,0x825d55fc
	if (ctx.cr6.eq) goto loc_825D55FC;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// cmpwi cr6,r8,0
	ctx.cr6.compare<int32_t>(ctx.r8.s32, 0, ctx.xer);
	// beq cr6,0x825d55d8
	if (ctx.cr6.eq) goto loc_825D55D8;
loc_825D55FC:
	// cntlzw r11,r8
	ctx.r11.u64 = ctx.r8.u32 == 0 ? 32 : __builtin_clz(ctx.r8.u32);
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D5618);
PPC_WEAK_FUNC(sub_825D5618) { __imp__sub_825D5618(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D5618) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lfs f0,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lwz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lfs f13,2612(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 2612);
	ctx.f13.f64 = double(temp.f32);
	// lis r10,-32255
	ctx.r10.s64 = -2113863680;
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// lfs f0,-27840(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -27840);
	ctx.f0.f64 = double(temp.f32);
	// bge cr6,0x825d5654
	if (!ctx.cr6.lt) goto loc_825D5654;
loc_825D5640:
	// lfs f12,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f12.f64 = double(temp.f32);
	// fadds f12,f12,f0
	ctx.f12.f64 = double(float(ctx.f12.f64 + ctx.f0.f64));
	// stfs f12,0(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// fcmpu cr6,f12,f13
	ctx.cr6.compare(ctx.f12.f64, ctx.f13.f64);
	// blt cr6,0x825d5640
	if (ctx.cr6.lt) goto loc_825D5640;
loc_825D5654:
	// lfs f13,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// fcmpu cr6,f13,f0
	ctx.cr6.compare(ctx.f13.f64, ctx.f0.f64);
	// blelr cr6
	if (!ctx.cr6.gt) return;
loc_825D5660:
	// lfs f13,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// fsubs f13,f13,f0
	ctx.f13.f64 = double(float(ctx.f13.f64 - ctx.f0.f64));
	// stfs f13,0(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// fcmpu cr6,f13,f0
	ctx.cr6.compare(ctx.f13.f64, ctx.f0.f64);
	// bgt cr6,0x825d5660
	if (ctx.cr6.gt) goto loc_825D5660;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D5678);
PPC_WEAK_FUNC(sub_825D5678) { __imp__sub_825D5678(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D5678) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d37b0
	sub_825D37B0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D5688);
PPC_WEAK_FUNC(sub_825D5688) { __imp__sub_825D5688(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D5688) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D56A8);
PPC_WEAK_FUNC(sub_825D56A8) { __imp__sub_825D56A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D56A8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x825d2fb0
	ctx.lr = 0x825D56CC;
	sub_825D2FB0(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D56F0);
PPC_WEAK_FUNC(sub_825D56F0) { __imp__sub_825D56F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D56F0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x825d3090
	sub_825D3090(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D5708);
PPC_WEAK_FUNC(sub_825D5708) { __imp__sub_825D5708(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D5708) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r7,1
	ctx.r7.s64 = 1;
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r9,12(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r8,8(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// subf r8,r10,r8
	ctx.r8.u64 = ctx.r8.u64 - ctx.r10.u64;
	// slw r6,r9,r10
	ctx.r6.u64 = ctx.r10.u8 & 0x20 ? 0 : (ctx.r9.u32 << (ctx.r10.u8 & 0x3F));
	// addi r9,r8,1
	ctx.r9.s64 = ctx.r8.s64 + 1;
	// slw r9,r7,r9
	ctx.r9.u64 = ctx.r9.u8 & 0x20 ? 0 : (ctx.r7.u32 << (ctx.r9.u8 & 0x3F));
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// slw r10,r9,r10
	ctx.r10.u64 = ctx.r10.u8 & 0x20 ? 0 : (ctx.r9.u32 << (ctx.r10.u8 & 0x3F));
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// andc r10,r9,r10
	ctx.r10.u64 = ctx.r9.u64 & ~ctx.r10.u64;
	// or r10,r10,r6
	ctx.r10.u64 = ctx.r10.u64 | ctx.r6.u64;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D5750);
PPC_WEAK_FUNC(sub_825D5750) { __imp__sub_825D5750(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D5750) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8225a610
	sub_8225A610(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D5760);
PPC_WEAK_FUNC(sub_825D5760) { __imp__sub_825D5760(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D5760) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mr r11,r4
	ctx.r11.u64 = ctx.r4.u64;
	// mr r8,r3
	ctx.r8.u64 = ctx.r3.u64;
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,8(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// addi r10,r10,4
	ctx.r10.s64 = ctx.r10.s64 + 4;
	// rlwinm r7,r10,2,0,29
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwz r10,4(r9)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r9.u32 + 4);
	// lwz r3,0(r9)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
	// lfs f0,8(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// stwx r10,r7,r11
	PPC_STORE_U32(ctx.r7.u32 + ctx.r11.u32, ctx.r10.u32);
	// lfs f13,4(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,0(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f12.f64 = double(temp.f32);
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// addi r10,r10,2
	ctx.r10.s64 = ctx.r10.s64 + 2;
	// rlwinm r10,r10,4,0,27
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
	// add r10,r10,r11
	ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
	// stfs f12,0(r10)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// stfs f13,4(r10)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// stfs f0,8(r10)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + 8, temp.u32);
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// addi r9,r10,1
	ctx.r9.s64 = ctx.r10.s64 + 1;
	// addi r10,r10,2
	ctx.r10.s64 = ctx.r10.s64 + 2;
	// rlwinm r10,r10,4,0,27
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
	// add r4,r10,r11
	ctx.r4.u64 = ctx.r10.u64 + ctx.r11.u64;
	// stw r9,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r9.u32);
	// addi r10,r9,4
	ctx.r10.s64 = ctx.r9.s64 + 4;
	// rlwinm r6,r10,2,0,29
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwz r10,8(r7)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r7.u32 + 8);
	// lfs f0,8(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// stwx r10,r6,r11
	PPC_STORE_U32(ctx.r6.u32 + ctx.r11.u32, ctx.r10.u32);
	// lfs f13,4(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,0(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f12.f64 = double(temp.f32);
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// addi r10,r10,2
	ctx.r10.s64 = ctx.r10.s64 + 2;
	// rlwinm r10,r10,4,0,27
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
	// add r10,r10,r11
	ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
	// stfs f12,0(r10)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// stfs f13,4(r10)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// stfs f0,8(r10)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + 8, temp.u32);
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// addi r9,r10,2
	ctx.r9.s64 = ctx.r10.s64 + 2;
	// rlwinm r10,r9,4,0,27
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
	// add r5,r10,r11
	ctx.r5.u64 = ctx.r10.u64 + ctx.r11.u64;
	// stw r7,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r7.u32);
	// mtctr r8
	ctx.ctr.u64 = ctx.r8.u64;
	// bctr 
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D5840);
PPC_WEAK_FUNC(sub_825D5840) { __imp__sub_825D5840(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D5840) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,14632
	ctx.r4.s64 = ctx.r11.s64 + 14632;
	// addi r3,r10,-31984
	ctx.r3.s64 = ctx.r10.s64 + -31984;
	// bl 0x82845600
	ctx.lr = 0x825D5860;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,20384
	ctx.r4.s64 = ctx.r11.s64 + 20384;
	// addi r3,r10,-32008
	ctx.r3.s64 = ctx.r10.s64 + -32008;
	// bl 0x82845600
	ctx.lr = 0x825D5874;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,20480
	ctx.r4.s64 = ctx.r11.s64 + 20480;
	// addi r3,r10,-32028
	ctx.r3.s64 = ctx.r10.s64 + -32028;
	// bl 0x82845600
	ctx.lr = 0x825D5888;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,14712
	ctx.r4.s64 = ctx.r11.s64 + 14712;
	// addi r3,r10,-32044
	ctx.r3.s64 = ctx.r10.s64 + -32044;
	// bl 0x82845600
	ctx.lr = 0x825D589C;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,14744
	ctx.r4.s64 = ctx.r11.s64 + 14744;
	// addi r3,r10,-32064
	ctx.r3.s64 = ctx.r10.s64 + -32064;
	// bl 0x82845600
	ctx.lr = 0x825D58B0;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,14832
	ctx.r4.s64 = ctx.r11.s64 + 14832;
	// addi r3,r10,-32084
	ctx.r3.s64 = ctx.r10.s64 + -32084;
	// bl 0x82845600
	ctx.lr = 0x825D58C4;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,14888
	ctx.r4.s64 = ctx.r11.s64 + 14888;
	// addi r3,r10,-32100
	ctx.r3.s64 = ctx.r10.s64 + -32100;
	// bl 0x82845600
	ctx.lr = 0x825D58D8;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,15000
	ctx.r4.s64 = ctx.r11.s64 + 15000;
	// addi r3,r10,-32120
	ctx.r3.s64 = ctx.r10.s64 + -32120;
	// bl 0x82845600
	ctx.lr = 0x825D58EC;
	sub_82845600(ctx, base);
	// lis r11,-32094
	ctx.r11.s64 = -2103312384;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,3728
	ctx.r4.s64 = ctx.r11.s64 + 3728;
	// addi r3,r10,-32140
	ctx.r3.s64 = ctx.r10.s64 + -32140;
	// bl 0x82845600
	ctx.lr = 0x825D5900;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,12808
	ctx.r4.s64 = ctx.r11.s64 + 12808;
	// addi r3,r10,-32156
	ctx.r3.s64 = ctx.r10.s64 + -32156;
	// bl 0x82845600
	ctx.lr = 0x825D5914;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,20552
	ctx.r4.s64 = ctx.r11.s64 + 20552;
	// addi r3,r10,-32176
	ctx.r3.s64 = ctx.r10.s64 + -32176;
	// bl 0x82845600
	ctx.lr = 0x825D5928;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,20616
	ctx.r4.s64 = ctx.r11.s64 + 20616;
	// addi r3,r10,-32204
	ctx.r3.s64 = ctx.r10.s64 + -32204;
	// bl 0x82845600
	ctx.lr = 0x825D593C;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,15112
	ctx.r4.s64 = ctx.r11.s64 + 15112;
	// addi r3,r10,-32216
	ctx.r3.s64 = ctx.r10.s64 + -32216;
	// bl 0x82845600
	ctx.lr = 0x825D5950;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,20672
	ctx.r4.s64 = ctx.r11.s64 + 20672;
	// addi r3,r10,-32232
	ctx.r3.s64 = ctx.r10.s64 + -32232;
	// bl 0x82845600
	ctx.lr = 0x825D5964;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,20696
	ctx.r4.s64 = ctx.r11.s64 + 20696;
	// addi r3,r10,-32248
	ctx.r3.s64 = ctx.r10.s64 + -32248;
	// bl 0x82845600
	ctx.lr = 0x825D5978;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,20728
	ctx.r4.s64 = ctx.r11.s64 + 20728;
	// addi r3,r10,-32280
	ctx.r3.s64 = ctx.r10.s64 + -32280;
	// bl 0x82845600
	ctx.lr = 0x825D598C;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,20840
	ctx.r4.s64 = ctx.r11.s64 + 20840;
	// addi r3,r10,-32312
	ctx.r3.s64 = ctx.r10.s64 + -32312;
	// bl 0x82845600
	ctx.lr = 0x825D59A0;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,20944
	ctx.r4.s64 = ctx.r11.s64 + 20944;
	// addi r3,r10,-32340
	ctx.r3.s64 = ctx.r10.s64 + -32340;
	// bl 0x82845600
	ctx.lr = 0x825D59B4;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,15136
	ctx.r4.s64 = ctx.r11.s64 + 15136;
	// addi r3,r10,-32348
	ctx.r3.s64 = ctx.r10.s64 + -32348;
	// bl 0x82845600
	ctx.lr = 0x825D59C8;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,15216
	ctx.r4.s64 = ctx.r11.s64 + 15216;
	// addi r3,r10,-32356
	ctx.r3.s64 = ctx.r10.s64 + -32356;
	// bl 0x82845600
	ctx.lr = 0x825D59DC;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,15296
	ctx.r4.s64 = ctx.r11.s64 + 15296;
	// addi r3,r10,-32360
	ctx.r3.s64 = ctx.r10.s64 + -32360;
	// bl 0x82845600
	ctx.lr = 0x825D59F0;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,15376
	ctx.r4.s64 = ctx.r11.s64 + 15376;
	// addi r3,r10,-32368
	ctx.r3.s64 = ctx.r10.s64 + -32368;
	// bl 0x82845600
	ctx.lr = 0x825D5A04;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,15464
	ctx.r4.s64 = ctx.r11.s64 + 15464;
	// addi r3,r10,-32376
	ctx.r3.s64 = ctx.r10.s64 + -32376;
	// bl 0x82845600
	ctx.lr = 0x825D5A18;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,15544
	ctx.r4.s64 = ctx.r11.s64 + 15544;
	// addi r3,r10,-32396
	ctx.r3.s64 = ctx.r10.s64 + -32396;
	// bl 0x82845600
	ctx.lr = 0x825D5A2C;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,15560
	ctx.r4.s64 = ctx.r11.s64 + 15560;
	// addi r3,r10,-32436
	ctx.r3.s64 = ctx.r10.s64 + -32436;
	// bl 0x82845600
	ctx.lr = 0x825D5A40;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,15648
	ctx.r4.s64 = ctx.r11.s64 + 15648;
	// addi r3,r10,-32448
	ctx.r3.s64 = ctx.r10.s64 + -32448;
	// bl 0x82845600
	ctx.lr = 0x825D5A54;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,15696
	ctx.r4.s64 = ctx.r11.s64 + 15696;
	// addi r3,r10,-32468
	ctx.r3.s64 = ctx.r10.s64 + -32468;
	// bl 0x82845600
	ctx.lr = 0x825D5A68;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,20280
	ctx.r4.s64 = ctx.r11.s64 + 20280;
	// addi r3,r10,-32496
	ctx.r3.s64 = ctx.r10.s64 + -32496;
	// bl 0x82845600
	ctx.lr = 0x825D5A7C;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,15832
	ctx.r4.s64 = ctx.r11.s64 + 15832;
	// addi r3,r10,-32520
	ctx.r3.s64 = ctx.r10.s64 + -32520;
	// bl 0x82845600
	ctx.lr = 0x825D5A90;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,15856
	ctx.r4.s64 = ctx.r11.s64 + 15856;
	// addi r3,r10,-32540
	ctx.r3.s64 = ctx.r10.s64 + -32540;
	// bl 0x82845600
	ctx.lr = 0x825D5AA4;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,15984
	ctx.r4.s64 = ctx.r11.s64 + 15984;
	// addi r3,r10,-32560
	ctx.r3.s64 = ctx.r10.s64 + -32560;
	// bl 0x82845600
	ctx.lr = 0x825D5AB8;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,12816
	ctx.r4.s64 = ctx.r11.s64 + 12816;
	// addi r3,r10,-32580
	ctx.r3.s64 = ctx.r10.s64 + -32580;
	// bl 0x82845600
	ctx.lr = 0x825D5ACC;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// addi r4,r11,16112
	ctx.r4.s64 = ctx.r11.s64 + 16112;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r3,r11,-32612
	ctx.r3.s64 = ctx.r11.s64 + -32612;
	// bl 0x82845600
	ctx.lr = 0x825D5AE0;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,12824
	ctx.r4.s64 = ctx.r11.s64 + 12824;
	// addi r3,r10,-32644
	ctx.r3.s64 = ctx.r10.s64 + -32644;
	// bl 0x82845600
	ctx.lr = 0x825D5AF4;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,12832
	ctx.r4.s64 = ctx.r11.s64 + 12832;
	// addi r3,r10,-32680
	ctx.r3.s64 = ctx.r10.s64 + -32680;
	// bl 0x82845600
	ctx.lr = 0x825D5B08;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,12864
	ctx.r4.s64 = ctx.r11.s64 + 12864;
	// addi r3,r10,-32696
	ctx.r3.s64 = ctx.r10.s64 + -32696;
	// bl 0x82845600
	ctx.lr = 0x825D5B1C;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,12872
	ctx.r4.s64 = ctx.r11.s64 + 12872;
	// addi r3,r10,-32712
	ctx.r3.s64 = ctx.r10.s64 + -32712;
	// bl 0x82845600
	ctx.lr = 0x825D5B30;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16152
	ctx.r4.s64 = ctx.r11.s64 + 16152;
	// addi r3,r10,-32736
	ctx.r3.s64 = ctx.r10.s64 + -32736;
	// bl 0x82845600
	ctx.lr = 0x825D5B44;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,16184
	ctx.r4.s64 = ctx.r11.s64 + 16184;
	// addi r3,r10,32764
	ctx.r3.s64 = ctx.r10.s64 + 32764;
	// bl 0x82845600
	ctx.lr = 0x825D5B58;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21048
	ctx.r4.s64 = ctx.r11.s64 + 21048;
	// addi r3,r10,32724
	ctx.r3.s64 = ctx.r10.s64 + 32724;
	// bl 0x82845600
	ctx.lr = 0x825D5B6C;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,12880
	ctx.r4.s64 = ctx.r11.s64 + 12880;
	// addi r3,r10,32684
	ctx.r3.s64 = ctx.r10.s64 + 32684;
	// bl 0x82845600
	ctx.lr = 0x825D5B80;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,12928
	ctx.r4.s64 = ctx.r11.s64 + 12928;
	// addi r3,r10,32644
	ctx.r3.s64 = ctx.r10.s64 + 32644;
	// bl 0x82845600
	ctx.lr = 0x825D5B94;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,12968
	ctx.r4.s64 = ctx.r11.s64 + 12968;
	// addi r3,r10,32584
	ctx.r3.s64 = ctx.r10.s64 + 32584;
	// bl 0x82845600
	ctx.lr = 0x825D5BA8;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,12984
	ctx.r4.s64 = ctx.r11.s64 + 12984;
	// addi r3,r10,32556
	ctx.r3.s64 = ctx.r10.s64 + 32556;
	// bl 0x82845600
	ctx.lr = 0x825D5BBC;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,16216
	ctx.r4.s64 = ctx.r11.s64 + 16216;
	// addi r3,r10,32512
	ctx.r3.s64 = ctx.r10.s64 + 32512;
	// bl 0x82845600
	ctx.lr = 0x825D5BD0;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,16320
	ctx.r4.s64 = ctx.r11.s64 + 16320;
	// addi r3,r10,32476
	ctx.r3.s64 = ctx.r10.s64 + 32476;
	// bl 0x82845600
	ctx.lr = 0x825D5BE4;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21064
	ctx.r4.s64 = ctx.r11.s64 + 21064;
	// addi r3,r10,32444
	ctx.r3.s64 = ctx.r10.s64 + 32444;
	// bl 0x82845600
	ctx.lr = 0x825D5BF8;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21120
	ctx.r4.s64 = ctx.r11.s64 + 21120;
	// addi r3,r10,32412
	ctx.r3.s64 = ctx.r10.s64 + 32412;
	// bl 0x82845600
	ctx.lr = 0x825D5C0C;
	sub_82845600(ctx, base);
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// addi r3,r10,32380
	ctx.r3.s64 = ctx.r10.s64 + 32380;
	// addi r4,r11,21240
	ctx.r4.s64 = ctx.r11.s64 + 21240;
	// bl 0x82845600
	ctx.lr = 0x825D5C20;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21272
	ctx.r4.s64 = ctx.r11.s64 + 21272;
	// addi r3,r10,32352
	ctx.r3.s64 = ctx.r10.s64 + 32352;
	// bl 0x82845600
	ctx.lr = 0x825D5C34;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,16416
	ctx.r4.s64 = ctx.r11.s64 + 16416;
	// addi r3,r10,32332
	ctx.r3.s64 = ctx.r10.s64 + 32332;
	// bl 0x82845600
	ctx.lr = 0x825D5C48;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,13024
	ctx.r4.s64 = ctx.r11.s64 + 13024;
	// addi r3,r10,32312
	ctx.r3.s64 = ctx.r10.s64 + 32312;
	// bl 0x82845600
	ctx.lr = 0x825D5C5C;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,16496
	ctx.r4.s64 = ctx.r11.s64 + 16496;
	// addi r3,r10,32280
	ctx.r3.s64 = ctx.r10.s64 + 32280;
	// bl 0x82845600
	ctx.lr = 0x825D5C70;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,16576
	ctx.r4.s64 = ctx.r11.s64 + 16576;
	// addi r3,r10,32240
	ctx.r3.s64 = ctx.r10.s64 + 32240;
	// bl 0x82845600
	ctx.lr = 0x825D5C84;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,16712
	ctx.r4.s64 = ctx.r11.s64 + 16712;
	// addi r3,r10,32196
	ctx.r3.s64 = ctx.r10.s64 + 32196;
	// bl 0x82845600
	ctx.lr = 0x825D5C98;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,16792
	ctx.r4.s64 = ctx.r11.s64 + 16792;
	// addi r3,r10,32148
	ctx.r3.s64 = ctx.r10.s64 + 32148;
	// bl 0x82845600
	ctx.lr = 0x825D5CAC;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,16928
	ctx.r4.s64 = ctx.r11.s64 + 16928;
	// addi r3,r10,32104
	ctx.r3.s64 = ctx.r10.s64 + 32104;
	// bl 0x82845600
	ctx.lr = 0x825D5CC0;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,16984
	ctx.r4.s64 = ctx.r11.s64 + 16984;
	// addi r3,r10,32080
	ctx.r3.s64 = ctx.r10.s64 + 32080;
	// bl 0x82845600
	ctx.lr = 0x825D5CD4;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,17016
	ctx.r4.s64 = ctx.r11.s64 + 17016;
	// addi r3,r10,32060
	ctx.r3.s64 = ctx.r10.s64 + 32060;
	// bl 0x82845600
	ctx.lr = 0x825D5CE8;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,17048
	ctx.r4.s64 = ctx.r11.s64 + 17048;
	// addi r3,r10,32036
	ctx.r3.s64 = ctx.r10.s64 + 32036;
	// bl 0x82845600
	ctx.lr = 0x825D5CFC;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,13032
	ctx.r4.s64 = ctx.r11.s64 + 13032;
	// addi r3,r10,32012
	ctx.r3.s64 = ctx.r10.s64 + 32012;
	// bl 0x82845600
	ctx.lr = 0x825D5D10;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,17176
	ctx.r4.s64 = ctx.r11.s64 + 17176;
	// addi r3,r10,31976
	ctx.r3.s64 = ctx.r10.s64 + 31976;
	// bl 0x82845600
	ctx.lr = 0x825D5D24;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,17256
	ctx.r4.s64 = ctx.r11.s64 + 17256;
	// addi r3,r10,31936
	ctx.r3.s64 = ctx.r10.s64 + 31936;
	// bl 0x82845600
	ctx.lr = 0x825D5D38;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,17336
	ctx.r4.s64 = ctx.r11.s64 + 17336;
	// addi r3,r10,31900
	ctx.r3.s64 = ctx.r10.s64 + 31900;
	// bl 0x82845600
	ctx.lr = 0x825D5D4C;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21296
	ctx.r4.s64 = ctx.r11.s64 + 21296;
	// addi r3,r10,31880
	ctx.r3.s64 = ctx.r10.s64 + 31880;
	// bl 0x82845600
	ctx.lr = 0x825D5D60;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,17368
	ctx.r4.s64 = ctx.r11.s64 + 17368;
	// addi r3,r10,31860
	ctx.r3.s64 = ctx.r10.s64 + 31860;
	// bl 0x82845600
	ctx.lr = 0x825D5D74;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,13040
	ctx.r4.s64 = ctx.r11.s64 + 13040;
	// addi r3,r10,31844
	ctx.r3.s64 = ctx.r10.s64 + 31844;
	// bl 0x82845600
	ctx.lr = 0x825D5D88;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,17400
	ctx.r4.s64 = ctx.r11.s64 + 17400;
	// addi r3,r10,31816
	ctx.r3.s64 = ctx.r10.s64 + 31816;
	// bl 0x82845600
	ctx.lr = 0x825D5D9C;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,17472
	ctx.r4.s64 = ctx.r11.s64 + 17472;
	// addi r3,r10,31784
	ctx.r3.s64 = ctx.r10.s64 + 31784;
	// bl 0x82845600
	ctx.lr = 0x825D5DB0;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,17552
	ctx.r4.s64 = ctx.r11.s64 + 17552;
	// addi r3,r10,31760
	ctx.r3.s64 = ctx.r10.s64 + 31760;
	// bl 0x82845600
	ctx.lr = 0x825D5DC4;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,17624
	ctx.r4.s64 = ctx.r11.s64 + 17624;
	// addi r3,r10,31736
	ctx.r3.s64 = ctx.r10.s64 + 31736;
	// bl 0x82845600
	ctx.lr = 0x825D5DD8;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,17672
	ctx.r4.s64 = ctx.r11.s64 + 17672;
	// addi r3,r10,31716
	ctx.r3.s64 = ctx.r10.s64 + 31716;
	// bl 0x82845600
	ctx.lr = 0x825D5DEC;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22560
	ctx.r4.s64 = ctx.r11.s64 + 22560;
	// addi r3,r10,31692
	ctx.r3.s64 = ctx.r10.s64 + 31692;
	// bl 0x82845600
	ctx.lr = 0x825D5E00;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,17712
	ctx.r4.s64 = ctx.r11.s64 + 17712;
	// addi r3,r10,31668
	ctx.r3.s64 = ctx.r10.s64 + 31668;
	// bl 0x82845600
	ctx.lr = 0x825D5E14;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,17728
	ctx.r4.s64 = ctx.r11.s64 + 17728;
	// addi r3,r10,31644
	ctx.r3.s64 = ctx.r10.s64 + 31644;
	// bl 0x82845600
	ctx.lr = 0x825D5E28;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21392
	ctx.r4.s64 = ctx.r11.s64 + 21392;
	// addi r3,r10,31624
	ctx.r3.s64 = ctx.r10.s64 + 31624;
	// bl 0x82845600
	ctx.lr = 0x825D5E3C;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,17784
	ctx.r4.s64 = ctx.r11.s64 + 17784;
	// addi r3,r10,31600
	ctx.r3.s64 = ctx.r10.s64 + 31600;
	// bl 0x82845600
	ctx.lr = 0x825D5E50;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,13048
	ctx.r4.s64 = ctx.r11.s64 + 13048;
	// addi r3,r10,31580
	ctx.r3.s64 = ctx.r10.s64 + 31580;
	// bl 0x82845600
	ctx.lr = 0x825D5E64;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,17840
	ctx.r4.s64 = ctx.r11.s64 + 17840;
	// addi r3,r10,31568
	ctx.r3.s64 = ctx.r10.s64 + 31568;
	// bl 0x82845600
	ctx.lr = 0x825D5E78;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21488
	ctx.r4.s64 = ctx.r11.s64 + 21488;
	// addi r3,r10,31560
	ctx.r3.s64 = ctx.r10.s64 + 31560;
	// bl 0x82845600
	ctx.lr = 0x825D5E8C;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21528
	ctx.r4.s64 = ctx.r11.s64 + 21528;
	// addi r3,r10,31548
	ctx.r3.s64 = ctx.r10.s64 + 31548;
	// bl 0x82845600
	ctx.lr = 0x825D5EA0;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21568
	ctx.r4.s64 = ctx.r11.s64 + 21568;
	// addi r3,r10,31532
	ctx.r3.s64 = ctx.r10.s64 + 31532;
	// bl 0x82845600
	ctx.lr = 0x825D5EB4;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,17888
	ctx.r4.s64 = ctx.r11.s64 + 17888;
	// addi r3,r10,31504
	ctx.r3.s64 = ctx.r10.s64 + 31504;
	// bl 0x82845600
	ctx.lr = 0x825D5EC8;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,17920
	ctx.r4.s64 = ctx.r11.s64 + 17920;
	// addi r3,r10,31480
	ctx.r3.s64 = ctx.r10.s64 + 31480;
	// bl 0x82845600
	ctx.lr = 0x825D5EDC;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,17960
	ctx.r4.s64 = ctx.r11.s64 + 17960;
	// addi r3,r10,31452
	ctx.r3.s64 = ctx.r10.s64 + 31452;
	// bl 0x82845600
	ctx.lr = 0x825D5EF0;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18048
	ctx.r4.s64 = ctx.r11.s64 + 18048;
	// addi r3,r10,31432
	ctx.r3.s64 = ctx.r10.s64 + 31432;
	// bl 0x82845600
	ctx.lr = 0x825D5F04;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18088
	ctx.r4.s64 = ctx.r11.s64 + 18088;
	// addi r3,r10,31408
	ctx.r3.s64 = ctx.r10.s64 + 31408;
	// bl 0x82845600
	ctx.lr = 0x825D5F18;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18112
	ctx.r4.s64 = ctx.r11.s64 + 18112;
	// addi r3,r10,31388
	ctx.r3.s64 = ctx.r10.s64 + 31388;
	// bl 0x82845600
	ctx.lr = 0x825D5F2C;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18184
	ctx.r4.s64 = ctx.r11.s64 + 18184;
	// addi r3,r10,31372
	ctx.r3.s64 = ctx.r10.s64 + 31372;
	// bl 0x82845600
	ctx.lr = 0x825D5F40;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21632
	ctx.r4.s64 = ctx.r11.s64 + 21632;
	// addi r3,r10,31352
	ctx.r3.s64 = ctx.r10.s64 + 31352;
	// bl 0x82845600
	ctx.lr = 0x825D5F54;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21712
	ctx.r4.s64 = ctx.r11.s64 + 21712;
	// addi r3,r10,31324
	ctx.r3.s64 = ctx.r10.s64 + 31324;
	// bl 0x82845600
	ctx.lr = 0x825D5F68;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21800
	ctx.r4.s64 = ctx.r11.s64 + 21800;
	// addi r3,r10,31292
	ctx.r3.s64 = ctx.r10.s64 + 31292;
	// bl 0x82845600
	ctx.lr = 0x825D5F7C;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21864
	ctx.r4.s64 = ctx.r11.s64 + 21864;
	// addi r3,r10,31260
	ctx.r3.s64 = ctx.r10.s64 + 31260;
	// bl 0x82845600
	ctx.lr = 0x825D5F90;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21928
	ctx.r4.s64 = ctx.r11.s64 + 21928;
	// addi r3,r10,31244
	ctx.r3.s64 = ctx.r10.s64 + 31244;
	// bl 0x82845600
	ctx.lr = 0x825D5FA4;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18240
	ctx.r4.s64 = ctx.r11.s64 + 18240;
	// addi r3,r10,31236
	ctx.r3.s64 = ctx.r10.s64 + 31236;
	// bl 0x82845600
	ctx.lr = 0x825D5FB8;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18272
	ctx.r4.s64 = ctx.r11.s64 + 18272;
	// addi r3,r10,31228
	ctx.r3.s64 = ctx.r10.s64 + 31228;
	// bl 0x82845600
	ctx.lr = 0x825D5FCC;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18296
	ctx.r4.s64 = ctx.r11.s64 + 18296;
	// addi r3,r10,31200
	ctx.r3.s64 = ctx.r10.s64 + 31200;
	// bl 0x82845600
	ctx.lr = 0x825D5FE0;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18384
	ctx.r4.s64 = ctx.r11.s64 + 18384;
	// addi r3,r10,31176
	ctx.r3.s64 = ctx.r10.s64 + 31176;
	// bl 0x82845600
	ctx.lr = 0x825D5FF4;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18472
	ctx.r4.s64 = ctx.r11.s64 + 18472;
	// addi r3,r10,31156
	ctx.r3.s64 = ctx.r10.s64 + 31156;
	// bl 0x82845600
	ctx.lr = 0x825D6008;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18624
	ctx.r4.s64 = ctx.r11.s64 + 18624;
	// addi r3,r10,31136
	ctx.r3.s64 = ctx.r10.s64 + 31136;
	// bl 0x82845600
	ctx.lr = 0x825D601C;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22040
	ctx.r4.s64 = ctx.r11.s64 + 22040;
	// addi r3,r10,31124
	ctx.r3.s64 = ctx.r10.s64 + 31124;
	// bl 0x82845600
	ctx.lr = 0x825D6030;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18728
	ctx.r4.s64 = ctx.r11.s64 + 18728;
	// addi r3,r10,31108
	ctx.r3.s64 = ctx.r10.s64 + 31108;
	// bl 0x82845600
	ctx.lr = 0x825D6044;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18840
	ctx.r4.s64 = ctx.r11.s64 + 18840;
	// addi r3,r10,31088
	ctx.r3.s64 = ctx.r10.s64 + 31088;
	// bl 0x82845600
	ctx.lr = 0x825D6058;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-6424
	ctx.r4.s64 = ctx.r11.s64 + -6424;
	// addi r3,r10,31068
	ctx.r3.s64 = ctx.r10.s64 + 31068;
	// bl 0x82845600
	ctx.lr = 0x825D606C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-6424
	ctx.r4.s64 = ctx.r11.s64 + -6424;
	// addi r3,r10,31048
	ctx.r3.s64 = ctx.r10.s64 + 31048;
	// bl 0x82845600
	ctx.lr = 0x825D6080;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-6424
	ctx.r4.s64 = ctx.r11.s64 + -6424;
	// addi r3,r10,31028
	ctx.r3.s64 = ctx.r10.s64 + 31028;
	// bl 0x82845600
	ctx.lr = 0x825D6094;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18856
	ctx.r4.s64 = ctx.r11.s64 + 18856;
	// addi r3,r10,31004
	ctx.r3.s64 = ctx.r10.s64 + 31004;
	// bl 0x82845600
	ctx.lr = 0x825D60A8;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22576
	ctx.r4.s64 = ctx.r11.s64 + 22576;
	// addi r3,r10,30960
	ctx.r3.s64 = ctx.r10.s64 + 30960;
	// bl 0x82845600
	ctx.lr = 0x825D60BC;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22136
	ctx.r4.s64 = ctx.r11.s64 + 22136;
	// addi r3,r10,30936
	ctx.r3.s64 = ctx.r10.s64 + 30936;
	// bl 0x82845600
	ctx.lr = 0x825D60D0;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22152
	ctx.r4.s64 = ctx.r11.s64 + 22152;
	// addi r3,r10,30920
	ctx.r3.s64 = ctx.r10.s64 + 30920;
	// bl 0x82845600
	ctx.lr = 0x825D60E4;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22184
	ctx.r4.s64 = ctx.r11.s64 + 22184;
	// addi r3,r10,30904
	ctx.r3.s64 = ctx.r10.s64 + 30904;
	// bl 0x82845600
	ctx.lr = 0x825D60F8;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22256
	ctx.r4.s64 = ctx.r11.s64 + 22256;
	// addi r3,r10,30884
	ctx.r3.s64 = ctx.r10.s64 + 30884;
	// bl 0x82845600
	ctx.lr = 0x825D610C;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18936
	ctx.r4.s64 = ctx.r11.s64 + 18936;
	// addi r3,r10,30832
	ctx.r3.s64 = ctx.r10.s64 + 30832;
	// bl 0x82845600
	ctx.lr = 0x825D6120;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// addi r4,r11,18968
	ctx.r4.s64 = ctx.r11.s64 + 18968;
	// lis r11,-32253
	ctx.r11.s64 = -2113732608;
	// addi r3,r11,30780
	ctx.r3.s64 = ctx.r11.s64 + 30780;
	// bl 0x82845600
	ctx.lr = 0x825D6134;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18992
	ctx.r4.s64 = ctx.r11.s64 + 18992;
	// addi r3,r10,30756
	ctx.r3.s64 = ctx.r10.s64 + 30756;
	// bl 0x82845600
	ctx.lr = 0x825D6148;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,19032
	ctx.r4.s64 = ctx.r11.s64 + 19032;
	// addi r3,r10,30732
	ctx.r3.s64 = ctx.r10.s64 + 30732;
	// bl 0x82845600
	ctx.lr = 0x825D615C;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22280
	ctx.r4.s64 = ctx.r11.s64 + 22280;
	// addi r3,r10,30712
	ctx.r3.s64 = ctx.r10.s64 + 30712;
	// bl 0x82845600
	ctx.lr = 0x825D6170;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,19072
	ctx.r4.s64 = ctx.r11.s64 + 19072;
	// addi r3,r10,30692
	ctx.r3.s64 = ctx.r10.s64 + 30692;
	// bl 0x82845600
	ctx.lr = 0x825D6184;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,20296
	ctx.r4.s64 = ctx.r11.s64 + 20296;
	// addi r3,r10,30676
	ctx.r3.s64 = ctx.r10.s64 + 30676;
	// bl 0x82845600
	ctx.lr = 0x825D6198;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,19128
	ctx.r4.s64 = ctx.r11.s64 + 19128;
	// addi r3,r10,30644
	ctx.r3.s64 = ctx.r10.s64 + 30644;
	// bl 0x82845600
	ctx.lr = 0x825D61AC;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,13056
	ctx.r4.s64 = ctx.r11.s64 + 13056;
	// addi r3,r10,30632
	ctx.r3.s64 = ctx.r10.s64 + 30632;
	// bl 0x82845600
	ctx.lr = 0x825D61C0;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,13096
	ctx.r4.s64 = ctx.r11.s64 + 13096;
	// addi r3,r10,30616
	ctx.r3.s64 = ctx.r10.s64 + 30616;
	// bl 0x82845600
	ctx.lr = 0x825D61D4;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,19160
	ctx.r4.s64 = ctx.r11.s64 + 19160;
	// addi r3,r10,30584
	ctx.r3.s64 = ctx.r10.s64 + 30584;
	// bl 0x82845600
	ctx.lr = 0x825D61E8;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,19224
	ctx.r4.s64 = ctx.r11.s64 + 19224;
	// addi r3,r10,30528
	ctx.r3.s64 = ctx.r10.s64 + 30528;
	// bl 0x82845600
	ctx.lr = 0x825D61FC;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,13136
	ctx.r4.s64 = ctx.r11.s64 + 13136;
	// addi r3,r10,30504
	ctx.r3.s64 = ctx.r10.s64 + 30504;
	// bl 0x82845600
	ctx.lr = 0x825D6210;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,13176
	ctx.r4.s64 = ctx.r11.s64 + 13176;
	// addi r3,r10,30480
	ctx.r3.s64 = ctx.r10.s64 + 30480;
	// bl 0x82845600
	ctx.lr = 0x825D6224;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22352
	ctx.r4.s64 = ctx.r11.s64 + 22352;
	// addi r3,r10,30448
	ctx.r3.s64 = ctx.r10.s64 + 30448;
	// bl 0x82845600
	ctx.lr = 0x825D6238;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-6424
	ctx.r4.s64 = ctx.r11.s64 + -6424;
	// addi r3,r10,30428
	ctx.r3.s64 = ctx.r10.s64 + 30428;
	// bl 0x82845600
	ctx.lr = 0x825D624C;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,19288
	ctx.r4.s64 = ctx.r11.s64 + 19288;
	// addi r3,r10,30412
	ctx.r3.s64 = ctx.r10.s64 + 30412;
	// bl 0x82845600
	ctx.lr = 0x825D6260;
	sub_82845600(ctx, base);
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// addi r3,r10,30388
	ctx.r3.s64 = ctx.r10.s64 + 30388;
	// addi r4,r11,19304
	ctx.r4.s64 = ctx.r11.s64 + 19304;
	// bl 0x82845600
	ctx.lr = 0x825D6274;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,19376
	ctx.r4.s64 = ctx.r11.s64 + 19376;
	// addi r3,r10,30368
	ctx.r3.s64 = ctx.r10.s64 + 30368;
	// bl 0x82845600
	ctx.lr = 0x825D6288;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,19408
	ctx.r4.s64 = ctx.r11.s64 + 19408;
	// addi r3,r10,30344
	ctx.r3.s64 = ctx.r10.s64 + 30344;
	// bl 0x82845600
	ctx.lr = 0x825D629C;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,19440
	ctx.r4.s64 = ctx.r11.s64 + 19440;
	// addi r3,r10,30312
	ctx.r3.s64 = ctx.r10.s64 + 30312;
	// bl 0x82845600
	ctx.lr = 0x825D62B0;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,13184
	ctx.r4.s64 = ctx.r11.s64 + 13184;
	// addi r3,r10,30272
	ctx.r3.s64 = ctx.r10.s64 + 30272;
	// bl 0x82845600
	ctx.lr = 0x825D62C4;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,19464
	ctx.r4.s64 = ctx.r11.s64 + 19464;
	// addi r3,r10,30244
	ctx.r3.s64 = ctx.r10.s64 + 30244;
	// bl 0x82845600
	ctx.lr = 0x825D62D8;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,13192
	ctx.r4.s64 = ctx.r11.s64 + 13192;
	// addi r3,r10,30212
	ctx.r3.s64 = ctx.r10.s64 + 30212;
	// bl 0x82845600
	ctx.lr = 0x825D62EC;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,19496
	ctx.r4.s64 = ctx.r11.s64 + 19496;
	// addi r3,r10,30180
	ctx.r3.s64 = ctx.r10.s64 + 30180;
	// bl 0x82845600
	ctx.lr = 0x825D6300;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,19528
	ctx.r4.s64 = ctx.r11.s64 + 19528;
	// addi r3,r10,30160
	ctx.r3.s64 = ctx.r10.s64 + 30160;
	// bl 0x82845600
	ctx.lr = 0x825D6314;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,19632
	ctx.r4.s64 = ctx.r11.s64 + 19632;
	// addi r3,r10,30140
	ctx.r3.s64 = ctx.r10.s64 + 30140;
	// bl 0x82845600
	ctx.lr = 0x825D6328;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,13248
	ctx.r4.s64 = ctx.r11.s64 + 13248;
	// addi r3,r10,30092
	ctx.r3.s64 = ctx.r10.s64 + 30092;
	// bl 0x82845600
	ctx.lr = 0x825D633C;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,13272
	ctx.r4.s64 = ctx.r11.s64 + 13272;
	// addi r3,r10,30076
	ctx.r3.s64 = ctx.r10.s64 + 30076;
	// bl 0x82845600
	ctx.lr = 0x825D6350;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,19656
	ctx.r4.s64 = ctx.r11.s64 + 19656;
	// addi r3,r10,30056
	ctx.r3.s64 = ctx.r10.s64 + 30056;
	// bl 0x82845600
	ctx.lr = 0x825D6364;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,19680
	ctx.r4.s64 = ctx.r11.s64 + 19680;
	// addi r3,r10,30032
	ctx.r3.s64 = ctx.r10.s64 + 30032;
	// bl 0x82845600
	ctx.lr = 0x825D6378;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,20312
	ctx.r4.s64 = ctx.r11.s64 + 20312;
	// addi r3,r10,30012
	ctx.r3.s64 = ctx.r10.s64 + 30012;
	// bl 0x82845600
	ctx.lr = 0x825D638C;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,19752
	ctx.r4.s64 = ctx.r11.s64 + 19752;
	// addi r3,r10,29988
	ctx.r3.s64 = ctx.r10.s64 + 29988;
	// bl 0x82845600
	ctx.lr = 0x825D63A0;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,19832
	ctx.r4.s64 = ctx.r11.s64 + 19832;
	// addi r3,r10,29952
	ctx.r3.s64 = ctx.r10.s64 + 29952;
	// bl 0x82845600
	ctx.lr = 0x825D63B4;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,13288
	ctx.r4.s64 = ctx.r11.s64 + 13288;
	// addi r3,r10,29936
	ctx.r3.s64 = ctx.r10.s64 + 29936;
	// bl 0x82845600
	ctx.lr = 0x825D63C8;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,19848
	ctx.r4.s64 = ctx.r11.s64 + 19848;
	// addi r3,r10,29916
	ctx.r3.s64 = ctx.r10.s64 + 29916;
	// bl 0x82845600
	ctx.lr = 0x825D63DC;
	sub_82845600(ctx, base);
	// lis r11,-32163
	ctx.r11.s64 = -2107834368;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,19880
	ctx.r4.s64 = ctx.r11.s64 + 19880;
	// addi r3,r10,29884
	ctx.r3.s64 = ctx.r10.s64 + 29884;
	// bl 0x82845600
	ctx.lr = 0x825D63F0;
	sub_82845600(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D6400);
PPC_WEAK_FUNC(sub_825D6400) { __imp__sub_825D6400(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D6400) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c8
	ctx.lr = 0x825D6408;
	__savegprlr_28(ctx, base);
	// stwu r1,-192(r1)
	ea = -192 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,-1640
	ctx.r31.s64 = ctx.r11.s64 + -1640;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D6428;
	sub_8221FD88(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// li r10,0
	ctx.r10.s64 = 0;
	// lwz r4,604(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// mr r8,r28
	ctx.r8.u64 = ctx.r28.u64;
	// mr r7,r29
	ctx.r7.u64 = ctx.r29.u64;
	// li r6,-1
	ctx.r6.s64 = -1;
	// lbz r11,-16940(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -16940);
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,0
	ctx.r9.s64 = 0;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x82223a58
	ctx.lr = 0x825D6484;
	sub_82223A58(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16940(r30)
	PPC_STORE_U8(ctx.r30.u32 + -16940, ctx.r11.u8);
	// addi r1,r1,192
	ctx.r1.s64 = ctx.r1.s64 + 192;
	// b 0x829ff818
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D6498);
PPC_WEAK_FUNC(sub_825D6498) { __imp__sub_825D6498(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D6498) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c8
	ctx.lr = 0x825D64A0;
	__savegprlr_28(ctx, base);
	// stwu r1,-192(r1)
	ea = -192 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,-1640
	ctx.r31.s64 = ctx.r11.s64 + -1640;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D64C0;
	sub_8221FD88(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// li r10,0
	ctx.r10.s64 = 0;
	// lwz r4,604(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// mr r8,r28
	ctx.r8.u64 = ctx.r28.u64;
	// mr r7,r29
	ctx.r7.u64 = ctx.r29.u64;
	// li r6,-1
	ctx.r6.s64 = -1;
	// lbz r11,-16940(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -16940);
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,1
	ctx.r9.s64 = 1;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x82223a58
	ctx.lr = 0x825D651C;
	sub_82223A58(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16940(r30)
	PPC_STORE_U8(ctx.r30.u32 + -16940, ctx.r11.u8);
	// addi r1,r1,192
	ctx.r1.s64 = ctx.r1.s64 + 192;
	// b 0x829ff818
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D6530);
PPC_WEAK_FUNC(sub_825D6530) { __imp__sub_825D6530(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D6530) {
	PPC_FUNC_PROLOGUE();
	// mr r10,r3
	ctx.r10.u64 = ctx.r3.u64;
	// mr r11,r4
	ctx.r11.u64 = ctx.r4.u64;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// lis r9,-32070
	ctx.r9.s64 = -2101739520;
	// beq cr6,0x825d6564
	if (ctx.cr6.eq) goto loc_825D6564;
	// addi r9,r9,-556
	ctx.r9.s64 = ctx.r9.s64 + -556;
	// subf r8,r10,r9
	ctx.r8.u64 = ctx.r9.u64 - ctx.r10.u64;
loc_825D654C:
	// lbz r9,0(r10)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// stbx r9,r8,r10
	PPC_STORE_U8(ctx.r8.u32 + ctx.r10.u32, ctx.r9.u8);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// bne cr6,0x825d654c
	if (!ctx.cr6.eq) goto loc_825D654C;
	// b 0x825d656c
	goto loc_825D656C;
loc_825D6564:
	// li r10,0
	ctx.r10.s64 = 0;
	// stb r10,-556(r9)
	PPC_STORE_U8(ctx.r9.u32 + -556, ctx.r10.u8);
loc_825D656C:
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// beq cr6,0x825d6598
	if (ctx.cr6.eq) goto loc_825D6598;
	// addi r10,r10,-572
	ctx.r10.s64 = ctx.r10.s64 + -572;
	// subf r9,r11,r10
	ctx.r9.u64 = ctx.r10.u64 - ctx.r11.u64;
loc_825D6580:
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r9,r11
	PPC_STORE_U8(ctx.r9.u32 + ctx.r11.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x825d6580
	if (!ctx.cr6.eq) goto loc_825D6580;
	// blr 
	return;
loc_825D6598:
	// li r11,0
	ctx.r11.s64 = 0;
	// stb r11,-572(r10)
	PPC_STORE_U8(ctx.r10.u32 + -572, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D65A8);
PPC_WEAK_FUNC(sub_825D65A8) { __imp__sub_825D65A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D65A8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c4
	ctx.lr = 0x825D65B0;
	__savegprlr_27(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,-1640
	ctx.r31.s64 = ctx.r11.s64 + -1640;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D65D4;
	sub_8221FD88(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// lwz r4,604(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r29,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r29.u32);
	// mr r8,r27
	ctx.r8.u64 = ctx.r27.u64;
	// mr r7,r28
	ctx.r7.u64 = ctx.r28.u64;
	// li r6,-1
	ctx.r6.s64 = -1;
	// lbz r11,-16940(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -16940);
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,0
	ctx.r9.s64 = 0;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// bl 0x82223a58
	ctx.lr = 0x825D6630;
	sub_82223A58(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16940(r30)
	PPC_STORE_U8(ctx.r30.u32 + -16940, ctx.r11.u8);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x829ff814
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D6640);
PPC_WEAK_FUNC(sub_825D6640) { __imp__sub_825D6640(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D6640) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c4
	ctx.lr = 0x825D6648;
	__savegprlr_27(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,-1640
	ctx.r31.s64 = ctx.r11.s64 + -1640;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D666C;
	sub_8221FD88(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// lwz r4,604(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r29,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r29.u32);
	// mr r8,r27
	ctx.r8.u64 = ctx.r27.u64;
	// mr r7,r28
	ctx.r7.u64 = ctx.r28.u64;
	// li r6,-1
	ctx.r6.s64 = -1;
	// lbz r11,-16940(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -16940);
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,1
	ctx.r9.s64 = 1;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// bl 0x82223a58
	ctx.lr = 0x825D66C8;
	sub_82223A58(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16940(r30)
	PPC_STORE_U8(ctx.r30.u32 + -16940, ctx.r11.u8);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x829ff814
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D66D8);
PPC_WEAK_FUNC(sub_825D66D8) { __imp__sub_825D66D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D66D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c0
	ctx.lr = 0x825D66E0;
	__savegprlr_26(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,-1640
	ctx.r31.s64 = ctx.r11.s64 + -1640;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// mr r26,r7
	ctx.r26.u64 = ctx.r7.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D6708;
	sub_8221FD88(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// lwz r4,604(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r28,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r28.u32);
	// mr r8,r26
	ctx.r8.u64 = ctx.r26.u64;
	// stw r29,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r29.u32);
	// mr r7,r27
	ctx.r7.u64 = ctx.r27.u64;
	// li r6,-1
	ctx.r6.s64 = -1;
	// lbz r11,-16940(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -16940);
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,0
	ctx.r9.s64 = 0;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// bl 0x82223a58
	ctx.lr = 0x825D6764;
	sub_82223A58(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16940(r30)
	PPC_STORE_U8(ctx.r30.u32 + -16940, ctx.r11.u8);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D6778);
PPC_WEAK_FUNC(sub_825D6778) { __imp__sub_825D6778(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D6778) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c0
	ctx.lr = 0x825D6780;
	__savegprlr_26(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,-1640
	ctx.r31.s64 = ctx.r11.s64 + -1640;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// mr r26,r7
	ctx.r26.u64 = ctx.r7.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D67A8;
	sub_8221FD88(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// lwz r4,604(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r28,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r28.u32);
	// mr r8,r26
	ctx.r8.u64 = ctx.r26.u64;
	// stw r29,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r29.u32);
	// mr r7,r27
	ctx.r7.u64 = ctx.r27.u64;
	// li r6,-1
	ctx.r6.s64 = -1;
	// lbz r11,-16940(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -16940);
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,1
	ctx.r9.s64 = 1;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// bl 0x82223a58
	ctx.lr = 0x825D6804;
	sub_82223A58(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16940(r30)
	PPC_STORE_U8(ctx.r30.u32 + -16940, ctx.r11.u8);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D6818);
PPC_WEAK_FUNC(sub_825D6818) { __imp__sub_825D6818(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D6818) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7bc
	ctx.lr = 0x825D6820;
	__savegprlr_25(ctx, base);
	// stwu r1,-224(r1)
	ea = -224 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,-1640
	ctx.r31.s64 = ctx.r11.s64 + -1640;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// mr r26,r7
	ctx.r26.u64 = ctx.r7.u64;
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D684C;
	sub_8221FD88(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// lwz r4,604(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r27,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r27.u32);
	// mr r8,r25
	ctx.r8.u64 = ctx.r25.u64;
	// stw r28,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r28.u32);
	// mr r7,r26
	ctx.r7.u64 = ctx.r26.u64;
	// stw r29,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r29.u32);
	// li r6,-1
	ctx.r6.s64 = -1;
	// lbz r11,-16940(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -16940);
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,0
	ctx.r9.s64 = 0;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// bl 0x82223a58
	ctx.lr = 0x825D68A8;
	sub_82223A58(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16940(r30)
	PPC_STORE_U8(ctx.r30.u32 + -16940, ctx.r11.u8);
	// addi r1,r1,224
	ctx.r1.s64 = ctx.r1.s64 + 224;
	// b 0x829ff80c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D68B8);
PPC_WEAK_FUNC(sub_825D68B8) { __imp__sub_825D68B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D68B8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7bc
	ctx.lr = 0x825D68C0;
	__savegprlr_25(ctx, base);
	// stwu r1,-224(r1)
	ea = -224 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,-1640
	ctx.r31.s64 = ctx.r11.s64 + -1640;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// mr r26,r7
	ctx.r26.u64 = ctx.r7.u64;
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D68EC;
	sub_8221FD88(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// lwz r4,604(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r27,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r27.u32);
	// mr r8,r25
	ctx.r8.u64 = ctx.r25.u64;
	// stw r28,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r28.u32);
	// mr r7,r26
	ctx.r7.u64 = ctx.r26.u64;
	// stw r29,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r29.u32);
	// li r6,-1
	ctx.r6.s64 = -1;
	// lbz r11,-16940(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -16940);
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,1
	ctx.r9.s64 = 1;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// bl 0x82223a58
	ctx.lr = 0x825D6948;
	sub_82223A58(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16940(r30)
	PPC_STORE_U8(ctx.r30.u32 + -16940, ctx.r11.u8);
	// addi r1,r1,224
	ctx.r1.s64 = ctx.r1.s64 + 224;
	// b 0x829ff80c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D6958);
PPC_WEAK_FUNC(sub_825D6958) { __imp__sub_825D6958(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D6958) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7b8
	ctx.lr = 0x825D6960;
	__savegprlr_24(ctx, base);
	// stwu r1,-224(r1)
	ea = -224 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,-1640
	ctx.r31.s64 = ctx.r11.s64 + -1640;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// mr r26,r7
	ctx.r26.u64 = ctx.r7.u64;
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// mr r24,r9
	ctx.r24.u64 = ctx.r9.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D6990;
	sub_8221FD88(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// lwz r4,604(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r26,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r26.u32);
	// mr r8,r24
	ctx.r8.u64 = ctx.r24.u64;
	// stw r27,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r27.u32);
	// mr r7,r25
	ctx.r7.u64 = ctx.r25.u64;
	// stw r28,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r28.u32);
	// li r6,-1
	ctx.r6.s64 = -1;
	// stw r29,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r29.u32);
	// lbz r11,-16940(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -16940);
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,0
	ctx.r9.s64 = 0;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// bl 0x82223a58
	ctx.lr = 0x825D69EC;
	sub_82223A58(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16940(r30)
	PPC_STORE_U8(ctx.r30.u32 + -16940, ctx.r11.u8);
	// addi r1,r1,224
	ctx.r1.s64 = ctx.r1.s64 + 224;
	// b 0x829ff808
	__restgprlr_24(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D6A00);
PPC_WEAK_FUNC(sub_825D6A00) { __imp__sub_825D6A00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D6A00) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7b8
	ctx.lr = 0x825D6A08;
	__savegprlr_24(ctx, base);
	// stwu r1,-224(r1)
	ea = -224 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,-1640
	ctx.r31.s64 = ctx.r11.s64 + -1640;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// mr r26,r7
	ctx.r26.u64 = ctx.r7.u64;
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// mr r24,r9
	ctx.r24.u64 = ctx.r9.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D6A38;
	sub_8221FD88(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// lwz r4,604(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r26,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r26.u32);
	// mr r8,r24
	ctx.r8.u64 = ctx.r24.u64;
	// stw r27,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r27.u32);
	// mr r7,r25
	ctx.r7.u64 = ctx.r25.u64;
	// stw r28,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r28.u32);
	// li r6,-1
	ctx.r6.s64 = -1;
	// stw r29,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r29.u32);
	// lbz r11,-16940(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -16940);
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,1
	ctx.r9.s64 = 1;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// bl 0x82223a58
	ctx.lr = 0x825D6A94;
	sub_82223A58(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16940(r30)
	PPC_STORE_U8(ctx.r30.u32 + -16940, ctx.r11.u8);
	// addi r1,r1,224
	ctx.r1.s64 = ctx.r1.s64 + 224;
	// b 0x829ff808
	__restgprlr_24(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D6AA8);
PPC_WEAK_FUNC(sub_825D6AA8) { __imp__sub_825D6AA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D6AA8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7b4
	ctx.lr = 0x825D6AB0;
	__savegprlr_23(ctx, base);
	// stwu r1,-240(r1)
	ea = -240 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,-1640
	ctx.r31.s64 = ctx.r11.s64 + -1640;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// mr r26,r7
	ctx.r26.u64 = ctx.r7.u64;
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// mr r24,r9
	ctx.r24.u64 = ctx.r9.u64;
	// mr r23,r10
	ctx.r23.u64 = ctx.r10.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D6AE4;
	sub_8221FD88(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// lwz r4,604(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r25,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r25.u32);
	// mr r7,r24
	ctx.r7.u64 = ctx.r24.u64;
	// stw r26,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r26.u32);
	// li r6,-1
	ctx.r6.s64 = -1;
	// stw r27,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r27.u32);
	// li r5,0
	ctx.r5.s64 = 0;
	// stw r28,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r28.u32);
	// lbz r11,-16940(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -16940);
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// stw r29,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r29.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r8,r9,27,31,31
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,0
	ctx.r9.s64 = 0;
	// xori r10,r8,1
	ctx.r10.u64 = ctx.r8.u64 ^ 1;
	// mr r8,r23
	ctx.r8.u64 = ctx.r23.u64;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// bl 0x82223a58
	ctx.lr = 0x825D6B40;
	sub_82223A58(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16940(r30)
	PPC_STORE_U8(ctx.r30.u32 + -16940, ctx.r11.u8);
	// addi r1,r1,240
	ctx.r1.s64 = ctx.r1.s64 + 240;
	// b 0x829ff804
	__restgprlr_23(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D6B50);
PPC_WEAK_FUNC(sub_825D6B50) { __imp__sub_825D6B50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D6B50) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7b4
	ctx.lr = 0x825D6B58;
	__savegprlr_23(ctx, base);
	// stwu r1,-240(r1)
	ea = -240 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,-1640
	ctx.r31.s64 = ctx.r11.s64 + -1640;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// mr r26,r7
	ctx.r26.u64 = ctx.r7.u64;
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// mr r24,r9
	ctx.r24.u64 = ctx.r9.u64;
	// mr r23,r10
	ctx.r23.u64 = ctx.r10.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D6B8C;
	sub_8221FD88(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// lwz r4,604(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r25,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r25.u32);
	// mr r7,r24
	ctx.r7.u64 = ctx.r24.u64;
	// stw r26,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r26.u32);
	// li r6,-1
	ctx.r6.s64 = -1;
	// stw r27,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r27.u32);
	// li r5,0
	ctx.r5.s64 = 0;
	// stw r28,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r28.u32);
	// lbz r11,-16940(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -16940);
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// stw r29,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r29.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r8,r9,27,31,31
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,1
	ctx.r9.s64 = 1;
	// xori r10,r8,1
	ctx.r10.u64 = ctx.r8.u64 ^ 1;
	// mr r8,r23
	ctx.r8.u64 = ctx.r23.u64;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// bl 0x82223a58
	ctx.lr = 0x825D6BE8;
	sub_82223A58(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16940(r30)
	PPC_STORE_U8(ctx.r30.u32 + -16940, ctx.r11.u8);
	// addi r1,r1,240
	ctx.r1.s64 = ctx.r1.s64 + 240;
	// b 0x829ff804
	__restgprlr_23(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D6BF8);
PPC_WEAK_FUNC(sub_825D6BF8) { __imp__sub_825D6BF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D6BF8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7b4
	ctx.lr = 0x825D6C00;
	__savegprlr_23(ctx, base);
	// stwu r1,-240(r1)
	ea = -240 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,-1640
	ctx.r31.s64 = ctx.r11.s64 + -1640;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// mr r26,r7
	ctx.r26.u64 = ctx.r7.u64;
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// mr r24,r9
	ctx.r24.u64 = ctx.r9.u64;
	// mr r23,r10
	ctx.r23.u64 = ctx.r10.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D6C34;
	sub_8221FD88(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// lwz r4,604(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r24,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r24.u32);
	// mr r7,r23
	ctx.r7.u64 = ctx.r23.u64;
	// stw r25,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r25.u32);
	// li r6,-1
	ctx.r6.s64 = -1;
	// stw r26,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r26.u32);
	// li r5,0
	ctx.r5.s64 = 0;
	// stw r27,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r27.u32);
	// lbz r11,-16940(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -16940);
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// stw r28,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r28.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// stw r29,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r29.u32);
	// rlwinm r8,r9,27,31,31
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,0
	ctx.r9.s64 = 0;
	// xori r10,r8,1
	ctx.r10.u64 = ctx.r8.u64 ^ 1;
	// lwz r8,324(r1)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r1.u32 + 324);
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// bl 0x82223a58
	ctx.lr = 0x825D6C90;
	sub_82223A58(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16940(r30)
	PPC_STORE_U8(ctx.r30.u32 + -16940, ctx.r11.u8);
	// addi r1,r1,240
	ctx.r1.s64 = ctx.r1.s64 + 240;
	// b 0x829ff804
	__restgprlr_23(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D6CA0);
PPC_WEAK_FUNC(sub_825D6CA0) { __imp__sub_825D6CA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D6CA0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7b4
	ctx.lr = 0x825D6CA8;
	__savegprlr_23(ctx, base);
	// stwu r1,-240(r1)
	ea = -240 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,-1640
	ctx.r31.s64 = ctx.r11.s64 + -1640;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// mr r26,r7
	ctx.r26.u64 = ctx.r7.u64;
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// mr r24,r9
	ctx.r24.u64 = ctx.r9.u64;
	// mr r23,r10
	ctx.r23.u64 = ctx.r10.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D6CDC;
	sub_8221FD88(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// lwz r4,604(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r24,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r24.u32);
	// mr r7,r23
	ctx.r7.u64 = ctx.r23.u64;
	// stw r25,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r25.u32);
	// li r6,-1
	ctx.r6.s64 = -1;
	// stw r26,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r26.u32);
	// li r5,0
	ctx.r5.s64 = 0;
	// stw r27,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r27.u32);
	// lbz r11,-16940(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -16940);
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// stw r28,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r28.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// stw r29,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r29.u32);
	// rlwinm r8,r9,27,31,31
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,1
	ctx.r9.s64 = 1;
	// xori r10,r8,1
	ctx.r10.u64 = ctx.r8.u64 ^ 1;
	// lwz r8,324(r1)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r1.u32 + 324);
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// bl 0x82223a58
	ctx.lr = 0x825D6D38;
	sub_82223A58(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16940(r30)
	PPC_STORE_U8(ctx.r30.u32 + -16940, ctx.r11.u8);
	// addi r1,r1,240
	ctx.r1.s64 = ctx.r1.s64 + 240;
	// b 0x829ff804
	__restgprlr_23(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D6D48);
PPC_WEAK_FUNC(sub_825D6D48) { __imp__sub_825D6D48(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D6D48) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c0
	ctx.lr = 0x825D6D50;
	__savegprlr_26(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// addi r31,r11,-1640
	ctx.r31.s64 = ctx.r11.s64 + -1640;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D6D74;
	sub_8221FD88(ctx, base);
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// lwz r26,604(r31)
	ctx.r26.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D6D88;
	sub_8221FD88(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// li r10,0
	ctx.r10.s64 = 0;
	// lwz r6,604(r31)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
	// mr r8,r28
	ctx.r8.u64 = ctx.r28.u64;
	// mr r7,r29
	ctx.r7.u64 = ctx.r29.u64;
	// lbz r11,-16940(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -16940);
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,0
	ctx.r9.s64 = 0;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x82223a58
	ctx.lr = 0x825D6DE8;
	sub_82223A58(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16940(r30)
	PPC_STORE_U8(ctx.r30.u32 + -16940, ctx.r11.u8);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D6DF8);
PPC_WEAK_FUNC(sub_825D6DF8) { __imp__sub_825D6DF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D6DF8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c0
	ctx.lr = 0x825D6E00;
	__savegprlr_26(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// addi r31,r11,-1640
	ctx.r31.s64 = ctx.r11.s64 + -1640;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D6E24;
	sub_8221FD88(ctx, base);
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// lwz r26,604(r31)
	ctx.r26.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D6E38;
	sub_8221FD88(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// li r10,0
	ctx.r10.s64 = 0;
	// lwz r6,604(r31)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
	// mr r8,r28
	ctx.r8.u64 = ctx.r28.u64;
	// mr r7,r29
	ctx.r7.u64 = ctx.r29.u64;
	// lbz r11,-16940(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -16940);
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,1
	ctx.r9.s64 = 1;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x82223a58
	ctx.lr = 0x825D6E98;
	sub_82223A58(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16940(r30)
	PPC_STORE_U8(ctx.r30.u32 + -16940, ctx.r11.u8);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D6EA8);
PPC_WEAK_FUNC(sub_825D6EA8) { __imp__sub_825D6EA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D6EA8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c0
	ctx.lr = 0x825D6EB0;
	__savegprlr_26(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// addi r31,r11,-1640
	ctx.r31.s64 = ctx.r11.s64 + -1640;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D6ED4;
	sub_8221FD88(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// addi r11,r11,-10248
	ctx.r11.s64 = ctx.r11.s64 + -10248;
	// li r5,1
	ctx.r5.s64 = 1;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x821fe1c0
	ctx.lr = 0x825D6EF0;
	sub_821FE1C0(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// li r10,0
	ctx.r10.s64 = 0;
	// lwz r4,604(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// mr r8,r28
	ctx.r8.u64 = ctx.r28.u64;
	// mr r7,r29
	ctx.r7.u64 = ctx.r29.u64;
	// lbz r11,-16940(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -16940);
	// li r6,-1
	ctx.r6.s64 = -1;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// li r5,0
	ctx.r5.s64 = 0;
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// stw r26,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r26.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,0
	ctx.r9.s64 = 0;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x82223a58
	ctx.lr = 0x825D6F54;
	sub_82223A58(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16940(r30)
	PPC_STORE_U8(ctx.r30.u32 + -16940, ctx.r11.u8);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D6F68);
PPC_WEAK_FUNC(sub_825D6F68) { __imp__sub_825D6F68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D6F68) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c0
	ctx.lr = 0x825D6F70;
	__savegprlr_26(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// addi r31,r11,-1640
	ctx.r31.s64 = ctx.r11.s64 + -1640;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D6F94;
	sub_8221FD88(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// addi r11,r11,-10248
	ctx.r11.s64 = ctx.r11.s64 + -10248;
	// li r5,1
	ctx.r5.s64 = 1;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x821fe1c0
	ctx.lr = 0x825D6FB0;
	sub_821FE1C0(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// li r10,0
	ctx.r10.s64 = 0;
	// lwz r4,604(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// mr r8,r28
	ctx.r8.u64 = ctx.r28.u64;
	// mr r7,r29
	ctx.r7.u64 = ctx.r29.u64;
	// lbz r11,-16940(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -16940);
	// li r6,-1
	ctx.r6.s64 = -1;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// li r5,0
	ctx.r5.s64 = 0;
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// stw r26,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r26.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,1
	ctx.r9.s64 = 1;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x82223a58
	ctx.lr = 0x825D7014;
	sub_82223A58(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16940(r30)
	PPC_STORE_U8(ctx.r30.u32 + -16940, ctx.r11.u8);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D7028);
PPC_WEAK_FUNC(sub_825D7028) { __imp__sub_825D7028(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D7028) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7bc
	ctx.lr = 0x825D7030;
	__savegprlr_25(ctx, base);
	// stwu r1,-224(r1)
	ea = -224 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,-1640
	ctx.r31.s64 = ctx.r11.s64 + -1640;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// mr r26,r7
	ctx.r26.u64 = ctx.r7.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D7058;
	sub_8221FD88(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// mr r25,r3
	ctx.r25.u64 = ctx.r3.u64;
	// addi r30,r11,-10248
	ctx.r30.s64 = ctx.r11.s64 + -10248;
	// li r5,1
	ctx.r5.s64 = 1;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x821fe1c0
	ctx.lr = 0x825D7074;
	sub_821FE1C0(ctx, base);
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// li r5,1
	ctx.r5.s64 = 1;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x821fe1c0
	ctx.lr = 0x825D7088;
	sub_821FE1C0(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// li r10,0
	ctx.r10.s64 = 0;
	// lwz r4,604(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// mr r28,r3
	ctx.r28.u64 = ctx.r3.u64;
	// stw r29,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r29.u32);
	// mr r8,r26
	ctx.r8.u64 = ctx.r26.u64;
	// mr r7,r27
	ctx.r7.u64 = ctx.r27.u64;
	// lbz r11,-16940(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -16940);
	// li r6,-1
	ctx.r6.s64 = -1;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// li r5,0
	ctx.r5.s64 = 0;
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// stw r28,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r28.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,0
	ctx.r9.s64 = 0;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x82223a58
	ctx.lr = 0x825D70EC;
	sub_82223A58(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16940(r30)
	PPC_STORE_U8(ctx.r30.u32 + -16940, ctx.r11.u8);
	// addi r1,r1,224
	ctx.r1.s64 = ctx.r1.s64 + 224;
	// b 0x829ff80c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D7100);
PPC_WEAK_FUNC(sub_825D7100) { __imp__sub_825D7100(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D7100) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7bc
	ctx.lr = 0x825D7108;
	__savegprlr_25(ctx, base);
	// stwu r1,-224(r1)
	ea = -224 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,-1640
	ctx.r31.s64 = ctx.r11.s64 + -1640;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// mr r26,r7
	ctx.r26.u64 = ctx.r7.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D7130;
	sub_8221FD88(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// mr r25,r3
	ctx.r25.u64 = ctx.r3.u64;
	// addi r30,r11,-10248
	ctx.r30.s64 = ctx.r11.s64 + -10248;
	// li r5,1
	ctx.r5.s64 = 1;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x821fe1c0
	ctx.lr = 0x825D714C;
	sub_821FE1C0(ctx, base);
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// li r5,1
	ctx.r5.s64 = 1;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x821fe1c0
	ctx.lr = 0x825D7160;
	sub_821FE1C0(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// li r10,0
	ctx.r10.s64 = 0;
	// lwz r4,604(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// mr r28,r3
	ctx.r28.u64 = ctx.r3.u64;
	// stw r29,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r29.u32);
	// mr r8,r26
	ctx.r8.u64 = ctx.r26.u64;
	// mr r7,r27
	ctx.r7.u64 = ctx.r27.u64;
	// lbz r11,-16940(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -16940);
	// li r6,-1
	ctx.r6.s64 = -1;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// li r5,0
	ctx.r5.s64 = 0;
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// stw r28,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r28.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,1
	ctx.r9.s64 = 1;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x82223a58
	ctx.lr = 0x825D71C4;
	sub_82223A58(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16940(r30)
	PPC_STORE_U8(ctx.r30.u32 + -16940, ctx.r11.u8);
	// addi r1,r1,224
	ctx.r1.s64 = ctx.r1.s64 + 224;
	// b 0x829ff80c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D71D8);
PPC_WEAK_FUNC(sub_825D71D8) { __imp__sub_825D71D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D71D8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7a8
	ctx.lr = 0x825D71E0;
	__savegprlr_20(ctx, base);
	// stwu r1,-1808(r1)
	ea = -1808 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32076
	ctx.r11.s64 = -2102132736;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r29,r11,-26224
	ctx.r29.s64 = ctx.r11.s64 + -26224;
	// lis r11,-32076
	ctx.r11.s64 = -2102132736;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// addi r28,r11,-25960
	ctx.r28.s64 = ctx.r11.s64 + -25960;
	// mr r23,r5
	ctx.r23.u64 = ctx.r5.u64;
	// mr r22,r6
	ctx.r22.u64 = ctx.r6.u64;
	// mr r21,r7
	ctx.r21.u64 = ctx.r7.u64;
	// mr r27,r8
	ctx.r27.u64 = ctx.r8.u64;
	// lwz r11,196(r28)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r28.u32 + 196);
	// mr r26,r9
	ctx.r26.u64 = ctx.r9.u64;
	// mr r25,r10
	ctx.r25.u64 = ctx.r10.u64;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// li r20,0
	ctx.r20.s64 = 0;
	// lwzx r3,r11,r29
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + ctx.r29.u32);
	// bl 0x821c2fa8
	ctx.lr = 0x825D7228;
	sub_821C2FA8(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d7398
	if (ctx.cr6.eq) goto loc_825D7398;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// addi r24,r11,-1640
	ctx.r24.s64 = ctx.r11.s64 + -1640;
	// mr r3,r24
	ctx.r3.u64 = ctx.r24.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D7248;
	sub_8221FD88(ctx, base);
	// addi r11,r30,-4
	ctx.r11.s64 = ctx.r30.s64 + -4;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x825d728c
	if (ctx.cr6.gt) goto loc_825D728C;
	// li r4,-1
	ctx.r4.s64 = -1;
	// lwz r9,1908(r1)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 1908);
	// addi r10,r1,96
	ctx.r10.s64 = ctx.r1.s64 + 96;
	// lwz r8,1900(r1)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r1.u32 + 1900);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lwz r7,1892(r1)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + 1892);
	// mr r6,r25
	ctx.r6.u64 = ctx.r25.u64;
	// mr r5,r26
	ctx.r5.u64 = ctx.r26.u64;
	// stw r4,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r4.u32);
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// stw r10,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r10.u32);
	// lfs f1,2612(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2612);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x822219b0
	ctx.lr = 0x825D7288;
	sub_822219B0(ctx, base);
	// b 0x825d729c
	goto loc_825D729C;
loc_825D728C:
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// li r5,400
	ctx.r5.s64 = 400;
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// bl 0x822217b0
	ctx.lr = 0x825D729C;
	sub_822217B0(ctx, base);
loc_825D729C:
	// cmpwi cr6,r30,1
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 1, ctx.xer);
	// bne cr6,0x825d72d4
	if (!ctx.cr6.eq) goto loc_825D72D4;
	// mr r4,r23
	ctx.r4.u64 = ctx.r23.u64;
	// mr r3,r24
	ctx.r3.u64 = ctx.r24.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D72B0;
	sub_8221FD88(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// li r9,-1
	ctx.r9.s64 = -1;
	// li r8,-1
	ctx.r8.s64 = -1;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// bl 0x82222698
	ctx.lr = 0x825D72D0;
	sub_82222698(ctx, base);
	// b 0x825d7334
	goto loc_825D7334;
loc_825D72D4:
	// cmpwi cr6,r30,2
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 2, ctx.xer);
	// beq cr6,0x825d72e4
	if (ctx.cr6.eq) goto loc_825D72E4;
	// cmpwi cr6,r30,3
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 3, ctx.xer);
	// bne cr6,0x825d7334
	if (!ctx.cr6.eq) goto loc_825D7334;
loc_825D72E4:
	// cmplwi cr6,r22,0
	ctx.cr6.compare<uint32_t>(ctx.r22.u32, 0, ctx.xer);
	// beq cr6,0x825d7300
	if (ctx.cr6.eq) goto loc_825D7300;
	// li r5,400
	ctx.r5.s64 = 400;
	// mr r4,r22
	ctx.r4.u64 = ctx.r22.u64;
	// addi r3,r1,896
	ctx.r3.s64 = ctx.r1.s64 + 896;
	// bl 0x82221838
	ctx.lr = 0x825D72FC;
	sub_82221838(ctx, base);
	// b 0x825d7304
	goto loc_825D7304;
loc_825D7300:
	// stb r20,896(r1)
	PPC_STORE_U8(ctx.r1.u32 + 896, ctx.r20.u8);
loc_825D7304:
	// cmplwi cr6,r21,0
	ctx.cr6.compare<uint32_t>(ctx.r21.u32, 0, ctx.xer);
	// beq cr6,0x825d7320
	if (ctx.cr6.eq) goto loc_825D7320;
	// li r5,400
	ctx.r5.s64 = 400;
	// mr r4,r21
	ctx.r4.u64 = ctx.r21.u64;
	// addi r3,r1,1296
	ctx.r3.s64 = ctx.r1.s64 + 1296;
	// bl 0x82221838
	ctx.lr = 0x825D731C;
	sub_82221838(ctx, base);
	// b 0x825d7324
	goto loc_825D7324;
loc_825D7320:
	// stb r20,1296(r1)
	PPC_STORE_U8(ctx.r1.u32 + 1296, ctx.r20.u8);
loc_825D7324:
	// addi r5,r1,1296
	ctx.r5.s64 = ctx.r1.s64 + 1296;
	// addi r4,r1,896
	ctx.r4.s64 = ctx.r1.s64 + 896;
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// bl 0x82221c28
	ctx.lr = 0x825D7334;
	sub_82221C28(ctx, base);
loc_825D7334:
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// bl 0x822bca90
	ctx.lr = 0x825D733C;
	sub_822BCA90(ctx, base);
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// bl 0x82221768
	ctx.lr = 0x825D7344;
	sub_82221768(ctx, base);
	// lwz r11,196(r28)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r28.u32 + 196);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r11,r11,r29
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + ctx.r29.u32);
	// addi r3,r11,108
	ctx.r3.s64 = ctx.r11.s64 + 108;
	// bl 0x82221768
	ctx.lr = 0x825D735C;
	sub_82221768(ctx, base);
	// clrlwi r11,r31,16
	ctx.r11.u64 = ctx.r31.u32 & 0xFFFF;
	// clrlwi r10,r3,16
	ctx.r10.u64 = ctx.r3.u32 & 0xFFFF;
	// cmplw cr6,r11,r10
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, ctx.r10.u32, ctx.xer);
	// bne cr6,0x825d7398
	if (!ctx.cr6.eq) goto loc_825D7398;
	// lwz r11,196(r28)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r28.u32 + 196);
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r11,r11,r29
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + ctx.r29.u32);
	// addi r4,r11,108
	ctx.r4.s64 = ctx.r11.s64 + 108;
	// bl 0x822218c0
	ctx.lr = 0x825D7388;
	sub_822218C0(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// li r3,1
	ctx.r3.s64 = 1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825d739c
	if (!ctx.cr6.eq) goto loc_825D739C;
loc_825D7398:
	// mr r3,r20
	ctx.r3.u64 = ctx.r20.u64;
loc_825D739C:
	// addi r1,r1,1808
	ctx.r1.s64 = ctx.r1.s64 + 1808;
	// b 0x829ff7f8
	__restgprlr_20(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D73A8);
PPC_WEAK_FUNC(sub_825D73A8) { __imp__sub_825D73A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D73A8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r30,r11,-1640
	ctx.r30.s64 = ctx.r11.s64 + -1640;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8221f948
	ctx.lr = 0x825D73D4;
	sub_8221F948(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d73f8
	if (ctx.cr6.eq) goto loc_825D73F8;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D73EC;
	sub_8221FD88(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// bl 0x8221f068
	ctx.lr = 0x825D73F4;
	sub_8221F068(ctx, base);
	// b 0x825d7400
	goto loc_825D7400;
loc_825D73F8:
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// lwz r3,-17288(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + -17288);
loc_825D7400:
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D7418);
PPC_WEAK_FUNC(sub_825D7418) { __imp__sub_825D7418(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D7418) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7cc
	ctx.lr = 0x825D7420;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// cmpwi cr6,r29,0
	ctx.cr6.compare<int32_t>(ctx.r29.s32, 0, ctx.xer);
	// blt cr6,0x825d74c8
	if (ctx.cr6.lt) goto loc_825D74C8;
	// cmpwi cr6,r29,1024
	ctx.cr6.compare<int32_t>(ctx.r29.s32, 1024, ctx.xer);
	// bge cr6,0x825d74c8
	if (!ctx.cr6.lt) goto loc_825D74C8;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// addi r31,r11,-1640
	ctx.r31.s64 = ctx.r11.s64 + -1640;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8221f948
	ctx.lr = 0x825D7450;
	sub_8221F948(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d74c8
	if (ctx.cr6.eq) goto loc_825D74C8;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D7468;
	sub_8221FD88(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// bl 0x8221f068
	ctx.lr = 0x825D7470;
	sub_8221F068(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r11,r4
	ctx.r11.u64 = ctx.r4.u64;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_825D747C:
	// lbz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// bne cr6,0x825d747c
	if (!ctx.cr6.eq) goto loc_825D747C;
	// subf r11,r10,r11
	ctx.r11.u64 = ctx.r11.u64 - ctx.r10.u64;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// rotlwi r11,r11,0
	ctx.r11.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// cmpw cr6,r29,r11
	ctx.cr6.compare<int32_t>(ctx.r29.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x825d74c8
	if (ctx.cr6.gt) goto loc_825D74C8;
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// mr r5,r29
	ctx.r5.u64 = ctx.r29.u64;
	// addi r31,r11,952
	ctx.r31.s64 = ctx.r11.s64 + 952;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a00ba0
	ctx.lr = 0x825D74B4;
	rexcrt_strncpy(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stbx r11,r31,r29
	PPC_STORE_U8(ctx.r31.u32 + ctx.r29.u32, ctx.r11.u8);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
loc_825D74C8:
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// lwz r3,-17288(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + -17288);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D74D8);
PPC_WEAK_FUNC(sub_825D74D8) { __imp__sub_825D74D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D74D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x825d7504
	if (!ctx.cr6.lt) goto loc_825D7504;
loc_825D74F8:
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// lwz r3,-17288(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + -17288);
	// b 0x825d755c
	goto loc_825D755C;
loc_825D7504:
	// cmpwi cr6,r30,1024
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 1024, ctx.xer);
	// bge cr6,0x825d74f8
	if (!ctx.cr6.lt) goto loc_825D74F8;
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_825D7514:
	// lbz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// bne cr6,0x825d7514
	if (!ctx.cr6.eq) goto loc_825D7514;
	// subf r11,r10,r11
	ctx.r11.u64 = ctx.r11.u64 - ctx.r10.u64;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// rotlwi r11,r11,0
	ctx.r11.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// cmpw cr6,r30,r11
	ctx.cr6.compare<int32_t>(ctx.r30.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x825d74f8
	if (ctx.cr6.gt) goto loc_825D74F8;
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r31,r11,1976
	ctx.r31.s64 = ctx.r11.s64 + 1976;
	// mr r5,r30
	ctx.r5.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a00ba0
	ctx.lr = 0x825D7550;
	rexcrt_strncpy(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stbx r11,r31,r30
	PPC_STORE_U8(ctx.r31.u32 + ctx.r30.u32, ctx.r11.u8);
loc_825D755C:
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D7578);
PPC_WEAK_FUNC(sub_825D7578) { __imp__sub_825D7578(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D7578) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r30,r11,-1640
	ctx.r30.s64 = ctx.r11.s64 + -1640;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8221f948
	ctx.lr = 0x825D75A4;
	sub_8221F948(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d75e8
	if (ctx.cr6.eq) goto loc_825D75E8;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D75BC;
	sub_8221FD88(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// bl 0x8221f068
	ctx.lr = 0x825D75C4;
	sub_8221F068(ctx, base);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
loc_825D75C8:
	// lbz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r3.u32 + 0);
	// addi r3,r3,1
	ctx.r3.s64 = ctx.r3.s64 + 1;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// bne cr6,0x825d75c8
	if (!ctx.cr6.eq) goto loc_825D75C8;
	// subf r11,r11,r3
	ctx.r11.u64 = ctx.r3.u64 - ctx.r11.u64;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// rotlwi r3,r11,0
	ctx.r3.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// b 0x825d75ec
	goto loc_825D75EC;
loc_825D75E8:
	// li r3,0
	ctx.r3.s64 = 0;
loc_825D75EC:
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D7608);
PPC_WEAK_FUNC(sub_825D7608) { __imp__sub_825D7608(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D7608) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c8
	ctx.lr = 0x825D7610;
	__savegprlr_28(ctx, base);
	// stwu r1,-1152(r1)
	ea = -1152 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// mr r31,r5
	ctx.r31.u64 = ctx.r5.u64;
	// cmpwi cr6,r29,0
	ctx.cr6.compare<int32_t>(ctx.r29.s32, 0, ctx.xer);
	// bge cr6,0x825d7634
	if (!ctx.cr6.lt) goto loc_825D7634;
loc_825D7624:
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// lwz r3,-17288(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + -17288);
	// addi r1,r1,1152
	ctx.r1.s64 = ctx.r1.s64 + 1152;
	// b 0x829ff818
	__restgprlr_28(ctx, base);
	return;
loc_825D7634:
	// cmpwi cr6,r31,1024
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 1024, ctx.xer);
	// bge cr6,0x825d7624
	if (!ctx.cr6.lt) goto loc_825D7624;
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_825D7644:
	// lbz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// bne cr6,0x825d7644
	if (!ctx.cr6.eq) goto loc_825D7644;
	// subf r11,r10,r11
	ctx.r11.u64 = ctx.r11.u64 - ctx.r10.u64;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// rotlwi r11,r11,0
	ctx.r11.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// cmpw cr6,r31,r11
	ctx.cr6.compare<int32_t>(ctx.r31.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x825d7624
	if (ctx.cr6.gt) goto loc_825D7624;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x82a00ba0
	ctx.lr = 0x825D7678;
	rexcrt_strncpy(ctx, base);
	// addi r10,r1,80
	ctx.r10.s64 = ctx.r1.s64 + 80;
	// li r28,0
	ctx.r28.s64 = 0;
	// cmpw cr6,r29,r31
	ctx.cr6.compare<int32_t>(ctx.r29.s32, ctx.r31.s32, ctx.xer);
	// mr r11,r28
	ctx.r11.u64 = ctx.r28.u64;
	// stbx r28,r31,r10
	PPC_STORE_U8(ctx.r31.u32 + ctx.r10.u32, ctx.r28.u8);
	// lis r10,-31992
	ctx.r10.s64 = -2096627712;
	// addi r30,r10,3000
	ctx.r30.s64 = ctx.r10.s64 + 3000;
	// bgt cr6,0x825d76b8
	if (ctx.cr6.gt) goto loc_825D76B8;
	// subf r11,r29,r31
	ctx.r11.u64 = ctx.r31.u64 - ctx.r29.u64;
	// addi r10,r1,80
	ctx.r10.s64 = ctx.r1.s64 + 80;
	// addi r31,r11,1
	ctx.r31.s64 = ctx.r11.s64 + 1;
	// add r4,r29,r10
	ctx.r4.u64 = ctx.r29.u64 + ctx.r10.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// bl 0x82a01248
	ctx.lr = 0x825D76B4;
	sub_82A01248(ctx, base);
	// mr r11,r31
	ctx.r11.u64 = ctx.r31.u64;
loc_825D76B8:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// stbx r28,r11,r30
	PPC_STORE_U8(ctx.r11.u32 + ctx.r30.u32, ctx.r28.u8);
	// addi r1,r1,1152
	ctx.r1.s64 = ctx.r1.s64 + 1152;
	// b 0x829ff818
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D76C8);
PPC_WEAK_FUNC(sub_825D76C8) { __imp__sub_825D76C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D76C8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7cc
	ctx.lr = 0x825D76D0;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r30,r11,-1640
	ctx.r30.s64 = ctx.r11.s64 + -1640;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8221f948
	ctx.lr = 0x825D76F0;
	sub_8221F948(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d7760
	if (ctx.cr6.eq) goto loc_825D7760;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D7708;
	sub_8221FD88(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// bl 0x8221f068
	ctx.lr = 0x825D7710;
	sub_8221F068(ctx, base);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_825D7718:
	// lbz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// bne cr6,0x825d7718
	if (!ctx.cr6.eq) goto loc_825D7718;
	// subf r11,r10,r11
	ctx.r11.u64 = ctx.r11.u64 - ctx.r10.u64;
	// cmpwi cr6,r29,0
	ctx.cr6.compare<int32_t>(ctx.r29.s32, 0, ctx.xer);
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// rotlwi r11,r11,0
	ctx.r11.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// blt cr6,0x825d7760
	if (ctx.cr6.lt) goto loc_825D7760;
	// cmpw cr6,r29,r11
	ctx.cr6.compare<int32_t>(ctx.r29.s32, ctx.r11.s32, ctx.xer);
	// bge cr6,0x825d7760
	if (!ctx.cr6.lt) goto loc_825D7760;
	// lbzx r11,r3,r29
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + ctx.r29.u32);
	// extsb r11,r11
	ctx.r11.s64 = ctx.r11.s8;
	// cmpwi cr6,r11,48
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 48, ctx.xer);
	// blt cr6,0x825d7760
	if (ctx.cr6.lt) goto loc_825D7760;
	// cmpwi cr6,r11,57
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 57, ctx.xer);
	// addi r3,r11,-48
	ctx.r3.s64 = ctx.r11.s64 + -48;
	// ble cr6,0x825d7764
	if (!ctx.cr6.gt) goto loc_825D7764;
loc_825D7760:
	// li r3,-1
	ctx.r3.s64 = -1;
loc_825D7764:
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D7770);
PPC_WEAK_FUNC(sub_825D7770) { __imp__sub_825D7770(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D7770) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7cc
	ctx.lr = 0x825D7778;
	__savegprlr_29(ctx, base);
	// stfd f30,-48(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -48, ctx.f30.u64);
	// stfd f31,-40(r1)
	PPC_STORE_U64(ctx.r1.u32 + -40, ctx.f31.u64);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// addi r31,r10,32256
	ctx.r31.s64 = ctx.r10.s64 + 32256;
	// lwz r11,-536(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// addi r10,r31,25
	ctx.r10.s64 = ctx.r31.s64 + 25;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lbzx r10,r11,r10
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + ctx.r10.u32);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x825d77bc
	if (ctx.cr6.eq) goto loc_825D77BC;
	// li r3,2
	ctx.r3.s64 = 2;
	// b 0x825d77ec
	goto loc_825D77EC;
loc_825D77BC:
	// addi r10,r31,26
	ctx.r10.s64 = ctx.r31.s64 + 26;
	// lbzx r10,r11,r10
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + ctx.r10.u32);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x825d77d4
	if (ctx.cr6.eq) goto loc_825D77D4;
	// li r3,0
	ctx.r3.s64 = 0;
	// b 0x825d77ec
	goto loc_825D77EC;
loc_825D77D4:
	// addi r10,r31,24
	ctx.r10.s64 = ctx.r31.s64 + 24;
	// li r3,3
	ctx.r3.s64 = 3;
	// lbzx r11,r11,r10
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + ctx.r10.u32);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825d77ec
	if (!ctx.cr6.eq) goto loc_825D77EC;
	// li r3,1
	ctx.r3.s64 = 1;
loc_825D77EC:
	// bl 0x821f3380
	ctx.lr = 0x825D77F0;
	sub_821F3380(ctx, base);
	// lwz r11,-536(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// addi r10,r31,36
	ctx.r10.s64 = ctx.r31.s64 + 36;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lfsx f2,r11,r10
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + ctx.r10.u32);
	ctx.f2.f64 = double(temp.f32);
	// addi r9,r31,32
	ctx.r9.s64 = ctx.r31.s64 + 32;
	// lfsx f1,r11,r9
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + ctx.r9.u32);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x821f2fd8
	ctx.lr = 0x825D780C;
	sub_821F2FD8(ctx, base);
	// lwz r11,-536(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// addi r10,r31,29
	ctx.r10.s64 = ctx.r31.s64 + 29;
	// li r4,0
	ctx.r4.s64 = 0;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lbzx r3,r11,r10
	ctx.r3.u64 = PPC_LOAD_U8(ctx.r11.u32 + ctx.r10.u32);
	// bl 0x821f32b8
	ctx.lr = 0x825D7824;
	sub_821F32B8(ctx, base);
	// lwz r11,-536(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// addi r10,r31,27
	ctx.r10.s64 = ctx.r31.s64 + 27;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lbzx r3,r11,r10
	ctx.r3.u64 = PPC_LOAD_U8(ctx.r11.u32 + ctx.r10.u32);
	// bl 0x821f3278
	ctx.lr = 0x825D7838;
	sub_821F3278(ctx, base);
	// lwz r11,-536(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// addi r10,r31,57
	ctx.r10.s64 = ctx.r31.s64 + 57;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lbzx r10,r11,r10
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + ctx.r10.u32);
	// extsb r10,r10
	ctx.r10.s64 = ctx.r10.s8;
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// beq cr6,0x825d7870
	if (ctx.cr6.eq) goto loc_825D7870;
	// extsw r11,r10
	ctx.r11.s64 = ctx.r10.s32;
	// std r11,80(r1)
	PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r11.u64);
	// lfd f0,80(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f0.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
	// fcfid f0,f0
	ctx.f0.f64 = double(ctx.f0.s64);
	// frsp f1,f0
	ctx.f1.f64 = double(float(ctx.f0.f64));
	// bl 0x821f3160
	ctx.lr = 0x825D786C;
	sub_821F3160(ctx, base);
	// b 0x825d7890
	goto loc_825D7890;
loc_825D7870:
	// addi r10,r31,56
	ctx.r10.s64 = ctx.r31.s64 + 56;
	// lbzx r11,r11,r10
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + ctx.r10.u32);
	// extsb r11,r11
	ctx.r11.s64 = ctx.r11.s8;
	// std r11,80(r1)
	PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r11.u64);
	// lfd f0,80(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f0.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
	// fcfid f0,f0
	ctx.f0.f64 = double(ctx.f0.s64);
	// frsp f1,f0
	ctx.f1.f64 = double(float(ctx.f0.f64));
	// bl 0x821f3100
	ctx.lr = 0x825D7890;
	sub_821F3100(ctx, base);
loc_825D7890:
	// lwz r11,-536(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// addi r10,r31,60
	ctx.r10.s64 = ctx.r31.s64 + 60;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lwzx r11,r11,r10
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + ctx.r10.u32);
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// bl 0x821f2ed0
	ctx.lr = 0x825D78A8;
	sub_821F2ED0(ctx, base);
	// lwz r11,-536(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// addi r10,r31,8
	ctx.r10.s64 = ctx.r31.s64 + 8;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lfsx f2,r11,r10
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + ctx.r10.u32);
	ctx.f2.f64 = double(temp.f32);
	// addi r9,r31,4
	ctx.r9.s64 = ctx.r31.s64 + 4;
	// lfsx f1,r11,r9
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + ctx.r9.u32);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x821f2d38
	ctx.lr = 0x825D78C4;
	sub_821F2D38(ctx, base);
	// mr r5,r29
	ctx.r5.u64 = ctx.r29.u64;
	// fmr f2,f30
	ctx.fpscr.disableFlushMode();
	ctx.f2.f64 = ctx.f30.f64;
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// bl 0x821f6bf8
	ctx.lr = 0x825D78D4;
	sub_821F6BF8(ctx, base);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lfd f30,-48(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -48);
	// lfd f31,-40(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -40);
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D78E8);
PPC_WEAK_FUNC(sub_825D78E8) { __imp__sub_825D78E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D78E8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c4
	ctx.lr = 0x825D78F0;
	__savegprlr_27(ctx, base);
	// stfd f30,-64(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -64, ctx.f30.u64);
	// stfd f31,-56(r1)
	PPC_STORE_U64(ctx.r1.u32 + -56, ctx.f31.u64);
	// stwu r1,-544(r1)
	ea = -544 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// mr r4,r5
	ctx.r4.u64 = ctx.r5.u64;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// addi r30,r11,-1640
	ctx.r30.s64 = ctx.r11.s64 + -1640;
	// mr r31,r6
	ctx.r31.u64 = ctx.r6.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// mr r29,r7
	ctx.r29.u64 = ctx.r7.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D7920;
	sub_8221FD88(ctx, base);
	// mr r11,r31
	ctx.r11.u64 = ctx.r31.u64;
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// li r28,0
	ctx.r28.s64 = 0;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_825D7930:
	// lbz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// bne cr6,0x825d7930
	if (!ctx.cr6.eq) goto loc_825D7930;
	// subf r11,r10,r11
	ctx.r11.u64 = ctx.r11.u64 - ctx.r10.u64;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// rotlwi r11,r11,0
	ctx.r11.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d7964
	if (ctx.cr6.eq) goto loc_825D7964;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D7960;
	sub_8221FD88(ctx, base);
	// mr r28,r3
	ctx.r28.u64 = ctx.r3.u64;
loc_825D7964:
	// mr r11,r29
	ctx.r11.u64 = ctx.r29.u64;
	// li r31,0
	ctx.r31.s64 = 0;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_825D7970:
	// lbz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// bne cr6,0x825d7970
	if (!ctx.cr6.eq) goto loc_825D7970;
	// subf r11,r10,r11
	ctx.r11.u64 = ctx.r11.u64 - ctx.r10.u64;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// rotlwi r11,r11,0
	ctx.r11.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d79a4
	if (ctx.cr6.eq) goto loc_825D79A4;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D79A0;
	sub_8221FD88(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
loc_825D79A4:
	// li r5,400
	ctx.r5.s64 = 400;
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x822217b0
	ctx.lr = 0x825D79B4;
	sub_822217B0(ctx, base);
	// li r9,-1
	ctx.r9.s64 = -1;
	// li r8,-1
	ctx.r8.s64 = -1;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x82222698
	ctx.lr = 0x825D79D4;
	sub_82222698(ctx, base);
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// fmr f2,f30
	ctx.fpscr.disableFlushMode();
	ctx.f2.f64 = ctx.f30.f64;
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// bl 0x825d7770
	ctx.lr = 0x825D79E4;
	sub_825D7770(ctx, base);
	// addi r1,r1,544
	ctx.r1.s64 = ctx.r1.s64 + 544;
	// lfd f30,-64(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -64);
	// lfd f31,-56(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -56);
	// b 0x829ff814
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D79F8);
PPC_WEAK_FUNC(sub_825D79F8) { __imp__sub_825D79F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D79F8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c8
	ctx.lr = 0x825D7A00;
	__savegprlr_28(ctx, base);
	// stfd f30,-56(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -56, ctx.f30.u64);
	// stfd f31,-48(r1)
	PPC_STORE_U64(ctx.r1.u32 + -48, ctx.f31.u64);
	// stwu r1,-544(r1)
	ea = -544 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// mr r4,r5
	ctx.r4.u64 = ctx.r5.u64;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// addi r3,r11,-1640
	ctx.r3.s64 = ctx.r11.s64 + -1640;
	// mr r30,r6
	ctx.r30.u64 = ctx.r6.u64;
	// mr r29,r7
	ctx.r29.u64 = ctx.r7.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D7A2C;
	sub_8221FD88(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// mr r28,r3
	ctx.r28.u64 = ctx.r3.u64;
	// addi r31,r11,-10248
	ctx.r31.s64 = ctx.r11.s64 + -10248;
	// li r5,0
	ctx.r5.s64 = 0;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x821fe1c0
	ctx.lr = 0x825D7A48;
	sub_821FE1C0(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// li r5,0
	ctx.r5.s64 = 0;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x821fe1c0
	ctx.lr = 0x825D7A5C;
	sub_821FE1C0(ctx, base);
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// li r5,400
	ctx.r5.s64 = 400;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x822217b0
	ctx.lr = 0x825D7A70;
	sub_822217B0(ctx, base);
	// mr r9,r29
	ctx.r9.u64 = ctx.r29.u64;
	// mr r8,r30
	ctx.r8.u64 = ctx.r30.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,1
	ctx.r6.s64 = 1;
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x82222698
	ctx.lr = 0x825D7A90;
	sub_82222698(ctx, base);
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// fmr f2,f30
	ctx.fpscr.disableFlushMode();
	ctx.f2.f64 = ctx.f30.f64;
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// bl 0x825d7770
	ctx.lr = 0x825D7AA0;
	sub_825D7770(ctx, base);
	// mr r28,r3
	ctx.r28.u64 = ctx.r3.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x821fe130
	ctx.lr = 0x825D7AB0;
	sub_821FE130(ctx, base);
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x821fe130
	ctx.lr = 0x825D7ABC;
	sub_821FE130(ctx, base);
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// addi r1,r1,544
	ctx.r1.s64 = ctx.r1.s64 + 544;
	// lfd f30,-56(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -56);
	// lfd f31,-48(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -48);
	// b 0x829ff818
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D7AD0);
PPC_WEAK_FUNC(sub_825D7AD0) { __imp__sub_825D7AD0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D7AD0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c4
	ctx.lr = 0x825D7AD8;
	__savegprlr_27(ctx, base);
	// stfd f30,-64(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -64, ctx.f30.u64);
	// stfd f31,-56(r1)
	PPC_STORE_U64(ctx.r1.u32 + -56, ctx.f31.u64);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// addi r3,r11,-1640
	ctx.r3.s64 = ctx.r11.s64 + -1640;
	// bl 0x8221fd88
	ctx.lr = 0x825D7B00;
	sub_8221FD88(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lis r9,-32070
	ctx.r9.s64 = -2101739520;
	// addi r31,r10,32256
	ctx.r31.s64 = ctx.r10.s64 + 32256;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,-536(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// addi r7,r31,64
	ctx.r7.s64 = ctx.r31.s64 + 64;
	// addi r6,r31,68
	ctx.r6.s64 = ctx.r31.s64 + 68;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lbz r9,7580(r9)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + 7580);
	// lwz r10,-540(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -540);
	// stfsx f31,r11,r7
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, temp.u32);
	// stfsx f30,r11,r6
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, temp.u32);
	// addi r28,r31,12
	ctx.r28.s64 = ctx.r31.s64 + 12;
	// addi r27,r31,58
	ctx.r27.s64 = ctx.r31.s64 + 58;
	// addi r8,r31,72
	ctx.r8.s64 = ctx.r31.s64 + 72;
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// add r3,r11,r8
	ctx.r3.u64 = ctx.r11.u64 + ctx.r8.u64;
	// stwx r10,r11,r28
	PPC_STORE_U32(ctx.r11.u32 + ctx.r28.u32, ctx.r10.u32);
	// stbx r9,r11,r27
	PPC_STORE_U8(ctx.r11.u32 + ctx.r27.u32, ctx.r9.u8);
	// bl 0x82a00ba0
	ctx.lr = 0x825D7B58;
	rexcrt_strncpy(ctx, base);
	// lwz r3,-536(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// bl 0x821fe0b8
	ctx.lr = 0x825D7B60;
	sub_821FE0B8(ctx, base);
	// lwz r11,-536(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lis r9,-32070
	ctx.r9.s64 = -2101739520;
	// mulli r8,r11,124
	ctx.r8.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lwz r10,7576(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 7576);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// stwx r10,r8,r31
	PPC_STORE_U32(ctx.r8.u32 + ctx.r31.u32, ctx.r10.u32);
	// stw r11,-536(r30)
	PPC_STORE_U32(ctx.r30.u32 + -536, ctx.r11.u32);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16913(r9)
	PPC_STORE_U8(ctx.r9.u32 + -16913, ctx.r11.u8);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lfd f30,-64(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -64);
	// lfd f31,-56(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -56);
	// b 0x829ff814
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D7B98);
PPC_WEAK_FUNC(sub_825D7B98) { __imp__sub_825D7B98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D7B98) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c0
	ctx.lr = 0x825D7BA0;
	__savegprlr_26(ctx, base);
	// stfd f30,-72(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -72, ctx.f30.u64);
	// stfd f31,-64(r1)
	PPC_STORE_U64(ctx.r1.u32 + -64, ctx.f31.u64);
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// addi r3,r11,-1640
	ctx.r3.s64 = ctx.r11.s64 + -1640;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D7BCC;
	sub_8221FD88(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lis r9,-32070
	ctx.r9.s64 = -2101739520;
	// addi r31,r10,32256
	ctx.r31.s64 = ctx.r10.s64 + 32256;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,-536(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// addi r7,r31,64
	ctx.r7.s64 = ctx.r31.s64 + 64;
	// addi r6,r31,68
	ctx.r6.s64 = ctx.r31.s64 + 68;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lbz r9,7580(r9)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + 7580);
	// lwz r10,-540(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -540);
	// stfsx f31,r11,r7
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, temp.u32);
	// stfsx f30,r11,r6
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, temp.u32);
	// addi r27,r31,12
	ctx.r27.s64 = ctx.r31.s64 + 12;
	// addi r26,r31,58
	ctx.r26.s64 = ctx.r31.s64 + 58;
	// addi r8,r31,72
	ctx.r8.s64 = ctx.r31.s64 + 72;
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// add r3,r11,r8
	ctx.r3.u64 = ctx.r11.u64 + ctx.r8.u64;
	// stwx r10,r11,r27
	PPC_STORE_U32(ctx.r11.u32 + ctx.r27.u32, ctx.r10.u32);
	// stbx r9,r11,r26
	PPC_STORE_U8(ctx.r11.u32 + ctx.r26.u32, ctx.r9.u8);
	// bl 0x82a00ba0
	ctx.lr = 0x825D7C24;
	rexcrt_strncpy(ctx, base);
	// lwz r3,-536(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// bl 0x821fe0b8
	ctx.lr = 0x825D7C2C;
	sub_821FE0B8(ctx, base);
	// lwz r11,-536(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// lis r9,-32070
	ctx.r9.s64 = -2101739520;
	// addi r7,r31,96
	ctx.r7.s64 = ctx.r31.s64 + 96;
	// mulli r10,r11,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lwz r9,7576(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + 7576);
	// stwx r28,r10,r7
	PPC_STORE_U32(ctx.r10.u32 + ctx.r7.u32, ctx.r28.u32);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// lis r8,-32070
	ctx.r8.s64 = -2101739520;
	// stwx r9,r10,r31
	PPC_STORE_U32(ctx.r10.u32 + ctx.r31.u32, ctx.r9.u32);
	// stw r11,-536(r30)
	PPC_STORE_U32(ctx.r30.u32 + -536, ctx.r11.u32);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16913(r8)
	PPC_STORE_U8(ctx.r8.u32 + -16913, ctx.r11.u8);
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// lfd f30,-72(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -72);
	// lfd f31,-64(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -64);
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D7C70);
PPC_WEAK_FUNC(sub_825D7C70) { __imp__sub_825D7C70(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D7C70) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7bc
	ctx.lr = 0x825D7C78;
	__savegprlr_25(ctx, base);
	// stfd f30,-80(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -80, ctx.f30.u64);
	// stfd f31,-72(r1)
	PPC_STORE_U64(ctx.r1.u32 + -72, ctx.f31.u64);
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// addi r3,r11,-1640
	ctx.r3.s64 = ctx.r11.s64 + -1640;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D7CA8;
	sub_8221FD88(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lis r9,-32070
	ctx.r9.s64 = -2101739520;
	// addi r31,r10,32256
	ctx.r31.s64 = ctx.r10.s64 + 32256;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,-536(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// addi r7,r31,64
	ctx.r7.s64 = ctx.r31.s64 + 64;
	// addi r6,r31,68
	ctx.r6.s64 = ctx.r31.s64 + 68;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lbz r9,7580(r9)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + 7580);
	// lwz r10,-540(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -540);
	// stfsx f31,r11,r7
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, temp.u32);
	// stfsx f30,r11,r6
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, temp.u32);
	// addi r26,r31,12
	ctx.r26.s64 = ctx.r31.s64 + 12;
	// addi r25,r31,58
	ctx.r25.s64 = ctx.r31.s64 + 58;
	// addi r8,r31,72
	ctx.r8.s64 = ctx.r31.s64 + 72;
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// add r3,r11,r8
	ctx.r3.u64 = ctx.r11.u64 + ctx.r8.u64;
	// stwx r10,r11,r26
	PPC_STORE_U32(ctx.r11.u32 + ctx.r26.u32, ctx.r10.u32);
	// stbx r9,r11,r25
	PPC_STORE_U8(ctx.r11.u32 + ctx.r25.u32, ctx.r9.u8);
	// bl 0x82a00ba0
	ctx.lr = 0x825D7D00;
	rexcrt_strncpy(ctx, base);
	// lwz r3,-536(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// bl 0x821fe0b8
	ctx.lr = 0x825D7D08;
	sub_821FE0B8(ctx, base);
	// lwz r10,-536(r30)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// lis r9,-32070
	ctx.r9.s64 = -2101739520;
	// addi r7,r31,96
	ctx.r7.s64 = ctx.r31.s64 + 96;
	// mulli r11,r10,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// lwz r9,7576(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + 7576);
	// stwx r28,r11,r7
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, ctx.r28.u32);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r6,r31,100
	ctx.r6.s64 = ctx.r31.s64 + 100;
	// lis r8,-32070
	ctx.r8.s64 = -2101739520;
	// stw r10,-536(r30)
	PPC_STORE_U32(ctx.r30.u32 + -536, ctx.r10.u32);
	// li r10,1
	ctx.r10.s64 = 1;
	// stwx r27,r11,r6
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, ctx.r27.u32);
	// stwx r9,r11,r31
	PPC_STORE_U32(ctx.r11.u32 + ctx.r31.u32, ctx.r9.u32);
	// stb r10,-16913(r8)
	PPC_STORE_U8(ctx.r8.u32 + -16913, ctx.r10.u8);
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// lfd f30,-80(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -80);
	// lfd f31,-72(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -72);
	// b 0x829ff80c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D7D50);
PPC_WEAK_FUNC(sub_825D7D50) { __imp__sub_825D7D50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D7D50) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7b8
	ctx.lr = 0x825D7D58;
	__savegprlr_24(ctx, base);
	// stfd f30,-88(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -88, ctx.f30.u64);
	// stfd f31,-80(r1)
	PPC_STORE_U64(ctx.r1.u32 + -80, ctx.f31.u64);
	// stwu r1,-176(r1)
	ea = -176 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// addi r3,r11,-1640
	ctx.r3.s64 = ctx.r11.s64 + -1640;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// mr r26,r8
	ctx.r26.u64 = ctx.r8.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D7D8C;
	sub_8221FD88(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lis r9,-32070
	ctx.r9.s64 = -2101739520;
	// addi r31,r10,32256
	ctx.r31.s64 = ctx.r10.s64 + 32256;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,-536(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// addi r7,r31,64
	ctx.r7.s64 = ctx.r31.s64 + 64;
	// addi r6,r31,68
	ctx.r6.s64 = ctx.r31.s64 + 68;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lbz r9,7580(r9)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + 7580);
	// lwz r10,-540(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -540);
	// stfsx f31,r11,r7
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, temp.u32);
	// stfsx f30,r11,r6
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, temp.u32);
	// addi r25,r31,12
	ctx.r25.s64 = ctx.r31.s64 + 12;
	// addi r24,r31,58
	ctx.r24.s64 = ctx.r31.s64 + 58;
	// addi r8,r31,72
	ctx.r8.s64 = ctx.r31.s64 + 72;
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// add r3,r11,r8
	ctx.r3.u64 = ctx.r11.u64 + ctx.r8.u64;
	// stwx r10,r11,r25
	PPC_STORE_U32(ctx.r11.u32 + ctx.r25.u32, ctx.r10.u32);
	// stbx r9,r11,r24
	PPC_STORE_U8(ctx.r11.u32 + ctx.r24.u32, ctx.r9.u8);
	// bl 0x82a00ba0
	ctx.lr = 0x825D7DE4;
	rexcrt_strncpy(ctx, base);
	// lwz r3,-536(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// bl 0x821fe0b8
	ctx.lr = 0x825D7DEC;
	sub_821FE0B8(ctx, base);
	// lwz r10,-536(r30)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// lis r9,-32070
	ctx.r9.s64 = -2101739520;
	// addi r7,r31,96
	ctx.r7.s64 = ctx.r31.s64 + 96;
	// mulli r11,r10,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// lwz r9,7576(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + 7576);
	// stwx r28,r11,r7
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, ctx.r28.u32);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r6,r31,100
	ctx.r6.s64 = ctx.r31.s64 + 100;
	// addi r5,r31,104
	ctx.r5.s64 = ctx.r31.s64 + 104;
	// lis r8,-32070
	ctx.r8.s64 = -2101739520;
	// stw r10,-536(r30)
	PPC_STORE_U32(ctx.r30.u32 + -536, ctx.r10.u32);
	// li r10,1
	ctx.r10.s64 = 1;
	// stwx r27,r11,r6
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, ctx.r27.u32);
	// stwx r26,r11,r5
	PPC_STORE_U32(ctx.r11.u32 + ctx.r5.u32, ctx.r26.u32);
	// stwx r9,r11,r31
	PPC_STORE_U32(ctx.r11.u32 + ctx.r31.u32, ctx.r9.u32);
	// stb r10,-16913(r8)
	PPC_STORE_U8(ctx.r8.u32 + -16913, ctx.r10.u8);
	// addi r1,r1,176
	ctx.r1.s64 = ctx.r1.s64 + 176;
	// lfd f30,-88(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -88);
	// lfd f31,-80(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -80);
	// b 0x829ff808
	__restgprlr_24(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D7E40);
PPC_WEAK_FUNC(sub_825D7E40) { __imp__sub_825D7E40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D7E40) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c0
	ctx.lr = 0x825D7E48;
	__savegprlr_26(ctx, base);
	// stfd f29,-80(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -80, ctx.f29.u64);
	// stfd f30,-72(r1)
	PPC_STORE_U64(ctx.r1.u32 + -72, ctx.f30.u64);
	// stfd f31,-64(r1)
	PPC_STORE_U64(ctx.r1.u32 + -64, ctx.f31.u64);
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// fmr f29,f3
	ctx.f29.f64 = ctx.f3.f64;
	// addi r3,r11,-1640
	ctx.r3.s64 = ctx.r11.s64 + -1640;
	// mr r28,r7
	ctx.r28.u64 = ctx.r7.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D7E7C;
	sub_8221FD88(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lis r9,-32070
	ctx.r9.s64 = -2101739520;
	// addi r31,r10,32256
	ctx.r31.s64 = ctx.r10.s64 + 32256;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,-536(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// addi r7,r31,64
	ctx.r7.s64 = ctx.r31.s64 + 64;
	// addi r6,r31,68
	ctx.r6.s64 = ctx.r31.s64 + 68;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lbz r9,7580(r9)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + 7580);
	// lwz r10,-540(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -540);
	// stfsx f31,r11,r7
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, temp.u32);
	// stfsx f30,r11,r6
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, temp.u32);
	// addi r27,r31,12
	ctx.r27.s64 = ctx.r31.s64 + 12;
	// addi r26,r31,58
	ctx.r26.s64 = ctx.r31.s64 + 58;
	// addi r8,r31,72
	ctx.r8.s64 = ctx.r31.s64 + 72;
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// add r3,r11,r8
	ctx.r3.u64 = ctx.r11.u64 + ctx.r8.u64;
	// stwx r10,r11,r27
	PPC_STORE_U32(ctx.r11.u32 + ctx.r27.u32, ctx.r10.u32);
	// stbx r9,r11,r26
	PPC_STORE_U8(ctx.r11.u32 + ctx.r26.u32, ctx.r9.u8);
	// bl 0x82a00ba0
	ctx.lr = 0x825D7ED4;
	rexcrt_strncpy(ctx, base);
	// lwz r3,-536(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// bl 0x821fe0b8
	ctx.lr = 0x825D7EDC;
	sub_821FE0B8(ctx, base);
	// lwz r10,-536(r30)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// lis r9,-32070
	ctx.r9.s64 = -2101739520;
	// addi r7,r31,108
	ctx.r7.s64 = ctx.r31.s64 + 108;
	// mulli r11,r10,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// lwz r9,7576(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + 7576);
	// stfsx f29,r11,r7
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f29.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, temp.u32);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r6,r31,112
	ctx.r6.s64 = ctx.r31.s64 + 112;
	// lis r8,-32070
	ctx.r8.s64 = -2101739520;
	// stw r10,-536(r30)
	PPC_STORE_U32(ctx.r30.u32 + -536, ctx.r10.u32);
	// li r10,1
	ctx.r10.s64 = 1;
	// stwx r28,r11,r6
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, ctx.r28.u32);
	// stwx r9,r11,r31
	PPC_STORE_U32(ctx.r11.u32 + ctx.r31.u32, ctx.r9.u32);
	// stb r10,-16913(r8)
	PPC_STORE_U8(ctx.r8.u32 + -16913, ctx.r10.u8);
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// lfd f29,-80(r1)
	ctx.f29.u64 = PPC_LOAD_U64(ctx.r1.u32 + -80);
	// lfd f30,-72(r1)
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -72);
	// lfd f31,-64(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -64);
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D7F28);
PPC_WEAK_FUNC(sub_825D7F28) { __imp__sub_825D7F28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D7F28) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c0
	ctx.lr = 0x825D7F30;
	__savegprlr_26(ctx, base);
	// stfd f30,-72(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -72, ctx.f30.u64);
	// stfd f31,-64(r1)
	PPC_STORE_U64(ctx.r1.u32 + -64, ctx.f31.u64);
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// addi r3,r11,-1640
	ctx.r3.s64 = ctx.r11.s64 + -1640;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D7F5C;
	sub_8221FD88(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lis r9,-32070
	ctx.r9.s64 = -2101739520;
	// addi r31,r10,32256
	ctx.r31.s64 = ctx.r10.s64 + 32256;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,-536(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// addi r7,r31,64
	ctx.r7.s64 = ctx.r31.s64 + 64;
	// addi r6,r31,68
	ctx.r6.s64 = ctx.r31.s64 + 68;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lbz r9,7580(r9)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + 7580);
	// lwz r10,-540(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -540);
	// stfsx f31,r11,r7
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, temp.u32);
	// stfsx f30,r11,r6
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, temp.u32);
	// addi r27,r31,12
	ctx.r27.s64 = ctx.r31.s64 + 12;
	// addi r26,r31,58
	ctx.r26.s64 = ctx.r31.s64 + 58;
	// addi r8,r31,72
	ctx.r8.s64 = ctx.r31.s64 + 72;
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// add r3,r11,r8
	ctx.r3.u64 = ctx.r11.u64 + ctx.r8.u64;
	// stwx r10,r11,r27
	PPC_STORE_U32(ctx.r11.u32 + ctx.r27.u32, ctx.r10.u32);
	// stbx r9,r11,r26
	PPC_STORE_U8(ctx.r11.u32 + ctx.r26.u32, ctx.r9.u8);
	// bl 0x82a00ba0
	ctx.lr = 0x825D7FB4;
	rexcrt_strncpy(ctx, base);
	// lwz r3,-536(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// bl 0x821fe0b8
	ctx.lr = 0x825D7FBC;
	sub_821FE0B8(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r3,r11,-10248
	ctx.r3.s64 = ctx.r11.s64 + -10248;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// bl 0x821fe1c0
	ctx.lr = 0x825D7FD0;
	sub_821FE1C0(ctx, base);
	// lwz r11,-536(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// lis r9,-32070
	ctx.r9.s64 = -2101739520;
	// mulli r10,r11,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lwz r9,7576(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + 7576);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// addi r7,r31,116
	ctx.r7.s64 = ctx.r31.s64 + 116;
	// lis r8,-32070
	ctx.r8.s64 = -2101739520;
	// stw r11,-536(r30)
	PPC_STORE_U32(ctx.r30.u32 + -536, ctx.r11.u32);
	// li r11,1
	ctx.r11.s64 = 1;
	// stwx r3,r10,r7
	PPC_STORE_U32(ctx.r10.u32 + ctx.r7.u32, ctx.r3.u32);
	// stwx r9,r10,r31
	PPC_STORE_U32(ctx.r10.u32 + ctx.r31.u32, ctx.r9.u32);
	// stb r11,-16913(r8)
	PPC_STORE_U8(ctx.r8.u32 + -16913, ctx.r11.u8);
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// lfd f30,-72(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -72);
	// lfd f31,-64(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -64);
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D8010);
PPC_WEAK_FUNC(sub_825D8010) { __imp__sub_825D8010(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D8010) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7bc
	ctx.lr = 0x825D8018;
	__savegprlr_25(ctx, base);
	// stfd f30,-80(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -80, ctx.f30.u64);
	// stfd f31,-72(r1)
	PPC_STORE_U64(ctx.r1.u32 + -72, ctx.f31.u64);
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// addi r3,r11,-1640
	ctx.r3.s64 = ctx.r11.s64 + -1640;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D8048;
	sub_8221FD88(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lis r9,-32070
	ctx.r9.s64 = -2101739520;
	// addi r31,r10,32256
	ctx.r31.s64 = ctx.r10.s64 + 32256;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,-536(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// addi r7,r31,64
	ctx.r7.s64 = ctx.r31.s64 + 64;
	// addi r6,r31,68
	ctx.r6.s64 = ctx.r31.s64 + 68;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lbz r9,7580(r9)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + 7580);
	// lwz r10,-540(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -540);
	// stfsx f31,r11,r7
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, temp.u32);
	// stfsx f30,r11,r6
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, temp.u32);
	// addi r26,r31,12
	ctx.r26.s64 = ctx.r31.s64 + 12;
	// addi r25,r31,58
	ctx.r25.s64 = ctx.r31.s64 + 58;
	// addi r8,r31,72
	ctx.r8.s64 = ctx.r31.s64 + 72;
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// add r3,r11,r8
	ctx.r3.u64 = ctx.r11.u64 + ctx.r8.u64;
	// stwx r10,r11,r26
	PPC_STORE_U32(ctx.r11.u32 + ctx.r26.u32, ctx.r10.u32);
	// stbx r9,r11,r25
	PPC_STORE_U8(ctx.r11.u32 + ctx.r25.u32, ctx.r9.u8);
	// bl 0x82a00ba0
	ctx.lr = 0x825D80A0;
	rexcrt_strncpy(ctx, base);
	// lwz r3,-536(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// bl 0x821fe0b8
	ctx.lr = 0x825D80A8;
	sub_821FE0B8(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r29,r11,-10248
	ctx.r29.s64 = ctx.r11.s64 + -10248;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x821fe1c0
	ctx.lr = 0x825D80C0;
	sub_821FE1C0(ctx, base);
	// mr r28,r3
	ctx.r28.u64 = ctx.r3.u64;
	// li r5,0
	ctx.r5.s64 = 0;
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x821fe1c0
	ctx.lr = 0x825D80D4;
	sub_821FE1C0(ctx, base);
	// lwz r10,-536(r30)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// lis r9,-32070
	ctx.r9.s64 = -2101739520;
	// mulli r11,r10,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// lwz r9,7576(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + 7576);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r7,r31,116
	ctx.r7.s64 = ctx.r31.s64 + 116;
	// addi r6,r31,120
	ctx.r6.s64 = ctx.r31.s64 + 120;
	// lis r8,-32070
	ctx.r8.s64 = -2101739520;
	// stw r10,-536(r30)
	PPC_STORE_U32(ctx.r30.u32 + -536, ctx.r10.u32);
	// li r10,1
	ctx.r10.s64 = 1;
	// stwx r28,r11,r7
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, ctx.r28.u32);
	// stwx r3,r11,r6
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, ctx.r3.u32);
	// stwx r9,r11,r31
	PPC_STORE_U32(ctx.r11.u32 + ctx.r31.u32, ctx.r9.u32);
	// stb r10,-16913(r8)
	PPC_STORE_U8(ctx.r8.u32 + -16913, ctx.r10.u8);
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// lfd f30,-80(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -80);
	// lfd f31,-72(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -72);
	// b 0x829ff80c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D8120);
PPC_WEAK_FUNC(sub_825D8120) { __imp__sub_825D8120(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D8120) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c0
	ctx.lr = 0x825D8128;
	__savegprlr_26(ctx, base);
	// stfd f30,-72(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -72, ctx.f30.u64);
	// stfd f31,-64(r1)
	PPC_STORE_U64(ctx.r1.u32 + -64, ctx.f31.u64);
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// addi r31,r11,-1640
	ctx.r31.s64 = ctx.r11.s64 + -1640;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r30,r6
	ctx.r30.u64 = ctx.r6.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D8158;
	sub_8221FD88(ctx, base);
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D8164;
	sub_8221FD88(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lis r9,-32070
	ctx.r9.s64 = -2101739520;
	// addi r31,r10,32256
	ctx.r31.s64 = ctx.r10.s64 + 32256;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,-536(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// addi r7,r31,64
	ctx.r7.s64 = ctx.r31.s64 + 64;
	// addi r6,r31,68
	ctx.r6.s64 = ctx.r31.s64 + 68;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lbz r9,7580(r9)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + 7580);
	// lwz r10,-540(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -540);
	// stfsx f31,r11,r7
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, temp.u32);
	// stfsx f30,r11,r6
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, temp.u32);
	// addi r27,r31,12
	ctx.r27.s64 = ctx.r31.s64 + 12;
	// addi r26,r31,58
	ctx.r26.s64 = ctx.r31.s64 + 58;
	// addi r8,r31,72
	ctx.r8.s64 = ctx.r31.s64 + 72;
	// mr r28,r3
	ctx.r28.u64 = ctx.r3.u64;
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// add r3,r11,r8
	ctx.r3.u64 = ctx.r11.u64 + ctx.r8.u64;
	// stwx r10,r11,r27
	PPC_STORE_U32(ctx.r11.u32 + ctx.r27.u32, ctx.r10.u32);
	// stbx r9,r11,r26
	PPC_STORE_U8(ctx.r11.u32 + ctx.r26.u32, ctx.r9.u8);
	// bl 0x82a00ba0
	ctx.lr = 0x825D81C0;
	rexcrt_strncpy(ctx, base);
	// lwz r3,-536(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// bl 0x821fe0b8
	ctx.lr = 0x825D81C8;
	sub_821FE0B8(ctx, base);
	// lwz r11,-536(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// lis r9,-32070
	ctx.r9.s64 = -2101739520;
	// mulli r10,r11,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lwz r9,7576(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + 7576);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// addi r7,r31,88
	ctx.r7.s64 = ctx.r31.s64 + 88;
	// lis r8,-32070
	ctx.r8.s64 = -2101739520;
	// stw r11,-536(r30)
	PPC_STORE_U32(ctx.r30.u32 + -536, ctx.r11.u32);
	// li r11,1
	ctx.r11.s64 = 1;
	// stwx r28,r10,r7
	PPC_STORE_U32(ctx.r10.u32 + ctx.r7.u32, ctx.r28.u32);
	// stwx r9,r10,r31
	PPC_STORE_U32(ctx.r10.u32 + ctx.r31.u32, ctx.r9.u32);
	// stb r11,-16913(r8)
	PPC_STORE_U8(ctx.r8.u32 + -16913, ctx.r11.u8);
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// lfd f30,-72(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -72);
	// lfd f31,-64(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -64);
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D8208);
PPC_WEAK_FUNC(sub_825D8208) { __imp__sub_825D8208(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D8208) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7bc
	ctx.lr = 0x825D8210;
	__savegprlr_25(ctx, base);
	// stfd f30,-80(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -80, ctx.f30.u64);
	// stfd f31,-72(r1)
	PPC_STORE_U64(ctx.r1.u32 + -72, ctx.f31.u64);
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// addi r31,r11,-1640
	ctx.r31.s64 = ctx.r11.s64 + -1640;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r30,r6
	ctx.r30.u64 = ctx.r6.u64;
	// mr r28,r7
	ctx.r28.u64 = ctx.r7.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D8244;
	sub_8221FD88(ctx, base);
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D8250;
	sub_8221FD88(ctx, base);
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D8260;
	sub_8221FD88(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lis r9,-32070
	ctx.r9.s64 = -2101739520;
	// addi r31,r10,32256
	ctx.r31.s64 = ctx.r10.s64 + 32256;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,-536(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// addi r7,r31,64
	ctx.r7.s64 = ctx.r31.s64 + 64;
	// addi r6,r31,68
	ctx.r6.s64 = ctx.r31.s64 + 68;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lbz r9,7580(r9)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + 7580);
	// lwz r10,-540(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -540);
	// stfsx f31,r11,r7
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, temp.u32);
	// stfsx f30,r11,r6
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, temp.u32);
	// addi r26,r31,12
	ctx.r26.s64 = ctx.r31.s64 + 12;
	// addi r25,r31,58
	ctx.r25.s64 = ctx.r31.s64 + 58;
	// addi r8,r31,72
	ctx.r8.s64 = ctx.r31.s64 + 72;
	// mr r28,r3
	ctx.r28.u64 = ctx.r3.u64;
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// add r3,r11,r8
	ctx.r3.u64 = ctx.r11.u64 + ctx.r8.u64;
	// stwx r10,r11,r26
	PPC_STORE_U32(ctx.r11.u32 + ctx.r26.u32, ctx.r10.u32);
	// stbx r9,r11,r25
	PPC_STORE_U8(ctx.r11.u32 + ctx.r25.u32, ctx.r9.u8);
	// bl 0x82a00ba0
	ctx.lr = 0x825D82BC;
	rexcrt_strncpy(ctx, base);
	// lwz r3,-536(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// bl 0x821fe0b8
	ctx.lr = 0x825D82C4;
	sub_821FE0B8(ctx, base);
	// lwz r10,-536(r30)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r30.u32 + -536);
	// lis r9,-32070
	ctx.r9.s64 = -2101739520;
	// mulli r11,r10,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// lwz r9,7576(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + 7576);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r7,r31,88
	ctx.r7.s64 = ctx.r31.s64 + 88;
	// addi r6,r31,92
	ctx.r6.s64 = ctx.r31.s64 + 92;
	// lis r8,-32070
	ctx.r8.s64 = -2101739520;
	// stw r10,-536(r30)
	PPC_STORE_U32(ctx.r30.u32 + -536, ctx.r10.u32);
	// li r10,1
	ctx.r10.s64 = 1;
	// stwx r27,r11,r7
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, ctx.r27.u32);
	// stwx r28,r11,r6
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, ctx.r28.u32);
	// stwx r9,r11,r31
	PPC_STORE_U32(ctx.r11.u32 + ctx.r31.u32, ctx.r9.u32);
	// stb r10,-16913(r8)
	PPC_STORE_U8(ctx.r8.u32 + -16913, ctx.r10.u8);
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// lfd f30,-80(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -80);
	// lfd f31,-72(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -72);
	// b 0x829ff80c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D8310);
PPC_WEAK_FUNC(sub_825D8310) { __imp__sub_825D8310(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D8310) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c0
	ctx.lr = 0x825D8318;
	__savegprlr_26(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// lwz r11,-536(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -536);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d8450
	if (ctx.cr6.eq) goto loc_825D8450;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// addi r10,r10,32256
	ctx.r10.s64 = ctx.r10.s64 + 32256;
	// addi r9,r10,4
	ctx.r9.s64 = ctx.r10.s64 + 4;
	// addi r8,r10,8
	ctx.r8.s64 = ctx.r10.s64 + 8;
	// add r9,r11,r9
	ctx.r9.u64 = ctx.r11.u64 + ctx.r9.u64;
	// addi r7,r10,20
	ctx.r7.s64 = ctx.r10.s64 + 20;
	// add r8,r11,r8
	ctx.r8.u64 = ctx.r11.u64 + ctx.r8.u64;
	// add r7,r11,r7
	ctx.r7.u64 = ctx.r11.u64 + ctx.r7.u64;
	// addi r6,r10,24
	ctx.r6.s64 = ctx.r10.s64 + 24;
	// lfs f0,-124(r9)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + -124);
	ctx.f0.f64 = double(temp.f32);
	// addi r5,r10,25
	ctx.r5.s64 = ctx.r10.s64 + 25;
	// stfs f0,0(r9)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// add r6,r11,r6
	ctx.r6.u64 = ctx.r11.u64 + ctx.r6.u64;
	// lfs f0,-124(r8)
	temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + -124);
	ctx.f0.f64 = double(temp.f32);
	// add r5,r11,r5
	ctx.r5.u64 = ctx.r11.u64 + ctx.r5.u64;
	// stfs f0,0(r8)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// lwz r9,-124(r7)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r7.u32 + -124);
	// addi r4,r10,26
	ctx.r4.s64 = ctx.r10.s64 + 26;
	// addi r3,r10,27
	ctx.r3.s64 = ctx.r10.s64 + 27;
	// add r4,r11,r4
	ctx.r4.u64 = ctx.r11.u64 + ctx.r4.u64;
	// add r3,r11,r3
	ctx.r3.u64 = ctx.r11.u64 + ctx.r3.u64;
	// stw r9,0(r7)
	PPC_STORE_U32(ctx.r7.u32 + 0, ctx.r9.u32);
	// addi r31,r10,28
	ctx.r31.s64 = ctx.r10.s64 + 28;
	// lbz r9,-124(r6)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r6.u32 + -124);
	// addi r30,r10,29
	ctx.r30.s64 = ctx.r10.s64 + 29;
	// add r31,r11,r31
	ctx.r31.u64 = ctx.r11.u64 + ctx.r31.u64;
	// add r30,r11,r30
	ctx.r30.u64 = ctx.r11.u64 + ctx.r30.u64;
	// addi r29,r10,30
	ctx.r29.s64 = ctx.r10.s64 + 30;
	// addi r28,r10,32
	ctx.r28.s64 = ctx.r10.s64 + 32;
	// stb r9,0(r6)
	PPC_STORE_U8(ctx.r6.u32 + 0, ctx.r9.u8);
	// add r29,r11,r29
	ctx.r29.u64 = ctx.r11.u64 + ctx.r29.u64;
	// lbz r9,-124(r5)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r5.u32 + -124);
	// addi r27,r10,36
	ctx.r27.s64 = ctx.r10.s64 + 36;
	// add r28,r11,r28
	ctx.r28.u64 = ctx.r11.u64 + ctx.r28.u64;
	// addi r26,r10,48
	ctx.r26.s64 = ctx.r10.s64 + 48;
	// add r27,r11,r27
	ctx.r27.u64 = ctx.r11.u64 + ctx.r27.u64;
	// add r26,r11,r26
	ctx.r26.u64 = ctx.r11.u64 + ctx.r26.u64;
	// stb r9,0(r5)
	PPC_STORE_U8(ctx.r5.u32 + 0, ctx.r9.u8);
	// addi r8,r10,56
	ctx.r8.s64 = ctx.r10.s64 + 56;
	// lbz r9,-124(r4)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r4.u32 + -124);
	// addi r7,r10,57
	ctx.r7.s64 = ctx.r10.s64 + 57;
	// addi r6,r10,60
	ctx.r6.s64 = ctx.r10.s64 + 60;
	// add r8,r11,r8
	ctx.r8.u64 = ctx.r11.u64 + ctx.r8.u64;
	// add r7,r11,r7
	ctx.r7.u64 = ctx.r11.u64 + ctx.r7.u64;
	// add r6,r11,r6
	ctx.r6.u64 = ctx.r11.u64 + ctx.r6.u64;
	// stb r9,0(r4)
	PPC_STORE_U8(ctx.r4.u32 + 0, ctx.r9.u8);
	// lbz r9,-124(r3)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r3.u32 + -124);
	// stb r9,0(r3)
	PPC_STORE_U8(ctx.r3.u32 + 0, ctx.r9.u8);
	// lbz r9,-124(r31)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r31.u32 + -124);
	// stb r9,0(r31)
	PPC_STORE_U8(ctx.r31.u32 + 0, ctx.r9.u8);
	// lbz r9,-124(r30)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r30.u32 + -124);
	// stb r9,0(r30)
	PPC_STORE_U8(ctx.r30.u32 + 0, ctx.r9.u8);
	// lbz r9,-124(r29)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r29.u32 + -124);
	// stb r9,0(r29)
	PPC_STORE_U8(ctx.r29.u32 + 0, ctx.r9.u8);
	// lfs f0,-124(r28)
	temp.u32 = PPC_LOAD_U32(ctx.r28.u32 + -124);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r28)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r28.u32 + 0, temp.u32);
	// lfs f0,-124(r27)
	temp.u32 = PPC_LOAD_U32(ctx.r27.u32 + -124);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r27)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r27.u32 + 0, temp.u32);
	// lwz r9,-124(r26)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r26.u32 + -124);
	// stw r9,0(r26)
	PPC_STORE_U32(ctx.r26.u32 + 0, ctx.r9.u32);
	// addi r9,r10,52
	ctx.r9.s64 = ctx.r10.s64 + 52;
	// add r9,r11,r9
	ctx.r9.u64 = ctx.r11.u64 + ctx.r9.u64;
	// add r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
	// lwz r10,-124(r9)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r9.u32 + -124);
	// stw r10,0(r9)
	PPC_STORE_U32(ctx.r9.u32 + 0, ctx.r10.u32);
	// lbz r10,-124(r8)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r8.u32 + -124);
	// stb r10,0(r8)
	PPC_STORE_U8(ctx.r8.u32 + 0, ctx.r10.u8);
	// lbz r10,-124(r7)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r7.u32 + -124);
	// stb r10,0(r7)
	PPC_STORE_U8(ctx.r7.u32 + 0, ctx.r10.u8);
	// lwz r10,-124(r6)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r6.u32 + -124);
	// stw r10,0(r6)
	PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r10.u32);
	// lwz r10,-124(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + -124);
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
loc_825D8450:
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D8458);
PPC_WEAK_FUNC(sub_825D8458) { __imp__sub_825D8458(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D8458) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// lbz r11,0(r4)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r4.u32 + 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d84fc
	if (ctx.cr6.eq) goto loc_825D84FC;
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r30,r11,-10248
	ctx.r30.s64 = ctx.r11.s64 + -10248;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x821fe1c0
	ctx.lr = 0x825D8490;
	sub_821FE1C0(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// cmpwi cr6,r31,-1
	ctx.cr6.compare<int32_t>(ctx.r31.s32, -1, ctx.xer);
	// beq cr6,0x825d84fc
	if (ctx.cr6.eq) goto loc_825D84FC;
	// li r5,0
	ctx.r5.s64 = 0;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x821fe158
	ctx.lr = 0x825D84AC;
	sub_821FE158(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r4,r11,-31956
	ctx.r4.s64 = ctx.r11.s64 + -31956;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// addi r3,r11,-1640
	ctx.r3.s64 = ctx.r11.s64 + -1640;
	// bl 0x8221fd88
	ctx.lr = 0x825D84C0;
	sub_8221FD88(ctx, base);
	// li r8,0
	ctx.r8.s64 = 0;
	// li r11,-1
	ctx.r11.s64 = -1;
	// stw r31,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r31.u32);
	// li r10,-1
	ctx.r10.s64 = -1;
	// li r9,-1
	ctx.r9.s64 = -1;
	// li r7,-1
	ctx.r7.s64 = -1;
	// stb r8,119(r1)
	PPC_STORE_U8(ctx.r1.u32 + 119, ctx.r8.u8);
	// li r6,-1
	ctx.r6.s64 = -1;
	// li r8,-1
	ctx.r8.s64 = -1;
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// li r5,0
	ctx.r5.s64 = 0;
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// li r4,-1
	ctx.r4.s64 = -1;
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x82222de8
	ctx.lr = 0x825D84FC;
	sub_82222DE8(ctx, base);
loc_825D84FC:
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D8518);
PPC_WEAK_FUNC(sub_825D8518) { __imp__sub_825D8518(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D8518) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// lbz r11,0(r4)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r4.u32 + 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d85bc
	if (ctx.cr6.eq) goto loc_825D85BC;
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r30,r11,-10248
	ctx.r30.s64 = ctx.r11.s64 + -10248;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x821fe1c0
	ctx.lr = 0x825D8550;
	sub_821FE1C0(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// cmpwi cr6,r31,-1
	ctx.cr6.compare<int32_t>(ctx.r31.s32, -1, ctx.xer);
	// beq cr6,0x825d85bc
	if (ctx.cr6.eq) goto loc_825D85BC;
	// li r5,0
	ctx.r5.s64 = 0;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x821fe158
	ctx.lr = 0x825D856C;
	sub_821FE158(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r4,r11,-31956
	ctx.r4.s64 = ctx.r11.s64 + -31956;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// addi r3,r11,-1640
	ctx.r3.s64 = ctx.r11.s64 + -1640;
	// bl 0x8221fd88
	ctx.lr = 0x825D8580;
	sub_8221FD88(ctx, base);
	// li r8,1
	ctx.r8.s64 = 1;
	// li r11,-1
	ctx.r11.s64 = -1;
	// stw r31,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r31.u32);
	// li r10,-1
	ctx.r10.s64 = -1;
	// li r9,-1
	ctx.r9.s64 = -1;
	// li r7,-1
	ctx.r7.s64 = -1;
	// stb r8,119(r1)
	PPC_STORE_U8(ctx.r1.u32 + 119, ctx.r8.u8);
	// li r6,-1
	ctx.r6.s64 = -1;
	// li r8,-1
	ctx.r8.s64 = -1;
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// li r5,0
	ctx.r5.s64 = 0;
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// li r4,-1
	ctx.r4.s64 = -1;
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x82222de8
	ctx.lr = 0x825D85BC;
	sub_82222DE8(ctx, base);
loc_825D85BC:
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D85D8);
PPC_WEAK_FUNC(sub_825D85D8) { __imp__sub_825D85D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D85D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c0
	ctx.lr = 0x825D85E0;
	__savegprlr_26(ctx, base);
	// stwu r1,-192(r1)
	ea = -192 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r26,r11,-1640
	ctx.r26.s64 = ctx.r11.s64 + -1640;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// mr r30,r5
	ctx.r30.u64 = ctx.r5.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D8600;
	sub_8221FD88(ctx, base);
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// bl 0x821fe708
	ctx.lr = 0x825D8608;
	sub_821FE708(ctx, base);
	// lbz r11,133(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 133);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825d8634
	if (!ctx.cr6.eq) goto loc_825D8634;
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// lwz r11,7488(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 7488);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x825d8634
	if (ctx.cr6.eq) goto loc_825D8634;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r10,7484(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 7484);
	// cmpw cr6,r11,r10
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r10.s32, ctx.xer);
	// bne cr6,0x825d86d8
	if (!ctx.cr6.eq) goto loc_825D86D8;
loc_825D8634:
	// stb r30,87(r1)
	PPC_STORE_U8(ctx.r1.u32 + 87, ctx.r30.u8);
	// li r30,0
	ctx.r30.s64 = 0;
	// li r28,1
	ctx.r28.s64 = 1;
	// li r31,-1
	ctx.r31.s64 = -1;
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r3,r11,19272
	ctx.r3.s64 = ctx.r11.s64 + 19272;
	// stb r30,119(r1)
	PPC_STORE_U8(ctx.r1.u32 + 119, ctx.r30.u8);
	// li r9,0
	ctx.r9.s64 = 0;
	// stb r28,127(r1)
	PPC_STORE_U8(ctx.r1.u32 + 127, ctx.r28.u8);
	// li r8,0
	ctx.r8.s64 = 0;
	// stw r31,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r31.u32);
	// li r7,0
	ctx.r7.s64 = 0;
	// stb r28,111(r1)
	PPC_STORE_U8(ctx.r1.u32 + 111, ctx.r28.u8);
	// mr r6,r29
	ctx.r6.u64 = ctx.r29.u64;
	// stb r30,103(r1)
	PPC_STORE_U8(ctx.r1.u32 + 103, ctx.r30.u8);
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r30,95(r1)
	PPC_STORE_U8(ctx.r1.u32 + 95, ctx.r30.u8);
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// bl 0x82222f50
	ctx.lr = 0x825D8684;
	sub_82222F50(ctx, base);
	// lis r29,-32070
	ctx.r29.s64 = -2101739520;
	// lbz r11,-16940(r29)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r29.u32 + -16940);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d86cc
	if (ctx.cr6.eq) goto loc_825D86CC;
	// li r10,-1
	ctx.r10.s64 = -1;
	// lwz r4,604(r26)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r26.u32 + 604);
	// li r9,-1
	ctx.r9.s64 = -1;
	// stb r30,119(r1)
	PPC_STORE_U8(ctx.r1.u32 + 119, ctx.r30.u8);
	// li r8,-1
	ctx.r8.s64 = -1;
	// stw r31,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r31.u32);
	// li r7,-1
	ctx.r7.s64 = -1;
	// stw r31,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r31.u32);
	// li r6,-1
	ctx.r6.s64 = -1;
	// stw r31,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r31.u32);
	// li r5,0
	ctx.r5.s64 = 0;
	// stw r31,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r31.u32);
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// bl 0x82222de8
	ctx.lr = 0x825D86CC;
	sub_82222DE8(ctx, base);
loc_825D86CC:
	// stb r28,-16940(r29)
	PPC_STORE_U8(ctx.r29.u32 + -16940, ctx.r28.u8);
	// addi r1,r1,192
	ctx.r1.s64 = ctx.r1.s64 + 192;
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
loc_825D86D8:
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16940(r10)
	PPC_STORE_U8(ctx.r10.u32 + -16940, ctx.r11.u8);
	// addi r1,r1,192
	ctx.r1.s64 = ctx.r1.s64 + 192;
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D86F0);
PPC_WEAK_FUNC(sub_825D86F0) { __imp__sub_825D86F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D86F0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7bc
	ctx.lr = 0x825D86F8;
	__savegprlr_25(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r28,r4
	ctx.r28.u64 = ctx.r4.u64;
	// addi r25,r11,-1640
	ctx.r25.s64 = ctx.r11.s64 + -1640;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D8718;
	sub_8221FD88(ctx, base);
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// bl 0x821fe708
	ctx.lr = 0x825D8720;
	sub_821FE708(ctx, base);
	// lbz r11,133(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 133);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825d874c
	if (!ctx.cr6.eq) goto loc_825D874C;
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// lwz r11,7488(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 7488);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x825d874c
	if (ctx.cr6.eq) goto loc_825D874C;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r10,7484(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 7484);
	// cmpw cr6,r11,r10
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r10.s32, ctx.xer);
	// bne cr6,0x825d87f0
	if (!ctx.cr6.eq) goto loc_825D87F0;
loc_825D874C:
	// li r31,0
	ctx.r31.s64 = 0;
	// li r26,1
	ctx.r26.s64 = 1;
	// li r30,-1
	ctx.r30.s64 = -1;
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r3,r11,19272
	ctx.r3.s64 = ctx.r11.s64 + 19272;
	// stb r31,119(r1)
	PPC_STORE_U8(ctx.r1.u32 + 119, ctx.r31.u8);
	// li r9,0
	ctx.r9.s64 = 0;
	// stb r26,127(r1)
	PPC_STORE_U8(ctx.r1.u32 + 127, ctx.r26.u8);
	// mr r8,r28
	ctx.r8.u64 = ctx.r28.u64;
	// stw r30,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r30.u32);
	// li r7,1
	ctx.r7.s64 = 1;
	// stb r26,111(r1)
	PPC_STORE_U8(ctx.r1.u32 + 111, ctx.r26.u8);
	// mr r6,r29
	ctx.r6.u64 = ctx.r29.u64;
	// stb r31,103(r1)
	PPC_STORE_U8(ctx.r1.u32 + 103, ctx.r31.u8);
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r31,95(r1)
	PPC_STORE_U8(ctx.r1.u32 + 95, ctx.r31.u8);
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// stb r31,87(r1)
	PPC_STORE_U8(ctx.r1.u32 + 87, ctx.r31.u8);
	// bl 0x82222f50
	ctx.lr = 0x825D879C;
	sub_82222F50(ctx, base);
	// lis r29,-32070
	ctx.r29.s64 = -2101739520;
	// lbz r11,-16940(r29)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r29.u32 + -16940);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d87e4
	if (ctx.cr6.eq) goto loc_825D87E4;
	// li r10,-1
	ctx.r10.s64 = -1;
	// lwz r4,604(r25)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r25.u32 + 604);
	// li r9,-1
	ctx.r9.s64 = -1;
	// stb r31,119(r1)
	PPC_STORE_U8(ctx.r1.u32 + 119, ctx.r31.u8);
	// li r8,-1
	ctx.r8.s64 = -1;
	// stw r30,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r30.u32);
	// mr r7,r28
	ctx.r7.u64 = ctx.r28.u64;
	// stw r30,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r30.u32);
	// li r6,-1
	ctx.r6.s64 = -1;
	// stw r30,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r30.u32);
	// li r5,0
	ctx.r5.s64 = 0;
	// stw r30,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r30.u32);
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// bl 0x82222de8
	ctx.lr = 0x825D87E4;
	sub_82222DE8(ctx, base);
loc_825D87E4:
	// stb r26,-16940(r29)
	PPC_STORE_U8(ctx.r29.u32 + -16940, ctx.r26.u8);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x829ff80c
	__restgprlr_25(ctx, base);
	return;
loc_825D87F0:
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16940(r10)
	PPC_STORE_U8(ctx.r10.u32 + -16940, ctx.r11.u8);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x829ff80c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D8808);
PPC_WEAK_FUNC(sub_825D8808) { __imp__sub_825D8808(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D8808) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7bc
	ctx.lr = 0x825D8810;
	__savegprlr_25(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r25,r11,-1640
	ctx.r25.s64 = ctx.r11.s64 + -1640;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r30,r6
	ctx.r30.u64 = ctx.r6.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D8834;
	sub_8221FD88(ctx, base);
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// bl 0x821fe708
	ctx.lr = 0x825D883C;
	sub_821FE708(ctx, base);
	// lbz r11,133(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 133);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825d8868
	if (!ctx.cr6.eq) goto loc_825D8868;
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// lwz r11,7488(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 7488);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x825d8868
	if (ctx.cr6.eq) goto loc_825D8868;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r10,7484(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 7484);
	// cmpw cr6,r11,r10
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r10.s32, ctx.xer);
	// bne cr6,0x825d891c
	if (!ctx.cr6.eq) goto loc_825D891C;
loc_825D8868:
	// li r31,0
	ctx.r31.s64 = 0;
	// stw r28,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r28.u32);
	// li r26,1
	ctx.r26.s64 = 1;
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r3,r11,19272
	ctx.r3.s64 = ctx.r11.s64 + 19272;
	// li r9,0
	ctx.r9.s64 = 0;
	// stb r31,119(r1)
	PPC_STORE_U8(ctx.r1.u32 + 119, ctx.r31.u8);
	// mr r8,r29
	ctx.r8.u64 = ctx.r29.u64;
	// stb r26,127(r1)
	PPC_STORE_U8(ctx.r1.u32 + 127, ctx.r26.u8);
	// li r7,1
	ctx.r7.s64 = 1;
	// stb r26,111(r1)
	PPC_STORE_U8(ctx.r1.u32 + 111, ctx.r26.u8);
	// mr r6,r30
	ctx.r6.u64 = ctx.r30.u64;
	// stb r31,103(r1)
	PPC_STORE_U8(ctx.r1.u32 + 103, ctx.r31.u8);
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r31,95(r1)
	PPC_STORE_U8(ctx.r1.u32 + 95, ctx.r31.u8);
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// stb r31,87(r1)
	PPC_STORE_U8(ctx.r1.u32 + 87, ctx.r31.u8);
	// bl 0x82222f50
	ctx.lr = 0x825D88B4;
	sub_82222F50(ctx, base);
	// lis r30,-32070
	ctx.r30.s64 = -2101739520;
	// lbz r11,-16940(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -16940);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d8910
	if (ctx.cr6.eq) goto loc_825D8910;
	// li r11,-1
	ctx.r11.s64 = -1;
	// lwz r4,604(r25)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r25.u32 + 604);
	// li r10,-1
	ctx.r10.s64 = -1;
	// stb r31,119(r1)
	PPC_STORE_U8(ctx.r1.u32 + 119, ctx.r31.u8);
	// li r9,-1
	ctx.r9.s64 = -1;
	// mr r8,r28
	ctx.r8.u64 = ctx.r28.u64;
	// mr r7,r29
	ctx.r7.u64 = ctx.r29.u64;
	// li r6,-1
	ctx.r6.s64 = -1;
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// li r5,0
	ctx.r5.s64 = 0;
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x82222de8
	ctx.lr = 0x825D8900;
	sub_82222DE8(ctx, base);
	// mr r11,r26
	ctx.r11.u64 = ctx.r26.u64;
	// stb r11,-16940(r30)
	PPC_STORE_U8(ctx.r30.u32 + -16940, ctx.r11.u8);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x829ff80c
	__restgprlr_25(ctx, base);
	return;
loc_825D8910:
	// stb r26,-16940(r30)
	PPC_STORE_U8(ctx.r30.u32 + -16940, ctx.r26.u8);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x829ff80c
	__restgprlr_25(ctx, base);
	return;
loc_825D891C:
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16940(r10)
	PPC_STORE_U8(ctx.r10.u32 + -16940, ctx.r11.u8);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x829ff80c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D8930);
PPC_WEAK_FUNC(sub_825D8930) { __imp__sub_825D8930(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D8930) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7b8
	ctx.lr = 0x825D8938;
	__savegprlr_24(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// addi r31,r11,-1640
	ctx.r31.s64 = ctx.r11.s64 + -1640;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r26,r6
	ctx.r26.u64 = ctx.r6.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D895C;
	sub_8221FD88(ctx, base);
	// mr r28,r3
	ctx.r28.u64 = ctx.r3.u64;
	// lwz r25,604(r31)
	ctx.r25.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D8970;
	sub_8221FD88(ctx, base);
	// lwz r24,604(r31)
	ctx.r24.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// bl 0x821fe708
	ctx.lr = 0x825D897C;
	sub_821FE708(ctx, base);
	// lbz r11,133(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 133);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825d89a8
	if (!ctx.cr6.eq) goto loc_825D89A8;
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// lwz r11,7488(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 7488);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x825d89a8
	if (ctx.cr6.eq) goto loc_825D89A8;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r10,7484(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 7484);
	// cmpw cr6,r11,r10
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r10.s32, ctx.xer);
	// bne cr6,0x825d8a50
	if (!ctx.cr6.eq) goto loc_825D8A50;
loc_825D89A8:
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// stb r26,111(r1)
	PPC_STORE_U8(ctx.r1.u32 + 111, ctx.r26.u8);
	// li r31,0
	ctx.r31.s64 = 0;
	// addi r3,r11,19272
	ctx.r3.s64 = ctx.r11.s64 + 19272;
	// li r11,1
	ctx.r11.s64 = 1;
	// li r30,-1
	ctx.r30.s64 = -1;
	// li r10,0
	ctx.r10.s64 = 0;
	// mr r9,r27
	ctx.r9.u64 = ctx.r27.u64;
	// stb r31,119(r1)
	PPC_STORE_U8(ctx.r1.u32 + 119, ctx.r31.u8);
	// li r8,0
	ctx.r8.s64 = 0;
	// stb r31,103(r1)
	PPC_STORE_U8(ctx.r1.u32 + 103, ctx.r31.u8);
	// li r7,0
	ctx.r7.s64 = 0;
	// stb r11,127(r1)
	PPC_STORE_U8(ctx.r1.u32 + 127, ctx.r11.u8);
	// mr r6,r29
	ctx.r6.u64 = ctx.r29.u64;
	// stw r30,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r30.u32);
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r31,95(r1)
	PPC_STORE_U8(ctx.r1.u32 + 95, ctx.r31.u8);
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// stb r31,87(r1)
	PPC_STORE_U8(ctx.r1.u32 + 87, ctx.r31.u8);
	// bl 0x82222f50
	ctx.lr = 0x825D89F8;
	sub_82222F50(ctx, base);
	// lis r29,-32070
	ctx.r29.s64 = -2101739520;
	// lbz r11,-16940(r29)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r29.u32 + -16940);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d8a40
	if (ctx.cr6.eq) goto loc_825D8A40;
	// li r10,-1
	ctx.r10.s64 = -1;
	// stb r31,119(r1)
	PPC_STORE_U8(ctx.r1.u32 + 119, ctx.r31.u8);
	// li r9,-1
	ctx.r9.s64 = -1;
	// stw r30,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r30.u32);
	// li r8,-1
	ctx.r8.s64 = -1;
	// stw r30,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r30.u32);
	// li r7,-1
	ctx.r7.s64 = -1;
	// stw r30,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r30.u32);
	// mr r6,r24
	ctx.r6.u64 = ctx.r24.u64;
	// stw r30,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r30.u32);
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r25
	ctx.r4.u64 = ctx.r25.u64;
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// bl 0x82222de8
	ctx.lr = 0x825D8A40;
	sub_82222DE8(ctx, base);
loc_825D8A40:
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16940(r29)
	PPC_STORE_U8(ctx.r29.u32 + -16940, ctx.r11.u8);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x829ff808
	__restgprlr_24(ctx, base);
	return;
loc_825D8A50:
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16940(r10)
	PPC_STORE_U8(ctx.r10.u32 + -16940, ctx.r11.u8);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x829ff808
	__restgprlr_24(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D8A68);
PPC_WEAK_FUNC(sub_825D8A68) { __imp__sub_825D8A68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D8A68) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r10,-1
	ctx.r10.s64 = -1;
	// li r9,1
	ctx.r9.s64 = 1;
	// li r11,0
	ctx.r11.s64 = 0;
	// li r8,0
	ctx.r8.s64 = 0;
	// li r7,0
	ctx.r7.s64 = 0;
	// stw r10,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r10.u32);
	// lis r10,-32087
	ctx.r10.s64 = -2102853632;
	// stb r9,127(r1)
	PPC_STORE_U8(ctx.r1.u32 + 127, ctx.r9.u8);
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r3,r10,19272
	ctx.r3.s64 = ctx.r10.s64 + 19272;
	// stb r9,111(r1)
	PPC_STORE_U8(ctx.r1.u32 + 111, ctx.r9.u8);
	// li r10,0
	ctx.r10.s64 = 0;
	// stb r11,119(r1)
	PPC_STORE_U8(ctx.r1.u32 + 119, ctx.r11.u8);
	// li r9,0
	ctx.r9.s64 = 0;
	// stb r11,103(r1)
	PPC_STORE_U8(ctx.r1.u32 + 103, ctx.r11.u8);
	// li r5,1
	ctx.r5.s64 = 1;
	// stb r11,95(r1)
	PPC_STORE_U8(ctx.r1.u32 + 95, ctx.r11.u8);
	// li r4,0
	ctx.r4.s64 = 0;
	// stb r11,87(r1)
	PPC_STORE_U8(ctx.r1.u32 + 87, ctx.r11.u8);
	// bl 0x82222f50
	ctx.lr = 0x825D8AC4;
	sub_82222F50(ctx, base);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D8AD8);
PPC_WEAK_FUNC(sub_825D8AD8) { __imp__sub_825D8AD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D8AD8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// bl 0x821e3788
	ctx.lr = 0x825D8AF0;
	sub_821E3788(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d8b14
	if (ctx.cr6.eq) goto loc_825D8B14;
	// li r3,0
	ctx.r3.s64 = 0;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
loc_825D8B14:
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// addi r3,r11,-1640
	ctx.r3.s64 = ctx.r11.s64 + -1640;
	// bl 0x8221fd88
	ctx.lr = 0x825D8B24;
	sub_8221FD88(ctx, base);
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,19272
	ctx.r3.s64 = ctx.r11.s64 + 19272;
	// li r31,0
	ctx.r31.s64 = 0;
	// bl 0x82222460
	ctx.lr = 0x825D8B38;
	sub_82222460(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d8b48
	if (ctx.cr6.eq) goto loc_825D8B48;
	// li r31,1
	ctx.r31.s64 = 1;
loc_825D8B48:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D8B60);
PPC_WEAK_FUNC(sub_825D8B60) { __imp__sub_825D8B60(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D8B60) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-912(r1)
	ea = -912 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// lis r10,-32067
	ctx.r10.s64 = -2101542912;
	// mr r31,r4
	ctx.r31.u64 = ctx.r4.u64;
	// addi r3,r10,-1640
	ctx.r3.s64 = ctx.r10.s64 + -1640;
	// mr r4,r11
	ctx.r4.u64 = ctx.r11.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D8B88;
	sub_8221FD88(ctx, base);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r10,r1,96
	ctx.r10.s64 = ctx.r1.s64 + 96;
	// li r9,-1
	ctx.r9.s64 = -1;
	// li r8,-1
	ctx.r8.s64 = -1;
	// li r7,-1
	ctx.r7.s64 = -1;
	// lfs f1,2612(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2612);
	ctx.f1.f64 = double(temp.f32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// li r6,-1
	ctx.r6.s64 = -1;
	// stw r10,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r10.u32);
	// li r5,-1
	ctx.r5.s64 = -1;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x822219b0
	ctx.lr = 0x825D8BBC;
	sub_822219B0(ctx, base);
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// addi r4,r1,96
	ctx.r4.s64 = ctx.r1.s64 + 96;
	// addi r3,r11,19272
	ctx.r3.s64 = ctx.r11.s64 + 19272;
	// li r31,0
	ctx.r31.s64 = 0;
	// bl 0x82222460
	ctx.lr = 0x825D8BD0;
	sub_82222460(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// li r3,1
	ctx.r3.s64 = 1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825d8be4
	if (!ctx.cr6.eq) goto loc_825D8BE4;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
loc_825D8BE4:
	// addi r1,r1,912
	ctx.r1.s64 = ctx.r1.s64 + 912;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D8BF8);
PPC_WEAK_FUNC(sub_825D8BF8) { __imp__sub_825D8BF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D8BF8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7cc
	ctx.lr = 0x825D8C00;
	__savegprlr_29(ctx, base);
	// stwu r1,-912(r1)
	ea = -912 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// addi r31,r11,-1640
	ctx.r31.s64 = ctx.r11.s64 + -1640;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D8C1C;
	sub_8221FD88(ctx, base);
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8221fd88
	ctx.lr = 0x825D8C2C;
	sub_8221FD88(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// li r5,400
	ctx.r5.s64 = 400;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x822217b0
	ctx.lr = 0x825D8C40;
	sub_822217B0(ctx, base);
	// li r9,-1
	ctx.r9.s64 = -1;
	// li r8,-1
	ctx.r8.s64 = -1;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// li r5,0
	ctx.r5.s64 = 0;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x82222698
	ctx.lr = 0x825D8C60;
	sub_82222698(ctx, base);
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// addi r3,r11,19272
	ctx.r3.s64 = ctx.r11.s64 + 19272;
	// li r31,0
	ctx.r31.s64 = 0;
	// bl 0x82222460
	ctx.lr = 0x825D8C74;
	sub_82222460(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// li r3,1
	ctx.r3.s64 = 1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825d8c88
	if (!ctx.cr6.eq) goto loc_825D8C88;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
loc_825D8C88:
	// addi r1,r1,912
	ctx.r1.s64 = ctx.r1.s64 + 912;
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D8C90);
PPC_WEAK_FUNC(sub_825D8C90) { __imp__sub_825D8C90(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D8C90) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// bl 0x821fe708
	ctx.lr = 0x825D8CA8;
	sub_821FE708(ctx, base);
	// lbz r11,133(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 133);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d8d20
	if (ctx.cr6.eq) goto loc_825D8D20;
	// clrlwi r11,r31,24
	ctx.r11.u64 = ctx.r31.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d8cec
	if (ctx.cr6.eq) goto loc_825D8CEC;
	// bl 0x821fe708
	ctx.lr = 0x825D8CC4;
	sub_821FE708(ctx, base);
	// lbz r11,134(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 134);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825d8d20
	if (!ctx.cr6.eq) goto loc_825D8D20;
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// lwz r10,7484(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 7484);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// stw r10,7484(r11)
	PPC_STORE_U32(ctx.r11.u32 + 7484, ctx.r10.u32);
	// bl 0x821fe708
	ctx.lr = 0x825D8CE4;
	sub_821FE708(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// b 0x825d8d1c
	goto loc_825D8D1C;
loc_825D8CEC:
	// bl 0x821fe708
	ctx.lr = 0x825D8CF0;
	sub_821FE708(ctx, base);
	// lbz r11,134(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 134);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d8d20
	if (ctx.cr6.eq) goto loc_825D8D20;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,7484(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 7484);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// ble cr6,0x825d8d14
	if (!ctx.cr6.gt) goto loc_825D8D14;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// stw r11,7484(r10)
	PPC_STORE_U32(ctx.r10.u32 + 7484, ctx.r11.u32);
loc_825D8D14:
	// bl 0x821fe708
	ctx.lr = 0x825D8D18;
	sub_821FE708(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
loc_825D8D1C:
	// stb r11,134(r3)
	PPC_STORE_U8(ctx.r3.u32 + 134, ctx.r11.u8);
loc_825D8D20:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D8D38);
PPC_WEAK_FUNC(sub_825D8D38) { __imp__sub_825D8D38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D8D38) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c4
	ctx.lr = 0x825D8D40;
	__savegprlr_27(ctx, base);
	// stfd f29,-72(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -72, ctx.f29.u64);
	// stfd f30,-64(r1)
	PPC_STORE_U64(ctx.r1.u32 + -64, ctx.f30.u64);
	// stfd f31,-56(r1)
	PPC_STORE_U64(ctx.r1.u32 + -56, ctx.f31.u64);
	// stwu r1,-176(r1)
	ea = -176 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// addi r4,r11,-16180
	ctx.r4.s64 = ctx.r11.s64 + -16180;
	// fmr f29,f3
	ctx.f29.f64 = ctx.f3.f64;
	// li r11,0
	ctx.r11.s64 = 0;
	// mr r30,r7
	ctx.r30.u64 = ctx.r7.u64;
	// mr r29,r8
	ctx.r29.u64 = ctx.r8.u64;
	// mr r28,r9
	ctx.r28.u64 = ctx.r9.u64;
	// mr r27,r10
	ctx.r27.u64 = ctx.r10.u64;
	// stb r11,88(r1)
	PPC_STORE_U8(ctx.r1.u32 + 88, ctx.r11.u8);
	// bl 0x829ffcf0
	ctx.lr = 0x825D8D84;
	rexcrt__stricmp(ctx, base);
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// beq cr6,0x825d8dac
	if (ctx.cr6.eq) goto loc_825D8DAC;
	// addi r10,r1,88
	ctx.r10.s64 = ctx.r1.s64 + 88;
	// mr r11,r31
	ctx.r11.u64 = ctx.r31.u64;
	// subf r9,r31,r10
	ctx.r9.u64 = ctx.r10.u64 - ctx.r31.u64;
loc_825D8D98:
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r9,r11
	PPC_STORE_U8(ctx.r9.u32 + ctx.r11.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x825d8d98
	if (!ctx.cr6.eq) goto loc_825D8D98;
loc_825D8DAC:
	// mr r10,r27
	ctx.r10.u64 = ctx.r27.u64;
	// stfs f31,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// mr r9,r28
	ctx.r9.u64 = ctx.r28.u64;
	// stfs f30,84(r1)
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// mr r8,r29
	ctx.r8.u64 = ctx.r29.u64;
	// fmr f1,f29
	ctx.f1.f64 = ctx.f29.f64;
	// mr r7,r30
	ctx.r7.u64 = ctx.r30.u64;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r4,r1,88
	ctx.r4.s64 = ctx.r1.s64 + 88;
	// li r3,0
	ctx.r3.s64 = 0;
	// bl 0x8229ed30
	ctx.lr = 0x825D8DD8;
	sub_8229ED30(ctx, base);
	// lwz r11,260(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 260);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,176
	ctx.r1.s64 = ctx.r1.s64 + 176;
	// lfd f29,-72(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f29.u64 = PPC_LOAD_U64(ctx.r1.u32 + -72);
	// lfd f30,-64(r1)
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -64);
	// lfd f31,-56(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -56);
	// b 0x829ff814
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D8DF8);
PPC_WEAK_FUNC(sub_825D8DF8) { __imp__sub_825D8DF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D8DF8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7cc
	ctx.lr = 0x825D8E00;
	__savegprlr_29(ctx, base);
	// stwu r1,-864(r1)
	ea = -864 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// addi r11,r1,140
	ctx.r11.s64 = ctx.r1.s64 + 140;
	// li r5,12
	ctx.r5.s64 = 12;
	// li r30,0
	ctx.r30.s64 = 0;
	// li r31,-1
	ctx.r31.s64 = -1;
loc_825D8E14:
	// addi r5,r5,-1
	ctx.r5.s64 = ctx.r5.s64 + -1;
	// stb r30,-60(r11)
	PPC_STORE_U8(ctx.r11.u32 + -60, ctx.r30.u8);
	// stw r31,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r31.u32);
	// stw r31,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r31.u32);
	// cmplwi cr6,r5,0
	ctx.cr6.compare<uint32_t>(ctx.r5.u32, 0, ctx.xer);
	// addi r11,r11,68
	ctx.r11.s64 = ctx.r11.s64 + 68;
	// bne cr6,0x825d8e14
	if (!ctx.cr6.eq) goto loc_825D8E14;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// addi r29,r11,-16180
	ctx.r29.s64 = ctx.r11.s64 + -16180;
	// mr r11,r6
	ctx.r11.u64 = ctx.r6.u64;
	// mr r5,r29
	ctx.r5.u64 = ctx.r29.u64;
loc_825D8E40:
	// lbz r31,0(r11)
	ctx.r31.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// lbz r30,0(r5)
	ctx.r30.u64 = PPC_LOAD_U8(ctx.r5.u32 + 0);
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// subf r30,r30,r31
	ctx.r30.u64 = ctx.r31.u64 - ctx.r30.u64;
	// beq cr6,0x825d8e64
	if (ctx.cr6.eq) goto loc_825D8E64;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// addi r5,r5,1
	ctx.r5.s64 = ctx.r5.s64 + 1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// beq cr6,0x825d8e40
	if (ctx.cr6.eq) goto loc_825D8E40;
loc_825D8E64:
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// beq cr6,0x825d8e8c
	if (ctx.cr6.eq) goto loc_825D8E8C;
	// addi r5,r1,148
	ctx.r5.s64 = ctx.r1.s64 + 148;
	// mr r11,r6
	ctx.r11.u64 = ctx.r6.u64;
	// subf r5,r6,r5
	ctx.r5.u64 = ctx.r5.u64 - ctx.r6.u64;
loc_825D8E78:
	// lbz r6,0(r11)
	ctx.r6.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r6,0
	ctx.cr6.compare<uint32_t>(ctx.r6.u32, 0, ctx.xer);
	// stbx r6,r5,r11
	PPC_STORE_U8(ctx.r5.u32 + ctx.r11.u32, ctx.r6.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x825d8e78
	if (!ctx.cr6.eq) goto loc_825D8E78;
loc_825D8E8C:
	// mr r11,r7
	ctx.r11.u64 = ctx.r7.u64;
	// mr r6,r29
	ctx.r6.u64 = ctx.r29.u64;
loc_825D8E94:
	// lbz r5,0(r11)
	ctx.r5.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// lbz r31,0(r6)
	ctx.r31.u64 = PPC_LOAD_U8(ctx.r6.u32 + 0);
	// cmpwi cr6,r5,0
	ctx.cr6.compare<int32_t>(ctx.r5.s32, 0, ctx.xer);
	// subf r31,r31,r5
	ctx.r31.u64 = ctx.r5.u64 - ctx.r31.u64;
	// beq cr6,0x825d8eb8
	if (ctx.cr6.eq) goto loc_825D8EB8;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// addi r6,r6,1
	ctx.r6.s64 = ctx.r6.s64 + 1;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// beq cr6,0x825d8e94
	if (ctx.cr6.eq) goto loc_825D8E94;
loc_825D8EB8:
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// beq cr6,0x825d8ee0
	if (ctx.cr6.eq) goto loc_825D8EE0;
	// addi r6,r1,216
	ctx.r6.s64 = ctx.r1.s64 + 216;
	// mr r11,r7
	ctx.r11.u64 = ctx.r7.u64;
	// subf r6,r7,r6
	ctx.r6.u64 = ctx.r6.u64 - ctx.r7.u64;
loc_825D8ECC:
	// lbz r7,0(r11)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r7,0
	ctx.cr6.compare<uint32_t>(ctx.r7.u32, 0, ctx.xer);
	// stbx r7,r6,r11
	PPC_STORE_U8(ctx.r6.u32 + ctx.r11.u32, ctx.r7.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x825d8ecc
	if (!ctx.cr6.eq) goto loc_825D8ECC;
loc_825D8EE0:
	// mr r11,r8
	ctx.r11.u64 = ctx.r8.u64;
	// mr r7,r29
	ctx.r7.u64 = ctx.r29.u64;
loc_825D8EE8:
	// lbz r6,0(r11)
	ctx.r6.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// lbz r5,0(r7)
	ctx.r5.u64 = PPC_LOAD_U8(ctx.r7.u32 + 0);
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// subf r5,r5,r6
	ctx.r5.u64 = ctx.r6.u64 - ctx.r5.u64;
	// beq cr6,0x825d8f0c
	if (ctx.cr6.eq) goto loc_825D8F0C;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// addi r7,r7,1
	ctx.r7.s64 = ctx.r7.s64 + 1;
	// cmpwi cr6,r5,0
	ctx.cr6.compare<int32_t>(ctx.r5.s32, 0, ctx.xer);
	// beq cr6,0x825d8ee8
	if (ctx.cr6.eq) goto loc_825D8EE8;
loc_825D8F0C:
	// cmpwi cr6,r5,0
	ctx.cr6.compare<int32_t>(ctx.r5.s32, 0, ctx.xer);
	// beq cr6,0x825d8f34
	if (ctx.cr6.eq) goto loc_825D8F34;
	// addi r7,r1,284
	ctx.r7.s64 = ctx.r1.s64 + 284;
	// mr r11,r8
	ctx.r11.u64 = ctx.r8.u64;
	// subf r7,r8,r7
	ctx.r7.u64 = ctx.r7.u64 - ctx.r8.u64;
loc_825D8F20:
	// lbz r8,0(r11)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r8,0
	ctx.cr6.compare<uint32_t>(ctx.r8.u32, 0, ctx.xer);
	// stbx r8,r7,r11
	PPC_STORE_U8(ctx.r7.u32 + ctx.r11.u32, ctx.r8.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x825d8f20
	if (!ctx.cr6.eq) goto loc_825D8F20;
loc_825D8F34:
	// mr r11,r9
	ctx.r11.u64 = ctx.r9.u64;
	// mr r8,r29
	ctx.r8.u64 = ctx.r29.u64;
loc_825D8F3C:
	// lbz r7,0(r11)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// lbz r6,0(r8)
	ctx.r6.u64 = PPC_LOAD_U8(ctx.r8.u32 + 0);
	// cmpwi cr6,r7,0
	ctx.cr6.compare<int32_t>(ctx.r7.s32, 0, ctx.xer);
	// subf r6,r6,r7
	ctx.r6.u64 = ctx.r7.u64 - ctx.r6.u64;
	// beq cr6,0x825d8f60
	if (ctx.cr6.eq) goto loc_825D8F60;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// addi r8,r8,1
	ctx.r8.s64 = ctx.r8.s64 + 1;
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x825d8f3c
	if (ctx.cr6.eq) goto loc_825D8F3C;
loc_825D8F60:
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x825d8f88
	if (ctx.cr6.eq) goto loc_825D8F88;
	// addi r8,r1,352
	ctx.r8.s64 = ctx.r1.s64 + 352;
	// mr r11,r9
	ctx.r11.u64 = ctx.r9.u64;
	// subf r7,r9,r8
	ctx.r7.u64 = ctx.r8.u64 - ctx.r9.u64;
loc_825D8F74:
	// lbz r8,0(r11)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r8,0
	ctx.cr6.compare<uint32_t>(ctx.r8.u32, 0, ctx.xer);
	// stbx r8,r7,r11
	PPC_STORE_U8(ctx.r7.u32 + ctx.r11.u32, ctx.r8.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x825d8f74
	if (!ctx.cr6.eq) goto loc_825D8F74;
loc_825D8F88:
	// mr r11,r10
	ctx.r11.u64 = ctx.r10.u64;
	// mr r8,r29
	ctx.r8.u64 = ctx.r29.u64;
loc_825D8F90:
	// lbz r7,0(r11)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// lbz r9,0(r8)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r8.u32 + 0);
	// cmpwi cr6,r7,0
	ctx.cr6.compare<int32_t>(ctx.r7.s32, 0, ctx.xer);
	// subf r6,r9,r7
	ctx.r6.u64 = ctx.r7.u64 - ctx.r9.u64;
	// beq cr6,0x825d8fb4
	if (ctx.cr6.eq) goto loc_825D8FB4;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// addi r8,r8,1
	ctx.r8.s64 = ctx.r8.s64 + 1;
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x825d8f90
	if (ctx.cr6.eq) goto loc_825D8F90;
loc_825D8FB4:
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x825d8fdc
	if (ctx.cr6.eq) goto loc_825D8FDC;
	// addi r9,r1,420
	ctx.r9.s64 = ctx.r1.s64 + 420;
	// mr r11,r10
	ctx.r11.u64 = ctx.r10.u64;
	// subf r8,r10,r9
	ctx.r8.u64 = ctx.r9.u64 - ctx.r10.u64;
loc_825D8FC8:
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r8,r11
	PPC_STORE_U8(ctx.r8.u32 + ctx.r11.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x825d8fc8
	if (!ctx.cr6.eq) goto loc_825D8FC8;
loc_825D8FDC:
	// lwz r11,948(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 948);
	// mr r8,r29
	ctx.r8.u64 = ctx.r29.u64;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_825D8FE8:
	// lbz r7,0(r10)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
	// lbz r9,0(r8)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r8.u32 + 0);
	// cmpwi cr6,r7,0
	ctx.cr6.compare<int32_t>(ctx.r7.s32, 0, ctx.xer);
	// subf r6,r9,r7
	ctx.r6.u64 = ctx.r7.u64 - ctx.r9.u64;
	// beq cr6,0x825d900c
	if (ctx.cr6.eq) goto loc_825D900C;
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r8,r8,1
	ctx.r8.s64 = ctx.r8.s64 + 1;
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x825d8fe8
	if (ctx.cr6.eq) goto loc_825D8FE8;
loc_825D900C:
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x825d9030
	if (ctx.cr6.eq) goto loc_825D9030;
	// addi r10,r1,488
	ctx.r10.s64 = ctx.r1.s64 + 488;
	// subf r8,r11,r10
	ctx.r8.u64 = ctx.r10.u64 - ctx.r11.u64;
loc_825D901C:
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r8,r11
	PPC_STORE_U8(ctx.r8.u32 + ctx.r11.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x825d901c
	if (!ctx.cr6.eq) goto loc_825D901C;
loc_825D9030:
	// lwz r11,956(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 956);
	// mr r8,r29
	ctx.r8.u64 = ctx.r29.u64;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_825D903C:
	// lbz r7,0(r10)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
	// lbz r9,0(r8)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r8.u32 + 0);
	// cmpwi cr6,r7,0
	ctx.cr6.compare<int32_t>(ctx.r7.s32, 0, ctx.xer);
	// subf r6,r9,r7
	ctx.r6.u64 = ctx.r7.u64 - ctx.r9.u64;
	// beq cr6,0x825d9060
	if (ctx.cr6.eq) goto loc_825D9060;
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r8,r8,1
	ctx.r8.s64 = ctx.r8.s64 + 1;
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x825d903c
	if (ctx.cr6.eq) goto loc_825D903C;
loc_825D9060:
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x825d9084
	if (ctx.cr6.eq) goto loc_825D9084;
	// addi r10,r1,556
	ctx.r10.s64 = ctx.r1.s64 + 556;
	// subf r8,r11,r10
	ctx.r8.u64 = ctx.r10.u64 - ctx.r11.u64;
loc_825D9070:
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r8,r11
	PPC_STORE_U8(ctx.r8.u32 + ctx.r11.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x825d9070
	if (!ctx.cr6.eq) goto loc_825D9070;
loc_825D9084:
	// lwz r11,964(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 964);
	// mr r8,r29
	ctx.r8.u64 = ctx.r29.u64;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_825D9090:
	// lbz r7,0(r10)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
	// lbz r9,0(r8)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r8.u32 + 0);
	// cmpwi cr6,r7,0
	ctx.cr6.compare<int32_t>(ctx.r7.s32, 0, ctx.xer);
	// subf r6,r9,r7
	ctx.r6.u64 = ctx.r7.u64 - ctx.r9.u64;
	// beq cr6,0x825d90b4
	if (ctx.cr6.eq) goto loc_825D90B4;
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r8,r8,1
	ctx.r8.s64 = ctx.r8.s64 + 1;
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x825d9090
	if (ctx.cr6.eq) goto loc_825D9090;
loc_825D90B4:
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x825d90d8
	if (ctx.cr6.eq) goto loc_825D90D8;
	// addi r10,r1,624
	ctx.r10.s64 = ctx.r1.s64 + 624;
	// subf r8,r11,r10
	ctx.r8.u64 = ctx.r10.u64 - ctx.r11.u64;
loc_825D90C4:
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r8,r11
	PPC_STORE_U8(ctx.r8.u32 + ctx.r11.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x825d90c4
	if (!ctx.cr6.eq) goto loc_825D90C4;
loc_825D90D8:
	// lwz r11,972(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 972);
	// mr r8,r29
	ctx.r8.u64 = ctx.r29.u64;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_825D90E4:
	// lbz r7,0(r10)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
	// lbz r9,0(r8)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r8.u32 + 0);
	// cmpwi cr6,r7,0
	ctx.cr6.compare<int32_t>(ctx.r7.s32, 0, ctx.xer);
	// subf r6,r9,r7
	ctx.r6.u64 = ctx.r7.u64 - ctx.r9.u64;
	// beq cr6,0x825d9108
	if (ctx.cr6.eq) goto loc_825D9108;
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r8,r8,1
	ctx.r8.s64 = ctx.r8.s64 + 1;
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x825d90e4
	if (ctx.cr6.eq) goto loc_825D90E4;
loc_825D9108:
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x825d912c
	if (ctx.cr6.eq) goto loc_825D912C;
	// addi r10,r1,692
	ctx.r10.s64 = ctx.r1.s64 + 692;
	// subf r8,r11,r10
	ctx.r8.u64 = ctx.r10.u64 - ctx.r11.u64;
loc_825D9118:
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r8,r11
	PPC_STORE_U8(ctx.r8.u32 + ctx.r11.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x825d9118
	if (!ctx.cr6.eq) goto loc_825D9118;
loc_825D912C:
	// lwz r11,980(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 980);
	// mr r8,r29
	ctx.r8.u64 = ctx.r29.u64;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_825D9138:
	// lbz r7,0(r10)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
	// lbz r9,0(r8)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r8.u32 + 0);
	// cmpwi cr6,r7,0
	ctx.cr6.compare<int32_t>(ctx.r7.s32, 0, ctx.xer);
	// subf r6,r9,r7
	ctx.r6.u64 = ctx.r7.u64 - ctx.r9.u64;
	// beq cr6,0x825d915c
	if (ctx.cr6.eq) goto loc_825D915C;
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r8,r8,1
	ctx.r8.s64 = ctx.r8.s64 + 1;
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x825d9138
	if (ctx.cr6.eq) goto loc_825D9138;
loc_825D915C:
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x825d9180
	if (ctx.cr6.eq) goto loc_825D9180;
	// addi r10,r1,760
	ctx.r10.s64 = ctx.r1.s64 + 760;
	// subf r8,r11,r10
	ctx.r8.u64 = ctx.r10.u64 - ctx.r11.u64;
loc_825D916C:
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r8,r11
	PPC_STORE_U8(ctx.r8.u32 + ctx.r11.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x825d916c
	if (!ctx.cr6.eq) goto loc_825D916C;
loc_825D9180:
	// lwz r11,988(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 988);
	// mr r8,r29
	ctx.r8.u64 = ctx.r29.u64;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_825D918C:
	// lbz r7,0(r10)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
	// lbz r9,0(r8)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r8.u32 + 0);
	// cmpwi cr6,r7,0
	ctx.cr6.compare<int32_t>(ctx.r7.s32, 0, ctx.xer);
	// subf r6,r9,r7
	ctx.r6.u64 = ctx.r7.u64 - ctx.r9.u64;
	// beq cr6,0x825d91b0
	if (ctx.cr6.eq) goto loc_825D91B0;
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r8,r8,1
	ctx.r8.s64 = ctx.r8.s64 + 1;
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x825d918c
	if (ctx.cr6.eq) goto loc_825D918C;
loc_825D91B0:
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x825d91d4
	if (ctx.cr6.eq) goto loc_825D91D4;
	// addi r10,r1,828
	ctx.r10.s64 = ctx.r1.s64 + 828;
	// subf r8,r11,r10
	ctx.r8.u64 = ctx.r10.u64 - ctx.r11.u64;
loc_825D91C0:
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r8,r11
	PPC_STORE_U8(ctx.r8.u32 + ctx.r11.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x825d91c0
	if (!ctx.cr6.eq) goto loc_825D91C0;
loc_825D91D4:
	// lwz r11,996(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 996);
	// mr r9,r29
	ctx.r9.u64 = ctx.r29.u64;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_825D91E0:
	// lbz r8,0(r10)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
	// lbz r7,0(r9)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r9.u32 + 0);
	// cmpwi cr6,r8,0
	ctx.cr6.compare<int32_t>(ctx.r8.s32, 0, ctx.xer);
	// subf r7,r7,r8
	ctx.r7.u64 = ctx.r8.u64 - ctx.r7.u64;
	// beq cr6,0x825d9204
	if (ctx.cr6.eq) goto loc_825D9204;
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r9,r9,1
	ctx.r9.s64 = ctx.r9.s64 + 1;
	// cmpwi cr6,r7,0
	ctx.cr6.compare<int32_t>(ctx.r7.s32, 0, ctx.xer);
	// beq cr6,0x825d91e0
	if (ctx.cr6.eq) goto loc_825D91E0;
loc_825D9204:
	// cmpwi cr6,r7,0
	ctx.cr6.compare<int32_t>(ctx.r7.s32, 0, ctx.xer);
	// beq cr6,0x825d9228
	if (ctx.cr6.eq) goto loc_825D9228;
	// addi r10,r1,896
	ctx.r10.s64 = ctx.r1.s64 + 896;
	// subf r9,r11,r10
	ctx.r9.u64 = ctx.r10.u64 - ctx.r11.u64;
loc_825D9214:
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r9,r11
	PPC_STORE_U8(ctx.r9.u32 + ctx.r11.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x825d9214
	if (!ctx.cr6.eq) goto loc_825D9214;
loc_825D9228:
	// li r7,12
	ctx.r7.s64 = 12;
	// addi r6,r1,80
	ctx.r6.s64 = ctx.r1.s64 + 80;
	// li r5,0
	ctx.r5.s64 = 0;
	// bl 0x8229cd40
	ctx.lr = 0x825D9238;
	sub_8229CD40(ctx, base);
	// addi r1,r1,864
	ctx.r1.s64 = ctx.r1.s64 + 864;
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D9240);
PPC_WEAK_FUNC(sub_825D9240) { __imp__sub_825D9240(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D9240) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r4,-32070
	ctx.r4.s64 = -2101739520;
	// lwz r11,-11452(r4)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + -11452);
	// cmpwi cr6,r11,1500
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 1500, ctx.xer);
	// bge cr6,0x825d9300
	if (!ctx.cr6.lt) goto loc_825D9300;
	// lis r10,-32033
	ctx.r10.s64 = -2099314688;
	// lis r8,-32086
	ctx.r8.s64 = -2102788096;
	// addi r6,r10,464
	ctx.r6.s64 = ctx.r10.s64 + 464;
	// rlwinm r10,r11,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lis r9,-32086
	ctx.r9.s64 = -2102788096;
	// add r7,r10,r6
	ctx.r7.u64 = ctx.r10.u64 + ctx.r6.u64;
	// lis r10,-32086
	ctx.r10.s64 = -2102788096;
	// lwz r8,2672(r8)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r8.u32 + 2672);
	// lwz r9,2680(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + 2680);
	// lwz r5,2664(r10)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r10.u32 + 2664);
loc_825D9288:
	// lwz r10,0(r7)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x825d92d0
	if (ctx.cr6.eq) goto loc_825D92D0;
	// lbz r31,8(r10)
	ctx.r31.u64 = PPC_LOAD_U8(ctx.r10.u32 + 8);
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// bne cr6,0x825d92a8
	if (!ctx.cr6.eq) goto loc_825D92A8;
	// rlwinm r10,r5,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r10,r10,r6
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r6.u32);
loc_825D92A8:
	// lwz r10,92(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 92);
	// lwz r10,4(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// cmpw cr6,r10,r3
	ctx.cr6.compare<int32_t>(ctx.r10.s32, ctx.r3.s32, ctx.xer);
	// bne cr6,0x825d92d0
	if (!ctx.cr6.eq) goto loc_825D92D0;
	// cmpw cr6,r11,r5
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r5.s32, ctx.xer);
	// beq cr6,0x825d92d0
	if (ctx.cr6.eq) goto loc_825D92D0;
	// cmpw cr6,r11,r9
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r9.s32, ctx.xer);
	// beq cr6,0x825d92d0
	if (ctx.cr6.eq) goto loc_825D92D0;
	// cmpw cr6,r11,r8
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825d92e8
	if (!ctx.cr6.eq) goto loc_825D92E8;
loc_825D92D0:
	// addi r7,r7,4
	ctx.r7.s64 = ctx.r7.s64 + 4;
	// addi r10,r6,6000
	ctx.r10.s64 = ctx.r6.s64 + 6000;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// cmpw cr6,r7,r10
	ctx.cr6.compare<int32_t>(ctx.r7.s32, ctx.r10.s32, ctx.xer);
	// blt cr6,0x825d9288
	if (ctx.cr6.lt) goto loc_825D9288;
	// b 0x825d9300
	goto loc_825D9300;
loc_825D92E8:
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// stw r11,-11452(r4)
	PPC_STORE_U32(ctx.r4.u32 + -11452, ctx.r11.u32);
	// bl 0x82333040
	ctx.lr = 0x825D92F8;
	sub_82333040(ctx, base);
	// cmpwi cr6,r3,-1
	ctx.cr6.compare<int32_t>(ctx.r3.s32, -1, ctx.xer);
	// bne cr6,0x825d9304
	if (!ctx.cr6.eq) goto loc_825D9304;
loc_825D9300:
	// li r3,0
	ctx.r3.s64 = 0;
loc_825D9304:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D9318);
PPC_WEAK_FUNC(sub_825D9318) { __imp__sub_825D9318(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D9318) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r4
	ctx.r31.u64 = ctx.r4.u64;
	// bl 0x82333000
	ctx.lr = 0x825D9330;
	sub_82333000(ctx, base);
	// cmpwi cr6,r3,-1
	ctx.cr6.compare<int32_t>(ctx.r3.s32, -1, ctx.xer);
	// beq cr6,0x825d937c
	if (ctx.cr6.eq) goto loc_825D937C;
	// lis r11,-32033
	ctx.r11.s64 = -2099314688;
	// rlwinm r10,r3,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r11,r11,464
	ctx.r11.s64 = ctx.r11.s64 + 464;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lbz r10,8(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 8);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x825d9360
	if (ctx.cr6.eq) goto loc_825D9360;
	// li r10,48
	ctx.r10.s64 = 48;
	// lvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// b 0x825d9398
	goto loc_825D9398;
loc_825D9360:
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lfs f0,32(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 32);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// lfs f0,2612(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// lfs f0,36(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	ctx.f0.f64 = double(temp.f32);
	// b 0x825d938c
	goto loc_825D938C;
loc_825D937C:
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,2612(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f0,88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
loc_825D938C:
	// stfs f0,84(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
loc_825D9398:
	// stvx128 v0,r0,r31
	ea = (ctx.r31.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D93B0);
PPC_WEAK_FUNC(sub_825D93B0) { __imp__sub_825D93B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D93B0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// bl 0x82333000
	ctx.lr = 0x825D93C0;
	sub_82333000(ctx, base);
	// cmpwi cr6,r3,-1
	ctx.cr6.compare<int32_t>(ctx.r3.s32, -1, ctx.xer);
	// beq cr6,0x825d941c
	if (ctx.cr6.eq) goto loc_825D941C;
	// lis r11,-32033
	ctx.r11.s64 = -2099314688;
	// rlwinm r10,r3,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r11,r11,464
	ctx.r11.s64 = ctx.r11.s64 + 464;
	// lwzx r10,r10,r11
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lbz r9,8(r10)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r10.u32 + 8);
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// beq cr6,0x825d93f8
	if (ctx.cr6.eq) goto loc_825D93F8;
	// lwz r3,76(r10)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r10.u32 + 76);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_825D93F8:
	// lis r10,-32086
	ctx.r10.s64 = -2102788096;
	// lwz r10,2664(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 2664);
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lwz r3,76(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 76);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_825D941C:
	// li r3,0
	ctx.r3.s64 = 0;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D9430);
PPC_WEAK_FUNC(sub_825D9430) { __imp__sub_825D9430(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D9430) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// bl 0x82333000
	ctx.lr = 0x825D9440;
	sub_82333000(ctx, base);
	// cmpwi cr6,r3,-1
	ctx.cr6.compare<int32_t>(ctx.r3.s32, -1, ctx.xer);
	// beq cr6,0x825d949c
	if (ctx.cr6.eq) goto loc_825D949C;
	// lis r11,-32033
	ctx.r11.s64 = -2099314688;
	// rlwinm r10,r3,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r11,r11,464
	ctx.r11.s64 = ctx.r11.s64 + 464;
	// lwzx r10,r10,r11
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lbz r9,8(r10)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r10.u32 + 8);
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// beq cr6,0x825d9478
	if (ctx.cr6.eq) goto loc_825D9478;
	// lwz r3,72(r10)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r10.u32 + 72);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_825D9478:
	// lis r10,-32086
	ctx.r10.s64 = -2102788096;
	// lwz r10,2664(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 2664);
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lwz r3,72(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 72);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_825D949C:
	// li r3,0
	ctx.r3.s64 = 0;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D94B0);
PPC_WEAK_FUNC(sub_825D94B0) { __imp__sub_825D94B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D94B0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r4
	ctx.r31.u64 = ctx.r4.u64;
	// bl 0x82226028
	ctx.lr = 0x825D94CC;
	sub_82226028(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,4
	ctx.r6.s64 = 4;
	// li r4,6
	ctx.r4.s64 = 6;
	// li r3,1
	ctx.r3.s64 = 1;
	// mr r5,r30
	ctx.r5.u64 = ctx.r30.u64;
	// bl 0x82339fd8
	ctx.lr = 0x825D94E8;
	sub_82339FD8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r3,0
	ctx.r3.s64 = 0;
	// stw r4,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r4.u32);
	// lfs f1,3400(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3400);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x82334990
	ctx.lr = 0x825D9500;
	sub_82334990(ctx, base);
	// li r5,2
	ctx.r5.s64 = 2;
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// li r3,5
	ctx.r3.s64 = 5;
	// bl 0x8233a180
	ctx.lr = 0x825D9510;
	sub_8233A180(ctx, base);
	// rlwinm r10,r30,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// add r10,r30,r10
	ctx.r10.u64 = ctx.r30.u64 + ctx.r10.u64;
	// addi r11,r11,17152
	ctx.r11.s64 = ctx.r11.s64 + 17152;
	// rlwinm r10,r10,4,0,27
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// add r11,r10,r11
	ctx.r11.u64 = ctx.r10.u64 + ctx.r11.u64;
	// li r3,2
	ctx.r3.s64 = 2;
	// lfs f0,28(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// lfs f0,32(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 32);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,84(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// lfs f0,36(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// bl 0x82334a08
	ctx.lr = 0x825D9550;
	sub_82334A08(ctx, base);
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x82334830
	ctx.lr = 0x825D9558;
	sub_82334830(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825d956c
	if (!ctx.cr6.eq) goto loc_825D956C;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// b 0x825d9594
	goto loc_825D9594;
loc_825D956C:
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// lwz r30,8272(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8272);
	// bl 0x821fe708
	ctx.lr = 0x825D9578;
	sub_821FE708(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// li r5,18
	ctx.r5.s64 = 18;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r8,0
	ctx.r8.s64 = 0;
	// bl 0x822d6358
	ctx.lr = 0x825D9594;
	sub_822D6358(ctx, base);
loc_825D9594:
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D95B0);
PPC_WEAK_FUNC(sub_825D95B0) { __imp__sub_825D95B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D95B0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// bl 0x82226028
	ctx.lr = 0x825D95C8;
	sub_82226028(ctx, base);
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
	// addi r31,r11,17152
	ctx.r31.s64 = ctx.r11.s64 + 17152;
	// rlwinm r11,r5,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r10,r31,28
	ctx.r10.s64 = ctx.r31.s64 + 28;
	// add r11,r5,r11
	ctx.r11.u64 = ctx.r5.u64 + ctx.r11.u64;
	// addi r9,r31,32
	ctx.r9.s64 = ctx.r31.s64 + 32;
	// rlwinm r30,r11,4,0,27
	ctx.r30.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 4) & 0xFFFFFFF0;
	// addi r8,r31,36
	ctx.r8.s64 = ctx.r31.s64 + 36;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// lfsx f0,r30,r10
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r30.u32 + ctx.r10.u32);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// lfsx f0,r30,r9
	temp.u32 = PPC_LOAD_U32(ctx.r30.u32 + ctx.r9.u32);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,84(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// lfsx f0,r30,r8
	temp.u32 = PPC_LOAD_U32(ctx.r30.u32 + ctx.r8.u32);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// bl 0x8233a030
	ctx.lr = 0x825D9610;
	sub_8233A030(ctx, base);
	// addi r11,r31,20
	ctx.r11.s64 = ctx.r31.s64 + 20;
	// stwx r3,r30,r11
	PPC_STORE_U32(ctx.r30.u32 + ctx.r11.u32, ctx.r3.u32);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D9630);
PPC_WEAK_FUNC(sub_825D9630) { __imp__sub_825D9630(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D9630) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stfd f30,-40(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -40, ctx.f30.u64);
	// stfd f31,-32(r1)
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f31.u64);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// mr r31,r6
	ctx.r31.u64 = ctx.r6.u64;
	// lfs f0,5704(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 5704);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f3,f0
	ctx.cr6.compare(ctx.f3.f64, ctx.f0.f64);
	// bgt cr6,0x825d9674
	if (ctx.cr6.gt) goto loc_825D9674;
	// li r5,4
	ctx.r5.s64 = 4;
	// bl 0x82363040
	ctx.lr = 0x825D9670;
	sub_82363040(ctx, base);
	// fmr f3,f1
	ctx.fpscr.disableFlushMode();
	ctx.f3.f64 = ctx.f1.f64;
loc_825D9674:
	// li r7,0
	ctx.r7.s64 = 0;
	// stfs f31,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// li r6,4
	ctx.r6.s64 = 4;
	// stfs f30,84(r1)
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// stfs f3,88(r1)
	temp.f32 = float(ctx.f3.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// li r4,4
	ctx.r4.s64 = 4;
	// li r3,1
	ctx.r3.s64 = 1;
	// bl 0x8233a018
	ctx.lr = 0x825D9698;
	sub_8233A018(ctx, base);
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// bl 0x82334830
	ctx.lr = 0x825D96A0;
	sub_82334830(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825d96b4
	if (!ctx.cr6.eq) goto loc_825D96B4;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// b 0x825d96dc
	goto loc_825D96DC;
loc_825D96B4:
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// lwz r30,8272(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8272);
	// bl 0x821fe708
	ctx.lr = 0x825D96C0;
	sub_821FE708(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// li r5,18
	ctx.r5.s64 = 18;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r8,0
	ctx.r8.s64 = 0;
	// bl 0x822d6358
	ctx.lr = 0x825D96DC;
	sub_822D6358(ctx, base);
loc_825D96DC:
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f30,-40(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -40);
	// lfd f31,-32(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D9700);
PPC_WEAK_FUNC(sub_825D9700) { __imp__sub_825D9700(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D9700) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r4
	ctx.r31.u64 = ctx.r4.u64;
	// bl 0x82333000
	ctx.lr = 0x825D9718;
	sub_82333000(ctx, base);
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// blt cr6,0x825d9770
	if (ctx.cr6.lt) goto loc_825D9770;
	// lis r11,-32033
	ctx.r11.s64 = -2099314688;
	// rlwinm r10,r3,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r11,r11,464
	ctx.r11.s64 = ctx.r11.s64 + 464;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lbz r10,8(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 8);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x825d9748
	if (ctx.cr6.eq) goto loc_825D9748;
	// li r10,48
	ctx.r10.s64 = 48;
	// lvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// b 0x825d9774
	goto loc_825D9774;
loc_825D9748:
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lfs f0,32(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 32);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// lfs f0,2612(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// lfs f0,36(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,84(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// b 0x825d9774
	goto loc_825D9774;
loc_825D9770:
	// vspltisw v0,0
	simde_mm_store_si128((simde__m128i*)ctx.v0.u32, simde_mm_set1_epi32(int(0x0)));
loc_825D9774:
	// stvx128 v0,r0,r31
	ea = (ctx.r31.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D9790);
PPC_WEAK_FUNC(sub_825D9790) { __imp__sub_825D9790(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D9790) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stfd f30,-40(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -40, ctx.f30.u64);
	// stfd f31,-32(r1)
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f31.u64);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// mr r31,r6
	ctx.r31.u64 = ctx.r6.u64;
	// lfs f0,5704(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 5704);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f3,f0
	ctx.cr6.compare(ctx.f3.f64, ctx.f0.f64);
	// bgt cr6,0x825d97d4
	if (ctx.cr6.gt) goto loc_825D97D4;
	// li r5,4
	ctx.r5.s64 = 4;
	// bl 0x82363040
	ctx.lr = 0x825D97D0;
	sub_82363040(ctx, base);
	// fmr f3,f1
	ctx.fpscr.disableFlushMode();
	ctx.f3.f64 = ctx.f1.f64;
loc_825D97D4:
	// li r7,0
	ctx.r7.s64 = 0;
	// stfs f31,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// li r6,4
	ctx.r6.s64 = 4;
	// stfs f30,84(r1)
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// stfs f3,88(r1)
	temp.f32 = float(ctx.f3.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// li r4,5
	ctx.r4.s64 = 5;
	// li r3,1
	ctx.r3.s64 = 1;
	// bl 0x8233a018
	ctx.lr = 0x825D97F8;
	sub_8233A018(ctx, base);
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// bl 0x82334830
	ctx.lr = 0x825D9800;
	sub_82334830(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825d9814
	if (!ctx.cr6.eq) goto loc_825D9814;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// b 0x825d983c
	goto loc_825D983C;
loc_825D9814:
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// lwz r30,8272(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8272);
	// bl 0x821fe708
	ctx.lr = 0x825D9820;
	sub_821FE708(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// li r5,18
	ctx.r5.s64 = 18;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r8,0
	ctx.r8.s64 = 0;
	// bl 0x822d6358
	ctx.lr = 0x825D983C;
	sub_822D6358(ctx, base);
loc_825D983C:
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f30,-40(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -40);
	// lfd f31,-32(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D9860);
PPC_WEAK_FUNC(sub_825D9860) { __imp__sub_825D9860(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D9860) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// bl 0x82333000
	ctx.lr = 0x825D9870;
	sub_82333000(ctx, base);
	// mr r10,r3
	ctx.r10.u64 = ctx.r3.u64;
	// li r3,-1
	ctx.r3.s64 = -1;
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// blt cr6,0x825d98cc
	if (ctx.cr6.lt) goto loc_825D98CC;
	// lis r11,-32033
	ctx.r11.s64 = -2099314688;
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r11,r11,464
	ctx.r11.s64 = ctx.r11.s64 + 464;
	// lwzx r10,r10,r11
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lbz r9,8(r10)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r10.u32 + 8);
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// beq cr6,0x825d98b4
	if (ctx.cr6.eq) goto loc_825D98B4;
	// lwz r11,92(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 92);
	// lwz r3,4(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_825D98B4:
	// lis r10,-32086
	ctx.r10.s64 = -2102788096;
	// lwz r10,2664(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 2664);
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lwz r11,92(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 92);
	// lwz r3,4(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
loc_825D98CC:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D98E0);
PPC_WEAK_FUNC(sub_825D98E0) { __imp__sub_825D98E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D98E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// bl 0x82333000
	ctx.lr = 0x825D98F4;
	sub_82333000(ctx, base);
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// addi r31,r11,4024
	ctx.r31.s64 = ctx.r11.s64 + 4024;
	// li r11,0
	ctx.r11.s64 = 0;
	// stb r11,0(r31)
	PPC_STORE_U8(ctx.r31.u32 + 0, ctx.r11.u8);
	// blt cr6,0x825d9934
	if (ctx.cr6.lt) goto loc_825D9934;
	// bl 0x82333768
	ctx.lr = 0x825D9910;
	sub_82333768(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// bl 0x8221f068
	ctx.lr = 0x825D9918;
	sub_8221F068(ctx, base);
	// mr r11,r31
	ctx.r11.u64 = ctx.r31.u64;
loc_825D991C:
	// lbz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r3.u32 + 0);
	// addi r3,r3,1
	ctx.r3.s64 = ctx.r3.s64 + 1;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stb r10,0(r11)
	PPC_STORE_U8(ctx.r11.u32 + 0, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x825d991c
	if (!ctx.cr6.eq) goto loc_825D991C;
loc_825D9934:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D9950);
PPC_WEAK_FUNC(sub_825D9950) { __imp__sub_825D9950(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D9950) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r4
	ctx.r31.u64 = ctx.r4.u64;
	// bl 0x82333000
	ctx.lr = 0x825D9968;
	sub_82333000(ctx, base);
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// blt cr6,0x825d99f8
	if (ctx.cr6.lt) goto loc_825D99F8;
	// lis r11,-32033
	ctx.r11.s64 = -2099314688;
	// lwz r9,0(r31)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// rlwinm r10,r3,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r11,r11,464
	ctx.r11.s64 = ctx.r11.s64 + 464;
	// stw r9,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r9.u32);
	// lis r9,-32086
	ctx.r9.s64 = -2102788096;
	// lwzx r10,r10,r11
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lwz r9,2664(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + 2664);
	// lbz r8,8(r10)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r10.u32 + 8);
	// cmplwi cr6,r8,0
	ctx.cr6.compare<uint32_t>(ctx.r8.u32, 0, ctx.xer);
	// beq cr6,0x825d99a4
	if (ctx.cr6.eq) goto loc_825D99A4;
	// lhz r7,32(r10)
	ctx.r7.u64 = PPC_LOAD_U16(ctx.r10.u32 + 32);
	// b 0x825d99b0
	goto loc_825D99B0;
loc_825D99A4:
	// rlwinm r7,r9,2,0,29
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r7,r7,r11
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r7.u32 + ctx.r11.u32);
	// lhz r7,32(r7)
	ctx.r7.u64 = PPC_LOAD_U16(ctx.r7.u32 + 32);
loc_825D99B0:
	// rlwinm r7,r7,31,1,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 31) & 0x7FFFFFFF;
	// cmplwi cr6,r8,0
	ctx.cr6.compare<uint32_t>(ctx.r8.u32, 0, ctx.xer);
	// clrlwi r5,r7,31
	ctx.r5.u64 = ctx.r7.u32 & 0x1;
	// beq cr6,0x825d99d8
	if (ctx.cr6.eq) goto loc_825D99D8;
	// addi r6,r1,80
	ctx.r6.s64 = ctx.r1.s64 + 80;
	// lwz r4,84(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 84);
	// addi r3,r1,84
	ctx.r3.s64 = ctx.r1.s64 + 84;
	// bl 0x823376d0
	ctx.lr = 0x825D99D0;
	sub_823376D0(ctx, base);
	// lwz r11,80(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// b 0x825d99fc
	goto loc_825D99FC;
loc_825D99D8:
	// rlwinm r10,r9,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r6,r1,80
	ctx.r6.s64 = ctx.r1.s64 + 80;
	// addi r3,r1,84
	ctx.r3.s64 = ctx.r1.s64 + 84;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lwz r4,84(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 84);
	// bl 0x823376d0
	ctx.lr = 0x825D99F0;
	sub_823376D0(ctx, base);
	// lwz r11,80(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// b 0x825d99fc
	goto loc_825D99FC;
loc_825D99F8:
	// li r11,0
	ctx.r11.s64 = 0;
loc_825D99FC:
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D9A18);
PPC_WEAK_FUNC(sub_825D9A18) { __imp__sub_825D9A18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D9A18) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r4
	ctx.r31.u64 = ctx.r4.u64;
	// bl 0x82333000
	ctx.lr = 0x825D9A30;
	sub_82333000(ctx, base);
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// blt cr6,0x825d9a74
	if (ctx.cr6.lt) goto loc_825D9A74;
	// lis r11,-32033
	ctx.r11.s64 = -2099314688;
	// rlwinm r10,r3,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r11,r11,464
	ctx.r11.s64 = ctx.r11.s64 + 464;
	// lwzx r10,r10,r11
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lbz r9,8(r10)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r10.u32 + 8);
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// beq cr6,0x825d9a5c
	if (ctx.cr6.eq) goto loc_825D9A5C;
	// lbz r11,88(r10)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r10.u32 + 88);
	// b 0x825d9a78
	goto loc_825D9A78;
loc_825D9A5C:
	// lis r10,-32086
	ctx.r10.s64 = -2102788096;
	// lwz r10,2664(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 2664);
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lbz r11,88(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 88);
	// b 0x825d9a78
	goto loc_825D9A78;
loc_825D9A74:
	// li r11,255
	ctx.r11.s64 = 255;
loc_825D9A78:
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D9A90);
PPC_WEAK_FUNC(sub_825D9A90) { __imp__sub_825D9A90(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D9A90) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// bl 0x82333000
	ctx.lr = 0x825D9AA0;
	sub_82333000(ctx, base);
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// blt cr6,0x825d9b0c
	if (ctx.cr6.lt) goto loc_825D9B0C;
	// lis r11,-32033
	ctx.r11.s64 = -2099314688;
	// rlwinm r10,r3,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r11,r11,464
	ctx.r11.s64 = ctx.r11.s64 + 464;
	// lwzx r10,r10,r11
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lbz r9,8(r10)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r10.u32 + 8);
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// beq cr6,0x825d9ae0
	if (ctx.cr6.eq) goto loc_825D9AE0;
	// lhz r11,32(r10)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r10.u32 + 32);
	// rlwinm r11,r11,29,3,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 29) & 0x1FFFFFFF;
	// clrlwi r3,r11,31
	ctx.r3.u64 = ctx.r11.u32 & 0x1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_825D9AE0:
	// lis r10,-32086
	ctx.r10.s64 = -2102788096;
	// lwz r10,2664(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 2664);
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lhz r11,32(r11)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r11.u32 + 32);
	// rlwinm r11,r11,29,3,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 29) & 0x1FFFFFFF;
	// clrlwi r3,r11,31
	ctx.r3.u64 = ctx.r11.u32 & 0x1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_825D9B0C:
	// li r3,0
	ctx.r3.s64 = 0;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D9B20);
PPC_WEAK_FUNC(sub_825D9B20) { __imp__sub_825D9B20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D9B20) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
loc_825D9B2C:
	// lbz r8,0(r10)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// cmplwi cr6,r8,0
	ctx.cr6.compare<uint32_t>(ctx.r8.u32, 0, ctx.xer);
	// bne cr6,0x825d9b2c
	if (!ctx.cr6.eq) goto loc_825D9B2C;
	// subf r10,r9,r10
	ctx.r10.u64 = ctx.r10.u64 - ctx.r9.u64;
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
	// rotlwi r10,r10,0
	ctx.r10.u64 = __builtin_rotateleft32(ctx.r10.u32, 0);
	// cmplwi cr6,r10,10
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 10, ctx.xer);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// ble cr6,0x825d9b60
	if (!ctx.cr6.gt) goto loc_825D9B60;
	// li r11,0
	ctx.r11.s64 = 0;
	// stb r11,-10352(r10)
	PPC_STORE_U8(ctx.r10.u32 + -10352, ctx.r11.u8);
	// blr 
	return;
loc_825D9B60:
	// addi r10,r10,-10352
	ctx.r10.s64 = ctx.r10.s64 + -10352;
	// subf r9,r11,r10
	ctx.r9.u64 = ctx.r10.u64 - ctx.r11.u64;
loc_825D9B68:
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r9,r11
	PPC_STORE_U8(ctx.r9.u32 + ctx.r11.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x825d9b68
	if (!ctx.cr6.eq) goto loc_825D9B68;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D9B80);
PPC_WEAK_FUNC(sub_825D9B80) { __imp__sub_825D9B80(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D9B80) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// clrlwi r9,r3,24
	ctx.r9.u64 = ctx.r3.u32 & 0xFF;
	// addi r31,r11,-10340
	ctx.r31.s64 = ctx.r11.s64 + -10340;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r10,r4
	ctx.r10.u64 = ctx.r4.u64;
	// mr r30,r5
	ctx.r30.u64 = ctx.r5.u64;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// stb r11,0(r31)
	PPC_STORE_U8(ctx.r31.u32 + 0, ctx.r11.u8);
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// addi r11,r11,-24840
	ctx.r11.s64 = ctx.r11.s64 + -24840;
	// lwz r11,128(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 128);
	// beq cr6,0x825d9bec
	if (ctx.cr6.eq) goto loc_825D9BEC;
	// cmpwi cr6,r11,4
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 4, ctx.xer);
	// beq cr6,0x825d9be0
	if (ctx.cr6.eq) goto loc_825D9BE0;
	// cmpwi cr6,r11,1
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 1, ctx.xer);
	// beq cr6,0x825d9be0
	if (ctx.cr6.eq) goto loc_825D9BE0;
	// lis r11,-32253
	ctx.r11.s64 = -2113732608;
	// addi r4,r11,8828
	ctx.r4.s64 = ctx.r11.s64 + 8828;
	// b 0x825d9c10
	goto loc_825D9C10;
loc_825D9BE0:
	// lis r11,-32253
	ctx.r11.s64 = -2113732608;
	// addi r4,r11,8820
	ctx.r4.s64 = ctx.r11.s64 + 8820;
	// b 0x825d9c10
	goto loc_825D9C10;
loc_825D9BEC:
	// cmpwi cr6,r11,4
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 4, ctx.xer);
	// beq cr6,0x825d9c08
	if (ctx.cr6.eq) goto loc_825D9C08;
	// cmpwi cr6,r11,1
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 1, ctx.xer);
	// beq cr6,0x825d9c08
	if (ctx.cr6.eq) goto loc_825D9C08;
	// lis r11,-32253
	ctx.r11.s64 = -2113732608;
	// addi r4,r11,8812
	ctx.r4.s64 = ctx.r11.s64 + 8812;
	// b 0x825d9c10
	goto loc_825D9C10;
loc_825D9C08:
	// lis r11,-32253
	ctx.r11.s64 = -2113732608;
	// addi r4,r11,8804
	ctx.r4.s64 = ctx.r11.s64 + 8804;
loc_825D9C10:
	// mr r5,r10
	ctx.r5.u64 = ctx.r10.u64;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x82a00108
	ctx.lr = 0x825D9C1C;
	sub_82A00108(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
loc_825D9C20:
	// addi r10,r1,80
	ctx.r10.s64 = ctx.r1.s64 + 80;
	// addi r9,r31,1
	ctx.r9.s64 = ctx.r31.s64 + 1;
	// lbzx r10,r11,r10
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + ctx.r10.u32);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r11,r9
	PPC_STORE_U8(ctx.r11.u32 + ctx.r9.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x825d9c20
	if (!ctx.cr6.eq) goto loc_825D9C20;
	// stw r30,52(r31)
	PPC_STORE_U32(ctx.r31.u32 + 52, ctx.r30.u32);
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D9C58);
PPC_WEAK_FUNC(sub_825D9C58) { __imp__sub_825D9C58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D9C58) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// lwz r11,-24260(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -24260);
	// cmpwi cr6,r11,2
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 2, ctx.xer);
	// beq cr6,0x825d9cfc
	if (ctx.cr6.eq) goto loc_825D9CFC;
	// cmpwi cr6,r11,6
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 6, ctx.xer);
	// beq cr6,0x825d9cfc
	if (ctx.cr6.eq) goto loc_825D9CFC;
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// lbz r11,-24252(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + -24252);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d9cfc
	if (ctx.cr6.eq) goto loc_825D9CFC;
	// bl 0x826cbdc0
	ctx.lr = 0x825D9C90;
	sub_826CBDC0(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825d9cfc
	if (!ctx.cr6.eq) goto loc_825D9CFC;
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// lbz r11,-24280(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + -24280);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825d9cfc
	if (!ctx.cr6.eq) goto loc_825D9CFC;
	// bl 0x826c6be0
	ctx.lr = 0x825D9CB0;
	sub_826C6BE0(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d9cf4
	if (ctx.cr6.eq) goto loc_825D9CF4;
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// lbz r11,-24278(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + -24278);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d9cd4
	if (ctx.cr6.eq) goto loc_825D9CD4;
	// li r3,0
	ctx.r3.s64 = 0;
	// bl 0x82168b60
	ctx.lr = 0x825D9CD4;
	sub_82168B60(ctx, base);
loc_825D9CD4:
	// bl 0x8214ad88
	ctx.lr = 0x825D9CD8;
	sub_8214AD88(ctx, base);
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// addi r3,r11,-2080
	ctx.r3.s64 = ctx.r11.s64 + -2080;
	// bl 0x821b5a68
	ctx.lr = 0x825D9CE4;
	sub_821B5A68(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_825D9CF4:
	// li r3,2
	ctx.r3.s64 = 2;
	// bl 0x8224efe8
	ctx.lr = 0x825D9CFC;
	sub_8224EFE8(ctx, base);
loc_825D9CFC:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D9D10);
PPC_WEAK_FUNC(sub_825D9D10) { __imp__sub_825D9D10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D9D10) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// lwz r11,-24260(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -24260);
	// cmpwi cr6,r11,2
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 2, ctx.xer);
	// beq cr6,0x825d9d34
	if (ctx.cr6.eq) goto loc_825D9D34;
	// cmpwi cr6,r11,6
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 6, ctx.xer);
	// beq cr6,0x825d9d34
	if (ctx.cr6.eq) goto loc_825D9D34;
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// lbz r3,-24252(r11)
	ctx.r3.u64 = PPC_LOAD_U8(ctx.r11.u32 + -24252);
	// blr 
	return;
loc_825D9D34:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D9D40);
PPC_WEAK_FUNC(sub_825D9D40) { __imp__sub_825D9D40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D9D40) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7cc
	ctx.lr = 0x825D9D48;
	__savegprlr_29(ctx, base);
	// addi r12,r1,-32
	ctx.r12.s64 = ctx.r1.s64 + -32;
	// bl 0x82a01324
	ctx.lr = 0x825D9D50;
	__savefpr_27(ctx, base);
	// stwu r1,-240(r1)
	ea = -240 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// fmr f31,f1
	ctx.fpscr.disableFlushMode();
	ctx.f31.f64 = ctx.f1.f64;
	// mr r29,r7
	ctx.r29.u64 = ctx.r7.u64;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// mr r31,r8
	ctx.r31.u64 = ctx.r8.u64;
	// fmr f29,f3
	ctx.f29.f64 = ctx.f3.f64;
	// mr r30,r10
	ctx.r30.u64 = ctx.r10.u64;
	// fmr f28,f4
	ctx.f28.f64 = ctx.f4.f64;
	// fmr f27,f5
	ctx.f27.f64 = ctx.f5.f64;
	// bl 0x821cb4f0
	ctx.lr = 0x825D9D78;
	sub_821CB4F0(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d9e34
	if (ctx.cr6.eq) goto loc_825D9E34;
	// clrlwi r3,r31,24
	ctx.r3.u64 = ctx.r31.u32 & 0xFF;
	// bl 0x8250e700
	ctx.lr = 0x825D9D8C;
	sub_8250E700(ctx, base);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x825d9e34
	if (ctx.cr6.eq) goto loc_825D9E34;
	// lwz r11,5544(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 5544);
	// li r3,0
	ctx.r3.s64 = 0;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825d9da8
	if (ctx.cr6.eq) goto loc_825D9DA8;
	// lwz r3,10(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 10);
loc_825D9DA8:
	// bl 0x821cb190
	ctx.lr = 0x825D9DAC;
	sub_821CB190(ctx, base);
	// clrlwi r11,r30,24
	ctx.r11.u64 = ctx.r30.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// addi r11,r3,4
	ctx.r11.s64 = ctx.r3.s64 + 4;
	// bne cr6,0x825d9dc0
	if (!ctx.cr6.eq) goto loc_825D9DC0;
	// addi r11,r3,8
	ctx.r11.s64 = ctx.r3.s64 + 8;
loc_825D9DC0:
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r9,r1,152
	ctx.r9.s64 = ctx.r1.s64 + 152;
	// fmr f5,f27
	ctx.fpscr.disableFlushMode();
	ctx.f5.f64 = ctx.f27.f64;
	// addi r10,r1,144
	ctx.r10.s64 = ctx.r1.s64 + 144;
	// stw r29,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r29.u32);
	// li r8,6
	ctx.r8.s64 = 6;
	// fmr f4,f28
	ctx.f4.f64 = ctx.f28.f64;
	// li r4,-2
	ctx.r4.s64 = -2;
	// fmr f3,f29
	ctx.f3.f64 = ctx.f29.f64;
	// lfs f0,3400(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3400);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// stfs f0,144(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 144, temp.u32);
	// stw r9,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r9.u32);
	// stfs f0,148(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 148, temp.u32);
	// li r9,0
	ctx.r9.s64 = 0;
	// stw r10,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r10.u32);
	// li r10,255
	ctx.r10.s64 = 255;
	// stw r8,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r8.u32);
	// fmr f2,f30
	ctx.f2.f64 = ctx.f30.f64;
	// lfs f0,2612(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// li r11,255
	ctx.r11.s64 = 255;
	// stfs f0,152(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 152, temp.u32);
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// stfs f0,156(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 156, temp.u32);
	// stb r9,111(r1)
	PPC_STORE_U8(ctx.r1.u32 + 111, ctx.r9.u8);
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x8247a3a0
	ctx.lr = 0x825D9E34;
	sub_8247A3A0(ctx, base);
loc_825D9E34:
	// addi r1,r1,240
	ctx.r1.s64 = ctx.r1.s64 + 240;
	// addi r12,r1,-32
	ctx.r12.s64 = ctx.r1.s64 + -32;
	// bl 0x82a01370
	ctx.lr = 0x825D9E40;
	__restfpr_27(ctx, base);
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D9E48);
PPC_WEAK_FUNC(sub_825D9E48) { __imp__sub_825D9E48(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D9E48) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stfd f30,-40(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -40, ctx.f30.u64);
	// stfd f31,-32(r1)
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f31.u64);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// stfs f1,80(r1)
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// mr r30,r7
	ctx.r30.u64 = ctx.r7.u64;
	// stfs f2,84(r1)
	temp.f32 = float(ctx.f2.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// li r7,0
	ctx.r7.s64 = 0;
	// fmr f31,f3
	ctx.f31.f64 = ctx.f3.f64;
	// li r6,4
	ctx.r6.s64 = 4;
	// fmr f30,f4
	ctx.f30.f64 = ctx.f4.f64;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// li r4,9
	ctx.r4.s64 = 9;
	// lfs f0,2612(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// li r3,1
	ctx.r3.s64 = 1;
	// stfs f0,88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// mr r31,r8
	ctx.r31.u64 = ctx.r8.u64;
	// bl 0x8233a018
	ctx.lr = 0x825D9EA0;
	sub_8233A018(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// li r3,0
	ctx.r3.s64 = 0;
	// fmr f1,f31
	ctx.fpscr.disableFlushMode();
	ctx.f1.f64 = ctx.f31.f64;
	// stw r4,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r4.u32);
	// bl 0x82334990
	ctx.lr = 0x825D9EB4;
	sub_82334990(ctx, base);
	// li r3,16
	ctx.r3.s64 = 16;
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// fmr f1,f30
	ctx.fpscr.disableFlushMode();
	ctx.f1.f64 = ctx.f30.f64;
	// bl 0x82334990
	ctx.lr = 0x825D9EC4;
	sub_82334990(ctx, base);
	// mr r5,r30
	ctx.r5.u64 = ctx.r30.u64;
	// li r3,5
	ctx.r3.s64 = 5;
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x8233a180
	ctx.lr = 0x825D9ED4;
	sub_8233A180(ctx, base);
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x82334830
	ctx.lr = 0x825D9EDC;
	sub_82334830(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825d9ef0
	if (!ctx.cr6.eq) goto loc_825D9EF0;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// b 0x825d9f18
	goto loc_825D9F18;
loc_825D9EF0:
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// lwz r30,8272(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8272);
	// bl 0x821fe708
	ctx.lr = 0x825D9EFC;
	sub_821FE708(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// li r5,18
	ctx.r5.s64 = 18;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r8,0
	ctx.r8.s64 = 0;
	// bl 0x822d6358
	ctx.lr = 0x825D9F18;
	sub_822D6358(ctx, base);
loc_825D9F18:
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f30,-40(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -40);
	// lfd f31,-32(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825D9F38);
PPC_WEAK_FUNC(sub_825D9F38) { __imp__sub_825D9F38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825D9F38) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stfd f31,-32(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f31.u64);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// stfs f1,80(r1)
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// mr r30,r6
	ctx.r30.u64 = ctx.r6.u64;
	// stfs f2,84(r1)
	temp.f32 = float(ctx.f2.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// mr r31,r7
	ctx.r31.u64 = ctx.r7.u64;
	// fmr f31,f3
	ctx.f31.f64 = ctx.f3.f64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,4
	ctx.r6.s64 = 4;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// lfs f0,2612(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// li r4,7
	ctx.r4.s64 = 7;
	// stfs f0,88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// li r3,1
	ctx.r3.s64 = 1;
	// bl 0x8233a018
	ctx.lr = 0x825D9F88;
	sub_8233A018(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// li r3,0
	ctx.r3.s64 = 0;
	// fmr f1,f31
	ctx.fpscr.disableFlushMode();
	ctx.f1.f64 = ctx.f31.f64;
	// stw r4,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r4.u32);
	// bl 0x82334990
	ctx.lr = 0x825D9F9C;
	sub_82334990(ctx, base);
	// mr r5,r30
	ctx.r5.u64 = ctx.r30.u64;
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// li r3,5
	ctx.r3.s64 = 5;
	// bl 0x8233a180
	ctx.lr = 0x825D9FAC;
	sub_8233A180(ctx, base);
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x82334830
	ctx.lr = 0x825D9FB4;
	sub_82334830(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825d9fc8
	if (!ctx.cr6.eq) goto loc_825D9FC8;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// b 0x825d9ff0
	goto loc_825D9FF0;
loc_825D9FC8:
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// lwz r30,8272(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8272);
	// bl 0x821fe708
	ctx.lr = 0x825D9FD4;
	sub_821FE708(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// li r5,18
	ctx.r5.s64 = 18;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r8,0
	ctx.r8.s64 = 0;
	// bl 0x822d6358
	ctx.lr = 0x825D9FF0;
	sub_822D6358(ctx, base);
loc_825D9FF0:
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f31,-32(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA010);
PPC_WEAK_FUNC(sub_825DA010) { __imp__sub_825DA010(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA010) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stfd f30,-32(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f30.u64);
	// stfd f31,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// bl 0x82333000
	ctx.lr = 0x825DA038;
	sub_82333000(ctx, base);
	// lis r11,-32033
	ctx.r11.s64 = -2099314688;
	// rlwinm r10,r3,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r11,r11,464
	ctx.r11.s64 = ctx.r11.s64 + 464;
	// lwzx r10,r10,r11
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lbz r9,8(r10)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r10.u32 + 8);
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// beq cr6,0x825da05c
	if (ctx.cr6.eq) goto loc_825DA05C;
	// lwz r11,72(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 72);
	// b 0x825da070
	goto loc_825DA070;
loc_825DA05C:
	// lis r10,-32086
	ctx.r10.s64 = -2102788096;
	// lwz r10,2664(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 2664);
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lwz r11,72(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 72);
loc_825DA070:
	// cmpwi cr6,r11,9
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 9, ctx.xer);
	// bne cr6,0x825da098
	if (!ctx.cr6.eq) goto loc_825DA098;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// fmr f1,f31
	ctx.fpscr.disableFlushMode();
	ctx.f1.f64 = ctx.f31.f64;
	// li r3,0
	ctx.r3.s64 = 0;
	// bl 0x82334990
	ctx.lr = 0x825DA088;
	sub_82334990(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// li r3,16
	ctx.r3.s64 = 16;
	// fmr f1,f30
	ctx.fpscr.disableFlushMode();
	ctx.f1.f64 = ctx.f30.f64;
	// bl 0x82334990
	ctx.lr = 0x825DA098;
	sub_82334990(ctx, base);
loc_825DA098:
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f30,-32(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// lfd f31,-24(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA0B8);
PPC_WEAK_FUNC(sub_825DA0B8) { __imp__sub_825DA0B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA0B8) {
	PPC_FUNC_PROLOGUE();
	// b 0x822225b8
	sub_822225B8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA0C0);
PPC_WEAK_FUNC(sub_825DA0C0) { __imp__sub_825DA0C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA0C0) {
	PPC_FUNC_PROLOGUE();
	// b 0x822233e8
	sub_822233E8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA0C8);
PPC_WEAK_FUNC(sub_825DA0C8) { __imp__sub_825DA0C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA0C8) {
	PPC_FUNC_PROLOGUE();
	// b 0x82221e58
	sub_82221E58(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA0D0);
PPC_WEAK_FUNC(sub_825DA0D0) { __imp__sub_825DA0D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA0D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// lbz r11,-24252(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + -24252);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bnelr cr6
	if (!ctx.cr6.eq) return;
	// li r3,15
	ctx.r3.s64 = 15;
	// b 0x8224efe8
	sub_8224EFE8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA0E8);
PPC_WEAK_FUNC(sub_825DA0E8) { __imp__sub_825DA0E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA0E8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA0F0);
PPC_WEAK_FUNC(sub_825DA0F0) { __imp__sub_825DA0F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA0F0) {
	PPC_FUNC_PROLOGUE();
	// b 0x825d9c58
	sub_825D9C58(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA0F8);
PPC_WEAK_FUNC(sub_825DA0F8) { __imp__sub_825DA0F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA0F8) {
	PPC_FUNC_PROLOGUE();
	// b 0x825d8310
	sub_825D8310(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA100);
PPC_WEAK_FUNC(sub_825DA100) { __imp__sub_825DA100(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA100) {
	PPC_FUNC_PROLOGUE();
	// b 0x821f3458
	sub_821F3458(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA108);
PPC_WEAK_FUNC(sub_825DA108) { __imp__sub_825DA108(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA108) {
	PPC_FUNC_PROLOGUE();
	// b 0x825d8a68
	sub_825D8A68(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA110);
PPC_WEAK_FUNC(sub_825DA110) { __imp__sub_825DA110(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA110) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// bl 0x8224f9d0
	ctx.lr = 0x825DA120;
	sub_8224F9D0(ctx, base);
	// bl 0x826c4ca8
	ctx.lr = 0x825DA124;
	sub_826C4CA8(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825da134
	if (ctx.cr6.eq) goto loc_825DA134;
	// bl 0x826d6b48
	ctx.lr = 0x825DA134;
	sub_826D6B48(ctx, base);
loc_825DA134:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA148);
PPC_WEAK_FUNC(sub_825DA148) { __imp__sub_825DA148(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA148) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32087
	ctx.r10.s64 = -2102853632;
	// li r11,0
	ctx.r11.s64 = 0;
	// stb r11,17844(r10)
	PPC_STORE_U8(ctx.r10.u32 + 17844, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA158);
PPC_WEAK_FUNC(sub_825DA158) { __imp__sub_825DA158(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA158) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,7588(r10)
	PPC_STORE_U8(ctx.r10.u32 + 7588, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA168);
PPC_WEAK_FUNC(sub_825DA168) { __imp__sub_825DA168(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA168) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,7587(r10)
	PPC_STORE_U8(ctx.r10.u32 + 7587, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA178);
PPC_WEAK_FUNC(sub_825DA178) { __imp__sub_825DA178(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA178) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,7586(r10)
	PPC_STORE_U8(ctx.r10.u32 + 7586, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA188);
PPC_WEAK_FUNC(sub_825DA188) { __imp__sub_825DA188(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA188) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,7589(r10)
	PPC_STORE_U8(ctx.r10.u32 + 7589, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA198);
PPC_WEAK_FUNC(sub_825DA198) { __imp__sub_825DA198(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA198) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// li r10,1
	ctx.r10.s64 = 1;
	// addi r11,r11,19272
	ctx.r11.s64 = ctx.r11.s64 + 19272;
	// stb r10,820(r11)
	PPC_STORE_U8(ctx.r11.u32 + 820, ctx.r10.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA1B0);
PPC_WEAK_FUNC(sub_825DA1B0) { __imp__sub_825DA1B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA1B0) {
	PPC_FUNC_PROLOGUE();
	// li r3,1
	ctx.r3.s64 = 1;
	// b 0x821ccf68
	sub_821CCF68(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA1B8);
PPC_WEAK_FUNC(sub_825DA1B8) { __imp__sub_825DA1B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA1B8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,7584(r10)
	PPC_STORE_U8(ctx.r10.u32 + 7584, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA1C8);
PPC_WEAK_FUNC(sub_825DA1C8) { __imp__sub_825DA1C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA1C8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,7590(r10)
	PPC_STORE_U8(ctx.r10.u32 + 7590, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA1D8);
PPC_WEAK_FUNC(sub_825DA1D8) { __imp__sub_825DA1D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA1D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// addi r10,r11,-1472
	ctx.r10.s64 = ctx.r11.s64 + -1472;
	// li r11,0
	ctx.r11.s64 = 0;
	// lhz r9,4(r10)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r10.u32 + 4);
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// ble cr6,0x825da218
	if (!ctx.cr6.gt) goto loc_825DA218;
	// lwz r8,0(r10)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// mr r10,r8
	ctx.r10.u64 = ctx.r8.u64;
loc_825DA1F8:
	// lwz r7,0(r10)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// lwz r7,1340(r7)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r7.u32 + 1340);
	// cmpw cr6,r7,r3
	ctx.cr6.compare<int32_t>(ctx.r7.s32, ctx.r3.s32, ctx.xer);
	// beq cr6,0x825da238
	if (ctx.cr6.eq) goto loc_825DA238;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// addi r10,r10,4
	ctx.r10.s64 = ctx.r10.s64 + 4;
	// cmpw cr6,r11,r9
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r9.s32, ctx.xer);
	// blt cr6,0x825da1f8
	if (ctx.cr6.lt) goto loc_825DA1F8;
loc_825DA218:
	// li r11,0
	ctx.r11.s64 = 0;
loc_825DA21C:
	// lwz r11,1344(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 1344);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// stw r11,-540(r10)
	PPC_STORE_U32(ctx.r10.u32 + -540, ctx.r11.u32);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,7576(r10)
	PPC_STORE_U32(ctx.r10.u32 + 7576, ctx.r11.u32);
	// blr 
	return;
loc_825DA238:
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r11,r11,r8
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + ctx.r8.u32);
	// b 0x825da21c
	goto loc_825DA21C;
}

PPC_FUNC_IMPL(__imp__sub_825DA248);
PPC_WEAK_FUNC(sub_825DA248) { __imp__sub_825DA248(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA248) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r31,0
	ctx.r31.s64 = 0;
	// stw r31,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r31.u32);
	// bl 0x82333000
	ctx.lr = 0x825DA264;
	sub_82333000(ctx, base);
	// cmpwi cr6,r3,-1
	ctx.cr6.compare<int32_t>(ctx.r3.s32, -1, ctx.xer);
	// beq cr6,0x825da304
	if (ctx.cr6.eq) goto loc_825DA304;
	// lis r11,-32033
	ctx.r11.s64 = -2099314688;
	// rlwinm r10,r3,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r11,r11,464
	ctx.r11.s64 = ctx.r11.s64 + 464;
	// lwzx r10,r10,r11
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lbz r9,8(r10)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r10.u32 + 8);
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// beq cr6,0x825da290
	if (ctx.cr6.eq) goto loc_825DA290;
	// lfs f13,64(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 64);
	ctx.f13.f64 = double(temp.f32);
	// b 0x825da2a4
	goto loc_825DA2A4;
loc_825DA290:
	// lis r10,-32086
	ctx.r10.s64 = -2102788096;
	// lwz r10,2664(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 2664);
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lfs f13,64(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 64);
	ctx.f13.f64 = double(temp.f32);
loc_825DA2A4:
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r10,r1,80
	ctx.r10.s64 = ctx.r1.s64 + 80;
	// lfs f0,9508(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 9508);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32244
	ctx.r11.s64 = -2113142784;
	// fmuls f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
	// lfs f13,-4616(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -4616);
	ctx.f13.f64 = double(temp.f32);
	// lis r11,-32244
	ctx.r11.s64 = -2113142784;
	// lfs f12,-4620(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -4620);
	ctx.f12.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// fsel f13,f0,f13,f12
	ctx.f13.f64 = ctx.f0.f64 >= 0.0 ? ctx.f13.f64 : ctx.f12.f64;
	// fsubs f12,f0,f13
	ctx.f12.f64 = double(float(ctx.f0.f64 - ctx.f13.f64));
	// fadds f13,f12,f13
	ctx.f13.f64 = double(float(ctx.f12.f64 + ctx.f13.f64));
	// fsubs f12,f0,f13
	ctx.f12.f64 = double(float(ctx.f0.f64 - ctx.f13.f64));
	// lfs f0,3400(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3400);
	ctx.f0.f64 = double(temp.f32);
	// fsubs f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 - ctx.f0.f64));
	// fsel f0,f12,f13,f0
	ctx.f0.f64 = ctx.f12.f64 >= 0.0 ? ctx.f13.f64 : ctx.f0.f64;
	// fctiwz f0,f0
	ctx.f0.s64 = std::isnan(ctx.f0.f64) ? int64_t(0x80000000U) : (ctx.f0.f64 > double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&ctx.f0.f64));
	// stfiwx f0,0,r10
	PPC_STORE_U32(ctx.r10.u32, ctx.f0.u32);
	// lwz r3,80(r1)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
loc_825DA304:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA320);
PPC_WEAK_FUNC(sub_825DA320) { __imp__sub_825DA320(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA320) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
	// mr r31,r4
	ctx.r31.u64 = ctx.r4.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,4
	ctx.r6.s64 = 4;
	// li r4,1
	ctx.r4.s64 = 1;
	// li r3,1
	ctx.r3.s64 = 1;
	// bl 0x82339fd8
	ctx.lr = 0x825DA350;
	sub_82339FD8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r3,0
	ctx.r3.s64 = 0;
	// stw r4,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r4.u32);
	// lfs f1,3400(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3400);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x82334990
	ctx.lr = 0x825DA368;
	sub_82334990(ctx, base);
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x82334830
	ctx.lr = 0x825DA370;
	sub_82334830(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825da384
	if (!ctx.cr6.eq) goto loc_825DA384;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// b 0x825da3ac
	goto loc_825DA3AC;
loc_825DA384:
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// lwz r30,8272(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8272);
	// bl 0x821fe708
	ctx.lr = 0x825DA390;
	sub_821FE708(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// li r5,18
	ctx.r5.s64 = 18;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r8,0
	ctx.r8.s64 = 0;
	// bl 0x822d6358
	ctx.lr = 0x825DA3AC;
	sub_822D6358(ctx, base);
loc_825DA3AC:
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA3C8);
PPC_WEAK_FUNC(sub_825DA3C8) { __imp__sub_825DA3C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA3C8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// srawi r10,r5,8
	ctx.xer.ca = (ctx.r5.s32 < 0) & ((ctx.r5.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r5.s32 >> 8;
	// clrlwi r8,r5,24
	ctx.r8.u64 = ctx.r5.u32 & 0xFF;
	// lwz r11,21384(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 21384);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825da418
	if (!ctx.cr6.eq) goto loc_825DA418;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r31,r11,r9
	ctx.r31.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825da41c
	goto loc_825DA41C;
loc_825DA418:
	// li r31,0
	ctx.r31.s64 = 0;
loc_825DA41C:
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,4
	ctx.r6.s64 = 4;
	// li r4,2
	ctx.r4.s64 = 2;
	// li r3,1
	ctx.r3.s64 = 1;
	// bl 0x82339fd8
	ctx.lr = 0x825DA430;
	sub_82339FD8(ctx, base);
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// stw r3,0(r30)
	PPC_STORE_U32(ctx.r30.u32 + 0, ctx.r3.u32);
	// beq cr6,0x825da448
	if (ctx.cr6.eq) goto loc_825DA448;
	// lwz r11,568(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 568);
	// oris r11,r11,8192
	ctx.r11.u64 = ctx.r11.u64 | 536870912;
	// stw r11,568(r31)
	PPC_STORE_U32(ctx.r31.u32 + 568, ctx.r11.u32);
loc_825DA448:
	// lwz r3,0(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// bl 0x82334830
	ctx.lr = 0x825DA450;
	sub_82334830(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825da464
	if (!ctx.cr6.eq) goto loc_825DA464;
	// stw r11,0(r30)
	PPC_STORE_U32(ctx.r30.u32 + 0, ctx.r11.u32);
	// b 0x825da48c
	goto loc_825DA48C;
loc_825DA464:
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// lwz r31,8272(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8272);
	// bl 0x821fe708
	ctx.lr = 0x825DA470;
	sub_821FE708(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// lwz r4,0(r30)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// li r5,18
	ctx.r5.s64 = 18;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r8,0
	ctx.r8.s64 = 0;
	// bl 0x822d6358
	ctx.lr = 0x825DA48C;
	sub_822D6358(ctx, base);
loc_825DA48C:
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA4A8);
PPC_WEAK_FUNC(sub_825DA4A8) { __imp__sub_825DA4A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA4A8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
	// mr r31,r4
	ctx.r31.u64 = ctx.r4.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,4
	ctx.r6.s64 = 4;
	// li r4,3
	ctx.r4.s64 = 3;
	// li r3,1
	ctx.r3.s64 = 1;
	// bl 0x82339fd8
	ctx.lr = 0x825DA4D8;
	sub_82339FD8(ctx, base);
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// bl 0x82334830
	ctx.lr = 0x825DA4E0;
	sub_82334830(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825da4f4
	if (!ctx.cr6.eq) goto loc_825DA4F4;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// b 0x825da51c
	goto loc_825DA51C;
loc_825DA4F4:
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// lwz r30,8272(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8272);
	// bl 0x821fe708
	ctx.lr = 0x825DA500;
	sub_821FE708(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// li r5,18
	ctx.r5.s64 = 18;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r8,0
	ctx.r8.s64 = 0;
	// bl 0x822d6358
	ctx.lr = 0x825DA51C;
	sub_822D6358(ctx, base);
loc_825DA51C:
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA538);
PPC_WEAK_FUNC(sub_825DA538) { __imp__sub_825DA538(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA538) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// std r4,160(r1)
	PPC_STORE_U64(ctx.r1.u32 + 160, ctx.r4.u64);
	// std r5,168(r1)
	PPC_STORE_U64(ctx.r1.u32 + 168, ctx.r5.u64);
	// bl 0x82333000
	ctx.lr = 0x825DA558;
	sub_82333000(ctx, base);
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// blt cr6,0x825da620
	if (ctx.cr6.lt) goto loc_825DA620;
	// lis r11,-32033
	ctx.r11.s64 = -2099314688;
	// rlwinm r10,r3,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r11,r11,464
	ctx.r11.s64 = ctx.r11.s64 + 464;
	// lwzx r9,r10,r11
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lis r10,-32086
	ctx.r10.s64 = -2102788096;
	// lbz r7,8(r9)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r9.u32 + 8);
	// lwz r10,2664(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 2664);
	// cmplwi cr6,r7,0
	ctx.cr6.compare<uint32_t>(ctx.r7.u32, 0, ctx.xer);
	// beq cr6,0x825da58c
	if (ctx.cr6.eq) goto loc_825DA58C;
	// lwz r8,72(r9)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 72);
	// b 0x825da598
	goto loc_825DA598;
loc_825DA58C:
	// rlwinm r8,r10,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r8,r8,r11
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r8.u32 + ctx.r11.u32);
	// lwz r8,72(r8)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r8.u32 + 72);
loc_825DA598:
	// cmpwi cr6,r8,4
	ctx.cr6.compare<int32_t>(ctx.r8.s32, 4, ctx.xer);
	// beq cr6,0x825da5e8
	if (ctx.cr6.eq) goto loc_825DA5E8;
	// cmplwi cr6,r7,0
	ctx.cr6.compare<uint32_t>(ctx.r7.u32, 0, ctx.xer);
	// beq cr6,0x825da5b0
	if (ctx.cr6.eq) goto loc_825DA5B0;
	// lwz r8,72(r9)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r9.u32 + 72);
	// b 0x825da5bc
	goto loc_825DA5BC;
loc_825DA5B0:
	// rlwinm r8,r10,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r8,r8,r11
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r8.u32 + ctx.r11.u32);
	// lwz r8,72(r8)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r8.u32 + 72);
loc_825DA5BC:
	// cmpwi cr6,r8,5
	ctx.cr6.compare<int32_t>(ctx.r8.s32, 5, ctx.xer);
	// beq cr6,0x825da5e8
	if (ctx.cr6.eq) goto loc_825DA5E8;
	// cmplwi cr6,r7,0
	ctx.cr6.compare<uint32_t>(ctx.r7.u32, 0, ctx.xer);
	// beq cr6,0x825da5d4
	if (ctx.cr6.eq) goto loc_825DA5D4;
	// lwz r11,72(r9)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r9.u32 + 72);
	// b 0x825da5e0
	goto loc_825DA5E0;
loc_825DA5D4:
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lwz r11,72(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 72);
loc_825DA5E0:
	// cmpwi cr6,r11,7
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 7, ctx.xer);
	// bne cr6,0x825da620
	if (!ctx.cr6.eq) goto loc_825DA620;
loc_825DA5E8:
	// lfs f0,160(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 160);
	ctx.f0.f64 = double(temp.f32);
	// addi r11,r1,96
	ctx.r11.s64 = ctx.r1.s64 + 96;
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// addi r5,r1,96
	ctx.r5.s64 = ctx.r1.s64 + 96;
	// lfs f0,164(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 164);
	ctx.f0.f64 = double(temp.f32);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// stfs f0,84(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// li r3,2
	ctx.r3.s64 = 2;
	// lfs f0,168(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 168);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// addi r10,r1,80
	ctx.r10.s64 = ctx.r1.s64 + 80;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x82334a08
	ctx.lr = 0x825DA620;
	sub_82334A08(ctx, base);
loc_825DA620:
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA638);
PPC_WEAK_FUNC(sub_825DA638) { __imp__sub_825DA638(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA638) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c8
	ctx.lr = 0x825DA640;
	__savegprlr_28(ctx, base);
	// stfd f31,-48(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -48, ctx.f31.u64);
	// stwu r1,-176(r1)
	ea = -176 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// lis r10,-1
	ctx.r10.s64 = -65536;
	// clrlwi r9,r5,24
	ctx.r9.u64 = ctx.r5.u32 & 0xFF;
	// rlwimi r10,r11,8,16,23
	ctx.r10.u64 = (__builtin_rotateleft32(ctx.r11.u32, 8) & 0xFF00) | (ctx.r10.u64 & 0xFFFFFFFFFFFF00FF);
	// clrlwi r11,r4,24
	ctx.r11.u64 = ctx.r4.u32 & 0xFF;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// or r11,r10,r11
	ctx.r11.u64 = ctx.r10.u64 | ctx.r11.u64;
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// rlwimi r9,r11,8,0,23
	ctx.r9.u64 = (__builtin_rotateleft32(ctx.r11.u32, 8) & 0xFFFFFF00) | (ctx.r9.u64 & 0xFFFFFFFF000000FF);
	// lis r11,-32044
	ctx.r11.s64 = -2100035584;
	// mr r30,r6
	ctx.r30.u64 = ctx.r6.u64;
	// mr r29,r7
	ctx.r29.u64 = ctx.r7.u64;
	// mr r28,r8
	ctx.r28.u64 = ctx.r8.u64;
	// stw r9,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r9.u32);
	// lwz r31,30160(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 30160);
	// lfs f31,632(r31)
	temp.u32 = PPC_LOAD_U32(ctx.r31.u32 + 632);
	ctx.f31.f64 = double(temp.f32);
	// bl 0x8286a570
	ctx.lr = 0x825DA68C;
	sub_8286A570(ctx, base);
	// addi r5,r1,112
	ctx.r5.s64 = ctx.r1.s64 + 112;
	// fmr f1,f31
	ctx.fpscr.disableFlushMode();
	ctx.f1.f64 = ctx.f31.f64;
	// addi r4,r1,96
	ctx.r4.s64 = ctx.r1.s64 + 96;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x822cf580
	ctx.lr = 0x825DA6A0;
	sub_822CF580(ctx, base);
	// addi r4,r1,112
	ctx.r4.s64 = ctx.r1.s64 + 112;
	// addi r3,r1,84
	ctx.r3.s64 = ctx.r1.s64 + 84;
	// bl 0x8286a3b0
	ctx.lr = 0x825DA6AC;
	sub_8286A3B0(ctx, base);
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// rlwinm r10,r11,16,24,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 16) & 0xFF;
	// rlwinm r9,r11,24,24,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 24) & 0xFF;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r10,0(r30)
	PPC_STORE_U32(ctx.r30.u32 + 0, ctx.r10.u32);
	// stw r9,0(r29)
	PPC_STORE_U32(ctx.r29.u32 + 0, ctx.r9.u32);
	// stw r11,0(r28)
	PPC_STORE_U32(ctx.r28.u32 + 0, ctx.r11.u32);
	// addi r1,r1,176
	ctx.r1.s64 = ctx.r1.s64 + 176;
	// lfd f31,-48(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -48);
	// b 0x829ff818
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA6D8);
PPC_WEAK_FUNC(sub_825DA6D8) { __imp__sub_825DA6D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA6D8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// addi r3,r11,-1
	ctx.r3.s64 = ctx.r11.s64 + -1;
	// b 0x822222e8
	sub_822222E8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA6E8);
PPC_WEAK_FUNC(sub_825DA6E8) { __imp__sub_825DA6E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA6E8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// addi r3,r11,-1640
	ctx.r3.s64 = ctx.r11.s64 + -1640;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x8221f580
	ctx.lr = 0x825DA710;
	sub_8221F580(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA730);
PPC_WEAK_FUNC(sub_825DA730) { __imp__sub_825DA730(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA730) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r10
	ctx.r11.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82223420
	sub_82223420(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA750);
PPC_WEAK_FUNC(sub_825DA750) { __imp__sub_825DA750(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA750) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// addi r3,r11,-1640
	ctx.r3.s64 = ctx.r11.s64 + -1640;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x8221f5e0
	ctx.lr = 0x825DA778;
	sub_8221F5E0(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA798);
PPC_WEAK_FUNC(sub_825DA798) { __imp__sub_825DA798(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA798) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32076
	ctx.r10.s64 = -2102132736;
	// lis r11,-32076
	ctx.r11.s64 = -2102132736;
	// addi r10,r10,-25960
	ctx.r10.s64 = ctx.r10.s64 + -25960;
	// addi r11,r11,-26224
	ctx.r11.s64 = ctx.r11.s64 + -26224;
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// li r31,0
	ctx.r31.s64 = 0;
	// lwz r10,196(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 196);
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r3,r10,r11
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// bl 0x821c2fa8
	ctx.lr = 0x825DA7D4;
	sub_821C2FA8(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825da7e4
	if (ctx.cr6.eq) goto loc_825DA7E4;
	// li r31,1
	ctx.r31.s64 = 1;
loc_825DA7E4:
	// lwz r11,0(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// clrlwi r10,r31,24
	ctx.r10.u64 = ctx.r31.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA808);
PPC_WEAK_FUNC(sub_825DA808) { __imp__sub_825DA808(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA808) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// addi r11,r11,-24840
	ctx.r11.s64 = ctx.r11.s64 + -24840;
	// lwz r11,68(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 68);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA830);
PPC_WEAK_FUNC(sub_825DA830) { __imp__sub_825DA830(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA830) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// addi r11,r11,-24840
	ctx.r11.s64 = ctx.r11.s64 + -24840;
	// lwz r11,72(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 72);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA858);
PPC_WEAK_FUNC(sub_825DA858) { __imp__sub_825DA858(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA858) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,-16937(r10)
	PPC_STORE_U8(ctx.r10.u32 + -16937, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA888);
PPC_WEAK_FUNC(sub_825DA888) { __imp__sub_825DA888(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA888) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825da8b0
	if (!ctx.cr6.eq) goto loc_825DA8B0;
	// li r11,0
	ctx.r11.s64 = 0;
loc_825DA8B0:
	// stb r11,-16938(r10)
	PPC_STORE_U8(ctx.r10.u32 + -16938, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA8B8);
PPC_WEAK_FUNC(sub_825DA8B8) { __imp__sub_825DA8B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA8B8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825da8e0
	if (!ctx.cr6.eq) goto loc_825DA8E0;
	// li r11,0
	ctx.r11.s64 = 0;
loc_825DA8E0:
	// stb r11,7591(r10)
	PPC_STORE_U8(ctx.r10.u32 + 7591, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA8E8);
PPC_WEAK_FUNC(sub_825DA8E8) { __imp__sub_825DA8E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA8E8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r31,r11,1
	ctx.r31.u64 = ctx.r11.u64 ^ 1;
	// bl 0x821c3148
	ctx.lr = 0x825DA910;
	sub_821C3148(ctx, base);
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,2688
	ctx.r11.s64 = ctx.r11.s64 + 2688;
	// stb r31,41(r11)
	PPC_STORE_U8(ctx.r11.u32 + 41, ctx.r31.u8);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA930);
PPC_WEAK_FUNC(sub_825DA930) { __imp__sub_825DA930(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA930) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r31,r11,1
	ctx.r31.u64 = ctx.r11.u64 ^ 1;
	// bl 0x821c3148
	ctx.lr = 0x825DA958;
	sub_821C3148(ctx, base);
	// lis r10,-32065
	ctx.r10.s64 = -2101411840;
	// stb r31,6256(r10)
	PPC_STORE_U8(ctx.r10.u32 + 6256, ctx.r31.u8);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA978);
PPC_WEAK_FUNC(sub_825DA978) { __imp__sub_825DA978(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA978) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,8
	ctx.r3.s64 = 8;
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r10
	ctx.r11.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r5,r11,24
	ctx.r5.u64 = ctx.r11.u32 & 0xFF;
	// b 0x8233a180
	sub_8233A180(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA9A0);
PPC_WEAK_FUNC(sub_825DA9A0) { __imp__sub_825DA9A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA9A0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r8,-32070
	ctx.r8.s64 = -2101739520;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r10,r11,1
	ctx.r10.u64 = ctx.r11.u64 ^ 1;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// cntlzw r10,r10
	ctx.r10.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// rlwinm r9,r10,27,31,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// xori r9,r9,1
	ctx.r9.u64 = ctx.r9.u64 ^ 1;
	// stb r9,-16914(r8)
	PPC_STORE_U8(ctx.r8.u32 + -16914, ctx.r9.u8);
	// lis r9,-32070
	ctx.r9.s64 = -2101739520;
	// sth r10,-10360(r9)
	PPC_STORE_U16(ctx.r9.u32 + -10360, ctx.r10.u16);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// sth r11,-10364(r10)
	PPC_STORE_U16(ctx.r10.u32 + -10364, ctx.r11.u16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DA9F0);
PPC_WEAK_FUNC(sub_825DA9F0) { __imp__sub_825DA9F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DA9F0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,-16940(r10)
	PPC_STORE_U8(ctx.r10.u32 + -16940, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DAA10);
PPC_WEAK_FUNC(sub_825DAA10) { __imp__sub_825DAA10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DAA10) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lbz r11,-24252(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + -24252);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825daa34
	if (!ctx.cr6.eq) goto loc_825DAA34;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stw r11,-16920(r10)
	PPC_STORE_U32(ctx.r10.u32 + -16920, ctx.r11.u32);
	// blr 
	return;
loc_825DAA34:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,-16920(r10)
	PPC_STORE_U32(ctx.r10.u32 + -16920, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DAA40);
PPC_WEAK_FUNC(sub_825DAA40) { __imp__sub_825DAA40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DAA40) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// lwz r11,-24260(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -24260);
	// cmpwi cr6,r11,2
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 2, ctx.xer);
	// beq cr6,0x825daa70
	if (ctx.cr6.eq) goto loc_825DAA70;
	// cmpwi cr6,r11,6
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 6, ctx.xer);
	// beq cr6,0x825daa70
	if (ctx.cr6.eq) goto loc_825DAA70;
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lbz r11,-24252(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + -24252);
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
loc_825DAA70:
	// li r11,0
	ctx.r11.s64 = 0;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DAA88);
PPC_WEAK_FUNC(sub_825DAA88) { __imp__sub_825DAA88(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DAA88) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r9,-32070
	ctx.r9.s64 = -2101739520;
	// addi r9,r9,32256
	ctx.r9.s64 = ctx.r9.s64 + 32256;
	// lwz r10,-536(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -536);
	// addi r8,r9,4
	ctx.r8.s64 = ctx.r9.s64 + 4;
	// addi r9,r9,8
	ctx.r9.s64 = ctx.r9.s64 + 8;
	// lfs f0,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// lfs f13,0(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// stfsx f13,r10,r8
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r10.u32 + ctx.r8.u32, temp.u32);
	// stfsx f0,r10,r9
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + ctx.r9.u32, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DAAC0);
PPC_WEAK_FUNC(sub_825DAAC0) { __imp__sub_825DAAC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DAAC0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r9,-32070
	ctx.r9.s64 = -2101739520;
	// addi r9,r9,32256
	ctx.r9.s64 = ctx.r9.s64 + 32256;
	// lwz r10,-536(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -536);
	// addi r8,r9,16
	ctx.r8.s64 = ctx.r9.s64 + 16;
	// addi r7,r9,17
	ctx.r7.s64 = ctx.r9.s64 + 17;
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// stbx r11,r10,r8
	PPC_STORE_U8(ctx.r10.u32 + ctx.r8.u32, ctx.r11.u8);
	// stbx r9,r10,r7
	PPC_STORE_U8(ctx.r10.u32 + ctx.r7.u32, ctx.r9.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DAAF8);
PPC_WEAK_FUNC(sub_825DAAF8) { __imp__sub_825DAAF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DAAF8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// li r9,1
	ctx.r9.s64 = 1;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r10,-536(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -536);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r11,r11,32256
	ctx.r11.s64 = ctx.r11.s64 + 32256;
	// addi r11,r11,24
	ctx.r11.s64 = ctx.r11.s64 + 24;
	// bne cr6,0x825dab38
	if (!ctx.cr6.eq) goto loc_825DAB38;
	// li r9,0
	ctx.r9.s64 = 0;
loc_825DAB38:
	// stbx r9,r10,r11
	PPC_STORE_U8(ctx.r10.u32 + ctx.r11.u32, ctx.r9.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DAB40);
PPC_WEAK_FUNC(sub_825DAB40) { __imp__sub_825DAB40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DAB40) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// li r9,1
	ctx.r9.s64 = 1;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r10,-536(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -536);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r11,r11,32256
	ctx.r11.s64 = ctx.r11.s64 + 32256;
	// addi r11,r11,26
	ctx.r11.s64 = ctx.r11.s64 + 26;
	// bne cr6,0x825dab80
	if (!ctx.cr6.eq) goto loc_825DAB80;
	// li r9,0
	ctx.r9.s64 = 0;
loc_825DAB80:
	// stbx r9,r10,r11
	PPC_STORE_U8(ctx.r10.u32 + ctx.r11.u32, ctx.r9.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DAB88);
PPC_WEAK_FUNC(sub_825DAB88) { __imp__sub_825DAB88(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DAB88) {
	PPC_FUNC_PROLOGUE();
	// lwz r9,8(r3)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r11,r11,32256
	ctx.r11.s64 = ctx.r11.s64 + 32256;
	// lwz r9,0(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
	// addi r8,r11,44
	ctx.r8.s64 = ctx.r11.s64 + 44;
	// lwz r10,-536(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -536);
	// cntlzw r9,r9
	ctx.r9.u64 = ctx.r9.u32 == 0 ? 32 : __builtin_clz(ctx.r9.u32);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// rlwinm r9,r9,27,31,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// xori r11,r9,1
	ctx.r11.u64 = ctx.r9.u64 ^ 1;
	// stbx r11,r10,r8
	PPC_STORE_U8(ctx.r10.u32 + ctx.r8.u32, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DABC0);
PPC_WEAK_FUNC(sub_825DABC0) { __imp__sub_825DABC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DABC0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r9,8(r3)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r11,r11,32256
	ctx.r11.s64 = ctx.r11.s64 + 32256;
	// lwz r10,-536(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -536);
	// addi r11,r11,40
	ctx.r11.s64 = ctx.r11.s64 + 40;
	// lfs f0,0(r9)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// stfsx f0,r10,r11
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + ctx.r11.u32, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DABE8);
PPC_WEAK_FUNC(sub_825DABE8) { __imp__sub_825DABE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DABE8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r9,-32070
	ctx.r9.s64 = -2101739520;
	// addi r9,r9,32256
	ctx.r9.s64 = ctx.r9.s64 + 32256;
	// lwz r10,-536(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -536);
	// addi r8,r9,32
	ctx.r8.s64 = ctx.r9.s64 + 32;
	// addi r9,r9,36
	ctx.r9.s64 = ctx.r9.s64 + 36;
	// lfs f0,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// lfs f13,0(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// stfsx f13,r10,r8
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r10.u32 + ctx.r8.u32, temp.u32);
	// stfsx f0,r10,r9
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + ctx.r9.u32, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DAC20);
PPC_WEAK_FUNC(sub_825DAC20) { __imp__sub_825DAC20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DAC20) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r9,8(r3)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r11,r11,32256
	ctx.r11.s64 = ctx.r11.s64 + 32256;
	// lwz r10,-536(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -536);
	// addi r11,r11,36
	ctx.r11.s64 = ctx.r11.s64 + 36;
	// lfs f0,0(r9)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// stfsx f0,r10,r11
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + ctx.r11.u32, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DAC48);
PPC_WEAK_FUNC(sub_825DAC48) { __imp__sub_825DAC48(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DAC48) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// li r9,1
	ctx.r9.s64 = 1;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r10,-536(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -536);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r11,r11,32256
	ctx.r11.s64 = ctx.r11.s64 + 32256;
	// addi r11,r11,29
	ctx.r11.s64 = ctx.r11.s64 + 29;
	// bne cr6,0x825dac88
	if (!ctx.cr6.eq) goto loc_825DAC88;
	// li r9,0
	ctx.r9.s64 = 0;
loc_825DAC88:
	// stbx r9,r10,r11
	PPC_STORE_U8(ctx.r10.u32 + ctx.r11.u32, ctx.r9.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DAC90);
PPC_WEAK_FUNC(sub_825DAC90) { __imp__sub_825DAC90(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DAC90) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// li r9,1
	ctx.r9.s64 = 1;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r10,-536(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -536);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r11,r11,32256
	ctx.r11.s64 = ctx.r11.s64 + 32256;
	// addi r11,r11,28
	ctx.r11.s64 = ctx.r11.s64 + 28;
	// bne cr6,0x825dacd0
	if (!ctx.cr6.eq) goto loc_825DACD0;
	// li r9,0
	ctx.r9.s64 = 0;
loc_825DACD0:
	// stbx r9,r10,r11
	PPC_STORE_U8(ctx.r10.u32 + ctx.r11.u32, ctx.r9.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DACD8);
PPC_WEAK_FUNC(sub_825DACD8) { __imp__sub_825DACD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DACD8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// li r9,1
	ctx.r9.s64 = 1;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r10,-536(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -536);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r11,r11,32256
	ctx.r11.s64 = ctx.r11.s64 + 32256;
	// addi r11,r11,27
	ctx.r11.s64 = ctx.r11.s64 + 27;
	// bne cr6,0x825dad18
	if (!ctx.cr6.eq) goto loc_825DAD18;
	// li r9,0
	ctx.r9.s64 = 0;
loc_825DAD18:
	// stbx r9,r10,r11
	PPC_STORE_U8(ctx.r10.u32 + ctx.r11.u32, ctx.r9.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DAD20);
PPC_WEAK_FUNC(sub_825DAD20) { __imp__sub_825DAD20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DAD20) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x821f50b0
	sub_821F50B0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DAD30);
PPC_WEAK_FUNC(sub_825DAD30) { __imp__sub_825DAD30(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DAD30) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x821f1450
	ctx.lr = 0x825DAD50;
	sub_821F1450(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DAD70);
PPC_WEAK_FUNC(sub_825DAD70) { __imp__sub_825DAD70(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DAD70) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x821f1490
	ctx.lr = 0x825DAD90;
	sub_821F1490(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DADB0);
PPC_WEAK_FUNC(sub_825DADB0) { __imp__sub_825DADB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DADB0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r9,8(r3)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r11,r11,32256
	ctx.r11.s64 = ctx.r11.s64 + 32256;
	// lwz r10,-536(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -536);
	// addi r11,r11,60
	ctx.r11.s64 = ctx.r11.s64 + 60;
	// lwz r9,0(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// stwx r9,r10,r11
	PPC_STORE_U32(ctx.r10.u32 + ctx.r11.u32, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DADD8);
PPC_WEAK_FUNC(sub_825DADD8) { __imp__sub_825DADD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DADD8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,7580(r10)
	PPC_STORE_U8(ctx.r10.u32 + 7580, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DADF8);
PPC_WEAK_FUNC(sub_825DADF8) { __imp__sub_825DADF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DADF8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// li r9,1
	ctx.r9.s64 = 1;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r10,-536(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -536);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r11,r11,32256
	ctx.r11.s64 = ctx.r11.s64 + 32256;
	// addi r11,r11,25
	ctx.r11.s64 = ctx.r11.s64 + 25;
	// bne cr6,0x825dae38
	if (!ctx.cr6.eq) goto loc_825DAE38;
	// li r9,0
	ctx.r9.s64 = 0;
loc_825DAE38:
	// stbx r9,r10,r11
	PPC_STORE_U8(ctx.r10.u32 + ctx.r11.u32, ctx.r9.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DAE40);
PPC_WEAK_FUNC(sub_825DAE40) { __imp__sub_825DAE40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DAE40) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825da1d8
	sub_825DA1D8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DAE50);
PPC_WEAK_FUNC(sub_825DAE50) { __imp__sub_825DAE50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DAE50) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,-8828
	ctx.r3.s64 = ctx.r11.s64 + -8828;
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// stw r4,-540(r11)
	PPC_STORE_U32(ctx.r11.u32 + -540, ctx.r4.u32);
	// bl 0x8235c6c0
	ctx.lr = 0x825DAE78;
	sub_8235C6C0(ctx, base);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,7576(r10)
	PPC_STORE_U32(ctx.r10.u32 + 7576, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DAE98);
PPC_WEAK_FUNC(sub_825DAE98) { __imp__sub_825DAE98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DAE98) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r11,19272
	ctx.r3.s64 = ctx.r11.s64 + 19272;
	// bl 0x82222420
	ctx.lr = 0x825DAEB8;
	sub_82222420(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DAED8);
PPC_WEAK_FUNC(sub_825DAED8) { __imp__sub_825DAED8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DAED8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// addi r3,r11,19272
	ctx.r3.s64 = ctx.r11.s64 + 19272;
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// extsw r11,r11
	ctx.r11.s64 = ctx.r11.s32;
	// std r11,80(r1)
	PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r11.u64);
	// lfd f0,80(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f0.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
	// fcfid f0,f0
	ctx.f0.f64 = double(ctx.f0.s64);
	// frsp f1,f0
	ctx.f1.f64 = double(float(ctx.f0.f64));
	// bl 0x82222458
	ctx.lr = 0x825DAF0C;
	sub_82222458(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DAF20);
PPC_WEAK_FUNC(sub_825DAF20) { __imp__sub_825DAF20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DAF20) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x825d8c90
	sub_825D8C90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DAF40);
PPC_WEAK_FUNC(sub_825DAF40) { __imp__sub_825DAF40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DAF40) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// bl 0x821fe708
	ctx.lr = 0x825DAF58;
	sub_821FE708(ctx, base);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x825daf84
	if (ctx.cr6.eq) goto loc_825DAF84;
	// bl 0x821fe708
	ctx.lr = 0x825DAF64;
	sub_821FE708(ctx, base);
	// lbz r11,133(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 133);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825daf84
	if (ctx.cr6.eq) goto loc_825DAF84;
	// bl 0x821fe708
	ctx.lr = 0x825DAF74;
	sub_821FE708(ctx, base);
	// lbz r11,134(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 134);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// bne cr6,0x825daf88
	if (!ctx.cr6.eq) goto loc_825DAF88;
loc_825DAF84:
	// li r11,0
	ctx.r11.s64 = 0;
loc_825DAF88:
	// lwz r10,0(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DAFA8);
PPC_WEAK_FUNC(sub_825DAFA8) { __imp__sub_825DAFA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DAFA8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r31,r11,1
	ctx.r31.u64 = ctx.r11.u64 ^ 1;
	// bl 0x821fe708
	ctx.lr = 0x825DAFD0;
	sub_821FE708(ctx, base);
	// stb r31,132(r3)
	PPC_STORE_U8(ctx.r3.u32 + 132, ctx.r31.u8);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DAFE8);
PPC_WEAK_FUNC(sub_825DAFE8) { __imp__sub_825DAFE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DAFE8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x8229d200
	sub_8229D200(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB000);
PPC_WEAK_FUNC(sub_825DB000) { __imp__sub_825DB000(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB000) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x8229c630
	ctx.lr = 0x825DB020;
	sub_8229C630(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB040);
PPC_WEAK_FUNC(sub_825DB040) { __imp__sub_825DB040(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB040) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x8229c5e8
	ctx.lr = 0x825DB060;
	sub_8229C5E8(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB080);
PPC_WEAK_FUNC(sub_825DB080) { __imp__sub_825DB080(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB080) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r10,8(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r10,r10
	ctx.r10.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// rlwinm r11,r10,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r5,r11,24
	ctx.r5.u64 = ctx.r11.u32 & 0xFF;
	// b 0x8229c7b0
	sub_8229C7B0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB0A8);
PPC_WEAK_FUNC(sub_825DB0A8) { __imp__sub_825DB0A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB0A8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8229c2e0
	sub_8229C2E0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB0B8);
PPC_WEAK_FUNC(sub_825DB0B8) { __imp__sub_825DB0B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB0B8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8229d230
	sub_8229D230(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB0C8);
PPC_WEAK_FUNC(sub_825DB0C8) { __imp__sub_825DB0C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB0C8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f1,8(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f1.f64 = double(temp.f32);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x8229c2b0
	sub_8229C2B0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB0E0);
PPC_WEAK_FUNC(sub_825DB0E0) { __imp__sub_825DB0E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB0E0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r10,8(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r10,r10
	ctx.r10.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// rlwinm r11,r10,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r5,r11,24
	ctx.r5.u64 = ctx.r11.u32 & 0xFF;
	// b 0x8229c8e0
	sub_8229C8E0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB108);
PPC_WEAK_FUNC(sub_825DB108) { __imp__sub_825DB108(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB108) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r3,2664(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 2664);
	// bl 0x82332fa0
	ctx.lr = 0x825DB128;
	sub_82332FA0(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB148);
PPC_WEAK_FUNC(sub_825DB148) { __imp__sub_825DB148(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB148) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x825d9240
	ctx.lr = 0x825DB168;
	sub_825D9240(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB188);
PPC_WEAK_FUNC(sub_825DB188) { __imp__sub_825DB188(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB188) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,-11452(r10)
	PPC_STORE_U32(ctx.r10.u32 + -11452, ctx.r11.u32);
	// bl 0x825d9240
	ctx.lr = 0x825DB1B4;
	sub_825D9240(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB1D0);
PPC_WEAK_FUNC(sub_825DB1D0) { __imp__sub_825DB1D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB1D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x825d93b0
	ctx.lr = 0x825DB1F0;
	sub_825D93B0(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB210);
PPC_WEAK_FUNC(sub_825DB210) { __imp__sub_825DB210(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB210) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x825d9430
	ctx.lr = 0x825DB230;
	sub_825D9430(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB250);
PPC_WEAK_FUNC(sub_825DB250) { __imp__sub_825DB250(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB250) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x82333000
	ctx.lr = 0x825DB270;
	sub_82333000(ctx, base);
	// cmpwi cr6,r3,-1
	ctx.cr6.compare<int32_t>(ctx.r3.s32, -1, ctx.xer);
	// beq cr6,0x825db29c
	if (ctx.cr6.eq) goto loc_825DB29C;
	// lis r11,-32033
	ctx.r11.s64 = -2099314688;
	// rlwinm r10,r3,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r11,r11,464
	ctx.r11.s64 = ctx.r11.s64 + 464;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lbz r10,8(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 8);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x825db29c
	if (ctx.cr6.eq) goto loc_825DB29C;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// b 0x825db2a0
	goto loc_825DB2A0;
loc_825DB29C:
	// li r11,-1
	ctx.r11.s64 = -1;
loc_825DB2A0:
	// lwz r10,0(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB2C0);
PPC_WEAK_FUNC(sub_825DB2C0) { __imp__sub_825DB2C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB2C0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d95b0
	sub_825D95B0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB2D0);
PPC_WEAK_FUNC(sub_825DB2D0) { __imp__sub_825DB2D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB2D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x825d9860
	ctx.lr = 0x825DB2F0;
	sub_825D9860(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB310);
PPC_WEAK_FUNC(sub_825DB310) { __imp__sub_825DB310(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB310) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,6
	ctx.r3.s64 = 6;
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8233a180
	sub_8233A180(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB328);
PPC_WEAK_FUNC(sub_825DB328) { __imp__sub_825DB328(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB328) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,5
	ctx.r3.s64 = 5;
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8233a180
	sub_8233A180(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB340);
PPC_WEAK_FUNC(sub_825DB340) { __imp__sub_825DB340(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB340) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x825d9a90
	ctx.lr = 0x825DB360;
	sub_825D9A90(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB380);
PPC_WEAK_FUNC(sub_825DB380) { __imp__sub_825DB380(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB380) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,14
	ctx.r3.s64 = 14;
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r10
	ctx.r11.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r5,r11,24
	ctx.r5.u64 = ctx.r11.u32 & 0xFF;
	// b 0x8233a180
	sub_8233A180(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB3A8);
PPC_WEAK_FUNC(sub_825DB3A8) { __imp__sub_825DB3A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB3A8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,11
	ctx.r3.s64 = 11;
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r10
	ctx.r11.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r5,r11,24
	ctx.r5.u64 = ctx.r11.u32 & 0xFF;
	// b 0x8233a180
	sub_8233A180(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB3D0);
PPC_WEAK_FUNC(sub_825DB3D0) { __imp__sub_825DB3D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB3D0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,12
	ctx.r3.s64 = 12;
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r10
	ctx.r11.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r5,r11,24
	ctx.r5.u64 = ctx.r11.u32 & 0xFF;
	// b 0x8233a180
	sub_8233A180(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB3F8);
PPC_WEAK_FUNC(sub_825DB3F8) { __imp__sub_825DB3F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB3F8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,13
	ctx.r3.s64 = 13;
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r10
	ctx.r11.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r5,r11,24
	ctx.r5.u64 = ctx.r11.u32 & 0xFF;
	// b 0x8233a180
	sub_8233A180(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB420);
PPC_WEAK_FUNC(sub_825DB420) { __imp__sub_825DB420(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB420) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stfd f31,-24(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r31,0(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f31,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f31.f64 = double(temp.f32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82333000
	ctx.lr = 0x825DB448;
	sub_82333000(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// li r3,0
	ctx.r3.s64 = 0;
	// fmr f1,f31
	ctx.fpscr.disableFlushMode();
	ctx.f1.f64 = ctx.f31.f64;
	// bl 0x82334990
	ctx.lr = 0x825DB458;
	sub_82334990(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f31,-24(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB470);
PPC_WEAK_FUNC(sub_825DB470) { __imp__sub_825DB470(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB470) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmpwi cr6,r5,3
	ctx.cr6.compare<int32_t>(ctx.r5.s32, 3, ctx.xer);
	// bne cr6,0x825db488
	if (!ctx.cr6.eq) goto loc_825DB488;
	// li r5,2
	ctx.r5.s64 = 2;
loc_825DB488:
	// li r3,15
	ctx.r3.s64 = 15;
	// b 0x8233a180
	sub_8233A180(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB490);
PPC_WEAK_FUNC(sub_825DB490) { __imp__sub_825DB490(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB490) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,1
	ctx.r3.s64 = 1;
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8233a180
	sub_8233A180(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB4A8);
PPC_WEAK_FUNC(sub_825DB4A8) { __imp__sub_825DB4A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB4A8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,3
	ctx.r3.s64 = 3;
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8233a180
	sub_8233A180(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB4C0);
PPC_WEAK_FUNC(sub_825DB4C0) { __imp__sub_825DB4C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB4C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r5,0
	ctx.r5.s64 = 0;
	// li r3,4
	ctx.r3.s64 = 4;
	// lwz r31,0(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8233a180
	ctx.lr = 0x825DB4EC;
	sub_8233A180(ctx, base);
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// lwz r30,8272(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8272);
	// bl 0x821fe708
	ctx.lr = 0x825DB4F8;
	sub_821FE708(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// li r5,18
	ctx.r5.s64 = 18;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// bl 0x822d6b60
	ctx.lr = 0x825DB510;
	sub_822D6B60(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB528);
PPC_WEAK_FUNC(sub_825DB528) { __imp__sub_825DB528(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB528) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,7
	ctx.r3.s64 = 7;
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r10
	ctx.r11.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r5,r11,24
	ctx.r5.u64 = ctx.r11.u32 & 0xFF;
	// b 0x8233a180
	sub_8233A180(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB550);
PPC_WEAK_FUNC(sub_825DB550) { __imp__sub_825DB550(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB550) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x82334830
	ctx.lr = 0x825DB570;
	sub_82334830(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB590);
PPC_WEAK_FUNC(sub_825DB590) { __imp__sub_825DB590(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB590) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,9
	ctx.r3.s64 = 9;
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8233a180
	sub_8233A180(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB5A8);
PPC_WEAK_FUNC(sub_825DB5A8) { __imp__sub_825DB5A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB5A8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,10
	ctx.r3.s64 = 10;
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8233a180
	sub_8233A180(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB5C0);
PPC_WEAK_FUNC(sub_825DB5C0) { __imp__sub_825DB5C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB5C0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32052
	ctx.r10.s64 = -2100559872;
	// addi r10,r10,31752
	ctx.r10.s64 = ctx.r10.s64 + 31752;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,341(r10)
	PPC_STORE_U8(ctx.r10.u32 + 341, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB5E8);
PPC_WEAK_FUNC(sub_825DB5E8) { __imp__sub_825DB5E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB5E8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stw r11,7576(r10)
	PPC_STORE_U32(ctx.r10.u32 + 7576, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB600);
PPC_WEAK_FUNC(sub_825DB600) { __imp__sub_825DB600(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB600) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// stb r11,7582(r10)
	PPC_STORE_U8(ctx.r10.u32 + 7582, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB630);
PPC_WEAK_FUNC(sub_825DB630) { __imp__sub_825DB630(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB630) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,7583(r10)
	PPC_STORE_U8(ctx.r10.u32 + 7583, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB650);
PPC_WEAK_FUNC(sub_825DB650) { __imp__sub_825DB650(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB650) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,7585(r10)
	PPC_STORE_U8(ctx.r10.u32 + 7585, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB670);
PPC_WEAK_FUNC(sub_825DB670) { __imp__sub_825DB670(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB670) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// stb r11,-16915(r10)
	PPC_STORE_U8(ctx.r10.u32 + -16915, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB6A0);
PPC_WEAK_FUNC(sub_825DB6A0) { __imp__sub_825DB6A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB6A0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stw r11,-10356(r10)
	PPC_STORE_U32(ctx.r10.u32 + -10356, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB6B8);
PPC_WEAK_FUNC(sub_825DB6B8) { __imp__sub_825DB6B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB6B8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r10,r11,1
	ctx.r10.u64 = ctx.r11.u64 ^ 1;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// clrlwi r3,r10,24
	ctx.r3.u64 = ctx.r10.u32 & 0xFF;
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x825d9b80
	sub_825D9B80(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB6E0);
PPC_WEAK_FUNC(sub_825DB6E0) { __imp__sub_825DB6E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB6E0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x82236d00
	sub_82236D00(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB6F0);
PPC_WEAK_FUNC(sub_825DB6F0) { __imp__sub_825DB6F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB6F0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82236d98
	sub_82236D98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB710);
PPC_WEAK_FUNC(sub_825DB710) { __imp__sub_825DB710(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB710) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82236d80
	sub_82236D80(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB730);
PPC_WEAK_FUNC(sub_825DB730) { __imp__sub_825DB730(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB730) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r10,28(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// cntlzw r10,r10
	ctx.r10.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// lwz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lfs f3,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f3.f64 = double(temp.f32);
	// rlwinm r10,r10,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// lfs f4,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f4.f64 = double(temp.f32);
	// lfs f5,24(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	ctx.f5.f64 = double(temp.f32);
	// xori r11,r10,1
	ctx.r11.u64 = ctx.r10.u64 ^ 1;
	// clrlwi r10,r11,24
	ctx.r10.u64 = ctx.r11.u32 & 0xFF;
	// b 0x825d9d40
	sub_825D9D40(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB768);
PPC_WEAK_FUNC(sub_825DB768) { __imp__sub_825DB768(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB768) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// bl 0x821cb4f0
	ctx.lr = 0x825DB780;
	sub_821CB4F0(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB7A0);
PPC_WEAK_FUNC(sub_825DB7A0) { __imp__sub_825DB7A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB7A0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stw r11,7600(r10)
	PPC_STORE_U32(ctx.r10.u32 + 7600, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB7B8);
PPC_WEAK_FUNC(sub_825DB7B8) { __imp__sub_825DB7B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB7B8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// lfs f0,0(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,20944(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 20944, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB7D0);
PPC_WEAK_FUNC(sub_825DB7D0) { __imp__sub_825DB7D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB7D0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32067
	ctx.r10.s64 = -2101542912;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmpwi cr6,r11,3
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 3, ctx.xer);
	// bne cr6,0x825db7e8
	if (!ctx.cr6.eq) goto loc_825DB7E8;
	// li r11,2
	ctx.r11.s64 = 2;
loc_825DB7E8:
	// stw r11,17136(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17136, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB7F0);
PPC_WEAK_FUNC(sub_825DB7F0) { __imp__sub_825DB7F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB7F0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32067
	ctx.r10.s64 = -2101542912;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stw r11,17132(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17132, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB808);
PPC_WEAK_FUNC(sub_825DB808) { __imp__sub_825DB808(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB808) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32067
	ctx.r10.s64 = -2101542912;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stw r11,17140(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17140, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB820);
PPC_WEAK_FUNC(sub_825DB820) { __imp__sub_825DB820(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB820) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32067
	ctx.r10.s64 = -2101542912;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stw r11,17144(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17144, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB838);
PPC_WEAK_FUNC(sub_825DB838) { __imp__sub_825DB838(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB838) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f2,8(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f2.f64 = double(temp.f32);
	// lfs f1,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// b 0x825da010
	sub_825DA010(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB850);
PPC_WEAK_FUNC(sub_825DB850) { __imp__sub_825DB850(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB850) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-31971
	ctx.r10.s64 = -2095251456;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,21376(r10)
	PPC_STORE_U8(ctx.r10.u32 + 21376, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB870);
PPC_WEAK_FUNC(sub_825DB870) { __imp__sub_825DB870(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB870) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32065
	ctx.r10.s64 = -2101411840;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,6249(r10)
	PPC_STORE_U8(ctx.r10.u32 + 6249, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB890);
PPC_WEAK_FUNC(sub_825DB890) { __imp__sub_825DB890(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB890) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32065
	ctx.r10.s64 = -2101411840;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,6248(r10)
	PPC_STORE_U8(ctx.r10.u32 + 6248, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB8B0);
PPC_WEAK_FUNC(sub_825DB8B0) { __imp__sub_825DB8B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB8B0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,16
	ctx.r3.s64 = 16;
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// extsw r11,r10
	ctx.r11.s64 = ctx.r10.s32;
	// std r11,80(r1)
	PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r11.u64);
	// lis r11,-32244
	ctx.r11.s64 = -2113142784;
	// lfd f0,80(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f0.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
	// fcfid f0,f0
	ctx.f0.f64 = double(ctx.f0.s64);
	// frsp f13,f0
	ctx.f13.f64 = double(float(ctx.f0.f64));
	// lfs f0,-4364(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -4364);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f1,f13,f0
	ctx.f1.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
	// bl 0x82334990
	ctx.lr = 0x825DB8F0;
	sub_82334990(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB900);
PPC_WEAK_FUNC(sub_825DB900) { __imp__sub_825DB900(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB900) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// bl 0x82236b40
	ctx.lr = 0x825DB918;
	sub_82236B40(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB938);
PPC_WEAK_FUNC(sub_825DB938) { __imp__sub_825DB938(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB938) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x825da248
	ctx.lr = 0x825DB958;
	sub_825DA248(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB978);
PPC_WEAK_FUNC(sub_825DB978) { __imp__sub_825DB978(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB978) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32064
	ctx.r10.s64 = -2101346304;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stw r11,-24264(r10)
	PPC_STORE_U32(ctx.r10.u32 + -24264, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB990);
PPC_WEAK_FUNC(sub_825DB990) { __imp__sub_825DB990(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB990) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,19
	ctx.r3.s64 = 19;
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r10
	ctx.r11.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r5,r11,24
	ctx.r5.u64 = ctx.r11.u32 & 0xFF;
	// b 0x8233a180
	sub_8233A180(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB9B8);
PPC_WEAK_FUNC(sub_825DB9B8) { __imp__sub_825DB9B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB9B8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// lbz r11,-24252(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + -24252);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825db9dc
	if (!ctx.cr6.eq) goto loc_825DB9DC;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// lfs f0,0(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,-16924(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -16924, temp.u32);
	// blr 
	return;
loc_825DB9DC:
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,3400(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3400);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stfs f0,-16924(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -16924, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DB9F0);
PPC_WEAK_FUNC(sub_825DB9F0) { __imp__sub_825DB9F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DB9F0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// addi r3,r11,19272
	ctx.r3.s64 = ctx.r11.s64 + 19272;
	// lfs f1,0(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// b 0x82222458
	sub_82222458(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DBA08);
PPC_WEAK_FUNC(sub_825DBA08) { __imp__sub_825DBA08(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DBA08) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stfd f30,-32(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f30.u64);
	// stfd f31,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f31.u64);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,0(r13)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r13.u32 + 0);
	// li r10,20
	ctx.r10.s64 = 20;
	// fmr f31,f3
	ctx.f31.f64 = ctx.f3.f64;
	// mr r31,r7
	ctx.r31.u64 = ctx.r7.u64;
	// fmr f30,f4
	ctx.f30.f64 = ctx.f4.f64;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825dba84
	if (ctx.cr6.eq) goto loc_825DBA84;
	// li r4,0
	ctx.r4.s64 = 0;
	// stfs f1,80(r1)
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// li r3,28
	ctx.r3.s64 = 28;
	// stfs f2,84(r1)
	temp.f32 = float(ctx.f2.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// bl 0x821bb3d8
	ctx.lr = 0x825DBA54;
	sub_821BB3D8(ctx, base);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x825dba78
	if (ctx.cr6.eq) goto loc_825DBA78;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// fmr f2,f30
	ctx.fpscr.disableFlushMode();
	ctx.f2.f64 = ctx.f30.f64;
	// rlwinm r7,r31,24,0,7
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 24) & 0xFF000000;
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// bl 0x821bf628
	ctx.lr = 0x825DBA70;
	sub_821BF628(ctx, base);
	// bl 0x82146790
	ctx.lr = 0x825DBA74;
	sub_82146790(ctx, base);
	// b 0x825dba94
	goto loc_825DBA94;
loc_825DBA78:
	// li r3,0
	ctx.r3.s64 = 0;
	// bl 0x82146790
	ctx.lr = 0x825DBA80;
	sub_82146790(ctx, base);
	// b 0x825dba94
	goto loc_825DBA94;
loc_825DBA84:
	// rlwinm r7,r31,24,0,7
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 24) & 0xFF000000;
	// fmr f4,f30
	ctx.fpscr.disableFlushMode();
	ctx.f4.f64 = ctx.f30.f64;
	// fmr f3,f31
	ctx.f3.f64 = ctx.f31.f64;
	// bl 0x8224d1e0
	ctx.lr = 0x825DBA94;
	sub_8224D1E0(ctx, base);
loc_825DBA94:
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f30,-32(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// lfd f31,-24(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DBAB0);
PPC_WEAK_FUNC(sub_825DBAB0) { __imp__sub_825DBAB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DBAB0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,276(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 276);
	// addi r7,r1,96
	ctx.r7.s64 = ctx.r1.s64 + 96;
	// lwz r10,252(r1)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r1.u32 + 252);
	// addi r6,r1,104
	ctx.r6.s64 = ctx.r1.s64 + 104;
	// lwz r9,260(r1)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 260);
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// lwz r8,268(r1)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r1.u32 + 268);
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// clrlwi r9,r9,24
	ctx.r9.u64 = ctx.r9.u32 & 0xFF;
	// stfs f1,128(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r1.u32 + 128, temp.u32);
	// clrlwi r8,r8,24
	ctx.r8.u64 = ctx.r8.u32 & 0xFF;
	// stfs f2,132(r1)
	temp.f32 = float(ctx.f2.f64);
	PPC_STORE_U32(ctx.r1.u32 + 132, temp.u32);
	// rlwimi r10,r11,8,16,23
	ctx.r10.u64 = (__builtin_rotateleft32(ctx.r11.u32, 8) & 0xFF00) | (ctx.r10.u64 & 0xFFFFFFFFFFFF00FF);
	// stfs f3,120(r1)
	temp.f32 = float(ctx.f3.f64);
	PPC_STORE_U32(ctx.r1.u32 + 120, temp.u32);
	// rlwimi r8,r9,8,16,23
	ctx.r8.u64 = (__builtin_rotateleft32(ctx.r9.u32, 8) & 0xFF00) | (ctx.r8.u64 & 0xFFFFFFFFFFFF00FF);
	// stfs f4,124(r1)
	temp.f32 = float(ctx.f4.f64);
	PPC_STORE_U32(ctx.r1.u32 + 124, temp.u32);
	// mr r11,r10
	ctx.r11.u64 = ctx.r10.u64;
	// lwz r10,284(r1)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r1.u32 + 284);
	// mr r9,r8
	ctx.r9.u64 = ctx.r8.u64;
	// lwz r8,244(r1)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r1.u32 + 244);
	// addi r5,r1,112
	ctx.r5.s64 = ctx.r1.s64 + 112;
	// stfs f5,112(r1)
	temp.f32 = float(ctx.f5.f64);
	PPC_STORE_U32(ctx.r1.u32 + 112, temp.u32);
	// rlwimi r9,r11,16,0,15
	ctx.r9.u64 = (__builtin_rotateleft32(ctx.r11.u32, 16) & 0xFFFF0000) | (ctx.r9.u64 & 0xFFFFFFFF0000FFFF);
	// lwz r11,292(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 292);
	// addi r4,r1,120
	ctx.r4.s64 = ctx.r1.s64 + 120;
	// stfs f6,116(r1)
	temp.f32 = float(ctx.f6.f64);
	PPC_STORE_U32(ctx.r1.u32 + 116, temp.u32);
	// addi r3,r1,128
	ctx.r3.s64 = ctx.r1.s64 + 128;
	// stfs f7,104(r1)
	temp.f32 = float(ctx.f7.f64);
	PPC_STORE_U32(ctx.r1.u32 + 104, temp.u32);
	// stfs f8,108(r1)
	temp.f32 = float(ctx.f8.f64);
	PPC_STORE_U32(ctx.r1.u32 + 108, temp.u32);
	// stfs f9,96(r1)
	temp.f32 = float(ctx.f9.f64);
	PPC_STORE_U32(ctx.r1.u32 + 96, temp.u32);
	// stfs f10,100(r1)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r1.u32 + 100, temp.u32);
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x8224dd68
	ctx.lr = 0x825DBB40;
	sub_8224DD68(ctx, base);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DBB50);
PPC_WEAK_FUNC(sub_825DBB50) { __imp__sub_825DBB50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DBB50) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lis r11,-32244
	ctx.r11.s64 = -2113142784;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// addi r4,r11,-15112
	ctx.r4.s64 = ctx.r11.s64 + -15112;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r30,0(r10)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// mr r5,r30
	ctx.r5.u64 = ctx.r30.u64;
	// bl 0x82a00108
	ctx.lr = 0x825DBB84;
	sub_82A00108(ctx, base);
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r3,r11,-1640
	ctx.r3.s64 = ctx.r11.s64 + -1640;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// bl 0x8221f840
	ctx.lr = 0x825DBB98;
	sub_8221F840(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// bl 0x8221f068
	ctx.lr = 0x825DBBA0;
	sub_8221F068(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DBBC0);
PPC_WEAK_FUNC(sub_825DBBC0) { __imp__sub_825DBBC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DBBC0) {
	PPC_FUNC_PROLOGUE();
	// lis r9,-32070
	ctx.r9.s64 = -2101739520;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// addi r10,r10,32256
	ctx.r10.s64 = ctx.r10.s64 + 32256;
	// lwz r9,-536(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + -536);
	// addi r7,r10,20
	ctx.r7.s64 = ctx.r10.s64 + 20;
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mulli r8,r9,124
	ctx.r8.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(124));
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// clrlwi r6,r6,24
	ctx.r6.u64 = ctx.r6.u32 & 0xFF;
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// clrlwi r9,r9,24
	ctx.r9.u64 = ctx.r9.u32 & 0xFF;
	// rlwimi r10,r6,8,16,23
	ctx.r10.u64 = (__builtin_rotateleft32(ctx.r6.u32, 8) & 0xFF00) | (ctx.r10.u64 & 0xFFFFFFFFFFFF00FF);
	// rlwimi r11,r9,8,16,23
	ctx.r11.u64 = (__builtin_rotateleft32(ctx.r9.u32, 8) & 0xFF00) | (ctx.r11.u64 & 0xFFFFFFFFFFFF00FF);
	// rlwimi r11,r10,16,0,15
	ctx.r11.u64 = (__builtin_rotateleft32(ctx.r10.u32, 16) & 0xFFFF0000) | (ctx.r11.u64 & 0xFFFFFFFF0000FFFF);
	// stwx r11,r8,r7
	PPC_STORE_U32(ctx.r8.u32 + ctx.r7.u32, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DBC10);
PPC_WEAK_FUNC(sub_825DBC10) { __imp__sub_825DBC10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DBC10) {
	PPC_FUNC_PROLOGUE();
	// lis r9,-32070
	ctx.r9.s64 = -2101739520;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// addi r9,r9,32256
	ctx.r9.s64 = ctx.r9.s64 + 32256;
	// addi r6,r9,52
	ctx.r6.s64 = ctx.r9.s64 + 52;
	// addi r5,r9,56
	ctx.r5.s64 = ctx.r9.s64 + 56;
	// lwz r8,8(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,16(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// clrlwi r8,r8,24
	ctx.r8.u64 = ctx.r8.u32 & 0xFF;
	// lwz r7,0(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// clrlwi r9,r9,24
	ctx.r9.u64 = ctx.r9.u32 & 0xFF;
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// clrlwi r4,r4,24
	ctx.r4.u64 = ctx.r4.u32 & 0xFF;
	// lwz r10,-536(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -536);
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// rlwimi r9,r4,8,16,23
	ctx.r9.u64 = (__builtin_rotateleft32(ctx.r4.u32, 8) & 0xFF00) | (ctx.r9.u64 & 0xFFFFFFFFFFFF00FF);
	// rlwimi r11,r8,8,16,23
	ctx.r11.u64 = (__builtin_rotateleft32(ctx.r8.u32, 8) & 0xFF00) | (ctx.r11.u64 & 0xFFFFFFFFFFFF00FF);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// rlwimi r11,r9,16,0,15
	ctx.r11.u64 = (__builtin_rotateleft32(ctx.r9.u32, 16) & 0xFFFF0000) | (ctx.r11.u64 & 0xFFFFFFFF0000FFFF);
	// stwx r11,r10,r6
	PPC_STORE_U32(ctx.r10.u32 + ctx.r6.u32, ctx.r11.u32);
	// stbx r7,r10,r5
	PPC_STORE_U8(ctx.r10.u32 + ctx.r5.u32, ctx.r7.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DBC70);
PPC_WEAK_FUNC(sub_825DBC70) { __imp__sub_825DBC70(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DBC70) {
	PPC_FUNC_PROLOGUE();
	// lis r9,-32070
	ctx.r9.s64 = -2101739520;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// addi r9,r9,32256
	ctx.r9.s64 = ctx.r9.s64 + 32256;
	// addi r6,r9,52
	ctx.r6.s64 = ctx.r9.s64 + 52;
	// addi r5,r9,57
	ctx.r5.s64 = ctx.r9.s64 + 57;
	// lwz r8,8(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,16(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// clrlwi r8,r8,24
	ctx.r8.u64 = ctx.r8.u32 & 0xFF;
	// lwz r7,0(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// clrlwi r9,r9,24
	ctx.r9.u64 = ctx.r9.u32 & 0xFF;
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// clrlwi r4,r4,24
	ctx.r4.u64 = ctx.r4.u32 & 0xFF;
	// lwz r10,-536(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -536);
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// rlwimi r9,r4,8,16,23
	ctx.r9.u64 = (__builtin_rotateleft32(ctx.r4.u32, 8) & 0xFF00) | (ctx.r9.u64 & 0xFFFFFFFFFFFF00FF);
	// rlwimi r11,r8,8,16,23
	ctx.r11.u64 = (__builtin_rotateleft32(ctx.r8.u32, 8) & 0xFF00) | (ctx.r11.u64 & 0xFFFFFFFFFFFF00FF);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// rlwimi r11,r9,16,0,15
	ctx.r11.u64 = (__builtin_rotateleft32(ctx.r9.u32, 16) & 0xFFFF0000) | (ctx.r11.u64 & 0xFFFFFFFF0000FFFF);
	// stwx r11,r10,r6
	PPC_STORE_U32(ctx.r10.u32 + ctx.r6.u32, ctx.r11.u32);
	// stbx r7,r10,r5
	PPC_STORE_U8(ctx.r10.u32 + ctx.r5.u32, ctx.r7.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DBCD0);
PPC_WEAK_FUNC(sub_825DBCD0) { __imp__sub_825DBCD0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DBCD0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// lfs f3,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f3.f64 = double(temp.f32);
	// lfs f4,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f4.f64 = double(temp.f32);
	// b 0x825dba08
	sub_825DBA08(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DBCF0);
PPC_WEAK_FUNC(sub_825DBCF0) { __imp__sub_825DBCF0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DBCF0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f0,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// lfs f0,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,84(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// ld r4,80(r1)
	ctx.r4.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
	// lfs f0,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// lwz r11,88(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
	// rldicr r5,r11,32,63
	ctx.r5.u64 = __builtin_rotateleft64(ctx.r11.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// bl 0x825da538
	ctx.lr = 0x825DBD2C;
	sub_825DA538(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DBD40);
PPC_WEAK_FUNC(sub_825DBD40) { __imp__sub_825DBD40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DBD40) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x825d98e0
	ctx.lr = 0x825DBD60;
	sub_825D98E0(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DBD80);
PPC_WEAK_FUNC(sub_825DBD80) { __imp__sub_825DBD80(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DBD80) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r4)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 8);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// mr r8,r11
	ctx.r8.u64 = ctx.r11.u64;
	// lwz r10,20(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lfs f1,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// lfs f2,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f2.f64 = double(temp.f32);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// cntlzw r9,r10
	ctx.r9.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// lwz r8,24(r8)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r8.u32 + 24);
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
	// lfs f3,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f3.f64 = double(temp.f32);
	// cntlzw r8,r8
	ctx.r8.u64 = ctx.r8.u32 == 0 ? 32 : __builtin_clz(ctx.r8.u32);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// rlwinm r9,r9,27,31,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// rlwinm r8,r8,27,31,31
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 27) & 0x1;
	// xori r9,r9,1
	ctx.r9.u64 = ctx.r9.u64 ^ 1;
	// lwz r11,32(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 32);
	// xori r5,r8,1
	ctx.r5.u64 = ctx.r8.u64 ^ 1;
	// clrlwi r8,r9,24
	ctx.r8.u64 = ctx.r9.u32 & 0xFF;
	// lwz r10,28(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 28);
	// clrlwi r9,r5,24
	ctx.r9.u64 = ctx.r5.u32 & 0xFF;
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// mtctr r6
	ctx.ctr.u64 = ctx.r6.u64;
	// bctrl 
	ctx.lr = 0x825DBDEC;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DBE00);
PPC_WEAK_FUNC(sub_825DBE00) { __imp__sub_825DBE00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DBE00) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c0
	ctx.lr = 0x825DBE08;
	__savegprlr_26(ctx, base);
	// stwu r1,-192(r1)
	ea = -192 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r4)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 8);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r10,56(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 56);
	// lwz r9,52(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 52);
	// lwz r30,32(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 32);
	// lwz r29,36(r11)
	ctx.r29.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// lwz r28,40(r11)
	ctx.r28.u64 = PPC_LOAD_U32(ctx.r11.u32 + 40);
	// lwz r27,44(r11)
	ctx.r27.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
	// lwz r26,48(r11)
	ctx.r26.u64 = PPC_LOAD_U32(ctx.r11.u32 + 48);
	// stw r10,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r10.u32);
	// stw r9,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r9.u32);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lwz r9,24(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// lwz r10,28(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	// stw r26,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r26.u32);
	// stw r27,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r27.u32);
	// stw r28,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r28.u32);
	// stw r29,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r29.u32);
	// stw r30,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r30.u32);
	// mtctr r31
	ctx.ctr.u64 = ctx.r31.u64;
	// bctrl 
	ctx.lr = 0x825DBE74;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// addi r1,r1,192
	ctx.r1.s64 = ctx.r1.s64 + 192;
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DBE80);
PPC_WEAK_FUNC(sub_825DBE80) { __imp__sub_825DBE80(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DBE80) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r4)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 8);
	// lwz r10,40(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 40);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lwz r9,44(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// lwz r8,48(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 48);
	// lfs f3,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f3.f64 = double(temp.f32);
	// lwz r7,52(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 52);
	// lfs f4,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f4.f64 = double(temp.f32);
	// lwz r6,56(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 56);
	// lfs f5,16(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f5.f64 = double(temp.f32);
	// lwz r5,64(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 64);
	// lfs f6,20(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	ctx.f6.f64 = double(temp.f32);
	// lwz r4,60(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 60);
	// lfs f7,24(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	ctx.f7.f64 = double(temp.f32);
	// lfs f8,28(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	ctx.f8.f64 = double(temp.f32);
	// stw r10,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r10.u32);
	// lfs f9,32(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 32);
	ctx.f9.f64 = double(temp.f32);
	// stw r9,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r9.u32);
	// lfs f10,36(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	ctx.f10.f64 = double(temp.f32);
	// stw r8,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r8.u32);
	// stw r5,148(r1)
	PPC_STORE_U32(ctx.r1.u32 + 148, ctx.r5.u32);
	// stw r4,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r4.u32);
	// stw r7,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r7.u32);
	// stw r6,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r6.u32);
	// mtctr r3
	ctx.ctr.u64 = ctx.r3.u64;
	// bctrl 
	ctx.lr = 0x825DBEF8;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DBF08);
PPC_WEAK_FUNC(sub_825DBF08) { __imp__sub_825DBF08(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DBF08) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7cc
	ctx.lr = 0x825DBF10;
	__savegprlr_29(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32067
	ctx.r10.s64 = -2101542912;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,-1640
	ctx.r31.s64 = ctx.r10.s64 + -1640;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r30,4(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r29,8(r11)
	ctx.r29.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x8221fd88
	ctx.lr = 0x825DBF34;
	sub_8221FD88(ctx, base);
	// li r11,-1
	ctx.r11.s64 = -1;
	// lwz r4,604(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// li r10,-1
	ctx.r10.s64 = -1;
	// li r9,-1
	ctx.r9.s64 = -1;
	// li r8,-1
	ctx.r8.s64 = -1;
	// li r7,1
	ctx.r7.s64 = 1;
	// addi r6,r29,-1
	ctx.r6.s64 = ctx.r29.s64 + -1;
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// mr r5,r30
	ctx.r5.u64 = ctx.r30.u64;
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x82222308
	ctx.lr = 0x825DBF64;
	sub_82222308(ctx, base);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DBF70);
PPC_WEAK_FUNC(sub_825DBF70) { __imp__sub_825DBF70(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DBF70) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d6400
	sub_825D6400(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DBF88);
PPC_WEAK_FUNC(sub_825DBF88) { __imp__sub_825DBF88(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DBF88) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d6498
	sub_825D6498(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DBFA0);
PPC_WEAK_FUNC(sub_825DBFA0) { __imp__sub_825DBFA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DBFA0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7cc
	ctx.lr = 0x825DBFA8;
	__savegprlr_29(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32067
	ctx.r10.s64 = -2101542912;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,-1640
	ctx.r31.s64 = ctx.r10.s64 + -1640;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r30,4(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r29,8(r11)
	ctx.r29.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x8221fd88
	ctx.lr = 0x825DBFCC;
	sub_8221FD88(ctx, base);
	// li r11,-1
	ctx.r11.s64 = -1;
	// lwz r4,604(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// li r10,-1
	ctx.r10.s64 = -1;
	// li r9,-1
	ctx.r9.s64 = -1;
	// li r8,-1
	ctx.r8.s64 = -1;
	// li r7,0
	ctx.r7.s64 = 0;
	// addi r6,r29,-1
	ctx.r6.s64 = ctx.r29.s64 + -1;
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// mr r5,r30
	ctx.r5.u64 = ctx.r30.u64;
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x82222308
	ctx.lr = 0x825DBFFC;
	sub_82222308(ctx, base);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC008);
PPC_WEAK_FUNC(sub_825DC008) { __imp__sub_825DC008(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC008) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c8
	ctx.lr = 0x825DC010;
	__savegprlr_28(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32067
	ctx.r10.s64 = -2101542912;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,-1640
	ctx.r31.s64 = ctx.r10.s64 + -1640;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r30,4(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r29,8(r11)
	ctx.r29.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r28,12(r11)
	ctx.r28.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x8221fd88
	ctx.lr = 0x825DC038;
	sub_8221FD88(ctx, base);
	// li r11,-1
	ctx.r11.s64 = -1;
	// lwz r4,604(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// li r10,-1
	ctx.r10.s64 = -1;
	// li r9,-1
	ctx.r9.s64 = -1;
	// mr r8,r30
	ctx.r8.u64 = ctx.r30.u64;
	// li r7,1
	ctx.r7.s64 = 1;
	// addi r6,r28,-1
	ctx.r6.s64 = ctx.r28.s64 + -1;
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// mr r5,r29
	ctx.r5.u64 = ctx.r29.u64;
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x82222308
	ctx.lr = 0x825DC068;
	sub_82222308(ctx, base);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x829ff818
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC070);
PPC_WEAK_FUNC(sub_825DC070) { __imp__sub_825DC070(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC070) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c4
	ctx.lr = 0x825DC078;
	__savegprlr_27(ctx, base);
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32067
	ctx.r10.s64 = -2101542912;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,-1640
	ctx.r31.s64 = ctx.r10.s64 + -1640;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r30,4(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r29,8(r11)
	ctx.r29.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r28,12(r11)
	ctx.r28.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r27,16(r11)
	ctx.r27.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x8221fd88
	ctx.lr = 0x825DC0A4;
	sub_8221FD88(ctx, base);
	// li r11,-1
	ctx.r11.s64 = -1;
	// lwz r4,604(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 604);
	// li r10,-1
	ctx.r10.s64 = -1;
	// mr r9,r29
	ctx.r9.u64 = ctx.r29.u64;
	// mr r8,r30
	ctx.r8.u64 = ctx.r30.u64;
	// li r7,1
	ctx.r7.s64 = 1;
	// addi r6,r27,-1
	ctx.r6.s64 = ctx.r27.s64 + -1;
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x82222308
	ctx.lr = 0x825DC0D4;
	sub_82222308(ctx, base);
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// b 0x829ff814
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC0E0);
PPC_WEAK_FUNC(sub_825DC0E0) { __imp__sub_825DC0E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC0E0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d65a8
	sub_825D65A8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC0F8);
PPC_WEAK_FUNC(sub_825DC0F8) { __imp__sub_825DC0F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC0F8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d6640
	sub_825D6640(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC110);
PPC_WEAK_FUNC(sub_825DC110) { __imp__sub_825DC110(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC110) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d66d8
	sub_825D66D8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC130);
PPC_WEAK_FUNC(sub_825DC130) { __imp__sub_825DC130(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC130) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d6778
	sub_825D6778(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC150);
PPC_WEAK_FUNC(sub_825DC150) { __imp__sub_825DC150(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC150) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d6818
	sub_825D6818(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC170);
PPC_WEAK_FUNC(sub_825DC170) { __imp__sub_825DC170(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC170) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d68b8
	sub_825D68B8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC190);
PPC_WEAK_FUNC(sub_825DC190) { __imp__sub_825DC190(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC190) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r9,24(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// lwz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d6958
	sub_825D6958(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC1B8);
PPC_WEAK_FUNC(sub_825DC1B8) { __imp__sub_825DC1B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC1B8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r9,24(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// lwz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d6a00
	sub_825D6A00(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC1E0);
PPC_WEAK_FUNC(sub_825DC1E0) { __imp__sub_825DC1E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC1E0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lwz r10,28(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	// lwz r9,24(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d6aa8
	sub_825D6AA8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC208);
PPC_WEAK_FUNC(sub_825DC208) { __imp__sub_825DC208(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC208) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lwz r10,28(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	// lwz r9,24(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d6b50
	sub_825D6B50(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC230);
PPC_WEAK_FUNC(sub_825DC230) { __imp__sub_825DC230(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC230) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,32(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 32);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// stw r3,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r3.u32);
	// lwz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lwz r9,24(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// lwz r10,28(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x825d6bf8
	ctx.lr = 0x825DC26C;
	sub_825D6BF8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC280);
PPC_WEAK_FUNC(sub_825DC280) { __imp__sub_825DC280(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC280) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,32(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 32);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// stw r3,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r3.u32);
	// lwz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lwz r9,24(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// lwz r10,28(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x825d6ca0
	ctx.lr = 0x825DC2BC;
	sub_825D6CA0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC2D0);
PPC_WEAK_FUNC(sub_825DC2D0) { __imp__sub_825DC2D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC2D0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x825d6d48
	sub_825D6D48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC2E8);
PPC_WEAK_FUNC(sub_825DC2E8) { __imp__sub_825DC2E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC2E8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x825d6df8
	sub_825D6DF8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC300);
PPC_WEAK_FUNC(sub_825DC300) { __imp__sub_825DC300(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC300) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x825d6ea8
	sub_825D6EA8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC318);
PPC_WEAK_FUNC(sub_825DC318) { __imp__sub_825DC318(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC318) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x825d6f68
	sub_825D6F68(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC330);
PPC_WEAK_FUNC(sub_825DC330) { __imp__sub_825DC330(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC330) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// b 0x825d7028
	sub_825D7028(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC350);
PPC_WEAK_FUNC(sub_825DC350) { __imp__sub_825DC350(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC350) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// b 0x825d7100
	sub_825D7100(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC370);
PPC_WEAK_FUNC(sub_825DC370) { __imp__sub_825DC370(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC370) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// addi r3,r11,-1640
	ctx.r3.s64 = ctx.r11.s64 + -1640;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x8221fd88
	ctx.lr = 0x825DC390;
	sub_8221FD88(ctx, base);
	// bl 0x822238a0
	ctx.lr = 0x825DC394;
	sub_822238A0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC3A8);
PPC_WEAK_FUNC(sub_825DC3A8) { __imp__sub_825DC3A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC3A8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// addi r3,r11,-1640
	ctx.r3.s64 = ctx.r11.s64 + -1640;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x8221fd88
	ctx.lr = 0x825DC3C8;
	sub_8221FD88(ctx, base);
	// bl 0x82222130
	ctx.lr = 0x825DC3CC;
	sub_82222130(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC3E0);
PPC_WEAK_FUNC(sub_825DC3E0) { __imp__sub_825DC3E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC3E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c8
	ctx.lr = 0x825DC3E8;
	__savegprlr_28(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r30,32(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 32);
	// lwz r29,36(r11)
	ctx.r29.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// lwz r28,40(r11)
	ctx.r28.u64 = PPC_LOAD_U32(ctx.r11.u32 + 40);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lwz r9,24(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// lwz r10,28(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// stw r30,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r30.u32);
	// stw r29,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r29.u32);
	// stw r28,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r28.u32);
	// bl 0x825d71d8
	ctx.lr = 0x825DC430;
	sub_825D71D8(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x829ff818
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC448);
PPC_WEAK_FUNC(sub_825DC448) { __imp__sub_825DC448(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC448) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32067
	ctx.r10.s64 = -2101542912;
	// addi r3,r10,-1640
	ctx.r3.s64 = ctx.r10.s64 + -1640;
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x82220488
	sub_82220488(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC460);
PPC_WEAK_FUNC(sub_825DC460) { __imp__sub_825DC460(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC460) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32067
	ctx.r10.s64 = -2101542912;
	// addi r3,r10,-1640
	ctx.r3.s64 = ctx.r10.s64 + -1640;
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x822205f8
	sub_822205F8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC478);
PPC_WEAK_FUNC(sub_825DC478) { __imp__sub_825DC478(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC478) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lis r10,-32067
	ctx.r10.s64 = -2101542912;
	// addi r3,r10,-1640
	ctx.r3.s64 = ctx.r10.s64 + -1640;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x8221f590
	ctx.lr = 0x825DC4A4;
	sub_8221F590(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC4C8);
PPC_WEAK_FUNC(sub_825DC4C8) { __imp__sub_825DC4C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC4C8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lis r10,-32067
	ctx.r10.s64 = -2101542912;
	// addi r3,r10,-1640
	ctx.r3.s64 = ctx.r10.s64 + -1640;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x8221f5f0
	ctx.lr = 0x825DC4F4;
	sub_8221F5F0(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC518);
PPC_WEAK_FUNC(sub_825DC518) { __imp__sub_825DC518(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC518) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// addi r3,r11,-1640
	ctx.r3.s64 = ctx.r11.s64 + -1640;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x8221f948
	ctx.lr = 0x825DC540;
	sub_8221F948(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// lwz r10,0(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC570);
PPC_WEAK_FUNC(sub_825DC570) { __imp__sub_825DC570(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC570) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x825d73a8
	ctx.lr = 0x825DC590;
	sub_825D73A8(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC5B0);
PPC_WEAK_FUNC(sub_825DC5B0) { __imp__sub_825DC5B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC5B0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x825d7418
	ctx.lr = 0x825DC5D4;
	sub_825D7418(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC5F0);
PPC_WEAK_FUNC(sub_825DC5F0) { __imp__sub_825DC5F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC5F0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x825d74d8
	ctx.lr = 0x825DC614;
	sub_825D74D8(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC630);
PPC_WEAK_FUNC(sub_825DC630) { __imp__sub_825DC630(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC630) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x825d7578
	ctx.lr = 0x825DC650;
	sub_825D7578(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC670);
PPC_WEAK_FUNC(sub_825DC670) { __imp__sub_825DC670(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC670) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x825d7608
	ctx.lr = 0x825DC698;
	sub_825D7608(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC6B8);
PPC_WEAK_FUNC(sub_825DC6B8) { __imp__sub_825DC6B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC6B8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825dc6d4
	if (!ctx.cr6.eq) goto loc_825DC6D4;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
loc_825DC6D4:
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_825DC6D8:
	// lbz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// bne cr6,0x825dc6d8
	if (!ctx.cr6.eq) goto loc_825DC6D8;
	// subf r11,r10,r11
	ctx.r11.u64 = ctx.r11.u64 - ctx.r10.u64;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// rotlwi r11,r11,0
	ctx.r11.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC700);
PPC_WEAK_FUNC(sub_825DC700) { __imp__sub_825DC700(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC700) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x825d76c8
	ctx.lr = 0x825DC724;
	sub_825D76C8(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC740);
PPC_WEAK_FUNC(sub_825DC740) { __imp__sub_825DC740(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC740) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c8
	ctx.lr = 0x825DC748;
	__savegprlr_28(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r31,12(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r30,16(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r29,4(r11)
	ctx.r29.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r28,8(r11)
	ctx.r28.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// bl 0x8239a8a0
	ctx.lr = 0x825DC76C;
	sub_8239A8A0(ctx, base);
	// lwz r11,80(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// rlwinm r11,r11,16,24,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 16) & 0xFF;
	// stw r11,0(r29)
	PPC_STORE_U32(ctx.r29.u32 + 0, ctx.r11.u32);
	// lwz r11,80(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// rlwinm r11,r11,24,24,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 24) & 0xFF;
	// stw r11,0(r28)
	PPC_STORE_U32(ctx.r28.u32 + 0, ctx.r11.u32);
	// lwz r11,80(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// lwz r11,80(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// rlwinm r11,r11,8,24,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 8) & 0xFF;
	// stw r11,0(r30)
	PPC_STORE_U32(ctx.r30.u32 + 0, ctx.r11.u32);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x829ff818
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC7A8);
PPC_WEAK_FUNC(sub_825DC7A8) { __imp__sub_825DC7A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC7A8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// b 0x825da638
	sub_825DA638(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC7C8);
PPC_WEAK_FUNC(sub_825DC7C8) { __imp__sub_825DC7C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC7C8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stfd f30,-32(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f30.u64);
	// stfd f31,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lis r10,-32067
	ctx.r10.s64 = -2101542912;
	// addi r3,r10,-1640
	ctx.r3.s64 = ctx.r10.s64 + -1640;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,8(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lfs f31,0(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f31.f64 = double(temp.f32);
	// lfs f30,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f30.f64 = double(temp.f32);
	// bl 0x8221fd88
	ctx.lr = 0x825DC800;
	sub_8221FD88(ctx, base);
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
	// fmr f2,f30
	ctx.fpscr.disableFlushMode();
	ctx.f2.f64 = ctx.f30.f64;
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// bl 0x825d7770
	ctx.lr = 0x825DC810;
	sub_825D7770(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f30,-32(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// lfd f31,-24(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC838);
PPC_WEAK_FUNC(sub_825DC838) { __imp__sub_825DC838(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC838) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// bl 0x825d78e8
	ctx.lr = 0x825DC868;
	sub_825D78E8(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC888);
PPC_WEAK_FUNC(sub_825DC888) { __imp__sub_825DC888(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC888) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// bl 0x825d79f8
	ctx.lr = 0x825DC8B8;
	sub_825D79F8(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC8D8);
PPC_WEAK_FUNC(sub_825DC8D8) { __imp__sub_825DC8D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC8D8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// b 0x825d7ad0
	sub_825D7AD0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC8F0);
PPC_WEAK_FUNC(sub_825DC8F0) { __imp__sub_825DC8F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC8F0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// b 0x825d7b98
	sub_825D7B98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC908);
PPC_WEAK_FUNC(sub_825DC908) { __imp__sub_825DC908(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC908) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// b 0x825d7c70
	sub_825D7C70(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC928);
PPC_WEAK_FUNC(sub_825DC928) { __imp__sub_825DC928(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC928) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// lwz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// b 0x825d7d50
	sub_825D7D50(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC948);
PPC_WEAK_FUNC(sub_825DC948) { __imp__sub_825DC948(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC948) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lfs f3,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f3.f64 = double(temp.f32);
	// b 0x825d7e40
	sub_825D7E40(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC968);
PPC_WEAK_FUNC(sub_825DC968) { __imp__sub_825DC968(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC968) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// b 0x825d7f28
	sub_825D7F28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC980);
PPC_WEAK_FUNC(sub_825DC980) { __imp__sub_825DC980(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC980) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// b 0x825d8010
	sub_825D8010(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC9A0);
PPC_WEAK_FUNC(sub_825DC9A0) { __imp__sub_825DC9A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC9A0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// b 0x825d8120
	sub_825D8120(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC9B8);
PPC_WEAK_FUNC(sub_825DC9B8) { __imp__sub_825DC9B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC9B8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// b 0x825d8208
	sub_825D8208(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC9D8);
PPC_WEAK_FUNC(sub_825DC9D8) { __imp__sub_825DC9D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC9D8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,0
	ctx.r4.s64 = 0;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d85d8
	sub_825D85D8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DC9F0);
PPC_WEAK_FUNC(sub_825DC9F0) { __imp__sub_825DC9F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DC9F0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r5,1
	ctx.r5.s64 = 1;
	// li r4,0
	ctx.r4.s64 = 0;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d85d8
	sub_825D85D8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCA08);
PPC_WEAK_FUNC(sub_825DCA08) { __imp__sub_825DCA08(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCA08) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r5,0
	ctx.r5.s64 = 0;
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d86f0
	sub_825D86F0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCA20);
PPC_WEAK_FUNC(sub_825DCA20) { __imp__sub_825DCA20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCA20) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r6,0
	ctx.r6.s64 = 0;
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d8808
	sub_825D8808(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCA38);
PPC_WEAK_FUNC(sub_825DCA38) { __imp__sub_825DCA38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCA38) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r6,1
	ctx.r6.s64 = 1;
	// li r5,0
	ctx.r5.s64 = 0;
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d8930
	sub_825D8930(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCA50);
PPC_WEAK_FUNC(sub_825DCA50) { __imp__sub_825DCA50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCA50) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r6,0
	ctx.r6.s64 = 0;
	// li r5,0
	ctx.r5.s64 = 0;
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d8930
	sub_825D8930(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCA68);
PPC_WEAK_FUNC(sub_825DCA68) { __imp__sub_825DCA68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCA68) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,1
	ctx.r4.s64 = 1;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d85d8
	sub_825D85D8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCA80);
PPC_WEAK_FUNC(sub_825DCA80) { __imp__sub_825DCA80(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCA80) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r5,1
	ctx.r5.s64 = 1;
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d86f0
	sub_825D86F0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCA98);
PPC_WEAK_FUNC(sub_825DCA98) { __imp__sub_825DCA98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCA98) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r6,1
	ctx.r6.s64 = 1;
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d8808
	sub_825D8808(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCAB0);
PPC_WEAK_FUNC(sub_825DCAB0) { __imp__sub_825DCAB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCAB0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r6,1
	ctx.r6.s64 = 1;
	// li r5,1
	ctx.r5.s64 = 1;
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d8930
	sub_825D8930(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCAC8);
PPC_WEAK_FUNC(sub_825DCAC8) { __imp__sub_825DCAC8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCAC8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r6,0
	ctx.r6.s64 = 0;
	// li r5,1
	ctx.r5.s64 = 1;
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d8930
	sub_825D8930(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCAE0);
PPC_WEAK_FUNC(sub_825DCAE0) { __imp__sub_825DCAE0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCAE0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r10,-32067
	ctx.r10.s64 = -2101542912;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f0,14452(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 14452);
	ctx.f0.f64 = double(temp.f32);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// stfs f0,0(r10)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// lfs f0,14448(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 14448);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r9)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCB08);
PPC_WEAK_FUNC(sub_825DCB08) { __imp__sub_825DCB08(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCB08) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x825d8ad8
	ctx.lr = 0x825DCB28;
	sub_825D8AD8(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCB48);
PPC_WEAK_FUNC(sub_825DCB48) { __imp__sub_825DCB48(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCB48) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x825d8b60
	ctx.lr = 0x825DCB6C;
	sub_825D8B60(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCB90);
PPC_WEAK_FUNC(sub_825DCB90) { __imp__sub_825DCB90(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCB90) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x825d8bf8
	ctx.lr = 0x825DCBB4;
	sub_825D8BF8(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCBD8);
PPC_WEAK_FUNC(sub_825DCBD8) { __imp__sub_825DCBD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCBD8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d8458
	sub_825D8458(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCBE8);
PPC_WEAK_FUNC(sub_825DCBE8) { __imp__sub_825DCBE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCBE8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d8518
	sub_825D8518(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCBF8);
PPC_WEAK_FUNC(sub_825DCBF8) { __imp__sub_825DCBF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCBF8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r8,0
	ctx.r8.s64 = 0;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r7,-1
	ctx.r7.s64 = -1;
	// li r10,-1
	ctx.r10.s64 = -1;
	// li r9,0
	ctx.r9.s64 = 0;
	// stw r8,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r8.u32);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// stw r8,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r8.u32);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// stw r7,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r7.u32);
	// lfs f3,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f3.f64 = double(temp.f32);
	// stb r8,87(r1)
	PPC_STORE_U8(ctx.r1.u32 + 87, ctx.r8.u8);
	// lwz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// bl 0x8224cf58
	ctx.lr = 0x825DCC44;
	sub_8224CF58(ctx, base);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCC58);
PPC_WEAK_FUNC(sub_825DCC58) { __imp__sub_825DCC58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCC58) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// clrlwi r6,r8,24
	ctx.r6.u64 = ctx.r8.u32 & 0xFF;
	// lfs f3,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f3.f64 = double(temp.f32);
	// lwz r10,28(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	// clrlwi r7,r7,24
	ctx.r7.u64 = ctx.r7.u32 & 0xFF;
	// lwz r9,24(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// lwz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// b 0x8224e420
	sub_8224E420(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCC88);
PPC_WEAK_FUNC(sub_825DCC88) { __imp__sub_825DCC88(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCC88) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,-29384
	ctx.r3.s64 = ctx.r11.s64 + -29384;
	// b 0x825dbd80
	sub_825DBD80(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCC98);
PPC_WEAK_FUNC(sub_825DCC98) { __imp__sub_825DCC98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCC98) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7cc
	ctx.lr = 0x825DCCA0;
	__savegprlr_29(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// lwz r31,0(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r30,4(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// lwz r29,8(r11)
	ctx.r29.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// bl 0x8229c920
	ctx.lr = 0x825DCCC0;
	sub_8229C920(ctx, base);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// addi r3,r1,88
	ctx.r3.s64 = ctx.r1.s64 + 88;
	// lfs f0,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r30)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r30.u32 + 0, temp.u32);
	// bl 0x8229c920
	ctx.lr = 0x825DCCD8;
	sub_8229C920(ctx, base);
	// lfs f0,4(r3)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r29)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r29.u32 + 0, temp.u32);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCCE8);
PPC_WEAK_FUNC(sub_825DCCE8) { __imp__sub_825DCCE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCCE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,-29192
	ctx.r3.s64 = ctx.r11.s64 + -29192;
	// b 0x825dbe00
	sub_825DBE00(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCCF8);
PPC_WEAK_FUNC(sub_825DCCF8) { __imp__sub_825DCCF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCCF8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r9,r1,80
	ctx.r9.s64 = ctx.r1.s64 + 80;
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// subf r9,r10,r9
	ctx.r9.u64 = ctx.r9.u64 - ctx.r10.u64;
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// mr r11,r10
	ctx.r11.u64 = ctx.r10.u64;
loc_825DCD28:
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r9,r11
	PPC_STORE_U8(ctx.r9.u32 + ctx.r11.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x825dcd28
	if (!ctx.cr6.eq) goto loc_825DCD28;
	// li r8,-1
	ctx.r8.s64 = -1;
	// addi r6,r1,80
	ctx.r6.s64 = ctx.r1.s64 + 80;
	// bl 0x8229cb18
	ctx.lr = 0x825DCD48;
	sub_8229CB18(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCD58);
PPC_WEAK_FUNC(sub_825DCD58) { __imp__sub_825DCD58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCD58) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r10,r1,80
	ctx.r10.s64 = ctx.r1.s64 + 80;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// subf r9,r11,r10
	ctx.r9.u64 = ctx.r10.u64 - ctx.r11.u64;
loc_825DCD88:
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r9,r11
	PPC_STORE_U8(ctx.r9.u32 + ctx.r11.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x825dcd88
	if (!ctx.cr6.eq) goto loc_825DCD88;
	// addi r6,r1,80
	ctx.r6.s64 = ctx.r1.s64 + 80;
	// bl 0x8229cb18
	ctx.lr = 0x825DCDA4;
	sub_8229CB18(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCDB8);
PPC_WEAK_FUNC(sub_825DCDB8) { __imp__sub_825DCDB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCDB8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825da320
	sub_825DA320(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCDC8);
PPC_WEAK_FUNC(sub_825DCDC8) { __imp__sub_825DCDC8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCDC8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825da3c8
	sub_825DA3C8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCDD8);
PPC_WEAK_FUNC(sub_825DCDD8) { __imp__sub_825DCDD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCDD8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825da4a8
	sub_825DA4A8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCDE8);
PPC_WEAK_FUNC(sub_825DCDE8) { __imp__sub_825DCDE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCDE8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d94b0
	sub_825D94B0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCDF8);
PPC_WEAK_FUNC(sub_825DCDF8) { __imp__sub_825DCDF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCDF8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// lfs f3,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f3.f64 = double(temp.f32);
	// b 0x825d9630
	sub_825D9630(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCE10);
PPC_WEAK_FUNC(sub_825DCE10) { __imp__sub_825DCE10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCE10) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// lfs f3,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f3.f64 = double(temp.f32);
	// b 0x825d9790
	sub_825D9790(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCE28);
PPC_WEAK_FUNC(sub_825DCE28) { __imp__sub_825DCE28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCE28) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r9,0(r13)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r13.u32 + 0);
	// li r10,24
	ctx.r10.s64 = 24;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// add r31,r10,r9
	ctx.r31.u64 = ctx.r10.u64 + ctx.r9.u64;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// lwz r30,0(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r3,4(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x821f1268
	ctx.lr = 0x825DCE5C;
	sub_821F1268(ctx, base);
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// li r3,17
	ctx.r3.s64 = 17;
	// bl 0x823375f0
	ctx.lr = 0x825DCE6C;
	sub_823375F0(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCE88);
PPC_WEAK_FUNC(sub_825DCE88) { __imp__sub_825DCE88(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCE88) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r9,0(r13)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r13.u32 + 0);
	// li r10,24
	ctx.r10.s64 = 24;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// add r31,r10,r9
	ctx.r31.u64 = ctx.r10.u64 + ctx.r9.u64;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// lwz r30,0(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r3,4(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x821f1268
	ctx.lr = 0x825DCEBC;
	sub_821F1268(ctx, base);
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// li r3,18
	ctx.r3.s64 = 18;
	// bl 0x823375f0
	ctx.lr = 0x825DCECC;
	sub_823375F0(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCEE8);
PPC_WEAK_FUNC(sub_825DCEE8) { __imp__sub_825DCEE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCEE8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d9a18
	sub_825D9A18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCEF8);
PPC_WEAK_FUNC(sub_825DCEF8) { __imp__sub_825DCEF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCEF8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d9950
	sub_825D9950(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCF08);
PPC_WEAK_FUNC(sub_825DCF08) { __imp__sub_825DCF08(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCF08) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r5,0
	ctx.r5.s64 = 0;
	// li r3,4
	ctx.r3.s64 = 4;
	// lwz r31,0(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x8233a180
	ctx.lr = 0x825DCF34;
	sub_8233A180(ctx, base);
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// lwz r30,8272(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8272);
	// bl 0x821fe708
	ctx.lr = 0x825DCF40;
	sub_821FE708(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// li r5,18
	ctx.r5.s64 = 18;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// bl 0x822d6b60
	ctx.lr = 0x825DCF58;
	sub_822D6B60(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DCF78);
PPC_WEAK_FUNC(sub_825DCF78) { __imp__sub_825DCF78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DCF78) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// lis r9,-32052
	ctx.r9.s64 = -2100559872;
	// addi r3,r9,31752
	ctx.r3.s64 = ctx.r9.s64 + 31752;
	// lwz r10,8(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// mr r8,r10
	ctx.r8.u64 = ctx.r10.u64;
	// lwz r9,4(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// lwz r31,0(r10)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cntlzw r11,r9
	ctx.r11.u64 = ctx.r9.u32 == 0 ? 32 : __builtin_clz(ctx.r9.u32);
	// lwz r5,8(r8)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r8.u32 + 8);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r6,r11,24
	ctx.r6.u64 = ctx.r11.u32 & 0xFF;
	// bl 0x823f1818
	ctx.lr = 0x825DCFC4;
	sub_823F1818(ctx, base);
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// lwz r30,8272(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8272);
	// bl 0x821fe708
	ctx.lr = 0x825DCFD0;
	sub_821FE708(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// li r5,19
	ctx.r5.s64 = 19;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r8,0
	ctx.r8.s64 = 0;
	// bl 0x822d6358
	ctx.lr = 0x825DCFEC;
	sub_822D6358(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DD008);
PPC_WEAK_FUNC(sub_825DD008) { __imp__sub_825DD008(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DD008) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// addi r3,r11,31752
	ctx.r3.s64 = ctx.r11.s64 + 31752;
	// lwz r31,0(r10)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x823f1928
	ctx.lr = 0x825DD034;
	sub_823F1928(ctx, base);
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// lwz r30,8272(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8272);
	// bl 0x821fe708
	ctx.lr = 0x825DD040;
	sub_821FE708(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// li r5,19
	ctx.r5.s64 = 19;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// bl 0x822d6b60
	ctx.lr = 0x825DD058;
	sub_822D6B60(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DD070);
PPC_WEAK_FUNC(sub_825DD070) { __imp__sub_825DD070(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DD070) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32052
	ctx.r10.s64 = -2100559872;
	// addi r3,r10,31752
	ctx.r3.s64 = ctx.r10.s64 + 31752;
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x823f19e0
	sub_823F19E0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DD088);
PPC_WEAK_FUNC(sub_825DD088) { __imp__sub_825DD088(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DD088) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32052
	ctx.r10.s64 = -2100559872;
	// addi r3,r10,31752
	ctx.r3.s64 = ctx.r10.s64 + 31752;
	// lwz r10,8(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r31,0(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// addi r7,r10,-1
	ctx.r7.s64 = ctx.r10.s64 + -1;
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// bl 0x823f1890
	ctx.lr = 0x825DD0C4;
	sub_823F1890(ctx, base);
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// lwz r30,8272(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8272);
	// bl 0x821fe708
	ctx.lr = 0x825DD0D0;
	sub_821FE708(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// li r5,20
	ctx.r5.s64 = 20;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r8,0
	ctx.r8.s64 = 0;
	// bl 0x822d6358
	ctx.lr = 0x825DD0EC;
	sub_822D6358(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DD108);
PPC_WEAK_FUNC(sub_825DD108) { __imp__sub_825DD108(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DD108) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// addi r3,r11,31752
	ctx.r3.s64 = ctx.r11.s64 + 31752;
	// lwz r31,0(r10)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x823f1970
	ctx.lr = 0x825DD134;
	sub_823F1970(ctx, base);
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// lwz r30,8272(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8272);
	// bl 0x821fe708
	ctx.lr = 0x825DD140;
	sub_821FE708(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// li r5,20
	ctx.r5.s64 = 20;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// bl 0x822d6b60
	ctx.lr = 0x825DD158;
	sub_822D6B60(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DD170);
PPC_WEAK_FUNC(sub_825DD170) { __imp__sub_825DD170(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DD170) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32052
	ctx.r10.s64 = -2100559872;
	// addi r3,r10,31752
	ctx.r3.s64 = ctx.r10.s64 + 31752;
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r10
	ctx.r11.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r5,r11,24
	ctx.r5.u64 = ctx.r11.u32 & 0xFF;
	// b 0x823f19b0
	sub_823F19B0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DD1A8);
PPC_WEAK_FUNC(sub_825DD1A8) { __imp__sub_825DD1A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DD1A8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32067
	ctx.r10.s64 = -2101542912;
	// addi r3,r10,-1640
	ctx.r3.s64 = ctx.r10.s64 + -1640;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r31,r11,1
	ctx.r31.u64 = ctx.r11.u64 ^ 1;
	// bl 0x8221fd88
	ctx.lr = 0x825DD1DC;
	sub_8221FD88(ctx, base);
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,19272
	ctx.r3.s64 = ctx.r11.s64 + 19272;
	// clrlwi r5,r31,24
	ctx.r5.u64 = ctx.r31.u32 & 0xFF;
	// bl 0x82223318
	ctx.lr = 0x825DD1F0;
	sub_82223318(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DD208);
PPC_WEAK_FUNC(sub_825DD208) { __imp__sub_825DD208(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DD208) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,-17744
	ctx.r3.s64 = ctx.r11.s64 + -17744;
	// b 0x825dbe80
	sub_825DBE80(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DD218);
PPC_WEAK_FUNC(sub_825DD218) { __imp__sub_825DD218(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DD218) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7cc
	ctx.lr = 0x825DD220;
	__savegprlr_29(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// lwz r31,0(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r30,4(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// lwz r29,8(r11)
	ctx.r29.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// bl 0x8224ebd8
	ctx.lr = 0x825DD240;
	sub_8224EBD8(ctx, base);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// addi r3,r1,88
	ctx.r3.s64 = ctx.r1.s64 + 88;
	// lfs f0,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r30)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r30.u32 + 0, temp.u32);
	// bl 0x8224ebd8
	ctx.lr = 0x825DD258;
	sub_8224EBD8(ctx, base);
	// lfs f0,4(r3)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r29)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r29.u32 + 0, temp.u32);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DD268);
PPC_WEAK_FUNC(sub_825DD268) { __imp__sub_825DD268(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DD268) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825d6530
	sub_825D6530(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DD298);
PPC_WEAK_FUNC(sub_825DD298) { __imp__sub_825DD298(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DD298) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lfs f3,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f3.f64 = double(temp.f32);
	// b 0x825d9f38
	sub_825D9F38(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DD2B8);
PPC_WEAK_FUNC(sub_825DD2B8) { __imp__sub_825DD2B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DD2B8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,8(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// addi r10,r10,4
	ctx.r10.s64 = ctx.r10.s64 + 4;
	// rlwinm r8,r10,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwz r10,4(r9)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r9.u32 + 4);
	// lwz r3,0(r9)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
	// lfs f0,8(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// stwx r10,r8,r11
	PPC_STORE_U32(ctx.r8.u32 + ctx.r11.u32, ctx.r10.u32);
	// lfs f13,4(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,0(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f12.f64 = double(temp.f32);
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// addi r10,r10,2
	ctx.r10.s64 = ctx.r10.s64 + 2;
	// rlwinm r10,r10,4,0,27
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
	// add r10,r10,r11
	ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
	// stfs f12,0(r10)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// stfs f13,4(r10)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// stfs f0,8(r10)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + 8, temp.u32);
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// addi r8,r10,1
	ctx.r8.s64 = ctx.r10.s64 + 1;
	// addi r9,r10,2
	ctx.r9.s64 = ctx.r10.s64 + 2;
	// rlwinm r10,r9,4,0,27
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
	// add r4,r10,r11
	ctx.r4.u64 = ctx.r10.u64 + ctx.r11.u64;
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// b 0x825d9318
	sub_825D9318(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DD320);
PPC_WEAK_FUNC(sub_825DD320) { __imp__sub_825DD320(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DD320) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,8(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// addi r10,r10,4
	ctx.r10.s64 = ctx.r10.s64 + 4;
	// rlwinm r8,r10,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwz r10,4(r9)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r9.u32 + 4);
	// lwz r3,0(r9)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
	// lfs f0,8(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// stwx r10,r8,r11
	PPC_STORE_U32(ctx.r8.u32 + ctx.r11.u32, ctx.r10.u32);
	// lfs f13,4(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,0(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f12.f64 = double(temp.f32);
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// addi r10,r10,2
	ctx.r10.s64 = ctx.r10.s64 + 2;
	// rlwinm r10,r10,4,0,27
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
	// add r10,r10,r11
	ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
	// stfs f12,0(r10)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// stfs f13,4(r10)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// stfs f0,8(r10)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + 8, temp.u32);
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// addi r8,r10,1
	ctx.r8.s64 = ctx.r10.s64 + 1;
	// addi r9,r10,2
	ctx.r9.s64 = ctx.r10.s64 + 2;
	// rlwinm r10,r9,4,0,27
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
	// add r4,r10,r11
	ctx.r4.u64 = ctx.r10.u64 + ctx.r11.u64;
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// b 0x825d9700
	sub_825D9700(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DD388);
PPC_WEAK_FUNC(sub_825DD388) { __imp__sub_825DD388(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DD388) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// lwz r9,8(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// addi r8,r10,4
	ctx.r8.s64 = ctx.r10.s64 + 4;
	// lwz r10,0(r9)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
	// rlwinm r9,r8,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f0,8(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// stwx r10,r9,r11
	PPC_STORE_U32(ctx.r9.u32 + ctx.r11.u32, ctx.r10.u32);
	// lfs f13,4(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,0(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f12.f64 = double(temp.f32);
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// addi r10,r10,2
	ctx.r10.s64 = ctx.r10.s64 + 2;
	// rlwinm r10,r10,4,0,27
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
	// add r10,r10,r11
	ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
	// stfs f12,0(r10)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// stfs f13,4(r10)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// stfs f0,8(r10)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + 8, temp.u32);
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// addi r8,r10,1
	ctx.r8.s64 = ctx.r10.s64 + 1;
	// addi r9,r10,2
	ctx.r9.s64 = ctx.r10.s64 + 2;
	// rlwinm r10,r9,4,0,27
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
	// add r3,r10,r11
	ctx.r3.u64 = ctx.r10.u64 + ctx.r11.u64;
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// b 0x82236d50
	sub_82236D50(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DD3E8);
PPC_WEAK_FUNC(sub_825DD3E8) { __imp__sub_825DD3E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DD3E8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16632
	ctx.r4.s64 = ctx.r11.s64 + -16632;
	// addi r3,r10,-25812
	ctx.r3.s64 = ctx.r10.s64 + -25812;
	// bl 0x82845600
	ctx.lr = 0x825DD408;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16528
	ctx.r4.s64 = ctx.r11.s64 + -16528;
	// addi r3,r10,-25820
	ctx.r3.s64 = ctx.r10.s64 + -25820;
	// bl 0x82845600
	ctx.lr = 0x825DD41C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16504
	ctx.r4.s64 = ctx.r11.s64 + -16504;
	// addi r3,r10,-25832
	ctx.r3.s64 = ctx.r10.s64 + -25832;
	// bl 0x82845600
	ctx.lr = 0x825DD430;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-24392
	ctx.r4.s64 = ctx.r11.s64 + -24392;
	// addi r3,r10,-25848
	ctx.r3.s64 = ctx.r10.s64 + -25848;
	// bl 0x82845600
	ctx.lr = 0x825DD444;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-24384
	ctx.r4.s64 = ctx.r11.s64 + -24384;
	// addi r3,r10,-25860
	ctx.r3.s64 = ctx.r10.s64 + -25860;
	// bl 0x82845600
	ctx.lr = 0x825DD458;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16480
	ctx.r4.s64 = ctx.r11.s64 + -16480;
	// addi r3,r10,-25872
	ctx.r3.s64 = ctx.r10.s64 + -25872;
	// bl 0x82845600
	ctx.lr = 0x825DD46C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16376
	ctx.r4.s64 = ctx.r11.s64 + -16376;
	// addi r3,r10,-25896
	ctx.r3.s64 = ctx.r10.s64 + -25896;
	// bl 0x82845600
	ctx.lr = 0x825DD480;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16272
	ctx.r4.s64 = ctx.r11.s64 + -16272;
	// addi r3,r10,-25924
	ctx.r3.s64 = ctx.r10.s64 + -25924;
	// bl 0x82845600
	ctx.lr = 0x825DD494;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16160
	ctx.r4.s64 = ctx.r11.s64 + -16160;
	// addi r3,r10,-25944
	ctx.r3.s64 = ctx.r10.s64 + -25944;
	// bl 0x82845600
	ctx.lr = 0x825DD4A8;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16136
	ctx.r4.s64 = ctx.r11.s64 + -16136;
	// addi r3,r10,-25968
	ctx.r3.s64 = ctx.r10.s64 + -25968;
	// bl 0x82845600
	ctx.lr = 0x825DD4BC;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16112
	ctx.r4.s64 = ctx.r11.s64 + -16112;
	// addi r3,r10,-25992
	ctx.r3.s64 = ctx.r10.s64 + -25992;
	// bl 0x82845600
	ctx.lr = 0x825DD4D0;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16080
	ctx.r4.s64 = ctx.r11.s64 + -16080;
	// addi r3,r10,-26020
	ctx.r3.s64 = ctx.r10.s64 + -26020;
	// bl 0x82845600
	ctx.lr = 0x825DD4E4;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16048
	ctx.r4.s64 = ctx.r11.s64 + -16048;
	// addi r3,r10,-26044
	ctx.r3.s64 = ctx.r10.s64 + -26044;
	// bl 0x82845600
	ctx.lr = 0x825DD4F8;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16016
	ctx.r4.s64 = ctx.r11.s64 + -16016;
	// addi r3,r10,-26072
	ctx.r3.s64 = ctx.r10.s64 + -26072;
	// bl 0x82845600
	ctx.lr = 0x825DD50C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15984
	ctx.r4.s64 = ctx.r11.s64 + -15984;
	// addi r3,r10,-26096
	ctx.r3.s64 = ctx.r10.s64 + -26096;
	// bl 0x82845600
	ctx.lr = 0x825DD520;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15944
	ctx.r4.s64 = ctx.r11.s64 + -15944;
	// addi r3,r10,-26124
	ctx.r3.s64 = ctx.r10.s64 + -26124;
	// bl 0x82845600
	ctx.lr = 0x825DD534;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15904
	ctx.r4.s64 = ctx.r11.s64 + -15904;
	// addi r3,r10,-26148
	ctx.r3.s64 = ctx.r10.s64 + -26148;
	// bl 0x82845600
	ctx.lr = 0x825DD548;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15864
	ctx.r4.s64 = ctx.r11.s64 + -15864;
	// addi r3,r10,-26176
	ctx.r3.s64 = ctx.r10.s64 + -26176;
	// bl 0x82845600
	ctx.lr = 0x825DD55C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15824
	ctx.r4.s64 = ctx.r11.s64 + -15824;
	// addi r3,r10,-26200
	ctx.r3.s64 = ctx.r10.s64 + -26200;
	// bl 0x82845600
	ctx.lr = 0x825DD570;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15744
	ctx.r4.s64 = ctx.r11.s64 + -15744;
	// addi r3,r10,-26228
	ctx.r3.s64 = ctx.r10.s64 + -26228;
	// bl 0x82845600
	ctx.lr = 0x825DD584;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15664
	ctx.r4.s64 = ctx.r11.s64 + -15664;
	// addi r3,r10,-26252
	ctx.r3.s64 = ctx.r10.s64 + -26252;
	// bl 0x82845600
	ctx.lr = 0x825DD598;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15640
	ctx.r4.s64 = ctx.r11.s64 + -15640;
	// addi r3,r10,-26280
	ctx.r3.s64 = ctx.r10.s64 + -26280;
	// bl 0x82845600
	ctx.lr = 0x825DD5AC;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15616
	ctx.r4.s64 = ctx.r11.s64 + -15616;
	// addi r3,r10,-26316
	ctx.r3.s64 = ctx.r10.s64 + -26316;
	// bl 0x82845600
	ctx.lr = 0x825DD5C0;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15592
	ctx.r4.s64 = ctx.r11.s64 + -15592;
	// addi r3,r10,-26356
	ctx.r3.s64 = ctx.r10.s64 + -26356;
	// bl 0x82845600
	ctx.lr = 0x825DD5D4;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15568
	ctx.r4.s64 = ctx.r11.s64 + -15568;
	// addi r3,r10,-26396
	ctx.r3.s64 = ctx.r10.s64 + -26396;
	// bl 0x82845600
	ctx.lr = 0x825DD5E8;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15536
	ctx.r4.s64 = ctx.r11.s64 + -15536;
	// addi r3,r10,-26440
	ctx.r3.s64 = ctx.r10.s64 + -26440;
	// bl 0x82845600
	ctx.lr = 0x825DD5FC;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15504
	ctx.r4.s64 = ctx.r11.s64 + -15504;
	// addi r3,r10,-26460
	ctx.r3.s64 = ctx.r10.s64 + -26460;
	// bl 0x82845600
	ctx.lr = 0x825DD610;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15448
	ctx.r4.s64 = ctx.r11.s64 + -15448;
	// addi r3,r10,-26484
	ctx.r3.s64 = ctx.r10.s64 + -26484;
	// bl 0x82845600
	ctx.lr = 0x825DD624;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-22824
	ctx.r4.s64 = ctx.r11.s64 + -22824;
	// addi r3,r10,-26512
	ctx.r3.s64 = ctx.r10.s64 + -26512;
	// bl 0x82845600
	ctx.lr = 0x825DD638;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-24376
	ctx.r4.s64 = ctx.r11.s64 + -24376;
	// addi r3,r10,-26532
	ctx.r3.s64 = ctx.r10.s64 + -26532;
	// bl 0x82845600
	ctx.lr = 0x825DD64C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15392
	ctx.r4.s64 = ctx.r11.s64 + -15392;
	// addi r3,r10,-26564
	ctx.r3.s64 = ctx.r10.s64 + -26564;
	// bl 0x82845600
	ctx.lr = 0x825DD660;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15288
	ctx.r4.s64 = ctx.r11.s64 + -15288;
	// addi r3,r10,-26588
	ctx.r3.s64 = ctx.r10.s64 + -26588;
	// bl 0x82845600
	ctx.lr = 0x825DD674;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// addi r4,r11,-15264
	ctx.r4.s64 = ctx.r11.s64 + -15264;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r3,r11,-26612
	ctx.r3.s64 = ctx.r11.s64 + -26612;
	// bl 0x82845600
	ctx.lr = 0x825DD688;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-22808
	ctx.r4.s64 = ctx.r11.s64 + -22808;
	// addi r3,r10,-26640
	ctx.r3.s64 = ctx.r10.s64 + -26640;
	// bl 0x82845600
	ctx.lr = 0x825DD69C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-22736
	ctx.r4.s64 = ctx.r11.s64 + -22736;
	// addi r3,r10,-26664
	ctx.r3.s64 = ctx.r10.s64 + -26664;
	// bl 0x82845600
	ctx.lr = 0x825DD6B0;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-22704
	ctx.r4.s64 = ctx.r11.s64 + -22704;
	// addi r3,r10,-26696
	ctx.r3.s64 = ctx.r10.s64 + -26696;
	// bl 0x82845600
	ctx.lr = 0x825DD6C4;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15240
	ctx.r4.s64 = ctx.r11.s64 + -15240;
	// addi r3,r10,-26728
	ctx.r3.s64 = ctx.r10.s64 + -26728;
	// bl 0x82845600
	ctx.lr = 0x825DD6D8;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15160
	ctx.r4.s64 = ctx.r11.s64 + -15160;
	// addi r3,r10,-26764
	ctx.r3.s64 = ctx.r10.s64 + -26764;
	// bl 0x82845600
	ctx.lr = 0x825DD6EC;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-22632
	ctx.r4.s64 = ctx.r11.s64 + -22632;
	// addi r3,r10,-26792
	ctx.r3.s64 = ctx.r10.s64 + -26792;
	// bl 0x82845600
	ctx.lr = 0x825DD700;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15080
	ctx.r4.s64 = ctx.r11.s64 + -15080;
	// addi r3,r10,-26816
	ctx.r3.s64 = ctx.r10.s64 + -26816;
	// bl 0x82845600
	ctx.lr = 0x825DD714;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-14992
	ctx.r4.s64 = ctx.r11.s64 + -14992;
	// addi r3,r10,-26844
	ctx.r3.s64 = ctx.r10.s64 + -26844;
	// bl 0x82845600
	ctx.lr = 0x825DD728;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-14928
	ctx.r4.s64 = ctx.r11.s64 + -14928;
	// addi r3,r10,-26880
	ctx.r3.s64 = ctx.r10.s64 + -26880;
	// bl 0x82845600
	ctx.lr = 0x825DD73C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-14864
	ctx.r4.s64 = ctx.r11.s64 + -14864;
	// addi r3,r10,-26924
	ctx.r3.s64 = ctx.r10.s64 + -26924;
	// bl 0x82845600
	ctx.lr = 0x825DD750;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-14800
	ctx.r4.s64 = ctx.r11.s64 + -14800;
	// addi r3,r10,-26968
	ctx.r3.s64 = ctx.r10.s64 + -26968;
	// bl 0x82845600
	ctx.lr = 0x825DD764;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-14736
	ctx.r4.s64 = ctx.r11.s64 + -14736;
	// addi r3,r10,-26992
	ctx.r3.s64 = ctx.r10.s64 + -26992;
	// bl 0x82845600
	ctx.lr = 0x825DD778;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-14664
	ctx.r4.s64 = ctx.r11.s64 + -14664;
	// addi r3,r10,-27024
	ctx.r3.s64 = ctx.r10.s64 + -27024;
	// bl 0x82845600
	ctx.lr = 0x825DD78C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-17584
	ctx.r4.s64 = ctx.r11.s64 + -17584;
	// addi r3,r10,-27052
	ctx.r3.s64 = ctx.r10.s64 + -27052;
	// bl 0x82845600
	ctx.lr = 0x825DD7A0;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-14592
	ctx.r4.s64 = ctx.r11.s64 + -14592;
	// addi r3,r10,-27080
	ctx.r3.s64 = ctx.r10.s64 + -27080;
	// bl 0x82845600
	ctx.lr = 0x825DD7B4;
	sub_82845600(ctx, base);
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// addi r3,r10,-27112
	ctx.r3.s64 = ctx.r10.s64 + -27112;
	// addi r4,r11,-22520
	ctx.r4.s64 = ctx.r11.s64 + -22520;
	// bl 0x82845600
	ctx.lr = 0x825DD7C8;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-22480
	ctx.r4.s64 = ctx.r11.s64 + -22480;
	// addi r3,r10,-27136
	ctx.r3.s64 = ctx.r10.s64 + -27136;
	// bl 0x82845600
	ctx.lr = 0x825DD7DC;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-22440
	ctx.r4.s64 = ctx.r11.s64 + -22440;
	// addi r3,r10,-27148
	ctx.r3.s64 = ctx.r10.s64 + -27148;
	// bl 0x82845600
	ctx.lr = 0x825DD7F0;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-22392
	ctx.r4.s64 = ctx.r11.s64 + -22392;
	// addi r3,r10,-27164
	ctx.r3.s64 = ctx.r10.s64 + -27164;
	// bl 0x82845600
	ctx.lr = 0x825DD804;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-22344
	ctx.r4.s64 = ctx.r11.s64 + -22344;
	// addi r3,r10,-27184
	ctx.r3.s64 = ctx.r10.s64 + -27184;
	// bl 0x82845600
	ctx.lr = 0x825DD818;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-22296
	ctx.r4.s64 = ctx.r11.s64 + -22296;
	// addi r3,r10,-27196
	ctx.r3.s64 = ctx.r10.s64 + -27196;
	// bl 0x82845600
	ctx.lr = 0x825DD82C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-22224
	ctx.r4.s64 = ctx.r11.s64 + -22224;
	// addi r3,r10,-27208
	ctx.r3.s64 = ctx.r10.s64 + -27208;
	// bl 0x82845600
	ctx.lr = 0x825DD840;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-22152
	ctx.r4.s64 = ctx.r11.s64 + -22152;
	// addi r3,r10,-27220
	ctx.r3.s64 = ctx.r10.s64 + -27220;
	// bl 0x82845600
	ctx.lr = 0x825DD854;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-22112
	ctx.r4.s64 = ctx.r11.s64 + -22112;
	// addi r3,r10,-27244
	ctx.r3.s64 = ctx.r10.s64 + -27244;
	// bl 0x82845600
	ctx.lr = 0x825DD868;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-22032
	ctx.r4.s64 = ctx.r11.s64 + -22032;
	// addi r3,r10,-27280
	ctx.r3.s64 = ctx.r10.s64 + -27280;
	// bl 0x82845600
	ctx.lr = 0x825DD87C;
	sub_82845600(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13680
	ctx.r4.s64 = ctx.r11.s64 + -13680;
	// addi r3,r10,-27296
	ctx.r3.s64 = ctx.r10.s64 + -27296;
	// bl 0x82845600
	ctx.lr = 0x825DD890;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-22000
	ctx.r4.s64 = ctx.r11.s64 + -22000;
	// addi r3,r10,-27312
	ctx.r3.s64 = ctx.r10.s64 + -27312;
	// bl 0x82845600
	ctx.lr = 0x825DD8A4;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-14528
	ctx.r4.s64 = ctx.r11.s64 + -14528;
	// addi r3,r10,-27328
	ctx.r3.s64 = ctx.r10.s64 + -27328;
	// bl 0x82845600
	ctx.lr = 0x825DD8B8;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-14424
	ctx.r4.s64 = ctx.r11.s64 + -14424;
	// addi r3,r10,-27352
	ctx.r3.s64 = ctx.r10.s64 + -27352;
	// bl 0x82845600
	ctx.lr = 0x825DD8CC;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-24368
	ctx.r4.s64 = ctx.r11.s64 + -24368;
	// addi r3,r10,-27372
	ctx.r3.s64 = ctx.r10.s64 + -27372;
	// bl 0x82845600
	ctx.lr = 0x825DD8E0;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-24336
	ctx.r4.s64 = ctx.r11.s64 + -24336;
	// addi r3,r10,-27392
	ctx.r3.s64 = ctx.r10.s64 + -27392;
	// bl 0x82845600
	ctx.lr = 0x825DD8F4;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-21952
	ctx.r4.s64 = ctx.r11.s64 + -21952;
	// addi r3,r10,-27416
	ctx.r3.s64 = ctx.r10.s64 + -27416;
	// bl 0x82845600
	ctx.lr = 0x825DD908;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-24328
	ctx.r4.s64 = ctx.r11.s64 + -24328;
	// addi r3,r10,-27444
	ctx.r3.s64 = ctx.r10.s64 + -27444;
	// bl 0x82845600
	ctx.lr = 0x825DD91C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-14392
	ctx.r4.s64 = ctx.r11.s64 + -14392;
	// addi r3,r10,-27464
	ctx.r3.s64 = ctx.r10.s64 + -27464;
	// bl 0x82845600
	ctx.lr = 0x825DD930;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-14280
	ctx.r4.s64 = ctx.r11.s64 + -14280;
	// addi r3,r10,-27500
	ctx.r3.s64 = ctx.r10.s64 + -27500;
	// bl 0x82845600
	ctx.lr = 0x825DD944;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-14200
	ctx.r4.s64 = ctx.r11.s64 + -14200;
	// addi r3,r10,-27540
	ctx.r3.s64 = ctx.r10.s64 + -27540;
	// bl 0x82845600
	ctx.lr = 0x825DD958;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-14120
	ctx.r4.s64 = ctx.r11.s64 + -14120;
	// addi r3,r10,-27556
	ctx.r3.s64 = ctx.r10.s64 + -27556;
	// bl 0x82845600
	ctx.lr = 0x825DD96C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-21880
	ctx.r4.s64 = ctx.r11.s64 + -21880;
	// addi r3,r10,-27572
	ctx.r3.s64 = ctx.r10.s64 + -27572;
	// bl 0x82845600
	ctx.lr = 0x825DD980;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-21824
	ctx.r4.s64 = ctx.r11.s64 + -21824;
	// addi r3,r10,-27596
	ctx.r3.s64 = ctx.r10.s64 + -27596;
	// bl 0x82845600
	ctx.lr = 0x825DD994;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-17472
	ctx.r4.s64 = ctx.r11.s64 + -17472;
	// addi r3,r10,-27612
	ctx.r3.s64 = ctx.r10.s64 + -27612;
	// bl 0x82845600
	ctx.lr = 0x825DD9A8;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-21768
	ctx.r4.s64 = ctx.r11.s64 + -21768;
	// addi r3,r10,-27632
	ctx.r3.s64 = ctx.r10.s64 + -27632;
	// bl 0x82845600
	ctx.lr = 0x825DD9BC;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-21696
	ctx.r4.s64 = ctx.r11.s64 + -21696;
	// addi r3,r10,-27648
	ctx.r3.s64 = ctx.r10.s64 + -27648;
	// bl 0x82845600
	ctx.lr = 0x825DD9D0;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-21624
	ctx.r4.s64 = ctx.r11.s64 + -21624;
	// addi r3,r10,-27684
	ctx.r3.s64 = ctx.r10.s64 + -27684;
	// bl 0x82845600
	ctx.lr = 0x825DD9E4;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-21568
	ctx.r4.s64 = ctx.r11.s64 + -21568;
	// addi r3,r10,-27712
	ctx.r3.s64 = ctx.r10.s64 + -27712;
	// bl 0x82845600
	ctx.lr = 0x825DD9F8;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-21528
	ctx.r4.s64 = ctx.r11.s64 + -21528;
	// addi r3,r10,-27728
	ctx.r3.s64 = ctx.r10.s64 + -27728;
	// bl 0x82845600
	ctx.lr = 0x825DDA0C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-21472
	ctx.r4.s64 = ctx.r11.s64 + -21472;
	// addi r3,r10,-27744
	ctx.r3.s64 = ctx.r10.s64 + -27744;
	// bl 0x82845600
	ctx.lr = 0x825DDA20;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-21472
	ctx.r4.s64 = ctx.r11.s64 + -21472;
	// addi r3,r10,-27768
	ctx.r3.s64 = ctx.r10.s64 + -27768;
	// bl 0x82845600
	ctx.lr = 0x825DDA34;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-21432
	ctx.r4.s64 = ctx.r11.s64 + -21432;
	// addi r3,r10,-27788
	ctx.r3.s64 = ctx.r10.s64 + -27788;
	// bl 0x82845600
	ctx.lr = 0x825DDA48;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-21360
	ctx.r4.s64 = ctx.r11.s64 + -21360;
	// addi r3,r10,-27812
	ctx.r3.s64 = ctx.r10.s64 + -27812;
	// bl 0x82845600
	ctx.lr = 0x825DDA5C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-21288
	ctx.r4.s64 = ctx.r11.s64 + -21288;
	// addi r3,r10,-27836
	ctx.r3.s64 = ctx.r10.s64 + -27836;
	// bl 0x82845600
	ctx.lr = 0x825DDA70;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-21216
	ctx.r4.s64 = ctx.r11.s64 + -21216;
	// addi r3,r10,-27852
	ctx.r3.s64 = ctx.r10.s64 + -27852;
	// bl 0x82845600
	ctx.lr = 0x825DDA84;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-24320
	ctx.r4.s64 = ctx.r11.s64 + -24320;
	// addi r3,r10,-27872
	ctx.r3.s64 = ctx.r10.s64 + -27872;
	// bl 0x82845600
	ctx.lr = 0x825DDA98;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-21200
	ctx.r4.s64 = ctx.r11.s64 + -21200;
	// addi r3,r10,-27892
	ctx.r3.s64 = ctx.r10.s64 + -27892;
	// bl 0x82845600
	ctx.lr = 0x825DDAAC;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-21136
	ctx.r4.s64 = ctx.r11.s64 + -21136;
	// addi r3,r10,-27908
	ctx.r3.s64 = ctx.r10.s64 + -27908;
	// bl 0x82845600
	ctx.lr = 0x825DDAC0;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-21072
	ctx.r4.s64 = ctx.r11.s64 + -21072;
	// addi r3,r10,-27924
	ctx.r3.s64 = ctx.r10.s64 + -27924;
	// bl 0x82845600
	ctx.lr = 0x825DDAD4;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-21032
	ctx.r4.s64 = ctx.r11.s64 + -21032;
	// addi r3,r10,-27952
	ctx.r3.s64 = ctx.r10.s64 + -27952;
	// bl 0x82845600
	ctx.lr = 0x825DDAE8;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-21000
	ctx.r4.s64 = ctx.r11.s64 + -21000;
	// addi r3,r10,-27976
	ctx.r3.s64 = ctx.r10.s64 + -27976;
	// bl 0x82845600
	ctx.lr = 0x825DDAFC;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-14096
	ctx.r4.s64 = ctx.r11.s64 + -14096;
	// addi r3,r10,-28004
	ctx.r3.s64 = ctx.r10.s64 + -28004;
	// bl 0x82845600
	ctx.lr = 0x825DDB10;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-14072
	ctx.r4.s64 = ctx.r11.s64 + -14072;
	// addi r3,r10,-28032
	ctx.r3.s64 = ctx.r10.s64 + -28032;
	// bl 0x82845600
	ctx.lr = 0x825DDB24;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-14040
	ctx.r4.s64 = ctx.r11.s64 + -14040;
	// addi r3,r10,-28060
	ctx.r3.s64 = ctx.r10.s64 + -28060;
	// bl 0x82845600
	ctx.lr = 0x825DDB38;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-17392
	ctx.r4.s64 = ctx.r11.s64 + -17392;
	// addi r3,r10,-28080
	ctx.r3.s64 = ctx.r10.s64 + -28080;
	// bl 0x82845600
	ctx.lr = 0x825DDB4C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-14008
	ctx.r4.s64 = ctx.r11.s64 + -14008;
	// addi r3,r10,-28104
	ctx.r3.s64 = ctx.r10.s64 + -28104;
	// bl 0x82845600
	ctx.lr = 0x825DDB60;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13976
	ctx.r4.s64 = ctx.r11.s64 + -13976;
	// addi r3,r10,-28140
	ctx.r3.s64 = ctx.r10.s64 + -28140;
	// bl 0x82845600
	ctx.lr = 0x825DDB74;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13952
	ctx.r4.s64 = ctx.r11.s64 + -13952;
	// addi r3,r10,-28180
	ctx.r3.s64 = ctx.r10.s64 + -28180;
	// bl 0x82845600
	ctx.lr = 0x825DDB88;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13920
	ctx.r4.s64 = ctx.r11.s64 + -13920;
	// addi r3,r10,-28208
	ctx.r3.s64 = ctx.r10.s64 + -28208;
	// bl 0x82845600
	ctx.lr = 0x825DDB9C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13896
	ctx.r4.s64 = ctx.r11.s64 + -13896;
	// addi r3,r10,-28240
	ctx.r3.s64 = ctx.r10.s64 + -28240;
	// bl 0x82845600
	ctx.lr = 0x825DDBB0;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-17296
	ctx.r4.s64 = ctx.r11.s64 + -17296;
	// addi r3,r10,-28256
	ctx.r3.s64 = ctx.r10.s64 + -28256;
	// bl 0x82845600
	ctx.lr = 0x825DDBC4;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20928
	ctx.r4.s64 = ctx.r11.s64 + -20928;
	// addi r3,r10,-28280
	ctx.r3.s64 = ctx.r10.s64 + -28280;
	// bl 0x82845600
	ctx.lr = 0x825DDBD8;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20912
	ctx.r4.s64 = ctx.r11.s64 + -20912;
	// addi r3,r10,-28300
	ctx.r3.s64 = ctx.r10.s64 + -28300;
	// bl 0x82845600
	ctx.lr = 0x825DDBEC;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13864
	ctx.r4.s64 = ctx.r11.s64 + -13864;
	// addi r3,r10,-28312
	ctx.r3.s64 = ctx.r10.s64 + -28312;
	// bl 0x82845600
	ctx.lr = 0x825DDC00;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13840
	ctx.r4.s64 = ctx.r11.s64 + -13840;
	// addi r3,r10,-28340
	ctx.r3.s64 = ctx.r10.s64 + -28340;
	// bl 0x82845600
	ctx.lr = 0x825DDC14;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13816
	ctx.r4.s64 = ctx.r11.s64 + -13816;
	// addi r3,r10,-28364
	ctx.r3.s64 = ctx.r10.s64 + -28364;
	// bl 0x82845600
	ctx.lr = 0x825DDC28;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13792
	ctx.r4.s64 = ctx.r11.s64 + -13792;
	// addi r3,r10,-28392
	ctx.r3.s64 = ctx.r10.s64 + -28392;
	// bl 0x82845600
	ctx.lr = 0x825DDC3C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13768
	ctx.r4.s64 = ctx.r11.s64 + -13768;
	// addi r3,r10,-28416
	ctx.r3.s64 = ctx.r10.s64 + -28416;
	// bl 0x82845600
	ctx.lr = 0x825DDC50;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13744
	ctx.r4.s64 = ctx.r11.s64 + -13744;
	// addi r3,r10,-28448
	ctx.r3.s64 = ctx.r10.s64 + -28448;
	// bl 0x82845600
	ctx.lr = 0x825DDC64;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-24312
	ctx.r4.s64 = ctx.r11.s64 + -24312;
	// addi r3,r10,-28460
	ctx.r3.s64 = ctx.r10.s64 + -28460;
	// bl 0x82845600
	ctx.lr = 0x825DDC78;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13720
	ctx.r4.s64 = ctx.r11.s64 + -13720;
	// addi r3,r10,-28480
	ctx.r3.s64 = ctx.r10.s64 + -28480;
	// bl 0x82845600
	ctx.lr = 0x825DDC8C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13696
	ctx.r4.s64 = ctx.r11.s64 + -13696;
	// addi r3,r10,-28512
	ctx.r3.s64 = ctx.r10.s64 + -28512;
	// bl 0x82845600
	ctx.lr = 0x825DDCA0;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13672
	ctx.r4.s64 = ctx.r11.s64 + -13672;
	// addi r3,r10,-28548
	ctx.r3.s64 = ctx.r10.s64 + -28548;
	// bl 0x82845600
	ctx.lr = 0x825DDCB4;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13648
	ctx.r4.s64 = ctx.r11.s64 + -13648;
	// addi r3,r10,-28580
	ctx.r3.s64 = ctx.r10.s64 + -28580;
	// bl 0x82845600
	ctx.lr = 0x825DDCC8;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// addi r4,r11,-13624
	ctx.r4.s64 = ctx.r11.s64 + -13624;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r3,r11,-28620
	ctx.r3.s64 = ctx.r11.s64 + -28620;
	// bl 0x82845600
	ctx.lr = 0x825DDCDC;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20840
	ctx.r4.s64 = ctx.r11.s64 + -20840;
	// addi r3,r10,-28652
	ctx.r3.s64 = ctx.r10.s64 + -28652;
	// bl 0x82845600
	ctx.lr = 0x825DDCF0;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20776
	ctx.r4.s64 = ctx.r11.s64 + -20776;
	// addi r3,r10,-28680
	ctx.r3.s64 = ctx.r10.s64 + -28680;
	// bl 0x82845600
	ctx.lr = 0x825DDD04;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13600
	ctx.r4.s64 = ctx.r11.s64 + -13600;
	// addi r3,r10,-28708
	ctx.r3.s64 = ctx.r10.s64 + -28708;
	// bl 0x82845600
	ctx.lr = 0x825DDD18;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13560
	ctx.r4.s64 = ctx.r11.s64 + -13560;
	// addi r3,r10,-28748
	ctx.r3.s64 = ctx.r10.s64 + -28748;
	// bl 0x82845600
	ctx.lr = 0x825DDD2C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13496
	ctx.r4.s64 = ctx.r11.s64 + -13496;
	// addi r3,r10,-28800
	ctx.r3.s64 = ctx.r10.s64 + -28800;
	// bl 0x82845600
	ctx.lr = 0x825DDD40;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13424
	ctx.r4.s64 = ctx.r11.s64 + -13424;
	// addi r3,r10,-28852
	ctx.r3.s64 = ctx.r10.s64 + -28852;
	// bl 0x82845600
	ctx.lr = 0x825DDD54;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20704
	ctx.r4.s64 = ctx.r11.s64 + -20704;
	// addi r3,r10,-28888
	ctx.r3.s64 = ctx.r10.s64 + -28888;
	// bl 0x82845600
	ctx.lr = 0x825DDD68;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20672
	ctx.r4.s64 = ctx.r11.s64 + -20672;
	// addi r3,r10,-28948
	ctx.r3.s64 = ctx.r10.s64 + -28948;
	// bl 0x82845600
	ctx.lr = 0x825DDD7C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20568
	ctx.r4.s64 = ctx.r11.s64 + -20568;
	// addi r3,r10,-28984
	ctx.r3.s64 = ctx.r10.s64 + -28984;
	// bl 0x82845600
	ctx.lr = 0x825DDD90;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13352
	ctx.r4.s64 = ctx.r11.s64 + -13352;
	// addi r3,r10,-29008
	ctx.r3.s64 = ctx.r10.s64 + -29008;
	// bl 0x82845600
	ctx.lr = 0x825DDDA4;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13336
	ctx.r4.s64 = ctx.r11.s64 + -13336;
	// addi r3,r10,-29048
	ctx.r3.s64 = ctx.r10.s64 + -29048;
	// bl 0x82845600
	ctx.lr = 0x825DDDB8;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13320
	ctx.r4.s64 = ctx.r11.s64 + -13320;
	// addi r3,r10,-29068
	ctx.r3.s64 = ctx.r10.s64 + -29068;
	// bl 0x82845600
	ctx.lr = 0x825DDDCC;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13224
	ctx.r4.s64 = ctx.r11.s64 + -13224;
	// addi r3,r10,-29092
	ctx.r3.s64 = ctx.r10.s64 + -29092;
	// bl 0x82845600
	ctx.lr = 0x825DDDE0;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-17200
	ctx.r4.s64 = ctx.r11.s64 + -17200;
	// addi r3,r10,-29120
	ctx.r3.s64 = ctx.r10.s64 + -29120;
	// bl 0x82845600
	ctx.lr = 0x825DDDF4;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13176
	ctx.r4.s64 = ctx.r11.s64 + -13176;
	// addi r3,r10,-29132
	ctx.r3.s64 = ctx.r10.s64 + -29132;
	// bl 0x82845600
	ctx.lr = 0x825DDE08;
	sub_82845600(ctx, base);
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// addi r3,r10,-29160
	ctx.r3.s64 = ctx.r10.s64 + -29160;
	// addi r4,r11,-20504
	ctx.r4.s64 = ctx.r11.s64 + -20504;
	// bl 0x82845600
	ctx.lr = 0x825DDE1C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20480
	ctx.r4.s64 = ctx.r11.s64 + -20480;
	// addi r3,r10,-29184
	ctx.r3.s64 = ctx.r10.s64 + -29184;
	// bl 0x82845600
	ctx.lr = 0x825DDE30;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20416
	ctx.r4.s64 = ctx.r11.s64 + -20416;
	// addi r3,r10,-29208
	ctx.r3.s64 = ctx.r10.s64 + -29208;
	// bl 0x82845600
	ctx.lr = 0x825DDE44;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20352
	ctx.r4.s64 = ctx.r11.s64 + -20352;
	// addi r3,r10,-29228
	ctx.r3.s64 = ctx.r10.s64 + -29228;
	// bl 0x82845600
	ctx.lr = 0x825DDE58;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20312
	ctx.r4.s64 = ctx.r11.s64 + -20312;
	// addi r3,r10,-29240
	ctx.r3.s64 = ctx.r10.s64 + -29240;
	// bl 0x82845600
	ctx.lr = 0x825DDE6C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20296
	ctx.r4.s64 = ctx.r11.s64 + -20296;
	// addi r3,r10,-29264
	ctx.r3.s64 = ctx.r10.s64 + -29264;
	// bl 0x82845600
	ctx.lr = 0x825DDE80;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13160
	ctx.r4.s64 = ctx.r11.s64 + -13160;
	// addi r3,r10,-29284
	ctx.r3.s64 = ctx.r10.s64 + -29284;
	// bl 0x82845600
	ctx.lr = 0x825DDE94;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20280
	ctx.r4.s64 = ctx.r11.s64 + -20280;
	// addi r3,r10,-29308
	ctx.r3.s64 = ctx.r10.s64 + -29308;
	// bl 0x82845600
	ctx.lr = 0x825DDEA8;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20256
	ctx.r4.s64 = ctx.r11.s64 + -20256;
	// addi r3,r10,-29328
	ctx.r3.s64 = ctx.r10.s64 + -29328;
	// bl 0x82845600
	ctx.lr = 0x825DDEBC;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13080
	ctx.r4.s64 = ctx.r11.s64 + -13080;
	// addi r3,r10,-29344
	ctx.r3.s64 = ctx.r10.s64 + -29344;
	// bl 0x82845600
	ctx.lr = 0x825DDED0;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13064
	ctx.r4.s64 = ctx.r11.s64 + -13064;
	// addi r3,r10,-29372
	ctx.r3.s64 = ctx.r10.s64 + -29372;
	// bl 0x82845600
	ctx.lr = 0x825DDEE4;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12968
	ctx.r4.s64 = ctx.r11.s64 + -12968;
	// addi r3,r10,-29404
	ctx.r3.s64 = ctx.r10.s64 + -29404;
	// bl 0x82845600
	ctx.lr = 0x825DDEF8;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20216
	ctx.r4.s64 = ctx.r11.s64 + -20216;
	// addi r3,r10,-29424
	ctx.r3.s64 = ctx.r10.s64 + -29424;
	// bl 0x82845600
	ctx.lr = 0x825DDF0C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20152
	ctx.r4.s64 = ctx.r11.s64 + -20152;
	// addi r3,r10,-29448
	ctx.r3.s64 = ctx.r10.s64 + -29448;
	// bl 0x82845600
	ctx.lr = 0x825DDF20;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20088
	ctx.r4.s64 = ctx.r11.s64 + -20088;
	// addi r3,r10,-29472
	ctx.r3.s64 = ctx.r10.s64 + -29472;
	// bl 0x82845600
	ctx.lr = 0x825DDF34;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11592
	ctx.r4.s64 = ctx.r11.s64 + -11592;
	// addi r3,r10,-29500
	ctx.r3.s64 = ctx.r10.s64 + -29500;
	// bl 0x82845600
	ctx.lr = 0x825DDF48;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20016
	ctx.r4.s64 = ctx.r11.s64 + -20016;
	// addi r3,r10,-29528
	ctx.r3.s64 = ctx.r10.s64 + -29528;
	// bl 0x82845600
	ctx.lr = 0x825DDF5C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19952
	ctx.r4.s64 = ctx.r11.s64 + -19952;
	// addi r3,r10,-29552
	ctx.r3.s64 = ctx.r10.s64 + -29552;
	// bl 0x82845600
	ctx.lr = 0x825DDF70;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19888
	ctx.r4.s64 = ctx.r11.s64 + -19888;
	// addi r3,r10,-29580
	ctx.r3.s64 = ctx.r10.s64 + -29580;
	// bl 0x82845600
	ctx.lr = 0x825DDF84;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19888
	ctx.r4.s64 = ctx.r11.s64 + -19888;
	// addi r3,r10,-29608
	ctx.r3.s64 = ctx.r10.s64 + -29608;
	// bl 0x82845600
	ctx.lr = 0x825DDF98;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19888
	ctx.r4.s64 = ctx.r11.s64 + -19888;
	// addi r3,r10,-29640
	ctx.r3.s64 = ctx.r10.s64 + -29640;
	// bl 0x82845600
	ctx.lr = 0x825DDFAC;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19888
	ctx.r4.s64 = ctx.r11.s64 + -19888;
	// addi r3,r10,-29672
	ctx.r3.s64 = ctx.r10.s64 + -29672;
	// bl 0x82845600
	ctx.lr = 0x825DDFC0;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12872
	ctx.r4.s64 = ctx.r11.s64 + -12872;
	// addi r3,r10,-29692
	ctx.r3.s64 = ctx.r10.s64 + -29692;
	// bl 0x82845600
	ctx.lr = 0x825DDFD4;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12856
	ctx.r4.s64 = ctx.r11.s64 + -12856;
	// addi r3,r10,-29712
	ctx.r3.s64 = ctx.r10.s64 + -29712;
	// bl 0x82845600
	ctx.lr = 0x825DDFE8;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12840
	ctx.r4.s64 = ctx.r11.s64 + -12840;
	// addi r3,r10,-29732
	ctx.r3.s64 = ctx.r10.s64 + -29732;
	// bl 0x82845600
	ctx.lr = 0x825DDFFC;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12824
	ctx.r4.s64 = ctx.r11.s64 + -12824;
	// addi r3,r10,-29752
	ctx.r3.s64 = ctx.r10.s64 + -29752;
	// bl 0x82845600
	ctx.lr = 0x825DE010;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19776
	ctx.r4.s64 = ctx.r11.s64 + -19776;
	// addi r3,r10,-29780
	ctx.r3.s64 = ctx.r10.s64 + -29780;
	// bl 0x82845600
	ctx.lr = 0x825DE024;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12808
	ctx.r4.s64 = ctx.r11.s64 + -12808;
	// addi r3,r10,-29800
	ctx.r3.s64 = ctx.r10.s64 + -29800;
	// bl 0x82845600
	ctx.lr = 0x825DE038;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12808
	ctx.r4.s64 = ctx.r11.s64 + -12808;
	// addi r3,r10,-29820
	ctx.r3.s64 = ctx.r10.s64 + -29820;
	// bl 0x82845600
	ctx.lr = 0x825DE04C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-17168
	ctx.r4.s64 = ctx.r11.s64 + -17168;
	// addi r3,r10,-29844
	ctx.r3.s64 = ctx.r10.s64 + -29844;
	// bl 0x82845600
	ctx.lr = 0x825DE060;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11488
	ctx.r4.s64 = ctx.r11.s64 + -11488;
	// addi r3,r10,-29860
	ctx.r3.s64 = ctx.r10.s64 + -29860;
	// bl 0x82845600
	ctx.lr = 0x825DE074;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12784
	ctx.r4.s64 = ctx.r11.s64 + -12784;
	// addi r3,r10,-29884
	ctx.r3.s64 = ctx.r10.s64 + -29884;
	// bl 0x82845600
	ctx.lr = 0x825DE088;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19760
	ctx.r4.s64 = ctx.r11.s64 + -19760;
	// addi r3,r10,-29900
	ctx.r3.s64 = ctx.r10.s64 + -29900;
	// bl 0x82845600
	ctx.lr = 0x825DE09C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-17088
	ctx.r4.s64 = ctx.r11.s64 + -17088;
	// addi r3,r10,-29916
	ctx.r3.s64 = ctx.r10.s64 + -29916;
	// bl 0x82845600
	ctx.lr = 0x825DE0B0;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12760
	ctx.r4.s64 = ctx.r11.s64 + -12760;
	// addi r3,r10,-29948
	ctx.r3.s64 = ctx.r10.s64 + -29948;
	// bl 0x82845600
	ctx.lr = 0x825DE0C4;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12664
	ctx.r4.s64 = ctx.r11.s64 + -12664;
	// addi r3,r10,-29976
	ctx.r3.s64 = ctx.r10.s64 + -29976;
	// bl 0x82845600
	ctx.lr = 0x825DE0D8;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19696
	ctx.r4.s64 = ctx.r11.s64 + -19696;
	// addi r3,r10,-29996
	ctx.r3.s64 = ctx.r10.s64 + -29996;
	// bl 0x82845600
	ctx.lr = 0x825DE0EC;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12568
	ctx.r4.s64 = ctx.r11.s64 + -12568;
	// addi r3,r10,-30012
	ctx.r3.s64 = ctx.r10.s64 + -30012;
	// bl 0x82845600
	ctx.lr = 0x825DE100;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19672
	ctx.r4.s64 = ctx.r11.s64 + -19672;
	// addi r3,r10,-30032
	ctx.r3.s64 = ctx.r10.s64 + -30032;
	// bl 0x82845600
	ctx.lr = 0x825DE114;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12552
	ctx.r4.s64 = ctx.r11.s64 + -12552;
	// addi r3,r10,-30048
	ctx.r3.s64 = ctx.r10.s64 + -30048;
	// bl 0x82845600
	ctx.lr = 0x825DE128;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19648
	ctx.r4.s64 = ctx.r11.s64 + -19648;
	// addi r3,r10,-30068
	ctx.r3.s64 = ctx.r10.s64 + -30068;
	// bl 0x82845600
	ctx.lr = 0x825DE13C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19584
	ctx.r4.s64 = ctx.r11.s64 + -19584;
	// addi r3,r10,-30100
	ctx.r3.s64 = ctx.r10.s64 + -30100;
	// bl 0x82845600
	ctx.lr = 0x825DE150;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19544
	ctx.r4.s64 = ctx.r11.s64 + -19544;
	// addi r3,r10,-30112
	ctx.r3.s64 = ctx.r10.s64 + -30112;
	// bl 0x82845600
	ctx.lr = 0x825DE164;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19504
	ctx.r4.s64 = ctx.r11.s64 + -19504;
	// addi r3,r10,-30128
	ctx.r3.s64 = ctx.r10.s64 + -30128;
	// bl 0x82845600
	ctx.lr = 0x825DE178;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19464
	ctx.r4.s64 = ctx.r11.s64 + -19464;
	// addi r3,r10,-30152
	ctx.r3.s64 = ctx.r10.s64 + -30152;
	// bl 0x82845600
	ctx.lr = 0x825DE18C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19424
	ctx.r4.s64 = ctx.r11.s64 + -19424;
	// addi r3,r10,-30172
	ctx.r3.s64 = ctx.r10.s64 + -30172;
	// bl 0x82845600
	ctx.lr = 0x825DE1A0;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19344
	ctx.r4.s64 = ctx.r11.s64 + -19344;
	// addi r3,r10,-30196
	ctx.r3.s64 = ctx.r10.s64 + -30196;
	// bl 0x82845600
	ctx.lr = 0x825DE1B4;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19312
	ctx.r4.s64 = ctx.r11.s64 + -19312;
	// addi r3,r10,-30216
	ctx.r3.s64 = ctx.r10.s64 + -30216;
	// bl 0x82845600
	ctx.lr = 0x825DE1C8;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19288
	ctx.r4.s64 = ctx.r11.s64 + -19288;
	// addi r3,r10,-30236
	ctx.r3.s64 = ctx.r10.s64 + -30236;
	// bl 0x82845600
	ctx.lr = 0x825DE1DC;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19264
	ctx.r4.s64 = ctx.r11.s64 + -19264;
	// addi r3,r10,-30248
	ctx.r3.s64 = ctx.r10.s64 + -30248;
	// bl 0x82845600
	ctx.lr = 0x825DE1F0;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12536
	ctx.r4.s64 = ctx.r11.s64 + -12536;
	// addi r3,r10,-30276
	ctx.r3.s64 = ctx.r10.s64 + -30276;
	// bl 0x82845600
	ctx.lr = 0x825DE204;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19160
	ctx.r4.s64 = ctx.r11.s64 + -19160;
	// addi r3,r10,-30300
	ctx.r3.s64 = ctx.r10.s64 + -30300;
	// bl 0x82845600
	ctx.lr = 0x825DE218;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19120
	ctx.r4.s64 = ctx.r11.s64 + -19120;
	// addi r3,r10,-30316
	ctx.r3.s64 = ctx.r10.s64 + -30316;
	// bl 0x82845600
	ctx.lr = 0x825DE22C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-24304
	ctx.r4.s64 = ctx.r11.s64 + -24304;
	// addi r3,r10,-30336
	ctx.r3.s64 = ctx.r10.s64 + -30336;
	// bl 0x82845600
	ctx.lr = 0x825DE240;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19056
	ctx.r4.s64 = ctx.r11.s64 + -19056;
	// addi r3,r10,-30348
	ctx.r3.s64 = ctx.r10.s64 + -30348;
	// bl 0x82845600
	ctx.lr = 0x825DE254;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19032
	ctx.r4.s64 = ctx.r11.s64 + -19032;
	// addi r3,r10,-30376
	ctx.r3.s64 = ctx.r10.s64 + -30376;
	// bl 0x82845600
	ctx.lr = 0x825DE268;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12424
	ctx.r4.s64 = ctx.r11.s64 + -12424;
	// addi r3,r10,-30412
	ctx.r3.s64 = ctx.r10.s64 + -30412;
	// bl 0x82845600
	ctx.lr = 0x825DE27C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12280
	ctx.r4.s64 = ctx.r11.s64 + -12280;
	// addi r3,r10,-30436
	ctx.r3.s64 = ctx.r10.s64 + -30436;
	// bl 0x82845600
	ctx.lr = 0x825DE290;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12176
	ctx.r4.s64 = ctx.r11.s64 + -12176;
	// addi r3,r10,-30468
	ctx.r3.s64 = ctx.r10.s64 + -30468;
	// bl 0x82845600
	ctx.lr = 0x825DE2A4;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19008
	ctx.r4.s64 = ctx.r11.s64 + -19008;
	// addi r3,r10,-30492
	ctx.r3.s64 = ctx.r10.s64 + -30492;
	// bl 0x82845600
	ctx.lr = 0x825DE2B8;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-24248
	ctx.r4.s64 = ctx.r11.s64 + -24248;
	// addi r3,r10,-30532
	ctx.r3.s64 = ctx.r10.s64 + -30532;
	// bl 0x82845600
	ctx.lr = 0x825DE2CC;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-24232
	ctx.r4.s64 = ctx.r11.s64 + -24232;
	// addi r3,r10,-30580
	ctx.r3.s64 = ctx.r10.s64 + -30580;
	// bl 0x82845600
	ctx.lr = 0x825DE2E0;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-24216
	ctx.r4.s64 = ctx.r11.s64 + -24216;
	// addi r3,r10,-30616
	ctx.r3.s64 = ctx.r10.s64 + -30616;
	// bl 0x82845600
	ctx.lr = 0x825DE2F4;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12152
	ctx.r4.s64 = ctx.r11.s64 + -12152;
	// addi r3,r10,-30660
	ctx.r3.s64 = ctx.r10.s64 + -30660;
	// bl 0x82845600
	ctx.lr = 0x825DE308;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12024
	ctx.r4.s64 = ctx.r11.s64 + -12024;
	// addi r3,r10,-30684
	ctx.r3.s64 = ctx.r10.s64 + -30684;
	// bl 0x82845600
	ctx.lr = 0x825DE31C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// addi r4,r11,-11920
	ctx.r4.s64 = ctx.r11.s64 + -11920;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r3,r11,-30732
	ctx.r3.s64 = ctx.r11.s64 + -30732;
	// bl 0x82845600
	ctx.lr = 0x825DE330;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18968
	ctx.r4.s64 = ctx.r11.s64 + -18968;
	// addi r3,r10,-30756
	ctx.r3.s64 = ctx.r10.s64 + -30756;
	// bl 0x82845600
	ctx.lr = 0x825DE344;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18944
	ctx.r4.s64 = ctx.r11.s64 + -18944;
	// addi r3,r10,-30776
	ctx.r3.s64 = ctx.r10.s64 + -30776;
	// bl 0x82845600
	ctx.lr = 0x825DE358;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18896
	ctx.r4.s64 = ctx.r11.s64 + -18896;
	// addi r3,r10,-30792
	ctx.r3.s64 = ctx.r10.s64 + -30792;
	// bl 0x82845600
	ctx.lr = 0x825DE36C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18864
	ctx.r4.s64 = ctx.r11.s64 + -18864;
	// addi r3,r10,-30808
	ctx.r3.s64 = ctx.r10.s64 + -30808;
	// bl 0x82845600
	ctx.lr = 0x825DE380;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-24200
	ctx.r4.s64 = ctx.r11.s64 + -24200;
	// addi r3,r10,-30840
	ctx.r3.s64 = ctx.r10.s64 + -30840;
	// bl 0x82845600
	ctx.lr = 0x825DE394;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18832
	ctx.r4.s64 = ctx.r11.s64 + -18832;
	// addi r3,r10,-30868
	ctx.r3.s64 = ctx.r10.s64 + -30868;
	// bl 0x82845600
	ctx.lr = 0x825DE3A8;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-24184
	ctx.r4.s64 = ctx.r11.s64 + -24184;
	// addi r3,r10,-30900
	ctx.r3.s64 = ctx.r10.s64 + -30900;
	// bl 0x82845600
	ctx.lr = 0x825DE3BC;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11880
	ctx.r4.s64 = ctx.r11.s64 + -11880;
	// addi r3,r10,-30928
	ctx.r3.s64 = ctx.r10.s64 + -30928;
	// bl 0x82845600
	ctx.lr = 0x825DE3D0;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18784
	ctx.r4.s64 = ctx.r11.s64 + -18784;
	// addi r3,r10,-30956
	ctx.r3.s64 = ctx.r10.s64 + -30956;
	// bl 0x82845600
	ctx.lr = 0x825DE3E4;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18760
	ctx.r4.s64 = ctx.r11.s64 + -18760;
	// addi r3,r10,-30980
	ctx.r3.s64 = ctx.r10.s64 + -30980;
	// bl 0x82845600
	ctx.lr = 0x825DE3F8;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-24168
	ctx.r4.s64 = ctx.r11.s64 + -24168;
	// addi r3,r10,-31008
	ctx.r3.s64 = ctx.r10.s64 + -31008;
	// bl 0x82845600
	ctx.lr = 0x825DE40C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11864
	ctx.r4.s64 = ctx.r11.s64 + -11864;
	// addi r3,r10,-31040
	ctx.r3.s64 = ctx.r10.s64 + -31040;
	// bl 0x82845600
	ctx.lr = 0x825DE420;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18720
	ctx.r4.s64 = ctx.r11.s64 + -18720;
	// addi r3,r10,-31064
	ctx.r3.s64 = ctx.r10.s64 + -31064;
	// bl 0x82845600
	ctx.lr = 0x825DE434;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11384
	ctx.r4.s64 = ctx.r11.s64 + -11384;
	// addi r3,r10,-31092
	ctx.r3.s64 = ctx.r10.s64 + -31092;
	// bl 0x82845600
	ctx.lr = 0x825DE448;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18704
	ctx.r4.s64 = ctx.r11.s64 + -18704;
	// addi r3,r10,-31112
	ctx.r3.s64 = ctx.r10.s64 + -31112;
	// bl 0x82845600
	ctx.lr = 0x825DE45C;
	sub_82845600(ctx, base);
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// addi r3,r10,-31128
	ctx.r3.s64 = ctx.r10.s64 + -31128;
	// addi r4,r11,-18672
	ctx.r4.s64 = ctx.r11.s64 + -18672;
	// bl 0x82845600
	ctx.lr = 0x825DE470;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11768
	ctx.r4.s64 = ctx.r11.s64 + -11768;
	// addi r3,r10,-31152
	ctx.r3.s64 = ctx.r10.s64 + -31152;
	// bl 0x82845600
	ctx.lr = 0x825DE484;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18640
	ctx.r4.s64 = ctx.r11.s64 + -18640;
	// addi r3,r10,-31192
	ctx.r3.s64 = ctx.r10.s64 + -31192;
	// bl 0x82845600
	ctx.lr = 0x825DE498;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18584
	ctx.r4.s64 = ctx.r11.s64 + -18584;
	// addi r3,r10,-31236
	ctx.r3.s64 = ctx.r10.s64 + -31236;
	// bl 0x82845600
	ctx.lr = 0x825DE4AC;
	sub_82845600(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13680
	ctx.r4.s64 = ctx.r11.s64 + -13680;
	// addi r3,r10,-31268
	ctx.r3.s64 = ctx.r10.s64 + -31268;
	// bl 0x82845600
	ctx.lr = 0x825DE4C0;
	sub_82845600(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13680
	ctx.r4.s64 = ctx.r11.s64 + -13680;
	// addi r3,r10,-31296
	ctx.r3.s64 = ctx.r10.s64 + -31296;
	// bl 0x82845600
	ctx.lr = 0x825DE4D4;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-24144
	ctx.r4.s64 = ctx.r11.s64 + -24144;
	// addi r3,r10,-31324
	ctx.r3.s64 = ctx.r10.s64 + -31324;
	// bl 0x82845600
	ctx.lr = 0x825DE4E8;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11752
	ctx.r4.s64 = ctx.r11.s64 + -11752;
	// addi r3,r10,-31352
	ctx.r3.s64 = ctx.r10.s64 + -31352;
	// bl 0x82845600
	ctx.lr = 0x825DE4FC;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18528
	ctx.r4.s64 = ctx.r11.s64 + -18528;
	// addi r3,r10,-31376
	ctx.r3.s64 = ctx.r10.s64 + -31376;
	// bl 0x82845600
	ctx.lr = 0x825DE510;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11672
	ctx.r4.s64 = ctx.r11.s64 + -11672;
	// addi r3,r10,-31400
	ctx.r3.s64 = ctx.r10.s64 + -31400;
	// bl 0x82845600
	ctx.lr = 0x825DE524;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18504
	ctx.r4.s64 = ctx.r11.s64 + -18504;
	// addi r3,r10,-31428
	ctx.r3.s64 = ctx.r10.s64 + -31428;
	// bl 0x82845600
	ctx.lr = 0x825DE538;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18480
	ctx.r4.s64 = ctx.r11.s64 + -18480;
	// addi r3,r10,-31456
	ctx.r3.s64 = ctx.r10.s64 + -31456;
	// bl 0x82845600
	ctx.lr = 0x825DE54C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18448
	ctx.r4.s64 = ctx.r11.s64 + -18448;
	// addi r3,r10,-31484
	ctx.r3.s64 = ctx.r10.s64 + -31484;
	// bl 0x82845600
	ctx.lr = 0x825DE560;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18424
	ctx.r4.s64 = ctx.r11.s64 + -18424;
	// addi r3,r10,-31512
	ctx.r3.s64 = ctx.r10.s64 + -31512;
	// bl 0x82845600
	ctx.lr = 0x825DE574;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18400
	ctx.r4.s64 = ctx.r11.s64 + -18400;
	// addi r3,r10,-31540
	ctx.r3.s64 = ctx.r10.s64 + -31540;
	// bl 0x82845600
	ctx.lr = 0x825DE588;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11656
	ctx.r4.s64 = ctx.r11.s64 + -11656;
	// addi r3,r10,-31568
	ctx.r3.s64 = ctx.r10.s64 + -31568;
	// bl 0x82845600
	ctx.lr = 0x825DE59C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18376
	ctx.r4.s64 = ctx.r11.s64 + -18376;
	// addi r3,r10,-31596
	ctx.r3.s64 = ctx.r10.s64 + -31596;
	// bl 0x82845600
	ctx.lr = 0x825DE5B0;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18352
	ctx.r4.s64 = ctx.r11.s64 + -18352;
	// addi r3,r10,-31620
	ctx.r3.s64 = ctx.r10.s64 + -31620;
	// bl 0x82845600
	ctx.lr = 0x825DE5C4;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-24136
	ctx.r4.s64 = ctx.r11.s64 + -24136;
	// addi r3,r10,-31648
	ctx.r3.s64 = ctx.r10.s64 + -31648;
	// bl 0x82845600
	ctx.lr = 0x825DE5D8;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18320
	ctx.r4.s64 = ctx.r11.s64 + -18320;
	// addi r3,r10,-31672
	ctx.r3.s64 = ctx.r10.s64 + -31672;
	// bl 0x82845600
	ctx.lr = 0x825DE5EC;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18288
	ctx.r4.s64 = ctx.r11.s64 + -18288;
	// addi r3,r10,-31716
	ctx.r3.s64 = ctx.r10.s64 + -31716;
	// bl 0x82845600
	ctx.lr = 0x825DE600;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18256
	ctx.r4.s64 = ctx.r11.s64 + -18256;
	// addi r3,r10,-31740
	ctx.r3.s64 = ctx.r10.s64 + -31740;
	// bl 0x82845600
	ctx.lr = 0x825DE614;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18176
	ctx.r4.s64 = ctx.r11.s64 + -18176;
	// addi r3,r10,-31756
	ctx.r3.s64 = ctx.r10.s64 + -31756;
	// bl 0x82845600
	ctx.lr = 0x825DE628;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11624
	ctx.r4.s64 = ctx.r11.s64 + -11624;
	// addi r3,r10,-31776
	ctx.r3.s64 = ctx.r10.s64 + -31776;
	// bl 0x82845600
	ctx.lr = 0x825DE63C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18120
	ctx.r4.s64 = ctx.r11.s64 + -18120;
	// addi r3,r10,-31804
	ctx.r3.s64 = ctx.r10.s64 + -31804;
	// bl 0x82845600
	ctx.lr = 0x825DE650;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18056
	ctx.r4.s64 = ctx.r11.s64 + -18056;
	// addi r3,r10,-31840
	ctx.r3.s64 = ctx.r10.s64 + -31840;
	// bl 0x82845600
	ctx.lr = 0x825DE664;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-24120
	ctx.r4.s64 = ctx.r11.s64 + -24120;
	// addi r3,r10,-31872
	ctx.r3.s64 = ctx.r10.s64 + -31872;
	// bl 0x82845600
	ctx.lr = 0x825DE678;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18032
	ctx.r4.s64 = ctx.r11.s64 + -18032;
	// addi r3,r10,-31896
	ctx.r3.s64 = ctx.r10.s64 + -31896;
	// bl 0x82845600
	ctx.lr = 0x825DE68C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-17992
	ctx.r4.s64 = ctx.r11.s64 + -17992;
	// addi r3,r10,-31912
	ctx.r3.s64 = ctx.r10.s64 + -31912;
	// bl 0x82845600
	ctx.lr = 0x825DE6A0;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-17936
	ctx.r4.s64 = ctx.r11.s64 + -17936;
	// addi r3,r10,-31940
	ctx.r3.s64 = ctx.r10.s64 + -31940;
	// bl 0x82845600
	ctx.lr = 0x825DE6B4;
	sub_82845600(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DE6C8);
PPC_WEAK_FUNC(sub_825DE6C8) { __imp__sub_825DE6C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DE6C8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r3,0
	ctx.r3.s64 = 0;
	// lfs f0,2612(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,8(r4)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r4.u32 + 8, temp.u32);
	// stfs f0,4(r4)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r4.u32 + 4, temp.u32);
	// stfs f0,0(r4)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r4.u32 + 0, temp.u32);
	// stfs f0,0(r5)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r5.u32 + 0, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DE6E8);
PPC_WEAK_FUNC(sub_825DE6E8) { __imp__sub_825DE6E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DE6E8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DE6F8);
PPC_WEAK_FUNC(sub_825DE6F8) { __imp__sub_825DE6F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DE6F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32057
	ctx.r11.s64 = -2100887552;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,-15688(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -15688);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DE710);
PPC_WEAK_FUNC(sub_825DE710) { __imp__sub_825DE710(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DE710) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,-17284(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -17284);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DE728);
PPC_WEAK_FUNC(sub_825DE728) { __imp__sub_825DE728(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DE728) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,-17280(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -17280);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DE740);
PPC_WEAK_FUNC(sub_825DE740) { __imp__sub_825DE740(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DE740) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// addi r11,r11,-17276
	ctx.r11.s64 = ctx.r11.s64 + -17276;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DE758);
PPC_WEAK_FUNC(sub_825DE758) { __imp__sub_825DE758(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DE758) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r4
	ctx.r31.u64 = ctx.r4.u64;
	// mr r9,r3
	ctx.r9.u64 = ctx.r3.u64;
	// lwz r11,12(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 12);
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// rlwinm r8,r11,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwz r11,4(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// lwz r3,0(r10)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// lfs f0,8(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// stwx r11,r8,r31
	PPC_STORE_U32(ctx.r8.u32 + ctx.r31.u32, ctx.r11.u32);
	// lfs f13,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,0(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f12.f64 = double(temp.f32);
	// lwz r11,12(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 12);
	// addi r11,r11,2
	ctx.r11.s64 = ctx.r11.s64 + 2;
	// rlwinm r11,r11,4,0,27
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 4) & 0xFFFFFFF0;
	// add r11,r11,r31
	ctx.r11.u64 = ctx.r11.u64 + ctx.r31.u64;
	// stfs f12,0(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f13,4(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// lwz r11,12(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 12);
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// addi r8,r11,1
	ctx.r8.s64 = ctx.r11.s64 + 1;
	// addi r11,r11,2
	ctx.r11.s64 = ctx.r11.s64 + 2;
	// rlwinm r11,r11,4,0,27
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 4) & 0xFFFFFFF0;
	// stw r8,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r8.u32);
	// add r4,r11,r31
	ctx.r4.u64 = ctx.r11.u64 + ctx.r31.u64;
	// lwz r5,8(r10)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r10.u32 + 8);
	// mtctr r9
	ctx.ctr.u64 = ctx.r9.u64;
	// bctrl 
	ctx.lr = 0x825DE7DC;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DE800);
PPC_WEAK_FUNC(sub_825DE800) { __imp__sub_825DE800(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DE800) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,-6456
	ctx.r3.s64 = ctx.r11.s64 + -6456;
	// b 0x825de758
	sub_825DE758(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DE810);
PPC_WEAK_FUNC(sub_825DE810) { __imp__sub_825DE810(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DE810) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13680
	ctx.r4.s64 = ctx.r11.s64 + -13680;
	// addi r3,r10,-24760
	ctx.r3.s64 = ctx.r10.s64 + -24760;
	// bl 0x82845600
	ctx.lr = 0x825DE830;
	sub_82845600(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13680
	ctx.r4.s64 = ctx.r11.s64 + -13680;
	// addi r3,r10,-24772
	ctx.r3.s64 = ctx.r10.s64 + -24772;
	// bl 0x82845600
	ctx.lr = 0x825DE844;
	sub_82845600(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13680
	ctx.r4.s64 = ctx.r11.s64 + -13680;
	// addi r3,r10,-24796
	ctx.r3.s64 = ctx.r10.s64 + -24796;
	// bl 0x82845600
	ctx.lr = 0x825DE858;
	sub_82845600(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13680
	ctx.r4.s64 = ctx.r11.s64 + -13680;
	// addi r3,r10,-24812
	ctx.r3.s64 = ctx.r10.s64 + -24812;
	// bl 0x82845600
	ctx.lr = 0x825DE86C;
	sub_82845600(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13680
	ctx.r4.s64 = ctx.r11.s64 + -13680;
	// addi r3,r10,-24828
	ctx.r3.s64 = ctx.r10.s64 + -24828;
	// bl 0x82845600
	ctx.lr = 0x825DE880;
	sub_82845600(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13680
	ctx.r4.s64 = ctx.r11.s64 + -13680;
	// addi r3,r10,-24848
	ctx.r3.s64 = ctx.r10.s64 + -24848;
	// bl 0x82845600
	ctx.lr = 0x825DE894;
	sub_82845600(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13680
	ctx.r4.s64 = ctx.r11.s64 + -13680;
	// addi r3,r10,-24872
	ctx.r3.s64 = ctx.r10.s64 + -24872;
	// bl 0x82845600
	ctx.lr = 0x825DE8A8;
	sub_82845600(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13680
	ctx.r4.s64 = ctx.r11.s64 + -13680;
	// addi r3,r10,-24900
	ctx.r3.s64 = ctx.r10.s64 + -24900;
	// bl 0x82845600
	ctx.lr = 0x825DE8BC;
	sub_82845600(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13680
	ctx.r4.s64 = ctx.r11.s64 + -13680;
	// addi r3,r10,-24928
	ctx.r3.s64 = ctx.r10.s64 + -24928;
	// bl 0x82845600
	ctx.lr = 0x825DE8D0;
	sub_82845600(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13680
	ctx.r4.s64 = ctx.r11.s64 + -13680;
	// addi r3,r10,-24956
	ctx.r3.s64 = ctx.r10.s64 + -24956;
	// bl 0x82845600
	ctx.lr = 0x825DE8E4;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-6384
	ctx.r4.s64 = ctx.r11.s64 + -6384;
	// addi r3,r10,-24996
	ctx.r3.s64 = ctx.r10.s64 + -24996;
	// bl 0x82845600
	ctx.lr = 0x825DE8F8;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-6384
	ctx.r4.s64 = ctx.r11.s64 + -6384;
	// addi r3,r10,-25024
	ctx.r3.s64 = ctx.r10.s64 + -25024;
	// bl 0x82845600
	ctx.lr = 0x825DE90C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-6384
	ctx.r4.s64 = ctx.r11.s64 + -6384;
	// addi r3,r10,-25056
	ctx.r3.s64 = ctx.r10.s64 + -25056;
	// bl 0x82845600
	ctx.lr = 0x825DE920;
	sub_82845600(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13680
	ctx.r4.s64 = ctx.r11.s64 + -13680;
	// addi r3,r10,-25076
	ctx.r3.s64 = ctx.r10.s64 + -25076;
	// bl 0x82845600
	ctx.lr = 0x825DE934;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-6424
	ctx.r4.s64 = ctx.r11.s64 + -6424;
	// addi r3,r10,-25096
	ctx.r3.s64 = ctx.r10.s64 + -25096;
	// bl 0x82845600
	ctx.lr = 0x825DE948;
	sub_82845600(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13680
	ctx.r4.s64 = ctx.r11.s64 + -13680;
	// addi r3,r10,-25116
	ctx.r3.s64 = ctx.r10.s64 + -25116;
	// bl 0x82845600
	ctx.lr = 0x825DE95C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-6424
	ctx.r4.s64 = ctx.r11.s64 + -6424;
	// addi r3,r10,-25136
	ctx.r3.s64 = ctx.r10.s64 + -25136;
	// bl 0x82845600
	ctx.lr = 0x825DE970;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-6424
	ctx.r4.s64 = ctx.r11.s64 + -6424;
	// addi r3,r10,-25160
	ctx.r3.s64 = ctx.r10.s64 + -25160;
	// bl 0x82845600
	ctx.lr = 0x825DE984;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-6424
	ctx.r4.s64 = ctx.r11.s64 + -6424;
	// addi r3,r10,-25184
	ctx.r3.s64 = ctx.r10.s64 + -25184;
	// bl 0x82845600
	ctx.lr = 0x825DE998;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-6424
	ctx.r4.s64 = ctx.r11.s64 + -6424;
	// addi r3,r10,-25212
	ctx.r3.s64 = ctx.r10.s64 + -25212;
	// bl 0x82845600
	ctx.lr = 0x825DE9AC;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-6424
	ctx.r4.s64 = ctx.r11.s64 + -6424;
	// addi r3,r10,-25232
	ctx.r3.s64 = ctx.r10.s64 + -25232;
	// bl 0x82845600
	ctx.lr = 0x825DE9C0;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-6424
	ctx.r4.s64 = ctx.r11.s64 + -6424;
	// addi r3,r10,-25252
	ctx.r3.s64 = ctx.r10.s64 + -25252;
	// bl 0x82845600
	ctx.lr = 0x825DE9D4;
	sub_82845600(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13680
	ctx.r4.s64 = ctx.r11.s64 + -13680;
	// addi r3,r10,-25272
	ctx.r3.s64 = ctx.r10.s64 + -25272;
	// bl 0x82845600
	ctx.lr = 0x825DE9E8;
	sub_82845600(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13680
	ctx.r4.s64 = ctx.r11.s64 + -13680;
	// addi r3,r10,-25288
	ctx.r3.s64 = ctx.r10.s64 + -25288;
	// bl 0x82845600
	ctx.lr = 0x825DE9FC;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-6424
	ctx.r4.s64 = ctx.r11.s64 + -6424;
	// addi r3,r10,-25312
	ctx.r3.s64 = ctx.r10.s64 + -25312;
	// bl 0x82845600
	ctx.lr = 0x825DEA10;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-6424
	ctx.r4.s64 = ctx.r11.s64 + -6424;
	// addi r3,r10,-25332
	ctx.r3.s64 = ctx.r10.s64 + -25332;
	// bl 0x82845600
	ctx.lr = 0x825DEA24;
	sub_82845600(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13680
	ctx.r4.s64 = ctx.r11.s64 + -13680;
	// addi r3,r10,-25356
	ctx.r3.s64 = ctx.r10.s64 + -25356;
	// bl 0x82845600
	ctx.lr = 0x825DEA38;
	sub_82845600(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13680
	ctx.r4.s64 = ctx.r11.s64 + -13680;
	// addi r3,r10,-25380
	ctx.r3.s64 = ctx.r10.s64 + -25380;
	// bl 0x82845600
	ctx.lr = 0x825DEA4C;
	sub_82845600(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13680
	ctx.r4.s64 = ctx.r11.s64 + -13680;
	// addi r3,r10,-25400
	ctx.r3.s64 = ctx.r10.s64 + -25400;
	// bl 0x82845600
	ctx.lr = 0x825DEA60;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-6424
	ctx.r4.s64 = ctx.r11.s64 + -6424;
	// addi r3,r10,-25420
	ctx.r3.s64 = ctx.r10.s64 + -25420;
	// bl 0x82845600
	ctx.lr = 0x825DEA74;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-6424
	ctx.r4.s64 = ctx.r11.s64 + -6424;
	// addi r3,r10,-25436
	ctx.r3.s64 = ctx.r10.s64 + -25436;
	// bl 0x82845600
	ctx.lr = 0x825DEA88;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-6360
	ctx.r4.s64 = ctx.r11.s64 + -6360;
	// addi r3,r10,-25464
	ctx.r3.s64 = ctx.r10.s64 + -25464;
	// bl 0x82845600
	ctx.lr = 0x825DEA9C;
	sub_82845600(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// addi r4,r11,-13680
	ctx.r4.s64 = ctx.r11.s64 + -13680;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r3,r11,-25492
	ctx.r3.s64 = ctx.r11.s64 + -25492;
	// bl 0x82845600
	ctx.lr = 0x825DEAB0;
	sub_82845600(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13680
	ctx.r4.s64 = ctx.r11.s64 + -13680;
	// addi r3,r10,-25512
	ctx.r3.s64 = ctx.r10.s64 + -25512;
	// bl 0x82845600
	ctx.lr = 0x825DEAC4;
	sub_82845600(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13680
	ctx.r4.s64 = ctx.r11.s64 + -13680;
	// addi r3,r10,-25540
	ctx.r3.s64 = ctx.r10.s64 + -25540;
	// bl 0x82845600
	ctx.lr = 0x825DEAD8;
	sub_82845600(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13680
	ctx.r4.s64 = ctx.r11.s64 + -13680;
	// addi r3,r10,-25572
	ctx.r3.s64 = ctx.r10.s64 + -25572;
	// bl 0x82845600
	ctx.lr = 0x825DEAEC;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-6144
	ctx.r4.s64 = ctx.r11.s64 + -6144;
	// addi r3,r10,-25604
	ctx.r3.s64 = ctx.r10.s64 + -25604;
	// bl 0x82845600
	ctx.lr = 0x825DEB00;
	sub_82845600(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13680
	ctx.r4.s64 = ctx.r11.s64 + -13680;
	// addi r3,r10,-25624
	ctx.r3.s64 = ctx.r10.s64 + -25624;
	// bl 0x82845600
	ctx.lr = 0x825DEB14;
	sub_82845600(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13680
	ctx.r4.s64 = ctx.r11.s64 + -13680;
	// addi r3,r10,-25664
	ctx.r3.s64 = ctx.r10.s64 + -25664;
	// bl 0x82845600
	ctx.lr = 0x825DEB28;
	sub_82845600(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13680
	ctx.r4.s64 = ctx.r11.s64 + -13680;
	// addi r3,r10,-25692
	ctx.r3.s64 = ctx.r10.s64 + -25692;
	// bl 0x82845600
	ctx.lr = 0x825DEB3C;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-6336
	ctx.r4.s64 = ctx.r11.s64 + -6336;
	// addi r3,r10,-25736
	ctx.r3.s64 = ctx.r10.s64 + -25736;
	// bl 0x82845600
	ctx.lr = 0x825DEB50;
	sub_82845600(ctx, base);
	// lis r11,-32162
	ctx.r11.s64 = -2107768832;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-6408
	ctx.r4.s64 = ctx.r11.s64 + -6408;
	// addi r3,r10,-25752
	ctx.r3.s64 = ctx.r10.s64 + -25752;
	// bl 0x82845600
	ctx.lr = 0x825DEB64;
	sub_82845600(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DEB78);
PPC_WEAK_FUNC(sub_825DEB78) { __imp__sub_825DEB78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DEB78) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7cc
	ctx.lr = 0x825DEB80;
	__savegprlr_29(ctx, base);
	// stfd f30,-48(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -48, ctx.f30.u64);
	// stfd f31,-40(r1)
	PPC_STORE_U64(ctx.r1.u32 + -40, ctx.f31.u64);
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// stfs f1,80(r1)
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// stfs f2,84(r1)
	temp.f32 = float(ctx.f2.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// addi r31,r11,-9624
	ctx.r31.s64 = ctx.r11.s64 + -9624;
	// stfs f3,88(r1)
	temp.f32 = float(ctx.f3.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// addi r5,r1,96
	ctx.r5.s64 = ctx.r1.s64 + 96;
	// fmr f31,f4
	ctx.f31.f64 = ctx.f4.f64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824efdc8
	ctx.lr = 0x825DEBB8;
	sub_824EFDC8(ctx, base);
	// addi r11,r1,96
	ctx.r11.s64 = ctx.r1.s64 + 96;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// lvx128 v13,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// vsubfp v0,v0,v13
	ctx.fpscr.enableFlushMode();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_sub_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v13.f32)));
	// lfs f0,3444(r11)
	ctx.fpscr.disableFlushModeUnconditional();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3444);
	ctx.f0.f64 = double(temp.f32);
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// fmuls f31,f31,f0
	ctx.f31.f64 = double(float(ctx.f31.f64 * ctx.f0.f64));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lfs f13,88(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 88);
	ctx.f13.f64 = double(temp.f32);
	// fmuls f12,f13,f13
	ctx.f12.f64 = double(float(ctx.f13.f64 * ctx.f13.f64));
	// lfs f0,84(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,80(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f13.f64 = double(temp.f32);
	// fmadds f0,f0,f0,f12
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f0.f64 + ctx.f12.f64));
	// fmadds f0,f13,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 * ctx.f13.f64 + ctx.f0.f64));
	// fsqrts f30,f0
	ctx.f30.f64 = double(float(sqrt(ctx.f0.f64)));
	// fsubs f1,f30,f31
	ctx.f1.f64 = double(float(ctx.f30.f64 - ctx.f31.f64));
	// bl 0x824e96c8
	ctx.lr = 0x825DEC0C;
	sub_824E96C8(ctx, base);
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// fadds f1,f31,f30
	ctx.fpscr.disableFlushMode();
	ctx.f1.f64 = double(float(ctx.f31.f64 + ctx.f30.f64));
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824e9730
	ctx.lr = 0x825DEC20;
	sub_824E9730(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DEC30;
	sub_824ED1C0(ctx, base);
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DEC3C;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// lfd f30,-48(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -48);
	// lfd f31,-40(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -40);
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DEC50);
PPC_WEAK_FUNC(sub_825DEC50) { __imp__sub_825DEC50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DEC50) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r30,r11,-9624
	ctx.r30.s64 = ctx.r11.s64 + -9624;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x824e9cb0
	ctx.lr = 0x825DEC7C;
	sub_824E9CB0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DEC88;
	sub_824ED1C0(ctx, base);
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// lwz r30,8272(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8272);
	// bl 0x821fe708
	ctx.lr = 0x825DEC94;
	sub_821FE708(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// li r5,21
	ctx.r5.s64 = 21;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// bl 0x822d6b60
	ctx.lr = 0x825DECAC;
	sub_822D6B60(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DECC8);
PPC_WEAK_FUNC(sub_825DECC8) { __imp__sub_825DECC8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DECC8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// clrlwi r11,r10,24
	ctx.r11.u64 = ctx.r10.u32 & 0xFF;
	// clrlwi r7,r7,24
	ctx.r7.u64 = ctx.r7.u32 & 0xFF;
	// clrlwi r8,r8,24
	ctx.r8.u64 = ctx.r8.u32 & 0xFF;
	// rlwimi r7,r11,8,16,23
	ctx.r7.u64 = (__builtin_rotateleft32(ctx.r11.u32, 8) & 0xFF00) | (ctx.r7.u64 & 0xFFFFFFFFFFFF00FF);
	// cntlzw r10,r10
	ctx.r10.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// clrlwi r11,r7,16
	ctx.r11.u64 = ctx.r7.u32 & 0xFFFF;
	// clrlwi r9,r9,24
	ctx.r9.u64 = ctx.r9.u32 & 0xFF;
	// rlwimi r8,r11,8,0,23
	ctx.r8.u64 = (__builtin_rotateleft32(ctx.r11.u32, 8) & 0xFFFFFF00) | (ctx.r8.u64 & 0xFFFFFFFF000000FF);
	// rlwinm r11,r10,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// rlwimi r9,r8,8,0,23
	ctx.r9.u64 = (__builtin_rotateleft32(ctx.r8.u32, 8) & 0xFFFFFF00) | (ctx.r9.u64 & 0xFFFFFFFF000000FF);
	// clrlwi r10,r6,24
	ctx.r10.u64 = ctx.r6.u32 & 0xFF;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r9,96(r1)
	PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r9.u32);
	// beq cr6,0x825ded20
	if (ctx.cr6.eq) goto loc_825DED20;
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
loc_825DED20:
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
	// lwz r9,212(r1)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 212);
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// mr r7,r6
	ctx.r7.u64 = ctx.r6.u64;
	// addi r31,r11,-9624
	ctx.r31.s64 = ctx.r11.s64 + -9624;
	// mr r6,r5
	ctx.r6.u64 = ctx.r5.u64;
	// mr r5,r4
	ctx.r5.u64 = ctx.r4.u64;
	// stb r10,95(r1)
	PPC_STORE_U8(ctx.r1.u32 + 95, ctx.r10.u8);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r8,r1,96
	ctx.r8.s64 = ctx.r1.s64 + 96;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed0b0
	ctx.lr = 0x825DED50;
	sub_824ED0B0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DED5C;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DED70);
PPC_WEAK_FUNC(sub_825DED70) { __imp__sub_825DED70(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DED70) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.r30.u64);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// stfs f1,96(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r1.u32 + 96, temp.u32);
	// mr r5,r6
	ctx.r5.u64 = ctx.r6.u64;
	// stfs f2,100(r1)
	temp.f32 = float(ctx.f2.f64);
	PPC_STORE_U32(ctx.r1.u32 + 100, temp.u32);
	// addi r3,r11,-9624
	ctx.r3.s64 = ctx.r11.s64 + -9624;
	// stfs f3,104(r1)
	temp.f32 = float(ctx.f3.f64);
	PPC_STORE_U32(ctx.r1.u32 + 104, temp.u32);
	// addi r6,r1,80
	ctx.r6.s64 = ctx.r1.s64 + 80;
	// addi r4,r1,96
	ctx.r4.s64 = ctx.r1.s64 + 96;
	// mr r31,r7
	ctx.r31.u64 = ctx.r7.u64;
	// mr r30,r8
	ctx.r30.u64 = ctx.r8.u64;
	// bl 0x824ee2a0
	ctx.lr = 0x825DEDB0;
	sub_824EE2A0(ctx, base);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// lfs f12,84(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	ctx.f12.f64 = double(temp.f32);
	// stfs f0,0(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r31.u32 + 0, temp.u32);
	// stfs f12,0(r30)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r30.u32 + 0, temp.u32);
	// lfs f13,2612(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2612);
	ctx.f13.f64 = double(temp.f32);
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// blt cr6,0x825dedf4
	if (ctx.cr6.lt) goto loc_825DEDF4;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f11,3400(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3400);
	ctx.f11.f64 = double(temp.f32);
	// fcmpu cr6,f0,f11
	ctx.cr6.compare(ctx.f0.f64, ctx.f11.f64);
	// bgt cr6,0x825dedf4
	if (ctx.cr6.gt) goto loc_825DEDF4;
	// fcmpu cr6,f12,f13
	ctx.cr6.compare(ctx.f12.f64, ctx.f13.f64);
	// blt cr6,0x825dedf4
	if (ctx.cr6.lt) goto loc_825DEDF4;
	// fcmpu cr6,f12,f11
	ctx.cr6.compare(ctx.f12.f64, ctx.f11.f64);
	// li r11,1
	ctx.r11.s64 = 1;
	// ble cr6,0x825dedf8
	if (!ctx.cr6.gt) goto loc_825DEDF8;
loc_825DEDF4:
	// li r11,0
	ctx.r11.s64 = 0;
loc_825DEDF8:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DEE18);
PPC_WEAK_FUNC(sub_825DEE18) { __imp__sub_825DEE18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DEE18) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// lwz r31,8272(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8272);
	// bl 0x821fe708
	ctx.lr = 0x825DEE34;
	sub_821FE708(ctx, base);
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
	// li r4,21
	ctx.r4.s64 = 21;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x822d6e20
	ctx.lr = 0x825DEE44;
	sub_822D6E20(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DEE58);
PPC_WEAK_FUNC(sub_825DEE58) { __imp__sub_825DEE58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DEE58) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// addi r31,r11,-9624
	ctx.r31.s64 = ctx.r11.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ec240
	ctx.lr = 0x825DEE78;
	sub_824EC240(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DEE84;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DEE98);
PPC_WEAK_FUNC(sub_825DEE98) { __imp__sub_825DEE98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DEE98) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// addi r3,r11,-9624
	ctx.r3.s64 = ctx.r11.s64 + -9624;
	// b 0x824ed2e8
	sub_824ED2E8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DEEA8);
PPC_WEAK_FUNC(sub_825DEEA8) { __imp__sub_825DEEA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DEEA8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// lwz r31,8272(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8272);
	// bl 0x821fe708
	ctx.lr = 0x825DEEC4;
	sub_821FE708(ctx, base);
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
	// li r4,22
	ctx.r4.s64 = 22;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x822d6e20
	ctx.lr = 0x825DEED4;
	sub_822D6E20(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DEEE8);
PPC_WEAK_FUNC(sub_825DEEE8) { __imp__sub_825DEEE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DEEE8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// addi r31,r11,-9624
	ctx.r31.s64 = ctx.r11.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ec630
	ctx.lr = 0x825DEF08;
	sub_824EC630(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DEF14;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DEF28);
PPC_WEAK_FUNC(sub_825DEF28) { __imp__sub_825DEF28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DEF28) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// addi r31,r11,-9624
	ctx.r31.s64 = ctx.r11.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ec808
	ctx.lr = 0x825DEF48;
	sub_824EC808(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DEF54;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DEF68);
PPC_WEAK_FUNC(sub_825DEF68) { __imp__sub_825DEF68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DEF68) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// addi r31,r11,-9624
	ctx.r31.s64 = ctx.r11.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ec9d0
	ctx.lr = 0x825DEF88;
	sub_824EC9D0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DEF94;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DEFA8);
PPC_WEAK_FUNC(sub_825DEFA8) { __imp__sub_825DEFA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DEFA8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// addi r31,r11,-9624
	ctx.r31.s64 = ctx.r11.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ecea8
	ctx.lr = 0x825DEFC8;
	sub_824ECEA8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DEFD4;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DEFE8);
PPC_WEAK_FUNC(sub_825DEFE8) { __imp__sub_825DEFE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DEFE8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31996
	ctx.r10.s64 = -2096889856;
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-27534(r10)
	PPC_STORE_U8(ctx.r10.u32 + -27534, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DEFF8);
PPC_WEAK_FUNC(sub_825DEFF8) { __imp__sub_825DEFF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DEFF8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31996
	ctx.r10.s64 = -2096889856;
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-27536(r10)
	PPC_STORE_U8(ctx.r10.u32 + -27536, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DF008);
PPC_WEAK_FUNC(sub_825DF008) { __imp__sub_825DF008(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DF008) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// addi r31,r11,-9624
	ctx.r31.s64 = ctx.r11.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824eac10
	ctx.lr = 0x825DF028;
	sub_824EAC10(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DF034;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DF048);
PPC_WEAK_FUNC(sub_825DF048) { __imp__sub_825DF048(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DF048) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// addi r31,r11,-9624
	ctx.r31.s64 = ctx.r11.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824eac68
	ctx.lr = 0x825DF068;
	sub_824EAC68(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DF074;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DF088);
PPC_WEAK_FUNC(sub_825DF088) { __imp__sub_825DF088(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DF088) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r10,-31997
	ctx.r10.s64 = -2096955392;
	// li r11,0
	ctx.r11.s64 = 0;
	// stb r11,10964(r10)
	PPC_STORE_U8(ctx.r10.u32 + 10964, ctx.r11.u8);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lis r10,-31997
	ctx.r10.s64 = -2096955392;
	// lfs f0,2612(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// stfs f0,-24000(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -24000, temp.u32);
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// stfs f0,10972(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 10972, temp.u32);
	// li r11,0
	ctx.r11.s64 = 0;
	// stb r11,10965(r10)
	PPC_STORE_U8(ctx.r10.u32 + 10965, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DF0C0);
PPC_WEAK_FUNC(sub_825DF0C0) { __imp__sub_825DF0C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DF0C0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r10,8(r4)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r4.u32 + 8);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// mr r11,r10
	ctx.r11.u64 = ctx.r10.u64;
	// lwz r9,12(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 12);
	// lwz r3,0(r10)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cntlzw r9,r9
	ctx.r9.u64 = ctx.r9.u32 == 0 ? 32 : __builtin_clz(ctx.r9.u32);
	// lwz r4,4(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// lwz r5,8(r10)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r10.u32 + 8);
	// lfs f2,40(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 40);
	ctx.f2.f64 = double(temp.f32);
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lfs f1,36(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	ctx.f1.f64 = double(temp.f32);
	// xori r6,r10,1
	ctx.r6.u64 = ctx.r10.u64 ^ 1;
	// lwz r9,24(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// lwz r10,28(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	// lwz r11,32(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 32);
	// clrlwi r6,r6,24
	ctx.r6.u64 = ctx.r6.u32 & 0xFF;
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// mtctr r31
	ctx.ctr.u64 = ctx.r31.u64;
	// bctrl 
	ctx.lr = 0x825DF124;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DF138);
PPC_WEAK_FUNC(sub_825DF138) { __imp__sub_825DF138(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DF138) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// srawi r10,r4,8
	ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r4.s32 >> 8;
	// clrlwi r8,r4,24
	ctx.r8.u64 = ctx.r4.u32 & 0xFF;
	// lwz r11,21384(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 21384);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825df17c
	if (!ctx.cr6.eq) goto loc_825DF17C;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r5,r11,r9
	ctx.r5.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825df180
	goto loc_825DF180;
loc_825DF17C:
	// li r5,0
	ctx.r5.s64 = 0;
loc_825DF180:
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r31,r11,-9624
	ctx.r31.s64 = ctx.r11.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824e9a80
	ctx.lr = 0x825DF194;
	sub_824E9A80(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DF1A0;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DF1B8);
PPC_WEAK_FUNC(sub_825DF1B8) { __imp__sub_825DF1B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DF1B8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// srawi r10,r4,8
	ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r4.s32 >> 8;
	// clrlwi r8,r4,24
	ctx.r8.u64 = ctx.r4.u32 & 0xFF;
	// lwz r11,1792(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 1792);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825df1fc
	if (!ctx.cr6.eq) goto loc_825DF1FC;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r5,r11,r9
	ctx.r5.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825df200
	goto loc_825DF200;
loc_825DF1FC:
	// li r5,0
	ctx.r5.s64 = 0;
loc_825DF200:
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r31,r11,-9624
	ctx.r31.s64 = ctx.r11.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824e9a80
	ctx.lr = 0x825DF214;
	sub_824E9A80(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DF220;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DF238);
PPC_WEAK_FUNC(sub_825DF238) { __imp__sub_825DF238(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DF238) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32034
	ctx.r11.s64 = -2099380224;
	// srawi r10,r4,8
	ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r4.s32 >> 8;
	// clrlwi r8,r4,24
	ctx.r8.u64 = ctx.r4.u32 & 0xFF;
	// lwz r11,23120(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 23120);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825df27c
	if (!ctx.cr6.eq) goto loc_825DF27C;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r5,r11,r9
	ctx.r5.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825df280
	goto loc_825DF280;
loc_825DF27C:
	// li r5,0
	ctx.r5.s64 = 0;
loc_825DF280:
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r31,r11,-9624
	ctx.r31.s64 = ctx.r11.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824e9a80
	ctx.lr = 0x825DF294;
	sub_824E9A80(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DF2A0;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DF2B8);
PPC_WEAK_FUNC(sub_825DF2B8) { __imp__sub_825DF2B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DF2B8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// srawi r10,r4,8
	ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r4.s32 >> 8;
	// clrlwi r8,r4,24
	ctx.r8.u64 = ctx.r4.u32 & 0xFF;
	// lwz r11,21384(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 21384);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825df2fc
	if (!ctx.cr6.eq) goto loc_825DF2FC;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r5,r11,r9
	ctx.r5.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825df300
	goto loc_825DF300;
loc_825DF2FC:
	// li r5,0
	ctx.r5.s64 = 0;
loc_825DF300:
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r31,r11,-9624
	ctx.r31.s64 = ctx.r11.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824e9870
	ctx.lr = 0x825DF314;
	sub_824E9870(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DF320;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DF338);
PPC_WEAK_FUNC(sub_825DF338) { __imp__sub_825DF338(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DF338) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// srawi r10,r4,8
	ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r4.s32 >> 8;
	// clrlwi r8,r4,24
	ctx.r8.u64 = ctx.r4.u32 & 0xFF;
	// lwz r11,1792(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 1792);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825df37c
	if (!ctx.cr6.eq) goto loc_825DF37C;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r5,r11,r9
	ctx.r5.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825df380
	goto loc_825DF380;
loc_825DF37C:
	// li r5,0
	ctx.r5.s64 = 0;
loc_825DF380:
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r31,r11,-9624
	ctx.r31.s64 = ctx.r11.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824e9870
	ctx.lr = 0x825DF394;
	sub_824E9870(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DF3A0;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DF3B8);
PPC_WEAK_FUNC(sub_825DF3B8) { __imp__sub_825DF3B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DF3B8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32034
	ctx.r11.s64 = -2099380224;
	// srawi r10,r4,8
	ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r4.s32 >> 8;
	// clrlwi r8,r4,24
	ctx.r8.u64 = ctx.r4.u32 & 0xFF;
	// lwz r11,23120(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 23120);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825df3fc
	if (!ctx.cr6.eq) goto loc_825DF3FC;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r5,r11,r9
	ctx.r5.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825df400
	goto loc_825DF400;
loc_825DF3FC:
	// li r5,0
	ctx.r5.s64 = 0;
loc_825DF400:
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r31,r11,-9624
	ctx.r31.s64 = ctx.r11.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824e9870
	ctx.lr = 0x825DF414;
	sub_824E9870(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DF420;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DF438);
PPC_WEAK_FUNC(sub_825DF438) { __imp__sub_825DF438(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DF438) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// srawi r10,r4,8
	ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r4.s32 >> 8;
	// clrlwi r8,r4,24
	ctx.r8.u64 = ctx.r4.u32 & 0xFF;
	// lwz r11,21384(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 21384);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825df47c
	if (!ctx.cr6.eq) goto loc_825DF47C;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r5,r11,r9
	ctx.r5.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825df480
	goto loc_825DF480;
loc_825DF47C:
	// li r5,0
	ctx.r5.s64 = 0;
loc_825DF480:
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r31,r11,-9624
	ctx.r31.s64 = ctx.r11.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824e99b0
	ctx.lr = 0x825DF494;
	sub_824E99B0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DF4A0;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DF4B8);
PPC_WEAK_FUNC(sub_825DF4B8) { __imp__sub_825DF4B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DF4B8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// srawi r10,r4,8
	ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r4.s32 >> 8;
	// clrlwi r8,r4,24
	ctx.r8.u64 = ctx.r4.u32 & 0xFF;
	// lwz r11,1792(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 1792);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825df4fc
	if (!ctx.cr6.eq) goto loc_825DF4FC;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r5,r11,r9
	ctx.r5.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825df500
	goto loc_825DF500;
loc_825DF4FC:
	// li r5,0
	ctx.r5.s64 = 0;
loc_825DF500:
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r31,r11,-9624
	ctx.r31.s64 = ctx.r11.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824e99b0
	ctx.lr = 0x825DF514;
	sub_824E99B0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DF520;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DF538);
PPC_WEAK_FUNC(sub_825DF538) { __imp__sub_825DF538(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DF538) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32034
	ctx.r11.s64 = -2099380224;
	// srawi r10,r4,8
	ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r4.s32 >> 8;
	// clrlwi r8,r4,24
	ctx.r8.u64 = ctx.r4.u32 & 0xFF;
	// lwz r11,23120(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 23120);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825df57c
	if (!ctx.cr6.eq) goto loc_825DF57C;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r5,r11,r9
	ctx.r5.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825df580
	goto loc_825DF580;
loc_825DF57C:
	// li r5,0
	ctx.r5.s64 = 0;
loc_825DF580:
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r31,r11,-9624
	ctx.r31.s64 = ctx.r11.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824e99b0
	ctx.lr = 0x825DF594;
	sub_824E99B0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DF5A0;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DF5B8);
PPC_WEAK_FUNC(sub_825DF5B8) { __imp__sub_825DF5B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DF5B8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// srawi r10,r4,8
	ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r4.s32 >> 8;
	// clrlwi r8,r4,24
	ctx.r8.u64 = ctx.r4.u32 & 0xFF;
	// lwz r11,21384(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 21384);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825df5fc
	if (!ctx.cr6.eq) goto loc_825DF5FC;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r5,r11,r9
	ctx.r5.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825df600
	goto loc_825DF600;
loc_825DF5FC:
	// li r5,0
	ctx.r5.s64 = 0;
loc_825DF600:
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r31,r11,-9624
	ctx.r31.s64 = ctx.r11.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ea9e8
	ctx.lr = 0x825DF614;
	sub_824EA9E8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DF620;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DF638);
PPC_WEAK_FUNC(sub_825DF638) { __imp__sub_825DF638(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DF638) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// srawi r10,r3,8
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r3.s32 >> 8;
	// clrlwi r8,r3,24
	ctx.r8.u64 = ctx.r3.u32 & 0xFF;
	// lwz r11,21384(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 21384);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825df67c
	if (!ctx.cr6.eq) goto loc_825DF67C;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r4,r11,r9
	ctx.r4.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825df680
	goto loc_825DF680;
loc_825DF67C:
	// li r4,0
	ctx.r4.s64 = 0;
loc_825DF680:
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// addi r31,r11,-9624
	ctx.r31.s64 = ctx.r11.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824eb2e0
	ctx.lr = 0x825DF690;
	sub_824EB2E0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DF69C;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DF6B0);
PPC_WEAK_FUNC(sub_825DF6B0) { __imp__sub_825DF6B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DF6B0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// srawi r10,r3,8
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r3.s32 >> 8;
	// clrlwi r8,r3,24
	ctx.r8.u64 = ctx.r3.u32 & 0xFF;
	// lwz r11,21384(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 21384);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825df6f4
	if (!ctx.cr6.eq) goto loc_825DF6F4;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r4,r11,r9
	ctx.r4.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825df6f8
	goto loc_825DF6F8;
loc_825DF6F4:
	// li r4,0
	ctx.r4.s64 = 0;
loc_825DF6F8:
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// addi r31,r11,-9624
	ctx.r31.s64 = ctx.r11.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824eb348
	ctx.lr = 0x825DF708;
	sub_824EB348(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DF714;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DF728);
PPC_WEAK_FUNC(sub_825DF728) { __imp__sub_825DF728(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DF728) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// srawi r10,r3,8
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r3.s32 >> 8;
	// clrlwi r8,r3,24
	ctx.r8.u64 = ctx.r3.u32 & 0xFF;
	// mr r5,r4
	ctx.r5.u64 = ctx.r4.u64;
	// lwz r11,1792(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 1792);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825df770
	if (!ctx.cr6.eq) goto loc_825DF770;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825df774
	goto loc_825DF774;
loc_825DF770:
	// li r11,0
	ctx.r11.s64 = 0;
loc_825DF774:
	// lis r10,-32000
	ctx.r10.s64 = -2097152000;
	// mr r4,r11
	ctx.r4.u64 = ctx.r11.u64;
	// addi r31,r10,-9624
	ctx.r31.s64 = ctx.r10.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824eb3b0
	ctx.lr = 0x825DF788;
	sub_824EB3B0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DF794;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DF7A8);
PPC_WEAK_FUNC(sub_825DF7A8) { __imp__sub_825DF7A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DF7A8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// srawi r10,r3,8
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r3.s32 >> 8;
	// clrlwi r8,r3,24
	ctx.r8.u64 = ctx.r3.u32 & 0xFF;
	// mr r5,r4
	ctx.r5.u64 = ctx.r4.u64;
	// lwz r11,21384(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 21384);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825df7f0
	if (!ctx.cr6.eq) goto loc_825DF7F0;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825df7f4
	goto loc_825DF7F4;
loc_825DF7F0:
	// li r11,0
	ctx.r11.s64 = 0;
loc_825DF7F4:
	// lis r10,-32000
	ctx.r10.s64 = -2097152000;
	// mr r4,r11
	ctx.r4.u64 = ctx.r11.u64;
	// addi r31,r10,-9624
	ctx.r31.s64 = ctx.r10.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824eb418
	ctx.lr = 0x825DF808;
	sub_824EB418(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DF814;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DF828);
PPC_WEAK_FUNC(sub_825DF828) { __imp__sub_825DF828(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DF828) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32034
	ctx.r11.s64 = -2099380224;
	// srawi r10,r3,8
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r3.s32 >> 8;
	// clrlwi r8,r3,24
	ctx.r8.u64 = ctx.r3.u32 & 0xFF;
	// mr r5,r4
	ctx.r5.u64 = ctx.r4.u64;
	// lwz r11,23120(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 23120);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825df870
	if (!ctx.cr6.eq) goto loc_825DF870;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825df874
	goto loc_825DF874;
loc_825DF870:
	// li r11,0
	ctx.r11.s64 = 0;
loc_825DF874:
	// lis r10,-32000
	ctx.r10.s64 = -2097152000;
	// mr r4,r11
	ctx.r4.u64 = ctx.r11.u64;
	// addi r31,r10,-9624
	ctx.r31.s64 = ctx.r10.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824eb480
	ctx.lr = 0x825DF888;
	sub_824EB480(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DF894;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DF8A8);
PPC_WEAK_FUNC(sub_825DF8A8) { __imp__sub_825DF8A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DF8A8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// srawi r10,r3,8
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r3.s32 >> 8;
	// clrlwi r8,r3,24
	ctx.r8.u64 = ctx.r3.u32 & 0xFF;
	// mr r5,r4
	ctx.r5.u64 = ctx.r4.u64;
	// lwz r11,1792(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 1792);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825df8f0
	if (!ctx.cr6.eq) goto loc_825DF8F0;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825df8f4
	goto loc_825DF8F4;
loc_825DF8F0:
	// li r11,0
	ctx.r11.s64 = 0;
loc_825DF8F4:
	// lis r10,-32000
	ctx.r10.s64 = -2097152000;
	// mr r4,r11
	ctx.r4.u64 = ctx.r11.u64;
	// addi r31,r10,-9624
	ctx.r31.s64 = ctx.r10.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824eb568
	ctx.lr = 0x825DF908;
	sub_824EB568(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DF914;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DF928);
PPC_WEAK_FUNC(sub_825DF928) { __imp__sub_825DF928(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DF928) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32000
	ctx.r10.s64 = -2097152000;
	// addi r31,r10,-9624
	ctx.r31.s64 = ctx.r10.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// cntlzw r10,r10
	ctx.r10.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r10,r10,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// rlwinm r9,r11,27,31,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r10,1
	ctx.r11.u64 = ctx.r10.u64 ^ 1;
	// xori r10,r9,1
	ctx.r10.u64 = ctx.r9.u64 ^ 1;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// clrlwi r5,r10,24
	ctx.r5.u64 = ctx.r10.u32 & 0xFF;
	// bl 0x824e9350
	ctx.lr = 0x825DF974;
	sub_824E9350(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DF980;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DF998);
PPC_WEAK_FUNC(sub_825DF998) { __imp__sub_825DF998(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DF998) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32000
	ctx.r10.s64 = -2097152000;
	// addi r31,r10,-9624
	ctx.r31.s64 = ctx.r10.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// cntlzw r11,r10
	ctx.r11.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// bl 0x824e91e0
	ctx.lr = 0x825DF9D4;
	sub_824E91E0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DF9E0;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DF9F8);
PPC_WEAK_FUNC(sub_825DF9F8) { __imp__sub_825DF9F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DF9F8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32000
	ctx.r10.s64 = -2097152000;
	// addi r31,r10,-9624
	ctx.r31.s64 = ctx.r10.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// cntlzw r11,r10
	ctx.r11.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// bl 0x824e9298
	ctx.lr = 0x825DFA34;
	sub_824E9298(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DFA40;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DFA58);
PPC_WEAK_FUNC(sub_825DFA58) { __imp__sub_825DFA58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DFA58) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32000
	ctx.r10.s64 = -2097152000;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r31,r10,-9624
	ctx.r31.s64 = ctx.r10.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f0,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f12.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f12,88(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// bl 0x824e94b0
	ctx.lr = 0x825DFA9C;
	sub_824E94B0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DFAA8;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DFAC0);
PPC_WEAK_FUNC(sub_825DFAC0) { __imp__sub_825DFAC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DFAC0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32000
	ctx.r10.s64 = -2097152000;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r31,r10,-9624
	ctx.r31.s64 = ctx.r10.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f0,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f12.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f12,88(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// bl 0x824e9520
	ctx.lr = 0x825DFB04;
	sub_824E9520(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DFB10;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DFB28);
PPC_WEAK_FUNC(sub_825DFB28) { __imp__sub_825DFB28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DFB28) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32000
	ctx.r10.s64 = -2097152000;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,-9624
	ctx.r31.s64 = ctx.r10.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f1,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x824e9590
	ctx.lr = 0x825DFB54;
	sub_824E9590(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DFB60;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DFB78);
PPC_WEAK_FUNC(sub_825DFB78) { __imp__sub_825DFB78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DFB78) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32000
	ctx.r10.s64 = -2097152000;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,-9624
	ctx.r31.s64 = ctx.r10.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f1,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x824e95f8
	ctx.lr = 0x825DFBA4;
	sub_824E95F8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DFBB0;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DFBC8);
PPC_WEAK_FUNC(sub_825DFBC8) { __imp__sub_825DFBC8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DFBC8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32000
	ctx.r10.s64 = -2097152000;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,-9624
	ctx.r31.s64 = ctx.r10.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f1,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x824e9660
	ctx.lr = 0x825DFBF4;
	sub_824E9660(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DFC00;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DFC18);
PPC_WEAK_FUNC(sub_825DFC18) { __imp__sub_825DFC18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DFC18) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32000
	ctx.r10.s64 = -2097152000;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,-9624
	ctx.r31.s64 = ctx.r10.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f1,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x824e96c8
	ctx.lr = 0x825DFC44;
	sub_824E96C8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DFC50;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DFC68);
PPC_WEAK_FUNC(sub_825DFC68) { __imp__sub_825DFC68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DFC68) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32000
	ctx.r10.s64 = -2097152000;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,-9624
	ctx.r31.s64 = ctx.r10.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f1,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x824e9730
	ctx.lr = 0x825DFC94;
	sub_824E9730(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DFCA0;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DFCB8);
PPC_WEAK_FUNC(sub_825DFCB8) { __imp__sub_825DFCB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DFCB8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f1,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// lfs f2,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f2.f64 = double(temp.f32);
	// lfs f4,16(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f4.f64 = double(temp.f32);
	// lfs f3,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f3.f64 = double(temp.f32);
	// b 0x825deb78
	sub_825DEB78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DFCD8);
PPC_WEAK_FUNC(sub_825DFCD8) { __imp__sub_825DFCD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DFCD8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r10,r11,1
	ctx.r10.u64 = ctx.r11.u64 ^ 1;
	// lis r11,-32044
	ctx.r11.s64 = -2100035584;
	// lwz r11,30160(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 30160);
	// stb r10,628(r11)
	PPC_STORE_U8(ctx.r11.u32 + 628, ctx.r10.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DFD00);
PPC_WEAK_FUNC(sub_825DFD00) { __imp__sub_825DFD00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DFD00) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32000
	ctx.r10.s64 = -2097152000;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,-9624
	ctx.r31.s64 = ctx.r10.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f1,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x824e9798
	ctx.lr = 0x825DFD2C;
	sub_824E9798(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DFD38;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DFD50);
PPC_WEAK_FUNC(sub_825DFD50) { __imp__sub_825DFD50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DFD50) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32000
	ctx.r10.s64 = -2097152000;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,-9624
	ctx.r31.s64 = ctx.r10.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f1,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x824e9a18
	ctx.lr = 0x825DFD7C;
	sub_824E9A18(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DFD88;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DFDA0);
PPC_WEAK_FUNC(sub_825DFDA0) { __imp__sub_825DFDA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DFDA0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825df1b8
	sub_825DF1B8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DFDB0);
PPC_WEAK_FUNC(sub_825DFDB0) { __imp__sub_825DFDB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DFDB0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825df138
	sub_825DF138(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DFDC0);
PPC_WEAK_FUNC(sub_825DFDC0) { __imp__sub_825DFDC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DFDC0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825df238
	sub_825DF238(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DFDD0);
PPC_WEAK_FUNC(sub_825DFDD0) { __imp__sub_825DFDD0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DFDD0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r11,-9624
	ctx.r31.s64 = ctx.r11.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824e9f88
	ctx.lr = 0x825DFDF8;
	sub_824E9F88(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DFE04;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DFE18);
PPC_WEAK_FUNC(sub_825DFE18) { __imp__sub_825DFE18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DFE18) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825df2b8
	sub_825DF2B8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DFE28);
PPC_WEAK_FUNC(sub_825DFE28) { __imp__sub_825DFE28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DFE28) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825df338
	sub_825DF338(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DFE38);
PPC_WEAK_FUNC(sub_825DFE38) { __imp__sub_825DFE38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DFE38) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825df3b8
	sub_825DF3B8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DFE48);
PPC_WEAK_FUNC(sub_825DFE48) { __imp__sub_825DFE48(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DFE48) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32000
	ctx.r10.s64 = -2097152000;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r31,r10,-9624
	ctx.r31.s64 = ctx.r10.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f0,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f12.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f12,88(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// bl 0x824e98d8
	ctx.lr = 0x825DFE8C;
	sub_824E98D8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DFE98;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DFEB0);
PPC_WEAK_FUNC(sub_825DFEB0) { __imp__sub_825DFEB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DFEB0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32000
	ctx.r10.s64 = -2097152000;
	// addi r31,r10,-9624
	ctx.r31.s64 = ctx.r10.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r10
	ctx.r11.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r5,r11,24
	ctx.r5.u64 = ctx.r11.u32 & 0xFF;
	// bl 0x824e9948
	ctx.lr = 0x825DFEEC;
	sub_824E9948(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DFEF8;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DFF10);
PPC_WEAK_FUNC(sub_825DFF10) { __imp__sub_825DFF10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DFF10) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32000
	ctx.r10.s64 = -2097152000;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r31,r10,-9624
	ctx.r31.s64 = ctx.r10.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f0,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f12.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f12,88(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// bl 0x824e9800
	ctx.lr = 0x825DFF54;
	sub_824E9800(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DFF60;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DFF78);
PPC_WEAK_FUNC(sub_825DFF78) { __imp__sub_825DFF78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DFF78) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825df438
	sub_825DF438(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DFF88);
PPC_WEAK_FUNC(sub_825DFF88) { __imp__sub_825DFF88(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DFF88) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825df4b8
	sub_825DF4B8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DFF98);
PPC_WEAK_FUNC(sub_825DFF98) { __imp__sub_825DFF98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DFF98) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825df538
	sub_825DF538(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DFFA8);
PPC_WEAK_FUNC(sub_825DFFA8) { __imp__sub_825DFFA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DFFA8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32000
	ctx.r10.s64 = -2097152000;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,-9624
	ctx.r31.s64 = ctx.r10.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x824e9b58
	ctx.lr = 0x825DFFD4;
	sub_824E9B58(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825DFFE0;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825DFFF8);
PPC_WEAK_FUNC(sub_825DFFF8) { __imp__sub_825DFFF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825DFFF8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32000
	ctx.r10.s64 = -2097152000;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r31,r10,-9624
	ctx.r31.s64 = ctx.r10.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f0,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f12.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f12,88(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// bl 0x824e9ae8
	ctx.lr = 0x825E003C;
	sub_824E9AE8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825E0048;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825E0060);
PPC_WEAK_FUNC(sub_825E0060) { __imp__sub_825E0060(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825E0060) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32000
	ctx.r10.s64 = -2097152000;
	// addi r31,r10,-9624
	ctx.r31.s64 = ctx.r10.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r10
	ctx.r11.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r5,r11,24
	ctx.r5.u64 = ctx.r11.u32 & 0xFF;
	// bl 0x824e9bc0
	ctx.lr = 0x825E009C;
	sub_824E9BC0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825E00A8;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825E00C0);
PPC_WEAK_FUNC(sub_825E00C0) { __imp__sub_825E00C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825E00C0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32000
	ctx.r10.s64 = -2097152000;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,-9624
	ctx.r31.s64 = ctx.r10.s64 + -9624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f1,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// lfs f2,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f2.f64 = double(temp.f32);
	// lfs f3,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f3.f64 = double(temp.f32);
	// bl 0x824e9c28
	ctx.lr = 0x825E00F4;
	sub_824E9C28(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824ed1c0
	ctx.lr = 0x825E0100;
	sub_824ED1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825E0118);
PPC_WEAK_FUNC(sub_825E0118) { __imp__sub_825E0118(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_825E0118) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825dec50
	sub_825DEC50(ctx, base);
	return;
}

