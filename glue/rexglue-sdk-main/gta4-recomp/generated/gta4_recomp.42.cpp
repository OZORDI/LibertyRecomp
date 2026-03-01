#include "gta4_init.h"

PPC_FUNC_IMPL(__imp__sub_826E42F0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E42F0) { __imp__sub_826E42F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E42F0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x826E42F8;
	__savegprlr_27(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// mr r31,r4
	ctx.r31.u64 = ctx.r4.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// mr r28,r7
	ctx.r28.u64 = ctx.r7.u64;
	// bl 0x826ba0a0
	ctx.lr = 0x826E4314;
	sub_826BA0A0(ctx, base);
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826af7c0
	ctx.lr = 0x826E4320;
	sub_826AF7C0(ctx, base);
	// lwz r11,0(r28)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r28.u32 + 0);
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
	// rlwinm r11,r11,17,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 17) & 0x1;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e436c
	if (ctx.cr6.eq) goto loc_826E436C;
	// li r4,2
	ctx.r4.s64 = 2;
	// bl 0x826e3b20
	ctx.lr = 0x826E4340;
	sub_826E3B20(ctx, base);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// beq cr6,0x826e4378
	if (ctx.cr6.eq) goto loc_826E4378;
	// lwz r11,0(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// ori r11,r11,32768
	ctx.r11.u64 = ctx.r11.u64 | 32768;
	// stw r11,0(r30)
	PPC_STORE_U32(ctx.r30.u32 + 0, ctx.r11.u32);
	// lwz r11,0(r27)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r27.u32 + 0);
	// ori r11,r11,32768
	ctx.r11.u64 = ctx.r11.u64 | 32768;
	// stw r11,0(r27)
	PPC_STORE_U32(ctx.r27.u32 + 0, ctx.r11.u32);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_826E436C:
	// li r4,1
	ctx.r4.s64 = 1;
	// bl 0x826e3b20
	ctx.lr = 0x826E4374;
	sub_826E3B20(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
loc_826E4378:
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E4380);  /* forward decl */
PPC_WEAK_FUNC(sub_826E4380) { __imp__sub_826E4380(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E4380) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x826E4388;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// mr r31,r5
	ctx.r31.u64 = ctx.r5.u64;
	// bl 0x826ba6b0
	ctx.lr = 0x826E439C;
	sub_826BA6B0(ctx, base);
	// rlwinm r11,r30,17,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 17) & 0x1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e43c0
	if (ctx.cr6.eq) goto loc_826E43C0;
	// li r4,3
	ctx.r4.s64 = 3;
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e3b20
	ctx.lr = 0x826E43B8;
	sub_826E3B20(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
loc_826E43C0:
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826E43D4;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e43f0
	if (ctx.cr6.eq) goto loc_826E43F0;
	// li r4,1
	ctx.r4.s64 = 1;
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e3b20
	ctx.lr = 0x826E43F0;
	sub_826E3B20(ctx, base);
loc_826E43F0:
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E43F8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E43F8) { __imp__sub_826E43F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E43F8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x826E4400;
	__savegprlr_27(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// mr r31,r5
	ctx.r31.u64 = ctx.r5.u64;
	// mr r29,r6
	ctx.r29.u64 = ctx.r6.u64;
	// mr r28,r7
	ctx.r28.u64 = ctx.r7.u64;
	// bl 0x826baa20
	ctx.lr = 0x826E441C;
	sub_826BAA20(ctx, base);
	// rlwinm r11,r31,17,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 17) & 0x1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e4458
	if (ctx.cr6.eq) goto loc_826E4458;
	// mr r5,r29
	ctx.r5.u64 = ctx.r29.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e3b20
	ctx.lr = 0x826E4438;
	sub_826E3B20(ctx, base);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// bne cr6,0x826e445c
	if (!ctx.cr6.eq) goto loc_826E445C;
	// lwz r11,0(r27)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r27.u32 + 0);
	// ori r11,r11,32768
	ctx.r11.u64 = ctx.r11.u64 | 32768;
	// stw r11,0(r27)
	PPC_STORE_U32(ctx.r27.u32 + 0, ctx.r11.u32);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_826E4458:
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
loc_826E445C:
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E4468);  /* forward decl */
PPC_WEAK_FUNC(sub_826E4468) { __imp__sub_826E4468(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E4468) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x826E4470;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// bl 0x826bac30
	ctx.lr = 0x826E4484;
	sub_826BAC30(ctx, base);
	// rlwinm r11,r30,17,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 17) & 0x1;
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826e44b4
	if (!ctx.cr6.eq) goto loc_826E44B4;
	// lwz r11,0(r29)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r29.u32 + 0);
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// lwz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826E44A8;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e44d4
	if (ctx.cr6.eq) goto loc_826E44D4;
loc_826E44B4:
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r11,24(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826E44C8;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// addi r3,r30,15
	ctx.r3.s64 = ctx.r30.s64 + 15;
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
loc_826E44D4:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E44E0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E44E0) { __imp__sub_826E44E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E44E0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826E44E8;
	__savegprlr_25(ctx, base);
	// stfd f30,-80(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -80, ctx.f30.u64);
	// stfd f31,-72(r1)
	PPC_STORE_U64(ctx.r1.u32 + -72, ctx.f31.u64);
	// stwu r1,-256(r1)
	ea = -256 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// mr r28,r4
	ctx.r28.u64 = ctx.r4.u64;
	// mr r31,r5
	ctx.r31.u64 = ctx.r5.u64;
	// lwz r11,0(r27)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r27.u32 + 0);
	// lwz r11,24(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826E4510;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r25,0
	ctx.r25.s64 = 0;
	// li r26,255
	ctx.r26.s64 = 255;
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// lfs f31,3068(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3068);
	ctx.f31.f64 = double(temp.f32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// stfs f31,100(r1)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 100, temp.u32);
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// stw r25,104(r1)
	PPC_STORE_U32(ctx.r1.u32 + 104, ctx.r25.u32);
	// mr r29,r25
	ctx.r29.u64 = ctx.r25.u64;
	// stw r26,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r26.u32);
	// stw r25,88(r1)
	PPC_STORE_U32(ctx.r1.u32 + 88, ctx.r25.u32);
	// stw r11,96(r1)
	PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
	// stb r25,80(r1)
	PPC_STORE_U8(ctx.r1.u32 + 80, ctx.r25.u8);
	// bl 0x826b5f38
	ctx.lr = 0x826E4554;
	sub_826B5F38(ctx, base);
	// stw r3,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r3.u32);
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// beq cr6,0x826e45dc
	if (ctx.cr6.eq) goto loc_826E45DC;
	// lwz r11,544(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 544);
	// lwz r11,1184(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 1184);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e4578
	if (ctx.cr6.eq) goto loc_826E4578;
	// li r29,1
	ctx.r29.s64 = 1;
loc_826E4578:
	// lbz r11,3137(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + 3137);
	// cmplwi cr6,r11,2
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 2, ctx.xer);
	// beq cr6,0x826e4594
	if (ctx.cr6.eq) goto loc_826E4594;
	// cmplwi cr6,r11,3
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 3, ctx.xer);
	// beq cr6,0x826e4594
	if (ctx.cr6.eq) goto loc_826E4594;
	// lbz r11,3136(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + 3136);
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
loc_826E4594:
	// lwz r11,544(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 544);
	// lbz r10,14(r27)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r27.u32 + 14);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// lwz r11,184(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 184);
	// stw r11,88(r1)
	PPC_STORE_U32(ctx.r1.u32 + 88, ctx.r11.u32);
	// lwz r11,544(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 544);
	// lwz r11,1368(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 1368);
	// stw r11,96(r1)
	PPC_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
	// lwz r11,544(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 544);
	// lfs f0,1200(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 1200);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,100(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 100, temp.u32);
	// beq cr6,0x826e45d0
	if (ctx.cr6.eq) goto loc_826E45D0;
	// lbz r11,1152(r27)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r27.u32 + 1152);
	// rlwinm r11,r11,26,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 26) & 0x1;
	// b 0x826e45d8
	goto loc_826E45D8;
loc_826E45D0:
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// lbz r11,17432(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 17432);
loc_826E45D8:
	// stb r11,80(r1)
	PPC_STORE_U8(ctx.r1.u32 + 80, ctx.r11.u8);
loc_826E45DC:
	// cmpwi cr6,r28,4
	ctx.cr6.compare<int32_t>(ctx.r28.s32, 4, ctx.xer);
	// beq cr6,0x826e45f0
	if (ctx.cr6.eq) goto loc_826E45F0;
	// cmpwi cr6,r28,5
	ctx.cr6.compare<int32_t>(ctx.r28.s32, 5, ctx.xer);
	// beq cr6,0x826e45f0
	if (ctx.cr6.eq) goto loc_826E45F0;
	// sth r25,1164(r27)
	PPC_STORE_U16(ctx.r27.u32 + 1164, ctx.r25.u16);
loc_826E45F0:
	// cmplwi cr6,r28,5
	ctx.cr6.compare<uint32_t>(ctx.r28.u32, 5, ctx.xer);
	// bgt cr6,0x826e4ee4
	if (ctx.cr6.gt) goto loc_826E4EE4;
	// lis r12,-32146
	ctx.r12.s64 = -2106720256;
	// addi r12,r12,17936
	ctx.r12.s64 = ctx.r12.s64 + 17936;
	// rlwinm r0,r28,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r28.u32 | (ctx.r28.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r28.u32) {
	case 0:
		goto loc_826E4628;
	case 1:
		goto loc_826E4894;
	case 2:
		goto loc_826E495C;
	case 3:
		goto loc_826E4B4C;
	case 4:
		goto loc_826E4D60;
	case 5:
		goto loc_826E4EA4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E4628:
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r4,r1,104
	ctx.r4.s64 = ctx.r1.s64 + 104;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826be460
	ctx.lr = 0x826E4638;
	sub_826BE460(ctx, base);
	// lwz r11,104(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 104);
	// clrlwi r10,r29,31
	ctx.r10.u64 = ctx.r29.u32 & 0x1;
	// clrlwi r11,r11,31
	ctx.r11.u64 = ctx.r11.u32 & 0x1;
	// cmplw cr6,r11,r10
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, ctx.r10.u32, ctx.xer);
	// beq cr6,0x826e4690
	if (ctx.cr6.eq) goto loc_826E4690;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// lwz r3,544(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 544);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r8,0
	ctx.r8.s64 = 0;
	// li r7,1
	ctx.r7.s64 = 1;
	// li r4,0
	ctx.r4.s64 = 0;
	// lfs f1,29496(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 29496);
	ctx.f1.f64 = double(temp.f32);
	// beq cr6,0x826e4678
	if (ctx.cr6.eq) goto loc_826E4678;
	// li r5,1
	ctx.r5.s64 = 1;
	// bl 0x822fe6c8
	ctx.lr = 0x826E4674;
	sub_822FE6C8(ctx, base);
	// b 0x826e4690
	goto loc_826E4690;
loc_826E4678:
	// li r5,0
	ctx.r5.s64 = 0;
	// bl 0x822fe6c8
	ctx.lr = 0x826E4680;
	sub_822FE6C8(ctx, base);
	// lbz r11,172(r27)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r27.u32 + 172);
	// lwz r10,36(r30)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r30.u32 + 36);
	// rlwimi r10,r11,31,0,0
	ctx.r10.u64 = (__builtin_rotateleft32(ctx.r11.u32, 31) & 0x80000000) | (ctx.r10.u64 & 0xFFFFFFFF7FFFFFFF);
	// stw r10,36(r30)
	PPC_STORE_U32(ctx.r30.u32 + 36, ctx.r10.u32);
loc_826E4690:
	// li r5,8
	ctx.r5.s64 = 8;
	// addi r4,r1,92
	ctx.r4.s64 = ctx.r1.s64 + 92;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826be460
	ctx.lr = 0x826E46A0;
	sub_826BE460(ctx, base);
	// lwz r4,92(r1)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r1.u32 + 92);
	// cmplwi cr6,r4,255
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 255, ctx.xer);
	// bne cr6,0x826e46b4
	if (!ctx.cr6.eq) goto loc_826E46B4;
	// stb r26,3136(r30)
	PPC_STORE_U8(ctx.r30.u32 + 3136, ctx.r26.u8);
	// b 0x826e46c8
	goto loc_826E46C8;
loc_826E46B4:
	// lbz r11,3136(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + 3136);
	// cmplw cr6,r11,r4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, ctx.r4.u32, ctx.xer);
	// beq cr6,0x826e46c8
	if (ctx.cr6.eq) goto loc_826E46C8;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8223d438
	ctx.lr = 0x826E46C8;
	sub_8223D438(ctx, base);
loc_826E46C8:
	// li r5,3
	ctx.r5.s64 = 3;
	// addi r4,r1,88
	ctx.r4.s64 = ctx.r1.s64 + 88;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826be460
	ctx.lr = 0x826E46D8;
	sub_826BE460(ctx, base);
	// lwz r11,544(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 544);
	// lwz r10,88(r1)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
	// li r5,4
	ctx.r5.s64 = 4;
	// addi r4,r1,96
	ctx.r4.s64 = ctx.r1.s64 + 96;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r10,184(r11)
	PPC_STORE_U32(ctx.r11.u32 + 184, ctx.r10.u32);
	// bl 0x826be4c0
	ctx.lr = 0x826E46F4;
	sub_826BE4C0(ctx, base);
	// lwz r10,544(r30)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r30.u32 + 544);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lwz r9,96(r1)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 96);
	// li r6,7
	ctx.r6.s64 = 7;
	// addi r4,r1,100
	ctx.r4.s64 = ctx.r1.s64 + 100;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lfs f1,7908(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 7908);
	ctx.f1.f64 = double(temp.f32);
	// stw r9,1368(r10)
	PPC_STORE_U32(ctx.r10.u32 + 1368, ctx.r9.u32);
	// bl 0x826be560
	ctx.lr = 0x826E4718;
	sub_826BE560(ctx, base);
	// lwz r11,544(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 544);
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lfs f0,100(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 100);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,1200(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 1200, temp.u32);
	// bl 0x826bdc68
	ctx.lr = 0x826E4730;
	sub_826BDC68(ctx, base);
	// lbz r11,1152(r27)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r27.u32 + 1152);
	// addi r4,r1,81
	ctx.r4.s64 = ctx.r1.s64 + 81;
	// lbz r10,80(r1)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r1.u32 + 80);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stb r25,81(r1)
	PPC_STORE_U8(ctx.r1.u32 + 81, ctx.r25.u8);
	// rlwimi r11,r10,6,25,25
	ctx.r11.u64 = (__builtin_rotateleft32(ctx.r10.u32, 6) & 0x40) | (ctx.r11.u64 & 0xFFFFFFFFFFFFFFBF);
	// stb r11,1152(r27)
	PPC_STORE_U8(ctx.r27.u32 + 1152, ctx.r11.u8);
	// bl 0x826bdc68
	ctx.lr = 0x826E4750;
	sub_826BDC68(ctx, base);
	// lbz r11,81(r1)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r1.u32 + 81);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826e47a8
	if (!ctx.cr6.eq) goto loc_826E47A8;
	// addi r31,r27,1128
	ctx.r31.s64 = ctx.r27.s64 + 1128;
	// bl 0x826c2e98
	ctx.lr = 0x826E4764;
	sub_826C2E98(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826c5888
	ctx.lr = 0x826E4770;
	sub_826C5888(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e4ee4
	if (ctx.cr6.eq) goto loc_826E4EE4;
	// bl 0x826c2e98
	ctx.lr = 0x826E4780;
	sub_826C2E98(ctx, base);
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// li r6,1
	ctx.r6.s64 = 1;
	// bl 0x826c56a8
	ctx.lr = 0x826E4794;
	sub_826C56A8(ctx, base);
	// lwz r3,84(r1)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	// addi r1,r1,256
	ctx.r1.s64 = ctx.r1.s64 + 256;
	// lfd f30,-80(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -80);
	// lfd f31,-72(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -72);
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826E47A8:
	// addi r28,r27,1164
	ctx.r28.s64 = ctx.r27.s64 + 1164;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// bl 0x826be7b0
	ctx.lr = 0x826E47B8;
	sub_826BE7B0(ctx, base);
	// lis r29,-32251
	ctx.r29.s64 = -2113601536;
	// li r6,12
	ctx.r6.s64 = 12;
	// addi r4,r1,128
	ctx.r4.s64 = ctx.r1.s64 + 128;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lfs f1,28972(r29)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r29.u32 + 28972);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x826be4c8
	ctx.lr = 0x826E47D0;
	sub_826BE4C8(ctx, base);
	// li r6,12
	ctx.r6.s64 = 12;
	// lfs f1,28972(r29)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r29.u32 + 28972);
	ctx.f1.f64 = double(temp.f32);
	// addi r4,r1,132
	ctx.r4.s64 = ctx.r1.s64 + 132;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826be4c8
	ctx.lr = 0x826E47E4;
	sub_826BE4C8(ctx, base);
	// stfs f31,136(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 136, temp.u32);
	// addi r31,r27,1128
	ctx.r31.s64 = ctx.r27.s64 + 1128;
	// bl 0x826c2e98
	ctx.lr = 0x826E47F0;
	sub_826C2E98(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826c5888
	ctx.lr = 0x826E47FC;
	sub_826C5888(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e4840
	if (ctx.cr6.eq) goto loc_826E4840;
	// bl 0x826c2e98
	ctx.lr = 0x826E480C;
	sub_826C2E98(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826c5f38
	ctx.lr = 0x826E4818;
	sub_826C5F38(ctx, base);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x826e4ee4
	if (ctx.cr6.eq) goto loc_826E4EE4;
	// addi r5,r1,128
	ctx.r5.s64 = ctx.r1.s64 + 128;
	// lhz r4,0(r28)
	ctx.r4.u64 = PPC_LOAD_U16(ctx.r28.u32 + 0);
	// bl 0x826c2c88
	ctx.lr = 0x826E482C;
	sub_826C2C88(ctx, base);
	// lwz r3,84(r1)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	// addi r1,r1,256
	ctx.r1.s64 = ctx.r1.s64 + 256;
	// lfd f30,-80(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -80);
	// lfd f31,-72(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -72);
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826E4840:
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// lwz r3,4344(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4344);
	// bl 0x821250b0
	ctx.lr = 0x826E484C;
	sub_821250B0(ctx, base);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x826e4868
	if (ctx.cr6.eq) goto loc_826E4868;
	// addi r5,r1,128
	ctx.r5.s64 = ctx.r1.s64 + 128;
	// lhz r4,0(r28)
	ctx.r4.u64 = PPC_LOAD_U16(ctx.r28.u32 + 0);
	// bl 0x826c36f8
	ctx.lr = 0x826E4860;
	sub_826C36F8(ctx, base);
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
	// b 0x826e486c
	goto loc_826E486C;
loc_826E4868:
	// mr r5,r25
	ctx.r5.u64 = ctx.r25.u64;
loc_826E486C:
	// li r7,1
	ctx.r7.s64 = 1;
	// li r6,1
	ctx.r6.s64 = 1;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826c5fa8
	ctx.lr = 0x826E4880;
	sub_826C5FA8(ctx, base);
	// lwz r3,84(r1)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	// addi r1,r1,256
	ctx.r1.s64 = ctx.r1.s64 + 256;
	// lfd f30,-80(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -80);
	// lfd f31,-72(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -72);
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826E4894:
	// li r5,1
	ctx.r5.s64 = 1;
	// li r4,1
	ctx.r4.s64 = 1;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826be7f0
	ctx.lr = 0x826E48A4;
	sub_826BE7F0(ctx, base);
	// li r5,1
	ctx.r5.s64 = 1;
	// li r4,8
	ctx.r4.s64 = 8;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826be7f0
	ctx.lr = 0x826E48B4;
	sub_826BE7F0(ctx, base);
	// li r5,3
	ctx.r5.s64 = 3;
	// addi r4,r1,88
	ctx.r4.s64 = ctx.r1.s64 + 88;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826be460
	ctx.lr = 0x826E48C4;
	sub_826BE460(ctx, base);
	// li r5,1
	ctx.r5.s64 = 1;
	// li r4,4
	ctx.r4.s64 = 4;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826bdcf8
	ctx.lr = 0x826E48D4;
	sub_826BDCF8(ctx, base);
	// li r5,1
	ctx.r5.s64 = 1;
	// li r4,7
	ctx.r4.s64 = 7;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826be870
	ctx.lr = 0x826E48E4;
	sub_826BE870(ctx, base);
	// li r4,1
	ctx.r4.s64 = 1;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826bdd60
	ctx.lr = 0x826E48F0;
	sub_826BDD60(ctx, base);
	// addi r4,r1,81
	ctx.r4.s64 = ctx.r1.s64 + 81;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stb r25,81(r1)
	PPC_STORE_U8(ctx.r1.u32 + 81, ctx.r25.u8);
	// bl 0x826bdc68
	ctx.lr = 0x826E4900;
	sub_826BDC68(ctx, base);
	// lbz r11,81(r1)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r1.u32 + 81);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826e492c
	if (!ctx.cr6.eq) goto loc_826E492C;
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826E4920;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e4ee4
	if (ctx.cr6.eq) goto loc_826E4EE4;
loc_826E492C:
	// addi r4,r27,1164
	ctx.r4.s64 = ctx.r27.s64 + 1164;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826be7b0
	ctx.lr = 0x826E4938;
	sub_826BE7B0(ctx, base);
	// li r5,2
	ctx.r5.s64 = 2;
	// li r4,12
	ctx.r4.s64 = 12;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826bddd8
	ctx.lr = 0x826E4948;
	sub_826BDDD8(ctx, base);
	// lwz r3,84(r1)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	// addi r1,r1,256
	ctx.r1.s64 = ctx.r1.s64 + 256;
	// lfd f30,-80(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -80);
	// lfd f31,-72(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -72);
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826E495C:
	// addi r6,r1,84
	ctx.r6.s64 = ctx.r1.s64 + 84;
	// li r5,1
	ctx.r5.s64 = 1;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826beaa0
	ctx.lr = 0x826E4970;
	sub_826BEAA0(ctx, base);
	// addi r6,r1,84
	ctx.r6.s64 = ctx.r1.s64 + 84;
	// li r5,8
	ctx.r5.s64 = 8;
	// lwz r4,92(r1)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r1.u32 + 92);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826beaa0
	ctx.lr = 0x826E4984;
	sub_826BEAA0(ctx, base);
	// addi r6,r1,84
	ctx.r6.s64 = ctx.r1.s64 + 84;
	// li r5,3
	ctx.r5.s64 = 3;
	// lwz r4,88(r1)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826beaa0
	ctx.lr = 0x826E4998;
	sub_826BEAA0(ctx, base);
	// addi r6,r1,84
	ctx.r6.s64 = ctx.r1.s64 + 84;
	// li r5,4
	ctx.r5.s64 = 4;
	// lwz r4,96(r1)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r1.u32 + 96);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826beb58
	ctx.lr = 0x826E49AC;
	sub_826BEB58(ctx, base);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r7,r1,84
	ctx.r7.s64 = ctx.r1.s64 + 84;
	// lfs f1,100(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 100);
	ctx.f1.f64 = double(temp.f32);
	// li r6,7
	ctx.r6.s64 = 7;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lfs f2,7908(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 7908);
	ctx.f2.f64 = double(temp.f32);
	// bl 0x826bec58
	ctx.lr = 0x826E49C8;
	sub_826BEC58(ctx, base);
	// addi r5,r1,84
	ctx.r5.s64 = ctx.r1.s64 + 84;
	// lbz r4,80(r1)
	ctx.r4.u64 = PPC_LOAD_U8(ctx.r1.u32 + 80);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826bdec0
	ctx.lr = 0x826E49D8;
	sub_826BDEC0(ctx, base);
	// lwz r11,2528(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 2528);
	// stfs f31,112(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 112, temp.u32);
	// mr r28,r25
	ctx.r28.u64 = ctx.r25.u64;
	// stfs f31,116(r1)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 116, temp.u32);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// lis r29,-32251
	ctx.r29.s64 = -2113601536;
	// beq cr6,0x826e4a9c
	if (ctx.cr6.eq) goto loc_826E4A9C;
	// lwz r10,100(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 100);
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// beq cr6,0x826e4a9c
	if (ctx.cr6.eq) goto loc_826E4A9C;
	// lwz r10,32(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 32);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// addi r10,r10,48
	ctx.r10.s64 = ctx.r10.s64 + 48;
	// bne cr6,0x826e4a14
	if (!ctx.cr6.eq) goto loc_826E4A14;
	// addi r10,r11,16
	ctx.r10.s64 = ctx.r11.s64 + 16;
loc_826E4A14:
	// lwz r9,32(r30)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r30.u32 + 32);
	// li r8,48
	ctx.r8.s64 = 48;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lwz r11,32(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 32);
	// li r10,32
	ctx.r10.s64 = 32;
	// lfs f0,28972(r29)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r29.u32 + 28972);
	ctx.f0.f64 = double(temp.f32);
	// li r7,16
	ctx.r7.s64 = 16;
	// lvx128 v13,r9,r8
	ea = (ctx.r9.u32 + ctx.r8.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r9,r1,144
	ctx.r9.s64 = ctx.r1.s64 + 144;
	// vsubfp v0,v13,v0
	ctx.fpscr.enableFlushModeUnconditional();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_sub_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v0.f32)));
	// lvx128 v12,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lvx128 v13,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lvx128 v11,r11,r7
	ea = (ctx.r11.u32 + ctx.r7.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v11.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx128 v0,r0,r9
	ea = (ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stfs f31,152(r1)
	ctx.fpscr.disableFlushModeUnconditional();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 152, temp.u32);
	// addi r11,r1,144
	ctx.r11.s64 = ctx.r1.s64 + 144;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,112
	ctx.r11.s64 = ctx.r1.s64 + 112;
	// vmsum3fp128 v13,v13,v0
	ctx.fpscr.enableFlushModeUnconditional();
	simde_mm_store_ps(ctx.v13.f32, simde_mm_dp_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v0.f32), 0xEF));
	// vmsum3fp128 v12,v12,v0
	simde_mm_store_ps(ctx.v12.f32, simde_mm_dp_ps(simde_mm_load_ps(ctx.v12.f32), simde_mm_load_ps(ctx.v0.f32), 0xEF));
	// vmsum3fp128 v11,v11,v0
	simde_mm_store_ps(ctx.v11.f32, simde_mm_dp_ps(simde_mm_load_ps(ctx.v11.f32), simde_mm_load_ps(ctx.v0.f32), 0xEF));
	// vmrghw v0,v12,v13
	simde_mm_store_si128((simde__m128i*)ctx.v0.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)ctx.v13.u32), simde_mm_load_si128((simde__m128i*)ctx.v12.u32)));
	// vmrghw v13,v11,v0
	simde_mm_store_si128((simde__m128i*)ctx.v13.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)ctx.v0.u32), simde_mm_load_si128((simde__m128i*)ctx.v11.u32)));
	// vmrghw v0,v0,v13
	simde_mm_store_si128((simde__m128i*)ctx.v0.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)ctx.v13.u32), simde_mm_load_si128((simde__m128i*)ctx.v0.u32)));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lfs f31,112(r1)
	ctx.fpscr.disableFlushModeUnconditional();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 112);
	ctx.f31.f64 = double(temp.f32);
	// fabs f13,f31
	ctx.f13.u64 = ctx.f31.u64 & ~0x8000000000000000;
	// fcmpu cr6,f13,f0
	ctx.cr6.compare(ctx.f13.f64, ctx.f0.f64);
	// bge cr6,0x826e4a9c
	if (!ctx.cr6.lt) goto loc_826E4A9C;
	// lfs f13,116(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 116);
	ctx.f13.f64 = double(temp.f32);
	// fabs f13,f13
	ctx.f13.u64 = ctx.f13.u64 & ~0x8000000000000000;
	// fcmpu cr6,f13,f0
	ctx.cr6.compare(ctx.f13.f64, ctx.f0.f64);
	// bge cr6,0x826e4a9c
	if (!ctx.cr6.lt) goto loc_826E4A9C;
	// li r28,1
	ctx.r28.s64 = 1;
loc_826E4A9C:
	// addi r5,r1,84
	ctx.r5.s64 = ctx.r1.s64 + 84;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826bdec0
	ctx.lr = 0x826E4AAC;
	sub_826BDEC0(ctx, base);
	// clrlwi r11,r28,24
	ctx.r11.u64 = ctx.r28.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e4b1c
	if (ctx.cr6.eq) goto loc_826E4B1C;
	// lwz r11,2528(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 2528);
	// lwz r3,100(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 100);
	// bl 0x826ae700
	ctx.lr = 0x826E4AC4;
	sub_826AE700(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r5,r1,84
	ctx.r5.s64 = ctx.r1.s64 + 84;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// sth r4,1164(r27)
	PPC_STORE_U16(ctx.r27.u32 + 1164, ctx.r4.u16);
	// bl 0x826bef00
	ctx.lr = 0x826E4AD8;
	sub_826BEF00(ctx, base);
	// addi r7,r1,84
	ctx.r7.s64 = ctx.r1.s64 + 84;
	// lfs f2,28972(r29)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r29.u32 + 28972);
	ctx.f2.f64 = double(temp.f32);
	// li r6,12
	ctx.r6.s64 = 12;
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826bebf0
	ctx.lr = 0x826E4AF0;
	sub_826BEBF0(ctx, base);
	// addi r7,r1,84
	ctx.r7.s64 = ctx.r1.s64 + 84;
	// li r6,12
	ctx.r6.s64 = 12;
	// lfs f2,28972(r29)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r29.u32 + 28972);
	ctx.f2.f64 = double(temp.f32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lfs f1,116(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 116);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x826bebf0
	ctx.lr = 0x826E4B08;
	sub_826BEBF0(ctx, base);
	// lwz r3,84(r1)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	// addi r1,r1,256
	ctx.r1.s64 = ctx.r1.s64 + 256;
	// lfd f30,-80(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -80);
	// lfd f31,-72(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -72);
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826E4B1C:
	// li r4,1
	ctx.r4.s64 = 1;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826be960
	ctx.lr = 0x826E4B28;
	sub_826BE960(ctx, base);
	// li r5,2
	ctx.r5.s64 = 2;
	// li r4,12
	ctx.r4.s64 = 12;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826bddd8
	ctx.lr = 0x826E4B38;
	sub_826BDDD8(ctx, base);
	// lwz r3,84(r1)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	// addi r1,r1,256
	ctx.r1.s64 = ctx.r1.s64 + 256;
	// lfd f30,-80(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -80);
	// lfd f31,-72(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -72);
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826E4B4C:
	// li r5,1
	ctx.r5.s64 = 1;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826be088
	ctx.lr = 0x826E4B5C;
	sub_826BE088(ctx, base);
	// li r5,8
	ctx.r5.s64 = 8;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,92(r1)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r1.u32 + 92);
	// bl 0x826be088
	ctx.lr = 0x826E4B6C;
	sub_826BE088(ctx, base);
	// li r5,3
	ctx.r5.s64 = 3;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,88(r1)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
	// bl 0x826be088
	ctx.lr = 0x826E4B7C;
	sub_826BE088(ctx, base);
	// li r5,4
	ctx.r5.s64 = 4;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,96(r1)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r1.u32 + 96);
	// bl 0x826be0a8
	ctx.lr = 0x826E4B8C;
	sub_826BE0A8(ctx, base);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,7
	ctx.r6.s64 = 7;
	// lfs f1,100(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 100);
	ctx.f1.f64 = double(temp.f32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lfs f2,7908(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 7908);
	ctx.f2.f64 = double(temp.f32);
	// bl 0x826be118
	ctx.lr = 0x826E4BA4;
	sub_826BE118(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lbz r4,80(r1)
	ctx.r4.u64 = PPC_LOAD_U8(ctx.r1.u32 + 80);
	// bl 0x826bdc48
	ctx.lr = 0x826E4BB0;
	sub_826BDC48(ctx, base);
	// lwz r11,2528(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 2528);
	// fmr f30,f31
	ctx.fpscr.disableFlushMode();
	ctx.f30.f64 = ctx.f31.f64;
	// stfs f31,112(r1)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 112, temp.u32);
	// stfs f30,116(r1)
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r1.u32 + 116, temp.u32);
	// mr r29,r25
	ctx.r29.u64 = ctx.r25.u64;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// lis r28,-32251
	ctx.r28.s64 = -2113601536;
	// beq cr6,0x826e4cc4
	if (ctx.cr6.eq) goto loc_826E4CC4;
	// lwz r10,100(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 100);
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// beq cr6,0x826e4cc4
	if (ctx.cr6.eq) goto loc_826E4CC4;
	// lwz r10,32(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 32);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// addi r10,r10,48
	ctx.r10.s64 = ctx.r10.s64 + 48;
	// bne cr6,0x826e4bf0
	if (!ctx.cr6.eq) goto loc_826E4BF0;
	// addi r10,r11,16
	ctx.r10.s64 = ctx.r11.s64 + 16;
loc_826E4BF0:
	// lwz r9,32(r30)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r30.u32 + 32);
	// li r8,48
	ctx.r8.s64 = 48;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lwz r11,32(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 32);
	// li r10,32
	ctx.r10.s64 = 32;
	// lfs f0,28972(r28)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r28.u32 + 28972);
	ctx.f0.f64 = double(temp.f32);
	// li r7,16
	ctx.r7.s64 = 16;
	// lvx128 v13,r9,r8
	ea = (ctx.r9.u32 + ctx.r8.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r9,r1,144
	ctx.r9.s64 = ctx.r1.s64 + 144;
	// vsubfp v0,v13,v0
	ctx.fpscr.enableFlushModeUnconditional();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_sub_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v0.f32)));
	// lvx128 v12,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lvx128 v13,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lvx128 v11,r11,r7
	ea = (ctx.r11.u32 + ctx.r7.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v11.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx128 v0,r0,r9
	ea = (ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stfs f31,152(r1)
	ctx.fpscr.disableFlushModeUnconditional();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 152, temp.u32);
	// addi r11,r1,144
	ctx.r11.s64 = ctx.r1.s64 + 144;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,112
	ctx.r11.s64 = ctx.r1.s64 + 112;
	// vmsum3fp128 v13,v13,v0
	ctx.fpscr.enableFlushModeUnconditional();
	simde_mm_store_ps(ctx.v13.f32, simde_mm_dp_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v0.f32), 0xEF));
	// vmsum3fp128 v12,v12,v0
	simde_mm_store_ps(ctx.v12.f32, simde_mm_dp_ps(simde_mm_load_ps(ctx.v12.f32), simde_mm_load_ps(ctx.v0.f32), 0xEF));
	// vmsum3fp128 v11,v11,v0
	simde_mm_store_ps(ctx.v11.f32, simde_mm_dp_ps(simde_mm_load_ps(ctx.v11.f32), simde_mm_load_ps(ctx.v0.f32), 0xEF));
	// vmrghw v0,v12,v13
	simde_mm_store_si128((simde__m128i*)ctx.v0.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)ctx.v13.u32), simde_mm_load_si128((simde__m128i*)ctx.v12.u32)));
	// vmrghw v13,v11,v0
	simde_mm_store_si128((simde__m128i*)ctx.v13.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)ctx.v0.u32), simde_mm_load_si128((simde__m128i*)ctx.v11.u32)));
	// vmrghw v0,v0,v13
	simde_mm_store_si128((simde__m128i*)ctx.v0.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)ctx.v13.u32), simde_mm_load_si128((simde__m128i*)ctx.v0.u32)));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lfs f31,112(r1)
	ctx.fpscr.disableFlushModeUnconditional();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 112);
	ctx.f31.f64 = double(temp.f32);
	// fabs f13,f31
	ctx.f13.u64 = ctx.f31.u64 & ~0x8000000000000000;
	// lfs f30,116(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 116);
	ctx.f30.f64 = double(temp.f32);
	// fcmpu cr6,f13,f0
	ctx.cr6.compare(ctx.f13.f64, ctx.f0.f64);
	// bge cr6,0x826e4c7c
	if (!ctx.cr6.lt) goto loc_826E4C7C;
	// fabs f13,f30
	ctx.f13.u64 = ctx.f30.u64 & ~0x8000000000000000;
	// fcmpu cr6,f13,f0
	ctx.cr6.compare(ctx.f13.f64, ctx.f0.f64);
	// bge cr6,0x826e4c7c
	if (!ctx.cr6.lt) goto loc_826E4C7C;
	// li r29,1
	ctx.r29.s64 = 1;
	// b 0x826e4cc4
	goto loc_826E4CC4;
