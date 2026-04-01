#include "gta4_init.h"

PPC_FUNC_IMPL(__imp__sub_82A689B0);
PPC_WEAK_FUNC(sub_82A689B0) { __imp__sub_82A689B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A689B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r10,15
	ctx.r10.s64 = 15;
	// addi r11,r11,-4824
	ctx.r11.s64 = ctx.r11.s64 + -4824;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
loc_82A689C4:
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
	// stw r9,-8(r11)
	PPC_STORE_U32(ctx.r11.u32 + -8, ctx.r9.u32);
	// stw r9,-4(r11)
	PPC_STORE_U32(ctx.r11.u32 + -4, ctx.r9.u32);
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// addi r11,r11,12
	ctx.r11.s64 = ctx.r11.s64 + 12;
	// bge cr6,0x82a689c4
	if (!ctx.cr6.lt) goto loc_82A689C4;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A689E8);
PPC_WEAK_FUNC(sub_82A689E8) { __imp__sub_82A689E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A689E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,19760
	ctx.r5.s64 = ctx.r11.s64 + 19760;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-4632
	ctx.r3.s64 = ctx.r11.s64 + -4632;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A68A08);
PPC_WEAK_FUNC(sub_82A68A08) { __imp__sub_82A68A08(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68A08) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,22092
	ctx.r5.s64 = ctx.r11.s64 + 22092;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-4396
	ctx.r3.s64 = ctx.r11.s64 + -4396;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A68A28);
PPC_WEAK_FUNC(sub_82A68A28) { __imp__sub_82A68A28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68A28) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r5,r11,22436
	ctx.r5.s64 = ctx.r11.s64 + 22436;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-4344
	ctx.r31.s64 = ctx.r11.s64 + -4344;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A68A5C;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A68A6C;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A68A74;
	sub_829DC040(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,22432
	ctx.r11.s64 = ctx.r11.s64 + 22432;
	// addi r3,r10,8552
	ctx.r3.s64 = ctx.r10.s64 + 8552;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A68A8C;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A68AA0);
PPC_WEAK_FUNC(sub_82A68AA0) { __imp__sub_82A68AA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68AA0) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r5,r11,22464
	ctx.r5.s64 = ctx.r11.s64 + 22464;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-4376
	ctx.r31.s64 = ctx.r11.s64 + -4376;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A68AD4;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A68AE4;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A68AEC;
	sub_829DC040(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,22460
	ctx.r11.s64 = ctx.r11.s64 + 22460;
	// addi r3,r10,8632
	ctx.r3.s64 = ctx.r10.s64 + 8632;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A68B04;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A68B18);
PPC_WEAK_FUNC(sub_82A68B18) { __imp__sub_82A68B18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68B18) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r5,r11,23220
	ctx.r5.s64 = ctx.r11.s64 + 23220;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-4176
	ctx.r31.s64 = ctx.r11.s64 + -4176;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A68B4C;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A68B5C;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A68B64;
	sub_829DC040(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,23216
	ctx.r11.s64 = ctx.r11.s64 + 23216;
	// addi r3,r10,8712
	ctx.r3.s64 = ctx.r10.s64 + 8712;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A68B7C;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A68B90);
