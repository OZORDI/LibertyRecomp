#include "gta4_init.h"

PPC_FUNC_IMPL(__imp__sub_82A6FCA8);
PPC_WEAK_FUNC(sub_82A6FCA8) { __imp__sub_82A6FCA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FCA8) {
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
	// lis r11,-32071
	ctx.r11.s64 = -2101805056;
	// li r30,4
	ctx.r30.s64 = 4;
	// addi r11,r11,30720
	ctx.r11.s64 = ctx.r11.s64 + 30720;
	// addi r31,r11,20
	ctx.r31.s64 = ctx.r11.s64 + 20;
loc_82A6FCCC:
	// addi r31,r31,-4
	ctx.r31.s64 = ctx.r31.s64 + -4;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8227ecf8
	ctx.lr = 0x82A6FCD8;
	sub_8227ECF8(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a6fccc
	if (!ctx.cr6.lt) goto loc_82A6FCCC;
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

PPC_FUNC_IMPL(__imp__sub_82A6FD00);
PPC_WEAK_FUNC(sub_82A6FD00) { __imp__sub_82A6FD00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FD00) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32071
	ctx.r11.s64 = -2101805056;
	// addi r3,r11,30896
	ctx.r3.s64 = ctx.r11.s64 + 30896;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FD10);
PPC_WEAK_FUNC(sub_82A6FD10) { __imp__sub_82A6FD10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FD10) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r3,r11,-31640
	ctx.r3.s64 = ctx.r11.s64 + -31640;
	// b 0x821f03a0
	sub_821F03A0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FD20);
PPC_WEAK_FUNC(sub_82A6FD20) { __imp__sub_82A6FD20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FD20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r3,r11,-24328
	ctx.r3.s64 = ctx.r11.s64 + -24328;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FD30);
