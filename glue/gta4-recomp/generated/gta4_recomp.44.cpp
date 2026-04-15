#include "gta4_init.h"

PPC_FUNC_IMPL(__imp__sub_827345F8);
PPC_WEAK_FUNC(sub_827345F8) { __imp__sub_827345F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827345F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-8072
	ctx.r3.s64 = ctx.r11.s64 + -8072;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734608);
PPC_WEAK_FUNC(sub_82734608) { __imp__sub_82734608(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734608) {
	PPC_FUNC_PROLOGUE();
	// li r3,165
	ctx.r3.s64 = 165;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734610);
PPC_WEAK_FUNC(sub_82734610) { __imp__sub_82734610(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734610) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-8040
	ctx.r3.s64 = ctx.r11.s64 + -8040;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734620);
PPC_WEAK_FUNC(sub_82734620) { __imp__sub_82734620(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734620) {
	PPC_FUNC_PROLOGUE();
	// li r3,166
	ctx.r3.s64 = 166;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734628);
PPC_WEAK_FUNC(sub_82734628) { __imp__sub_82734628(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734628) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x827346f4
	if (ctx.cr6.gt) goto loc_827346F4;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x827346e4
	if (ctx.cr6.eq) goto loc_827346E4;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,31
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 31, ctx.xer);
	// bgt cr6,0x827346f4
	if (ctx.cr6.gt) goto loc_827346F4;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,18020
	ctx.r12.s64 = ctx.r12.s64 + 18020;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_827346E4;
	case 1:
		goto loc_827346E4;
	case 2:
		goto loc_827346E4;
	case 3:
		goto loc_827346E4;
	case 4:
		goto loc_827346E4;
	case 5:
		goto loc_827346E4;
	case 6:
		goto loc_827346E4;
	case 7:
		goto loc_827346E4;
	case 8:
		goto loc_827346F4;
	case 9:
		goto loc_827346E4;
	case 10:
		goto loc_827346E4;
	case 11:
		goto loc_827346E4;
	case 12:
		goto loc_827346E4;
	case 13:
		goto loc_827346E4;
	case 14:
		goto loc_827346E4;
	case 15:
		goto loc_827346E4;
	case 16:
		goto loc_827346F4;
	case 17:
		goto loc_827346F4;
	case 18:
		goto loc_827346E4;
	case 19:
		goto loc_827346E4;
	case 20:
		goto loc_827346E4;
	case 21:
		goto loc_827346EC;
	case 22:
		goto loc_827346F4;
	case 23:
		goto loc_827346F4;
	case 24:
		goto loc_827346F4;
	case 25:
		goto loc_827346E4;
	case 26:
		goto loc_827346E4;
	case 27:
		goto loc_827346E4;
	case 28:
		goto loc_827346F4;
	case 29:
		goto loc_827346E4;
	case 30:
		goto loc_827346E4;
	case 31:
		goto loc_827346E4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_827346E4:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_827346EC:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_827346F4:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734700);
PPC_WEAK_FUNC(sub_82734700) { __imp__sub_82734700(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734700) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-8008
	ctx.r3.s64 = ctx.r11.s64 + -8008;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734710);
PPC_WEAK_FUNC(sub_82734710) { __imp__sub_82734710(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734710) {
	PPC_FUNC_PROLOGUE();
	// li r3,168
	ctx.r3.s64 = 168;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734718);
PPC_WEAK_FUNC(sub_82734718) { __imp__sub_82734718(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734718) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,25
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 25, ctx.xer);
	// bgt cr6,0x82734874
	if (ctx.cr6.gt) goto loc_82734874;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,18232
	ctx.r12.s64 = ctx.r12.s64 + 18232;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_827347A0;
	case 1:
		goto loc_827347A8;
	case 2:
		goto loc_827347B0;
	case 3:
		goto loc_827347B8;
	case 4:
		goto loc_827347C0;
	case 5:
		goto loc_827347C8;
	case 6:
		goto loc_827347D0;
	case 7:
		goto loc_827347D8;
	case 8:
		goto loc_827347E0;
	case 9:
		goto loc_827347E8;
	case 10:
		goto loc_827347F0;
	case 11:
		goto loc_827347F8;
	case 12:
		goto loc_82734800;
	case 13:
		goto loc_82734808;
	case 14:
		goto loc_82734810;
	case 15:
		goto loc_82734818;
	case 16:
		goto loc_82734820;
	case 17:
		goto loc_82734828;
	case 18:
		goto loc_82734830;
	case 19:
		goto loc_82734838;
	case 20:
		goto loc_82734840;
	case 21:
		goto loc_82734848;
	case 22:
		goto loc_82734850;
	case 23:
		goto loc_82734858;
	case 24:
		goto loc_82734860;
	case 25:
		goto loc_82734868;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_827347A0:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_827347A8:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_827347B0:
	// li r3,27
	ctx.r3.s64 = 27;
	// blr 
	return;
loc_827347B8:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_827347C0:
	// li r3,26
	ctx.r3.s64 = 26;
	// blr 
	return;
loc_827347C8:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_827347D0:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_827347D8:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_827347E0:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_827347E8:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_827347F0:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_827347F8:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_82734800:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_82734808:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_82734810:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_82734818:
	// li r3,19
	ctx.r3.s64 = 19;
	// blr 
	return;
loc_82734820:
	// li r3,20
	ctx.r3.s64 = 20;
	// blr 
	return;
loc_82734828:
	// li r3,21
	ctx.r3.s64 = 21;
	// blr 
	return;
loc_82734830:
	// li r3,22
	ctx.r3.s64 = 22;
	// blr 
	return;
loc_82734838:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_82734840:
	// li r3,28
	ctx.r3.s64 = 28;
	// blr 
	return;
loc_82734848:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_82734850:
	// li r3,30
	ctx.r3.s64 = 30;
	// blr 
	return;
loc_82734858:
	// li r3,31
	ctx.r3.s64 = 31;
	// blr 
	return;
loc_82734860:
	// li r3,32
	ctx.r3.s64 = 32;
	// blr 
	return;
loc_82734868:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_82734874:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734880);
PPC_WEAK_FUNC(sub_82734880) { __imp__sub_82734880(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734880) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7976
	ctx.r3.s64 = ctx.r11.s64 + -7976;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734890);
PPC_WEAK_FUNC(sub_82734890) { __imp__sub_82734890(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734890) {
	PPC_FUNC_PROLOGUE();
	// li r3,167
	ctx.r3.s64 = 167;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734898);
PPC_WEAK_FUNC(sub_82734898) { __imp__sub_82734898(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734898) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x82734718
	sub_82734718(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_827348A0);
PPC_WEAK_FUNC(sub_827348A0) { __imp__sub_827348A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827348A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7944
	ctx.r3.s64 = ctx.r11.s64 + -7944;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827348B0);
PPC_WEAK_FUNC(sub_827348B0) { __imp__sub_827348B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827348B0) {
	PPC_FUNC_PROLOGUE();
	// li r3,169
	ctx.r3.s64 = 169;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827348B8);
PPC_WEAK_FUNC(sub_827348B8) { __imp__sub_827348B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827348B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7912
	ctx.r3.s64 = ctx.r11.s64 + -7912;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827348C8);
PPC_WEAK_FUNC(sub_827348C8) { __imp__sub_827348C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827348C8) {
	PPC_FUNC_PROLOGUE();
	// li r3,170
	ctx.r3.s64 = 170;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827348D0);
PPC_WEAK_FUNC(sub_827348D0) { __imp__sub_827348D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827348D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7880
	ctx.r3.s64 = ctx.r11.s64 + -7880;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827348E0);
PPC_WEAK_FUNC(sub_827348E0) { __imp__sub_827348E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827348E0) {
	PPC_FUNC_PROLOGUE();
	// li r3,171
	ctx.r3.s64 = 171;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827348E8);
PPC_WEAK_FUNC(sub_827348E8) { __imp__sub_827348E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827348E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7848
	ctx.r3.s64 = ctx.r11.s64 + -7848;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827348F8);
PPC_WEAK_FUNC(sub_827348F8) { __imp__sub_827348F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827348F8) {
	PPC_FUNC_PROLOGUE();
	// li r3,172
	ctx.r3.s64 = 172;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734900);
PPC_WEAK_FUNC(sub_82734900) { __imp__sub_82734900(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734900) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7816
	ctx.r3.s64 = ctx.r11.s64 + -7816;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734910);
PPC_WEAK_FUNC(sub_82734910) { __imp__sub_82734910(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734910) {
	PPC_FUNC_PROLOGUE();
	// li r3,173
	ctx.r3.s64 = 173;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734918);
PPC_WEAK_FUNC(sub_82734918) { __imp__sub_82734918(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734918) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x82734aa4
	if (ctx.cr6.gt) goto loc_82734AA4;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x82734a9c
	if (ctx.cr6.eq) goto loc_82734A9C;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,31
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 31, ctx.xer);
	// bgt cr6,0x82734aa4
	if (ctx.cr6.gt) goto loc_82734AA4;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,18772
	ctx.r12.s64 = ctx.r12.s64 + 18772;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_827349EC;
	case 1:
		goto loc_82734A6C;
	case 2:
		goto loc_82734A7C;
	case 3:
		goto loc_827349D4;
	case 4:
		goto loc_827349DC;
	case 5:
		goto loc_827349FC;
	case 6:
		goto loc_82734A04;
	case 7:
		goto loc_82734A0C;
	case 8:
		goto loc_82734AA4;
	case 9:
		goto loc_82734A3C;
	case 10:
		goto loc_82734A44;
	case 11:
		goto loc_82734A14;
	case 12:
		goto loc_82734A1C;
	case 13:
		goto loc_82734A24;
	case 14:
		goto loc_82734A2C;
	case 15:
		goto loc_82734A34;
	case 16:
		goto loc_82734AA4;
	case 17:
		goto loc_82734AA4;
	case 18:
		goto loc_82734A4C;
	case 19:
		goto loc_82734A54;
	case 20:
		goto loc_82734A5C;
	case 21:
		goto loc_82734A64;
	case 22:
		goto loc_82734AA4;
	case 23:
		goto loc_82734AA4;
	case 24:
		goto loc_82734AA4;
	case 25:
		goto loc_827349F4;
	case 26:
		goto loc_827349E4;
	case 27:
		goto loc_82734A74;
	case 28:
		goto loc_82734AA4;
	case 29:
		goto loc_82734A84;
	case 30:
		goto loc_82734A8C;
	case 31:
		goto loc_82734A94;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_827349D4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_827349DC:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_827349E4:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_827349EC:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_827349F4:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_827349FC:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_82734A04:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_82734A0C:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_82734A14:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_82734A1C:
	// li r3,9
	ctx.r3.s64 = 9;
	// blr 
	return;
loc_82734A24:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_82734A2C:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_82734A34:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_82734A3C:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_82734A44:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_82734A4C:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_82734A54:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_82734A5C:
	// li r3,17
	ctx.r3.s64 = 17;
	// blr 
	return;
loc_82734A64:
	// li r3,18
	ctx.r3.s64 = 18;
	// blr 
	return;
loc_82734A6C:
	// li r3,19
	ctx.r3.s64 = 19;
	// blr 
	return;
loc_82734A74:
	// li r3,20
	ctx.r3.s64 = 20;
	// blr 
	return;
loc_82734A7C:
	// li r3,21
	ctx.r3.s64 = 21;
	// blr 
	return;
loc_82734A84:
	// li r3,22
	ctx.r3.s64 = 22;
	// blr 
	return;
loc_82734A8C:
	// li r3,23
	ctx.r3.s64 = 23;
	// blr 
	return;
loc_82734A94:
	// li r3,24
	ctx.r3.s64 = 24;
	// blr 
	return;
loc_82734A9C:
	// li r3,25
	ctx.r3.s64 = 25;
	// blr 
	return;
loc_82734AA4:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734AB0);
PPC_WEAK_FUNC(sub_82734AB0) { __imp__sub_82734AB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734AB0) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,25
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 25, ctx.xer);
	// bgt cr6,0x82734b48
	if (ctx.cr6.gt) goto loc_82734B48;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,19152
	ctx.r12.s64 = ctx.r12.s64 + 19152;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_82734B38;
	case 1:
		goto loc_82734B38;
	case 2:
		goto loc_82734B38;
	case 3:
		goto loc_82734B40;
	case 4:
		goto loc_82734B40;
	case 5:
		goto loc_82734B38;
	case 6:
		goto loc_82734B38;
	case 7:
		goto loc_82734B38;
	case 8:
		goto loc_82734B38;
	case 9:
		goto loc_82734B38;
	case 10:
		goto loc_82734B38;
	case 11:
		goto loc_82734B38;
	case 12:
		goto loc_82734B38;
	case 13:
		goto loc_82734B40;
	case 14:
		goto loc_82734B40;
	case 15:
		goto loc_82734B40;
	case 16:
		goto loc_82734B40;
	case 17:
		goto loc_82734B40;
	case 18:
		goto loc_82734B38;
	case 19:
		goto loc_82734B38;
	case 20:
		goto loc_82734B38;
	case 21:
		goto loc_82734B38;
	case 22:
		goto loc_82734B38;
	case 23:
		goto loc_82734B38;
	case 24:
		goto loc_82734B38;
	case 25:
		goto loc_82734B40;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_82734B38:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_82734B40:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_82734B48:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734B50);
PPC_WEAK_FUNC(sub_82734B50) { __imp__sub_82734B50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734B50) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7784
	ctx.r3.s64 = ctx.r11.s64 + -7784;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734B60);
PPC_WEAK_FUNC(sub_82734B60) { __imp__sub_82734B60(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734B60) {
	PPC_FUNC_PROLOGUE();
	// li r3,174
	ctx.r3.s64 = 174;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734B68);
PPC_WEAK_FUNC(sub_82734B68) { __imp__sub_82734B68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734B68) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x82734918
	sub_82734918(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734B70);
PPC_WEAK_FUNC(sub_82734B70) { __imp__sub_82734B70(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734B70) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x82734ab0
	sub_82734AB0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734B78);
PPC_WEAK_FUNC(sub_82734B78) { __imp__sub_82734B78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734B78) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7752
	ctx.r3.s64 = ctx.r11.s64 + -7752;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734B88);
PPC_WEAK_FUNC(sub_82734B88) { __imp__sub_82734B88(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734B88) {
	PPC_FUNC_PROLOGUE();
	// li r3,210
	ctx.r3.s64 = 210;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734B90);
PPC_WEAK_FUNC(sub_82734B90) { __imp__sub_82734B90(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734B90) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7720
	ctx.r3.s64 = ctx.r11.s64 + -7720;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734BA0);
PPC_WEAK_FUNC(sub_82734BA0) { __imp__sub_82734BA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734BA0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7688
	ctx.r3.s64 = ctx.r11.s64 + -7688;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734BB0);
PPC_WEAK_FUNC(sub_82734BB0) { __imp__sub_82734BB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734BB0) {
	PPC_FUNC_PROLOGUE();
	// li r3,212
	ctx.r3.s64 = 212;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734BB8);
PPC_WEAK_FUNC(sub_82734BB8) { __imp__sub_82734BB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734BB8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7656
	ctx.r3.s64 = ctx.r11.s64 + -7656;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734BC8);
PPC_WEAK_FUNC(sub_82734BC8) { __imp__sub_82734BC8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734BC8) {
	PPC_FUNC_PROLOGUE();
	// li r3,251
	ctx.r3.s64 = 251;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734BD0);
PPC_WEAK_FUNC(sub_82734BD0) { __imp__sub_82734BD0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734BD0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x82734c94
	if (ctx.cr6.gt) goto loc_82734C94;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x82734c8c
	if (ctx.cr6.eq) goto loc_82734C8C;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,31
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 31, ctx.xer);
	// bgt cr6,0x82734c94
	if (ctx.cr6.gt) goto loc_82734C94;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,19468
	ctx.r12.s64 = ctx.r12.s64 + 19468;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_82734C8C;
	case 1:
		goto loc_82734C8C;
	case 2:
		goto loc_82734C8C;
	case 3:
		goto loc_82734C8C;
	case 4:
		goto loc_82734C94;
	case 5:
		goto loc_82734C8C;
	case 6:
		goto loc_82734C8C;
	case 7:
		goto loc_82734C8C;
	case 8:
		goto loc_82734C94;
	case 9:
		goto loc_82734C8C;
	case 10:
		goto loc_82734C8C;
	case 11:
		goto loc_82734C8C;
	case 12:
		goto loc_82734C8C;
	case 13:
		goto loc_82734C8C;
	case 14:
		goto loc_82734C8C;
	case 15:
		goto loc_82734C8C;
	case 16:
		goto loc_82734C94;
	case 17:
		goto loc_82734C94;
	case 18:
		goto loc_82734C8C;
	case 19:
		goto loc_82734C8C;
	case 20:
		goto loc_82734C8C;
	case 21:
		goto loc_82734C8C;
	case 22:
		goto loc_82734C94;
	case 23:
		goto loc_82734C94;
	case 24:
		goto loc_82734C94;
	case 25:
		goto loc_82734C8C;
	case 26:
		goto loc_82734C94;
	case 27:
		goto loc_82734C8C;
	case 28:
		goto loc_82734C94;
	case 29:
		goto loc_82734C8C;
	case 30:
		goto loc_82734C8C;
	case 31:
		goto loc_82734C8C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_82734C8C:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_82734C94:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734CA0);
PPC_WEAK_FUNC(sub_82734CA0) { __imp__sub_82734CA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734CA0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7620
	ctx.r3.s64 = ctx.r11.s64 + -7620;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734CB0);
PPC_WEAK_FUNC(sub_82734CB0) { __imp__sub_82734CB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734CB0) {
	PPC_FUNC_PROLOGUE();
	// li r3,159
	ctx.r3.s64 = 159;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734CB8);
PPC_WEAK_FUNC(sub_82734CB8) { __imp__sub_82734CB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734CB8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7584
	ctx.r3.s64 = ctx.r11.s64 + -7584;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734CC8);
PPC_WEAK_FUNC(sub_82734CC8) { __imp__sub_82734CC8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734CC8) {
	PPC_FUNC_PROLOGUE();
	// li r3,160
	ctx.r3.s64 = 160;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734CD0);
PPC_WEAK_FUNC(sub_82734CD0) { __imp__sub_82734CD0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734CD0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7548
	ctx.r3.s64 = ctx.r11.s64 + -7548;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734CE0);
PPC_WEAK_FUNC(sub_82734CE0) { __imp__sub_82734CE0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734CE0) {
	PPC_FUNC_PROLOGUE();
	// li r3,180
	ctx.r3.s64 = 180;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734CE8);
PPC_WEAK_FUNC(sub_82734CE8) { __imp__sub_82734CE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734CE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7512
	ctx.r3.s64 = ctx.r11.s64 + -7512;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734CF8);
PPC_WEAK_FUNC(sub_82734CF8) { __imp__sub_82734CF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734CF8) {
	PPC_FUNC_PROLOGUE();
	// li r3,181
	ctx.r3.s64 = 181;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734D00);
