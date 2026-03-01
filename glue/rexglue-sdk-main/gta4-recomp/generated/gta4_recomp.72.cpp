#include "gta4_init.h"

PPC_FUNC_IMPL(__imp__sub_829FE530);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE530) { __imp__sub_829FE530(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE530) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32015
	ctx.r11.s64 = -2098135040;
	// addi r11,r11,19596
	ctx.r11.s64 = ctx.r11.s64 + 19596;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE54C);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE54C) { __imp__sub_829FE54C(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE54C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE550);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE550) { __imp__sub_829FE550(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE550) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32015
	ctx.r10.s64 = -2098135040;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// addi r31,r10,19744
	ctx.r31.s64 = ctx.r10.s64 + 19744;
	// addi r11,r11,-30436
	ctx.r11.s64 = ctx.r11.s64 + -30436;
	// lhz r10,98(r31)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r31.u32 + 98);
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x829fe588
	if (ctx.cr6.eq) goto loc_829FE588;
	// lwz r3,92(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 92);
	// bl 0x8218be78
	ctx.lr = 0x829FE588;
	sub_8218BE78(ctx, base);
loc_829FE588:
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r11,r11,9888
	ctx.r11.s64 = ctx.r11.s64 + 9888;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82893c88
	ctx.lr = 0x829FE59C;
	sub_82893C88(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE5B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE5B0) { __imp__sub_829FE5B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE5B0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE5B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE5B8) { __imp__sub_829FE5B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE5B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,-30400
	ctx.r3.s64 = ctx.r11.s64 + -30400;
	// b 0x82409508
	sub_82409508(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE5C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE5C8) { __imp__sub_829FE5C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE5C8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE5D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE5D0) { __imp__sub_829FE5D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE5D0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE5D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE5D8) { __imp__sub_829FE5D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE5D8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE5E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE5E0) { __imp__sub_829FE5E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE5E0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE5E8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE5E8) { __imp__sub_829FE5E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE5E8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x829FE5F0;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// li r30,47
	ctx.r30.s64 = 47;
	// addi r11,r11,-12384
	ctx.r11.s64 = ctx.r11.s64 + -12384;
	// li r29,0
	ctx.r29.s64 = 0;
	// addi r31,r11,4496
	ctx.r31.s64 = ctx.r11.s64 + 4496;
loc_829FE608:
	// addi r31,r31,-92
	ctx.r31.s64 = ctx.r31.s64 + -92;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x829fe630
	if (ctx.cr6.eq) goto loc_829FE630;
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// li r4,1
	ctx.r4.s64 = 1;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x829FE62C;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// stw r29,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r29.u32);
loc_829FE630:
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r3,r31,-72
	ctx.r3.s64 = ctx.r31.s64 + -72;
	// bl 0x8243c1b8
	ctx.lr = 0x829FE63C;
	sub_8243C1B8(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829fe608
	if (!ctx.cr6.lt) goto loc_829FE608;
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE650);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE650) { __imp__sub_829FE650(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE650) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r11,r11,-7872
	ctx.r11.s64 = ctx.r11.s64 + -7872;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE66C);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE66C) { __imp__sub_829FE66C(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE66C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE670);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE670) { __imp__sub_829FE670(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE670) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r11,r11,-7864
	ctx.r11.s64 = ctx.r11.s64 + -7864;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE68C);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE68C) { __imp__sub_829FE68C(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE68C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE690);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE690) { __imp__sub_829FE690(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE690) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE698);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE698) { __imp__sub_829FE698(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE698) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE6A0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE6A0) { __imp__sub_829FE6A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE6A0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE6A8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE6A8) { __imp__sub_829FE6A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE6A8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE6B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE6B0) { __imp__sub_829FE6B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE6B0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE6B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE6B8) { __imp__sub_829FE6B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE6B8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE6C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE6C0) { __imp__sub_829FE6C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE6C0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE6C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE6C8) { __imp__sub_829FE6C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE6C8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE6D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE6D0) { __imp__sub_829FE6D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE6D0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE6D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE6D8) { __imp__sub_829FE6D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE6D8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE6E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE6E0) { __imp__sub_829FE6E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE6E0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE6E8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE6E8) { __imp__sub_829FE6E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE6E8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE6F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE6F0) { __imp__sub_829FE6F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE6F0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE6F8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE6F8) { __imp__sub_829FE6F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE6F8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE700);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE700) { __imp__sub_829FE700(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE700) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,-6884
	ctx.r3.s64 = ctx.r11.s64 + -6884;
	// b 0x8266ca80
	sub_8266CA80(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE710);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE710) { __imp__sub_829FE710(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE710) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r11,r11,-2984
	ctx.r11.s64 = ctx.r11.s64 + -2984;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE72C);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE72C) { __imp__sub_829FE72C(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE72C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE730);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE730) { __imp__sub_829FE730(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE730) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r11,r11,-2972
	ctx.r11.s64 = ctx.r11.s64 + -2972;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE74C);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE74C) { __imp__sub_829FE74C(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE74C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE750);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE750) { __imp__sub_829FE750(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE750) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,-2956
	ctx.r3.s64 = ctx.r11.s64 + -2956;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE760);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE760) { __imp__sub_829FE760(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE760) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r3,r11,20692
	ctx.r3.s64 = ctx.r11.s64 + 20692;
	// b 0x827d8560
	sub_827D8560(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE770);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE770) { __imp__sub_829FE770(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE770) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r11,r11,-2492
	ctx.r11.s64 = ctx.r11.s64 + -2492;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE78C);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE78C) { __imp__sub_829FE78C(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE78C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE790);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE790) { __imp__sub_829FE790(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE790) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE798);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE798) { __imp__sub_829FE798(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE798) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE7A0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE7A0) { __imp__sub_829FE7A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE7A0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE7A8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE7A8) { __imp__sub_829FE7A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE7A8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE7B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE7B0) { __imp__sub_829FE7B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE7B0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE7B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE7B8) { __imp__sub_829FE7B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE7B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,-2432
	ctx.r3.s64 = ctx.r11.s64 + -2432;
	// b 0x824ac518
	sub_824AC518(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE7C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE7C8) { __imp__sub_829FE7C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE7C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,27840
	ctx.r3.s64 = ctx.r11.s64 + 27840;
	// b 0x824b68f8
	sub_824B68F8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE7D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE7D8) { __imp__sub_829FE7D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE7D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32011
	ctx.r11.s64 = -2097872896;
	// addi r11,r11,-19772
	ctx.r11.s64 = ctx.r11.s64 + -19772;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE7F4);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE7F4) { __imp__sub_829FE7F4(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE7F4) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE7F8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE7F8) { __imp__sub_829FE7F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE7F8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE800);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE800) { __imp__sub_829FE800(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE800) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE808);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE808) { __imp__sub_829FE808(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE808) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE810);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE810) { __imp__sub_829FE810(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE810) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32011
	ctx.r11.s64 = -2097872896;
	// addi r3,r11,-5792
	ctx.r3.s64 = ctx.r11.s64 + -5792;
	// b 0x825b5ab8
	sub_825B5AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE820);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE820) { __imp__sub_829FE820(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE820) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32011
	ctx.r11.s64 = -2097872896;
	// addi r3,r11,-5788
	ctx.r3.s64 = ctx.r11.s64 + -5788;
	// b 0x825b5ab8
	sub_825B5AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE830);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE830) { __imp__sub_829FE830(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE830) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32011
	ctx.r11.s64 = -2097872896;
	// addi r11,r11,-5764
	ctx.r11.s64 = ctx.r11.s64 + -5764;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE84C);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE84C) { __imp__sub_829FE84C(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE84C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE850);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE850) { __imp__sub_829FE850(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE850) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32011
	ctx.r11.s64 = -2097872896;
	// addi r11,r11,-5756
	ctx.r11.s64 = ctx.r11.s64 + -5756;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE86C);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE86C) { __imp__sub_829FE86C(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE86C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE870);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE870) { __imp__sub_829FE870(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE870) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32011
	ctx.r11.s64 = -2097872896;
	// addi r11,r11,-5748
	ctx.r11.s64 = ctx.r11.s64 + -5748;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE88C);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE88C) { __imp__sub_829FE88C(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE88C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE890);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE890) { __imp__sub_829FE890(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE890) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32011
	ctx.r11.s64 = -2097872896;
	// addi r11,r11,-5740
	ctx.r11.s64 = ctx.r11.s64 + -5740;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE8AC);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE8AC) { __imp__sub_829FE8AC(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE8AC) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE8B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE8B0) { __imp__sub_829FE8B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE8B0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE8B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE8B8) { __imp__sub_829FE8B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE8B8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE8C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE8C0) { __imp__sub_829FE8C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE8C0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE8C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE8C8) { __imp__sub_829FE8C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE8C8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE8D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE8D0) { __imp__sub_829FE8D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE8D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32009
	ctx.r11.s64 = -2097741824;
	// addi r31,r11,31456
	ctx.r31.s64 = ctx.r11.s64 + 31456;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// bl 0x8218be78
	ctx.lr = 0x829FE8F0;
	sub_8218BE78(ctx, base);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,9888
	ctx.r11.s64 = ctx.r11.s64 + 9888;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r10,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r10.u32);
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82893c88
	ctx.lr = 0x829FE90C;
	sub_82893C88(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE920);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE920) { __imp__sub_829FE920(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE920) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// addi r11,r11,-30064
	ctx.r11.s64 = ctx.r11.s64 + -30064;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE93C);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE93C) { __imp__sub_829FE93C(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE93C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE940);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE940) { __imp__sub_829FE940(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE940) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// addi r3,r11,-29816
	ctx.r3.s64 = ctx.r11.s64 + -29816;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r11,r11,9888
	ctx.r11.s64 = ctx.r11.s64 + 9888;
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r11.u32);
	// b 0x82893c88
	sub_82893C88(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE958);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE958) { __imp__sub_829FE958(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE958) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32007
	ctx.r11.s64 = -2097610752;
	// addi r11,r11,-11104
	ctx.r11.s64 = ctx.r11.s64 + -11104;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE974);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE974) { __imp__sub_829FE974(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE974) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE978);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE978) { __imp__sub_829FE978(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE978) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE980);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE980) { __imp__sub_829FE980(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE980) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE988);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE988) { __imp__sub_829FE988(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE988) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32006
	ctx.r11.s64 = -2097545216;
	// addi r3,r11,-4544
	ctx.r3.s64 = ctx.r11.s64 + -4544;
	// b 0x8284ece8
	sub_8284ECE8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE998);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE998) { __imp__sub_829FE998(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE998) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32006
	ctx.r11.s64 = -2097545216;
	// addi r31,r11,-5520
	ctx.r31.s64 = ctx.r11.s64 + -5520;
	// addi r3,r31,96
	ctx.r3.s64 = ctx.r31.s64 + 96;
	// bl 0x827fb5e0
	ctx.lr = 0x829FE9B8;
	sub_827FB5E0(ctx, base);
	// lwz r3,32(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 32);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x829fe9d4
	if (ctx.cr6.eq) goto loc_829FE9D4;
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x829FE9D4;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
loc_829FE9D4:
	// lwz r3,28(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 28);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x829fe9f0
	if (ctx.cr6.eq) goto loc_829FE9F0;
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x829FE9F0;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
loc_829FE9F0:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEA08);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEA08) { __imp__sub_829FEA08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEA08) {
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
	// li r30,1
	ctx.r30.s64 = 1;
	// addi r11,r11,-5168
	ctx.r11.s64 = ctx.r11.s64 + -5168;
	// addi r31,r11,416
	ctx.r31.s64 = ctx.r11.s64 + 416;
loc_829FEA2C:
	// addi r31,r31,-208
	ctx.r31.s64 = ctx.r31.s64 + -208;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x827fb5e0
	ctx.lr = 0x829FEA38;
	sub_827FB5E0(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829fea2c
	if (!ctx.cr6.lt) goto loc_829FEA2C;
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

PPC_FUNC_IMPL(__imp__sub_829FEA60);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEA60) { __imp__sub_829FEA60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEA60) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x829FEA68;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32006
	ctx.r11.s64 = -2097545216;
	// li r30,1
	ctx.r30.s64 = 1;
	// addi r11,r11,-4752
	ctx.r11.s64 = ctx.r11.s64 + -4752;
	// addi r31,r11,192
	ctx.r31.s64 = ctx.r11.s64 + 192;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// addi r29,r11,25556
	ctx.r29.s64 = ctx.r11.s64 + 25556;
loc_829FEA84:
	// addi r31,r31,-96
	ctx.r31.s64 = ctx.r31.s64 + -96;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r29,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r29.u32);
	// bl 0x8275bad8
	ctx.lr = 0x829FEA94;
	sub_8275BAD8(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x827fb5e0
	ctx.lr = 0x829FEA9C;
	sub_827FB5E0(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829fea84
	if (!ctx.cr6.lt) goto loc_829FEA84;
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEAB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEAB0) { __imp__sub_829FEAB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEAB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32006
	ctx.r11.s64 = -2097545216;
	// lwz r3,-4560(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + -4560);
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

PPC_FUNC_IMPL(__imp__sub_829FEAD8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEAD8) { __imp__sub_829FEAD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEAD8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32006
	ctx.r11.s64 = -2097545216;
	// addi r3,r11,-5728
	ctx.r3.s64 = ctx.r11.s64 + -5728;
	// b 0x8251cd00
	sub_8251CD00(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEAE8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEAE8) { __imp__sub_829FEAE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEAE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32006
	ctx.r11.s64 = -2097545216;
	// addi r3,r11,18420
	ctx.r3.s64 = ctx.r11.s64 + 18420;
	// b 0x825b5ab8
	sub_825B5AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEAF8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEAF8) { __imp__sub_829FEAF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEAF8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32006
	ctx.r11.s64 = -2097545216;
	// addi r3,r11,18424
	ctx.r3.s64 = ctx.r11.s64 + 18424;
	// b 0x825b5ab8
	sub_825B5AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEB08);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEB08) { __imp__sub_829FEB08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEB08) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32006
	ctx.r11.s64 = -2097545216;
	// addi r3,r11,18428
	ctx.r3.s64 = ctx.r11.s64 + 18428;
	// b 0x825b5ab8
	sub_825B5AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEB18);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEB18) { __imp__sub_829FEB18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEB18) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32006
	ctx.r11.s64 = -2097545216;
	// addi r3,r11,18432
	ctx.r3.s64 = ctx.r11.s64 + 18432;
	// b 0x825b5ab8
	sub_825B5AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEB28);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEB28) { __imp__sub_829FEB28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEB28) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEB30);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEB30) { __imp__sub_829FEB30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEB30) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEB38);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEB38) { __imp__sub_829FEB38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEB38) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r11,r11,-4276
	ctx.r11.s64 = ctx.r11.s64 + -4276;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEB54);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEB54) { __imp__sub_829FEB54(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEB54) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEB58);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEB58) { __imp__sub_829FEB58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEB58) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEB60);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEB60) { __imp__sub_829FEB60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEB60) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEB68);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEB68) { __imp__sub_829FEB68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEB68) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,-3248
	ctx.r3.s64 = ctx.r11.s64 + -3248;
	// b 0x82893c88
	sub_82893C88(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEB78);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEB78) { __imp__sub_829FEB78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEB78) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEB80);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEB80) { __imp__sub_829FEB80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEB80) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEB88);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEB88) { __imp__sub_829FEB88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEB88) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEB90);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEB90) { __imp__sub_829FEB90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEB90) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEB98);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEB98) { __imp__sub_829FEB98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEB98) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEBA0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEBA0) { __imp__sub_829FEBA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEBA0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEBA8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEBA8) { __imp__sub_829FEBA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEBA8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEBB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEBB0) { __imp__sub_829FEBB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEBB0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEBB8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEBB8) { __imp__sub_829FEBB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEBB8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEBC0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEBC0) { __imp__sub_829FEBC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEBC0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEBC8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEBC8) { __imp__sub_829FEBC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEBC8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEBD0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEBD0) { __imp__sub_829FEBD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEBD0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEBD8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEBD8) { __imp__sub_829FEBD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEBD8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEBE0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEBE0) { __imp__sub_829FEBE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEBE0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEBE8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEBE8) { __imp__sub_829FEBE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEBE8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEBF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEBF0) { __imp__sub_829FEBF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEBF0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEBF8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEBF8) { __imp__sub_829FEBF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEBF8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEC00);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEC00) { __imp__sub_829FEC00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEC00) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEC08);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEC08) { __imp__sub_829FEC08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEC08) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEC10);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEC10) { __imp__sub_829FEC10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEC10) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEC18);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEC18) { __imp__sub_829FEC18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEC18) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEC20);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEC20) { __imp__sub_829FEC20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEC20) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEC28);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEC28) { __imp__sub_829FEC28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEC28) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEC30);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEC30) { __imp__sub_829FEC30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEC30) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEC38);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEC38) { __imp__sub_829FEC38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEC38) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEC40);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEC40) { __imp__sub_829FEC40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEC40) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEC48);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEC48) { __imp__sub_829FEC48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEC48) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEC50);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEC50) { __imp__sub_829FEC50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEC50) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEC58);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEC58) { __imp__sub_829FEC58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEC58) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEC60);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEC60) { __imp__sub_829FEC60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEC60) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEC68);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEC68) { __imp__sub_829FEC68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEC68) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEC70);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEC70) { __imp__sub_829FEC70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEC70) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEC78);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEC78) { __imp__sub_829FEC78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEC78) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEC80);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEC80) { __imp__sub_829FEC80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEC80) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEC88);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEC88) { __imp__sub_829FEC88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEC88) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEC90);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEC90) { __imp__sub_829FEC90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEC90) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEC98);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEC98) { __imp__sub_829FEC98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEC98) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FECA0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FECA0) { __imp__sub_829FECA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FECA0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FECA8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FECA8) { __imp__sub_829FECA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FECA8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FECB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FECB0) { __imp__sub_829FECB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FECB0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FECB8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FECB8) { __imp__sub_829FECB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FECB8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FECC0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FECC0) { __imp__sub_829FECC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FECC0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FECC8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FECC8) { __imp__sub_829FECC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FECC8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FECD0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FECD0) { __imp__sub_829FECD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FECD0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FECD8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FECD8) { __imp__sub_829FECD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FECD8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FECE0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FECE0) { __imp__sub_829FECE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FECE0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FECE8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FECE8) { __imp__sub_829FECE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FECE8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FECF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FECF0) { __imp__sub_829FECF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FECF0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FECF8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FECF8) { __imp__sub_829FECF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FECF8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FED00);  /* forward decl */