loc_826E4C7C:
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f0,-23972(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -23972);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// fsubs f13,f31,f0
	ctx.f13.f64 = double(float(ctx.f31.f64 - ctx.f0.f64));
	// fsubs f12,f30,f0
	ctx.f12.f64 = double(float(ctx.f30.f64 - ctx.f0.f64));
	// lfd f0,30176(r11)
	ctx.f0.u64 = PPC_LOAD_U64(ctx.r11.u32 + 30176);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// fsel f11,f13,f31,f0
	ctx.f11.f64 = ctx.f13.f64 >= 0.0 ? ctx.f31.f64 : ctx.f0.f64;
	// fsel f10,f12,f30,f0
	ctx.f10.f64 = ctx.f12.f64 >= 0.0 ? ctx.f30.f64 : ctx.f0.f64;
	// fsel f13,f13,f31,f0
	ctx.f13.f64 = ctx.f13.f64 >= 0.0 ? ctx.f31.f64 : ctx.f0.f64;
	// fsel f12,f12,f30,f0
	ctx.f12.f64 = ctx.f12.f64 >= 0.0 ? ctx.f30.f64 : ctx.f0.f64;
	// lfd f0,30168(r11)
	ctx.f0.u64 = PPC_LOAD_U64(ctx.r11.u32 + 30168);
	// fsub f11,f11,f0
	ctx.f11.f64 = ctx.f11.f64 - ctx.f0.f64;
	// fsub f10,f10,f0
	ctx.f10.f64 = ctx.f10.f64 - ctx.f0.f64;
	// fsel f13,f11,f0,f13
	ctx.f13.f64 = ctx.f11.f64 >= 0.0 ? ctx.f0.f64 : ctx.f13.f64;
	// fsel f0,f10,f0,f12
	ctx.f0.f64 = ctx.f10.f64 >= 0.0 ? ctx.f0.f64 : ctx.f12.f64;
	// frsp f31,f13
	ctx.f31.f64 = double(float(ctx.f13.f64));
	// frsp f30,f0
	ctx.f30.f64 = double(float(ctx.f0.f64));
loc_826E4CC4:
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826bdc48
	ctx.lr = 0x826E4CD0;
	sub_826BDC48(ctx, base);
	// clrlwi r29,r29,24
	ctx.r29.u64 = ctx.r29.u32 & 0xFF;
	// cmplwi cr6,r29,0
	ctx.cr6.compare<uint32_t>(ctx.r29.u32, 0, ctx.xer);
	// bne cr6,0x826e4cfc
	if (!ctx.cr6.eq) goto loc_826E4CFC;
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826E4CF0;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e4ee4
	if (ctx.cr6.eq) goto loc_826E4EE4;
loc_826E4CFC:
	// cmplwi cr6,r29,0
	ctx.cr6.compare<uint32_t>(ctx.r29.u32, 0, ctx.xer);
	// sth r25,1164(r27)
	PPC_STORE_U16(ctx.r27.u32 + 1164, ctx.r25.u16);
	// beq cr6,0x826e4d18
	if (ctx.cr6.eq) goto loc_826E4D18;
	// lwz r11,2528(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 2528);
	// lwz r3,100(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 100);
	// bl 0x826ae700
	ctx.lr = 0x826E4D14;
	sub_826AE700(ctx, base);
	// sth r3,1164(r27)
	PPC_STORE_U16(ctx.r27.u32 + 1164, ctx.r3.u16);
loc_826E4D18:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lhz r4,1164(r27)
	ctx.r4.u64 = PPC_LOAD_U16(ctx.r27.u32 + 1164);
	// bl 0x826be450
	ctx.lr = 0x826E4D24;
	sub_826BE450(ctx, base);
	// li r6,12
	ctx.r6.s64 = 12;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lfs f2,28972(r28)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r28.u32 + 28972);
	ctx.f2.f64 = double(temp.f32);
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// bl 0x826be0b0
	ctx.lr = 0x826E4D38;
	sub_826BE0B0(ctx, base);
	// li r6,12
	ctx.r6.s64 = 12;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lfs f2,28972(r28)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r28.u32 + 28972);
	ctx.f2.f64 = double(temp.f32);
	// fmr f1,f30
	ctx.f1.f64 = ctx.f30.f64;
	// bl 0x826be0b0
	ctx.lr = 0x826E4D4C;
	sub_826BE0B0(ctx, base);
	// lwz r3,84(r1)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	// addi r1,r1,256
	ctx.r1.s64 = ctx.r1.s64 + 256;
	// lfd f30,-80(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -80);
	// lfd f31,-72(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -72);
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826E4D60:
	// lwz r11,544(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 544);
	// lwz r11,1184(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 1184);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r30,r11,24232
	ctx.r30.s64 = ctx.r11.s64 + 24232;
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// mr r7,r30
	ctx.r7.u64 = ctx.r30.u64;
	// addi r29,r11,24224
	ctx.r29.s64 = ctx.r11.s64 + 24224;
	// bne cr6,0x826e4d8c
	if (!ctx.cr6.eq) goto loc_826E4D8C;
	// mr r7,r29
	ctx.r7.u64 = ctx.r29.u64;
