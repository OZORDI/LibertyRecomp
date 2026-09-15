// Derived by tools/derive_mouse_camera_hooks.py --write.
// Complete retail functions; only reviewed mouse integration sites differ.
// Do not edit by hand. Run the derivation without --write to verify.

extern "C" void sub_825337F0(PPCContext& ctx, uint8_t* base) {
  gta4::input::MouseRotation native_mouse{};
  uint32_t native_control = 0;
  const double native_fov = ctx.f1.f64;
  native_control = ctx.r4.u32;
	REX_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7bc
	ctx.lr = 0x825337F8;
	__savegprlr_25(ctx, base);
	// addi r12,r1,-64
	ctx.r12.s64 = ctx.r1.s64 + -64;
	// bl 0x82a01324
	ctx.lr = 0x82533800;
	__savefpr_27(ctx, base);
	// stwu r1,-224(r1)
	ea = -224 + ctx.r1.u32;
	REX_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// fmr f28,f1
	ctx.fpscr.disableFlushMode();
	ctx.f28.f64 = ctx.f1.f64;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// mr r27,r5
	ctx.r27.u64 = ctx.r5.u64;
	// mr r26,r6
	ctx.r26.u64 = ctx.r6.u64;
	// mr r28,r8
	ctx.r28.u64 = ctx.r8.u64;
	// mr r25,r9
	ctx.r25.u64 = ctx.r9.u64;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// beq cr6,0x82533cc0
	if (ctx.cr6.eq) goto loc_82533CC0;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f0,640(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 640);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// lfs f13,17536(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 17536);
	ctx.f13.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f30,3400(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 3400);
	ctx.f30.f64 = double(temp.f32);
	// fsubs f12,f30,f0
	ctx.f12.f64 = double(float(ctx.f30.f64 - ctx.f0.f64));
	// fmadds f0,f12,f13,f0
	ctx.f0.f64 = double(float(std::fma(ctx.f12.f64, ctx.f13.f64, ctx.f0.f64)));
	// stfs f0,640(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 640, temp.u32);
	// bl 0x822b80c8
	ctx.lr = 0x82533850;
	sub_822B80C8(ctx, base);
	// lwz r11,8(r3)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r3.u32 + 8);
	// ld r3,0(r3)
	ctx.r3.u64 = REX_LOAD_U64(ctx.r3.u32 + 0);
	// rldicr r4,r11,32,63
	ctx.r4.u64 = __builtin_rotateleft64(ctx.r11.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// bl 0x822b7a50
	ctx.lr = 0x82533860;
	sub_822B7A50(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// fneg f31,f1
	ctx.fpscr.disableFlushMode();
	ctx.f31.u64 = ctx.f1.u64 ^ 0x8000000000000000;
	// bl 0x822b8070
	ctx.lr = 0x8253386C;
	sub_822B8070(ctx, base);
	// lwz r11,8(r3)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r3.u32 + 8);
	// ld r3,0(r3)
	ctx.r3.u64 = REX_LOAD_U64(ctx.r3.u32 + 0);
	// rldicr r4,r11,32,63
	ctx.r4.u64 = __builtin_rotateleft64(ctx.r11.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// bl 0x822b7ae0
	ctx.lr = 0x8253387C;
	sub_822B7AE0(ctx, base);
	// lwz r30,280(r31)
	ctx.r30.u64 = REX_LOAD_U32(ctx.r31.u32 + 280);
	// fneg f29,f1
	ctx.fpscr.disableFlushMode();
	ctx.f29.u64 = ctx.f1.u64 ^ 0x8000000000000000;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// beq cr6,0x825338bc
	if (ctx.cr6.eq) goto loc_825338BC;
	// lwz r11,0(r30)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r30.u32 + 0);
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// lwz r11,40(r11)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r11.u32 + 40);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x825338A0;
	REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmpwi cr6,r3,7
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 7, ctx.xer);
	// bne cr6,0x825338bc
	if (!ctx.cr6.eq) goto loc_825338BC;
	// lbz r11,453(r30)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r30.u32 + 453);
	// rlwinm r11,r11,0,25,25
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x40;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825338bc
	if (ctx.cr6.eq) goto loc_825338BC;
	// fneg f29,f29
	ctx.fpscr.disableFlushMode();
	ctx.f29.u64 = ctx.f29.u64 ^ 0x8000000000000000;
loc_825338BC:
	// fmuls f0,f31,f31
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(float(ctx.f31.f64 * ctx.f31.f64));
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// fmadds f1,f29,f29,f0
	ctx.f1.f64 = double(float(std::fma(ctx.f29.f64, ctx.f29.f64, ctx.f0.f64)));
	// lfs f0,2612(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f1,f0
	ctx.cr6.compare(ctx.f1.f64, ctx.f0.f64);
	// ble cr6,0x825338e4
	if (!ctx.cr6.gt) goto loc_825338E4;
	// fsqrts f1,f1
	ctx.f1.f64 = double(float(sqrt(ctx.f1.f64)));
	// fdivs f0,f30,f1
	ctx.f0.f64 = double(float(ctx.f30.f64 / ctx.f1.f64));
	// fmuls f31,f0,f31
	ctx.f31.f64 = double(float(ctx.f0.f64 * ctx.f31.f64));
	// fmuls f29,f0,f29
	ctx.f29.f64 = double(float(ctx.f0.f64 * ctx.f29.f64));
loc_825338E4:
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// addi r11,r11,-24840
	ctx.r11.s64 = ctx.r11.s64 + -24840;
	// lwz r30,36(r11)
	ctx.r30.u64 = REX_LOAD_U32(ctx.r11.u32 + 36);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f0,-31684(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -31684);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32057
	ctx.r11.s64 = -2100887552;
	// fcmpu cr6,f1,f0
	ctx.cr6.compare(ctx.f1.f64, ctx.f0.f64);
	// addi r29,r11,-15708
	ctx.r29.s64 = ctx.r11.s64 + -15708;
	// ble cr6,0x82533944
	if (!ctx.cr6.gt) goto loc_82533944;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// lfs f13,520(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 520);
	ctx.f13.f64 = double(temp.f32);
	// rlwinm r11,r30,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f0,8(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// addi r10,r10,-13596
	ctx.r10.s64 = ctx.r10.s64 + -13596;
	// lis r9,-32254
	ctx.r9.s64 = -2113798144;
	// addi r9,r9,17652
	ctx.r9.s64 = ctx.r9.s64 + 17652;
	// lfsx f12,r11,r10
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + ctx.r10.u32);
	ctx.f12.f64 = double(temp.f32);
	// fmadds f0,f12,f0,f13
	ctx.f0.f64 = double(float(std::fma(ctx.f12.f64, ctx.f0.f64, ctx.f13.f64)));
	// stfs f0,520(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 520, temp.u32);
	// lfsx f13,r11,r9
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + ctx.r9.u32);
	ctx.f13.f64 = double(temp.f32);
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// ble cr6,0x82533948
	if (!ctx.cr6.gt) goto loc_82533948;
	// stfs f13,520(r31)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r31.u32 + 520, temp.u32);
	// b 0x82533948
	goto loc_82533948;
loc_82533944:
	// stfs f30,520(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f30.f64);
	REX_STORE_U32(ctx.r31.u32 + 520, temp.u32);
loc_82533948:
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// lfd f2,29720(r11)
	ctx.fpscr.disableFlushMode();
	ctx.f2.u64 = REX_LOAD_U64(ctx.r11.u32 + 29720);
	// bl 0x82a02648
	ctx.lr = 0x82533954;
	sub_82A02648(ctx, base);
	// lfs f0,520(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 520);
	ctx.f0.f64 = double(temp.f32);
	// lis r9,-32256
	ctx.r9.s64 = -2113929216;
	// fsubs f9,f30,f0
	ctx.f9.f64 = double(float(ctx.f30.f64 - ctx.f0.f64));
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// rlwinm r11,r30,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// fabs f10,f31
	ctx.f10.u64 = ctx.f31.u64 & ~0x8000000000000000;
	// addi r10,r10,-13608
	ctx.r10.s64 = ctx.r10.s64 + -13608;
	// frsp f12,f1
	ctx.f12.f64 = double(float(ctx.f1.f64));
	// lis r8,-32254
	ctx.r8.s64 = -2113798144;
	// lfs f13,11864(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 11864);
	ctx.f13.f64 = double(temp.f32);
	// lis r9,-32254
	ctx.r9.s64 = -2113798144;
	// fmuls f13,f28,f13
	ctx.f13.f64 = double(float(ctx.f28.f64 * ctx.f13.f64));
	// addi r9,r9,17640
	ctx.r9.s64 = ctx.r9.s64 + 17640;
	// lfsx f8,r11,r10
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + ctx.r10.u32);
	ctx.f8.f64 = double(temp.f32);
	// lfs f11,17124(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 17124);
	ctx.f11.f64 = double(temp.f32);
	// fsel f0,f9,f30,f0
	ctx.f0.f64 = ctx.f9.f64 >= 0.0 ? ctx.f30.f64 : ctx.f0.f64;
	// lfsx f7,r11,r9
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + ctx.r9.u32);
	ctx.f7.f64 = double(temp.f32);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// fcmpu cr6,f10,f11
	ctx.cr6.compare(ctx.f10.f64, ctx.f11.f64);
	// lfs f10,17204(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 17204);
	ctx.f10.f64 = double(temp.f32);
	// lis r11,-32244
	ctx.r11.s64 = -2113142784;
	// fmuls f1,f7,f10
	ctx.f1.f64 = double(float(ctx.f7.f64 * ctx.f10.f64));
	// fmuls f0,f0,f8
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f8.f64));
	// fmuls f10,f13,f0
	ctx.f10.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
	// fmuls f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
	// fmuls f13,f10,f12
	ctx.f13.f64 = double(float(ctx.f10.f64 * ctx.f12.f64));
	// fmuls f0,f0,f12
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f12.f64));
	// fmuls f13,f13,f31
	ctx.f13.f64 = double(float(ctx.f13.f64 * ctx.f31.f64));
	// fmuls f12,f0,f29
	ctx.f12.f64 = double(float(ctx.f0.f64 * ctx.f29.f64));
	// lfs f0,-4220(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -4220);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// fmuls f28,f13,f0
	ctx.f28.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
	// lfs f0,17224(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 17224);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f27,f12,f0
	ctx.f27.f64 = double(float(ctx.f12.f64 * ctx.f0.f64));
	// bge cr6,0x825339f4
	if (!ctx.cr6.lt) goto loc_825339F4;
	// fabs f0,f29
	ctx.f0.u64 = ctx.f29.u64 & ~0x8000000000000000;
	// fcmpu cr6,f0,f11
	ctx.cr6.compare(ctx.f0.f64, ctx.f11.f64);
	// bge cr6,0x825339f4
	if (!ctx.cr6.lt) goto loc_825339F4;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f1,17284(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 17284);
	ctx.f1.f64 = double(temp.f32);
loc_825339F4:
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,8(r29)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,2604(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 2604);
	ctx.f13.f64 = double(temp.f32);
	// fmuls f2,f0,f13
	ctx.f2.f64 = double(float(ctx.f0.f64 * ctx.f13.f64));
	// bl 0x82a02648
	ctx.lr = 0x82533A08;
	sub_82A02648(ctx, base);
	// frsp f0,f1
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(float(ctx.f1.f64));
	// lfs f11,516(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 516);
	ctx.f11.f64 = double(temp.f32);
	// lfs f12,512(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 512);
	ctx.f12.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f10,508(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 508);
	ctx.f10.f64 = double(temp.f32);
	// addi r6,r1,84
	ctx.r6.s64 = ctx.r1.s64 + 84;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// lwz r4,484(r31)
	ctx.r4.u64 = REX_LOAD_U32(ctx.r31.u32 + 484);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lfs f31,9096(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 9096);
	ctx.f31.f64 = double(temp.f32);
	// fsubs f13,f30,f0
	ctx.f13.f64 = double(float(ctx.f30.f64 - ctx.f0.f64));
	// fmuls f11,f11,f0
	ctx.f11.f64 = double(float(ctx.f11.f64 * ctx.f0.f64));
	// fmuls f9,f13,f27
	ctx.f9.f64 = double(float(ctx.f13.f64 * ctx.f27.f64));
	// fmadds f30,f12,f0,f9
	ctx.f30.f64 = double(float(std::fma(ctx.f12.f64, ctx.f0.f64, ctx.f9.f64)));
	// stfs f30,512(r31)
	temp.f32 = float(ctx.f30.f64);
	REX_STORE_U32(ctx.r31.u32 + 512, temp.u32);
	// fmadds f0,f13,f28,f11
	ctx.f0.f64 = double(float(std::fma(ctx.f13.f64, ctx.f28.f64, ctx.f11.f64)));
	// stfs f0,516(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 516, temp.u32);
	// fmr f29,f0
	ctx.f29.f64 = ctx.f0.f64;
native_mouse = gta4::input::TakeMouseRotation(base, native_control, ctx.r31.u32, native_fov, ctx.r28.u32 == 0);
ctx.f29.f64 += gta4::input::MouseRotationRate(native_mouse.yaw, gta4::input::LoadFloat(base, ctx.r29.u32 + 8));
ctx.f30.f64 += gta4::input::MouseRotationRate(native_mouse.pitch, gta4::input::LoadFloat(base, ctx.r29.u32 + 8));
	// lfs f0,8(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// stfs f29,84(r1)
	temp.f32 = float(ctx.f29.f64);
	REX_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// fmuls f0,f0,f29
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f29.f64));
	// fmadds f0,f0,f31,f10
	ctx.f0.f64 = double(float(std::fma(ctx.f0.f64, ctx.f31.f64, ctx.f10.f64)));
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// bl 0x82533638
	ctx.lr = 0x82533A68;
	sub_82533638(ctx, base);
	// clrlwi r11,r28,24
	ctx.r11.u64 = ctx.r28.u32 & 0xFF;
	// lis r30,-32254
	ctx.r30.s64 = -2113798144;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82533ba4
	if (ctx.cr6.eq) goto loc_82533BA4;
	// li r11,1
	ctx.r11.s64 = 1;
	// lfs f13,580(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 580);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,576(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 576);
	ctx.f12.f64 = double(temp.f32);
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// stb r11,676(r31)
	REX_STORE_U8(ctx.r31.u32 + 676, ctx.r11.u8);
	// lfs f0,8(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f0,f0,f29
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f29.f64));
	// fmadds f0,f0,f31,f13
	ctx.f0.f64 = double(float(std::fma(ctx.f0.f64, ctx.f31.f64, ctx.f13.f64)));
	// stfs f0,580(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 580, temp.u32);
	// lfs f0,8(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f0,f0,f30
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f30.f64));
	// fmadds f0,f0,f31,f12
	ctx.f0.f64 = double(float(std::fma(ctx.f0.f64, ctx.f31.f64, ctx.f12.f64)));
	// stfs f0,576(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 576, temp.u32);
	// bl 0x8237acf0
	ctx.lr = 0x82533AB4;
	sub_8237ACF0(ctx, base);
	// lfs f8,576(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 576);
	ctx.f8.f64 = double(temp.f32);
	// lfs f7,580(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 580);
	ctx.f7.f64 = double(temp.f32);
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// beq cr6,0x82533b1c
	if (ctx.cr6.eq) goto loc_82533B1C;
	// cmpwi cr6,r3,1
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 1, ctx.xer);
	// beq cr6,0x82533b1c
	if (ctx.cr6.eq) goto loc_82533B1C;
	// cmpwi cr6,r3,3
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 3, ctx.xer);
	// bne cr6,0x82533af8
	if (!ctx.cr6.eq) goto loc_82533AF8;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f13,16880(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 16880);
	ctx.f13.f64 = double(temp.f32);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f0,16876(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 16876);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f9,16904(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 16904);
	ctx.f9.f64 = double(temp.f32);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f12,16900(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 16900);
	ctx.f12.f64 = double(temp.f32);
	// b 0x82533b3c
	goto loc_82533B3C;
loc_82533AF8:
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f13,16864(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 16864);
	ctx.f13.f64 = double(temp.f32);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f0,16860(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 16860);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f9,16904(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 16904);
	ctx.f9.f64 = double(temp.f32);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f12,16868(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 16868);
	ctx.f12.f64 = double(temp.f32);
	// b 0x82533b3c
	goto loc_82533B3C;
loc_82533B1C:
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f13,16880(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 16880);
	ctx.f13.f64 = double(temp.f32);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f0,16876(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 16876);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f9,16888(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 16888);
	ctx.f9.f64 = double(temp.f32);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f12,16884(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 16884);
	ctx.f12.f64 = double(temp.f32);
loc_82533B3C:
	// fsubs f11,f8,f0
	ctx.fpscr.disableFlushMode();
	ctx.f11.f64 = double(float(ctx.f8.f64 - ctx.f0.f64));
	// lwz r11,304(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 304);
	// fsubs f10,f7,f12
	ctx.f10.f64 = double(float(ctx.f7.f64 - ctx.f12.f64));
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// fsel f11,f11,f8,f0
	ctx.f11.f64 = ctx.f11.f64 >= 0.0 ? ctx.f8.f64 : ctx.f0.f64;
	// fsel f10,f10,f7,f12
	ctx.f10.f64 = ctx.f10.f64 >= 0.0 ? ctx.f7.f64 : ctx.f12.f64;
	// fsubs f6,f11,f13
	ctx.f6.f64 = double(float(ctx.f11.f64 - ctx.f13.f64));
	// fsubs f5,f10,f9
	ctx.f5.f64 = double(float(ctx.f10.f64 - ctx.f9.f64));
	// fsel f11,f6,f13,f11
	ctx.f11.f64 = ctx.f6.f64 >= 0.0 ? ctx.f13.f64 : ctx.f11.f64;
	// stfs f11,576(r31)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r31.u32 + 576, temp.u32);
	// fsel f10,f5,f9,f10
	ctx.f10.f64 = ctx.f5.f64 >= 0.0 ? ctx.f9.f64 : ctx.f10.f64;
	// stfs f10,580(r31)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r31.u32 + 580, temp.u32);
	// bne cr6,0x82533bc0
	if (!ctx.cr6.eq) goto loc_82533BC0;
	// fcmpu cr6,f11,f8
	ctx.cr6.compare(ctx.f11.f64, ctx.f8.f64);
	// beq cr6,0x82533b88
	if (ctx.cr6.eq) goto loc_82533B88;
	// fsubs f11,f13,f0
	ctx.f11.f64 = double(float(ctx.f13.f64 - ctx.f0.f64));
	// lfs f13,17420(r30)
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + 17420);
	ctx.f13.f64 = double(temp.f32);
	// fmadds f0,f11,f13,f0
	ctx.f0.f64 = double(float(std::fma(ctx.f11.f64, ctx.f13.f64, ctx.f0.f64)));
	// stfs f0,576(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 576, temp.u32);
loc_82533B88:
	// fcmpu cr6,f10,f7
	ctx.fpscr.disableFlushMode();
	ctx.cr6.compare(ctx.f10.f64, ctx.f7.f64);
	// beq cr6,0x82533bc0
	if (ctx.cr6.eq) goto loc_82533BC0;
	// fsubs f13,f9,f12
	ctx.f13.f64 = double(float(ctx.f9.f64 - ctx.f12.f64));
	// lfs f0,17420(r30)
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + 17420);
	ctx.f0.f64 = double(temp.f32);
	// fmadds f0,f13,f0,f12
	ctx.f0.f64 = double(float(std::fma(ctx.f13.f64, ctx.f0.f64, ctx.f12.f64)));
	// stfs f0,580(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 580, temp.u32);
	// b 0x82533bc0
	goto loc_82533BC0;
loc_82533BA4:
	// lfs f0,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,508(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 508, temp.u32);
	// lfs f0,8(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f0,f0,f30
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f30.f64));
	// lfs f13,504(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 504);
	ctx.f13.f64 = double(temp.f32);
	// fmadds f0,f0,f31,f13
	ctx.f0.f64 = double(float(std::fma(ctx.f0.f64, ctx.f31.f64, ctx.f13.f64)));
	// stfs f0,504(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 504, temp.u32);
loc_82533BC0:
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// lfs f2,508(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 508);
	ctx.f2.f64 = double(temp.f32);
	// lfs f1,504(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 504);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x82369750
	ctx.lr = 0x82533BD0;
	sub_82369750(ctx, base);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r10,r1,84
	ctx.r10.s64 = ctx.r1.s64 + 84;
	// addi r9,r31,64
	ctx.r9.s64 = ctx.r31.s64 + 64;
	// li r8,352
	ctx.r8.s64 = 352;
	// lfs f0,3432(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 3432);
	ctx.f0.f64 = double(temp.f32);
	// addi r11,r1,96
	ctx.r11.s64 = ctx.r1.s64 + 96;
	// stfs f0,84(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// lvlx v13,0,r10
	temp.u32 = ctx.r10.u32;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
	// vspltw v13,v13,0
	simde_mm_store_si128((simde__m128i*)ctx.v13.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)ctx.v13.u32), 0xFF));
	// lvx128 v12,r0,r9
	ea = (ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// vmaddfp v0,v0,v13,v12
	ctx.fpscr.enableFlushModeUnconditional();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v13.f32)), simde_mm_load_ps(ctx.v12.f32)));
	// lfs f12,9764(r11)
	ctx.fpscr.disableFlushModeUnconditional();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 9764);
	ctx.f12.f64 = double(temp.f32);
	// stvx128 v0,r31,r8
	ea = (ctx.r31.u32 + ctx.r8.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lbz r11,480(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 480);
	// rlwinm r10,r11,25,7,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 25) & 0x1FFFFFF;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// addi r11,r11,16912
	ctx.r11.s64 = ctx.r11.s64 + 16912;
	// beq cr6,0x82533c38
	if (ctx.cr6.eq) goto loc_82533C38;
	// lwz r8,484(r31)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r31.u32 + 484);
	// lwz r8,640(r8)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r8.u32 + 640);
	// rlwinm r8,r8,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 2) & 0xFFFFFFFC;
	// lfsx f0,r8,r11
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + ctx.r11.u32);
	ctx.f0.f64 = double(temp.f32);
	// fsubs f12,f12,f0
	ctx.f12.f64 = double(float(ctx.f12.f64 - ctx.f0.f64));
loc_82533C38:
	// lfs f0,96(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 96);
	ctx.f0.f64 = double(temp.f32);
	// clrlwi r8,r25,24
	ctx.r8.u64 = ctx.r25.u32 & 0xFF;
	// fsubs f12,f12,f0
	ctx.f12.f64 = double(float(ctx.f12.f64 - ctx.f0.f64));
	// lfs f13,17420(r30)
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + 17420);
	ctx.f13.f64 = double(temp.f32);
	// cmplwi cr6,r8,0
	ctx.cr6.compare<uint32_t>(ctx.r8.u32, 0, ctx.xer);
	// fmadds f0,f12,f13,f0
	ctx.f0.f64 = double(float(std::fma(ctx.f12.f64, ctx.f13.f64, ctx.f0.f64)));
	// beq cr6,0x82533c80
	if (ctx.cr6.eq) goto loc_82533C80;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x82533c80
	if (ctx.cr6.eq) goto loc_82533C80;
	// lwz r10,484(r31)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r31.u32 + 484);
	// lfs f0,684(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 684);
	ctx.f0.f64 = double(temp.f32);
	// lwz r10,640(r10)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r10.u32 + 640);
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lfsx f12,r10,r11
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	ctx.f12.f64 = double(temp.f32);
	// fsubs f12,f12,f0
	ctx.f12.f64 = double(float(ctx.f12.f64 - ctx.f0.f64));
	// fmadds f0,f12,f13,f0
	ctx.f0.f64 = double(float(std::fma(ctx.f12.f64, ctx.f13.f64, ctx.f0.f64)));
	// stfs f0,684(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 684, temp.u32);
	// b 0x82533c90
	goto loc_82533C90;
loc_82533C80:
	// lfs f12,684(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 684);
	ctx.f12.f64 = double(temp.f32);
	// fmuls f13,f12,f13
	ctx.f13.f64 = double(float(ctx.f12.f64 * ctx.f13.f64));
	// stfs f0,96(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 96, temp.u32);
	// stfs f13,684(r31)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r31.u32 + 684, temp.u32);
loc_82533C90:
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lvx128 v0,r0,r9
	ea = (ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,84
	ctx.r10.s64 = ctx.r1.s64 + 84;
	// li r9,336
	ctx.r9.s64 = 336;
	// lfs f0,17636(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 17636);
	ctx.f0.f64 = double(temp.f32);
	// li r11,32
	ctx.r11.s64 = 32;
	// stfs f0,84(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// lvlx v13,0,r10
	temp.u32 = ctx.r10.u32;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
	// vspltw v13,v13,0
	simde_mm_store_si128((simde__m128i*)ctx.v13.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)ctx.v13.u32), 0xFF));
	// lvx128 v12,r31,r11
	ea = (ctx.r31.u32 + ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// vmaddfp v0,v12,v13,v0
	ctx.fpscr.enableFlushModeUnconditional();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(ctx.v12.f32), simde_mm_load_ps(ctx.v13.f32)), simde_mm_load_ps(ctx.v0.f32)));
	// stvx128 v0,r31,r9
	ea = (ctx.r31.u32 + ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
loc_82533CC0:
	// addi r1,r1,224
	ctx.r1.s64 = ctx.r1.s64 + 224;
	// addi r12,r1,-64
	ctx.r12.s64 = ctx.r1.s64 + -64;
	// bl 0x82a01370
	ctx.lr = 0x82533CCC;
	__restfpr_27(ctx, base);
	// b 0x829ff80c
	__restgprlr_25(ctx, base);
	return;
}

extern "C" void sub_825ED3C8(PPCContext& ctx, uint8_t* base) {
  gta4::input::MouseRotation native_mouse{};
  uint32_t native_control = 0;
	REX_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7b4
	ctx.lr = 0x825ED3D0;
	__savegprlr_23(ctx, base);
	// addi r12,r1,-80
	ctx.r12.s64 = ctx.r1.s64 + -80;
	// bl 0x82a0131c
	ctx.lr = 0x825ED3D8;
	__savefpr_25(ctx, base);
	// stwu r1,-336(r1)
	ea = -336 + ctx.r1.u32;
	REX_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lis r24,-32256
	ctx.r24.s64 = -2113929216;
	// lwz r27,320(r31)
	ctx.r27.u64 = REX_LOAD_U32(ctx.r31.u32 + 320);
	// lfs f0,2612(r24)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r24.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,116(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 116, temp.u32);
	// cmplwi cr6,r27,0
	ctx.cr6.compare<uint32_t>(ctx.r27.u32, 0, ctx.xer);
	// beq cr6,0x825ede4c
	if (ctx.cr6.eq) goto loc_825EDE4C;
	// lwz r11,40(r27)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r27.u32 + 40);
	// lis r10,192
	ctx.r10.s64 = 12582912;
	// rlwinm r11,r11,0,6,9
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x3C00000;
	// cmplw cr6,r11,r10
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, ctx.r10.u32, ctx.xer);
	// bne cr6,0x825ede4c
	if (!ctx.cr6.eq) goto loc_825EDE4C;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// bl 0x82369a08
	ctx.lr = 0x825ED418;
	sub_82369A08(ctx, base);
	// lhz r11,482(r27)
	ctx.r11.u64 = REX_LOAD_U16(ctx.r27.u32 + 482);
	// li r10,2
	ctx.r10.s64 = 2;
	// lwz r9,356(r31)
	ctx.r9.u64 = REX_LOAD_U32(ctx.r31.u32 + 356);
	// clrlwi r11,r11,28
	ctx.r11.u64 = ctx.r11.u32 & 0xF;
	// mr r28,r3
	ctx.r28.u64 = ctx.r3.u64;
	// subfc r11,r10,r11
	ctx.xer.ca = ctx.r11.u32 >= ctx.r10.u32;
	ctx.r11.u64 = ctx.r11.u64 - ctx.r10.u64;
	// li r30,1
	ctx.r30.s64 = 1;
	// subfe r11,r11,r11
	temp.u8 = (~ctx.r11.u32 + ctx.r11.u32 < ~ctx.r11.u32) | (~ctx.r11.u32 + ctx.r11.u32 + ctx.xer.ca < ctx.xer.ca);
	ctx.r11.u64 = ~ctx.r11.u64 + ctx.r11.u64 + ctx.xer.ca;
	ctx.xer.ca = temp.u8;
	// cmpwi cr6,r9,1
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 1, ctx.xer);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// li r23,0
	ctx.r23.s64 = 0;
	// clrlwi r25,r11,24
	ctx.r25.u64 = ctx.r11.u32 & 0xFF;
	// bne cr6,0x825ed48c
	if (!ctx.cr6.eq) goto loc_825ED48C;
	// lwz r11,572(r27)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r27.u32 + 572);
	// rlwinm r11,r11,0,2,2
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x20000000;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825ed468
	if (ctx.cr6.eq) goto loc_825ED468;
	// lwz r11,2688(r27)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r27.u32 + 2688);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825ed488
	if (!ctx.cr6.eq) goto loc_825ED488;