PPC_WEAK_FUNC(sub_829FED00) { __imp__sub_829FED00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FED00) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FED08);  /* forward decl */
PPC_WEAK_FUNC(sub_829FED08) { __imp__sub_829FED08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FED08) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FED10);  /* forward decl */
PPC_WEAK_FUNC(sub_829FED10) { __imp__sub_829FED10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FED10) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FED18);  /* forward decl */
PPC_WEAK_FUNC(sub_829FED18) { __imp__sub_829FED18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FED18) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FED20);  /* forward decl */
PPC_WEAK_FUNC(sub_829FED20) { __imp__sub_829FED20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FED20) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FED28);  /* forward decl */
PPC_WEAK_FUNC(sub_829FED28) { __imp__sub_829FED28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FED28) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FED30);  /* forward decl */
PPC_WEAK_FUNC(sub_829FED30) { __imp__sub_829FED30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FED30) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FED38);  /* forward decl */
PPC_WEAK_FUNC(sub_829FED38) { __imp__sub_829FED38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FED38) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FED40);  /* forward decl */
PPC_WEAK_FUNC(sub_829FED40) { __imp__sub_829FED40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FED40) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FED48);  /* forward decl */
PPC_WEAK_FUNC(sub_829FED48) { __imp__sub_829FED48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FED48) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FED50);  /* forward decl */
PPC_WEAK_FUNC(sub_829FED50) { __imp__sub_829FED50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FED50) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FED58);  /* forward decl */
PPC_WEAK_FUNC(sub_829FED58) { __imp__sub_829FED58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FED58) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FED60);  /* forward decl */
PPC_WEAK_FUNC(sub_829FED60) { __imp__sub_829FED60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FED60) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FED68);  /* forward decl */
PPC_WEAK_FUNC(sub_829FED68) { __imp__sub_829FED68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FED68) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FED70);  /* forward decl */
PPC_WEAK_FUNC(sub_829FED70) { __imp__sub_829FED70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FED70) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FED78);  /* forward decl */
PPC_WEAK_FUNC(sub_829FED78) { __imp__sub_829FED78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FED78) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FED80);  /* forward decl */
PPC_WEAK_FUNC(sub_829FED80) { __imp__sub_829FED80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FED80) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FED88);  /* forward decl */
PPC_WEAK_FUNC(sub_829FED88) { __imp__sub_829FED88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FED88) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FED90);  /* forward decl */
PPC_WEAK_FUNC(sub_829FED90) { __imp__sub_829FED90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FED90) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FED98);  /* forward decl */
PPC_WEAK_FUNC(sub_829FED98) { __imp__sub_829FED98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FED98) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEDA0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEDA0) { __imp__sub_829FEDA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEDA0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEDA8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEDA8) { __imp__sub_829FEDA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEDA8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEDB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEDB0) { __imp__sub_829FEDB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEDB0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEDB8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEDB8) { __imp__sub_829FEDB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEDB8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEDC0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEDC0) { __imp__sub_829FEDC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEDC0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEDC8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEDC8) { __imp__sub_829FEDC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEDC8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEDD0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEDD0) { __imp__sub_829FEDD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEDD0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEDD8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEDD8) { __imp__sub_829FEDD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEDD8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEDE0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEDE0) { __imp__sub_829FEDE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEDE0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r3,3824(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 3824);
	// bl 0x8218be78
	ctx.lr = 0x829FEDFC;
	sub_8218BE78(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,3824(r31)
	PPC_STORE_U32(ctx.r31.u32 + 3824, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEE18);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEE18) { __imp__sub_829FEE18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEE18) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32004
	ctx.r11.s64 = -2097414144;
	// addi r3,r11,32760
	ctx.r3.s64 = ctx.r11.s64 + 32760;
	// b 0x825b5b78
	sub_825B5B78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEE28);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEE28) { __imp__sub_829FEE28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEE28) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEE30);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEE30) { __imp__sub_829FEE30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEE30) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEE38);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEE38) { __imp__sub_829FEE38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEE38) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r11,r11,-32768
	ctx.r11.s64 = ctx.r11.s64 + -32768;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEE54);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEE54) { __imp__sub_829FEE54(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEE54) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEE58);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEE58) { __imp__sub_829FEE58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEE58) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,16672
	ctx.r3.s64 = ctx.r11.s64 + 16672;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEE68);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEE68) { __imp__sub_829FEE68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEE68) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,16704
	ctx.r3.s64 = ctx.r11.s64 + 16704;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEE78);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEE78) { __imp__sub_829FEE78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEE78) {
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
	// addi r31,r11,10048
	ctx.r31.s64 = ctx.r11.s64 + 10048;
loc_829FEE9C:
	// lis r11,-32206
	ctx.r11.s64 = -2110652416;
	// addi r31,r31,-4240
	ctx.r31.s64 = ctx.r31.s64 + -4240;
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r6,r11,-512
	ctx.r6.s64 = ctx.r11.s64 + -512;
	// li r4,2496
	ctx.r4.s64 = 2496;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82120268
	ctx.lr = 0x829FEEB8;
	sub_82120268(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829fee9c
	if (!ctx.cr6.lt) goto loc_829FEE9C;
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

PPC_FUNC_IMPL(__imp__sub_829FEEE0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEEE0) { __imp__sub_829FEEE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEEE0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEEE8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEEE8) { __imp__sub_829FEEE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEEE8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEEF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEEF0) { __imp__sub_829FEEF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEEF0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r11,r11,17652
	ctx.r11.s64 = ctx.r11.s64 + 17652;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEF0C);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEF0C) { __imp__sub_829FEF0C(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEF0C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEF10);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEF10) { __imp__sub_829FEF10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEF10) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,32688
	ctx.r3.s64 = ctx.r11.s64 + 32688;
	// b 0x8266ca80
	sub_8266CA80(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEF20);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEF20) { __imp__sub_829FEF20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEF20) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r31,r11,17696
	ctx.r31.s64 = ctx.r11.s64 + 17696;
	// addi r3,r31,11232
	ctx.r3.s64 = ctx.r31.s64 + 11232;
	// bl 0x82412a90
	ctx.lr = 0x829FEF40;
	sub_82412A90(ctx, base);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r11,r11,9888
	ctx.r11.s64 = ctx.r11.s64 + 9888;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82893c88
	ctx.lr = 0x829FEF54;
	sub_82893C88(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEF68);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEF68) { __imp__sub_829FEF68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEF68) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,32712
	ctx.r3.s64 = ctx.r11.s64 + 32712;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r11,r11,9888
	ctx.r11.s64 = ctx.r11.s64 + 9888;
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r11.u32);
	// b 0x82893c88
	sub_82893C88(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEF80);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEF80) { __imp__sub_829FEF80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEF80) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEF88);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEF88) { __imp__sub_829FEF88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEF88) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEF90);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEF90) { __imp__sub_829FEF90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEF90) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEF98);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEF98) { __imp__sub_829FEF98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEF98) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEFA0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEFA0) { __imp__sub_829FEFA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEFA0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEFA8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEFA8) { __imp__sub_829FEFA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEFA8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEFB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEFB0) { __imp__sub_829FEFB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEFB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32002
	ctx.r11.s64 = -2097283072;
	// addi r3,r11,-32560
	ctx.r3.s64 = ctx.r11.s64 + -32560;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r11,r11,9888
	ctx.r11.s64 = ctx.r11.s64 + 9888;
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r11.u32);
	// b 0x82893c88
	sub_82893C88(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEFC8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEFC8) { __imp__sub_829FEFC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEFC8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEFD0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEFD0) { __imp__sub_829FEFD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEFD0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEFD8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEFD8) { __imp__sub_829FEFD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEFD8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEFE0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEFE0) { __imp__sub_829FEFE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEFE0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEFE8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEFE8) { __imp__sub_829FEFE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEFE8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEFF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEFF0) { __imp__sub_829FEFF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEFF0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FEFF8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FEFF8) { __imp__sub_829FEFF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FEFF8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF000);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF000) { __imp__sub_829FF000(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF000) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF008);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF008) { __imp__sub_829FF008(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF008) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32002
	ctx.r11.s64 = -2097283072;
	// addi r3,r11,-22960
	ctx.r3.s64 = ctx.r11.s64 + -22960;
	// b 0x82608e80
	sub_82608E80(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF018);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF018) { __imp__sub_829FF018(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF018) {
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
	// lis r11,-31998
	ctx.r11.s64 = -2097020928;
	// lis r10,-32093
	ctx.r10.s64 = -2103246848;
	// addi r31,r11,19776
	ctx.r31.s64 = ctx.r11.s64 + 19776;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lfs f0,2592(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2592);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// stfs f0,-12332(r10)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + -12332, temp.u32);
	// stfs f0,-12324(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -12324, temp.u32);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,4740
	ctx.r11.s64 = ctx.r11.s64 + 4740;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x823cba78
	ctx.lr = 0x829FF05C;
	sub_823CBA78(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x823cb1e8
	ctx.lr = 0x829FF064;
	sub_823CB1E8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF078);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF078) { __imp__sub_829FF078(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF078) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// addi r3,r11,-30720
	ctx.r3.s64 = ctx.r11.s64 + -30720;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF088);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF088) { __imp__sub_829FF088(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF088) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// addi r11,r11,-27420
	ctx.r11.s64 = ctx.r11.s64 + -27420;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF0A4);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF0A4) { __imp__sub_829FF0A4(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF0A4) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF0A8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF0A8) { __imp__sub_829FF0A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF0A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32092
	ctx.r11.s64 = -2103181312;
	// addi r11,r11,-10708
	ctx.r11.s64 = ctx.r11.s64 + -10708;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF0C4);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF0C4) { __imp__sub_829FF0C4(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF0C4) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF0C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF0C8) { __imp__sub_829FF0C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF0C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32092
	ctx.r11.s64 = -2103181312;
	// addi r11,r11,-10700
	ctx.r11.s64 = ctx.r11.s64 + -10700;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF0E4);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF0E4) { __imp__sub_829FF0E4(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF0E4) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF0E8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF0E8) { __imp__sub_829FF0E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF0E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32092
	ctx.r11.s64 = -2103181312;
	// addi r11,r11,-10692
	ctx.r11.s64 = ctx.r11.s64 + -10692;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF104);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF104) { __imp__sub_829FF104(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF104) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF108);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF108) { __imp__sub_829FF108(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF108) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32092
	ctx.r11.s64 = -2103181312;
	// addi r11,r11,-9832
	ctx.r11.s64 = ctx.r11.s64 + -9832;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF124);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF124) { __imp__sub_829FF124(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF124) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF128);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF128) { __imp__sub_829FF128(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF128) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// lwz r3,-9848(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + -9848);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF13C);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF13C) { __imp__sub_829FF13C(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF13C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF140);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF140) { __imp__sub_829FF140(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF140) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF148);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF148) { __imp__sub_829FF148(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF148) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF150);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF150) { __imp__sub_829FF150(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF150) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF158);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF158) { __imp__sub_829FF158(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF158) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF160);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF160) { __imp__sub_829FF160(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF160) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31995
	ctx.r11.s64 = -2096824320;
	// addi r3,r11,16792
	ctx.r3.s64 = ctx.r11.s64 + 16792;
	// b 0x8297b9c8
	sub_8297B9C8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF170);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF170) { __imp__sub_829FF170(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF170) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31996
	ctx.r11.s64 = -2096889856;
	// addi r3,r11,24464
	ctx.r3.s64 = ctx.r11.s64 + 24464;
	// b 0x82979de0
	sub_82979DE0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF180);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF180) { __imp__sub_829FF180(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF180) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31996
	ctx.r11.s64 = -2096889856;
	// addi r3,r11,26904
	ctx.r3.s64 = ctx.r11.s64 + 26904;
	// b 0x827d9bb0
	sub_827D9BB0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF190);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF190) { __imp__sub_829FF190(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF190) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31996
	ctx.r11.s64 = -2096889856;
	// addi r3,r11,24372
	ctx.r3.s64 = ctx.r11.s64 + 24372;
	// b 0x829778f8
	sub_829778F8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF1A0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF1A0) { __imp__sub_829FF1A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF1A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31996
	ctx.r11.s64 = -2096889856;
	// addi r3,r11,24808
	ctx.r3.s64 = ctx.r11.s64 + 24808;
	// b 0x8297b048
	sub_8297B048(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF1B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF1B0) { __imp__sub_829FF1B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF1B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31996
	ctx.r11.s64 = -2096889856;
	// addi r3,r11,24832
	ctx.r3.s64 = ctx.r11.s64 + 24832;
	// b 0x829755b8
	sub_829755B8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF1C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF1C0) { __imp__sub_829FF1C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF1C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31995
	ctx.r11.s64 = -2096824320;
	// addi r3,r11,-16736
	ctx.r3.s64 = ctx.r11.s64 + -16736;
	// b 0x8297b4b0
	sub_8297B4B0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF1D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF1D0) { __imp__sub_829FF1D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF1D0) {
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
	// li r30,99
	ctx.r30.s64 = 99;
	// addi r11,r11,29200
	ctx.r11.s64 = ctx.r11.s64 + 29200;
	// addi r31,r11,19600
	ctx.r31.s64 = ctx.r11.s64 + 19600;
loc_829FF1F4:
	// addi r31,r31,-196
	ctx.r31.s64 = ctx.r31.s64 + -196;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x828e0ab8
	ctx.lr = 0x829FF200;
	sub_828E0AB8(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829ff1f4
	if (!ctx.cr6.lt) goto loc_829FF1F4;
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

PPC_FUNC_IMPL(__imp__sub_829FF228);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF228) { __imp__sub_829FF228(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF228) {
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
	// li r30,99
	ctx.r30.s64 = 99;
	// addi r11,r11,-14616
	ctx.r11.s64 = ctx.r11.s64 + -14616;
	// addi r31,r11,19600
	ctx.r31.s64 = ctx.r11.s64 + 19600;
loc_829FF24C:
	// addi r31,r31,-196
	ctx.r31.s64 = ctx.r31.s64 + -196;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x828e0ab8
	ctx.lr = 0x829FF258;
	sub_828E0AB8(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829ff24c
	if (!ctx.cr6.lt) goto loc_829FF24C;
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

PPC_FUNC_IMPL(__imp__sub_829FF280);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF280) { __imp__sub_829FF280(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF280) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF288);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF288) { __imp__sub_829FF288(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF288) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF290);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF290) { __imp__sub_829FF290(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF290) {
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
	// li r30,79
	ctx.r30.s64 = 79;
	// addi r11,r11,5272
	ctx.r11.s64 = ctx.r11.s64 + 5272;
	// addi r31,r11,11520
	ctx.r31.s64 = ctx.r11.s64 + 11520;
loc_829FF2B4:
	// addi r31,r31,-144
	ctx.r31.s64 = ctx.r31.s64 + -144;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x828e0ab8
	ctx.lr = 0x829FF2C0;
	sub_828E0AB8(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829ff2b4
	if (!ctx.cr6.lt) goto loc_829FF2B4;
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

PPC_FUNC_IMPL(__imp__sub_829FF2E8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF2E8) { __imp__sub_829FF2E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF2E8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF2F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF2F0) { __imp__sub_829FF2F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF2F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31996
	ctx.r11.s64 = -2096889856;
	// addi r3,r11,24820
	ctx.r3.s64 = ctx.r11.s64 + 24820;
	// b 0x826720a0
	sub_826720A0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF300);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF300) { __imp__sub_829FF300(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF300) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// addi r3,r11,20080
	ctx.r3.s64 = ctx.r11.s64 + 20080;
	// b 0x827d9bb0
	sub_827D9BB0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF310);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF310) { __imp__sub_829FF310(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF310) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// addi r3,r11,22384
	ctx.r3.s64 = ctx.r11.s64 + 22384;
	// b 0x827d9bb0
	sub_827D9BB0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF320);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF320) { __imp__sub_829FF320(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF320) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,-30956
	ctx.r3.s64 = ctx.r11.s64 + -30956;
	// b 0x82975d38
	sub_82975D38(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF330);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF330) { __imp__sub_829FF330(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF330) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,-32712
	ctx.r3.s64 = ctx.r11.s64 + -32712;
	// b 0x8297dae0
	sub_8297DAE0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF340);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF340) { __imp__sub_829FF340(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF340) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// addi r3,r11,25492
	ctx.r3.s64 = ctx.r11.s64 + 25492;
	// b 0x82687d48
	sub_82687D48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF350);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF350) { __imp__sub_829FF350(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF350) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// addi r3,r11,22588
	ctx.r3.s64 = ctx.r11.s64 + 22588;
	// b 0x826913f8
	sub_826913F8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF360);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF360) { __imp__sub_829FF360(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF360) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,-30872
	ctx.r3.s64 = ctx.r11.s64 + -30872;
	// b 0x8268cc18
	sub_8268CC18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF370);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF370) { __imp__sub_829FF370(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF370) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// addi r3,r11,21152
	ctx.r3.s64 = ctx.r11.s64 + 21152;
	// b 0x82972a50
	sub_82972A50(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF380);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF380) { __imp__sub_829FF380(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF380) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,-31408
	ctx.r3.s64 = ctx.r11.s64 + -31408;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF390);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF390) { __imp__sub_829FF390(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF390) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// addi r3,r11,12180
	ctx.r3.s64 = ctx.r11.s64 + 12180;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF3A0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF3A0) { __imp__sub_829FF3A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF3A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,-31360
	ctx.r3.s64 = ctx.r11.s64 + -31360;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF3B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF3B0) { __imp__sub_829FF3B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF3B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// addi r3,r11,12208
	ctx.r3.s64 = ctx.r11.s64 + 12208;
	// b 0x826aaa88
	sub_826AAA88(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF3C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF3C0) { __imp__sub_829FF3C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF3C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31986
	ctx.r11.s64 = -2096234496;
	// addi r3,r11,31816
	ctx.r3.s64 = ctx.r11.s64 + 31816;
	// b 0x8297d510
	sub_8297D510(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF3D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF3D0) { __imp__sub_829FF3D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF3D0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF3D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF3D8) { __imp__sub_829FF3D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF3D8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF3E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF3E0) { __imp__sub_829FF3E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF3E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,664
	ctx.r3.s64 = ctx.r11.s64 + 664;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF3F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF3F0) { __imp__sub_829FF3F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF3F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// addi r3,r11,24792
	ctx.r3.s64 = ctx.r11.s64 + 24792;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF400);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF400) { __imp__sub_829FF400(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF400) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,-31280
	ctx.r3.s64 = ctx.r11.s64 + -31280;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF410);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF410) { __imp__sub_829FF410(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF410) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// addi r3,r11,11912
	ctx.r3.s64 = ctx.r11.s64 + 11912;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF420);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF420) { __imp__sub_829FF420(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF420) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// addi r3,r11,24700
	ctx.r3.s64 = ctx.r11.s64 + 24700;
	// b 0x8297ba60
	sub_8297BA60(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF430);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF430) { __imp__sub_829FF430(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF430) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,-31464
	ctx.r3.s64 = ctx.r11.s64 + -31464;
	// b 0x8297ba60
	sub_8297BA60(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF440);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF440) { __imp__sub_829FF440(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF440) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// addi r3,r11,22532
	ctx.r3.s64 = ctx.r11.s64 + 22532;
	// b 0x8297ba60
	sub_8297BA60(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF450);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF450) { __imp__sub_829FF450(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF450) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF458);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF458) { __imp__sub_829FF458(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF458) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// addi r3,r11,25416
	ctx.r3.s64 = ctx.r11.s64 + 25416;
	// b 0x8297ba60
	sub_8297BA60(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF468);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF468) { __imp__sub_829FF468(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF468) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// addi r11,r11,28696
	ctx.r11.s64 = ctx.r11.s64 + 28696;
	// stw r11,932(r10)
	PPC_STORE_U32(ctx.r10.u32 + 932, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF480);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF480) { __imp__sub_829FF480(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF480) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,-32768
	ctx.r31.s64 = ctx.r10.s64 + -32768;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FF4A8;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FF4B0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FF4D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF4D0) { __imp__sub_829FF4D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF4D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,-31312
	ctx.r31.s64 = ctx.r10.s64 + -31312;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FF4F8;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FF500;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FF520);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF520) { __imp__sub_829FF520(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF520) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,-30288
	ctx.r3.s64 = ctx.r11.s64 + -30288;
	// b 0x826798c0
	sub_826798C0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF530);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF530) { __imp__sub_829FF530(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF530) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,956
	ctx.r3.s64 = ctx.r11.s64 + 956;
	// lhz r5,6(r3)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r3.u32 + 6);
	// cmplwi cr6,r5,0
	ctx.cr6.compare<uint32_t>(ctx.r5.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r4,0(r3)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// b 0x8267cf70
	sub_8267CF70(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF54C);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF54C) { __imp__sub_829FF54C(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF54C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF550);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF550) { __imp__sub_829FF550(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF550) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,1104
	ctx.r31.s64 = ctx.r10.s64 + 1104;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FF578;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FF580;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FF5A0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF5A0) { __imp__sub_829FF5A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF5A0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,1072
	ctx.r31.s64 = ctx.r10.s64 + 1072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FF5C8;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FF5D0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FF5F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF5F0) { __imp__sub_829FF5F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF5F0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,1040
	ctx.r31.s64 = ctx.r10.s64 + 1040;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FF618;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FF620;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FF640);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF640) { __imp__sub_829FF640(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF640) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,1320
	ctx.r31.s64 = ctx.r10.s64 + 1320;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FF668;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FF670;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FF690);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF690) { __imp__sub_829FF690(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF690) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,1352
	ctx.r31.s64 = ctx.r10.s64 + 1352;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FF6B8;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FF6C0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FF6E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF6E0) { __imp__sub_829FF6E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF6E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,1416
	ctx.r31.s64 = ctx.r10.s64 + 1416;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FF708;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FF710;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FF730);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF730) { __imp__sub_829FF730(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF730) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,1448
	ctx.r31.s64 = ctx.r10.s64 + 1448;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FF758;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FF760;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FF780);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF780) { __imp__sub_829FF780(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF780) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,1384
	ctx.r31.s64 = ctx.r10.s64 + 1384;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FF7A8;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FF7B0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FF7D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF7D0) { __imp__sub_829FF7D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF7D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,1480
	ctx.r31.s64 = ctx.r10.s64 + 1480;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FF7F8;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FF800;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FF820);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF820) { __imp__sub_829FF820(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF820) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,4352
	ctx.r3.s64 = ctx.r11.s64 + 4352;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FF830);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF830) { __imp__sub_829FF830(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF830) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,5304
	ctx.r31.s64 = ctx.r10.s64 + 5304;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FF858;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FF860;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FF880);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF880) { __imp__sub_829FF880(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF880) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,5240
	ctx.r31.s64 = ctx.r10.s64 + 5240;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FF8A8;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FF8B0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FF8D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF8D0) { __imp__sub_829FF8D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF8D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,5144
	ctx.r31.s64 = ctx.r10.s64 + 5144;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FF8F8;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FF900;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FF920);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF920) { __imp__sub_829FF920(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF920) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,5272
	ctx.r31.s64 = ctx.r10.s64 + 5272;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FF948;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FF950;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FF970);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF970) { __imp__sub_829FF970(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF970) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,5176
	ctx.r31.s64 = ctx.r10.s64 + 5176;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FF998;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FF9A0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FF9C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FF9C0) { __imp__sub_829FF9C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FF9C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,5208
	ctx.r31.s64 = ctx.r10.s64 + 5208;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FF9E8;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FF9F0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FFA10);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFA10) { __imp__sub_829FFA10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFA10) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,5820
	ctx.r31.s64 = ctx.r10.s64 + 5820;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FFA38;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FFA40;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FFA60);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFA60) { __imp__sub_829FFA60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFA60) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,5852
	ctx.r31.s64 = ctx.r10.s64 + 5852;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FFA88;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FFA90;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FFAB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFAB0) { __imp__sub_829FFAB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFAB0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,5724
	ctx.r31.s64 = ctx.r10.s64 + 5724;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FFAD8;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FFAE0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FFB00);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFB00) { __imp__sub_829FFB00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFB00) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,5692
	ctx.r31.s64 = ctx.r10.s64 + 5692;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FFB28;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FFB30;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FFB50);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFB50) { __imp__sub_829FFB50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFB50) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,5788
	ctx.r31.s64 = ctx.r10.s64 + 5788;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FFB78;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FFB80;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FFBA0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFBA0) { __imp__sub_829FFBA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFBA0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,5756
	ctx.r31.s64 = ctx.r10.s64 + 5756;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FFBC8;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FFBD0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FFBF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFBF0) { __imp__sub_829FFBF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFBF0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// addi r11,r11,21816
	ctx.r11.s64 = ctx.r11.s64 + 21816;
	// stw r11,5892(r10)
	PPC_STORE_U32(ctx.r10.u32 + 5892, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FFC08);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFC08) { __imp__sub_829FFC08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFC08) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// addi r11,r11,21816
	ctx.r11.s64 = ctx.r11.s64 + 21816;
	// stw r11,5904(r10)
	PPC_STORE_U32(ctx.r10.u32 + 5904, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FFC20);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFC20) { __imp__sub_829FFC20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFC20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// addi r11,r11,21816
	ctx.r11.s64 = ctx.r11.s64 + 21816;
	// stw r11,5916(r10)
	PPC_STORE_U32(ctx.r10.u32 + 5916, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FFC38);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFC38) { __imp__sub_829FFC38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFC38) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// addi r11,r11,21816
	ctx.r11.s64 = ctx.r11.s64 + 21816;
	// stw r11,5928(r10)
	PPC_STORE_U32(ctx.r10.u32 + 5928, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FFC50);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFC50) { __imp__sub_829FFC50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFC50) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// addi r11,r11,21816
	ctx.r11.s64 = ctx.r11.s64 + 21816;
	// stw r11,5940(r10)
	PPC_STORE_U32(ctx.r10.u32 + 5940, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FFC68);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFC68) { __imp__sub_829FFC68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFC68) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// addi r11,r11,21816
	ctx.r11.s64 = ctx.r11.s64 + 21816;
	// stw r11,5952(r10)
	PPC_STORE_U32(ctx.r10.u32 + 5952, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FFC80);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFC80) { __imp__sub_829FFC80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFC80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// addi r11,r11,21816
	ctx.r11.s64 = ctx.r11.s64 + 21816;
	// stw r11,5964(r10)
	PPC_STORE_U32(ctx.r10.u32 + 5964, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FFC98);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFC98) { __imp__sub_829FFC98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFC98) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// addi r11,r11,21816
	ctx.r11.s64 = ctx.r11.s64 + 21816;
	// stw r11,5976(r10)
	PPC_STORE_U32(ctx.r10.u32 + 5976, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FFCB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFCB0) { __imp__sub_829FFCB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFCB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// addi r11,r11,21816
	ctx.r11.s64 = ctx.r11.s64 + 21816;
	// stw r11,5988(r10)
	PPC_STORE_U32(ctx.r10.u32 + 5988, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FFCC8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFCC8) { __imp__sub_829FFCC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFCC8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,6164
	ctx.r31.s64 = ctx.r10.s64 + 6164;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FFCF0;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FFCF8;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FFD18);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFD18) { __imp__sub_829FFD18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFD18) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,6132
	ctx.r31.s64 = ctx.r10.s64 + 6132;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FFD40;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FFD48;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FFD68);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFD68) { __imp__sub_829FFD68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFD68) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// lis r10,-32090
	ctx.r10.s64 = -2103050240;
	// addi r11,r11,-28952
	ctx.r11.s64 = ctx.r11.s64 + -28952;
	// addi r10,r10,-26576
	ctx.r10.s64 = ctx.r10.s64 + -26576;
	// stw r11,16(r10)
	PPC_STORE_U32(ctx.r10.u32 + 16, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FFD80);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFD80) { __imp__sub_829FFD80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFD80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,6196
	ctx.r3.s64 = ctx.r11.s64 + 6196;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FFD90);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFD90) { __imp__sub_829FFD90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFD90) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r11,r11,6224
	ctx.r11.s64 = ctx.r11.s64 + 6224;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FFDAC);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFDAC) { __imp__sub_829FFDAC(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFDAC) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FFDB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFDB0) { __imp__sub_829FFDB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFDB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,6232
	ctx.r3.s64 = ctx.r11.s64 + 6232;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FFDC0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFDC0) { __imp__sub_829FFDC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFDC0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,6256
	ctx.r3.s64 = ctx.r11.s64 + 6256;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FFDD0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFDD0) { __imp__sub_829FFDD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFDD0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,6280
	ctx.r3.s64 = ctx.r11.s64 + 6280;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FFDE0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFDE0) { __imp__sub_829FFDE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFDE0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,6304
	ctx.r3.s64 = ctx.r11.s64 + 6304;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FFDF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFDF0) { __imp__sub_829FFDF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFDF0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,6328
	ctx.r3.s64 = ctx.r11.s64 + 6328;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FFE00);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFE00) { __imp__sub_829FFE00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFE00) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,6352
	ctx.r3.s64 = ctx.r11.s64 + 6352;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FFE10);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFE10) { __imp__sub_829FFE10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFE10) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,6376
	ctx.r3.s64 = ctx.r11.s64 + 6376;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FFE20);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFE20) { __imp__sub_829FFE20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFE20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,6400
	ctx.r3.s64 = ctx.r11.s64 + 6400;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FFE30);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFE30) { __imp__sub_829FFE30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFE30) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,6424
	ctx.r3.s64 = ctx.r11.s64 + 6424;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FFE40);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFE40) { __imp__sub_829FFE40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFE40) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,6448
	ctx.r3.s64 = ctx.r11.s64 + 6448;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FFE50);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFE50) { __imp__sub_829FFE50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFE50) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,6472
	ctx.r3.s64 = ctx.r11.s64 + 6472;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FFE60);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFE60) { __imp__sub_829FFE60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFE60) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,9332
	ctx.r3.s64 = ctx.r11.s64 + 9332;
	// b 0x8296ca60
	sub_8296CA60(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FFE70);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFE70) { __imp__sub_829FFE70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFE70) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r31,r10,9776
	ctx.r31.s64 = ctx.r10.s64 + 9776;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x829FFE98;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FFEA0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FFEC0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFEC0) { __imp__sub_829FFEC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFEC0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r31,r10,9648
	ctx.r31.s64 = ctx.r10.s64 + 9648;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x829FFEE8;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FFEF0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FFF10);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFF10) { __imp__sub_829FFF10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFF10) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r31,r10,9584
	ctx.r31.s64 = ctx.r10.s64 + 9584;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x829FFF38;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FFF40;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FFF60);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFF60) { __imp__sub_829FFF60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFF60) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r31,r10,9424
	ctx.r31.s64 = ctx.r10.s64 + 9424;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x829FFF88;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FFF90;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_829FFFB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FFFB0) { __imp__sub_829FFFB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FFFB0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r31,r10,9712
	ctx.r31.s64 = ctx.r10.s64 + 9712;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x829FFFD8;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x829FFFE0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A00000);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00000) { __imp__sub_82A00000(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00000) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r31,r10,9392
	ctx.r31.s64 = ctx.r10.s64 + 9392;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x82A00028;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A00030;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A00050);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00050) { __imp__sub_82A00050(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00050) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r31,r10,9488
	ctx.r31.s64 = ctx.r10.s64 + 9488;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x82A00078;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A00080;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A000A0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A000A0) { __imp__sub_82A000A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A000A0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r31,r10,9360
	ctx.r31.s64 = ctx.r10.s64 + 9360;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x82A000C8;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A000D0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A000F0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A000F0) { __imp__sub_82A000F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A000F0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r31,r10,9616
	ctx.r31.s64 = ctx.r10.s64 + 9616;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x82A00118;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A00120;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A00140);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00140) { __imp__sub_82A00140(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00140) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r31,r10,9552
	ctx.r31.s64 = ctx.r10.s64 + 9552;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x82A00168;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A00170;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A00190);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00190) { __imp__sub_82A00190(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00190) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r31,r10,9520
	ctx.r31.s64 = ctx.r10.s64 + 9520;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x82A001B8;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A001C0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A001E0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A001E0) { __imp__sub_82A001E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A001E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r31,r10,9680
	ctx.r31.s64 = ctx.r10.s64 + 9680;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x82A00208;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A00210;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A00230);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00230) { __imp__sub_82A00230(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00230) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r31,r10,9456
	ctx.r31.s64 = ctx.r10.s64 + 9456;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x82A00258;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A00260;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A00280);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00280) { __imp__sub_82A00280(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00280) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r31,r10,9744
	ctx.r31.s64 = ctx.r10.s64 + 9744;
	// addi r11,r11,-16708
	ctx.r11.s64 = ctx.r11.s64 + -16708;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82761308
	ctx.lr = 0x82A002A8;
	sub_82761308(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A002B0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A002D0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A002D0) { __imp__sub_82A002D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A002D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lhz r11,18(r31)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r31.u32 + 18);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a002fc
	if (ctx.cr6.eq) goto loc_82A002FC;
	// lwz r3,12(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 12);
	// bl 0x8218be78
	ctx.lr = 0x82A002FC;
	sub_8218BE78(ctx, base);
loc_82A002FC:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8276f8c0
	ctx.lr = 0x82A00304;
	sub_8276F8C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00318);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00318) { __imp__sub_82A00318(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00318) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lhz r11,18(r31)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r31.u32 + 18);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a00344
	if (ctx.cr6.eq) goto loc_82A00344;
	// lwz r3,12(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 12);
	// bl 0x8218be78
	ctx.lr = 0x82A00344;
	sub_8218BE78(ctx, base);