loc_826E4D8C:
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r6,r11,30144
	ctx.r6.s64 = ctx.r11.s64 + 30144;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r31,r11,664
	ctx.r31.s64 = ctx.r11.s64 + 664;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x828e0ab8
	ctx.lr = 0x826E4DAC;
	sub_828E0AB8(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// li r5,1
	ctx.r5.s64 = 1;
	// lwz r7,92(r1)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + 92);
	// addi r6,r11,30120
	ctx.r6.s64 = ctx.r11.s64 + 30120;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x828e0ab8
	ctx.lr = 0x826E4DC8;
	sub_828E0AB8(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// li r5,1
	ctx.r5.s64 = 1;
	// lwz r7,88(r1)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
	// addi r6,r11,30096
	ctx.r6.s64 = ctx.r11.s64 + 30096;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x828e0ab8
	ctx.lr = 0x826E4DE4;
	sub_828E0AB8(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// li r5,1
	ctx.r5.s64 = 1;
	// lwz r7,96(r1)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + 96);
	// addi r6,r11,30072
	ctx.r6.s64 = ctx.r11.s64 + 30072;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x828e0ab8
	ctx.lr = 0x826E4E00;
	sub_828E0AB8(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// lfs f1,100(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 100);
	ctx.f1.f64 = double(temp.f32);
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r6,r11,30048
	ctx.r6.s64 = ctx.r11.s64 + 30048;
	// stfd f1,48(r1)
	PPC_STORE_U64(ctx.r1.u32 + 48, ctx.f1.u64);
	// li r4,0
	ctx.r4.s64 = 0;
	// ld r7,48(r1)
	ctx.r7.u64 = PPC_LOAD_U64(ctx.r1.u32 + 48);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x828e0ab8
	ctx.lr = 0x826E4E24;
	sub_828E0AB8(ctx, base);
	// lbz r11,80(r1)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r1.u32 + 80);
	// mr r7,r30
	ctx.r7.u64 = ctx.r30.u64;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826e4e38
	if (!ctx.cr6.eq) goto loc_826E4E38;
	// mr r7,r29
	ctx.r7.u64 = ctx.r29.u64;
loc_826E4E38:
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r6,r11,30028
	ctx.r6.s64 = ctx.r11.s64 + 30028;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x828e0ab8
	ctx.lr = 0x826E4E50;
	sub_828E0AB8(ctx, base);
	// addi r3,r1,144
	ctx.r3.s64 = ctx.r1.s64 + 144;
	// lhz r4,1164(r27)
	ctx.r4.u64 = PPC_LOAD_U16(ctx.r27.u32 + 1164);
	// bl 0x826ae808
	ctx.lr = 0x826E4E5C;
	sub_826AE808(ctx, base);
	// lhz r11,1164(r27)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r27.u32 + 1164);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e4e70
	if (ctx.cr6.eq) goto loc_826E4E70;
	// addi r7,r1,144
	ctx.r7.s64 = ctx.r1.s64 + 144;
	// b 0x826e4e78
	goto loc_826E4E78;
loc_826E4E70:
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r7,r11,-15816
	ctx.r7.s64 = ctx.r11.s64 + -15816;
loc_826E4E78:
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r6,r11,29996
	ctx.r6.s64 = ctx.r11.s64 + 29996;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x828e0ab8
	ctx.lr = 0x826E4E90;
	sub_828E0AB8(ctx, base);
	// lwz r3,84(r1)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	// addi r1,r1,256
	ctx.r1.s64 = ctx.r1.s64 + 256;
	// lfd f30,-80(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -80);
	// lfd f31,-72(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -72);
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826E4EA4:
	// addi r10,r3,25
	ctx.r10.s64 = ctx.r3.s64 + 25;
	// lhz r11,1164(r27)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r27.u32 + 1164);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// stw r10,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r10.u32);
	// bne cr6,0x826e4ed8
	if (!ctx.cr6.eq) goto loc_826E4ED8;
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826E4ECC;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e4ee4
	if (ctx.cr6.eq) goto loc_826E4EE4;
loc_826E4ED8:
	// lwz r11,84(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	// addi r11,r11,36
	ctx.r11.s64 = ctx.r11.s64 + 36;
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
loc_826E4EE4:
	// lwz r3,84(r1)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	// addi r1,r1,256
	ctx.r1.s64 = ctx.r1.s64 + 256;
	// lfd f30,-80(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -80);
	// lfd f31,-72(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -72);
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E4EF8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E4EF8) { __imp__sub_826E4EF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E4EF8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// lwz r11,4976(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4976);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r7,12(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// subf r8,r10,r3
	ctx.r8.u64 = ctx.r3.u64 - ctx.r10.u64;
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// twllei r7,0
	if (ctx.r7.s32 == 0 || ctx.r7.u32 < 0u) ppc_trap(ctx, base, 0);
	// divw r10,r8,r7
	ctx.r10.s32 = (ctx.r7.s32 && !(ctx.r8.s32 == INT32_MIN && ctx.r7.s32 == -1)) ? ctx.r8.s32 / ctx.r7.s32 : 0;
	// rotlwi r8,r8,1
	ctx.r8.u64 = __builtin_rotateleft32(ctx.r8.u32, 1);
	// addi r8,r8,-1
	ctx.r8.s64 = ctx.r8.s64 + -1;
	// andc r8,r7,r8
	ctx.r8.u64 = ctx.r7.u64 & ~ctx.r8.u64;
	// twlgei r8,-1
	if (ctx.r8.s32 == -1 || ctx.r8.u32 > 4294967295u) ppc_trap(ctx, base, 0);
	// lbzx r8,r9,r10
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// ori r8,r8,128
	ctx.r8.u64 = ctx.r8.u64 | 128;
	// stbx r8,r9,r10
	PPC_STORE_U8(ctx.r9.u32 + ctx.r10.u32, ctx.r8.u8);
	// lwz r9,16(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// cmpw cr6,r10,r9
	ctx.cr6.compare<int32_t>(ctx.r10.s32, ctx.r9.s32, ctx.xer);
	// bge cr6,0x826e4f44
	if (!ctx.cr6.lt) goto loc_826E4F44;
	// stw r10,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r10.u32);
loc_826E4F44:
	// lwz r10,20(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
	// stw r10,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r10.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E4F58);  /* forward decl */
PPC_WEAK_FUNC(sub_826E4F58) { __imp__sub_826E4F58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E4F58) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// lis r10,-32251
	ctx.r10.s64 = -2113601536;
	// addi r11,r11,-16384
	ctx.r11.s64 = ctx.r11.s64 + -16384;
	// addi r10,r10,0
	ctx.r10.s64 = ctx.r10.s64 + 0;
	// clrlwi r9,r4,31
	ctx.r9.u64 = ctx.r4.u32 & 0x1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// stw r11,280(r3)
	PPC_STORE_U32(ctx.r3.u32 + 280, ctx.r11.u32);
	// stw r10,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r10.u32);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// lwz r11,4972(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4972);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r7,12(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// subf r8,r10,r3
	ctx.r8.u64 = ctx.r3.u64 - ctx.r10.u64;
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// twllei r7,0
	if (ctx.r7.s32 == 0 || ctx.r7.u32 < 0u) ppc_trap(ctx, base, 0);
	// divw r10,r8,r7
	ctx.r10.s32 = (ctx.r7.s32 && !(ctx.r8.s32 == INT32_MIN && ctx.r7.s32 == -1)) ? ctx.r8.s32 / ctx.r7.s32 : 0;
	// rotlwi r8,r8,1
	ctx.r8.u64 = __builtin_rotateleft32(ctx.r8.u32, 1);
	// addi r8,r8,-1
	ctx.r8.s64 = ctx.r8.s64 + -1;
	// andc r8,r7,r8
	ctx.r8.u64 = ctx.r7.u64 & ~ctx.r8.u64;
	// twlgei r8,-1
	if (ctx.r8.s32 == -1 || ctx.r8.u32 > 4294967295u) ppc_trap(ctx, base, 0);
	// lbzx r8,r9,r10
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// ori r8,r8,128
	ctx.r8.u64 = ctx.r8.u64 | 128;
	// stbx r8,r9,r10
	PPC_STORE_U8(ctx.r9.u32 + ctx.r10.u32, ctx.r8.u8);
	// lwz r9,16(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// cmpw cr6,r10,r9
	ctx.cr6.compare<int32_t>(ctx.r10.s32, ctx.r9.s32, ctx.xer);
	// bge cr6,0x826e4fc8
	if (!ctx.cr6.lt) goto loc_826E4FC8;
	// stw r10,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r10.u32);
loc_826E4FC8:
	// lwz r10,20(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
	// stw r10,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r10.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E4FD8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E4FD8) { __imp__sub_826E4FD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E4FD8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x826E4FE0;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r11,r11,29004
	ctx.r11.s64 = ctx.r11.s64 + 29004;
	// lwz r30,1160(r31)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r31.u32 + 1160);
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// beq cr6,0x826e5014
	if (ctx.cr6.eq) goto loc_826E5014;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8284ece8
	ctx.lr = 0x826E500C;
	sub_8284ECE8(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8218be78
	ctx.lr = 0x826E5014;
	sub_8218BE78(ctx, base);
loc_826E5014:
	// li r11,0
	ctx.r11.s64 = 0;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r11,1160(r31)
	PPC_STORE_U32(ctx.r31.u32 + 1160, ctx.r11.u32);
	// bl 0x826b4f10
	ctx.lr = 0x826E5024;
	sub_826B4F10(ctx, base);
	// clrlwi r11,r29,31
	ctx.r11.u64 = ctx.r29.u32 & 0x1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e5088
	if (ctx.cr6.eq) goto loc_826E5088;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// lwz r11,4336(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4336);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r7,12(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// subf r8,r10,r31
	ctx.r8.u64 = ctx.r31.u64 - ctx.r10.u64;
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// twllei r7,0
	if (ctx.r7.s32 == 0 || ctx.r7.u32 < 0u) ppc_trap(ctx, base, 0);
	// divw r10,r8,r7
	ctx.r10.s32 = (ctx.r7.s32 && !(ctx.r8.s32 == INT32_MIN && ctx.r7.s32 == -1)) ? ctx.r8.s32 / ctx.r7.s32 : 0;
	// rotlwi r8,r8,1
	ctx.r8.u64 = __builtin_rotateleft32(ctx.r8.u32, 1);
	// addi r8,r8,-1
	ctx.r8.s64 = ctx.r8.s64 + -1;
	// andc r8,r7,r8
	ctx.r8.u64 = ctx.r7.u64 & ~ctx.r8.u64;
	// twlgei r8,-1
	if (ctx.r8.s32 == -1 || ctx.r8.u32 > 4294967295u) ppc_trap(ctx, base, 0);
	// lbzx r8,r9,r10
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// ori r8,r8,128
	ctx.r8.u64 = ctx.r8.u64 | 128;
	// stbx r8,r9,r10
	PPC_STORE_U8(ctx.r9.u32 + ctx.r10.u32, ctx.r8.u8);
	// lwz r9,16(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// cmpw cr6,r10,r9
	ctx.cr6.compare<int32_t>(ctx.r10.s32, ctx.r9.s32, ctx.xer);
	// bge cr6,0x826e507c
	if (!ctx.cr6.lt) goto loc_826E507C;
	// stw r10,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r10.u32);
loc_826E507C:
	// lwz r10,20(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
	// stw r10,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r10.u32);
loc_826E5088:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E5098);  /* forward decl */
PPC_WEAK_FUNC(sub_826E5098) { __imp__sub_826E5098(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E5098) {
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
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// lwz r11,24(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826E50C0;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// lbz r11,14(r31)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r31.u32 + 14);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e5270
	if (ctx.cr6.eq) goto loc_826E5270;
	// lwz r11,1156(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 1156);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e51ac
	if (ctx.cr6.eq) goto loc_826E51AC;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmpwi cr6,r3,-1
	ctx.cr6.compare<int32_t>(ctx.r3.s32, -1, ctx.xer);
	// beq cr6,0x826e51ac
	if (ctx.cr6.eq) goto loc_826E51AC;
	// lis r11,-32083
	ctx.r11.s64 = -2102591488;
	// lwz r4,-17100(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + -17100);
	// bl 0x821a86b8
	ctx.lr = 0x826E50F4;
	sub_821A86B8(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e51ac
	if (ctx.cr6.eq) goto loc_826E51AC;
	// lwz r11,1156(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 1156);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x826e33d0
	ctx.lr = 0x826E5110;
	sub_826E33D0(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r11,24(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826E5124;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// addi r11,r11,-10032
	ctx.r11.s64 = ctx.r11.s64 + -10032;
	// lhz r10,46(r30)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r30.u32 + 46);
	// extsh r10,r10
	ctx.r10.s64 = ctx.r10.s16;
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e5158
	if (ctx.cr6.eq) goto loc_826E5158;
	// lbz r11,140(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 140);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// bne cr6,0x826e515c
	if (!ctx.cr6.eq) goto loc_826E515C;
loc_826E5158:
	// li r11,0
	ctx.r11.s64 = 0;
loc_826E515C:
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826e518c
	if (!ctx.cr6.eq) goto loc_826E518C;
	// lwz r11,1156(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 1156);
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x823e2390
	ctx.lr = 0x826E5178;
	sub_823E2390(ctx, base);
	// lwz r11,1156(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 1156);
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// lwz r5,12(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r4,8(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// bl 0x822e05e8
	ctx.lr = 0x826E518C;
	sub_822E05E8(ctx, base);
loc_826E518C:
	// lwz r11,1156(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 1156);
	// addi r3,r30,1264
	ctx.r3.s64 = ctx.r30.s64 + 1264;
	// lwz r4,16(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// bl 0x82411980
	ctx.lr = 0x826E519C;
	sub_82411980(ctx, base);
	// lwz r3,1156(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 1156);
	// bl 0x826e4ef8
	ctx.lr = 0x826E51A4;
	sub_826E4EF8(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,1156(r31)
	PPC_STORE_U32(ctx.r31.u32 + 1156, ctx.r11.u32);
loc_826E51AC:
	// lwz r3,1160(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 1160);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x826e5248
	if (ctx.cr6.eq) goto loc_826E5248;
	// lwz r8,32(r30)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r30.u32 + 32);
	// li r9,16
	ctx.r9.s64 = 16;
	// vspltisw v0,0
	simde_mm_store_si128((simde__m128i*)ctx.v0.u32, simde_mm_set1_epi32(int(0x0)));
	// addi r11,r3,64
	ctx.r11.s64 = ctx.r3.s64 + 64;
	// li r7,32
	ctx.r7.s64 = 32;
	// li r10,48
	ctx.r10.s64 = 48;
	// addi r4,r1,112
	ctx.r4.s64 = ctx.r1.s64 + 112;
	// lvx128 v13,r8,r9
	ea = (ctx.r8.u32 + ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r8,r1,96
	ctx.r8.s64 = ctx.r1.s64 + 96;
	// vsubfp v0,v0,v13
	ctx.fpscr.enableFlushMode();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_sub_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v13.f32)));
	// stvx128 v0,r0,r8
	ea = (ctx.r8.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r8,r1,112
	ctx.r8.s64 = ctx.r1.s64 + 112;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lfs f13,104(r1)
	ctx.fpscr.disableFlushModeUnconditional();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 104);
	ctx.f13.f64 = double(temp.f32);
	// stvx128 v0,r0,r8
	ea = (ctx.r8.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lvx128 v0,r11,r9
	ea = (ctx.r11.u32 + ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r9,r1,128
	ctx.r9.s64 = ctx.r1.s64 + 128;
	// stvx128 v0,r0,r9
	ea = (ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r9,r1,144
	ctx.r9.s64 = ctx.r1.s64 + 144;
	// lvx128 v0,r11,r7
	ea = (ctx.r11.u32 + ctx.r7.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx128 v0,r0,r9
	ea = (ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,160
	ctx.r11.s64 = ctx.r1.s64 + 160;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lwz r11,32(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 32);
	// lvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,2592(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2592);
	ctx.f0.f64 = double(temp.f32);
	// fadds f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 + ctx.f0.f64));
	// stfs f0,104(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 104, temp.u32);
	// addi r11,r1,96
	ctx.r11.s64 = ctx.r1.s64 + 96;
	// lvx128 v13,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,160
	ctx.r11.s64 = ctx.r1.s64 + 160;
	// vaddfp v0,v0,v13
	ctx.fpscr.enableFlushModeUnconditional();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_add_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v13.f32)));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x8284f020
	ctx.lr = 0x826E5248;
	sub_8284F020(ctx, base);
loc_826E5248:
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r11,284(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 284);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826E525C;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// lwz r11,36(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 36);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// rlwimi r11,r10,26,5,5
	ctx.r11.u64 = (__builtin_rotateleft32(ctx.r10.u32, 26) & 0x4000000) | (ctx.r11.u64 & 0xFFFFFFFFFBFFFFFF);
	// stw r11,36(r30)
	PPC_STORE_U32(ctx.r30.u32 + 36, ctx.r11.u32);
	// b 0x826e52cc
	goto loc_826E52CC;
loc_826E5270:
	// lhz r11,1166(r31)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r31.u32 + 1166);
	// extsh r10,r11
	ctx.r10.s64 = ctx.r11.s16;
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// ble cr6,0x826e52cc
	if (!ctx.cr6.gt) goto loc_826E52CC;
	// lis r11,-32056
	ctx.r11.s64 = -2100822016;
	// addi r11,r11,-7580
	ctx.r11.s64 = ctx.r11.s64 + -7580;
	// lfs f0,8(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// lfs f13,5908(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 5908);
	ctx.f13.f64 = double(temp.f32);
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// fmuls f0,f0,f13
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f13.f64));
	// fctidz f0,f0
	ctx.f0.s64 = (ctx.f0.f64 > double(LLONG_MAX)) ? LLONG_MAX : simde_mm_cvttsd_si64(simde_mm_load_sd(&ctx.f0.f64));
	// stfiwx f0,0,r11
	PPC_STORE_U32(ctx.r11.u32, ctx.f0.u32);
	// lwz r11,80(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// subf r11,r11,r10
	ctx.r11.u64 = ctx.r10.u64 - ctx.r11.u64;
	// extsh r11,r11
	ctx.r11.s64 = ctx.r11.s16;
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// sth r11,1166(r31)
	PPC_STORE_U16(ctx.r31.u32 + 1166, ctx.r11.u16);
	// ble cr6,0x826e52c4
	if (!ctx.cr6.gt) goto loc_826E52C4;
	// extsh r11,r11
	ctx.r11.s64 = ctx.r11.s16;
	// b 0x826e52c8
	goto loc_826E52C8;
loc_826E52C4:
	// li r11,0
	ctx.r11.s64 = 0;
loc_826E52C8:
	// sth r11,1166(r31)
	PPC_STORE_U16(ctx.r31.u32 + 1166, ctx.r11.u16);
loc_826E52CC:
	// lwz r11,544(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 544);
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// lwz r10,1368(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 1368);
	// cmpwi cr6,r10,-1
	ctx.cr6.compare<int32_t>(ctx.r10.s32, -1, ctx.xer);
	// bne cr6,0x826e52f0
	if (!ctx.cr6.eq) goto loc_826E52F0;
	// lbz r11,1214(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 1214);
	// extsb r11,r11
	ctx.r11.s64 = ctx.r11.s8;
	// addi r4,r11,31
	ctx.r4.s64 = ctx.r11.s64 + 31;
	// b 0x826e52f4
	goto loc_826E52F4;
loc_826E52F0:
	// addi r4,r10,47
	ctx.r4.s64 = ctx.r10.s64 + 47;
loc_826E52F4:
	// bl 0x822396e8
	ctx.lr = 0x826E52F8;
	sub_822396E8(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826b9da0
	ctx.lr = 0x826E5300;
	sub_826B9DA0(ctx, base);
	// lbz r11,14(r31)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r31.u32 + 14);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e5340
	if (ctx.cr6.eq) goto loc_826E5340;
	// lwz r11,544(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 544);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x826e5340
	if (ctx.cr6.eq) goto loc_826E5340;
	// lwz r11,1184(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 1184);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e5340
	if (ctx.cr6.eq) goto loc_826E5340;
	// lwz r11,36(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 36);
	// rlwinm r10,r11,0,5,5
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x4000000;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// bne cr6,0x826e5340
	if (!ctx.cr6.eq) goto loc_826E5340;
	// clrlwi r11,r11,1
	ctx.r11.u64 = ctx.r11.u32 & 0x7FFFFFFF;
	// stw r11,36(r30)
	PPC_STORE_U32(ctx.r30.u32 + 36, ctx.r11.u32);
loc_826E5340:
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
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

PPC_FUNC_IMPL(__imp__sub_826E5358);  /* forward decl */
PPC_WEAK_FUNC(sub_826E5358) { __imp__sub_826E5358(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E5358) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb10
	ctx.lr = 0x826E5360;
	__savegprlr_26(ctx, base);
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// mr r27,r4
	ctx.r27.u64 = ctx.r4.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// lwz r11,0(r29)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r29.u32 + 0);
	// lwz r11,24(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826E5380;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x826e53a0
	if (ctx.cr6.eq) goto loc_826E53A0;
	// lhz r11,46(r3)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r3.u32 + 46);
	// extsh r30,r11
	ctx.r30.s64 = ctx.r11.s16;
	// addic. r11,r3,1264
	ctx.xer.ca = ctx.r3.u32 > 4294966031;
	ctx.r11.s64 = ctx.r3.s64 + 1264;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826e53a4
	if (ctx.cr0.eq) goto loc_826E53A4;
	// lwz r7,1340(r3)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 1340);
	// b 0x826e53ac
	goto loc_826E53AC;
loc_826E53A0:
	// li r30,-1
	ctx.r30.s64 = -1;
loc_826E53A4:
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// lwz r7,-29328(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + -29328);
loc_826E53AC:
	// li r11,1
	ctx.r11.s64 = 1;
	// stw r7,88(r1)
	PPC_STORE_U32(ctx.r1.u32 + 88, ctx.r7.u32);
	// li r31,0
	ctx.r31.s64 = 0;
	// cmplwi cr6,r27,5
	ctx.cr6.compare<uint32_t>(ctx.r27.u32, 5, ctx.xer);
	// mr r26,r31
	ctx.r26.u64 = ctx.r31.u64;
	// stb r11,80(r1)
	PPC_STORE_U8(ctx.r1.u32 + 80, ctx.r11.u8);
	// bgt cr6,0x826e55b0
	if (ctx.cr6.gt) goto loc_826E55B0;
	// lis r12,-32146
	ctx.r12.s64 = -2106720256;
	// addi r12,r12,21472
	ctx.r12.s64 = ctx.r12.s64 + 21472;
	// rlwinm r0,r27,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r27.u32 | (ctx.r27.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r27.u32) {
	case 0:
		goto loc_826E53F8;
	case 1:
		goto loc_826E5514;
	case 2:
		goto loc_826E5538;
	case 3:
		goto loc_826E5568;
	case 4:
		goto loc_826E558C;
	case 5:
		goto loc_826E55AC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E53F8:
	// li r5,32
	ctx.r5.s64 = 32;
	// stw r31,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r31.u32);
	// addi r4,r1,84
	ctx.r4.s64 = ctx.r1.s64 + 84;
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// bl 0x826be4c0
	ctx.lr = 0x826E540C;
	sub_826BE4C0(ctx, base);
	// li r5,32
	ctx.r5.s64 = 32;
	// addi r4,r1,88
	ctx.r4.s64 = ctx.r1.s64 + 88;
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// bl 0x826be460
	ctx.lr = 0x826E541C;
	sub_826BE460(ctx, base);
	// lwz r3,84(r1)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	// cmpw cr6,r3,r30
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r30.s32, ctx.xer);
	// beq cr6,0x826e54d4
	if (ctx.cr6.eq) goto loc_826E54D4;
	// lis r30,-32083
	ctx.r30.s64 = -2102591488;
	// lwz r4,-17100(r30)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r30.u32 + -17100);
	// bl 0x821a86b8
	ctx.lr = 0x826E5434;
	sub_821A86B8(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826e54b4
	if (!ctx.cr6.eq) goto loc_826E54B4;
	// li r5,24
	ctx.r5.s64 = 24;
	// lwz r4,-17100(r30)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r30.u32 + -17100);
	// lwz r3,84(r1)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	// bl 0x821a8478
	ctx.lr = 0x826E5450;
	sub_821A8478(ctx, base);
	// lwz r11,1156(r29)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r29.u32 + 1156);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826e5498
	if (!ctx.cr6.eq) goto loc_826E5498;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// lwz r3,4976(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4976);
	// bl 0x821250b0
	ctx.lr = 0x826E5468;
	sub_821250B0(ctx, base);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x826e5490
	if (ctx.cr6.eq) goto loc_826E5490;
	// li r11,-1
	ctx.r11.s64 = -1;
	// stw r31,4(r3)
	PPC_STORE_U32(ctx.r3.u32 + 4, ctx.r31.u32);
	// stw r31,8(r3)
	PPC_STORE_U32(ctx.r3.u32 + 8, ctx.r31.u32);
	// stw r31,12(r3)
	PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r31.u32);
	// stw r31,16(r3)
	PPC_STORE_U32(ctx.r3.u32 + 16, ctx.r31.u32);
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r11.u32);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// b 0x826e5494
	goto loc_826E5494;
loc_826E5490:
	// mr r11,r31
	ctx.r11.u64 = ctx.r31.u64;
loc_826E5494:
	// stw r11,1156(r29)
	PPC_STORE_U32(ctx.r29.u32 + 1156, ctx.r11.u32);
loc_826E5498:
	// lwz r11,1156(r29)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r29.u32 + 1156);
	// lwz r10,84(r1)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// lwz r11,88(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
	// lwz r10,1156(r29)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r29.u32 + 1156);
	// stw r11,16(r10)
	PPC_STORE_U32(ctx.r10.u32 + 16, ctx.r11.u32);
	// b 0x826e54d4
	goto loc_826E54D4;
loc_826E54B4:
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// lwz r4,84(r1)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r1.u32 + 84);
	// bl 0x826e33d0
	ctx.lr = 0x826E54C0;
	sub_826E33D0(ctx, base);
	// lwz r3,1156(r29)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r29.u32 + 1156);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x826e54d4
	if (ctx.cr6.eq) goto loc_826E54D4;
	// bl 0x826e4ef8
	ctx.lr = 0x826E54D0;
	sub_826E4EF8(ctx, base);
	// stw r31,1156(r29)
	PPC_STORE_U32(ctx.r29.u32 + 1156, ctx.r31.u32);
loc_826E54D4:
	// lwz r11,0(r29)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r29.u32 + 0);
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// lwz r11,24(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826E54E8;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x826e55b0
	if (ctx.cr6.eq) goto loc_826E55B0;
	// lwz r11,0(r29)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r29.u32 + 0);
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// lwz r11,24(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826E5504;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// addi r3,r3,1264
	ctx.r3.s64 = ctx.r3.s64 + 1264;
	// lwz r4,88(r1)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
	// bl 0x82411980
	ctx.lr = 0x826E5510;
	sub_82411980(ctx, base);
	// b 0x826e55b0
	goto loc_826E55B0;
loc_826E5514:
	// li r5,1
	ctx.r5.s64 = 1;
	// li r4,32
	ctx.r4.s64 = 32;
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// bl 0x826bdcf8
	ctx.lr = 0x826E5524;
	sub_826BDCF8(ctx, base);
	// li r5,1
	ctx.r5.s64 = 1;
	// li r4,32
	ctx.r4.s64 = 32;
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// bl 0x826be7f0
	ctx.lr = 0x826E5534;
	sub_826BE7F0(ctx, base);
	// b 0x826e55b0
	goto loc_826E55B0;
loc_826E5538:
	// addi r6,r1,80
	ctx.r6.s64 = ctx.r1.s64 + 80;
	// stb r31,80(r1)
	PPC_STORE_U8(ctx.r1.u32 + 80, ctx.r31.u8);
	// li r5,32
	ctx.r5.s64 = 32;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// bl 0x826beb58
	ctx.lr = 0x826E5550;
	sub_826BEB58(ctx, base);
	// addi r6,r1,80
	ctx.r6.s64 = ctx.r1.s64 + 80;
	// li r5,32
	ctx.r5.s64 = 32;
	// lwz r4,88(r1)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// bl 0x826beaa0
	ctx.lr = 0x826E5564;
	sub_826BEAA0(ctx, base);
	// b 0x826e55b0
	goto loc_826E55B0;
loc_826E5568:
	// li r5,32
	ctx.r5.s64 = 32;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// bl 0x826be0a8
	ctx.lr = 0x826E5578;
	sub_826BE0A8(ctx, base);
	// li r5,32
	ctx.r5.s64 = 32;
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// lwz r4,88(r1)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
	// bl 0x826be088
	ctx.lr = 0x826E5588;
	sub_826BE088(ctx, base);
	// b 0x826e55b0
	goto loc_826E55B0;
loc_826E558C:
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r6,r11,30184
	ctx.r6.s64 = ctx.r11.s64 + 30184;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,664
	ctx.r3.s64 = ctx.r11.s64 + 664;
	// bl 0x828e0ab8
	ctx.lr = 0x826E55A8;
	sub_828E0AB8(ctx, base);
	// b 0x826e55b0
	goto loc_826E55B0;
loc_826E55AC:
	// li r26,64
	ctx.r26.s64 = 64;
loc_826E55B0:
	// mr r31,r27
	ctx.r31.u64 = ctx.r27.u64;
	// cmpwi cr6,r27,0
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 0, ctx.xer);
	// bne cr6,0x826e55f8
	if (!ctx.cr6.eq) goto loc_826E55F8;
	// lwz r11,0(r29)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r29.u32 + 0);
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// lwz r11,24(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826E55D0;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// lhz r10,46(r3)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r3.u32 + 46);
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// extsh r10,r10
	ctx.r10.s64 = ctx.r10.s16;
	// addi r11,r11,-10032
	ctx.r11.s64 = ctx.r11.s64 + -10032;
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lbz r11,140(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 140);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e55f8
	if (ctx.cr6.eq) goto loc_826E55F8;
	// li r31,1
	ctx.r31.s64 = 1;
loc_826E55F8:
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// lwz r6,1156(r29)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r29.u32 + 1156);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826b3c10
	ctx.lr = 0x826E560C;
	sub_826B3C10(ctx, base);
	// cmpwi cr6,r27,0
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 0, ctx.xer);
	// beq cr6,0x826e5650
	if (ctx.cr6.eq) goto loc_826E5650;
	// cmpwi cr6,r27,2
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 2, ctx.xer);
	// beq cr6,0x826e5630
	if (ctx.cr6.eq) goto loc_826E5630;
	// cmpwi cr6,r27,5
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 5, ctx.xer);
	// bne cr6,0x826e5670
	if (!ctx.cr6.eq) goto loc_826E5670;
	// add r3,r3,r26
	ctx.r3.u64 = ctx.r3.u64 + ctx.r26.u64;
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
loc_826E5630:
	// cntlzw r11,r3
	ctx.r11.u64 = ctx.r3.u32 == 0 ? 32 : __builtin_clz(ctx.r3.u32);
	// lbz r10,80(r1)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r1.u32 + 80);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// or r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 | ctx.r10.u64;
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
loc_826E5650:
	// cntlzw r11,r3
	ctx.r11.u64 = ctx.r3.u32 == 0 ? 32 : __builtin_clz(ctx.r3.u32);
	// lbz r10,80(r1)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r1.u32 + 80);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// and r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 & ctx.r10.u64;
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
loc_826E5670:
	// lbz r11,80(r1)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r1.u32 + 80);
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E5680);  /* forward decl */
PPC_WEAK_FUNC(sub_826E5680) { __imp__sub_826E5680(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E5680) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r11,r11,30608
	ctx.r11.s64 = ctx.r11.s64 + 30608;
	// clrlwi r10,r4,31
	ctx.r10.u64 = ctx.r4.u32 & 0x1;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// beq cr6,0x826e56b4
	if (ctx.cr6.eq) goto loc_826E56B4;
	// bl 0x8218be78
	ctx.lr = 0x826E56B0;
	sub_8218BE78(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
loc_826E56B4:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E56C8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E56C8) { __imp__sub_826E56C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E56C8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r11,r11,30628
	ctx.r11.s64 = ctx.r11.s64 + 30628;
	// clrlwi r10,r4,31
	ctx.r10.u64 = ctx.r4.u32 & 0x1;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// beq cr6,0x826e56fc
	if (ctx.cr6.eq) goto loc_826E56FC;
	// bl 0x8218be78
	ctx.lr = 0x826E56F8;
	sub_8218BE78(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
loc_826E56FC:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E5710);  /* forward decl */
PPC_WEAK_FUNC(sub_826E5710) { __imp__sub_826E5710(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E5710) {
	PPC_FUNC_PROLOGUE();
	// lis r9,4096
	ctx.r9.s64 = 268435456;
	// li r11,19
	ctx.r11.s64 = 19;
	// ori r9,r9,38
	ctx.r9.u64 = ctx.r9.u64 | 38;
	// li r10,0
	ctx.r10.s64 = 0;
	// lis r8,4096
	ctx.r8.s64 = 268435456;
	// lis r7,8192
	ctx.r7.s64 = 536870912;
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r11.u32);
	// lis r6,8192
	ctx.r6.s64 = 536870912;
	// stw r9,12(r3)
	PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r9.u32);
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// std r10,4(r3)
	PPC_STORE_U64(ctx.r3.u32 + 4, ctx.r10.u64);
	// lis r9,4096
	ctx.r9.s64 = 268435456;
	// lis r10,4096
	ctx.r10.s64 = 268435456;
	// ori r8,r8,39
	ctx.r8.u64 = ctx.r8.u64 | 39;
	// ori r7,r7,44
	ctx.r7.u64 = ctx.r7.u64 | 44;
	// ori r6,r6,13
	ctx.r6.u64 = ctx.r6.u64 | 13;
	// ori r11,r11,14
	ctx.r11.u64 = ctx.r11.u64 | 14;
	// ori r10,r10,15
	ctx.r10.u64 = ctx.r10.u64 | 15;
	// ori r9,r9,65
	ctx.r9.u64 = ctx.r9.u64 | 65;
	// stw r8,20(r3)
	PPC_STORE_U32(ctx.r3.u32 + 20, ctx.r8.u32);
	// stw r7,28(r3)
	PPC_STORE_U32(ctx.r3.u32 + 28, ctx.r7.u32);
	// lis r8,4096
	ctx.r8.s64 = 268435456;
	// stw r6,40(r3)
	PPC_STORE_U32(ctx.r3.u32 + 40, ctx.r6.u32);
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// stw r11,52(r3)
	PPC_STORE_U32(ctx.r3.u32 + 52, ctx.r11.u32);
	// lis r6,4096
	ctx.r6.s64 = 268435456;
	// stw r10,60(r3)
	PPC_STORE_U32(ctx.r3.u32 + 60, ctx.r10.u32);
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// stw r9,68(r3)
	PPC_STORE_U32(ctx.r3.u32 + 68, ctx.r9.u32);
	// lis r10,4096
	ctx.r10.s64 = 268435456;
	// lis r9,4096
	ctx.r9.s64 = 268435456;
	// ori r8,r8,66
	ctx.r8.u64 = ctx.r8.u64 | 66;
	// ori r7,r7,67
	ctx.r7.u64 = ctx.r7.u64 | 67;
	// ori r6,r6,68
	ctx.r6.u64 = ctx.r6.u64 | 68;
	// ori r11,r11,69
	ctx.r11.u64 = ctx.r11.u64 | 69;
	// ori r10,r10,70
	ctx.r10.u64 = ctx.r10.u64 | 70;
	// ori r9,r9,75
	ctx.r9.u64 = ctx.r9.u64 | 75;
	// stw r8,76(r3)
	PPC_STORE_U32(ctx.r3.u32 + 76, ctx.r8.u32);
	// stw r7,84(r3)
	PPC_STORE_U32(ctx.r3.u32 + 84, ctx.r7.u32);
	// lis r8,8192
	ctx.r8.s64 = 536870912;
	// stw r6,92(r3)
	PPC_STORE_U32(ctx.r3.u32 + 92, ctx.r6.u32);
	// lis r7,8192
	ctx.r7.s64 = 536870912;
	// stw r11,100(r3)
	PPC_STORE_U32(ctx.r3.u32 + 100, ctx.r11.u32);
	// lis r6,8192
	ctx.r6.s64 = 536870912;
	// stw r10,108(r3)
	PPC_STORE_U32(ctx.r3.u32 + 108, ctx.r10.u32);
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// stw r9,116(r3)
	PPC_STORE_U32(ctx.r3.u32 + 116, ctx.r9.u32);
	// lis r10,8192
	ctx.r10.s64 = 536870912;
	// lis r9,8192
	ctx.r9.s64 = 536870912;
	// ori r8,r8,76
	ctx.r8.u64 = ctx.r8.u64 | 76;
	// ori r7,r7,77
	ctx.r7.u64 = ctx.r7.u64 | 77;
	// ori r6,r6,78
	ctx.r6.u64 = ctx.r6.u64 | 78;
	// ori r11,r11,79
	ctx.r11.u64 = ctx.r11.u64 | 79;
	// ori r10,r10,80
	ctx.r10.u64 = ctx.r10.u64 | 80;
	// ori r9,r9,55
	ctx.r9.u64 = ctx.r9.u64 | 55;
	// stw r8,124(r3)
	PPC_STORE_U32(ctx.r3.u32 + 124, ctx.r8.u32);
	// stw r7,136(r3)
	PPC_STORE_U32(ctx.r3.u32 + 136, ctx.r7.u32);
	// stw r6,148(r3)
	PPC_STORE_U32(ctx.r3.u32 + 148, ctx.r6.u32);
	// stw r11,160(r3)
	PPC_STORE_U32(ctx.r3.u32 + 160, ctx.r11.u32);
	// stw r10,172(r3)
	PPC_STORE_U32(ctx.r3.u32 + 172, ctx.r10.u32);
	// stw r9,184(r3)
	PPC_STORE_U32(ctx.r3.u32 + 184, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E5808);  /* forward decl */
PPC_WEAK_FUNC(sub_826E5808) { __imp__sub_826E5808(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E5808) {
	PPC_FUNC_PROLOGUE();
	// lis r9,4096
	ctx.r9.s64 = 268435456;
	// li r11,26
	ctx.r11.s64 = 26;
	// ori r9,r9,38
	ctx.r9.u64 = ctx.r9.u64 | 38;
	// li r10,0
	ctx.r10.s64 = 0;
	// lis r8,4096
	ctx.r8.s64 = 268435456;
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// ori r8,r8,39
	ctx.r8.u64 = ctx.r8.u64 | 39;
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r11.u32);
	// stw r9,12(r3)
	PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r9.u32);
	// lis r6,8192
	ctx.r6.s64 = 536870912;
	// std r10,4(r3)
	PPC_STORE_U64(ctx.r3.u32 + 4, ctx.r10.u64);
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// lis r10,4096
	ctx.r10.s64 = 268435456;
	// lis r9,4096
	ctx.r9.s64 = 268435456;
	// ori r7,r7,41
	ctx.r7.u64 = ctx.r7.u64 | 41;
	// stw r8,20(r3)
	PPC_STORE_U32(ctx.r3.u32 + 20, ctx.r8.u32);
	// ori r6,r6,13
	ctx.r6.u64 = ctx.r6.u64 | 13;
	// ori r11,r11,29
	ctx.r11.u64 = ctx.r11.u64 | 29;
	// ori r10,r10,65
	ctx.r10.u64 = ctx.r10.u64 | 65;
	// ori r9,r9,66
	ctx.r9.u64 = ctx.r9.u64 | 66;
	// lis r8,4096
	ctx.r8.s64 = 268435456;
	// stw r7,28(r3)
	PPC_STORE_U32(ctx.r3.u32 + 28, ctx.r7.u32);
	// stw r6,36(r3)
	PPC_STORE_U32(ctx.r3.u32 + 36, ctx.r6.u32);
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// ori r8,r8,67
	ctx.r8.u64 = ctx.r8.u64 | 67;
	// stw r11,48(r3)
	PPC_STORE_U32(ctx.r3.u32 + 48, ctx.r11.u32);
	// stw r10,60(r3)
	PPC_STORE_U32(ctx.r3.u32 + 60, ctx.r10.u32);
	// lis r6,4096
	ctx.r6.s64 = 268435456;
	// stw r9,68(r3)
	PPC_STORE_U32(ctx.r3.u32 + 68, ctx.r9.u32);
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// lis r10,4096
	ctx.r10.s64 = 268435456;
	// lis r9,4096
	ctx.r9.s64 = 268435456;
	// ori r7,r7,68
	ctx.r7.u64 = ctx.r7.u64 | 68;
	// stw r8,76(r3)
	PPC_STORE_U32(ctx.r3.u32 + 76, ctx.r8.u32);
	// ori r6,r6,69
	ctx.r6.u64 = ctx.r6.u64 | 69;
	// ori r11,r11,70
	ctx.r11.u64 = ctx.r11.u64 | 70;
	// ori r10,r10,71
	ctx.r10.u64 = ctx.r10.u64 | 71;
	// ori r9,r9,72
	ctx.r9.u64 = ctx.r9.u64 | 72;
	// lis r8,8192
	ctx.r8.s64 = 536870912;
	// stw r7,84(r3)
	PPC_STORE_U32(ctx.r3.u32 + 84, ctx.r7.u32);
	// stw r6,92(r3)
	PPC_STORE_U32(ctx.r3.u32 + 92, ctx.r6.u32);
	// lis r7,8192
	ctx.r7.s64 = 536870912;
	// ori r8,r8,76
	ctx.r8.u64 = ctx.r8.u64 | 76;
	// stw r11,100(r3)
	PPC_STORE_U32(ctx.r3.u32 + 100, ctx.r11.u32);
	// stw r10,108(r3)
	PPC_STORE_U32(ctx.r3.u32 + 108, ctx.r10.u32);
	// lis r6,8192
	ctx.r6.s64 = 536870912;
	// stw r9,116(r3)
	PPC_STORE_U32(ctx.r3.u32 + 116, ctx.r9.u32);
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// lis r10,8192
	ctx.r10.s64 = 536870912;
	// lis r9,4096
	ctx.r9.s64 = 268435456;
	// ori r7,r7,77
	ctx.r7.u64 = ctx.r7.u64 | 77;
	// stw r8,124(r3)
	PPC_STORE_U32(ctx.r3.u32 + 124, ctx.r8.u32);
	// ori r6,r6,78
	ctx.r6.u64 = ctx.r6.u64 | 78;
	// ori r11,r11,79
	ctx.r11.u64 = ctx.r11.u64 | 79;
	// ori r10,r10,80
	ctx.r10.u64 = ctx.r10.u64 | 80;
	// ori r9,r9,81
	ctx.r9.u64 = ctx.r9.u64 | 81;
	// lis r8,4096
	ctx.r8.s64 = 268435456;
	// stw r7,136(r3)
	PPC_STORE_U32(ctx.r3.u32 + 136, ctx.r7.u32);
	// stw r6,148(r3)
	PPC_STORE_U32(ctx.r3.u32 + 148, ctx.r6.u32);
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// ori r8,r8,14
	ctx.r8.u64 = ctx.r8.u64 | 14;
	// stw r11,160(r3)
	PPC_STORE_U32(ctx.r3.u32 + 160, ctx.r11.u32);
	// stw r10,172(r3)
	PPC_STORE_U32(ctx.r3.u32 + 172, ctx.r10.u32);
	// lis r6,4096
	ctx.r6.s64 = 268435456;
	// stw r9,184(r3)
	PPC_STORE_U32(ctx.r3.u32 + 184, ctx.r9.u32);
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// lis r10,4096
	ctx.r10.s64 = 268435456;
	// lis r9,4096
	ctx.r9.s64 = 268435456;
	// ori r7,r7,18
	ctx.r7.u64 = ctx.r7.u64 | 18;
	// stw r8,192(r3)
	PPC_STORE_U32(ctx.r3.u32 + 192, ctx.r8.u32);
	// ori r6,r6,15
	ctx.r6.u64 = ctx.r6.u64 | 15;
	// ori r11,r11,17
	ctx.r11.u64 = ctx.r11.u64 | 17;
	// ori r10,r10,33
	ctx.r10.u64 = ctx.r10.u64 | 33;
	// ori r9,r9,34
	ctx.r9.u64 = ctx.r9.u64 | 34;
	// lis r8,8192
	ctx.r8.s64 = 536870912;
	// stw r7,200(r3)
	PPC_STORE_U32(ctx.r3.u32 + 200, ctx.r7.u32);
	// stw r6,208(r3)
	PPC_STORE_U32(ctx.r3.u32 + 208, ctx.r6.u32);
	// ori r8,r8,55
	ctx.r8.u64 = ctx.r8.u64 | 55;
	// stw r11,216(r3)
	PPC_STORE_U32(ctx.r3.u32 + 216, ctx.r11.u32);
	// stw r10,224(r3)
	PPC_STORE_U32(ctx.r3.u32 + 224, ctx.r10.u32);
	// stw r9,232(r3)
	PPC_STORE_U32(ctx.r3.u32 + 232, ctx.r9.u32);
	// stw r8,240(r3)
	PPC_STORE_U32(ctx.r3.u32 + 240, ctx.r8.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E5958);  /* forward decl */
PPC_WEAK_FUNC(sub_826E5958) { __imp__sub_826E5958(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E5958) {
	PPC_FUNC_PROLOGUE();
	// lis r9,8192
	ctx.r9.s64 = 536870912;
	// li r11,25
	ctx.r11.s64 = 25;
	// ori r9,r9,42
	ctx.r9.u64 = ctx.r9.u64 | 42;
	// li r10,0
	ctx.r10.s64 = 0;
	// lis r8,8192
	ctx.r8.s64 = 536870912;
	// lis r7,8192
	ctx.r7.s64 = 536870912;
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r11.u32);
	// lis r6,4096
	ctx.r6.s64 = 268435456;
	// stw r9,12(r3)
	PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r9.u32);
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// std r10,4(r3)
	PPC_STORE_U64(ctx.r3.u32 + 4, ctx.r10.u64);
	// lis r9,4096
	ctx.r9.s64 = 268435456;
	// lis r10,4096
	ctx.r10.s64 = 268435456;
	// ori r8,r8,13
	ctx.r8.u64 = ctx.r8.u64 | 13;
	// ori r7,r7,29
	ctx.r7.u64 = ctx.r7.u64 | 29;
	// ori r6,r6,65
	ctx.r6.u64 = ctx.r6.u64 | 65;
	// ori r11,r11,66
	ctx.r11.u64 = ctx.r11.u64 | 66;
	// ori r10,r10,67
	ctx.r10.u64 = ctx.r10.u64 | 67;
	// ori r9,r9,68
	ctx.r9.u64 = ctx.r9.u64 | 68;
	// stw r8,24(r3)
	PPC_STORE_U32(ctx.r3.u32 + 24, ctx.r8.u32);
	// stw r7,36(r3)
	PPC_STORE_U32(ctx.r3.u32 + 36, ctx.r7.u32);
	// lis r8,4096
	ctx.r8.s64 = 268435456;
	// stw r6,48(r3)
	PPC_STORE_U32(ctx.r3.u32 + 48, ctx.r6.u32);
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// stw r11,56(r3)
	PPC_STORE_U32(ctx.r3.u32 + 56, ctx.r11.u32);
	// lis r6,4096
	ctx.r6.s64 = 268435456;
	// stw r10,64(r3)
	PPC_STORE_U32(ctx.r3.u32 + 64, ctx.r10.u32);
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// stw r9,72(r3)
	PPC_STORE_U32(ctx.r3.u32 + 72, ctx.r9.u32);
	// lis r10,4096
	ctx.r10.s64 = 268435456;
	// lis r9,8192
	ctx.r9.s64 = 536870912;
	// ori r8,r8,69
	ctx.r8.u64 = ctx.r8.u64 | 69;
	// ori r7,r7,70
	ctx.r7.u64 = ctx.r7.u64 | 70;
	// ori r6,r6,71
	ctx.r6.u64 = ctx.r6.u64 | 71;
	// ori r11,r11,72
	ctx.r11.u64 = ctx.r11.u64 | 72;
	// ori r10,r10,73
	ctx.r10.u64 = ctx.r10.u64 | 73;
	// ori r9,r9,76
	ctx.r9.u64 = ctx.r9.u64 | 76;
	// stw r8,80(r3)
	PPC_STORE_U32(ctx.r3.u32 + 80, ctx.r8.u32);
	// stw r7,88(r3)
	PPC_STORE_U32(ctx.r3.u32 + 88, ctx.r7.u32);
	// lis r8,8192
	ctx.r8.s64 = 536870912;
	// stw r6,96(r3)
	PPC_STORE_U32(ctx.r3.u32 + 96, ctx.r6.u32);
	// lis r7,8192
	ctx.r7.s64 = 536870912;
	// stw r11,104(r3)
	PPC_STORE_U32(ctx.r3.u32 + 104, ctx.r11.u32);
	// lis r6,8192
	ctx.r6.s64 = 536870912;
	// stw r10,112(r3)
	PPC_STORE_U32(ctx.r3.u32 + 112, ctx.r10.u32);
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// stw r9,120(r3)
	PPC_STORE_U32(ctx.r3.u32 + 120, ctx.r9.u32);
	// lis r10,4096
	ctx.r10.s64 = 268435456;
	// lis r9,4096
	ctx.r9.s64 = 268435456;
	// ori r8,r8,77
	ctx.r8.u64 = ctx.r8.u64 | 77;
	// ori r7,r7,78
	ctx.r7.u64 = ctx.r7.u64 | 78;
	// ori r6,r6,79
	ctx.r6.u64 = ctx.r6.u64 | 79;
	// ori r11,r11,80
	ctx.r11.u64 = ctx.r11.u64 | 80;
	// ori r10,r10,81
	ctx.r10.u64 = ctx.r10.u64 | 81;
	// ori r9,r9,14
	ctx.r9.u64 = ctx.r9.u64 | 14;
	// stw r8,132(r3)
	PPC_STORE_U32(ctx.r3.u32 + 132, ctx.r8.u32);
	// stw r7,144(r3)
	PPC_STORE_U32(ctx.r3.u32 + 144, ctx.r7.u32);
	// lis r8,4096
	ctx.r8.s64 = 268435456;
	// stw r6,156(r3)
	PPC_STORE_U32(ctx.r3.u32 + 156, ctx.r6.u32);
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// stw r11,168(r3)
	PPC_STORE_U32(ctx.r3.u32 + 168, ctx.r11.u32);
	// lis r6,4096
	ctx.r6.s64 = 268435456;
	// stw r10,180(r3)
	PPC_STORE_U32(ctx.r3.u32 + 180, ctx.r10.u32);
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// stw r9,188(r3)
	PPC_STORE_U32(ctx.r3.u32 + 188, ctx.r9.u32);
	// lis r10,4096
	ctx.r10.s64 = 268435456;
	// lis r9,8192
	ctx.r9.s64 = 536870912;
	// ori r8,r8,18
	ctx.r8.u64 = ctx.r8.u64 | 18;
	// ori r7,r7,15
	ctx.r7.u64 = ctx.r7.u64 | 15;
	// ori r6,r6,17
	ctx.r6.u64 = ctx.r6.u64 | 17;
	// ori r11,r11,33
	ctx.r11.u64 = ctx.r11.u64 | 33;
	// ori r10,r10,34
	ctx.r10.u64 = ctx.r10.u64 | 34;
	// ori r9,r9,55
	ctx.r9.u64 = ctx.r9.u64 | 55;
	// stw r8,196(r3)
	PPC_STORE_U32(ctx.r3.u32 + 196, ctx.r8.u32);
	// stw r7,204(r3)
	PPC_STORE_U32(ctx.r3.u32 + 204, ctx.r7.u32);
	// stw r6,212(r3)
	PPC_STORE_U32(ctx.r3.u32 + 212, ctx.r6.u32);
	// stw r11,220(r3)
	PPC_STORE_U32(ctx.r3.u32 + 220, ctx.r11.u32);
	// stw r10,228(r3)
	PPC_STORE_U32(ctx.r3.u32 + 228, ctx.r10.u32);
	// stw r9,236(r3)
	PPC_STORE_U32(ctx.r3.u32 + 236, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E5A98);  /* forward decl */
PPC_WEAK_FUNC(sub_826E5A98) { __imp__sub_826E5A98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E5A98) {
	PPC_FUNC_PROLOGUE();
	// li r11,23
	ctx.r11.s64 = 23;
	// lis r9,8192
	ctx.r9.s64 = 536870912;
	// lis r8,8192
	ctx.r8.s64 = 536870912;
	// lis r7,8192
	ctx.r7.s64 = 536870912;
	// lis r6,4096
	ctx.r6.s64 = 268435456;
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r11.u32);
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r9,r9,42
	ctx.r9.u64 = ctx.r9.u64 | 42;
	// ori r8,r8,13
	ctx.r8.u64 = ctx.r8.u64 | 13;
	// ori r7,r7,29
	ctx.r7.u64 = ctx.r7.u64 | 29;
	// ori r6,r6,65
	ctx.r6.u64 = ctx.r6.u64 | 65;
	// ori r11,r11,66
	ctx.r11.u64 = ctx.r11.u64 | 66;
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r9,12(r3)
	PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r9.u32);
	// stw r8,24(r3)
	PPC_STORE_U32(ctx.r3.u32 + 24, ctx.r8.u32);
	// lis r9,4096
	ctx.r9.s64 = 268435456;
	// stw r7,36(r3)
	PPC_STORE_U32(ctx.r3.u32 + 36, ctx.r7.u32);
	// lis r8,4096
	ctx.r8.s64 = 268435456;
	// stw r6,48(r3)
	PPC_STORE_U32(ctx.r3.u32 + 48, ctx.r6.u32);
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// stw r11,56(r3)
	PPC_STORE_U32(ctx.r3.u32 + 56, ctx.r11.u32);
	// lis r6,4096
	ctx.r6.s64 = 268435456;
	// std r10,4(r3)
	PPC_STORE_U64(ctx.r3.u32 + 4, ctx.r10.u64);
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// lis r10,4096
	ctx.r10.s64 = 268435456;
	// ori r9,r9,68
	ctx.r9.u64 = ctx.r9.u64 | 68;
	// ori r10,r10,67
	ctx.r10.u64 = ctx.r10.u64 | 67;
	// ori r8,r8,69
	ctx.r8.u64 = ctx.r8.u64 | 69;
	// ori r7,r7,70
	ctx.r7.u64 = ctx.r7.u64 | 70;
	// ori r6,r6,71
	ctx.r6.u64 = ctx.r6.u64 | 71;
	// ori r11,r11,76
	ctx.r11.u64 = ctx.r11.u64 | 76;
	// stw r9,72(r3)
	PPC_STORE_U32(ctx.r3.u32 + 72, ctx.r9.u32);
	// stw r10,64(r3)
	PPC_STORE_U32(ctx.r3.u32 + 64, ctx.r10.u32);
	// lis r9,8192
	ctx.r9.s64 = 536870912;
	// stw r8,80(r3)
	PPC_STORE_U32(ctx.r3.u32 + 80, ctx.r8.u32);
	// lis r10,8192
	ctx.r10.s64 = 536870912;
	// stw r7,88(r3)
	PPC_STORE_U32(ctx.r3.u32 + 88, ctx.r7.u32);
	// lis r8,8192
	ctx.r8.s64 = 536870912;
	// stw r6,96(r3)
	PPC_STORE_U32(ctx.r3.u32 + 96, ctx.r6.u32);
	// lis r7,8192
	ctx.r7.s64 = 536870912;
	// stw r11,104(r3)
	PPC_STORE_U32(ctx.r3.u32 + 104, ctx.r11.u32);
	// lis r6,4096
	ctx.r6.s64 = 268435456;
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r10,r10,77
	ctx.r10.u64 = ctx.r10.u64 | 77;
	// ori r9,r9,78
	ctx.r9.u64 = ctx.r9.u64 | 78;
	// ori r8,r8,79
	ctx.r8.u64 = ctx.r8.u64 | 79;
	// ori r7,r7,80
	ctx.r7.u64 = ctx.r7.u64 | 80;
	// ori r6,r6,81
	ctx.r6.u64 = ctx.r6.u64 | 81;
	// ori r11,r11,14
	ctx.r11.u64 = ctx.r11.u64 | 14;
	// stw r10,116(r3)
	PPC_STORE_U32(ctx.r3.u32 + 116, ctx.r10.u32);
	// stw r9,128(r3)
	PPC_STORE_U32(ctx.r3.u32 + 128, ctx.r9.u32);
	// lis r10,4096
	ctx.r10.s64 = 268435456;
	// stw r8,140(r3)
	PPC_STORE_U32(ctx.r3.u32 + 140, ctx.r8.u32);
	// lis r9,4096
	ctx.r9.s64 = 268435456;
	// stw r7,152(r3)
	PPC_STORE_U32(ctx.r3.u32 + 152, ctx.r7.u32);
	// lis r8,4096
	ctx.r8.s64 = 268435456;
	// stw r6,164(r3)
	PPC_STORE_U32(ctx.r3.u32 + 164, ctx.r6.u32);
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// stw r11,172(r3)
	PPC_STORE_U32(ctx.r3.u32 + 172, ctx.r11.u32);
	// lis r6,4096
	ctx.r6.s64 = 268435456;
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// ori r10,r10,18
	ctx.r10.u64 = ctx.r10.u64 | 18;
	// ori r9,r9,15
	ctx.r9.u64 = ctx.r9.u64 | 15;
	// ori r8,r8,17
	ctx.r8.u64 = ctx.r8.u64 | 17;
	// ori r7,r7,33
	ctx.r7.u64 = ctx.r7.u64 | 33;
	// ori r6,r6,34
	ctx.r6.u64 = ctx.r6.u64 | 34;
	// ori r11,r11,55
	ctx.r11.u64 = ctx.r11.u64 | 55;
	// stw r10,180(r3)
	PPC_STORE_U32(ctx.r3.u32 + 180, ctx.r10.u32);
	// stw r9,188(r3)
	PPC_STORE_U32(ctx.r3.u32 + 188, ctx.r9.u32);
	// stw r8,196(r3)
	PPC_STORE_U32(ctx.r3.u32 + 196, ctx.r8.u32);
	// stw r7,204(r3)
	PPC_STORE_U32(ctx.r3.u32 + 204, ctx.r7.u32);
	// stw r6,212(r3)
	PPC_STORE_U32(ctx.r3.u32 + 212, ctx.r6.u32);
	// stw r11,220(r3)
	PPC_STORE_U32(ctx.r3.u32 + 220, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E5BC0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E5BC0) { __imp__sub_826E5BC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E5BC0) {
	PPC_FUNC_PROLOGUE();
	// li r11,24
	ctx.r11.s64 = 24;
	// li r10,0
	ctx.r10.s64 = 0;
	// lis r9,8192
	ctx.r9.s64 = 536870912;
	// lis r8,8192
	ctx.r8.s64 = 536870912;
	// lis r7,8192
	ctx.r7.s64 = 536870912;
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r11.u32);
	// lis r6,4096
	ctx.r6.s64 = 268435456;
	// std r10,4(r3)
	PPC_STORE_U64(ctx.r3.u32 + 4, ctx.r10.u64);
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// lis r10,4096
	ctx.r10.s64 = 268435456;
	// ori r9,r9,42
	ctx.r9.u64 = ctx.r9.u64 | 42;
	// ori r8,r8,13
	ctx.r8.u64 = ctx.r8.u64 | 13;
	// ori r7,r7,29
	ctx.r7.u64 = ctx.r7.u64 | 29;
	// ori r6,r6,65
	ctx.r6.u64 = ctx.r6.u64 | 65;
	// ori r11,r11,66
	ctx.r11.u64 = ctx.r11.u64 | 66;
	// ori r10,r10,67
	ctx.r10.u64 = ctx.r10.u64 | 67;
	// stw r9,12(r3)
	PPC_STORE_U32(ctx.r3.u32 + 12, ctx.r9.u32);
	// stw r8,24(r3)
	PPC_STORE_U32(ctx.r3.u32 + 24, ctx.r8.u32);
	// lis r9,4096
	ctx.r9.s64 = 268435456;
	// stw r7,36(r3)
	PPC_STORE_U32(ctx.r3.u32 + 36, ctx.r7.u32);
	// lis r8,4096
	ctx.r8.s64 = 268435456;
	// stw r6,48(r3)
	PPC_STORE_U32(ctx.r3.u32 + 48, ctx.r6.u32);
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// stw r11,56(r3)
	PPC_STORE_U32(ctx.r3.u32 + 56, ctx.r11.u32);
	// lis r6,4096
	ctx.r6.s64 = 268435456;
	// stw r10,64(r3)
	PPC_STORE_U32(ctx.r3.u32 + 64, ctx.r10.u32);
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// lis r10,8192
	ctx.r10.s64 = 536870912;
	// ori r9,r9,68
	ctx.r9.u64 = ctx.r9.u64 | 68;
	// ori r8,r8,69
	ctx.r8.u64 = ctx.r8.u64 | 69;
	// ori r7,r7,70
	ctx.r7.u64 = ctx.r7.u64 | 70;
	// ori r6,r6,71
	ctx.r6.u64 = ctx.r6.u64 | 71;
	// ori r11,r11,72
	ctx.r11.u64 = ctx.r11.u64 | 72;
	// ori r10,r10,76
	ctx.r10.u64 = ctx.r10.u64 | 76;
	// stw r9,72(r3)
	PPC_STORE_U32(ctx.r3.u32 + 72, ctx.r9.u32);
	// stw r8,80(r3)
	PPC_STORE_U32(ctx.r3.u32 + 80, ctx.r8.u32);
	// lis r9,8192
	ctx.r9.s64 = 536870912;
	// stw r7,88(r3)
	PPC_STORE_U32(ctx.r3.u32 + 88, ctx.r7.u32);
	// lis r8,8192
	ctx.r8.s64 = 536870912;
	// stw r6,96(r3)
	PPC_STORE_U32(ctx.r3.u32 + 96, ctx.r6.u32);
	// lis r7,8192
	ctx.r7.s64 = 536870912;
	// stw r11,104(r3)
	PPC_STORE_U32(ctx.r3.u32 + 104, ctx.r11.u32);
	// lis r6,8192
	ctx.r6.s64 = 536870912;
	// stw r10,112(r3)
	PPC_STORE_U32(ctx.r3.u32 + 112, ctx.r10.u32);
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// lis r10,4096
	ctx.r10.s64 = 268435456;
	// ori r9,r9,77
	ctx.r9.u64 = ctx.r9.u64 | 77;
	// ori r8,r8,78
	ctx.r8.u64 = ctx.r8.u64 | 78;
	// ori r7,r7,79
	ctx.r7.u64 = ctx.r7.u64 | 79;
	// ori r6,r6,80
	ctx.r6.u64 = ctx.r6.u64 | 80;
	// ori r11,r11,81
	ctx.r11.u64 = ctx.r11.u64 | 81;
	// ori r10,r10,14
	ctx.r10.u64 = ctx.r10.u64 | 14;
	// stw r9,124(r3)
	PPC_STORE_U32(ctx.r3.u32 + 124, ctx.r9.u32);
	// stw r8,136(r3)
	PPC_STORE_U32(ctx.r3.u32 + 136, ctx.r8.u32);
	// lis r9,4096
	ctx.r9.s64 = 268435456;
	// stw r7,148(r3)
	PPC_STORE_U32(ctx.r3.u32 + 148, ctx.r7.u32);
	// lis r8,4096
	ctx.r8.s64 = 268435456;
	// stw r6,160(r3)
	PPC_STORE_U32(ctx.r3.u32 + 160, ctx.r6.u32);
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// stw r11,172(r3)
	PPC_STORE_U32(ctx.r3.u32 + 172, ctx.r11.u32);
	// lis r6,4096
	ctx.r6.s64 = 268435456;
	// stw r10,180(r3)
	PPC_STORE_U32(ctx.r3.u32 + 180, ctx.r10.u32);
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// lis r10,8192
	ctx.r10.s64 = 536870912;
	// ori r9,r9,18
	ctx.r9.u64 = ctx.r9.u64 | 18;
	// ori r8,r8,15
	ctx.r8.u64 = ctx.r8.u64 | 15;
	// ori r7,r7,17
	ctx.r7.u64 = ctx.r7.u64 | 17;
	// ori r6,r6,33
	ctx.r6.u64 = ctx.r6.u64 | 33;
	// ori r11,r11,34
	ctx.r11.u64 = ctx.r11.u64 | 34;
	// ori r10,r10,55
	ctx.r10.u64 = ctx.r10.u64 | 55;
	// stw r9,188(r3)
	PPC_STORE_U32(ctx.r3.u32 + 188, ctx.r9.u32);
	// stw r8,196(r3)
	PPC_STORE_U32(ctx.r3.u32 + 196, ctx.r8.u32);
	// stw r7,204(r3)
	PPC_STORE_U32(ctx.r3.u32 + 204, ctx.r7.u32);
	// stw r6,212(r3)
	PPC_STORE_U32(ctx.r3.u32 + 212, ctx.r6.u32);
	// stw r11,220(r3)
	PPC_STORE_U32(ctx.r3.u32 + 220, ctx.r11.u32);
	// stw r10,228(r3)
	PPC_STORE_U32(ctx.r3.u32 + 228, ctx.r10.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E5CF8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E5CF8) { __imp__sub_826E5CF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E5CF8) {
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
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// li r30,0
	ctx.r30.s64 = 0;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x826e5d38
	if (ctx.cr6.eq) goto loc_826E5D38;
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// li r4,1
	ctx.r4.s64 = 1;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826E5D34;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// stw r30,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r30.u32);
loc_826E5D38:
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x826e5d5c
	if (ctx.cr6.eq) goto loc_826E5D5C;
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// li r4,1
	ctx.r4.s64 = 1;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826E5D58;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// stw r30,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r30.u32);
loc_826E5D5C:
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x826e5d70
	if (ctx.cr6.eq) goto loc_826E5D70;
	// bl 0x8218be78
	ctx.lr = 0x826E5D6C;
	sub_8218BE78(ctx, base);
	// stw r30,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r30.u32);
loc_826E5D70:
	// li r11,-1
	ctx.r11.s64 = -1;
	// stw r11,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r11.u32);
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

PPC_FUNC_IMPL(__imp__sub_826E5D90);  /* forward decl */
PPC_WEAK_FUNC(sub_826E5D90) { __imp__sub_826E5D90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E5D90) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e5db4
	if (ctx.cr6.eq) goto loc_826E5DB4;
loc_826E5D9C:
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// cmpw cr6,r4,r10
	ctx.cr6.compare<int32_t>(ctx.r4.s32, ctx.r10.s32, ctx.xer);
	// beq cr6,0x826e5dbc
	if (ctx.cr6.eq) goto loc_826E5DBC;
	// lwz r11,16(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826e5d9c
	if (!ctx.cr6.eq) goto loc_826E5D9C;
loc_826E5DB4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E5DBC:
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E5DC8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E5DC8) { __imp__sub_826E5DC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E5DC8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e5dec
	if (ctx.cr6.eq) goto loc_826E5DEC;
loc_826E5DD4:
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// cmpw cr6,r4,r10
	ctx.cr6.compare<int32_t>(ctx.r4.s32, ctx.r10.s32, ctx.xer);
	// beq cr6,0x826e5df4
	if (ctx.cr6.eq) goto loc_826E5DF4;
	// lwz r11,16(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826e5dd4
	if (!ctx.cr6.eq) goto loc_826E5DD4;
loc_826E5DEC:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E5DF4:
	// lwz r3,4(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E5E00);  /* forward decl */
PPC_WEAK_FUNC(sub_826E5E00) { __imp__sub_826E5E00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E5E00) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb10
	ctx.lr = 0x826E5E08;
	__savegprlr_26(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// mr r28,r4
	ctx.r28.u64 = ctx.r4.u64;
	// mr r27,r5
	ctx.r27.u64 = ctx.r5.u64;
	// mr r26,r6
	ctx.r26.u64 = ctx.r6.u64;
	// li r31,0
	ctx.r31.s64 = 0;
	// bl 0x82674758
	ctx.lr = 0x826E5E24;
	sub_82674758(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// bl 0x826a1b48
	ctx.lr = 0x826E5E2C;
	sub_826A1B48(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826e5e44
	if (!ctx.cr6.eq) goto loc_826E5E44;
	// li r3,0
	ctx.r3.s64 = 0;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
loc_826E5E44:
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// bl 0x824b8758
	ctx.lr = 0x826E5E4C;
	sub_824B8758(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826a2228
	ctx.lr = 0x826E5E58;
	sub_826A2228(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e5ea4
	if (ctx.cr6.eq) goto loc_826E5EA4;
	// lwz r11,0(r29)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r29.u32 + 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e5e88
	if (ctx.cr6.eq) goto loc_826E5E88;
loc_826E5E70:
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// cmpwi cr6,r10,4
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 4, ctx.xer);
	// beq cr6,0x826e5eb0
	if (ctx.cr6.eq) goto loc_826E5EB0;
	// lwz r11,16(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826e5e70
	if (!ctx.cr6.eq) goto loc_826E5E70;
loc_826E5E88:
	// li r7,0
	ctx.r7.s64 = 0;
loc_826E5E8C:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r26
	ctx.r5.u64 = ctx.r26.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826a36d8
	ctx.lr = 0x826E5EA0;
	sub_826A36D8(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
loc_826E5EA4:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
loc_826E5EB0:
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826E5EC4;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r7,r3
	ctx.r7.u64 = ctx.r3.u64;
	// b 0x826e5e8c
	goto loc_826E5E8C;
}

PPC_FUNC_IMPL(__imp__sub_826E5ED0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E5ED0) { __imp__sub_826E5ED0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E5ED0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb10
	ctx.lr = 0x826E5ED8;
	__savegprlr_26(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// mr r28,r4
	ctx.r28.u64 = ctx.r4.u64;
	// mr r26,r5
	ctx.r26.u64 = ctx.r5.u64;
	// mr r29,r6
	ctx.r29.u64 = ctx.r6.u64;
	// li r27,0
	ctx.r27.s64 = 0;
	// bl 0x82674758
	ctx.lr = 0x826E5EF4;
	sub_82674758(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// bl 0x826a1b48
	ctx.lr = 0x826E5EFC;
	sub_826A1B48(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826e5f14
	if (!ctx.cr6.eq) goto loc_826E5F14;
	// li r3,0
	ctx.r3.s64 = 0;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
loc_826E5F14:
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// bl 0x824b8758
	ctx.lr = 0x826E5F1C;
	sub_824B8758(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826a2228
	ctx.lr = 0x826E5F28;
	sub_826A2228(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e5f84
	if (ctx.cr6.eq) goto loc_826E5F84;
	// lwz r11,0(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e5f84
	if (ctx.cr6.eq) goto loc_826E5F84;
loc_826E5F40:
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// cmpw cr6,r29,r10
	ctx.cr6.compare<int32_t>(ctx.r29.s32, ctx.r10.s32, ctx.xer);
	// beq cr6,0x826e5f64
	if (ctx.cr6.eq) goto loc_826E5F64;
	// lwz r11,16(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826e5f40
	if (!ctx.cr6.eq) goto loc_826E5F40;
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
loc_826E5F64:
	// lwz r6,4(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// cmplwi cr6,r6,0
	ctx.cr6.compare<uint32_t>(ctx.r6.u32, 0, ctx.xer);
	// beq cr6,0x826e5f84
	if (ctx.cr6.eq) goto loc_826E5F84;
	// mr r5,r26
	ctx.r5.u64 = ctx.r26.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826a3600
	ctx.lr = 0x826E5F80;
	sub_826A3600(ctx, base);
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
loc_826E5F84:
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E5F90);  /* forward decl */
PPC_WEAK_FUNC(sub_826E5F90) { __imp__sub_826E5F90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E5F90) {
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
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e5fc8
	if (ctx.cr6.eq) goto loc_826E5FC8;
loc_826E5FB0:
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// cmpw cr6,r4,r10
	ctx.cr6.compare<int32_t>(ctx.r4.s32, ctx.r10.s32, ctx.xer);
	// beq cr6,0x826e5fe4
	if (ctx.cr6.eq) goto loc_826E5FE4;
	// lwz r11,16(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826e5fb0
	if (!ctx.cr6.eq) goto loc_826E5FB0;
loc_826E5FC8:
	// li r3,1
	ctx.r3.s64 = 1;
loc_826E5FCC:
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
loc_826E5FE4:
	// lwz r31,4(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x826e5fc8
	if (ctx.cr6.eq) goto loc_826E5FC8;
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// li r30,0
	ctx.r30.s64 = 0;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826E6008;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// bl 0x82977c48
	ctx.lr = 0x826E600C;
	sub_82977C48(ctx, base);
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// ble cr6,0x826e5fc8
	if (!ctx.cr6.gt) goto loc_826E5FC8;
loc_826E6014:
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826E6028;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// bl 0x82977d18
	ctx.lr = 0x826E6030;
	sub_82977D18(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826e6064
	if (!ctx.cr6.eq) goto loc_826E6064;
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// addi r30,r30,1
	ctx.r30.s64 = ctx.r30.s64 + 1;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826E6054;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// bl 0x82977c48
	ctx.lr = 0x826E6058;
	sub_82977C48(ctx, base);
	// cmpw cr6,r30,r3
	ctx.cr6.compare<int32_t>(ctx.r30.s32, ctx.r3.s32, ctx.xer);
	// blt cr6,0x826e6014
	if (ctx.cr6.lt) goto loc_826E6014;
	// b 0x826e5fc8
	goto loc_826E5FC8;
loc_826E6064:
	// li r3,0
	ctx.r3.s64 = 0;
	// b 0x826e5fcc
	goto loc_826E5FCC;
}

PPC_FUNC_IMPL(__imp__sub_826E6070);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6070) { __imp__sub_826E6070(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6070) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,20(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 20);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// bne cr6,0x826e60bc
	if (!ctx.cr6.eq) goto loc_826E60BC;
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e6100
	if (ctx.cr6.eq) goto loc_826E6100;
loc_826E6088:
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// cmpw cr6,r4,r10
	ctx.cr6.compare<int32_t>(ctx.r4.s32, ctx.r10.s32, ctx.xer);
	// beq cr6,0x826e60a8
	if (ctx.cr6.eq) goto loc_826E60A8;
	// lwz r11,16(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826e6088
	if (!ctx.cr6.eq) goto loc_826E6088;
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E60A8:
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e6100
	if (ctx.cr6.eq) goto loc_826E6100;
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E60BC:
	// cmpwi cr6,r11,1
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 1, ctx.xer);
	// bne cr6,0x826e6100
	if (!ctx.cr6.eq) goto loc_826E6100;
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e6100
	if (ctx.cr6.eq) goto loc_826E6100;
loc_826E60D0:
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// cmpw cr6,r4,r10
	ctx.cr6.compare<int32_t>(ctx.r4.s32, ctx.r10.s32, ctx.xer);
	// beq cr6,0x826e60f0
	if (ctx.cr6.eq) goto loc_826E60F0;
	// lwz r11,16(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826e60d0
	if (!ctx.cr6.eq) goto loc_826E60D0;
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E60F0:
	// lwz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// li r3,1
	ctx.r3.s64 = 1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bnelr cr6
	if (!ctx.cr6.eq) return;
loc_826E6100:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6108);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6108) { __imp__sub_826E6108(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6108) {
	PPC_FUNC_PROLOGUE();
	// clrlwi r11,r4,24
	ctx.r11.u64 = ctx.r4.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e6164
	if (ctx.cr6.eq) goto loc_826E6164;
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826e646c
	if (ctx.cr6.gt) goto loc_826E646C;
	// lis r12,-32146
	ctx.r12.s64 = -2106720256;
	// addi r12,r12,24884
	ctx.r12.s64 = ctx.r12.s64 + 24884;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826E6144;
	case 1:
		goto loc_826E614C;
	case 2:
		goto loc_826E6154;
	case 3:
		goto loc_826E615C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E6144:
	// li r3,36
	ctx.r3.s64 = 36;
	// blr 
	return;
loc_826E614C:
	// li r3,37
	ctx.r3.s64 = 37;
	// blr 
	return;
loc_826E6154:
	// li r3,38
	ctx.r3.s64 = 38;
	// blr 
	return;
loc_826E615C:
	// li r3,39
	ctx.r3.s64 = 39;
	// blr 
	return;
loc_826E6164:
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,72
	ctx.r11.u64 = ctx.r11.u64 | 72;
	// cmplw cr6,r3,r11
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, ctx.r11.u32, ctx.xer);
	// bgt cr6,0x826e6310
	if (ctx.cr6.gt) goto loc_826E6310;
	// beq cr6,0x826e6308
	if (ctx.cr6.eq) goto loc_826E6308;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addi r11,r11,-14
	ctx.r11.s64 = ctx.r11.s64 + -14;
	// cmplwi cr6,r11,57
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 57, ctx.xer);
	// bgt cr6,0x826e646c
	if (ctx.cr6.gt) goto loc_826E646C;
	// lis r12,-32146
	ctx.r12.s64 = -2106720256;
	// addi r12,r12,24992
	ctx.r12.s64 = ctx.r12.s64 + 24992;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E6288;
	case 1:
		goto loc_826E62C8;
	case 2:
		goto loc_826E646C;
	case 3:
		goto loc_826E62B8;
	case 4:
		goto loc_826E62C0;
	case 5:
		goto loc_826E646C;
	case 6:
		goto loc_826E646C;
	case 7:
		goto loc_826E646C;
	case 8:
		goto loc_826E646C;
	case 9:
		goto loc_826E646C;
	case 10:
		goto loc_826E646C;
	case 11:
		goto loc_826E646C;
	case 12:
		goto loc_826E646C;
	case 13:
		goto loc_826E646C;
	case 14:
		goto loc_826E646C;
	case 15:
		goto loc_826E646C;
	case 16:
		goto loc_826E646C;
	case 17:
		goto loc_826E646C;
	case 18:
		goto loc_826E646C;
	case 19:
		goto loc_826E62B0;
	case 20:
		goto loc_826E62A8;
	case 21:
		goto loc_826E646C;
	case 22:
		goto loc_826E646C;
	case 23:
		goto loc_826E646C;
	case 24:
		goto loc_826E62A0;
	case 25:
		goto loc_826E6298;
	case 26:
		goto loc_826E646C;
	case 27:
		goto loc_826E6290;
	case 28:
		goto loc_826E646C;
	case 29:
		goto loc_826E646C;
	case 30:
		goto loc_826E646C;
	case 31:
		goto loc_826E646C;
	case 32:
		goto loc_826E646C;
	case 33:
		goto loc_826E646C;
	case 34:
		goto loc_826E646C;
	case 35:
		goto loc_826E646C;
	case 36:
		goto loc_826E646C;
	case 37:
		goto loc_826E646C;
	case 38:
		goto loc_826E646C;
	case 39:
		goto loc_826E646C;
	case 40:
		goto loc_826E646C;
	case 41:
		goto loc_826E646C;
	case 42:
		goto loc_826E646C;
	case 43:
		goto loc_826E646C;
	case 44:
		goto loc_826E646C;
	case 45:
		goto loc_826E646C;
	case 46:
		goto loc_826E646C;
	case 47:
		goto loc_826E646C;
	case 48:
		goto loc_826E646C;
	case 49:
		goto loc_826E646C;
	case 50:
		goto loc_826E646C;
	case 51:
		goto loc_826E62D0;
	case 52:
		goto loc_826E62D8;
	case 53:
		goto loc_826E62E0;
	case 54:
		goto loc_826E62E8;
	case 55:
		goto loc_826E62F0;
	case 56:
		goto loc_826E62F8;
	case 57:
		goto loc_826E6300;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E6288:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E6290:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E6298:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E62A0:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E62A8:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826E62B0:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826E62B8:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826E62C0:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826E62C8:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E62D0:
	// li r3,9
	ctx.r3.s64 = 9;
	// blr 
	return;
loc_826E62D8:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_826E62E0:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_826E62E8:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_826E62F0:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_826E62F8:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_826E6300:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_826E6308:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_826E6310:
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// ori r11,r11,55
	ctx.r11.u64 = ctx.r11.u64 | 55;
	// cmplw cr6,r3,r11
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, ctx.r11.u32, ctx.xer);
	// bgt cr6,0x826e63b0
	if (ctx.cr6.gt) goto loc_826E63B0;
	// beq cr6,0x826e63a8
	if (ctx.cr6.eq) goto loc_826E63A8;
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// ori r11,r11,13
	ctx.r11.u64 = ctx.r11.u64 | 13;
	// cmplw cr6,r3,r11
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, ctx.r11.u32, ctx.xer);
	// bgt cr6,0x826e6374
	if (ctx.cr6.gt) goto loc_826E6374;
	// beq cr6,0x826e636c
	if (ctx.cr6.eq) goto loc_826E636C;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addic. r11,r11,-73
	ctx.xer.ca = ctx.r11.u32 > 72;
	ctx.r11.s64 = ctx.r11.s64 + -73;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826e6364
	if (ctx.cr0.eq) goto loc_826E6364;
	// cmplwi cr6,r11,2
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 2, ctx.xer);
	// beq cr6,0x826e635c
	if (ctx.cr6.eq) goto loc_826E635C;
	// cmplwi cr6,r11,8
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 8, ctx.xer);
	// bne cr6,0x826e646c
	if (!ctx.cr6.eq) goto loc_826E646C;
	// li r3,18
	ctx.r3.s64 = 18;
	// blr 
	return;
loc_826E635C:
	// li r3,26
	ctx.r3.s64 = 26;
	// blr 
	return;
loc_826E6364:
	// li r3,17
	ctx.r3.s64 = 17;
	// blr 
	return;
loc_826E636C:
	// li r3,19
	ctx.r3.s64 = 19;
	// blr 
	return;
loc_826E6374:
	// addis r11,r3,-8192
	ctx.r11.s64 = ctx.r3.s64 + -536870912;
	// addic. r11,r11,-29
	ctx.xer.ca = ctx.r11.u32 > 28;
	ctx.r11.s64 = ctx.r11.s64 + -29;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826e63a0
	if (ctx.cr0.eq) goto loc_826E63A0;
	// cmplwi cr6,r11,13
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 13, ctx.xer);
	// beq cr6,0x826e6398
	if (ctx.cr6.eq) goto loc_826E6398;
	// cmplwi cr6,r11,15
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 15, ctx.xer);
	// bne cr6,0x826e646c
	if (!ctx.cr6.eq) goto loc_826E646C;
	// li r3,22
	ctx.r3.s64 = 22;
	// blr 
	return;
loc_826E6398:
	// li r3,21
	ctx.r3.s64 = 21;
	// blr 
	return;
loc_826E63A0:
	// li r3,20
	ctx.r3.s64 = 20;
	// blr 
	return;
loc_826E63A8:
	// li r3,23
	ctx.r3.s64 = 23;
	// blr 
	return;
loc_826E63B0:
	// addis r11,r3,-8192
	ctx.r11.s64 = ctx.r3.s64 + -536870912;
	// addi r11,r11,-58
	ctx.r11.s64 = ctx.r11.s64 + -58;
	// cmplwi cr6,r11,22
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 22, ctx.xer);
	// bgt cr6,0x826e646c
	if (ctx.cr6.gt) goto loc_826E646C;
	// lis r12,-32146
	ctx.r12.s64 = -2106720256;
	// addi r12,r12,25560
	ctx.r12.s64 = ctx.r12.s64 + 25560;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E6434;
	case 1:
		goto loc_826E646C;
	case 2:
		goto loc_826E646C;
	case 3:
		goto loc_826E646C;
	case 4:
		goto loc_826E643C;
	case 5:
		goto loc_826E646C;
	case 6:
		goto loc_826E646C;
	case 7:
		goto loc_826E646C;
	case 8:
		goto loc_826E646C;
	case 9:
		goto loc_826E646C;
	case 10:
		goto loc_826E646C;
	case 11:
		goto loc_826E646C;
	case 12:
		goto loc_826E646C;
	case 13:
		goto loc_826E646C;
	case 14:
		goto loc_826E646C;
	case 15:
		goto loc_826E646C;
	case 16:
		goto loc_826E646C;
	case 17:
		goto loc_826E646C;
	case 18:
		goto loc_826E6444;
	case 19:
		goto loc_826E644C;
	case 20:
		goto loc_826E6454;
	case 21:
		goto loc_826E645C;
	case 22:
		goto loc_826E6464;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E6434:
	// li r3,24
	ctx.r3.s64 = 24;
	// blr 
	return;
loc_826E643C:
	// li r3,25
	ctx.r3.s64 = 25;
	// blr 
	return;
loc_826E6444:
	// li r3,27
	ctx.r3.s64 = 27;
	// blr 
	return;
loc_826E644C:
	// li r3,28
	ctx.r3.s64 = 28;
	// blr 
	return;
loc_826E6454:
	// li r3,29
	ctx.r3.s64 = 29;
	// blr 
	return;
loc_826E645C:
	// li r3,30
	ctx.r3.s64 = 30;
	// blr 
	return;
loc_826E6464:
	// li r3,31
	ctx.r3.s64 = 31;
	// blr 
	return;
loc_826E646C:
	// lis r3,-32768
	ctx.r3.s64 = -2147483648;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6478);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6478) { __imp__sub_826E6478(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6478) {
	PPC_FUNC_PROLOGUE();
	// lis r11,76
	ctx.r11.s64 = 4980736;
	// ori r11,r11,19264
	ctx.r11.u64 = ctx.r11.u64 | 19264;
	// cmpd cr6,r3,r11
	ctx.cr6.compare<int64_t>(ctx.r3.s64, ctx.r11.s64, ctx.xer);
	// blt cr6,0x826e6490
	if (ctx.cr6.lt) goto loc_826E6490;
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_826E6490:
	// lis r11,38
	ctx.r11.s64 = 2490368;
	// ori r11,r11,9632
	ctx.r11.u64 = ctx.r11.u64 | 9632;
	// cmpd cr6,r3,r11
	ctx.cr6.compare<int64_t>(ctx.r3.s64, ctx.r11.s64, ctx.xer);
	// blt cr6,0x826e64a8
	if (ctx.cr6.lt) goto loc_826E64A8;
	// li r3,9
	ctx.r3.s64 = 9;
	// blr 
	return;
loc_826E64A8:
	// lis r11,15
	ctx.r11.s64 = 983040;
	// ori r11,r11,16960
	ctx.r11.u64 = ctx.r11.u64 | 16960;
	// cmpd cr6,r3,r11
	ctx.cr6.compare<int64_t>(ctx.r3.s64, ctx.r11.s64, ctx.xer);
	// blt cr6,0x826e64c0
	if (ctx.cr6.lt) goto loc_826E64C0;
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826E64C0:
	// lis r11,11
	ctx.r11.s64 = 720896;
	// ori r11,r11,29104
	ctx.r11.u64 = ctx.r11.u64 | 29104;
	// cmpd cr6,r3,r11
	ctx.cr6.compare<int64_t>(ctx.r3.s64, ctx.r11.s64, ctx.xer);
	// blt cr6,0x826e64d8
	if (ctx.cr6.lt) goto loc_826E64D8;
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826E64D8:
	// lis r11,7
	ctx.r11.s64 = 458752;
	// ori r11,r11,41248
	ctx.r11.u64 = ctx.r11.u64 | 41248;
	// cmpd cr6,r3,r11
	ctx.cr6.compare<int64_t>(ctx.r3.s64, ctx.r11.s64, ctx.xer);
	// blt cr6,0x826e64f0
	if (ctx.cr6.lt) goto loc_826E64F0;
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826E64F0:
	// lis r11,3
	ctx.r11.s64 = 196608;
	// ori r11,r11,53392
	ctx.r11.u64 = ctx.r11.u64 | 53392;
	// cmpd cr6,r3,r11
	ctx.cr6.compare<int64_t>(ctx.r3.s64, ctx.r11.s64, ctx.xer);
	// blt cr6,0x826e6508
	if (ctx.cr6.lt) goto loc_826E6508;
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826E6508:
	// lis r11,1
	ctx.r11.s64 = 65536;
	// ori r11,r11,34464
	ctx.r11.u64 = ctx.r11.u64 | 34464;
	// cmpd cr6,r3,r11
	ctx.cr6.compare<int64_t>(ctx.r3.s64, ctx.r11.s64, ctx.xer);
	// blt cr6,0x826e6520
	if (ctx.cr6.lt) goto loc_826E6520;
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E6520:
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,50000
	ctx.r11.u64 = ctx.r11.u64 | 50000;
	// cmpd cr6,r3,r11
	ctx.cr6.compare<int64_t>(ctx.r3.s64, ctx.r11.s64, ctx.xer);
	// blt cr6,0x826e6538
	if (ctx.cr6.lt) goto loc_826E6538;
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E6538:
	// cmpdi cr6,r3,10000
	ctx.cr6.compare<int64_t>(ctx.r3.s64, 10000, ctx.xer);
	// blt cr6,0x826e6548
	if (ctx.cr6.lt) goto loc_826E6548;
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E6548:
	// cmpdi cr6,r3,1000
	ctx.cr6.compare<int64_t>(ctx.r3.s64, 1000, ctx.xer);
	// li r3,1
	ctx.r3.s64 = 1;
	// bgelr cr6
	if (!ctx.cr6.lt) return;
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6560);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6560) { __imp__sub_826E6560(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6560) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x826E6568;
	__savegprlr_28(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// mr r28,r4
	ctx.r28.u64 = ctx.r4.u64;
	// li r29,0
	ctx.r29.s64 = 0;
	// lbz r11,5(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + 5);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e65b8
	if (ctx.cr6.eq) goto loc_826E65B8;
	// li r31,0
	ctx.r31.s64 = 0;
loc_826E6588:
	// lwz r11,20(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 20);
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// add r3,r11,r31
	ctx.r3.u64 = ctx.r11.u64 + ctx.r31.u64;
	// bl 0x82974470
	ctx.lr = 0x826E6598;
	sub_82974470(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826e65c4
	if (!ctx.cr6.eq) goto loc_826E65C4;
	// lbz r11,5(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + 5);
	// addi r29,r29,1
	ctx.r29.s64 = ctx.r29.s64 + 1;
	// addi r31,r31,16
	ctx.r31.s64 = ctx.r31.s64 + 16;
	// cmplw cr6,r29,r11
	ctx.cr6.compare<uint32_t>(ctx.r29.u32, ctx.r11.u32, ctx.xer);
	// blt cr6,0x826e6588
	if (ctx.cr6.lt) goto loc_826E6588;
loc_826E65B8:
	// li r3,0
	ctx.r3.s64 = 0;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
loc_826E65C4:
	// li r3,1
	ctx.r3.s64 = 1;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E65D0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E65D0) { __imp__sub_826E65D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E65D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x826E65D8;
	__savegprlr_28(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// li r28,0
	ctx.r28.s64 = 0;
	// li r29,0
	ctx.r29.s64 = 0;
	// lbz r11,5(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + 5);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e6628
	if (ctx.cr6.eq) goto loc_826E6628;
	// li r31,0
	ctx.r31.s64 = 0;
loc_826E65F8:
	// lwz r11,20(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 20);
	// add r3,r11,r31
	ctx.r3.u64 = ctx.r11.u64 + ctx.r31.u64;
	// bl 0x829743e8
	ctx.lr = 0x826E6604;
	sub_829743E8(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e6614
	if (ctx.cr6.eq) goto loc_826E6614;
	// addi r28,r28,1
	ctx.r28.s64 = ctx.r28.s64 + 1;
loc_826E6614:
	// lbz r11,5(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + 5);
	// addi r29,r29,1
	ctx.r29.s64 = ctx.r29.s64 + 1;
	// addi r31,r31,16
	ctx.r31.s64 = ctx.r31.s64 + 16;
	// cmplw cr6,r29,r11
	ctx.cr6.compare<uint32_t>(ctx.r29.u32, ctx.r11.u32, ctx.xer);
	// blt cr6,0x826e65f8
	if (ctx.cr6.lt) goto loc_826E65F8;
loc_826E6628:
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6638);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6638) { __imp__sub_826E6638(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6638) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x826E6640;
	__savegprlr_27(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// lwz r11,8(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 8);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e66c8
	if (ctx.cr6.eq) goto loc_826E66C8;
	// lbz r11,4(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + 4);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e66bc
	if (ctx.cr6.eq) goto loc_826E66BC;
	// li r27,0
	ctx.r27.s64 = 0;
	// lis r11,-31996
	ctx.r11.s64 = -2096889856;
	// mr r29,r27
	ctx.r29.u64 = ctx.r27.u64;
	// addi r28,r11,24372
	ctx.r28.s64 = ctx.r11.s64 + 24372;
loc_826E6670:
	// lwz r11,8(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 8);
	// rlwinm r31,r29,3,0,28
	ctx.r31.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 3) & 0xFFFFFFF8;
	// add r4,r31,r11
	ctx.r4.u64 = ctx.r31.u64 + ctx.r11.u64;
	// lwz r11,0(r4)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 0);
	// cmpwi cr6,r11,1
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 1, ctx.xer);
	// bne cr6,0x826e66a8
	if (!ctx.cr6.eq) goto loc_826E66A8;
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// bl 0x829777c8
	ctx.lr = 0x826E6690;
	sub_829777C8(ctx, base);
	// lwz r11,8(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 8);
	// li r4,0
	ctx.r4.s64 = 0;
	// add r31,r31,r11
	ctx.r31.u64 = ctx.r31.u64 + ctx.r11.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x827df9f0
	ctx.lr = 0x826E66A4;
	sub_827DF9F0(ctx, base);
	// stw r27,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r27.u32);
loc_826E66A8:
	// addi r11,r29,1
	ctx.r11.s64 = ctx.r29.s64 + 1;
	// lbz r10,4(r30)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r30.u32 + 4);
	// clrlwi r29,r11,24
	ctx.r29.u64 = ctx.r11.u32 & 0xFF;
	// cmplw cr6,r29,r10
	ctx.cr6.compare<uint32_t>(ctx.r29.u32, ctx.r10.u32, ctx.xer);
	// blt cr6,0x826e6670
	if (ctx.cr6.lt) goto loc_826E6670;
loc_826E66BC:
	// lwz r11,0(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// ori r11,r11,64
	ctx.r11.u64 = ctx.r11.u64 | 64;
	// stw r11,0(r30)
	PPC_STORE_U32(ctx.r30.u32 + 0, ctx.r11.u32);
loc_826E66C8:
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E66D0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E66D0) { __imp__sub_826E66D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E66D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x826E66D8;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r31,0(r3)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// li r29,0
	ctx.r29.s64 = 0;
loc_826E66E8:
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x826e671c
	if (ctx.cr6.eq) goto loc_826E671C;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826e6560
	ctx.lr = 0x826E66FC;
	sub_826E6560(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826e6710
	if (!ctx.cr6.eq) goto loc_826E6710;
	// lwz r31,24(r31)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r31.u32 + 24);
	// b 0x826e66e8
	goto loc_826E66E8;
loc_826E6710:
	// li r3,1
	ctx.r3.s64 = 1;
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
loc_826E671C:
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6728);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6728) { __imp__sub_826E6728(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6728) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
loc_826E672C:
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e6754
	if (ctx.cr6.eq) goto loc_826E6754;
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// rlwinm r10,r10,31,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x1;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x826e674c
	if (ctx.cr6.eq) goto loc_826E674C;
	// lwz r11,24(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// b 0x826e672c
	goto loc_826E672C;
loc_826E674C:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E6754:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6760);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6760) { __imp__sub_826E6760(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6760) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r31,0(r3)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x826e67a0
	if (ctx.cr6.eq) goto loc_826E67A0;
loc_826E677C:
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// rlwinm r11,r11,31,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 31) & 0x1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826e6794
	if (!ctx.cr6.eq) goto loc_826E6794;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826e6638
	ctx.lr = 0x826E6794;
	sub_826E6638(ctx, base);
loc_826E6794:
	// lwz r31,24(r31)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r31.u32 + 24);
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// bne cr6,0x826e677c
	if (!ctx.cr6.eq) goto loc_826E677C;
loc_826E67A0:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E67B8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E67B8) { __imp__sub_826E67B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E67B8) {
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
	// lwz r31,0(r3)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
loc_826E67D4:
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x826e6834
	if (ctx.cr6.eq) goto loc_826E6834;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826e6560
	ctx.lr = 0x826E67E8;
	sub_826E6560(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826e67fc
	if (!ctx.cr6.eq) goto loc_826E67FC;
	// lwz r31,24(r31)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r31.u32 + 24);
	// b 0x826e67d4
	goto loc_826E67D4;
loc_826E67FC:
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// rlwinm r10,r11,31,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 31) & 0x1;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x826e6828
	if (ctx.cr6.eq) goto loc_826E6828;
	// rlwinm r10,r11,28,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 28) & 0x1;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x826e6828
	if (ctx.cr6.eq) goto loc_826E6828;
	// rlwinm r11,r11,29,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 29) & 0x1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// beq cr6,0x826e682c
	if (ctx.cr6.eq) goto loc_826E682C;
loc_826E6828:
	// li r11,0
	ctx.r11.s64 = 0;
loc_826E682C:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x826e6838
	goto loc_826E6838;
loc_826E6834:
	// li r3,1
	ctx.r3.s64 = 1;
loc_826E6838:
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

PPC_FUNC_IMPL(__imp__sub_826E6850);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6850) { __imp__sub_826E6850(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6850) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,30644
	ctx.r3.s64 = ctx.r11.s64 + 30644;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6860);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6860) { __imp__sub_826E6860(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6860) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6868);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6868) { __imp__sub_826E6868(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6868) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,30684
	ctx.r3.s64 = ctx.r11.s64 + 30684;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6878);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6878) { __imp__sub_826E6878(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6878) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6880);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6880) { __imp__sub_826E6880(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6880) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,30724
	ctx.r3.s64 = ctx.r11.s64 + 30724;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6890);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6890) { __imp__sub_826E6890(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6890) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,5
	ctx.r3.u64 = ctx.r3.u64 | 5;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E68A0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E68A0) { __imp__sub_826E68A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E68A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,30760
	ctx.r3.s64 = ctx.r11.s64 + 30760;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E68B0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E68B0) { __imp__sub_826E68B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E68B0) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,5
	ctx.r3.u64 = ctx.r3.u64 | 5;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E68C0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E68C0) { __imp__sub_826E68C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E68C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,30800
	ctx.r3.s64 = ctx.r11.s64 + 30800;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E68D0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E68D0) { __imp__sub_826E68D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E68D0) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,2
	ctx.r3.u64 = ctx.r3.u64 | 2;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E68E0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E68E0) { __imp__sub_826E68E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E68E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,30840
	ctx.r3.s64 = ctx.r11.s64 + 30840;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E68F0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E68F0) { __imp__sub_826E68F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E68F0) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,2
	ctx.r3.u64 = ctx.r3.u64 | 2;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6900);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6900) { __imp__sub_826E6900(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6900) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,30880
	ctx.r3.s64 = ctx.r11.s64 + 30880;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6910);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6910) { __imp__sub_826E6910(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6910) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,1
	ctx.r3.u64 = ctx.r3.u64 | 1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6920);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6920) { __imp__sub_826E6920(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6920) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,30920
	ctx.r3.s64 = ctx.r11.s64 + 30920;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6930);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6930) { __imp__sub_826E6930(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6930) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,1
	ctx.r3.u64 = ctx.r3.u64 | 1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6940);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6940) { __imp__sub_826E6940(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6940) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,30960
	ctx.r3.s64 = ctx.r11.s64 + 30960;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6950);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6950) { __imp__sub_826E6950(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6950) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,4
	ctx.r3.u64 = ctx.r3.u64 | 4;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6960);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6960) { __imp__sub_826E6960(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6960) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,30996
	ctx.r3.s64 = ctx.r11.s64 + 30996;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6970);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6970) { __imp__sub_826E6970(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6970) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,4
	ctx.r3.u64 = ctx.r3.u64 | 4;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6980);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6980) { __imp__sub_826E6980(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6980) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,31036
	ctx.r3.s64 = ctx.r11.s64 + 31036;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6990);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6990) { __imp__sub_826E6990(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6990) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,3
	ctx.r3.u64 = ctx.r3.u64 | 3;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E69A0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E69A0) { __imp__sub_826E69A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E69A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,31072
	ctx.r3.s64 = ctx.r11.s64 + 31072;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E69B0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E69B0) { __imp__sub_826E69B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E69B0) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,3
	ctx.r3.u64 = ctx.r3.u64 | 3;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E69C0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E69C0) { __imp__sub_826E69C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E69C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,31108
	ctx.r3.s64 = ctx.r11.s64 + 31108;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E69D0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E69D0) { __imp__sub_826E69D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E69D0) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,10
	ctx.r3.u64 = ctx.r3.u64 | 10;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E69E0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E69E0) { __imp__sub_826E69E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E69E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,31140
	ctx.r3.s64 = ctx.r11.s64 + 31140;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E69F0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E69F0) { __imp__sub_826E69F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E69F0) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,10
	ctx.r3.u64 = ctx.r3.u64 | 10;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6A00);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6A00) { __imp__sub_826E6A00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6A00) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,3
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 3, ctx.xer);
	// bgt cr6,0x826e6a38
	if (ctx.cr6.gt) goto loc_826E6A38;
	// lis r12,-32146
	ctx.r12.s64 = -2106720256;
	// addi r12,r12,27168
	ctx.r12.s64 = ctx.r12.s64 + 27168;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826E6A30;
	case 1:
		goto loc_826E6A30;
	case 2:
		goto loc_826E6A30;
	case 3:
		goto loc_826E6A30;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E6A30:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826E6A38:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6A40);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6A40) { __imp__sub_826E6A40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6A40) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,31176
	ctx.r3.s64 = ctx.r11.s64 + 31176;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6A50);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6A50) { __imp__sub_826E6A50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6A50) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,11
	ctx.r3.u64 = ctx.r3.u64 | 11;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6A60);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6A60) { __imp__sub_826E6A60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6A60) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,31208
	ctx.r3.s64 = ctx.r11.s64 + 31208;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6A70);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6A70) { __imp__sub_826E6A70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6A70) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,11
	ctx.r3.u64 = ctx.r3.u64 | 11;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6A80);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6A80) { __imp__sub_826E6A80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6A80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,31244
	ctx.r3.s64 = ctx.r11.s64 + 31244;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6A90);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6A90) { __imp__sub_826E6A90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6A90) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,9
	ctx.r3.u64 = ctx.r3.u64 | 9;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6AA0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6AA0) { __imp__sub_826E6AA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6AA0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,31276
	ctx.r3.s64 = ctx.r11.s64 + 31276;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6AB0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6AB0) { __imp__sub_826E6AB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6AB0) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,9
	ctx.r3.u64 = ctx.r3.u64 | 9;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6AC0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6AC0) { __imp__sub_826E6AC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6AC0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,31312
	ctx.r3.s64 = ctx.r11.s64 + 31312;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6AD0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6AD0) { __imp__sub_826E6AD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6AD0) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,12
	ctx.r3.u64 = ctx.r3.u64 | 12;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6AE0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6AE0) { __imp__sub_826E6AE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6AE0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,31344
	ctx.r3.s64 = ctx.r11.s64 + 31344;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6AF0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6AF0) { __imp__sub_826E6AF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6AF0) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,12
	ctx.r3.u64 = ctx.r3.u64 | 12;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6B00);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6B00) { __imp__sub_826E6B00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6B00) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,31380
	ctx.r3.s64 = ctx.r11.s64 + 31380;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6B10);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6B10) { __imp__sub_826E6B10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6B10) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,15
	ctx.r3.u64 = ctx.r3.u64 | 15;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6B20);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6B20) { __imp__sub_826E6B20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6B20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,31416
	ctx.r3.s64 = ctx.r11.s64 + 31416;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6B30);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6B30) { __imp__sub_826E6B30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6B30) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,15
	ctx.r3.u64 = ctx.r3.u64 | 15;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6B40);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6B40) { __imp__sub_826E6B40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6B40) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,3
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 3, ctx.xer);
	// bgt cr6,0x826e6b80
	if (ctx.cr6.gt) goto loc_826E6B80;
	// lis r12,-32146
	ctx.r12.s64 = -2106720256;
	// addi r12,r12,27488
	ctx.r12.s64 = ctx.r12.s64 + 27488;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826E6B70;
	case 1:
		goto loc_826E6B70;
	case 2:
		goto loc_826E6B78;
	case 3:
		goto loc_826E6B78;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E6B70:
	// li r11,2
	ctx.r11.s64 = 2;
	// b 0x826e6b84
	goto loc_826E6B84;
