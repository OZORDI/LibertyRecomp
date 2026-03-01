#include "gta4_init.h"

PPC_FUNC_IMPL(__imp__sub_8259C838);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C838) { __imp__sub_8259C838(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C838) {
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
	// b 0x8259c570
	sub_8259C570(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C858);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C858) { __imp__sub_8259C858(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C858) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x8259aaa8
	ctx.lr = 0x8259C878;
	sub_8259AAA8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259C898);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C898) { __imp__sub_8259C898(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C898) {
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
	ctx.lr = 0x8259C904;
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

PPC_FUNC_IMPL(__imp__sub_8259C918);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C918) { __imp__sub_8259C918(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C918) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb10
	ctx.lr = 0x8259C920;
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
	ctx.lr = 0x8259C98C;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// addi r1,r1,192
	ctx.r1.s64 = ctx.r1.s64 + 192;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C998);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C998) { __imp__sub_8259C998(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C998) {
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
	ctx.lr = 0x8259CA10;
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

PPC_FUNC_IMPL(__imp__sub_8259CA20);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CA20) { __imp__sub_8259CA20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CA20) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x8259CA28;
	__savegprlr_29(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32061
	ctx.r10.s64 = -2101149696;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,12544
	ctx.r31.s64 = ctx.r10.s64 + 12544;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r30,4(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r29,8(r11)
	ctx.r29.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x82273620
	ctx.lr = 0x8259CA4C;
	sub_82273620(ctx, base);
	// li r11,-1
	ctx.r11.s64 = -1;
	// lwz r4,352(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
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
	// bl 0x8221e788
	ctx.lr = 0x8259CA7C;
	sub_8221E788(ctx, base);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CA88);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CA88) { __imp__sub_8259CA88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CA88) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x82597768
	sub_82597768(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CAA0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CAA0) { __imp__sub_8259CAA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CAA0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x82597800
	sub_82597800(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CAB8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CAB8) { __imp__sub_8259CAB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CAB8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x8259CAC0;
	__savegprlr_29(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32061
	ctx.r10.s64 = -2101149696;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,12544
	ctx.r31.s64 = ctx.r10.s64 + 12544;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r30,4(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r29,8(r11)
	ctx.r29.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x82273620
	ctx.lr = 0x8259CAE4;
	sub_82273620(ctx, base);
	// li r11,-1
	ctx.r11.s64 = -1;
	// lwz r4,352(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
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
	// bl 0x8221e788
	ctx.lr = 0x8259CB14;
	sub_8221E788(ctx, base);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CB20);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CB20) { __imp__sub_8259CB20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CB20) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x8259CB28;
	__savegprlr_28(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32061
	ctx.r10.s64 = -2101149696;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,12544
	ctx.r31.s64 = ctx.r10.s64 + 12544;
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
	// bl 0x82273620
	ctx.lr = 0x8259CB50;
	sub_82273620(ctx, base);
	// li r11,-1
	ctx.r11.s64 = -1;
	// lwz r4,352(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
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
	// bl 0x8221e788
	ctx.lr = 0x8259CB80;
	sub_8221E788(ctx, base);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CB88);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CB88) { __imp__sub_8259CB88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CB88) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x8259CB90;
	__savegprlr_27(ctx, base);
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32061
	ctx.r10.s64 = -2101149696;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,12544
	ctx.r31.s64 = ctx.r10.s64 + 12544;
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
	// bl 0x82273620
	ctx.lr = 0x8259CBBC;
	sub_82273620(ctx, base);
	// li r11,-1
	ctx.r11.s64 = -1;
	// lwz r4,352(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
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
	// bl 0x8221e788
	ctx.lr = 0x8259CBEC;
	sub_8221E788(ctx, base);
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CBF8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CBF8) { __imp__sub_8259CBF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CBF8) {
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
	// b 0x82597910
	sub_82597910(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CC10);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CC10) { __imp__sub_8259CC10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CC10) {
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
	// b 0x825979a8
	sub_825979A8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CC28);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CC28) { __imp__sub_8259CC28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CC28) {
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
	// b 0x82597a40
	sub_82597A40(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CC48);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CC48) { __imp__sub_8259CC48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CC48) {
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
	// b 0x82597ae0
	sub_82597AE0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CC68);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CC68) { __imp__sub_8259CC68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CC68) {
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
	// b 0x82597b80
	sub_82597B80(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CC88);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CC88) { __imp__sub_8259CC88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CC88) {
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
	// b 0x82597c20
	sub_82597C20(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CCA8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CCA8) { __imp__sub_8259CCA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CCA8) {
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
	// b 0x82597cc0
	sub_82597CC0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CCD0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CCD0) { __imp__sub_8259CCD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CCD0) {
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
	// b 0x82597d68
	sub_82597D68(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CCF8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CCF8) { __imp__sub_8259CCF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CCF8) {
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
	// b 0x82597e10
	sub_82597E10(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CD20);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CD20) { __imp__sub_8259CD20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CD20) {
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
	// b 0x82597eb8
	sub_82597EB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CD48);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CD48) { __imp__sub_8259CD48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CD48) {
	PPC_FUNC_PROLOGUE();
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
	// bl 0x82597f60
	ctx.lr = 0x8259CD84;
	sub_82597F60(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CD98);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CD98) { __imp__sub_8259CD98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CD98) {
	PPC_FUNC_PROLOGUE();
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
	// bl 0x82598008
	ctx.lr = 0x8259CDD4;
	sub_82598008(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CDE8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CDE8) { __imp__sub_8259CDE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CDE8) {
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
	// b 0x825980b0
	sub_825980B0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CE00);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CE00) { __imp__sub_8259CE00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CE00) {
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
	// b 0x82598160
	sub_82598160(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CE18);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CE18) { __imp__sub_8259CE18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CE18) {
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
	// b 0x82598210
	sub_82598210(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CE30);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CE30) { __imp__sub_8259CE30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CE30) {
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
	// b 0x825982d0
	sub_825982D0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CE48);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CE48) { __imp__sub_8259CE48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CE48) {
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
	// b 0x82598390
	sub_82598390(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CE68);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CE68) { __imp__sub_8259CE68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CE68) {
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
	// b 0x82598468
	sub_82598468(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CE88);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CE88) { __imp__sub_8259CE88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CE88) {
	PPC_FUNC_PROLOGUE();
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
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// addi r3,r11,12544
	ctx.r3.s64 = ctx.r11.s64 + 12544;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x82273620
	ctx.lr = 0x8259CEA8;
	sub_82273620(ctx, base);
	// bl 0x8221fde8
	ctx.lr = 0x8259CEAC;
	sub_8221FDE8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CEC0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CEC0) { __imp__sub_8259CEC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CEC0) {
	PPC_FUNC_PROLOGUE();
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
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// addi r3,r11,12544
	ctx.r3.s64 = ctx.r11.s64 + 12544;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x82273620
	ctx.lr = 0x8259CEE0;
	sub_82273620(ctx, base);
	// bl 0x8221e5b0
	ctx.lr = 0x8259CEE4;
	sub_8221E5B0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CEF8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CEF8) { __imp__sub_8259CEF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CEF8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x8259CF00;
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
	// bl 0x82598540
	ctx.lr = 0x8259CF48;
	sub_82598540(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CF60);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CF60) { __imp__sub_8259CF60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CF60) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32061
	ctx.r10.s64 = -2101149696;
	// addi r3,r10,12544
	ctx.r3.s64 = ctx.r10.s64 + 12544;
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x82273ca8
	sub_82273CA8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CF78);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CF78) { __imp__sub_8259CF78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CF78) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32061
	ctx.r10.s64 = -2101149696;
	// addi r3,r10,12544
	ctx.r3.s64 = ctx.r10.s64 + 12544;
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x82273e18
	sub_82273E18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259CF90);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CF90) { __imp__sub_8259CF90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CF90) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r10,-32061
	ctx.r10.s64 = -2101149696;
	// addi r3,r10,12544
	ctx.r3.s64 = ctx.r10.s64 + 12544;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x82272db0
	ctx.lr = 0x8259CFBC;
	sub_82272DB0(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259CFE0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259CFE0) { __imp__sub_8259CFE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259CFE0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r10,-32061
	ctx.r10.s64 = -2101149696;
	// addi r3,r10,12544
	ctx.r3.s64 = ctx.r10.s64 + 12544;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x82272e10
	ctx.lr = 0x8259D00C;
	sub_82272E10(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259D030);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D030) { __imp__sub_8259D030(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D030) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// addi r3,r11,12544
	ctx.r3.s64 = ctx.r11.s64 + 12544;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x82273168
	ctx.lr = 0x8259D058;
	sub_82273168(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259D088);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D088) { __imp__sub_8259D088(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D088) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x82598710
	ctx.lr = 0x8259D0A8;
	sub_82598710(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259D0C8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D0C8) { __imp__sub_8259D0C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D0C8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x82598780
	ctx.lr = 0x8259D0EC;
	sub_82598780(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259D108);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D108) { __imp__sub_8259D108(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D108) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x82598840
	ctx.lr = 0x8259D12C;
	sub_82598840(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259D148);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D148) { __imp__sub_8259D148(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D148) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x825988e0
	ctx.lr = 0x8259D168;
	sub_825988E0(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259D188);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D188) { __imp__sub_8259D188(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D188) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x82598970
	ctx.lr = 0x8259D1B0;
	sub_82598970(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259D1D0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D1D0) { __imp__sub_8259D1D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D1D0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8259d1ec
	if (!ctx.cr6.eq) goto loc_8259D1EC;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
loc_8259D1EC:
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_8259D1F0:
	// lbz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// bne cr6,0x8259d1f0
	if (!ctx.cr6.eq) goto loc_8259D1F0;
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

PPC_FUNC_IMPL(__imp__sub_8259D218);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D218) { __imp__sub_8259D218(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D218) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x82598a30
	ctx.lr = 0x8259D23C;
	sub_82598A30(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259D258);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D258) { __imp__sub_8259D258(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D258) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x8259D260;
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
	// bl 0x823d83c0
	ctx.lr = 0x8259D284;
	sub_823D83C0(ctx, base);
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
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D2C0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D2C0) { __imp__sub_8259D2C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D2C0) {
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
	// b 0x8259b3c8
	sub_8259B3C8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D2E0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D2E0) { __imp__sub_8259D2E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D2E0) {
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
	// lis r10,-32061
	ctx.r10.s64 = -2101149696;
	// addi r3,r10,12544
	ctx.r3.s64 = ctx.r10.s64 + 12544;
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
	// bl 0x82273620
	ctx.lr = 0x8259D318;
	sub_82273620(ctx, base);
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
	// fmr f2,f30
	ctx.fpscr.disableFlushMode();
	ctx.f2.f64 = ctx.f30.f64;
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// bl 0x82598ad8
	ctx.lr = 0x8259D328;
	sub_82598AD8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259D350);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D350) { __imp__sub_8259D350(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D350) {
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
	// bl 0x82598c50
	ctx.lr = 0x8259D380;
	sub_82598C50(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259D3A0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D3A0) { __imp__sub_8259D3A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D3A0) {
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
	// bl 0x82598d60
	ctx.lr = 0x8259D3D0;
	sub_82598D60(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259D3F0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D3F0) { __imp__sub_8259D3F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D3F0) {
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
	// b 0x82598e38
	sub_82598E38(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D408);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D408) { __imp__sub_8259D408(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D408) {
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
	// b 0x82598f00
	sub_82598F00(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D420);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D420) { __imp__sub_8259D420(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D420) {
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
	// b 0x82598fd8
	sub_82598FD8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D440);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D440) { __imp__sub_8259D440(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D440) {
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
	// b 0x825990b8
	sub_825990B8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D460);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D460) { __imp__sub_8259D460(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D460) {
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
	// b 0x825991a8
	sub_825991A8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D480);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D480) { __imp__sub_8259D480(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D480) {
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
	// b 0x82599290
	sub_82599290(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D498);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D498) { __imp__sub_8259D498(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D498) {
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
	// b 0x82599378
	sub_82599378(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D4B8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D4B8) { __imp__sub_8259D4B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D4B8) {
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
	// b 0x82599488
	sub_82599488(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D4D0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D4D0) { __imp__sub_8259D4D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D4D0) {
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
	// b 0x82599570
	sub_82599570(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D4F0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D4F0) { __imp__sub_8259D4F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D4F0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,0
	ctx.r4.s64 = 0;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x82599940
	sub_82599940(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D508);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D508) { __imp__sub_8259D508(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D508) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r5,1
	ctx.r5.s64 = 1;
	// li r4,0
	ctx.r4.s64 = 0;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x82599940
	sub_82599940(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D520);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D520) { __imp__sub_8259D520(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D520) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r5,0
	ctx.r5.s64 = 0;
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x82599a50
	sub_82599A50(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D538);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D538) { __imp__sub_8259D538(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D538) {
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
	// b 0x82599b70
	sub_82599B70(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D550);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D550) { __imp__sub_8259D550(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D550) {
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
	// b 0x82599b70
	sub_82599B70(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D568);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D568) { __imp__sub_8259D568(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D568) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,1
	ctx.r4.s64 = 1;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x82599940
	sub_82599940(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D580);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D580) { __imp__sub_8259D580(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D580) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r5,1
	ctx.r5.s64 = 1;
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x82599a50
	sub_82599A50(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D598);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D598) { __imp__sub_8259D598(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D598) {
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
	// b 0x82599b70
	sub_82599B70(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D5B0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D5B0) { __imp__sub_8259D5B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D5B0) {
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
	// b 0x82599b70
	sub_82599B70(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D5C8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D5C8) { __imp__sub_8259D5C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D5C8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f0,23444(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 23444);
	ctx.f0.f64 = double(temp.f32);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stfs f0,0(r10)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// lfs f0,23440(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 23440);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r9)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D5F0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D5F0) { __imp__sub_8259D5F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D5F0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x82599ca0
	ctx.lr = 0x8259D610;
	sub_82599CA0(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259D630);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D630) { __imp__sub_8259D630(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D630) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x82599d28
	ctx.lr = 0x8259D654;
	sub_82599D28(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259D678);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D678) { __imp__sub_8259D678(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D678) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x82599dc0
	ctx.lr = 0x8259D69C;
	sub_82599DC0(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259D6C0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D6C0) { __imp__sub_8259D6C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D6C0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825997c0
	sub_825997C0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D6D0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D6D0) { __imp__sub_8259D6D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D6D0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x82599880
	sub_82599880(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D6E0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D6E0) { __imp__sub_8259D6E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D6E0) {
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
	// bl 0x821b2f60
	ctx.lr = 0x8259D72C;
	sub_821B2F60(ctx, base);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D740);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D740) { __imp__sub_8259D740(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D740) {
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
	// b 0x821b4220
	sub_821B4220(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D770);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D770) { __imp__sub_8259D770(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D770) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,-24832
	ctx.r3.s64 = ctx.r11.s64 + -24832;
	// b 0x8259c898
	sub_8259C898(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D780);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D780) { __imp__sub_8259D780(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D780) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x8259D788;
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
	// bl 0x8225e558
	ctx.lr = 0x8259D7A8;
	sub_8225E558(ctx, base);
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
	// bl 0x8225e558
	ctx.lr = 0x8259D7C0;
	sub_8225E558(ctx, base);
	// lfs f0,4(r3)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r29)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r29.u32 + 0, temp.u32);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D7D0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D7D0) { __imp__sub_8259D7D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D7D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,-24640
	ctx.r3.s64 = ctx.r11.s64 + -24640;
	// b 0x8259c918
	sub_8259C918(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D7E0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D7E0) { __imp__sub_8259D7E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D7E0) {
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
loc_8259D810:
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r9,r11
	PPC_STORE_U8(ctx.r9.u32 + ctx.r11.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x8259d810
	if (!ctx.cr6.eq) goto loc_8259D810;
	// li r8,-1
	ctx.r8.s64 = -1;
	// addi r6,r1,80
	ctx.r6.s64 = ctx.r1.s64 + 80;
	// bl 0x8225e750
	ctx.lr = 0x8259D830;
	sub_8225E750(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D840);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D840) { __imp__sub_8259D840(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D840) {
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
loc_8259D870:
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r9,r11
	PPC_STORE_U8(ctx.r9.u32 + ctx.r11.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x8259d870
	if (!ctx.cr6.eq) goto loc_8259D870;
	// addi r6,r1,80
	ctx.r6.s64 = ctx.r1.s64 + 80;
	// bl 0x8225e750
	ctx.lr = 0x8259D88C;
	sub_8225E750(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D8A0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D8A0) { __imp__sub_8259D8A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D8A0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8259b1b0
	sub_8259B1B0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D8B0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D8B0) { __imp__sub_8259D8B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D8B0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8259b258
	sub_8259B258(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D8C0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D8C0) { __imp__sub_8259D8C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D8C0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8259b338
	sub_8259B338(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D8D0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D8D0) { __imp__sub_8259D8D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D8D0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8259a678
	sub_8259A678(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D8E0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D8E0) { __imp__sub_8259D8E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D8E0) {
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
	// b 0x8259a888
	sub_8259A888(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D8F8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D8F8) { __imp__sub_8259D8F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D8F8) {
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
	// b 0x8259a958
	sub_8259A958(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D910);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D910) { __imp__sub_8259D910(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D910) {
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
	// bl 0x821da6e8
	ctx.lr = 0x8259D944;
	sub_821DA6E8(ctx, base);
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// li r3,17
	ctx.r3.s64 = 17;
	// bl 0x822ca248
	ctx.lr = 0x8259D954;
	sub_822CA248(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259D970);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D970) { __imp__sub_8259D970(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D970) {
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
	// bl 0x821da6e8
	ctx.lr = 0x8259D9A4;
	sub_821DA6E8(ctx, base);
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// li r3,18
	ctx.r3.s64 = 18;
	// bl 0x822ca248
	ctx.lr = 0x8259D9B4;
	sub_822CA248(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259D9D0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D9D0) { __imp__sub_8259D9D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D9D0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8259abe0
	sub_8259ABE0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D9E0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D9E0) { __imp__sub_8259D9E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D9E0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8259ab18
	sub_8259AB18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259D9F0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259D9F0) { __imp__sub_8259D9F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259D9F0) {
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
	// lis r9,-32081
	ctx.r9.s64 = -2102460416;
	// addi r3,r9,-16736
	ctx.r3.s64 = ctx.r9.s64 + -16736;
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
	// bl 0x823accd8
	ctx.lr = 0x8259DA3C;
	sub_823ACCD8(ctx, base);
	// lis r11,-32039
	ctx.r11.s64 = -2099707904;
	// lwz r30,15072(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 15072);
	// bl 0x821d2088
	ctx.lr = 0x8259DA48;
	sub_821D2088(ctx, base);
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
	// bl 0x823093f8
	ctx.lr = 0x8259DA64;
	sub_823093F8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259DA80);  /* forward decl */
PPC_WEAK_FUNC(sub_8259DA80) { __imp__sub_8259DA80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259DA80) {
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
	// lis r11,-32081
	ctx.r11.s64 = -2102460416;
	// addi r3,r11,-16736
	ctx.r3.s64 = ctx.r11.s64 + -16736;
	// lwz r31,0(r10)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x823acde8
	ctx.lr = 0x8259DAAC;
	sub_823ACDE8(ctx, base);
	// lis r11,-32039
	ctx.r11.s64 = -2099707904;
	// lwz r30,15072(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 15072);
	// bl 0x821d2088
	ctx.lr = 0x8259DAB8;
	sub_821D2088(ctx, base);
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
	// bl 0x823086d8
	ctx.lr = 0x8259DAD0;
	sub_823086D8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259DAE8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259DAE8) { __imp__sub_8259DAE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259DAE8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32081
	ctx.r10.s64 = -2102460416;
	// addi r3,r10,-16736
	ctx.r3.s64 = ctx.r10.s64 + -16736;
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x823acea0
	sub_823ACEA0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259DB00);  /* forward decl */
PPC_WEAK_FUNC(sub_8259DB00) { __imp__sub_8259DB00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259DB00) {
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
	// lis r10,-32081
	ctx.r10.s64 = -2102460416;
	// addi r3,r10,-16736
	ctx.r3.s64 = ctx.r10.s64 + -16736;
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
	// bl 0x823acd50
	ctx.lr = 0x8259DB3C;
	sub_823ACD50(ctx, base);
	// lis r11,-32039
	ctx.r11.s64 = -2099707904;
	// lwz r30,15072(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 15072);
	// bl 0x821d2088
	ctx.lr = 0x8259DB48;
	sub_821D2088(ctx, base);
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
	// bl 0x823093f8
	ctx.lr = 0x8259DB64;
	sub_823093F8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259DB80);  /* forward decl */
PPC_WEAK_FUNC(sub_8259DB80) { __imp__sub_8259DB80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259DB80) {
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
	// lis r11,-32081
	ctx.r11.s64 = -2102460416;
	// addi r3,r11,-16736
	ctx.r3.s64 = ctx.r11.s64 + -16736;
	// lwz r31,0(r10)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x823ace30
	ctx.lr = 0x8259DBAC;
	sub_823ACE30(ctx, base);
	// lis r11,-32039
	ctx.r11.s64 = -2099707904;
	// lwz r30,15072(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 15072);
	// bl 0x821d2088
	ctx.lr = 0x8259DBB8;
	sub_821D2088(ctx, base);
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
	// bl 0x823086d8
	ctx.lr = 0x8259DBD0;
	sub_823086D8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259DBE8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259DBE8) { __imp__sub_8259DBE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259DBE8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32081
	ctx.r10.s64 = -2102460416;
	// addi r3,r10,-16736
	ctx.r3.s64 = ctx.r10.s64 + -16736;
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
	// b 0x823ace70
	sub_823ACE70(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259DC20);  /* forward decl */
PPC_WEAK_FUNC(sub_8259DC20) { __imp__sub_8259DC20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259DC20) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r10,-32061
	ctx.r10.s64 = -2101149696;
	// addi r3,r10,12544
	ctx.r3.s64 = ctx.r10.s64 + 12544;
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
	// bl 0x82273620
	ctx.lr = 0x8259DC54;
	sub_82273620(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,23448
	ctx.r3.s64 = ctx.r11.s64 + 23448;
	// clrlwi r5,r31,24
	ctx.r5.u64 = ctx.r31.u32 & 0xFF;
	// bl 0x8221f820
	ctx.lr = 0x8259DC68;
	sub_8221F820(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259DC80);  /* forward decl */
PPC_WEAK_FUNC(sub_8259DC80) { __imp__sub_8259DC80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259DC80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,-14824
	ctx.r3.s64 = ctx.r11.s64 + -14824;
	// b 0x8259c998
	sub_8259C998(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259DC90);  /* forward decl */
PPC_WEAK_FUNC(sub_8259DC90) { __imp__sub_8259DC90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259DC90) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x8259DC98;
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
	// bl 0x8224b570
	ctx.lr = 0x8259DCB8;
	sub_8224B570(ctx, base);
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
	// bl 0x8224b570
	ctx.lr = 0x8259DCD0;
	sub_8224B570(ctx, base);
	// lfs f0,4(r3)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r29)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r29.u32 + 0, temp.u32);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259DCE0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259DCE0) { __imp__sub_8259DCE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259DCE0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x82597898
	sub_82597898(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259DD58);  /* forward decl */
PPC_WEAK_FUNC(sub_8259DD58) { __imp__sub_8259DD58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259DD58) {
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
	// b 0x8259a7f8
	sub_8259A7F8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259DDC0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259DDC0) { __imp__sub_8259DDC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259DDC0) {
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
	// b 0x8222dcd8
	sub_8222DCD8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259DE20);  /* forward decl */
PPC_WEAK_FUNC(sub_8259DE20) { __imp__sub_8259DE20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259DE20) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13792
	ctx.r4.s64 = ctx.r11.s64 + -13792;
	// addi r3,r10,-28016
	ctx.r3.s64 = ctx.r10.s64 + -28016;
	// bl 0x827d6c90
	ctx.lr = 0x8259DE40;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13688
	ctx.r4.s64 = ctx.r11.s64 + -13688;
	// addi r3,r10,-28024
	ctx.r3.s64 = ctx.r10.s64 + -28024;
	// bl 0x827d6c90
	ctx.lr = 0x8259DE54;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13664
	ctx.r4.s64 = ctx.r11.s64 + -13664;
	// addi r3,r10,-28036
	ctx.r3.s64 = ctx.r10.s64 + -28036;
	// bl 0x827d6c90
	ctx.lr = 0x8259DE68;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20512
	ctx.r4.s64 = ctx.r11.s64 + -20512;
	// addi r3,r10,-28052
	ctx.r3.s64 = ctx.r10.s64 + -28052;
	// bl 0x827d6c90
	ctx.lr = 0x8259DE7C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20504
	ctx.r4.s64 = ctx.r11.s64 + -20504;
	// addi r3,r10,-28064
	ctx.r3.s64 = ctx.r10.s64 + -28064;
	// bl 0x827d6c90
	ctx.lr = 0x8259DE90;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13640
	ctx.r4.s64 = ctx.r11.s64 + -13640;
	// addi r3,r10,-28076
	ctx.r3.s64 = ctx.r10.s64 + -28076;
	// bl 0x827d6c90
	ctx.lr = 0x8259DEA4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13536
	ctx.r4.s64 = ctx.r11.s64 + -13536;
	// addi r3,r10,-28100
	ctx.r3.s64 = ctx.r10.s64 + -28100;
	// bl 0x827d6c90
	ctx.lr = 0x8259DEB8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13432
	ctx.r4.s64 = ctx.r11.s64 + -13432;
	// addi r3,r10,-28128
	ctx.r3.s64 = ctx.r10.s64 + -28128;
	// bl 0x827d6c90
	ctx.lr = 0x8259DECC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13320
	ctx.r4.s64 = ctx.r11.s64 + -13320;
	// addi r3,r10,-28148
	ctx.r3.s64 = ctx.r10.s64 + -28148;
	// bl 0x827d6c90
	ctx.lr = 0x8259DEE0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13296
	ctx.r4.s64 = ctx.r11.s64 + -13296;
	// addi r3,r10,-28172
	ctx.r3.s64 = ctx.r10.s64 + -28172;
	// bl 0x827d6c90
	ctx.lr = 0x8259DEF4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13272
	ctx.r4.s64 = ctx.r11.s64 + -13272;
	// addi r3,r10,-28196
	ctx.r3.s64 = ctx.r10.s64 + -28196;
	// bl 0x827d6c90
	ctx.lr = 0x8259DF08;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13240
	ctx.r4.s64 = ctx.r11.s64 + -13240;
	// addi r3,r10,-28224
	ctx.r3.s64 = ctx.r10.s64 + -28224;
	// bl 0x827d6c90
	ctx.lr = 0x8259DF1C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13208
	ctx.r4.s64 = ctx.r11.s64 + -13208;
	// addi r3,r10,-28248
	ctx.r3.s64 = ctx.r10.s64 + -28248;
	// bl 0x827d6c90
	ctx.lr = 0x8259DF30;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13176
	ctx.r4.s64 = ctx.r11.s64 + -13176;
	// addi r3,r10,-28276
	ctx.r3.s64 = ctx.r10.s64 + -28276;
	// bl 0x827d6c90
	ctx.lr = 0x8259DF44;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13144
	ctx.r4.s64 = ctx.r11.s64 + -13144;
	// addi r3,r10,-28300
	ctx.r3.s64 = ctx.r10.s64 + -28300;
	// bl 0x827d6c90
	ctx.lr = 0x8259DF58;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13104
	ctx.r4.s64 = ctx.r11.s64 + -13104;
	// addi r3,r10,-28328
	ctx.r3.s64 = ctx.r10.s64 + -28328;
	// bl 0x827d6c90
	ctx.lr = 0x8259DF6C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13064
	ctx.r4.s64 = ctx.r11.s64 + -13064;
	// addi r3,r10,-28352
	ctx.r3.s64 = ctx.r10.s64 + -28352;
	// bl 0x827d6c90
	ctx.lr = 0x8259DF80;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-13024
	ctx.r4.s64 = ctx.r11.s64 + -13024;
	// addi r3,r10,-28380
	ctx.r3.s64 = ctx.r10.s64 + -28380;
	// bl 0x827d6c90
	ctx.lr = 0x8259DF94;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12984
	ctx.r4.s64 = ctx.r11.s64 + -12984;
	// addi r3,r10,-28404
	ctx.r3.s64 = ctx.r10.s64 + -28404;
	// bl 0x827d6c90
	ctx.lr = 0x8259DFA8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12904
	ctx.r4.s64 = ctx.r11.s64 + -12904;
	// addi r3,r10,-28432
	ctx.r3.s64 = ctx.r10.s64 + -28432;
	// bl 0x827d6c90
	ctx.lr = 0x8259DFBC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12824
	ctx.r4.s64 = ctx.r11.s64 + -12824;
	// addi r3,r10,-28456
	ctx.r3.s64 = ctx.r10.s64 + -28456;
	// bl 0x827d6c90
	ctx.lr = 0x8259DFD0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12800
	ctx.r4.s64 = ctx.r11.s64 + -12800;
	// addi r3,r10,-28484
	ctx.r3.s64 = ctx.r10.s64 + -28484;
	// bl 0x827d6c90
	ctx.lr = 0x8259DFE4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12776
	ctx.r4.s64 = ctx.r11.s64 + -12776;
	// addi r3,r10,-28520
	ctx.r3.s64 = ctx.r10.s64 + -28520;
	// bl 0x827d6c90
	ctx.lr = 0x8259DFF8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12752
	ctx.r4.s64 = ctx.r11.s64 + -12752;
	// addi r3,r10,-28560
	ctx.r3.s64 = ctx.r10.s64 + -28560;
	// bl 0x827d6c90
	ctx.lr = 0x8259E00C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12728
	ctx.r4.s64 = ctx.r11.s64 + -12728;
	// addi r3,r10,-28600
	ctx.r3.s64 = ctx.r10.s64 + -28600;
	// bl 0x827d6c90
	ctx.lr = 0x8259E020;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12696
	ctx.r4.s64 = ctx.r11.s64 + -12696;
	// addi r3,r10,-28644
	ctx.r3.s64 = ctx.r10.s64 + -28644;
	// bl 0x827d6c90
	ctx.lr = 0x8259E034;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12664
	ctx.r4.s64 = ctx.r11.s64 + -12664;
	// addi r3,r10,-28664
	ctx.r3.s64 = ctx.r10.s64 + -28664;
	// bl 0x827d6c90
	ctx.lr = 0x8259E048;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12608
	ctx.r4.s64 = ctx.r11.s64 + -12608;
	// addi r3,r10,-28688
	ctx.r3.s64 = ctx.r10.s64 + -28688;
	// bl 0x827d6c90
	ctx.lr = 0x8259E05C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19352
	ctx.r4.s64 = ctx.r11.s64 + -19352;
	// addi r3,r10,-28716
	ctx.r3.s64 = ctx.r10.s64 + -28716;
	// bl 0x827d6c90
	ctx.lr = 0x8259E070;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20496
	ctx.r4.s64 = ctx.r11.s64 + -20496;
	// addi r3,r10,-28736
	ctx.r3.s64 = ctx.r10.s64 + -28736;
	// bl 0x827d6c90
	ctx.lr = 0x8259E084;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12552
	ctx.r4.s64 = ctx.r11.s64 + -12552;
	// addi r3,r10,-28768
	ctx.r3.s64 = ctx.r10.s64 + -28768;
	// bl 0x827d6c90
	ctx.lr = 0x8259E098;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12448
	ctx.r4.s64 = ctx.r11.s64 + -12448;
	// addi r3,r10,-28792
	ctx.r3.s64 = ctx.r10.s64 + -28792;
	// bl 0x827d6c90
	ctx.lr = 0x8259E0AC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// addi r4,r11,-12424
	ctx.r4.s64 = ctx.r11.s64 + -12424;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r3,r11,-28816
	ctx.r3.s64 = ctx.r11.s64 + -28816;
	// bl 0x827d6c90
	ctx.lr = 0x8259E0C0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19336
	ctx.r4.s64 = ctx.r11.s64 + -19336;
	// addi r3,r10,-28844
	ctx.r3.s64 = ctx.r10.s64 + -28844;
	// bl 0x827d6c90
	ctx.lr = 0x8259E0D4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19264
	ctx.r4.s64 = ctx.r11.s64 + -19264;
	// addi r3,r10,-28868
	ctx.r3.s64 = ctx.r10.s64 + -28868;
	// bl 0x827d6c90
	ctx.lr = 0x8259E0E8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19232
	ctx.r4.s64 = ctx.r11.s64 + -19232;
	// addi r3,r10,-28900
	ctx.r3.s64 = ctx.r10.s64 + -28900;
	// bl 0x827d6c90
	ctx.lr = 0x8259E0FC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12400
	ctx.r4.s64 = ctx.r11.s64 + -12400;
	// addi r3,r10,-28932
	ctx.r3.s64 = ctx.r10.s64 + -28932;
	// bl 0x827d6c90
	ctx.lr = 0x8259E110;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12320
	ctx.r4.s64 = ctx.r11.s64 + -12320;
	// addi r3,r10,-28968
	ctx.r3.s64 = ctx.r10.s64 + -28968;
	// bl 0x827d6c90
	ctx.lr = 0x8259E124;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19160
	ctx.r4.s64 = ctx.r11.s64 + -19160;
	// addi r3,r10,-28996
	ctx.r3.s64 = ctx.r10.s64 + -28996;
	// bl 0x827d6c90
	ctx.lr = 0x8259E138;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12240
	ctx.r4.s64 = ctx.r11.s64 + -12240;
	// addi r3,r10,-29020
	ctx.r3.s64 = ctx.r10.s64 + -29020;
	// bl 0x827d6c90
	ctx.lr = 0x8259E14C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12152
	ctx.r4.s64 = ctx.r11.s64 + -12152;
	// addi r3,r10,-29048
	ctx.r3.s64 = ctx.r10.s64 + -29048;
	// bl 0x827d6c90
	ctx.lr = 0x8259E160;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12088
	ctx.r4.s64 = ctx.r11.s64 + -12088;
	// addi r3,r10,-29084
	ctx.r3.s64 = ctx.r10.s64 + -29084;
	// bl 0x827d6c90
	ctx.lr = 0x8259E174;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-12024
	ctx.r4.s64 = ctx.r11.s64 + -12024;
	// addi r3,r10,-29128
	ctx.r3.s64 = ctx.r10.s64 + -29128;
	// bl 0x827d6c90
	ctx.lr = 0x8259E188;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11960
	ctx.r4.s64 = ctx.r11.s64 + -11960;
	// addi r3,r10,-29172
	ctx.r3.s64 = ctx.r10.s64 + -29172;
	// bl 0x827d6c90
	ctx.lr = 0x8259E19C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11896
	ctx.r4.s64 = ctx.r11.s64 + -11896;
	// addi r3,r10,-29196
	ctx.r3.s64 = ctx.r10.s64 + -29196;
	// bl 0x827d6c90
	ctx.lr = 0x8259E1B0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11824
	ctx.r4.s64 = ctx.r11.s64 + -11824;
	// addi r3,r10,-29228
	ctx.r3.s64 = ctx.r10.s64 + -29228;
	// bl 0x827d6c90
	ctx.lr = 0x8259E1C4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-14664
	ctx.r4.s64 = ctx.r11.s64 + -14664;
	// addi r3,r10,-29256
	ctx.r3.s64 = ctx.r10.s64 + -29256;
	// bl 0x827d6c90
	ctx.lr = 0x8259E1D8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11752
	ctx.r4.s64 = ctx.r11.s64 + -11752;
	// addi r3,r10,-29284
	ctx.r3.s64 = ctx.r10.s64 + -29284;
	// bl 0x827d6c90
	ctx.lr = 0x8259E1EC;
	sub_827D6C90(ctx, base);
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// addi r3,r10,-29316
	ctx.r3.s64 = ctx.r10.s64 + -29316;
	// addi r4,r11,-19048
	ctx.r4.s64 = ctx.r11.s64 + -19048;
	// bl 0x827d6c90
	ctx.lr = 0x8259E200;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-19008
	ctx.r4.s64 = ctx.r11.s64 + -19008;
	// addi r3,r10,-29340
	ctx.r3.s64 = ctx.r10.s64 + -29340;
	// bl 0x827d6c90
	ctx.lr = 0x8259E214;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18968
	ctx.r4.s64 = ctx.r11.s64 + -18968;
	// addi r3,r10,-29352
	ctx.r3.s64 = ctx.r10.s64 + -29352;
	// bl 0x827d6c90
	ctx.lr = 0x8259E228;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18920
	ctx.r4.s64 = ctx.r11.s64 + -18920;
	// addi r3,r10,-29368
	ctx.r3.s64 = ctx.r10.s64 + -29368;
	// bl 0x827d6c90
	ctx.lr = 0x8259E23C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18872
	ctx.r4.s64 = ctx.r11.s64 + -18872;
	// addi r3,r10,-29388
	ctx.r3.s64 = ctx.r10.s64 + -29388;
	// bl 0x827d6c90
	ctx.lr = 0x8259E250;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18824
	ctx.r4.s64 = ctx.r11.s64 + -18824;
	// addi r3,r10,-29400
	ctx.r3.s64 = ctx.r10.s64 + -29400;
	// bl 0x827d6c90
	ctx.lr = 0x8259E264;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18752
	ctx.r4.s64 = ctx.r11.s64 + -18752;
	// addi r3,r10,-29412
	ctx.r3.s64 = ctx.r10.s64 + -29412;
	// bl 0x827d6c90
	ctx.lr = 0x8259E278;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18680
	ctx.r4.s64 = ctx.r11.s64 + -18680;
	// addi r3,r10,-29424
	ctx.r3.s64 = ctx.r10.s64 + -29424;
	// bl 0x827d6c90
	ctx.lr = 0x8259E28C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18640
	ctx.r4.s64 = ctx.r11.s64 + -18640;
	// addi r3,r10,-29448
	ctx.r3.s64 = ctx.r10.s64 + -29448;
	// bl 0x827d6c90
	ctx.lr = 0x8259E2A0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18560
	ctx.r4.s64 = ctx.r11.s64 + -18560;
	// addi r3,r10,-29484
	ctx.r3.s64 = ctx.r10.s64 + -29484;
	// bl 0x827d6c90
	ctx.lr = 0x8259E2B4;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,-29500
	ctx.r3.s64 = ctx.r10.s64 + -29500;
	// bl 0x827d6c90
	ctx.lr = 0x8259E2C8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18528
	ctx.r4.s64 = ctx.r11.s64 + -18528;
	// addi r3,r10,-29516
	ctx.r3.s64 = ctx.r10.s64 + -29516;
	// bl 0x827d6c90
	ctx.lr = 0x8259E2DC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11688
	ctx.r4.s64 = ctx.r11.s64 + -11688;
	// addi r3,r10,-29532
	ctx.r3.s64 = ctx.r10.s64 + -29532;
	// bl 0x827d6c90
	ctx.lr = 0x8259E2F0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11584
	ctx.r4.s64 = ctx.r11.s64 + -11584;
	// addi r3,r10,-29556
	ctx.r3.s64 = ctx.r10.s64 + -29556;
	// bl 0x827d6c90
	ctx.lr = 0x8259E304;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20488
	ctx.r4.s64 = ctx.r11.s64 + -20488;
	// addi r3,r10,-29576
	ctx.r3.s64 = ctx.r10.s64 + -29576;
	// bl 0x827d6c90
	ctx.lr = 0x8259E318;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20456
	ctx.r4.s64 = ctx.r11.s64 + -20456;
	// addi r3,r10,-29596
	ctx.r3.s64 = ctx.r10.s64 + -29596;
	// bl 0x827d6c90
	ctx.lr = 0x8259E32C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18480
	ctx.r4.s64 = ctx.r11.s64 + -18480;
	// addi r3,r10,-29620
	ctx.r3.s64 = ctx.r10.s64 + -29620;
	// bl 0x827d6c90
	ctx.lr = 0x8259E340;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20448
	ctx.r4.s64 = ctx.r11.s64 + -20448;
	// addi r3,r10,-29648
	ctx.r3.s64 = ctx.r10.s64 + -29648;
	// bl 0x827d6c90
	ctx.lr = 0x8259E354;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11552
	ctx.r4.s64 = ctx.r11.s64 + -11552;
	// addi r3,r10,-29668
	ctx.r3.s64 = ctx.r10.s64 + -29668;
	// bl 0x827d6c90
	ctx.lr = 0x8259E368;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11440
	ctx.r4.s64 = ctx.r11.s64 + -11440;
	// addi r3,r10,-29704
	ctx.r3.s64 = ctx.r10.s64 + -29704;
	// bl 0x827d6c90
	ctx.lr = 0x8259E37C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11360
	ctx.r4.s64 = ctx.r11.s64 + -11360;
	// addi r3,r10,-29744
	ctx.r3.s64 = ctx.r10.s64 + -29744;
	// bl 0x827d6c90
	ctx.lr = 0x8259E390;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11280
	ctx.r4.s64 = ctx.r11.s64 + -11280;
	// addi r3,r10,-29760
	ctx.r3.s64 = ctx.r10.s64 + -29760;
	// bl 0x827d6c90
	ctx.lr = 0x8259E3A4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18408
	ctx.r4.s64 = ctx.r11.s64 + -18408;
	// addi r3,r10,-29776
	ctx.r3.s64 = ctx.r10.s64 + -29776;
	// bl 0x827d6c90
	ctx.lr = 0x8259E3B8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18352
	ctx.r4.s64 = ctx.r11.s64 + -18352;
	// addi r3,r10,-29800
	ctx.r3.s64 = ctx.r10.s64 + -29800;
	// bl 0x827d6c90
	ctx.lr = 0x8259E3CC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-14552
	ctx.r4.s64 = ctx.r11.s64 + -14552;
	// addi r3,r10,-29816
	ctx.r3.s64 = ctx.r10.s64 + -29816;
	// bl 0x827d6c90
	ctx.lr = 0x8259E3E0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18296
	ctx.r4.s64 = ctx.r11.s64 + -18296;
	// addi r3,r10,-29836
	ctx.r3.s64 = ctx.r10.s64 + -29836;
	// bl 0x827d6c90
	ctx.lr = 0x8259E3F4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18224
	ctx.r4.s64 = ctx.r11.s64 + -18224;
	// addi r3,r10,-29852
	ctx.r3.s64 = ctx.r10.s64 + -29852;
	// bl 0x827d6c90
	ctx.lr = 0x8259E408;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18152
	ctx.r4.s64 = ctx.r11.s64 + -18152;
	// addi r3,r10,-29888
	ctx.r3.s64 = ctx.r10.s64 + -29888;
	// bl 0x827d6c90
	ctx.lr = 0x8259E41C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18096
	ctx.r4.s64 = ctx.r11.s64 + -18096;
	// addi r3,r10,-29916
	ctx.r3.s64 = ctx.r10.s64 + -29916;
	// bl 0x827d6c90
	ctx.lr = 0x8259E430;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18056
	ctx.r4.s64 = ctx.r11.s64 + -18056;
	// addi r3,r10,-29932
	ctx.r3.s64 = ctx.r10.s64 + -29932;
	// bl 0x827d6c90
	ctx.lr = 0x8259E444;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18000
	ctx.r4.s64 = ctx.r11.s64 + -18000;
	// addi r3,r10,-29948
	ctx.r3.s64 = ctx.r10.s64 + -29948;
	// bl 0x827d6c90
	ctx.lr = 0x8259E458;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-18000
	ctx.r4.s64 = ctx.r11.s64 + -18000;
	// addi r3,r10,-29972
	ctx.r3.s64 = ctx.r10.s64 + -29972;
	// bl 0x827d6c90
	ctx.lr = 0x8259E46C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-17960
	ctx.r4.s64 = ctx.r11.s64 + -17960;
	// addi r3,r10,-29992
	ctx.r3.s64 = ctx.r10.s64 + -29992;
	// bl 0x827d6c90
	ctx.lr = 0x8259E480;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-17888
	ctx.r4.s64 = ctx.r11.s64 + -17888;
	// addi r3,r10,-30016
	ctx.r3.s64 = ctx.r10.s64 + -30016;
	// bl 0x827d6c90
	ctx.lr = 0x8259E494;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-17816
	ctx.r4.s64 = ctx.r11.s64 + -17816;
	// addi r3,r10,-30040
	ctx.r3.s64 = ctx.r10.s64 + -30040;
	// bl 0x827d6c90
	ctx.lr = 0x8259E4A8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-17744
	ctx.r4.s64 = ctx.r11.s64 + -17744;
	// addi r3,r10,-30056
	ctx.r3.s64 = ctx.r10.s64 + -30056;
	// bl 0x827d6c90
	ctx.lr = 0x8259E4BC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20440
	ctx.r4.s64 = ctx.r11.s64 + -20440;
	// addi r3,r10,-30076
	ctx.r3.s64 = ctx.r10.s64 + -30076;
	// bl 0x827d6c90
	ctx.lr = 0x8259E4D0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-17728
	ctx.r4.s64 = ctx.r11.s64 + -17728;
	// addi r3,r10,-30096
	ctx.r3.s64 = ctx.r10.s64 + -30096;
	// bl 0x827d6c90
	ctx.lr = 0x8259E4E4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-17664
	ctx.r4.s64 = ctx.r11.s64 + -17664;
	// addi r3,r10,-30112
	ctx.r3.s64 = ctx.r10.s64 + -30112;
	// bl 0x827d6c90
	ctx.lr = 0x8259E4F8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-17600
	ctx.r4.s64 = ctx.r11.s64 + -17600;
	// addi r3,r10,-30128
	ctx.r3.s64 = ctx.r10.s64 + -30128;
	// bl 0x827d6c90
	ctx.lr = 0x8259E50C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-17560
	ctx.r4.s64 = ctx.r11.s64 + -17560;
	// addi r3,r10,-30156
	ctx.r3.s64 = ctx.r10.s64 + -30156;
	// bl 0x827d6c90
	ctx.lr = 0x8259E520;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-17528
	ctx.r4.s64 = ctx.r11.s64 + -17528;
	// addi r3,r10,-30180
	ctx.r3.s64 = ctx.r10.s64 + -30180;
	// bl 0x827d6c90
	ctx.lr = 0x8259E534;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11256
	ctx.r4.s64 = ctx.r11.s64 + -11256;
	// addi r3,r10,-30208
	ctx.r3.s64 = ctx.r10.s64 + -30208;
	// bl 0x827d6c90
	ctx.lr = 0x8259E548;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11232
	ctx.r4.s64 = ctx.r11.s64 + -11232;
	// addi r3,r10,-30236
	ctx.r3.s64 = ctx.r10.s64 + -30236;
	// bl 0x827d6c90
	ctx.lr = 0x8259E55C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11200
	ctx.r4.s64 = ctx.r11.s64 + -11200;
	// addi r3,r10,-30264
	ctx.r3.s64 = ctx.r10.s64 + -30264;
	// bl 0x827d6c90
	ctx.lr = 0x8259E570;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-14472
	ctx.r4.s64 = ctx.r11.s64 + -14472;
	// addi r3,r10,-30284
	ctx.r3.s64 = ctx.r10.s64 + -30284;
	// bl 0x827d6c90
	ctx.lr = 0x8259E584;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11168
	ctx.r4.s64 = ctx.r11.s64 + -11168;
	// addi r3,r10,-30308
	ctx.r3.s64 = ctx.r10.s64 + -30308;
	// bl 0x827d6c90
	ctx.lr = 0x8259E598;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11136
	ctx.r4.s64 = ctx.r11.s64 + -11136;
	// addi r3,r10,-30344
	ctx.r3.s64 = ctx.r10.s64 + -30344;
	// bl 0x827d6c90
	ctx.lr = 0x8259E5AC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11112
	ctx.r4.s64 = ctx.r11.s64 + -11112;
	// addi r3,r10,-30384
	ctx.r3.s64 = ctx.r10.s64 + -30384;
	// bl 0x827d6c90
	ctx.lr = 0x8259E5C0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11080
	ctx.r4.s64 = ctx.r11.s64 + -11080;
	// addi r3,r10,-30412
	ctx.r3.s64 = ctx.r10.s64 + -30412;
	// bl 0x827d6c90
	ctx.lr = 0x8259E5D4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11056
	ctx.r4.s64 = ctx.r11.s64 + -11056;
	// addi r3,r10,-30444
	ctx.r3.s64 = ctx.r10.s64 + -30444;
	// bl 0x827d6c90
	ctx.lr = 0x8259E5E8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-14376
	ctx.r4.s64 = ctx.r11.s64 + -14376;
	// addi r3,r10,-30460
	ctx.r3.s64 = ctx.r10.s64 + -30460;
	// bl 0x827d6c90
	ctx.lr = 0x8259E5FC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-17456
	ctx.r4.s64 = ctx.r11.s64 + -17456;
	// addi r3,r10,-30484
	ctx.r3.s64 = ctx.r10.s64 + -30484;
	// bl 0x827d6c90
	ctx.lr = 0x8259E610;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-17440
	ctx.r4.s64 = ctx.r11.s64 + -17440;
	// addi r3,r10,-30504
	ctx.r3.s64 = ctx.r10.s64 + -30504;
	// bl 0x827d6c90
	ctx.lr = 0x8259E624;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11024
	ctx.r4.s64 = ctx.r11.s64 + -11024;
	// addi r3,r10,-30516
	ctx.r3.s64 = ctx.r10.s64 + -30516;
	// bl 0x827d6c90
	ctx.lr = 0x8259E638;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-11000
	ctx.r4.s64 = ctx.r11.s64 + -11000;
	// addi r3,r10,-30544
	ctx.r3.s64 = ctx.r10.s64 + -30544;
	// bl 0x827d6c90
	ctx.lr = 0x8259E64C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-10976
	ctx.r4.s64 = ctx.r11.s64 + -10976;
	// addi r3,r10,-30568
	ctx.r3.s64 = ctx.r10.s64 + -30568;
	// bl 0x827d6c90
	ctx.lr = 0x8259E660;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-10952
	ctx.r4.s64 = ctx.r11.s64 + -10952;
	// addi r3,r10,-30592
	ctx.r3.s64 = ctx.r10.s64 + -30592;
	// bl 0x827d6c90
	ctx.lr = 0x8259E674;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-10928
	ctx.r4.s64 = ctx.r11.s64 + -10928;
	// addi r3,r10,-30624
	ctx.r3.s64 = ctx.r10.s64 + -30624;
	// bl 0x827d6c90
	ctx.lr = 0x8259E688;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20432
	ctx.r4.s64 = ctx.r11.s64 + -20432;
	// addi r3,r10,-30636
	ctx.r3.s64 = ctx.r10.s64 + -30636;
	// bl 0x827d6c90
	ctx.lr = 0x8259E69C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-10904
	ctx.r4.s64 = ctx.r11.s64 + -10904;
	// addi r3,r10,-30656
	ctx.r3.s64 = ctx.r10.s64 + -30656;
	// bl 0x827d6c90
	ctx.lr = 0x8259E6B0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-10880
	ctx.r4.s64 = ctx.r11.s64 + -10880;
	// addi r3,r10,-30688
	ctx.r3.s64 = ctx.r10.s64 + -30688;
	// bl 0x827d6c90
	ctx.lr = 0x8259E6C4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-10856
	ctx.r4.s64 = ctx.r11.s64 + -10856;
	// addi r3,r10,-30720
	ctx.r3.s64 = ctx.r10.s64 + -30720;
	// bl 0x827d6c90
	ctx.lr = 0x8259E6D8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-10832
	ctx.r4.s64 = ctx.r11.s64 + -10832;
	// addi r3,r10,-30760
	ctx.r3.s64 = ctx.r10.s64 + -30760;
	// bl 0x827d6c90
	ctx.lr = 0x8259E6EC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-17368
	ctx.r4.s64 = ctx.r11.s64 + -17368;
	// addi r3,r10,-30792
	ctx.r3.s64 = ctx.r10.s64 + -30792;
	// bl 0x827d6c90
	ctx.lr = 0x8259E700;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// addi r4,r11,-17304
	ctx.r4.s64 = ctx.r11.s64 + -17304;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r3,r11,-30820
	ctx.r3.s64 = ctx.r11.s64 + -30820;
	// bl 0x827d6c90
	ctx.lr = 0x8259E714;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-10808
	ctx.r4.s64 = ctx.r11.s64 + -10808;
	// addi r3,r10,-30848
	ctx.r3.s64 = ctx.r10.s64 + -30848;
	// bl 0x827d6c90
	ctx.lr = 0x8259E728;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-10768
	ctx.r4.s64 = ctx.r11.s64 + -10768;
	// addi r3,r10,-30888
	ctx.r3.s64 = ctx.r10.s64 + -30888;
	// bl 0x827d6c90
	ctx.lr = 0x8259E73C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-10704
	ctx.r4.s64 = ctx.r11.s64 + -10704;
	// addi r3,r10,-30940
	ctx.r3.s64 = ctx.r10.s64 + -30940;
	// bl 0x827d6c90
	ctx.lr = 0x8259E750;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-10632
	ctx.r4.s64 = ctx.r11.s64 + -10632;
	// addi r3,r10,-30992
	ctx.r3.s64 = ctx.r10.s64 + -30992;
	// bl 0x827d6c90
	ctx.lr = 0x8259E764;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-17232
	ctx.r4.s64 = ctx.r11.s64 + -17232;
	// addi r3,r10,-31028
	ctx.r3.s64 = ctx.r10.s64 + -31028;
	// bl 0x827d6c90
	ctx.lr = 0x8259E778;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-17200
	ctx.r4.s64 = ctx.r11.s64 + -17200;
	// addi r3,r10,-31088
	ctx.r3.s64 = ctx.r10.s64 + -31088;
	// bl 0x827d6c90
	ctx.lr = 0x8259E78C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-17096
	ctx.r4.s64 = ctx.r11.s64 + -17096;
	// addi r3,r10,-31124
	ctx.r3.s64 = ctx.r10.s64 + -31124;
	// bl 0x827d6c90
	ctx.lr = 0x8259E7A0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-10560
	ctx.r4.s64 = ctx.r11.s64 + -10560;
	// addi r3,r10,-31148
	ctx.r3.s64 = ctx.r10.s64 + -31148;
	// bl 0x827d6c90
	ctx.lr = 0x8259E7B4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-10544
	ctx.r4.s64 = ctx.r11.s64 + -10544;
	// addi r3,r10,-31188
	ctx.r3.s64 = ctx.r10.s64 + -31188;
	// bl 0x827d6c90
	ctx.lr = 0x8259E7C8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-10528
	ctx.r4.s64 = ctx.r11.s64 + -10528;
	// addi r3,r10,-31208
	ctx.r3.s64 = ctx.r10.s64 + -31208;
	// bl 0x827d6c90
	ctx.lr = 0x8259E7DC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-10432
	ctx.r4.s64 = ctx.r11.s64 + -10432;
	// addi r3,r10,-31232
	ctx.r3.s64 = ctx.r10.s64 + -31232;
	// bl 0x827d6c90
	ctx.lr = 0x8259E7F0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-14280
	ctx.r4.s64 = ctx.r11.s64 + -14280;
	// addi r3,r10,-31260
	ctx.r3.s64 = ctx.r10.s64 + -31260;
	// bl 0x827d6c90
	ctx.lr = 0x8259E804;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-10384
	ctx.r4.s64 = ctx.r11.s64 + -10384;
	// addi r3,r10,-31272
	ctx.r3.s64 = ctx.r10.s64 + -31272;
	// bl 0x827d6c90
	ctx.lr = 0x8259E818;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-17032
	ctx.r4.s64 = ctx.r11.s64 + -17032;
	// addi r3,r10,-31300
	ctx.r3.s64 = ctx.r10.s64 + -31300;
	// bl 0x827d6c90
	ctx.lr = 0x8259E82C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-17008
	ctx.r4.s64 = ctx.r11.s64 + -17008;
	// addi r3,r10,-31324
	ctx.r3.s64 = ctx.r10.s64 + -31324;
	// bl 0x827d6c90
	ctx.lr = 0x8259E840;
	sub_827D6C90(ctx, base);
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// addi r3,r10,-31348
	ctx.r3.s64 = ctx.r10.s64 + -31348;
	// addi r4,r11,-16944
	ctx.r4.s64 = ctx.r11.s64 + -16944;
	// bl 0x827d6c90
	ctx.lr = 0x8259E854;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16880
	ctx.r4.s64 = ctx.r11.s64 + -16880;
	// addi r3,r10,-31368
	ctx.r3.s64 = ctx.r10.s64 + -31368;
	// bl 0x827d6c90
	ctx.lr = 0x8259E868;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16840
	ctx.r4.s64 = ctx.r11.s64 + -16840;
	// addi r3,r10,-31380
	ctx.r3.s64 = ctx.r10.s64 + -31380;
	// bl 0x827d6c90
	ctx.lr = 0x8259E87C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16824
	ctx.r4.s64 = ctx.r11.s64 + -16824;
	// addi r3,r10,-31404
	ctx.r3.s64 = ctx.r10.s64 + -31404;
	// bl 0x827d6c90
	ctx.lr = 0x8259E890;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-10368
	ctx.r4.s64 = ctx.r11.s64 + -10368;
	// addi r3,r10,-31424
	ctx.r3.s64 = ctx.r10.s64 + -31424;
	// bl 0x827d6c90
	ctx.lr = 0x8259E8A4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16808
	ctx.r4.s64 = ctx.r11.s64 + -16808;
	// addi r3,r10,-31448
	ctx.r3.s64 = ctx.r10.s64 + -31448;
	// bl 0x827d6c90
	ctx.lr = 0x8259E8B8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16784
	ctx.r4.s64 = ctx.r11.s64 + -16784;
	// addi r3,r10,-31468
	ctx.r3.s64 = ctx.r10.s64 + -31468;
	// bl 0x827d6c90
	ctx.lr = 0x8259E8CC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-10288
	ctx.r4.s64 = ctx.r11.s64 + -10288;
	// addi r3,r10,-31484
	ctx.r3.s64 = ctx.r10.s64 + -31484;
	// bl 0x827d6c90
	ctx.lr = 0x8259E8E0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-10272
	ctx.r4.s64 = ctx.r11.s64 + -10272;
	// addi r3,r10,-31512
	ctx.r3.s64 = ctx.r10.s64 + -31512;
	// bl 0x827d6c90
	ctx.lr = 0x8259E8F4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-10176
	ctx.r4.s64 = ctx.r11.s64 + -10176;
	// addi r3,r10,-31544
	ctx.r3.s64 = ctx.r10.s64 + -31544;
	// bl 0x827d6c90
	ctx.lr = 0x8259E908;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16744
	ctx.r4.s64 = ctx.r11.s64 + -16744;
	// addi r3,r10,-31564
	ctx.r3.s64 = ctx.r10.s64 + -31564;
	// bl 0x827d6c90
	ctx.lr = 0x8259E91C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16680
	ctx.r4.s64 = ctx.r11.s64 + -16680;
	// addi r3,r10,-31588
	ctx.r3.s64 = ctx.r10.s64 + -31588;
	// bl 0x827d6c90
	ctx.lr = 0x8259E930;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16616
	ctx.r4.s64 = ctx.r11.s64 + -16616;
	// addi r3,r10,-31612
	ctx.r3.s64 = ctx.r10.s64 + -31612;
	// bl 0x827d6c90
	ctx.lr = 0x8259E944;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-8976
	ctx.r4.s64 = ctx.r11.s64 + -8976;
	// addi r3,r10,-31640
	ctx.r3.s64 = ctx.r10.s64 + -31640;
	// bl 0x827d6c90
	ctx.lr = 0x8259E958;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16544
	ctx.r4.s64 = ctx.r11.s64 + -16544;
	// addi r3,r10,-31668
	ctx.r3.s64 = ctx.r10.s64 + -31668;
	// bl 0x827d6c90
	ctx.lr = 0x8259E96C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16480
	ctx.r4.s64 = ctx.r11.s64 + -16480;
	// addi r3,r10,-31692
	ctx.r3.s64 = ctx.r10.s64 + -31692;
	// bl 0x827d6c90
	ctx.lr = 0x8259E980;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16416
	ctx.r4.s64 = ctx.r11.s64 + -16416;
	// addi r3,r10,-31720
	ctx.r3.s64 = ctx.r10.s64 + -31720;
	// bl 0x827d6c90
	ctx.lr = 0x8259E994;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16416
	ctx.r4.s64 = ctx.r11.s64 + -16416;
	// addi r3,r10,-31748
	ctx.r3.s64 = ctx.r10.s64 + -31748;
	// bl 0x827d6c90
	ctx.lr = 0x8259E9A8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16416
	ctx.r4.s64 = ctx.r11.s64 + -16416;
	// addi r3,r10,-31780
	ctx.r3.s64 = ctx.r10.s64 + -31780;
	// bl 0x827d6c90
	ctx.lr = 0x8259E9BC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16416
	ctx.r4.s64 = ctx.r11.s64 + -16416;
	// addi r3,r10,-31812
	ctx.r3.s64 = ctx.r10.s64 + -31812;
	// bl 0x827d6c90
	ctx.lr = 0x8259E9D0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-10080
	ctx.r4.s64 = ctx.r11.s64 + -10080;
	// addi r3,r10,-31832
	ctx.r3.s64 = ctx.r10.s64 + -31832;
	// bl 0x827d6c90
	ctx.lr = 0x8259E9E4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-10064
	ctx.r4.s64 = ctx.r11.s64 + -10064;
	// addi r3,r10,-31852
	ctx.r3.s64 = ctx.r10.s64 + -31852;
	// bl 0x827d6c90
	ctx.lr = 0x8259E9F8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-10048
	ctx.r4.s64 = ctx.r11.s64 + -10048;
	// addi r3,r10,-31872
	ctx.r3.s64 = ctx.r10.s64 + -31872;
	// bl 0x827d6c90
	ctx.lr = 0x8259EA0C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-10032
	ctx.r4.s64 = ctx.r11.s64 + -10032;
	// addi r3,r10,-31892
	ctx.r3.s64 = ctx.r10.s64 + -31892;
	// bl 0x827d6c90
	ctx.lr = 0x8259EA20;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16304
	ctx.r4.s64 = ctx.r11.s64 + -16304;
	// addi r3,r10,-31920
	ctx.r3.s64 = ctx.r10.s64 + -31920;
	// bl 0x827d6c90
	ctx.lr = 0x8259EA34;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-10016
	ctx.r4.s64 = ctx.r11.s64 + -10016;
	// addi r3,r10,-31940
	ctx.r3.s64 = ctx.r10.s64 + -31940;
	// bl 0x827d6c90
	ctx.lr = 0x8259EA48;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-10016
	ctx.r4.s64 = ctx.r11.s64 + -10016;
	// addi r3,r10,-31960
	ctx.r3.s64 = ctx.r10.s64 + -31960;
	// bl 0x827d6c90
	ctx.lr = 0x8259EA5C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-8872
	ctx.r4.s64 = ctx.r11.s64 + -8872;
	// addi r3,r10,-31976
	ctx.r3.s64 = ctx.r10.s64 + -31976;
	// bl 0x827d6c90
	ctx.lr = 0x8259EA70;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-9992
	ctx.r4.s64 = ctx.r11.s64 + -9992;
	// addi r3,r10,-32000
	ctx.r3.s64 = ctx.r10.s64 + -32000;
	// bl 0x827d6c90
	ctx.lr = 0x8259EA84;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16288
	ctx.r4.s64 = ctx.r11.s64 + -16288;
	// addi r3,r10,-32016
	ctx.r3.s64 = ctx.r10.s64 + -32016;
	// bl 0x827d6c90
	ctx.lr = 0x8259EA98;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-14248
	ctx.r4.s64 = ctx.r11.s64 + -14248;
	// addi r3,r10,-32032
	ctx.r3.s64 = ctx.r10.s64 + -32032;
	// bl 0x827d6c90
	ctx.lr = 0x8259EAAC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-9968
	ctx.r4.s64 = ctx.r11.s64 + -9968;
	// addi r3,r10,-32064
	ctx.r3.s64 = ctx.r10.s64 + -32064;
	// bl 0x827d6c90
	ctx.lr = 0x8259EAC0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-9872
	ctx.r4.s64 = ctx.r11.s64 + -9872;
	// addi r3,r10,-32092
	ctx.r3.s64 = ctx.r10.s64 + -32092;
	// bl 0x827d6c90
	ctx.lr = 0x8259EAD4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16224
	ctx.r4.s64 = ctx.r11.s64 + -16224;
	// addi r3,r10,-32112
	ctx.r3.s64 = ctx.r10.s64 + -32112;
	// bl 0x827d6c90
	ctx.lr = 0x8259EAE8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-9776
	ctx.r4.s64 = ctx.r11.s64 + -9776;
	// addi r3,r10,-32128
	ctx.r3.s64 = ctx.r10.s64 + -32128;
	// bl 0x827d6c90
	ctx.lr = 0x8259EAFC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16200
	ctx.r4.s64 = ctx.r11.s64 + -16200;
	// addi r3,r10,-32148
	ctx.r3.s64 = ctx.r10.s64 + -32148;
	// bl 0x827d6c90
	ctx.lr = 0x8259EB10;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-9760
	ctx.r4.s64 = ctx.r11.s64 + -9760;
	// addi r3,r10,-32164
	ctx.r3.s64 = ctx.r10.s64 + -32164;
	// bl 0x827d6c90
	ctx.lr = 0x8259EB24;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16176
	ctx.r4.s64 = ctx.r11.s64 + -16176;
	// addi r3,r10,-32184
	ctx.r3.s64 = ctx.r10.s64 + -32184;
	// bl 0x827d6c90
	ctx.lr = 0x8259EB38;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16112
	ctx.r4.s64 = ctx.r11.s64 + -16112;
	// addi r3,r10,-32216
	ctx.r3.s64 = ctx.r10.s64 + -32216;
	// bl 0x827d6c90
	ctx.lr = 0x8259EB4C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16072
	ctx.r4.s64 = ctx.r11.s64 + -16072;
	// addi r3,r10,-32228
	ctx.r3.s64 = ctx.r10.s64 + -32228;
	// bl 0x827d6c90
	ctx.lr = 0x8259EB60;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-16032
	ctx.r4.s64 = ctx.r11.s64 + -16032;
	// addi r3,r10,-32244
	ctx.r3.s64 = ctx.r10.s64 + -32244;
	// bl 0x827d6c90
	ctx.lr = 0x8259EB74;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15992
	ctx.r4.s64 = ctx.r11.s64 + -15992;
	// addi r3,r10,-32268
	ctx.r3.s64 = ctx.r10.s64 + -32268;
	// bl 0x827d6c90
	ctx.lr = 0x8259EB88;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15952
	ctx.r4.s64 = ctx.r11.s64 + -15952;
	// addi r3,r10,-32288
	ctx.r3.s64 = ctx.r10.s64 + -32288;
	// bl 0x827d6c90
	ctx.lr = 0x8259EB9C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15928
	ctx.r4.s64 = ctx.r11.s64 + -15928;
	// addi r3,r10,-32312
	ctx.r3.s64 = ctx.r10.s64 + -32312;
	// bl 0x827d6c90
	ctx.lr = 0x8259EBB0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15896
	ctx.r4.s64 = ctx.r11.s64 + -15896;
	// addi r3,r10,-32332
	ctx.r3.s64 = ctx.r10.s64 + -32332;
	// bl 0x827d6c90
	ctx.lr = 0x8259EBC4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15872
	ctx.r4.s64 = ctx.r11.s64 + -15872;
	// addi r3,r10,-32352
	ctx.r3.s64 = ctx.r10.s64 + -32352;
	// bl 0x827d6c90
	ctx.lr = 0x8259EBD8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15848
	ctx.r4.s64 = ctx.r11.s64 + -15848;
	// addi r3,r10,-32364
	ctx.r3.s64 = ctx.r10.s64 + -32364;
	// bl 0x827d6c90
	ctx.lr = 0x8259EBEC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15744
	ctx.r4.s64 = ctx.r11.s64 + -15744;
	// addi r3,r10,-32388
	ctx.r3.s64 = ctx.r10.s64 + -32388;
	// bl 0x827d6c90
	ctx.lr = 0x8259EC00;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15704
	ctx.r4.s64 = ctx.r11.s64 + -15704;
	// addi r3,r10,-32404
	ctx.r3.s64 = ctx.r10.s64 + -32404;
	// bl 0x827d6c90
	ctx.lr = 0x8259EC14;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20328
	ctx.r4.s64 = ctx.r11.s64 + -20328;
	// addi r3,r10,-32424
	ctx.r3.s64 = ctx.r10.s64 + -32424;
	// bl 0x827d6c90
	ctx.lr = 0x8259EC28;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15640
	ctx.r4.s64 = ctx.r11.s64 + -15640;
	// addi r3,r10,-32436
	ctx.r3.s64 = ctx.r10.s64 + -32436;
	// bl 0x827d6c90
	ctx.lr = 0x8259EC3C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15616
	ctx.r4.s64 = ctx.r11.s64 + -15616;
	// addi r3,r10,-32464
	ctx.r3.s64 = ctx.r10.s64 + -32464;
	// bl 0x827d6c90
	ctx.lr = 0x8259EC50;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-9744
	ctx.r4.s64 = ctx.r11.s64 + -9744;
	// addi r3,r10,-32500
	ctx.r3.s64 = ctx.r10.s64 + -32500;
	// bl 0x827d6c90
	ctx.lr = 0x8259EC64;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-9600
	ctx.r4.s64 = ctx.r11.s64 + -9600;
	// addi r3,r10,-32524
	ctx.r3.s64 = ctx.r10.s64 + -32524;
	// bl 0x827d6c90
	ctx.lr = 0x8259EC78;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-9496
	ctx.r4.s64 = ctx.r11.s64 + -9496;
	// addi r3,r10,-32556
	ctx.r3.s64 = ctx.r10.s64 + -32556;
	// bl 0x827d6c90
	ctx.lr = 0x8259EC8C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-15592
	ctx.r4.s64 = ctx.r11.s64 + -15592;
	// addi r3,r10,-32580
	ctx.r3.s64 = ctx.r10.s64 + -32580;
	// bl 0x827d6c90
	ctx.lr = 0x8259ECA0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20272
	ctx.r4.s64 = ctx.r11.s64 + -20272;
	// addi r3,r10,-32620
	ctx.r3.s64 = ctx.r10.s64 + -32620;
	// bl 0x827d6c90
	ctx.lr = 0x8259ECB4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20256
	ctx.r4.s64 = ctx.r11.s64 + -20256;
	// addi r3,r10,-32668
	ctx.r3.s64 = ctx.r10.s64 + -32668;
	// bl 0x827d6c90
	ctx.lr = 0x8259ECC8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-20240
	ctx.r4.s64 = ctx.r11.s64 + -20240;
	// addi r3,r10,-32704
	ctx.r3.s64 = ctx.r10.s64 + -32704;
	// bl 0x827d6c90
	ctx.lr = 0x8259ECDC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-9472
	ctx.r4.s64 = ctx.r11.s64 + -9472;
	// addi r3,r10,-32748
	ctx.r3.s64 = ctx.r10.s64 + -32748;
	// bl 0x827d6c90
	ctx.lr = 0x8259ECF0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-9344
	ctx.r4.s64 = ctx.r11.s64 + -9344;
	// addi r3,r10,32764
	ctx.r3.s64 = ctx.r10.s64 + 32764;
	// bl 0x827d6c90
	ctx.lr = 0x8259ED04;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-9240
	ctx.r4.s64 = ctx.r11.s64 + -9240;
	// addi r3,r10,32716
	ctx.r3.s64 = ctx.r10.s64 + 32716;
	// bl 0x827d6c90
	ctx.lr = 0x8259ED18;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-15552
	ctx.r4.s64 = ctx.r11.s64 + -15552;
	// addi r3,r10,32692
	ctx.r3.s64 = ctx.r10.s64 + 32692;
	// bl 0x827d6c90
	ctx.lr = 0x8259ED2C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-15528
	ctx.r4.s64 = ctx.r11.s64 + -15528;
	// addi r3,r10,32672
	ctx.r3.s64 = ctx.r10.s64 + 32672;
	// bl 0x827d6c90
	ctx.lr = 0x8259ED40;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-15480
	ctx.r4.s64 = ctx.r11.s64 + -15480;
	// addi r3,r10,32656
	ctx.r3.s64 = ctx.r10.s64 + 32656;
	// bl 0x827d6c90
	ctx.lr = 0x8259ED54;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// addi r4,r11,-15448
	ctx.r4.s64 = ctx.r11.s64 + -15448;
	// lis r11,-32253
	ctx.r11.s64 = -2113732608;
	// addi r3,r11,32640
	ctx.r3.s64 = ctx.r11.s64 + 32640;
	// bl 0x827d6c90
	ctx.lr = 0x8259ED68;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-20224
	ctx.r4.s64 = ctx.r11.s64 + -20224;
	// addi r3,r10,32608
	ctx.r3.s64 = ctx.r10.s64 + 32608;
	// bl 0x827d6c90
	ctx.lr = 0x8259ED7C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-15416
	ctx.r4.s64 = ctx.r11.s64 + -15416;
	// addi r3,r10,32580
	ctx.r3.s64 = ctx.r10.s64 + 32580;
	// bl 0x827d6c90
	ctx.lr = 0x8259ED90;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-20208
	ctx.r4.s64 = ctx.r11.s64 + -20208;
	// addi r3,r10,32548
	ctx.r3.s64 = ctx.r10.s64 + 32548;
	// bl 0x827d6c90
	ctx.lr = 0x8259EDA4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-9200
	ctx.r4.s64 = ctx.r11.s64 + -9200;
	// addi r3,r10,32520
	ctx.r3.s64 = ctx.r10.s64 + 32520;
	// bl 0x827d6c90
	ctx.lr = 0x8259EDB8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-15368
	ctx.r4.s64 = ctx.r11.s64 + -15368;
	// addi r3,r10,32492
	ctx.r3.s64 = ctx.r10.s64 + 32492;
	// bl 0x827d6c90
	ctx.lr = 0x8259EDCC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-15344
	ctx.r4.s64 = ctx.r11.s64 + -15344;
	// addi r3,r10,32468
	ctx.r3.s64 = ctx.r10.s64 + 32468;
	// bl 0x827d6c90
	ctx.lr = 0x8259EDE0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-20192
	ctx.r4.s64 = ctx.r11.s64 + -20192;
	// addi r3,r10,32440
	ctx.r3.s64 = ctx.r10.s64 + 32440;
	// bl 0x827d6c90
	ctx.lr = 0x8259EDF4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-9184
	ctx.r4.s64 = ctx.r11.s64 + -9184;
	// addi r3,r10,32408
	ctx.r3.s64 = ctx.r10.s64 + 32408;
	// bl 0x827d6c90
	ctx.lr = 0x8259EE08;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-15304
	ctx.r4.s64 = ctx.r11.s64 + -15304;
	// addi r3,r10,32384
	ctx.r3.s64 = ctx.r10.s64 + 32384;
	// bl 0x827d6c90
	ctx.lr = 0x8259EE1C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-8768
	ctx.r4.s64 = ctx.r11.s64 + -8768;
	// addi r3,r10,32356
	ctx.r3.s64 = ctx.r10.s64 + 32356;
	// bl 0x827d6c90
	ctx.lr = 0x8259EE30;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-15288
	ctx.r4.s64 = ctx.r11.s64 + -15288;
	// addi r3,r10,32336
	ctx.r3.s64 = ctx.r10.s64 + 32336;
	// bl 0x827d6c90
	ctx.lr = 0x8259EE44;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-9088
	ctx.r4.s64 = ctx.r11.s64 + -9088;
	// addi r3,r10,32312
	ctx.r3.s64 = ctx.r10.s64 + 32312;
	// bl 0x827d6c90
	ctx.lr = 0x8259EE58;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-15256
	ctx.r4.s64 = ctx.r11.s64 + -15256;
	// addi r3,r10,32272
	ctx.r3.s64 = ctx.r10.s64 + 32272;
	// bl 0x827d6c90
	ctx.lr = 0x8259EE6C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-15200
	ctx.r4.s64 = ctx.r11.s64 + -15200;
	// addi r3,r10,32228
	ctx.r3.s64 = ctx.r10.s64 + 32228;
	// bl 0x827d6c90
	ctx.lr = 0x8259EE80;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,32196
	ctx.r3.s64 = ctx.r10.s64 + 32196;
	// bl 0x827d6c90
	ctx.lr = 0x8259EE94;
	sub_827D6C90(ctx, base);
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// addi r3,r10,32168
	ctx.r3.s64 = ctx.r10.s64 + 32168;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// bl 0x827d6c90
	ctx.lr = 0x8259EEA8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-20168
	ctx.r4.s64 = ctx.r11.s64 + -20168;
	// addi r3,r10,32140
	ctx.r3.s64 = ctx.r10.s64 + 32140;
	// bl 0x827d6c90
	ctx.lr = 0x8259EEBC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-9072
	ctx.r4.s64 = ctx.r11.s64 + -9072;
	// addi r3,r10,32112
	ctx.r3.s64 = ctx.r10.s64 + 32112;
	// bl 0x827d6c90
	ctx.lr = 0x8259EED0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-15144
	ctx.r4.s64 = ctx.r11.s64 + -15144;
	// addi r3,r10,32088
	ctx.r3.s64 = ctx.r10.s64 + 32088;
	// bl 0x827d6c90
	ctx.lr = 0x8259EEE4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-8992
	ctx.r4.s64 = ctx.r11.s64 + -8992;
	// addi r3,r10,32064
	ctx.r3.s64 = ctx.r10.s64 + 32064;
	// bl 0x827d6c90
	ctx.lr = 0x8259EEF8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-15120
	ctx.r4.s64 = ctx.r11.s64 + -15120;
	// addi r3,r10,32036
	ctx.r3.s64 = ctx.r10.s64 + 32036;
	// bl 0x827d6c90
	ctx.lr = 0x8259EF0C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-15096
	ctx.r4.s64 = ctx.r11.s64 + -15096;
	// addi r3,r10,32008
	ctx.r3.s64 = ctx.r10.s64 + 32008;
	// bl 0x827d6c90
	ctx.lr = 0x8259EF20;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-15064
	ctx.r4.s64 = ctx.r11.s64 + -15064;
	// addi r3,r10,31980
	ctx.r3.s64 = ctx.r10.s64 + 31980;
	// bl 0x827d6c90
	ctx.lr = 0x8259EF34;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-15040
	ctx.r4.s64 = ctx.r11.s64 + -15040;
	// addi r3,r10,31952
	ctx.r3.s64 = ctx.r10.s64 + 31952;
	// bl 0x827d6c90
	ctx.lr = 0x8259EF48;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-15016
	ctx.r4.s64 = ctx.r11.s64 + -15016;
	// addi r3,r10,31924
	ctx.r3.s64 = ctx.r10.s64 + 31924;
	// bl 0x827d6c90
	ctx.lr = 0x8259EF5C;
	sub_827D6C90(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259EF70);  /* forward decl */
PPC_WEAK_FUNC(sub_8259EF70) { __imp__sub_8259EF70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259EF70) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r3,0
	ctx.r3.s64 = 0;
	// lfs f0,3068(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3068);
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

PPC_FUNC_IMPL(__imp__sub_8259EF90);  /* forward decl */
PPC_WEAK_FUNC(sub_8259EF90) { __imp__sub_8259EF90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259EF90) {
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

PPC_FUNC_IMPL(__imp__sub_8259EFA0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259EFA0) { __imp__sub_8259EFA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259EFA0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32056
	ctx.r11.s64 = -2100822016;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,-7544(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -7544);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259EFB8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259EFB8) { __imp__sub_8259EFB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259EFB8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32092
	ctx.r11.s64 = -2103181312;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,-22380(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -22380);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259EFD0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259EFD0) { __imp__sub_8259EFD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259EFD0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32092
	ctx.r11.s64 = -2103181312;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,-22376(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -22376);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259EFE8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259EFE8) { __imp__sub_8259EFE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259EFE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32092
	ctx.r11.s64 = -2103181312;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// addi r11,r11,-22372
	ctx.r11.s64 = ctx.r11.s64 + -22372;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259F000);  /* forward decl */
PPC_WEAK_FUNC(sub_8259F000) { __imp__sub_8259F000(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259F000) {
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
	ctx.lr = 0x8259F084;
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

PPC_FUNC_IMPL(__imp__sub_8259F0A8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259F0A8) { __imp__sub_8259F0A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259F0A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,-4240
	ctx.r3.s64 = ctx.r11.s64 + -4240;
	// b 0x8259f000
	sub_8259F000(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259F0B8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259F0B8) { __imp__sub_8259F0B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259F0B8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,-26964
	ctx.r3.s64 = ctx.r10.s64 + -26964;
	// bl 0x827d6c90
	ctx.lr = 0x8259F0D8;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,-26976
	ctx.r3.s64 = ctx.r10.s64 + -26976;
	// bl 0x827d6c90
	ctx.lr = 0x8259F0EC;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,-27000
	ctx.r3.s64 = ctx.r10.s64 + -27000;
	// bl 0x827d6c90
	ctx.lr = 0x8259F100;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,-27016
	ctx.r3.s64 = ctx.r10.s64 + -27016;
	// bl 0x827d6c90
	ctx.lr = 0x8259F114;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,-27032
	ctx.r3.s64 = ctx.r10.s64 + -27032;
	// bl 0x827d6c90
	ctx.lr = 0x8259F128;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,-27052
	ctx.r3.s64 = ctx.r10.s64 + -27052;
	// bl 0x827d6c90
	ctx.lr = 0x8259F13C;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,-27076
	ctx.r3.s64 = ctx.r10.s64 + -27076;
	// bl 0x827d6c90
	ctx.lr = 0x8259F150;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,-27104
	ctx.r3.s64 = ctx.r10.s64 + -27104;
	// bl 0x827d6c90
	ctx.lr = 0x8259F164;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,-27132
	ctx.r3.s64 = ctx.r10.s64 + -27132;
	// bl 0x827d6c90
	ctx.lr = 0x8259F178;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,-27160
	ctx.r3.s64 = ctx.r10.s64 + -27160;
	// bl 0x827d6c90
	ctx.lr = 0x8259F18C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-4168
	ctx.r4.s64 = ctx.r11.s64 + -4168;
	// addi r3,r10,-27200
	ctx.r3.s64 = ctx.r10.s64 + -27200;
	// bl 0x827d6c90
	ctx.lr = 0x8259F1A0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-4168
	ctx.r4.s64 = ctx.r11.s64 + -4168;
	// addi r3,r10,-27228
	ctx.r3.s64 = ctx.r10.s64 + -27228;
	// bl 0x827d6c90
	ctx.lr = 0x8259F1B4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-4168
	ctx.r4.s64 = ctx.r11.s64 + -4168;
	// addi r3,r10,-27260
	ctx.r3.s64 = ctx.r10.s64 + -27260;
	// bl 0x827d6c90
	ctx.lr = 0x8259F1C8;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,-27280
	ctx.r3.s64 = ctx.r10.s64 + -27280;
	// bl 0x827d6c90
	ctx.lr = 0x8259F1DC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-4208
	ctx.r4.s64 = ctx.r11.s64 + -4208;
	// addi r3,r10,-27300
	ctx.r3.s64 = ctx.r10.s64 + -27300;
	// bl 0x827d6c90
	ctx.lr = 0x8259F1F0;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,-27320
	ctx.r3.s64 = ctx.r10.s64 + -27320;
	// bl 0x827d6c90
	ctx.lr = 0x8259F204;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-4208
	ctx.r4.s64 = ctx.r11.s64 + -4208;
	// addi r3,r10,-27340
	ctx.r3.s64 = ctx.r10.s64 + -27340;
	// bl 0x827d6c90
	ctx.lr = 0x8259F218;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-4208
	ctx.r4.s64 = ctx.r11.s64 + -4208;
	// addi r3,r10,-27364
	ctx.r3.s64 = ctx.r10.s64 + -27364;
	// bl 0x827d6c90
	ctx.lr = 0x8259F22C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-4208
	ctx.r4.s64 = ctx.r11.s64 + -4208;
	// addi r3,r10,-27388
	ctx.r3.s64 = ctx.r10.s64 + -27388;
	// bl 0x827d6c90
	ctx.lr = 0x8259F240;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-4208
	ctx.r4.s64 = ctx.r11.s64 + -4208;
	// addi r3,r10,-27416
	ctx.r3.s64 = ctx.r10.s64 + -27416;
	// bl 0x827d6c90
	ctx.lr = 0x8259F254;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-4208
	ctx.r4.s64 = ctx.r11.s64 + -4208;
	// addi r3,r10,-27436
	ctx.r3.s64 = ctx.r10.s64 + -27436;
	// bl 0x827d6c90
	ctx.lr = 0x8259F268;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-4208
	ctx.r4.s64 = ctx.r11.s64 + -4208;
	// addi r3,r10,-27456
	ctx.r3.s64 = ctx.r10.s64 + -27456;
	// bl 0x827d6c90
	ctx.lr = 0x8259F27C;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,-27476
	ctx.r3.s64 = ctx.r10.s64 + -27476;
	// bl 0x827d6c90
	ctx.lr = 0x8259F290;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,-27492
	ctx.r3.s64 = ctx.r10.s64 + -27492;
	// bl 0x827d6c90
	ctx.lr = 0x8259F2A4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-4208
	ctx.r4.s64 = ctx.r11.s64 + -4208;
	// addi r3,r10,-27516
	ctx.r3.s64 = ctx.r10.s64 + -27516;
	// bl 0x827d6c90
	ctx.lr = 0x8259F2B8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-4208
	ctx.r4.s64 = ctx.r11.s64 + -4208;
	// addi r3,r10,-27536
	ctx.r3.s64 = ctx.r10.s64 + -27536;
	// bl 0x827d6c90
	ctx.lr = 0x8259F2CC;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,-27560
	ctx.r3.s64 = ctx.r10.s64 + -27560;
	// bl 0x827d6c90
	ctx.lr = 0x8259F2E0;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,-27584
	ctx.r3.s64 = ctx.r10.s64 + -27584;
	// bl 0x827d6c90
	ctx.lr = 0x8259F2F4;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,-27604
	ctx.r3.s64 = ctx.r10.s64 + -27604;
	// bl 0x827d6c90
	ctx.lr = 0x8259F308;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-4208
	ctx.r4.s64 = ctx.r11.s64 + -4208;
	// addi r3,r10,-27624
	ctx.r3.s64 = ctx.r10.s64 + -27624;
	// bl 0x827d6c90
	ctx.lr = 0x8259F31C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-4208
	ctx.r4.s64 = ctx.r11.s64 + -4208;
	// addi r3,r10,-27640
	ctx.r3.s64 = ctx.r10.s64 + -27640;
	// bl 0x827d6c90
	ctx.lr = 0x8259F330;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-4144
	ctx.r4.s64 = ctx.r11.s64 + -4144;
	// addi r3,r10,-27668
	ctx.r3.s64 = ctx.r10.s64 + -27668;
	// bl 0x827d6c90
	ctx.lr = 0x8259F344;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r3,r11,-27696
	ctx.r3.s64 = ctx.r11.s64 + -27696;
	// bl 0x827d6c90
	ctx.lr = 0x8259F358;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,-27716
	ctx.r3.s64 = ctx.r10.s64 + -27716;
	// bl 0x827d6c90
	ctx.lr = 0x8259F36C;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,-27744
	ctx.r3.s64 = ctx.r10.s64 + -27744;
	// bl 0x827d6c90
	ctx.lr = 0x8259F380;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,-27776
	ctx.r3.s64 = ctx.r10.s64 + -27776;
	// bl 0x827d6c90
	ctx.lr = 0x8259F394;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-3928
	ctx.r4.s64 = ctx.r11.s64 + -3928;
	// addi r3,r10,-27808
	ctx.r3.s64 = ctx.r10.s64 + -27808;
	// bl 0x827d6c90
	ctx.lr = 0x8259F3A8;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,-27828
	ctx.r3.s64 = ctx.r10.s64 + -27828;
	// bl 0x827d6c90
	ctx.lr = 0x8259F3BC;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,-27868
	ctx.r3.s64 = ctx.r10.s64 + -27868;
	// bl 0x827d6c90
	ctx.lr = 0x8259F3D0;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,-27896
	ctx.r3.s64 = ctx.r10.s64 + -27896;
	// bl 0x827d6c90
	ctx.lr = 0x8259F3E4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-4120
	ctx.r4.s64 = ctx.r11.s64 + -4120;
	// addi r3,r10,-27940
	ctx.r3.s64 = ctx.r10.s64 + -27940;
	// bl 0x827d6c90
	ctx.lr = 0x8259F3F8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-4192
	ctx.r4.s64 = ctx.r11.s64 + -4192;
	// addi r3,r10,-27956
	ctx.r3.s64 = ctx.r10.s64 + -27956;
	// bl 0x827d6c90
	ctx.lr = 0x8259F40C;
	sub_827D6C90(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259F420);  /* forward decl */
PPC_WEAK_FUNC(sub_8259F420) { __imp__sub_8259F420(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259F420) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x8259F428;
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stfs f1,80(r1)
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// stfs f2,84(r1)
	temp.f32 = float(ctx.f2.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
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
	// bl 0x824b5278
	ctx.lr = 0x8259F460;
	sub_824B5278(ctx, base);
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
	// lfs f0,2980(r11)
	ctx.fpscr.disableFlushModeUnconditional();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2980);
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
	// bl 0x824aee88
	ctx.lr = 0x8259F4B4;
	sub_824AEE88(ctx, base);
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// fadds f1,f31,f30
	ctx.fpscr.disableFlushMode();
	ctx.f1.f64 = double(float(ctx.f31.f64 + ctx.f30.f64));
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824aeef0
	ctx.lr = 0x8259F4C8;
	sub_824AEEF0(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x8259F4D8;
	sub_824B26C8(ctx, base);
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x8259F4E4;
	sub_824B26C8(ctx, base);
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// lfd f30,-48(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -48);
	// lfd f31,-40(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -40);
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259F4F8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259F4F8) { __imp__sub_8259F4F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259F4F8) {
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
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r30,r11,26488
	ctx.r30.s64 = ctx.r11.s64 + 26488;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x824af3e8
	ctx.lr = 0x8259F524;
	sub_824AF3E8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x824b26c8
	ctx.lr = 0x8259F530;
	sub_824B26C8(ctx, base);
	// lis r11,-32039
	ctx.r11.s64 = -2099707904;
	// lwz r30,15072(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 15072);
	// bl 0x821d2088
	ctx.lr = 0x8259F53C;
	sub_821D2088(ctx, base);
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
	// bl 0x823086d8
	ctx.lr = 0x8259F554;
	sub_823086D8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259F570);  /* forward decl */
PPC_WEAK_FUNC(sub_8259F570) { __imp__sub_8259F570(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259F570) {
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
	// beq cr6,0x8259f5c8
	if (ctx.cr6.eq) goto loc_8259F5C8;
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
loc_8259F5C8:
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
	// lwz r9,212(r1)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 212);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// mr r7,r6
	ctx.r7.u64 = ctx.r6.u64;
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
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
	// bl 0x824b25b8
	ctx.lr = 0x8259F5F8;
	sub_824B25B8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x8259F604;
	sub_824B26C8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259F618);  /* forward decl */
PPC_WEAK_FUNC(sub_8259F618) { __imp__sub_8259F618(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259F618) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stfs f1,96(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r1.u32 + 96, temp.u32);
	// mr r5,r6
	ctx.r5.u64 = ctx.r6.u64;
	// stfs f2,100(r1)
	temp.f32 = float(ctx.f2.f64);
	PPC_STORE_U32(ctx.r1.u32 + 100, temp.u32);
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
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
	// bl 0x824b37a8
	ctx.lr = 0x8259F658;
	sub_824B37A8(ctx, base);
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
	// lfs f13,3068(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3068);
	ctx.f13.f64 = double(temp.f32);
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// blt cr6,0x8259f69c
	if (ctx.cr6.lt) goto loc_8259F69C;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f11,2592(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2592);
	ctx.f11.f64 = double(temp.f32);
	// fcmpu cr6,f0,f11
	ctx.cr6.compare(ctx.f0.f64, ctx.f11.f64);
	// bgt cr6,0x8259f69c
	if (ctx.cr6.gt) goto loc_8259F69C;
	// fcmpu cr6,f12,f13
	ctx.cr6.compare(ctx.f12.f64, ctx.f13.f64);
	// blt cr6,0x8259f69c
	if (ctx.cr6.lt) goto loc_8259F69C;
	// fcmpu cr6,f12,f11
	ctx.cr6.compare(ctx.f12.f64, ctx.f11.f64);
	// li r11,1
	ctx.r11.s64 = 1;
	// ble cr6,0x8259f6a0
	if (!ctx.cr6.gt) goto loc_8259F6A0;
loc_8259F69C:
	// li r11,0
	ctx.r11.s64 = 0;
loc_8259F6A0:
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

PPC_FUNC_IMPL(__imp__sub_8259F6C0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259F6C0) { __imp__sub_8259F6C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259F6C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32039
	ctx.r11.s64 = -2099707904;
	// lwz r31,15072(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 15072);
	// bl 0x821d2088
	ctx.lr = 0x8259F6DC;
	sub_821D2088(ctx, base);
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
	// li r4,21
	ctx.r4.s64 = 21;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82308b30
	ctx.lr = 0x8259F6EC;
	sub_82308B30(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259F700);  /* forward decl */
PPC_WEAK_FUNC(sub_8259F700) { __imp__sub_8259F700(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259F700) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b1978
	ctx.lr = 0x8259F720;
	sub_824B1978(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x8259F72C;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259F740);  /* forward decl */
PPC_WEAK_FUNC(sub_8259F740) { __imp__sub_8259F740(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259F740) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// b 0x824b27f0
	sub_824B27F0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259F750);  /* forward decl */
PPC_WEAK_FUNC(sub_8259F750) { __imp__sub_8259F750(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259F750) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32039
	ctx.r11.s64 = -2099707904;
	// lwz r31,15072(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 15072);
	// bl 0x821d2088
	ctx.lr = 0x8259F76C;
	sub_821D2088(ctx, base);
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
	// li r4,22
	ctx.r4.s64 = 22;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82308b30
	ctx.lr = 0x8259F77C;
	sub_82308B30(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259F790);  /* forward decl */
PPC_WEAK_FUNC(sub_8259F790) { __imp__sub_8259F790(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259F790) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b1d68
	ctx.lr = 0x8259F7B0;
	sub_824B1D68(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x8259F7BC;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259F7D0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259F7D0) { __imp__sub_8259F7D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259F7D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b1f40
	ctx.lr = 0x8259F7F0;
	sub_824B1F40(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x8259F7FC;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259F810);  /* forward decl */
PPC_WEAK_FUNC(sub_8259F810) { __imp__sub_8259F810(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259F810) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b2030
	ctx.lr = 0x8259F830;
	sub_824B2030(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x8259F83C;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259F850);  /* forward decl */
PPC_WEAK_FUNC(sub_8259F850) { __imp__sub_8259F850(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259F850) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b0348
	ctx.lr = 0x8259F870;
	sub_824B0348(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x8259F87C;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259F890);  /* forward decl */
PPC_WEAK_FUNC(sub_8259F890) { __imp__sub_8259F890(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259F890) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b03a0
	ctx.lr = 0x8259F8B0;
	sub_824B03A0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x8259F8BC;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259F8D0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259F8D0) { __imp__sub_8259F8D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259F8D0) {
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
	ctx.lr = 0x8259F934;
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

PPC_FUNC_IMPL(__imp__sub_8259F948);  /* forward decl */
PPC_WEAK_FUNC(sub_8259F948) { __imp__sub_8259F948(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259F948) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// srawi r10,r4,8
	ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r4.s32 >> 8;
	// clrlwi r8,r4,24
	ctx.r8.u64 = ctx.r4.u32 & 0xFF;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x8259f98c
	if (!ctx.cr6.eq) goto loc_8259F98C;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r5,r11,r9
	ctx.r5.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x8259f990
	goto loc_8259F990;
loc_8259F98C:
	// li r5,0
	ctx.r5.s64 = 0;
loc_8259F990:
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824af240
	ctx.lr = 0x8259F9A4;
	sub_824AF240(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x8259F9B0;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259F9C8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259F9C8) { __imp__sub_8259F9C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259F9C8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// srawi r10,r4,8
	ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r4.s32 >> 8;
	// clrlwi r8,r4,24
	ctx.r8.u64 = ctx.r4.u32 & 0xFF;
	// lwz r11,24544(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24544);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x8259fa0c
	if (!ctx.cr6.eq) goto loc_8259FA0C;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r5,r11,r9
	ctx.r5.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x8259fa10
	goto loc_8259FA10;
loc_8259FA0C:
	// li r5,0
	ctx.r5.s64 = 0;
loc_8259FA10:
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824af240
	ctx.lr = 0x8259FA24;
	sub_824AF240(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x8259FA30;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259FA48);  /* forward decl */
PPC_WEAK_FUNC(sub_8259FA48) { __imp__sub_8259FA48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259FA48) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// srawi r10,r4,8
	ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r4.s32 >> 8;
	// clrlwi r8,r4,24
	ctx.r8.u64 = ctx.r4.u32 & 0xFF;
	// lwz r11,88(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 88);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x8259fa8c
	if (!ctx.cr6.eq) goto loc_8259FA8C;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r5,r11,r9
	ctx.r5.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x8259fa90
	goto loc_8259FA90;
loc_8259FA8C:
	// li r5,0
	ctx.r5.s64 = 0;
loc_8259FA90:
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824af240
	ctx.lr = 0x8259FAA4;
	sub_824AF240(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x8259FAB0;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259FAC8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259FAC8) { __imp__sub_8259FAC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259FAC8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// srawi r10,r4,8
	ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r4.s32 >> 8;
	// clrlwi r8,r4,24
	ctx.r8.u64 = ctx.r4.u32 & 0xFF;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x8259fb0c
	if (!ctx.cr6.eq) goto loc_8259FB0C;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r5,r11,r9
	ctx.r5.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x8259fb10
	goto loc_8259FB10;
loc_8259FB0C:
	// li r5,0
	ctx.r5.s64 = 0;
loc_8259FB10:
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824af030
	ctx.lr = 0x8259FB24;
	sub_824AF030(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x8259FB30;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259FB48);  /* forward decl */
PPC_WEAK_FUNC(sub_8259FB48) { __imp__sub_8259FB48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259FB48) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// srawi r10,r4,8
	ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r4.s32 >> 8;
	// clrlwi r8,r4,24
	ctx.r8.u64 = ctx.r4.u32 & 0xFF;
	// lwz r11,24544(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24544);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x8259fb8c
	if (!ctx.cr6.eq) goto loc_8259FB8C;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r5,r11,r9
	ctx.r5.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x8259fb90
	goto loc_8259FB90;
loc_8259FB8C:
	// li r5,0
	ctx.r5.s64 = 0;
loc_8259FB90:
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824af030
	ctx.lr = 0x8259FBA4;
	sub_824AF030(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x8259FBB0;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259FBC8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259FBC8) { __imp__sub_8259FBC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259FBC8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// srawi r10,r4,8
	ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r4.s32 >> 8;
	// clrlwi r8,r4,24
	ctx.r8.u64 = ctx.r4.u32 & 0xFF;
	// lwz r11,88(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 88);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x8259fc0c
	if (!ctx.cr6.eq) goto loc_8259FC0C;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r5,r11,r9
	ctx.r5.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x8259fc10
	goto loc_8259FC10;
loc_8259FC0C:
	// li r5,0
	ctx.r5.s64 = 0;
loc_8259FC10:
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824af030
	ctx.lr = 0x8259FC24;
	sub_824AF030(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x8259FC30;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259FC48);  /* forward decl */
PPC_WEAK_FUNC(sub_8259FC48) { __imp__sub_8259FC48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259FC48) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// srawi r10,r4,8
	ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r4.s32 >> 8;
	// clrlwi r8,r4,24
	ctx.r8.u64 = ctx.r4.u32 & 0xFF;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x8259fc8c
	if (!ctx.cr6.eq) goto loc_8259FC8C;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r5,r11,r9
	ctx.r5.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x8259fc90
	goto loc_8259FC90;
loc_8259FC8C:
	// li r5,0
	ctx.r5.s64 = 0;
loc_8259FC90:
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824af170
	ctx.lr = 0x8259FCA4;
	sub_824AF170(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x8259FCB0;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259FCC8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259FCC8) { __imp__sub_8259FCC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259FCC8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// srawi r10,r4,8
	ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r4.s32 >> 8;
	// clrlwi r8,r4,24
	ctx.r8.u64 = ctx.r4.u32 & 0xFF;
	// lwz r11,24544(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24544);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x8259fd0c
	if (!ctx.cr6.eq) goto loc_8259FD0C;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r5,r11,r9
	ctx.r5.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x8259fd10
	goto loc_8259FD10;
loc_8259FD0C:
	// li r5,0
	ctx.r5.s64 = 0;
loc_8259FD10:
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824af170
	ctx.lr = 0x8259FD24;
	sub_824AF170(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x8259FD30;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259FD48);  /* forward decl */
PPC_WEAK_FUNC(sub_8259FD48) { __imp__sub_8259FD48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259FD48) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// srawi r10,r4,8
	ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r4.s32 >> 8;
	// clrlwi r8,r4,24
	ctx.r8.u64 = ctx.r4.u32 & 0xFF;
	// lwz r11,88(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 88);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x8259fd8c
	if (!ctx.cr6.eq) goto loc_8259FD8C;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r5,r11,r9
	ctx.r5.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x8259fd90
	goto loc_8259FD90;
loc_8259FD8C:
	// li r5,0
	ctx.r5.s64 = 0;
loc_8259FD90:
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824af170
	ctx.lr = 0x8259FDA4;
	sub_824AF170(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x8259FDB0;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259FDC8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259FDC8) { __imp__sub_8259FDC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259FDC8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// srawi r10,r4,8
	ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r4.s32 >> 8;
	// clrlwi r8,r4,24
	ctx.r8.u64 = ctx.r4.u32 & 0xFF;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x8259fe0c
	if (!ctx.cr6.eq) goto loc_8259FE0C;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r5,r11,r9
	ctx.r5.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x8259fe10
	goto loc_8259FE10;
loc_8259FE0C:
	// li r5,0
	ctx.r5.s64 = 0;
loc_8259FE10:
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b0120
	ctx.lr = 0x8259FE24;
	sub_824B0120(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x8259FE30;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259FE48);  /* forward decl */
PPC_WEAK_FUNC(sub_8259FE48) { __imp__sub_8259FE48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259FE48) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// srawi r10,r3,8
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r3.s32 >> 8;
	// clrlwi r8,r3,24
	ctx.r8.u64 = ctx.r3.u32 & 0xFF;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x8259fe8c
	if (!ctx.cr6.eq) goto loc_8259FE8C;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r4,r11,r9
	ctx.r4.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x8259fe90
	goto loc_8259FE90;
loc_8259FE8C:
	// li r4,0
	ctx.r4.s64 = 0;
loc_8259FE90:
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b0a18
	ctx.lr = 0x8259FEA0;
	sub_824B0A18(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x8259FEAC;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259FEC0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259FEC0) { __imp__sub_8259FEC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259FEC0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// srawi r10,r3,8
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r3.s32 >> 8;
	// clrlwi r8,r3,24
	ctx.r8.u64 = ctx.r3.u32 & 0xFF;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x8259ff04
	if (!ctx.cr6.eq) goto loc_8259FF04;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r4,r11,r9
	ctx.r4.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x8259ff08
	goto loc_8259FF08;
loc_8259FF04:
	// li r4,0
	ctx.r4.s64 = 0;
loc_8259FF08:
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b0a80
	ctx.lr = 0x8259FF18;
	sub_824B0A80(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x8259FF24;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259FF38);  /* forward decl */
PPC_WEAK_FUNC(sub_8259FF38) { __imp__sub_8259FF38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259FF38) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// srawi r10,r3,8
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r3.s32 >> 8;
	// clrlwi r8,r3,24
	ctx.r8.u64 = ctx.r3.u32 & 0xFF;
	// mr r5,r4
	ctx.r5.u64 = ctx.r4.u64;
	// lwz r11,24544(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24544);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x8259ff80
	if (!ctx.cr6.eq) goto loc_8259FF80;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x8259ff84
	goto loc_8259FF84;
loc_8259FF80:
	// li r11,0
	ctx.r11.s64 = 0;
loc_8259FF84:
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// mr r4,r11
	ctx.r4.u64 = ctx.r11.u64;
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b0ae8
	ctx.lr = 0x8259FF98;
	sub_824B0AE8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x8259FFA4;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259FFB8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259FFB8) { __imp__sub_8259FFB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259FFB8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// srawi r10,r3,8
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r3.s32 >> 8;
	// clrlwi r8,r3,24
	ctx.r8.u64 = ctx.r3.u32 & 0xFF;
	// mr r5,r4
	ctx.r5.u64 = ctx.r4.u64;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825a0000
	if (!ctx.cr6.eq) goto loc_825A0000;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825a0004
	goto loc_825A0004;
loc_825A0000:
	// li r11,0
	ctx.r11.s64 = 0;
loc_825A0004:
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// mr r4,r11
	ctx.r4.u64 = ctx.r11.u64;
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b0b50
	ctx.lr = 0x825A0018;
	sub_824B0B50(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0024;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0038);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0038) { __imp__sub_825A0038(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0038) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// srawi r10,r3,8
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r3.s32 >> 8;
	// clrlwi r8,r3,24
	ctx.r8.u64 = ctx.r3.u32 & 0xFF;
	// mr r5,r4
	ctx.r5.u64 = ctx.r4.u64;
	// lwz r11,88(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 88);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825a0080
	if (!ctx.cr6.eq) goto loc_825A0080;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825a0084
	goto loc_825A0084;
loc_825A0080:
	// li r11,0
	ctx.r11.s64 = 0;
loc_825A0084:
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// mr r4,r11
	ctx.r4.u64 = ctx.r11.u64;
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b0bb8
	ctx.lr = 0x825A0098;
	sub_824B0BB8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A00A4;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A00B8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A00B8) { __imp__sub_825A00B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A00B8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// srawi r10,r3,8
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r3.s32 >> 8;
	// clrlwi r8,r3,24
	ctx.r8.u64 = ctx.r3.u32 & 0xFF;
	// mr r5,r4
	ctx.r5.u64 = ctx.r4.u64;
	// lwz r11,24544(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24544);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825a0100
	if (!ctx.cr6.eq) goto loc_825A0100;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825a0104
	goto loc_825A0104;
loc_825A0100:
	// li r11,0
	ctx.r11.s64 = 0;
loc_825A0104:
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// mr r4,r11
	ctx.r4.u64 = ctx.r11.u64;
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b0ca0
	ctx.lr = 0x825A0118;
	sub_824B0CA0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0124;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0138);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0138) { __imp__sub_825A0138(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0138) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
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
	// bl 0x824aeb10
	ctx.lr = 0x825A0184;
	sub_824AEB10(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0190;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A01A8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A01A8) { __imp__sub_825A01A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A01A8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
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
	// bl 0x824ae9a0
	ctx.lr = 0x825A01E4;
	sub_824AE9A0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A01F0;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0208);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0208) { __imp__sub_825A0208(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0208) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
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
	// bl 0x824aea58
	ctx.lr = 0x825A0244;
	sub_824AEA58(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0250;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0268);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0268) { __imp__sub_825A0268(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0268) {
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
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
	// bl 0x824aec70
	ctx.lr = 0x825A02AC;
	sub_824AEC70(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A02B8;
	sub_824B26C8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A02D0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A02D0) { __imp__sub_825A02D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A02D0) {
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
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
	// bl 0x824aece0
	ctx.lr = 0x825A0314;
	sub_824AECE0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0320;
	sub_824B26C8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A0338);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0338) { __imp__sub_825A0338(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0338) {
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f1,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x824aed50
	ctx.lr = 0x825A0364;
	sub_824AED50(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0370;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0388);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0388) { __imp__sub_825A0388(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0388) {
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f1,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x824aedb8
	ctx.lr = 0x825A03B4;
	sub_824AEDB8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A03C0;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A03D8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A03D8) { __imp__sub_825A03D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A03D8) {
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f1,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x824aee20
	ctx.lr = 0x825A0404;
	sub_824AEE20(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0410;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0428);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0428) { __imp__sub_825A0428(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0428) {
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f1,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x824aee88
	ctx.lr = 0x825A0454;
	sub_824AEE88(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0460;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0478);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0478) { __imp__sub_825A0478(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0478) {
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f1,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x824aeef0
	ctx.lr = 0x825A04A4;
	sub_824AEEF0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A04B0;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A04C8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A04C8) { __imp__sub_825A04C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A04C8) {
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
	// b 0x8259f420
	sub_8259F420(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A04E8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A04E8) { __imp__sub_825A04E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A04E8) {
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
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// lwz r11,4376(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4376);
	// stb r10,660(r11)
	PPC_STORE_U8(ctx.r11.u32 + 660, ctx.r10.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0510);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0510) { __imp__sub_825A0510(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0510) {
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f1,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x824aef58
	ctx.lr = 0x825A053C;
	sub_824AEF58(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0548;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0560);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0560) { __imp__sub_825A0560(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0560) {
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f1,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x824af1d8
	ctx.lr = 0x825A058C;
	sub_824AF1D8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0598;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A05B0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A05B0) { __imp__sub_825A05B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A05B0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8259f9c8
	sub_8259F9C8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A05C0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A05C0) { __imp__sub_825A05C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A05C0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8259f948
	sub_8259F948(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A05D0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A05D0) { __imp__sub_825A05D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A05D0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8259fa48
	sub_8259FA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A05E0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A05E0) { __imp__sub_825A05E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A05E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824af6c0
	ctx.lr = 0x825A0608;
	sub_824AF6C0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0614;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0628);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0628) { __imp__sub_825A0628(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0628) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8259fac8
	sub_8259FAC8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0638);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0638) { __imp__sub_825A0638(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0638) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8259fb48
	sub_8259FB48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0648);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0648) { __imp__sub_825A0648(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0648) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8259fbc8
	sub_8259FBC8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0658);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0658) { __imp__sub_825A0658(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0658) {
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
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
	// bl 0x824af098
	ctx.lr = 0x825A069C;
	sub_824AF098(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A06A8;
	sub_824B26C8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A06C0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A06C0) { __imp__sub_825A06C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A06C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
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
	// bl 0x824af108
	ctx.lr = 0x825A06FC;
	sub_824AF108(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0708;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0720);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0720) { __imp__sub_825A0720(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0720) {
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
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
	// bl 0x824aefc0
	ctx.lr = 0x825A0764;
	sub_824AEFC0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0770;
	sub_824B26C8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A0788);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0788) { __imp__sub_825A0788(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0788) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8259fc48
	sub_8259FC48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0798);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0798) { __imp__sub_825A0798(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0798) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8259fcc8
	sub_8259FCC8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A07A8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A07A8) { __imp__sub_825A07A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A07A8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8259fd48
	sub_8259FD48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A07B8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A07B8) { __imp__sub_825A07B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A07B8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x824af318
	ctx.lr = 0x825A07E4;
	sub_824AF318(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A07F0;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0808);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0808) { __imp__sub_825A0808(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0808) {
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
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
	// bl 0x824af2a8
	ctx.lr = 0x825A084C;
	sub_824AF2A8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0858;
	sub_824B26C8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A0870);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0870) { __imp__sub_825A0870(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0870) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
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
	// bl 0x824af380
	ctx.lr = 0x825A08AC;
	sub_824AF380(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A08B8;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A08D0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A08D0) { __imp__sub_825A08D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A08D0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8259f4f8
	sub_8259F4F8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A08E0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A08E0) { __imp__sub_825A08E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A08E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r3,r10,26488
	ctx.r3.s64 = ctx.r10.s64 + 26488;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x826f26b8
	ctx.lr = 0x825A090C;
	sub_826F26B8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A0930);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0930) { __imp__sub_825A0930(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0930) {
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f1,16(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f1.f64 = double(temp.f32);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lfs f2,20(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	ctx.f2.f64 = double(temp.f32);
	// lwz r6,8(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lfs f3,24(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	ctx.f3.f64 = double(temp.f32);
	// lwz r7,12(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// bl 0x824af488
	ctx.lr = 0x825A0970;
	sub_824AF488(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A097C;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0990);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0990) { __imp__sub_825A0990(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0990) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r6,8(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// bl 0x824af528
	ctx.lr = 0x825A09C0;
	sub_824AF528(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A09CC;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A09E0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A09E0) { __imp__sub_825A09E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A09E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824b3610
	ctx.lr = 0x825A0A08;
	sub_824B3610(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A0A28);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0A28) { __imp__sub_825A0A28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0A28) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
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
	// bl 0x824af588
	ctx.lr = 0x825A0A64;
	sub_824AF588(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0A70;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0A88);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0A88) { __imp__sub_825A0A88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0A88) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
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
	// bl 0x824af728
	ctx.lr = 0x825A0AC4;
	sub_824AF728(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0AD0;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0AE8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0AE8) { __imp__sub_825A0AE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0AE8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x824af790
	ctx.lr = 0x825A0B14;
	sub_824AF790(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0B20;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0B38);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0B38) { __imp__sub_825A0B38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0B38) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x824af860
	ctx.lr = 0x825A0B64;
	sub_824AF860(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0B70;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0B88);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0B88) { __imp__sub_825A0B88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0B88) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x824af7f8
	ctx.lr = 0x825A0BB4;
	sub_824AF7F8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0BC0;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0BD8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0BD8) { __imp__sub_825A0BD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0BD8) {
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f1,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x824af8c8
	ctx.lr = 0x825A0C04;
	sub_824AF8C8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0C10;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0C28);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0C28) { __imp__sub_825A0C28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0C28) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
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
	// bl 0x824af930
	ctx.lr = 0x825A0C64;
	sub_824AF930(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0C70;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0C88);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0C88) { __imp__sub_825A0C88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0C88) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lfs f1,0(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x824b19e0
	ctx.lr = 0x825A0CB0;
	sub_824B19E0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0CBC;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0CD0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0CD0) { __imp__sub_825A0CD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0CD0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r6,8(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r7,12(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r8,16(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// bl 0x824af998
	ctx.lr = 0x825A0D08;
	sub_824AF998(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0D14;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0D28);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0D28) { __imp__sub_825A0D28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0D28) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r6,8(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r7,12(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r8,16(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// bl 0x824afb40
	ctx.lr = 0x825A0D60;
	sub_824AFB40(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0D6C;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0D80);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0D80) { __imp__sub_825A0D80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0D80) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824afbb0
	ctx.lr = 0x825A0DA8;
	sub_824AFBB0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0DB4;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0DC8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0DC8) { __imp__sub_825A0DC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0DC8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824afc18
	ctx.lr = 0x825A0DF0;
	sub_824AFC18(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0DFC;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0E10);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0E10) { __imp__sub_825A0E10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0E10) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
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
	// bl 0x824b1be0
	ctx.lr = 0x825A0E4C;
	sub_824B1BE0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0E58;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0E70);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0E70) { __imp__sub_825A0E70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0E70) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824b2d18
	ctx.lr = 0x825A0E98;
	sub_824B2D18(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A0EB8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0EB8) { __imp__sub_825A0EB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0EB8) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// lwz r31,0(r10)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x824b3028
	ctx.lr = 0x825A0EE4;
	sub_824B3028(ctx, base);
	// lis r11,-32039
	ctx.r11.s64 = -2099707904;
	// lwz r30,15072(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 15072);
	// bl 0x821d2088
	ctx.lr = 0x825A0EF0;
	sub_821D2088(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// li r5,22
	ctx.r5.s64 = 22;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// bl 0x823086d8
	ctx.lr = 0x825A0F08;
	sub_823086D8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A0F20);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0F20) { __imp__sub_825A0F20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0F20) {
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
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
	// lfs f4,16(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f4.f64 = double(temp.f32);
	// bl 0x824afc80
	ctx.lr = 0x825A0F58;
	sub_824AFC80(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0F64;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0F78);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0F78) { __imp__sub_825A0F78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0F78) {
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f1,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// lwz r9,20(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lfs f2,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f2.f64 = double(temp.f32);
	// lwz r10,24(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// lfs f3,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f3.f64 = double(temp.f32);
	// lfs f4,16(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f4.f64 = double(temp.f32);
	// bl 0x824afd08
	ctx.lr = 0x825A0FB8;
	sub_824AFD08(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A0FC4;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A0FD8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A0FD8) { __imp__sub_825A0FD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A0FD8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
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
	// bl 0x824afda0
	ctx.lr = 0x825A1014;
	sub_824AFDA0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A1020;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1038);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1038) { __imp__sub_825A1038(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1038) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x824afe08
	ctx.lr = 0x825A1064;
	sub_824AFE08(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A1070;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1088);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1088) { __imp__sub_825A1088(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1088) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824afe70
	ctx.lr = 0x825A10B0;
	sub_824AFE70(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A10BC;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A10D0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A10D0) { __imp__sub_825A10D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A10D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x824afed8
	ctx.lr = 0x825A10FC;
	sub_824AFED8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A1108;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1120);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1120) { __imp__sub_825A1120(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1120) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x824b0f80
	ctx.lr = 0x825A114C;
	sub_824B0F80(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A1158;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1170);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1170) { __imp__sub_825A1170(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1170) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
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
	// bl 0x824b0fe8
	ctx.lr = 0x825A11AC;
	sub_824B0FE8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A11B8;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A11D0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A11D0) { __imp__sub_825A11D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A11D0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8259fdc8
	sub_8259FDC8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A11E0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A11E0) { __imp__sub_825A11E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A11E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,-2704
	ctx.r3.s64 = ctx.r11.s64 + -2704;
	// b 0x8259f8d0
	sub_8259F8D0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A11F0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A11F0) { __imp__sub_825A11F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A11F0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x824aff40
	ctx.lr = 0x825A121C;
	sub_824AFF40(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A1228;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1240);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1240) { __imp__sub_825A1240(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1240) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824affe8
	ctx.lr = 0x825A1268;
	sub_824AFFE8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A1274;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1288);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1288) { __imp__sub_825A1288(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1288) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824b0050
	ctx.lr = 0x825A12B0;
	sub_824B0050(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A12BC;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A12D0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A12D0) { __imp__sub_825A12D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A12D0) {
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
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824b0310
	ctx.lr = 0x825A12FC;
	sub_824B0310(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
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
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1320);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1320) { __imp__sub_825A1320(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1320) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824b00b8
	ctx.lr = 0x825A1348;
	sub_824B00B8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A1354;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1368);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1368) { __imp__sub_825A1368(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1368) {
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r3,r10,26488
	ctx.r3.s64 = ctx.r10.s64 + 26488;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f1,16(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f1.f64 = double(temp.f32);
	// lfs f0,4(r11)
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
	// bl 0x824b3038
	ctx.lr = 0x825A13B0;
	sub_824B3038(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
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

PPC_FUNC_IMPL(__imp__sub_825A13D0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A13D0) { __imp__sub_825A13D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A13D0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r3,r10,26488
	ctx.r3.s64 = ctx.r10.s64 + 26488;
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x824b31b8
	sub_824B31B8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A13E8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A13E8) { __imp__sub_825A13E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A13E8) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// b 0x824b32b0
	sub_824B32B0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1400);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1400) { __imp__sub_825A1400(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1400) {
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// addi r3,r10,26488
	ctx.r3.s64 = ctx.r10.s64 + 26488;
	// lwz r5,12(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lfs f0,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lwz r6,16(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lfs f13,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lwz r7,20(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lfs f12,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f12.f64 = double(temp.f32);
	// lwz r8,24(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f12,88(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// bl 0x824b11c8
	ctx.lr = 0x825A1448;
	sub_824B11C8(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1458);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1458) { __imp__sub_825A1458(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1458) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// bl 0x824b1318
	ctx.lr = 0x825A1478;
	sub_824B1318(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A1498);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1498) { __imp__sub_825A1498(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1498) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// bl 0x824b1218
	ctx.lr = 0x825A14D0;
	sub_824B1218(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A14DC;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A14F0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A14F0) { __imp__sub_825A14F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A14F0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// bl 0x824b1298
	ctx.lr = 0x825A1528;
	sub_824B1298(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A1534;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1548);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1548) { __imp__sub_825A1548(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1548) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// bl 0x824b1358
	ctx.lr = 0x825A1568;
	sub_824B1358(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A1588);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1588) { __imp__sub_825A1588(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1588) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// bl 0x824b2ed0
	ctx.lr = 0x825A15A8;
	sub_824B2ED0(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A15C8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A15C8) { __imp__sub_825A15C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A15C8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// bl 0x824b0db0
	ctx.lr = 0x825A15E8;
	sub_824B0DB0(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A1608);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1608) { __imp__sub_825A1608(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1608) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// bl 0x824b0e08
	ctx.lr = 0x825A1628;
	sub_824B0E08(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A1648);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1648) { __imp__sub_825A1648(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1648) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// bl 0x824b0e60
	ctx.lr = 0x825A1668;
	sub_824B0E60(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A1688);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1688) { __imp__sub_825A1688(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1688) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// bl 0x824b0eb8
	ctx.lr = 0x825A16A8;
	sub_824B0EB8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A16C8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A16C8) { __imp__sub_825A16C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A16C8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824b4a98
	ctx.lr = 0x825A16F0;
	sub_824B4A98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A1710);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1710) { __imp__sub_825A1710(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1710) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824af5f0
	ctx.lr = 0x825A1738;
	sub_824AF5F0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A1744;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1758);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1758) { __imp__sub_825A1758(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1758) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824af658
	ctx.lr = 0x825A1780;
	sub_824AF658(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A178C;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A17A0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A17A0) { __imp__sub_825A17A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A17A0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// bl 0x824b0d10
	ctx.lr = 0x825A17C0;
	sub_824B0D10(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A17E0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A17E0) { __imp__sub_825A17E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A17E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824b03f8
	ctx.lr = 0x825A1808;
	sub_824B03F8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A1814;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1828);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1828) { __imp__sub_825A1828(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1828) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824b04d0
	ctx.lr = 0x825A1850;
	sub_824B04D0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A185C;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1870);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1870) { __imp__sub_825A1870(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1870) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824b05a8
	ctx.lr = 0x825A1898;
	sub_824B05A8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A18A4;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A18B8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A18B8) { __imp__sub_825A18B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A18B8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824b0678
	ctx.lr = 0x825A18E0;
	sub_824B0678(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A18EC;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1900);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1900) { __imp__sub_825A1900(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1900) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8259fe48
	sub_8259FE48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1910);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1910) { __imp__sub_825A1910(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1910) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8259fec0
	sub_8259FEC0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1920);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1920) { __imp__sub_825A1920(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1920) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x824afa08
	ctx.lr = 0x825A194C;
	sub_824AFA08(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A1958;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1970);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1970) { __imp__sub_825A1970(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1970) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x824afad8
	ctx.lr = 0x825A199C;
	sub_824AFAD8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A19A8;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A19C0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A19C0) { __imp__sub_825A19C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A19C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x824afa70
	ctx.lr = 0x825A19EC;
	sub_824AFA70(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A19F8;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1A10);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1A10) { __imp__sub_825A1A10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1A10) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824b3418
	ctx.lr = 0x825A1A38;
	sub_824B3418(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A1A58);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1A58) { __imp__sub_825A1A58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1A58) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824b3468
	ctx.lr = 0x825A1A80;
	sub_824B3468(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A1AA0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1AA0) { __imp__sub_825A1AA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1AA0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824b34b8
	ctx.lr = 0x825A1AC8;
	sub_824B34B8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A1AE8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1AE8) { __imp__sub_825A1AE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1AE8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// bl 0x824b1050
	ctx.lr = 0x825A1B08;
	sub_824B1050(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A1B28);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1B28) { __imp__sub_825A1B28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1B28) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// li r7,0
	ctx.r7.s64 = 0;
	// addi r3,r10,26488
	ctx.r3.s64 = ctx.r10.s64 + 26488;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r6,8(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// bl 0x824b1088
	ctx.lr = 0x825A1B5C;
	sub_824B1088(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A1B78);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1B78) { __imp__sub_825A1B78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1B78) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824b3538
	ctx.lr = 0x825A1BA4;
	sub_824B3538(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A1BC8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1BC8) { __imp__sub_825A1BC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1BC8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r3,r10,26488
	ctx.r3.s64 = ctx.r10.s64 + 26488;
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x824b3650
	sub_824B3650(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1C08);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1C08) { __imp__sub_825A1C08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1C08) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// b 0x824b11c0
	sub_824B11C0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1C30);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1C30) { __imp__sub_825A1C30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1C30) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// bl 0x824b1368
	ctx.lr = 0x825A1C68;
	sub_824B1368(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A1C74;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1C88);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1C88) { __imp__sub_825A1C88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1C88) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// bl 0x824b02b0
	ctx.lr = 0x825A1CA8;
	sub_824B02B0(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A1CC8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1CC8) { __imp__sub_825A1CC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1CC8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// bl 0x824b1420
	ctx.lr = 0x825A1D00;
	sub_824B1420(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A1D0C;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1D20);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1D20) { __imp__sub_825A1D20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1D20) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lfs f1,0(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x824b1580
	ctx.lr = 0x825A1D48;
	sub_824B1580(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A1D54;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1D68);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1D68) { __imp__sub_825A1D68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1D68) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lfs f1,0(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x824b0898
	ctx.lr = 0x825A1D90;
	sub_824B0898(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A1D9C;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1DB0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1DB0) { __imp__sub_825A1DB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1DB0) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lfs f1,0(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x824b0958
	ctx.lr = 0x825A1DD8;
	sub_824B0958(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A1DE4;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1DF8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1DF8) { __imp__sub_825A1DF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1DF8) {
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f1,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x824b29d0
	ctx.lr = 0x825A1E24;
	sub_824B29D0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A1E30;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1E48);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1E48) { __imp__sub_825A1E48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1E48) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lfs f0,0(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,-6516(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -6516);
	ctx.f13.f64 = double(temp.f32);
	// fmuls f1,f0,f13
	ctx.f1.f64 = double(float(ctx.f0.f64 * ctx.f13.f64));
	// bl 0x824b22b0
	ctx.lr = 0x825A1E7C;
	sub_824B22B0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A1E88;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1EA0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1EA0) { __imp__sub_825A1EA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1EA0) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lfs f0,0(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,-6516(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -6516);
	ctx.f13.f64 = double(temp.f32);
	// fmuls f1,f0,f13
	ctx.f1.f64 = double(float(ctx.f0.f64 * ctx.f13.f64));
	// bl 0x824b2318
	ctx.lr = 0x825A1ED4;
	sub_824B2318(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A1EE0;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1EF8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1EF8) { __imp__sub_825A1EF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1EF8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824b2390
	ctx.lr = 0x825A1F20;
	sub_824B2390(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A1F2C;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1F40);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1F40) { __imp__sub_825A1F40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1F40) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
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
	// bl 0x824b1628
	ctx.lr = 0x825A1F7C;
	sub_824B1628(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A1F88;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1FA0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1FA0) { __imp__sub_825A1FA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1FA0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x824b1690
	ctx.lr = 0x825A1FCC;
	sub_824B1690(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A1FD8;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A1FF0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A1FF0) { __imp__sub_825A1FF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A1FF0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824b16f8
	ctx.lr = 0x825A2018;
	sub_824B16F8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A2024;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2038);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2038) { __imp__sub_825A2038(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2038) {
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
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// lwz r9,8(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r11,r9
	ctx.r11.u64 = ctx.r9.u64;
	// lwz r10,0(r9)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
	// cntlzw r10,r10
	ctx.r10.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// lfs f0,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f13.f64 = double(temp.f32);
	// rlwinm r10,r10,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// lfs f12,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f12.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f12,88(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// clrlwi r4,r10,24
	ctx.r4.u64 = ctx.r10.u32 & 0xFF;
	// bl 0x824b18d0
	ctx.lr = 0x825A2094;
	sub_824B18D0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A20A0;
	sub_824B26C8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A20B8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A20B8) { __imp__sub_825A20B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A20B8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824b1760
	ctx.lr = 0x825A20E0;
	sub_824B1760(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A20EC;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2100);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2100) { __imp__sub_825A2100(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2100) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// bl 0x824b1968
	ctx.lr = 0x825A2120;
	sub_824B1968(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A2140);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2140) { __imp__sub_825A2140(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2140) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// bl 0x824b1a50
	ctx.lr = 0x825A2178;
	sub_824B1A50(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A2184;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2198);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2198) { __imp__sub_825A2198(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2198) {
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
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
	// bl 0x824b1b00
	ctx.lr = 0x825A21D4;
	sub_824B1B00(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A21E0;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A21F8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A21F8) { __imp__sub_825A21F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A21F8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// bl 0x824b37f0
	ctx.lr = 0x825A2218;
	sub_824B37F0(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A2238);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2238) { __imp__sub_825A2238(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2238) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// bl 0x824b1500
	ctx.lr = 0x825A2270;
	sub_824B1500(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A227C;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2290);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2290) { __imp__sub_825A2290(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2290) {
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f1,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// lwz r6,8(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// bl 0x824b1c48
	ctx.lr = 0x825A22C0;
	sub_824B1C48(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A22CC;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A22E0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A22E0) { __imp__sub_825A22E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A22E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r6,8(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// bl 0x824b1cc0
	ctx.lr = 0x825A2310;
	sub_824B1CC0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A231C;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2330);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2330) { __imp__sub_825A2330(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2330) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lfs f1,0(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x824b1df0
	ctx.lr = 0x825A2358;
	sub_824B1DF0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A2364;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2378);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2378) { __imp__sub_825A2378(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2378) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lfs f1,0(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x824b1e98
	ctx.lr = 0x825A23A0;
	sub_824B1E98(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A23AC;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A23C0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A23C0) { __imp__sub_825A23C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A23C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x824b1fc8
	ctx.lr = 0x825A23EC;
	sub_824B1FC8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A23F8;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2410);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2410) { __imp__sub_825A2410(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2410) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824b3af8
	ctx.lr = 0x825A2438;
	sub_824B3AF8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A2458);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2458) { __imp__sub_825A2458(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2458) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lfs f1,0(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x824b2098
	ctx.lr = 0x825A2480;
	sub_824B2098(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A248C;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A24A0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A24A0) { __imp__sub_825A24A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A24A0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// bl 0x824b2128
	ctx.lr = 0x825A24D8;
	sub_824B2128(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A24E4;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A24F8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A24F8) { __imp__sub_825A24F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A24F8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,27832(r10)
	PPC_STORE_U8(ctx.r10.u32 + 27832, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2518);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2518) { __imp__sub_825A2518(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2518) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// bl 0x824b21b8
	ctx.lr = 0x825A2550;
	sub_824B21B8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A255C;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2570);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2570) { __imp__sub_825A2570(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2570) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// bl 0x824b2248
	ctx.lr = 0x825A25A8;
	sub_824B2248(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A25B4;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A25C8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A25C8) { __imp__sub_825A25C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A25C8) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// b 0x824b2220
	sub_824B2220(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A25F0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A25F0) { __imp__sub_825A25F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A25F0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32093
	ctx.r10.s64 = -2103246848;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,-14396(r10)
	PPC_STORE_U8(ctx.r10.u32 + -14396, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2610);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2610) { __imp__sub_825A2610(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2610) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// bl 0x826f26b8
	ctx.lr = 0x825A2630;
	sub_826F26B8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A2650);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2650) { __imp__sub_825A2650(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2650) {
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
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
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f12,88(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// bl 0x824b0748
	ctx.lr = 0x825A2690;
	sub_824B0748(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A269C;
	sub_824B26C8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A26B0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A26B0) { __imp__sub_825A26B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A26B0) {
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r5,12(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
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
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f12,88(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// bl 0x824b07b8
	ctx.lr = 0x825A26F4;
	sub_824B07B8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A2700;
	sub_824B26C8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A2718);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2718) { __imp__sub_825A2718(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2718) {
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r5,12(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
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
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f12,88(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// bl 0x824b0828
	ctx.lr = 0x825A275C;
	sub_824B0828(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A2768;
	sub_824B26C8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A2780);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2780) { __imp__sub_825A2780(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2780) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8259ff38
	sub_8259FF38(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2790);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2790) { __imp__sub_825A2790(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2790) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8259ffb8
	sub_8259FFB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A27A0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A27A0) { __imp__sub_825A27A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A27A0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825a0038
	sub_825A0038(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A27B0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A27B0) { __imp__sub_825A27B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A27B0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r11,26488
	ctx.r31.s64 = ctx.r11.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824b0c20
	ctx.lr = 0x825A27D8;
	sub_824B0C20(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A27E4;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A27F8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A27F8) { __imp__sub_825A27F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A27F8) {
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
	// b 0x825a00b8
	sub_825A00B8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2818);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2818) { __imp__sub_825A2818(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2818) {
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
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// lwz r31,0(r10)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824b0188
	ctx.lr = 0x825A2844;
	sub_824B0188(ctx, base);
	// lbz r11,80(r1)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r1.u32 + 80);
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

PPC_FUNC_IMPL(__imp__sub_825A2860);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2860) { __imp__sub_825A2860(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2860) {
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
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// lwz r31,0(r10)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824b0220
	ctx.lr = 0x825A2888;
	sub_824B0220(ctx, base);
	// lbz r11,80(r1)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r1.u32 + 80);
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

PPC_FUNC_IMPL(__imp__sub_825A28A8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A28A8) { __imp__sub_825A28A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A28A8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r31,r10,26488
	ctx.r31.s64 = ctx.r10.s64 + 26488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x824aebe8
	ctx.lr = 0x825A28D4;
	sub_824AEBE8(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824b26c8
	ctx.lr = 0x825A28E0;
	sub_824B26C8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A28F8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A28F8) { __imp__sub_825A28F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A28F8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x825A2900;
	__savegprlr_29(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r3,r10,26488
	ctx.r3.s64 = ctx.r10.s64 + 26488;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r31,8(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r30,12(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r29,4(r11)
	ctx.r29.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x824b5278
	ctx.lr = 0x825A2928;
	sub_824B5278(ctx, base);
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r29)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r29.u32 + 0, temp.u32);
	// lfs f0,84(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r31.u32 + 0, temp.u32);
	// lfs f0,88(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 88);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r30)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r30.u32 + 0, temp.u32);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2948);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2948) { __imp__sub_825A2948(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2948) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x825A2950;
	__savegprlr_29(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r3,r10,26488
	ctx.r3.s64 = ctx.r10.s64 + 26488;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r31,8(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r30,12(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r29,4(r11)
	ctx.r29.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x824b52d0
	ctx.lr = 0x825A2978;
	sub_824B52D0(ctx, base);
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r29)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r29.u32 + 0, temp.u32);
	// lfs f0,84(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r31.u32 + 0, temp.u32);
	// lfs f0,88(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 88);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r30)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r30.u32 + 0, temp.u32);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2998);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2998) { __imp__sub_825A2998(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2998) {
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
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r3,r10,26488
	ctx.r3.s64 = ctx.r10.s64 + 26488;
	// lwz r31,4(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x824b5388
	ctx.lr = 0x825A29C4;
	sub_824B5388(ctx, base);
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r31.u32 + 0, temp.u32);
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

PPC_FUNC_IMPL(__imp__sub_825A29E0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A29E0) { __imp__sub_825A29E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A29E0) {
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
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r3,r10,26488
	ctx.r3.s64 = ctx.r10.s64 + 26488;
	// lwz r31,4(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x824b53d8
	ctx.lr = 0x825A2A0C;
	sub_824B53D8(ctx, base);
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r31.u32 + 0, temp.u32);
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

PPC_FUNC_IMPL(__imp__sub_825A2A28);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2A28) { __imp__sub_825A2A28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2A28) {
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
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r3,r10,26488
	ctx.r3.s64 = ctx.r10.s64 + 26488;
	// lwz r31,4(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x824b5428
	ctx.lr = 0x825A2A54;
	sub_824B5428(ctx, base);
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r31.u32 + 0, temp.u32);
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

PPC_FUNC_IMPL(__imp__sub_825A2A70);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2A70) { __imp__sub_825A2A70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2A70) {
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
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r3,r10,26488
	ctx.r3.s64 = ctx.r10.s64 + 26488;
	// lwz r31,4(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x824b5478
	ctx.lr = 0x825A2A9C;
	sub_824B5478(ctx, base);
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r31.u32 + 0, temp.u32);
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

PPC_FUNC_IMPL(__imp__sub_825A2AB8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2AB8) { __imp__sub_825A2AB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2AB8) {
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
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r3,r10,26488
	ctx.r3.s64 = ctx.r10.s64 + 26488;
	// lwz r31,4(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x824b54c8
	ctx.lr = 0x825A2AE4;
	sub_824B54C8(ctx, base);
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r31.u32 + 0, temp.u32);
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

PPC_FUNC_IMPL(__imp__sub_825A2B00);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2B00) { __imp__sub_825A2B00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2B00) {
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
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r3,r10,26488
	ctx.r3.s64 = ctx.r10.s64 + 26488;
	// lwz r31,4(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x824b5518
	ctx.lr = 0x825A2B2C;
	sub_824B5518(ctx, base);
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r31.u32 + 0, temp.u32);
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

PPC_FUNC_IMPL(__imp__sub_825A2B48);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2B48) { __imp__sub_825A2B48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2B48) {
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r3,r10,26488
	ctx.r3.s64 = ctx.r10.s64 + 26488;
	// lwz r31,4(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// bl 0x824b0258
	ctx.lr = 0x825A2B78;
	sub_824B0258(ctx, base);
	// lis r11,-32039
	ctx.r11.s64 = -2099707904;
	// lwz r30,15072(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 15072);
	// bl 0x821d2088
	ctx.lr = 0x825A2B84;
	sub_821D2088(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// li r5,21
	ctx.r5.s64 = 21;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r8,0
	ctx.r8.s64 = 0;
	// bl 0x823093f8
	ctx.lr = 0x825A2BA0;
	sub_823093F8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A2BB8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2BB8) { __imp__sub_825A2BB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2BB8) {
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r3,r10,26488
	ctx.r3.s64 = ctx.r10.s64 + 26488;
	// lwz r31,4(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// bl 0x824b5568
	ctx.lr = 0x825A2BE8;
	sub_824B5568(ctx, base);
	// lis r11,-32039
	ctx.r11.s64 = -2099707904;
	// lwz r30,15072(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 15072);
	// bl 0x821d2088
	ctx.lr = 0x825A2BF4;
	sub_821D2088(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// li r5,21
	ctx.r5.s64 = 21;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r8,0
	ctx.r8.s64 = 0;
	// bl 0x823093f8
	ctx.lr = 0x825A2C10;
	sub_823093F8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A2C28);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2C28) { __imp__sub_825A2C28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2C28) {
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
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// lwz r31,0(r10)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x824b6028
	ctx.lr = 0x825A2C54;
	sub_824B6028(ctx, base);
	// lis r11,-32039
	ctx.r11.s64 = -2099707904;
	// lwz r30,15072(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 15072);
	// bl 0x821d2088
	ctx.lr = 0x825A2C60;
	sub_821D2088(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// li r5,22
	ctx.r5.s64 = 22;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r8,0
	ctx.r8.s64 = 0;
	// bl 0x823093f8
	ctx.lr = 0x825A2C7C;
	sub_823093F8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A2C98);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2C98) { __imp__sub_825A2C98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2C98) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// b 0x824b5660
	sub_824B5660(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2CB0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2CB0) { __imp__sub_825A2CB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2CB0) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// b 0x824b56b0
	sub_824B56B0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2CC8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2CC8) { __imp__sub_825A2CC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2CC8) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// b 0x824b57a8
	sub_824B57A8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2CE0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2CE0) { __imp__sub_825A2CE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2CE0) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// b 0x824b5820
	sub_824B5820(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2CF8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2CF8) { __imp__sub_825A2CF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2CF8) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// b 0x824b58a0
	sub_824B58A0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2D10);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2D10) { __imp__sub_825A2D10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2D10) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// b 0x824b5920
	sub_824B5920(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2D28);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2D28) { __imp__sub_825A2D28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2D28) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// lwz r31,0(r10)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824b5f70
	ctx.lr = 0x825A2D4C;
	sub_824B5F70(ctx, base);
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2D68);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2D68) { __imp__sub_825A2D68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2D68) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// b 0x824b5728
	sub_824B5728(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2D80);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2D80) { __imp__sub_825A2D80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2D80) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// b 0x824b27a8
	sub_824B27A8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2D98);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2D98) { __imp__sub_825A2D98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2D98) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r3,r10,26488
	ctx.r3.s64 = ctx.r10.s64 + 26488;
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x824b0330
	sub_824B0330(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2DB0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2DB0) { __imp__sub_825A2DB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2DB0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r3,4(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r31,0(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x82318f60
	ctx.lr = 0x825A2DD0;
	sub_82318F60(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x824b31b8
	ctx.lr = 0x825A2DE4;
	sub_824B31B8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2DF8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2DF8) { __imp__sub_825A2DF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2DF8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r3,r10,26488
	ctx.r3.s64 = ctx.r10.s64 + 26488;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r31,4(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x824b3360
	ctx.lr = 0x825A2E20;
	sub_824B3360(ctx, base);
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2E38);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2E38) { __imp__sub_825A2E38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2E38) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32066
	ctx.r11.s64 = -2101477376;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r11,r11,12896
	ctx.r11.s64 = ctx.r11.s64 + 12896;
	// lwz r10,0(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// lwz r11,16(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r11,1340(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 1340);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2E58);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2E58) { __imp__sub_825A2E58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2E58) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32066
	ctx.r11.s64 = -2101477376;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r11,r11,12896
	ctx.r11.s64 = ctx.r11.s64 + 12896;
	// lwz r10,0(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// lwz r11,20(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lwz r11,1340(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 1340);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2E78);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2E78) { __imp__sub_825A2E78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2E78) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32066
	ctx.r11.s64 = -2101477376;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r11,r11,12896
	ctx.r11.s64 = ctx.r11.s64 + 12896;
	// lwz r10,0(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// lwz r11,24(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// lwz r11,1340(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 1340);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2E98);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2E98) { __imp__sub_825A2E98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2E98) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x825A2EA0;
	__savegprlr_28(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// addi r6,r1,88
	ctx.r6.s64 = ctx.r1.s64 + 88;
	// addi r3,r10,26488
	ctx.r3.s64 = ctx.r10.s64 + 26488;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
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
	// bl 0x824b35a0
	ctx.lr = 0x825A2ED0;
	sub_824B35A0(ctx, base);
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r29)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r29.u32 + 0, temp.u32);
	// lfs f0,84(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r28)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r28.u32 + 0, temp.u32);
	// lfs f0,88(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 88);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r31.u32 + 0, temp.u32);
	// lfs f0,92(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 92);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r30)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r30.u32 + 0, temp.u32);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2EF8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2EF8) { __imp__sub_825A2EF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2EF8) {
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
	// lwz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lfs f3,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f3.f64 = double(temp.f32);
	// bl 0x8259f618
	ctx.lr = 0x825A2F2C;
	sub_8259F618(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A2F50);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2F50) { __imp__sub_825A2F50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2F50) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// addi r3,r11,26488
	ctx.r3.s64 = ctx.r11.s64 + 26488;
	// lwz r31,0(r10)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824b1860
	ctx.lr = 0x825A2F74;
	sub_824B1860(ctx, base);
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A2F90);  /* forward decl */
PPC_WEAK_FUNC(sub_825A2F90) { __imp__sub_825A2F90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A2F90) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,10264
	ctx.r4.s64 = ctx.r11.s64 + 10264;
	// addi r3,r10,-22584
	ctx.r3.s64 = ctx.r10.s64 + -22584;
	// bl 0x827d6c90
	ctx.lr = 0x825A2FB0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,10336
	ctx.r4.s64 = ctx.r11.s64 + 10336;
	// addi r3,r10,-22604
	ctx.r3.s64 = ctx.r10.s64 + -22604;
	// bl 0x827d6c90
	ctx.lr = 0x825A2FC4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,312
	ctx.r4.s64 = ctx.r11.s64 + 312;
	// addi r3,r10,-22628
	ctx.r3.s64 = ctx.r10.s64 + -22628;
	// bl 0x827d6c90
	ctx.lr = 0x825A2FD8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,424
	ctx.r4.s64 = ctx.r11.s64 + 424;
	// addi r3,r10,-22660
	ctx.r3.s64 = ctx.r10.s64 + -22660;
	// bl 0x827d6c90
	ctx.lr = 0x825A2FEC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,520
	ctx.r4.s64 = ctx.r11.s64 + 520;
	// addi r3,r10,-22692
	ctx.r3.s64 = ctx.r10.s64 + -22692;
	// bl 0x827d6c90
	ctx.lr = 0x825A3000;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,10408
	ctx.r4.s64 = ctx.r11.s64 + 10408;
	// addi r3,r10,-22708
	ctx.r3.s64 = ctx.r10.s64 + -22708;
	// bl 0x827d6c90
	ctx.lr = 0x825A3014;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,616
	ctx.r4.s64 = ctx.r11.s64 + 616;
	// addi r3,r10,-22720
	ctx.r3.s64 = ctx.r10.s64 + -22720;
	// bl 0x827d6c90
	ctx.lr = 0x825A3028;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,720
	ctx.r4.s64 = ctx.r11.s64 + 720;
	// addi r3,r10,-22732
	ctx.r3.s64 = ctx.r10.s64 + -22732;
	// bl 0x827d6c90
	ctx.lr = 0x825A303C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,824
	ctx.r4.s64 = ctx.r11.s64 + 824;
	// addi r3,r10,-22744
	ctx.r3.s64 = ctx.r10.s64 + -22744;
	// bl 0x827d6c90
	ctx.lr = 0x825A3050;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,904
	ctx.r4.s64 = ctx.r11.s64 + 904;
	// addi r3,r10,-22764
	ctx.r3.s64 = ctx.r10.s64 + -22764;
	// bl 0x827d6c90
	ctx.lr = 0x825A3064;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,984
	ctx.r4.s64 = ctx.r11.s64 + 984;
	// addi r3,r10,-22784
	ctx.r3.s64 = ctx.r10.s64 + -22784;
	// bl 0x827d6c90
	ctx.lr = 0x825A3078;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,1064
	ctx.r4.s64 = ctx.r11.s64 + 1064;
	// addi r3,r10,-22804
	ctx.r3.s64 = ctx.r10.s64 + -22804;
	// bl 0x827d6c90
	ctx.lr = 0x825A308C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,1144
	ctx.r4.s64 = ctx.r11.s64 + 1144;
	// addi r3,r10,-22820
	ctx.r3.s64 = ctx.r10.s64 + -22820;
	// bl 0x827d6c90
	ctx.lr = 0x825A30A0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,1224
	ctx.r4.s64 = ctx.r11.s64 + 1224;
	// addi r3,r10,-22844
	ctx.r3.s64 = ctx.r10.s64 + -22844;
	// bl 0x827d6c90
	ctx.lr = 0x825A30B4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,1256
	ctx.r4.s64 = ctx.r11.s64 + 1256;
	// addi r3,r10,-22860
	ctx.r3.s64 = ctx.r10.s64 + -22860;
	// bl 0x827d6c90
	ctx.lr = 0x825A30C8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,1296
	ctx.r4.s64 = ctx.r11.s64 + 1296;
	// addi r3,r10,-22880
	ctx.r3.s64 = ctx.r10.s64 + -22880;
	// bl 0x827d6c90
	ctx.lr = 0x825A30DC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,10488
	ctx.r4.s64 = ctx.r11.s64 + 10488;
	// addi r3,r10,-22892
	ctx.r3.s64 = ctx.r10.s64 + -22892;
	// bl 0x827d6c90
	ctx.lr = 0x825A30F0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,10568
	ctx.r4.s64 = ctx.r11.s64 + 10568;
	// addi r3,r10,-22904
	ctx.r3.s64 = ctx.r10.s64 + -22904;
	// bl 0x827d6c90
	ctx.lr = 0x825A3104;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,10648
	ctx.r4.s64 = ctx.r11.s64 + 10648;
	// addi r3,r10,-22916
	ctx.r3.s64 = ctx.r10.s64 + -22916;
	// bl 0x827d6c90
	ctx.lr = 0x825A3118;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,10720
	ctx.r4.s64 = ctx.r11.s64 + 10720;
	// addi r3,r10,-22936
	ctx.r3.s64 = ctx.r10.s64 + -22936;
	// bl 0x827d6c90
	ctx.lr = 0x825A312C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,10792
	ctx.r4.s64 = ctx.r11.s64 + 10792;
	// addi r3,r10,-22956
	ctx.r3.s64 = ctx.r10.s64 + -22956;
	// bl 0x827d6c90
	ctx.lr = 0x825A3140;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,10864
	ctx.r4.s64 = ctx.r11.s64 + 10864;
	// addi r3,r10,-22976
	ctx.r3.s64 = ctx.r10.s64 + -22976;
	// bl 0x827d6c90
	ctx.lr = 0x825A3154;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,10936
	ctx.r4.s64 = ctx.r11.s64 + 10936;
	// addi r3,r10,-22992
	ctx.r3.s64 = ctx.r10.s64 + -22992;
	// bl 0x827d6c90
	ctx.lr = 0x825A3168;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,11008
	ctx.r4.s64 = ctx.r11.s64 + 11008;
	// addi r3,r10,-23012
	ctx.r3.s64 = ctx.r10.s64 + -23012;
	// bl 0x827d6c90
	ctx.lr = 0x825A317C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,1376
	ctx.r4.s64 = ctx.r11.s64 + 1376;
	// addi r3,r10,-23028
	ctx.r3.s64 = ctx.r10.s64 + -23028;
	// bl 0x827d6c90
	ctx.lr = 0x825A3190;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,1456
	ctx.r4.s64 = ctx.r11.s64 + 1456;
	// addi r3,r10,-23060
	ctx.r3.s64 = ctx.r10.s64 + -23060;
	// bl 0x827d6c90
	ctx.lr = 0x825A31A4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,1472
	ctx.r4.s64 = ctx.r11.s64 + 1472;
	// addi r3,r10,-23088
	ctx.r3.s64 = ctx.r10.s64 + -23088;
	// bl 0x827d6c90
	ctx.lr = 0x825A31B8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,1488
	ctx.r4.s64 = ctx.r11.s64 + 1488;
	// addi r3,r10,-23116
	ctx.r3.s64 = ctx.r10.s64 + -23116;
	// bl 0x827d6c90
	ctx.lr = 0x825A31CC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,1504
	ctx.r4.s64 = ctx.r11.s64 + 1504;
	// addi r3,r10,-23136
	ctx.r3.s64 = ctx.r10.s64 + -23136;
	// bl 0x827d6c90
	ctx.lr = 0x825A31E0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,1576
	ctx.r4.s64 = ctx.r11.s64 + 1576;
	// addi r3,r10,-23156
	ctx.r3.s64 = ctx.r10.s64 + -23156;
	// bl 0x827d6c90
	ctx.lr = 0x825A31F4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,1592
	ctx.r4.s64 = ctx.r11.s64 + 1592;
	// addi r3,r10,-23180
	ctx.r3.s64 = ctx.r10.s64 + -23180;
	// bl 0x827d6c90
	ctx.lr = 0x825A3208;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,1608
	ctx.r4.s64 = ctx.r11.s64 + 1608;
	// addi r3,r10,-23204
	ctx.r3.s64 = ctx.r10.s64 + -23204;
	// bl 0x827d6c90
	ctx.lr = 0x825A321C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// addi r4,r11,1624
	ctx.r4.s64 = ctx.r11.s64 + 1624;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r3,r11,-23228
	ctx.r3.s64 = ctx.r11.s64 + -23228;
	// bl 0x827d6c90
	ctx.lr = 0x825A3230;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,1728
	ctx.r4.s64 = ctx.r11.s64 + 1728;
	// addi r3,r10,-23264
	ctx.r3.s64 = ctx.r10.s64 + -23264;
	// bl 0x827d6c90
	ctx.lr = 0x825A3244;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,1824
	ctx.r4.s64 = ctx.r11.s64 + 1824;
	// addi r3,r10,-23284
	ctx.r3.s64 = ctx.r10.s64 + -23284;
	// bl 0x827d6c90
	ctx.lr = 0x825A3258;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,1928
	ctx.r4.s64 = ctx.r11.s64 + 1928;
	// addi r3,r10,-23304
	ctx.r3.s64 = ctx.r10.s64 + -23304;
	// bl 0x827d6c90
	ctx.lr = 0x825A326C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,1944
	ctx.r4.s64 = ctx.r11.s64 + 1944;
	// addi r3,r10,-23328
	ctx.r3.s64 = ctx.r10.s64 + -23328;
	// bl 0x827d6c90
	ctx.lr = 0x825A3280;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,1960
	ctx.r4.s64 = ctx.r11.s64 + 1960;
	// addi r3,r10,-23348
	ctx.r3.s64 = ctx.r10.s64 + -23348;
	// bl 0x827d6c90
	ctx.lr = 0x825A3294;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,1976
	ctx.r4.s64 = ctx.r11.s64 + 1976;
	// addi r3,r10,-23368
	ctx.r3.s64 = ctx.r10.s64 + -23368;
	// bl 0x827d6c90
	ctx.lr = 0x825A32A8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2056
	ctx.r4.s64 = ctx.r11.s64 + 2056;
	// addi r3,r10,-23392
	ctx.r3.s64 = ctx.r10.s64 + -23392;
	// bl 0x827d6c90
	ctx.lr = 0x825A32BC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2160
	ctx.r4.s64 = ctx.r11.s64 + 2160;
	// addi r3,r10,-23428
	ctx.r3.s64 = ctx.r10.s64 + -23428;
	// bl 0x827d6c90
	ctx.lr = 0x825A32D0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,11080
	ctx.r4.s64 = ctx.r11.s64 + 11080;
	// addi r3,r10,-23440
	ctx.r3.s64 = ctx.r10.s64 + -23440;
	// bl 0x827d6c90
	ctx.lr = 0x825A32E4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2256
	ctx.r4.s64 = ctx.r11.s64 + 2256;
	// addi r3,r10,-23452
	ctx.r3.s64 = ctx.r10.s64 + -23452;
	// bl 0x827d6c90
	ctx.lr = 0x825A32F8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-2368
	ctx.r4.s64 = ctx.r11.s64 + -2368;
	// addi r3,r10,-23472
	ctx.r3.s64 = ctx.r10.s64 + -23472;
	// bl 0x827d6c90
	ctx.lr = 0x825A330C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2272
	ctx.r4.s64 = ctx.r11.s64 + 2272;
	// addi r3,r10,-23492
	ctx.r3.s64 = ctx.r10.s64 + -23492;
	// bl 0x827d6c90
	ctx.lr = 0x825A3320;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2352
	ctx.r4.s64 = ctx.r11.s64 + 2352;
	// addi r3,r10,-23516
	ctx.r3.s64 = ctx.r10.s64 + -23516;
	// bl 0x827d6c90
	ctx.lr = 0x825A3334;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2448
	ctx.r4.s64 = ctx.r11.s64 + 2448;
	// addi r3,r10,-23532
	ctx.r3.s64 = ctx.r10.s64 + -23532;
	// bl 0x827d6c90
	ctx.lr = 0x825A3348;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,11192
	ctx.r4.s64 = ctx.r11.s64 + 11192;
	// addi r3,r10,-23544
	ctx.r3.s64 = ctx.r10.s64 + -23544;
	// bl 0x827d6c90
	ctx.lr = 0x825A335C;
	sub_827D6C90(ctx, base);
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// addi r3,r10,-23560
	ctx.r3.s64 = ctx.r10.s64 + -23560;
	// addi r4,r11,7112
	ctx.r4.s64 = ctx.r11.s64 + 7112;
	// bl 0x827d6c90
	ctx.lr = 0x825A3370;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2528
	ctx.r4.s64 = ctx.r11.s64 + 2528;
	// addi r3,r10,-23576
	ctx.r3.s64 = ctx.r10.s64 + -23576;
	// bl 0x827d6c90
	ctx.lr = 0x825A3384;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2600
	ctx.r4.s64 = ctx.r11.s64 + 2600;
	// addi r3,r10,-23592
	ctx.r3.s64 = ctx.r10.s64 + -23592;
	// bl 0x827d6c90
	ctx.lr = 0x825A3398;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2696
	ctx.r4.s64 = ctx.r11.s64 + 2696;
	// addi r3,r10,-23612
	ctx.r3.s64 = ctx.r10.s64 + -23612;
	// bl 0x827d6c90
	ctx.lr = 0x825A33AC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2792
	ctx.r4.s64 = ctx.r11.s64 + 2792;
	// addi r3,r10,-23632
	ctx.r3.s64 = ctx.r10.s64 + -23632;
	// bl 0x827d6c90
	ctx.lr = 0x825A33C0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2872
	ctx.r4.s64 = ctx.r11.s64 + 2872;
	// addi r3,r10,-23656
	ctx.r3.s64 = ctx.r10.s64 + -23656;
	// bl 0x827d6c90
	ctx.lr = 0x825A33D4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2952
	ctx.r4.s64 = ctx.r11.s64 + 2952;
	// addi r3,r10,-23684
	ctx.r3.s64 = ctx.r10.s64 + -23684;
	// bl 0x827d6c90
	ctx.lr = 0x825A33E8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,3032
	ctx.r4.s64 = ctx.r11.s64 + 3032;
	// addi r3,r10,-23708
	ctx.r3.s64 = ctx.r10.s64 + -23708;
	// bl 0x827d6c90
	ctx.lr = 0x825A33FC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,3112
	ctx.r4.s64 = ctx.r11.s64 + 3112;
	// addi r3,r10,-23740
	ctx.r3.s64 = ctx.r10.s64 + -23740;
	// bl 0x827d6c90
	ctx.lr = 0x825A3410;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-2304
	ctx.r4.s64 = ctx.r11.s64 + -2304;
	// addi r3,r10,-23776
	ctx.r3.s64 = ctx.r10.s64 + -23776;
	// bl 0x827d6c90
	ctx.lr = 0x825A3424;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,3208
	ctx.r4.s64 = ctx.r11.s64 + 3208;
	// addi r3,r10,-23812
	ctx.r3.s64 = ctx.r10.s64 + -23812;
	// bl 0x827d6c90
	ctx.lr = 0x825A3438;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,3280
	ctx.r4.s64 = ctx.r11.s64 + 3280;
	// addi r3,r10,-23840
	ctx.r3.s64 = ctx.r10.s64 + -23840;
	// bl 0x827d6c90
	ctx.lr = 0x825A344C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,3368
	ctx.r4.s64 = ctx.r11.s64 + 3368;
	// addi r3,r10,-23872
	ctx.r3.s64 = ctx.r10.s64 + -23872;
	// bl 0x827d6c90
	ctx.lr = 0x825A3460;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,3456
	ctx.r4.s64 = ctx.r11.s64 + 3456;
	// addi r3,r10,-23912
	ctx.r3.s64 = ctx.r10.s64 + -23912;
	// bl 0x827d6c90
	ctx.lr = 0x825A3474;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,3528
	ctx.r4.s64 = ctx.r11.s64 + 3528;
	// addi r3,r10,-23952
	ctx.r3.s64 = ctx.r10.s64 + -23952;
	// bl 0x827d6c90
	ctx.lr = 0x825A3488;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,3600
	ctx.r4.s64 = ctx.r11.s64 + 3600;
	// addi r3,r10,-23976
	ctx.r3.s64 = ctx.r10.s64 + -23976;
	// bl 0x827d6c90
	ctx.lr = 0x825A349C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,3696
	ctx.r4.s64 = ctx.r11.s64 + 3696;
	// addi r3,r10,-24008
	ctx.r3.s64 = ctx.r10.s64 + -24008;
	// bl 0x827d6c90
	ctx.lr = 0x825A34B0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,11304
	ctx.r4.s64 = ctx.r11.s64 + 11304;
	// addi r3,r10,-24024
	ctx.r3.s64 = ctx.r10.s64 + -24024;
	// bl 0x827d6c90
	ctx.lr = 0x825A34C4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,3768
	ctx.r4.s64 = ctx.r11.s64 + 3768;
	// addi r3,r10,-24044
	ctx.r3.s64 = ctx.r10.s64 + -24044;
	// bl 0x827d6c90
	ctx.lr = 0x825A34D8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,3872
	ctx.r4.s64 = ctx.r11.s64 + 3872;
	// addi r3,r10,-24060
	ctx.r3.s64 = ctx.r10.s64 + -24060;
	// bl 0x827d6c90
	ctx.lr = 0x825A34EC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,3960
	ctx.r4.s64 = ctx.r11.s64 + 3960;
	// addi r3,r10,-24088
	ctx.r3.s64 = ctx.r10.s64 + -24088;
	// bl 0x827d6c90
	ctx.lr = 0x825A3500;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,4056
	ctx.r4.s64 = ctx.r11.s64 + 4056;
	// addi r3,r10,-24108
	ctx.r3.s64 = ctx.r10.s64 + -24108;
	// bl 0x827d6c90
	ctx.lr = 0x825A3514;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,4152
	ctx.r4.s64 = ctx.r11.s64 + 4152;
	// addi r3,r10,-24132
	ctx.r3.s64 = ctx.r10.s64 + -24132;
	// bl 0x827d6c90
	ctx.lr = 0x825A3528;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,4232
	ctx.r4.s64 = ctx.r11.s64 + 4232;
	// addi r3,r10,-24160
	ctx.r3.s64 = ctx.r10.s64 + -24160;
	// bl 0x827d6c90
	ctx.lr = 0x825A353C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,4304
	ctx.r4.s64 = ctx.r11.s64 + 4304;
	// addi r3,r10,-24184
	ctx.r3.s64 = ctx.r10.s64 + -24184;
	// bl 0x827d6c90
	ctx.lr = 0x825A3550;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,4384
	ctx.r4.s64 = ctx.r11.s64 + 4384;
	// addi r3,r10,-24204
	ctx.r3.s64 = ctx.r10.s64 + -24204;
	// bl 0x827d6c90
	ctx.lr = 0x825A3564;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,4464
	ctx.r4.s64 = ctx.r11.s64 + 4464;
	// addi r3,r10,-24228
	ctx.r3.s64 = ctx.r10.s64 + -24228;
	// bl 0x827d6c90
	ctx.lr = 0x825A3578;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,4560
	ctx.r4.s64 = ctx.r11.s64 + 4560;
	// addi r3,r10,-24260
	ctx.r3.s64 = ctx.r10.s64 + -24260;
	// bl 0x827d6c90
	ctx.lr = 0x825A358C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,4576
	ctx.r4.s64 = ctx.r11.s64 + 4576;
	// addi r3,r10,-24276
	ctx.r3.s64 = ctx.r10.s64 + -24276;
	// bl 0x827d6c90
	ctx.lr = 0x825A35A0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,11416
	ctx.r4.s64 = ctx.r11.s64 + 11416;
	// addi r3,r10,-24292
	ctx.r3.s64 = ctx.r10.s64 + -24292;
	// bl 0x827d6c90
	ctx.lr = 0x825A35B4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,11440
	ctx.r4.s64 = ctx.r11.s64 + 11440;
	// addi r3,r10,-24308
	ctx.r3.s64 = ctx.r10.s64 + -24308;
	// bl 0x827d6c90
	ctx.lr = 0x825A35C8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,11464
	ctx.r4.s64 = ctx.r11.s64 + 11464;
	// addi r3,r10,-24324
	ctx.r3.s64 = ctx.r10.s64 + -24324;
	// bl 0x827d6c90
	ctx.lr = 0x825A35DC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,11488
	ctx.r4.s64 = ctx.r11.s64 + 11488;
	// addi r3,r10,-24344
	ctx.r3.s64 = ctx.r10.s64 + -24344;
	// bl 0x827d6c90
	ctx.lr = 0x825A35F0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,11512
	ctx.r4.s64 = ctx.r11.s64 + 11512;
	// addi r3,r10,-24360
	ctx.r3.s64 = ctx.r10.s64 + -24360;
	// bl 0x827d6c90
	ctx.lr = 0x825A3604;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,11536
	ctx.r4.s64 = ctx.r11.s64 + 11536;
	// addi r3,r10,-24376
	ctx.r3.s64 = ctx.r10.s64 + -24376;
	// bl 0x827d6c90
	ctx.lr = 0x825A3618;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,11560
	ctx.r4.s64 = ctx.r11.s64 + 11560;
	// addi r3,r10,-24396
	ctx.r3.s64 = ctx.r10.s64 + -24396;
	// bl 0x827d6c90
	ctx.lr = 0x825A362C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,11624
	ctx.r4.s64 = ctx.r11.s64 + 11624;
	// addi r3,r10,-24416
	ctx.r3.s64 = ctx.r10.s64 + -24416;
	// bl 0x827d6c90
	ctx.lr = 0x825A3640;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,11648
	ctx.r4.s64 = ctx.r11.s64 + 11648;
	// addi r3,r10,-24436
	ctx.r3.s64 = ctx.r10.s64 + -24436;
	// bl 0x827d6c90
	ctx.lr = 0x825A3654;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,4592
	ctx.r4.s64 = ctx.r11.s64 + 4592;
	// addi r3,r10,-24456
	ctx.r3.s64 = ctx.r10.s64 + -24456;
	// bl 0x827d6c90
	ctx.lr = 0x825A3668;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,4672
	ctx.r4.s64 = ctx.r11.s64 + 4672;
	// addi r3,r10,-24476
	ctx.r3.s64 = ctx.r10.s64 + -24476;
	// bl 0x827d6c90
	ctx.lr = 0x825A367C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,4744
	ctx.r4.s64 = ctx.r11.s64 + 4744;
	// addi r3,r10,-24496
	ctx.r3.s64 = ctx.r10.s64 + -24496;
	// bl 0x827d6c90
	ctx.lr = 0x825A3690;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,4816
	ctx.r4.s64 = ctx.r11.s64 + 4816;
	// addi r3,r10,-24524
	ctx.r3.s64 = ctx.r10.s64 + -24524;
	// bl 0x827d6c90
	ctx.lr = 0x825A36A4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,11672
	ctx.r4.s64 = ctx.r11.s64 + 11672;
	// addi r3,r10,-24552
	ctx.r3.s64 = ctx.r10.s64 + -24552;
	// bl 0x827d6c90
	ctx.lr = 0x825A36B8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,4896
	ctx.r4.s64 = ctx.r11.s64 + 4896;
	// addi r3,r10,-24572
	ctx.r3.s64 = ctx.r10.s64 + -24572;
	// bl 0x827d6c90
	ctx.lr = 0x825A36CC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-2240
	ctx.r4.s64 = ctx.r11.s64 + -2240;
	// addi r3,r10,-24592
	ctx.r3.s64 = ctx.r10.s64 + -24592;
	// bl 0x827d6c90
	ctx.lr = 0x825A36E0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,4968
	ctx.r4.s64 = ctx.r11.s64 + 4968;
	// addi r3,r10,-24616
	ctx.r3.s64 = ctx.r10.s64 + -24616;
	// bl 0x827d6c90
	ctx.lr = 0x825A36F4;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,-24644
	ctx.r3.s64 = ctx.r10.s64 + -24644;
	// bl 0x827d6c90
	ctx.lr = 0x825A3708;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,-24680
	ctx.r3.s64 = ctx.r10.s64 + -24680;
	// bl 0x827d6c90
	ctx.lr = 0x825A371C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,11696
	ctx.r4.s64 = ctx.r11.s64 + 11696;
	// addi r3,r10,-24712
	ctx.r3.s64 = ctx.r10.s64 + -24712;
	// bl 0x827d6c90
	ctx.lr = 0x825A3730;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,5072
	ctx.r4.s64 = ctx.r11.s64 + 5072;
	// addi r3,r10,-24744
	ctx.r3.s64 = ctx.r10.s64 + -24744;
	// bl 0x827d6c90
	ctx.lr = 0x825A3744;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,5096
	ctx.r4.s64 = ctx.r11.s64 + 5096;
	// addi r3,r10,-24768
	ctx.r3.s64 = ctx.r10.s64 + -24768;
	// bl 0x827d6c90
	ctx.lr = 0x825A3758;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,11768
	ctx.r4.s64 = ctx.r11.s64 + 11768;
	// addi r3,r10,-24800
	ctx.r3.s64 = ctx.r10.s64 + -24800;
	// bl 0x827d6c90
	ctx.lr = 0x825A376C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,5120
	ctx.r4.s64 = ctx.r11.s64 + 5120;
	// addi r3,r10,-24812
	ctx.r3.s64 = ctx.r10.s64 + -24812;
	// bl 0x827d6c90
	ctx.lr = 0x825A3780;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,5208
	ctx.r4.s64 = ctx.r11.s64 + 5208;
	// addi r3,r10,-24828
	ctx.r3.s64 = ctx.r10.s64 + -24828;
	// bl 0x827d6c90
	ctx.lr = 0x825A3794;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,5272
	ctx.r4.s64 = ctx.r11.s64 + 5272;
	// addi r3,r10,-24852
	ctx.r3.s64 = ctx.r10.s64 + -24852;
	// bl 0x827d6c90
	ctx.lr = 0x825A37A8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,5360
	ctx.r4.s64 = ctx.r11.s64 + 5360;
	// addi r3,r10,-24884
	ctx.r3.s64 = ctx.r10.s64 + -24884;
	// bl 0x827d6c90
	ctx.lr = 0x825A37BC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,5448
	ctx.r4.s64 = ctx.r11.s64 + 5448;
	// addi r3,r10,-24916
	ctx.r3.s64 = ctx.r10.s64 + -24916;
	// bl 0x827d6c90
	ctx.lr = 0x825A37D0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,5512
	ctx.r4.s64 = ctx.r11.s64 + 5512;
	// addi r3,r10,-24936
	ctx.r3.s64 = ctx.r10.s64 + -24936;
	// bl 0x827d6c90
	ctx.lr = 0x825A37E4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,5576
	ctx.r4.s64 = ctx.r11.s64 + 5576;
	// addi r3,r10,-24956
	ctx.r3.s64 = ctx.r10.s64 + -24956;
	// bl 0x827d6c90
	ctx.lr = 0x825A37F8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,5640
	ctx.r4.s64 = ctx.r11.s64 + 5640;
	// addi r3,r10,-24976
	ctx.r3.s64 = ctx.r10.s64 + -24976;
	// bl 0x827d6c90
	ctx.lr = 0x825A380C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,5704
	ctx.r4.s64 = ctx.r11.s64 + 5704;
	// addi r3,r10,-25000
	ctx.r3.s64 = ctx.r10.s64 + -25000;
	// bl 0x827d6c90
	ctx.lr = 0x825A3820;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,5768
	ctx.r4.s64 = ctx.r11.s64 + 5768;
	// addi r3,r10,-25020
	ctx.r3.s64 = ctx.r10.s64 + -25020;
	// bl 0x827d6c90
	ctx.lr = 0x825A3834;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,5832
	ctx.r4.s64 = ctx.r11.s64 + 5832;
	// addi r3,r10,-25040
	ctx.r3.s64 = ctx.r10.s64 + -25040;
	// bl 0x827d6c90
	ctx.lr = 0x825A3848;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,5904
	ctx.r4.s64 = ctx.r11.s64 + 5904;
	// addi r3,r10,-25056
	ctx.r3.s64 = ctx.r10.s64 + -25056;
	// bl 0x827d6c90
	ctx.lr = 0x825A385C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,5976
	ctx.r4.s64 = ctx.r11.s64 + 5976;
	// addi r3,r10,-25068
	ctx.r3.s64 = ctx.r10.s64 + -25068;
	// bl 0x827d6c90
	ctx.lr = 0x825A3870;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// addi r4,r11,6048
	ctx.r4.s64 = ctx.r11.s64 + 6048;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r3,r11,-25092
	ctx.r3.s64 = ctx.r11.s64 + -25092;
	// bl 0x827d6c90
	ctx.lr = 0x825A3884;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,6112
	ctx.r4.s64 = ctx.r11.s64 + 6112;
	// addi r3,r10,-25112
	ctx.r3.s64 = ctx.r10.s64 + -25112;
	// bl 0x827d6c90
	ctx.lr = 0x825A3898;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,6184
	ctx.r4.s64 = ctx.r11.s64 + 6184;
	// addi r3,r10,-25132
	ctx.r3.s64 = ctx.r10.s64 + -25132;
	// bl 0x827d6c90
	ctx.lr = 0x825A38AC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,6256
	ctx.r4.s64 = ctx.r11.s64 + 6256;
	// addi r3,r10,-25160
	ctx.r3.s64 = ctx.r10.s64 + -25160;
	// bl 0x827d6c90
	ctx.lr = 0x825A38C0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,6328
	ctx.r4.s64 = ctx.r11.s64 + 6328;
	// addi r3,r10,-25188
	ctx.r3.s64 = ctx.r10.s64 + -25188;
	// bl 0x827d6c90
	ctx.lr = 0x825A38D4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,6400
	ctx.r4.s64 = ctx.r11.s64 + 6400;
	// addi r3,r10,-25208
	ctx.r3.s64 = ctx.r10.s64 + -25208;
	// bl 0x827d6c90
	ctx.lr = 0x825A38E8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,6416
	ctx.r4.s64 = ctx.r11.s64 + 6416;
	// addi r3,r10,-25232
	ctx.r3.s64 = ctx.r10.s64 + -25232;
	// bl 0x827d6c90
	ctx.lr = 0x825A38FC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,11832
	ctx.r4.s64 = ctx.r11.s64 + 11832;
	// addi r3,r10,-25256
	ctx.r3.s64 = ctx.r10.s64 + -25256;
	// bl 0x827d6c90
	ctx.lr = 0x825A3910;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,11864
	ctx.r4.s64 = ctx.r11.s64 + 11864;
	// addi r3,r10,-25280
	ctx.r3.s64 = ctx.r10.s64 + -25280;
	// bl 0x827d6c90
	ctx.lr = 0x825A3924;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,11896
	ctx.r4.s64 = ctx.r11.s64 + 11896;
	// addi r3,r10,-25304
	ctx.r3.s64 = ctx.r10.s64 + -25304;
	// bl 0x827d6c90
	ctx.lr = 0x825A3938;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,6432
	ctx.r4.s64 = ctx.r11.s64 + 6432;
	// addi r3,r10,-25332
	ctx.r3.s64 = ctx.r10.s64 + -25332;
	// bl 0x827d6c90
	ctx.lr = 0x825A394C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,6432
	ctx.r4.s64 = ctx.r11.s64 + 6432;
	// addi r3,r10,-25360
	ctx.r3.s64 = ctx.r10.s64 + -25360;
	// bl 0x827d6c90
	ctx.lr = 0x825A3960;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,6512
	ctx.r4.s64 = ctx.r11.s64 + 6512;
	// addi r3,r10,-25388
	ctx.r3.s64 = ctx.r10.s64 + -25388;
	// bl 0x827d6c90
	ctx.lr = 0x825A3974;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,6592
	ctx.r4.s64 = ctx.r11.s64 + 6592;
	// addi r3,r10,-25416
	ctx.r3.s64 = ctx.r10.s64 + -25416;
	// bl 0x827d6c90
	ctx.lr = 0x825A3988;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,6672
	ctx.r4.s64 = ctx.r11.s64 + 6672;
	// addi r3,r10,-25432
	ctx.r3.s64 = ctx.r10.s64 + -25432;
	// bl 0x827d6c90
	ctx.lr = 0x825A399C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,6744
	ctx.r4.s64 = ctx.r11.s64 + 6744;
	// addi r3,r10,-25452
	ctx.r3.s64 = ctx.r10.s64 + -25452;
	// bl 0x827d6c90
	ctx.lr = 0x825A39B0;
	sub_827D6C90(ctx, base);
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// addi r3,r10,-25472
	ctx.r3.s64 = ctx.r10.s64 + -25472;
	// addi r4,r11,6816
	ctx.r4.s64 = ctx.r11.s64 + 6816;
	// bl 0x827d6c90
	ctx.lr = 0x825A39C4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-2224
	ctx.r4.s64 = ctx.r11.s64 + -2224;
	// addi r3,r10,-25504
	ctx.r3.s64 = ctx.r10.s64 + -25504;
	// bl 0x827d6c90
	ctx.lr = 0x825A39D8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,6888
	ctx.r4.s64 = ctx.r11.s64 + 6888;
	// addi r3,r10,-25524
	ctx.r3.s64 = ctx.r10.s64 + -25524;
	// bl 0x827d6c90
	ctx.lr = 0x825A39EC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,6952
	ctx.r4.s64 = ctx.r11.s64 + 6952;
	// addi r3,r10,-25564
	ctx.r3.s64 = ctx.r10.s64 + -25564;
	// bl 0x827d6c90
	ctx.lr = 0x825A3A00;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,7032
	ctx.r4.s64 = ctx.r11.s64 + 7032;
	// addi r3,r10,-25580
	ctx.r3.s64 = ctx.r10.s64 + -25580;
	// bl 0x827d6c90
	ctx.lr = 0x825A3A14;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,11928
	ctx.r4.s64 = ctx.r11.s64 + 11928;
	// addi r3,r10,-25608
	ctx.r3.s64 = ctx.r10.s64 + -25608;
	// bl 0x827d6c90
	ctx.lr = 0x825A3A28;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,7112
	ctx.r4.s64 = ctx.r11.s64 + 7112;
	// addi r3,r10,-25628
	ctx.r3.s64 = ctx.r10.s64 + -25628;
	// bl 0x827d6c90
	ctx.lr = 0x825A3A3C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,7136
	ctx.r4.s64 = ctx.r11.s64 + 7136;
	// addi r3,r10,-25668
	ctx.r3.s64 = ctx.r10.s64 + -25668;
	// bl 0x827d6c90
	ctx.lr = 0x825A3A50;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,7176
	ctx.r4.s64 = ctx.r11.s64 + 7176;
	// addi r3,r10,-25704
	ctx.r3.s64 = ctx.r10.s64 + -25704;
	// bl 0x827d6c90
	ctx.lr = 0x825A3A64;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,7216
	ctx.r4.s64 = ctx.r11.s64 + 7216;
	// addi r3,r10,-25736
	ctx.r3.s64 = ctx.r10.s64 + -25736;
	// bl 0x827d6c90
	ctx.lr = 0x825A3A78;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,7304
	ctx.r4.s64 = ctx.r11.s64 + 7304;
	// addi r3,r10,-25756
	ctx.r3.s64 = ctx.r10.s64 + -25756;
	// bl 0x827d6c90
	ctx.lr = 0x825A3A8C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,7368
	ctx.r4.s64 = ctx.r11.s64 + 7368;
	// addi r3,r10,-25776
	ctx.r3.s64 = ctx.r10.s64 + -25776;
	// bl 0x827d6c90
	ctx.lr = 0x825A3AA0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,7456
	ctx.r4.s64 = ctx.r11.s64 + 7456;
	// addi r3,r10,-25800
	ctx.r3.s64 = ctx.r10.s64 + -25800;
	// bl 0x827d6c90
	ctx.lr = 0x825A3AB4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,7528
	ctx.r4.s64 = ctx.r11.s64 + 7528;
	// addi r3,r10,-25824
	ctx.r3.s64 = ctx.r10.s64 + -25824;
	// bl 0x827d6c90
	ctx.lr = 0x825A3AC8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,7600
	ctx.r4.s64 = ctx.r11.s64 + 7600;
	// addi r3,r10,-25844
	ctx.r3.s64 = ctx.r10.s64 + -25844;
	// bl 0x827d6c90
	ctx.lr = 0x825A3ADC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,7672
	ctx.r4.s64 = ctx.r11.s64 + 7672;
	// addi r3,r10,-25848
	ctx.r3.s64 = ctx.r10.s64 + -25848;
	// bl 0x827d6c90
	ctx.lr = 0x825A3AF0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,7752
	ctx.r4.s64 = ctx.r11.s64 + 7752;
	// addi r3,r10,-25880
	ctx.r3.s64 = ctx.r10.s64 + -25880;
	// bl 0x827d6c90
	ctx.lr = 0x825A3B04;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,7840
	ctx.r4.s64 = ctx.r11.s64 + 7840;
	// addi r3,r10,-25912
	ctx.r3.s64 = ctx.r10.s64 + -25912;
	// bl 0x827d6c90
	ctx.lr = 0x825A3B18;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,7928
	ctx.r4.s64 = ctx.r11.s64 + 7928;
	// addi r3,r10,-25928
	ctx.r3.s64 = ctx.r10.s64 + -25928;
	// bl 0x827d6c90
	ctx.lr = 0x825A3B2C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,8000
	ctx.r4.s64 = ctx.r11.s64 + 8000;
	// addi r3,r10,-25952
	ctx.r3.s64 = ctx.r10.s64 + -25952;
	// bl 0x827d6c90
	ctx.lr = 0x825A3B40;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,8096
	ctx.r4.s64 = ctx.r11.s64 + 8096;
	// addi r3,r10,-25968
	ctx.r3.s64 = ctx.r10.s64 + -25968;
	// bl 0x827d6c90
	ctx.lr = 0x825A3B54;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,8176
	ctx.r4.s64 = ctx.r11.s64 + 8176;
	// addi r3,r10,-25980
	ctx.r3.s64 = ctx.r10.s64 + -25980;
	// bl 0x827d6c90
	ctx.lr = 0x825A3B68;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,12024
	ctx.r4.s64 = ctx.r11.s64 + 12024;
	// addi r3,r10,-26012
	ctx.r3.s64 = ctx.r10.s64 + -26012;
	// bl 0x827d6c90
	ctx.lr = 0x825A3B7C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,8248
	ctx.r4.s64 = ctx.r11.s64 + 8248;
	// addi r3,r10,-26044
	ctx.r3.s64 = ctx.r10.s64 + -26044;
	// bl 0x827d6c90
	ctx.lr = 0x825A3B90;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,8376
	ctx.r4.s64 = ctx.r11.s64 + 8376;
	// addi r3,r10,-26076
	ctx.r3.s64 = ctx.r10.s64 + -26076;
	// bl 0x827d6c90
	ctx.lr = 0x825A3BA4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,12112
	ctx.r4.s64 = ctx.r11.s64 + 12112;
	// addi r3,r10,-26108
	ctx.r3.s64 = ctx.r10.s64 + -26108;
	// bl 0x827d6c90
	ctx.lr = 0x825A3BB8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,8448
	ctx.r4.s64 = ctx.r11.s64 + 8448;
	// addi r3,r10,-26144
	ctx.r3.s64 = ctx.r10.s64 + -26144;
	// bl 0x827d6c90
	ctx.lr = 0x825A3BCC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,8512
	ctx.r4.s64 = ctx.r11.s64 + 8512;
	// addi r3,r10,-26172
	ctx.r3.s64 = ctx.r10.s64 + -26172;
	// bl 0x827d6c90
	ctx.lr = 0x825A3BE0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,8600
	ctx.r4.s64 = ctx.r11.s64 + 8600;
	// addi r3,r10,-26204
	ctx.r3.s64 = ctx.r10.s64 + -26204;
	// bl 0x827d6c90
	ctx.lr = 0x825A3BF4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,8696
	ctx.r4.s64 = ctx.r11.s64 + 8696;
	// addi r3,r10,-26228
	ctx.r3.s64 = ctx.r10.s64 + -26228;
	// bl 0x827d6c90
	ctx.lr = 0x825A3C08;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,8760
	ctx.r4.s64 = ctx.r11.s64 + 8760;
	// addi r3,r10,-26280
	ctx.r3.s64 = ctx.r10.s64 + -26280;
	// bl 0x827d6c90
	ctx.lr = 0x825A3C1C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,8848
	ctx.r4.s64 = ctx.r11.s64 + 8848;
	// addi r3,r10,-26296
	ctx.r3.s64 = ctx.r10.s64 + -26296;
	// bl 0x827d6c90
	ctx.lr = 0x825A3C30;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,8928
	ctx.r4.s64 = ctx.r11.s64 + 8928;
	// addi r3,r10,-26312
	ctx.r3.s64 = ctx.r10.s64 + -26312;
	// bl 0x827d6c90
	ctx.lr = 0x825A3C44;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-2160
	ctx.r4.s64 = ctx.r11.s64 + -2160;
	// addi r3,r10,-26336
	ctx.r3.s64 = ctx.r10.s64 + -26336;
	// bl 0x827d6c90
	ctx.lr = 0x825A3C58;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,9008
	ctx.r4.s64 = ctx.r11.s64 + 9008;
	// addi r3,r10,-26352
	ctx.r3.s64 = ctx.r10.s64 + -26352;
	// bl 0x827d6c90
	ctx.lr = 0x825A3C6C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,9080
	ctx.r4.s64 = ctx.r11.s64 + 9080;
	// addi r3,r10,-26376
	ctx.r3.s64 = ctx.r10.s64 + -26376;
	// bl 0x827d6c90
	ctx.lr = 0x825A3C80;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-2096
	ctx.r4.s64 = ctx.r11.s64 + -2096;
	// addi r3,r10,-26408
	ctx.r3.s64 = ctx.r10.s64 + -26408;
	// bl 0x827d6c90
	ctx.lr = 0x825A3C94;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,9152
	ctx.r4.s64 = ctx.r11.s64 + 9152;
	// addi r3,r10,-26428
	ctx.r3.s64 = ctx.r10.s64 + -26428;
	// bl 0x827d6c90
	ctx.lr = 0x825A3CA8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,9232
	ctx.r4.s64 = ctx.r11.s64 + 9232;
	// addi r3,r10,-26444
	ctx.r3.s64 = ctx.r10.s64 + -26444;
	// bl 0x827d6c90
	ctx.lr = 0x825A3CBC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-2032
	ctx.r4.s64 = ctx.r11.s64 + -2032;
	// addi r3,r10,-26480
	ctx.r3.s64 = ctx.r10.s64 + -26480;
	// bl 0x827d6c90
	ctx.lr = 0x825A3CD0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,9304
	ctx.r4.s64 = ctx.r11.s64 + 9304;
	// addi r3,r10,-26516
	ctx.r3.s64 = ctx.r10.s64 + -26516;
	// bl 0x827d6c90
	ctx.lr = 0x825A3CE4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,9376
	ctx.r4.s64 = ctx.r11.s64 + 9376;
	// addi r3,r10,-26540
	ctx.r3.s64 = ctx.r10.s64 + -26540;
	// bl 0x827d6c90
	ctx.lr = 0x825A3CF8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,9464
	ctx.r4.s64 = ctx.r11.s64 + 9464;
	// addi r3,r10,-26560
	ctx.r3.s64 = ctx.r10.s64 + -26560;
	// bl 0x827d6c90
	ctx.lr = 0x825A3D0C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,9496
	ctx.r4.s64 = ctx.r11.s64 + 9496;
	// addi r3,r10,-26592
	ctx.r3.s64 = ctx.r10.s64 + -26592;
	// bl 0x827d6c90
	ctx.lr = 0x825A3D20;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,9584
	ctx.r4.s64 = ctx.r11.s64 + 9584;
	// addi r3,r10,-26616
	ctx.r3.s64 = ctx.r10.s64 + -26616;
	// bl 0x827d6c90
	ctx.lr = 0x825A3D34;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,9672
	ctx.r4.s64 = ctx.r11.s64 + 9672;
	// addi r3,r10,-26664
	ctx.r3.s64 = ctx.r10.s64 + -26664;
	// bl 0x827d6c90
	ctx.lr = 0x825A3D48;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,9712
	ctx.r4.s64 = ctx.r11.s64 + 9712;
	// addi r3,r10,-26692
	ctx.r3.s64 = ctx.r10.s64 + -26692;
	// bl 0x827d6c90
	ctx.lr = 0x825A3D5C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,9744
	ctx.r4.s64 = ctx.r11.s64 + 9744;
	// addi r3,r10,-26708
	ctx.r3.s64 = ctx.r10.s64 + -26708;
	// bl 0x827d6c90
	ctx.lr = 0x825A3D70;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-1968
	ctx.r4.s64 = ctx.r11.s64 + -1968;
	// addi r3,r10,-26720
	ctx.r3.s64 = ctx.r10.s64 + -26720;
	// bl 0x827d6c90
	ctx.lr = 0x825A3D84;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,-1904
	ctx.r4.s64 = ctx.r11.s64 + -1904;
	// addi r3,r10,-26740
	ctx.r3.s64 = ctx.r10.s64 + -26740;
	// bl 0x827d6c90
	ctx.lr = 0x825A3D98;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,9808
	ctx.r4.s64 = ctx.r11.s64 + 9808;
	// addi r3,r10,-26760
	ctx.r3.s64 = ctx.r10.s64 + -26760;
	// bl 0x827d6c90
	ctx.lr = 0x825A3DAC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,9904
	ctx.r4.s64 = ctx.r11.s64 + 9904;
	// addi r3,r10,-26784
	ctx.r3.s64 = ctx.r10.s64 + -26784;
	// bl 0x827d6c90
	ctx.lr = 0x825A3DC0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,10008
	ctx.r4.s64 = ctx.r11.s64 + 10008;
	// addi r3,r10,-26800
	ctx.r3.s64 = ctx.r10.s64 + -26800;
	// bl 0x827d6c90
	ctx.lr = 0x825A3DD4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,10112
	ctx.r4.s64 = ctx.r11.s64 + 10112;
	// addi r3,r10,-26828
	ctx.r3.s64 = ctx.r10.s64 + -26828;
	// bl 0x827d6c90
	ctx.lr = 0x825A3DE8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,10128
	ctx.r4.s64 = ctx.r11.s64 + 10128;
	// addi r3,r10,-26852
	ctx.r3.s64 = ctx.r10.s64 + -26852;
	// bl 0x827d6c90
	ctx.lr = 0x825A3DFC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,10144
	ctx.r4.s64 = ctx.r11.s64 + 10144;
	// addi r3,r10,-26876
	ctx.r3.s64 = ctx.r10.s64 + -26876;
	// bl 0x827d6c90
	ctx.lr = 0x825A3E10;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,10160
	ctx.r4.s64 = ctx.r11.s64 + 10160;
	// addi r3,r10,-26908
	ctx.r3.s64 = ctx.r10.s64 + -26908;
	// bl 0x827d6c90
	ctx.lr = 0x825A3E24;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,10232
	ctx.r4.s64 = ctx.r11.s64 + 10232;
	// addi r3,r10,-26928
	ctx.r3.s64 = ctx.r10.s64 + -26928;
	// bl 0x827d6c90
	ctx.lr = 0x825A3E38;
	sub_827D6C90(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A3E48);  /* forward decl */
PPC_WEAK_FUNC(sub_825A3E48) { __imp__sub_825A3E48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A3E48) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r6,r4
	ctx.r6.u64 = ctx.r4.u64;
	// mr r7,r5
	ctx.r7.u64 = ctx.r5.u64;
	// li r10,0
	ctx.r10.s64 = 0;
	// li r8,9
	ctx.r8.s64 = 9;
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// bne cr6,0x825a3e78
	if (!ctx.cr6.eq) goto loc_825A3E78;
	// li r8,2
	ctx.r8.s64 = 2;
	// b 0x825a3e98
	goto loc_825A3E98;
loc_825A3E78:
	// cmpwi cr6,r3,1
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 1, ctx.xer);
	// bne cr6,0x825a3e8c
	if (!ctx.cr6.eq) goto loc_825A3E8C;
	// li r8,0
	ctx.r8.s64 = 0;
	// li r10,5
	ctx.r10.s64 = 5;
	// b 0x825a3e98
	goto loc_825A3E98;
loc_825A3E8C:
	// cmpwi cr6,r3,2
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 2, ctx.xer);
	// bne cr6,0x825a3e98
	if (!ctx.cr6.eq) goto loc_825A3E98;
	// li r10,4
	ctx.r10.s64 = 4;
loc_825A3E98:
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// lis r31,-32005
	ctx.r31.s64 = -2097479680;
	// addi r11,r11,17696
	ctx.r11.s64 = ctx.r11.s64 + 17696;
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
	// clrlwi r9,r10,24
	ctx.r9.u64 = ctx.r10.u32 & 0xFF;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// lwz r4,-9216(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + -9216);
	// bl 0x825fda50
	ctx.lr = 0x825A3EB8;
	sub_825FDA50(ctx, base);
	// lwz r11,-9216(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + -9216);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// stw r11,-9216(r31)
	PPC_STORE_U32(ctx.r31.u32 + -9216, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A3ED8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A3ED8) { __imp__sub_825A3ED8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A3ED8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// li r4,0
	ctx.r4.s64 = 0;
	// b 0x825fd908
	sub_825FD908(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A3EF0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A3EF0) { __imp__sub_825A3EF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A3EF0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// b 0x825fd878
	sub_825FD878(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A3F00);  /* forward decl */
PPC_WEAK_FUNC(sub_825A3F00) { __imp__sub_825A3F00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A3F00) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// b 0x825fd948
	sub_825FD948(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A3F10);  /* forward decl */
PPC_WEAK_FUNC(sub_825A3F10) { __imp__sub_825A3F10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A3F10) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// b 0x825fdfd8
	sub_825FDFD8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A3F20);  /* forward decl */
PPC_WEAK_FUNC(sub_825A3F20) { __imp__sub_825A3F20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A3F20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// lis r10,-32005
	ctx.r10.s64 = -2097479680;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,-9216(r10)
	PPC_STORE_U32(ctx.r10.u32 + -9216, ctx.r11.u32);
	// b 0x825fd9b8
	sub_825FD9B8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A3F38);  /* forward decl */
PPC_WEAK_FUNC(sub_825A3F38) { __imp__sub_825A3F38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A3F38) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// b 0x825fdec8
	sub_825FDEC8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A3F48);  /* forward decl */
PPC_WEAK_FUNC(sub_825A3F48) { __imp__sub_825A3F48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A3F48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// b 0x825fdf30
	sub_825FDF30(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A3F58);  /* forward decl */
PPC_WEAK_FUNC(sub_825A3F58) { __imp__sub_825A3F58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A3F58) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// b 0x825ff8f8
	sub_825FF8F8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A3F68);  /* forward decl */
PPC_WEAK_FUNC(sub_825A3F68) { __imp__sub_825A3F68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A3F68) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// b 0x825ff7e0
	sub_825FF7E0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A3F78);  /* forward decl */
PPC_WEAK_FUNC(sub_825A3F78) { __imp__sub_825A3F78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A3F78) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r3,r11,-28768
	ctx.r3.s64 = ctx.r11.s64 + -28768;
	// bl 0x82893490
	ctx.lr = 0x825A3F90;
	sub_82893490(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825a3fa0
	if (ctx.cr6.eq) goto loc_825A3FA0;
	// bl 0x8225adc0
	ctx.lr = 0x825A3FA0;
	sub_8225ADC0(ctx, base);
loc_825A3FA0:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A3FB0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A3FB0) { __imp__sub_825A3FB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A3FB0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r3,r11,-28768
	ctx.r3.s64 = ctx.r11.s64 + -28768;
	// bl 0x82893490
	ctx.lr = 0x825A3FC8;
	sub_82893490(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825a3fd8
	if (ctx.cr6.eq) goto loc_825A3FD8;
	// bl 0x8225aeb8
	ctx.lr = 0x825A3FD8;
	sub_8225AEB8(ctx, base);
loc_825A3FD8:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A3FE8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A3FE8) { __imp__sub_825A3FE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A3FE8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-28110(r10)
	PPC_STORE_U8(ctx.r10.u32 + -28110, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A3FF8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A3FF8) { __imp__sub_825A3FF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A3FF8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// li r11,0
	ctx.r11.s64 = 0;
	// stb r11,-28110(r10)
	PPC_STORE_U8(ctx.r10.u32 + -28110, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4008);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4008) { __imp__sub_825A4008(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4008) {
	PPC_FUNC_PROLOGUE();
	// b 0x825f9540
	sub_825F9540(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4010);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4010) { __imp__sub_825A4010(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4010) {
	PPC_FUNC_PROLOGUE();
	// b 0x825fa7c8
	sub_825FA7C8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4018);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4018) { __imp__sub_825A4018(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4018) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32062
	ctx.r11.s64 = -2101215232;
	// addi r3,r11,-28104
	ctx.r3.s64 = ctx.r11.s64 + -28104;
	// b 0x8225b460
	sub_8225B460(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4028);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4028) { __imp__sub_825A4028(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4028) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32062
	ctx.r11.s64 = -2101215232;
	// addi r3,r11,-28104
	ctx.r3.s64 = ctx.r11.s64 + -28104;
	// b 0x8225b4c0
	sub_8225B4C0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4038);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4038) { __imp__sub_825A4038(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4038) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32086
	ctx.r10.s64 = -2102788096;
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,15777(r10)
	PPC_STORE_U8(ctx.r10.u32 + 15777, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4048);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4048) { __imp__sub_825A4048(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4048) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32062
	ctx.r11.s64 = -2101215232;
	// addi r3,r11,-28104
	ctx.r3.s64 = ctx.r11.s64 + -28104;
	// b 0x8225ac10
	sub_8225AC10(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4058);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4058) { __imp__sub_825A4058(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4058) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32062
	ctx.r11.s64 = -2101215232;
	// addi r3,r11,-28104
	ctx.r3.s64 = ctx.r11.s64 + -28104;
	// b 0x8225a808
	sub_8225A808(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4068);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4068) { __imp__sub_825A4068(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4068) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32015
	ctx.r11.s64 = -2098135040;
	// addi r3,r11,19744
	ctx.r3.s64 = ctx.r11.s64 + 19744;
	// b 0x823ed538
	sub_823ED538(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4078);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4078) { __imp__sub_825A4078(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4078) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32015
	ctx.r11.s64 = -2098135040;
	// addi r3,r11,19744
	ctx.r3.s64 = ctx.r11.s64 + 19744;
	// b 0x823ed568
	sub_823ED568(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4088);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4088) { __imp__sub_825A4088(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4088) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r3,r11,10272
	ctx.r3.s64 = ctx.r11.s64 + 10272;
	// b 0x8252ff40
	sub_8252FF40(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4098);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4098) { __imp__sub_825A4098(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4098) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r3,r11,10272
	ctx.r3.s64 = ctx.r11.s64 + 10272;
	// b 0x82530040
	sub_82530040(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A40A8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A40A8) { __imp__sub_825A40A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A40A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r3,r11,10272
	ctx.r3.s64 = ctx.r11.s64 + 10272;
	// b 0x82530110
	sub_82530110(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A40B8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A40B8) { __imp__sub_825A40B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A40B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// b 0x825fe000
	sub_825FE000(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A40C8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A40C8) { __imp__sub_825A40C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A40C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// b 0x825fe078
	sub_825FE078(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A40D8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A40D8) { __imp__sub_825A40D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A40D8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32005
	ctx.r10.s64 = -2097479680;
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-9212(r10)
	PPC_STORE_U8(ctx.r10.u32 + -9212, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A40E8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A40E8) { __imp__sub_825A40E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A40E8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32005
	ctx.r10.s64 = -2097479680;
	// li r11,0
	ctx.r11.s64 = 0;
	// stb r11,-9212(r10)
	PPC_STORE_U8(ctx.r10.u32 + -9212, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A40F8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A40F8) { __imp__sub_825A40F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A40F8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb10
	ctx.lr = 0x825A4100;
	__savegprlr_26(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r30,r5
	ctx.r30.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// mr r26,r8
	ctx.r26.u64 = ctx.r8.u64;
	// li r5,0
	ctx.r5.s64 = 0;
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// lis r29,-31981
	ctx.r29.s64 = -2095906816;
	// beq cr6,0x825a4158
	if (ctx.cr6.eq) goto loc_825A4158;
	// lwz r11,31824(r29)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r29.u32 + 31824);
	// srawi r10,r3,8
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r3.s32 >> 8;
	// clrlwi r8,r3,24
	ctx.r8.u64 = ctx.r3.u32 & 0xFF;
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825a4154
	if (!ctx.cr6.eq) goto loc_825A4154;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r5,r11,r9
	ctx.r5.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825a4158
	goto loc_825A4158;
loc_825A4154:
	// li r5,0
	ctx.r5.s64 = 0;
loc_825A4158:
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// mr r6,r4
	ctx.r6.u64 = ctx.r4.u64;
	// addi r31,r11,17696
	ctx.r31.s64 = ctx.r11.s64 + 17696;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x825fdaa8
	ctx.lr = 0x825A4170;
	sub_825FDAA8(ctx, base);
	// mr r6,r28
	ctx.r6.u64 = ctx.r28.u64;
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,1
	ctx.r4.s64 = 1;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x825fdaa8
	ctx.lr = 0x825A4184;
	sub_825FDAA8(ctx, base);
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// beq cr6,0x825a41d0
	if (ctx.cr6.eq) goto loc_825A41D0;
	// lwz r11,31824(r29)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r29.u32 + 31824);
	// srawi r10,r30,8
	ctx.xer.ca = (ctx.r30.s32 < 0) & ((ctx.r30.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r30.s32 >> 8;
	// clrlwi r8,r30,24
	ctx.r8.u64 = ctx.r30.u32 & 0xFF;
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825a41bc
	if (!ctx.cr6.eq) goto loc_825A41BC;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r5,r11,r9
	ctx.r5.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825a41c0
	goto loc_825A41C0;
loc_825A41BC:
	// li r5,0
	ctx.r5.s64 = 0;
loc_825A41C0:
	// mr r6,r28
	ctx.r6.u64 = ctx.r28.u64;
	// li r4,2
	ctx.r4.s64 = 2;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x825fdaa8
	ctx.lr = 0x825A41D0;
	sub_825FDAA8(ctx, base);
loc_825A41D0:
	// mr r6,r26
	ctx.r6.u64 = ctx.r26.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// li r4,1
	ctx.r4.s64 = 1;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x825fee88
	ctx.lr = 0x825A41E4;
	sub_825FEE88(ctx, base);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A41F0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A41F0) { __imp__sub_825A41F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A41F0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r31,0
	ctx.r31.s64 = 0;
	// cmpwi cr6,r4,0
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 0, ctx.xer);
	// beq cr6,0x825a4244
	if (ctx.cr6.eq) goto loc_825A4244;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// srawi r10,r4,8
	ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r4.s32 >> 8;
	// clrlwi r8,r4,24
	ctx.r8.u64 = ctx.r4.u32 & 0xFF;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825a4240
	if (!ctx.cr6.eq) goto loc_825A4240;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r31,r11,r9
	ctx.r31.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825a4244
	goto loc_825A4244;
loc_825A4240:
	// li r31,0
	ctx.r31.s64 = 0;
loc_825A4244:
	// li r4,0
	ctx.r4.s64 = 0;
	// bl 0x827df490
	ctx.lr = 0x825A424C;
	sub_827DF490(ctx, base);
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r11,r11,17696
	ctx.r11.s64 = ctx.r11.s64 + 17696;
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x825fd7f8
	ctx.lr = 0x825A4264;
	sub_825FD7F8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4278);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4278) { __imp__sub_825A4278(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4278) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r31,0
	ctx.r31.s64 = 0;
	// cmpwi cr6,r4,0
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 0, ctx.xer);
	// beq cr6,0x825a42cc
	if (ctx.cr6.eq) goto loc_825A42CC;
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// srawi r10,r4,8
	ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r4.s32 >> 8;
	// clrlwi r8,r4,24
	ctx.r8.u64 = ctx.r4.u32 & 0xFF;
	// lwz r11,24544(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24544);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825a42c8
	if (!ctx.cr6.eq) goto loc_825A42C8;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r31,r11,r9
	ctx.r31.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825a42cc
	goto loc_825A42CC;
loc_825A42C8:
	// li r31,0
	ctx.r31.s64 = 0;
loc_825A42CC:
	// li r4,0
	ctx.r4.s64 = 0;
	// bl 0x827df490
	ctx.lr = 0x825A42D4;
	sub_827DF490(ctx, base);
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r11,r11,17696
	ctx.r11.s64 = ctx.r11.s64 + 17696;
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x825fd7f8
	ctx.lr = 0x825A42EC;
	sub_825FD7F8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4300);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4300) { __imp__sub_825A4300(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4300) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r31,0
	ctx.r31.s64 = 0;
	// cmpwi cr6,r4,0
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 0, ctx.xer);
	// beq cr6,0x825a4354
	if (ctx.cr6.eq) goto loc_825A4354;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// srawi r10,r4,8
	ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r4.s32 >> 8;
	// clrlwi r8,r4,24
	ctx.r8.u64 = ctx.r4.u32 & 0xFF;
	// lwz r11,88(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 88);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825a4350
	if (!ctx.cr6.eq) goto loc_825A4350;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r31,r11,r9
	ctx.r31.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825a4354
	goto loc_825A4354;
loc_825A4350:
	// li r31,0
	ctx.r31.s64 = 0;
loc_825A4354:
	// li r4,0
	ctx.r4.s64 = 0;
	// bl 0x827df490
	ctx.lr = 0x825A435C;
	sub_827DF490(ctx, base);
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r11,r11,17696
	ctx.r11.s64 = ctx.r11.s64 + 17696;
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x825fd7f8
	ctx.lr = 0x825A4374;
	sub_825FD7F8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4388);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4388) { __imp__sub_825A4388(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4388) {
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
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// beq cr6,0x825a447c
	if (ctx.cr6.eq) goto loc_825A447C;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// srawi r10,r3,8
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r3.s32 >> 8;
	// clrlwi r8,r3,24
	ctx.r8.u64 = ctx.r3.u32 & 0xFF;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825a43dc
	if (!ctx.cr6.eq) goto loc_825A43DC;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r31,r11,r9
	ctx.r31.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825a43e0
	goto loc_825A43E0;
loc_825A43DC:
	// li r31,0
	ctx.r31.s64 = 0;
loc_825A43E0:
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// stw r7,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r7.u32);
	// li r10,0
	ctx.r10.s64 = 0;
	// li r9,0
	ctx.r9.s64 = 0;
	// li r8,-1
	ctx.r8.s64 = -1;
	// li r7,0
	ctx.r7.s64 = 0;
	// lfs f1,2592(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2592);
	ctx.f1.f64 = double(temp.f32);
	// li r11,0
	ctx.r11.s64 = 0;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// addi r3,r31,1264
	ctx.r3.s64 = ctx.r31.s64 + 1264;
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// bl 0x82413388
	ctx.lr = 0x825A4410;
	sub_82413388(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
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
	// beq cr6,0x825a447c
	if (ctx.cr6.eq) goto loc_825A447C;
	// lbz r11,528(r31)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r31.u32 + 528);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825a4448
	if (!ctx.cr6.eq) goto loc_825A4448;
	// lbz r11,529(r31)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r31.u32 + 529);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// bne cr6,0x825a444c
	if (!ctx.cr6.eq) goto loc_825A444C;
loc_825A4448:
	// li r11,0
	ctx.r11.s64 = 0;
loc_825A444C:
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825a447c
	if (ctx.cr6.eq) goto loc_825A447C;
	// bl 0x826745d8
	ctx.lr = 0x825A445C;
	sub_826745D8(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825a447c
	if (ctx.cr6.eq) goto loc_825A447C;
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// lwz r5,1356(r31)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r31.u32 + 1356);
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// addi r3,r11,22588
	ctx.r3.s64 = ctx.r11.s64 + 22588;
	// bl 0x826909c8
	ctx.lr = 0x825A447C;
	sub_826909C8(ctx, base);
loc_825A447C:
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

PPC_FUNC_IMPL(__imp__sub_825A4498);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4498) { __imp__sub_825A4498(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4498) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x825A44A0;
	__savegprlr_27(ctx, base);
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// mr r29,r6
	ctx.r29.u64 = ctx.r6.u64;
	// mr r28,r7
	ctx.r28.u64 = ctx.r7.u64;
	// mr r27,r8
	ctx.r27.u64 = ctx.r8.u64;
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x825a453c
	if (ctx.cr6.eq) goto loc_825A453C;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// srawi r9,r11,8
	ctx.xer.ca = (ctx.r11.s32 < 0) & ((ctx.r11.u32 & 0xFF) != 0);
	ctx.r9.s64 = ctx.r11.s32 >> 8;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// lwz r10,31824(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 31824);
	// lwz r8,4(r10)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// lbzx r8,r8,r9
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r8.u32 + ctx.r9.u32);
	// cmpw cr6,r8,r11
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r11.s32, ctx.xer);
	// bne cr6,0x825a44f4
	if (!ctx.cr6.eq) goto loc_825A44F4;
	// lwz r11,12(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 12);
	// lwz r10,0(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// mullw r11,r11,r9
	ctx.r11.s64 = int64_t(ctx.r11.s32) * int64_t(ctx.r9.s32);
	// add r31,r11,r10
	ctx.r31.u64 = ctx.r11.u64 + ctx.r10.u64;
	// b 0x825a44f8
	goto loc_825A44F8;
loc_825A44F4:
	// li r31,0
	ctx.r31.s64 = 0;
loc_825A44F8:
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r5
	ctx.r3.u64 = ctx.r5.u64;
	// bl 0x827df490
	ctx.lr = 0x825A4504;
	sub_827DF490(ctx, base);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// mr r7,r3
	ctx.r7.u64 = ctx.r3.u64;
	// stw r27,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r27.u32);
	// li r10,0
	ctx.r10.s64 = 0;
	// li r9,0
	ctx.r9.s64 = 0;
	// li r8,-1
	ctx.r8.s64 = -1;
	// lfs f1,2592(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2592);
	ctx.f1.f64 = double(temp.f32);
	// li r11,0
	ctx.r11.s64 = 0;
	// mr r6,r28
	ctx.r6.u64 = ctx.r28.u64;
	// mr r5,r29
	ctx.r5.u64 = ctx.r29.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// addi r3,r31,1264
	ctx.r3.s64 = ctx.r31.s64 + 1264;
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// bl 0x82413388
	ctx.lr = 0x825A453C;
	sub_82413388(ctx, base);
loc_825A453C:
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4548);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4548) { __imp__sub_825A4548(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4548) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r9,r3
	ctx.r9.u64 = ctx.r3.u64;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// beq cr6,0x825a45b4
	if (ctx.cr6.eq) goto loc_825A45B4;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// srawi r10,r9,8
	ctx.xer.ca = (ctx.r9.s32 < 0) & ((ctx.r9.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r9.s32 >> 8;
	// clrlwi r9,r9,24
	ctx.r9.u64 = ctx.r9.u32 & 0xFF;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r8,r8,r10
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r8.u32 + ctx.r10.u32);
	// cmpw cr6,r8,r9
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r9.s32, ctx.xer);
	// bne cr6,0x825a459c
	if (!ctx.cr6.eq) goto loc_825A459C;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r31,r11,r9
	ctx.r31.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825a45a0
	goto loc_825A45A0;
loc_825A459C:
	// li r31,0
	ctx.r31.s64 = 0;
loc_825A45A0:
	// li r4,0
	ctx.r4.s64 = 0;
	// bl 0x827df490
	ctx.lr = 0x825A45A8;
	sub_827DF490(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r31,1264
	ctx.r3.s64 = ctx.r31.s64 + 1264;
	// bl 0x82411980
	ctx.lr = 0x825A45B4;
	sub_82411980(ctx, base);
loc_825A45B4:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A45C8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A45C8) { __imp__sub_825A45C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A45C8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb10
	ctx.lr = 0x825A45D0;
	__savegprlr_26(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// beq cr6,0x825a47ac
	if (ctx.cr6.eq) goto loc_825A47AC;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// srawi r10,r3,8
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r3.s32 >> 8;
	// clrlwi r8,r3,24
	ctx.r8.u64 = ctx.r3.u32 & 0xFF;
	// li r26,0
	ctx.r26.s64 = 0;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825a4614
	if (!ctx.cr6.eq) goto loc_825A4614;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r27,r11,r9
	ctx.r27.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825a4618
	goto loc_825A4618;
loc_825A4614:
	// mr r27,r26
	ctx.r27.u64 = ctx.r26.u64;
loc_825A4618:
	// lhz r11,46(r27)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r27.u32 + 46);
	// extsh r10,r11
	ctx.r10.s64 = ctx.r11.s16;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14120
	ctx.r11.s64 = ctx.r11.s64 + -14120;
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// cmpw cr6,r8,r10
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r10.s32, ctx.xer);
	// beq cr6,0x825a4658
	if (ctx.cr6.eq) goto loc_825A4658;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14108
	ctx.r11.s64 = ctx.r11.s64 + -14108;
	// lwz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// cmpw cr6,r11,r10
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r10.s32, ctx.xer);
	// beq cr6,0x825a4658
	if (ctx.cr6.eq) goto loc_825A4658;
	// addi r3,r27,1264
	ctx.r3.s64 = ctx.r27.s64 + 1264;
	// bl 0x82411b40
	ctx.lr = 0x825A4650;
	sub_82411B40(ctx, base);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
loc_825A4658:
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// subf r8,r8,r10
	ctx.r8.u64 = ctx.r10.u64 - ctx.r8.u64;
	// li r29,-1
	ctx.r29.s64 = -1;
	// lwz r11,31772(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31772);
	// mr r9,r11
	ctx.r9.u64 = ctx.r11.u64;
	// stw r29,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r29.u32);
	// rlwinm r10,r11,31,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 31) & 0x1;
	// cntlzw r11,r8
	ctx.r11.u64 = ctx.r8.u32 == 0 ? 32 : __builtin_clz(ctx.r8.u32);
	// rlwinm r30,r11,27,31,31
	ctx.r30.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// clrlwi r11,r9,31
	ctx.r11.u64 = ctx.r9.u32 & 0x1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825a4718
	if (!ctx.cr6.eq) goto loc_825A4718;
	// clrlwi r9,r10,24
	ctx.r9.u64 = ctx.r10.u32 & 0xFF;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// bne cr6,0x825a4718
	if (!ctx.cr6.eq) goto loc_825A4718;
	// clrlwi r28,r30,24
	ctx.r28.u64 = ctx.r30.u32 & 0xFF;
	// cmplwi cr6,r28,0
	ctx.cr6.compare<uint32_t>(ctx.r28.u32, 0, ctx.xer);
	// beq cr6,0x825a46ac
	if (ctx.cr6.eq) goto loc_825A46AC;
	// li r11,3
	ctx.r11.s64 = 3;
	// stw r11,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
	// b 0x825a46bc
	goto loc_825A46BC;
loc_825A46AC:
	// li r4,4
	ctx.r4.s64 = 4;
	// li r3,1
	ctx.r3.s64 = 1;
	// bl 0x821233f0
	ctx.lr = 0x825A46B8;
	sub_821233F0(ctx, base);
	// stw r3,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r3.u32);
loc_825A46BC:
	// lis r11,-32050
	ctx.r11.s64 = -2100428800;
	// stw r29,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r29.u32);
	// lwz r31,-9924(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + -9924);
	// lbz r11,16(r31)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r31.u32 + 16);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825a475c
	if (ctx.cr6.eq) goto loc_825A475C;
	// li r4,404
	ctx.r4.s64 = 404;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x822ef320
	ctx.lr = 0x825A46E0;
	sub_822EF320(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825a475c
	if (ctx.cr6.eq) goto loc_825A475C;
	// li r4,404
	ctx.r4.s64 = 404;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x822ef2c0
	ctx.lr = 0x825A46F8;
	sub_822EF2C0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r8,r1,84
	ctx.r8.s64 = ctx.r1.s64 + 84;
	// addi r7,r1,80
	ctx.r7.s64 = ctx.r1.s64 + 80;
	// mr r6,r30
	ctx.r6.u64 = ctx.r30.u64;
	// li r5,0
	ctx.r5.s64 = 0;
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// bl 0x8213b170
	ctx.lr = 0x825A4714;
	sub_8213B170(ctx, base);
	// b 0x825a475c
	goto loc_825A475C;
loc_825A4718:
	// clrlwi r28,r30,24
	ctx.r28.u64 = ctx.r30.u32 & 0xFF;
	// stw r26,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r26.u32);
	// cmplwi cr6,r28,0
	ctx.cr6.compare<uint32_t>(ctx.r28.u32, 0, ctx.xer);
	// beq cr6,0x825a474c
	if (ctx.cr6.eq) goto loc_825A474C;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825a4738
	if (ctx.cr6.eq) goto loc_825A4738;
	// li r11,3
	ctx.r11.s64 = 3;
	// b 0x825a4758
	goto loc_825A4758;
loc_825A4738:
	// clrlwi r11,r10,24
	ctx.r11.u64 = ctx.r10.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825a475c
	if (ctx.cr6.eq) goto loc_825A475C;
	// li r11,4
	ctx.r11.s64 = 4;
	// b 0x825a4758
	goto loc_825A4758;
loc_825A474C:
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825a475c
	if (ctx.cr6.eq) goto loc_825A475C;
	// li r11,1
	ctx.r11.s64 = 1;
loc_825A4758:
	// stw r11,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
loc_825A475C:
	// lwz r3,80(r1)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// cmplwi cr6,r28,0
	ctx.cr6.compare<uint32_t>(ctx.r28.u32, 0, ctx.xer);
	// beq cr6,0x825a4770
	if (ctx.cr6.eq) goto loc_825A4770;
	// bl 0x8213b440
	ctx.lr = 0x825A476C;
	sub_8213B440(ctx, base);
	// b 0x825a4774
	goto loc_825A4774;
loc_825A4770:
	// bl 0x8213b520
	ctx.lr = 0x825A4774;
	sub_8213B520(ctx, base);
loc_825A4774:
	// lwz r11,100(r27)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r27.u32 + 100);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x825a4790
	if (ctx.cr6.eq) goto loc_825A4790;
	// lbz r11,14(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 14);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// bne cr6,0x825a4794
	if (!ctx.cr6.eq) goto loc_825A4794;
loc_825A4790:
	// mr r11,r26
	ctx.r11.u64 = ctx.r26.u64;
loc_825A4794:
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825a47ac
	if (!ctx.cr6.eq) goto loc_825A47AC;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r27,1264
	ctx.r3.s64 = ctx.r27.s64 + 1264;
	// bl 0x82411980
	ctx.lr = 0x825A47AC;
	sub_82411980(ctx, base);
loc_825A47AC:
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A47B8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A47B8) { __imp__sub_825A47B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A47B8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// beq cr6,0x825a4870
	if (ctx.cr6.eq) goto loc_825A4870;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// srawi r10,r3,8
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r3.s32 >> 8;
	// clrlwi r8,r3,24
	ctx.r8.u64 = ctx.r3.u32 & 0xFF;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825a4804
	if (!ctx.cr6.eq) goto loc_825A4804;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825a4808
	goto loc_825A4808;
loc_825A4804:
	// li r11,0
	ctx.r11.s64 = 0;
loc_825A4808:
	// addi r31,r11,1264
	ctx.r31.s64 = ctx.r11.s64 + 1264;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x824111e8
	ctx.lr = 0x825A4814;
	sub_824111E8(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825a4854
	if (!ctx.cr6.eq) goto loc_825A4854;
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82411d28
	ctx.lr = 0x825A4830;
	sub_82411D28(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825a4854
	if (!ctx.cr6.eq) goto loc_825A4854;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82411260
	ctx.lr = 0x825A4844;
	sub_82411260(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// li r11,0
	ctx.r11.s64 = 0;
	// beq cr6,0x825a4858
	if (ctx.cr6.eq) goto loc_825A4858;
loc_825A4854:
	// li r11,1
	ctx.r11.s64 = 1;
loc_825A4858:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
loc_825A4870:
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
}

PPC_FUNC_IMPL(__imp__sub_825A4888);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4888) { __imp__sub_825A4888(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4888) {
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
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// beq cr6,0x825a4914
	if (ctx.cr6.eq) goto loc_825A4914;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// srawi r10,r3,8
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r3.s32 >> 8;
	// clrlwi r8,r3,24
	ctx.r8.u64 = ctx.r3.u32 & 0xFF;
	// li r6,0
	ctx.r6.s64 = 0;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825a48d4
	if (!ctx.cr6.eq) goto loc_825A48D4;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825a48d8
	goto loc_825A48D8;
loc_825A48D4:
	// mr r11,r6
	ctx.r11.u64 = ctx.r6.u64;
loc_825A48D8:
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// stw r6,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r6.u32);
	// lis r7,-32014
	ctx.r7.s64 = -2098069504;
	// stw r6,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r6.u32);
	// li r9,0
	ctx.r9.s64 = 0;
	// li r8,-1
	ctx.r8.s64 = -1;
	// li r6,1
	ctx.r6.s64 = 1;
	// lfs f1,2592(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 2592);
	ctx.f1.f64 = double(temp.f32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// li r5,1
	ctx.r5.s64 = 1;
	// lwz r7,-29700(r7)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r7.u32 + -29700);
	// addi r4,r10,23028
	ctx.r4.s64 = ctx.r10.s64 + 23028;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r3,r11,1264
	ctx.r3.s64 = ctx.r11.s64 + 1264;
	// bl 0x82413388
	ctx.lr = 0x825A4914;
	sub_82413388(ctx, base);
loc_825A4914:
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4928);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4928) { __imp__sub_825A4928(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4928) {
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
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// beq cr6,0x825a49b4
	if (ctx.cr6.eq) goto loc_825A49B4;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// srawi r10,r3,8
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r3.s32 >> 8;
	// clrlwi r8,r3,24
	ctx.r8.u64 = ctx.r3.u32 & 0xFF;
	// li r6,0
	ctx.r6.s64 = 0;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825a4974
	if (!ctx.cr6.eq) goto loc_825A4974;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825a4978
	goto loc_825A4978;
loc_825A4974:
	// mr r11,r6
	ctx.r11.u64 = ctx.r6.u64;
loc_825A4978:
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// stw r6,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r6.u32);
	// lis r7,-32014
	ctx.r7.s64 = -2098069504;
	// stw r6,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r6.u32);
	// li r9,0
	ctx.r9.s64 = 0;
	// li r8,-1
	ctx.r8.s64 = -1;
	// li r6,1
	ctx.r6.s64 = 1;
	// lfs f1,2592(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 2592);
	ctx.f1.f64 = double(temp.f32);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// li r5,1
	ctx.r5.s64 = 1;
	// lwz r7,-29700(r7)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r7.u32 + -29700);
	// addi r4,r10,-25524
	ctx.r4.s64 = ctx.r10.s64 + -25524;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r3,r11,1264
	ctx.r3.s64 = ctx.r11.s64 + 1264;
	// bl 0x82413388
	ctx.lr = 0x825A49B4;
	sub_82413388(ctx, base);
loc_825A49B4:
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A49C8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A49C8) { __imp__sub_825A49C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A49C8) {
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
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// beq cr6,0x825a4a54
	if (ctx.cr6.eq) goto loc_825A4A54;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// srawi r10,r3,8
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r3.s32 >> 8;
	// clrlwi r8,r3,24
	ctx.r8.u64 = ctx.r3.u32 & 0xFF;
	// li r6,0
	ctx.r6.s64 = 0;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x825a4a14
	if (!ctx.cr6.eq) goto loc_825A4A14;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825a4a18
	goto loc_825A4A18;
loc_825A4A14:
	// mr r11,r6
	ctx.r11.u64 = ctx.r6.u64;
loc_825A4A18:
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// stw r6,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r6.u32);
	// lis r7,-32014
	ctx.r7.s64 = -2098069504;
	// stw r6,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r6.u32);
	// li r9,0
	ctx.r9.s64 = 0;
	// li r8,-1
	ctx.r8.s64 = -1;
	// li r6,1
	ctx.r6.s64 = 1;
	// lfs f1,2592(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 2592);
	ctx.f1.f64 = double(temp.f32);
	// lis r10,-32255
	ctx.r10.s64 = -2113863680;
	// li r5,1
	ctx.r5.s64 = 1;
	// lwz r7,-29700(r7)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r7.u32 + -29700);
	// addi r4,r10,21616
	ctx.r4.s64 = ctx.r10.s64 + 21616;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r3,r11,1264
	ctx.r3.s64 = ctx.r11.s64 + 1264;
	// bl 0x82413388
	ctx.lr = 0x825A4A54;
	sub_82413388(ctx, base);
loc_825A4A54:
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4A68);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4A68) { __imp__sub_825A4A68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4A68) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r9,r3
	ctx.r9.u64 = ctx.r3.u64;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// beq cr6,0x825a4ad4
	if (ctx.cr6.eq) goto loc_825A4AD4;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// srawi r10,r9,8
	ctx.xer.ca = (ctx.r9.s32 < 0) & ((ctx.r9.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r9.s32 >> 8;
	// clrlwi r9,r9,24
	ctx.r9.u64 = ctx.r9.u32 & 0xFF;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r8,r8,r10
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r8.u32 + ctx.r10.u32);
	// cmpw cr6,r8,r9
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r9.s32, ctx.xer);
	// bne cr6,0x825a4abc
	if (!ctx.cr6.eq) goto loc_825A4ABC;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r31,r11,r9
	ctx.r31.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825a4ac0
	goto loc_825A4AC0;
loc_825A4ABC:
	// li r31,0
	ctx.r31.s64 = 0;
loc_825A4AC0:
	// li r4,0
	ctx.r4.s64 = 0;
	// bl 0x827df490
	ctx.lr = 0x825A4AC8;
	sub_827DF490(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r31,832
	ctx.r3.s64 = ctx.r31.s64 + 832;
	// bl 0x82486a70
	ctx.lr = 0x825A4AD4;
	sub_82486A70(ctx, base);
loc_825A4AD4:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4AE8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4AE8) { __imp__sub_825A4AE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4AE8) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// lwz r5,0(r10)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// b 0x825fd908
	sub_825FD908(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4B00);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4B00) { __imp__sub_825A4B00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4B00) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// lwz r5,0(r10)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// b 0x825fee30
	sub_825FEE30(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4B18);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4B18) { __imp__sub_825A4B18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4B18) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// b 0x82600610
	sub_82600610(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4B30);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4B30) { __imp__sub_825A4B30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4B30) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r4,0
	ctx.r4.s64 = 0;
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// lwz r5,4(r10)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x825a4b80
	if (ctx.cr6.eq) goto loc_825A4B80;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// srawi r9,r11,8
	ctx.xer.ca = (ctx.r11.s32 < 0) & ((ctx.r11.u32 & 0xFF) != 0);
	ctx.r9.s64 = ctx.r11.s32 >> 8;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// lwz r10,31824(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 31824);
	// lwz r8,4(r10)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// lbzx r8,r8,r9
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r8.u32 + ctx.r9.u32);
	// cmpw cr6,r8,r11
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r11.s32, ctx.xer);
	// bne cr6,0x825a4b7c
	if (!ctx.cr6.eq) goto loc_825A4B7C;
	// lwz r11,12(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 12);
	// lwz r10,0(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// mullw r11,r11,r9
	ctx.r11.s64 = int64_t(ctx.r11.s32) * int64_t(ctx.r9.s32);
	// add r4,r11,r10
	ctx.r4.u64 = ctx.r11.u64 + ctx.r10.u64;
	// b 0x825a4b80
	goto loc_825A4B80;
loc_825A4B7C:
	// li r4,0
	ctx.r4.s64 = 0;
loc_825A4B80:
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// b 0x825fd908
	sub_825FD908(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4B90);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4B90) { __imp__sub_825A4B90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4B90) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r4,0
	ctx.r4.s64 = 0;
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// beq cr6,0x825a4bdc
	if (ctx.cr6.eq) goto loc_825A4BDC;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// srawi r9,r10,8
	ctx.xer.ca = (ctx.r10.s32 < 0) & ((ctx.r10.u32 & 0xFF) != 0);
	ctx.r9.s64 = ctx.r10.s32 >> 8;
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r8,r8,r9
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r8.u32 + ctx.r9.u32);
	// cmpw cr6,r8,r10
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r10.s32, ctx.xer);
	// bne cr6,0x825a4bd8
	if (!ctx.cr6.eq) goto loc_825A4BD8;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r9
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r9.s32);
	// add r4,r11,r10
	ctx.r4.u64 = ctx.r11.u64 + ctx.r10.u64;
	// b 0x825a4bdc
	goto loc_825A4BDC;
loc_825A4BD8:
	// li r4,0
	ctx.r4.s64 = 0;
loc_825A4BDC:
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// b 0x825fd948
	sub_825FD948(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4BE8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4BE8) { __imp__sub_825A4BE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4BE8) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r4,0
	ctx.r4.s64 = 0;
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// lwz r5,4(r10)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x825a4c38
	if (ctx.cr6.eq) goto loc_825A4C38;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// srawi r9,r11,8
	ctx.xer.ca = (ctx.r11.s32 < 0) & ((ctx.r11.u32 & 0xFF) != 0);
	ctx.r9.s64 = ctx.r11.s32 >> 8;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// lwz r10,31824(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 31824);
	// lwz r8,4(r10)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// lbzx r8,r8,r9
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r8.u32 + ctx.r9.u32);
	// cmpw cr6,r8,r11
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r11.s32, ctx.xer);
	// bne cr6,0x825a4c34
	if (!ctx.cr6.eq) goto loc_825A4C34;
	// lwz r11,12(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 12);
	// lwz r10,0(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// mullw r11,r11,r9
	ctx.r11.s64 = int64_t(ctx.r11.s32) * int64_t(ctx.r9.s32);
	// add r4,r11,r10
	ctx.r4.u64 = ctx.r11.u64 + ctx.r10.u64;
	// b 0x825a4c38
	goto loc_825A4C38;
loc_825A4C34:
	// li r4,0
	ctx.r4.s64 = 0;
loc_825A4C38:
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// b 0x825fee30
	sub_825FEE30(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4C48);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4C48) { __imp__sub_825A4C48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4C48) {
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
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r31,r11,17696
	ctx.r31.s64 = ctx.r11.s64 + 17696;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x825fdb70
	ctx.lr = 0x825A4C70;
	sub_825FDB70(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825a4c94
	if (ctx.cr6.eq) goto loc_825A4C94;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x825fdba0
	ctx.lr = 0x825A4C84;
	sub_825FDBA0(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// bne cr6,0x825a4c98
	if (!ctx.cr6.eq) goto loc_825A4C98;
loc_825A4C94:
	// li r11,0
	ctx.r11.s64 = 0;
loc_825A4C98:
	// lwz r10,0(r30)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
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

PPC_FUNC_IMPL(__imp__sub_825A4CC0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4CC0) { __imp__sub_825A4CC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4CC0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32003
	ctx.r10.s64 = -2097348608;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r10,17696
	ctx.r3.s64 = ctx.r10.s64 + 17696;
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
	// clrlwi r5,r11,24
	ctx.r5.u64 = ctx.r11.u32 & 0xFF;
	// clrlwi r6,r10,24
	ctx.r6.u64 = ctx.r10.u32 & 0xFF;
	// b 0x825fee88
	sub_825FEE88(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4D00);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4D00) { __imp__sub_825A4D00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4D00) {
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
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r31,r11,17696
	ctx.r31.s64 = ctx.r11.s64 + 17696;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x825fdb70
	ctx.lr = 0x825A4D28;
	sub_825FDB70(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825a4d4c
	if (ctx.cr6.eq) goto loc_825A4D4C;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x825fdba0
	ctx.lr = 0x825A4D3C;
	sub_825FDBA0(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// beq cr6,0x825a4d50
	if (ctx.cr6.eq) goto loc_825A4D50;
loc_825A4D4C:
	// li r11,0
	ctx.r11.s64 = 0;
loc_825A4D50:
	// lwz r10,0(r30)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
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

PPC_FUNC_IMPL(__imp__sub_825A4D78);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4D78) { __imp__sub_825A4D78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4D78) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// bl 0x825fdbb0
	ctx.lr = 0x825A4D98;
	sub_825FDBB0(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A4DB8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4DB8) { __imp__sub_825A4DB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4DB8) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// b 0x825fdde8
	sub_825FDDE8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4DE0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4DE0) { __imp__sub_825A4DE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4DE0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// bl 0x825ff0a0
	ctx.lr = 0x825A4E18;
	sub_825FF0A0(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A4E38);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4E38) { __imp__sub_825A4E38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4E38) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// b 0x825ff150
	sub_825FF150(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4E60);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4E60) { __imp__sub_825A4E60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4E60) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// bl 0x821d2088
	ctx.lr = 0x825A4E70;
	sub_821D2088(ctx, base);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// lis r10,-32003
	ctx.r10.s64 = -2097348608;
	// addi r3,r10,17696
	ctx.r3.s64 = ctx.r10.s64 + 17696;
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x82601620
	ctx.lr = 0x825A4E84;
	sub_82601620(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4E98);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4E98) { __imp__sub_825A4E98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4E98) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// bl 0x825ff9d8
	ctx.lr = 0x825A4EB8;
	sub_825FF9D8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A4ED8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4ED8) { __imp__sub_825A4ED8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4ED8) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// b 0x825ffb18
	sub_825FFB18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4EF0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4EF0) { __imp__sub_825A4EF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4EF0) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// b 0x826000e8
	sub_826000E8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4F08);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4F08) { __imp__sub_825A4F08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4F08) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x82600350
	ctx.lr = 0x825A4F30;
	sub_82600350(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A4F50);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4F50) { __imp__sub_825A4F50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4F50) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// srawi r10,r9,8
	ctx.xer.ca = (ctx.r9.s32 < 0) & ((ctx.r9.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r9.s32 >> 8;
	// clrlwi r9,r9,24
	ctx.r9.u64 = ctx.r9.u32 & 0xFF;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r8,r8,r10
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r8.u32 + ctx.r10.u32);
	// cmpw cr6,r8,r9
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r9.s32, ctx.xer);
	// bne cr6,0x825a4f94
	if (!ctx.cr6.eq) goto loc_825A4F94;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825a4f98
	goto loc_825A4F98;
loc_825A4F94:
	// li r11,0
	ctx.r11.s64 = 0;
loc_825A4F98:
	// addi r3,r11,1264
	ctx.r3.s64 = ctx.r11.s64 + 1264;
	// b 0x82411b40
	sub_82411B40(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4FA0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4FA0) { __imp__sub_825A4FA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4FA0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4FA8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4FA8) { __imp__sub_825A4FA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4FA8) {
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
	// b 0x825a45c8
	sub_825A45C8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A4FD0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A4FD0) { __imp__sub_825A4FD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A4FD0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// srawi r10,r9,8
	ctx.xer.ca = (ctx.r9.s32 < 0) & ((ctx.r9.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r9.s32 >> 8;
	// clrlwi r9,r9,24
	ctx.r9.u64 = ctx.r9.u32 & 0xFF;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r8,r8,r10
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r8.u32 + ctx.r10.u32);
	// cmpw cr6,r8,r9
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r9.s32, ctx.xer);
	// bne cr6,0x825a5014
	if (!ctx.cr6.eq) goto loc_825A5014;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825a5018
	goto loc_825A5018;
loc_825A5014:
	// li r11,0
	ctx.r11.s64 = 0;
loc_825A5018:
	// addi r3,r11,1264
	ctx.r3.s64 = ctx.r11.s64 + 1264;
	// b 0x82411288
	sub_82411288(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5020);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5020) { __imp__sub_825A5020(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5020) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5028);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5028) { __imp__sub_825A5028(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5028) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// beq cr6,0x825a5098
	if (ctx.cr6.eq) goto loc_825A5098;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// srawi r10,r9,8
	ctx.xer.ca = (ctx.r9.s32 < 0) & ((ctx.r9.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r9.s32 >> 8;
	// clrlwi r9,r9,24
	ctx.r9.u64 = ctx.r9.u32 & 0xFF;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r8,r8,r10
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r8.u32 + ctx.r10.u32);
	// cmpw cr6,r8,r9
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r9.s32, ctx.xer);
	// bne cr6,0x825a5088
	if (!ctx.cr6.eq) goto loc_825A5088;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// addi r3,r11,1264
	ctx.r3.s64 = ctx.r11.s64 + 1264;
	// bl 0x824111e8
	ctx.lr = 0x825A5084;
	sub_824111E8(ctx, base);
	// b 0x825a509c
	goto loc_825A509C;
loc_825A5088:
	// li r11,0
	ctx.r11.s64 = 0;
	// addi r3,r11,1264
	ctx.r3.s64 = ctx.r11.s64 + 1264;
	// bl 0x824111e8
	ctx.lr = 0x825A5094;
	sub_824111E8(ctx, base);
	// b 0x825a509c
	goto loc_825A509C;
loc_825A5098:
	// li r3,0
	ctx.r3.s64 = 0;
loc_825A509C:
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

PPC_FUNC_IMPL(__imp__sub_825A50C0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A50C0) { __imp__sub_825A50C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A50C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// beq cr6,0x825a5140
	if (ctx.cr6.eq) goto loc_825A5140;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// srawi r10,r9,8
	ctx.xer.ca = (ctx.r9.s32 < 0) & ((ctx.r9.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r9.s32 >> 8;
	// clrlwi r9,r9,24
	ctx.r9.u64 = ctx.r9.u32 & 0xFF;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r8,r8,r10
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r8.u32 + ctx.r10.u32);
	// cmpw cr6,r8,r9
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r9.s32, ctx.xer);
	// bne cr6,0x825a5128
	if (!ctx.cr6.eq) goto loc_825A5128;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// li r5,0
	ctx.r5.s64 = 0;
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// li r4,0
	ctx.r4.s64 = 0;
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// addi r3,r11,1264
	ctx.r3.s64 = ctx.r11.s64 + 1264;
	// bl 0x82411d28
	ctx.lr = 0x825A5124;
	sub_82411D28(ctx, base);
	// b 0x825a5144
	goto loc_825A5144;
loc_825A5128:
	// li r11,0
	ctx.r11.s64 = 0;
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,1264
	ctx.r3.s64 = ctx.r11.s64 + 1264;
	// bl 0x82411d28
	ctx.lr = 0x825A513C;
	sub_82411D28(ctx, base);
	// b 0x825a5144
	goto loc_825A5144;
loc_825A5140:
	// li r3,0
	ctx.r3.s64 = 0;
loc_825A5144:
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

PPC_FUNC_IMPL(__imp__sub_825A5168);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5168) { __imp__sub_825A5168(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5168) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// beq cr6,0x825a51d8
	if (ctx.cr6.eq) goto loc_825A51D8;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// srawi r10,r9,8
	ctx.xer.ca = (ctx.r9.s32 < 0) & ((ctx.r9.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r9.s32 >> 8;
	// clrlwi r9,r9,24
	ctx.r9.u64 = ctx.r9.u32 & 0xFF;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r8,r8,r10
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r8.u32 + ctx.r10.u32);
	// cmpw cr6,r8,r9
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r9.s32, ctx.xer);
	// bne cr6,0x825a51c8
	if (!ctx.cr6.eq) goto loc_825A51C8;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// addi r3,r11,1264
	ctx.r3.s64 = ctx.r11.s64 + 1264;
	// bl 0x82411260
	ctx.lr = 0x825A51C4;
	sub_82411260(ctx, base);
	// b 0x825a51dc
	goto loc_825A51DC;
loc_825A51C8:
	// li r11,0
	ctx.r11.s64 = 0;
	// addi r3,r11,1264
	ctx.r3.s64 = ctx.r11.s64 + 1264;
	// bl 0x82411260
	ctx.lr = 0x825A51D4;
	sub_82411260(ctx, base);
	// b 0x825a51dc
	goto loc_825A51DC;
loc_825A51D8:
	// li r3,0
	ctx.r3.s64 = 0;
loc_825A51DC:
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

PPC_FUNC_IMPL(__imp__sub_825A5200);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5200) { __imp__sub_825A5200(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5200) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x825a47b8
	ctx.lr = 0x825A5220;
	sub_825A47B8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A5240);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5240) { __imp__sub_825A5240(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5240) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825a4888
	sub_825A4888(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5250);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5250) { __imp__sub_825A5250(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5250) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825a4928
	sub_825A4928(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5260);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5260) { __imp__sub_825A5260(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5260) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825a49c8
	sub_825A49C8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5270);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5270) { __imp__sub_825A5270(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5270) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r9
	ctx.r11.u64 = ctx.r9.u32 == 0 ? 32 : __builtin_clz(ctx.r9.u32);
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r8,r11,24
	ctx.r8.u64 = ctx.r11.u32 & 0xFF;
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// srawi r9,r10,8
	ctx.xer.ca = (ctx.r10.s32 < 0) & ((ctx.r10.u32 & 0xFF) != 0);
	ctx.r9.s64 = ctx.r10.s32 >> 8;
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r7,4(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r7,r7,r9
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r7.u32 + ctx.r9.u32);
	// cmpw cr6,r7,r10
	ctx.cr6.compare<int32_t>(ctx.r7.s32, ctx.r10.s32, ctx.xer);
	// bne cr6,0x825a52cc
	if (!ctx.cr6.eq) goto loc_825A52CC;
	// lwz r7,12(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r7,r9
	ctx.r11.s64 = int64_t(ctx.r7.s32) * int64_t(ctx.r9.s32);
	// add r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
	// stb r8,1696(r11)
	PPC_STORE_U8(ctx.r11.u32 + 1696, ctx.r8.u8);
	// blr 
	return;
loc_825A52CC:
	// li r11,0
	ctx.r11.s64 = 0;
	// stb r8,1696(r11)
	PPC_STORE_U8(ctx.r11.u32 + 1696, ctx.r8.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A52D8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A52D8) { __imp__sub_825A52D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A52D8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r9
	ctx.r11.u64 = ctx.r9.u32 == 0 ? 32 : __builtin_clz(ctx.r9.u32);
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r8,r11,24
	ctx.r8.u64 = ctx.r11.u32 & 0xFF;
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// srawi r9,r10,8
	ctx.xer.ca = (ctx.r10.s32 < 0) & ((ctx.r10.u32 & 0xFF) != 0);
	ctx.r9.s64 = ctx.r10.s32 >> 8;
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r7,4(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r7,r7,r9
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r7.u32 + ctx.r9.u32);
	// cmpw cr6,r7,r10
	ctx.cr6.compare<int32_t>(ctx.r7.s32, ctx.r10.s32, ctx.xer);
	// bne cr6,0x825a5334
	if (!ctx.cr6.eq) goto loc_825A5334;
	// lwz r7,12(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r7,r9
	ctx.r11.s64 = int64_t(ctx.r7.s32) * int64_t(ctx.r9.s32);
	// add r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
	// stb r8,1697(r11)
	PPC_STORE_U8(ctx.r11.u32 + 1697, ctx.r8.u8);
	// blr 
	return;
loc_825A5334:
	// li r11,0
	ctx.r11.s64 = 0;
	// stb r8,1697(r11)
	PPC_STORE_U8(ctx.r11.u32 + 1697, ctx.r8.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5340);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5340) { __imp__sub_825A5340(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5340) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32062
	ctx.r11.s64 = -2101215232;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lbz r11,-28111(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + -28111);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5358);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5358) { __imp__sub_825A5358(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5358) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32062
	ctx.r11.s64 = -2101215232;
	// addi r3,r11,-28104
	ctx.r3.s64 = ctx.r11.s64 + -28104;
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// b 0x8225afd8
	sub_8225AFD8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5380);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5380) { __imp__sub_825A5380(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5380) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32062
	ctx.r11.s64 = -2101215232;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// addi r11,r11,-28104
	ctx.r11.s64 = ctx.r11.s64 + -28104;
	// lbz r11,111(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 111);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5398);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5398) { __imp__sub_825A5398(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5398) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32062
	ctx.r11.s64 = -2101215232;
	// addi r11,r11,-28104
	ctx.r11.s64 = ctx.r11.s64 + -28104;
	// lbz r10,108(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 108);
	// cmplwi cr6,r10,4
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 4, ctx.xer);
	// beq cr6,0x825a53bc
	if (ctx.cr6.eq) goto loc_825A53BC;
	// lbz r11,109(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 109);
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// li r11,0
	ctx.r11.s64 = 0;
	// bne cr6,0x825a53c0
	if (!ctx.cr6.eq) goto loc_825A53C0;
loc_825A53BC:
	// li r11,1
	ctx.r11.s64 = 1;
loc_825A53C0:
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A53D0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A53D0) { __imp__sub_825A53D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A53D0) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32062
	ctx.r11.s64 = -2101215232;
	// addi r3,r11,-28104
	ctx.r3.s64 = ctx.r11.s64 + -28104;
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// b 0x8225afd0
	sub_8225AFD0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A53E8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A53E8) { __imp__sub_825A53E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A53E8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825fa3a0
	sub_825FA3A0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A53F8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A53F8) { __imp__sub_825A53F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A53F8) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// li r9,1
	ctx.r9.s64 = 1;
	// addi r11,r11,17248
	ctx.r11.s64 = ctx.r11.s64 + 17248;
	// lwz r10,0(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// add r11,r10,r11
	ctx.r11.u64 = ctx.r10.u64 + ctx.r11.u64;
	// stb r9,-1(r11)
	PPC_STORE_U8(ctx.r11.u32 + -1, ctx.r9.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5418);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5418) { __imp__sub_825A5418(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5418) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x825f9560
	ctx.lr = 0x825A5430;
	sub_825F9560(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A5450);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5450) { __imp__sub_825A5450(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5450) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32062
	ctx.r11.s64 = -2101215232;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r11,-28104
	ctx.r3.s64 = ctx.r11.s64 + -28104;
	// bl 0x8225b1e0
	ctx.lr = 0x825A5470;
	sub_8225B1E0(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A5490);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5490) { __imp__sub_825A5490(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5490) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x82206850
	sub_82206850(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A54A0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A54A0) { __imp__sub_825A54A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A54A0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r11,32712
	ctx.r3.s64 = ctx.r11.s64 + 32712;
	// bl 0x82601d88
	ctx.lr = 0x825A54C0;
	sub_82601D88(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A54E0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A54E0) { __imp__sub_825A54E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A54E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r11,10272
	ctx.r3.s64 = ctx.r11.s64 + 10272;
	// bl 0x8252ff08
	ctx.lr = 0x825A5500;
	sub_8252FF08(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A5520);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5520) { __imp__sub_825A5520(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5520) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r5,0
	ctx.r5.s64 = 0;
	// lwz r11,4(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// lwz r8,0(r10)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x825a5570
	if (ctx.cr6.eq) goto loc_825A5570;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// srawi r9,r11,8
	ctx.xer.ca = (ctx.r11.s32 < 0) & ((ctx.r11.u32 & 0xFF) != 0);
	ctx.r9.s64 = ctx.r11.s32 >> 8;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// lwz r10,31824(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 31824);
	// lwz r7,4(r10)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// lbzx r7,r7,r9
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r7.u32 + ctx.r9.u32);
	// cmpw cr6,r7,r11
	ctx.cr6.compare<int32_t>(ctx.r7.s32, ctx.r11.s32, ctx.xer);
	// bne cr6,0x825a556c
	if (!ctx.cr6.eq) goto loc_825A556C;
	// lwz r11,12(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 12);
	// lwz r10,0(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// mullw r11,r11,r9
	ctx.r11.s64 = int64_t(ctx.r11.s32) * int64_t(ctx.r9.s32);
	// add r5,r11,r10
	ctx.r5.u64 = ctx.r11.u64 + ctx.r10.u64;
	// b 0x825a5570
	goto loc_825A5570;
loc_825A556C:
	// li r5,0
	ctx.r5.s64 = 0;
loc_825A5570:
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// clrlwi r4,r8,24
	ctx.r4.u64 = ctx.r8.u32 & 0xFF;
	// addi r3,r11,10272
	ctx.r3.s64 = ctx.r11.s64 + 10272;
	// b 0x8252ffa8
	sub_8252FFA8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5598);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5598) { __imp__sub_825A5598(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5598) {
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
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// addi r3,r10,10272
	ctx.r3.s64 = ctx.r10.s64 + 10272;
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
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f12,88(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// bl 0x825300a0
	ctx.lr = 0x825A55D0;
	sub_825300A0(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A55E0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A55E0) { __imp__sub_825A55E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A55E0) {
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
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// addi r3,r10,10272
	ctx.r3.s64 = ctx.r10.s64 + 10272;
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
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f12,88(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// bl 0x825300b8
	ctx.lr = 0x825A5618;
	sub_825300B8(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5628);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5628) { __imp__sub_825A5628(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5628) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r3,r11,10272
	ctx.r3.s64 = ctx.r11.s64 + 10272;
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82530120
	sub_82530120(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5650);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5650) { __imp__sub_825A5650(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5650) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r3,r11,10272
	ctx.r3.s64 = ctx.r11.s64 + 10272;
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82530178
	sub_82530178(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5678);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5678) { __imp__sub_825A5678(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5678) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82600480
	sub_82600480(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A56A0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A56A0) { __imp__sub_825A56A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A56A0) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82600510
	sub_82600510(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A56C8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A56C8) { __imp__sub_825A56C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A56C8) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82600590
	sub_82600590(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A56F0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A56F0) { __imp__sub_825A56F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A56F0) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// b 0x825fdff0
	sub_825FDFF0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5718);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5718) { __imp__sub_825A5718(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5718) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32003
	ctx.r10.s64 = -2097348608;
	// addi r3,r10,17696
	ctx.r3.s64 = ctx.r10.s64 + 17696;
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
	// b 0x82600718
	sub_82600718(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5750);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5750) { __imp__sub_825A5750(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5750) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,24544(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24544);
	// lfs f1,4(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// lwz r9,0(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// srawi r10,r9,8
	ctx.xer.ca = (ctx.r9.s32 < 0) & ((ctx.r9.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r9.s32 >> 8;
	// clrlwi r9,r9,24
	ctx.r9.u64 = ctx.r9.u32 & 0xFF;
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r8,r8,r10
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r8.u32 + ctx.r10.u32);
	// cmpw cr6,r8,r9
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r9.s32, ctx.xer);
	// bnelr cr6
	if (!ctx.cr6.eq) return;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// addi r3,r11,528
	ctx.r3.s64 = ctx.r11.s64 + 528;
	// b 0x8262ba10
	sub_8262BA10(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A579C);  /* forward decl */
PPC_WEAK_FUNC(sub_825A579C) { __imp__sub_825A579C(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A579C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A57A0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A57A0) { __imp__sub_825A57A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A57A0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r11,24544(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24544);
	// lwz r10,0(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// srawi r9,r10,8
	ctx.xer.ca = (ctx.r10.s32 < 0) & ((ctx.r10.u32 & 0xFF) != 0);
	ctx.r9.s64 = ctx.r10.s32 >> 8;
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// lbzx r8,r8,r9
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r8.u32 + ctx.r9.u32);
	// cmpw cr6,r8,r10
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r10.s32, ctx.xer);
	// bne cr6,0x825a5800
	if (!ctx.cr6.eq) goto loc_825A5800;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r9
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r9.s32);
	// add r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825a5800
	if (ctx.cr6.eq) goto loc_825A5800;
	// addi r3,r11,528
	ctx.r3.s64 = ctx.r11.s64 + 528;
	// bl 0x825da820
	ctx.lr = 0x825A57FC;
	sub_825DA820(ctx, base);
	// b 0x825a5804
	goto loc_825A5804;
loc_825A5800:
	// li r3,0
	ctx.r3.s64 = 0;
loc_825A5804:
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

PPC_FUNC_IMPL(__imp__sub_825A5828);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5828) { __imp__sub_825A5828(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5828) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r10,0(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// srawi r9,r10,8
	ctx.xer.ca = (ctx.r10.s32 < 0) & ((ctx.r10.u32 & 0xFF) != 0);
	ctx.r9.s64 = ctx.r10.s32 >> 8;
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r8,r8,r9
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r8.u32 + ctx.r9.u32);
	// cmpw cr6,r8,r10
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r10.s32, ctx.xer);
	// bne cr6,0x825a5864
	if (!ctx.cr6.eq) goto loc_825A5864;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r9
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r9.s32);
	// add r4,r11,r10
	ctx.r4.u64 = ctx.r11.u64 + ctx.r10.u64;
	// b 0x825a5868
	goto loc_825A5868;
loc_825A5864:
	// li r4,0
	ctx.r4.s64 = 0;
loc_825A5868:
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// b 0x82601850
	sub_82601850(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5878);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5878) { __imp__sub_825A5878(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5878) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// srawi r10,r9,8
	ctx.xer.ca = (ctx.r9.s32 < 0) & ((ctx.r9.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r9.s32 >> 8;
	// clrlwi r9,r9,24
	ctx.r9.u64 = ctx.r9.u32 & 0xFF;
	// lwz r11,88(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 88);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r8,r8,r10
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r8.u32 + ctx.r10.u32);
	// cmpw cr6,r8,r9
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r9.s32, ctx.xer);
	// bne cr6,0x825a58bc
	if (!ctx.cr6.eq) goto loc_825A58BC;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r4,r11,r9
	ctx.r4.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825a58c0
	goto loc_825A58C0;
loc_825A58BC:
	// li r4,0
	ctx.r4.s64 = 0;
loc_825A58C0:
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// b 0x82601850
	sub_82601850(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A58CC);  /* forward decl */
PPC_WEAK_FUNC(sub_825A58CC) { __imp__sub_825A58CC(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A58CC) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A58D0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A58D0) { __imp__sub_825A58D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A58D0) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// lwz r9,0(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// lwz r10,4(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// cntlzw r10,r10
	ctx.r10.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// rlwinm r10,r10,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// xori r8,r10,1
	ctx.r8.u64 = ctx.r10.u64 ^ 1;
	// lwz r7,4(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// srawi r10,r9,8
	ctx.xer.ca = (ctx.r9.s32 < 0) & ((ctx.r9.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r9.s32 >> 8;
	// clrlwi r5,r8,24
	ctx.r5.u64 = ctx.r8.u32 & 0xFF;
	// clrlwi r9,r9,24
	ctx.r9.u64 = ctx.r9.u32 & 0xFF;
	// lbzx r8,r7,r10
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r7.u32 + ctx.r10.u32);
	// cmpw cr6,r8,r9
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r9.s32, ctx.xer);
	// bne cr6,0x825a5920
	if (!ctx.cr6.eq) goto loc_825A5920;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r4,r11,r9
	ctx.r4.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825a5924
	goto loc_825A5924;
loc_825A5920:
	// li r4,0
	ctx.r4.s64 = 0;
loc_825A5924:
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// b 0x825fe1a0
	sub_825FE1A0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5930);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5930) { __imp__sub_825A5930(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5930) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// beq cr6,0x825a599c
	if (ctx.cr6.eq) goto loc_825A599C;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// srawi r10,r9,8
	ctx.xer.ca = (ctx.r9.s32 < 0) & ((ctx.r9.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r9.s32 >> 8;
	// clrlwi r9,r9,24
	ctx.r9.u64 = ctx.r9.u32 & 0xFF;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r8,r8,r10
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r8.u32 + ctx.r10.u32);
	// cmpw cr6,r8,r9
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r9.s32, ctx.xer);
	// bne cr6,0x825a5984
	if (!ctx.cr6.eq) goto loc_825A5984;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// lbz r11,1704(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 1704);
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
loc_825A5984:
	// li r11,0
	ctx.r11.s64 = 0;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lbz r11,1704(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 1704);
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
loc_825A599C:
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

PPC_FUNC_IMPL(__imp__sub_825A59B0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A59B0) { __imp__sub_825A59B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A59B0) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// lwz r9,0(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// lwz r10,4(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// lwz r11,24544(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24544);
	// clrlwi r6,r9,24
	ctx.r6.u64 = ctx.r9.u32 & 0xFF;
	// cntlzw r10,r10
	ctx.r10.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// rlwinm r10,r10,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// xori r8,r10,1
	ctx.r8.u64 = ctx.r10.u64 ^ 1;
	// lwz r7,4(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// srawi r10,r9,8
	ctx.xer.ca = (ctx.r9.s32 < 0) & ((ctx.r9.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r9.s32 >> 8;
	// clrlwi r9,r8,24
	ctx.r9.u64 = ctx.r8.u32 & 0xFF;
	// lbzx r8,r7,r10
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r7.u32 + ctx.r10.u32);
	// cmpw cr6,r8,r6
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r6.s32, ctx.xer);
	// bne cr6,0x825a5a04
	if (!ctx.cr6.eq) goto loc_825A5A04;
	// lwz r7,12(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r8,0(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r7,r10
	ctx.r11.s64 = int64_t(ctx.r7.s32) * int64_t(ctx.r10.s32);
	// add r11,r11,r8
	ctx.r11.u64 = ctx.r11.u64 + ctx.r8.u64;
	// stb r9,3392(r11)
	PPC_STORE_U8(ctx.r11.u32 + 3392, ctx.r9.u8);
	// blr 
	return;
loc_825A5A04:
	// li r11,0
	ctx.r11.s64 = 0;
	// stb r9,3392(r11)
	PPC_STORE_U8(ctx.r11.u32 + 3392, ctx.r9.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5A10);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5A10) { __imp__sub_825A5A10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5A10) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// lwz r9,0(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// lwz r10,4(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// lwz r11,24544(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24544);
	// cntlzw r10,r10
	ctx.r10.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// rlwinm r10,r10,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// xori r8,r10,1
	ctx.r8.u64 = ctx.r10.u64 ^ 1;
	// lwz r7,4(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// srawi r10,r9,8
	ctx.xer.ca = (ctx.r9.s32 < 0) & ((ctx.r9.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r9.s32 >> 8;
	// clrlwi r4,r8,24
	ctx.r4.u64 = ctx.r8.u32 & 0xFF;
	// clrlwi r9,r9,24
	ctx.r9.u64 = ctx.r9.u32 & 0xFF;
	// lbzx r8,r7,r10
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r7.u32 + ctx.r10.u32);
	// cmpw cr6,r8,r9
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r9.s32, ctx.xer);
	// bnelr cr6
	if (!ctx.cr6.eq) return;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// addi r3,r11,528
	ctx.r3.s64 = ctx.r11.s64 + 528;
	// b 0x825da5a0
	sub_825DA5A0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5A6C);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5A6C) { __imp__sub_825A5A6C(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5A6C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5A70);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5A70) { __imp__sub_825A5A70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5A70) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r11,-28768
	ctx.r3.s64 = ctx.r11.s64 + -28768;
	// bl 0x82893490
	ctx.lr = 0x825A5A90;
	sub_82893490(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A5AB0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5AB0) { __imp__sub_825A5AB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5AB0) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// lwz r9,0(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// lwz r10,4(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// lwz r11,24544(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24544);
	// cntlzw r10,r10
	ctx.r10.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// rlwinm r10,r10,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// xori r8,r10,1
	ctx.r8.u64 = ctx.r10.u64 ^ 1;
	// lwz r7,4(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// srawi r10,r9,8
	ctx.xer.ca = (ctx.r9.s32 < 0) & ((ctx.r9.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r9.s32 >> 8;
	// clrlwi r9,r9,24
	ctx.r9.u64 = ctx.r9.u32 & 0xFF;
	// clrlwi r8,r8,24
	ctx.r8.u64 = ctx.r8.u32 & 0xFF;
	// lbzx r7,r7,r10
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r7.u32 + ctx.r10.u32);
	// cmpw cr6,r7,r9
	ctx.cr6.compare<int32_t>(ctx.r7.s32, ctx.r9.s32, ctx.xer);
	// bnelr cr6
	if (!ctx.cr6.eq) return;
	// lwz r7,12(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r7,r10
	ctx.r11.s64 = int64_t(ctx.r7.s32) * int64_t(ctx.r10.s32);
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// clrlwi r10,r8,24
	ctx.r10.u64 = ctx.r8.u32 & 0xFF;
	// addi r3,r11,528
	ctx.r3.s64 = ctx.r11.s64 + 528;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x825a5b1c
	if (ctx.cr6.eq) goto loc_825A5B1C;
	// li r4,1
	ctx.r4.s64 = 1;
	// b 0x825da0e8
	sub_825DA0E8(ctx, base);
	return;
loc_825A5B1C:
	// li r4,0
	ctx.r4.s64 = 0;
	// b 0x825da0e8
	sub_825DA0E8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5B28);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5B28) { __imp__sub_825A5B28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5B28) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,24544(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24544);
	// lwz r9,0(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// lwz r8,4(r10)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// srawi r10,r9,8
	ctx.xer.ca = (ctx.r9.s32 < 0) & ((ctx.r9.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r9.s32 >> 8;
	// clrlwi r9,r9,24
	ctx.r9.u64 = ctx.r9.u32 & 0xFF;
	// lwz r7,4(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r7,r7,r10
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r7.u32 + ctx.r10.u32);
	// cmpw cr6,r7,r9
	ctx.cr6.compare<int32_t>(ctx.r7.s32, ctx.r9.s32, ctx.xer);
	// bnelr cr6
	if (!ctx.cr6.eq) return;
	// lwz r7,12(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r7,r10
	ctx.r11.s64 = int64_t(ctx.r7.s32) * int64_t(ctx.r10.s32);
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// clrlwi r4,r8,24
	ctx.r4.u64 = ctx.r8.u32 & 0xFF;
	// addi r3,r11,528
	ctx.r3.s64 = ctx.r11.s64 + 528;
	// b 0x825dc960
	sub_825DC960(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5B80);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5B80) { __imp__sub_825A5B80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5B80) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32066
	ctx.r10.s64 = -2101477376;
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
	// stb r11,14021(r10)
	PPC_STORE_U8(ctx.r10.u32 + 14021, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5BB0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5BB0) { __imp__sub_825A5BB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5BB0) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32048
	ctx.r11.s64 = -2100297728;
	// addi r3,r11,-25088
	ctx.r3.s64 = ctx.r11.s64 + -25088;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// b 0x822fa520
	sub_822FA520(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5BC8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5BC8) { __imp__sub_825A5BC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5BC8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32048
	ctx.r11.s64 = -2100297728;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r11,-25088
	ctx.r3.s64 = ctx.r11.s64 + -25088;
	// bl 0x822fa5d0
	ctx.lr = 0x825A5BE8;
	sub_822FA5D0(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A5C08);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5C08) { __imp__sub_825A5C08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5C08) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32002
	ctx.r11.s64 = -2097283072;
	// addi r3,r11,-22960
	ctx.r3.s64 = ctx.r11.s64 + -22960;
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r4,r11,24
	ctx.r4.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82608630
	sub_82608630(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5C30);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5C30) { __imp__sub_825A5C30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5C30) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// lwz r9,0(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// lwz r10,4(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// lwz r11,24544(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24544);
	// cntlzw r10,r10
	ctx.r10.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// rlwinm r10,r10,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// xori r8,r10,1
	ctx.r8.u64 = ctx.r10.u64 ^ 1;
	// lwz r7,4(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// srawi r10,r9,8
	ctx.xer.ca = (ctx.r9.s32 < 0) & ((ctx.r9.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r9.s32 >> 8;
	// clrlwi r4,r8,24
	ctx.r4.u64 = ctx.r8.u32 & 0xFF;
	// clrlwi r9,r9,24
	ctx.r9.u64 = ctx.r9.u32 & 0xFF;
	// lbzx r8,r7,r10
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r7.u32 + ctx.r10.u32);
	// cmpw cr6,r8,r9
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r9.s32, ctx.xer);
	// bnelr cr6
	if (!ctx.cr6.eq) return;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// addi r3,r11,528
	ctx.r3.s64 = ctx.r11.s64 + 528;
	// b 0x825db3a0
	sub_825DB3A0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5C90);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5C90) { __imp__sub_825A5C90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5C90) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32005
	ctx.r10.s64 = -2097479680;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stw r11,-9208(r10)
	PPC_STORE_U32(ctx.r10.u32 + -9208, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5CA8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5CA8) { __imp__sub_825A5CA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5CA8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32005
	ctx.r10.s64 = -2097479680;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stw r11,-9204(r10)
	PPC_STORE_U32(ctx.r10.u32 + -9204, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5CC0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5CC0) { __imp__sub_825A5CC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5CC0) {
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
	// stb r11,-1824(r10)
	PPC_STORE_U8(ctx.r10.u32 + -1824, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5CE0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5CE0) { __imp__sub_825A5CE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5CE0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// addi r10,r10,-28104
	ctx.r10.s64 = ctx.r10.s64 + -28104;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,124(r10)
	PPC_STORE_U8(ctx.r10.u32 + 124, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5D08);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5D08) { __imp__sub_825A5D08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5D08) {
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
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r31,r11,-9200
	ctx.r31.s64 = ctx.r11.s64 + -9200;
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// bl 0x825fdf88
	ctx.lr = 0x825A5D38;
	sub_825FDF88(ctx, base);
	// lwz r11,0(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// stw r31,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r31.u32);
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

PPC_FUNC_IMPL(__imp__sub_825A5D58);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5D58) { __imp__sub_825A5D58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5D58) {
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
	// lis r10,-32003
	ctx.r10.s64 = -2097348608;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r3,r10,17696
	ctx.r3.s64 = ctx.r10.s64 + 17696;
	// lfs f13,8(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f13.f64 = double(temp.f32);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f0,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// lfs f13,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f13.f64 = double(temp.f32);
	// stfs f13,88(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// addi r10,r1,80
	ctx.r10.s64 = ctx.r1.s64 + 80;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x825fff70
	ctx.lr = 0x825A5DA4;
	sub_825FFF70(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5DB8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5DB8) { __imp__sub_825A5DB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5DB8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32062
	ctx.r11.s64 = -2101215232;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r11,-28104
	ctx.r3.s64 = ctx.r11.s64 + -28104;
	// bl 0x8225ac48
	ctx.lr = 0x825A5DD8;
	sub_8225AC48(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A5DF8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5DF8) { __imp__sub_825A5DF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5DF8) {
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
	// lis r10,-32015
	ctx.r10.s64 = -2098135040;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r3,r10,19744
	ctx.r3.s64 = ctx.r10.s64 + 19744;
	// lfs f13,8(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f13.f64 = double(temp.f32);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f0,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// lfs f13,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f13.f64 = double(temp.f32);
	// stfs f13,88(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// addi r10,r1,80
	ctx.r10.s64 = ctx.r1.s64 + 80;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x823f1538
	ctx.lr = 0x825A5E44;
	sub_823F1538(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5E58);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5E58) { __imp__sub_825A5E58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5E58) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r4,0
	ctx.r4.s64 = 0;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// bl 0x82265788
	ctx.lr = 0x825A5E80;
	sub_82265788(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A5EA0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5EA0) { __imp__sub_825A5EA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5EA0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x825a3e48
	sub_825A3E48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5EB8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5EB8) { __imp__sub_825A5EB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5EB8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// mr r9,r11
	ctx.r9.u64 = ctx.r11.u64;
	// lwz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r9,20(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + 20);
	// cntlzw r10,r10
	ctx.r10.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// cntlzw r9,r9
	ctx.r9.u64 = ctx.r9.u32 == 0 ? 32 : __builtin_clz(ctx.r9.u32);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// rlwinm r10,r10,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// rlwinm r9,r9,27,31,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// xori r9,r9,1
	ctx.r9.u64 = ctx.r9.u64 ^ 1;
	// clrlwi r7,r10,24
	ctx.r7.u64 = ctx.r10.u32 & 0xFF;
	// clrlwi r8,r9,24
	ctx.r8.u64 = ctx.r9.u32 & 0xFF;
	// b 0x825a40f8
	sub_825A40F8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5F00);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5F00) { __imp__sub_825A5F00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5F00) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r31,-32005
	ctx.r31.s64 = -2097479680;
	// lis r10,-32003
	ctx.r10.s64 = -2097348608;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r3,r10,17696
	ctx.r3.s64 = ctx.r10.s64 + 17696;
	// li r8,9
	ctx.r8.s64 = 9;
	// lwz r4,-9216(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + -9216);
	// lwz r5,0(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r6,4(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x825fda50
	ctx.lr = 0x825A5F3C;
	sub_825FDA50(ctx, base);
	// lwz r11,-9216(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + -9216);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// stw r11,-9216(r31)
	PPC_STORE_U32(ctx.r31.u32 + -9216, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5F60);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5F60) { __imp__sub_825A5F60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5F60) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r10,-32003
	ctx.r10.s64 = -2097348608;
	// lis r31,-32005
	ctx.r31.s64 = -2097479680;
	// addi r3,r10,17696
	ctx.r3.s64 = ctx.r10.s64 + 17696;
	// lwz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r4,-9216(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + -9216);
	// clrlwi r9,r10,24
	ctx.r9.u64 = ctx.r10.u32 & 0xFF;
	// lwz r5,0(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r6,4(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// bl 0x825fda50
	ctx.lr = 0x825A5FA0;
	sub_825FDA50(ctx, base);
	// lwz r11,-9216(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + -9216);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// stw r11,-9216(r31)
	PPC_STORE_U32(ctx.r31.u32 + -9216, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A5FC0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A5FC0) { __imp__sub_825A5FC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A5FC0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r5,0
	ctx.r5.s64 = 0;
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r6,8(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// beq cr6,0x825a6014
	if (ctx.cr6.eq) goto loc_825A6014;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// srawi r9,r10,8
	ctx.xer.ca = (ctx.r10.s32 < 0) & ((ctx.r10.u32 & 0xFF) != 0);
	ctx.r9.s64 = ctx.r10.s32 >> 8;
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r8,r8,r9
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r8.u32 + ctx.r9.u32);
	// cmpw cr6,r8,r10
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r10.s32, ctx.xer);
	// bne cr6,0x825a6010
	if (!ctx.cr6.eq) goto loc_825A6010;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r9
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r9.s32);
	// add r5,r11,r10
	ctx.r5.u64 = ctx.r11.u64 + ctx.r10.u64;
	// b 0x825a6014
	goto loc_825A6014;
loc_825A6010:
	// li r5,0
	ctx.r5.s64 = 0;
loc_825A6014:
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// b 0x825fdaa8
	sub_825FDAA8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A6020);  /* forward decl */
PPC_WEAK_FUNC(sub_825A6020) { __imp__sub_825A6020(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A6020) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32003
	ctx.r10.s64 = -2097348608;
	// addi r3,r10,17696
	ctx.r3.s64 = ctx.r10.s64 + 17696;
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825fdb28
	sub_825FDB28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A6038);  /* forward decl */
PPC_WEAK_FUNC(sub_825A6038) { __imp__sub_825A6038(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A6038) {
	PPC_FUNC_PROLOGUE();
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
	// li r4,0
	ctx.r4.s64 = 0;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x827df490
	ctx.lr = 0x825A6054;
	sub_827DF490(ctx, base);
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r11,r11,17696
	ctx.r11.s64 = ctx.r11.s64 + 17696;
	// li r5,0
	ctx.r5.s64 = 0;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x825fd7f8
	ctx.lr = 0x825A606C;
	sub_825FD7F8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A6080);  /* forward decl */
PPC_WEAK_FUNC(sub_825A6080) { __imp__sub_825A6080(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A6080) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825a41f0
	sub_825A41F0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A6090);  /* forward decl */
PPC_WEAK_FUNC(sub_825A6090) { __imp__sub_825A6090(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A6090) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825a4278
	sub_825A4278(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A60A0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A60A0) { __imp__sub_825A60A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A60A0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825a4300
	sub_825A4300(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A60B0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A60B0) { __imp__sub_825A60B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A60B0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x825ff298
	ctx.lr = 0x825A60D8;
	sub_825FF298(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A60F8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A60F8) { __imp__sub_825A60F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A60F8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x825ff4f0
	ctx.lr = 0x825A6120;
	sub_825FF4F0(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A6140);  /* forward decl */
PPC_WEAK_FUNC(sub_825A6140) { __imp__sub_825A6140(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A6140) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32003
	ctx.r10.s64 = -2097348608;
	// addi r3,r10,17696
	ctx.r3.s64 = ctx.r10.s64 + 17696;
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x82600050
	sub_82600050(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A6158);  /* forward decl */
PPC_WEAK_FUNC(sub_825A6158) { __imp__sub_825A6158(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A6158) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32003
	ctx.r10.s64 = -2097348608;
	// addi r3,r10,17696
	ctx.r3.s64 = ctx.r10.s64 + 17696;
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x82601760
	sub_82601760(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A6170);  /* forward decl */
PPC_WEAK_FUNC(sub_825A6170) { __imp__sub_825A6170(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A6170) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r10,8(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// srawi r9,r10,8
	ctx.xer.ca = (ctx.r10.s32 < 0) & ((ctx.r10.u32 & 0xFF) != 0);
	ctx.r9.s64 = ctx.r10.s32 >> 8;
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r8,r8,r9
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r8.u32 + ctx.r9.u32);
	// cmpw cr6,r8,r10
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r10.s32, ctx.xer);
	// bne cr6,0x825a61bc
	if (!ctx.cr6.eq) goto loc_825A61BC;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r9
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r9.s32);
	// add r6,r11,r10
	ctx.r6.u64 = ctx.r11.u64 + ctx.r10.u64;
	// b 0x825a61c0
	goto loc_825A61C0;
loc_825A61BC:
	// li r6,0
	ctx.r6.s64 = 0;
loc_825A61C0:
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// b 0x82601758
	sub_82601758(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A61D0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A61D0) { __imp__sub_825A61D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A61D0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r10,8(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// srawi r9,r10,8
	ctx.xer.ca = (ctx.r10.s32 < 0) & ((ctx.r10.u32 & 0xFF) != 0);
	ctx.r9.s64 = ctx.r10.s32 >> 8;
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// lwz r11,24544(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24544);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r8,r8,r9
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r8.u32 + ctx.r9.u32);
	// cmpw cr6,r8,r10
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r10.s32, ctx.xer);
	// bne cr6,0x825a621c
	if (!ctx.cr6.eq) goto loc_825A621C;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r9
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r9.s32);
	// add r6,r11,r10
	ctx.r6.u64 = ctx.r11.u64 + ctx.r10.u64;
	// b 0x825a6220
	goto loc_825A6220;
loc_825A621C:
	// li r6,0
	ctx.r6.s64 = 0;
loc_825A6220:
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// b 0x82601758
	sub_82601758(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A6230);  /* forward decl */
PPC_WEAK_FUNC(sub_825A6230) { __imp__sub_825A6230(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A6230) {
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
	// lis r10,-32003
	ctx.r10.s64 = -2097348608;
	// addi r6,r1,80
	ctx.r6.s64 = ctx.r1.s64 + 80;
	// addi r3,r10,17696
	ctx.r3.s64 = ctx.r10.s64 + 17696;
	// lfs f0,8(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f13,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,16(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f12.f64 = double(temp.f32);
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f12,88(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// addi r10,r1,80
	ctx.r10.s64 = ctx.r1.s64 + 80;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x825ffed0
	ctx.lr = 0x825A6280;
	sub_825FFED0(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A6290);  /* forward decl */
PPC_WEAK_FUNC(sub_825A6290) { __imp__sub_825A6290(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A6290) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r10,8(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// srawi r9,r10,8
	ctx.xer.ca = (ctx.r10.s32 < 0) & ((ctx.r10.u32 & 0xFF) != 0);
	ctx.r9.s64 = ctx.r10.s32 >> 8;
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// lwz r11,88(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 88);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r8,r8,r9
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r8.u32 + ctx.r9.u32);
	// cmpw cr6,r8,r10
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r10.s32, ctx.xer);
	// bne cr6,0x825a62dc
	if (!ctx.cr6.eq) goto loc_825A62DC;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r9
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r9.s32);
	// add r6,r11,r10
	ctx.r6.u64 = ctx.r11.u64 + ctx.r10.u64;
	// b 0x825a62e0
	goto loc_825A62E0;
loc_825A62DC:
	// li r6,0
	ctx.r6.s64 = 0;
loc_825A62E0:
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// b 0x82601758
	sub_82601758(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A62F0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A62F0) { __imp__sub_825A62F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A62F0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32003
	ctx.r10.s64 = -2097348608;
	// addi r3,r10,17696
	ctx.r3.s64 = ctx.r10.s64 + 17696;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f1,8(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f1.f64 = double(temp.f32);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x82600208
	sub_82600208(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A6310);  /* forward decl */
PPC_WEAK_FUNC(sub_825A6310) { __imp__sub_825A6310(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A6310) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
	// lwz r9,8(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,12(r10)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 12);
	// cntlzw r9,r9
	ctx.r9.u64 = ctx.r9.u32 == 0 ? 32 : __builtin_clz(ctx.r9.u32);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// cntlzw r11,r8
	ctx.r11.u64 = ctx.r8.u32 == 0 ? 32 : __builtin_clz(ctx.r8.u32);
	// lwz r7,16(r10)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r10.u32 + 16);
	// rlwinm r9,r9,27,31,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r9,r9,1
	ctx.r9.u64 = ctx.r9.u64 ^ 1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r5,r9,24
	ctx.r5.u64 = ctx.r9.u32 & 0xFF;
	// clrlwi r6,r11,24
	ctx.r6.u64 = ctx.r11.u32 & 0xFF;
	// b 0x825a4388
	sub_825A4388(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A6350);  /* forward decl */
PPC_WEAK_FUNC(sub_825A6350) { __imp__sub_825A6350(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A6350) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
	// lwz r9,12(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,16(r10)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 16);
	// cntlzw r9,r9
	ctx.r9.u64 = ctx.r9.u32 == 0 ? 32 : __builtin_clz(ctx.r9.u32);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// cntlzw r7,r8
	ctx.r7.u64 = ctx.r8.u32 == 0 ? 32 : __builtin_clz(ctx.r8.u32);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// rlwinm r9,r9,27,31,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// lwz r8,20(r10)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 20);
	// rlwinm r11,r7,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 27) & 0x1;
	// xori r9,r9,1
	ctx.r9.u64 = ctx.r9.u64 ^ 1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r6,r9,24
	ctx.r6.u64 = ctx.r9.u32 & 0xFF;
	// clrlwi r7,r11,24
	ctx.r7.u64 = ctx.r11.u32 & 0xFF;
	// b 0x825a4498
	sub_825A4498(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A6398);  /* forward decl */
PPC_WEAK_FUNC(sub_825A6398) { __imp__sub_825A6398(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A6398) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825a4548
	sub_825A4548(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A63A8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A63A8) { __imp__sub_825A63A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A63A8) {
	PPC_FUNC_PROLOGUE();
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
	// li r4,0
	ctx.r4.s64 = 0;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x827df490
	ctx.lr = 0x825A63C4;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29736(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29736, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A63E0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A63E0) { __imp__sub_825A63E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A63E0) {
	PPC_FUNC_PROLOGUE();
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
	// li r4,0
	ctx.r4.s64 = 0;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x827df490
	ctx.lr = 0x825A63FC;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29740(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29740, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A6418);  /* forward decl */
PPC_WEAK_FUNC(sub_825A6418) { __imp__sub_825A6418(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A6418) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r3,r11,-3248
	ctx.r3.s64 = ctx.r11.s64 + -3248;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// b 0x825c30f0
	sub_825C30F0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A6430);  /* forward decl */
PPC_WEAK_FUNC(sub_825A6430) { __imp__sub_825A6430(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A6430) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825a4a68
	sub_825A4A68(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A6440);  /* forward decl */
PPC_WEAK_FUNC(sub_825A6440) { __imp__sub_825A6440(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A6440) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32062
	ctx.r11.s64 = -2101215232;
	// addi r3,r11,-28104
	ctx.r3.s64 = ctx.r11.s64 + -28104;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// b 0x8225cad0
	sub_8225CAD0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A6458);  /* forward decl */
PPC_WEAK_FUNC(sub_825A6458) { __imp__sub_825A6458(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A6458) {
	PPC_FUNC_PROLOGUE();
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
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x825fbba8
	ctx.lr = 0x825A6470;
	sub_825FBBA8(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,5875(r3)
	PPC_STORE_U8(ctx.r3.u32 + 5875, ctx.r11.u8);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A6488);  /* forward decl */
PPC_WEAK_FUNC(sub_825A6488) { __imp__sub_825A6488(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A6488) {
	PPC_FUNC_PROLOGUE();
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
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x825fbba8
	ctx.lr = 0x825A64A0;
	sub_825FBBA8(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// stb r11,5875(r3)
	PPC_STORE_U8(ctx.r3.u32 + 5875, ctx.r11.u8);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A64B8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A64B8) { __imp__sub_825A64B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A64B8) {
	PPC_FUNC_PROLOGUE();
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
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x825fbba8
	ctx.lr = 0x825A64D0;
	sub_825FBBA8(ctx, base);
	// lis r11,-32062
	ctx.r11.s64 = -2101215232;
	// lbz r10,5860(r3)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r3.u32 + 5860);
	// addi r11,r11,-28104
	ctx.r11.s64 = ctx.r11.s64 + -28104;
	// stw r10,104(r11)
	PPC_STORE_U32(ctx.r11.u32 + 104, ctx.r10.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A64F0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A64F0) { __imp__sub_825A64F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A64F0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x825A64F8;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r31,8(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r30,12(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r29,4(r11)
	ctx.r29.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x825fbba8
	ctx.lr = 0x825A6514;
	sub_825FBBA8(ctx, base);
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// mr r6,r30
	ctx.r6.u64 = ctx.r30.u64;
	// bl 0x825fa2d0
	ctx.lr = 0x825A6524;
	sub_825FA2D0(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A6530);  /* forward decl */
PPC_WEAK_FUNC(sub_825A6530) { __imp__sub_825A6530(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A6530) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32015
	ctx.r11.s64 = -2098135040;
	// addi r3,r11,19744
	ctx.r3.s64 = ctx.r11.s64 + 19744;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// b 0x823f0178
	sub_823F0178(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A6548);  /* forward decl */
PPC_WEAK_FUNC(sub_825A6548) { __imp__sub_825A6548(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A6548) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r3,r11,17696
	ctx.r3.s64 = ctx.r11.s64 + 17696;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x82600790
	ctx.lr = 0x825A6574;
	sub_82600790(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A6598);  /* forward decl */
PPC_WEAK_FUNC(sub_825A6598) { __imp__sub_825A6598(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A6598) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r10,-32003
	ctx.r10.s64 = -2097348608;
	// addi r3,r10,17696
	ctx.r3.s64 = ctx.r10.s64 + 17696;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x82600790
	ctx.lr = 0x825A65C4;
	sub_82600790(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825A65E8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A65E8) { __imp__sub_825A65E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A65E8) {
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
	// li r10,-1
	ctx.r10.s64 = -1;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// lwz r31,4(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stw r10,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r10.u32);
	// bl 0x8219ea28
	ctx.lr = 0x825A6614;
	sub_8219EA28(ctx, base);
	// lwz r11,80(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// cmplwi cr6,r11,30999
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 30999, ctx.xer);
	// bgt cr6,0x825a6644
	if (ctx.cr6.gt) goto loc_825A6644;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x827df490
	ctx.lr = 0x825A662C;
	sub_827DF490(ctx, base);
	// lis r11,-32048
	ctx.r11.s64 = -2100297728;
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
	// lwz r4,80(r1)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// addi r11,r11,-25088
	ctx.r11.s64 = ctx.r11.s64 + -25088;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x822fb488
	ctx.lr = 0x825A6644;
	sub_822FB488(ctx, base);
loc_825A6644:
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

PPC_FUNC_IMPL(__imp__sub_825A6658);  /* forward decl */
PPC_WEAK_FUNC(sub_825A6658) { __imp__sub_825A6658(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A6658) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16088
	ctx.r4.s64 = ctx.r11.s64 + 16088;
	// addi r3,r10,-18756
	ctx.r3.s64 = ctx.r10.s64 + -18756;
	// bl 0x827d6c90
	ctx.lr = 0x825A6678;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,19176
	ctx.r4.s64 = ctx.r11.s64 + 19176;
	// addi r3,r10,-18792
	ctx.r3.s64 = ctx.r10.s64 + -18792;
	// bl 0x827d6c90
	ctx.lr = 0x825A668C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16112
	ctx.r4.s64 = ctx.r11.s64 + 16112;
	// addi r3,r10,-18820
	ctx.r3.s64 = ctx.r10.s64 + -18820;
	// bl 0x827d6c90
	ctx.lr = 0x825A66A0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16128
	ctx.r4.s64 = ctx.r11.s64 + 16128;
	// addi r3,r10,-18848
	ctx.r3.s64 = ctx.r10.s64 + -18848;
	// bl 0x827d6c90
	ctx.lr = 0x825A66B4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,19200
	ctx.r4.s64 = ctx.r11.s64 + 19200;
	// addi r3,r10,-18872
	ctx.r3.s64 = ctx.r10.s64 + -18872;
	// bl 0x827d6c90
	ctx.lr = 0x825A66C8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,19224
	ctx.r4.s64 = ctx.r11.s64 + 19224;
	// addi r3,r10,-18892
	ctx.r3.s64 = ctx.r10.s64 + -18892;
	// bl 0x827d6c90
	ctx.lr = 0x825A66DC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16144
	ctx.r4.s64 = ctx.r11.s64 + 16144;
	// addi r3,r10,-18916
	ctx.r3.s64 = ctx.r10.s64 + -18916;
	// bl 0x827d6c90
	ctx.lr = 0x825A66F0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,19248
	ctx.r4.s64 = ctx.r11.s64 + 19248;
	// addi r3,r10,-18944
	ctx.r3.s64 = ctx.r10.s64 + -18944;
	// bl 0x827d6c90
	ctx.lr = 0x825A6704;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,19344
	ctx.r4.s64 = ctx.r11.s64 + 19344;
	// addi r3,r10,-18968
	ctx.r3.s64 = ctx.r10.s64 + -18968;
	// bl 0x827d6c90
	ctx.lr = 0x825A6718;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,19432
	ctx.r4.s64 = ctx.r11.s64 + 19432;
	// addi r3,r10,-18996
	ctx.r3.s64 = ctx.r10.s64 + -18996;
	// bl 0x827d6c90
	ctx.lr = 0x825A672C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16160
	ctx.r4.s64 = ctx.r11.s64 + 16160;
	// addi r3,r10,-19020
	ctx.r3.s64 = ctx.r10.s64 + -19020;
	// bl 0x827d6c90
	ctx.lr = 0x825A6740;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,24224
	ctx.r4.s64 = ctx.r11.s64 + 24224;
	// addi r3,r10,-19052
	ctx.r3.s64 = ctx.r10.s64 + -19052;
	// bl 0x827d6c90
	ctx.lr = 0x825A6754;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,24248
	ctx.r4.s64 = ctx.r11.s64 + 24248;
	// addi r3,r10,-19076
	ctx.r3.s64 = ctx.r10.s64 + -19076;
	// bl 0x827d6c90
	ctx.lr = 0x825A6768;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,19528
	ctx.r4.s64 = ctx.r11.s64 + 19528;
	// addi r3,r10,-19108
	ctx.r3.s64 = ctx.r10.s64 + -19108;
	// bl 0x827d6c90
	ctx.lr = 0x825A677C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16160
	ctx.r4.s64 = ctx.r11.s64 + 16160;
	// addi r3,r10,-19136
	ctx.r3.s64 = ctx.r10.s64 + -19136;
	// bl 0x827d6c90
	ctx.lr = 0x825A6790;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,24320
	ctx.r4.s64 = ctx.r11.s64 + 24320;
	// addi r3,r10,-19172
	ctx.r3.s64 = ctx.r10.s64 + -19172;
	// bl 0x827d6c90
	ctx.lr = 0x825A67A4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,24416
	ctx.r4.s64 = ctx.r11.s64 + 24416;
	// addi r3,r10,-19200
	ctx.r3.s64 = ctx.r10.s64 + -19200;
	// bl 0x827d6c90
	ctx.lr = 0x825A67B8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,24512
	ctx.r4.s64 = ctx.r11.s64 + 24512;
	// addi r3,r10,-19232
	ctx.r3.s64 = ctx.r10.s64 + -19232;
	// bl 0x827d6c90
	ctx.lr = 0x825A67CC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,24608
	ctx.r4.s64 = ctx.r11.s64 + 24608;
	// addi r3,r10,-19272
	ctx.r3.s64 = ctx.r10.s64 + -19272;
	// bl 0x827d6c90
	ctx.lr = 0x825A67E0;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,-19300
	ctx.r3.s64 = ctx.r10.s64 + -19300;
	// bl 0x827d6c90
	ctx.lr = 0x825A67F4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,19648
	ctx.r4.s64 = ctx.r11.s64 + 19648;
	// addi r3,r10,-19328
	ctx.r3.s64 = ctx.r10.s64 + -19328;
	// bl 0x827d6c90
	ctx.lr = 0x825A6808;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,19712
	ctx.r4.s64 = ctx.r11.s64 + 19712;
	// addi r3,r10,-19364
	ctx.r3.s64 = ctx.r10.s64 + -19364;
	// bl 0x827d6c90
	ctx.lr = 0x825A681C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,19832
	ctx.r4.s64 = ctx.r11.s64 + 19832;
	// addi r3,r10,-19404
	ctx.r3.s64 = ctx.r10.s64 + -19404;
	// bl 0x827d6c90
	ctx.lr = 0x825A6830;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,19896
	ctx.r4.s64 = ctx.r11.s64 + 19896;
	// addi r3,r10,-19432
	ctx.r3.s64 = ctx.r10.s64 + -19432;
	// bl 0x827d6c90
	ctx.lr = 0x825A6844;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16184
	ctx.r4.s64 = ctx.r11.s64 + 16184;
	// addi r3,r10,-19464
	ctx.r3.s64 = ctx.r10.s64 + -19464;
	// bl 0x827d6c90
	ctx.lr = 0x825A6858;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,19936
	ctx.r4.s64 = ctx.r11.s64 + 19936;
	// addi r3,r10,-19492
	ctx.r3.s64 = ctx.r10.s64 + -19492;
	// bl 0x827d6c90
	ctx.lr = 0x825A686C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16200
	ctx.r4.s64 = ctx.r11.s64 + 16200;
	// addi r3,r10,-19532
	ctx.r3.s64 = ctx.r10.s64 + -19532;
	// bl 0x827d6c90
	ctx.lr = 0x825A6880;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,23816
	ctx.r4.s64 = ctx.r11.s64 + 23816;
	// addi r3,r10,-19572
	ctx.r3.s64 = ctx.r10.s64 + -19572;
	// bl 0x827d6c90
	ctx.lr = 0x825A6894;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,24632
	ctx.r4.s64 = ctx.r11.s64 + 24632;
	// addi r3,r10,-19592
	ctx.r3.s64 = ctx.r10.s64 + -19592;
	// bl 0x827d6c90
	ctx.lr = 0x825A68A8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,24704
	ctx.r4.s64 = ctx.r11.s64 + 24704;
	// addi r3,r10,-19620
	ctx.r3.s64 = ctx.r10.s64 + -19620;
	// bl 0x827d6c90
	ctx.lr = 0x825A68BC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,24720
	ctx.r4.s64 = ctx.r11.s64 + 24720;
	// addi r3,r10,-19652
	ctx.r3.s64 = ctx.r10.s64 + -19652;
	// bl 0x827d6c90
	ctx.lr = 0x825A68D0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,24736
	ctx.r4.s64 = ctx.r11.s64 + 24736;
	// addi r3,r10,-19684
	ctx.r3.s64 = ctx.r10.s64 + -19684;
	// bl 0x827d6c90
	ctx.lr = 0x825A68E4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// addi r4,r11,20024
	ctx.r4.s64 = ctx.r11.s64 + 20024;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r3,r11,-19712
	ctx.r3.s64 = ctx.r11.s64 + -19712;
	// bl 0x827d6c90
	ctx.lr = 0x825A68F8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,20064
	ctx.r4.s64 = ctx.r11.s64 + 20064;
	// addi r3,r10,-19744
	ctx.r3.s64 = ctx.r10.s64 + -19744;
	// bl 0x827d6c90
	ctx.lr = 0x825A690C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,24752
	ctx.r4.s64 = ctx.r11.s64 + 24752;
	// addi r3,r10,-19772
	ctx.r3.s64 = ctx.r10.s64 + -19772;
	// bl 0x827d6c90
	ctx.lr = 0x825A6920;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,24824
	ctx.r4.s64 = ctx.r11.s64 + 24824;
	// addi r3,r10,-19800
	ctx.r3.s64 = ctx.r10.s64 + -19800;
	// bl 0x827d6c90
	ctx.lr = 0x825A6934;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16216
	ctx.r4.s64 = ctx.r11.s64 + 16216;
	// addi r3,r10,-19836
	ctx.r3.s64 = ctx.r10.s64 + -19836;
	// bl 0x827d6c90
	ctx.lr = 0x825A6948;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16232
	ctx.r4.s64 = ctx.r11.s64 + 16232;
	// addi r3,r10,-19872
	ctx.r3.s64 = ctx.r10.s64 + -19872;
	// bl 0x827d6c90
	ctx.lr = 0x825A695C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,20120
	ctx.r4.s64 = ctx.r11.s64 + 20120;
	// addi r3,r10,-19888
	ctx.r3.s64 = ctx.r10.s64 + -19888;
	// bl 0x827d6c90
	ctx.lr = 0x825A6970;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,20184
	ctx.r4.s64 = ctx.r11.s64 + 20184;
	// addi r3,r10,-19908
	ctx.r3.s64 = ctx.r10.s64 + -19908;
	// bl 0x827d6c90
	ctx.lr = 0x825A6984;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,24896
	ctx.r4.s64 = ctx.r11.s64 + 24896;
	// addi r3,r10,-19920
	ctx.r3.s64 = ctx.r10.s64 + -19920;
	// bl 0x827d6c90
	ctx.lr = 0x825A6998;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,24920
	ctx.r4.s64 = ctx.r11.s64 + 24920;
	// addi r3,r10,-19940
	ctx.r3.s64 = ctx.r10.s64 + -19940;
	// bl 0x827d6c90
	ctx.lr = 0x825A69AC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,24944
	ctx.r4.s64 = ctx.r11.s64 + 24944;
	// addi r3,r10,-19960
	ctx.r3.s64 = ctx.r10.s64 + -19960;
	// bl 0x827d6c90
	ctx.lr = 0x825A69C0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,25040
	ctx.r4.s64 = ctx.r11.s64 + 25040;
	// addi r3,r10,-19984
	ctx.r3.s64 = ctx.r10.s64 + -19984;
	// bl 0x827d6c90
	ctx.lr = 0x825A69D4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,25136
	ctx.r4.s64 = ctx.r11.s64 + 25136;
	// addi r3,r10,-20012
	ctx.r3.s64 = ctx.r10.s64 + -20012;
	// bl 0x827d6c90
	ctx.lr = 0x825A69E8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,23896
	ctx.r4.s64 = ctx.r11.s64 + 23896;
	// addi r3,r10,-20044
	ctx.r3.s64 = ctx.r10.s64 + -20044;
	// bl 0x827d6c90
	ctx.lr = 0x825A69FC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,25232
	ctx.r4.s64 = ctx.r11.s64 + 25232;
	// addi r3,r10,-20068
	ctx.r3.s64 = ctx.r10.s64 + -20068;
	// bl 0x827d6c90
	ctx.lr = 0x825A6A10;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,20208
	ctx.r4.s64 = ctx.r11.s64 + 20208;
	// addi r3,r10,-20080
	ctx.r3.s64 = ctx.r10.s64 + -20080;
	// bl 0x827d6c90
	ctx.lr = 0x825A6A24;
	sub_827D6C90(ctx, base);
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// addi r3,r10,-20104
	ctx.r3.s64 = ctx.r10.s64 + -20104;
	// addi r4,r11,25328
	ctx.r4.s64 = ctx.r11.s64 + 25328;
	// bl 0x827d6c90
	ctx.lr = 0x825A6A38;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,20232
	ctx.r4.s64 = ctx.r11.s64 + 20232;
	// addi r3,r10,-20124
	ctx.r3.s64 = ctx.r10.s64 + -20124;
	// bl 0x827d6c90
	ctx.lr = 0x825A6A4C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,25360
	ctx.r4.s64 = ctx.r11.s64 + 25360;
	// addi r3,r10,-20144
	ctx.r3.s64 = ctx.r10.s64 + -20144;
	// bl 0x827d6c90
	ctx.lr = 0x825A6A60;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,25424
	ctx.r4.s64 = ctx.r11.s64 + 25424;
	// addi r3,r10,-20176
	ctx.r3.s64 = ctx.r10.s64 + -20176;
	// bl 0x827d6c90
	ctx.lr = 0x825A6A74;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,25496
	ctx.r4.s64 = ctx.r11.s64 + 25496;
	// addi r3,r10,-20200
	ctx.r3.s64 = ctx.r10.s64 + -20200;
	// bl 0x827d6c90
	ctx.lr = 0x825A6A88;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,20304
	ctx.r4.s64 = ctx.r11.s64 + 20304;
	// addi r3,r10,-20220
	ctx.r3.s64 = ctx.r10.s64 + -20220;
	// bl 0x827d6c90
	ctx.lr = 0x825A6A9C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,20392
	ctx.r4.s64 = ctx.r11.s64 + 20392;
	// addi r3,r10,-20256
	ctx.r3.s64 = ctx.r10.s64 + -20256;
	// bl 0x827d6c90
	ctx.lr = 0x825A6AB0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,20432
	ctx.r4.s64 = ctx.r11.s64 + 20432;
	// addi r3,r10,-20296
	ctx.r3.s64 = ctx.r10.s64 + -20296;
	// bl 0x827d6c90
	ctx.lr = 0x825A6AC4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,20520
	ctx.r4.s64 = ctx.r11.s64 + 20520;
	// addi r3,r10,-20324
	ctx.r3.s64 = ctx.r10.s64 + -20324;
	// bl 0x827d6c90
	ctx.lr = 0x825A6AD8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,20672
	ctx.r4.s64 = ctx.r11.s64 + 20672;
	// addi r3,r10,-20352
	ctx.r3.s64 = ctx.r10.s64 + -20352;
	// bl 0x827d6c90
	ctx.lr = 0x825A6AEC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,20840
	ctx.r4.s64 = ctx.r11.s64 + 20840;
	// addi r3,r10,-20368
	ctx.r3.s64 = ctx.r10.s64 + -20368;
	// bl 0x827d6c90
	ctx.lr = 0x825A6B00;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,20992
	ctx.r4.s64 = ctx.r11.s64 + 20992;
	// addi r3,r10,-20392
	ctx.r3.s64 = ctx.r10.s64 + -20392;
	// bl 0x827d6c90
	ctx.lr = 0x825A6B14;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,21056
	ctx.r4.s64 = ctx.r11.s64 + 21056;
	// addi r3,r10,-20408
	ctx.r3.s64 = ctx.r10.s64 + -20408;
	// bl 0x827d6c90
	ctx.lr = 0x825A6B28;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,21072
	ctx.r4.s64 = ctx.r11.s64 + 21072;
	// addi r3,r10,-20424
	ctx.r3.s64 = ctx.r10.s64 + -20424;
	// bl 0x827d6c90
	ctx.lr = 0x825A6B3C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,21088
	ctx.r4.s64 = ctx.r11.s64 + 21088;
	// addi r3,r10,-20444
	ctx.r3.s64 = ctx.r10.s64 + -20444;
	// bl 0x827d6c90
	ctx.lr = 0x825A6B50;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,25512
	ctx.r4.s64 = ctx.r11.s64 + 25512;
	// addi r3,r10,-20468
	ctx.r3.s64 = ctx.r10.s64 + -20468;
	// bl 0x827d6c90
	ctx.lr = 0x825A6B64;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,25568
	ctx.r4.s64 = ctx.r11.s64 + 25568;
	// addi r3,r10,-20496
	ctx.r3.s64 = ctx.r10.s64 + -20496;
	// bl 0x827d6c90
	ctx.lr = 0x825A6B78;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,25624
	ctx.r4.s64 = ctx.r11.s64 + 25624;
	// addi r3,r10,-20528
	ctx.r3.s64 = ctx.r10.s64 + -20528;
	// bl 0x827d6c90
	ctx.lr = 0x825A6B8C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,21104
	ctx.r4.s64 = ctx.r11.s64 + 21104;
	// addi r3,r10,-20548
	ctx.r3.s64 = ctx.r10.s64 + -20548;
	// bl 0x827d6c90
	ctx.lr = 0x825A6BA0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,21208
	ctx.r4.s64 = ctx.r11.s64 + 21208;
	// addi r3,r10,-20572
	ctx.r3.s64 = ctx.r10.s64 + -20572;
	// bl 0x827d6c90
	ctx.lr = 0x825A6BB4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,25648
	ctx.r4.s64 = ctx.r11.s64 + 25648;
	// addi r3,r10,-20596
	ctx.r3.s64 = ctx.r10.s64 + -20596;
	// bl 0x827d6c90
	ctx.lr = 0x825A6BC8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,21312
	ctx.r4.s64 = ctx.r11.s64 + 21312;
	// addi r3,r10,-20628
	ctx.r3.s64 = ctx.r10.s64 + -20628;
	// bl 0x827d6c90
	ctx.lr = 0x825A6BDC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,21336
	ctx.r4.s64 = ctx.r11.s64 + 21336;
	// addi r3,r10,-20660
	ctx.r3.s64 = ctx.r10.s64 + -20660;
	// bl 0x827d6c90
	ctx.lr = 0x825A6BF0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,21376
	ctx.r4.s64 = ctx.r11.s64 + 21376;
	// addi r3,r10,-20692
	ctx.r3.s64 = ctx.r10.s64 + -20692;
	// bl 0x827d6c90
	ctx.lr = 0x825A6C04;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,23992
	ctx.r4.s64 = ctx.r11.s64 + 23992;
	// addi r3,r10,-20724
	ctx.r3.s64 = ctx.r10.s64 + -20724;
	// bl 0x827d6c90
	ctx.lr = 0x825A6C18;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,21400
	ctx.r4.s64 = ctx.r11.s64 + 21400;
	// addi r3,r10,-20744
	ctx.r3.s64 = ctx.r10.s64 + -20744;
	// bl 0x827d6c90
	ctx.lr = 0x825A6C2C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16248
	ctx.r4.s64 = ctx.r11.s64 + 16248;
	// addi r3,r10,-20760
	ctx.r3.s64 = ctx.r10.s64 + -20760;
	// bl 0x827d6c90
	ctx.lr = 0x825A6C40;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16304
	ctx.r4.s64 = ctx.r11.s64 + 16304;
	// addi r3,r10,-20780
	ctx.r3.s64 = ctx.r10.s64 + -20780;
	// bl 0x827d6c90
	ctx.lr = 0x825A6C54;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,25664
	ctx.r4.s64 = ctx.r11.s64 + 25664;
	// addi r3,r10,-20812
	ctx.r3.s64 = ctx.r10.s64 + -20812;
	// bl 0x827d6c90
	ctx.lr = 0x825A6C68;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,21456
	ctx.r4.s64 = ctx.r11.s64 + 21456;
	// addi r3,r10,-20844
	ctx.r3.s64 = ctx.r10.s64 + -20844;
	// bl 0x827d6c90
	ctx.lr = 0x825A6C7C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16360
	ctx.r4.s64 = ctx.r11.s64 + 16360;
	// addi r3,r10,-20868
	ctx.r3.s64 = ctx.r10.s64 + -20868;
	// bl 0x827d6c90
	ctx.lr = 0x825A6C90;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16376
	ctx.r4.s64 = ctx.r11.s64 + 16376;
	// addi r3,r10,-20892
	ctx.r3.s64 = ctx.r10.s64 + -20892;
	// bl 0x827d6c90
	ctx.lr = 0x825A6CA4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,21480
	ctx.r4.s64 = ctx.r11.s64 + 21480;
	// addi r3,r10,-20920
	ctx.r3.s64 = ctx.r10.s64 + -20920;
	// bl 0x827d6c90
	ctx.lr = 0x825A6CB8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,21496
	ctx.r4.s64 = ctx.r11.s64 + 21496;
	// addi r3,r10,-20948
	ctx.r3.s64 = ctx.r10.s64 + -20948;
	// bl 0x827d6c90
	ctx.lr = 0x825A6CCC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16392
	ctx.r4.s64 = ctx.r11.s64 + 16392;
	// addi r3,r10,-20972
	ctx.r3.s64 = ctx.r10.s64 + -20972;
	// bl 0x827d6c90
	ctx.lr = 0x825A6CE0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16400
	ctx.r4.s64 = ctx.r11.s64 + 16400;
	// addi r3,r10,-20992
	ctx.r3.s64 = ctx.r10.s64 + -20992;
	// bl 0x827d6c90
	ctx.lr = 0x825A6CF4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,21528
	ctx.r4.s64 = ctx.r11.s64 + 21528;
	// addi r3,r10,-21020
	ctx.r3.s64 = ctx.r10.s64 + -21020;
	// bl 0x827d6c90
	ctx.lr = 0x825A6D08;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,21584
	ctx.r4.s64 = ctx.r11.s64 + 21584;
	// addi r3,r10,-21052
	ctx.r3.s64 = ctx.r10.s64 + -21052;
	// bl 0x827d6c90
	ctx.lr = 0x825A6D1C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,21648
	ctx.r4.s64 = ctx.r11.s64 + 21648;
	// addi r3,r10,-21080
	ctx.r3.s64 = ctx.r10.s64 + -21080;
	// bl 0x827d6c90
	ctx.lr = 0x825A6D30;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16408
	ctx.r4.s64 = ctx.r11.s64 + 16408;
	// addi r3,r10,-21104
	ctx.r3.s64 = ctx.r10.s64 + -21104;
	// bl 0x827d6c90
	ctx.lr = 0x825A6D44;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16424
	ctx.r4.s64 = ctx.r11.s64 + 16424;
	// addi r3,r10,-21128
	ctx.r3.s64 = ctx.r10.s64 + -21128;
	// bl 0x827d6c90
	ctx.lr = 0x825A6D58;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16440
	ctx.r4.s64 = ctx.r11.s64 + 16440;
	// addi r3,r10,-21164
	ctx.r3.s64 = ctx.r10.s64 + -21164;
	// bl 0x827d6c90
	ctx.lr = 0x825A6D6C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16456
	ctx.r4.s64 = ctx.r11.s64 + 16456;
	// addi r3,r10,-21184
	ctx.r3.s64 = ctx.r10.s64 + -21184;
	// bl 0x827d6c90
	ctx.lr = 0x825A6D80;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,21664
	ctx.r4.s64 = ctx.r11.s64 + 21664;
	// addi r3,r10,-21212
	ctx.r3.s64 = ctx.r10.s64 + -21212;
	// bl 0x827d6c90
	ctx.lr = 0x825A6D94;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16472
	ctx.r4.s64 = ctx.r11.s64 + 16472;
	// addi r3,r10,-21228
	ctx.r3.s64 = ctx.r10.s64 + -21228;
	// bl 0x827d6c90
	ctx.lr = 0x825A6DA8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,25688
	ctx.r4.s64 = ctx.r11.s64 + 25688;
	// addi r3,r10,-21252
	ctx.r3.s64 = ctx.r10.s64 + -21252;
	// bl 0x827d6c90
	ctx.lr = 0x825A6DBC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,25736
	ctx.r4.s64 = ctx.r11.s64 + 25736;
	// addi r3,r10,-21276
	ctx.r3.s64 = ctx.r10.s64 + -21276;
	// bl 0x827d6c90
	ctx.lr = 0x825A6DD0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,25784
	ctx.r4.s64 = ctx.r11.s64 + 25784;
	// addi r3,r10,-21308
	ctx.r3.s64 = ctx.r10.s64 + -21308;
	// bl 0x827d6c90
	ctx.lr = 0x825A6DE4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,25840
	ctx.r4.s64 = ctx.r11.s64 + 25840;
	// addi r3,r10,-21328
	ctx.r3.s64 = ctx.r10.s64 + -21328;
	// bl 0x827d6c90
	ctx.lr = 0x825A6DF8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,25904
	ctx.r4.s64 = ctx.r11.s64 + 25904;
	// addi r3,r10,-21352
	ctx.r3.s64 = ctx.r10.s64 + -21352;
	// bl 0x827d6c90
	ctx.lr = 0x825A6E0C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16488
	ctx.r4.s64 = ctx.r11.s64 + 16488;
	// addi r3,r10,-21376
	ctx.r3.s64 = ctx.r10.s64 + -21376;
	// bl 0x827d6c90
	ctx.lr = 0x825A6E20;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16504
	ctx.r4.s64 = ctx.r11.s64 + 16504;
	// addi r3,r10,-21400
	ctx.r3.s64 = ctx.r10.s64 + -21400;
	// bl 0x827d6c90
	ctx.lr = 0x825A6E34;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,24056
	ctx.r4.s64 = ctx.r11.s64 + 24056;
	// addi r3,r10,-21424
	ctx.r3.s64 = ctx.r10.s64 + -21424;
	// bl 0x827d6c90
	ctx.lr = 0x825A6E48;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,24152
	ctx.r4.s64 = ctx.r11.s64 + 24152;
	// addi r3,r10,-21440
	ctx.r3.s64 = ctx.r10.s64 + -21440;
	// bl 0x827d6c90
	ctx.lr = 0x825A6E5C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,21728
	ctx.r4.s64 = ctx.r11.s64 + 21728;
	// addi r3,r10,-21476
	ctx.r3.s64 = ctx.r10.s64 + -21476;
	// bl 0x827d6c90
	ctx.lr = 0x825A6E70;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16520
	ctx.r4.s64 = ctx.r11.s64 + 16520;
	// addi r3,r10,-21516
	ctx.r3.s64 = ctx.r10.s64 + -21516;
	// bl 0x827d6c90
	ctx.lr = 0x825A6E84;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,21792
	ctx.r4.s64 = ctx.r11.s64 + 21792;
	// addi r3,r10,-21532
	ctx.r3.s64 = ctx.r10.s64 + -21532;
	// bl 0x827d6c90
	ctx.lr = 0x825A6E98;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,21888
	ctx.r4.s64 = ctx.r11.s64 + 21888;
	// addi r3,r10,-21556
	ctx.r3.s64 = ctx.r10.s64 + -21556;
	// bl 0x827d6c90
	ctx.lr = 0x825A6EAC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16536
	ctx.r4.s64 = ctx.r11.s64 + 16536;
	// addi r3,r10,-21596
	ctx.r3.s64 = ctx.r10.s64 + -21596;
	// bl 0x827d6c90
	ctx.lr = 0x825A6EC0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,21912
	ctx.r4.s64 = ctx.r11.s64 + 21912;
	// addi r3,r10,-21620
	ctx.r3.s64 = ctx.r10.s64 + -21620;
	// bl 0x827d6c90
	ctx.lr = 0x825A6ED4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,21984
	ctx.r4.s64 = ctx.r11.s64 + 21984;
	// addi r3,r10,-21644
	ctx.r3.s64 = ctx.r10.s64 + -21644;
	// bl 0x827d6c90
	ctx.lr = 0x825A6EE8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16552
	ctx.r4.s64 = ctx.r11.s64 + 16552;
	// addi r3,r10,-21664
	ctx.r3.s64 = ctx.r10.s64 + -21664;
	// bl 0x827d6c90
	ctx.lr = 0x825A6EFC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,22056
	ctx.r4.s64 = ctx.r11.s64 + 22056;
	// addi r3,r10,-21692
	ctx.r3.s64 = ctx.r10.s64 + -21692;
	// bl 0x827d6c90
	ctx.lr = 0x825A6F10;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,22096
	ctx.r4.s64 = ctx.r11.s64 + 22096;
	// addi r3,r10,-21728
	ctx.r3.s64 = ctx.r10.s64 + -21728;
	// bl 0x827d6c90
	ctx.lr = 0x825A6F24;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,22136
	ctx.r4.s64 = ctx.r11.s64 + 22136;
	// addi r3,r10,-21748
	ctx.r3.s64 = ctx.r10.s64 + -21748;
	// bl 0x827d6c90
	ctx.lr = 0x825A6F38;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// addi r4,r11,22176
	ctx.r4.s64 = ctx.r11.s64 + 22176;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r3,r11,-21772
	ctx.r3.s64 = ctx.r11.s64 + -21772;
	// bl 0x827d6c90
	ctx.lr = 0x825A6F4C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,22216
	ctx.r4.s64 = ctx.r11.s64 + 22216;
	// addi r3,r10,-21796
	ctx.r3.s64 = ctx.r10.s64 + -21796;
	// bl 0x827d6c90
	ctx.lr = 0x825A6F60;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,22256
	ctx.r4.s64 = ctx.r11.s64 + 22256;
	// addi r3,r10,-21840
	ctx.r3.s64 = ctx.r10.s64 + -21840;
	// bl 0x827d6c90
	ctx.lr = 0x825A6F74;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,22296
	ctx.r4.s64 = ctx.r11.s64 + 22296;
	// addi r3,r10,-21872
	ctx.r3.s64 = ctx.r10.s64 + -21872;
	// bl 0x827d6c90
	ctx.lr = 0x825A6F88;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,22352
	ctx.r4.s64 = ctx.r11.s64 + 22352;
	// addi r3,r10,-21896
	ctx.r3.s64 = ctx.r10.s64 + -21896;
	// bl 0x827d6c90
	ctx.lr = 0x825A6F9C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,22432
	ctx.r4.s64 = ctx.r11.s64 + 22432;
	// addi r3,r10,-21924
	ctx.r3.s64 = ctx.r10.s64 + -21924;
	// bl 0x827d6c90
	ctx.lr = 0x825A6FB0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,25928
	ctx.r4.s64 = ctx.r11.s64 + 25928;
	// addi r3,r10,-21940
	ctx.r3.s64 = ctx.r10.s64 + -21940;
	// bl 0x827d6c90
	ctx.lr = 0x825A6FC4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,26008
	ctx.r4.s64 = ctx.r11.s64 + 26008;
	// addi r3,r10,-21976
	ctx.r3.s64 = ctx.r10.s64 + -21976;
	// bl 0x827d6c90
	ctx.lr = 0x825A6FD8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,22568
	ctx.r4.s64 = ctx.r11.s64 + 22568;
	// addi r3,r10,-22000
	ctx.r3.s64 = ctx.r10.s64 + -22000;
	// bl 0x827d6c90
	ctx.lr = 0x825A6FEC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,22648
	ctx.r4.s64 = ctx.r11.s64 + 22648;
	// addi r3,r10,-22024
	ctx.r3.s64 = ctx.r10.s64 + -22024;
	// bl 0x827d6c90
	ctx.lr = 0x825A7000;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16568
	ctx.r4.s64 = ctx.r11.s64 + 16568;
	// addi r3,r10,-22048
	ctx.r3.s64 = ctx.r10.s64 + -22048;
	// bl 0x827d6c90
	ctx.lr = 0x825A7014;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16584
	ctx.r4.s64 = ctx.r11.s64 + 16584;
	// addi r3,r10,-22060
	ctx.r3.s64 = ctx.r10.s64 + -22060;
	// bl 0x827d6c90
	ctx.lr = 0x825A7028;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,22736
	ctx.r4.s64 = ctx.r11.s64 + 22736;
	// addi r3,r10,-22080
	ctx.r3.s64 = ctx.r10.s64 + -22080;
	// bl 0x827d6c90
	ctx.lr = 0x825A703C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,22832
	ctx.r4.s64 = ctx.r11.s64 + 22832;
	// addi r3,r10,-22108
	ctx.r3.s64 = ctx.r10.s64 + -22108;
	// bl 0x827d6c90
	ctx.lr = 0x825A7050;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,22960
	ctx.r4.s64 = ctx.r11.s64 + 22960;
	// addi r3,r10,-22136
	ctx.r3.s64 = ctx.r10.s64 + -22136;
	// bl 0x827d6c90
	ctx.lr = 0x825A7064;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,23056
	ctx.r4.s64 = ctx.r11.s64 + 23056;
	// addi r3,r10,-22168
	ctx.r3.s64 = ctx.r10.s64 + -22168;
	// bl 0x827d6c90
	ctx.lr = 0x825A7078;
	sub_827D6C90(ctx, base);
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// addi r3,r10,-22196
	ctx.r3.s64 = ctx.r10.s64 + -22196;
	// addi r4,r11,23152
	ctx.r4.s64 = ctx.r11.s64 + 23152;
	// bl 0x827d6c90
	ctx.lr = 0x825A708C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16600
	ctx.r4.s64 = ctx.r11.s64 + 16600;
	// addi r3,r10,-22224
	ctx.r3.s64 = ctx.r10.s64 + -22224;
	// bl 0x827d6c90
	ctx.lr = 0x825A70A0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,16616
	ctx.r4.s64 = ctx.r11.s64 + 16616;
	// addi r3,r10,-22248
	ctx.r3.s64 = ctx.r10.s64 + -22248;
	// bl 0x827d6c90
	ctx.lr = 0x825A70B4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,23216
	ctx.r4.s64 = ctx.r11.s64 + 23216;
	// addi r3,r10,-22272
	ctx.r3.s64 = ctx.r10.s64 + -22272;
	// bl 0x827d6c90
	ctx.lr = 0x825A70C8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,23336
	ctx.r4.s64 = ctx.r11.s64 + 23336;
	// addi r3,r10,-22300
	ctx.r3.s64 = ctx.r10.s64 + -22300;
	// bl 0x827d6c90
	ctx.lr = 0x825A70DC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,23424
	ctx.r4.s64 = ctx.r11.s64 + 23424;
	// addi r3,r10,-22312
	ctx.r3.s64 = ctx.r10.s64 + -22312;
	// bl 0x827d6c90
	ctx.lr = 0x825A70F0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,23472
	ctx.r4.s64 = ctx.r11.s64 + 23472;
	// addi r3,r10,-22344
	ctx.r3.s64 = ctx.r10.s64 + -22344;
	// bl 0x827d6c90
	ctx.lr = 0x825A7104;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,23496
	ctx.r4.s64 = ctx.r11.s64 + 23496;
	// addi r3,r10,-22372
	ctx.r3.s64 = ctx.r10.s64 + -22372;
	// bl 0x827d6c90
	ctx.lr = 0x825A7118;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,23560
	ctx.r4.s64 = ctx.r11.s64 + 23560;
	// addi r3,r10,-22400
	ctx.r3.s64 = ctx.r10.s64 + -22400;
	// bl 0x827d6c90
	ctx.lr = 0x825A712C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,23600
	ctx.r4.s64 = ctx.r11.s64 + 23600;
	// addi r3,r10,-22424
	ctx.r3.s64 = ctx.r10.s64 + -22424;
	// bl 0x827d6c90
	ctx.lr = 0x825A7140;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,23696
	ctx.r4.s64 = ctx.r11.s64 + 23696;
	// addi r3,r10,-22440
	ctx.r3.s64 = ctx.r10.s64 + -22440;
	// bl 0x827d6c90
	ctx.lr = 0x825A7154;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,23720
	ctx.r4.s64 = ctx.r11.s64 + 23720;
	// addi r3,r10,-22456
	ctx.r3.s64 = ctx.r10.s64 + -22456;
	// bl 0x827d6c90
	ctx.lr = 0x825A7168;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,26088
	ctx.r4.s64 = ctx.r11.s64 + 26088;
	// addi r3,r10,-22484
	ctx.r3.s64 = ctx.r10.s64 + -22484;
	// bl 0x827d6c90
	ctx.lr = 0x825A717C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,23744
	ctx.r4.s64 = ctx.r11.s64 + 23744;
	// addi r3,r10,-22508
	ctx.r3.s64 = ctx.r10.s64 + -22508;
	// bl 0x827d6c90
	ctx.lr = 0x825A7190;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r4,r11,23776
	ctx.r4.s64 = ctx.r11.s64 + 23776;
	// addi r3,r10,-22552
	ctx.r3.s64 = ctx.r10.s64 + -22552;
	// bl 0x827d6c90
	ctx.lr = 0x825A71A4;
	sub_827D6C90(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A71B8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A71B8) { __imp__sub_825A71B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A71B8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// mr r10,r3
	ctx.r10.u64 = ctx.r3.u64;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// lwz r11,-9184(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -9184);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// rlwinm r9,r9,0,0,24
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// beq cr6,0x825a71f4
	if (ctx.cr6.eq) goto loc_825A71F4;
	// li r31,0
	ctx.r31.s64 = 0;
	// b 0x825a7204
	goto loc_825A7204;
loc_825A71F4:
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r31,r11,r9
	ctx.r31.u64 = ctx.r11.u64 + ctx.r9.u64;
loc_825A7204:
	// addi r4,r31,12
	ctx.r4.s64 = ctx.r31.s64 + 12;
	// bl 0x827d80e8
	ctx.lr = 0x825A720C;
	sub_827D80E8(ctx, base);
	// bl 0x827d7f00
	ctx.lr = 0x825A7210;
	sub_827D7F00(ctx, base);
	// cntlzw r11,r3
	ctx.r11.u64 = ctx.r3.u32 == 0 ? 32 : __builtin_clz(ctx.r3.u32);
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r3,r11,1
	ctx.r3.u64 = ctx.r11.u64 ^ 1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A7238);  /* forward decl */
PPC_WEAK_FUNC(sub_825A7238) { __imp__sub_825A7238(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A7238) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r11,-9184(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -9184);
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r10,r10,r3
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r10.u32 + ctx.r3.u32);
	// rlwinm r10,r10,0,0,24
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x825a726c
	if (ctx.cr6.eq) goto loc_825A726C;
	// li r31,0
	ctx.r31.s64 = 0;
	// b 0x825a727c
	goto loc_825A727C;
loc_825A726C:
	// lwz r9,12(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r9,r3
	ctx.r11.s64 = int64_t(ctx.r9.s32) * int64_t(ctx.r3.s32);
	// add r31,r11,r10
	ctx.r31.u64 = ctx.r11.u64 + ctx.r10.u64;
loc_825A727C:
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x827d8020
	ctx.lr = 0x825A7284;
	sub_827D8020(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
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

PPC_FUNC_IMPL(__imp__sub_825A72A0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A72A0) { __imp__sub_825A72A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A72A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// lwz r11,-9184(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -9184);
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r10,r10,r3
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r10.u32 + ctx.r3.u32);
	// rlwinm r10,r10,0,0,24
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x825a72cc
	if (ctx.cr6.eq) goto loc_825A72CC;
	// li r11,0
	ctx.r11.s64 = 0;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r3,4(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// blr 
	return;
loc_825A72CC:
	// lwz r9,12(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r9,r3
	ctx.r11.s64 = int64_t(ctx.r9.s32) * int64_t(ctx.r3.s32);
	// lwzx r11,r11,r10
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + ctx.r10.u32);
	// lwz r3,4(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A72E8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A72E8) { __imp__sub_825A72E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A72E8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r6,r5
	ctx.r6.u64 = ctx.r5.u64;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// mr r5,r4
	ctx.r5.u64 = ctx.r4.u64;
	// li r8,0
	ctx.r8.s64 = 0;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r4,64
	ctx.r4.s64 = 64;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827e21a0
	ctx.lr = 0x825A7318;
	sub_827E21A0(ctx, base);
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x825a71b8
	ctx.lr = 0x825A7324;
	sub_825A71B8(ctx, base);
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A7338);  /* forward decl */
PPC_WEAK_FUNC(sub_825A7338) { __imp__sub_825A7338(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A7338) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// lwz r11,-9184(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -9184);
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r10,r10,r3
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r10.u32 + ctx.r3.u32);
	// rlwinm r10,r10,0,0,24
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x825a7368
	if (ctx.cr6.eq) goto loc_825A7368;
	// li r11,0
	ctx.r11.s64 = 0;
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// stw r10,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
	// blr 
	return;
loc_825A7368:
	// lwz r9,12(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r9,r3
	ctx.r11.s64 = int64_t(ctx.r9.s32) * int64_t(ctx.r3.s32);
	// add r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// stw r10,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A7388);  /* forward decl */
PPC_WEAK_FUNC(sub_825A7388) { __imp__sub_825A7388(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A7388) {
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
	// lis r30,-32092
	ctx.r30.s64 = -2103181312;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r4,-22348(r30)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r30.u32 + -22348);
	// bl 0x821a87a8
	ctx.lr = 0x825A73AC;
	sub_821A87A8(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825a73c8
	if (ctx.cr6.eq) goto loc_825A73C8;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,-22348(r30)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r30.u32 + -22348);
	// bl 0x821a84a8
	ctx.lr = 0x825A73C4;
	sub_821A84A8(ctx, base);
	// b 0x825a7410
	goto loc_825A7410;
loc_825A73C8:
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// lwz r11,-9184(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -9184);
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r10,r10,r31
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r10.u32 + ctx.r31.u32);
	// rlwinm r10,r10,0,0,24
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x825a73ec
	if (ctx.cr6.eq) goto loc_825A73EC;
	// li r11,0
	ctx.r11.s64 = 0;
	// b 0x825a73fc
	goto loc_825A73FC;
loc_825A73EC:
	// lwz r9,12(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r9,r31
	ctx.r11.s64 = int64_t(ctx.r9.s32) * int64_t(ctx.r31.s32);
	// add r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
loc_825A73FC:
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x825a7410
	if (ctx.cr6.eq) goto loc_825A7410;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x825a7238
	ctx.lr = 0x825A7410;
	sub_825A7238(ctx, base);
loc_825A7410:
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

PPC_FUNC_IMPL(__imp__sub_825A7428);  /* forward decl */
PPC_WEAK_FUNC(sub_825A7428) { __imp__sub_825A7428(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A7428) {
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
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// lwz r11,-9184(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -9184);
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r10,r10,r30
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r10.u32 + ctx.r30.u32);
	// rlwinm r10,r10,0,0,24
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x825a7464
	if (ctx.cr6.eq) goto loc_825A7464;
	// li r31,0
	ctx.r31.s64 = 0;
	// b 0x825a7474
	goto loc_825A7474;
loc_825A7464:
	// lwz r9,12(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r9,r30
	ctx.r11.s64 = int64_t(ctx.r9.s32) * int64_t(ctx.r30.s32);
	// add r31,r11,r10
	ctx.r31.u64 = ctx.r11.u64 + ctx.r10.u64;
loc_825A7474:
	// lwz r11,4(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4);
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// stw r11,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r11.u32);
	// bl 0x827d7ca8
	ctx.lr = 0x825A7488;
	sub_827D7CA8(ctx, base);
	// lwz r11,4(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4);
	// add. r11,r3,r11
	ctx.r11.u64 = ctx.r3.u64 + ctx.r11.u64;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// bgt 0x825a749c
	if (ctx.cr0.gt) goto loc_825A749C;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x825a7388
	ctx.lr = 0x825A749C;
	sub_825A7388(ctx, base);
loc_825A749C:
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

PPC_FUNC_IMPL(__imp__sub_825A74B8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A74B8) { __imp__sub_825A74B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A74B8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x825A74C0;
	__savegprlr_28(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r30,-32005
	ctx.r30.s64 = -2097479680;
	// mr r28,r3
	ctx.r28.u64 = ctx.r3.u64;
	// li r31,0
	ctx.r31.s64 = 0;
	// lwz r11,-9184(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -9184);
	// lwz r10,8(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// ble cr6,0x825a756c
	if (!ctx.cr6.gt) goto loc_825A756C;
	// lis r29,-32092
	ctx.r29.s64 = -2103181312;
loc_825A74E4:
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r10,r10,r31
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r10.u32 + ctx.r31.u32);
	// rlwinm r10,r10,0,0,24
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// bne cr6,0x825a755c
	if (!ctx.cr6.eq) goto loc_825A755C;
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r10,r10,r31
	ctx.r10.s64 = int64_t(ctx.r10.s32) * int64_t(ctx.r31.s32);
	// add r10,r10,r9
	ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x825a755c
	if (ctx.cr6.eq) goto loc_825A755C;
	// lwz r10,0(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// beq cr6,0x825a755c
	if (ctx.cr6.eq) goto loc_825A755C;
	// clrlwi r11,r28,24
	ctx.r11.u64 = ctx.r28.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825a7550
	if (!ctx.cr6.eq) goto loc_825A7550;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8254e4f8
	ctx.lr = 0x825A7530;
	sub_8254E4F8(ctx, base);
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// bne cr6,0x825a7558
	if (!ctx.cr6.eq) goto loc_825A7558;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,-22348(r29)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r29.u32 + -22348);
	// bl 0x821a84d8
	ctx.lr = 0x825A7544;
	sub_821A84D8(ctx, base);
	// andi. r11,r3,198
	ctx.r11.u64 = ctx.r3.u64 & 198;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825a7558
	if (!ctx.cr6.eq) goto loc_825A7558;
loc_825A7550:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x825a7388
	ctx.lr = 0x825A7558;
	sub_825A7388(ctx, base);
loc_825A7558:
	// lwz r11,-9184(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -9184);
loc_825A755C:
	// lwz r10,8(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// addi r31,r31,1
	ctx.r31.s64 = ctx.r31.s64 + 1;
	// cmpw cr6,r31,r10
	ctx.cr6.compare<int32_t>(ctx.r31.s32, ctx.r10.s32, ctx.xer);
	// blt cr6,0x825a74e4
	if (ctx.cr6.lt) goto loc_825A74E4;
loc_825A756C:
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A7578);  /* forward decl */
PPC_WEAK_FUNC(sub_825A7578) { __imp__sub_825A7578(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A7578) {
	PPC_FUNC_PROLOGUE();
	// li r3,0
	ctx.r3.s64 = 0;
	// b 0x825a74b8
	sub_825A74B8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A7580);  /* forward decl */
PPC_WEAK_FUNC(sub_825A7580) { __imp__sub_825A7580(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A7580) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x825A7588;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r3,0
	ctx.r3.s64 = 0;
	// bl 0x825a74b8
	ctx.lr = 0x825A7594;
	sub_825A74B8(ctx, base);
	// lis r29,-32005
	ctx.r29.s64 = -2097479680;
	// lwz r31,-9184(r29)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r29.u32 + -9184);
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x825a75fc
	if (ctx.cr6.eq) goto loc_825A75FC;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// li r30,0
	ctx.r30.s64 = 0;
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// ble cr6,0x825a75e4
	if (!ctx.cr6.gt) goto loc_825A75E4;
	// lbz r11,24(r31)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r31.u32 + 24);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825a75d0
	if (ctx.cr6.eq) goto loc_825A75D0;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x8218be78
	ctx.lr = 0x825A75C8;
	sub_8218BE78(ctx, base);
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4);
	// bl 0x8218be78
	ctx.lr = 0x825A75D0;
	sub_8218BE78(ctx, base);
loc_825A75D0:
	// stw r30,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r30.u32);
	// stw r30,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r30.u32);
	// stw r30,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r30.u32);
	// stw r30,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r30.u32);
	// stw r30,20(r31)
	PPC_STORE_U32(ctx.r31.u32 + 20, ctx.r30.u32);
loc_825A75E4:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8218be78
	ctx.lr = 0x825A75EC;
	sub_8218BE78(ctx, base);
	// mr r11,r30
	ctx.r11.u64 = ctx.r30.u64;
	// stw r11,-9184(r29)
	PPC_STORE_U32(ctx.r29.u32 + -9184, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
loc_825A75FC:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,-9184(r29)
	PPC_STORE_U32(ctx.r29.u32 + -9184, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A7610);  /* forward decl */
PPC_WEAK_FUNC(sub_825A7610) { __imp__sub_825A7610(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A7610) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x825A7618;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// bl 0x82318f60
	ctx.lr = 0x825A7624;
	sub_82318F60(ctx, base);
	// bl 0x821d4828
	ctx.lr = 0x825A7628;
	sub_821D4828(ctx, base);
	// cmpwi cr6,r3,-1
	ctx.cr6.compare<int32_t>(ctx.r3.s32, -1, ctx.xer);
	// bne cr6,0x825a7670
	if (!ctx.cr6.eq) goto loc_825A7670;
	// lis r30,-32005
	ctx.r30.s64 = -2097479680;
	// lwz r3,-9184(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + -9184);
	// bl 0x821250b0
	ctx.lr = 0x825A763C;
	sub_821250B0(ctx, base);
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// bl 0x824e3308
	ctx.lr = 0x825A7648;
	sub_824E3308(ctx, base);
	// lwz r11,-9184(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -9184);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r9,12(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// subf r11,r10,r31
	ctx.r11.u64 = ctx.r31.u64 - ctx.r10.u64;
	// twllei r9,0
	if (ctx.r9.s32 == 0 || ctx.r9.u32 < 0u) ppc_trap(ctx, base, 0);
	// divw r3,r11,r9
	ctx.r3.s32 = (ctx.r9.s32 && !(ctx.r11.s32 == INT32_MIN && ctx.r9.s32 == -1)) ? ctx.r11.s32 / ctx.r9.s32 : 0;
	// rotlwi r11,r11,1
	ctx.r11.u64 = __builtin_rotateleft32(ctx.r11.u32, 1);
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// andc r11,r9,r11
	ctx.r11.u64 = ctx.r9.u64 & ~ctx.r11.u64;
	// twlgei r11,-1
	if (ctx.r11.s32 == -1 || ctx.r11.u32 > 4294967295u) ppc_trap(ctx, base, 0);
loc_825A7670:
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A7678);  /* forward decl */
PPC_WEAK_FUNC(sub_825A7678) { __imp__sub_825A7678(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A7678) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eaf0
	ctx.lr = 0x825A7680;
	__savegprlr_18(ctx, base);
	// stwu r1,-288(r1)
	ea = -288 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r3,28
	ctx.r3.s64 = 28;
	// bl 0x8218be28
	ctx.lr = 0x825A768C;
	sub_8218BE28(ctx, base);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r28,0
	ctx.r28.s64 = 0;
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// addi r27,r11,13948
	ctx.r27.s64 = ctx.r11.s64 + 13948;
	// beq cr6,0x825a76bc
	if (ctx.cr6.eq) goto loc_825A76BC;
	// li r6,36
	ctx.r6.s64 = 36;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// li r4,500
	ctx.r4.s64 = 500;
	// bl 0x825ee000
	ctx.lr = 0x825A76B0;
	sub_825EE000(ctx, base);
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// stw r3,-9184(r11)
	PPC_STORE_U32(ctx.r11.u32 + -9184, ctx.r3.u32);
	// b 0x825a76c8
	goto loc_825A76C8;
loc_825A76BC:
	// lis r10,-32005
	ctx.r10.s64 = -2097479680;
	// mr r11,r28
	ctx.r11.u64 = ctx.r28.u64;
	// stw r11,-9184(r10)
	PPC_STORE_U32(ctx.r10.u32 + -9184, ctx.r11.u32);
loc_825A76C8:
	// lis r3,-32145
	ctx.r3.s64 = -2106654720;
	// lis r4,-32171
	ctx.r4.s64 = -2108358656;
	// lis r5,-32166
	ctx.r5.s64 = -2108030976;
	// lis r6,-32166
	ctx.r6.s64 = -2108030976;
	// lis r7,-32166
	ctx.r7.s64 = -2108030976;
	// lis r8,-32166
	ctx.r8.s64 = -2108030976;
	// lis r9,-32166
	ctx.r9.s64 = -2108030976;
	// lis r10,-32145
	ctx.r10.s64 = -2106654720;
	// lis r11,-32145
	ctx.r11.s64 = -2106654720;
	// lis r29,-32230
	ctx.r29.s64 = -2112225280;
	// lis r30,-32145
	ctx.r30.s64 = -2106654720;
	// lis r31,-32252
	ctx.r31.s64 = -2113667072;
	// addi r29,r29,-19720
	ctx.r29.s64 = ctx.r29.s64 + -19720;
	// addi r31,r31,-18728
	ctx.r31.s64 = ctx.r31.s64 + -18728;
	// addi r30,r30,9912
	ctx.r30.s64 = ctx.r30.s64 + 9912;
	// addi r26,r3,9912
	ctx.r26.s64 = ctx.r3.s64 + 9912;
	// addi r25,r4,-6920
	ctx.r25.s64 = ctx.r4.s64 + -6920;
	// addi r24,r5,29736
	ctx.r24.s64 = ctx.r5.s64 + 29736;
	// addi r23,r6,29496
	ctx.r23.s64 = ctx.r6.s64 + 29496;
	// addi r22,r7,30224
	ctx.r22.s64 = ctx.r7.s64 + 30224;
	// addi r21,r8,29240
	ctx.r21.s64 = ctx.r8.s64 + 29240;
	// addi r20,r9,29416
	ctx.r20.s64 = ctx.r9.s64 + 29416;
	// addi r19,r10,-25088
	ctx.r19.s64 = ctx.r10.s64 + -25088;
	// addi r18,r11,9912
	ctx.r18.s64 = ctx.r11.s64 + 9912;
	// bl 0x821a8858
	ctx.lr = 0x825A772C;
	sub_821A8858(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stw r21,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r21.u32);
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// stw r22,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r22.u32);
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// stw r23,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r23.u32);
	// li r6,500
	ctx.r6.s64 = 500;
	// stw r24,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r24.u32);
	// mr r7,r18
	ctx.r7.u64 = ctx.r18.u64;
	// stw r25,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r25.u32);
	// li r8,0
	ctx.r8.s64 = 0;
	// stw r26,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r26.u32);
	// mr r9,r19
	ctx.r9.u64 = ctx.r19.u64;
	// stw r30,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r30.u32);
	// mr r10,r20
	ctx.r10.u64 = ctx.r20.u64;
	// stw r29,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r29.u32);
	// stb r11,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r11.u8);
	// stb r28,159(r1)
	PPC_STORE_U8(ctx.r1.u32 + 159, ctx.r28.u8);
	// stw r28,164(r1)
	PPC_STORE_U32(ctx.r1.u32 + 164, ctx.r28.u32);
	// bl 0x824c1338
	ctx.lr = 0x825A777C;
	sub_824C1338(ctx, base);
	// lis r11,-32092
	ctx.r11.s64 = -2103181312;
	// stw r3,-22348(r11)
	PPC_STORE_U32(ctx.r11.u32 + -22348, ctx.r3.u32);
	// addi r1,r1,288
	ctx.r1.s64 = ctx.r1.s64 + 288;
	// b 0x8298eb40
	__restgprlr_18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A7790);  /* forward decl */
PPC_WEAK_FUNC(sub_825A7790) { __imp__sub_825A7790(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A7790) {
	PPC_FUNC_PROLOGUE();
	// b 0x825a7580
	sub_825A7580(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A7798);  /* forward decl */
PPC_WEAK_FUNC(sub_825A7798) { __imp__sub_825A7798(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A7798) {
	PPC_FUNC_PROLOGUE();
	// lwz r9,152(r3)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 152);
	// li r11,0
	ctx.r11.s64 = 0;
	// li r10,1
	ctx.r10.s64 = 1;
	// li r8,-1
	ctx.r8.s64 = -1;
	// clrlwi r9,r9,2
	ctx.r9.u64 = ctx.r9.u32 & 0x3FFFFFFF;
	// stb r11,92(r3)
	PPC_STORE_U8(ctx.r3.u32 + 92, ctx.r11.u8);
	// stw r11,116(r3)
	PPC_STORE_U32(ctx.r3.u32 + 116, ctx.r11.u32);
	// stw r11,120(r3)
	PPC_STORE_U32(ctx.r3.u32 + 120, ctx.r11.u32);
	// stw r11,124(r3)
	PPC_STORE_U32(ctx.r3.u32 + 124, ctx.r11.u32);
	// stb r11,128(r3)
	PPC_STORE_U8(ctx.r3.u32 + 128, ctx.r11.u8);
	// stb r11,129(r3)
	PPC_STORE_U8(ctx.r3.u32 + 129, ctx.r11.u8);
	// stb r11,130(r3)
	PPC_STORE_U8(ctx.r3.u32 + 130, ctx.r11.u8);
	// stb r10,131(r3)
	PPC_STORE_U8(ctx.r3.u32 + 131, ctx.r10.u8);
	// stb r10,132(r3)
	PPC_STORE_U8(ctx.r3.u32 + 132, ctx.r10.u8);
	// stb r11,133(r3)
	PPC_STORE_U8(ctx.r3.u32 + 133, ctx.r11.u8);
	// stb r11,134(r3)
	PPC_STORE_U8(ctx.r3.u32 + 134, ctx.r11.u8);
	// stb r11,135(r3)
	PPC_STORE_U8(ctx.r3.u32 + 135, ctx.r11.u8);
	// stb r11,136(r3)
	PPC_STORE_U8(ctx.r3.u32 + 136, ctx.r11.u8);
	// stb r10,137(r3)
	PPC_STORE_U8(ctx.r3.u32 + 137, ctx.r10.u8);
	// stb r8,138(r3)
	PPC_STORE_U8(ctx.r3.u32 + 138, ctx.r8.u8);
	// stw r11,140(r3)
	PPC_STORE_U32(ctx.r3.u32 + 140, ctx.r11.u32);
	// stb r11,144(r3)
	PPC_STORE_U8(ctx.r3.u32 + 144, ctx.r11.u8);
	// stw r9,152(r3)
	PPC_STORE_U32(ctx.r3.u32 + 152, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A77F8);  /* forward decl */
PPC_WEAK_FUNC(sub_825A77F8) { __imp__sub_825A77F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A77F8) {
	PPC_FUNC_PROLOGUE();
	// lbz r11,133(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 133);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lbz r11,134(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 134);
	// li r9,0
	ctx.r9.s64 = 0;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825a7830
	if (ctx.cr6.eq) goto loc_825A7830;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r11,1768(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 1768);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// ble cr6,0x825a782c
	if (!ctx.cr6.gt) goto loc_825A782C;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// stw r11,1768(r10)
	PPC_STORE_U32(ctx.r10.u32 + 1768, ctx.r11.u32);
loc_825A782C:
	// stb r9,134(r3)
	PPC_STORE_U8(ctx.r3.u32 + 134, ctx.r9.u8);
loc_825A7830:
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r11,1772(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 1772);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// ble cr6,0x825a7848
	if (!ctx.cr6.gt) goto loc_825A7848;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// stw r11,1772(r10)
	PPC_STORE_U32(ctx.r10.u32 + 1772, ctx.r11.u32);
loc_825A7848:
	// stb r9,133(r3)
	PPC_STORE_U8(ctx.r3.u32 + 133, ctx.r9.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A7850);  /* forward decl */
PPC_WEAK_FUNC(sub_825A7850) { __imp__sub_825A7850(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A7850) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,16(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	// lwz r10,20(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 20);
	// lwz r9,24(r3)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 24);
	// addi r11,r11,7
	ctx.r11.s64 = ctx.r11.s64 + 7;
	// stw r10,120(r3)
	PPC_STORE_U32(ctx.r3.u32 + 120, ctx.r10.u32);
	// stw r11,116(r3)
	PPC_STORE_U32(ctx.r3.u32 + 116, ctx.r11.u32);
	// stw r9,124(r3)
	PPC_STORE_U32(ctx.r3.u32 + 124, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A7870);  /* forward decl */
PPC_WEAK_FUNC(sub_825A7870) { __imp__sub_825A7870(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A7870) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,16(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 16);
	// lwz r10,4(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4);
	// add r3,r11,r10
	ctx.r3.u64 = ctx.r11.u64 + ctx.r10.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A7880);  /* forward decl */
PPC_WEAK_FUNC(sub_825A7880) { __imp__sub_825A7880(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A7880) {
	PPC_FUNC_PROLOGUE();
	// li r11,0
	ctx.r11.s64 = 0;
	// lis r10,-32005
	ctx.r10.s64 = -2097479680;
	// stw r11,-9160(r10)
	PPC_STORE_U32(ctx.r10.u32 + -9160, ctx.r11.u32);
	// lis r10,-32005
	ctx.r10.s64 = -2097479680;
	// stw r11,-9164(r10)
	PPC_STORE_U32(ctx.r10.u32 + -9164, ctx.r11.u32);
	// lis r10,-32005
	ctx.r10.s64 = -2097479680;
	// stw r11,-9176(r10)
	PPC_STORE_U32(ctx.r10.u32 + -9176, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A78A0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A78A0) { __imp__sub_825A78A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A78A0) {
	PPC_FUNC_PROLOGUE();
	// li r11,0
	ctx.r11.s64 = 0;
	// lis r10,-32005
	ctx.r10.s64 = -2097479680;
	// stw r11,-9168(r10)
	PPC_STORE_U32(ctx.r10.u32 + -9168, ctx.r11.u32);
	// lis r10,-32005
	ctx.r10.s64 = -2097479680;
	// stw r11,-9172(r10)
	PPC_STORE_U32(ctx.r10.u32 + -9172, ctx.r11.u32);
	// lis r10,-32005
	ctx.r10.s64 = -2097479680;
	// stw r11,-9180(r10)
	PPC_STORE_U32(ctx.r10.u32 + -9180, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A78C0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A78C0) { __imp__sub_825A78C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A78C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x827d7318
	ctx.lr = 0x825A78D8;
	sub_827D7318(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// lwz r9,152(r31)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r31.u32 + 152);
	// li r8,-1
	ctx.r8.s64 = -1;
	// addi r10,r11,-18708
	ctx.r10.s64 = ctx.r11.s64 + -18708;
	// li r11,0
	ctx.r11.s64 = 0;
	// clrlwi r9,r9,2
	ctx.r9.u64 = ctx.r9.u32 & 0x3FFFFFFF;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stb r8,138(r31)
	PPC_STORE_U8(ctx.r31.u32 + 138, ctx.r8.u8);
	// stw r10,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r10.u32);
	// li r10,1
	ctx.r10.s64 = 1;
	// stb r11,92(r31)
	PPC_STORE_U8(ctx.r31.u32 + 92, ctx.r11.u8);
	// stw r11,116(r31)
	PPC_STORE_U32(ctx.r31.u32 + 116, ctx.r11.u32);
	// stw r11,120(r31)
	PPC_STORE_U32(ctx.r31.u32 + 120, ctx.r11.u32);
	// stw r11,124(r31)
	PPC_STORE_U32(ctx.r31.u32 + 124, ctx.r11.u32);
	// stb r11,128(r31)
	PPC_STORE_U8(ctx.r31.u32 + 128, ctx.r11.u8);
	// stb r11,129(r31)
	PPC_STORE_U8(ctx.r31.u32 + 129, ctx.r11.u8);
	// stb r11,130(r31)
	PPC_STORE_U8(ctx.r31.u32 + 130, ctx.r11.u8);
	// stb r10,131(r31)
	PPC_STORE_U8(ctx.r31.u32 + 131, ctx.r10.u8);
	// stb r10,132(r31)
	PPC_STORE_U8(ctx.r31.u32 + 132, ctx.r10.u8);
	// stb r11,133(r31)
	PPC_STORE_U8(ctx.r31.u32 + 133, ctx.r11.u8);
	// stb r11,134(r31)
	PPC_STORE_U8(ctx.r31.u32 + 134, ctx.r11.u8);
	// stb r11,135(r31)
	PPC_STORE_U8(ctx.r31.u32 + 135, ctx.r11.u8);
	// stb r11,136(r31)
	PPC_STORE_U8(ctx.r31.u32 + 136, ctx.r11.u8);
	// stb r10,137(r31)
	PPC_STORE_U8(ctx.r31.u32 + 137, ctx.r10.u8);
	// stw r11,140(r31)
	PPC_STORE_U32(ctx.r31.u32 + 140, ctx.r11.u32);
	// stb r11,144(r31)
	PPC_STORE_U8(ctx.r31.u32 + 144, ctx.r11.u8);
	// stw r9,152(r31)
	PPC_STORE_U32(ctx.r31.u32 + 152, ctx.r9.u32);
	// stw r11,148(r31)
	PPC_STORE_U32(ctx.r31.u32 + 148, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A7960);  /* forward decl */
PPC_WEAK_FUNC(sub_825A7960) { __imp__sub_825A7960(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A7960) {
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
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r11,r11,-18708
	ctx.r11.s64 = ctx.r11.s64 + -18708;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x827d57e0
	ctx.lr = 0x825A798C;
	sub_827D57E0(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = ctx.r30.u32 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825a79a4
	if (ctx.cr6.eq) goto loc_825A79A4;
	// bl 0x8218be78
	ctx.lr = 0x825A79A0;
	sub_8218BE78(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
loc_825A79A4:
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

PPC_FUNC_IMPL(__imp__sub_825A79C0);  /* forward decl */
PPC_WEAK_FUNC(sub_825A79C0) { __imp__sub_825A79C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A79C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x825A79C8;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32039
	ctx.r11.s64 = -2099707904;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// lwz r3,15072(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 15072);
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x825A79EC;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// lbz r11,138(r31)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r31.u32 + 138);
	// li r29,1
	ctx.r29.s64 = 1;
	// extsb r11,r11
	ctx.r11.s64 = ctx.r11.s8;
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// blt cr6,0x825a7a90
	if (ctx.cr6.lt) goto loc_825A7A90;
	// beq cr6,0x825a7a40
	if (ctx.cr6.eq) goto loc_825A7A40;
	// cmplwi cr6,r11,3
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 3, ctx.xer);
	// bge cr6,0x825a7ad8
	if (!ctx.cr6.lt) goto loc_825A7AD8;
	// lwz r30,140(r31)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r31.u32 + 140);
	// bl 0x8219b710
	ctx.lr = 0x825A7A14;
	sub_8219B710(ctx, base);
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// mulli r11,r30,120
	ctx.r11.s64 = static_cast<int64_t>(ctx.r30.u64 * static_cast<uint64_t>(120));
	// add r3,r11,r10
	ctx.r3.u64 = ctx.r11.u64 + ctx.r10.u64;
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x825a7ad8
	if (ctx.cr6.eq) goto loc_825A7AD8;
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,60(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 60);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x825A7A38;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// sth r29,110(r3)
	PPC_STORE_U16(ctx.r3.u32 + 110, ctx.r29.u16);
	// b 0x825a7ad8
	goto loc_825A7AD8;
loc_825A7A40:
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// lwz r10,140(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 140);
	// srawi r9,r10,8
	ctx.xer.ca = (ctx.r10.s32 < 0) & ((ctx.r10.u32 & 0xFF) != 0);
	ctx.r9.s64 = ctx.r10.s32 >> 8;
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// lwz r11,88(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 88);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r8,r8,r9
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r8.u32 + ctx.r9.u32);
	// cmpw cr6,r8,r10
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r10.s32, ctx.xer);
	// bne cr6,0x825a7ad8
	if (!ctx.cr6.eq) goto loc_825A7AD8;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r9
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r9.s32);
	// add r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825a7ad8
	if (ctx.cr6.eq) goto loc_825A7AD8;
	// lwz r10,528(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 528);
	// li r9,3
	ctx.r9.s64 = 3;
	// rlwimi r10,r9,0,29,31
	ctx.r10.u64 = (__builtin_rotateleft32(ctx.r9.u32, 0) & 0x7) | (ctx.r10.u64 & 0xFFFFFFFFFFFFFFF8);
	// stw r10,528(r11)
	PPC_STORE_U32(ctx.r11.u32 + 528, ctx.r10.u32);
	// b 0x825a7ad8
	goto loc_825A7AD8;
loc_825A7A90:
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// lwz r10,140(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 140);
	// srawi r9,r10,8
	ctx.xer.ca = (ctx.r10.s32 < 0) & ((ctx.r10.u32 & 0xFF) != 0);
	ctx.r9.s64 = ctx.r10.s32 >> 8;
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r8,r8,r9
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r8.u32 + ctx.r9.u32);
	// cmpw cr6,r8,r10
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r10.s32, ctx.xer);
	// bne cr6,0x825a7ad8
	if (!ctx.cr6.eq) goto loc_825A7AD8;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r9
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r9.s32);
	// add r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825a7ad8
	if (ctx.cr6.eq) goto loc_825A7AD8;
	// lwz r10,556(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 556);
	// rlwinm r10,r10,0,18,16
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0xFFFFFFFFFFFFBFFF;
	// stw r10,556(r11)
	PPC_STORE_U32(ctx.r11.u32 + 556, ctx.r10.u32);
loc_825A7AD8:
	// lis r10,-32005
	ctx.r10.s64 = -2097479680;
	// lwz r8,4(r31)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4);
	// li r11,0
	ctx.r11.s64 = 0;
	// lwz r9,-9176(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -9176);
	// cmplw cr6,r8,r9
	ctx.cr6.compare<uint32_t>(ctx.r8.u32, ctx.r9.u32, ctx.xer);
	// bne cr6,0x825a7b04
	if (!ctx.cr6.eq) goto loc_825A7B04;
	// lis r9,-32005
	ctx.r9.s64 = -2097479680;
	// stw r11,-9176(r10)
	PPC_STORE_U32(ctx.r10.u32 + -9176, ctx.r11.u32);
	// stw r11,-9160(r9)
	PPC_STORE_U32(ctx.r9.u32 + -9160, ctx.r11.u32);
	// lis r9,-32005
	ctx.r9.s64 = -2097479680;
	// stw r11,-9164(r9)
	PPC_STORE_U32(ctx.r9.u32 + -9164, ctx.r11.u32);
loc_825A7B04:
	// lis r9,-32005
	ctx.r9.s64 = -2097479680;
	// lwz r8,4(r31)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4);
	// lwz r10,-9180(r9)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r9.u32 + -9180);
	// cmplw cr6,r8,r10
	ctx.cr6.compare<uint32_t>(ctx.r8.u32, ctx.r10.u32, ctx.xer);
	// bne cr6,0x825a7b2c
	if (!ctx.cr6.eq) goto loc_825A7B2C;
	// lis r10,-32005
	ctx.r10.s64 = -2097479680;
	// stw r11,-9180(r9)
	PPC_STORE_U32(ctx.r9.u32 + -9180, ctx.r11.u32);
	// stw r11,-9168(r10)
	PPC_STORE_U32(ctx.r10.u32 + -9168, ctx.r11.u32);
	// lis r10,-32005
	ctx.r10.s64 = -2097479680;
	// stw r11,-9172(r10)
	PPC_STORE_U32(ctx.r10.u32 + -9172, ctx.r11.u32);
loc_825A7B2C:
	// lwz r10,152(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 152);
	// li r9,-1
	ctx.r9.s64 = -1;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stb r11,92(r31)
	PPC_STORE_U8(ctx.r31.u32 + 92, ctx.r11.u8);
	// clrlwi r10,r10,2
	ctx.r10.u64 = ctx.r10.u32 & 0x3FFFFFFF;
	// stw r11,116(r31)
	PPC_STORE_U32(ctx.r31.u32 + 116, ctx.r11.u32);
	// stw r11,120(r31)
	PPC_STORE_U32(ctx.r31.u32 + 120, ctx.r11.u32);
	// stw r11,124(r31)
	PPC_STORE_U32(ctx.r31.u32 + 124, ctx.r11.u32);
	// stb r11,128(r31)
	PPC_STORE_U8(ctx.r31.u32 + 128, ctx.r11.u8);
	// stb r11,129(r31)
	PPC_STORE_U8(ctx.r31.u32 + 129, ctx.r11.u8);
	// stb r11,130(r31)
	PPC_STORE_U8(ctx.r31.u32 + 130, ctx.r11.u8);
	// stb r29,131(r31)
	PPC_STORE_U8(ctx.r31.u32 + 131, ctx.r29.u8);
	// stb r29,132(r31)
	PPC_STORE_U8(ctx.r31.u32 + 132, ctx.r29.u8);
	// stb r11,133(r31)
	PPC_STORE_U8(ctx.r31.u32 + 133, ctx.r11.u8);
	// stb r11,134(r31)
	PPC_STORE_U8(ctx.r31.u32 + 134, ctx.r11.u8);
	// stb r11,135(r31)
	PPC_STORE_U8(ctx.r31.u32 + 135, ctx.r11.u8);
	// stb r11,136(r31)
	PPC_STORE_U8(ctx.r31.u32 + 136, ctx.r11.u8);
	// stb r29,137(r31)
	PPC_STORE_U8(ctx.r31.u32 + 137, ctx.r29.u8);
	// stb r9,138(r31)
	PPC_STORE_U8(ctx.r31.u32 + 138, ctx.r9.u8);
	// stw r11,140(r31)
	PPC_STORE_U32(ctx.r31.u32 + 140, ctx.r11.u32);
	// stb r11,144(r31)
	PPC_STORE_U8(ctx.r31.u32 + 144, ctx.r11.u8);
	// stw r10,152(r31)
	PPC_STORE_U32(ctx.r31.u32 + 152, ctx.r10.u32);
	// bl 0x827d7148
	ctx.lr = 0x825A7B88;
	sub_827D7148(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825A7B90);  /* forward decl */
PPC_WEAK_FUNC(sub_825A7B90) { __imp__sub_825A7B90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825A7B90) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x825A7B98;
	__savegprlr_28(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// mr r28,r3
	ctx.r28.u64 = ctx.r3.u64;
	// addi r30,r11,20424
	ctx.r30.s64 = ctx.r11.s64 + 20424;
	// li r29,0
	ctx.r29.s64 = 0;
	// lhz r11,4(r30)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r30.u32 + 4);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825a7c28
	if (ctx.cr6.eq) goto loc_825A7C28;
	// li r31,0
	ctx.r31.s64 = 0;
loc_825A7BBC:
	// lwz r11,0(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// lwz r10,0(r30)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// lwzx r11,r31,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + ctx.r11.u32);
	// lwz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
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
	// beq cr6,0x825a7c14
	if (ctx.cr6.eq) goto loc_825A7C14;
	// lwzx r3,r31,r10
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + ctx.r10.u32);
	// bl 0x827d5660
	ctx.lr = 0x825A7BEC;
	sub_827D5660(ctx, base);
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// bl 0x8298f040
	ctx.lr = 0x825A7BF4;
	sub_8298F040(ctx, base);
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// bne cr6,0x825a7c14
	if (!ctx.cr6.eq) goto loc_825A7C14;
	// lwz r11,0(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// lwzx r3,r31,r11
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + ctx.r11.u32);
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,16(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x825A7C14;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
loc_825A7C14:
	// lhz r11,4(r30)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r30.u32 + 4);
	// addi r29,r29,1
	ctx.r29.s64 = ctx.r29.s64 + 1;
	// addi r31,r31,4
	ctx.r31.s64 = ctx.r31.s64 + 4;
	// cmpw cr6,r29,r11
	ctx.cr6.compare<int32_t>(ctx.r29.s32, ctx.r11.s32, ctx.xer);
	// blt cr6,0x825a7bbc
	if (ctx.cr6.lt) goto loc_825A7BBC;
loc_825A7C28:
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