loc_82A00344:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8276f8c0
	ctx.lr = 0x82A0034C;
	sub_8276F8C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00360);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00360) { __imp__sub_82A00360(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00360) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lhz r11,18(r31)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r31.u32 + 18);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a0038c
	if (ctx.cr6.eq) goto loc_82A0038C;
	// lwz r3,12(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 12);
	// bl 0x8218be78
	ctx.lr = 0x82A0038C;
	sub_8218BE78(ctx, base);
loc_82A0038C:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8276f8c0
	ctx.lr = 0x82A00394;
	sub_8276F8C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A003A8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A003A8) { __imp__sub_82A003A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A003A8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lhz r11,18(r31)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r31.u32 + 18);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a003d4
	if (ctx.cr6.eq) goto loc_82A003D4;
	// lwz r3,12(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 12);
	// bl 0x8218be78
	ctx.lr = 0x82A003D4;
	sub_8218BE78(ctx, base);
loc_82A003D4:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8276f8c0
	ctx.lr = 0x82A003DC;
	sub_8276F8C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A003F0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A003F0) { __imp__sub_82A003F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A003F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,8864
	ctx.r11.s64 = ctx.r11.s64 + 8864;
	// lhz r10,10(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 10);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,4(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A0040C);  /* forward decl */
PPC_WEAK_FUNC(sub_82A0040C) { __imp__sub_82A0040C(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A0040C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00410);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00410) { __imp__sub_82A00410(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00410) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r11,r11,8876
	ctx.r11.s64 = ctx.r11.s64 + 8876;
	// lhz r10,10(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 10);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,4(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A0042C);  /* forward decl */
PPC_WEAK_FUNC(sub_82A0042C) { __imp__sub_82A0042C(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A0042C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00430);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00430) { __imp__sub_82A00430(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00430) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r3,r11,-12140
	ctx.r3.s64 = ctx.r11.s64 + -12140;
	// b 0x827d8560
	sub_827D8560(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00440);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00440) { __imp__sub_82A00440(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00440) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,12120
	ctx.r31.s64 = ctx.r10.s64 + 12120;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A00468;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A00470;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A00490);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00490) { __imp__sub_82A00490(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00490) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,12248
	ctx.r31.s64 = ctx.r10.s64 + 12248;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A004B8;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A004C0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A004E0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A004E0) { __imp__sub_82A004E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A004E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,11832
	ctx.r31.s64 = ctx.r10.s64 + 11832;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A00508;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A00510;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A00530);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00530) { __imp__sub_82A00530(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00530) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,12216
	ctx.r31.s64 = ctx.r10.s64 + 12216;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A00558;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A00560;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A00580);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00580) { __imp__sub_82A00580(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00580) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,12152
	ctx.r31.s64 = ctx.r10.s64 + 12152;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A005A8;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A005B0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A005D0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A005D0) { __imp__sub_82A005D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A005D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,11896
	ctx.r31.s64 = ctx.r10.s64 + 11896;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A005F8;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A00600;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A00620);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00620) { __imp__sub_82A00620(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00620) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,12056
	ctx.r31.s64 = ctx.r10.s64 + 12056;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A00648;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A00650;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A00670);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00670) { __imp__sub_82A00670(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00670) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,11768
	ctx.r31.s64 = ctx.r10.s64 + 11768;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A00698;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A006A0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A006C0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A006C0) { __imp__sub_82A006C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A006C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,11928
	ctx.r31.s64 = ctx.r10.s64 + 11928;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A006E8;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A006F0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A00710);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00710) { __imp__sub_82A00710(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00710) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,12280
	ctx.r31.s64 = ctx.r10.s64 + 12280;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A00738;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A00740;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A00760);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00760) { __imp__sub_82A00760(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00760) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,12088
	ctx.r31.s64 = ctx.r10.s64 + 12088;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A00788;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A00790;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A007B0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A007B0) { __imp__sub_82A007B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A007B0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,11864
	ctx.r31.s64 = ctx.r10.s64 + 11864;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A007D8;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A007E0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A00800);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00800) { __imp__sub_82A00800(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00800) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,11992
	ctx.r31.s64 = ctx.r10.s64 + 11992;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A00828;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A00830;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A00850);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00850) { __imp__sub_82A00850(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00850) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,11800
	ctx.r31.s64 = ctx.r10.s64 + 11800;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A00878;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A00880;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A008A0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A008A0) { __imp__sub_82A008A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A008A0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,12184
	ctx.r31.s64 = ctx.r10.s64 + 12184;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A008C8;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A008D0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A008F0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A008F0) { __imp__sub_82A008F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A008F0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,12024
	ctx.r31.s64 = ctx.r10.s64 + 12024;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A00918;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A00920;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A00940);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00940) { __imp__sub_82A00940(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00940) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31985
	ctx.r10.s64 = -2096168960;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,11960
	ctx.r31.s64 = ctx.r10.s64 + 11960;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A00968;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A00970;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A00990);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00990) { __imp__sub_82A00990(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00990) {
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
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r30,63
	ctx.r30.s64 = 63;
	// addi r11,r11,12320
	ctx.r11.s64 = ctx.r11.s64 + 12320;
	// addi r31,r11,7204
	ctx.r31.s64 = ctx.r11.s64 + 7204;
loc_82A009B4:
	// addi r31,r31,-112
	ctx.r31.s64 = ctx.r31.s64 + -112;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82989c60
	ctx.lr = 0x82A009C0;
	sub_82989C60(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a009b4
	if (!ctx.cr6.lt) goto loc_82A009B4;
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

PPC_FUNC_IMPL(__imp__sub_82A009E8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A009E8) { __imp__sub_82A009E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A009E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r3,r11,-2716
	ctx.r3.s64 = ctx.r11.s64 + -2716;
	// b 0x827d8560
	sub_827D8560(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A009F8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A009F8) { __imp__sub_82A009F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A009F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,20316
	ctx.r3.s64 = ctx.r11.s64 + 20316;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00A08);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00A08) { __imp__sub_82A00A08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00A08) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,20332
	ctx.r3.s64 = ctx.r11.s64 + 20332;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00A18);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00A18) { __imp__sub_82A00A18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00A18) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r11,r11,20352
	ctx.r11.s64 = ctx.r11.s64 + 20352;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00A34);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00A34) { __imp__sub_82A00A34(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00A34) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00A38);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00A38) { __imp__sub_82A00A38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00A38) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32088
	ctx.r10.s64 = -2102919168;
	// addi r11,r11,14704
	ctx.r11.s64 = ctx.r11.s64 + 14704;
	// stw r11,-2384(r10)
	PPC_STORE_U32(ctx.r10.u32 + -2384, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00A50);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00A50) { __imp__sub_82A00A50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00A50) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r11,r11,20364
	ctx.r11.s64 = ctx.r11.s64 + 20364;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00A6C);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00A6C) { __imp__sub_82A00A6C(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00A6C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00A70);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00A70) { __imp__sub_82A00A70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00A70) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,20372
	ctx.r3.s64 = ctx.r11.s64 + 20372;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00A80);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00A80) { __imp__sub_82A00A80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00A80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,20388
	ctx.r3.s64 = ctx.r11.s64 + 20388;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00A90);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00A90) { __imp__sub_82A00A90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00A90) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r31,-31985
	ctx.r31.s64 = -2096168960;
	// lwz r3,20448(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 20448);
	// bl 0x8218be78
	ctx.lr = 0x82A00AAC;
	sub_8218BE78(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,20448(r31)
	PPC_STORE_U32(ctx.r31.u32 + 20448, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00AC8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00AC8) { __imp__sub_82A00AC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00AC8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r11,r11,20424
	ctx.r11.s64 = ctx.r11.s64 + 20424;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00AE4);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00AE4) { __imp__sub_82A00AE4(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00AE4) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00AE8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00AE8) { __imp__sub_82A00AE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00AE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r11,r11,20440
	ctx.r11.s64 = ctx.r11.s64 + 20440;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00B04);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00B04) { __imp__sub_82A00B04(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00B04) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00B08);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00B08) { __imp__sub_82A00B08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00B08) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r11,r11,20432
	ctx.r11.s64 = ctx.r11.s64 + 20432;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00B24);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00B24) { __imp__sub_82A00B24(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00B24) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00B28);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00B28) { __imp__sub_82A00B28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00B28) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,20484
	ctx.r3.s64 = ctx.r11.s64 + 20484;
	// b 0x827db0c0
	sub_827DB0C0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00B38);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00B38) { __imp__sub_82A00B38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00B38) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r31,-32088
	ctx.r31.s64 = -2102919168;
	// lwz r3,-1096(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + -1096);
	// bl 0x8218be78
	ctx.lr = 0x82A00B54;
	sub_8218BE78(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,-1096(r31)
	PPC_STORE_U32(ctx.r31.u32 + -1096, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00B70);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00B70) { __imp__sub_82A00B70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00B70) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,20860
	ctx.r3.s64 = ctx.r11.s64 + 20860;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00B80);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00B80) { __imp__sub_82A00B80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00B80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,20896
	ctx.r3.s64 = ctx.r11.s64 + 20896;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00B90);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00B90) { __imp__sub_82A00B90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00B90) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,21664
	ctx.r3.s64 = ctx.r11.s64 + 21664;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00BA0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00BA0) { __imp__sub_82A00BA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00BA0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,21716
	ctx.r3.s64 = ctx.r11.s64 + 21716;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00BB0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00BB0) { __imp__sub_82A00BB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00BB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,21760
	ctx.r3.s64 = ctx.r11.s64 + 21760;
	// b 0x827de168
	sub_827DE168(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00BC0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00BC0) { __imp__sub_82A00BC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00BC0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,22648
	ctx.r3.s64 = ctx.r11.s64 + 22648;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00BD0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00BD0) { __imp__sub_82A00BD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00BD0) {
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
	// lis r11,-31984
	ctx.r11.s64 = -2096103424;
	// li r30,1
	ctx.r30.s64 = 1;
	// addi r11,r11,7072
	ctx.r11.s64 = ctx.r11.s64 + 7072;
	// addis r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 65536;
	// addi r31,r11,9248
	ctx.r31.s64 = ctx.r11.s64 + 9248;
loc_82A00BF8:
	// addi r31,r31,-24944
	ctx.r31.s64 = ctx.r31.s64 + -24944;
	// lwz r3,44(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 44);
	// bl 0x827dadb0
	ctx.lr = 0x82A00C04;
	sub_827DADB0(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x828e0ab8
	ctx.lr = 0x82A00C0C;
	sub_828E0AB8(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a00bf8
	if (!ctx.cr6.lt) goto loc_82A00BF8;
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

PPC_FUNC_IMPL(__imp__sub_82A00C30);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00C30) { __imp__sub_82A00C30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00C30) {
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
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r30,1
	ctx.r30.s64 = 1;
	// addi r11,r11,22720
	ctx.r11.s64 = ctx.r11.s64 + 22720;
	// addis r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 65536;
	// addi r31,r11,9248
	ctx.r31.s64 = ctx.r11.s64 + 9248;
loc_82A00C58:
	// addi r31,r31,-24944
	ctx.r31.s64 = ctx.r31.s64 + -24944;
	// lwz r3,44(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 44);
	// bl 0x827dadb0
	ctx.lr = 0x82A00C64;
	sub_827DADB0(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x828e0ab8
	ctx.lr = 0x82A00C6C;
	sub_828E0AB8(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a00c58
	if (!ctx.cr6.lt) goto loc_82A00C58;
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

PPC_FUNC_IMPL(__imp__sub_82A00C90);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00C90) { __imp__sub_82A00C90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00C90) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00C98);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00C98) { __imp__sub_82A00C98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00C98) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lis r10,-32088
	ctx.r10.s64 = -2102919168;
	// addi r11,r11,4388
	ctx.r11.s64 = ctx.r11.s64 + 4388;
	// stw r11,2596(r10)
	PPC_STORE_U32(ctx.r10.u32 + 2596, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00CB0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00CB0) { __imp__sub_82A00CB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00CB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lis r10,-32088
	ctx.r10.s64 = -2102919168;
	// addi r11,r11,4388
	ctx.r11.s64 = ctx.r11.s64 + 4388;
	// stw r11,2600(r10)
	PPC_STORE_U32(ctx.r10.u32 + 2600, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00CC8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00CC8) { __imp__sub_82A00CC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00CC8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r3,r11,-7852
	ctx.r3.s64 = ctx.r11.s64 + -7852;
	// lhz r5,6(r3)
	ctx.r5.u64 = PPC_LOAD_U16(ctx.r3.u32 + 6);
	// cmplwi cr6,r5,0
	ctx.cr6.compare<uint32_t>(ctx.r5.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r4,0(r3)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// b 0x827e23f0
	sub_827E23F0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00CE4);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00CE4) { __imp__sub_82A00CE4(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00CE4) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00CE8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00CE8) { __imp__sub_82A00CE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00CE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,2928
	ctx.r11.s64 = ctx.r11.s64 + 2928;
	// lhz r10,10(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 10);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,4(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00D04);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00D04) { __imp__sub_82A00D04(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00D04) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00D08);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00D08) { __imp__sub_82A00D08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00D08) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r3,r11,-7804
	ctx.r3.s64 = ctx.r11.s64 + -7804;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00D18);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00D18) { __imp__sub_82A00D18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00D18) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00D20);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00D20) { __imp__sub_82A00D20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00D20) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00D28);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00D28) { __imp__sub_82A00D28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00D28) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00D30);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00D30) { __imp__sub_82A00D30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00D30) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32088
	ctx.r10.s64 = -2102919168;
	// addi r11,r11,26452
	ctx.r11.s64 = ctx.r11.s64 + 26452;
	// stw r11,3768(r10)
	PPC_STORE_U32(ctx.r10.u32 + 3768, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00D48);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00D48) { __imp__sub_82A00D48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00D48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// lis r10,-32249
	ctx.r10.s64 = -2113470464;
	// addi r11,r11,4312
	ctx.r11.s64 = ctx.r11.s64 + 4312;
	// addi r10,r10,31684
	ctx.r10.s64 = ctx.r10.s64 + 31684;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// lis r10,-31982
	ctx.r10.s64 = -2095972352;
	// lwz r9,-31264(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -31264);
	// cmplw cr6,r9,r11
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, ctx.r11.u32, ctx.xer);
	// bnelr cr6
	if (!ctx.cr6.eq) return;
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,-31264(r10)
	PPC_STORE_U32(ctx.r10.u32 + -31264, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00D78);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00D78) { __imp__sub_82A00D78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00D78) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lis r10,-32088
	ctx.r10.s64 = -2102919168;
	// addi r11,r11,4388
	ctx.r11.s64 = ctx.r11.s64 + 4388;
	// stw r11,7576(r10)
	PPC_STORE_U32(ctx.r10.u32 + 7576, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00D90);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00D90) { __imp__sub_82A00D90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00D90) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// addi r3,r11,18272
	ctx.r3.s64 = ctx.r11.s64 + 18272;
	// b 0x828fdbe0
	sub_828FDBE0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00DA0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00DA0) { __imp__sub_82A00DA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00DA0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-31982
	ctx.r10.s64 = -2095972352;
	// addi r11,r11,-15436
	ctx.r11.s64 = ctx.r11.s64 + -15436;
	// stw r11,18960(r10)
	PPC_STORE_U32(ctx.r10.u32 + 18960, ctx.r11.u32);
	// bl 0x8280da50
	ctx.lr = 0x82A00DC0;
	sub_8280DA50(ctx, base);
	// bl 0x8280d8f0
	ctx.lr = 0x82A00DC4;
	sub_8280D8F0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00DD8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00DD8) { __imp__sub_82A00DD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00DD8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// addi r3,r11,20008
	ctx.r3.s64 = ctx.r11.s64 + 20008;
	// b 0x8285bce8
	sub_8285BCE8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00DE8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00DE8) { __imp__sub_82A00DE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00DE8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00DF0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00DF0) { __imp__sub_82A00DF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00DF0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// addi r31,r11,22032
	ctx.r31.s64 = ctx.r11.s64 + 22032;
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x8285bc60
	ctx.lr = 0x82A00E10;
	sub_8285BC60(ctx, base);
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x8285bce8
	ctx.lr = 0x82A00E18;
	sub_8285BCE8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00E30);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00E30) { __imp__sub_82A00E30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00E30) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00E38);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00E38) { __imp__sub_82A00E38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00E38) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// addi r11,r11,22152
	ctx.r11.s64 = ctx.r11.s64 + 22152;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00E54);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00E54) { __imp__sub_82A00E54(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00E54) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00E58);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00E58) { __imp__sub_82A00E58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00E58) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// addi r11,r11,22160
	ctx.r11.s64 = ctx.r11.s64 + 22160;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00E74);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00E74) { __imp__sub_82A00E74(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00E74) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00E78);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00E78) { __imp__sub_82A00E78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00E78) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// addi r3,r11,22168
	ctx.r3.s64 = ctx.r11.s64 + 22168;
	// b 0x82854ba8
	sub_82854BA8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00E88);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00E88) { __imp__sub_82A00E88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00E88) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// addi r3,r11,22324
	ctx.r3.s64 = ctx.r11.s64 + 22324;
	// b 0x82854c30
	sub_82854C30(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00E98);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00E98) { __imp__sub_82A00E98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00E98) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00EA0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00EA0) { __imp__sub_82A00EA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00EA0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00EA8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00EA8) { __imp__sub_82A00EA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00EA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// addi r3,r11,26168
	ctx.r3.s64 = ctx.r11.s64 + 26168;
	// b 0x8285af80
	sub_8285AF80(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00EB8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00EB8) { __imp__sub_82A00EB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00EB8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// addi r3,r11,30824
	ctx.r3.s64 = ctx.r11.s64 + 30824;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00EC8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00EC8) { __imp__sub_82A00EC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00EC8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// addi r3,r11,30784
	ctx.r3.s64 = ctx.r11.s64 + 30784;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00ED8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00ED8) { __imp__sub_82A00ED8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00ED8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// addi r11,r11,30816
	ctx.r11.s64 = ctx.r11.s64 + 30816;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00EF4);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00EF4) { __imp__sub_82A00EF4(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00EF4) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00EF8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00EF8) { __imp__sub_82A00EF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00EF8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// addi r11,r11,30856
	ctx.r11.s64 = ctx.r11.s64 + 30856;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00F14);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00F14) { __imp__sub_82A00F14(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00F14) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00F18);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00F18) { __imp__sub_82A00F18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00F18) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00F20);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00F20) { __imp__sub_82A00F20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00F20) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00F28);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00F28) { __imp__sub_82A00F28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00F28) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// addi r3,r11,31268
	ctx.r3.s64 = ctx.r11.s64 + 31268;
	// b 0x82854ba8
	sub_82854BA8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00F38);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00F38) { __imp__sub_82A00F38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00F38) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r31,r11,-29988
	ctx.r31.s64 = ctx.r11.s64 + -29988;
	// lwz r11,16(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 16);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a00f7c
	if (ctx.cr6.eq) goto loc_82A00F7C;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,4(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4);
	// bl 0x82878b68
	ctx.lr = 0x82A00F68;
	sub_82878B68(ctx, base);
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
loc_82A00F7C:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00F90);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00F90) { __imp__sub_82A00F90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00F90) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// addi r3,r11,6736
	ctx.r3.s64 = ctx.r11.s64 + 6736;
	// b 0x8287a2f0
	sub_8287A2F0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00FA0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00FA0) { __imp__sub_82A00FA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00FA0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// addi r3,r11,6904
	ctx.r3.s64 = ctx.r11.s64 + 6904;
	// b 0x8287cd50
	sub_8287CD50(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00FB0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00FB0) { __imp__sub_82A00FB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00FB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r3,r11,-28832
	ctx.r3.s64 = ctx.r11.s64 + -28832;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00FC0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00FC0) { __imp__sub_82A00FC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00FC0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// addi r11,r11,10816
	ctx.r11.s64 = ctx.r11.s64 + 10816;
	// addi r3,r11,44
	ctx.r3.s64 = ctx.r11.s64 + 44;
	// b 0x828aea30
	sub_828AEA30(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A00FD0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A00FD0) { __imp__sub_82A00FD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A00FD0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r31,r11,-28768
	ctx.r31.s64 = ctx.r11.s64 + -28768;
	// addi r3,r31,1328
	ctx.r3.s64 = ctx.r31.s64 + 1328;
	// bl 0x828e0ab8
	ctx.lr = 0x82A00FF0;
	sub_828E0AB8(ctx, base);
	// addi r3,r31,1284
	ctx.r3.s64 = ctx.r31.s64 + 1284;
	// bl 0x828aea30
	ctx.lr = 0x82A00FF8;
	sub_828AEA30(ctx, base);
	// addi r3,r31,700
	ctx.r3.s64 = ctx.r31.s64 + 700;
	// bl 0x8289daa0
	ctx.lr = 0x82A01000;
	sub_8289DAA0(ctx, base);
	// addi r3,r31,380
	ctx.r3.s64 = ctx.r31.s64 + 380;
	// bl 0x828b05c8
	ctx.lr = 0x82A01008;
	sub_828B05C8(ctx, base);
	// addi r3,r31,172
	ctx.r3.s64 = ctx.r31.s64 + 172;
	// bl 0x828a3468
	ctx.lr = 0x82A01010;
	sub_828A3468(ctx, base);
	// addi r3,r31,100
	ctx.r3.s64 = ctx.r31.s64 + 100;
	// bl 0x828a6d48
	ctx.lr = 0x82A01018;
	sub_828A6D48(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x828a6d48
	ctx.lr = 0x82A01020;
	sub_828A6D48(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A01038);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01038) { __imp__sub_82A01038(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01038) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r3,r11,-20780
	ctx.r3.s64 = ctx.r11.s64 + -20780;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A01048);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01048) { __imp__sub_82A01048(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01048) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r3,r11,-20812
	ctx.r3.s64 = ctx.r11.s64 + -20812;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A01058);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01058) { __imp__sub_82A01058(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01058) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r3,r11,-20728
	ctx.r3.s64 = ctx.r11.s64 + -20728;
	// b 0x822de5f0
	sub_822DE5F0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A01068);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01068) { __imp__sub_82A01068(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01068) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r3,r11,-20824
	ctx.r3.s64 = ctx.r11.s64 + -20824;
	// b 0x82898810
	sub_82898810(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A01078);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01078) { __imp__sub_82A01078(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01078) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r3,r11,-19824
	ctx.r3.s64 = ctx.r11.s64 + -19824;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A01088);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01088) { __imp__sub_82A01088(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01088) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r11,r11,-18836
	ctx.r11.s64 = ctx.r11.s64 + -18836;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A010A4);  /* forward decl */