PPC_WEAK_FUNC(sub_82734D00) { __imp__sub_82734D00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734D00) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,23
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 23, ctx.xer);
	// bgt cr6,0x82734e44
	if (ctx.cr6.gt) goto loc_82734E44;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,19744
	ctx.r12.s64 = ctx.r12.s64 + 19744;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_82734D80;
	case 1:
		goto loc_82734D88;
	case 2:
		goto loc_82734D90;
	case 3:
		goto loc_82734D98;
	case 4:
		goto loc_82734DA0;
	case 5:
		goto loc_82734DA8;
	case 6:
		goto loc_82734DB0;
	case 7:
		goto loc_82734DB8;
	case 8:
		goto loc_82734DC0;
	case 9:
		goto loc_82734DC8;
	case 10:
		goto loc_82734DD0;
	case 11:
		goto loc_82734DD8;
	case 12:
		goto loc_82734DE0;
	case 13:
		goto loc_82734DE8;
	case 14:
		goto loc_82734DF0;
	case 15:
		goto loc_82734DF8;
	case 16:
		goto loc_82734E00;
	case 17:
		goto loc_82734E08;
	case 18:
		goto loc_82734E10;
	case 19:
		goto loc_82734E18;
	case 20:
		goto loc_82734E20;
	case 21:
		goto loc_82734E28;
	case 22:
		goto loc_82734E30;
	case 23:
		goto loc_82734E38;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_82734D80:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_82734D88:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_82734D90:
	// li r3,26
	ctx.r3.s64 = 26;
	// blr 
	return;
loc_82734D98:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_82734DA0:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_82734DA8:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_82734DB0:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_82734DB8:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_82734DC0:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_82734DC8:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_82734DD0:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_82734DD8:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_82734DE0:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_82734DE8:
	// li r3,19
	ctx.r3.s64 = 19;
	// blr 
	return;
loc_82734DF0:
	// li r3,20
	ctx.r3.s64 = 20;
	// blr 
	return;
loc_82734DF8:
	// li r3,21
	ctx.r3.s64 = 21;
	// blr 
	return;
loc_82734E00:
	// li r3,22
	ctx.r3.s64 = 22;
	// blr 
	return;
loc_82734E08:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_82734E10:
	// li r3,28
	ctx.r3.s64 = 28;
	// blr 
	return;
loc_82734E18:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_82734E20:
	// li r3,30
	ctx.r3.s64 = 30;
	// blr 
	return;
loc_82734E28:
	// li r3,31
	ctx.r3.s64 = 31;
	// blr 
	return;
loc_82734E30:
	// li r3,32
	ctx.r3.s64 = 32;
	// blr 
	return;
loc_82734E38:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_82734E44:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734E50);
PPC_WEAK_FUNC(sub_82734E50) { __imp__sub_82734E50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734E50) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7476
	ctx.r3.s64 = ctx.r11.s64 + -7476;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734E60);
PPC_WEAK_FUNC(sub_82734E60) { __imp__sub_82734E60(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734E60) {
	PPC_FUNC_PROLOGUE();
	// li r3,183
	ctx.r3.s64 = 183;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734E68);
PPC_WEAK_FUNC(sub_82734E68) { __imp__sub_82734E68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734E68) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x82734d00
	sub_82734D00(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734E70);
PPC_WEAK_FUNC(sub_82734E70) { __imp__sub_82734E70(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734E70) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x82734fec
	if (ctx.cr6.gt) goto loc_82734FEC;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x82734fe4
	if (ctx.cr6.eq) goto loc_82734FE4;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,31
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 31, ctx.xer);
	// bgt cr6,0x82734fec
	if (ctx.cr6.gt) goto loc_82734FEC;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,20140
	ctx.r12.s64 = ctx.r12.s64 + 20140;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_82734F34;
	case 1:
		goto loc_82734FB4;
	case 2:
		goto loc_82734FC4;
	case 3:
		goto loc_82734F2C;
	case 4:
		goto loc_82734FEC;
	case 5:
		goto loc_82734F44;
	case 6:
		goto loc_82734F4C;
	case 7:
		goto loc_82734F54;
	case 8:
		goto loc_82734FEC;
	case 9:
		goto loc_82734F84;
	case 10:
		goto loc_82734F8C;
	case 11:
		goto loc_82734F5C;
	case 12:
		goto loc_82734F64;
	case 13:
		goto loc_82734F6C;
	case 14:
		goto loc_82734F74;
	case 15:
		goto loc_82734F7C;
	case 16:
		goto loc_82734FEC;
	case 17:
		goto loc_82734FEC;
	case 18:
		goto loc_82734F94;
	case 19:
		goto loc_82734F9C;
	case 20:
		goto loc_82734FA4;
	case 21:
		goto loc_82734FAC;
	case 22:
		goto loc_82734FEC;
	case 23:
		goto loc_82734FEC;
	case 24:
		goto loc_82734FEC;
	case 25:
		goto loc_82734F3C;
	case 26:
		goto loc_82734FEC;
	case 27:
		goto loc_82734FBC;
	case 28:
		goto loc_82734FEC;
	case 29:
		goto loc_82734FCC;
	case 30:
		goto loc_82734FD4;
	case 31:
		goto loc_82734FDC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_82734F2C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_82734F34:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_82734F3C:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_82734F44:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_82734F4C:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_82734F54:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_82734F5C:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_82734F64:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_82734F6C:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_82734F74:
	// li r3,9
	ctx.r3.s64 = 9;
	// blr 
	return;
loc_82734F7C:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_82734F84:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_82734F8C:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_82734F94:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_82734F9C:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_82734FA4:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_82734FAC:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_82734FB4:
	// li r3,17
	ctx.r3.s64 = 17;
	// blr 
	return;
loc_82734FBC:
	// li r3,18
	ctx.r3.s64 = 18;
	// blr 
	return;
loc_82734FC4:
	// li r3,19
	ctx.r3.s64 = 19;
	// blr 
	return;
loc_82734FCC:
	// li r3,20
	ctx.r3.s64 = 20;
	// blr 
	return;
loc_82734FD4:
	// li r3,21
	ctx.r3.s64 = 21;
	// blr 
	return;
loc_82734FDC:
	// li r3,22
	ctx.r3.s64 = 22;
	// blr 
	return;
loc_82734FE4:
	// li r3,23
	ctx.r3.s64 = 23;
	// blr 
	return;
loc_82734FEC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82734FF8);
PPC_WEAK_FUNC(sub_82734FF8) { __imp__sub_82734FF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82734FF8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7440
	ctx.r3.s64 = ctx.r11.s64 + -7440;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82735008);
PPC_WEAK_FUNC(sub_82735008) { __imp__sub_82735008(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735008) {
	PPC_FUNC_PROLOGUE();
	// li r3,184
	ctx.r3.s64 = 184;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82735010);
PPC_WEAK_FUNC(sub_82735010) { __imp__sub_82735010(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735010) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x82734e70
	sub_82734E70(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82735018);
PPC_WEAK_FUNC(sub_82735018) { __imp__sub_82735018(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735018) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7404
	ctx.r3.s64 = ctx.r11.s64 + -7404;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82735028);
PPC_WEAK_FUNC(sub_82735028) { __imp__sub_82735028(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735028) {
	PPC_FUNC_PROLOGUE();
	// li r3,185
	ctx.r3.s64 = 185;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82735030);
PPC_WEAK_FUNC(sub_82735030) { __imp__sub_82735030(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735030) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,23
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 23, ctx.xer);
	// bgt cr6,0x827350c0
	if (ctx.cr6.gt) goto loc_827350C0;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,20560
	ctx.r12.s64 = ctx.r12.s64 + 20560;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_827350B0;
	case 1:
		goto loc_827350B0;
	case 2:
		goto loc_827350B0;
	case 3:
		goto loc_827350B8;
	case 4:
		goto loc_827350B8;
	case 5:
		goto loc_827350B8;
	case 6:
		goto loc_827350B8;
	case 7:
		goto loc_827350B8;
	case 8:
		goto loc_827350B8;
	case 9:
		goto loc_827350B8;
	case 10:
		goto loc_827350B8;
	case 11:
		goto loc_827350B0;
	case 12:
		goto loc_827350B0;
	case 13:
		goto loc_827350B0;
	case 14:
		goto loc_827350B0;
	case 15:
		goto loc_827350B0;
	case 16:
		goto loc_827350B8;
	case 17:
		goto loc_827350B8;
	case 18:
		goto loc_827350B8;
	case 19:
		goto loc_827350B8;
	case 20:
		goto loc_827350B8;
	case 21:
		goto loc_827350B8;
	case 22:
		goto loc_827350B8;
	case 23:
		goto loc_827350B0;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_827350B0:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_827350B8:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_827350C0:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827350C8);
PPC_WEAK_FUNC(sub_827350C8) { __imp__sub_827350C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827350C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7368
	ctx.r3.s64 = ctx.r11.s64 + -7368;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827350D8);
PPC_WEAK_FUNC(sub_827350D8) { __imp__sub_827350D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827350D8) {
	PPC_FUNC_PROLOGUE();
	// li r3,186
	ctx.r3.s64 = 186;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827350E0);
PPC_WEAK_FUNC(sub_827350E0) { __imp__sub_827350E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827350E0) {
	PPC_FUNC_PROLOGUE();
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
	// bl 0x82735030
	ctx.lr = 0x827350F4;
	sub_82735030(ctx, base);
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x8273517c
	if (ctx.cr6.gt) goto loc_8273517C;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,20760
	ctx.r12.s64 = ctx.r12.s64 + 20760;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_8273512C;
	case 1:
		goto loc_82735140;
	case 2:
		goto loc_82735168;
	case 3:
		goto loc_8273517C;
	case 4:
		goto loc_82735154;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_8273512C:
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
loc_82735140:
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
loc_82735154:
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
loc_82735168:
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
loc_8273517C:
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

PPC_FUNC_IMPL(__imp__sub_82735190);
PPC_WEAK_FUNC(sub_82735190) { __imp__sub_82735190(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735190) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7332
	ctx.r3.s64 = ctx.r11.s64 + -7332;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827351A0);
PPC_WEAK_FUNC(sub_827351A0) { __imp__sub_827351A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827351A0) {
	PPC_FUNC_PROLOGUE();
	// li r3,187
	ctx.r3.s64 = 187;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827351A8);
PPC_WEAK_FUNC(sub_827351A8) { __imp__sub_827351A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827351A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7296
	ctx.r3.s64 = ctx.r11.s64 + -7296;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827351B8);
PPC_WEAK_FUNC(sub_827351B8) { __imp__sub_827351B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827351B8) {
	PPC_FUNC_PROLOGUE();
	// li r3,188
	ctx.r3.s64 = 188;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827351C0);
PPC_WEAK_FUNC(sub_827351C0) { __imp__sub_827351C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827351C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7260
	ctx.r3.s64 = ctx.r11.s64 + -7260;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827351D0);
PPC_WEAK_FUNC(sub_827351D0) { __imp__sub_827351D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827351D0) {
	PPC_FUNC_PROLOGUE();
	// li r3,189
	ctx.r3.s64 = 189;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827351D8);
PPC_WEAK_FUNC(sub_827351D8) { __imp__sub_827351D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827351D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7224
	ctx.r3.s64 = ctx.r11.s64 + -7224;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827351E8);
PPC_WEAK_FUNC(sub_827351E8) { __imp__sub_827351E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827351E8) {
	PPC_FUNC_PROLOGUE();
	// li r3,190
	ctx.r3.s64 = 190;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827351F0);
PPC_WEAK_FUNC(sub_827351F0) { __imp__sub_827351F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827351F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7188
	ctx.r3.s64 = ctx.r11.s64 + -7188;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82735200);
PPC_WEAK_FUNC(sub_82735200) { __imp__sub_82735200(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735200) {
	PPC_FUNC_PROLOGUE();
	// li r3,191
	ctx.r3.s64 = 191;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82735208);
PPC_WEAK_FUNC(sub_82735208) { __imp__sub_82735208(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735208) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x827352cc
	if (ctx.cr6.gt) goto loc_827352CC;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x827352c4
	if (ctx.cr6.eq) goto loc_827352C4;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,31
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 31, ctx.xer);
	// bgt cr6,0x827352cc
	if (ctx.cr6.gt) goto loc_827352CC;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,21060
	ctx.r12.s64 = ctx.r12.s64 + 21060;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_827352C4;
	case 1:
		goto loc_827352C4;
	case 2:
		goto loc_827352C4;
	case 3:
		goto loc_827352C4;
	case 4:
		goto loc_827352CC;
	case 5:
		goto loc_827352C4;
	case 6:
		goto loc_827352C4;
	case 7:
		goto loc_827352C4;
	case 8:
		goto loc_827352CC;
	case 9:
		goto loc_827352C4;
	case 10:
		goto loc_827352C4;
	case 11:
		goto loc_827352C4;
	case 12:
		goto loc_827352C4;
	case 13:
		goto loc_827352C4;
	case 14:
		goto loc_827352C4;
	case 15:
		goto loc_827352C4;
	case 16:
		goto loc_827352C4;
	case 17:
		goto loc_827352CC;
	case 18:
		goto loc_827352C4;
	case 19:
		goto loc_827352C4;
	case 20:
		goto loc_827352C4;
	case 21:
		goto loc_827352C4;
	case 22:
		goto loc_827352CC;
	case 23:
		goto loc_827352CC;
	case 24:
		goto loc_827352CC;
	case 25:
		goto loc_827352C4;
	case 26:
		goto loc_827352CC;
	case 27:
		goto loc_827352C4;
	case 28:
		goto loc_827352CC;
	case 29:
		goto loc_827352C4;
	case 30:
		goto loc_827352C4;
	case 31:
		goto loc_827352C4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_827352C4:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_827352CC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827352D8);
PPC_WEAK_FUNC(sub_827352D8) { __imp__sub_827352D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827352D8) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,24
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 24, ctx.xer);
	// bgt cr6,0x82735428
	if (ctx.cr6.gt) goto loc_82735428;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,21240
	ctx.r12.s64 = ctx.r12.s64 + 21240;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_8273535C;
	case 1:
		goto loc_82735364;
	case 2:
		goto loc_8273536C;
	case 3:
		goto loc_82735374;
	case 4:
		goto loc_8273537C;
	case 5:
		goto loc_82735384;
	case 6:
		goto loc_8273538C;
	case 7:
		goto loc_82735394;
	case 8:
		goto loc_8273539C;
	case 9:
		goto loc_827353A4;
	case 10:
		goto loc_827353AC;
	case 11:
		goto loc_827353B4;
	case 12:
		goto loc_827353BC;
	case 13:
		goto loc_827353C4;
	case 14:
		goto loc_827353CC;
	case 15:
		goto loc_827353D4;
	case 16:
		goto loc_827353DC;
	case 17:
		goto loc_827353E4;
	case 18:
		goto loc_827353EC;
	case 19:
		goto loc_827353F4;
	case 20:
		goto loc_827353FC;
	case 21:
		goto loc_82735404;
	case 22:
		goto loc_8273540C;
	case 23:
		goto loc_82735414;
	case 24:
		goto loc_8273541C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_8273535C:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_82735364:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_8273536C:
	// li r3,26
	ctx.r3.s64 = 26;
	// blr 
	return;
loc_82735374:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_8273537C:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_82735384:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_8273538C:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_82735394:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_8273539C:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_827353A4:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_827353AC:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_827353B4:
	// li r3,17
	ctx.r3.s64 = 17;
	// blr 
	return;
loc_827353BC:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_827353C4:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_827353CC:
	// li r3,19
	ctx.r3.s64 = 19;
	// blr 
	return;
loc_827353D4:
	// li r3,20
	ctx.r3.s64 = 20;
	// blr 
	return;
loc_827353DC:
	// li r3,21
	ctx.r3.s64 = 21;
	// blr 
	return;
loc_827353E4:
	// li r3,22
	ctx.r3.s64 = 22;
	// blr 
	return;
loc_827353EC:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_827353F4:
	// li r3,28
	ctx.r3.s64 = 28;
	// blr 
	return;
loc_827353FC:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_82735404:
	// li r3,30
	ctx.r3.s64 = 30;
	// blr 
	return;
loc_8273540C:
	// li r3,31
	ctx.r3.s64 = 31;
	// blr 
	return;
loc_82735414:
	// li r3,32
	ctx.r3.s64 = 32;
	// blr 
	return;
loc_8273541C:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_82735428:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82735430);
PPC_WEAK_FUNC(sub_82735430) { __imp__sub_82735430(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735430) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x827355b4
	if (ctx.cr6.gt) goto loc_827355B4;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x827355ac
	if (ctx.cr6.eq) goto loc_827355AC;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,31
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 31, ctx.xer);
	// bgt cr6,0x827355b4
	if (ctx.cr6.gt) goto loc_827355B4;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,21612
	ctx.r12.s64 = ctx.r12.s64 + 21612;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_827354F4;
	case 1:
		goto loc_8273557C;
	case 2:
		goto loc_8273558C;
	case 3:
		goto loc_827354EC;
	case 4:
		goto loc_827355B4;
	case 5:
		goto loc_82735504;
	case 6:
		goto loc_8273550C;
	case 7:
		goto loc_82735514;
	case 8:
		goto loc_827355B4;
	case 9:
		goto loc_8273554C;
	case 10:
		goto loc_82735554;
	case 11:
		goto loc_8273551C;
	case 12:
		goto loc_82735524;
	case 13:
		goto loc_8273552C;
	case 14:
		goto loc_82735534;
	case 15:
		goto loc_8273553C;
	case 16:
		goto loc_82735544;
	case 17:
		goto loc_827355B4;
	case 18:
		goto loc_8273555C;
	case 19:
		goto loc_82735564;
	case 20:
		goto loc_8273556C;
	case 21:
		goto loc_82735574;
	case 22:
		goto loc_827355B4;
	case 23:
		goto loc_827355B4;
	case 24:
		goto loc_827355B4;
	case 25:
		goto loc_827354FC;
	case 26:
		goto loc_827355B4;
	case 27:
		goto loc_82735584;
	case 28:
		goto loc_827355B4;
	case 29:
		goto loc_82735594;
	case 30:
		goto loc_8273559C;
	case 31:
		goto loc_827355A4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_827354EC:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_827354F4:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_827354FC:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_82735504:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_8273550C:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_82735514:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_8273551C:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_82735524:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_8273552C:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_82735534:
	// li r3,9
	ctx.r3.s64 = 9;
	// blr 
	return;
loc_8273553C:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_82735544:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_8273554C:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_82735554:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_8273555C:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_82735564:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_8273556C:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_82735574:
	// li r3,17
	ctx.r3.s64 = 17;
	// blr 
	return;
loc_8273557C:
	// li r3,18
	ctx.r3.s64 = 18;
	// blr 
	return;
loc_82735584:
	// li r3,19
	ctx.r3.s64 = 19;
	// blr 
	return;
loc_8273558C:
	// li r3,20
	ctx.r3.s64 = 20;
	// blr 
	return;
loc_82735594:
	// li r3,21
	ctx.r3.s64 = 21;
	// blr 
	return;
loc_8273559C:
	// li r3,22
	ctx.r3.s64 = 22;
	// blr 
	return;
loc_827355A4:
	// li r3,23
	ctx.r3.s64 = 23;
	// blr 
	return;
loc_827355AC:
	// li r3,24
	ctx.r3.s64 = 24;
	// blr 
	return;
loc_827355B4:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827355C0);
PPC_WEAK_FUNC(sub_827355C0) { __imp__sub_827355C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827355C0) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,24
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 24, ctx.xer);
	// bgt cr6,0x82735654
	if (ctx.cr6.gt) goto loc_82735654;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,21984
	ctx.r12.s64 = ctx.r12.s64 + 21984;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_82735644;
	case 1:
		goto loc_82735644;
	case 2:
		goto loc_82735644;
	case 3:
		goto loc_8273564C;
	case 4:
		goto loc_8273564C;
	case 5:
		goto loc_8273564C;
	case 6:
		goto loc_8273564C;
	case 7:
		goto loc_8273564C;
	case 8:
		goto loc_8273564C;
	case 9:
		goto loc_8273564C;
	case 10:
		goto loc_8273564C;
	case 11:
		goto loc_8273564C;
	case 12:
		goto loc_82735644;
	case 13:
		goto loc_82735644;
	case 14:
		goto loc_82735644;
	case 15:
		goto loc_82735644;
	case 16:
		goto loc_82735644;
	case 17:
		goto loc_8273564C;
	case 18:
		goto loc_8273564C;
	case 19:
		goto loc_8273564C;
	case 20:
		goto loc_8273564C;
	case 21:
		goto loc_8273564C;
	case 22:
		goto loc_8273564C;
	case 23:
		goto loc_8273564C;
	case 24:
		goto loc_82735644;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_82735644:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_8273564C:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_82735654:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82735660);