loc_825ED468:
	// lwz r11,540(r27)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r27.u32 + 540);
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,734
	ctx.r4.s64 = 734;
	// addi r3,r11,736
	ctx.r3.s64 = ctx.r11.s64 + 736;
	// bl 0x82159078
	ctx.lr = 0x825ED47C;
	sub_82159078(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825ed48c
	if (ctx.cr6.eq) goto loc_825ED48C;
loc_825ED488:
	// mr r30,r23
	ctx.r30.u64 = ctx.r23.u64;
loc_825ED48C:
	// clrlwi r11,r30,24
	ctx.r11.u64 = ctx.r30.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825ed4b0
	if (ctx.cr6.eq) goto loc_825ED4B0;
	// li r4,1
	ctx.r4.s64 = 1;
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// bl 0x823b26d8
	ctx.lr = 0x825ED4A4;
	sub_823B26D8(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// bl 0x822f2fd0
	ctx.lr = 0x825ED4B0;
	sub_822F2FD0(ctx, base);
loc_825ED4B0:
	// lwz r11,356(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 356);
	// lis r10,-32085
	ctx.r10.s64 = -2102722560;
	// rlwinm r9,r11,3,0,28
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 3) & 0xFFFFFFF8;
	// addi r10,r10,-16976
	ctx.r10.s64 = ctx.r10.s64 + -16976;
	// add r9,r11,r9
	ctx.r9.u64 = ctx.r11.u64 + ctx.r9.u64;
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// rlwinm r9,r9,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// add r26,r9,r10
	ctx.r26.u64 = ctx.r9.u64 + ctx.r10.u64;
	// bne cr6,0x825ed50c
	if (!ctx.cr6.eq) goto loc_825ED50C;
	// addi r9,r1,160
	ctx.r9.s64 = ctx.r1.s64 + 160;
	// mr r10,r26
	ctx.r10.u64 = ctx.r26.u64;
	// li r8,9
	ctx.r8.s64 = 9;
	// mtctr r8
	ctx.ctr.u64 = ctx.r8.u64;
loc_825ED4E4:
	// lwz r8,0(r10)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r10.u32 + 0);
	// addi r10,r10,4
	ctx.r10.s64 = ctx.r10.s64 + 4;
	// stw r8,0(r9)
	REX_STORE_U32(ctx.r9.u32 + 0, ctx.r8.u32);
	// addi r9,r9,4
	ctx.r9.s64 = ctx.r9.s64 + 4;
	// bdnz 0x825ed4e4
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_825ED4E4;
	// lfs f0,2944(r27)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r27.u32 + 2944);
	ctx.f0.f64 = double(temp.f32);
	// addi r26,r1,160
	ctx.r26.s64 = ctx.r1.s64 + 160;
	// stfs f0,172(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 172, temp.u32);
	// lfs f0,2940(r27)
	temp.u32 = REX_LOAD_U32(ctx.r27.u32 + 2940);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,168(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 168, temp.u32);
loc_825ED50C:
	// lbz r10,360(r31)
	ctx.r10.u64 = REX_LOAD_U8(ctx.r31.u32 + 360);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lfs f27,3400(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 3400);
	ctx.f27.f64 = double(temp.f32);
	// beq cr6,0x825ed5b8
	if (ctx.cr6.eq) goto loc_825ED5B8;
	// lfs f0,2544(r27)
	temp.u32 = REX_LOAD_U32(ctx.r27.u32 + 2544);
	ctx.f0.f64 = double(temp.f32);
	// addi r30,r31,328
	ctx.r30.s64 = ctx.r31.s64 + 328;
	// lfs f13,2612(r24)
	temp.u32 = REX_LOAD_U32(ctx.r24.u32 + 2612);
	ctx.f13.f64 = double(temp.f32);
	// addi r29,r31,324
	ctx.r29.s64 = ctx.r31.s64 + 324;
	// stfs f0,328(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 328, temp.u32);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// stfs f13,324(r31)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r31.u32 + 324, temp.u32);
	// bne cr6,0x825ed598
	if (!ctx.cr6.eq) goto loc_825ED598;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x823d3d08
	ctx.lr = 0x825ED548;
	sub_823D3D08(ctx, base);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x825ed598
	if (ctx.cr6.eq) goto loc_825ED598;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x823d3d08
	ctx.lr = 0x825ED558;
	sub_823D3D08(ctx, base);
	// li r4,1
	ctx.r4.s64 = 1;
	// li r5,0
	ctx.r5.s64 = 0;
	// bl 0x823d4518
	ctx.lr = 0x825ED564;
	sub_823D4518(ctx, base);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x825ed580
	if (ctx.cr6.eq) goto loc_825ED580;
	// lfs f0,772(r3)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 772);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r29)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r29.u32 + 0, temp.u32);
	// lfs f0,776(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 776);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r30)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r30.u32 + 0, temp.u32);
	// b 0x825ed598
	goto loc_825ED598;
loc_825ED580:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x823d3d08
	ctx.lr = 0x825ED588;
	sub_823D3D08(ctx, base);
	// mr r5,r30
	ctx.r5.u64 = ctx.r30.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// addi r3,r3,32
	ctx.r3.s64 = ctx.r3.s64 + 32;
	// bl 0x8236bfa0
	ctx.lr = 0x825ED598;
	sub_8236BFA0(ctx, base);
loc_825ED598:
	// lfs f0,2612(r24)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r24.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// fmr f13,f0
	ctx.f13.f64 = ctx.f0.f64;
	// stfs f0,332(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 332, temp.u32);
	// stfs f13,336(r31)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r31.u32 + 336, temp.u32);
	// stfs f27,340(r31)
	temp.f32 = float(ctx.f27.f64);
	REX_STORE_U32(ctx.r31.u32 + 340, temp.u32);
	// lfs f0,24(r26)
	temp.u32 = REX_LOAD_U32(ctx.r26.u32 + 24);
	ctx.f0.f64 = double(temp.f32);
	// stb r23,360(r31)
	REX_STORE_U8(ctx.r31.u32 + 360, ctx.r23.u8);
	// stfs f0,96(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 96, temp.u32);
loc_825ED5B8:
	// lfs f25,96(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 96);
	ctx.f25.f64 = double(temp.f32);
	// lfs f29,24(r26)
	temp.u32 = REX_LOAD_U32(ctx.r26.u32 + 24);
	ctx.f29.f64 = double(temp.f32);
	// lfs f30,28(r26)
	temp.u32 = REX_LOAD_U32(ctx.r26.u32 + 28);
	ctx.f30.f64 = double(temp.f32);
	// bl 0x826c4ca8
	ctx.lr = 0x825ED5C8;
	sub_826C4CA8(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825ed5d8
	if (ctx.cr6.eq) goto loc_825ED5D8;
	// lfs f30,32(r26)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r26.u32 + 32);
	ctx.f30.f64 = double(temp.f32);
loc_825ED5D8:
	// lis r11,-32057
	ctx.r11.s64 = -2100887552;
	// fcmpu cr6,f30,f29
	ctx.fpscr.disableFlushMode();
	ctx.cr6.compare(ctx.f30.f64, ctx.f29.f64);
	// addi r29,r11,-15708
	ctx.r29.s64 = ctx.r11.s64 + -15708;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,8(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// lfs f26,2604(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 2604);
	ctx.f26.f64 = double(temp.f32);
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// fmuls f31,f0,f26
	ctx.f31.f64 = double(float(ctx.f0.f64 * ctx.f26.f64));
	// addi r30,r11,-24840
	ctx.r30.s64 = ctx.r11.s64 + -24840;
	// bge cr6,0x825ed6e8
	if (!ctx.cr6.lt) goto loc_825ED6E8;
	// lwz r11,356(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 356);
	// cmpwi cr6,r11,1
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 1, ctx.xer);
	// beq cr6,0x825ed6e8
	if (ctx.cr6.eq) goto loc_825ED6E8;
	// lfs f0,2612(r24)
	temp.u32 = REX_LOAD_U32(ctx.r24.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// cmplwi cr6,r28,0
	ctx.cr6.compare<uint32_t>(ctx.r28.u32, 0, ctx.xer);
	// beq cr6,0x825ed67c
	if (ctx.cr6.eq) goto loc_825ED67C;
	// lwz r11,40(r30)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r30.u32 + 40);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x825ed648
	if (ctx.cr6.eq) goto loc_825ED648;
	// addi r11,r28,2640
	ctx.r11.s64 = ctx.r28.s64 + 2640;
	// lwz r10,8(r11)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r11.u32 + 8);
	// ld r3,0(r11)
	ctx.r3.u64 = REX_LOAD_U64(ctx.r11.u32 + 0);
	// rldicr r4,r10,32,63
	ctx.r4.u64 = __builtin_rotateleft64(ctx.r10.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// bl 0x822b7958
	ctx.lr = 0x825ED638;
	sub_822B7958(ctx, base);
	// extsw r11,r3
	ctx.r11.s64 = ctx.r3.s32;
	// std r11,88(r1)
	REX_STORE_U64(ctx.r1.u32 + 88, ctx.r11.u64);
	// lfd f0,88(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f0.u64 = REX_LOAD_U64(ctx.r1.u32 + 88);
	// b 0x825ed668
	goto loc_825ED668;
loc_825ED648:
	// addi r11,r28,2616
	ctx.r11.s64 = ctx.r28.s64 + 2616;
	// lwz r10,8(r11)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r11.u32 + 8);
	// ld r3,0(r11)
	ctx.r3.u64 = REX_LOAD_U64(ctx.r11.u32 + 0);
	// rldicr r4,r10,32,63
	ctx.r4.u64 = __builtin_rotateleft64(ctx.r10.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// bl 0x822b7958
	ctx.lr = 0x825ED65C;
	sub_822B7958(ctx, base);
	// extsw r11,r3
	ctx.r11.s64 = ctx.r3.s32;
	// std r11,88(r1)
	REX_STORE_U64(ctx.r1.u32 + 88, ctx.r11.u64);
	// lfd f0,88(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f0.u64 = REX_LOAD_U64(ctx.r1.u32 + 88);
loc_825ED668:
	// fcfid f0,f0
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(ctx.f0.s64);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// frsp f13,f0
	ctx.f13.f64 = double(float(ctx.f0.f64));
	// lfs f0,15428(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 15428);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
loc_825ED67C:
	// lis r11,-32244
	ctx.r11.s64 = -2113142784;
	// fneg f11,f0
	ctx.fpscr.disableFlushMode();
	ctx.f11.u64 = ctx.f0.u64 ^ 0x8000000000000000;
	// lfs f13,-4652(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -4652);
	ctx.f13.f64 = double(temp.f32);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// lfs f12,-27544(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -27544);
	ctx.f12.f64 = double(temp.f32);
	// lis r11,-32244
	ctx.r11.s64 = -2113142784;
	// fcmpu cr6,f0,f12
	ctx.cr6.compare(ctx.f0.f64, ctx.f12.f64);
	// fsel f12,f0,f0,f11
	ctx.f12.f64 = ctx.f0.f64 >= 0.0 ? ctx.f0.f64 : ctx.f11.f64;
	// fmuls f11,f12,f31
	ctx.f11.f64 = double(float(ctx.f12.f64 * ctx.f31.f64));
	// lfs f12,-4896(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -4896);
	ctx.f12.f64 = double(temp.f32);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// fmadds f12,f11,f13,f12
	ctx.f12.f64 = double(float(std::fma(ctx.f11.f64, ctx.f13.f64, ctx.f12.f64)));
	// lfs f13,-17764(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -17764);
	ctx.f13.f64 = double(temp.f32);
	// fmuls f13,f12,f13
	ctx.f13.f64 = double(float(ctx.f12.f64 * ctx.f13.f64));
	// ble cr6,0x825ed6c0
	if (!ctx.cr6.gt) goto loc_825ED6C0;
	// fmuls f25,f13,f25
	ctx.f25.f64 = double(float(ctx.f13.f64 * ctx.f25.f64));
	// b 0x825ed6d4
	goto loc_825ED6D4;
loc_825ED6C0:
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// lfs f12,-13584(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -13584);
	ctx.f12.f64 = double(temp.f32);
	// fcmpu cr6,f0,f12
	ctx.cr6.compare(ctx.f0.f64, ctx.f12.f64);
	// bge cr6,0x825ed6d4
	if (!ctx.cr6.lt) goto loc_825ED6D4;
	// fdivs f25,f25,f13
	ctx.f25.f64 = double(float(ctx.f25.f64 / ctx.f13.f64));
loc_825ED6D4:
	// fsubs f0,f30,f25
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(float(ctx.f30.f64 - ctx.f25.f64));
	// fsel f0,f0,f30,f25
	ctx.f0.f64 = ctx.f0.f64 >= 0.0 ? ctx.f30.f64 : ctx.f25.f64;
	// fsubs f13,f29,f0
	ctx.f13.f64 = double(float(ctx.f29.f64 - ctx.f0.f64));
	// fsel f25,f13,f0,f29
	ctx.f25.f64 = ctx.f13.f64 >= 0.0 ? ctx.f0.f64 : ctx.f29.f64;
	// stfs f25,96(r31)
	temp.f32 = float(ctx.f25.f64);
	REX_STORE_U32(ctx.r31.u32 + 96, temp.u32);
loc_825ED6E8:
	// lfs f29,2612(r24)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r24.u32 + 2612);
	ctx.f29.f64 = double(temp.f32);
	// cmplwi cr6,r28,0
	ctx.cr6.compare<uint32_t>(ctx.r28.u32, 0, ctx.xer);
	// fmr f28,f29
	ctx.f28.f64 = ctx.f29.f64;
	// beq cr6,0x825ed760
	if (ctx.cr6.eq) goto loc_825ED760;
	// lwz r11,40(r30)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r30.u32 + 40);
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x825ed72c
	if (ctx.cr6.eq) goto loc_825ED72C;
	// bl 0x822b7fc0
	ctx.lr = 0x825ED70C;
	sub_822B7FC0(ctx, base);
	// lwz r11,8(r3)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r3.u32 + 8);
	// ld r3,0(r3)
	ctx.r3.u64 = REX_LOAD_U64(ctx.r3.u32 + 0);
	// rldicr r4,r11,32,63
	ctx.r4.u64 = __builtin_rotateleft64(ctx.r11.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// bl 0x822b7a50
	ctx.lr = 0x825ED71C;
	sub_822B7A50(ctx, base);
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// fneg f29,f1
	ctx.fpscr.disableFlushMode();
	ctx.f29.u64 = ctx.f1.u64 ^ 0x8000000000000000;
	// bl 0x822b8018
	ctx.lr = 0x825ED728;
	sub_822B8018(ctx, base);
	// b 0x825ed74c
	goto loc_825ED74C;
loc_825ED72C:
	// bl 0x822b80c8
	ctx.lr = 0x825ED730;
	sub_822B80C8(ctx, base);
	// lwz r11,8(r3)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r3.u32 + 8);
	// ld r3,0(r3)
	ctx.r3.u64 = REX_LOAD_U64(ctx.r3.u32 + 0);
	// rldicr r4,r11,32,63
	ctx.r4.u64 = __builtin_rotateleft64(ctx.r11.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// bl 0x822b7a50
	ctx.lr = 0x825ED740;
	sub_822B7A50(ctx, base);
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// fneg f29,f1
	ctx.fpscr.disableFlushMode();
	ctx.f29.u64 = ctx.f1.u64 ^ 0x8000000000000000;
	// bl 0x822b8070
	ctx.lr = 0x825ED74C;
	sub_822B8070(ctx, base);
loc_825ED74C:
	// lwz r11,8(r3)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r3.u32 + 8);
	// ld r3,0(r3)
	ctx.r3.u64 = REX_LOAD_U64(ctx.r3.u32 + 0);
	// rldicr r4,r11,32,63
	ctx.r4.u64 = __builtin_rotateleft64(ctx.r11.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// bl 0x822b7ae0
	ctx.lr = 0x825ED75C;
	sub_822B7AE0(ctx, base);
	// fneg f28,f1
	ctx.fpscr.disableFlushMode();
	ctx.f28.u64 = ctx.f1.u64 ^ 0x8000000000000000;
loc_825ED760:
	// fmuls f0,f29,f29
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(float(ctx.f29.f64 * ctx.f29.f64));
	// fmadds f1,f28,f28,f0
	ctx.f1.f64 = double(float(std::fma(ctx.f28.f64, ctx.f28.f64, ctx.f0.f64)));
	// lfs f0,2612(r24)
	temp.u32 = REX_LOAD_U32(ctx.r24.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f1,f0
	ctx.cr6.compare(ctx.f1.f64, ctx.f0.f64);
	// ble cr6,0x825ed784
	if (!ctx.cr6.gt) goto loc_825ED784;
	// fsqrts f1,f1
	ctx.f1.f64 = double(float(sqrt(ctx.f1.f64)));
	// fdivs f0,f27,f1
	ctx.f0.f64 = double(float(ctx.f27.f64 / ctx.f1.f64));
	// fmuls f29,f0,f29
	ctx.f29.f64 = double(float(ctx.f0.f64 * ctx.f29.f64));
	// fmuls f28,f0,f28
	ctx.f28.f64 = double(float(ctx.f0.f64 * ctx.f28.f64));
loc_825ED784:
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// lwz r30,36(r30)
	ctx.r30.u64 = REX_LOAD_U32(ctx.r30.u32 + 36);
	// lfs f0,-16752(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -16752);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f1,f0
	ctx.cr6.compare(ctx.f1.f64, ctx.f0.f64);
	// ble cr6,0x825ed7d4
	if (!ctx.cr6.gt) goto loc_825ED7D4;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// lfs f13,340(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 340);
	ctx.f13.f64 = double(temp.f32);
	// rlwinm r11,r30,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f0,8(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// addi r10,r10,-13596
	ctx.r10.s64 = ctx.r10.s64 + -13596;
	// lis r9,-32254
	ctx.r9.s64 = -2113798144;
	// addi r9,r9,17652
	ctx.r9.s64 = ctx.r9.s64 + 17652;
	// lfsx f12,r11,r10
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + ctx.r10.u32);
	ctx.f12.f64 = double(temp.f32);
	// fmadds f0,f12,f0,f13
	ctx.f0.f64 = double(float(std::fma(ctx.f12.f64, ctx.f0.f64, ctx.f13.f64)));
	// stfs f0,340(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 340, temp.u32);
	// lfsx f13,r11,r9
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + ctx.r9.u32);
	ctx.f13.f64 = double(temp.f32);
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// ble cr6,0x825ed7d8
	if (!ctx.cr6.gt) goto loc_825ED7D8;
	// stfs f13,340(r31)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r31.u32 + 340, temp.u32);
	// b 0x825ed7d8
	goto loc_825ED7D8;
loc_825ED7D4:
	// stfs f27,340(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f27.f64);
	REX_STORE_U32(ctx.r31.u32 + 340, temp.u32);
loc_825ED7D8:
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// lfd f2,29720(r11)
	ctx.fpscr.disableFlushMode();
	ctx.f2.u64 = REX_LOAD_U64(ctx.r11.u32 + 29720);
	// bl 0x82a02648
	ctx.lr = 0x825ED7E4;
	sub_82A02648(ctx, base);
	// lfs f0,340(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 340);
	ctx.f0.f64 = double(temp.f32);
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// fsubs f11,f27,f0
	ctx.f11.f64 = double(float(ctx.f27.f64 - ctx.f0.f64));
	// lis r9,-32256
	ctx.r9.s64 = -2113929216;
	// addi r10,r10,-13608
	ctx.r10.s64 = ctx.r10.s64 + -13608;
	// frsp f12,f1
	ctx.f12.f64 = double(float(ctx.f1.f64));
	// rlwinm r11,r30,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f13,11864(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 11864);
	ctx.f13.f64 = double(temp.f32);
	// clrlwi r9,r25,24
	ctx.r9.u64 = ctx.r25.u32 & 0xFF;
	// fmuls f13,f25,f13
	ctx.f13.f64 = double(float(ctx.f25.f64 * ctx.f13.f64));
	// lis r25,-32252
	ctx.r25.s64 = -2113667072;
	// lfsx f10,r11,r10
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + ctx.r10.u32);
	ctx.f10.f64 = double(temp.f32);
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// fsel f0,f11,f27,f0
	ctx.f0.f64 = ctx.f11.f64 >= 0.0 ? ctx.f27.f64 : ctx.f0.f64;
	// fmuls f0,f0,f10
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f10.f64));
	// fmuls f11,f13,f0
	ctx.f11.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
	// fmuls f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
	// fmuls f13,f11,f12
	ctx.f13.f64 = double(float(ctx.f11.f64 * ctx.f12.f64));
	// fmuls f0,f0,f12
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f12.f64));
	// fmuls f13,f13,f29
	ctx.f13.f64 = double(float(ctx.f13.f64 * ctx.f29.f64));
	// fmuls f12,f0,f28
	ctx.f12.f64 = double(float(ctx.f0.f64 * ctx.f28.f64));
	// lfs f0,-13760(r25)
	temp.u32 = REX_LOAD_U32(ctx.r25.u32 + -13760);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f31,f13,f0
	ctx.f31.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
	// lfs f0,-13764(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + -13764);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f30,f12,f0
	ctx.f30.f64 = double(float(ctx.f12.f64 * ctx.f0.f64));
	// beq cr6,0x825ed860
	if (ctx.cr6.eq) goto loc_825ED860;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// lfs f0,-13756(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + -13756);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f31,f31,f0
	ctx.f31.f64 = double(float(ctx.f31.f64 * ctx.f0.f64));
	// fmuls f30,f30,f0
	ctx.f30.f64 = double(float(ctx.f30.f64 * ctx.f0.f64));
loc_825ED860:
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// addi r10,r10,17640
	ctx.r10.s64 = ctx.r10.s64 + 17640;
	// lfsx f13,r11,r10
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + ctx.r10.u32);
	ctx.f13.f64 = double(temp.f32);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// lfs f0,-13772(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -13772);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// fmuls f1,f13,f0
	ctx.f1.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
	// fabs f13,f29
	ctx.f13.u64 = ctx.f29.u64 & ~0x8000000000000000;
	// lfs f0,3528(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 3528);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f13,f0
	ctx.cr6.compare(ctx.f13.f64, ctx.f0.f64);
	// bge cr6,0x825ed8a0
	if (!ctx.cr6.lt) goto loc_825ED8A0;
	// fabs f13,f28
	ctx.f13.u64 = ctx.f28.u64 & ~0x8000000000000000;
	// fcmpu cr6,f13,f0
	ctx.cr6.compare(ctx.f13.f64, ctx.f0.f64);
	// bge cr6,0x825ed8a0
	if (!ctx.cr6.lt) goto loc_825ED8A0;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// lfs f1,-13768(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -13768);
	ctx.f1.f64 = double(temp.f32);
loc_825ED8A0:
	// lfs f0,8(r29)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f2,f0,f26
	ctx.f2.f64 = double(float(ctx.f0.f64 * ctx.f26.f64));
	// bl 0x82a02648
	ctx.lr = 0x825ED8AC;
	sub_82A02648(ctx, base);
	// frsp f0,f1
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(float(ctx.f1.f64));
	// lfs f11,336(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 336);
	ctx.f11.f64 = double(temp.f32);
	// lfs f10,332(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 332);
	ctx.f10.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f9,324(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 324);
	ctx.f9.f64 = double(temp.f32);
	// lfs f8,328(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 328);
	ctx.f8.f64 = double(temp.f32);
	// lfs f13,9096(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 9096);
	ctx.f13.f64 = double(temp.f32);
	// fsubs f12,f27,f0
	ctx.f12.f64 = double(float(ctx.f27.f64 - ctx.f0.f64));
	// fmuls f11,f11,f0
	ctx.f11.f64 = double(float(ctx.f11.f64 * ctx.f0.f64));
	// fmuls f7,f12,f30
	ctx.f7.f64 = double(float(ctx.f12.f64 * ctx.f30.f64));
	// fmadds f12,f12,f31,f11
	ctx.f12.f64 = double(float(std::fma(ctx.f12.f64, ctx.f31.f64, ctx.f11.f64)));
	// stfs f12,336(r31)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r31.u32 + 336, temp.u32);
	// fmadds f0,f10,f0,f7
	ctx.f0.f64 = double(float(std::fma(ctx.f10.f64, ctx.f0.f64, ctx.f7.f64)));
	// stfs f0,332(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 332, temp.u32);
	// fmr f11,f0
	ctx.f11.f64 = ctx.f0.f64;
native_mouse = gta4::input::TakeMouseRotation(base, ctx.r28.u32, ctx.r31.u32, ctx.f25.f64, true);
	// lfs f0,8(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f0,f11,f0
	ctx.f0.f64 = double(float(ctx.f11.f64 * ctx.f0.f64));
	// fmadds f0,f0,f13,f9
	ctx.f0.f64 = double(float(std::fma(ctx.f0.f64, ctx.f13.f64, ctx.f9.f64)));
ctx.f0.f64 += native_mouse.pitch;
	// stfs f0,324(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 324, temp.u32);
	// lfs f0,8(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f0,f12,f0
	ctx.f0.f64 = double(float(ctx.f12.f64 * ctx.f0.f64));
	// fmadds f1,f0,f13,f8
	ctx.f1.f64 = double(float(std::fma(ctx.f0.f64, ctx.f13.f64, ctx.f8.f64)));
ctx.f1.f64 += native_mouse.yaw;
	// stfs f1,328(r31)
	temp.f32 = float(ctx.f1.f64);
	REX_STORE_U32(ctx.r31.u32 + 328, temp.u32);
	// bl 0x82369828
	ctx.lr = 0x825ED90C;
	sub_82369828(ctx, base);
	// stfs f1,328(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	REX_STORE_U32(ctx.r31.u32 + 328, temp.u32);
	// lfs f0,8(r26)
	temp.u32 = REX_LOAD_U32(ctx.r26.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// fmr f11,f1
	ctx.f11.f64 = ctx.f1.f64;
	// lfs f13,324(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 324);
	ctx.f13.f64 = double(temp.f32);
	// fsubs f10,f13,f0
	ctx.f10.f64 = double(float(ctx.f13.f64 - ctx.f0.f64));
	// lfs f12,12(r26)
	temp.u32 = REX_LOAD_U32(ctx.r26.u32 + 12);
	ctx.f12.f64 = double(temp.f32);
	// fsel f0,f10,f0,f13
	ctx.f0.f64 = ctx.f10.f64 >= 0.0 ? ctx.f0.f64 : ctx.f13.f64;
	// fsubs f13,f0,f12
	ctx.f13.f64 = double(float(ctx.f0.f64 - ctx.f12.f64));
	// fsel f0,f13,f0,f12
	ctx.f0.f64 = ctx.f13.f64 >= 0.0 ? ctx.f0.f64 : ctx.f12.f64;
	// stfs f0,324(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 324, temp.u32);
	// lfs f0,16(r26)
	temp.u32 = REX_LOAD_U32(ctx.r26.u32 + 16);
	ctx.f0.f64 = double(temp.f32);
	// fsubs f12,f11,f0
	ctx.f12.f64 = double(float(ctx.f11.f64 - ctx.f0.f64));
	// lfs f13,20(r26)
	temp.u32 = REX_LOAD_U32(ctx.r26.u32 + 20);
	ctx.f13.f64 = double(temp.f32);
	// fsel f0,f12,f0,f11
	ctx.f0.f64 = ctx.f12.f64 >= 0.0 ? ctx.f0.f64 : ctx.f11.f64;
	// fsubs f12,f0,f13
	ctx.f12.f64 = double(float(ctx.f0.f64 - ctx.f13.f64));
	// fsel f1,f12,f0,f13
	ctx.f1.f64 = ctx.f12.f64 >= 0.0 ? ctx.f0.f64 : ctx.f13.f64;
	// stfs f1,328(r31)
	temp.f32 = float(ctx.f1.f64);
	REX_STORE_U32(ctx.r31.u32 + 328, temp.u32);
	// lwz r11,56(r27)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r27.u32 + 56);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x825ed96c
	if (ctx.cr6.eq) goto loc_825ED96C;
	// lwz r10,1796(r27)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r27.u32 + 1796);
	// cmpw cr6,r11,r10
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r10.s32, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// beq cr6,0x825ed970
	if (ctx.cr6.eq) goto loc_825ED970;
loc_825ED96C:
	// mr r11,r23
	ctx.r11.u64 = ctx.r23.u64;
loc_825ED970:
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825ed9a8
	if (!ctx.cr6.eq) goto loc_825ED9A8;
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// bl 0x822f10a8
	ctx.lr = 0x825ED984;
	sub_822F10A8(ctx, base);
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// lfs f1,328(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 328);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x822f10a0
	ctx.lr = 0x825ED990;
	sub_822F10A0(ctx, base);
	// lwz r11,0(r27)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r27.u32 + 0);
	// lfs f1,328(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 328);
	ctx.f1.f64 = double(temp.f32);
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// lwz r11,12(r11)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r11.u32 + 12);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x825ED9A8;
	REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
loc_825ED9A8:
	// lbz r11,244(r27)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r27.u32 + 244);
	// ori r11,r11,32
	ctx.r11.u64 = ctx.r11.u64 | 32;
	// stb r11,244(r27)
	REX_STORE_U8(ctx.r27.u32 + 244, ctx.r11.u8);
	// lwz r11,356(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 356);
	// cmpwi cr6,r11,1
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 1, ctx.xer);
	// bne cr6,0x825ed9f4
	if (!ctx.cr6.eq) goto loc_825ED9F4;
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// bl 0x822f03c0
	ctx.lr = 0x825ED9C8;
	sub_822F03C0(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825ed9f4
	if (ctx.cr6.eq) goto loc_825ED9F4;
	// li r6,1
	ctx.r6.s64 = 1;
	// li r5,1202
	ctx.r5.s64 = 1202;
	// addi r4,r1,112
	ctx.r4.s64 = ctx.r1.s64 + 112;
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// bl 0x822fc4c8
	ctx.lr = 0x825ED9E8;
	sub_822FC4C8(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// lfs f0,-13692(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -13692);
	ctx.f0.f64 = double(temp.f32);
	// b 0x825eda58
	goto loc_825EDA58;
loc_825ED9F4:
	// lwz r11,32(r27)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r27.u32 + 32);
	// li r10,48
	ctx.r10.s64 = 48;
	// lwz r9,304(r31)
	ctx.r9.u64 = REX_LOAD_U32(ctx.r31.u32 + 304);
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// lvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,112
	ctx.r11.s64 = ctx.r1.s64 + 112;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bne cr6,0x825eda1c
	if (!ctx.cr6.eq) goto loc_825EDA1C;
	// fmr f31,f27
	ctx.fpscr.disableFlushMode();
	ctx.f31.f64 = ctx.f27.f64;
	// b 0x825eda24
	goto loc_825EDA24;
loc_825EDA1C:
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// lfs f31,-13672(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -13672);
	ctx.f31.f64 = double(temp.f32);
loc_825EDA24:
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// bl 0x822f03c0
	ctx.lr = 0x825EDA2C;
	sub_822F03C0(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// lfs f0,352(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 352);
	ctx.f0.f64 = double(temp.f32);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825eda44
	if (ctx.cr6.eq) goto loc_825EDA44;
	// fneg f13,f0
	ctx.f13.u64 = ctx.f0.u64 ^ 0x8000000000000000;
	// b 0x825eda50
	goto loc_825EDA50;
loc_825EDA44:
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// lfs f13,-13680(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -13680);
	ctx.f13.f64 = double(temp.f32);
	// fsubs f13,f13,f0
	ctx.f13.f64 = double(float(ctx.f13.f64 - ctx.f0.f64));
loc_825EDA50:
	// fmadds f0,f13,f31,f0
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(float(std::fma(ctx.f13.f64, ctx.f31.f64, ctx.f0.f64)));
	// stfs f0,352(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 352, temp.u32);
loc_825EDA58:
	// lfs f13,120(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 120);
	ctx.f13.f64 = double(temp.f32);
	// addi r11,r1,88
	ctx.r11.s64 = ctx.r1.s64 + 88;
	// fadds f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 + ctx.f0.f64));
	// stfs f0,120(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 120, temp.u32);
	// lfs f0,0(r26)
	temp.u32 = REX_LOAD_U32(ctx.r26.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lwz r10,32(r27)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r27.u32 + 32);
	// addi r8,r1,112
	ctx.r8.s64 = ctx.r1.s64 + 112;
	// stfs f0,88(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// li r9,16
	ctx.r9.s64 = 16;
	// lfs f0,4(r26)
	temp.u32 = REX_LOAD_U32(ctx.r26.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lvlx v13,0,r11
	temp.u32 = ctx.r11.u32;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
	// addi r11,r1,112
	ctx.r11.s64 = ctx.r1.s64 + 112;
	// vspltw v13,v13,0
	simde_mm_store_si128((simde__m128i*)ctx.v13.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)ctx.v13.u32), 0xFF));
	// lfs f31,2612(r24)
	temp.u32 = REX_LOAD_U32(ctx.r24.u32 + 2612);
	ctx.f31.f64 = double(temp.f32);
	// addi r3,r27,640
	ctx.r3.s64 = ctx.r27.s64 + 640;
	// fmr f30,f31
	ctx.f30.f64 = ctx.f31.f64;
	// lvx128 v0,r0,r8
	ea = (ctx.r8.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lvx128 v12,r10,r9
	ea = (ctx.r10.u32 + ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// vmaddfp v0,v12,v13,v0
	ctx.fpscr.enableFlushModeUnconditional();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(ctx.v12.f32), simde_mm_load_ps(ctx.v13.f32)), simde_mm_load_ps(ctx.v0.f32)));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lfs f13,120(r1)
	ctx.fpscr.disableFlushModeUnconditional();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 120);
	ctx.f13.f64 = double(temp.f32);
	// fadds f0,f0,f13
	ctx.f0.f64 = double(float(ctx.f0.f64 + ctx.f13.f64));
	// stfs f0,120(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 120, temp.u32);
	// bl 0x823d4d60
	ctx.lr = 0x825EDAB8;
	sub_823D4D60(ctx, base);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x825edac8
	if (ctx.cr6.eq) goto loc_825EDAC8;
	// lwz r3,20(r3)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r3.u32 + 20);
	// b 0x825edacc
	goto loc_825EDACC;
loc_825EDAC8:
	// li r3,0
	ctx.r3.s64 = 0;
loc_825EDACC:
	// bl 0x82299ad0
	ctx.lr = 0x825EDAD0;
	sub_82299AD0(ctx, base);
	// li r11,-1
	ctx.r11.s64 = -1;
	// lwz r29,824(r27)
	ctx.r29.u64 = REX_LOAD_U32(ctx.r27.u32 + 824);
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r4,r1,96
	ctx.r4.s64 = ctx.r1.s64 + 96;
	// stb r23,80(r1)
	REX_STORE_U8(ctx.r1.u32 + 80, ctx.r23.u8);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// stw r11,96(r1)
	REX_STORE_U32(ctx.r1.u32 + 96, ctx.r11.u32);
	// lwz r3,540(r27)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r27.u32 + 540);
	// bl 0x823c0fd0
	ctx.lr = 0x825EDAF4;
	sub_823C0FD0(ctx, base);
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// beq cr6,0x825edb80
	if (ctx.cr6.eq) goto loc_825EDB80;
	// lwz r11,8(r30)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r30.u32 + 8);
	// cmpwi cr6,r11,1
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 1, ctx.xer);
	// beq cr6,0x825edb28
	if (ctx.cr6.eq) goto loc_825EDB28;
	// cmpwi cr6,r11,2
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 2, ctx.xer);
	// beq cr6,0x825edb28
	if (ctx.cr6.eq) goto loc_825EDB28;
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x825edb28
	if (ctx.cr6.eq) goto loc_825EDB28;
	// cmpwi cr6,r11,3
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 3, ctx.xer);
	// beq cr6,0x825edb28
	if (ctx.cr6.eq) goto loc_825EDB28;
	// cmpwi cr6,r11,5
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 5, ctx.xer);
	// bne cr6,0x825edb80
	if (!ctx.cr6.eq) goto loc_825EDB80;
loc_825EDB28:
	// cmplwi cr6,r29,0
	ctx.cr6.compare<uint32_t>(ctx.r29.u32, 0, ctx.xer);
	// bne cr6,0x825edb54
	if (!ctx.cr6.eq) goto loc_825EDB54;
	// lwz r11,544(r27)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r27.u32 + 544);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// addi r11,r11,96
	ctx.r11.s64 = ctx.r11.s64 + 96;
	// bne cr6,0x825edb44
	if (!ctx.cr6.eq) goto loc_825EDB44;
	// mr r11,r23
	ctx.r11.u64 = ctx.r23.u64;
loc_825EDB44:
	// lwz r11,960(r11)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r11.u32 + 960);
	// rlwinm r11,r11,0,3,3
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x10000000;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825edb80
	if (ctx.cr6.eq) goto loc_825EDB80;
loc_825EDB54:
	// lwz r11,32(r30)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r30.u32 + 32);
	// rlwinm r11,r11,29,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 29) & 0x1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825edb70
	if (ctx.cr6.eq) goto loc_825EDB70;
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825edb78
	if (ctx.cr6.eq) goto loc_825EDB78;
loc_825EDB70:
	// cmplwi cr6,r29,0
	ctx.cr6.compare<uint32_t>(ctx.r29.u32, 0, ctx.xer);
	// beq cr6,0x825edb80
	if (ctx.cr6.eq) goto loc_825EDB80;
loc_825EDB78:
	// li r11,1
	ctx.r11.s64 = 1;
	// b 0x825edb84
	goto loc_825EDB84;
loc_825EDB80:
	// mr r11,r23
	ctx.r11.u64 = ctx.r23.u64;
loc_825EDB84:
	// clrlwi r28,r11,24
	ctx.r28.u64 = ctx.r11.u32 & 0xFF;
	// lis r30,-32057
	ctx.r30.s64 = -2100887552;
	// mr r29,r28
	ctx.r29.u64 = ctx.r28.u64;
	// cmplwi cr6,r29,0
	ctx.cr6.compare<uint32_t>(ctx.r29.u32, 0, ctx.xer);
	// beq cr6,0x825edc5c
	if (ctx.cr6.eq) goto loc_825EDC5C;
	// lbz r11,361(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 361);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825edbb8
	if (ctx.cr6.eq) goto loc_825EDBB8;
	// lfs f13,344(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 344);
	ctx.f13.f64 = double(temp.f32);
	// stb r23,361(r31)
	REX_STORE_U8(ctx.r31.u32 + 361, ctx.r23.u8);
	// lfs f0,-13760(r25)
	temp.u32 = REX_LOAD_U32(ctx.r25.u32 + -13760);
	ctx.f0.f64 = double(temp.f32);
	// fadds f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 + ctx.f0.f64));
	// stfs f0,344(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 344, temp.u32);
loc_825EDBB8:
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// lfs f12,344(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 344);
	ctx.f12.f64 = double(temp.f32);
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// lfs f13,-13696(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -13696);
	ctx.f13.f64 = double(temp.f32);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// lfs f29,-13724(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + -13724);
	ctx.f29.f64 = double(temp.f32);
	// lfs f0,-13700(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -13700);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// fmuls f0,f12,f0
	ctx.f0.f64 = double(float(ctx.f12.f64 * ctx.f0.f64));
	// lfs f12,348(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 348);
	ctx.f12.f64 = double(temp.f32);
	// stfs f0,344(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 344, temp.u32);
	// lfs f30,-13736(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -13736);
	ctx.f30.f64 = double(temp.f32);
	// fmadds f0,f12,f13,f0
	ctx.f0.f64 = double(float(std::fma(ctx.f12.f64, ctx.f13.f64, ctx.f0.f64)));
	// stfs f0,348(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 348, temp.u32);
	// lwz r10,-15772(r30)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r30.u32 + -15772);
	// std r10,88(r1)
	REX_STORE_U64(ctx.r1.u32 + 88, ctx.r10.u64);
	// lfd f0,88(r1)
	ctx.f0.u64 = REX_LOAD_U64(ctx.r1.u32 + 88);
	// fcfid f0,f0
	ctx.f0.f64 = double(ctx.f0.s64);
	// frsp f13,f0
	ctx.f13.f64 = double(float(ctx.f0.f64));
	// fmr f0,f30
	ctx.f0.f64 = ctx.f30.f64;
	// fmuls f31,f13,f0
	ctx.f31.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
	// bl 0x822f03c0
	ctx.lr = 0x825EDC14;
	sub_822F03C0(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825edc28
	if (ctx.cr6.eq) goto loc_825EDC28;
	// lfs f30,2612(r24)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r24.u32 + 2612);
	ctx.f30.f64 = double(temp.f32);
	// fmr f29,f30
	ctx.f29.f64 = ctx.f30.f64;
loc_825EDC28:
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// lfs f0,-13612(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -13612);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f1,f31,f0
	ctx.f1.f64 = double(float(ctx.f31.f64 * ctx.f0.f64));
	// bl 0x829ffd48
	ctx.lr = 0x825EDC38;
	sub_829FFD48(ctx, base);
	// fmr f13,f1
	ctx.fpscr.disableFlushMode();
	ctx.f13.f64 = ctx.f1.f64;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// lfs f0,-13616(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -13616);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f1,f31,f0
	ctx.f1.f64 = double(float(ctx.f31.f64 * ctx.f0.f64));
	// frsp f0,f13
	ctx.f0.f64 = double(float(ctx.f13.f64));
	// fmuls f31,f0,f30
	ctx.f31.f64 = double(float(ctx.f0.f64 * ctx.f30.f64));
	// bl 0x829ffd48
	ctx.lr = 0x825EDC54;
	sub_829FFD48(ctx, base);
	// frsp f0,f1
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(float(ctx.f1.f64));
	// fmuls f30,f0,f29
	ctx.f30.f64 = double(float(ctx.f0.f64 * ctx.f29.f64));
loc_825EDC5C:
	// lis r9,-31992
	ctx.r9.s64 = -2096627712;
	// lfs f12,2612(r24)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r24.u32 + 2612);
	ctx.f12.f64 = double(temp.f32);
	// mr r11,r23
	ctx.r11.u64 = ctx.r23.u64;
	// cmplwi cr6,r29,0
	ctx.cr6.compare<uint32_t>(ctx.r29.u32, 0, ctx.xer);
	// stb r11,6791(r9)
	REX_STORE_U8(ctx.r9.u32 + 6791, ctx.r11.u8);
	// bne cr6,0x825edc80
	if (!ctx.cr6.eq) goto loc_825EDC80;
	// lwz r11,356(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 356);
	// cmpwi cr6,r11,4
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 4, ctx.xer);
	// bne cr6,0x825edd24
	if (!ctx.cr6.eq) goto loc_825EDD24;
loc_825EDC80:
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// lwz r10,-15772(r30)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r30.u32 + -15772);
	// lwz r11,6792(r11)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r11.u32 + 6792);
	// subf r10,r11,r10
	ctx.r10.u64 = ctx.r10.u64 - ctx.r11.u64;
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// lwz r11,-16792(r11)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r11.u32 + -16792);
	// cmplw cr6,r10,r11
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, ctx.r11.u32, ctx.xer);
	// bge cr6,0x825edca8
	if (!ctx.cr6.lt) goto loc_825EDCA8;
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,6791(r9)
	REX_STORE_U8(ctx.r9.u32 + 6791, ctx.r11.u8);
loc_825EDCA8:
	// lis r10,-32085
	ctx.r10.s64 = -2102722560;
	// lwz r11,356(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 356);
	// lfs f13,116(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 116);
	ctx.f13.f64 = double(temp.f32);
	// cmplwi cr6,r29,0
	ctx.cr6.compare<uint32_t>(ctx.r29.u32, 0, ctx.xer);
	// addi r11,r11,-4
	ctx.r11.s64 = ctx.r11.s64 + -4;
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// lfs f0,-16796(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + -16796);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// fadds f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 + ctx.f0.f64));
	// lis r10,-31992
	ctx.r10.s64 = -2096627712;
	// lfs f13,-16788(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -16788);
	ctx.f13.f64 = double(temp.f32);
	// rlwinm r11,r9,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// fsubs f11,f0,f13
	ctx.f11.f64 = double(float(ctx.f0.f64 - ctx.f13.f64));
	// fsel f0,f11,f13,f0
	ctx.f0.f64 = ctx.f11.f64 >= 0.0 ? ctx.f13.f64 : ctx.f0.f64;
	// fneg f13,f0
	ctx.f13.u64 = ctx.f0.u64 ^ 0x8000000000000000;
	// fsel f0,f13,f12,f0
	ctx.f0.f64 = ctx.f13.f64 >= 0.0 ? ctx.f12.f64 : ctx.f0.f64;
	// fsubs f13,f27,f0
	ctx.f13.f64 = double(float(ctx.f27.f64 - ctx.f0.f64));
	// fsel f0,f13,f0,f27
	ctx.f0.f64 = ctx.f13.f64 >= 0.0 ? ctx.f0.f64 : ctx.f27.f64;
	// stfs f0,116(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 116, temp.u32);
	// stb r11,6788(r10)
	REX_STORE_U8(ctx.r10.u32 + 6788, ctx.r11.u8);
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// stb r28,6789(r11)
	REX_STORE_U8(ctx.r11.u32 + 6789, ctx.r28.u8);
	// beq cr6,0x825edd18
	if (ctx.cr6.eq) goto loc_825EDD18;
	// lis r11,-32244
	ctx.r11.s64 = -2113142784;
	// lfs f0,-4228(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -4228);
	ctx.f0.f64 = double(temp.f32);
	// li r11,1
	ctx.r11.s64 = 1;
	// fcmpu cr6,f25,f0
	ctx.cr6.compare(ctx.f25.f64, ctx.f0.f64);
	// blt cr6,0x825edd1c
	if (ctx.cr6.lt) goto loc_825EDD1C;
loc_825EDD18:
	// mr r11,r23
	ctx.r11.u64 = ctx.r23.u64;
loc_825EDD1C:
	// lis r10,-31992
	ctx.r10.s64 = -2096627712;
	// stb r11,6790(r10)
	REX_STORE_U8(ctx.r10.u32 + 6790, ctx.r11.u8);
loc_825EDD24:
	// lwz r11,304(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 304);
	// cmplwi cr6,r11,2
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 2, ctx.xer);
	// bgt cr6,0x825edd34
	if (ctx.cr6.gt) goto loc_825EDD34;
	// stfs f12,116(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r31.u32 + 116, temp.u32);