PPC_WEAK_FUNC(sub_82A010A4) { __imp__sub_82A010A4(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A010A4) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A010A8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A010A8) { __imp__sub_82A010A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A010A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r11,r11,-18884
	ctx.r11.s64 = ctx.r11.s64 + -18884;
	// lhz r10,6(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A010C4);  /* forward decl */
PPC_WEAK_FUNC(sub_82A010C4) { __imp__sub_82A010C4(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A010C4) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A010C8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A010C8) { __imp__sub_82A010C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A010C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// addi r11,r11,13844
	ctx.r11.s64 = ctx.r11.s64 + 13844;
	// addi r9,r11,16
	ctx.r9.s64 = ctx.r11.s64 + 16;
	// lwz r10,20(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// cmplw cr6,r10,r9
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, ctx.r9.u32, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A010F0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A010F0) { __imp__sub_82A010F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A010F0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A010F8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A010F8) { __imp__sub_82A010F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A010F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// addi r11,r11,13888
	ctx.r11.s64 = ctx.r11.s64 + 13888;
	// addi r9,r11,16
	ctx.r9.s64 = ctx.r11.s64 + 16;
	// lwz r10,20(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// cmplw cr6,r10,r9
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, ctx.r9.u32, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A01120);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01120) { __imp__sub_82A01120(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01120) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A01128);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01128) { __imp__sub_82A01128(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01128) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// addi r11,r11,13912
	ctx.r11.s64 = ctx.r11.s64 + 13912;
	// addi r9,r11,16
	ctx.r9.s64 = ctx.r11.s64 + 16;
	// lwz r10,20(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// cmplw cr6,r10,r9
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, ctx.r9.u32, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A01150);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01150) { __imp__sub_82A01150(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01150) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A01158);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01158) { __imp__sub_82A01158(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01158) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// addi r11,r11,13936
	ctx.r11.s64 = ctx.r11.s64 + 13936;
	// addi r9,r11,16
	ctx.r9.s64 = ctx.r11.s64 + 16;
	// lwz r10,20(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// cmplw cr6,r10,r9
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, ctx.r9.u32, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A01180);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01180) { __imp__sub_82A01180(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01180) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A01188);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01188) { __imp__sub_82A01188(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01188) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// addi r11,r11,13960
	ctx.r11.s64 = ctx.r11.s64 + 13960;
	// addi r9,r11,32
	ctx.r9.s64 = ctx.r11.s64 + 32;
	// lwz r10,36(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// cmplw cr6,r10,r9
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, ctx.r9.u32, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A011B0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A011B0) { __imp__sub_82A011B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A011B0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A011B8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A011B8) { __imp__sub_82A011B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A011B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// addi r11,r11,14000
	ctx.r11.s64 = ctx.r11.s64 + 14000;
	// addi r9,r11,32
	ctx.r9.s64 = ctx.r11.s64 + 32;
	// lwz r10,36(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// cmplw cr6,r10,r9
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, ctx.r9.u32, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A011E0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A011E0) { __imp__sub_82A011E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A011E0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A011E8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A011E8) { __imp__sub_82A011E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A011E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// addi r11,r11,14040
	ctx.r11.s64 = ctx.r11.s64 + 14040;
	// addi r9,r11,16
	ctx.r9.s64 = ctx.r11.s64 + 16;
	// lwz r10,20(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// cmplw cr6,r10,r9
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, ctx.r9.u32, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A01210);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01210) { __imp__sub_82A01210(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01210) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A01218);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01218) { __imp__sub_82A01218(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01218) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// addi r11,r11,14096
	ctx.r11.s64 = ctx.r11.s64 + 14096;
	// addi r9,r11,16
	ctx.r9.s64 = ctx.r11.s64 + 16;
	// lwz r10,20(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// cmplw cr6,r10,r9
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, ctx.r9.u32, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A01240);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01240) { __imp__sub_82A01240(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01240) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A01248);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01248) { __imp__sub_82A01248(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01248) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// addi r11,r11,14120
	ctx.r11.s64 = ctx.r11.s64 + 14120;
	// addi r9,r11,32
	ctx.r9.s64 = ctx.r11.s64 + 32;
	// lwz r10,36(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// cmplw cr6,r10,r9
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, ctx.r9.u32, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// b 0x8218be78
	sub_8218BE78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A01270);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01270) { __imp__sub_82A01270(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01270) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A01278);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01278) { __imp__sub_82A01278(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01278) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r3,r11,-3688
	ctx.r3.s64 = ctx.r11.s64 + -3688;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A01288);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01288) { __imp__sub_82A01288(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01288) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// lis r10,-32087
	ctx.r10.s64 = -2102853632;
	// addi r11,r11,25908
	ctx.r11.s64 = ctx.r11.s64 + 25908;
	// stw r11,17776(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17776, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A012A0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A012A0) { __imp__sub_82A012A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A012A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// lis r10,-32087
	ctx.r10.s64 = -2102853632;
	// addi r11,r11,25908
	ctx.r11.s64 = ctx.r11.s64 + 25908;
	// stw r11,17780(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17780, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A012B8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A012B8) { __imp__sub_82A012B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A012B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// lis r10,-32087
	ctx.r10.s64 = -2102853632;
	// addi r11,r11,25908
	ctx.r11.s64 = ctx.r11.s64 + 25908;
	// stw r11,17784(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17784, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A012D0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A012D0) { __imp__sub_82A012D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A012D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// lis r10,-32087
	ctx.r10.s64 = -2102853632;
	// addi r11,r11,25908
	ctx.r11.s64 = ctx.r11.s64 + 25908;
	// stw r11,18048(r10)
	PPC_STORE_U32(ctx.r10.u32 + 18048, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A012E8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A012E8) { __imp__sub_82A012E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A012E8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r31,r11,2320
	ctx.r31.s64 = ctx.r11.s64 + 2320;
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// ble cr6,0x82a01314
	if (!ctx.cr6.gt) goto loc_82A01314;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// bl 0x8218be78
	ctx.lr = 0x82A01314;
	sub_8218BE78(ctx, base);
loc_82A01314:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r11.u32);
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// stw r11,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r11.u32);
	// stw r11,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A01340);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01340) { __imp__sub_82A01340(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01340) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r31,r11,2368
	ctx.r31.s64 = ctx.r11.s64 + 2368;
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// ble cr6,0x82a0136c
	if (!ctx.cr6.gt) goto loc_82A0136C;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// bl 0x8218be78
	ctx.lr = 0x82A0136C;
	sub_8218BE78(ctx, base);
loc_82A0136C:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r11.u32);
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// stw r11,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r11.u32);
	// stw r11,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A01398);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01398) { __imp__sub_82A01398(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01398) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// lis r10,-32087
	ctx.r10.s64 = -2102853632;
	// addi r11,r11,25908
	ctx.r11.s64 = ctx.r11.s64 + 25908;
	// stw r11,19876(r10)
	PPC_STORE_U32(ctx.r10.u32 + 19876, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A013B0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A013B0) { __imp__sub_82A013B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A013B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// lis r10,-32087
	ctx.r10.s64 = -2102853632;
	// addi r11,r11,25908
	ctx.r11.s64 = ctx.r11.s64 + 25908;
	// stw r11,19880(r10)
	PPC_STORE_U32(ctx.r10.u32 + 19880, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A013C8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A013C8) { __imp__sub_82A013C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A013C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// lis r10,-32087
	ctx.r10.s64 = -2102853632;
	// addi r11,r11,25908
	ctx.r11.s64 = ctx.r11.s64 + 25908;
	// stw r11,19884(r10)
	PPC_STORE_U32(ctx.r10.u32 + 19884, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A013E0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A013E0) { __imp__sub_82A013E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A013E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,3088
	ctx.r31.s64 = ctx.r10.s64 + 3088;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A01408;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01410;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01430);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01430) { __imp__sub_82A01430(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01430) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r3,r11,3204
	ctx.r3.s64 = ctx.r11.s64 + 3204;
	// b 0x8296ca60
	sub_8296CA60(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A01440);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01440) { __imp__sub_82A01440(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01440) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// addi r31,r10,3424
	ctx.r31.s64 = ctx.r10.s64 + 3424;
	// addi r11,r11,32480
	ctx.r11.s64 = ctx.r11.s64 + 32480;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82974d60
	ctx.lr = 0x82A01468;
	sub_82974D60(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01470;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01490);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01490) { __imp__sub_82A01490(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01490) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// addi r31,r10,3264
	ctx.r31.s64 = ctx.r10.s64 + 3264;
	// addi r11,r11,32480
	ctx.r11.s64 = ctx.r11.s64 + 32480;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82974d60
	ctx.lr = 0x82A014B8;
	sub_82974D60(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A014C0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A014E0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A014E0) { __imp__sub_82A014E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A014E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// addi r31,r10,3296
	ctx.r31.s64 = ctx.r10.s64 + 3296;
	// addi r11,r11,32480
	ctx.r11.s64 = ctx.r11.s64 + 32480;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82974d60
	ctx.lr = 0x82A01508;
	sub_82974D60(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01510;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01530);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01530) { __imp__sub_82A01530(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01530) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// addi r31,r10,3328
	ctx.r31.s64 = ctx.r10.s64 + 3328;
	// addi r11,r11,32480
	ctx.r11.s64 = ctx.r11.s64 + 32480;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82974d60
	ctx.lr = 0x82A01558;
	sub_82974D60(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01560;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01580);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01580) { __imp__sub_82A01580(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01580) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// addi r31,r10,3392
	ctx.r31.s64 = ctx.r10.s64 + 3392;
	// addi r11,r11,32480
	ctx.r11.s64 = ctx.r11.s64 + 32480;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82974d60
	ctx.lr = 0x82A015A8;
	sub_82974D60(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A015B0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A015D0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A015D0) { __imp__sub_82A015D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A015D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// addi r31,r10,3232
	ctx.r31.s64 = ctx.r10.s64 + 3232;
	// addi r11,r11,32480
	ctx.r11.s64 = ctx.r11.s64 + 32480;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82974d60
	ctx.lr = 0x82A015F8;
	sub_82974D60(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01600;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01620);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01620) { __imp__sub_82A01620(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01620) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// addi r31,r10,3360
	ctx.r31.s64 = ctx.r10.s64 + 3360;
	// addi r11,r11,32480
	ctx.r11.s64 = ctx.r11.s64 + 32480;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82974d60
	ctx.lr = 0x82A01648;
	sub_82974D60(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01650;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01670);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01670) { __imp__sub_82A01670(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01670) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// addi r31,r10,3456
	ctx.r31.s64 = ctx.r10.s64 + 3456;
	// addi r11,r11,32480
	ctx.r11.s64 = ctx.r11.s64 + 32480;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82974d60
	ctx.lr = 0x82A01698;
	sub_82974D60(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A016A0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A016C0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A016C0) { __imp__sub_82A016C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A016C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,3488
	ctx.r31.s64 = ctx.r10.s64 + 3488;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A016E8;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A016F0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01710);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01710) { __imp__sub_82A01710(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01710) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,3552
	ctx.r31.s64 = ctx.r10.s64 + 3552;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A01738;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01740;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01760);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01760) { __imp__sub_82A01760(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01760) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,3520
	ctx.r31.s64 = ctx.r10.s64 + 3520;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A01788;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01790;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A017B0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A017B0) { __imp__sub_82A017B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A017B0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,3604
	ctx.r31.s64 = ctx.r10.s64 + 3604;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A017D8;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A017E0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01800);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01800) { __imp__sub_82A01800(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01800) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,3640
	ctx.r31.s64 = ctx.r10.s64 + 3640;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A01828;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01830;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01850);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01850) { __imp__sub_82A01850(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01850) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,3672
	ctx.r31.s64 = ctx.r10.s64 + 3672;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A01878;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01880;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A018A0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A018A0) { __imp__sub_82A018A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A018A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r3,r11,3704
	ctx.r3.s64 = ctx.r11.s64 + 3704;
	// b 0x8296ca60
	sub_8296CA60(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A018B0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A018B0) { __imp__sub_82A018B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A018B0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r31,r10,3764
	ctx.r31.s64 = ctx.r10.s64 + 3764;
	// addi r11,r11,-30676
	ctx.r11.s64 = ctx.r11.s64 + -30676;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298a440
	ctx.lr = 0x82A018D8;
	sub_8298A440(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A018E0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01900);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01900) { __imp__sub_82A01900(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01900) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r31,r10,3860
	ctx.r31.s64 = ctx.r10.s64 + 3860;
	// addi r11,r11,-30676
	ctx.r11.s64 = ctx.r11.s64 + -30676;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298a440
	ctx.lr = 0x82A01928;
	sub_8298A440(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01930;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01950);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01950) { __imp__sub_82A01950(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01950) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r31,r10,3796
	ctx.r31.s64 = ctx.r10.s64 + 3796;
	// addi r11,r11,-30676
	ctx.r11.s64 = ctx.r11.s64 + -30676;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298a440
	ctx.lr = 0x82A01978;
	sub_8298A440(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01980;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A019A0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A019A0) { __imp__sub_82A019A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A019A0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r31,r10,3956
	ctx.r31.s64 = ctx.r10.s64 + 3956;
	// addi r11,r11,-30676
	ctx.r11.s64 = ctx.r11.s64 + -30676;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298a440
	ctx.lr = 0x82A019C8;
	sub_8298A440(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A019D0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A019F0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A019F0) { __imp__sub_82A019F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A019F0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r31,r10,3828
	ctx.r31.s64 = ctx.r10.s64 + 3828;
	// addi r11,r11,-30676
	ctx.r11.s64 = ctx.r11.s64 + -30676;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298a440
	ctx.lr = 0x82A01A18;
	sub_8298A440(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01A20;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01A40);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01A40) { __imp__sub_82A01A40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01A40) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r31,r10,3732
	ctx.r31.s64 = ctx.r10.s64 + 3732;
	// addi r11,r11,-30676
	ctx.r11.s64 = ctx.r11.s64 + -30676;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298a440
	ctx.lr = 0x82A01A68;
	sub_8298A440(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01A70;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01A90);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01A90) { __imp__sub_82A01A90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01A90) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r31,r10,3892
	ctx.r31.s64 = ctx.r10.s64 + 3892;
	// addi r11,r11,-30676
	ctx.r11.s64 = ctx.r11.s64 + -30676;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298a440
	ctx.lr = 0x82A01AB8;
	sub_8298A440(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01AC0;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01AE0);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01AE0) { __imp__sub_82A01AE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01AE0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r31,r10,3988
	ctx.r31.s64 = ctx.r10.s64 + 3988;
	// addi r11,r11,-30676
	ctx.r11.s64 = ctx.r11.s64 + -30676;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298a440
	ctx.lr = 0x82A01B08;
	sub_8298A440(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01B10;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01B30);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01B30) { __imp__sub_82A01B30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01B30) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r31,r10,3924
	ctx.r31.s64 = ctx.r10.s64 + 3924;
	// addi r11,r11,-30676
	ctx.r11.s64 = ctx.r11.s64 + -30676;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298a440
	ctx.lr = 0x82A01B58;
	sub_8298A440(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01B60;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01B80);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01B80) { __imp__sub_82A01B80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01B80) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r30,3
	ctx.r30.s64 = 3;
	// addi r11,r11,4248
	ctx.r11.s64 = ctx.r11.s64 + 4248;
	// addi r31,r11,896
	ctx.r31.s64 = ctx.r11.s64 + 896;