loc_826E6B78:
	// li r11,3
	ctx.r11.s64 = 3;
	// b 0x826e6b84
	goto loc_826E6B84;
loc_826E6B80:
	// li r11,-1
	ctx.r11.s64 = -1;
loc_826E6B84:
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826e6bdc
	if (ctx.cr6.gt) goto loc_826E6BDC;
	// lis r12,-32146
	ctx.r12.s64 = -2106720256;
	// addi r12,r12,27560
	ctx.r12.s64 = ctx.r12.s64 + 27560;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E6BBC;
	case 1:
		goto loc_826E6BC4;
	case 2:
		goto loc_826E6BD4;
	case 3:
		goto loc_826E6BDC;
	case 4:
		goto loc_826E6BCC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E6BBC:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E6BC4:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E6BCC:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E6BD4:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E6BDC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6BE8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6BE8) { __imp__sub_826E6BE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6BE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,31452
	ctx.r3.s64 = ctx.r11.s64 + 31452;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6BF8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6BF8) { __imp__sub_826E6BF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6BF8) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6C08);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6C08) { __imp__sub_826E6C08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6C08) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,31484
	ctx.r3.s64 = ctx.r11.s64 + 31484;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6C18);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6C18) { __imp__sub_826E6C18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6C18) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6C28);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6C28) { __imp__sub_826E6C28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6C28) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,31520
	ctx.r3.s64 = ctx.r11.s64 + 31520;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6C38);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6C38) { __imp__sub_826E6C38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6C38) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6C48);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6C48) { __imp__sub_826E6C48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6C48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,31560
	ctx.r3.s64 = ctx.r11.s64 + 31560;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6C58);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6C58) { __imp__sub_826E6C58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6C58) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6C68);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6C68) { __imp__sub_826E6C68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6C68) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,31604
	ctx.r3.s64 = ctx.r11.s64 + 31604;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6C78);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6C78) { __imp__sub_826E6C78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6C78) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,6
	ctx.r3.u64 = ctx.r3.u64 | 6;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6C88);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6C88) { __imp__sub_826E6C88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6C88) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,31640
	ctx.r3.s64 = ctx.r11.s64 + 31640;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6C98);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6C98) { __imp__sub_826E6C98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6C98) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,6
	ctx.r3.u64 = ctx.r3.u64 | 6;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6CA8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6CA8) { __imp__sub_826E6CA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6CA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,31680
	ctx.r3.s64 = ctx.r11.s64 + 31680;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6CB8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6CB8) { __imp__sub_826E6CB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6CB8) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,8
	ctx.r3.u64 = ctx.r3.u64 | 8;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6CC8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6CC8) { __imp__sub_826E6CC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6CC8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,31716
	ctx.r3.s64 = ctx.r11.s64 + 31716;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6CD8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6CD8) { __imp__sub_826E6CD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6CD8) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,8
	ctx.r3.u64 = ctx.r3.u64 | 8;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6CE8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6CE8) { __imp__sub_826E6CE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6CE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,31756
	ctx.r3.s64 = ctx.r11.s64 + 31756;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6CF8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6CF8) { __imp__sub_826E6CF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6CF8) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,7
	ctx.r3.u64 = ctx.r3.u64 | 7;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6D08);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6D08) { __imp__sub_826E6D08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6D08) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,31788
	ctx.r3.s64 = ctx.r11.s64 + 31788;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6D18);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6D18) { __imp__sub_826E6D18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6D18) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,7
	ctx.r3.u64 = ctx.r3.u64 | 7;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6D28);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6D28) { __imp__sub_826E6D28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6D28) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,31824
	ctx.r3.s64 = ctx.r11.s64 + 31824;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6D38);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6D38) { __imp__sub_826E6D38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6D38) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,16
	ctx.r3.u64 = ctx.r3.u64 | 16;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6D48);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6D48) { __imp__sub_826E6D48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6D48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,31856
	ctx.r3.s64 = ctx.r11.s64 + 31856;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6D58);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6D58) { __imp__sub_826E6D58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6D58) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,16
	ctx.r3.u64 = ctx.r3.u64 | 16;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6D68);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6D68) { __imp__sub_826E6D68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6D68) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,31892
	ctx.r3.s64 = ctx.r11.s64 + 31892;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6D78);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6D78) { __imp__sub_826E6D78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6D78) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,18
	ctx.r3.u64 = ctx.r3.u64 | 18;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6D88);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6D88) { __imp__sub_826E6D88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6D88) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,31928
	ctx.r3.s64 = ctx.r11.s64 + 31928;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6D98);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6D98) { __imp__sub_826E6D98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6D98) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,18
	ctx.r3.u64 = ctx.r3.u64 | 18;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6DA8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6DA8) { __imp__sub_826E6DA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6DA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,31964
	ctx.r3.s64 = ctx.r11.s64 + 31964;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6DB8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6DB8) { __imp__sub_826E6DB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6DB8) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,19
	ctx.r3.u64 = ctx.r3.u64 | 19;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6DC8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6DC8) { __imp__sub_826E6DC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6DC8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,32000
	ctx.r3.s64 = ctx.r11.s64 + 32000;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6DD8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6DD8) { __imp__sub_826E6DD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6DD8) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,19
	ctx.r3.u64 = ctx.r3.u64 | 19;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6DE8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6DE8) { __imp__sub_826E6DE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6DE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,32036
	ctx.r3.s64 = ctx.r11.s64 + 32036;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6DF8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6DF8) { __imp__sub_826E6DF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6DF8) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,20
	ctx.r3.u64 = ctx.r3.u64 | 20;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6E08);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6E08) { __imp__sub_826E6E08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6E08) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,32072
	ctx.r3.s64 = ctx.r11.s64 + 32072;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6E18);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6E18) { __imp__sub_826E6E18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6E18) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,20
	ctx.r3.u64 = ctx.r3.u64 | 20;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6E28);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6E28) { __imp__sub_826E6E28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6E28) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,32108
	ctx.r3.s64 = ctx.r11.s64 + 32108;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6E38);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6E38) { __imp__sub_826E6E38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6E38) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,21
	ctx.r3.u64 = ctx.r3.u64 | 21;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6E48);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6E48) { __imp__sub_826E6E48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6E48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,32144
	ctx.r3.s64 = ctx.r11.s64 + 32144;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6E58);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6E58) { __imp__sub_826E6E58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6E58) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,21
	ctx.r3.u64 = ctx.r3.u64 | 21;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6E68);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6E68) { __imp__sub_826E6E68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6E68) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,32180
	ctx.r3.s64 = ctx.r11.s64 + 32180;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6E78);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6E78) { __imp__sub_826E6E78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6E78) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,22
	ctx.r3.u64 = ctx.r3.u64 | 22;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6E88);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6E88) { __imp__sub_826E6E88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6E88) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,32216
	ctx.r3.s64 = ctx.r11.s64 + 32216;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6E98);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6E98) { __imp__sub_826E6E98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6E98) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,22
	ctx.r3.u64 = ctx.r3.u64 | 22;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6EA8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6EA8) { __imp__sub_826E6EA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6EA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,32252
	ctx.r3.s64 = ctx.r11.s64 + 32252;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6EB8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6EB8) { __imp__sub_826E6EB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6EB8) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,23
	ctx.r3.u64 = ctx.r3.u64 | 23;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6EC8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6EC8) { __imp__sub_826E6EC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6EC8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,32288
	ctx.r3.s64 = ctx.r11.s64 + 32288;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6ED8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6ED8) { __imp__sub_826E6ED8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6ED8) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,24
	ctx.r3.u64 = ctx.r3.u64 | 24;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6EE8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6EE8) { __imp__sub_826E6EE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6EE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,32324
	ctx.r3.s64 = ctx.r11.s64 + 32324;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6EF8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6EF8) { __imp__sub_826E6EF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6EF8) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,24
	ctx.r3.u64 = ctx.r3.u64 | 24;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6F08);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6F08) { __imp__sub_826E6F08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6F08) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,32360
	ctx.r3.s64 = ctx.r11.s64 + 32360;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6F18);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6F18) { __imp__sub_826E6F18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6F18) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,25
	ctx.r3.u64 = ctx.r3.u64 | 25;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6F28);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6F28) { __imp__sub_826E6F28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6F28) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,32396
	ctx.r3.s64 = ctx.r11.s64 + 32396;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6F38);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6F38) { __imp__sub_826E6F38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6F38) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,25
	ctx.r3.u64 = ctx.r3.u64 | 25;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6F48);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6F48) { __imp__sub_826E6F48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6F48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,32432
	ctx.r3.s64 = ctx.r11.s64 + 32432;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6F58);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6F58) { __imp__sub_826E6F58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6F58) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,26
	ctx.r3.u64 = ctx.r3.u64 | 26;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6F68);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6F68) { __imp__sub_826E6F68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6F68) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,32468
	ctx.r3.s64 = ctx.r11.s64 + 32468;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6F78);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6F78) { __imp__sub_826E6F78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6F78) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,26
	ctx.r3.u64 = ctx.r3.u64 | 26;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6F88);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6F88) { __imp__sub_826E6F88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6F88) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,32504
	ctx.r3.s64 = ctx.r11.s64 + 32504;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6F98);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6F98) { __imp__sub_826E6F98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6F98) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,27
	ctx.r3.u64 = ctx.r3.u64 | 27;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E6FA8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E6FA8) { __imp__sub_826E6FA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E6FA8) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,3
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 3, ctx.xer);
	// bgt cr6,0x826e6ff8
	if (ctx.cr6.gt) goto loc_826E6FF8;
	// lis r12,-32146
	ctx.r12.s64 = -2106720256;
	// addi r12,r12,28616
	ctx.r12.s64 = ctx.r12.s64 + 28616;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826E6FD8;
	case 1:
		goto loc_826E6FE0;
	case 2:
		goto loc_826E6FE8;
	case 3:
		goto loc_826E6FF0;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E6FD8:
	// li r3,61
	ctx.r3.s64 = 61;
	// blr 
	return;