loc_825EDD34:
	// clrldi r11,r11,32
	ctx.r11.u64 = ctx.r11.u64 & 0xFFFFFFFF;
	// lfs f13,348(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 348);
	ctx.f13.f64 = double(temp.f32);
	// addi r30,r31,16
	ctx.r30.s64 = ctx.r31.s64 + 16;
	// lfs f12,328(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 328);
	ctx.f12.f64 = double(temp.f32);
	// lfs f11,324(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 324);
	ctx.f11.f64 = double(temp.f32);
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// std r11,88(r1)
	REX_STORE_U64(ctx.r1.u32 + 88, ctx.r11.u64);
	// lfd f0,88(r1)
	ctx.f0.u64 = REX_LOAD_U64(ctx.r1.u32 + 88);
	// fcfid f0,f0
	ctx.f0.f64 = double(ctx.f0.s64);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// frsp f10,f0
	ctx.f10.f64 = double(float(ctx.f0.f64));
	// lfs f0,-13728(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -13728);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f0,f10,f0
	ctx.f0.f64 = double(float(ctx.f10.f64 * ctx.f0.f64));
	// fsubs f10,f27,f0
	ctx.f10.f64 = double(float(ctx.f27.f64 - ctx.f0.f64));
	// fsel f0,f10,f0,f27
	ctx.f0.f64 = ctx.f10.f64 >= 0.0 ? ctx.f0.f64 : ctx.f27.f64;
	// fmadds f13,f0,f30,f13
	ctx.f13.f64 = double(float(std::fma(ctx.f0.f64, ctx.f30.f64, ctx.f13.f64)));
	// fmadds f2,f0,f31,f12
	ctx.f2.f64 = double(float(std::fma(ctx.f0.f64, ctx.f31.f64, ctx.f12.f64)));
	// fadds f1,f13,f11
	ctx.f1.f64 = double(float(ctx.f13.f64 + ctx.f11.f64));
	// bl 0x823710b8
	ctx.lr = 0x825EDD80;
	sub_823710B8(ctx, base);
	// addi r10,r1,112
	ctx.r10.s64 = ctx.r1.s64 + 112;
	// addi r11,r31,64
	ctx.r11.s64 = ctx.r31.s64 + 64;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,144
	ctx.r11.s64 = ctx.r1.s64 + 144;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x82370788
	ctx.lr = 0x825EDDA4;
	sub_82370788(ctx, base);
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// vspltisw v9,0
	simde_mm_store_si128((simde__m128i*)ctx.v9.u32, simde_mm_set1_epi32(int(0x0)));
	// addi r9,r1,88
	ctx.r9.s64 = ctx.r1.s64 + 88;
	// addi r10,r10,-18560
	ctx.r10.s64 = ctx.r10.s64 + -18560;
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// addi r4,r1,128
	ctx.r4.s64 = ctx.r1.s64 + 128;
	// addi r3,r1,144
	ctx.r3.s64 = ctx.r1.s64 + 144;
	// lvx128 v12,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r10,-32080
	ctx.r10.s64 = -2102394880;
	// lvx128 v13,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,128
	ctx.r11.s64 = ctx.r1.s64 + 128;
	// addi r10,r10,29104
	ctx.r10.s64 = ctx.r10.s64 + 29104;
	// stvx128 v13,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,128
	ctx.r11.s64 = ctx.r1.s64 + 128;
	// lvx128 v11,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v11.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r10,-32080
	ctx.r10.s64 = -2102394880;
	// addi r10,r10,29088
	ctx.r10.s64 = ctx.r10.s64 + 29088;
	// lvx128 v10,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v10.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// lfs f0,-27028(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + -27028);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,88(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// lvlx v0,0,r9
	temp.u32 = ctx.r9.u32;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
	// vspltw v8,v0,0
	simde_mm_store_si128((simde__m128i*)ctx.v8.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)ctx.v0.u32), 0xFF));
	// vmsum3fp128 v0,v13,v13
	ctx.fpscr.enableFlushModeUnconditional();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_dp_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v13.f32), 0xEF));
	// vor v7,v0,v0
	simde_mm_store_si128((simde__m128i*)ctx.v7.u8, simde_mm_load_si128((simde__m128i*)ctx.v0.u8));
	// vrsqrtefp v0,v0
	simde_mm_store_ps(ctx.v0.f32, simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(simde_mm_load_ps(ctx.v0.f32))));
	// vor v6,v0,v0
	simde_mm_store_si128((simde__m128i*)ctx.v6.u8, simde_mm_load_si128((simde__m128i*)ctx.v0.u8));
	// vcmpeqfp v0,v0,v12
	simde_mm_store_ps(ctx.v0.f32, simde_mm_cmpeq_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v12.f32)));
	// vsel v0,v6,v9,v0
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)ctx.v6.u8)), simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)ctx.v9.u8))));
	// vmulfp128 v12,v0,v0
	simde_mm_store_ps(ctx.v12.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v0.f32)));
	// vmulfp128 v11,v11,v0
	simde_mm_store_ps(ctx.v11.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v11.f32), simde_mm_load_ps(ctx.v0.f32)));
	// vnmsubfp v12,v7,v12,v10
	simde_mm_store_ps(ctx.v12.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(ctx.v7.f32), simde_mm_load_ps(ctx.v12.f32)), simde_mm_load_ps(ctx.v10.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
	// vmaddfp v0,v12,v11,v0
	simde_mm_store_ps(ctx.v0.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(ctx.v12.f32), simde_mm_load_ps(ctx.v11.f32)), simde_mm_load_ps(ctx.v0.f32)));
	// vmulfp128 v0,v13,v0
	simde_mm_store_ps(ctx.v0.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v0.f32)));
	// vmulfp128 v0,v0,v8
	simde_mm_store_ps(ctx.v0.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v8.f32)));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x8251f840
	ctx.lr = 0x825EDE38;
	sub_8251F840(ctx, base);
	// li r3,1
	ctx.r3.s64 = 1;
	// addi r1,r1,336
	ctx.r1.s64 = ctx.r1.s64 + 336;
	// addi r12,r1,-80
	ctx.r12.s64 = ctx.r1.s64 + -80;
	// bl 0x82a01368
	ctx.lr = 0x825EDE48;
	__restfpr_25(ctx, base);
	// b 0x829ff804
	__restgprlr_23(ctx, base);
	return;
loc_825EDE4C:
	// li r3,0
	ctx.r3.s64 = 0;
	// addi r1,r1,336
	ctx.r1.s64 = ctx.r1.s64 + 336;
	// addi r12,r1,-80
	ctx.r12.s64 = ctx.r1.s64 + -80;
	// bl 0x82a01368
	ctx.lr = 0x825EDE5C;
	__restfpr_25(ctx, base);
	// b 0x829ff804
	__restgprlr_23(ctx, base);
	return;
}

extern "C" void sub_82542F80(PPCContext& ctx, uint8_t* base) {
  gta4::input::MouseRotation native_mouse{};
  uint32_t native_control = 0;
  bool native_yaw_allowed = true;
	REX_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff790
	ctx.lr = 0x82542F88;
	__savegprlr_14(ctx, base);
	// addi r12,r1,-152
	ctx.r12.s64 = ctx.r1.s64 + -152;
	// bl 0x82a01300
	ctx.lr = 0x82542F90;
	__savefpr_18(ctx, base);
	// li r12,-288
	ctx.r12.s64 = -288;
	// stvx128 v127,r1,r12
	ea = (ctx.r1.u32 + ctx.r12.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v127.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stwu r1,-560(r1)
	ea = -560 + ctx.r1.u32;
	REX_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r24,0
	ctx.r24.s64 = 0;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lis r10,-31994
	ctx.r10.s64 = -2096758784;
	// addi r30,r31,896
	ctx.r30.s64 = ctx.r31.s64 + 896;
	// lfs f20,2612(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 2612);
	ctx.f20.f64 = double(temp.f32);
	// mr r11,r24
	ctx.r11.u64 = ctx.r24.u64;
	// stfs f20,116(r31)
	temp.f32 = float(ctx.f20.f64);
	REX_STORE_U32(ctx.r31.u32 + 116, temp.u32);
	// stw r11,-27248(r10)
	REX_STORE_U32(ctx.r10.u32 + -27248, ctx.r11.u32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lis r10,-31994
	ctx.r10.s64 = -2096758784;
	// lfs f0,21984(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 21984);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// stfs f0,21440(r11)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r11.u32 + 21440, temp.u32);
	// mr r11,r24
	ctx.r11.u64 = ctx.r24.u64;
	// stw r11,-27244(r10)
	REX_STORE_U32(ctx.r10.u32 + -27244, ctx.r11.u32);
	// lwz r11,896(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 896);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82542ff8
	if (!ctx.cr6.eq) goto loc_82542FF8;
	// bl 0x8236bcf0
	ctx.lr = 0x82542FEC;
	sub_8236BCF0(ctx, base);
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// stw r3,0(r30)
	REX_STORE_U32(ctx.r30.u32 + 0, ctx.r3.u32);
	// bl 0x823c0598
	ctx.lr = 0x82542FF8;
	sub_823C0598(ctx, base);
loc_82542FF8:
	// lwz r30,0(r30)
	ctx.r30.u64 = REX_LOAD_U32(ctx.r30.u32 + 0);
	// lis r11,192
	ctx.r11.s64 = 12582912;
	// lwz r10,40(r30)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r30.u32 + 40);
	// rlwinm r10,r10,0,6,9
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0x3C00000;
	// cmplw cr6,r10,r11
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, ctx.r11.u32, ctx.xer);
	// beq cr6,0x82543018
	if (ctx.cr6.eq) goto loc_82543018;
	// li r3,0
	ctx.r3.s64 = 0;
	// b 0x82543c04
	goto loc_82543C04;
loc_82543018:
	// lis r11,-32057
	ctx.r11.s64 = -2100887552;
	// lis r14,-32255
	ctx.r14.s64 = -2113863680;
	// lbz r10,-15679(r11)
	ctx.r10.u64 = REX_LOAD_U8(ctx.r11.u32 + -15679);
	// lis r11,-32057
	ctx.r11.s64 = -2100887552;
	// lbz r11,-15678(r11)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r11.u32 + -15678);
	// or r11,r10,r11
	ctx.r11.u64 = ctx.r10.u64 | ctx.r11.u64;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82543040
	if (ctx.cr6.eq) goto loc_82543040;
	// lfs f21,-17764(r14)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r14.u32 + -17764);
	ctx.f21.f64 = double(temp.f32);
	// b 0x82543058
	goto loc_82543058;
loc_82543040:
	// lis r11,-32057
	ctx.r11.s64 = -2100887552;
	// addi r11,r11,-15772
	ctx.r11.s64 = ctx.r11.s64 + -15772;
	// lfs f0,8(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f13,2604(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 2604);
	ctx.f13.f64 = double(temp.f32);
	// fmuls f21,f0,f13
	ctx.f21.f64 = double(float(ctx.f0.f64 * ctx.f13.f64));
loc_82543058:
	// lhz r10,482(r30)
	ctx.r10.u64 = REX_LOAD_U16(ctx.r30.u32 + 482);
	// li r11,2
	ctx.r11.s64 = 2;
	// lwz r9,2560(r30)
	ctx.r9.u64 = REX_LOAD_U32(ctx.r30.u32 + 2560);
	// clrlwi r10,r10,28
	ctx.r10.u64 = ctx.r10.u32 & 0xF;
	// lwz r25,540(r30)
	ctx.r25.u64 = REX_LOAD_U32(ctx.r30.u32 + 540);
	// subfc r11,r11,r10
	ctx.xer.ca = ctx.r10.u32 >= ctx.r11.u32;
	ctx.r11.u64 = ctx.r10.u64 - ctx.r11.u64;
	// subfe r11,r11,r11
	temp.u8 = (~ctx.r11.u32 + ctx.r11.u32 < ~ctx.r11.u32) | (~ctx.r11.u32 + ctx.r11.u32 + ctx.xer.ca < ctx.xer.ca);
	ctx.r11.u64 = ~ctx.r11.u64 + ctx.r11.u64 + ctx.xer.ca;
	ctx.xer.ca = temp.u8;
	// stw r9,124(r1)
	REX_STORE_U32(ctx.r1.u32 + 124, ctx.r9.u32);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82543094
	if (ctx.cr6.eq) goto loc_82543094;
	// lwz r11,444(r30)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r30.u32 + 444);
	// stw r11,120(r1)
	REX_STORE_U32(ctx.r1.u32 + 120, ctx.r11.u32);
	// b 0x82543098
	goto loc_82543098;
loc_82543094:
	// stw r24,120(r1)
	REX_STORE_U32(ctx.r1.u32 + 120, ctx.r24.u32);
loc_82543098:
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82369a08
	ctx.lr = 0x825430A4;
	sub_82369A08(ctx, base);
	// lwz r11,2512(r30)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r30.u32 + 2512);
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// lwz r10,0(r11)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r11,24(r10)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r10.u32 + 24);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x825430C0;
	REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r18,r3
	ctx.r18.u64 = ctx.r3.u64;
	// li r4,123
	ctx.r4.s64 = 123;
	// lwz r3,120(r30)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r30.u32 + 120);
	// clrlwi r11,r18,24
	ctx.r11.u64 = ctx.r18.u32 & 0xFF;
	// subfic r11,r11,0
	ctx.xer.ca = ctx.r11.u32 <= 0;
	ctx.r11.u64 = static_cast<uint64_t>(0) - ctx.r11.u64;
	// subfe r11,r11,r11
	temp.u8 = (~ctx.r11.u32 + ctx.r11.u32 < ~ctx.r11.u32) | (~ctx.r11.u32 + ctx.r11.u32 + ctx.xer.ca < ctx.xer.ca);
	ctx.r11.u64 = ~ctx.r11.u64 + ctx.r11.u64 + ctx.xer.ca;
	ctx.xer.ca = temp.u8;
	// clrlwi r16,r11,30
	ctx.r16.u64 = ctx.r11.u32 & 0x3;
	// bl 0x824450d0
	ctx.lr = 0x825430E0;
	sub_824450D0(ctx, base);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// bne cr6,0x82543100
	if (!ctx.cr6.eq) goto loc_82543100;
	// li r4,124
	ctx.r4.s64 = 124;
	// lwz r3,120(r30)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r30.u32 + 120);
	// bl 0x824450d0
	ctx.lr = 0x825430F4;
	sub_824450D0(ctx, base);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// mr r11,r24
	ctx.r11.u64 = ctx.r24.u64;
	// beq cr6,0x82543104
	if (ctx.cr6.eq) goto loc_82543104;
loc_82543100:
	// li r11,1
	ctx.r11.s64 = 1;
loc_82543104:
	// lwz r10,0(r30)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r30.u32 + 0);
	// clrlwi r28,r11,24
	ctx.r28.u64 = ctx.r11.u32 & 0xFF;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// addi r3,r1,208
	ctx.r3.s64 = ctx.r1.s64 + 208;
	// lwz r11,232(r10)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r10.u32 + 232);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x82543120;
	REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// lfs f13,4(r3)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// fmuls f13,f13,f13
	ctx.f13.f64 = double(float(ctx.f13.f64 * ctx.f13.f64));
	// lfs f0,0(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f12,8(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 8);
	ctx.f12.f64 = double(temp.f32);
	// cmplwi cr6,r28,0
	ctx.cr6.compare<uint32_t>(ctx.r28.u32, 0, ctx.xer);
	// fmadds f0,f0,f0,f13
	ctx.f0.f64 = double(float(std::fma(ctx.f0.f64, ctx.f0.f64, ctx.f13.f64)));
	// fmadds f0,f12,f12,f0
	ctx.f0.f64 = double(float(std::fma(ctx.f12.f64, ctx.f12.f64, ctx.f0.f64)));
	// fsqrts f31,f0
	ctx.f31.f64 = double(float(sqrt(ctx.f0.f64)));
	// beq cr6,0x8254314c
	if (ctx.cr6.eq) goto loc_8254314C;
	// fmr f26,f20
	ctx.f26.f64 = ctx.f20.f64;
	// b 0x82543150
	goto loc_82543150;
loc_8254314C:
	// fmr f26,f31
	ctx.fpscr.disableFlushMode();
	ctx.f26.f64 = ctx.f31.f64;
loc_82543150:
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// lwz r3,280(r31)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r31.u32 + 280);
	// li r7,0
	ctx.r7.s64 = 0;
	// addi r6,r11,5832
	ctx.r6.s64 = ctx.r11.s64 + 5832;
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r5,r11,21004
	ctx.r5.s64 = ctx.r11.s64 + 21004;
	// bl 0x82a009c0
	ctx.lr = 0x82543170;
	sub_82A009C0(ctx, base);
	// mr r19,r3
	ctx.r19.u64 = ctx.r3.u64;
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8253c7b0
	ctx.lr = 0x82543180;
	sub_8253C7B0(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// mulli r10,r16,60
	ctx.r10.s64 = static_cast<int64_t>(ctx.r16.u64 * static_cast<uint64_t>(60));
	// stfs f20,112(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f20.f64);
	REX_STORE_U32(ctx.r1.u32 + 112, temp.u32);
	// stfs f20,116(r1)
	temp.f32 = float(ctx.f20.f64);
	REX_STORE_U32(ctx.r1.u32 + 116, temp.u32);
	// stfs f20,192(r1)
	temp.f32 = float(ctx.f20.f64);
	REX_STORE_U32(ctx.r1.u32 + 192, temp.u32);
	// addi r11,r11,19344
	ctx.r11.s64 = ctx.r11.s64 + 19344;
	// lis r9,-31992
	ctx.r9.s64 = -2096627712;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// add r29,r10,r11
	ctx.r29.u64 = ctx.r10.u64 + ctx.r11.u64;
	// addi r7,r1,124
	ctx.r7.s64 = ctx.r1.s64 + 124;
	// addi r6,r1,120
	ctx.r6.s64 = ctx.r1.s64 + 120;
	// mr r5,r25
	ctx.r5.u64 = ctx.r25.u64;
	// stfs f26,17944(r9)
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r9.u32 + 17944, temp.u32);
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lfs f27,12(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 12);
	ctx.f27.f64 = double(temp.f32);
	// lfs f28,16(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 16);
	ctx.f28.f64 = double(temp.f32);
	// lfs f19,52(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 52);
	ctx.f19.f64 = double(temp.f32);
	// lfs f22,56(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 56);
	ctx.f22.f64 = double(temp.f32);
	// lfs f30,0(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 0);
	ctx.f30.f64 = double(temp.f32);
	// lfs f29,4(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 4);
	ctx.f29.f64 = double(temp.f32);
	// lfs f25,40(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 40);
	ctx.f25.f64 = double(temp.f32);
	// lfs f24,44(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 44);
	ctx.f24.f64 = double(temp.f32);
	// bl 0x8253f010
	ctx.lr = 0x825431E0;
	sub_8253F010(ctx, base);
	// addi r9,r1,152
	ctx.r9.s64 = ctx.r1.s64 + 152;
	// fmr f1,f31
	ctx.fpscr.disableFlushMode();
	ctx.f1.f64 = ctx.f31.f64;
	// addi r8,r1,144
	ctx.r8.s64 = ctx.r1.s64 + 144;
	// mr r7,r28
	ctx.r7.u64 = ctx.r28.u64;
	// mr r5,r26
	ctx.r5.u64 = ctx.r26.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8253cc78
	ctx.lr = 0x82543200;
	sub_8253CC78(ctx, base);
	// addi r7,r1,160
	ctx.r7.s64 = ctx.r1.s64 + 160;
	// li r6,1
	ctx.r6.s64 = 1;
	// addi r5,r1,176
	ctx.r5.s64 = ctx.r1.s64 + 176;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82541af8
	ctx.lr = 0x82543218;
	sub_82541AF8(ctx, base);
	// addi r7,r1,176
	ctx.r7.s64 = ctx.r1.s64 + 176;
	// addi r6,r1,160
	ctx.r6.s64 = ctx.r1.s64 + 160;
	// mr r5,r25
	ctx.r5.u64 = ctx.r25.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8253c868
	ctx.lr = 0x82543230;
	sub_8253C868(ctx, base);
	// addi r5,r1,160
	ctx.r5.s64 = ctx.r1.s64 + 160;
	// mr r4,r25
	ctx.r4.u64 = ctx.r25.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8253cba8
	ctx.lr = 0x82543240;
	sub_8253CBA8(ctx, base);
	// lwz r11,304(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 304);
	// mr r15,r3
	ctx.r15.u64 = ctx.r3.u64;
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// rlwinm r5,r11,27,31,31
	ctx.r5.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// bl 0x8253c098
	ctx.lr = 0x8254325C;
	sub_8253C098(ctx, base);
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lfs f12,168(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 168);
	ctx.f12.f64 = double(temp.f32);
	// fadds f12,f1,f12
	ctx.f12.f64 = double(float(ctx.f1.f64 + ctx.f12.f64));
	// stfs f12,168(r1)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r1.u32 + 168, temp.u32);
	// lfs f12,184(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 184);
	ctx.f12.f64 = double(temp.f32);
	// fadds f12,f1,f12
	ctx.f12.f64 = double(float(ctx.f1.f64 + ctx.f12.f64));
	// stfs f12,184(r1)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r1.u32 + 184, temp.u32);
	// lfs f13,-18864(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -18864);
	ctx.f13.f64 = double(temp.f32);
	// lfs f0,3624(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 3624);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f13,f0
	ctx.cr6.compare(ctx.f13.f64, ctx.f0.f64);
	// beq cr6,0x82543294
	if (ctx.cr6.eq) goto loc_82543294;
	// stfs f0,-18864(r11)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r11.u32 + -18864, temp.u32);
	// fmr f19,f13
	ctx.f19.f64 = ctx.f13.f64;
loc_82543294:
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// lfs f13,-18860(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -18860);
	ctx.f13.f64 = double(temp.f32);
	// fcmpu cr6,f13,f0
	ctx.cr6.compare(ctx.f13.f64, ctx.f0.f64);
	// beq cr6,0x825432ac
	if (ctx.cr6.eq) goto loc_825432AC;
	// stfs f0,-18860(r11)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r11.u32 + -18860, temp.u32);
	// fmr f22,f13
	ctx.f22.f64 = ctx.f13.f64;
loc_825432AC:
	// mr r5,r30
	ctx.r5.u64 = ctx.r30.u64;
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8253c120
	ctx.lr = 0x825432BC;
	sub_8253C120(ctx, base);
	// mr r4,r16
	ctx.r4.u64 = ctx.r16.u64;
	// lfs f1,8(r29)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 8);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x8253d0b0
	ctx.lr = 0x825432C8;
	sub_8253D0B0(ctx, base);
	// lfs f0,812(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 812);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,800(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 800);
	ctx.f13.f64 = double(temp.f32);
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// fmadds f23,f0,f13,f29
	ctx.f23.f64 = double(float(std::fma(ctx.f0.f64, ctx.f13.f64, ctx.f29.f64)));
	// stfs f23,148(r1)
	temp.f32 = float(ctx.f23.f64);
	REX_STORE_U32(ctx.r1.u32 + 148, temp.u32);
	// fmr f18,f1
	ctx.f18.f64 = ctx.f1.f64;
	// bl 0x8253cd98
	ctx.lr = 0x825432E4;
	sub_8253CD98(ctx, base);
	// lfs f0,720(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 720);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f23,f0
	ctx.cr6.compare(ctx.f23.f64, ctx.f0.f64);
	// ble cr6,0x825432f4
	if (!ctx.cr6.gt) goto loc_825432F4;
	// fmr f27,f23
	ctx.f27.f64 = ctx.f23.f64;
loc_825432F4:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stfs f23,720(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f23.f64);
	REX_STORE_U32(ctx.r31.u32 + 720, temp.u32);
	// fmr f1,f21
	ctx.f1.f64 = ctx.f21.f64;
	// bl 0x8253ce30
	ctx.lr = 0x82543304;
	sub_8253CE30(ctx, base);
	// bl 0x8253c540
	ctx.lr = 0x82543308;
	sub_8253C540(ctx, base);
	// lbz r11,904(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 904);
	// clrlwi r11,r11,31
	ctx.r11.u64 = ctx.r11.u32 & 0x1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82543330
	if (ctx.cr6.eq) goto loc_82543330;
	// mr r6,r30
	ctx.r6.u64 = ctx.r30.u64;
	// addi r5,r1,160
	ctx.r5.s64 = ctx.r1.s64 + 160;
	// mr r4,r16
	ctx.r4.u64 = ctx.r16.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8253ea00
	ctx.lr = 0x8254332C;
	sub_8253EA00(ctx, base);
	// b 0x82543c04
	goto loc_82543C04;
loc_82543330:
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lwz r11,32(r30)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r30.u32 + 32);
	// li r9,0
	ctx.r9.s64 = 0;
	// stw r24,92(r1)
	REX_STORE_U32(ctx.r1.u32 + 92, ctx.r24.u32);
	// li r8,0
	ctx.r8.s64 = 0;
	// li r7,0
	ctx.r7.s64 = 0;
	// addi r6,r1,192
	ctx.r6.s64 = ctx.r1.s64 + 192;
	// lfs f5,19980(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 19980);
	ctx.f5.f64 = double(temp.f32);
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// lfs f3,56(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 56);
	ctx.f3.f64 = double(temp.f32);
	// lfs f2,52(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 52);
	ctx.f2.f64 = double(temp.f32);
	// lfs f1,48(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 48);
	ctx.f1.f64 = double(temp.f32);
	// lfs f4,26808(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 26808);
	ctx.f4.f64 = double(temp.f32);
	// bl 0x82292798
	ctx.lr = 0x82543368;
	sub_82292798(ctx, base);
	// lfs f0,168(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 168);
	ctx.f0.f64 = double(temp.f32);
	// mr r20,r3
	ctx.r20.u64 = ctx.r3.u64;
	// fadds f0,f0,f30
	ctx.f0.f64 = double(float(ctx.f0.f64 + ctx.f30.f64));
	// stfs f0,168(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 168, temp.u32);
	// lfs f0,184(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 184);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f23,f28
	ctx.cr6.compare(ctx.f23.f64, ctx.f28.f64);
	// fadds f0,f0,f30
	ctx.f0.f64 = double(float(ctx.f0.f64 + ctx.f30.f64));
	// stfs f0,184(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 184, temp.u32);
	// ble cr6,0x82543394
	if (!ctx.cr6.gt) goto loc_82543394;
	// fmr f1,f23
	ctx.f1.f64 = ctx.f23.f64;
	// b 0x82543398
	goto loc_82543398;
loc_82543394:
	// fmr f1,f28
	ctx.fpscr.disableFlushMode();
	ctx.f1.f64 = ctx.f28.f64;
loc_82543398:
	// lbz r11,904(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 904);
	// stfs f1,152(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	REX_STORE_U32(ctx.r1.u32 + 152, temp.u32);
	// lwz r22,124(r1)
	ctx.r22.u64 = REX_LOAD_U32(ctx.r1.u32 + 124);
	// rlwinm r11,r11,0,27,27
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x10;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825433cc
	if (ctx.cr6.eq) goto loc_825433CC;
	// addi r8,r1,160
	ctx.r8.s64 = ctx.r1.s64 + 160;
	// fmr f2,f23
	ctx.f2.f64 = ctx.f23.f64;
	// addi r5,r1,128
	ctx.r5.s64 = ctx.r1.s64 + 128;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8253f4a0
	ctx.lr = 0x825433C8;
	sub_8253F4A0(ctx, base);
	// b 0x825433e4
	goto loc_825433E4;
loc_825433CC:
	// cmplwi cr6,r22,0
	ctx.cr6.compare<uint32_t>(ctx.r22.u32, 0, ctx.xer);
	// beq cr6,0x825433e4
	if (ctx.cr6.eq) goto loc_825433E4;
	// mr r4,r22
	ctx.r4.u64 = ctx.r22.u64;
	// lwz r5,120(r1)
	ctx.r5.u64 = REX_LOAD_U32(ctx.r1.u32 + 120);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8253f648
	ctx.lr = 0x825433E4;
	sub_8253F648(ctx, base);
loc_825433E4:
	// addi r5,r1,160
	ctx.r5.s64 = ctx.r1.s64 + 160;
	// addi r4,r1,128
	ctx.r4.s64 = ctx.r1.s64 + 128;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8253cf18
	ctx.lr = 0x825433F4;
	sub_8253CF18(ctx, base);
	// addi r11,r1,160
	ctx.r11.s64 = ctx.r1.s64 + 160;
	// addi r17,r31,560
	ctx.r17.s64 = ctx.r31.s64 + 560;
	// fmr f1,f18
	ctx.fpscr.disableFlushMode();
	ctx.f1.f64 = ctx.f18.f64;
	// addi r4,r1,128
	ctx.r4.s64 = ctx.r1.s64 + 128;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,128
	ctx.r11.s64 = ctx.r1.s64 + 128;
	// lvx128 v13,r0,r17
	ea = (ctx.r17.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// vsubfp v0,v0,v13
	ctx.fpscr.enableFlushModeUnconditional();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_sub_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v13.f32)));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x8253cea0
	ctx.lr = 0x82543420;
	sub_8253CEA0(ctx, base);
	// lwz r11,304(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 304);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82543430
	if (!ctx.cr6.eq) goto loc_82543430;
	// stfs f20,772(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f20.f64);
	REX_STORE_U32(ctx.r31.u32 + 772, temp.u32);
loc_82543430:
	// addi r5,r1,128
	ctx.r5.s64 = ctx.r1.s64 + 128;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8253f0d8
	ctx.lr = 0x82543440;
	sub_8253F0D8(ctx, base);
	// addi r11,r1,128
	ctx.r11.s64 = ctx.r1.s64 + 128;
	// addi r21,r31,576
	ctx.r21.s64 = ctx.r31.s64 + 576;
	// vspltisw128 v127,0
	simde_mm_store_si128((simde__m128i*)ctx.v127.u32, simde_mm_set1_epi32(int(0x0)));
	// lvx128 v13,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,160
	ctx.r11.s64 = ctx.r1.s64 + 160;
	// vmsum3fp128 v0,v13,v13
	ctx.fpscr.enableFlushMode();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_dp_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v13.f32), 0xEF));
	// lvx128 v8,r0,r21
	ea = (ctx.r21.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v8.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lvx128 v9,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v9.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// vsubfp v9,v9,v8
	simde_mm_store_ps(ctx.v9.f32, simde_mm_sub_ps(simde_mm_load_ps(ctx.v9.f32), simde_mm_load_ps(ctx.v8.f32)));
	// vor128 v8,v127,v127
	simde_mm_store_si128((simde__m128i*)ctx.v8.u8, simde_mm_load_si128((simde__m128i*)ctx.v127.u8));
	// addi r28,r11,-18560
	ctx.r28.s64 = ctx.r11.s64 + -18560;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r24,r11,29104
	ctx.r24.s64 = ctx.r11.s64 + 29104;
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// lvx128 v12,r0,r28
	ea = (ctx.r28.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r23,r11,29088
	ctx.r23.s64 = ctx.r11.s64 + 29088;
	// addi r11,r1,208
	ctx.r11.s64 = ctx.r1.s64 + 208;
	// lvx128 v11,r0,r24
	ea = (ctx.r24.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v11.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lvx128 v10,r0,r23
	ea = (ctx.r23.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v10.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx128 v9,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v9.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// vor v9,v0,v0
	simde_mm_store_si128((simde__m128i*)ctx.v9.u8, simde_mm_load_si128((simde__m128i*)ctx.v0.u8));
	// vrsqrtefp v0,v0
	simde_mm_store_ps(ctx.v0.f32, simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(simde_mm_load_ps(ctx.v0.f32))));
	// lfs f13,212(r1)
	ctx.fpscr.disableFlushModeUnconditional();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 212);
	ctx.f13.f64 = double(temp.f32);
	// fmuls f12,f13,f13
	ctx.f12.f64 = double(float(ctx.f13.f64 * ctx.f13.f64));
	// lfs f0,216(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 216);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,208(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 208);
	ctx.f13.f64 = double(temp.f32);
	// addi r11,r1,128
	ctx.r11.s64 = ctx.r1.s64 + 128;
	// fmadds f0,f0,f0,f12
	ctx.f0.f64 = double(float(std::fma(ctx.f0.f64, ctx.f0.f64, ctx.f12.f64)));
	// vor v7,v0,v0
	simde_mm_store_si128((simde__m128i*)ctx.v7.u8, simde_mm_load_si128((simde__m128i*)ctx.v0.u8));
	// vcmpeqfp v0,v0,v12
	ctx.fpscr.enableFlushModeUnconditional();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_cmpeq_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v12.f32)));
	// fmadds f0,f13,f13,f0
	ctx.fpscr.disableFlushModeUnconditional();
	ctx.f0.f64 = double(float(std::fma(ctx.f13.f64, ctx.f13.f64, ctx.f0.f64)));
	// vsel v0,v7,v8,v0
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)ctx.v7.u8)), simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)ctx.v8.u8))));
	// vmulfp128 v12,v0,v0
	ctx.fpscr.enableFlushModeUnconditional();
	simde_mm_store_ps(ctx.v12.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v0.f32)));
	// vmulfp128 v11,v11,v0
	simde_mm_store_ps(ctx.v11.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v11.f32), simde_mm_load_ps(ctx.v0.f32)));
	// fsqrts f0,f0
	ctx.fpscr.disableFlushModeUnconditional();
	ctx.f0.f64 = double(float(sqrt(ctx.f0.f64)));
	// fcmpu cr6,f0,f23
	ctx.cr6.compare(ctx.f0.f64, ctx.f23.f64);
	// vnmsubfp v12,v9,v12,v10
	ctx.fpscr.enableFlushModeUnconditional();
	simde_mm_store_ps(ctx.v12.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(ctx.v9.f32), simde_mm_load_ps(ctx.v12.f32)), simde_mm_load_ps(ctx.v10.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
	// vmaddfp v0,v12,v11,v0
	simde_mm_store_ps(ctx.v0.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(ctx.v12.f32), simde_mm_load_ps(ctx.v11.f32)), simde_mm_load_ps(ctx.v0.f32)));
	// vmulfp128 v0,v13,v0
	simde_mm_store_ps(ctx.v0.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v0.f32)));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bge cr6,0x82543500
	if (!ctx.cr6.lt) goto loc_82543500;
	// lfs f13,12(r29)
	ctx.fpscr.disableFlushModeUnconditional();
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 12);
	ctx.f13.f64 = double(temp.f32);
	// fcmpu cr6,f23,f13
	ctx.cr6.compare(ctx.f23.f64, ctx.f13.f64);
	// ble cr6,0x82543500
	if (!ctx.cr6.gt) goto loc_82543500;
	// fsubs f13,f27,f0
	ctx.f13.f64 = double(float(ctx.f27.f64 - ctx.f0.f64));
	// fsel f23,f13,f27,f0
	ctx.f23.f64 = ctx.f13.f64 >= 0.0 ? ctx.f27.f64 : ctx.f0.f64;
	// stfs f23,148(r1)
	temp.f32 = float(ctx.f23.f64);
	REX_STORE_U32(ctx.r1.u32 + 148, temp.u32);
loc_82543500:
	// addi r3,r1,128
	ctx.r3.s64 = ctx.r1.s64 + 128;
	// bl 0x8236aa50
	ctx.lr = 0x82543508;
	sub_8236AA50(ctx, base);
	// lfs f31,144(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 144);
	ctx.f31.f64 = double(temp.f32);
	// fsubs f0,f31,f1
	ctx.f0.f64 = double(float(ctx.f31.f64 - ctx.f1.f64));
	// lis r11,-32244
	ctx.r11.s64 = -2113142784;
	// stfs f1,124(r1)
	temp.f32 = float(ctx.f1.f64);
	REX_STORE_U32(ctx.r1.u32 + 124, temp.u32);
	// lfs f13,-4388(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -4388);
	ctx.f13.f64 = double(temp.f32);
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// ble cr6,0x82543534
	if (!ctx.cr6.gt) goto loc_82543534;
	// lis r11,-32244
	ctx.r11.s64 = -2113142784;
	// lfs f0,-4384(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -4384);
	ctx.f0.f64 = double(temp.f32);
	// fsubs f31,f31,f0
	ctx.f31.f64 = double(float(ctx.f31.f64 - ctx.f0.f64));
	// b 0x82543550
	goto loc_82543550;
loc_82543534:
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f13,21896(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 21896);
	ctx.f13.f64 = double(temp.f32);
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// bge cr6,0x82543550
	if (!ctx.cr6.lt) goto loc_82543550;
	// lis r11,-32244
	ctx.r11.s64 = -2113142784;
	// lfs f0,-4384(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -4384);
	ctx.f0.f64 = double(temp.f32);
	// fadds f31,f31,f0
	ctx.f31.f64 = double(float(ctx.f31.f64 + ctx.f0.f64));
loc_82543550:
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8253cfc8
	ctx.lr = 0x8254355C;
	sub_8253CFC8(ctx, base);
	// lbz r11,904(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 904);
	// rlwinm r11,r11,0,0,24
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82543578
	if (ctx.cr6.eq) goto loc_82543578;
	// clrlwi r11,r26,24
	ctx.r11.u64 = ctx.r26.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8254357c
	if (!ctx.cr6.eq) goto loc_8254357C;
loc_82543578:
	// fmr f26,f20
	ctx.fpscr.disableFlushMode();
	ctx.f26.f64 = ctx.f20.f64;
