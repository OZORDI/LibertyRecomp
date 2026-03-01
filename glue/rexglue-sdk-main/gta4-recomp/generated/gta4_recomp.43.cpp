#include "gta4_init.h"

PPC_FUNC_IMPL(__imp__sub_826E9FD0);  /* forward decl */
PPC_WEAK_FUNC(sub_826E9FD0) { __imp__sub_826E9FD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826E9FD0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826ea074
	if (ctx.cr6.gt) goto loc_826EA074;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826ea06c
	if (ctx.cr6.eq) goto loc_826EA06C;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,7
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 7, ctx.xer);
	// bgt cr6,0x826ea074
	if (ctx.cr6.gt) goto loc_826EA074;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-24564
	ctx.r12.s64 = ctx.r12.s64 + -24564;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EA044;
	case 1:
		goto loc_826EA054;
	case 2:
		goto loc_826EA05C;
	case 3:
		goto loc_826EA064;
	case 4:
		goto loc_826EA04C;
	case 5:
		goto loc_826EA034;
	case 6:
		goto loc_826EA02C;
	case 7:
		goto loc_826EA03C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EA02C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826EA034:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EA03C:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EA044:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EA04C:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EA054:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826EA05C:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826EA064:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826EA06C:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826EA074:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA080);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA080) { __imp__sub_826EA080(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA080) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-27660
	ctx.r3.s64 = ctx.r11.s64 + -27660;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA090);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA090) { __imp__sub_826EA090(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA090) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e9fd0
	sub_826E9FD0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA098);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA098) { __imp__sub_826EA098(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA098) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826e9f38
	sub_826E9F38(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA0A0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA0A0) { __imp__sub_826EA0A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA0A0) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,8
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 8, ctx.xer);
	// bgt cr6,0x826ea0f4
	if (ctx.cr6.gt) goto loc_826EA0F4;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-24384
	ctx.r12.s64 = ctx.r12.s64 + -24384;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826EA0E4;
	case 1:
		goto loc_826EA0E4;
	case 2:
		goto loc_826EA0EC;
	case 3:
		goto loc_826EA0EC;
	case 4:
		goto loc_826EA0E4;
	case 5:
		goto loc_826EA0E4;
	case 6:
		goto loc_826EA0E4;
	case 7:
		goto loc_826EA0E4;
	case 8:
		goto loc_826EA0EC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EA0E4:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EA0EC:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826EA0F4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA100);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA100) { __imp__sub_826EA100(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA100) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-27628
	ctx.r3.s64 = ctx.r11.s64 + -27628;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA110);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA110) { __imp__sub_826EA110(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA110) {
	PPC_FUNC_PROLOGUE();
	// li r3,38
	ctx.r3.s64 = 38;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA118);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA118) { __imp__sub_826EA118(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA118) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,6
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 6, ctx.xer);
	// bgt cr6,0x826ea164
	if (ctx.cr6.gt) goto loc_826EA164;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-24264
	ctx.r12.s64 = ctx.r12.s64 + -24264;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826EA154;
	case 1:
		goto loc_826EA154;
	case 2:
		goto loc_826EA15C;
	case 3:
		goto loc_826EA15C;
	case 4:
		goto loc_826EA15C;
	case 5:
		goto loc_826EA15C;
	case 6:
		goto loc_826EA154;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EA154:
	// li r11,2
	ctx.r11.s64 = 2;
	// b 0x826ea168
	goto loc_826EA168;
loc_826EA15C:
	// li r11,1
	ctx.r11.s64 = 1;
	// b 0x826ea168
	goto loc_826EA168;
loc_826EA164:
	// li r11,-1
	ctx.r11.s64 = -1;
loc_826EA168:
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826ea1c0
	if (ctx.cr6.gt) goto loc_826EA1C0;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-24180
	ctx.r12.s64 = ctx.r12.s64 + -24180;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EA1A0;
	case 1:
		goto loc_826EA1A8;
	case 2:
		goto loc_826EA1B8;
	case 3:
		goto loc_826EA1C0;
	case 4:
		goto loc_826EA1B0;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EA1A0:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826EA1A8:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EA1B0:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EA1B8:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EA1C0:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA1C8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA1C8) { __imp__sub_826EA1C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA1C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826ea26c
	if (ctx.cr6.gt) goto loc_826EA26C;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826ea264
	if (ctx.cr6.eq) goto loc_826EA264;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,7
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 7, ctx.xer);
	// bgt cr6,0x826ea26c
	if (ctx.cr6.gt) goto loc_826EA26C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-24060
	ctx.r12.s64 = ctx.r12.s64 + -24060;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EA23C;
	case 1:
		goto loc_826EA24C;
	case 2:
		goto loc_826EA254;
	case 3:
		goto loc_826EA25C;
	case 4:
		goto loc_826EA244;
	case 5:
		goto loc_826EA224;
	case 6:
		goto loc_826EA22C;
	case 7:
		goto loc_826EA234;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EA224:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826EA22C:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EA234:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EA23C:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EA244:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EA24C:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826EA254:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826EA25C:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826EA264:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826EA26C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA278);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA278) { __imp__sub_826EA278(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA278) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-27592
	ctx.r3.s64 = ctx.r11.s64 + -27592;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA288);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA288) { __imp__sub_826EA288(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA288) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826ea1c8
	sub_826EA1C8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA290);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA290) { __imp__sub_826EA290(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA290) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826ea31c
	if (ctx.cr6.gt) goto loc_826EA31C;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826ea314
	if (ctx.cr6.eq) goto loc_826EA314;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x826ea31c
	if (ctx.cr6.gt) goto loc_826EA31C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-23860
	ctx.r12.s64 = ctx.r12.s64 + -23860;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EA2EC;
	case 1:
		goto loc_826EA2FC;
	case 2:
		goto loc_826EA304;
	case 3:
		goto loc_826EA30C;
	case 4:
		goto loc_826EA2F4;
	case 5:
		goto loc_826EA2E4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EA2E4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826EA2EC:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EA2F4:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EA2FC:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EA304:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EA30C:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826EA314:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826EA31C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA328);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA328) { __imp__sub_826EA328(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA328) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-27560
	ctx.r3.s64 = ctx.r11.s64 + -27560;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA338);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA338) { __imp__sub_826EA338(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA338) {
	PPC_FUNC_PROLOGUE();
	// li r3,54
	ctx.r3.s64 = 54;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA340);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA340) { __imp__sub_826EA340(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA340) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826ea290
	sub_826EA290(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA348);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA348) { __imp__sub_826EA348(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA348) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,7
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 7, ctx.xer);
	// bgt cr6,0x826ea3cc
	if (ctx.cr6.gt) goto loc_826EA3CC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-23704
	ctx.r12.s64 = ctx.r12.s64 + -23704;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826EA388;
	case 1:
		goto loc_826EA390;
	case 2:
		goto loc_826EA398;
	case 3:
		goto loc_826EA3A0;
	case 4:
		goto loc_826EA3A8;
	case 5:
		goto loc_826EA3B0;
	case 6:
		goto loc_826EA3B8;
	case 7:
		goto loc_826EA3C0;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EA388:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826EA390:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826EA398:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826EA3A0:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EA3A8:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EA3B0:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EA3B8:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EA3C0:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826EA3CC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA3D8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA3D8) { __imp__sub_826EA3D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA3D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826ea474
	if (ctx.cr6.gt) goto loc_826EA474;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826ea46c
	if (ctx.cr6.eq) goto loc_826EA46C;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,7
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 7, ctx.xer);
	// bgt cr6,0x826ea474
	if (ctx.cr6.gt) goto loc_826EA474;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-23532
	ctx.r12.s64 = ctx.r12.s64 + -23532;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EA44C;
	case 1:
		goto loc_826EA454;
	case 2:
		goto loc_826EA45C;
	case 3:
		goto loc_826EA464;
	case 4:
		goto loc_826EA434;
	case 5:
		goto loc_826EA43C;
	case 6:
		goto loc_826EA474;
	case 7:
		goto loc_826EA444;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EA434:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826EA43C:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EA444:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EA44C:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EA454:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EA45C:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826EA464:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826EA46C:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826EA474:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA480);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA480) { __imp__sub_826EA480(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA480) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-27528
	ctx.r3.s64 = ctx.r11.s64 + -27528;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA490);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA490) { __imp__sub_826EA490(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA490) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826ea3d8
	sub_826EA3D8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA498);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA498) { __imp__sub_826EA498(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA498) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826ea348
	sub_826EA348(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA4A0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA4A0) { __imp__sub_826EA4A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA4A0) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,7
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 7, ctx.xer);
	// bgt cr6,0x826ea4f0
	if (ctx.cr6.gt) goto loc_826EA4F0;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-23360
	ctx.r12.s64 = ctx.r12.s64 + -23360;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826EA4E0;
	case 1:
		goto loc_826EA4E0;
	case 2:
		goto loc_826EA4E8;
	case 3:
		goto loc_826EA4E8;
	case 4:
		goto loc_826EA4E0;
	case 5:
		goto loc_826EA4E0;
	case 6:
		goto loc_826EA4E0;
	case 7:
		goto loc_826EA4E8;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EA4E0:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EA4E8:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826EA4F0:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA4F8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA4F8) { __imp__sub_826EA4F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA4F8) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,5
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 5, ctx.xer);
	// bgt cr6,0x826ea564
	if (ctx.cr6.gt) goto loc_826EA564;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-23272
	ctx.r12.s64 = ctx.r12.s64 + -23272;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826EA530;
	case 1:
		goto loc_826EA538;
	case 2:
		goto loc_826EA540;
	case 3:
		goto loc_826EA548;
	case 4:
		goto loc_826EA550;
	case 5:
		goto loc_826EA558;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EA530:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EA538:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826EA540:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EA548:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EA550:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EA558:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826EA564:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA570);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA570) { __imp__sub_826EA570(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA570) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826ea5f0
	if (ctx.cr6.gt) goto loc_826EA5F0;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826ea5e8
	if (ctx.cr6.eq) goto loc_826EA5E8;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826ea5f0
	if (ctx.cr6.gt) goto loc_826EA5F0;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-23124
	ctx.r12.s64 = ctx.r12.s64 + -23124;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EA5C0;
	case 1:
		goto loc_826EA5D0;
	case 2:
		goto loc_826EA5D8;
	case 3:
		goto loc_826EA5E0;
	case 4:
		goto loc_826EA5C8;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EA5C0:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826EA5C8:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EA5D0:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EA5D8:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EA5E0:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EA5E8:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826EA5F0:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA5F8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA5F8) { __imp__sub_826EA5F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA5F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-27500
	ctx.r3.s64 = ctx.r11.s64 + -27500;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA608);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA608) { __imp__sub_826EA608(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA608) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826ea570
	sub_826EA570(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA610);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA610) { __imp__sub_826EA610(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA610) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826ea4f8
	sub_826EA4F8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA618);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA618) { __imp__sub_826EA618(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA618) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,5
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 5, ctx.xer);
	// bgt cr6,0x826ea660
	if (ctx.cr6.gt) goto loc_826EA660;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-22984
	ctx.r12.s64 = ctx.r12.s64 + -22984;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826EA650;
	case 1:
		goto loc_826EA650;
	case 2:
		goto loc_826EA658;
	case 3:
		goto loc_826EA658;
	case 4:
		goto loc_826EA658;
	case 5:
		goto loc_826EA650;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EA650:
	// li r11,2
	ctx.r11.s64 = 2;
	// b 0x826ea664
	goto loc_826EA664;
loc_826EA658:
	// li r11,1
	ctx.r11.s64 = 1;
	// b 0x826ea664
	goto loc_826EA664;
loc_826EA660:
	// li r11,-1
	ctx.r11.s64 = -1;
loc_826EA664:
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826ea6bc
	if (ctx.cr6.gt) goto loc_826EA6BC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-22904
	ctx.r12.s64 = ctx.r12.s64 + -22904;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EA69C;
	case 1:
		goto loc_826EA6A4;
	case 2:
		goto loc_826EA6B4;
	case 3:
		goto loc_826EA6BC;
	case 4:
		goto loc_826EA6AC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EA69C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826EA6A4:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EA6AC:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EA6B4:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EA6BC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA6C8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA6C8) { __imp__sub_826EA6C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA6C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-27468
	ctx.r3.s64 = ctx.r11.s64 + -27468;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA6D8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA6D8) { __imp__sub_826EA6D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA6D8) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r4,3
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 3, ctx.xer);
	// bgt cr6,0x826ea70c
	if (ctx.cr6.gt) goto loc_826EA70C;
	// beq cr6,0x826ea704
	if (ctx.cr6.eq) goto loc_826EA704;
	// cmpwi cr6,r4,1
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 1, ctx.xer);
	// beq cr6,0x826ea6fc
	if (ctx.cr6.eq) goto loc_826EA6FC;
	// cmpwi cr6,r4,2
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 2, ctx.xer);
	// bne cr6,0x826ea714
	if (!ctx.cr6.eq) goto loc_826EA714;
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EA6FC:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EA704:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EA70C:
	// cmplwi cr6,r4,65534
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 65534, ctx.xer);
	// beq cr6,0x826ea71c
	if (ctx.cr6.eq) goto loc_826EA71C;
loc_826EA714:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826EA71C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA728);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA728) { __imp__sub_826EA728(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA728) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,3
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 3, ctx.xer);
	// bgt cr6,0x826ea77c
	if (ctx.cr6.gt) goto loc_826EA77C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-22712
	ctx.r12.s64 = ctx.r12.s64 + -22712;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826EA758;
	case 1:
		goto loc_826EA764;
	case 2:
		goto loc_826EA76C;
	case 3:
		goto loc_826EA774;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EA758:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826EA764:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EA76C:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EA774:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EA77C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA788);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA788) { __imp__sub_826EA788(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA788) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,3
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 3, ctx.xer);
	// bgt cr6,0x826ea7c8
	if (ctx.cr6.gt) goto loc_826EA7C8;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-22616
	ctx.r12.s64 = ctx.r12.s64 + -22616;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826EA7B8;
	case 1:
		goto loc_826EA7B8;
	case 2:
		goto loc_826EA7C0;
	case 3:
		goto loc_826EA7C0;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EA7B8:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826EA7C0:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EA7C8:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA7D0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA7D0) { __imp__sub_826EA7D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA7D0) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,3
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 3, ctx.xer);
	// bgt cr6,0x826ea810
	if (ctx.cr6.gt) goto loc_826EA810;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-22544
	ctx.r12.s64 = ctx.r12.s64 + -22544;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826EA800;
	case 1:
		goto loc_826EA800;
	case 2:
		goto loc_826EA808;
	case 3:
		goto loc_826EA808;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EA800:
	// li r11,2
	ctx.r11.s64 = 2;
	// b 0x826ea814
	goto loc_826EA814;
loc_826EA808:
	// li r11,1
	ctx.r11.s64 = 1;
	// b 0x826ea814
	goto loc_826EA814;
loc_826EA810:
	// li r11,-1
	ctx.r11.s64 = -1;
loc_826EA814:
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826ea86c
	if (ctx.cr6.gt) goto loc_826EA86C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-22472
	ctx.r12.s64 = ctx.r12.s64 + -22472;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EA84C;
	case 1:
		goto loc_826EA854;
	case 2:
		goto loc_826EA864;
	case 3:
		goto loc_826EA86C;
	case 4:
		goto loc_826EA85C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EA84C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826EA854:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EA85C:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EA864:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EA86C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA878);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA878) { __imp__sub_826EA878(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA878) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-27436
	ctx.r3.s64 = ctx.r11.s64 + -27436;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA888);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA888) { __imp__sub_826EA888(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA888) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r4,6
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 6, ctx.xer);
	// bgt cr6,0x826ea8bc
	if (ctx.cr6.gt) goto loc_826EA8BC;
	// beq cr6,0x826ea8b4
	if (ctx.cr6.eq) goto loc_826EA8B4;
	// cmpwi cr6,r4,1
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 1, ctx.xer);
	// beq cr6,0x826ea8ac
	if (ctx.cr6.eq) goto loc_826EA8AC;
	// cmpwi cr6,r4,3
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 3, ctx.xer);
	// bne cr6,0x826ea8c4
	if (!ctx.cr6.eq) goto loc_826EA8C4;
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826EA8AC:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EA8B4:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EA8BC:
	// cmplwi cr6,r4,65534
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 65534, ctx.xer);
	// beq cr6,0x826ea8cc
	if (ctx.cr6.eq) goto loc_826EA8CC;
loc_826EA8C4:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826EA8CC:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA8D8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA8D8) { __imp__sub_826EA8D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA8D8) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,3
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 3, ctx.xer);
	// bgt cr6,0x826ea92c
	if (ctx.cr6.gt) goto loc_826EA92C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-22280
	ctx.r12.s64 = ctx.r12.s64 + -22280;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826EA908;
	case 1:
		goto loc_826EA910;
	case 2:
		goto loc_826EA918;
	case 3:
		goto loc_826EA920;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EA908:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EA910:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EA918:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826EA920:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826EA92C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA938);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA938) { __imp__sub_826EA938(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA938) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,3
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 3, ctx.xer);
	// bgt cr6,0x826ea978
	if (ctx.cr6.gt) goto loc_826EA978;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-22184
	ctx.r12.s64 = ctx.r12.s64 + -22184;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826EA968;
	case 1:
		goto loc_826EA968;
	case 2:
		goto loc_826EA970;
	case 3:
		goto loc_826EA970;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EA968:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EA970:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826EA978:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EA980);  /* forward decl */
PPC_WEAK_FUNC(sub_826EA980) { __imp__sub_826EA980(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EA980) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,3
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 3, ctx.xer);
	// bgt cr6,0x826ea9c0
	if (ctx.cr6.gt) goto loc_826EA9C0;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-22112
	ctx.r12.s64 = ctx.r12.s64 + -22112;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826EA9B0;
	case 1:
		goto loc_826EA9B0;
	case 2:
		goto loc_826EA9B8;
	case 3:
		goto loc_826EA9B8;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EA9B0:
	// li r11,1
	ctx.r11.s64 = 1;
	// b 0x826ea9c4
	goto loc_826EA9C4;
loc_826EA9B8:
	// li r11,2
	ctx.r11.s64 = 2;
	// b 0x826ea9c4
	goto loc_826EA9C4;
loc_826EA9C0:
	// li r11,-1
	ctx.r11.s64 = -1;
loc_826EA9C4:
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826eaa1c
	if (ctx.cr6.gt) goto loc_826EAA1C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-22040
	ctx.r12.s64 = ctx.r12.s64 + -22040;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EA9FC;
	case 1:
		goto loc_826EAA04;
	case 2:
		goto loc_826EAA14;
	case 3:
		goto loc_826EAA1C;
	case 4:
		goto loc_826EAA0C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EA9FC:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826EAA04:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EAA0C:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EAA14:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EAA1C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EAA28);  /* forward decl */
PPC_WEAK_FUNC(sub_826EAA28) { __imp__sub_826EAA28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EAA28) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-27400
	ctx.r3.s64 = ctx.r11.s64 + -27400;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EAA38);  /* forward decl */
PPC_WEAK_FUNC(sub_826EAA38) { __imp__sub_826EAA38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EAA38) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r4,5
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 5, ctx.xer);
	// bgt cr6,0x826eaa6c
	if (ctx.cr6.gt) goto loc_826EAA6C;
	// beq cr6,0x826eaa64
	if (ctx.cr6.eq) goto loc_826EAA64;
	// cmpwi cr6,r4,1
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 1, ctx.xer);
	// beq cr6,0x826eaa5c
	if (ctx.cr6.eq) goto loc_826EAA5C;
	// cmpwi cr6,r4,3
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 3, ctx.xer);
	// bne cr6,0x826eaa74
	if (!ctx.cr6.eq) goto loc_826EAA74;
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EAA5C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826EAA64:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EAA6C:
	// cmplwi cr6,r4,65534
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 65534, ctx.xer);
	// beq cr6,0x826eaa7c
	if (ctx.cr6.eq) goto loc_826EAA7C;
loc_826EAA74:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826EAA7C:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EAA88);  /* forward decl */
PPC_WEAK_FUNC(sub_826EAA88) { __imp__sub_826EAA88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EAA88) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,3
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 3, ctx.xer);
	// bgt cr6,0x826eaadc
	if (ctx.cr6.gt) goto loc_826EAADC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-21848
	ctx.r12.s64 = ctx.r12.s64 + -21848;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826EAAB8;
	case 1:
		goto loc_826EAAC0;
	case 2:
		goto loc_826EAAC8;
	case 3:
		goto loc_826EAAD0;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EAAB8:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EAAC0:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EAAC8:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826EAAD0:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826EAADC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EAAE8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EAAE8) { __imp__sub_826EAAE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EAAE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-27364
	ctx.r3.s64 = ctx.r11.s64 + -27364;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EAAF8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EAAF8) { __imp__sub_826EAAF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EAAF8) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r4,6
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 6, ctx.xer);
	// bgt cr6,0x826eab2c
	if (ctx.cr6.gt) goto loc_826EAB2C;
	// beq cr6,0x826eab24
	if (ctx.cr6.eq) goto loc_826EAB24;
	// cmpwi cr6,r4,1
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 1, ctx.xer);
	// beq cr6,0x826eab1c
	if (ctx.cr6.eq) goto loc_826EAB1C;
	// cmpwi cr6,r4,3
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 3, ctx.xer);
	// bne cr6,0x826eab34
	if (!ctx.cr6.eq) goto loc_826EAB34;
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EAB1C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826EAB24:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EAB2C:
	// cmplwi cr6,r4,65534
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 65534, ctx.xer);
	// beq cr6,0x826eab3c
	if (ctx.cr6.eq) goto loc_826EAB3C;
loc_826EAB34:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826EAB3C:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EAB48);  /* forward decl */
PPC_WEAK_FUNC(sub_826EAB48) { __imp__sub_826EAB48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EAB48) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,3
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 3, ctx.xer);
	// bgt cr6,0x826eab9c
	if (ctx.cr6.gt) goto loc_826EAB9C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-21656
	ctx.r12.s64 = ctx.r12.s64 + -21656;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826EAB78;
	case 1:
		goto loc_826EAB80;
	case 2:
		goto loc_826EAB88;
	case 3:
		goto loc_826EAB90;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EAB78:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EAB80:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EAB88:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826EAB90:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826EAB9C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EABA8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EABA8) { __imp__sub_826EABA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EABA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-27328
	ctx.r3.s64 = ctx.r11.s64 + -27328;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EABB8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EABB8) { __imp__sub_826EABB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EABB8) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,5
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 5, ctx.xer);
	// bgt cr6,0x826eac00
	if (ctx.cr6.gt) goto loc_826EAC00;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-21544
	ctx.r12.s64 = ctx.r12.s64 + -21544;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826EABF0;
	case 1:
		goto loc_826EABF0;
	case 2:
		goto loc_826EABF0;
	case 3:
		goto loc_826EABF8;
	case 4:
		goto loc_826EABF8;
	case 5:
		goto loc_826EABF8;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EABF0:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EABF8:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826EAC00:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EAC08);  /* forward decl */
PPC_WEAK_FUNC(sub_826EAC08) { __imp__sub_826EAC08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EAC08) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,5
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 5, ctx.xer);
	// bgt cr6,0x826eac74
	if (ctx.cr6.gt) goto loc_826EAC74;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-21464
	ctx.r12.s64 = ctx.r12.s64 + -21464;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826EAC40;
	case 1:
		goto loc_826EAC48;
	case 2:
		goto loc_826EAC50;
	case 3:
		goto loc_826EAC58;
	case 4:
		goto loc_826EAC60;
	case 5:
		goto loc_826EAC68;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EAC40:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EAC48:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826EAC50:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EAC58:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826EAC60:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EAC68:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826EAC74:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EAC80);  /* forward decl */
PPC_WEAK_FUNC(sub_826EAC80) { __imp__sub_826EAC80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EAC80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-27284
	ctx.r3.s64 = ctx.r11.s64 + -27284;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EAC90);  /* forward decl */
PPC_WEAK_FUNC(sub_826EAC90) { __imp__sub_826EAC90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EAC90) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826eac08
	sub_826EAC08(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EAC98);  /* forward decl */
PPC_WEAK_FUNC(sub_826EAC98) { __imp__sub_826EAC98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EAC98) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,5
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 5, ctx.xer);
	// bgt cr6,0x826eace0
	if (ctx.cr6.gt) goto loc_826EACE0;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-21320
	ctx.r12.s64 = ctx.r12.s64 + -21320;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826EACD0;
	case 1:
		goto loc_826EACD0;
	case 2:
		goto loc_826EACD0;
	case 3:
		goto loc_826EACD8;
	case 4:
		goto loc_826EACD8;
	case 5:
		goto loc_826EACD8;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EACD0:
	// li r11,1
	ctx.r11.s64 = 1;
	// b 0x826eace4
	goto loc_826EACE4;
loc_826EACD8:
	// li r11,2
	ctx.r11.s64 = 2;
	// b 0x826eace4
	goto loc_826EACE4;
loc_826EACE0:
	// li r11,-1
	ctx.r11.s64 = -1;
loc_826EACE4:
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826ead3c
	if (ctx.cr6.gt) goto loc_826EAD3C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-21240
	ctx.r12.s64 = ctx.r12.s64 + -21240;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EAD1C;
	case 1:
		goto loc_826EAD24;
	case 2:
		goto loc_826EAD34;
	case 3:
		goto loc_826EAD3C;
	case 4:
		goto loc_826EAD2C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EAD1C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826EAD24:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EAD2C:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EAD34:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EAD3C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EAD48);  /* forward decl */
PPC_WEAK_FUNC(sub_826EAD48) { __imp__sub_826EAD48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EAD48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826eadcc
	if (ctx.cr6.gt) goto loc_826EADCC;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826eadc4
	if (ctx.cr6.eq) goto loc_826EADC4;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x826eadcc
	if (ctx.cr6.gt) goto loc_826EADCC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-21116
	ctx.r12.s64 = ctx.r12.s64 + -21116;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EAD9C;
	case 1:
		goto loc_826EADBC;
	case 2:
		goto loc_826EADAC;
	case 3:
		goto loc_826EADCC;
	case 4:
		goto loc_826EADA4;
	case 5:
		goto loc_826EADB4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EAD9C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826EADA4:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EADAC:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EADB4:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EADBC:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EADC4:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826EADCC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EADD8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EADD8) { __imp__sub_826EADD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EADD8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-27236
	ctx.r3.s64 = ctx.r11.s64 + -27236;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EADE8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EADE8) { __imp__sub_826EADE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EADE8) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826ead48
	sub_826EAD48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EADF0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EADF0) { __imp__sub_826EADF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EADF0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-27188
	ctx.r3.s64 = ctx.r11.s64 + -27188;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EAE00);  /* forward decl */
PPC_WEAK_FUNC(sub_826EAE00) { __imp__sub_826EAE00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EAE00) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r4,7
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 7, ctx.xer);
	// bgt cr6,0x826eae34
	if (ctx.cr6.gt) goto loc_826EAE34;
	// beq cr6,0x826eae2c
	if (ctx.cr6.eq) goto loc_826EAE2C;
	// cmpwi cr6,r4,1
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 1, ctx.xer);
	// beq cr6,0x826eae24
	if (ctx.cr6.eq) goto loc_826EAE24;
	// cmpwi cr6,r4,6
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 6, ctx.xer);
	// bne cr6,0x826eae44
	if (!ctx.cr6.eq) goto loc_826EAE44;
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826EAE24:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EAE2C:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EAE34:
	// cmpwi cr6,r4,8
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 8, ctx.xer);
	// beq cr6,0x826eae54
	if (ctx.cr6.eq) goto loc_826EAE54;
	// cmplwi cr6,r4,65534
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 65534, ctx.xer);
	// beq cr6,0x826eae4c
	if (ctx.cr6.eq) goto loc_826EAE4C;
loc_826EAE44:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826EAE4C:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EAE54:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EAE60);  /* forward decl */
PPC_WEAK_FUNC(sub_826EAE60) { __imp__sub_826EAE60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EAE60) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,4
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 4, ctx.xer);
	// bgt cr6,0x826eaec0
	if (ctx.cr6.gt) goto loc_826EAEC0;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-20864
	ctx.r12.s64 = ctx.r12.s64 + -20864;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826EAE94;
	case 1:
		goto loc_826EAE9C;
	case 2:
		goto loc_826EAEA4;
	case 3:
		goto loc_826EAEAC;
	case 4:
		goto loc_826EAEB4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EAE94:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826EAE9C:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826EAEA4:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826EAEAC:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EAEB4:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826EAEC0:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EAEC8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EAEC8) { __imp__sub_826EAEC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EAEC8) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,4
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 4, ctx.xer);
	// bgt cr6,0x826eaf0c
	if (ctx.cr6.gt) goto loc_826EAF0C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-20760
	ctx.r12.s64 = ctx.r12.s64 + -20760;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826EAEFC;
	case 1:
		goto loc_826EAEFC;
	case 2:
		goto loc_826EAF04;
	case 3:
		goto loc_826EAF04;
	case 4:
		goto loc_826EAF04;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EAEFC:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EAF04:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826EAF0C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EAF18);  /* forward decl */
PPC_WEAK_FUNC(sub_826EAF18) { __imp__sub_826EAF18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EAF18) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-27152
	ctx.r3.s64 = ctx.r11.s64 + -27152;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EAF28);  /* forward decl */
PPC_WEAK_FUNC(sub_826EAF28) { __imp__sub_826EAF28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EAF28) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-27116
	ctx.r3.s64 = ctx.r11.s64 + -27116;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EAF38);  /* forward decl */
PPC_WEAK_FUNC(sub_826EAF38) { __imp__sub_826EAF38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EAF38) {
	PPC_FUNC_PROLOGUE();
	// li r3,73
	ctx.r3.s64 = 73;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EAF40);  /* forward decl */
PPC_WEAK_FUNC(sub_826EAF40) { __imp__sub_826EAF40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EAF40) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r4,7
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 7, ctx.xer);
	// bgt cr6,0x826eaf74
	if (ctx.cr6.gt) goto loc_826EAF74;
	// beq cr6,0x826eaf6c
	if (ctx.cr6.eq) goto loc_826EAF6C;
	// cmpwi cr6,r4,1
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 1, ctx.xer);
	// beq cr6,0x826eaf64
	if (ctx.cr6.eq) goto loc_826EAF64;
	// cmpwi cr6,r4,6
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 6, ctx.xer);
	// bne cr6,0x826eaf84
	if (!ctx.cr6.eq) goto loc_826EAF84;
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EAF64:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EAF6C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826EAF74:
	// cmpwi cr6,r4,8
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 8, ctx.xer);
	// beq cr6,0x826eaf94
	if (ctx.cr6.eq) goto loc_826EAF94;
	// cmplwi cr6,r4,65534
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 65534, ctx.xer);
	// beq cr6,0x826eaf8c
	if (ctx.cr6.eq) goto loc_826EAF8C;
loc_826EAF84:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826EAF8C:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EAF94:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EAFA0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EAFA0) { __imp__sub_826EAFA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EAFA0) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,4
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 4, ctx.xer);
	// bgt cr6,0x826eb000
	if (ctx.cr6.gt) goto loc_826EB000;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-20544
	ctx.r12.s64 = ctx.r12.s64 + -20544;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826EAFD4;
	case 1:
		goto loc_826EAFDC;
	case 2:
		goto loc_826EAFE4;
	case 3:
		goto loc_826EAFEC;
	case 4:
		goto loc_826EAFF4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EAFD4:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826EAFDC:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826EAFE4:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826EAFEC:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EAFF4:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826EB000:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EB008);  /* forward decl */
PPC_WEAK_FUNC(sub_826EB008) { __imp__sub_826EB008(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EB008) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-27080
	ctx.r3.s64 = ctx.r11.s64 + -27080;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EB018);  /* forward decl */
PPC_WEAK_FUNC(sub_826EB018) { __imp__sub_826EB018(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EB018) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-27048
	ctx.r3.s64 = ctx.r11.s64 + -27048;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EB028);  /* forward decl */
PPC_WEAK_FUNC(sub_826EB028) { __imp__sub_826EB028(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EB028) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r4,6
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 6, ctx.xer);
	// bgt cr6,0x826eb05c
	if (ctx.cr6.gt) goto loc_826EB05C;
	// beq cr6,0x826eb054
	if (ctx.cr6.eq) goto loc_826EB054;
	// cmpwi cr6,r4,1
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 1, ctx.xer);
	// beq cr6,0x826eb04c
	if (ctx.cr6.eq) goto loc_826EB04C;
	// cmpwi cr6,r4,5
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 5, ctx.xer);
	// bne cr6,0x826eb06c
	if (!ctx.cr6.eq) goto loc_826EB06C;
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826EB04C:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EB054:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EB05C:
	// cmpwi cr6,r4,8
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 8, ctx.xer);
	// beq cr6,0x826eb07c
	if (ctx.cr6.eq) goto loc_826EB07C;
	// cmplwi cr6,r4,65534
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 65534, ctx.xer);
	// beq cr6,0x826eb074
	if (ctx.cr6.eq) goto loc_826EB074;
loc_826EB06C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826EB074:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EB07C:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EB088);  /* forward decl */
PPC_WEAK_FUNC(sub_826EB088) { __imp__sub_826EB088(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EB088) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r4,4
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 4, ctx.xer);
	// bgt cr6,0x826eb0e8
	if (ctx.cr6.gt) goto loc_826EB0E8;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-20312
	ctx.r12.s64 = ctx.r12.s64 + -20312;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826EB0BC;
	case 1:
		goto loc_826EB0C4;
	case 2:
		goto loc_826EB0CC;
	case 3:
		goto loc_826EB0D4;
	case 4:
		goto loc_826EB0DC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EB0BC:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826EB0C4:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826EB0CC:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826EB0D4:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EB0DC:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826EB0E8:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EB0F0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EB0F0) { __imp__sub_826EB0F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EB0F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826eb1b4
	if (ctx.cr6.gt) goto loc_826EB1B4;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826eb1ac
	if (ctx.cr6.eq) goto loc_826EB1AC;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,31
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 31, ctx.xer);
	// bgt cr6,0x826eb1b4
	if (ctx.cr6.gt) goto loc_826EB1B4;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-20180
	ctx.r12.s64 = ctx.r12.s64 + -20180;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EB1AC;
	case 1:
		goto loc_826EB1AC;
	case 2:
		goto loc_826EB1AC;
	case 3:
		goto loc_826EB1AC;
	case 4:
		goto loc_826EB1AC;
	case 5:
		goto loc_826EB1AC;
	case 6:
		goto loc_826EB1AC;
	case 7:
		goto loc_826EB1AC;
	case 8:
		goto loc_826EB1AC;
	case 9:
		goto loc_826EB1AC;
	case 10:
		goto loc_826EB1AC;
	case 11:
		goto loc_826EB1AC;
	case 12:
		goto loc_826EB1AC;
	case 13:
		goto loc_826EB1AC;
	case 14:
		goto loc_826EB1AC;
	case 15:
		goto loc_826EB1AC;
	case 16:
		goto loc_826EB1B4;
	case 17:
		goto loc_826EB1B4;
	case 18:
		goto loc_826EB1AC;
	case 19:
		goto loc_826EB1AC;
	case 20:
		goto loc_826EB1AC;
	case 21:
		goto loc_826EB1B4;
	case 22:
		goto loc_826EB1B4;
	case 23:
		goto loc_826EB1B4;
	case 24:
		goto loc_826EB1B4;
	case 25:
		goto loc_826EB1AC;
	case 26:
		goto loc_826EB1AC;
	case 27:
		goto loc_826EB1AC;
	case 28:
		goto loc_826EB1B4;
	case 29:
		goto loc_826EB1AC;
	case 30:
		goto loc_826EB1AC;
	case 31:
		goto loc_826EB1AC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EB1AC:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EB1B4:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EB1C0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EB1C0) { __imp__sub_826EB1C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EB1C0) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,25
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 25, ctx.xer);
	// bgt cr6,0x826eb31c
	if (ctx.cr6.gt) goto loc_826EB31C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-20000
	ctx.r12.s64 = ctx.r12.s64 + -20000;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826EB248;
	case 1:
		goto loc_826EB250;
	case 2:
		goto loc_826EB258;
	case 3:
		goto loc_826EB260;
	case 4:
		goto loc_826EB268;
	case 5:
		goto loc_826EB270;
	case 6:
		goto loc_826EB278;
	case 7:
		goto loc_826EB280;
	case 8:
		goto loc_826EB288;
	case 9:
		goto loc_826EB290;
	case 10:
		goto loc_826EB298;
	case 11:
		goto loc_826EB2A0;
	case 12:
		goto loc_826EB2A8;
	case 13:
		goto loc_826EB2B0;
	case 14:
		goto loc_826EB2B8;
	case 15:
		goto loc_826EB2C0;
	case 16:
		goto loc_826EB2C8;
	case 17:
		goto loc_826EB2D0;
	case 18:
		goto loc_826EB2D8;
	case 19:
		goto loc_826EB2E0;
	case 20:
		goto loc_826EB2E8;
	case 21:
		goto loc_826EB2F0;
	case 22:
		goto loc_826EB2F8;
	case 23:
		goto loc_826EB300;
	case 24:
		goto loc_826EB308;
	case 25:
		goto loc_826EB310;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EB248:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EB250:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826EB258:
	// li r3,27
	ctx.r3.s64 = 27;
	// blr 
	return;
loc_826EB260:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EB268:
	// li r3,26
	ctx.r3.s64 = 26;
	// blr 
	return;
loc_826EB270:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826EB278:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826EB280:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826EB288:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_826EB290:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_826EB298:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_826EB2A0:
	// li r3,9
	ctx.r3.s64 = 9;
	// blr 
	return;
loc_826EB2A8:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_826EB2B0:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_826EB2B8:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_826EB2C0:
	// li r3,19
	ctx.r3.s64 = 19;
	// blr 
	return;
loc_826EB2C8:
	// li r3,20
	ctx.r3.s64 = 20;
	// blr 
	return;
loc_826EB2D0:
	// li r3,21
	ctx.r3.s64 = 21;
	// blr 
	return;
loc_826EB2D8:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_826EB2E0:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EB2E8:
	// li r3,28
	ctx.r3.s64 = 28;
	// blr 
	return;
loc_826EB2F0:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EB2F8:
	// li r3,30
	ctx.r3.s64 = 30;
	// blr 
	return;
loc_826EB300:
	// li r3,31
	ctx.r3.s64 = 31;
	// blr 
	return;
loc_826EB308:
	// li r3,32
	ctx.r3.s64 = 32;
	// blr 
	return;
loc_826EB310:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826EB31C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EB328);  /* forward decl */
PPC_WEAK_FUNC(sub_826EB328) { __imp__sub_826EB328(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EB328) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826eb4b4
	if (ctx.cr6.gt) goto loc_826EB4B4;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826eb4ac
	if (ctx.cr6.eq) goto loc_826EB4AC;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,31
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 31, ctx.xer);
	// bgt cr6,0x826eb4b4
	if (ctx.cr6.gt) goto loc_826EB4B4;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-19612
	ctx.r12.s64 = ctx.r12.s64 + -19612;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EB3FC;
	case 1:
		goto loc_826EB47C;
	case 2:
		goto loc_826EB48C;
	case 3:
		goto loc_826EB3E4;
	case 4:
		goto loc_826EB3EC;
	case 5:
		goto loc_826EB40C;
	case 6:
		goto loc_826EB414;
	case 7:
		goto loc_826EB41C;
	case 8:
		goto loc_826EB43C;
	case 9:
		goto loc_826EB44C;
	case 10:
		goto loc_826EB454;
	case 11:
		goto loc_826EB424;
	case 12:
		goto loc_826EB42C;
	case 13:
		goto loc_826EB434;
	case 14:
		goto loc_826EB474;
	case 15:
		goto loc_826EB444;
	case 16:
		goto loc_826EB4B4;
	case 17:
		goto loc_826EB4B4;
	case 18:
		goto loc_826EB45C;
	case 19:
		goto loc_826EB464;
	case 20:
		goto loc_826EB46C;
	case 21:
		goto loc_826EB4B4;
	case 22:
		goto loc_826EB4B4;
	case 23:
		goto loc_826EB4B4;
	case 24:
		goto loc_826EB4B4;
	case 25:
		goto loc_826EB404;
	case 26:
		goto loc_826EB3F4;
	case 27:
		goto loc_826EB484;
	case 28:
		goto loc_826EB4B4;
	case 29:
		goto loc_826EB494;
	case 30:
		goto loc_826EB49C;
	case 31:
		goto loc_826EB4A4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EB3E4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826EB3EC:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EB3F4:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EB3FC:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EB404:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EB40C:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826EB414:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826EB41C:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826EB424:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826EB42C:
	// li r3,9
	ctx.r3.s64 = 9;
	// blr 
	return;
loc_826EB434:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_826EB43C:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_826EB444:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_826EB44C:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_826EB454:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_826EB45C:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_826EB464:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_826EB46C:
	// li r3,17
	ctx.r3.s64 = 17;
	// blr 
	return;
loc_826EB474:
	// li r3,18
	ctx.r3.s64 = 18;
	// blr 
	return;
loc_826EB47C:
	// li r3,19
	ctx.r3.s64 = 19;
	// blr 
	return;
loc_826EB484:
	// li r3,20
	ctx.r3.s64 = 20;
	// blr 
	return;
loc_826EB48C:
	// li r3,21
	ctx.r3.s64 = 21;
	// blr 
	return;
loc_826EB494:
	// li r3,22
	ctx.r3.s64 = 22;
	// blr 
	return;
loc_826EB49C:
	// li r3,23
	ctx.r3.s64 = 23;
	// blr 
	return;
loc_826EB4A4:
	// li r3,24
	ctx.r3.s64 = 24;
	// blr 
	return;
loc_826EB4AC:
	// li r3,25
	ctx.r3.s64 = 25;
	// blr 
	return;
loc_826EB4B4:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EB4C0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EB4C0) { __imp__sub_826EB4C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EB4C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-27016
	ctx.r3.s64 = ctx.r11.s64 + -27016;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EB4D0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EB4D0) { __imp__sub_826EB4D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EB4D0) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826eb328
	sub_826EB328(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EB4D8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EB4D8) { __imp__sub_826EB4D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EB4D8) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826eb1c0
	sub_826EB1C0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EB4E0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EB4E0) { __imp__sub_826EB4E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EB4E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826eb5a4
	if (ctx.cr6.gt) goto loc_826EB5A4;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826eb59c
	if (ctx.cr6.eq) goto loc_826EB59C;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,31
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 31, ctx.xer);
	// bgt cr6,0x826eb5a4
	if (ctx.cr6.gt) goto loc_826EB5A4;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-19172
	ctx.r12.s64 = ctx.r12.s64 + -19172;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EB59C;
	case 1:
		goto loc_826EB59C;
	case 2:
		goto loc_826EB59C;
	case 3:
		goto loc_826EB59C;
	case 4:
		goto loc_826EB59C;
	case 5:
		goto loc_826EB59C;
	case 6:
		goto loc_826EB59C;
	case 7:
		goto loc_826EB59C;
	case 8:
		goto loc_826EB5A4;
	case 9:
		goto loc_826EB59C;
	case 10:
		goto loc_826EB59C;
	case 11:
		goto loc_826EB59C;
	case 12:
		goto loc_826EB59C;
	case 13:
		goto loc_826EB59C;
	case 14:
		goto loc_826EB59C;
	case 15:
		goto loc_826EB59C;
	case 16:
		goto loc_826EB59C;
	case 17:
		goto loc_826EB5A4;
	case 18:
		goto loc_826EB59C;
	case 19:
		goto loc_826EB59C;
	case 20:
		goto loc_826EB59C;
	case 21:
		goto loc_826EB5A4;
	case 22:
		goto loc_826EB5A4;
	case 23:
		goto loc_826EB5A4;
	case 24:
		goto loc_826EB5A4;
	case 25:
		goto loc_826EB59C;
	case 26:
		goto loc_826EB59C;
	case 27:
		goto loc_826EB59C;
	case 28:
		goto loc_826EB5A4;
	case 29:
		goto loc_826EB59C;
	case 30:
		goto loc_826EB59C;
	case 31:
		goto loc_826EB59C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EB59C:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EB5A4:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EB5B0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EB5B0) { __imp__sub_826EB5B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EB5B0) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,25
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 25, ctx.xer);
	// bgt cr6,0x826eb70c
	if (ctx.cr6.gt) goto loc_826EB70C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-18992
	ctx.r12.s64 = ctx.r12.s64 + -18992;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826EB638;
	case 1:
		goto loc_826EB640;
	case 2:
		goto loc_826EB648;
	case 3:
		goto loc_826EB650;
	case 4:
		goto loc_826EB658;
	case 5:
		goto loc_826EB660;
	case 6:
		goto loc_826EB668;
	case 7:
		goto loc_826EB670;
	case 8:
		goto loc_826EB678;
	case 9:
		goto loc_826EB680;
	case 10:
		goto loc_826EB688;
	case 11:
		goto loc_826EB690;
	case 12:
		goto loc_826EB698;
	case 13:
		goto loc_826EB6A0;
	case 14:
		goto loc_826EB6A8;
	case 15:
		goto loc_826EB6B0;
	case 16:
		goto loc_826EB6B8;
	case 17:
		goto loc_826EB6C0;
	case 18:
		goto loc_826EB6C8;
	case 19:
		goto loc_826EB6D0;
	case 20:
		goto loc_826EB6D8;
	case 21:
		goto loc_826EB6E0;
	case 22:
		goto loc_826EB6E8;
	case 23:
		goto loc_826EB6F0;
	case 24:
		goto loc_826EB6F8;
	case 25:
		goto loc_826EB700;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EB638:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EB640:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826EB648:
	// li r3,27
	ctx.r3.s64 = 27;
	// blr 
	return;
loc_826EB650:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EB658:
	// li r3,26
	ctx.r3.s64 = 26;
	// blr 
	return;
loc_826EB660:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826EB668:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826EB670:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826EB678:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_826EB680:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_826EB688:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_826EB690:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_826EB698:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_826EB6A0:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_826EB6A8:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_826EB6B0:
	// li r3,19
	ctx.r3.s64 = 19;
	// blr 
	return;
loc_826EB6B8:
	// li r3,20
	ctx.r3.s64 = 20;
	// blr 
	return;
loc_826EB6C0:
	// li r3,21
	ctx.r3.s64 = 21;
	// blr 
	return;
loc_826EB6C8:
	// li r3,17
	ctx.r3.s64 = 17;
	// blr 
	return;
loc_826EB6D0:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EB6D8:
	// li r3,28
	ctx.r3.s64 = 28;
	// blr 
	return;
loc_826EB6E0:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EB6E8:
	// li r3,30
	ctx.r3.s64 = 30;
	// blr 
	return;
loc_826EB6F0:
	// li r3,31
	ctx.r3.s64 = 31;
	// blr 
	return;
loc_826EB6F8:
	// li r3,32
	ctx.r3.s64 = 32;
	// blr 
	return;
loc_826EB700:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826EB70C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EB718);  /* forward decl */
PPC_WEAK_FUNC(sub_826EB718) { __imp__sub_826EB718(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EB718) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826eb8a4
	if (ctx.cr6.gt) goto loc_826EB8A4;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826eb89c
	if (ctx.cr6.eq) goto loc_826EB89C;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,31
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 31, ctx.xer);
	// bgt cr6,0x826eb8a4
	if (ctx.cr6.gt) goto loc_826EB8A4;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-18604
	ctx.r12.s64 = ctx.r12.s64 + -18604;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EB7EC;
	case 1:
		goto loc_826EB86C;
	case 2:
		goto loc_826EB87C;
	case 3:
		goto loc_826EB7D4;
	case 4:
		goto loc_826EB7DC;
	case 5:
		goto loc_826EB7FC;
	case 6:
		goto loc_826EB804;
	case 7:
		goto loc_826EB80C;
	case 8:
		goto loc_826EB8A4;
	case 9:
		goto loc_826EB83C;
	case 10:
		goto loc_826EB844;
	case 11:
		goto loc_826EB814;
	case 12:
		goto loc_826EB81C;
	case 13:
		goto loc_826EB824;
	case 14:
		goto loc_826EB82C;
	case 15:
		goto loc_826EB834;
	case 16:
		goto loc_826EB864;
	case 17:
		goto loc_826EB8A4;
	case 18:
		goto loc_826EB84C;
	case 19:
		goto loc_826EB854;
	case 20:
		goto loc_826EB85C;
	case 21:
		goto loc_826EB8A4;
	case 22:
		goto loc_826EB8A4;
	case 23:
		goto loc_826EB8A4;
	case 24:
		goto loc_826EB8A4;
	case 25:
		goto loc_826EB7F4;
	case 26:
		goto loc_826EB7E4;
	case 27:
		goto loc_826EB874;
	case 28:
		goto loc_826EB8A4;
	case 29:
		goto loc_826EB884;
	case 30:
		goto loc_826EB88C;
	case 31:
		goto loc_826EB894;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EB7D4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826EB7DC:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EB7E4:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EB7EC:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EB7F4:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EB7FC:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826EB804:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826EB80C:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826EB814:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826EB81C:
	// li r3,9
	ctx.r3.s64 = 9;
	// blr 
	return;
loc_826EB824:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_826EB82C:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_826EB834:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_826EB83C:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_826EB844:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_826EB84C:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_826EB854:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_826EB85C:
	// li r3,17
	ctx.r3.s64 = 17;
	// blr 
	return;
loc_826EB864:
	// li r3,18
	ctx.r3.s64 = 18;
	// blr 
	return;
loc_826EB86C:
	// li r3,19
	ctx.r3.s64 = 19;
	// blr 
	return;
loc_826EB874:
	// li r3,20
	ctx.r3.s64 = 20;
	// blr 
	return;
loc_826EB87C:
	// li r3,21
	ctx.r3.s64 = 21;
	// blr 
	return;
loc_826EB884:
	// li r3,22
	ctx.r3.s64 = 22;
	// blr 
	return;
loc_826EB88C:
	// li r3,23
	ctx.r3.s64 = 23;
	// blr 
	return;
loc_826EB894:
	// li r3,24
	ctx.r3.s64 = 24;
	// blr 
	return;
loc_826EB89C:
	// li r3,25
	ctx.r3.s64 = 25;
	// blr 
	return;
loc_826EB8A4:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EB8B0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EB8B0) { __imp__sub_826EB8B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EB8B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26984
	ctx.r3.s64 = ctx.r11.s64 + -26984;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EB8C0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EB8C0) { __imp__sub_826EB8C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EB8C0) {
	PPC_FUNC_PROLOGUE();
	// li r3,161
	ctx.r3.s64 = 161;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EB8C8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EB8C8) { __imp__sub_826EB8C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EB8C8) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826eb718
	sub_826EB718(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EB8D0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EB8D0) { __imp__sub_826EB8D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EB8D0) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826eb5b0
	sub_826EB5B0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EB8D8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EB8D8) { __imp__sub_826EB8D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EB8D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826eba64
	if (ctx.cr6.gt) goto loc_826EBA64;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826eba5c
	if (ctx.cr6.eq) goto loc_826EBA5C;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,31
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 31, ctx.xer);
	// bgt cr6,0x826eba64
	if (ctx.cr6.gt) goto loc_826EBA64;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-18156
	ctx.r12.s64 = ctx.r12.s64 + -18156;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EB9AC;
	case 1:
		goto loc_826EBA2C;
	case 2:
		goto loc_826EBA3C;
	case 3:
		goto loc_826EB994;
	case 4:
		goto loc_826EB99C;
	case 5:
		goto loc_826EB9BC;
	case 6:
		goto loc_826EB9C4;
	case 7:
		goto loc_826EB9CC;
	case 8:
		goto loc_826EBA64;
	case 9:
		goto loc_826EB9FC;
	case 10:
		goto loc_826EBA04;
	case 11:
		goto loc_826EB9D4;
	case 12:
		goto loc_826EB9DC;
	case 13:
		goto loc_826EB9E4;
	case 14:
		goto loc_826EB9EC;
	case 15:
		goto loc_826EB9F4;
	case 16:
		goto loc_826EBA64;
	case 17:
		goto loc_826EBA64;
	case 18:
		goto loc_826EBA0C;
	case 19:
		goto loc_826EBA14;
	case 20:
		goto loc_826EBA1C;
	case 21:
		goto loc_826EBA24;
	case 22:
		goto loc_826EBA64;
	case 23:
		goto loc_826EBA64;
	case 24:
		goto loc_826EBA64;
	case 25:
		goto loc_826EB9B4;
	case 26:
		goto loc_826EB9A4;
	case 27:
		goto loc_826EBA34;
	case 28:
		goto loc_826EBA64;
	case 29:
		goto loc_826EBA44;
	case 30:
		goto loc_826EBA4C;
	case 31:
		goto loc_826EBA54;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EB994:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826EB99C:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EB9A4:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EB9AC:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EB9B4:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EB9BC:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826EB9C4:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826EB9CC:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826EB9D4:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826EB9DC:
	// li r3,9
	ctx.r3.s64 = 9;
	// blr 
	return;
loc_826EB9E4:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_826EB9EC:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_826EB9F4:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_826EB9FC:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_826EBA04:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_826EBA0C:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_826EBA14:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_826EBA1C:
	// li r3,17
	ctx.r3.s64 = 17;
	// blr 
	return;
loc_826EBA24:
	// li r3,18
	ctx.r3.s64 = 18;
	// blr 
	return;
loc_826EBA2C:
	// li r3,19
	ctx.r3.s64 = 19;
	// blr 
	return;
loc_826EBA34:
	// li r3,20
	ctx.r3.s64 = 20;
	// blr 
	return;
loc_826EBA3C:
	// li r3,21
	ctx.r3.s64 = 21;
	// blr 
	return;
loc_826EBA44:
	// li r3,22
	ctx.r3.s64 = 22;
	// blr 
	return;
loc_826EBA4C:
	// li r3,23
	ctx.r3.s64 = 23;
	// blr 
	return;
loc_826EBA54:
	// li r3,24
	ctx.r3.s64 = 24;
	// blr 
	return;
loc_826EBA5C:
	// li r3,25
	ctx.r3.s64 = 25;
	// blr 
	return;
loc_826EBA64:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBA70);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBA70) { __imp__sub_826EBA70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBA70) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26952
	ctx.r3.s64 = ctx.r11.s64 + -26952;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBA80);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBA80) { __imp__sub_826EBA80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBA80) {
	PPC_FUNC_PROLOGUE();
	// li r3,162
	ctx.r3.s64 = 162;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBA88);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBA88) { __imp__sub_826EBA88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBA88) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826eb8d8
	sub_826EB8D8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBA90);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBA90) { __imp__sub_826EBA90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBA90) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26920
	ctx.r3.s64 = ctx.r11.s64 + -26920;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBAA0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBAA0) { __imp__sub_826EBAA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBAA0) {
	PPC_FUNC_PROLOGUE();
	// li r3,163
	ctx.r3.s64 = 163;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBAA8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBAA8) { __imp__sub_826EBAA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBAA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26888
	ctx.r3.s64 = ctx.r11.s64 + -26888;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBAB8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBAB8) { __imp__sub_826EBAB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBAB8) {
	PPC_FUNC_PROLOGUE();
	// li r3,164
	ctx.r3.s64 = 164;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBAC0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBAC0) { __imp__sub_826EBAC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBAC0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26856
	ctx.r3.s64 = ctx.r11.s64 + -26856;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBAD0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBAD0) { __imp__sub_826EBAD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBAD0) {
	PPC_FUNC_PROLOGUE();
	// li r3,165
	ctx.r3.s64 = 165;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBAD8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBAD8) { __imp__sub_826EBAD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBAD8) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,25
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 25, ctx.xer);
	// bgt cr6,0x826ebb70
	if (ctx.cr6.gt) goto loc_826EBB70;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-17672
	ctx.r12.s64 = ctx.r12.s64 + -17672;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826EBB60;
	case 1:
		goto loc_826EBB60;
	case 2:
		goto loc_826EBB60;
	case 3:
		goto loc_826EBB68;
	case 4:
		goto loc_826EBB68;
	case 5:
		goto loc_826EBB60;
	case 6:
		goto loc_826EBB60;
	case 7:
		goto loc_826EBB60;
	case 8:
		goto loc_826EBB60;
	case 9:
		goto loc_826EBB60;
	case 10:
		goto loc_826EBB60;
	case 11:
		goto loc_826EBB60;
	case 12:
		goto loc_826EBB60;
	case 13:
		goto loc_826EBB68;
	case 14:
		goto loc_826EBB68;
	case 15:
		goto loc_826EBB68;
	case 16:
		goto loc_826EBB68;
	case 17:
		goto loc_826EBB68;
	case 18:
		goto loc_826EBB60;
	case 19:
		goto loc_826EBB60;
	case 20:
		goto loc_826EBB60;
	case 21:
		goto loc_826EBB60;
	case 22:
		goto loc_826EBB60;
	case 23:
		goto loc_826EBB60;
	case 24:
		goto loc_826EBB60;
	case 25:
		goto loc_826EBB68;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EBB60:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EBB68:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EBB70:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBB78);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBB78) { __imp__sub_826EBB78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBB78) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26824
	ctx.r3.s64 = ctx.r11.s64 + -26824;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBB88);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBB88) { __imp__sub_826EBB88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBB88) {
	PPC_FUNC_PROLOGUE();
	// li r3,166
	ctx.r3.s64 = 166;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBB90);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBB90) { __imp__sub_826EBB90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBB90) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bl 0x826ebad8
	ctx.lr = 0x826EBBA4;
	sub_826EBAD8(ctx, base);
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826ebc2c
	if (ctx.cr6.gt) goto loc_826EBC2C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-17464
	ctx.r12.s64 = ctx.r12.s64 + -17464;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EBBDC;
	case 1:
		goto loc_826EBBF0;
	case 2:
		goto loc_826EBC18;
	case 3:
		goto loc_826EBC2C;
	case 4:
		goto loc_826EBC04;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EBBDC:
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
loc_826EBBF0:
	// li r3,1
	ctx.r3.s64 = 1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826EBC04:
	// li r3,2
	ctx.r3.s64 = 2;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826EBC18:
	// li r3,3
	ctx.r3.s64 = 3;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826EBC2C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBC40);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBC40) { __imp__sub_826EBC40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBC40) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826ebd0c
	if (ctx.cr6.gt) goto loc_826EBD0C;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826ebcfc
	if (ctx.cr6.eq) goto loc_826EBCFC;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,31
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 31, ctx.xer);
	// bgt cr6,0x826ebd0c
	if (ctx.cr6.gt) goto loc_826EBD0C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-17284
	ctx.r12.s64 = ctx.r12.s64 + -17284;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EBCFC;
	case 1:
		goto loc_826EBCFC;
	case 2:
		goto loc_826EBCFC;
	case 3:
		goto loc_826EBCFC;
	case 4:
		goto loc_826EBCFC;
	case 5:
		goto loc_826EBCFC;
	case 6:
		goto loc_826EBCFC;
	case 7:
		goto loc_826EBCFC;
	case 8:
		goto loc_826EBD0C;
	case 9:
		goto loc_826EBCFC;
	case 10:
		goto loc_826EBCFC;
	case 11:
		goto loc_826EBCFC;
	case 12:
		goto loc_826EBCFC;
	case 13:
		goto loc_826EBCFC;
	case 14:
		goto loc_826EBCFC;
	case 15:
		goto loc_826EBCFC;
	case 16:
		goto loc_826EBD0C;
	case 17:
		goto loc_826EBD0C;
	case 18:
		goto loc_826EBCFC;
	case 19:
		goto loc_826EBCFC;
	case 20:
		goto loc_826EBCFC;
	case 21:
		goto loc_826EBD04;
	case 22:
		goto loc_826EBD0C;
	case 23:
		goto loc_826EBD0C;
	case 24:
		goto loc_826EBD0C;
	case 25:
		goto loc_826EBCFC;
	case 26:
		goto loc_826EBCFC;
	case 27:
		goto loc_826EBCFC;
	case 28:
		goto loc_826EBD0C;
	case 29:
		goto loc_826EBCFC;
	case 30:
		goto loc_826EBCFC;
	case 31:
		goto loc_826EBCFC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EBCFC:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EBD04:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EBD0C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBD18);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBD18) { __imp__sub_826EBD18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBD18) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26792
	ctx.r3.s64 = ctx.r11.s64 + -26792;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBD28);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBD28) { __imp__sub_826EBD28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBD28) {
	PPC_FUNC_PROLOGUE();
	// li r3,168
	ctx.r3.s64 = 168;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBD30);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBD30) { __imp__sub_826EBD30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBD30) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,25
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 25, ctx.xer);
	// bgt cr6,0x826ebe8c
	if (ctx.cr6.gt) goto loc_826EBE8C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-17072
	ctx.r12.s64 = ctx.r12.s64 + -17072;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826EBDB8;
	case 1:
		goto loc_826EBDC0;
	case 2:
		goto loc_826EBDC8;
	case 3:
		goto loc_826EBDD0;
	case 4:
		goto loc_826EBDD8;
	case 5:
		goto loc_826EBDE0;
	case 6:
		goto loc_826EBDE8;
	case 7:
		goto loc_826EBDF0;
	case 8:
		goto loc_826EBDF8;
	case 9:
		goto loc_826EBE00;
	case 10:
		goto loc_826EBE08;
	case 11:
		goto loc_826EBE10;
	case 12:
		goto loc_826EBE18;
	case 13:
		goto loc_826EBE20;
	case 14:
		goto loc_826EBE28;
	case 15:
		goto loc_826EBE30;
	case 16:
		goto loc_826EBE38;
	case 17:
		goto loc_826EBE40;
	case 18:
		goto loc_826EBE48;
	case 19:
		goto loc_826EBE50;
	case 20:
		goto loc_826EBE58;
	case 21:
		goto loc_826EBE60;
	case 22:
		goto loc_826EBE68;
	case 23:
		goto loc_826EBE70;
	case 24:
		goto loc_826EBE78;
	case 25:
		goto loc_826EBE80;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EBDB8:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EBDC0:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826EBDC8:
	// li r3,27
	ctx.r3.s64 = 27;
	// blr 
	return;
loc_826EBDD0:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EBDD8:
	// li r3,26
	ctx.r3.s64 = 26;
	// blr 
	return;
loc_826EBDE0:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826EBDE8:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826EBDF0:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826EBDF8:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_826EBE00:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_826EBE08:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_826EBE10:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_826EBE18:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_826EBE20:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_826EBE28:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_826EBE30:
	// li r3,19
	ctx.r3.s64 = 19;
	// blr 
	return;
loc_826EBE38:
	// li r3,20
	ctx.r3.s64 = 20;
	// blr 
	return;
loc_826EBE40:
	// li r3,21
	ctx.r3.s64 = 21;
	// blr 
	return;
loc_826EBE48:
	// li r3,22
	ctx.r3.s64 = 22;
	// blr 
	return;
loc_826EBE50:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EBE58:
	// li r3,28
	ctx.r3.s64 = 28;
	// blr 
	return;
loc_826EBE60:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EBE68:
	// li r3,30
	ctx.r3.s64 = 30;
	// blr 
	return;
loc_826EBE70:
	// li r3,31
	ctx.r3.s64 = 31;
	// blr 
	return;
loc_826EBE78:
	// li r3,32
	ctx.r3.s64 = 32;
	// blr 
	return;
loc_826EBE80:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826EBE8C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBE98);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBE98) { __imp__sub_826EBE98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBE98) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26760
	ctx.r3.s64 = ctx.r11.s64 + -26760;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBEA8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBEA8) { __imp__sub_826EBEA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBEA8) {
	PPC_FUNC_PROLOGUE();
	// li r3,167
	ctx.r3.s64 = 167;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBEB0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBEB0) { __imp__sub_826EBEB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBEB0) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826ebd30
	sub_826EBD30(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBEB8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBEB8) { __imp__sub_826EBEB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBEB8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26728
	ctx.r3.s64 = ctx.r11.s64 + -26728;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBEC8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBEC8) { __imp__sub_826EBEC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBEC8) {
	PPC_FUNC_PROLOGUE();
	// li r3,169
	ctx.r3.s64 = 169;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBED0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBED0) { __imp__sub_826EBED0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBED0) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f71e0
	sub_826F71E0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBED8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBED8) { __imp__sub_826EBED8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBED8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26696
	ctx.r3.s64 = ctx.r11.s64 + -26696;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBEE8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBEE8) { __imp__sub_826EBEE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBEE8) {
	PPC_FUNC_PROLOGUE();
	// li r3,170
	ctx.r3.s64 = 170;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBEF0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBEF0) { __imp__sub_826EBEF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBEF0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26664
	ctx.r3.s64 = ctx.r11.s64 + -26664;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBF00);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBF00) { __imp__sub_826EBF00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBF00) {
	PPC_FUNC_PROLOGUE();
	// li r3,171
	ctx.r3.s64 = 171;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBF08);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBF08) { __imp__sub_826EBF08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBF08) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26632
	ctx.r3.s64 = ctx.r11.s64 + -26632;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBF18);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBF18) { __imp__sub_826EBF18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBF18) {
	PPC_FUNC_PROLOGUE();
	// li r3,172
	ctx.r3.s64 = 172;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBF20);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBF20) { __imp__sub_826EBF20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBF20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26600
	ctx.r3.s64 = ctx.r11.s64 + -26600;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBF30);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBF30) { __imp__sub_826EBF30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBF30) {
	PPC_FUNC_PROLOGUE();
	// li r3,173
	ctx.r3.s64 = 173;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBF38);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBF38) { __imp__sub_826EBF38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBF38) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26568
	ctx.r3.s64 = ctx.r11.s64 + -26568;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBF48);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBF48) { __imp__sub_826EBF48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBF48) {
	PPC_FUNC_PROLOGUE();
	// li r3,174
	ctx.r3.s64 = 174;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBF50);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBF50) { __imp__sub_826EBF50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBF50) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26536
	ctx.r3.s64 = ctx.r11.s64 + -26536;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBF60);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBF60) { __imp__sub_826EBF60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBF60) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26504
	ctx.r3.s64 = ctx.r11.s64 + -26504;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBF70);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBF70) { __imp__sub_826EBF70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBF70) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26472
	ctx.r3.s64 = ctx.r11.s64 + -26472;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EBF80);  /* forward decl */
PPC_WEAK_FUNC(sub_826EBF80) { __imp__sub_826EBF80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EBF80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826ec044
	if (ctx.cr6.gt) goto loc_826EC044;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826ec03c
	if (ctx.cr6.eq) goto loc_826EC03C;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,31
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 31, ctx.xer);
	// bgt cr6,0x826ec044
	if (ctx.cr6.gt) goto loc_826EC044;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-16452
	ctx.r12.s64 = ctx.r12.s64 + -16452;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EC03C;
	case 1:
		goto loc_826EC03C;
	case 2:
		goto loc_826EC03C;
	case 3:
		goto loc_826EC03C;
	case 4:
		goto loc_826EC044;
	case 5:
		goto loc_826EC03C;
	case 6:
		goto loc_826EC03C;
	case 7:
		goto loc_826EC03C;
	case 8:
		goto loc_826EC044;
	case 9:
		goto loc_826EC03C;
	case 10:
		goto loc_826EC03C;
	case 11:
		goto loc_826EC03C;
	case 12:
		goto loc_826EC03C;
	case 13:
		goto loc_826EC03C;
	case 14:
		goto loc_826EC03C;
	case 15:
		goto loc_826EC03C;
	case 16:
		goto loc_826EC044;
	case 17:
		goto loc_826EC044;
	case 18:
		goto loc_826EC03C;
	case 19:
		goto loc_826EC03C;
	case 20:
		goto loc_826EC03C;
	case 21:
		goto loc_826EC03C;
	case 22:
		goto loc_826EC044;
	case 23:
		goto loc_826EC044;
	case 24:
		goto loc_826EC044;
	case 25:
		goto loc_826EC03C;
	case 26:
		goto loc_826EC044;
	case 27:
		goto loc_826EC03C;
	case 28:
		goto loc_826EC044;
	case 29:
		goto loc_826EC03C;
	case 30:
		goto loc_826EC03C;
	case 31:
		goto loc_826EC03C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EC03C:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EC044:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC050);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC050) { __imp__sub_826EC050(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC050) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26440
	ctx.r3.s64 = ctx.r11.s64 + -26440;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC060);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC060) { __imp__sub_826EC060(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC060) {
	PPC_FUNC_PROLOGUE();
	// li r3,251
	ctx.r3.s64 = 251;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC068);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC068) { __imp__sub_826EC068(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC068) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26404
	ctx.r3.s64 = ctx.r11.s64 + -26404;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC078);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC078) { __imp__sub_826EC078(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC078) {
	PPC_FUNC_PROLOGUE();
	// li r3,159
	ctx.r3.s64 = 159;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC080);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC080) { __imp__sub_826EC080(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC080) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26368
	ctx.r3.s64 = ctx.r11.s64 + -26368;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC090);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC090) { __imp__sub_826EC090(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC090) {
	PPC_FUNC_PROLOGUE();
	// li r3,160
	ctx.r3.s64 = 160;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC098);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC098) { __imp__sub_826EC098(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC098) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26332
	ctx.r3.s64 = ctx.r11.s64 + -26332;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC0A8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC0A8) { __imp__sub_826EC0A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC0A8) {
	PPC_FUNC_PROLOGUE();
	// li r3,180
	ctx.r3.s64 = 180;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC0B0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC0B0) { __imp__sub_826EC0B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC0B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26296
	ctx.r3.s64 = ctx.r11.s64 + -26296;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC0C0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC0C0) { __imp__sub_826EC0C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC0C0) {
	PPC_FUNC_PROLOGUE();
	// li r3,181
	ctx.r3.s64 = 181;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC0C8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC0C8) { __imp__sub_826EC0C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC0C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26260
	ctx.r3.s64 = ctx.r11.s64 + -26260;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC0D8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC0D8) { __imp__sub_826EC0D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC0D8) {
	PPC_FUNC_PROLOGUE();
	// li r3,183
	ctx.r3.s64 = 183;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC0E0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC0E0) { __imp__sub_826EC0E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC0E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826ec25c
	if (ctx.cr6.gt) goto loc_826EC25C;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826ec254
	if (ctx.cr6.eq) goto loc_826EC254;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,31
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 31, ctx.xer);
	// bgt cr6,0x826ec25c
	if (ctx.cr6.gt) goto loc_826EC25C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-16100
	ctx.r12.s64 = ctx.r12.s64 + -16100;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EC1A4;
	case 1:
		goto loc_826EC224;
	case 2:
		goto loc_826EC234;
	case 3:
		goto loc_826EC19C;
	case 4:
		goto loc_826EC25C;
	case 5:
		goto loc_826EC1B4;
	case 6:
		goto loc_826EC1BC;
	case 7:
		goto loc_826EC1C4;
	case 8:
		goto loc_826EC25C;
	case 9:
		goto loc_826EC1F4;
	case 10:
		goto loc_826EC1FC;
	case 11:
		goto loc_826EC1CC;
	case 12:
		goto loc_826EC1D4;
	case 13:
		goto loc_826EC1DC;
	case 14:
		goto loc_826EC1E4;
	case 15:
		goto loc_826EC1EC;
	case 16:
		goto loc_826EC25C;
	case 17:
		goto loc_826EC25C;
	case 18:
		goto loc_826EC204;
	case 19:
		goto loc_826EC20C;
	case 20:
		goto loc_826EC214;
	case 21:
		goto loc_826EC21C;
	case 22:
		goto loc_826EC25C;
	case 23:
		goto loc_826EC25C;
	case 24:
		goto loc_826EC25C;
	case 25:
		goto loc_826EC1AC;
	case 26:
		goto loc_826EC25C;
	case 27:
		goto loc_826EC22C;
	case 28:
		goto loc_826EC25C;
	case 29:
		goto loc_826EC23C;
	case 30:
		goto loc_826EC244;
	case 31:
		goto loc_826EC24C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EC19C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826EC1A4:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EC1AC:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EC1B4:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EC1BC:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EC1C4:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826EC1CC:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826EC1D4:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826EC1DC:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826EC1E4:
	// li r3,9
	ctx.r3.s64 = 9;
	// blr 
	return;
loc_826EC1EC:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_826EC1F4:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_826EC1FC:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_826EC204:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_826EC20C:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_826EC214:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_826EC21C:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_826EC224:
	// li r3,17
	ctx.r3.s64 = 17;
	// blr 
	return;
loc_826EC22C:
	// li r3,18
	ctx.r3.s64 = 18;
	// blr 
	return;
loc_826EC234:
	// li r3,19
	ctx.r3.s64 = 19;
	// blr 
	return;
loc_826EC23C:
	// li r3,20
	ctx.r3.s64 = 20;
	// blr 
	return;
loc_826EC244:
	// li r3,21
	ctx.r3.s64 = 21;
	// blr 
	return;
loc_826EC24C:
	// li r3,22
	ctx.r3.s64 = 22;
	// blr 
	return;
loc_826EC254:
	// li r3,23
	ctx.r3.s64 = 23;
	// blr 
	return;
loc_826EC25C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC268);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC268) { __imp__sub_826EC268(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC268) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26224
	ctx.r3.s64 = ctx.r11.s64 + -26224;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC278);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC278) { __imp__sub_826EC278(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC278) {
	PPC_FUNC_PROLOGUE();
	// li r3,184
	ctx.r3.s64 = 184;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC280);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC280) { __imp__sub_826EC280(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC280) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826ec0e0
	sub_826EC0E0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC288);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC288) { __imp__sub_826EC288(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC288) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26188
	ctx.r3.s64 = ctx.r11.s64 + -26188;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC298);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC298) { __imp__sub_826EC298(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC298) {
	PPC_FUNC_PROLOGUE();
	// li r3,185
	ctx.r3.s64 = 185;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC2A0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC2A0) { __imp__sub_826EC2A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC2A0) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,23
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 23, ctx.xer);
	// bgt cr6,0x826ec3e4
	if (ctx.cr6.gt) goto loc_826EC3E4;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-15680
	ctx.r12.s64 = ctx.r12.s64 + -15680;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826EC320;
	case 1:
		goto loc_826EC328;
	case 2:
		goto loc_826EC330;
	case 3:
		goto loc_826EC338;
	case 4:
		goto loc_826EC340;
	case 5:
		goto loc_826EC348;
	case 6:
		goto loc_826EC350;
	case 7:
		goto loc_826EC358;
	case 8:
		goto loc_826EC360;
	case 9:
		goto loc_826EC368;
	case 10:
		goto loc_826EC370;
	case 11:
		goto loc_826EC378;
	case 12:
		goto loc_826EC380;
	case 13:
		goto loc_826EC388;
	case 14:
		goto loc_826EC390;
	case 15:
		goto loc_826EC398;
	case 16:
		goto loc_826EC3A0;
	case 17:
		goto loc_826EC3A8;
	case 18:
		goto loc_826EC3B0;
	case 19:
		goto loc_826EC3B8;
	case 20:
		goto loc_826EC3C0;
	case 21:
		goto loc_826EC3C8;
	case 22:
		goto loc_826EC3D0;
	case 23:
		goto loc_826EC3D8;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EC320:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EC328:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EC330:
	// li r3,26
	ctx.r3.s64 = 26;
	// blr 
	return;
loc_826EC338:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826EC340:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826EC348:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826EC350:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_826EC358:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_826EC360:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_826EC368:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_826EC370:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_826EC378:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_826EC380:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_826EC388:
	// li r3,19
	ctx.r3.s64 = 19;
	// blr 
	return;
loc_826EC390:
	// li r3,20
	ctx.r3.s64 = 20;
	// blr 
	return;
loc_826EC398:
	// li r3,21
	ctx.r3.s64 = 21;
	// blr 
	return;
loc_826EC3A0:
	// li r3,22
	ctx.r3.s64 = 22;
	// blr 
	return;
loc_826EC3A8:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EC3B0:
	// li r3,28
	ctx.r3.s64 = 28;
	// blr 
	return;
loc_826EC3B8:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EC3C0:
	// li r3,30
	ctx.r3.s64 = 30;
	// blr 
	return;
loc_826EC3C8:
	// li r3,31
	ctx.r3.s64 = 31;
	// blr 
	return;
loc_826EC3D0:
	// li r3,32
	ctx.r3.s64 = 32;
	// blr 
	return;
loc_826EC3D8:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826EC3E4:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC3F0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC3F0) { __imp__sub_826EC3F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC3F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26152
	ctx.r3.s64 = ctx.r11.s64 + -26152;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC400);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC400) { __imp__sub_826EC400(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC400) {
	PPC_FUNC_PROLOGUE();
	// li r3,186
	ctx.r3.s64 = 186;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC408);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC408) { __imp__sub_826EC408(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC408) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826ec2a0
	sub_826EC2A0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC410);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC410) { __imp__sub_826EC410(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC410) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26116
	ctx.r3.s64 = ctx.r11.s64 + -26116;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC420);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC420) { __imp__sub_826EC420(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC420) {
	PPC_FUNC_PROLOGUE();
	// li r3,187
	ctx.r3.s64 = 187;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC428);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC428) { __imp__sub_826EC428(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC428) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26080
	ctx.r3.s64 = ctx.r11.s64 + -26080;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC438);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC438) { __imp__sub_826EC438(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC438) {
	PPC_FUNC_PROLOGUE();
	// li r3,188
	ctx.r3.s64 = 188;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC440);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC440) { __imp__sub_826EC440(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC440) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26044
	ctx.r3.s64 = ctx.r11.s64 + -26044;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC450);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC450) { __imp__sub_826EC450(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC450) {
	PPC_FUNC_PROLOGUE();
	// li r3,189
	ctx.r3.s64 = 189;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC458);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC458) { __imp__sub_826EC458(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC458) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-26008
	ctx.r3.s64 = ctx.r11.s64 + -26008;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC468);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC468) { __imp__sub_826EC468(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC468) {
	PPC_FUNC_PROLOGUE();
	// li r3,190
	ctx.r3.s64 = 190;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC470);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC470) { __imp__sub_826EC470(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC470) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25972
	ctx.r3.s64 = ctx.r11.s64 + -25972;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC480);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC480) { __imp__sub_826EC480(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC480) {
	PPC_FUNC_PROLOGUE();
	// li r3,191
	ctx.r3.s64 = 191;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC488);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC488) { __imp__sub_826EC488(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC488) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826ec54c
	if (ctx.cr6.gt) goto loc_826EC54C;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826ec544
	if (ctx.cr6.eq) goto loc_826EC544;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,31
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 31, ctx.xer);
	// bgt cr6,0x826ec54c
	if (ctx.cr6.gt) goto loc_826EC54C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-15164
	ctx.r12.s64 = ctx.r12.s64 + -15164;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EC544;
	case 1:
		goto loc_826EC544;
	case 2:
		goto loc_826EC544;
	case 3:
		goto loc_826EC544;
	case 4:
		goto loc_826EC54C;
	case 5:
		goto loc_826EC544;
	case 6:
		goto loc_826EC544;
	case 7:
		goto loc_826EC544;
	case 8:
		goto loc_826EC54C;
	case 9:
		goto loc_826EC544;
	case 10:
		goto loc_826EC544;
	case 11:
		goto loc_826EC544;
	case 12:
		goto loc_826EC544;
	case 13:
		goto loc_826EC544;
	case 14:
		goto loc_826EC544;
	case 15:
		goto loc_826EC544;
	case 16:
		goto loc_826EC544;
	case 17:
		goto loc_826EC54C;
	case 18:
		goto loc_826EC544;
	case 19:
		goto loc_826EC544;
	case 20:
		goto loc_826EC544;
	case 21:
		goto loc_826EC544;
	case 22:
		goto loc_826EC54C;
	case 23:
		goto loc_826EC54C;
	case 24:
		goto loc_826EC54C;
	case 25:
		goto loc_826EC544;
	case 26:
		goto loc_826EC54C;
	case 27:
		goto loc_826EC544;
	case 28:
		goto loc_826EC54C;
	case 29:
		goto loc_826EC544;
	case 30:
		goto loc_826EC544;
	case 31:
		goto loc_826EC544;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EC544:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EC54C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC558);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC558) { __imp__sub_826EC558(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC558) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,24
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 24, ctx.xer);
	// bgt cr6,0x826ec6a8
	if (ctx.cr6.gt) goto loc_826EC6A8;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-14984
	ctx.r12.s64 = ctx.r12.s64 + -14984;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826EC5DC;
	case 1:
		goto loc_826EC5E4;
	case 2:
		goto loc_826EC5EC;
	case 3:
		goto loc_826EC5F4;
	case 4:
		goto loc_826EC5FC;
	case 5:
		goto loc_826EC604;
	case 6:
		goto loc_826EC60C;
	case 7:
		goto loc_826EC614;
	case 8:
		goto loc_826EC61C;
	case 9:
		goto loc_826EC624;
	case 10:
		goto loc_826EC62C;
	case 11:
		goto loc_826EC634;
	case 12:
		goto loc_826EC63C;
	case 13:
		goto loc_826EC644;
	case 14:
		goto loc_826EC64C;
	case 15:
		goto loc_826EC654;
	case 16:
		goto loc_826EC65C;
	case 17:
		goto loc_826EC664;
	case 18:
		goto loc_826EC66C;
	case 19:
		goto loc_826EC674;
	case 20:
		goto loc_826EC67C;
	case 21:
		goto loc_826EC684;
	case 22:
		goto loc_826EC68C;
	case 23:
		goto loc_826EC694;
	case 24:
		goto loc_826EC69C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EC5DC:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EC5E4:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EC5EC:
	// li r3,26
	ctx.r3.s64 = 26;
	// blr 
	return;
loc_826EC5F4:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826EC5FC:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826EC604:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826EC60C:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_826EC614:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_826EC61C:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_826EC624:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_826EC62C:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_826EC634:
	// li r3,17
	ctx.r3.s64 = 17;
	// blr 
	return;
loc_826EC63C:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_826EC644:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_826EC64C:
	// li r3,19
	ctx.r3.s64 = 19;
	// blr 
	return;
loc_826EC654:
	// li r3,20
	ctx.r3.s64 = 20;
	// blr 
	return;
loc_826EC65C:
	// li r3,21
	ctx.r3.s64 = 21;
	// blr 
	return;
loc_826EC664:
	// li r3,22
	ctx.r3.s64 = 22;
	// blr 
	return;
loc_826EC66C:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EC674:
	// li r3,28
	ctx.r3.s64 = 28;
	// blr 
	return;
loc_826EC67C:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EC684:
	// li r3,30
	ctx.r3.s64 = 30;
	// blr 
	return;
loc_826EC68C:
	// li r3,31
	ctx.r3.s64 = 31;
	// blr 
	return;
loc_826EC694:
	// li r3,32
	ctx.r3.s64 = 32;
	// blr 
	return;
loc_826EC69C:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826EC6A8:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC6B0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC6B0) { __imp__sub_826EC6B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC6B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826ec834
	if (ctx.cr6.gt) goto loc_826EC834;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826ec82c
	if (ctx.cr6.eq) goto loc_826EC82C;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,31
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 31, ctx.xer);
	// bgt cr6,0x826ec834
	if (ctx.cr6.gt) goto loc_826EC834;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-14612
	ctx.r12.s64 = ctx.r12.s64 + -14612;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EC774;
	case 1:
		goto loc_826EC7FC;
	case 2:
		goto loc_826EC80C;
	case 3:
		goto loc_826EC76C;
	case 4:
		goto loc_826EC834;
	case 5:
		goto loc_826EC784;
	case 6:
		goto loc_826EC78C;
	case 7:
		goto loc_826EC794;
	case 8:
		goto loc_826EC834;
	case 9:
		goto loc_826EC7CC;
	case 10:
		goto loc_826EC7D4;
	case 11:
		goto loc_826EC79C;
	case 12:
		goto loc_826EC7A4;
	case 13:
		goto loc_826EC7AC;
	case 14:
		goto loc_826EC7B4;
	case 15:
		goto loc_826EC7BC;
	case 16:
		goto loc_826EC7C4;
	case 17:
		goto loc_826EC834;
	case 18:
		goto loc_826EC7DC;
	case 19:
		goto loc_826EC7E4;
	case 20:
		goto loc_826EC7EC;
	case 21:
		goto loc_826EC7F4;
	case 22:
		goto loc_826EC834;
	case 23:
		goto loc_826EC834;
	case 24:
		goto loc_826EC834;
	case 25:
		goto loc_826EC77C;
	case 26:
		goto loc_826EC834;
	case 27:
		goto loc_826EC804;
	case 28:
		goto loc_826EC834;
	case 29:
		goto loc_826EC814;
	case 30:
		goto loc_826EC81C;
	case 31:
		goto loc_826EC824;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EC76C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826EC774:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EC77C:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EC784:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EC78C:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EC794:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826EC79C:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826EC7A4:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826EC7AC:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826EC7B4:
	// li r3,9
	ctx.r3.s64 = 9;
	// blr 
	return;
loc_826EC7BC:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_826EC7C4:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_826EC7CC:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_826EC7D4:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_826EC7DC:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_826EC7E4:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_826EC7EC:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_826EC7F4:
	// li r3,17
	ctx.r3.s64 = 17;
	// blr 
	return;
loc_826EC7FC:
	// li r3,18
	ctx.r3.s64 = 18;
	// blr 
	return;
loc_826EC804:
	// li r3,19
	ctx.r3.s64 = 19;
	// blr 
	return;
loc_826EC80C:
	// li r3,20
	ctx.r3.s64 = 20;
	// blr 
	return;
loc_826EC814:
	// li r3,21
	ctx.r3.s64 = 21;
	// blr 
	return;
loc_826EC81C:
	// li r3,22
	ctx.r3.s64 = 22;
	// blr 
	return;
loc_826EC824:
	// li r3,23
	ctx.r3.s64 = 23;
	// blr 
	return;
loc_826EC82C:
	// li r3,24
	ctx.r3.s64 = 24;
	// blr 
	return;
loc_826EC834:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC840);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC840) { __imp__sub_826EC840(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC840) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,24
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 24, ctx.xer);
	// bgt cr6,0x826ec8d4
	if (ctx.cr6.gt) goto loc_826EC8D4;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-14240
	ctx.r12.s64 = ctx.r12.s64 + -14240;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826EC8C4;
	case 1:
		goto loc_826EC8C4;
	case 2:
		goto loc_826EC8C4;
	case 3:
		goto loc_826EC8CC;
	case 4:
		goto loc_826EC8CC;
	case 5:
		goto loc_826EC8CC;
	case 6:
		goto loc_826EC8CC;
	case 7:
		goto loc_826EC8CC;
	case 8:
		goto loc_826EC8CC;
	case 9:
		goto loc_826EC8CC;
	case 10:
		goto loc_826EC8CC;
	case 11:
		goto loc_826EC8CC;
	case 12:
		goto loc_826EC8C4;
	case 13:
		goto loc_826EC8C4;
	case 14:
		goto loc_826EC8C4;
	case 15:
		goto loc_826EC8C4;
	case 16:
		goto loc_826EC8C4;
	case 17:
		goto loc_826EC8CC;
	case 18:
		goto loc_826EC8CC;
	case 19:
		goto loc_826EC8CC;
	case 20:
		goto loc_826EC8CC;
	case 21:
		goto loc_826EC8CC;
	case 22:
		goto loc_826EC8CC;
	case 23:
		goto loc_826EC8CC;
	case 24:
		goto loc_826EC8C4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EC8C4:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EC8CC:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EC8D4:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC8E0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC8E0) { __imp__sub_826EC8E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC8E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25936
	ctx.r3.s64 = ctx.r11.s64 + -25936;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC8F0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC8F0) { __imp__sub_826EC8F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC8F0) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826ec6b0
	sub_826EC6B0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC8F8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC8F8) { __imp__sub_826EC8F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC8F8) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826ec558
	sub_826EC558(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC900);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC900) { __imp__sub_826EC900(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC900) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f7db0
	sub_826F7DB0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC908);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC908) { __imp__sub_826EC908(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC908) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bl 0x826ec840
	ctx.lr = 0x826EC91C;
	sub_826EC840(ctx, base);
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826ec9a4
	if (ctx.cr6.gt) goto loc_826EC9A4;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-14016
	ctx.r12.s64 = ctx.r12.s64 + -14016;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EC954;
	case 1:
		goto loc_826EC968;
	case 2:
		goto loc_826EC990;
	case 3:
		goto loc_826EC9A4;
	case 4:
		goto loc_826EC97C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EC954:
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
loc_826EC968:
	// li r3,1
	ctx.r3.s64 = 1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826EC97C:
	// li r3,2
	ctx.r3.s64 = 2;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826EC990:
	// li r3,3
	ctx.r3.s64 = 3;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826EC9A4:
	// li r3,-1
	ctx.r3.s64 = -1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC9B8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC9B8) { __imp__sub_826EC9B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC9B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25900
	ctx.r3.s64 = ctx.r11.s64 + -25900;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC9C8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC9C8) { __imp__sub_826EC9C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC9C8) {
	PPC_FUNC_PROLOGUE();
	// li r3,193
	ctx.r3.s64 = 193;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EC9D0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EC9D0) { __imp__sub_826EC9D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EC9D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826eca94
	if (ctx.cr6.gt) goto loc_826ECA94;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826eca8c
	if (ctx.cr6.eq) goto loc_826ECA8C;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,31
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 31, ctx.xer);
	// bgt cr6,0x826eca94
	if (ctx.cr6.gt) goto loc_826ECA94;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-13812
	ctx.r12.s64 = ctx.r12.s64 + -13812;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826ECA8C;
	case 1:
		goto loc_826ECA8C;
	case 2:
		goto loc_826ECA8C;
	case 3:
		goto loc_826ECA8C;
	case 4:
		goto loc_826ECA94;
	case 5:
		goto loc_826ECA8C;
	case 6:
		goto loc_826ECA8C;
	case 7:
		goto loc_826ECA8C;
	case 8:
		goto loc_826ECA94;
	case 9:
		goto loc_826ECA8C;
	case 10:
		goto loc_826ECA8C;
	case 11:
		goto loc_826ECA8C;
	case 12:
		goto loc_826ECA8C;
	case 13:
		goto loc_826ECA8C;
	case 14:
		goto loc_826ECA8C;
	case 15:
		goto loc_826ECA94;
	case 16:
		goto loc_826ECA94;
	case 17:
		goto loc_826ECA94;
	case 18:
		goto loc_826ECA8C;
	case 19:
		goto loc_826ECA8C;
	case 20:
		goto loc_826ECA8C;
	case 21:
		goto loc_826ECA8C;
	case 22:
		goto loc_826ECA94;
	case 23:
		goto loc_826ECA94;
	case 24:
		goto loc_826ECA94;
	case 25:
		goto loc_826ECA8C;
	case 26:
		goto loc_826ECA94;
	case 27:
		goto loc_826ECA8C;
	case 28:
		goto loc_826ECA94;
	case 29:
		goto loc_826ECA8C;
	case 30:
		goto loc_826ECA8C;
	case 31:
		goto loc_826ECA8C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826ECA8C:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826ECA94:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ECAA0);  /* forward decl */
PPC_WEAK_FUNC(sub_826ECAA0) { __imp__sub_826ECAA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ECAA0) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,22
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 22, ctx.xer);
	// bgt cr6,0x826ecbd8
	if (ctx.cr6.gt) goto loc_826ECBD8;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-13632
	ctx.r12.s64 = ctx.r12.s64 + -13632;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826ECB1C;
	case 1:
		goto loc_826ECB24;
	case 2:
		goto loc_826ECB2C;
	case 3:
		goto loc_826ECB34;
	case 4:
		goto loc_826ECB3C;
	case 5:
		goto loc_826ECB44;
	case 6:
		goto loc_826ECB4C;
	case 7:
		goto loc_826ECB54;
	case 8:
		goto loc_826ECB5C;
	case 9:
		goto loc_826ECB64;
	case 10:
		goto loc_826ECB6C;
	case 11:
		goto loc_826ECB74;
	case 12:
		goto loc_826ECB7C;
	case 13:
		goto loc_826ECB84;
	case 14:
		goto loc_826ECB8C;
	case 15:
		goto loc_826ECB94;
	case 16:
		goto loc_826ECB9C;
	case 17:
		goto loc_826ECBA4;
	case 18:
		goto loc_826ECBAC;
	case 19:
		goto loc_826ECBB4;
	case 20:
		goto loc_826ECBBC;
	case 21:
		goto loc_826ECBC4;
	case 22:
		goto loc_826ECBCC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826ECB1C:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826ECB24:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826ECB2C:
	// li r3,26
	ctx.r3.s64 = 26;
	// blr 
	return;
loc_826ECB34:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826ECB3C:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826ECB44:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826ECB4C:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_826ECB54:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_826ECB5C:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_826ECB64:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_826ECB6C:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_826ECB74:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_826ECB7C:
	// li r3,19
	ctx.r3.s64 = 19;
	// blr 
	return;
loc_826ECB84:
	// li r3,20
	ctx.r3.s64 = 20;
	// blr 
	return;
loc_826ECB8C:
	// li r3,21
	ctx.r3.s64 = 21;
	// blr 
	return;
loc_826ECB94:
	// li r3,22
	ctx.r3.s64 = 22;
	// blr 
	return;
loc_826ECB9C:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826ECBA4:
	// li r3,28
	ctx.r3.s64 = 28;
	// blr 
	return;
loc_826ECBAC:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826ECBB4:
	// li r3,30
	ctx.r3.s64 = 30;
	// blr 
	return;
loc_826ECBBC:
	// li r3,31
	ctx.r3.s64 = 31;
	// blr 
	return;
loc_826ECBC4:
	// li r3,32
	ctx.r3.s64 = 32;
	// blr 
	return;
loc_826ECBCC:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826ECBD8:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ECBE0);  /* forward decl */
PPC_WEAK_FUNC(sub_826ECBE0) { __imp__sub_826ECBE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ECBE0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826ecd54
	if (ctx.cr6.gt) goto loc_826ECD54;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826ecd4c
	if (ctx.cr6.eq) goto loc_826ECD4C;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,31
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 31, ctx.xer);
	// bgt cr6,0x826ecd54
	if (ctx.cr6.gt) goto loc_826ECD54;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-13284
	ctx.r12.s64 = ctx.r12.s64 + -13284;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826ECCA4;
	case 1:
		goto loc_826ECD1C;
	case 2:
		goto loc_826ECD2C;
	case 3:
		goto loc_826ECC9C;
	case 4:
		goto loc_826ECD54;
	case 5:
		goto loc_826ECCB4;
	case 6:
		goto loc_826ECCBC;
	case 7:
		goto loc_826ECCC4;
	case 8:
		goto loc_826ECD54;
	case 9:
		goto loc_826ECCEC;
	case 10:
		goto loc_826ECCF4;
	case 11:
		goto loc_826ECCCC;
	case 12:
		goto loc_826ECCD4;
	case 13:
		goto loc_826ECCDC;
	case 14:
		goto loc_826ECCE4;
	case 15:
		goto loc_826ECD54;
	case 16:
		goto loc_826ECD54;
	case 17:
		goto loc_826ECD54;
	case 18:
		goto loc_826ECCFC;
	case 19:
		goto loc_826ECD04;
	case 20:
		goto loc_826ECD0C;
	case 21:
		goto loc_826ECD14;
	case 22:
		goto loc_826ECD54;
	case 23:
		goto loc_826ECD54;
	case 24:
		goto loc_826ECD54;
	case 25:
		goto loc_826ECCAC;
	case 26:
		goto loc_826ECD54;
	case 27:
		goto loc_826ECD24;
	case 28:
		goto loc_826ECD54;
	case 29:
		goto loc_826ECD34;
	case 30:
		goto loc_826ECD3C;
	case 31:
		goto loc_826ECD44;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826ECC9C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826ECCA4:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826ECCAC:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826ECCB4:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826ECCBC:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826ECCC4:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826ECCCC:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826ECCD4:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826ECCDC:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826ECCE4:
	// li r3,9
	ctx.r3.s64 = 9;
	// blr 
	return;
loc_826ECCEC:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_826ECCF4:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_826ECCFC:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_826ECD04:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_826ECD0C:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_826ECD14:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_826ECD1C:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_826ECD24:
	// li r3,17
	ctx.r3.s64 = 17;
	// blr 
	return;
loc_826ECD2C:
	// li r3,18
	ctx.r3.s64 = 18;
	// blr 
	return;
loc_826ECD34:
	// li r3,19
	ctx.r3.s64 = 19;
	// blr 
	return;
loc_826ECD3C:
	// li r3,20
	ctx.r3.s64 = 20;
	// blr 
	return;
loc_826ECD44:
	// li r3,21
	ctx.r3.s64 = 21;
	// blr 
	return;
loc_826ECD4C:
	// li r3,22
	ctx.r3.s64 = 22;
	// blr 
	return;
loc_826ECD54:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ECD60);  /* forward decl */
PPC_WEAK_FUNC(sub_826ECD60) { __imp__sub_826ECD60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ECD60) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,22
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 22, ctx.xer);
	// bgt cr6,0x826ecdec
	if (ctx.cr6.gt) goto loc_826ECDEC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-12928
	ctx.r12.s64 = ctx.r12.s64 + -12928;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826ECDDC;
	case 1:
		goto loc_826ECDDC;
	case 2:
		goto loc_826ECDDC;
	case 3:
		goto loc_826ECDE4;
	case 4:
		goto loc_826ECDE4;
	case 5:
		goto loc_826ECDE4;
	case 6:
		goto loc_826ECDE4;
	case 7:
		goto loc_826ECDE4;
	case 8:
		goto loc_826ECDE4;
	case 9:
		goto loc_826ECDE4;
	case 10:
		goto loc_826ECDDC;
	case 11:
		goto loc_826ECDDC;
	case 12:
		goto loc_826ECDDC;
	case 13:
		goto loc_826ECDDC;
	case 14:
		goto loc_826ECDDC;
	case 15:
		goto loc_826ECDE4;
	case 16:
		goto loc_826ECDE4;
	case 17:
		goto loc_826ECDE4;
	case 18:
		goto loc_826ECDE4;
	case 19:
		goto loc_826ECDE4;
	case 20:
		goto loc_826ECDE4;
	case 21:
		goto loc_826ECDE4;
	case 22:
		goto loc_826ECDDC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826ECDDC:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826ECDE4:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826ECDEC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ECDF8);  /* forward decl */
PPC_WEAK_FUNC(sub_826ECDF8) { __imp__sub_826ECDF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ECDF8) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,22
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 22, ctx.xer);
	// bgt cr6,0x826ece84
	if (ctx.cr6.gt) goto loc_826ECE84;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-12776
	ctx.r12.s64 = ctx.r12.s64 + -12776;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826ECE74;
	case 1:
		goto loc_826ECE74;
	case 2:
		goto loc_826ECE74;
	case 3:
		goto loc_826ECE7C;
	case 4:
		goto loc_826ECE7C;
	case 5:
		goto loc_826ECE7C;
	case 6:
		goto loc_826ECE7C;
	case 7:
		goto loc_826ECE7C;
	case 8:
		goto loc_826ECE7C;
	case 9:
		goto loc_826ECE7C;
	case 10:
		goto loc_826ECE74;
	case 11:
		goto loc_826ECE74;
	case 12:
		goto loc_826ECE74;
	case 13:
		goto loc_826ECE74;
	case 14:
		goto loc_826ECE74;
	case 15:
		goto loc_826ECE7C;
	case 16:
		goto loc_826ECE7C;
	case 17:
		goto loc_826ECE7C;
	case 18:
		goto loc_826ECE7C;
	case 19:
		goto loc_826ECE7C;
	case 20:
		goto loc_826ECE7C;
	case 21:
		goto loc_826ECE7C;
	case 22:
		goto loc_826ECE74;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826ECE74:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826ECE7C:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826ECE84:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ECE90);  /* forward decl */
PPC_WEAK_FUNC(sub_826ECE90) { __imp__sub_826ECE90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ECE90) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25864
	ctx.r3.s64 = ctx.r11.s64 + -25864;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ECEA0);  /* forward decl */
PPC_WEAK_FUNC(sub_826ECEA0) { __imp__sub_826ECEA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ECEA0) {
	PPC_FUNC_PROLOGUE();
	// li r3,199
	ctx.r3.s64 = 199;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ECEA8);  /* forward decl */
PPC_WEAK_FUNC(sub_826ECEA8) { __imp__sub_826ECEA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ECEA8) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826ecbe0
	sub_826ECBE0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ECEB0);  /* forward decl */
PPC_WEAK_FUNC(sub_826ECEB0) { __imp__sub_826ECEB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ECEB0) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826ecaa0
	sub_826ECAA0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ECEB8);  /* forward decl */
PPC_WEAK_FUNC(sub_826ECEB8) { __imp__sub_826ECEB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ECEB8) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826ecdf8
	sub_826ECDF8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ECEC0);  /* forward decl */
PPC_WEAK_FUNC(sub_826ECEC0) { __imp__sub_826ECEC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ECEC0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bl 0x826ecd60
	ctx.lr = 0x826ECED4;
	sub_826ECD60(ctx, base);
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826ecf5c
	if (ctx.cr6.gt) goto loc_826ECF5C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-12552
	ctx.r12.s64 = ctx.r12.s64 + -12552;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826ECF0C;
	case 1:
		goto loc_826ECF20;
	case 2:
		goto loc_826ECF48;
	case 3:
		goto loc_826ECF5C;
	case 4:
		goto loc_826ECF34;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826ECF0C:
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
loc_826ECF20:
	// li r3,1
	ctx.r3.s64 = 1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826ECF34:
	// li r3,2
	ctx.r3.s64 = 2;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826ECF48:
	// li r3,3
	ctx.r3.s64 = 3;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826ECF5C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ECF70);  /* forward decl */
PPC_WEAK_FUNC(sub_826ECF70) { __imp__sub_826ECF70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ECF70) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25828
	ctx.r3.s64 = ctx.r11.s64 + -25828;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ECF80);  /* forward decl */
PPC_WEAK_FUNC(sub_826ECF80) { __imp__sub_826ECF80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ECF80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25792
	ctx.r3.s64 = ctx.r11.s64 + -25792;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ECF90);  /* forward decl */
PPC_WEAK_FUNC(sub_826ECF90) { __imp__sub_826ECF90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ECF90) {
	PPC_FUNC_PROLOGUE();
	// li r3,201
	ctx.r3.s64 = 201;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ECF98);  /* forward decl */
PPC_WEAK_FUNC(sub_826ECF98) { __imp__sub_826ECF98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ECF98) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,23
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 23, ctx.xer);
	// bgt cr6,0x826ed0dc
	if (ctx.cr6.gt) goto loc_826ED0DC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-12360
	ctx.r12.s64 = ctx.r12.s64 + -12360;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826ED018;
	case 1:
		goto loc_826ED020;
	case 2:
		goto loc_826ED028;
	case 3:
		goto loc_826ED030;
	case 4:
		goto loc_826ED038;
	case 5:
		goto loc_826ED040;
	case 6:
		goto loc_826ED048;
	case 7:
		goto loc_826ED050;
	case 8:
		goto loc_826ED058;
	case 9:
		goto loc_826ED060;
	case 10:
		goto loc_826ED068;
	case 11:
		goto loc_826ED070;
	case 12:
		goto loc_826ED078;
	case 13:
		goto loc_826ED080;
	case 14:
		goto loc_826ED088;
	case 15:
		goto loc_826ED090;
	case 16:
		goto loc_826ED098;
	case 17:
		goto loc_826ED0A0;
	case 18:
		goto loc_826ED0A8;
	case 19:
		goto loc_826ED0B0;
	case 20:
		goto loc_826ED0B8;
	case 21:
		goto loc_826ED0C0;
	case 22:
		goto loc_826ED0C8;
	case 23:
		goto loc_826ED0D0;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826ED018:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826ED020:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826ED028:
	// li r3,26
	ctx.r3.s64 = 26;
	// blr 
	return;
loc_826ED030:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826ED038:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826ED040:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826ED048:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_826ED050:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_826ED058:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_826ED060:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_826ED068:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_826ED070:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_826ED078:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_826ED080:
	// li r3,19
	ctx.r3.s64 = 19;
	// blr 
	return;
loc_826ED088:
	// li r3,20
	ctx.r3.s64 = 20;
	// blr 
	return;
loc_826ED090:
	// li r3,21
	ctx.r3.s64 = 21;
	// blr 
	return;
loc_826ED098:
	// li r3,25
	ctx.r3.s64 = 25;
	// blr 
	return;
loc_826ED0A0:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826ED0A8:
	// li r3,28
	ctx.r3.s64 = 28;
	// blr 
	return;
loc_826ED0B0:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826ED0B8:
	// li r3,30
	ctx.r3.s64 = 30;
	// blr 
	return;
loc_826ED0C0:
	// li r3,31
	ctx.r3.s64 = 31;
	// blr 
	return;
loc_826ED0C8:
	// li r3,32
	ctx.r3.s64 = 32;
	// blr 
	return;
loc_826ED0D0:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826ED0DC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED0E8);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED0E8) { __imp__sub_826ED0E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED0E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25756
	ctx.r3.s64 = ctx.r11.s64 + -25756;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED0F8);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED0F8) { __imp__sub_826ED0F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED0F8) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826ecf98
	sub_826ECF98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED100);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED100) { __imp__sub_826ED100(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED100) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25720
	ctx.r3.s64 = ctx.r11.s64 + -25720;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED110);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED110) { __imp__sub_826ED110(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED110) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25684
	ctx.r3.s64 = ctx.r11.s64 + -25684;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED120);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED120) { __imp__sub_826ED120(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED120) {
	PPC_FUNC_PROLOGUE();
	// li r3,204
	ctx.r3.s64 = 204;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED128);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED128) { __imp__sub_826ED128(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED128) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25648
	ctx.r3.s64 = ctx.r11.s64 + -25648;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED138);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED138) { __imp__sub_826ED138(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED138) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826ed1fc
	if (ctx.cr6.gt) goto loc_826ED1FC;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826ed1f4
	if (ctx.cr6.eq) goto loc_826ED1F4;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,31
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 31, ctx.xer);
	// bgt cr6,0x826ed1fc
	if (ctx.cr6.gt) goto loc_826ED1FC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-11916
	ctx.r12.s64 = ctx.r12.s64 + -11916;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826ED1F4;
	case 1:
		goto loc_826ED1F4;
	case 2:
		goto loc_826ED1F4;
	case 3:
		goto loc_826ED1F4;
	case 4:
		goto loc_826ED1FC;
	case 5:
		goto loc_826ED1F4;
	case 6:
		goto loc_826ED1F4;
	case 7:
		goto loc_826ED1F4;
	case 8:
		goto loc_826ED1FC;
	case 9:
		goto loc_826ED1F4;
	case 10:
		goto loc_826ED1F4;
	case 11:
		goto loc_826ED1F4;
	case 12:
		goto loc_826ED1F4;
	case 13:
		goto loc_826ED1F4;
	case 14:
		goto loc_826ED1F4;
	case 15:
		goto loc_826ED1F4;
	case 16:
		goto loc_826ED1FC;
	case 17:
		goto loc_826ED1FC;
	case 18:
		goto loc_826ED1F4;
	case 19:
		goto loc_826ED1F4;
	case 20:
		goto loc_826ED1F4;
	case 21:
		goto loc_826ED1FC;
	case 22:
		goto loc_826ED1FC;
	case 23:
		goto loc_826ED1FC;
	case 24:
		goto loc_826ED1F4;
	case 25:
		goto loc_826ED1F4;
	case 26:
		goto loc_826ED1FC;
	case 27:
		goto loc_826ED1F4;
	case 28:
		goto loc_826ED1FC;
	case 29:
		goto loc_826ED1F4;
	case 30:
		goto loc_826ED1F4;
	case 31:
		goto loc_826ED1F4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826ED1F4:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826ED1FC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED208);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED208) { __imp__sub_826ED208(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED208) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826ed384
	if (ctx.cr6.gt) goto loc_826ED384;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826ed37c
	if (ctx.cr6.eq) goto loc_826ED37C;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,31
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 31, ctx.xer);
	// bgt cr6,0x826ed384
	if (ctx.cr6.gt) goto loc_826ED384;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-11708
	ctx.r12.s64 = ctx.r12.s64 + -11708;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826ED2CC;
	case 1:
		goto loc_826ED34C;
	case 2:
		goto loc_826ED35C;
	case 3:
		goto loc_826ED2C4;
	case 4:
		goto loc_826ED384;
	case 5:
		goto loc_826ED2DC;
	case 6:
		goto loc_826ED2E4;
	case 7:
		goto loc_826ED2EC;
	case 8:
		goto loc_826ED384;
	case 9:
		goto loc_826ED31C;
	case 10:
		goto loc_826ED324;
	case 11:
		goto loc_826ED2F4;
	case 12:
		goto loc_826ED2FC;
	case 13:
		goto loc_826ED304;
	case 14:
		goto loc_826ED30C;
	case 15:
		goto loc_826ED314;
	case 16:
		goto loc_826ED384;
	case 17:
		goto loc_826ED384;
	case 18:
		goto loc_826ED32C;
	case 19:
		goto loc_826ED334;
	case 20:
		goto loc_826ED33C;
	case 21:
		goto loc_826ED384;
	case 22:
		goto loc_826ED384;
	case 23:
		goto loc_826ED384;
	case 24:
		goto loc_826ED344;
	case 25:
		goto loc_826ED2D4;
	case 26:
		goto loc_826ED384;
	case 27:
		goto loc_826ED354;
	case 28:
		goto loc_826ED384;
	case 29:
		goto loc_826ED364;
	case 30:
		goto loc_826ED36C;
	case 31:
		goto loc_826ED374;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826ED2C4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826ED2CC:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826ED2D4:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826ED2DC:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826ED2E4:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826ED2EC:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826ED2F4:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826ED2FC:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826ED304:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826ED30C:
	// li r3,9
	ctx.r3.s64 = 9;
	// blr 
	return;
loc_826ED314:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_826ED31C:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_826ED324:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_826ED32C:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_826ED334:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_826ED33C:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_826ED344:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_826ED34C:
	// li r3,17
	ctx.r3.s64 = 17;
	// blr 
	return;
loc_826ED354:
	// li r3,18
	ctx.r3.s64 = 18;
	// blr 
	return;
loc_826ED35C:
	// li r3,19
	ctx.r3.s64 = 19;
	// blr 
	return;
loc_826ED364:
	// li r3,20
	ctx.r3.s64 = 20;
	// blr 
	return;
loc_826ED36C:
	// li r3,21
	ctx.r3.s64 = 21;
	// blr 
	return;
loc_826ED374:
	// li r3,22
	ctx.r3.s64 = 22;
	// blr 
	return;
loc_826ED37C:
	// li r3,23
	ctx.r3.s64 = 23;
	// blr 
	return;
loc_826ED384:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED390);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED390) { __imp__sub_826ED390(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED390) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25612
	ctx.r3.s64 = ctx.r11.s64 + -25612;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED3A0);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED3A0) { __imp__sub_826ED3A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED3A0) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826ed208
	sub_826ED208(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED3A8);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED3A8) { __imp__sub_826ED3A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED3A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25576
	ctx.r3.s64 = ctx.r11.s64 + -25576;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED3B8);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED3B8) { __imp__sub_826ED3B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED3B8) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,23
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 23, ctx.xer);
	// bgt cr6,0x826ed448
	if (ctx.cr6.gt) goto loc_826ED448;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-11304
	ctx.r12.s64 = ctx.r12.s64 + -11304;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826ED438;
	case 1:
		goto loc_826ED438;
	case 2:
		goto loc_826ED438;
	case 3:
		goto loc_826ED440;
	case 4:
		goto loc_826ED440;
	case 5:
		goto loc_826ED440;
	case 6:
		goto loc_826ED440;
	case 7:
		goto loc_826ED440;
	case 8:
		goto loc_826ED440;
	case 9:
		goto loc_826ED440;
	case 10:
		goto loc_826ED440;
	case 11:
		goto loc_826ED438;
	case 12:
		goto loc_826ED438;
	case 13:
		goto loc_826ED438;
	case 14:
		goto loc_826ED438;
	case 15:
		goto loc_826ED438;
	case 16:
		goto loc_826ED440;
	case 17:
		goto loc_826ED440;
	case 18:
		goto loc_826ED440;
	case 19:
		goto loc_826ED440;
	case 20:
		goto loc_826ED440;
	case 21:
		goto loc_826ED440;
	case 22:
		goto loc_826ED440;
	case 23:
		goto loc_826ED438;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826ED438:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826ED440:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826ED448:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED450);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED450) { __imp__sub_826ED450(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED450) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25540
	ctx.r3.s64 = ctx.r11.s64 + -25540;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED460);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED460) { __imp__sub_826ED460(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED460) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826ed3b8
	sub_826ED3B8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED468);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED468) { __imp__sub_826ED468(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED468) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,23
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 23, ctx.xer);
	// bgt cr6,0x826ed4f8
	if (ctx.cr6.gt) goto loc_826ED4F8;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-11128
	ctx.r12.s64 = ctx.r12.s64 + -11128;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826ED4E8;
	case 1:
		goto loc_826ED4E8;
	case 2:
		goto loc_826ED4E8;
	case 3:
		goto loc_826ED4F0;
	case 4:
		goto loc_826ED4F0;
	case 5:
		goto loc_826ED4F0;
	case 6:
		goto loc_826ED4F0;
	case 7:
		goto loc_826ED4F0;
	case 8:
		goto loc_826ED4F0;
	case 9:
		goto loc_826ED4F0;
	case 10:
		goto loc_826ED4F0;
	case 11:
		goto loc_826ED4E8;
	case 12:
		goto loc_826ED4E8;
	case 13:
		goto loc_826ED4E8;
	case 14:
		goto loc_826ED4E8;
	case 15:
		goto loc_826ED4E8;
	case 16:
		goto loc_826ED4F0;
	case 17:
		goto loc_826ED4F0;
	case 18:
		goto loc_826ED4F0;
	case 19:
		goto loc_826ED4F0;
	case 20:
		goto loc_826ED4F0;
	case 21:
		goto loc_826ED4F0;
	case 22:
		goto loc_826ED4F0;
	case 23:
		goto loc_826ED4E8;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826ED4E8:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826ED4F0:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826ED4F8:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED500);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED500) { __imp__sub_826ED500(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED500) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25504
	ctx.r3.s64 = ctx.r11.s64 + -25504;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED510);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED510) { __imp__sub_826ED510(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED510) {
	PPC_FUNC_PROLOGUE();
	// li r3,182
	ctx.r3.s64 = 182;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED518);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED518) { __imp__sub_826ED518(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED518) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bl 0x826ed468
	ctx.lr = 0x826ED52C;
	sub_826ED468(ctx, base);
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826ed5b4
	if (ctx.cr6.gt) goto loc_826ED5B4;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-10928
	ctx.r12.s64 = ctx.r12.s64 + -10928;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826ED564;
	case 1:
		goto loc_826ED578;
	case 2:
		goto loc_826ED5A0;
	case 3:
		goto loc_826ED5B4;
	case 4:
		goto loc_826ED58C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826ED564:
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
loc_826ED578:
	// li r3,1
	ctx.r3.s64 = 1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826ED58C:
	// li r3,2
	ctx.r3.s64 = 2;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826ED5A0:
	// li r3,3
	ctx.r3.s64 = 3;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826ED5B4:
	// li r3,-1
	ctx.r3.s64 = -1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED5C8);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED5C8) { __imp__sub_826ED5C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED5C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25468
	ctx.r3.s64 = ctx.r11.s64 + -25468;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED5D8);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED5D8) { __imp__sub_826ED5D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED5D8) {
	PPC_FUNC_PROLOGUE();
	// li r3,175
	ctx.r3.s64 = 175;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED5E0);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED5E0) { __imp__sub_826ED5E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED5E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25432
	ctx.r3.s64 = ctx.r11.s64 + -25432;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED5F0);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED5F0) { __imp__sub_826ED5F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED5F0) {
	PPC_FUNC_PROLOGUE();
	// li r3,176
	ctx.r3.s64 = 176;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED5F8);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED5F8) { __imp__sub_826ED5F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED5F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25396
	ctx.r3.s64 = ctx.r11.s64 + -25396;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED608);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED608) { __imp__sub_826ED608(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED608) {
	PPC_FUNC_PROLOGUE();
	// li r3,177
	ctx.r3.s64 = 177;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED610);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED610) { __imp__sub_826ED610(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED610) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826ed6ac
	if (ctx.cr6.gt) goto loc_826ED6AC;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826ed6a4
	if (ctx.cr6.eq) goto loc_826ED6A4;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,17
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 17, ctx.xer);
	// bgt cr6,0x826ed6ac
	if (ctx.cr6.gt) goto loc_826ED6AC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-10676
	ctx.r12.s64 = ctx.r12.s64 + -10676;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826ED694;
	case 1:
		goto loc_826ED694;
	case 2:
		goto loc_826ED69C;
	case 3:
		goto loc_826ED694;
	case 4:
		goto loc_826ED694;
	case 5:
		goto loc_826ED694;
	case 6:
		goto loc_826ED694;
	case 7:
		goto loc_826ED694;
	case 8:
		goto loc_826ED6A4;
	case 9:
		goto loc_826ED69C;
	case 10:
		goto loc_826ED694;
	case 11:
		goto loc_826ED69C;
	case 12:
		goto loc_826ED69C;
	case 13:
		goto loc_826ED694;
	case 14:
		goto loc_826ED694;
	case 15:
		goto loc_826ED694;
	case 16:
		goto loc_826ED69C;
	case 17:
		goto loc_826ED6A4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826ED694:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826ED69C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826ED6A4:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826ED6AC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED6B8);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED6B8) { __imp__sub_826ED6B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED6B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25360
	ctx.r3.s64 = ctx.r11.s64 + -25360;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED6C8);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED6C8) { __imp__sub_826ED6C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED6C8) {
	PPC_FUNC_PROLOGUE();
	// li r3,178
	ctx.r3.s64 = 178;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED6D0);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED6D0) { __imp__sub_826ED6D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED6D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25324
	ctx.r3.s64 = ctx.r11.s64 + -25324;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED6E0);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED6E0) { __imp__sub_826ED6E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED6E0) {
	PPC_FUNC_PROLOGUE();
	// li r3,179
	ctx.r3.s64 = 179;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED6E8);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED6E8) { __imp__sub_826ED6E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED6E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25288
	ctx.r3.s64 = ctx.r11.s64 + -25288;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED6F8);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED6F8) { __imp__sub_826ED6F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED6F8) {
	PPC_FUNC_PROLOGUE();
	// li r3,194
	ctx.r3.s64 = 194;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED700);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED700) { __imp__sub_826ED700(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED700) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25252
	ctx.r3.s64 = ctx.r11.s64 + -25252;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED710);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED710) { __imp__sub_826ED710(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED710) {
	PPC_FUNC_PROLOGUE();
	// li r3,195
	ctx.r3.s64 = 195;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED718);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED718) { __imp__sub_826ED718(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED718) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25216
	ctx.r3.s64 = ctx.r11.s64 + -25216;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED728);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED728) { __imp__sub_826ED728(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED728) {
	PPC_FUNC_PROLOGUE();
	// li r3,196
	ctx.r3.s64 = 196;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED730);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED730) { __imp__sub_826ED730(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED730) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25180
	ctx.r3.s64 = ctx.r11.s64 + -25180;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED740);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED740) { __imp__sub_826ED740(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED740) {
	PPC_FUNC_PROLOGUE();
	// li r3,197
	ctx.r3.s64 = 197;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED748);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED748) { __imp__sub_826ED748(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED748) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25144
	ctx.r3.s64 = ctx.r11.s64 + -25144;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED758);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED758) { __imp__sub_826ED758(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED758) {
	PPC_FUNC_PROLOGUE();
	// li r3,198
	ctx.r3.s64 = 198;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED760);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED760) { __imp__sub_826ED760(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED760) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25108
	ctx.r3.s64 = ctx.r11.s64 + -25108;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED770);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED770) { __imp__sub_826ED770(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED770) {
	PPC_FUNC_PROLOGUE();
	// li r3,213
	ctx.r3.s64 = 213;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED778);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED778) { __imp__sub_826ED778(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED778) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25072
	ctx.r3.s64 = ctx.r11.s64 + -25072;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED788);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED788) { __imp__sub_826ED788(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED788) {
	PPC_FUNC_PROLOGUE();
	// li r3,214
	ctx.r3.s64 = 214;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED790);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED790) { __imp__sub_826ED790(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED790) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25036
	ctx.r3.s64 = ctx.r11.s64 + -25036;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED7A0);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED7A0) { __imp__sub_826ED7A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED7A0) {
	PPC_FUNC_PROLOGUE();
	// li r3,215
	ctx.r3.s64 = 215;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED7A8);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED7A8) { __imp__sub_826ED7A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED7A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-25000
	ctx.r3.s64 = ctx.r11.s64 + -25000;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED7B8);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED7B8) { __imp__sub_826ED7B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED7B8) {
	PPC_FUNC_PROLOGUE();
	// li r3,216
	ctx.r3.s64 = 216;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED7C0);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED7C0) { __imp__sub_826ED7C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED7C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-24964
	ctx.r3.s64 = ctx.r11.s64 + -24964;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED7D0);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED7D0) { __imp__sub_826ED7D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED7D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-24928
	ctx.r3.s64 = ctx.r11.s64 + -24928;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED7E0);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED7E0) { __imp__sub_826ED7E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED7E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-24892
	ctx.r3.s64 = ctx.r11.s64 + -24892;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED7F0);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED7F0) { __imp__sub_826ED7F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED7F0) {
	PPC_FUNC_PROLOGUE();
	// li r3,219
	ctx.r3.s64 = 219;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED7F8);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED7F8) { __imp__sub_826ED7F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED7F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-24856
	ctx.r3.s64 = ctx.r11.s64 + -24856;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED808);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED808) { __imp__sub_826ED808(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED808) {
	PPC_FUNC_PROLOGUE();
	// li r3,220
	ctx.r3.s64 = 220;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED810);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED810) { __imp__sub_826ED810(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED810) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-24820
	ctx.r3.s64 = ctx.r11.s64 + -24820;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED820);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED820) { __imp__sub_826ED820(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED820) {
	PPC_FUNC_PROLOGUE();
	// li r3,221
	ctx.r3.s64 = 221;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED828);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED828) { __imp__sub_826ED828(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED828) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-24784
	ctx.r3.s64 = ctx.r11.s64 + -24784;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED838);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED838) { __imp__sub_826ED838(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED838) {
	PPC_FUNC_PROLOGUE();
	// li r3,222
	ctx.r3.s64 = 222;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED840);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED840) { __imp__sub_826ED840(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED840) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-24748
	ctx.r3.s64 = ctx.r11.s64 + -24748;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED850);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED850) { __imp__sub_826ED850(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED850) {
	PPC_FUNC_PROLOGUE();
	// li r3,223
	ctx.r3.s64 = 223;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED858);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED858) { __imp__sub_826ED858(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED858) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-24712
	ctx.r3.s64 = ctx.r11.s64 + -24712;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED868);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED868) { __imp__sub_826ED868(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED868) {
	PPC_FUNC_PROLOGUE();
	// li r3,224
	ctx.r3.s64 = 224;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED870);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED870) { __imp__sub_826ED870(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED870) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-24676
	ctx.r3.s64 = ctx.r11.s64 + -24676;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED880);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED880) { __imp__sub_826ED880(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED880) {
	PPC_FUNC_PROLOGUE();
	// li r3,225
	ctx.r3.s64 = 225;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED888);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED888) { __imp__sub_826ED888(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED888) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-24640
	ctx.r3.s64 = ctx.r11.s64 + -24640;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED898);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED898) { __imp__sub_826ED898(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED898) {
	PPC_FUNC_PROLOGUE();
	// li r3,226
	ctx.r3.s64 = 226;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED8A0);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED8A0) { __imp__sub_826ED8A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED8A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-24604
	ctx.r3.s64 = ctx.r11.s64 + -24604;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED8B0);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED8B0) { __imp__sub_826ED8B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED8B0) {
	PPC_FUNC_PROLOGUE();
	// li r3,227
	ctx.r3.s64 = 227;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED8B8);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED8B8) { __imp__sub_826ED8B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED8B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-24568
	ctx.r3.s64 = ctx.r11.s64 + -24568;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED8C8);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED8C8) { __imp__sub_826ED8C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED8C8) {
	PPC_FUNC_PROLOGUE();
	// li r3,229
	ctx.r3.s64 = 229;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED8D0);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED8D0) { __imp__sub_826ED8D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED8D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826ed9ec
	if (ctx.cr6.gt) goto loc_826ED9EC;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826ed9e4
	if (ctx.cr6.eq) goto loc_826ED9E4;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,17
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 17, ctx.xer);
	// bgt cr6,0x826ed9ec
	if (ctx.cr6.gt) goto loc_826ED9EC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-9972
	ctx.r12.s64 = ctx.r12.s64 + -9972;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826ED954;
	case 1:
		goto loc_826ED95C;
	case 2:
		goto loc_826ED964;
	case 3:
		goto loc_826ED96C;
	case 4:
		goto loc_826ED974;
	case 5:
		goto loc_826ED97C;
	case 6:
		goto loc_826ED9AC;
	case 7:
		goto loc_826ED9A4;
	case 8:
		goto loc_826ED9D4;
	case 9:
		goto loc_826ED9CC;
	case 10:
		goto loc_826ED99C;
	case 11:
		goto loc_826ED9C4;
	case 12:
		goto loc_826ED9BC;
	case 13:
		goto loc_826ED984;
	case 14:
		goto loc_826ED98C;
	case 15:
		goto loc_826ED994;
	case 16:
		goto loc_826ED9B4;
	case 17:
		goto loc_826ED9DC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826ED954:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826ED95C:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826ED964:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826ED96C:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826ED974:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826ED97C:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826ED984:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826ED98C:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826ED994:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826ED99C:
	// li r3,9
	ctx.r3.s64 = 9;
	// blr 
	return;
loc_826ED9A4:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_826ED9AC:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_826ED9B4:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_826ED9BC:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_826ED9C4:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_826ED9CC:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_826ED9D4:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_826ED9DC:
	// li r3,17
	ctx.r3.s64 = 17;
	// blr 
	return;
loc_826ED9E4:
	// li r3,18
	ctx.r3.s64 = 18;
	// blr 
	return;
loc_826ED9EC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826ED9F8);  /* forward decl */
PPC_WEAK_FUNC(sub_826ED9F8) { __imp__sub_826ED9F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826ED9F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-24532
	ctx.r3.s64 = ctx.r11.s64 + -24532;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDA08);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDA08) { __imp__sub_826EDA08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDA08) {
	PPC_FUNC_PROLOGUE();
	// li r3,233
	ctx.r3.s64 = 233;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDA10);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDA10) { __imp__sub_826EDA10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDA10) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826ed8d0
	sub_826ED8D0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDA18);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDA18) { __imp__sub_826EDA18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDA18) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-24496
	ctx.r3.s64 = ctx.r11.s64 + -24496;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDA28);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDA28) { __imp__sub_826EDA28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDA28) {
	PPC_FUNC_PROLOGUE();
	// li r3,237
	ctx.r3.s64 = 237;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDA30);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDA30) { __imp__sub_826EDA30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDA30) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,18
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 18, ctx.xer);
	// bgt cr6,0x826edaac
	if (ctx.cr6.gt) goto loc_826EDAAC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-9648
	ctx.r12.s64 = ctx.r12.s64 + -9648;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826EDA9C;
	case 1:
		goto loc_826EDA9C;
	case 2:
		goto loc_826EDAA4;
	case 3:
		goto loc_826EDAA4;
	case 4:
		goto loc_826EDA9C;
	case 5:
		goto loc_826EDA9C;
	case 6:
		goto loc_826EDA9C;
	case 7:
		goto loc_826EDA9C;
	case 8:
		goto loc_826EDA9C;
	case 9:
		goto loc_826EDA9C;
	case 10:
		goto loc_826EDA9C;
	case 11:
		goto loc_826EDA9C;
	case 12:
		goto loc_826EDA9C;
	case 13:
		goto loc_826EDAA4;
	case 14:
		goto loc_826EDAA4;
	case 15:
		goto loc_826EDAA4;
	case 16:
		goto loc_826EDAA4;
	case 17:
		goto loc_826EDAA4;
	case 18:
		goto loc_826EDAA4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EDA9C:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EDAA4:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EDAAC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDAB8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDAB8) { __imp__sub_826EDAB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDAB8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-24460
	ctx.r3.s64 = ctx.r11.s64 + -24460;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDAC8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDAC8) { __imp__sub_826EDAC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDAC8) {
	PPC_FUNC_PROLOGUE();
	// li r3,239
	ctx.r3.s64 = 239;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDAD0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDAD0) { __imp__sub_826EDAD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDAD0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bl 0x826eda30
	ctx.lr = 0x826EDAE4;
	sub_826EDA30(ctx, base);
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826edb6c
	if (ctx.cr6.gt) goto loc_826EDB6C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-9464
	ctx.r12.s64 = ctx.r12.s64 + -9464;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EDB1C;
	case 1:
		goto loc_826EDB30;
	case 2:
		goto loc_826EDB58;
	case 3:
		goto loc_826EDB6C;
	case 4:
		goto loc_826EDB44;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EDB1C:
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
loc_826EDB30:
	// li r3,1
	ctx.r3.s64 = 1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826EDB44:
	// li r3,2
	ctx.r3.s64 = 2;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826EDB58:
	// li r3,3
	ctx.r3.s64 = 3;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826EDB6C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDB80);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDB80) { __imp__sub_826EDB80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDB80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-24424
	ctx.r3.s64 = ctx.r11.s64 + -24424;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDB90);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDB90) { __imp__sub_826EDB90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDB90) {
	PPC_FUNC_PROLOGUE();
	// li r3,243
	ctx.r3.s64 = 243;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDB98);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDB98) { __imp__sub_826EDB98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDB98) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-24388
	ctx.r3.s64 = ctx.r11.s64 + -24388;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDBA8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDBA8) { __imp__sub_826EDBA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDBA8) {
	PPC_FUNC_PROLOGUE();
	// li r3,232
	ctx.r3.s64 = 232;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDBB0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDBB0) { __imp__sub_826EDBB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDBB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-24352
	ctx.r3.s64 = ctx.r11.s64 + -24352;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDBC0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDBC0) { __imp__sub_826EDBC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDBC0) {
	PPC_FUNC_PROLOGUE();
	// li r3,235
	ctx.r3.s64 = 235;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDBC8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDBC8) { __imp__sub_826EDBC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDBC8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-24316
	ctx.r3.s64 = ctx.r11.s64 + -24316;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDBD8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDBD8) { __imp__sub_826EDBD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDBD8) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,18
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 18, ctx.xer);
	// bgt cr6,0x826edce0
	if (ctx.cr6.gt) goto loc_826EDCE0;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-9224
	ctx.r12.s64 = ctx.r12.s64 + -9224;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826EDC44;
	case 1:
		goto loc_826EDC4C;
	case 2:
		goto loc_826EDC54;
	case 3:
		goto loc_826EDC5C;
	case 4:
		goto loc_826EDC64;
	case 5:
		goto loc_826EDC6C;
	case 6:
		goto loc_826EDC74;
	case 7:
		goto loc_826EDC7C;
	case 8:
		goto loc_826EDC84;
	case 9:
		goto loc_826EDC8C;
	case 10:
		goto loc_826EDC94;
	case 11:
		goto loc_826EDC9C;
	case 12:
		goto loc_826EDCA4;
	case 13:
		goto loc_826EDCAC;
	case 14:
		goto loc_826EDCB4;
	case 15:
		goto loc_826EDCBC;
	case 16:
		goto loc_826EDCC4;
	case 17:
		goto loc_826EDCCC;
	case 18:
		goto loc_826EDCD4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EDC44:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826EDC4C:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826EDC54:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826EDC5C:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EDC64:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826EDC6C:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826EDC74:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_826EDC7C:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_826EDC84:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_826EDC8C:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_826EDC94:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826EDC9C:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826EDCA4:
	// li r3,17
	ctx.r3.s64 = 17;
	// blr 
	return;
loc_826EDCAC:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_826EDCB4:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_826EDCBC:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_826EDCC4:
	// li r3,9
	ctx.r3.s64 = 9;
	// blr 
	return;
loc_826EDCCC:
	// li r3,18
	ctx.r3.s64 = 18;
	// blr 
	return;
loc_826EDCD4:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_826EDCE0:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDCE8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDCE8) { __imp__sub_826EDCE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDCE8) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,18
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 18, ctx.xer);
	// bgt cr6,0x826edd64
	if (ctx.cr6.gt) goto loc_826EDD64;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-8952
	ctx.r12.s64 = ctx.r12.s64 + -8952;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826EDD54;
	case 1:
		goto loc_826EDD54;
	case 2:
		goto loc_826EDD5C;
	case 3:
		goto loc_826EDD5C;
	case 4:
		goto loc_826EDD54;
	case 5:
		goto loc_826EDD54;
	case 6:
		goto loc_826EDD54;
	case 7:
		goto loc_826EDD54;
	case 8:
		goto loc_826EDD54;
	case 9:
		goto loc_826EDD54;
	case 10:
		goto loc_826EDD54;
	case 11:
		goto loc_826EDD54;
	case 12:
		goto loc_826EDD54;
	case 13:
		goto loc_826EDD5C;
	case 14:
		goto loc_826EDD5C;
	case 15:
		goto loc_826EDD5C;
	case 16:
		goto loc_826EDD5C;
	case 17:
		goto loc_826EDD5C;
	case 18:
		goto loc_826EDD5C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EDD54:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826EDD5C:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826EDD64:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDD70);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDD70) { __imp__sub_826EDD70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDD70) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-24280
	ctx.r3.s64 = ctx.r11.s64 + -24280;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDD80);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDD80) { __imp__sub_826EDD80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDD80) {
	PPC_FUNC_PROLOGUE();
	// li r3,245
	ctx.r3.s64 = 245;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDD88);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDD88) { __imp__sub_826EDD88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDD88) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826edbd8
	sub_826EDBD8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDD90);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDD90) { __imp__sub_826EDD90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDD90) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826edce8
	sub_826EDCE8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDD98);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDD98) { __imp__sub_826EDD98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDD98) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-24244
	ctx.r3.s64 = ctx.r11.s64 + -24244;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDDA8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDDA8) { __imp__sub_826EDDA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDDA8) {
	PPC_FUNC_PROLOGUE();
	// li r3,247
	ctx.r3.s64 = 247;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDDB0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDDB0) { __imp__sub_826EDDB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDDB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-24208
	ctx.r3.s64 = ctx.r11.s64 + -24208;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDDC0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDDC0) { __imp__sub_826EDDC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDDC0) {
	PPC_FUNC_PROLOGUE();
	// li r3,241
	ctx.r3.s64 = 241;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDDC8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDDC8) { __imp__sub_826EDDC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDDC8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-24172
	ctx.r3.s64 = ctx.r11.s64 + -24172;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDDD8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDDD8) { __imp__sub_826EDDD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDDD8) {
	PPC_FUNC_PROLOGUE();
	// li r3,240
	ctx.r3.s64 = 240;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDDE0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDDE0) { __imp__sub_826EDDE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDDE0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-24136
	ctx.r3.s64 = ctx.r11.s64 + -24136;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDDF0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDDF0) { __imp__sub_826EDDF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDDF0) {
	PPC_FUNC_PROLOGUE();
	// li r3,234
	ctx.r3.s64 = 234;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDDF8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDDF8) { __imp__sub_826EDDF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDDF8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-24100
	ctx.r3.s64 = ctx.r11.s64 + -24100;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDE08);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDE08) { __imp__sub_826EDE08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDE08) {
	PPC_FUNC_PROLOGUE();
	// li r3,244
	ctx.r3.s64 = 244;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDE10);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDE10) { __imp__sub_826EDE10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDE10) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-24064
	ctx.r3.s64 = ctx.r11.s64 + -24064;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDE20);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDE20) { __imp__sub_826EDE20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDE20) {
	PPC_FUNC_PROLOGUE();
	// li r3,246
	ctx.r3.s64 = 246;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDE28);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDE28) { __imp__sub_826EDE28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDE28) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-24028
	ctx.r3.s64 = ctx.r11.s64 + -24028;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDE38);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDE38) { __imp__sub_826EDE38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDE38) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-23992
	ctx.r3.s64 = ctx.r11.s64 + -23992;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDE48);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDE48) { __imp__sub_826EDE48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDE48) {
	PPC_FUNC_PROLOGUE();
	// li r3,231
	ctx.r3.s64 = 231;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDE50);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDE50) { __imp__sub_826EDE50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDE50) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-23956
	ctx.r3.s64 = ctx.r11.s64 + -23956;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDE60);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDE60) { __imp__sub_826EDE60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDE60) {
	PPC_FUNC_PROLOGUE();
	// li r3,230
	ctx.r3.s64 = 230;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDE68);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDE68) { __imp__sub_826EDE68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDE68) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-23920
	ctx.r3.s64 = ctx.r11.s64 + -23920;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDE78);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDE78) { __imp__sub_826EDE78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDE78) {
	PPC_FUNC_PROLOGUE();
	// li r3,248
	ctx.r3.s64 = 248;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDE80);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDE80) { __imp__sub_826EDE80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDE80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-23884
	ctx.r3.s64 = ctx.r11.s64 + -23884;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDE90);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDE90) { __imp__sub_826EDE90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDE90) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-23848
	ctx.r3.s64 = ctx.r11.s64 + -23848;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDEA0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDEA0) { __imp__sub_826EDEA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDEA0) {
	PPC_FUNC_PROLOGUE();
	// li r3,249
	ctx.r3.s64 = 249;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDEA8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDEA8) { __imp__sub_826EDEA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDEA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-23812
	ctx.r3.s64 = ctx.r11.s64 + -23812;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDEB8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDEB8) { __imp__sub_826EDEB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDEB8) {
	PPC_FUNC_PROLOGUE();
	// li r3,238
	ctx.r3.s64 = 238;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDEC0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDEC0) { __imp__sub_826EDEC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDEC0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-23776
	ctx.r3.s64 = ctx.r11.s64 + -23776;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDED0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDED0) { __imp__sub_826EDED0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDED0) {
	PPC_FUNC_PROLOGUE();
	// li r3,250
	ctx.r3.s64 = 250;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDED8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDED8) { __imp__sub_826EDED8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDED8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x826EDEE0;
	__savegprlr_28(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r28,r3
	ctx.r28.u64 = ctx.r3.u64;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// li r30,0
	ctx.r30.s64 = 0;
	// lbz r11,5(r28)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r28.u32 + 5);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826edfe0
	if (ctx.cr6.eq) goto loc_826EDFE0;
	// li r31,0
	ctx.r31.s64 = 0;
loc_826EDF00:
	// lwz r11,20(r28)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r28.u32 + 20);
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// add r3,r11,r31
	ctx.r3.u64 = ctx.r11.u64 + ctx.r31.u64;
	// bl 0x82974470
	ctx.lr = 0x826EDF10;
	sub_82974470(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826edf38
	if (!ctx.cr6.eq) goto loc_826EDF38;
	// lbz r11,5(r28)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r28.u32 + 5);
	// addi r30,r30,1
	ctx.r30.s64 = ctx.r30.s64 + 1;
	// addi r31,r31,16
	ctx.r31.s64 = ctx.r31.s64 + 16;
	// cmplw cr6,r30,r11
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, ctx.r11.u32, ctx.xer);
	// blt cr6,0x826edf00
	if (ctx.cr6.lt) goto loc_826EDF00;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
loc_826EDF38:
	// lwz r10,20(r28)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r28.u32 + 20);
	// rlwinm r11,r30,4,0,27
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 4) & 0xFFFFFFF0;
	// add r3,r11,r10
	ctx.r3.u64 = ctx.r11.u64 + ctx.r10.u64;
	// bl 0x829743a8
	ctx.lr = 0x826EDF48;
	sub_829743A8(ctx, base);
	// lbz r11,4(r28)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r28.u32 + 4);
	// lwz r7,12(r28)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r28.u32 + 12);
	// li r6,0
	ctx.r6.s64 = 0;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826edfe0
	if (ctx.cr6.eq) goto loc_826EDFE0;
	// mulli r4,r30,240
	ctx.r4.s64 = static_cast<int64_t>(ctx.r30.u64 * static_cast<uint64_t>(240));
	// li r5,19
	ctx.r5.s64 = 19;
	// lis r8,-32768
	ctx.r8.s64 = -2147483648;
	// li r3,17
	ctx.r3.s64 = 17;
	// li r31,-1
	ctx.r31.s64 = -1;
loc_826EDF70:
	// add r11,r4,r7
	ctx.r11.u64 = ctx.r4.u64 + ctx.r7.u64;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r9,r11,4
	ctx.r9.s64 = ctx.r11.s64 + 4;
	// stw r5,80(r11)
	PPC_STORE_U32(ctx.r11.u32 + 80, ctx.r5.u32);
loc_826EDF80:
	// stw r8,0(r9)
	PPC_STORE_U32(ctx.r9.u32 + 0, ctx.r8.u32);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// lwz r30,80(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 80);
	// addi r9,r9,4
	ctx.r9.s64 = ctx.r9.s64 + 4;
	// cmpw cr6,r10,r30
	ctx.cr6.compare<int32_t>(ctx.r10.s32, ctx.r30.s32, ctx.xer);
	// blt cr6,0x826edf80
	if (ctx.cr6.lt) goto loc_826EDF80;
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r3,224(r11)
	PPC_STORE_U32(ctx.r11.u32 + 224, ctx.r3.u32);
	// addi r9,r11,88
	ctx.r9.s64 = ctx.r11.s64 + 88;
loc_826EDFA4:
	// std r8,0(r9)
	PPC_STORE_U64(ctx.r9.u32 + 0, ctx.r8.u64);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// lwz r30,224(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 224);
	// addi r9,r9,8
	ctx.r9.s64 = ctx.r9.s64 + 8;
	// cmpw cr6,r10,r30
	ctx.cr6.compare<int32_t>(ctx.r10.s32, ctx.r30.s32, ctx.xer);
	// blt cr6,0x826edfa4
	if (ctx.cr6.lt) goto loc_826EDFA4;
	// stw r31,232(r11)
	PPC_STORE_U32(ctx.r11.u32 + 232, ctx.r31.u32);
	// lwz r11,0(r7)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826edfe0
	if (ctx.cr6.eq) goto loc_826EDFE0;
	// lbz r10,4(r28)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r28.u32 + 4);
	// addi r6,r6,1
	ctx.r6.s64 = ctx.r6.s64 + 1;
	// mr r7,r11
	ctx.r7.u64 = ctx.r11.u64;
	// cmpw cr6,r6,r10
	ctx.cr6.compare<int32_t>(ctx.r6.s32, ctx.r10.s32, ctx.xer);
	// blt cr6,0x826edf70
	if (ctx.cr6.lt) goto loc_826EDF70;
loc_826EDFE0:
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EDFE8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EDFE8) { __imp__sub_826EDFE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EDFE8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x826EDFF0;
	__savegprlr_28(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r28,r3
	ctx.r28.u64 = ctx.r3.u64;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// li r30,0
	ctx.r30.s64 = 0;
	// lbz r11,5(r28)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r28.u32 + 5);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826ee0f4
	if (ctx.cr6.eq) goto loc_826EE0F4;
	// li r31,0
	ctx.r31.s64 = 0;
loc_826EE010:
	// lwz r11,20(r28)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r28.u32 + 20);
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// add r3,r11,r31
	ctx.r3.u64 = ctx.r11.u64 + ctx.r31.u64;
	// bl 0x82974470
	ctx.lr = 0x826EE020;
	sub_82974470(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826ee048
	if (!ctx.cr6.eq) goto loc_826EE048;
	// lbz r11,5(r28)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r28.u32 + 5);
	// addi r30,r30,1
	ctx.r30.s64 = ctx.r30.s64 + 1;
	// addi r31,r31,16
	ctx.r31.s64 = ctx.r31.s64 + 16;
	// cmplw cr6,r30,r11
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, ctx.r11.u32, ctx.xer);
	// blt cr6,0x826ee010
	if (ctx.cr6.lt) goto loc_826EE010;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
loc_826EE048:
	// lwz r10,20(r28)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r28.u32 + 20);
	// rlwinm r11,r30,4,0,27
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 4) & 0xFFFFFFF0;
	// add r3,r11,r10
	ctx.r3.u64 = ctx.r11.u64 + ctx.r10.u64;
	// bl 0x829743a8
	ctx.lr = 0x826EE058;
	sub_829743A8(ctx, base);
	// lbz r11,4(r28)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r28.u32 + 4);
	// lwz r7,12(r28)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r28.u32 + 12);
	// li r6,0
	ctx.r6.s64 = 0;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826ee0f4
	if (ctx.cr6.eq) goto loc_826EE0F4;
	// lis r11,-32768
	ctx.r11.s64 = -2147483648;
	// mulli r4,r30,240
	ctx.r4.s64 = static_cast<int64_t>(ctx.r30.u64 * static_cast<uint64_t>(240));
	// li r5,19
	ctx.r5.s64 = 19;
	// ori r8,r11,1
	ctx.r8.u64 = ctx.r11.u64 | 1;
	// li r3,17
	ctx.r3.s64 = 17;
	// li r31,-1
	ctx.r31.s64 = -1;
loc_826EE084:
	// add r11,r4,r7
	ctx.r11.u64 = ctx.r4.u64 + ctx.r7.u64;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r9,r11,4
	ctx.r9.s64 = ctx.r11.s64 + 4;
	// stw r5,80(r11)
	PPC_STORE_U32(ctx.r11.u32 + 80, ctx.r5.u32);
loc_826EE094:
	// stw r8,0(r9)
	PPC_STORE_U32(ctx.r9.u32 + 0, ctx.r8.u32);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// lwz r30,80(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 80);
	// addi r9,r9,4
	ctx.r9.s64 = ctx.r9.s64 + 4;
	// cmpw cr6,r10,r30
	ctx.cr6.compare<int32_t>(ctx.r10.s32, ctx.r30.s32, ctx.xer);
	// blt cr6,0x826ee094
	if (ctx.cr6.lt) goto loc_826EE094;
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r3,224(r11)
	PPC_STORE_U32(ctx.r11.u32 + 224, ctx.r3.u32);
	// addi r9,r11,88
	ctx.r9.s64 = ctx.r11.s64 + 88;
loc_826EE0B8:
	// std r8,0(r9)
	PPC_STORE_U64(ctx.r9.u32 + 0, ctx.r8.u64);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// lwz r30,224(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 224);
	// addi r9,r9,8
	ctx.r9.s64 = ctx.r9.s64 + 8;
	// cmpw cr6,r10,r30
	ctx.cr6.compare<int32_t>(ctx.r10.s32, ctx.r30.s32, ctx.xer);
	// blt cr6,0x826ee0b8
	if (ctx.cr6.lt) goto loc_826EE0B8;
	// stw r31,232(r11)
	PPC_STORE_U32(ctx.r11.u32 + 232, ctx.r31.u32);
	// lwz r11,0(r7)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826ee0f4
	if (ctx.cr6.eq) goto loc_826EE0F4;
	// lbz r10,4(r28)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r28.u32 + 4);
	// addi r6,r6,1
	ctx.r6.s64 = ctx.r6.s64 + 1;
	// mr r7,r11
	ctx.r7.u64 = ctx.r11.u64;
	// cmpw cr6,r6,r10
	ctx.cr6.compare<int32_t>(ctx.r6.s32, ctx.r10.s32, ctx.xer);
	// blt cr6,0x826ee084
	if (ctx.cr6.lt) goto loc_826EE084;
loc_826EE0F4:
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EE100);  /* forward decl */
PPC_WEAK_FUNC(sub_826EE100) { __imp__sub_826EE100(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EE100) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb10
	ctx.lr = 0x826EE108;
	__savegprlr_26(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// mr r28,r4
	ctx.r28.u64 = ctx.r4.u64;
	// mr r26,r5
	ctx.r26.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// li r29,0
	ctx.r29.s64 = 0;
	// lbz r11,5(r31)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r31.u32 + 5);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826ee198
	if (ctx.cr6.eq) goto loc_826EE198;
	// li r30,0
	ctx.r30.s64 = 0;
loc_826EE130:
	// lwz r11,20(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 20);
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// add r3,r11,r30
	ctx.r3.u64 = ctx.r11.u64 + ctx.r30.u64;
	// bl 0x82974470
	ctx.lr = 0x826EE140;
	sub_82974470(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826ee184
	if (ctx.cr6.eq) goto loc_826EE184;
	// lbz r9,4(r31)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r31.u32 + 4);
	// li r11,0
	ctx.r11.s64 = 0;
	// lwz r10,12(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 12);
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// ble cr6,0x826ee184
	if (!ctx.cr6.gt) goto loc_826EE184;
	// clrlwi r8,r27,24
	ctx.r8.u64 = ctx.r27.u32 & 0xFF;
loc_826EE164:
	// cmpw cr6,r11,r8
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r8.s32, ctx.xer);
	// beq cr6,0x826ee1a4
	if (ctx.cr6.eq) goto loc_826EE1A4;
	// lwz r10,0(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x826ee184
	if (ctx.cr6.eq) goto loc_826EE184;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// cmpw cr6,r11,r9
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r9.s32, ctx.xer);
	// blt cr6,0x826ee164
	if (ctx.cr6.lt) goto loc_826EE164;
loc_826EE184:
	// lbz r11,5(r31)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r31.u32 + 5);
	// addi r29,r29,1
	ctx.r29.s64 = ctx.r29.s64 + 1;
	// addi r30,r30,16
	ctx.r30.s64 = ctx.r30.s64 + 16;
	// cmplw cr6,r29,r11
	ctx.cr6.compare<uint32_t>(ctx.r29.u32, ctx.r11.u32, ctx.xer);
	// blt cr6,0x826ee130
	if (ctx.cr6.lt) goto loc_826EE130;
loc_826EE198:
	// lis r3,-32768
	ctx.r3.s64 = -2147483648;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
loc_826EE1A4:
	// cmplwi cr6,r26,18
	ctx.cr6.compare<uint32_t>(ctx.r26.u32, 18, ctx.xer);
	// bgt cr6,0x826ee1c8
	if (ctx.cr6.gt) goto loc_826EE1C8;
	// mulli r11,r29,60
	ctx.r11.s64 = static_cast<int64_t>(ctx.r29.u64 * static_cast<uint64_t>(60));
	// add r11,r11,r26
	ctx.r11.u64 = ctx.r11.u64 + ctx.r26.u64;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r3,r11,r10
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + ctx.r10.u32);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
loc_826EE1C8:
	// lwz r3,80(r1)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EE1D8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EE1D8) { __imp__sub_826EE1D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EE1D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb10
	ctx.lr = 0x826EE1E0;
	__savegprlr_26(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// mr r28,r4
	ctx.r28.u64 = ctx.r4.u64;
	// mr r27,r5
	ctx.r27.u64 = ctx.r5.u64;
	// mr r26,r6
	ctx.r26.u64 = ctx.r6.u64;
	// li r29,0
	ctx.r29.s64 = 0;
	// lbz r11,5(r31)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r31.u32 + 5);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826ee270
	if (ctx.cr6.eq) goto loc_826EE270;
	// li r30,0
	ctx.r30.s64 = 0;
loc_826EE208:
	// lwz r11,20(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 20);
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// add r3,r11,r30
	ctx.r3.u64 = ctx.r11.u64 + ctx.r30.u64;
	// bl 0x82974470
	ctx.lr = 0x826EE218;
	sub_82974470(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826ee25c
	if (ctx.cr6.eq) goto loc_826EE25C;
	// lbz r9,4(r31)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r31.u32 + 4);
	// li r11,0
	ctx.r11.s64 = 0;
	// lwz r10,12(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 12);
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// ble cr6,0x826ee25c
	if (!ctx.cr6.gt) goto loc_826EE25C;
	// clrlwi r8,r26,24
	ctx.r8.u64 = ctx.r26.u32 & 0xFF;
loc_826EE23C:
	// cmpw cr6,r11,r8
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r8.s32, ctx.xer);
	// beq cr6,0x826ee27c
	if (ctx.cr6.eq) goto loc_826EE27C;
	// lwz r10,0(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x826ee25c
	if (ctx.cr6.eq) goto loc_826EE25C;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// cmpw cr6,r11,r9
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r9.s32, ctx.xer);
	// blt cr6,0x826ee23c
	if (ctx.cr6.lt) goto loc_826EE23C;
loc_826EE25C:
	// lbz r11,5(r31)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r31.u32 + 5);
	// addi r29,r29,1
	ctx.r29.s64 = ctx.r29.s64 + 1;
	// addi r30,r30,16
	ctx.r30.s64 = ctx.r30.s64 + 16;
	// cmplw cr6,r29,r11
	ctx.cr6.compare<uint32_t>(ctx.r29.u32, ctx.r11.u32, ctx.xer);
	// blt cr6,0x826ee208
	if (ctx.cr6.lt) goto loc_826EE208;
loc_826EE270:
	// lis r3,-32768
	ctx.r3.s64 = -2147483648;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
loc_826EE27C:
	// cmpwi cr6,r27,19
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 19, ctx.xer);
	// blt cr6,0x826ee2a8
	if (ctx.cr6.lt) goto loc_826EE2A8;
	// cmpwi cr6,r27,36
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 36, ctx.xer);
	// bge cr6,0x826ee2a8
	if (!ctx.cr6.lt) goto loc_826EE2A8;
	// mulli r11,r29,30
	ctx.r11.s64 = static_cast<int64_t>(ctx.r29.u64 * static_cast<uint64_t>(30));
	// add r11,r11,r27
	ctx.r11.u64 = ctx.r11.u64 + ctx.r27.u64;
	// addi r11,r11,-8
	ctx.r11.s64 = ctx.r11.s64 + -8;
	// rlwinm r11,r11,3,0,28
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 3) & 0xFFFFFFF8;
	// ldx r3,r11,r10
	ctx.r3.u64 = PPC_LOAD_U64(ctx.r11.u32 + ctx.r10.u32);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
loc_826EE2A8:
	// ld r3,80(r1)
	ctx.r3.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EE2B8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EE2B8) { __imp__sub_826EE2B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EE2B8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eafc
	ctx.lr = 0x826EE2C0;
	__savegprlr_21(ctx, base);
	// stwu r1,-192(r1)
	ea = -192 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// mr r23,r4
	ctx.r23.u64 = ctx.r4.u64;
	// mr r26,r5
	ctx.r26.u64 = ctx.r5.u64;
	// mr r22,r6
	ctx.r22.u64 = ctx.r6.u64;
	// mr r25,r7
	ctx.r25.u64 = ctx.r7.u64;
	// lbz r11,5(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + 5);
	// mr r21,r8
	ctx.r21.u64 = ctx.r8.u64;
	// li r27,0
	ctx.r27.s64 = 0;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826ee3b0
	if (ctx.cr6.eq) goto loc_826EE3B0;
	// addi r11,r26,1
	ctx.r11.s64 = ctx.r26.s64 + 1;
	// lwz r31,80(r1)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// li r28,0
	ctx.r28.s64 = 0;
	// rlwinm r29,r11,2,0,29
	ctx.r29.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lis r11,-32768
	ctx.r11.s64 = -2147483648;
	// ori r24,r11,2
	ctx.r24.u64 = ctx.r11.u64 | 2;
loc_826EE304:
	// lwz r11,20(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 20);
	// mr r4,r23
	ctx.r4.u64 = ctx.r23.u64;
	// add r3,r11,r28
	ctx.r3.u64 = ctx.r11.u64 + ctx.r28.u64;
	// bl 0x82974470
	ctx.lr = 0x826EE314;
	sub_82974470(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826ee398
	if (ctx.cr6.eq) goto loc_826EE398;
	// lbz r9,4(r30)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r30.u32 + 4);
	// li r10,0
	ctx.r10.s64 = 0;
	// lwz r11,12(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 12);
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// beq cr6,0x826ee398
	if (ctx.cr6.eq) goto loc_826EE398;
	// clrlwi r9,r22,24
	ctx.r9.u64 = ctx.r22.u32 & 0xFF;
loc_826EE338:
	// cmpw cr6,r10,r9
	ctx.cr6.compare<int32_t>(ctx.r10.s32, ctx.r9.s32, ctx.xer);
	// bne cr6,0x826ee37c
	if (!ctx.cr6.eq) goto loc_826EE37C;
	// cmpwi cr6,r26,19
	ctx.cr6.compare<int32_t>(ctx.r26.s32, 19, ctx.xer);
	// bge cr6,0x826ee37c
	if (!ctx.cr6.lt) goto loc_826EE37C;
	// cmplwi cr6,r26,18
	ctx.cr6.compare<uint32_t>(ctx.r26.u32, 18, ctx.xer);
	// bgt cr6,0x826ee354
	if (ctx.cr6.gt) goto loc_826EE354;
	// lwzx r31,r29,r11
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r29.u32 + ctx.r11.u32);
loc_826EE354:
	// cmpw cr6,r31,r24
	ctx.cr6.compare<int32_t>(ctx.r31.s32, ctx.r24.s32, ctx.xer);
	// ble cr6,0x826ee36c
	if (!ctx.cr6.gt) goto loc_826EE36C;
	// clrlwi r8,r21,24
	ctx.r8.u64 = ctx.r21.u32 & 0xFF;
	// add r31,r31,r25
	ctx.r31.u64 = ctx.r31.u64 + ctx.r25.u64;
	// cmplwi cr6,r8,0
	ctx.cr6.compare<uint32_t>(ctx.r8.u32, 0, ctx.xer);
	// beq cr6,0x826ee370
	if (ctx.cr6.eq) goto loc_826EE370;
loc_826EE36C:
	// mr r31,r25
	ctx.r31.u64 = ctx.r25.u64;
loc_826EE370:
	// cmplwi cr6,r26,18
	ctx.cr6.compare<uint32_t>(ctx.r26.u32, 18, ctx.xer);
	// bgt cr6,0x826ee37c
	if (ctx.cr6.gt) goto loc_826EE37C;
	// stwx r31,r29,r11
	PPC_STORE_U32(ctx.r29.u32 + ctx.r11.u32, ctx.r31.u32);
loc_826EE37C:
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826ee398
	if (ctx.cr6.eq) goto loc_826EE398;
	// lbz r8,4(r30)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r30.u32 + 4);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// cmpw cr6,r10,r8
	ctx.cr6.compare<int32_t>(ctx.r10.s32, ctx.r8.s32, ctx.xer);
	// blt cr6,0x826ee338
	if (ctx.cr6.lt) goto loc_826EE338;
loc_826EE398:
	// lbz r11,5(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + 5);
	// addi r27,r27,1
	ctx.r27.s64 = ctx.r27.s64 + 1;
	// addi r28,r28,16
	ctx.r28.s64 = ctx.r28.s64 + 16;
	// addi r29,r29,240
	ctx.r29.s64 = ctx.r29.s64 + 240;
	// cmplw cr6,r27,r11
	ctx.cr6.compare<uint32_t>(ctx.r27.u32, ctx.r11.u32, ctx.xer);
	// blt cr6,0x826ee304
	if (ctx.cr6.lt) goto loc_826EE304;
loc_826EE3B0:
	// addi r1,r1,192
	ctx.r1.s64 = ctx.r1.s64 + 192;
	// b 0x8298eb4c
	__restgprlr_21(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EE3B8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EE3B8) { __imp__sub_826EE3B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EE3B8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eafc
	ctx.lr = 0x826EE3C0;
	__savegprlr_21(ctx, base);
	// stwu r1,-192(r1)
	ea = -192 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// mr r23,r4
	ctx.r23.u64 = ctx.r4.u64;
	// mr r26,r5
	ctx.r26.u64 = ctx.r5.u64;
	// mr r22,r6
	ctx.r22.u64 = ctx.r6.u64;
	// mr r25,r7
	ctx.r25.u64 = ctx.r7.u64;
	// lbz r11,5(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + 5);
	// mr r21,r8
	ctx.r21.u64 = ctx.r8.u64;
	// li r27,0
	ctx.r27.s64 = 0;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826ee4b0
	if (ctx.cr6.eq) goto loc_826EE4B0;
	// addi r11,r26,-8
	ctx.r11.s64 = ctx.r26.s64 + -8;
	// ld r31,80(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
	// li r28,0
	ctx.r28.s64 = 0;
	// rlwinm r29,r11,3,0,28
	ctx.r29.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 3) & 0xFFFFFFF8;
	// lis r11,-32768
	ctx.r11.s64 = -2147483648;
	// ori r24,r11,2
	ctx.r24.u64 = ctx.r11.u64 | 2;
loc_826EE404:
	// lwz r11,20(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 20);
	// mr r4,r23
	ctx.r4.u64 = ctx.r23.u64;
	// add r3,r11,r28
	ctx.r3.u64 = ctx.r11.u64 + ctx.r28.u64;
	// bl 0x82974470
	ctx.lr = 0x826EE414;
	sub_82974470(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826ee498
	if (ctx.cr6.eq) goto loc_826EE498;
	// lbz r9,4(r30)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r30.u32 + 4);
	// li r10,0
	ctx.r10.s64 = 0;
	// lwz r11,12(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 12);
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// beq cr6,0x826ee498
	if (ctx.cr6.eq) goto loc_826EE498;
	// clrlwi r9,r22,24
	ctx.r9.u64 = ctx.r22.u32 & 0xFF;
loc_826EE438:
	// cmpw cr6,r10,r9
	ctx.cr6.compare<int32_t>(ctx.r10.s32, ctx.r9.s32, ctx.xer);
	// bne cr6,0x826ee47c
	if (!ctx.cr6.eq) goto loc_826EE47C;
	// cmpwi cr6,r26,19
	ctx.cr6.compare<int32_t>(ctx.r26.s32, 19, ctx.xer);
	// blt cr6,0x826ee47c
	if (ctx.cr6.lt) goto loc_826EE47C;
	// cmpwi cr6,r26,36
	ctx.cr6.compare<int32_t>(ctx.r26.s32, 36, ctx.xer);
	// bge cr6,0x826ee454
	if (!ctx.cr6.lt) goto loc_826EE454;
	// ldx r31,r29,r11
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r29.u32 + ctx.r11.u32);
loc_826EE454:
	// cmpd cr6,r31,r24
	ctx.cr6.compare<int64_t>(ctx.r31.s64, ctx.r24.s64, ctx.xer);
	// ble cr6,0x826ee46c
	if (!ctx.cr6.gt) goto loc_826EE46C;
	// clrlwi r8,r21,24
	ctx.r8.u64 = ctx.r21.u32 & 0xFF;
	// add r31,r31,r25
	ctx.r31.u64 = ctx.r31.u64 + ctx.r25.u64;
	// cmplwi cr6,r8,0
	ctx.cr6.compare<uint32_t>(ctx.r8.u32, 0, ctx.xer);
	// beq cr6,0x826ee470
	if (ctx.cr6.eq) goto loc_826EE470;
loc_826EE46C:
	// mr r31,r25
	ctx.r31.u64 = ctx.r25.u64;
loc_826EE470:
	// cmpwi cr6,r26,36
	ctx.cr6.compare<int32_t>(ctx.r26.s32, 36, ctx.xer);
	// bge cr6,0x826ee47c
	if (!ctx.cr6.lt) goto loc_826EE47C;
	// stdx r31,r29,r11
	PPC_STORE_U64(ctx.r29.u32 + ctx.r11.u32, ctx.r31.u64);
loc_826EE47C:
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826ee498
	if (ctx.cr6.eq) goto loc_826EE498;
	// lbz r8,4(r30)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r30.u32 + 4);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// cmpw cr6,r10,r8
	ctx.cr6.compare<int32_t>(ctx.r10.s32, ctx.r8.s32, ctx.xer);
	// blt cr6,0x826ee438
	if (ctx.cr6.lt) goto loc_826EE438;
loc_826EE498:
	// lbz r11,5(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + 5);
	// addi r27,r27,1
	ctx.r27.s64 = ctx.r27.s64 + 1;
	// addi r28,r28,16
	ctx.r28.s64 = ctx.r28.s64 + 16;
	// addi r29,r29,240
	ctx.r29.s64 = ctx.r29.s64 + 240;
	// cmplw cr6,r27,r11
	ctx.cr6.compare<uint32_t>(ctx.r27.u32, ctx.r11.u32, ctx.xer);
	// blt cr6,0x826ee404
	if (ctx.cr6.lt) goto loc_826EE404;
loc_826EE4B0:
	// addi r1,r1,192
	ctx.r1.s64 = ctx.r1.s64 + 192;
	// b 0x8298eb4c
	__restgprlr_21(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EE4B8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EE4B8) { __imp__sub_826EE4B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EE4B8) {
	PPC_FUNC_PROLOGUE();
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x826EE4C0;
	__savegprlr_27(ctx, base);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// li r3,0
	ctx.r3.s64 = 0;
	// lbz r10,5(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 5);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x826ee5b4
	if (ctx.cr6.eq) goto loc_826EE5B4;
	// lwz r29,12(r11)
	ctx.r29.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// li r31,0
	ctx.r31.s64 = 0;
	// lbz r30,4(r11)
	ctx.r30.u64 = PPC_LOAD_U8(ctx.r11.u32 + 4);
	// lis r11,-32768
	ctx.r11.s64 = -2147483648;
	// mr r28,r10
	ctx.r28.u64 = ctx.r10.u64;
	// ori r5,r11,1
	ctx.r5.u64 = ctx.r11.u64 | 1;
loc_826EE4EC:
	// mr r7,r29
	ctx.r7.u64 = ctx.r29.u64;
	// li r6,0
	ctx.r6.s64 = 0;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// ble cr6,0x826ee5a4
	if (!ctx.cr6.gt) goto loc_826EE5A4;
loc_826EE4FC:
	// cmpw cr6,r4,r6
	ctx.cr6.compare<int32_t>(ctx.r4.s32, ctx.r6.s32, ctx.xer);
	// bne cr6,0x826ee578
	if (!ctx.cr6.eq) goto loc_826EE578;
	// add r8,r31,r7
	ctx.r8.u64 = ctx.r31.u64 + ctx.r7.u64;
	// li r11,0
	ctx.r11.s64 = 0;
	// lwz r9,80(r8)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r8.u32 + 80);
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// ble cr6,0x826ee538
	if (!ctx.cr6.gt) goto loc_826EE538;
	// addi r10,r8,4
	ctx.r10.s64 = ctx.r8.s64 + 4;
loc_826EE51C:
	// lwz r27,0(r10)
	ctx.r27.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cmpw cr6,r27,r5
	ctx.cr6.compare<int32_t>(ctx.r27.s32, ctx.r5.s32, ctx.xer);
	// bne cr6,0x826ee598
	if (!ctx.cr6.eq) goto loc_826EE598;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// addi r10,r10,4
	ctx.r10.s64 = ctx.r10.s64 + 4;
	// cmpw cr6,r11,r9
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r9.s32, ctx.xer);
	// blt cr6,0x826ee51c
	if (ctx.cr6.lt) goto loc_826EE51C;
loc_826EE538:
	// lwz r9,224(r8)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r8.u32 + 224);
	// li r10,0
	ctx.r10.s64 = 0;
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// ble cr6,0x826ee568
	if (!ctx.cr6.gt) goto loc_826EE568;
	// addi r11,r8,88
	ctx.r11.s64 = ctx.r8.s64 + 88;
loc_826EE54C:
	// ld r8,0(r11)
	ctx.r8.u64 = PPC_LOAD_U64(ctx.r11.u32 + 0);
	// cmpd cr6,r8,r5
	ctx.cr6.compare<int64_t>(ctx.r8.s64, ctx.r5.s64, ctx.xer);
	// bne cr6,0x826ee598
	if (!ctx.cr6.eq) goto loc_826EE598;
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// cmpw cr6,r10,r9
	ctx.cr6.compare<int32_t>(ctx.r10.s32, ctx.r9.s32, ctx.xer);
	// blt cr6,0x826ee54c
	if (ctx.cr6.lt) goto loc_826EE54C;
loc_826EE568:
	// li r11,1
	ctx.r11.s64 = 1;
loc_826EE56C:
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826ee5a0
	if (ctx.cr6.eq) goto loc_826EE5A0;
loc_826EE578:
	// lwz r11,0(r7)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826ee5a4
	if (ctx.cr6.eq) goto loc_826EE5A4;
	// addi r6,r6,1
	ctx.r6.s64 = ctx.r6.s64 + 1;
	// mr r7,r11
	ctx.r7.u64 = ctx.r11.u64;
	// cmpw cr6,r6,r30
	ctx.cr6.compare<int32_t>(ctx.r6.s32, ctx.r30.s32, ctx.xer);
	// blt cr6,0x826ee4fc
	if (ctx.cr6.lt) goto loc_826EE4FC;
	// b 0x826ee5a4
	goto loc_826EE5A4;
loc_826EE598:
	// li r11,0
	ctx.r11.s64 = 0;
	// b 0x826ee56c
	goto loc_826EE56C;
loc_826EE5A0:
	// addi r3,r3,1
	ctx.r3.s64 = ctx.r3.s64 + 1;
loc_826EE5A4:
	// addi r28,r28,-1
	ctx.r28.s64 = ctx.r28.s64 + -1;
	// addi r31,r31,240
	ctx.r31.s64 = ctx.r31.s64 + 240;
	// cmplwi cr6,r28,0
	ctx.cr6.compare<uint32_t>(ctx.r28.u32, 0, ctx.xer);
	// bne cr6,0x826ee4ec
	if (!ctx.cr6.eq) goto loc_826EE4EC;
loc_826EE5B4:
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EE5B8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EE5B8) { __imp__sub_826EE5B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EE5B8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eaf0
	ctx.lr = 0x826EE5C0;
	__savegprlr_18(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// mr r24,r4
	ctx.r24.u64 = ctx.r4.u64;
	// mr r21,r5
	ctx.r21.u64 = ctx.r5.u64;
	// li r25,0
	ctx.r25.s64 = 0;
	// lwz r11,0(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// rlwinm r10,r11,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// bne cr6,0x826ee884
	if (!ctx.cr6.eq) goto loc_826EE884;
	// rlwinm r11,r11,26,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 26) & 0x1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826ee884
	if (!ctx.cr6.eq) goto loc_826EE884;
	// lwz r11,4(r24)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r24.u32 + 4);
	// mr r18,r25
	ctx.r18.u64 = ctx.r25.u64;
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// ble cr6,0x826ee884
	if (!ctx.cr6.gt) goto loc_826EE884;
	// lis r19,-32768
	ctx.r19.s64 = -2147483648;
loc_826EE604:
	// lwz r11,0(r24)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r24.u32 + 0);
	// mr r4,r18
	ctx.r4.u64 = ctx.r18.u64;
	// mr r3,r24
	ctx.r3.u64 = ctx.r24.u64;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826EE61C;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r23,r3
	ctx.r23.u64 = ctx.r3.u64;
	// lwz r22,40(r23)
	ctx.r22.u64 = PPC_LOAD_U32(ctx.r23.u32 + 40);
	// mr r3,r22
	ctx.r3.u64 = ctx.r22.u64;
	// bl 0x82977c48
	ctx.lr = 0x826EE62C;
	sub_82977C48(ctx, base);
	// mr r20,r3
	ctx.r20.u64 = ctx.r3.u64;
	// mr r27,r25
	ctx.r27.u64 = ctx.r25.u64;
	// cmplwi cr6,r20,0
	ctx.cr6.compare<uint32_t>(ctx.r20.u32, 0, ctx.xer);
	// beq cr6,0x826ee874
	if (ctx.cr6.eq) goto loc_826EE874;
loc_826EE63C:
	// lwz r11,0(r24)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r24.u32 + 0);
	// mr r3,r24
	ctx.r3.u64 = ctx.r24.u64;
	// lwz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826EE650;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// lwz r11,28(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826EE664;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// lwz r11,0(r24)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r24.u32 + 0);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// mr r3,r24
	ctx.r3.u64 = ctx.r24.u64;
	// lwz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826EE67C;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// lwz r11,52(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 52);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826EE690;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// bl 0x826e6108
	ctx.lr = 0x826EE698;
	sub_826E6108(ctx, base);
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// cmpw cr6,r26,r19
	ctx.cr6.compare<int32_t>(ctx.r26.s32, ctx.r19.s32, ctx.xer);
	// beq cr6,0x826ee868
	if (ctx.cr6.eq) goto loc_826EE868;
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// mr r3,r22
	ctx.r3.u64 = ctx.r22.u64;
	// bl 0x82978318
	ctx.lr = 0x826EE6B0;
	sub_82978318(ctx, base);
	// cmplwi cr6,r3,1
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 1, ctx.xer);
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// mr r3,r22
	ctx.r3.u64 = ctx.r22.u64;
	// blt cr6,0x826ee7a0
	if (ctx.cr6.lt) goto loc_826EE7A0;
	// beq cr6,0x826ee6cc
	if (ctx.cr6.eq) goto loc_826EE6CC;
	// bl 0x82978318
	ctx.lr = 0x826EE6C8;
	sub_82978318(ctx, base);
	// b 0x826ee868
	goto loc_826EE868;
loc_826EE6CC:
	// li r6,8
	ctx.r6.s64 = 8;
	// std r25,80(r1)
	PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r25.u64);
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// bl 0x82978530
	ctx.lr = 0x826EE6DC;
	sub_82978530(ctx, base);
	// lbz r11,5(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + 5);
	// mr r28,r25
	ctx.r28.u64 = ctx.r25.u64;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826ee868
	if (ctx.cr6.eq) goto loc_826EE868;
	// addi r11,r26,-8
	ctx.r11.s64 = ctx.r26.s64 + -8;
	// mr r31,r25
	ctx.r31.u64 = ctx.r25.u64;
	// rlwinm r29,r11,3,0,28
	ctx.r29.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 3) & 0xFFFFFFF8;
loc_826EE6F8:
	// lwz r11,20(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 20);
	// add r3,r31,r11
	ctx.r3.u64 = ctx.r31.u64 + ctx.r11.u64;
	// bl 0x829743e8
	ctx.lr = 0x826EE704;
	sub_829743E8(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826ee784
	if (ctx.cr6.eq) goto loc_826EE784;
	// lwz r11,20(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 20);
	// addi r3,r23,24
	ctx.r3.s64 = ctx.r23.s64 + 24;
	// add r4,r31,r11
	ctx.r4.u64 = ctx.r31.u64 + ctx.r11.u64;
	// bl 0x82974470
	ctx.lr = 0x826EE720;
	sub_82974470(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826ee784
	if (ctx.cr6.eq) goto loc_826EE784;
	// lbz r8,4(r30)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r30.u32 + 4);
	// mr r10,r25
	ctx.r10.u64 = ctx.r25.u64;
	// lwz r9,12(r30)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r30.u32 + 12);
	// cmpwi cr6,r8,0
	ctx.cr6.compare<int32_t>(ctx.r8.s32, 0, ctx.xer);
	// ble cr6,0x826ee784
	if (!ctx.cr6.gt) goto loc_826EE784;
	// clrlwi r7,r21,24
	ctx.r7.u64 = ctx.r21.u32 & 0xFF;
loc_826EE744:
	// cmpw cr6,r10,r7
	ctx.cr6.compare<int32_t>(ctx.r10.s32, ctx.r7.s32, ctx.xer);
	// beq cr6,0x826ee76c
	if (ctx.cr6.eq) goto loc_826EE76C;
	// lwz r11,0(r9)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826ee75c
	if (ctx.cr6.eq) goto loc_826EE75C;
	// mr r9,r11
	ctx.r9.u64 = ctx.r11.u64;
loc_826EE75C:
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// cmpw cr6,r10,r8
	ctx.cr6.compare<int32_t>(ctx.r10.s32, ctx.r8.s32, ctx.xer);
	// blt cr6,0x826ee744
	if (ctx.cr6.lt) goto loc_826EE744;
	// b 0x826ee784
	goto loc_826EE784;
loc_826EE76C:
	// cmpwi cr6,r26,19
	ctx.cr6.compare<int32_t>(ctx.r26.s32, 19, ctx.xer);
	// blt cr6,0x826ee784
	if (ctx.cr6.lt) goto loc_826EE784;
	// cmpwi cr6,r26,36
	ctx.cr6.compare<int32_t>(ctx.r26.s32, 36, ctx.xer);
	// bge cr6,0x826ee784
	if (!ctx.cr6.lt) goto loc_826EE784;
	// ld r11,80(r1)
	ctx.r11.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
	// stdx r11,r29,r9
	PPC_STORE_U64(ctx.r29.u32 + ctx.r9.u32, ctx.r11.u64);
loc_826EE784:
	// lbz r11,5(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + 5);
	// addi r28,r28,1
	ctx.r28.s64 = ctx.r28.s64 + 1;
	// addi r31,r31,16
	ctx.r31.s64 = ctx.r31.s64 + 16;
	// addi r29,r29,240
	ctx.r29.s64 = ctx.r29.s64 + 240;
	// cmpw cr6,r28,r11
	ctx.cr6.compare<int32_t>(ctx.r28.s32, ctx.r11.s32, ctx.xer);
	// blt cr6,0x826ee6f8
	if (ctx.cr6.lt) goto loc_826EE6F8;
	// b 0x826ee868
	goto loc_826EE868;
loc_826EE7A0:
	// li r6,4
	ctx.r6.s64 = 4;
	// stw r25,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r25.u32);
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// bl 0x82978530
	ctx.lr = 0x826EE7B0;
	sub_82978530(ctx, base);
	// lbz r11,5(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + 5);
	// mr r28,r25
	ctx.r28.u64 = ctx.r25.u64;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826ee868
	if (ctx.cr6.eq) goto loc_826EE868;
	// mr r31,r25
	ctx.r31.u64 = ctx.r25.u64;
	// rlwinm r29,r26,2,0,29
	ctx.r29.u64 = __builtin_rotateleft64(ctx.r26.u32 | (ctx.r26.u64 << 32), 2) & 0xFFFFFFFC;
loc_826EE7C8:
	// lwz r11,20(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 20);
	// add r3,r31,r11
	ctx.r3.u64 = ctx.r31.u64 + ctx.r11.u64;
	// bl 0x829743e8
	ctx.lr = 0x826EE7D4;
	sub_829743E8(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826ee850
	if (ctx.cr6.eq) goto loc_826EE850;
	// lwz r11,20(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 20);
	// addi r3,r23,24
	ctx.r3.s64 = ctx.r23.s64 + 24;
	// add r4,r31,r11
	ctx.r4.u64 = ctx.r31.u64 + ctx.r11.u64;
	// bl 0x82974470
	ctx.lr = 0x826EE7F0;
	sub_82974470(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826ee850
	if (ctx.cr6.eq) goto loc_826EE850;
	// lbz r8,4(r30)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r30.u32 + 4);
	// mr r10,r25
	ctx.r10.u64 = ctx.r25.u64;
	// lwz r9,12(r30)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r30.u32 + 12);
	// cmpwi cr6,r8,0
	ctx.cr6.compare<int32_t>(ctx.r8.s32, 0, ctx.xer);
	// ble cr6,0x826ee850
	if (!ctx.cr6.gt) goto loc_826EE850;
	// clrlwi r7,r21,24
	ctx.r7.u64 = ctx.r21.u32 & 0xFF;
loc_826EE814:
	// cmpw cr6,r10,r7
	ctx.cr6.compare<int32_t>(ctx.r10.s32, ctx.r7.s32, ctx.xer);
	// beq cr6,0x826ee83c
	if (ctx.cr6.eq) goto loc_826EE83C;
	// lwz r11,0(r9)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826ee82c
	if (ctx.cr6.eq) goto loc_826EE82C;
	// mr r9,r11
	ctx.r9.u64 = ctx.r11.u64;
loc_826EE82C:
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// cmpw cr6,r10,r8
	ctx.cr6.compare<int32_t>(ctx.r10.s32, ctx.r8.s32, ctx.xer);
	// blt cr6,0x826ee814
	if (ctx.cr6.lt) goto loc_826EE814;
	// b 0x826ee850
	goto loc_826EE850;
loc_826EE83C:
	// cmplwi cr6,r26,18
	ctx.cr6.compare<uint32_t>(ctx.r26.u32, 18, ctx.xer);
	// bgt cr6,0x826ee850
	if (ctx.cr6.gt) goto loc_826EE850;
	// lwz r10,80(r1)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// add r11,r29,r9
	ctx.r11.u64 = ctx.r29.u64 + ctx.r9.u64;
	// stw r10,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
loc_826EE850:
	// lbz r11,5(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + 5);
	// addi r28,r28,1
	ctx.r28.s64 = ctx.r28.s64 + 1;
	// addi r31,r31,16
	ctx.r31.s64 = ctx.r31.s64 + 16;
	// addi r29,r29,240
	ctx.r29.s64 = ctx.r29.s64 + 240;
	// cmpw cr6,r28,r11
	ctx.cr6.compare<int32_t>(ctx.r28.s32, ctx.r11.s32, ctx.xer);
	// blt cr6,0x826ee7c8
	if (ctx.cr6.lt) goto loc_826EE7C8;
loc_826EE868:
	// addi r27,r27,1
	ctx.r27.s64 = ctx.r27.s64 + 1;
	// cmplw cr6,r27,r20
	ctx.cr6.compare<uint32_t>(ctx.r27.u32, ctx.r20.u32, ctx.xer);
	// blt cr6,0x826ee63c
	if (ctx.cr6.lt) goto loc_826EE63C;
loc_826EE874:
	// lwz r11,4(r24)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r24.u32 + 4);
	// addi r18,r18,1
	ctx.r18.s64 = ctx.r18.s64 + 1;
	// cmpw cr6,r18,r11
	ctx.cr6.compare<int32_t>(ctx.r18.s32, ctx.r11.s32, ctx.xer);
	// blt cr6,0x826ee604
	if (ctx.cr6.lt) goto loc_826EE604;
loc_826EE884:
	// lwz r10,8(r30)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r30.u32 + 8);
	// rlwinm r11,r21,3,21,28
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r21.u32 | (ctx.r21.u64 << 32), 3) & 0x7F8;
	// li r4,0
	ctx.r4.s64 = 0;
	// add r31,r11,r10
	ctx.r31.u64 = ctx.r11.u64 + ctx.r10.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x827df9f0
	ctx.lr = 0x826EE89C;
	sub_827DF9F0(ctx, base);
	// stw r25,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r25.u32);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x8298eb40
	__restgprlr_18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EE8A8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EE8A8) { __imp__sub_826EE8A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EE8A8) {
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
loc_826EE8C4:
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x826ee918
	if (ctx.cr6.eq) goto loc_826EE918;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826e6560
	ctx.lr = 0x826EE8D8;
	sub_826E6560(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826ee8ec
	if (!ctx.cr6.eq) goto loc_826EE8EC;
	// lwz r31,24(r31)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r31.u32 + 24);
	// b 0x826ee8c4
	goto loc_826EE8C4;
loc_826EE8EC:
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// rlwinm r11,r11,31,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 31) & 0x1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826ee918
	if (ctx.cr6.eq) goto loc_826EE918;
	// li r6,1
	ctx.r6.s64 = 1;
	// li r5,19
	ctx.r5.s64 = 19;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826ee1d8
	ctx.lr = 0x826EE910;
	sub_826EE1D8(ctx, base);
	// bl 0x826e6478
	ctx.lr = 0x826EE914;
	sub_826E6478(ctx, base);
	// b 0x826ee91c
	goto loc_826EE91C;
loc_826EE918:
	// li r3,-1
	ctx.r3.s64 = -1;
loc_826EE91C:
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

PPC_FUNC_IMPL(__imp__sub_826EE938);  /* forward decl */
PPC_WEAK_FUNC(sub_826EE938) { __imp__sub_826EE938(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EE938) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x826EE940;
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
	// bl 0x826e67b8
	ctx.lr = 0x826EE954;
	sub_826E67B8(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826ee9d8
	if (ctx.cr6.eq) goto loc_826EE9D8;
	// lwz r31,0(r31)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
loc_826EE964:
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x826ee9d8
	if (ctx.cr6.eq) goto loc_826EE9D8;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826e6560
	ctx.lr = 0x826EE978;
	sub_826E6560(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826ee98c
	if (!ctx.cr6.eq) goto loc_826EE98C;
	// lwz r31,24(r31)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r31.u32 + 24);
	// b 0x826ee964
	goto loc_826EE964;
loc_826EE98C:
	// cmpwi cr6,r29,19
	ctx.cr6.compare<int32_t>(ctx.r29.s32, 19, ctx.xer);
	// bge cr6,0x826ee9b0
	if (!ctx.cr6.lt) goto loc_826EE9B0;
	// li r6,1
	ctx.r6.s64 = 1;
	// mr r5,r29
	ctx.r5.u64 = ctx.r29.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826ee100
	ctx.lr = 0x826EE9A8;
	sub_826EE100(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
loc_826EE9B0:
	// cmpwi cr6,r29,36
	ctx.cr6.compare<int32_t>(ctx.r29.s32, 36, ctx.xer);
	// bge cr6,0x826ee9d8
	if (!ctx.cr6.lt) goto loc_826EE9D8;
	// li r6,1
	ctx.r6.s64 = 1;
	// mr r5,r29
	ctx.r5.u64 = ctx.r29.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826ee1d8
	ctx.lr = 0x826EE9CC;
	sub_826EE1D8(ctx, base);
	// extsw r3,r3
	ctx.r3.s64 = ctx.r3.s32;
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
loc_826EE9D8:
	// lis r3,-32768
	ctx.r3.s64 = -2147483648;
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EE9E8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EE9E8) { __imp__sub_826EE9E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EE9E8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x826EE9F0;
	__savegprlr_27(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r31,0(r3)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
loc_826EEA08:
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x826eea90
	if (ctx.cr6.eq) goto loc_826EEA90;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826e6560
	ctx.lr = 0x826EEA1C;
	sub_826E6560(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826eea30
	if (!ctx.cr6.eq) goto loc_826EEA30;
	// lwz r31,24(r31)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r31.u32 + 24);
	// b 0x826eea08
	goto loc_826EEA08;
loc_826EEA30:
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// rlwinm r11,r11,31,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 31) & 0x1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826eea90
	if (ctx.cr6.eq) goto loc_826EEA90;
	// cmpwi cr6,r29,19
	ctx.cr6.compare<int32_t>(ctx.r29.s32, 19, ctx.xer);
	// bge cr6,0x826eea6c
	if (!ctx.cr6.lt) goto loc_826EEA6C;
	// mr r8,r27
	ctx.r8.u64 = ctx.r27.u64;
	// extsw r7,r28
	ctx.r7.s64 = ctx.r28.s32;
	// li r6,1
	ctx.r6.s64 = 1;
	// mr r5,r29
	ctx.r5.u64 = ctx.r29.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826ee2b8
	ctx.lr = 0x826EEA64;
	sub_826EE2B8(ctx, base);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_826EEA6C:
	// cmpwi cr6,r29,36
	ctx.cr6.compare<int32_t>(ctx.r29.s32, 36, ctx.xer);
	// bge cr6,0x826eea90
	if (!ctx.cr6.lt) goto loc_826EEA90;
	// mr r8,r27
	ctx.r8.u64 = ctx.r27.u64;
	// mr r7,r28
	ctx.r7.u64 = ctx.r28.u64;
	// li r6,1
	ctx.r6.s64 = 1;
	// mr r5,r29
	ctx.r5.u64 = ctx.r29.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826ee3b8
	ctx.lr = 0x826EEA90;
	sub_826EE3B8(ctx, base);
loc_826EEA90:
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EEA98);  /* forward decl */
PPC_WEAK_FUNC(sub_826EEA98) { __imp__sub_826EEA98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EEA98) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x826EEAA0;
	__savegprlr_27(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// li r30,0
	ctx.r30.s64 = 0;
	// cmplwi cr6,r4,3
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 3, ctx.xer);
	// bgt cr6,0x826eeb04
	if (ctx.cr6.gt) goto loc_826EEB04;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-5420
	ctx.r12.s64 = ctx.r12.s64 + -5420;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826EEAE4;
	case 1:
		goto loc_826EEAEC;
	case 2:
		goto loc_826EEAF4;
	case 3:
		goto loc_826EEAFC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EEAE4:
	// li r11,61
	ctx.r11.s64 = 61;
	// b 0x826eeb08
	goto loc_826EEB08;
loc_826EEAEC:
	// li r11,62
	ctx.r11.s64 = 62;
	// b 0x826eeb08
	goto loc_826EEB08;
loc_826EEAF4:
	// li r11,63
	ctx.r11.s64 = 63;
	// b 0x826eeb08
	goto loc_826EEB08;
loc_826EEAFC:
	// li r11,64
	ctx.r11.s64 = 64;
	// b 0x826eeb08
	goto loc_826EEB08;
loc_826EEB04:
	// li r11,-1
	ctx.r11.s64 = -1;
loc_826EEB08:
	// addi r11,r11,-61
	ctx.r11.s64 = ctx.r11.s64 + -61;
	// cmplwi cr6,r11,3
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 3, ctx.xer);
	// bgt cr6,0x826eeb4c
	if (ctx.cr6.gt) goto loc_826EEB4C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-5332
	ctx.r12.s64 = ctx.r12.s64 + -5332;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EEB3C;
	case 1:
		goto loc_826EEB44;
	case 2:
		goto loc_826EEB3C;
	case 3:
		goto loc_826EEB3C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EEB3C:
	// li r31,2
	ctx.r31.s64 = 2;
	// b 0x826eeb50
	goto loc_826EEB50;
loc_826EEB44:
	// li r31,3
	ctx.r31.s64 = 3;
	// b 0x826eeb50
	goto loc_826EEB50;
loc_826EEB4C:
	// li r31,-1
	ctx.r31.s64 = -1;
loc_826EEB50:
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826EEB60;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826eec08
	if (ctx.cr6.gt) goto loc_826EEC08;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-5248
	ctx.r12.s64 = ctx.r12.s64 + -5248;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826EEB90;
	case 1:
		goto loc_826EEBB0;
	case 2:
		goto loc_826EEBD0;
	case 3:
		goto loc_826EEBF0;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EEB90:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9798
	ctx.lr = 0x826EEBA4;
	sub_826E9798(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_826EEBB0:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9390
	ctx.lr = 0x826EEBC4;
	sub_826E9390(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_826EEBD0:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9428
	ctx.lr = 0x826EEBE4;
	sub_826E9428(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_826EEBF0:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826EEC04;
	sub_826E95D8(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
loc_826EEC08:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EEC18);  /* forward decl */
PPC_WEAK_FUNC(sub_826EEC18) { __imp__sub_826EEC18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EEC18) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826EEC34;
	sub_829745B0(ctx, base);
	// addi r11,r31,48
	ctx.r11.s64 = ctx.r31.s64 + 48;
	// li r10,4
	ctx.r10.s64 = 4;
	// lis r8,8192
	ctx.r8.s64 = 536870912;
	// lis r7,8192
	ctx.r7.s64 = 536870912;
	// lis r6,12288
	ctx.r6.s64 = 805306368;
	// lbz r4,256(r11)
	ctx.r4.u64 = PPC_LOAD_U8(ctx.r11.u32 + 256);
	// lis r5,12288
	ctx.r5.s64 = 805306368;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,1
	ctx.r8.u64 = ctx.r8.u64 | 1;
	// ori r7,r7,2
	ctx.r7.u64 = ctx.r7.u64 | 2;
	// ori r6,r6,3
	ctx.r6.u64 = ctx.r6.u64 | 3;
	// ori r5,r5,4
	ctx.r5.u64 = ctx.r5.u64 | 4;
	// ori r10,r4,128
	ctx.r10.u64 = ctx.r4.u64 | 128;
	// std r9,4(r11)
	PPC_STORE_U64(ctx.r11.u32 + 4, ctx.r9.u64);
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r7,24(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24, ctx.r7.u32);
	// stw r6,36(r11)
	PPC_STORE_U32(ctx.r11.u32 + 36, ctx.r6.u32);
	// stw r5,48(r11)
	PPC_STORE_U32(ctx.r11.u32 + 48, ctx.r5.u32);
	// stb r10,256(r11)
	PPC_STORE_U8(ctx.r11.u32 + 256, ctx.r10.u8);
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EECA0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EECA0) { __imp__sub_826EECA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EECA0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x826EECA8;
	__savegprlr_28(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826EECB8;
	sub_829745B0(ctx, base);
	// addi r11,r31,48
	ctx.r11.s64 = ctx.r31.s64 + 48;
	// li r10,7
	ctx.r10.s64 = 7;
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// lis r8,4096
	ctx.r8.s64 = 268435456;
	// ori r30,r3,15
	ctx.r30.u64 = ctx.r3.u64 | 15;
	// lbz r28,256(r11)
	ctx.r28.u64 = PPC_LOAD_U8(ctx.r11.u32 + 256);
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// lis r6,8192
	ctx.r6.s64 = 536870912;
	// lis r5,8192
	ctx.r5.s64 = 536870912;
	// lis r4,4096
	ctx.r4.s64 = 268435456;
	// stw r30,60(r11)
	PPC_STORE_U32(ctx.r11.u32 + 60, ctx.r30.u32);
	// ori r29,r3,55
	ctx.r29.u64 = ctx.r3.u64 | 55;
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,38
	ctx.r8.u64 = ctx.r8.u64 | 38;
	// ori r7,r7,39
	ctx.r7.u64 = ctx.r7.u64 | 39;
	// ori r6,r6,44
	ctx.r6.u64 = ctx.r6.u64 | 44;
	// ori r5,r5,13
	ctx.r5.u64 = ctx.r5.u64 | 13;
	// stw r29,68(r11)
	PPC_STORE_U32(ctx.r11.u32 + 68, ctx.r29.u32);
	// ori r4,r4,14
	ctx.r4.u64 = ctx.r4.u64 | 14;
	// std r9,4(r11)
	PPC_STORE_U64(ctx.r11.u32 + 4, ctx.r9.u64);
	// ori r10,r28,128
	ctx.r10.u64 = ctx.r28.u64 | 128;
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// stw r7,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r7.u32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r6,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r6.u32);
	// stw r5,40(r11)
	PPC_STORE_U32(ctx.r11.u32 + 40, ctx.r5.u32);
	// stw r4,52(r11)
	PPC_STORE_U32(ctx.r11.u32 + 52, ctx.r4.u32);
	// stb r10,256(r11)
	PPC_STORE_U8(ctx.r11.u32 + 256, ctx.r10.u8);
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EED40);  /* forward decl */
PPC_WEAK_FUNC(sub_826EED40) { __imp__sub_826EED40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EED40) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826EED48;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r25,r4
	ctx.r25.u64 = ctx.r4.u64;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r30,r5
	ctx.r30.u64 = ctx.r5.u64;
	// mr r29,r6
	ctx.r29.u64 = ctx.r6.u64;
	// mr r28,r7
	ctx.r28.u64 = ctx.r7.u64;
	// li r31,0
	ctx.r31.s64 = 0;
	// bl 0x826e7470
	ctx.lr = 0x826EED6C;
	sub_826E7470(ctx, base);
	// bl 0x826e7718
	ctx.lr = 0x826EED70;
	sub_826E7718(ctx, base);
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// mr r4,r25
	ctx.r4.u64 = ctx.r25.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826EED8C;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826eee34
	if (ctx.cr6.gt) goto loc_826EEE34;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-4692
	ctx.r12.s64 = ctx.r12.s64 + -4692;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826EEDBC;
	case 1:
		goto loc_826EEDDC;
	case 2:
		goto loc_826EEDFC;
	case 3:
		goto loc_826EEE1C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EEDBC:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9798
	ctx.lr = 0x826EEDD0;
	sub_826E9798(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826EEDDC:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9390
	ctx.lr = 0x826EEDF0;
	sub_826E9390(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826EEDFC:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9428
	ctx.lr = 0x826EEE10;
	sub_826E9428(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826EEE1C:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826EEE30;
	sub_826E95D8(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
loc_826EEE34:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EEE40);  /* forward decl */
PPC_WEAK_FUNC(sub_826EEE40) { __imp__sub_826EEE40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EEE40) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826EEE48;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r25,r4
	ctx.r25.u64 = ctx.r4.u64;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r30,r5
	ctx.r30.u64 = ctx.r5.u64;
	// mr r29,r6
	ctx.r29.u64 = ctx.r6.u64;
	// mr r28,r7
	ctx.r28.u64 = ctx.r7.u64;
	// li r31,0
	ctx.r31.s64 = 0;
	// bl 0x826e7a40
	ctx.lr = 0x826EEE6C;
	sub_826E7A40(ctx, base);
	// bl 0x826e7718
	ctx.lr = 0x826EEE70;
	sub_826E7718(ctx, base);
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// mr r4,r25
	ctx.r4.u64 = ctx.r25.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826EEE8C;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826eef34
	if (ctx.cr6.gt) goto loc_826EEF34;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-4436
	ctx.r12.s64 = ctx.r12.s64 + -4436;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826EEEBC;
	case 1:
		goto loc_826EEEDC;
	case 2:
		goto loc_826EEEFC;
	case 3:
		goto loc_826EEF1C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EEEBC:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9798
	ctx.lr = 0x826EEED0;
	sub_826E9798(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826EEEDC:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9390
	ctx.lr = 0x826EEEF0;
	sub_826E9390(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826EEEFC:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9428
	ctx.lr = 0x826EEF10;
	sub_826E9428(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826EEF1C:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826EEF30;
	sub_826E95D8(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
loc_826EEF34:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EEF40);  /* forward decl */
PPC_WEAK_FUNC(sub_826EEF40) { __imp__sub_826EEF40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EEF40) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x826EEF48;
	__savegprlr_28(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826EEF58;
	sub_829745B0(ctx, base);
	// addi r11,r31,48
	ctx.r11.s64 = ctx.r31.s64 + 48;
	// li r10,7
	ctx.r10.s64 = 7;
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// lis r8,4096
	ctx.r8.s64 = 268435456;
	// ori r30,r3,14
	ctx.r30.u64 = ctx.r3.u64 | 14;
	// lbz r28,256(r11)
	ctx.r28.u64 = PPC_LOAD_U8(ctx.r11.u32 + 256);
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// lis r6,8192
	ctx.r6.s64 = 536870912;
	// lis r5,8192
	ctx.r5.s64 = 536870912;
	// lis r4,8192
	ctx.r4.s64 = 536870912;
	// stw r30,64(r11)
	PPC_STORE_U32(ctx.r11.u32 + 64, ctx.r30.u32);
	// ori r29,r3,15
	ctx.r29.u64 = ctx.r3.u64 | 15;
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,38
	ctx.r8.u64 = ctx.r8.u64 | 38;
	// ori r7,r7,39
	ctx.r7.u64 = ctx.r7.u64 | 39;
	// ori r6,r6,44
	ctx.r6.u64 = ctx.r6.u64 | 44;
	// ori r5,r5,13
	ctx.r5.u64 = ctx.r5.u64 | 13;
	// stw r29,72(r11)
	PPC_STORE_U32(ctx.r11.u32 + 72, ctx.r29.u32);
	// ori r4,r4,55
	ctx.r4.u64 = ctx.r4.u64 | 55;
	// std r9,4(r11)
	PPC_STORE_U64(ctx.r11.u32 + 4, ctx.r9.u64);
	// ori r10,r28,128
	ctx.r10.u64 = ctx.r28.u64 | 128;
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// stw r7,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r7.u32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r6,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r6.u32);
	// stw r5,40(r11)
	PPC_STORE_U32(ctx.r11.u32 + 40, ctx.r5.u32);
	// stw r4,52(r11)
	PPC_STORE_U32(ctx.r11.u32 + 52, ctx.r4.u32);
	// stb r10,256(r11)
	PPC_STORE_U8(ctx.r11.u32 + 256, ctx.r10.u8);
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EEFE0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EEFE0) { __imp__sub_826EEFE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EEFE0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826EEFE8;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r25,r4
	ctx.r25.u64 = ctx.r4.u64;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r30,r5
	ctx.r30.u64 = ctx.r5.u64;
	// mr r29,r6
	ctx.r29.u64 = ctx.r6.u64;
	// mr r28,r7
	ctx.r28.u64 = ctx.r7.u64;
	// li r31,0
	ctx.r31.s64 = 0;
	// bl 0x826e7908
	ctx.lr = 0x826EF00C;
	sub_826E7908(ctx, base);
	// bl 0x826e7718
	ctx.lr = 0x826EF010;
	sub_826E7718(ctx, base);
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// mr r4,r25
	ctx.r4.u64 = ctx.r25.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826EF02C;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826ef0d4
	if (ctx.cr6.gt) goto loc_826EF0D4;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-4020
	ctx.r12.s64 = ctx.r12.s64 + -4020;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826EF05C;
	case 1:
		goto loc_826EF07C;
	case 2:
		goto loc_826EF09C;
	case 3:
		goto loc_826EF0BC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EF05C:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9798
	ctx.lr = 0x826EF070;
	sub_826E9798(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826EF07C:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9390
	ctx.lr = 0x826EF090;
	sub_826E9390(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826EF09C:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9428
	ctx.lr = 0x826EF0B0;
	sub_826E9428(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826EF0BC:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826EF0D0;
	sub_826E95D8(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
loc_826EF0D4:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EF0E0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EF0E0) { __imp__sub_826EF0E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EF0E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826EF0E8;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r25,r4
	ctx.r25.u64 = ctx.r4.u64;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r30,r5
	ctx.r30.u64 = ctx.r5.u64;
	// mr r29,r6
	ctx.r29.u64 = ctx.r6.u64;
	// mr r28,r7
	ctx.r28.u64 = ctx.r7.u64;
	// li r31,0
	ctx.r31.s64 = 0;
	// bl 0x826e7908
	ctx.lr = 0x826EF10C;
	sub_826E7908(ctx, base);
	// bl 0x826e7e30
	ctx.lr = 0x826EF110;
	sub_826E7E30(ctx, base);
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// mr r4,r25
	ctx.r4.u64 = ctx.r25.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826EF12C;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826ef1d4
	if (ctx.cr6.gt) goto loc_826EF1D4;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-3764
	ctx.r12.s64 = ctx.r12.s64 + -3764;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826EF15C;
	case 1:
		goto loc_826EF17C;
	case 2:
		goto loc_826EF19C;
	case 3:
		goto loc_826EF1BC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EF15C:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9798
	ctx.lr = 0x826EF170;
	sub_826E9798(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826EF17C:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9390
	ctx.lr = 0x826EF190;
	sub_826E9390(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826EF19C:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9428
	ctx.lr = 0x826EF1B0;
	sub_826E9428(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826EF1BC:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826EF1D0;
	sub_826E95D8(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
loc_826EF1D4:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EF1E0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EF1E0) { __imp__sub_826EF1E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EF1E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x826EF1E8;
	__savegprlr_27(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// li r30,0
	ctx.r30.s64 = 0;
	// cmplwi cr6,r4,4
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 4, ctx.xer);
	// bgt cr6,0x826ef238
	if (ctx.cr6.gt) goto loc_826EF238;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-3556
	ctx.r12.s64 = ctx.r12.s64 + -3556;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826EF248;
	case 1:
		goto loc_826EF248;
	case 2:
		goto loc_826EF230;
	case 3:
		goto loc_826EF248;
	case 4:
		goto loc_826EF240;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EF230:
	// li r31,0
	ctx.r31.s64 = 0;
	// b 0x826ef24c
	goto loc_826EF24C;
loc_826EF238:
	// li r31,-1
	ctx.r31.s64 = -1;
	// b 0x826ef24c
	goto loc_826EF24C;
loc_826EF240:
	// li r31,1
	ctx.r31.s64 = 1;
	// b 0x826ef24c
	goto loc_826EF24C;
loc_826EF248:
	// li r31,3
	ctx.r31.s64 = 3;
loc_826EF24C:
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826EF25C;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826ef304
	if (ctx.cr6.gt) goto loc_826EF304;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-3460
	ctx.r12.s64 = ctx.r12.s64 + -3460;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826EF28C;
	case 1:
		goto loc_826EF2AC;
	case 2:
		goto loc_826EF2CC;
	case 3:
		goto loc_826EF2EC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EF28C:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9798
	ctx.lr = 0x826EF2A0;
	sub_826E9798(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_826EF2AC:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9390
	ctx.lr = 0x826EF2C0;
	sub_826E9390(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_826EF2CC:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9428
	ctx.lr = 0x826EF2E0;
	sub_826E9428(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_826EF2EC:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826EF300;
	sub_826E95D8(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
loc_826EF304:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EF310);  /* forward decl */
PPC_WEAK_FUNC(sub_826EF310) { __imp__sub_826EF310(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EF310) {
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
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826EF330;
	sub_829745B0(ctx, base);
	// addi r11,r31,48
	ctx.r11.s64 = ctx.r31.s64 + 48;
	// li r10,5
	ctx.r10.s64 = 5;
	// lis r8,4096
	ctx.r8.s64 = 268435456;
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// lis r6,8192
	ctx.r6.s64 = 536870912;
	// lbz r30,256(r11)
	ctx.r30.u64 = PPC_LOAD_U8(ctx.r11.u32 + 256);
	// lis r5,8192
	ctx.r5.s64 = 536870912;
	// lis r4,8192
	ctx.r4.s64 = 536870912;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,38
	ctx.r8.u64 = ctx.r8.u64 | 38;
	// ori r7,r7,39
	ctx.r7.u64 = ctx.r7.u64 | 39;
	// ori r6,r6,44
	ctx.r6.u64 = ctx.r6.u64 | 44;
	// ori r5,r5,13
	ctx.r5.u64 = ctx.r5.u64 | 13;
	// ori r4,r4,55
	ctx.r4.u64 = ctx.r4.u64 | 55;
	// std r9,4(r11)
	PPC_STORE_U64(ctx.r11.u32 + 4, ctx.r9.u64);
	// ori r10,r30,128
	ctx.r10.u64 = ctx.r30.u64 | 128;
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// stw r7,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r7.u32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r6,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r6.u32);
	// stw r5,40(r11)
	PPC_STORE_U32(ctx.r11.u32 + 40, ctx.r5.u32);
	// stw r4,52(r11)
	PPC_STORE_U32(ctx.r11.u32 + 52, ctx.r4.u32);
	// stb r10,256(r11)
	PPC_STORE_U8(ctx.r11.u32 + 256, ctx.r10.u8);
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
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

PPC_FUNC_IMPL(__imp__sub_826EF3B0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EF3B0) { __imp__sub_826EF3B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EF3B0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x826EF3B8;
	__savegprlr_27(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// li r30,0
	ctx.r30.s64 = 0;
	// cmplwi cr6,r4,4
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 4, ctx.xer);
	// bgt cr6,0x826ef408
	if (ctx.cr6.gt) goto loc_826EF408;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-3092
	ctx.r12.s64 = ctx.r12.s64 + -3092;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826EF418;
	case 1:
		goto loc_826EF418;
	case 2:
		goto loc_826EF418;
	case 3:
		goto loc_826EF410;
	case 4:
		goto loc_826EF400;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EF400:
	// li r31,0
	ctx.r31.s64 = 0;
	// b 0x826ef41c
	goto loc_826EF41C;
loc_826EF408:
	// li r31,-1
	ctx.r31.s64 = -1;
	// b 0x826ef41c
	goto loc_826EF41C;
loc_826EF410:
	// li r31,1
	ctx.r31.s64 = 1;
	// b 0x826ef41c
	goto loc_826EF41C;
loc_826EF418:
	// li r31,3
	ctx.r31.s64 = 3;
loc_826EF41C:
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826EF42C;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826ef4d4
	if (ctx.cr6.gt) goto loc_826EF4D4;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-2996
	ctx.r12.s64 = ctx.r12.s64 + -2996;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826EF45C;
	case 1:
		goto loc_826EF47C;
	case 2:
		goto loc_826EF49C;
	case 3:
		goto loc_826EF4BC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EF45C:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9798
	ctx.lr = 0x826EF470;
	sub_826E9798(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_826EF47C:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9390
	ctx.lr = 0x826EF490;
	sub_826E9390(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_826EF49C:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9428
	ctx.lr = 0x826EF4B0;
	sub_826E9428(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_826EF4BC:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826EF4D0;
	sub_826E95D8(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
loc_826EF4D4:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EF4E0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EF4E0) { __imp__sub_826EF4E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EF4E0) {
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
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826EF500;
	sub_829745B0(ctx, base);
	// addi r11,r31,48
	ctx.r11.s64 = ctx.r31.s64 + 48;
	// li r10,5
	ctx.r10.s64 = 5;
	// lis r8,4096
	ctx.r8.s64 = 268435456;
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// lis r6,8192
	ctx.r6.s64 = 536870912;
	// lbz r30,256(r11)
	ctx.r30.u64 = PPC_LOAD_U8(ctx.r11.u32 + 256);
	// lis r5,8192
	ctx.r5.s64 = 536870912;
	// lis r4,8192
	ctx.r4.s64 = 536870912;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,38
	ctx.r8.u64 = ctx.r8.u64 | 38;
	// ori r7,r7,39
	ctx.r7.u64 = ctx.r7.u64 | 39;
	// ori r6,r6,13
	ctx.r6.u64 = ctx.r6.u64 | 13;
	// ori r5,r5,55
	ctx.r5.u64 = ctx.r5.u64 | 55;
	// ori r4,r4,44
	ctx.r4.u64 = ctx.r4.u64 | 44;
	// std r9,4(r11)
	PPC_STORE_U64(ctx.r11.u32 + 4, ctx.r9.u64);
	// ori r10,r30,128
	ctx.r10.u64 = ctx.r30.u64 | 128;
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// stw r7,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r7.u32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r6,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r6.u32);
	// stw r5,40(r11)
	PPC_STORE_U32(ctx.r11.u32 + 40, ctx.r5.u32);
	// stw r4,52(r11)
	PPC_STORE_U32(ctx.r11.u32 + 52, ctx.r4.u32);
	// stb r10,256(r11)
	PPC_STORE_U8(ctx.r11.u32 + 256, ctx.r10.u8);
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
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

PPC_FUNC_IMPL(__imp__sub_826EF580);  /* forward decl */
PPC_WEAK_FUNC(sub_826EF580) { __imp__sub_826EF580(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EF580) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826EF588;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r26,r4
	ctx.r26.u64 = ctx.r4.u64;
	// mr r25,r3
	ctx.r25.u64 = ctx.r3.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// li r30,0
	ctx.r30.s64 = 0;
	// bl 0x826e8708
	ctx.lr = 0x826EF5AC;
	sub_826E8708(ctx, base);
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826ef60c
	if (ctx.cr6.gt) goto loc_826EF60C;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826ef604
	if (ctx.cr6.eq) goto loc_826EF604;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,6
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 6, ctx.xer);
	// bgt cr6,0x826ef60c
	if (ctx.cr6.gt) goto loc_826EF60C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-2584
	ctx.r12.s64 = ctx.r12.s64 + -2584;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EF604;
	case 1:
		goto loc_826EF604;
	case 2:
		goto loc_826EF604;
	case 3:
		goto loc_826EF604;
	case 4:
		goto loc_826EF604;
	case 5:
		goto loc_826EF60C;
	case 6:
		goto loc_826EF604;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EF604:
	// li r31,3
	ctx.r31.s64 = 3;
	// b 0x826ef610
	goto loc_826EF610;
loc_826EF60C:
	// li r31,-1
	ctx.r31.s64 = -1;
loc_826EF610:
	// lwz r11,0(r25)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r25.u32 + 0);
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826EF628;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826ef6d0
	if (ctx.cr6.gt) goto loc_826EF6D0;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-2488
	ctx.r12.s64 = ctx.r12.s64 + -2488;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826EF658;
	case 1:
		goto loc_826EF678;
	case 2:
		goto loc_826EF698;
	case 3:
		goto loc_826EF6B8;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EF658:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9798
	ctx.lr = 0x826EF66C;
	sub_826E9798(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826EF678:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9390
	ctx.lr = 0x826EF68C;
	sub_826E9390(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826EF698:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9428
	ctx.lr = 0x826EF6AC;
	sub_826E9428(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826EF6B8:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826EF6CC;
	sub_826E95D8(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
loc_826EF6D0:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EF6E0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EF6E0) { __imp__sub_826EF6E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EF6E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x826EF6E8;
	__savegprlr_28(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826EF6F8;
	sub_829745B0(ctx, base);
	// addi r11,r31,48
	ctx.r11.s64 = ctx.r31.s64 + 48;
	// li r10,7
	ctx.r10.s64 = 7;
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// lis r8,4096
	ctx.r8.s64 = 268435456;
	// ori r30,r3,15
	ctx.r30.u64 = ctx.r3.u64 | 15;
	// lbz r28,256(r11)
	ctx.r28.u64 = PPC_LOAD_U8(ctx.r11.u32 + 256);
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// lis r6,8192
	ctx.r6.s64 = 536870912;
	// lis r5,4096
	ctx.r5.s64 = 268435456;
	// lis r4,4096
	ctx.r4.s64 = 268435456;
	// stw r30,56(r11)
	PPC_STORE_U32(ctx.r11.u32 + 56, ctx.r30.u32);
	// ori r29,r3,55
	ctx.r29.u64 = ctx.r3.u64 | 55;
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,38
	ctx.r8.u64 = ctx.r8.u64 | 38;
	// ori r7,r7,39
	ctx.r7.u64 = ctx.r7.u64 | 39;
	// ori r6,r6,13
	ctx.r6.u64 = ctx.r6.u64 | 13;
	// ori r5,r5,34
	ctx.r5.u64 = ctx.r5.u64 | 34;
	// stw r29,64(r11)
	PPC_STORE_U32(ctx.r11.u32 + 64, ctx.r29.u32);
	// ori r4,r4,14
	ctx.r4.u64 = ctx.r4.u64 | 14;
	// std r9,4(r11)
	PPC_STORE_U64(ctx.r11.u32 + 4, ctx.r9.u64);
	// ori r10,r28,128
	ctx.r10.u64 = ctx.r28.u64 | 128;
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// stw r7,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r7.u32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r6,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r6.u32);
	// stw r5,40(r11)
	PPC_STORE_U32(ctx.r11.u32 + 40, ctx.r5.u32);
	// stw r4,48(r11)
	PPC_STORE_U32(ctx.r11.u32 + 48, ctx.r4.u32);
	// stb r10,256(r11)
	PPC_STORE_U8(ctx.r11.u32 + 256, ctx.r10.u8);
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EF780);  /* forward decl */
PPC_WEAK_FUNC(sub_826EF780) { __imp__sub_826EF780(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EF780) {
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
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826EF7A0;
	sub_829745B0(ctx, base);
	// addi r11,r31,48
	ctx.r11.s64 = ctx.r31.s64 + 48;
	// li r10,5
	ctx.r10.s64 = 5;
	// lis r8,8192
	ctx.r8.s64 = 536870912;
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// lis r6,4096
	ctx.r6.s64 = 268435456;
	// lbz r30,256(r11)
	ctx.r30.u64 = PPC_LOAD_U8(ctx.r11.u32 + 256);
	// lis r5,4096
	ctx.r5.s64 = 268435456;
	// lis r4,8192
	ctx.r4.s64 = 536870912;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,13
	ctx.r8.u64 = ctx.r8.u64 | 13;
	// ori r7,r7,34
	ctx.r7.u64 = ctx.r7.u64 | 34;
	// ori r6,r6,14
	ctx.r6.u64 = ctx.r6.u64 | 14;
	// ori r5,r5,15
	ctx.r5.u64 = ctx.r5.u64 | 15;
	// ori r4,r4,55
	ctx.r4.u64 = ctx.r4.u64 | 55;
	// std r9,4(r11)
	PPC_STORE_U64(ctx.r11.u32 + 4, ctx.r9.u64);
	// ori r10,r30,128
	ctx.r10.u64 = ctx.r30.u64 | 128;
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// stw r7,24(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24, ctx.r7.u32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r6,32(r11)
	PPC_STORE_U32(ctx.r11.u32 + 32, ctx.r6.u32);
	// stw r5,40(r11)
	PPC_STORE_U32(ctx.r11.u32 + 40, ctx.r5.u32);
	// stw r4,48(r11)
	PPC_STORE_U32(ctx.r11.u32 + 48, ctx.r4.u32);
	// stb r10,256(r11)
	PPC_STORE_U8(ctx.r11.u32 + 256, ctx.r10.u8);
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
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

PPC_FUNC_IMPL(__imp__sub_826EF820);  /* forward decl */
PPC_WEAK_FUNC(sub_826EF820) { __imp__sub_826EF820(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EF820) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826EF828;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r26,r4
	ctx.r26.u64 = ctx.r4.u64;
	// mr r25,r3
	ctx.r25.u64 = ctx.r3.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// li r30,0
	ctx.r30.s64 = 0;
	// bl 0x826e89c8
	ctx.lr = 0x826EF84C;
	sub_826E89C8(ctx, base);
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826ef8a4
	if (ctx.cr6.gt) goto loc_826EF8A4;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826ef89c
	if (ctx.cr6.eq) goto loc_826EF89C;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826ef8a4
	if (ctx.cr6.gt) goto loc_826EF8A4;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-1912
	ctx.r12.s64 = ctx.r12.s64 + -1912;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EF89C;
	case 1:
		goto loc_826EF89C;
	case 2:
		goto loc_826EF89C;
	case 3:
		goto loc_826EF89C;
	case 4:
		goto loc_826EF89C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EF89C:
	// li r31,3
	ctx.r31.s64 = 3;
	// b 0x826ef8a8
	goto loc_826EF8A8;
loc_826EF8A4:
	// li r31,-1
	ctx.r31.s64 = -1;
loc_826EF8A8:
	// lwz r11,0(r25)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r25.u32 + 0);
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826EF8C0;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826ef968
	if (ctx.cr6.gt) goto loc_826EF968;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-1824
	ctx.r12.s64 = ctx.r12.s64 + -1824;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826EF8F0;
	case 1:
		goto loc_826EF910;
	case 2:
		goto loc_826EF930;
	case 3:
		goto loc_826EF950;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EF8F0:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9798
	ctx.lr = 0x826EF904;
	sub_826E9798(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826EF910:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9390
	ctx.lr = 0x826EF924;
	sub_826E9390(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826EF930:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9428
	ctx.lr = 0x826EF944;
	sub_826E9428(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826EF950:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826EF964;
	sub_826E95D8(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
loc_826EF968:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EF978);  /* forward decl */
PPC_WEAK_FUNC(sub_826EF978) { __imp__sub_826EF978(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EF978) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x826EF980;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826EF990;
	sub_829745B0(ctx, base);
	// addi r11,r31,48
	ctx.r11.s64 = ctx.r31.s64 + 48;
	// li r10,6
	ctx.r10.s64 = 6;
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// lis r8,4096
	ctx.r8.s64 = 268435456;
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// lbz r29,256(r11)
	ctx.r29.u64 = PPC_LOAD_U8(ctx.r11.u32 + 256);
	// lis r6,8192
	ctx.r6.s64 = 536870912;
	// lis r5,4096
	ctx.r5.s64 = 268435456;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// lis r4,4096
	ctx.r4.s64 = 268435456;
	// ori r30,r3,55
	ctx.r30.u64 = ctx.r3.u64 | 55;
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,38
	ctx.r8.u64 = ctx.r8.u64 | 38;
	// ori r7,r7,39
	ctx.r7.u64 = ctx.r7.u64 | 39;
	// ori r6,r6,13
	ctx.r6.u64 = ctx.r6.u64 | 13;
	// ori r5,r5,14
	ctx.r5.u64 = ctx.r5.u64 | 14;
	// stw r30,56(r11)
	PPC_STORE_U32(ctx.r11.u32 + 56, ctx.r30.u32);
	// ori r4,r4,15
	ctx.r4.u64 = ctx.r4.u64 | 15;
	// std r9,4(r11)
	PPC_STORE_U64(ctx.r11.u32 + 4, ctx.r9.u64);
	// ori r10,r29,128
	ctx.r10.u64 = ctx.r29.u64 | 128;
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// stw r7,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r7.u32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r6,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r6.u32);
	// stw r5,40(r11)
	PPC_STORE_U32(ctx.r11.u32 + 40, ctx.r5.u32);
	// stw r4,48(r11)
	PPC_STORE_U32(ctx.r11.u32 + 48, ctx.r4.u32);
	// stb r10,256(r11)
	PPC_STORE_U8(ctx.r11.u32 + 256, ctx.r10.u8);
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EFA08);  /* forward decl */
PPC_WEAK_FUNC(sub_826EFA08) { __imp__sub_826EFA08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EFA08) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x826EFA10;
	__savegprlr_27(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// li r30,0
	ctx.r30.s64 = 0;
	// cmplwi cr6,r4,3
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 3, ctx.xer);
	// bgt cr6,0x826efa54
	if (ctx.cr6.gt) goto loc_826EFA54;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-1468
	ctx.r12.s64 = ctx.r12.s64 + -1468;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826EFA5C;
	case 1:
		goto loc_826EFA5C;
	case 2:
		goto loc_826EFA5C;
	case 3:
		goto loc_826EFA5C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EFA54:
	// li r31,-1
	ctx.r31.s64 = -1;
	// b 0x826efa60
	goto loc_826EFA60;
loc_826EFA5C:
	// li r31,2
	ctx.r31.s64 = 2;
loc_826EFA60:
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826EFA70;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826efb18
	if (ctx.cr6.gt) goto loc_826EFB18;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-1392
	ctx.r12.s64 = ctx.r12.s64 + -1392;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826EFAA0;
	case 1:
		goto loc_826EFAC0;
	case 2:
		goto loc_826EFAE0;
	case 3:
		goto loc_826EFB00;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EFAA0:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9798
	ctx.lr = 0x826EFAB4;
	sub_826E9798(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_826EFAC0:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9390
	ctx.lr = 0x826EFAD4;
	sub_826E9390(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_826EFAE0:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9428
	ctx.lr = 0x826EFAF4;
	sub_826E9428(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_826EFB00:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826EFB14;
	sub_826E95D8(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
loc_826EFB18:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EFB28);  /* forward decl */
PPC_WEAK_FUNC(sub_826EFB28) { __imp__sub_826EFB28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EFB28) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826EFB44;
	sub_829745B0(ctx, base);
	// addi r11,r31,48
	ctx.r11.s64 = ctx.r31.s64 + 48;
	// li r10,4
	ctx.r10.s64 = 4;
	// lis r8,8192
	ctx.r8.s64 = 536870912;
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// lis r6,4096
	ctx.r6.s64 = 268435456;
	// lbz r4,256(r11)
	ctx.r4.u64 = PPC_LOAD_U8(ctx.r11.u32 + 256);
	// lis r5,8192
	ctx.r5.s64 = 536870912;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,13
	ctx.r8.u64 = ctx.r8.u64 | 13;
	// ori r7,r7,14
	ctx.r7.u64 = ctx.r7.u64 | 14;
	// ori r6,r6,15
	ctx.r6.u64 = ctx.r6.u64 | 15;
	// ori r5,r5,55
	ctx.r5.u64 = ctx.r5.u64 | 55;
	// ori r10,r4,128
	ctx.r10.u64 = ctx.r4.u64 | 128;
	// std r9,4(r11)
	PPC_STORE_U64(ctx.r11.u32 + 4, ctx.r9.u64);
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r7,24(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24, ctx.r7.u32);
	// stw r6,32(r11)
	PPC_STORE_U32(ctx.r11.u32 + 32, ctx.r6.u32);
	// stw r5,40(r11)
	PPC_STORE_U32(ctx.r11.u32 + 40, ctx.r5.u32);
	// stb r10,256(r11)
	PPC_STORE_U8(ctx.r11.u32 + 256, ctx.r10.u8);
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EFBB0);  /* forward decl */
PPC_WEAK_FUNC(sub_826EFBB0) { __imp__sub_826EFBB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EFBB0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826EFBB8;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r26,r4
	ctx.r26.u64 = ctx.r4.u64;
	// mr r25,r3
	ctx.r25.u64 = ctx.r3.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// li r30,0
	ctx.r30.s64 = 0;
	// bl 0x826e8da0
	ctx.lr = 0x826EFBDC;
	sub_826E8DA0(ctx, base);
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826efc38
	if (ctx.cr6.gt) goto loc_826EFC38;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826efc30
	if (ctx.cr6.eq) goto loc_826EFC30;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x826efc38
	if (ctx.cr6.gt) goto loc_826EFC38;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-1000
	ctx.r12.s64 = ctx.r12.s64 + -1000;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EFC30;
	case 1:
		goto loc_826EFC30;
	case 2:
		goto loc_826EFC30;
	case 3:
		goto loc_826EFC30;
	case 4:
		goto loc_826EFC30;
	case 5:
		goto loc_826EFC30;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EFC30:
	// li r31,3
	ctx.r31.s64 = 3;
	// b 0x826efc3c
	goto loc_826EFC3C;
loc_826EFC38:
	// li r31,-1
	ctx.r31.s64 = -1;
loc_826EFC3C:
	// lwz r11,0(r25)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r25.u32 + 0);
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826EFC54;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826efcfc
	if (ctx.cr6.gt) goto loc_826EFCFC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-908
	ctx.r12.s64 = ctx.r12.s64 + -908;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826EFC84;
	case 1:
		goto loc_826EFCA4;
	case 2:
		goto loc_826EFCC4;
	case 3:
		goto loc_826EFCE4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EFC84:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9798
	ctx.lr = 0x826EFC98;
	sub_826E9798(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826EFCA4:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9390
	ctx.lr = 0x826EFCB8;
	sub_826E9390(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826EFCC4:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9428
	ctx.lr = 0x826EFCD8;
	sub_826E9428(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826EFCE4:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826EFCF8;
	sub_826E95D8(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
loc_826EFCFC:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EFD08);  /* forward decl */
PPC_WEAK_FUNC(sub_826EFD08) { __imp__sub_826EFD08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EFD08) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x826EFD10;
	__savegprlr_28(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826EFD20;
	sub_829745B0(ctx, base);
	// addi r11,r31,48
	ctx.r11.s64 = ctx.r31.s64 + 48;
	// li r10,7
	ctx.r10.s64 = 7;
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// lis r8,4096
	ctx.r8.s64 = 268435456;
	// ori r30,r3,15
	ctx.r30.u64 = ctx.r3.u64 | 15;
	// lbz r28,256(r11)
	ctx.r28.u64 = PPC_LOAD_U8(ctx.r11.u32 + 256);
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// lis r6,8192
	ctx.r6.s64 = 536870912;
	// lis r5,4096
	ctx.r5.s64 = 268435456;
	// lis r4,4096
	ctx.r4.s64 = 268435456;
	// stw r30,56(r11)
	PPC_STORE_U32(ctx.r11.u32 + 56, ctx.r30.u32);
	// ori r29,r3,55
	ctx.r29.u64 = ctx.r3.u64 | 55;
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,38
	ctx.r8.u64 = ctx.r8.u64 | 38;
	// ori r7,r7,39
	ctx.r7.u64 = ctx.r7.u64 | 39;
	// ori r6,r6,13
	ctx.r6.u64 = ctx.r6.u64 | 13;
	// ori r5,r5,33
	ctx.r5.u64 = ctx.r5.u64 | 33;
	// stw r29,64(r11)
	PPC_STORE_U32(ctx.r11.u32 + 64, ctx.r29.u32);
	// ori r4,r4,14
	ctx.r4.u64 = ctx.r4.u64 | 14;
	// std r9,4(r11)
	PPC_STORE_U64(ctx.r11.u32 + 4, ctx.r9.u64);
	// ori r10,r28,128
	ctx.r10.u64 = ctx.r28.u64 | 128;
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// stw r7,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r7.u32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r6,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r6.u32);
	// stw r5,40(r11)
	PPC_STORE_U32(ctx.r11.u32 + 40, ctx.r5.u32);
	// stw r4,48(r11)
	PPC_STORE_U32(ctx.r11.u32 + 48, ctx.r4.u32);
	// stb r10,256(r11)
	PPC_STORE_U8(ctx.r11.u32 + 256, ctx.r10.u8);
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EFDA8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EFDA8) { __imp__sub_826EFDA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EFDA8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x826EFDB0;
	__savegprlr_27(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// li r30,0
	ctx.r30.s64 = 0;
	// cmplwi cr6,r4,4
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 4, ctx.xer);
	// bgt cr6,0x826efdf8
	if (ctx.cr6.gt) goto loc_826EFDF8;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-540
	ctx.r12.s64 = ctx.r12.s64 + -540;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826EFE00;
	case 1:
		goto loc_826EFE00;
	case 2:
		goto loc_826EFE00;
	case 3:
		goto loc_826EFE00;
	case 4:
		goto loc_826EFE00;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EFDF8:
	// li r31,-1
	ctx.r31.s64 = -1;
	// b 0x826efe04
	goto loc_826EFE04;
loc_826EFE00:
	// li r31,2
	ctx.r31.s64 = 2;
loc_826EFE04:
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826EFE14;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826efebc
	if (ctx.cr6.gt) goto loc_826EFEBC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-460
	ctx.r12.s64 = ctx.r12.s64 + -460;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826EFE44;
	case 1:
		goto loc_826EFE64;
	case 2:
		goto loc_826EFE84;
	case 3:
		goto loc_826EFEA4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EFE44:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9798
	ctx.lr = 0x826EFE58;
	sub_826E9798(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_826EFE64:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9390
	ctx.lr = 0x826EFE78;
	sub_826E9390(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_826EFE84:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9428
	ctx.lr = 0x826EFE98;
	sub_826E9428(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_826EFEA4:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826EFEB8;
	sub_826E95D8(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
loc_826EFEBC:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826EFEC8);  /* forward decl */
PPC_WEAK_FUNC(sub_826EFEC8) { __imp__sub_826EFEC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EFEC8) {
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
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826EFEE8;
	sub_829745B0(ctx, base);
	// addi r11,r31,48
	ctx.r11.s64 = ctx.r31.s64 + 48;
	// li r10,5
	ctx.r10.s64 = 5;
	// lis r8,8192
	ctx.r8.s64 = 536870912;
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// lis r6,4096
	ctx.r6.s64 = 268435456;
	// lbz r30,256(r11)
	ctx.r30.u64 = PPC_LOAD_U8(ctx.r11.u32 + 256);
	// lis r5,4096
	ctx.r5.s64 = 268435456;
	// lis r4,8192
	ctx.r4.s64 = 536870912;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,13
	ctx.r8.u64 = ctx.r8.u64 | 13;
	// ori r7,r7,33
	ctx.r7.u64 = ctx.r7.u64 | 33;
	// ori r6,r6,14
	ctx.r6.u64 = ctx.r6.u64 | 14;
	// ori r5,r5,15
	ctx.r5.u64 = ctx.r5.u64 | 15;
	// ori r4,r4,55
	ctx.r4.u64 = ctx.r4.u64 | 55;
	// std r9,4(r11)
	PPC_STORE_U64(ctx.r11.u32 + 4, ctx.r9.u64);
	// ori r10,r30,128
	ctx.r10.u64 = ctx.r30.u64 | 128;
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// stw r7,24(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24, ctx.r7.u32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r6,32(r11)
	PPC_STORE_U32(ctx.r11.u32 + 32, ctx.r6.u32);
	// stw r5,40(r11)
	PPC_STORE_U32(ctx.r11.u32 + 40, ctx.r5.u32);
	// stw r4,48(r11)
	PPC_STORE_U32(ctx.r11.u32 + 48, ctx.r4.u32);
	// stb r10,256(r11)
	PPC_STORE_U8(ctx.r11.u32 + 256, ctx.r10.u8);
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
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

PPC_FUNC_IMPL(__imp__sub_826EFF68);  /* forward decl */
PPC_WEAK_FUNC(sub_826EFF68) { __imp__sub_826EFF68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826EFF68) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826EFF70;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r26,r4
	ctx.r26.u64 = ctx.r4.u64;
	// mr r25,r3
	ctx.r25.u64 = ctx.r3.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// li r30,0
	ctx.r30.s64 = 0;
	// bl 0x826e90e0
	ctx.lr = 0x826EFF94;
	sub_826E90E0(ctx, base);
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826efff4
	if (ctx.cr6.gt) goto loc_826EFFF4;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826effec
	if (ctx.cr6.eq) goto loc_826EFFEC;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,6
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 6, ctx.xer);
	// bgt cr6,0x826efff4
	if (ctx.cr6.gt) goto loc_826EFFF4;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,-48
	ctx.r12.s64 = ctx.r12.s64 + -48;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826EFFEC;
	case 1:
		goto loc_826EFFEC;
	case 2:
		goto loc_826EFFEC;
	case 3:
		goto loc_826EFFEC;
	case 4:
		goto loc_826EFFEC;
	case 5:
		goto loc_826EFFEC;
	case 6:
		goto loc_826EFFEC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826EFFEC:
	// li r31,3
	ctx.r31.s64 = 3;
	// b 0x826efff8
	goto loc_826EFFF8;
loc_826EFFF4:
	// li r31,-1
	ctx.r31.s64 = -1;
loc_826EFFF8:
	// lwz r11,0(r25)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r25.u32 + 0);
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F0010;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826f00b8
	if (ctx.cr6.gt) goto loc_826F00B8;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,48
	ctx.r12.s64 = ctx.r12.s64 + 48;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826F0040;
	case 1:
		goto loc_826F0060;
	case 2:
		goto loc_826F0080;
	case 3:
		goto loc_826F00A0;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F0040:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9798
	ctx.lr = 0x826F0054;
	sub_826E9798(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F0060:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9390
	ctx.lr = 0x826F0074;
	sub_826E9390(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F0080:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9428
	ctx.lr = 0x826F0094;
	sub_826E9428(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F00A0:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826F00B4;
	sub_826E95D8(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
loc_826F00B8:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F00C8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F00C8) { __imp__sub_826F00C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F00C8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826F00D0;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r26,r4
	ctx.r26.u64 = ctx.r4.u64;
	// mr r25,r3
	ctx.r25.u64 = ctx.r3.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// li r30,0
	ctx.r30.s64 = 0;
	// bl 0x826e9670
	ctx.lr = 0x826F00F4;
	sub_826E9670(ctx, base);
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f014c
	if (ctx.cr6.gt) goto loc_826F014C;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f0144
	if (ctx.cr6.eq) goto loc_826F0144;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826f014c
	if (ctx.cr6.gt) goto loc_826F014C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,304
	ctx.r12.s64 = ctx.r12.s64 + 304;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F0144;
	case 1:
		goto loc_826F0144;
	case 2:
		goto loc_826F0144;
	case 3:
		goto loc_826F0144;
	case 4:
		goto loc_826F0144;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F0144:
	// li r31,2
	ctx.r31.s64 = 2;
	// b 0x826f0150
	goto loc_826F0150;
loc_826F014C:
	// li r31,-1
	ctx.r31.s64 = -1;
loc_826F0150:
	// lwz r11,0(r25)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r25.u32 + 0);
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F0168;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826f0210
	if (ctx.cr6.gt) goto loc_826F0210;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,392
	ctx.r12.s64 = ctx.r12.s64 + 392;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826F0198;
	case 1:
		goto loc_826F01B8;
	case 2:
		goto loc_826F01D8;
	case 3:
		goto loc_826F01F8;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F0198:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9798
	ctx.lr = 0x826F01AC;
	sub_826E9798(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F01B8:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9390
	ctx.lr = 0x826F01CC;
	sub_826E9390(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F01D8:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9428
	ctx.lr = 0x826F01EC;
	sub_826E9428(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F01F8:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826F020C;
	sub_826E95D8(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
loc_826F0210:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F0220);  /* forward decl */
PPC_WEAK_FUNC(sub_826F0220) { __imp__sub_826F0220(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F0220) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x826F0228;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826F0238;
	sub_829745B0(ctx, base);
	// addi r11,r31,48
	ctx.r11.s64 = ctx.r31.s64 + 48;
	// li r10,6
	ctx.r10.s64 = 6;
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// lis r8,8192
	ctx.r8.s64 = 536870912;
	// lis r7,8192
	ctx.r7.s64 = 536870912;
	// lbz r29,256(r11)
	ctx.r29.u64 = PPC_LOAD_U8(ctx.r11.u32 + 256);
	// lis r6,4096
	ctx.r6.s64 = 268435456;
	// lis r5,4096
	ctx.r5.s64 = 268435456;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// lis r4,4096
	ctx.r4.s64 = 268435456;
	// ori r30,r3,55
	ctx.r30.u64 = ctx.r3.u64 | 55;
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,42
	ctx.r8.u64 = ctx.r8.u64 | 42;
	// ori r7,r7,13
	ctx.r7.u64 = ctx.r7.u64 | 13;
	// ori r6,r6,14
	ctx.r6.u64 = ctx.r6.u64 | 14;
	// ori r5,r5,18
	ctx.r5.u64 = ctx.r5.u64 | 18;
	// stw r30,60(r11)
	PPC_STORE_U32(ctx.r11.u32 + 60, ctx.r30.u32);
	// ori r4,r4,15
	ctx.r4.u64 = ctx.r4.u64 | 15;
	// std r9,4(r11)
	PPC_STORE_U64(ctx.r11.u32 + 4, ctx.r9.u64);
	// ori r10,r29,128
	ctx.r10.u64 = ctx.r29.u64 | 128;
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// stw r7,24(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24, ctx.r7.u32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r6,36(r11)
	PPC_STORE_U32(ctx.r11.u32 + 36, ctx.r6.u32);
	// stw r5,44(r11)
	PPC_STORE_U32(ctx.r11.u32 + 44, ctx.r5.u32);
	// stw r4,52(r11)
	PPC_STORE_U32(ctx.r11.u32 + 52, ctx.r4.u32);
	// stb r10,256(r11)
	PPC_STORE_U8(ctx.r11.u32 + 256, ctx.r10.u8);
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F02B0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F02B0) { __imp__sub_826F02B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F02B0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x826F02B8;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826F02C8;
	sub_829745B0(ctx, base);
	// addi r11,r31,48
	ctx.r11.s64 = ctx.r31.s64 + 48;
	// li r10,6
	ctx.r10.s64 = 6;
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// lis r8,8192
	ctx.r8.s64 = 536870912;
	// lis r7,8192
	ctx.r7.s64 = 536870912;
	// lbz r29,256(r11)
	ctx.r29.u64 = PPC_LOAD_U8(ctx.r11.u32 + 256);
	// lis r6,4096
	ctx.r6.s64 = 268435456;
	// lis r5,4096
	ctx.r5.s64 = 268435456;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// lis r4,4096
	ctx.r4.s64 = 268435456;
	// ori r30,r3,58
	ctx.r30.u64 = ctx.r3.u64 | 58;
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,42
	ctx.r8.u64 = ctx.r8.u64 | 42;
	// ori r7,r7,13
	ctx.r7.u64 = ctx.r7.u64 | 13;
	// ori r6,r6,14
	ctx.r6.u64 = ctx.r6.u64 | 14;
	// ori r5,r5,18
	ctx.r5.u64 = ctx.r5.u64 | 18;
	// stw r30,60(r11)
	PPC_STORE_U32(ctx.r11.u32 + 60, ctx.r30.u32);
	// ori r4,r4,15
	ctx.r4.u64 = ctx.r4.u64 | 15;
	// std r9,4(r11)
	PPC_STORE_U64(ctx.r11.u32 + 4, ctx.r9.u64);
	// ori r10,r29,128
	ctx.r10.u64 = ctx.r29.u64 | 128;
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// stw r7,24(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24, ctx.r7.u32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r6,36(r11)
	PPC_STORE_U32(ctx.r11.u32 + 36, ctx.r6.u32);
	// stw r5,44(r11)
	PPC_STORE_U32(ctx.r11.u32 + 44, ctx.r5.u32);
	// stw r4,52(r11)
	PPC_STORE_U32(ctx.r11.u32 + 52, ctx.r4.u32);
	// stb r10,256(r11)
	PPC_STORE_U8(ctx.r11.u32 + 256, ctx.r10.u8);
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F0340);  /* forward decl */
PPC_WEAK_FUNC(sub_826F0340) { __imp__sub_826F0340(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F0340) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826F0348;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r26,r4
	ctx.r26.u64 = ctx.r4.u64;
	// mr r25,r3
	ctx.r25.u64 = ctx.r3.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// li r30,0
	ctx.r30.s64 = 0;
	// bl 0x826e9260
	ctx.lr = 0x826F036C;
	sub_826E9260(ctx, base);
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f03c8
	if (ctx.cr6.gt) goto loc_826F03C8;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f03c0
	if (ctx.cr6.eq) goto loc_826F03C0;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x826f03c8
	if (ctx.cr6.gt) goto loc_826F03C8;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,936
	ctx.r12.s64 = ctx.r12.s64 + 936;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F03C0;
	case 1:
		goto loc_826F03C0;
	case 2:
		goto loc_826F03C0;
	case 3:
		goto loc_826F03C0;
	case 4:
		goto loc_826F03C8;
	case 5:
		goto loc_826F03C0;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F03C0:
	// li r31,2
	ctx.r31.s64 = 2;
	// b 0x826f03cc
	goto loc_826F03CC;
loc_826F03C8:
	// li r31,-1
	ctx.r31.s64 = -1;
loc_826F03CC:
	// lwz r11,0(r25)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r25.u32 + 0);
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F03E4;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826f048c
	if (ctx.cr6.gt) goto loc_826F048C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,1028
	ctx.r12.s64 = ctx.r12.s64 + 1028;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826F0414;
	case 1:
		goto loc_826F0434;
	case 2:
		goto loc_826F0454;
	case 3:
		goto loc_826F0474;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F0414:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9798
	ctx.lr = 0x826F0428;
	sub_826E9798(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F0434:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9390
	ctx.lr = 0x826F0448;
	sub_826E9390(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F0454:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9428
	ctx.lr = 0x826F0468;
	sub_826E9428(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F0474:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826F0488;
	sub_826E95D8(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
loc_826F048C:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F0498);  /* forward decl */
PPC_WEAK_FUNC(sub_826F0498) { __imp__sub_826F0498(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F0498) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x826F04A0;
	__savegprlr_27(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826F04B0;
	sub_829745B0(ctx, base);
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// addi r11,r31,48
	ctx.r11.s64 = ctx.r31.s64 + 48;
	// ori r30,r3,18
	ctx.r30.u64 = ctx.r3.u64 | 18;
	// li r10,8
	ctx.r10.s64 = 8;
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// lis r8,4096
	ctx.r8.s64 = 268435456;
	// ori r29,r3,15
	ctx.r29.u64 = ctx.r3.u64 | 15;
	// lbz r27,256(r11)
	ctx.r27.u64 = PPC_LOAD_U8(ctx.r11.u32 + 256);
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// stw r30,56(r11)
	PPC_STORE_U32(ctx.r11.u32 + 56, ctx.r30.u32);
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// lis r6,4096
	ctx.r6.s64 = 268435456;
	// lis r5,8192
	ctx.r5.s64 = 536870912;
	// lis r4,4096
	ctx.r4.s64 = 268435456;
	// stw r29,64(r11)
	PPC_STORE_U32(ctx.r11.u32 + 64, ctx.r29.u32);
	// ori r28,r3,55
	ctx.r28.u64 = ctx.r3.u64 | 55;
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,38
	ctx.r8.u64 = ctx.r8.u64 | 38;
	// ori r7,r7,41
	ctx.r7.u64 = ctx.r7.u64 | 41;
	// ori r6,r6,39
	ctx.r6.u64 = ctx.r6.u64 | 39;
	// ori r5,r5,13
	ctx.r5.u64 = ctx.r5.u64 | 13;
	// stw r28,72(r11)
	PPC_STORE_U32(ctx.r11.u32 + 72, ctx.r28.u32);
	// ori r4,r4,14
	ctx.r4.u64 = ctx.r4.u64 | 14;
	// std r9,4(r11)
	PPC_STORE_U64(ctx.r11.u32 + 4, ctx.r9.u64);
	// ori r10,r27,128
	ctx.r10.u64 = ctx.r27.u64 | 128;
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// stw r7,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r7.u32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r6,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r6.u32);
	// stw r5,36(r11)
	PPC_STORE_U32(ctx.r11.u32 + 36, ctx.r5.u32);
	// stw r4,48(r11)
	PPC_STORE_U32(ctx.r11.u32 + 48, ctx.r4.u32);
	// stb r10,256(r11)
	PPC_STORE_U8(ctx.r11.u32 + 256, ctx.r10.u8);
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F0540);  /* forward decl */
PPC_WEAK_FUNC(sub_826F0540) { __imp__sub_826F0540(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F0540) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826F0548;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r26,r4
	ctx.r26.u64 = ctx.r4.u64;
	// mr r25,r3
	ctx.r25.u64 = ctx.r3.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// li r30,0
	ctx.r30.s64 = 0;
	// bl 0x826e9830
	ctx.lr = 0x826F056C;
	sub_826E9830(ctx, base);
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f05c8
	if (ctx.cr6.gt) goto loc_826F05C8;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f05c0
	if (ctx.cr6.eq) goto loc_826F05C0;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x826f05c8
	if (ctx.cr6.gt) goto loc_826F05C8;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,1448
	ctx.r12.s64 = ctx.r12.s64 + 1448;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F05C0;
	case 1:
		goto loc_826F05C0;
	case 2:
		goto loc_826F05C0;
	case 3:
		goto loc_826F05C0;
	case 4:
		goto loc_826F05C8;
	case 5:
		goto loc_826F05C0;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F05C0:
	// li r31,2
	ctx.r31.s64 = 2;
	// b 0x826f05cc
	goto loc_826F05CC;
loc_826F05C8:
	// li r31,-1
	ctx.r31.s64 = -1;
loc_826F05CC:
	// lwz r11,0(r25)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r25.u32 + 0);
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F05E4;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826f068c
	if (ctx.cr6.gt) goto loc_826F068C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,1540
	ctx.r12.s64 = ctx.r12.s64 + 1540;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826F0614;
	case 1:
		goto loc_826F0634;
	case 2:
		goto loc_826F0654;
	case 3:
		goto loc_826F0674;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F0614:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9798
	ctx.lr = 0x826F0628;
	sub_826E9798(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F0634:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9390
	ctx.lr = 0x826F0648;
	sub_826E9390(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F0654:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9428
	ctx.lr = 0x826F0668;
	sub_826E9428(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F0674:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826F0688;
	sub_826E95D8(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
loc_826F068C:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F0698);  /* forward decl */
PPC_WEAK_FUNC(sub_826F0698) { __imp__sub_826F0698(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F0698) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x826F06A0;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826F06B0;
	sub_829745B0(ctx, base);
	// addi r11,r31,48
	ctx.r11.s64 = ctx.r31.s64 + 48;
	// li r10,6
	ctx.r10.s64 = 6;
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// lis r8,8192
	ctx.r8.s64 = 536870912;
	// lis r7,8192
	ctx.r7.s64 = 536870912;
	// lbz r29,256(r11)
	ctx.r29.u64 = PPC_LOAD_U8(ctx.r11.u32 + 256);
	// lis r6,4096
	ctx.r6.s64 = 268435456;
	// lis r5,4096
	ctx.r5.s64 = 268435456;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// lis r4,4096
	ctx.r4.s64 = 268435456;
	// ori r30,r3,58
	ctx.r30.u64 = ctx.r3.u64 | 58;
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,42
	ctx.r8.u64 = ctx.r8.u64 | 42;
	// ori r7,r7,13
	ctx.r7.u64 = ctx.r7.u64 | 13;
	// ori r6,r6,14
	ctx.r6.u64 = ctx.r6.u64 | 14;
	// ori r5,r5,15
	ctx.r5.u64 = ctx.r5.u64 | 15;
	// stw r30,60(r11)
	PPC_STORE_U32(ctx.r11.u32 + 60, ctx.r30.u32);
	// ori r4,r4,18
	ctx.r4.u64 = ctx.r4.u64 | 18;
	// std r9,4(r11)
	PPC_STORE_U64(ctx.r11.u32 + 4, ctx.r9.u64);
	// ori r10,r29,128
	ctx.r10.u64 = ctx.r29.u64 | 128;
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// stw r7,24(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24, ctx.r7.u32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r6,36(r11)
	PPC_STORE_U32(ctx.r11.u32 + 36, ctx.r6.u32);
	// stw r5,44(r11)
	PPC_STORE_U32(ctx.r11.u32 + 44, ctx.r5.u32);
	// stw r4,52(r11)
	PPC_STORE_U32(ctx.r11.u32 + 52, ctx.r4.u32);
	// stb r10,256(r11)
	PPC_STORE_U8(ctx.r11.u32 + 256, ctx.r10.u8);
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F0728);  /* forward decl */
PPC_WEAK_FUNC(sub_826F0728) { __imp__sub_826F0728(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F0728) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb10
	ctx.lr = 0x826F0730;
	__savegprlr_26(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826F0740;
	sub_829745B0(ctx, base);
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// addi r11,r31,48
	ctx.r11.s64 = ctx.r31.s64 + 48;
	// ori r30,r3,14
	ctx.r30.u64 = ctx.r3.u64 | 14;
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// li r10,9
	ctx.r10.s64 = 9;
	// ori r29,r3,18
	ctx.r29.u64 = ctx.r3.u64 | 18;
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// lbz r26,256(r11)
	ctx.r26.u64 = PPC_LOAD_U8(ctx.r11.u32 + 256);
	// lis r8,4096
	ctx.r8.s64 = 268435456;
	// stw r30,60(r11)
	PPC_STORE_U32(ctx.r11.u32 + 60, ctx.r30.u32);
	// ori r28,r3,15
	ctx.r28.u64 = ctx.r3.u64 | 15;
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// stw r29,68(r11)
	PPC_STORE_U32(ctx.r11.u32 + 68, ctx.r29.u32);
	// lis r6,8192
	ctx.r6.s64 = 536870912;
	// lis r5,8192
	ctx.r5.s64 = 536870912;
	// lis r4,4096
	ctx.r4.s64 = 268435456;
	// stw r28,76(r11)
	PPC_STORE_U32(ctx.r11.u32 + 76, ctx.r28.u32);
	// ori r27,r3,55
	ctx.r27.u64 = ctx.r3.u64 | 55;
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,38
	ctx.r8.u64 = ctx.r8.u64 | 38;
	// ori r7,r7,39
	ctx.r7.u64 = ctx.r7.u64 | 39;
	// ori r6,r6,13
	ctx.r6.u64 = ctx.r6.u64 | 13;
	// ori r5,r5,29
	ctx.r5.u64 = ctx.r5.u64 | 29;
	// stw r27,84(r11)
	PPC_STORE_U32(ctx.r11.u32 + 84, ctx.r27.u32);
	// ori r4,r4,34
	ctx.r4.u64 = ctx.r4.u64 | 34;
	// std r9,4(r11)
	PPC_STORE_U64(ctx.r11.u32 + 4, ctx.r9.u64);
	// ori r10,r26,128
	ctx.r10.u64 = ctx.r26.u64 | 128;
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// stw r7,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r7.u32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r6,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r6.u32);
	// stw r5,40(r11)
	PPC_STORE_U32(ctx.r11.u32 + 40, ctx.r5.u32);
	// stw r4,52(r11)
	PPC_STORE_U32(ctx.r11.u32 + 52, ctx.r4.u32);
	// stb r10,256(r11)
	PPC_STORE_U8(ctx.r11.u32 + 256, ctx.r10.u8);
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F07E0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F07E0) { __imp__sub_826F07E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F07E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x826F07E8;
	__savegprlr_28(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826F07F8;
	sub_829745B0(ctx, base);
	// addi r11,r31,48
	ctx.r11.s64 = ctx.r31.s64 + 48;
	// li r10,7
	ctx.r10.s64 = 7;
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// lis r8,8192
	ctx.r8.s64 = 536870912;
	// ori r30,r3,15
	ctx.r30.u64 = ctx.r3.u64 | 15;
	// lbz r28,256(r11)
	ctx.r28.u64 = PPC_LOAD_U8(ctx.r11.u32 + 256);
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// lis r7,8192
	ctx.r7.s64 = 536870912;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// lis r6,4096
	ctx.r6.s64 = 268435456;
	// lis r5,4096
	ctx.r5.s64 = 268435456;
	// lis r4,4096
	ctx.r4.s64 = 268435456;
	// stw r30,60(r11)
	PPC_STORE_U32(ctx.r11.u32 + 60, ctx.r30.u32);
	// ori r29,r3,55
	ctx.r29.u64 = ctx.r3.u64 | 55;
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,13
	ctx.r8.u64 = ctx.r8.u64 | 13;
	// ori r7,r7,29
	ctx.r7.u64 = ctx.r7.u64 | 29;
	// ori r6,r6,34
	ctx.r6.u64 = ctx.r6.u64 | 34;
	// ori r5,r5,14
	ctx.r5.u64 = ctx.r5.u64 | 14;
	// stw r29,68(r11)
	PPC_STORE_U32(ctx.r11.u32 + 68, ctx.r29.u32);
	// ori r4,r4,18
	ctx.r4.u64 = ctx.r4.u64 | 18;
	// std r9,4(r11)
	PPC_STORE_U64(ctx.r11.u32 + 4, ctx.r9.u64);
	// ori r10,r28,128
	ctx.r10.u64 = ctx.r28.u64 | 128;
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// stw r7,24(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24, ctx.r7.u32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r6,36(r11)
	PPC_STORE_U32(ctx.r11.u32 + 36, ctx.r6.u32);
	// stw r5,44(r11)
	PPC_STORE_U32(ctx.r11.u32 + 44, ctx.r5.u32);
	// stw r4,52(r11)
	PPC_STORE_U32(ctx.r11.u32 + 52, ctx.r4.u32);
	// stb r10,256(r11)
	PPC_STORE_U8(ctx.r11.u32 + 256, ctx.r10.u8);
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F0880);  /* forward decl */
PPC_WEAK_FUNC(sub_826F0880) { __imp__sub_826F0880(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F0880) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826F0888;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r26,r4
	ctx.r26.u64 = ctx.r4.u64;
	// mr r25,r3
	ctx.r25.u64 = ctx.r3.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// li r30,0
	ctx.r30.s64 = 0;
	// bl 0x826e9be0
	ctx.lr = 0x826F08AC;
	sub_826E9BE0(ctx, base);
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f0910
	if (ctx.cr6.gt) goto loc_826F0910;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f0908
	if (ctx.cr6.eq) goto loc_826F0908;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,7
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 7, ctx.xer);
	// bgt cr6,0x826f0910
	if (ctx.cr6.gt) goto loc_826F0910;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,2280
	ctx.r12.s64 = ctx.r12.s64 + 2280;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F0908;
	case 1:
		goto loc_826F0908;
	case 2:
		goto loc_826F0908;
	case 3:
		goto loc_826F0908;
	case 4:
		goto loc_826F0910;
	case 5:
		goto loc_826F0908;
	case 6:
		goto loc_826F0908;
	case 7:
		goto loc_826F0908;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F0908:
	// li r31,3
	ctx.r31.s64 = 3;
	// b 0x826f0914
	goto loc_826F0914;
loc_826F0910:
	// li r31,-1
	ctx.r31.s64 = -1;
loc_826F0914:
	// lwz r11,0(r25)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r25.u32 + 0);
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F092C;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826f09d4
	if (ctx.cr6.gt) goto loc_826F09D4;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,2380
	ctx.r12.s64 = ctx.r12.s64 + 2380;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826F095C;
	case 1:
		goto loc_826F097C;
	case 2:
		goto loc_826F099C;
	case 3:
		goto loc_826F09BC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F095C:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9798
	ctx.lr = 0x826F0970;
	sub_826E9798(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F097C:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9390
	ctx.lr = 0x826F0990;
	sub_826E9390(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F099C:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9428
	ctx.lr = 0x826F09B0;
	sub_826E9428(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F09BC:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826F09D0;
	sub_826E95D8(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
loc_826F09D4:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F09E0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F09E0) { __imp__sub_826F09E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F09E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826F09E8;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r26,r4
	ctx.r26.u64 = ctx.r4.u64;
	// mr r25,r3
	ctx.r25.u64 = ctx.r3.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// li r30,0
	ctx.r30.s64 = 0;
	// bl 0x826e9e08
	ctx.lr = 0x826F0A0C;
	sub_826E9E08(ctx, base);
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f0a68
	if (ctx.cr6.gt) goto loc_826F0A68;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f0a60
	if (ctx.cr6.eq) goto loc_826F0A60;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x826f0a68
	if (ctx.cr6.gt) goto loc_826F0A68;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,2632
	ctx.r12.s64 = ctx.r12.s64 + 2632;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F0A60;
	case 1:
		goto loc_826F0A60;
	case 2:
		goto loc_826F0A60;
	case 3:
		goto loc_826F0A60;
	case 4:
		goto loc_826F0A68;
	case 5:
		goto loc_826F0A60;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F0A60:
	// li r31,2
	ctx.r31.s64 = 2;
	// b 0x826f0a6c
	goto loc_826F0A6C;
loc_826F0A68:
	// li r31,-1
	ctx.r31.s64 = -1;
loc_826F0A6C:
	// lwz r11,0(r25)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r25.u32 + 0);
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F0A84;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826f0b2c
	if (ctx.cr6.gt) goto loc_826F0B2C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,2724
	ctx.r12.s64 = ctx.r12.s64 + 2724;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826F0AB4;
	case 1:
		goto loc_826F0AD4;
	case 2:
		goto loc_826F0AF4;
	case 3:
		goto loc_826F0B14;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F0AB4:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9798
	ctx.lr = 0x826F0AC8;
	sub_826E9798(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F0AD4:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9390
	ctx.lr = 0x826F0AE8;
	sub_826E9390(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F0AF4:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9428
	ctx.lr = 0x826F0B08;
	sub_826E9428(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F0B14:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826F0B28;
	sub_826E95D8(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
loc_826F0B2C:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F0B38);  /* forward decl */
PPC_WEAK_FUNC(sub_826F0B38) { __imp__sub_826F0B38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F0B38) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x826F0B40;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826F0B50;
	sub_829745B0(ctx, base);
	// addi r11,r31,48
	ctx.r11.s64 = ctx.r31.s64 + 48;
	// li r10,6
	ctx.r10.s64 = 6;
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// lis r8,8192
	ctx.r8.s64 = 536870912;
	// lis r7,8192
	ctx.r7.s64 = 536870912;
	// lbz r29,256(r11)
	ctx.r29.u64 = PPC_LOAD_U8(ctx.r11.u32 + 256);
	// lis r6,4096
	ctx.r6.s64 = 268435456;
	// lis r5,4096
	ctx.r5.s64 = 268435456;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// lis r4,4096
	ctx.r4.s64 = 268435456;
	// ori r30,r3,55
	ctx.r30.u64 = ctx.r3.u64 | 55;
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,13
	ctx.r8.u64 = ctx.r8.u64 | 13;
	// ori r7,r7,29
	ctx.r7.u64 = ctx.r7.u64 | 29;
	// ori r6,r6,14
	ctx.r6.u64 = ctx.r6.u64 | 14;
	// ori r5,r5,18
	ctx.r5.u64 = ctx.r5.u64 | 18;
	// stw r30,60(r11)
	PPC_STORE_U32(ctx.r11.u32 + 60, ctx.r30.u32);
	// ori r4,r4,15
	ctx.r4.u64 = ctx.r4.u64 | 15;
	// std r9,4(r11)
	PPC_STORE_U64(ctx.r11.u32 + 4, ctx.r9.u64);
	// ori r10,r29,128
	ctx.r10.u64 = ctx.r29.u64 | 128;
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// stw r7,24(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24, ctx.r7.u32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r6,36(r11)
	PPC_STORE_U32(ctx.r11.u32 + 36, ctx.r6.u32);
	// stw r5,44(r11)
	PPC_STORE_U32(ctx.r11.u32 + 44, ctx.r5.u32);
	// stw r4,52(r11)
	PPC_STORE_U32(ctx.r11.u32 + 52, ctx.r4.u32);
	// stb r10,256(r11)
	PPC_STORE_U8(ctx.r11.u32 + 256, ctx.r10.u8);
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F0BC8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F0BC8) { __imp__sub_826F0BC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F0BC8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826F0BD0;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r26,r4
	ctx.r26.u64 = ctx.r4.u64;
	// mr r25,r3
	ctx.r25.u64 = ctx.r3.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// li r30,0
	ctx.r30.s64 = 0;
	// bl 0x826e9f38
	ctx.lr = 0x826F0BF4;
	sub_826E9F38(ctx, base);
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f0c58
	if (ctx.cr6.gt) goto loc_826F0C58;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f0c50
	if (ctx.cr6.eq) goto loc_826F0C50;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,7
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 7, ctx.xer);
	// bgt cr6,0x826f0c58
	if (ctx.cr6.gt) goto loc_826F0C58;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,3120
	ctx.r12.s64 = ctx.r12.s64 + 3120;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F0C50;
	case 1:
		goto loc_826F0C50;
	case 2:
		goto loc_826F0C50;
	case 3:
		goto loc_826F0C50;
	case 4:
		goto loc_826F0C50;
	case 5:
		goto loc_826F0C50;
	case 6:
		goto loc_826F0C50;
	case 7:
		goto loc_826F0C50;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F0C50:
	// li r31,3
	ctx.r31.s64 = 3;
	// b 0x826f0c5c
	goto loc_826F0C5C;
loc_826F0C58:
	// li r31,-1
	ctx.r31.s64 = -1;
loc_826F0C5C:
	// lwz r11,0(r25)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r25.u32 + 0);
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F0C74;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826f0d1c
	if (ctx.cr6.gt) goto loc_826F0D1C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,3220
	ctx.r12.s64 = ctx.r12.s64 + 3220;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826F0CA4;
	case 1:
		goto loc_826F0CC4;
	case 2:
		goto loc_826F0CE4;
	case 3:
		goto loc_826F0D04;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F0CA4:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9798
	ctx.lr = 0x826F0CB8;
	sub_826E9798(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F0CC4:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9390
	ctx.lr = 0x826F0CD8;
	sub_826E9390(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F0CE4:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9428
	ctx.lr = 0x826F0CF8;
	sub_826E9428(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F0D04:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826F0D18;
	sub_826E95D8(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
loc_826F0D1C:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F0D28);  /* forward decl */
PPC_WEAK_FUNC(sub_826F0D28) { __imp__sub_826F0D28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F0D28) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb10
	ctx.lr = 0x826F0D30;
	__savegprlr_26(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826F0D40;
	sub_829745B0(ctx, base);
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// addi r11,r31,48
	ctx.r11.s64 = ctx.r31.s64 + 48;
	// ori r30,r3,14
	ctx.r30.u64 = ctx.r3.u64 | 14;
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// li r10,9
	ctx.r10.s64 = 9;
	// ori r29,r3,18
	ctx.r29.u64 = ctx.r3.u64 | 18;
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// lbz r26,256(r11)
	ctx.r26.u64 = PPC_LOAD_U8(ctx.r11.u32 + 256);
	// lis r8,4096
	ctx.r8.s64 = 268435456;
	// stw r30,60(r11)
	PPC_STORE_U32(ctx.r11.u32 + 60, ctx.r30.u32);
	// ori r28,r3,15
	ctx.r28.u64 = ctx.r3.u64 | 15;
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// stw r29,68(r11)
	PPC_STORE_U32(ctx.r11.u32 + 68, ctx.r29.u32);
	// lis r6,8192
	ctx.r6.s64 = 536870912;
	// lis r5,8192
	ctx.r5.s64 = 536870912;
	// lis r4,4096
	ctx.r4.s64 = 268435456;
	// stw r28,76(r11)
	PPC_STORE_U32(ctx.r11.u32 + 76, ctx.r28.u32);
	// ori r27,r3,55
	ctx.r27.u64 = ctx.r3.u64 | 55;
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,38
	ctx.r8.u64 = ctx.r8.u64 | 38;
	// ori r7,r7,39
	ctx.r7.u64 = ctx.r7.u64 | 39;
	// ori r6,r6,13
	ctx.r6.u64 = ctx.r6.u64 | 13;
	// ori r5,r5,29
	ctx.r5.u64 = ctx.r5.u64 | 29;
	// stw r27,84(r11)
	PPC_STORE_U32(ctx.r11.u32 + 84, ctx.r27.u32);
	// ori r4,r4,17
	ctx.r4.u64 = ctx.r4.u64 | 17;
	// std r9,4(r11)
	PPC_STORE_U64(ctx.r11.u32 + 4, ctx.r9.u64);
	// ori r10,r26,128
	ctx.r10.u64 = ctx.r26.u64 | 128;
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// stw r7,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r7.u32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r6,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r6.u32);
	// stw r5,40(r11)
	PPC_STORE_U32(ctx.r11.u32 + 40, ctx.r5.u32);
	// stw r4,52(r11)
	PPC_STORE_U32(ctx.r11.u32 + 52, ctx.r4.u32);
	// stb r10,256(r11)
	PPC_STORE_U8(ctx.r11.u32 + 256, ctx.r10.u8);
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F0DE0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F0DE0) { __imp__sub_826F0DE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F0DE0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826F0DE8;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r26,r4
	ctx.r26.u64 = ctx.r4.u64;
	// mr r25,r3
	ctx.r25.u64 = ctx.r3.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// li r30,0
	ctx.r30.s64 = 0;
	// bl 0x826e9ae0
	ctx.lr = 0x826F0E0C;
	sub_826E9AE0(ctx, base);
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f0e68
	if (ctx.cr6.gt) goto loc_826F0E68;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f0e60
	if (ctx.cr6.eq) goto loc_826F0E60;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x826f0e68
	if (ctx.cr6.gt) goto loc_826F0E68;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,3656
	ctx.r12.s64 = ctx.r12.s64 + 3656;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F0E60;
	case 1:
		goto loc_826F0E60;
	case 2:
		goto loc_826F0E60;
	case 3:
		goto loc_826F0E60;
	case 4:
		goto loc_826F0E60;
	case 5:
		goto loc_826F0E60;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F0E60:
	// li r31,2
	ctx.r31.s64 = 2;
	// b 0x826f0e6c
	goto loc_826F0E6C;
loc_826F0E68:
	// li r31,-1
	ctx.r31.s64 = -1;
loc_826F0E6C:
	// lwz r11,0(r25)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r25.u32 + 0);
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F0E84;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826f0f2c
	if (ctx.cr6.gt) goto loc_826F0F2C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,3748
	ctx.r12.s64 = ctx.r12.s64 + 3748;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826F0EB4;
	case 1:
		goto loc_826F0ED4;
	case 2:
		goto loc_826F0EF4;
	case 3:
		goto loc_826F0F14;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F0EB4:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9798
	ctx.lr = 0x826F0EC8;
	sub_826E9798(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F0ED4:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9390
	ctx.lr = 0x826F0EE8;
	sub_826E9390(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F0EF4:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9428
	ctx.lr = 0x826F0F08;
	sub_826E9428(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F0F14:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826F0F28;
	sub_826E95D8(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
loc_826F0F2C:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F0F38);  /* forward decl */
PPC_WEAK_FUNC(sub_826F0F38) { __imp__sub_826F0F38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F0F38) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x826F0F40;
	__savegprlr_28(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826F0F50;
	sub_829745B0(ctx, base);
	// addi r11,r31,48
	ctx.r11.s64 = ctx.r31.s64 + 48;
	// li r10,7
	ctx.r10.s64 = 7;
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// lis r8,8192
	ctx.r8.s64 = 536870912;
	// ori r30,r3,15
	ctx.r30.u64 = ctx.r3.u64 | 15;
	// lbz r28,256(r11)
	ctx.r28.u64 = PPC_LOAD_U8(ctx.r11.u32 + 256);
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// lis r7,8192
	ctx.r7.s64 = 536870912;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// lis r6,4096
	ctx.r6.s64 = 268435456;
	// lis r5,4096
	ctx.r5.s64 = 268435456;
	// lis r4,4096
	ctx.r4.s64 = 268435456;
	// stw r30,60(r11)
	PPC_STORE_U32(ctx.r11.u32 + 60, ctx.r30.u32);
	// ori r29,r3,55
	ctx.r29.u64 = ctx.r3.u64 | 55;
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,13
	ctx.r8.u64 = ctx.r8.u64 | 13;
	// ori r7,r7,29
	ctx.r7.u64 = ctx.r7.u64 | 29;
	// ori r6,r6,17
	ctx.r6.u64 = ctx.r6.u64 | 17;
	// ori r5,r5,14
	ctx.r5.u64 = ctx.r5.u64 | 14;
	// stw r29,68(r11)
	PPC_STORE_U32(ctx.r11.u32 + 68, ctx.r29.u32);
	// ori r4,r4,18
	ctx.r4.u64 = ctx.r4.u64 | 18;
	// std r9,4(r11)
	PPC_STORE_U64(ctx.r11.u32 + 4, ctx.r9.u64);
	// ori r10,r28,128
	ctx.r10.u64 = ctx.r28.u64 | 128;
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// stw r7,24(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24, ctx.r7.u32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r6,36(r11)
	PPC_STORE_U32(ctx.r11.u32 + 36, ctx.r6.u32);
	// stw r5,44(r11)
	PPC_STORE_U32(ctx.r11.u32 + 44, ctx.r5.u32);
	// stw r4,52(r11)
	PPC_STORE_U32(ctx.r11.u32 + 52, ctx.r4.u32);
	// stb r10,256(r11)
	PPC_STORE_U8(ctx.r11.u32 + 256, ctx.r10.u8);
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F0FD8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F0FD8) { __imp__sub_826F0FD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F0FD8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826F0FE0;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r26,r4
	ctx.r26.u64 = ctx.r4.u64;
	// mr r25,r3
	ctx.r25.u64 = ctx.r3.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// li r30,0
	ctx.r30.s64 = 0;
	// bl 0x826e9968
	ctx.lr = 0x826F1004;
	sub_826E9968(ctx, base);
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f1068
	if (ctx.cr6.gt) goto loc_826F1068;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f1060
	if (ctx.cr6.eq) goto loc_826F1060;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,7
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 7, ctx.xer);
	// bgt cr6,0x826f1068
	if (ctx.cr6.gt) goto loc_826F1068;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,4160
	ctx.r12.s64 = ctx.r12.s64 + 4160;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F1060;
	case 1:
		goto loc_826F1060;
	case 2:
		goto loc_826F1060;
	case 3:
		goto loc_826F1060;
	case 4:
		goto loc_826F1060;
	case 5:
		goto loc_826F1060;
	case 6:
		goto loc_826F1060;
	case 7:
		goto loc_826F1060;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F1060:
	// li r31,3
	ctx.r31.s64 = 3;
	// b 0x826f106c
	goto loc_826F106C;
loc_826F1068:
	// li r31,-1
	ctx.r31.s64 = -1;
loc_826F106C:
	// lwz r11,0(r25)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r25.u32 + 0);
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F1084;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826f112c
	if (ctx.cr6.gt) goto loc_826F112C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,4260
	ctx.r12.s64 = ctx.r12.s64 + 4260;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826F10B4;
	case 1:
		goto loc_826F10D4;
	case 2:
		goto loc_826F10F4;
	case 3:
		goto loc_826F1114;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F10B4:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9798
	ctx.lr = 0x826F10C8;
	sub_826E9798(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F10D4:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9390
	ctx.lr = 0x826F10E8;
	sub_826E9390(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F10F4:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9428
	ctx.lr = 0x826F1108;
	sub_826E9428(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F1114:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826F1128;
	sub_826E95D8(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
loc_826F112C:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F1138);  /* forward decl */
PPC_WEAK_FUNC(sub_826F1138) { __imp__sub_826F1138(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F1138) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb10
	ctx.lr = 0x826F1140;
	__savegprlr_26(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826F1150;
	sub_829745B0(ctx, base);
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// addi r11,r31,48
	ctx.r11.s64 = ctx.r31.s64 + 48;
	// ori r30,r3,14
	ctx.r30.u64 = ctx.r3.u64 | 14;
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// li r10,9
	ctx.r10.s64 = 9;
	// ori r29,r3,18
	ctx.r29.u64 = ctx.r3.u64 | 18;
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// lbz r26,256(r11)
	ctx.r26.u64 = PPC_LOAD_U8(ctx.r11.u32 + 256);
	// lis r8,4096
	ctx.r8.s64 = 268435456;
	// stw r30,60(r11)
	PPC_STORE_U32(ctx.r11.u32 + 60, ctx.r30.u32);
	// ori r28,r3,15
	ctx.r28.u64 = ctx.r3.u64 | 15;
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// stw r29,68(r11)
	PPC_STORE_U32(ctx.r11.u32 + 68, ctx.r29.u32);
	// lis r6,8192
	ctx.r6.s64 = 536870912;
	// lis r5,8192
	ctx.r5.s64 = 536870912;
	// lis r4,4096
	ctx.r4.s64 = 268435456;
	// stw r28,76(r11)
	PPC_STORE_U32(ctx.r11.u32 + 76, ctx.r28.u32);
	// ori r27,r3,55
	ctx.r27.u64 = ctx.r3.u64 | 55;
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,38
	ctx.r8.u64 = ctx.r8.u64 | 38;
	// ori r7,r7,39
	ctx.r7.u64 = ctx.r7.u64 | 39;
	// ori r6,r6,13
	ctx.r6.u64 = ctx.r6.u64 | 13;
	// ori r5,r5,29
	ctx.r5.u64 = ctx.r5.u64 | 29;
	// stw r27,84(r11)
	PPC_STORE_U32(ctx.r11.u32 + 84, ctx.r27.u32);
	// ori r4,r4,33
	ctx.r4.u64 = ctx.r4.u64 | 33;
	// std r9,4(r11)
	PPC_STORE_U64(ctx.r11.u32 + 4, ctx.r9.u64);
	// ori r10,r26,128
	ctx.r10.u64 = ctx.r26.u64 | 128;
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// stw r7,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r7.u32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r6,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r6.u32);
	// stw r5,40(r11)
	PPC_STORE_U32(ctx.r11.u32 + 40, ctx.r5.u32);
	// stw r4,52(r11)
	PPC_STORE_U32(ctx.r11.u32 + 52, ctx.r4.u32);
	// stb r10,256(r11)
	PPC_STORE_U8(ctx.r11.u32 + 256, ctx.r10.u8);
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F11F0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F11F0) { __imp__sub_826F11F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F11F0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x826F11F8;
	__savegprlr_28(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826F1208;
	sub_829745B0(ctx, base);
	// addi r11,r31,48
	ctx.r11.s64 = ctx.r31.s64 + 48;
	// li r10,7
	ctx.r10.s64 = 7;
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// lis r8,8192
	ctx.r8.s64 = 536870912;
	// ori r30,r3,15
	ctx.r30.u64 = ctx.r3.u64 | 15;
	// lbz r28,256(r11)
	ctx.r28.u64 = PPC_LOAD_U8(ctx.r11.u32 + 256);
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// lis r7,8192
	ctx.r7.s64 = 536870912;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// lis r6,4096
	ctx.r6.s64 = 268435456;
	// lis r5,4096
	ctx.r5.s64 = 268435456;
	// lis r4,4096
	ctx.r4.s64 = 268435456;
	// stw r30,60(r11)
	PPC_STORE_U32(ctx.r11.u32 + 60, ctx.r30.u32);
	// ori r29,r3,55
	ctx.r29.u64 = ctx.r3.u64 | 55;
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,13
	ctx.r8.u64 = ctx.r8.u64 | 13;
	// ori r7,r7,29
	ctx.r7.u64 = ctx.r7.u64 | 29;
	// ori r6,r6,33
	ctx.r6.u64 = ctx.r6.u64 | 33;
	// ori r5,r5,14
	ctx.r5.u64 = ctx.r5.u64 | 14;
	// stw r29,68(r11)
	PPC_STORE_U32(ctx.r11.u32 + 68, ctx.r29.u32);
	// ori r4,r4,18
	ctx.r4.u64 = ctx.r4.u64 | 18;
	// std r9,4(r11)
	PPC_STORE_U64(ctx.r11.u32 + 4, ctx.r9.u64);
	// ori r10,r28,128
	ctx.r10.u64 = ctx.r28.u64 | 128;
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// stw r7,24(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24, ctx.r7.u32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r6,36(r11)
	PPC_STORE_U32(ctx.r11.u32 + 36, ctx.r6.u32);
	// stw r5,44(r11)
	PPC_STORE_U32(ctx.r11.u32 + 44, ctx.r5.u32);
	// stw r4,52(r11)
	PPC_STORE_U32(ctx.r11.u32 + 52, ctx.r4.u32);
	// stb r10,256(r11)
	PPC_STORE_U8(ctx.r11.u32 + 256, ctx.r10.u8);
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F1290);  /* forward decl */
PPC_WEAK_FUNC(sub_826F1290) { __imp__sub_826F1290(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F1290) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826F1298;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r26,r4
	ctx.r26.u64 = ctx.r4.u64;
	// mr r25,r3
	ctx.r25.u64 = ctx.r3.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// li r30,0
	ctx.r30.s64 = 0;
	// bl 0x826ea348
	ctx.lr = 0x826F12BC;
	sub_826EA348(ctx, base);
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f1320
	if (ctx.cr6.gt) goto loc_826F1320;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f1318
	if (ctx.cr6.eq) goto loc_826F1318;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,7
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 7, ctx.xer);
	// bgt cr6,0x826f1320
	if (ctx.cr6.gt) goto loc_826F1320;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,4856
	ctx.r12.s64 = ctx.r12.s64 + 4856;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F1318;
	case 1:
		goto loc_826F1318;
	case 2:
		goto loc_826F1318;
	case 3:
		goto loc_826F1318;
	case 4:
		goto loc_826F1318;
	case 5:
		goto loc_826F1318;
	case 6:
		goto loc_826F1320;
	case 7:
		goto loc_826F1318;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F1318:
	// li r31,3
	ctx.r31.s64 = 3;
	// b 0x826f1324
	goto loc_826F1324;
loc_826F1320:
	// li r31,-1
	ctx.r31.s64 = -1;
loc_826F1324:
	// lwz r11,0(r25)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r25.u32 + 0);
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F133C;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826f13e4
	if (ctx.cr6.gt) goto loc_826F13E4;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,4956
	ctx.r12.s64 = ctx.r12.s64 + 4956;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826F136C;
	case 1:
		goto loc_826F138C;
	case 2:
		goto loc_826F13AC;
	case 3:
		goto loc_826F13CC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F136C:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9798
	ctx.lr = 0x826F1380;
	sub_826E9798(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F138C:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9390
	ctx.lr = 0x826F13A0;
	sub_826E9390(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F13AC:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9428
	ctx.lr = 0x826F13C0;
	sub_826E9428(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F13CC:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826F13E0;
	sub_826E95D8(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
loc_826F13E4:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F13F0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F13F0) { __imp__sub_826F13F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F13F0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x826F13F8;
	__savegprlr_27(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826F1408;
	sub_829745B0(ctx, base);
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// addi r11,r31,48
	ctx.r11.s64 = ctx.r31.s64 + 48;
	// ori r30,r3,18
	ctx.r30.u64 = ctx.r3.u64 | 18;
	// li r10,8
	ctx.r10.s64 = 8;
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// lis r8,4096
	ctx.r8.s64 = 268435456;
	// ori r29,r3,15
	ctx.r29.u64 = ctx.r3.u64 | 15;
	// lbz r27,256(r11)
	ctx.r27.u64 = PPC_LOAD_U8(ctx.r11.u32 + 256);
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// stw r30,60(r11)
	PPC_STORE_U32(ctx.r11.u32 + 60, ctx.r30.u32);
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// lis r6,8192
	ctx.r6.s64 = 536870912;
	// lis r5,8192
	ctx.r5.s64 = 536870912;
	// lis r4,4096
	ctx.r4.s64 = 268435456;
	// stw r29,68(r11)
	PPC_STORE_U32(ctx.r11.u32 + 68, ctx.r29.u32);
	// ori r28,r3,55
	ctx.r28.u64 = ctx.r3.u64 | 55;
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,38
	ctx.r8.u64 = ctx.r8.u64 | 38;
	// ori r7,r7,39
	ctx.r7.u64 = ctx.r7.u64 | 39;
	// ori r6,r6,13
	ctx.r6.u64 = ctx.r6.u64 | 13;
	// ori r5,r5,29
	ctx.r5.u64 = ctx.r5.u64 | 29;
	// stw r28,76(r11)
	PPC_STORE_U32(ctx.r11.u32 + 76, ctx.r28.u32);
	// ori r4,r4,14
	ctx.r4.u64 = ctx.r4.u64 | 14;
	// std r9,4(r11)
	PPC_STORE_U64(ctx.r11.u32 + 4, ctx.r9.u64);
	// ori r10,r27,128
	ctx.r10.u64 = ctx.r27.u64 | 128;
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// stw r7,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r7.u32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r6,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r6.u32);
	// stw r5,40(r11)
	PPC_STORE_U32(ctx.r11.u32 + 40, ctx.r5.u32);
	// stw r4,52(r11)
	PPC_STORE_U32(ctx.r11.u32 + 52, ctx.r4.u32);
	// stb r10,256(r11)
	PPC_STORE_U8(ctx.r11.u32 + 256, ctx.r10.u8);
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F1498);  /* forward decl */
PPC_WEAK_FUNC(sub_826F1498) { __imp__sub_826F1498(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F1498) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826F14A0;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r26,r4
	ctx.r26.u64 = ctx.r4.u64;
	// mr r25,r3
	ctx.r25.u64 = ctx.r3.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// li r30,0
	ctx.r30.s64 = 0;
	// bl 0x826ea4f8
	ctx.lr = 0x826F14C4;
	sub_826EA4F8(ctx, base);
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f151c
	if (ctx.cr6.gt) goto loc_826F151C;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f1514
	if (ctx.cr6.eq) goto loc_826F1514;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826f151c
	if (ctx.cr6.gt) goto loc_826F151C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,5376
	ctx.r12.s64 = ctx.r12.s64 + 5376;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F1514;
	case 1:
		goto loc_826F1514;
	case 2:
		goto loc_826F1514;
	case 3:
		goto loc_826F1514;
	case 4:
		goto loc_826F1514;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F1514:
	// li r31,2
	ctx.r31.s64 = 2;
	// b 0x826f1520
	goto loc_826F1520;
loc_826F151C:
	// li r31,-1
	ctx.r31.s64 = -1;
loc_826F1520:
	// lwz r11,0(r25)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r25.u32 + 0);
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F1538;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826f15e0
	if (ctx.cr6.gt) goto loc_826F15E0;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,5464
	ctx.r12.s64 = ctx.r12.s64 + 5464;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826F1568;
	case 1:
		goto loc_826F1588;
	case 2:
		goto loc_826F15A8;
	case 3:
		goto loc_826F15C8;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F1568:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9798
	ctx.lr = 0x826F157C;
	sub_826E9798(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F1588:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9390
	ctx.lr = 0x826F159C;
	sub_826E9390(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F15A8:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9428
	ctx.lr = 0x826F15BC;
	sub_826E9428(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F15C8:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826F15DC;
	sub_826E95D8(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
loc_826F15E0:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F15F0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F15F0) { __imp__sub_826F15F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F15F0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x826F15F8;
	__savegprlr_27(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// li r30,0
	ctx.r30.s64 = 0;
	// cmplwi cr6,r4,3
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 3, ctx.xer);
	// bgt cr6,0x826f1644
	if (ctx.cr6.gt) goto loc_826F1644;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,5676
	ctx.r12.s64 = ctx.r12.s64 + 5676;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826F164C;
	case 1:
		goto loc_826F163C;
	case 2:
		goto loc_826F163C;
	case 3:
		goto loc_826F163C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F163C:
	// li r31,3
	ctx.r31.s64 = 3;
	// b 0x826f1650
	goto loc_826F1650;
loc_826F1644:
	// li r31,-1
	ctx.r31.s64 = -1;
	// b 0x826f1650
	goto loc_826F1650;
loc_826F164C:
	// li r31,2
	ctx.r31.s64 = 2;
loc_826F1650:
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F1660;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826f1708
	if (ctx.cr6.gt) goto loc_826F1708;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,5760
	ctx.r12.s64 = ctx.r12.s64 + 5760;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826F1690;
	case 1:
		goto loc_826F16B0;
	case 2:
		goto loc_826F16D0;
	case 3:
		goto loc_826F16F0;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F1690:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9798
	ctx.lr = 0x826F16A4;
	sub_826E9798(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_826F16B0:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9390
	ctx.lr = 0x826F16C4;
	sub_826E9390(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_826F16D0:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9428
	ctx.lr = 0x826F16E4;
	sub_826E9428(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_826F16F0:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826F1704;
	sub_826E95D8(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
loc_826F1708:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F1718);  /* forward decl */
PPC_WEAK_FUNC(sub_826F1718) { __imp__sub_826F1718(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F1718) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826F1734;
	sub_829745B0(ctx, base);
	// addi r11,r31,48
	ctx.r11.s64 = ctx.r31.s64 + 48;
	// li r10,4
	ctx.r10.s64 = 4;
	// lis r8,8192
	ctx.r8.s64 = 536870912;
	// lis r7,8192
	ctx.r7.s64 = 536870912;
	// lis r6,4096
	ctx.r6.s64 = 268435456;
	// lbz r4,256(r11)
	ctx.r4.u64 = PPC_LOAD_U8(ctx.r11.u32 + 256);
	// lis r5,4096
	ctx.r5.s64 = 268435456;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,62
	ctx.r8.u64 = ctx.r8.u64 | 62;
	// ori r7,r7,13
	ctx.r7.u64 = ctx.r7.u64 | 13;
	// ori r6,r6,38
	ctx.r6.u64 = ctx.r6.u64 | 38;
	// ori r5,r5,39
	ctx.r5.u64 = ctx.r5.u64 | 39;
	// ori r10,r4,128
	ctx.r10.u64 = ctx.r4.u64 | 128;
	// std r9,4(r11)
	PPC_STORE_U64(ctx.r11.u32 + 4, ctx.r9.u64);
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r7,24(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24, ctx.r7.u32);
	// stw r6,36(r11)
	PPC_STORE_U32(ctx.r11.u32 + 36, ctx.r6.u32);
	// stw r5,44(r11)
	PPC_STORE_U32(ctx.r11.u32 + 44, ctx.r5.u32);
	// stb r10,256(r11)
	PPC_STORE_U8(ctx.r11.u32 + 256, ctx.r10.u8);
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F17A0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F17A0) { __imp__sub_826F17A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F17A0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x826F17A8;
	__savegprlr_27(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// li r30,0
	ctx.r30.s64 = 0;
	// cmplwi cr6,r4,3
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 3, ctx.xer);
	// bgt cr6,0x826f17ec
	if (ctx.cr6.gt) goto loc_826F17EC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,6108
	ctx.r12.s64 = ctx.r12.s64 + 6108;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826F17F4;
	case 1:
		goto loc_826F17F4;
	case 2:
		goto loc_826F17F4;
	case 3:
		goto loc_826F17F4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F17EC:
	// li r31,-1
	ctx.r31.s64 = -1;
	// b 0x826f17f8
	goto loc_826F17F8;
loc_826F17F4:
	// li r31,3
	ctx.r31.s64 = 3;
loc_826F17F8:
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F1808;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826f18b0
	if (ctx.cr6.gt) goto loc_826F18B0;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,6184
	ctx.r12.s64 = ctx.r12.s64 + 6184;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826F1838;
	case 1:
		goto loc_826F1858;
	case 2:
		goto loc_826F1878;
	case 3:
		goto loc_826F1898;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F1838:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9798
	ctx.lr = 0x826F184C;
	sub_826E9798(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_826F1858:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9390
	ctx.lr = 0x826F186C;
	sub_826E9390(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_826F1878:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9428
	ctx.lr = 0x826F188C;
	sub_826E9428(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_826F1898:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826F18AC;
	sub_826E95D8(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
loc_826F18B0:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F18C0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F18C0) { __imp__sub_826F18C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F18C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826F18DC;
	sub_829745B0(ctx, base);
	// addi r11,r31,48
	ctx.r11.s64 = ctx.r31.s64 + 48;
	// li r10,4
	ctx.r10.s64 = 4;
	// lis r8,4096
	ctx.r8.s64 = 268435456;
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// lis r6,8192
	ctx.r6.s64 = 536870912;
	// lbz r4,256(r11)
	ctx.r4.u64 = PPC_LOAD_U8(ctx.r11.u32 + 256);
	// lis r5,8192
	ctx.r5.s64 = 536870912;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,38
	ctx.r8.u64 = ctx.r8.u64 | 38;
	// ori r7,r7,39
	ctx.r7.u64 = ctx.r7.u64 | 39;
	// ori r6,r6,13
	ctx.r6.u64 = ctx.r6.u64 | 13;
	// ori r5,r5,55
	ctx.r5.u64 = ctx.r5.u64 | 55;
	// ori r10,r4,128
	ctx.r10.u64 = ctx.r4.u64 | 128;
	// std r9,4(r11)
	PPC_STORE_U64(ctx.r11.u32 + 4, ctx.r9.u64);
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r7,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r7.u32);
	// stw r6,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r6.u32);
	// stw r5,40(r11)
	PPC_STORE_U32(ctx.r11.u32 + 40, ctx.r5.u32);
	// stb r10,256(r11)
	PPC_STORE_U8(ctx.r11.u32 + 256, ctx.r10.u8);
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F1948);  /* forward decl */
PPC_WEAK_FUNC(sub_826F1948) { __imp__sub_826F1948(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F1948) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x826F1950;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826F1960;
	sub_829745B0(ctx, base);
	// addi r11,r31,48
	ctx.r11.s64 = ctx.r31.s64 + 48;
	// li r10,6
	ctx.r10.s64 = 6;
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// lis r8,4096
	ctx.r8.s64 = 268435456;
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// lbz r29,256(r11)
	ctx.r29.u64 = PPC_LOAD_U8(ctx.r11.u32 + 256);
	// lis r6,4096
	ctx.r6.s64 = 268435456;
	// lis r5,8192
	ctx.r5.s64 = 536870912;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// lis r4,8192
	ctx.r4.s64 = 536870912;
	// ori r30,r3,58
	ctx.r30.u64 = ctx.r3.u64 | 58;
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,38
	ctx.r8.u64 = ctx.r8.u64 | 38;
	// ori r7,r7,41
	ctx.r7.u64 = ctx.r7.u64 | 41;
	// ori r6,r6,39
	ctx.r6.u64 = ctx.r6.u64 | 39;
	// ori r5,r5,42
	ctx.r5.u64 = ctx.r5.u64 | 42;
	// stw r30,60(r11)
	PPC_STORE_U32(ctx.r11.u32 + 60, ctx.r30.u32);
	// ori r4,r4,13
	ctx.r4.u64 = ctx.r4.u64 | 13;
	// std r9,4(r11)
	PPC_STORE_U64(ctx.r11.u32 + 4, ctx.r9.u64);
	// ori r10,r29,128
	ctx.r10.u64 = ctx.r29.u64 | 128;
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// stw r7,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r7.u32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r6,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r6.u32);
	// stw r5,36(r11)
	PPC_STORE_U32(ctx.r11.u32 + 36, ctx.r5.u32);
	// stw r4,48(r11)
	PPC_STORE_U32(ctx.r11.u32 + 48, ctx.r4.u32);
	// stb r10,256(r11)
	PPC_STORE_U8(ctx.r11.u32 + 256, ctx.r10.u8);
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F19D8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F19D8) { __imp__sub_826F19D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F19D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826F19E0;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r26,r4
	ctx.r26.u64 = ctx.r4.u64;
	// mr r25,r3
	ctx.r25.u64 = ctx.r3.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// li r30,0
	ctx.r30.s64 = 0;
	// bl 0x826eac08
	ctx.lr = 0x826F1A04;
	sub_826EAC08(ctx, base);
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f1a68
	if (ctx.cr6.gt) goto loc_826F1A68;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f1a60
	if (ctx.cr6.eq) goto loc_826F1A60;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x826f1a68
	if (ctx.cr6.gt) goto loc_826F1A68;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,6720
	ctx.r12.s64 = ctx.r12.s64 + 6720;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F1A58;
	case 1:
		goto loc_826F1A60;
	case 2:
		goto loc_826F1A58;
	case 3:
		goto loc_826F1A68;
	case 4:
		goto loc_826F1A58;
	case 5:
		goto loc_826F1A60;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F1A58:
	// li r31,3
	ctx.r31.s64 = 3;
	// b 0x826f1a6c
	goto loc_826F1A6C;
loc_826F1A60:
	// li r31,2
	ctx.r31.s64 = 2;
	// b 0x826f1a6c
	goto loc_826F1A6C;
loc_826F1A68:
	// li r31,-1
	ctx.r31.s64 = -1;
loc_826F1A6C:
	// lwz r11,0(r25)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r25.u32 + 0);
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F1A84;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826f1b2c
	if (ctx.cr6.gt) goto loc_826F1B2C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,6820
	ctx.r12.s64 = ctx.r12.s64 + 6820;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826F1AB4;
	case 1:
		goto loc_826F1AD4;
	case 2:
		goto loc_826F1AF4;
	case 3:
		goto loc_826F1B14;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F1AB4:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9798
	ctx.lr = 0x826F1AC8;
	sub_826E9798(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F1AD4:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9390
	ctx.lr = 0x826F1AE8;
	sub_826E9390(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F1AF4:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9428
	ctx.lr = 0x826F1B08;
	sub_826E9428(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F1B14:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826F1B28;
	sub_826E95D8(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
loc_826F1B2C:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F1B38);  /* forward decl */
PPC_WEAK_FUNC(sub_826F1B38) { __imp__sub_826F1B38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F1B38) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x826F1B40;
	__savegprlr_27(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// li r30,0
	ctx.r30.s64 = 0;
	// cmplwi cr6,r4,4
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 4, ctx.xer);
	// bgt cr6,0x826f1b88
	if (ctx.cr6.gt) goto loc_826F1B88;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,7028
	ctx.r12.s64 = ctx.r12.s64 + 7028;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826F1B90;
	case 1:
		goto loc_826F1B90;
	case 2:
		goto loc_826F1B90;
	case 3:
		goto loc_826F1B90;
	case 4:
		goto loc_826F1B90;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F1B88:
	// li r31,-1
	ctx.r31.s64 = -1;
	// b 0x826f1b94
	goto loc_826F1B94;
loc_826F1B90:
	// li r31,3
	ctx.r31.s64 = 3;
loc_826F1B94:
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F1BA4;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826f1c4c
	if (ctx.cr6.gt) goto loc_826F1C4C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,7108
	ctx.r12.s64 = ctx.r12.s64 + 7108;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826F1BD4;
	case 1:
		goto loc_826F1BF4;
	case 2:
		goto loc_826F1C14;
	case 3:
		goto loc_826F1C34;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F1BD4:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9798
	ctx.lr = 0x826F1BE8;
	sub_826E9798(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_826F1BF4:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9390
	ctx.lr = 0x826F1C08;
	sub_826E9390(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_826F1C14:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e9428
	ctx.lr = 0x826F1C28;
	sub_826E9428(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_826F1C34:
	// mr r6,r31
	ctx.r6.u64 = ctx.r31.u64;
	// mr r5,r27
	ctx.r5.u64 = ctx.r27.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826F1C48;
	sub_826E95D8(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
loc_826F1C4C:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F1C58);  /* forward decl */
PPC_WEAK_FUNC(sub_826F1C58) { __imp__sub_826F1C58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F1C58) {
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
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826F1C78;
	sub_829745B0(ctx, base);
	// addi r11,r31,48
	ctx.r11.s64 = ctx.r31.s64 + 48;
	// li r10,5
	ctx.r10.s64 = 5;
	// lis r8,4096
	ctx.r8.s64 = 268435456;
	// lis r7,4096
	ctx.r7.s64 = 268435456;
	// lis r6,8192
	ctx.r6.s64 = 536870912;
	// lbz r30,256(r11)
	ctx.r30.u64 = PPC_LOAD_U8(ctx.r11.u32 + 256);
	// lis r5,8192
	ctx.r5.s64 = 536870912;
	// lis r4,8192
	ctx.r4.s64 = 536870912;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,38
	ctx.r8.u64 = ctx.r8.u64 | 38;
	// ori r7,r7,39
	ctx.r7.u64 = ctx.r7.u64 | 39;
	// ori r6,r6,13
	ctx.r6.u64 = ctx.r6.u64 | 13;
	// ori r5,r5,29
	ctx.r5.u64 = ctx.r5.u64 | 29;
	// ori r4,r4,55
	ctx.r4.u64 = ctx.r4.u64 | 55;
	// std r9,4(r11)
	PPC_STORE_U64(ctx.r11.u32 + 4, ctx.r9.u64);
	// ori r10,r30,128
	ctx.r10.u64 = ctx.r30.u64 | 128;
	// stw r8,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r8.u32);
	// stw r7,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r7.u32);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r6,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r6.u32);
	// stw r5,40(r11)
	PPC_STORE_U32(ctx.r11.u32 + 40, ctx.r5.u32);
	// stw r4,52(r11)
	PPC_STORE_U32(ctx.r11.u32 + 52, ctx.r4.u32);
	// stb r10,256(r11)
	PPC_STORE_U8(ctx.r11.u32 + 256, ctx.r10.u8);
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
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

PPC_FUNC_IMPL(__imp__sub_826F1CF8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F1CF8) { __imp__sub_826F1CF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F1CF8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826F1D00;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r25,r4
	ctx.r25.u64 = ctx.r4.u64;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r30,r5
	ctx.r30.u64 = ctx.r5.u64;
	// mr r29,r6
	ctx.r29.u64 = ctx.r6.u64;
	// mr r28,r7
	ctx.r28.u64 = ctx.r7.u64;
	// li r31,0
	ctx.r31.s64 = 0;
	// bl 0x826eb1c0
	ctx.lr = 0x826F1D24;
	sub_826EB1C0(ctx, base);
	// bl 0x826eb0f0
	ctx.lr = 0x826F1D28;
	sub_826EB0F0(ctx, base);
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// mr r4,r25
	ctx.r4.u64 = ctx.r25.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F1D44;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826f1dec
	if (ctx.cr6.gt) goto loc_826F1DEC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,7524
	ctx.r12.s64 = ctx.r12.s64 + 7524;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826F1D74;
	case 1:
		goto loc_826F1D94;
	case 2:
		goto loc_826F1DB4;
	case 3:
		goto loc_826F1DD4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F1D74:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9798
	ctx.lr = 0x826F1D88;
	sub_826E9798(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F1D94:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9390
	ctx.lr = 0x826F1DA8;
	sub_826E9390(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F1DB4:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9428
	ctx.lr = 0x826F1DC8;
	sub_826E9428(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F1DD4:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826F1DE8;
	sub_826E95D8(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
loc_826F1DEC:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F1DF8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F1DF8) { __imp__sub_826F1DF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F1DF8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826F1E00;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r25,r4
	ctx.r25.u64 = ctx.r4.u64;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r30,r5
	ctx.r30.u64 = ctx.r5.u64;
	// mr r29,r6
	ctx.r29.u64 = ctx.r6.u64;
	// mr r28,r7
	ctx.r28.u64 = ctx.r7.u64;
	// li r31,0
	ctx.r31.s64 = 0;
	// bl 0x826eb5b0
	ctx.lr = 0x826F1E24;
	sub_826EB5B0(ctx, base);
	// bl 0x826eb4e0
	ctx.lr = 0x826F1E28;
	sub_826EB4E0(ctx, base);
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// mr r4,r25
	ctx.r4.u64 = ctx.r25.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F1E44;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826f1eec
	if (ctx.cr6.gt) goto loc_826F1EEC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,7780
	ctx.r12.s64 = ctx.r12.s64 + 7780;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826F1E74;
	case 1:
		goto loc_826F1E94;
	case 2:
		goto loc_826F1EB4;
	case 3:
		goto loc_826F1ED4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F1E74:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9798
	ctx.lr = 0x826F1E88;
	sub_826E9798(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F1E94:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9390
	ctx.lr = 0x826F1EA8;
	sub_826E9390(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F1EB4:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9428
	ctx.lr = 0x826F1EC8;
	sub_826E9428(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F1ED4:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826F1EE8;
	sub_826E95D8(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
loc_826F1EEC:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F1EF8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F1EF8) { __imp__sub_826F1EF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F1EF8) {
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
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826F1F18;
	sub_829745B0(ctx, base);
	// addi r30,r31,48
	ctx.r30.s64 = ctx.r31.s64 + 48;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// lbz r11,256(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + 256);
	// ori r11,r11,128
	ctx.r11.u64 = ctx.r11.u64 | 128;
	// stb r11,256(r30)
	PPC_STORE_U8(ctx.r30.u32 + 256, ctx.r11.u8);
	// bl 0x826e5808
	ctx.lr = 0x826F1F30;
	sub_826E5808(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r30,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r30.u32);
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

PPC_FUNC_IMPL(__imp__sub_826F1F50);  /* forward decl */
PPC_WEAK_FUNC(sub_826F1F50) { __imp__sub_826F1F50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F1F50) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826F1F58;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r25,r4
	ctx.r25.u64 = ctx.r4.u64;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r30,r5
	ctx.r30.u64 = ctx.r5.u64;
	// mr r29,r6
	ctx.r29.u64 = ctx.r6.u64;
	// mr r28,r7
	ctx.r28.u64 = ctx.r7.u64;
	// li r31,0
	ctx.r31.s64 = 0;
	// bl 0x826ebd30
	ctx.lr = 0x826F1F7C;
	sub_826EBD30(ctx, base);
	// bl 0x826ebc40
	ctx.lr = 0x826F1F80;
	sub_826EBC40(ctx, base);
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// mr r4,r25
	ctx.r4.u64 = ctx.r25.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F1F9C;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826f2044
	if (ctx.cr6.gt) goto loc_826F2044;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,8124
	ctx.r12.s64 = ctx.r12.s64 + 8124;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826F1FCC;
	case 1:
		goto loc_826F1FEC;
	case 2:
		goto loc_826F200C;
	case 3:
		goto loc_826F202C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F1FCC:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9798
	ctx.lr = 0x826F1FE0;
	sub_826E9798(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F1FEC:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9390
	ctx.lr = 0x826F2000;
	sub_826E9390(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F200C:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9428
	ctx.lr = 0x826F2020;
	sub_826E9428(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F202C:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826F2040;
	sub_826E95D8(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
loc_826F2044:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F2050);  /* forward decl */
PPC_WEAK_FUNC(sub_826F2050) { __imp__sub_826F2050(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F2050) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826F2058;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r25,r4
	ctx.r25.u64 = ctx.r4.u64;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r30,r5
	ctx.r30.u64 = ctx.r5.u64;
	// mr r29,r6
	ctx.r29.u64 = ctx.r6.u64;
	// mr r28,r7
	ctx.r28.u64 = ctx.r7.u64;
	// li r31,0
	ctx.r31.s64 = 0;
	// bl 0x826ec2a0
	ctx.lr = 0x826F207C;
	sub_826EC2A0(ctx, base);
	// bl 0x826ebf80
	ctx.lr = 0x826F2080;
	sub_826EBF80(ctx, base);
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// mr r4,r25
	ctx.r4.u64 = ctx.r25.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F209C;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826f2144
	if (ctx.cr6.gt) goto loc_826F2144;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,8380
	ctx.r12.s64 = ctx.r12.s64 + 8380;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826F20CC;
	case 1:
		goto loc_826F20EC;
	case 2:
		goto loc_826F210C;
	case 3:
		goto loc_826F212C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F20CC:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9798
	ctx.lr = 0x826F20E0;
	sub_826E9798(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F20EC:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9390
	ctx.lr = 0x826F2100;
	sub_826E9390(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F210C:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9428
	ctx.lr = 0x826F2120;
	sub_826E9428(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F212C:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826F2140;
	sub_826E95D8(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
loc_826F2144:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F2150);  /* forward decl */
PPC_WEAK_FUNC(sub_826F2150) { __imp__sub_826F2150(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F2150) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826F2158;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r25,r4
	ctx.r25.u64 = ctx.r4.u64;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r30,r5
	ctx.r30.u64 = ctx.r5.u64;
	// mr r29,r6
	ctx.r29.u64 = ctx.r6.u64;
	// mr r28,r7
	ctx.r28.u64 = ctx.r7.u64;
	// li r31,0
	ctx.r31.s64 = 0;
	// bl 0x826ec558
	ctx.lr = 0x826F217C;
	sub_826EC558(ctx, base);
	// bl 0x826ec488
	ctx.lr = 0x826F2180;
	sub_826EC488(ctx, base);
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// mr r4,r25
	ctx.r4.u64 = ctx.r25.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F219C;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826f2244
	if (ctx.cr6.gt) goto loc_826F2244;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,8636
	ctx.r12.s64 = ctx.r12.s64 + 8636;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826F21CC;
	case 1:
		goto loc_826F21EC;
	case 2:
		goto loc_826F220C;
	case 3:
		goto loc_826F222C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F21CC:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9798
	ctx.lr = 0x826F21E0;
	sub_826E9798(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F21EC:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9390
	ctx.lr = 0x826F2200;
	sub_826E9390(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F220C:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9428
	ctx.lr = 0x826F2220;
	sub_826E9428(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F222C:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826F2240;
	sub_826E95D8(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
loc_826F2244:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F2250);  /* forward decl */
PPC_WEAK_FUNC(sub_826F2250) { __imp__sub_826F2250(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F2250) {
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
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826F2270;
	sub_829745B0(ctx, base);
	// addi r30,r31,48
	ctx.r30.s64 = ctx.r31.s64 + 48;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// lbz r11,256(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + 256);
	// ori r11,r11,128
	ctx.r11.u64 = ctx.r11.u64 | 128;
	// stb r11,256(r30)
	PPC_STORE_U8(ctx.r30.u32 + 256, ctx.r11.u8);
	// bl 0x826e5958
	ctx.lr = 0x826F2288;
	sub_826E5958(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r30,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r30.u32);
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

PPC_FUNC_IMPL(__imp__sub_826F22A8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F22A8) { __imp__sub_826F22A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F22A8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826F22B0;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r25,r4
	ctx.r25.u64 = ctx.r4.u64;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r30,r5
	ctx.r30.u64 = ctx.r5.u64;
	// mr r29,r6
	ctx.r29.u64 = ctx.r6.u64;
	// mr r28,r7
	ctx.r28.u64 = ctx.r7.u64;
	// li r31,0
	ctx.r31.s64 = 0;
	// bl 0x826ecaa0
	ctx.lr = 0x826F22D4;
	sub_826ECAA0(ctx, base);
	// bl 0x826ec9d0
	ctx.lr = 0x826F22D8;
	sub_826EC9D0(ctx, base);
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// mr r4,r25
	ctx.r4.u64 = ctx.r25.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F22F4;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826f239c
	if (ctx.cr6.gt) goto loc_826F239C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,8980
	ctx.r12.s64 = ctx.r12.s64 + 8980;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826F2324;
	case 1:
		goto loc_826F2344;
	case 2:
		goto loc_826F2364;
	case 3:
		goto loc_826F2384;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F2324:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9798
	ctx.lr = 0x826F2338;
	sub_826E9798(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F2344:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9390
	ctx.lr = 0x826F2358;
	sub_826E9390(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F2364:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9428
	ctx.lr = 0x826F2378;
	sub_826E9428(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F2384:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826F2398;
	sub_826E95D8(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
loc_826F239C:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F23A8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F23A8) { __imp__sub_826F23A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F23A8) {
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
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826F23C8;
	sub_829745B0(ctx, base);
	// addi r30,r31,48
	ctx.r30.s64 = ctx.r31.s64 + 48;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// lbz r11,256(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + 256);
	// ori r11,r11,128
	ctx.r11.u64 = ctx.r11.u64 | 128;
	// stb r11,256(r30)
	PPC_STORE_U8(ctx.r30.u32 + 256, ctx.r11.u8);
	// bl 0x826e5a98
	ctx.lr = 0x826F23E0;
	sub_826E5A98(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r30,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r30.u32);
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

PPC_FUNC_IMPL(__imp__sub_826F2400);  /* forward decl */
PPC_WEAK_FUNC(sub_826F2400) { __imp__sub_826F2400(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F2400) {
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
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826F2420;
	sub_829745B0(ctx, base);
	// addi r30,r31,48
	ctx.r30.s64 = ctx.r31.s64 + 48;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// lbz r11,256(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + 256);
	// ori r11,r11,128
	ctx.r11.u64 = ctx.r11.u64 | 128;
	// stb r11,256(r30)
	PPC_STORE_U8(ctx.r30.u32 + 256, ctx.r11.u8);
	// bl 0x826e5bc0
	ctx.lr = 0x826F2438;
	sub_826E5BC0(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r30,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r30.u32);
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

PPC_FUNC_IMPL(__imp__sub_826F2458);  /* forward decl */
PPC_WEAK_FUNC(sub_826F2458) { __imp__sub_826F2458(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F2458) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826F2460;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r25,r4
	ctx.r25.u64 = ctx.r4.u64;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r30,r5
	ctx.r30.u64 = ctx.r5.u64;
	// mr r29,r6
	ctx.r29.u64 = ctx.r6.u64;
	// mr r28,r7
	ctx.r28.u64 = ctx.r7.u64;
	// li r31,0
	ctx.r31.s64 = 0;
	// bl 0x826ecf98
	ctx.lr = 0x826F2484;
	sub_826ECF98(ctx, base);
	// bl 0x826ed138
	ctx.lr = 0x826F2488;
	sub_826ED138(ctx, base);
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// mr r4,r25
	ctx.r4.u64 = ctx.r25.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F24A4;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826f254c
	if (ctx.cr6.gt) goto loc_826F254C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,9412
	ctx.r12.s64 = ctx.r12.s64 + 9412;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826F24D4;
	case 1:
		goto loc_826F24F4;
	case 2:
		goto loc_826F2514;
	case 3:
		goto loc_826F2534;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F24D4:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9798
	ctx.lr = 0x826F24E8;
	sub_826E9798(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F24F4:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9390
	ctx.lr = 0x826F2508;
	sub_826E9390(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F2514:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9428
	ctx.lr = 0x826F2528;
	sub_826E9428(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F2534:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826F2548;
	sub_826E95D8(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
loc_826F254C:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F2558);  /* forward decl */
PPC_WEAK_FUNC(sub_826F2558) { __imp__sub_826F2558(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F2558) {
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
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x829745b0
	ctx.lr = 0x826F2578;
	sub_829745B0(ctx, base);
	// addi r30,r31,48
	ctx.r30.s64 = ctx.r31.s64 + 48;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// lbz r11,256(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + 256);
	// ori r11,r11,128
	ctx.r11.u64 = ctx.r11.u64 | 128;
	// stb r11,256(r30)
	PPC_STORE_U8(ctx.r30.u32 + 256, ctx.r11.u8);
	// bl 0x826e5710
	ctx.lr = 0x826F2590;
	sub_826E5710(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r30,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r30.u32);
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

PPC_FUNC_IMPL(__imp__sub_826F25B0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F25B0) { __imp__sub_826F25B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F25B0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x826F25B8;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r25,r4
	ctx.r25.u64 = ctx.r4.u64;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r30,r5
	ctx.r30.u64 = ctx.r5.u64;
	// mr r29,r6
	ctx.r29.u64 = ctx.r6.u64;
	// mr r28,r7
	ctx.r28.u64 = ctx.r7.u64;
	// li r31,0
	ctx.r31.s64 = 0;
	// bl 0x826edbd8
	ctx.lr = 0x826F25DC;
	sub_826EDBD8(ctx, base);
	// bl 0x826ed610
	ctx.lr = 0x826F25E0;
	sub_826ED610(ctx, base);
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// mr r4,r25
	ctx.r4.u64 = ctx.r25.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// lwz r11,36(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F25FC;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmplwi cr6,r3,3
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 3, ctx.xer);
	// bgt cr6,0x826f26a4
	if (ctx.cr6.gt) goto loc_826F26A4;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,9756
	ctx.r12.s64 = ctx.r12.s64 + 9756;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_826F262C;
	case 1:
		goto loc_826F264C;
	case 2:
		goto loc_826F266C;
	case 3:
		goto loc_826F268C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F262C:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9798
	ctx.lr = 0x826F2640;
	sub_826E9798(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F264C:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9390
	ctx.lr = 0x826F2660;
	sub_826E9390(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F266C:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e9428
	ctx.lr = 0x826F2680;
	sub_826E9428(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
loc_826F268C:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826e95d8
	ctx.lr = 0x826F26A0;
	sub_826E95D8(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
loc_826F26A4:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F26B0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F26B0) { __imp__sub_826F26B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F26B0) {
	PPC_FUNC_PROLOGUE();
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F26B8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F26B8) { __imp__sub_826F26B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F26B8) {
	PPC_FUNC_PROLOGUE();
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F26C0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F26C0) { __imp__sub_826F26C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F26C0) {
	PPC_FUNC_PROLOGUE();
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F26C8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F26C8) { __imp__sub_826F26C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F26C8) {
	PPC_FUNC_PROLOGUE();
	// li r11,0
	ctx.r11.s64 = 0;
	// li r10,-1
	ctx.r10.s64 = -1;
	// stw r11,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r11.u32);
	// stw r11,80(r3)
	PPC_STORE_U32(ctx.r3.u32 + 80, ctx.r11.u32);
	// stw r11,224(r3)
	PPC_STORE_U32(ctx.r3.u32 + 224, ctx.r11.u32);
	// stw r10,232(r3)
	PPC_STORE_U32(ctx.r3.u32 + 232, ctx.r10.u32);
	// b 0x826e7190
	sub_826E7190(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F26E8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F26E8) { __imp__sub_826F26E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F26E8) {
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
	// lwz r31,0(r30)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x826f2730
	if (ctx.cr6.eq) goto loc_826F2730;
loc_826F270C:
	// lwz r11,16(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 16);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r11,0(r30)
	PPC_STORE_U32(ctx.r30.u32 + 0, ctx.r11.u32);
	// bl 0x826e5cf8
	ctx.lr = 0x826F271C;
	sub_826E5CF8(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8218be78
	ctx.lr = 0x826F2724;
	sub_8218BE78(ctx, base);
	// lwz r31,0(r30)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// bne cr6,0x826f270c
	if (!ctx.cr6.eq) goto loc_826F270C;
loc_826F2730:
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

PPC_FUNC_IMPL(__imp__sub_826F2748);  /* forward decl */
PPC_WEAK_FUNC(sub_826F2748) { __imp__sub_826F2748(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F2748) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// mr r7,r4
	ctx.r7.u64 = ctx.r4.u64;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
loc_826F2758:
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// cmpw cr6,r6,r10
	ctx.cr6.compare<int32_t>(ctx.r6.s32, ctx.r10.s32, ctx.xer);
	// beq cr6,0x826f2774
	if (ctx.cr6.eq) goto loc_826F2774;
	// lwz r11,16(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826f2758
	if (!ctx.cr6.eq) goto loc_826F2758;
	// blr 
	return;
loc_826F2774:
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// mr r4,r10
	ctx.r4.u64 = ctx.r10.u64;
	// lfs f1,-6944(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -6944);
	ctx.f1.f64 = double(temp.f32);
	// b 0x826e72f8
	sub_826E72F8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F2790);  /* forward decl */
PPC_WEAK_FUNC(sub_826F2790) { __imp__sub_826F2790(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F2790) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f2838
	if (ctx.cr6.gt) goto loc_826F2838;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f282c
	if (ctx.cr6.eq) goto loc_826F282C;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x826f2838
	if (ctx.cr6.gt) goto loc_826F2838;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,10188
	ctx.r12.s64 = ctx.r12.s64 + 10188;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F27E4;
	case 1:
		goto loc_826F27F0;
	case 2:
		goto loc_826F27FC;
	case 3:
		goto loc_826F2808;
	case 4:
		goto loc_826F2820;
	case 5:
		goto loc_826F2814;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F27E4:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,38
	ctx.r3.u64 = ctx.r3.u64 | 38;
	// blr 
	return;
loc_826F27F0:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,39
	ctx.r3.u64 = ctx.r3.u64 | 39;
	// blr 
	return;
loc_826F27FC:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,44
	ctx.r3.u64 = ctx.r3.u64 | 44;
	// blr 
	return;
loc_826F2808:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F2814:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	// blr 
	return;
loc_826F2820:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,15
	ctx.r3.u64 = ctx.r3.u64 | 15;
	// blr 
	return;
loc_826F282C:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
loc_826F2838:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F2840);  /* forward decl */
PPC_WEAK_FUNC(sub_826F2840) { __imp__sub_826F2840(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F2840) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bl 0x826e7470
	ctx.lr = 0x826F2854;
	sub_826E7470(ctx, base);
	// bl 0x826f2790
	ctx.lr = 0x826F2858;
	sub_826F2790(ctx, base);
	// bl 0x826f2b90
	ctx.lr = 0x826F285C;
	sub_826F2B90(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F2870);  /* forward decl */
PPC_WEAK_FUNC(sub_826F2870) { __imp__sub_826F2870(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F2870) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f2790
	sub_826F2790(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F2878);  /* forward decl */
PPC_WEAK_FUNC(sub_826F2878) { __imp__sub_826F2878(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F2878) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f2920
	if (ctx.cr6.gt) goto loc_826F2920;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f2914
	if (ctx.cr6.eq) goto loc_826F2914;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x826f2920
	if (ctx.cr6.gt) goto loc_826F2920;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,10420
	ctx.r12.s64 = ctx.r12.s64 + 10420;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F28CC;
	case 1:
		goto loc_826F28D8;
	case 2:
		goto loc_826F28E4;
	case 3:
		goto loc_826F28F0;
	case 4:
		goto loc_826F28FC;
	case 5:
		goto loc_826F2908;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F28CC:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,38
	ctx.r3.u64 = ctx.r3.u64 | 38;
	// blr 
	return;
loc_826F28D8:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,39
	ctx.r3.u64 = ctx.r3.u64 | 39;
	// blr 
	return;
loc_826F28E4:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,44
	ctx.r3.u64 = ctx.r3.u64 | 44;
	// blr 
	return;
loc_826F28F0:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F28FC:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	// blr 
	return;
loc_826F2908:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,15
	ctx.r3.u64 = ctx.r3.u64 | 15;
	// blr 
	return;
loc_826F2914:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
loc_826F2920:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F2928);  /* forward decl */
PPC_WEAK_FUNC(sub_826F2928) { __imp__sub_826F2928(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F2928) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f2878
	sub_826F2878(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F2930);  /* forward decl */
PPC_WEAK_FUNC(sub_826F2930) { __imp__sub_826F2930(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F2930) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bl 0x826e7908
	ctx.lr = 0x826F2944;
	sub_826E7908(ctx, base);
	// bl 0x826f2878
	ctx.lr = 0x826F2948;
	sub_826F2878(ctx, base);
	// bl 0x826f2b90
	ctx.lr = 0x826F294C;
	sub_826F2B90(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F2960);  /* forward decl */
PPC_WEAK_FUNC(sub_826F2960) { __imp__sub_826F2960(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F2960) {
	PPC_FUNC_PROLOGUE();
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,39
	ctx.r11.u64 = ctx.r11.u64 | 39;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f29b0
	if (ctx.cr6.gt) goto loc_826F29B0;
	// beq cr6,0x826f29a8
	if (ctx.cr6.eq) goto loc_826F29A8;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addic. r11,r11,-14
	ctx.xer.ca = ctx.r11.u32 > 13;
	ctx.r11.s64 = ctx.r11.s64 + -14;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f29a0
	if (ctx.cr0.eq) goto loc_826F29A0;
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// beq cr6,0x826f2998
	if (ctx.cr6.eq) goto loc_826F2998;
	// cmplwi cr6,r11,24
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 24, ctx.xer);
	// bne cr6,0x826f29cc
	if (!ctx.cr6.eq) goto loc_826F29CC;
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826F2998:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826F29A0:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826F29A8:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826F29B0:
	// addis r11,r3,-8192
	ctx.r11.s64 = ctx.r3.s64 + -536870912;
	// addic. r11,r11,-13
	ctx.xer.ca = ctx.r11.u32 > 12;
	ctx.r11.s64 = ctx.r11.s64 + -13;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f29e4
	if (ctx.cr0.eq) goto loc_826F29E4;
	// cmplwi cr6,r11,31
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 31, ctx.xer);
	// beq cr6,0x826f29dc
	if (ctx.cr6.eq) goto loc_826F29DC;
	// cmplwi cr6,r11,42
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 42, ctx.xer);
	// beq cr6,0x826f29d4
	if (ctx.cr6.eq) goto loc_826F29D4;
loc_826F29CC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F29D4:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826F29DC:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826F29E4:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F29F0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F29F0) { __imp__sub_826F29F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F29F0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bl 0x826e7a40
	ctx.lr = 0x826F2A04;
	sub_826E7A40(ctx, base);
	// bl 0x826f2878
	ctx.lr = 0x826F2A08;
	sub_826F2878(ctx, base);
	// bl 0x826f2960
	ctx.lr = 0x826F2A0C;
	sub_826F2960(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F2A20);  /* forward decl */
PPC_WEAK_FUNC(sub_826F2A20) { __imp__sub_826F2A20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F2A20) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eaf0
	ctx.lr = 0x826F2A28;
	__savegprlr_18(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r22,0
	ctx.r22.s64 = 0;
	// mr r23,r6
	ctx.r23.u64 = ctx.r6.u64;
	// mr r24,r7
	ctx.r24.u64 = ctx.r7.u64;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// li r18,1
	ctx.r18.s64 = 1;
	// mr r21,r5
	ctx.r21.u64 = ctx.r5.u64;
	// std r22,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r22.u64);
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// stb r22,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r22.u8);
	// mr r31,r18
	ctx.r31.u64 = ctx.r18.u64;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r28,r4
	ctx.r28.u64 = ctx.r4.u64;
	// add r20,r4,r9
	ctx.r20.u64 = ctx.r4.u64 + ctx.r9.u64;
	// lwz r11,44(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F2A6C;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r19,r3
	ctx.r19.u64 = ctx.r3.u64;
	// mr r27,r22
	ctx.r27.u64 = ctx.r22.u64;
loc_826F2A74:
	// clrlwi r11,r31,24
	ctx.r11.u64 = ctx.r31.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826f2b84
	if (ctx.cr6.eq) goto loc_826F2B84;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// lwz r11,48(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 48);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F2A98;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// bl 0x82977d18
	ctx.lr = 0x826F2AA8;
	sub_82977D18(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826f2b78
	if (!ctx.cr6.eq) goto loc_826F2B78;
	// cmplwi cr6,r27,6
	ctx.cr6.compare<uint32_t>(ctx.r27.u32, 6, ctx.xer);
	// bgt cr6,0x826f2b00
	if (ctx.cr6.gt) goto loc_826F2B00;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,10964
	ctx.r12.s64 = ctx.r12.s64 + 10964;
	// rlwinm r0,r27,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r27.u32 | (ctx.r27.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r27.u32) {
	case 0:
		goto loc_826F2AF0;
	case 1:
		goto loc_826F2AF0;
	case 2:
		goto loc_826F2AF8;
	case 3:
		goto loc_826F2AF8;
	case 4:
		goto loc_826F2AF8;
	case 5:
		goto loc_826F2AF0;
	case 6:
		goto loc_826F2AF0;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F2AF0:
	// li r6,4
	ctx.r6.s64 = 4;
	// b 0x826f2b04
	goto loc_826F2B04;
loc_826F2AF8:
	// li r6,8
	ctx.r6.s64 = 8;
	// b 0x826f2b04
	goto loc_826F2B04;
loc_826F2B00:
	// mr r6,r22
	ctx.r6.u64 = ctx.r22.u64;
loc_826F2B04:
	// cmplw cr6,r19,r27
	ctx.cr6.compare<uint32_t>(ctx.r19.u32, ctx.r27.u32, ctx.xer);
	// bne cr6,0x826f2b2c
	if (!ctx.cr6.eq) goto loc_826F2B2C;
	// li r6,8
	ctx.r6.s64 = 8;
	// mr r5,r21
	ctx.r5.u64 = ctx.r21.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F2B20;
	sub_82978530(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// stb r31,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r31.u8);
	// b 0x826f2b78
	goto loc_826F2B78;
loc_826F2B2C:
	// add r30,r6,r28
	ctx.r30.u64 = ctx.r6.u64 + ctx.r28.u64;
	// cmplw cr6,r30,r20
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, ctx.r20.u32, ctx.xer);
	// bgt cr6,0x826f2b58
	if (ctx.cr6.gt) goto loc_826F2B58;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F2B48;
	sub_82978530(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// mr r11,r18
	ctx.r11.u64 = ctx.r18.u64;
	// bne cr6,0x826f2b5c
	if (!ctx.cr6.eq) goto loc_826F2B5C;
loc_826F2B58:
	// mr r11,r22
	ctx.r11.u64 = ctx.r22.u64;
loc_826F2B5C:
	// clrlwi r31,r11,24
	ctx.r31.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x826f2b74
	if (ctx.cr6.eq) goto loc_826F2B74;
	// extsw r11,r27
	ctx.r11.s64 = ctx.r27.s32;
	// sld r11,r18,r11
	ctx.r11.u64 = ctx.r11.u8 & 0x40 ? 0 : (ctx.r18.u64 << (ctx.r11.u8 & 0x7F));
	// std r11,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r11.u64);
loc_826F2B74:
	// mr r28,r30
	ctx.r28.u64 = ctx.r30.u64;
loc_826F2B78:
	// addi r27,r27,1
	ctx.r27.s64 = ctx.r27.s64 + 1;
	// cmpwi cr6,r27,7
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 7, ctx.xer);
	// blt cr6,0x826f2a74
	if (ctx.cr6.lt) goto loc_826F2A74;
loc_826F2B84:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x8298eb40
	__restgprlr_18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F2B90);  /* forward decl */
PPC_WEAK_FUNC(sub_826F2B90) { __imp__sub_826F2B90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F2B90) {
	PPC_FUNC_PROLOGUE();
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,39
	ctx.r11.u64 = ctx.r11.u64 | 39;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f2be0
	if (ctx.cr6.gt) goto loc_826F2BE0;
	// beq cr6,0x826f2bd8
	if (ctx.cr6.eq) goto loc_826F2BD8;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addic. r11,r11,-14
	ctx.xer.ca = ctx.r11.u32 > 13;
	ctx.r11.s64 = ctx.r11.s64 + -14;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f2bd0
	if (ctx.cr0.eq) goto loc_826F2BD0;
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// beq cr6,0x826f2bc8
	if (ctx.cr6.eq) goto loc_826F2BC8;
	// cmplwi cr6,r11,24
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 24, ctx.xer);
	// bne cr6,0x826f2bfc
	if (!ctx.cr6.eq) goto loc_826F2BFC;
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826F2BC8:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826F2BD0:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826F2BD8:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826F2BE0:
	// addis r11,r3,-8192
	ctx.r11.s64 = ctx.r3.s64 + -536870912;
	// addic. r11,r11,-13
	ctx.xer.ca = ctx.r11.u32 > 12;
	ctx.r11.s64 = ctx.r11.s64 + -13;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f2c14
	if (ctx.cr0.eq) goto loc_826F2C14;
	// cmplwi cr6,r11,31
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 31, ctx.xer);
	// beq cr6,0x826f2c0c
	if (ctx.cr6.eq) goto loc_826F2C0C;
	// cmplwi cr6,r11,42
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 42, ctx.xer);
	// beq cr6,0x826f2c04
	if (ctx.cr6.eq) goto loc_826F2C04;
loc_826F2BFC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F2C04:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826F2C0C:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826F2C14:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F2C20);  /* forward decl */
PPC_WEAK_FUNC(sub_826F2C20) { __imp__sub_826F2C20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F2C20) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eaf0
	ctx.lr = 0x826F2C28;
	__savegprlr_18(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r22,0
	ctx.r22.s64 = 0;
	// mr r23,r6
	ctx.r23.u64 = ctx.r6.u64;
	// mr r24,r7
	ctx.r24.u64 = ctx.r7.u64;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// li r18,1
	ctx.r18.s64 = 1;
	// mr r21,r5
	ctx.r21.u64 = ctx.r5.u64;
	// std r22,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r22.u64);
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// stb r22,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r22.u8);
	// mr r31,r18
	ctx.r31.u64 = ctx.r18.u64;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r28,r4
	ctx.r28.u64 = ctx.r4.u64;
	// add r20,r4,r9
	ctx.r20.u64 = ctx.r4.u64 + ctx.r9.u64;
	// lwz r11,44(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F2C6C;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r19,r3
	ctx.r19.u64 = ctx.r3.u64;
	// mr r27,r22
	ctx.r27.u64 = ctx.r22.u64;
loc_826F2C74:
	// clrlwi r11,r31,24
	ctx.r11.u64 = ctx.r31.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826f2d84
	if (ctx.cr6.eq) goto loc_826F2D84;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// lwz r11,48(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 48);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F2C98;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// bl 0x82977d18
	ctx.lr = 0x826F2CA8;
	sub_82977D18(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826f2d78
	if (!ctx.cr6.eq) goto loc_826F2D78;
	// cmplwi cr6,r27,6
	ctx.cr6.compare<uint32_t>(ctx.r27.u32, 6, ctx.xer);
	// bgt cr6,0x826f2d00
	if (ctx.cr6.gt) goto loc_826F2D00;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,11476
	ctx.r12.s64 = ctx.r12.s64 + 11476;
	// rlwinm r0,r27,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r27.u32 | (ctx.r27.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r27.u32) {
	case 0:
		goto loc_826F2CF0;
	case 1:
		goto loc_826F2CF0;
	case 2:
		goto loc_826F2CF8;
	case 3:
		goto loc_826F2CF8;
	case 4:
		goto loc_826F2CF0;
	case 5:
		goto loc_826F2CF0;
	case 6:
		goto loc_826F2CF8;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F2CF0:
	// li r6,4
	ctx.r6.s64 = 4;
	// b 0x826f2d04
	goto loc_826F2D04;
loc_826F2CF8:
	// li r6,8
	ctx.r6.s64 = 8;
	// b 0x826f2d04
	goto loc_826F2D04;
loc_826F2D00:
	// mr r6,r22
	ctx.r6.u64 = ctx.r22.u64;
loc_826F2D04:
	// cmplw cr6,r19,r27
	ctx.cr6.compare<uint32_t>(ctx.r19.u32, ctx.r27.u32, ctx.xer);
	// bne cr6,0x826f2d2c
	if (!ctx.cr6.eq) goto loc_826F2D2C;
	// li r6,8
	ctx.r6.s64 = 8;
	// mr r5,r21
	ctx.r5.u64 = ctx.r21.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F2D20;
	sub_82978530(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// stb r31,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r31.u8);
	// b 0x826f2d78
	goto loc_826F2D78;
loc_826F2D2C:
	// add r30,r6,r28
	ctx.r30.u64 = ctx.r6.u64 + ctx.r28.u64;
	// cmplw cr6,r30,r20
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, ctx.r20.u32, ctx.xer);
	// bgt cr6,0x826f2d58
	if (ctx.cr6.gt) goto loc_826F2D58;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F2D48;
	sub_82978530(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// mr r11,r18
	ctx.r11.u64 = ctx.r18.u64;
	// bne cr6,0x826f2d5c
	if (!ctx.cr6.eq) goto loc_826F2D5C;
loc_826F2D58:
	// mr r11,r22
	ctx.r11.u64 = ctx.r22.u64;
loc_826F2D5C:
	// clrlwi r31,r11,24
	ctx.r31.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x826f2d74
	if (ctx.cr6.eq) goto loc_826F2D74;
	// extsw r11,r27
	ctx.r11.s64 = ctx.r27.s32;
	// sld r11,r18,r11
	ctx.r11.u64 = ctx.r11.u8 & 0x40 ? 0 : (ctx.r18.u64 << (ctx.r11.u8 & 0x7F));
	// std r11,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r11.u64);
loc_826F2D74:
	// mr r28,r30
	ctx.r28.u64 = ctx.r30.u64;
loc_826F2D78:
	// addi r27,r27,1
	ctx.r27.s64 = ctx.r27.s64 + 1;
	// cmpwi cr6,r27,7
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 7, ctx.xer);
	// blt cr6,0x826f2c74
	if (ctx.cr6.lt) goto loc_826F2C74;
loc_826F2D84:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x8298eb40
	__restgprlr_18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F2D90);  /* forward decl */
PPC_WEAK_FUNC(sub_826F2D90) { __imp__sub_826F2D90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F2D90) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r3,3
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 3, ctx.xer);
	// bgt cr6,0x826f2dd0
	if (ctx.cr6.gt) goto loc_826F2DD0;
	// beq cr6,0x826f2dc4
	if (ctx.cr6.eq) goto loc_826F2DC4;
	// cmpwi cr6,r3,1
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 1, ctx.xer);
	// beq cr6,0x826f2db8
	if (ctx.cr6.eq) goto loc_826F2DB8;
	// cmpwi cr6,r3,2
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 2, ctx.xer);
	// bne cr6,0x826f2de0
	if (!ctx.cr6.eq) goto loc_826F2DE0;
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,39
	ctx.r3.u64 = ctx.r3.u64 | 39;
	// blr 
	return;
loc_826F2DB8:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,38
	ctx.r3.u64 = ctx.r3.u64 | 38;
	// blr 
	return;
loc_826F2DC4:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,44
	ctx.r3.u64 = ctx.r3.u64 | 44;
	// blr 
	return;
loc_826F2DD0:
	// cmpwi cr6,r3,4
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 4, ctx.xer);
	// beq cr6,0x826f2df4
	if (ctx.cr6.eq) goto loc_826F2DF4;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f2de8
	if (ctx.cr6.eq) goto loc_826F2DE8;
loc_826F2DE0:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F2DE8:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
loc_826F2DF4:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F2E00);  /* forward decl */
PPC_WEAK_FUNC(sub_826F2E00) { __imp__sub_826F2E00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F2E00) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f2d90
	sub_826F2D90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F2E08);  /* forward decl */
PPC_WEAK_FUNC(sub_826F2E08) { __imp__sub_826F2E08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F2E08) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eaf0
	ctx.lr = 0x826F2E10;
	__savegprlr_18(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r22,0
	ctx.r22.s64 = 0;
	// mr r23,r6
	ctx.r23.u64 = ctx.r6.u64;
	// mr r24,r7
	ctx.r24.u64 = ctx.r7.u64;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// li r18,1
	ctx.r18.s64 = 1;
	// mr r21,r5
	ctx.r21.u64 = ctx.r5.u64;
	// std r22,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r22.u64);
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// stb r22,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r22.u8);
	// mr r31,r18
	ctx.r31.u64 = ctx.r18.u64;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r28,r4
	ctx.r28.u64 = ctx.r4.u64;
	// add r20,r4,r9
	ctx.r20.u64 = ctx.r4.u64 + ctx.r9.u64;
	// lwz r11,44(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F2E54;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r19,r3
	ctx.r19.u64 = ctx.r3.u64;
	// mr r27,r22
	ctx.r27.u64 = ctx.r22.u64;
loc_826F2E5C:
	// clrlwi r11,r31,24
	ctx.r11.u64 = ctx.r31.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826f2f64
	if (ctx.cr6.eq) goto loc_826F2F64;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// lwz r11,48(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 48);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F2E80;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// bl 0x82977d18
	ctx.lr = 0x826F2E90;
	sub_82977D18(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826f2f58
	if (!ctx.cr6.eq) goto loc_826F2F58;
	// cmplwi cr6,r27,4
	ctx.cr6.compare<uint32_t>(ctx.r27.u32, 4, ctx.xer);
	// bgt cr6,0x826f2ee0
	if (ctx.cr6.gt) goto loc_826F2EE0;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,11964
	ctx.r12.s64 = ctx.r12.s64 + 11964;
	// rlwinm r0,r27,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r27.u32 | (ctx.r27.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r27.u32) {
	case 0:
		goto loc_826F2ED0;
	case 1:
		goto loc_826F2ED0;
	case 2:
		goto loc_826F2ED8;
	case 3:
		goto loc_826F2ED8;
	case 4:
		goto loc_826F2ED8;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F2ED0:
	// li r6,4
	ctx.r6.s64 = 4;
	// b 0x826f2ee4
	goto loc_826F2EE4;
loc_826F2ED8:
	// li r6,8
	ctx.r6.s64 = 8;
	// b 0x826f2ee4
	goto loc_826F2EE4;
loc_826F2EE0:
	// mr r6,r22
	ctx.r6.u64 = ctx.r22.u64;
loc_826F2EE4:
	// cmplw cr6,r19,r27
	ctx.cr6.compare<uint32_t>(ctx.r19.u32, ctx.r27.u32, ctx.xer);
	// bne cr6,0x826f2f0c
	if (!ctx.cr6.eq) goto loc_826F2F0C;
	// li r6,8
	ctx.r6.s64 = 8;
	// mr r5,r21
	ctx.r5.u64 = ctx.r21.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F2F00;
	sub_82978530(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// stb r31,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r31.u8);
	// b 0x826f2f58
	goto loc_826F2F58;
loc_826F2F0C:
	// add r30,r6,r28
	ctx.r30.u64 = ctx.r6.u64 + ctx.r28.u64;
	// cmplw cr6,r30,r20
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, ctx.r20.u32, ctx.xer);
	// bgt cr6,0x826f2f38
	if (ctx.cr6.gt) goto loc_826F2F38;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F2F28;
	sub_82978530(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// mr r11,r18
	ctx.r11.u64 = ctx.r18.u64;
	// bne cr6,0x826f2f3c
	if (!ctx.cr6.eq) goto loc_826F2F3C;
loc_826F2F38:
	// mr r11,r22
	ctx.r11.u64 = ctx.r22.u64;
loc_826F2F3C:
	// clrlwi r31,r11,24
	ctx.r31.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x826f2f54
	if (ctx.cr6.eq) goto loc_826F2F54;
	// extsw r11,r27
	ctx.r11.s64 = ctx.r27.s32;
	// sld r11,r18,r11
	ctx.r11.u64 = ctx.r11.u8 & 0x40 ? 0 : (ctx.r18.u64 << (ctx.r11.u8 & 0x7F));
	// std r11,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r11.u64);
loc_826F2F54:
	// mr r28,r30
	ctx.r28.u64 = ctx.r30.u64;
loc_826F2F58:
	// addi r27,r27,1
	ctx.r27.s64 = ctx.r27.s64 + 1;
	// cmpwi cr6,r27,5
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 5, ctx.xer);
	// blt cr6,0x826f2e5c
	if (ctx.cr6.lt) goto loc_826F2E5C;
loc_826F2F64:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x8298eb40
	__restgprlr_18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F2F70);  /* forward decl */
PPC_WEAK_FUNC(sub_826F2F70) { __imp__sub_826F2F70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F2F70) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// cmplwi cr6,r4,4
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 4, ctx.xer);
	// bgt cr6,0x826f2fdc
	if (ctx.cr6.gt) goto loc_826F2FDC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,12188
	ctx.r12.s64 = ctx.r12.s64 + 12188;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826F2FB0;
	case 1:
		goto loc_826F2FB8;
	case 2:
		goto loc_826F2FC0;
	case 3:
		goto loc_826F2FC8;
	case 4:
		goto loc_826F2FD4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F2FB0:
	// li r3,1
	ctx.r3.s64 = 1;
	// b 0x826f2fe0
	goto loc_826F2FE0;
loc_826F2FB8:
	// li r3,2
	ctx.r3.s64 = 2;
	// b 0x826f2fe0
	goto loc_826F2FE0;
loc_826F2FC0:
	// li r3,4
	ctx.r3.s64 = 4;
	// b 0x826f2fe0
	goto loc_826F2FE0;
loc_826F2FC8:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// b 0x826f2fe0
	goto loc_826F2FE0;
loc_826F2FD4:
	// li r3,3
	ctx.r3.s64 = 3;
	// b 0x826f2fe0
	goto loc_826F2FE0;
loc_826F2FDC:
	// li r3,-1
	ctx.r3.s64 = -1;
loc_826F2FE0:
	// bl 0x826f2d90
	ctx.lr = 0x826F2FE4;
	sub_826F2D90(ctx, base);
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// ori r11,r11,13
	ctx.r11.u64 = ctx.r11.u64 | 13;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f3048
	if (ctx.cr6.gt) goto loc_826F3048;
	// beq cr6,0x826f3034
	if (ctx.cr6.eq) goto loc_826F3034;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addic. r11,r11,-38
	ctx.xer.ca = ctx.r11.u32 > 37;
	ctx.r11.s64 = ctx.r11.s64 + -38;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f3020
	if (ctx.cr0.eq) goto loc_826F3020;
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// bne cr6,0x826f305c
	if (!ctx.cr6.eq) goto loc_826F305C;
	// li r3,1
	ctx.r3.s64 = 1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F3020:
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
loc_826F3034:
	// li r3,2
	ctx.r3.s64 = 2;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F3048:
	// addis r11,r3,-8192
	ctx.r11.s64 = ctx.r3.s64 + -536870912;
	// addic. r11,r11,-44
	ctx.xer.ca = ctx.r11.u32 > 43;
	ctx.r11.s64 = ctx.r11.s64 + -44;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f3084
	if (ctx.cr0.eq) goto loc_826F3084;
	// cmplwi cr6,r11,11
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 11, ctx.xer);
	// beq cr6,0x826f3070
	if (ctx.cr6.eq) goto loc_826F3070;
loc_826F305C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F3070:
	// li r3,3
	ctx.r3.s64 = 3;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F3084:
	// li r3,4
	ctx.r3.s64 = 4;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F3098);  /* forward decl */
PPC_WEAK_FUNC(sub_826F3098) { __imp__sub_826F3098(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F3098) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// cmplwi cr6,r4,4
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 4, ctx.xer);
	// bgt cr6,0x826f3104
	if (ctx.cr6.gt) goto loc_826F3104;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,12484
	ctx.r12.s64 = ctx.r12.s64 + 12484;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826F30D8;
	case 1:
		goto loc_826F30E0;
	case 2:
		goto loc_826F30E8;
	case 3:
		goto loc_826F30F0;
	case 4:
		goto loc_826F30F8;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F30D8:
	// li r3,1
	ctx.r3.s64 = 1;
	// b 0x826f3108
	goto loc_826F3108;
loc_826F30E0:
	// li r3,2
	ctx.r3.s64 = 2;
	// b 0x826f3108
	goto loc_826F3108;
loc_826F30E8:
	// li r3,3
	ctx.r3.s64 = 3;
	// b 0x826f3108
	goto loc_826F3108;
loc_826F30F0:
	// li r3,4
	ctx.r3.s64 = 4;
	// b 0x826f3108
	goto loc_826F3108;
loc_826F30F8:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// b 0x826f3108
	goto loc_826F3108;
loc_826F3104:
	// li r3,-1
	ctx.r3.s64 = -1;
loc_826F3108:
	// bl 0x826f2d90
	ctx.lr = 0x826F310C;
	sub_826F2D90(ctx, base);
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// ori r11,r11,13
	ctx.r11.u64 = ctx.r11.u64 | 13;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f3170
	if (ctx.cr6.gt) goto loc_826F3170;
	// beq cr6,0x826f315c
	if (ctx.cr6.eq) goto loc_826F315C;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addic. r11,r11,-38
	ctx.xer.ca = ctx.r11.u32 > 37;
	ctx.r11.s64 = ctx.r11.s64 + -38;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f3148
	if (ctx.cr0.eq) goto loc_826F3148;
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// bne cr6,0x826f3184
	if (!ctx.cr6.eq) goto loc_826F3184;
	// li r3,1
	ctx.r3.s64 = 1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F3148:
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
loc_826F315C:
	// li r3,3
	ctx.r3.s64 = 3;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F3170:
	// addis r11,r3,-8192
	ctx.r11.s64 = ctx.r3.s64 + -536870912;
	// addic. r11,r11,-44
	ctx.xer.ca = ctx.r11.u32 > 43;
	ctx.r11.s64 = ctx.r11.s64 + -44;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f31ac
	if (ctx.cr0.eq) goto loc_826F31AC;
	// cmplwi cr6,r11,11
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 11, ctx.xer);
	// beq cr6,0x826f3198
	if (ctx.cr6.eq) goto loc_826F3198;
loc_826F3184:
	// li r3,-1
	ctx.r3.s64 = -1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F3198:
	// li r3,4
	ctx.r3.s64 = 4;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F31AC:
	// li r3,2
	ctx.r3.s64 = 2;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F31C0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F31C0) { __imp__sub_826F31C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F31C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f326c
	if (ctx.cr6.gt) goto loc_826F326C;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f3260
	if (ctx.cr6.eq) goto loc_826F3260;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,6
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 6, ctx.xer);
	// bgt cr6,0x826f326c
	if (ctx.cr6.gt) goto loc_826F326C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,12796
	ctx.r12.s64 = ctx.r12.s64 + 12796;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F3224;
	case 1:
		goto loc_826F3248;
	case 2:
		goto loc_826F3218;
	case 3:
		goto loc_826F3254;
	case 4:
		goto loc_826F323C;
	case 5:
		goto loc_826F326C;
	case 6:
		goto loc_826F3230;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F3218:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,38
	ctx.r3.u64 = ctx.r3.u64 | 38;
	// blr 
	return;
loc_826F3224:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,39
	ctx.r3.u64 = ctx.r3.u64 | 39;
	// blr 
	return;
loc_826F3230:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F323C:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,34
	ctx.r3.u64 = ctx.r3.u64 | 34;
	// blr 
	return;
loc_826F3248:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	// blr 
	return;
loc_826F3254:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,15
	ctx.r3.u64 = ctx.r3.u64 | 15;
	// blr 
	return;
loc_826F3260:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
loc_826F326C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F3278);  /* forward decl */
PPC_WEAK_FUNC(sub_826F3278) { __imp__sub_826F3278(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F3278) {
	PPC_FUNC_PROLOGUE();
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,38
	ctx.r11.u64 = ctx.r11.u64 | 38;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f32c8
	if (ctx.cr6.gt) goto loc_826F32C8;
	// beq cr6,0x826f32c0
	if (ctx.cr6.eq) goto loc_826F32C0;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addic. r11,r11,-14
	ctx.xer.ca = ctx.r11.u32 > 13;
	ctx.r11.s64 = ctx.r11.s64 + -14;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f32b8
	if (ctx.cr0.eq) goto loc_826F32B8;
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// beq cr6,0x826f32b0
	if (ctx.cr6.eq) goto loc_826F32B0;
	// cmplwi cr6,r11,20
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 20, ctx.xer);
	// bne cr6,0x826f32ec
	if (!ctx.cr6.eq) goto loc_826F32EC;
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826F32B0:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826F32B8:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826F32C0:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826F32C8:
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,39
	ctx.r11.u64 = ctx.r11.u64 | 39;
	// cmplw cr6,r3,r11
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, ctx.r11.u32, ctx.xer);
	// beq cr6,0x826f3304
	if (ctx.cr6.eq) goto loc_826F3304;
	// addis r11,r3,-8192
	ctx.r11.s64 = ctx.r3.s64 + -536870912;
	// addic. r11,r11,-13
	ctx.xer.ca = ctx.r11.u32 > 12;
	ctx.r11.s64 = ctx.r11.s64 + -13;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f32fc
	if (ctx.cr0.eq) goto loc_826F32FC;
	// cmplwi cr6,r11,42
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 42, ctx.xer);
	// beq cr6,0x826f32f4
	if (ctx.cr6.eq) goto loc_826F32F4;
loc_826F32EC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F32F4:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826F32FC:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826F3304:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F3310);  /* forward decl */
PPC_WEAK_FUNC(sub_826F3310) { __imp__sub_826F3310(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F3310) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bl 0x826e8708
	ctx.lr = 0x826F3324;
	sub_826E8708(ctx, base);
	// bl 0x826f31c0
	ctx.lr = 0x826F3328;
	sub_826F31C0(ctx, base);
	// bl 0x826f3278
	ctx.lr = 0x826F332C;
	sub_826F3278(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F3340);  /* forward decl */
PPC_WEAK_FUNC(sub_826F3340) { __imp__sub_826F3340(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F3340) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f31c0
	sub_826F31C0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F3348);  /* forward decl */
PPC_WEAK_FUNC(sub_826F3348) { __imp__sub_826F3348(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F3348) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eaf0
	ctx.lr = 0x826F3350;
	__savegprlr_18(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r22,0
	ctx.r22.s64 = 0;
	// mr r23,r6
	ctx.r23.u64 = ctx.r6.u64;
	// mr r24,r7
	ctx.r24.u64 = ctx.r7.u64;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// li r18,1
	ctx.r18.s64 = 1;
	// mr r21,r5
	ctx.r21.u64 = ctx.r5.u64;
	// std r22,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r22.u64);
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// stb r22,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r22.u8);
	// mr r31,r18
	ctx.r31.u64 = ctx.r18.u64;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r28,r4
	ctx.r28.u64 = ctx.r4.u64;
	// add r20,r4,r9
	ctx.r20.u64 = ctx.r4.u64 + ctx.r9.u64;
	// lwz r11,44(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F3394;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r19,r3
	ctx.r19.u64 = ctx.r3.u64;
	// mr r27,r22
	ctx.r27.u64 = ctx.r22.u64;
loc_826F339C:
	// clrlwi r11,r31,24
	ctx.r11.u64 = ctx.r31.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826f34ac
	if (ctx.cr6.eq) goto loc_826F34AC;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// lwz r11,48(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 48);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F33C0;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// bl 0x82977d18
	ctx.lr = 0x826F33D0;
	sub_82977D18(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826f34a0
	if (!ctx.cr6.eq) goto loc_826F34A0;
	// cmplwi cr6,r27,6
	ctx.cr6.compare<uint32_t>(ctx.r27.u32, 6, ctx.xer);
	// bgt cr6,0x826f3428
	if (ctx.cr6.gt) goto loc_826F3428;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,13308
	ctx.r12.s64 = ctx.r12.s64 + 13308;
	// rlwinm r0,r27,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r27.u32 | (ctx.r27.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r27.u32) {
	case 0:
		goto loc_826F3418;
	case 1:
		goto loc_826F3418;
	case 2:
		goto loc_826F3420;
	case 3:
		goto loc_826F3418;
	case 4:
		goto loc_826F3418;
	case 5:
		goto loc_826F3418;
	case 6:
		goto loc_826F3420;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F3418:
	// li r6,4
	ctx.r6.s64 = 4;
	// b 0x826f342c
	goto loc_826F342C;
loc_826F3420:
	// li r6,8
	ctx.r6.s64 = 8;
	// b 0x826f342c
	goto loc_826F342C;
loc_826F3428:
	// mr r6,r22
	ctx.r6.u64 = ctx.r22.u64;
loc_826F342C:
	// cmplw cr6,r19,r27
	ctx.cr6.compare<uint32_t>(ctx.r19.u32, ctx.r27.u32, ctx.xer);
	// bne cr6,0x826f3454
	if (!ctx.cr6.eq) goto loc_826F3454;
	// li r6,8
	ctx.r6.s64 = 8;
	// mr r5,r21
	ctx.r5.u64 = ctx.r21.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F3448;
	sub_82978530(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// stb r31,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r31.u8);
	// b 0x826f34a0
	goto loc_826F34A0;
loc_826F3454:
	// add r30,r6,r28
	ctx.r30.u64 = ctx.r6.u64 + ctx.r28.u64;
	// cmplw cr6,r30,r20
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, ctx.r20.u32, ctx.xer);
	// bgt cr6,0x826f3480
	if (ctx.cr6.gt) goto loc_826F3480;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F3470;
	sub_82978530(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// mr r11,r18
	ctx.r11.u64 = ctx.r18.u64;
	// bne cr6,0x826f3484
	if (!ctx.cr6.eq) goto loc_826F3484;
loc_826F3480:
	// mr r11,r22
	ctx.r11.u64 = ctx.r22.u64;
loc_826F3484:
	// clrlwi r31,r11,24
	ctx.r31.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x826f349c
	if (ctx.cr6.eq) goto loc_826F349C;
	// extsw r11,r27
	ctx.r11.s64 = ctx.r27.s32;
	// sld r11,r18,r11
	ctx.r11.u64 = ctx.r11.u8 & 0x40 ? 0 : (ctx.r18.u64 << (ctx.r11.u8 & 0x7F));
	// std r11,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r11.u64);
loc_826F349C:
	// mr r28,r30
	ctx.r28.u64 = ctx.r30.u64;
loc_826F34A0:
	// addi r27,r27,1
	ctx.r27.s64 = ctx.r27.s64 + 1;
	// cmpwi cr6,r27,7
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 7, ctx.xer);
	// blt cr6,0x826f339c
	if (ctx.cr6.lt) goto loc_826F339C;
loc_826F34AC:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x8298eb40
	__restgprlr_18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F34B8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F34B8) { __imp__sub_826F34B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F34B8) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r3,4
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 4, ctx.xer);
	// bgt cr6,0x826f34f8
	if (ctx.cr6.gt) goto loc_826F34F8;
	// beq cr6,0x826f34ec
	if (ctx.cr6.eq) goto loc_826F34EC;
	// cmpwi cr6,r3,1
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 1, ctx.xer);
	// beq cr6,0x826f34e0
	if (ctx.cr6.eq) goto loc_826F34E0;
	// cmpwi cr6,r3,2
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 2, ctx.xer);
	// bne cr6,0x826f3508
	if (!ctx.cr6.eq) goto loc_826F3508;
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	// blr 
	return;
loc_826F34E0:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F34EC:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,15
	ctx.r3.u64 = ctx.r3.u64 | 15;
	// blr 
	return;
loc_826F34F8:
	// cmpwi cr6,r3,5
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 5, ctx.xer);
	// beq cr6,0x826f351c
	if (ctx.cr6.eq) goto loc_826F351C;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f3510
	if (ctx.cr6.eq) goto loc_826F3510;
loc_826F3508:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F3510:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
loc_826F351C:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,34
	ctx.r3.u64 = ctx.r3.u64 | 34;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F3528);  /* forward decl */
PPC_WEAK_FUNC(sub_826F3528) { __imp__sub_826F3528(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F3528) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// cmplwi cr6,r4,4
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 4, ctx.xer);
	// bgt cr6,0x826f3594
	if (ctx.cr6.gt) goto loc_826F3594;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,13652
	ctx.r12.s64 = ctx.r12.s64 + 13652;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826F3568;
	case 1:
		goto loc_826F3570;
	case 2:
		goto loc_826F3578;
	case 3:
		goto loc_826F3580;
	case 4:
		goto loc_826F3588;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F3568:
	// li r3,1
	ctx.r3.s64 = 1;
	// b 0x826f3598
	goto loc_826F3598;
loc_826F3570:
	// li r3,5
	ctx.r3.s64 = 5;
	// b 0x826f3598
	goto loc_826F3598;
loc_826F3578:
	// li r3,2
	ctx.r3.s64 = 2;
	// b 0x826f3598
	goto loc_826F3598;
loc_826F3580:
	// li r3,4
	ctx.r3.s64 = 4;
	// b 0x826f3598
	goto loc_826F3598;
loc_826F3588:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// b 0x826f3598
	goto loc_826F3598;
loc_826F3594:
	// li r3,-1
	ctx.r3.s64 = -1;
loc_826F3598:
	// bl 0x826f34b8
	ctx.lr = 0x826F359C;
	sub_826F34B8(ctx, base);
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,34
	ctx.r11.u64 = ctx.r11.u64 | 34;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f3600
	if (ctx.cr6.gt) goto loc_826F3600;
	// beq cr6,0x826f35ec
	if (ctx.cr6.eq) goto loc_826F35EC;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addic. r11,r11,-14
	ctx.xer.ca = ctx.r11.u32 > 13;
	ctx.r11.s64 = ctx.r11.s64 + -14;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f35d8
	if (ctx.cr0.eq) goto loc_826F35D8;
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// bne cr6,0x826f3614
	if (!ctx.cr6.eq) goto loc_826F3614;
	// li r3,3
	ctx.r3.s64 = 3;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F35D8:
	// li r3,2
	ctx.r3.s64 = 2;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F35EC:
	// li r3,1
	ctx.r3.s64 = 1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F3600:
	// addis r11,r3,-8192
	ctx.r11.s64 = ctx.r3.s64 + -536870912;
	// addic. r11,r11,-13
	ctx.xer.ca = ctx.r11.u32 > 12;
	ctx.r11.s64 = ctx.r11.s64 + -13;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f363c
	if (ctx.cr0.eq) goto loc_826F363C;
	// cmplwi cr6,r11,42
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 42, ctx.xer);
	// beq cr6,0x826f3628
	if (ctx.cr6.eq) goto loc_826F3628;
loc_826F3614:
	// li r3,-1
	ctx.r3.s64 = -1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F3628:
	// li r3,4
	ctx.r3.s64 = 4;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F363C:
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

PPC_FUNC_IMPL(__imp__sub_826F3650);  /* forward decl */
PPC_WEAK_FUNC(sub_826F3650) { __imp__sub_826F3650(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F3650) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f34b8
	sub_826F34B8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F3658);  /* forward decl */
PPC_WEAK_FUNC(sub_826F3658) { __imp__sub_826F3658(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F3658) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f36f0
	if (ctx.cr6.gt) goto loc_826F36F0;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f36e4
	if (ctx.cr6.eq) goto loc_826F36E4;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826f36f0
	if (ctx.cr6.gt) goto loc_826F36F0;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,13972
	ctx.r12.s64 = ctx.r12.s64 + 13972;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F36A8;
	case 1:
		goto loc_826F36CC;
	case 2:
		goto loc_826F36B4;
	case 3:
		goto loc_826F36D8;
	case 4:
		goto loc_826F36C0;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F36A8:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,38
	ctx.r3.u64 = ctx.r3.u64 | 38;
	// blr 
	return;
loc_826F36B4:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,39
	ctx.r3.u64 = ctx.r3.u64 | 39;
	// blr 
	return;
loc_826F36C0:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F36CC:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	// blr 
	return;
loc_826F36D8:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,15
	ctx.r3.u64 = ctx.r3.u64 | 15;
	// blr 
	return;
loc_826F36E4:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
loc_826F36F0:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F36F8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F36F8) { __imp__sub_826F36F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F36F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,39
	ctx.r11.u64 = ctx.r11.u64 | 39;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f3748
	if (ctx.cr6.gt) goto loc_826F3748;
	// beq cr6,0x826f3740
	if (ctx.cr6.eq) goto loc_826F3740;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addic. r11,r11,-14
	ctx.xer.ca = ctx.r11.u32 > 13;
	ctx.r11.s64 = ctx.r11.s64 + -14;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f3738
	if (ctx.cr0.eq) goto loc_826F3738;
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// beq cr6,0x826f3730
	if (ctx.cr6.eq) goto loc_826F3730;
	// cmplwi cr6,r11,24
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 24, ctx.xer);
	// bne cr6,0x826f375c
	if (!ctx.cr6.eq) goto loc_826F375C;
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826F3730:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826F3738:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826F3740:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826F3748:
	// addis r11,r3,-8192
	ctx.r11.s64 = ctx.r3.s64 + -536870912;
	// addic. r11,r11,-13
	ctx.xer.ca = ctx.r11.u32 > 12;
	ctx.r11.s64 = ctx.r11.s64 + -13;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f376c
	if (ctx.cr0.eq) goto loc_826F376C;
	// cmplwi cr6,r11,42
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 42, ctx.xer);
	// beq cr6,0x826f3764
	if (ctx.cr6.eq) goto loc_826F3764;
loc_826F375C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F3764:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826F376C:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F3778);  /* forward decl */
PPC_WEAK_FUNC(sub_826F3778) { __imp__sub_826F3778(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F3778) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bl 0x826e89c8
	ctx.lr = 0x826F378C;
	sub_826E89C8(ctx, base);
	// bl 0x826f3658
	ctx.lr = 0x826F3790;
	sub_826F3658(ctx, base);
	// bl 0x826f36f8
	ctx.lr = 0x826F3794;
	sub_826F36F8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F37A8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F37A8) { __imp__sub_826F37A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F37A8) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f3658
	sub_826F3658(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F37B0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F37B0) { __imp__sub_826F37B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F37B0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eaf0
	ctx.lr = 0x826F37B8;
	__savegprlr_18(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r22,0
	ctx.r22.s64 = 0;
	// mr r23,r6
	ctx.r23.u64 = ctx.r6.u64;
	// mr r24,r7
	ctx.r24.u64 = ctx.r7.u64;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// li r18,1
	ctx.r18.s64 = 1;
	// mr r21,r5
	ctx.r21.u64 = ctx.r5.u64;
	// std r22,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r22.u64);
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// stb r22,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r22.u8);
	// mr r31,r18
	ctx.r31.u64 = ctx.r18.u64;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r28,r4
	ctx.r28.u64 = ctx.r4.u64;
	// add r20,r4,r9
	ctx.r20.u64 = ctx.r4.u64 + ctx.r9.u64;
	// lwz r11,44(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F37FC;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r19,r3
	ctx.r19.u64 = ctx.r3.u64;
	// mr r27,r22
	ctx.r27.u64 = ctx.r22.u64;
loc_826F3804:
	// clrlwi r11,r31,24
	ctx.r11.u64 = ctx.r31.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826f3910
	if (ctx.cr6.eq) goto loc_826F3910;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// lwz r11,48(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 48);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F3828;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// bl 0x82977d18
	ctx.lr = 0x826F3838;
	sub_82977D18(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826f3904
	if (!ctx.cr6.eq) goto loc_826F3904;
	// cmplwi cr6,r27,5
	ctx.cr6.compare<uint32_t>(ctx.r27.u32, 5, ctx.xer);
	// bgt cr6,0x826f388c
	if (ctx.cr6.gt) goto loc_826F388C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,14436
	ctx.r12.s64 = ctx.r12.s64 + 14436;
	// rlwinm r0,r27,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r27.u32 | (ctx.r27.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r27.u32) {
	case 0:
		goto loc_826F387C;
	case 1:
		goto loc_826F387C;
	case 2:
		goto loc_826F3884;
	case 3:
		goto loc_826F387C;
	case 4:
		goto loc_826F387C;
	case 5:
		goto loc_826F3884;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F387C:
	// li r6,4
	ctx.r6.s64 = 4;
	// b 0x826f3890
	goto loc_826F3890;
loc_826F3884:
	// li r6,8
	ctx.r6.s64 = 8;
	// b 0x826f3890
	goto loc_826F3890;
loc_826F388C:
	// mr r6,r22
	ctx.r6.u64 = ctx.r22.u64;
loc_826F3890:
	// cmplw cr6,r19,r27
	ctx.cr6.compare<uint32_t>(ctx.r19.u32, ctx.r27.u32, ctx.xer);
	// bne cr6,0x826f38b8
	if (!ctx.cr6.eq) goto loc_826F38B8;
	// li r6,8
	ctx.r6.s64 = 8;
	// mr r5,r21
	ctx.r5.u64 = ctx.r21.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F38AC;
	sub_82978530(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// stb r31,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r31.u8);
	// b 0x826f3904
	goto loc_826F3904;
loc_826F38B8:
	// add r30,r6,r28
	ctx.r30.u64 = ctx.r6.u64 + ctx.r28.u64;
	// cmplw cr6,r30,r20
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, ctx.r20.u32, ctx.xer);
	// bgt cr6,0x826f38e4
	if (ctx.cr6.gt) goto loc_826F38E4;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F38D4;
	sub_82978530(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// mr r11,r18
	ctx.r11.u64 = ctx.r18.u64;
	// bne cr6,0x826f38e8
	if (!ctx.cr6.eq) goto loc_826F38E8;
loc_826F38E4:
	// mr r11,r22
	ctx.r11.u64 = ctx.r22.u64;
loc_826F38E8:
	// clrlwi r31,r11,24
	ctx.r31.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x826f3900
	if (ctx.cr6.eq) goto loc_826F3900;
	// extsw r11,r27
	ctx.r11.s64 = ctx.r27.s32;
	// sld r11,r18,r11
	ctx.r11.u64 = ctx.r11.u8 & 0x40 ? 0 : (ctx.r18.u64 << (ctx.r11.u8 & 0x7F));
	// std r11,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r11.u64);
loc_826F3900:
	// mr r28,r30
	ctx.r28.u64 = ctx.r30.u64;
loc_826F3904:
	// addi r27,r27,1
	ctx.r27.s64 = ctx.r27.s64 + 1;
	// cmpwi cr6,r27,6
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 6, ctx.xer);
	// blt cr6,0x826f3804
	if (ctx.cr6.lt) goto loc_826F3804;
loc_826F3910:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x8298eb40
	__restgprlr_18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F3920);  /* forward decl */
PPC_WEAK_FUNC(sub_826F3920) { __imp__sub_826F3920(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F3920) {
	PPC_FUNC_PROLOGUE();
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// cmplwi cr6,r4,3
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 3, ctx.xer);
	// ori r10,r11,14
	ctx.r10.u64 = ctx.r11.u64 | 14;
	// bgt cr6,0x826f396c
	if (ctx.cr6.gt) goto loc_826F396C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,14664
	ctx.r12.s64 = ctx.r12.s64 + 14664;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826F3990;
	case 1:
		goto loc_826F3964;
	case 2:
		goto loc_826F3958;
	case 3:
		goto loc_826F39A0;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F3958:
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,15
	ctx.r11.u64 = ctx.r11.u64 | 15;
	// b 0x826f3970
	goto loc_826F3970;
loc_826F3964:
	// mr r11,r10
	ctx.r11.u64 = ctx.r10.u64;
	// b 0x826f3970
	goto loc_826F3970;
loc_826F396C:
	// li r11,-1
	ctx.r11.s64 = -1;
loc_826F3970:
	// subf. r11,r10,r11
	ctx.r11.u64 = ctx.r11.u64 - ctx.r10.u64;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f3988
	if (ctx.cr0.eq) goto loc_826F3988;
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// bne cr6,0x826f3998
	if (!ctx.cr6.eq) goto loc_826F3998;
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826F3988:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826F3990:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826F3998:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F39A0:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F39A8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F39A8) { __imp__sub_826F39A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F39A8) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r4,3
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 3, ctx.xer);
	// bgt cr6,0x826f39e8
	if (ctx.cr6.gt) goto loc_826F39E8;
	// beq cr6,0x826f39dc
	if (ctx.cr6.eq) goto loc_826F39DC;
	// cmpwi cr6,r4,1
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 1, ctx.xer);
	// beq cr6,0x826f39d0
	if (ctx.cr6.eq) goto loc_826F39D0;
	// cmpwi cr6,r4,2
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 2, ctx.xer);
	// bne cr6,0x826f39f0
	if (!ctx.cr6.eq) goto loc_826F39F0;
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,15
	ctx.r3.u64 = ctx.r3.u64 | 15;
	// blr 
	return;
loc_826F39D0:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	// blr 
	return;
loc_826F39DC:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F39E8:
	// cmplwi cr6,r4,65534
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 65534, ctx.xer);
	// beq cr6,0x826f39f8
	if (ctx.cr6.eq) goto loc_826F39F8;
loc_826F39F0:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F39F8:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F3A08);  /* forward decl */
PPC_WEAK_FUNC(sub_826F3A08) { __imp__sub_826F3A08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F3A08) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eaf0
	ctx.lr = 0x826F3A10;
	__savegprlr_18(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r22,0
	ctx.r22.s64 = 0;
	// mr r23,r6
	ctx.r23.u64 = ctx.r6.u64;
	// mr r24,r7
	ctx.r24.u64 = ctx.r7.u64;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// li r18,1
	ctx.r18.s64 = 1;
	// mr r21,r5
	ctx.r21.u64 = ctx.r5.u64;
	// std r22,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r22.u64);
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// stb r22,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r22.u8);
	// mr r31,r18
	ctx.r31.u64 = ctx.r18.u64;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r28,r4
	ctx.r28.u64 = ctx.r4.u64;
	// add r20,r4,r9
	ctx.r20.u64 = ctx.r4.u64 + ctx.r9.u64;
	// lwz r11,44(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F3A54;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r19,r3
	ctx.r19.u64 = ctx.r3.u64;
	// mr r27,r22
	ctx.r27.u64 = ctx.r22.u64;
loc_826F3A5C:
	// clrlwi r11,r31,24
	ctx.r11.u64 = ctx.r31.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826f3b60
	if (ctx.cr6.eq) goto loc_826F3B60;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// lwz r11,48(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 48);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F3A80;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// bl 0x82977d18
	ctx.lr = 0x826F3A90;
	sub_82977D18(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826f3b54
	if (!ctx.cr6.eq) goto loc_826F3B54;
	// cmplwi cr6,r27,3
	ctx.cr6.compare<uint32_t>(ctx.r27.u32, 3, ctx.xer);
	// bgt cr6,0x826f3adc
	if (ctx.cr6.gt) goto loc_826F3ADC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,15036
	ctx.r12.s64 = ctx.r12.s64 + 15036;
	// rlwinm r0,r27,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r27.u32 | (ctx.r27.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r27.u32) {
	case 0:
		goto loc_826F3ACC;
	case 1:
		goto loc_826F3AD4;
	case 2:
		goto loc_826F3AD4;
	case 3:
		goto loc_826F3ACC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F3ACC:
	// li r6,8
	ctx.r6.s64 = 8;
	// b 0x826f3ae0
	goto loc_826F3AE0;
loc_826F3AD4:
	// li r6,4
	ctx.r6.s64 = 4;
	// b 0x826f3ae0
	goto loc_826F3AE0;
loc_826F3ADC:
	// mr r6,r22
	ctx.r6.u64 = ctx.r22.u64;
loc_826F3AE0:
	// cmplw cr6,r19,r27
	ctx.cr6.compare<uint32_t>(ctx.r19.u32, ctx.r27.u32, ctx.xer);
	// bne cr6,0x826f3b08
	if (!ctx.cr6.eq) goto loc_826F3B08;
	// li r6,8
	ctx.r6.s64 = 8;
	// mr r5,r21
	ctx.r5.u64 = ctx.r21.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F3AFC;
	sub_82978530(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// stb r31,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r31.u8);
	// b 0x826f3b54
	goto loc_826F3B54;
loc_826F3B08:
	// add r29,r6,r28
	ctx.r29.u64 = ctx.r6.u64 + ctx.r28.u64;
	// cmplw cr6,r29,r20
	ctx.cr6.compare<uint32_t>(ctx.r29.u32, ctx.r20.u32, ctx.xer);
	// bgt cr6,0x826f3b34
	if (ctx.cr6.gt) goto loc_826F3B34;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F3B24;
	sub_82978530(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// mr r11,r18
	ctx.r11.u64 = ctx.r18.u64;
	// bne cr6,0x826f3b38
	if (!ctx.cr6.eq) goto loc_826F3B38;
loc_826F3B34:
	// mr r11,r22
	ctx.r11.u64 = ctx.r22.u64;
loc_826F3B38:
	// clrlwi r31,r11,24
	ctx.r31.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x826f3b50
	if (ctx.cr6.eq) goto loc_826F3B50;
	// extsw r11,r27
	ctx.r11.s64 = ctx.r27.s32;
	// sld r11,r18,r11
	ctx.r11.u64 = ctx.r11.u8 & 0x40 ? 0 : (ctx.r18.u64 << (ctx.r11.u8 & 0x7F));
	// std r11,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r11.u64);
loc_826F3B50:
	// mr r28,r29
	ctx.r28.u64 = ctx.r29.u64;
loc_826F3B54:
	// addi r27,r27,1
	ctx.r27.s64 = ctx.r27.s64 + 1;
	// cmpwi cr6,r27,4
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 4, ctx.xer);
	// blt cr6,0x826f3a5c
	if (ctx.cr6.lt) goto loc_826F3A5C;
loc_826F3B60:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x8298eb40
	__restgprlr_18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F3B70);  /* forward decl */
PPC_WEAK_FUNC(sub_826F3B70) { __imp__sub_826F3B70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F3B70) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f3c18
	if (ctx.cr6.gt) goto loc_826F3C18;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f3c0c
	if (ctx.cr6.eq) goto loc_826F3C0C;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x826f3c18
	if (ctx.cr6.gt) goto loc_826F3C18;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,15276
	ctx.r12.s64 = ctx.r12.s64 + 15276;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F3BC4;
	case 1:
		goto loc_826F3BF4;
	case 2:
		goto loc_826F3BD0;
	case 3:
		goto loc_826F3C00;
	case 4:
		goto loc_826F3BE8;
	case 5:
		goto loc_826F3BDC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F3BC4:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,38
	ctx.r3.u64 = ctx.r3.u64 | 38;
	// blr 
	return;
loc_826F3BD0:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,39
	ctx.r3.u64 = ctx.r3.u64 | 39;
	// blr 
	return;
loc_826F3BDC:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F3BE8:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,33
	ctx.r3.u64 = ctx.r3.u64 | 33;
	// blr 
	return;
loc_826F3BF4:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	// blr 
	return;
loc_826F3C00:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,15
	ctx.r3.u64 = ctx.r3.u64 | 15;
	// blr 
	return;
loc_826F3C0C:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
loc_826F3C18:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F3C20);  /* forward decl */
PPC_WEAK_FUNC(sub_826F3C20) { __imp__sub_826F3C20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F3C20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,38
	ctx.r11.u64 = ctx.r11.u64 | 38;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f3c70
	if (ctx.cr6.gt) goto loc_826F3C70;
	// beq cr6,0x826f3c68
	if (ctx.cr6.eq) goto loc_826F3C68;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addic. r11,r11,-14
	ctx.xer.ca = ctx.r11.u32 > 13;
	ctx.r11.s64 = ctx.r11.s64 + -14;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f3c60
	if (ctx.cr0.eq) goto loc_826F3C60;
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// beq cr6,0x826f3c58
	if (ctx.cr6.eq) goto loc_826F3C58;
	// cmplwi cr6,r11,19
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 19, ctx.xer);
	// bne cr6,0x826f3c94
	if (!ctx.cr6.eq) goto loc_826F3C94;
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826F3C58:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826F3C60:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826F3C68:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826F3C70:
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,39
	ctx.r11.u64 = ctx.r11.u64 | 39;
	// cmplw cr6,r3,r11
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, ctx.r11.u32, ctx.xer);
	// beq cr6,0x826f3cac
	if (ctx.cr6.eq) goto loc_826F3CAC;
	// addis r11,r3,-8192
	ctx.r11.s64 = ctx.r3.s64 + -536870912;
	// addic. r11,r11,-13
	ctx.xer.ca = ctx.r11.u32 > 12;
	ctx.r11.s64 = ctx.r11.s64 + -13;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f3ca4
	if (ctx.cr0.eq) goto loc_826F3CA4;
	// cmplwi cr6,r11,42
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 42, ctx.xer);
	// beq cr6,0x826f3c9c
	if (ctx.cr6.eq) goto loc_826F3C9C;
loc_826F3C94:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F3C9C:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826F3CA4:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826F3CAC:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F3CB8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F3CB8) { __imp__sub_826F3CB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F3CB8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bl 0x826e8da0
	ctx.lr = 0x826F3CCC;
	sub_826E8DA0(ctx, base);
	// bl 0x826f3b70
	ctx.lr = 0x826F3CD0;
	sub_826F3B70(ctx, base);
	// bl 0x826f3c20
	ctx.lr = 0x826F3CD4;
	sub_826F3C20(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F3CE8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F3CE8) { __imp__sub_826F3CE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F3CE8) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f3b70
	sub_826F3B70(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F3CF0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F3CF0) { __imp__sub_826F3CF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F3CF0) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r3,4
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 4, ctx.xer);
	// bgt cr6,0x826f3d30
	if (ctx.cr6.gt) goto loc_826F3D30;
	// beq cr6,0x826f3d24
	if (ctx.cr6.eq) goto loc_826F3D24;
	// cmpwi cr6,r3,1
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 1, ctx.xer);
	// beq cr6,0x826f3d18
	if (ctx.cr6.eq) goto loc_826F3D18;
	// cmpwi cr6,r3,2
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 2, ctx.xer);
	// bne cr6,0x826f3d40
	if (!ctx.cr6.eq) goto loc_826F3D40;
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	// blr 
	return;
loc_826F3D18:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F3D24:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,15
	ctx.r3.u64 = ctx.r3.u64 | 15;
	// blr 
	return;
loc_826F3D30:
	// cmpwi cr6,r3,5
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 5, ctx.xer);
	// beq cr6,0x826f3d54
	if (ctx.cr6.eq) goto loc_826F3D54;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f3d48
	if (ctx.cr6.eq) goto loc_826F3D48;
loc_826F3D40:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F3D48:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
loc_826F3D54:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,33
	ctx.r3.u64 = ctx.r3.u64 | 33;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F3D60);  /* forward decl */
PPC_WEAK_FUNC(sub_826F3D60) { __imp__sub_826F3D60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F3D60) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// cmplwi cr6,r4,4
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 4, ctx.xer);
	// bgt cr6,0x826f3dcc
	if (ctx.cr6.gt) goto loc_826F3DCC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,15756
	ctx.r12.s64 = ctx.r12.s64 + 15756;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826F3DA0;
	case 1:
		goto loc_826F3DA8;
	case 2:
		goto loc_826F3DB0;
	case 3:
		goto loc_826F3DB8;
	case 4:
		goto loc_826F3DC0;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F3DA0:
	// li r3,1
	ctx.r3.s64 = 1;
	// b 0x826f3dd0
	goto loc_826F3DD0;
loc_826F3DA8:
	// li r3,5
	ctx.r3.s64 = 5;
	// b 0x826f3dd0
	goto loc_826F3DD0;
loc_826F3DB0:
	// li r3,2
	ctx.r3.s64 = 2;
	// b 0x826f3dd0
	goto loc_826F3DD0;
loc_826F3DB8:
	// li r3,4
	ctx.r3.s64 = 4;
	// b 0x826f3dd0
	goto loc_826F3DD0;
loc_826F3DC0:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// b 0x826f3dd0
	goto loc_826F3DD0;
loc_826F3DCC:
	// li r3,-1
	ctx.r3.s64 = -1;
loc_826F3DD0:
	// bl 0x826f3cf0
	ctx.lr = 0x826F3DD4;
	sub_826F3CF0(ctx, base);
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,33
	ctx.r11.u64 = ctx.r11.u64 | 33;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f3e38
	if (ctx.cr6.gt) goto loc_826F3E38;
	// beq cr6,0x826f3e24
	if (ctx.cr6.eq) goto loc_826F3E24;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addic. r11,r11,-14
	ctx.xer.ca = ctx.r11.u32 > 13;
	ctx.r11.s64 = ctx.r11.s64 + -14;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f3e10
	if (ctx.cr0.eq) goto loc_826F3E10;
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// bne cr6,0x826f3e4c
	if (!ctx.cr6.eq) goto loc_826F3E4C;
	// li r3,3
	ctx.r3.s64 = 3;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F3E10:
	// li r3,2
	ctx.r3.s64 = 2;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F3E24:
	// li r3,1
	ctx.r3.s64 = 1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F3E38:
	// addis r11,r3,-8192
	ctx.r11.s64 = ctx.r3.s64 + -536870912;
	// addic. r11,r11,-13
	ctx.xer.ca = ctx.r11.u32 > 12;
	ctx.r11.s64 = ctx.r11.s64 + -13;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f3e74
	if (ctx.cr0.eq) goto loc_826F3E74;
	// cmplwi cr6,r11,42
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 42, ctx.xer);
	// beq cr6,0x826f3e60
	if (ctx.cr6.eq) goto loc_826F3E60;
loc_826F3E4C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F3E60:
	// li r3,4
	ctx.r3.s64 = 4;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F3E74:
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

PPC_FUNC_IMPL(__imp__sub_826F3E88);  /* forward decl */
PPC_WEAK_FUNC(sub_826F3E88) { __imp__sub_826F3E88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F3E88) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f3cf0
	sub_826F3CF0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F3E90);  /* forward decl */
PPC_WEAK_FUNC(sub_826F3E90) { __imp__sub_826F3E90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F3E90) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eaf0
	ctx.lr = 0x826F3E98;
	__savegprlr_18(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r22,0
	ctx.r22.s64 = 0;
	// mr r23,r6
	ctx.r23.u64 = ctx.r6.u64;
	// mr r24,r7
	ctx.r24.u64 = ctx.r7.u64;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// li r18,1
	ctx.r18.s64 = 1;
	// mr r21,r5
	ctx.r21.u64 = ctx.r5.u64;
	// std r22,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r22.u64);
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// stb r22,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r22.u8);
	// mr r31,r18
	ctx.r31.u64 = ctx.r18.u64;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r28,r4
	ctx.r28.u64 = ctx.r4.u64;
	// add r20,r4,r9
	ctx.r20.u64 = ctx.r4.u64 + ctx.r9.u64;
	// lwz r11,44(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F3EDC;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r19,r3
	ctx.r19.u64 = ctx.r3.u64;
	// mr r27,r22
	ctx.r27.u64 = ctx.r22.u64;
loc_826F3EE4:
	// clrlwi r11,r31,24
	ctx.r11.u64 = ctx.r31.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826f3fec
	if (ctx.cr6.eq) goto loc_826F3FEC;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// lwz r11,48(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 48);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F3F08;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// bl 0x82977d18
	ctx.lr = 0x826F3F18;
	sub_82977D18(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826f3fe0
	if (!ctx.cr6.eq) goto loc_826F3FE0;
	// cmplwi cr6,r27,4
	ctx.cr6.compare<uint32_t>(ctx.r27.u32, 4, ctx.xer);
	// bgt cr6,0x826f3f68
	if (ctx.cr6.gt) goto loc_826F3F68;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,16196
	ctx.r12.s64 = ctx.r12.s64 + 16196;
	// rlwinm r0,r27,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r27.u32 | (ctx.r27.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r27.u32) {
	case 0:
		goto loc_826F3F58;
	case 1:
		goto loc_826F3F60;
	case 2:
		goto loc_826F3F60;
	case 3:
		goto loc_826F3F60;
	case 4:
		goto loc_826F3F58;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F3F58:
	// li r6,8
	ctx.r6.s64 = 8;
	// b 0x826f3f6c
	goto loc_826F3F6C;
loc_826F3F60:
	// li r6,4
	ctx.r6.s64 = 4;
	// b 0x826f3f6c
	goto loc_826F3F6C;
loc_826F3F68:
	// mr r6,r22
	ctx.r6.u64 = ctx.r22.u64;
loc_826F3F6C:
	// cmplw cr6,r19,r27
	ctx.cr6.compare<uint32_t>(ctx.r19.u32, ctx.r27.u32, ctx.xer);
	// bne cr6,0x826f3f94
	if (!ctx.cr6.eq) goto loc_826F3F94;
	// li r6,8
	ctx.r6.s64 = 8;
	// mr r5,r21
	ctx.r5.u64 = ctx.r21.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F3F88;
	sub_82978530(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// stb r31,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r31.u8);
	// b 0x826f3fe0
	goto loc_826F3FE0;
loc_826F3F94:
	// add r30,r6,r28
	ctx.r30.u64 = ctx.r6.u64 + ctx.r28.u64;
	// cmplw cr6,r30,r20
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, ctx.r20.u32, ctx.xer);
	// bgt cr6,0x826f3fc0
	if (ctx.cr6.gt) goto loc_826F3FC0;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F3FB0;
	sub_82978530(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// mr r11,r18
	ctx.r11.u64 = ctx.r18.u64;
	// bne cr6,0x826f3fc4
	if (!ctx.cr6.eq) goto loc_826F3FC4;
loc_826F3FC0:
	// mr r11,r22
	ctx.r11.u64 = ctx.r22.u64;
loc_826F3FC4:
	// clrlwi r31,r11,24
	ctx.r31.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x826f3fdc
	if (ctx.cr6.eq) goto loc_826F3FDC;
	// extsw r11,r27
	ctx.r11.s64 = ctx.r27.s32;
	// sld r11,r18,r11
	ctx.r11.u64 = ctx.r11.u8 & 0x40 ? 0 : (ctx.r18.u64 << (ctx.r11.u8 & 0x7F));
	// std r11,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r11.u64);
loc_826F3FDC:
	// mr r28,r30
	ctx.r28.u64 = ctx.r30.u64;
loc_826F3FE0:
	// addi r27,r27,1
	ctx.r27.s64 = ctx.r27.s64 + 1;
	// cmpwi cr6,r27,5
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 5, ctx.xer);
	// blt cr6,0x826f3ee4
	if (ctx.cr6.lt) goto loc_826F3EE4;
loc_826F3FEC:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x8298eb40
	__restgprlr_18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F3FF8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F3FF8) { __imp__sub_826F3FF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F3FF8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,39
	ctx.r11.u64 = ctx.r11.u64 | 39;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f40c0
	if (ctx.cr6.gt) goto loc_826F40C0;
	// beq cr6,0x826f40b8
	if (ctx.cr6.eq) goto loc_826F40B8;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addi r11,r11,-14
	ctx.r11.s64 = ctx.r11.s64 + -14;
	// cmplwi cr6,r11,24
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 24, ctx.xer);
	// bgt cr6,0x826f40e4
	if (ctx.cr6.gt) goto loc_826F40E4;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,16436
	ctx.r12.s64 = ctx.r12.s64 + 16436;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F40A0;
	case 1:
		goto loc_826F40B0;
	case 2:
		goto loc_826F40E4;
	case 3:
		goto loc_826F40E4;
	case 4:
		goto loc_826F40A8;
	case 5:
		goto loc_826F40E4;
	case 6:
		goto loc_826F40E4;
	case 7:
		goto loc_826F40E4;
	case 8:
		goto loc_826F40E4;
	case 9:
		goto loc_826F40E4;
	case 10:
		goto loc_826F40E4;
	case 11:
		goto loc_826F40E4;
	case 12:
		goto loc_826F40E4;
	case 13:
		goto loc_826F40E4;
	case 14:
		goto loc_826F40E4;
	case 15:
		goto loc_826F40E4;
	case 16:
		goto loc_826F40E4;
	case 17:
		goto loc_826F40E4;
	case 18:
		goto loc_826F40E4;
	case 19:
		goto loc_826F40E4;
	case 20:
		goto loc_826F40E4;
	case 21:
		goto loc_826F40E4;
	case 22:
		goto loc_826F40E4;
	case 23:
		goto loc_826F40E4;
	case 24:
		goto loc_826F4098;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F4098:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826F40A0:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826F40A8:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826F40B0:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826F40B8:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826F40C0:
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,41
	ctx.r11.u64 = ctx.r11.u64 | 41;
	// cmplw cr6,r3,r11
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, ctx.r11.u32, ctx.xer);
	// beq cr6,0x826f40fc
	if (ctx.cr6.eq) goto loc_826F40FC;
	// addis r11,r3,-8192
	ctx.r11.s64 = ctx.r3.s64 + -536870912;
	// addic. r11,r11,-13
	ctx.xer.ca = ctx.r11.u32 > 12;
	ctx.r11.s64 = ctx.r11.s64 + -13;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f40f4
	if (ctx.cr0.eq) goto loc_826F40F4;
	// cmplwi cr6,r11,42
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 42, ctx.xer);
	// beq cr6,0x826f40ec
	if (ctx.cr6.eq) goto loc_826F40EC;
loc_826F40E4:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F40EC:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826F40F4:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826F40FC:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F4108);  /* forward decl */
PPC_WEAK_FUNC(sub_826F4108) { __imp__sub_826F4108(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F4108) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bl 0x826e90e0
	ctx.lr = 0x826F411C;
	sub_826E90E0(ctx, base);
	// bl 0x826f4458
	ctx.lr = 0x826F4120;
	sub_826F4458(ctx, base);
	// bl 0x826f3ff8
	ctx.lr = 0x826F4124;
	sub_826F3FF8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F4138);  /* forward decl */
PPC_WEAK_FUNC(sub_826F4138) { __imp__sub_826F4138(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F4138) {
	PPC_FUNC_PROLOGUE();
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// ori r11,r11,13
	ctx.r11.u64 = ctx.r11.u64 | 13;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f4188
	if (ctx.cr6.gt) goto loc_826F4188;
	// beq cr6,0x826f4180
	if (ctx.cr6.eq) goto loc_826F4180;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addic. r11,r11,-14
	ctx.xer.ca = ctx.r11.u32 > 13;
	ctx.r11.s64 = ctx.r11.s64 + -14;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f4178
	if (ctx.cr0.eq) goto loc_826F4178;
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// beq cr6,0x826f4170
	if (ctx.cr6.eq) goto loc_826F4170;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bne cr6,0x826f419c
	if (!ctx.cr6.eq) goto loc_826F419C;
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826F4170:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826F4178:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826F4180:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826F4188:
	// addis r11,r3,-8192
	ctx.r11.s64 = ctx.r3.s64 + -536870912;
	// addic. r11,r11,-42
	ctx.xer.ca = ctx.r11.u32 > 41;
	ctx.r11.s64 = ctx.r11.s64 + -42;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f41ac
	if (ctx.cr0.eq) goto loc_826F41AC;
	// cmplwi cr6,r11,13
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 13, ctx.xer);
	// beq cr6,0x826f41a4
	if (ctx.cr6.eq) goto loc_826F41A4;
loc_826F419C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F41A4:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826F41AC:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F41B8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F41B8) { __imp__sub_826F41B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F41B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f4254
	if (ctx.cr6.gt) goto loc_826F4254;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f4248
	if (ctx.cr6.eq) goto loc_826F4248;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x826f4254
	if (ctx.cr6.gt) goto loc_826F4254;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,16884
	ctx.r12.s64 = ctx.r12.s64 + 16884;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F4224;
	case 1:
		goto loc_826F4218;
	case 2:
		goto loc_826F423C;
	case 3:
		goto loc_826F4230;
	case 4:
		goto loc_826F4254;
	case 5:
		goto loc_826F420C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F420C:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,42
	ctx.r3.u64 = ctx.r3.u64 | 42;
	// blr 
	return;
loc_826F4218:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F4224:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	// blr 
	return;
loc_826F4230:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,18
	ctx.r3.u64 = ctx.r3.u64 | 18;
	// blr 
	return;
loc_826F423C:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,15
	ctx.r3.u64 = ctx.r3.u64 | 15;
	// blr 
	return;
loc_826F4248:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,58
	ctx.r3.u64 = ctx.r3.u64 | 58;
	// blr 
	return;
loc_826F4254:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F4260);  /* forward decl */
PPC_WEAK_FUNC(sub_826F4260) { __imp__sub_826F4260(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F4260) {
	PPC_FUNC_PROLOGUE();
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// ori r11,r11,13
	ctx.r11.u64 = ctx.r11.u64 | 13;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f42b0
	if (ctx.cr6.gt) goto loc_826F42B0;
	// beq cr6,0x826f42a8
	if (ctx.cr6.eq) goto loc_826F42A8;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addic. r11,r11,-14
	ctx.xer.ca = ctx.r11.u32 > 13;
	ctx.r11.s64 = ctx.r11.s64 + -14;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f42a0
	if (ctx.cr0.eq) goto loc_826F42A0;
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// beq cr6,0x826f4298
	if (ctx.cr6.eq) goto loc_826F4298;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bne cr6,0x826f42c4
	if (!ctx.cr6.eq) goto loc_826F42C4;
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826F4298:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826F42A0:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826F42A8:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826F42B0:
	// addis r11,r3,-8192
	ctx.r11.s64 = ctx.r3.s64 + -536870912;
	// addic. r11,r11,-42
	ctx.xer.ca = ctx.r11.u32 > 41;
	ctx.r11.s64 = ctx.r11.s64 + -42;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f42d4
	if (ctx.cr0.eq) goto loc_826F42D4;
	// cmplwi cr6,r11,16
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 16, ctx.xer);
	// beq cr6,0x826f42cc
	if (ctx.cr6.eq) goto loc_826F42CC;
loc_826F42C4:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F42CC:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826F42D4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F42E0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F42E0) { __imp__sub_826F42E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F42E0) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f41b8
	sub_826F41B8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F42E8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F42E8) { __imp__sub_826F42E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F42E8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eaf0
	ctx.lr = 0x826F42F0;
	__savegprlr_18(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r22,0
	ctx.r22.s64 = 0;
	// mr r23,r6
	ctx.r23.u64 = ctx.r6.u64;
	// mr r24,r7
	ctx.r24.u64 = ctx.r7.u64;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// li r18,1
	ctx.r18.s64 = 1;
	// mr r21,r5
	ctx.r21.u64 = ctx.r5.u64;
	// std r22,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r22.u64);
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// stb r22,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r22.u8);
	// mr r31,r18
	ctx.r31.u64 = ctx.r18.u64;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r28,r4
	ctx.r28.u64 = ctx.r4.u64;
	// add r20,r4,r9
	ctx.r20.u64 = ctx.r4.u64 + ctx.r9.u64;
	// lwz r11,44(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F4334;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r19,r3
	ctx.r19.u64 = ctx.r3.u64;
	// mr r27,r22
	ctx.r27.u64 = ctx.r22.u64;
loc_826F433C:
	// clrlwi r11,r31,24
	ctx.r11.u64 = ctx.r31.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826f4448
	if (ctx.cr6.eq) goto loc_826F4448;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// lwz r11,48(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 48);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F4360;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// bl 0x82977d18
	ctx.lr = 0x826F4370;
	sub_82977D18(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826f443c
	if (!ctx.cr6.eq) goto loc_826F443C;
	// cmplwi cr6,r27,5
	ctx.cr6.compare<uint32_t>(ctx.r27.u32, 5, ctx.xer);
	// bgt cr6,0x826f43c4
	if (ctx.cr6.gt) goto loc_826F43C4;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,17308
	ctx.r12.s64 = ctx.r12.s64 + 17308;
	// rlwinm r0,r27,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r27.u32 | (ctx.r27.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r27.u32) {
	case 0:
		goto loc_826F43B4;
	case 1:
		goto loc_826F43B4;
	case 2:
		goto loc_826F43BC;
	case 3:
		goto loc_826F43BC;
	case 4:
		goto loc_826F43BC;
	case 5:
		goto loc_826F43B4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F43B4:
	// li r6,8
	ctx.r6.s64 = 8;
	// b 0x826f43c8
	goto loc_826F43C8;
loc_826F43BC:
	// li r6,4
	ctx.r6.s64 = 4;
	// b 0x826f43c8
	goto loc_826F43C8;
loc_826F43C4:
	// mr r6,r22
	ctx.r6.u64 = ctx.r22.u64;
loc_826F43C8:
	// cmplw cr6,r19,r27
	ctx.cr6.compare<uint32_t>(ctx.r19.u32, ctx.r27.u32, ctx.xer);
	// bne cr6,0x826f43f0
	if (!ctx.cr6.eq) goto loc_826F43F0;
	// li r6,8
	ctx.r6.s64 = 8;
	// mr r5,r21
	ctx.r5.u64 = ctx.r21.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F43E4;
	sub_82978530(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// stb r31,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r31.u8);
	// b 0x826f443c
	goto loc_826F443C;
loc_826F43F0:
	// add r30,r6,r28
	ctx.r30.u64 = ctx.r6.u64 + ctx.r28.u64;
	// cmplw cr6,r30,r20
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, ctx.r20.u32, ctx.xer);
	// bgt cr6,0x826f441c
	if (ctx.cr6.gt) goto loc_826F441C;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F440C;
	sub_82978530(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// mr r11,r18
	ctx.r11.u64 = ctx.r18.u64;
	// bne cr6,0x826f4420
	if (!ctx.cr6.eq) goto loc_826F4420;
loc_826F441C:
	// mr r11,r22
	ctx.r11.u64 = ctx.r22.u64;
loc_826F4420:
	// clrlwi r31,r11,24
	ctx.r31.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x826f4438
	if (ctx.cr6.eq) goto loc_826F4438;
	// extsw r11,r27
	ctx.r11.s64 = ctx.r27.s32;
	// sld r11,r18,r11
	ctx.r11.u64 = ctx.r11.u8 & 0x40 ? 0 : (ctx.r18.u64 << (ctx.r11.u8 & 0x7F));
	// std r11,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r11.u64);
loc_826F4438:
	// mr r28,r30
	ctx.r28.u64 = ctx.r30.u64;
loc_826F443C:
	// addi r27,r27,1
	ctx.r27.s64 = ctx.r27.s64 + 1;
	// cmpwi cr6,r27,6
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 6, ctx.xer);
	// blt cr6,0x826f433c
	if (ctx.cr6.lt) goto loc_826F433C;
loc_826F4448:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x8298eb40
	__restgprlr_18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F4458);  /* forward decl */
PPC_WEAK_FUNC(sub_826F4458) { __imp__sub_826F4458(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F4458) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f4510
	if (ctx.cr6.gt) goto loc_826F4510;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f4504
	if (ctx.cr6.eq) goto loc_826F4504;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,6
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 6, ctx.xer);
	// bgt cr6,0x826f4510
	if (ctx.cr6.gt) goto loc_826F4510;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,17556
	ctx.r12.s64 = ctx.r12.s64 + 17556;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F44E0;
	case 1:
		goto loc_826F44EC;
	case 2:
		goto loc_826F44F8;
	case 3:
		goto loc_826F44BC;
	case 4:
		goto loc_826F44B0;
	case 5:
		goto loc_826F44D4;
	case 6:
		goto loc_826F44C8;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F44B0:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,38
	ctx.r3.u64 = ctx.r3.u64 | 38;
	// blr 
	return;
loc_826F44BC:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,41
	ctx.r3.u64 = ctx.r3.u64 | 41;
	// blr 
	return;
loc_826F44C8:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,39
	ctx.r3.u64 = ctx.r3.u64 | 39;
	// blr 
	return;
loc_826F44D4:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F44E0:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	// blr 
	return;
loc_826F44EC:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,18
	ctx.r3.u64 = ctx.r3.u64 | 18;
	// blr 
	return;
loc_826F44F8:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,15
	ctx.r3.u64 = ctx.r3.u64 | 15;
	// blr 
	return;
loc_826F4504:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
loc_826F4510:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F4518);  /* forward decl */
PPC_WEAK_FUNC(sub_826F4518) { __imp__sub_826F4518(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F4518) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f4458
	sub_826F4458(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F4520);  /* forward decl */
PPC_WEAK_FUNC(sub_826F4520) { __imp__sub_826F4520(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F4520) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f45b8
	if (ctx.cr6.gt) goto loc_826F45B8;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f45ac
	if (ctx.cr6.eq) goto loc_826F45AC;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826f45b8
	if (ctx.cr6.gt) goto loc_826F45B8;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,17756
	ctx.r12.s64 = ctx.r12.s64 + 17756;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F4588;
	case 1:
		goto loc_826F4570;
	case 2:
		goto loc_826F45A0;
	case 3:
		goto loc_826F4594;
	case 4:
		goto loc_826F457C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F4570:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,42
	ctx.r3.u64 = ctx.r3.u64 | 42;
	// blr 
	return;
loc_826F457C:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F4588:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	// blr 
	return;
loc_826F4594:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,18
	ctx.r3.u64 = ctx.r3.u64 | 18;
	// blr 
	return;
loc_826F45A0:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,15
	ctx.r3.u64 = ctx.r3.u64 | 15;
	// blr 
	return;
loc_826F45AC:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
loc_826F45B8:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F45C0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F45C0) { __imp__sub_826F45C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F45C0) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f4520
	sub_826F4520(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F45C8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F45C8) { __imp__sub_826F45C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F45C8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bl 0x826e9260
	ctx.lr = 0x826F45DC;
	sub_826E9260(ctx, base);
	// bl 0x826f41b8
	ctx.lr = 0x826F45E0;
	sub_826F41B8(ctx, base);
	// bl 0x826f4260
	ctx.lr = 0x826F45E4;
	sub_826F4260(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F45F8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F45F8) { __imp__sub_826F45F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F45F8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eaf0
	ctx.lr = 0x826F4600;
	__savegprlr_18(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r22,0
	ctx.r22.s64 = 0;
	// mr r23,r6
	ctx.r23.u64 = ctx.r6.u64;
	// mr r24,r7
	ctx.r24.u64 = ctx.r7.u64;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// li r18,1
	ctx.r18.s64 = 1;
	// mr r21,r5
	ctx.r21.u64 = ctx.r5.u64;
	// std r22,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r22.u64);
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// stb r22,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r22.u8);
	// mr r31,r18
	ctx.r31.u64 = ctx.r18.u64;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r28,r4
	ctx.r28.u64 = ctx.r4.u64;
	// add r20,r4,r9
	ctx.r20.u64 = ctx.r4.u64 + ctx.r9.u64;
	// lwz r11,44(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F4644;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r19,r3
	ctx.r19.u64 = ctx.r3.u64;
	// mr r27,r22
	ctx.r27.u64 = ctx.r22.u64;
loc_826F464C:
	// clrlwi r11,r31,24
	ctx.r11.u64 = ctx.r31.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826f4760
	if (ctx.cr6.eq) goto loc_826F4760;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// lwz r11,48(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 48);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F4670;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// bl 0x82977d18
	ctx.lr = 0x826F4680;
	sub_82977D18(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826f4754
	if (!ctx.cr6.eq) goto loc_826F4754;
	// cmplwi cr6,r27,7
	ctx.cr6.compare<uint32_t>(ctx.r27.u32, 7, ctx.xer);
	// bgt cr6,0x826f46dc
	if (ctx.cr6.gt) goto loc_826F46DC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,18092
	ctx.r12.s64 = ctx.r12.s64 + 18092;
	// rlwinm r0,r27,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r27.u32 | (ctx.r27.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r27.u32) {
	case 0:
		goto loc_826F46CC;
	case 1:
		goto loc_826F46CC;
	case 2:
		goto loc_826F46CC;
	case 3:
		goto loc_826F46D4;
	case 4:
		goto loc_826F46CC;
	case 5:
		goto loc_826F46CC;
	case 6:
		goto loc_826F46CC;
	case 7:
		goto loc_826F46D4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F46CC:
	// li r6,4
	ctx.r6.s64 = 4;
	// b 0x826f46e0
	goto loc_826F46E0;
loc_826F46D4:
	// li r6,8
	ctx.r6.s64 = 8;
	// b 0x826f46e0
	goto loc_826F46E0;
loc_826F46DC:
	// mr r6,r22
	ctx.r6.u64 = ctx.r22.u64;
loc_826F46E0:
	// cmplw cr6,r19,r27
	ctx.cr6.compare<uint32_t>(ctx.r19.u32, ctx.r27.u32, ctx.xer);
	// bne cr6,0x826f4708
	if (!ctx.cr6.eq) goto loc_826F4708;
	// li r6,8
	ctx.r6.s64 = 8;
	// mr r5,r21
	ctx.r5.u64 = ctx.r21.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F46FC;
	sub_82978530(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// stb r31,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r31.u8);
	// b 0x826f4754
	goto loc_826F4754;
loc_826F4708:
	// add r30,r6,r28
	ctx.r30.u64 = ctx.r6.u64 + ctx.r28.u64;
	// cmplw cr6,r30,r20
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, ctx.r20.u32, ctx.xer);
	// bgt cr6,0x826f4734
	if (ctx.cr6.gt) goto loc_826F4734;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F4724;
	sub_82978530(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// mr r11,r18
	ctx.r11.u64 = ctx.r18.u64;
	// bne cr6,0x826f4738
	if (!ctx.cr6.eq) goto loc_826F4738;
loc_826F4734:
	// mr r11,r22
	ctx.r11.u64 = ctx.r22.u64;
loc_826F4738:
	// clrlwi r31,r11,24
	ctx.r31.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x826f4750
	if (ctx.cr6.eq) goto loc_826F4750;
	// extsw r11,r27
	ctx.r11.s64 = ctx.r27.s32;
	// sld r11,r18,r11
	ctx.r11.u64 = ctx.r11.u8 & 0x40 ? 0 : (ctx.r18.u64 << (ctx.r11.u8 & 0x7F));
	// std r11,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r11.u64);
loc_826F4750:
	// mr r28,r30
	ctx.r28.u64 = ctx.r30.u64;
loc_826F4754:
	// addi r27,r27,1
	ctx.r27.s64 = ctx.r27.s64 + 1;
	// cmpwi cr6,r27,8
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 8, ctx.xer);
	// blt cr6,0x826f464c
	if (ctx.cr6.lt) goto loc_826F464C;
loc_826F4760:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x8298eb40
	__restgprlr_18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F4770);  /* forward decl */
PPC_WEAK_FUNC(sub_826F4770) { __imp__sub_826F4770(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F4770) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bl 0x826e9670
	ctx.lr = 0x826F4784;
	sub_826E9670(ctx, base);
	// bl 0x826f4520
	ctx.lr = 0x826F4788;
	sub_826F4520(ctx, base);
	// bl 0x826f4138
	ctx.lr = 0x826F478C;
	sub_826F4138(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F47A0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F47A0) { __imp__sub_826F47A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F47A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f483c
	if (ctx.cr6.gt) goto loc_826F483C;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f4830
	if (ctx.cr6.eq) goto loc_826F4830;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x826f483c
	if (ctx.cr6.gt) goto loc_826F483C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,18396
	ctx.r12.s64 = ctx.r12.s64 + 18396;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F480C;
	case 1:
		goto loc_826F4800;
	case 2:
		goto loc_826F4818;
	case 3:
		goto loc_826F4824;
	case 4:
		goto loc_826F483C;
	case 5:
		goto loc_826F47F4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F47F4:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,42
	ctx.r3.u64 = ctx.r3.u64 | 42;
	// blr 
	return;
loc_826F4800:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F480C:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	// blr 
	return;
loc_826F4818:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,15
	ctx.r3.u64 = ctx.r3.u64 | 15;
	// blr 
	return;
loc_826F4824:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,18
	ctx.r3.u64 = ctx.r3.u64 | 18;
	// blr 
	return;
loc_826F4830:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,58
	ctx.r3.u64 = ctx.r3.u64 | 58;
	// blr 
	return;
loc_826F483C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F4848);  /* forward decl */
PPC_WEAK_FUNC(sub_826F4848) { __imp__sub_826F4848(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F4848) {
	PPC_FUNC_PROLOGUE();
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// ori r11,r11,13
	ctx.r11.u64 = ctx.r11.u64 | 13;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f4898
	if (ctx.cr6.gt) goto loc_826F4898;
	// beq cr6,0x826f4890
	if (ctx.cr6.eq) goto loc_826F4890;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addic. r11,r11,-14
	ctx.xer.ca = ctx.r11.u32 > 13;
	ctx.r11.s64 = ctx.r11.s64 + -14;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f4888
	if (ctx.cr0.eq) goto loc_826F4888;
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// beq cr6,0x826f4880
	if (ctx.cr6.eq) goto loc_826F4880;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bne cr6,0x826f48ac
	if (!ctx.cr6.eq) goto loc_826F48AC;
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826F4880:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826F4888:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826F4890:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826F4898:
	// addis r11,r3,-8192
	ctx.r11.s64 = ctx.r3.s64 + -536870912;
	// addic. r11,r11,-42
	ctx.xer.ca = ctx.r11.u32 > 41;
	ctx.r11.s64 = ctx.r11.s64 + -42;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f48bc
	if (ctx.cr0.eq) goto loc_826F48BC;
	// cmplwi cr6,r11,16
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 16, ctx.xer);
	// beq cr6,0x826f48b4
	if (ctx.cr6.eq) goto loc_826F48B4;
loc_826F48AC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F48B4:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826F48BC:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F48C8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F48C8) { __imp__sub_826F48C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F48C8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bl 0x826e9830
	ctx.lr = 0x826F48DC;
	sub_826E9830(ctx, base);
	// bl 0x826f47a0
	ctx.lr = 0x826F48E0;
	sub_826F47A0(ctx, base);
	// bl 0x826f4848
	ctx.lr = 0x826F48E4;
	sub_826F4848(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F48F8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F48F8) { __imp__sub_826F48F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F48F8) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f47a0
	sub_826F47A0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F4900);  /* forward decl */
PPC_WEAK_FUNC(sub_826F4900) { __imp__sub_826F4900(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F4900) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f49c8
	if (ctx.cr6.gt) goto loc_826F49C8;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f49bc
	if (ctx.cr6.eq) goto loc_826F49BC;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,7
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 7, ctx.xer);
	// bgt cr6,0x826f49c8
	if (ctx.cr6.gt) goto loc_826F49C8;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,18748
	ctx.r12.s64 = ctx.r12.s64 + 18748;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F4980;
	case 1:
		goto loc_826F4998;
	case 2:
		goto loc_826F49A4;
	case 3:
		goto loc_826F49B0;
	case 4:
		goto loc_826F498C;
	case 5:
		goto loc_826F495C;
	case 6:
		goto loc_826F4968;
	case 7:
		goto loc_826F4974;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F495C:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,38
	ctx.r3.u64 = ctx.r3.u64 | 38;
	// blr 
	return;
loc_826F4968:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,39
	ctx.r3.u64 = ctx.r3.u64 | 39;
	// blr 
	return;
loc_826F4974:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F4980:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,29
	ctx.r3.u64 = ctx.r3.u64 | 29;
	// blr 
	return;
loc_826F498C:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,34
	ctx.r3.u64 = ctx.r3.u64 | 34;
	// blr 
	return;
loc_826F4998:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	// blr 
	return;
loc_826F49A4:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,18
	ctx.r3.u64 = ctx.r3.u64 | 18;
	// blr 
	return;
loc_826F49B0:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,15
	ctx.r3.u64 = ctx.r3.u64 | 15;
	// blr 
	return;
loc_826F49BC:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
loc_826F49C8:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F49D0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F49D0) { __imp__sub_826F49D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F49D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,38
	ctx.r11.u64 = ctx.r11.u64 | 38;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f4a88
	if (ctx.cr6.gt) goto loc_826F4A88;
	// beq cr6,0x826f4a80
	if (ctx.cr6.eq) goto loc_826F4A80;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addi r11,r11,-14
	ctx.r11.s64 = ctx.r11.s64 + -14;
	// cmplwi cr6,r11,20
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 20, ctx.xer);
	// bgt cr6,0x826f4ae4
	if (ctx.cr6.gt) goto loc_826F4AE4;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,18956
	ctx.r12.s64 = ctx.r12.s64 + 18956;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F4A68;
	case 1:
		goto loc_826F4A78;
	case 2:
		goto loc_826F4AE4;
	case 3:
		goto loc_826F4AE4;
	case 4:
		goto loc_826F4A70;
	case 5:
		goto loc_826F4AE4;
	case 6:
		goto loc_826F4AE4;
	case 7:
		goto loc_826F4AE4;
	case 8:
		goto loc_826F4AE4;
	case 9:
		goto loc_826F4AE4;
	case 10:
		goto loc_826F4AE4;
	case 11:
		goto loc_826F4AE4;
	case 12:
		goto loc_826F4AE4;
	case 13:
		goto loc_826F4AE4;
	case 14:
		goto loc_826F4AE4;
	case 15:
		goto loc_826F4AE4;
	case 16:
		goto loc_826F4AE4;
	case 17:
		goto loc_826F4AE4;
	case 18:
		goto loc_826F4AE4;
	case 19:
		goto loc_826F4AE4;
	case 20:
		goto loc_826F4A60;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F4A60:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826F4A68:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826F4A70:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826F4A78:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826F4A80:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826F4A88:
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// ori r11,r11,29
	ctx.r11.u64 = ctx.r11.u64 | 29;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f4ad4
	if (ctx.cr6.gt) goto loc_826F4AD4;
	// beq cr6,0x826f4acc
	if (ctx.cr6.eq) goto loc_826F4ACC;
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,39
	ctx.r11.u64 = ctx.r11.u64 | 39;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// beq cr6,0x826f4ac4
	if (ctx.cr6.eq) goto loc_826F4AC4;
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// ori r11,r11,13
	ctx.r11.u64 = ctx.r11.u64 | 13;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bne cr6,0x826f4ae4
	if (!ctx.cr6.eq) goto loc_826F4AE4;
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826F4AC4:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826F4ACC:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826F4AD4:
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// ori r11,r11,55
	ctx.r11.u64 = ctx.r11.u64 | 55;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// beq cr6,0x826f4aec
	if (ctx.cr6.eq) goto loc_826F4AEC;
loc_826F4AE4:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F4AEC:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F4AF8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F4AF8) { __imp__sub_826F4AF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F4AF8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bl 0x826e9968
	ctx.lr = 0x826F4B0C;
	sub_826E9968(ctx, base);
	// bl 0x826f4900
	ctx.lr = 0x826F4B10;
	sub_826F4900(ctx, base);
	// bl 0x826f49d0
	ctx.lr = 0x826F4B14;
	sub_826F49D0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F4B28);  /* forward decl */
PPC_WEAK_FUNC(sub_826F4B28) { __imp__sub_826F4B28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F4B28) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f4900
	sub_826F4900(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F4B30);  /* forward decl */
PPC_WEAK_FUNC(sub_826F4B30) { __imp__sub_826F4B30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F4B30) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f4bd8
	if (ctx.cr6.gt) goto loc_826F4BD8;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f4bcc
	if (ctx.cr6.eq) goto loc_826F4BCC;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x826f4bd8
	if (ctx.cr6.gt) goto loc_826F4BD8;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,19308
	ctx.r12.s64 = ctx.r12.s64 + 19308;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F4B90;
	case 1:
		goto loc_826F4BA8;
	case 2:
		goto loc_826F4BB4;
	case 3:
		goto loc_826F4BC0;
	case 4:
		goto loc_826F4B9C;
	case 5:
		goto loc_826F4B84;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F4B84:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F4B90:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,29
	ctx.r3.u64 = ctx.r3.u64 | 29;
	// blr 
	return;
loc_826F4B9C:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,34
	ctx.r3.u64 = ctx.r3.u64 | 34;
	// blr 
	return;
loc_826F4BA8:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	// blr 
	return;
loc_826F4BB4:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,18
	ctx.r3.u64 = ctx.r3.u64 | 18;
	// blr 
	return;
loc_826F4BC0:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,15
	ctx.r3.u64 = ctx.r3.u64 | 15;
	// blr 
	return;
loc_826F4BCC:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
loc_826F4BD8:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F4BE0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F4BE0) { __imp__sub_826F4BE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F4BE0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,34
	ctx.r11.u64 = ctx.r11.u64 | 34;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f4c30
	if (ctx.cr6.gt) goto loc_826F4C30;
	// beq cr6,0x826f4c28
	if (ctx.cr6.eq) goto loc_826F4C28;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addic. r11,r11,-14
	ctx.xer.ca = ctx.r11.u32 > 13;
	ctx.r11.s64 = ctx.r11.s64 + -14;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f4c20
	if (ctx.cr0.eq) goto loc_826F4C20;
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// beq cr6,0x826f4c18
	if (ctx.cr6.eq) goto loc_826F4C18;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bne cr6,0x826f4c4c
	if (!ctx.cr6.eq) goto loc_826F4C4C;
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826F4C18:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826F4C20:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826F4C28:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826F4C30:
	// addis r11,r3,-8192
	ctx.r11.s64 = ctx.r3.s64 + -536870912;
	// addic. r11,r11,-13
	ctx.xer.ca = ctx.r11.u32 > 12;
	ctx.r11.s64 = ctx.r11.s64 + -13;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f4c64
	if (ctx.cr0.eq) goto loc_826F4C64;
	// cmplwi cr6,r11,16
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 16, ctx.xer);
	// beq cr6,0x826f4c5c
	if (ctx.cr6.eq) goto loc_826F4C5C;
	// cmplwi cr6,r11,42
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 42, ctx.xer);
	// beq cr6,0x826f4c54
	if (ctx.cr6.eq) goto loc_826F4C54;
loc_826F4C4C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F4C54:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826F4C5C:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826F4C64:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F4C70);  /* forward decl */
PPC_WEAK_FUNC(sub_826F4C70) { __imp__sub_826F4C70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F4C70) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bl 0x826e9ae0
	ctx.lr = 0x826F4C84;
	sub_826E9AE0(ctx, base);
	// bl 0x826f4b30
	ctx.lr = 0x826F4C88;
	sub_826F4B30(ctx, base);
	// bl 0x826f4be0
	ctx.lr = 0x826F4C8C;
	sub_826F4BE0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F4CA0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F4CA0) { __imp__sub_826F4CA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F4CA0) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f4b30
	sub_826F4B30(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F4CA8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F4CA8) { __imp__sub_826F4CA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F4CA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f4d64
	if (ctx.cr6.gt) goto loc_826F4D64;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f4d58
	if (ctx.cr6.eq) goto loc_826F4D58;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,7
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 7, ctx.xer);
	// bgt cr6,0x826f4d64
	if (ctx.cr6.gt) goto loc_826F4D64;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,19684
	ctx.r12.s64 = ctx.r12.s64 + 19684;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F4D28;
	case 1:
		goto loc_826F4D34;
	case 2:
		goto loc_826F4D40;
	case 3:
		goto loc_826F4D4C;
	case 4:
		goto loc_826F4D64;
	case 5:
		goto loc_826F4D04;
	case 6:
		goto loc_826F4D10;
	case 7:
		goto loc_826F4D1C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F4D04:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,38
	ctx.r3.u64 = ctx.r3.u64 | 38;
	// blr 
	return;
loc_826F4D10:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,39
	ctx.r3.u64 = ctx.r3.u64 | 39;
	// blr 
	return;
loc_826F4D1C:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F4D28:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,29
	ctx.r3.u64 = ctx.r3.u64 | 29;
	// blr 
	return;
loc_826F4D34:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	// blr 
	return;
loc_826F4D40:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,18
	ctx.r3.u64 = ctx.r3.u64 | 18;
	// blr 
	return;
loc_826F4D4C:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,15
	ctx.r3.u64 = ctx.r3.u64 | 15;
	// blr 
	return;
loc_826F4D58:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
loc_826F4D64:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F4D70);  /* forward decl */
PPC_WEAK_FUNC(sub_826F4D70) { __imp__sub_826F4D70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F4D70) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bl 0x826e9be0
	ctx.lr = 0x826F4D84;
	sub_826E9BE0(ctx, base);
	// bl 0x826f4ca8
	ctx.lr = 0x826F4D88;
	sub_826F4CA8(ctx, base);
	// bl 0x826f5ab8
	ctx.lr = 0x826F4D8C;
	sub_826F5AB8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F4DA0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F4DA0) { __imp__sub_826F4DA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F4DA0) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f4ca8
	sub_826F4CA8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F4DA8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F4DA8) { __imp__sub_826F4DA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F4DA8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eaf0
	ctx.lr = 0x826F4DB0;
	__savegprlr_18(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r22,0
	ctx.r22.s64 = 0;
	// mr r23,r6
	ctx.r23.u64 = ctx.r6.u64;
	// mr r24,r7
	ctx.r24.u64 = ctx.r7.u64;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// li r18,1
	ctx.r18.s64 = 1;
	// mr r21,r5
	ctx.r21.u64 = ctx.r5.u64;
	// std r22,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r22.u64);
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// stb r22,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r22.u8);
	// mr r31,r18
	ctx.r31.u64 = ctx.r18.u64;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r28,r4
	ctx.r28.u64 = ctx.r4.u64;
	// add r20,r4,r9
	ctx.r20.u64 = ctx.r4.u64 + ctx.r9.u64;
	// lwz r11,44(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F4DF4;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r19,r3
	ctx.r19.u64 = ctx.r3.u64;
	// mr r27,r22
	ctx.r27.u64 = ctx.r22.u64;
loc_826F4DFC:
	// clrlwi r11,r31,24
	ctx.r11.u64 = ctx.r31.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826f4f10
	if (ctx.cr6.eq) goto loc_826F4F10;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// lwz r11,48(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 48);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F4E20;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// bl 0x82977d18
	ctx.lr = 0x826F4E30;
	sub_82977D18(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826f4f04
	if (!ctx.cr6.eq) goto loc_826F4F04;
	// cmplwi cr6,r27,7
	ctx.cr6.compare<uint32_t>(ctx.r27.u32, 7, ctx.xer);
	// bgt cr6,0x826f4e8c
	if (ctx.cr6.gt) goto loc_826F4E8C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,20060
	ctx.r12.s64 = ctx.r12.s64 + 20060;
	// rlwinm r0,r27,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r27.u32 | (ctx.r27.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r27.u32) {
	case 0:
		goto loc_826F4E7C;
	case 1:
		goto loc_826F4E7C;
	case 2:
		goto loc_826F4E84;
	case 3:
		goto loc_826F4E84;
	case 4:
		goto loc_826F4E7C;
	case 5:
		goto loc_826F4E7C;
	case 6:
		goto loc_826F4E7C;
	case 7:
		goto loc_826F4E84;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F4E7C:
	// li r6,4
	ctx.r6.s64 = 4;
	// b 0x826f4e90
	goto loc_826F4E90;
loc_826F4E84:
	// li r6,8
	ctx.r6.s64 = 8;
	// b 0x826f4e90
	goto loc_826F4E90;
loc_826F4E8C:
	// mr r6,r22
	ctx.r6.u64 = ctx.r22.u64;
loc_826F4E90:
	// cmplw cr6,r19,r27
	ctx.cr6.compare<uint32_t>(ctx.r19.u32, ctx.r27.u32, ctx.xer);
	// bne cr6,0x826f4eb8
	if (!ctx.cr6.eq) goto loc_826F4EB8;
	// li r6,8
	ctx.r6.s64 = 8;
	// mr r5,r21
	ctx.r5.u64 = ctx.r21.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F4EAC;
	sub_82978530(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// stb r31,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r31.u8);
	// b 0x826f4f04
	goto loc_826F4F04;
loc_826F4EB8:
	// add r30,r6,r28
	ctx.r30.u64 = ctx.r6.u64 + ctx.r28.u64;
	// cmplw cr6,r30,r20
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, ctx.r20.u32, ctx.xer);
	// bgt cr6,0x826f4ee4
	if (ctx.cr6.gt) goto loc_826F4EE4;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F4ED4;
	sub_82978530(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// mr r11,r18
	ctx.r11.u64 = ctx.r18.u64;
	// bne cr6,0x826f4ee8
	if (!ctx.cr6.eq) goto loc_826F4EE8;
loc_826F4EE4:
	// mr r11,r22
	ctx.r11.u64 = ctx.r22.u64;
loc_826F4EE8:
	// clrlwi r31,r11,24
	ctx.r31.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x826f4f00
	if (ctx.cr6.eq) goto loc_826F4F00;
	// extsw r11,r27
	ctx.r11.s64 = ctx.r27.s32;
	// sld r11,r18,r11
	ctx.r11.u64 = ctx.r11.u8 & 0x40 ? 0 : (ctx.r18.u64 << (ctx.r11.u8 & 0x7F));
	// std r11,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r11.u64);
loc_826F4F00:
	// mr r28,r30
	ctx.r28.u64 = ctx.r30.u64;
loc_826F4F04:
	// addi r27,r27,1
	ctx.r27.s64 = ctx.r27.s64 + 1;
	// cmpwi cr6,r27,8
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 8, ctx.xer);
	// blt cr6,0x826f4dfc
	if (ctx.cr6.lt) goto loc_826F4DFC;
loc_826F4F10:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x8298eb40
	__restgprlr_18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F4F20);  /* forward decl */
PPC_WEAK_FUNC(sub_826F4F20) { __imp__sub_826F4F20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F4F20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f4fbc
	if (ctx.cr6.gt) goto loc_826F4FBC;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f4fb0
	if (ctx.cr6.eq) goto loc_826F4FB0;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x826f4fbc
	if (ctx.cr6.gt) goto loc_826F4FBC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,20316
	ctx.r12.s64 = ctx.r12.s64 + 20316;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F4F74;
	case 1:
		goto loc_826F4F8C;
	case 2:
		goto loc_826F4F98;
	case 3:
		goto loc_826F4FA4;
	case 4:
		goto loc_826F4FBC;
	case 5:
		goto loc_826F4F80;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F4F74:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F4F80:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,29
	ctx.r3.u64 = ctx.r3.u64 | 29;
	// blr 
	return;
loc_826F4F8C:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	// blr 
	return;
loc_826F4F98:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,18
	ctx.r3.u64 = ctx.r3.u64 | 18;
	// blr 
	return;
loc_826F4FA4:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,15
	ctx.r3.u64 = ctx.r3.u64 | 15;
	// blr 
	return;
loc_826F4FB0:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
loc_826F4FBC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F4FC8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F4FC8) { __imp__sub_826F4FC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F4FC8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bl 0x826e9e08
	ctx.lr = 0x826F4FDC;
	sub_826E9E08(ctx, base);
	// bl 0x826f4f20
	ctx.lr = 0x826F4FE0;
	sub_826F4F20(ctx, base);
	// bl 0x826f5c98
	ctx.lr = 0x826F4FE4;
	sub_826F5C98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F4FF8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F4FF8) { __imp__sub_826F4FF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F4FF8) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f4f20
	sub_826F4F20(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F5000);  /* forward decl */
PPC_WEAK_FUNC(sub_826F5000) { __imp__sub_826F5000(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F5000) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f50c8
	if (ctx.cr6.gt) goto loc_826F50C8;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f50bc
	if (ctx.cr6.eq) goto loc_826F50BC;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,7
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 7, ctx.xer);
	// bgt cr6,0x826f50c8
	if (ctx.cr6.gt) goto loc_826F50C8;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,20540
	ctx.r12.s64 = ctx.r12.s64 + 20540;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F5080;
	case 1:
		goto loc_826F5098;
	case 2:
		goto loc_826F50A4;
	case 3:
		goto loc_826F50B0;
	case 4:
		goto loc_826F508C;
	case 5:
		goto loc_826F5068;
	case 6:
		goto loc_826F505C;
	case 7:
		goto loc_826F5074;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F505C:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,38
	ctx.r3.u64 = ctx.r3.u64 | 38;
	// blr 
	return;
loc_826F5068:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,39
	ctx.r3.u64 = ctx.r3.u64 | 39;
	// blr 
	return;
loc_826F5074:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F5080:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,29
	ctx.r3.u64 = ctx.r3.u64 | 29;
	// blr 
	return;
loc_826F508C:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,17
	ctx.r3.u64 = ctx.r3.u64 | 17;
	// blr 
	return;
loc_826F5098:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	// blr 
	return;
loc_826F50A4:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,18
	ctx.r3.u64 = ctx.r3.u64 | 18;
	// blr 
	return;
loc_826F50B0:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,15
	ctx.r3.u64 = ctx.r3.u64 | 15;
	// blr 
	return;
loc_826F50BC:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
loc_826F50C8:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F50D0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F50D0) { __imp__sub_826F50D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F50D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,38
	ctx.r11.u64 = ctx.r11.u64 | 38;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f5148
	if (ctx.cr6.gt) goto loc_826F5148;
	// beq cr6,0x826f5140
	if (ctx.cr6.eq) goto loc_826F5140;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addi r11,r11,-14
	ctx.r11.s64 = ctx.r11.s64 + -14;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826f51a4
	if (ctx.cr6.gt) goto loc_826F51A4;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,20748
	ctx.r12.s64 = ctx.r12.s64 + 20748;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F5128;
	case 1:
		goto loc_826F5138;
	case 2:
		goto loc_826F51A4;
	case 3:
		goto loc_826F5120;
	case 4:
		goto loc_826F5130;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F5120:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826F5128:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826F5130:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826F5138:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826F5140:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826F5148:
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// ori r11,r11,29
	ctx.r11.u64 = ctx.r11.u64 | 29;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f5194
	if (ctx.cr6.gt) goto loc_826F5194;
	// beq cr6,0x826f518c
	if (ctx.cr6.eq) goto loc_826F518C;
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,39
	ctx.r11.u64 = ctx.r11.u64 | 39;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// beq cr6,0x826f5184
	if (ctx.cr6.eq) goto loc_826F5184;
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// ori r11,r11,13
	ctx.r11.u64 = ctx.r11.u64 | 13;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bne cr6,0x826f51a4
	if (!ctx.cr6.eq) goto loc_826F51A4;
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826F5184:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826F518C:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826F5194:
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// ori r11,r11,55
	ctx.r11.u64 = ctx.r11.u64 | 55;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// beq cr6,0x826f51ac
	if (ctx.cr6.eq) goto loc_826F51AC;
loc_826F51A4:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F51AC:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F51B8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F51B8) { __imp__sub_826F51B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F51B8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bl 0x826e9f38
	ctx.lr = 0x826F51CC;
	sub_826E9F38(ctx, base);
	// bl 0x826f5000
	ctx.lr = 0x826F51D0;
	sub_826F5000(ctx, base);
	// bl 0x826f50d0
	ctx.lr = 0x826F51D4;
	sub_826F50D0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F51E8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F51E8) { __imp__sub_826F51E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F51E8) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f5000
	sub_826F5000(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F51F0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F51F0) { __imp__sub_826F51F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F51F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f5298
	if (ctx.cr6.gt) goto loc_826F5298;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f528c
	if (ctx.cr6.eq) goto loc_826F528C;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x826f5298
	if (ctx.cr6.gt) goto loc_826F5298;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,21036
	ctx.r12.s64 = ctx.r12.s64 + 21036;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F5250;
	case 1:
		goto loc_826F5268;
	case 2:
		goto loc_826F5274;
	case 3:
		goto loc_826F5280;
	case 4:
		goto loc_826F525C;
	case 5:
		goto loc_826F5244;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F5244:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F5250:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,29
	ctx.r3.u64 = ctx.r3.u64 | 29;
	// blr 
	return;
loc_826F525C:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,17
	ctx.r3.u64 = ctx.r3.u64 | 17;
	// blr 
	return;
loc_826F5268:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	// blr 
	return;
loc_826F5274:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,18
	ctx.r3.u64 = ctx.r3.u64 | 18;
	// blr 
	return;
loc_826F5280:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,15
	ctx.r3.u64 = ctx.r3.u64 | 15;
	// blr 
	return;
loc_826F528C:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
loc_826F5298:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F52A0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F52A0) { __imp__sub_826F52A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F52A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,18
	ctx.r11.u64 = ctx.r11.u64 | 18;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f52f0
	if (ctx.cr6.gt) goto loc_826F52F0;
	// beq cr6,0x826f52e8
	if (ctx.cr6.eq) goto loc_826F52E8;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addic. r11,r11,-14
	ctx.xer.ca = ctx.r11.u32 > 13;
	ctx.r11.s64 = ctx.r11.s64 + -14;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f52e0
	if (ctx.cr0.eq) goto loc_826F52E0;
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// beq cr6,0x826f52d8
	if (ctx.cr6.eq) goto loc_826F52D8;
	// cmplwi cr6,r11,3
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 3, ctx.xer);
	// bne cr6,0x826f530c
	if (!ctx.cr6.eq) goto loc_826F530C;
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826F52D8:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826F52E0:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826F52E8:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826F52F0:
	// addis r11,r3,-8192
	ctx.r11.s64 = ctx.r3.s64 + -536870912;
	// addic. r11,r11,-13
	ctx.xer.ca = ctx.r11.u32 > 12;
	ctx.r11.s64 = ctx.r11.s64 + -13;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f5324
	if (ctx.cr0.eq) goto loc_826F5324;
	// cmplwi cr6,r11,16
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 16, ctx.xer);
	// beq cr6,0x826f531c
	if (ctx.cr6.eq) goto loc_826F531C;
	// cmplwi cr6,r11,42
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 42, ctx.xer);
	// beq cr6,0x826f5314
	if (ctx.cr6.eq) goto loc_826F5314;
loc_826F530C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F5314:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826F531C:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826F5324:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F5330);  /* forward decl */
PPC_WEAK_FUNC(sub_826F5330) { __imp__sub_826F5330(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F5330) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bl 0x826e9ae0
	ctx.lr = 0x826F5344;
	sub_826E9AE0(ctx, base);
	// bl 0x826f51f0
	ctx.lr = 0x826F5348;
	sub_826F51F0(ctx, base);
	// bl 0x826f52a0
	ctx.lr = 0x826F534C;
	sub_826F52A0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F5360);  /* forward decl */
PPC_WEAK_FUNC(sub_826F5360) { __imp__sub_826F5360(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F5360) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f51f0
	sub_826F51F0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F5368);  /* forward decl */
PPC_WEAK_FUNC(sub_826F5368) { __imp__sub_826F5368(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F5368) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f5430
	if (ctx.cr6.gt) goto loc_826F5430;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f5424
	if (ctx.cr6.eq) goto loc_826F5424;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,7
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 7, ctx.xer);
	// bgt cr6,0x826f5430
	if (ctx.cr6.gt) goto loc_826F5430;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,21412
	ctx.r12.s64 = ctx.r12.s64 + 21412;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F53E8;
	case 1:
		goto loc_826F5400;
	case 2:
		goto loc_826F540C;
	case 3:
		goto loc_826F5418;
	case 4:
		goto loc_826F53F4;
	case 5:
		goto loc_826F53C4;
	case 6:
		goto loc_826F53D0;
	case 7:
		goto loc_826F53DC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F53C4:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,38
	ctx.r3.u64 = ctx.r3.u64 | 38;
	// blr 
	return;
loc_826F53D0:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,39
	ctx.r3.u64 = ctx.r3.u64 | 39;
	// blr 
	return;
loc_826F53DC:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F53E8:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,29
	ctx.r3.u64 = ctx.r3.u64 | 29;
	// blr 
	return;
loc_826F53F4:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,33
	ctx.r3.u64 = ctx.r3.u64 | 33;
	// blr 
	return;
loc_826F5400:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	// blr 
	return;
loc_826F540C:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,18
	ctx.r3.u64 = ctx.r3.u64 | 18;
	// blr 
	return;
loc_826F5418:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,15
	ctx.r3.u64 = ctx.r3.u64 | 15;
	// blr 
	return;
loc_826F5424:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
loc_826F5430:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F5438);  /* forward decl */
PPC_WEAK_FUNC(sub_826F5438) { __imp__sub_826F5438(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F5438) {
	PPC_FUNC_PROLOGUE();
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,38
	ctx.r11.u64 = ctx.r11.u64 | 38;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f54ec
	if (ctx.cr6.gt) goto loc_826F54EC;
	// beq cr6,0x826f54e4
	if (ctx.cr6.eq) goto loc_826F54E4;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addi r11,r11,-14
	ctx.r11.s64 = ctx.r11.s64 + -14;
	// cmplwi cr6,r11,19
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 19, ctx.xer);
	// bgt cr6,0x826f5548
	if (ctx.cr6.gt) goto loc_826F5548;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,21620
	ctx.r12.s64 = ctx.r12.s64 + 21620;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F54CC;
	case 1:
		goto loc_826F54DC;
	case 2:
		goto loc_826F5548;
	case 3:
		goto loc_826F5548;
	case 4:
		goto loc_826F54D4;
	case 5:
		goto loc_826F5548;
	case 6:
		goto loc_826F5548;
	case 7:
		goto loc_826F5548;
	case 8:
		goto loc_826F5548;
	case 9:
		goto loc_826F5548;
	case 10:
		goto loc_826F5548;
	case 11:
		goto loc_826F5548;
	case 12:
		goto loc_826F5548;
	case 13:
		goto loc_826F5548;
	case 14:
		goto loc_826F5548;
	case 15:
		goto loc_826F5548;
	case 16:
		goto loc_826F5548;
	case 17:
		goto loc_826F5548;
	case 18:
		goto loc_826F5548;
	case 19:
		goto loc_826F54C4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F54C4:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826F54CC:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826F54D4:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826F54DC:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826F54E4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826F54EC:
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// ori r11,r11,29
	ctx.r11.u64 = ctx.r11.u64 | 29;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f5538
	if (ctx.cr6.gt) goto loc_826F5538;
	// beq cr6,0x826f5530
	if (ctx.cr6.eq) goto loc_826F5530;
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,39
	ctx.r11.u64 = ctx.r11.u64 | 39;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// beq cr6,0x826f5528
	if (ctx.cr6.eq) goto loc_826F5528;
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// ori r11,r11,13
	ctx.r11.u64 = ctx.r11.u64 | 13;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bne cr6,0x826f5548
	if (!ctx.cr6.eq) goto loc_826F5548;
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826F5528:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826F5530:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826F5538:
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// ori r11,r11,55
	ctx.r11.u64 = ctx.r11.u64 | 55;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// beq cr6,0x826f5550
	if (ctx.cr6.eq) goto loc_826F5550;
loc_826F5548:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F5550:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F5558);  /* forward decl */
PPC_WEAK_FUNC(sub_826F5558) { __imp__sub_826F5558(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F5558) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bl 0x826e9968
	ctx.lr = 0x826F556C;
	sub_826E9968(ctx, base);
	// bl 0x826f5368
	ctx.lr = 0x826F5570;
	sub_826F5368(ctx, base);
	// bl 0x826f5438
	ctx.lr = 0x826F5574;
	sub_826F5438(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F5588);  /* forward decl */
PPC_WEAK_FUNC(sub_826F5588) { __imp__sub_826F5588(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F5588) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f5368
	sub_826F5368(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F5590);  /* forward decl */
PPC_WEAK_FUNC(sub_826F5590) { __imp__sub_826F5590(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F5590) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eaf0
	ctx.lr = 0x826F5598;
	__savegprlr_18(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r22,0
	ctx.r22.s64 = 0;
	// mr r23,r6
	ctx.r23.u64 = ctx.r6.u64;
	// mr r24,r7
	ctx.r24.u64 = ctx.r7.u64;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// li r18,1
	ctx.r18.s64 = 1;
	// mr r21,r5
	ctx.r21.u64 = ctx.r5.u64;
	// std r22,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r22.u64);
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// stb r22,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r22.u8);
	// mr r31,r18
	ctx.r31.u64 = ctx.r18.u64;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r28,r4
	ctx.r28.u64 = ctx.r4.u64;
	// add r20,r4,r9
	ctx.r20.u64 = ctx.r4.u64 + ctx.r9.u64;
	// lwz r11,44(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F55DC;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r19,r3
	ctx.r19.u64 = ctx.r3.u64;
	// mr r27,r22
	ctx.r27.u64 = ctx.r22.u64;
loc_826F55E4:
	// clrlwi r11,r31,24
	ctx.r11.u64 = ctx.r31.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826f56fc
	if (ctx.cr6.eq) goto loc_826F56FC;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// lwz r11,48(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 48);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F5608;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// bl 0x82977d18
	ctx.lr = 0x826F5618;
	sub_82977D18(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826f56f0
	if (!ctx.cr6.eq) goto loc_826F56F0;
	// cmplwi cr6,r27,8
	ctx.cr6.compare<uint32_t>(ctx.r27.u32, 8, ctx.xer);
	// bgt cr6,0x826f5678
	if (ctx.cr6.gt) goto loc_826F5678;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,22084
	ctx.r12.s64 = ctx.r12.s64 + 22084;
	// rlwinm r0,r27,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r27.u32 | (ctx.r27.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r27.u32) {
	case 0:
		goto loc_826F5668;
	case 1:
		goto loc_826F5668;
	case 2:
		goto loc_826F5670;
	case 3:
		goto loc_826F5670;
	case 4:
		goto loc_826F5668;
	case 5:
		goto loc_826F5668;
	case 6:
		goto loc_826F5668;
	case 7:
		goto loc_826F5668;
	case 8:
		goto loc_826F5670;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F5668:
	// li r6,4
	ctx.r6.s64 = 4;
	// b 0x826f567c
	goto loc_826F567C;
loc_826F5670:
	// li r6,8
	ctx.r6.s64 = 8;
	// b 0x826f567c
	goto loc_826F567C;
loc_826F5678:
	// mr r6,r22
	ctx.r6.u64 = ctx.r22.u64;
loc_826F567C:
	// cmplw cr6,r19,r27
	ctx.cr6.compare<uint32_t>(ctx.r19.u32, ctx.r27.u32, ctx.xer);
	// bne cr6,0x826f56a4
	if (!ctx.cr6.eq) goto loc_826F56A4;
	// li r6,8
	ctx.r6.s64 = 8;
	// mr r5,r21
	ctx.r5.u64 = ctx.r21.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F5698;
	sub_82978530(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// stb r31,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r31.u8);
	// b 0x826f56f0
	goto loc_826F56F0;
loc_826F56A4:
	// add r30,r6,r28
	ctx.r30.u64 = ctx.r6.u64 + ctx.r28.u64;
	// cmplw cr6,r30,r20
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, ctx.r20.u32, ctx.xer);
	// bgt cr6,0x826f56d0
	if (ctx.cr6.gt) goto loc_826F56D0;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F56C0;
	sub_82978530(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// mr r11,r18
	ctx.r11.u64 = ctx.r18.u64;
	// bne cr6,0x826f56d4
	if (!ctx.cr6.eq) goto loc_826F56D4;
loc_826F56D0:
	// mr r11,r22
	ctx.r11.u64 = ctx.r22.u64;
loc_826F56D4:
	// clrlwi r31,r11,24
	ctx.r31.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x826f56ec
	if (ctx.cr6.eq) goto loc_826F56EC;
	// extsw r11,r27
	ctx.r11.s64 = ctx.r27.s32;
	// sld r11,r18,r11
	ctx.r11.u64 = ctx.r11.u8 & 0x40 ? 0 : (ctx.r18.u64 << (ctx.r11.u8 & 0x7F));
	// std r11,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r11.u64);
loc_826F56EC:
	// mr r28,r30
	ctx.r28.u64 = ctx.r30.u64;
loc_826F56F0:
	// addi r27,r27,1
	ctx.r27.s64 = ctx.r27.s64 + 1;
	// cmpwi cr6,r27,9
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 9, ctx.xer);
	// blt cr6,0x826f55e4
	if (ctx.cr6.lt) goto loc_826F55E4;
loc_826F56FC:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x8298eb40
	__restgprlr_18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F5708);  /* forward decl */
PPC_WEAK_FUNC(sub_826F5708) { __imp__sub_826F5708(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F5708) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f57b0
	if (ctx.cr6.gt) goto loc_826F57B0;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f57a4
	if (ctx.cr6.eq) goto loc_826F57A4;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x826f57b0
	if (ctx.cr6.gt) goto loc_826F57B0;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,22340
	ctx.r12.s64 = ctx.r12.s64 + 22340;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F5768;
	case 1:
		goto loc_826F5780;
	case 2:
		goto loc_826F578C;
	case 3:
		goto loc_826F5798;
	case 4:
		goto loc_826F5774;
	case 5:
		goto loc_826F575C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F575C:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F5768:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,29
	ctx.r3.u64 = ctx.r3.u64 | 29;
	// blr 
	return;
loc_826F5774:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,33
	ctx.r3.u64 = ctx.r3.u64 | 33;
	// blr 
	return;
loc_826F5780:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	// blr 
	return;
loc_826F578C:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,18
	ctx.r3.u64 = ctx.r3.u64 | 18;
	// blr 
	return;
loc_826F5798:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,15
	ctx.r3.u64 = ctx.r3.u64 | 15;
	// blr 
	return;
loc_826F57A4:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
loc_826F57B0:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F57B8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F57B8) { __imp__sub_826F57B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F57B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,33
	ctx.r11.u64 = ctx.r11.u64 | 33;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f5808
	if (ctx.cr6.gt) goto loc_826F5808;
	// beq cr6,0x826f5800
	if (ctx.cr6.eq) goto loc_826F5800;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addic. r11,r11,-14
	ctx.xer.ca = ctx.r11.u32 > 13;
	ctx.r11.s64 = ctx.r11.s64 + -14;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f57f8
	if (ctx.cr0.eq) goto loc_826F57F8;
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// beq cr6,0x826f57f0
	if (ctx.cr6.eq) goto loc_826F57F0;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bne cr6,0x826f5824
	if (!ctx.cr6.eq) goto loc_826F5824;
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826F57F0:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826F57F8:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826F5800:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826F5808:
	// addis r11,r3,-8192
	ctx.r11.s64 = ctx.r3.s64 + -536870912;
	// addic. r11,r11,-13
	ctx.xer.ca = ctx.r11.u32 > 12;
	ctx.r11.s64 = ctx.r11.s64 + -13;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f583c
	if (ctx.cr0.eq) goto loc_826F583C;
	// cmplwi cr6,r11,16
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 16, ctx.xer);
	// beq cr6,0x826f5834
	if (ctx.cr6.eq) goto loc_826F5834;
	// cmplwi cr6,r11,42
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 42, ctx.xer);
	// beq cr6,0x826f582c
	if (ctx.cr6.eq) goto loc_826F582C;
loc_826F5824:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F582C:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826F5834:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826F583C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F5848);  /* forward decl */
PPC_WEAK_FUNC(sub_826F5848) { __imp__sub_826F5848(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F5848) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bl 0x826e9ae0
	ctx.lr = 0x826F585C;
	sub_826E9AE0(ctx, base);
	// bl 0x826f5708
	ctx.lr = 0x826F5860;
	sub_826F5708(ctx, base);
	// bl 0x826f57b8
	ctx.lr = 0x826F5864;
	sub_826F57B8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F5878);  /* forward decl */
PPC_WEAK_FUNC(sub_826F5878) { __imp__sub_826F5878(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F5878) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f5708
	sub_826F5708(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F5880);  /* forward decl */
PPC_WEAK_FUNC(sub_826F5880) { __imp__sub_826F5880(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F5880) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eaf0
	ctx.lr = 0x826F5888;
	__savegprlr_18(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r22,0
	ctx.r22.s64 = 0;
	// mr r23,r6
	ctx.r23.u64 = ctx.r6.u64;
	// mr r24,r7
	ctx.r24.u64 = ctx.r7.u64;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// li r18,1
	ctx.r18.s64 = 1;
	// mr r21,r5
	ctx.r21.u64 = ctx.r5.u64;
	// std r22,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r22.u64);
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// stb r22,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r22.u8);
	// mr r31,r18
	ctx.r31.u64 = ctx.r18.u64;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r28,r4
	ctx.r28.u64 = ctx.r4.u64;
	// add r20,r4,r9
	ctx.r20.u64 = ctx.r4.u64 + ctx.r9.u64;
	// lwz r11,44(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F58CC;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r19,r3
	ctx.r19.u64 = ctx.r3.u64;
	// mr r27,r22
	ctx.r27.u64 = ctx.r22.u64;
loc_826F58D4:
	// clrlwi r11,r31,24
	ctx.r11.u64 = ctx.r31.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826f59e4
	if (ctx.cr6.eq) goto loc_826F59E4;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// lwz r11,48(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 48);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F58F8;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// bl 0x82977d18
	ctx.lr = 0x826F5908;
	sub_82977D18(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826f59d8
	if (!ctx.cr6.eq) goto loc_826F59D8;
	// cmplwi cr6,r27,6
	ctx.cr6.compare<uint32_t>(ctx.r27.u32, 6, ctx.xer);
	// bgt cr6,0x826f5960
	if (ctx.cr6.gt) goto loc_826F5960;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,22836
	ctx.r12.s64 = ctx.r12.s64 + 22836;
	// rlwinm r0,r27,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r27.u32 | (ctx.r27.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r27.u32) {
	case 0:
		goto loc_826F5950;
	case 1:
		goto loc_826F5950;
	case 2:
		goto loc_826F5958;
	case 3:
		goto loc_826F5958;
	case 4:
		goto loc_826F5958;
	case 5:
		goto loc_826F5958;
	case 6:
		goto loc_826F5950;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F5950:
	// li r6,8
	ctx.r6.s64 = 8;
	// b 0x826f5964
	goto loc_826F5964;
loc_826F5958:
	// li r6,4
	ctx.r6.s64 = 4;
	// b 0x826f5964
	goto loc_826F5964;
loc_826F5960:
	// mr r6,r22
	ctx.r6.u64 = ctx.r22.u64;
loc_826F5964:
	// cmplw cr6,r19,r27
	ctx.cr6.compare<uint32_t>(ctx.r19.u32, ctx.r27.u32, ctx.xer);
	// bne cr6,0x826f598c
	if (!ctx.cr6.eq) goto loc_826F598C;
	// li r6,8
	ctx.r6.s64 = 8;
	// mr r5,r21
	ctx.r5.u64 = ctx.r21.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F5980;
	sub_82978530(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// stb r31,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r31.u8);
	// b 0x826f59d8
	goto loc_826F59D8;
loc_826F598C:
	// add r30,r6,r28
	ctx.r30.u64 = ctx.r6.u64 + ctx.r28.u64;
	// cmplw cr6,r30,r20
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, ctx.r20.u32, ctx.xer);
	// bgt cr6,0x826f59b8
	if (ctx.cr6.gt) goto loc_826F59B8;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F59A8;
	sub_82978530(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// mr r11,r18
	ctx.r11.u64 = ctx.r18.u64;
	// bne cr6,0x826f59bc
	if (!ctx.cr6.eq) goto loc_826F59BC;
loc_826F59B8:
	// mr r11,r22
	ctx.r11.u64 = ctx.r22.u64;
loc_826F59BC:
	// clrlwi r31,r11,24
	ctx.r31.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x826f59d4
	if (ctx.cr6.eq) goto loc_826F59D4;
	// extsw r11,r27
	ctx.r11.s64 = ctx.r27.s32;
	// sld r11,r18,r11
	ctx.r11.u64 = ctx.r11.u8 & 0x40 ? 0 : (ctx.r18.u64 << (ctx.r11.u8 & 0x7F));
	// std r11,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r11.u64);
loc_826F59D4:
	// mr r28,r30
	ctx.r28.u64 = ctx.r30.u64;
loc_826F59D8:
	// addi r27,r27,1
	ctx.r27.s64 = ctx.r27.s64 + 1;
	// cmpwi cr6,r27,7
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 7, ctx.xer);
	// blt cr6,0x826f58d4
	if (ctx.cr6.lt) goto loc_826F58D4;
loc_826F59E4:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x8298eb40
	__restgprlr_18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F59F0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F59F0) { __imp__sub_826F59F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F59F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f5aac
	if (ctx.cr6.gt) goto loc_826F5AAC;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f5aa0
	if (ctx.cr6.eq) goto loc_826F5AA0;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,7
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 7, ctx.xer);
	// bgt cr6,0x826f5aac
	if (ctx.cr6.gt) goto loc_826F5AAC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,23084
	ctx.r12.s64 = ctx.r12.s64 + 23084;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F5A70;
	case 1:
		goto loc_826F5A7C;
	case 2:
		goto loc_826F5A88;
	case 3:
		goto loc_826F5A94;
	case 4:
		goto loc_826F5A4C;
	case 5:
		goto loc_826F5A58;
	case 6:
		goto loc_826F5AAC;
	case 7:
		goto loc_826F5A64;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F5A4C:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,38
	ctx.r3.u64 = ctx.r3.u64 | 38;
	// blr 
	return;
loc_826F5A58:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,39
	ctx.r3.u64 = ctx.r3.u64 | 39;
	// blr 
	return;
loc_826F5A64:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F5A70:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,29
	ctx.r3.u64 = ctx.r3.u64 | 29;
	// blr 
	return;
loc_826F5A7C:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	// blr 
	return;
loc_826F5A88:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,18
	ctx.r3.u64 = ctx.r3.u64 | 18;
	// blr 
	return;
loc_826F5A94:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,15
	ctx.r3.u64 = ctx.r3.u64 | 15;
	// blr 
	return;
loc_826F5AA0:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
loc_826F5AAC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F5AB8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F5AB8) { __imp__sub_826F5AB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F5AB8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,39
	ctx.r11.u64 = ctx.r11.u64 | 39;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f5b80
	if (ctx.cr6.gt) goto loc_826F5B80;
	// beq cr6,0x826f5b78
	if (ctx.cr6.eq) goto loc_826F5B78;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addi r11,r11,-14
	ctx.r11.s64 = ctx.r11.s64 + -14;
	// cmplwi cr6,r11,24
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 24, ctx.xer);
	// bgt cr6,0x826f5b9c
	if (ctx.cr6.gt) goto loc_826F5B9C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,23284
	ctx.r12.s64 = ctx.r12.s64 + 23284;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F5B60;
	case 1:
		goto loc_826F5B70;
	case 2:
		goto loc_826F5B9C;
	case 3:
		goto loc_826F5B9C;
	case 4:
		goto loc_826F5B68;
	case 5:
		goto loc_826F5B9C;
	case 6:
		goto loc_826F5B9C;
	case 7:
		goto loc_826F5B9C;
	case 8:
		goto loc_826F5B9C;
	case 9:
		goto loc_826F5B9C;
	case 10:
		goto loc_826F5B9C;
	case 11:
		goto loc_826F5B9C;
	case 12:
		goto loc_826F5B9C;
	case 13:
		goto loc_826F5B9C;
	case 14:
		goto loc_826F5B9C;
	case 15:
		goto loc_826F5B9C;
	case 16:
		goto loc_826F5B9C;
	case 17:
		goto loc_826F5B9C;
	case 18:
		goto loc_826F5B9C;
	case 19:
		goto loc_826F5B9C;
	case 20:
		goto loc_826F5B9C;
	case 21:
		goto loc_826F5B9C;
	case 22:
		goto loc_826F5B9C;
	case 23:
		goto loc_826F5B9C;
	case 24:
		goto loc_826F5B58;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F5B58:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826F5B60:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826F5B68:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826F5B70:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826F5B78:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826F5B80:
	// addis r11,r3,-8192
	ctx.r11.s64 = ctx.r3.s64 + -536870912;
	// addic. r11,r11,-13
	ctx.xer.ca = ctx.r11.u32 > 12;
	ctx.r11.s64 = ctx.r11.s64 + -13;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f5bb4
	if (ctx.cr0.eq) goto loc_826F5BB4;
	// cmplwi cr6,r11,16
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 16, ctx.xer);
	// beq cr6,0x826f5bac
	if (ctx.cr6.eq) goto loc_826F5BAC;
	// cmplwi cr6,r11,42
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 42, ctx.xer);
	// beq cr6,0x826f5ba4
	if (ctx.cr6.eq) goto loc_826F5BA4;
loc_826F5B9C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F5BA4:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826F5BAC:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826F5BB4:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F5BC0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F5BC0) { __imp__sub_826F5BC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F5BC0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bl 0x826ea348
	ctx.lr = 0x826F5BD4;
	sub_826EA348(ctx, base);
	// bl 0x826f59f0
	ctx.lr = 0x826F5BD8;
	sub_826F59F0(ctx, base);
	// bl 0x826f5ab8
	ctx.lr = 0x826F5BDC;
	sub_826F5AB8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F5BF0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F5BF0) { __imp__sub_826F5BF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F5BF0) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f59f0
	sub_826F59F0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F5BF8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F5BF8) { __imp__sub_826F5BF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F5BF8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f5c90
	if (ctx.cr6.gt) goto loc_826F5C90;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f5c84
	if (ctx.cr6.eq) goto loc_826F5C84;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826f5c90
	if (ctx.cr6.gt) goto loc_826F5C90;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,23604
	ctx.r12.s64 = ctx.r12.s64 + 23604;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F5C48;
	case 1:
		goto loc_826F5C60;
	case 2:
		goto loc_826F5C6C;
	case 3:
		goto loc_826F5C78;
	case 4:
		goto loc_826F5C54;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F5C48:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F5C54:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,29
	ctx.r3.u64 = ctx.r3.u64 | 29;
	// blr 
	return;
loc_826F5C60:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	// blr 
	return;
loc_826F5C6C:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,18
	ctx.r3.u64 = ctx.r3.u64 | 18;
	// blr 
	return;
loc_826F5C78:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,15
	ctx.r3.u64 = ctx.r3.u64 | 15;
	// blr 
	return;
loc_826F5C84:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
loc_826F5C90:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F5C98);  /* forward decl */
PPC_WEAK_FUNC(sub_826F5C98) { __imp__sub_826F5C98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F5C98) {
	PPC_FUNC_PROLOGUE();
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// ori r11,r11,13
	ctx.r11.u64 = ctx.r11.u64 | 13;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f5ce8
	if (ctx.cr6.gt) goto loc_826F5CE8;
	// beq cr6,0x826f5ce0
	if (ctx.cr6.eq) goto loc_826F5CE0;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addic. r11,r11,-14
	ctx.xer.ca = ctx.r11.u32 > 13;
	ctx.r11.s64 = ctx.r11.s64 + -14;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f5cd8
	if (ctx.cr0.eq) goto loc_826F5CD8;
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// beq cr6,0x826f5cd0
	if (ctx.cr6.eq) goto loc_826F5CD0;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bne cr6,0x826f5cfc
	if (!ctx.cr6.eq) goto loc_826F5CFC;
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826F5CD0:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826F5CD8:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826F5CE0:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826F5CE8:
	// addis r11,r3,-8192
	ctx.r11.s64 = ctx.r3.s64 + -536870912;
	// addic. r11,r11,-29
	ctx.xer.ca = ctx.r11.u32 > 28;
	ctx.r11.s64 = ctx.r11.s64 + -29;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f5d0c
	if (ctx.cr0.eq) goto loc_826F5D0C;
	// cmplwi cr6,r11,26
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 26, ctx.xer);
	// beq cr6,0x826f5d04
	if (ctx.cr6.eq) goto loc_826F5D04;
loc_826F5CFC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F5D04:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826F5D0C:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F5D18);  /* forward decl */
PPC_WEAK_FUNC(sub_826F5D18) { __imp__sub_826F5D18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F5D18) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bl 0x826ea4f8
	ctx.lr = 0x826F5D2C;
	sub_826EA4F8(ctx, base);
	// bl 0x826f5bf8
	ctx.lr = 0x826F5D30;
	sub_826F5BF8(ctx, base);
	// bl 0x826f5c98
	ctx.lr = 0x826F5D34;
	sub_826F5C98(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F5D48);  /* forward decl */
PPC_WEAK_FUNC(sub_826F5D48) { __imp__sub_826F5D48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F5D48) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f5bf8
	sub_826F5BF8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F5D50);  /* forward decl */
PPC_WEAK_FUNC(sub_826F5D50) { __imp__sub_826F5D50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F5D50) {
	PPC_FUNC_PROLOGUE();
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// cmplwi cr6,r4,3
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 3, ctx.xer);
	// ori r10,r11,38
	ctx.r10.u64 = ctx.r11.u64 | 38;
	// bgt cr6,0x826f5d9c
	if (ctx.cr6.gt) goto loc_826F5D9C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,23928
	ctx.r12.s64 = ctx.r12.s64 + 23928;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826F5DD0;
	case 1:
		goto loc_826F5DC0;
	case 2:
		goto loc_826F5D94;
	case 3:
		goto loc_826F5D88;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F5D88:
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,39
	ctx.r11.u64 = ctx.r11.u64 | 39;
	// b 0x826f5da0
	goto loc_826F5DA0;
loc_826F5D94:
	// mr r11,r10
	ctx.r11.u64 = ctx.r10.u64;
	// b 0x826f5da0
	goto loc_826F5DA0;
loc_826F5D9C:
	// li r11,-1
	ctx.r11.s64 = -1;
loc_826F5DA0:
	// subf. r11,r10,r11
	ctx.r11.u64 = ctx.r11.u64 - ctx.r10.u64;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f5db8
	if (ctx.cr0.eq) goto loc_826F5DB8;
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// bne cr6,0x826f5dc8
	if (!ctx.cr6.eq) goto loc_826F5DC8;
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826F5DB8:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826F5DC0:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826F5DC8:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F5DD0:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F5DD8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F5DD8) { __imp__sub_826F5DD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F5DD8) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r4,3
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 3, ctx.xer);
	// bgt cr6,0x826f5e18
	if (ctx.cr6.gt) goto loc_826F5E18;
	// beq cr6,0x826f5e0c
	if (ctx.cr6.eq) goto loc_826F5E0C;
	// cmpwi cr6,r4,1
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 1, ctx.xer);
	// beq cr6,0x826f5e00
	if (ctx.cr6.eq) goto loc_826F5E00;
	// cmpwi cr6,r4,2
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 2, ctx.xer);
	// bne cr6,0x826f5e20
	if (!ctx.cr6.eq) goto loc_826F5E20;
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,39
	ctx.r3.u64 = ctx.r3.u64 | 39;
	// blr 
	return;
loc_826F5E00:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,38
	ctx.r3.u64 = ctx.r3.u64 | 38;
	// blr 
	return;
loc_826F5E0C:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F5E18:
	// cmplwi cr6,r4,65534
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 65534, ctx.xer);
	// beq cr6,0x826f5e28
	if (ctx.cr6.eq) goto loc_826F5E28;
loc_826F5E20:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F5E28:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,62
	ctx.r3.u64 = ctx.r3.u64 | 62;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F5E38);  /* forward decl */
PPC_WEAK_FUNC(sub_826F5E38) { __imp__sub_826F5E38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F5E38) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eaf0
	ctx.lr = 0x826F5E40;
	__savegprlr_18(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r22,0
	ctx.r22.s64 = 0;
	// mr r23,r6
	ctx.r23.u64 = ctx.r6.u64;
	// mr r24,r7
	ctx.r24.u64 = ctx.r7.u64;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// li r18,1
	ctx.r18.s64 = 1;
	// mr r21,r5
	ctx.r21.u64 = ctx.r5.u64;
	// std r22,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r22.u64);
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// stb r22,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r22.u8);
	// mr r31,r18
	ctx.r31.u64 = ctx.r18.u64;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r28,r4
	ctx.r28.u64 = ctx.r4.u64;
	// add r20,r4,r9
	ctx.r20.u64 = ctx.r4.u64 + ctx.r9.u64;
	// lwz r11,44(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F5E84;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r19,r3
	ctx.r19.u64 = ctx.r3.u64;
	// mr r27,r22
	ctx.r27.u64 = ctx.r22.u64;
loc_826F5E8C:
	// clrlwi r11,r31,24
	ctx.r11.u64 = ctx.r31.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826f5f90
	if (ctx.cr6.eq) goto loc_826F5F90;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// lwz r11,48(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 48);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F5EB0;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// bl 0x82977d18
	ctx.lr = 0x826F5EC0;
	sub_82977D18(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826f5f84
	if (!ctx.cr6.eq) goto loc_826F5F84;
	// cmplwi cr6,r27,3
	ctx.cr6.compare<uint32_t>(ctx.r27.u32, 3, ctx.xer);
	// bgt cr6,0x826f5f0c
	if (ctx.cr6.gt) goto loc_826F5F0C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,24300
	ctx.r12.s64 = ctx.r12.s64 + 24300;
	// rlwinm r0,r27,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r27.u32 | (ctx.r27.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r27.u32) {
	case 0:
		goto loc_826F5EFC;
	case 1:
		goto loc_826F5EFC;
	case 2:
		goto loc_826F5F04;
	case 3:
		goto loc_826F5F04;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F5EFC:
	// li r6,8
	ctx.r6.s64 = 8;
	// b 0x826f5f10
	goto loc_826F5F10;
loc_826F5F04:
	// li r6,4
	ctx.r6.s64 = 4;
	// b 0x826f5f10
	goto loc_826F5F10;
loc_826F5F0C:
	// mr r6,r22
	ctx.r6.u64 = ctx.r22.u64;
loc_826F5F10:
	// cmplw cr6,r19,r27
	ctx.cr6.compare<uint32_t>(ctx.r19.u32, ctx.r27.u32, ctx.xer);
	// bne cr6,0x826f5f38
	if (!ctx.cr6.eq) goto loc_826F5F38;
	// li r6,8
	ctx.r6.s64 = 8;
	// mr r5,r21
	ctx.r5.u64 = ctx.r21.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F5F2C;
	sub_82978530(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// stb r31,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r31.u8);
	// b 0x826f5f84
	goto loc_826F5F84;
loc_826F5F38:
	// add r29,r6,r28
	ctx.r29.u64 = ctx.r6.u64 + ctx.r28.u64;
	// cmplw cr6,r29,r20
	ctx.cr6.compare<uint32_t>(ctx.r29.u32, ctx.r20.u32, ctx.xer);
	// bgt cr6,0x826f5f64
	if (ctx.cr6.gt) goto loc_826F5F64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F5F54;
	sub_82978530(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// mr r11,r18
	ctx.r11.u64 = ctx.r18.u64;
	// bne cr6,0x826f5f68
	if (!ctx.cr6.eq) goto loc_826F5F68;
loc_826F5F64:
	// mr r11,r22
	ctx.r11.u64 = ctx.r22.u64;
loc_826F5F68:
	// clrlwi r31,r11,24
	ctx.r31.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x826f5f80
	if (ctx.cr6.eq) goto loc_826F5F80;
	// extsw r11,r27
	ctx.r11.s64 = ctx.r27.s32;
	// sld r11,r18,r11
	ctx.r11.u64 = ctx.r11.u8 & 0x40 ? 0 : (ctx.r18.u64 << (ctx.r11.u8 & 0x7F));
	// std r11,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r11.u64);
loc_826F5F80:
	// mr r28,r29
	ctx.r28.u64 = ctx.r29.u64;
loc_826F5F84:
	// addi r27,r27,1
	ctx.r27.s64 = ctx.r27.s64 + 1;
	// cmpwi cr6,r27,4
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 4, ctx.xer);
	// blt cr6,0x826f5e8c
	if (ctx.cr6.lt) goto loc_826F5E8C;
loc_826F5F90:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x8298eb40
	__restgprlr_18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F5FA0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F5FA0) { __imp__sub_826F5FA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F5FA0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// cmplwi cr6,r4,3
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 3, ctx.xer);
	// ori r10,r11,38
	ctx.r10.u64 = ctx.r11.u64 | 38;
	// bgt cr6,0x826f5fec
	if (ctx.cr6.gt) goto loc_826F5FEC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,24520
	ctx.r12.s64 = ctx.r12.s64 + 24520;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826F5FD8;
	case 1:
		goto loc_826F5FE0;
	case 2:
		goto loc_826F6010;
	case 3:
		goto loc_826F6020;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F5FD8:
	// mr r11,r10
	ctx.r11.u64 = ctx.r10.u64;
	// b 0x826f5ff0
	goto loc_826F5FF0;
loc_826F5FE0:
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,39
	ctx.r11.u64 = ctx.r11.u64 | 39;
	// b 0x826f5ff0
	goto loc_826F5FF0;
loc_826F5FEC:
	// li r11,-1
	ctx.r11.s64 = -1;
loc_826F5FF0:
	// subf. r11,r10,r11
	ctx.r11.u64 = ctx.r11.u64 - ctx.r10.u64;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f6008
	if (ctx.cr0.eq) goto loc_826F6008;
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// bne cr6,0x826f6018
	if (!ctx.cr6.eq) goto loc_826F6018;
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826F6008:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826F6010:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826F6018:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F6020:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F6028);  /* forward decl */
PPC_WEAK_FUNC(sub_826F6028) { __imp__sub_826F6028(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F6028) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r4,6
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 6, ctx.xer);
	// bgt cr6,0x826f6068
	if (ctx.cr6.gt) goto loc_826F6068;
	// beq cr6,0x826f605c
	if (ctx.cr6.eq) goto loc_826F605C;
	// cmpwi cr6,r4,1
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 1, ctx.xer);
	// beq cr6,0x826f6050
	if (ctx.cr6.eq) goto loc_826F6050;
	// cmpwi cr6,r4,3
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 3, ctx.xer);
	// bne cr6,0x826f6070
	if (!ctx.cr6.eq) goto loc_826F6070;
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,38
	ctx.r3.u64 = ctx.r3.u64 | 38;
	// blr 
	return;
loc_826F6050:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,39
	ctx.r3.u64 = ctx.r3.u64 | 39;
	// blr 
	return;
loc_826F605C:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F6068:
	// cmplwi cr6,r4,65534
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 65534, ctx.xer);
	// beq cr6,0x826f6078
	if (ctx.cr6.eq) goto loc_826F6078;
loc_826F6070:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F6078:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F6088);  /* forward decl */
PPC_WEAK_FUNC(sub_826F6088) { __imp__sub_826F6088(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F6088) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eaf0
	ctx.lr = 0x826F6090;
	__savegprlr_18(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r22,0
	ctx.r22.s64 = 0;
	// mr r23,r6
	ctx.r23.u64 = ctx.r6.u64;
	// mr r24,r7
	ctx.r24.u64 = ctx.r7.u64;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// li r18,1
	ctx.r18.s64 = 1;
	// mr r21,r5
	ctx.r21.u64 = ctx.r5.u64;
	// std r22,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r22.u64);
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// stb r22,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r22.u8);
	// mr r31,r18
	ctx.r31.u64 = ctx.r18.u64;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r28,r4
	ctx.r28.u64 = ctx.r4.u64;
	// add r20,r4,r9
	ctx.r20.u64 = ctx.r4.u64 + ctx.r9.u64;
	// lwz r11,44(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F60D4;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r19,r3
	ctx.r19.u64 = ctx.r3.u64;
	// mr r27,r22
	ctx.r27.u64 = ctx.r22.u64;
loc_826F60DC:
	// clrlwi r11,r31,24
	ctx.r11.u64 = ctx.r31.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826f61e0
	if (ctx.cr6.eq) goto loc_826F61E0;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// lwz r11,48(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 48);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F6100;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// bl 0x82977d18
	ctx.lr = 0x826F6110;
	sub_82977D18(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826f61d4
	if (!ctx.cr6.eq) goto loc_826F61D4;
	// cmplwi cr6,r27,3
	ctx.cr6.compare<uint32_t>(ctx.r27.u32, 3, ctx.xer);
	// bgt cr6,0x826f615c
	if (ctx.cr6.gt) goto loc_826F615C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,24892
	ctx.r12.s64 = ctx.r12.s64 + 24892;
	// rlwinm r0,r27,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r27.u32 | (ctx.r27.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r27.u32) {
	case 0:
		goto loc_826F614C;
	case 1:
		goto loc_826F614C;
	case 2:
		goto loc_826F6154;
	case 3:
		goto loc_826F6154;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F614C:
	// li r6,4
	ctx.r6.s64 = 4;
	// b 0x826f6160
	goto loc_826F6160;
loc_826F6154:
	// li r6,8
	ctx.r6.s64 = 8;
	// b 0x826f6160
	goto loc_826F6160;
loc_826F615C:
	// mr r6,r22
	ctx.r6.u64 = ctx.r22.u64;
loc_826F6160:
	// cmplw cr6,r19,r27
	ctx.cr6.compare<uint32_t>(ctx.r19.u32, ctx.r27.u32, ctx.xer);
	// bne cr6,0x826f6188
	if (!ctx.cr6.eq) goto loc_826F6188;
	// li r6,8
	ctx.r6.s64 = 8;
	// mr r5,r21
	ctx.r5.u64 = ctx.r21.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F617C;
	sub_82978530(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// stb r31,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r31.u8);
	// b 0x826f61d4
	goto loc_826F61D4;
loc_826F6188:
	// add r29,r6,r28
	ctx.r29.u64 = ctx.r6.u64 + ctx.r28.u64;
	// cmplw cr6,r29,r20
	ctx.cr6.compare<uint32_t>(ctx.r29.u32, ctx.r20.u32, ctx.xer);
	// bgt cr6,0x826f61b4
	if (ctx.cr6.gt) goto loc_826F61B4;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F61A4;
	sub_82978530(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// mr r11,r18
	ctx.r11.u64 = ctx.r18.u64;
	// bne cr6,0x826f61b8
	if (!ctx.cr6.eq) goto loc_826F61B8;
loc_826F61B4:
	// mr r11,r22
	ctx.r11.u64 = ctx.r22.u64;
loc_826F61B8:
	// clrlwi r31,r11,24
	ctx.r31.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x826f61d0
	if (ctx.cr6.eq) goto loc_826F61D0;
	// extsw r11,r27
	ctx.r11.s64 = ctx.r27.s32;
	// sld r11,r18,r11
	ctx.r11.u64 = ctx.r11.u8 & 0x40 ? 0 : (ctx.r18.u64 << (ctx.r11.u8 & 0x7F));
	// std r11,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r11.u64);
loc_826F61D0:
	// mr r28,r29
	ctx.r28.u64 = ctx.r29.u64;
loc_826F61D4:
	// addi r27,r27,1
	ctx.r27.s64 = ctx.r27.s64 + 1;
	// cmpwi cr6,r27,4
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 4, ctx.xer);
	// blt cr6,0x826f60dc
	if (ctx.cr6.lt) goto loc_826F60DC;
loc_826F61E0:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x8298eb40
	__restgprlr_18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F61F0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F61F0) { __imp__sub_826F61F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F61F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// cmplwi cr6,r4,3
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 3, ctx.xer);
	// ori r10,r11,38
	ctx.r10.u64 = ctx.r11.u64 | 38;
	// bgt cr6,0x826f623c
	if (ctx.cr6.gt) goto loc_826F623C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,25112
	ctx.r12.s64 = ctx.r12.s64 + 25112;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826F6234;
	case 1:
		goto loc_826F6228;
	case 2:
		goto loc_826F6260;
	case 3:
		goto loc_826F6270;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F6228:
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,39
	ctx.r11.u64 = ctx.r11.u64 | 39;
	// b 0x826f6240
	goto loc_826F6240;
loc_826F6234:
	// mr r11,r10
	ctx.r11.u64 = ctx.r10.u64;
	// b 0x826f6240
	goto loc_826F6240;
loc_826F623C:
	// li r11,-1
	ctx.r11.s64 = -1;
loc_826F6240:
	// subf. r11,r10,r11
	ctx.r11.u64 = ctx.r11.u64 - ctx.r10.u64;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f6258
	if (ctx.cr0.eq) goto loc_826F6258;
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// bne cr6,0x826f6268
	if (!ctx.cr6.eq) goto loc_826F6268;
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826F6258:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826F6260:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826F6268:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F6270:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F6278);  /* forward decl */
PPC_WEAK_FUNC(sub_826F6278) { __imp__sub_826F6278(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F6278) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r4,5
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 5, ctx.xer);
	// bgt cr6,0x826f62b8
	if (ctx.cr6.gt) goto loc_826F62B8;
	// beq cr6,0x826f62ac
	if (ctx.cr6.eq) goto loc_826F62AC;
	// cmpwi cr6,r4,1
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 1, ctx.xer);
	// beq cr6,0x826f62a0
	if (ctx.cr6.eq) goto loc_826F62A0;
	// cmpwi cr6,r4,3
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 3, ctx.xer);
	// bne cr6,0x826f62c0
	if (!ctx.cr6.eq) goto loc_826F62C0;
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,39
	ctx.r3.u64 = ctx.r3.u64 | 39;
	// blr 
	return;
loc_826F62A0:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,38
	ctx.r3.u64 = ctx.r3.u64 | 38;
	// blr 
	return;
loc_826F62AC:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F62B8:
	// cmplwi cr6,r4,65534
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 65534, ctx.xer);
	// beq cr6,0x826f62c8
	if (ctx.cr6.eq) goto loc_826F62C8;
loc_826F62C0:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F62C8:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F62D8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F62D8) { __imp__sub_826F62D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F62D8) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r4,6
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 6, ctx.xer);
	// bgt cr6,0x826f6318
	if (ctx.cr6.gt) goto loc_826F6318;
	// beq cr6,0x826f630c
	if (ctx.cr6.eq) goto loc_826F630C;
	// cmpwi cr6,r4,1
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 1, ctx.xer);
	// beq cr6,0x826f6300
	if (ctx.cr6.eq) goto loc_826F6300;
	// cmpwi cr6,r4,3
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 3, ctx.xer);
	// bne cr6,0x826f6320
	if (!ctx.cr6.eq) goto loc_826F6320;
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,39
	ctx.r3.u64 = ctx.r3.u64 | 39;
	// blr 
	return;
loc_826F6300:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,38
	ctx.r3.u64 = ctx.r3.u64 | 38;
	// blr 
	return;
loc_826F630C:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F6318:
	// cmplwi cr6,r4,65534
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 65534, ctx.xer);
	// beq cr6,0x826f6328
	if (ctx.cr6.eq) goto loc_826F6328;
loc_826F6320:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F6328:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F6338);  /* forward decl */
PPC_WEAK_FUNC(sub_826F6338) { __imp__sub_826F6338(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F6338) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f63d4
	if (ctx.cr6.gt) goto loc_826F63D4;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f63c8
	if (ctx.cr6.eq) goto loc_826F63C8;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x826f63d4
	if (ctx.cr6.gt) goto loc_826F63D4;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,25460
	ctx.r12.s64 = ctx.r12.s64 + 25460;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F638C;
	case 1:
		goto loc_826F63BC;
	case 2:
		goto loc_826F63A4;
	case 3:
		goto loc_826F63D4;
	case 4:
		goto loc_826F6398;
	case 5:
		goto loc_826F63B0;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F638C:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,38
	ctx.r3.u64 = ctx.r3.u64 | 38;
	// blr 
	return;
loc_826F6398:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,41
	ctx.r3.u64 = ctx.r3.u64 | 41;
	// blr 
	return;
loc_826F63A4:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,39
	ctx.r3.u64 = ctx.r3.u64 | 39;
	// blr 
	return;
loc_826F63B0:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,42
	ctx.r3.u64 = ctx.r3.u64 | 42;
	// blr 
	return;
loc_826F63BC:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F63C8:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,58
	ctx.r3.u64 = ctx.r3.u64 | 58;
	// blr 
	return;
loc_826F63D4:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F63E0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F63E0) { __imp__sub_826F63E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F63E0) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f6338
	sub_826F6338(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F63E8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F63E8) { __imp__sub_826F63E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F63E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// ori r11,r11,13
	ctx.r11.u64 = ctx.r11.u64 | 13;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f6438
	if (ctx.cr6.gt) goto loc_826F6438;
	// beq cr6,0x826f6430
	if (ctx.cr6.eq) goto loc_826F6430;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addic. r11,r11,-38
	ctx.xer.ca = ctx.r11.u32 > 37;
	ctx.r11.s64 = ctx.r11.s64 + -38;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f6428
	if (ctx.cr0.eq) goto loc_826F6428;
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// beq cr6,0x826f6420
	if (ctx.cr6.eq) goto loc_826F6420;
	// cmplwi cr6,r11,3
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 3, ctx.xer);
	// bne cr6,0x826f644c
	if (!ctx.cr6.eq) goto loc_826F644C;
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826F6420:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826F6428:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826F6430:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826F6438:
	// addis r11,r3,-8192
	ctx.r11.s64 = ctx.r3.s64 + -536870912;
	// addic. r11,r11,-42
	ctx.xer.ca = ctx.r11.u32 > 41;
	ctx.r11.s64 = ctx.r11.s64 + -42;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f645c
	if (ctx.cr0.eq) goto loc_826F645C;
	// cmplwi cr6,r11,16
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 16, ctx.xer);
	// beq cr6,0x826f6454
	if (ctx.cr6.eq) goto loc_826F6454;
loc_826F644C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F6454:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826F645C:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F6468);  /* forward decl */
PPC_WEAK_FUNC(sub_826F6468) { __imp__sub_826F6468(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F6468) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bl 0x826eac08
	ctx.lr = 0x826F647C;
	sub_826EAC08(ctx, base);
	// bl 0x826f6338
	ctx.lr = 0x826F6480;
	sub_826F6338(ctx, base);
	// bl 0x826f63e8
	ctx.lr = 0x826F6484;
	sub_826F63E8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F6498);  /* forward decl */
PPC_WEAK_FUNC(sub_826F6498) { __imp__sub_826F6498(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F6498) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eaf0
	ctx.lr = 0x826F64A0;
	__savegprlr_18(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r22,0
	ctx.r22.s64 = 0;
	// mr r23,r6
	ctx.r23.u64 = ctx.r6.u64;
	// mr r24,r7
	ctx.r24.u64 = ctx.r7.u64;
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// li r18,1
	ctx.r18.s64 = 1;
	// mr r21,r5
	ctx.r21.u64 = ctx.r5.u64;
	// std r22,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r22.u64);
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// stb r22,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r22.u8);
	// mr r31,r18
	ctx.r31.u64 = ctx.r18.u64;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r28,r4
	ctx.r28.u64 = ctx.r4.u64;
	// add r20,r4,r9
	ctx.r20.u64 = ctx.r4.u64 + ctx.r9.u64;
	// lwz r11,44(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F64E4;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r19,r3
	ctx.r19.u64 = ctx.r3.u64;
	// mr r27,r22
	ctx.r27.u64 = ctx.r22.u64;
loc_826F64EC:
	// clrlwi r11,r31,24
	ctx.r11.u64 = ctx.r31.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x826f65f8
	if (ctx.cr6.eq) goto loc_826F65F8;
	// lwz r11,0(r26)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r26.u32 + 0);
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// lwz r11,48(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 48);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x826F6510;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// bl 0x82977d18
	ctx.lr = 0x826F6520;
	sub_82977D18(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x826f65ec
	if (!ctx.cr6.eq) goto loc_826F65EC;
	// cmplwi cr6,r27,5
	ctx.cr6.compare<uint32_t>(ctx.r27.u32, 5, ctx.xer);
	// bgt cr6,0x826f6574
	if (ctx.cr6.gt) goto loc_826F6574;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,25932
	ctx.r12.s64 = ctx.r12.s64 + 25932;
	// rlwinm r0,r27,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r27.u32 | (ctx.r27.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r27.u32) {
	case 0:
		goto loc_826F6564;
	case 1:
		goto loc_826F6564;
	case 2:
		goto loc_826F6564;
	case 3:
		goto loc_826F656C;
	case 4:
		goto loc_826F656C;
	case 5:
		goto loc_826F656C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F6564:
	// li r6,4
	ctx.r6.s64 = 4;
	// b 0x826f6578
	goto loc_826F6578;
loc_826F656C:
	// li r6,8
	ctx.r6.s64 = 8;
	// b 0x826f6578
	goto loc_826F6578;
loc_826F6574:
	// mr r6,r22
	ctx.r6.u64 = ctx.r22.u64;
loc_826F6578:
	// cmplw cr6,r19,r27
	ctx.cr6.compare<uint32_t>(ctx.r19.u32, ctx.r27.u32, ctx.xer);
	// bne cr6,0x826f65a0
	if (!ctx.cr6.eq) goto loc_826F65A0;
	// li r6,8
	ctx.r6.s64 = 8;
	// mr r5,r21
	ctx.r5.u64 = ctx.r21.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F6594;
	sub_82978530(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// stb r31,0(r24)
	PPC_STORE_U8(ctx.r24.u32 + 0, ctx.r31.u8);
	// b 0x826f65ec
	goto loc_826F65EC;
loc_826F65A0:
	// add r30,r6,r28
	ctx.r30.u64 = ctx.r6.u64 + ctx.r28.u64;
	// cmplw cr6,r30,r20
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, ctx.r20.u32, ctx.xer);
	// bgt cr6,0x826f65cc
	if (ctx.cr6.gt) goto loc_826F65CC;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// bl 0x82978530
	ctx.lr = 0x826F65BC;
	sub_82978530(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// mr r11,r18
	ctx.r11.u64 = ctx.r18.u64;
	// bne cr6,0x826f65d0
	if (!ctx.cr6.eq) goto loc_826F65D0;
loc_826F65CC:
	// mr r11,r22
	ctx.r11.u64 = ctx.r22.u64;
loc_826F65D0:
	// clrlwi r31,r11,24
	ctx.r31.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x826f65e8
	if (ctx.cr6.eq) goto loc_826F65E8;
	// extsw r11,r27
	ctx.r11.s64 = ctx.r27.s32;
	// sld r11,r18,r11
	ctx.r11.u64 = ctx.r11.u8 & 0x40 ? 0 : (ctx.r18.u64 << (ctx.r11.u8 & 0x7F));
	// std r11,0(r23)
	PPC_STORE_U64(ctx.r23.u32 + 0, ctx.r11.u64);
loc_826F65E8:
	// mr r28,r30
	ctx.r28.u64 = ctx.r30.u64;
loc_826F65EC:
	// addi r27,r27,1
	ctx.r27.s64 = ctx.r27.s64 + 1;
	// cmpwi cr6,r27,6
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 6, ctx.xer);
	// blt cr6,0x826f64ec
	if (ctx.cr6.lt) goto loc_826F64EC;
loc_826F65F8:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x8298eb40
	__restgprlr_18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F6608);  /* forward decl */
PPC_WEAK_FUNC(sub_826F6608) { __imp__sub_826F6608(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F6608) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// cmplwi cr6,r4,4
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 4, ctx.xer);
	// bgt cr6,0x826f6674
	if (ctx.cr6.gt) goto loc_826F6674;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,26164
	ctx.r12.s64 = ctx.r12.s64 + 26164;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826F6648;
	case 1:
		goto loc_826F6650;
	case 2:
		goto loc_826F6658;
	case 3:
		goto loc_826F6660;
	case 4:
		goto loc_826F6668;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F6648:
	// li r3,6
	ctx.r3.s64 = 6;
	// b 0x826f6678
	goto loc_826F6678;
loc_826F6650:
	// li r3,7
	ctx.r3.s64 = 7;
	// b 0x826f6678
	goto loc_826F6678;
loc_826F6658:
	// li r3,8
	ctx.r3.s64 = 8;
	// b 0x826f6678
	goto loc_826F6678;
loc_826F6660:
	// li r3,1
	ctx.r3.s64 = 1;
	// b 0x826f6678
	goto loc_826F6678;
loc_826F6668:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// b 0x826f6678
	goto loc_826F6678;
loc_826F6674:
	// li r3,-1
	ctx.r3.s64 = -1;
loc_826F6678:
	// bl 0x826f68d0
	ctx.lr = 0x826F667C;
	sub_826F68D0(ctx, base);
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// ori r11,r11,13
	ctx.r11.u64 = ctx.r11.u64 | 13;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f66e0
	if (ctx.cr6.gt) goto loc_826F66E0;
	// beq cr6,0x826f66cc
	if (ctx.cr6.eq) goto loc_826F66CC;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addic. r11,r11,-38
	ctx.xer.ca = ctx.r11.u32 > 37;
	ctx.r11.s64 = ctx.r11.s64 + -38;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f66b8
	if (ctx.cr0.eq) goto loc_826F66B8;
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// bne cr6,0x826f66f4
	if (!ctx.cr6.eq) goto loc_826F66F4;
	// li r3,1
	ctx.r3.s64 = 1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F66B8:
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
loc_826F66CC:
	// li r3,2
	ctx.r3.s64 = 2;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F66E0:
	// addis r11,r3,-8192
	ctx.r11.s64 = ctx.r3.s64 + -536870912;
	// addic. r11,r11,-29
	ctx.xer.ca = ctx.r11.u32 > 28;
	ctx.r11.s64 = ctx.r11.s64 + -29;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f671c
	if (ctx.cr0.eq) goto loc_826F671C;
	// cmplwi cr6,r11,26
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 26, ctx.xer);
	// beq cr6,0x826f6708
	if (ctx.cr6.eq) goto loc_826F6708;
loc_826F66F4:
	// li r3,-1
	ctx.r3.s64 = -1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F6708:
	// li r3,4
	ctx.r3.s64 = 4;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F671C:
	// li r3,3
	ctx.r3.s64 = 3;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F6730);  /* forward decl */
PPC_WEAK_FUNC(sub_826F6730) { __imp__sub_826F6730(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F6730) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r3,7
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 7, ctx.xer);
	// bgt cr6,0x826f6770
	if (ctx.cr6.gt) goto loc_826F6770;
	// beq cr6,0x826f6764
	if (ctx.cr6.eq) goto loc_826F6764;
	// cmpwi cr6,r3,1
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 1, ctx.xer);
	// beq cr6,0x826f6758
	if (ctx.cr6.eq) goto loc_826F6758;
	// cmpwi cr6,r3,6
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 6, ctx.xer);
	// bne cr6,0x826f6780
	if (!ctx.cr6.eq) goto loc_826F6780;
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,39
	ctx.r3.u64 = ctx.r3.u64 | 39;
	// blr 
	return;
loc_826F6758:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,29
	ctx.r3.u64 = ctx.r3.u64 | 29;
	// blr 
	return;
loc_826F6764:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,38
	ctx.r3.u64 = ctx.r3.u64 | 38;
	// blr 
	return;
loc_826F6770:
	// cmpwi cr6,r3,8
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 8, ctx.xer);
	// beq cr6,0x826f6794
	if (ctx.cr6.eq) goto loc_826F6794;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f6788
	if (ctx.cr6.eq) goto loc_826F6788;
loc_826F6780:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F6788:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
loc_826F6794:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F67A0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F67A0) { __imp__sub_826F67A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F67A0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// cmplwi cr6,r4,4
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 4, ctx.xer);
	// bgt cr6,0x826f680c
	if (ctx.cr6.gt) goto loc_826F680C;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,26572
	ctx.r12.s64 = ctx.r12.s64 + 26572;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826F67E0;
	case 1:
		goto loc_826F67E8;
	case 2:
		goto loc_826F67F0;
	case 3:
		goto loc_826F67F8;
	case 4:
		goto loc_826F6800;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F67E0:
	// li r3,7
	ctx.r3.s64 = 7;
	// b 0x826f6810
	goto loc_826F6810;
loc_826F67E8:
	// li r3,6
	ctx.r3.s64 = 6;
	// b 0x826f6810
	goto loc_826F6810;
loc_826F67F0:
	// li r3,8
	ctx.r3.s64 = 8;
	// b 0x826f6810
	goto loc_826F6810;
loc_826F67F8:
	// li r3,1
	ctx.r3.s64 = 1;
	// b 0x826f6810
	goto loc_826F6810;
loc_826F6800:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// b 0x826f6810
	goto loc_826F6810;
loc_826F680C:
	// li r3,-1
	ctx.r3.s64 = -1;
loc_826F6810:
	// bl 0x826f6730
	ctx.lr = 0x826F6814;
	sub_826F6730(ctx, base);
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// ori r11,r11,13
	ctx.r11.u64 = ctx.r11.u64 | 13;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f6878
	if (ctx.cr6.gt) goto loc_826F6878;
	// beq cr6,0x826f6864
	if (ctx.cr6.eq) goto loc_826F6864;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addic. r11,r11,-38
	ctx.xer.ca = ctx.r11.u32 > 37;
	ctx.r11.s64 = ctx.r11.s64 + -38;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f6850
	if (ctx.cr0.eq) goto loc_826F6850;
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// bne cr6,0x826f688c
	if (!ctx.cr6.eq) goto loc_826F688C;
	// li r3,1
	ctx.r3.s64 = 1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F6850:
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
loc_826F6864:
	// li r3,2
	ctx.r3.s64 = 2;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F6878:
	// addis r11,r3,-8192
	ctx.r11.s64 = ctx.r3.s64 + -536870912;
	// addic. r11,r11,-29
	ctx.xer.ca = ctx.r11.u32 > 28;
	ctx.r11.s64 = ctx.r11.s64 + -29;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f68b4
	if (ctx.cr0.eq) goto loc_826F68B4;
	// cmplwi cr6,r11,26
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 26, ctx.xer);
	// beq cr6,0x826f68a0
	if (ctx.cr6.eq) goto loc_826F68A0;
loc_826F688C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F68A0:
	// li r3,4
	ctx.r3.s64 = 4;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F68B4:
	// li r3,3
	ctx.r3.s64 = 3;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F68C8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F68C8) { __imp__sub_826F68C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F68C8) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f6730
	sub_826F6730(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F68D0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F68D0) { __imp__sub_826F68D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F68D0) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r3,7
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 7, ctx.xer);
	// bgt cr6,0x826f6910
	if (ctx.cr6.gt) goto loc_826F6910;
	// beq cr6,0x826f6904
	if (ctx.cr6.eq) goto loc_826F6904;
	// cmpwi cr6,r3,1
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 1, ctx.xer);
	// beq cr6,0x826f68f8
	if (ctx.cr6.eq) goto loc_826F68F8;
	// cmpwi cr6,r3,6
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 6, ctx.xer);
	// bne cr6,0x826f6920
	if (!ctx.cr6.eq) goto loc_826F6920;
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,38
	ctx.r3.u64 = ctx.r3.u64 | 38;
	// blr 
	return;
loc_826F68F8:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,29
	ctx.r3.u64 = ctx.r3.u64 | 29;
	// blr 
	return;
loc_826F6904:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,39
	ctx.r3.u64 = ctx.r3.u64 | 39;
	// blr 
	return;
loc_826F6910:
	// cmpwi cr6,r3,8
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 8, ctx.xer);
	// beq cr6,0x826f6934
	if (ctx.cr6.eq) goto loc_826F6934;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f6928
	if (ctx.cr6.eq) goto loc_826F6928;
loc_826F6920:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F6928:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
loc_826F6934:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F6940);  /* forward decl */
PPC_WEAK_FUNC(sub_826F6940) { __imp__sub_826F6940(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F6940) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f68d0
	sub_826F68D0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F6948);  /* forward decl */
PPC_WEAK_FUNC(sub_826F6948) { __imp__sub_826F6948(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F6948) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r3,6
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 6, ctx.xer);
	// bgt cr6,0x826f6988
	if (ctx.cr6.gt) goto loc_826F6988;
	// beq cr6,0x826f697c
	if (ctx.cr6.eq) goto loc_826F697C;
	// cmpwi cr6,r3,1
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 1, ctx.xer);
	// beq cr6,0x826f6970
	if (ctx.cr6.eq) goto loc_826F6970;
	// cmpwi cr6,r3,5
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 5, ctx.xer);
	// bne cr6,0x826f6998
	if (!ctx.cr6.eq) goto loc_826F6998;
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,38
	ctx.r3.u64 = ctx.r3.u64 | 38;
	// blr 
	return;
loc_826F6970:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,29
	ctx.r3.u64 = ctx.r3.u64 | 29;
	// blr 
	return;
loc_826F697C:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,39
	ctx.r3.u64 = ctx.r3.u64 | 39;
	// blr 
	return;
loc_826F6988:
	// cmpwi cr6,r3,8
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 8, ctx.xer);
	// beq cr6,0x826f69ac
	if (ctx.cr6.eq) goto loc_826F69AC;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f69a0
	if (ctx.cr6.eq) goto loc_826F69A0;
loc_826F6998:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
loc_826F69A0:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
loc_826F69AC:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F69B8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F69B8) { __imp__sub_826F69B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F69B8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// cmplwi cr6,r4,4
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 4, ctx.xer);
	// bgt cr6,0x826f6a24
	if (ctx.cr6.gt) goto loc_826F6A24;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,27108
	ctx.r12.s64 = ctx.r12.s64 + 27108;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_826F69F8;
	case 1:
		goto loc_826F6A00;
	case 2:
		goto loc_826F6A08;
	case 3:
		goto loc_826F6A10;
	case 4:
		goto loc_826F6A18;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F69F8:
	// li r3,5
	ctx.r3.s64 = 5;
	// b 0x826f6a28
	goto loc_826F6A28;
loc_826F6A00:
	// li r3,6
	ctx.r3.s64 = 6;
	// b 0x826f6a28
	goto loc_826F6A28;
loc_826F6A08:
	// li r3,8
	ctx.r3.s64 = 8;
	// b 0x826f6a28
	goto loc_826F6A28;
loc_826F6A10:
	// li r3,1
	ctx.r3.s64 = 1;
	// b 0x826f6a28
	goto loc_826F6A28;
loc_826F6A18:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// b 0x826f6a28
	goto loc_826F6A28;
loc_826F6A24:
	// li r3,-1
	ctx.r3.s64 = -1;
loc_826F6A28:
	// bl 0x826f6948
	ctx.lr = 0x826F6A2C;
	sub_826F6948(ctx, base);
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// ori r11,r11,13
	ctx.r11.u64 = ctx.r11.u64 | 13;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f6a90
	if (ctx.cr6.gt) goto loc_826F6A90;
	// beq cr6,0x826f6a7c
	if (ctx.cr6.eq) goto loc_826F6A7C;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addic. r11,r11,-38
	ctx.xer.ca = ctx.r11.u32 > 37;
	ctx.r11.s64 = ctx.r11.s64 + -38;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f6a68
	if (ctx.cr0.eq) goto loc_826F6A68;
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// bne cr6,0x826f6aa4
	if (!ctx.cr6.eq) goto loc_826F6AA4;
	// li r3,1
	ctx.r3.s64 = 1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F6A68:
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
loc_826F6A7C:
	// li r3,2
	ctx.r3.s64 = 2;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F6A90:
	// addis r11,r3,-8192
	ctx.r11.s64 = ctx.r3.s64 + -536870912;
	// addic. r11,r11,-29
	ctx.xer.ca = ctx.r11.u32 > 28;
	ctx.r11.s64 = ctx.r11.s64 + -29;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f6acc
	if (ctx.cr0.eq) goto loc_826F6ACC;
	// cmplwi cr6,r11,26
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 26, ctx.xer);
	// beq cr6,0x826f6ab8
	if (ctx.cr6.eq) goto loc_826F6AB8;
loc_826F6AA4:
	// li r3,-1
	ctx.r3.s64 = -1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F6AB8:
	// li r3,4
	ctx.r3.s64 = 4;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_826F6ACC:
	// li r3,3
	ctx.r3.s64 = 3;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F6AE0);  /* forward decl */
PPC_WEAK_FUNC(sub_826F6AE0) { __imp__sub_826F6AE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F6AE0) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x826f6948
	sub_826F6948(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F6AE8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F6AE8) { __imp__sub_826F6AE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F6AE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f6cdc
	if (ctx.cr6.gt) goto loc_826F6CDC;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x826f6cd0
	if (ctx.cr6.eq) goto loc_826F6CD0;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,31
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 31, ctx.xer);
	// bgt cr6,0x826f6cdc
	if (ctx.cr6.gt) goto loc_826F6CDC;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,27428
	ctx.r12.s64 = ctx.r12.s64 + 27428;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F6BC8;
	case 1:
		goto loc_826F6C88;
	case 2:
		goto loc_826F6CA0;
	case 3:
		goto loc_826F6BA4;
	case 4:
		goto loc_826F6BB0;
	case 5:
		goto loc_826F6BE0;
	case 6:
		goto loc_826F6BEC;
	case 7:
		goto loc_826F6BF8;
	case 8:
		goto loc_826F6C28;
	case 9:
		goto loc_826F6C40;
	case 10:
		goto loc_826F6C4C;
	case 11:
		goto loc_826F6C04;
	case 12:
		goto loc_826F6C10;
	case 13:
		goto loc_826F6C1C;
	case 14:
		goto loc_826F6C7C;
	case 15:
		goto loc_826F6C34;
	case 16:
		goto loc_826F6CDC;
	case 17:
		goto loc_826F6CDC;
	case 18:
		goto loc_826F6C58;
	case 19:
		goto loc_826F6C64;
	case 20:
		goto loc_826F6C70;
	case 21:
		goto loc_826F6CDC;
	case 22:
		goto loc_826F6CDC;
	case 23:
		goto loc_826F6CDC;
	case 24:
		goto loc_826F6CDC;
	case 25:
		goto loc_826F6BD4;
	case 26:
		goto loc_826F6BBC;
	case 27:
		goto loc_826F6C94;
	case 28:
		goto loc_826F6CDC;
	case 29:
		goto loc_826F6CAC;
	case 30:
		goto loc_826F6CB8;
	case 31:
		goto loc_826F6CC4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F6BA4:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,38
	ctx.r3.u64 = ctx.r3.u64 | 38;
	// blr 
	return;
loc_826F6BB0:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,39
	ctx.r3.u64 = ctx.r3.u64 | 39;
	// blr 
	return;
loc_826F6BBC:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,41
	ctx.r3.u64 = ctx.r3.u64 | 41;
	// blr 
	return;
loc_826F6BC8:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,13
	ctx.r3.u64 = ctx.r3.u64 | 13;
	// blr 
	return;
loc_826F6BD4:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,29
	ctx.r3.u64 = ctx.r3.u64 | 29;
	// blr 
	return;
loc_826F6BE0:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,65
	ctx.r3.u64 = ctx.r3.u64 | 65;
	// blr 
	return;
loc_826F6BEC:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,66
	ctx.r3.u64 = ctx.r3.u64 | 66;
	// blr 
	return;
loc_826F6BF8:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,67
	ctx.r3.u64 = ctx.r3.u64 | 67;
	// blr 
	return;
loc_826F6C04:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,68
	ctx.r3.u64 = ctx.r3.u64 | 68;
	// blr 
	return;
loc_826F6C10:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,69
	ctx.r3.u64 = ctx.r3.u64 | 69;
	// blr 
	return;
loc_826F6C1C:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,70
	ctx.r3.u64 = ctx.r3.u64 | 70;
	// blr 
	return;
loc_826F6C28:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,71
	ctx.r3.u64 = ctx.r3.u64 | 71;
	// blr 
	return;
loc_826F6C34:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,72
	ctx.r3.u64 = ctx.r3.u64 | 72;
	// blr 
	return;
loc_826F6C40:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,76
	ctx.r3.u64 = ctx.r3.u64 | 76;
	// blr 
	return;
loc_826F6C4C:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,77
	ctx.r3.u64 = ctx.r3.u64 | 77;
	// blr 
	return;
loc_826F6C58:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,78
	ctx.r3.u64 = ctx.r3.u64 | 78;
	// blr 
	return;
loc_826F6C64:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,79
	ctx.r3.u64 = ctx.r3.u64 | 79;
	// blr 
	return;
loc_826F6C70:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,80
	ctx.r3.u64 = ctx.r3.u64 | 80;
	// blr 
	return;
loc_826F6C7C:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,81
	ctx.r3.u64 = ctx.r3.u64 | 81;
	// blr 
	return;
loc_826F6C88:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,14
	ctx.r3.u64 = ctx.r3.u64 | 14;
	// blr 
	return;
loc_826F6C94:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,18
	ctx.r3.u64 = ctx.r3.u64 | 18;
	// blr 
	return;
loc_826F6CA0:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,15
	ctx.r3.u64 = ctx.r3.u64 | 15;
	// blr 
	return;
loc_826F6CAC:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,17
	ctx.r3.u64 = ctx.r3.u64 | 17;
	// blr 
	return;
loc_826F6CB8:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,33
	ctx.r3.u64 = ctx.r3.u64 | 33;
	// blr 
	return;
loc_826F6CC4:
	// lis r3,4096
	ctx.r3.s64 = 268435456;
	// ori r3,r3,34
	ctx.r3.u64 = ctx.r3.u64 | 34;
	// blr 
	return;
loc_826F6CD0:
	// lis r3,8192
	ctx.r3.s64 = 536870912;
	// ori r3,r3,55
	ctx.r3.u64 = ctx.r3.u64 | 55;
	// blr 
	return;
loc_826F6CDC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_826F6CE8);  /* forward decl */
PPC_WEAK_FUNC(sub_826F6CE8) { __imp__sub_826F6CE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_826F6CE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,69
	ctx.r11.u64 = ctx.r11.u64 | 69;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f6e70
	if (ctx.cr6.gt) goto loc_826F6E70;
	// beq cr6,0x826f6e68
	if (ctx.cr6.eq) goto loc_826F6E68;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addi r11,r11,-14
	ctx.r11.s64 = ctx.r11.s64 + -14;
	// cmplwi cr6,r11,54
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 54, ctx.xer);
	// bgt cr6,0x826f6f64
	if (ctx.cr6.gt) goto loc_826F6F64;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,27940
	ctx.r12.s64 = ctx.r12.s64 + 27940;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F6E38;
	case 1:
		goto loc_826F6E48;
	case 2:
		goto loc_826F6F64;
	case 3:
		goto loc_826F6E50;
	case 4:
		goto loc_826F6E40;
	case 5:
		goto loc_826F6F64;
	case 6:
		goto loc_826F6F64;
	case 7:
		goto loc_826F6F64;
	case 8:
		goto loc_826F6F64;
	case 9:
		goto loc_826F6F64;
	case 10:
		goto loc_826F6F64;
	case 11:
		goto loc_826F6F64;
	case 12:
		goto loc_826F6F64;
	case 13:
		goto loc_826F6F64;
	case 14:
		goto loc_826F6F64;
	case 15:
		goto loc_826F6F64;
	case 16:
		goto loc_826F6F64;
	case 17:
		goto loc_826F6F64;
	case 18:
		goto loc_826F6F64;
	case 19:
		goto loc_826F6E58;
	case 20:
		goto loc_826F6E60;
	case 21:
		goto loc_826F6F64;
	case 22:
		goto loc_826F6F64;
	case 23:
		goto loc_826F6F64;
	case 24:
		goto loc_826F6E00;
	case 25:
		goto loc_826F6E08;
	case 26:
		goto loc_826F6F64;
	case 27:
		goto loc_826F6E10;
	case 28:
		goto loc_826F6F64;
	case 29:
		goto loc_826F6F64;
	case 30:
		goto loc_826F6F64;
	case 31:
		goto loc_826F6F64;
	case 32:
		goto loc_826F6F64;
	case 33:
		goto loc_826F6F64;
	case 34:
		goto loc_826F6F64;
	case 35:
		goto loc_826F6F64;
	case 36:
		goto loc_826F6F64;
	case 37:
		goto loc_826F6F64;
	case 38:
		goto loc_826F6F64;
	case 39:
		goto loc_826F6F64;
	case 40:
		goto loc_826F6F64;
	case 41:
		goto loc_826F6F64;
	case 42:
		goto loc_826F6F64;
	case 43:
		goto loc_826F6F64;
	case 44:
		goto loc_826F6F64;
	case 45:
		goto loc_826F6F64;
	case 46:
		goto loc_826F6F64;
	case 47:
		goto loc_826F6F64;
	case 48:
		goto loc_826F6F64;
	case 49:
		goto loc_826F6F64;
	case 50:
		goto loc_826F6F64;
	case 51:
		goto loc_826F6E18;
	case 52:
		goto loc_826F6E20;
	case 53:
		goto loc_826F6E28;
	case 54:
		goto loc_826F6E30;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F6E00:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_826F6E08:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_826F6E10:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_826F6E18:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_826F6E20:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_826F6E28:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_826F6E30:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_826F6E38:
	// li r3,19
	ctx.r3.s64 = 19;
	// blr 
	return;
loc_826F6E40:
	// li r3,20
	ctx.r3.s64 = 20;
	// blr 
	return;
loc_826F6E48:
	// li r3,21
	ctx.r3.s64 = 21;
	// blr 
	return;
loc_826F6E50:
	// li r3,22
	ctx.r3.s64 = 22;
	// blr 
	return;
loc_826F6E58:
	// li r3,23
	ctx.r3.s64 = 23;
	// blr 
	return;
loc_826F6E60:
	// li r3,24
	ctx.r3.s64 = 24;
	// blr 
	return;
loc_826F6E68:
	// li r3,9
	ctx.r3.s64 = 9;
	// blr 
	return;
loc_826F6E70:
	// lis r11,8192
	ctx.r11.s64 = 536870912;
	// ori r11,r11,55
	ctx.r11.u64 = ctx.r11.u64 | 55;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f6f00
	if (ctx.cr6.gt) goto loc_826F6F00;
	// beq cr6,0x826f6ef8
	if (ctx.cr6.eq) goto loc_826F6EF8;
	// lis r11,4096
	ctx.r11.s64 = 268435456;
	// ori r11,r11,81
	ctx.r11.u64 = ctx.r11.u64 | 81;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x826f6ed4
	if (ctx.cr6.gt) goto loc_826F6ED4;
	// beq cr6,0x826f6ecc
	if (ctx.cr6.eq) goto loc_826F6ECC;
	// addis r11,r3,-4096
	ctx.r11.s64 = ctx.r3.s64 + -268435456;
	// addic. r11,r11,-70
	ctx.xer.ca = ctx.r11.u32 > 69;
	ctx.r11.s64 = ctx.r11.s64 + -70;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f6ec4
	if (ctx.cr0.eq) goto loc_826F6EC4;
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// beq cr6,0x826f6ebc
	if (ctx.cr6.eq) goto loc_826F6EBC;
	// cmplwi cr6,r11,2
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 2, ctx.xer);
	// bne cr6,0x826f6f64
	if (!ctx.cr6.eq) goto loc_826F6F64;
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_826F6EBC:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_826F6EC4:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_826F6ECC:
	// li r3,18
	ctx.r3.s64 = 18;
	// blr 
	return;
loc_826F6ED4:
	// addis r11,r3,-8192
	ctx.r11.s64 = ctx.r3.s64 + -536870912;
	// addic. r11,r11,-13
	ctx.xer.ca = ctx.r11.u32 > 12;
	ctx.r11.s64 = ctx.r11.s64 + -13;
	ctx.cr0.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq 0x826f6ef0
	if (ctx.cr0.eq) goto loc_826F6EF0;
	// cmplwi cr6,r11,16
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 16, ctx.xer);
	// bne cr6,0x826f6f64
	if (!ctx.cr6.eq) goto loc_826F6F64;
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_826F6EF0:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_826F6EF8:
	// li r3,25
	ctx.r3.s64 = 25;
	// blr 
	return;
loc_826F6F00:
	// addis r11,r3,-8192
	ctx.r11.s64 = ctx.r3.s64 + -536870912;
	// addi r11,r11,-76
	ctx.r11.s64 = ctx.r11.s64 + -76;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x826f6f64
	if (ctx.cr6.gt) goto loc_826F6F64;
	// lis r12,-32145
	ctx.r12.s64 = -2106654720;
	// addi r12,r12,28456
	ctx.r12.s64 = ctx.r12.s64 + 28456;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_826F6F3C;
	case 1:
		goto loc_826F6F44;
	case 2:
		goto loc_826F6F4C;
	case 3:
		goto loc_826F6F54;
	case 4:
		goto loc_826F6F5C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_826F6F3C:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_826F6F44:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_826F6F4C:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_826F6F54:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_826F6F5C:
	// li r3,17
	ctx.r3.s64 = 17;
	// blr 
	return;
loc_826F6F64:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

