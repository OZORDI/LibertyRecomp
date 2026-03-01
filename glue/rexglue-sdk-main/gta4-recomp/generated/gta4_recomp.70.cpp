#include "gta4_init.h"

PPC_FUNC_IMPL(__imp__sub_829EFEF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EFEF0) { __imp__sub_829EFEF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EFEF0) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,-29368
	ctx.r3.s64 = ctx.r11.s64 + -29368;
	// bl 0x8289df08
	ctx.lr = 0x829EFF08;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6688
	ctx.r3.s64 = ctx.r11.s64 + -6688;
	// bl 0x8298ed98
	ctx.lr = 0x829EFF14;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EFF28);  /* forward decl */
PPC_WEAK_FUNC(sub_829EFF28) { __imp__sub_829EFF28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EFF28) {
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
	// addi r3,r11,-24440
	ctx.r3.s64 = ctx.r11.s64 + -24440;
	// bl 0x827df490
	ctx.lr = 0x829EFF44;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29328(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29328, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EFF60);  /* forward decl */
PPC_WEAK_FUNC(sub_829EFF60) { __imp__sub_829EFF60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EFF60) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-21780
	ctx.r5.s64 = ctx.r11.s64 + -21780;
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-12796
	ctx.r3.s64 = ctx.r11.s64 + -12796;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EFF80);  /* forward decl */