loc_8254357C:
	// mr r10,r22
	ctx.r10.u64 = ctx.r22.u64;
	// fmr f6,f21
	ctx.fpscr.disableFlushMode();
	ctx.f6.f64 = ctx.f21.f64;
	// mr r9,r30
	ctx.r9.u64 = ctx.r30.u64;
	// stb r18,95(r1)
	REX_STORE_U8(ctx.r1.u32 + 95, ctx.r18.u8);
	// addi r4,r1,124
	ctx.r4.s64 = ctx.r1.s64 + 124;
	// fmr f5,f26
	ctx.f5.f64 = ctx.f26.f64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// fmr f4,f24
	ctx.f4.f64 = ctx.f24.f64;
	// fmr f3,f25
	ctx.f3.f64 = ctx.f25.f64;
	// fmr f2,f20
	ctx.f2.f64 = ctx.f20.f64;
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// bl 0x8253f228
	ctx.lr = 0x825435AC;
	sub_8253F228(ctx, base);
	// lfs f0,776(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 776);
	ctx.f0.f64 = double(temp.f32);
	// cmplwi cr6,r27,0
	ctx.cr6.compare<uint32_t>(ctx.r27.u32, 0, ctx.xer);
	// lfs f30,124(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 124);
	ctx.f30.f64 = double(temp.f32);
	// fsubs f0,f30,f0
	ctx.f0.f64 = double(float(ctx.f30.f64 - ctx.f0.f64));
	// fdivs f29,f0,f21
	ctx.f29.f64 = double(float(ctx.f0.f64 / ctx.f21.f64));
	// beq cr6,0x825435e8
	if (ctx.cr6.eq) goto loc_825435E8;
	// lbz r11,904(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 904);
	// rlwinm r11,r11,0,26,26
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x20;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825435e8
	if (ctx.cr6.eq) goto loc_825435E8;
	// addi r6,r1,112
	ctx.r6.s64 = ctx.r1.s64 + 112;
	// addi r5,r1,116
	ctx.r5.s64 = ctx.r1.s64 + 116;
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8253c3a8
	ctx.lr = 0x825435E8;
	sub_8253C3A8(ctx, base);
native_control = ctx.r27.u32;
loc_825435E8:
	// lbz r11,906(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 906);
	// rlwinm r11,r11,0,26,26
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x20;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82543618
	if (ctx.cr6.eq) goto loc_82543618;
	// li r6,1
	ctx.r6.s64 = 1;
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,3
	ctx.r4.s64 = 3;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x823d3c00
	ctx.lr = 0x8254360C;
	sub_823D3C00(ctx, base);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x82543618
	if (ctx.cr6.eq) goto loc_82543618;
	// stfs f20,112(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f20.f64);
	REX_STORE_U32(ctx.r1.u32 + 112, temp.u32);
native_yaw_allowed = false;
loc_82543618:
	// addi r7,r1,112
	ctx.r7.s64 = ctx.r1.s64 + 112;
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r25
	ctx.r5.u64 = ctx.r25.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8253e6a8
	ctx.lr = 0x82543630;
	sub_8253E6A8(ctx, base);
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lfs f2,112(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 112);
	ctx.f2.f64 = double(temp.f32);
	// lfs f1,116(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 116);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x825407c0
	ctx.lr = 0x82543644;
	sub_825407C0(ctx, base);
	// lbz r11,906(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 906);
	// lfs f0,112(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 112);
	ctx.f0.f64 = double(temp.f32);
	// lbz r9,904(r31)
	ctx.r9.u64 = REX_LOAD_U8(ctx.r31.u32 + 904);
	// fabs f12,f0
	ctx.f12.u64 = ctx.f0.u64 & ~0x8000000000000000;
	// lfs f13,116(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 116);
	ctx.f13.f64 = double(temp.f32);
	// fabs f11,f13
	ctx.f11.u64 = ctx.f13.u64 & ~0x8000000000000000;
	// fmuls f12,f12,f0
	ctx.f12.f64 = double(float(ctx.f12.f64 * ctx.f0.f64));
	// fmuls f13,f11,f13
	ctx.f13.f64 = double(float(ctx.f11.f64 * ctx.f13.f64));
	// andi. r10,r11,239
	ctx.r10.u64 = ctx.r11.u64 & 239;
	ctx.cr0.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// rlwinm r11,r9,0,27,27
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 0) & 0x10;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stb r10,906(r31)
	REX_STORE_U8(ctx.r31.u32 + 906, ctx.r10.u8);
	// lfs f0,20240(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 20240);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// fmuls f0,f12,f0
	ctx.f0.f64 = double(float(ctx.f12.f64 * ctx.f0.f64));
	// stfs f0,112(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 112, temp.u32);
	// lfs f0,20236(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 20236);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
	// stfs f0,116(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 116, temp.u32);
	// beq cr6,0x825436e8
	if (ctx.cr6.eq) goto loc_825436E8;
	// stfs f20,136(r1)
	temp.f32 = float(ctx.f20.f64);
	REX_STORE_U32(ctx.r1.u32 + 136, temp.u32);
	// addi r11,r1,128
	ctx.r11.s64 = ctx.r1.s64 + 128;
	// lvx128 v12,r0,r28
	ea = (ctx.r28.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// vor128 v9,v127,v127
	simde_mm_store_si128((simde__m128i*)ctx.v9.u8, simde_mm_load_si128((simde__m128i*)ctx.v127.u8));
	// lvx128 v11,r0,r24
	ea = (ctx.r24.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v11.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lvx128 v10,r0,r23
	ea = (ctx.r23.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v10.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lvx128 v13,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,128
	ctx.r11.s64 = ctx.r1.s64 + 128;
	// vmsum3fp128 v0,v13,v13
	ctx.fpscr.enableFlushModeUnconditional();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_dp_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v13.f32), 0xEF));
	// vor v8,v0,v0
	simde_mm_store_si128((simde__m128i*)ctx.v8.u8, simde_mm_load_si128((simde__m128i*)ctx.v0.u8));
	// vrsqrtefp v0,v0
	simde_mm_store_ps(ctx.v0.f32, simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(simde_mm_load_ps(ctx.v0.f32))));
	// vor v7,v0,v0
	simde_mm_store_si128((simde__m128i*)ctx.v7.u8, simde_mm_load_si128((simde__m128i*)ctx.v0.u8));
	// vcmpeqfp v0,v0,v12
	simde_mm_store_ps(ctx.v0.f32, simde_mm_cmpeq_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v12.f32)));
	// vsel v0,v7,v9,v0
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)ctx.v7.u8)), simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)ctx.v9.u8))));
	// vmulfp128 v12,v0,v0
	simde_mm_store_ps(ctx.v12.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v0.f32)));
	// vmulfp128 v11,v11,v0
	simde_mm_store_ps(ctx.v11.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v11.f32), simde_mm_load_ps(ctx.v0.f32)));
	// vnmsubfp v12,v8,v12,v10
	simde_mm_store_ps(ctx.v12.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(ctx.v8.f32), simde_mm_load_ps(ctx.v12.f32)), simde_mm_load_ps(ctx.v10.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
	// vmaddfp v0,v12,v11,v0
	simde_mm_store_ps(ctx.v0.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(ctx.v12.f32), simde_mm_load_ps(ctx.v11.f32)), simde_mm_load_ps(ctx.v0.f32)));
	// vmulfp128 v0,v13,v0
	simde_mm_store_ps(ctx.v0.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v0.f32)));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
loc_825436E8:
	// rlwinm r11,r10,0,27,27
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0x10;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82543700
	if (ctx.cr6.eq) goto loc_82543700;
	// lfs f0,772(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 772);
	ctx.f0.f64 = double(temp.f32);
	// fadds f1,f0,f18
	ctx.f1.f64 = double(float(ctx.f0.f64 + ctx.f18.f64));
	// b 0x82543708
	goto loc_82543708;
loc_82543700:
	// lfs f1,136(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 136);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x8236ada8
	ctx.lr = 0x82543708;
	sub_8236ADA8(ctx, base);
loc_82543708:
	// lwz r11,304(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 304);
	// stfs f1,120(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	REX_STORE_U32(ctx.r1.u32 + 120, temp.u32);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82543728
	if (ctx.cr6.eq) goto loc_82543728;
	// lbz r11,904(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 904);
	// rlwinm r11,r11,0,27,27
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x10;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8254372c
	if (ctx.cr6.eq) goto loc_8254372C;
loc_82543728:
	// stfs f1,772(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	REX_STORE_U32(ctx.r31.u32 + 772, temp.u32);
loc_8254372C:
	// addi r4,r1,120
	ctx.r4.s64 = ctx.r1.s64 + 120;
	// fmr f4,f21
	ctx.fpscr.disableFlushMode();
	ctx.f4.f64 = ctx.f21.f64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// fmr f3,f26
	ctx.f3.f64 = ctx.f26.f64;
	// fmr f2,f30
	ctx.f2.f64 = ctx.f30.f64;
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// bl 0x8253f7b0
	ctx.lr = 0x82543748;
	sub_8253F7B0(ctx, base);
	// lfs f0,120(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 120);
	ctx.f0.f64 = double(temp.f32);
	// fneg f24,f22
	ctx.f24.u64 = ctx.f22.u64 ^ 0x8000000000000000;
	// fsubs f27,f0,f18
	ctx.f27.f64 = double(float(ctx.f0.f64 - ctx.f18.f64));
	// fcmpu cr6,f27,f24
	ctx.cr6.compare(ctx.f27.f64, ctx.f24.f64);
	// bge cr6,0x82543764
	if (!ctx.cr6.lt) goto loc_82543764;
	// fmr f27,f24
	ctx.f27.f64 = ctx.f24.f64;
	// b 0x82543770
	goto loc_82543770;
loc_82543764:
	// fcmpu cr6,f27,f19
	ctx.fpscr.disableFlushMode();
	ctx.cr6.compare(ctx.f27.f64, ctx.f19.f64);
	// ble cr6,0x82543770
	if (!ctx.cr6.gt) goto loc_82543770;
	// fmr f27,f19
	ctx.f27.f64 = ctx.f19.f64;
loc_82543770:
	// lwz r11,304(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 304);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8254378c
	if (ctx.cr6.eq) goto loc_8254378C;
	// lbz r11,904(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 904);
	// rlwinm r11,r11,0,27,27
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x10;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82543790
	if (ctx.cr6.eq) goto loc_82543790;
loc_8254378C:
	// stfs f27,772(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f27.f64);
	REX_STORE_U32(ctx.r31.u32 + 772, temp.u32);
loc_82543790:
	// fmr f31,f21
	ctx.fpscr.disableFlushMode();
	ctx.f31.f64 = ctx.f21.f64;
	// lfs f1,20(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 20);
	ctx.f1.f64 = double(temp.f32);
	// fmr f2,f31
	ctx.f2.f64 = ctx.f31.f64;
	// bl 0x82a02648
	ctx.lr = 0x825437A0;
	sub_82A02648(ctx, base);
	// frsp f12,f1
	ctx.fpscr.disableFlushMode();
	ctx.f12.f64 = double(float(ctx.f1.f64));
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f11,772(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 772);
	ctx.f11.f64 = double(temp.f32);
	// lfs f0,24(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 24);
	ctx.f0.f64 = double(temp.f32);
	// fsubs f10,f27,f11
	ctx.f10.f64 = double(float(ctx.f27.f64 - ctx.f11.f64));
	// fmuls f0,f0,f21
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f21.f64));
	// lfs f25,3400(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 3400);
	ctx.f25.f64 = double(temp.f32);
	// fsubs f12,f25,f12
	ctx.f12.f64 = double(float(ctx.f25.f64 - ctx.f12.f64));
	// fneg f13,f0
	ctx.f13.u64 = ctx.f0.u64 ^ 0x8000000000000000;
	// fmuls f26,f10,f12
	ctx.f26.f64 = double(float(ctx.f10.f64 * ctx.f12.f64));
	// fcmpu cr6,f26,f13
	ctx.cr6.compare(ctx.f26.f64, ctx.f13.f64);
	// bge cr6,0x825437d8
	if (!ctx.cr6.lt) goto loc_825437D8;
	// fmr f26,f13
	ctx.f26.f64 = ctx.f13.f64;
	// b 0x825437e4
	goto loc_825437E4;
loc_825437D8:
	// fcmpu cr6,f26,f0
	ctx.fpscr.disableFlushMode();
	ctx.cr6.compare(ctx.f26.f64, ctx.f0.f64);
	// ble cr6,0x825437e4
	if (!ctx.cr6.gt) goto loc_825437E4;
	// fmr f26,f0
	ctx.f26.f64 = ctx.f0.f64;
loc_825437E4:
	// addi r3,r1,208
	ctx.r3.s64 = ctx.r1.s64 + 208;
	// lfs f2,776(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 776);
	ctx.f2.f64 = double(temp.f32);
	// fmr f1,f11
	ctx.f1.f64 = ctx.f11.f64;
	// bl 0x82369750
	ctx.lr = 0x825437F4;
	sub_82369750(ctx, base);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// addi r9,r1,116
	ctx.r9.s64 = ctx.r1.s64 + 116;
	// lfs f2,192(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 192);
	ctx.f2.f64 = double(temp.f32);
	// mr r7,r20
	ctx.r7.u64 = ctx.r20.u64;
	// fmr f1,f23
	ctx.f1.f64 = ctx.f23.f64;
	// addi r5,r1,176
	ctx.r5.s64 = ctx.r1.s64 + 176;
	// addi r4,r1,128
	ctx.r4.s64 = ctx.r1.s64 + 128;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,128
	ctx.r11.s64 = ctx.r1.s64 + 128;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x8253d160
	ctx.lr = 0x82543824;
	sub_8253D160(ctx, base);
	// addi r7,r1,112
	ctx.r7.s64 = ctx.r1.s64 + 112;
	// addi r6,r1,116
	ctx.r6.s64 = ctx.r1.s64 + 116;
	// mr r5,r25
	ctx.r5.u64 = ctx.r25.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// bl 0x8253f890
	ctx.lr = 0x82543838;
	sub_8253F890(ctx, base);
	// addi r7,r1,112
	ctx.r7.s64 = ctx.r1.s64 + 112;
	// addi r6,r1,116
	ctx.r6.s64 = ctx.r1.s64 + 116;
	// mr r5,r25
	ctx.r5.u64 = ctx.r25.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8253d1c8
	ctx.lr = 0x82543850;
	sub_8253D1C8(ctx, base);
	// cmplwi cr6,r19,0
	ctx.cr6.compare<uint32_t>(ctx.r19.u32, 0, ctx.xer);
	// beq cr6,0x82543868
	if (ctx.cr6.eq) goto loc_82543868;
	// addi r5,r1,112
	ctx.r5.s64 = ctx.r1.s64 + 112;
	// addi r4,r1,116
	ctx.r4.s64 = ctx.r1.s64 + 116;
	// mr r3,r19
	ctx.r3.u64 = ctx.r19.u64;
	// bl 0x823971c0
	ctx.lr = 0x82543868;
	sub_823971C0(ctx, base);
loc_82543868:
	// lfs f1,32(r29)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 32);
	ctx.f1.f64 = double(temp.f32);
	// fmr f2,f31
	ctx.f2.f64 = ctx.f31.f64;
	// bl 0x82a02648
	ctx.lr = 0x82543874;
	sub_82A02648(ctx, base);
	// lfs f0,112(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 112);
	ctx.f0.f64 = double(temp.f32);
	// lfs f28,36(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 36);
	ctx.f28.f64 = double(temp.f32);
	// fadds f0,f29,f0
	ctx.f0.f64 = double(float(ctx.f29.f64 + ctx.f0.f64));
	// fneg f29,f28
	ctx.f29.u64 = ctx.f28.u64 ^ 0x8000000000000000;
	// frsp f31,f1
	ctx.f31.f64 = double(float(ctx.f1.f64));
	// fcmpu cr6,f0,f29
	ctx.cr6.compare(ctx.f0.f64, ctx.f29.f64);
	// bge cr6,0x82543898
	if (!ctx.cr6.lt) goto loc_82543898;
	// fmr f0,f29
	ctx.f0.f64 = ctx.f29.f64;
	// b 0x825438a4
	goto loc_825438A4;
loc_82543898:
	// fcmpu cr6,f0,f28
	ctx.fpscr.disableFlushMode();
	ctx.cr6.compare(ctx.f0.f64, ctx.f28.f64);
	// ble cr6,0x825438a4
	if (!ctx.cr6.gt) goto loc_825438A4;
	// fmr f0,f28
	ctx.f0.f64 = ctx.f28.f64;
loc_825438A4:
	// fsubs f30,f25,f31
	ctx.fpscr.disableFlushMode();
	ctx.f30.f64 = double(float(ctx.f25.f64 - ctx.f31.f64));
	// lfs f13,780(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 780);
	ctx.f13.f64 = double(temp.f32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// fmuls f0,f30,f0
	ctx.f0.f64 = double(float(ctx.f30.f64 * ctx.f0.f64));
	// fmadds f0,f13,f31,f0
	ctx.f0.f64 = double(float(std::fma(ctx.f13.f64, ctx.f31.f64, ctx.f0.f64)));
	// stfs f0,780(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 780, temp.u32);
	// bl 0x8253c2b0
	ctx.lr = 0x825438C0;
	sub_8253C2B0(ctx, base);
	// lfs f0,780(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 780);
	ctx.f0.f64 = double(temp.f32);
	// fabs f13,f0
	ctx.f13.u64 = ctx.f0.u64 & ~0x8000000000000000;
	// lfs f0,-17764(r14)
	temp.u32 = REX_LOAD_U32(ctx.r14.u32 + -17764);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f13,f0
	ctx.cr6.compare(ctx.f13.f64, ctx.f0.f64);
	// bge cr6,0x825438d8
	if (!ctx.cr6.lt) goto loc_825438D8;
	// stfs f20,780(r31)
	temp.f32 = float(ctx.f20.f64);
	REX_STORE_U32(ctx.r31.u32 + 780, temp.u32);
loc_825438D8:
	// lfs f0,776(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 776);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,780(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 780);
	ctx.f13.f64 = double(temp.f32);
	// fmadds f1,f13,f21,f0
	ctx.f1.f64 = double(float(std::fma(ctx.f13.f64, ctx.f21.f64, ctx.f0.f64)));
native_mouse = gta4::input::TakeMouseRotation(base, native_control, ctx.r31.u32, gta4::input::LoadFloat(base, ctx.r31.u32 + 96), true);
if (!native_yaw_allowed) native_mouse.yaw = 0;
ctx.f1.f64 += native_mouse.yaw;
	// stfs f1,776(r31)
	temp.f32 = float(ctx.f1.f64);
	REX_STORE_U32(ctx.r31.u32 + 776, temp.u32);
	// bl 0x82369828
	ctx.lr = 0x825438EC;
	sub_82369828(ctx, base);
	// lfs f13,116(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 116);
	ctx.f13.f64 = double(temp.f32);
	// fmuls f13,f31,f13
	ctx.f13.f64 = double(float(ctx.f31.f64 * ctx.f13.f64));
	// lfs f0,784(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 784);
	ctx.f0.f64 = double(temp.f32);
	// stfs f1,776(r31)
	temp.f32 = float(ctx.f1.f64);
	REX_STORE_U32(ctx.r31.u32 + 776, temp.u32);
	// fmadds f0,f30,f0,f13
	ctx.f0.f64 = double(float(std::fma(ctx.f30.f64, ctx.f0.f64, ctx.f13.f64)));
	// stfs f0,784(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 784, temp.u32);
	// fcmpu cr6,f0,f29
	ctx.cr6.compare(ctx.f0.f64, ctx.f29.f64);
	// bge cr6,0x82543914
	if (!ctx.cr6.lt) goto loc_82543914;
	// fmr f0,f29
	ctx.f0.f64 = ctx.f29.f64;
	// b 0x82543920
	goto loc_82543920;
loc_82543914:
	// fcmpu cr6,f0,f28
	ctx.fpscr.disableFlushMode();
	ctx.cr6.compare(ctx.f0.f64, ctx.f28.f64);
	// ble cr6,0x82543920
	if (!ctx.cr6.gt) goto loc_82543920;
	// fmr f0,f28
	ctx.f0.f64 = ctx.f28.f64;
loc_82543920:
	// fabs f13,f0
	ctx.fpscr.disableFlushMode();
	ctx.f13.u64 = ctx.f0.u64 & ~0x8000000000000000;
	// stfs f0,784(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 784, temp.u32);
	// lfs f0,-17764(r14)
	temp.u32 = REX_LOAD_U32(ctx.r14.u32 + -17764);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f13,f0
	ctx.cr6.compare(ctx.f13.f64, ctx.f0.f64);
	// bge cr6,0x82543938
	if (!ctx.cr6.lt) goto loc_82543938;
	// stfs f20,784(r31)
	temp.f32 = float(ctx.f20.f64);
	REX_STORE_U32(ctx.r31.u32 + 784, temp.u32);
loc_82543938:
	// lfs f0,772(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 772);
	ctx.f0.f64 = double(temp.f32);
	// fadds f0,f0,f26
	ctx.f0.f64 = double(float(ctx.f0.f64 + ctx.f26.f64));
native_mouse.pitch = gta4::input::ConstrainMousePitch(ctx.f0.f64, native_mouse.pitch, ctx.f24.f64, ctx.f19.f64);
ctx.f0.f64 += native_mouse.pitch;
	// lfs f13,784(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 784);
	ctx.f13.f64 = double(temp.f32);
	// stfs f0,772(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 772, temp.u32);
	// fmadds f31,f21,f13,f27
	ctx.f31.f64 = double(float(std::fma(ctx.f21.f64, ctx.f13.f64, ctx.f27.f64)));
ctx.f31.f64 += native_mouse.pitch;
	// fcmpu cr6,f0,f19
	ctx.cr6.compare(ctx.f0.f64, ctx.f19.f64);
	// ble cr6,0x8254395c
	if (!ctx.cr6.gt) goto loc_8254395C;
	// stfs f19,772(r31)
	temp.f32 = float(ctx.f19.f64);
	REX_STORE_U32(ctx.r31.u32 + 772, temp.u32);
	// b 0x82543968
	goto loc_82543968;
loc_8254395C:
	// fcmpu cr6,f0,f24
	ctx.fpscr.disableFlushMode();
	ctx.cr6.compare(ctx.f0.f64, ctx.f24.f64);
	// bge cr6,0x8254396c
	if (!ctx.cr6.lt) goto loc_8254396C;
	// stfs f24,772(r31)
	temp.f32 = float(ctx.f24.f64);
	REX_STORE_U32(ctx.r31.u32 + 772, temp.u32);
loc_82543968:
	// stfs f20,784(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f20.f64);
	REX_STORE_U32(ctx.r31.u32 + 784, temp.u32);
loc_8254396C:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8253e650
	ctx.lr = 0x82543974;
gta4::input::AcceptMouseAngles(base, ctx.r31.u32, native_mouse, 772, 776, 788, 792);
	sub_8253E650(ctx, base);
	// addi r3,r1,208
	ctx.r3.s64 = ctx.r1.s64 + 208;
	// lfs f2,776(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 776);
	ctx.f2.f64 = double(temp.f32);
	// lfs f1,772(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 772);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x82369750
	ctx.lr = 0x82543984;
	sub_82369750(ctx, base);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// addi r4,r1,128
	ctx.r4.s64 = ctx.r1.s64 + 128;
	// addi r3,r1,256
	ctx.r3.s64 = ctx.r1.s64 + 256;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,128
	ctx.r11.s64 = ctx.r1.s64 + 128;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x8236aa80
	ctx.lr = 0x825439A0;
	sub_8236AA80(ctx, base);
	// addi r11,r1,148
	ctx.r11.s64 = ctx.r1.s64 + 148;
	// addi r10,r1,128
	ctx.r10.s64 = ctx.r1.s64 + 128;
	// li r9,80
	ctx.r9.s64 = 80;
	// li r4,4
	ctx.r4.s64 = 4;
	// addi r3,r1,224
	ctx.r3.s64 = ctx.r1.s64 + 224;
	// lvlx v12,0,r11
	temp.u32 = ctx.r11.u32;
	simde_mm_store_si128((simde__m128i*)ctx.v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
	// addi r11,r1,224
	ctx.r11.s64 = ctx.r1.s64 + 224;
	// lvx128 v13,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,176
	ctx.r10.s64 = ctx.r1.s64 + 176;
	// vspltw v12,v12,0
	simde_mm_store_si128((simde__m128i*)ctx.v12.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)ctx.v12.u32), 0xFF));
	// vmulfp128 v13,v13,v12
	ctx.fpscr.enableFlushMode();
	simde_mm_store_ps(ctx.v13.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v12.f32)));
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lbz r10,905(r31)
	ctx.r10.u64 = REX_LOAD_U8(ctx.r31.u32 + 905);
	// clrlwi r10,r10,26
	ctx.r10.u64 = ctx.r10.u32 & 0x3F;
	// stb r10,905(r31)
	REX_STORE_U8(ctx.r31.u32 + 905, ctx.r10.u8);
	// stvx128 v0,r31,r9
	ea = (ctx.r31.u32 + ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// vsubfp v0,v0,v13
	simde_mm_store_ps(ctx.v0.f32, simde_mm_sub_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v13.f32)));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x823696e8
	ctx.lr = 0x825439EC;
	sub_823696E8(ctx, base);
	// addi r11,r1,224
	ctx.r11.s64 = ctx.r1.s64 + 224;
	// fadds f1,f31,f18
	ctx.fpscr.disableFlushMode();
	ctx.f1.f64 = double(float(ctx.f31.f64 + ctx.f18.f64));
	// addi r29,r31,64
	ctx.r29.s64 = ctx.r31.s64 + 64;
	// addi r3,r1,240
	ctx.r3.s64 = ctx.r1.s64 + 240;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx128 v0,r0,r29
	ea = (ctx.r29.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lfs f2,776(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 776);
	ctx.f2.f64 = double(temp.f32);
	// bl 0x82369750
	ctx.lr = 0x82543A0C;
	sub_82369750(ctx, base);
	// addi r10,r1,240
	ctx.r10.s64 = ctx.r1.s64 + 240;
	// addi r11,r1,152
	ctx.r11.s64 = ctx.r1.s64 + 152;
	// addi r9,r1,160
	ctx.r9.s64 = ctx.r1.s64 + 160;
	// li r4,4
	ctx.r4.s64 = 4;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,148
	ctx.r10.s64 = ctx.r1.s64 + 148;
	// lvlx v12,0,r11
	temp.u32 = ctx.r11.u32;
	simde_mm_store_si128((simde__m128i*)ctx.v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
	// vspltw v12,v12,0
	simde_mm_store_si128((simde__m128i*)ctx.v12.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)ctx.v12.u32), 0xFF));
	// lvx128 v13,r0,r9
	ea = (ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lvlx v11,0,r10
	temp.u32 = ctx.r10.u32;
	simde_mm_store_si128((simde__m128i*)ctx.v11.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
	// vspltw v11,v11,0
	simde_mm_store_si128((simde__m128i*)ctx.v11.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)ctx.v11.u32), 0xFF));
	// vmulfp128 v12,v0,v12
	ctx.fpscr.enableFlushMode();
	simde_mm_store_ps(ctx.v12.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v12.f32)));
	// vmulfp128 v0,v0,v11
	simde_mm_store_ps(ctx.v0.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v11.f32)));
	// vsubfp v12,v13,v12
	simde_mm_store_ps(ctx.v12.f32, simde_mm_sub_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v12.f32)));
	// vsubfp v0,v13,v0
	simde_mm_store_ps(ctx.v0.f32, simde_mm_sub_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v0.f32)));
	// stvx128 v12,r0,r17
	ea = (ctx.r17.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v12.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx128 v0,r0,r21
	ea = (ctx.r21.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x823696e8
	ctx.lr = 0x82543A58;
	sub_823696E8(ctx, base);
	// cmplwi cr6,r19,0
	ctx.cr6.compare<uint32_t>(ctx.r19.u32, 0, ctx.xer);
	// beq cr6,0x82543a98
	if (ctx.cr6.eq) goto loc_82543A98;
	// addi r10,r1,176
	ctx.r10.s64 = ctx.r1.s64 + 176;
	// lwz r11,300(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 300);
	// lvx128 v13,r0,r29
	ea = (ctx.r29.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r9,368
	ctx.r9.s64 = 368;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lwz r11,5224(r11)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r11.u32 + 5224);
	// addi r10,r1,144
	ctx.r10.s64 = ctx.r1.s64 + 144;
	// vsubfp v13,v13,v0
	ctx.fpscr.enableFlushMode();
	simde_mm_store_ps(ctx.v13.f32, simde_mm_sub_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v0.f32)));
	// lfs f0,12(r11)
	ctx.fpscr.disableFlushModeUnconditional();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,144(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 144, temp.u32);
	// lvlx v12,0,r10
	temp.u32 = ctx.r10.u32;
	simde_mm_store_si128((simde__m128i*)ctx.v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
	// vspltw v12,v12,0
	simde_mm_store_si128((simde__m128i*)ctx.v12.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)ctx.v12.u32), 0xFF));
	// vmaddfp v0,v13,v12,v0
	ctx.fpscr.enableFlushModeUnconditional();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v12.f32)), simde_mm_load_ps(ctx.v0.f32)));
	// stvx128 v0,r19,r9
	ea = (ctx.r19.u32 + ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
loc_82543A98:
	// lwz r11,300(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 300);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x82543b88
	if (ctx.cr6.eq) goto loc_82543B88;
	// lbz r11,316(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 316);
	// rlwinm r11,r11,0,28,28
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x8;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82543b88
	if (ctx.cr6.eq) goto loc_82543B88;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// lbz r11,-13252(r11)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r11.u32 + -13252);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82543b88
	if (ctx.cr6.eq) goto loc_82543B88;
	// addi r5,r1,176
	ctx.r5.s64 = ctx.r1.s64 + 176;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8253e7f0
	ctx.lr = 0x82543AD4;
	sub_8253E7F0(ctx, base);
	// mr r4,r16
	ctx.r4.u64 = ctx.r16.u64;
	// lwz r5,860(r31)
	ctx.r5.u64 = REX_LOAD_U32(ctx.r31.u32 + 860);
	// lwz r3,300(r31)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r31.u32 + 300);
	// bl 0x825ef200
	ctx.lr = 0x82543AE4;
	sub_825EF200(ctx, base);
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// lwz r3,300(r31)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r31.u32 + 300);
	// bl 0x822bca90
	ctx.lr = 0x82543AF0;
	sub_822BCA90(ctx, base);
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// lwz r3,300(r31)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r31.u32 + 300);
	// bl 0x825ef300
	ctx.lr = 0x82543AFC;
	sub_825EF300(ctx, base);
	// lbz r11,904(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 904);
	// rlwinm r11,r11,0,0,24
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82543b28
	if (ctx.cr6.eq) goto loc_82543B28;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f13,716(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 716);
	ctx.f13.f64 = double(temp.f32);
	// lfs f0,20068(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 20068);
	ctx.f0.f64 = double(temp.f32);
	// fadds f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 + ctx.f0.f64));
	// fsubs f13,f0,f25
	ctx.f13.f64 = double(float(ctx.f0.f64 - ctx.f25.f64));
	// fsel f0,f13,f25,f0
	ctx.f0.f64 = ctx.f13.f64 >= 0.0 ? ctx.f25.f64 : ctx.f0.f64;
	// stfs f0,716(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 716, temp.u32);
loc_82543B28:
	// li r11,0
	ctx.r11.s64 = 0;
	// lfs f1,716(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 716);
	ctx.f1.f64 = double(temp.f32);
	// mr r9,r15
	ctx.r9.u64 = ctx.r15.u64;
	// ld r8,480(r31)
	ctx.r8.u64 = REX_LOAD_U64(ctx.r31.u32 + 480);
	// mr r7,r30
	ctx.r7.u64 = ctx.r30.u64;
	// lwz r3,300(r31)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r31.u32 + 300);
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// addi r5,r1,176
	ctx.r5.s64 = ctx.r1.s64 + 176;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// stb r11,87(r1)
	REX_STORE_U8(ctx.r1.u32 + 87, ctx.r11.u8);
	// bl 0x825f0250
	ctx.lr = 0x82543B54;
	sub_825F0250(ctx, base);
	// addi r4,r1,128
	ctx.r4.s64 = ctx.r1.s64 + 128;
	// addi r3,r31,16
	ctx.r3.s64 = ctx.r31.s64 + 16;
	// bl 0x82371000
	ctx.lr = 0x82543B60;
	sub_82371000(ctx, base);
	// mr r8,r31
	ctx.r8.u64 = ctx.r31.u64;
	// addi r7,r1,176
	ctx.r7.s64 = ctx.r1.s64 + 176;
	// lwz r3,300(r31)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r31.u32 + 300);
	// mr r6,r29
	ctx.r6.u64 = ctx.r29.u64;
	// mr r5,r30
	ctx.r5.u64 = ctx.r30.u64;
	// li r4,0
	ctx.r4.s64 = 0;
	// bl 0x822bca90
	ctx.lr = 0x82543B7C;
	sub_822BCA90(ctx, base);
	// li r4,4
	ctx.r4.s64 = 4;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x823696e8
	ctx.lr = 0x82543B88;
	sub_823696E8(ctx, base);
loc_82543B88:
	// addi r4,r1,128
	ctx.r4.s64 = ctx.r1.s64 + 128;
	// addi r3,r31,16
	ctx.r3.s64 = ctx.r31.s64 + 16;
	// bl 0x82371000
	ctx.lr = 0x82543B94;
	sub_82371000(ctx, base);
	// lbz r10,904(r31)
	ctx.r10.u64 = REX_LOAD_U8(ctx.r31.u32 + 904);
	// lbz r11,905(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 905);
	// lbz r9,907(r31)
	ctx.r9.u64 = REX_LOAD_U8(ctx.r31.u32 + 907);
	// clrlwi r11,r11,26
	ctx.r11.u64 = ctx.r11.u32 & 0x3F;
	// stb r9,908(r31)
	REX_STORE_U8(ctx.r31.u32 + 908, ctx.r9.u8);
	// stb r11,905(r31)
	REX_STORE_U8(ctx.r31.u32 + 905, ctx.r11.u8);
	// li r11,1
	ctx.r11.s64 = 1;
	// andi. r10,r10,239
	ctx.r10.u64 = ctx.r10.u64 & 239;
	ctx.cr0.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// stb r10,904(r31)
	REX_STORE_U8(ctx.r31.u32 + 904, ctx.r10.u8);
	// lfs f0,484(r30)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + 484);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f0,f20
	ctx.cr6.compare(ctx.f0.f64, ctx.f20.f64);
	// ble cr6,0x82543bc8
	if (!ctx.cr6.gt) goto loc_82543BC8;
	// li r11,0
	ctx.r11.s64 = 0;
loc_82543BC8:
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82543be0
	if (!ctx.cr6.eq) goto loc_82543BE0;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8253edf8
	ctx.lr = 0x82543BDC;
	sub_8253EDF8(ctx, base);
	// b 0x82543c00
	goto loc_82543C00;
loc_82543BE0:
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f0,96(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 96);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,-27016(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -27016);
	ctx.f13.f64 = double(temp.f32);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// lfs f12,27448(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 27448);
	ctx.f12.f64 = double(temp.f32);
	// fsubs f12,f12,f0
	ctx.f12.f64 = double(float(ctx.f12.f64 - ctx.f0.f64));
	// fmadds f0,f12,f13,f0
	ctx.f0.f64 = double(float(std::fma(ctx.f12.f64, ctx.f13.f64, ctx.f0.f64)));
	// stfs f0,96(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 96, temp.u32);
loc_82543C00:
	// li r3,1
	ctx.r3.s64 = 1;
loc_82543C04:
	// addi r1,r1,560
	ctx.r1.s64 = ctx.r1.s64 + 560;
	// li r0,-288
	ctx.r0.s64 = -288;
	// lvx128 v127,r1,r0
	ea = (ctx.r1.u32 + ctx.r0.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v127.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r12,r1,-152
	ctx.r12.s64 = ctx.r1.s64 + -152;
	// bl 0x82a0134c
	ctx.lr = 0x82543C18;
	__restfpr_18(ctx, base);
	// b 0x829ff7e0
	__restgprlr_14(ctx, base);
	return;
}

extern "C" void sub_8253EA00(PPCContext& ctx, uint8_t* base) {
  gta4::input::MouseRotation native_mouse{};
  uint32_t native_control = 0;
	REX_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c4
	ctx.lr = 0x8253EA08;
	__savegprlr_27(ctx, base);
	// stfd f30,-64(r1)
	ctx.fpscr.disableFlushMode();
	REX_STORE_U64(ctx.r1.u32 + -64, ctx.f30.u64);
	// stfd f31,-56(r1)
	REX_STORE_U64(ctx.r1.u32 + -56, ctx.f31.u64);
	// li r12,-80
	ctx.r12.s64 = -80;
	// stvx128 v127,r1,r12
	ea = (ctx.r1.u32 + ctx.r12.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v127.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stwu r1,-224(r1)
	ea = -224 + ctx.r1.u32;
	REX_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// mr r27,r4
	ctx.r27.u64 = ctx.r4.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// lwz r11,0(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 0);
	// lwz r11,12(r11)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r11.u32 + 12);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x8253EA3C;
	REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// lwz r3,540(r3)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r3.u32 + 540);
	// bl 0x823c0f40
	ctx.lr = 0x8253EA44;
	sub_823C0F40(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8253ea7c
	if (!ctx.cr6.eq) goto loc_8253EA7C;
	// lwz r11,0(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 0);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r11,12(r11)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r11.u32 + 12);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x8253EA64;
	REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// lwz r3,540(r3)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r3.u32 + 540);
	// bl 0x823c1178
	ctx.lr = 0x8253EA6C;
	sub_823C1178(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// li r11,0
	ctx.r11.s64 = 0;
	// beq cr6,0x8253ea80
	if (ctx.cr6.eq) goto loc_8253EA80;
loc_8253EA7C:
	// li r11,1
	ctx.r11.s64 = 1;
loc_8253EA80:
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8253ea9c
	if (ctx.cr6.eq) goto loc_8253EA9C;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f0,19984(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 19984);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,772(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 772, temp.u32);
	// b 0x8253eb10
	goto loc_8253EB10;
loc_8253EA9C:
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// bl 0x82369a08
	ctx.lr = 0x8253EAA8;
	sub_82369A08(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 0, ctx.xer);
	// beq cr6,0x8253eb10
	if (ctx.cr6.eq) goto loc_8253EB10;
	// lbz r11,904(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 904);
	// rlwinm r11,r11,0,26,26
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x20;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8253eb10
	if (ctx.cr6.eq) goto loc_8253EB10;
	// addi r6,r1,100
	ctx.r6.s64 = ctx.r1.s64 + 100;
	// addi r5,r1,96
	ctx.r5.s64 = ctx.r1.s64 + 96;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8253c3a8
	ctx.lr = 0x8253EAD4;
native_control = ctx.r4.u32;
	sub_8253C3A8(ctx, base);
native_mouse = gta4::input::TakeMouseRotation(base, native_control, ctx.r31.u32, gta4::input::LoadFloat(base, ctx.r31.u32 + 96), true);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f13,96(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 96);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,772(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 772);
	ctx.f12.f64 = double(temp.f32);
	// lfs f0,20072(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 20072);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// fmadds f0,f13,f0,f12
	ctx.f0.f64 = double(float(std::fma(ctx.f13.f64, ctx.f0.f64, ctx.f12.f64)));
ctx.f0.f64 += native_mouse.pitch;
	// stfs f0,772(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 772, temp.u32);
	// lfs f13,20076(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 20076);
	ctx.f13.f64 = double(temp.f32);
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// blt cr6,0x8253eb0c
	if (ctx.cr6.lt) goto loc_8253EB0C;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f13,20080(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 20080);
	ctx.f13.f64 = double(temp.f32);
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// ble cr6,0x8253eb10
	if (!ctx.cr6.gt) goto loc_8253EB10;
loc_8253EB0C:
	// stfs f13,772(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r31.u32 + 772, temp.u32);
loc_8253EB10:
	// addi r3,r1,112
	ctx.r3.s64 = ctx.r1.s64 + 112;
	// lfs f2,776(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 776);
	ctx.f2.f64 = double(temp.f32);
	// lfs f1,772(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 772);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x82369750
	ctx.lr = 0x8253EB20;
	sub_82369750(ctx, base);
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// vspltisw v0,0
	simde_mm_store_si128((simde__m128i*)ctx.v0.u32, simde_mm_set1_epi32(int(0x0)));
	// lvx128 v13,r0,r3
	ea = (ctx.r3.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r11,-18560
	ctx.r11.s64 = ctx.r11.s64 + -18560;
	// lfs f13,800(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 800);
	ctx.f13.f64 = double(temp.f32);
	// vor v9,v0,v0
	simde_mm_store_si128((simde__m128i*)ctx.v9.u8, simde_mm_load_si128((simde__m128i*)ctx.v0.u8));
	// vsubfp v0,v0,v13
	ctx.fpscr.enableFlushModeUnconditional();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_sub_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v13.f32)));
	// lvx128 v12,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r11,r11,29104
	ctx.r11.s64 = ctx.r11.s64 + 29104;
	// lvx128 v11,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v11.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f0,20092(r11)
	ctx.fpscr.disableFlushModeUnconditional();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 20092);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// fadds f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 + ctx.f0.f64));
	// addi r11,r11,29088
	ctx.r11.s64 = ctx.r11.s64 + 29088;
	// lvx128 v10,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v10.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f13,19824(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 19824);
	ctx.f13.f64 = double(temp.f32);
	// addi r11,r1,112
	ctx.r11.s64 = ctx.r1.s64 + 112;
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f11,120(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 120);
	ctx.f11.f64 = double(temp.f32);
	// lfs f12,14280(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 14280);
	ctx.f12.f64 = double(temp.f32);
	// fadds f12,f11,f12
	ctx.f12.f64 = double(float(ctx.f11.f64 + ctx.f12.f64));
	// stfs f12,120(r1)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r1.u32 + 120, temp.u32);
	// addi r11,r1,112
	ctx.r11.s64 = ctx.r1.s64 + 112;
	// lvx128 v13,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// vmsum3fp128 v0,v13,v13
	ctx.fpscr.enableFlushModeUnconditional();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_dp_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v13.f32), 0xEF));
	// vor v8,v0,v0
	simde_mm_store_si128((simde__m128i*)ctx.v8.u8, simde_mm_load_si128((simde__m128i*)ctx.v0.u8));
	// vrsqrtefp v0,v0
	simde_mm_store_ps(ctx.v0.f32, simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(simde_mm_load_ps(ctx.v0.f32))));
	// vor v7,v0,v0
	simde_mm_store_si128((simde__m128i*)ctx.v7.u8, simde_mm_load_si128((simde__m128i*)ctx.v0.u8));
	// vcmpeqfp v0,v0,v12
	simde_mm_store_ps(ctx.v0.f32, simde_mm_cmpeq_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v12.f32)));
	// vsel v0,v7,v9,v0
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)ctx.v7.u8)), simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)ctx.v9.u8))));
	// vmulfp128 v12,v0,v0
	simde_mm_store_ps(ctx.v12.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v0.f32)));
	// vmulfp128 v11,v11,v0
	simde_mm_store_ps(ctx.v11.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v11.f32), simde_mm_load_ps(ctx.v0.f32)));
	// vnmsubfp v12,v8,v12,v10
	simde_mm_store_ps(ctx.v12.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(ctx.v8.f32), simde_mm_load_ps(ctx.v12.f32)), simde_mm_load_ps(ctx.v10.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
	// vmaddfp v0,v12,v11,v0
	simde_mm_store_ps(ctx.v0.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(ctx.v12.f32), simde_mm_load_ps(ctx.v11.f32)), simde_mm_load_ps(ctx.v0.f32)));
	// vmulfp128 v0,v13,v0
	simde_mm_store_ps(ctx.v0.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v0.f32)));
	// bge cr6,0x8253ebc8
	if (!ctx.cr6.lt) goto loc_8253EBC8;
	// fmr f0,f13
	ctx.fpscr.disableFlushModeUnconditional();
	ctx.f0.f64 = ctx.f13.f64;