loc_82A01BA4:
	// addi r31,r31,-224
	ctx.r31.s64 = ctx.r31.s64 + -224;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82973d68
	ctx.lr = 0x82A01BB0;
	sub_82973D68(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a01ba4
	if (!ctx.cr6.lt) goto loc_82A01BA4;
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

PPC_FUNC_IMPL(__imp__sub_82A01BD8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01BD8) { __imp__sub_82A01BD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01BD8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,5720
	ctx.r31.s64 = ctx.r10.s64 + 5720;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A01C00;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01C08;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01C28);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01C28) { __imp__sub_82A01C28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01C28) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,5496
	ctx.r31.s64 = ctx.r10.s64 + 5496;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A01C50;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01C58;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01C78);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01C78) { __imp__sub_82A01C78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01C78) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,5560
	ctx.r31.s64 = ctx.r10.s64 + 5560;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A01CA0;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01CA8;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01CC8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01CC8) { __imp__sub_82A01CC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01CC8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,5304
	ctx.r31.s64 = ctx.r10.s64 + 5304;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A01CF0;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01CF8;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01D18);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01D18) { __imp__sub_82A01D18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01D18) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,5176
	ctx.r31.s64 = ctx.r10.s64 + 5176;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A01D40;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01D48;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01D68);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01D68) { __imp__sub_82A01D68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01D68) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,5400
	ctx.r31.s64 = ctx.r10.s64 + 5400;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A01D90;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01D98;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01DB8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01DB8) { __imp__sub_82A01DB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01DB8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,4180
	ctx.r31.s64 = ctx.r10.s64 + 4180;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A01DE0;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01DE8;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01E08);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01E08) { __imp__sub_82A01E08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01E08) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,5624
	ctx.r31.s64 = ctx.r10.s64 + 5624;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A01E30;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01E38;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01E58);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01E58) { __imp__sub_82A01E58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01E58) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,5208
	ctx.r31.s64 = ctx.r10.s64 + 5208;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A01E80;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01E88;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01EA8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01EA8) { __imp__sub_82A01EA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01EA8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,5784
	ctx.r31.s64 = ctx.r10.s64 + 5784;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A01ED0;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01ED8;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01EF8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01EF8) { __imp__sub_82A01EF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01EF8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,5144
	ctx.r31.s64 = ctx.r10.s64 + 5144;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A01F20;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01F28;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01F48);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01F48) { __imp__sub_82A01F48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01F48) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,5592
	ctx.r31.s64 = ctx.r10.s64 + 5592;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A01F70;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01F78;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01F98);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01F98) { __imp__sub_82A01F98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01F98) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,4212
	ctx.r31.s64 = ctx.r10.s64 + 4212;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A01FC0;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A01FC8;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A01FE8);  /* forward decl */
PPC_WEAK_FUNC(sub_82A01FE8) { __imp__sub_82A01FE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A01FE8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,5752
	ctx.r31.s64 = ctx.r10.s64 + 5752;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A02010;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A02018;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A02038);  /* forward decl */
PPC_WEAK_FUNC(sub_82A02038) { __imp__sub_82A02038(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A02038) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,5656
	ctx.r31.s64 = ctx.r10.s64 + 5656;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A02060;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A02068;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

PPC_FUNC_IMPL(__imp__sub_82A02088);  /* forward decl */
PPC_WEAK_FUNC(sub_82A02088) { __imp__sub_82A02088(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82A02088) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,4116
	ctx.r31.s64 = ctx.r10.s64 + 4116;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x82A020B0;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c7d8
	ctx.lr = 0x82A020B8;
	sub_8296C7D8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,28688
	ctx.r11.s64 = ctx.r11.s64 + 28688;
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