PPC_WEAK_FUNC(sub_82735660) { __imp__sub_82735660(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735660) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,24
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 24, ctx.xer);
	// bgt cr6,0x827356f4
	if (ctx.cr6.gt) goto loc_827356F4;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,22144
	ctx.r12.s64 = ctx.r12.s64 + 22144;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_827356E4;
	case 1:
		goto loc_827356E4;
	case 2:
		goto loc_827356E4;
	case 3:
		goto loc_827356EC;
	case 4:
		goto loc_827356EC;
	case 5:
		goto loc_827356EC;
	case 6:
		goto loc_827356EC;
	case 7:
		goto loc_827356EC;
	case 8:
		goto loc_827356EC;
	case 9:
		goto loc_827356EC;
	case 10:
		goto loc_827356EC;
	case 11:
		goto loc_827356EC;
	case 12:
		goto loc_827356E4;
	case 13:
		goto loc_827356E4;
	case 14:
		goto loc_827356E4;
	case 15:
		goto loc_827356E4;
	case 16:
		goto loc_827356E4;
	case 17:
		goto loc_827356EC;
	case 18:
		goto loc_827356EC;
	case 19:
		goto loc_827356EC;
	case 20:
		goto loc_827356EC;
	case 21:
		goto loc_827356EC;
	case 22:
		goto loc_827356EC;
	case 23:
		goto loc_827356EC;
	case 24:
		goto loc_827356E4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_827356E4:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_827356EC:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_827356F4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82735700);
PPC_WEAK_FUNC(sub_82735700) { __imp__sub_82735700(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735700) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7152
	ctx.r3.s64 = ctx.r11.s64 + -7152;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82735710);
PPC_WEAK_FUNC(sub_82735710) { __imp__sub_82735710(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735710) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x82735430
	sub_82735430(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82735718);
PPC_WEAK_FUNC(sub_82735718) { __imp__sub_82735718(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735718) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x827352d8
	sub_827352D8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82735720);
PPC_WEAK_FUNC(sub_82735720) { __imp__sub_82735720(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735720) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x82735660
	sub_82735660(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82735728);
PPC_WEAK_FUNC(sub_82735728) { __imp__sub_82735728(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735728) {
	PPC_FUNC_PROLOGUE();
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
	// bl 0x827355c0
	ctx.lr = 0x8273573C;
	sub_827355C0(ctx, base);
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x827357c4
	if (ctx.cr6.gt) goto loc_827357C4;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,22368
	ctx.r12.s64 = ctx.r12.s64 + 22368;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_82735774;
	case 1:
		goto loc_82735788;
	case 2:
		goto loc_827357B0;
	case 3:
		goto loc_827357C4;
	case 4:
		goto loc_8273579C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_82735774:
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
loc_82735788:
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
loc_8273579C:
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
loc_827357B0:
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
loc_827357C4:
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

PPC_FUNC_IMPL(__imp__sub_827357D8);
PPC_WEAK_FUNC(sub_827357D8) { __imp__sub_827357D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827357D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7116
	ctx.r3.s64 = ctx.r11.s64 + -7116;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827357E8);
PPC_WEAK_FUNC(sub_827357E8) { __imp__sub_827357E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827357E8) {
	PPC_FUNC_PROLOGUE();
	// li r3,193
	ctx.r3.s64 = 193;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827357F0);
PPC_WEAK_FUNC(sub_827357F0) { __imp__sub_827357F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827357F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x827358b4
	if (ctx.cr6.gt) goto loc_827358B4;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x827358ac
	if (ctx.cr6.eq) goto loc_827358AC;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,31
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 31, ctx.xer);
	// bgt cr6,0x827358b4
	if (ctx.cr6.gt) goto loc_827358B4;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,22572
	ctx.r12.s64 = ctx.r12.s64 + 22572;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_827358AC;
	case 1:
		goto loc_827358AC;
	case 2:
		goto loc_827358AC;
	case 3:
		goto loc_827358AC;
	case 4:
		goto loc_827358B4;
	case 5:
		goto loc_827358AC;
	case 6:
		goto loc_827358AC;
	case 7:
		goto loc_827358AC;
	case 8:
		goto loc_827358B4;
	case 9:
		goto loc_827358AC;
	case 10:
		goto loc_827358AC;
	case 11:
		goto loc_827358AC;
	case 12:
		goto loc_827358AC;
	case 13:
		goto loc_827358AC;
	case 14:
		goto loc_827358AC;
	case 15:
		goto loc_827358B4;
	case 16:
		goto loc_827358B4;
	case 17:
		goto loc_827358B4;
	case 18:
		goto loc_827358AC;
	case 19:
		goto loc_827358AC;
	case 20:
		goto loc_827358AC;
	case 21:
		goto loc_827358AC;
	case 22:
		goto loc_827358B4;
	case 23:
		goto loc_827358B4;
	case 24:
		goto loc_827358B4;
	case 25:
		goto loc_827358AC;
	case 26:
		goto loc_827358B4;
	case 27:
		goto loc_827358AC;
	case 28:
		goto loc_827358B4;
	case 29:
		goto loc_827358AC;
	case 30:
		goto loc_827358AC;
	case 31:
		goto loc_827358AC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_827358AC:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_827358B4:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827358C0);
PPC_WEAK_FUNC(sub_827358C0) { __imp__sub_827358C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827358C0) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,22
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 22, ctx.xer);
	// bgt cr6,0x827359f8
	if (ctx.cr6.gt) goto loc_827359F8;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,22752
	ctx.r12.s64 = ctx.r12.s64 + 22752;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_8273593C;
	case 1:
		goto loc_82735944;
	case 2:
		goto loc_8273594C;
	case 3:
		goto loc_82735954;
	case 4:
		goto loc_8273595C;
	case 5:
		goto loc_82735964;
	case 6:
		goto loc_8273596C;
	case 7:
		goto loc_82735974;
	case 8:
		goto loc_8273597C;
	case 9:
		goto loc_82735984;
	case 10:
		goto loc_8273598C;
	case 11:
		goto loc_82735994;
	case 12:
		goto loc_8273599C;
	case 13:
		goto loc_827359A4;
	case 14:
		goto loc_827359AC;
	case 15:
		goto loc_827359B4;
	case 16:
		goto loc_827359BC;
	case 17:
		goto loc_827359C4;
	case 18:
		goto loc_827359CC;
	case 19:
		goto loc_827359D4;
	case 20:
		goto loc_827359DC;
	case 21:
		goto loc_827359E4;
	case 22:
		goto loc_827359EC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_8273593C:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_82735944:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_8273594C:
	// li r3,26
	ctx.r3.s64 = 26;
	// blr 
	return;
loc_82735954:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_8273595C:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_82735964:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_8273596C:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_82735974:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_8273597C:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_82735984:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_8273598C:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_82735994:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_8273599C:
	// li r3,19
	ctx.r3.s64 = 19;
	// blr 
	return;
loc_827359A4:
	// li r3,20
	ctx.r3.s64 = 20;
	// blr 
	return;
loc_827359AC:
	// li r3,21
	ctx.r3.s64 = 21;
	// blr 
	return;
loc_827359B4:
	// li r3,22
	ctx.r3.s64 = 22;
	// blr 
	return;
loc_827359BC:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_827359C4:
	// li r3,28
	ctx.r3.s64 = 28;
	// blr 
	return;
loc_827359CC:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_827359D4:
	// li r3,30
	ctx.r3.s64 = 30;
	// blr 
	return;
loc_827359DC:
	// li r3,31
	ctx.r3.s64 = 31;
	// blr 
	return;
loc_827359E4:
	// li r3,32
	ctx.r3.s64 = 32;
	// blr 
	return;
loc_827359EC:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_827359F8:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82735A00);
PPC_WEAK_FUNC(sub_82735A00) { __imp__sub_82735A00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735A00) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x82735b74
	if (ctx.cr6.gt) goto loc_82735B74;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x82735b6c
	if (ctx.cr6.eq) goto loc_82735B6C;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,31
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 31, ctx.xer);
	// bgt cr6,0x82735b74
	if (ctx.cr6.gt) goto loc_82735B74;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,23100
	ctx.r12.s64 = ctx.r12.s64 + 23100;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_82735AC4;
	case 1:
		goto loc_82735B3C;
	case 2:
		goto loc_82735B4C;
	case 3:
		goto loc_82735ABC;
	case 4:
		goto loc_82735B74;
	case 5:
		goto loc_82735AD4;
	case 6:
		goto loc_82735ADC;
	case 7:
		goto loc_82735AE4;
	case 8:
		goto loc_82735B74;
	case 9:
		goto loc_82735B0C;
	case 10:
		goto loc_82735B14;
	case 11:
		goto loc_82735AEC;
	case 12:
		goto loc_82735AF4;
	case 13:
		goto loc_82735AFC;
	case 14:
		goto loc_82735B04;
	case 15:
		goto loc_82735B74;
	case 16:
		goto loc_82735B74;
	case 17:
		goto loc_82735B74;
	case 18:
		goto loc_82735B1C;
	case 19:
		goto loc_82735B24;
	case 20:
		goto loc_82735B2C;
	case 21:
		goto loc_82735B34;
	case 22:
		goto loc_82735B74;
	case 23:
		goto loc_82735B74;
	case 24:
		goto loc_82735B74;
	case 25:
		goto loc_82735ACC;
	case 26:
		goto loc_82735B74;
	case 27:
		goto loc_82735B44;
	case 28:
		goto loc_82735B74;
	case 29:
		goto loc_82735B54;
	case 30:
		goto loc_82735B5C;
	case 31:
		goto loc_82735B64;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_82735ABC:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_82735AC4:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_82735ACC:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_82735AD4:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_82735ADC:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_82735AE4:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_82735AEC:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_82735AF4:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_82735AFC:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_82735B04:
	// li r3,9
	ctx.r3.s64 = 9;
	// blr 
	return;
loc_82735B0C:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_82735B14:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_82735B1C:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_82735B24:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_82735B2C:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_82735B34:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_82735B3C:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_82735B44:
	// li r3,17
	ctx.r3.s64 = 17;
	// blr 
	return;
loc_82735B4C:
	// li r3,18
	ctx.r3.s64 = 18;
	// blr 
	return;
loc_82735B54:
	// li r3,19
	ctx.r3.s64 = 19;
	// blr 
	return;
loc_82735B5C:
	// li r3,20
	ctx.r3.s64 = 20;
	// blr 
	return;
loc_82735B64:
	// li r3,21
	ctx.r3.s64 = 21;
	// blr 
	return;
loc_82735B6C:
	// li r3,22
	ctx.r3.s64 = 22;
	// blr 
	return;
loc_82735B74:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82735B80);
PPC_WEAK_FUNC(sub_82735B80) { __imp__sub_82735B80(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735B80) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,22
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 22, ctx.xer);
	// bgt cr6,0x82735c0c
	if (ctx.cr6.gt) goto loc_82735C0C;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,23456
	ctx.r12.s64 = ctx.r12.s64 + 23456;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_82735BFC;
	case 1:
		goto loc_82735BFC;
	case 2:
		goto loc_82735BFC;
	case 3:
		goto loc_82735C04;
	case 4:
		goto loc_82735C04;
	case 5:
		goto loc_82735C04;
	case 6:
		goto loc_82735C04;
	case 7:
		goto loc_82735C04;
	case 8:
		goto loc_82735C04;
	case 9:
		goto loc_82735C04;
	case 10:
		goto loc_82735BFC;
	case 11:
		goto loc_82735BFC;
	case 12:
		goto loc_82735BFC;
	case 13:
		goto loc_82735BFC;
	case 14:
		goto loc_82735BFC;
	case 15:
		goto loc_82735C04;
	case 16:
		goto loc_82735C04;
	case 17:
		goto loc_82735C04;
	case 18:
		goto loc_82735C04;
	case 19:
		goto loc_82735C04;
	case 20:
		goto loc_82735C04;
	case 21:
		goto loc_82735C04;
	case 22:
		goto loc_82735BFC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_82735BFC:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_82735C04:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_82735C0C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82735C18);
PPC_WEAK_FUNC(sub_82735C18) { __imp__sub_82735C18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735C18) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,22
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 22, ctx.xer);
	// bgt cr6,0x82735ca4
	if (ctx.cr6.gt) goto loc_82735CA4;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,23608
	ctx.r12.s64 = ctx.r12.s64 + 23608;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_82735C94;
	case 1:
		goto loc_82735C94;
	case 2:
		goto loc_82735C94;
	case 3:
		goto loc_82735C9C;
	case 4:
		goto loc_82735C9C;
	case 5:
		goto loc_82735C9C;
	case 6:
		goto loc_82735C9C;
	case 7:
		goto loc_82735C9C;
	case 8:
		goto loc_82735C9C;
	case 9:
		goto loc_82735C9C;
	case 10:
		goto loc_82735C94;
	case 11:
		goto loc_82735C94;
	case 12:
		goto loc_82735C94;
	case 13:
		goto loc_82735C94;
	case 14:
		goto loc_82735C94;
	case 15:
		goto loc_82735C9C;
	case 16:
		goto loc_82735C9C;
	case 17:
		goto loc_82735C9C;
	case 18:
		goto loc_82735C9C;
	case 19:
		goto loc_82735C9C;
	case 20:
		goto loc_82735C9C;
	case 21:
		goto loc_82735C9C;
	case 22:
		goto loc_82735C94;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_82735C94:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_82735C9C:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_82735CA4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82735CB0);
PPC_WEAK_FUNC(sub_82735CB0) { __imp__sub_82735CB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735CB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7080
	ctx.r3.s64 = ctx.r11.s64 + -7080;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82735CC0);
PPC_WEAK_FUNC(sub_82735CC0) { __imp__sub_82735CC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735CC0) {
	PPC_FUNC_PROLOGUE();
	// li r3,199
	ctx.r3.s64 = 199;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82735CC8);
PPC_WEAK_FUNC(sub_82735CC8) { __imp__sub_82735CC8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735CC8) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x82735a00
	sub_82735A00(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82735CD0);
PPC_WEAK_FUNC(sub_82735CD0) { __imp__sub_82735CD0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735CD0) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x827358c0
	sub_827358C0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82735CD8);
PPC_WEAK_FUNC(sub_82735CD8) { __imp__sub_82735CD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735CD8) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x82735c18
	sub_82735C18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82735CE0);
PPC_WEAK_FUNC(sub_82735CE0) { __imp__sub_82735CE0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735CE0) {
	PPC_FUNC_PROLOGUE();
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
	// bl 0x82735b80
	ctx.lr = 0x82735CF4;
	sub_82735B80(ctx, base);
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x82735d7c
	if (ctx.cr6.gt) goto loc_82735D7C;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,23832
	ctx.r12.s64 = ctx.r12.s64 + 23832;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_82735D2C;
	case 1:
		goto loc_82735D40;
	case 2:
		goto loc_82735D68;
	case 3:
		goto loc_82735D7C;
	case 4:
		goto loc_82735D54;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_82735D2C:
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
loc_82735D40:
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
loc_82735D54:
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
loc_82735D68:
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
loc_82735D7C:
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

PPC_FUNC_IMPL(__imp__sub_82735D90);
PPC_WEAK_FUNC(sub_82735D90) { __imp__sub_82735D90(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735D90) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7044
	ctx.r3.s64 = ctx.r11.s64 + -7044;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82735DA0);
PPC_WEAK_FUNC(sub_82735DA0) { __imp__sub_82735DA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735DA0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-7008
	ctx.r3.s64 = ctx.r11.s64 + -7008;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82735DB0);
PPC_WEAK_FUNC(sub_82735DB0) { __imp__sub_82735DB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735DB0) {
	PPC_FUNC_PROLOGUE();
	// li r3,201
	ctx.r3.s64 = 201;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82735DB8);
PPC_WEAK_FUNC(sub_82735DB8) { __imp__sub_82735DB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735DB8) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,23
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 23, ctx.xer);
	// bgt cr6,0x82735efc
	if (ctx.cr6.gt) goto loc_82735EFC;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,24024
	ctx.r12.s64 = ctx.r12.s64 + 24024;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_82735E38;
	case 1:
		goto loc_82735E40;
	case 2:
		goto loc_82735E48;
	case 3:
		goto loc_82735E50;
	case 4:
		goto loc_82735E58;
	case 5:
		goto loc_82735E60;
	case 6:
		goto loc_82735E68;
	case 7:
		goto loc_82735E70;
	case 8:
		goto loc_82735E78;
	case 9:
		goto loc_82735E80;
	case 10:
		goto loc_82735E88;
	case 11:
		goto loc_82735E90;
	case 12:
		goto loc_82735E98;
	case 13:
		goto loc_82735EA0;
	case 14:
		goto loc_82735EA8;
	case 15:
		goto loc_82735EB0;
	case 16:
		goto loc_82735EB8;
	case 17:
		goto loc_82735EC0;
	case 18:
		goto loc_82735EC8;
	case 19:
		goto loc_82735ED0;
	case 20:
		goto loc_82735ED8;
	case 21:
		goto loc_82735EE0;
	case 22:
		goto loc_82735EE8;
	case 23:
		goto loc_82735EF0;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_82735E38:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_82735E40:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_82735E48:
	// li r3,26
	ctx.r3.s64 = 26;
	// blr 
	return;
loc_82735E50:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_82735E58:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_82735E60:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_82735E68:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_82735E70:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_82735E78:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_82735E80:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_82735E88:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_82735E90:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_82735E98:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_82735EA0:
	// li r3,19
	ctx.r3.s64 = 19;
	// blr 
	return;
loc_82735EA8:
	// li r3,20
	ctx.r3.s64 = 20;
	// blr 
	return;
loc_82735EB0:
	// li r3,21
	ctx.r3.s64 = 21;
	// blr 
	return;
loc_82735EB8:
	// li r3,25
	ctx.r3.s64 = 25;
	// blr 
	return;
loc_82735EC0:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_82735EC8:
	// li r3,28
	ctx.r3.s64 = 28;
	// blr 
	return;
loc_82735ED0:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_82735ED8:
	// li r3,30
	ctx.r3.s64 = 30;
	// blr 
	return;
loc_82735EE0:
	// li r3,31
	ctx.r3.s64 = 31;
	// blr 
	return;
loc_82735EE8:
	// li r3,32
	ctx.r3.s64 = 32;
	// blr 
	return;