loc_826E6FE0:
	// li r3,62
	ctx.r3.s64 = 62;
	// blr 
	return;
loc_826E6FE8:
	// li r3,63
	ctx.r3.s64 = 63;
	// blr 
	return;
loc_826E6FF0:
	// li r3,64
	ctx.r3.s64 = 64;
	// blr 
	return;
loc_826E6FF8:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7000);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7000) { __imp__sub_826E7000(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7000) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,32540
	ctx.r3.s64 = ctx.r11.s64 + 32540;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7010);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7010) { __imp__sub_826E7010(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7010) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,27
	ctx.r3.u64 = ctx.r3.u64 | 27;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7020);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7020) { __imp__sub_826E7020(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7020) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,32580
	ctx.r3.s64 = ctx.r11.s64 + 32580;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7030);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7030) { __imp__sub_826E7030(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7030) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,28
	ctx.r3.u64 = ctx.r3.u64 | 28;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7040);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7040) { __imp__sub_826E7040(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7040) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,32616
	ctx.r3.s64 = ctx.r11.s64 + 32616;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7050);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7050) { __imp__sub_826E7050(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7050) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,28
	ctx.r3.u64 = ctx.r3.u64 | 28;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7060);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7060) { __imp__sub_826E7060(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7060) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,32656
	ctx.r3.s64 = ctx.r11.s64 + 32656;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7070);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7070) { __imp__sub_826E7070(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7070) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,29
	ctx.r3.u64 = ctx.r3.u64 | 29;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7080);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7080) { __imp__sub_826E7080(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7080) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,32692
	ctx.r3.s64 = ctx.r11.s64 + 32692;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7090);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7090) { __imp__sub_826E7090(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7090) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,29
	ctx.r3.u64 = ctx.r3.u64 | 29;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E70A0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E70A0) { __imp__sub_826E70A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E70A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r3,r11,32732
	ctx.r3.s64 = ctx.r11.s64 + 32732;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E70B0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E70B0) { __imp__sub_826E70B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E70B0) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,30
	ctx.r3.u64 = ctx.r3.u64 | 30;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E70C0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E70C0) { __imp__sub_826E70C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E70C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-32768
	ctx.r3.s64 = ctx.r11.s64 + -32768;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E70D0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E70D0) { __imp__sub_826E70D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E70D0) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,30
	ctx.r3.u64 = ctx.r3.u64 | 30;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E70E0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E70E0) { __imp__sub_826E70E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E70E0) {
	PPC_FUNC_PROLOGUE();
	// addi r11,r4,-61
	ctx.r11.s64 = ctx.r4.s64 + -61;
	// cmplwi cr6,r11,3
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 3, ctx.xer);
	// bgt cr6,0x826e7134
	if (ctx.cr6.gt) goto loc_826E7134;
	// lis r12,-32146
	ctx.r12.s64 = -2106720256;
	// addi r12,r12,28932
	ctx.r12.s64 = ctx.r12.s64 + 28932;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E7114;
	case 1:
		goto loc_826E711C;
	case 2:
		goto loc_826E7124;
	case 3:
		goto loc_826E712C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E7114:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E711C:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E7124:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E712C:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E7134:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7140);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7140) { __imp__sub_826E7140(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7140) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-32728
	ctx.r3.s64 = ctx.r11.s64 + -32728;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7150);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7150) { __imp__sub_826E7150(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7150) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-1
	ctx.r3.s64 = -65536;
	// ori r3,r3,31
	ctx.r3.u64 = ctx.r3.u64 | 31;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7160);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7160) { __imp__sub_826E7160(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7160) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-32692
	ctx.r3.s64 = ctx.r11.s64 + -32692;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7170);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7170) { __imp__sub_826E7170(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7170) {
	PPC_FUNC_PROLOGUE();
	// lis r3,-2
	ctx.r3.s64 = -131072;
	// ori r3,r3,31
	ctx.r3.u64 = ctx.r3.u64 | 31;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7180);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7180) { __imp__sub_826E7180(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7180) {
	PPC_FUNC_PROLOGUE();
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,80(r3)
	PPC_STORE_U32(ctx.r3.u32 + 80, ctx.r11.u32);
	// stw r11,224(r3)
	PPC_STORE_U32(ctx.r3.u32 + 224, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7190);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7190) { __imp__sub_826E7190(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7190) {
	PPC_FUNC_PROLOGUE();
	// li r9,19
	ctx.r9.s64 = 19;
	// li r11,0
	ctx.r11.s64 = 0;
	// addi r10,r3,4
	ctx.r10.s64 = ctx.r3.s64 + 4;
	// stw r9,80(r3)
	PPC_STORE_U32(ctx.r3.u32 + 80, ctx.r9.u32);
	// lis r9,-32768
	ctx.r9.s64 = -2147483648;
loc_826E71A4:
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// lwz r8,80(r3)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 80);
	// addi r10,r10,4
	ctx.r10.s64 = ctx.r10.s64 + 4;
	// cmpw cr6,r11,r8
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r8.s32, ctx.xer);
	// blt cr6,0x826e71a4
	if (ctx.cr6.lt) goto loc_826E71A4;
	// li r8,17
	ctx.r8.s64 = 17;
	// li r11,0
	ctx.r11.s64 = 0;
	// addi r10,r3,88
	ctx.r10.s64 = ctx.r3.s64 + 88;
	// stw r8,224(r3)
	PPC_STORE_U32(ctx.r3.u32 + 224, ctx.r8.u32);
loc_826E71CC:
	// std r9,0(r10)
	PPC_STORE_U64(ctx.r10.u32 + 0, ctx.r9.u64);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// lwz r8,224(r3)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r3.u32 + 224);
	// addi r10,r10,8
	ctx.r10.s64 = ctx.r10.s64 + 8;
	// cmpw cr6,r11,r8
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r8.s32, ctx.xer);
	// blt cr6,0x826e71cc
	if (ctx.cr6.lt) goto loc_826E71CC;
	// li r11,-1
	ctx.r11.s64 = -1;
	// stw r11,232(r3)
	PPC_STORE_U32(ctx.r3.u32 + 232, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E71F0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E71F0) { __imp__sub_826E71F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E71F0) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,18
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 18, ctx.xer);
	// bgt cr6,0x826e7210
	if (ctx.cr6.gt) goto loc_826E7210;
	// addi r10,r4,1
	ctx.r10.s64 = ctx.r4.s64 + 1;
	// li r3,1
	ctx.r3.s64 = 1;
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// stwx r5,r10,r11
	PPC_STORE_U32(ctx.r10.u32 + ctx.r11.u32, ctx.r5.u32);
	// blr 
	return;
loc_826E7210:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7218);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7218) { __imp__sub_826E7218(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7218) {
	PPC_FUNC_PROLOGUE();
	// addi r10,r4,-19
	ctx.r10.s64 = ctx.r4.s64 + -19;
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// cmplwi cr6,r10,16
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 16, ctx.xer);
	// bgt cr6,0x826e723c
	if (ctx.cr6.gt) goto loc_826E723C;
	// addi r10,r4,-8
	ctx.r10.s64 = ctx.r4.s64 + -8;
	// li r3,1
	ctx.r3.s64 = 1;
	// rlwinm r10,r10,3,0,28
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 3) & 0xFFFFFFF8;
	// stdx r5,r10,r11
	PPC_STORE_U64(ctx.r10.u32 + ctx.r11.u32, ctx.r5.u64);
	// blr 
	return;
loc_826E723C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7248);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7248) { __imp__sub_826E7248(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7248) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,18
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 18, ctx.xer);
	// bgt cr6,0x826e726c
	if (ctx.cr6.gt) goto loc_826E726C;
	// addi r10,r4,1
	ctx.r10.s64 = ctx.r4.s64 + 1;
	// li r3,1
	ctx.r3.s64 = 1;
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// stw r11,0(r5)
	PPC_STORE_U32(ctx.r5.u32 + 0, ctx.r11.u32);
	// blr 
	return;
loc_826E726C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7278);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7278) { __imp__sub_826E7278(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7278) {
	PPC_FUNC_PROLOGUE();
	// addi r10,r4,-19
	ctx.r10.s64 = ctx.r4.s64 + -19;
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// cmplwi cr6,r10,16
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 16, ctx.xer);
	// bgt cr6,0x826e72a0
	if (ctx.cr6.gt) goto loc_826E72A0;
	// addi r10,r4,-8
	ctx.r10.s64 = ctx.r4.s64 + -8;
	// li r3,1
	ctx.r3.s64 = 1;
	// rlwinm r10,r10,3,0,28
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 3) & 0xFFFFFFF8;
	// ldx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U64(ctx.r10.u32 + ctx.r11.u32);
	// std r11,0(r5)
	PPC_STORE_U64(ctx.r5.u32 + 0, ctx.r11.u64);
	// blr 
	return;