PPC_WEAK_FUNC(sub_829EFF80) { __imp__sub_829EFF80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EFF80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// li r9,11
	ctx.r9.s64 = 11;
	// addi r11,r11,-12752
	ctx.r11.s64 = ctx.r11.s64 + -12752;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
loc_829EFF94:
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r10,-8(r11)
	PPC_STORE_U32(ctx.r11.u32 + -8, ctx.r10.u32);
	// stw r10,-4(r11)
	PPC_STORE_U32(ctx.r11.u32 + -4, ctx.r10.u32);
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// stw r10,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
	// stw r10,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r10.u32);
	// addi r11,r11,20
	ctx.r11.s64 = ctx.r11.s64 + 20;
	// bge cr6,0x829eff94
	if (!ctx.cr6.lt) goto loc_829EFF94;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EFFC0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EFFC0) { __imp__sub_829EFFC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EFFC0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x829EFFC8;
	__savegprlr_28(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// li r29,47
	ctx.r29.s64 = 47;
	// addi r11,r11,-12384
	ctx.r11.s64 = ctx.r11.s64 + -12384;
	// li r28,3
	ctx.r28.s64 = 3;
	// addi r31,r11,86
	ctx.r31.s64 = ctx.r11.s64 + 86;
	// li r30,0
	ctx.r30.s64 = 0;
loc_829EFFE4:
	// addi r3,r31,-78
	ctx.r3.s64 = ctx.r31.s64 + -78;
	// bl 0x8243b9e8
	ctx.lr = 0x829EFFEC;
	sub_8243B9E8(ctx, base);
	// lhz r10,0(r31)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r31.u32 + 0);
	// addi r11,r31,-86
	ctx.r11.s64 = ctx.r31.s64 + -86;
	// stw r30,-6(r31)
	PPC_STORE_U32(ctx.r31.u32 + -6, ctx.r30.u32);
	// rlwimi r10,r28,13,0,18
	ctx.r10.u64 = (__builtin_rotateleft32(ctx.r28.u32, 13) & 0xFFFFE000) | (ctx.r10.u64 & 0xFFFFFFFF00001FFF);
	// stb r30,-2(r31)
	PPC_STORE_U8(ctx.r31.u32 + -2, ctx.r30.u8);
	// li r3,0
	ctx.r3.s64 = 0;
	// stw r30,2(r31)
	PPC_STORE_U32(ctx.r31.u32 + 2, ctx.r30.u32);
	// stw r30,-86(r31)
	PPC_STORE_U32(ctx.r31.u32 + -86, ctx.r30.u32);
	// stw r11,-78(r31)
	PPC_STORE_U32(ctx.r31.u32 + -78, ctx.r11.u32);
	// sth r10,0(r31)
	PPC_STORE_U16(ctx.r31.u32 + 0, ctx.r10.u16);
	// bl 0x8266c6a8
	ctx.lr = 0x829F0018;
	sub_8266C6A8(ctx, base);
	// addi r29,r29,-1
	ctx.r29.s64 = ctx.r29.s64 + -1;
	// stw r3,-6(r31)
	PPC_STORE_U32(ctx.r31.u32 + -6, ctx.r3.u32);
	// addi r31,r31,92
	ctx.r31.s64 = ctx.r31.s64 + 92;
	// cmpwi cr6,r29,0
	ctx.cr6.compare<int32_t>(ctx.r29.s32, 0, ctx.xer);
	// bge cr6,0x829effe4
	if (!ctx.cr6.lt) goto loc_829EFFE4;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6680
	ctx.r3.s64 = ctx.r11.s64 + -6680;
	// bl 0x8298ed98
	ctx.lr = 0x829F0038;
	sub_8298ED98(ctx, base);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0040);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0040) { __imp__sub_829F0040(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0040) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6576
	ctx.r3.s64 = ctx.r11.s64 + -6576;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0050);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0050) { __imp__sub_829F0050(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0050) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6544
	ctx.r3.s64 = ctx.r11.s64 + -6544;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0060);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0060) { __imp__sub_829F0060(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0060) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,10952
	ctx.r11.s64 = ctx.r11.s64 + 10952;
	// lfs f0,12(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stfs f0,-7844(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -7844, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0078);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0078) { __imp__sub_829F0078(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0078) {
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
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-8884
	ctx.r3.s64 = ctx.r11.s64 + -8884;
	// bl 0x827df490
	ctx.lr = 0x829F0098;
	sub_827DF490(ctx, base);
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// addi r31,r10,-7768
	ctx.r31.s64 = ctx.r10.s64 + -7768;
	// addi r11,r11,-29044
	ctx.r11.s64 = ctx.r11.s64 + -29044;
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F00B8;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29124
	ctx.r11.s64 = ctx.r11.s64 + -29124;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F00D0;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29068
	ctx.r11.s64 = ctx.r11.s64 + -29068;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F00E8;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29236
	ctx.r11.s64 = ctx.r11.s64 + -29236;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0100;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-28964
	ctx.r11.s64 = ctx.r11.s64 + -28964;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0118;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,20(r31)
	PPC_STORE_U32(ctx.r31.u32 + 20, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29104
	ctx.r11.s64 = ctx.r11.s64 + -29104;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0130;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,24(r31)
	PPC_STORE_U32(ctx.r31.u32 + 24, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29288
	ctx.r11.s64 = ctx.r11.s64 + -29288;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0148;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,28(r31)
	PPC_STORE_U32(ctx.r31.u32 + 28, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29512
	ctx.r11.s64 = ctx.r11.s64 + -29512;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0160;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,32(r31)
	PPC_STORE_U32(ctx.r31.u32 + 32, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-8908
	ctx.r11.s64 = ctx.r11.s64 + -8908;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0178;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,36(r31)
	PPC_STORE_U32(ctx.r31.u32 + 36, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-8936
	ctx.r11.s64 = ctx.r11.s64 + -8936;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0190;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-28996
	ctx.r11.s64 = ctx.r11.s64 + -28996;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F01A8;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,44(r31)
	PPC_STORE_U32(ctx.r31.u32 + 44, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-28980
	ctx.r11.s64 = ctx.r11.s64 + -28980;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F01C0;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,48(r31)
	PPC_STORE_U32(ctx.r31.u32 + 48, ctx.r3.u32);
	// addi r11,r11,-29220
	ctx.r11.s64 = ctx.r11.s64 + -29220;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F01D8;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,52(r31)
	PPC_STORE_U32(ctx.r31.u32 + 52, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29180
	ctx.r11.s64 = ctx.r11.s64 + -29180;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F01F0;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,56(r31)
	PPC_STORE_U32(ctx.r31.u32 + 56, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29164
	ctx.r11.s64 = ctx.r11.s64 + -29164;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0208;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,60(r31)
	PPC_STORE_U32(ctx.r31.u32 + 60, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-29144
	ctx.r3.s64 = ctx.r11.s64 + -29144;
	// bl 0x827df490
	ctx.lr = 0x829F021C;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,64(r31)
	PPC_STORE_U32(ctx.r31.u32 + 64, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-8956
	ctx.r11.s64 = ctx.r11.s64 + -8956;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0234;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,68(r31)
	PPC_STORE_U32(ctx.r31.u32 + 68, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29020
	ctx.r11.s64 = ctx.r11.s64 + -29020;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F024C;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,72(r31)
	PPC_STORE_U32(ctx.r31.u32 + 72, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-28888
	ctx.r11.s64 = ctx.r11.s64 + -28888;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0264;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,76(r31)
	PPC_STORE_U32(ctx.r31.u32 + 76, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-28864
	ctx.r11.s64 = ctx.r11.s64 + -28864;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F027C;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,80(r31)
	PPC_STORE_U32(ctx.r31.u32 + 80, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-8976
	ctx.r11.s64 = ctx.r11.s64 + -8976;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0294;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,84(r31)
	PPC_STORE_U32(ctx.r31.u32 + 84, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-9000
	ctx.r11.s64 = ctx.r11.s64 + -9000;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F02AC;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,88(r31)
	PPC_STORE_U32(ctx.r31.u32 + 88, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29268
	ctx.r11.s64 = ctx.r11.s64 + -29268;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F02C4;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,92(r31)
	PPC_STORE_U32(ctx.r31.u32 + 92, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29252
	ctx.r11.s64 = ctx.r11.s64 + -29252;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F02DC;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,96(r31)
	PPC_STORE_U32(ctx.r31.u32 + 96, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-9024
	ctx.r11.s64 = ctx.r11.s64 + -9024;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F02F4;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,100(r31)
	PPC_STORE_U32(ctx.r31.u32 + 100, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-28916
	ctx.r11.s64 = ctx.r11.s64 + -28916;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F030C;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// addi r11,r11,-29308
	ctx.r11.s64 = ctx.r11.s64 + -29308;
	// stw r3,104(r31)
	PPC_STORE_U32(ctx.r31.u32 + 104, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0324;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,108(r31)
	PPC_STORE_U32(ctx.r31.u32 + 108, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29204
	ctx.r11.s64 = ctx.r11.s64 + -29204;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F033C;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,112(r31)
	PPC_STORE_U32(ctx.r31.u32 + 112, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-9048
	ctx.r11.s64 = ctx.r11.s64 + -9048;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0354;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,116(r31)
	PPC_STORE_U32(ctx.r31.u32 + 116, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-9064
	ctx.r11.s64 = ctx.r11.s64 + -9064;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F036C;
	sub_827DF490(ctx, base);
	// stw r3,120(r31)
	PPC_STORE_U32(ctx.r31.u32 + 120, ctx.r3.u32);
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

PPC_FUNC_IMPL(__imp__sub_829F0388);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0388) { __imp__sub_829F0388(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0388) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,-7444
	ctx.r3.s64 = ctx.r11.s64 + -7444;
	// bl 0x8289df08
	ctx.lr = 0x829F03A0;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6512
	ctx.r3.s64 = ctx.r11.s64 + -6512;
	// bl 0x8298ed98
	ctx.lr = 0x829F03AC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F03C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F03C0) { __imp__sub_829F03C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F03C0) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,-7404
	ctx.r3.s64 = ctx.r11.s64 + -7404;
	// bl 0x8289df08
	ctx.lr = 0x829F03D8;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6504
	ctx.r3.s64 = ctx.r11.s64 + -6504;
	// bl 0x8298ed98
	ctx.lr = 0x829F03E4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F03F8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F03F8) { __imp__sub_829F03F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F03F8) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,-7364
	ctx.r3.s64 = ctx.r11.s64 + -7364;
	// bl 0x8289df08
	ctx.lr = 0x829F0410;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6496
	ctx.r3.s64 = ctx.r11.s64 + -6496;
	// bl 0x8298ed98
	ctx.lr = 0x829F041C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0430);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0430) { __imp__sub_829F0430(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0430) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,-7324
	ctx.r3.s64 = ctx.r11.s64 + -7324;
	// bl 0x8289df08
	ctx.lr = 0x829F0448;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6488
	ctx.r3.s64 = ctx.r11.s64 + -6488;
	// bl 0x8298ed98
	ctx.lr = 0x829F0454;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0468);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0468) { __imp__sub_829F0468(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0468) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,-7284
	ctx.r3.s64 = ctx.r11.s64 + -7284;
	// bl 0x8289df08
	ctx.lr = 0x829F0480;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6480
	ctx.r3.s64 = ctx.r11.s64 + -6480;
	// bl 0x8298ed98
	ctx.lr = 0x829F048C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F04A0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F04A0) { __imp__sub_829F04A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F04A0) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,-7244
	ctx.r3.s64 = ctx.r11.s64 + -7244;
	// bl 0x8289df08
	ctx.lr = 0x829F04B8;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6472
	ctx.r3.s64 = ctx.r11.s64 + -6472;
	// bl 0x8298ed98
	ctx.lr = 0x829F04C4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F04D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F04D8) { __imp__sub_829F04D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F04D8) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,-7204
	ctx.r3.s64 = ctx.r11.s64 + -7204;
	// bl 0x8289df08
	ctx.lr = 0x829F04F0;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6464
	ctx.r3.s64 = ctx.r11.s64 + -6464;
	// bl 0x8298ed98
	ctx.lr = 0x829F04FC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0510);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0510) { __imp__sub_829F0510(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0510) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,-7164
	ctx.r3.s64 = ctx.r11.s64 + -7164;
	// bl 0x8289df08
	ctx.lr = 0x829F0528;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6456
	ctx.r3.s64 = ctx.r11.s64 + -6456;
	// bl 0x8298ed98
	ctx.lr = 0x829F0534;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0548);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0548) { __imp__sub_829F0548(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0548) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,-7124
	ctx.r3.s64 = ctx.r11.s64 + -7124;
	// bl 0x8289df08
	ctx.lr = 0x829F0560;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6448
	ctx.r3.s64 = ctx.r11.s64 + -6448;
	// bl 0x8298ed98
	ctx.lr = 0x829F056C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0580);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0580) { __imp__sub_829F0580(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0580) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,-7084
	ctx.r3.s64 = ctx.r11.s64 + -7084;
	// bl 0x8289df08
	ctx.lr = 0x829F0598;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6440
	ctx.r3.s64 = ctx.r11.s64 + -6440;
	// bl 0x8298ed98
	ctx.lr = 0x829F05A4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F05B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F05B8) { __imp__sub_829F05B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F05B8) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,-7044
	ctx.r3.s64 = ctx.r11.s64 + -7044;
	// bl 0x8289df08
	ctx.lr = 0x829F05D0;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6432
	ctx.r3.s64 = ctx.r11.s64 + -6432;
	// bl 0x8298ed98
	ctx.lr = 0x829F05DC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F05F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F05F0) { __imp__sub_829F05F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F05F0) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,-7004
	ctx.r3.s64 = ctx.r11.s64 + -7004;
	// bl 0x8289df08
	ctx.lr = 0x829F0608;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6424
	ctx.r3.s64 = ctx.r11.s64 + -6424;
	// bl 0x8298ed98
	ctx.lr = 0x829F0614;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0628);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0628) { __imp__sub_829F0628(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0628) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,-6964
	ctx.r3.s64 = ctx.r11.s64 + -6964;
	// bl 0x8289df08
	ctx.lr = 0x829F0640;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6416
	ctx.r3.s64 = ctx.r11.s64 + -6416;
	// bl 0x8298ed98
	ctx.lr = 0x829F064C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0660);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0660) { __imp__sub_829F0660(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0660) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,-6924
	ctx.r3.s64 = ctx.r11.s64 + -6924;
	// bl 0x8289df08
	ctx.lr = 0x829F0678;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6408
	ctx.r3.s64 = ctx.r11.s64 + -6408;
	// bl 0x8298ed98
	ctx.lr = 0x829F0684;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0698);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0698) { __imp__sub_829F0698(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0698) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,-6884
	ctx.r3.s64 = ctx.r11.s64 + -6884;
	// bl 0x8266ca60
	ctx.lr = 0x829F06B0;
	sub_8266CA60(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6400
	ctx.r3.s64 = ctx.r11.s64 + -6400;
	// bl 0x8298ed98
	ctx.lr = 0x829F06BC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F06D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F06D0) { __imp__sub_829F06D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F06D0) {
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
	// addi r3,r11,-5200
	ctx.r3.s64 = ctx.r11.s64 + -5200;
	// bl 0x827df490
	ctx.lr = 0x829F06EC;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-6868(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6868, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0708);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0708) { __imp__sub_829F0708(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0708) {
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
	// addi r3,r11,-5188
	ctx.r3.s64 = ctx.r11.s64 + -5188;
	// bl 0x827df490
	ctx.lr = 0x829F0724;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-6864(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6864, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0740);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0740) { __imp__sub_829F0740(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0740) {
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
	// addi r3,r11,-5172
	ctx.r3.s64 = ctx.r11.s64 + -5172;
	// bl 0x827df490
	ctx.lr = 0x829F075C;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-6860(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6860, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0778);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0778) { __imp__sub_829F0778(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0778) {
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
	// addi r3,r11,-5160
	ctx.r3.s64 = ctx.r11.s64 + -5160;
	// bl 0x827df490
	ctx.lr = 0x829F0794;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-6856(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6856, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F07B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F07B0) { __imp__sub_829F07B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F07B0) {
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
	// addi r3,r11,-5936
	ctx.r3.s64 = ctx.r11.s64 + -5936;
	// bl 0x827df490
	ctx.lr = 0x829F07CC;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-6852(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6852, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F07E8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F07E8) { __imp__sub_829F07E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F07E8) {
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
	// addi r3,r11,-5964
	ctx.r3.s64 = ctx.r11.s64 + -5964;
	// bl 0x827df490
	ctx.lr = 0x829F0804;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-6848(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6848, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0820);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0820) { __imp__sub_829F0820(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0820) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// addi r4,r11,-5132
	ctx.r4.s64 = ctx.r11.s64 + -5132;
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,-6844
	ctx.r3.s64 = ctx.r11.s64 + -6844;
	// b 0x8226f518
	sub_8226F518(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0838);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0838) { __imp__sub_829F0838(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0838) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// addi r4,r11,-5120
	ctx.r4.s64 = ctx.r11.s64 + -5120;
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,-6832
	ctx.r3.s64 = ctx.r11.s64 + -6832;
	// b 0x8226f518
	sub_8226F518(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0850);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0850) { __imp__sub_829F0850(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0850) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x829F0858;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r29,r11,-5040
	ctx.r29.s64 = ctx.r11.s64 + -5040;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0870;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r30,r11,-5060
	ctx.r30.s64 = ctx.r11.s64 + -5060;
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r31,r11,-6816
	ctx.r31.s64 = ctx.r11.s64 + -6816;
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0890;
	sub_827DF490(ctx, base);
	// stw r3,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x827df490
	ctx.lr = 0x829F08A0;
	sub_827DF490(ctx, base);
	// stw r3,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x827df490
	ctx.lr = 0x829F08B0;
	sub_827DF490(ctx, base);
	// stw r3,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x827df490
	ctx.lr = 0x829F08C0;
	sub_827DF490(ctx, base);
	// stw r3,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x827df490
	ctx.lr = 0x829F08D0;
	sub_827DF490(ctx, base);
	// stw r3,20(r31)
	PPC_STORE_U32(ctx.r31.u32 + 20, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x827df490
	ctx.lr = 0x829F08E0;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,24(r31)
	PPC_STORE_U32(ctx.r31.u32 + 24, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-5080
	ctx.r11.s64 = ctx.r11.s64 + -5080;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F08F8;
	sub_827DF490(ctx, base);
	// stw r3,28(r31)
	PPC_STORE_U32(ctx.r31.u32 + 28, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0908;
	sub_827DF490(ctx, base);
	// stw r3,32(r31)
	PPC_STORE_U32(ctx.r31.u32 + 32, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0918;
	sub_827DF490(ctx, base);
	// stw r3,36(r31)
	PPC_STORE_U32(ctx.r31.u32 + 36, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0928;
	sub_827DF490(ctx, base);
	// stw r3,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0938;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,44(r31)
	PPC_STORE_U32(ctx.r31.u32 + 44, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-5100
	ctx.r11.s64 = ctx.r11.s64 + -5100;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0950;
	sub_827DF490(ctx, base);
	// stw r3,48(r31)
	PPC_STORE_U32(ctx.r31.u32 + 48, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0960;
	sub_827DF490(ctx, base);
	// stw r3,52(r31)
	PPC_STORE_U32(ctx.r31.u32 + 52, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0970;
	sub_827DF490(ctx, base);
	// stw r3,56(r31)
	PPC_STORE_U32(ctx.r31.u32 + 56, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0980;
	sub_827DF490(ctx, base);
	// stw r3,60(r31)
	PPC_STORE_U32(ctx.r31.u32 + 60, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0990;
	sub_827DF490(ctx, base);
	// stw r3,64(r31)
	PPC_STORE_U32(ctx.r31.u32 + 64, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x827df490
	ctx.lr = 0x829F09A0;
	sub_827DF490(ctx, base);
	// stw r3,68(r31)
	PPC_STORE_U32(ctx.r31.u32 + 68, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x827df490
	ctx.lr = 0x829F09B0;
	sub_827DF490(ctx, base);
	// stw r3,72(r31)
	PPC_STORE_U32(ctx.r31.u32 + 72, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x827df490
	ctx.lr = 0x829F09C0;
	sub_827DF490(ctx, base);
	// stw r3,76(r31)
	PPC_STORE_U32(ctx.r31.u32 + 76, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x827df490
	ctx.lr = 0x829F09D0;
	sub_827DF490(ctx, base);
	// stw r3,80(r31)
	PPC_STORE_U32(ctx.r31.u32 + 80, ctx.r3.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F09E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F09E0) { __imp__sub_829F09E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F09E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x829F09E8;
	__savegprlr_28(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r29,r11,-4980
	ctx.r29.s64 = ctx.r11.s64 + -4980;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0A00;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r30,r11,-5004
	ctx.r30.s64 = ctx.r11.s64 + -5004;
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r31,r11,-6728
	ctx.r31.s64 = ctx.r11.s64 + -6728;
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0A20;
	sub_827DF490(ctx, base);
	// stw r3,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0A30;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r28,r11,-5040
	ctx.r28.s64 = ctx.r11.s64 + -5040;
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0A48;
	sub_827DF490(ctx, base);
	// stw r3,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0A58;
	sub_827DF490(ctx, base);
	// stw r3,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0A68;
	sub_827DF490(ctx, base);
	// stw r3,20(r31)
	PPC_STORE_U32(ctx.r31.u32 + 20, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0A78;
	sub_827DF490(ctx, base);
	// stw r3,24(r31)
	PPC_STORE_U32(ctx.r31.u32 + 24, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0A88;
	sub_827DF490(ctx, base);
	// stw r3,28(r31)
	PPC_STORE_U32(ctx.r31.u32 + 28, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0A98;
	sub_827DF490(ctx, base);
	// stw r3,32(r31)
	PPC_STORE_U32(ctx.r31.u32 + 32, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0AA8;
	sub_827DF490(ctx, base);
	// stw r3,36(r31)
	PPC_STORE_U32(ctx.r31.u32 + 36, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0AB8;
	sub_827DF490(ctx, base);
	// stw r3,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0AC8;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,44(r31)
	PPC_STORE_U32(ctx.r31.u32 + 44, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-5028
	ctx.r11.s64 = ctx.r11.s64 + -5028;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0AE0;
	sub_827DF490(ctx, base);
	// stw r3,48(r31)
	PPC_STORE_U32(ctx.r31.u32 + 48, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0AF0;
	sub_827DF490(ctx, base);
	// stw r3,52(r31)
	PPC_STORE_U32(ctx.r31.u32 + 52, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0B00;
	sub_827DF490(ctx, base);
	// stw r3,56(r31)
	PPC_STORE_U32(ctx.r31.u32 + 56, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0B10;
	sub_827DF490(ctx, base);
	// stw r3,60(r31)
	PPC_STORE_U32(ctx.r31.u32 + 60, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0B20;
	sub_827DF490(ctx, base);
	// stw r3,64(r31)
	PPC_STORE_U32(ctx.r31.u32 + 64, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0B30;
	sub_827DF490(ctx, base);
	// stw r3,68(r31)
	PPC_STORE_U32(ctx.r31.u32 + 68, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0B40;
	sub_827DF490(ctx, base);
	// stw r3,72(r31)
	PPC_STORE_U32(ctx.r31.u32 + 72, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0B50;
	sub_827DF490(ctx, base);
	// stw r3,76(r31)
	PPC_STORE_U32(ctx.r31.u32 + 76, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// bl 0x827df490
	ctx.lr = 0x829F0B60;
	sub_827DF490(ctx, base);
	// stw r3,80(r31)
	PPC_STORE_U32(ctx.r31.u32 + 80, ctx.r3.u32);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0B70);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0B70) { __imp__sub_829F0B70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0B70) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,-2956
	ctx.r3.s64 = ctx.r11.s64 + -2956;
	// bl 0x827f1320
	ctx.lr = 0x829F0B88;
	sub_827F1320(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6320
	ctx.r3.s64 = ctx.r11.s64 + -6320;
	// bl 0x8298ed98
	ctx.lr = 0x829F0B94;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0BA8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0BA8) { __imp__sub_829F0BA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0BA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6304
	ctx.r3.s64 = ctx.r11.s64 + -6304;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0BB8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0BB8) { __imp__sub_829F0BB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0BB8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6288
	ctx.r3.s64 = ctx.r11.s64 + -6288;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0BC8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0BC8) { __imp__sub_829F0BC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0BC8) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r31,r11,-2432
	ctx.r31.s64 = ctx.r11.s64 + -2432;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82893a58
	ctx.lr = 0x829F0BE8;
	sub_82893A58(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// addi r3,r31,28476
	ctx.r3.s64 = ctx.r31.s64 + 28476;
	// addi r11,r11,-388
	ctx.r11.s64 = ctx.r11.s64 + -388;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,28456(r31)
	PPC_STORE_U32(ctx.r31.u32 + 28456, ctx.r11.u32);
	// sth r11,28460(r31)
	PPC_STORE_U16(ctx.r31.u32 + 28460, ctx.r11.u16);
	// sth r11,28462(r31)
	PPC_STORE_U16(ctx.r31.u32 + 28462, ctx.r11.u16);
	// stw r11,28464(r31)
	PPC_STORE_U32(ctx.r31.u32 + 28464, ctx.r11.u32);
	// sth r11,28468(r31)
	PPC_STORE_U16(ctx.r31.u32 + 28468, ctx.r11.u16);
	// sth r11,28470(r31)
	PPC_STORE_U16(ctx.r31.u32 + 28470, ctx.r11.u16);
	// bl 0x827f1320
	ctx.lr = 0x829F0C18;
	sub_827F1320(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6216
	ctx.r3.s64 = ctx.r11.s64 + -6216;
	// bl 0x8298ed98
	ctx.lr = 0x829F0C24;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F0C38);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0C38) { __imp__sub_829F0C38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0C38) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,26080
	ctx.r3.s64 = ctx.r11.s64 + 26080;
	// bl 0x8289df08
	ctx.lr = 0x829F0C50;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6256
	ctx.r3.s64 = ctx.r11.s64 + -6256;
	// bl 0x8298ed98
	ctx.lr = 0x829F0C5C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0C70);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0C70) { __imp__sub_829F0C70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0C70) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// li r31,1
	ctx.r31.s64 = 1;
	// addi r30,r11,26120
	ctx.r30.s64 = ctx.r11.s64 + 26120;
loc_829F0C90:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8289df08
	ctx.lr = 0x829F0C98;
	sub_8289DF08(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,40
	ctx.r30.s64 = ctx.r30.s64 + 40;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x829f0c90
	if (!ctx.cr6.lt) goto loc_829F0C90;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6232
	ctx.r3.s64 = ctx.r11.s64 + -6232;
	// bl 0x8298ed98
	ctx.lr = 0x829F0CB4;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F0CD0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0CD0) { __imp__sub_829F0CD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0CD0) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// li r31,1
	ctx.r31.s64 = 1;
	// addi r30,r11,26200
	ctx.r30.s64 = ctx.r11.s64 + 26200;
loc_829F0CF0:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8289df08
	ctx.lr = 0x829F0CF8;
	sub_8289DF08(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,40
	ctx.r30.s64 = ctx.r30.s64 + 40;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x829f0cf0
	if (!ctx.cr6.lt) goto loc_829F0CF0;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6240
	ctx.r3.s64 = ctx.r11.s64 + -6240;
	// bl 0x8298ed98
	ctx.lr = 0x829F0D14;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F0D30);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0D30) { __imp__sub_829F0D30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0D30) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// li r31,1
	ctx.r31.s64 = 1;
	// addi r30,r11,26280
	ctx.r30.s64 = ctx.r11.s64 + 26280;
loc_829F0D50:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8289df08
	ctx.lr = 0x829F0D58;
	sub_8289DF08(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,40
	ctx.r30.s64 = ctx.r30.s64 + 40;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x829f0d50
	if (!ctx.cr6.lt) goto loc_829F0D50;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6248
	ctx.r3.s64 = ctx.r11.s64 + -6248;
	// bl 0x8298ed98
	ctx.lr = 0x829F0D74;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F0D90);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0D90) { __imp__sub_829F0D90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0D90) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,26360
	ctx.r3.s64 = ctx.r11.s64 + 26360;
	// bl 0x8289df08
	ctx.lr = 0x829F0DA8;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6224
	ctx.r3.s64 = ctx.r11.s64 + -6224;
	// bl 0x8298ed98
	ctx.lr = 0x829F0DB4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0DC8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0DC8) { __imp__sub_829F0DC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0DC8) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,27840
	ctx.r3.s64 = ctx.r11.s64 + 27840;
	// bl 0x824b6898
	ctx.lr = 0x829F0DE0;
	sub_824B6898(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6200
	ctx.r3.s64 = ctx.r11.s64 + -6200;
	// bl 0x8298ed98
	ctx.lr = 0x829F0DEC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0E00);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0E00) { __imp__sub_829F0E00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0E00) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6184
	ctx.r3.s64 = ctx.r11.s64 + -6184;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0E10);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0E10) { __imp__sub_829F0E10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0E10) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32011
	ctx.r11.s64 = -2097872896;
	// li r9,-1
	ctx.r9.s64 = -1;
	// addi r11,r11,-18152
	ctx.r11.s64 = ctx.r11.s64 + -18152;
	// li r10,196
	ctx.r10.s64 = 196;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_829F0E24:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x829f0e24
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_829F0E24;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0E38);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0E38) { __imp__sub_829F0E38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0E38) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32255
	ctx.r10.s64 = -2113863680;
	// lfs f0,-6588(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -6588);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32011
	ctx.r11.s64 = -2097872896;
	// addi r11,r11,-11760
	ctx.r11.s64 = ctx.r11.s64 + -11760;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// lfs f0,29496(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 29496);
	ctx.f0.f64 = double(temp.f32);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// lfs f0,8404(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 8404);
	ctx.f0.f64 = double(temp.f32);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// lfs f0,-9136(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -9136);
	ctx.f0.f64 = double(temp.f32);
	// lis r10,-32245
	ctx.r10.s64 = -2113208320;
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// lfs f0,-6348(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -6348);
	ctx.f0.f64 = double(temp.f32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// stfs f0,16(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 16, temp.u32);
	// lfs f0,3068(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 3068);
	ctx.f0.f64 = double(temp.f32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// stfs f0,20(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 20, temp.u32);
	// lfs f0,2592(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 2592);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,24(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 24, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0E98);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0E98) { __imp__sub_829F0E98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0E98) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eaf8
	ctx.lr = 0x829F0EA0;
	__savegprlr_20(ctx, base);
	// lis r11,-32011
	ctx.r11.s64 = -2097872896;
	// lis r26,-32245
	ctx.r26.s64 = -2113208320;
	// lis r27,-32245
	ctx.r27.s64 = -2113208320;
	// lis r28,-32256
	ctx.r28.s64 = -2113929216;
	// lis r29,-32255
	ctx.r29.s64 = -2113863680;
	// lis r30,-32256
	ctx.r30.s64 = -2113929216;
	// lis r31,-32256
	ctx.r31.s64 = -2113929216;
	// lfs f1,-6932(r26)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r26.u32 + -6932);
	ctx.f1.f64 = double(temp.f32);
	// lis r3,-32256
	ctx.r3.s64 = -2113929216;
	// lfs f2,-6360(r27)
	temp.u32 = PPC_LOAD_U32(ctx.r27.u32 + -6360);
	ctx.f2.f64 = double(temp.f32);
	// lis r4,-32256
	ctx.r4.s64 = -2113929216;
	// lfs f3,5564(r28)
	temp.u32 = PPC_LOAD_U32(ctx.r28.u32 + 5564);
	ctx.f3.f64 = double(temp.f32);
	// lis r5,-32256
	ctx.r5.s64 = -2113929216;
	// lfs f4,31416(r29)
	temp.u32 = PPC_LOAD_U32(ctx.r29.u32 + 31416);
	ctx.f4.f64 = double(temp.f32);
	// lis r6,-32255
	ctx.r6.s64 = -2113863680;
	// lfs f5,3068(r30)
	temp.u32 = PPC_LOAD_U32(ctx.r30.u32 + 3068);
	ctx.f5.f64 = double(temp.f32);
	// lis r7,-32254
	ctx.r7.s64 = -2113798144;
	// lfs f6,10284(r31)
	temp.u32 = PPC_LOAD_U32(ctx.r31.u32 + 10284);
	ctx.f6.f64 = double(temp.f32);
	// lis r8,-32245
	ctx.r8.s64 = -2113208320;
	// lfs f7,2980(r3)
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 2980);
	ctx.f7.f64 = double(temp.f32);
	// lis r9,-32245
	ctx.r9.s64 = -2113208320;
	// lfs f8,8912(r4)
	temp.u32 = PPC_LOAD_U32(ctx.r4.u32 + 8912);
	ctx.f8.f64 = double(temp.f32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lfs f9,5180(r5)
	temp.u32 = PPC_LOAD_U32(ctx.r5.u32 + 5180);
	ctx.f9.f64 = double(temp.f32);
	// addi r11,r11,-11728
	ctx.r11.s64 = ctx.r11.s64 + -11728;
	// lfs f10,17816(r6)
	temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + 17816);
	ctx.f10.f64 = double(temp.f32);
	// li r20,1
	ctx.r20.s64 = 1;
	// lfs f11,-23368(r7)
	temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + -23368);
	ctx.f11.f64 = double(temp.f32);
	// lfs f12,-6972(r8)
	temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + -6972);
	ctx.f12.f64 = double(temp.f32);
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// lfs f13,-6820(r9)
	temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + -6820);
	ctx.f13.f64 = double(temp.f32);
	// mr r25,r20
	ctx.r25.u64 = ctx.r20.u64;
	// lfs f0,8904(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 8904);
	ctx.f0.f64 = double(temp.f32);
	// li r24,0
	ctx.r24.s64 = 0;
	// li r21,800
	ctx.r21.s64 = 800;
	// li r22,200
	ctx.r22.s64 = 200;
	// li r23,700
	ctx.r23.s64 = 700;
loc_829F0F34:
	// addi r25,r25,-1
	ctx.r25.s64 = ctx.r25.s64 + -1;
	// stfs f0,-8(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -8, temp.u32);
	// stfs f13,-4(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + -4, temp.u32);
	// stw r24,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r24.u32);
	// stfs f12,0(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stw r21,32(r11)
	PPC_STORE_U32(ctx.r11.u32 + 32, ctx.r21.u32);
	// stfs f11,4(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stw r22,36(r11)
	PPC_STORE_U32(ctx.r11.u32 + 36, ctx.r22.u32);
	// stfs f10,8(r11)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// stw r23,40(r11)
	PPC_STORE_U32(ctx.r11.u32 + 40, ctx.r23.u32);
	// stfs f9,12(r11)
	temp.f32 = float(ctx.f9.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// stb r24,72(r11)
	PPC_STORE_U8(ctx.r11.u32 + 72, ctx.r24.u8);
	// stfs f8,16(r11)
	temp.f32 = float(ctx.f8.f64);
	PPC_STORE_U32(ctx.r11.u32 + 16, temp.u32);
	// stb r20,73(r11)
	PPC_STORE_U8(ctx.r11.u32 + 73, ctx.r20.u8);
	// stfs f7,20(r11)
	temp.f32 = float(ctx.f7.f64);
	PPC_STORE_U32(ctx.r11.u32 + 20, temp.u32);
	// cmpwi cr6,r25,0
	ctx.cr6.compare<int32_t>(ctx.r25.s32, 0, ctx.xer);
	// stfs f6,24(r11)
	temp.f32 = float(ctx.f6.f64);
	PPC_STORE_U32(ctx.r11.u32 + 24, temp.u32);
	// stfs f5,44(r11)
	temp.f32 = float(ctx.f5.f64);
	PPC_STORE_U32(ctx.r11.u32 + 44, temp.u32);
	// stfs f4,48(r11)
	temp.f32 = float(ctx.f4.f64);
	PPC_STORE_U32(ctx.r11.u32 + 48, temp.u32);
	// stfs f3,52(r11)
	temp.f32 = float(ctx.f3.f64);
	PPC_STORE_U32(ctx.r11.u32 + 52, temp.u32);
	// stfs f2,56(r11)
	temp.f32 = float(ctx.f2.f64);
	PPC_STORE_U32(ctx.r11.u32 + 56, temp.u32);
	// stfs f1,60(r11)
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r11.u32 + 60, temp.u32);
	// addi r11,r11,84
	ctx.r11.s64 = ctx.r11.s64 + 84;
	// bge cr6,0x829f0f34
	if (!ctx.cr6.lt) goto loc_829F0F34;
	// b 0x8298eb48
	__restgprlr_20(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0F98);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0F98) { __imp__sub_829F0F98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0F98) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r10,2
	ctx.r10.s64 = 2;
	// lfs f0,12408(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12408);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32011
	ctx.r11.s64 = -2097872896;
	// addi r11,r11,-11560
	ctx.r11.s64 = ctx.r11.s64 + -11560;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stw r10,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r10.u32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lfs f0,5584(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 5584);
	ctx.f0.f64 = double(temp.f32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stb r10,28(r11)
	PPC_STORE_U8(ctx.r11.u32 + 28, ctx.r10.u8);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lfs f0,8912(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 8912);
	ctx.f0.f64 = double(temp.f32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// lfs f0,2592(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 2592);
	ctx.f0.f64 = double(temp.f32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// stfs f0,20(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 20, temp.u32);
	// lfs f0,2980(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 2980);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,24(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 24, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F0FF8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F0FF8) { __imp__sub_829F0FF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F0FF8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eaf4
	ctx.lr = 0x829F1000;
	__savegprlr_19(ctx, base);
	// stfd f30,-128(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -128, ctx.f30.u64);
	// stfd f31,-120(r1)
	PPC_STORE_U64(ctx.r1.u32 + -120, ctx.f31.u64);
	// lis r11,-32011
	ctx.r11.s64 = -2097872896;
	// lis r23,-32256
	ctx.r23.s64 = -2113929216;
	// lis r24,-32256
	ctx.r24.s64 = -2113929216;
	// lis r25,-32255
	ctx.r25.s64 = -2113863680;
	// lis r26,-32256
	ctx.r26.s64 = -2113929216;
	// lis r27,-32256
	ctx.r27.s64 = -2113929216;
	// lis r28,-32256
	ctx.r28.s64 = -2113929216;
	// lfs f30,10284(r23)
	temp.u32 = PPC_LOAD_U32(ctx.r23.u32 + 10284);
	ctx.f30.f64 = double(temp.f32);
	// lis r29,-32245
	ctx.r29.s64 = -2113208320;
	// lfs f31,3068(r24)
	temp.u32 = PPC_LOAD_U32(ctx.r24.u32 + 3068);
	ctx.f31.f64 = double(temp.f32);
	// lis r30,-32256
	ctx.r30.s64 = -2113929216;
	// lfs f1,22804(r25)
	temp.u32 = PPC_LOAD_U32(ctx.r25.u32 + 22804);
	ctx.f1.f64 = double(temp.f32);
	// lis r31,-32256
	ctx.r31.s64 = -2113929216;
	// lfs f2,7916(r26)
	temp.u32 = PPC_LOAD_U32(ctx.r26.u32 + 7916);
	ctx.f2.f64 = double(temp.f32);
	// lis r3,-32256
	ctx.r3.s64 = -2113929216;
	// lfs f3,11476(r27)
	temp.u32 = PPC_LOAD_U32(ctx.r27.u32 + 11476);
	ctx.f3.f64 = double(temp.f32);
	// lis r4,-32256
	ctx.r4.s64 = -2113929216;
	// lfs f4,7956(r28)
	temp.u32 = PPC_LOAD_U32(ctx.r28.u32 + 7956);
	ctx.f4.f64 = double(temp.f32);
	// lis r5,-32255
	ctx.r5.s64 = -2113863680;
	// lfs f5,-6932(r29)
	temp.u32 = PPC_LOAD_U32(ctx.r29.u32 + -6932);
	ctx.f5.f64 = double(temp.f32);
	// lis r6,-32256
	ctx.r6.s64 = -2113929216;
	// lfs f9,2980(r30)
	temp.u32 = PPC_LOAD_U32(ctx.r30.u32 + 2980);
	ctx.f9.f64 = double(temp.f32);
	// lis r7,-32255
	ctx.r7.s64 = -2113863680;
	// lfs f10,8916(r31)
	temp.u32 = PPC_LOAD_U32(ctx.r31.u32 + 8916);
	ctx.f10.f64 = double(temp.f32);
	// lis r8,-32256
	ctx.r8.s64 = -2113929216;
	// lfs f11,5180(r3)
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 5180);
	ctx.f11.f64 = double(temp.f32);
	// lis r9,-32254
	ctx.r9.s64 = -2113798144;
	// lfs f6,8004(r4)
	temp.u32 = PPC_LOAD_U32(ctx.r4.u32 + 8004);
	ctx.f6.f64 = double(temp.f32);
	// addi r11,r11,-11528
	ctx.r11.s64 = ctx.r11.s64 + -11528;
	// lfs f7,29912(r5)
	temp.u32 = PPC_LOAD_U32(ctx.r5.u32 + 29912);
	ctx.f7.f64 = double(temp.f32);
	// lfs f13,8904(r6)
	temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + 8904);
	ctx.f13.f64 = double(temp.f32);
	// li r21,9
	ctx.r21.s64 = 9;
	// lfs f12,31416(r7)
	temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + 31416);
	ctx.f12.f64 = double(temp.f32);
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// lfs f0,2592(r8)
	temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + 2592);
	ctx.f0.f64 = double(temp.f32);
	// li r19,6
	ctx.r19.s64 = 6;
	// lfs f8,-10324(r9)
	temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + -10324);
	ctx.f8.f64 = double(temp.f32);
	// li r20,400
	ctx.r20.s64 = 400;
	// li r22,1000
	ctx.r22.s64 = 1000;
	// li r10,0
	ctx.r10.s64 = 0;
loc_829F10A8:
	// addi r21,r21,-1
	ctx.r21.s64 = ctx.r21.s64 + -1;
	// stfs f8,-8(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f8.f64);
	PPC_STORE_U32(ctx.r11.u32 + -8, temp.u32);
	// stfs f0,-4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -4, temp.u32);
	// stw r19,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r19.u32);
	// stfs f12,0(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stw r20,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r20.u32);
	// stfs f13,8(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// stw r22,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r22.u32);
	// stfs f13,12(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// stw r22,136(r11)
	PPC_STORE_U32(ctx.r11.u32 + 136, ctx.r22.u32);
	// stfs f7,36(r11)
	temp.f32 = float(ctx.f7.f64);
	PPC_STORE_U32(ctx.r11.u32 + 36, temp.u32);
	// stb r10,140(r11)
	PPC_STORE_U8(ctx.r11.u32 + 140, ctx.r10.u8);
	// stfs f6,40(r11)
	temp.f32 = float(ctx.f6.f64);
	PPC_STORE_U32(ctx.r11.u32 + 40, temp.u32);
	// stb r10,141(r11)
	PPC_STORE_U8(ctx.r11.u32 + 141, ctx.r10.u8);
	// stfs f0,44(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 44, temp.u32);
	// stb r10,142(r11)
	PPC_STORE_U8(ctx.r11.u32 + 142, ctx.r10.u8);
	// stfs f11,48(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 48, temp.u32);
	// stb r10,143(r11)
	PPC_STORE_U8(ctx.r11.u32 + 143, ctx.r10.u8);
	// stfs f10,52(r11)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r11.u32 + 52, temp.u32);
	// stb r10,144(r11)
	PPC_STORE_U8(ctx.r11.u32 + 144, ctx.r10.u8);
	// stfs f0,56(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 56, temp.u32);
	// cmpwi cr6,r21,0
	ctx.cr6.compare<int32_t>(ctx.r21.s32, 0, ctx.xer);
	// stfs f11,60(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 60, temp.u32);
	// stfs f9,64(r11)
	temp.f32 = float(ctx.f9.f64);
	PPC_STORE_U32(ctx.r11.u32 + 64, temp.u32);
	// stfs f0,68(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 68, temp.u32);
	// stfs f12,72(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 72, temp.u32);
	// stfs f5,76(r11)
	temp.f32 = float(ctx.f5.f64);
	PPC_STORE_U32(ctx.r11.u32 + 76, temp.u32);
	// stfs f4,80(r11)
	temp.f32 = float(ctx.f4.f64);
	PPC_STORE_U32(ctx.r11.u32 + 80, temp.u32);
	// stfs f3,84(r11)
	temp.f32 = float(ctx.f3.f64);
	PPC_STORE_U32(ctx.r11.u32 + 84, temp.u32);
	// stfs f13,88(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 88, temp.u32);
	// stfs f0,92(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 92, temp.u32);
	// stfs f2,96(r11)
	temp.f32 = float(ctx.f2.f64);
	PPC_STORE_U32(ctx.r11.u32 + 96, temp.u32);
	// stfs f1,100(r11)
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r11.u32 + 100, temp.u32);
	// stfs f31,104(r11)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r11.u32 + 104, temp.u32);
	// stfs f10,108(r11)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r11.u32 + 108, temp.u32);
	// stfs f9,112(r11)
	temp.f32 = float(ctx.f9.f64);
	PPC_STORE_U32(ctx.r11.u32 + 112, temp.u32);
	// stfs f30,132(r11)
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r11.u32 + 132, temp.u32);
	// addi r11,r11,156
	ctx.r11.s64 = ctx.r11.s64 + 156;
	// bge cr6,0x829f10a8
	if (!ctx.cr6.lt) goto loc_829F10A8;
	// lfd f30,-128(r1)
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -128);
	// lfd f31,-120(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -120);
	// b 0x8298eb44
	__restgprlr_19(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F1150);  /* forward decl */
PPC_WEAK_FUNC(sub_829F1150) { __imp__sub_829F1150(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F1150) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r10,0
	ctx.r10.s64 = 0;
	// lfs f0,2592(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2592);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32011
	ctx.r11.s64 = -2097872896;
	// addi r11,r11,-9968
	ctx.r11.s64 = ctx.r11.s64 + -9968;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stb r10,60(r11)
	PPC_STORE_U8(ctx.r11.u32 + 60, ctx.r10.u8);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lfs f13,8004(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 8004);
	ctx.f13.f64 = double(temp.f32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stfs f13,4(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stb r10,61(r11)
	PPC_STORE_U8(ctx.r11.u32 + 61, ctx.r10.u8);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lfs f11,12408(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 12408);
	ctx.f11.f64 = double(temp.f32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stfs f11,8(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// stb r10,62(r11)
	PPC_STORE_U8(ctx.r11.u32 + 62, ctx.r10.u8);
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// stb r10,124(r11)
	PPC_STORE_U8(ctx.r11.u32 + 124, ctx.r10.u8);
	// lis r10,-32255
	ctx.r10.s64 = -2113863680;
	// lfs f10,17816(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 17816);
	ctx.f10.f64 = double(temp.f32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stfs f10,16(r11)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r11.u32 + 16, temp.u32);
	// stb r10,125(r11)
	PPC_STORE_U8(ctx.r11.u32 + 125, ctx.r10.u8);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// lfs f12,8136(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 8136);
	ctx.f12.f64 = double(temp.f32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stfs f12,20(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 20, temp.u32);
	// stb r10,126(r11)
	PPC_STORE_U8(ctx.r11.u32 + 126, ctx.r10.u8);
	// lis r10,-32255
	ctx.r10.s64 = -2113863680;
	// lfs f9,29912(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 29912);
	ctx.f9.f64 = double(temp.f32);
	// lis r10,-32245
	ctx.r10.s64 = -2113208320;
	// stfs f9,24(r11)
	temp.f32 = float(ctx.f9.f64);
	PPC_STORE_U32(ctx.r11.u32 + 24, temp.u32);
	// stfs f12,28(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 28, temp.u32);
	// lfs f8,-6392(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -6392);
	ctx.f8.f64 = double(temp.f32);
	// lis r10,-32255
	ctx.r10.s64 = -2113863680;
	// stfs f8,32(r11)
	temp.f32 = float(ctx.f8.f64);
	PPC_STORE_U32(ctx.r11.u32 + 32, temp.u32);
	// lfs f7,31412(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 31412);
	ctx.f7.f64 = double(temp.f32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// stfs f7,36(r11)
	temp.f32 = float(ctx.f7.f64);
	PPC_STORE_U32(ctx.r11.u32 + 36, temp.u32);
	// lfs f6,2980(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 2980);
	ctx.f6.f64 = double(temp.f32);
	// lis r10,-32255
	ctx.r10.s64 = -2113863680;
	// stfs f6,40(r11)
	temp.f32 = float(ctx.f6.f64);
	PPC_STORE_U32(ctx.r11.u32 + 40, temp.u32);
	// stfs f13,44(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 44, temp.u32);
	// lfs f5,31416(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 31416);
	ctx.f5.f64 = double(temp.f32);
	// lis r10,-32245
	ctx.r10.s64 = -2113208320;
	// stfs f5,48(r11)
	temp.f32 = float(ctx.f5.f64);
	PPC_STORE_U32(ctx.r11.u32 + 48, temp.u32);
	// lfs f4,-6972(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -6972);
	ctx.f4.f64 = double(temp.f32);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// stfs f4,52(r11)
	temp.f32 = float(ctx.f4.f64);
	PPC_STORE_U32(ctx.r11.u32 + 52, temp.u32);
	// lfs f3,9576(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 9576);
	ctx.f3.f64 = double(temp.f32);
	// stfs f3,56(r11)
	temp.f32 = float(ctx.f3.f64);
	PPC_STORE_U32(ctx.r11.u32 + 56, temp.u32);
	// stfs f0,64(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 64, temp.u32);
	// stfs f13,68(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 68, temp.u32);
	// stfs f11,72(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 72, temp.u32);
	// stfs f0,76(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 76, temp.u32);
	// stfs f10,80(r11)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r11.u32 + 80, temp.u32);
	// stfs f12,84(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 84, temp.u32);
	// stfs f9,88(r11)
	temp.f32 = float(ctx.f9.f64);
	PPC_STORE_U32(ctx.r11.u32 + 88, temp.u32);
	// stfs f12,92(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 92, temp.u32);
	// stfs f8,96(r11)
	temp.f32 = float(ctx.f8.f64);
	PPC_STORE_U32(ctx.r11.u32 + 96, temp.u32);
	// stfs f7,100(r11)
	temp.f32 = float(ctx.f7.f64);
	PPC_STORE_U32(ctx.r11.u32 + 100, temp.u32);
	// stfs f6,104(r11)
	temp.f32 = float(ctx.f6.f64);
	PPC_STORE_U32(ctx.r11.u32 + 104, temp.u32);
	// stfs f13,108(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 108, temp.u32);
	// stfs f5,112(r11)
	temp.f32 = float(ctx.f5.f64);
	PPC_STORE_U32(ctx.r11.u32 + 112, temp.u32);
	// stfs f4,116(r11)
	temp.f32 = float(ctx.f4.f64);
	PPC_STORE_U32(ctx.r11.u32 + 116, temp.u32);
	// stfs f3,120(r11)
	temp.f32 = float(ctx.f3.f64);
	PPC_STORE_U32(ctx.r11.u32 + 120, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F1260);  /* forward decl */
PPC_WEAK_FUNC(sub_829F1260) { __imp__sub_829F1260(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F1260) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eaf0
	ctx.lr = 0x829F1268;
	__savegprlr_18(ctx, base);
	// addi r12,r1,-120
	ctx.r12.s64 = ctx.r1.s64 + -120;
	// bl 0x82990140
	ctx.lr = 0x829F1270;
	__savefpr_26(ctx, base);
	// lis r11,-32011
	ctx.r11.s64 = -2097872896;
	// lis r20,-32254
	ctx.r20.s64 = -2113798144;
	// lis r21,-32256
	ctx.r21.s64 = -2113929216;
	// lis r22,-32245
	ctx.r22.s64 = -2113208320;
	// lis r23,-32255
	ctx.r23.s64 = -2113863680;
	// lis r24,-32256
	ctx.r24.s64 = -2113929216;
	// lis r25,-32252
	ctx.r25.s64 = -2113667072;
	// lfs f12,8136(r20)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r20.u32 + 8136);
	ctx.f12.f64 = double(temp.f32);
	// lis r26,-32245
	ctx.r26.s64 = -2113208320;
	// lfs f5,5560(r21)
	temp.u32 = PPC_LOAD_U32(ctx.r21.u32 + 5560);
	ctx.f5.f64 = double(temp.f32);
	// lis r27,-32256
	ctx.r27.s64 = -2113929216;
	// lfs f26,-6972(r22)
	temp.u32 = PPC_LOAD_U32(ctx.r22.u32 + -6972);
	ctx.f26.f64 = double(temp.f32);
	// lis r28,-32256
	ctx.r28.s64 = -2113929216;
	// lfs f27,29912(r23)
	temp.u32 = PPC_LOAD_U32(ctx.r23.u32 + 29912);
	ctx.f27.f64 = double(temp.f32);
	// lis r29,-32256
	ctx.r29.s64 = -2113929216;
	// lfs f28,2980(r24)
	temp.u32 = PPC_LOAD_U32(ctx.r24.u32 + 2980);
	ctx.f28.f64 = double(temp.f32);
	// lis r30,-32255
	ctx.r30.s64 = -2113863680;
	// lfs f29,10520(r25)
	temp.u32 = PPC_LOAD_U32(ctx.r25.u32 + 10520);
	ctx.f29.f64 = double(temp.f32);
	// lis r31,-32256
	ctx.r31.s64 = -2113929216;
	// lfs f6,-6396(r26)
	temp.u32 = PPC_LOAD_U32(ctx.r26.u32 + -6396);
	ctx.f6.f64 = double(temp.f32);
	// lis r3,-32256
	ctx.r3.s64 = -2113929216;
	// lfs f7,8916(r27)
	temp.u32 = PPC_LOAD_U32(ctx.r27.u32 + 8916);
	ctx.f7.f64 = double(temp.f32);
	// lis r4,-32256
	ctx.r4.s64 = -2113929216;
	// lfs f30,8912(r28)
	temp.u32 = PPC_LOAD_U32(ctx.r28.u32 + 8912);
	ctx.f30.f64 = double(temp.f32);
	// lis r5,-32254
	ctx.r5.s64 = -2113798144;
	// lfs f31,4208(r29)
	temp.u32 = PPC_LOAD_U32(ctx.r29.u32 + 4208);
	ctx.f31.f64 = double(temp.f32);
	// lis r6,-32256
	ctx.r6.s64 = -2113929216;
	// lfs f8,31416(r30)
	temp.u32 = PPC_LOAD_U32(ctx.r30.u32 + 31416);
	ctx.f8.f64 = double(temp.f32);
	// lis r7,-32256
	ctx.r7.s64 = -2113929216;
	// lfs f1,10176(r31)
	temp.u32 = PPC_LOAD_U32(ctx.r31.u32 + 10176);
	ctx.f1.f64 = double(temp.f32);
	// lis r8,-32256
	ctx.r8.s64 = -2113929216;
	// lfs f13,3068(r3)
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 3068);
	ctx.f13.f64 = double(temp.f32);
	// lis r9,-32245
	ctx.r9.s64 = -2113208320;
	// lfs f9,8004(r4)
	temp.u32 = PPC_LOAD_U32(ctx.r4.u32 + 8004);
	ctx.f9.f64 = double(temp.f32);
	// lis r10,-32255
	ctx.r10.s64 = -2113863680;
	// lfs f2,-23880(r5)
	temp.u32 = PPC_LOAD_U32(ctx.r5.u32 + -23880);
	ctx.f2.f64 = double(temp.f32);
	// addi r11,r11,-9840
	ctx.r11.s64 = ctx.r11.s64 + -9840;
	// lfs f10,8904(r6)
	temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + 8904);
	ctx.f10.f64 = double(temp.f32);
	// lfs f0,2592(r7)
	temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + 2592);
	ctx.f0.f64 = double(temp.f32);
	// li r18,3
	ctx.r18.s64 = 3;
	// lfs f3,10284(r8)
	temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + 10284);
	ctx.f3.f64 = double(temp.f32);
	// addi r11,r11,88
	ctx.r11.s64 = ctx.r11.s64 + 88;
	// lfs f11,-6820(r9)
	temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + -6820);
	ctx.f11.f64 = double(temp.f32);
	// li r19,1
	ctx.r19.s64 = 1;
	// lfs f4,9008(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 9008);
	ctx.f4.f64 = double(temp.f32);
loc_829F1324:
	// stfs f4,-88(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f4.f64);
	PPC_STORE_U32(ctx.r11.u32 + -88, temp.u32);
	// addi r18,r18,-1
	ctx.r18.s64 = ctx.r18.s64 + -1;
	// stfs f11,-84(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + -84, temp.u32);
	// stfs f3,-80(r11)
	temp.f32 = float(ctx.f3.f64);
	PPC_STORE_U32(ctx.r11.u32 + -80, temp.u32);
	// cmpwi cr6,r18,0
	ctx.cr6.compare<int32_t>(ctx.r18.s32, 0, ctx.xer);
	// stfs f0,-76(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -76, temp.u32);
	// stfs f10,-72(r11)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r11.u32 + -72, temp.u32);
	// stfs f2,-68(r11)
	temp.f32 = float(ctx.f2.f64);
	PPC_STORE_U32(ctx.r11.u32 + -68, temp.u32);
	// stfs f12,-64(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + -64, temp.u32);
	// stfs f9,-60(r11)
	temp.f32 = float(ctx.f9.f64);
	PPC_STORE_U32(ctx.r11.u32 + -60, temp.u32);
	// stfs f13,-56(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + -56, temp.u32);
	// stfs f1,-52(r11)
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r11.u32 + -52, temp.u32);
	// stfs f0,-48(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -48, temp.u32);
	// stfs f0,-44(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -44, temp.u32);
	// stfs f8,-40(r11)
	temp.f32 = float(ctx.f8.f64);
	PPC_STORE_U32(ctx.r11.u32 + -40, temp.u32);
	// stfs f0,-36(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -36, temp.u32);
	// stfs f8,-32(r11)
	temp.f32 = float(ctx.f8.f64);
	PPC_STORE_U32(ctx.r11.u32 + -32, temp.u32);
	// stfs f13,-28(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + -28, temp.u32);
	// stfs f13,-24(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + -24, temp.u32);
	// stfs f31,-20(r11)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r11.u32 + -20, temp.u32);
	// stfs f30,-16(r11)
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r11.u32 + -16, temp.u32);
	// stfs f7,-8(r11)
	temp.f32 = float(ctx.f7.f64);
	PPC_STORE_U32(ctx.r11.u32 + -8, temp.u32);
	// stfs f0,-4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -4, temp.u32);
	// stfs f7,0(r11)
	temp.f32 = float(ctx.f7.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f6,8(r11)
	temp.f32 = float(ctx.f6.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// stfs f10,12(r11)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// stfs f6,16(r11)
	temp.f32 = float(ctx.f6.f64);
	PPC_STORE_U32(ctx.r11.u32 + 16, temp.u32);
	// stb r19,80(r11)
	PPC_STORE_U8(ctx.r11.u32 + 80, ctx.r19.u8);
	// stfs f0,24(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 24, temp.u32);
	// stb r19,81(r11)
	PPC_STORE_U8(ctx.r11.u32 + 81, ctx.r19.u8);
	// stfs f0,28(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 28, temp.u32);
	// stb r19,82(r11)
	PPC_STORE_U8(ctx.r11.u32 + 82, ctx.r19.u8);
	// stfs f29,32(r11)
	temp.f32 = float(ctx.f29.f64);
	PPC_STORE_U32(ctx.r11.u32 + 32, temp.u32);
	// stfs f28,36(r11)
	temp.f32 = float(ctx.f28.f64);
	PPC_STORE_U32(ctx.r11.u32 + 36, temp.u32);
	// stfs f11,40(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 40, temp.u32);
	// stfs f12,44(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 44, temp.u32);
	// stfs f13,48(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 48, temp.u32);
	// stfs f27,52(r11)
	temp.f32 = float(ctx.f27.f64);
	PPC_STORE_U32(ctx.r11.u32 + 52, temp.u32);
	// stfs f12,56(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 56, temp.u32);
	// stfs f26,60(r11)
	temp.f32 = float(ctx.f26.f64);
	PPC_STORE_U32(ctx.r11.u32 + 60, temp.u32);
	// stfs f5,64(r11)
	temp.f32 = float(ctx.f5.f64);
	PPC_STORE_U32(ctx.r11.u32 + 64, temp.u32);
	// stfs f5,68(r11)
	temp.f32 = float(ctx.f5.f64);
	PPC_STORE_U32(ctx.r11.u32 + 68, temp.u32);
	// stfs f9,72(r11)
	temp.f32 = float(ctx.f9.f64);
	PPC_STORE_U32(ctx.r11.u32 + 72, temp.u32);
	// stfs f0,76(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 76, temp.u32);
	// addi r11,r11,176
	ctx.r11.s64 = ctx.r11.s64 + 176;
	// bge cr6,0x829f1324
	if (!ctx.cr6.lt) goto loc_829F1324;
	// addi r12,r1,-120
	ctx.r12.s64 = ctx.r1.s64 + -120;
	// bl 0x8299018c
	ctx.lr = 0x829F13E4;
	__restfpr_26(ctx, base);
	// b 0x8298eb40
	__restgprlr_18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F13E8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F13E8) { __imp__sub_829F13E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F13E8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// li r10,500
	ctx.r10.s64 = 500;
	// lfs f0,10052(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 10052);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32011
	ctx.r11.s64 = -2097872896;
	// addi r11,r11,-9136
	ctx.r11.s64 = ctx.r11.s64 + -9136;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stw r10,108(r11)
	PPC_STORE_U32(ctx.r11.u32 + 108, ctx.r10.u32);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// lfs f0,10048(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 10048);
	ctx.f0.f64 = double(temp.f32);
	// li r10,1000
	ctx.r10.s64 = 1000;
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stw r10,112(r11)
	PPC_STORE_U32(ctx.r11.u32 + 112, ctx.r10.u32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lfs f0,2592(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 2592);
	ctx.f0.f64 = double(temp.f32);
	// li r10,1000
	ctx.r10.s64 = 1000;
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// stw r10,116(r11)
	PPC_STORE_U32(ctx.r11.u32 + 116, ctx.r10.u32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lfs f13,7956(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 7956);
	ctx.f13.f64 = double(temp.f32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// stfs f13,12(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// lfs f13,3068(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 3068);
	ctx.f13.f64 = double(temp.f32);
	// lis r10,-32255
	ctx.r10.s64 = -2113863680;
	// stfs f13,16(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 16, temp.u32);
	// lfs f12,29496(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 29496);
	ctx.f12.f64 = double(temp.f32);
	// lis r10,-32255
	ctx.r10.s64 = -2113863680;
	// stfs f12,20(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 20, temp.u32);
	// lfs f11,31416(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 31416);
	ctx.f11.f64 = double(temp.f32);
	// lis r10,-32245
	ctx.r10.s64 = -2113208320;
	// stfs f11,24(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 24, temp.u32);
	// stfs f12,28(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 28, temp.u32);
	// lfs f11,-6972(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -6972);
	ctx.f11.f64 = double(temp.f32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// stfs f11,32(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 32, temp.u32);
	// lfs f11,10176(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 10176);
	ctx.f11.f64 = double(temp.f32);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// stfs f11,36(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 36, temp.u32);
	// lfs f11,-23880(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -23880);
	ctx.f11.f64 = double(temp.f32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// stfs f11,40(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 40, temp.u32);
	// stfs f11,44(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 44, temp.u32);
	// stfs f11,48(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 48, temp.u32);
	// stfs f0,52(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 52, temp.u32);
	// stfs f0,56(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 56, temp.u32);
	// stfs f0,60(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 60, temp.u32);
	// lfs f11,2980(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 2980);
	ctx.f11.f64 = double(temp.f32);
	// lis r10,-32245
	ctx.r10.s64 = -2113208320;
	// stfs f11,64(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 64, temp.u32);
	// stfs f0,68(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 68, temp.u32);
	// stfs f0,72(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 72, temp.u32);
	// stfs f0,76(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 76, temp.u32);
	// stfs f0,80(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 80, temp.u32);
	// stfs f0,84(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 84, temp.u32);
	// lfs f0,-6348(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -6348);
	ctx.f0.f64 = double(temp.f32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// stfs f12,88(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 88, temp.u32);
	// stfs f13,92(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 92, temp.u32);
	// stfs f0,96(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 96, temp.u32);
	// lfs f0,10172(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 10172);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,100(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 100, temp.u32);
	// stfs f13,104(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 104, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F14E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F14E0) { __imp__sub_829F14E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F14E0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb04
	ctx.lr = 0x829F14E8;
	__savegprlr_23(ctx, base);
	// stfd f31,-88(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -88, ctx.f31.u64);
	// lis r11,-32011
	ctx.r11.s64 = -2097872896;
	// lis r25,-32255
	ctx.r25.s64 = -2113863680;
	// lis r26,-32255
	ctx.r26.s64 = -2113863680;
	// lis r27,-32245
	ctx.r27.s64 = -2113208320;
	// lis r28,-32255
	ctx.r28.s64 = -2113863680;
	// lis r29,-32245
	ctx.r29.s64 = -2113208320;
	// lis r30,-32245
	ctx.r30.s64 = -2113208320;
	// lfs f31,22800(r25)
	temp.u32 = PPC_LOAD_U32(ctx.r25.u32 + 22800);
	ctx.f31.f64 = double(temp.f32);
	// lis r31,-32256
	ctx.r31.s64 = -2113929216;
	// lfs f1,31412(r26)
	temp.u32 = PPC_LOAD_U32(ctx.r26.u32 + 31412);
	ctx.f1.f64 = double(temp.f32);
	// lis r3,-32256
	ctx.r3.s64 = -2113929216;
	// lfs f2,-6396(r27)
	temp.u32 = PPC_LOAD_U32(ctx.r27.u32 + -6396);
	ctx.f2.f64 = double(temp.f32);
	// lis r4,-32254
	ctx.r4.s64 = -2113798144;
	// lfs f10,29496(r28)
	temp.u32 = PPC_LOAD_U32(ctx.r28.u32 + 29496);
	ctx.f10.f64 = double(temp.f32);
	// lis r5,-32245
	ctx.r5.s64 = -2113208320;
	// lfs f3,-6932(r29)
	temp.u32 = PPC_LOAD_U32(ctx.r29.u32 + -6932);
	ctx.f3.f64 = double(temp.f32);
	// lis r6,-32256
	ctx.r6.s64 = -2113929216;
	// lfs f4,-6940(r30)
	temp.u32 = PPC_LOAD_U32(ctx.r30.u32 + -6940);
	ctx.f4.f64 = double(temp.f32);
	// lis r7,-32245
	ctx.r7.s64 = -2113208320;
	// lfs f11,5180(r31)
	temp.u32 = PPC_LOAD_U32(ctx.r31.u32 + 5180);
	ctx.f11.f64 = double(temp.f32);
	// lis r8,-32252
	ctx.r8.s64 = -2113667072;
	// lfs f13,2980(r3)
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 2980);
	ctx.f13.f64 = double(temp.f32);
	// lis r9,-32254
	ctx.r9.s64 = -2113798144;
	// lfs f5,10048(r4)
	temp.u32 = PPC_LOAD_U32(ctx.r4.u32 + 10048);
	ctx.f5.f64 = double(temp.f32);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// lfs f12,-6348(r5)
	temp.u32 = PPC_LOAD_U32(ctx.r5.u32 + -6348);
	ctx.f12.f64 = double(temp.f32);
	// addi r11,r11,-9008
	ctx.r11.s64 = ctx.r11.s64 + -9008;
	// lfs f0,2592(r6)
	temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + 2592);
	ctx.f0.f64 = double(temp.f32);
	// lfs f6,-6820(r7)
	temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + -6820);
	ctx.f6.f64 = double(temp.f32);
	// li r24,1
	ctx.r24.s64 = 1;
	// lfs f7,10548(r8)
	temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + 10548);
	ctx.f7.f64 = double(temp.f32);
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// lfs f8,-23880(r9)
	temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + -23880);
	ctx.f8.f64 = double(temp.f32);
	// li r23,1500
	ctx.r23.s64 = 1500;
	// lfs f9,-23368(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -23368);
	ctx.f9.f64 = double(temp.f32);
loc_829F1578:
	// addi r24,r24,-1
	ctx.r24.s64 = ctx.r24.s64 + -1;
	// stfs f9,-8(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f9.f64);
	PPC_STORE_U32(ctx.r11.u32 + -8, temp.u32);
	// stfs f8,-4(r11)
	temp.f32 = float(ctx.f8.f64);
	PPC_STORE_U32(ctx.r11.u32 + -4, temp.u32);
	// stw r23,88(r11)
	PPC_STORE_U32(ctx.r11.u32 + 88, ctx.r23.u32);
	// stfs f7,0(r11)
	temp.f32 = float(ctx.f7.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// cmpwi cr6,r24,0
	ctx.cr6.compare<int32_t>(ctx.r24.s32, 0, ctx.xer);
	// stfs f6,4(r11)
	temp.f32 = float(ctx.f6.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// stfs f12,12(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// stfs f5,16(r11)
	temp.f32 = float(ctx.f5.f64);
	PPC_STORE_U32(ctx.r11.u32 + 16, temp.u32);
	// stfs f13,20(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 20, temp.u32);
	// stfs f0,24(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 24, temp.u32);
	// stfs f11,28(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 28, temp.u32);
	// stfs f4,32(r11)
	temp.f32 = float(ctx.f4.f64);
	PPC_STORE_U32(ctx.r11.u32 + 32, temp.u32);
	// stfs f0,36(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 36, temp.u32);
	// stfs f3,40(r11)
	temp.f32 = float(ctx.f3.f64);
	PPC_STORE_U32(ctx.r11.u32 + 40, temp.u32);
	// stfs f0,44(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 44, temp.u32);
	// stfs f12,48(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 48, temp.u32);
	// stfs f10,52(r11)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r11.u32 + 52, temp.u32);
	// stfs f13,56(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 56, temp.u32);
	// stfs f13,60(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 60, temp.u32);
	// stfs f11,64(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 64, temp.u32);
	// stfs f10,68(r11)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r11.u32 + 68, temp.u32);
	// stfs f13,72(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 72, temp.u32);
	// stfs f2,76(r11)
	temp.f32 = float(ctx.f2.f64);
	PPC_STORE_U32(ctx.r11.u32 + 76, temp.u32);
	// stfs f0,80(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 80, temp.u32);
	// stfs f1,84(r11)
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r11.u32 + 84, temp.u32);
	// stfs f31,92(r11)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r11.u32 + 92, temp.u32);
	// addi r11,r11,104
	ctx.r11.s64 = ctx.r11.s64 + 104;
	// bge cr6,0x829f1578
	if (!ctx.cr6.lt) goto loc_829F1578;
	// lfd f31,-88(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -88);
	// b 0x8298eb54
	__restgprlr_23(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F15F8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F15F8) { __imp__sub_829F15F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F15F8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r10,750
	ctx.r10.s64 = 750;
	// lfs f0,10176(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 10176);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32011
	ctx.r11.s64 = -2097872896;
	// addi r11,r11,-8800
	ctx.r11.s64 = ctx.r11.s64 + -8800;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stw r10,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r10.u32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lfs f0,10260(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 10260);
	ctx.f0.f64 = double(temp.f32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stb r10,68(r11)
	PPC_STORE_U8(ctx.r11.u32 + 68, ctx.r10.u8);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lfs f0,8904(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 8904);
	ctx.f0.f64 = double(temp.f32);
	// li r10,1
	ctx.r10.s64 = 1;
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// stb r10,69(r11)
	PPC_STORE_U8(ctx.r11.u32 + 69, ctx.r10.u8);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lfs f0,12408(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 12408);
	ctx.f0.f64 = double(temp.f32);
	// lis r10,-32255
	ctx.r10.s64 = -2113863680;
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// lfs f0,29496(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 29496);
	ctx.f0.f64 = double(temp.f32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// stfs f0,20(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 20, temp.u32);
	// lfs f13,2592(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 2592);
	ctx.f13.f64 = double(temp.f32);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// stfs f13,24(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 24, temp.u32);
	// lfs f12,8136(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 8136);
	ctx.f12.f64 = double(temp.f32);
	// lis r10,-32255
	ctx.r10.s64 = -2113863680;
	// stfs f12,28(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 28, temp.u32);
	// lfs f12,29912(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 29912);
	ctx.f12.f64 = double(temp.f32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// stfs f12,32(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 32, temp.u32);
	// lfs f12,29296(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 29296);
	ctx.f12.f64 = double(temp.f32);
	// lis r10,-32245
	ctx.r10.s64 = -2113208320;
	// stfs f12,36(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 36, temp.u32);
	// lfs f12,-6932(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -6932);
	ctx.f12.f64 = double(temp.f32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// stfs f12,40(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 40, temp.u32);
	// lfs f11,8004(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 8004);
	ctx.f11.f64 = double(temp.f32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// stfs f11,44(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 44, temp.u32);
	// stfs f13,48(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 48, temp.u32);
	// lfs f13,8916(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 8916);
	ctx.f13.f64 = double(temp.f32);
	// stfs f13,52(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 52, temp.u32);
	// stfs f0,56(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 56, temp.u32);
	// stfs f12,60(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 60, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F16B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F16B8) { __imp__sub_829F16B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F16B8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r10,3
	ctx.r10.s64 = 3;
	// lfs f13,12408(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12408);
	ctx.f13.f64 = double(temp.f32);
	// lis r11,-32011
	ctx.r11.s64 = -2097872896;
	// addi r11,r11,-8728
	ctx.r11.s64 = ctx.r11.s64 + -8728;
	// stfs f13,0(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stw r10,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r10.u32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lfs f12,2592(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 2592);
	ctx.f12.f64 = double(temp.f32);
	// li r10,8
	ctx.r10.s64 = 8;
	// stfs f12,4(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stw r10,32(r11)
	PPC_STORE_U32(ctx.r11.u32 + 32, ctx.r10.u32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lfs f11,8004(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 8004);
	ctx.f11.f64 = double(temp.f32);
	// li r10,1000
	ctx.r10.s64 = 1000;
	// stfs f11,8(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// stw r10,36(r11)
	PPC_STORE_U32(ctx.r11.u32 + 36, ctx.r10.u32);
	// lis r10,-32255
	ctx.r10.s64 = -2113863680;
	// lfs f10,-14468(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -14468);
	ctx.f10.f64 = double(temp.f32);
	// li r10,1000
	ctx.r10.s64 = 1000;
	// stfs f10,12(r11)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// stw r10,40(r11)
	PPC_STORE_U32(ctx.r11.u32 + 40, ctx.r10.u32);
	// lis r10,-32255
	ctx.r10.s64 = -2113863680;
	// lfs f0,29912(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 29912);
	ctx.f0.f64 = double(temp.f32);
	// li r10,1
	ctx.r10.s64 = 1;
	// stfs f0,16(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 16, temp.u32);
	// stw r10,48(r11)
	PPC_STORE_U32(ctx.r11.u32 + 48, ctx.r10.u32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stfs f0,20(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 20, temp.u32);
	// stb r10,52(r11)
	PPC_STORE_U8(ctx.r11.u32 + 52, ctx.r10.u8);
	// li r10,3
	ctx.r10.s64 = 3;
	// stfs f0,24(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 24, temp.u32);
	// stw r10,84(r11)
	PPC_STORE_U32(ctx.r11.u32 + 84, ctx.r10.u32);
	// li r10,8
	ctx.r10.s64 = 8;
	// stfs f0,44(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 44, temp.u32);
	// stw r10,88(r11)
	PPC_STORE_U32(ctx.r11.u32 + 88, ctx.r10.u32);
	// li r10,1000
	ctx.r10.s64 = 1000;
	// stfs f13,56(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 56, temp.u32);
	// stw r10,92(r11)
	PPC_STORE_U32(ctx.r11.u32 + 92, ctx.r10.u32);
	// stfs f12,60(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 60, temp.u32);
	// stw r10,96(r11)
	PPC_STORE_U32(ctx.r11.u32 + 96, ctx.r10.u32);
	// li r10,1
	ctx.r10.s64 = 1;
	// stfs f11,64(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 64, temp.u32);
	// stw r10,104(r11)
	PPC_STORE_U32(ctx.r11.u32 + 104, ctx.r10.u32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stfs f10,68(r11)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r11.u32 + 68, temp.u32);
	// stb r10,108(r11)
	PPC_STORE_U8(ctx.r11.u32 + 108, ctx.r10.u8);
	// li r10,3
	ctx.r10.s64 = 3;
	// stfs f0,72(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 72, temp.u32);
	// stw r10,140(r11)
	PPC_STORE_U32(ctx.r11.u32 + 140, ctx.r10.u32);
	// li r10,8
	ctx.r10.s64 = 8;
	// stfs f0,76(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 76, temp.u32);
	// stw r10,144(r11)
	PPC_STORE_U32(ctx.r11.u32 + 144, ctx.r10.u32);
	// li r10,1000
	ctx.r10.s64 = 1000;
	// stfs f0,80(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 80, temp.u32);
	// stw r10,148(r11)
	PPC_STORE_U32(ctx.r11.u32 + 148, ctx.r10.u32);
	// stfs f0,100(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 100, temp.u32);
	// stw r10,152(r11)
	PPC_STORE_U32(ctx.r11.u32 + 152, ctx.r10.u32);
	// li r10,1
	ctx.r10.s64 = 1;
	// stfs f13,112(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 112, temp.u32);
	// stw r10,160(r11)
	PPC_STORE_U32(ctx.r11.u32 + 160, ctx.r10.u32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stfs f12,116(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 116, temp.u32);
	// stb r10,164(r11)
	PPC_STORE_U8(ctx.r11.u32 + 164, ctx.r10.u8);
	// li r10,3
	ctx.r10.s64 = 3;
	// stfs f11,120(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 120, temp.u32);
	// stw r10,196(r11)
	PPC_STORE_U32(ctx.r11.u32 + 196, ctx.r10.u32);
	// li r10,8
	ctx.r10.s64 = 8;
	// stfs f10,124(r11)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r11.u32 + 124, temp.u32);
	// stw r10,200(r11)
	PPC_STORE_U32(ctx.r11.u32 + 200, ctx.r10.u32);
	// li r10,1000
	ctx.r10.s64 = 1000;
	// stfs f0,128(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 128, temp.u32);
	// stw r10,204(r11)
	PPC_STORE_U32(ctx.r11.u32 + 204, ctx.r10.u32);
	// stfs f0,132(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 132, temp.u32);
	// stw r10,208(r11)
	PPC_STORE_U32(ctx.r11.u32 + 208, ctx.r10.u32);
	// li r10,1
	ctx.r10.s64 = 1;
	// stfs f0,136(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 136, temp.u32);
	// stw r10,216(r11)
	PPC_STORE_U32(ctx.r11.u32 + 216, ctx.r10.u32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stfs f0,156(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 156, temp.u32);
	// stb r10,220(r11)
	PPC_STORE_U8(ctx.r11.u32 + 220, ctx.r10.u8);
	// li r10,3
	ctx.r10.s64 = 3;
	// stfs f13,168(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 168, temp.u32);
	// stw r10,252(r11)
	PPC_STORE_U32(ctx.r11.u32 + 252, ctx.r10.u32);
	// li r10,8
	ctx.r10.s64 = 8;
	// stfs f12,172(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 172, temp.u32);
	// stw r10,256(r11)
	PPC_STORE_U32(ctx.r11.u32 + 256, ctx.r10.u32);
	// li r10,1000
	ctx.r10.s64 = 1000;
	// stfs f11,176(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 176, temp.u32);
	// stw r10,260(r11)
	PPC_STORE_U32(ctx.r11.u32 + 260, ctx.r10.u32);
	// stfs f10,180(r11)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r11.u32 + 180, temp.u32);
	// stw r10,264(r11)
	PPC_STORE_U32(ctx.r11.u32 + 264, ctx.r10.u32);
	// li r10,1
	ctx.r10.s64 = 1;
	// stfs f0,184(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 184, temp.u32);
	// stw r10,272(r11)
	PPC_STORE_U32(ctx.r11.u32 + 272, ctx.r10.u32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stfs f0,188(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 188, temp.u32);
	// stb r10,276(r11)
	PPC_STORE_U8(ctx.r11.u32 + 276, ctx.r10.u8);
	// li r10,3
	ctx.r10.s64 = 3;
	// stfs f0,192(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 192, temp.u32);
	// stw r10,308(r11)
	PPC_STORE_U32(ctx.r11.u32 + 308, ctx.r10.u32);
	// li r10,8
	ctx.r10.s64 = 8;
	// stfs f0,212(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 212, temp.u32);
	// stw r10,312(r11)
	PPC_STORE_U32(ctx.r11.u32 + 312, ctx.r10.u32);
	// li r10,1000
	ctx.r10.s64 = 1000;
	// stfs f13,224(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 224, temp.u32);
	// stw r10,316(r11)
	PPC_STORE_U32(ctx.r11.u32 + 316, ctx.r10.u32);
	// stfs f12,228(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 228, temp.u32);
	// stw r10,320(r11)
	PPC_STORE_U32(ctx.r11.u32 + 320, ctx.r10.u32);
	// stfs f11,232(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 232, temp.u32);
	// stfs f10,236(r11)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r11.u32 + 236, temp.u32);
	// li r10,1
	ctx.r10.s64 = 1;
	// stfs f0,240(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 240, temp.u32);
	// stfs f0,244(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 244, temp.u32);
	// stfs f0,248(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 248, temp.u32);
	// stfs f0,268(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 268, temp.u32);
	// stfs f13,280(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 280, temp.u32);
	// stfs f12,284(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 284, temp.u32);
	// stfs f11,288(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 288, temp.u32);
	// stfs f10,292(r11)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r11.u32 + 292, temp.u32);
	// stfs f0,296(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 296, temp.u32);
	// stfs f0,300(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 300, temp.u32);
	// stfs f0,304(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 304, temp.u32);
	// stfs f0,324(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 324, temp.u32);
	// stw r10,328(r11)
	PPC_STORE_U32(ctx.r11.u32 + 328, ctx.r10.u32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stb r10,332(r11)
	PPC_STORE_U8(ctx.r11.u32 + 332, ctx.r10.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F18B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F18B8) { __imp__sub_829F18B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F18B8) {
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
	// lis r11,-32011
	ctx.r11.s64 = -2097872896;
	// addi r3,r11,-8388
	ctx.r3.s64 = ctx.r11.s64 + -8388;
	// bl 0x8289df08
	ctx.lr = 0x829F18D0;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6136
	ctx.r3.s64 = ctx.r11.s64 + -6136;
	// bl 0x8298ed98
	ctx.lr = 0x829F18DC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F18F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F18F0) { __imp__sub_829F18F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F18F0) {
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
	// lis r11,-32011
	ctx.r11.s64 = -2097872896;
	// addi r3,r11,-8348
	ctx.r3.s64 = ctx.r11.s64 + -8348;
	// bl 0x8289df08
	ctx.lr = 0x829F1908;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6144
	ctx.r3.s64 = ctx.r11.s64 + -6144;
	// bl 0x8298ed98
	ctx.lr = 0x829F1914;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F1928);  /* forward decl */
PPC_WEAK_FUNC(sub_829F1928) { __imp__sub_829F1928(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F1928) {
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
	// lis r11,-32011
	ctx.r11.s64 = -2097872896;
	// addi r3,r11,-8308
	ctx.r3.s64 = ctx.r11.s64 + -8308;
	// bl 0x8289df08
	ctx.lr = 0x829F1940;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6152
	ctx.r3.s64 = ctx.r11.s64 + -6152;
	// bl 0x8298ed98
	ctx.lr = 0x829F194C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F1960);  /* forward decl */
PPC_WEAK_FUNC(sub_829F1960) { __imp__sub_829F1960(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F1960) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6128
	ctx.r3.s64 = ctx.r11.s64 + -6128;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F1970);  /* forward decl */
PPC_WEAK_FUNC(sub_829F1970) { __imp__sub_829F1970(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F1970) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6112
	ctx.r3.s64 = ctx.r11.s64 + -6112;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F1980);  /* forward decl */
PPC_WEAK_FUNC(sub_829F1980) { __imp__sub_829F1980(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F1980) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6096
	ctx.r3.s64 = ctx.r11.s64 + -6096;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F1990);  /* forward decl */
PPC_WEAK_FUNC(sub_829F1990) { __imp__sub_829F1990(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F1990) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6064
	ctx.r3.s64 = ctx.r11.s64 + -6064;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F19A0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F19A0) { __imp__sub_829F19A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F19A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6032
	ctx.r3.s64 = ctx.r11.s64 + -6032;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F19B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F19B0) { __imp__sub_829F19B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F19B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6000
	ctx.r3.s64 = ctx.r11.s64 + -6000;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F19C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F19C0) { __imp__sub_829F19C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F19C0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// lfs f0,7384(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 7384);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32011
	ctx.r11.s64 = -2097872896;
	// stfs f0,27424(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 27424, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F19D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F19D8) { __imp__sub_829F19D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F19D8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32011
	ctx.r11.s64 = -2097872896;
	// lfs f0,27424(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 27424);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32092
	ctx.r11.s64 = -2103181312;
	// addi r11,r11,-32096
	ctx.r11.s64 = ctx.r11.s64 + -32096;
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F19F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F19F0) { __imp__sub_829F19F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F19F0) {
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
	// addi r3,r11,16600
	ctx.r3.s64 = ctx.r11.s64 + 16600;
	// bl 0x827df490
	ctx.lr = 0x829F1A0C;
	sub_827DF490(ctx, base);
	// lis r11,-32011
	ctx.r11.s64 = -2097872896;
	// stw r3,27428(r11)
	PPC_STORE_U32(ctx.r11.u32 + 27428, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F1A28);  /* forward decl */
PPC_WEAK_FUNC(sub_829F1A28) { __imp__sub_829F1A28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F1A28) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,16636
	ctx.r5.s64 = ctx.r11.s64 + 16636;
	// lis r11,-32011
	ctx.r11.s64 = -2097872896;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,27432
	ctx.r3.s64 = ctx.r11.s64 + 27432;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F1A48);  /* forward decl */
PPC_WEAK_FUNC(sub_829F1A48) { __imp__sub_829F1A48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F1A48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,16656
	ctx.r5.s64 = ctx.r11.s64 + 16656;
	// lis r11,-32011
	ctx.r11.s64 = -2097872896;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,27452
	ctx.r3.s64 = ctx.r11.s64 + 27452;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F1A68);  /* forward decl */
PPC_WEAK_FUNC(sub_829F1A68) { __imp__sub_829F1A68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F1A68) {
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
	// lis r11,-32011
	ctx.r11.s64 = -2097872896;
	// li r31,149
	ctx.r31.s64 = 149;
	// addi r30,r11,27472
	ctx.r30.s64 = ctx.r11.s64 + 27472;
loc_829F1A88:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x824ea5e0
	ctx.lr = 0x829F1A90;
	sub_824EA5E0(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,640
	ctx.r30.s64 = ctx.r30.s64 + 640;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x829f1a88
	if (!ctx.cr6.lt) goto loc_829F1A88;
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

PPC_FUNC_IMPL(__imp__sub_829F1AB8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F1AB8) { __imp__sub_829F1AB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F1AB8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// lis r10,-32009
	ctx.r10.s64 = -2097741824;
	// addi r11,r11,-13784
	ctx.r11.s64 = ctx.r11.s64 + -13784;
	// lwz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// stw r11,-7600(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7600, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F1AD0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F1AD0) { __imp__sub_829F1AD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F1AD0) {
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
	// addi r3,r11,17548
	ctx.r3.s64 = ctx.r11.s64 + 17548;
	// bl 0x827df490
	ctx.lr = 0x829F1AEC;
	sub_827DF490(ctx, base);
	// lis r11,-32009
	ctx.r11.s64 = -2097741824;
	// stw r3,-7560(r11)
	PPC_STORE_U32(ctx.r11.u32 + -7560, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F1B08);  /* forward decl */
PPC_WEAK_FUNC(sub_829F1B08) { __imp__sub_829F1B08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F1B08) {
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
	// lis r11,-32009
	ctx.r11.s64 = -2097741824;
	// li r31,4
	ctx.r31.s64 = 4;
	// addi r30,r11,-7552
	ctx.r30.s64 = ctx.r11.s64 + -7552;
loc_829F1B28:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82328070
	ctx.lr = 0x829F1B30;
	sub_82328070(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,160
	ctx.r30.s64 = ctx.r30.s64 + 160;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x829f1b28
	if (!ctx.cr6.lt) goto loc_829F1B28;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5968
	ctx.r3.s64 = ctx.r11.s64 + -5968;
	// bl 0x8298ed98
	ctx.lr = 0x829F1B4C;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F1B68);  /* forward decl */
PPC_WEAK_FUNC(sub_829F1B68) { __imp__sub_829F1B68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F1B68) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// lfs f0,7384(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 7384);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32092
	ctx.r11.s64 = -2103181312;
	// addi r11,r11,-31856
	ctx.r11.s64 = ctx.r11.s64 + -31856;
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F1B80);  /* forward decl */
PPC_WEAK_FUNC(sub_829F1B80) { __imp__sub_829F1B80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F1B80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,19044
	ctx.r5.s64 = ctx.r11.s64 + 19044;
	// lis r11,-32009
	ctx.r11.s64 = -2097741824;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-6592
	ctx.r3.s64 = ctx.r11.s64 + -6592;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F1BA0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F1BA0) { __imp__sub_829F1BA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F1BA0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x829F1BA8;
	__savegprlr_27(ctx, base);
	// stfd f31,-56(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -56, ctx.f31.u64);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32009
	ctx.r11.s64 = -2097741824;
	// li r30,39
	ctx.r30.s64 = 39;
	// addi r11,r11,-6560
	ctx.r11.s64 = ctx.r11.s64 + -6560;
	// li r27,-1
	ctx.r27.s64 = -1;
	// addi r31,r11,484
	ctx.r31.s64 = ctx.r11.s64 + 484;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r28,0
	ctx.r28.s64 = 0;
	// li r29,1
	ctx.r29.s64 = 1;
	// lfs f31,3068(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3068);
	ctx.f31.f64 = double(temp.f32);
loc_829F1BD4:
	// lis r11,-32176
	ctx.r11.s64 = -2108686336;
	// li r5,10
	ctx.r5.s64 = 10;
	// addi r6,r11,-26336
	ctx.r6.s64 = ctx.r11.s64 + -26336;
	// li r4,48
	ctx.r4.s64 = 48;
	// addi r3,r31,-484
	ctx.r3.s64 = ctx.r31.s64 + -484;
	// bl 0x82120220
	ctx.lr = 0x829F1BEC;
	sub_82120220(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// stfs f31,4(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r31.u32 + 4, temp.u32);
	// stw r27,-4(r31)
	PPC_STORE_U32(ctx.r31.u32 + -4, ctx.r27.u32);
	// stfs f31,8(r31)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r31.u32 + 8, temp.u32);
	// stw r28,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r28.u32);
	// stb r29,12(r31)
	PPC_STORE_U8(ctx.r31.u32 + 12, ctx.r29.u8);
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// addi r31,r31,512
	ctx.r31.s64 = ctx.r31.s64 + 512;
	// bge cr6,0x829f1bd4
	if (!ctx.cr6.lt) goto loc_829F1BD4;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5960
	ctx.r3.s64 = ctx.r11.s64 + -5960;
	// bl 0x8298ed98
	ctx.lr = 0x829F1C1C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lfd f31,-56(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -56);
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F1C28);  /* forward decl */
PPC_WEAK_FUNC(sub_829F1C28) { __imp__sub_829F1C28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F1C28) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-32009
	ctx.r11.s64 = -2097741824;
	// vspltisw v0,0
	simde_mm_store_si128((simde__m128i*)ctx.v0.u32, simde_mm_set1_epi32(int(0x0)));
	// li r9,78
	ctx.r9.s64 = 78;
	// addi r11,r11,13920
	ctx.r11.s64 = ctx.r11.s64 + 13920;
	// addi r10,r11,16
	ctx.r10.s64 = ctx.r11.s64 + 16;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// mtctr r9
	ctx.ctr.u64 = ctx.r9.u64;
loc_829F1C44:
	// ld r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U64(ctx.r11.u32 + 0);
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// std r9,0(r10)
	PPC_STORE_U64(ctx.r10.u32 + 0, ctx.r9.u64);
	// addi r10,r10,8
	ctx.r10.s64 = ctx.r10.s64 + 8;
	// bdnz 0x829f1c44
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_829F1C44;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5952
	ctx.r3.s64 = ctx.r11.s64 + -5952;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F1C68);  /* forward decl */
PPC_WEAK_FUNC(sub_829F1C68) { __imp__sub_829F1C68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F1C68) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r10,r11,10952
	ctx.r10.s64 = ctx.r11.s64 + 10952;
	// lis r11,-32092
	ctx.r11.s64 = -2103181312;
	// addi r11,r11,-29248
	ctx.r11.s64 = ctx.r11.s64 + -29248;
	// lfs f0,12(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 12);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,16(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 16);
	ctx.f13.f64 = double(temp.f32);
	// stfs f0,28(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 28, temp.u32);
	// stfs f13,32(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 32, temp.u32);
	// stfs f0,68(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 68, temp.u32);
	// stfs f13,72(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 72, temp.u32);
	// stfs f0,108(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 108, temp.u32);
	// stfs f13,112(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 112, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F1CA0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F1CA0) { __imp__sub_829F1CA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F1CA0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32009
	ctx.r11.s64 = -2097741824;
	// lis r7,-32256
	ctx.r7.s64 = -2113929216;
	// lis r8,-32256
	ctx.r8.s64 = -2113929216;
	// addi r11,r11,25792
	ctx.r11.s64 = ctx.r11.s64 + 25792;
	// li r9,116
	ctx.r9.s64 = 116;
	// addi r11,r11,24
	ctx.r11.s64 = ctx.r11.s64 + 24;
	// lfs f13,2592(r7)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + 2592);
	ctx.f13.f64 = double(temp.f32);
	// li r10,-1
	ctx.r10.s64 = -1;
	// lfs f0,3068(r8)
	temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + 3068);
	ctx.f0.f64 = double(temp.f32);
	// li r4,0
	ctx.r4.s64 = 0;
	// li r5,1
	ctx.r5.s64 = 1;
	// li r6,1000
	ctx.r6.s64 = 1000;
loc_829F1CD0:
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stfs f0,-8(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -8, temp.u32);
	// stfs f0,-24(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -24, temp.u32);
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// stfs f0,-20(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -20, temp.u32);
	// stw r10,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r10.u32);
	// stfs f13,-16(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + -16, temp.u32);
	// stb r4,20(r11)
	PPC_STORE_U8(ctx.r11.u32 + 20, ctx.r4.u8);
	// stfs f0,-12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -12, temp.u32);
	// stw r10,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
	// stw r5,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r5.u32);
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// stw r6,-4(r11)
	PPC_STORE_U32(ctx.r11.u32 + -4, ctx.r6.u32);
	// addi r11,r11,48
	ctx.r11.s64 = ctx.r11.s64 + 48;
	// bge cr6,0x829f1cd0
	if (!ctx.cr6.lt) goto loc_829F1CD0;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5944
	ctx.r3.s64 = ctx.r11.s64 + -5944;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F1D18);  /* forward decl */
PPC_WEAK_FUNC(sub_829F1D18) { __imp__sub_829F1D18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F1D18) {
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
	// lis r11,-32009
	ctx.r11.s64 = -2097741824;
	// addi r3,r11,31456
	ctx.r3.s64 = ctx.r11.s64 + 31456;
	// bl 0x82507698
	ctx.lr = 0x829F1D30;
	sub_82507698(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5936
	ctx.r3.s64 = ctx.r11.s64 + -5936;
	// bl 0x8298ed98
	ctx.lr = 0x829F1D3C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F1D50);  /* forward decl */
PPC_WEAK_FUNC(sub_829F1D50) { __imp__sub_829F1D50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F1D50) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5856
	ctx.r3.s64 = ctx.r11.s64 + -5856;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F1D60);  /* forward decl */
PPC_WEAK_FUNC(sub_829F1D60) { __imp__sub_829F1D60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F1D60) {
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
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// addi r31,r11,-29816
	ctx.r31.s64 = ctx.r11.s64 + -29816;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82893a58
	ctx.lr = 0x829F1D80;
	sub_82893A58(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,21384
	ctx.r11.s64 = ctx.r11.s64 + 21384;
	// addi r3,r10,-5824
	ctx.r3.s64 = ctx.r10.s64 + -5824;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r11.u32);
	// stw r11,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r11.u32);
	// stw r11,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F1DA8;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F1DC0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F1DC0) { __imp__sub_829F1DC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F1DC0) {
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
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,22464
	ctx.r3.s64 = ctx.r11.s64 + 22464;
	// bl 0x827df490
	ctx.lr = 0x829F1DE0;
	sub_827DF490(ctx, base);
	// lis r10,-32007
	ctx.r10.s64 = -2097610752;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// addi r31,r10,-29792
	ctx.r31.s64 = ctx.r10.s64 + -29792;
	// addi r11,r11,22432
	ctx.r11.s64 = ctx.r11.s64 + 22432;
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F1E00;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,22400
	ctx.r11.s64 = ctx.r11.s64 + 22400;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F1E18;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,22368
	ctx.r11.s64 = ctx.r11.s64 + 22368;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F1E30;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,22336
	ctx.r11.s64 = ctx.r11.s64 + 22336;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F1E48;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,22304
	ctx.r11.s64 = ctx.r11.s64 + 22304;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F1E60;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,20(r31)
	PPC_STORE_U32(ctx.r31.u32 + 20, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,22272
	ctx.r11.s64 = ctx.r11.s64 + 22272;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F1E78;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,24(r31)
	PPC_STORE_U32(ctx.r31.u32 + 24, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,22240
	ctx.r11.s64 = ctx.r11.s64 + 22240;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F1E90;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,28(r31)
	PPC_STORE_U32(ctx.r31.u32 + 28, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,22208
	ctx.r11.s64 = ctx.r11.s64 + 22208;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F1EA8;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,32(r31)
	PPC_STORE_U32(ctx.r31.u32 + 32, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,22176
	ctx.r11.s64 = ctx.r11.s64 + 22176;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F1EC0;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,36(r31)
	PPC_STORE_U32(ctx.r31.u32 + 36, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,22144
	ctx.r11.s64 = ctx.r11.s64 + 22144;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F1ED8;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,22112
	ctx.r11.s64 = ctx.r11.s64 + 22112;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F1EF0;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,44(r31)
	PPC_STORE_U32(ctx.r31.u32 + 44, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,22080
	ctx.r11.s64 = ctx.r11.s64 + 22080;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F1F08;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,48(r31)
	PPC_STORE_U32(ctx.r31.u32 + 48, ctx.r3.u32);
	// addi r11,r11,22048
	ctx.r11.s64 = ctx.r11.s64 + 22048;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F1F20;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,52(r31)
	PPC_STORE_U32(ctx.r31.u32 + 52, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,22016
	ctx.r11.s64 = ctx.r11.s64 + 22016;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F1F38;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,56(r31)
	PPC_STORE_U32(ctx.r31.u32 + 56, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,21984
	ctx.r11.s64 = ctx.r11.s64 + 21984;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F1F50;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,60(r31)
	PPC_STORE_U32(ctx.r31.u32 + 60, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,21952
	ctx.r3.s64 = ctx.r11.s64 + 21952;
	// bl 0x827df490
	ctx.lr = 0x829F1F64;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,64(r31)
	PPC_STORE_U32(ctx.r31.u32 + 64, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,21920
	ctx.r11.s64 = ctx.r11.s64 + 21920;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F1F7C;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,68(r31)
	PPC_STORE_U32(ctx.r31.u32 + 68, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,21888
	ctx.r11.s64 = ctx.r11.s64 + 21888;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F1F94;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,72(r31)
	PPC_STORE_U32(ctx.r31.u32 + 72, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,21852
	ctx.r11.s64 = ctx.r11.s64 + 21852;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F1FAC;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,76(r31)
	PPC_STORE_U32(ctx.r31.u32 + 76, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,21820
	ctx.r11.s64 = ctx.r11.s64 + 21820;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F1FC4;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,80(r31)
	PPC_STORE_U32(ctx.r31.u32 + 80, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,21784
	ctx.r11.s64 = ctx.r11.s64 + 21784;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F1FDC;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,84(r31)
	PPC_STORE_U32(ctx.r31.u32 + 84, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,21752
	ctx.r11.s64 = ctx.r11.s64 + 21752;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F1FF4;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,88(r31)
	PPC_STORE_U32(ctx.r31.u32 + 88, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,21716
	ctx.r11.s64 = ctx.r11.s64 + 21716;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F200C;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,92(r31)
	PPC_STORE_U32(ctx.r31.u32 + 92, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,21684
	ctx.r11.s64 = ctx.r11.s64 + 21684;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F2024;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,96(r31)
	PPC_STORE_U32(ctx.r31.u32 + 96, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,21648
	ctx.r11.s64 = ctx.r11.s64 + 21648;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F203C;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,100(r31)
	PPC_STORE_U32(ctx.r31.u32 + 100, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,21616
	ctx.r11.s64 = ctx.r11.s64 + 21616;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F2054;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// addi r11,r11,21580
	ctx.r11.s64 = ctx.r11.s64 + 21580;
	// stw r3,104(r31)
	PPC_STORE_U32(ctx.r31.u32 + 104, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F206C;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,108(r31)
	PPC_STORE_U32(ctx.r31.u32 + 108, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,21548
	ctx.r11.s64 = ctx.r11.s64 + 21548;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F2084;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,112(r31)
	PPC_STORE_U32(ctx.r31.u32 + 112, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,21512
	ctx.r11.s64 = ctx.r11.s64 + 21512;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F209C;
	sub_827DF490(ctx, base);
	// stw r3,116(r31)
	PPC_STORE_U32(ctx.r31.u32 + 116, ctx.r3.u32);
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

PPC_FUNC_IMPL(__imp__sub_829F20B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F20B8) { __imp__sub_829F20B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F20B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5800
	ctx.r3.s64 = ctx.r11.s64 + -5800;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F20C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F20C8) { __imp__sub_829F20C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F20C8) {
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
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-10800
	ctx.r3.s64 = ctx.r11.s64 + -10800;
	// bl 0x827df490
	ctx.lr = 0x829F20E4;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6864(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6864, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2100);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2100) { __imp__sub_829F2100(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2100) {
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
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,22536
	ctx.r3.s64 = ctx.r11.s64 + 22536;
	// bl 0x827df490
	ctx.lr = 0x829F211C;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6860(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6860, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2138);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2138) { __imp__sub_829F2138(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2138) {
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
	// addi r3,r11,23988
	ctx.r3.s64 = ctx.r11.s64 + 23988;
	// bl 0x827df490
	ctx.lr = 0x829F2154;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6856(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6856, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2170);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2170) { __imp__sub_829F2170(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2170) {
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
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-11320
	ctx.r3.s64 = ctx.r11.s64 + -11320;
	// bl 0x827df490
	ctx.lr = 0x829F218C;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6852(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6852, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F21A8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F21A8) { __imp__sub_829F21A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F21A8) {
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
	// addi r3,r11,23996
	ctx.r3.s64 = ctx.r11.s64 + 23996;
	// bl 0x827df490
	ctx.lr = 0x829F21C4;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6848(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6848, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F21E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F21E0) { __imp__sub_829F21E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F21E0) {
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
	// addi r3,r11,24004
	ctx.r3.s64 = ctx.r11.s64 + 24004;
	// bl 0x827df490
	ctx.lr = 0x829F21FC;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6844(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6844, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2218);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2218) { __imp__sub_829F2218(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2218) {
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
	// addi r3,r11,24012
	ctx.r3.s64 = ctx.r11.s64 + 24012;
	// bl 0x827df490
	ctx.lr = 0x829F2234;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6840(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6840, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2250);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2250) { __imp__sub_829F2250(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2250) {
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
	// addi r3,r11,24028
	ctx.r3.s64 = ctx.r11.s64 + 24028;
	// bl 0x827df490
	ctx.lr = 0x829F226C;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6836(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6836, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2288);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2288) { __imp__sub_829F2288(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2288) {
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
	// addi r3,r11,24040
	ctx.r3.s64 = ctx.r11.s64 + 24040;
	// bl 0x827df490
	ctx.lr = 0x829F22A4;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6832(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6832, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F22C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F22C0) { __imp__sub_829F22C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F22C0) {
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
	// addi r3,r11,24048
	ctx.r3.s64 = ctx.r11.s64 + 24048;
	// bl 0x827df490
	ctx.lr = 0x829F22DC;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6828(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6828, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F22F8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F22F8) { __imp__sub_829F22F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F22F8) {
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
	// addi r3,r11,24056
	ctx.r3.s64 = ctx.r11.s64 + 24056;
	// bl 0x827df490
	ctx.lr = 0x829F2314;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6824(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6824, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2330);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2330) { __imp__sub_829F2330(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2330) {
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
	// addi r3,r11,24068
	ctx.r3.s64 = ctx.r11.s64 + 24068;
	// bl 0x827df490
	ctx.lr = 0x829F234C;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6820(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6820, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2368);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2368) { __imp__sub_829F2368(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2368) {
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
	// addi r3,r11,24080
	ctx.r3.s64 = ctx.r11.s64 + 24080;
	// bl 0x827df490
	ctx.lr = 0x829F2384;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6816(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6816, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F23A0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F23A0) { __imp__sub_829F23A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F23A0) {
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
	// addi r3,r11,24092
	ctx.r3.s64 = ctx.r11.s64 + 24092;
	// bl 0x827df490
	ctx.lr = 0x829F23BC;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6812(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6812, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F23D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F23D8) { __imp__sub_829F23D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F23D8) {
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
	// addi r3,r11,24104
	ctx.r3.s64 = ctx.r11.s64 + 24104;
	// bl 0x827df490
	ctx.lr = 0x829F23F4;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6808(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6808, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2410);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2410) { __imp__sub_829F2410(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2410) {
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
	// addi r3,r11,24120
	ctx.r3.s64 = ctx.r11.s64 + 24120;
	// bl 0x827df490
	ctx.lr = 0x829F242C;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6804(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6804, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2448);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2448) { __imp__sub_829F2448(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2448) {
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
	// addi r3,r11,24136
	ctx.r3.s64 = ctx.r11.s64 + 24136;
	// bl 0x827df490
	ctx.lr = 0x829F2464;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6800(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6800, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2480);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2480) { __imp__sub_829F2480(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2480) {
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
	// addi r3,r11,24156
	ctx.r3.s64 = ctx.r11.s64 + 24156;
	// bl 0x827df490
	ctx.lr = 0x829F249C;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6796(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6796, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F24B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F24B8) { __imp__sub_829F24B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F24B8) {
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
	// addi r3,r11,24180
	ctx.r3.s64 = ctx.r11.s64 + 24180;
	// bl 0x827df490
	ctx.lr = 0x829F24D4;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6792(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6792, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F24F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F24F0) { __imp__sub_829F24F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F24F0) {
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
	// addi r3,r11,24208
	ctx.r3.s64 = ctx.r11.s64 + 24208;
	// bl 0x827df490
	ctx.lr = 0x829F250C;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6788(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6788, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2528);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2528) { __imp__sub_829F2528(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2528) {
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
	// addi r3,r11,24240
	ctx.r3.s64 = ctx.r11.s64 + 24240;
	// bl 0x827df490
	ctx.lr = 0x829F2544;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6784(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6784, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2560);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2560) { __imp__sub_829F2560(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2560) {
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
	// addi r3,r11,24260
	ctx.r3.s64 = ctx.r11.s64 + 24260;
	// bl 0x827df490
	ctx.lr = 0x829F257C;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6780(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6780, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2598);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2598) { __imp__sub_829F2598(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2598) {
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
	// addi r3,r11,24288
	ctx.r3.s64 = ctx.r11.s64 + 24288;
	// bl 0x827df490
	ctx.lr = 0x829F25B4;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6776(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6776, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F25D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F25D0) { __imp__sub_829F25D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F25D0) {
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
	// addi r3,r11,24308
	ctx.r3.s64 = ctx.r11.s64 + 24308;
	// bl 0x827df490
	ctx.lr = 0x829F25EC;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6772(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6772, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2608);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2608) { __imp__sub_829F2608(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2608) {
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
	// addi r3,r11,24336
	ctx.r3.s64 = ctx.r11.s64 + 24336;
	// bl 0x827df490
	ctx.lr = 0x829F2624;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6768(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6768, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2640);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2640) { __imp__sub_829F2640(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2640) {
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
	// addi r3,r11,24360
	ctx.r3.s64 = ctx.r11.s64 + 24360;
	// bl 0x827df490
	ctx.lr = 0x829F265C;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6764(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6764, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2678);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2678) { __imp__sub_829F2678(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2678) {
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
	// addi r3,r11,24388
	ctx.r3.s64 = ctx.r11.s64 + 24388;
	// bl 0x827df490
	ctx.lr = 0x829F2694;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6760(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6760, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F26B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F26B0) { __imp__sub_829F26B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F26B0) {
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
	// addi r3,r11,24420
	ctx.r3.s64 = ctx.r11.s64 + 24420;
	// bl 0x827df490
	ctx.lr = 0x829F26CC;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6756(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6756, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F26E8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F26E8) { __imp__sub_829F26E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F26E8) {
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
	// addi r3,r11,24456
	ctx.r3.s64 = ctx.r11.s64 + 24456;
	// bl 0x827df490
	ctx.lr = 0x829F2704;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6752(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6752, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2720);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2720) { __imp__sub_829F2720(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2720) {
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
	// addi r3,r11,24480
	ctx.r3.s64 = ctx.r11.s64 + 24480;
	// bl 0x827df490
	ctx.lr = 0x829F273C;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6748(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6748, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2758);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2758) { __imp__sub_829F2758(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2758) {
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
	// addi r3,r11,24508
	ctx.r3.s64 = ctx.r11.s64 + 24508;
	// bl 0x827df490
	ctx.lr = 0x829F2774;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6744(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6744, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2790);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2790) { __imp__sub_829F2790(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2790) {
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
	// addi r3,r11,24540
	ctx.r3.s64 = ctx.r11.s64 + 24540;
	// bl 0x827df490
	ctx.lr = 0x829F27AC;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6740(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6740, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F27C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F27C8) { __imp__sub_829F27C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F27C8) {
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
	// addi r3,r11,24576
	ctx.r3.s64 = ctx.r11.s64 + 24576;
	// bl 0x827df490
	ctx.lr = 0x829F27E4;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6736(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6736, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2800);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2800) { __imp__sub_829F2800(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2800) {
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
	// addi r3,r11,24608
	ctx.r3.s64 = ctx.r11.s64 + 24608;
	// bl 0x827df490
	ctx.lr = 0x829F281C;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6732(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6732, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2838);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2838) { __imp__sub_829F2838(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2838) {
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
	// addi r3,r11,24644
	ctx.r3.s64 = ctx.r11.s64 + 24644;
	// bl 0x827df490
	ctx.lr = 0x829F2854;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6728(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6728, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2870);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2870) { __imp__sub_829F2870(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2870) {
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
	// addi r3,r11,24684
	ctx.r3.s64 = ctx.r11.s64 + 24684;
	// bl 0x827df490
	ctx.lr = 0x829F288C;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6724(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6724, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F28A8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F28A8) { __imp__sub_829F28A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F28A8) {
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
	// addi r3,r11,23988
	ctx.r3.s64 = ctx.r11.s64 + 23988;
	// bl 0x827df490
	ctx.lr = 0x829F28C4;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6720(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6720, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F28E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F28E0) { __imp__sub_829F28E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F28E0) {
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
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,10180
	ctx.r3.s64 = ctx.r11.s64 + 10180;
	// bl 0x827df490
	ctx.lr = 0x829F28FC;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6716(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6716, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2918);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2918) { __imp__sub_829F2918(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2918) {
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
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-30460
	ctx.r3.s64 = ctx.r11.s64 + -30460;
	// bl 0x827df490
	ctx.lr = 0x829F2934;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6712(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6712, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2950);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2950) { __imp__sub_829F2950(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2950) {
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
	// addi r3,r11,24728
	ctx.r3.s64 = ctx.r11.s64 + 24728;
	// bl 0x827df490
	ctx.lr = 0x829F296C;
	sub_827DF490(ctx, base);
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// stw r3,-6708(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6708, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2988);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2988) { __imp__sub_829F2988(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2988) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// li r10,31
	ctx.r10.s64 = 31;
	// addi r11,r11,-480
	ctx.r11.s64 = ctx.r11.s64 + -480;
	// li r8,-1
	ctx.r8.s64 = -1;
	// addi r9,r11,96
	ctx.r9.s64 = ctx.r11.s64 + 96;
loc_829F299C:
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
	// stw r8,0(r9)
	PPC_STORE_U32(ctx.r9.u32 + 0, ctx.r8.u32);
	// addi r9,r9,112
	ctx.r9.s64 = ctx.r9.s64 + 112;
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// bge cr6,0x829f299c
	if (!ctx.cr6.lt) goto loc_829F299C;
	// li r10,0
	ctx.r10.s64 = 0;
	// lis r9,-32096
	ctx.r9.s64 = -2103443456;
	// addi r3,r9,-5768
	ctx.r3.s64 = ctx.r9.s64 + -5768;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// stw r10,3600(r11)
	PPC_STORE_U32(ctx.r11.u32 + 3600, ctx.r10.u32);
	// stw r10,3604(r11)
	PPC_STORE_U32(ctx.r11.u32 + 3604, ctx.r10.u32);
	// stw r10,3608(r11)
	PPC_STORE_U32(ctx.r11.u32 + 3608, ctx.r10.u32);
	// stw r10,3612(r11)
	PPC_STORE_U32(ctx.r11.u32 + 3612, ctx.r10.u32);
	// stw r10,3616(r11)
	PPC_STORE_U32(ctx.r11.u32 + 3616, ctx.r10.u32);
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F29D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F29D8) { __imp__sub_829F29D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F29D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r11,r11,3264
	ctx.r11.s64 = ctx.r11.s64 + 3264;
	// li r10,330
	ctx.r10.s64 = 330;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_829F29EC:
	// std r9,0(r11)
	PPC_STORE_U64(ctx.r11.u32 + 0, ctx.r9.u64);
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// bdnz 0x829f29ec
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_829F29EC;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5760
	ctx.r3.s64 = ctx.r11.s64 + -5760;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2A08);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2A08) { __imp__sub_829F2A08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2A08) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r8,r11,5904
	ctx.r8.s64 = ctx.r11.s64 + 5904;
	// li r11,16
	ctx.r11.s64 = 16;
	// addi r10,r8,40
	ctx.r10.s64 = ctx.r8.s64 + 40;
loc_829F2A1C:
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// addi r10,r10,44
	ctx.r10.s64 = ctx.r10.s64 + 44;
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// bge cr6,0x829f2a1c
	if (!ctx.cr6.lt) goto loc_829F2A1C;
	// mr r11,r9
	ctx.r11.u64 = ctx.r9.u64;
	// stw r11,748(r8)
	PPC_STORE_U32(ctx.r8.u32 + 748, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2A40);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2A40) { __imp__sub_829F2A40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2A40) {
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
	// lis r11,-32006
	ctx.r11.s64 = -2097545216;
	// li r31,4
	ctx.r31.s64 = 4;
	// addi r11,r11,-5728
	ctx.r11.s64 = ctx.r11.s64 + -5728;
	// addi r30,r11,60
	ctx.r30.s64 = ctx.r11.s64 + 60;
loc_829F2A64:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8222bdd8
	ctx.lr = 0x829F2A6C;
	sub_8222BDD8(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,8
	ctx.r30.s64 = ctx.r30.s64 + 8;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x829f2a64
	if (!ctx.cr6.lt) goto loc_829F2A64;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5416
	ctx.r3.s64 = ctx.r11.s64 + -5416;
	// bl 0x8298ed98
	ctx.lr = 0x829F2A88;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F2AA0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2AA0) { __imp__sub_829F2AA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2AA0) {
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
	// lis r11,-32006
	ctx.r11.s64 = -2097545216;
	// addi r3,r11,-5520
	ctx.r3.s64 = ctx.r11.s64 + -5520;
	// bl 0x8275c058
	ctx.lr = 0x829F2AB8;
	sub_8275C058(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5736
	ctx.r3.s64 = ctx.r11.s64 + -5736;
	// bl 0x8298ed98
	ctx.lr = 0x829F2AC4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2AD8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2AD8) { __imp__sub_829F2AD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2AD8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// addi r3,r10,-5624
	ctx.r3.s64 = ctx.r10.s64 + -5624;
	// lis r10,-32006
	ctx.r10.s64 = -2097545216;
	// addi r11,r11,25564
	ctx.r11.s64 = ctx.r11.s64 + 25564;
	// addi r10,r10,-5168
	ctx.r10.s64 = ctx.r10.s64 + -5168;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// stw r11,208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 208, ctx.r11.u32);
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2B00);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2B00) { __imp__sub_829F2B00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2B00) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32006
	ctx.r11.s64 = -2097545216;
	// lis r4,-32256
	ctx.r4.s64 = -2113929216;
	// lis r5,-32256
	ctx.r5.s64 = -2113929216;
	// lis r6,-32256
	ctx.r6.s64 = -2113929216;
	// lis r7,-32256
	ctx.r7.s64 = -2113929216;
	// lis r9,-32254
	ctx.r9.s64 = -2113798144;
	// addi r11,r11,-4752
	ctx.r11.s64 = ctx.r11.s64 + -4752;
	// lfs f12,2980(r4)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r4.u32 + 2980);
	ctx.f12.f64 = double(temp.f32);
	// li r3,1
	ctx.r3.s64 = 1;
	// lfs f11,5180(r5)
	temp.u32 = PPC_LOAD_U32(ctx.r5.u32 + 5180);
	ctx.f11.f64 = double(temp.f32);
	// addi r8,r9,25556
	ctx.r8.s64 = ctx.r9.s64 + 25556;
	// lfs f0,2592(r6)
	temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + 2592);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,3068(r7)
	temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + 3068);
	ctx.f13.f64 = double(temp.f32);
	// mr r10,r3
	ctx.r10.u64 = ctx.r3.u64;
	// addi r11,r11,52
	ctx.r11.s64 = ctx.r11.s64 + 52;
	// li r9,0
	ctx.r9.s64 = 0;
loc_829F2B40:
	// stfs f13,-48(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + -48, temp.u32);
	// stw r8,-52(r11)
	PPC_STORE_U32(ctx.r11.u32 + -52, ctx.r8.u32);
	// stfs f0,-44(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -44, temp.u32);
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
	// stfs f13,-40(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + -40, temp.u32);
	// stfs f11,-36(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + -36, temp.u32);
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// stfs f13,-32(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + -32, temp.u32);
	// stfs f13,-28(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + -28, temp.u32);
	// stfs f13,-24(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + -24, temp.u32);
	// stb r3,-20(r11)
	PPC_STORE_U8(ctx.r11.u32 + -20, ctx.r3.u8);
	// stw r9,-16(r11)
	PPC_STORE_U32(ctx.r11.u32 + -16, ctx.r9.u32);
	// stw r9,-12(r11)
	PPC_STORE_U32(ctx.r11.u32 + -12, ctx.r9.u32);
	// stfs f0,-4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -4, temp.u32);
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stfs f12,8(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// stfs f0,16(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 16, temp.u32);
	// stfs f0,20(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 20, temp.u32);
	// stfs f12,24(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 24, temp.u32);
	// stfs f13,28(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 28, temp.u32);
	// stfs f13,32(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 32, temp.u32);
	// stfs f0,36(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 36, temp.u32);
	// addi r11,r11,96
	ctx.r11.s64 = ctx.r11.s64 + 96;
	// bge cr6,0x829f2b40
	if (!ctx.cr6.lt) goto loc_829F2B40;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5536
	ctx.r3.s64 = ctx.r11.s64 + -5536;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2BB8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2BB8) { __imp__sub_829F2BB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2BB8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5456
	ctx.r3.s64 = ctx.r11.s64 + -5456;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2BC8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2BC8) { __imp__sub_829F2BC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2BC8) {
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
	// lis r11,-32006
	ctx.r11.s64 = -2097545216;
	// addi r3,r11,-4544
	ctx.r3.s64 = ctx.r11.s64 + -4544;
	// bl 0x8284f880
	ctx.lr = 0x829F2BE0;
	sub_8284F880(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5752
	ctx.r3.s64 = ctx.r11.s64 + -5752;
	// bl 0x8298ed98
	ctx.lr = 0x829F2BEC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2C00);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2C00) { __imp__sub_829F2C00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2C00) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5400
	ctx.r3.s64 = ctx.r11.s64 + -5400;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2C10);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2C10) { __imp__sub_829F2C10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2C10) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5384
	ctx.r3.s64 = ctx.r11.s64 + -5384;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2C20);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2C20) { __imp__sub_829F2C20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2C20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5368
	ctx.r3.s64 = ctx.r11.s64 + -5368;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2C30);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2C30) { __imp__sub_829F2C30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2C30) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5352
	ctx.r3.s64 = ctx.r11.s64 + -5352;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2C40);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2C40) { __imp__sub_829F2C40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2C40) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32006
	ctx.r11.s64 = -2097545216;
	// li r8,2400
	ctx.r8.s64 = 2400;
	// addi r11,r11,18608
	ctx.r11.s64 = ctx.r11.s64 + 18608;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r11,r11,3
	ctx.r11.s64 = ctx.r11.s64 + 3;
	// li r10,-128
	ctx.r10.s64 = -128;
loc_829F2C58:
	// addi r8,r8,-1
	ctx.r8.s64 = ctx.r8.s64 + -1;
	// stb r9,-2(r11)
	PPC_STORE_U8(ctx.r11.u32 + -2, ctx.r9.u8);
	// stb r10,-3(r11)
	PPC_STORE_U8(ctx.r11.u32 + -3, ctx.r10.u8);
	// stb r9,0(r11)
	PPC_STORE_U8(ctx.r11.u32 + 0, ctx.r9.u8);
	// cmplwi cr6,r8,0
	ctx.cr6.compare<uint32_t>(ctx.r8.u32, 0, ctx.xer);
	// stb r10,-1(r11)
	PPC_STORE_U8(ctx.r11.u32 + -1, ctx.r10.u8);
	// stb r9,2(r11)
	PPC_STORE_U8(ctx.r11.u32 + 2, ctx.r9.u8);
	// stb r10,1(r11)
	PPC_STORE_U8(ctx.r11.u32 + 1, ctx.r10.u8);
	// stb r9,4(r11)
	PPC_STORE_U8(ctx.r11.u32 + 4, ctx.r9.u8);
	// stb r10,3(r11)
	PPC_STORE_U8(ctx.r11.u32 + 3, ctx.r10.u8);
	// stb r9,6(r11)
	PPC_STORE_U8(ctx.r11.u32 + 6, ctx.r9.u8);
	// stb r10,5(r11)
	PPC_STORE_U8(ctx.r11.u32 + 5, ctx.r10.u8);
	// stb r9,8(r11)
	PPC_STORE_U8(ctx.r11.u32 + 8, ctx.r9.u8);
	// stb r10,7(r11)
	PPC_STORE_U8(ctx.r11.u32 + 7, ctx.r10.u8);
	// addi r11,r11,12
	ctx.r11.s64 = ctx.r11.s64 + 12;
	// bne cr6,0x829f2c58
	if (!ctx.cr6.eq) goto loc_829F2C58;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2CA0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2CA0) { __imp__sub_829F2CA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2CA0) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,-18064
	ctx.r3.s64 = ctx.r11.s64 + -18064;
	// bl 0x8289df08
	ctx.lr = 0x829F2CB8;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5336
	ctx.r3.s64 = ctx.r11.s64 + -5336;
	// bl 0x8298ed98
	ctx.lr = 0x829F2CC4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2CD8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2CD8) { __imp__sub_829F2CD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2CD8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,29776
	ctx.r5.s64 = ctx.r11.s64 + 29776;
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-18024
	ctx.r3.s64 = ctx.r11.s64 + -18024;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2CF8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2CF8) { __imp__sub_829F2CF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2CF8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,29788
	ctx.r5.s64 = ctx.r11.s64 + 29788;
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-18004
	ctx.r3.s64 = ctx.r11.s64 + -18004;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2D18);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2D18) { __imp__sub_829F2D18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2D18) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-26952
	ctx.r5.s64 = ctx.r11.s64 + -26952;
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-9236
	ctx.r3.s64 = ctx.r11.s64 + -9236;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2D38);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2D38) { __imp__sub_829F2D38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2D38) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-18688
	ctx.r5.s64 = ctx.r11.s64 + -18688;
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-9156
	ctx.r3.s64 = ctx.r11.s64 + -9156;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2D58);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2D58) { __imp__sub_829F2D58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2D58) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-18672
	ctx.r5.s64 = ctx.r11.s64 + -18672;
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-9136
	ctx.r3.s64 = ctx.r11.s64 + -9136;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2D78);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2D78) { __imp__sub_829F2D78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2D78) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// li r10,127
	ctx.r10.s64 = 127;
	// addi r11,r11,-9112
	ctx.r11.s64 = ctx.r11.s64 + -9112;
	// li r7,-1
	ctx.r7.s64 = -1;
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// li r9,0
	ctx.r9.s64 = 0;
	// li r8,3
	ctx.r8.s64 = 3;
loc_829F2D94:
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
	// stw r7,-8(r11)
	PPC_STORE_U32(ctx.r11.u32 + -8, ctx.r7.u32);
	// stw r9,-4(r11)
	PPC_STORE_U32(ctx.r11.u32 + -4, ctx.r9.u32);
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// stw r8,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r8.u32);
	// addi r11,r11,20
	ctx.r11.s64 = ctx.r11.s64 + 20;
	// bge cr6,0x829f2d94
	if (!ctx.cr6.lt) goto loc_829F2D94;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5328
	ctx.r3.s64 = ctx.r11.s64 + -5328;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2DC8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2DC8) { __imp__sub_829F2DC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2DC8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-14256
	ctx.r5.s64 = ctx.r11.s64 + -14256;
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-4296
	ctx.r3.s64 = ctx.r11.s64 + -4296;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2DE8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2DE8) { __imp__sub_829F2DE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2DE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5320
	ctx.r3.s64 = ctx.r11.s64 + -5320;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2DF8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2DF8) { __imp__sub_829F2DF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2DF8) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,-3328
	ctx.r3.s64 = ctx.r11.s64 + -3328;
	// bl 0x8289df08
	ctx.lr = 0x829F2E10;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5288
	ctx.r3.s64 = ctx.r11.s64 + -5288;
	// bl 0x8298ed98
	ctx.lr = 0x829F2E1C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2E30);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2E30) { __imp__sub_829F2E30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2E30) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,-3288
	ctx.r3.s64 = ctx.r11.s64 + -3288;
	// bl 0x8289df08
	ctx.lr = 0x829F2E48;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5280
	ctx.r3.s64 = ctx.r11.s64 + -5280;
	// bl 0x8298ed98
	ctx.lr = 0x829F2E54;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2E68);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2E68) { __imp__sub_829F2E68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2E68) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,-3248
	ctx.r3.s64 = ctx.r11.s64 + -3248;
	// bl 0x825c1d00
	ctx.lr = 0x829F2E80;
	sub_825C1D00(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5272
	ctx.r3.s64 = ctx.r11.s64 + -5272;
	// bl 0x8298ed98
	ctx.lr = 0x829F2E8C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2EA0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2EA0) { __imp__sub_829F2EA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2EA0) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,-48
	ctx.r3.s64 = ctx.r11.s64 + -48;
	// bl 0x8289df08
	ctx.lr = 0x829F2EB8;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5256
	ctx.r3.s64 = ctx.r11.s64 + -5256;
	// bl 0x8298ed98
	ctx.lr = 0x829F2EC4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2ED8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2ED8) { __imp__sub_829F2ED8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2ED8) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,-8
	ctx.r3.s64 = ctx.r11.s64 + -8;
	// bl 0x8289df08
	ctx.lr = 0x829F2EF0;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5248
	ctx.r3.s64 = ctx.r11.s64 + -5248;
	// bl 0x8298ed98
	ctx.lr = 0x829F2EFC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2F10);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2F10) { __imp__sub_829F2F10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2F10) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,32
	ctx.r3.s64 = ctx.r11.s64 + 32;
	// bl 0x8289df08
	ctx.lr = 0x829F2F28;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5240
	ctx.r3.s64 = ctx.r11.s64 + -5240;
	// bl 0x8298ed98
	ctx.lr = 0x829F2F34;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2F48);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2F48) { __imp__sub_829F2F48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2F48) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,72
	ctx.r3.s64 = ctx.r11.s64 + 72;
	// bl 0x8289df08
	ctx.lr = 0x829F2F60;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5232
	ctx.r3.s64 = ctx.r11.s64 + -5232;
	// bl 0x8298ed98
	ctx.lr = 0x829F2F6C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2F80);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2F80) { __imp__sub_829F2F80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2F80) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,112
	ctx.r3.s64 = ctx.r11.s64 + 112;
	// bl 0x8289df08
	ctx.lr = 0x829F2F98;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5224
	ctx.r3.s64 = ctx.r11.s64 + -5224;
	// bl 0x8298ed98
	ctx.lr = 0x829F2FA4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2FB8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2FB8) { __imp__sub_829F2FB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2FB8) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,152
	ctx.r3.s64 = ctx.r11.s64 + 152;
	// bl 0x8289df08
	ctx.lr = 0x829F2FD0;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5216
	ctx.r3.s64 = ctx.r11.s64 + -5216;
	// bl 0x8298ed98
	ctx.lr = 0x829F2FDC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F2FF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F2FF0) { __imp__sub_829F2FF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F2FF0) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,192
	ctx.r3.s64 = ctx.r11.s64 + 192;
	// bl 0x8289df08
	ctx.lr = 0x829F3008;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5208
	ctx.r3.s64 = ctx.r11.s64 + -5208;
	// bl 0x8298ed98
	ctx.lr = 0x829F3014;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3028);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3028) { __imp__sub_829F3028(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3028) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,232
	ctx.r3.s64 = ctx.r11.s64 + 232;
	// bl 0x8289df08
	ctx.lr = 0x829F3040;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5176
	ctx.r3.s64 = ctx.r11.s64 + -5176;
	// bl 0x8298ed98
	ctx.lr = 0x829F304C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3060);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3060) { __imp__sub_829F3060(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3060) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,272
	ctx.r3.s64 = ctx.r11.s64 + 272;
	// bl 0x8289df08
	ctx.lr = 0x829F3078;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5184
	ctx.r3.s64 = ctx.r11.s64 + -5184;
	// bl 0x8298ed98
	ctx.lr = 0x829F3084;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3098);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3098) { __imp__sub_829F3098(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3098) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,312
	ctx.r3.s64 = ctx.r11.s64 + 312;
	// bl 0x8289df08
	ctx.lr = 0x829F30B0;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5192
	ctx.r3.s64 = ctx.r11.s64 + -5192;
	// bl 0x8298ed98
	ctx.lr = 0x829F30BC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F30D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F30D0) { __imp__sub_829F30D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F30D0) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,352
	ctx.r3.s64 = ctx.r11.s64 + 352;
	// bl 0x8289df08
	ctx.lr = 0x829F30E8;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5200
	ctx.r3.s64 = ctx.r11.s64 + -5200;
	// bl 0x8298ed98
	ctx.lr = 0x829F30F4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3108);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3108) { __imp__sub_829F3108(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3108) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,392
	ctx.r3.s64 = ctx.r11.s64 + 392;
	// bl 0x8289df08
	ctx.lr = 0x829F3120;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5168
	ctx.r3.s64 = ctx.r11.s64 + -5168;
	// bl 0x8298ed98
	ctx.lr = 0x829F312C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3140);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3140) { __imp__sub_829F3140(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3140) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,432
	ctx.r3.s64 = ctx.r11.s64 + 432;
	// bl 0x8289df08
	ctx.lr = 0x829F3158;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5152
	ctx.r3.s64 = ctx.r11.s64 + -5152;
	// bl 0x8298ed98
	ctx.lr = 0x829F3164;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3178);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3178) { __imp__sub_829F3178(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3178) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,472
	ctx.r3.s64 = ctx.r11.s64 + 472;
	// bl 0x8289df08
	ctx.lr = 0x829F3190;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5160
	ctx.r3.s64 = ctx.r11.s64 + -5160;
	// bl 0x8298ed98
	ctx.lr = 0x829F319C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F31B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F31B0) { __imp__sub_829F31B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F31B0) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,512
	ctx.r3.s64 = ctx.r11.s64 + 512;
	// bl 0x8289df08
	ctx.lr = 0x829F31C8;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5120
	ctx.r3.s64 = ctx.r11.s64 + -5120;
	// bl 0x8298ed98
	ctx.lr = 0x829F31D4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F31E8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F31E8) { __imp__sub_829F31E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F31E8) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,552
	ctx.r3.s64 = ctx.r11.s64 + 552;
	// bl 0x8289df08
	ctx.lr = 0x829F3200;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5128
	ctx.r3.s64 = ctx.r11.s64 + -5128;
	// bl 0x8298ed98
	ctx.lr = 0x829F320C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3220);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3220) { __imp__sub_829F3220(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3220) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,592
	ctx.r3.s64 = ctx.r11.s64 + 592;
	// bl 0x8289df08
	ctx.lr = 0x829F3238;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5136
	ctx.r3.s64 = ctx.r11.s64 + -5136;
	// bl 0x8298ed98
	ctx.lr = 0x829F3244;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3258);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3258) { __imp__sub_829F3258(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3258) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,632
	ctx.r3.s64 = ctx.r11.s64 + 632;
	// bl 0x8289df08
	ctx.lr = 0x829F3270;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5144
	ctx.r3.s64 = ctx.r11.s64 + -5144;
	// bl 0x8298ed98
	ctx.lr = 0x829F327C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3290);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3290) { __imp__sub_829F3290(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3290) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,672
	ctx.r3.s64 = ctx.r11.s64 + 672;
	// bl 0x8289df08
	ctx.lr = 0x829F32A8;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5112
	ctx.r3.s64 = ctx.r11.s64 + -5112;
	// bl 0x8298ed98
	ctx.lr = 0x829F32B4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F32C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F32C8) { __imp__sub_829F32C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F32C8) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,712
	ctx.r3.s64 = ctx.r11.s64 + 712;
	// bl 0x8289df08
	ctx.lr = 0x829F32E0;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5088
	ctx.r3.s64 = ctx.r11.s64 + -5088;
	// bl 0x8298ed98
	ctx.lr = 0x829F32EC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3300);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3300) { __imp__sub_829F3300(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3300) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,752
	ctx.r3.s64 = ctx.r11.s64 + 752;
	// bl 0x8289df08
	ctx.lr = 0x829F3318;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5096
	ctx.r3.s64 = ctx.r11.s64 + -5096;
	// bl 0x8298ed98
	ctx.lr = 0x829F3324;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3338);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3338) { __imp__sub_829F3338(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3338) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,792
	ctx.r3.s64 = ctx.r11.s64 + 792;
	// bl 0x8289df08
	ctx.lr = 0x829F3350;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5104
	ctx.r3.s64 = ctx.r11.s64 + -5104;
	// bl 0x8298ed98
	ctx.lr = 0x829F335C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3370);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3370) { __imp__sub_829F3370(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3370) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,832
	ctx.r3.s64 = ctx.r11.s64 + 832;
	// bl 0x8289df08
	ctx.lr = 0x829F3388;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5072
	ctx.r3.s64 = ctx.r11.s64 + -5072;
	// bl 0x8298ed98
	ctx.lr = 0x829F3394;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F33A8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F33A8) { __imp__sub_829F33A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F33A8) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,872
	ctx.r3.s64 = ctx.r11.s64 + 872;
	// bl 0x8289df08
	ctx.lr = 0x829F33C0;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5080
	ctx.r3.s64 = ctx.r11.s64 + -5080;
	// bl 0x8298ed98
	ctx.lr = 0x829F33CC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F33E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F33E0) { __imp__sub_829F33E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F33E0) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,912
	ctx.r3.s64 = ctx.r11.s64 + 912;
	// bl 0x8289df08
	ctx.lr = 0x829F33F8;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5056
	ctx.r3.s64 = ctx.r11.s64 + -5056;
	// bl 0x8298ed98
	ctx.lr = 0x829F3404;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3418);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3418) { __imp__sub_829F3418(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3418) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,952
	ctx.r3.s64 = ctx.r11.s64 + 952;
	// bl 0x8289df08
	ctx.lr = 0x829F3430;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5064
	ctx.r3.s64 = ctx.r11.s64 + -5064;
	// bl 0x8298ed98
	ctx.lr = 0x829F343C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3450);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3450) { __imp__sub_829F3450(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3450) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,992
	ctx.r3.s64 = ctx.r11.s64 + 992;
	// bl 0x8289df08
	ctx.lr = 0x829F3468;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5032
	ctx.r3.s64 = ctx.r11.s64 + -5032;
	// bl 0x8298ed98
	ctx.lr = 0x829F3474;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3488);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3488) { __imp__sub_829F3488(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3488) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,1032
	ctx.r3.s64 = ctx.r11.s64 + 1032;
	// bl 0x8289df08
	ctx.lr = 0x829F34A0;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5040
	ctx.r3.s64 = ctx.r11.s64 + -5040;
	// bl 0x8298ed98
	ctx.lr = 0x829F34AC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F34C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F34C0) { __imp__sub_829F34C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F34C0) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,1072
	ctx.r3.s64 = ctx.r11.s64 + 1072;
	// bl 0x8289df08
	ctx.lr = 0x829F34D8;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5048
	ctx.r3.s64 = ctx.r11.s64 + -5048;
	// bl 0x8298ed98
	ctx.lr = 0x829F34E4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F34F8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F34F8) { __imp__sub_829F34F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F34F8) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,1112
	ctx.r3.s64 = ctx.r11.s64 + 1112;
	// bl 0x8289df08
	ctx.lr = 0x829F3510;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5016
	ctx.r3.s64 = ctx.r11.s64 + -5016;
	// bl 0x8298ed98
	ctx.lr = 0x829F351C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3530);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3530) { __imp__sub_829F3530(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3530) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,1152
	ctx.r3.s64 = ctx.r11.s64 + 1152;
	// bl 0x8289df08
	ctx.lr = 0x829F3548;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5024
	ctx.r3.s64 = ctx.r11.s64 + -5024;
	// bl 0x8298ed98
	ctx.lr = 0x829F3554;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3568);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3568) { __imp__sub_829F3568(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3568) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,1192
	ctx.r3.s64 = ctx.r11.s64 + 1192;
	// bl 0x8289df08
	ctx.lr = 0x829F3580;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4992
	ctx.r3.s64 = ctx.r11.s64 + -4992;
	// bl 0x8298ed98
	ctx.lr = 0x829F358C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F35A0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F35A0) { __imp__sub_829F35A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F35A0) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,1232
	ctx.r3.s64 = ctx.r11.s64 + 1232;
	// bl 0x8289df08
	ctx.lr = 0x829F35B8;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5000
	ctx.r3.s64 = ctx.r11.s64 + -5000;
	// bl 0x8298ed98
	ctx.lr = 0x829F35C4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F35D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F35D8) { __imp__sub_829F35D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F35D8) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,1272
	ctx.r3.s64 = ctx.r11.s64 + 1272;
	// bl 0x8289df08
	ctx.lr = 0x829F35F0;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-5008
	ctx.r3.s64 = ctx.r11.s64 + -5008;
	// bl 0x8298ed98
	ctx.lr = 0x829F35FC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3610);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3610) { __imp__sub_829F3610(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3610) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,1312
	ctx.r3.s64 = ctx.r11.s64 + 1312;
	// bl 0x8289df08
	ctx.lr = 0x829F3628;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4984
	ctx.r3.s64 = ctx.r11.s64 + -4984;
	// bl 0x8298ed98
	ctx.lr = 0x829F3634;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3648);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3648) { __imp__sub_829F3648(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3648) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,1352
	ctx.r3.s64 = ctx.r11.s64 + 1352;
	// bl 0x8289df08
	ctx.lr = 0x829F3660;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4976
	ctx.r3.s64 = ctx.r11.s64 + -4976;
	// bl 0x8298ed98
	ctx.lr = 0x829F366C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3680);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3680) { __imp__sub_829F3680(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3680) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,1392
	ctx.r3.s64 = ctx.r11.s64 + 1392;
	// bl 0x8289df08
	ctx.lr = 0x829F3698;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4968
	ctx.r3.s64 = ctx.r11.s64 + -4968;
	// bl 0x8298ed98
	ctx.lr = 0x829F36A4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F36B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F36B8) { __imp__sub_829F36B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F36B8) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,1432
	ctx.r3.s64 = ctx.r11.s64 + 1432;
	// bl 0x8289df08
	ctx.lr = 0x829F36D0;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4960
	ctx.r3.s64 = ctx.r11.s64 + -4960;
	// bl 0x8298ed98
	ctx.lr = 0x829F36DC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F36F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F36F0) { __imp__sub_829F36F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F36F0) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,1472
	ctx.r3.s64 = ctx.r11.s64 + 1472;
	// bl 0x8289df08
	ctx.lr = 0x829F3708;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4952
	ctx.r3.s64 = ctx.r11.s64 + -4952;
	// bl 0x8298ed98
	ctx.lr = 0x829F3714;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3728);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3728) { __imp__sub_829F3728(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3728) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,1512
	ctx.r3.s64 = ctx.r11.s64 + 1512;
	// bl 0x8289df08
	ctx.lr = 0x829F3740;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4944
	ctx.r3.s64 = ctx.r11.s64 + -4944;
	// bl 0x8298ed98
	ctx.lr = 0x829F374C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3760);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3760) { __imp__sub_829F3760(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3760) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,1552
	ctx.r3.s64 = ctx.r11.s64 + 1552;
	// bl 0x8289df08
	ctx.lr = 0x829F3778;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4928
	ctx.r3.s64 = ctx.r11.s64 + -4928;
	// bl 0x8298ed98
	ctx.lr = 0x829F3784;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3798);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3798) { __imp__sub_829F3798(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3798) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,1592
	ctx.r3.s64 = ctx.r11.s64 + 1592;
	// bl 0x8289df08
	ctx.lr = 0x829F37B0;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4936
	ctx.r3.s64 = ctx.r11.s64 + -4936;
	// bl 0x8298ed98
	ctx.lr = 0x829F37BC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F37D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F37D0) { __imp__sub_829F37D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F37D0) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,1632
	ctx.r3.s64 = ctx.r11.s64 + 1632;
	// bl 0x8289df08
	ctx.lr = 0x829F37E8;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4920
	ctx.r3.s64 = ctx.r11.s64 + -4920;
	// bl 0x8298ed98
	ctx.lr = 0x829F37F4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3808);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3808) { __imp__sub_829F3808(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3808) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,1672
	ctx.r3.s64 = ctx.r11.s64 + 1672;
	// bl 0x8289df08
	ctx.lr = 0x829F3820;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4912
	ctx.r3.s64 = ctx.r11.s64 + -4912;
	// bl 0x8298ed98
	ctx.lr = 0x829F382C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3840);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3840) { __imp__sub_829F3840(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3840) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,1712
	ctx.r3.s64 = ctx.r11.s64 + 1712;
	// bl 0x8289df08
	ctx.lr = 0x829F3858;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4904
	ctx.r3.s64 = ctx.r11.s64 + -4904;
	// bl 0x8298ed98
	ctx.lr = 0x829F3864;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3878);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3878) { __imp__sub_829F3878(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3878) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,1752
	ctx.r3.s64 = ctx.r11.s64 + 1752;
	// bl 0x8289df08
	ctx.lr = 0x829F3890;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4888
	ctx.r3.s64 = ctx.r11.s64 + -4888;
	// bl 0x8298ed98
	ctx.lr = 0x829F389C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F38B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F38B0) { __imp__sub_829F38B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F38B0) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,1792
	ctx.r3.s64 = ctx.r11.s64 + 1792;
	// bl 0x8289df08
	ctx.lr = 0x829F38C8;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4896
	ctx.r3.s64 = ctx.r11.s64 + -4896;
	// bl 0x8298ed98
	ctx.lr = 0x829F38D4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F38E8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F38E8) { __imp__sub_829F38E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F38E8) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,1832
	ctx.r3.s64 = ctx.r11.s64 + 1832;
	// bl 0x8289df08
	ctx.lr = 0x829F3900;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4872
	ctx.r3.s64 = ctx.r11.s64 + -4872;
	// bl 0x8298ed98
	ctx.lr = 0x829F390C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3920);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3920) { __imp__sub_829F3920(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3920) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,1872
	ctx.r3.s64 = ctx.r11.s64 + 1872;
	// bl 0x8289df08
	ctx.lr = 0x829F3938;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4880
	ctx.r3.s64 = ctx.r11.s64 + -4880;
	// bl 0x8298ed98
	ctx.lr = 0x829F3944;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3958);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3958) { __imp__sub_829F3958(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3958) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,1912
	ctx.r3.s64 = ctx.r11.s64 + 1912;
	// bl 0x8289df08
	ctx.lr = 0x829F3970;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4840
	ctx.r3.s64 = ctx.r11.s64 + -4840;
	// bl 0x8298ed98
	ctx.lr = 0x829F397C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3990);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3990) { __imp__sub_829F3990(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3990) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,1952
	ctx.r3.s64 = ctx.r11.s64 + 1952;
	// bl 0x8289df08
	ctx.lr = 0x829F39A8;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4848
	ctx.r3.s64 = ctx.r11.s64 + -4848;
	// bl 0x8298ed98
	ctx.lr = 0x829F39B4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F39C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F39C8) { __imp__sub_829F39C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F39C8) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,1992
	ctx.r3.s64 = ctx.r11.s64 + 1992;
	// bl 0x8289df08
	ctx.lr = 0x829F39E0;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4856
	ctx.r3.s64 = ctx.r11.s64 + -4856;
	// bl 0x8298ed98
	ctx.lr = 0x829F39EC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3A00);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3A00) { __imp__sub_829F3A00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3A00) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,2032
	ctx.r3.s64 = ctx.r11.s64 + 2032;
	// bl 0x8289df08
	ctx.lr = 0x829F3A18;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4864
	ctx.r3.s64 = ctx.r11.s64 + -4864;
	// bl 0x8298ed98
	ctx.lr = 0x829F3A24;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3A38);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3A38) { __imp__sub_829F3A38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3A38) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,2072
	ctx.r3.s64 = ctx.r11.s64 + 2072;
	// bl 0x8289df08
	ctx.lr = 0x829F3A50;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4808
	ctx.r3.s64 = ctx.r11.s64 + -4808;
	// bl 0x8298ed98
	ctx.lr = 0x829F3A5C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3A70);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3A70) { __imp__sub_829F3A70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3A70) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,2112
	ctx.r3.s64 = ctx.r11.s64 + 2112;
	// bl 0x8289df08
	ctx.lr = 0x829F3A88;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4816
	ctx.r3.s64 = ctx.r11.s64 + -4816;
	// bl 0x8298ed98
	ctx.lr = 0x829F3A94;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3AA8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3AA8) { __imp__sub_829F3AA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3AA8) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,2152
	ctx.r3.s64 = ctx.r11.s64 + 2152;
	// bl 0x8289df08
	ctx.lr = 0x829F3AC0;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4824
	ctx.r3.s64 = ctx.r11.s64 + -4824;
	// bl 0x8298ed98
	ctx.lr = 0x829F3ACC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3AE0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3AE0) { __imp__sub_829F3AE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3AE0) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,2192
	ctx.r3.s64 = ctx.r11.s64 + 2192;
	// bl 0x8289df08
	ctx.lr = 0x829F3AF8;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4832
	ctx.r3.s64 = ctx.r11.s64 + -4832;
	// bl 0x8298ed98
	ctx.lr = 0x829F3B04;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3B18);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3B18) { __imp__sub_829F3B18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3B18) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,2232
	ctx.r3.s64 = ctx.r11.s64 + 2232;
	// bl 0x8289df08
	ctx.lr = 0x829F3B30;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4776
	ctx.r3.s64 = ctx.r11.s64 + -4776;
	// bl 0x8298ed98
	ctx.lr = 0x829F3B3C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3B50);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3B50) { __imp__sub_829F3B50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3B50) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,2272
	ctx.r3.s64 = ctx.r11.s64 + 2272;
	// bl 0x8289df08
	ctx.lr = 0x829F3B68;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4784
	ctx.r3.s64 = ctx.r11.s64 + -4784;
	// bl 0x8298ed98
	ctx.lr = 0x829F3B74;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3B88);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3B88) { __imp__sub_829F3B88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3B88) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,2312
	ctx.r3.s64 = ctx.r11.s64 + 2312;
	// bl 0x8289df08
	ctx.lr = 0x829F3BA0;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4792
	ctx.r3.s64 = ctx.r11.s64 + -4792;
	// bl 0x8298ed98
	ctx.lr = 0x829F3BAC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3BC0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3BC0) { __imp__sub_829F3BC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3BC0) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,2352
	ctx.r3.s64 = ctx.r11.s64 + 2352;
	// bl 0x8289df08
	ctx.lr = 0x829F3BD8;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4800
	ctx.r3.s64 = ctx.r11.s64 + -4800;
	// bl 0x8298ed98
	ctx.lr = 0x829F3BE4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3BF8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3BF8) { __imp__sub_829F3BF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3BF8) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,2392
	ctx.r3.s64 = ctx.r11.s64 + 2392;
	// bl 0x8289df08
	ctx.lr = 0x829F3C10;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4744
	ctx.r3.s64 = ctx.r11.s64 + -4744;
	// bl 0x8298ed98
	ctx.lr = 0x829F3C1C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3C30);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3C30) { __imp__sub_829F3C30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3C30) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,2432
	ctx.r3.s64 = ctx.r11.s64 + 2432;
	// bl 0x8289df08
	ctx.lr = 0x829F3C48;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4752
	ctx.r3.s64 = ctx.r11.s64 + -4752;
	// bl 0x8298ed98
	ctx.lr = 0x829F3C54;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3C68);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3C68) { __imp__sub_829F3C68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3C68) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,2472
	ctx.r3.s64 = ctx.r11.s64 + 2472;
	// bl 0x8289df08
	ctx.lr = 0x829F3C80;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4760
	ctx.r3.s64 = ctx.r11.s64 + -4760;
	// bl 0x8298ed98
	ctx.lr = 0x829F3C8C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3CA0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3CA0) { __imp__sub_829F3CA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3CA0) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,2512
	ctx.r3.s64 = ctx.r11.s64 + 2512;
	// bl 0x8289df08
	ctx.lr = 0x829F3CB8;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4768
	ctx.r3.s64 = ctx.r11.s64 + -4768;
	// bl 0x8298ed98
	ctx.lr = 0x829F3CC4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3CD8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3CD8) { __imp__sub_829F3CD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3CD8) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,2552
	ctx.r3.s64 = ctx.r11.s64 + 2552;
	// bl 0x8289df08
	ctx.lr = 0x829F3CF0;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4712
	ctx.r3.s64 = ctx.r11.s64 + -4712;
	// bl 0x8298ed98
	ctx.lr = 0x829F3CFC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3D10);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3D10) { __imp__sub_829F3D10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3D10) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,2592
	ctx.r3.s64 = ctx.r11.s64 + 2592;
	// bl 0x8289df08
	ctx.lr = 0x829F3D28;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4720
	ctx.r3.s64 = ctx.r11.s64 + -4720;
	// bl 0x8298ed98
	ctx.lr = 0x829F3D34;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3D48);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3D48) { __imp__sub_829F3D48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3D48) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,2632
	ctx.r3.s64 = ctx.r11.s64 + 2632;
	// bl 0x8289df08
	ctx.lr = 0x829F3D60;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4728
	ctx.r3.s64 = ctx.r11.s64 + -4728;
	// bl 0x8298ed98
	ctx.lr = 0x829F3D6C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3D80);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3D80) { __imp__sub_829F3D80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3D80) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,2672
	ctx.r3.s64 = ctx.r11.s64 + 2672;
	// bl 0x8289df08
	ctx.lr = 0x829F3D98;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4736
	ctx.r3.s64 = ctx.r11.s64 + -4736;
	// bl 0x8298ed98
	ctx.lr = 0x829F3DA4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3DB8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3DB8) { __imp__sub_829F3DB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3DB8) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,2712
	ctx.r3.s64 = ctx.r11.s64 + 2712;
	// bl 0x8289df08
	ctx.lr = 0x829F3DD0;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4704
	ctx.r3.s64 = ctx.r11.s64 + -4704;
	// bl 0x8298ed98
	ctx.lr = 0x829F3DDC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3DF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3DF0) { __imp__sub_829F3DF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3DF0) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,2752
	ctx.r3.s64 = ctx.r11.s64 + 2752;
	// bl 0x8289df08
	ctx.lr = 0x829F3E08;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4696
	ctx.r3.s64 = ctx.r11.s64 + -4696;
	// bl 0x8298ed98
	ctx.lr = 0x829F3E14;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3E28);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3E28) { __imp__sub_829F3E28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3E28) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,2792
	ctx.r3.s64 = ctx.r11.s64 + 2792;
	// bl 0x8289df08
	ctx.lr = 0x829F3E40;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4680
	ctx.r3.s64 = ctx.r11.s64 + -4680;
	// bl 0x8298ed98
	ctx.lr = 0x829F3E4C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3E60);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3E60) { __imp__sub_829F3E60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3E60) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,2832
	ctx.r3.s64 = ctx.r11.s64 + 2832;
	// bl 0x8289df08
	ctx.lr = 0x829F3E78;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4688
	ctx.r3.s64 = ctx.r11.s64 + -4688;
	// bl 0x8298ed98
	ctx.lr = 0x829F3E84;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3E98);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3E98) { __imp__sub_829F3E98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3E98) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,2872
	ctx.r3.s64 = ctx.r11.s64 + 2872;
	// bl 0x8289df08
	ctx.lr = 0x829F3EB0;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4672
	ctx.r3.s64 = ctx.r11.s64 + -4672;
	// bl 0x8298ed98
	ctx.lr = 0x829F3EBC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3ED0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3ED0) { __imp__sub_829F3ED0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3ED0) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,2912
	ctx.r3.s64 = ctx.r11.s64 + 2912;
	// bl 0x8289df08
	ctx.lr = 0x829F3EE8;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4664
	ctx.r3.s64 = ctx.r11.s64 + -4664;
	// bl 0x8298ed98
	ctx.lr = 0x829F3EF4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3F08);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3F08) { __imp__sub_829F3F08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3F08) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,2952
	ctx.r3.s64 = ctx.r11.s64 + 2952;
	// bl 0x8289df08
	ctx.lr = 0x829F3F20;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4656
	ctx.r3.s64 = ctx.r11.s64 + -4656;
	// bl 0x8298ed98
	ctx.lr = 0x829F3F2C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3F40);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3F40) { __imp__sub_829F3F40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3F40) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-11712
	ctx.r3.s64 = ctx.r11.s64 + -11712;
	// bl 0x827df490
	ctx.lr = 0x829F3F5C;
	sub_827DF490(ctx, base);
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// stw r3,2992(r11)
	PPC_STORE_U32(ctx.r11.u32 + 2992, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3F78);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3F78) { __imp__sub_829F3F78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3F78) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-6420
	ctx.r3.s64 = ctx.r11.s64 + -6420;
	// bl 0x827df490
	ctx.lr = 0x829F3F94;
	sub_827DF490(ctx, base);
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// stw r3,2996(r11)
	PPC_STORE_U32(ctx.r11.u32 + 2996, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3FB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3FB0) { __imp__sub_829F3FB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3FB0) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-6380
	ctx.r3.s64 = ctx.r11.s64 + -6380;
	// bl 0x827df490
	ctx.lr = 0x829F3FCC;
	sub_827DF490(ctx, base);
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// stw r3,3000(r11)
	PPC_STORE_U32(ctx.r11.u32 + 3000, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F3FE8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F3FE8) { __imp__sub_829F3FE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F3FE8) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-6344
	ctx.r3.s64 = ctx.r11.s64 + -6344;
	// bl 0x827df490
	ctx.lr = 0x829F4004;
	sub_827DF490(ctx, base);
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// stw r3,3004(r11)
	PPC_STORE_U32(ctx.r11.u32 + 3004, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4020);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4020) { __imp__sub_829F4020(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4020) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-6308
	ctx.r3.s64 = ctx.r11.s64 + -6308;
	// bl 0x827df490
	ctx.lr = 0x829F403C;
	sub_827DF490(ctx, base);
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// stw r3,3008(r11)
	PPC_STORE_U32(ctx.r11.u32 + 3008, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4058);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4058) { __imp__sub_829F4058(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4058) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-6280
	ctx.r3.s64 = ctx.r11.s64 + -6280;
	// bl 0x827df490
	ctx.lr = 0x829F4074;
	sub_827DF490(ctx, base);
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// stw r3,3012(r11)
	PPC_STORE_U32(ctx.r11.u32 + 3012, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4090);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4090) { __imp__sub_829F4090(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4090) {
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
	// addi r3,r11,-7824
	ctx.r3.s64 = ctx.r11.s64 + -7824;
	// bl 0x827df490
	ctx.lr = 0x829F40AC;
	sub_827DF490(ctx, base);
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// stw r3,3016(r11)
	PPC_STORE_U32(ctx.r11.u32 + 3016, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F40C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F40C8) { __imp__sub_829F40C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F40C8) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-6268
	ctx.r3.s64 = ctx.r11.s64 + -6268;
	// bl 0x827df490
	ctx.lr = 0x829F40E4;
	sub_827DF490(ctx, base);
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// stw r3,3020(r11)
	PPC_STORE_U32(ctx.r11.u32 + 3020, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4100);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4100) { __imp__sub_829F4100(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4100) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-5980
	ctx.r3.s64 = ctx.r11.s64 + -5980;
	// bl 0x827df490
	ctx.lr = 0x829F4120;
	sub_827DF490(ctx, base);
	// lis r10,-32005
	ctx.r10.s64 = -2097479680;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,3264
	ctx.r31.s64 = ctx.r10.s64 + 3264;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r11,r11,-6000
	ctx.r11.s64 = ctx.r11.s64 + -6000;
	// addi r10,r10,-6036
	ctx.r10.s64 = ctx.r10.s64 + -6036;
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// stw r10,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r10.u32);
	// bl 0x827df490
	ctx.lr = 0x829F414C;
	sub_827DF490(ctx, base);
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// stw r3,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r3.u32);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r10,r10,-6064
	ctx.r10.s64 = ctx.r10.s64 + -6064;
	// addi r11,r11,-6084
	ctx.r11.s64 = ctx.r11.s64 + -6084;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// stw r10,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r10.u32);
	// bl 0x827df490
	ctx.lr = 0x829F4170;
	sub_827DF490(ctx, base);
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// stw r3,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r3.u32);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r10,r10,-6112
	ctx.r10.s64 = ctx.r10.s64 + -6112;
	// addi r11,r11,-6132
	ctx.r11.s64 = ctx.r11.s64 + -6132;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// stw r10,20(r31)
	PPC_STORE_U32(ctx.r31.u32 + 20, ctx.r10.u32);
	// bl 0x827df490
	ctx.lr = 0x829F4194;
	sub_827DF490(ctx, base);
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// stw r3,24(r31)
	PPC_STORE_U32(ctx.r31.u32 + 24, ctx.r3.u32);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r10,r10,-6160
	ctx.r10.s64 = ctx.r10.s64 + -6160;
	// addi r11,r11,-6180
	ctx.r11.s64 = ctx.r11.s64 + -6180;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// stw r10,28(r31)
	PPC_STORE_U32(ctx.r31.u32 + 28, ctx.r10.u32);
	// bl 0x827df490
	ctx.lr = 0x829F41B8;
	sub_827DF490(ctx, base);
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// stw r3,32(r31)
	PPC_STORE_U32(ctx.r31.u32 + 32, ctx.r3.u32);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r10,r10,-6208
	ctx.r10.s64 = ctx.r10.s64 + -6208;
	// addi r11,r11,-6228
	ctx.r11.s64 = ctx.r11.s64 + -6228;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// stw r10,36(r31)
	PPC_STORE_U32(ctx.r31.u32 + 36, ctx.r10.u32);
	// bl 0x827df490
	ctx.lr = 0x829F41DC;
	sub_827DF490(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r3.u32);
	// addi r11,r11,-6256
	ctx.r11.s64 = ctx.r11.s64 + -6256;
	// stw r11,44(r31)
	PPC_STORE_U32(ctx.r31.u32 + 44, ctx.r11.u32);
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

PPC_FUNC_IMPL(__imp__sub_829F4200);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4200) { __imp__sub_829F4200(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4200) {
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
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// li r31,5
	ctx.r31.s64 = 5;
	// addi r30,r11,3024
	ctx.r30.s64 = ctx.r11.s64 + 3024;
loc_829F4220:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8289df08
	ctx.lr = 0x829F4228;
	sub_8289DF08(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,40
	ctx.r30.s64 = ctx.r30.s64 + 40;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x829f4220
	if (!ctx.cr6.lt) goto loc_829F4220;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4648
	ctx.r3.s64 = ctx.r11.s64 + -4648;
	// bl 0x8298ed98
	ctx.lr = 0x829F4244;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F4260);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4260) { __imp__sub_829F4260(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4260) {
	PPC_FUNC_PROLOGUE();
loc_829F4260:
	// mftb r11
	ctx.r11.u64 = PPC_QUERY_TIMEBASE();
	// rotlwi r10,r11,0
	ctx.r10.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x829f4260
	if (ctx.cr6.eq) goto loc_829F4260;
	// lis r10,-32005
	ctx.r10.s64 = -2097479680;
	// std r11,3704(r10)
	PPC_STORE_U64(ctx.r10.u32 + 3704, ctx.r11.u64);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4280);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4280) { __imp__sub_829F4280(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4280) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4640
	ctx.r3.s64 = ctx.r11.s64 + -4640;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4290);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4290) { __imp__sub_829F4290(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4290) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// li r9,2623
	ctx.r9.s64 = 2623;
	// addi r11,r11,3832
	ctx.r11.s64 = ctx.r11.s64 + 3832;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,20
	ctx.r11.s64 = ctx.r11.s64 + 20;
loc_829F42A4:
	// stw r10,-4(r11)
	PPC_STORE_U32(ctx.r11.u32 + -4, ctx.r10.u32);
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// stw r10,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// stw r10,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r10.u32);
	// stw r10,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r10.u32);
	// addi r11,r11,36
	ctx.r11.s64 = ctx.r11.s64 + 36;
	// bge cr6,0x829f42a4
	if (!ctx.cr6.lt) goto loc_829F42A4;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F42D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F42D0) { __imp__sub_829F42D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F42D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4584
	ctx.r3.s64 = ctx.r11.s64 + -4584;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F42E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F42E0) { __imp__sub_829F42E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F42E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32092
	ctx.r11.s64 = -2103181312;
	// lwz r11,-17864(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -17864);
	// rlwinm r10,r11,1,0,30
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// add r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
	// lis r10,-32004
	ctx.r10.s64 = -2097414144;
	// rlwinm r11,r11,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// stw r11,32764(r10)
	PPC_STORE_U32(ctx.r10.u32 + 32764, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4300);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4300) { __imp__sub_829F4300(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4300) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4552
	ctx.r3.s64 = ctx.r11.s64 + -4552;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4310);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4310) { __imp__sub_829F4310(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4310) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4568
	ctx.r3.s64 = ctx.r11.s64 + -4568;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4320);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4320) { __imp__sub_829F4320(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4320) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// li r10,15
	ctx.r10.s64 = 15;
	// addi r11,r11,-32240
	ctx.r11.s64 = ctx.r11.s64 + -32240;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r11,r11,33
	ctx.r11.s64 = ctx.r11.s64 + 33;
loc_829F4334:
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
	// stb r9,-1(r11)
	PPC_STORE_U8(ctx.r11.u32 + -1, ctx.r9.u8);
	// stb r9,0(r11)
	PPC_STORE_U8(ctx.r11.u32 + 0, ctx.r9.u8);
	// stb r9,1(r11)
	PPC_STORE_U8(ctx.r11.u32 + 1, ctx.r9.u8);
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// addi r11,r11,48
	ctx.r11.s64 = ctx.r11.s64 + 48;
	// bge cr6,0x829f4334
	if (!ctx.cr6.lt) goto loc_829F4334;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4560
	ctx.r3.s64 = ctx.r11.s64 + -4560;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4360);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4360) { __imp__sub_829F4360(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4360) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32092
	ctx.r11.s64 = -2103181312;
	// lis r10,-32092
	ctx.r10.s64 = -2103181312;
	// lfs f0,-17888(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -17888);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r11,r11,-31472
	ctx.r11.s64 = ctx.r11.s64 + -31472;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// lfs f0,-17884(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -17884);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4388);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4388) { __imp__sub_829F4388(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4388) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32092
	ctx.r11.s64 = -2103181312;
	// lis r10,-32092
	ctx.r10.s64 = -2103181312;
	// lfs f0,-17888(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -17888);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r11,r11,-31456
	ctx.r11.s64 = ctx.r11.s64 + -31456;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// lfs f0,-17884(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -17884);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F43B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F43B0) { __imp__sub_829F43B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F43B0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x829F43B8;
	__savegprlr_29(ctx, base);
	// stfd f31,-40(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -40, ctx.f31.u64);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// li r30,49
	ctx.r30.s64 = 49;
	// addi r11,r11,-31440
	ctx.r11.s64 = ctx.r11.s64 + -31440;
	// li r29,0
	ctx.r29.s64 = 0;
	// addi r31,r11,600
	ctx.r31.s64 = ctx.r11.s64 + 600;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f31,3068(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3068);
	ctx.f31.f64 = double(temp.f32);
loc_829F43DC:
	// lis r11,-32180
	ctx.r11.s64 = -2108948480;
	// li r5,12
	ctx.r5.s64 = 12;
	// addi r6,r11,-17224
	ctx.r6.s64 = ctx.r11.s64 + -17224;
	// li r4,48
	ctx.r4.s64 = 48;
	// addi r3,r31,-600
	ctx.r3.s64 = ctx.r31.s64 + -600;
	// bl 0x82120220
	ctx.lr = 0x829F43F4;
	sub_82120220(ctx, base);
	// stw r29,-24(r31)
	PPC_STORE_U32(ctx.r31.u32 + -24, ctx.r29.u32);
	// stw r29,-20(r31)
	PPC_STORE_U32(ctx.r31.u32 + -20, ctx.r29.u32);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// stb r29,-16(r31)
	PPC_STORE_U8(ctx.r31.u32 + -16, ctx.r29.u8);
	// stfs f31,-8(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r31.u32 + -8, temp.u32);
	// stfs f31,-4(r31)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r31.u32 + -4, temp.u32);
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// stfs f31,0(r31)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r31.u32 + 0, temp.u32);
	// stfs f31,8(r31)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r31.u32 + 8, temp.u32);
	// stfs f31,12(r31)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r31.u32 + 12, temp.u32);
	// stfs f31,16(r31)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r31.u32 + 16, temp.u32);
	// addi r31,r31,640
	ctx.r31.s64 = ctx.r31.s64 + 640;
	// bge cr6,0x829f43dc
	if (!ctx.cr6.lt) goto loc_829F43DC;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// lfd f31,-40(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -40);
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4438);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4438) { __imp__sub_829F4438(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4438) {
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
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// li r30,1
	ctx.r30.s64 = 1;
	// addi r11,r11,8192
	ctx.r11.s64 = ctx.r11.s64 + 8192;
	// addi r31,r11,1568
	ctx.r31.s64 = ctx.r11.s64 + 1568;
loc_829F445C:
	// lis r11,-32161
	ctx.r11.s64 = -2107703296;
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r6,r11,20240
	ctx.r6.s64 = ctx.r11.s64 + 20240;
	// li r4,1552
	ctx.r4.s64 = 1552;
	// addi r3,r31,-1552
	ctx.r3.s64 = ctx.r31.s64 + -1552;
	// bl 0x82120220
	ctx.lr = 0x829F4474;
	sub_82120220(ctx, base);
	// lis r11,-32161
	ctx.r11.s64 = -2107703296;
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r6,r11,23792
	ctx.r6.s64 = ctx.r11.s64 + 23792;
	// li r4,2496
	ctx.r4.s64 = 2496;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82120220
	ctx.lr = 0x829F448C;
	sub_82120220(ctx, base);
	// addi r3,r31,2500
	ctx.r3.s64 = ctx.r31.s64 + 2500;
	// bl 0x827ee2d8
	ctx.lr = 0x829F4494;
	sub_827EE2D8(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// addi r31,r31,4240
	ctx.r31.s64 = ctx.r31.s64 + 4240;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829f445c
	if (!ctx.cr6.lt) goto loc_829F445C;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4488
	ctx.r3.s64 = ctx.r11.s64 + -4488;
	// bl 0x8298ed98
	ctx.lr = 0x829F44B0;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F44C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F44C8) { __imp__sub_829F44C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F44C8) {
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
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,16672
	ctx.r3.s64 = ctx.r11.s64 + 16672;
	// bl 0x827f1320
	ctx.lr = 0x829F44E0;
	sub_827F1320(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4520
	ctx.r3.s64 = ctx.r11.s64 + -4520;
	// bl 0x8298ed98
	ctx.lr = 0x829F44EC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4500);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4500) { __imp__sub_829F4500(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4500) {
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
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,16704
	ctx.r3.s64 = ctx.r11.s64 + 16704;
	// bl 0x827f1320
	ctx.lr = 0x829F4518;
	sub_827F1320(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4504
	ctx.r3.s64 = ctx.r11.s64 + -4504;
	// bl 0x8298ed98
	ctx.lr = 0x829F4524;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4538);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4538) { __imp__sub_829F4538(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4538) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-2420
	ctx.r3.s64 = ctx.r11.s64 + -2420;
	// bl 0x827df490
	ctx.lr = 0x829F4558;
	sub_827DF490(ctx, base);
	// lis r10,-32003
	ctx.r10.s64 = -2097348608;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,17660
	ctx.r31.s64 = ctx.r10.s64 + 17660;
	// addi r11,r11,-2428
	ctx.r11.s64 = ctx.r11.s64 + -2428;
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F4578;
	sub_827DF490(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-2436
	ctx.r11.s64 = ctx.r11.s64 + -2436;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F4590;
	sub_827DF490(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-2440
	ctx.r11.s64 = ctx.r11.s64 + -2440;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F45A8;
	sub_827DF490(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-2448
	ctx.r11.s64 = ctx.r11.s64 + -2448;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829F45C0;
	sub_827DF490(ctx, base);
	// stw r3,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r3.u32);
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

PPC_FUNC_IMPL(__imp__sub_829F45D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F45D8) { __imp__sub_829F45D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F45D8) {
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
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17584
	ctx.r3.s64 = ctx.r11.s64 + 17584;
	// bl 0x8289df08
	ctx.lr = 0x829F45F0;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4384
	ctx.r3.s64 = ctx.r11.s64 + -4384;
	// bl 0x8298ed98
	ctx.lr = 0x829F45FC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4610);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4610) { __imp__sub_829F4610(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4610) {
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
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17624
	ctx.r3.s64 = ctx.r11.s64 + 17624;
	// bl 0x828a2de0
	ctx.lr = 0x829F4628;
	sub_828A2DE0(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4376
	ctx.r3.s64 = ctx.r11.s64 + -4376;
	// bl 0x8298ed98
	ctx.lr = 0x829F4634;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4648);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4648) { __imp__sub_829F4648(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4648) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4368
	ctx.r3.s64 = ctx.r11.s64 + -4368;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4658);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4658) { __imp__sub_829F4658(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4658) {
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
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// bl 0x82600908
	ctx.lr = 0x829F4670;
	sub_82600908(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4320
	ctx.r3.s64 = ctx.r11.s64 + -4320;
	// bl 0x8298ed98
	ctx.lr = 0x829F467C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4690);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4690) { __imp__sub_829F4690(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4690) {
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
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,32688
	ctx.r3.s64 = ctx.r11.s64 + 32688;
	// bl 0x8266ca60
	ctx.lr = 0x829F46A8;
	sub_8266CA60(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4336
	ctx.r3.s64 = ctx.r11.s64 + -4336;
	// bl 0x8298ed98
	ctx.lr = 0x829F46B4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F46C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F46C8) { __imp__sub_829F46C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F46C8) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-1876
	ctx.r3.s64 = ctx.r11.s64 + -1876;
	// bl 0x827df490
	ctx.lr = 0x829F46E4;
	sub_827DF490(ctx, base);
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// stw r3,17684(r11)
	PPC_STORE_U32(ctx.r11.u32 + 17684, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4700);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4700) { __imp__sub_829F4700(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4700) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-1844
	ctx.r3.s64 = ctx.r11.s64 + -1844;
	// bl 0x827df490
	ctx.lr = 0x829F471C;
	sub_827DF490(ctx, base);
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// stw r3,17688(r11)
	PPC_STORE_U32(ctx.r11.u32 + 17688, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4738);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4738) { __imp__sub_829F4738(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4738) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-1820
	ctx.r3.s64 = ctx.r11.s64 + -1820;
	// bl 0x827df490
	ctx.lr = 0x829F4754;
	sub_827DF490(ctx, base);
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// stw r3,17692(r11)
	PPC_STORE_U32(ctx.r11.u32 + 17692, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4770);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4770) { __imp__sub_829F4770(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4770) {
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
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,32712
	ctx.r3.s64 = ctx.r11.s64 + 32712;
	// bl 0x826019b8
	ctx.lr = 0x829F4788;
	sub_826019B8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4248
	ctx.r3.s64 = ctx.r11.s64 + -4248;
	// bl 0x8298ed98
	ctx.lr = 0x829F4794;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F47A8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F47A8) { __imp__sub_829F47A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F47A8) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,456
	ctx.r3.s64 = ctx.r11.s64 + 456;
	// bl 0x827df490
	ctx.lr = 0x829F47C4;
	sub_827DF490(ctx, base);
	// lis r11,-32002
	ctx.r11.s64 = -2097283072;
	// stw r3,-32612(r11)
	PPC_STORE_U32(ctx.r11.u32 + -32612, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F47E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F47E0) { __imp__sub_829F47E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F47E0) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,480
	ctx.r3.s64 = ctx.r11.s64 + 480;
	// bl 0x827df490
	ctx.lr = 0x829F47FC;
	sub_827DF490(ctx, base);
	// lis r11,-32002
	ctx.r11.s64 = -2097283072;
	// stw r3,-32608(r11)
	PPC_STORE_U32(ctx.r11.u32 + -32608, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4818);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4818) { __imp__sub_829F4818(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4818) {
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
	// lis r11,-32002
	ctx.r11.s64 = -2097283072;
	// addi r3,r11,-32560
	ctx.r3.s64 = ctx.r11.s64 + -32560;
	// bl 0x82602740
	ctx.lr = 0x829F4830;
	sub_82602740(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4176
	ctx.r3.s64 = ctx.r11.s64 + -4176;
	// bl 0x8298ed98
	ctx.lr = 0x829F483C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4850);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4850) { __imp__sub_829F4850(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4850) {
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
	// lis r11,-32002
	ctx.r11.s64 = -2097283072;
	// addi r3,r11,-32604
	ctx.r3.s64 = ctx.r11.s64 + -32604;
	// bl 0x8289df08
	ctx.lr = 0x829F4868;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4208
	ctx.r3.s64 = ctx.r11.s64 + -4208;
	// bl 0x8298ed98
	ctx.lr = 0x829F4874;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4888);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4888) { __imp__sub_829F4888(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4888) {
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
	// lis r11,-32002
	ctx.r11.s64 = -2097283072;
	// addi r3,r11,-26800
	ctx.r3.s64 = ctx.r11.s64 + -26800;
	// bl 0x8289df08
	ctx.lr = 0x829F48A0;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4216
	ctx.r3.s64 = ctx.r11.s64 + -4216;
	// bl 0x8298ed98
	ctx.lr = 0x829F48AC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F48C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F48C0) { __imp__sub_829F48C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F48C0) {
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
	// lis r11,-32002
	ctx.r11.s64 = -2097283072;
	// addi r3,r11,-26760
	ctx.r3.s64 = ctx.r11.s64 + -26760;
	// bl 0x8289df08
	ctx.lr = 0x829F48D8;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4224
	ctx.r3.s64 = ctx.r11.s64 + -4224;
	// bl 0x8298ed98
	ctx.lr = 0x829F48E4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F48F8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F48F8) { __imp__sub_829F48F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F48F8) {
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
	// lis r11,-32002
	ctx.r11.s64 = -2097283072;
	// addi r3,r11,-26720
	ctx.r3.s64 = ctx.r11.s64 + -26720;
	// bl 0x8289df08
	ctx.lr = 0x829F4910;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4192
	ctx.r3.s64 = ctx.r11.s64 + -4192;
	// bl 0x8298ed98
	ctx.lr = 0x829F491C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4930);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4930) { __imp__sub_829F4930(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4930) {
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
	// lis r11,-32002
	ctx.r11.s64 = -2097283072;
	// addi r3,r11,-26680
	ctx.r3.s64 = ctx.r11.s64 + -26680;
	// bl 0x8289df08
	ctx.lr = 0x829F4948;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4200
	ctx.r3.s64 = ctx.r11.s64 + -4200;
	// bl 0x8298ed98
	ctx.lr = 0x829F4954;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4968);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4968) { __imp__sub_829F4968(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4968) {
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
	// lis r11,-32002
	ctx.r11.s64 = -2097283072;
	// addi r3,r11,-26640
	ctx.r3.s64 = ctx.r11.s64 + -26640;
	// bl 0x8289df08
	ctx.lr = 0x829F4980;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4184
	ctx.r3.s64 = ctx.r11.s64 + -4184;
	// bl 0x8298ed98
	ctx.lr = 0x829F498C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F49A0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F49A0) { __imp__sub_829F49A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F49A0) {
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
	// lis r11,-32002
	ctx.r11.s64 = -2097283072;
	// addi r3,r11,-25412
	ctx.r3.s64 = ctx.r11.s64 + -25412;
	// bl 0x8289df08
	ctx.lr = 0x829F49B8;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4152
	ctx.r3.s64 = ctx.r11.s64 + -4152;
	// bl 0x8298ed98
	ctx.lr = 0x829F49C4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F49D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F49D8) { __imp__sub_829F49D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F49D8) {
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
	// lis r11,-32002
	ctx.r11.s64 = -2097283072;
	// addi r3,r11,-25372
	ctx.r3.s64 = ctx.r11.s64 + -25372;
	// bl 0x8289df08
	ctx.lr = 0x829F49F0;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4144
	ctx.r3.s64 = ctx.r11.s64 + -4144;
	// bl 0x8298ed98
	ctx.lr = 0x829F49FC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4A10);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4A10) { __imp__sub_829F4A10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4A10) {
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
	// lis r11,-32002
	ctx.r11.s64 = -2097283072;
	// addi r3,r11,-25332
	ctx.r3.s64 = ctx.r11.s64 + -25332;
	// bl 0x8289df08
	ctx.lr = 0x829F4A28;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4136
	ctx.r3.s64 = ctx.r11.s64 + -4136;
	// bl 0x8298ed98
	ctx.lr = 0x829F4A34;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4A48);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4A48) { __imp__sub_829F4A48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4A48) {
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
	// lis r11,-32002
	ctx.r11.s64 = -2097283072;
	// addi r3,r11,-25292
	ctx.r3.s64 = ctx.r11.s64 + -25292;
	// bl 0x8289df08
	ctx.lr = 0x829F4A60;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4128
	ctx.r3.s64 = ctx.r11.s64 + -4128;
	// bl 0x8298ed98
	ctx.lr = 0x829F4A6C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4A80);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4A80) { __imp__sub_829F4A80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4A80) {
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
	// lis r11,-32002
	ctx.r11.s64 = -2097283072;
	// addi r3,r11,-25252
	ctx.r3.s64 = ctx.r11.s64 + -25252;
	// bl 0x8289df08
	ctx.lr = 0x829F4A98;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4120
	ctx.r3.s64 = ctx.r11.s64 + -4120;
	// bl 0x8298ed98
	ctx.lr = 0x829F4AA4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4AB8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4AB8) { __imp__sub_829F4AB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4AB8) {
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
	// lis r11,-32002
	ctx.r11.s64 = -2097283072;
	// addi r3,r11,-25212
	ctx.r3.s64 = ctx.r11.s64 + -25212;
	// bl 0x8289df08
	ctx.lr = 0x829F4AD0;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4112
	ctx.r3.s64 = ctx.r11.s64 + -4112;
	// bl 0x8298ed98
	ctx.lr = 0x829F4ADC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4AF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4AF0) { __imp__sub_829F4AF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4AF0) {
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
	// lis r11,-32002
	ctx.r11.s64 = -2097283072;
	// addi r3,r11,-25172
	ctx.r3.s64 = ctx.r11.s64 + -25172;
	// bl 0x8289df08
	ctx.lr = 0x829F4B08;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4104
	ctx.r3.s64 = ctx.r11.s64 + -4104;
	// bl 0x8298ed98
	ctx.lr = 0x829F4B14;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4B28);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4B28) { __imp__sub_829F4B28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4B28) {
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
	// lis r11,-32002
	ctx.r11.s64 = -2097283072;
	// addi r3,r11,-25132
	ctx.r3.s64 = ctx.r11.s64 + -25132;
	// bl 0x8289df08
	ctx.lr = 0x829F4B40;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4096
	ctx.r3.s64 = ctx.r11.s64 + -4096;
	// bl 0x8298ed98
	ctx.lr = 0x829F4B4C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4B60);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4B60) { __imp__sub_829F4B60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4B60) {
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
	// lis r11,-32002
	ctx.r11.s64 = -2097283072;
	// addi r3,r11,-22960
	ctx.r3.s64 = ctx.r11.s64 + -22960;
	// bl 0x82608c60
	ctx.lr = 0x829F4B78;
	sub_82608C60(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4088
	ctx.r3.s64 = ctx.r11.s64 + -4088;
	// bl 0x8298ed98
	ctx.lr = 0x829F4B84;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4B98);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4B98) { __imp__sub_829F4B98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4B98) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,1540
	ctx.r3.s64 = ctx.r11.s64 + 1540;
	// bl 0x827df490
	ctx.lr = 0x829F4BB4;
	sub_827DF490(ctx, base);
	// lis r11,-31998
	ctx.r11.s64 = -2097020928;
	// stw r3,19376(r11)
	PPC_STORE_U32(ctx.r11.u32 + 19376, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4BD0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4BD0) { __imp__sub_829F4BD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4BD0) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-7868
	ctx.r3.s64 = ctx.r11.s64 + -7868;
	// bl 0x827df490
	ctx.lr = 0x829F4BEC;
	sub_827DF490(ctx, base);
	// lis r11,-31998
	ctx.r11.s64 = -2097020928;
	// stw r3,19460(r11)
	PPC_STORE_U32(ctx.r11.u32 + 19460, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4C08);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4C08) { __imp__sub_829F4C08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4C08) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,1552
	ctx.r3.s64 = ctx.r11.s64 + 1552;
	// bl 0x827df490
	ctx.lr = 0x829F4C24;
	sub_827DF490(ctx, base);
	// lis r11,-31998
	ctx.r11.s64 = -2097020928;
	// stw r3,19464(r11)
	PPC_STORE_U32(ctx.r11.u32 + 19464, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4C40);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4C40) { __imp__sub_829F4C40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4C40) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,3560
	ctx.r3.s64 = ctx.r11.s64 + 3560;
	// bl 0x827df490
	ctx.lr = 0x829F4C5C;
	sub_827DF490(ctx, base);
	// lis r11,-31998
	ctx.r11.s64 = -2097020928;
	// stw r3,19600(r11)
	PPC_STORE_U32(ctx.r11.u32 + 19600, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4C78);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4C78) { __imp__sub_829F4C78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4C78) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,3564
	ctx.r3.s64 = ctx.r11.s64 + 3564;
	// bl 0x827df490
	ctx.lr = 0x829F4C94;
	sub_827DF490(ctx, base);
	// lis r11,-31998
	ctx.r11.s64 = -2097020928;
	// stw r3,19604(r11)
	PPC_STORE_U32(ctx.r11.u32 + 19604, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4CB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4CB0) { __imp__sub_829F4CB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4CB0) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,3576
	ctx.r3.s64 = ctx.r11.s64 + 3576;
	// bl 0x827df490
	ctx.lr = 0x829F4CCC;
	sub_827DF490(ctx, base);
	// lis r11,-31998
	ctx.r11.s64 = -2097020928;
	// stw r3,19608(r11)
	PPC_STORE_U32(ctx.r11.u32 + 19608, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4CE8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4CE8) { __imp__sub_829F4CE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4CE8) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,3588
	ctx.r3.s64 = ctx.r11.s64 + 3588;
	// bl 0x827df490
	ctx.lr = 0x829F4D04;
	sub_827DF490(ctx, base);
	// lis r11,-31998
	ctx.r11.s64 = -2097020928;
	// stw r3,19612(r11)
	PPC_STORE_U32(ctx.r11.u32 + 19612, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4D20);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4D20) { __imp__sub_829F4D20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4D20) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,3600
	ctx.r3.s64 = ctx.r11.s64 + 3600;
	// bl 0x827df490
	ctx.lr = 0x829F4D3C;
	sub_827DF490(ctx, base);
	// lis r11,-31998
	ctx.r11.s64 = -2097020928;
	// stw r3,19616(r11)
	PPC_STORE_U32(ctx.r11.u32 + 19616, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4D58);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4D58) { __imp__sub_829F4D58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4D58) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,3612
	ctx.r3.s64 = ctx.r11.s64 + 3612;
	// bl 0x827df490
	ctx.lr = 0x829F4D74;
	sub_827DF490(ctx, base);
	// lis r11,-31998
	ctx.r11.s64 = -2097020928;
	// stw r3,19620(r11)
	PPC_STORE_U32(ctx.r11.u32 + 19620, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4D90);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4D90) { __imp__sub_829F4D90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4D90) {
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
	// lis r11,-31998
	ctx.r11.s64 = -2097020928;
	// addi r31,r11,19776
	ctx.r31.s64 = ctx.r11.s64 + 19776;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x823cb140
	ctx.lr = 0x829F4DB0;
	sub_823CB140(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r11,r11,4740
	ctx.r11.s64 = ctx.r11.s64 + 4740;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8261f8f0
	ctx.lr = 0x829F4DC4;
	sub_8261F8F0(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-4072
	ctx.r3.s64 = ctx.r11.s64 + -4072;
	// bl 0x8298ed98
	ctx.lr = 0x829F4DD0;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F4DE8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4DE8) { __imp__sub_829F4DE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4DE8) {
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
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// addi r3,r11,-30720
	ctx.r3.s64 = ctx.r11.s64 + -30720;
	// bl 0x827f1320
	ctx.lr = 0x829F4E00;
	sub_827F1320(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3976
	ctx.r3.s64 = ctx.r11.s64 + -3976;
	// bl 0x8298ed98
	ctx.lr = 0x829F4E0C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4E20);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4E20) { __imp__sub_829F4E20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4E20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// addi r3,r11,-30688
	ctx.r3.s64 = ctx.r11.s64 + -30688;
	// b 0x826266a8
	sub_826266A8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4E30);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4E30) { __imp__sub_829F4E30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4E30) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3960
	ctx.r3.s64 = ctx.r11.s64 + -3960;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4E40);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4E40) { __imp__sub_829F4E40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4E40) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,6508
	ctx.r3.s64 = ctx.r11.s64 + 6508;
	// bl 0x827df490
	ctx.lr = 0x829F4E5C;
	sub_827DF490(ctx, base);
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// stw r3,-27408(r11)
	PPC_STORE_U32(ctx.r11.u32 + -27408, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4E78);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4E78) { __imp__sub_829F4E78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4E78) {
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
	// li r3,3072
	ctx.r3.s64 = 3072;
	// bl 0x8218be28
	ctx.lr = 0x829F4E8C;
	sub_8218BE28(ctx, base);
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// addi r3,r10,-3928
	ctx.r3.s64 = ctx.r10.s64 + -3928;
	// lis r10,-32092
	ctx.r10.s64 = -2103181312;
	// stw r11,-10708(r10)
	PPC_STORE_U32(ctx.r10.u32 + -10708, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F4EA4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4EB8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4EB8) { __imp__sub_829F4EB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4EB8) {
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
	// li r3,3072
	ctx.r3.s64 = 3072;
	// bl 0x8218be28
	ctx.lr = 0x829F4ECC;
	sub_8218BE28(ctx, base);
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// addi r3,r10,-3896
	ctx.r3.s64 = ctx.r10.s64 + -3896;
	// lis r10,-32092
	ctx.r10.s64 = -2103181312;
	// stw r11,-10700(r10)
	PPC_STORE_U32(ctx.r10.u32 + -10700, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F4EE4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4EF8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4EF8) { __imp__sub_829F4EF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4EF8) {
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
	// li r3,256
	ctx.r3.s64 = 256;
	// bl 0x8218be28
	ctx.lr = 0x829F4F0C;
	sub_8218BE28(ctx, base);
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// addi r3,r10,-3864
	ctx.r3.s64 = ctx.r10.s64 + -3864;
	// lis r10,-32092
	ctx.r10.s64 = -2103181312;
	// stw r11,-10692(r10)
	PPC_STORE_U32(ctx.r10.u32 + -10692, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F4F24;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4F38);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4F38) { __imp__sub_829F4F38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4F38) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8544
	ctx.r11.s64 = ctx.r11.s64 + -8544;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// addi r11,r11,-10656
	ctx.r11.s64 = ctx.r11.s64 + -10656;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4F58);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4F58) { __imp__sub_829F4F58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4F58) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31997
	ctx.r10.s64 = -2096955392;
	// li r11,-1
	ctx.r11.s64 = -1;
	// stw r11,-10628(r10)
	PPC_STORE_U32(ctx.r10.u32 + -10628, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4F68);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4F68) { __imp__sub_829F4F68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4F68) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3800
	ctx.r3.s64 = ctx.r11.s64 + -3800;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4F78);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4F78) { __imp__sub_829F4F78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4F78) {
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
	// li r3,80
	ctx.r3.s64 = 80;
	// bl 0x8218be28
	ctx.lr = 0x829F4F8C;
	sub_8218BE28(ctx, base);
	// lis r10,-32092
	ctx.r10.s64 = -2103181312;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// stw r3,-9832(r10)
	PPC_STORE_U32(ctx.r10.u32 + -9832, ctx.r3.u32);
	// addi r3,r11,-3832
	ctx.r3.s64 = ctx.r11.s64 + -3832;
	// bl 0x8298ed98
	ctx.lr = 0x829F4FA0;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4FB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4FB0) { __imp__sub_829F4FB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4FB0) {
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
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// addi r3,r11,-9728
	ctx.r3.s64 = ctx.r11.s64 + -9728;
	// bl 0x8289df08
	ctx.lr = 0x829F4FC8;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3776
	ctx.r3.s64 = ctx.r11.s64 + -3776;
	// bl 0x8298ed98
	ctx.lr = 0x829F4FD4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F4FE8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F4FE8) { __imp__sub_829F4FE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F4FE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r11,r11,-9552
	ctx.r11.s64 = ctx.r11.s64 + -9552;
	// li r10,24
	ctx.r10.s64 = 24;
	// addi r11,r11,400
	ctx.r11.s64 = ctx.r11.s64 + 400;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_829F5000:
	// stb r9,0(r11)
	PPC_STORE_U8(ctx.r11.u32 + 0, ctx.r9.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bdnz 0x829f5000
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_829F5000;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3768
	ctx.r3.s64 = ctx.r11.s64 + -3768;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5018);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5018) { __imp__sub_829F5018(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5018) {
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
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// addi r3,r11,-9120
	ctx.r3.s64 = ctx.r11.s64 + -9120;
	// bl 0x8235a4f0
	ctx.lr = 0x829F5030;
	sub_8235A4F0(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3760
	ctx.r3.s64 = ctx.r11.s64 + -3760;
	// bl 0x8298ed98
	ctx.lr = 0x829F503C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5050);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5050) { __imp__sub_829F5050(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5050) {
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
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// addi r3,r11,-8992
	ctx.r3.s64 = ctx.r11.s64 + -8992;
	// bl 0x8235a4f0
	ctx.lr = 0x829F5068;
	sub_8235A4F0(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3752
	ctx.r3.s64 = ctx.r11.s64 + -3752;
	// bl 0x8298ed98
	ctx.lr = 0x829F5074;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5088);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5088) { __imp__sub_829F5088(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5088) {
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
	// lis r11,-31995
	ctx.r11.s64 = -2096824320;
	// addi r3,r11,16792
	ctx.r3.s64 = ctx.r11.s64 + 16792;
	// bl 0x8297b980
	ctx.lr = 0x829F50A0;
	sub_8297B980(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3744
	ctx.r3.s64 = ctx.r11.s64 + -3744;
	// bl 0x8298ed98
	ctx.lr = 0x829F50AC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F50C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F50C0) { __imp__sub_829F50C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F50C0) {
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
	// lis r11,-31996
	ctx.r11.s64 = -2096889856;
	// addi r3,r11,24464
	ctx.r3.s64 = ctx.r11.s64 + 24464;
	// bl 0x82979d50
	ctx.lr = 0x829F50D8;
	sub_82979D50(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3728
	ctx.r3.s64 = ctx.r11.s64 + -3728;
	// bl 0x8298ed98
	ctx.lr = 0x829F50E4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F50F8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F50F8) { __imp__sub_829F50F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F50F8) {
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
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// lis r5,1
	ctx.r5.s64 = 65536;
	// addi r4,r11,-8792
	ctx.r4.s64 = ctx.r11.s64 + -8792;
	// lis r11,-31996
	ctx.r11.s64 = -2096889856;
	// li r6,1
	ctx.r6.s64 = 1;
	// addi r3,r11,26904
	ctx.r3.s64 = ctx.r11.s64 + 26904;
	// ori r5,r5,32768
	ctx.r5.u64 = ctx.r5.u64 | 32768;
	// bl 0x827daac8
	ctx.lr = 0x829F5124;
	sub_827DAAC8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3712
	ctx.r3.s64 = ctx.r11.s64 + -3712;
	// bl 0x8298ed98
	ctx.lr = 0x829F5130;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5140);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5140) { __imp__sub_829F5140(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5140) {
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
	// lis r11,-31996
	ctx.r11.s64 = -2096889856;
	// addi r3,r11,24372
	ctx.r3.s64 = ctx.r11.s64 + 24372;
	// bl 0x829778a8
	ctx.lr = 0x829F5158;
	sub_829778A8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3696
	ctx.r3.s64 = ctx.r11.s64 + -3696;
	// bl 0x8298ed98
	ctx.lr = 0x829F5164;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5178);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5178) { __imp__sub_829F5178(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5178) {
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
	// lis r11,-31996
	ctx.r11.s64 = -2096889856;
	// addi r3,r11,24808
	ctx.r3.s64 = ctx.r11.s64 + 24808;
	// bl 0x8297ac60
	ctx.lr = 0x829F5190;
	sub_8297AC60(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3680
	ctx.r3.s64 = ctx.r11.s64 + -3680;
	// bl 0x8298ed98
	ctx.lr = 0x829F519C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F51B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F51B0) { __imp__sub_829F51B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F51B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31995
	ctx.r11.s64 = -2096824320;
	// addi r3,r11,4984
	ctx.r3.s64 = ctx.r11.s64 + 4984;
	// b 0x8296c2f0
	sub_8296C2F0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F51C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F51C0) { __imp__sub_829F51C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F51C0) {
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
	// lis r11,-31996
	ctx.r11.s64 = -2096889856;
	// addi r3,r11,24832
	ctx.r3.s64 = ctx.r11.s64 + 24832;
	// bl 0x82975a10
	ctx.lr = 0x829F51D8;
	sub_82975A10(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3664
	ctx.r3.s64 = ctx.r11.s64 + -3664;
	// bl 0x8298ed98
	ctx.lr = 0x829F51E4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F51F8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F51F8) { __imp__sub_829F51F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F51F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3352
	ctx.r3.s64 = ctx.r11.s64 + -3352;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5208);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5208) { __imp__sub_829F5208(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5208) {
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
	// lis r11,-31995
	ctx.r11.s64 = -2096824320;
	// addi r3,r11,-16736
	ctx.r3.s64 = ctx.r11.s64 + -16736;
	// bl 0x8297b470
	ctx.lr = 0x829F5220;
	sub_8297B470(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3648
	ctx.r3.s64 = ctx.r11.s64 + -3648;
	// bl 0x8298ed98
	ctx.lr = 0x829F522C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5240);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5240) { __imp__sub_829F5240(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5240) {
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
	// lis r11,-31996
	ctx.r11.s64 = -2096889856;
	// li r31,99
	ctx.r31.s64 = 99;
	// addi r30,r11,29200
	ctx.r30.s64 = ctx.r11.s64 + 29200;
loc_829F5260:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82979348
	ctx.lr = 0x829F5268;
	sub_82979348(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,196
	ctx.r30.s64 = ctx.r30.s64 + 196;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x829f5260
	if (!ctx.cr6.lt) goto loc_829F5260;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3632
	ctx.r3.s64 = ctx.r11.s64 + -3632;
	// bl 0x8298ed98
	ctx.lr = 0x829F5284;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F52A0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F52A0) { __imp__sub_829F52A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F52A0) {
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
	// lis r11,-31995
	ctx.r11.s64 = -2096824320;
	// li r31,99
	ctx.r31.s64 = 99;
	// addi r30,r11,-14616
	ctx.r30.s64 = ctx.r11.s64 + -14616;
loc_829F52C0:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82979348
	ctx.lr = 0x829F52C8;
	sub_82979348(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,196
	ctx.r30.s64 = ctx.r30.s64 + 196;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x829f52c0
	if (!ctx.cr6.lt) goto loc_829F52C0;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3544
	ctx.r3.s64 = ctx.r11.s64 + -3544;
	// bl 0x8298ed98
	ctx.lr = 0x829F52E4;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F5300);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5300) { __imp__sub_829F5300(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5300) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3456
	ctx.r3.s64 = ctx.r11.s64 + -3456;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5310);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5310) { __imp__sub_829F5310(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5310) {
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
	// lis r11,-31996
	ctx.r11.s64 = -2096889856;
	// addi r31,r11,24616
	ctx.r31.s64 = ctx.r11.s64 + 24616;
	// addi r3,r31,4
	ctx.r3.s64 = ctx.r31.s64 + 4;
	// bl 0x82979ec0
	ctx.lr = 0x829F5330;
	sub_82979EC0(ctx, base);
	// addi r3,r31,64
	ctx.r3.s64 = ctx.r31.s64 + 64;
	// bl 0x8296c2f0
	ctx.lr = 0x829F5338;
	sub_8296C2F0(ctx, base);
	// addi r3,r31,160
	ctx.r3.s64 = ctx.r31.s64 + 160;
	// bl 0x829745b0
	ctx.lr = 0x829F5340;
	sub_829745B0(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// addi r3,r31,4
	ctx.r3.s64 = ctx.r31.s64 + 4;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82979e30
	ctx.lr = 0x829F5350;
	sub_82979E30(ctx, base);
	// addi r3,r31,64
	ctx.r3.s64 = ctx.r31.s64 + 64;
	// bl 0x8296c060
	ctx.lr = 0x829F5358;
	sub_8296C060(ctx, base);
	// addi r3,r31,160
	ctx.r3.s64 = ctx.r31.s64 + 160;
	// bl 0x829743a8
	ctx.lr = 0x829F5360;
	sub_829743A8(ctx, base);
	// li r11,-1
	ctx.r11.s64 = -1;
	// stw r11,180(r31)
	PPC_STORE_U32(ctx.r31.u32 + 180, ctx.r11.u32);
	// stw r11,176(r31)
	PPC_STORE_U32(ctx.r31.u32 + 176, ctx.r11.u32);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,184(r31)
	PPC_STORE_U32(ctx.r31.u32 + 184, ctx.r11.u32);
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

PPC_FUNC_IMPL(__imp__sub_829F5388);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5388) { __imp__sub_829F5388(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5388) {
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
	// lis r11,-31995
	ctx.r11.s64 = -2096824320;
	// addi r31,r11,5080
	ctx.r31.s64 = ctx.r11.s64 + 5080;
	// addi r3,r31,4
	ctx.r3.s64 = ctx.r31.s64 + 4;
	// bl 0x82979ec0
	ctx.lr = 0x829F53A8;
	sub_82979EC0(ctx, base);
	// addi r3,r31,64
	ctx.r3.s64 = ctx.r31.s64 + 64;
	// bl 0x8296c2f0
	ctx.lr = 0x829F53B0;
	sub_8296C2F0(ctx, base);
	// addi r3,r31,160
	ctx.r3.s64 = ctx.r31.s64 + 160;
	// bl 0x829745b0
	ctx.lr = 0x829F53B8;
	sub_829745B0(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// addi r3,r31,4
	ctx.r3.s64 = ctx.r31.s64 + 4;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82979e30
	ctx.lr = 0x829F53C8;
	sub_82979E30(ctx, base);
	// addi r3,r31,64
	ctx.r3.s64 = ctx.r31.s64 + 64;
	// bl 0x8296c060
	ctx.lr = 0x829F53D0;
	sub_8296C060(ctx, base);
	// addi r3,r31,160
	ctx.r3.s64 = ctx.r31.s64 + 160;
	// bl 0x829743a8
	ctx.lr = 0x829F53D8;
	sub_829743A8(ctx, base);
	// li r11,-1
	ctx.r11.s64 = -1;
	// stw r11,180(r31)
	PPC_STORE_U32(ctx.r31.u32 + 180, ctx.r11.u32);
	// stw r11,176(r31)
	PPC_STORE_U32(ctx.r31.u32 + 176, ctx.r11.u32);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,184(r31)
	PPC_STORE_U32(ctx.r31.u32 + 184, ctx.r11.u32);
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

PPC_FUNC_IMPL(__imp__sub_829F5400);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5400) { __imp__sub_829F5400(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5400) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x829F5408;
	__savegprlr_27(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31995
	ctx.r11.s64 = -2096824320;
	// li r29,7
	ctx.r29.s64 = 7;
	// addi r11,r11,-16616
	ctx.r11.s64 = ctx.r11.s64 + -16616;
	// li r30,0
	ctx.r30.s64 = 0;
	// addi r31,r11,64
	ctx.r31.s64 = ctx.r11.s64 + 64;
	// li r27,-1
	ctx.r27.s64 = -1;
loc_829F5424:
	// addi r28,r31,-64
	ctx.r28.s64 = ctx.r31.s64 + -64;
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// bl 0x82979ec0
	ctx.lr = 0x829F5430;
	sub_82979EC0(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829745b0
	ctx.lr = 0x829F5438;
	sub_829745B0(ctx, base);
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// stw r30,172(r31)
	PPC_STORE_U32(ctx.r31.u32 + 172, ctx.r30.u32);
	// stw r30,176(r31)
	PPC_STORE_U32(ctx.r31.u32 + 176, ctx.r30.u32);
	// bl 0x82979e30
	ctx.lr = 0x829F5448;
	sub_82979E30(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829743a8
	ctx.lr = 0x829F5450;
	sub_829743A8(ctx, base);
	// addi r29,r29,-1
	ctx.r29.s64 = ctx.r29.s64 + -1;
	// stw r27,20(r31)
	PPC_STORE_U32(ctx.r31.u32 + 20, ctx.r27.u32);
	// stw r27,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r27.u32);
	// cmpwi cr6,r29,0
	ctx.cr6.compare<int32_t>(ctx.r29.s32, 0, ctx.xer);
	// stb r30,24(r31)
	PPC_STORE_U8(ctx.r31.u32 + 24, ctx.r30.u8);
	// stb r30,40(r31)
	PPC_STORE_U8(ctx.r31.u32 + 40, ctx.r30.u8);
	// stw r30,168(r31)
	PPC_STORE_U32(ctx.r31.u32 + 168, ctx.r30.u32);
	// addi r31,r31,248
	ctx.r31.s64 = ctx.r31.s64 + 248;
	// bge cr6,0x829f5424
	if (!ctx.cr6.lt) goto loc_829F5424;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5480);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5480) { __imp__sub_829F5480(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5480) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3344
	ctx.r3.s64 = ctx.r11.s64 + -3344;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5490);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5490) { __imp__sub_829F5490(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5490) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3448
	ctx.r3.s64 = ctx.r11.s64 + -3448;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F54A0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F54A0) { __imp__sub_829F54A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F54A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31996
	ctx.r11.s64 = -2096889856;
	// addi r3,r11,24428
	ctx.r3.s64 = ctx.r11.s64 + 24428;
	// b 0x829734f0
	sub_829734F0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F54B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F54B0) { __imp__sub_829F54B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F54B0) {
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
	// lis r11,-31995
	ctx.r11.s64 = -2096824320;
	// li r31,79
	ctx.r31.s64 = 79;
	// addi r30,r11,5272
	ctx.r30.s64 = ctx.r11.s64 + 5272;
loc_829F54D0:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8297a5c0
	ctx.lr = 0x829F54D8;
	sub_8297A5C0(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,144
	ctx.r30.s64 = ctx.r30.s64 + 144;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x829f54d0
	if (!ctx.cr6.lt) goto loc_829F54D0;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3440
	ctx.r3.s64 = ctx.r11.s64 + -3440;
	// bl 0x8298ed98
	ctx.lr = 0x829F54F4;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F5510);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5510) { __imp__sub_829F5510(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5510) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,29204
	ctx.r5.s64 = ctx.r11.s64 + 29204;
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,25060
	ctx.r3.s64 = ctx.r11.s64 + 25060;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5530);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5530) { __imp__sub_829F5530(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5530) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,29216
	ctx.r5.s64 = ctx.r11.s64 + 29216;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-31380
	ctx.r3.s64 = ctx.r11.s64 + -31380;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5550);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5550) { __imp__sub_829F5550(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5550) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,29228
	ctx.r5.s64 = ctx.r11.s64 + 29228;
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,24680
	ctx.r3.s64 = ctx.r11.s64 + 24680;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5570);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5570) { __imp__sub_829F5570(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5570) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,29236
	ctx.r5.s64 = ctx.r11.s64 + 29236;
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,25472
	ctx.r3.s64 = ctx.r11.s64 + 25472;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5590);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5590) { __imp__sub_829F5590(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5590) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,29248
	ctx.r5.s64 = ctx.r11.s64 + 29248;
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,21112
	ctx.r3.s64 = ctx.r11.s64 + 21112;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F55B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F55B0) { __imp__sub_829F55B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F55B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,29268
	ctx.r5.s64 = ctx.r11.s64 + 29268;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-31332
	ctx.r3.s64 = ctx.r11.s64 + -31332;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F55D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F55D0) { __imp__sub_829F55D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F55D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,29272
	ctx.r5.s64 = ctx.r11.s64 + 29272;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-32732
	ctx.r3.s64 = ctx.r11.s64 + -32732;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F55F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F55F0) { __imp__sub_829F55F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F55F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,29284
	ctx.r5.s64 = ctx.r11.s64 + 29284;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-31012
	ctx.r3.s64 = ctx.r11.s64 + -31012;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5610);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5610) { __imp__sub_829F5610(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5610) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r5,r11,29040
	ctx.r5.s64 = ctx.r11.s64 + 29040;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,-32768
	ctx.r31.s64 = ctx.r11.s64 + -32768;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F5644;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F5654;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F565C;
	sub_8296C788(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,29036
	ctx.r11.s64 = ctx.r11.s64 + 29036;
	// addi r3,r10,-2944
	ctx.r3.s64 = ctx.r10.s64 + -2944;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F5674;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F5688);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5688) { __imp__sub_829F5688(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5688) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r5,r11,29064
	ctx.r5.s64 = ctx.r11.s64 + 29064;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,-31312
	ctx.r31.s64 = ctx.r11.s64 + -31312;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F56BC;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F56CC;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F56D4;
	sub_8296C788(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,29060
	ctx.r11.s64 = ctx.r11.s64 + 29060;
	// addi r3,r10,-2864
	ctx.r3.s64 = ctx.r10.s64 + -2864;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F56EC;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F5700);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5700) { __imp__sub_829F5700(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5700) {
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
	// lis r11,-31994
	ctx.r11.s64 = -2096758784;
	// li r6,1
	ctx.r6.s64 = 1;
	// addi r4,r11,16976
	ctx.r4.s64 = ctx.r11.s64 + 16976;
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// lis r5,2
	ctx.r5.s64 = 131072;
	// addi r3,r11,20080
	ctx.r3.s64 = ctx.r11.s64 + 20080;
	// bl 0x827daac8
	ctx.lr = 0x829F5728;
	sub_827DAAC8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3328
	ctx.r3.s64 = ctx.r11.s64 + -3328;
	// bl 0x8298ed98
	ctx.lr = 0x829F5734;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5748);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5748) { __imp__sub_829F5748(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5748) {
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
	// lis r11,-31995
	ctx.r11.s64 = -2096824320;
	// li r6,1
	ctx.r6.s64 = 1;
	// addi r4,r11,16848
	ctx.r4.s64 = ctx.r11.s64 + 16848;
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// lis r5,1
	ctx.r5.s64 = 65536;
	// addi r3,r11,22384
	ctx.r3.s64 = ctx.r11.s64 + 22384;
	// bl 0x827daac8
	ctx.lr = 0x829F5770;
	sub_827DAAC8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3312
	ctx.r3.s64 = ctx.r11.s64 + -3312;
	// bl 0x8298ed98
	ctx.lr = 0x829F577C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5790);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5790) { __imp__sub_829F5790(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5790) {
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
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,-30956
	ctx.r3.s64 = ctx.r11.s64 + -30956;
	// bl 0x82976428
	ctx.lr = 0x829F57A8;
	sub_82976428(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3296
	ctx.r3.s64 = ctx.r11.s64 + -3296;
	// bl 0x8298ed98
	ctx.lr = 0x829F57B4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F57C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F57C8) { __imp__sub_829F57C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F57C8) {
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
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,-32712
	ctx.r3.s64 = ctx.r11.s64 + -32712;
	// bl 0x8297e1e8
	ctx.lr = 0x829F57E0;
	sub_8297E1E8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3280
	ctx.r3.s64 = ctx.r11.s64 + -3280;
	// bl 0x8298ed98
	ctx.lr = 0x829F57EC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5800);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5800) { __imp__sub_829F5800(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5800) {
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
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// addi r3,r11,25492
	ctx.r3.s64 = ctx.r11.s64 + 25492;
	// bl 0x8268a138
	ctx.lr = 0x829F5818;
	sub_8268A138(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3264
	ctx.r3.s64 = ctx.r11.s64 + -3264;
	// bl 0x8298ed98
	ctx.lr = 0x829F5824;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5838);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5838) { __imp__sub_829F5838(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5838) {
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
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// addi r3,r11,22588
	ctx.r3.s64 = ctx.r11.s64 + 22588;
	// bl 0x8268dd38
	ctx.lr = 0x829F5850;
	sub_8268DD38(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3248
	ctx.r3.s64 = ctx.r11.s64 + -3248;
	// bl 0x8298ed98
	ctx.lr = 0x829F585C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5870);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5870) { __imp__sub_829F5870(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5870) {
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
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,-30872
	ctx.r3.s64 = ctx.r11.s64 + -30872;
	// bl 0x8268b1a8
	ctx.lr = 0x829F5888;
	sub_8268B1A8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3232
	ctx.r3.s64 = ctx.r11.s64 + -3232;
	// bl 0x8298ed98
	ctx.lr = 0x829F5894;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F58A8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F58A8) { __imp__sub_829F58A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F58A8) {
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
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// addi r3,r11,21152
	ctx.r3.s64 = ctx.r11.s64 + 21152;
	// bl 0x82970030
	ctx.lr = 0x829F58C0;
	sub_82970030(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3216
	ctx.r3.s64 = ctx.r11.s64 + -3216;
	// bl 0x8298ed98
	ctx.lr = 0x829F58CC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F58E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F58E0) { __imp__sub_829F58E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F58E0) {
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
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,-31408
	ctx.r3.s64 = ctx.r11.s64 + -31408;
	// bl 0x8296efa0
	ctx.lr = 0x829F58F8;
	sub_8296EFA0(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3200
	ctx.r3.s64 = ctx.r11.s64 + -3200;
	// bl 0x8298ed98
	ctx.lr = 0x829F5904;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5918);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5918) { __imp__sub_829F5918(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5918) {
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
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// addi r3,r11,12180
	ctx.r3.s64 = ctx.r11.s64 + 12180;
	// bl 0x8296efa0
	ctx.lr = 0x829F5930;
	sub_8296EFA0(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3184
	ctx.r3.s64 = ctx.r11.s64 + -3184;
	// bl 0x8298ed98
	ctx.lr = 0x829F593C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5950);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5950) { __imp__sub_829F5950(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5950) {
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
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,-31360
	ctx.r3.s64 = ctx.r11.s64 + -31360;
	// bl 0x8296efa0
	ctx.lr = 0x829F5968;
	sub_8296EFA0(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3168
	ctx.r3.s64 = ctx.r11.s64 + -3168;
	// bl 0x8298ed98
	ctx.lr = 0x829F5974;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5988);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5988) { __imp__sub_829F5988(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5988) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-2992
	ctx.r3.s64 = ctx.r11.s64 + -2992;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5998);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5998) { __imp__sub_829F5998(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5998) {
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
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// addi r3,r11,12208
	ctx.r3.s64 = ctx.r11.s64 + 12208;
	// bl 0x826aa968
	ctx.lr = 0x829F59B0;
	sub_826AA968(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3152
	ctx.r3.s64 = ctx.r11.s64 + -3152;
	// bl 0x8298ed98
	ctx.lr = 0x829F59BC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F59D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F59D0) { __imp__sub_829F59D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F59D0) {
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
	// lis r11,-31986
	ctx.r11.s64 = -2096234496;
	// addi r3,r11,31816
	ctx.r3.s64 = ctx.r11.s64 + 31816;
	// bl 0x8297c7a8
	ctx.lr = 0x829F59E8;
	sub_8297C7A8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3136
	ctx.r3.s64 = ctx.r11.s64 + -3136;
	// bl 0x8298ed98
	ctx.lr = 0x829F59F4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5A08);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5A08) { __imp__sub_829F5A08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5A08) {
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
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,-30288
	ctx.r3.s64 = ctx.r11.s64 + -30288;
	// bl 0x826a6530
	ctx.lr = 0x829F5A20;
	sub_826A6530(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-2784
	ctx.r3.s64 = ctx.r11.s64 + -2784;
	// bl 0x8298ed98
	ctx.lr = 0x829F5A2C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5A40);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5A40) { __imp__sub_829F5A40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5A40) {
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
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// li r5,36
	ctx.r5.s64 = 36;
	// addi r31,r11,24756
	ctx.r31.s64 = ctx.r11.s64 + 24756;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829a12b0
	ctx.lr = 0x829F5A68;
	sub_829A12B0(ctx, base);
	// li r11,-1
	ctx.r11.s64 = -1;
	// stw r11,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r11.u32);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,28(r31)
	PPC_STORE_U32(ctx.r31.u32 + 28, ctx.r11.u32);
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

PPC_FUNC_IMPL(__imp__sub_829F5A90);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5A90) { __imp__sub_829F5A90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5A90) {
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
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r5,36
	ctx.r5.s64 = 36;
	// addi r31,r11,-30992
	ctx.r31.s64 = ctx.r11.s64 + -30992;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829a12b0
	ctx.lr = 0x829F5AB8;
	sub_829A12B0(ctx, base);
	// li r11,-1
	ctx.r11.s64 = -1;
	// stw r11,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r11.u32);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,28(r31)
	PPC_STORE_U32(ctx.r31.u32 + 28, ctx.r11.u32);
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

PPC_FUNC_IMPL(__imp__sub_829F5AE0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5AE0) { __imp__sub_829F5AE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5AE0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3120
	ctx.r3.s64 = ctx.r11.s64 + -3120;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5AF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5AF0) { __imp__sub_829F5AF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5AF0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3112
	ctx.r3.s64 = ctx.r11.s64 + -3112;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5B00);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5B00) { __imp__sub_829F5B00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5B00) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r4,r11,29300
	ctx.r4.s64 = ctx.r11.s64 + 29300;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,664
	ctx.r3.s64 = ctx.r11.s64 + 664;
	// bl 0x828e0ab8
	ctx.lr = 0x829F5B20;
	sub_828E0AB8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3104
	ctx.r3.s64 = ctx.r11.s64 + -3104;
	// bl 0x8298ed98
	ctx.lr = 0x829F5B2C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5B40);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5B40) { __imp__sub_829F5B40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5B40) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r4,r11,29312
	ctx.r4.s64 = ctx.r11.s64 + 29312;
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// addi r3,r11,24792
	ctx.r3.s64 = ctx.r11.s64 + 24792;
	// bl 0x828e0ab8
	ctx.lr = 0x829F5B60;
	sub_828E0AB8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3088
	ctx.r3.s64 = ctx.r11.s64 + -3088;
	// bl 0x8298ed98
	ctx.lr = 0x829F5B6C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5B80);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5B80) { __imp__sub_829F5B80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5B80) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r4,r11,29328
	ctx.r4.s64 = ctx.r11.s64 + 29328;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,-31280
	ctx.r3.s64 = ctx.r11.s64 + -31280;
	// bl 0x828e0ab8
	ctx.lr = 0x829F5BA0;
	sub_828E0AB8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3072
	ctx.r3.s64 = ctx.r11.s64 + -3072;
	// bl 0x8298ed98
	ctx.lr = 0x829F5BAC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5BC0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5BC0) { __imp__sub_829F5BC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5BC0) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r4,r11,29348
	ctx.r4.s64 = ctx.r11.s64 + 29348;
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// addi r3,r11,11912
	ctx.r3.s64 = ctx.r11.s64 + 11912;
	// bl 0x828e0ab8
	ctx.lr = 0x829F5BE0;
	sub_828E0AB8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3056
	ctx.r3.s64 = ctx.r11.s64 + -3056;
	// bl 0x8298ed98
	ctx.lr = 0x829F5BEC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5C00);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5C00) { __imp__sub_829F5C00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5C00) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lwz r11,28084(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28084);
	// stw r11,-32736(r10)
	PPC_STORE_U32(ctx.r10.u32 + -32736, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5C18);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5C18) { __imp__sub_829F5C18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5C18) {
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
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// addi r31,r11,25336
	ctx.r31.s64 = ctx.r11.s64 + 25336;
	// addi r3,r31,4
	ctx.r3.s64 = ctx.r31.s64 + 4;
	// bl 0x82979ec0
	ctx.lr = 0x829F5C38;
	sub_82979EC0(ctx, base);
	// li r11,-1
	ctx.r11.s64 = -1;
	// addi r3,r31,4
	ctx.r3.s64 = ctx.r31.s64 + 4;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82979e30
	ctx.lr = 0x829F5C48;
	sub_82979E30(ctx, base);
	// li r11,-1
	ctx.r11.s64 = -1;
	// stw r11,64(r31)
	PPC_STORE_U32(ctx.r31.u32 + 64, ctx.r11.u32);
	// stw r11,68(r31)
	PPC_STORE_U32(ctx.r31.u32 + 68, ctx.r11.u32);
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

PPC_FUNC_IMPL(__imp__sub_829F5C68);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5C68) { __imp__sub_829F5C68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5C68) {
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
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// addi r3,r11,25416
	ctx.r3.s64 = ctx.r11.s64 + 25416;
	// bl 0x8297bce8
	ctx.lr = 0x829F5C80;
	sub_8297BCE8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-2984
	ctx.r3.s64 = ctx.r11.s64 + -2984;
	// bl 0x8298ed98
	ctx.lr = 0x829F5C8C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5CA0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5CA0) { __imp__sub_829F5CA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5CA0) {
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
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// addi r3,r11,24700
	ctx.r3.s64 = ctx.r11.s64 + 24700;
	// bl 0x8297bce8
	ctx.lr = 0x829F5CB8;
	sub_8297BCE8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3040
	ctx.r3.s64 = ctx.r11.s64 + -3040;
	// bl 0x8298ed98
	ctx.lr = 0x829F5CC4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5CD8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5CD8) { __imp__sub_829F5CD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5CD8) {
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
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,-31464
	ctx.r3.s64 = ctx.r11.s64 + -31464;
	// bl 0x8297bce8
	ctx.lr = 0x829F5CF0;
	sub_8297BCE8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3024
	ctx.r3.s64 = ctx.r11.s64 + -3024;
	// bl 0x8298ed98
	ctx.lr = 0x829F5CFC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5D10);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5D10) { __imp__sub_829F5D10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5D10) {
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
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// addi r3,r11,22532
	ctx.r3.s64 = ctx.r11.s64 + 22532;
	// bl 0x8297bce8
	ctx.lr = 0x829F5D28;
	sub_8297BCE8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-3008
	ctx.r3.s64 = ctx.r11.s64 + -3008;
	// bl 0x8298ed98
	ctx.lr = 0x829F5D34;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5D48);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5D48) { __imp__sub_829F5D48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5D48) {
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
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// li r31,9
	ctx.r31.s64 = 9;
	// addi r30,r11,17472
	ctx.r30.s64 = ctx.r11.s64 + 17472;
loc_829F5D68:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8297c258
	ctx.lr = 0x829F5D70;
	sub_8297C258(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,364
	ctx.r30.s64 = ctx.r30.s64 + 364;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x829f5d68
	if (!ctx.cr6.lt) goto loc_829F5D68;
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

PPC_FUNC_IMPL(__imp__sub_829F5D98);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5D98) { __imp__sub_829F5D98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5D98) {
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
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// li r31,31
	ctx.r31.s64 = 31;
	// addi r30,r11,25080
	ctx.r30.s64 = ctx.r11.s64 + 25080;
loc_829F5DB8:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8296be18
	ctx.lr = 0x829F5DC0;
	sub_8296BE18(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,8
	ctx.r30.s64 = ctx.r30.s64 + 8;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x829f5db8
	if (!ctx.cr6.lt) goto loc_829F5DB8;
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

PPC_FUNC_IMPL(__imp__sub_829F5DE8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5DE8) { __imp__sub_829F5DE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5DE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-2768
	ctx.r3.s64 = ctx.r11.s64 + -2768;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5DF8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5DF8) { __imp__sub_829F5DF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5DF8) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r5,r11,-11720
	ctx.r5.s64 = ctx.r11.s64 + -11720;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,1104
	ctx.r31.s64 = ctx.r11.s64 + 1104;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F5E2C;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F5E3C;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F5E44;
	sub_8296C788(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-11724
	ctx.r11.s64 = ctx.r11.s64 + -11724;
	// addi r3,r10,-2736
	ctx.r3.s64 = ctx.r10.s64 + -2736;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F5E5C;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F5E70);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5E70) { __imp__sub_829F5E70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5E70) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r5,r11,-11696
	ctx.r5.s64 = ctx.r11.s64 + -11696;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,1072
	ctx.r31.s64 = ctx.r11.s64 + 1072;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F5EA4;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F5EB4;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F5EBC;
	sub_8296C788(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-11700
	ctx.r11.s64 = ctx.r11.s64 + -11700;
	// addi r3,r10,-2656
	ctx.r3.s64 = ctx.r10.s64 + -2656;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F5ED4;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F5EE8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5EE8) { __imp__sub_829F5EE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5EE8) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r5,r11,-11664
	ctx.r5.s64 = ctx.r11.s64 + -11664;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,1040
	ctx.r31.s64 = ctx.r11.s64 + 1040;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F5F1C;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F5F2C;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F5F34;
	sub_8296C788(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-11668
	ctx.r11.s64 = ctx.r11.s64 + -11668;
	// addi r3,r10,-2576
	ctx.r3.s64 = ctx.r10.s64 + -2576;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F5F4C;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F5F60);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5F60) { __imp__sub_829F5F60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5F60) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r11,r11,1160
	ctx.r11.s64 = ctx.r11.s64 + 1160;
	// li r10,16
	ctx.r10.s64 = 16;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_829F5F74:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x829f5f74
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_829F5F74;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F5F88);  /* forward decl */
PPC_WEAK_FUNC(sub_829F5F88) { __imp__sub_829F5F88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F5F88) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r5,r11,-3936
	ctx.r5.s64 = ctx.r11.s64 + -3936;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,1320
	ctx.r31.s64 = ctx.r11.s64 + 1320;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F5FBC;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F5FCC;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F5FD4;
	sub_8296C788(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-3940
	ctx.r11.s64 = ctx.r11.s64 + -3940;
	// addi r3,r10,-2496
	ctx.r3.s64 = ctx.r10.s64 + -2496;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F5FEC;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F6000);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6000) { __imp__sub_829F6000(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6000) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r5,r11,-3908
	ctx.r5.s64 = ctx.r11.s64 + -3908;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,1352
	ctx.r31.s64 = ctx.r11.s64 + 1352;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F6034;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F6044;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F604C;
	sub_8296C788(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-3912
	ctx.r11.s64 = ctx.r11.s64 + -3912;
	// addi r3,r10,-2416
	ctx.r3.s64 = ctx.r10.s64 + -2416;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F6064;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F6078);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6078) { __imp__sub_829F6078(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6078) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r5,r11,-3164
	ctx.r5.s64 = ctx.r11.s64 + -3164;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,1416
	ctx.r31.s64 = ctx.r11.s64 + 1416;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F60AC;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F60BC;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F60C4;
	sub_8296C788(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-3168
	ctx.r11.s64 = ctx.r11.s64 + -3168;
	// addi r3,r10,-2336
	ctx.r3.s64 = ctx.r10.s64 + -2336;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F60DC;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F60F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F60F0) { __imp__sub_829F60F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F60F0) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r5,r11,-3140
	ctx.r5.s64 = ctx.r11.s64 + -3140;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,1448
	ctx.r31.s64 = ctx.r11.s64 + 1448;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F6124;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F6134;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F613C;
	sub_8296C788(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-3144
	ctx.r11.s64 = ctx.r11.s64 + -3144;
	// addi r3,r10,-2256
	ctx.r3.s64 = ctx.r10.s64 + -2256;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F6154;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F6168);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6168) { __imp__sub_829F6168(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6168) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r5,r11,-3104
	ctx.r5.s64 = ctx.r11.s64 + -3104;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,1384
	ctx.r31.s64 = ctx.r11.s64 + 1384;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F619C;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F61AC;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F61B4;
	sub_8296C788(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-3108
	ctx.r11.s64 = ctx.r11.s64 + -3108;
	// addi r3,r10,-2176
	ctx.r3.s64 = ctx.r10.s64 + -2176;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F61CC;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F61E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F61E0) { __imp__sub_829F61E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F61E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-2420
	ctx.r5.s64 = ctx.r11.s64 + -2420;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,1512
	ctx.r3.s64 = ctx.r11.s64 + 1512;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F6200);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6200) { __imp__sub_829F6200(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6200) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-2408
	ctx.r5.s64 = ctx.r11.s64 + -2408;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,1532
	ctx.r3.s64 = ctx.r11.s64 + 1532;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F6220);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6220) { __imp__sub_829F6220(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6220) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r5,r11,-2628
	ctx.r5.s64 = ctx.r11.s64 + -2628;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,1480
	ctx.r31.s64 = ctx.r11.s64 + 1480;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F6254;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F6264;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F626C;
	sub_8296C788(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-2632
	ctx.r11.s64 = ctx.r11.s64 + -2632;
	// addi r3,r10,-2096
	ctx.r3.s64 = ctx.r10.s64 + -2096;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F6284;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F6298);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6298) { __imp__sub_829F6298(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6298) {
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
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r4,r11,10428
	ctx.r4.s64 = ctx.r11.s64 + 10428;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,4352
	ctx.r3.s64 = ctx.r11.s64 + 4352;
	// bl 0x828e0ab8
	ctx.lr = 0x829F62B8;
	sub_828E0AB8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-2016
	ctx.r3.s64 = ctx.r11.s64 + -2016;
	// bl 0x8298ed98
	ctx.lr = 0x829F62C4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F62D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F62D8) { __imp__sub_829F62D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F62D8) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r5,r11,19748
	ctx.r5.s64 = ctx.r11.s64 + 19748;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,5304
	ctx.r31.s64 = ctx.r11.s64 + 5304;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F630C;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F631C;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F6324;
	sub_8296C788(ctx, base);
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,19744
	ctx.r11.s64 = ctx.r11.s64 + 19744;
	// addi r3,r10,-2000
	ctx.r3.s64 = ctx.r10.s64 + -2000;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F633C;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F6350);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6350) { __imp__sub_829F6350(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6350) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r5,r11,19772
	ctx.r5.s64 = ctx.r11.s64 + 19772;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,5240
	ctx.r31.s64 = ctx.r11.s64 + 5240;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F6384;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F6394;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F639C;
	sub_8296C788(ctx, base);
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,19768
	ctx.r11.s64 = ctx.r11.s64 + 19768;
	// addi r3,r10,-1920
	ctx.r3.s64 = ctx.r10.s64 + -1920;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F63B4;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F63C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F63C8) { __imp__sub_829F63C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F63C8) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r5,r11,19796
	ctx.r5.s64 = ctx.r11.s64 + 19796;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,5144
	ctx.r31.s64 = ctx.r11.s64 + 5144;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F63FC;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F640C;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F6414;
	sub_8296C788(ctx, base);
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,19792
	ctx.r11.s64 = ctx.r11.s64 + 19792;
	// addi r3,r10,-1840
	ctx.r3.s64 = ctx.r10.s64 + -1840;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F642C;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F6440);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6440) { __imp__sub_829F6440(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6440) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r5,r11,19832
	ctx.r5.s64 = ctx.r11.s64 + 19832;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,5272
	ctx.r31.s64 = ctx.r11.s64 + 5272;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F6474;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F6484;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F648C;
	sub_8296C788(ctx, base);
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,19828
	ctx.r11.s64 = ctx.r11.s64 + 19828;
	// addi r3,r10,-1760
	ctx.r3.s64 = ctx.r10.s64 + -1760;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F64A4;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F64B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F64B8) { __imp__sub_829F64B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F64B8) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r5,r11,19872
	ctx.r5.s64 = ctx.r11.s64 + 19872;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,5176
	ctx.r31.s64 = ctx.r11.s64 + 5176;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F64EC;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F64FC;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F6504;
	sub_8296C788(ctx, base);
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,19868
	ctx.r11.s64 = ctx.r11.s64 + 19868;
	// addi r3,r10,-1680
	ctx.r3.s64 = ctx.r10.s64 + -1680;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F651C;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F6530);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6530) { __imp__sub_829F6530(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6530) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r5,r11,19900
	ctx.r5.s64 = ctx.r11.s64 + 19900;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,5208
	ctx.r31.s64 = ctx.r11.s64 + 5208;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F6564;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F6574;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F657C;
	sub_8296C788(ctx, base);
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,19896
	ctx.r11.s64 = ctx.r11.s64 + 19896;
	// addi r3,r10,-1600
	ctx.r3.s64 = ctx.r10.s64 + -1600;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F6594;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F65A8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F65A8) { __imp__sub_829F65A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F65A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r10,15
	ctx.r10.s64 = 15;
	// addi r11,r11,5472
	ctx.r11.s64 = ctx.r11.s64 + 5472;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
loc_829F65BC:
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
	// bge cr6,0x829f65bc
	if (!ctx.cr6.lt) goto loc_829F65BC;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F65E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F65E0) { __imp__sub_829F65E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F65E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,20256
	ctx.r5.s64 = ctx.r11.s64 + 20256;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,5664
	ctx.r3.s64 = ctx.r11.s64 + 5664;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F6600);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6600) { __imp__sub_829F6600(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6600) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r5,r11,20612
	ctx.r5.s64 = ctx.r11.s64 + 20612;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,5820
	ctx.r31.s64 = ctx.r11.s64 + 5820;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F6634;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F6644;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F664C;
	sub_8296C788(ctx, base);
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,20608
	ctx.r11.s64 = ctx.r11.s64 + 20608;
	// addi r3,r10,-1520
	ctx.r3.s64 = ctx.r10.s64 + -1520;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F6664;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F6678);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6678) { __imp__sub_829F6678(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6678) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r5,r11,20636
	ctx.r5.s64 = ctx.r11.s64 + 20636;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,5852
	ctx.r31.s64 = ctx.r11.s64 + 5852;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F66AC;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F66BC;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F66C4;
	sub_8296C788(ctx, base);
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,20632
	ctx.r11.s64 = ctx.r11.s64 + 20632;
	// addi r3,r10,-1440
	ctx.r3.s64 = ctx.r10.s64 + -1440;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F66DC;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F66F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F66F0) { __imp__sub_829F66F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F66F0) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r5,r11,20668
	ctx.r5.s64 = ctx.r11.s64 + 20668;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,5724
	ctx.r31.s64 = ctx.r11.s64 + 5724;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F6724;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F6734;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F673C;
	sub_8296C788(ctx, base);
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,20664
	ctx.r11.s64 = ctx.r11.s64 + 20664;
	// addi r3,r10,-1360
	ctx.r3.s64 = ctx.r10.s64 + -1360;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F6754;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F6768);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6768) { __imp__sub_829F6768(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6768) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r5,r11,20696
	ctx.r5.s64 = ctx.r11.s64 + 20696;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,5692
	ctx.r31.s64 = ctx.r11.s64 + 5692;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F679C;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F67AC;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F67B4;
	sub_8296C788(ctx, base);
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,20692
	ctx.r11.s64 = ctx.r11.s64 + 20692;
	// addi r3,r10,-1280
	ctx.r3.s64 = ctx.r10.s64 + -1280;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F67CC;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F67E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F67E0) { __imp__sub_829F67E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F67E0) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r5,r11,20728
	ctx.r5.s64 = ctx.r11.s64 + 20728;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,5788
	ctx.r31.s64 = ctx.r11.s64 + 5788;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F6814;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F6824;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F682C;
	sub_8296C788(ctx, base);
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,20724
	ctx.r11.s64 = ctx.r11.s64 + 20724;
	// addi r3,r10,-1200
	ctx.r3.s64 = ctx.r10.s64 + -1200;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F6844;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F6858);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6858) { __imp__sub_829F6858(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6858) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r5,r11,20756
	ctx.r5.s64 = ctx.r11.s64 + 20756;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,5756
	ctx.r31.s64 = ctx.r11.s64 + 5756;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F688C;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F689C;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F68A4;
	sub_8296C788(ctx, base);
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,20752
	ctx.r11.s64 = ctx.r11.s64 + 20752;
	// addi r3,r10,-1120
	ctx.r3.s64 = ctx.r10.s64 + -1120;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F68BC;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F68D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F68D0) { __imp__sub_829F68D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F68D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,23064
	ctx.r5.s64 = ctx.r11.s64 + 23064;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,6004
	ctx.r3.s64 = ctx.r11.s64 + 6004;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F68F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F68F0) { __imp__sub_829F68F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F68F0) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r5,r11,23972
	ctx.r5.s64 = ctx.r11.s64 + 23972;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,6164
	ctx.r31.s64 = ctx.r11.s64 + 6164;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F6924;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F6934;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F693C;
	sub_8296C788(ctx, base);
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,23968
	ctx.r11.s64 = ctx.r11.s64 + 23968;
	// addi r3,r10,-824
	ctx.r3.s64 = ctx.r10.s64 + -824;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F6954;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F6968);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6968) { __imp__sub_829F6968(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6968) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r5,r11,24000
	ctx.r5.s64 = ctx.r11.s64 + 24000;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,6132
	ctx.r31.s64 = ctx.r11.s64 + 6132;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F699C;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F69AC;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F69B4;
	sub_8296C788(ctx, base);
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,23996
	ctx.r11.s64 = ctx.r11.s64 + 23996;
	// addi r3,r10,-744
	ctx.r3.s64 = ctx.r10.s64 + -744;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F69CC;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F69E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F69E0) { __imp__sub_829F69E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F69E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-664
	ctx.r3.s64 = ctx.r11.s64 + -664;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F69F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F69F0) { __imp__sub_829F69F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F69F0) {
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
	// lis r9,-32250
	ctx.r9.s64 = -2113536000;
	// lis r10,-32141
	ctx.r10.s64 = -2106392576;
	// addi r5,r9,27932
	ctx.r5.s64 = ctx.r9.s64 + 27932;
	// lis r9,-31985
	ctx.r9.s64 = -2096168960;
	// lis r11,-32141
	ctx.r11.s64 = -2106392576;
	// lis r8,-32141
	ctx.r8.s64 = -2106392576;
	// addi r3,r9,6196
	ctx.r3.s64 = ctx.r9.s64 + 6196;
	// li r9,36
	ctx.r9.s64 = 36;
	// addi r8,r8,-16504
	ctx.r8.s64 = ctx.r8.s64 + -16504;
	// addi r7,r10,-16584
	ctx.r7.s64 = ctx.r10.s64 + -16584;
	// addi r6,r11,-16736
	ctx.r6.s64 = ctx.r11.s64 + -16736;
	// li r4,19
	ctx.r4.s64 = 19;
	// bl 0x8272cef8
	ctx.lr = 0x829F6A30;
	sub_8272CEF8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-640
	ctx.r3.s64 = ctx.r11.s64 + -640;
	// bl 0x8298ed98
	ctx.lr = 0x829F6A3C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F6A50);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6A50) { __imp__sub_829F6A50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6A50) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-624
	ctx.r3.s64 = ctx.r11.s64 + -624;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F6A60);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6A60) { __imp__sub_829F6A60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6A60) {
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
	// lis r9,-32250
	ctx.r9.s64 = -2113536000;
	// lis r10,-32141
	ctx.r10.s64 = -2106392576;
	// addi r5,r9,29556
	ctx.r5.s64 = ctx.r9.s64 + 29556;
	// lis r9,-31985
	ctx.r9.s64 = -2096168960;
	// lis r11,-32141
	ctx.r11.s64 = -2106392576;
	// lis r8,-32141
	ctx.r8.s64 = -2106392576;
	// addi r3,r9,6232
	ctx.r3.s64 = ctx.r9.s64 + 6232;
	// li r9,60
	ctx.r9.s64 = 60;
	// addi r8,r8,-5064
	ctx.r8.s64 = ctx.r8.s64 + -5064;
	// addi r7,r10,-5152
	ctx.r7.s64 = ctx.r10.s64 + -5152;
	// addi r6,r11,-5280
	ctx.r6.s64 = ctx.r11.s64 + -5280;
	// li r4,4
	ctx.r4.s64 = 4;
	// bl 0x8272cef8
	ctx.lr = 0x829F6AA0;
	sub_8272CEF8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-592
	ctx.r3.s64 = ctx.r11.s64 + -592;
	// bl 0x8298ed98
	ctx.lr = 0x829F6AAC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F6AC0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6AC0) { __imp__sub_829F6AC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6AC0) {
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
	// lis r9,-32250
	ctx.r9.s64 = -2113536000;
	// lis r10,-32141
	ctx.r10.s64 = -2106392576;
	// addi r5,r9,29916
	ctx.r5.s64 = ctx.r9.s64 + 29916;
	// lis r9,-31985
	ctx.r9.s64 = -2096168960;
	// lis r11,-32141
	ctx.r11.s64 = -2106392576;
	// lis r8,-32141
	ctx.r8.s64 = -2106392576;
	// addi r3,r9,6256
	ctx.r3.s64 = ctx.r9.s64 + 6256;
	// li r9,52
	ctx.r9.s64 = 52;
	// addi r8,r8,-3672
	ctx.r8.s64 = ctx.r8.s64 + -3672;
	// addi r7,r10,-3688
	ctx.r7.s64 = ctx.r10.s64 + -3688;
	// addi r6,r11,-3816
	ctx.r6.s64 = ctx.r11.s64 + -3816;
	// li r4,18
	ctx.r4.s64 = 18;
	// bl 0x8272cef8
	ctx.lr = 0x829F6B00;
	sub_8272CEF8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-576
	ctx.r3.s64 = ctx.r11.s64 + -576;
	// bl 0x8298ed98
	ctx.lr = 0x829F6B0C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F6B20);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6B20) { __imp__sub_829F6B20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6B20) {
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
	// lis r9,-32250
	ctx.r9.s64 = -2113536000;
	// lis r10,-32141
	ctx.r10.s64 = -2106392576;
	// addi r5,r9,30268
	ctx.r5.s64 = ctx.r9.s64 + 30268;
	// lis r9,-31985
	ctx.r9.s64 = -2096168960;
	// lis r11,-32141
	ctx.r11.s64 = -2106392576;
	// lis r8,-32141
	ctx.r8.s64 = -2106392576;
	// addi r3,r9,6280
	ctx.r3.s64 = ctx.r9.s64 + 6280;
	// li r9,36
	ctx.r9.s64 = 36;
	// addi r8,r8,-2112
	ctx.r8.s64 = ctx.r8.s64 + -2112;
	// addi r7,r10,-2192
	ctx.r7.s64 = ctx.r10.s64 + -2192;
	// addi r6,r11,-2344
	ctx.r6.s64 = ctx.r11.s64 + -2344;
	// li r4,7
	ctx.r4.s64 = 7;
	// bl 0x8272cef8
	ctx.lr = 0x829F6B60;
	sub_8272CEF8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-560
	ctx.r3.s64 = ctx.r11.s64 + -560;
	// bl 0x8298ed98
	ctx.lr = 0x829F6B6C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F6B80);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6B80) { __imp__sub_829F6B80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6B80) {
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
	// lis r9,-32250
	ctx.r9.s64 = -2113536000;
	// lis r10,-32141
	ctx.r10.s64 = -2106392576;
	// addi r5,r9,30992
	ctx.r5.s64 = ctx.r9.s64 + 30992;
	// lis r9,-31985
	ctx.r9.s64 = -2096168960;
	// lis r11,-32141
	ctx.r11.s64 = -2106392576;
	// lis r8,-32141
	ctx.r8.s64 = -2106392576;
	// addi r3,r9,6304
	ctx.r3.s64 = ctx.r9.s64 + 6304;
	// li r9,296
	ctx.r9.s64 = 296;
	// addi r8,r8,5680
	ctx.r8.s64 = ctx.r8.s64 + 5680;
	// addi r7,r10,5608
	ctx.r7.s64 = ctx.r10.s64 + 5608;
	// addi r6,r11,5464
	ctx.r6.s64 = ctx.r11.s64 + 5464;
	// li r4,13
	ctx.r4.s64 = 13;
	// bl 0x8272cef8
	ctx.lr = 0x829F6BC0;
	sub_8272CEF8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-544
	ctx.r3.s64 = ctx.r11.s64 + -544;
	// bl 0x8298ed98
	ctx.lr = 0x829F6BCC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F6BE0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6BE0) { __imp__sub_829F6BE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6BE0) {
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
	// lis r9,-32250
	ctx.r9.s64 = -2113536000;
	// lis r10,-32141
	ctx.r10.s64 = -2106392576;
	// addi r5,r9,32748
	ctx.r5.s64 = ctx.r9.s64 + 32748;
	// lis r9,-31985
	ctx.r9.s64 = -2096168960;
	// lis r11,-32141
	ctx.r11.s64 = -2106392576;
	// lis r8,-32141
	ctx.r8.s64 = -2106392576;
	// addi r3,r9,6328
	ctx.r3.s64 = ctx.r9.s64 + 6328;
	// li r9,296
	ctx.r9.s64 = 296;
	// addi r8,r8,29160
	ctx.r8.s64 = ctx.r8.s64 + 29160;
	// addi r7,r10,29088
	ctx.r7.s64 = ctx.r10.s64 + 29088;
	// addi r6,r11,28944
	ctx.r6.s64 = ctx.r11.s64 + 28944;
	// li r4,22
	ctx.r4.s64 = 22;
	// bl 0x8272cef8
	ctx.lr = 0x829F6C20;
	sub_8272CEF8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-528
	ctx.r3.s64 = ctx.r11.s64 + -528;
	// bl 0x8298ed98
	ctx.lr = 0x829F6C2C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F6C40);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6C40) { __imp__sub_829F6C40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6C40) {
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
	// lis r10,-32141
	ctx.r10.s64 = -2106392576;
	// addi r5,r9,-32436
	ctx.r5.s64 = ctx.r9.s64 + -32436;
	// lis r9,-31985
	ctx.r9.s64 = -2096168960;
	// lis r11,-32141
	ctx.r11.s64 = -2106392576;
	// lis r8,-32141
	ctx.r8.s64 = -2106392576;
	// addi r3,r9,6352
	ctx.r3.s64 = ctx.r9.s64 + 6352;
	// li r9,44
	ctx.r9.s64 = 44;
	// addi r8,r8,30032
	ctx.r8.s64 = ctx.r8.s64 + 30032;
	// addi r7,r10,30016
	ctx.r7.s64 = ctx.r10.s64 + 30016;
	// addi r6,r11,29896
	ctx.r6.s64 = ctx.r11.s64 + 29896;
	// li r4,21
	ctx.r4.s64 = 21;
	// bl 0x8272cef8
	ctx.lr = 0x829F6C80;
	sub_8272CEF8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-512
	ctx.r3.s64 = ctx.r11.s64 + -512;
	// bl 0x8298ed98
	ctx.lr = 0x829F6C8C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F6CA0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6CA0) { __imp__sub_829F6CA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6CA0) {
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
	// lis r10,-32141
	ctx.r10.s64 = -2106392576;
	// addi r5,r9,-32084
	ctx.r5.s64 = ctx.r9.s64 + -32084;
	// lis r9,-31985
	ctx.r9.s64 = -2096168960;
	// lis r11,-32141
	ctx.r11.s64 = -2106392576;
	// lis r8,-32141
	ctx.r8.s64 = -2106392576;
	// addi r3,r9,6376
	ctx.r3.s64 = ctx.r9.s64 + 6376;
	// li r9,40
	ctx.r9.s64 = 40;
	// addi r8,r8,30648
	ctx.r8.s64 = ctx.r8.s64 + 30648;
	// addi r7,r10,30568
	ctx.r7.s64 = ctx.r10.s64 + 30568;
	// addi r6,r11,30416
	ctx.r6.s64 = ctx.r11.s64 + 30416;
	// li r4,20
	ctx.r4.s64 = 20;
	// bl 0x8272cef8
	ctx.lr = 0x829F6CE0;
	sub_8272CEF8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-496
	ctx.r3.s64 = ctx.r11.s64 + -496;
	// bl 0x8298ed98
	ctx.lr = 0x829F6CEC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F6D00);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6D00) { __imp__sub_829F6D00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6D00) {
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
	// lis r10,-32141
	ctx.r10.s64 = -2106392576;
	// addi r5,r9,-31732
	ctx.r5.s64 = ctx.r9.s64 + -31732;
	// lis r9,-31985
	ctx.r9.s64 = -2096168960;
	// lis r11,-32141
	ctx.r11.s64 = -2106392576;
	// lis r8,-32141
	ctx.r8.s64 = -2106392576;
	// addi r3,r9,6400
	ctx.r3.s64 = ctx.r9.s64 + 6400;
	// li r9,36
	ctx.r9.s64 = 36;
	// addi r8,r8,31480
	ctx.r8.s64 = ctx.r8.s64 + 31480;
	// addi r7,r10,31400
	ctx.r7.s64 = ctx.r10.s64 + 31400;
	// addi r6,r11,31240
	ctx.r6.s64 = ctx.r11.s64 + 31240;
	// li r4,9
	ctx.r4.s64 = 9;
	// bl 0x8272cef8
	ctx.lr = 0x829F6D40;
	sub_8272CEF8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-480
	ctx.r3.s64 = ctx.r11.s64 + -480;
	// bl 0x8298ed98
	ctx.lr = 0x829F6D4C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F6D60);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6D60) { __imp__sub_829F6D60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6D60) {
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
	// lis r10,-32141
	ctx.r10.s64 = -2106392576;
	// addi r5,r9,-31380
	ctx.r5.s64 = ctx.r9.s64 + -31380;
	// lis r9,-31985
	ctx.r9.s64 = -2096168960;
	// lis r11,-32141
	ctx.r11.s64 = -2106392576;
	// lis r8,-32141
	ctx.r8.s64 = -2106392576;
	// addi r3,r9,6424
	ctx.r3.s64 = ctx.r9.s64 + 6424;
	// li r9,36
	ctx.r9.s64 = 36;
	// addi r8,r8,32304
	ctx.r8.s64 = ctx.r8.s64 + 32304;
	// addi r7,r10,32224
	ctx.r7.s64 = ctx.r10.s64 + 32224;
	// addi r6,r11,32080
	ctx.r6.s64 = ctx.r11.s64 + 32080;
	// li r4,8
	ctx.r4.s64 = 8;
	// bl 0x8272cef8
	ctx.lr = 0x829F6DA0;
	sub_8272CEF8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-464
	ctx.r3.s64 = ctx.r11.s64 + -464;
	// bl 0x8298ed98
	ctx.lr = 0x829F6DAC;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F6DC0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6DC0) { __imp__sub_829F6DC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6DC0) {
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
	// lis r10,-32140
	ctx.r10.s64 = -2106327040;
	// addi r5,r9,-31024
	ctx.r5.s64 = ctx.r9.s64 + -31024;
	// lis r9,-31985
	ctx.r9.s64 = -2096168960;
	// lis r11,-32141
	ctx.r11.s64 = -2106392576;
	// lis r8,-32140
	ctx.r8.s64 = -2106327040;
	// addi r3,r9,6448
	ctx.r3.s64 = ctx.r9.s64 + 6448;
	// li r9,48
	ctx.r9.s64 = 48;
	// addi r8,r8,-32696
	ctx.r8.s64 = ctx.r8.s64 + -32696;
	// addi r7,r10,-32768
	ctx.r7.s64 = ctx.r10.s64 + -32768;
	// addi r6,r11,32624
	ctx.r6.s64 = ctx.r11.s64 + 32624;
	// li r4,6
	ctx.r4.s64 = 6;
	// bl 0x8272cef8
	ctx.lr = 0x829F6E00;
	sub_8272CEF8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-448
	ctx.r3.s64 = ctx.r11.s64 + -448;
	// bl 0x8298ed98
	ctx.lr = 0x829F6E0C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F6E20);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6E20) { __imp__sub_829F6E20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6E20) {
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
	// lis r10,-32140
	ctx.r10.s64 = -2106327040;
	// addi r5,r9,-30528
	ctx.r5.s64 = ctx.r9.s64 + -30528;
	// lis r9,-31985
	ctx.r9.s64 = -2096168960;
	// lis r11,-32140
	ctx.r11.s64 = -2106327040;
	// lis r8,-32140
	ctx.r8.s64 = -2106327040;
	// addi r3,r9,6472
	ctx.r3.s64 = ctx.r9.s64 + 6472;
	// li r9,52
	ctx.r9.s64 = 52;
	// addi r8,r8,-30632
	ctx.r8.s64 = ctx.r8.s64 + -30632;
	// addi r7,r10,-30648
	ctx.r7.s64 = ctx.r10.s64 + -30648;
	// addi r6,r11,-30776
	ctx.r6.s64 = ctx.r11.s64 + -30776;
	// li r4,5
	ctx.r4.s64 = 5;
	// bl 0x8272cef8
	ctx.lr = 0x829F6E60;
	sub_8272CEF8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-432
	ctx.r3.s64 = ctx.r11.s64 + -432;
	// bl 0x8298ed98
	ctx.lr = 0x829F6E6C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F6E80);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6E80) { __imp__sub_829F6E80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6E80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-30044
	ctx.r5.s64 = ctx.r11.s64 + -30044;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,6540
	ctx.r3.s64 = ctx.r11.s64 + 6540;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F6EA0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6EA0) { __imp__sub_829F6EA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6EA0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-30032
	ctx.r5.s64 = ctx.r11.s64 + -30032;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,6500
	ctx.r3.s64 = ctx.r11.s64 + 6500;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F6EC0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6EC0) { __imp__sub_829F6EC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6EC0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-30012
	ctx.r5.s64 = ctx.r11.s64 + -30012;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,6520
	ctx.r3.s64 = ctx.r11.s64 + 6520;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F6EE0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6EE0) { __imp__sub_829F6EE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6EE0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-27512
	ctx.r5.s64 = ctx.r11.s64 + -27512;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,6576
	ctx.r3.s64 = ctx.r11.s64 + 6576;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F6F00);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6F00) { __imp__sub_829F6F00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6F00) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-27496
	ctx.r5.s64 = ctx.r11.s64 + -27496;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,6596
	ctx.r3.s64 = ctx.r11.s64 + 6596;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F6F20);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6F20) { __imp__sub_829F6F20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6F20) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32090
	ctx.r11.s64 = -2103050240;
	// addi r11,r11,-10992
	ctx.r11.s64 = ctx.r11.s64 + -10992;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F6F40);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6F40) { __imp__sub_829F6F40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6F40) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32090
	ctx.r11.s64 = -2103050240;
	// li r6,3786
	ctx.r6.s64 = 3786;
	// addi r5,r11,-23872
	ctx.r5.s64 = ctx.r11.s64 + -23872;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r4,r11,-26600
	ctx.r4.s64 = ctx.r11.s64 + -26600;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,6644
	ctx.r3.s64 = ctx.r11.s64 + 6644;
	// b 0x827fe360
	sub_827FE360(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F6FE0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F6FE0) { __imp__sub_829F6FE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F6FE0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-26468
	ctx.r5.s64 = ctx.r11.s64 + -26468;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,6676
	ctx.r3.s64 = ctx.r11.s64 + 6676;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7000);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7000) { __imp__sub_829F7000(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7000) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32090
	ctx.r11.s64 = -2103050240;
	// li r6,3174
	ctx.r6.s64 = 3174;
	// addi r5,r11,-10872
	ctx.r5.s64 = ctx.r11.s64 + -10872;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r4,r11,-25456
	ctx.r4.s64 = ctx.r11.s64 + -25456;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,6776
	ctx.r3.s64 = ctx.r11.s64 + 6776;
	// b 0x827fe360
	sub_827FE360(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7040);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7040) { __imp__sub_829F7040(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7040) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32090
	ctx.r11.s64 = -2103050240;
	// addi r11,r11,-5876
	ctx.r11.s64 = ctx.r11.s64 + -5876;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7060);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7060) { __imp__sub_829F7060(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7060) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32090
	ctx.r11.s64 = -2103050240;
	// addi r11,r11,-5868
	ctx.r11.s64 = ctx.r11.s64 + -5868;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7080);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7080) { __imp__sub_829F7080(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7080) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32090
	ctx.r11.s64 = -2103050240;
	// addi r11,r11,-5860
	ctx.r11.s64 = ctx.r11.s64 + -5860;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F70A0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F70A0) { __imp__sub_829F70A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F70A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-21472
	ctx.r5.s64 = ctx.r11.s64 + -21472;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,9208
	ctx.r3.s64 = ctx.r11.s64 + 9208;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F70C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F70C0) { __imp__sub_829F70C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F70C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32090
	ctx.r11.s64 = -2103050240;
	// li r6,5496
	ctx.r6.s64 = 5496;
	// addi r5,r11,-4880
	ctx.r5.s64 = ctx.r11.s64 + -4880;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r4,r11,-19216
	ctx.r4.s64 = ctx.r11.s64 + -19216;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,9236
	ctx.r3.s64 = ctx.r11.s64 + 9236;
	// b 0x827fe360
	sub_827FE360(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7120);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7120) { __imp__sub_829F7120(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7120) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32090
	ctx.r11.s64 = -2103050240;
	// addi r11,r11,23340
	ctx.r11.s64 = ctx.r11.s64 + 23340;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7140);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7140) { __imp__sub_829F7140(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7140) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,2056
	ctx.r11.s64 = ctx.r11.s64 + 2056;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7160);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7160) { __imp__sub_829F7160(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7160) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,2064
	ctx.r11.s64 = ctx.r11.s64 + 2064;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7180);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7180) { __imp__sub_829F7180(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7180) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,2072
	ctx.r11.s64 = ctx.r11.s64 + 2072;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F71A0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F71A0) { __imp__sub_829F71A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F71A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32090
	ctx.r11.s64 = -2103050240;
	// lis r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,23584
	ctx.r5.s64 = ctx.r11.s64 + 23584;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// ori r6,r6,38365
	ctx.r6.u64 = ctx.r6.u64 | 38365;
	// addi r4,r11,-17888
	ctx.r4.s64 = ctx.r11.s64 + -17888;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,9304
	ctx.r3.s64 = ctx.r11.s64 + 9304;
	// b 0x827fe360
	sub_827FE360(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F71C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F71C8) { __imp__sub_829F71C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F71C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// li r6,5640
	ctx.r6.s64 = 5640;
	// addi r5,r11,-3584
	ctx.r5.s64 = ctx.r11.s64 + -3584;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r4,r11,-17852
	ctx.r4.s64 = ctx.r11.s64 + -17852;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,9288
	ctx.r3.s64 = ctx.r11.s64 + 9288;
	// b 0x827fe360
	sub_827FE360(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7208);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7208) { __imp__sub_829F7208(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7208) {
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
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-16696
	ctx.r5.s64 = ctx.r11.s64 + -16696;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r31,r11,9776
	ctx.r31.s64 = ctx.r11.s64 + 9776;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F7238;
	sub_8296C718(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x829F7248;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F7250;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-16700
	ctx.r11.s64 = ctx.r11.s64 + -16700;
	// addi r3,r10,-400
	ctx.r3.s64 = ctx.r10.s64 + -400;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F7268;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F7280);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7280) { __imp__sub_829F7280(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7280) {
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
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r7,r11,9776
	ctx.r7.s64 = ctx.r11.s64 + 9776;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-16680
	ctx.r6.s64 = ctx.r11.s64 + -16680;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,9648
	ctx.r31.s64 = ctx.r11.s64 + 9648;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829F72B8;
	sub_8296C750(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x829F72C8;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F72D0;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-16684
	ctx.r11.s64 = ctx.r11.s64 + -16684;
	// addi r3,r10,-320
	ctx.r3.s64 = ctx.r10.s64 + -320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F72E8;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F7300);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7300) { __imp__sub_829F7300(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7300) {
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
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r7,r11,9776
	ctx.r7.s64 = ctx.r11.s64 + 9776;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-16652
	ctx.r6.s64 = ctx.r11.s64 + -16652;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,9584
	ctx.r31.s64 = ctx.r11.s64 + 9584;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829F7338;
	sub_8296C750(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x829F7348;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F7350;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-16656
	ctx.r11.s64 = ctx.r11.s64 + -16656;
	// addi r3,r10,-240
	ctx.r3.s64 = ctx.r10.s64 + -240;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F7368;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F7380);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7380) { __imp__sub_829F7380(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7380) {
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
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r5,2
	ctx.r5.s64 = 2;
	// addi r7,r11,9776
	ctx.r7.s64 = ctx.r11.s64 + 9776;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-16620
	ctx.r6.s64 = ctx.r11.s64 + -16620;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,9424
	ctx.r31.s64 = ctx.r11.s64 + 9424;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829F73B8;
	sub_8296C750(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x829F73C8;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F73D0;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-16624
	ctx.r11.s64 = ctx.r11.s64 + -16624;
	// addi r3,r10,-160
	ctx.r3.s64 = ctx.r10.s64 + -160;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F73E8;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F7400);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7400) { __imp__sub_829F7400(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7400) {
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
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r5,3
	ctx.r5.s64 = 3;
	// addi r7,r11,9776
	ctx.r7.s64 = ctx.r11.s64 + 9776;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-16588
	ctx.r6.s64 = ctx.r11.s64 + -16588;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,9712
	ctx.r31.s64 = ctx.r11.s64 + 9712;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829F7438;
	sub_8296C750(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x829F7448;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F7450;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-16592
	ctx.r11.s64 = ctx.r11.s64 + -16592;
	// addi r3,r10,-80
	ctx.r3.s64 = ctx.r10.s64 + -80;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F7468;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F7480);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7480) { __imp__sub_829F7480(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7480) {
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
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r5,4
	ctx.r5.s64 = 4;
	// addi r7,r11,9776
	ctx.r7.s64 = ctx.r11.s64 + 9776;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-16560
	ctx.r6.s64 = ctx.r11.s64 + -16560;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,9392
	ctx.r31.s64 = ctx.r11.s64 + 9392;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829F74B8;
	sub_8296C750(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x829F74C8;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F74D0;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-16564
	ctx.r11.s64 = ctx.r11.s64 + -16564;
	// addi r3,r10,0
	ctx.r3.s64 = ctx.r10.s64 + 0;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F74E8;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F7500);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7500) { __imp__sub_829F7500(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7500) {
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
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r5,5
	ctx.r5.s64 = 5;
	// addi r7,r11,9776
	ctx.r7.s64 = ctx.r11.s64 + 9776;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-16532
	ctx.r6.s64 = ctx.r11.s64 + -16532;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,9488
	ctx.r31.s64 = ctx.r11.s64 + 9488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829F7538;
	sub_8296C750(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x829F7548;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F7550;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-16536
	ctx.r11.s64 = ctx.r11.s64 + -16536;
	// addi r3,r10,80
	ctx.r3.s64 = ctx.r10.s64 + 80;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F7568;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F7580);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7580) { __imp__sub_829F7580(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7580) {
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
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r5,6
	ctx.r5.s64 = 6;
	// addi r7,r11,9776
	ctx.r7.s64 = ctx.r11.s64 + 9776;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-16504
	ctx.r6.s64 = ctx.r11.s64 + -16504;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,9360
	ctx.r31.s64 = ctx.r11.s64 + 9360;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829F75B8;
	sub_8296C750(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x829F75C8;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F75D0;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-16508
	ctx.r11.s64 = ctx.r11.s64 + -16508;
	// addi r3,r10,160
	ctx.r3.s64 = ctx.r10.s64 + 160;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F75E8;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F7600);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7600) { __imp__sub_829F7600(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7600) {
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
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r5,7
	ctx.r5.s64 = 7;
	// addi r7,r11,9776
	ctx.r7.s64 = ctx.r11.s64 + 9776;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-16476
	ctx.r6.s64 = ctx.r11.s64 + -16476;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,9616
	ctx.r31.s64 = ctx.r11.s64 + 9616;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829F7638;
	sub_8296C750(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x829F7648;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F7650;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-16480
	ctx.r11.s64 = ctx.r11.s64 + -16480;
	// addi r3,r10,240
	ctx.r3.s64 = ctx.r10.s64 + 240;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F7668;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F7680);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7680) { __imp__sub_829F7680(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7680) {
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
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r5,8
	ctx.r5.s64 = 8;
	// addi r7,r11,9776
	ctx.r7.s64 = ctx.r11.s64 + 9776;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-16444
	ctx.r6.s64 = ctx.r11.s64 + -16444;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,9552
	ctx.r31.s64 = ctx.r11.s64 + 9552;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829F76B8;
	sub_8296C750(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x829F76C8;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F76D0;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-16448
	ctx.r11.s64 = ctx.r11.s64 + -16448;
	// addi r3,r10,320
	ctx.r3.s64 = ctx.r10.s64 + 320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F76E8;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F7700);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7700) { __imp__sub_829F7700(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7700) {
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
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r5,9
	ctx.r5.s64 = 9;
	// addi r7,r11,9776
	ctx.r7.s64 = ctx.r11.s64 + 9776;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-16416
	ctx.r6.s64 = ctx.r11.s64 + -16416;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,9520
	ctx.r31.s64 = ctx.r11.s64 + 9520;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829F7738;
	sub_8296C750(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x829F7748;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F7750;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-16420
	ctx.r11.s64 = ctx.r11.s64 + -16420;
	// addi r3,r10,400
	ctx.r3.s64 = ctx.r10.s64 + 400;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F7768;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F7780);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7780) { __imp__sub_829F7780(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7780) {
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
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r5,10
	ctx.r5.s64 = 10;
	// addi r7,r11,9776
	ctx.r7.s64 = ctx.r11.s64 + 9776;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-16380
	ctx.r6.s64 = ctx.r11.s64 + -16380;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,9680
	ctx.r31.s64 = ctx.r11.s64 + 9680;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829F77B8;
	sub_8296C750(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x829F77C8;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F77D0;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-16384
	ctx.r11.s64 = ctx.r11.s64 + -16384;
	// addi r3,r10,480
	ctx.r3.s64 = ctx.r10.s64 + 480;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F77E8;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F7800);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7800) { __imp__sub_829F7800(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7800) {
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
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r5,11
	ctx.r5.s64 = 11;
	// addi r7,r11,9776
	ctx.r7.s64 = ctx.r11.s64 + 9776;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-16344
	ctx.r6.s64 = ctx.r11.s64 + -16344;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,9456
	ctx.r31.s64 = ctx.r11.s64 + 9456;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829F7838;
	sub_8296C750(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x829F7848;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F7850;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-16348
	ctx.r11.s64 = ctx.r11.s64 + -16348;
	// addi r3,r10,560
	ctx.r3.s64 = ctx.r10.s64 + 560;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F7868;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F7880);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7880) { __imp__sub_829F7880(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7880) {
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
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r5,12
	ctx.r5.s64 = 12;
	// addi r7,r11,9776
	ctx.r7.s64 = ctx.r11.s64 + 9776;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-16308
	ctx.r6.s64 = ctx.r11.s64 + -16308;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,9744
	ctx.r31.s64 = ctx.r11.s64 + 9744;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829F78B8;
	sub_8296C750(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x829F78C8;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F78D0;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-16312
	ctx.r11.s64 = ctx.r11.s64 + -16312;
	// addi r3,r10,640
	ctx.r3.s64 = ctx.r10.s64 + 640;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F78E8;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F7900);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7900) { __imp__sub_829F7900(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7900) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-15108
	ctx.r5.s64 = ctx.r11.s64 + -15108;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,9812
	ctx.r3.s64 = ctx.r11.s64 + 9812;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7920);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7920) { __imp__sub_829F7920(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7920) {
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
	// lis r11,-32135
	ctx.r11.s64 = -2105999360;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-15424
	ctx.r5.s64 = ctx.r11.s64 + -15424;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829F7948;
	sub_827DB118(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// lis r9,-32137
	ctx.r9.s64 = -2106130432;
	// addi r10,r11,5108
	ctx.r10.s64 = ctx.r11.s64 + 5108;
	// addi r9,r9,-19072
	ctx.r9.s64 = ctx.r9.s64 + -19072;
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

PPC_FUNC_IMPL(__imp__sub_829F7998);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7998) { __imp__sub_829F7998(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7998) {
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
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,9860
	ctx.r31.s64 = ctx.r11.s64 + 9860;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b63b0
	ctx.lr = 0x829F79B8;
	sub_824B63B0(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r3,r10,720
	ctx.r3.s64 = ctx.r10.s64 + 720;
	// stw r11,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r11.u32);
	// sth r11,16(r31)
	PPC_STORE_U16(ctx.r31.u32 + 16, ctx.r11.u16);
	// sth r11,18(r31)
	PPC_STORE_U16(ctx.r31.u32 + 18, ctx.r11.u16);
	// bl 0x8298ed98
	ctx.lr = 0x829F79D4;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F79E8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F79E8) { __imp__sub_829F79E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F79E8) {
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
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,10000
	ctx.r31.s64 = ctx.r11.s64 + 10000;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b63b0
	ctx.lr = 0x829F7A08;
	sub_824B63B0(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r3,r10,936
	ctx.r3.s64 = ctx.r10.s64 + 936;
	// stw r11,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r11.u32);
	// sth r11,16(r31)
	PPC_STORE_U16(ctx.r31.u32 + 16, ctx.r11.u16);
	// sth r11,18(r31)
	PPC_STORE_U16(ctx.r31.u32 + 18, ctx.r11.u16);
	// bl 0x8298ed98
	ctx.lr = 0x829F7A24;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F7A38);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7A38) { __imp__sub_829F7A38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7A38) {
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
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,9980
	ctx.r31.s64 = ctx.r11.s64 + 9980;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b63b0
	ctx.lr = 0x829F7A58;
	sub_824B63B0(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r3,r10,792
	ctx.r3.s64 = ctx.r10.s64 + 792;
	// stw r11,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r11.u32);
	// sth r11,16(r31)
	PPC_STORE_U16(ctx.r31.u32 + 16, ctx.r11.u16);
	// sth r11,18(r31)
	PPC_STORE_U16(ctx.r31.u32 + 18, ctx.r11.u16);
	// bl 0x8298ed98
	ctx.lr = 0x829F7A74;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F7A88);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7A88) { __imp__sub_829F7A88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7A88) {
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
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,10020
	ctx.r31.s64 = ctx.r11.s64 + 10020;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b63b0
	ctx.lr = 0x829F7AA8;
	sub_824B63B0(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r3,r10,864
	ctx.r3.s64 = ctx.r10.s64 + 864;
	// stw r11,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r11.u32);
	// sth r11,16(r31)
	PPC_STORE_U16(ctx.r31.u32 + 16, ctx.r11.u16);
	// sth r11,18(r31)
	PPC_STORE_U16(ctx.r31.u32 + 18, ctx.r11.u16);
	// bl 0x8298ed98
	ctx.lr = 0x829F7AC4;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829F7AD8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7AD8) { __imp__sub_829F7AD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7AD8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,6336
	ctx.r11.s64 = ctx.r11.s64 + 6336;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7AF8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7AF8) { __imp__sub_829F7AF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7AF8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,7188
	ctx.r11.s64 = ctx.r11.s64 + 7188;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7B18);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7B18) { __imp__sub_829F7B18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7B18) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,7196
	ctx.r11.s64 = ctx.r11.s64 + 7196;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7B38);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7B38) { __imp__sub_829F7B38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7B38) {
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
	// lis r11,-32136
	ctx.r11.s64 = -2106064896;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-28400
	ctx.r5.s64 = ctx.r11.s64 + -28400;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829F7B60;
	sub_827DB118(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// lis r9,-32137
	ctx.r9.s64 = -2106130432;
	// addi r10,r11,7604
	ctx.r10.s64 = ctx.r11.s64 + 7604;
	// addi r9,r9,-19072
	ctx.r9.s64 = ctx.r9.s64 + -19072;
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

PPC_FUNC_IMPL(__imp__sub_829F7BB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7BB0) { __imp__sub_829F7BB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7BB0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,7652
	ctx.r11.s64 = ctx.r11.s64 + 7652;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7BD0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7BD0) { __imp__sub_829F7BD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7BD0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,7660
	ctx.r11.s64 = ctx.r11.s64 + 7660;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7BF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7BF0) { __imp__sub_829F7BF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7BF0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,8136
	ctx.r11.s64 = ctx.r11.s64 + 8136;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7C10);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7C10) { __imp__sub_829F7C10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7C10) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// lfs f0,8132(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8132);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f13,9928(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 9928);
	ctx.f13.f64 = double(temp.f32);
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// fmuls f0,f0,f13
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f13.f64));
	// addi r11,r11,10096
	ctx.r11.s64 = ctx.r11.s64 + 10096;
	// fctiwz f0,f0
	ctx.f0.s64 = (ctx.f0.f64 > double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&ctx.f0.f64));
	// stfiwx f0,0,r11
	PPC_STORE_U32(ctx.r11.u32, ctx.f0.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7C38);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7C38) { __imp__sub_829F7C38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7C38) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// lfs f0,-31256(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -31256);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r11,r11,10080
	ctx.r11.s64 = ctx.r11.s64 + 10080;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7C58);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7C58) { __imp__sub_829F7C58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7C58) {
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
	// lis r11,-32135
	ctx.r11.s64 = -2105999360;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-15424
	ctx.r5.s64 = ctx.r11.s64 + -15424;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829F7C80;
	sub_827DB118(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// lis r9,-32137
	ctx.r9.s64 = -2106130432;
	// addi r10,r11,8808
	ctx.r10.s64 = ctx.r11.s64 + 8808;
	// addi r9,r9,-19072
	ctx.r9.s64 = ctx.r9.s64 + -19072;
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

PPC_FUNC_IMPL(__imp__sub_829F7CD0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7CD0) { __imp__sub_829F7CD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7CD0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,8856
	ctx.r11.s64 = ctx.r11.s64 + 8856;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7CF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7CF0) { __imp__sub_829F7CF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7CF0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,1008
	ctx.r3.s64 = ctx.r11.s64 + 1008;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7D00);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7D00) { __imp__sub_829F7D00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7D00) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,1040
	ctx.r3.s64 = ctx.r11.s64 + 1040;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7D10);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7D10) { __imp__sub_829F7D10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7D10) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,11024
	ctx.r11.s64 = ctx.r11.s64 + 11024;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7D30);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7D30) { __imp__sub_829F7D30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7D30) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,11032
	ctx.r11.s64 = ctx.r11.s64 + 11032;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7D50);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7D50) { __imp__sub_829F7D50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7D50) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,11040
	ctx.r11.s64 = ctx.r11.s64 + 11040;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7D70);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7D70) { __imp__sub_829F7D70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7D70) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,11072
	ctx.r11.s64 = ctx.r11.s64 + 11072;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7D90);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7D90) { __imp__sub_829F7D90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7D90) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,11344
	ctx.r11.s64 = ctx.r11.s64 + 11344;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7DB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7DB0) { __imp__sub_829F7DB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7DB0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,12056
	ctx.r11.s64 = ctx.r11.s64 + 12056;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7DD0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7DD0) { __imp__sub_829F7DD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7DD0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,12064
	ctx.r11.s64 = ctx.r11.s64 + 12064;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7DF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7DF0) { __imp__sub_829F7DF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7DF0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,12072
	ctx.r11.s64 = ctx.r11.s64 + 12072;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7E10);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7E10) { __imp__sub_829F7E10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7E10) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,12080
	ctx.r11.s64 = ctx.r11.s64 + 12080;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7E30);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7E30) { __imp__sub_829F7E30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7E30) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,12088
	ctx.r11.s64 = ctx.r11.s64 + 12088;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7E50);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7E50) { __imp__sub_829F7E50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7E50) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,12096
	ctx.r11.s64 = ctx.r11.s64 + 12096;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F7E70);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7E70) { __imp__sub_829F7E70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7E70) {
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
	// lis r11,-32135
	ctx.r11.s64 = -2105999360;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-15424
	ctx.r5.s64 = ctx.r11.s64 + -15424;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829F7E98;
	sub_827DB118(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// lis r9,-32137
	ctx.r9.s64 = -2106130432;
	// addi r10,r11,12544
	ctx.r10.s64 = ctx.r11.s64 + 12544;
	// addi r9,r9,-19072
	ctx.r9.s64 = ctx.r9.s64 + -19072;
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

PPC_FUNC_IMPL(__imp__sub_829F7EE8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7EE8) { __imp__sub_829F7EE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7EE8) {
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
	// lis r11,-32135
	ctx.r11.s64 = -2105999360;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-15424
	ctx.r5.s64 = ctx.r11.s64 + -15424;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829F7F10;
	sub_827DB118(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// lis r9,-32137
	ctx.r9.s64 = -2106130432;
	// addi r10,r11,12592
	ctx.r10.s64 = ctx.r11.s64 + 12592;
	// addi r9,r9,-19072
	ctx.r9.s64 = ctx.r9.s64 + -19072;
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

PPC_FUNC_IMPL(__imp__sub_829F7F60);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7F60) { __imp__sub_829F7F60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7F60) {
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
	// lis r11,-32135
	ctx.r11.s64 = -2105999360;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-15424
	ctx.r5.s64 = ctx.r11.s64 + -15424;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829F7F88;
	sub_827DB118(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// lis r9,-32137
	ctx.r9.s64 = -2106130432;
	// addi r10,r11,12640
	ctx.r10.s64 = ctx.r11.s64 + 12640;
	// addi r9,r9,-19072
	ctx.r9.s64 = ctx.r9.s64 + -19072;
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

PPC_FUNC_IMPL(__imp__sub_829F7FD8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F7FD8) { __imp__sub_829F7FD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F7FD8) {
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
	// lis r11,-32135
	ctx.r11.s64 = -2105999360;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-23760
	ctx.r5.s64 = ctx.r11.s64 + -23760;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829F8000;
	sub_827DB118(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// lis r9,-32137
	ctx.r9.s64 = -2106130432;
	// addi r10,r11,12688
	ctx.r10.s64 = ctx.r11.s64 + 12688;
	// addi r9,r9,-19072
	ctx.r9.s64 = ctx.r9.s64 + -19072;
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

PPC_FUNC_IMPL(__imp__sub_829F8050);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8050) { __imp__sub_829F8050(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8050) {
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
	// lis r11,-32135
	ctx.r11.s64 = -2105999360;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-23656
	ctx.r5.s64 = ctx.r11.s64 + -23656;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829F8078;
	sub_827DB118(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// lis r9,-32137
	ctx.r9.s64 = -2106130432;
	// addi r10,r11,12736
	ctx.r10.s64 = ctx.r11.s64 + 12736;
	// addi r9,r9,-19072
	ctx.r9.s64 = ctx.r9.s64 + -19072;
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

PPC_FUNC_IMPL(__imp__sub_829F80C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F80C8) { __imp__sub_829F80C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F80C8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,12784
	ctx.r11.s64 = ctx.r11.s64 + 12784;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F80E8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F80E8) { __imp__sub_829F80E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F80E8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,12792
	ctx.r11.s64 = ctx.r11.s64 + 12792;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F8108);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8108) { __imp__sub_829F8108(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8108) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,12800
	ctx.r11.s64 = ctx.r11.s64 + 12800;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F8128);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8128) { __imp__sub_829F8128(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8128) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,12808
	ctx.r11.s64 = ctx.r11.s64 + 12808;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F8148);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8148) { __imp__sub_829F8148(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8148) {
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
	// lis r11,-32135
	ctx.r11.s64 = -2105999360;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-13632
	ctx.r5.s64 = ctx.r11.s64 + -13632;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829F8170;
	sub_827DB118(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// lis r9,-32137
	ctx.r9.s64 = -2106130432;
	// addi r10,r11,13224
	ctx.r10.s64 = ctx.r11.s64 + 13224;
	// addi r9,r9,-19072
	ctx.r9.s64 = ctx.r9.s64 + -19072;
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

PPC_FUNC_IMPL(__imp__sub_829F81C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F81C0) { __imp__sub_829F81C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F81C0) {
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
	// lis r11,-32135
	ctx.r11.s64 = -2105999360;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-15424
	ctx.r5.s64 = ctx.r11.s64 + -15424;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829F81E8;
	sub_827DB118(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// lis r9,-32137
	ctx.r9.s64 = -2106130432;
	// addi r10,r11,13272
	ctx.r10.s64 = ctx.r11.s64 + 13272;
	// addi r9,r9,-19072
	ctx.r9.s64 = ctx.r9.s64 + -19072;
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

PPC_FUNC_IMPL(__imp__sub_829F8238);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8238) { __imp__sub_829F8238(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8238) {
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
	// lis r11,-32135
	ctx.r11.s64 = -2105999360;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-15424
	ctx.r5.s64 = ctx.r11.s64 + -15424;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829F8260;
	sub_827DB118(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// lis r9,-32137
	ctx.r9.s64 = -2106130432;
	// addi r10,r11,13320
	ctx.r10.s64 = ctx.r11.s64 + 13320;
	// addi r9,r9,-19072
	ctx.r9.s64 = ctx.r9.s64 + -19072;
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

PPC_FUNC_IMPL(__imp__sub_829F82B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F82B0) { __imp__sub_829F82B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F82B0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,13368
	ctx.r11.s64 = ctx.r11.s64 + 13368;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F82D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F82D0) { __imp__sub_829F82D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F82D0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,13376
	ctx.r11.s64 = ctx.r11.s64 + 13376;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F82F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F82F0) { __imp__sub_829F82F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F82F0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,13384
	ctx.r11.s64 = ctx.r11.s64 + 13384;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F8310);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8310) { __imp__sub_829F8310(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8310) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,14548
	ctx.r11.s64 = ctx.r11.s64 + 14548;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F8330);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8330) { __imp__sub_829F8330(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8330) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,14556
	ctx.r11.s64 = ctx.r11.s64 + 14556;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F8350);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8350) { __imp__sub_829F8350(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8350) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,14564
	ctx.r11.s64 = ctx.r11.s64 + 14564;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F8370);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8370) { __imp__sub_829F8370(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8370) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,14572
	ctx.r11.s64 = ctx.r11.s64 + 14572;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F8390);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8390) { __imp__sub_829F8390(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8390) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,14580
	ctx.r11.s64 = ctx.r11.s64 + 14580;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F83B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F83B0) { __imp__sub_829F83B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F83B0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,14588
	ctx.r11.s64 = ctx.r11.s64 + 14588;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F83D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F83D0) { __imp__sub_829F83D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F83D0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,14596
	ctx.r11.s64 = ctx.r11.s64 + 14596;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F83F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F83F0) { __imp__sub_829F83F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F83F0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,14604
	ctx.r11.s64 = ctx.r11.s64 + 14604;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F8410);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8410) { __imp__sub_829F8410(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8410) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,14612
	ctx.r11.s64 = ctx.r11.s64 + 14612;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F8430);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8430) { __imp__sub_829F8430(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8430) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,14620
	ctx.r11.s64 = ctx.r11.s64 + 14620;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F8450);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8450) { __imp__sub_829F8450(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8450) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,1072
	ctx.r3.s64 = ctx.r11.s64 + 1072;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F8460);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8460) { __imp__sub_829F8460(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8460) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r5,r11,2368
	ctx.r5.s64 = ctx.r11.s64 + 2368;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,12120
	ctx.r31.s64 = ctx.r11.s64 + 12120;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F8494;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F84A4;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F84AC;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,2364
	ctx.r11.s64 = ctx.r11.s64 + 2364;
	// addi r3,r10,1088
	ctx.r3.s64 = ctx.r10.s64 + 1088;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F84C4;
	sub_8298ED98(ctx, base);
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