loc_82735EF0:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_82735EFC:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82735F08);
PPC_WEAK_FUNC(sub_82735F08) { __imp__sub_82735F08(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82735F08) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x82736084
	if (ctx.cr6.gt) goto loc_82736084;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x8273607c
	if (ctx.cr6.eq) goto loc_8273607C;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,31
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 31, ctx.xer);
	// bgt cr6,0x82736084
	if (ctx.cr6.gt) goto loc_82736084;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,24388
	ctx.r12.s64 = ctx.r12.s64 + 24388;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_82735FCC;
	case 1:
		goto loc_8273604C;
	case 2:
		goto loc_8273605C;
	case 3:
		goto loc_82735FC4;
	case 4:
		goto loc_82736084;
	case 5:
		goto loc_82735FDC;
	case 6:
		goto loc_82735FE4;
	case 7:
		goto loc_82735FEC;
	case 8:
		goto loc_82736084;
	case 9:
		goto loc_8273601C;
	case 10:
		goto loc_82736024;
	case 11:
		goto loc_82735FF4;
	case 12:
		goto loc_82735FFC;
	case 13:
		goto loc_82736004;
	case 14:
		goto loc_8273600C;
	case 15:
		goto loc_82736014;
	case 16:
		goto loc_82736084;
	case 17:
		goto loc_82736084;
	case 18:
		goto loc_8273602C;
	case 19:
		goto loc_82736034;
	case 20:
		goto loc_8273603C;
	case 21:
		goto loc_82736084;
	case 22:
		goto loc_82736084;
	case 23:
		goto loc_82736084;
	case 24:
		goto loc_82736044;
	case 25:
		goto loc_82735FD4;
	case 26:
		goto loc_82736084;
	case 27:
		goto loc_82736054;
	case 28:
		goto loc_82736084;
	case 29:
		goto loc_82736064;
	case 30:
		goto loc_8273606C;
	case 31:
		goto loc_82736074;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_82735FC4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_82735FCC:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_82735FD4:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_82735FDC:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_82735FE4:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_82735FEC:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_82735FF4:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_82735FFC:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_82736004:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_8273600C:
	// li r3,9
	ctx.r3.s64 = 9;
	// blr 
	return;
loc_82736014:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_8273601C:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_82736024:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_8273602C:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_82736034:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_8273603C:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_82736044:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_8273604C:
	// li r3,17
	ctx.r3.s64 = 17;
	// blr 
	return;
loc_82736054:
	// li r3,18
	ctx.r3.s64 = 18;
	// blr 
	return;
loc_8273605C:
	// li r3,19
	ctx.r3.s64 = 19;
	// blr 
	return;
loc_82736064:
	// li r3,20
	ctx.r3.s64 = 20;
	// blr 
	return;
loc_8273606C:
	// li r3,21
	ctx.r3.s64 = 21;
	// blr 
	return;
loc_82736074:
	// li r3,22
	ctx.r3.s64 = 22;
	// blr 
	return;
loc_8273607C:
	// li r3,23
	ctx.r3.s64 = 23;
	// blr 
	return;
loc_82736084:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736090);
PPC_WEAK_FUNC(sub_82736090) { __imp__sub_82736090(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736090) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6972
	ctx.r3.s64 = ctx.r11.s64 + -6972;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827360A0);
PPC_WEAK_FUNC(sub_827360A0) { __imp__sub_827360A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827360A0) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x82735f08
	sub_82735F08(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_827360A8);
PPC_WEAK_FUNC(sub_827360A8) { __imp__sub_827360A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827360A8) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x82735db8
	sub_82735DB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_827360B0);
PPC_WEAK_FUNC(sub_827360B0) { __imp__sub_827360B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827360B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6936
	ctx.r3.s64 = ctx.r11.s64 + -6936;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827360C0);
PPC_WEAK_FUNC(sub_827360C0) { __imp__sub_827360C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827360C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6900
	ctx.r3.s64 = ctx.r11.s64 + -6900;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827360D0);
PPC_WEAK_FUNC(sub_827360D0) { __imp__sub_827360D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827360D0) {
	PPC_FUNC_PROLOGUE();
	// li r3,204
	ctx.r3.s64 = 204;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827360D8);
PPC_WEAK_FUNC(sub_827360D8) { __imp__sub_827360D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827360D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6864
	ctx.r3.s64 = ctx.r11.s64 + -6864;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827360E8);
PPC_WEAK_FUNC(sub_827360E8) { __imp__sub_827360E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827360E8) {
	PPC_FUNC_PROLOGUE();
	// li r3,205
	ctx.r3.s64 = 205;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827360F0);
PPC_WEAK_FUNC(sub_827360F0) { __imp__sub_827360F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827360F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x827361b4
	if (ctx.cr6.gt) goto loc_827361B4;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x827361ac
	if (ctx.cr6.eq) goto loc_827361AC;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,31
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 31, ctx.xer);
	// bgt cr6,0x827361b4
	if (ctx.cr6.gt) goto loc_827361B4;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,24876
	ctx.r12.s64 = ctx.r12.s64 + 24876;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_827361AC;
	case 1:
		goto loc_827361AC;
	case 2:
		goto loc_827361AC;
	case 3:
		goto loc_827361AC;
	case 4:
		goto loc_827361B4;
	case 5:
		goto loc_827361AC;
	case 6:
		goto loc_827361AC;
	case 7:
		goto loc_827361AC;
	case 8:
		goto loc_827361B4;
	case 9:
		goto loc_827361AC;
	case 10:
		goto loc_827361AC;
	case 11:
		goto loc_827361AC;
	case 12:
		goto loc_827361AC;
	case 13:
		goto loc_827361AC;
	case 14:
		goto loc_827361AC;
	case 15:
		goto loc_827361AC;
	case 16:
		goto loc_827361B4;
	case 17:
		goto loc_827361B4;
	case 18:
		goto loc_827361AC;
	case 19:
		goto loc_827361AC;
	case 20:
		goto loc_827361AC;
	case 21:
		goto loc_827361B4;
	case 22:
		goto loc_827361B4;
	case 23:
		goto loc_827361B4;
	case 24:
		goto loc_827361AC;
	case 25:
		goto loc_827361AC;
	case 26:
		goto loc_827361B4;
	case 27:
		goto loc_827361AC;
	case 28:
		goto loc_827361B4;
	case 29:
		goto loc_827361AC;
	case 30:
		goto loc_827361AC;
	case 31:
		goto loc_827361AC;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_827361AC:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_827361B4:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827361C0);
PPC_WEAK_FUNC(sub_827361C0) { __imp__sub_827361C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827361C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6828
	ctx.r3.s64 = ctx.r11.s64 + -6828;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827361D0);
PPC_WEAK_FUNC(sub_827361D0) { __imp__sub_827361D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827361D0) {
	PPC_FUNC_PROLOGUE();
	// li r3,206
	ctx.r3.s64 = 206;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827361D8);
PPC_WEAK_FUNC(sub_827361D8) { __imp__sub_827361D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827361D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6792
	ctx.r3.s64 = ctx.r11.s64 + -6792;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827361E8);
PPC_WEAK_FUNC(sub_827361E8) { __imp__sub_827361E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827361E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6756
	ctx.r3.s64 = ctx.r11.s64 + -6756;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827361F8);
PPC_WEAK_FUNC(sub_827361F8) { __imp__sub_827361F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827361F8) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,23
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 23, ctx.xer);
	// bgt cr6,0x82736288
	if (ctx.cr6.gt) goto loc_82736288;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,25112
	ctx.r12.s64 = ctx.r12.s64 + 25112;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_82736278;
	case 1:
		goto loc_82736278;
	case 2:
		goto loc_82736278;
	case 3:
		goto loc_82736280;
	case 4:
		goto loc_82736280;
	case 5:
		goto loc_82736280;
	case 6:
		goto loc_82736280;
	case 7:
		goto loc_82736280;
	case 8:
		goto loc_82736280;
	case 9:
		goto loc_82736280;
	case 10:
		goto loc_82736280;
	case 11:
		goto loc_82736278;
	case 12:
		goto loc_82736278;
	case 13:
		goto loc_82736278;
	case 14:
		goto loc_82736278;
	case 15:
		goto loc_82736278;
	case 16:
		goto loc_82736280;
	case 17:
		goto loc_82736280;
	case 18:
		goto loc_82736280;
	case 19:
		goto loc_82736280;
	case 20:
		goto loc_82736280;
	case 21:
		goto loc_82736280;
	case 22:
		goto loc_82736280;
	case 23:
		goto loc_82736278;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_82736278:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_82736280:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_82736288:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736290);
PPC_WEAK_FUNC(sub_82736290) { __imp__sub_82736290(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736290) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6720
	ctx.r3.s64 = ctx.r11.s64 + -6720;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827362A0);
PPC_WEAK_FUNC(sub_827362A0) { __imp__sub_827362A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827362A0) {
	PPC_FUNC_PROLOGUE();
	// li r3,182
	ctx.r3.s64 = 182;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827362A8);
PPC_WEAK_FUNC(sub_827362A8) { __imp__sub_827362A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827362A8) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x827361f8
	sub_827361F8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_827362B0);
PPC_WEAK_FUNC(sub_827362B0) { __imp__sub_827362B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827362B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6684
	ctx.r3.s64 = ctx.r11.s64 + -6684;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827362C0);
PPC_WEAK_FUNC(sub_827362C0) { __imp__sub_827362C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827362C0) {
	PPC_FUNC_PROLOGUE();
	// li r3,175
	ctx.r3.s64 = 175;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827362C8);
PPC_WEAK_FUNC(sub_827362C8) { __imp__sub_827362C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827362C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6648
	ctx.r3.s64 = ctx.r11.s64 + -6648;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827362D8);
PPC_WEAK_FUNC(sub_827362D8) { __imp__sub_827362D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827362D8) {
	PPC_FUNC_PROLOGUE();
	// li r3,176
	ctx.r3.s64 = 176;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827362E0);
PPC_WEAK_FUNC(sub_827362E0) { __imp__sub_827362E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827362E0) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,18
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 18, ctx.xer);
	// bgt cr6,0x8273635c
	if (ctx.cr6.gt) goto loc_8273635C;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,25344
	ctx.r12.s64 = ctx.r12.s64 + 25344;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_8273634C;
	case 1:
		goto loc_8273634C;
	case 2:
		goto loc_82736354;
	case 3:
		goto loc_82736354;
	case 4:
		goto loc_8273634C;
	case 5:
		goto loc_8273634C;
	case 6:
		goto loc_8273634C;
	case 7:
		goto loc_8273634C;
	case 8:
		goto loc_8273634C;
	case 9:
		goto loc_8273634C;
	case 10:
		goto loc_8273634C;
	case 11:
		goto loc_8273634C;
	case 12:
		goto loc_8273634C;
	case 13:
		goto loc_82736354;
	case 14:
		goto loc_82736354;
	case 15:
		goto loc_82736354;
	case 16:
		goto loc_82736354;
	case 17:
		goto loc_82736354;
	case 18:
		goto loc_82736354;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_8273634C:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_82736354:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_8273635C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736368);
PPC_WEAK_FUNC(sub_82736368) { __imp__sub_82736368(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736368) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6612
	ctx.r3.s64 = ctx.r11.s64 + -6612;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736378);