loc_8253EBC8:
	// addi r8,r1,96
	ctx.r8.s64 = ctx.r1.s64 + 96;
	// stfs f0,96(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 96, temp.u32);
	// lvx128 v127,r0,r29
	ea = (ctx.r29.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v127.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lwz r11,860(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 860);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// lwz r3,540(r28)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r28.u32 + 540);
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// lis r9,-32254
	ctx.r9.s64 = -2113798144;
	// lvlx v13,0,r8
	temp.u32 = ctx.r8.u32;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
	// addi r8,r1,112
	ctx.r8.s64 = ctx.r1.s64 + 112;
	// vspltw v13,v13,0
	simde_mm_store_si128((simde__m128i*)ctx.v13.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)ctx.v13.u32), 0xFF));
	// rlwinm r30,r11,2,0,29
	ctx.r30.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r10,r10,19632
	ctx.r10.s64 = ctx.r10.s64 + 19632;
	// addi r9,r9,19648
	ctx.r9.s64 = ctx.r9.s64 + 19648;
	// vmaddfp128 v127,v0,v13,v127
	ctx.fpscr.enableFlushModeUnconditional();
	simde_mm_store_ps(ctx.v127.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v13.f32)), simde_mm_load_ps(ctx.v127.f32)));
	// lfsx f31,r30,r10
	ctx.fpscr.disableFlushModeUnconditional();
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + ctx.r10.u32);
	ctx.f31.f64 = double(temp.f32);
	// lfsx f30,r30,r9
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + ctx.r9.u32);
	ctx.f30.f64 = double(temp.f32);
	// stvx128 v127,r0,r8
	ea = (ctx.r8.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v127.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x823c23c0
	ctx.lr = 0x8253EC14;
	sub_823C23C0(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8253ec68
	if (ctx.cr6.eq) goto loc_8253EC68;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lvx128 v0,r0,r29
	ea = (ctx.r29.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r9,r1,96
	ctx.r9.s64 = ctx.r1.s64 + 96;
	// vsubfp128 v12,v0,v127
	ctx.fpscr.enableFlushMode();
	simde_mm_store_ps(ctx.v12.f32, simde_mm_sub_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v127.f32)));
	// addi r11,r11,19664
	ctx.r11.s64 = ctx.r11.s64 + 19664;
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// addi r10,r10,19680
	ctx.r10.s64 = ctx.r10.s64 + 19680;
	// lfsx f0,r30,r11
	ctx.fpscr.disableFlushModeUnconditional();
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + ctx.r11.u32);
	ctx.f0.f64 = double(temp.f32);
	// addi r11,r1,112
	ctx.r11.s64 = ctx.r1.s64 + 112;
	// stfs f0,96(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 96, temp.u32);
	// lvlx v13,0,r9
	temp.u32 = ctx.r9.u32;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
	// vspltw v13,v13,0
	simde_mm_store_si128((simde__m128i*)ctx.v13.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)ctx.v13.u32), 0xFF));
	// lfsx f0,r30,r10
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + ctx.r10.u32);
	ctx.f0.f64 = double(temp.f32);
	// vmaddfp v0,v12,v13,v0
	ctx.fpscr.enableFlushModeUnconditional();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(ctx.v12.f32), simde_mm_load_ps(ctx.v13.f32)), simde_mm_load_ps(ctx.v0.f32)));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lfs f13,120(r1)
	ctx.fpscr.disableFlushModeUnconditional();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 120);
	ctx.f13.f64 = double(temp.f32);
	// fadds f0,f0,f13
	ctx.f0.f64 = double(float(ctx.f0.f64 + ctx.f13.f64));
	// b 0x8253ec6c
	goto loc_8253EC6C;
loc_8253EC68:
	// lfs f0,120(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 120);
	ctx.f0.f64 = double(temp.f32);
loc_8253EC6C:
	// fadds f0,f31,f0
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(float(ctx.f31.f64 + ctx.f0.f64));
	// stfs f0,120(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 120, temp.u32);
	// addi r11,r1,112
	ctx.r11.s64 = ctx.r1.s64 + 112;
	// lfs f0,8(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// addi r30,r31,64
	ctx.r30.s64 = ctx.r31.s64 + 64;
	// fadds f0,f0,f30
	ctx.f0.f64 = double(float(ctx.f0.f64 + ctx.f30.f64));
	// li r4,4
	ctx.r4.s64 = 4;
	// stfs f0,8(r29)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r29.u32 + 8, temp.u32);
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// lvx128 v127,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v127.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx128 v127,r0,r30
	ea = (ctx.r30.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v127.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x823696e8
	ctx.lr = 0x8253EC9C;
	sub_823696E8(ctx, base);
	// lwz r11,300(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 300);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x8253ed74
	if (ctx.cr6.eq) goto loc_8253ED74;
	// lbz r11,316(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 316);
	// rlwinm r11,r11,0,28,28
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x8;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8253ed74
	if (ctx.cr6.eq) goto loc_8253ED74;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// lbz r11,-13252(r11)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r11.u32 + -13252);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8253ed74
	if (ctx.cr6.eq) goto loc_8253ED74;
	// mr r5,r29
	ctx.r5.u64 = ctx.r29.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8253e7f0
	ctx.lr = 0x8253ECD8;
	sub_8253E7F0(ctx, base);
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// lwz r5,860(r31)
	ctx.r5.u64 = REX_LOAD_U32(ctx.r31.u32 + 860);
	// lwz r3,300(r31)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r31.u32 + 300);
	// bl 0x825ef200
	ctx.lr = 0x8253ECE8;
	sub_825EF200(ctx, base);
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// lwz r3,300(r31)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r31.u32 + 300);
	// bl 0x822bca90
	ctx.lr = 0x8253ECF4;
	sub_822BCA90(ctx, base);
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// lwz r3,300(r31)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r31.u32 + 300);
	// bl 0x825ef300
	ctx.lr = 0x8253ED00;
	sub_825EF300(ctx, base);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r9,1
	ctx.r9.s64 = 1;
	// ld r8,480(r31)
	ctx.r8.u64 = REX_LOAD_U64(ctx.r31.u32 + 480);
	// li r7,0
	ctx.r7.s64 = 0;
	// lwz r3,300(r31)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r31.u32 + 300);
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r29
	ctx.r5.u64 = ctx.r29.u64;
	// lfs f1,3400(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 3400);
	ctx.f1.f64 = double(temp.f32);
	// li r11,0
	ctx.r11.s64 = 0;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// stb r11,87(r1)
	REX_STORE_U8(ctx.r1.u32 + 87, ctx.r11.u8);
	// bl 0x825f0250
	ctx.lr = 0x8253ED30;
	sub_825F0250(ctx, base);
	// lvx128 v0,r0,r29
	ea = (ctx.r29.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,128
	ctx.r11.s64 = ctx.r1.s64 + 128;
	// vsubfp128 v0,v0,v127
	ctx.fpscr.enableFlushMode();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_sub_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v127.f32)));
	// addi r4,r1,128
	ctx.r4.s64 = ctx.r1.s64 + 128;
	// addi r3,r31,16
	ctx.r3.s64 = ctx.r31.s64 + 16;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x82371000
	ctx.lr = 0x8253ED4C;
	sub_82371000(ctx, base);
	// mr r8,r31
	ctx.r8.u64 = ctx.r31.u64;
	// lwz r3,300(r31)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r31.u32 + 300);
	// mr r7,r29
	ctx.r7.u64 = ctx.r29.u64;
	// mr r6,r30
	ctx.r6.u64 = ctx.r30.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// li r4,0
	ctx.r4.s64 = 0;
	// bl 0x822bca90
	ctx.lr = 0x8253ED68;
	sub_822BCA90(ctx, base);
	// li r4,4
	ctx.r4.s64 = 4;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x823696e8
	ctx.lr = 0x8253ED74;
	sub_823696E8(ctx, base);
loc_8253ED74:
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lvx128 v0,r0,r29
	ea = (ctx.r29.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lvx128 v13,r0,r30
	ea = (ctx.r30.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// vsubfp v0,v13,v0
	ctx.fpscr.enableFlushMode();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_sub_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v0.f32)));
	// lfs f5,2612(r11)
	ctx.fpscr.disableFlushModeUnconditional();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 2612);
	ctx.f5.f64 = double(temp.f32);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// addi r11,r11,20104
	ctx.r11.s64 = ctx.r11.s64 + 20104;
	// lfs f4,8(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f4.f64 = double(temp.f32);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f3,19960(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 19960);
	ctx.f3.f64 = double(temp.f32);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f2,-27020(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -27020);
	ctx.f2.f64 = double(temp.f32);
	// addi r11,r1,112
	ctx.r11.s64 = ctx.r1.s64 + 112;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lfs f13,116(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 116);
	ctx.f13.f64 = double(temp.f32);
	// fmuls f13,f13,f13
	ctx.f13.f64 = double(float(ctx.f13.f64 * ctx.f13.f64));
	// lfs f0,112(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 112);
	ctx.f0.f64 = double(temp.f32);
	// fmadds f0,f0,f0,f13
	ctx.f0.f64 = double(float(std::fma(ctx.f0.f64, ctx.f0.f64, ctx.f13.f64)));
	// fsqrts f1,f0
	ctx.f1.f64 = double(float(sqrt(ctx.f0.f64)));
	// bl 0x8236a6f0
	ctx.lr = 0x8253EDC4;
	sub_8236A6F0(ctx, base);
	// lfs f0,72(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 72);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// fadds f0,f0,f1
	ctx.f0.f64 = double(float(ctx.f0.f64 + ctx.f1.f64));
	// stfs f0,72(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 72, temp.u32);
	// bl 0x8253c2b0
	ctx.lr = 0x8253EDD8;
	sub_8253C2B0(ctx, base);
	// li r3,1
	ctx.r3.s64 = 1;
	// addi r1,r1,224
	ctx.r1.s64 = ctx.r1.s64 + 224;
	// li r0,-80
	ctx.r0.s64 = -80;
	// lvx128 v127,r1,r0
	ea = (ctx.r1.u32 + ctx.r0.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v127.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lfd f30,-64(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = REX_LOAD_U64(ctx.r1.u32 + -64);
	// lfd f31,-56(r1)
	ctx.f31.u64 = REX_LOAD_U64(ctx.r1.u32 + -56);
	// b 0x829ff814
	__restgprlr_27(ctx, base);
	return;
}

extern "C" void sub_8253AB20(PPCContext& ctx, uint8_t* base) {
  gta4::input::MouseRotation native_mouse{};
  uint32_t native_control = 0;
  double native_pitch_lower = 0, native_pitch_upper = 0;
	REX_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff790
	ctx.lr = 0x8253AB28;
	__savegprlr_14(ctx, base);
	// addi r12,r1,-152
	ctx.r12.s64 = ctx.r1.s64 + -152;
	// bl 0x82a01300
	ctx.lr = 0x8253AB30;
	__savefpr_18(ctx, base);
	// li r12,-288
	ctx.r12.s64 = -288;
	// stvx128 v127,r1,r12
	ea = (ctx.r1.u32 + ctx.r12.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v127.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stwu r1,-576(r1)
	ea = -576 + ctx.r1.u32;
	REX_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lbz r9,317(r31)
	ctx.r9.u64 = REX_LOAD_U8(ctx.r31.u32 + 317);
	// lfs f19,2612(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 2612);
	ctx.f19.f64 = double(temp.f32);
	// lwz r11,300(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 300);
	// ori r10,r9,64
	ctx.r10.u64 = ctx.r9.u64 | 64;
	// stb r10,317(r31)
	REX_STORE_U8(ctx.r31.u32 + 317, ctx.r10.u8);
	// lbz r10,5248(r11)
	ctx.r10.u64 = REX_LOAD_U8(ctx.r11.u32 + 5248);
	// andi. r10,r10,253
	ctx.r10.u64 = ctx.r10.u64 & 253;
	ctx.cr0.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// stb r10,5248(r11)
	REX_STORE_U8(ctx.r11.u32 + 5248, ctx.r10.u8);
	// li r11,0
	ctx.r11.s64 = 0;
	// lis r10,-31996
	ctx.r10.s64 = -2096889856;
	// stb r11,826(r31)
	REX_STORE_U8(ctx.r31.u32 + 826, ctx.r11.u8);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,21980(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 21980);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// stfs f0,21440(r11)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r11.u32 + 21440, temp.u32);
	// li r11,0
	ctx.r11.s64 = 0;
	// lbz r9,396(r31)
	ctx.r9.u64 = REX_LOAD_U8(ctx.r31.u32 + 396);
	// stb r11,-27511(r10)
	REX_STORE_U8(ctx.r10.u32 + -27511, ctx.r11.u8);
	// rlwinm r11,r9,31,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 31) & 0x1;
	// stfs f19,116(r31)
	temp.f32 = float(ctx.f19.f64);
	REX_STORE_U32(ctx.r31.u32 + 116, temp.u32);
	// stb r11,112(r1)
	REX_STORE_U8(ctx.r1.u32 + 112, ctx.r11.u8);
	// bl 0x82535890
	ctx.lr = 0x8253AB9C;
	sub_82535890(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8253abb0
	if (!ctx.cr6.eq) goto loc_8253ABB0;
loc_8253ABA8:
	// li r3,0
	ctx.r3.s64 = 0;
	// b 0x8253bc54
	goto loc_8253BC54;
loc_8253ABB0:
	// lwz r10,368(r31)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r31.u32 + 368);
	// lis r11,-32076
	ctx.r11.s64 = -2102132736;
	// addi r11,r11,-24160
	ctx.r11.s64 = ctx.r11.s64 + -24160;
	// lhz r10,46(r10)
	ctx.r10.u64 = REX_LOAD_U16(ctx.r10.u32 + 46);
	// extsh r10,r10
	ctx.r10.s64 = ctx.r10.s16;
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r10,r10,r11
	ctx.r10.u64 = REX_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lhz r11,964(r10)
	ctx.r11.u64 = REX_LOAD_U16(ctx.r10.u32 + 964);
	// cmplwi cr6,r11,65535
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 65535, ctx.xer);
	// bne cr6,0x8253abf0
	if (!ctx.cr6.eq) goto loc_8253ABF0;
	// lis r11,-31996
	ctx.r11.s64 = -2096889856;
	// lwz r9,-27524(r11)
	ctx.r9.u64 = REX_LOAD_U32(ctx.r11.u32 + -27524);
	// sth r9,964(r10)
	REX_STORE_U16(ctx.r10.u32 + 964, ctx.r9.u16);
	// lwz r10,-27524(r11)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r11.u32 + -27524);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// stw r10,-27524(r11)
	REX_STORE_U32(ctx.r11.u32 + -27524, ctx.r10.u32);
loc_8253ABF0:
	// lwz r30,280(r31)
	ctx.r30.u64 = REX_LOAD_U32(ctx.r31.u32 + 280);
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// lwz r11,0(r30)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r30.u32 + 0);
	// lwz r11,40(r11)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r11.u32 + 40);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x8253AC08;
	REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmpwi cr6,r3,7
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 7, ctx.xer);
	// mr r16,r30
	ctx.r16.u64 = ctx.r30.u64;
	// beq cr6,0x8253ac18
	if (ctx.cr6.eq) goto loc_8253AC18;
	// li r16,0
	ctx.r16.s64 = 0;
loc_8253AC18:
	// lwz r30,368(r31)
	ctx.r30.u64 = REX_LOAD_U32(ctx.r31.u32 + 368);
	// addi r3,r1,208
	ctx.r3.s64 = ctx.r1.s64 + 208;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// lhz r10,46(r30)
	ctx.r10.u64 = REX_LOAD_U16(ctx.r30.u32 + 46);
	// lwz r11,4836(r30)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r30.u32 + 4836);
	// extsh r8,r10
	ctx.r8.s64 = ctx.r10.s16;
	// lwz r9,0(r30)
	ctx.r9.u64 = REX_LOAD_U32(ctx.r30.u32 + 0);
	// lis r10,-32086
	ctx.r10.s64 = -2102788096;
	// addi r29,r10,-23728
	ctx.r29.s64 = ctx.r10.s64 + -23728;
	// lwz r9,232(r9)
	ctx.r9.u64 = REX_LOAD_U32(ctx.r9.u32 + 232);
	// lwz r10,4(r29)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r29.u32 + 4);
	// subf r10,r8,r10
	ctx.r10.u64 = ctx.r10.u64 - ctx.r8.u64;
	// addi r8,r11,-2
	ctx.r8.s64 = ctx.r11.s64 + -2;
	// cntlzw r10,r10
	ctx.r10.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// cntlzw r8,r8
	ctx.r8.u64 = ctx.r8.u32 == 0 ? 32 : __builtin_clz(ctx.r8.u32);
	// rlwinm r28,r10,27,31,31
	ctx.r28.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// addi r10,r11,-4
	ctx.r10.s64 = ctx.r11.s64 + -4;
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// cntlzw r10,r10
	ctx.r10.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// rlwinm r17,r8,27,31,31
	ctx.r17.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 27) & 0x1;
	// rlwinm r27,r10,27,31,31
	ctx.r27.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// rlwinm r26,r11,27,31,31
	ctx.r26.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// mtctr r9
	ctx.ctr.u64 = ctx.r9.u64;
	// bctrl 
	ctx.lr = 0x8253AC78;
	REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// lfs f13,212(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 212);
	ctx.f13.f64 = double(temp.f32);
	// fmuls f12,f13,f13
	ctx.f12.f64 = double(float(ctx.f13.f64 * ctx.f13.f64));
	// lfs f0,208(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 208);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,216(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 216);
	ctx.f13.f64 = double(temp.f32);
	// lwz r11,4836(r30)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r30.u32 + 4836);
	// lbz r10,556(r31)
	ctx.r10.u64 = REX_LOAD_U8(ctx.r31.u32 + 556);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-3
	ctx.r11.s64 = ctx.r11.s64 + -3;
	// li r3,0
	ctx.r3.s64 = 0;
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// stb r10,120(r1)
	REX_STORE_U8(ctx.r1.u32 + 120, ctx.r10.u8);
	// rlwinm r14,r11,27,31,31
	ctx.r14.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// fmadds f0,f0,f0,f12
	ctx.f0.f64 = double(float(std::fma(ctx.f0.f64, ctx.f0.f64, ctx.f12.f64)));
	// fmadds f0,f13,f13,f0
	ctx.f0.f64 = double(float(std::fma(ctx.f13.f64, ctx.f13.f64, ctx.f0.f64)));
	// fsqrts f31,f0
	ctx.f31.f64 = double(float(sqrt(ctx.f0.f64)));
	// bl 0x82369a08
	ctx.lr = 0x8253ACB8;
	sub_82369A08(ctx, base);
	// mr r18,r3
	ctx.r18.u64 = ctx.r3.u64;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82536ba0
	ctx.lr = 0x8253ACC8;
	sub_82536BA0(ctx, base);
	// mr r24,r3
	ctx.r24.u64 = ctx.r3.u64;
	// li r19,1
	ctx.r19.s64 = 1;
	// cmplwi cr6,r16,0
	ctx.cr6.compare<uint32_t>(ctx.r16.u32, 0, ctx.xer);
	// lis r15,-31996
	ctx.r15.s64 = -2096889856;
	// beq cr6,0x8253acf4
	if (ctx.cr6.eq) goto loc_8253ACF4;
	// addi r4,r1,240
	ctx.r4.s64 = ctx.r1.s64 + 240;
	// mr r3,r16
	ctx.r3.u64 = ctx.r16.u64;
	// bl 0x82396eb0
	ctx.lr = 0x8253ACE8;
	sub_82396EB0(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8253ad14
	if (!ctx.cr6.eq) goto loc_8253AD14;
loc_8253ACF4:
	// lbz r11,-27512(r15)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r15.u32 + -27512);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8253ad18
	if (!ctx.cr6.eq) goto loc_8253AD18;
	// clrlwi r10,r28,24
	ctx.r10.u64 = ctx.r28.u32 & 0xFF;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// bne cr6,0x8253ad18
	if (!ctx.cr6.eq) goto loc_8253AD18;
	// mr r10,r19
	ctx.r10.u64 = ctx.r19.u64;
	// b 0x8253ad1c
	goto loc_8253AD1C;
loc_8253AD14:
	// lbz r11,-27512(r15)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r15.u32 + -27512);
loc_8253AD18:
	// li r10,0
	ctx.r10.s64 = 0;
loc_8253AD1C:
	// clrlwi r9,r11,24
	ctx.r9.u64 = ctx.r11.u32 & 0xFF;
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// stfs f31,17944(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	REX_STORE_U32(ctx.r11.u32 + 17944, temp.u32);
	// beq cr6,0x8253af0c
	if (ctx.cr6.eq) goto loc_8253AF0C;
	// stb r19,556(r31)
	REX_STORE_U8(ctx.r31.u32 + 556, ctx.r19.u8);
loc_8253AD34:
	// lbz r11,397(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 397);
	// stw r19,844(r31)
	REX_STORE_U32(ctx.r31.u32 + 844, ctx.r19.u32);
	// rlwinm r10,r11,0,0,24
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x8253ad54
	if (ctx.cr6.eq) goto loc_8253AD54;
	// clrlwi r11,r11,25
	ctx.r11.u64 = ctx.r11.u32 & 0x7F;
	// stfs f19,404(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f19.f64);
	REX_STORE_U32(ctx.r31.u32 + 404, temp.u32);
	// stb r11,397(r31)
	REX_STORE_U8(ctx.r31.u32 + 397, ctx.r11.u8);
loc_8253AD54:
	// lis r11,-32057
	ctx.r11.s64 = -2100887552;
	// lwz r10,32(r30)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r30.u32 + 32);
	// li r9,48
	ctx.r9.s64 = 48;
	// addi r11,r11,-15708
	ctx.r11.s64 = ctx.r11.s64 + -15708;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lvx128 v0,r10,r9
	ea = (ctx.r10.u32 + ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lfs f0,8(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f13,2604(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 2604);
	ctx.f13.f64 = double(temp.f32);
	// addi r11,r1,144
	ctx.r11.s64 = ctx.r1.s64 + 144;
	// fmuls f23,f0,f13
	ctx.f23.f64 = double(float(ctx.f0.f64 * ctx.f13.f64));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x825355e8
	ctx.lr = 0x8253AD8C;
	sub_825355E8(ctx, base);
	// mr r20,r3
	ctx.r20.u64 = ctx.r3.u64;
	// lfs f0,444(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 444);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// mulli r10,r20,60
	ctx.r10.s64 = static_cast<int64_t>(ctx.r20.u64 * static_cast<uint64_t>(60));
	// addi r11,r11,17816
	ctx.r11.s64 = ctx.r11.s64 + 17816;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// add r25,r10,r11
	ctx.r25.u64 = ctx.r10.u64 + ctx.r11.u64;
	// lfs f13,4(r25)
	temp.u32 = REX_LOAD_U32(ctx.r25.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// fadds f0,f0,f13
	ctx.f0.f64 = double(float(ctx.f0.f64 + ctx.f13.f64));
	// stfs f0,116(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 116, temp.u32);
	// bl 0x82535730
	ctx.lr = 0x8253ADB8;
	sub_82535730(ctx, base);
	// addi r6,r1,124
	ctx.r6.s64 = ctx.r1.s64 + 124;
	// addi r5,r1,128
	ctx.r5.s64 = ctx.r1.s64 + 128;
	// lfs f0,0(r25)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r25.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// fmr f30,f1
	ctx.f30.f64 = ctx.f1.f64;
	// stfs f30,132(r1)
	temp.f32 = float(ctx.f30.f64);
	REX_STORE_U32(ctx.r1.u32 + 132, temp.u32);
	// fmr f1,f23
	ctx.f1.f64 = ctx.f23.f64;
	// stfs f0,136(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 136, temp.u32);
	// bl 0x825370c0
	ctx.lr = 0x8253ADDC;
	sub_825370C0(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f31,124(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 124);
	ctx.f31.f64 = double(temp.f32);
	// lfs f0,18732(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 18732);
	ctx.f0.f64 = double(temp.f32);
	// mr r11,r19
	ctx.r11.u64 = ctx.r19.u64;
	// fcmpu cr6,f31,f0
	ctx.cr6.compare(ctx.f31.f64, ctx.f0.f64);
	// bgt cr6,0x8253adf8
	if (ctx.cr6.gt) goto loc_8253ADF8;
	// li r11,0
	ctx.r11.s64 = 0;
loc_8253ADF8:
	// clrlwi r28,r11,24
	ctx.r28.u64 = ctx.r11.u32 & 0xFF;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f0,19076(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 19076);
	ctx.f0.f64 = double(temp.f32);
	// mr r11,r19
	ctx.r11.u64 = ctx.r19.u64;
	// fcmpu cr6,f31,f0
	ctx.cr6.compare(ctx.f31.f64, ctx.f0.f64);
	// bgt cr6,0x8253ae14
	if (ctx.cr6.gt) goto loc_8253AE14;
	// li r11,0
	ctx.r11.s64 = 0;
loc_8253AE14:
	// addi r10,r1,160
	ctx.r10.s64 = ctx.r1.s64 + 160;
	// lfs f1,48(r25)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r25.u32 + 48);
	ctx.f1.f64 = double(temp.f32);
	// addi r9,r1,140
	ctx.r9.s64 = ctx.r1.s64 + 140;
	// mr r7,r20
	ctx.r7.u64 = ctx.r20.u64;
	// mr r6,r16
	ctx.r6.u64 = ctx.r16.u64;
	// mr r5,r30
	ctx.r5.u64 = ctx.r30.u64;
	// mr r4,r18
	ctx.r4.u64 = ctx.r18.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// clrlwi r29,r11,24
	ctx.r29.u64 = ctx.r11.u32 & 0xFF;
	// bl 0x825383b0
	ctx.lr = 0x8253AE3C;
	sub_825383B0(ctx, base);
native_control = ctx.r18.u32;
	// mr r4,r18
	ctx.r4.u64 = ctx.r18.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82539bf0
	ctx.lr = 0x8253AE48;
	sub_82539BF0(ctx, base);
	// addi r5,r1,192
	ctx.r5.s64 = ctx.r1.s64 + 192;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82535d98
	ctx.lr = 0x8253AE58;
	sub_82535D98(ctx, base);
	// addi r11,r1,144
	ctx.r11.s64 = ctx.r1.s64 + 144;
	// clrlwi r22,r27,24
	ctx.r22.u64 = ctx.r27.u32 & 0xFF;
	// cmplwi cr6,r22,0
	ctx.cr6.compare<uint32_t>(ctx.r22.u32, 0, ctx.xer);
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,192
	ctx.r11.s64 = ctx.r1.s64 + 192;
	// lvx128 v13,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,144
	ctx.r11.s64 = ctx.r1.s64 + 144;
	// vaddfp v0,v0,v13
	ctx.fpscr.enableFlushMode();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_add_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v13.f32)));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// beq cr6,0x8253ae98
	if (ctx.cr6.eq) goto loc_8253AE98;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82537d08
	ctx.lr = 0x8253AE8C;
	sub_82537D08(ctx, base);
	// lfs f0,152(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 152);
	ctx.f0.f64 = double(temp.f32);
	// fadds f0,f1,f0
	ctx.f0.f64 = double(float(ctx.f1.f64 + ctx.f0.f64));
	// stfs f0,152(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 152, temp.u32);
loc_8253AE98:
	// clrlwi r11,r26,24
	ctx.r11.u64 = ctx.r26.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8253aed8
	if (ctx.cr6.eq) goto loc_8253AED8;
	// clrlwi r11,r28,24
	ctx.r11.u64 = ctx.r28.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8253aed8
	if (ctx.cr6.eq) goto loc_8253AED8;
	// addi r9,r1,124
	ctx.r9.s64 = ctx.r1.s64 + 124;
	// addi r8,r1,116
	ctx.r8.s64 = ctx.r1.s64 + 116;
	// addi r7,r1,132
	ctx.r7.s64 = ctx.r1.s64 + 132;
	// addi r6,r1,136
	ctx.r6.s64 = ctx.r1.s64 + 136;
	// mr r5,r29
	ctx.r5.u64 = ctx.r29.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82535f18
	ctx.lr = 0x8253AED0;
	sub_82535F18(ctx, base);
	// lfs f30,132(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 132);
	ctx.f30.f64 = double(temp.f32);
	// lfs f31,124(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 124);
	ctx.f31.f64 = double(temp.f32);
loc_8253AED8:
	// lwz r11,304(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 304);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8253b0b0
	if (ctx.cr6.eq) goto loc_8253B0B0;
	// lbz r11,112(r1)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r1.u32 + 112);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8253b0b0
	if (!ctx.cr6.eq) goto loc_8253B0B0;
	// lfs f0,380(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 380);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// fsubs f12,f30,f0
	ctx.f12.f64 = double(float(ctx.f30.f64 - ctx.f0.f64));
	// lfs f13,18868(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 18868);
	ctx.f13.f64 = double(temp.f32);
	// fmadds f0,f12,f13,f0
	ctx.f0.f64 = double(float(std::fma(ctx.f12.f64, ctx.f13.f64, ctx.f0.f64)));
	// stfs f0,380(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 380, temp.u32);
	// b 0x8253b0b4
	goto loc_8253B0B4;
loc_8253AF0C:
	// clrlwi r11,r10,24
	ctx.r11.u64 = ctx.r10.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8253af34
	if (ctx.cr6.eq) goto loc_8253AF34;
	// lbz r11,396(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 396);
	// addi r7,r1,112
	ctx.r7.s64 = ctx.r1.s64 + 112;
	// mr r6,r14
	ctx.r6.u64 = ctx.r14.u64;
	// rlwinm r5,r11,31,31,31
	ctx.r5.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 31) & 0x1;
	// mr r4,r18
	ctx.r4.u64 = ctx.r18.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82537250
	ctx.lr = 0x8253AF34;
	sub_82537250(ctx, base);
loc_8253AF34:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82537b00
	ctx.lr = 0x8253AF3C;
	sub_82537B00(ctx, base);
	// clrlwi r11,r14,24
	ctx.r11.u64 = ctx.r14.u32 & 0xFF;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8253af98
	if (ctx.cr6.eq) goto loc_8253AF98;
	// lhz r11,46(r30)
	ctx.r11.u64 = REX_LOAD_U16(ctx.r30.u32 + 46);
	// lbz r10,320(r31)
	ctx.r10.u64 = REX_LOAD_U8(ctx.r31.u32 + 320);
	// extsh r9,r11
	ctx.r9.s64 = ctx.r11.s16;
	// lwz r11,4(r29)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r29.u32 + 4);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// subf r11,r9,r11
	ctx.r11.u64 = ctx.r11.u64 - ctx.r9.u64;
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r4,r11,27,31,31
	ctx.r4.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// beq cr6,0x8253af90
	if (ctx.cr6.eq) goto loc_8253AF90;
	// lbz r11,396(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 396);
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,0
	ctx.r4.s64 = 0;
	// andi. r11,r11,253
	ctx.r11.u64 = ctx.r11.u64 & 253;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// stb r11,396(r31)
	REX_STORE_U8(ctx.r31.u32 + 396, ctx.r11.u8);
	// bl 0x82539178
	ctx.lr = 0x8253AF88;
	sub_82539178(ctx, base);
	// stb r3,826(r31)
	REX_STORE_U8(ctx.r31.u32 + 826, ctx.r3.u8);
	// b 0x8253bc54
	goto loc_8253BC54;
loc_8253AF90:
	// bl 0x8253a6a0
	ctx.lr = 0x8253AF94;
	sub_8253A6A0(ctx, base);
	// b 0x8253bc50
	goto loc_8253BC50;
loc_8253AF98:
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// bl 0x825363b0
	ctx.lr = 0x8253AFA0;
	sub_825363B0(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8253afe4
	if (ctx.cr6.eq) goto loc_8253AFE4;
	// lbz r11,556(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 556);
	// extsb r10,r11
	ctx.r10.s64 = ctx.r11.s8;
	// cmpwi cr6,r10,4
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 4, ctx.xer);
	// beq cr6,0x8253afd0
	if (ctx.cr6.eq) goto loc_8253AFD0;
	// lbz r9,320(r31)
	ctx.r9.u64 = REX_LOAD_U8(ctx.r31.u32 + 320);
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// bne cr6,0x8253afd0
	if (!ctx.cr6.eq) goto loc_8253AFD0;
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// bne cr6,0x8253afe4
	if (!ctx.cr6.eq) goto loc_8253AFE4;
loc_8253AFD0:
	// lis r10,-32085
	ctx.r10.s64 = -2102722560;
	// li r9,2
	ctx.r9.s64 = 2;
	// stb r11,-20995(r10)
	REX_STORE_U8(ctx.r10.u32 + -20995, ctx.r11.u8);
	// stb r9,556(r31)
	REX_STORE_U8(ctx.r31.u32 + 556, ctx.r9.u8);
	// b 0x8253ad34
	goto loc_8253AD34;
loc_8253AFE4:
	// lbz r11,320(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 320);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8253b014
	if (ctx.cr6.eq) goto loc_8253B014;
	// lbz r11,396(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 396);
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// andi. r11,r11,253
	ctx.r11.u64 = ctx.r11.u64 & 253;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// stb r11,396(r31)
	REX_STORE_U8(ctx.r31.u32 + 396, ctx.r11.u8);
	// bl 0x82539178
	ctx.lr = 0x8253B00C;
	sub_82539178(ctx, base);
	// stb r3,826(r31)
	REX_STORE_U8(ctx.r31.u32 + 826, ctx.r3.u8);
	// b 0x8253bc54
	goto loc_8253BC54;
loc_8253B014:
	// lbz r11,556(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 556);
	// extsb r11,r11
	ctx.r11.s64 = ctx.r11.s8;
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// bne cr6,0x8253b040
	if (!ctx.cr6.eq) goto loc_8253B040;
	// lbz r11,396(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 396);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// andi. r11,r11,253
	ctx.r11.u64 = ctx.r11.u64 & 253;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// stb r11,396(r31)
	REX_STORE_U8(ctx.r31.u32 + 396, ctx.r11.u8);
	// bl 0x8253a738
	ctx.lr = 0x8253B038;
	sub_8253A738(ctx, base);
	// stb r3,826(r31)
	REX_STORE_U8(ctx.r31.u32 + 826, ctx.r3.u8);
	// b 0x8253bc54
	goto loc_8253BC54;
loc_8253B040:
	// cmpwi cr6,r11,4
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 4, ctx.xer);
	// bne cr6,0x8253ad34
	if (!ctx.cr6.eq) goto loc_8253AD34;
	// bl 0x8236bcf0
	ctx.lr = 0x8253B04C;
	sub_8236BCF0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// cmplwi cr6,r4,0
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 0, ctx.xer);
	// beq cr6,0x8253b098
	if (ctx.cr6.eq) goto loc_8253B098;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8237acf0
	ctx.lr = 0x8253B060;
	sub_8237ACF0(ctx, base);
	// cmpwi cr6,r3,3
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 3, ctx.xer);
	// beq cr6,0x8253b070
	if (ctx.cr6.eq) goto loc_8253B070;
	// cmpwi cr6,r3,1
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 1, ctx.xer);
	// bne cr6,0x8253b098
	if (!ctx.cr6.eq) goto loc_8253B098;