loc_826E72A0:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E72A8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E72A8) { __imp__sub_826E72A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E72A8) {
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
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// bl 0x826e5cf8
	ctx.lr = 0x826E72C8;
	sub_826E5CF8(ctx, base);
	// clrlwi r11,r30,31
	ctx.r11.u64 = ctx.r30.u32 & 0x1;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826e72e0
	if (ctx.cr6.eq) goto loc_826E72E0;
	// bl 0x8218be78
	ctx.lr = 0x826E72DC;
	sub_8218BE78(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
loc_826E72E0:
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

PPC_FUNC_IMPL(__imp__sub_826E72F8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E72F8) { __imp__sub_826E72F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E72F8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826E7300;
	__savegprlr_25(ctx, base);
	// stfd f31,-72(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -72, ctx.f31.u64);
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,0(r4)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 0);
	// mr r25,r5
	ctx.r25.u64 = ctx.r5.u64;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// mr r28,r7
	ctx.r28.u64 = ctx.r7.u64;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826E7328;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// bl 0x82977c48
	ctx.lr = 0x826E7330;
	sub_82977C48(ctx, base);
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// li r31,0
	ctx.r31.s64 = 0;
	// cmplwi cr6,r27,0
	ctx.cr6.compare<uint32_t>(ctx.r27.u32, 0, ctx.xer);
	// beq cr6,0x826e7408
	if (ctx.cr6.eq) goto loc_826E7408;
	// lis r26,-32768
	ctx.r26.s64 = -2147483648;
loc_826E7344:
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x829782d8
	ctx.lr = 0x826E7350;
	sub_829782D8(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// bl 0x826e6108
	ctx.lr = 0x826E7358;
	sub_826E6108(ctx, base);
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// cmpw cr6,r29,r26
	ctx.cr6.compare<int32_t>(ctx.r29.s32, ctx.r26.s32, ctx.xer);
	// beq cr6,0x826e73d8
	if (ctx.cr6.eq) goto loc_826E73D8;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82978318
	ctx.lr = 0x826E7370;
	sub_82978318(ctx, base);
	// cmplwi cr6,r3,1
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 1, ctx.xer);
	// blt cr6,0x826e73d0
	if (ctx.cr6.lt) goto loc_826E73D0;
	// beq cr6,0x826e73b4
	if (ctx.cr6.eq) goto loc_826E73B4;
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// blt cr6,0x826e73a0
	if (ctx.cr6.lt) goto loc_826E73A0;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x829785e0
	ctx.lr = 0x826E7390;
	sub_829785E0(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82978318
	ctx.lr = 0x826E739C;
	sub_82978318(ctx, base);
	// b 0x826e73d8
	goto loc_826E73D8;
loc_826E73A0:
	// cmpw cr6,r29,r28
	ctx.cr6.compare<int32_t>(ctx.r29.s32, ctx.r28.s32, ctx.xer);
	// bne cr6,0x826e73d8
	if (!ctx.cr6.eq) goto loc_826E73D8;
	// stfs f31,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// b 0x826e73f8
	goto loc_826E73F8;
loc_826E73B4:
	// cmpw cr6,r29,r28
	ctx.cr6.compare<int32_t>(ctx.r29.s32, ctx.r28.s32, ctx.xer);
	// bne cr6,0x826e73d8
	if (!ctx.cr6.eq) goto loc_826E73D8;
	// extsw r11,r25
	ctx.r11.s64 = ctx.r25.s32;
	// li r6,8
	ctx.r6.s64 = 8;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// std r11,80(r1)
	PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r11.u64);
	// b 0x826e73fc
	goto loc_826E73FC;
loc_826E73D0:
	// cmpw cr6,r29,r28
	ctx.cr6.compare<int32_t>(ctx.r29.s32, ctx.r28.s32, ctx.xer);
	// beq cr6,0x826e73f0
	if (ctx.cr6.eq) goto loc_826E73F0;
loc_826E73D8:
	// addi r31,r31,1
	ctx.r31.s64 = ctx.r31.s64 + 1;
	// cmplw cr6,r31,r27
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, ctx.r27.u32, ctx.xer);
	// blt cr6,0x826e7344
	if (ctx.cr6.lt) goto loc_826E7344;
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// lfd f31,-72(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -72);
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826E73F0:
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// stw r25,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r25.u32);
loc_826E73F8:
	// li r6,4
	ctx.r6.s64 = 4;
loc_826E73FC:
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x829783d0
	ctx.lr = 0x826E7408;
	sub_829783D0(ctx, base);
loc_826E7408:
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// lfd f31,-72(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -72);
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7418);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7418) { __imp__sub_826E7418(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7418) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-32652
	ctx.r3.s64 = ctx.r11.s64 + -32652;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7428);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7428) { __imp__sub_826E7428(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7428) {
	PPC_FUNC_PROLOGUE();
	// li r3,94
	ctx.r3.s64 = 94;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7430);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7430) { __imp__sub_826E7430(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7430) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-32620
	ctx.r3.s64 = ctx.r11.s64 + -32620;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7440);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7440) { __imp__sub_826E7440(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7440) {
	PPC_FUNC_PROLOGUE();
	// li r3,95
	ctx.r3.s64 = 95;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7448);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7448) { __imp__sub_826E7448(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7448) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-32588
	ctx.r3.s64 = ctx.r11.s64 + -32588;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7458);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7458) { __imp__sub_826E7458(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7458) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-32556
	ctx.r3.s64 = ctx.r11.s64 + -32556;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7468);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7468) { __imp__sub_826E7468(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7468) {
	PPC_FUNC_PROLOGUE();
	// li r3,97
	ctx.r3.s64 = 97;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7470);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7470) { __imp__sub_826E7470(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7470) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,6
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 6, ctx.xer);
	// bgt cr6,0x826e74e8
	if (ctx.cr6.gt) goto loc_826E74E8;
	// lis r12,-32146
	ctx.r12.s64 = -2106720256;
	// addi r12,r12,29840
	ctx.r12.s64 = ctx.r12.s64 + 29840;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826E74AC;
	case 1:
		goto loc_826E74B4;
	case 2:
		goto loc_826E74BC;
	case 3:
		goto loc_826E74C4;
	case 4:
		goto loc_826E74CC;
	case 5:
		goto loc_826E74D4;
	case 6:
		goto loc_826E74DC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E74AC:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E74B4:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E74BC:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E74C4:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E74CC:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826E74D4:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826E74DC:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826E74E8:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E74F0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E74F0) { __imp__sub_826E74F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E74F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826e757c
	if (ctx.cr6.gt) goto loc_826E757C;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826e7574
	if (ctx.cr6.eq) goto loc_826E7574;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x826e757c
	if (ctx.cr6.gt) goto loc_826E757C;
	// lis r12,-32146
	ctx.r12.s64 = -2106720256;
	// addi r12,r12,29996
	ctx.r12.s64 = ctx.r12.s64 + 29996;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E7544;
	case 1:
		goto loc_826E754C;
	case 2:
		goto loc_826E7554;
	case 3:
		goto loc_826E755C;
	case 4:
		goto loc_826E756C;
	case 5:
		goto loc_826E7564;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E7544:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E754C:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E7554:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E755C:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E7564:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E756C:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826E7574:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826E757C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7588);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7588) { __imp__sub_826E7588(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7588) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-32524
	ctx.r3.s64 = ctx.r11.s64 + -32524;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7598);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7598) { __imp__sub_826E7598(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7598) {
	PPC_FUNC_PROLOGUE();
	// li r3,98
	ctx.r3.s64 = 98;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E75A0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E75A0) { __imp__sub_826E75A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E75A0) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e74f0
	sub_826E74F0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E75A8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E75A8) { __imp__sub_826E75A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E75A8) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e7470
	sub_826E7470(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E75B0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E75B0) { __imp__sub_826E75B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E75B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-32492
	ctx.r3.s64 = ctx.r11.s64 + -32492;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E75C0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E75C0) { __imp__sub_826E75C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E75C0) {
	PPC_FUNC_PROLOGUE();
	// li r3,99
	ctx.r3.s64 = 99;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E75C8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E75C8) { __imp__sub_826E75C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E75C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-32460
	ctx.r3.s64 = ctx.r11.s64 + -32460;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E75D8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E75D8) { __imp__sub_826E75D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E75D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-32428
	ctx.r3.s64 = ctx.r11.s64 + -32428;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E75E8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E75E8) { __imp__sub_826E75E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E75E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-32396
	ctx.r3.s64 = ctx.r11.s64 + -32396;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E75F8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E75F8) { __imp__sub_826E75F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E75F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-32364
	ctx.r3.s64 = ctx.r11.s64 + -32364;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7608);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7608) { __imp__sub_826E7608(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7608) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-32328
	ctx.r3.s64 = ctx.r11.s64 + -32328;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7618);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7618) { __imp__sub_826E7618(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7618) {
	PPC_FUNC_PROLOGUE();
	// li r3,104
	ctx.r3.s64 = 104;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7620);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7620) { __imp__sub_826E7620(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7620) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826e76ac
	if (ctx.cr6.gt) goto loc_826E76AC;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826e76a4
	if (ctx.cr6.eq) goto loc_826E76A4;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x826e76ac
	if (ctx.cr6.gt) goto loc_826E76AC;
	// lis r12,-32146
	ctx.r12.s64 = -2106720256;
	// addi r12,r12,30300
	ctx.r12.s64 = ctx.r12.s64 + 30300;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E7674;
	case 1:
		goto loc_826E767C;
	case 2:
		goto loc_826E7684;
	case 3:
		goto loc_826E768C;
	case 4:
		goto loc_826E7694;
	case 5:
		goto loc_826E769C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E7674:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E767C:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E7684:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E768C:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E7694:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E769C:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826E76A4:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826E76AC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E76B8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E76B8) { __imp__sub_826E76B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E76B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-32292
	ctx.r3.s64 = ctx.r11.s64 + -32292;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E76C8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E76C8) { __imp__sub_826E76C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E76C8) {
	PPC_FUNC_PROLOGUE();
	// li r3,105
	ctx.r3.s64 = 105;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E76D0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E76D0) { __imp__sub_826E76D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E76D0) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e7620
	sub_826E7620(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E76D8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E76D8) { __imp__sub_826E76D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E76D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-32256
	ctx.r3.s64 = ctx.r11.s64 + -32256;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E76E8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E76E8) { __imp__sub_826E76E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E76E8) {
	PPC_FUNC_PROLOGUE();
	// li r3,106
	ctx.r3.s64 = 106;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E76F0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E76F0) { __imp__sub_826E76F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E76F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-32220
	ctx.r3.s64 = ctx.r11.s64 + -32220;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7700);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7700) { __imp__sub_826E7700(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7700) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-32184
	ctx.r3.s64 = ctx.r11.s64 + -32184;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7710);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7710) { __imp__sub_826E7710(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7710) {
	PPC_FUNC_PROLOGUE();
	// li r3,108
	ctx.r3.s64 = 108;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7718);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7718) { __imp__sub_826E7718(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7718) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826e7784
	if (ctx.cr6.gt) goto loc_826E7784;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826e777c
	if (ctx.cr6.eq) goto loc_826E777C;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x826e7784
	if (ctx.cr6.gt) goto loc_826E7784;
	// lis r12,-32146
	ctx.r12.s64 = -2106720256;
	// addi r12,r12,30548
	ctx.r12.s64 = ctx.r12.s64 + 30548;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E776C;
	case 1:
		goto loc_826E776C;
	case 2:
		goto loc_826E7774;
	case 3:
		goto loc_826E776C;
	case 4:
		goto loc_826E776C;
	case 5:
		goto loc_826E776C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E776C:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E7774:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E777C:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E7784:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7790);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7790) { __imp__sub_826E7790(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7790) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-32148
	ctx.r3.s64 = ctx.r11.s64 + -32148;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E77A0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E77A0) { __imp__sub_826E77A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E77A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-32112
	ctx.r3.s64 = ctx.r11.s64 + -32112;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E77B0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E77B0) { __imp__sub_826E77B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E77B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-32076
	ctx.r3.s64 = ctx.r11.s64 + -32076;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E77C0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E77C0) { __imp__sub_826E77C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E77C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-32040
	ctx.r3.s64 = ctx.r11.s64 + -32040;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E77D0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E77D0) { __imp__sub_826E77D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E77D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-32004
	ctx.r3.s64 = ctx.r11.s64 + -32004;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E77E0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E77E0) { __imp__sub_826E77E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E77E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-31968
	ctx.r3.s64 = ctx.r11.s64 + -31968;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E77F0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E77F0) { __imp__sub_826E77F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E77F0) {
	PPC_FUNC_PROLOGUE();
	// li r3,114
	ctx.r3.s64 = 114;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E77F8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E77F8) { __imp__sub_826E77F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E77F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-31932
	ctx.r3.s64 = ctx.r11.s64 + -31932;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7808);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7808) { __imp__sub_826E7808(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7808) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,6
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 6, ctx.xer);
	// bgt cr6,0x826e7854
	if (ctx.cr6.gt) goto loc_826E7854;
	// lis r12,-32146
	ctx.r12.s64 = -2106720256;
	// addi r12,r12,30760
	ctx.r12.s64 = ctx.r12.s64 + 30760;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826E7844;
	case 1:
		goto loc_826E7844;
	case 2:
		goto loc_826E784C;
	case 3:
		goto loc_826E784C;
	case 4:
		goto loc_826E7844;
	case 5:
		goto loc_826E7844;
	case 6:
		goto loc_826E784C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E7844:
	// li r11,1
	ctx.r11.s64 = 1;
	// b 0x826e7858
	goto loc_826E7858;
loc_826E784C:
	// li r11,2
	ctx.r11.s64 = 2;
	// b 0x826e7858
	goto loc_826E7858;
loc_826E7854:
	// li r11,-1
	ctx.r11.s64 = -1;
loc_826E7858:
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826e78b0
	if (ctx.cr6.gt) goto loc_826E78B0;
	// lis r12,-32146
	ctx.r12.s64 = -2106720256;
	// addi r12,r12,30844
	ctx.r12.s64 = ctx.r12.s64 + 30844;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E7890;
	case 1:
		goto loc_826E7898;
	case 2:
		goto loc_826E78A8;
	case 3:
		goto loc_826E78B0;
	case 4:
		goto loc_826E78A0;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E7890:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E7898:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E78A0:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E78A8:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E78B0:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E78B8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E78B8) { __imp__sub_826E78B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E78B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-31896
	ctx.r3.s64 = ctx.r11.s64 + -31896;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E78C8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E78C8) { __imp__sub_826E78C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E78C8) {
	PPC_FUNC_PROLOGUE();
	// li r3,116
	ctx.r3.s64 = 116;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E78D0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E78D0) { __imp__sub_826E78D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E78D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-31860
	ctx.r3.s64 = ctx.r11.s64 + -31860;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E78E0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E78E0) { __imp__sub_826E78E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E78E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-31824
	ctx.r3.s64 = ctx.r11.s64 + -31824;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E78F0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E78F0) { __imp__sub_826E78F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E78F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-31788
	ctx.r3.s64 = ctx.r11.s64 + -31788;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7900);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7900) { __imp__sub_826E7900(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7900) {
	PPC_FUNC_PROLOGUE();
	// li r3,60
	ctx.r3.s64 = 60;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7908);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7908) { __imp__sub_826E7908(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7908) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,6
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 6, ctx.xer);
	// bgt cr6,0x826e7980
	if (ctx.cr6.gt) goto loc_826E7980;
	// lis r12,-32146
	ctx.r12.s64 = -2106720256;
	// addi r12,r12,31016
	ctx.r12.s64 = ctx.r12.s64 + 31016;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826E7944;
	case 1:
		goto loc_826E794C;
	case 2:
		goto loc_826E7954;
	case 3:
		goto loc_826E795C;
	case 4:
		goto loc_826E7964;
	case 5:
		goto loc_826E796C;
	case 6:
		goto loc_826E7974;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E7944:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E794C:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E7954:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E795C:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E7964:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826E796C:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826E7974:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826E7980:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7988);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7988) { __imp__sub_826E7988(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7988) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-31752
	ctx.r3.s64 = ctx.r11.s64 + -31752;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7998);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7998) { __imp__sub_826E7998(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7998) {
	PPC_FUNC_PROLOGUE();
	// li r3,61
	ctx.r3.s64 = 61;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E79A0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E79A0) { __imp__sub_826E79A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E79A0) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e7908
	sub_826E7908(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E79A8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E79A8) { __imp__sub_826E79A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E79A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-31716
	ctx.r3.s64 = ctx.r11.s64 + -31716;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E79B8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E79B8) { __imp__sub_826E79B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E79B8) {
	PPC_FUNC_PROLOGUE();
	// li r3,62
	ctx.r3.s64 = 62;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E79C0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E79C0) { __imp__sub_826E79C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E79C0) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,6
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 6, ctx.xer);
	// bgt cr6,0x826e7a0c
	if (ctx.cr6.gt) goto loc_826E7A0C;
	// lis r12,-32146
	ctx.r12.s64 = -2106720256;
	// addi r12,r12,31200
	ctx.r12.s64 = ctx.r12.s64 + 31200;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826E79FC;
	case 1:
		goto loc_826E79FC;
	case 2:
		goto loc_826E7A04;
	case 3:
		goto loc_826E7A04;
	case 4:
		goto loc_826E79FC;
	case 5:
		goto loc_826E79FC;
	case 6:
		goto loc_826E7A04;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E79FC:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E7A04:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826E7A0C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7A18);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7A18) { __imp__sub_826E7A18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7A18) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-31680
	ctx.r3.s64 = ctx.r11.s64 + -31680;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7A28);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7A28) { __imp__sub_826E7A28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7A28) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-31644
	ctx.r3.s64 = ctx.r11.s64 + -31644;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7A38);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7A38) { __imp__sub_826E7A38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7A38) {
	PPC_FUNC_PROLOGUE();
	// li r3,70
	ctx.r3.s64 = 70;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7A40);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7A40) { __imp__sub_826E7A40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7A40) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,6
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 6, ctx.xer);
	// bgt cr6,0x826e7ab8
	if (ctx.cr6.gt) goto loc_826E7AB8;
	// lis r12,-32146
	ctx.r12.s64 = -2106720256;
	// addi r12,r12,31328
	ctx.r12.s64 = ctx.r12.s64 + 31328;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826E7A7C;
	case 1:
		goto loc_826E7A84;
	case 2:
		goto loc_826E7A8C;
	case 3:
		goto loc_826E7A94;
	case 4:
		goto loc_826E7A9C;
	case 5:
		goto loc_826E7AA8;
	case 6:
		goto loc_826E7AB0;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E7A7C:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E7A84:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E7A8C:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E7A94:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E7A9C:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826E7AA8:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826E7AB0:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826E7AB8:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7AC0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7AC0) { __imp__sub_826E7AC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7AC0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826e7b4c
	if (ctx.cr6.gt) goto loc_826E7B4C;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826e7b44
	if (ctx.cr6.eq) goto loc_826E7B44;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x826e7b4c
	if (ctx.cr6.gt) goto loc_826E7B4C;
	// lis r12,-32146
	ctx.r12.s64 = -2106720256;
	// addi r12,r12,31484
	ctx.r12.s64 = ctx.r12.s64 + 31484;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E7B14;
	case 1:
		goto loc_826E7B1C;
	case 2:
		goto loc_826E7B24;
	case 3:
		goto loc_826E7B2C;
	case 4:
		goto loc_826E7B34;
	case 5:
		goto loc_826E7B3C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E7B14:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E7B1C:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E7B24:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E7B2C:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E7B34:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826E7B3C:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826E7B44:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E7B4C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7B58);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7B58) { __imp__sub_826E7B58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7B58) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-31608
	ctx.r3.s64 = ctx.r11.s64 + -31608;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7B68);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7B68) { __imp__sub_826E7B68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7B68) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e7ac0
	sub_826E7AC0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7B70);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7B70) { __imp__sub_826E7B70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7B70) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e7a40
	sub_826E7A40(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7B78);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7B78) { __imp__sub_826E7B78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7B78) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,6
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 6, ctx.xer);
	// bgt cr6,0x826e7bc4
	if (ctx.cr6.gt) goto loc_826E7BC4;
	// lis r12,-32146
	ctx.r12.s64 = -2106720256;
	// addi r12,r12,31640
	ctx.r12.s64 = ctx.r12.s64 + 31640;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826E7BB4;
	case 1:
		goto loc_826E7BB4;
	case 2:
		goto loc_826E7BBC;
	case 3:
		goto loc_826E7BBC;
	case 4:
		goto loc_826E7BBC;
	case 5:
		goto loc_826E7BB4;
	case 6:
		goto loc_826E7BB4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E7BB4:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E7BBC:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826E7BC4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7BD0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7BD0) { __imp__sub_826E7BD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7BD0) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,6
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 6, ctx.xer);
	// bgt cr6,0x826e7c1c
	if (ctx.cr6.gt) goto loc_826E7C1C;
	// lis r12,-32146
	ctx.r12.s64 = -2106720256;
	// addi r12,r12,31728
	ctx.r12.s64 = ctx.r12.s64 + 31728;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826E7C0C;
	case 1:
		goto loc_826E7C0C;
	case 2:
		goto loc_826E7C14;
	case 3:
		goto loc_826E7C14;
	case 4:
		goto loc_826E7C14;
	case 5:
		goto loc_826E7C0C;
	case 6:
		goto loc_826E7C0C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E7C0C:
	// li r11,1
	ctx.r11.s64 = 1;
	// b 0x826e7c20
	goto loc_826E7C20;
loc_826E7C14:
	// li r11,2
	ctx.r11.s64 = 2;
	// b 0x826e7c20
	goto loc_826E7C20;
loc_826E7C1C:
	// li r11,-1
	ctx.r11.s64 = -1;
loc_826E7C20:
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826e7c78
	if (ctx.cr6.gt) goto loc_826E7C78;
	// lis r12,-32146
	ctx.r12.s64 = -2106720256;
	// addi r12,r12,31812
	ctx.r12.s64 = ctx.r12.s64 + 31812;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E7C58;
	case 1:
		goto loc_826E7C60;
	case 2:
		goto loc_826E7C70;
	case 3:
		goto loc_826E7C78;
	case 4:
		goto loc_826E7C68;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E7C58:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E7C60:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E7C68:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E7C70:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E7C78:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7C80);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7C80) { __imp__sub_826E7C80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7C80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-31572
	ctx.r3.s64 = ctx.r11.s64 + -31572;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7C90);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7C90) { __imp__sub_826E7C90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7C90) {
	PPC_FUNC_PROLOGUE();
	// li r3,117
	ctx.r3.s64 = 117;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7C98);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7C98) { __imp__sub_826E7C98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7C98) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-31536
	ctx.r3.s64 = ctx.r11.s64 + -31536;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7CA8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7CA8) { __imp__sub_826E7CA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7CA8) {
	PPC_FUNC_PROLOGUE();
	// li r3,118
	ctx.r3.s64 = 118;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7CB0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7CB0) { __imp__sub_826E7CB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7CB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-31500
	ctx.r3.s64 = ctx.r11.s64 + -31500;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7CC0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7CC0) { __imp__sub_826E7CC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7CC0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-31464
	ctx.r3.s64 = ctx.r11.s64 + -31464;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7CD0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7CD0) { __imp__sub_826E7CD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7CD0) {
	PPC_FUNC_PROLOGUE();
	// li r3,120
	ctx.r3.s64 = 120;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7CD8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7CD8) { __imp__sub_826E7CD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7CD8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-31428
	ctx.r3.s64 = ctx.r11.s64 + -31428;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7CE8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7CE8) { __imp__sub_826E7CE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7CE8) {
	PPC_FUNC_PROLOGUE();
	// li r3,121
	ctx.r3.s64 = 121;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7CF0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7CF0) { __imp__sub_826E7CF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7CF0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-31392
	ctx.r3.s64 = ctx.r11.s64 + -31392;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7D00);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7D00) { __imp__sub_826E7D00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7D00) {
	PPC_FUNC_PROLOGUE();
	// li r3,122
	ctx.r3.s64 = 122;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7D08);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7D08) { __imp__sub_826E7D08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7D08) {
	PPC_FUNC_PROLOGUE();
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7D18);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7D18) { __imp__sub_826E7D18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7D18) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-31356
	ctx.r3.s64 = ctx.r11.s64 + -31356;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7D28);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7D28) { __imp__sub_826E7D28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7D28) {
	PPC_FUNC_PROLOGUE();
	// li r3,123
	ctx.r3.s64 = 123;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7D30);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7D30) { __imp__sub_826E7D30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7D30) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-31320
	ctx.r3.s64 = ctx.r11.s64 + -31320;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7D40);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7D40) { __imp__sub_826E7D40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7D40) {
	PPC_FUNC_PROLOGUE();
	// li r3,124
	ctx.r3.s64 = 124;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7D48);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7D48) { __imp__sub_826E7D48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7D48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-31284
	ctx.r3.s64 = ctx.r11.s64 + -31284;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7D58);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7D58) { __imp__sub_826E7D58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7D58) {
	PPC_FUNC_PROLOGUE();
	// li r3,125
	ctx.r3.s64 = 125;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7D60);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7D60) { __imp__sub_826E7D60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7D60) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-31248
	ctx.r3.s64 = ctx.r11.s64 + -31248;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7D70);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7D70) { __imp__sub_826E7D70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7D70) {
	PPC_FUNC_PROLOGUE();
	// li r3,126
	ctx.r3.s64 = 126;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7D78);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7D78) { __imp__sub_826E7D78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7D78) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-31212
	ctx.r3.s64 = ctx.r11.s64 + -31212;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7D88);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7D88) { __imp__sub_826E7D88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7D88) {
	PPC_FUNC_PROLOGUE();
	// li r3,127
	ctx.r3.s64 = 127;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7D90);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7D90) { __imp__sub_826E7D90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7D90) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-31176
	ctx.r3.s64 = ctx.r11.s64 + -31176;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7DA0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7DA0) { __imp__sub_826E7DA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7DA0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-31140
	ctx.r3.s64 = ctx.r11.s64 + -31140;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7DB0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7DB0) { __imp__sub_826E7DB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7DB0) {
	PPC_FUNC_PROLOGUE();
	// li r3,25
	ctx.r3.s64 = 25;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7DB8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7DB8) { __imp__sub_826E7DB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7DB8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-31104
	ctx.r3.s64 = ctx.r11.s64 + -31104;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7DC8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7DC8) { __imp__sub_826E7DC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7DC8) {
	PPC_FUNC_PROLOGUE();
	// li r3,30
	ctx.r3.s64 = 30;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7DD0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7DD0) { __imp__sub_826E7DD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7DD0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-31068
	ctx.r3.s64 = ctx.r11.s64 + -31068;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7DE0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7DE0) { __imp__sub_826E7DE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7DE0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-31032
	ctx.r3.s64 = ctx.r11.s64 + -31032;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7DF0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7DF0) { __imp__sub_826E7DF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7DF0) {
	PPC_FUNC_PROLOGUE();
	// li r3,33
	ctx.r3.s64 = 33;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7DF8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7DF8) { __imp__sub_826E7DF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7DF8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30996
	ctx.r3.s64 = ctx.r11.s64 + -30996;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7E08);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7E08) { __imp__sub_826E7E08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7E08) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30960
	ctx.r3.s64 = ctx.r11.s64 + -30960;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7E18);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7E18) { __imp__sub_826E7E18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7E18) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30924
	ctx.r3.s64 = ctx.r11.s64 + -30924;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7E28);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7E28) { __imp__sub_826E7E28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7E28) {
	PPC_FUNC_PROLOGUE();
	// li r3,35
	ctx.r3.s64 = 35;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7E30);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7E30) { __imp__sub_826E7E30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7E30) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826e7e9c
	if (ctx.cr6.gt) goto loc_826E7E9C;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826e7e94
	if (ctx.cr6.eq) goto loc_826E7E94;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x826e7e9c
	if (ctx.cr6.gt) goto loc_826E7E9C;
	// lis r12,-32146
	ctx.r12.s64 = -2106720256;
	// addi r12,r12,32364
	ctx.r12.s64 = ctx.r12.s64 + 32364;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E7E84;
	case 1:
		goto loc_826E7E84;
	case 2:
		goto loc_826E7E8C;
	case 3:
		goto loc_826E7E84;
	case 4:
		goto loc_826E7E84;
	case 5:
		goto loc_826E7E94;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E7E84:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E7E8C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E7E94:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E7E9C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7EA8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7EA8) { __imp__sub_826E7EA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7EA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30888
	ctx.r3.s64 = ctx.r11.s64 + -30888;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7EB8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7EB8) { __imp__sub_826E7EB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7EB8) {
	PPC_FUNC_PROLOGUE();
	// li r3,53
	ctx.r3.s64 = 53;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7EC0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7EC0) { __imp__sub_826E7EC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7EC0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30852
	ctx.r3.s64 = ctx.r11.s64 + -30852;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7ED0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7ED0) { __imp__sub_826E7ED0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7ED0) {
	PPC_FUNC_PROLOGUE();
	// li r3,65
	ctx.r3.s64 = 65;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7ED8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7ED8) { __imp__sub_826E7ED8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7ED8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30816
	ctx.r3.s64 = ctx.r11.s64 + -30816;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7EE8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7EE8) { __imp__sub_826E7EE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7EE8) {
	PPC_FUNC_PROLOGUE();
	// li r3,57
	ctx.r3.s64 = 57;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7EF0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7EF0) { __imp__sub_826E7EF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7EF0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30780
	ctx.r3.s64 = ctx.r11.s64 + -30780;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7F00);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7F00) { __imp__sub_826E7F00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7F00) {
	PPC_FUNC_PROLOGUE();
	// li r3,66
	ctx.r3.s64 = 66;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7F08);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7F08) { __imp__sub_826E7F08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7F08) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30744
	ctx.r3.s64 = ctx.r11.s64 + -30744;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7F18);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7F18) { __imp__sub_826E7F18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7F18) {
	PPC_FUNC_PROLOGUE();
	// li r3,67
	ctx.r3.s64 = 67;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7F20);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7F20) { __imp__sub_826E7F20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7F20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30708
	ctx.r3.s64 = ctx.r11.s64 + -30708;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7F30);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7F30) { __imp__sub_826E7F30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7F30) {
	PPC_FUNC_PROLOGUE();
	// li r3,68
	ctx.r3.s64 = 68;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7F38);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7F38) { __imp__sub_826E7F38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7F38) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30672
	ctx.r3.s64 = ctx.r11.s64 + -30672;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7F48);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7F48) { __imp__sub_826E7F48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7F48) {
	PPC_FUNC_PROLOGUE();
	// li r3,72
	ctx.r3.s64 = 72;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7F50);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7F50) { __imp__sub_826E7F50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7F50) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30636
	ctx.r3.s64 = ctx.r11.s64 + -30636;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7F60);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7F60) { __imp__sub_826E7F60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7F60) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30600
	ctx.r3.s64 = ctx.r11.s64 + -30600;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7F70);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7F70) { __imp__sub_826E7F70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7F70) {
	PPC_FUNC_PROLOGUE();
	// li r3,74
	ctx.r3.s64 = 74;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7F78);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7F78) { __imp__sub_826E7F78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7F78) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30564
	ctx.r3.s64 = ctx.r11.s64 + -30564;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7F88);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7F88) { __imp__sub_826E7F88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7F88) {
	PPC_FUNC_PROLOGUE();
	// li r3,76
	ctx.r3.s64 = 76;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7F90);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7F90) { __imp__sub_826E7F90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7F90) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30528
	ctx.r3.s64 = ctx.r11.s64 + -30528;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7FA0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7FA0) { __imp__sub_826E7FA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7FA0) {
	PPC_FUNC_PROLOGUE();
	// li r3,79
	ctx.r3.s64 = 79;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7FA8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7FA8) { __imp__sub_826E7FA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7FA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30492
	ctx.r3.s64 = ctx.r11.s64 + -30492;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7FB8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7FB8) { __imp__sub_826E7FB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7FB8) {
	PPC_FUNC_PROLOGUE();
	// li r3,80
	ctx.r3.s64 = 80;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7FC0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7FC0) { __imp__sub_826E7FC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7FC0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30456
	ctx.r3.s64 = ctx.r11.s64 + -30456;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7FD0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7FD0) { __imp__sub_826E7FD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7FD0) {
	PPC_FUNC_PROLOGUE();
	// li r3,81
	ctx.r3.s64 = 81;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7FD8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7FD8) { __imp__sub_826E7FD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7FD8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30420
	ctx.r3.s64 = ctx.r11.s64 + -30420;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7FE8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7FE8) { __imp__sub_826E7FE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7FE8) {
	PPC_FUNC_PROLOGUE();
	// li r3,82
	ctx.r3.s64 = 82;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E7FF0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E7FF0) { __imp__sub_826E7FF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E7FF0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30384
	ctx.r3.s64 = ctx.r11.s64 + -30384;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8000);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8000) { __imp__sub_826E8000(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8000) {
	PPC_FUNC_PROLOGUE();
	// li r3,83
	ctx.r3.s64 = 83;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8008);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8008) { __imp__sub_826E8008(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8008) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30348
	ctx.r3.s64 = ctx.r11.s64 + -30348;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8018);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8018) { __imp__sub_826E8018(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8018) {
	PPC_FUNC_PROLOGUE();
	// li r3,84
	ctx.r3.s64 = 84;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8020);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8020) { __imp__sub_826E8020(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8020) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30312
	ctx.r3.s64 = ctx.r11.s64 + -30312;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8030);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8030) { __imp__sub_826E8030(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8030) {
	PPC_FUNC_PROLOGUE();
	// li r3,85
	ctx.r3.s64 = 85;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8038);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8038) { __imp__sub_826E8038(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8038) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30276
	ctx.r3.s64 = ctx.r11.s64 + -30276;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8048);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8048) { __imp__sub_826E8048(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8048) {
	PPC_FUNC_PROLOGUE();
	// li r3,86
	ctx.r3.s64 = 86;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8050);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8050) { __imp__sub_826E8050(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8050) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30240
	ctx.r3.s64 = ctx.r11.s64 + -30240;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8060);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8060) { __imp__sub_826E8060(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8060) {
	PPC_FUNC_PROLOGUE();
	// li r3,87
	ctx.r3.s64 = 87;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8068);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8068) { __imp__sub_826E8068(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8068) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30204
	ctx.r3.s64 = ctx.r11.s64 + -30204;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8078);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8078) { __imp__sub_826E8078(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8078) {
	PPC_FUNC_PROLOGUE();
	// li r3,88
	ctx.r3.s64 = 88;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8080);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8080) { __imp__sub_826E8080(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8080) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30168
	ctx.r3.s64 = ctx.r11.s64 + -30168;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8090);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8090) { __imp__sub_826E8090(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8090) {
	PPC_FUNC_PROLOGUE();
	// li r3,89
	ctx.r3.s64 = 89;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8098);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8098) { __imp__sub_826E8098(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8098) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30132
	ctx.r3.s64 = ctx.r11.s64 + -30132;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E80A8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E80A8) { __imp__sub_826E80A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E80A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30096
	ctx.r3.s64 = ctx.r11.s64 + -30096;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E80B8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E80B8) { __imp__sub_826E80B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E80B8) {
	PPC_FUNC_PROLOGUE();
	// li r3,91
	ctx.r3.s64 = 91;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E80C0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E80C0) { __imp__sub_826E80C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E80C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30060
	ctx.r3.s64 = ctx.r11.s64 + -30060;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E80D0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E80D0) { __imp__sub_826E80D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E80D0) {
	PPC_FUNC_PROLOGUE();
	// li r3,92
	ctx.r3.s64 = 92;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E80D8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E80D8) { __imp__sub_826E80D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E80D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-30024
	ctx.r3.s64 = ctx.r11.s64 + -30024;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E80E8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E80E8) { __imp__sub_826E80E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E80E8) {
	PPC_FUNC_PROLOGUE();
	// li r3,93
	ctx.r3.s64 = 93;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E80F0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E80F0) { __imp__sub_826E80F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E80F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29988
	ctx.r3.s64 = ctx.r11.s64 + -29988;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8100);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8100) { __imp__sub_826E8100(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8100) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29952
	ctx.r3.s64 = ctx.r11.s64 + -29952;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8110);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8110) { __imp__sub_826E8110(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8110) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29916
	ctx.r3.s64 = ctx.r11.s64 + -29916;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8120);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8120) { __imp__sub_826E8120(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8120) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29880
	ctx.r3.s64 = ctx.r11.s64 + -29880;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8130);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8130) { __imp__sub_826E8130(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8130) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29844
	ctx.r3.s64 = ctx.r11.s64 + -29844;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8140);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8140) { __imp__sub_826E8140(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8140) {
	PPC_FUNC_PROLOGUE();
	// li r3,20
	ctx.r3.s64 = 20;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8148);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8148) { __imp__sub_826E8148(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8148) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29808
	ctx.r3.s64 = ctx.r11.s64 + -29808;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8158);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8158) { __imp__sub_826E8158(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8158) {
	PPC_FUNC_PROLOGUE();
	// li r3,26
	ctx.r3.s64 = 26;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8160);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8160) { __imp__sub_826E8160(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8160) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29772
	ctx.r3.s64 = ctx.r11.s64 + -29772;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8170);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8170) { __imp__sub_826E8170(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8170) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29736
	ctx.r3.s64 = ctx.r11.s64 + -29736;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8180);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8180) { __imp__sub_826E8180(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8180) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29700
	ctx.r3.s64 = ctx.r11.s64 + -29700;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8190);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8190) { __imp__sub_826E8190(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8190) {
	PPC_FUNC_PROLOGUE();
	// li r3,37
	ctx.r3.s64 = 37;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8198);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8198) { __imp__sub_826E8198(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8198) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29664
	ctx.r3.s64 = ctx.r11.s64 + -29664;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E81A8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E81A8) { __imp__sub_826E81A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E81A8) {
	PPC_FUNC_PROLOGUE();
	// li r3,42
	ctx.r3.s64 = 42;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E81B0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E81B0) { __imp__sub_826E81B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E81B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29628
	ctx.r3.s64 = ctx.r11.s64 + -29628;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E81C0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E81C0) { __imp__sub_826E81C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E81C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29592
	ctx.r3.s64 = ctx.r11.s64 + -29592;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E81D0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E81D0) { __imp__sub_826E81D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E81D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29556
	ctx.r3.s64 = ctx.r11.s64 + -29556;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E81E0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E81E0) { __imp__sub_826E81E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E81E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29520
	ctx.r3.s64 = ctx.r11.s64 + -29520;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E81F0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E81F0) { __imp__sub_826E81F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E81F0) {
	PPC_FUNC_PROLOGUE();
	// li r3,59
	ctx.r3.s64 = 59;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E81F8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E81F8) { __imp__sub_826E81F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E81F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29484
	ctx.r3.s64 = ctx.r11.s64 + -29484;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8208);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8208) { __imp__sub_826E8208(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8208) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29448
	ctx.r3.s64 = ctx.r11.s64 + -29448;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8218);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8218) { __imp__sub_826E8218(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8218) {
	PPC_FUNC_PROLOGUE();
	// li r3,128
	ctx.r3.s64 = 128;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8220);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8220) { __imp__sub_826E8220(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8220) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r4,3
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 3, ctx.xer);
	// bgt cr6,0x826e8254
	if (ctx.cr6.gt) goto loc_826E8254;
	// beq cr6,0x826e824c
	if (ctx.cr6.eq) goto loc_826E824C;
	// cmpwi cr6,r4,1
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 1, ctx.xer);
	// beq cr6,0x826e8244
	if (ctx.cr6.eq) goto loc_826E8244;
	// cmpwi cr6,r4,2
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 2, ctx.xer);
	// bne cr6,0x826e8264
	if (!ctx.cr6.eq) goto loc_826E8264;
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E8244:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E824C:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E8254:
	// cmpwi cr6,r4,4
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 4, ctx.xer);
	// beq cr6,0x826e8274
	if (ctx.cr6.eq) goto loc_826E8274;
	// cmplwi cr6,r4,65534
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 65534, ctx.xer);
	// beq cr6,0x826e826c
	if (ctx.cr6.eq) goto loc_826E826C;