PPC_WEAK_FUNC(sub_82736378) { __imp__sub_82736378(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736378) {
	PPC_FUNC_PROLOGUE();
	// li r3,177
	ctx.r3.s64 = 177;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736380);
PPC_WEAK_FUNC(sub_82736380) { __imp__sub_82736380(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736380) {
	PPC_FUNC_PROLOGUE();
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
	// bl 0x827362e0
	ctx.lr = 0x82736394;
	sub_827362E0(ctx, base);
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// bgt cr6,0x8273641c
	if (ctx.cr6.gt) goto loc_8273641C;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,25528
	ctx.r12.s64 = ctx.r12.s64 + 25528;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_827363CC;
	case 1:
		goto loc_827363E0;
	case 2:
		goto loc_82736408;
	case 3:
		goto loc_8273641C;
	case 4:
		goto loc_827363F4;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_827363CC:
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
loc_827363E0:
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
loc_827363F4:
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
loc_82736408:
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
loc_8273641C:
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

PPC_FUNC_IMPL(__imp__sub_82736430);
PPC_WEAK_FUNC(sub_82736430) { __imp__sub_82736430(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736430) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6576
	ctx.r3.s64 = ctx.r11.s64 + -6576;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736440);
PPC_WEAK_FUNC(sub_82736440) { __imp__sub_82736440(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736440) {
	PPC_FUNC_PROLOGUE();
	// li r3,178
	ctx.r3.s64 = 178;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736448);
PPC_WEAK_FUNC(sub_82736448) { __imp__sub_82736448(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736448) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6540
	ctx.r3.s64 = ctx.r11.s64 + -6540;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736458);
PPC_WEAK_FUNC(sub_82736458) { __imp__sub_82736458(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736458) {
	PPC_FUNC_PROLOGUE();
	// li r3,179
	ctx.r3.s64 = 179;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736460);
PPC_WEAK_FUNC(sub_82736460) { __imp__sub_82736460(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736460) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6504
	ctx.r3.s64 = ctx.r11.s64 + -6504;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736470);
PPC_WEAK_FUNC(sub_82736470) { __imp__sub_82736470(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736470) {
	PPC_FUNC_PROLOGUE();
	// li r3,194
	ctx.r3.s64 = 194;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736478);
PPC_WEAK_FUNC(sub_82736478) { __imp__sub_82736478(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736478) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6468
	ctx.r3.s64 = ctx.r11.s64 + -6468;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736488);
PPC_WEAK_FUNC(sub_82736488) { __imp__sub_82736488(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736488) {
	PPC_FUNC_PROLOGUE();
	// li r3,195
	ctx.r3.s64 = 195;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736490);
PPC_WEAK_FUNC(sub_82736490) { __imp__sub_82736490(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736490) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6432
	ctx.r3.s64 = ctx.r11.s64 + -6432;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827364A0);
PPC_WEAK_FUNC(sub_827364A0) { __imp__sub_827364A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827364A0) {
	PPC_FUNC_PROLOGUE();
	// li r3,196
	ctx.r3.s64 = 196;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827364A8);
PPC_WEAK_FUNC(sub_827364A8) { __imp__sub_827364A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827364A8) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// li r3,1
	ctx.r3.s64 = 1;
	// cmplwi cr6,r6,3
	ctx.cr6.compare<uint32_t>(ctx.r6.u32, 3, ctx.xer);
	// bgt cr6,0x82736538
	if (ctx.cr6.gt) goto loc_82736538;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,25808
	ctx.r12.s64 = ctx.r12.s64 + 25808;
	// rlwinm r0,r6,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r6.u32) {
	case 0:
		goto loc_827364E0;
	case 1:
		goto loc_827364FC;
	case 2:
		goto loc_8273652C;
	case 3:
		goto loc_82736518;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_827364E0:
	// ld r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U64(ctx.r11.u32 + 0);
	// ld r10,0(r4)
	ctx.r10.u64 = PPC_LOAD_U64(ctx.r4.u32 + 0);
	// cmpd cr6,r11,r10
	ctx.cr6.compare<int64_t>(ctx.r11.s64, ctx.r10.s64, ctx.xer);
	// blt cr6,0x827364f4
	if (ctx.cr6.lt) goto loc_827364F4;
	// mr r11,r10
	ctx.r11.u64 = ctx.r10.u64;
loc_827364F4:
	// std r11,0(r5)
	PPC_STORE_U64(ctx.r5.u32 + 0, ctx.r11.u64);
	// blr 
	return;
loc_827364FC:
	// ld r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U64(ctx.r11.u32 + 0);
	// ld r10,0(r4)
	ctx.r10.u64 = PPC_LOAD_U64(ctx.r4.u32 + 0);
	// cmpd cr6,r11,r10
	ctx.cr6.compare<int64_t>(ctx.r11.s64, ctx.r10.s64, ctx.xer);
	// bgt cr6,0x827364f4
	if (ctx.cr6.gt) goto loc_827364F4;
	// mr r11,r10
	ctx.r11.u64 = ctx.r10.u64;
	// std r11,0(r5)
	PPC_STORE_U64(ctx.r5.u32 + 0, ctx.r11.u64);
	// blr 
	return;
loc_82736518:
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
loc_8273652C:
	// ld r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U64(ctx.r11.u32 + 0);
	// std r11,0(r5)
	PPC_STORE_U64(ctx.r5.u32 + 0, ctx.r11.u64);
	// blr 
	return;
loc_82736538:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736540);
PPC_WEAK_FUNC(sub_82736540) { __imp__sub_82736540(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736540) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6396
	ctx.r3.s64 = ctx.r11.s64 + -6396;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736550);
PPC_WEAK_FUNC(sub_82736550) { __imp__sub_82736550(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736550) {
	PPC_FUNC_PROLOGUE();
	// li r3,197
	ctx.r3.s64 = 197;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736558);
PPC_WEAK_FUNC(sub_82736558) { __imp__sub_82736558(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736558) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6360
	ctx.r3.s64 = ctx.r11.s64 + -6360;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736568);
PPC_WEAK_FUNC(sub_82736568) { __imp__sub_82736568(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736568) {
	PPC_FUNC_PROLOGUE();
	// li r3,198
	ctx.r3.s64 = 198;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736570);
PPC_WEAK_FUNC(sub_82736570) { __imp__sub_82736570(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736570) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6324
	ctx.r3.s64 = ctx.r11.s64 + -6324;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736580);
PPC_WEAK_FUNC(sub_82736580) { __imp__sub_82736580(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736580) {
	PPC_FUNC_PROLOGUE();
	// li r3,213
	ctx.r3.s64 = 213;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736588);
PPC_WEAK_FUNC(sub_82736588) { __imp__sub_82736588(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736588) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6288
	ctx.r3.s64 = ctx.r11.s64 + -6288;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736598);
PPC_WEAK_FUNC(sub_82736598) { __imp__sub_82736598(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736598) {
	PPC_FUNC_PROLOGUE();
	// li r3,214
	ctx.r3.s64 = 214;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827365A0);
PPC_WEAK_FUNC(sub_827365A0) { __imp__sub_827365A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827365A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6252
	ctx.r3.s64 = ctx.r11.s64 + -6252;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827365B0);
PPC_WEAK_FUNC(sub_827365B0) { __imp__sub_827365B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827365B0) {
	PPC_FUNC_PROLOGUE();
	// li r3,215
	ctx.r3.s64 = 215;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827365B8);
PPC_WEAK_FUNC(sub_827365B8) { __imp__sub_827365B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827365B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6216
	ctx.r3.s64 = ctx.r11.s64 + -6216;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827365C8);
PPC_WEAK_FUNC(sub_827365C8) { __imp__sub_827365C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827365C8) {
	PPC_FUNC_PROLOGUE();
	// li r3,216
	ctx.r3.s64 = 216;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827365D0);
PPC_WEAK_FUNC(sub_827365D0) { __imp__sub_827365D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827365D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6180
	ctx.r3.s64 = ctx.r11.s64 + -6180;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827365E0);
PPC_WEAK_FUNC(sub_827365E0) { __imp__sub_827365E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827365E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6144
	ctx.r3.s64 = ctx.r11.s64 + -6144;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827365F0);
PPC_WEAK_FUNC(sub_827365F0) { __imp__sub_827365F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827365F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6108
	ctx.r3.s64 = ctx.r11.s64 + -6108;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736600);
PPC_WEAK_FUNC(sub_82736600) { __imp__sub_82736600(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736600) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6072
	ctx.r3.s64 = ctx.r11.s64 + -6072;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736610);
PPC_WEAK_FUNC(sub_82736610) { __imp__sub_82736610(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736610) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6036
	ctx.r3.s64 = ctx.r11.s64 + -6036;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736620);
PPC_WEAK_FUNC(sub_82736620) { __imp__sub_82736620(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736620) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-6000
	ctx.r3.s64 = ctx.r11.s64 + -6000;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736630);
PPC_WEAK_FUNC(sub_82736630) { __imp__sub_82736630(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736630) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-5964
	ctx.r3.s64 = ctx.r11.s64 + -5964;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736640);
PPC_WEAK_FUNC(sub_82736640) { __imp__sub_82736640(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736640) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-5928
	ctx.r3.s64 = ctx.r11.s64 + -5928;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736650);
PPC_WEAK_FUNC(sub_82736650) { __imp__sub_82736650(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736650) {
	PPC_FUNC_PROLOGUE();
	// li r3,224
	ctx.r3.s64 = 224;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736658);
PPC_WEAK_FUNC(sub_82736658) { __imp__sub_82736658(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736658) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-5892
	ctx.r3.s64 = ctx.r11.s64 + -5892;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736668);
PPC_WEAK_FUNC(sub_82736668) { __imp__sub_82736668(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736668) {
	PPC_FUNC_PROLOGUE();
	// li r3,225
	ctx.r3.s64 = 225;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736670);
PPC_WEAK_FUNC(sub_82736670) { __imp__sub_82736670(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736670) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-5856
	ctx.r3.s64 = ctx.r11.s64 + -5856;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736680);
PPC_WEAK_FUNC(sub_82736680) { __imp__sub_82736680(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736680) {
	PPC_FUNC_PROLOGUE();
	// li r3,226
	ctx.r3.s64 = 226;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736688);
PPC_WEAK_FUNC(sub_82736688) { __imp__sub_82736688(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736688) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-5820
	ctx.r3.s64 = ctx.r11.s64 + -5820;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736698);
PPC_WEAK_FUNC(sub_82736698) { __imp__sub_82736698(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736698) {
	PPC_FUNC_PROLOGUE();
	// li r3,227
	ctx.r3.s64 = 227;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827366A0);
PPC_WEAK_FUNC(sub_827366A0) { __imp__sub_827366A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827366A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-5784
	ctx.r3.s64 = ctx.r11.s64 + -5784;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827366B0);
PPC_WEAK_FUNC(sub_827366B0) { __imp__sub_827366B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827366B0) {
	PPC_FUNC_PROLOGUE();
	// li r3,229
	ctx.r3.s64 = 229;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827366B8);
PPC_WEAK_FUNC(sub_827366B8) { __imp__sub_827366B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827366B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-5748
	ctx.r3.s64 = ctx.r11.s64 + -5748;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827366C8);
PPC_WEAK_FUNC(sub_827366C8) { __imp__sub_827366C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827366C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-5712
	ctx.r3.s64 = ctx.r11.s64 + -5712;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827366D8);
PPC_WEAK_FUNC(sub_827366D8) { __imp__sub_827366D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827366D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-5676
	ctx.r3.s64 = ctx.r11.s64 + -5676;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827366E8);
PPC_WEAK_FUNC(sub_827366E8) { __imp__sub_827366E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827366E8) {
	PPC_FUNC_PROLOGUE();
	// li r3,239
	ctx.r3.s64 = 239;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827366F0);
PPC_WEAK_FUNC(sub_827366F0) { __imp__sub_827366F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827366F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-5640
	ctx.r3.s64 = ctx.r11.s64 + -5640;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736700);
PPC_WEAK_FUNC(sub_82736700) { __imp__sub_82736700(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736700) {
	PPC_FUNC_PROLOGUE();
	// li r3,243
	ctx.r3.s64 = 243;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736708);
PPC_WEAK_FUNC(sub_82736708) { __imp__sub_82736708(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736708) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-5604
	ctx.r3.s64 = ctx.r11.s64 + -5604;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736718);
PPC_WEAK_FUNC(sub_82736718) { __imp__sub_82736718(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736718) {
	PPC_FUNC_PROLOGUE();
	// li r3,232
	ctx.r3.s64 = 232;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736720);
PPC_WEAK_FUNC(sub_82736720) { __imp__sub_82736720(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736720) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-5568
	ctx.r3.s64 = ctx.r11.s64 + -5568;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736730);
PPC_WEAK_FUNC(sub_82736730) { __imp__sub_82736730(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736730) {
	PPC_FUNC_PROLOGUE();
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736740);
PPC_WEAK_FUNC(sub_82736740) { __imp__sub_82736740(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736740) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-5532
	ctx.r3.s64 = ctx.r11.s64 + -5532;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736750);
PPC_WEAK_FUNC(sub_82736750) { __imp__sub_82736750(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736750) {
	PPC_FUNC_PROLOGUE();
	// li r3,242
	ctx.r3.s64 = 242;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736758);
PPC_WEAK_FUNC(sub_82736758) { __imp__sub_82736758(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736758) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-5496
	ctx.r3.s64 = ctx.r11.s64 + -5496;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736768);
PPC_WEAK_FUNC(sub_82736768) { __imp__sub_82736768(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736768) {
	PPC_FUNC_PROLOGUE();
	// li r3,245
	ctx.r3.s64 = 245;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736770);
PPC_WEAK_FUNC(sub_82736770) { __imp__sub_82736770(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736770) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-5460
	ctx.r3.s64 = ctx.r11.s64 + -5460;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736780);
PPC_WEAK_FUNC(sub_82736780) { __imp__sub_82736780(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736780) {
	PPC_FUNC_PROLOGUE();
	// li r3,247
	ctx.r3.s64 = 247;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736788);
PPC_WEAK_FUNC(sub_82736788) { __imp__sub_82736788(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736788) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-5424
	ctx.r3.s64 = ctx.r11.s64 + -5424;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736798);
PPC_WEAK_FUNC(sub_82736798) { __imp__sub_82736798(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736798) {
	PPC_FUNC_PROLOGUE();
	// li r3,241
	ctx.r3.s64 = 241;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827367A0);
PPC_WEAK_FUNC(sub_827367A0) { __imp__sub_827367A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827367A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-5388
	ctx.r3.s64 = ctx.r11.s64 + -5388;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827367B0);
PPC_WEAK_FUNC(sub_827367B0) { __imp__sub_827367B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827367B0) {
	PPC_FUNC_PROLOGUE();
	// li r3,240
	ctx.r3.s64 = 240;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827367B8);
PPC_WEAK_FUNC(sub_827367B8) { __imp__sub_827367B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827367B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-5352
	ctx.r3.s64 = ctx.r11.s64 + -5352;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827367C8);
PPC_WEAK_FUNC(sub_827367C8) { __imp__sub_827367C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827367C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-5316
	ctx.r3.s64 = ctx.r11.s64 + -5316;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827367D8);
PPC_WEAK_FUNC(sub_827367D8) { __imp__sub_827367D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827367D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-5280
	ctx.r3.s64 = ctx.r11.s64 + -5280;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827367E8);
PPC_WEAK_FUNC(sub_827367E8) { __imp__sub_827367E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827367E8) {
	PPC_FUNC_PROLOGUE();
	// li r3,246
	ctx.r3.s64 = 246;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827367F0);
PPC_WEAK_FUNC(sub_827367F0) { __imp__sub_827367F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827367F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-5244
	ctx.r3.s64 = ctx.r11.s64 + -5244;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736800);
PPC_WEAK_FUNC(sub_82736800) { __imp__sub_82736800(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736800) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-5208
	ctx.r3.s64 = ctx.r11.s64 + -5208;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736810);
PPC_WEAK_FUNC(sub_82736810) { __imp__sub_82736810(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736810) {
	PPC_FUNC_PROLOGUE();
	// li r3,231
	ctx.r3.s64 = 231;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736818);
PPC_WEAK_FUNC(sub_82736818) { __imp__sub_82736818(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736818) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-5172
	ctx.r3.s64 = ctx.r11.s64 + -5172;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736828);
PPC_WEAK_FUNC(sub_82736828) { __imp__sub_82736828(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736828) {
	PPC_FUNC_PROLOGUE();
	// li r3,230
	ctx.r3.s64 = 230;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736830);
PPC_WEAK_FUNC(sub_82736830) { __imp__sub_82736830(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736830) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-5136
	ctx.r3.s64 = ctx.r11.s64 + -5136;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736840);
PPC_WEAK_FUNC(sub_82736840) { __imp__sub_82736840(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736840) {
	PPC_FUNC_PROLOGUE();
	// li r3,248
	ctx.r3.s64 = 248;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736848);
PPC_WEAK_FUNC(sub_82736848) { __imp__sub_82736848(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736848) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-5100
	ctx.r3.s64 = ctx.r11.s64 + -5100;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736858);
PPC_WEAK_FUNC(sub_82736858) { __imp__sub_82736858(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736858) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-5064
	ctx.r3.s64 = ctx.r11.s64 + -5064;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736868);
PPC_WEAK_FUNC(sub_82736868) { __imp__sub_82736868(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736868) {
	PPC_FUNC_PROLOGUE();
	// li r3,249
	ctx.r3.s64 = 249;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736870);
PPC_WEAK_FUNC(sub_82736870) { __imp__sub_82736870(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736870) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-5028
	ctx.r3.s64 = ctx.r11.s64 + -5028;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736880);
PPC_WEAK_FUNC(sub_82736880) { __imp__sub_82736880(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736880) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4992
	ctx.r3.s64 = ctx.r11.s64 + -4992;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736890);
PPC_WEAK_FUNC(sub_82736890) { __imp__sub_82736890(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736890) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4956
	ctx.r3.s64 = ctx.r11.s64 + -4956;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827368A0);
PPC_WEAK_FUNC(sub_827368A0) { __imp__sub_827368A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827368A0) {
	PPC_FUNC_PROLOGUE();
	// li r3,209
	ctx.r3.s64 = 209;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827368A8);
PPC_WEAK_FUNC(sub_827368A8) { __imp__sub_827368A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827368A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4920
	ctx.r3.s64 = ctx.r11.s64 + -4920;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827368B8);
PPC_WEAK_FUNC(sub_827368B8) { __imp__sub_827368B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827368B8) {
	PPC_FUNC_PROLOGUE();
	// li r3,252
	ctx.r3.s64 = 252;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827368C0);
PPC_WEAK_FUNC(sub_827368C0) { __imp__sub_827368C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827368C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4884
	ctx.r3.s64 = ctx.r11.s64 + -4884;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827368D0);
PPC_WEAK_FUNC(sub_827368D0) { __imp__sub_827368D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827368D0) {
	PPC_FUNC_PROLOGUE();
	// li r3,253
	ctx.r3.s64 = 253;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827368D8);
PPC_WEAK_FUNC(sub_827368D8) { __imp__sub_827368D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827368D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4848
	ctx.r3.s64 = ctx.r11.s64 + -4848;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827368E8);
PPC_WEAK_FUNC(sub_827368E8) { __imp__sub_827368E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827368E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4812
	ctx.r3.s64 = ctx.r11.s64 + -4812;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827368F8);
PPC_WEAK_FUNC(sub_827368F8) { __imp__sub_827368F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827368F8) {
	PPC_FUNC_PROLOGUE();
	// li r3,255
	ctx.r3.s64 = 255;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736900);
PPC_WEAK_FUNC(sub_82736900) { __imp__sub_82736900(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736900) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4776
	ctx.r3.s64 = ctx.r11.s64 + -4776;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736910);
PPC_WEAK_FUNC(sub_82736910) { __imp__sub_82736910(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736910) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4740
	ctx.r3.s64 = ctx.r11.s64 + -4740;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736920);
PPC_WEAK_FUNC(sub_82736920) { __imp__sub_82736920(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736920) {
	PPC_FUNC_PROLOGUE();
	// li r3,257
	ctx.r3.s64 = 257;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736928);
PPC_WEAK_FUNC(sub_82736928) { __imp__sub_82736928(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736928) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4704
	ctx.r3.s64 = ctx.r11.s64 + -4704;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736938);
PPC_WEAK_FUNC(sub_82736938) { __imp__sub_82736938(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736938) {
	PPC_FUNC_PROLOGUE();
	// li r3,258
	ctx.r3.s64 = 258;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736940);
PPC_WEAK_FUNC(sub_82736940) { __imp__sub_82736940(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736940) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4668
	ctx.r3.s64 = ctx.r11.s64 + -4668;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736950);
PPC_WEAK_FUNC(sub_82736950) { __imp__sub_82736950(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736950) {
	PPC_FUNC_PROLOGUE();
	// li r3,259
	ctx.r3.s64 = 259;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736958);
PPC_WEAK_FUNC(sub_82736958) { __imp__sub_82736958(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736958) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4632
	ctx.r3.s64 = ctx.r11.s64 + -4632;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736968);
PPC_WEAK_FUNC(sub_82736968) { __imp__sub_82736968(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736968) {
	PPC_FUNC_PROLOGUE();
	// li r3,260
	ctx.r3.s64 = 260;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736970);
PPC_WEAK_FUNC(sub_82736970) { __imp__sub_82736970(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736970) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x82736a8c
	if (ctx.cr6.gt) goto loc_82736A8C;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x82736a84
	if (ctx.cr6.eq) goto loc_82736A84;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,17
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 17, ctx.xer);
	// bgt cr6,0x82736a8c
	if (ctx.cr6.gt) goto loc_82736A8C;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,27052
	ctx.r12.s64 = ctx.r12.s64 + 27052;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_827369F4;
	case 1:
		goto loc_827369FC;
	case 2:
		goto loc_82736A04;
	case 3:
		goto loc_82736A0C;
	case 4:
		goto loc_82736A14;
	case 5:
		goto loc_82736A1C;
	case 6:
		goto loc_82736A4C;
	case 7:
		goto loc_82736A44;
	case 8:
		goto loc_82736A74;
	case 9:
		goto loc_82736A6C;
	case 10:
		goto loc_82736A3C;
	case 11:
		goto loc_82736A64;
	case 12:
		goto loc_82736A5C;
	case 13:
		goto loc_82736A24;
	case 14:
		goto loc_82736A2C;
	case 15:
		goto loc_82736A34;
	case 16:
		goto loc_82736A54;
	case 17:
		goto loc_82736A7C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_827369F4:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_827369FC:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_82736A04:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_82736A0C:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_82736A14:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_82736A1C:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_82736A24:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_82736A2C:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_82736A34:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_82736A3C:
	// li r3,9
	ctx.r3.s64 = 9;
	// blr 
	return;
loc_82736A44:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_82736A4C:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_82736A54:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_82736A5C:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_82736A64:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_82736A6C:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_82736A74:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_82736A7C:
	// li r3,17
	ctx.r3.s64 = 17;
	// blr 
	return;
loc_82736A84:
	// li r3,18
	ctx.r3.s64 = 18;
	// blr 
	return;
loc_82736A8C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736A98);
PPC_WEAK_FUNC(sub_82736A98) { __imp__sub_82736A98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736A98) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4596
	ctx.r3.s64 = ctx.r11.s64 + -4596;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736AA8);
PPC_WEAK_FUNC(sub_82736AA8) { __imp__sub_82736AA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736AA8) {
	PPC_FUNC_PROLOGUE();
	// li r3,261
	ctx.r3.s64 = 261;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736AB0);
PPC_WEAK_FUNC(sub_82736AB0) { __imp__sub_82736AB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736AB0) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x82736970
	sub_82736970(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736AB8);
PPC_WEAK_FUNC(sub_82736AB8) { __imp__sub_82736AB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736AB8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4560
	ctx.r3.s64 = ctx.r11.s64 + -4560;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736AC8);
PPC_WEAK_FUNC(sub_82736AC8) { __imp__sub_82736AC8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736AC8) {
	PPC_FUNC_PROLOGUE();
	// li r3,262
	ctx.r3.s64 = 262;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736AD0);
PPC_WEAK_FUNC(sub_82736AD0) { __imp__sub_82736AD0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736AD0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4524
	ctx.r3.s64 = ctx.r11.s64 + -4524;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736AE0);
PPC_WEAK_FUNC(sub_82736AE0) { __imp__sub_82736AE0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736AE0) {
	PPC_FUNC_PROLOGUE();
	// li r3,263
	ctx.r3.s64 = 263;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736AE8);
PPC_WEAK_FUNC(sub_82736AE8) { __imp__sub_82736AE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736AE8) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// li r3,1
	ctx.r3.s64 = 1;
	// cmplwi cr6,r6,3
	ctx.cr6.compare<uint32_t>(ctx.r6.u32, 3, ctx.xer);
	// bgt cr6,0x82736b78
	if (ctx.cr6.gt) goto loc_82736B78;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,27408
	ctx.r12.s64 = ctx.r12.s64 + 27408;
	// rlwinm r0,r6,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r6.u32) {
	case 0:
		goto loc_82736B20;
	case 1:
		goto loc_82736B3C;
	case 2:
		goto loc_82736B6C;
	case 3:
		goto loc_82736B58;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_82736B20:
	// lfd f0,0(r11)
	ctx.fpscr.disableFlushMode();
	ctx.f0.u64 = PPC_LOAD_U64(ctx.r11.u32 + 0);
	// lfd f13,0(r4)
	ctx.f13.u64 = PPC_LOAD_U64(ctx.r4.u32 + 0);
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// blt cr6,0x82736b34
	if (ctx.cr6.lt) goto loc_82736B34;
	// fmr f0,f13
	ctx.f0.f64 = ctx.f13.f64;
loc_82736B34:
	// stfd f0,0(r5)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r5.u32 + 0, ctx.f0.u64);
	// blr 
	return;
loc_82736B3C:
	// lfd f0,0(r11)
	ctx.fpscr.disableFlushMode();
	ctx.f0.u64 = PPC_LOAD_U64(ctx.r11.u32 + 0);
	// lfd f13,0(r4)
	ctx.f13.u64 = PPC_LOAD_U64(ctx.r4.u32 + 0);
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// bgt cr6,0x82736b34
	if (ctx.cr6.gt) goto loc_82736B34;
	// fmr f0,f13
	ctx.f0.f64 = ctx.f13.f64;
	// stfd f0,0(r5)
	PPC_STORE_U64(ctx.r5.u32 + 0, ctx.f0.u64);
	// blr 
	return;
loc_82736B58:
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
loc_82736B6C:
	// lfd f0,0(r11)
	ctx.fpscr.disableFlushMode();
	ctx.f0.u64 = PPC_LOAD_U64(ctx.r11.u32 + 0);
	// stfd f0,0(r5)
	PPC_STORE_U64(ctx.r5.u32 + 0, ctx.f0.u64);
	// blr 
	return;
loc_82736B78:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736B80);
PPC_WEAK_FUNC(sub_82736B80) { __imp__sub_82736B80(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736B80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4488
	ctx.r3.s64 = ctx.r11.s64 + -4488;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736B90);
PPC_WEAK_FUNC(sub_82736B90) { __imp__sub_82736B90(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736B90) {
	PPC_FUNC_PROLOGUE();
	// li r3,264
	ctx.r3.s64 = 264;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736B98);
PPC_WEAK_FUNC(sub_82736B98) { __imp__sub_82736B98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736B98) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4452
	ctx.r3.s64 = ctx.r11.s64 + -4452;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736BA8);
PPC_WEAK_FUNC(sub_82736BA8) { __imp__sub_82736BA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736BA8) {
	PPC_FUNC_PROLOGUE();
	// li r3,265
	ctx.r3.s64 = 265;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736BB0);
PPC_WEAK_FUNC(sub_82736BB0) { __imp__sub_82736BB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736BB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4416
	ctx.r3.s64 = ctx.r11.s64 + -4416;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736BC0);
PPC_WEAK_FUNC(sub_82736BC0) { __imp__sub_82736BC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736BC0) {
	PPC_FUNC_PROLOGUE();
	// li r3,266
	ctx.r3.s64 = 266;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736BC8);
PPC_WEAK_FUNC(sub_82736BC8) { __imp__sub_82736BC8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736BC8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4380
	ctx.r3.s64 = ctx.r11.s64 + -4380;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736BD8);
PPC_WEAK_FUNC(sub_82736BD8) { __imp__sub_82736BD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736BD8) {
	PPC_FUNC_PROLOGUE();
	// li r3,267
	ctx.r3.s64 = 267;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736BE0);
PPC_WEAK_FUNC(sub_82736BE0) { __imp__sub_82736BE0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736BE0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4344
	ctx.r3.s64 = ctx.r11.s64 + -4344;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736BF0);
PPC_WEAK_FUNC(sub_82736BF0) { __imp__sub_82736BF0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736BF0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4308
	ctx.r3.s64 = ctx.r11.s64 + -4308;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736C00);
PPC_WEAK_FUNC(sub_82736C00) { __imp__sub_82736C00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736C00) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4272
	ctx.r3.s64 = ctx.r11.s64 + -4272;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736C10);
PPC_WEAK_FUNC(sub_82736C10) { __imp__sub_82736C10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736C10) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4236
	ctx.r3.s64 = ctx.r11.s64 + -4236;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736C20);
PPC_WEAK_FUNC(sub_82736C20) { __imp__sub_82736C20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736C20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4200
	ctx.r3.s64 = ctx.r11.s64 + -4200;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736C30);
PPC_WEAK_FUNC(sub_82736C30) { __imp__sub_82736C30(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736C30) {
	PPC_FUNC_PROLOGUE();
	// li r3,272
	ctx.r3.s64 = 272;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736C38);
PPC_WEAK_FUNC(sub_82736C38) { __imp__sub_82736C38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736C38) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4164
	ctx.r3.s64 = ctx.r11.s64 + -4164;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736C48);
PPC_WEAK_FUNC(sub_82736C48) { __imp__sub_82736C48(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736C48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4128
	ctx.r3.s64 = ctx.r11.s64 + -4128;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736C58);
PPC_WEAK_FUNC(sub_82736C58) { __imp__sub_82736C58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736C58) {
	PPC_FUNC_PROLOGUE();
	// li r3,274
	ctx.r3.s64 = 274;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736C60);
PPC_WEAK_FUNC(sub_82736C60) { __imp__sub_82736C60(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736C60) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4092
	ctx.r3.s64 = ctx.r11.s64 + -4092;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736C70);
PPC_WEAK_FUNC(sub_82736C70) { __imp__sub_82736C70(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736C70) {
	PPC_FUNC_PROLOGUE();
	// li r3,275
	ctx.r3.s64 = 275;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736C78);
PPC_WEAK_FUNC(sub_82736C78) { __imp__sub_82736C78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736C78) {
	PPC_FUNC_PROLOGUE();
	// cmplwi cr6,r3,18
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 18, ctx.xer);
	// bgt cr6,0x82736d80
	if (ctx.cr6.gt) goto loc_82736D80;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,27800
	ctx.r12.s64 = ctx.r12.s64 + 27800;
	// rlwinm r0,r3,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r3.u32) {
	case 0:
		goto loc_82736CE4;
	case 1:
		goto loc_82736CEC;
	case 2:
		goto loc_82736CF4;
	case 3:
		goto loc_82736CFC;
	case 4:
		goto loc_82736D04;
	case 5:
		goto loc_82736D0C;
	case 6:
		goto loc_82736D14;
	case 7:
		goto loc_82736D1C;
	case 8:
		goto loc_82736D24;
	case 9:
		goto loc_82736D2C;
	case 10:
		goto loc_82736D34;
	case 11:
		goto loc_82736D3C;
	case 12:
		goto loc_82736D44;
	case 13:
		goto loc_82736D4C;
	case 14:
		goto loc_82736D54;
	case 15:
		goto loc_82736D5C;
	case 16:
		goto loc_82736D64;
	case 17:
		goto loc_82736D6C;
	case 18:
		goto loc_82736D74;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_82736CE4:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_82736CEC:
	// li r3,2
	ctx.r3.s64 = 2;
	// blr 
	return;
loc_82736CF4:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_82736CFC:
	// li r3,4
	ctx.r3.s64 = 4;
	// blr 
	return;
loc_82736D04:
	// li r3,5
	ctx.r3.s64 = 5;
	// blr 
	return;
loc_82736D0C:
	// li r3,6
	ctx.r3.s64 = 6;
	// blr 
	return;
loc_82736D14:
	// li r3,14
	ctx.r3.s64 = 14;
	// blr 
	return;
loc_82736D1C:
	// li r3,15
	ctx.r3.s64 = 15;
	// blr 
	return;
loc_82736D24:
	// li r3,16
	ctx.r3.s64 = 16;
	// blr 
	return;
loc_82736D2C:
	// li r3,11
	ctx.r3.s64 = 11;
	// blr 
	return;
loc_82736D34:
	// li r3,8
	ctx.r3.s64 = 8;
	// blr 
	return;
loc_82736D3C:
	// li r3,7
	ctx.r3.s64 = 7;
	// blr 
	return;
loc_82736D44:
	// li r3,17
	ctx.r3.s64 = 17;
	// blr 
	return;
loc_82736D4C:
	// li r3,13
	ctx.r3.s64 = 13;
	// blr 
	return;
loc_82736D54:
	// li r3,12
	ctx.r3.s64 = 12;
	// blr 
	return;
loc_82736D5C:
	// li r3,10
	ctx.r3.s64 = 10;
	// blr 
	return;
loc_82736D64:
	// li r3,9
	ctx.r3.s64 = 9;
	// blr 
	return;
loc_82736D6C:
	// li r3,18
	ctx.r3.s64 = 18;
	// blr 
	return;
loc_82736D74:
	// lis r3,0
	ctx.r3.s64 = 0;
	// ori r3,r3,65534
	ctx.r3.u64 = ctx.r3.u64 | 65534;
	// blr 
	return;
loc_82736D80:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736D88);
PPC_WEAK_FUNC(sub_82736D88) { __imp__sub_82736D88(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736D88) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4056
	ctx.r3.s64 = ctx.r11.s64 + -4056;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736D98);
PPC_WEAK_FUNC(sub_82736D98) { __imp__sub_82736D98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736D98) {
	PPC_FUNC_PROLOGUE();
	// li r3,276
	ctx.r3.s64 = 276;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736DA0);
PPC_WEAK_FUNC(sub_82736DA0) { __imp__sub_82736DA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736DA0) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x82736c78
	sub_82736C78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736DA8);
PPC_WEAK_FUNC(sub_82736DA8) { __imp__sub_82736DA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736DA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4020
	ctx.r3.s64 = ctx.r11.s64 + -4020;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736DB8);
PPC_WEAK_FUNC(sub_82736DB8) { __imp__sub_82736DB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736DB8) {
	PPC_FUNC_PROLOGUE();
	// li r3,277
	ctx.r3.s64 = 277;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736DC0);
PPC_WEAK_FUNC(sub_82736DC0) { __imp__sub_82736DC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736DC0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3984
	ctx.r3.s64 = ctx.r11.s64 + -3984;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736DD0);
PPC_WEAK_FUNC(sub_82736DD0) { __imp__sub_82736DD0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736DD0) {
	PPC_FUNC_PROLOGUE();
	// li r3,278
	ctx.r3.s64 = 278;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736DD8);
PPC_WEAK_FUNC(sub_82736DD8) { __imp__sub_82736DD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736DD8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3948
	ctx.r3.s64 = ctx.r11.s64 + -3948;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736DE8);
PPC_WEAK_FUNC(sub_82736DE8) { __imp__sub_82736DE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736DE8) {
	PPC_FUNC_PROLOGUE();
	// li r3,279
	ctx.r3.s64 = 279;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736DF0);
PPC_WEAK_FUNC(sub_82736DF0) { __imp__sub_82736DF0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736DF0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3912
	ctx.r3.s64 = ctx.r11.s64 + -3912;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736E00);
PPC_WEAK_FUNC(sub_82736E00) { __imp__sub_82736E00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736E00) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3876
	ctx.r3.s64 = ctx.r11.s64 + -3876;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736E10);
PPC_WEAK_FUNC(sub_82736E10) { __imp__sub_82736E10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736E10) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3840
	ctx.r3.s64 = ctx.r11.s64 + -3840;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736E20);
PPC_WEAK_FUNC(sub_82736E20) { __imp__sub_82736E20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736E20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3804
	ctx.r3.s64 = ctx.r11.s64 + -3804;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736E30);
PPC_WEAK_FUNC(sub_82736E30) { __imp__sub_82736E30(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736E30) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3768
	ctx.r3.s64 = ctx.r11.s64 + -3768;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736E40);
PPC_WEAK_FUNC(sub_82736E40) { __imp__sub_82736E40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736E40) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3732
	ctx.r3.s64 = ctx.r11.s64 + -3732;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736E50);
PPC_WEAK_FUNC(sub_82736E50) { __imp__sub_82736E50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736E50) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3696
	ctx.r3.s64 = ctx.r11.s64 + -3696;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736E60);
PPC_WEAK_FUNC(sub_82736E60) { __imp__sub_82736E60(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736E60) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3660
	ctx.r3.s64 = ctx.r11.s64 + -3660;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736E70);
PPC_WEAK_FUNC(sub_82736E70) { __imp__sub_82736E70(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736E70) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3624
	ctx.r3.s64 = ctx.r11.s64 + -3624;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736E80);
PPC_WEAK_FUNC(sub_82736E80) { __imp__sub_82736E80(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736E80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3588
	ctx.r3.s64 = ctx.r11.s64 + -3588;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736E90);
PPC_WEAK_FUNC(sub_82736E90) { __imp__sub_82736E90(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736E90) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3552
	ctx.r3.s64 = ctx.r11.s64 + -3552;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736EA0);
PPC_WEAK_FUNC(sub_82736EA0) { __imp__sub_82736EA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736EA0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3516
	ctx.r3.s64 = ctx.r11.s64 + -3516;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736EB0);
PPC_WEAK_FUNC(sub_82736EB0) { __imp__sub_82736EB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736EB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3480
	ctx.r3.s64 = ctx.r11.s64 + -3480;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736EC0);
PPC_WEAK_FUNC(sub_82736EC0) { __imp__sub_82736EC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736EC0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3444
	ctx.r3.s64 = ctx.r11.s64 + -3444;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736ED0);
PPC_WEAK_FUNC(sub_82736ED0) { __imp__sub_82736ED0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736ED0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3408
	ctx.r3.s64 = ctx.r11.s64 + -3408;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736EE0);
PPC_WEAK_FUNC(sub_82736EE0) { __imp__sub_82736EE0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736EE0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3372
	ctx.r3.s64 = ctx.r11.s64 + -3372;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736EF0);
PPC_WEAK_FUNC(sub_82736EF0) { __imp__sub_82736EF0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736EF0) {
	PPC_FUNC_PROLOGUE();
	// li r3,295
	ctx.r3.s64 = 295;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736EF8);
PPC_WEAK_FUNC(sub_82736EF8) { __imp__sub_82736EF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736EF8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3336
	ctx.r3.s64 = ctx.r11.s64 + -3336;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736F08);
PPC_WEAK_FUNC(sub_82736F08) { __imp__sub_82736F08(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736F08) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3300
	ctx.r3.s64 = ctx.r11.s64 + -3300;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736F18);
PPC_WEAK_FUNC(sub_82736F18) { __imp__sub_82736F18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736F18) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3264
	ctx.r3.s64 = ctx.r11.s64 + -3264;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736F28);
PPC_WEAK_FUNC(sub_82736F28) { __imp__sub_82736F28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736F28) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3228
	ctx.r3.s64 = ctx.r11.s64 + -3228;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736F38);
PPC_WEAK_FUNC(sub_82736F38) { __imp__sub_82736F38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736F38) {
	PPC_FUNC_PROLOGUE();
	// li r3,299
	ctx.r3.s64 = 299;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736F40);
PPC_WEAK_FUNC(sub_82736F40) { __imp__sub_82736F40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736F40) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3192
	ctx.r3.s64 = ctx.r11.s64 + -3192;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736F50);
PPC_WEAK_FUNC(sub_82736F50) { __imp__sub_82736F50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736F50) {
	PPC_FUNC_PROLOGUE();
	// li r3,300
	ctx.r3.s64 = 300;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736F58);
PPC_WEAK_FUNC(sub_82736F58) { __imp__sub_82736F58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736F58) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3156
	ctx.r3.s64 = ctx.r11.s64 + -3156;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736F68);
PPC_WEAK_FUNC(sub_82736F68) { __imp__sub_82736F68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736F68) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3120
	ctx.r3.s64 = ctx.r11.s64 + -3120;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736F78);
PPC_WEAK_FUNC(sub_82736F78) { __imp__sub_82736F78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736F78) {
	PPC_FUNC_PROLOGUE();
	// li r3,302
	ctx.r3.s64 = 302;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736F80);
PPC_WEAK_FUNC(sub_82736F80) { __imp__sub_82736F80(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736F80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3084
	ctx.r3.s64 = ctx.r11.s64 + -3084;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736F90);
PPC_WEAK_FUNC(sub_82736F90) { __imp__sub_82736F90(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736F90) {
	PPC_FUNC_PROLOGUE();
	// li r3,303
	ctx.r3.s64 = 303;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736F98);
PPC_WEAK_FUNC(sub_82736F98) { __imp__sub_82736F98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736F98) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3044
	ctx.r3.s64 = ctx.r11.s64 + -3044;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736FA8);
PPC_WEAK_FUNC(sub_82736FA8) { __imp__sub_82736FA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736FA8) {
	PPC_FUNC_PROLOGUE();
	// li r3,304
	ctx.r3.s64 = 304;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736FB0);
PPC_WEAK_FUNC(sub_82736FB0) { __imp__sub_82736FB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736FB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-3004
	ctx.r3.s64 = ctx.r11.s64 + -3004;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736FC0);
PPC_WEAK_FUNC(sub_82736FC0) { __imp__sub_82736FC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736FC0) {
	PPC_FUNC_PROLOGUE();
	// li r3,305
	ctx.r3.s64 = 305;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736FC8);
PPC_WEAK_FUNC(sub_82736FC8) { __imp__sub_82736FC8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736FC8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-2964
	ctx.r3.s64 = ctx.r11.s64 + -2964;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736FD8);
PPC_WEAK_FUNC(sub_82736FD8) { __imp__sub_82736FD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736FD8) {
	PPC_FUNC_PROLOGUE();
	// li r3,306
	ctx.r3.s64 = 306;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736FE0);
PPC_WEAK_FUNC(sub_82736FE0) { __imp__sub_82736FE0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736FE0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-2924
	ctx.r3.s64 = ctx.r11.s64 + -2924;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736FF0);
PPC_WEAK_FUNC(sub_82736FF0) { __imp__sub_82736FF0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736FF0) {
	PPC_FUNC_PROLOGUE();
	// li r3,307
	ctx.r3.s64 = 307;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82736FF8);
PPC_WEAK_FUNC(sub_82736FF8) { __imp__sub_82736FF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82736FF8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-2884
	ctx.r3.s64 = ctx.r11.s64 + -2884;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737008);
PPC_WEAK_FUNC(sub_82737008) { __imp__sub_82737008(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737008) {
	PPC_FUNC_PROLOGUE();
	// li r3,308
	ctx.r3.s64 = 308;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737010);
PPC_WEAK_FUNC(sub_82737010) { __imp__sub_82737010(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737010) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-2844
	ctx.r3.s64 = ctx.r11.s64 + -2844;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737020);
PPC_WEAK_FUNC(sub_82737020) { __imp__sub_82737020(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737020) {
	PPC_FUNC_PROLOGUE();
	// li r3,309
	ctx.r3.s64 = 309;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737028);
PPC_WEAK_FUNC(sub_82737028) { __imp__sub_82737028(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737028) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-2804
	ctx.r3.s64 = ctx.r11.s64 + -2804;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737038);
PPC_WEAK_FUNC(sub_82737038) { __imp__sub_82737038(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737038) {
	PPC_FUNC_PROLOGUE();
	// li r3,310
	ctx.r3.s64 = 310;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737040);
PPC_WEAK_FUNC(sub_82737040) { __imp__sub_82737040(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737040) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-2764
	ctx.r3.s64 = ctx.r11.s64 + -2764;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737050);
PPC_WEAK_FUNC(sub_82737050) { __imp__sub_82737050(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737050) {
	PPC_FUNC_PROLOGUE();
	// li r3,311
	ctx.r3.s64 = 311;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737058);
PPC_WEAK_FUNC(sub_82737058) { __imp__sub_82737058(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737058) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-2724
	ctx.r3.s64 = ctx.r11.s64 + -2724;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737068);
PPC_WEAK_FUNC(sub_82737068) { __imp__sub_82737068(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737068) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-2684
	ctx.r3.s64 = ctx.r11.s64 + -2684;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737078);
PPC_WEAK_FUNC(sub_82737078) { __imp__sub_82737078(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737078) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-2644
	ctx.r3.s64 = ctx.r11.s64 + -2644;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737088);
PPC_WEAK_FUNC(sub_82737088) { __imp__sub_82737088(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737088) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-2604
	ctx.r3.s64 = ctx.r11.s64 + -2604;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737098);
PPC_WEAK_FUNC(sub_82737098) { __imp__sub_82737098(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737098) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-2564
	ctx.r3.s64 = ctx.r11.s64 + -2564;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827370A8);
PPC_WEAK_FUNC(sub_827370A8) { __imp__sub_827370A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827370A8) {
	PPC_FUNC_PROLOGUE();
	// li r3,316
	ctx.r3.s64 = 316;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827370B0);
PPC_WEAK_FUNC(sub_827370B0) { __imp__sub_827370B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827370B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-2524
	ctx.r3.s64 = ctx.r11.s64 + -2524;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827370C0);
PPC_WEAK_FUNC(sub_827370C0) { __imp__sub_827370C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827370C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-2484
	ctx.r3.s64 = ctx.r11.s64 + -2484;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827370D0);
PPC_WEAK_FUNC(sub_827370D0) { __imp__sub_827370D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827370D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-2444
	ctx.r3.s64 = ctx.r11.s64 + -2444;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827370E0);
PPC_WEAK_FUNC(sub_827370E0) { __imp__sub_827370E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827370E0) {
	PPC_FUNC_PROLOGUE();
	// li r3,319
	ctx.r3.s64 = 319;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827370E8);
PPC_WEAK_FUNC(sub_827370E8) { __imp__sub_827370E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827370E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-2404
	ctx.r3.s64 = ctx.r11.s64 + -2404;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827370F8);
PPC_WEAK_FUNC(sub_827370F8) { __imp__sub_827370F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827370F8) {
	PPC_FUNC_PROLOGUE();
	// li r3,320
	ctx.r3.s64 = 320;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737100);
PPC_WEAK_FUNC(sub_82737100) { __imp__sub_82737100(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737100) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-2364
	ctx.r3.s64 = ctx.r11.s64 + -2364;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737110);
PPC_WEAK_FUNC(sub_82737110) { __imp__sub_82737110(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737110) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-2324
	ctx.r3.s64 = ctx.r11.s64 + -2324;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737120);
PPC_WEAK_FUNC(sub_82737120) { __imp__sub_82737120(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737120) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-2284
	ctx.r3.s64 = ctx.r11.s64 + -2284;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737130);
PPC_WEAK_FUNC(sub_82737130) { __imp__sub_82737130(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737130) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-2244
	ctx.r3.s64 = ctx.r11.s64 + -2244;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737140);
PPC_WEAK_FUNC(sub_82737140) { __imp__sub_82737140(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737140) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-2204
	ctx.r3.s64 = ctx.r11.s64 + -2204;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737150);
PPC_WEAK_FUNC(sub_82737150) { __imp__sub_82737150(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737150) {
	PPC_FUNC_PROLOGUE();
	// li r3,325
	ctx.r3.s64 = 325;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737158);
PPC_WEAK_FUNC(sub_82737158) { __imp__sub_82737158(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737158) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-2164
	ctx.r3.s64 = ctx.r11.s64 + -2164;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737168);
PPC_WEAK_FUNC(sub_82737168) { __imp__sub_82737168(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737168) {
	PPC_FUNC_PROLOGUE();
	// li r3,326
	ctx.r3.s64 = 326;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737170);
PPC_WEAK_FUNC(sub_82737170) { __imp__sub_82737170(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737170) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-2124
	ctx.r3.s64 = ctx.r11.s64 + -2124;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737180);
PPC_WEAK_FUNC(sub_82737180) { __imp__sub_82737180(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737180) {
	PPC_FUNC_PROLOGUE();
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,65534
	ctx.r11.u64 = ctx.r11.u64 | 65534;
	// cmpw cr6,r3,r11
	ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x8273721c
	if (ctx.cr6.gt) goto loc_8273721C;
	// cmplwi cr6,r3,65534
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 65534, ctx.xer);
	// beq cr6,0x82737214
	if (ctx.cr6.eq) goto loc_82737214;
	// addi r11,r3,-1
	ctx.r11.s64 = ctx.r3.s64 + -1;
	// cmplwi cr6,r11,17
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 17, ctx.xer);
	// bgt cr6,0x8273721c
	if (ctx.cr6.gt) goto loc_8273721C;
	// lis r12,-32141
	ctx.r12.s64 = -2106392576;
	// addi r12,r12,29116
	ctx.r12.s64 = ctx.r12.s64 + 29116;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_82737204;
	case 1:
		goto loc_82737204;
	case 2:
		goto loc_8273720C;
	case 3:
		goto loc_82737204;
	case 4:
		goto loc_82737204;
	case 5:
		goto loc_82737204;
	case 6:
		goto loc_82737204;
	case 7:
		goto loc_82737204;
	case 8:
		goto loc_82737214;
	case 9:
		goto loc_8273720C;
	case 10:
		goto loc_82737204;
	case 11:
		goto loc_8273720C;
	case 12:
		goto loc_8273720C;
	case 13:
		goto loc_82737204;
	case 14:
		goto loc_82737204;
	case 15:
		goto loc_82737204;
	case 16:
		goto loc_8273720C;
	case 17:
		goto loc_82737214;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_82737204:
	// li r3,3
	ctx.r3.s64 = 3;
	// blr 
	return;
loc_8273720C:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
loc_82737214:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
loc_8273721C:
	// li r3,-1
	ctx.r3.s64 = -1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737228);
PPC_WEAK_FUNC(sub_82737228) { __imp__sub_82737228(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737228) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-2084
	ctx.r3.s64 = ctx.r11.s64 + -2084;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737238);
PPC_WEAK_FUNC(sub_82737238) { __imp__sub_82737238(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737238) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-2044
	ctx.r3.s64 = ctx.r11.s64 + -2044;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737248);
PPC_WEAK_FUNC(sub_82737248) { __imp__sub_82737248(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737248) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-2004
	ctx.r3.s64 = ctx.r11.s64 + -2004;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737258);
PPC_WEAK_FUNC(sub_82737258) { __imp__sub_82737258(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737258) {
	PPC_FUNC_PROLOGUE();
	// li r3,330
	ctx.r3.s64 = 330;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737260);
PPC_WEAK_FUNC(sub_82737260) { __imp__sub_82737260(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737260) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-1964
	ctx.r3.s64 = ctx.r11.s64 + -1964;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737270);
PPC_WEAK_FUNC(sub_82737270) { __imp__sub_82737270(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737270) {
	PPC_FUNC_PROLOGUE();
	// li r3,331
	ctx.r3.s64 = 331;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737278);
PPC_WEAK_FUNC(sub_82737278) { __imp__sub_82737278(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737278) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-1924
	ctx.r3.s64 = ctx.r11.s64 + -1924;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737288);
PPC_WEAK_FUNC(sub_82737288) { __imp__sub_82737288(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737288) {
	PPC_FUNC_PROLOGUE();
	// li r3,332
	ctx.r3.s64 = 332;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737290);
PPC_WEAK_FUNC(sub_82737290) { __imp__sub_82737290(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737290) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-1884
	ctx.r3.s64 = ctx.r11.s64 + -1884;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827372A0);
PPC_WEAK_FUNC(sub_827372A0) { __imp__sub_827372A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827372A0) {
	PPC_FUNC_PROLOGUE();
	// li r3,333
	ctx.r3.s64 = 333;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827372A8);
PPC_WEAK_FUNC(sub_827372A8) { __imp__sub_827372A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827372A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-1844
	ctx.r3.s64 = ctx.r11.s64 + -1844;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827372B8);
PPC_WEAK_FUNC(sub_827372B8) { __imp__sub_827372B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827372B8) {
	PPC_FUNC_PROLOGUE();
	// li r3,334
	ctx.r3.s64 = 334;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827372C0);
PPC_WEAK_FUNC(sub_827372C0) { __imp__sub_827372C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827372C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-1804
	ctx.r3.s64 = ctx.r11.s64 + -1804;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827372D0);
PPC_WEAK_FUNC(sub_827372D0) { __imp__sub_827372D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827372D0) {
	PPC_FUNC_PROLOGUE();
	// li r3,335
	ctx.r3.s64 = 335;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827372D8);
PPC_WEAK_FUNC(sub_827372D8) { __imp__sub_827372D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827372D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-1764
	ctx.r3.s64 = ctx.r11.s64 + -1764;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827372E8);
PPC_WEAK_FUNC(sub_827372E8) { __imp__sub_827372E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827372E8) {
	PPC_FUNC_PROLOGUE();
	// li r3,336
	ctx.r3.s64 = 336;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827372F0);
PPC_WEAK_FUNC(sub_827372F0) { __imp__sub_827372F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827372F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-1724
	ctx.r3.s64 = ctx.r11.s64 + -1724;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737300);
PPC_WEAK_FUNC(sub_82737300) { __imp__sub_82737300(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737300) {
	PPC_FUNC_PROLOGUE();
	// li r3,337
	ctx.r3.s64 = 337;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737308);
PPC_WEAK_FUNC(sub_82737308) { __imp__sub_82737308(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737308) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-1684
	ctx.r3.s64 = ctx.r11.s64 + -1684;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737318);
PPC_WEAK_FUNC(sub_82737318) { __imp__sub_82737318(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737318) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-1644
	ctx.r3.s64 = ctx.r11.s64 + -1644;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737328);
PPC_WEAK_FUNC(sub_82737328) { __imp__sub_82737328(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737328) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-1604
	ctx.r3.s64 = ctx.r11.s64 + -1604;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737338);
PPC_WEAK_FUNC(sub_82737338) { __imp__sub_82737338(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737338) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-1564
	ctx.r3.s64 = ctx.r11.s64 + -1564;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737348);
PPC_WEAK_FUNC(sub_82737348) { __imp__sub_82737348(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737348) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-1524
	ctx.r3.s64 = ctx.r11.s64 + -1524;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737358);
PPC_WEAK_FUNC(sub_82737358) { __imp__sub_82737358(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737358) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-1484
	ctx.r3.s64 = ctx.r11.s64 + -1484;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737368);
PPC_WEAK_FUNC(sub_82737368) { __imp__sub_82737368(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737368) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-1444
	ctx.r3.s64 = ctx.r11.s64 + -1444;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737378);
PPC_WEAK_FUNC(sub_82737378) { __imp__sub_82737378(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737378) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-1404
	ctx.r3.s64 = ctx.r11.s64 + -1404;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737388);
PPC_WEAK_FUNC(sub_82737388) { __imp__sub_82737388(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737388) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-1364
	ctx.r3.s64 = ctx.r11.s64 + -1364;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737398);
PPC_WEAK_FUNC(sub_82737398) { __imp__sub_82737398(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737398) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-1324
	ctx.r3.s64 = ctx.r11.s64 + -1324;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827373A8);
PPC_WEAK_FUNC(sub_827373A8) { __imp__sub_827373A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827373A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-1284
	ctx.r3.s64 = ctx.r11.s64 + -1284;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827373B8);
PPC_WEAK_FUNC(sub_827373B8) { __imp__sub_827373B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827373B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-1244
	ctx.r3.s64 = ctx.r11.s64 + -1244;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827373C8);
PPC_WEAK_FUNC(sub_827373C8) { __imp__sub_827373C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827373C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-1204
	ctx.r3.s64 = ctx.r11.s64 + -1204;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827373D8);
PPC_WEAK_FUNC(sub_827373D8) { __imp__sub_827373D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827373D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-1164
	ctx.r3.s64 = ctx.r11.s64 + -1164;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827373E8);
PPC_WEAK_FUNC(sub_827373E8) { __imp__sub_827373E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827373E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-1124
	ctx.r3.s64 = ctx.r11.s64 + -1124;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827373F8);
PPC_WEAK_FUNC(sub_827373F8) { __imp__sub_827373F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827373F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-1084
	ctx.r3.s64 = ctx.r11.s64 + -1084;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737408);
PPC_WEAK_FUNC(sub_82737408) { __imp__sub_82737408(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737408) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-1044
	ctx.r3.s64 = ctx.r11.s64 + -1044;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737418);
PPC_WEAK_FUNC(sub_82737418) { __imp__sub_82737418(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737418) {
	PPC_FUNC_PROLOGUE();
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// b 0x82742c90
	sub_82742C90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737420);
PPC_WEAK_FUNC(sub_82737420) { __imp__sub_82737420(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737420) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-1004
	ctx.r3.s64 = ctx.r11.s64 + -1004;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737430);
PPC_WEAK_FUNC(sub_82737430) { __imp__sub_82737430(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737430) {
	PPC_FUNC_PROLOGUE();
	// li r3,368
	ctx.r3.s64 = 368;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737438);
PPC_WEAK_FUNC(sub_82737438) { __imp__sub_82737438(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737438) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-964
	ctx.r3.s64 = ctx.r11.s64 + -964;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737448);
PPC_WEAK_FUNC(sub_82737448) { __imp__sub_82737448(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737448) {
	PPC_FUNC_PROLOGUE();
	// li r3,369
	ctx.r3.s64 = 369;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737450);
PPC_WEAK_FUNC(sub_82737450) { __imp__sub_82737450(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737450) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-924
	ctx.r3.s64 = ctx.r11.s64 + -924;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737460);
PPC_WEAK_FUNC(sub_82737460) { __imp__sub_82737460(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737460) {
	PPC_FUNC_PROLOGUE();
	// li r3,370
	ctx.r3.s64 = 370;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737468);
PPC_WEAK_FUNC(sub_82737468) { __imp__sub_82737468(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737468) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-884
	ctx.r3.s64 = ctx.r11.s64 + -884;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737478);
PPC_WEAK_FUNC(sub_82737478) { __imp__sub_82737478(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737478) {
	PPC_FUNC_PROLOGUE();
	// li r3,371
	ctx.r3.s64 = 371;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737480);
PPC_WEAK_FUNC(sub_82737480) { __imp__sub_82737480(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737480) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-844
	ctx.r3.s64 = ctx.r11.s64 + -844;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737490);
PPC_WEAK_FUNC(sub_82737490) { __imp__sub_82737490(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737490) {
	PPC_FUNC_PROLOGUE();
	// li r3,372
	ctx.r3.s64 = 372;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737498);
PPC_WEAK_FUNC(sub_82737498) { __imp__sub_82737498(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737498) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-804
	ctx.r3.s64 = ctx.r11.s64 + -804;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827374A8);
PPC_WEAK_FUNC(sub_827374A8) { __imp__sub_827374A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827374A8) {
	PPC_FUNC_PROLOGUE();
	// li r3,373
	ctx.r3.s64 = 373;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827374B0);
PPC_WEAK_FUNC(sub_827374B0) { __imp__sub_827374B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827374B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-764
	ctx.r3.s64 = ctx.r11.s64 + -764;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827374C0);
PPC_WEAK_FUNC(sub_827374C0) { __imp__sub_827374C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827374C0) {
	PPC_FUNC_PROLOGUE();
	// li r3,374
	ctx.r3.s64 = 374;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827374C8);
PPC_WEAK_FUNC(sub_827374C8) { __imp__sub_827374C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827374C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-724
	ctx.r3.s64 = ctx.r11.s64 + -724;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827374D8);
PPC_WEAK_FUNC(sub_827374D8) { __imp__sub_827374D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827374D8) {
	PPC_FUNC_PROLOGUE();
	// li r3,375
	ctx.r3.s64 = 375;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827374E0);
PPC_WEAK_FUNC(sub_827374E0) { __imp__sub_827374E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827374E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-684
	ctx.r3.s64 = ctx.r11.s64 + -684;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827374F0);
PPC_WEAK_FUNC(sub_827374F0) { __imp__sub_827374F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827374F0) {
	PPC_FUNC_PROLOGUE();
	// li r3,376
	ctx.r3.s64 = 376;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827374F8);
PPC_WEAK_FUNC(sub_827374F8) { __imp__sub_827374F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827374F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-644
	ctx.r3.s64 = ctx.r11.s64 + -644;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737508);
PPC_WEAK_FUNC(sub_82737508) { __imp__sub_82737508(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737508) {
	PPC_FUNC_PROLOGUE();
	// li r3,377
	ctx.r3.s64 = 377;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737510);
PPC_WEAK_FUNC(sub_82737510) { __imp__sub_82737510(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737510) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-604
	ctx.r3.s64 = ctx.r11.s64 + -604;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737520);
PPC_WEAK_FUNC(sub_82737520) { __imp__sub_82737520(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737520) {
	PPC_FUNC_PROLOGUE();
	// li r3,378
	ctx.r3.s64 = 378;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737528);
PPC_WEAK_FUNC(sub_82737528) { __imp__sub_82737528(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737528) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-564
	ctx.r3.s64 = ctx.r11.s64 + -564;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737538);
PPC_WEAK_FUNC(sub_82737538) { __imp__sub_82737538(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737538) {
	PPC_FUNC_PROLOGUE();
	// li r3,379
	ctx.r3.s64 = 379;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737540);
PPC_WEAK_FUNC(sub_82737540) { __imp__sub_82737540(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737540) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-524
	ctx.r3.s64 = ctx.r11.s64 + -524;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737550);
PPC_WEAK_FUNC(sub_82737550) { __imp__sub_82737550(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737550) {
	PPC_FUNC_PROLOGUE();
	// li r3,380
	ctx.r3.s64 = 380;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737558);
PPC_WEAK_FUNC(sub_82737558) { __imp__sub_82737558(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737558) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-484
	ctx.r3.s64 = ctx.r11.s64 + -484;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737568);
PPC_WEAK_FUNC(sub_82737568) { __imp__sub_82737568(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737568) {
	PPC_FUNC_PROLOGUE();
	// li r3,381
	ctx.r3.s64 = 381;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737570);
PPC_WEAK_FUNC(sub_82737570) { __imp__sub_82737570(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737570) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-444
	ctx.r3.s64 = ctx.r11.s64 + -444;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737580);
PPC_WEAK_FUNC(sub_82737580) { __imp__sub_82737580(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737580) {
	PPC_FUNC_PROLOGUE();
	// li r3,382
	ctx.r3.s64 = 382;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737588);
PPC_WEAK_FUNC(sub_82737588) { __imp__sub_82737588(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737588) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-404
	ctx.r3.s64 = ctx.r11.s64 + -404;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737598);
PPC_WEAK_FUNC(sub_82737598) { __imp__sub_82737598(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737598) {
	PPC_FUNC_PROLOGUE();
	// li r3,383
	ctx.r3.s64 = 383;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827375A0);
PPC_WEAK_FUNC(sub_827375A0) { __imp__sub_827375A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827375A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-364
	ctx.r3.s64 = ctx.r11.s64 + -364;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827375B0);
PPC_WEAK_FUNC(sub_827375B0) { __imp__sub_827375B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827375B0) {
	PPC_FUNC_PROLOGUE();
	// li r3,384
	ctx.r3.s64 = 384;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827375B8);
PPC_WEAK_FUNC(sub_827375B8) { __imp__sub_827375B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827375B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-324
	ctx.r3.s64 = ctx.r11.s64 + -324;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827375C8);
PPC_WEAK_FUNC(sub_827375C8) { __imp__sub_827375C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827375C8) {
	PPC_FUNC_PROLOGUE();
	// li r3,385
	ctx.r3.s64 = 385;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827375D0);
PPC_WEAK_FUNC(sub_827375D0) { __imp__sub_827375D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827375D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-284
	ctx.r3.s64 = ctx.r11.s64 + -284;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827375E0);
PPC_WEAK_FUNC(sub_827375E0) { __imp__sub_827375E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827375E0) {
	PPC_FUNC_PROLOGUE();
	// li r3,386
	ctx.r3.s64 = 386;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827375E8);
PPC_WEAK_FUNC(sub_827375E8) { __imp__sub_827375E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827375E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-244
	ctx.r3.s64 = ctx.r11.s64 + -244;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827375F8);
PPC_WEAK_FUNC(sub_827375F8) { __imp__sub_827375F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827375F8) {
	PPC_FUNC_PROLOGUE();
	// li r3,387
	ctx.r3.s64 = 387;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737600);
PPC_WEAK_FUNC(sub_82737600) { __imp__sub_82737600(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737600) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-204
	ctx.r3.s64 = ctx.r11.s64 + -204;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737610);
PPC_WEAK_FUNC(sub_82737610) { __imp__sub_82737610(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737610) {
	PPC_FUNC_PROLOGUE();
	// li r3,388
	ctx.r3.s64 = 388;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737618);
PPC_WEAK_FUNC(sub_82737618) { __imp__sub_82737618(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737618) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-164
	ctx.r3.s64 = ctx.r11.s64 + -164;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737628);
PPC_WEAK_FUNC(sub_82737628) { __imp__sub_82737628(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737628) {
	PPC_FUNC_PROLOGUE();
	// li r3,389
	ctx.r3.s64 = 389;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737630);
PPC_WEAK_FUNC(sub_82737630) { __imp__sub_82737630(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737630) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-124
	ctx.r3.s64 = ctx.r11.s64 + -124;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737640);
PPC_WEAK_FUNC(sub_82737640) { __imp__sub_82737640(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737640) {
	PPC_FUNC_PROLOGUE();
	// li r3,390
	ctx.r3.s64 = 390;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737648);
PPC_WEAK_FUNC(sub_82737648) { __imp__sub_82737648(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737648) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-84
	ctx.r3.s64 = ctx.r11.s64 + -84;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737658);
PPC_WEAK_FUNC(sub_82737658) { __imp__sub_82737658(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737658) {
	PPC_FUNC_PROLOGUE();
	// li r3,391
	ctx.r3.s64 = 391;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737660);
PPC_WEAK_FUNC(sub_82737660) { __imp__sub_82737660(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737660) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-44
	ctx.r3.s64 = ctx.r11.s64 + -44;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737670);
PPC_WEAK_FUNC(sub_82737670) { __imp__sub_82737670(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737670) {
	PPC_FUNC_PROLOGUE();
	// li r3,392
	ctx.r3.s64 = 392;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737678);
PPC_WEAK_FUNC(sub_82737678) { __imp__sub_82737678(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737678) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,-4
	ctx.r3.s64 = ctx.r11.s64 + -4;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737688);
PPC_WEAK_FUNC(sub_82737688) { __imp__sub_82737688(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737688) {
	PPC_FUNC_PROLOGUE();
	// li r3,393
	ctx.r3.s64 = 393;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737690);
PPC_WEAK_FUNC(sub_82737690) { __imp__sub_82737690(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737690) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,36
	ctx.r3.s64 = ctx.r11.s64 + 36;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827376A0);
PPC_WEAK_FUNC(sub_827376A0) { __imp__sub_827376A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827376A0) {
	PPC_FUNC_PROLOGUE();
	// li r3,394
	ctx.r3.s64 = 394;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827376A8);
PPC_WEAK_FUNC(sub_827376A8) { __imp__sub_827376A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827376A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,76
	ctx.r3.s64 = ctx.r11.s64 + 76;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827376B8);
PPC_WEAK_FUNC(sub_827376B8) { __imp__sub_827376B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827376B8) {
	PPC_FUNC_PROLOGUE();
	// li r3,395
	ctx.r3.s64 = 395;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827376C0);
PPC_WEAK_FUNC(sub_827376C0) { __imp__sub_827376C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827376C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,116
	ctx.r3.s64 = ctx.r11.s64 + 116;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827376D0);
PPC_WEAK_FUNC(sub_827376D0) { __imp__sub_827376D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827376D0) {
	PPC_FUNC_PROLOGUE();
	// li r3,396
	ctx.r3.s64 = 396;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827376D8);
PPC_WEAK_FUNC(sub_827376D8) { __imp__sub_827376D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827376D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,156
	ctx.r3.s64 = ctx.r11.s64 + 156;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827376E8);
PPC_WEAK_FUNC(sub_827376E8) { __imp__sub_827376E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827376E8) {
	PPC_FUNC_PROLOGUE();
	// li r3,397
	ctx.r3.s64 = 397;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827376F0);
PPC_WEAK_FUNC(sub_827376F0) { __imp__sub_827376F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827376F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,196
	ctx.r3.s64 = ctx.r11.s64 + 196;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737700);
PPC_WEAK_FUNC(sub_82737700) { __imp__sub_82737700(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737700) {
	PPC_FUNC_PROLOGUE();
	// li r3,398
	ctx.r3.s64 = 398;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737708);
PPC_WEAK_FUNC(sub_82737708) { __imp__sub_82737708(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737708) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,236
	ctx.r3.s64 = ctx.r11.s64 + 236;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737718);
PPC_WEAK_FUNC(sub_82737718) { __imp__sub_82737718(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737718) {
	PPC_FUNC_PROLOGUE();
	// li r3,399
	ctx.r3.s64 = 399;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737720);
PPC_WEAK_FUNC(sub_82737720) { __imp__sub_82737720(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737720) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,276
	ctx.r3.s64 = ctx.r11.s64 + 276;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737730);
PPC_WEAK_FUNC(sub_82737730) { __imp__sub_82737730(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737730) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,316
	ctx.r3.s64 = ctx.r11.s64 + 316;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737740);
PPC_WEAK_FUNC(sub_82737740) { __imp__sub_82737740(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737740) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,356
	ctx.r3.s64 = ctx.r11.s64 + 356;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737750);
PPC_WEAK_FUNC(sub_82737750) { __imp__sub_82737750(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737750) {
	PPC_FUNC_PROLOGUE();
	// li r3,402
	ctx.r3.s64 = 402;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737758);
PPC_WEAK_FUNC(sub_82737758) { __imp__sub_82737758(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737758) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,396
	ctx.r3.s64 = ctx.r11.s64 + 396;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737768);
PPC_WEAK_FUNC(sub_82737768) { __imp__sub_82737768(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737768) {
	PPC_FUNC_PROLOGUE();
	// li r3,403
	ctx.r3.s64 = 403;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737770);
PPC_WEAK_FUNC(sub_82737770) { __imp__sub_82737770(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737770) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,436
	ctx.r3.s64 = ctx.r11.s64 + 436;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737780);
PPC_WEAK_FUNC(sub_82737780) { __imp__sub_82737780(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737780) {
	PPC_FUNC_PROLOGUE();
	// li r3,404
	ctx.r3.s64 = 404;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737788);
PPC_WEAK_FUNC(sub_82737788) { __imp__sub_82737788(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737788) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,476
	ctx.r3.s64 = ctx.r11.s64 + 476;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737798);
PPC_WEAK_FUNC(sub_82737798) { __imp__sub_82737798(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737798) {
	PPC_FUNC_PROLOGUE();
	// li r3,405
	ctx.r3.s64 = 405;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827377A0);
PPC_WEAK_FUNC(sub_827377A0) { __imp__sub_827377A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827377A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,516
	ctx.r3.s64 = ctx.r11.s64 + 516;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827377B0);
PPC_WEAK_FUNC(sub_827377B0) { __imp__sub_827377B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827377B0) {
	PPC_FUNC_PROLOGUE();
	// li r3,406
	ctx.r3.s64 = 406;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827377B8);
PPC_WEAK_FUNC(sub_827377B8) { __imp__sub_827377B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827377B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,556
	ctx.r3.s64 = ctx.r11.s64 + 556;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827377C8);
PPC_WEAK_FUNC(sub_827377C8) { __imp__sub_827377C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827377C8) {
	PPC_FUNC_PROLOGUE();
	// li r3,407
	ctx.r3.s64 = 407;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827377D0);
PPC_WEAK_FUNC(sub_827377D0) { __imp__sub_827377D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827377D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,596
	ctx.r3.s64 = ctx.r11.s64 + 596;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827377E0);
PPC_WEAK_FUNC(sub_827377E0) { __imp__sub_827377E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827377E0) {
	PPC_FUNC_PROLOGUE();
	// li r3,408
	ctx.r3.s64 = 408;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827377E8);
PPC_WEAK_FUNC(sub_827377E8) { __imp__sub_827377E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827377E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,636
	ctx.r3.s64 = ctx.r11.s64 + 636;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827377F8);
PPC_WEAK_FUNC(sub_827377F8) { __imp__sub_827377F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827377F8) {
	PPC_FUNC_PROLOGUE();
	// li r3,409
	ctx.r3.s64 = 409;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737800);
PPC_WEAK_FUNC(sub_82737800) { __imp__sub_82737800(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737800) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,676
	ctx.r3.s64 = ctx.r11.s64 + 676;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737810);
PPC_WEAK_FUNC(sub_82737810) { __imp__sub_82737810(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737810) {
	PPC_FUNC_PROLOGUE();
	// li r3,410
	ctx.r3.s64 = 410;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737818);
PPC_WEAK_FUNC(sub_82737818) { __imp__sub_82737818(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737818) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,716
	ctx.r3.s64 = ctx.r11.s64 + 716;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737828);
PPC_WEAK_FUNC(sub_82737828) { __imp__sub_82737828(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737828) {
	PPC_FUNC_PROLOGUE();
	// li r3,411
	ctx.r3.s64 = 411;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737830);
PPC_WEAK_FUNC(sub_82737830) { __imp__sub_82737830(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737830) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,756
	ctx.r3.s64 = ctx.r11.s64 + 756;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737840);
PPC_WEAK_FUNC(sub_82737840) { __imp__sub_82737840(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737840) {
	PPC_FUNC_PROLOGUE();
	// li r3,412
	ctx.r3.s64 = 412;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737848);
PPC_WEAK_FUNC(sub_82737848) { __imp__sub_82737848(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737848) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,796
	ctx.r3.s64 = ctx.r11.s64 + 796;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737858);
PPC_WEAK_FUNC(sub_82737858) { __imp__sub_82737858(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737858) {
	PPC_FUNC_PROLOGUE();
	// li r3,413
	ctx.r3.s64 = 413;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737860);
PPC_WEAK_FUNC(sub_82737860) { __imp__sub_82737860(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737860) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,836
	ctx.r3.s64 = ctx.r11.s64 + 836;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737870);
PPC_WEAK_FUNC(sub_82737870) { __imp__sub_82737870(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737870) {
	PPC_FUNC_PROLOGUE();
	// li r3,414
	ctx.r3.s64 = 414;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737878);
PPC_WEAK_FUNC(sub_82737878) { __imp__sub_82737878(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737878) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,876
	ctx.r3.s64 = ctx.r11.s64 + 876;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737888);
PPC_WEAK_FUNC(sub_82737888) { __imp__sub_82737888(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737888) {
	PPC_FUNC_PROLOGUE();
	// li r3,415
	ctx.r3.s64 = 415;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737890);
PPC_WEAK_FUNC(sub_82737890) { __imp__sub_82737890(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737890) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,916
	ctx.r3.s64 = ctx.r11.s64 + 916;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827378A0);
PPC_WEAK_FUNC(sub_827378A0) { __imp__sub_827378A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827378A0) {
	PPC_FUNC_PROLOGUE();
	// li r3,416
	ctx.r3.s64 = 416;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827378A8);
PPC_WEAK_FUNC(sub_827378A8) { __imp__sub_827378A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827378A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,956
	ctx.r3.s64 = ctx.r11.s64 + 956;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827378B8);
PPC_WEAK_FUNC(sub_827378B8) { __imp__sub_827378B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827378B8) {
	PPC_FUNC_PROLOGUE();
	// li r3,417
	ctx.r3.s64 = 417;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827378C0);
PPC_WEAK_FUNC(sub_827378C0) { __imp__sub_827378C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827378C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,996
	ctx.r3.s64 = ctx.r11.s64 + 996;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827378D0);
PPC_WEAK_FUNC(sub_827378D0) { __imp__sub_827378D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827378D0) {
	PPC_FUNC_PROLOGUE();
	// li r3,418
	ctx.r3.s64 = 418;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827378D8);
PPC_WEAK_FUNC(sub_827378D8) { __imp__sub_827378D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827378D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,1036
	ctx.r3.s64 = ctx.r11.s64 + 1036;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827378E8);
PPC_WEAK_FUNC(sub_827378E8) { __imp__sub_827378E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827378E8) {
	PPC_FUNC_PROLOGUE();
	// li r3,419
	ctx.r3.s64 = 419;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827378F0);
PPC_WEAK_FUNC(sub_827378F0) { __imp__sub_827378F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827378F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,1076
	ctx.r3.s64 = ctx.r11.s64 + 1076;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737900);
PPC_WEAK_FUNC(sub_82737900) { __imp__sub_82737900(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737900) {
	PPC_FUNC_PROLOGUE();
	// li r3,420
	ctx.r3.s64 = 420;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737908);
PPC_WEAK_FUNC(sub_82737908) { __imp__sub_82737908(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737908) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,1116
	ctx.r3.s64 = ctx.r11.s64 + 1116;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737918);
PPC_WEAK_FUNC(sub_82737918) { __imp__sub_82737918(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737918) {
	PPC_FUNC_PROLOGUE();
	// li r3,421
	ctx.r3.s64 = 421;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737920);
PPC_WEAK_FUNC(sub_82737920) { __imp__sub_82737920(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737920) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,1156
	ctx.r3.s64 = ctx.r11.s64 + 1156;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737930);
PPC_WEAK_FUNC(sub_82737930) { __imp__sub_82737930(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737930) {
	PPC_FUNC_PROLOGUE();
	// li r3,422
	ctx.r3.s64 = 422;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737938);
PPC_WEAK_FUNC(sub_82737938) { __imp__sub_82737938(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737938) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,1196
	ctx.r3.s64 = ctx.r11.s64 + 1196;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737948);
PPC_WEAK_FUNC(sub_82737948) { __imp__sub_82737948(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737948) {
	PPC_FUNC_PROLOGUE();
	// li r3,423
	ctx.r3.s64 = 423;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737950);
PPC_WEAK_FUNC(sub_82737950) { __imp__sub_82737950(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737950) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,1236
	ctx.r3.s64 = ctx.r11.s64 + 1236;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737960);
PPC_WEAK_FUNC(sub_82737960) { __imp__sub_82737960(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737960) {
	PPC_FUNC_PROLOGUE();
	// li r3,424
	ctx.r3.s64 = 424;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737968);
PPC_WEAK_FUNC(sub_82737968) { __imp__sub_82737968(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737968) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,1276
	ctx.r3.s64 = ctx.r11.s64 + 1276;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737978);
PPC_WEAK_FUNC(sub_82737978) { __imp__sub_82737978(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737978) {
	PPC_FUNC_PROLOGUE();
	// li r3,425
	ctx.r3.s64 = 425;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737980);
PPC_WEAK_FUNC(sub_82737980) { __imp__sub_82737980(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737980) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,1316
	ctx.r3.s64 = ctx.r11.s64 + 1316;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737990);
PPC_WEAK_FUNC(sub_82737990) { __imp__sub_82737990(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737990) {
	PPC_FUNC_PROLOGUE();
	// li r3,426
	ctx.r3.s64 = 426;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737998);
PPC_WEAK_FUNC(sub_82737998) { __imp__sub_82737998(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737998) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,1356
	ctx.r3.s64 = ctx.r11.s64 + 1356;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827379A8);
PPC_WEAK_FUNC(sub_827379A8) { __imp__sub_827379A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827379A8) {
	PPC_FUNC_PROLOGUE();
	// li r3,427
	ctx.r3.s64 = 427;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827379B0);
PPC_WEAK_FUNC(sub_827379B0) { __imp__sub_827379B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827379B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,1396
	ctx.r3.s64 = ctx.r11.s64 + 1396;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827379C0);
PPC_WEAK_FUNC(sub_827379C0) { __imp__sub_827379C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827379C0) {
	PPC_FUNC_PROLOGUE();
	// li r3,428
	ctx.r3.s64 = 428;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827379C8);
PPC_WEAK_FUNC(sub_827379C8) { __imp__sub_827379C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827379C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,1436
	ctx.r3.s64 = ctx.r11.s64 + 1436;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827379D8);
PPC_WEAK_FUNC(sub_827379D8) { __imp__sub_827379D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827379D8) {
	PPC_FUNC_PROLOGUE();
	// li r3,429
	ctx.r3.s64 = 429;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827379E0);
PPC_WEAK_FUNC(sub_827379E0) { __imp__sub_827379E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827379E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,1476
	ctx.r3.s64 = ctx.r11.s64 + 1476;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827379F0);
PPC_WEAK_FUNC(sub_827379F0) { __imp__sub_827379F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827379F0) {
	PPC_FUNC_PROLOGUE();
	// li r3,430
	ctx.r3.s64 = 430;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_827379F8);
PPC_WEAK_FUNC(sub_827379F8) { __imp__sub_827379F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_827379F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,1516
	ctx.r3.s64 = ctx.r11.s64 + 1516;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737A08);
PPC_WEAK_FUNC(sub_82737A08) { __imp__sub_82737A08(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737A08) {
	PPC_FUNC_PROLOGUE();
	// li r3,431
	ctx.r3.s64 = 431;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737A10);
PPC_WEAK_FUNC(sub_82737A10) { __imp__sub_82737A10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737A10) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,1556
	ctx.r3.s64 = ctx.r11.s64 + 1556;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737A20);
PPC_WEAK_FUNC(sub_82737A20) { __imp__sub_82737A20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737A20) {
	PPC_FUNC_PROLOGUE();
	// li r3,432
	ctx.r3.s64 = 432;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737A28);
PPC_WEAK_FUNC(sub_82737A28) { __imp__sub_82737A28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737A28) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,1596
	ctx.r3.s64 = ctx.r11.s64 + 1596;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737A38);
PPC_WEAK_FUNC(sub_82737A38) { __imp__sub_82737A38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737A38) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,1636
	ctx.r3.s64 = ctx.r11.s64 + 1636;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737A48);
PPC_WEAK_FUNC(sub_82737A48) { __imp__sub_82737A48(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737A48) {
	PPC_FUNC_PROLOGUE();
	// li r3,434
	ctx.r3.s64 = 434;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737A50);
PPC_WEAK_FUNC(sub_82737A50) { __imp__sub_82737A50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737A50) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,1676
	ctx.r3.s64 = ctx.r11.s64 + 1676;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737A60);
PPC_WEAK_FUNC(sub_82737A60) { __imp__sub_82737A60(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737A60) {
	PPC_FUNC_PROLOGUE();
	// li r3,435
	ctx.r3.s64 = 435;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737A68);
PPC_WEAK_FUNC(sub_82737A68) { __imp__sub_82737A68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737A68) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,1716
	ctx.r3.s64 = ctx.r11.s64 + 1716;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737A78);
PPC_WEAK_FUNC(sub_82737A78) { __imp__sub_82737A78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737A78) {
	PPC_FUNC_PROLOGUE();
	// li r3,436
	ctx.r3.s64 = 436;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737A80);
PPC_WEAK_FUNC(sub_82737A80) { __imp__sub_82737A80(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737A80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,1756
	ctx.r3.s64 = ctx.r11.s64 + 1756;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737A90);
PPC_WEAK_FUNC(sub_82737A90) { __imp__sub_82737A90(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737A90) {
	PPC_FUNC_PROLOGUE();
	// li r3,437
	ctx.r3.s64 = 437;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737A98);
PPC_WEAK_FUNC(sub_82737A98) { __imp__sub_82737A98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737A98) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,1796
	ctx.r3.s64 = ctx.r11.s64 + 1796;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737AA8);
PPC_WEAK_FUNC(sub_82737AA8) { __imp__sub_82737AA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737AA8) {
	PPC_FUNC_PROLOGUE();
	// li r3,438
	ctx.r3.s64 = 438;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737AB0);
PPC_WEAK_FUNC(sub_82737AB0) { __imp__sub_82737AB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737AB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,1836
	ctx.r3.s64 = ctx.r11.s64 + 1836;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737AC0);
PPC_WEAK_FUNC(sub_82737AC0) { __imp__sub_82737AC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737AC0) {
	PPC_FUNC_PROLOGUE();
	// li r3,439
	ctx.r3.s64 = 439;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737AC8);
PPC_WEAK_FUNC(sub_82737AC8) { __imp__sub_82737AC8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737AC8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,1876
	ctx.r3.s64 = ctx.r11.s64 + 1876;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737AD8);
PPC_WEAK_FUNC(sub_82737AD8) { __imp__sub_82737AD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737AD8) {
	PPC_FUNC_PROLOGUE();
	// li r3,440
	ctx.r3.s64 = 440;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737AE0);
PPC_WEAK_FUNC(sub_82737AE0) { __imp__sub_82737AE0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737AE0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r3,r11,1916
	ctx.r3.s64 = ctx.r11.s64 + 1916;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82737AF0);
PPC_WEAK_FUNC(sub_82737AF0) { __imp__sub_82737AF0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82737AF0) {
	PPC_FUNC_PROLOGUE();
	// li r3,441
	ctx.r3.s64 = 441;
	// blr 
	return;
}