loc_8253B070:
	// lwz r11,844(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 844);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// ble cr6,0x8253b098
	if (!ctx.cr6.gt) goto loc_8253B098;
	// lbz r10,396(r31)
	ctx.r10.u64 = REX_LOAD_U8(ctx.r31.u32 + 396);
	// rlwinm r10,r10,0,29,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0x4;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// bne cr6,0x8253b098
	if (!ctx.cr6.eq) goto loc_8253B098;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// stw r11,844(r31)
	REX_STORE_U32(ctx.r31.u32 + 844, ctx.r11.u32);
	// b 0x8253bc50
	goto loc_8253BC50;
loc_8253B098:
	// lbz r11,396(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 396);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// andi. r11,r11,253
	ctx.r11.u64 = ctx.r11.u64 & 253;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// stb r11,396(r31)
	REX_STORE_U8(ctx.r31.u32 + 396, ctx.r11.u8);
	// bl 0x82538948
	ctx.lr = 0x8253B0AC;
	sub_82538948(ctx, base);
	// b 0x8253bc54
	goto loc_8253BC54;
loc_8253B0B0:
	// stfs f30,380(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f30.f64);
	REX_STORE_U32(ctx.r31.u32 + 380, temp.u32);
loc_8253B0B4:
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// lfs f0,380(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 380);
	ctx.f0.f64 = double(temp.f32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stfs f0,132(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 132, temp.u32);
	// fmr f1,f23
	ctx.f1.f64 = ctx.f23.f64;
	// bl 0x82537670
	ctx.lr = 0x8253B0CC;
	sub_82537670(ctx, base);
	// addi r11,r1,144
	ctx.r11.s64 = ctx.r1.s64 + 144;
	// addi r10,r1,132
	ctx.r10.s64 = ctx.r1.s64 + 132;
	// lfs f2,128(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 128);
	ctx.f2.f64 = double(temp.f32);
	// addi r9,r1,116
	ctx.r9.s64 = ctx.r1.s64 + 116;
	// lfs f13,116(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 116);
	ctx.f13.f64 = double(temp.f32);
	// lfs f0,12(r25)
	temp.u32 = REX_LOAD_U32(ctx.r25.u32 + 12);
	ctx.f0.f64 = double(temp.f32);
	// mr r6,r17
	ctx.r6.u64 = ctx.r17.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// fadds f13,f2,f13
	ctx.f13.f64 = double(float(ctx.f2.f64 + ctx.f13.f64));
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// lfs f4,8(r25)
	temp.u32 = REX_LOAD_U32(ctx.r25.u32 + 8);
	ctx.f4.f64 = double(temp.f32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lfs f3,136(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 136);
	ctx.f3.f64 = double(temp.f32);
	// stfs f13,116(r1)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r1.u32 + 116, temp.u32);
	// fmuls f30,f0,f2
	ctx.f30.f64 = double(float(ctx.f0.f64 * ctx.f2.f64));
	// stw r11,100(r1)
	REX_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// stw r10,92(r1)
	REX_STORE_U32(ctx.r1.u32 + 92, ctx.r10.u32);
	// stw r9,84(r1)
	REX_STORE_U32(ctx.r1.u32 + 84, ctx.r9.u32);
	// bl 0x825360e0
	ctx.lr = 0x8253B11C;
	sub_825360E0(ctx, base);
	// lbz r11,556(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 556);
	// lfs f0,16(r25)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r25.u32 + 16);
	ctx.f0.f64 = double(temp.f32);
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// bne cr6,0x8253b148
	if (!ctx.cr6.eq) goto loc_8253B148;
	// cmpwi cr6,r20,0
	ctx.cr6.compare<int32_t>(ctx.r20.s32, 0, ctx.xer);
	// beq cr6,0x8253b13c
	if (ctx.cr6.eq) goto loc_8253B13C;
	// cmpwi cr6,r20,1
	ctx.cr6.compare<int32_t>(ctx.r20.s32, 1, ctx.xer);
	// bne cr6,0x8253b148
	if (!ctx.cr6.eq) goto loc_8253B148;
loc_8253B13C:
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f13,18592(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 18592);
	ctx.f13.f64 = double(temp.f32);
	// fmuls f0,f0,f13
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f13.f64));
loc_8253B148:
	// lfs f22,116(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 116);
	ctx.f22.f64 = double(temp.f32);
	// lwz r11,304(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 304);
	// fsubs f13,f22,f0
	ctx.f13.f64 = double(float(ctx.f22.f64 - ctx.f0.f64));
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// fsel f1,f13,f22,f0
	ctx.f1.f64 = ctx.f13.f64 >= 0.0 ? ctx.f22.f64 : ctx.f0.f64;
	// stfs f1,136(r1)
	temp.f32 = float(ctx.f1.f64);
	REX_STORE_U32(ctx.r1.u32 + 136, temp.u32);
	// bne cr6,0x8253b16c
	if (!ctx.cr6.eq) goto loc_8253B16C;
	// stfs f22,372(r31)
	temp.f32 = float(ctx.f22.f64);
	REX_STORE_U32(ctx.r31.u32 + 372, temp.u32);
	// b 0x8253b184
	goto loc_8253B184;
loc_8253B16C:
	// lfs f0,372(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 372);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// fsubs f12,f22,f0
	ctx.f12.f64 = double(float(ctx.f22.f64 - ctx.f0.f64));
	// lfs f13,18832(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 18832);
	ctx.f13.f64 = double(temp.f32);
	// fmadds f0,f12,f13,f0
	ctx.f0.f64 = double(float(std::fma(ctx.f12.f64, ctx.f13.f64, ctx.f0.f64)));
	// stfs f0,372(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 372, temp.u32);
loc_8253B184:
	// clrlwi r11,r17,24
	ctx.r11.u64 = ctx.r17.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8253b19c
	if (ctx.cr6.eq) goto loc_8253B19C;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f0,18648(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 18648);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,372(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 372, temp.u32);
loc_8253B19C:
	// addi r10,r1,144
	ctx.r10.s64 = ctx.r1.s64 + 144;
	// li r11,352
	ctx.r11.s64 = 352;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx128 v0,r31,r11
	ea = (ctx.r31.u32 + ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lbz r11,396(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 396);
	// rlwinm r11,r11,0,29,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x4;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8253b1e8
	if (ctx.cr6.eq) goto loc_8253B1E8;
	// lbz r11,557(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 557);
	// cmplwi cr6,r11,3
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 3, ctx.xer);
	// bne cr6,0x8253aba8
	if (!ctx.cr6.eq) goto loc_8253ABA8;
	// mr r6,r30
	ctx.r6.u64 = ctx.r30.u64;
	// lfs f2,160(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 160);
	ctx.f2.f64 = double(temp.f32);
	// addi r5,r1,144
	ctx.r5.s64 = ctx.r1.s64 + 144;
	// lfs f1,140(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 140);
	ctx.f1.f64 = double(temp.f32);
	// mr r4,r20
	ctx.r4.u64 = ctx.r20.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8253a1f0
	ctx.lr = 0x8253B1E4;
	sub_8253A1F0(ctx, base);
	// b 0x8253bc54
	goto loc_8253BC54;
loc_8253B1E8:
	// lfs f18,132(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 132);
	ctx.f18.f64 = double(temp.f32);
	// addi r6,r1,144
	ctx.r6.s64 = ctx.r1.s64 + 144;
	// addi r5,r1,176
	ctx.r5.s64 = ctx.r1.s64 + 176;
	// fmr f3,f18
	ctx.f3.f64 = ctx.f18.f64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// fmr f2,f22
	ctx.f2.f64 = ctx.f22.f64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82537970
	ctx.lr = 0x8253B208;
	sub_82537970(ctx, base);
	// addi r5,r1,144
	ctx.r5.s64 = ctx.r1.s64 + 144;
	// addi r4,r1,176
	ctx.r4.s64 = ctx.r1.s64 + 176;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82535948
	ctx.lr = 0x8253B218;
	sub_82535948(ctx, base);
	// addi r11,r1,144
	ctx.r11.s64 = ctx.r1.s64 + 144;
	// addi r21,r31,576
	ctx.r21.s64 = ctx.r31.s64 + 576;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,192
	ctx.r11.s64 = ctx.r1.s64 + 192;
	// lvx128 v13,r0,r21
	ea = (ctx.r21.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// vsubfp v0,v0,v13
	ctx.fpscr.enableFlushMode();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_sub_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v13.f32)));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lfs f13,196(r1)
	ctx.fpscr.disableFlushModeUnconditional();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 196);
	ctx.f13.f64 = double(temp.f32);
	// fmuls f12,f13,f13
	ctx.f12.f64 = double(float(ctx.f13.f64 * ctx.f13.f64));
	// lfs f0,192(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 192);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,200(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 200);
	ctx.f13.f64 = double(temp.f32);
	// fmadds f0,f0,f0,f12
	ctx.f0.f64 = double(float(std::fma(ctx.f0.f64, ctx.f0.f64, ctx.f12.f64)));
	// fmadds f0,f13,f13,f0
	ctx.f0.f64 = double(float(std::fma(ctx.f13.f64, ctx.f13.f64, ctx.f0.f64)));
	// fsqrts f21,f0
	ctx.f21.f64 = double(float(sqrt(ctx.f0.f64)));
	// fcmpu cr6,f21,f22
	ctx.cr6.compare(ctx.f21.f64, ctx.f22.f64);
	// bge cr6,0x8253b26c
	if (!ctx.cr6.lt) goto loc_8253B26C;
	// fcmpu cr6,f22,f30
	ctx.cr6.compare(ctx.f22.f64, ctx.f30.f64);
	// ble cr6,0x8253b26c
	if (!ctx.cr6.gt) goto loc_8253B26C;
	// fsubs f0,f30,f21
	ctx.f0.f64 = double(float(ctx.f30.f64 - ctx.f21.f64));
	// fsel f22,f0,f30,f21
	ctx.f22.f64 = ctx.f0.f64 >= 0.0 ? ctx.f30.f64 : ctx.f21.f64;
	// stfs f22,116(r1)
	temp.f32 = float(ctx.f22.f64);
	REX_STORE_U32(ctx.r1.u32 + 116, temp.u32);
loc_8253B26C:
	// addi r4,r1,176
	ctx.r4.s64 = ctx.r1.s64 + 176;
	// fmr f1,f18
	ctx.fpscr.disableFlushMode();
	ctx.f1.f64 = ctx.f18.f64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x825359d8
	ctx.lr = 0x8253B27C;
	sub_825359D8(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8253b2a8
	if (!ctx.cr6.eq) goto loc_8253B2A8;
	// addi r10,r1,144
	ctx.r10.s64 = ctx.r1.s64 + 144;
	// li r11,560
	ctx.r11.s64 = 560;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lvx128 v13,r31,r11
	ea = (ctx.r31.u32 + ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,176
	ctx.r11.s64 = ctx.r1.s64 + 176;
	// vsubfp v13,v0,v13
	ctx.fpscr.enableFlushMode();
	simde_mm_store_ps(ctx.v13.f32, simde_mm_sub_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v13.f32)));
	// stvx128 v13,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// b 0x8253b2b0
	goto loc_8253B2B0;
loc_8253B2A8:
	// addi r11,r1,176
	ctx.r11.s64 = ctx.r1.s64 + 176;
	// lvx128 v13,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
loc_8253B2B0:
	// vmsum3fp128 v0,v13,v13
	ctx.fpscr.enableFlushMode();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_dp_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v13.f32), 0xEF));
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// vspltisw v9,0
	simde_mm_store_si128((simde__m128i*)ctx.v9.u32, simde_mm_set1_epi32(int(0x0)));
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// addi r11,r11,-18560
	ctx.r11.s64 = ctx.r11.s64 + -18560;
	// lfs f0,52(r25)
	ctx.fpscr.disableFlushModeUnconditional();
	temp.u32 = REX_LOAD_U32(ctx.r25.u32 + 52);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,124(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 124, temp.u32);
	// lfs f27,56(r25)
	temp.u32 = REX_LOAD_U32(ctx.r25.u32 + 56);
	ctx.f27.f64 = double(temp.f32);
	// lfs f0,3624(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 3624);
	ctx.f0.f64 = double(temp.f32);
	// lis r10,-32080
	ctx.r10.s64 = -2102394880;
	// lvx128 v12,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// addi r10,r10,29088
	ctx.r10.s64 = ctx.r10.s64 + 29088;
	// addi r11,r11,29104
	ctx.r11.s64 = ctx.r11.s64 + 29104;
	// lvx128 v10,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v10.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,176
	ctx.r10.s64 = ctx.r1.s64 + 176;
	// lvx128 v11,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v11.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// vor v8,v0,v0
	simde_mm_store_si128((simde__m128i*)ctx.v8.u8, simde_mm_load_si128((simde__m128i*)ctx.v0.u8));
	// vrsqrtefp v0,v0
	ctx.fpscr.enableFlushModeUnconditional();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(simde_mm_load_ps(ctx.v0.f32))));
	// lfs f13,-21004(r11)
	ctx.fpscr.disableFlushModeUnconditional();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -21004);
	ctx.f13.f64 = double(temp.f32);
	// fcmpu cr6,f13,f0
	ctx.cr6.compare(ctx.f13.f64, ctx.f0.f64);
	// vor v7,v0,v0
	simde_mm_store_si128((simde__m128i*)ctx.v7.u8, simde_mm_load_si128((simde__m128i*)ctx.v0.u8));
	// vcmpeqfp v0,v0,v12
	ctx.fpscr.enableFlushModeUnconditional();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_cmpeq_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v12.f32)));
	// vsel v0,v7,v9,v0
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)ctx.v7.u8)), simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)ctx.v9.u8))));
	// vmulfp128 v12,v0,v0
	simde_mm_store_ps(ctx.v12.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v0.f32)));
	// vmulfp128 v11,v11,v0
	simde_mm_store_ps(ctx.v11.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v11.f32), simde_mm_load_ps(ctx.v0.f32)));
	// vnmsubfp v12,v8,v12,v10
	simde_mm_store_ps(ctx.v12.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(ctx.v8.f32), simde_mm_load_ps(ctx.v12.f32)), simde_mm_load_ps(ctx.v10.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
	// vmaddfp v0,v12,v11,v0
	simde_mm_store_ps(ctx.v0.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(ctx.v12.f32), simde_mm_load_ps(ctx.v11.f32)), simde_mm_load_ps(ctx.v0.f32)));
	// vmulfp128 v0,v13,v0
	simde_mm_store_ps(ctx.v0.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v0.f32)));
	// stvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// beq cr6,0x8253b338
	if (ctx.cr6.eq) goto loc_8253B338;
	// stfs f13,124(r1)
	ctx.fpscr.disableFlushModeUnconditional();
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r1.u32 + 124, temp.u32);
	// stfs f0,-21004(r11)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r11.u32 + -21004, temp.u32);
loc_8253B338:
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// lfs f13,-21000(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -21000);
	ctx.f13.f64 = double(temp.f32);
	// fcmpu cr6,f13,f0
	ctx.cr6.compare(ctx.f13.f64, ctx.f0.f64);
	// beq cr6,0x8253b350
	if (ctx.cr6.eq) goto loc_8253B350;
	// stfs f0,-21000(r11)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r11.u32 + -21000, temp.u32);
	// fmr f27,f13
	ctx.f27.f64 = ctx.f13.f64;
loc_8253B350:
	// addi r7,r1,124
	ctx.r7.s64 = ctx.r1.s64 + 124;
	// addi r6,r1,144
	ctx.r6.s64 = ctx.r1.s64 + 144;
	// addi r5,r1,176
	ctx.r5.s64 = ctx.r1.s64 + 176;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82537e40
	ctx.lr = 0x8253B368;
	sub_82537E40(ctx, base);
	// lbz r11,-27512(r15)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r15.u32 + -27512);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8253b380
	if (ctx.cr6.eq) goto loc_8253B380;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82147e90
	ctx.lr = 0x8253B37C;
	sub_82147E90(ctx, base);
	// b 0x8253b388
	goto loc_8253B388;
loc_8253B380:
	// addi r3,r1,176
	ctx.r3.s64 = ctx.r1.s64 + 176;
	// bl 0x8236aa50
	ctx.lr = 0x8253B388;
	sub_8236AA50(ctx, base);
loc_8253B388:
	// fmr f29,f1
	ctx.fpscr.disableFlushMode();
	ctx.f29.f64 = ctx.f1.f64;
	// bl 0x8236bcf0
	ctx.lr = 0x8253B390;
	sub_8236BCF0(ctx, base);
	// lwz r11,572(r3)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r3.u32 + 572);
	// rlwinm r11,r11,0,2,2
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x20000000;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8253b3bc
	if (ctx.cr6.eq) goto loc_8253B3BC;
	// lwz r11,2688(r3)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r3.u32 + 2688);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8253b3bc
	if (ctx.cr6.eq) goto loc_8253B3BC;
	// lwz r11,3904(r11)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r11.u32 + 3904);
	// cmplw cr6,r11,r3
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, ctx.r3.u32, ctx.xer);
	// mr r11,r19
	ctx.r11.u64 = ctx.r19.u64;
	// beq cr6,0x8253b3c0
	if (ctx.cr6.eq) goto loc_8253B3C0;
loc_8253B3BC:
	// li r11,0
	ctx.r11.s64 = 0;
loc_8253B3C0:
	// clrlwi r29,r24,24
	ctx.r29.u64 = ctx.r24.u32 & 0xFF;
	// clrlwi r27,r11,24
	ctx.r27.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r29,0
	ctx.cr6.compare<uint32_t>(ctx.r29.u32, 0, ctx.xer);
	// beq cr6,0x8253b3e0
	if (ctx.cr6.eq) goto loc_8253B3E0;
	// clrlwi r11,r27,24
	ctx.r11.u64 = ctx.r27.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8253b3e0
	if (ctx.cr6.eq) goto loc_8253B3E0;
	// lfs f29,404(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 404);
	ctx.f29.f64 = double(temp.f32);
loc_8253B3E0:
	// lfs f13,212(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 212);
	ctx.f13.f64 = double(temp.f32);
	// cmplwi cr6,r22,0
	ctx.cr6.compare<uint32_t>(ctx.r22.u32, 0, ctx.xer);
	// fmuls f13,f13,f13
	ctx.f13.f64 = double(float(ctx.f13.f64 * ctx.f13.f64));
	// lfs f0,208(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 208);
	ctx.f0.f64 = double(temp.f32);
	// lis r23,-32256
	ctx.r23.s64 = -2113929216;
	// fmadds f0,f0,f0,f13
	ctx.f0.f64 = double(float(std::fma(ctx.f0.f64, ctx.f0.f64, ctx.f13.f64)));
	// fsqrts f20,f0
	ctx.f20.f64 = double(float(sqrt(ctx.f0.f64)));
	// beq cr6,0x8253b40c
	if (ctx.cr6.eq) goto loc_8253B40C;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f0,19088(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 19088);
	ctx.f0.f64 = double(temp.f32);
	// b 0x8253b410
	goto loc_8253B410;
loc_8253B40C:
	// lfs f0,3400(r23)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r23.u32 + 3400);
	ctx.f0.f64 = double(temp.f32);
loc_8253B410:
	// fcmpu cr6,f20,f0
	ctx.fpscr.disableFlushMode();
	ctx.cr6.compare(ctx.f20.f64, ctx.f0.f64);
	// mr r11,r19
	ctx.r11.u64 = ctx.r19.u64;
	// blt cr6,0x8253b420
	if (ctx.cr6.lt) goto loc_8253B420;
	// li r11,0
	ctx.r11.s64 = 0;
loc_8253B420:
	// lis r26,-31996
	ctx.r26.s64 = -2096889856;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r22,0
	ctx.cr6.compare<uint32_t>(ctx.r22.u32, 0, ctx.xer);
	// lbz r9,-27534(r26)
	ctx.r9.u64 = REX_LOAD_U8(ctx.r26.u32 + -27534);
	// beq cr6,0x8253b454
	if (ctx.cr6.eq) goto loc_8253B454;
	// lis r10,-32076
	ctx.r10.s64 = -2102132736;
	// lwz r10,-27388(r10)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r10.u32 + -27388);
	// cmpwi cr6,r10,2
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 2, ctx.xer);
	// blt cr6,0x8253b454
	if (ctx.cr6.lt) goto loc_8253B454;
	// clrlwi r10,r9,24
	ctx.r10.u64 = ctx.r9.u32 & 0xFF;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// bne cr6,0x8253b454
	if (!ctx.cr6.eq) goto loc_8253B454;
	// mr r11,r19
	ctx.r11.u64 = ctx.r19.u64;
loc_8253B454:
	// lfs f0,40(r25)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r25.u32 + 40);
	ctx.f0.f64 = double(temp.f32);
	// clrlwi r24,r11,24
	ctx.r24.u64 = ctx.r11.u32 & 0xFF;
	// fmuls f13,f0,f23
	ctx.f13.f64 = double(float(ctx.f0.f64 * ctx.f23.f64));
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f0,44(r25)
	temp.u32 = REX_LOAD_U32(ctx.r25.u32 + 44);
	ctx.f0.f64 = double(temp.f32);
	// fmr f31,f29
	ctx.f31.f64 = ctx.f29.f64;
	// fmuls f28,f0,f23
	ctx.f28.f64 = double(float(ctx.f0.f64 * ctx.f23.f64));
	// cmplwi cr6,r24,0
	ctx.cr6.compare<uint32_t>(ctx.r24.u32, 0, ctx.xer);
	// lis r28,-32254
	ctx.r28.s64 = -2113798144;
	// lfs f0,18596(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 18596);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f30,f13,f0
	ctx.f30.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
	// bne cr6,0x8253b59c
	if (!ctx.cr6.eq) goto loc_8253B59C;
	// lwz r11,0(r30)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r30.u32 + 0);
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// addi r3,r1,192
	ctx.r3.s64 = ctx.r1.s64 + 192;
	// lwz r11,232(r11)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r11.u32 + 232);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x8253B49C;
	REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// lwz r11,0(r30)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r30.u32 + 0);
	// mr r10,r3
	ctx.r10.u64 = ctx.r3.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// addi r3,r1,256
	ctx.r3.s64 = ctx.r1.s64 + 256;
	// lwz r11,232(r11)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r11.u32 + 232);
	// lfs f31,4(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f31.f64 = double(temp.f32);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x8253B4BC;
	REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// lfs f0,0(r3)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// fmr f2,f31
	ctx.f2.f64 = ctx.f31.f64;
	// fneg f1,f0
	ctx.f1.u64 = ctx.f0.u64 ^ 0x8000000000000000;
	// bl 0x829fffd8
	ctx.lr = 0x8253B4CC;
	sub_829FFFD8(ctx, base);
	// lwz r11,0(r30)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r30.u32 + 0);
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// addi r3,r1,256
	ctx.r3.s64 = ctx.r1.s64 + 256;
	// frsp f31,f1
	ctx.fpscr.disableFlushMode();
	ctx.f31.f64 = double(float(ctx.f1.f64));
	// lwz r11,232(r11)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r11.u32 + 232);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x8253B4E8;
	REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// lfs f11,180(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 180);
	ctx.f11.f64 = double(temp.f32);
	// lwz r10,0(r30)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r30.u32 + 0);
	// lfs f0,184(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 184);
	ctx.f0.f64 = double(temp.f32);
	// addi r9,r1,176
	ctx.r9.s64 = ctx.r1.s64 + 176;
	// lfs f13,176(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 176);
	ctx.f13.f64 = double(temp.f32);
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// addi r3,r1,192
	ctx.r3.s64 = ctx.r1.s64 + 192;
	// lfs f12,4(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// fmuls f12,f12,f11
	ctx.f12.f64 = double(float(ctx.f12.f64 * ctx.f11.f64));
	// lfs f11,8(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f11.f64 = double(temp.f32);
	// lfs f10,0(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f10.f64 = double(temp.f32);
	// lwz r10,232(r10)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r10.u32 + 232);
	// lvx128 v127,r0,r9
	ea = (ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v127.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// fmadds f0,f11,f0,f12
	ctx.f0.f64 = double(float(std::fma(ctx.f11.f64, ctx.f0.f64, ctx.f12.f64)));
	// fmadds f0,f13,f10,f0
	ctx.f0.f64 = double(float(std::fma(ctx.f13.f64, ctx.f10.f64, ctx.f0.f64)));
	// stfs f0,128(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 128, temp.u32);
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
	// bctrl 
	ctx.lr = 0x8253B534;
	REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// addi r11,r1,128
	ctx.r11.s64 = ctx.r1.s64 + 128;
	// lvx128 v13,r0,r3
	ea = (ctx.r3.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lvlx v0,0,r11
	temp.u32 = ctx.r11.u32;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
	// addi r11,r1,192
	ctx.r11.s64 = ctx.r1.s64 + 192;
	// vspltw v0,v0,0
	simde_mm_store_si128((simde__m128i*)ctx.v0.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)ctx.v0.u32), 0xFF));
	// vmulfp128 v0,v127,v0
	ctx.fpscr.enableFlushMode();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v127.f32), simde_mm_load_ps(ctx.v0.f32)));
	// vsubfp v0,v13,v0
	simde_mm_store_ps(ctx.v0.f32, simde_mm_sub_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v0.f32)));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-32244
	ctx.r11.s64 = -2113142784;
	// lfs f13,196(r1)
	ctx.fpscr.disableFlushModeUnconditional();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 196);
	ctx.f13.f64 = double(temp.f32);
	// fmuls f12,f13,f13
	ctx.f12.f64 = double(float(ctx.f13.f64 * ctx.f13.f64));
	// lfs f0,192(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 192);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,200(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 200);
	ctx.f13.f64 = double(temp.f32);
	// fmadds f0,f0,f0,f12
	ctx.f0.f64 = double(float(std::fma(ctx.f0.f64, ctx.f0.f64, ctx.f12.f64)));
	// fmadds f0,f13,f13,f0
	ctx.f0.f64 = double(float(std::fma(ctx.f13.f64, ctx.f13.f64, ctx.f0.f64)));
	// lfd f13,-4848(r11)
	ctx.f13.u64 = REX_LOAD_U64(ctx.r11.u32 + -4848);
	// lis r11,-32244
	ctx.r11.s64 = -2113142784;
	// lfd f12,-4840(r11)
	ctx.f12.u64 = REX_LOAD_U64(ctx.r11.u32 + -4840);
	// fsqrts f0,f0
	ctx.f0.f64 = double(float(sqrt(ctx.f0.f64)));
	// fmuls f0,f0,f30
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f30.f64));
	// fsel f11,f0,f0,f13
	ctx.f11.f64 = ctx.f0.f64 >= 0.0 ? ctx.f0.f64 : ctx.f13.f64;
	// fsel f0,f0,f0,f13
	ctx.f0.f64 = ctx.f0.f64 >= 0.0 ? ctx.f0.f64 : ctx.f13.f64;
	// fsub f11,f11,f12
	ctx.f11.f64 = ctx.f11.f64 - ctx.f12.f64;
	// fsel f0,f11,f12,f0
	ctx.f0.f64 = ctx.f11.f64 >= 0.0 ? ctx.f12.f64 : ctx.f0.f64;
	// frsp f30,f0
	ctx.f30.f64 = double(float(ctx.f0.f64));
	// b 0x8253b5e8
	goto loc_8253B5E8;
loc_8253B59C:
	// cmplwi cr6,r22,0
	ctx.cr6.compare<uint32_t>(ctx.r22.u32, 0, ctx.xer);
	// beq cr6,0x8253b5e8
	if (ctx.cr6.eq) goto loc_8253B5E8;
	// clrlwi r11,r9,24
	ctx.r11.u64 = ctx.r9.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8253b5e8
	if (!ctx.cr6.eq) goto loc_8253B5E8;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x822a6648
	ctx.lr = 0x8253B5B8;
	sub_822A6648(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8253b5e8
	if (!ctx.cr6.eq) goto loc_8253B5E8;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x822a66a0
	ctx.lr = 0x8253B5CC;
	sub_822A66A0(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8253b5e8
	if (!ctx.cr6.eq) goto loc_8253B5E8;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82147e90
	ctx.lr = 0x8253B5E0;
	sub_82147E90(ctx, base);
	// lfs f30,18636(r28)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r28.u32 + 18636);
	ctx.f30.f64 = double(temp.f32);
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
loc_8253B5E8:
	// cmplwi cr6,r29,0
	ctx.cr6.compare<uint32_t>(ctx.r29.u32, 0, ctx.xer);
	// beq cr6,0x8253b5f4
	if (ctx.cr6.eq) goto loc_8253B5F4;
	// fmr f30,f19
	ctx.fpscr.disableFlushMode();
	ctx.f30.f64 = ctx.f19.f64;
loc_8253B5F4:
	// lis r11,-32244
	ctx.r11.s64 = -2113142784;
	// lfs f12,-4388(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -4388);
	ctx.f12.f64 = double(temp.f32);
	// lis r11,-32244
	ctx.r11.s64 = -2113142784;
	// fadds f0,f29,f12
	ctx.f0.f64 = double(float(ctx.f29.f64 + ctx.f12.f64));
	// lfs f11,-4384(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -4384);
	ctx.f11.f64 = double(temp.f32);
	// fcmpu cr6,f31,f0
	ctx.cr6.compare(ctx.f31.f64, ctx.f0.f64);
	// ble cr6,0x8253b618
	if (!ctx.cr6.gt) goto loc_8253B618;
	// fsubs f31,f31,f11
	ctx.f31.f64 = double(float(ctx.f31.f64 - ctx.f11.f64));
	// b 0x8253b628
	goto loc_8253B628;
loc_8253B618:
	// fsubs f0,f29,f12
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(float(ctx.f29.f64 - ctx.f12.f64));
	// fcmpu cr6,f31,f0
	ctx.cr6.compare(ctx.f31.f64, ctx.f0.f64);
	// bge cr6,0x8253b628
	if (!ctx.cr6.lt) goto loc_8253B628;
	// fadds f31,f31,f11
	ctx.f31.f64 = double(float(ctx.f31.f64 + ctx.f11.f64));
loc_8253B628:
	// fsubs f0,f31,f29
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(float(ctx.f31.f64 - ctx.f29.f64));
	// fneg f13,f28
	ctx.f13.u64 = ctx.f28.u64 ^ 0x8000000000000000;
	// fmuls f0,f0,f30
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f30.f64));
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// bge cr6,0x8253b644
	if (!ctx.cr6.lt) goto loc_8253B644;
	// fmr f0,f13
	ctx.f0.f64 = ctx.f13.f64;
	// b 0x8253b650
	goto loc_8253B650;
loc_8253B644:
	// fcmpu cr6,f0,f28
	ctx.fpscr.disableFlushMode();
	ctx.cr6.compare(ctx.f0.f64, ctx.f28.f64);
	// ble cr6,0x8253b650
	if (!ctx.cr6.gt) goto loc_8253B650;
	// fmr f0,f28
	ctx.f0.f64 = ctx.f28.f64;
loc_8253B650:
	// lfs f13,404(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 404);
	ctx.f13.f64 = double(temp.f32);
	// fadds f24,f0,f29
	ctx.f24.f64 = double(float(ctx.f0.f64 + ctx.f29.f64));
	// fadds f0,f13,f12
	ctx.f0.f64 = double(float(ctx.f13.f64 + ctx.f12.f64));
	// fcmpu cr6,f24,f0
	ctx.cr6.compare(ctx.f24.f64, ctx.f0.f64);
	// ble cr6,0x8253b66c
	if (!ctx.cr6.gt) goto loc_8253B66C;
	// fsubs f24,f24,f11
	ctx.f24.f64 = double(float(ctx.f24.f64 - ctx.f11.f64));
	// b 0x8253b67c
	goto loc_8253B67C;
loc_8253B66C:
	// fsubs f0,f13,f12
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(float(ctx.f13.f64 - ctx.f12.f64));
	// fcmpu cr6,f24,f0
	ctx.cr6.compare(ctx.f24.f64, ctx.f0.f64);
	// bge cr6,0x8253b67c
	if (!ctx.cr6.lt) goto loc_8253B67C;
	// fadds f24,f24,f11
	ctx.f24.f64 = double(float(ctx.f24.f64 + ctx.f11.f64));
loc_8253B67C:
	// lwz r11,304(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 304);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8253b68c
	if (!ctx.cr6.eq) goto loc_8253B68C;
	// stfs f24,404(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f24.f64);
	REX_STORE_U32(ctx.r31.u32 + 404, temp.u32);
loc_8253B68C:
	// lfs f1,184(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 184);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x8236ada8
	ctx.lr = 0x8253B694;
	sub_8236ADA8(ctx, base);
	// cmplwi cr6,r29,0
	ctx.cr6.compare<uint32_t>(ctx.r29.u32, 0, ctx.xer);
	// beq cr6,0x8253b6b0
	if (ctx.cr6.eq) goto loc_8253B6B0;
	// clrlwi r11,r27,24
	ctx.r11.u64 = ctx.r27.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8253b6b0
	if (ctx.cr6.eq) goto loc_8253B6B0;
	// lfs f0,400(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 400);
	ctx.f0.f64 = double(temp.f32);
	// fadds f1,f0,f18
	ctx.f1.f64 = double(float(ctx.f0.f64 + ctx.f18.f64));
loc_8253B6B0:
	// lwz r11,304(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 304);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8253b6c0
	if (!ctx.cr6.eq) goto loc_8253B6C0;
	// stfs f1,400(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	REX_STORE_U32(ctx.r31.u32 + 400, temp.u32);
loc_8253B6C0:
	// fsubs f29,f1,f18
	ctx.fpscr.disableFlushMode();
	ctx.f29.f64 = double(float(ctx.f1.f64 - ctx.f18.f64));
	// lfs f26,124(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 124);
	ctx.f26.f64 = double(temp.f32);
	// fneg f25,f27
	ctx.f25.u64 = ctx.f27.u64 ^ 0x8000000000000000;
	// fcmpu cr6,f29,f25
	ctx.cr6.compare(ctx.f29.f64, ctx.f25.f64);
	// bge cr6,0x8253b6dc
	if (!ctx.cr6.lt) goto loc_8253B6DC;
	// fmr f29,f25
	ctx.f29.f64 = ctx.f25.f64;
	// b 0x8253b6e8
	goto loc_8253B6E8;
loc_8253B6DC:
	// fcmpu cr6,f29,f26
	ctx.fpscr.disableFlushMode();
	ctx.cr6.compare(ctx.f29.f64, ctx.f26.f64);
	// ble cr6,0x8253b6e8
	if (!ctx.cr6.gt) goto loc_8253B6E8;
	// fmr f29,f26
	ctx.f29.f64 = ctx.f26.f64;