PPC_WEAK_FUNC(sub_82A6FD30) { __imp__sub_82A6FD30(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FD30) {
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
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// li r30,41
	ctx.r30.s64 = 41;
	// addi r11,r11,-24496
	ctx.r11.s64 = ctx.r11.s64 + -24496;
	// addi r31,r11,168
	ctx.r31.s64 = ctx.r11.s64 + 168;
loc_82A6FD54:
	// addi r31,r31,-4
	ctx.r31.s64 = ctx.r31.s64 + -4;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8227ecf8
	ctx.lr = 0x82A6FD60;
	sub_8227ECF8(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a6fd54
	if (!ctx.cr6.lt) goto loc_82A6FD54;
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

PPC_FUNC_IMPL(__imp__sub_82A6FD88);
PPC_WEAK_FUNC(sub_82A6FD88) { __imp__sub_82A6FD88(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FD88) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7cc
	ctx.lr = 0x82A6FD90;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// li r30,39
	ctx.r30.s64 = 39;
	// addi r11,r11,-24108
	ctx.r11.s64 = ctx.r11.s64 + -24108;
	// addi r31,r11,4412
	ctx.r31.s64 = ctx.r11.s64 + 4412;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r29,r11,13876
	ctx.r29.s64 = ctx.r11.s64 + 13876;
loc_82A6FDAC:
	// addi r31,r31,-108
	ctx.r31.s64 = ctx.r31.s64 + -108;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r29,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r29.u32);
	// bl 0x82902da0
	ctx.lr = 0x82A6FDBC;
	sub_82902DA0(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a6fdac
	if (!ctx.cr6.lt) goto loc_82A6FDAC;
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FDD0);
PPC_WEAK_FUNC(sub_82A6FDD0) { __imp__sub_82A6FDD0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FDD0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FDD8);
PPC_WEAK_FUNC(sub_82A6FDD8) { __imp__sub_82A6FDD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FDD8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// addi r3,r11,-18492
	ctx.r3.s64 = ctx.r11.s64 + -18492;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FDE8);
PPC_WEAK_FUNC(sub_82A6FDE8) { __imp__sub_82A6FDE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FDE8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FDF0);
PPC_WEAK_FUNC(sub_82A6FDF0) { __imp__sub_82A6FDF0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FDF0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FDF8);
PPC_WEAK_FUNC(sub_82A6FDF8) { __imp__sub_82A6FDF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FDF8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// addi r3,r11,-15880
	ctx.r3.s64 = ctx.r11.s64 + -15880;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r11,r11,13876
	ctx.r11.s64 = ctx.r11.s64 + 13876;
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r11.u32);
	// b 0x82902da0
	sub_82902DA0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FE10);
PPC_WEAK_FUNC(sub_82A6FE10) { __imp__sub_82A6FE10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FE10) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FE18);
PPC_WEAK_FUNC(sub_82A6FE18) { __imp__sub_82A6FE18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FE18) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// addi r3,r11,-1640
	ctx.r3.s64 = ctx.r11.s64 + -1640;
	// b 0x8221fb00
	sub_8221FB00(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FE28);
PPC_WEAK_FUNC(sub_82A6FE28) { __imp__sub_82A6FE28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FE28) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// addi r3,r11,-1472
	ctx.r3.s64 = ctx.r11.s64 + -1472;
	// b 0x8222e578
	sub_8222E578(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FE38);
PPC_WEAK_FUNC(sub_82A6FE38) { __imp__sub_82A6FE38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FE38) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c4
	ctx.lr = 0x82A6FE40;
	__savegprlr_27(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// li r27,15
	ctx.r27.s64 = 15;
	// addi r11,r11,5728
	ctx.r11.s64 = ctx.r11.s64 + 5728;
	// li r28,0
	ctx.r28.s64 = 0;
	// addi r29,r11,512
	ctx.r29.s64 = ctx.r11.s64 + 512;
loc_82A6FE58:
	// addi r29,r29,-32
	ctx.r29.s64 = ctx.r29.s64 + -32;
	// li r30,8
	ctx.r30.s64 = 8;
	// mr r31,r29
	ctx.r31.u64 = ctx.r29.u64;
loc_82A6FE64:
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x82a6fe7c
	if (ctx.cr6.eq) goto loc_82A6FE7C;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x823c05e8
	ctx.lr = 0x82A6FE78;
	sub_823C05E8(ctx, base);
	// stw r28,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r28.u32);
loc_82A6FE7C:
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// addi r31,r31,4
	ctx.r31.s64 = ctx.r31.s64 + 4;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// bne cr6,0x82a6fe64
	if (!ctx.cr6.eq) goto loc_82A6FE64;
	// addi r27,r27,-1
	ctx.r27.s64 = ctx.r27.s64 + -1;
	// cmpwi cr6,r27,0
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 0, ctx.xer);
	// bge cr6,0x82a6fe58
	if (!ctx.cr6.lt) goto loc_82A6FE58;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x829ff814
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FEA0);
PPC_WEAK_FUNC(sub_82A6FEA0) { __imp__sub_82A6FEA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FEA0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FEA8);
PPC_WEAK_FUNC(sub_82A6FEA8) { __imp__sub_82A6FEA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FEA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// addi r11,r11,-25940
	ctx.r11.s64 = ctx.r11.s64 + -25940;
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

PPC_FUNC_IMPL(__imp__sub_82A6FEC4);
PPC_WEAK_FUNC(sub_82A6FEC4) { __imp__sub_82A6FEC4(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FEC4) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FEC8);
PPC_WEAK_FUNC(sub_82A6FEC8) { __imp__sub_82A6FEC8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FEC8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// addi r11,r11,-25924
	ctx.r11.s64 = ctx.r11.s64 + -25924;
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

PPC_FUNC_IMPL(__imp__sub_82A6FEE4);
PPC_WEAK_FUNC(sub_82A6FEE4) { __imp__sub_82A6FEE4(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FEE4) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FEE8);
PPC_WEAK_FUNC(sub_82A6FEE8) { __imp__sub_82A6FEE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FEE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// addi r11,r11,-25892
	ctx.r11.s64 = ctx.r11.s64 + -25892;
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

PPC_FUNC_IMPL(__imp__sub_82A6FF04);
PPC_WEAK_FUNC(sub_82A6FF04) { __imp__sub_82A6FF04(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FF04) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FF08);
PPC_WEAK_FUNC(sub_82A6FF08) { __imp__sub_82A6FF08(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FF08) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// addi r3,r11,-25932
	ctx.r3.s64 = ctx.r11.s64 + -25932;
	// b 0x82249120
	sub_82249120(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FF18);
PPC_WEAK_FUNC(sub_82A6FF18) { __imp__sub_82A6FF18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FF18) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// addi r3,r11,-25916
	ctx.r3.s64 = ctx.r11.s64 + -25916;
	// b 0x822499d8
	sub_822499D8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FF28);
PPC_WEAK_FUNC(sub_82A6FF28) { __imp__sub_82A6FF28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FF28) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// addi r3,r11,-25904
	ctx.r3.s64 = ctx.r11.s64 + -25904;
	// b 0x82172f00
	sub_82172F00(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FF38);
PPC_WEAK_FUNC(sub_82A6FF38) { __imp__sub_82A6FF38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FF38) {
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
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// li r30,3
	ctx.r30.s64 = 3;
	// addi r11,r11,-23704
	ctx.r11.s64 = ctx.r11.s64 + -23704;
	// addi r31,r11,16
	ctx.r31.s64 = ctx.r11.s64 + 16;
loc_82A6FF5C:
	// addi r31,r31,-4
	ctx.r31.s64 = ctx.r31.s64 + -4;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8227ecf8
	ctx.lr = 0x82A6FF68;
	sub_8227ECF8(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a6ff5c
	if (!ctx.cr6.lt) goto loc_82A6FF5C;
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

PPC_FUNC_IMPL(__imp__sub_82A6FF90);
PPC_WEAK_FUNC(sub_82A6FF90) { __imp__sub_82A6FF90(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FF90) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// addi r3,r11,-23688
	ctx.r3.s64 = ctx.r11.s64 + -23688;
	// b 0x8227ecf8
	sub_8227ECF8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FFA0);
PPC_WEAK_FUNC(sub_82A6FFA0) { __imp__sub_82A6FFA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FFA0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A6FFA8);
PPC_WEAK_FUNC(sub_82A6FFA8) { __imp__sub_82A6FFA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A6FFA8) {
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
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// li r30,899
	ctx.r30.s64 = 899;
	// addi r11,r11,-11264
	ctx.r11.s64 = ctx.r11.s64 + -11264;
	// addi r31,r11,3600
	ctx.r31.s64 = ctx.r11.s64 + 3600;
loc_82A6FFCC:
	// addi r31,r31,-4
	ctx.r31.s64 = ctx.r31.s64 + -4;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8251bad8
	ctx.lr = 0x82A6FFD8;
	sub_8251BAD8(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a6ffcc
	if (!ctx.cr6.lt) goto loc_82A6FFCC;
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

PPC_FUNC_IMPL(__imp__sub_82A70000);
PPC_WEAK_FUNC(sub_82A70000) { __imp__sub_82A70000(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70000) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// addi r3,r11,-7664
	ctx.r3.s64 = ctx.r11.s64 + -7664;
	// b 0x8251bb98
	sub_8251BB98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70010);
PPC_WEAK_FUNC(sub_82A70010) { __imp__sub_82A70010(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70010) {
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
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// li r30,14399
	ctx.r30.s64 = 14399;
	// addi r11,r11,7424
	ctx.r11.s64 = ctx.r11.s64 + 7424;
	// addis r11,r11,3
	ctx.r11.s64 = ctx.r11.s64 + 196608;
	// addi r31,r11,-23808
	ctx.r31.s64 = ctx.r11.s64 + -23808;
loc_82A70038:
	// addi r31,r31,-12
	ctx.r31.s64 = ctx.r31.s64 + -12;
	// addi r3,r31,4
	ctx.r3.s64 = ctx.r31.s64 + 4;
	// bl 0x8251bb98
	ctx.lr = 0x82A70044;
	sub_8251BB98(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8251bad8
	ctx.lr = 0x82A7004C;
	sub_8251BAD8(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a70038
	if (!ctx.cr6.lt) goto loc_82A70038;
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

PPC_FUNC_IMPL(__imp__sub_82A70070);
PPC_WEAK_FUNC(sub_82A70070) { __imp__sub_82A70070(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70070) {
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
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// li r30,255
	ctx.r30.s64 = 255;
	// addi r11,r11,-16384
	ctx.r11.s64 = ctx.r11.s64 + -16384;
	// addi r31,r11,5120
	ctx.r31.s64 = ctx.r11.s64 + 5120;
loc_82A70094:
	// lis r11,-32218
	ctx.r11.s64 = -2111438848;
	// addi r31,r31,-20
	ctx.r31.s64 = ctx.r31.s64 + -20;
	// li r5,5
	ctx.r5.s64 = 5;
	// addi r6,r11,-16544
	ctx.r6.s64 = ctx.r11.s64 + -16544;
	// li r4,4
	ctx.r4.s64 = 4;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x821401d0
	ctx.lr = 0x82A700B0;
	sub_821401D0(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a70094
	if (!ctx.cr6.lt) goto loc_82A70094;
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

PPC_FUNC_IMPL(__imp__sub_82A700D8);
PPC_WEAK_FUNC(sub_82A700D8) { __imp__sub_82A700D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A700D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// addi r11,r11,-7660
	ctx.r11.s64 = ctx.r11.s64 + -7660;
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

PPC_FUNC_IMPL(__imp__sub_82A700F4);
PPC_WEAK_FUNC(sub_82A700F4) { __imp__sub_82A700F4(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A700F4) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A700F8);
PPC_WEAK_FUNC(sub_82A700F8) { __imp__sub_82A700F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A700F8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7cc
	ctx.lr = 0x82A70100;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// li r30,39
	ctx.r30.s64 = 39;
	// addi r11,r11,11904
	ctx.r11.s64 = ctx.r11.s64 + 11904;
	// li r29,0
	ctx.r29.s64 = 0;
	// addi r31,r11,3928
	ctx.r31.s64 = ctx.r11.s64 + 3928;
loc_82A70118:
	// addi r31,r31,-96
	ctx.r31.s64 = ctx.r31.s64 + -96;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x821b3560
	ctx.lr = 0x82A70124;
	sub_821B3560(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// stw r29,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r29.u32);
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a70118
	if (!ctx.cr6.lt) goto loc_82A70118;
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70140);
PPC_WEAK_FUNC(sub_82A70140) { __imp__sub_82A70140(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70140) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32053
	ctx.r11.s64 = -2100625408;
	// addi r3,r11,-2416
	ctx.r3.s64 = ctx.r11.s64 + -2416;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70150);
PPC_WEAK_FUNC(sub_82A70150) { __imp__sub_82A70150(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70150) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32053
	ctx.r11.s64 = -2100625408;
	// addi r11,r11,6948
	ctx.r11.s64 = ctx.r11.s64 + 6948;
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

PPC_FUNC_IMPL(__imp__sub_82A7016C);
PPC_WEAK_FUNC(sub_82A7016C) { __imp__sub_82A7016C(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A7016C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70170);
PPC_WEAK_FUNC(sub_82A70170) { __imp__sub_82A70170(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70170) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32053
	ctx.r11.s64 = -2100625408;
	// addi r3,r11,10232
	ctx.r3.s64 = ctx.r11.s64 + 10232;
	// b 0x8290d518
	sub_8290D518(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70180);
PPC_WEAK_FUNC(sub_82A70180) { __imp__sub_82A70180(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70180) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32053
	ctx.r11.s64 = -2100625408;
	// addi r3,r11,22304
	ctx.r3.s64 = ctx.r11.s64 + 22304;
	// b 0x825f0e00
	sub_825F0E00(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70190);
PPC_WEAK_FUNC(sub_82A70190) { __imp__sub_82A70190(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70190) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70198);
PPC_WEAK_FUNC(sub_82A70198) { __imp__sub_82A70198(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70198) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A701A0);
PPC_WEAK_FUNC(sub_82A701A0) { __imp__sub_82A701A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A701A0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A701A8);
PPC_WEAK_FUNC(sub_82A701A8) { __imp__sub_82A701A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A701A8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A701B0);
PPC_WEAK_FUNC(sub_82A701B0) { __imp__sub_82A701B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A701B0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A701B8);
PPC_WEAK_FUNC(sub_82A701B8) { __imp__sub_82A701B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A701B8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A701C0);
PPC_WEAK_FUNC(sub_82A701C0) { __imp__sub_82A701C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A701C0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A701C8);
PPC_WEAK_FUNC(sub_82A701C8) { __imp__sub_82A701C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A701C8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A701D0);
PPC_WEAK_FUNC(sub_82A701D0) { __imp__sub_82A701D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A701D0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A701D8);
PPC_WEAK_FUNC(sub_82A701D8) { __imp__sub_82A701D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A701D8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A701E0);
PPC_WEAK_FUNC(sub_82A701E0) { __imp__sub_82A701E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A701E0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A701E8);
PPC_WEAK_FUNC(sub_82A701E8) { __imp__sub_82A701E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A701E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// lis r10,-32086
	ctx.r10.s64 = -2102788096;
	// addi r11,r11,-22864
	ctx.r11.s64 = ctx.r11.s64 + -22864;
	// stw r11,-27848(r10)
	PPC_STORE_U32(ctx.r10.u32 + -27848, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70200);
PPC_WEAK_FUNC(sub_82A70200) { __imp__sub_82A70200(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70200) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70208);
PPC_WEAK_FUNC(sub_82A70208) { __imp__sub_82A70208(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70208) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// addi r11,r11,-30044
	ctx.r11.s64 = ctx.r11.s64 + -30044;
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

PPC_FUNC_IMPL(__imp__sub_82A70224);
PPC_WEAK_FUNC(sub_82A70224) { __imp__sub_82A70224(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70224) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70228);
PPC_WEAK_FUNC(sub_82A70228) { __imp__sub_82A70228(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70228) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// addi r3,r11,31720
	ctx.r3.s64 = ctx.r11.s64 + 31720;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70238);
PPC_WEAK_FUNC(sub_82A70238) { __imp__sub_82A70238(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70238) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70240);
PPC_WEAK_FUNC(sub_82A70240) { __imp__sub_82A70240(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70240) {
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
	// lis r11,-32049
	ctx.r11.s64 = -2100363264;
	// addi r31,r11,9844
	ctx.r31.s64 = ctx.r11.s64 + 9844;
	// lhz r11,18(r31)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r31.u32 + 18);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a7026c
	if (ctx.cr6.eq) goto loc_82A7026C;
	// lwz r3,12(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 12);
	// bl 0x821b3560
	ctx.lr = 0x82A7026C;
	sub_821B3560(ctx, base);
loc_82A7026C:
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a70280
	if (ctx.cr6.eq) goto loc_82A70280;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x821b3560
	ctx.lr = 0x82A70280;
	sub_821B3560(ctx, base);
loc_82A70280:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70298);
PPC_WEAK_FUNC(sub_82A70298) { __imp__sub_82A70298(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70298) {
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
	// lis r11,-32047
	ctx.r11.s64 = -2100232192;
	// addi r31,r11,-8016
	ctx.r31.s64 = ctx.r11.s64 + -8016;
	// lwz r11,16(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 16);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a702dc
	if (ctx.cr6.eq) goto loc_82A702DC;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,4(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4);
	// bl 0x822b1900
	ctx.lr = 0x82A702C8;
	sub_822B1900(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r31,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r31.u32);
	// stw r11,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r11.u32);
	// stw r31,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r31.u32);
	// stw r11,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r11.u32);
loc_82A702DC:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A702F0);
PPC_WEAK_FUNC(sub_82A702F0) { __imp__sub_82A702F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A702F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32046
	ctx.r11.s64 = -2100166656;
	// addi r3,r11,-13808
	ctx.r3.s64 = ctx.r11.s64 + -13808;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70300);
PPC_WEAK_FUNC(sub_82A70300) { __imp__sub_82A70300(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70300) {
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
	// lis r11,-32047
	ctx.r11.s64 = -2100232192;
	// addi r31,r11,18400
	ctx.r31.s64 = ctx.r11.s64 + 18400;
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,33326
	ctx.r11.u64 = ctx.r11.u64 | 33326;
	// lhzx r11,r31,r11
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r31.u32 + ctx.r11.u32);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a7033c
	if (ctx.cr6.eq) goto loc_82A7033C;
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,33320
	ctx.r11.u64 = ctx.r11.u64 | 33320;
	// lwzx r3,r31,r11
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + ctx.r11.u32);
	// bl 0x821b3560
	ctx.lr = 0x82A7033C;
	sub_821B3560(ctx, base);
loc_82A7033C:
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,33318
	ctx.r11.u64 = ctx.r11.u64 | 33318;
	// lhzx r11,r31,r11
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r31.u32 + ctx.r11.u32);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a70360
	if (ctx.cr6.eq) goto loc_82A70360;
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,33312
	ctx.r11.u64 = ctx.r11.u64 | 33312;
	// lwzx r3,r31,r11
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + ctx.r11.u32);
	// bl 0x821b3560
	ctx.lr = 0x82A70360;
	sub_821B3560(ctx, base);
loc_82A70360:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70378);
PPC_WEAK_FUNC(sub_82A70378) { __imp__sub_82A70378(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70378) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// addi r3,r11,-17928
	ctx.r3.s64 = ctx.r11.s64 + -17928;
	// b 0x82660298
	sub_82660298(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70388);
PPC_WEAK_FUNC(sub_82A70388) { __imp__sub_82A70388(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70388) {
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
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// addi r31,r11,20476
	ctx.r31.s64 = ctx.r11.s64 + 20476;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// bl 0x821b3560
	ctx.lr = 0x82A703A8;
	sub_821B3560(ctx, base);
	// lhz r11,6(r31)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r31.u32 + 6);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a703bc
	if (ctx.cr6.eq) goto loc_82A703BC;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x821b3560
	ctx.lr = 0x82A703BC;
	sub_821B3560(ctx, base);
loc_82A703BC:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A703D0);
PPC_WEAK_FUNC(sub_82A703D0) { __imp__sub_82A703D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A703D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lis r10,-32086
	ctx.r10.s64 = -2102788096;
	// addi r11,r11,2636
	ctx.r11.s64 = ctx.r11.s64 + 2636;
	// addi r10,r10,-21892
	ctx.r10.s64 = ctx.r10.s64 + -21892;
	// stw r11,72(r10)
	PPC_STORE_U32(ctx.r10.u32 + 72, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A703E8);
PPC_WEAK_FUNC(sub_82A703E8) { __imp__sub_82A703E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A703E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// addi r3,r11,8096
	ctx.r3.s64 = ctx.r11.s64 + 8096;
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r10,r10,13876
	ctx.r10.s64 = ctx.r10.s64 + 13876;
	// addi r11,r11,-8676
	ctx.r11.s64 = ctx.r11.s64 + -8676;
	// stw r11,8(r3)
	PPC_STORE_U32(ctx.r3.u32 + 8, ctx.r11.u32);
	// stw r10,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r10.u32);
	// b 0x82902da0
	sub_82902DA0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70410);
PPC_WEAK_FUNC(sub_82A70410) { __imp__sub_82A70410(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70410) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c8
	ctx.lr = 0x82A70418;
	__savegprlr_28(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// li r29,31
	ctx.r29.s64 = 31;
	// addi r11,r11,8320
	ctx.r11.s64 = ctx.r11.s64 + 8320;
	// li r28,0
	ctx.r28.s64 = 0;
	// addi r31,r11,2600
	ctx.r31.s64 = ctx.r11.s64 + 2600;
loc_82A70430:
	// addi r31,r31,-80
	ctx.r31.s64 = ctx.r31.s64 + -80;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x82a70448
	if (ctx.cr6.eq) goto loc_82A70448;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x823c05e8
	ctx.lr = 0x82A70448;
	sub_823C05E8(ctx, base);
loc_82A70448:
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4);
	// addi r30,r31,4
	ctx.r30.s64 = ctx.r31.s64 + 4;
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x82a70460
	if (ctx.cr6.eq) goto loc_82A70460;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// bl 0x823c05e8
	ctx.lr = 0x82A70460;
	sub_823C05E8(ctx, base);
loc_82A70460:
	// addi r29,r29,-1
	ctx.r29.s64 = ctx.r29.s64 + -1;
	// stb r28,-2(r31)
	PPC_STORE_U8(ctx.r31.u32 + -2, ctx.r28.u8);
	// stw r28,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r28.u32);
	// stw r28,0(r30)
	PPC_STORE_U32(ctx.r30.u32 + 0, ctx.r28.u32);
	// cmpwi cr6,r29,0
	ctx.cr6.compare<int32_t>(ctx.r29.s32, 0, ctx.xer);
	// bge cr6,0x82a70430
	if (!ctx.cr6.lt) goto loc_82A70430;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x829ff818
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70480);
PPC_WEAK_FUNC(sub_82A70480) { __imp__sub_82A70480(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70480) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70488);
PPC_WEAK_FUNC(sub_82A70488) { __imp__sub_82A70488(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70488) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70490);
PPC_WEAK_FUNC(sub_82A70490) { __imp__sub_82A70490(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70490) {
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
	// addi r31,r11,12672
	ctx.r31.s64 = ctx.r11.s64 + 12672;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x821b3560
	ctx.lr = 0x82A704B0;
	sub_821B3560(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// sth r11,4(r31)
	PPC_STORE_U16(ctx.r31.u32 + 4, ctx.r11.u16);
	// sth r11,6(r31)
	PPC_STORE_U16(ctx.r31.u32 + 6, ctx.r11.u16);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A704D8);
PPC_WEAK_FUNC(sub_82A704D8) { __imp__sub_82A704D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A704D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// addi r3,r11,12532
	ctx.r3.s64 = ctx.r11.s64 + 12532;
	// b 0x828dca28
	sub_828DCA28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A704E8);
PPC_WEAK_FUNC(sub_82A704E8) { __imp__sub_82A704E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A704E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// addi r11,r11,14660
	ctx.r11.s64 = ctx.r11.s64 + 14660;
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

PPC_FUNC_IMPL(__imp__sub_82A70504);
PPC_WEAK_FUNC(sub_82A70504) { __imp__sub_82A70504(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70504) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70508);
PPC_WEAK_FUNC(sub_82A70508) { __imp__sub_82A70508(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70508) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// addi r11,r11,14668
	ctx.r11.s64 = ctx.r11.s64 + 14668;
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

PPC_FUNC_IMPL(__imp__sub_82A70524);
PPC_WEAK_FUNC(sub_82A70524) { __imp__sub_82A70524(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70524) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70528);
PPC_WEAK_FUNC(sub_82A70528) { __imp__sub_82A70528(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70528) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// addi r11,r11,14676
	ctx.r11.s64 = ctx.r11.s64 + 14676;
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

PPC_FUNC_IMPL(__imp__sub_82A70544);
PPC_WEAK_FUNC(sub_82A70544) { __imp__sub_82A70544(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70544) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70548);
PPC_WEAK_FUNC(sub_82A70548) { __imp__sub_82A70548(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70548) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// addi r11,r11,14684
	ctx.r11.s64 = ctx.r11.s64 + 14684;
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

PPC_FUNC_IMPL(__imp__sub_82A70564);
PPC_WEAK_FUNC(sub_82A70564) { __imp__sub_82A70564(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70564) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70568);
PPC_WEAK_FUNC(sub_82A70568) { __imp__sub_82A70568(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70568) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// addi r11,r11,21204
	ctx.r11.s64 = ctx.r11.s64 + 21204;
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

PPC_FUNC_IMPL(__imp__sub_82A70584);
PPC_WEAK_FUNC(sub_82A70584) { __imp__sub_82A70584(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70584) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70588);
PPC_WEAK_FUNC(sub_82A70588) { __imp__sub_82A70588(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70588) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c8
	ctx.lr = 0x82A70590;
	__savegprlr_28(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// li r29,31
	ctx.r29.s64 = 31;
	// addi r11,r11,23732
	ctx.r11.s64 = ctx.r11.s64 + 23732;
	// li r30,0
	ctx.r30.s64 = 0;
	// addi r31,r11,516
	ctx.r31.s64 = ctx.r11.s64 + 516;
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r28,r11,-2596
	ctx.r28.s64 = ctx.r11.s64 + -2596;
loc_82A705B0:
	// addi r31,r31,-16
	ctx.r31.s64 = ctx.r31.s64 + -16;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r28,-4(r31)
	PPC_STORE_U32(ctx.r31.u32 + -4, ctx.r28.u32);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x82a705d0
	if (ctx.cr6.eq) goto loc_82A705D0;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x823c05e8
	ctx.lr = 0x82A705CC;
	sub_823C05E8(ctx, base);
	// stw r30,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r30.u32);
loc_82A705D0:
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x82a705f4
	if (ctx.cr6.eq) goto loc_82A705F4;
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// li r4,1
	ctx.r4.s64 = 1;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x82A705F0;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// stw r30,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r30.u32);
loc_82A705F4:
	// addi r29,r29,-1
	ctx.r29.s64 = ctx.r29.s64 + -1;
	// stw r30,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r30.u32);
	// cmpwi cr6,r29,0
	ctx.cr6.compare<int32_t>(ctx.r29.s32, 0, ctx.xer);
	// bge cr6,0x82a705b0
	if (!ctx.cr6.lt) goto loc_82A705B0;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x829ff818
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70610);
PPC_WEAK_FUNC(sub_82A70610) { __imp__sub_82A70610(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70610) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c8
	ctx.lr = 0x82A70618;
	__savegprlr_28(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// li r29,7
	ctx.r29.s64 = 7;
	// addi r11,r11,24244
	ctx.r11.s64 = ctx.r11.s64 + 24244;
	// li r30,0
	ctx.r30.s64 = 0;
	// addi r31,r11,164
	ctx.r31.s64 = ctx.r11.s64 + 164;
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r28,r11,-2588
	ctx.r28.s64 = ctx.r11.s64 + -2588;
loc_82A70638:
	// addi r31,r31,-20
	ctx.r31.s64 = ctx.r31.s64 + -20;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r28,-4(r31)
	PPC_STORE_U32(ctx.r31.u32 + -4, ctx.r28.u32);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x82a70658
	if (ctx.cr6.eq) goto loc_82A70658;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x823c05e8
	ctx.lr = 0x82A70654;
	sub_823C05E8(ctx, base);
	// stw r30,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r30.u32);
loc_82A70658:
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x82a7067c
	if (ctx.cr6.eq) goto loc_82A7067C;
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// li r4,1
	ctx.r4.s64 = 1;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x82A70678;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// stw r30,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r30.u32);
loc_82A7067C:
	// addi r29,r29,-1
	ctx.r29.s64 = ctx.r29.s64 + -1;
	// stw r30,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r30.u32);
	// cmpwi cr6,r29,0
	ctx.cr6.compare<int32_t>(ctx.r29.s32, 0, ctx.xer);
	// bge cr6,0x82a70638
	if (!ctx.cr6.lt) goto loc_82A70638;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x829ff818
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70698);
PPC_WEAK_FUNC(sub_82A70698) { __imp__sub_82A70698(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70698) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A706A0);
PPC_WEAK_FUNC(sub_82A706A0) { __imp__sub_82A706A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A706A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// addi r3,r11,25216
	ctx.r3.s64 = ctx.r11.s64 + 25216;
	// b 0x8231bd90
	sub_8231BD90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A706B0);
PPC_WEAK_FUNC(sub_82A706B0) { __imp__sub_82A706B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A706B0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A706B8);
PPC_WEAK_FUNC(sub_82A706B8) { __imp__sub_82A706B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A706B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32034
	ctx.r11.s64 = -2099380224;
	// addi r3,r11,23132
	ctx.r3.s64 = ctx.r11.s64 + 23132;
	// b 0x82329c40
	sub_82329C40(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A706C8);
PPC_WEAK_FUNC(sub_82A706C8) { __imp__sub_82A706C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A706C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32033
	ctx.r11.s64 = -2099314688;
	// addi r3,r11,7008
	ctx.r3.s64 = ctx.r11.s64 + 7008;
	// b 0x828bdab8
	sub_828BDAB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A706D8);
PPC_WEAK_FUNC(sub_82A706D8) { __imp__sub_82A706D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A706D8) {
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
	// addi r31,r11,2688
	ctx.r31.s64 = ctx.r11.s64 + 2688;
	// addi r3,r31,36
	ctx.r3.s64 = ctx.r31.s64 + 36;
	// bl 0x8227ecf8
	ctx.lr = 0x82A706F8;
	sub_8227ECF8(ctx, base);
	// addi r3,r31,32
	ctx.r3.s64 = ctx.r31.s64 + 32;
	// bl 0x8227ecf8
	ctx.lr = 0x82A70700;
	sub_8227ECF8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70718);
PPC_WEAK_FUNC(sub_82A70718) { __imp__sub_82A70718(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70718) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32010
	ctx.r11.s64 = -2097807360;
	// addi r3,r11,-3120
	ctx.r3.s64 = ctx.r11.s64 + -3120;
	// b 0x8234cc48
	sub_8234CC48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70728);
PPC_WEAK_FUNC(sub_82A70728) { __imp__sub_82A70728(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70728) {
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
	// addi r31,r11,-26228
	ctx.r31.s64 = ctx.r11.s64 + -26228;
	// addi r3,r31,4
	ctx.r3.s64 = ctx.r31.s64 + 4;
	// bl 0x828dca28
	ctx.lr = 0x82A70748;
	sub_828DCA28(ctx, base);
	// addi r3,r31,16
	ctx.r3.s64 = ctx.r31.s64 + 16;
	// bl 0x8234ef90
	ctx.lr = 0x82A70750;
	sub_8234EF90(ctx, base);
	// addi r3,r31,16
	ctx.r3.s64 = ctx.r31.s64 + 16;
	// bl 0x8234ef90
	ctx.lr = 0x82A70758;
	sub_8234EF90(ctx, base);
	// addi r3,r31,4
	ctx.r3.s64 = ctx.r31.s64 + 4;
	// bl 0x828dca28
	ctx.lr = 0x82A70760;
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

PPC_FUNC_IMPL(__imp__sub_82A70778);
PPC_WEAK_FUNC(sub_82A70778) { __imp__sub_82A70778(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70778) {
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
	// addi r31,r11,-8828
	ctx.r31.s64 = ctx.r11.s64 + -8828;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// ble cr6,0x82a707d8
	if (!ctx.cr6.gt) goto loc_82A707D8;
	// lbz r11,24(r31)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r31.u32 + 24);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a707b8
	if (ctx.cr6.eq) goto loc_82A707B8;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x821b3560
	ctx.lr = 0x82A707B0;
	sub_821B3560(ctx, base);
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4);
	// bl 0x821b3560
	ctx.lr = 0x82A707B8;
	sub_821B3560(ctx, base);
loc_82A707B8:
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
loc_82A707D8:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A707F0);
PPC_WEAK_FUNC(sub_82A707F0) { __imp__sub_82A707F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A707F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,-8688
	ctx.r3.s64 = ctx.r11.s64 + -8688;
	// b 0x828ca980
	sub_828CA980(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70800);
PPC_WEAK_FUNC(sub_82A70800) { __imp__sub_82A70800(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70800) {
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
	// li r30,31
	ctx.r30.s64 = 31;
	// addi r11,r11,-5248
	ctx.r11.s64 = ctx.r11.s64 + -5248;
	// addi r31,r11,6704
	ctx.r31.s64 = ctx.r11.s64 + 6704;
loc_82A70824:
	// addi r31,r31,-208
	ctx.r31.s64 = ctx.r31.s64 + -208;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x822bca90
	ctx.lr = 0x82A70830;
	sub_822BCA90(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a70824
	if (!ctx.cr6.lt) goto loc_82A70824;
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

PPC_FUNC_IMPL(__imp__sub_82A70858);
PPC_WEAK_FUNC(sub_82A70858) { __imp__sub_82A70858(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70858) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r3,r11,5792
	ctx.r3.s64 = ctx.r11.s64 + 5792;
	// bl 0x823941d0
	ctx.lr = 0x82A70870;
	sub_823941D0(ctx, base);
	// bl 0x827bad18
	ctx.lr = 0x82A70874;
	sub_827BAD18(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70888);
PPC_WEAK_FUNC(sub_82A70888) { __imp__sub_82A70888(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70888) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,22176
	ctx.r3.s64 = ctx.r11.s64 + 22176;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70898);
PPC_WEAK_FUNC(sub_82A70898) { __imp__sub_82A70898(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70898) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,22208
	ctx.r3.s64 = ctx.r11.s64 + 22208;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A708A8);
PPC_WEAK_FUNC(sub_82A708A8) { __imp__sub_82A708A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A708A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,22240
	ctx.r3.s64 = ctx.r11.s64 + 22240;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A708B8);
PPC_WEAK_FUNC(sub_82A708B8) { __imp__sub_82A708B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A708B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,22272
	ctx.r3.s64 = ctx.r11.s64 + 22272;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A708C8);
PPC_WEAK_FUNC(sub_82A708C8) { __imp__sub_82A708C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A708C8) {
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
	// lis r31,-32005
	ctx.r31.s64 = -2097479680;
	// lwz r3,22628(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 22628);
	// bl 0x821b3560
	ctx.lr = 0x82A708E4;
	sub_821B3560(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,22628(r31)
	PPC_STORE_U32(ctx.r31.u32 + 22628, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70900);
PPC_WEAK_FUNC(sub_82A70900) { __imp__sub_82A70900(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70900) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70908);
PPC_WEAK_FUNC(sub_82A70908) { __imp__sub_82A70908(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70908) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70910);
PPC_WEAK_FUNC(sub_82A70910) { __imp__sub_82A70910(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70910) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70918);
PPC_WEAK_FUNC(sub_82A70918) { __imp__sub_82A70918(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70918) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70920);
PPC_WEAK_FUNC(sub_82A70920) { __imp__sub_82A70920(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70920) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70928);
PPC_WEAK_FUNC(sub_82A70928) { __imp__sub_82A70928(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70928) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32004
	ctx.r11.s64 = -2097414144;
	// addi r3,r11,11968
	ctx.r3.s64 = ctx.r11.s64 + 11968;
	// b 0x82699928
	sub_82699928(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70938);
PPC_WEAK_FUNC(sub_82A70938) { __imp__sub_82A70938(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70938) {
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
	// lis r11,-32004
	ctx.r11.s64 = -2097414144;
	// li r30,15
	ctx.r30.s64 = 15;
	// addi r11,r11,16336
	ctx.r11.s64 = ctx.r11.s64 + 16336;
	// addi r31,r11,512
	ctx.r31.s64 = ctx.r11.s64 + 512;
loc_82A7095C:
	// addi r31,r31,-32
	ctx.r31.s64 = ctx.r31.s64 + -32;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x822bca90
	ctx.lr = 0x82A70968;
	sub_822BCA90(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a7095c
	if (!ctx.cr6.lt) goto loc_82A7095C;
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

PPC_FUNC_IMPL(__imp__sub_82A70990);
PPC_WEAK_FUNC(sub_82A70990) { __imp__sub_82A70990(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70990) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,22304
	ctx.r3.s64 = ctx.r11.s64 + 22304;
	// b 0x823a0d30
	sub_823A0D30(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A709A0);
PPC_WEAK_FUNC(sub_82A709A0) { __imp__sub_82A709A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A709A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r11,r11,25776
	ctx.r11.s64 = ctx.r11.s64 + 25776;
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

PPC_FUNC_IMPL(__imp__sub_82A709BC);
PPC_WEAK_FUNC(sub_82A709BC) { __imp__sub_82A709BC(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A709BC) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A709C0);
PPC_WEAK_FUNC(sub_82A709C0) { __imp__sub_82A709C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A709C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r11,r11,25784
	ctx.r11.s64 = ctx.r11.s64 + 25784;
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

PPC_FUNC_IMPL(__imp__sub_82A709DC);
PPC_WEAK_FUNC(sub_82A709DC) { __imp__sub_82A709DC(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A709DC) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A709E0);
PPC_WEAK_FUNC(sub_82A709E0) { __imp__sub_82A709E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A709E0) {
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
	// lis r10,-32004
	ctx.r10.s64 = -2097414144;
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r31,r10,23552
	ctx.r31.s64 = ctx.r10.s64 + 23552;
	// addi r11,r11,17192
	ctx.r11.s64 = ctx.r11.s64 + 17192;
	// lhz r10,98(r31)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r31.u32 + 98);
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x82a70a18
	if (ctx.cr6.eq) goto loc_82A70A18;
	// lwz r3,92(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 92);
	// bl 0x821b3560
	ctx.lr = 0x82A70A18;
	sub_821B3560(ctx, base);
loc_82A70A18:
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r11,r11,13876
	ctx.r11.s64 = ctx.r11.s64 + 13876;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82902da0
	ctx.lr = 0x82A70A2C;
	sub_82902DA0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70A40);
PPC_WEAK_FUNC(sub_82A70A40) { __imp__sub_82A70A40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70A40) {
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
	// li r30,3
	ctx.r30.s64 = 3;
	// addi r11,r11,7756
	ctx.r11.s64 = ctx.r11.s64 + 7756;
	// addi r31,r11,64
	ctx.r31.s64 = ctx.r11.s64 + 64;
loc_82A70A64:
	// addi r31,r31,-16
	ctx.r31.s64 = ctx.r31.s64 + -16;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8225a078
	ctx.lr = 0x82A70A70;
	sub_8225A078(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a70a64
	if (!ctx.cr6.lt) goto loc_82A70A64;
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

PPC_FUNC_IMPL(__imp__sub_82A70A98);
PPC_WEAK_FUNC(sub_82A70A98) { __imp__sub_82A70A98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70A98) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70AA0);
PPC_WEAK_FUNC(sub_82A70AA0) { __imp__sub_82A70AA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70AA0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70AA8);
PPC_WEAK_FUNC(sub_82A70AA8) { __imp__sub_82A70AA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70AA8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70AB0);
PPC_WEAK_FUNC(sub_82A70AB0) { __imp__sub_82A70AB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70AB0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70AB8);
PPC_WEAK_FUNC(sub_82A70AB8) { __imp__sub_82A70AB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70AB8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,18048
	ctx.r3.s64 = ctx.r11.s64 + 18048;
	// b 0x824314b8
	sub_824314B8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70AC8);
PPC_WEAK_FUNC(sub_82A70AC8) { __imp__sub_82A70AC8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70AC8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c0
	ctx.lr = 0x82A70AD0;
	__savegprlr_26(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32002
	ctx.r11.s64 = -2097283072;
	// li r29,127
	ctx.r29.s64 = 127;
	// addi r11,r11,27328
	ctx.r11.s64 = ctx.r11.s64 + 27328;
	// li r30,0
	ctx.r30.s64 = 0;
	// addi r31,r11,13844
	ctx.r31.s64 = ctx.r11.s64 + 13844;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// addi r27,r11,-24732
	ctx.r27.s64 = ctx.r11.s64 + -24732;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// addi r26,r11,-23716
	ctx.r26.s64 = ctx.r11.s64 + -23716;
loc_82A70AF8:
	// addi r31,r31,-108
	ctx.r31.s64 = ctx.r31.s64 + -108;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r28,r31,-20
	ctx.r28.s64 = ctx.r31.s64 + -20;
	// stw r26,-20(r31)
	PPC_STORE_U32(ctx.r31.u32 + -20, ctx.r26.u32);
	// bl 0x82434a18
	ctx.lr = 0x82A70B0C;
	sub_82434A18(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r30,68(r31)
	PPC_STORE_U32(ctx.r31.u32 + 68, ctx.r30.u32);
	// stw r30,72(r31)
	PPC_STORE_U32(ctx.r31.u32 + 72, ctx.r30.u32);
	// stw r30,76(r31)
	PPC_STORE_U32(ctx.r31.u32 + 76, ctx.r30.u32);
	// stw r27,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r27.u32);
	// bl 0x82434a18
	ctx.lr = 0x82A70B24;
	sub_82434A18(ctx, base);
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// bl 0x824122c8
	ctx.lr = 0x82A70B2C;
	sub_824122C8(ctx, base);
	// addi r29,r29,-1
	ctx.r29.s64 = ctx.r29.s64 + -1;
	// cmpwi cr6,r29,0
	ctx.cr6.compare<int32_t>(ctx.r29.s32, 0, ctx.xer);
	// bge cr6,0x82a70af8
	if (!ctx.cr6.lt) goto loc_82A70AF8;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70B40);
PPC_WEAK_FUNC(sub_82A70B40) { __imp__sub_82A70B40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70B40) {
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
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// li r30,5
	ctx.r30.s64 = 5;
	// addi r11,r11,-24000
	ctx.r11.s64 = ctx.r11.s64 + -24000;
	// addi r31,r11,480
	ctx.r31.s64 = ctx.r11.s64 + 480;
loc_82A70B64:
	// addi r31,r31,-80
	ctx.r31.s64 = ctx.r31.s64 + -80;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x822bca90
	ctx.lr = 0x82A70B70;
	sub_822BCA90(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a70b64
	if (!ctx.cr6.lt) goto loc_82A70B64;
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

PPC_FUNC_IMPL(__imp__sub_82A70B98);
PPC_WEAK_FUNC(sub_82A70B98) { __imp__sub_82A70B98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70B98) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,-23480
	ctx.r3.s64 = ctx.r11.s64 + -23480;
	// b 0x8279aaf8
	sub_8279AAF8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70BA8);
PPC_WEAK_FUNC(sub_82A70BA8) { __imp__sub_82A70BA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70BA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,-23440
	ctx.r3.s64 = ctx.r11.s64 + -23440;
	// b 0x82798b00
	sub_82798B00(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70BB8);
PPC_WEAK_FUNC(sub_82A70BB8) { __imp__sub_82A70BB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70BB8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r11,r11,-764
	ctx.r11.s64 = ctx.r11.s64 + -764;
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

PPC_FUNC_IMPL(__imp__sub_82A70BD4);
PPC_WEAK_FUNC(sub_82A70BD4) { __imp__sub_82A70BD4(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70BD4) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70BD8);
PPC_WEAK_FUNC(sub_82A70BD8) { __imp__sub_82A70BD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70BD8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7cc
	ctx.lr = 0x82A70BE0;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// li r30,47
	ctx.r30.s64 = 47;
	// addi r11,r11,-528
	ctx.r11.s64 = ctx.r11.s64 + -528;
	// li r29,0
	ctx.r29.s64 = 0;
	// addi r31,r11,4496
	ctx.r31.s64 = ctx.r11.s64 + 4496;
loc_82A70BF8:
	// addi r31,r31,-92
	ctx.r31.s64 = ctx.r31.s64 + -92;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x82a70c20
	if (ctx.cr6.eq) goto loc_82A70C20;
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// li r4,1
	ctx.r4.s64 = 1;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x82A70C1C;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// stw r29,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r29.u32);
loc_82A70C20:
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r3,r31,-72
	ctx.r3.s64 = ctx.r31.s64 + -72;
	// bl 0x8244fa70
	ctx.lr = 0x82A70C2C;
	sub_8244FA70(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a70bf8
	if (!ctx.cr6.lt) goto loc_82A70BF8;
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70C40);
PPC_WEAK_FUNC(sub_82A70C40) { __imp__sub_82A70C40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70C40) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,7920
	ctx.r3.s64 = ctx.r11.s64 + 7920;
	// b 0x82460d60
	sub_82460D60(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70C50);
PPC_WEAK_FUNC(sub_82A70C50) { __imp__sub_82A70C50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70C50) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70C58);
PPC_WEAK_FUNC(sub_82A70C58) { __imp__sub_82A70C58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70C58) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70C60);
PPC_WEAK_FUNC(sub_82A70C60) { __imp__sub_82A70C60(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70C60) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70C68);
PPC_WEAK_FUNC(sub_82A70C68) { __imp__sub_82A70C68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70C68) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70C70);
PPC_WEAK_FUNC(sub_82A70C70) { __imp__sub_82A70C70(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70C70) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r11,r11,25400
	ctx.r11.s64 = ctx.r11.s64 + 25400;
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

PPC_FUNC_IMPL(__imp__sub_82A70C8C);
PPC_WEAK_FUNC(sub_82A70C8C) { __imp__sub_82A70C8C(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70C8C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70C90);
PPC_WEAK_FUNC(sub_82A70C90) { __imp__sub_82A70C90(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70C90) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r11,r11,25412
	ctx.r11.s64 = ctx.r11.s64 + 25412;
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

PPC_FUNC_IMPL(__imp__sub_82A70CAC);
PPC_WEAK_FUNC(sub_82A70CAC) { __imp__sub_82A70CAC(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70CAC) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70CB0);
PPC_WEAK_FUNC(sub_82A70CB0) { __imp__sub_82A70CB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70CB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r11,r11,25428
	ctx.r11.s64 = ctx.r11.s64 + 25428;
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

PPC_FUNC_IMPL(__imp__sub_82A70CCC);
PPC_WEAK_FUNC(sub_82A70CCC) { __imp__sub_82A70CCC(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70CCC) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70CD0);
PPC_WEAK_FUNC(sub_82A70CD0) { __imp__sub_82A70CD0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70CD0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r11,r11,25436
	ctx.r11.s64 = ctx.r11.s64 + 25436;
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

PPC_FUNC_IMPL(__imp__sub_82A70CEC);
PPC_WEAK_FUNC(sub_82A70CEC) { __imp__sub_82A70CEC(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70CEC) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70CF0);
PPC_WEAK_FUNC(sub_82A70CF0) { __imp__sub_82A70CF0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70CF0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70CF8);
PPC_WEAK_FUNC(sub_82A70CF8) { __imp__sub_82A70CF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70CF8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70D00);
PPC_WEAK_FUNC(sub_82A70D00) { __imp__sub_82A70D00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70D00) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70D08);
PPC_WEAK_FUNC(sub_82A70D08) { __imp__sub_82A70D08(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70D08) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70D10);
PPC_WEAK_FUNC(sub_82A70D10) { __imp__sub_82A70D10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70D10) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70D18);
PPC_WEAK_FUNC(sub_82A70D18) { __imp__sub_82A70D18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70D18) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70D20);
PPC_WEAK_FUNC(sub_82A70D20) { __imp__sub_82A70D20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70D20) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70D28);
PPC_WEAK_FUNC(sub_82A70D28) { __imp__sub_82A70D28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70D28) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70D30);
PPC_WEAK_FUNC(sub_82A70D30) { __imp__sub_82A70D30(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70D30) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70D38);
PPC_WEAK_FUNC(sub_82A70D38) { __imp__sub_82A70D38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70D38) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70D40);
PPC_WEAK_FUNC(sub_82A70D40) { __imp__sub_82A70D40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70D40) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70D48);
PPC_WEAK_FUNC(sub_82A70D48) { __imp__sub_82A70D48(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70D48) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70D50);
PPC_WEAK_FUNC(sub_82A70D50) { __imp__sub_82A70D50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70D50) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70D58);
PPC_WEAK_FUNC(sub_82A70D58) { __imp__sub_82A70D58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70D58) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70D60);
PPC_WEAK_FUNC(sub_82A70D60) { __imp__sub_82A70D60(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70D60) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,26352
	ctx.r3.s64 = ctx.r11.s64 + 26352;
	// b 0x826bdd88
	sub_826BDD88(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70D70);
PPC_WEAK_FUNC(sub_82A70D70) { __imp__sub_82A70D70(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70D70) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,26904
	ctx.r3.s64 = ctx.r11.s64 + 26904;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70D80);
PPC_WEAK_FUNC(sub_82A70D80) { __imp__sub_82A70D80(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70D80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r3,r11,26956
	ctx.r3.s64 = ctx.r11.s64 + 26956;
	// b 0x82847060
	sub_82847060(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70D90);
PPC_WEAK_FUNC(sub_82A70D90) { __imp__sub_82A70D90(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70D90) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70D98);
PPC_WEAK_FUNC(sub_82A70D98) { __imp__sub_82A70D98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70D98) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70DA0);
PPC_WEAK_FUNC(sub_82A70DA0) { __imp__sub_82A70DA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70DA0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70DA8);
PPC_WEAK_FUNC(sub_82A70DA8) { __imp__sub_82A70DA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70DA8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70DB0);
PPC_WEAK_FUNC(sub_82A70DB0) { __imp__sub_82A70DB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70DB0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70DB8);
PPC_WEAK_FUNC(sub_82A70DB8) { __imp__sub_82A70DB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70DB8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,26992
	ctx.r3.s64 = ctx.r11.s64 + 26992;
	// b 0x824e6cf0
	sub_824E6CF0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70DC8);
PPC_WEAK_FUNC(sub_82A70DC8) { __imp__sub_82A70DC8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70DC8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// addi r3,r11,-8272
	ctx.r3.s64 = ctx.r11.s64 + -8272;
	// b 0x824f1430
	sub_824F1430(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70DD8);
PPC_WEAK_FUNC(sub_82A70DD8) { __imp__sub_82A70DD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70DD8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31999
	ctx.r11.s64 = -2097086464;
	// addi r11,r11,27604
	ctx.r11.s64 = ctx.r11.s64 + 27604;
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

PPC_FUNC_IMPL(__imp__sub_82A70DF4);
PPC_WEAK_FUNC(sub_82A70DF4) { __imp__sub_82A70DF4(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70DF4) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70DF8);
PPC_WEAK_FUNC(sub_82A70DF8) { __imp__sub_82A70DF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70DF8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70E00);
PPC_WEAK_FUNC(sub_82A70E00) { __imp__sub_82A70E00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70E00) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// addi r11,r11,1116
	ctx.r11.s64 = ctx.r11.s64 + 1116;
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

PPC_FUNC_IMPL(__imp__sub_82A70E1C);
PPC_WEAK_FUNC(sub_82A70E1C) { __imp__sub_82A70E1C(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70E1C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70E20);
PPC_WEAK_FUNC(sub_82A70E20) { __imp__sub_82A70E20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70E20) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70E28);
PPC_WEAK_FUNC(sub_82A70E28) { __imp__sub_82A70E28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70E28) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70E30);
PPC_WEAK_FUNC(sub_82A70E30) { __imp__sub_82A70E30(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70E30) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// addi r11,r11,10024
	ctx.r11.s64 = ctx.r11.s64 + 10024;
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

PPC_FUNC_IMPL(__imp__sub_82A70E4C);
PPC_WEAK_FUNC(sub_82A70E4C) { __imp__sub_82A70E4C(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70E4C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70E50);
PPC_WEAK_FUNC(sub_82A70E50) { __imp__sub_82A70E50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70E50) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// addi r3,r11,10392
	ctx.r3.s64 = ctx.r11.s64 + 10392;
	// b 0x8251bad8
	sub_8251BAD8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70E60);
PPC_WEAK_FUNC(sub_82A70E60) { __imp__sub_82A70E60(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70E60) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// addi r3,r11,10396
	ctx.r3.s64 = ctx.r11.s64 + 10396;
	// b 0x8251bad8
	sub_8251BAD8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70E70);
PPC_WEAK_FUNC(sub_82A70E70) { __imp__sub_82A70E70(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70E70) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// addi r11,r11,10424
	ctx.r11.s64 = ctx.r11.s64 + 10424;
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

PPC_FUNC_IMPL(__imp__sub_82A70E8C);
PPC_WEAK_FUNC(sub_82A70E8C) { __imp__sub_82A70E8C(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70E8C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70E90);
PPC_WEAK_FUNC(sub_82A70E90) { __imp__sub_82A70E90(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70E90) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// addi r11,r11,10432
	ctx.r11.s64 = ctx.r11.s64 + 10432;
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

PPC_FUNC_IMPL(__imp__sub_82A70EAC);
PPC_WEAK_FUNC(sub_82A70EAC) { __imp__sub_82A70EAC(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70EAC) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70EB0);
PPC_WEAK_FUNC(sub_82A70EB0) { __imp__sub_82A70EB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70EB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// addi r11,r11,10440
	ctx.r11.s64 = ctx.r11.s64 + 10440;
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

PPC_FUNC_IMPL(__imp__sub_82A70ECC);
PPC_WEAK_FUNC(sub_82A70ECC) { __imp__sub_82A70ECC(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70ECC) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70ED0);
PPC_WEAK_FUNC(sub_82A70ED0) { __imp__sub_82A70ED0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70ED0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// addi r11,r11,10448
	ctx.r11.s64 = ctx.r11.s64 + 10448;
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

PPC_FUNC_IMPL(__imp__sub_82A70EEC);
PPC_WEAK_FUNC(sub_82A70EEC) { __imp__sub_82A70EEC(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70EEC) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70EF0);
PPC_WEAK_FUNC(sub_82A70EF0) { __imp__sub_82A70EF0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70EF0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// addi r3,r11,10596
	ctx.r3.s64 = ctx.r11.s64 + 10596;
	// b 0x8251bad8
	sub_8251BAD8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70F00);
PPC_WEAK_FUNC(sub_82A70F00) { __imp__sub_82A70F00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70F00) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// addi r3,r11,10600
	ctx.r3.s64 = ctx.r11.s64 + 10600;
	// b 0x8251bad8
	sub_8251BAD8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70F10);
PPC_WEAK_FUNC(sub_82A70F10) { __imp__sub_82A70F10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70F10) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// addi r3,r11,10604
	ctx.r3.s64 = ctx.r11.s64 + 10604;
	// b 0x8251bad8
	sub_8251BAD8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70F20);
PPC_WEAK_FUNC(sub_82A70F20) { __imp__sub_82A70F20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70F20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// addi r3,r11,10608
	ctx.r3.s64 = ctx.r11.s64 + 10608;
	// b 0x8251bad8
	sub_8251BAD8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70F30);
PPC_WEAK_FUNC(sub_82A70F30) { __imp__sub_82A70F30(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70F30) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// addi r3,r11,10736
	ctx.r3.s64 = ctx.r11.s64 + 10736;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r11,r11,13876
	ctx.r11.s64 = ctx.r11.s64 + 13876;
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r11.u32);
	// b 0x82902da0
	sub_82902DA0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70F48);
PPC_WEAK_FUNC(sub_82A70F48) { __imp__sub_82A70F48(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70F48) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70F50);
PPC_WEAK_FUNC(sub_82A70F50) { __imp__sub_82A70F50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70F50) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70F58);
PPC_WEAK_FUNC(sub_82A70F58) { __imp__sub_82A70F58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70F58) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70F60);
PPC_WEAK_FUNC(sub_82A70F60) { __imp__sub_82A70F60(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70F60) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70F68);
PPC_WEAK_FUNC(sub_82A70F68) { __imp__sub_82A70F68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70F68) {
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
	// lis r31,-31994
	ctx.r31.s64 = -2096758784;
	// lwz r3,-14864(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + -14864);
	// bl 0x821b3560
	ctx.lr = 0x82A70F84;
	sub_821B3560(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,-14864(r31)
	PPC_STORE_U32(ctx.r31.u32 + -14864, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70FA0);
PPC_WEAK_FUNC(sub_82A70FA0) { __imp__sub_82A70FA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70FA0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31993
	ctx.r11.s64 = -2096693248;
	// addi r3,r11,14072
	ctx.r3.s64 = ctx.r11.s64 + 14072;
	// b 0x8251bb98
	sub_8251BB98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70FB0);
PPC_WEAK_FUNC(sub_82A70FB0) { __imp__sub_82A70FB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70FB0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70FB8);
PPC_WEAK_FUNC(sub_82A70FB8) { __imp__sub_82A70FB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70FB8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70FC0);
PPC_WEAK_FUNC(sub_82A70FC0) { __imp__sub_82A70FC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70FC0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31993
	ctx.r11.s64 = -2096693248;
	// addi r11,r11,14080
	ctx.r11.s64 = ctx.r11.s64 + 14080;
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

PPC_FUNC_IMPL(__imp__sub_82A70FDC);
PPC_WEAK_FUNC(sub_82A70FDC) { __imp__sub_82A70FDC(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70FDC) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70FE0);
PPC_WEAK_FUNC(sub_82A70FE0) { __imp__sub_82A70FE0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70FE0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// addi r3,r11,-2016
	ctx.r3.s64 = ctx.r11.s64 + -2016;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A70FF0);
PPC_WEAK_FUNC(sub_82A70FF0) { __imp__sub_82A70FF0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A70FF0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// addi r3,r11,-1984
	ctx.r3.s64 = ctx.r11.s64 + -1984;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71000);
PPC_WEAK_FUNC(sub_82A71000) { __imp__sub_82A71000(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71000) {
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
	// li r30,1
	ctx.r30.s64 = 1;
	// addi r11,r11,-10496
	ctx.r11.s64 = ctx.r11.s64 + -10496;
	// addi r31,r11,10048
	ctx.r31.s64 = ctx.r11.s64 + 10048;
loc_82A71024:
	// lis r11,-32202
	ctx.r11.s64 = -2110390272;
	// addi r31,r31,-4240
	ctx.r31.s64 = ctx.r31.s64 + -4240;
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r6,r11,12944
	ctx.r6.s64 = ctx.r11.s64 + 12944;
	// li r4,2496
	ctx.r4.s64 = 2496;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x821401d0
	ctx.lr = 0x82A71040;
	sub_821401D0(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a71024
	if (!ctx.cr6.lt) goto loc_82A71024;
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

PPC_FUNC_IMPL(__imp__sub_82A71068);
PPC_WEAK_FUNC(sub_82A71068) { __imp__sub_82A71068(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71068) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// addi r11,r11,-1952
	ctx.r11.s64 = ctx.r11.s64 + -1952;
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

PPC_FUNC_IMPL(__imp__sub_82A71084);
PPC_WEAK_FUNC(sub_82A71084) { __imp__sub_82A71084(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71084) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71088);
PPC_WEAK_FUNC(sub_82A71088) { __imp__sub_82A71088(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71088) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// addi r11,r11,-1224
	ctx.r11.s64 = ctx.r11.s64 + -1224;
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

PPC_FUNC_IMPL(__imp__sub_82A710A4);
PPC_WEAK_FUNC(sub_82A710A4) { __imp__sub_82A710A4(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A710A4) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A710A8);
PPC_WEAK_FUNC(sub_82A710A8) { __imp__sub_82A710A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A710A8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A710B0);
PPC_WEAK_FUNC(sub_82A710B0) { __imp__sub_82A710B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A710B0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A710B8);
PPC_WEAK_FUNC(sub_82A710B8) { __imp__sub_82A710B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A710B8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A710C0);
PPC_WEAK_FUNC(sub_82A710C0) { __imp__sub_82A710C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A710C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// addi r3,r11,24720
	ctx.r3.s64 = ctx.r11.s64 + 24720;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A710D0);
PPC_WEAK_FUNC(sub_82A710D0) { __imp__sub_82A710D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A710D0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A710D8);
PPC_WEAK_FUNC(sub_82A710D8) { __imp__sub_82A710D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A710D8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A710E0);
PPC_WEAK_FUNC(sub_82A710E0) { __imp__sub_82A710E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A710E0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A710E8);
PPC_WEAK_FUNC(sub_82A710E8) { __imp__sub_82A710E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A710E8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A710F0);
PPC_WEAK_FUNC(sub_82A710F0) { __imp__sub_82A710F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A710F0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A710F8);
PPC_WEAK_FUNC(sub_82A710F8) { __imp__sub_82A710F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A710F8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71100);
PPC_WEAK_FUNC(sub_82A71100) { __imp__sub_82A71100(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71100) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71108);
PPC_WEAK_FUNC(sub_82A71108) { __imp__sub_82A71108(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71108) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71110);
PPC_WEAK_FUNC(sub_82A71110) { __imp__sub_82A71110(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71110) {
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
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// lis r10,-32086
	ctx.r10.s64 = -2102788096;
	// addi r31,r11,-15056
	ctx.r31.s64 = ctx.r11.s64 + -15056;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lfs f0,3400(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3400);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// stfs f0,-29908(r10)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + -29908, temp.u32);
	// stfs f0,-29900(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -29900, temp.u32);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,-10448
	ctx.r11.s64 = ctx.r11.s64 + -10448;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x823d4970
	ctx.lr = 0x82A71154;
	sub_823D4970(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x823d40e0
	ctx.lr = 0x82A7115C;
	sub_823D40E0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71170);
PPC_WEAK_FUNC(sub_82A71170) { __imp__sub_82A71170(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71170) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71178);
PPC_WEAK_FUNC(sub_82A71178) { __imp__sub_82A71178(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71178) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71180);
PPC_WEAK_FUNC(sub_82A71180) { __imp__sub_82A71180(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71180) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71188);
PPC_WEAK_FUNC(sub_82A71188) { __imp__sub_82A71188(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71188) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,6240
	ctx.r3.s64 = ctx.r11.s64 + 6240;
	// b 0x826bdd88
	sub_826BDD88(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71198);
PPC_WEAK_FUNC(sub_82A71198) { __imp__sub_82A71198(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71198) {
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
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r31,r11,-8768
	ctx.r31.s64 = ctx.r11.s64 + -8768;
	// addi r3,r31,11232
	ctx.r3.s64 = ctx.r31.s64 + 11232;
	// bl 0x8246a660
	ctx.lr = 0x82A711B8;
	sub_8246A660(ctx, base);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r11,r11,13876
	ctx.r11.s64 = ctx.r11.s64 + 13876;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82902da0
	ctx.lr = 0x82A711CC;
	sub_82902DA0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A711E0);
PPC_WEAK_FUNC(sub_82A711E0) { __imp__sub_82A711E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A711E0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A711E8);
PPC_WEAK_FUNC(sub_82A711E8) { __imp__sub_82A711E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A711E8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A711F0);
PPC_WEAK_FUNC(sub_82A711F0) { __imp__sub_82A711F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A711F0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A711F8);
PPC_WEAK_FUNC(sub_82A711F8) { __imp__sub_82A711F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A711F8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71200);
PPC_WEAK_FUNC(sub_82A71200) { __imp__sub_82A71200(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71200) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71208);
PPC_WEAK_FUNC(sub_82A71208) { __imp__sub_82A71208(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71208) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71210);
PPC_WEAK_FUNC(sub_82A71210) { __imp__sub_82A71210(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71210) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71218);
PPC_WEAK_FUNC(sub_82A71218) { __imp__sub_82A71218(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71218) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71220);
PPC_WEAK_FUNC(sub_82A71220) { __imp__sub_82A71220(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71220) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71228);
PPC_WEAK_FUNC(sub_82A71228) { __imp__sub_82A71228(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71228) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71230);
PPC_WEAK_FUNC(sub_82A71230) { __imp__sub_82A71230(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71230) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71238);
PPC_WEAK_FUNC(sub_82A71238) { __imp__sub_82A71238(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71238) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71240);
PPC_WEAK_FUNC(sub_82A71240) { __imp__sub_82A71240(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71240) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71248);
PPC_WEAK_FUNC(sub_82A71248) { __imp__sub_82A71248(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71248) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71250);
PPC_WEAK_FUNC(sub_82A71250) { __imp__sub_82A71250(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71250) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71258);
PPC_WEAK_FUNC(sub_82A71258) { __imp__sub_82A71258(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71258) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71260);
PPC_WEAK_FUNC(sub_82A71260) { __imp__sub_82A71260(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71260) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71268);
PPC_WEAK_FUNC(sub_82A71268) { __imp__sub_82A71268(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71268) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71270);
PPC_WEAK_FUNC(sub_82A71270) { __imp__sub_82A71270(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71270) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71278);
PPC_WEAK_FUNC(sub_82A71278) { __imp__sub_82A71278(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71278) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71280);
PPC_WEAK_FUNC(sub_82A71280) { __imp__sub_82A71280(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71280) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71288);
PPC_WEAK_FUNC(sub_82A71288) { __imp__sub_82A71288(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71288) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71290);
PPC_WEAK_FUNC(sub_82A71290) { __imp__sub_82A71290(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71290) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71298);
PPC_WEAK_FUNC(sub_82A71298) { __imp__sub_82A71298(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71298) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A712A0);
PPC_WEAK_FUNC(sub_82A712A0) { __imp__sub_82A712A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A712A0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A712A8);
PPC_WEAK_FUNC(sub_82A712A8) { __imp__sub_82A712A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A712A8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A712B0);
PPC_WEAK_FUNC(sub_82A712B0) { __imp__sub_82A712B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A712B0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A712B8);
PPC_WEAK_FUNC(sub_82A712B8) { __imp__sub_82A712B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A712B8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A712C0);
PPC_WEAK_FUNC(sub_82A712C0) { __imp__sub_82A712C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A712C0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A712C8);
PPC_WEAK_FUNC(sub_82A712C8) { __imp__sub_82A712C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A712C8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A712D0);
PPC_WEAK_FUNC(sub_82A712D0) { __imp__sub_82A712D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A712D0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A712D8);
PPC_WEAK_FUNC(sub_82A712D8) { __imp__sub_82A712D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A712D8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A712E0);
PPC_WEAK_FUNC(sub_82A712E0) { __imp__sub_82A712E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A712E0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A712E8);
PPC_WEAK_FUNC(sub_82A712E8) { __imp__sub_82A712E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A712E8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A712F0);
PPC_WEAK_FUNC(sub_82A712F0) { __imp__sub_82A712F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A712F0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A712F8);
PPC_WEAK_FUNC(sub_82A712F8) { __imp__sub_82A712F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A712F8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71300);
PPC_WEAK_FUNC(sub_82A71300) { __imp__sub_82A71300(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71300) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71308);
PPC_WEAK_FUNC(sub_82A71308) { __imp__sub_82A71308(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71308) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71310);
PPC_WEAK_FUNC(sub_82A71310) { __imp__sub_82A71310(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71310) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71318);
PPC_WEAK_FUNC(sub_82A71318) { __imp__sub_82A71318(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71318) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71320);
PPC_WEAK_FUNC(sub_82A71320) { __imp__sub_82A71320(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71320) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71328);
PPC_WEAK_FUNC(sub_82A71328) { __imp__sub_82A71328(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71328) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71330);
PPC_WEAK_FUNC(sub_82A71330) { __imp__sub_82A71330(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71330) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71338);
PPC_WEAK_FUNC(sub_82A71338) { __imp__sub_82A71338(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71338) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71340);
PPC_WEAK_FUNC(sub_82A71340) { __imp__sub_82A71340(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71340) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71348);
PPC_WEAK_FUNC(sub_82A71348) { __imp__sub_82A71348(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71348) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71350);
PPC_WEAK_FUNC(sub_82A71350) { __imp__sub_82A71350(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71350) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71358);
PPC_WEAK_FUNC(sub_82A71358) { __imp__sub_82A71358(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71358) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71360);
PPC_WEAK_FUNC(sub_82A71360) { __imp__sub_82A71360(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71360) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71368);
PPC_WEAK_FUNC(sub_82A71368) { __imp__sub_82A71368(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71368) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71370);
PPC_WEAK_FUNC(sub_82A71370) { __imp__sub_82A71370(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71370) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71378);
PPC_WEAK_FUNC(sub_82A71378) { __imp__sub_82A71378(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71378) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71380);
PPC_WEAK_FUNC(sub_82A71380) { __imp__sub_82A71380(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71380) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71388);
PPC_WEAK_FUNC(sub_82A71388) { __imp__sub_82A71388(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71388) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71390);
PPC_WEAK_FUNC(sub_82A71390) { __imp__sub_82A71390(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71390) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71398);
PPC_WEAK_FUNC(sub_82A71398) { __imp__sub_82A71398(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71398) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A713A0);
PPC_WEAK_FUNC(sub_82A713A0) { __imp__sub_82A713A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A713A0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A713A8);
PPC_WEAK_FUNC(sub_82A713A8) { __imp__sub_82A713A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A713A8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A713B0);
PPC_WEAK_FUNC(sub_82A713B0) { __imp__sub_82A713B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A713B0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A713B8);
PPC_WEAK_FUNC(sub_82A713B8) { __imp__sub_82A713B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A713B8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A713C0);
PPC_WEAK_FUNC(sub_82A713C0) { __imp__sub_82A713C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A713C0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A713C8);
PPC_WEAK_FUNC(sub_82A713C8) { __imp__sub_82A713C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A713C8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A713D0);
PPC_WEAK_FUNC(sub_82A713D0) { __imp__sub_82A713D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A713D0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A713D8);
PPC_WEAK_FUNC(sub_82A713D8) { __imp__sub_82A713D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A713D8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A713E0);
PPC_WEAK_FUNC(sub_82A713E0) { __imp__sub_82A713E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A713E0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A713E8);
PPC_WEAK_FUNC(sub_82A713E8) { __imp__sub_82A713E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A713E8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A713F0);
PPC_WEAK_FUNC(sub_82A713F0) { __imp__sub_82A713F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A713F0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A713F8);
PPC_WEAK_FUNC(sub_82A713F8) { __imp__sub_82A713F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A713F8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71400);
PPC_WEAK_FUNC(sub_82A71400) { __imp__sub_82A71400(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71400) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71408);
PPC_WEAK_FUNC(sub_82A71408) { __imp__sub_82A71408(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71408) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71410);
PPC_WEAK_FUNC(sub_82A71410) { __imp__sub_82A71410(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71410) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71418);
PPC_WEAK_FUNC(sub_82A71418) { __imp__sub_82A71418(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71418) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71420);
PPC_WEAK_FUNC(sub_82A71420) { __imp__sub_82A71420(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71420) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71428);
PPC_WEAK_FUNC(sub_82A71428) { __imp__sub_82A71428(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71428) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71430);
PPC_WEAK_FUNC(sub_82A71430) { __imp__sub_82A71430(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71430) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71438);
PPC_WEAK_FUNC(sub_82A71438) { __imp__sub_82A71438(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71438) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71440);
PPC_WEAK_FUNC(sub_82A71440) { __imp__sub_82A71440(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71440) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71448);
PPC_WEAK_FUNC(sub_82A71448) { __imp__sub_82A71448(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71448) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r11,r11,15524
	ctx.r11.s64 = ctx.r11.s64 + 15524;
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

PPC_FUNC_IMPL(__imp__sub_82A71464);
PPC_WEAK_FUNC(sub_82A71464) { __imp__sub_82A71464(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71464) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71468);
PPC_WEAK_FUNC(sub_82A71468) { __imp__sub_82A71468(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71468) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71470);
PPC_WEAK_FUNC(sub_82A71470) { __imp__sub_82A71470(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71470) {
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
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r31,r11,19408
	ctx.r31.s64 = ctx.r11.s64 + 19408;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// bl 0x821b3560
	ctx.lr = 0x82A71490;
	sub_821B3560(ctx, base);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,13876
	ctx.r11.s64 = ctx.r11.s64 + 13876;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r10,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r10.u32);
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82902da0
	ctx.lr = 0x82A714AC;
	sub_82902DA0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A714C0);
PPC_WEAK_FUNC(sub_82A714C0) { __imp__sub_82A714C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A714C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// addi r11,r11,23424
	ctx.r11.s64 = ctx.r11.s64 + 23424;
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

PPC_FUNC_IMPL(__imp__sub_82A714DC);
PPC_WEAK_FUNC(sub_82A714DC) { __imp__sub_82A714DC(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A714DC) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A714E0);
PPC_WEAK_FUNC(sub_82A714E0) { __imp__sub_82A714E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A714E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// addi r3,r11,24344
	ctx.r3.s64 = ctx.r11.s64 + 24344;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r11,r11,13876
	ctx.r11.s64 = ctx.r11.s64 + 13876;
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r11.u32);
	// b 0x82902da0
	sub_82902DA0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A714F8);
PPC_WEAK_FUNC(sub_82A714F8) { __imp__sub_82A714F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A714F8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71500);
PPC_WEAK_FUNC(sub_82A71500) { __imp__sub_82A71500(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71500) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71508);
PPC_WEAK_FUNC(sub_82A71508) { __imp__sub_82A71508(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71508) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// addi r3,r11,-29384
	ctx.r3.s64 = ctx.r11.s64 + -29384;
	// b 0x82902da0
	sub_82902DA0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71518);
PPC_WEAK_FUNC(sub_82A71518) { __imp__sub_82A71518(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71518) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71520);
PPC_WEAK_FUNC(sub_82A71520) { __imp__sub_82A71520(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71520) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71528);
PPC_WEAK_FUNC(sub_82A71528) { __imp__sub_82A71528(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71528) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71530);
PPC_WEAK_FUNC(sub_82A71530) { __imp__sub_82A71530(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71530) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71538);
PPC_WEAK_FUNC(sub_82A71538) { __imp__sub_82A71538(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71538) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71540);
PPC_WEAK_FUNC(sub_82A71540) { __imp__sub_82A71540(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71540) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71548);
PPC_WEAK_FUNC(sub_82A71548) { __imp__sub_82A71548(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71548) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// addi r3,r11,-26752
	ctx.r3.s64 = ctx.r11.s64 + -26752;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r11,r11,13876
	ctx.r11.s64 = ctx.r11.s64 + 13876;
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r11.u32);
	// b 0x82902da0
	sub_82902DA0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71560);
PPC_WEAK_FUNC(sub_82A71560) { __imp__sub_82A71560(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71560) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// addi r3,r11,-19184
	ctx.r3.s64 = ctx.r11.s64 + -19184;
	// b 0x828bdab8
	sub_828BDAB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71570);
PPC_WEAK_FUNC(sub_82A71570) { __imp__sub_82A71570(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71570) {
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
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// addi r31,r11,-20160
	ctx.r31.s64 = ctx.r11.s64 + -20160;
	// addi r3,r31,96
	ctx.r3.s64 = ctx.r31.s64 + 96;
	// bl 0x8286a5d8
	ctx.lr = 0x82A71590;
	sub_8286A5D8(ctx, base);
	// lwz r3,32(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 32);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x82a715ac
	if (ctx.cr6.eq) goto loc_82A715AC;
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x82A715AC;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
loc_82A715AC:
	// lwz r3,28(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 28);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x82a715c8
	if (ctx.cr6.eq) goto loc_82A715C8;
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x82A715C8;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
loc_82A715C8:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A715E0);
PPC_WEAK_FUNC(sub_82A715E0) { __imp__sub_82A715E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A715E0) {
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
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// li r30,1
	ctx.r30.s64 = 1;
	// addi r11,r11,-19808
	ctx.r11.s64 = ctx.r11.s64 + -19808;
	// addi r31,r11,416
	ctx.r31.s64 = ctx.r11.s64 + 416;
loc_82A71604:
	// addi r31,r31,-208
	ctx.r31.s64 = ctx.r31.s64 + -208;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8286a5d8
	ctx.lr = 0x82A71610;
	sub_8286A5D8(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a71604
	if (!ctx.cr6.lt) goto loc_82A71604;
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

PPC_FUNC_IMPL(__imp__sub_82A71638);
PPC_WEAK_FUNC(sub_82A71638) { __imp__sub_82A71638(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71638) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7cc
	ctx.lr = 0x82A71640;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// li r30,1
	ctx.r30.s64 = 1;
	// addi r11,r11,-19392
	ctx.r11.s64 = ctx.r11.s64 + -19392;
	// addi r31,r11,192
	ctx.r31.s64 = ctx.r11.s64 + 192;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r29,r11,17948
	ctx.r29.s64 = ctx.r11.s64 + 17948;
loc_82A7165C:
	// addi r31,r31,-96
	ctx.r31.s64 = ctx.r31.s64 + -96;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r29,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r29.u32);
	// bl 0x827d5590
	ctx.lr = 0x82A7166C;
	sub_827D5590(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8286a5d8
	ctx.lr = 0x82A71674;
	sub_8286A5D8(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a7165c
	if (!ctx.cr6.lt) goto loc_82A7165C;
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71688);
PPC_WEAK_FUNC(sub_82A71688) { __imp__sub_82A71688(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71688) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// lwz r3,-19200(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + -19200);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctr 
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A716B0);
PPC_WEAK_FUNC(sub_82A716B0) { __imp__sub_82A716B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A716B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// addi r3,r11,-20368
	ctx.r3.s64 = ctx.r11.s64 + -20368;
	// b 0x82672378
	sub_82672378(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A716C0);
PPC_WEAK_FUNC(sub_82A716C0) { __imp__sub_82A716C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A716C0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A716C8);
PPC_WEAK_FUNC(sub_82A716C8) { __imp__sub_82A716C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A716C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// addi r11,r11,-1680
	ctx.r11.s64 = ctx.r11.s64 + -1680;
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

PPC_FUNC_IMPL(__imp__sub_82A716E4);
PPC_WEAK_FUNC(sub_82A716E4) { __imp__sub_82A716E4(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A716E4) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A716E8);
PPC_WEAK_FUNC(sub_82A716E8) { __imp__sub_82A716E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A716E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// addi r11,r11,-1672
	ctx.r11.s64 = ctx.r11.s64 + -1672;
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

PPC_FUNC_IMPL(__imp__sub_82A71704);
PPC_WEAK_FUNC(sub_82A71704) { __imp__sub_82A71704(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71704) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71708);
PPC_WEAK_FUNC(sub_82A71708) { __imp__sub_82A71708(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71708) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// addi r11,r11,-1664
	ctx.r11.s64 = ctx.r11.s64 + -1664;
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

PPC_FUNC_IMPL(__imp__sub_82A71724);
PPC_WEAK_FUNC(sub_82A71724) { __imp__sub_82A71724(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71724) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71728);
PPC_WEAK_FUNC(sub_82A71728) { __imp__sub_82A71728(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71728) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// addi r11,r11,-400
	ctx.r11.s64 = ctx.r11.s64 + -400;
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

PPC_FUNC_IMPL(__imp__sub_82A71744);
PPC_WEAK_FUNC(sub_82A71744) { __imp__sub_82A71744(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71744) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71748);
PPC_WEAK_FUNC(sub_82A71748) { __imp__sub_82A71748(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71748) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31987
	ctx.r11.s64 = -2096300032;
	// lwz r3,-21732(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + -21732);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// b 0x821b3560
	sub_821B3560(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A7175C);
PPC_WEAK_FUNC(sub_82A7175C) { __imp__sub_82A7175C(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A7175C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71760);
PPC_WEAK_FUNC(sub_82A71760) { __imp__sub_82A71760(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71760) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71768);
PPC_WEAK_FUNC(sub_82A71768) { __imp__sub_82A71768(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71768) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71770);
PPC_WEAK_FUNC(sub_82A71770) { __imp__sub_82A71770(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71770) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71778);
PPC_WEAK_FUNC(sub_82A71778) { __imp__sub_82A71778(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71778) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31987
	ctx.r11.s64 = -2096300032;
	// addi r3,r11,-20848
	ctx.r3.s64 = ctx.r11.s64 + -20848;
	// lhz r5,6(r3)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r3.u32 + 6);
	// cmplwi cr6,r5,0
	ctx.cr6.compare<uint32_t>(ctx.r5.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r4,0(r3)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// b 0x826c3ae0
	sub_826C3AE0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71794);
PPC_WEAK_FUNC(sub_82A71794) { __imp__sub_82A71794(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71794) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71798);
PPC_WEAK_FUNC(sub_82A71798) { __imp__sub_82A71798(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71798) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r3,r11,-17456
	ctx.r3.s64 = ctx.r11.s64 + -17456;
	// b 0x828486b0
	sub_828486B0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A717A8);
PPC_WEAK_FUNC(sub_82A717A8) { __imp__sub_82A717A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A717A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r3,r11,-15152
	ctx.r3.s64 = ctx.r11.s64 + -15152;
	// b 0x828486b0
	sub_828486B0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A717B8);
PPC_WEAK_FUNC(sub_82A717B8) { __imp__sub_82A717B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A717B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31978
	ctx.r11.s64 = -2095710208;
	// addi r3,r11,-2020
	ctx.r3.s64 = ctx.r11.s64 + -2020;
	// b 0x829e5560
	sub_829E5560(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A717C8);
PPC_WEAK_FUNC(sub_82A717C8) { __imp__sub_82A717C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A717C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31978
	ctx.r11.s64 = -2095710208;
	// addi r3,r11,-4944
	ctx.r3.s64 = ctx.r11.s64 + -4944;
	// b 0x829ebc30
	sub_829EBC30(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A717D8);
PPC_WEAK_FUNC(sub_82A717D8) { __imp__sub_82A717D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A717D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r3,r11,-12292
	ctx.r3.s64 = ctx.r11.s64 + -12292;
	// b 0x826f2040
	sub_826F2040(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A717E8);
PPC_WEAK_FUNC(sub_82A717E8) { __imp__sub_82A717E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A717E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31984
	ctx.r11.s64 = -2096103424;
	// addi r3,r11,-15084
	ctx.r3.s64 = ctx.r11.s64 + -15084;
	// b 0x826e2cc8
	sub_826E2CC8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A717F8);
PPC_WEAK_FUNC(sub_82A717F8) { __imp__sub_82A717F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A717F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31978
	ctx.r11.s64 = -2095710208;
	// addi r3,r11,-1936
	ctx.r3.s64 = ctx.r11.s64 + -1936;
	// b 0x826de388
	sub_826DE388(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71808);
PPC_WEAK_FUNC(sub_82A71808) { __imp__sub_82A71808(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71808) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31984
	ctx.r11.s64 = -2096103424;
	// addi r3,r11,-16528
	ctx.r3.s64 = ctx.r11.s64 + -16528;
	// b 0x829e2258
	sub_829E2258(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71818);
PPC_WEAK_FUNC(sub_82A71818) { __imp__sub_82A71818(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71818) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31978
	ctx.r11.s64 = -2095710208;
	// addi r3,r11,-2224
	ctx.r3.s64 = ctx.r11.s64 + -2224;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71828);
PPC_WEAK_FUNC(sub_82A71828) { __imp__sub_82A71828(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71828) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r3,r11,-25492
	ctx.r3.s64 = ctx.r11.s64 + -25492;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71838);
PPC_WEAK_FUNC(sub_82A71838) { __imp__sub_82A71838(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71838) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31978
	ctx.r11.s64 = -2095710208;
	// addi r3,r11,-2176
	ctx.r3.s64 = ctx.r11.s64 + -2176;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71848);
PPC_WEAK_FUNC(sub_82A71848) { __imp__sub_82A71848(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71848) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r3,r11,-25464
	ctx.r3.s64 = ctx.r11.s64 + -25464;
	// b 0x82701e08
	sub_82701E08(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71858);
PPC_WEAK_FUNC(sub_82A71858) { __imp__sub_82A71858(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71858) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31978
	ctx.r11.s64 = -2095710208;
	// addi r3,r11,-5952
	ctx.r3.s64 = ctx.r11.s64 + -5952;
	// b 0x829eb520
	sub_829EB520(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71868);
PPC_WEAK_FUNC(sub_82A71868) { __imp__sub_82A71868(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71868) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71870);
PPC_WEAK_FUNC(sub_82A71870) { __imp__sub_82A71870(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71870) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71878);
PPC_WEAK_FUNC(sub_82A71878) { __imp__sub_82A71878(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71878) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71880);
PPC_WEAK_FUNC(sub_82A71880) { __imp__sub_82A71880(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71880) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31978
	ctx.r11.s64 = -2095710208;
	// addi r3,r11,29600
	ctx.r3.s64 = ctx.r11.s64 + 29600;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71890);
PPC_WEAK_FUNC(sub_82A71890) { __imp__sub_82A71890(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71890) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r3,r11,-25760
	ctx.r3.s64 = ctx.r11.s64 + -25760;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A718A0);
PPC_WEAK_FUNC(sub_82A718A0) { __imp__sub_82A718A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A718A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r3,r11,-12836
	ctx.r3.s64 = ctx.r11.s64 + -12836;
	// b 0x829e9b18
	sub_829E9B18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A718B0);
PPC_WEAK_FUNC(sub_82A718B0) { __imp__sub_82A718B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A718B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31978
	ctx.r11.s64 = -2095710208;
	// addi r3,r11,-2280
	ctx.r3.s64 = ctx.r11.s64 + -2280;
	// b 0x829e9b18
	sub_829E9B18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A718C0);
PPC_WEAK_FUNC(sub_82A718C0) { __imp__sub_82A718C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A718C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31984
	ctx.r11.s64 = -2096103424;
	// addi r3,r11,-15140
	ctx.r3.s64 = ctx.r11.s64 + -15140;
	// b 0x829e9b18
	sub_829E9B18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A718D0);
PPC_WEAK_FUNC(sub_82A718D0) { __imp__sub_82A718D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A718D0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A718D8);
PPC_WEAK_FUNC(sub_82A718D8) { __imp__sub_82A718D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A718D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r3,r11,-12376
	ctx.r3.s64 = ctx.r11.s64 + -12376;
	// b 0x829e9b18
	sub_829E9B18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A718E8);
PPC_WEAK_FUNC(sub_82A718E8) { __imp__sub_82A718E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A718E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// lis r10,-31978
	ctx.r10.s64 = -2095710208;
	// addi r11,r11,9752
	ctx.r11.s64 = ctx.r11.s64 + 9752;
	// stw r11,29868(r10)
	PPC_STORE_U32(ctx.r10.u32 + 29868, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71900);
PPC_WEAK_FUNC(sub_82A71900) { __imp__sub_82A71900(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71900) {
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
	// lis r10,-31978
	ctx.r10.s64 = -2095710208;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,-5000
	ctx.r31.s64 = ctx.r10.s64 + -5000;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A71928;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A71930;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A71950);
PPC_WEAK_FUNC(sub_82A71950) { __imp__sub_82A71950(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71950) {
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
	// lis r10,-31978
	ctx.r10.s64 = -2095710208;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,-2128
	ctx.r31.s64 = ctx.r10.s64 + -2128;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A71978;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A71980;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A719A0);
PPC_WEAK_FUNC(sub_82A719A0) { __imp__sub_82A719A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A719A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31978
	ctx.r11.s64 = -2095710208;
	// addi r3,r11,-1352
	ctx.r3.s64 = ctx.r11.s64 + -1352;
	// b 0x826cab88
	sub_826CAB88(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A719B0);
PPC_WEAK_FUNC(sub_82A719B0) { __imp__sub_82A719B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A719B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,-9852
	ctx.r3.s64 = ctx.r11.s64 + -9852;
	// b 0x829efac8
	sub_829EFAC8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A719C0);
PPC_WEAK_FUNC(sub_82A719C0) { __imp__sub_82A719C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A719C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31976
	ctx.r11.s64 = -2095579136;
	// addi r3,r11,-2336
	ctx.r3.s64 = ctx.r11.s64 + -2336;
	// b 0x829e9508
	sub_829E9508(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A719D0);
PPC_WEAK_FUNC(sub_82A719D0) { __imp__sub_82A719D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A719D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31976
	ctx.r11.s64 = -2095579136;
	// addi r3,r11,152
	ctx.r3.s64 = ctx.r11.s64 + 152;
	// b 0x828486b0
	sub_828486B0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A719E0);
PPC_WEAK_FUNC(sub_82A719E0) { __imp__sub_82A719E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A719E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31976
	ctx.r11.s64 = -2095579136;
	// addi r3,r11,-2448
	ctx.r3.s64 = ctx.r11.s64 + -2448;
	// b 0x829e7060
	sub_829E7060(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A719F0);
PPC_WEAK_FUNC(sub_82A719F0) { __imp__sub_82A719F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A719F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31976
	ctx.r11.s64 = -2095579136;
	// addi r3,r11,-1992
	ctx.r3.s64 = ctx.r11.s64 + -1992;
	// b 0x829ef118
	sub_829EF118(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71A00);
PPC_WEAK_FUNC(sub_82A71A00) { __imp__sub_82A71A00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71A00) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,-9960
	ctx.r3.s64 = ctx.r11.s64 + -9960;
	// b 0x8278f2c0
	sub_8278F2C0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71A10);
PPC_WEAK_FUNC(sub_82A71A10) { __imp__sub_82A71A10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71A10) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31976
	ctx.r11.s64 = -2095579136;
	// addi r3,r11,-1920
	ctx.r3.s64 = ctx.r11.s64 + -1920;
	// b 0x829e39c0
	sub_829E39C0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71A20);
PPC_WEAK_FUNC(sub_82A71A20) { __imp__sub_82A71A20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71A20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31976
	ctx.r11.s64 = -2095579136;
	// addi r3,r11,22048
	ctx.r3.s64 = ctx.r11.s64 + 22048;
	// b 0x829ef5b0
	sub_829EF5B0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71A30);
PPC_WEAK_FUNC(sub_82A71A30) { __imp__sub_82A71A30(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71A30) {
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
	// lis r11,-31976
	ctx.r11.s64 = -2095579136;
	// li r30,99
	ctx.r30.s64 = 99;
	// addi r11,r11,2448
	ctx.r11.s64 = ctx.r11.s64 + 2448;
	// addi r31,r11,19600
	ctx.r31.s64 = ctx.r11.s64 + 19600;
loc_82A71A54:
	// addi r31,r31,-196
	ctx.r31.s64 = ctx.r31.s64 + -196;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x822bca90
	ctx.lr = 0x82A71A60;
	sub_822BCA90(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a71a54
	if (!ctx.cr6.lt) goto loc_82A71A54;
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

PPC_FUNC_IMPL(__imp__sub_82A71A88);
PPC_WEAK_FUNC(sub_82A71A88) { __imp__sub_82A71A88(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71A88) {
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
	// lis r11,-31976
	ctx.r11.s64 = -2095579136;
	// li r30,99
	ctx.r30.s64 = 99;
	// addi r11,r11,24168
	ctx.r11.s64 = ctx.r11.s64 + 24168;
	// addi r31,r11,19600
	ctx.r31.s64 = ctx.r11.s64 + 19600;
loc_82A71AAC:
	// addi r31,r31,-196
	ctx.r31.s64 = ctx.r31.s64 + -196;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x822bca90
	ctx.lr = 0x82A71AB8;
	sub_822BCA90(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a71aac
	if (!ctx.cr6.lt) goto loc_82A71AAC;
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

PPC_FUNC_IMPL(__imp__sub_82A71AE0);
PPC_WEAK_FUNC(sub_82A71AE0) { __imp__sub_82A71AE0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71AE0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71AE8);
PPC_WEAK_FUNC(sub_82A71AE8) { __imp__sub_82A71AE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71AE8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71AF0);
PPC_WEAK_FUNC(sub_82A71AF0) { __imp__sub_82A71AF0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71AF0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31976
	ctx.r11.s64 = -2095579136;
	// addi r3,r11,-2372
	ctx.r3.s64 = ctx.r11.s64 + -2372;
	// b 0x829e4270
	sub_829E4270(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71B00);
PPC_WEAK_FUNC(sub_82A71B00) { __imp__sub_82A71B00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71B00) {
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
	// li r30,79
	ctx.r30.s64 = 79;
	// addi r11,r11,-21480
	ctx.r11.s64 = ctx.r11.s64 + -21480;
	// addi r31,r11,11520
	ctx.r31.s64 = ctx.r11.s64 + 11520;
loc_82A71B24:
	// addi r31,r31,-144
	ctx.r31.s64 = ctx.r31.s64 + -144;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x822bca90
	ctx.lr = 0x82A71B30;
	sub_822BCA90(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a71b24
	if (!ctx.cr6.lt) goto loc_82A71B24;
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

PPC_FUNC_IMPL(__imp__sub_82A71B58);
PPC_WEAK_FUNC(sub_82A71B58) { __imp__sub_82A71B58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71B58) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71B60);
PPC_WEAK_FUNC(sub_82A71B60) { __imp__sub_82A71B60(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71B60) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31976
	ctx.r11.s64 = -2095579136;
	// addi r3,r11,-1932
	ctx.r3.s64 = ctx.r11.s64 + -1932;
	// b 0x826ccec0
	sub_826CCEC0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71B70);
PPC_WEAK_FUNC(sub_82A71B70) { __imp__sub_82A71B70(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71B70) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,-9716
	ctx.r31.s64 = ctx.r10.s64 + -9716;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A71B98;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A71BA0;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A71BC0);
PPC_WEAK_FUNC(sub_82A71BC0) { __imp__sub_82A71BC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71BC0) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,-9684
	ctx.r31.s64 = ctx.r10.s64 + -9684;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A71BE8;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A71BF0;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A71C10);
PPC_WEAK_FUNC(sub_82A71C10) { __imp__sub_82A71C10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71C10) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,-9580
	ctx.r31.s64 = ctx.r10.s64 + -9580;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A71C38;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A71C40;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A71C60);
PPC_WEAK_FUNC(sub_82A71C60) { __imp__sub_82A71C60(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71C60) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,-9612
	ctx.r31.s64 = ctx.r10.s64 + -9612;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A71C88;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A71C90;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A71CB0);
PPC_WEAK_FUNC(sub_82A71CB0) { __imp__sub_82A71CB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71CB0) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,-9644
	ctx.r31.s64 = ctx.r10.s64 + -9644;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A71CD8;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A71CE0;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A71D00);
PPC_WEAK_FUNC(sub_82A71D00) { __imp__sub_82A71D00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71D00) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,-7952
	ctx.r31.s64 = ctx.r10.s64 + -7952;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A71D28;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A71D30;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A71D50);
PPC_WEAK_FUNC(sub_82A71D50) { __imp__sub_82A71D50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71D50) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,-7888
	ctx.r31.s64 = ctx.r10.s64 + -7888;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A71D78;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A71D80;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A71DA0);
PPC_WEAK_FUNC(sub_82A71DA0) { __imp__sub_82A71DA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71DA0) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,-7856
	ctx.r31.s64 = ctx.r10.s64 + -7856;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A71DC8;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A71DD0;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A71DF0);
PPC_WEAK_FUNC(sub_82A71DF0) { __imp__sub_82A71DF0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71DF0) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,-7920
	ctx.r31.s64 = ctx.r10.s64 + -7920;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A71E18;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A71E20;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A71E40);
PPC_WEAK_FUNC(sub_82A71E40) { __imp__sub_82A71E40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71E40) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,-6416
	ctx.r3.s64 = ctx.r11.s64 + -6416;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71E50);
PPC_WEAK_FUNC(sub_82A71E50) { __imp__sub_82A71E50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71E50) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71E58);
PPC_WEAK_FUNC(sub_82A71E58) { __imp__sub_82A71E58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71E58) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A71E60);
PPC_WEAK_FUNC(sub_82A71E60) { __imp__sub_82A71E60(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71E60) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,-5028
	ctx.r31.s64 = ctx.r10.s64 + -5028;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A71E88;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A71E90;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A71EB0);
PPC_WEAK_FUNC(sub_82A71EB0) { __imp__sub_82A71EB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71EB0) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,-4996
	ctx.r31.s64 = ctx.r10.s64 + -4996;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A71ED8;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A71EE0;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A71F00);
PPC_WEAK_FUNC(sub_82A71F00) { __imp__sub_82A71F00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71F00) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,-5092
	ctx.r31.s64 = ctx.r10.s64 + -5092;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A71F28;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A71F30;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A71F50);
PPC_WEAK_FUNC(sub_82A71F50) { __imp__sub_82A71F50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71F50) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,-5188
	ctx.r31.s64 = ctx.r10.s64 + -5188;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A71F78;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A71F80;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A71FA0);
PPC_WEAK_FUNC(sub_82A71FA0) { __imp__sub_82A71FA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71FA0) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,-5060
	ctx.r31.s64 = ctx.r10.s64 + -5060;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A71FC8;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A71FD0;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A71FF0);
PPC_WEAK_FUNC(sub_82A71FF0) { __imp__sub_82A71FF0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A71FF0) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,-5156
	ctx.r31.s64 = ctx.r10.s64 + -5156;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A72018;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72020;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72040);
PPC_WEAK_FUNC(sub_82A72040) { __imp__sub_82A72040(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72040) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,-5124
	ctx.r31.s64 = ctx.r10.s64 + -5124;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A72068;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72070;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72090);
PPC_WEAK_FUNC(sub_82A72090) { __imp__sub_82A72090(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72090) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// addi r11,r11,21000
	ctx.r11.s64 = ctx.r11.s64 + 21000;
	// stw r11,-4508(r10)
	PPC_STORE_U32(ctx.r10.u32 + -4508, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A720A8);
PPC_WEAK_FUNC(sub_82A720A8) { __imp__sub_82A720A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A720A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// addi r11,r11,21000
	ctx.r11.s64 = ctx.r11.s64 + 21000;
	// stw r11,-4496(r10)
	PPC_STORE_U32(ctx.r10.u32 + -4496, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A720C0);
PPC_WEAK_FUNC(sub_82A720C0) { __imp__sub_82A720C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A720C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// addi r11,r11,21000
	ctx.r11.s64 = ctx.r11.s64 + 21000;
	// stw r11,-4484(r10)
	PPC_STORE_U32(ctx.r10.u32 + -4484, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A720D8);
PPC_WEAK_FUNC(sub_82A720D8) { __imp__sub_82A720D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A720D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// addi r11,r11,21000
	ctx.r11.s64 = ctx.r11.s64 + 21000;
	// stw r11,-4472(r10)
	PPC_STORE_U32(ctx.r10.u32 + -4472, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A720F0);
PPC_WEAK_FUNC(sub_82A720F0) { __imp__sub_82A720F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A720F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// addi r11,r11,21000
	ctx.r11.s64 = ctx.r11.s64 + 21000;
	// stw r11,-4460(r10)
	PPC_STORE_U32(ctx.r10.u32 + -4460, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A72108);
PPC_WEAK_FUNC(sub_82A72108) { __imp__sub_82A72108(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72108) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// addi r11,r11,21000
	ctx.r11.s64 = ctx.r11.s64 + 21000;
	// stw r11,-4448(r10)
	PPC_STORE_U32(ctx.r10.u32 + -4448, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A72120);
PPC_WEAK_FUNC(sub_82A72120) { __imp__sub_82A72120(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72120) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// addi r11,r11,21000
	ctx.r11.s64 = ctx.r11.s64 + 21000;
	// stw r11,-4436(r10)
	PPC_STORE_U32(ctx.r10.u32 + -4436, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A72138);
PPC_WEAK_FUNC(sub_82A72138) { __imp__sub_82A72138(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72138) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// addi r11,r11,21000
	ctx.r11.s64 = ctx.r11.s64 + 21000;
	// stw r11,-4424(r10)
	PPC_STORE_U32(ctx.r10.u32 + -4424, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A72150);
PPC_WEAK_FUNC(sub_82A72150) { __imp__sub_82A72150(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72150) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// addi r11,r11,21000
	ctx.r11.s64 = ctx.r11.s64 + 21000;
	// stw r11,-4412(r10)
	PPC_STORE_U32(ctx.r10.u32 + -4412, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A72168);
PPC_WEAK_FUNC(sub_82A72168) { __imp__sub_82A72168(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72168) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,-4344
	ctx.r31.s64 = ctx.r10.s64 + -4344;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A72190;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72198;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A721B8);
PPC_WEAK_FUNC(sub_82A721B8) { __imp__sub_82A721B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A721B8) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,-4376
	ctx.r31.s64 = ctx.r10.s64 + -4376;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A721E0;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A721E8;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72208);
PPC_WEAK_FUNC(sub_82A72208) { __imp__sub_82A72208(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72208) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,-4176
	ctx.r31.s64 = ctx.r10.s64 + -4176;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A72230;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72238;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72258);
PPC_WEAK_FUNC(sub_82A72258) { __imp__sub_82A72258(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72258) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,-4144
	ctx.r31.s64 = ctx.r10.s64 + -4144;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A72280;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72288;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A722A8);
PPC_WEAK_FUNC(sub_82A722A8) { __imp__sub_82A722A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A722A8) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,-4272
	ctx.r31.s64 = ctx.r10.s64 + -4272;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A722D0;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A722D8;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A722F8);
PPC_WEAK_FUNC(sub_82A722F8) { __imp__sub_82A722F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A722F8) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,-4304
	ctx.r31.s64 = ctx.r10.s64 + -4304;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A72320;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72328;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72348);
PPC_WEAK_FUNC(sub_82A72348) { __imp__sub_82A72348(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72348) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,-4208
	ctx.r31.s64 = ctx.r10.s64 + -4208;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A72370;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72378;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72398);
PPC_WEAK_FUNC(sub_82A72398) { __imp__sub_82A72398(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72398) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,-4240
	ctx.r31.s64 = ctx.r10.s64 + -4240;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A723C0;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A723C8;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A723E8);
PPC_WEAK_FUNC(sub_82A723E8) { __imp__sub_82A723E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A723E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// lis r10,-32082
	ctx.r10.s64 = -2102525952;
	// addi r11,r11,-31284
	ctx.r11.s64 = ctx.r11.s64 + -31284;
	// addi r10,r10,2832
	ctx.r10.s64 = ctx.r10.s64 + 2832;
	// stw r11,16(r10)
	PPC_STORE_U32(ctx.r10.u32 + 16, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A72400);
PPC_WEAK_FUNC(sub_82A72400) { __imp__sub_82A72400(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72400) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,-4108
	ctx.r3.s64 = ctx.r11.s64 + -4108;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A72410);
PPC_WEAK_FUNC(sub_82A72410) { __imp__sub_82A72410(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72410) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r11,r11,-4080
	ctx.r11.s64 = ctx.r11.s64 + -4080;
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

PPC_FUNC_IMPL(__imp__sub_82A7242C);
PPC_WEAK_FUNC(sub_82A7242C) { __imp__sub_82A7242C(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A7242C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A72430);
PPC_WEAK_FUNC(sub_82A72430) { __imp__sub_82A72430(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72430) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,-4072
	ctx.r3.s64 = ctx.r11.s64 + -4072;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A72440);
PPC_WEAK_FUNC(sub_82A72440) { __imp__sub_82A72440(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72440) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,-4048
	ctx.r3.s64 = ctx.r11.s64 + -4048;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A72450);
PPC_WEAK_FUNC(sub_82A72450) { __imp__sub_82A72450(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72450) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,-4024
	ctx.r3.s64 = ctx.r11.s64 + -4024;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A72460);
PPC_WEAK_FUNC(sub_82A72460) { __imp__sub_82A72460(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72460) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,-4000
	ctx.r3.s64 = ctx.r11.s64 + -4000;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A72470);
PPC_WEAK_FUNC(sub_82A72470) { __imp__sub_82A72470(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72470) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,-3976
	ctx.r3.s64 = ctx.r11.s64 + -3976;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A72480);
PPC_WEAK_FUNC(sub_82A72480) { __imp__sub_82A72480(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72480) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,-3952
	ctx.r3.s64 = ctx.r11.s64 + -3952;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A72490);
PPC_WEAK_FUNC(sub_82A72490) { __imp__sub_82A72490(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72490) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,-3928
	ctx.r3.s64 = ctx.r11.s64 + -3928;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A724A0);
PPC_WEAK_FUNC(sub_82A724A0) { __imp__sub_82A724A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A724A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,-3904
	ctx.r3.s64 = ctx.r11.s64 + -3904;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A724B0);
PPC_WEAK_FUNC(sub_82A724B0) { __imp__sub_82A724B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A724B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,-3880
	ctx.r3.s64 = ctx.r11.s64 + -3880;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A724C0);
PPC_WEAK_FUNC(sub_82A724C0) { __imp__sub_82A724C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A724C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,-3856
	ctx.r3.s64 = ctx.r11.s64 + -3856;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A724D0);
PPC_WEAK_FUNC(sub_82A724D0) { __imp__sub_82A724D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A724D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,-3832
	ctx.r3.s64 = ctx.r11.s64 + -3832;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A724E0);
PPC_WEAK_FUNC(sub_82A724E0) { __imp__sub_82A724E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A724E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,-1076
	ctx.r3.s64 = ctx.r11.s64 + -1076;
	// b 0x829dc318
	sub_829DC318(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A724F0);
PPC_WEAK_FUNC(sub_82A724F0) { __imp__sub_82A724F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A724F0) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r31,r10,-632
	ctx.r31.s64 = ctx.r10.s64 + -632;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A72518;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72520;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72540);
PPC_WEAK_FUNC(sub_82A72540) { __imp__sub_82A72540(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72540) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r31,r10,-760
	ctx.r31.s64 = ctx.r10.s64 + -760;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A72568;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72570;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72590);
PPC_WEAK_FUNC(sub_82A72590) { __imp__sub_82A72590(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72590) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r31,r10,-824
	ctx.r31.s64 = ctx.r10.s64 + -824;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A725B8;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A725C0;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A725E0);
PPC_WEAK_FUNC(sub_82A725E0) { __imp__sub_82A725E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A725E0) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r31,r10,-984
	ctx.r31.s64 = ctx.r10.s64 + -984;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A72608;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72610;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72630);
PPC_WEAK_FUNC(sub_82A72630) { __imp__sub_82A72630(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72630) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r31,r10,-696
	ctx.r31.s64 = ctx.r10.s64 + -696;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A72658;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72660;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72680);
PPC_WEAK_FUNC(sub_82A72680) { __imp__sub_82A72680(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72680) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r31,r10,-1016
	ctx.r31.s64 = ctx.r10.s64 + -1016;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A726A8;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A726B0;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A726D0);
PPC_WEAK_FUNC(sub_82A726D0) { __imp__sub_82A726D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A726D0) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r31,r10,-920
	ctx.r31.s64 = ctx.r10.s64 + -920;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A726F8;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72700;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72720);
PPC_WEAK_FUNC(sub_82A72720) { __imp__sub_82A72720(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72720) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r31,r10,-1048
	ctx.r31.s64 = ctx.r10.s64 + -1048;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A72748;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72750;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72770);
PPC_WEAK_FUNC(sub_82A72770) { __imp__sub_82A72770(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72770) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r31,r10,-792
	ctx.r31.s64 = ctx.r10.s64 + -792;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A72798;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A727A0;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A727C0);
PPC_WEAK_FUNC(sub_82A727C0) { __imp__sub_82A727C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A727C0) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r31,r10,-856
	ctx.r31.s64 = ctx.r10.s64 + -856;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A727E8;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A727F0;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72810);
PPC_WEAK_FUNC(sub_82A72810) { __imp__sub_82A72810(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72810) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r31,r10,-888
	ctx.r31.s64 = ctx.r10.s64 + -888;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A72838;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72840;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72860);
PPC_WEAK_FUNC(sub_82A72860) { __imp__sub_82A72860(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72860) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r31,r10,-728
	ctx.r31.s64 = ctx.r10.s64 + -728;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A72888;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72890;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A728B0);
PPC_WEAK_FUNC(sub_82A728B0) { __imp__sub_82A728B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A728B0) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r31,r10,-952
	ctx.r31.s64 = ctx.r10.s64 + -952;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A728D8;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A728E0;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72900);
PPC_WEAK_FUNC(sub_82A72900) { __imp__sub_82A72900(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72900) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r31,r10,-664
	ctx.r31.s64 = ctx.r10.s64 + -664;
	// addi r11,r11,-20320
	ctx.r11.s64 = ctx.r11.s64 + -20320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827caba8
	ctx.lr = 0x82A72928;
	sub_827CABA8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72930;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72950);
PPC_WEAK_FUNC(sub_82A72950) { __imp__sub_82A72950(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72950) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-26352
	ctx.r11.s64 = ctx.r11.s64 + -26352;
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

PPC_FUNC_IMPL(__imp__sub_82A7296C);
PPC_WEAK_FUNC(sub_82A7296C) { __imp__sub_82A7296C(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A7296C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A72970);
PPC_WEAK_FUNC(sub_82A72970) { __imp__sub_82A72970(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72970) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,-26340
	ctx.r11.s64 = ctx.r11.s64 + -26340;
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

PPC_FUNC_IMPL(__imp__sub_82A7298C);
PPC_WEAK_FUNC(sub_82A7298C) { __imp__sub_82A7298C(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A7298C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A72990);
PPC_WEAK_FUNC(sub_82A72990) { __imp__sub_82A72990(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72990) {
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
	// lhz r11,18(r31)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r31.u32 + 18);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a729bc
	if (ctx.cr6.eq) goto loc_82A729BC;
	// lwz r3,12(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 12);
	// bl 0x821b3560
	ctx.lr = 0x82A729BC;
	sub_821B3560(ctx, base);
loc_82A729BC:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x827fa060
	ctx.lr = 0x82A729C4;
	sub_827FA060(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A729D8);
PPC_WEAK_FUNC(sub_82A729D8) { __imp__sub_82A729D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A729D8) {
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
	// lhz r11,18(r31)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r31.u32 + 18);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a72a04
	if (ctx.cr6.eq) goto loc_82A72A04;
	// lwz r3,12(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 12);
	// bl 0x821b3560
	ctx.lr = 0x82A72A04;
	sub_821B3560(ctx, base);
loc_82A72A04:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x827fa060
	ctx.lr = 0x82A72A0C;
	sub_827FA060(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A72A20);
PPC_WEAK_FUNC(sub_82A72A20) { __imp__sub_82A72A20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72A20) {
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
	// lhz r11,18(r31)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r31.u32 + 18);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a72a4c
	if (ctx.cr6.eq) goto loc_82A72A4C;
	// lwz r3,12(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 12);
	// bl 0x821b3560
	ctx.lr = 0x82A72A4C;
	sub_821B3560(ctx, base);
loc_82A72A4C:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x827fa060
	ctx.lr = 0x82A72A54;
	sub_827FA060(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A72A68);
PPC_WEAK_FUNC(sub_82A72A68) { __imp__sub_82A72A68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72A68) {
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
	// lhz r11,18(r31)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r31.u32 + 18);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a72a94
	if (ctx.cr6.eq) goto loc_82A72A94;
	// lwz r3,12(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 12);
	// bl 0x821b3560
	ctx.lr = 0x82A72A94;
	sub_821B3560(ctx, base);
loc_82A72A94:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x827fa060
	ctx.lr = 0x82A72A9C;
	sub_827FA060(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A72AB0);
PPC_WEAK_FUNC(sub_82A72AB0) { __imp__sub_82A72AB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72AB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r3,r11,17660
	ctx.r3.s64 = ctx.r11.s64 + 17660;
	// b 0x82847060
	sub_82847060(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A72AC0);
PPC_WEAK_FUNC(sub_82A72AC0) { __imp__sub_82A72AC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72AC0) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,1824
	ctx.r31.s64 = ctx.r10.s64 + 1824;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A72AE8;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72AF0;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72B10);
PPC_WEAK_FUNC(sub_82A72B10) { __imp__sub_82A72B10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72B10) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,1952
	ctx.r31.s64 = ctx.r10.s64 + 1952;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A72B38;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72B40;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72B60);
PPC_WEAK_FUNC(sub_82A72B60) { __imp__sub_82A72B60(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72B60) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,1536
	ctx.r31.s64 = ctx.r10.s64 + 1536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A72B88;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72B90;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72BB0);
PPC_WEAK_FUNC(sub_82A72BB0) { __imp__sub_82A72BB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72BB0) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,1920
	ctx.r31.s64 = ctx.r10.s64 + 1920;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A72BD8;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72BE0;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72C00);
PPC_WEAK_FUNC(sub_82A72C00) { __imp__sub_82A72C00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72C00) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,1856
	ctx.r31.s64 = ctx.r10.s64 + 1856;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A72C28;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72C30;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72C50);
PPC_WEAK_FUNC(sub_82A72C50) { __imp__sub_82A72C50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72C50) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,1600
	ctx.r31.s64 = ctx.r10.s64 + 1600;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A72C78;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72C80;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72CA0);
PPC_WEAK_FUNC(sub_82A72CA0) { __imp__sub_82A72CA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72CA0) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,1760
	ctx.r31.s64 = ctx.r10.s64 + 1760;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A72CC8;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72CD0;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72CF0);
PPC_WEAK_FUNC(sub_82A72CF0) { __imp__sub_82A72CF0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72CF0) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,1472
	ctx.r31.s64 = ctx.r10.s64 + 1472;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A72D18;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72D20;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72D40);
PPC_WEAK_FUNC(sub_82A72D40) { __imp__sub_82A72D40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72D40) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,1632
	ctx.r31.s64 = ctx.r10.s64 + 1632;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A72D68;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72D70;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72D90);
PPC_WEAK_FUNC(sub_82A72D90) { __imp__sub_82A72D90(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72D90) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,1984
	ctx.r31.s64 = ctx.r10.s64 + 1984;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A72DB8;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72DC0;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72DE0);
PPC_WEAK_FUNC(sub_82A72DE0) { __imp__sub_82A72DE0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72DE0) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,1792
	ctx.r31.s64 = ctx.r10.s64 + 1792;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A72E08;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72E10;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72E30);
PPC_WEAK_FUNC(sub_82A72E30) { __imp__sub_82A72E30(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72E30) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,1568
	ctx.r31.s64 = ctx.r10.s64 + 1568;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A72E58;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72E60;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72E80);
PPC_WEAK_FUNC(sub_82A72E80) { __imp__sub_82A72E80(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72E80) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,1696
	ctx.r31.s64 = ctx.r10.s64 + 1696;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A72EA8;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72EB0;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72ED0);
PPC_WEAK_FUNC(sub_82A72ED0) { __imp__sub_82A72ED0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72ED0) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,1504
	ctx.r31.s64 = ctx.r10.s64 + 1504;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A72EF8;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72F00;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72F20);
PPC_WEAK_FUNC(sub_82A72F20) { __imp__sub_82A72F20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72F20) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,1888
	ctx.r31.s64 = ctx.r10.s64 + 1888;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A72F48;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72F50;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72F70);
PPC_WEAK_FUNC(sub_82A72F70) { __imp__sub_82A72F70(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72F70) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,1728
	ctx.r31.s64 = ctx.r10.s64 + 1728;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A72F98;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72FA0;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A72FC0);
PPC_WEAK_FUNC(sub_82A72FC0) { __imp__sub_82A72FC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A72FC0) {
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
	// lis r10,-31975
	ctx.r10.s64 = -2095513600;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r31,r10,1664
	ctx.r31.s64 = ctx.r10.s64 + 1664;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A72FE8;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc090
	ctx.lr = 0x82A72FF0;
	sub_829DC090(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,9744
	ctx.r11.s64 = ctx.r11.s64 + 9744;
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

PPC_FUNC_IMPL(__imp__sub_82A73010);
PPC_WEAK_FUNC(sub_82A73010) { __imp__sub_82A73010(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A73010) {
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
	// addi r11,r11,2024
	ctx.r11.s64 = ctx.r11.s64 + 2024;
	// addi r31,r11,7204
	ctx.r31.s64 = ctx.r11.s64 + 7204;
loc_82A73034:
	// addi r31,r31,-112
	ctx.r31.s64 = ctx.r31.s64 + -112;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829fa668
	ctx.lr = 0x82A73040;
	sub_829FA668(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a73034
	if (!ctx.cr6.lt) goto loc_82A73034;
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

PPC_FUNC_IMPL(__imp__sub_82A73068);
PPC_WEAK_FUNC(sub_82A73068) { __imp__sub_82A73068(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A73068) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r3,r11,27088
	ctx.r3.s64 = ctx.r11.s64 + 27088;
	// b 0x82847060
	sub_82847060(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A73078);
PPC_WEAK_FUNC(sub_82A73078) { __imp__sub_82A73078(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A73078) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,10012
	ctx.r3.s64 = ctx.r11.s64 + 10012;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A73088);
PPC_WEAK_FUNC(sub_82A73088) { __imp__sub_82A73088(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A73088) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,10028
	ctx.r3.s64 = ctx.r11.s64 + 10028;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A73098);
PPC_WEAK_FUNC(sub_82A73098) { __imp__sub_82A73098(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A73098) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r11,r11,10048
	ctx.r11.s64 = ctx.r11.s64 + 10048;
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

PPC_FUNC_IMPL(__imp__sub_82A730B4);
PPC_WEAK_FUNC(sub_82A730B4) { __imp__sub_82A730B4(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A730B4) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A730B8);
PPC_WEAK_FUNC(sub_82A730B8) { __imp__sub_82A730B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A730B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32080
	ctx.r10.s64 = -2102394880;
	// addi r11,r11,14192
	ctx.r11.s64 = ctx.r11.s64 + 14192;
	// stw r11,27420(r10)
	PPC_STORE_U32(ctx.r10.u32 + 27420, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A730D0);
PPC_WEAK_FUNC(sub_82A730D0) { __imp__sub_82A730D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A730D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r11,r11,10060
	ctx.r11.s64 = ctx.r11.s64 + 10060;
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

PPC_FUNC_IMPL(__imp__sub_82A730EC);
PPC_WEAK_FUNC(sub_82A730EC) { __imp__sub_82A730EC(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A730EC) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A730F0);
PPC_WEAK_FUNC(sub_82A730F0) { __imp__sub_82A730F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A730F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,10068
	ctx.r3.s64 = ctx.r11.s64 + 10068;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A73100);
PPC_WEAK_FUNC(sub_82A73100) { __imp__sub_82A73100(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A73100) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,10084
	ctx.r3.s64 = ctx.r11.s64 + 10084;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A73110);
PPC_WEAK_FUNC(sub_82A73110) { __imp__sub_82A73110(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A73110) {
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
	// lis r31,-31975
	ctx.r31.s64 = -2095513600;
	// lwz r3,10144(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 10144);
	// bl 0x821b3560
	ctx.lr = 0x82A7312C;
	sub_821B3560(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,10144(r31)
	PPC_STORE_U32(ctx.r31.u32 + 10144, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A73148);
PPC_WEAK_FUNC(sub_82A73148) { __imp__sub_82A73148(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A73148) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r11,r11,10120
	ctx.r11.s64 = ctx.r11.s64 + 10120;
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

PPC_FUNC_IMPL(__imp__sub_82A73164);
PPC_WEAK_FUNC(sub_82A73164) { __imp__sub_82A73164(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A73164) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A73168);
PPC_WEAK_FUNC(sub_82A73168) { __imp__sub_82A73168(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A73168) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r11,r11,10136
	ctx.r11.s64 = ctx.r11.s64 + 10136;
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

PPC_FUNC_IMPL(__imp__sub_82A73184);
PPC_WEAK_FUNC(sub_82A73184) { __imp__sub_82A73184(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A73184) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A73188);
PPC_WEAK_FUNC(sub_82A73188) { __imp__sub_82A73188(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A73188) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r11,r11,10128
	ctx.r11.s64 = ctx.r11.s64 + 10128;
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

PPC_FUNC_IMPL(__imp__sub_82A731A4);
PPC_WEAK_FUNC(sub_82A731A4) { __imp__sub_82A731A4(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A731A4) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A731A8);
PPC_WEAK_FUNC(sub_82A731A8) { __imp__sub_82A731A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A731A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,10180
	ctx.r3.s64 = ctx.r11.s64 + 10180;
	// b 0x82849bc0
	sub_82849BC0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A731B8);
PPC_WEAK_FUNC(sub_82A731B8) { __imp__sub_82A731B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A731B8) {
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
	// lis r31,-32080
	ctx.r31.s64 = -2102394880;
	// lwz r3,28704(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 28704);
	// bl 0x821b3560
	ctx.lr = 0x82A731D4;
	sub_821B3560(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,28704(r31)
	PPC_STORE_U32(ctx.r31.u32 + 28704, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A731F0);
PPC_WEAK_FUNC(sub_82A731F0) { __imp__sub_82A731F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A731F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,10556
	ctx.r3.s64 = ctx.r11.s64 + 10556;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A73200);
PPC_WEAK_FUNC(sub_82A73200) { __imp__sub_82A73200(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A73200) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,10592
	ctx.r3.s64 = ctx.r11.s64 + 10592;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A73210);
PPC_WEAK_FUNC(sub_82A73210) { __imp__sub_82A73210(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A73210) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,11352
	ctx.r3.s64 = ctx.r11.s64 + 11352;
	// b 0x8284bef8
	sub_8284BEF8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A73220);
PPC_WEAK_FUNC(sub_82A73220) { __imp__sub_82A73220(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A73220) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,12240
	ctx.r3.s64 = ctx.r11.s64 + 12240;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A73230);
PPC_WEAK_FUNC(sub_82A73230) { __imp__sub_82A73230(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A73230) {
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
	// lis r11,-31974
	ctx.r11.s64 = -2095448064;
	// li r30,1
	ctx.r30.s64 = 1;
	// addi r11,r11,-3336
	ctx.r11.s64 = ctx.r11.s64 + -3336;
	// addis r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 65536;
	// addi r31,r11,9248
	ctx.r31.s64 = ctx.r11.s64 + 9248;
loc_82A73258:
	// addi r31,r31,-24944
	ctx.r31.s64 = ctx.r31.s64 + -24944;
	// lwz r3,44(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 44);
	// bl 0x828498b0
	ctx.lr = 0x82A73264;
	sub_828498B0(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x822bca90
	ctx.lr = 0x82A7326C;
	sub_822BCA90(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a73258
	if (!ctx.cr6.lt) goto loc_82A73258;
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

PPC_FUNC_IMPL(__imp__sub_82A73290);
PPC_WEAK_FUNC(sub_82A73290) { __imp__sub_82A73290(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A73290) {
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
	// li r30,1
	ctx.r30.s64 = 1;
	// addi r11,r11,12312
	ctx.r11.s64 = ctx.r11.s64 + 12312;
	// addis r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 65536;
	// addi r31,r11,9248
	ctx.r31.s64 = ctx.r11.s64 + 9248;
loc_82A732B8:
	// addi r31,r31,-24944
	ctx.r31.s64 = ctx.r31.s64 + -24944;
	// lwz r3,44(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 44);
	// bl 0x828498b0
	ctx.lr = 0x82A732C4;
	sub_828498B0(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x822bca90
	ctx.lr = 0x82A732CC;
	sub_822BCA90(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a732b8
	if (!ctx.cr6.lt) goto loc_82A732B8;
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

PPC_FUNC_IMPL(__imp__sub_82A732F0);
PPC_WEAK_FUNC(sub_82A732F0) { __imp__sub_82A732F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A732F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r3,r11,-18968
	ctx.r3.s64 = ctx.r11.s64 + -18968;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A73300);
PPC_WEAK_FUNC(sub_82A73300) { __imp__sub_82A73300(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A73300) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r3,r11,-18884
	ctx.r3.s64 = ctx.r11.s64 + -18884;
	// b 0x822bca90
	sub_822BCA90(ctx, base);
	return;
}