loc_826E8264:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826E826C:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E8274:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8280);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8280) { __imp__sub_826E8280(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8280) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,4
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 4, ctx.xer);
	// bgt cr6,0x826e82e0
	if (ctx.cr6.gt) goto loc_826E82E0;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-32096
	ctx.r12.s64 = ctx.r12.s64 + -32096;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826E82B4;
	case 1:
		goto loc_826E82BC;
	case 2:
		goto loc_826E82C4;
	case 3:
		goto loc_826E82CC;
	case 4:
		goto loc_826E82D8;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E82B4:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E82BC:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E82C4:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E82CC:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826E82D8:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E82E0:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E82E8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E82E8) { __imp__sub_826E82E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E82E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29412
	ctx.r3.s64 = ctx.r11.s64 + -29412;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E82F8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E82F8) { __imp__sub_826E82F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E82F8) {
	PPC_FUNC_PROLOGUE();
	// li r3,129
	ctx.r3.s64 = 129;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8300);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8300) { __imp__sub_826E8300(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8300) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29376
	ctx.r3.s64 = ctx.r11.s64 + -29376;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8310);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8310) { __imp__sub_826E8310(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8310) {
	PPC_FUNC_PROLOGUE();
	// li r3,143
	ctx.r3.s64 = 143;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8318);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8318) { __imp__sub_826E8318(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8318) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29340
	ctx.r3.s64 = ctx.r11.s64 + -29340;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8328);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8328) { __imp__sub_826E8328(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8328) {
	PPC_FUNC_PROLOGUE();
	// li r3,144
	ctx.r3.s64 = 144;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8330);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8330) { __imp__sub_826E8330(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8330) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29304
	ctx.r3.s64 = ctx.r11.s64 + -29304;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8340);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8340) { __imp__sub_826E8340(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8340) {
	PPC_FUNC_PROLOGUE();
	// li r3,148
	ctx.r3.s64 = 148;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8348);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8348) { __imp__sub_826E8348(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8348) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29268
	ctx.r3.s64 = ctx.r11.s64 + -29268;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8358);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8358) { __imp__sub_826E8358(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8358) {
	PPC_FUNC_PROLOGUE();
	// li r3,136
	ctx.r3.s64 = 136;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8360);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8360) { __imp__sub_826E8360(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8360) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29232
	ctx.r3.s64 = ctx.r11.s64 + -29232;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8370);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8370) { __imp__sub_826E8370(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8370) {
	PPC_FUNC_PROLOGUE();
	// li r3,132
	ctx.r3.s64 = 132;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8378);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8378) { __imp__sub_826E8378(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8378) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29196
	ctx.r3.s64 = ctx.r11.s64 + -29196;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8388);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8388) { __imp__sub_826E8388(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8388) {
	PPC_FUNC_PROLOGUE();
	// li r3,154
	ctx.r3.s64 = 154;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8390);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8390) { __imp__sub_826E8390(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8390) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29160
	ctx.r3.s64 = ctx.r11.s64 + -29160;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E83A0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E83A0) { __imp__sub_826E83A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E83A0) {
	PPC_FUNC_PROLOGUE();
	// li r3,151
	ctx.r3.s64 = 151;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E83A8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E83A8) { __imp__sub_826E83A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E83A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29124
	ctx.r3.s64 = ctx.r11.s64 + -29124;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E83B8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E83B8) { __imp__sub_826E83B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E83B8) {
	PPC_FUNC_PROLOGUE();
	// li r3,152
	ctx.r3.s64 = 152;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E83C0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E83C0) { __imp__sub_826E83C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E83C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29088
	ctx.r3.s64 = ctx.r11.s64 + -29088;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E83D0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E83D0) { __imp__sub_826E83D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E83D0) {
	PPC_FUNC_PROLOGUE();
	// li r3,156
	ctx.r3.s64 = 156;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E83D8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E83D8) { __imp__sub_826E83D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E83D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29052
	ctx.r3.s64 = ctx.r11.s64 + -29052;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E83E8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E83E8) { __imp__sub_826E83E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E83E8) {
	PPC_FUNC_PROLOGUE();
	// li r3,134
	ctx.r3.s64 = 134;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E83F0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E83F0) { __imp__sub_826E83F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E83F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-29016
	ctx.r3.s64 = ctx.r11.s64 + -29016;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8400);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8400) { __imp__sub_826E8400(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8400) {
	PPC_FUNC_PROLOGUE();
	// li r3,137
	ctx.r3.s64 = 137;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8408);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8408) { __imp__sub_826E8408(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8408) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28980
	ctx.r3.s64 = ctx.r11.s64 + -28980;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8418);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8418) { __imp__sub_826E8418(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8418) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28944
	ctx.r3.s64 = ctx.r11.s64 + -28944;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8428);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8428) { __imp__sub_826E8428(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8428) {
	PPC_FUNC_PROLOGUE();
	// li r3,135
	ctx.r3.s64 = 135;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8430);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8430) { __imp__sub_826E8430(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8430) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r4,3
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 3, ctx.xer);
	// bgt cr6,0x826e8464
	if (ctx.cr6.gt) goto loc_826E8464;
	// beq cr6,0x826e845c
	if (ctx.cr6.eq) goto loc_826E845C;
	// cmpwi cr6,r4,1
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 1, ctx.xer);
	// beq cr6,0x826e8454
	if (ctx.cr6.eq) goto loc_826E8454;
	// cmpwi cr6,r4,2
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 2, ctx.xer);
	// bne cr6,0x826e8474
	if (!ctx.cr6.eq) goto loc_826E8474;
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E8454:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E845C:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E8464:
	// cmpwi cr6,r4,4
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 4, ctx.xer);
	// beq cr6,0x826e8484
	if (ctx.cr6.eq) goto loc_826E8484;
	// cmplwi cr6,r4,65534
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 65534, ctx.xer);
	// beq cr6,0x826e847c
	if (ctx.cr6.eq) goto loc_826E847C;
loc_826E8474:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826E847C:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E8484:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8490);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8490) { __imp__sub_826E8490(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8490) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28908
	ctx.r3.s64 = ctx.r11.s64 + -28908;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E84A0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E84A0) { __imp__sub_826E84A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E84A0) {
	PPC_FUNC_PROLOGUE();
	// li r3,140
	ctx.r3.s64 = 140;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E84A8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E84A8) { __imp__sub_826E84A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E84A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28872
	ctx.r3.s64 = ctx.r11.s64 + -28872;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E84B8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E84B8) { __imp__sub_826E84B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E84B8) {
	PPC_FUNC_PROLOGUE();
	// li r3,141
	ctx.r3.s64 = 141;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E84C0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E84C0) { __imp__sub_826E84C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E84C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28836
	ctx.r3.s64 = ctx.r11.s64 + -28836;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E84D0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E84D0) { __imp__sub_826E84D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E84D0) {
	PPC_FUNC_PROLOGUE();
	// li r3,146
	ctx.r3.s64 = 146;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E84D8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E84D8) { __imp__sub_826E84D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E84D8) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,4
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 4, ctx.xer);
	// bgt cr6,0x826e851c
	if (ctx.cr6.gt) goto loc_826E851C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-31496
	ctx.r12.s64 = ctx.r12.s64 + -31496;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826E850C;
	case 1:
		goto loc_826E850C;
	case 2:
		goto loc_826E8514;
	case 3:
		goto loc_826E8514;
	case 4:
		goto loc_826E8514;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E850C:
	// li r11,1
	ctx.r11.s64 = 1;
	// b 0x826e8520
	goto loc_826E8520;
loc_826E8514:
	// li r11,2
	ctx.r11.s64 = 2;
	// b 0x826e8520
	goto loc_826E8520;
loc_826E851C:
	// li r11,-1
	ctx.r11.s64 = -1;
loc_826E8520:
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826e8578
	if (ctx.cr6.gt) goto loc_826E8578;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-31420
	ctx.r12.s64 = ctx.r12.s64 + -31420;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E8558;
	case 1:
		goto loc_826E8560;
	case 2:
		goto loc_826E8570;
	case 3:
		goto loc_826E8578;
	case 4:
		goto loc_826E8568;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E8558:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E8560:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E8568:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E8570:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E8578:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8580);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8580) { __imp__sub_826E8580(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8580) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28800
	ctx.r3.s64 = ctx.r11.s64 + -28800;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8590);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8590) { __imp__sub_826E8590(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8590) {
	PPC_FUNC_PROLOGUE();
	// li r3,153
	ctx.r3.s64 = 153;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8598);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8598) { __imp__sub_826E8598(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8598) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,4
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 4, ctx.xer);
	// bgt cr6,0x826e85f8
	if (ctx.cr6.gt) goto loc_826E85F8;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-31304
	ctx.r12.s64 = ctx.r12.s64 + -31304;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826E85CC;
	case 1:
		goto loc_826E85D4;
	case 2:
		goto loc_826E85DC;
	case 3:
		goto loc_826E85E4;
	case 4:
		goto loc_826E85EC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E85CC:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E85D4:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E85DC:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E85E4:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E85EC:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826E85F8:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8600);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8600) { __imp__sub_826E8600(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8600) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28764
	ctx.r3.s64 = ctx.r11.s64 + -28764;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8610);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8610) { __imp__sub_826E8610(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8610) {
	PPC_FUNC_PROLOGUE();
	// li r3,142
	ctx.r3.s64 = 142;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8618);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8618) { __imp__sub_826E8618(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8618) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28728
	ctx.r3.s64 = ctx.r11.s64 + -28728;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8628);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8628) { __imp__sub_826E8628(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8628) {
	PPC_FUNC_PROLOGUE();
	// li r3,145
	ctx.r3.s64 = 145;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8630);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8630) { __imp__sub_826E8630(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8630) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28692
	ctx.r3.s64 = ctx.r11.s64 + -28692;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8640);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8640) { __imp__sub_826E8640(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8640) {
	PPC_FUNC_PROLOGUE();
	// li r3,131
	ctx.r3.s64 = 131;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8648);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8648) { __imp__sub_826E8648(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8648) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28656
	ctx.r3.s64 = ctx.r11.s64 + -28656;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8658);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8658) { __imp__sub_826E8658(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8658) {
	PPC_FUNC_PROLOGUE();
	// li r3,138
	ctx.r3.s64 = 138;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8660);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8660) { __imp__sub_826E8660(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8660) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28620
	ctx.r3.s64 = ctx.r11.s64 + -28620;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8670);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8670) { __imp__sub_826E8670(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8670) {
	PPC_FUNC_PROLOGUE();
	// li r3,149
	ctx.r3.s64 = 149;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8678);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8678) { __imp__sub_826E8678(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8678) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28584
	ctx.r3.s64 = ctx.r11.s64 + -28584;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8688);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8688) { __imp__sub_826E8688(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8688) {
	PPC_FUNC_PROLOGUE();
	// li r3,147
	ctx.r3.s64 = 147;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8690);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8690) { __imp__sub_826E8690(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8690) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28548
	ctx.r3.s64 = ctx.r11.s64 + -28548;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E86A0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E86A0) { __imp__sub_826E86A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E86A0) {
	PPC_FUNC_PROLOGUE();
	// li r3,130
	ctx.r3.s64 = 130;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E86A8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E86A8) { __imp__sub_826E86A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E86A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28512
	ctx.r3.s64 = ctx.r11.s64 + -28512;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E86B8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E86B8) { __imp__sub_826E86B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E86B8) {
	PPC_FUNC_PROLOGUE();
	// li r3,133
	ctx.r3.s64 = 133;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E86C0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E86C0) { __imp__sub_826E86C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E86C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28476
	ctx.r3.s64 = ctx.r11.s64 + -28476;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E86D0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E86D0) { __imp__sub_826E86D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E86D0) {
	PPC_FUNC_PROLOGUE();
	// li r3,155
	ctx.r3.s64 = 155;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E86D8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E86D8) { __imp__sub_826E86D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E86D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28440
	ctx.r3.s64 = ctx.r11.s64 + -28440;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E86E8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E86E8) { __imp__sub_826E86E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E86E8) {
	PPC_FUNC_PROLOGUE();
	// li r3,150
	ctx.r3.s64 = 150;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E86F0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E86F0) { __imp__sub_826E86F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E86F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28404
	ctx.r3.s64 = ctx.r11.s64 + -28404;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8700);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8700) { __imp__sub_826E8700(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8700) {
	PPC_FUNC_PROLOGUE();
	// li r3,157
	ctx.r3.s64 = 157;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8708);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8708) { __imp__sub_826E8708(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8708) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,6
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 6, ctx.xer);
	// bgt cr6,0x826e8780
	if (ctx.cr6.gt) goto loc_826E8780;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-30936
	ctx.r12.s64 = ctx.r12.s64 + -30936;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826E8744;
	case 1:
		goto loc_826E874C;
	case 2:
		goto loc_826E8754;
	case 3:
		goto loc_826E875C;
	case 4:
		goto loc_826E8764;
	case 5:
		goto loc_826E876C;
	case 6:
		goto loc_826E8774;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E8744:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E874C:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E8754:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826E875C:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826E8764:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E876C:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E8774:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826E8780:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8788);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8788) { __imp__sub_826E8788(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8788) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826e8818
	if (ctx.cr6.gt) goto loc_826E8818;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826e8810
	if (ctx.cr6.eq) goto loc_826E8810;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,6
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 6, ctx.xer);
	// bgt cr6,0x826e8818
	if (ctx.cr6.gt) goto loc_826E8818;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-30780
	ctx.r12.s64 = ctx.r12.s64 + -30780;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E87E8;
	case 1:
		goto loc_826E8800;
	case 2:
		goto loc_826E87E0;
	case 3:
		goto loc_826E8808;
	case 4:
		goto loc_826E87F8;
	case 5:
		goto loc_826E8818;
	case 6:
		goto loc_826E87F0;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E87E0:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E87E8:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E87F0:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E87F8:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E8800:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E8808:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826E8810:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826E8818:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8820);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8820) { __imp__sub_826E8820(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8820) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28368
	ctx.r3.s64 = ctx.r11.s64 + -28368;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8830);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8830) { __imp__sub_826E8830(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8830) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e8788
	sub_826E8788(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8838);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8838) { __imp__sub_826E8838(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8838) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e8708
	sub_826E8708(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8840);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8840) { __imp__sub_826E8840(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8840) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,6
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 6, ctx.xer);
	// bgt cr6,0x826e888c
	if (ctx.cr6.gt) goto loc_826E888C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-30624
	ctx.r12.s64 = ctx.r12.s64 + -30624;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826E887C;
	case 1:
		goto loc_826E887C;
	case 2:
		goto loc_826E8884;
	case 3:
		goto loc_826E887C;
	case 4:
		goto loc_826E887C;
	case 5:
		goto loc_826E887C;
	case 6:
		goto loc_826E8884;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E887C:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E8884:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826E888C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8898);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8898) { __imp__sub_826E8898(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8898) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,6
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 6, ctx.xer);
	// bgt cr6,0x826e88e4
	if (ctx.cr6.gt) goto loc_826E88E4;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-30536
	ctx.r12.s64 = ctx.r12.s64 + -30536;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826E88D4;
	case 1:
		goto loc_826E88D4;
	case 2:
		goto loc_826E88DC;
	case 3:
		goto loc_826E88D4;
	case 4:
		goto loc_826E88D4;
	case 5:
		goto loc_826E88D4;
	case 6:
		goto loc_826E88DC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E88D4:
	// li r11,1
	ctx.r11.s64 = 1;
	// b 0x826e88e8
	goto loc_826E88E8;
loc_826E88DC:
	// li r11,2
	ctx.r11.s64 = 2;
	// b 0x826e88e8
	goto loc_826E88E8;
loc_826E88E4:
	// li r11,-1
	ctx.r11.s64 = -1;
loc_826E88E8:
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826e8940
	if (ctx.cr6.gt) goto loc_826E8940;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-30452
	ctx.r12.s64 = ctx.r12.s64 + -30452;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E8920;
	case 1:
		goto loc_826E8928;
	case 2:
		goto loc_826E8938;
	case 3:
		goto loc_826E8940;
	case 4:
		goto loc_826E8930;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E8920:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E8928:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E8930:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E8938:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E8940:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8948);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8948) { __imp__sub_826E8948(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8948) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28336
	ctx.r3.s64 = ctx.r11.s64 + -28336;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8958);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8958) { __imp__sub_826E8958(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8958) {
	PPC_FUNC_PROLOGUE();
	// li r3,32
	ctx.r3.s64 = 32;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8960);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8960) { __imp__sub_826E8960(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8960) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,4
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 4, ctx.xer);
	// bgt cr6,0x826e89c0
	if (ctx.cr6.gt) goto loc_826E89C0;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-30336
	ctx.r12.s64 = ctx.r12.s64 + -30336;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826E8994;
	case 1:
		goto loc_826E899C;
	case 2:
		goto loc_826E89A4;
	case 3:
		goto loc_826E89AC;
	case 4:
		goto loc_826E89B4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E8994:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E899C:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826E89A4:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E89AC:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E89B4:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826E89C0:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E89C8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E89C8) { __imp__sub_826E89C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E89C8) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,5
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 5, ctx.xer);
	// bgt cr6,0x826e8a34
	if (ctx.cr6.gt) goto loc_826E8A34;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-30232
	ctx.r12.s64 = ctx.r12.s64 + -30232;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826E8A00;
	case 1:
		goto loc_826E8A08;
	case 2:
		goto loc_826E8A10;
	case 3:
		goto loc_826E8A18;
	case 4:
		goto loc_826E8A20;
	case 5:
		goto loc_826E8A28;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E8A00:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E8A08:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E8A10:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826E8A18:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E8A20:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E8A28:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826E8A34:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8A40);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8A40) { __imp__sub_826E8A40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8A40) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826e8ac0
	if (ctx.cr6.gt) goto loc_826E8AC0;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826e8ab8
	if (ctx.cr6.eq) goto loc_826E8AB8;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826e8ac0
	if (ctx.cr6.gt) goto loc_826E8AC0;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-30084
	ctx.r12.s64 = ctx.r12.s64 + -30084;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E8A90;
	case 1:
		goto loc_826E8AA8;
	case 2:
		goto loc_826E8A98;
	case 3:
		goto loc_826E8AB0;
	case 4:
		goto loc_826E8AA0;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E8A90:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E8A98:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E8AA0:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E8AA8:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E8AB0:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E8AB8:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826E8AC0:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8AC8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8AC8) { __imp__sub_826E8AC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8AC8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28300
	ctx.r3.s64 = ctx.r11.s64 + -28300;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8AD8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8AD8) { __imp__sub_826E8AD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8AD8) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e8a40
	sub_826E8A40(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8AE0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8AE0) { __imp__sub_826E8AE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8AE0) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e89c8
	sub_826E89C8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8AE8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8AE8) { __imp__sub_826E8AE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8AE8) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,5
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 5, ctx.xer);
	// bgt cr6,0x826e8b30
	if (ctx.cr6.gt) goto loc_826E8B30;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-29944
	ctx.r12.s64 = ctx.r12.s64 + -29944;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826E8B20;
	case 1:
		goto loc_826E8B20;
	case 2:
		goto loc_826E8B28;
	case 3:
		goto loc_826E8B20;
	case 4:
		goto loc_826E8B20;
	case 5:
		goto loc_826E8B28;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E8B20:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E8B28:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826E8B30:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8B38);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8B38) { __imp__sub_826E8B38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8B38) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,5
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 5, ctx.xer);
	// bgt cr6,0x826e8b80
	if (ctx.cr6.gt) goto loc_826E8B80;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-29864
	ctx.r12.s64 = ctx.r12.s64 + -29864;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826E8B70;
	case 1:
		goto loc_826E8B70;
	case 2:
		goto loc_826E8B78;
	case 3:
		goto loc_826E8B70;
	case 4:
		goto loc_826E8B70;
	case 5:
		goto loc_826E8B78;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E8B70:
	// li r11,1
	ctx.r11.s64 = 1;
	// b 0x826e8b84
	goto loc_826E8B84;
loc_826E8B78:
	// li r11,2
	ctx.r11.s64 = 2;
	// b 0x826e8b84
	goto loc_826E8B84;
loc_826E8B80:
	// li r11,-1
	ctx.r11.s64 = -1;
loc_826E8B84:
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826e8bdc
	if (ctx.cr6.gt) goto loc_826E8BDC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-29784
	ctx.r12.s64 = ctx.r12.s64 + -29784;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E8BBC;
	case 1:
		goto loc_826E8BC4;
	case 2:
		goto loc_826E8BD4;
	case 3:
		goto loc_826E8BDC;
	case 4:
		goto loc_826E8BCC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E8BBC:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E8BC4:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E8BCC:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E8BD4:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E8BDC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8BE8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8BE8) { __imp__sub_826E8BE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8BE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28264
	ctx.r3.s64 = ctx.r11.s64 + -28264;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8BF8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8BF8) { __imp__sub_826E8BF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8BF8) {
	PPC_FUNC_PROLOGUE();
	// li r3,23
	ctx.r3.s64 = 23;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8C00);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8C00) { __imp__sub_826E8C00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8C00) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r4,3
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 3, ctx.xer);
	// bgt cr6,0x826e8c34
	if (ctx.cr6.gt) goto loc_826E8C34;
	// beq cr6,0x826e8c2c
	if (ctx.cr6.eq) goto loc_826E8C2C;
	// cmpwi cr6,r4,1
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 1, ctx.xer);
	// beq cr6,0x826e8c24
	if (ctx.cr6.eq) goto loc_826E8C24;
	// cmpwi cr6,r4,2
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 2, ctx.xer);
	// bne cr6,0x826e8c3c
	if (!ctx.cr6.eq) goto loc_826E8C3C;
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E8C24:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E8C2C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E8C34:
	// cmplwi cr6,r4,65534
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 65534, ctx.xer);
	// beq cr6,0x826e8c44
	if (ctx.cr6.eq) goto loc_826E8C44;
loc_826E8C3C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826E8C44:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8C50);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8C50) { __imp__sub_826E8C50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8C50) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,3
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 3, ctx.xer);
	// bgt cr6,0x826e8ca4
	if (ctx.cr6.gt) goto loc_826E8CA4;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-29584
	ctx.r12.s64 = ctx.r12.s64 + -29584;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826E8C80;
	case 1:
		goto loc_826E8C88;
	case 2:
		goto loc_826E8C90;
	case 3:
		goto loc_826E8C98;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E8C80:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E8C88:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E8C90:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E8C98:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826E8CA4:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8CB0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8CB0) { __imp__sub_826E8CB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8CB0) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,3
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 3, ctx.xer);
	// bgt cr6,0x826e8cf0
	if (ctx.cr6.gt) goto loc_826E8CF0;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-29488
	ctx.r12.s64 = ctx.r12.s64 + -29488;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826E8CE0;
	case 1:
		goto loc_826E8CE8;
	case 2:
		goto loc_826E8CE8;
	case 3:
		goto loc_826E8CE0;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E8CE0:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826E8CE8:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E8CF0:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8CF8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8CF8) { __imp__sub_826E8CF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8CF8) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,3
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 3, ctx.xer);
	// bgt cr6,0x826e8d38
	if (ctx.cr6.gt) goto loc_826E8D38;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-29416
	ctx.r12.s64 = ctx.r12.s64 + -29416;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826E8D28;
	case 1:
		goto loc_826E8D30;
	case 2:
		goto loc_826E8D30;
	case 3:
		goto loc_826E8D28;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E8D28:
	// li r11,2
	ctx.r11.s64 = 2;
	// b 0x826e8d3c
	goto loc_826E8D3C;
loc_826E8D30:
	// li r11,1
	ctx.r11.s64 = 1;
	// b 0x826e8d3c
	goto loc_826E8D3C;
loc_826E8D38:
	// li r11,-1
	ctx.r11.s64 = -1;
loc_826E8D3C:
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826e8d94
	if (ctx.cr6.gt) goto loc_826E8D94;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-29344
	ctx.r12.s64 = ctx.r12.s64 + -29344;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E8D74;
	case 1:
		goto loc_826E8D7C;
	case 2:
		goto loc_826E8D8C;
	case 3:
		goto loc_826E8D94;
	case 4:
		goto loc_826E8D84;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E8D74:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E8D7C:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E8D84:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E8D8C:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E8D94:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8DA0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8DA0) { __imp__sub_826E8DA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8DA0) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,6
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 6, ctx.xer);
	// bgt cr6,0x826e8e18
	if (ctx.cr6.gt) goto loc_826E8E18;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-29248
	ctx.r12.s64 = ctx.r12.s64 + -29248;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826E8DDC;
	case 1:
		goto loc_826E8DE4;
	case 2:
		goto loc_826E8DEC;
	case 3:
		goto loc_826E8DF4;
	case 4:
		goto loc_826E8DFC;
	case 5:
		goto loc_826E8E04;
	case 6:
		goto loc_826E8E0C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E8DDC:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E8DE4:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E8DEC:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826E8DF4:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826E8DFC:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E8E04:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E8E0C:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826E8E18:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8E20);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8E20) { __imp__sub_826E8E20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8E20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826e8eac
	if (ctx.cr6.gt) goto loc_826E8EAC;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826e8ea4
	if (ctx.cr6.eq) goto loc_826E8EA4;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x826e8eac
	if (ctx.cr6.gt) goto loc_826E8EAC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-29092
	ctx.r12.s64 = ctx.r12.s64 + -29092;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E8E74;
	case 1:
		goto loc_826E8E94;
	case 2:
		goto loc_826E8E7C;
	case 3:
		goto loc_826E8E9C;
	case 4:
		goto loc_826E8E8C;
	case 5:
		goto loc_826E8E84;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E8E74:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E8E7C:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E8E84:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E8E8C:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E8E94:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E8E9C:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826E8EA4:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826E8EAC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8EB8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8EB8) { __imp__sub_826E8EB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8EB8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28224
	ctx.r3.s64 = ctx.r11.s64 + -28224;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8EC8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8EC8) { __imp__sub_826E8EC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8EC8) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e8e20
	sub_826E8E20(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8ED0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8ED0) { __imp__sub_826E8ED0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8ED0) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e8da0
	sub_826E8DA0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8ED8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8ED8) { __imp__sub_826E8ED8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8ED8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28188
	ctx.r3.s64 = ctx.r11.s64 + -28188;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8EE8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8EE8) { __imp__sub_826E8EE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8EE8) {
	PPC_FUNC_PROLOGUE();
	// li r3,29
	ctx.r3.s64 = 29;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8EF0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8EF0) { __imp__sub_826E8EF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8EF0) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r4,4
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 4, ctx.xer);
	// bgt cr6,0x826e8f24
	if (ctx.cr6.gt) goto loc_826E8F24;
	// beq cr6,0x826e8f1c
	if (ctx.cr6.eq) goto loc_826E8F1C;
	// cmpwi cr6,r4,1
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 1, ctx.xer);
	// beq cr6,0x826e8f14
	if (ctx.cr6.eq) goto loc_826E8F14;
	// cmpwi cr6,r4,2
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 2, ctx.xer);
	// bne cr6,0x826e8f34
	if (!ctx.cr6.eq) goto loc_826E8F34;
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E8F14:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E8F1C:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E8F24:
	// cmpwi cr6,r4,5
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 5, ctx.xer);
	// beq cr6,0x826e8f44
	if (ctx.cr6.eq) goto loc_826E8F44;
	// cmplwi cr6,r4,65534
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 65534, ctx.xer);
	// beq cr6,0x826e8f3c
	if (ctx.cr6.eq) goto loc_826E8F3C;
loc_826E8F34:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826E8F3C:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E8F44:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8F50);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8F50) { __imp__sub_826E8F50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8F50) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,4
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 4, ctx.xer);
	// bgt cr6,0x826e8f94
	if (ctx.cr6.gt) goto loc_826E8F94;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-28816
	ctx.r12.s64 = ctx.r12.s64 + -28816;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826E8F84;
	case 1:
		goto loc_826E8F8C;
	case 2:
		goto loc_826E8F8C;
	case 3:
		goto loc_826E8F8C;
	case 4:
		goto loc_826E8F84;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E8F84:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826E8F8C:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E8F94:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E8FA0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E8FA0) { __imp__sub_826E8FA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E8FA0) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,4
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 4, ctx.xer);
	// bgt cr6,0x826e8fe4
	if (ctx.cr6.gt) goto loc_826E8FE4;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-28736
	ctx.r12.s64 = ctx.r12.s64 + -28736;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826E8FD4;
	case 1:
		goto loc_826E8FDC;
	case 2:
		goto loc_826E8FDC;
	case 3:
		goto loc_826E8FDC;
	case 4:
		goto loc_826E8FD4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E8FD4:
	// li r11,2
	ctx.r11.s64 = 2;
	// b 0x826e8fe8
	goto loc_826E8FE8;