PPC_WEAK_FUNC(sub_82A68B90) { __imp__sub_82A68B90(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68B90) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r5,r11,23244
	ctx.r5.s64 = ctx.r11.s64 + 23244;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-4144
	ctx.r31.s64 = ctx.r11.s64 + -4144;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A68BC4;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A68BD4;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A68BDC;
	sub_829DC040(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,23240
	ctx.r11.s64 = ctx.r11.s64 + 23240;
	// addi r3,r10,8792
	ctx.r3.s64 = ctx.r10.s64 + 8792;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A68BF4;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A68C08);
PPC_WEAK_FUNC(sub_82A68C08) { __imp__sub_82A68C08(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68C08) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r5,r11,23276
	ctx.r5.s64 = ctx.r11.s64 + 23276;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-4272
	ctx.r31.s64 = ctx.r11.s64 + -4272;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A68C3C;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A68C4C;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A68C54;
	sub_829DC040(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,23272
	ctx.r11.s64 = ctx.r11.s64 + 23272;
	// addi r3,r10,8872
	ctx.r3.s64 = ctx.r10.s64 + 8872;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A68C6C;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A68C80);
PPC_WEAK_FUNC(sub_82A68C80) { __imp__sub_82A68C80(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68C80) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r5,r11,23304
	ctx.r5.s64 = ctx.r11.s64 + 23304;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-4304
	ctx.r31.s64 = ctx.r11.s64 + -4304;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A68CB4;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A68CC4;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A68CCC;
	sub_829DC040(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,23300
	ctx.r11.s64 = ctx.r11.s64 + 23300;
	// addi r3,r10,8952
	ctx.r3.s64 = ctx.r10.s64 + 8952;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A68CE4;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A68CF8);
PPC_WEAK_FUNC(sub_82A68CF8) { __imp__sub_82A68CF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68CF8) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r5,r11,23336
	ctx.r5.s64 = ctx.r11.s64 + 23336;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-4208
	ctx.r31.s64 = ctx.r11.s64 + -4208;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A68D2C;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A68D3C;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A68D44;
	sub_829DC040(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,23332
	ctx.r11.s64 = ctx.r11.s64 + 23332;
	// addi r3,r10,9032
	ctx.r3.s64 = ctx.r10.s64 + 9032;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A68D5C;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A68D70);
PPC_WEAK_FUNC(sub_82A68D70) { __imp__sub_82A68D70(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68D70) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r5,r11,23364
	ctx.r5.s64 = ctx.r11.s64 + 23364;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-4240
	ctx.r31.s64 = ctx.r11.s64 + -4240;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A68DA4;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A68DB4;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A68DBC;
	sub_829DC040(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,23360
	ctx.r11.s64 = ctx.r11.s64 + 23360;
	// addi r3,r10,9112
	ctx.r3.s64 = ctx.r10.s64 + 9112;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A68DD4;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A68DE8);
PPC_WEAK_FUNC(sub_82A68DE8) { __imp__sub_82A68DE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68DE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,9192
	ctx.r3.s64 = ctx.r11.s64 + 9192;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A68DF8);
PPC_WEAK_FUNC(sub_82A68DF8) { __imp__sub_82A68DF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68DF8) {
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
	// lis r9,-32249
	ctx.r9.s64 = -2113470464;
	// lis r10,-32134
	ctx.r10.s64 = -2105933824;
	// addi r5,r9,27244
	ctx.r5.s64 = ctx.r9.s64 + 27244;
	// lis r9,-31975
	ctx.r9.s64 = -2095513600;
	// lis r11,-32134
	ctx.r11.s64 = -2105933824;
	// lis r8,-32134
	ctx.r8.s64 = -2105933824;
	// addi r3,r9,-4108
	ctx.r3.s64 = ctx.r9.s64 + -4108;
	// li r9,36
	ctx.r9.s64 = 36;
	// addi r8,r8,-23608
	ctx.r8.s64 = ctx.r8.s64 + -23608;
	// addi r7,r10,-23688
	ctx.r7.s64 = ctx.r10.s64 + -23688;
	// addi r6,r11,-23840
	ctx.r6.s64 = ctx.r11.s64 + -23840;
	// li r4,19
	ctx.r4.s64 = 19;
	// bl 0x8279b328
	ctx.lr = 0x82A68E38;
	sub_8279B328(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,9216
	ctx.r3.s64 = ctx.r11.s64 + 9216;
	// bl 0x829ffa48
	ctx.lr = 0x82A68E44;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A68E58);
PPC_WEAK_FUNC(sub_82A68E58) { __imp__sub_82A68E58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68E58) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,9232
	ctx.r3.s64 = ctx.r11.s64 + 9232;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A68E68);
PPC_WEAK_FUNC(sub_82A68E68) { __imp__sub_82A68E68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68E68) {
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
	// lis r9,-32249
	ctx.r9.s64 = -2113470464;
	// lis r10,-32134
	ctx.r10.s64 = -2105933824;
	// addi r5,r9,28868
	ctx.r5.s64 = ctx.r9.s64 + 28868;
	// lis r9,-31975
	ctx.r9.s64 = -2095513600;
	// lis r11,-32134
	ctx.r11.s64 = -2105933824;
	// lis r8,-32134
	ctx.r8.s64 = -2105933824;
	// addi r3,r9,-4072
	ctx.r3.s64 = ctx.r9.s64 + -4072;
	// li r9,60
	ctx.r9.s64 = 60;
	// addi r8,r8,-12184
	ctx.r8.s64 = ctx.r8.s64 + -12184;
	// addi r7,r10,-12272
	ctx.r7.s64 = ctx.r10.s64 + -12272;
	// addi r6,r11,-12400
	ctx.r6.s64 = ctx.r11.s64 + -12400;
	// li r4,4
	ctx.r4.s64 = 4;
	// bl 0x8279b328
	ctx.lr = 0x82A68EA8;
	sub_8279B328(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,9264
	ctx.r3.s64 = ctx.r11.s64 + 9264;
	// bl 0x829ffa48
	ctx.lr = 0x82A68EB4;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A68EC8);
PPC_WEAK_FUNC(sub_82A68EC8) { __imp__sub_82A68EC8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68EC8) {
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
	// lis r9,-32249
	ctx.r9.s64 = -2113470464;
	// lis r10,-32134
	ctx.r10.s64 = -2105933824;
	// addi r5,r9,29228
	ctx.r5.s64 = ctx.r9.s64 + 29228;
	// lis r9,-31975
	ctx.r9.s64 = -2095513600;
	// lis r11,-32134
	ctx.r11.s64 = -2105933824;
	// lis r8,-32134
	ctx.r8.s64 = -2105933824;
	// addi r3,r9,-4048
	ctx.r3.s64 = ctx.r9.s64 + -4048;
	// li r9,52
	ctx.r9.s64 = 52;
	// addi r8,r8,-10792
	ctx.r8.s64 = ctx.r8.s64 + -10792;
	// addi r7,r10,-10808
	ctx.r7.s64 = ctx.r10.s64 + -10808;
	// addi r6,r11,-10936
	ctx.r6.s64 = ctx.r11.s64 + -10936;
	// li r4,18
	ctx.r4.s64 = 18;
	// bl 0x8279b328
	ctx.lr = 0x82A68F08;
	sub_8279B328(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,9280
	ctx.r3.s64 = ctx.r11.s64 + 9280;
	// bl 0x829ffa48
	ctx.lr = 0x82A68F14;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A68F28);
PPC_WEAK_FUNC(sub_82A68F28) { __imp__sub_82A68F28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68F28) {
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
	// lis r9,-32249
	ctx.r9.s64 = -2113470464;
	// lis r10,-32134
	ctx.r10.s64 = -2105933824;
	// addi r5,r9,29580
	ctx.r5.s64 = ctx.r9.s64 + 29580;
	// lis r9,-31975
	ctx.r9.s64 = -2095513600;
	// lis r11,-32134
	ctx.r11.s64 = -2105933824;
	// lis r8,-32134
	ctx.r8.s64 = -2105933824;
	// addi r3,r9,-4024
	ctx.r3.s64 = ctx.r9.s64 + -4024;
	// li r9,36
	ctx.r9.s64 = 36;
	// addi r8,r8,-9232
	ctx.r8.s64 = ctx.r8.s64 + -9232;
	// addi r7,r10,-9312
	ctx.r7.s64 = ctx.r10.s64 + -9312;
	// addi r6,r11,-9464
	ctx.r6.s64 = ctx.r11.s64 + -9464;
	// li r4,7
	ctx.r4.s64 = 7;
	// bl 0x8279b328
	ctx.lr = 0x82A68F68;
	sub_8279B328(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,9296
	ctx.r3.s64 = ctx.r11.s64 + 9296;
	// bl 0x829ffa48
	ctx.lr = 0x82A68F74;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A68F88);
PPC_WEAK_FUNC(sub_82A68F88) { __imp__sub_82A68F88(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68F88) {
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
	// lis r9,-32249
	ctx.r9.s64 = -2113470464;
	// lis r10,-32134
	ctx.r10.s64 = -2105933824;
	// addi r5,r9,30304
	ctx.r5.s64 = ctx.r9.s64 + 30304;
	// lis r9,-31975
	ctx.r9.s64 = -2095513600;
	// lis r11,-32134
	ctx.r11.s64 = -2105933824;
	// lis r8,-32134
	ctx.r8.s64 = -2105933824;
	// addi r3,r9,-4000
	ctx.r3.s64 = ctx.r9.s64 + -4000;
	// li r9,296
	ctx.r9.s64 = 296;
	// addi r8,r8,-1520
	ctx.r8.s64 = ctx.r8.s64 + -1520;
	// addi r7,r10,-1592
	ctx.r7.s64 = ctx.r10.s64 + -1592;
	// addi r6,r11,-1736
	ctx.r6.s64 = ctx.r11.s64 + -1736;
	// li r4,13
	ctx.r4.s64 = 13;
	// bl 0x8279b328
	ctx.lr = 0x82A68FC8;
	sub_8279B328(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,9312
	ctx.r3.s64 = ctx.r11.s64 + 9312;
	// bl 0x829ffa48
	ctx.lr = 0x82A68FD4;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A68FE8);
PPC_WEAK_FUNC(sub_82A68FE8) { __imp__sub_82A68FE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68FE8) {
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
	// lis r9,-32249
	ctx.r9.s64 = -2113470464;
	// lis r10,-32134
	ctx.r10.s64 = -2105933824;
	// addi r5,r9,32060
	ctx.r5.s64 = ctx.r9.s64 + 32060;
	// lis r9,-31975
	ctx.r9.s64 = -2095513600;
	// lis r11,-32134
	ctx.r11.s64 = -2105933824;
	// lis r8,-32134
	ctx.r8.s64 = -2105933824;
	// addi r3,r9,-3976
	ctx.r3.s64 = ctx.r9.s64 + -3976;
	// li r9,296
	ctx.r9.s64 = 296;
	// addi r8,r8,22072
	ctx.r8.s64 = ctx.r8.s64 + 22072;
	// addi r7,r10,22000
	ctx.r7.s64 = ctx.r10.s64 + 22000;
	// addi r6,r11,21856
	ctx.r6.s64 = ctx.r11.s64 + 21856;
	// li r4,22
	ctx.r4.s64 = 22;
	// bl 0x8279b328
	ctx.lr = 0x82A69028;
	sub_8279B328(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,9328
	ctx.r3.s64 = ctx.r11.s64 + 9328;
	// bl 0x829ffa48
	ctx.lr = 0x82A69034;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69048);
PPC_WEAK_FUNC(sub_82A69048) { __imp__sub_82A69048(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69048) {
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
	// lis r9,-32249
	ctx.r9.s64 = -2113470464;
	// lis r10,-32134
	ctx.r10.s64 = -2105933824;
	// addi r5,r9,32412
	ctx.r5.s64 = ctx.r9.s64 + 32412;
	// lis r9,-31975
	ctx.r9.s64 = -2095513600;
	// lis r11,-32134
	ctx.r11.s64 = -2105933824;
	// lis r8,-32134
	ctx.r8.s64 = -2105933824;
	// addi r3,r9,-3952
	ctx.r3.s64 = ctx.r9.s64 + -3952;
	// li r9,44
	ctx.r9.s64 = 44;
	// addi r8,r8,22944
	ctx.r8.s64 = ctx.r8.s64 + 22944;
	// addi r7,r10,22928
	ctx.r7.s64 = ctx.r10.s64 + 22928;
	// addi r6,r11,22808
	ctx.r6.s64 = ctx.r11.s64 + 22808;
	// li r4,21
	ctx.r4.s64 = 21;
	// bl 0x8279b328
	ctx.lr = 0x82A69088;
	sub_8279B328(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,9344
	ctx.r3.s64 = ctx.r11.s64 + 9344;
	// bl 0x829ffa48
	ctx.lr = 0x82A69094;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A690A8);
PPC_WEAK_FUNC(sub_82A690A8) { __imp__sub_82A690A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A690A8) {
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
	// lis r9,-32249
	ctx.r9.s64 = -2113470464;
	// lis r10,-32134
	ctx.r10.s64 = -2105933824;
	// addi r5,r9,32764
	ctx.r5.s64 = ctx.r9.s64 + 32764;
	// lis r9,-31975
	ctx.r9.s64 = -2095513600;
	// lis r11,-32134
	ctx.r11.s64 = -2105933824;
	// lis r8,-32134
	ctx.r8.s64 = -2105933824;
	// addi r3,r9,-3928
	ctx.r3.s64 = ctx.r9.s64 + -3928;
	// li r9,40
	ctx.r9.s64 = 40;
	// addi r8,r8,23560
	ctx.r8.s64 = ctx.r8.s64 + 23560;
	// addi r7,r10,23480
	ctx.r7.s64 = ctx.r10.s64 + 23480;
	// addi r6,r11,23328
	ctx.r6.s64 = ctx.r11.s64 + 23328;
	// li r4,20
	ctx.r4.s64 = 20;
	// bl 0x8279b328
	ctx.lr = 0x82A690E8;
	sub_8279B328(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,9360
	ctx.r3.s64 = ctx.r11.s64 + 9360;
	// bl 0x829ffa48
	ctx.lr = 0x82A690F4;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69108);
PPC_WEAK_FUNC(sub_82A69108) { __imp__sub_82A69108(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69108) {
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
	// lis r9,-32248
	ctx.r9.s64 = -2113404928;
	// lis r10,-32134
	ctx.r10.s64 = -2105933824;
	// addi r5,r9,-32420
	ctx.r5.s64 = ctx.r9.s64 + -32420;
	// lis r9,-31975
	ctx.r9.s64 = -2095513600;
	// lis r11,-32134
	ctx.r11.s64 = -2105933824;
	// lis r8,-32134
	ctx.r8.s64 = -2105933824;
	// addi r3,r9,-3904
	ctx.r3.s64 = ctx.r9.s64 + -3904;
	// li r9,36
	ctx.r9.s64 = 36;
	// addi r8,r8,24392
	ctx.r8.s64 = ctx.r8.s64 + 24392;
	// addi r7,r10,24312
	ctx.r7.s64 = ctx.r10.s64 + 24312;
	// addi r6,r11,24152
	ctx.r6.s64 = ctx.r11.s64 + 24152;
	// li r4,9
	ctx.r4.s64 = 9;
	// bl 0x8279b328
	ctx.lr = 0x82A69148;
	sub_8279B328(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,9376
	ctx.r3.s64 = ctx.r11.s64 + 9376;
	// bl 0x829ffa48
	ctx.lr = 0x82A69154;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69168);
PPC_WEAK_FUNC(sub_82A69168) { __imp__sub_82A69168(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69168) {
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
	// lis r9,-32248
	ctx.r9.s64 = -2113404928;
	// lis r10,-32134
	ctx.r10.s64 = -2105933824;
	// addi r5,r9,-32068
	ctx.r5.s64 = ctx.r9.s64 + -32068;
	// lis r9,-31975
	ctx.r9.s64 = -2095513600;
	// lis r11,-32134
	ctx.r11.s64 = -2105933824;
	// lis r8,-32134
	ctx.r8.s64 = -2105933824;
	// addi r3,r9,-3880
	ctx.r3.s64 = ctx.r9.s64 + -3880;
	// li r9,36
	ctx.r9.s64 = 36;
	// addi r8,r8,25216
	ctx.r8.s64 = ctx.r8.s64 + 25216;
	// addi r7,r10,25136
	ctx.r7.s64 = ctx.r10.s64 + 25136;
	// addi r6,r11,24992
	ctx.r6.s64 = ctx.r11.s64 + 24992;
	// li r4,8
	ctx.r4.s64 = 8;
	// bl 0x8279b328
	ctx.lr = 0x82A691A8;
	sub_8279B328(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,9392
	ctx.r3.s64 = ctx.r11.s64 + 9392;
	// bl 0x829ffa48
	ctx.lr = 0x82A691B4;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A691C8);
PPC_WEAK_FUNC(sub_82A691C8) { __imp__sub_82A691C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A691C8) {
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
	// lis r9,-32248
	ctx.r9.s64 = -2113404928;
	// lis r10,-32134
	ctx.r10.s64 = -2105933824;
	// addi r5,r9,-31712
	ctx.r5.s64 = ctx.r9.s64 + -31712;
	// lis r9,-31975
	ctx.r9.s64 = -2095513600;
	// lis r11,-32134
	ctx.r11.s64 = -2105933824;
	// lis r8,-32134
	ctx.r8.s64 = -2105933824;
	// addi r3,r9,-3856
	ctx.r3.s64 = ctx.r9.s64 + -3856;
	// li r9,48
	ctx.r9.s64 = 48;
	// addi r8,r8,25840
	ctx.r8.s64 = ctx.r8.s64 + 25840;
	// addi r7,r10,25768
	ctx.r7.s64 = ctx.r10.s64 + 25768;
	// addi r6,r11,25624
	ctx.r6.s64 = ctx.r11.s64 + 25624;
	// li r4,6
	ctx.r4.s64 = 6;
	// bl 0x8279b328
	ctx.lr = 0x82A69208;
	sub_8279B328(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,9408
	ctx.r3.s64 = ctx.r11.s64 + 9408;
	// bl 0x829ffa48
	ctx.lr = 0x82A69214;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69228);
PPC_WEAK_FUNC(sub_82A69228) { __imp__sub_82A69228(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69228) {
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
	// lis r9,-32248
	ctx.r9.s64 = -2113404928;
	// lis r10,-32134
	ctx.r10.s64 = -2105933824;
	// addi r5,r9,-31216
	ctx.r5.s64 = ctx.r9.s64 + -31216;
	// lis r9,-31975
	ctx.r9.s64 = -2095513600;
	// lis r11,-32134
	ctx.r11.s64 = -2105933824;
	// lis r8,-32134
	ctx.r8.s64 = -2105933824;
	// addi r3,r9,-3832
	ctx.r3.s64 = ctx.r9.s64 + -3832;
	// li r9,52
	ctx.r9.s64 = 52;
	// addi r8,r8,27792
	ctx.r8.s64 = ctx.r8.s64 + 27792;
	// addi r7,r10,27776
	ctx.r7.s64 = ctx.r10.s64 + 27776;
	// addi r6,r11,27648
	ctx.r6.s64 = ctx.r11.s64 + 27648;
	// li r4,5
	ctx.r4.s64 = 5;
	// bl 0x8279b328
	ctx.lr = 0x82A69268;
	sub_8279B328(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,9424
	ctx.r3.s64 = ctx.r11.s64 + 9424;
	// bl 0x829ffa48
	ctx.lr = 0x82A69274;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69288);
PPC_WEAK_FUNC(sub_82A69288) { __imp__sub_82A69288(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69288) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32082
	ctx.r11.s64 = -2102525952;
	// addi r11,r11,17560
	ctx.r11.s64 = ctx.r11.s64 + 17560;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A692A8);
PPC_WEAK_FUNC(sub_82A692A8) { __imp__sub_82A692A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A692A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32082
	ctx.r11.s64 = -2102525952;
	// li r6,3814
	ctx.r6.s64 = 3814;
	// addi r5,r11,4616
	ctx.r5.s64 = ctx.r11.s64 + 4616;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r4,r11,-29960
	ctx.r4.s64 = ctx.r11.s64 + -29960;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,-3776
	ctx.r3.s64 = ctx.r11.s64 + -3776;
	// b 0x8286c878
	sub_8286C878(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A692C8);
PPC_WEAK_FUNC(sub_82A692C8) { __imp__sub_82A692C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A692C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32082
	ctx.r11.s64 = -2102525952;
	// li r6,8397
	ctx.r6.s64 = 8397;
	// addi r5,r11,8432
	ctx.r5.s64 = ctx.r11.s64 + 8432;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r4,r11,-29928
	ctx.r4.s64 = ctx.r11.s64 + -29928;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,-3724
	ctx.r3.s64 = ctx.r11.s64 + -3724;
	// b 0x8286c878
	sub_8286C878(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A692E8);
PPC_WEAK_FUNC(sub_82A692E8) { __imp__sub_82A692E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A692E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32082
	ctx.r11.s64 = -2102525952;
	// li r6,126
	ctx.r6.s64 = 126;
	// addi r5,r11,16832
	ctx.r5.s64 = ctx.r11.s64 + 16832;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r4,r11,-29896
	ctx.r4.s64 = ctx.r11.s64 + -29896;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,-3760
	ctx.r3.s64 = ctx.r11.s64 + -3760;
	// b 0x8286c878
	sub_8286C878(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69308);
PPC_WEAK_FUNC(sub_82A69308) { __imp__sub_82A69308(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69308) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32082
	ctx.r11.s64 = -2102525952;
	// li r6,126
	ctx.r6.s64 = 126;
	// addi r5,r11,16960
	ctx.r5.s64 = ctx.r11.s64 + 16960;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r4,r11,-29872
	ctx.r4.s64 = ctx.r11.s64 + -29872;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,-3792
	ctx.r3.s64 = ctx.r11.s64 + -3792;
	// b 0x8286c878
	sub_8286C878(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69328);
PPC_WEAK_FUNC(sub_82A69328) { __imp__sub_82A69328(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69328) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-29844
	ctx.r5.s64 = ctx.r11.s64 + -29844;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-3708
	ctx.r3.s64 = ctx.r11.s64 + -3708;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69348);
PPC_WEAK_FUNC(sub_82A69348) { __imp__sub_82A69348(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69348) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-29828
	ctx.r5.s64 = ctx.r11.s64 + -29828;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-3744
	ctx.r3.s64 = ctx.r11.s64 + -3744;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69368);
PPC_WEAK_FUNC(sub_82A69368) { __imp__sub_82A69368(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69368) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-29356
	ctx.r5.s64 = ctx.r11.s64 + -29356;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-3636
	ctx.r3.s64 = ctx.r11.s64 + -3636;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69388);
PPC_WEAK_FUNC(sub_82A69388) { __imp__sub_82A69388(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69388) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-29344
	ctx.r5.s64 = ctx.r11.s64 + -29344;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-3676
	ctx.r3.s64 = ctx.r11.s64 + -3676;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A693A8);
PPC_WEAK_FUNC(sub_82A693A8) { __imp__sub_82A693A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A693A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-29324
	ctx.r5.s64 = ctx.r11.s64 + -29324;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-3656
	ctx.r3.s64 = ctx.r11.s64 + -3656;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A693C8);
PPC_WEAK_FUNC(sub_82A693C8) { __imp__sub_82A693C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A693C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-26824
	ctx.r5.s64 = ctx.r11.s64 + -26824;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-3600
	ctx.r3.s64 = ctx.r11.s64 + -3600;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A693E8);
PPC_WEAK_FUNC(sub_82A693E8) { __imp__sub_82A693E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A693E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-26808
	ctx.r5.s64 = ctx.r11.s64 + -26808;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-3580
	ctx.r3.s64 = ctx.r11.s64 + -3580;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69408);
PPC_WEAK_FUNC(sub_82A69408) { __imp__sub_82A69408(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69408) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32082
	ctx.r11.s64 = -2102525952;
	// li r6,3202
	ctx.r6.s64 = 3202;
	// addi r5,r11,18592
	ctx.r5.s64 = ctx.r11.s64 + 18592;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r4,r11,-26368
	ctx.r4.s64 = ctx.r11.s64 + -26368;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,-3528
	ctx.r3.s64 = ctx.r11.s64 + -3528;
	// b 0x8286c878
	sub_8286C878(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69428);
PPC_WEAK_FUNC(sub_82A69428) { __imp__sub_82A69428(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69428) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-25704
	ctx.r5.s64 = ctx.r11.s64 + -25704;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-3468
	ctx.r3.s64 = ctx.r11.s64 + -3468;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69448);
PPC_WEAK_FUNC(sub_82A69448) { __imp__sub_82A69448(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69448) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32082
	ctx.r11.s64 = -2102525952;
	// addi r11,r11,23628
	ctx.r11.s64 = ctx.r11.s64 + 23628;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69468);
PPC_WEAK_FUNC(sub_82A69468) { __imp__sub_82A69468(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69468) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32082
	ctx.r11.s64 = -2102525952;
	// addi r11,r11,23636
	ctx.r11.s64 = ctx.r11.s64 + 23636;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69488);
PPC_WEAK_FUNC(sub_82A69488) { __imp__sub_82A69488(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69488) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32082
	ctx.r11.s64 = -2102525952;
	// addi r11,r11,23644
	ctx.r11.s64 = ctx.r11.s64 + 23644;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A694A8);
PPC_WEAK_FUNC(sub_82A694A8) { __imp__sub_82A694A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A694A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-22116
	ctx.r5.s64 = ctx.r11.s64 + -22116;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-1096
	ctx.r3.s64 = ctx.r11.s64 + -1096;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A694C8);
PPC_WEAK_FUNC(sub_82A694C8) { __imp__sub_82A694C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A694C8) {
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
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-20308
	ctx.r5.s64 = ctx.r11.s64 + -20308;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r31,r11,-632
	ctx.r31.s64 = ctx.r11.s64 + -632;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A694F8;
	sub_829DBFD0(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A69508;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A69510;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-20312
	ctx.r11.s64 = ctx.r11.s64 + -20312;
	// addi r3,r10,9456
	ctx.r3.s64 = ctx.r10.s64 + 9456;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A69528;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A69540);
PPC_WEAK_FUNC(sub_82A69540) { __imp__sub_82A69540(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69540) {
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
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r7,r11,-632
	ctx.r7.s64 = ctx.r11.s64 + -632;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-20292
	ctx.r6.s64 = ctx.r11.s64 + -20292;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-760
	ctx.r31.s64 = ctx.r11.s64 + -760;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A69578;
	sub_829DC008(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A69588;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A69590;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-20296
	ctx.r11.s64 = ctx.r11.s64 + -20296;
	// addi r3,r10,9536
	ctx.r3.s64 = ctx.r10.s64 + 9536;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A695A8;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A695C0);
PPC_WEAK_FUNC(sub_82A695C0) { __imp__sub_82A695C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A695C0) {
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
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r7,r11,-632
	ctx.r7.s64 = ctx.r11.s64 + -632;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-20264
	ctx.r6.s64 = ctx.r11.s64 + -20264;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-824
	ctx.r31.s64 = ctx.r11.s64 + -824;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A695F8;
	sub_829DC008(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A69608;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A69610;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-20268
	ctx.r11.s64 = ctx.r11.s64 + -20268;
	// addi r3,r10,9616
	ctx.r3.s64 = ctx.r10.s64 + 9616;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A69628;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A69640);
PPC_WEAK_FUNC(sub_82A69640) { __imp__sub_82A69640(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69640) {
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
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r5,2
	ctx.r5.s64 = 2;
	// addi r7,r11,-632
	ctx.r7.s64 = ctx.r11.s64 + -632;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-20232
	ctx.r6.s64 = ctx.r11.s64 + -20232;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-984
	ctx.r31.s64 = ctx.r11.s64 + -984;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A69678;
	sub_829DC008(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A69688;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A69690;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-20236
	ctx.r11.s64 = ctx.r11.s64 + -20236;
	// addi r3,r10,9696
	ctx.r3.s64 = ctx.r10.s64 + 9696;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A696A8;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A696C0);
PPC_WEAK_FUNC(sub_82A696C0) { __imp__sub_82A696C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A696C0) {
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
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r5,3
	ctx.r5.s64 = 3;
	// addi r7,r11,-632
	ctx.r7.s64 = ctx.r11.s64 + -632;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-20200
	ctx.r6.s64 = ctx.r11.s64 + -20200;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-696
	ctx.r31.s64 = ctx.r11.s64 + -696;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A696F8;
	sub_829DC008(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A69708;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A69710;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-20204
	ctx.r11.s64 = ctx.r11.s64 + -20204;
	// addi r3,r10,9776
	ctx.r3.s64 = ctx.r10.s64 + 9776;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A69728;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A69740);
PPC_WEAK_FUNC(sub_82A69740) { __imp__sub_82A69740(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69740) {
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
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r5,4
	ctx.r5.s64 = 4;
	// addi r7,r11,-632
	ctx.r7.s64 = ctx.r11.s64 + -632;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-20172
	ctx.r6.s64 = ctx.r11.s64 + -20172;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-1016
	ctx.r31.s64 = ctx.r11.s64 + -1016;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A69778;
	sub_829DC008(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A69788;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A69790;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-20176
	ctx.r11.s64 = ctx.r11.s64 + -20176;
	// addi r3,r10,9856
	ctx.r3.s64 = ctx.r10.s64 + 9856;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A697A8;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A697C0);
PPC_WEAK_FUNC(sub_82A697C0) { __imp__sub_82A697C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A697C0) {
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
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r5,5
	ctx.r5.s64 = 5;
	// addi r7,r11,-632
	ctx.r7.s64 = ctx.r11.s64 + -632;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-20144
	ctx.r6.s64 = ctx.r11.s64 + -20144;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-920
	ctx.r31.s64 = ctx.r11.s64 + -920;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A697F8;
	sub_829DC008(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A69808;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A69810;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-20148
	ctx.r11.s64 = ctx.r11.s64 + -20148;
	// addi r3,r10,9936
	ctx.r3.s64 = ctx.r10.s64 + 9936;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A69828;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A69840);
PPC_WEAK_FUNC(sub_82A69840) { __imp__sub_82A69840(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69840) {
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
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r5,6
	ctx.r5.s64 = 6;
	// addi r7,r11,-632
	ctx.r7.s64 = ctx.r11.s64 + -632;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-20116
	ctx.r6.s64 = ctx.r11.s64 + -20116;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-1048
	ctx.r31.s64 = ctx.r11.s64 + -1048;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A69878;
	sub_829DC008(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A69888;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A69890;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-20120
	ctx.r11.s64 = ctx.r11.s64 + -20120;
	// addi r3,r10,10016
	ctx.r3.s64 = ctx.r10.s64 + 10016;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A698A8;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A698C0);
PPC_WEAK_FUNC(sub_82A698C0) { __imp__sub_82A698C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A698C0) {
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
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r5,7
	ctx.r5.s64 = 7;
	// addi r7,r11,-632
	ctx.r7.s64 = ctx.r11.s64 + -632;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-20088
	ctx.r6.s64 = ctx.r11.s64 + -20088;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-792
	ctx.r31.s64 = ctx.r11.s64 + -792;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A698F8;
	sub_829DC008(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A69908;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A69910;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-20092
	ctx.r11.s64 = ctx.r11.s64 + -20092;
	// addi r3,r10,10096
	ctx.r3.s64 = ctx.r10.s64 + 10096;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A69928;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A69940);
PPC_WEAK_FUNC(sub_82A69940) { __imp__sub_82A69940(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69940) {
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
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r5,8
	ctx.r5.s64 = 8;
	// addi r7,r11,-632
	ctx.r7.s64 = ctx.r11.s64 + -632;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-20056
	ctx.r6.s64 = ctx.r11.s64 + -20056;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-856
	ctx.r31.s64 = ctx.r11.s64 + -856;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A69978;
	sub_829DC008(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A69988;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A69990;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-20060
	ctx.r11.s64 = ctx.r11.s64 + -20060;
	// addi r3,r10,10176
	ctx.r3.s64 = ctx.r10.s64 + 10176;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A699A8;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A699C0);
PPC_WEAK_FUNC(sub_82A699C0) { __imp__sub_82A699C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A699C0) {
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
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r5,9
	ctx.r5.s64 = 9;
	// addi r7,r11,-632
	ctx.r7.s64 = ctx.r11.s64 + -632;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-20028
	ctx.r6.s64 = ctx.r11.s64 + -20028;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-888
	ctx.r31.s64 = ctx.r11.s64 + -888;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A699F8;
	sub_829DC008(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A69A08;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A69A10;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-20032
	ctx.r11.s64 = ctx.r11.s64 + -20032;
	// addi r3,r10,10256
	ctx.r3.s64 = ctx.r10.s64 + 10256;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A69A28;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A69A40);
PPC_WEAK_FUNC(sub_82A69A40) { __imp__sub_82A69A40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69A40) {
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
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r5,10
	ctx.r5.s64 = 10;
	// addi r7,r11,-632
	ctx.r7.s64 = ctx.r11.s64 + -632;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-19992
	ctx.r6.s64 = ctx.r11.s64 + -19992;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-728
	ctx.r31.s64 = ctx.r11.s64 + -728;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A69A78;
	sub_829DC008(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A69A88;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A69A90;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-19996
	ctx.r11.s64 = ctx.r11.s64 + -19996;
	// addi r3,r10,10336
	ctx.r3.s64 = ctx.r10.s64 + 10336;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A69AA8;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A69AC0);
PPC_WEAK_FUNC(sub_82A69AC0) { __imp__sub_82A69AC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69AC0) {
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
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r5,11
	ctx.r5.s64 = 11;
	// addi r7,r11,-632
	ctx.r7.s64 = ctx.r11.s64 + -632;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-19956
	ctx.r6.s64 = ctx.r11.s64 + -19956;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-952
	ctx.r31.s64 = ctx.r11.s64 + -952;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A69AF8;
	sub_829DC008(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A69B08;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A69B10;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-19960
	ctx.r11.s64 = ctx.r11.s64 + -19960;
	// addi r3,r10,10416
	ctx.r3.s64 = ctx.r10.s64 + 10416;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A69B28;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A69B40);
PPC_WEAK_FUNC(sub_82A69B40) { __imp__sub_82A69B40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69B40) {
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
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r5,12
	ctx.r5.s64 = 12;
	// addi r7,r11,-632
	ctx.r7.s64 = ctx.r11.s64 + -632;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-19920
	ctx.r6.s64 = ctx.r11.s64 + -19920;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-664
	ctx.r31.s64 = ctx.r11.s64 + -664;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A69B78;
	sub_829DC008(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A69B88;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A69B90;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-19924
	ctx.r11.s64 = ctx.r11.s64 + -19924;
	// addi r3,r10,10496
	ctx.r3.s64 = ctx.r10.s64 + 10496;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A69BA8;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A69BC0);
PPC_WEAK_FUNC(sub_82A69BC0) { __imp__sub_82A69BC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69BC0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32081
	ctx.r11.s64 = -2102460416;
	// addi r11,r11,5588
	ctx.r11.s64 = ctx.r11.s64 + 5588;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69BE0);
PPC_WEAK_FUNC(sub_82A69BE0) { __imp__sub_82A69BE0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69BE0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32081
	ctx.r11.s64 = -2102460416;
	// addi r11,r11,5596
	ctx.r11.s64 = ctx.r11.s64 + 5596;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69C00);
PPC_WEAK_FUNC(sub_82A69C00) { __imp__sub_82A69C00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69C00) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32081
	ctx.r11.s64 = -2102460416;
	// addi r11,r11,5604
	ctx.r11.s64 = ctx.r11.s64 + 5604;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69C20);
PPC_WEAK_FUNC(sub_82A69C20) { __imp__sub_82A69C20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69C20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32082
	ctx.r11.s64 = -2102525952;
	// lis r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,26944
	ctx.r5.s64 = ctx.r11.s64 + 26944;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// ori r6,r6,38509
	ctx.r6.u64 = ctx.r6.u64 | 38509;
	// addi r4,r11,-18624
	ctx.r4.s64 = ctx.r11.s64 + -18624;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,-564
	ctx.r3.s64 = ctx.r11.s64 + -564;
	// b 0x8286c878
	sub_8286C878(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69C48);
PPC_WEAK_FUNC(sub_82A69C48) { __imp__sub_82A69C48(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69C48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32081
	ctx.r11.s64 = -2102460416;
	// li r6,5668
	ctx.r6.s64 = 5668;
	// addi r5,r11,-80
	ctx.r5.s64 = ctx.r11.s64 + -80;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r4,r11,-18588
	ctx.r4.s64 = ctx.r11.s64 + -18588;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,-580
	ctx.r3.s64 = ctx.r11.s64 + -580;
	// b 0x8286c878
	sub_8286C878(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69C68);
PPC_WEAK_FUNC(sub_82A69C68) { __imp__sub_82A69C68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69C68) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32081
	ctx.r11.s64 = -2102460416;
	// addi r11,r11,6376
	ctx.r11.s64 = ctx.r11.s64 + 6376;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69C88);
PPC_WEAK_FUNC(sub_82A69C88) { __imp__sub_82A69C88(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69C88) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32081
	ctx.r11.s64 = -2102460416;
	// li r6,5528
	ctx.r6.s64 = 5528;
	// addi r5,r11,6424
	ctx.r5.s64 = ctx.r11.s64 + 6424;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r4,r11,-16944
	ctx.r4.s64 = ctx.r11.s64 + -16944;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,-528
	ctx.r3.s64 = ctx.r11.s64 + -528;
	// b 0x8286c878
	sub_8286C878(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69CA8);
PPC_WEAK_FUNC(sub_82A69CA8) { __imp__sub_82A69CA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69CA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32081
	ctx.r11.s64 = -2102460416;
	// li r6,21886
	ctx.r6.s64 = 21886;
	// addi r5,r11,11952
	ctx.r5.s64 = ctx.r11.s64 + 11952;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r4,r11,-16908
	ctx.r4.s64 = ctx.r11.s64 + -16908;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,-512
	ctx.r3.s64 = ctx.r11.s64 + -512;
	// b 0x8286c878
	sub_8286C878(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69CC8);
PPC_WEAK_FUNC(sub_82A69CC8) { __imp__sub_82A69CC8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69CC8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-30780
	ctx.r11.s64 = ctx.r11.s64 + -30780;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69CE8);
PPC_WEAK_FUNC(sub_82A69CE8) { __imp__sub_82A69CE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69CE8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-30772
	ctx.r11.s64 = ctx.r11.s64 + -30772;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69D08);
PPC_WEAK_FUNC(sub_82A69D08) { __imp__sub_82A69D08(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69D08) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-30756
	ctx.r11.s64 = ctx.r11.s64 + -30756;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69D28);
PPC_WEAK_FUNC(sub_82A69D28) { __imp__sub_82A69D28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69D28) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-30448
	ctx.r11.s64 = ctx.r11.s64 + -30448;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69D48);
PPC_WEAK_FUNC(sub_82A69D48) { __imp__sub_82A69D48(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69D48) {
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
	// lis r11,-32129
	ctx.r11.s64 = -2105606144;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-7488
	ctx.r5.s64 = ctx.r11.s64 + -7488;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A69D70;
	sub_8284D220(ctx, base);
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// lis r9,-32123
	ctx.r9.s64 = -2105212928;
	// addi r10,r11,-30140
	ctx.r10.s64 = ctx.r11.s64 + -30140;
	// addi r9,r9,6152
	ctx.r9.s64 = ctx.r9.s64 + 6152;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
	// stw r9,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r9.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r8,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
	// stw r7,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69DC0);
PPC_WEAK_FUNC(sub_82A69DC0) { __imp__sub_82A69DC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69DC0) {
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
	// lis r11,-32129
	ctx.r11.s64 = -2105606144;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-7488
	ctx.r5.s64 = ctx.r11.s64 + -7488;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A69DE8;
	sub_8284D220(ctx, base);
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// lis r9,-32123
	ctx.r9.s64 = -2105212928;
	// addi r10,r11,-30092
	ctx.r10.s64 = ctx.r11.s64 + -30092;
	// addi r9,r9,6152
	ctx.r9.s64 = ctx.r9.s64 + 6152;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
	// stw r9,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r9.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r8,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
	// stw r7,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69E38);
PPC_WEAK_FUNC(sub_82A69E38) { __imp__sub_82A69E38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69E38) {
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
	// lis r11,-32129
	ctx.r11.s64 = -2105606144;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-7488
	ctx.r5.s64 = ctx.r11.s64 + -7488;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A69E60;
	sub_8284D220(ctx, base);
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// lis r9,-32123
	ctx.r9.s64 = -2105212928;
	// addi r10,r11,-30044
	ctx.r10.s64 = ctx.r11.s64 + -30044;
	// addi r9,r9,6152
	ctx.r9.s64 = ctx.r9.s64 + 6152;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
	// stw r9,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r9.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r8,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
	// stw r7,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69EB0);
PPC_WEAK_FUNC(sub_82A69EB0) { __imp__sub_82A69EB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69EB0) {
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
	// lis r11,-32130
	ctx.r11.s64 = -2105671680;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-22968
	ctx.r5.s64 = ctx.r11.s64 + -22968;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A69ED8;
	sub_8284D220(ctx, base);
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// lis r9,-32123
	ctx.r9.s64 = -2105212928;
	// addi r10,r11,-29996
	ctx.r10.s64 = ctx.r11.s64 + -29996;
	// addi r9,r9,6152
	ctx.r9.s64 = ctx.r9.s64 + 6152;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
	// stw r9,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r9.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r8,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
	// stw r7,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69F28);
PPC_WEAK_FUNC(sub_82A69F28) { __imp__sub_82A69F28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69F28) {
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
	// lis r11,-32130
	ctx.r11.s64 = -2105671680;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-22864
	ctx.r5.s64 = ctx.r11.s64 + -22864;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A69F50;
	sub_8284D220(ctx, base);
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// lis r9,-32123
	ctx.r9.s64 = -2105212928;
	// addi r10,r11,-29948
	ctx.r10.s64 = ctx.r11.s64 + -29948;
	// addi r9,r9,6152
	ctx.r9.s64 = ctx.r9.s64 + 6152;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
	// stw r9,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r9.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r8,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
	// stw r7,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69FA0);
PPC_WEAK_FUNC(sub_82A69FA0) { __imp__sub_82A69FA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69FA0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-29900
	ctx.r11.s64 = ctx.r11.s64 + -29900;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69FC0);
PPC_WEAK_FUNC(sub_82A69FC0) { __imp__sub_82A69FC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69FC0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-29892
	ctx.r11.s64 = ctx.r11.s64 + -29892;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A69FE0);
PPC_WEAK_FUNC(sub_82A69FE0) { __imp__sub_82A69FE0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A69FE0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-29884
	ctx.r11.s64 = ctx.r11.s64 + -29884;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A000);
PPC_WEAK_FUNC(sub_82A6A000) { __imp__sub_82A6A000(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A000) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-29876
	ctx.r11.s64 = ctx.r11.s64 + -29876;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A020);
PPC_WEAK_FUNC(sub_82A6A020) { __imp__sub_82A6A020(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A020) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-28808
	ctx.r11.s64 = ctx.r11.s64 + -28808;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A040);
PPC_WEAK_FUNC(sub_82A6A040) { __imp__sub_82A6A040(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A040) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-27992
	ctx.r11.s64 = ctx.r11.s64 + -27992;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A060);
PPC_WEAK_FUNC(sub_82A6A060) { __imp__sub_82A6A060(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A060) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-27984
	ctx.r11.s64 = ctx.r11.s64 + -27984;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A080);
PPC_WEAK_FUNC(sub_82A6A080) { __imp__sub_82A6A080(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A080) {
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
	// lis r11,-32130
	ctx.r11.s64 = -2105671680;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,19048
	ctx.r5.s64 = ctx.r11.s64 + 19048;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A6A0A8;
	sub_8284D220(ctx, base);
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// lis r9,-32123
	ctx.r9.s64 = -2105212928;
	// addi r10,r11,-27608
	ctx.r10.s64 = ctx.r11.s64 + -27608;
	// addi r9,r9,6152
	ctx.r9.s64 = ctx.r9.s64 + 6152;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
	// stw r9,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r9.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r8,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
	// stw r7,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A0F8);
PPC_WEAK_FUNC(sub_82A6A0F8) { __imp__sub_82A6A0F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A0F8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-27560
	ctx.r11.s64 = ctx.r11.s64 + -27560;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A118);
PPC_WEAK_FUNC(sub_82A6A118) { __imp__sub_82A6A118(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A118) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-27552
	ctx.r11.s64 = ctx.r11.s64 + -27552;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A138);
PPC_WEAK_FUNC(sub_82A6A138) { __imp__sub_82A6A138(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A138) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-27080
	ctx.r11.s64 = ctx.r11.s64 + -27080;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A158);
PPC_WEAK_FUNC(sub_82A6A158) { __imp__sub_82A6A158(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A158) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// lfs f0,-27084(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -27084);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f13,3432(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3432);
	ctx.f13.f64 = double(temp.f32);
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// fmuls f0,f0,f13
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f13.f64));
	// addi r11,r11,-400
	ctx.r11.s64 = ctx.r11.s64 + -400;
	// fctiwz f0,f0
	ctx.f0.s64 = std::isnan(ctx.f0.f64) ? int64_t(0x80000000U) : (ctx.f0.f64 > double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&ctx.f0.f64));
	// stfiwx f0,0,r11
	PPC_STORE_U32(ctx.r11.u32, ctx.f0.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A180);
PPC_WEAK_FUNC(sub_82A6A180) { __imp__sub_82A6A180(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A180) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// lfs f0,24004(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 24004);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r11,r11,-416
	ctx.r11.s64 = ctx.r11.s64 + -416;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A1A0);
PPC_WEAK_FUNC(sub_82A6A1A0) { __imp__sub_82A6A1A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A1A0) {
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
	// lis r11,-32129
	ctx.r11.s64 = -2105606144;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-7488
	ctx.r5.s64 = ctx.r11.s64 + -7488;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A6A1C8;
	sub_8284D220(ctx, base);
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// lis r9,-32123
	ctx.r9.s64 = -2105212928;
	// addi r10,r11,-26408
	ctx.r10.s64 = ctx.r11.s64 + -26408;
	// addi r9,r9,6152
	ctx.r9.s64 = ctx.r9.s64 + 6152;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
	// stw r9,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r9.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r8,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
	// stw r7,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A218);
PPC_WEAK_FUNC(sub_82A6A218) { __imp__sub_82A6A218(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A218) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-26360
	ctx.r11.s64 = ctx.r11.s64 + -26360;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A238);
PPC_WEAK_FUNC(sub_82A6A238) { __imp__sub_82A6A238(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A238) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,10576
	ctx.r3.s64 = ctx.r11.s64 + 10576;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A248);
PPC_WEAK_FUNC(sub_82A6A248) { __imp__sub_82A6A248(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A248) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,10608
	ctx.r3.s64 = ctx.r11.s64 + 10608;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A258);
PPC_WEAK_FUNC(sub_82A6A258) { __imp__sub_82A6A258(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A258) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-24192
	ctx.r11.s64 = ctx.r11.s64 + -24192;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A278);
PPC_WEAK_FUNC(sub_82A6A278) { __imp__sub_82A6A278(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A278) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-24184
	ctx.r11.s64 = ctx.r11.s64 + -24184;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A298);
PPC_WEAK_FUNC(sub_82A6A298) { __imp__sub_82A6A298(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A298) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-24176
	ctx.r11.s64 = ctx.r11.s64 + -24176;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A2B8);
PPC_WEAK_FUNC(sub_82A6A2B8) { __imp__sub_82A6A2B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A2B8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-23444
	ctx.r11.s64 = ctx.r11.s64 + -23444;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A2D8);
PPC_WEAK_FUNC(sub_82A6A2D8) { __imp__sub_82A6A2D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A2D8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-23436
	ctx.r11.s64 = ctx.r11.s64 + -23436;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A2F8);
PPC_WEAK_FUNC(sub_82A6A2F8) { __imp__sub_82A6A2F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A2F8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-23428
	ctx.r11.s64 = ctx.r11.s64 + -23428;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A318);
PPC_WEAK_FUNC(sub_82A6A318) { __imp__sub_82A6A318(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A318) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-23420
	ctx.r11.s64 = ctx.r11.s64 + -23420;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A338);
PPC_WEAK_FUNC(sub_82A6A338) { __imp__sub_82A6A338(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A338) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-23412
	ctx.r11.s64 = ctx.r11.s64 + -23412;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A358);
PPC_WEAK_FUNC(sub_82A6A358) { __imp__sub_82A6A358(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A358) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-23404
	ctx.r11.s64 = ctx.r11.s64 + -23404;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A378);
PPC_WEAK_FUNC(sub_82A6A378) { __imp__sub_82A6A378(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A378) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-5764
	ctx.r5.s64 = ctx.r11.s64 + -5764;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,912
	ctx.r3.s64 = ctx.r11.s64 + 912;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A398);
PPC_WEAK_FUNC(sub_82A6A398) { __imp__sub_82A6A398(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A398) {
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
	// lis r11,-32129
	ctx.r11.s64 = -2105606144;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-7488
	ctx.r5.s64 = ctx.r11.s64 + -7488;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A6A3C0;
	sub_8284D220(ctx, base);
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// lis r9,-32123
	ctx.r9.s64 = -2105212928;
	// addi r10,r11,-23088
	ctx.r10.s64 = ctx.r11.s64 + -23088;
	// addi r9,r9,6152
	ctx.r9.s64 = ctx.r9.s64 + 6152;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
	// stw r9,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r9.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r8,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
	// stw r7,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A410);
PPC_WEAK_FUNC(sub_82A6A410) { __imp__sub_82A6A410(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A410) {
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
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,960
	ctx.r31.s64 = ctx.r11.s64 + 960;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x827fa048
	ctx.lr = 0x82A6A430;
	sub_827FA048(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r3,r10,10640
	ctx.r3.s64 = ctx.r10.s64 + 10640;
	// stw r11,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r11.u32);
	// sth r11,16(r31)
	PPC_STORE_U16(ctx.r31.u32 + 16, ctx.r11.u16);
	// sth r11,18(r31)
	PPC_STORE_U16(ctx.r31.u32 + 18, ctx.r11.u16);
	// bl 0x829ffa48
	ctx.lr = 0x82A6A44C;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6A460);
PPC_WEAK_FUNC(sub_82A6A460) { __imp__sub_82A6A460(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A460) {
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
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,1100
	ctx.r31.s64 = ctx.r11.s64 + 1100;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x827fa048
	ctx.lr = 0x82A6A480;
	sub_827FA048(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r3,r10,10856
	ctx.r3.s64 = ctx.r10.s64 + 10856;
	// stw r11,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r11.u32);
	// sth r11,16(r31)
	PPC_STORE_U16(ctx.r31.u32 + 16, ctx.r11.u16);
	// sth r11,18(r31)
	PPC_STORE_U16(ctx.r31.u32 + 18, ctx.r11.u16);
	// bl 0x829ffa48
	ctx.lr = 0x82A6A49C;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6A4B0);
PPC_WEAK_FUNC(sub_82A6A4B0) { __imp__sub_82A6A4B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A4B0) {
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
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,1080
	ctx.r31.s64 = ctx.r11.s64 + 1080;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x827fa048
	ctx.lr = 0x82A6A4D0;
	sub_827FA048(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r3,r10,10712
	ctx.r3.s64 = ctx.r10.s64 + 10712;
	// stw r11,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r11.u32);
	// sth r11,16(r31)
	PPC_STORE_U16(ctx.r31.u32 + 16, ctx.r11.u16);
	// sth r11,18(r31)
	PPC_STORE_U16(ctx.r31.u32 + 18, ctx.r11.u16);
	// bl 0x829ffa48
	ctx.lr = 0x82A6A4EC;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6A500);
PPC_WEAK_FUNC(sub_82A6A500) { __imp__sub_82A6A500(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A500) {
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
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,1120
	ctx.r31.s64 = ctx.r11.s64 + 1120;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x827fa048
	ctx.lr = 0x82A6A520;
	sub_827FA048(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r3,r10,10784
	ctx.r3.s64 = ctx.r10.s64 + 10784;
	// stw r11,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r11.u32);
	// sth r11,16(r31)
	PPC_STORE_U16(ctx.r31.u32 + 16, ctx.r11.u16);
	// sth r11,18(r31)
	PPC_STORE_U16(ctx.r31.u32 + 18, ctx.r11.u16);
	// bl 0x829ffa48
	ctx.lr = 0x82A6A53C;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6A550);
PPC_WEAK_FUNC(sub_82A6A550) { __imp__sub_82A6A550(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A550) {
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
	// lis r11,-32128
	ctx.r11.s64 = -2105540608;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-19544
	ctx.r5.s64 = ctx.r11.s64 + -19544;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A6A578;
	sub_8284D220(ctx, base);
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// lis r9,-32123
	ctx.r9.s64 = -2105212928;
	// addi r10,r11,-22512
	ctx.r10.s64 = ctx.r11.s64 + -22512;
	// addi r9,r9,6152
	ctx.r9.s64 = ctx.r9.s64 + 6152;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
	// stw r9,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r9.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r8,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
	// stw r7,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A5C8);
PPC_WEAK_FUNC(sub_82A6A5C8) { __imp__sub_82A6A5C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A5C8) {
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
	// lis r11,-32129
	ctx.r11.s64 = -2105606144;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-7488
	ctx.r5.s64 = ctx.r11.s64 + -7488;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A6A5F0;
	sub_8284D220(ctx, base);
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// lis r9,-32123
	ctx.r9.s64 = -2105212928;
	// addi r10,r11,-22464
	ctx.r10.s64 = ctx.r11.s64 + -22464;
	// addi r9,r9,6152
	ctx.r9.s64 = ctx.r9.s64 + 6152;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
	// stw r9,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r9.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r8,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
	// stw r7,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A640);
PPC_WEAK_FUNC(sub_82A6A640) { __imp__sub_82A6A640(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A640) {
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
	// lis r11,-32129
	ctx.r11.s64 = -2105606144;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-7488
	ctx.r5.s64 = ctx.r11.s64 + -7488;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A6A668;
	sub_8284D220(ctx, base);
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// lis r9,-32123
	ctx.r9.s64 = -2105212928;
	// addi r10,r11,-22416
	ctx.r10.s64 = ctx.r11.s64 + -22416;
	// addi r9,r9,6152
	ctx.r9.s64 = ctx.r9.s64 + 6152;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
	// stw r9,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r9.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r8,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
	// stw r7,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A6B8);
PPC_WEAK_FUNC(sub_82A6A6B8) { __imp__sub_82A6A6B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A6B8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-22368
	ctx.r11.s64 = ctx.r11.s64 + -22368;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A6D8);
PPC_WEAK_FUNC(sub_82A6A6D8) { __imp__sub_82A6A6D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A6D8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-22360
	ctx.r11.s64 = ctx.r11.s64 + -22360;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A6F8);
PPC_WEAK_FUNC(sub_82A6A6F8) { __imp__sub_82A6A6F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A6F8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-22352
	ctx.r11.s64 = ctx.r11.s64 + -22352;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A718);
PPC_WEAK_FUNC(sub_82A6A718) { __imp__sub_82A6A718(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A718) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-21188
	ctx.r11.s64 = ctx.r11.s64 + -21188;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A738);
PPC_WEAK_FUNC(sub_82A6A738) { __imp__sub_82A6A738(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A738) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-21180
	ctx.r11.s64 = ctx.r11.s64 + -21180;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A758);
PPC_WEAK_FUNC(sub_82A6A758) { __imp__sub_82A6A758(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A758) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-21172
	ctx.r11.s64 = ctx.r11.s64 + -21172;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A778);
PPC_WEAK_FUNC(sub_82A6A778) { __imp__sub_82A6A778(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A778) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-21164
	ctx.r11.s64 = ctx.r11.s64 + -21164;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A798);
PPC_WEAK_FUNC(sub_82A6A798) { __imp__sub_82A6A798(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A798) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-21156
	ctx.r11.s64 = ctx.r11.s64 + -21156;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A7B8);
PPC_WEAK_FUNC(sub_82A6A7B8) { __imp__sub_82A6A7B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A7B8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-21148
	ctx.r11.s64 = ctx.r11.s64 + -21148;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A7D8);
PPC_WEAK_FUNC(sub_82A6A7D8) { __imp__sub_82A6A7D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A7D8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-21140
	ctx.r11.s64 = ctx.r11.s64 + -21140;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A7F8);
PPC_WEAK_FUNC(sub_82A6A7F8) { __imp__sub_82A6A7F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A7F8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-21132
	ctx.r11.s64 = ctx.r11.s64 + -21132;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A818);
PPC_WEAK_FUNC(sub_82A6A818) { __imp__sub_82A6A818(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A818) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-21124
	ctx.r11.s64 = ctx.r11.s64 + -21124;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A838);
PPC_WEAK_FUNC(sub_82A6A838) { __imp__sub_82A6A838(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A838) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-21116
	ctx.r11.s64 = ctx.r11.s64 + -21116;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A858);
PPC_WEAK_FUNC(sub_82A6A858) { __imp__sub_82A6A858(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A858) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,10928
	ctx.r3.s64 = ctx.r11.s64 + 10928;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6A868);
PPC_WEAK_FUNC(sub_82A6A868) { __imp__sub_82A6A868(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A868) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r5,r11,1760
	ctx.r5.s64 = ctx.r11.s64 + 1760;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,1824
	ctx.r31.s64 = ctx.r11.s64 + 1824;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6A89C;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6A8AC;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6A8B4;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,1756
	ctx.r11.s64 = ctx.r11.s64 + 1756;
	// addi r3,r10,10944
	ctx.r3.s64 = ctx.r10.s64 + 10944;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6A8CC;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6A8E0);
PPC_WEAK_FUNC(sub_82A6A8E0) { __imp__sub_82A6A8E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A8E0) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r5,r11,1788
	ctx.r5.s64 = ctx.r11.s64 + 1788;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,1952
	ctx.r31.s64 = ctx.r11.s64 + 1952;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6A914;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6A924;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6A92C;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,1784
	ctx.r11.s64 = ctx.r11.s64 + 1784;
	// addi r3,r10,11024
	ctx.r3.s64 = ctx.r10.s64 + 11024;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6A944;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6A958);
PPC_WEAK_FUNC(sub_82A6A958) { __imp__sub_82A6A958(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A958) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r5,r11,1816
	ctx.r5.s64 = ctx.r11.s64 + 1816;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,1536
	ctx.r31.s64 = ctx.r11.s64 + 1536;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6A98C;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6A99C;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6A9A4;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,1812
	ctx.r11.s64 = ctx.r11.s64 + 1812;
	// addi r3,r10,11104
	ctx.r3.s64 = ctx.r10.s64 + 11104;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6A9BC;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6A9D0);
PPC_WEAK_FUNC(sub_82A6A9D0) { __imp__sub_82A6A9D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6A9D0) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r5,r11,1836
	ctx.r5.s64 = ctx.r11.s64 + 1836;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,1920
	ctx.r31.s64 = ctx.r11.s64 + 1920;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6AA04;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6AA14;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6AA1C;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,1832
	ctx.r11.s64 = ctx.r11.s64 + 1832;
	// addi r3,r10,11184
	ctx.r3.s64 = ctx.r10.s64 + 11184;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6AA34;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6AA48);
PPC_WEAK_FUNC(sub_82A6AA48) { __imp__sub_82A6AA48(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6AA48) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r5,r11,1864
	ctx.r5.s64 = ctx.r11.s64 + 1864;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,1856
	ctx.r31.s64 = ctx.r11.s64 + 1856;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6AA7C;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6AA8C;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6AA94;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,1860
	ctx.r11.s64 = ctx.r11.s64 + 1860;
	// addi r3,r10,11264
	ctx.r3.s64 = ctx.r10.s64 + 11264;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6AAAC;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6AAC0);
PPC_WEAK_FUNC(sub_82A6AAC0) { __imp__sub_82A6AAC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6AAC0) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r5,r11,1892
	ctx.r5.s64 = ctx.r11.s64 + 1892;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,1600
	ctx.r31.s64 = ctx.r11.s64 + 1600;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6AAF4;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6AB04;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6AB0C;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,1888
	ctx.r11.s64 = ctx.r11.s64 + 1888;
	// addi r3,r10,11344
	ctx.r3.s64 = ctx.r10.s64 + 11344;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6AB24;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6AB38);
PPC_WEAK_FUNC(sub_82A6AB38) { __imp__sub_82A6AB38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6AB38) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r5,r11,1928
	ctx.r5.s64 = ctx.r11.s64 + 1928;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,1760
	ctx.r31.s64 = ctx.r11.s64 + 1760;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6AB6C;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6AB7C;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6AB84;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,1924
	ctx.r11.s64 = ctx.r11.s64 + 1924;
	// addi r3,r10,11424
	ctx.r3.s64 = ctx.r10.s64 + 11424;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6AB9C;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6ABB0);
PPC_WEAK_FUNC(sub_82A6ABB0) { __imp__sub_82A6ABB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6ABB0) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r5,r11,2116
	ctx.r5.s64 = ctx.r11.s64 + 2116;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,1472
	ctx.r31.s64 = ctx.r11.s64 + 1472;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6ABE4;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6ABF4;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6ABFC;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,2112
	ctx.r11.s64 = ctx.r11.s64 + 2112;
	// addi r3,r10,11504
	ctx.r3.s64 = ctx.r10.s64 + 11504;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6AC14;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6AC28);
PPC_WEAK_FUNC(sub_82A6AC28) { __imp__sub_82A6AC28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6AC28) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r5,r11,1968
	ctx.r5.s64 = ctx.r11.s64 + 1968;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,1632
	ctx.r31.s64 = ctx.r11.s64 + 1632;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6AC5C;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6AC6C;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6AC74;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,1964
	ctx.r11.s64 = ctx.r11.s64 + 1964;
	// addi r3,r10,11584
	ctx.r3.s64 = ctx.r10.s64 + 11584;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6AC8C;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6ACA0);
PPC_WEAK_FUNC(sub_82A6ACA0) { __imp__sub_82A6ACA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6ACA0) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r5,r11,2000
	ctx.r5.s64 = ctx.r11.s64 + 2000;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,1984
	ctx.r31.s64 = ctx.r11.s64 + 1984;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6ACD4;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6ACE4;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6ACEC;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,1996
	ctx.r11.s64 = ctx.r11.s64 + 1996;
	// addi r3,r10,11664
	ctx.r3.s64 = ctx.r10.s64 + 11664;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6AD04;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6AD18);
PPC_WEAK_FUNC(sub_82A6AD18) { __imp__sub_82A6AD18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6AD18) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r5,r11,2036
	ctx.r5.s64 = ctx.r11.s64 + 2036;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,1792
	ctx.r31.s64 = ctx.r11.s64 + 1792;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6AD4C;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6AD5C;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6AD64;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,2032
	ctx.r11.s64 = ctx.r11.s64 + 2032;
	// addi r3,r10,11744
	ctx.r3.s64 = ctx.r10.s64 + 11744;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6AD7C;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6AD90);
PPC_WEAK_FUNC(sub_82A6AD90) { __imp__sub_82A6AD90(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6AD90) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r5,r11,2076
	ctx.r5.s64 = ctx.r11.s64 + 2076;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,1568
	ctx.r31.s64 = ctx.r11.s64 + 1568;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6ADC4;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6ADD4;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6ADDC;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,2072
	ctx.r11.s64 = ctx.r11.s64 + 2072;
	// addi r3,r10,11824
	ctx.r3.s64 = ctx.r10.s64 + 11824;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6ADF4;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6AE08);
PPC_WEAK_FUNC(sub_82A6AE08) { __imp__sub_82A6AE08(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6AE08) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r5,r11,2144
	ctx.r5.s64 = ctx.r11.s64 + 2144;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,1696
	ctx.r31.s64 = ctx.r11.s64 + 1696;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6AE3C;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6AE4C;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6AE54;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,2140
	ctx.r11.s64 = ctx.r11.s64 + 2140;
	// addi r3,r10,11904
	ctx.r3.s64 = ctx.r10.s64 + 11904;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6AE6C;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6AE80);
PPC_WEAK_FUNC(sub_82A6AE80) { __imp__sub_82A6AE80(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6AE80) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r5,r11,2180
	ctx.r5.s64 = ctx.r11.s64 + 2180;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,1504
	ctx.r31.s64 = ctx.r11.s64 + 1504;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6AEB4;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6AEC4;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6AECC;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,2176
	ctx.r11.s64 = ctx.r11.s64 + 2176;
	// addi r3,r10,11984
	ctx.r3.s64 = ctx.r10.s64 + 11984;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6AEE4;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6AEF8);
PPC_WEAK_FUNC(sub_82A6AEF8) { __imp__sub_82A6AEF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6AEF8) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r5,r11,2216
	ctx.r5.s64 = ctx.r11.s64 + 2216;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,1888
	ctx.r31.s64 = ctx.r11.s64 + 1888;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6AF2C;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6AF3C;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6AF44;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,2212
	ctx.r11.s64 = ctx.r11.s64 + 2212;
	// addi r3,r10,12064
	ctx.r3.s64 = ctx.r10.s64 + 12064;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6AF5C;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6AF70);
PPC_WEAK_FUNC(sub_82A6AF70) { __imp__sub_82A6AF70(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6AF70) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r5,r11,2256
	ctx.r5.s64 = ctx.r11.s64 + 2256;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,1728
	ctx.r31.s64 = ctx.r11.s64 + 1728;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6AFA4;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6AFB4;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6AFBC;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,2252
	ctx.r11.s64 = ctx.r11.s64 + 2252;
	// addi r3,r10,12144
	ctx.r3.s64 = ctx.r10.s64 + 12144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6AFD4;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6AFE8);
PPC_WEAK_FUNC(sub_82A6AFE8) { __imp__sub_82A6AFE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6AFE8) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r5,r11,2288
	ctx.r5.s64 = ctx.r11.s64 + 2288;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,1664
	ctx.r31.s64 = ctx.r11.s64 + 1664;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6B01C;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6B02C;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6B034;
	sub_829DC040(ctx, base);
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,2284
	ctx.r11.s64 = ctx.r11.s64 + 2284;
	// addi r3,r10,12224
	ctx.r3.s64 = ctx.r10.s64 + 12224;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6B04C;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6B060);
PPC_WEAK_FUNC(sub_82A6B060) { __imp__sub_82A6B060(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B060) {
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
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r30,63
	ctx.r30.s64 = 63;
	// addi r31,r11,2024
	ctx.r31.s64 = ctx.r11.s64 + 2024;
loc_82A6B080:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829e4c30
	ctx.lr = 0x82A6B088;
	sub_829E4C30(ctx, base);
	// addi r3,r31,36
	ctx.r3.s64 = ctx.r31.s64 + 36;
	// bl 0x829fa860
	ctx.lr = 0x82A6B090;
	sub_829FA860(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// addi r31,r31,112
	ctx.r31.s64 = ctx.r31.s64 + 112;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a6b080
	if (!ctx.cr6.lt) goto loc_82A6B080;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,12304
	ctx.r3.s64 = ctx.r11.s64 + 12304;
	// bl 0x829ffa48
	ctx.lr = 0x82A6B0AC;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6B0C8);
PPC_WEAK_FUNC(sub_82A6B0C8) { __imp__sub_82A6B0C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B0C8) {
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
	// lis r11,-32127
	ctx.r11.s64 = -2105475072;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-27360
	ctx.r5.s64 = ctx.r11.s64 + -27360;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A6B0F0;
	sub_8284D220(ctx, base);
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// lis r9,-32123
	ctx.r9.s64 = -2105212928;
	// addi r10,r11,19968
	ctx.r10.s64 = ctx.r11.s64 + 19968;
	// addi r9,r9,6152
	ctx.r9.s64 = ctx.r9.s64 + 6152;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
	// stw r9,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r9.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r8,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
	// stw r7,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B140);
PPC_WEAK_FUNC(sub_82A6B140) { __imp__sub_82A6B140(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B140) {
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
	// lis r11,-32129
	ctx.r11.s64 = -2105606144;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-7488
	ctx.r5.s64 = ctx.r11.s64 + -7488;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A6B168;
	sub_8284D220(ctx, base);
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// lis r9,-32123
	ctx.r9.s64 = -2105212928;
	// addi r10,r11,20016
	ctx.r10.s64 = ctx.r11.s64 + 20016;
	// addi r9,r9,6152
	ctx.r9.s64 = ctx.r9.s64 + 6152;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
	// stw r9,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r9.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r8,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
	// stw r7,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B1B8);
PPC_WEAK_FUNC(sub_82A6B1B8) { __imp__sub_82A6B1B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B1B8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,20160
	ctx.r11.s64 = ctx.r11.s64 + 20160;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B1D8);
PPC_WEAK_FUNC(sub_82A6B1D8) { __imp__sub_82A6B1D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B1D8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,20168
	ctx.r11.s64 = ctx.r11.s64 + 20168;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B1F8);
PPC_WEAK_FUNC(sub_82A6B1F8) { __imp__sub_82A6B1F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B1F8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,20176
	ctx.r11.s64 = ctx.r11.s64 + 20176;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B218);
PPC_WEAK_FUNC(sub_82A6B218) { __imp__sub_82A6B218(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B218) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,20184
	ctx.r11.s64 = ctx.r11.s64 + 20184;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B238);
PPC_WEAK_FUNC(sub_82A6B238) { __imp__sub_82A6B238(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B238) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,20192
	ctx.r11.s64 = ctx.r11.s64 + 20192;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B258);
PPC_WEAK_FUNC(sub_82A6B258) { __imp__sub_82A6B258(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B258) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,20200
	ctx.r11.s64 = ctx.r11.s64 + 20200;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B278);
PPC_WEAK_FUNC(sub_82A6B278) { __imp__sub_82A6B278(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B278) {
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
	// lis r11,-32129
	ctx.r11.s64 = -2105606144;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-7488
	ctx.r5.s64 = ctx.r11.s64 + -7488;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A6B2A0;
	sub_8284D220(ctx, base);
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// lis r9,-32123
	ctx.r9.s64 = -2105212928;
	// addi r10,r11,20412
	ctx.r10.s64 = ctx.r11.s64 + 20412;
	// addi r9,r9,6152
	ctx.r9.s64 = ctx.r9.s64 + 6152;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
	// stw r9,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r9.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r8,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
	// stw r7,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B2F0);
PPC_WEAK_FUNC(sub_82A6B2F0) { __imp__sub_82A6B2F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B2F0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,20460
	ctx.r11.s64 = ctx.r11.s64 + 20460;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B310);
PPC_WEAK_FUNC(sub_82A6B310) { __imp__sub_82A6B310(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B310) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,21048
	ctx.r11.s64 = ctx.r11.s64 + 21048;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B330);
PPC_WEAK_FUNC(sub_82A6B330) { __imp__sub_82A6B330(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B330) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,21056
	ctx.r11.s64 = ctx.r11.s64 + 21056;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B350);
PPC_WEAK_FUNC(sub_82A6B350) { __imp__sub_82A6B350(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B350) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,21064
	ctx.r11.s64 = ctx.r11.s64 + 21064;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B370);
PPC_WEAK_FUNC(sub_82A6B370) { __imp__sub_82A6B370(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B370) {
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
	// lis r11,-32129
	ctx.r11.s64 = -2105606144;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-7488
	ctx.r5.s64 = ctx.r11.s64 + -7488;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A6B398;
	sub_8284D220(ctx, base);
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// lis r9,-32123
	ctx.r9.s64 = -2105212928;
	// addi r10,r11,22632
	ctx.r10.s64 = ctx.r11.s64 + 22632;
	// addi r9,r9,6152
	ctx.r9.s64 = ctx.r9.s64 + 6152;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
	// stw r9,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r9.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r8,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
	// stw r7,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B3E8);
PPC_WEAK_FUNC(sub_82A6B3E8) { __imp__sub_82A6B3E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B3E8) {
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
	// lis r11,-32127
	ctx.r11.s64 = -2105475072;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-10504
	ctx.r5.s64 = ctx.r11.s64 + -10504;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A6B410;
	sub_8284D220(ctx, base);
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// lis r9,-32123
	ctx.r9.s64 = -2105212928;
	// addi r10,r11,22680
	ctx.r10.s64 = ctx.r11.s64 + 22680;
	// addi r9,r9,6152
	ctx.r9.s64 = ctx.r9.s64 + 6152;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
	// stw r9,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r9.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r8,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
	// stw r7,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B460);
PPC_WEAK_FUNC(sub_82A6B460) { __imp__sub_82A6B460(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B460) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,22728
	ctx.r11.s64 = ctx.r11.s64 + 22728;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B480);
PPC_WEAK_FUNC(sub_82A6B480) { __imp__sub_82A6B480(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B480) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,22736
	ctx.r11.s64 = ctx.r11.s64 + 22736;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B4A0);
PPC_WEAK_FUNC(sub_82A6B4A0) { __imp__sub_82A6B4A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B4A0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,22744
	ctx.r11.s64 = ctx.r11.s64 + 22744;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B4C0);
PPC_WEAK_FUNC(sub_82A6B4C0) { __imp__sub_82A6B4C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B4C0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,22752
	ctx.r11.s64 = ctx.r11.s64 + 22752;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B4E0);
PPC_WEAK_FUNC(sub_82A6B4E0) { __imp__sub_82A6B4E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B4E0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,22760
	ctx.r11.s64 = ctx.r11.s64 + 22760;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B500);
PPC_WEAK_FUNC(sub_82A6B500) { __imp__sub_82A6B500(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B500) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,26420
	ctx.r11.s64 = ctx.r11.s64 + 26420;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B520);
PPC_WEAK_FUNC(sub_82A6B520) { __imp__sub_82A6B520(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B520) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,26428
	ctx.r11.s64 = ctx.r11.s64 + 26428;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B540);
PPC_WEAK_FUNC(sub_82A6B540) { __imp__sub_82A6B540(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B540) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,26436
	ctx.r11.s64 = ctx.r11.s64 + 26436;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B560);
PPC_WEAK_FUNC(sub_82A6B560) { __imp__sub_82A6B560(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B560) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,26444
	ctx.r11.s64 = ctx.r11.s64 + 26444;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B580);
PPC_WEAK_FUNC(sub_82A6B580) { __imp__sub_82A6B580(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B580) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,26452
	ctx.r11.s64 = ctx.r11.s64 + 26452;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B5A0);
PPC_WEAK_FUNC(sub_82A6B5A0) { __imp__sub_82A6B5A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B5A0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,26460
	ctx.r11.s64 = ctx.r11.s64 + 26460;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B5C0);
PPC_WEAK_FUNC(sub_82A6B5C0) { __imp__sub_82A6B5C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B5C0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,26468
	ctx.r11.s64 = ctx.r11.s64 + 26468;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B5E0);
PPC_WEAK_FUNC(sub_82A6B5E0) { __imp__sub_82A6B5E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B5E0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,26476
	ctx.r11.s64 = ctx.r11.s64 + 26476;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B600);
PPC_WEAK_FUNC(sub_82A6B600) { __imp__sub_82A6B600(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B600) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,26484
	ctx.r11.s64 = ctx.r11.s64 + 26484;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B620);
PPC_WEAK_FUNC(sub_82A6B620) { __imp__sub_82A6B620(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B620) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,26748
	ctx.r11.s64 = ctx.r11.s64 + 26748;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B640);
PPC_WEAK_FUNC(sub_82A6B640) { __imp__sub_82A6B640(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B640) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,12392
	ctx.r3.s64 = ctx.r11.s64 + 12392;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B650);
PPC_WEAK_FUNC(sub_82A6B650) { __imp__sub_82A6B650(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B650) {
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
	// lis r9,-32248
	ctx.r9.s64 = -2113404928;
	// lis r10,-32124
	ctx.r10.s64 = -2105278464;
	// addi r5,r9,13164
	ctx.r5.s64 = ctx.r9.s64 + 13164;
	// lis r11,-32124
	ctx.r11.s64 = -2105278464;
	// lis r9,-31975
	ctx.r9.s64 = -2095513600;
	// addi r7,r10,-32512
	ctx.r7.s64 = ctx.r10.s64 + -32512;
	// addi r3,r9,10012
	ctx.r3.s64 = ctx.r9.s64 + 10012;
	// addi r6,r11,-32584
	ctx.r6.s64 = ctx.r11.s64 + -32584;
	// li r4,3
	ctx.r4.s64 = 3;
	// bl 0x82838720
	ctx.lr = 0x82A6B684;
	sub_82838720(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,12408
	ctx.r3.s64 = ctx.r11.s64 + 12408;
	// bl 0x829ffa48
	ctx.lr = 0x82A6B690;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B6A0);
PPC_WEAK_FUNC(sub_82A6B6A0) { __imp__sub_82A6B6A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B6A0) {
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
	// lis r9,-32248
	ctx.r9.s64 = -2113404928;
	// lis r10,-32124
	ctx.r10.s64 = -2105278464;
	// addi r5,r9,13268
	ctx.r5.s64 = ctx.r9.s64 + 13268;
	// lis r11,-32124
	ctx.r11.s64 = -2105278464;
	// lis r9,-31975
	ctx.r9.s64 = -2095513600;
	// addi r7,r10,-31056
	ctx.r7.s64 = ctx.r10.s64 + -31056;
	// addi r3,r9,10028
	ctx.r3.s64 = ctx.r9.s64 + 10028;
	// addi r6,r11,-31176
	ctx.r6.s64 = ctx.r11.s64 + -31176;
	// li r4,4
	ctx.r4.s64 = 4;
	// bl 0x82838720
	ctx.lr = 0x82A6B6D4;
	sub_82838720(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,12424
	ctx.r3.s64 = ctx.r11.s64 + 12424;
	// bl 0x829ffa48
	ctx.lr = 0x82A6B6E0;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B6F0);
PPC_WEAK_FUNC(sub_82A6B6F0) { __imp__sub_82A6B6F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B6F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,12440
	ctx.r3.s64 = ctx.r11.s64 + 12440;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B700);
PPC_WEAK_FUNC(sub_82A6B700) { __imp__sub_82A6B700(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B700) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,12472
	ctx.r3.s64 = ctx.r11.s64 + 12472;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B710);
PPC_WEAK_FUNC(sub_82A6B710) { __imp__sub_82A6B710(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B710) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,12496
	ctx.r3.s64 = ctx.r11.s64 + 12496;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B720);
PPC_WEAK_FUNC(sub_82A6B720) { __imp__sub_82A6B720(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B720) {
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
	// lis r9,-32248
	ctx.r9.s64 = -2113404928;
	// lis r10,-32124
	ctx.r10.s64 = -2105278464;
	// addi r5,r9,14916
	ctx.r5.s64 = ctx.r9.s64 + 14916;
	// lis r11,-32124
	ctx.r11.s64 = -2105278464;
	// lis r9,-31975
	ctx.r9.s64 = -2095513600;
	// addi r7,r10,-14008
	ctx.r7.s64 = ctx.r10.s64 + -14008;
	// addi r3,r9,10068
	ctx.r3.s64 = ctx.r9.s64 + 10068;
	// addi r6,r11,-14776
	ctx.r6.s64 = ctx.r11.s64 + -14776;
	// li r4,1
	ctx.r4.s64 = 1;
	// bl 0x82838720
	ctx.lr = 0x82A6B754;
	sub_82838720(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,12528
	ctx.r3.s64 = ctx.r11.s64 + 12528;
	// bl 0x829ffa48
	ctx.lr = 0x82A6B760;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B770);
PPC_WEAK_FUNC(sub_82A6B770) { __imp__sub_82A6B770(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B770) {
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
	// lis r9,-32248
	ctx.r9.s64 = -2113404928;
	// lis r10,-32124
	ctx.r10.s64 = -2105278464;
	// addi r5,r9,15268
	ctx.r5.s64 = ctx.r9.s64 + 15268;
	// lis r11,-32124
	ctx.r11.s64 = -2105278464;
	// lis r9,-31975
	ctx.r9.s64 = -2095513600;
	// addi r7,r10,-12216
	ctx.r7.s64 = ctx.r10.s64 + -12216;
	// addi r3,r9,10084
	ctx.r3.s64 = ctx.r9.s64 + 10084;
	// addi r6,r11,-13672
	ctx.r6.s64 = ctx.r11.s64 + -13672;
	// li r4,2
	ctx.r4.s64 = 2;
	// bl 0x82838720
	ctx.lr = 0x82A6B7A4;
	sub_82838720(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,12544
	ctx.r3.s64 = ctx.r11.s64 + 12544;
	// bl 0x829ffa48
	ctx.lr = 0x82A6B7B0;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B7C0);
PPC_WEAK_FUNC(sub_82A6B7C0) { __imp__sub_82A6B7C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B7C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,12616
	ctx.r3.s64 = ctx.r11.s64 + 12616;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B7D0);
PPC_WEAK_FUNC(sub_82A6B7D0) { __imp__sub_82A6B7D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B7D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,12648
	ctx.r3.s64 = ctx.r11.s64 + 12648;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B7E0);
PPC_WEAK_FUNC(sub_82A6B7E0) { __imp__sub_82A6B7E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B7E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,12680
	ctx.r3.s64 = ctx.r11.s64 + 12680;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B7F0);
PPC_WEAK_FUNC(sub_82A6B7F0) { __imp__sub_82A6B7F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B7F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,12560
	ctx.r3.s64 = ctx.r11.s64 + 12560;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B800);
PPC_WEAK_FUNC(sub_82A6B800) { __imp__sub_82A6B800(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B800) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,12728
	ctx.r3.s64 = ctx.r11.s64 + 12728;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B810);
PPC_WEAK_FUNC(sub_82A6B810) { __imp__sub_82A6B810(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B810) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,18372
	ctx.r5.s64 = ctx.r11.s64 + 18372;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,10536
	ctx.r3.s64 = ctx.r11.s64 + 10536;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B830);
PPC_WEAK_FUNC(sub_82A6B830) { __imp__sub_82A6B830(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B830) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,18388
	ctx.r5.s64 = ctx.r11.s64 + 18388;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,10476
	ctx.r3.s64 = ctx.r11.s64 + 10476;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B850);
PPC_WEAK_FUNC(sub_82A6B850) { __imp__sub_82A6B850(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B850) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,18400
	ctx.r5.s64 = ctx.r11.s64 + 18400;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,10516
	ctx.r3.s64 = ctx.r11.s64 + 10516;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B870);
PPC_WEAK_FUNC(sub_82A6B870) { __imp__sub_82A6B870(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B870) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,18412
	ctx.r5.s64 = ctx.r11.s64 + 18412;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,10496
	ctx.r3.s64 = ctx.r11.s64 + 10496;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B890);
PPC_WEAK_FUNC(sub_82A6B890) { __imp__sub_82A6B890(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B890) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,18428
	ctx.r5.s64 = ctx.r11.s64 + 18428;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,10456
	ctx.r3.s64 = ctx.r11.s64 + 10456;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B8B0);
PPC_WEAK_FUNC(sub_82A6B8B0) { __imp__sub_82A6B8B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B8B0) {
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
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,10556
	ctx.r3.s64 = ctx.r11.s64 + 10556;
	// bl 0x8285fe48
	ctx.lr = 0x82A6B8C8;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,12784
	ctx.r3.s64 = ctx.r11.s64 + 12784;
	// bl 0x829ffa48
	ctx.lr = 0x82A6B8D4;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B8E8);
PPC_WEAK_FUNC(sub_82A6B8E8) { __imp__sub_82A6B8E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B8E8) {
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
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,10592
	ctx.r3.s64 = ctx.r11.s64 + 10592;
	// bl 0x8285fe48
	ctx.lr = 0x82A6B900;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,12800
	ctx.r3.s64 = ctx.r11.s64 + 12800;
	// bl 0x829ffa48
	ctx.lr = 0x82A6B90C;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B920);
PPC_WEAK_FUNC(sub_82A6B920) { __imp__sub_82A6B920(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B920) {
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
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,11352
	ctx.r3.s64 = ctx.r11.s64 + 11352;
	// bl 0x8284be98
	ctx.lr = 0x82A6B938;
	sub_8284BE98(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,12816
	ctx.r3.s64 = ctx.r11.s64 + 12816;
	// bl 0x829ffa48
	ctx.lr = 0x82A6B944;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B958);
PPC_WEAK_FUNC(sub_82A6B958) { __imp__sub_82A6B958(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B958) {
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
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,12240
	ctx.r3.s64 = ctx.r11.s64 + 12240;
	// bl 0x8285fe48
	ctx.lr = 0x82A6B970;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,12832
	ctx.r3.s64 = ctx.r11.s64 + 12832;
	// bl 0x829ffa48
	ctx.lr = 0x82A6B97C;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B990);
PPC_WEAK_FUNC(sub_82A6B990) { __imp__sub_82A6B990(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B990) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,21124
	ctx.r5.s64 = ctx.r11.s64 + 21124;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,12272
	ctx.r3.s64 = ctx.r11.s64 + 12272;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B9B0);
PPC_WEAK_FUNC(sub_82A6B9B0) { __imp__sub_82A6B9B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B9B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,21136
	ctx.r5.s64 = ctx.r11.s64 + 21136;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,12292
	ctx.r3.s64 = ctx.r11.s64 + 12292;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6B9D0);
PPC_WEAK_FUNC(sub_82A6B9D0) { __imp__sub_82A6B9D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6B9D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7cc
	ctx.lr = 0x82A6B9D8;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31974
	ctx.r11.s64 = -2095448064;
	// li r30,1
	ctx.r30.s64 = 1;
	// addi r11,r11,-3336
	ctx.r11.s64 = ctx.r11.s64 + -3336;
	// li r29,0
	ctx.r29.s64 = 0;
	// addi r31,r11,24940
	ctx.r31.s64 = ctx.r11.s64 + 24940;
loc_82A6B9F0:
	// addi r3,r31,-44
	ctx.r3.s64 = ctx.r31.s64 + -44;
	// bl 0x8285fe48
	ctx.lr = 0x82A6B9F8;
	sub_8285FE48(ctx, base);
	// li r3,0
	ctx.r3.s64 = 0;
	// bl 0x82849778
	ctx.lr = 0x82A6BA00;
	sub_82849778(ctx, base);
	// stw r29,-4(r31)
	PPC_STORE_U32(ctx.r31.u32 + -4, ctx.r29.u32);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// stw r29,-8(r31)
	PPC_STORE_U32(ctx.r31.u32 + -8, ctx.r29.u32);
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// stw r29,-12(r31)
	PPC_STORE_U32(ctx.r31.u32 + -12, ctx.r29.u32);
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// addi r31,r31,24944
	ctx.r31.s64 = ctx.r31.s64 + 24944;
	// bge cr6,0x82a6b9f0
	if (!ctx.cr6.lt) goto loc_82A6B9F0;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,12848
	ctx.r3.s64 = ctx.r11.s64 + 12848;
	// bl 0x829ffa48
	ctx.lr = 0x82A6BA2C;
	sub_829FFA48(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BA38);
PPC_WEAK_FUNC(sub_82A6BA38) { __imp__sub_82A6BA38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BA38) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7cc
	ctx.lr = 0x82A6BA40;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r30,1
	ctx.r30.s64 = 1;
	// addi r11,r11,12312
	ctx.r11.s64 = ctx.r11.s64 + 12312;
	// li r29,0
	ctx.r29.s64 = 0;
	// addi r31,r11,24940
	ctx.r31.s64 = ctx.r11.s64 + 24940;
loc_82A6BA58:
	// addi r3,r31,-44
	ctx.r3.s64 = ctx.r31.s64 + -44;
	// bl 0x8285fe48
	ctx.lr = 0x82A6BA60;
	sub_8285FE48(ctx, base);
	// li r3,0
	ctx.r3.s64 = 0;
	// bl 0x82849778
	ctx.lr = 0x82A6BA68;
	sub_82849778(ctx, base);
	// stw r29,-4(r31)
	PPC_STORE_U32(ctx.r31.u32 + -4, ctx.r29.u32);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// stw r29,-8(r31)
	PPC_STORE_U32(ctx.r31.u32 + -8, ctx.r29.u32);
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// stw r29,-12(r31)
	PPC_STORE_U32(ctx.r31.u32 + -12, ctx.r29.u32);
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// addi r31,r31,24944
	ctx.r31.s64 = ctx.r31.s64 + 24944;
	// bge cr6,0x82a6ba58
	if (!ctx.cr6.lt) goto loc_82A6BA58;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,12944
	ctx.r3.s64 = ctx.r11.s64 + 12944;
	// bl 0x829ffa48
	ctx.lr = 0x82A6BA94;
	sub_829FFA48(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BAA0);
PPC_WEAK_FUNC(sub_82A6BAA0) { __imp__sub_82A6BAA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BAA0) {
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
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r3,r11,-18968
	ctx.r3.s64 = ctx.r11.s64 + -18968;
	// bl 0x8285fe48
	ctx.lr = 0x82A6BAB8;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13040
	ctx.r3.s64 = ctx.r11.s64 + 13040;
	// bl 0x829ffa48
	ctx.lr = 0x82A6BAC4;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BAD8);
PPC_WEAK_FUNC(sub_82A6BAD8) { __imp__sub_82A6BAD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BAD8) {
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
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r3,r11,-18884
	ctx.r3.s64 = ctx.r11.s64 + -18884;
	// bl 0x8285fe48
	ctx.lr = 0x82A6BAF0;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13056
	ctx.r3.s64 = ctx.r11.s64 + 13056;
	// bl 0x829ffa48
	ctx.lr = 0x82A6BAFC;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BB10);
PPC_WEAK_FUNC(sub_82A6BB10) { __imp__sub_82A6BB10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BB10) {
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
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r3,r11,-18916
	ctx.r3.s64 = ctx.r11.s64 + -18916;
	// bl 0x8285fe48
	ctx.lr = 0x82A6BB28;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13072
	ctx.r3.s64 = ctx.r11.s64 + 13072;
	// bl 0x829ffa48
	ctx.lr = 0x82A6BB34;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BB48);
PPC_WEAK_FUNC(sub_82A6BB48) { __imp__sub_82A6BB48(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BB48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,21588
	ctx.r5.s64 = ctx.r11.s64 + 21588;
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-18936
	ctx.r3.s64 = ctx.r11.s64 + -18936;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BB68);
PPC_WEAK_FUNC(sub_82A6BB68) { __imp__sub_82A6BB68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BB68) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,29136
	ctx.r11.s64 = ctx.r11.s64 + 29136;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18800
	ctx.r11.s64 = ctx.r11.s64 + -18800;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BB88);
PPC_WEAK_FUNC(sub_82A6BB88) { __imp__sub_82A6BB88(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BB88) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// lfs f0,24004(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 24004);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18576
	ctx.r11.s64 = ctx.r11.s64 + -18576;
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BBA8);
PPC_WEAK_FUNC(sub_82A6BBA8) { __imp__sub_82A6BBA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BBA8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// lfs f0,24004(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 24004);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18528
	ctx.r11.s64 = ctx.r11.s64 + -18528;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BBC8);
PPC_WEAK_FUNC(sub_82A6BBC8) { __imp__sub_82A6BBC8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BBC8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// lfs f0,24004(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 24004);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18480
	ctx.r11.s64 = ctx.r11.s64 + -18480;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BBE8);
PPC_WEAK_FUNC(sub_82A6BBE8) { __imp__sub_82A6BBE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BBE8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// lfs f0,24004(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 24004);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18432
	ctx.r11.s64 = ctx.r11.s64 + -18432;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BC08);
PPC_WEAK_FUNC(sub_82A6BC08) { __imp__sub_82A6BC08(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BC08) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// lfs f0,24004(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 24004);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// stfs f0,-18544(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -18544, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BC20);
PPC_WEAK_FUNC(sub_82A6BC20) { __imp__sub_82A6BC20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BC20) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// lfs f0,24004(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 24004);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18592
	ctx.r11.s64 = ctx.r11.s64 + -18592;
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BC38);
PPC_WEAK_FUNC(sub_82A6BC38) { __imp__sub_82A6BC38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BC38) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// lfs f0,24004(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 24004);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18496
	ctx.r11.s64 = ctx.r11.s64 + -18496;
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BC50);
PPC_WEAK_FUNC(sub_82A6BC50) { __imp__sub_82A6BC50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BC50) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// lfs f0,24004(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 24004);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18448
	ctx.r11.s64 = ctx.r11.s64 + -18448;
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BC68);
PPC_WEAK_FUNC(sub_82A6BC68) { __imp__sub_82A6BC68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BC68) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// lfs f0,24004(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 24004);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18512
	ctx.r11.s64 = ctx.r11.s64 + -18512;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BC90);
PPC_WEAK_FUNC(sub_82A6BC90) { __imp__sub_82A6BC90(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BC90) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// lfs f0,24012(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 24012);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18560
	ctx.r11.s64 = ctx.r11.s64 + -18560;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BCB8);
PPC_WEAK_FUNC(sub_82A6BCB8) { __imp__sub_82A6BCB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BCB8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// lfs f0,24008(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 24008);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18464
	ctx.r11.s64 = ctx.r11.s64 + -18464;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BCE0);
PPC_WEAK_FUNC(sub_82A6BCE0) { __imp__sub_82A6BCE0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BCE0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18576
	ctx.r11.s64 = ctx.r11.s64 + -18576;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18672
	ctx.r11.s64 = ctx.r11.s64 + -18672;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BD00);
PPC_WEAK_FUNC(sub_82A6BD00) { __imp__sub_82A6BD00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BD00) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18528
	ctx.r11.s64 = ctx.r11.s64 + -18528;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18768
	ctx.r11.s64 = ctx.r11.s64 + -18768;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BD20);
PPC_WEAK_FUNC(sub_82A6BD20) { __imp__sub_82A6BD20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BD20) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18480
	ctx.r11.s64 = ctx.r11.s64 + -18480;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18656
	ctx.r11.s64 = ctx.r11.s64 + -18656;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BD40);
PPC_WEAK_FUNC(sub_82A6BD40) { __imp__sub_82A6BD40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BD40) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18432
	ctx.r11.s64 = ctx.r11.s64 + -18432;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18704
	ctx.r11.s64 = ctx.r11.s64 + -18704;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BD60);
PPC_WEAK_FUNC(sub_82A6BD60) { __imp__sub_82A6BD60(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BD60) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18544
	ctx.r11.s64 = ctx.r11.s64 + -18544;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18608
	ctx.r11.s64 = ctx.r11.s64 + -18608;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BD80);
PPC_WEAK_FUNC(sub_82A6BD80) { __imp__sub_82A6BD80(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BD80) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18592
	ctx.r11.s64 = ctx.r11.s64 + -18592;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18688
	ctx.r11.s64 = ctx.r11.s64 + -18688;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BDA0);
PPC_WEAK_FUNC(sub_82A6BDA0) { __imp__sub_82A6BDA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BDA0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18496
	ctx.r11.s64 = ctx.r11.s64 + -18496;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18752
	ctx.r11.s64 = ctx.r11.s64 + -18752;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BDC0);
PPC_WEAK_FUNC(sub_82A6BDC0) { __imp__sub_82A6BDC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BDC0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18448
	ctx.r11.s64 = ctx.r11.s64 + -18448;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18720
	ctx.r11.s64 = ctx.r11.s64 + -18720;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BDE0);
PPC_WEAK_FUNC(sub_82A6BDE0) { __imp__sub_82A6BDE0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BDE0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18512
	ctx.r11.s64 = ctx.r11.s64 + -18512;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18624
	ctx.r11.s64 = ctx.r11.s64 + -18624;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BE00);
PPC_WEAK_FUNC(sub_82A6BE00) { __imp__sub_82A6BE00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BE00) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,29072
	ctx.r11.s64 = ctx.r11.s64 + 29072;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18736
	ctx.r11.s64 = ctx.r11.s64 + -18736;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BE20);
PPC_WEAK_FUNC(sub_82A6BE20) { __imp__sub_82A6BE20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BE20) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18560
	ctx.r11.s64 = ctx.r11.s64 + -18560;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18640
	ctx.r11.s64 = ctx.r11.s64 + -18640;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BE40);
PPC_WEAK_FUNC(sub_82A6BE40) { __imp__sub_82A6BE40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BE40) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18464
	ctx.r11.s64 = ctx.r11.s64 + -18464;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18784
	ctx.r11.s64 = ctx.r11.s64 + -18784;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BE60);
PPC_WEAK_FUNC(sub_82A6BE60) { __imp__sub_82A6BE60(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BE60) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13088
	ctx.r3.s64 = ctx.r11.s64 + 13088;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BE70);
PPC_WEAK_FUNC(sub_82A6BE70) { __imp__sub_82A6BE70(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BE70) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,21936
	ctx.r5.s64 = ctx.r11.s64 + 21936;
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-18412
	ctx.r3.s64 = ctx.r11.s64 + -18412;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BE90);
PPC_WEAK_FUNC(sub_82A6BE90) { __imp__sub_82A6BE90(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BE90) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13120
	ctx.r3.s64 = ctx.r11.s64 + 13120;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BEA0);
PPC_WEAK_FUNC(sub_82A6BEA0) { __imp__sub_82A6BEA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BEA0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13096
	ctx.r3.s64 = ctx.r11.s64 + 13096;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BEB0);
PPC_WEAK_FUNC(sub_82A6BEB0) { __imp__sub_82A6BEB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BEB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13168
	ctx.r3.s64 = ctx.r11.s64 + 13168;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BEC0);
PPC_WEAK_FUNC(sub_82A6BEC0) { __imp__sub_82A6BEC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BEC0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13144
	ctx.r3.s64 = ctx.r11.s64 + 13144;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BED0);
PPC_WEAK_FUNC(sub_82A6BED0) { __imp__sub_82A6BED0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BED0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13200
	ctx.r3.s64 = ctx.r11.s64 + 13200;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BEE0);
PPC_WEAK_FUNC(sub_82A6BEE0) { __imp__sub_82A6BEE0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BEE0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13248
	ctx.r3.s64 = ctx.r11.s64 + 13248;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BEF0);
PPC_WEAK_FUNC(sub_82A6BEF0) { __imp__sub_82A6BEF0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BEF0) {
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
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r3,r11,-18064
	ctx.r3.s64 = ctx.r11.s64 + -18064;
	// bl 0x8285fe48
	ctx.lr = 0x82A6BF08;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13232
	ctx.r3.s64 = ctx.r11.s64 + 13232;
	// bl 0x829ffa48
	ctx.lr = 0x82A6BF14;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BF28);
PPC_WEAK_FUNC(sub_82A6BF28) { __imp__sub_82A6BF28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BF28) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,23956
	ctx.r5.s64 = ctx.r11.s64 + 23956;
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-18084
	ctx.r3.s64 = ctx.r11.s64 + -18084;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BF48);
PPC_WEAK_FUNC(sub_82A6BF48) { __imp__sub_82A6BF48(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BF48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,24124
	ctx.r5.s64 = ctx.r11.s64 + 24124;
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-18028
	ctx.r3.s64 = ctx.r11.s64 + -18028;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BF68);
PPC_WEAK_FUNC(sub_82A6BF68) { __imp__sub_82A6BF68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BF68) {
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
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// li r31,255
	ctx.r31.s64 = 255;
	// addi r11,r11,-17960
	ctx.r11.s64 = ctx.r11.s64 + -17960;
	// addi r30,r11,8
	ctx.r30.s64 = ctx.r11.s64 + 8;
loc_82A6BF8C:
	// li r5,148
	ctx.r5.s64 = 148;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x829ff840
	ctx.lr = 0x82A6BF9C;
	sub_829FF840(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,160
	ctx.r30.s64 = ctx.r30.s64 + 160;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x82a6bf8c
	if (!ctx.cr6.lt) goto loc_82A6BF8C;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13256
	ctx.r3.s64 = ctx.r11.s64 + 13256;
	// bl 0x829ffa48
	ctx.lr = 0x82A6BFB8;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6BFD0);
PPC_WEAK_FUNC(sub_82A6BFD0) { __imp__sub_82A6BFD0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BFD0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13264
	ctx.r3.s64 = ctx.r11.s64 + 13264;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6BFE0);
PPC_WEAK_FUNC(sub_82A6BFE0) { __imp__sub_82A6BFE0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6BFE0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,24420
	ctx.r5.s64 = ctx.r11.s64 + 24420;
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,23028
	ctx.r3.s64 = ctx.r11.s64 + 23028;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C000);
PPC_WEAK_FUNC(sub_82A6C000) { __imp__sub_82A6C000(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C000) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// lfs f0,24004(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 24004);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,23136
	ctx.r11.s64 = ctx.r11.s64 + 23136;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C020);
PPC_WEAK_FUNC(sub_82A6C020) { __imp__sub_82A6C020(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C020) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// lfs f0,24004(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 24004);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,23152
	ctx.r11.s64 = ctx.r11.s64 + 23152;
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C038);
PPC_WEAK_FUNC(sub_82A6C038) { __imp__sub_82A6C038(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C038) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
loc_82A6C038:
	// mftb r9
	ctx.r9.u64 = PPC_QUERY_TIMEBASE();
	// rotlwi r11,r9,0
	ctx.r11.u64 = __builtin_rotateleft32(ctx.r9.u32, 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a6c038
	if (ctx.cr6.eq) goto loc_82A6C038;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// lis r11,-32079
	ctx.r11.s64 = -2102329344;
	// addi r3,r10,13272
	ctx.r3.s64 = ctx.r10.s64 + 13272;
	// addi r11,r11,-31688
	ctx.r11.s64 = ctx.r11.s64 + -31688;
	// li r10,0
	ctx.r10.s64 = 0;
	// std r9,56(r11)
	PPC_STORE_U64(ctx.r11.u32 + 56, ctx.r9.u64);
	// stb r10,64(r11)
	PPC_STORE_U8(ctx.r11.u32 + 64, ctx.r10.u8);
	// stb r10,65(r11)
	PPC_STORE_U8(ctx.r11.u32 + 65, ctx.r10.u8);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lfs f0,2612(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stfs f0,72(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 72, temp.u32);
	// stw r10,68(r11)
	PPC_STORE_U32(ctx.r11.u32 + 68, ctx.r10.u32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lfs f0,3400(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 3400);
	ctx.f0.f64 = double(temp.f32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// stfs f0,76(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 76, temp.u32);
	// lfs f0,9112(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 9112);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,80(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 80, temp.u32);
	// stfs f0,84(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 84, temp.u32);
	// stfs f0,88(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 88, temp.u32);
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C0A0);
PPC_WEAK_FUNC(sub_82A6C0A0) { __imp__sub_82A6C0A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C0A0) {
	PPC_FUNC_PROLOGUE();
loc_82A6C0A0:
	// mftb r11
	ctx.r11.u64 = PPC_QUERY_TIMEBASE();
	// rotlwi r10,r11,0
	ctx.r10.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x82a6c0a0
	if (ctx.cr6.eq) goto loc_82A6C0A0;
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// std r11,23176(r10)
	PPC_STORE_U64(ctx.r10.u32 + 23176, ctx.r11.u64);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C0C0);
PPC_WEAK_FUNC(sub_82A6C0C0) { __imp__sub_82A6C0C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C0C0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,2612(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32079
	ctx.r11.s64 = -2102329344;
	// stfs f0,-16(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -16, temp.u32);
	// stfs f0,-12(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -12, temp.u32);
	// addi r11,r11,-31504
	ctx.r11.s64 = ctx.r11.s64 + -31504;
	// stfs f0,-8(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -8, temp.u32);
	// addi r10,r1,-16
	ctx.r10.s64 = ctx.r1.s64 + -16;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,16
	ctx.r10.s64 = 16;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C0F0);
PPC_WEAK_FUNC(sub_82A6C0F0) { __imp__sub_82A6C0F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C0F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,31204
	ctx.r5.s64 = ctx.r11.s64 + 31204;
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,23196
	ctx.r3.s64 = ctx.r11.s64 + 23196;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C110);
PPC_WEAK_FUNC(sub_82A6C110) { __imp__sub_82A6C110(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C110) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,23216
	ctx.r11.s64 = ctx.r11.s64 + 23216;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// stw r10,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
	// stw r10,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r10.u32);
	// stw r10,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r10.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C130);
PPC_WEAK_FUNC(sub_82A6C130) { __imp__sub_82A6C130(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C130) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// li r5,512
	ctx.r5.s64 = 512;
	// addi r3,r11,23488
	ctx.r3.s64 = ctx.r11.s64 + 23488;
	// li r4,0
	ctx.r4.s64 = 0;
	// b 0x829ff840
	sub_829FF840(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C148);
PPC_WEAK_FUNC(sub_82A6C148) { __imp__sub_82A6C148(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C148) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// li r5,256
	ctx.r5.s64 = 256;
	// addi r3,r11,23232
	ctx.r3.s64 = ctx.r11.s64 + 23232;
	// li r4,0
	ctx.r4.s64 = 0;
	// b 0x829ff840
	sub_829FF840(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C160);
PPC_WEAK_FUNC(sub_82A6C160) { __imp__sub_82A6C160(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C160) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32079
	ctx.r11.s64 = -2102329344;
	// li r9,1
	ctx.r9.s64 = 1;
	// addi r11,r11,-31144
	ctx.r11.s64 = ctx.r11.s64 + -31144;
	// li r8,0
	ctx.r8.s64 = 0;
	// addi r10,r11,40
	ctx.r10.s64 = ctx.r11.s64 + 40;
loc_82A6C174:
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// sth r8,2(r10)
	PPC_STORE_U16(ctx.r10.u32 + 2, ctx.r8.u16);
	// sth r8,0(r10)
	PPC_STORE_U16(ctx.r10.u32 + 0, ctx.r8.u16);
	// addi r10,r10,1540
	ctx.r10.s64 = ctx.r10.s64 + 1540;
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// bge cr6,0x82a6c174
	if (!ctx.cr6.lt) goto loc_82A6C174;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r3,r10,13296
	ctx.r3.s64 = ctx.r10.s64 + 13296;
	// mr r10,r8
	ctx.r10.u64 = ctx.r8.u64;
	// stw r10,3120(r11)
	PPC_STORE_U32(ctx.r11.u32 + 3120, ctx.r10.u32);
	// stw r10,3124(r11)
	PPC_STORE_U32(ctx.r11.u32 + 3124, ctx.r10.u32);
	// stb r10,3128(r11)
	PPC_STORE_U8(ctx.r11.u32 + 3128, ctx.r10.u8);
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// stw r11,24000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 24000, ctx.r11.u32);
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C1B0);
PPC_WEAK_FUNC(sub_82A6C1B0) { __imp__sub_82A6C1B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C1B0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,24016(r10)
	PPC_STORE_U32(ctx.r10.u32 + 24016, ctx.r11.u32);
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// li r11,-1
	ctx.r11.s64 = -1;
	// stw r11,24004(r10)
	PPC_STORE_U32(ctx.r10.u32 + 24004, ctx.r11.u32);
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,32640
	ctx.r11.s64 = 2139095040;
	// stw r11,24012(r10)
	PPC_STORE_U32(ctx.r10.u32 + 24012, ctx.r11.u32);
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,32704
	ctx.r11.s64 = 2143289344;
	// stw r11,24008(r10)
	PPC_STORE_U32(ctx.r10.u32 + 24008, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C1E8);
PPC_WEAK_FUNC(sub_82A6C1E8) { __imp__sub_82A6C1E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C1E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13344
	ctx.r3.s64 = ctx.r11.s64 + 13344;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C1F8);
PPC_WEAK_FUNC(sub_82A6C1F8) { __imp__sub_82A6C1F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C1F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32079
	ctx.r11.s64 = -2102329344;
	// li r6,5930
	ctx.r6.s64 = 5930;
	// addi r5,r11,-25320
	ctx.r5.s64 = ctx.r11.s64 + -25320;
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// addi r4,r11,12268
	ctx.r4.s64 = ctx.r11.s64 + 12268;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// addi r3,r11,9728
	ctx.r3.s64 = ctx.r11.s64 + 9728;
	// b 0x8286c878
	sub_8286C878(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C218);
PPC_WEAK_FUNC(sub_82A6C218) { __imp__sub_82A6C218(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C218) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,12292
	ctx.r5.s64 = ctx.r11.s64 + 12292;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,9784
	ctx.r3.s64 = ctx.r11.s64 + 9784;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C238);
PPC_WEAK_FUNC(sub_82A6C238) { __imp__sub_82A6C238(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C238) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,12296
	ctx.r5.s64 = ctx.r11.s64 + 12296;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,9764
	ctx.r3.s64 = ctx.r11.s64 + 9764;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C258);
PPC_WEAK_FUNC(sub_82A6C258) { __imp__sub_82A6C258(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C258) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,12312
	ctx.r5.s64 = ctx.r11.s64 + 12312;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,9620
	ctx.r3.s64 = ctx.r11.s64 + 9620;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C278);
PPC_WEAK_FUNC(sub_82A6C278) { __imp__sub_82A6C278(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C278) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,12316
	ctx.r5.s64 = ctx.r11.s64 + 12316;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,9356
	ctx.r3.s64 = ctx.r11.s64 + 9356;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C298);
PPC_WEAK_FUNC(sub_82A6C298) { __imp__sub_82A6C298(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C298) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,12332
	ctx.r5.s64 = ctx.r11.s64 + 12332;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,9560
	ctx.r3.s64 = ctx.r11.s64 + 9560;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C2B8);
PPC_WEAK_FUNC(sub_82A6C2B8) { __imp__sub_82A6C2B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C2B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,12348
	ctx.r5.s64 = ctx.r11.s64 + 12348;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,9600
	ctx.r3.s64 = ctx.r11.s64 + 9600;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C2D8);
PPC_WEAK_FUNC(sub_82A6C2D8) { __imp__sub_82A6C2D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C2D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,12356
	ctx.r5.s64 = ctx.r11.s64 + 12356;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,9316
	ctx.r3.s64 = ctx.r11.s64 + 9316;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C2F8);
PPC_WEAK_FUNC(sub_82A6C2F8) { __imp__sub_82A6C2F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C2F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,12368
	ctx.r5.s64 = ctx.r11.s64 + 12368;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,9380
	ctx.r3.s64 = ctx.r11.s64 + 9380;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C318);
PPC_WEAK_FUNC(sub_82A6C318) { __imp__sub_82A6C318(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C318) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,12384
	ctx.r5.s64 = ctx.r11.s64 + 12384;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,9540
	ctx.r3.s64 = ctx.r11.s64 + 9540;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C338);
PPC_WEAK_FUNC(sub_82A6C338) { __imp__sub_82A6C338(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C338) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,12400
	ctx.r5.s64 = ctx.r11.s64 + 12400;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,9400
	ctx.r3.s64 = ctx.r11.s64 + 9400;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C358);
PPC_WEAK_FUNC(sub_82A6C358) { __imp__sub_82A6C358(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C358) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,12412
	ctx.r5.s64 = ctx.r11.s64 + 12412;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,9744
	ctx.r3.s64 = ctx.r11.s64 + 9744;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C378);
PPC_WEAK_FUNC(sub_82A6C378) { __imp__sub_82A6C378(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C378) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32244
	ctx.r11.s64 = -2113142784;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-13236
	ctx.r5.s64 = ctx.r11.s64 + -13236;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,9336
	ctx.r3.s64 = ctx.r11.s64 + 9336;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C398);
PPC_WEAK_FUNC(sub_82A6C398) { __imp__sub_82A6C398(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C398) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32244
	ctx.r11.s64 = -2113142784;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-13228
	ctx.r5.s64 = ctx.r11.s64 + -13228;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,9420
	ctx.r3.s64 = ctx.r11.s64 + 9420;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C3B8);
PPC_WEAK_FUNC(sub_82A6C3B8) { __imp__sub_82A6C3B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C3B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,12416
	ctx.r5.s64 = ctx.r11.s64 + 12416;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,9580
	ctx.r3.s64 = ctx.r11.s64 + 9580;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C3D8);
PPC_WEAK_FUNC(sub_82A6C3D8) { __imp__sub_82A6C3D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C3D8) {
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
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// addi r3,r11,9640
	ctx.r3.s64 = ctx.r11.s64 + 9640;
	// bl 0x828ca888
	ctx.lr = 0x82A6C3F0;
	sub_828CA888(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13440
	ctx.r3.s64 = ctx.r11.s64 + 13440;
	// bl 0x829ffa48
	ctx.lr = 0x82A6C3FC;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C410);
PPC_WEAK_FUNC(sub_82A6C410) { __imp__sub_82A6C410(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C410) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-31972
	ctx.r10.s64 = -2095316992;
	// lwz r11,20688(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20688);
	// stw r11,9536(r10)
	PPC_STORE_U32(ctx.r10.u32 + 9536, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C428);
PPC_WEAK_FUNC(sub_82A6C428) { __imp__sub_82A6C428(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C428) {
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
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,4
	ctx.r4.s64 = 4;
	// addi r3,r11,10224
	ctx.r3.s64 = ctx.r11.s64 + 10224;
	// bl 0x828e06f8
	ctx.lr = 0x82A6C444;
	sub_828E06F8(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13456
	ctx.r3.s64 = ctx.r11.s64 + 13456;
	// bl 0x829ffa48
	ctx.lr = 0x82A6C450;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C460);
PPC_WEAK_FUNC(sub_82A6C460) { __imp__sub_82A6C460(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C460) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32079
	ctx.r11.s64 = -2102329344;
	// li r6,5881
	ctx.r6.s64 = 5881;
	// addi r5,r11,-18904
	ctx.r5.s64 = ctx.r11.s64 + -18904;
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// addi r4,r11,14504
	ctx.r4.s64 = ctx.r11.s64 + 14504;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// addi r3,r11,11700
	ctx.r3.s64 = ctx.r11.s64 + 11700;
	// b 0x8286c878
	sub_8286C878(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C480);
PPC_WEAK_FUNC(sub_82A6C480) { __imp__sub_82A6C480(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C480) {
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
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// addi r31,r11,11720
	ctx.r31.s64 = ctx.r11.s64 + 11720;
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x828ca888
	ctx.lr = 0x82A6C4A0;
	sub_828CA888(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r3,r10,13464
	ctx.r3.s64 = ctx.r10.s64 + 13464;
	// stb r11,112(r31)
	PPC_STORE_U8(ctx.r31.u32 + 112, ctx.r11.u8);
	// bl 0x829ffa48
	ctx.lr = 0x82A6C4B4;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6C4C8);
PPC_WEAK_FUNC(sub_82A6C4C8) { __imp__sub_82A6C4C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C4C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,11872
	ctx.r11.s64 = ctx.r11.s64 + 11872;
	// lis r9,-32089
	ctx.r9.s64 = -2102984704;
	// addi r3,r9,13528
	ctx.r3.s64 = ctx.r9.s64 + 13528;
	// stw r10,24(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24, ctx.r10.u32);
	// stw r10,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r10.u32);
	// stw r10,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r10.u32);
	// stw r10,52(r11)
	PPC_STORE_U32(ctx.r11.u32 + 52, ctx.r10.u32);
	// stw r10,48(r11)
	PPC_STORE_U32(ctx.r11.u32 + 48, ctx.r10.u32);
	// stw r10,44(r11)
	PPC_STORE_U32(ctx.r11.u32 + 44, ctx.r10.u32);
	// stw r10,80(r11)
	PPC_STORE_U32(ctx.r11.u32 + 80, ctx.r10.u32);
	// stw r10,76(r11)
	PPC_STORE_U32(ctx.r11.u32 + 76, ctx.r10.u32);
	// stw r10,72(r11)
	PPC_STORE_U32(ctx.r11.u32 + 72, ctx.r10.u32);
	// stw r10,108(r11)
	PPC_STORE_U32(ctx.r11.u32 + 108, ctx.r10.u32);
	// stw r10,104(r11)
	PPC_STORE_U32(ctx.r11.u32 + 104, ctx.r10.u32);
	// stw r10,100(r11)
	PPC_STORE_U32(ctx.r11.u32 + 100, ctx.r10.u32);
	// stw r10,136(r11)
	PPC_STORE_U32(ctx.r11.u32 + 136, ctx.r10.u32);
	// stw r10,132(r11)
	PPC_STORE_U32(ctx.r11.u32 + 132, ctx.r10.u32);
	// stw r10,128(r11)
	PPC_STORE_U32(ctx.r11.u32 + 128, ctx.r10.u32);
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C520);
PPC_WEAK_FUNC(sub_82A6C520) { __imp__sub_82A6C520(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C520) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13536
	ctx.r3.s64 = ctx.r11.s64 + 13536;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C530);
PPC_WEAK_FUNC(sub_82A6C530) { __imp__sub_82A6C530(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C530) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13568
	ctx.r3.s64 = ctx.r11.s64 + 13568;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C540);
PPC_WEAK_FUNC(sub_82A6C540) { __imp__sub_82A6C540(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C540) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13600
	ctx.r3.s64 = ctx.r11.s64 + 13600;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C550);
PPC_WEAK_FUNC(sub_82A6C550) { __imp__sub_82A6C550(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C550) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13616
	ctx.r3.s64 = ctx.r11.s64 + 13616;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C560);
PPC_WEAK_FUNC(sub_82A6C560) { __imp__sub_82A6C560(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C560) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,14832
	ctx.r5.s64 = ctx.r11.s64 + 14832;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,12132
	ctx.r3.s64 = ctx.r11.s64 + 12132;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C580);
PPC_WEAK_FUNC(sub_82A6C580) { __imp__sub_82A6C580(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C580) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15200
	ctx.r5.s64 = ctx.r11.s64 + 15200;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,12452
	ctx.r3.s64 = ctx.r11.s64 + 12452;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C5A0);
PPC_WEAK_FUNC(sub_82A6C5A0) { __imp__sub_82A6C5A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C5A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15212
	ctx.r5.s64 = ctx.r11.s64 + 15212;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,12392
	ctx.r3.s64 = ctx.r11.s64 + 12392;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C5C0);
PPC_WEAK_FUNC(sub_82A6C5C0) { __imp__sub_82A6C5C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C5C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15224
	ctx.r5.s64 = ctx.r11.s64 + 15224;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,12312
	ctx.r3.s64 = ctx.r11.s64 + 12312;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C5E0);
PPC_WEAK_FUNC(sub_82A6C5E0) { __imp__sub_82A6C5E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C5E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15228
	ctx.r5.s64 = ctx.r11.s64 + 15228;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,12232
	ctx.r3.s64 = ctx.r11.s64 + 12232;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C600);
PPC_WEAK_FUNC(sub_82A6C600) { __imp__sub_82A6C600(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C600) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15240
	ctx.r5.s64 = ctx.r11.s64 + 15240;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,12352
	ctx.r3.s64 = ctx.r11.s64 + 12352;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C620);
PPC_WEAK_FUNC(sub_82A6C620) { __imp__sub_82A6C620(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C620) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15248
	ctx.r5.s64 = ctx.r11.s64 + 15248;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,12292
	ctx.r3.s64 = ctx.r11.s64 + 12292;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C640);
PPC_WEAK_FUNC(sub_82A6C640) { __imp__sub_82A6C640(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C640) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15256
	ctx.r5.s64 = ctx.r11.s64 + 15256;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,12332
	ctx.r3.s64 = ctx.r11.s64 + 12332;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C660);
PPC_WEAK_FUNC(sub_82A6C660) { __imp__sub_82A6C660(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C660) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15268
	ctx.r5.s64 = ctx.r11.s64 + 15268;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,12432
	ctx.r3.s64 = ctx.r11.s64 + 12432;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C680);
PPC_WEAK_FUNC(sub_82A6C680) { __imp__sub_82A6C680(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C680) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15280
	ctx.r5.s64 = ctx.r11.s64 + 15280;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,12252
	ctx.r3.s64 = ctx.r11.s64 + 12252;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C6A0);
PPC_WEAK_FUNC(sub_82A6C6A0) { __imp__sub_82A6C6A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C6A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15296
	ctx.r5.s64 = ctx.r11.s64 + 15296;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,12412
	ctx.r3.s64 = ctx.r11.s64 + 12412;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C6C0);
PPC_WEAK_FUNC(sub_82A6C6C0) { __imp__sub_82A6C6C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C6C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15312
	ctx.r5.s64 = ctx.r11.s64 + 15312;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,12372
	ctx.r3.s64 = ctx.r11.s64 + 12372;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C6E0);
PPC_WEAK_FUNC(sub_82A6C6E0) { __imp__sub_82A6C6E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C6E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15328
	ctx.r5.s64 = ctx.r11.s64 + 15328;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,12212
	ctx.r3.s64 = ctx.r11.s64 + 12212;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C700);
PPC_WEAK_FUNC(sub_82A6C700) { __imp__sub_82A6C700(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C700) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15344
	ctx.r5.s64 = ctx.r11.s64 + 15344;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,12192
	ctx.r3.s64 = ctx.r11.s64 + 12192;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C720);
PPC_WEAK_FUNC(sub_82A6C720) { __imp__sub_82A6C720(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C720) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15356
	ctx.r5.s64 = ctx.r11.s64 + 15356;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,12172
	ctx.r3.s64 = ctx.r11.s64 + 12172;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C740);
PPC_WEAK_FUNC(sub_82A6C740) { __imp__sub_82A6C740(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C740) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15368
	ctx.r5.s64 = ctx.r11.s64 + 15368;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,12272
	ctx.r3.s64 = ctx.r11.s64 + 12272;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C760);
PPC_WEAK_FUNC(sub_82A6C760) { __imp__sub_82A6C760(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C760) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,16660
	ctx.r5.s64 = ctx.r11.s64 + 16660;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,13756
	ctx.r3.s64 = ctx.r11.s64 + 13756;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C780);
PPC_WEAK_FUNC(sub_82A6C780) { __imp__sub_82A6C780(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C780) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13632
	ctx.r3.s64 = ctx.r11.s64 + 13632;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C790);
PPC_WEAK_FUNC(sub_82A6C790) { __imp__sub_82A6C790(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C790) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13640
	ctx.r3.s64 = ctx.r11.s64 + 13640;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C7A0);
PPC_WEAK_FUNC(sub_82A6C7A0) { __imp__sub_82A6C7A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C7A0) {
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
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// addi r3,r11,15856
	ctx.r3.s64 = ctx.r11.s64 + 15856;
	// bl 0x828c9bc0
	ctx.lr = 0x82A6C7B8;
	sub_828C9BC0(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13648
	ctx.r3.s64 = ctx.r11.s64 + 13648;
	// bl 0x829ffa48
	ctx.lr = 0x82A6C7C4;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C7D8);
PPC_WEAK_FUNC(sub_82A6C7D8) { __imp__sub_82A6C7D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C7D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,3
	ctx.r4.s64 = 3;
	// addi r11,r11,20472
	ctx.r11.s64 = ctx.r11.s64 + 20472;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// li r8,128
	ctx.r8.s64 = 128;
	// li r5,512
	ctx.r5.s64 = 512;
	// li r6,1
	ctx.r6.s64 = 1;
loc_82A6C7F8:
	// stw r10,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
	// addi r9,r11,12
	ctx.r9.s64 = ctx.r11.s64 + 12;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// li r7,12
	ctx.r7.s64 = 12;
	// stb r8,11(r11)
	PPC_STORE_U8(ctx.r11.u32 + 11, ctx.r8.u8);
	// stb r8,10(r11)
	PPC_STORE_U8(ctx.r11.u32 + 10, ctx.r8.u8);
	// stb r8,9(r11)
	PPC_STORE_U8(ctx.r11.u32 + 9, ctx.r8.u8);
	// stb r8,8(r11)
	PPC_STORE_U8(ctx.r11.u32 + 8, ctx.r8.u8);
	// mtctr r7
	ctx.ctr.u64 = ctx.r7.u64;
loc_82A6C81C:
	// stb r10,0(r9)
	PPC_STORE_U8(ctx.r9.u32 + 0, ctx.r10.u8);
	// addi r9,r9,1
	ctx.r9.s64 = ctx.r9.s64 + 1;
	// bdnz 0x82a6c81c
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_82A6C81C;
	// sth r5,24(r11)
	PPC_STORE_U16(ctx.r11.u32 + 24, ctx.r5.u16);
	// addi r4,r4,-1
	ctx.r4.s64 = ctx.r4.s64 + -1;
	// sth r5,26(r11)
	PPC_STORE_U16(ctx.r11.u32 + 26, ctx.r5.u16);
	// sth r5,28(r11)
	PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r5.u16);
	// cmpwi cr6,r4,0
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 0, ctx.xer);
	// sth r5,30(r11)
	PPC_STORE_U16(ctx.r11.u32 + 30, ctx.r5.u16);
	// stb r6,32(r11)
	PPC_STORE_U8(ctx.r11.u32 + 32, ctx.r6.u8);
	// stb r6,33(r11)
	PPC_STORE_U8(ctx.r11.u32 + 33, ctx.r6.u8);
	// stb r6,34(r11)
	PPC_STORE_U8(ctx.r11.u32 + 34, ctx.r6.u8);
	// sth r10,48(r11)
	PPC_STORE_U16(ctx.r11.u32 + 48, ctx.r10.u16);
	// sth r10,46(r11)
	PPC_STORE_U16(ctx.r11.u32 + 46, ctx.r10.u16);
	// stw r10,40(r11)
	PPC_STORE_U32(ctx.r11.u32 + 40, ctx.r10.u32);
	// stb r10,35(r11)
	PPC_STORE_U8(ctx.r11.u32 + 35, ctx.r10.u8);
	// stb r10,36(r11)
	PPC_STORE_U8(ctx.r11.u32 + 36, ctx.r10.u8);
	// stb r10,37(r11)
	PPC_STORE_U8(ctx.r11.u32 + 37, ctx.r10.u8);
	// stb r6,44(r11)
	PPC_STORE_U8(ctx.r11.u32 + 44, ctx.r6.u8);
	// addi r11,r11,56
	ctx.r11.s64 = ctx.r11.s64 + 56;
	// bge cr6,0x82a6c7f8
	if (!ctx.cr6.lt) goto loc_82A6C7F8;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13664
	ctx.r3.s64 = ctx.r11.s64 + 13664;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C880);
PPC_WEAK_FUNC(sub_82A6C880) { __imp__sub_82A6C880(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C880) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13704
	ctx.r3.s64 = ctx.r11.s64 + 13704;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C890);
PPC_WEAK_FUNC(sub_82A6C890) { __imp__sub_82A6C890(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C890) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13736
	ctx.r3.s64 = ctx.r11.s64 + 13736;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C8A0);
PPC_WEAK_FUNC(sub_82A6C8A0) { __imp__sub_82A6C8A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C8A0) {
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
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// addi r3,r11,20756
	ctx.r3.s64 = ctx.r11.s64 + 20756;
	// bl 0x8285fe48
	ctx.lr = 0x82A6C8B8;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13672
	ctx.r3.s64 = ctx.r11.s64 + 13672;
	// bl 0x829ffa48
	ctx.lr = 0x82A6C8C4;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C8D8);
PPC_WEAK_FUNC(sub_82A6C8D8) { __imp__sub_82A6C8D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C8D8) {
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
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// addi r3,r11,20716
	ctx.r3.s64 = ctx.r11.s64 + 20716;
	// bl 0x8285fe48
	ctx.lr = 0x82A6C8F0;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13688
	ctx.r3.s64 = ctx.r11.s64 + 13688;
	// bl 0x829ffa48
	ctx.lr = 0x82A6C8FC;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C910);
PPC_WEAK_FUNC(sub_82A6C910) { __imp__sub_82A6C910(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C910) {
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
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// addi r3,r11,20960
	ctx.r3.s64 = ctx.r11.s64 + 20960;
	// bl 0x828d09a8
	ctx.lr = 0x82A6C928;
	sub_828D09A8(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13768
	ctx.r3.s64 = ctx.r11.s64 + 13768;
	// bl 0x829ffa48
	ctx.lr = 0x82A6C934;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C948);
PPC_WEAK_FUNC(sub_82A6C948) { __imp__sub_82A6C948(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C948) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13776
	ctx.r3.s64 = ctx.r11.s64 + 13776;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C958);
PPC_WEAK_FUNC(sub_82A6C958) { __imp__sub_82A6C958(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C958) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32079
	ctx.r11.s64 = -2102329344;
	// lis r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-9784
	ctx.r5.s64 = ctx.r11.s64 + -9784;
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// ori r6,r6,46284
	ctx.r6.u64 = ctx.r6.u64 | 46284;
	// addi r4,r11,27792
	ctx.r4.s64 = ctx.r11.s64 + 27792;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// addi r3,r11,24056
	ctx.r3.s64 = ctx.r11.s64 + 24056;
	// b 0x8286c878
	sub_8286C878(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C980);
PPC_WEAK_FUNC(sub_82A6C980) { __imp__sub_82A6C980(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C980) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,12312
	ctx.r5.s64 = ctx.r11.s64 + 12312;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,24036
	ctx.r3.s64 = ctx.r11.s64 + 24036;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C9A0);
PPC_WEAK_FUNC(sub_82A6C9A0) { __imp__sub_82A6C9A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C9A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,31728
	ctx.r5.s64 = ctx.r11.s64 + 31728;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,25208
	ctx.r3.s64 = ctx.r11.s64 + 25208;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6C9C0);
PPC_WEAK_FUNC(sub_82A6C9C0) { __imp__sub_82A6C9C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6C9C0) {
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
	// lis r11,-32125
	ctx.r11.s64 = -2105344000;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,12592
	ctx.r5.s64 = ctx.r11.s64 + 12592;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A6C9E8;
	sub_8284D220(ctx, base);
	// lis r11,-32078
	ctx.r11.s64 = -2102263808;
	// lis r9,-32123
	ctx.r9.s64 = -2105212928;
	// addi r10,r11,-28320
	ctx.r10.s64 = ctx.r11.s64 + -28320;
	// addi r9,r9,6152
	ctx.r9.s64 = ctx.r9.s64 + 6152;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
	// stw r9,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r9.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r8,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
	// stw r7,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6CA38);
PPC_WEAK_FUNC(sub_82A6CA38) { __imp__sub_82A6CA38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6CA38) {
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
	// lis r11,-32129
	ctx.r11.s64 = -2105606144;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-7488
	ctx.r5.s64 = ctx.r11.s64 + -7488;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A6CA60;
	sub_8284D220(ctx, base);
	// lis r11,-32078
	ctx.r11.s64 = -2102263808;
	// lis r9,-32123
	ctx.r9.s64 = -2105212928;
	// addi r10,r11,-28272
	ctx.r10.s64 = ctx.r11.s64 + -28272;
	// addi r9,r9,6152
	ctx.r9.s64 = ctx.r9.s64 + 6152;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
	// stw r9,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r9.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r8,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
	// stw r7,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6CAB0);
PPC_WEAK_FUNC(sub_82A6CAB0) { __imp__sub_82A6CAB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6CAB0) {
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
	// lis r11,-32129
	ctx.r11.s64 = -2105606144;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-7488
	ctx.r5.s64 = ctx.r11.s64 + -7488;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A6CAD8;
	sub_8284D220(ctx, base);
	// lis r11,-32078
	ctx.r11.s64 = -2102263808;
	// lis r9,-32123
	ctx.r9.s64 = -2105212928;
	// addi r10,r11,-28224
	ctx.r10.s64 = ctx.r11.s64 + -28224;
	// addi r9,r9,6152
	ctx.r9.s64 = ctx.r9.s64 + 6152;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
	// stw r9,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r9.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r8,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
	// stw r7,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6CB28);
PPC_WEAK_FUNC(sub_82A6CB28) { __imp__sub_82A6CB28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6CB28) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32078
	ctx.r11.s64 = -2102263808;
	// addi r10,r11,-28176
	ctx.r10.s64 = ctx.r11.s64 + -28176;
	// lwz r9,12(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 12);
	// lwz r11,4(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
loc_82A6CB38:
	// rlwinm r11,r11,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// addi r9,r9,1
	ctx.r9.s64 = ctx.r9.s64 + 1;
	// cmpwi cr6,r11,16
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 16, ctx.xer);
	// stw r11,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r11.u32);
	// blt cr6,0x82a6cb38
	if (ctx.cr6.lt) goto loc_82A6CB38;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// stw r9,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r9.u32);
	// lis r9,-32089
	ctx.r9.s64 = -2102984704;
	// addi r3,r9,13880
	ctx.r3.s64 = ctx.r9.s64 + 13880;
	// stw r11,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r11.u32);
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6CB68);
PPC_WEAK_FUNC(sub_82A6CB68) { __imp__sub_82A6CB68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6CB68) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32078
	ctx.r11.s64 = -2102263808;
	// addi r10,r11,-28008
	ctx.r10.s64 = ctx.r11.s64 + -28008;
	// lwz r9,12(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 12);
	// lwz r11,4(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
loc_82A6CB78:
	// rlwinm r11,r11,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// addi r9,r9,1
	ctx.r9.s64 = ctx.r9.s64 + 1;
	// cmpwi cr6,r11,16
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 16, ctx.xer);
	// stw r11,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r11.u32);
	// blt cr6,0x82a6cb78
	if (ctx.cr6.lt) goto loc_82A6CB78;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// stw r9,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r9.u32);
	// lis r9,-32089
	ctx.r9.s64 = -2102984704;
	// addi r3,r9,13896
	ctx.r3.s64 = ctx.r9.s64 + 13896;
	// stw r11,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r11.u32);
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6CBA8);
PPC_WEAK_FUNC(sub_82A6CBA8) { __imp__sub_82A6CBA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6CBA8) {
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
	// lis r11,-32113
	ctx.r11.s64 = -2104557568;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-10280
	ctx.r5.s64 = ctx.r11.s64 + -10280;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A6CBD0;
	sub_8284D220(ctx, base);
	// lis r11,-32078
	ctx.r11.s64 = -2102263808;
	// lis r9,-32123
	ctx.r9.s64 = -2105212928;
	// addi r10,r11,-27348
	ctx.r10.s64 = ctx.r11.s64 + -27348;
	// addi r9,r9,6152
	ctx.r9.s64 = ctx.r9.s64 + 6152;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
	// stw r9,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r9.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r8,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
	// stw r7,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6CC20);
PPC_WEAK_FUNC(sub_82A6CC20) { __imp__sub_82A6CC20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6CC20) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32078
	ctx.r11.s64 = -2102263808;
	// addi r11,r11,-27300
	ctx.r11.s64 = ctx.r11.s64 + -27300;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6CC40);
PPC_WEAK_FUNC(sub_82A6CC40) { __imp__sub_82A6CC40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6CC40) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32078
	ctx.r11.s64 = -2102263808;
	// addi r11,r11,-27292
	ctx.r11.s64 = ctx.r11.s64 + -27292;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6CC60);
PPC_WEAK_FUNC(sub_82A6CC60) { __imp__sub_82A6CC60(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6CC60) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32078
	ctx.r11.s64 = -2102263808;
	// addi r11,r11,-27284
	ctx.r11.s64 = ctx.r11.s64 + -27284;
	// lwz r9,-18392(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18392);
	// stw r11,-18392(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18392, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6CC80);
PPC_WEAK_FUNC(sub_82A6CC80) { __imp__sub_82A6CC80(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6CC80) {
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
	// lis r11,-32129
	ctx.r11.s64 = -2105606144;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-7488
	ctx.r5.s64 = ctx.r11.s64 + -7488;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A6CCA8;
	sub_8284D220(ctx, base);
	// lis r11,-32078
	ctx.r11.s64 = -2102263808;
	// lis r9,-32123
	ctx.r9.s64 = -2105212928;
	// addi r10,r11,-25452
	ctx.r10.s64 = ctx.r11.s64 + -25452;
	// addi r9,r9,6152
	ctx.r9.s64 = ctx.r9.s64 + 6152;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
	// stw r9,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r9.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r8,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
	// stw r7,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6CCF8);
PPC_WEAK_FUNC(sub_82A6CCF8) { __imp__sub_82A6CCF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6CCF8) {
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
	// lis r11,-32112
	ctx.r11.s64 = -2104492032;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-9776
	ctx.r5.s64 = ctx.r11.s64 + -9776;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A6CD20;
	sub_8284D220(ctx, base);
	// lis r11,-32078
	ctx.r11.s64 = -2102263808;
	// lis r9,-32123
	ctx.r9.s64 = -2105212928;
	// addi r10,r11,-25404
	ctx.r10.s64 = ctx.r11.s64 + -25404;
	// addi r9,r9,6152
	ctx.r9.s64 = ctx.r9.s64 + 6152;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
	// stw r9,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r9.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r8,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
	// stw r7,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6CD70);
PPC_WEAK_FUNC(sub_82A6CD70) { __imp__sub_82A6CD70(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6CD70) {
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
	// lis r11,-32112
	ctx.r11.s64 = -2104492032;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-5992
	ctx.r5.s64 = ctx.r11.s64 + -5992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A6CD98;
	sub_8284D220(ctx, base);
	// lis r11,-32078
	ctx.r11.s64 = -2102263808;
	// lis r9,-32123
	ctx.r9.s64 = -2105212928;
	// addi r10,r11,-25136
	ctx.r10.s64 = ctx.r11.s64 + -25136;
	// addi r9,r9,6152
	ctx.r9.s64 = ctx.r9.s64 + 6152;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
	// stw r9,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r9.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r8,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
	// stw r7,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6CDE8);
PPC_WEAK_FUNC(sub_82A6CDE8) { __imp__sub_82A6CDE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6CDE8) {
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
	// lis r11,-32112
	ctx.r11.s64 = -2104492032;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-5992
	ctx.r5.s64 = ctx.r11.s64 + -5992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A6CE10;
	sub_8284D220(ctx, base);
	// lis r11,-32078
	ctx.r11.s64 = -2102263808;
	// lis r9,-32123
	ctx.r9.s64 = -2105212928;
	// addi r10,r11,-24816
	ctx.r10.s64 = ctx.r11.s64 + -24816;
	// addi r9,r9,6152
	ctx.r9.s64 = ctx.r9.s64 + 6152;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
	// stw r9,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r9.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r8,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
	// stw r7,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6CE60);
PPC_WEAK_FUNC(sub_82A6CE60) { __imp__sub_82A6CE60(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6CE60) {
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
	// lis r11,-32125
	ctx.r11.s64 = -2105344000;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,12592
	ctx.r5.s64 = ctx.r11.s64 + 12592;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A6CE88;
	sub_8284D220(ctx, base);
	// lis r11,-32078
	ctx.r11.s64 = -2102263808;
	// lis r9,-32123
	ctx.r9.s64 = -2105212928;
	// addi r10,r11,-24488
	ctx.r10.s64 = ctx.r11.s64 + -24488;
	// addi r9,r9,6152
	ctx.r9.s64 = ctx.r9.s64 + 6152;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
	// stw r9,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r9.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r8,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
	// stw r7,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6CED8);
PPC_WEAK_FUNC(sub_82A6CED8) { __imp__sub_82A6CED8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6CED8) {
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
	// lis r11,-32129
	ctx.r11.s64 = -2105606144;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-7488
	ctx.r5.s64 = ctx.r11.s64 + -7488;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A6CF00;
	sub_8284D220(ctx, base);
	// lis r11,-32078
	ctx.r11.s64 = -2102263808;
	// lis r9,-32123
	ctx.r9.s64 = -2105212928;
	// addi r10,r11,-24440
	ctx.r10.s64 = ctx.r11.s64 + -24440;
	// addi r9,r9,6152
	ctx.r9.s64 = ctx.r9.s64 + 6152;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
	// stw r9,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r9.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r8,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
	// stw r7,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6CF50);
PPC_WEAK_FUNC(sub_82A6CF50) { __imp__sub_82A6CF50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6CF50) {
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
	// lis r11,-32112
	ctx.r11.s64 = -2104492032;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-2424
	ctx.r5.s64 = ctx.r11.s64 + -2424;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A6CF78;
	sub_8284D220(ctx, base);
	// lis r11,-32078
	ctx.r11.s64 = -2102263808;
	// lis r9,-32123
	ctx.r9.s64 = -2105212928;
	// addi r10,r11,-24296
	ctx.r10.s64 = ctx.r11.s64 + -24296;
	// addi r9,r9,6152
	ctx.r9.s64 = ctx.r9.s64 + 6152;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
	// stw r9,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r9.u32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r8,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r8.u32);
	// stw r7,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r7.u32);
	// stw r11,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6CFC8);
PPC_WEAK_FUNC(sub_82A6CFC8) { __imp__sub_82A6CFC8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6CFC8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,3400(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3400);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32078
	ctx.r11.s64 = -2102263808;
	// lfs f13,-24120(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -24120);
	ctx.f13.f64 = double(temp.f32);
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// fdivs f0,f0,f13
	ctx.f0.f64 = double(float(ctx.f0.f64 / ctx.f13.f64));
	// stfs f0,26420(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 26420, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6CFE8);
PPC_WEAK_FUNC(sub_82A6CFE8) { __imp__sub_82A6CFE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6CFE8) {
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
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// addi r3,r11,26384
	ctx.r3.s64 = ctx.r11.s64 + 26384;
	// bl 0x822bca90
	ctx.lr = 0x82A6D000;
	sub_822BCA90(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13912
	ctx.r3.s64 = ctx.r11.s64 + 13912;
	// bl 0x829ffa48
	ctx.lr = 0x82A6D00C;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D020);
PPC_WEAK_FUNC(sub_82A6D020) { __imp__sub_82A6D020(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D020) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-19916
	ctx.r5.s64 = ctx.r11.s64 + -19916;
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,26424
	ctx.r3.s64 = ctx.r11.s64 + 26424;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D040);
PPC_WEAK_FUNC(sub_82A6D040) { __imp__sub_82A6D040(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D040) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13928
	ctx.r3.s64 = ctx.r11.s64 + 13928;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D050);
PPC_WEAK_FUNC(sub_82A6D050) { __imp__sub_82A6D050(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D050) {
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
	// lis r11,-31972
	ctx.r11.s64 = -2095316992;
	// addi r31,r11,26448
	ctx.r31.s64 = ctx.r11.s64 + 26448;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8290bcf0
	ctx.lr = 0x82A6D070;
	sub_8290BCF0(ctx, base);
	// addi r3,r31,100
	ctx.r3.s64 = ctx.r31.s64 + 100;
	// bl 0x82912b00
	ctx.lr = 0x82A6D078;
	sub_82912B00(ctx, base);
	// addi r3,r31,172
	ctx.r3.s64 = ctx.r31.s64 + 172;
	// bl 0x82915750
	ctx.lr = 0x82A6D080;
	sub_82915750(ctx, base);
	// addi r3,r31,380
	ctx.r3.s64 = ctx.r31.s64 + 380;
	// bl 0x8291f8b8
	ctx.lr = 0x82A6D088;
	sub_8291F8B8(ctx, base);
	// addi r3,r31,700
	ctx.r3.s64 = ctx.r31.s64 + 700;
	// bl 0x8290f818
	ctx.lr = 0x82A6D090;
	sub_8290F818(ctx, base);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lis r6,-32246
	ctx.r6.s64 = -2113273856;
	// lis r5,-32244
	ctx.r5.s64 = -2113142784;
	// lis r9,-32246
	ctx.r9.s64 = -2113273856;
	// lfs f0,2612(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32244
	ctx.r11.s64 = -2113142784;
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// lis r8,-32246
	ctx.r8.s64 = -2113273856;
	// stfs f0,84(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// lis r7,-32246
	ctx.r7.s64 = -2113273856;
	// stfs f0,96(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 96, temp.u32);
	// addi r9,r9,-19440
	ctx.r9.s64 = ctx.r9.s64 + -19440;
	// stfs f0,104(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 104, temp.u32);
	// addi r8,r8,-19536
	ctx.r8.s64 = ctx.r8.s64 + -19536;
	// lfs f13,-4876(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -4876);
	ctx.f13.f64 = double(temp.f32);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// stfs f13,88(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// addi r7,r7,-19496
	ctx.r7.s64 = ctx.r7.s64 + -19496;
	// lfs f13,3400(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 3400);
	ctx.f13.f64 = double(temp.f32);
	// lis r10,-32246
	ctx.r10.s64 = -2113273856;
	// stfs f13,1232(r31)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r31.u32 + 1232, temp.u32);
	// addi r11,r11,-19460
	ctx.r11.s64 = ctx.r11.s64 + -19460;
	// lfs f0,-19416(r6)
	temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + -19416);
	ctx.f0.f64 = double(temp.f32);
	// addi r10,r10,-19480
	ctx.r10.s64 = ctx.r10.s64 + -19480;
	// stfs f0,1236(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r31.u32 + 1236, temp.u32);
	// lis r6,-32246
	ctx.r6.s64 = -2113273856;
	// lfs f0,-4856(r5)
	temp.u32 = PPC_LOAD_U32(ctx.r5.u32 + -4856);
	ctx.f0.f64 = double(temp.f32);
	// li r5,0
	ctx.r5.s64 = 0;
	// stfs f0,1240(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r31.u32 + 1240, temp.u32);
	// addi r6,r6,-19516
	ctx.r6.s64 = ctx.r6.s64 + -19516;
	// stfs f13,100(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 100, temp.u32);
	// addi r3,r31,1328
	ctx.r3.s64 = ctx.r31.s64 + 1328;
	// stw r5,1284(r31)
	PPC_STORE_U32(ctx.r31.u32 + 1284, ctx.r5.u32);
	// stw r5,1288(r31)
	PPC_STORE_U32(ctx.r31.u32 + 1288, ctx.r5.u32);
	// stw r11,1244(r31)
	PPC_STORE_U32(ctx.r31.u32 + 1244, ctx.r11.u32);
	// li r11,255
	ctx.r11.s64 = 255;
	// stw r10,1248(r31)
	PPC_STORE_U32(ctx.r31.u32 + 1248, ctx.r10.u32);
	// stw r9,1252(r31)
	PPC_STORE_U32(ctx.r31.u32 + 1252, ctx.r9.u32);
	// stw r8,1256(r31)
	PPC_STORE_U32(ctx.r31.u32 + 1256, ctx.r8.u32);
	// stw r7,1260(r31)
	PPC_STORE_U32(ctx.r31.u32 + 1260, ctx.r7.u32);
	// stw r6,1264(r31)
	PPC_STORE_U32(ctx.r31.u32 + 1264, ctx.r6.u32);
	// stw r11,1268(r31)
	PPC_STORE_U32(ctx.r31.u32 + 1268, ctx.r11.u32);
	// li r11,96
	ctx.r11.s64 = 96;
	// stw r11,1276(r31)
	PPC_STORE_U32(ctx.r31.u32 + 1276, ctx.r11.u32);
	// li r11,24
	ctx.r11.s64 = 24;
	// stw r11,1272(r31)
	PPC_STORE_U32(ctx.r31.u32 + 1272, ctx.r11.u32);
	// li r11,4
	ctx.r11.s64 = 4;
	// stw r11,1280(r31)
	PPC_STORE_U32(ctx.r31.u32 + 1280, ctx.r11.u32);
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r11,832
	ctx.r11.s64 = 832;
	// stvx128 v0,r31,r11
	ea = (ctx.r31.u32 + ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,96
	ctx.r11.s64 = ctx.r1.s64 + 96;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r11,816
	ctx.r11.s64 = 816;
	// stvx128 v0,r31,r11
	ea = (ctx.r31.u32 + ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x82904ea8
	ctx.lr = 0x82A6D178;
	sub_82904EA8(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,13944
	ctx.r3.s64 = ctx.r11.s64 + 13944;
	// bl 0x829ffa48
	ctx.lr = 0x82A6D184;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6D198);
PPC_WEAK_FUNC(sub_82A6D198) { __imp__sub_82A6D198(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D198) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-19364
	ctx.r5.s64 = ctx.r11.s64 + -19364;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-31340
	ctx.r3.s64 = ctx.r11.s64 + -31340;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D1B8);
PPC_WEAK_FUNC(sub_82A6D1B8) { __imp__sub_82A6D1B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D1B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-19356
	ctx.r5.s64 = ctx.r11.s64 + -19356;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-31360
	ctx.r3.s64 = ctx.r11.s64 + -31360;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D1D8);
PPC_WEAK_FUNC(sub_82A6D1D8) { __imp__sub_82A6D1D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D1D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-19348
	ctx.r5.s64 = ctx.r11.s64 + -19348;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-31320
	ctx.r3.s64 = ctx.r11.s64 + -31320;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D1F8);
PPC_WEAK_FUNC(sub_82A6D1F8) { __imp__sub_82A6D1F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D1F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-19332
	ctx.r5.s64 = ctx.r11.s64 + -19332;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-31300
	ctx.r3.s64 = ctx.r11.s64 + -31300;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D218);
PPC_WEAK_FUNC(sub_82A6D218) { __imp__sub_82A6D218(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D218) {
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
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,2580
	ctx.r3.s64 = ctx.r11.s64 + 2580;
	// bl 0x8284e060
	ctx.lr = 0x82A6D234;
	sub_8284E060(ctx, base);
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// stw r3,-31280(r11)
	PPC_STORE_U32(ctx.r11.u32 + -31280, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D250);
PPC_WEAK_FUNC(sub_82A6D250) { __imp__sub_82A6D250(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D250) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-17132
	ctx.r5.s64 = ctx.r11.s64 + -17132;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-31076
	ctx.r3.s64 = ctx.r11.s64 + -31076;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D270);
PPC_WEAK_FUNC(sub_82A6D270) { __imp__sub_82A6D270(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D270) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-17116
	ctx.r5.s64 = ctx.r11.s64 + -17116;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-31172
	ctx.r3.s64 = ctx.r11.s64 + -31172;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D290);
PPC_WEAK_FUNC(sub_82A6D290) { __imp__sub_82A6D290(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D290) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,14096
	ctx.r3.s64 = ctx.r11.s64 + 14096;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D2A0);
PPC_WEAK_FUNC(sub_82A6D2A0) { __imp__sub_82A6D2A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D2A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,14080
	ctx.r3.s64 = ctx.r11.s64 + 14080;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D2B0);
PPC_WEAK_FUNC(sub_82A6D2B0) { __imp__sub_82A6D2B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D2B0) {
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
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r3,r11,-31108
	ctx.r3.s64 = ctx.r11.s64 + -31108;
	// bl 0x8285fe48
	ctx.lr = 0x82A6D2C8;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,14048
	ctx.r3.s64 = ctx.r11.s64 + 14048;
	// bl 0x829ffa48
	ctx.lr = 0x82A6D2D4;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D2E8);
PPC_WEAK_FUNC(sub_82A6D2E8) { __imp__sub_82A6D2E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D2E8) {
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
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r3,r11,-31140
	ctx.r3.s64 = ctx.r11.s64 + -31140;
	// bl 0x8285fe48
	ctx.lr = 0x82A6D300;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,14064
	ctx.r3.s64 = ctx.r11.s64 + 14064;
	// bl 0x829ffa48
	ctx.lr = 0x82A6D30C;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D320);
PPC_WEAK_FUNC(sub_82A6D320) { __imp__sub_82A6D320(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D320) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-16744
	ctx.r5.s64 = ctx.r11.s64 + -16744;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-30172
	ctx.r3.s64 = ctx.r11.s64 + -30172;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D340);
PPC_WEAK_FUNC(sub_82A6D340) { __imp__sub_82A6D340(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D340) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,14112
	ctx.r3.s64 = ctx.r11.s64 + 14112;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D350);
PPC_WEAK_FUNC(sub_82A6D350) { __imp__sub_82A6D350(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D350) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-15508
	ctx.r5.s64 = ctx.r11.s64 + -15508;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-29852
	ctx.r3.s64 = ctx.r11.s64 + -29852;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D370);
PPC_WEAK_FUNC(sub_82A6D370) { __imp__sub_82A6D370(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D370) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-15492
	ctx.r5.s64 = ctx.r11.s64 + -15492;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-29832
	ctx.r3.s64 = ctx.r11.s64 + -29832;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D390);
PPC_WEAK_FUNC(sub_82A6D390) { __imp__sub_82A6D390(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D390) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-15076
	ctx.r5.s64 = ctx.r11.s64 + -15076;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-29800
	ctx.r3.s64 = ctx.r11.s64 + -29800;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D3B0);
PPC_WEAK_FUNC(sub_82A6D3B0) { __imp__sub_82A6D3B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D3B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-11356
	ctx.r5.s64 = ctx.r11.s64 + -11356;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-29148
	ctx.r3.s64 = ctx.r11.s64 + -29148;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D3D0);
PPC_WEAK_FUNC(sub_82A6D3D0) { __imp__sub_82A6D3D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D3D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-11344
	ctx.r5.s64 = ctx.r11.s64 + -11344;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-29168
	ctx.r3.s64 = ctx.r11.s64 + -29168;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D3F0);
PPC_WEAK_FUNC(sub_82A6D3F0) { __imp__sub_82A6D3F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D3F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,14128
	ctx.r3.s64 = ctx.r11.s64 + 14128;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D400);
PPC_WEAK_FUNC(sub_82A6D400) { __imp__sub_82A6D400(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D400) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,14160
	ctx.r3.s64 = ctx.r11.s64 + 14160;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D410);
PPC_WEAK_FUNC(sub_82A6D410) { __imp__sub_82A6D410(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D410) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-9380
	ctx.r5.s64 = ctx.r11.s64 + -9380;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-29076
	ctx.r3.s64 = ctx.r11.s64 + -29076;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D430);
PPC_WEAK_FUNC(sub_82A6D430) { __imp__sub_82A6D430(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D430) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-7444
	ctx.r5.s64 = ctx.r11.s64 + -7444;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-29032
	ctx.r3.s64 = ctx.r11.s64 + -29032;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D450);
PPC_WEAK_FUNC(sub_82A6D450) { __imp__sub_82A6D450(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D450) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-7432
	ctx.r5.s64 = ctx.r11.s64 + -7432;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-29012
	ctx.r3.s64 = ctx.r11.s64 + -29012;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D470);
PPC_WEAK_FUNC(sub_82A6D470) { __imp__sub_82A6D470(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D470) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-6992
	ctx.r5.s64 = ctx.r11.s64 + -6992;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-28772
	ctx.r3.s64 = ctx.r11.s64 + -28772;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D490);
PPC_WEAK_FUNC(sub_82A6D490) { __imp__sub_82A6D490(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D490) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r4,r11,5148
	ctx.r4.s64 = ctx.r11.s64 + 5148;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r3,r11,-14200
	ctx.r3.s64 = ctx.r11.s64 + -14200;
	// b 0x8286a370
	sub_8286A370(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D4A8);
PPC_WEAK_FUNC(sub_82A6D4A8) { __imp__sub_82A6D4A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D4A8) {
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
	// lis r11,-32244
	ctx.r11.s64 = -2113142784;
	// addi r4,r11,-15160
	ctx.r4.s64 = ctx.r11.s64 + -15160;
	// lis r11,-32078
	ctx.r11.s64 = -2102263808;
	// addi r5,r4,1
	ctx.r5.s64 = ctx.r4.s64 + 1;
	// addi r3,r11,-21072
	ctx.r3.s64 = ctx.r11.s64 + -21072;
	// bl 0x827a4f78
	ctx.lr = 0x82A6D4CC;
	sub_827A4F78(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,14192
	ctx.r3.s64 = ctx.r11.s64 + 14192;
	// bl 0x829ffa48
	ctx.lr = 0x82A6D4D8;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D4E8);
PPC_WEAK_FUNC(sub_82A6D4E8) { __imp__sub_82A6D4E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D4E8) {
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
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r4,r11,22216
	ctx.r4.s64 = ctx.r11.s64 + 22216;
	// lis r11,-32078
	ctx.r11.s64 = -2102263808;
	// addi r5,r4,4
	ctx.r5.s64 = ctx.r4.s64 + 4;
	// addi r3,r11,-21028
	ctx.r3.s64 = ctx.r11.s64 + -21028;
	// bl 0x827a4f78
	ctx.lr = 0x82A6D50C;
	sub_827A4F78(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,14240
	ctx.r3.s64 = ctx.r11.s64 + 14240;
	// bl 0x829ffa48
	ctx.lr = 0x82A6D518;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D528);
PPC_WEAK_FUNC(sub_82A6D528) { __imp__sub_82A6D528(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D528) {
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
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r4,r11,22208
	ctx.r4.s64 = ctx.r11.s64 + 22208;
	// lis r11,-32078
	ctx.r11.s64 = -2102263808;
	// addi r5,r4,5
	ctx.r5.s64 = ctx.r4.s64 + 5;
	// addi r3,r11,-21004
	ctx.r3.s64 = ctx.r11.s64 + -21004;
	// bl 0x827a4f78
	ctx.lr = 0x82A6D54C;
	sub_827A4F78(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,14288
	ctx.r3.s64 = ctx.r11.s64 + 14288;
	// bl 0x829ffa48
	ctx.lr = 0x82A6D558;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D568);
PPC_WEAK_FUNC(sub_82A6D568) { __imp__sub_82A6D568(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D568) {
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
	// lis r11,-32244
	ctx.r11.s64 = -2113142784;
	// addi r5,r11,-27908
	ctx.r5.s64 = ctx.r11.s64 + -27908;
	// lis r11,-32078
	ctx.r11.s64 = -2102263808;
	// mr r4,r5
	ctx.r4.u64 = ctx.r5.u64;
	// addi r3,r11,-20980
	ctx.r3.s64 = ctx.r11.s64 + -20980;
	// bl 0x827a4f78
	ctx.lr = 0x82A6D58C;
	sub_827A4F78(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,14336
	ctx.r3.s64 = ctx.r11.s64 + 14336;
	// bl 0x829ffa48
	ctx.lr = 0x82A6D598;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D5A8);
PPC_WEAK_FUNC(sub_82A6D5A8) { __imp__sub_82A6D5A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D5A8) {
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
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r31,r11,6080
	ctx.r31.s64 = ctx.r11.s64 + 6080;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a040f0
	ctx.lr = 0x82A6D5C8;
	sub_82A040F0(ctx, base);
	// rlwinm r11,r3,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 1) & 0xFFFFFFFE;
	// lis r10,-32078
	ctx.r10.s64 = -2102263808;
	// add r5,r11,r31
	ctx.r5.u64 = ctx.r11.u64 + ctx.r31.u64;
	// addi r3,r10,-20956
	ctx.r3.s64 = ctx.r10.s64 + -20956;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x82939668
	ctx.lr = 0x82A6D5E0;
	sub_82939668(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,14384
	ctx.r3.s64 = ctx.r11.s64 + 14384;
	// bl 0x829ffa48
	ctx.lr = 0x82A6D5EC;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6D600);
PPC_WEAK_FUNC(sub_82A6D600) { __imp__sub_82A6D600(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D600) {
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
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r31,r11,6092
	ctx.r31.s64 = ctx.r11.s64 + 6092;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a040f0
	ctx.lr = 0x82A6D620;
	sub_82A040F0(ctx, base);
	// rlwinm r11,r3,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 1) & 0xFFFFFFFE;
	// lis r10,-32078
	ctx.r10.s64 = -2102263808;
	// add r5,r11,r31
	ctx.r5.u64 = ctx.r11.u64 + ctx.r31.u64;
	// addi r3,r10,-20916
	ctx.r3.s64 = ctx.r10.s64 + -20916;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x82939668
	ctx.lr = 0x82A6D638;
	sub_82939668(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,14432
	ctx.r3.s64 = ctx.r11.s64 + 14432;
	// bl 0x829ffa48
	ctx.lr = 0x82A6D644;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6D658);
PPC_WEAK_FUNC(sub_82A6D658) { __imp__sub_82A6D658(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D658) {
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
	// lis r11,-32244
	ctx.r11.s64 = -2113142784;
	// addi r5,r11,-27908
	ctx.r5.s64 = ctx.r11.s64 + -27908;
	// lis r11,-32078
	ctx.r11.s64 = -2102263808;
	// mr r4,r5
	ctx.r4.u64 = ctx.r5.u64;
	// addi r3,r11,-20876
	ctx.r3.s64 = ctx.r11.s64 + -20876;
	// bl 0x827a4f78
	ctx.lr = 0x82A6D67C;
	sub_827A4F78(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,14480
	ctx.r3.s64 = ctx.r11.s64 + 14480;
	// bl 0x829ffa48
	ctx.lr = 0x82A6D688;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D698);
PPC_WEAK_FUNC(sub_82A6D698) { __imp__sub_82A6D698(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D698) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,14528
	ctx.r3.s64 = ctx.r11.s64 + 14528;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D6A8);
PPC_WEAK_FUNC(sub_82A6D6A8) { __imp__sub_82A6D6A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D6A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,14576
	ctx.r3.s64 = ctx.r11.s64 + 14576;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D6B8);
PPC_WEAK_FUNC(sub_82A6D6B8) { __imp__sub_82A6D6B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D6B8) {
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
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r3,r11,-14052
	ctx.r3.s64 = ctx.r11.s64 + -14052;
	// bl 0x8285fe48
	ctx.lr = 0x82A6D6D0;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,14624
	ctx.r3.s64 = ctx.r11.s64 + 14624;
	// bl 0x829ffa48
	ctx.lr = 0x82A6D6DC;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D6F0);
PPC_WEAK_FUNC(sub_82A6D6F0) { __imp__sub_82A6D6F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D6F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,11548
	ctx.r5.s64 = ctx.r11.s64 + 11548;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-9712
	ctx.r3.s64 = ctx.r11.s64 + -9712;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D710);
PPC_WEAK_FUNC(sub_82A6D710) { __imp__sub_82A6D710(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D710) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,11560
	ctx.r5.s64 = ctx.r11.s64 + 11560;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-9732
	ctx.r3.s64 = ctx.r11.s64 + -9732;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D730);
PPC_WEAK_FUNC(sub_82A6D730) { __imp__sub_82A6D730(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D730) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,17004
	ctx.r5.s64 = ctx.r11.s64 + 17004;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-9572
	ctx.r3.s64 = ctx.r11.s64 + -9572;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D750);
PPC_WEAK_FUNC(sub_82A6D750) { __imp__sub_82A6D750(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D750) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,17028
	ctx.r5.s64 = ctx.r11.s64 + 17028;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-9592
	ctx.r3.s64 = ctx.r11.s64 + -9592;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D770);
PPC_WEAK_FUNC(sub_82A6D770) { __imp__sub_82A6D770(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D770) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,17048
	ctx.r5.s64 = ctx.r11.s64 + 17048;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-9512
	ctx.r3.s64 = ctx.r11.s64 + -9512;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D790);
PPC_WEAK_FUNC(sub_82A6D790) { __imp__sub_82A6D790(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D790) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,17068
	ctx.r5.s64 = ctx.r11.s64 + 17068;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-9532
	ctx.r3.s64 = ctx.r11.s64 + -9532;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D7B0);
PPC_WEAK_FUNC(sub_82A6D7B0) { __imp__sub_82A6D7B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D7B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,17080
	ctx.r5.s64 = ctx.r11.s64 + 17080;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-9552
	ctx.r3.s64 = ctx.r11.s64 + -9552;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D7D0);
PPC_WEAK_FUNC(sub_82A6D7D0) { __imp__sub_82A6D7D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D7D0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32078
	ctx.r11.s64 = -2102263808;
	// lfs f0,-17288(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -17288);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r11,r11,-8960
	ctx.r11.s64 = ctx.r11.s64 + -8960;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D7F0);
PPC_WEAK_FUNC(sub_82A6D7F0) { __imp__sub_82A6D7F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D7F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,14640
	ctx.r3.s64 = ctx.r11.s64 + 14640;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D800);
PPC_WEAK_FUNC(sub_82A6D800) { __imp__sub_82A6D800(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D800) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,14664
	ctx.r3.s64 = ctx.r11.s64 + 14664;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D810);
PPC_WEAK_FUNC(sub_82A6D810) { __imp__sub_82A6D810(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D810) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,14688
	ctx.r3.s64 = ctx.r11.s64 + 14688;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D820);
PPC_WEAK_FUNC(sub_82A6D820) { __imp__sub_82A6D820(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D820) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,14712
	ctx.r3.s64 = ctx.r11.s64 + 14712;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D830);
PPC_WEAK_FUNC(sub_82A6D830) { __imp__sub_82A6D830(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D830) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,14736
	ctx.r3.s64 = ctx.r11.s64 + 14736;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D840);
PPC_WEAK_FUNC(sub_82A6D840) { __imp__sub_82A6D840(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D840) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,14824
	ctx.r3.s64 = ctx.r11.s64 + 14824;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D850);
PPC_WEAK_FUNC(sub_82A6D850) { __imp__sub_82A6D850(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D850) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,14912
	ctx.r3.s64 = ctx.r11.s64 + 14912;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D860);
PPC_WEAK_FUNC(sub_82A6D860) { __imp__sub_82A6D860(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D860) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,14936
	ctx.r3.s64 = ctx.r11.s64 + 14936;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D870);
PPC_WEAK_FUNC(sub_82A6D870) { __imp__sub_82A6D870(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D870) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,14960
	ctx.r3.s64 = ctx.r11.s64 + 14960;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D880);
PPC_WEAK_FUNC(sub_82A6D880) { __imp__sub_82A6D880(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D880) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r3,r11,-7304
	ctx.r3.s64 = ctx.r11.s64 + -7304;
	// b 0x829e4a00
	sub_829E4A00(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6D890);
PPC_WEAK_FUNC(sub_82A6D890) { __imp__sub_82A6D890(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D890) {
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
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,31832
	ctx.r5.s64 = ctx.r11.s64 + 31832;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r31,r11,-7296
	ctx.r31.s64 = ctx.r11.s64 + -7296;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6D8C0;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6D8D0;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6D8D8;
	sub_829DC040(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,31828
	ctx.r11.s64 = ctx.r11.s64 + 31828;
	// addi r3,r10,14984
	ctx.r3.s64 = ctx.r10.s64 + 14984;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6D8F0;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6D908);
PPC_WEAK_FUNC(sub_82A6D908) { __imp__sub_82A6D908(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D908) {
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
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,31996
	ctx.r5.s64 = ctx.r11.s64 + 31996;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r31,r11,-7044
	ctx.r31.s64 = ctx.r11.s64 + -7044;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6D938;
	sub_829DBFD0(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r11,r11,31984
	ctx.r11.s64 = ctx.r11.s64 + 31984;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829e3230
	ctx.lr = 0x82A6D948;
	sub_829E3230(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6D950;
	sub_829DC040(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,31992
	ctx.r11.s64 = ctx.r11.s64 + 31992;
	// addi r3,r10,15080
	ctx.r3.s64 = ctx.r10.s64 + 15080;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6D968;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6D980);
PPC_WEAK_FUNC(sub_82A6D980) { __imp__sub_82A6D980(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6D980) {
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
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r7,r11,-7044
	ctx.r7.s64 = ctx.r11.s64 + -7044;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,32020
	ctx.r6.s64 = ctx.r11.s64 + 32020;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-7204
	ctx.r31.s64 = ctx.r11.s64 + -7204;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6D9B8;
	sub_829DC008(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r11,r11,31984
	ctx.r11.s64 = ctx.r11.s64 + 31984;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829e3230
	ctx.lr = 0x82A6D9C8;
	sub_829E3230(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6D9D0;
	sub_829DC040(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,32016
	ctx.r11.s64 = ctx.r11.s64 + 32016;
	// addi r3,r10,15160
	ctx.r3.s64 = ctx.r10.s64 + 15160;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6D9E8;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6DA00);
PPC_WEAK_FUNC(sub_82A6DA00) { __imp__sub_82A6DA00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6DA00) {
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
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r7,r11,-7044
	ctx.r7.s64 = ctx.r11.s64 + -7044;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,32064
	ctx.r6.s64 = ctx.r11.s64 + 32064;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-7172
	ctx.r31.s64 = ctx.r11.s64 + -7172;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6DA38;
	sub_829DC008(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r11,r11,31984
	ctx.r11.s64 = ctx.r11.s64 + 31984;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829e3230
	ctx.lr = 0x82A6DA48;
	sub_829E3230(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6DA50;
	sub_829DC040(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,32060
	ctx.r11.s64 = ctx.r11.s64 + 32060;
	// addi r3,r10,15240
	ctx.r3.s64 = ctx.r10.s64 + 15240;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6DA68;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6DA80);
PPC_WEAK_FUNC(sub_82A6DA80) { __imp__sub_82A6DA80(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6DA80) {
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
	// li r5,2
	ctx.r5.s64 = 2;
	// addi r7,r11,-7044
	ctx.r7.s64 = ctx.r11.s64 + -7044;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,32104
	ctx.r6.s64 = ctx.r11.s64 + 32104;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-7140
	ctx.r31.s64 = ctx.r11.s64 + -7140;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6DAB8;
	sub_829DC008(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r11,r11,31984
	ctx.r11.s64 = ctx.r11.s64 + 31984;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829e3230
	ctx.lr = 0x82A6DAC8;
	sub_829E3230(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6DAD0;
	sub_829DC040(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,32100
	ctx.r11.s64 = ctx.r11.s64 + 32100;
	// addi r3,r10,15320
	ctx.r3.s64 = ctx.r10.s64 + 15320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6DAE8;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6DB00);
PPC_WEAK_FUNC(sub_82A6DB00) { __imp__sub_82A6DB00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6DB00) {
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
	// li r5,3
	ctx.r5.s64 = 3;
	// addi r7,r11,-7044
	ctx.r7.s64 = ctx.r11.s64 + -7044;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,32148
	ctx.r6.s64 = ctx.r11.s64 + 32148;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-7076
	ctx.r31.s64 = ctx.r11.s64 + -7076;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6DB38;
	sub_829DC008(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r11,r11,31984
	ctx.r11.s64 = ctx.r11.s64 + 31984;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829e3230
	ctx.lr = 0x82A6DB48;
	sub_829E3230(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6DB50;
	sub_829DC040(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,32144
	ctx.r11.s64 = ctx.r11.s64 + 32144;
	// addi r3,r10,15400
	ctx.r3.s64 = ctx.r10.s64 + 15400;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6DB68;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6DB80);
PPC_WEAK_FUNC(sub_82A6DB80) { __imp__sub_82A6DB80(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6DB80) {
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
	// li r5,4
	ctx.r5.s64 = 4;
	// addi r7,r11,-7044
	ctx.r7.s64 = ctx.r11.s64 + -7044;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,32188
	ctx.r6.s64 = ctx.r11.s64 + 32188;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-7236
	ctx.r31.s64 = ctx.r11.s64 + -7236;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6DBB8;
	sub_829DC008(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r11,r11,31984
	ctx.r11.s64 = ctx.r11.s64 + 31984;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829e3230
	ctx.lr = 0x82A6DBC8;
	sub_829E3230(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6DBD0;
	sub_829DC040(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,32184
	ctx.r11.s64 = ctx.r11.s64 + 32184;
	// addi r3,r10,15480
	ctx.r3.s64 = ctx.r10.s64 + 15480;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6DBE8;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6DC00);
PPC_WEAK_FUNC(sub_82A6DC00) { __imp__sub_82A6DC00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6DC00) {
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
	// li r5,5
	ctx.r5.s64 = 5;
	// addi r7,r11,-7044
	ctx.r7.s64 = ctx.r11.s64 + -7044;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,32232
	ctx.r6.s64 = ctx.r11.s64 + 32232;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-7108
	ctx.r31.s64 = ctx.r11.s64 + -7108;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6DC38;
	sub_829DC008(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r11,r11,31984
	ctx.r11.s64 = ctx.r11.s64 + 31984;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829e3230
	ctx.lr = 0x82A6DC48;
	sub_829E3230(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6DC50;
	sub_829DC040(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,32228
	ctx.r11.s64 = ctx.r11.s64 + 32228;
	// addi r3,r10,15560
	ctx.r3.s64 = ctx.r10.s64 + 15560;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6DC68;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6DC80);
PPC_WEAK_FUNC(sub_82A6DC80) { __imp__sub_82A6DC80(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6DC80) {
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
	// li r5,6
	ctx.r5.s64 = 6;
	// addi r7,r11,-7044
	ctx.r7.s64 = ctx.r11.s64 + -7044;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,32272
	ctx.r6.s64 = ctx.r11.s64 + 32272;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-7012
	ctx.r31.s64 = ctx.r11.s64 + -7012;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6DCB8;
	sub_829DC008(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r11,r11,31984
	ctx.r11.s64 = ctx.r11.s64 + 31984;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829e3230
	ctx.lr = 0x82A6DCC8;
	sub_829E3230(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6DCD0;
	sub_829DC040(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,32268
	ctx.r11.s64 = ctx.r11.s64 + 32268;
	// addi r3,r10,15640
	ctx.r3.s64 = ctx.r10.s64 + 15640;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6DCE8;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6DD00);
PPC_WEAK_FUNC(sub_82A6DD00) { __imp__sub_82A6DD00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6DD00) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,32308
	ctx.r5.s64 = ctx.r11.s64 + 32308;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-6956
	ctx.r3.s64 = ctx.r11.s64 + -6956;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6DD20);
PPC_WEAK_FUNC(sub_82A6DD20) { __imp__sub_82A6DD20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6DD20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,32360
	ctx.r5.s64 = ctx.r11.s64 + 32360;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-6936
	ctx.r3.s64 = ctx.r11.s64 + -6936;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6DD40);
PPC_WEAK_FUNC(sub_82A6DD40) { __imp__sub_82A6DD40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6DD40) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// addi r5,r11,-32684
	ctx.r5.s64 = ctx.r11.s64 + -32684;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-6896
	ctx.r31.s64 = ctx.r11.s64 + -6896;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6DD74;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6DD84;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6DD8C;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-32688
	ctx.r11.s64 = ctx.r11.s64 + -32688;
	// addi r3,r10,15720
	ctx.r3.s64 = ctx.r10.s64 + 15720;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6DDA4;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6DDB8);
PPC_WEAK_FUNC(sub_82A6DDB8) { __imp__sub_82A6DDB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6DDB8) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// addi r5,r11,-32496
	ctx.r5.s64 = ctx.r11.s64 + -32496;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-6832
	ctx.r31.s64 = ctx.r11.s64 + -6832;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6DDEC;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6DDFC;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6DE04;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-32500
	ctx.r11.s64 = ctx.r11.s64 + -32500;
	// addi r3,r10,15800
	ctx.r3.s64 = ctx.r10.s64 + 15800;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6DE1C;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6DE30);
PPC_WEAK_FUNC(sub_82A6DE30) { __imp__sub_82A6DE30(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6DE30) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// addi r5,r11,-32468
	ctx.r5.s64 = ctx.r11.s64 + -32468;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-6864
	ctx.r31.s64 = ctx.r11.s64 + -6864;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6DE64;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6DE74;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6DE7C;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-32472
	ctx.r11.s64 = ctx.r11.s64 + -32472;
	// addi r3,r10,15880
	ctx.r3.s64 = ctx.r10.s64 + 15880;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6DE94;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6DEA8);
PPC_WEAK_FUNC(sub_82A6DEA8) { __imp__sub_82A6DEA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6DEA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-32448
	ctx.r5.s64 = ctx.r11.s64 + -32448;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-6800
	ctx.r3.s64 = ctx.r11.s64 + -6800;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6DEC8);
PPC_WEAK_FUNC(sub_82A6DEC8) { __imp__sub_82A6DEC8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6DEC8) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// addi r5,r11,-32152
	ctx.r5.s64 = ctx.r11.s64 + -32152;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-6780
	ctx.r31.s64 = ctx.r11.s64 + -6780;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6DEFC;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6DF0C;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6DF14;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-32156
	ctx.r11.s64 = ctx.r11.s64 + -32156;
	// addi r3,r10,15960
	ctx.r3.s64 = ctx.r10.s64 + 15960;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6DF2C;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6DF40);
PPC_WEAK_FUNC(sub_82A6DF40) { __imp__sub_82A6DF40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6DF40) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// addi r5,r11,-31168
	ctx.r5.s64 = ctx.r11.s64 + -31168;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-6744
	ctx.r31.s64 = ctx.r11.s64 + -6744;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6DF74;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6DF84;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6DF8C;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-31172
	ctx.r11.s64 = ctx.r11.s64 + -31172;
	// addi r3,r10,16040
	ctx.r3.s64 = ctx.r10.s64 + 16040;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6DFA4;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6DFB8);
PPC_WEAK_FUNC(sub_82A6DFB8) { __imp__sub_82A6DFB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6DFB8) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// addi r5,r11,-31136
	ctx.r5.s64 = ctx.r11.s64 + -31136;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-6712
	ctx.r31.s64 = ctx.r11.s64 + -6712;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6DFEC;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6DFFC;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6E004;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-31140
	ctx.r11.s64 = ctx.r11.s64 + -31140;
	// addi r3,r10,16120
	ctx.r3.s64 = ctx.r10.s64 + 16120;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6E01C;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6E030);
PPC_WEAK_FUNC(sub_82A6E030) { __imp__sub_82A6E030(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6E030) {
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
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-30840
	ctx.r5.s64 = ctx.r11.s64 + -30840;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r31,r11,-6620
	ctx.r31.s64 = ctx.r11.s64 + -6620;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6E060;
	sub_829DBFD0(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// addi r11,r11,-30852
	ctx.r11.s64 = ctx.r11.s64 + -30852;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829fb050
	ctx.lr = 0x82A6E070;
	sub_829FB050(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6E078;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-30844
	ctx.r11.s64 = ctx.r11.s64 + -30844;
	// addi r3,r10,16216
	ctx.r3.s64 = ctx.r10.s64 + 16216;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6E090;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6E0A8);
PPC_WEAK_FUNC(sub_82A6E0A8) { __imp__sub_82A6E0A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6E0A8) {
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
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r7,r11,-6620
	ctx.r7.s64 = ctx.r11.s64 + -6620;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-30780
	ctx.r6.s64 = ctx.r11.s64 + -30780;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-6524
	ctx.r31.s64 = ctx.r11.s64 + -6524;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6E0E0;
	sub_829DC008(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// addi r11,r11,-30852
	ctx.r11.s64 = ctx.r11.s64 + -30852;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829fb050
	ctx.lr = 0x82A6E0F0;
	sub_829FB050(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6E0F8;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-30784
	ctx.r11.s64 = ctx.r11.s64 + -30784;
	// addi r3,r10,16296
	ctx.r3.s64 = ctx.r10.s64 + 16296;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6E110;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6E128);
PPC_WEAK_FUNC(sub_82A6E128) { __imp__sub_82A6E128(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6E128) {
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
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r7,r11,-6620
	ctx.r7.s64 = ctx.r11.s64 + -6620;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-30820
	ctx.r6.s64 = ctx.r11.s64 + -30820;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-6588
	ctx.r31.s64 = ctx.r11.s64 + -6588;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6E160;
	sub_829DC008(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// addi r11,r11,-30852
	ctx.r11.s64 = ctx.r11.s64 + -30852;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829fb050
	ctx.lr = 0x82A6E170;
	sub_829FB050(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6E178;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-30824
	ctx.r11.s64 = ctx.r11.s64 + -30824;
	// addi r3,r10,16376
	ctx.r3.s64 = ctx.r10.s64 + 16376;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6E190;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6E1A8);
PPC_WEAK_FUNC(sub_82A6E1A8) { __imp__sub_82A6E1A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6E1A8) {
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
	// li r5,2
	ctx.r5.s64 = 2;
	// addi r7,r11,-6620
	ctx.r7.s64 = ctx.r11.s64 + -6620;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-30744
	ctx.r6.s64 = ctx.r11.s64 + -30744;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-6428
	ctx.r31.s64 = ctx.r11.s64 + -6428;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6E1E0;
	sub_829DC008(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// addi r11,r11,-30852
	ctx.r11.s64 = ctx.r11.s64 + -30852;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829fb050
	ctx.lr = 0x82A6E1F0;
	sub_829FB050(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6E1F8;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-30748
	ctx.r11.s64 = ctx.r11.s64 + -30748;
	// addi r3,r10,16456
	ctx.r3.s64 = ctx.r10.s64 + 16456;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6E210;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6E228);
PPC_WEAK_FUNC(sub_82A6E228) { __imp__sub_82A6E228(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6E228) {
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
	// li r5,3
	ctx.r5.s64 = 3;
	// addi r7,r11,-6620
	ctx.r7.s64 = ctx.r11.s64 + -6620;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-30712
	ctx.r6.s64 = ctx.r11.s64 + -30712;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-6556
	ctx.r31.s64 = ctx.r11.s64 + -6556;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6E260;
	sub_829DC008(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// addi r11,r11,-30852
	ctx.r11.s64 = ctx.r11.s64 + -30852;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829fb050
	ctx.lr = 0x82A6E270;
	sub_829FB050(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6E278;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-30716
	ctx.r11.s64 = ctx.r11.s64 + -30716;
	// addi r3,r10,16536
	ctx.r3.s64 = ctx.r10.s64 + 16536;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6E290;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6E2A8);
PPC_WEAK_FUNC(sub_82A6E2A8) { __imp__sub_82A6E2A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6E2A8) {
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
	// li r5,4
	ctx.r5.s64 = 4;
	// addi r7,r11,-6620
	ctx.r7.s64 = ctx.r11.s64 + -6620;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-30676
	ctx.r6.s64 = ctx.r11.s64 + -30676;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-6652
	ctx.r31.s64 = ctx.r11.s64 + -6652;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6E2E0;
	sub_829DC008(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// addi r11,r11,-30852
	ctx.r11.s64 = ctx.r11.s64 + -30852;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829fb050
	ctx.lr = 0x82A6E2F0;
	sub_829FB050(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6E2F8;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-30680
	ctx.r11.s64 = ctx.r11.s64 + -30680;
	// addi r3,r10,16616
	ctx.r3.s64 = ctx.r10.s64 + 16616;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6E310;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6E328);
PPC_WEAK_FUNC(sub_82A6E328) { __imp__sub_82A6E328(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6E328) {
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
	// li r5,5
	ctx.r5.s64 = 5;
	// addi r7,r11,-6620
	ctx.r7.s64 = ctx.r11.s64 + -6620;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-30644
	ctx.r6.s64 = ctx.r11.s64 + -30644;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-6492
	ctx.r31.s64 = ctx.r11.s64 + -6492;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6E360;
	sub_829DC008(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// addi r11,r11,-30852
	ctx.r11.s64 = ctx.r11.s64 + -30852;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829fb050
	ctx.lr = 0x82A6E370;
	sub_829FB050(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6E378;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-30648
	ctx.r11.s64 = ctx.r11.s64 + -30648;
	// addi r3,r10,16696
	ctx.r3.s64 = ctx.r10.s64 + 16696;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6E390;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6E3A8);
PPC_WEAK_FUNC(sub_82A6E3A8) { __imp__sub_82A6E3A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6E3A8) {
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
	// li r5,6
	ctx.r5.s64 = 6;
	// addi r7,r11,-6620
	ctx.r7.s64 = ctx.r11.s64 + -6620;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-30616
	ctx.r6.s64 = ctx.r11.s64 + -30616;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-6396
	ctx.r31.s64 = ctx.r11.s64 + -6396;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6E3E0;
	sub_829DC008(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// addi r11,r11,-30852
	ctx.r11.s64 = ctx.r11.s64 + -30852;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829fb050
	ctx.lr = 0x82A6E3F0;
	sub_829FB050(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6E3F8;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-30620
	ctx.r11.s64 = ctx.r11.s64 + -30620;
	// addi r3,r10,16776
	ctx.r3.s64 = ctx.r10.s64 + 16776;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6E410;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6E428);
PPC_WEAK_FUNC(sub_82A6E428) { __imp__sub_82A6E428(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6E428) {
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
	// li r5,7
	ctx.r5.s64 = 7;
	// addi r7,r11,-6620
	ctx.r7.s64 = ctx.r11.s64 + -6620;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-30580
	ctx.r6.s64 = ctx.r11.s64 + -30580;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-6460
	ctx.r31.s64 = ctx.r11.s64 + -6460;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6E460;
	sub_829DC008(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// addi r11,r11,-30852
	ctx.r11.s64 = ctx.r11.s64 + -30852;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829fb050
	ctx.lr = 0x82A6E470;
	sub_829FB050(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6E478;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-30584
	ctx.r11.s64 = ctx.r11.s64 + -30584;
	// addi r3,r10,16856
	ctx.r3.s64 = ctx.r10.s64 + 16856;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6E490;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6E4A8);
PPC_WEAK_FUNC(sub_82A6E4A8) { __imp__sub_82A6E4A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6E4A8) {
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
	// li r5,100
	ctx.r5.s64 = 100;
	// addi r7,r11,-7296
	ctx.r7.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-30100
	ctx.r6.s64 = ctx.r11.s64 + -30100;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-4664
	ctx.r31.s64 = ctx.r11.s64 + -4664;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6E4E0;
	sub_829DC008(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6E4F0;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6E4F8;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-30104
	ctx.r11.s64 = ctx.r11.s64 + -30104;
	// addi r3,r10,17024
	ctx.r3.s64 = ctx.r10.s64 + 17024;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6E510;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6E528);
PPC_WEAK_FUNC(sub_82A6E528) { __imp__sub_82A6E528(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6E528) {
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
	// li r5,101
	ctx.r5.s64 = 101;
	// addi r7,r11,-7296
	ctx.r7.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-30072
	ctx.r6.s64 = ctx.r11.s64 + -30072;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-4888
	ctx.r31.s64 = ctx.r11.s64 + -4888;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6E560;
	sub_829DC008(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6E570;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6E578;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-30076
	ctx.r11.s64 = ctx.r11.s64 + -30076;
	// addi r3,r10,17104
	ctx.r3.s64 = ctx.r10.s64 + 17104;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6E590;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6E5A8);
PPC_WEAK_FUNC(sub_82A6E5A8) { __imp__sub_82A6E5A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6E5A8) {
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
	// li r5,102
	ctx.r5.s64 = 102;
	// addi r7,r11,-7296
	ctx.r7.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-30032
	ctx.r6.s64 = ctx.r11.s64 + -30032;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-4824
	ctx.r31.s64 = ctx.r11.s64 + -4824;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6E5E0;
	sub_829DC008(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6E5F0;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6E5F8;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-30036
	ctx.r11.s64 = ctx.r11.s64 + -30036;
	// addi r3,r10,17184
	ctx.r3.s64 = ctx.r10.s64 + 17184;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6E610;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6E628);
PPC_WEAK_FUNC(sub_82A6E628) { __imp__sub_82A6E628(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6E628) {
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
	// li r5,103
	ctx.r5.s64 = 103;
	// addi r7,r11,-7296
	ctx.r7.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29992
	ctx.r6.s64 = ctx.r11.s64 + -29992;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-5080
	ctx.r31.s64 = ctx.r11.s64 + -5080;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6E660;
	sub_829DC008(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6E670;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6E678;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-29996
	ctx.r11.s64 = ctx.r11.s64 + -29996;
	// addi r3,r10,17264
	ctx.r3.s64 = ctx.r10.s64 + 17264;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6E690;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6E6A8);
PPC_WEAK_FUNC(sub_82A6E6A8) { __imp__sub_82A6E6A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6E6A8) {
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
	// li r5,104
	ctx.r5.s64 = 104;
	// addi r7,r11,-7296
	ctx.r7.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29952
	ctx.r6.s64 = ctx.r11.s64 + -29952;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-5208
	ctx.r31.s64 = ctx.r11.s64 + -5208;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6E6E0;
	sub_829DC008(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6E6F0;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6E6F8;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-29956
	ctx.r11.s64 = ctx.r11.s64 + -29956;
	// addi r3,r10,17344
	ctx.r3.s64 = ctx.r10.s64 + 17344;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6E710;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6E728);
PPC_WEAK_FUNC(sub_82A6E728) { __imp__sub_82A6E728(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6E728) {
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
	// li r5,105
	ctx.r5.s64 = 105;
	// addi r7,r11,-7296
	ctx.r7.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29916
	ctx.r6.s64 = ctx.r11.s64 + -29916;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-4984
	ctx.r31.s64 = ctx.r11.s64 + -4984;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6E760;
	sub_829DC008(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6E770;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6E778;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-29920
	ctx.r11.s64 = ctx.r11.s64 + -29920;
	// addi r3,r10,17424
	ctx.r3.s64 = ctx.r10.s64 + 17424;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6E790;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6E7A8);
PPC_WEAK_FUNC(sub_82A6E7A8) { __imp__sub_82A6E7A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6E7A8) {
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
	// li r5,106
	ctx.r5.s64 = 106;
	// addi r7,r11,-7296
	ctx.r7.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29868
	ctx.r6.s64 = ctx.r11.s64 + -29868;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-6204
	ctx.r31.s64 = ctx.r11.s64 + -6204;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6E7E0;
	sub_829DC008(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6E7F0;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6E7F8;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-29872
	ctx.r11.s64 = ctx.r11.s64 + -29872;
	// addi r3,r10,17504
	ctx.r3.s64 = ctx.r10.s64 + 17504;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6E810;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6E828);
PPC_WEAK_FUNC(sub_82A6E828) { __imp__sub_82A6E828(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6E828) {
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
	// li r5,107
	ctx.r5.s64 = 107;
	// addi r7,r11,-7296
	ctx.r7.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29824
	ctx.r6.s64 = ctx.r11.s64 + -29824;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-4760
	ctx.r31.s64 = ctx.r11.s64 + -4760;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6E860;
	sub_829DC008(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6E870;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6E878;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-29828
	ctx.r11.s64 = ctx.r11.s64 + -29828;
	// addi r3,r10,17584
	ctx.r3.s64 = ctx.r10.s64 + 17584;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6E890;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6E8A8);
PPC_WEAK_FUNC(sub_82A6E8A8) { __imp__sub_82A6E8A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6E8A8) {
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
	// li r5,108
	ctx.r5.s64 = 108;
	// addi r7,r11,-7296
	ctx.r7.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29780
	ctx.r6.s64 = ctx.r11.s64 + -29780;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-5176
	ctx.r31.s64 = ctx.r11.s64 + -5176;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6E8E0;
	sub_829DC008(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6E8F0;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6E8F8;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-29784
	ctx.r11.s64 = ctx.r11.s64 + -29784;
	// addi r3,r10,17664
	ctx.r3.s64 = ctx.r10.s64 + 17664;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6E910;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6E928);
PPC_WEAK_FUNC(sub_82A6E928) { __imp__sub_82A6E928(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6E928) {
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
	// li r5,109
	ctx.r5.s64 = 109;
	// addi r7,r11,-7296
	ctx.r7.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29736
	ctx.r6.s64 = ctx.r11.s64 + -29736;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-4600
	ctx.r31.s64 = ctx.r11.s64 + -4600;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6E960;
	sub_829DC008(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6E970;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6E978;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-29740
	ctx.r11.s64 = ctx.r11.s64 + -29740;
	// addi r3,r10,17744
	ctx.r3.s64 = ctx.r10.s64 + 17744;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6E990;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6E9A8);
PPC_WEAK_FUNC(sub_82A6E9A8) { __imp__sub_82A6E9A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6E9A8) {
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
	// li r5,110
	ctx.r5.s64 = 110;
	// addi r7,r11,-7296
	ctx.r7.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29692
	ctx.r6.s64 = ctx.r11.s64 + -29692;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-5240
	ctx.r31.s64 = ctx.r11.s64 + -5240;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6E9E0;
	sub_829DC008(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6E9F0;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6E9F8;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-29696
	ctx.r11.s64 = ctx.r11.s64 + -29696;
	// addi r3,r10,17824
	ctx.r3.s64 = ctx.r10.s64 + 17824;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6EA10;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6EA28);
PPC_WEAK_FUNC(sub_82A6EA28) { __imp__sub_82A6EA28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6EA28) {
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
	// li r5,111
	ctx.r5.s64 = 111;
	// addi r7,r11,-7296
	ctx.r7.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29648
	ctx.r6.s64 = ctx.r11.s64 + -29648;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-4792
	ctx.r31.s64 = ctx.r11.s64 + -4792;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6EA60;
	sub_829DC008(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6EA70;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6EA78;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-29652
	ctx.r11.s64 = ctx.r11.s64 + -29652;
	// addi r3,r10,17904
	ctx.r3.s64 = ctx.r10.s64 + 17904;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6EA90;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6EAA8);
PPC_WEAK_FUNC(sub_82A6EAA8) { __imp__sub_82A6EAA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6EAA8) {
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
	// li r5,112
	ctx.r5.s64 = 112;
	// addi r7,r11,-7296
	ctx.r7.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29608
	ctx.r6.s64 = ctx.r11.s64 + -29608;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-6172
	ctx.r31.s64 = ctx.r11.s64 + -6172;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6EAE0;
	sub_829DC008(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6EAF0;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6EAF8;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-29612
	ctx.r11.s64 = ctx.r11.s64 + -29612;
	// addi r3,r10,17984
	ctx.r3.s64 = ctx.r10.s64 + 17984;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6EB10;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6EB28);
PPC_WEAK_FUNC(sub_82A6EB28) { __imp__sub_82A6EB28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6EB28) {
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
	// li r5,113
	ctx.r5.s64 = 113;
	// addi r7,r11,-7296
	ctx.r7.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29572
	ctx.r6.s64 = ctx.r11.s64 + -29572;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-4632
	ctx.r31.s64 = ctx.r11.s64 + -4632;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6EB60;
	sub_829DC008(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6EB70;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6EB78;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-29576
	ctx.r11.s64 = ctx.r11.s64 + -29576;
	// addi r3,r10,18064
	ctx.r3.s64 = ctx.r10.s64 + 18064;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6EB90;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6EBA8);
PPC_WEAK_FUNC(sub_82A6EBA8) { __imp__sub_82A6EBA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6EBA8) {
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
	// li r5,114
	ctx.r5.s64 = 114;
	// addi r7,r11,-7296
	ctx.r7.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29532
	ctx.r6.s64 = ctx.r11.s64 + -29532;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-4728
	ctx.r31.s64 = ctx.r11.s64 + -4728;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6EBE0;
	sub_829DC008(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6EBF0;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6EBF8;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-29536
	ctx.r11.s64 = ctx.r11.s64 + -29536;
	// addi r3,r10,18144
	ctx.r3.s64 = ctx.r10.s64 + 18144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6EC10;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6EC28);
PPC_WEAK_FUNC(sub_82A6EC28) { __imp__sub_82A6EC28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6EC28) {
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
	// li r5,115
	ctx.r5.s64 = 115;
	// addi r7,r11,-7296
	ctx.r7.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29496
	ctx.r6.s64 = ctx.r11.s64 + -29496;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-6268
	ctx.r31.s64 = ctx.r11.s64 + -6268;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6EC60;
	sub_829DC008(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6EC70;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6EC78;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-29500
	ctx.r11.s64 = ctx.r11.s64 + -29500;
	// addi r3,r10,18224
	ctx.r3.s64 = ctx.r10.s64 + 18224;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6EC90;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6ECA8);
PPC_WEAK_FUNC(sub_82A6ECA8) { __imp__sub_82A6ECA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6ECA8) {
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
	// li r5,116
	ctx.r5.s64 = 116;
	// addi r7,r11,-7296
	ctx.r7.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29456
	ctx.r6.s64 = ctx.r11.s64 + -29456;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-6236
	ctx.r31.s64 = ctx.r11.s64 + -6236;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6ECE0;
	sub_829DC008(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6ECF0;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6ECF8;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-29460
	ctx.r11.s64 = ctx.r11.s64 + -29460;
	// addi r3,r10,18304
	ctx.r3.s64 = ctx.r10.s64 + 18304;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6ED10;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6ED28);
PPC_WEAK_FUNC(sub_82A6ED28) { __imp__sub_82A6ED28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6ED28) {
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
	// li r5,117
	ctx.r5.s64 = 117;
	// addi r7,r11,-7296
	ctx.r7.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29416
	ctx.r6.s64 = ctx.r11.s64 + -29416;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-5048
	ctx.r31.s64 = ctx.r11.s64 + -5048;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6ED60;
	sub_829DC008(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6ED70;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6ED78;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-29420
	ctx.r11.s64 = ctx.r11.s64 + -29420;
	// addi r3,r10,18384
	ctx.r3.s64 = ctx.r10.s64 + 18384;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6ED90;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6EDA8);
PPC_WEAK_FUNC(sub_82A6EDA8) { __imp__sub_82A6EDA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6EDA8) {
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
	// li r5,118
	ctx.r5.s64 = 118;
	// addi r7,r11,-7296
	ctx.r7.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29376
	ctx.r6.s64 = ctx.r11.s64 + -29376;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-5144
	ctx.r31.s64 = ctx.r11.s64 + -5144;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6EDE0;
	sub_829DC008(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6EDF0;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6EDF8;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-29380
	ctx.r11.s64 = ctx.r11.s64 + -29380;
	// addi r3,r10,18464
	ctx.r3.s64 = ctx.r10.s64 + 18464;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6EE10;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6EE28);
PPC_WEAK_FUNC(sub_82A6EE28) { __imp__sub_82A6EE28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6EE28) {
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
	// li r5,119
	ctx.r5.s64 = 119;
	// addi r7,r11,-7296
	ctx.r7.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29340
	ctx.r6.s64 = ctx.r11.s64 + -29340;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-5112
	ctx.r31.s64 = ctx.r11.s64 + -5112;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6EE60;
	sub_829DC008(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6EE70;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6EE78;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-29344
	ctx.r11.s64 = ctx.r11.s64 + -29344;
	// addi r3,r10,18544
	ctx.r3.s64 = ctx.r10.s64 + 18544;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6EE90;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6EEA8);
PPC_WEAK_FUNC(sub_82A6EEA8) { __imp__sub_82A6EEA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6EEA8) {
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
	// li r5,120
	ctx.r5.s64 = 120;
	// addi r7,r11,-7296
	ctx.r7.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29300
	ctx.r6.s64 = ctx.r11.s64 + -29300;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-4696
	ctx.r31.s64 = ctx.r11.s64 + -4696;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6EEE0;
	sub_829DC008(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6EEF0;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6EEF8;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-29304
	ctx.r11.s64 = ctx.r11.s64 + -29304;
	// addi r3,r10,18624
	ctx.r3.s64 = ctx.r10.s64 + 18624;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6EF10;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6EF28);
PPC_WEAK_FUNC(sub_82A6EF28) { __imp__sub_82A6EF28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6EF28) {
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
	// li r5,121
	ctx.r5.s64 = 121;
	// addi r7,r11,-7296
	ctx.r7.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29260
	ctx.r6.s64 = ctx.r11.s64 + -29260;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-4952
	ctx.r31.s64 = ctx.r11.s64 + -4952;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6EF60;
	sub_829DC008(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6EF70;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6EF78;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-29264
	ctx.r11.s64 = ctx.r11.s64 + -29264;
	// addi r3,r10,18704
	ctx.r3.s64 = ctx.r10.s64 + 18704;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6EF90;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6EFA8);
PPC_WEAK_FUNC(sub_82A6EFA8) { __imp__sub_82A6EFA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6EFA8) {
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
	// li r5,122
	ctx.r5.s64 = 122;
	// addi r7,r11,-7296
	ctx.r7.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29220
	ctx.r6.s64 = ctx.r11.s64 + -29220;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-4920
	ctx.r31.s64 = ctx.r11.s64 + -4920;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6EFE0;
	sub_829DC008(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6EFF0;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6EFF8;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-29224
	ctx.r11.s64 = ctx.r11.s64 + -29224;
	// addi r3,r10,18784
	ctx.r3.s64 = ctx.r10.s64 + 18784;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6F010;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6F028);
PPC_WEAK_FUNC(sub_82A6F028) { __imp__sub_82A6F028(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F028) {
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
	// li r5,123
	ctx.r5.s64 = 123;
	// addi r7,r11,-7296
	ctx.r7.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29180
	ctx.r6.s64 = ctx.r11.s64 + -29180;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-5016
	ctx.r31.s64 = ctx.r11.s64 + -5016;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6F060;
	sub_829DC008(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6F070;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6F078;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-29184
	ctx.r11.s64 = ctx.r11.s64 + -29184;
	// addi r3,r10,18864
	ctx.r3.s64 = ctx.r10.s64 + 18864;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6F090;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6F0A8);
PPC_WEAK_FUNC(sub_82A6F0A8) { __imp__sub_82A6F0A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F0A8) {
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
	// li r5,124
	ctx.r5.s64 = 124;
	// addi r7,r11,-7296
	ctx.r7.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29136
	ctx.r6.s64 = ctx.r11.s64 + -29136;
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r31,r11,-4856
	ctx.r31.s64 = ctx.r11.s64 + -4856;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dc008
	ctx.lr = 0x82A6F0E0;
	sub_829DC008(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6F0F0;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6F0F8;
	sub_829DC040(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-29140
	ctx.r11.s64 = ctx.r11.s64 + -29140;
	// addi r3,r10,18944
	ctx.r3.s64 = ctx.r10.s64 + 18944;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6F110;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6F128);
PPC_WEAK_FUNC(sub_82A6F128) { __imp__sub_82A6F128(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F128) {
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
	// li r31,3
	ctx.r31.s64 = 3;
	// addi r30,r11,-6136
	ctx.r30.s64 = ctx.r11.s64 + -6136;
loc_82A6F148:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x829e4828
	ctx.lr = 0x82A6F150;
	sub_829E4828(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,224
	ctx.r30.s64 = ctx.r30.s64 + 224;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x82a6f148
	if (!ctx.cr6.lt) goto loc_82A6F148;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,16936
	ctx.r3.s64 = ctx.r11.s64 + 16936;
	// bl 0x829ffa48
	ctx.lr = 0x82A6F16C;
	sub_829FFA48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6F188);
PPC_WEAK_FUNC(sub_82A6F188) { __imp__sub_82A6F188(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F188) {
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
	// lis r11,-32078
	ctx.r11.s64 = -2102263808;
	// addi r11,r11,-632
	ctx.r11.s64 = ctx.r11.s64 + -632;
	// addi r3,r11,4
	ctx.r3.s64 = ctx.r11.s64 + 4;
	// bl 0x82a74d74
	ctx.lr = 0x82A6F1A4;
	__imp__RtlInitializeCriticalSection(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,19024
	ctx.r3.s64 = ctx.r11.s64 + 19024;
	// bl 0x829ffa48
	ctx.lr = 0x82A6F1B0;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F1C0);
PPC_WEAK_FUNC(sub_82A6F1C0) { __imp__sub_82A6F1C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F1C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// li r5,44
	ctx.r5.s64 = 44;
	// addi r3,r11,-2464
	ctx.r3.s64 = ctx.r11.s64 + -2464;
	// li r4,0
	ctx.r4.s64 = 0;
	// b 0x829ff840
	sub_829FF840(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F1D8);
PPC_WEAK_FUNC(sub_82A6F1D8) { __imp__sub_82A6F1D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F1D8) {
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
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r3,r11,19596
	ctx.r3.s64 = ctx.r11.s64 + 19596;
	// bl 0x8285fe48
	ctx.lr = 0x82A6F1F0;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,19048
	ctx.r3.s64 = ctx.r11.s64 + 19048;
	// bl 0x829ffa48
	ctx.lr = 0x82A6F1FC;
	sub_829FFA48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F210);
PPC_WEAK_FUNC(sub_82A6F210) { __imp__sub_82A6F210(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F210) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,19064
	ctx.r3.s64 = ctx.r11.s64 + 19064;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F220);
PPC_WEAK_FUNC(sub_82A6F220) { __imp__sub_82A6F220(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F220) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31971
	ctx.r11.s64 = -2095251456;
	// addi r3,r11,26928
	ctx.r3.s64 = ctx.r11.s64 + 26928;
	// b 0x826dad00
	sub_826DAD00(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F230);
PPC_WEAK_FUNC(sub_82A6F230) { __imp__sub_82A6F230(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F230) {
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
	// addi r31,r11,-21892
	ctx.r31.s64 = ctx.r11.s64 + -21892;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8239a970
	ctx.lr = 0x82A6F254;
	sub_8239A970(ctx, base);
	// li r30,1
	ctx.r30.s64 = 1;
	// addi r31,r31,48
	ctx.r31.s64 = ctx.r31.s64 + 48;
loc_82A6F25C:
	// addi r31,r31,-4
	ctx.r31.s64 = ctx.r31.s64 + -4;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8227ecf8
	ctx.lr = 0x82A6F268;
	sub_8227ECF8(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a6f25c
	if (!ctx.cr6.lt) goto loc_82A6F25C;
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

PPC_FUNC_IMPL(__imp__sub_82A6F290);
PPC_WEAK_FUNC(sub_82A6F290) { __imp__sub_82A6F290(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F290) {
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
	// li r30,6
	ctx.r30.s64 = 6;
	// addi r11,r11,-21840
	ctx.r11.s64 = ctx.r11.s64 + -21840;
	// addi r31,r11,28
	ctx.r31.s64 = ctx.r11.s64 + 28;
loc_82A6F2B4:
	// addi r31,r31,-4
	ctx.r31.s64 = ctx.r31.s64 + -4;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8227ecf8
	ctx.lr = 0x82A6F2C0;
	sub_8227ECF8(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a6f2b4
	if (!ctx.cr6.lt) goto loc_82A6F2B4;
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

PPC_FUNC_IMPL(__imp__sub_82A6F2E8);
PPC_WEAK_FUNC(sub_82A6F2E8) { __imp__sub_82A6F2E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F2E8) {
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
	// li r30,3
	ctx.r30.s64 = 3;
	// addi r11,r11,-21704
	ctx.r11.s64 = ctx.r11.s64 + -21704;
	// addi r31,r11,32
	ctx.r31.s64 = ctx.r11.s64 + 32;
loc_82A6F30C:
	// addi r31,r31,-8
	ctx.r31.s64 = ctx.r31.s64 + -8;
	// lhz r5,6(r31)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r31.u32 + 6);
	// cmplwi cr6,r5,0
	ctx.cr6.compare<uint32_t>(ctx.r5.u32, 0, ctx.xer);
	// beq cr6,0x82a6f328
	if (ctx.cr6.eq) goto loc_82A6F328;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x82157bf8
	ctx.lr = 0x82A6F328;
	sub_82157BF8(ctx, base);
loc_82A6F328:
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a6f30c
	if (!ctx.cr6.lt) goto loc_82A6F30C;
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

PPC_FUNC_IMPL(__imp__sub_82A6F350);
PPC_WEAK_FUNC(sub_82A6F350) { __imp__sub_82A6F350(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F350) {
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
	// li r30,3
	ctx.r30.s64 = 3;
	// addi r11,r11,-21672
	ctx.r11.s64 = ctx.r11.s64 + -21672;
	// addi r31,r11,32
	ctx.r31.s64 = ctx.r11.s64 + 32;
loc_82A6F374:
	// addi r31,r31,-8
	ctx.r31.s64 = ctx.r31.s64 + -8;
	// lhz r11,6(r31)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r31.u32 + 6);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a6f38c
	if (ctx.cr6.eq) goto loc_82A6F38C;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x821b3560
	ctx.lr = 0x82A6F38C;
	sub_821B3560(ctx, base);
loc_82A6F38C:
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a6f374
	if (!ctx.cr6.lt) goto loc_82A6F374;
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

PPC_FUNC_IMPL(__imp__sub_82A6F3B0);
PPC_WEAK_FUNC(sub_82A6F3B0) { __imp__sub_82A6F3B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F3B0) {
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
	// li r30,3
	ctx.r30.s64 = 3;
	// addi r11,r11,-21640
	ctx.r11.s64 = ctx.r11.s64 + -21640;
	// addi r31,r11,32
	ctx.r31.s64 = ctx.r11.s64 + 32;
loc_82A6F3D4:
	// addi r31,r31,-8
	ctx.r31.s64 = ctx.r31.s64 + -8;
	// lhz r5,6(r31)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r31.u32 + 6);
	// cmplwi cr6,r5,0
	ctx.cr6.compare<uint32_t>(ctx.r5.u32, 0, ctx.xer);
	// beq cr6,0x82a6f3f0
	if (ctx.cr6.eq) goto loc_82A6F3F0;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x82157bf8
	ctx.lr = 0x82A6F3F0;
	sub_82157BF8(ctx, base);
loc_82A6F3F0:
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a6f3d4
	if (!ctx.cr6.lt) goto loc_82A6F3D4;
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

PPC_FUNC_IMPL(__imp__sub_82A6F418);
PPC_WEAK_FUNC(sub_82A6F418) { __imp__sub_82A6F418(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F418) {
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
	// li r30,3
	ctx.r30.s64 = 3;
	// addi r11,r11,-21608
	ctx.r11.s64 = ctx.r11.s64 + -21608;
	// addi r31,r11,32
	ctx.r31.s64 = ctx.r11.s64 + 32;
loc_82A6F43C:
	// addi r31,r31,-8
	ctx.r31.s64 = ctx.r31.s64 + -8;
	// lhz r11,6(r31)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r31.u32 + 6);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a6f454
	if (ctx.cr6.eq) goto loc_82A6F454;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x821b3560
	ctx.lr = 0x82A6F454;
	sub_821B3560(ctx, base);
loc_82A6F454:
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a6f43c
	if (!ctx.cr6.lt) goto loc_82A6F43C;
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

PPC_FUNC_IMPL(__imp__sub_82A6F478);
PPC_WEAK_FUNC(sub_82A6F478) { __imp__sub_82A6F478(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F478) {
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
	// li r30,51
	ctx.r30.s64 = 51;
	// addi r11,r11,-21576
	ctx.r11.s64 = ctx.r11.s64 + -21576;
	// addi r31,r11,628
	ctx.r31.s64 = ctx.r11.s64 + 628;
loc_82A6F49C:
	// addi r31,r31,-12
	ctx.r31.s64 = ctx.r31.s64 + -12;
	// lhz r11,6(r31)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r31.u32 + 6);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a6f4b4
	if (ctx.cr6.eq) goto loc_82A6F4B4;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x821b3560
	ctx.lr = 0x82A6F4B4;
	sub_821B3560(ctx, base);
loc_82A6F4B4:
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a6f49c
	if (!ctx.cr6.lt) goto loc_82A6F49C;
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

PPC_FUNC_IMPL(__imp__sub_82A6F4D8);
PPC_WEAK_FUNC(sub_82A6F4D8) { __imp__sub_82A6F4D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F4D8) {
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
	// li r30,54
	ctx.r30.s64 = 54;
	// addi r11,r11,-20952
	ctx.r11.s64 = ctx.r11.s64 + -20952;
	// addi r31,r11,1336
	ctx.r31.s64 = ctx.r11.s64 + 1336;
loc_82A6F4FC:
	// addi r31,r31,-24
	ctx.r31.s64 = ctx.r31.s64 + -24;
	// lhz r11,6(r31)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r31.u32 + 6);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a6f514
	if (ctx.cr6.eq) goto loc_82A6F514;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x821b3560
	ctx.lr = 0x82A6F514;
	sub_821B3560(ctx, base);
loc_82A6F514:
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a6f4fc
	if (!ctx.cr6.lt) goto loc_82A6F4FC;
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

PPC_FUNC_IMPL(__imp__sub_82A6F538);
PPC_WEAK_FUNC(sub_82A6F538) { __imp__sub_82A6F538(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F538) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c8
	ctx.lr = 0x82A6F540;
	__savegprlr_28(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// li r30,479
	ctx.r30.s64 = 479;
	// addi r11,r11,-19632
	ctx.r11.s64 = ctx.r11.s64 + -19632;
	// li r28,-1
	ctx.r28.s64 = -1;
	// addis r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 65536;
	// li r29,0
	ctx.r29.s64 = 0;
	// addi r31,r11,-27136
	ctx.r31.s64 = ctx.r11.s64 + -27136;
loc_82A6F560:
	// addi r31,r31,-80
	ctx.r31.s64 = ctx.r31.s64 + -80;
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a6f5a0
	if (ctx.cr6.eq) goto loc_82A6F5A0;
	// lwz r10,532(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 532);
	// lwz r10,284(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 284);
	// cmpwi cr6,r10,2
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 2, ctx.xer);
	// bne cr6,0x82a6f588
	if (!ctx.cr6.eq) goto loc_82A6F588;
	// lwz r11,548(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 548);
	// stw r28,84(r11)
	PPC_STORE_U32(ctx.r11.u32 + 84, ctx.r28.u32);
loc_82A6F588:
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x82a6f59c
	if (ctx.cr6.eq) goto loc_82A6F59C;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x823c05e8
	ctx.lr = 0x82A6F59C;
	sub_823C05E8(ctx, base);
loc_82A6F59C:
	// stw r29,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r29.u32);
loc_82A6F5A0:
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// sth r29,64(r31)
	PPC_STORE_U16(ctx.r31.u32 + 64, ctx.r29.u16);
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a6f560
	if (!ctx.cr6.lt) goto loc_82A6F560;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x829ff818
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F5B8);
PPC_WEAK_FUNC(sub_82A6F5B8) { __imp__sub_82A6F5B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F5B8) {
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
	// addi r31,r11,21568
	ctx.r31.s64 = ctx.r11.s64 + 21568;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x82a6f5e8
	if (ctx.cr6.eq) goto loc_82A6F5E8;
	// bl 0x823a1a60
	ctx.lr = 0x82A6F5E0;
	sub_823A1A60(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r11.u32);
loc_82A6F5E8:
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

PPC_FUNC_IMPL(__imp__sub_82A6F600);
PPC_WEAK_FUNC(sub_82A6F600) { __imp__sub_82A6F600(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F600) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// addi r3,r11,18768
	ctx.r3.s64 = ctx.r11.s64 + 18768;
	// b 0x8227ecf8
	sub_8227ECF8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F610);
PPC_WEAK_FUNC(sub_82A6F610) { __imp__sub_82A6F610(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F610) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// addi r3,r11,18772
	ctx.r3.s64 = ctx.r11.s64 + 18772;
	// b 0x8227ecf8
	sub_8227ECF8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F620);
PPC_WEAK_FUNC(sub_82A6F620) { __imp__sub_82A6F620(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F620) {
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
	// li r30,33
	ctx.r30.s64 = 33;
	// addi r11,r11,18776
	ctx.r11.s64 = ctx.r11.s64 + 18776;
	// addi r31,r11,136
	ctx.r31.s64 = ctx.r11.s64 + 136;
loc_82A6F644:
	// addi r31,r31,-4
	ctx.r31.s64 = ctx.r31.s64 + -4;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8227ecf8
	ctx.lr = 0x82A6F650;
	sub_8227ECF8(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a6f644
	if (!ctx.cr6.lt) goto loc_82A6F644;
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

PPC_FUNC_IMPL(__imp__sub_82A6F678);
PPC_WEAK_FUNC(sub_82A6F678) { __imp__sub_82A6F678(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F678) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// addi r11,r11,18912
	ctx.r11.s64 = ctx.r11.s64 + 18912;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x821b3560
	sub_821B3560(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F694);
PPC_WEAK_FUNC(sub_82A6F694) { __imp__sub_82A6F694(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F694) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F698);
PPC_WEAK_FUNC(sub_82A6F698) { __imp__sub_82A6F698(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F698) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// addi r3,r11,19000
	ctx.r3.s64 = ctx.r11.s64 + 19000;
	// b 0x82259e28
	sub_82259E28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F6A8);
PPC_WEAK_FUNC(sub_82A6F6A8) { __imp__sub_82A6F6A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F6A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// addi r3,r11,19012
	ctx.r3.s64 = ctx.r11.s64 + 19012;
	// b 0x82259e28
	sub_82259E28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F6B8);
PPC_WEAK_FUNC(sub_82A6F6B8) { __imp__sub_82A6F6B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F6B8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7cc
	ctx.lr = 0x82A6F6C0;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// addi r11,r11,19024
	ctx.r11.s64 = ctx.r11.s64 + 19024;
	// lhz r31,6(r11)
	ctx.r31.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x82a6f708
	if (ctx.cr6.eq) goto loc_82A6F708;
	// lwz r29,0(r11)
	ctx.r29.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// ble cr6,0x82a6f700
	if (!ctx.cr6.gt) goto loc_82A6F700;
	// mr r30,r29
	ctx.r30.u64 = ctx.r29.u64;
loc_82A6F6E8:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8227ecf8
	ctx.lr = 0x82A6F6F0;
	sub_8227ECF8(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,4
	ctx.r30.s64 = ctx.r30.s64 + 4;
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// bne cr6,0x82a6f6e8
	if (!ctx.cr6.eq) goto loc_82A6F6E8;
loc_82A6F700:
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x821b3560
	ctx.lr = 0x82A6F708;
	sub_821B3560(ctx, base);
loc_82A6F708:
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F710);
PPC_WEAK_FUNC(sub_82A6F710) { __imp__sub_82A6F710(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F710) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F718);
PPC_WEAK_FUNC(sub_82A6F718) { __imp__sub_82A6F718(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F718) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F720);
PPC_WEAK_FUNC(sub_82A6F720) { __imp__sub_82A6F720(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F720) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F728);
PPC_WEAK_FUNC(sub_82A6F728) { __imp__sub_82A6F728(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F728) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// addi r3,r11,19784
	ctx.r3.s64 = ctx.r11.s64 + 19784;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F738);
PPC_WEAK_FUNC(sub_82A6F738) { __imp__sub_82A6F738(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F738) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// addi r3,r11,19812
	ctx.r3.s64 = ctx.r11.s64 + 19812;
	// b 0x8216e0c8
	sub_8216E0C8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F748);
PPC_WEAK_FUNC(sub_82A6F748) { __imp__sub_82A6F748(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F748) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// addi r3,r11,22008
	ctx.r3.s64 = ctx.r11.s64 + 22008;
	// b 0x829dc318
	sub_829DC318(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F758);
PPC_WEAK_FUNC(sub_82A6F758) { __imp__sub_82A6F758(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F758) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// addi r11,r11,19844
	ctx.r11.s64 = ctx.r11.s64 + 19844;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x821b3560
	sub_821B3560(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F774);
PPC_WEAK_FUNC(sub_82A6F774) { __imp__sub_82A6F774(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F774) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F778);
PPC_WEAK_FUNC(sub_82A6F778) { __imp__sub_82A6F778(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F778) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// addi r11,r11,19852
	ctx.r11.s64 = ctx.r11.s64 + 19852;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x821b3560
	sub_821B3560(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F794);
PPC_WEAK_FUNC(sub_82A6F794) { __imp__sub_82A6F794(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F794) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F798);
PPC_WEAK_FUNC(sub_82A6F798) { __imp__sub_82A6F798(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F798) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// addi r11,r11,19876
	ctx.r11.s64 = ctx.r11.s64 + 19876;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x821b3560
	sub_821B3560(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F7B4);
PPC_WEAK_FUNC(sub_82A6F7B4) { __imp__sub_82A6F7B4(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F7B4) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F7B8);
PPC_WEAK_FUNC(sub_82A6F7B8) { __imp__sub_82A6F7B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F7B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lis r10,-31970
	ctx.r10.s64 = -2095185920;
	// addi r11,r11,2636
	ctx.r11.s64 = ctx.r11.s64 + 2636;
	// stw r11,22036(r10)
	PPC_STORE_U32(ctx.r10.u32 + 22036, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F7D0);
PPC_WEAK_FUNC(sub_82A6F7D0) { __imp__sub_82A6F7D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F7D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c4
	ctx.lr = 0x82A6F7D8;
	__savegprlr_27(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// li r27,0
	ctx.r27.s64 = 0;
	// addi r29,r11,19884
	ctx.r29.s64 = ctx.r11.s64 + 19884;
	// lhz r11,4(r29)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r29.u32 + 4);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a6f840
	if (ctx.cr6.eq) goto loc_82A6F840;
	// li r28,0
	ctx.r28.s64 = 0;
loc_82A6F7F8:
	// lwz r10,0(r29)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r29.u32 + 0);
	// lwzx r31,r28,r10
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r28.u32 + ctx.r10.u32);
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x82a6f82c
	if (ctx.cr6.eq) goto loc_82A6F82C;
loc_82A6F808:
	// mr r30,r31
	ctx.r30.u64 = ctx.r31.u64;
	// lwz r31,12(r31)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r31.u32 + 12);
	// lwz r3,0(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// bl 0x821b3560
	ctx.lr = 0x82A6F818;
	sub_821B3560(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x821b3560
	ctx.lr = 0x82A6F820;
	sub_821B3560(ctx, base);
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// bne cr6,0x82a6f808
	if (!ctx.cr6.eq) goto loc_82A6F808;
	// lhz r11,4(r29)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r29.u32 + 4);
loc_82A6F82C:
	// addi r27,r27,1
	ctx.r27.s64 = ctx.r27.s64 + 1;
	// clrlwi r10,r11,16
	ctx.r10.u64 = ctx.r11.u32 & 0xFFFF;
	// addi r28,r28,4
	ctx.r28.s64 = ctx.r28.s64 + 4;
	// cmplw cr6,r27,r10
	ctx.cr6.compare<uint32_t>(ctx.r27.u32, ctx.r10.u32, ctx.xer);
	// blt cr6,0x82a6f7f8
	if (ctx.cr6.lt) goto loc_82A6F7F8;
loc_82A6F840:
	// lwz r3,0(r29)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r29.u32 + 0);
	// bl 0x821b3560
	ctx.lr = 0x82A6F848;
	sub_821B3560(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,0(r29)
	PPC_STORE_U32(ctx.r29.u32 + 0, ctx.r11.u32);
	// sth r11,6(r29)
	PPC_STORE_U16(ctx.r29.u32 + 6, ctx.r11.u16);
	// sth r11,4(r29)
	PPC_STORE_U16(ctx.r29.u32 + 4, ctx.r11.u16);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x829ff814
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F860);
PPC_WEAK_FUNC(sub_82A6F860) { __imp__sub_82A6F860(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F860) {
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
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// addi r31,r11,-31972
	ctx.r31.s64 = ctx.r11.s64 + -31972;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// ble cr6,0x82a6f8c0
	if (!ctx.cr6.gt) goto loc_82A6F8C0;
	// lbz r11,24(r31)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r31.u32 + 24);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a6f8a0
	if (ctx.cr6.eq) goto loc_82A6F8A0;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x821b3560
	ctx.lr = 0x82A6F898;
	sub_821B3560(ctx, base);
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4);
	// bl 0x821b3560
	ctx.lr = 0x82A6F8A0;
	sub_821B3560(ctx, base);
loc_82A6F8A0:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// stw r11,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r11.u32);
	// stw r11,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r11.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// stw r11,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r11.u32);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,20(r31)
	PPC_STORE_U32(ctx.r31.u32 + 20, ctx.r11.u32);
loc_82A6F8C0:
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

PPC_FUNC_IMPL(__imp__sub_82A6F8D8);
PPC_WEAK_FUNC(sub_82A6F8D8) { __imp__sub_82A6F8D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F8D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7cc
	ctx.lr = 0x82A6F8E0;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// addi r11,r11,22312
	ctx.r11.s64 = ctx.r11.s64 + 22312;
	// lhz r31,6(r11)
	ctx.r31.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x82a6f928
	if (ctx.cr6.eq) goto loc_82A6F928;
	// lwz r29,0(r11)
	ctx.r29.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// ble cr6,0x82a6f920
	if (!ctx.cr6.gt) goto loc_82A6F920;
	// mr r30,r29
	ctx.r30.u64 = ctx.r29.u64;
loc_82A6F908:
	// lwz r3,0(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// bl 0x821b3560
	ctx.lr = 0x82A6F910;
	sub_821B3560(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,8
	ctx.r30.s64 = ctx.r30.s64 + 8;
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// bne cr6,0x82a6f908
	if (!ctx.cr6.eq) goto loc_82A6F908;
loc_82A6F920:
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x821b3560
	ctx.lr = 0x82A6F928;
	sub_821B3560(ctx, base);
loc_82A6F928:
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F930);
PPC_WEAK_FUNC(sub_82A6F930) { __imp__sub_82A6F930(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F930) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32244
	ctx.r11.s64 = -2113142784;
	// lis r10,-32077
	ctx.r10.s64 = -2102198272;
	// addi r11,r11,-7024
	ctx.r11.s64 = ctx.r11.s64 + -7024;
	// stw r11,-31944(r10)
	PPC_STORE_U32(ctx.r10.u32 + -31944, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F948);
PPC_WEAK_FUNC(sub_82A6F948) { __imp__sub_82A6F948(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F948) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32244
	ctx.r11.s64 = -2113142784;
	// lis r10,-32077
	ctx.r10.s64 = -2102198272;
	// addi r11,r11,-7024
	ctx.r11.s64 = ctx.r11.s64 + -7024;
	// stw r11,-31912(r10)
	PPC_STORE_U32(ctx.r10.u32 + -31912, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F960);
PPC_WEAK_FUNC(sub_82A6F960) { __imp__sub_82A6F960(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F960) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// addi r3,r11,-27392
	ctx.r3.s64 = ctx.r11.s64 + -27392;
	// b 0x828475c8
	sub_828475C8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F970);
PPC_WEAK_FUNC(sub_82A6F970) { __imp__sub_82A6F970(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F970) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// addi r3,r11,-27352
	ctx.r3.s64 = ctx.r11.s64 + -27352;
	// b 0x82847c70
	sub_82847C70(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F980);
PPC_WEAK_FUNC(sub_82A6F980) { __imp__sub_82A6F980(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F980) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// addi r3,r11,-27156
	ctx.r3.s64 = ctx.r11.s64 + -27156;
	// b 0x828486b0
	sub_828486B0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F990);
PPC_WEAK_FUNC(sub_82A6F990) { __imp__sub_82A6F990(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F990) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,0(r13)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r13.u32 + 0);
	// li r9,1676
	ctx.r9.s64 = 1676;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r4,r11,2408
	ctx.r4.s64 = ctx.r11.s64 + 2408;
	// lwzx r3,r9,r10
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r9.u32 + ctx.r10.u32);
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,48(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 48);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctr 
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F9C8);
PPC_WEAK_FUNC(sub_82A6F9C8) { __imp__sub_82A6F9C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F9C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// addi r3,r11,-5632
	ctx.r3.s64 = ctx.r11.s64 + -5632;
	// b 0x8285ec50
	sub_8285EC50(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F9D8);
PPC_WEAK_FUNC(sub_82A6F9D8) { __imp__sub_82A6F9D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F9D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// addi r3,r11,-4488
	ctx.r3.s64 = ctx.r11.s64 + -4488;
	// b 0x8285ec50
	sub_8285EC50(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6F9E8);
PPC_WEAK_FUNC(sub_82A6F9E8) { __imp__sub_82A6F9E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6F9E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lis r10,-32087
	ctx.r10.s64 = -2102853632;
	// addi r11,r11,2636
	ctx.r11.s64 = ctx.r11.s64 + 2636;
	// stw r11,6664(r10)
	PPC_STORE_U32(ctx.r10.u32 + 6664, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FA00);
PPC_WEAK_FUNC(sub_82A6FA00) { __imp__sub_82A6FA00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FA00) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lis r10,-32087
	ctx.r10.s64 = -2102853632;
	// addi r11,r11,2636
	ctx.r11.s64 = ctx.r11.s64 + 2636;
	// stw r11,6936(r10)
	PPC_STORE_U32(ctx.r10.u32 + 6936, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FA18);
PPC_WEAK_FUNC(sub_82A6FA18) { __imp__sub_82A6FA18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FA18) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lis r10,-32087
	ctx.r10.s64 = -2102853632;
	// addi r11,r11,2636
	ctx.r11.s64 = ctx.r11.s64 + 2636;
	// stw r11,7208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 7208, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FA30);
PPC_WEAK_FUNC(sub_82A6FA30) { __imp__sub_82A6FA30(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FA30) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lis r10,-32087
	ctx.r10.s64 = -2102853632;
	// addi r11,r11,2636
	ctx.r11.s64 = ctx.r11.s64 + 2636;
	// stw r11,7480(r10)
	PPC_STORE_U32(ctx.r10.u32 + 7480, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FA48);
PPC_WEAK_FUNC(sub_82A6FA48) { __imp__sub_82A6FA48(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FA48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lis r10,-32087
	ctx.r10.s64 = -2102853632;
	// addi r11,r11,2636
	ctx.r11.s64 = ctx.r11.s64 + 2636;
	// stw r11,7752(r10)
	PPC_STORE_U32(ctx.r10.u32 + 7752, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FA60);
PPC_WEAK_FUNC(sub_82A6FA60) { __imp__sub_82A6FA60(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FA60) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// addi r3,r11,6016
	ctx.r3.s64 = ctx.r11.s64 + 6016;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FA70);
PPC_WEAK_FUNC(sub_82A6FA70) { __imp__sub_82A6FA70(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FA70) {
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
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// addi r31,r11,-2080
	ctx.r31.s64 = ctx.r11.s64 + -2080;
	// lwz r3,4012(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4012);
	// bl 0x828498b0
	ctx.lr = 0x82A6FA90;
	sub_828498B0(ctx, base);
	// lwz r3,4016(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4016);
	// bl 0x828498b0
	ctx.lr = 0x82A6FA98;
	sub_828498B0(ctx, base);
	// lwz r3,4020(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4020);
	// bl 0x828498b0
	ctx.lr = 0x82A6FAA0;
	sub_828498B0(ctx, base);
	// lwz r3,4024(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4024);
	// bl 0x828498b0
	ctx.lr = 0x82A6FAA8;
	sub_828498B0(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6FAC0);
PPC_WEAK_FUNC(sub_82A6FAC0) { __imp__sub_82A6FAC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FAC0) {
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
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// addi r31,r11,6496
	ctx.r31.s64 = ctx.r11.s64 + 6496;
	// lwz r3,21732(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 21732);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x82a6faf4
	if (ctx.cr6.eq) goto loc_82A6FAF4;
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x82A6FAF4;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
loc_82A6FAF4:
	// lhz r11,7198(r31)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r31.u32 + 7198);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a6fb08
	if (ctx.cr6.eq) goto loc_82A6FB08;
	// lwz r3,7192(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 7192);
	// bl 0x821b3560
	ctx.lr = 0x82A6FB08;
	sub_821B3560(ctx, base);
loc_82A6FB08:
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

PPC_FUNC_IMPL(__imp__sub_82A6FB20);
PPC_WEAK_FUNC(sub_82A6FB20) { __imp__sub_82A6FB20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FB20) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FB28);
PPC_WEAK_FUNC(sub_82A6FB28) { __imp__sub_82A6FB28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FB28) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// addi r3,r11,28304
	ctx.r3.s64 = ctx.r11.s64 + 28304;
	// b 0x821bac60
	sub_821BAC60(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FB38);
PPC_WEAK_FUNC(sub_82A6FB38) { __imp__sub_82A6FB38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FB38) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// addi r3,r11,9716
	ctx.r3.s64 = ctx.r11.s64 + 9716;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r11,r11,3884
	ctx.r11.s64 = ctx.r11.s64 + 3884;
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r11.u32);
	// b 0x82847060
	sub_82847060(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FB50);
PPC_WEAK_FUNC(sub_82A6FB50) { __imp__sub_82A6FB50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FB50) {
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
	// lis r11,-32076
	ctx.r11.s64 = -2102132736;
	// addi r31,r11,-29864
	ctx.r31.s64 = ctx.r11.s64 + -29864;
	// addi r3,r31,60
	ctx.r3.s64 = ctx.r31.s64 + 60;
	// bl 0x828dca28
	ctx.lr = 0x82A6FB70;
	sub_828DCA28(ctx, base);
	// addi r3,r31,48
	ctx.r3.s64 = ctx.r31.s64 + 48;
	// bl 0x828dca28
	ctx.lr = 0x82A6FB78;
	sub_828DCA28(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A6FB90);
PPC_WEAK_FUNC(sub_82A6FB90) { __imp__sub_82A6FB90(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FB90) {
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
	// lis r11,-32076
	ctx.r11.s64 = -2102132736;
	// li r30,10
	ctx.r30.s64 = 10;
	// addi r11,r11,-27312
	ctx.r11.s64 = ctx.r11.s64 + -27312;
	// addi r31,r11,1056
	ctx.r31.s64 = ctx.r11.s64 + 1056;
loc_82A6FBB4:
	// addi r31,r31,-96
	ctx.r31.s64 = ctx.r31.s64 + -96;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82682900
	ctx.lr = 0x82A6FBC0;
	sub_82682900(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a6fbb4
	if (!ctx.cr6.lt) goto loc_82A6FBB4;
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

PPC_FUNC_IMPL(__imp__sub_82A6FBE8);
PPC_WEAK_FUNC(sub_82A6FBE8) { __imp__sub_82A6FBE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FBE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// addi r11,r11,12620
	ctx.r11.s64 = ctx.r11.s64 + 12620;
	// lhz r10,10(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 10);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,4(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x821b3560
	sub_821B3560(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FC04);
PPC_WEAK_FUNC(sub_82A6FC04) { __imp__sub_82A6FC04(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FC04) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FC08);
PPC_WEAK_FUNC(sub_82A6FC08) { __imp__sub_82A6FC08(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FC08) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FC10);
PPC_WEAK_FUNC(sub_82A6FC10) { __imp__sub_82A6FC10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FC10) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FC18);
PPC_WEAK_FUNC(sub_82A6FC18) { __imp__sub_82A6FC18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FC18) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FC20);
PPC_WEAK_FUNC(sub_82A6FC20) { __imp__sub_82A6FC20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FC20) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FC28);
PPC_WEAK_FUNC(sub_82A6FC28) { __imp__sub_82A6FC28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FC28) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