loc_8253B6E8:
native_pitch_lower = ctx.f25.f64;
native_pitch_upper = ctx.f26.f64;
	// lfs f0,3400(r23)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r23.u32 + 3400);
	ctx.f0.f64 = double(temp.f32);
	// fmr f2,f23
	ctx.f2.f64 = ctx.f23.f64;
	// fsubs f13,f0,f23
	ctx.f13.f64 = double(float(ctx.f0.f64 - ctx.f23.f64));
	// lfs f12,400(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 400);
	ctx.f12.f64 = double(temp.f32);
	// fsubs f12,f29,f12
	ctx.f12.f64 = double(float(ctx.f29.f64 - ctx.f12.f64));
	// lfs f1,20(r25)
	temp.u32 = REX_LOAD_U32(ctx.r25.u32 + 20);
	ctx.f1.f64 = double(temp.f32);
	// fsel f27,f13,f0,f23
	ctx.f27.f64 = ctx.f13.f64 >= 0.0 ? ctx.f0.f64 : ctx.f23.f64;
	// fdivs f30,f12,f27
	ctx.f30.f64 = double(float(ctx.f12.f64 / ctx.f27.f64));
	// bl 0x82a02648
	ctx.lr = 0x8253B70C;
	sub_82A02648(ctx, base);
	// lfs f0,24(r25)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r25.u32 + 24);
	ctx.f0.f64 = double(temp.f32);
	// cmplwi cr6,r29,0
	ctx.cr6.compare<uint32_t>(ctx.r29.u32, 0, ctx.xer);
	// fmuls f31,f0,f23
	ctx.f31.f64 = double(float(ctx.f0.f64 * ctx.f23.f64));
	// frsp f28,f1
	ctx.f28.f64 = double(float(ctx.f1.f64));
	// bne cr6,0x8253b7a4
	if (!ctx.cr6.eq) goto loc_8253B7A4;
	// cmpwi cr6,r20,1
	ctx.cr6.compare<int32_t>(ctx.r20.s32, 1, ctx.xer);
	// bgt cr6,0x8253b7a4
	if (ctx.cr6.gt) goto loc_8253B7A4;
	// lfs f0,400(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 400);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f29,f0
	ctx.cr6.compare(ctx.f29.f64, ctx.f0.f64);
	// bge cr6,0x8253b7a4
	if (!ctx.cr6.lt) goto loc_8253B7A4;
	// fcmpu cr6,f21,f22
	ctx.cr6.compare(ctx.f21.f64, ctx.f22.f64);
	// blt cr6,0x8253b7a4
	if (ctx.cr6.lt) goto loc_8253B7A4;
	// lwz r11,3904(r30)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r30.u32 + 3904);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8253b7a4
	if (ctx.cr6.eq) goto loc_8253B7A4;
	// lbz r10,528(r11)
	ctx.r10.u64 = REX_LOAD_U8(ctx.r11.u32 + 528);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// bne cr6,0x8253b764
	if (!ctx.cr6.eq) goto loc_8253B764;
	// lbz r11,529(r11)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r11.u32 + 529);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// mr r11,r19
	ctx.r11.u64 = ctx.r19.u64;
	// bne cr6,0x8253b768
	if (!ctx.cr6.eq) goto loc_8253B768;
loc_8253B764:
	// li r11,0
	ctx.r11.s64 = 0;
loc_8253B768:
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8253b7a4
	if (ctx.cr6.eq) goto loc_8253B7A4;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82378c50
	ctx.lr = 0x8253B77C;
	sub_82378C50(ctx, base);
	// cmpwi cr6,r3,1
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 1, ctx.xer);
	// ble cr6,0x8253b7a4
	if (!ctx.cr6.gt) goto loc_8253B7A4;
	// lfs f0,400(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 400);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// fsubs f13,f29,f0
	ctx.f13.f64 = double(float(ctx.f29.f64 - ctx.f0.f64));
	// lfs f12,140(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 140);
	ctx.f12.f64 = double(temp.f32);
	// lfs f0,18640(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 18640);
	ctx.f0.f64 = double(temp.f32);
	// fmadds f10,f13,f0,f12
	ctx.f10.f64 = double(float(std::fma(ctx.f13.f64, ctx.f0.f64, ctx.f12.f64)));
	// stfs f10,140(r1)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r1.u32 + 140, temp.u32);
	// b 0x8253b7a8
	goto loc_8253B7A8;
loc_8253B7A4:
	// lfs f10,140(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 140);
	ctx.f10.f64 = double(temp.f32);
loc_8253B7A8:
	// fcmpu cr6,f10,f19
	ctx.fpscr.disableFlushMode();
	ctx.cr6.compare(ctx.f10.f64, ctx.f19.f64);
	// ble cr6,0x8253b7bc
	if (!ctx.cr6.gt) goto loc_8253B7BC;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,3444(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 3444);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f31,f31,f0
	ctx.f31.f64 = double(float(ctx.f31.f64 * ctx.f0.f64));
loc_8253B7BC:
	// fadds f0,f30,f10
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(float(ctx.f30.f64 + ctx.f10.f64));
	// fneg f13,f31
	ctx.f13.u64 = ctx.f31.u64 ^ 0x8000000000000000;
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// bge cr6,0x8253b7d4
	if (!ctx.cr6.lt) goto loc_8253B7D4;
	// fmr f0,f13
	ctx.f0.f64 = ctx.f13.f64;
	// b 0x8253b7e0
	goto loc_8253B7E0;
loc_8253B7D4:
	// fcmpu cr6,f0,f31
	ctx.fpscr.disableFlushMode();
	ctx.cr6.compare(ctx.f0.f64, ctx.f31.f64);
	// ble cr6,0x8253b7e0
	if (!ctx.cr6.gt) goto loc_8253B7E0;
	// fmr f0,f31
	ctx.f0.f64 = ctx.f31.f64;
loc_8253B7E0:
	// lfs f13,3400(r23)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r23.u32 + 3400);
	ctx.f13.f64 = double(temp.f32);
	// cmplwi cr6,r22,0
	ctx.cr6.compare<uint32_t>(ctx.r22.u32, 0, ctx.xer);
	// beq cr6,0x8253b848
	if (ctx.cr6.eq) goto loc_8253B848;
	// lbz r11,-27534(r26)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r26.u32 + -27534);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8253b848
	if (!ctx.cr6.eq) goto loc_8253B848;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f11,400(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 400);
	ctx.f11.f64 = double(temp.f32);
	// lfs f12,19112(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 19112);
	ctx.f12.f64 = double(temp.f32);
	// fsubs f11,f12,f11
	ctx.f11.f64 = double(float(ctx.f12.f64 - ctx.f11.f64));
	// fmuls f12,f11,f10
	ctx.f12.f64 = double(float(ctx.f11.f64 * ctx.f10.f64));
	// fcmpu cr6,f12,f19
	ctx.cr6.compare(ctx.f12.f64, ctx.f19.f64);
	// bge cr6,0x8253b81c
	if (!ctx.cr6.lt) goto loc_8253B81C;
	// stfs f19,840(r31)
	temp.f32 = float(ctx.f19.f64);
	REX_STORE_U32(ctx.r31.u32 + 840, temp.u32);
	// b 0x8253b848
	goto loc_8253B848;
loc_8253B81C:
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f10,840(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 840);
	ctx.f10.f64 = double(temp.f32);
	// lfs f12,-22848(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -22848);
	ctx.f12.f64 = double(temp.f32);
	// fnmsubs f12,f20,f12,f13
	ctx.f12.f64 = double(float(-std::fma(ctx.f20.f64, ctx.f12.f64, -ctx.f13.f64)));
	// fsel f12,f12,f12,f19
	ctx.f12.f64 = ctx.f12.f64 >= 0.0 ? ctx.f12.f64 : ctx.f19.f64;
	// fmuls f10,f12,f10
	ctx.f10.f64 = double(float(ctx.f12.f64 * ctx.f10.f64));
	// lfs f12,18636(r28)
	temp.u32 = REX_LOAD_U32(ctx.r28.u32 + 18636);
	ctx.f12.f64 = double(temp.f32);
	// fmuls f12,f10,f12
	ctx.f12.f64 = double(float(ctx.f10.f64 * ctx.f12.f64));
	// fsubs f10,f12,f13
	ctx.f10.f64 = double(float(ctx.f12.f64 - ctx.f13.f64));
	// fsel f12,f10,f13,f12
	ctx.f12.f64 = ctx.f10.f64 >= 0.0 ? ctx.f13.f64 : ctx.f12.f64;
	// fmadds f0,f12,f11,f0
	ctx.f0.f64 = double(float(std::fma(ctx.f12.f64, ctx.f11.f64, ctx.f0.f64)));
loc_8253B848:
	// lfs f12,412(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 412);
	ctx.f12.f64 = double(temp.f32);
	// fsubs f13,f13,f28
	ctx.f13.f64 = double(float(ctx.f13.f64 - ctx.f28.f64));
	// fmuls f12,f28,f12
	ctx.f12.f64 = double(float(ctx.f28.f64 * ctx.f12.f64));
	// lis r28,-32254
	ctx.r28.s64 = -2113798144;
	// fmadds f0,f13,f0,f12
	ctx.f0.f64 = double(float(std::fma(ctx.f13.f64, ctx.f0.f64, ctx.f12.f64)));
	// stfs f0,412(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 412, temp.u32);
	// fabs f13,f0
	ctx.f13.u64 = ctx.f0.u64 & ~0x8000000000000000;
	// lfs f0,18604(r28)
	temp.u32 = REX_LOAD_U32(ctx.r28.u32 + 18604);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f13,f0
	ctx.cr6.compare(ctx.f13.f64, ctx.f0.f64);
	// bge cr6,0x8253b874
	if (!ctx.cr6.lt) goto loc_8253B874;
	// stfs f19,412(r31)
	temp.f32 = float(ctx.f19.f64);
	REX_STORE_U32(ctx.r31.u32 + 412, temp.u32);
loc_8253B874:
	// lfs f0,400(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 400);
	ctx.f0.f64 = double(temp.f32);
	// cmplwi cr6,r22,0
	ctx.cr6.compare<uint32_t>(ctx.r22.u32, 0, ctx.xer);
	// lfs f13,412(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 412);
	ctx.f13.f64 = double(temp.f32);
	// fmadds f0,f13,f23,f0
	ctx.f0.f64 = double(float(std::fma(ctx.f13.f64, ctx.f23.f64, ctx.f0.f64)));
native_mouse = gta4::input::TakeMouseRotation(base, native_control, ctx.r31.u32, gta4::input::LoadFloat(base, ctx.r31.u32 + 96), true);
native_mouse.pitch = gta4::input::ConstrainMousePitch(ctx.f0.f64, native_mouse.pitch, native_pitch_lower, native_pitch_upper);
ctx.f0.f64 += native_mouse.pitch;
	// stfs f0,400(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 400, temp.u32);
	// beq cr6,0x8253b8cc
	if (ctx.cr6.eq) goto loc_8253B8CC;
	// lwz r11,304(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 304);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8253b89c
	if (!ctx.cr6.eq) goto loc_8253B89C;
	// stfs f29,400(r31)
	temp.f32 = float(ctx.f29.f64);
	REX_STORE_U32(ctx.r31.u32 + 400, temp.u32);
loc_8253B89C:
	// cmplwi cr6,r22,0
	ctx.cr6.compare<uint32_t>(ctx.r22.u32, 0, ctx.xer);
	// beq cr6,0x8253b8cc
	if (ctx.cr6.eq) goto loc_8253B8CC;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,152(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 152);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,9096(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 9096);
	ctx.f13.f64 = double(temp.f32);
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// bgt cr6,0x8253b8cc
	if (ctx.cr6.gt) goto loc_8253B8CC;
	// fsel f0,f0,f0,f19
	ctx.f0.f64 = ctx.f0.f64 >= 0.0 ? ctx.f0.f64 : ctx.f19.f64;
	// lis r11,-32244
	ctx.r11.s64 = -2113142784;
	// fmuls f13,f0,f26
	ctx.f13.f64 = double(float(ctx.f0.f64 * ctx.f26.f64));
	// lfs f0,-4648(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -4648);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f26,f13,f0
	ctx.f26.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
loc_8253B8CC:
	// mr r6,r17
	ctx.r6.u64 = ctx.r17.u64;
	// fmr f2,f25
	ctx.fpscr.disableFlushMode();
	ctx.f2.f64 = ctx.f25.f64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// fmr f1,f26
	ctx.f1.f64 = ctx.f26.f64;
	// bl 0x82536348
	ctx.lr = 0x8253B8E0;
	sub_82536348(ctx, base);
	// lfs f0,404(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 404);
	ctx.f0.f64 = double(temp.f32);
	// fsubs f0,f24,f0
	ctx.f0.f64 = double(float(ctx.f24.f64 - ctx.f0.f64));
	// cmplwi cr6,r22,0
	ctx.cr6.compare<uint32_t>(ctx.r22.u32, 0, ctx.xer);
	// fdivs f31,f0,f27
	ctx.f31.f64 = double(float(ctx.f0.f64 / ctx.f27.f64));
	// beq cr6,0x8253b9e0
	if (ctx.cr6.eq) goto loc_8253B9E0;
	// lbz r11,-27534(r26)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r26.u32 + -27534);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8253b9e0
	if (!ctx.cr6.eq) goto loc_8253B9E0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x822a6648
	ctx.lr = 0x8253B908;
	sub_822A6648(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8253b9b4
	if (!ctx.cr6.eq) goto loc_8253B9B4;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x822a66a0
	ctx.lr = 0x8253B91C;
	sub_822A66A0(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8253b9b4
	if (!ctx.cr6.eq) goto loc_8253B9B4;
	// cmplwi cr6,r24,0
	ctx.cr6.compare<uint32_t>(ctx.r24.u32, 0, ctx.xer);
	// beq cr6,0x8253b9b4
	if (ctx.cr6.eq) goto loc_8253B9B4;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// lwz r29,3956(r30)
	ctx.r29.u64 = REX_LOAD_U32(ctx.r30.u32 + 3956);
	// bl 0x82378c50
	ctx.lr = 0x8253B93C;
	sub_82378C50(ctx, base);
	// cmpw cr6,r3,r29
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r29.s32, ctx.xer);
	// beq cr6,0x8253b9b4
	if (ctx.cr6.eq) goto loc_8253B9B4;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f12,3400(r23)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r23.u32 + 3400);
	ctx.f12.f64 = double(temp.f32);
	// lfs f0,19100(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 19100);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f13,f23,f0
	ctx.f13.f64 = double(float(ctx.f23.f64 * ctx.f0.f64));
	// lfs f0,836(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 836);
	ctx.f0.f64 = double(temp.f32);
	// fadds f0,f0,f13
	ctx.f0.f64 = double(float(ctx.f0.f64 + ctx.f13.f64));
	// stfs f0,836(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 836, temp.u32);
	// fcmpu cr6,f0,f19
	ctx.cr6.compare(ctx.f0.f64, ctx.f19.f64);
	// bge cr6,0x8253b970
	if (!ctx.cr6.lt) goto loc_8253B970;
	// fmr f0,f19
	ctx.f0.f64 = ctx.f19.f64;
	// b 0x8253b97c
	goto loc_8253B97C;
loc_8253B970:
	// fcmpu cr6,f0,f12
	ctx.fpscr.disableFlushMode();
	ctx.cr6.compare(ctx.f0.f64, ctx.f12.f64);
	// ble cr6,0x8253b97c
	if (!ctx.cr6.gt) goto loc_8253B97C;
	// fmr f0,f12
	ctx.f0.f64 = ctx.f12.f64;
loc_8253B97C:
	// lfs f11,840(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 840);
	ctx.f11.f64 = double(temp.f32);
	// fadds f13,f13,f11
	ctx.f13.f64 = double(float(ctx.f13.f64 + ctx.f11.f64));
	// stfs f0,836(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 836, temp.u32);
	// stfs f13,840(r31)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r31.u32 + 840, temp.u32);
	// fcmpu cr6,f13,f19
	ctx.cr6.compare(ctx.f13.f64, ctx.f19.f64);
	// bge cr6,0x8253b9a0
	if (!ctx.cr6.lt) goto loc_8253B9A0;
	// fmr f13,f19
	ctx.f13.f64 = ctx.f19.f64;
	// stfs f13,840(r31)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r31.u32 + 840, temp.u32);
	// b 0x8253b9bc
	goto loc_8253B9BC;
loc_8253B9A0:
	// fcmpu cr6,f13,f12
	ctx.fpscr.disableFlushMode();
	ctx.cr6.compare(ctx.f13.f64, ctx.f12.f64);
	// ble cr6,0x8253b9ac
	if (!ctx.cr6.gt) goto loc_8253B9AC;
	// fmr f13,f12
	ctx.f13.f64 = ctx.f12.f64;
loc_8253B9AC:
	// stfs f13,840(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r31.u32 + 840, temp.u32);
	// b 0x8253b9bc
	goto loc_8253B9BC;
loc_8253B9B4:
	// stfs f19,840(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f19.f64);
	REX_STORE_U32(ctx.r31.u32 + 840, temp.u32);
	// stfs f19,836(r31)
	temp.f32 = float(ctx.f19.f64);
	REX_STORE_U32(ctx.r31.u32 + 836, temp.u32);
loc_8253B9BC:
	// lfs f0,160(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 160);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f0,f31,f0
	ctx.f0.f64 = double(float(ctx.f31.f64 * ctx.f0.f64));
	// fcmpu cr6,f0,f19
	ctx.cr6.compare(ctx.f0.f64, ctx.f19.f64);
	// bge cr6,0x8253b9d0
	if (!ctx.cr6.lt) goto loc_8253B9D0;
	// stfs f19,836(r31)
	temp.f32 = float(ctx.f19.f64);
	REX_STORE_U32(ctx.r31.u32 + 836, temp.u32);
loc_8253B9D0:
	// cmplwi cr6,r24,0
	ctx.cr6.compare<uint32_t>(ctx.r24.u32, 0, ctx.xer);
	// beq cr6,0x8253b9e0
	if (ctx.cr6.eq) goto loc_8253B9E0;
	// lfs f0,836(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 836);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f31,f0,f31
	ctx.f31.f64 = double(float(ctx.f0.f64 * ctx.f31.f64));
loc_8253B9E0:
	// lfs f1,32(r25)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r25.u32 + 32);
	ctx.f1.f64 = double(temp.f32);
	// fmr f2,f23
	ctx.f2.f64 = ctx.f23.f64;
	// bl 0x82a02648
	ctx.lr = 0x8253B9EC;
	sub_82A02648(ctx, base);
	// lfs f13,36(r25)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r25.u32 + 36);
	ctx.f13.f64 = double(temp.f32);
	// lfs f0,160(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 160);
	ctx.f0.f64 = double(temp.f32);
	// fneg f12,f13
	ctx.f12.u64 = ctx.f13.u64 ^ 0x8000000000000000;
	// fadds f0,f31,f0
	ctx.f0.f64 = double(float(ctx.f31.f64 + ctx.f0.f64));
	// frsp f11,f1
	ctx.f11.f64 = double(float(ctx.f1.f64));
	// fcmpu cr6,f0,f12
	ctx.cr6.compare(ctx.f0.f64, ctx.f12.f64);
	// bge cr6,0x8253ba10
	if (!ctx.cr6.lt) goto loc_8253BA10;
	// fmr f0,f12
	ctx.f0.f64 = ctx.f12.f64;
	// b 0x8253ba1c
	goto loc_8253BA1C;
loc_8253BA10:
	// fcmpu cr6,f0,f13
	ctx.fpscr.disableFlushMode();
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// ble cr6,0x8253ba1c
	if (!ctx.cr6.gt) goto loc_8253BA1C;
	// fmr f0,f13
	ctx.f0.f64 = ctx.f13.f64;
loc_8253BA1C:
	// lfs f12,408(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 408);
	ctx.f12.f64 = double(temp.f32);
	// mr r5,r18
	ctx.r5.u64 = ctx.r18.u64;
	// lfs f13,3400(r23)
	temp.u32 = REX_LOAD_U32(ctx.r23.u32 + 3400);
	ctx.f13.f64 = double(temp.f32);
	// fmuls f12,f11,f12
	ctx.f12.f64 = double(float(ctx.f11.f64 * ctx.f12.f64));
	// fsubs f13,f13,f11
	ctx.f13.f64 = double(float(ctx.f13.f64 - ctx.f11.f64));
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// fmadds f0,f13,f0,f12
	ctx.f0.f64 = double(float(std::fma(ctx.f13.f64, ctx.f0.f64, ctx.f12.f64)));
	// stfs f0,408(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 408, temp.u32);
	// bl 0x825381c0
	ctx.lr = 0x8253BA44;
	sub_825381C0(ctx, base);
	// lfs f0,408(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 408);
	ctx.f0.f64 = double(temp.f32);
	// fadds f0,f1,f0
	ctx.f0.f64 = double(float(ctx.f1.f64 + ctx.f0.f64));
	// stfs f0,408(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 408, temp.u32);
	// fabs f13,f0
	ctx.f13.u64 = ctx.f0.u64 & ~0x8000000000000000;
	// lfs f0,18604(r28)
	temp.u32 = REX_LOAD_U32(ctx.r28.u32 + 18604);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f13,f0
	ctx.cr6.compare(ctx.f13.f64, ctx.f0.f64);
	// bge cr6,0x8253ba64
	if (!ctx.cr6.lt) goto loc_8253BA64;
	// stfs f19,408(r31)
	temp.f32 = float(ctx.f19.f64);
	REX_STORE_U32(ctx.r31.u32 + 408, temp.u32);
loc_8253BA64:
	// lfs f0,404(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 404);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,408(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 408);
	ctx.f13.f64 = double(temp.f32);
	// fmadds f1,f23,f13,f0
	ctx.f1.f64 = double(float(std::fma(ctx.f23.f64, ctx.f13.f64, ctx.f0.f64)));
ctx.f1.f64 += native_mouse.yaw;
	// stfs f1,404(r31)
	temp.f32 = float(ctx.f1.f64);
	REX_STORE_U32(ctx.r31.u32 + 404, temp.u32);
	// bl 0x82369828
	ctx.lr = 0x8253BA78;
	sub_82369828(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stfs f1,404(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	REX_STORE_U32(ctx.r31.u32 + 404, temp.u32);
	// bl 0x82537068
	ctx.lr = 0x8253BA84;
gta4::input::AcceptMouseAngles(base, ctx.r31.u32, native_mouse, 400, 404, 424, 428);
	sub_82537068(ctx, base);
	// addi r3,r1,224
	ctx.r3.s64 = ctx.r1.s64 + 224;
	// lfs f2,404(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 404);
	ctx.f2.f64 = double(temp.f32);
	// lfs f1,400(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 400);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x82369750
	ctx.lr = 0x8253BA94;
	sub_82369750(ctx, base);
	// addi r4,r1,224
	ctx.r4.s64 = ctx.r1.s64 + 224;
	// addi r3,r1,256
	ctx.r3.s64 = ctx.r1.s64 + 256;
	// bl 0x8236aa80
	ctx.lr = 0x8253BAA0;
	sub_8236AA80(ctx, base);
	// addi r11,r1,116
	ctx.r11.s64 = ctx.r1.s64 + 116;
	// addi r10,r1,224
	ctx.r10.s64 = ctx.r1.s64 + 224;
	// addi r29,r31,64
	ctx.r29.s64 = ctx.r31.s64 + 64;
	// li r4,4
	ctx.r4.s64 = 4;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// lvlx v12,0,r11
	temp.u32 = ctx.r11.u32;
	simde_mm_store_si128((simde__m128i*)ctx.v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
	// vspltw v12,v12,0
	simde_mm_store_si128((simde__m128i*)ctx.v12.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)ctx.v12.u32), 0xFF));
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,144
	ctx.r10.s64 = ctx.r1.s64 + 144;
	// vmulfp128 v0,v0,v12
	ctx.fpscr.enableFlushMode();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v12.f32)));
	// lvx128 v13,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// vsubfp v0,v13,v0
	simde_mm_store_ps(ctx.v0.f32, simde_mm_sub_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v0.f32)));
	// stvx128 v0,r0,r29
	ea = (ctx.r29.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x823696e8
	ctx.lr = 0x8253BAD8;
	sub_823696E8(ctx, base);
	// lfs f0,400(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 400);
	ctx.f0.f64 = double(temp.f32);
	// addi r3,r1,272
	ctx.r3.s64 = ctx.r1.s64 + 272;
	// lfs f2,404(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 404);
	ctx.f2.f64 = double(temp.f32);
	// fadds f1,f0,f18
	ctx.f1.f64 = double(float(ctx.f0.f64 + ctx.f18.f64));
	// bl 0x82369750
	ctx.lr = 0x8253BAEC;
	sub_82369750(ctx, base);
	// addi r9,r1,272
	ctx.r9.s64 = ctx.r1.s64 + 272;
	// addi r10,r1,136
	ctx.r10.s64 = ctx.r1.s64 + 136;
	// addi r8,r1,144
	ctx.r8.s64 = ctx.r1.s64 + 144;
	// addi r11,r31,560
	ctx.r11.s64 = ctx.r31.s64 + 560;
	// cmplwi cr6,r16,0
	ctx.cr6.compare<uint32_t>(ctx.r16.u32, 0, ctx.xer);
	// lvx128 v13,r0,r9
	ea = (ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r9,r1,116
	ctx.r9.s64 = ctx.r1.s64 + 116;
	// lvlx v12,0,r10
	temp.u32 = ctx.r10.u32;
	simde_mm_store_si128((simde__m128i*)ctx.v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
	// vspltw v12,v12,0
	simde_mm_store_si128((simde__m128i*)ctx.v12.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)ctx.v12.u32), 0xFF));
	// lvx128 v0,r0,r8
	ea = (ctx.r8.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r8,592
	ctx.r8.s64 = 592;
	// lvlx v11,0,r9
	temp.u32 = ctx.r9.u32;
	simde_mm_store_si128((simde__m128i*)ctx.v11.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
	// vspltw v11,v11,0
	simde_mm_store_si128((simde__m128i*)ctx.v11.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)ctx.v11.u32), 0xFF));
	// vmulfp128 v12,v13,v12
	ctx.fpscr.enableFlushMode();
	simde_mm_store_ps(ctx.v12.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v12.f32)));
	// vmulfp128 v13,v13,v11
	simde_mm_store_ps(ctx.v13.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v11.f32)));
	// lvx128 v11,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v11.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx128 v11,r31,r8
	ea = (ctx.r31.u32 + ctx.r8.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// vsubfp v12,v0,v12
	simde_mm_store_ps(ctx.v12.f32, simde_mm_sub_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v12.f32)));
	// vsubfp v13,v0,v13
	simde_mm_store_ps(ctx.v13.f32, simde_mm_sub_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v13.f32)));
	// stvx128 v12,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v12.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx128 v13,r0,r21
	ea = (ctx.r21.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// beq cr6,0x8253bb74
	if (ctx.cr6.eq) goto loc_8253BB74;
	// lwz r11,300(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 300);
	// lvx128 v13,r0,r29
	ea = (ctx.r29.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,128
	ctx.r10.s64 = ctx.r1.s64 + 128;
	// vsubfp v13,v13,v0
	simde_mm_store_ps(ctx.v13.f32, simde_mm_sub_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v0.f32)));
	// li r9,368
	ctx.r9.s64 = 368;
	// lwz r11,5224(r11)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r11.u32 + 5224);
	// lfs f0,12(r11)
	ctx.fpscr.disableFlushModeUnconditional();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,128(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 128, temp.u32);
	// lvlx v12,0,r10
	temp.u32 = ctx.r10.u32;
	simde_mm_store_si128((simde__m128i*)ctx.v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
	// vspltw v12,v12,0
	simde_mm_store_si128((simde__m128i*)ctx.v12.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)ctx.v12.u32), 0xFF));
	// vmaddfp v0,v13,v12,v0
	ctx.fpscr.enableFlushModeUnconditional();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v12.f32)), simde_mm_load_ps(ctx.v0.f32)));
	// stvx128 v0,r16,r9
	ea = (ctx.r16.u32 + ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
loc_8253BB74:
	// lwz r11,300(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 300);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x8253bc04
	if (ctx.cr6.eq) goto loc_8253BC04;
	// lbz r11,316(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 316);
	// rlwinm r11,r11,0,28,28
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x8;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8253bc04
	if (ctx.cr6.eq) goto loc_8253BC04;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// lbz r11,-13252(r11)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r11.u32 + -13252);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8253bc04
	if (ctx.cr6.eq) goto loc_8253BC04;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x825357f0
	ctx.lr = 0x8253BBAC;
	sub_825357F0(ctx, base);
	// lbz r11,556(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 556);
	// mr r4,r20
	ctx.r4.u64 = ctx.r20.u64;
	// lwz r3,300(r31)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r31.u32 + 300);
	// extsb r5,r11
	ctx.r5.s64 = ctx.r11.s8;
	// bl 0x825ef258
	ctx.lr = 0x8253BBC0;
	sub_825EF258(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// lwz r3,300(r31)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r31.u32 + 300);
	// bl 0x822bca90
	ctx.lr = 0x8253BBCC;
	sub_822BCA90(ctx, base);
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// lwz r3,300(r31)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r31.u32 + 300);
	// bl 0x825ef310
	ctx.lr = 0x8253BBD8;
	sub_825EF310(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// li r9,1
	ctx.r9.s64 = 1;
	// ld r8,784(r31)
	ctx.r8.u64 = REX_LOAD_U64(ctx.r31.u32 + 784);
	// mr r7,r30
	ctx.r7.u64 = ctx.r30.u64;
	// lwz r3,300(r31)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r31.u32 + 300);
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// lfs f1,3400(r23)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r23.u32 + 3400);
	ctx.f1.f64 = double(temp.f32);
	// addi r5,r1,144
	ctx.r5.s64 = ctx.r1.s64 + 144;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// stb r11,87(r1)
	REX_STORE_U8(ctx.r1.u32 + 87, ctx.r11.u8);
	// bl 0x825f0250
	ctx.lr = 0x8253BC04;
	sub_825F0250(ctx, base);
loc_8253BC04:
	// addi r4,r1,224
	ctx.r4.s64 = ctx.r1.s64 + 224;
	// addi r3,r31,16
	ctx.r3.s64 = ctx.r31.s64 + 16;
	// bl 0x82371000
	ctx.lr = 0x8253BC10;
	sub_82371000(ctx, base);
	// lbz r10,396(r31)
	ctx.r10.u64 = REX_LOAD_U8(ctx.r31.u32 + 396);
	// lbz r11,559(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 559);
	// clrlwi r9,r14,24
	ctx.r9.u64 = ctx.r14.u32 & 0xFF;
	// lbz r8,557(r31)
	ctx.r8.u64 = REX_LOAD_U8(ctx.r31.u32 + 557);
	// clrlwi r11,r11,26
	ctx.r11.u64 = ctx.r11.u32 & 0x3F;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// stb r8,558(r31)
	REX_STORE_U8(ctx.r31.u32 + 558, ctx.r8.u8);
	// stb r11,559(r31)
	REX_STORE_U8(ctx.r31.u32 + 559, ctx.r11.u8);
	// andi. r10,r10,253
	ctx.r10.u64 = ctx.r10.u64 & 253;
	ctx.cr0.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// stb r10,396(r31)
	REX_STORE_U8(ctx.r31.u32 + 396, ctx.r10.u8);
	// bne cr6,0x8253bc48
	if (!ctx.cr6.eq) goto loc_8253BC48;
	// lbz r11,-27512(r15)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r15.u32 + -27512);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8253bc50
	if (ctx.cr6.eq) goto loc_8253BC50;
loc_8253BC48:
	// lbz r11,120(r1)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r1.u32 + 120);
	// stb r11,556(r31)
	REX_STORE_U8(ctx.r31.u32 + 556, ctx.r11.u8);
loc_8253BC50:
	// li r3,1
	ctx.r3.s64 = 1;
loc_8253BC54:
	// addi r1,r1,576
	ctx.r1.s64 = ctx.r1.s64 + 576;
	// li r0,-288
	ctx.r0.s64 = -288;
	// lvx128 v127,r1,r0
	ea = (ctx.r1.u32 + ctx.r0.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v127.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r12,r1,-152
	ctx.r12.s64 = ctx.r1.s64 + -152;
	// bl 0x82a0134c
	ctx.lr = 0x8253BC68;
	__restfpr_18(ctx, base);
	// b 0x829ff7e0
	__restgprlr_14(ctx, base);
	return;
}

extern "C" void sub_82538948(PPCContext& ctx, uint8_t* base) {
  gta4::input::MouseRotation native_mouse{};
  uint32_t native_control = 0;
	REX_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7a4
	ctx.lr = 0x82538950;
	__savegprlr_19(ctx, base);
	// addi r12,r1,-112
	ctx.r12.s64 = ctx.r1.s64 + -112;
	// bl 0x82a01320
	ctx.lr = 0x82538958;
	__savefpr_26(ctx, base);
	// li r12,-192
	ctx.r12.s64 = -192;
	// stvx128 v126,r1,r12
	ea = (ctx.r1.u32 + ctx.r12.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v126.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r12,-176
	ctx.r12.s64 = -176;
	// stvx128 v127,r1,r12
	ea = (ctx.r1.u32 + ctx.r12.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v127.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stwu r1,-432(r1)
	ea = -432 + ctx.r1.u32;
	REX_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r30,368(r31)
	ctx.r30.u64 = REX_LOAD_U32(ctx.r31.u32 + 368);
	// lwz r11,32(r30)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r30.u32 + 32);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82538994
	if (!ctx.cr6.eq) goto loc_82538994;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x823b6118
	ctx.lr = 0x82538988;
	sub_823B6118(ctx, base);
	// addi r3,r30,16
	ctx.r3.s64 = ctx.r30.s64 + 16;
	// lwz r4,32(r30)
	ctx.r4.u64 = REX_LOAD_U32(ctx.r30.u32 + 32);
	// bl 0x823b3168
	ctx.lr = 0x82538994;
	sub_823B3168(ctx, base);
loc_82538994:
	// lis r11,-32076
	ctx.r11.s64 = -2102132736;
	// lhz r10,46(r30)
	ctx.r10.u64 = REX_LOAD_U16(ctx.r30.u32 + 46);
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// lwz r28,32(r30)
	ctx.r28.u64 = REX_LOAD_U32(ctx.r30.u32 + 32);
	// addi r27,r11,-24160
	ctx.r27.s64 = ctx.r11.s64 + -24160;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// extsh r10,r10
	ctx.r10.s64 = ctx.r10.s16;
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f26,2612(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 2612);
	ctx.f26.f64 = double(temp.f32);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stfs f26,112(r1)
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r1.u32 + 112, temp.u32);
	// lfs f0,18636(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 18636);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stfs f0,116(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 116, temp.u32);
	// lfs f0,18632(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 18632);
	ctx.f0.f64 = double(temp.f32);
	// lwzx r11,r10,r27
	ctx.r11.u64 = REX_LOAD_U32(ctx.r10.u32 + ctx.r27.u32);
	// stfs f0,120(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 120, temp.u32);
	// lwz r11,196(r11)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r11.u32 + 196);
	// lwz r29,4(r11)
	ctx.r29.u64 = REX_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x8217db60
	ctx.lr = 0x825389E4;
	sub_8217DB60(ctx, base);
	// rlwinm r9,r29,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 2) & 0xFFFFFFFC;
	// lwz r10,12(r3)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r3.u32 + 12);
	// addi r11,r1,112
	ctx.r11.s64 = ctx.r1.s64 + 112;
	// lvx128 v13,r0,r28
	ea = (ctx.r28.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// add r9,r29,r9
	ctx.r9.u64 = ctx.r29.u64 + ctx.r9.u64;
	// li r22,48
	ctx.r22.s64 = 48;
	// rlwinm r9,r9,4,0,27
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
	// addi r26,r28,32
	ctx.r26.s64 = ctx.r28.s64 + 32;
	// add r10,r10,r9
	ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r25,r28,48
	ctx.r25.s64 = ctx.r28.s64 + 48;
	// lwz r11,32(r30)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r30.u32 + 32);
	// addi r24,r28,16
	ctx.r24.s64 = ctx.r28.s64 + 16;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// lvx128 v12,r0,r26
	ea = (ctx.r26.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r23,-32256
	ctx.r23.s64 = -2113929216;
	// lvx128 v9,r10,r22
	ea = (ctx.r10.u32 + ctx.r22.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v9.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,112
	ctx.r10.s64 = ctx.r1.s64 + 112;
	// vaddfp v0,v9,v0
	ctx.fpscr.enableFlushMode();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_add_ps(simde_mm_load_ps(ctx.v9.f32), simde_mm_load_ps(ctx.v0.f32)));
	// lvx128 v11,r0,r25
	ea = (ctx.r25.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v11.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lvx128 v10,r0,r24
	ea = (ctx.r24.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v10.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// vmrghw v9,v13,v12
	simde_mm_store_si128((simde__m128i*)ctx.v9.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)ctx.v12.u32), simde_mm_load_si128((simde__m128i*)ctx.v13.u32)));
	// vmrghw v8,v10,v11
	simde_mm_store_si128((simde__m128i*)ctx.v8.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)ctx.v11.u32), simde_mm_load_si128((simde__m128i*)ctx.v10.u32)));
	// lvx128 v7,r11,r22
	ea = (ctx.r11.u32 + ctx.r22.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v7.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// vmrglw v13,v13,v12
	simde_mm_store_si128((simde__m128i*)ctx.v13.u32, simde_mm_unpacklo_epi32(simde_mm_load_si128((simde__m128i*)ctx.v12.u32), simde_mm_load_si128((simde__m128i*)ctx.v13.u32)));
	// li r19,32
	ctx.r19.s64 = 32;
	// vmrglw v11,v10,v11
	simde_mm_store_si128((simde__m128i*)ctx.v11.u32, simde_mm_unpacklo_epi32(simde_mm_load_si128((simde__m128i*)ctx.v11.u32), simde_mm_load_si128((simde__m128i*)ctx.v10.u32)));
	// vmrghw v12,v9,v8
	simde_mm_store_si128((simde__m128i*)ctx.v12.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)ctx.v8.u32), simde_mm_load_si128((simde__m128i*)ctx.v9.u32)));
	// vmrghw v13,v13,v11
	simde_mm_store_si128((simde__m128i*)ctx.v13.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)ctx.v11.u32), simde_mm_load_si128((simde__m128i*)ctx.v13.u32)));
	// vmrglw v11,v9,v8
	simde_mm_store_si128((simde__m128i*)ctx.v11.u32, simde_mm_unpacklo_epi32(simde_mm_load_si128((simde__m128i*)ctx.v8.u32), simde_mm_load_si128((simde__m128i*)ctx.v9.u32)));
	// vmsum3fp128 v13,v0,v13
	simde_mm_store_ps(ctx.v13.f32, simde_mm_dp_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v13.f32), 0xEF));
	// vor128 v127,v0,v0
	simde_mm_store_si128((simde__m128i*)ctx.v127.u8, simde_mm_load_si128((simde__m128i*)ctx.v0.u8));
	// vmsum3fp128 v12,v0,v12
	simde_mm_store_ps(ctx.v12.f32, simde_mm_dp_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v12.f32), 0xEF));
	// vmsum3fp128 v11,v0,v11
	simde_mm_store_ps(ctx.v11.f32, simde_mm_dp_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v11.f32), 0xEF));
	// stvx128 v127,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v127.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// vmrghw v0,v12,v13
	simde_mm_store_si128((simde__m128i*)ctx.v0.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)ctx.v13.u32), simde_mm_load_si128((simde__m128i*)ctx.v12.u32)));
	// vmrghw v13,v11,v0
	simde_mm_store_si128((simde__m128i*)ctx.v13.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)ctx.v0.u32), simde_mm_load_si128((simde__m128i*)ctx.v11.u32)));
	// vmrghw v0,v0,v13
	simde_mm_store_si128((simde__m128i*)ctx.v0.u32, simde_mm_unpackhi_epi32(simde_mm_load_si128((simde__m128i*)ctx.v13.u32), simde_mm_load_si128((simde__m128i*)ctx.v0.u32)));
	// vaddfp128 v126,v0,v7
	simde_mm_store_ps(ctx.v126.f32, simde_mm_add_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v7.f32)));
	// beq cr6,0x82538a94
	if (ctx.cr6.eq) goto loc_82538A94;
	// lvx128 v0,r11,r19
	ea = (ctx.r11.u32 + ctx.r19.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,128
	ctx.r11.s64 = ctx.r1.s64 + 128;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// b 0x82538aa4
	goto loc_82538AA4;
loc_82538A94:
	// lfs f0,3400(r23)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r23.u32 + 3400);
	ctx.f0.f64 = double(temp.f32);
	// stfs f26,128(r1)
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r1.u32 + 128, temp.u32);
	// stfs f26,132(r1)
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r1.u32 + 132, temp.u32);
	// stfs f0,136(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 136, temp.u32);