loc_826E8FDC:
	// li r11,1
	ctx.r11.s64 = 1;
	// b 0x826e8fe8
	goto loc_826E8FE8;
loc_826E8FE4:
	// li r11,-1
	ctx.r11.s64 = -1;
loc_826E8FE8:
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826e9040
	if (ctx.cr6.gt) goto loc_826E9040;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-28660
	ctx.r12.s64 = ctx.r12.s64 + -28660;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E9020;
	case 1:
		goto loc_826E9028;
	case 2:
		goto loc_826E9038;
	case 3:
		goto loc_826E9040;
	case 4:
		goto loc_826E9030;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E9020:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E9028:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E9030:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E9038:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E9040:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9048);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9048) { __imp__sub_826E9048(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9048) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28152
	ctx.r3.s64 = ctx.r11.s64 + -28152;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9058);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9058) { __imp__sub_826E9058(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9058) {
	PPC_FUNC_PROLOGUE();
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9060);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9060) { __imp__sub_826E9060(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9060) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28120
	ctx.r3.s64 = ctx.r11.s64 + -28120;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9070);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9070) { __imp__sub_826E9070(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9070) {
	PPC_FUNC_PROLOGUE();
	// li r3,40
	ctx.r3.s64 = 40;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9078);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9078) { __imp__sub_826E9078(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9078) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28084
	ctx.r3.s64 = ctx.r11.s64 + -28084;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9088);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9088) { __imp__sub_826E9088(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9088) {
	PPC_FUNC_PROLOGUE();
	// li r3,49
	ctx.r3.s64 = 49;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9090);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9090) { __imp__sub_826E9090(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9090) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,5
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 5, ctx.xer);
	// bgt cr6,0x826e90d8
	if (ctx.cr6.gt) goto loc_826E90D8;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-28496
	ctx.r12.s64 = ctx.r12.s64 + -28496;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826E90C8;
	case 1:
		goto loc_826E90C8;
	case 2:
		goto loc_826E90D0;
	case 3:
		goto loc_826E90D0;
	case 4:
		goto loc_826E90D0;
	case 5:
		goto loc_826E90C8;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E90C8:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826E90D0:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E90D8:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E90E0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E90E0) { __imp__sub_826E90E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E90E0) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,7
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 7, ctx.xer);
	// bgt cr6,0x826e9164
	if (ctx.cr6.gt) goto loc_826E9164;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-28416
	ctx.r12.s64 = ctx.r12.s64 + -28416;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826E9120;
	case 1:
		goto loc_826E9128;
	case 2:
		goto loc_826E9130;
	case 3:
		goto loc_826E9138;
	case 4:
		goto loc_826E9140;
	case 5:
		goto loc_826E9148;
	case 6:
		goto loc_826E9150;
	case 7:
		goto loc_826E9158;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E9120:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826E9128:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E9130:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826E9138:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826E9140:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E9148:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E9150:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E9158:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826E9164:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9170);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9170) { __imp__sub_826E9170(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9170) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28044
	ctx.r3.s64 = ctx.r11.s64 + -28044;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9180);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9180) { __imp__sub_826E9180(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9180) {
	PPC_FUNC_PROLOGUE();
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9188);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9188) { __imp__sub_826E9188(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9188) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e90e0
	sub_826E90E0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9190);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9190) { __imp__sub_826E9190(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9190) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,7
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 7, ctx.xer);
	// bgt cr6,0x826e91e0
	if (ctx.cr6.gt) goto loc_826E91E0;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-28240
	ctx.r12.s64 = ctx.r12.s64 + -28240;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826E91D0;
	case 1:
		goto loc_826E91D0;
	case 2:
		goto loc_826E91D0;
	case 3:
		goto loc_826E91D8;
	case 4:
		goto loc_826E91D0;
	case 5:
		goto loc_826E91D0;
	case 6:
		goto loc_826E91D0;
	case 7:
		goto loc_826E91D8;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E91D0:
	// li r11,1
	ctx.r11.s64 = 1;
	// b 0x826e91e4
	goto loc_826E91E4;
loc_826E91D8:
	// li r11,2
	ctx.r11.s64 = 2;
	// b 0x826e91e4
	goto loc_826E91E4;
loc_826E91E0:
	// li r11,-1
	ctx.r11.s64 = -1;
loc_826E91E4:
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826e923c
	if (ctx.cr6.gt) goto loc_826E923C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-28152
	ctx.r12.s64 = ctx.r12.s64 + -28152;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E921C;
	case 1:
		goto loc_826E9224;
	case 2:
		goto loc_826E9234;
	case 3:
		goto loc_826E923C;
	case 4:
		goto loc_826E922C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E921C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E9224:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E922C:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E9234:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E923C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9248);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9248) { __imp__sub_826E9248(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9248) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-28008
	ctx.r3.s64 = ctx.r11.s64 + -28008;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9258);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9258) { __imp__sub_826E9258(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9258) {
	PPC_FUNC_PROLOGUE();
	// li r3,41
	ctx.r3.s64 = 41;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9260);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9260) { __imp__sub_826E9260(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9260) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,5
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 5, ctx.xer);
	// bgt cr6,0x826e92cc
	if (ctx.cr6.gt) goto loc_826E92CC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-28032
	ctx.r12.s64 = ctx.r12.s64 + -28032;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826E9298;
	case 1:
		goto loc_826E92A0;
	case 2:
		goto loc_826E92A8;
	case 3:
		goto loc_826E92B0;
	case 4:
		goto loc_826E92B8;
	case 5:
		goto loc_826E92C0;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E9298:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826E92A0:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E92A8:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E92B0:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E92B8:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E92C0:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826E92CC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E92D8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E92D8) { __imp__sub_826E92D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E92D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826e935c
	if (ctx.cr6.gt) goto loc_826E935C;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826e9354
	if (ctx.cr6.eq) goto loc_826E9354;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x826e935c
	if (ctx.cr6.gt) goto loc_826E935C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-27884
	ctx.r12.s64 = ctx.r12.s64 + -27884;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E933C;
	case 1:
		goto loc_826E9334;
	case 2:
		goto loc_826E934C;
	case 3:
		goto loc_826E9344;
	case 4:
		goto loc_826E935C;
	case 5:
		goto loc_826E932C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E932C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E9334:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E933C:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E9344:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E934C:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E9354:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826E935C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9368);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9368) { __imp__sub_826E9368(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9368) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-27968
	ctx.r3.s64 = ctx.r11.s64 + -27968;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9378);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9378) { __imp__sub_826E9378(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9378) {
	PPC_FUNC_PROLOGUE();
	// li r3,22
	ctx.r3.s64 = 22;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9380);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9380) { __imp__sub_826E9380(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9380) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e92d8
	sub_826E92D8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9388);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9388) { __imp__sub_826E9388(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9388) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e9260
	sub_826E9260(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9390);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9390) { __imp__sub_826E9390(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9390) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// li r3,1
	ctx.r3.s64 = 1;
	// cmplwi cr6,r6,3
	ctx.cr6.compare<uint32_t>(ctx.r6.u32, 3, ctx.xer);
	// bgt cr6,0x826e9420
	if (ctx.cr6.gt) goto loc_826E9420;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-27720
	ctx.r12.s64 = ctx.r12.s64 + -27720;
	// rlwinm r0,r6,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r6.u32) {
	case 0:
		goto loc_826E93C8;
	case 1:
		goto loc_826E93E4;
	case 2:
		goto loc_826E9414;
	case 3:
		goto loc_826E9400;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E93C8:
	// ld r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U64(ctx.r11.u32 + 0);
	// ld r10,0(r4)
	ctx.r10.u64 = PPC_LOAD_U64(ctx.r4.u32 + 0);
	// cmpd cr6,r11,r10
	ctx.cr6.compare<int64_t>(ctx.r11.s64, ctx.r10.s64, ctx.xer);
	// blt cr6,0x826e93dc
	if (ctx.cr6.lt) goto loc_826E93DC;
	// mr r11,r10
	ctx.r11.u64 = ctx.r10.u64;
loc_826E93DC:
	// std r11,0(r5)
	PPC_STORE_U64(ctx.r5.u32 + 0, ctx.r11.u64);
	// blr 
	return;
loc_826E93E4:
	// ld r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U64(ctx.r11.u32 + 0);
	// ld r10,0(r4)
	ctx.r10.u64 = PPC_LOAD_U64(ctx.r4.u32 + 0);
	// cmpd cr6,r11,r10
	ctx.cr6.compare<int64_t>(ctx.r11.s64, ctx.r10.s64, ctx.xer);
	// bgt cr6,0x826e93dc
	if (ctx.cr6.gt) goto loc_826E93DC;
	// mr r11,r10
	ctx.r11.u64 = ctx.r10.u64;
	// std r11,0(r5)
	PPC_STORE_U64(ctx.r5.u32 + 0, ctx.r11.u64);
	// blr 
	return;
loc_826E9400:
	// ld r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U64(ctx.r11.u32 + 0);
	// ld r10,0(r4)
	ctx.r10.u64 = PPC_LOAD_U64(ctx.r4.u32 + 0);
	// add r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
	// std r11,0(r5)
	PPC_STORE_U64(ctx.r5.u32 + 0, ctx.r11.u64);
	// blr 
	return;
loc_826E9414:
	// ld r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U64(ctx.r11.u32 + 0);
	// std r11,0(r5)
	PPC_STORE_U64(ctx.r5.u32 + 0, ctx.r11.u64);
	// blr 
	return;
loc_826E9420:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9428);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9428) { __imp__sub_826E9428(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9428) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// li r3,1
	ctx.r3.s64 = 1;
	// cmplwi cr6,r6,3
	ctx.cr6.compare<uint32_t>(ctx.r6.u32, 3, ctx.xer);
	// bgt cr6,0x826e94b8
	if (ctx.cr6.gt) goto loc_826E94B8;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-27568
	ctx.r12.s64 = ctx.r12.s64 + -27568;
	// rlwinm r0,r6,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r6.u32) {
	case 0:
		goto loc_826E9460;
	case 1:
		goto loc_826E947C;
	case 2:
		goto loc_826E94AC;
	case 3:
		goto loc_826E9498;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E9460:
	// lfs f0,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,0(r4)
	temp.u32 = PPC_LOAD_U32(ctx.r4.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// blt cr6,0x826e9474
	if (ctx.cr6.lt) goto loc_826E9474;
	// fmr f0,f13
	ctx.f0.f64 = ctx.f13.f64;
loc_826E9474:
	// stfs f0,0(r5)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r5.u32 + 0, temp.u32);
	// blr 
	return;
loc_826E947C:
	// lfs f0,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,0(r4)
	temp.u32 = PPC_LOAD_U32(ctx.r4.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// bgt cr6,0x826e9474
	if (ctx.cr6.gt) goto loc_826E9474;
	// fmr f0,f13
	ctx.f0.f64 = ctx.f13.f64;
	// stfs f0,0(r5)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r5.u32 + 0, temp.u32);
	// blr 
	return;
loc_826E9498:
	// lfs f0,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,0(r4)
	temp.u32 = PPC_LOAD_U32(ctx.r4.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// fadds f0,f0,f13
	ctx.f0.f64 = double(float(ctx.f0.f64 + ctx.f13.f64));
	// stfs f0,0(r5)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r5.u32 + 0, temp.u32);
	// blr 
	return;
loc_826E94AC:
	// lfs f0,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r5)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r5.u32 + 0, temp.u32);
	// blr 
	return;
loc_826E94B8:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E94C0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E94C0) { __imp__sub_826E94C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E94C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826e9558
	if (ctx.cr6.gt) goto loc_826E9558;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826e9550
	if (ctx.cr6.eq) goto loc_826E9550;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,6
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 6, ctx.xer);
	// bgt cr6,0x826e9558
	if (ctx.cr6.gt) goto loc_826E9558;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-27396
	ctx.r12.s64 = ctx.r12.s64 + -27396;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E9538;
	case 1:
		goto loc_826E9540;
	case 2:
		goto loc_826E9548;
	case 3:
		goto loc_826E9520;
	case 4:
		goto loc_826E9518;
	case 5:
		goto loc_826E9530;
	case 6:
		goto loc_826E9528;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E9518:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E9520:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E9528:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E9530:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E9538:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E9540:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826E9548:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826E9550:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826E9558:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9560);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9560) { __imp__sub_826E9560(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9560) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-27924
	ctx.r3.s64 = ctx.r11.s64 + -27924;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9570);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9570) { __imp__sub_826E9570(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9570) {
	PPC_FUNC_PROLOGUE();
	// li r3,17
	ctx.r3.s64 = 17;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9578);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9578) { __imp__sub_826E9578(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9578) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e94c0
	sub_826E94C0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9580);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9580) { __imp__sub_826E9580(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9580) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,7
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 7, ctx.xer);
	// bgt cr6,0x826e95d0
	if (ctx.cr6.gt) goto loc_826E95D0;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-27232
	ctx.r12.s64 = ctx.r12.s64 + -27232;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826E95C0;
	case 1:
		goto loc_826E95C0;
	case 2:
		goto loc_826E95C0;
	case 3:
		goto loc_826E95C8;
	case 4:
		goto loc_826E95C0;
	case 5:
		goto loc_826E95C0;
	case 6:
		goto loc_826E95C0;
	case 7:
		goto loc_826E95C8;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E95C0:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E95C8:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826E95D0:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E95D8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E95D8) { __imp__sub_826E95D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E95D8) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// li r3,1
	ctx.r3.s64 = 1;
	// cmplwi cr6,r6,3
	ctx.cr6.compare<uint32_t>(ctx.r6.u32, 3, ctx.xer);
	// bgt cr6,0x826e9668
	if (ctx.cr6.gt) goto loc_826E9668;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-27136
	ctx.r12.s64 = ctx.r12.s64 + -27136;
	// rlwinm r0,r6,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r6.u32) {
	case 0:
		goto loc_826E9610;
	case 1:
		goto loc_826E962C;
	case 2:
		goto loc_826E965C;
	case 3:
		goto loc_826E9648;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E9610:
	// lfd f0,0(r11)
	ctx.fpscr.disableFlushMode();
	ctx.f0.u64 = PPC_LOAD_U64(ctx.r11.u32 + 0);
	// lfd f13,0(r4)
	ctx.f13.u64 = PPC_LOAD_U64(ctx.r4.u32 + 0);
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// blt cr6,0x826e9624
	if (ctx.cr6.lt) goto loc_826E9624;
	// fmr f0,f13
	ctx.f0.f64 = ctx.f13.f64;
loc_826E9624:
	// stfd f0,0(r5)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r5.u32 + 0, ctx.f0.u64);
	// blr 
	return;
loc_826E962C:
	// lfd f0,0(r11)
	ctx.fpscr.disableFlushMode();
	ctx.f0.u64 = PPC_LOAD_U64(ctx.r11.u32 + 0);
	// lfd f13,0(r4)
	ctx.f13.u64 = PPC_LOAD_U64(ctx.r4.u32 + 0);
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// bgt cr6,0x826e9624
	if (ctx.cr6.gt) goto loc_826E9624;
	// fmr f0,f13
	ctx.f0.f64 = ctx.f13.f64;
	// stfd f0,0(r5)
	PPC_STORE_U64(ctx.r5.u32 + 0, ctx.f0.u64);
	// blr 
	return;
loc_826E9648:
	// lfd f0,0(r11)
	ctx.fpscr.disableFlushMode();
	ctx.f0.u64 = PPC_LOAD_U64(ctx.r11.u32 + 0);
	// lfd f13,0(r4)
	ctx.f13.u64 = PPC_LOAD_U64(ctx.r4.u32 + 0);
	// fadd f0,f0,f13
	ctx.f0.f64 = ctx.f0.f64 + ctx.f13.f64;
	// stfd f0,0(r5)
	PPC_STORE_U64(ctx.r5.u32 + 0, ctx.f0.u64);
	// blr 
	return;
loc_826E965C:
	// lfd f0,0(r11)
	ctx.fpscr.disableFlushMode();
	ctx.f0.u64 = PPC_LOAD_U64(ctx.r11.u32 + 0);
	// stfd f0,0(r5)
	PPC_STORE_U64(ctx.r5.u32 + 0, ctx.f0.u64);
	// blr 
	return;
loc_826E9668:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9670);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9670) { __imp__sub_826E9670(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9670) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,5
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 5, ctx.xer);
	// bgt cr6,0x826e96dc
	if (ctx.cr6.gt) goto loc_826E96DC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-26992
	ctx.r12.s64 = ctx.r12.s64 + -26992;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826E96A8;
	case 1:
		goto loc_826E96B0;
	case 2:
		goto loc_826E96B8;
	case 3:
		goto loc_826E96C0;
	case 4:
		goto loc_826E96C8;
	case 5:
		goto loc_826E96D0;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E96A8:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E96B0:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826E96B8:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E96C0:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E96C8:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E96D0:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826E96DC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E96E8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E96E8) { __imp__sub_826E96E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E96E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826e9768
	if (ctx.cr6.gt) goto loc_826E9768;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826e9760
	if (ctx.cr6.eq) goto loc_826E9760;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826e9768
	if (ctx.cr6.gt) goto loc_826E9768;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-26844
	ctx.r12.s64 = ctx.r12.s64 + -26844;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E9748;
	case 1:
		goto loc_826E9738;
	case 2:
		goto loc_826E9758;
	case 3:
		goto loc_826E9750;
	case 4:
		goto loc_826E9740;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E9738:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E9740:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E9748:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E9750:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E9758:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E9760:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826E9768:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9770);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9770) { __imp__sub_826E9770(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9770) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-27888
	ctx.r3.s64 = ctx.r11.s64 + -27888;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9780);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9780) { __imp__sub_826E9780(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9780) {
	PPC_FUNC_PROLOGUE();
	// li r3,39
	ctx.r3.s64 = 39;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9788);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9788) { __imp__sub_826E9788(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9788) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e96e8
	sub_826E96E8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9790);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9790) { __imp__sub_826E9790(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9790) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e9670
	sub_826E9670(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9798);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9798) { __imp__sub_826E9798(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9798) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// li r3,1
	ctx.r3.s64 = 1;
	// cmplwi cr6,r6,3
	ctx.cr6.compare<uint32_t>(ctx.r6.u32, 3, ctx.xer);
	// bgt cr6,0x826e9828
	if (ctx.cr6.gt) goto loc_826E9828;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-26688
	ctx.r12.s64 = ctx.r12.s64 + -26688;
	// rlwinm r0,r6,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r6.u32) {
	case 0:
		goto loc_826E97D0;
	case 1:
		goto loc_826E97EC;
	case 2:
		goto loc_826E981C;
	case 3:
		goto loc_826E9808;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E97D0:
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r10,0(r4)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r4.u32 + 0);
	// cmpw cr6,r11,r10
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r10.s32, ctx.xer);
	// blt cr6,0x826e97e4
	if (ctx.cr6.lt) goto loc_826E97E4;
	// mr r11,r10
	ctx.r11.u64 = ctx.r10.u64;
loc_826E97E4:
	// stw r11,0(r5)
	PPC_STORE_U32(ctx.r5.u32 + 0, ctx.r11.u32);
	// blr 
	return;
loc_826E97EC:
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r10,0(r4)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r4.u32 + 0);
	// cmpw cr6,r11,r10
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r10.s32, ctx.xer);
	// bgt cr6,0x826e97e4
	if (ctx.cr6.gt) goto loc_826E97E4;
	// mr r11,r10
	ctx.r11.u64 = ctx.r10.u64;
	// stw r11,0(r5)
	PPC_STORE_U32(ctx.r5.u32 + 0, ctx.r11.u32);
	// blr 
	return;
loc_826E9808:
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r10,0(r4)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r4.u32 + 0);
	// add r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
	// stw r11,0(r5)
	PPC_STORE_U32(ctx.r5.u32 + 0, ctx.r11.u32);
	// blr 
	return;
loc_826E981C:
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stw r11,0(r5)
	PPC_STORE_U32(ctx.r5.u32 + 0, ctx.r11.u32);
	// blr 
	return;
loc_826E9828:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9830);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9830) { __imp__sub_826E9830(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9830) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,5
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 5, ctx.xer);
	// bgt cr6,0x826e989c
	if (ctx.cr6.gt) goto loc_826E989C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-26544
	ctx.r12.s64 = ctx.r12.s64 + -26544;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826E9868;
	case 1:
		goto loc_826E9870;
	case 2:
		goto loc_826E9878;
	case 3:
		goto loc_826E9880;
	case 4:
		goto loc_826E9888;
	case 5:
		goto loc_826E9890;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E9868:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826E9870:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E9878:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E9880:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E9888:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E9890:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826E989C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E98A8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E98A8) { __imp__sub_826E98A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E98A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826e992c
	if (ctx.cr6.gt) goto loc_826E992C;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826e9924
	if (ctx.cr6.eq) goto loc_826E9924;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x826e992c
	if (ctx.cr6.gt) goto loc_826E992C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-26396
	ctx.r12.s64 = ctx.r12.s64 + -26396;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E990C;
	case 1:
		goto loc_826E9904;
	case 2:
		goto loc_826E9914;
	case 3:
		goto loc_826E991C;
	case 4:
		goto loc_826E992C;
	case 5:
		goto loc_826E98FC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E98FC:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E9904:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E990C:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E9914:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E991C:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E9924:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826E992C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9938);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9938) { __imp__sub_826E9938(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9938) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-27848
	ctx.r3.s64 = ctx.r11.s64 + -27848;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9948);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9948) { __imp__sub_826E9948(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9948) {
	PPC_FUNC_PROLOGUE();
	// li r3,21
	ctx.r3.s64 = 21;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9950);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9950) { __imp__sub_826E9950(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9950) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e98a8
	sub_826E98A8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9958);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9958) { __imp__sub_826E9958(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9958) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e9830
	sub_826E9830(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9960);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9960) { __imp__sub_826E9960(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9960) {
	PPC_FUNC_PROLOGUE();
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9968);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9968) { __imp__sub_826E9968(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9968) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,8
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 8, ctx.xer);
	// bgt cr6,0x826e99f8
	if (ctx.cr6.gt) goto loc_826E99F8;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-26232
	ctx.r12.s64 = ctx.r12.s64 + -26232;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826E99AC;
	case 1:
		goto loc_826E99B4;
	case 2:
		goto loc_826E99BC;
	case 3:
		goto loc_826E99C4;
	case 4:
		goto loc_826E99CC;
	case 5:
		goto loc_826E99D4;
	case 6:
		goto loc_826E99DC;
	case 7:
		goto loc_826E99E4;
	case 8:
		goto loc_826E99EC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E99AC:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826E99B4:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826E99BC:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826E99C4:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E99CC:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826E99D4:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E99DC:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E99E4:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E99EC:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826E99F8:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9A00);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9A00) { __imp__sub_826E9A00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9A00) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-27804
	ctx.r3.s64 = ctx.r11.s64 + -27804;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9A10);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9A10) { __imp__sub_826E9A10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9A10) {
	PPC_FUNC_PROLOGUE();
	// li r3,46
	ctx.r3.s64 = 46;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9A18);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9A18) { __imp__sub_826E9A18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9A18) {
	PPC_FUNC_PROLOGUE();
	// li r3,9
	ctx.r3.s64 = 9;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9A20);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9A20) { __imp__sub_826E9A20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9A20) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e9968
	sub_826E9968(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9A28);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9A28) { __imp__sub_826E9A28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9A28) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,8
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 8, ctx.xer);
	// bgt cr6,0x826e9a7c
	if (ctx.cr6.gt) goto loc_826E9A7C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-26040
	ctx.r12.s64 = ctx.r12.s64 + -26040;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826E9A6C;
	case 1:
		goto loc_826E9A6C;
	case 2:
		goto loc_826E9A74;
	case 3:
		goto loc_826E9A74;
	case 4:
		goto loc_826E9A6C;
	case 5:
		goto loc_826E9A6C;
	case 6:
		goto loc_826E9A6C;
	case 7:
		goto loc_826E9A6C;
	case 8:
		goto loc_826E9A74;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E9A6C:
	// li r11,1
	ctx.r11.s64 = 1;
	// b 0x826e9a80
	goto loc_826E9A80;
loc_826E9A74:
	// li r11,2
	ctx.r11.s64 = 2;
	// b 0x826e9a80
	goto loc_826E9A80;
loc_826E9A7C:
	// li r11,-1
	ctx.r11.s64 = -1;
loc_826E9A80:
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826e9ad8
	if (ctx.cr6.gt) goto loc_826E9AD8;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-25948
	ctx.r12.s64 = ctx.r12.s64 + -25948;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E9AB8;
	case 1:
		goto loc_826E9AC0;
	case 2:
		goto loc_826E9AD0;
	case 3:
		goto loc_826E9AD8;
	case 4:
		goto loc_826E9AC8;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E9AB8:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E9AC0:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E9AC8:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E9AD0:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E9AD8:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9AE0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9AE0) { __imp__sub_826E9AE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9AE0) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,6
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 6, ctx.xer);
	// bgt cr6,0x826e9b58
	if (ctx.cr6.gt) goto loc_826E9B58;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-25856
	ctx.r12.s64 = ctx.r12.s64 + -25856;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826E9B1C;
	case 1:
		goto loc_826E9B24;
	case 2:
		goto loc_826E9B2C;
	case 3:
		goto loc_826E9B34;
	case 4:
		goto loc_826E9B3C;
	case 5:
		goto loc_826E9B44;
	case 6:
		goto loc_826E9B4C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E9B1C:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826E9B24:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E9B2C:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826E9B34:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E9B3C:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E9B44:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E9B4C:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826E9B58:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9B60);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9B60) { __imp__sub_826E9B60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9B60) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-27772
	ctx.r3.s64 = ctx.r11.s64 + -27772;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9B70);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9B70) { __imp__sub_826E9B70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9B70) {
	PPC_FUNC_PROLOGUE();
	// li r3,56
	ctx.r3.s64 = 56;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9B78);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9B78) { __imp__sub_826E9B78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9B78) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e9ae0
	sub_826E9AE0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9B80);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9B80) { __imp__sub_826E9B80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9B80) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,6
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 6, ctx.xer);
	// bgt cr6,0x826e9bcc
	if (ctx.cr6.gt) goto loc_826E9BCC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-25696
	ctx.r12.s64 = ctx.r12.s64 + -25696;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826E9BBC;
	case 1:
		goto loc_826E9BBC;
	case 2:
		goto loc_826E9BC4;
	case 3:
		goto loc_826E9BC4;
	case 4:
		goto loc_826E9BC4;
	case 5:
		goto loc_826E9BC4;
	case 6:
		goto loc_826E9BBC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E9BBC:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826E9BC4:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E9BCC:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9BD8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9BD8) { __imp__sub_826E9BD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9BD8) {
	PPC_FUNC_PROLOGUE();
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9BE0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9BE0) { __imp__sub_826E9BE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9BE0) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,7
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 7, ctx.xer);
	// bgt cr6,0x826e9c64
	if (ctx.cr6.gt) goto loc_826E9C64;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-25600
	ctx.r12.s64 = ctx.r12.s64 + -25600;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826E9C20;
	case 1:
		goto loc_826E9C28;
	case 2:
		goto loc_826E9C30;
	case 3:
		goto loc_826E9C38;
	case 4:
		goto loc_826E9C40;
	case 5:
		goto loc_826E9C48;
	case 6:
		goto loc_826E9C50;
	case 7:
		goto loc_826E9C58;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E9C20:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826E9C28:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826E9C30:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826E9C38:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E9C40:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E9C48:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E9C50:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E9C58:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826E9C64:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9C70);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9C70) { __imp__sub_826E9C70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9C70) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826e9d0c
	if (ctx.cr6.gt) goto loc_826E9D0C;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826e9d04
	if (ctx.cr6.eq) goto loc_826E9D04;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,7
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 7, ctx.xer);
	// bgt cr6,0x826e9d0c
	if (ctx.cr6.gt) goto loc_826E9D0C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-25428
	ctx.r12.s64 = ctx.r12.s64 + -25428;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E9CE4;
	case 1:
		goto loc_826E9CEC;
	case 2:
		goto loc_826E9CF4;
	case 3:
		goto loc_826E9CFC;
	case 4:
		goto loc_826E9D0C;
	case 5:
		goto loc_826E9CCC;
	case 6:
		goto loc_826E9CD4;
	case 7:
		goto loc_826E9CDC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E9CCC:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E9CD4:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E9CDC:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E9CE4:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E9CEC:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E9CF4:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826E9CFC:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826E9D04:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826E9D0C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9D18);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9D18) { __imp__sub_826E9D18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9D18) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-27736
	ctx.r3.s64 = ctx.r11.s64 + -27736;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9D28);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9D28) { __imp__sub_826E9D28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9D28) {
	PPC_FUNC_PROLOGUE();
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9D30);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9D30) { __imp__sub_826E9D30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9D30) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e9c70
	sub_826E9C70(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9D38);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9D38) { __imp__sub_826E9D38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9D38) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e9be0
	sub_826E9BE0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9D40);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9D40) { __imp__sub_826E9D40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9D40) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,7
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 7, ctx.xer);
	// bgt cr6,0x826e9d90
	if (ctx.cr6.gt) goto loc_826E9D90;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-25248
	ctx.r12.s64 = ctx.r12.s64 + -25248;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826E9D80;
	case 1:
		goto loc_826E9D80;
	case 2:
		goto loc_826E9D88;
	case 3:
		goto loc_826E9D88;
	case 4:
		goto loc_826E9D80;
	case 5:
		goto loc_826E9D80;
	case 6:
		goto loc_826E9D80;
	case 7:
		goto loc_826E9D88;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E9D80:
	// li r11,1
	ctx.r11.s64 = 1;
	// b 0x826e9d94
	goto loc_826E9D94;
loc_826E9D88:
	// li r11,2
	ctx.r11.s64 = 2;
	// b 0x826e9d94
	goto loc_826E9D94;
loc_826E9D90:
	// li r11,-1
	ctx.r11.s64 = -1;
loc_826E9D94:
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826e9dec
	if (ctx.cr6.gt) goto loc_826E9DEC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-25160
	ctx.r12.s64 = ctx.r12.s64 + -25160;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E9DCC;
	case 1:
		goto loc_826E9DD4;
	case 2:
		goto loc_826E9DE4;
	case 3:
		goto loc_826E9DEC;
	case 4:
		goto loc_826E9DDC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E9DCC:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E9DD4:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E9DDC:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E9DE4:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E9DEC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9DF8);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9DF8) { __imp__sub_826E9DF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9DF8) {
	PPC_FUNC_PROLOGUE();
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9E00);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9E00) { __imp__sub_826E9E00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9E00) {
	PPC_FUNC_PROLOGUE();
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9E08);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9E08) { __imp__sub_826E9E08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9E08) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,5
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 5, ctx.xer);
	// bgt cr6,0x826e9e74
	if (ctx.cr6.gt) goto loc_826E9E74;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-25048
	ctx.r12.s64 = ctx.r12.s64 + -25048;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826E9E40;
	case 1:
		goto loc_826E9E48;
	case 2:
		goto loc_826E9E50;
	case 3:
		goto loc_826E9E58;
	case 4:
		goto loc_826E9E60;
	case 5:
		goto loc_826E9E68;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E9E40:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E9E48:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826E9E50:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E9E58:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E9E60:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E9E68:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826E9E74:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9E80);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9E80) { __imp__sub_826E9E80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9E80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826e9f04
	if (ctx.cr6.gt) goto loc_826E9F04;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826e9efc
	if (ctx.cr6.eq) goto loc_826E9EFC;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x826e9f04
	if (ctx.cr6.gt) goto loc_826E9F04;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-24900
	ctx.r12.s64 = ctx.r12.s64 + -24900;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826E9ED4;
	case 1:
		goto loc_826E9EE4;
	case 2:
		goto loc_826E9EEC;
	case 3:
		goto loc_826E9EF4;
	case 4:
		goto loc_826E9F04;
	case 5:
		goto loc_826E9EDC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E9ED4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826E9EDC:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E9EE4:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E9EEC:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E9EF4:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E9EFC:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826E9F04:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9F10);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9F10) { __imp__sub_826E9F10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9F10) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-27700
	ctx.r3.s64 = ctx.r11.s64 + -27700;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9F20);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9F20) { __imp__sub_826E9F20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9F20) {
	PPC_FUNC_PROLOGUE();
	// li r3,52
	ctx.r3.s64 = 52;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9F28);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9F28) { __imp__sub_826E9F28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9F28) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e9e80
	sub_826E9E80(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9F30);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9F30) { __imp__sub_826E9F30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9F30) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e9e08
	sub_826E9E08(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826E9F38);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9F38) { __imp__sub_826E9F38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9F38) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,8
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 8, ctx.xer);
	// bgt cr6,0x826e9fc8
	if (ctx.cr6.gt) goto loc_826E9FC8;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-24744
	ctx.r12.s64 = ctx.r12.s64 + -24744;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826E9F7C;
	case 1:
		goto loc_826E9F84;
	case 2:
		goto loc_826E9F8C;
	case 3:
		goto loc_826E9F94;
	case 4:
		goto loc_826E9F9C;
	case 5:
		goto loc_826E9FA4;
	case 6:
		goto loc_826E9FAC;
	case 7:
		goto loc_826E9FB4;
	case 8:
		goto loc_826E9FBC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826E9F7C:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826E9F84:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826E9F8C:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826E9F94:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826E9F9C:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826E9FA4:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826E9FAC:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826E9FB4:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826E9FBC:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826E9FC8:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