loc_82538AA4:
	// lwz r11,32(r30)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r30.u32 + 32);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82538ac4
	if (ctx.cr6.eq) goto loc_82538AC4;
	// li r10,16
	ctx.r10.s64 = 16;
	// lvx128 v13,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,96
	ctx.r11.s64 = ctx.r1.s64 + 96;
	// stvx128 v13,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// b 0x82538ad8
	goto loc_82538AD8;
loc_82538AC4:
	// addi r4,r30,16
	ctx.r4.s64 = ctx.r30.s64 + 16;
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// bl 0x822bfa98
	ctx.lr = 0x82538AD0;
	sub_822BFA98(ctx, base);
	// addi r11,r1,96
	ctx.r11.s64 = ctx.r1.s64 + 96;
	// lvx128 v13,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
loc_82538AD8:
	// lwz r11,32(r30)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r30.u32 + 32);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82538af8
	if (!ctx.cr6.eq) goto loc_82538AF8;
	// addi r4,r30,16
	ctx.r4.s64 = ctx.r30.s64 + 16;
	// addi r3,r1,224
	ctx.r3.s64 = ctx.r1.s64 + 224;
	// bl 0x82451928
	ctx.lr = 0x82538AF0;
	sub_82451928(ctx, base);
	// addi r11,r1,96
	ctx.r11.s64 = ctx.r1.s64 + 96;
	// lvx128 v13,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
loc_82538AF8:
	// vmsum3fp128 v0,v13,v13
	ctx.fpscr.enableFlushMode();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_dp_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v13.f32), 0xEF));
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// vspltisw v9,0
	simde_mm_store_si128((simde__m128i*)ctx.v9.u32, simde_mm_set1_epi32(int(0x0)));
	// lwz r10,4836(r30)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r30.u32 + 4836);
	// addi r11,r11,-18560
	ctx.r11.s64 = ctx.r11.s64 + -18560;
	// fmr f1,f26
	ctx.fpscr.disableFlushModeUnconditional();
	ctx.f1.f64 = ctx.f26.f64;
	// addi r10,r10,-4
	ctx.r10.s64 = ctx.r10.s64 + -4;
	// addi r6,r1,84
	ctx.r6.s64 = ctx.r1.s64 + 84;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// lvx128 v12,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r11,r11,29104
	ctx.r11.s64 = ctx.r11.s64 + 29104;
	// lvx128 v11,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v11.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-32080
	ctx.r11.s64 = -2102394880;
	// vor v8,v0,v0
	simde_mm_store_si128((simde__m128i*)ctx.v8.u8, simde_mm_load_si128((simde__m128i*)ctx.v0.u8));
	// vrsqrtefp v0,v0
	ctx.fpscr.enableFlushModeUnconditional();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(simde_mm_load_ps(ctx.v0.f32))));
	// addi r11,r11,29088
	ctx.r11.s64 = ctx.r11.s64 + 29088;
	// lvx128 v10,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v10.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// cntlzw r11,r10
	ctx.r11.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// clrlwi r21,r11,24
	ctx.r21.u64 = ctx.r11.u32 & 0xFF;
	// addi r11,r1,96
	ctx.r11.s64 = ctx.r1.s64 + 96;
	// vor v7,v0,v0
	simde_mm_store_si128((simde__m128i*)ctx.v7.u8, simde_mm_load_si128((simde__m128i*)ctx.v0.u8));
	// vcmpeqfp v0,v0,v12
	simde_mm_store_ps(ctx.v0.f32, simde_mm_cmpeq_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v12.f32)));
	// vsel v0,v7,v9,v0
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)ctx.v7.u8)), simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)ctx.v9.u8))));
	// vmulfp128 v12,v0,v0
	simde_mm_store_ps(ctx.v12.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v0.f32)));
	// vmulfp128 v11,v11,v0
	simde_mm_store_ps(ctx.v11.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v11.f32), simde_mm_load_ps(ctx.v0.f32)));
	// vnmsubfp v12,v8,v12,v10
	simde_mm_store_ps(ctx.v12.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(ctx.v8.f32), simde_mm_load_ps(ctx.v12.f32)), simde_mm_load_ps(ctx.v10.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
	// vmaddfp v0,v12,v11,v0
	simde_mm_store_ps(ctx.v0.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(ctx.v12.f32), simde_mm_load_ps(ctx.v11.f32)), simde_mm_load_ps(ctx.v0.f32)));
	// vmulfp128 v0,v13,v0
	simde_mm_store_ps(ctx.v0.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v0.f32)));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x825370c0
	ctx.lr = 0x82538B80;
	sub_825370C0(ctx, base);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lbz r10,396(r31)
	ctx.r10.u64 = REX_LOAD_U8(ctx.r31.u32 + 396);
	// lfs f12,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 80);
	ctx.f12.f64 = double(temp.f32);
	// li r20,1
	ctx.r20.s64 = 1;
	// rlwinm r10,r10,0,29,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0x4;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// lfs f0,3444(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 3444);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f13,18580(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 18580);
	ctx.f13.f64 = double(temp.f32);
	// stfs f13,80(r1)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// lfs f13,116(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + 116);
	ctx.f13.f64 = double(temp.f32);
	// fmsubs f0,f12,f0,f13
	ctx.f0.f64 = double(float(std::fma(ctx.f12.f64, ctx.f0.f64, -ctx.f13.f64)));
	// stfs f0,84(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// beq cr6,0x82538bc8
	if (ctx.cr6.eq) goto loc_82538BC8;
	// lbz r11,557(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 557);
	// cmplwi cr6,r11,3
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 3, ctx.xer);
	// mr r11,r20
	ctx.r11.u64 = ctx.r20.u64;
	// beq cr6,0x82538bcc
	if (ctx.cr6.eq) goto loc_82538BCC;
loc_82538BC8:
	// li r11,0
	ctx.r11.s64 = 0;
loc_82538BCC:
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// lbz r11,824(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 824);
	// beq cr6,0x82538c08
	if (ctx.cr6.eq) goto loc_82538C08;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82538bf8
	if (!ctx.cr6.eq) goto loc_82538BF8;
	// stfs f26,412(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r31.u32 + 412, temp.u32);
	// stb r20,824(r31)
	REX_STORE_U8(ctx.r31.u32 + 824, ctx.r20.u8);
	// stfs f26,408(r31)
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r31.u32 + 408, temp.u32);
	// stfs f26,432(r31)
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r31.u32 + 432, temp.u32);
	// stfs f26,436(r31)
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r31.u32 + 436, temp.u32);
loc_82538BF8:
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f0,18592(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 18592);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// b 0x82538c28
	goto loc_82538C28;
loc_82538C08:
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82538c28
	if (ctx.cr6.eq) goto loc_82538C28;
	// li r11,0
	ctx.r11.s64 = 0;
	// stfs f26,412(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r31.u32 + 412, temp.u32);
	// stfs f26,408(r31)
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r31.u32 + 408, temp.u32);
	// stfs f26,432(r31)
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r31.u32 + 432, temp.u32);
	// stfs f26,436(r31)
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r31.u32 + 436, temp.u32);
	// stb r11,824(r31)
	REX_STORE_U8(ctx.r31.u32 + 824, ctx.r11.u8);
loc_82538C28:
	// addi r10,r1,80
	ctx.r10.s64 = ctx.r1.s64 + 80;
	// addi r11,r1,96
	ctx.r11.s64 = ctx.r1.s64 + 96;
	// addi r9,r1,84
	ctx.r9.s64 = ctx.r1.s64 + 84;
	// lvlx v13,0,r10
	temp.u32 = ctx.r10.u32;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
	// addi r10,r1,144
	ctx.r10.s64 = ctx.r1.s64 + 144;
	// vspltw v13,v13,0
	simde_mm_store_si128((simde__m128i*)ctx.v13.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)ctx.v13.u32), 0xFF));
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lvlx v12,0,r9
	temp.u32 = ctx.r9.u32;
	simde_mm_store_si128((simde__m128i*)ctx.v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
	// lwz r11,368(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 368);
	// vspltw v12,v12,0
	simde_mm_store_si128((simde__m128i*)ctx.v12.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)ctx.v12.u32), 0xFF));
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// vmulfp128 v0,v0,v13
	ctx.fpscr.enableFlushMode();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v13.f32)));
	// vmulfp128 v0,v0,v12
	simde_mm_store_ps(ctx.v0.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v12.f32)));
	// vaddfp128 v13,v127,v0
	simde_mm_store_ps(ctx.v13.f32, simde_mm_add_ps(simde_mm_load_ps(ctx.v127.f32), simde_mm_load_ps(ctx.v0.f32)));
	// vaddfp128 v127,v126,v0
	simde_mm_store_ps(ctx.v127.f32, simde_mm_add_ps(simde_mm_load_ps(ctx.v126.f32), simde_mm_load_ps(ctx.v0.f32)));
	// stvx128 v13,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// beq cr6,0x82538d5c
	if (ctx.cr6.eq) goto loc_82538D5C;
	// lhz r11,46(r11)
	ctx.r11.u64 = REX_LOAD_U16(ctx.r11.u32 + 46);
	// extsh r11,r11
	ctx.r11.s64 = ctx.r11.s16;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r11,r11,r27
	ctx.r11.u64 = REX_LOAD_U32(ctx.r11.u32 + ctx.r27.u32);
	// lhz r11,964(r11)
	ctx.r11.u64 = REX_LOAD_U16(ctx.r11.u32 + 964);
	// extsh r11,r11
	ctx.r11.s64 = ctx.r11.s16;
	// cmpwi cr6,r11,-1
	ctx.cr6.compare<int32_t>(ctx.r11.s32, -1, ctx.xer);
	// beq cr6,0x82538d5c
	if (ctx.cr6.eq) goto loc_82538D5C;
	// rlwinm r9,r11,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lis r10,-31996
	ctx.r10.s64 = -2096889856;
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// addi r10,r10,-27408
	ctx.r10.s64 = ctx.r10.s64 + -27408;
	// rlwinm r11,r11,7,0,24
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 7) & 0xFFFFFF80;
	// add r3,r11,r10
	ctx.r3.u64 = ctx.r11.u64 + ctx.r10.u64;
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x82538d5c
	if (ctx.cr6.eq) goto loc_82538D5C;
	// lbz r11,557(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 557);
	// extsb r11,r11
	ctx.r11.s64 = ctx.r11.s8;
	// cmplwi cr6,r11,3
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 3, ctx.xer);
	// bgt cr6,0x82538d0c
	if (ctx.cr6.gt) goto loc_82538D0C;
	// lis r12,-32172
	ctx.r12.s64 = -2108424192;
	// addi r12,r12,-29484
	ctx.r12.s64 = ctx.r12.s64 + -29484;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = REX_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_82538D08;
	case 1:
		goto loc_82538CF8;
	case 2:
		goto loc_82538D00;
	case 3:
		goto loc_82538CE4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_82538CE4:
	// lfs f0,40(r28)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r28.u32 + 40);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f0,f26
	ctx.cr6.compare(ctx.f0.f64, ctx.f26.f64);
	// ble cr6,0x82538d08
	if (!ctx.cr6.gt) goto loc_82538D08;
	// addi r3,r3,64
	ctx.r3.s64 = ctx.r3.s64 + 64;
	// b 0x82538d0c
	goto loc_82538D0C;
loc_82538CF8:
	// addi r3,r3,320
	ctx.r3.s64 = ctx.r3.s64 + 320;
	// b 0x82538d0c
	goto loc_82538D0C;
loc_82538D00:
	// addi r3,r3,352
	ctx.r3.s64 = ctx.r3.s64 + 352;
	// b 0x82538d0c
	goto loc_82538D0C;
loc_82538D08:
	// addi r3,r3,32
	ctx.r3.s64 = ctx.r3.s64 + 32;
loc_82538D0C:
	// li r6,1
	ctx.r6.s64 = 1;
	// addi r5,r1,160
	ctx.r5.s64 = ctx.r1.s64 + 160;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// bl 0x82536f50
	ctx.lr = 0x82538D1C;
	sub_82536F50(ctx, base);
	// addi r10,r1,176
	ctx.r10.s64 = ctx.r1.s64 + 176;
	// addi r11,r1,96
	ctx.r11.s64 = ctx.r1.s64 + 96;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,192
	ctx.r10.s64 = ctx.r1.s64 + 192;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,128
	ctx.r11.s64 = ctx.r1.s64 + 128;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,208
	ctx.r10.s64 = ctx.r1.s64 + 208;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lwz r11,32(r30)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r30.u32 + 32);
	// lvx128 v127,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v127.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lvx128 v0,r11,r22
	ea = (ctx.r11.u32 + ctx.r22.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,144
	ctx.r11.s64 = ctx.r1.s64 + 144;
	// vsubfp128 v0,v127,v0
	ctx.fpscr.enableFlushMode();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_sub_ps(simde_mm_load_ps(ctx.v127.f32), simde_mm_load_ps(ctx.v0.f32)));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// b 0x82538d8c
	goto loc_82538D8C;
loc_82538D5C:
	// addi r11,r1,160
	ctx.r11.s64 = ctx.r1.s64 + 160;
	// lvx128 v0,r0,r28
	ea = (ctx.r28.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lvx128 v13,r0,r24
	ea = (ctx.r24.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,176
	ctx.r11.s64 = ctx.r1.s64 + 176;
	// lvx128 v0,r0,r26
	ea = (ctx.r26.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx128 v13,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,192
	ctx.r11.s64 = ctx.r1.s64 + 192;
	// lvx128 v13,r0,r25
	ea = (ctx.r25.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,208
	ctx.r11.s64 = ctx.r1.s64 + 208;
	// stvx128 v13,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
loc_82538D8C:
	// addi r4,r1,112
	ctx.r4.s64 = ctx.r1.s64 + 112;
	// addi r3,r1,160
	ctx.r3.s64 = ctx.r1.s64 + 160;
	// bl 0x82535300
	ctx.lr = 0x82538D98;
	sub_82535300(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// li r3,0
	ctx.r3.s64 = 0;
	// bl 0x82369a08
	ctx.lr = 0x82538DA4;
	sub_82369A08(ctx, base);
native_control = ctx.r3.u32;
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// li r4,1
	ctx.r4.s64 = 1;
	// fmr f31,f26
	ctx.fpscr.disableFlushMode();
	ctx.f31.f64 = ctx.f26.f64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// fmr f27,f26
	ctx.f27.f64 = ctx.f26.f64;
	// bl 0x82536ba0
	ctx.lr = 0x82538DBC;
	sub_82536BA0(ctx, base);
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// cmplwi cr6,r29,0
	ctx.cr6.compare<uint32_t>(ctx.r29.u32, 0, ctx.xer);
	// beq cr6,0x82538e00
	if (ctx.cr6.eq) goto loc_82538E00;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x822b81d0
	ctx.lr = 0x82538DD0;
	sub_822B81D0(ctx, base);
	// lwz r11,8(r3)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r3.u32 + 8);
	// ld r3,0(r3)
	ctx.r3.u64 = REX_LOAD_U64(ctx.r3.u32 + 0);
	// rldicr r4,r11,32,63
	ctx.r4.u64 = __builtin_rotateleft64(ctx.r11.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// bl 0x822b7a50
	ctx.lr = 0x82538DE0;
	sub_822B7A50(ctx, base);
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// fneg f31,f1
	ctx.fpscr.disableFlushMode();
	ctx.f31.u64 = ctx.f1.u64 ^ 0x8000000000000000;
	// bl 0x822b8228
	ctx.lr = 0x82538DEC;
	sub_822B8228(ctx, base);
	// lwz r11,8(r3)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r3.u32 + 8);
	// ld r3,0(r3)
	ctx.r3.u64 = REX_LOAD_U64(ctx.r3.u32 + 0);
	// rldicr r4,r11,32,63
	ctx.r4.u64 = __builtin_rotateleft64(ctx.r11.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// bl 0x822b7ae0
	ctx.lr = 0x82538DFC;
	sub_822B7AE0(ctx, base);
	// fneg f27,f1
	ctx.fpscr.disableFlushMode();
	ctx.f27.u64 = ctx.f1.u64 ^ 0x8000000000000000;
loc_82538E00:
	// fmuls f0,f31,f31
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(float(ctx.f31.f64 * ctx.f31.f64));
	// lfs f13,3400(r23)
	temp.u32 = REX_LOAD_U32(ctx.r23.u32 + 3400);
	ctx.f13.f64 = double(temp.f32);
	// fmadds f1,f27,f27,f0
	ctx.f1.f64 = double(float(std::fma(ctx.f27.f64, ctx.f27.f64, ctx.f0.f64)));
	// fcmpu cr6,f1,f26
	ctx.cr6.compare(ctx.f1.f64, ctx.f26.f64);
	// ble cr6,0x82538e24
	if (!ctx.cr6.gt) goto loc_82538E24;
	// fsqrts f1,f1
	ctx.f1.f64 = double(float(sqrt(ctx.f1.f64)));
	// fdivs f0,f13,f1
	ctx.f0.f64 = double(float(ctx.f13.f64 / ctx.f1.f64));
	// fmuls f31,f0,f31
	ctx.f31.f64 = double(float(ctx.f0.f64 * ctx.f31.f64));
	// fmuls f27,f0,f27
	ctx.f27.f64 = double(float(ctx.f0.f64 * ctx.f27.f64));
loc_82538E24:
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// lfs f0,-18880(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -18880);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// fcmpu cr6,f1,f0
	ctx.cr6.compare(ctx.f1.f64, ctx.f0.f64);
	// addi r27,r11,-24840
	ctx.r27.s64 = ctx.r11.s64 + -24840;
	// lis r11,-32057
	ctx.r11.s64 = -2100887552;
	// addi r28,r11,-15708
	ctx.r28.s64 = ctx.r11.s64 + -15708;
	// ble cr6,0x82538e84
	if (!ctx.cr6.gt) goto loc_82538E84;
	// lwz r10,36(r27)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r27.u32 + 36);
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// lfs f0,8(r28)
	temp.u32 = REX_LOAD_U32(ctx.r28.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// addi r11,r11,-18892
	ctx.r11.s64 = ctx.r11.s64 + -18892;
	// rlwinm r9,r10,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lis r10,-32085
	ctx.r10.s64 = -2102722560;
	// lfsx f13,r9,r11
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + ctx.r11.u32);
	ctx.f13.f64 = double(temp.f32);
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// lfs f12,-18896(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + -18896);
	ctx.f12.f64 = double(temp.f32);
	// addi r29,r11,-18900
	ctx.r29.s64 = ctx.r11.s64 + -18900;
	// lfs f11,0(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// fmadds f0,f0,f12,f11
	ctx.f0.f64 = double(float(std::fma(ctx.f0.f64, ctx.f12.f64, ctx.f11.f64)));
	// stfs f0,0(r29)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r29.u32 + 0, temp.u32);
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// ble cr6,0x82538e90
	if (!ctx.cr6.gt) goto loc_82538E90;
	// b 0x82538e8c
	goto loc_82538E8C;
loc_82538E84:
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// addi r29,r11,-18900
	ctx.r29.s64 = ctx.r11.s64 + -18900;
loc_82538E8C:
	// stfs f13,0(r29)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r29.u32 + 0, temp.u32);
loc_82538E90:
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// lfs f2,-18904(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -18904);
	ctx.f2.f64 = double(temp.f32);
	// bl 0x82a02648
	ctx.lr = 0x82538E9C;
	sub_82A02648(ctx, base);
	// lfs f0,3400(r23)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r23.u32 + 3400);
	ctx.f0.f64 = double(temp.f32);
	// lwz r10,36(r27)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r27.u32 + 36);
	// lfs f13,0(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// fsubs f12,f0,f13
	ctx.f12.f64 = double(float(ctx.f0.f64 - ctx.f13.f64));
	// rlwinm r9,r10,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r11,r11,-18916
	ctx.r11.s64 = ctx.r11.s64 + -18916;
	// fabs f9,f31
	ctx.f9.u64 = ctx.f31.u64 & ~0x8000000000000000;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// lfsx f10,r9,r11
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + ctx.r11.u32);
	ctx.f10.f64 = double(temp.f32);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f11,-13772(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + -13772);
	ctx.f11.f64 = double(temp.f32);
	// fsel f13,f12,f0,f13
	ctx.f13.f64 = ctx.f12.f64 >= 0.0 ? ctx.f0.f64 : ctx.f13.f64;
	// frsp f0,f1
	ctx.f0.f64 = double(float(ctx.f1.f64));
	// fmuls f12,f13,f10
	ctx.f12.f64 = double(float(ctx.f13.f64 * ctx.f10.f64));
	// lfs f13,17772(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 17772);
	ctx.f13.f64 = double(temp.f32);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// fcmpu cr6,f9,f13
	ctx.cr6.compare(ctx.f9.f64, ctx.f13.f64);
	// fmuls f0,f12,f0
	ctx.f0.f64 = double(float(ctx.f12.f64 * ctx.f0.f64));
	// fmuls f12,f0,f31
	ctx.f12.f64 = double(float(ctx.f0.f64 * ctx.f31.f64));
	// fmuls f10,f0,f27
	ctx.f10.f64 = double(float(ctx.f0.f64 * ctx.f27.f64));
	// lfs f0,-13760(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -13760);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// fmuls f29,f12,f0
	ctx.f29.f64 = double(float(ctx.f12.f64 * ctx.f0.f64));
	// lfs f0,-13764(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -13764);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f28,f10,f0
	ctx.f28.f64 = double(float(ctx.f10.f64 * ctx.f0.f64));
	// bge cr6,0x82538f1c
	if (!ctx.cr6.lt) goto loc_82538F1C;
	// fabs f0,f27
	ctx.f0.u64 = ctx.f27.u64 & ~0x8000000000000000;
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// bge cr6,0x82538f1c
	if (!ctx.cr6.lt) goto loc_82538F1C;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// lfs f11,-13768(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -13768);
	ctx.f11.f64 = double(temp.f32);
loc_82538F1C:
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,8(r28)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r28.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// fmr f1,f11
	ctx.f1.f64 = ctx.f11.f64;
	// lfs f30,2604(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 2604);
	ctx.f30.f64 = double(temp.f32);
	// fmuls f2,f0,f30
	ctx.f2.f64 = double(float(ctx.f0.f64 * ctx.f30.f64));
	// bl 0x82a02648
	ctx.lr = 0x82538F34;
	sub_82A02648(ctx, base);
	// frsp f0,f1
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(float(ctx.f1.f64));
	// lfs f12,3400(r23)
	temp.u32 = REX_LOAD_U32(ctx.r23.u32 + 3400);
	ctx.f12.f64 = double(temp.f32);
	// lfs f10,412(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 412);
	ctx.f10.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f11,408(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 408);
	ctx.f11.f64 = double(temp.f32);
	// lfs f9,432(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 432);
	ctx.f9.f64 = double(temp.f32);
	// lfs f8,436(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 436);
	ctx.f8.f64 = double(temp.f32);
	// lfs f13,9096(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 9096);
	ctx.f13.f64 = double(temp.f32);
	// fsubs f12,f12,f0
	ctx.f12.f64 = double(float(ctx.f12.f64 - ctx.f0.f64));
	// fmuls f10,f10,f0
	ctx.f10.f64 = double(float(ctx.f10.f64 * ctx.f0.f64));
	// fmuls f7,f12,f29
	ctx.f7.f64 = double(float(ctx.f12.f64 * ctx.f29.f64));
	// fmadds f12,f12,f28,f10
	ctx.f12.f64 = double(float(std::fma(ctx.f12.f64, ctx.f28.f64, ctx.f10.f64)));
	// stfs f12,412(r31)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r31.u32 + 412, temp.u32);
	// fmadds f0,f11,f0,f7
	ctx.f0.f64 = double(float(std::fma(ctx.f11.f64, ctx.f0.f64, ctx.f7.f64)));
	// stfs f0,408(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 408, temp.u32);
	// fmr f11,f12
	ctx.f11.f64 = ctx.f12.f64;
native_mouse = gta4::input::TakeMouseRotation(base, native_control, ctx.r31.u32, gta4::input::LoadFloat(base, ctx.r31.u32 + 96), false);
	// lfs f12,8(r28)
	temp.u32 = REX_LOAD_U32(ctx.r28.u32 + 8);
	ctx.f12.f64 = double(temp.f32);
	// fmuls f12,f11,f12
	ctx.f12.f64 = double(float(ctx.f11.f64 * ctx.f12.f64));
	// fmadds f12,f12,f13,f9
	ctx.f12.f64 = double(float(std::fma(ctx.f12.f64, ctx.f13.f64, ctx.f9.f64)));
ctx.f12.f64 += native_mouse.pitch;
	// stfs f12,432(r31)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r31.u32 + 432, temp.u32);
	// lfs f12,8(r28)
	temp.u32 = REX_LOAD_U32(ctx.r28.u32 + 8);
	ctx.f12.f64 = double(temp.f32);
	// fmuls f0,f0,f12
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f12.f64));
	// fmadds f1,f0,f13,f8
	ctx.f1.f64 = double(float(std::fma(ctx.f0.f64, ctx.f13.f64, ctx.f8.f64)));
native_mouse.yaw = gta4::input::ConstrainMousePitch(ctx.f1.f64, native_mouse.yaw, gta4::input::LoadFloat(base, 0x82024A08), gta4::input::LoadFloat(base, 0x82024A04));
ctx.f1.f64 += native_mouse.yaw;
	// stfs f1,436(r31)
	temp.f32 = float(ctx.f1.f64);
	REX_STORE_U32(ctx.r31.u32 + 436, temp.u32);
	// bl 0x82369828
	ctx.lr = 0x82538F98;
	sub_82369828(ctx, base);
if (native_mouse.moving() || gta4::input::MouseHoldingAim(base, native_control)) {
  gta4::input::StoreU32(base, ctx.r31.u32 + 436, std::bit_cast<uint32_t>(float(ctx.f1.f64)));
  goto loc_82539028;
}
	// fabs f13,f31
	ctx.fpscr.disableFlushMode();
	ctx.f13.u64 = ctx.f31.u64 & ~0x8000000000000000;
	// lis r29,-32254
	ctx.r29.s64 = -2113798144;
	// stfs f1,436(r31)
	temp.f32 = float(ctx.f1.f64);
	REX_STORE_U32(ctx.r31.u32 + 436, temp.u32);
	// lfs f0,-22848(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + -22848);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f13,f0
	ctx.cr6.compare(ctx.f13.f64, ctx.f0.f64);
	// bge cr6,0x82539028
	if (!ctx.cr6.lt) goto loc_82539028;
	// fabs f13,f27
	ctx.f13.u64 = ctx.f27.u64 & ~0x8000000000000000;
	// fcmpu cr6,f13,f0
	ctx.cr6.compare(ctx.f13.f64, ctx.f0.f64);
	// bge cr6,0x82539028
	if (!ctx.cr6.lt) goto loc_82539028;
	// lwz r11,0(r30)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r30.u32 + 0);
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// addi r3,r1,224
	ctx.r3.s64 = ctx.r1.s64 + 224;
	// lwz r11,232(r11)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r11.u32 + 232);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x82538FD4;
	REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// lfs f13,4(r3)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// fmuls f13,f13,f13
	ctx.f13.f64 = double(float(ctx.f13.f64 * ctx.f13.f64));
	// lfs f0,0(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f12,8(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 8);
	ctx.f12.f64 = double(temp.f32);
	// fmadds f0,f0,f0,f13
	ctx.f0.f64 = double(float(std::fma(ctx.f0.f64, ctx.f0.f64, ctx.f13.f64)));
	// fmadds f0,f12,f12,f0
	ctx.f0.f64 = double(float(std::fma(ctx.f12.f64, ctx.f12.f64, ctx.f0.f64)));
	// fsqrts f13,f0
	ctx.f13.f64 = double(float(sqrt(ctx.f0.f64)));
	// lfs f0,-22848(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + -22848);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f13,f0
	ctx.cr6.compare(ctx.f13.f64, ctx.f0.f64);
	// ble cr6,0x82539028
	if (!ctx.cr6.gt) goto loc_82539028;
	// clrlwi r11,r26,24
	ctx.r11.u64 = ctx.r26.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82539028
	if (!ctx.cr6.eq) goto loc_82539028;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f13,432(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 432);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,436(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 436);
	ctx.f12.f64 = double(temp.f32);
	// lfs f0,18876(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 18876);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f13,f13,f0
	ctx.f13.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
	// stfs f13,432(r31)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r31.u32 + 432, temp.u32);
	// fmuls f0,f12,f0
	ctx.f0.f64 = double(float(ctx.f12.f64 * ctx.f0.f64));
	// stfs f0,436(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 436, temp.u32);
loc_82539028:
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f0,432(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 432);
	ctx.f0.f64 = double(temp.f32);
	// lfs f1,3400(r23)
	temp.u32 = REX_LOAD_U32(ctx.r23.u32 + 3400);
	ctx.f1.f64 = double(temp.f32);
	// addi r6,r1,112
	ctx.r6.s64 = ctx.r1.s64 + 112;
	// fsubs f11,f0,f1
	ctx.f11.f64 = double(float(ctx.f0.f64 - ctx.f1.f64));
	// lfs f13,436(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 436);
	ctx.f13.f64 = double(temp.f32);
	// addi r5,r1,144
	ctx.r5.s64 = ctx.r1.s64 + 144;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// lfs f12,18948(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 18948);
	ctx.f12.f64 = double(temp.f32);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// fsubs f10,f13,f12
	ctx.f10.f64 = double(float(ctx.f13.f64 - ctx.f12.f64));
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// fsel f0,f11,f1,f0
	ctx.f0.f64 = ctx.f11.f64 >= 0.0 ? ctx.f1.f64 : ctx.f0.f64;
	// fsel f13,f10,f12,f13
	ctx.f13.f64 = ctx.f10.f64 >= 0.0 ? ctx.f12.f64 : ctx.f13.f64;
	// lfs f12,18944(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 18944);
	ctx.f12.f64 = double(temp.f32);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// fsubs f10,f0,f12
	ctx.f10.f64 = double(float(ctx.f0.f64 - ctx.f12.f64));
	// lfs f11,18952(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 18952);
	ctx.f11.f64 = double(temp.f32);
	// fsubs f11,f13,f11
	ctx.f11.f64 = double(float(ctx.f13.f64 - ctx.f11.f64));
	// fsel f0,f10,f0,f12
	ctx.f0.f64 = ctx.f10.f64 >= 0.0 ? ctx.f0.f64 : ctx.f12.f64;
	// stfs f0,432(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 432, temp.u32);
	// lfs f0,18952(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 18952);
	ctx.f0.f64 = double(temp.f32);
	// fsel f0,f11,f13,f0
	ctx.f0.f64 = ctx.f11.f64 >= 0.0 ? ctx.f13.f64 : ctx.f0.f64;
	// stfs f0,436(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 436, temp.u32);
	// bl 0x825386f0
	ctx.lr = 0x8253908C;
	sub_825386F0(ctx, base);
	// addi r29,r31,16
	ctx.r29.s64 = ctx.r31.s64 + 16;
	// addi r4,r1,112
	ctx.r4.s64 = ctx.r1.s64 + 112;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x82225208
	ctx.lr = 0x8253909C;
	sub_82225208(ctx, base);
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// lfs f1,436(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 436);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x822a5e28
	ctx.lr = 0x825390A8;
	sub_822A5E28(ctx, base);
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// lfs f1,432(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 432);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x82233558
	ctx.lr = 0x825390B4;
	sub_82233558(ctx, base);
	// addi r11,r1,96
	ctx.r11.s64 = ctx.r1.s64 + 96;
	// lvx128 v0,r31,r19
	ea = (ctx.r31.u32 + ctx.r19.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r5,r1,128
	ctx.r5.s64 = ctx.r1.s64 + 128;
	// addi r4,r1,96
	ctx.r4.s64 = ctx.r1.s64 + 96;
	// lvx128 v13,r31,r22
	ea = (ctx.r31.u32 + ctx.r22.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,128
	ctx.r11.s64 = ctx.r1.s64 + 128;
	// stvx128 v13,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x82371078
	ctx.lr = 0x825390DC;
	sub_82371078(ctx, base);
	// addi r3,r31,64
	ctx.r3.s64 = ctx.r31.s64 + 64;
	// li r4,4
	ctx.r4.s64 = 4;
	// stvx128 v127,r0,r3
	ea = (ctx.r3.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v127.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x823696e8
	ctx.lr = 0x825390EC;
	sub_823696E8(ctx, base);
	// addi r5,r31,404
	ctx.r5.s64 = ctx.r31.s64 + 404;
	// addi r4,r31,400
	ctx.r4.s64 = ctx.r31.s64 + 400;
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// bl 0x8236bfa0
	ctx.lr = 0x825390FC;
	sub_8236BFA0(ctx, base);
	// lfs f0,8(r28)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r28.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// fmuls f1,f0,f30
	ctx.f1.f64 = double(float(ctx.f0.f64 * ctx.f30.f64));
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82537670
	ctx.lr = 0x82539110;
	sub_82537670(ctx, base);
	// lbz r11,397(r31)
	ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 397);
	// rlwinm r10,r11,0,0,24
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0xFFFFFF80;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// bne cr6,0x82539130
	if (!ctx.cr6.eq) goto loc_82539130;
	// lbz r10,317(r31)
	ctx.r10.u64 = REX_LOAD_U8(ctx.r31.u32 + 317);
	// stfs f26,116(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r31.u32 + 116, temp.u32);
	// andi. r10,r10,191
	ctx.r10.u64 = ctx.r10.u64 & 191;
	ctx.cr0.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// stb r10,317(r31)
	REX_STORE_U8(ctx.r31.u32 + 317, ctx.r10.u8);
loc_82539130:
	// rlwimi r11,r20,7,0,24
	ctx.r11.u64 = (__builtin_rotateleft64(ctx.r20.u32 | (ctx.r20.u64 << 32), 7) & 0xFFFFFF80) | (ctx.r11.u64 & 0xFFFFFFFF0000007F);
	// clrlwi r10,r21,24
	ctx.r10.u64 = ctx.r21.u32 & 0xFF;
	// li r3,1
	ctx.r3.s64 = 1;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stb r11,397(r31)
	REX_STORE_U8(ctx.r31.u32 + 397, ctx.r11.u8);
	// beq cr6,0x82539154
	if (ctx.cr6.eq) goto loc_82539154;
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// lfs f0,-24960(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -24960);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,100(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 100, temp.u32);
loc_82539154:
	// addi r1,r1,432
	ctx.r1.s64 = ctx.r1.s64 + 432;
	// li r0,-192
	ctx.r0.s64 = -192;
	// lvx128 v126,r1,r0
	ea = (ctx.r1.u32 + ctx.r0.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v126.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r0,-176
	ctx.r0.s64 = -176;
	// lvx128 v127,r1,r0
	ea = (ctx.r1.u32 + ctx.r0.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v127.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r12,r1,-112
	ctx.r12.s64 = ctx.r1.s64 + -112;
	// bl 0x82a0136c
	ctx.lr = 0x82539170;
	__restfpr_26(ctx, base);
	// b 0x829ff7f4
	__restgprlr_19(ctx, base);
	return;
}
