#include "gta4_init.h"

PPC_FUNC_IMPL(__imp__sub_82A60768);
PPC_WEAK_FUNC(sub_82A60768) { __imp__sub_82A60768(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60768) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// li r9,-1
	ctx.r9.s64 = -1;
	// addi r11,r11,12304
	ctx.r11.s64 = ctx.r11.s64 + 12304;
	// li r10,57
	ctx.r10.s64 = 57;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_82A6077C:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x82a6077c
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_82A6077C;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60790);
PPC_WEAK_FUNC(sub_82A60790) { __imp__sub_82A60790(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60790) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,22176
	ctx.r3.s64 = ctx.r11.s64 + 22176;
	// bl 0x8285fe48
	ctx.lr = 0x82A607A8;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,2184
	ctx.r3.s64 = ctx.r11.s64 + 2184;
	// bl 0x829ffa48
	ctx.lr = 0x82A607B4;
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

PPC_FUNC_IMPL(__imp__sub_82A607C8);
PPC_WEAK_FUNC(sub_82A607C8) { __imp__sub_82A607C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A607C8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,22208
	ctx.r3.s64 = ctx.r11.s64 + 22208;
	// bl 0x8285fe48
	ctx.lr = 0x82A607E0;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,2200
	ctx.r3.s64 = ctx.r11.s64 + 2200;
	// bl 0x829ffa48
	ctx.lr = 0x82A607EC;
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

PPC_FUNC_IMPL(__imp__sub_82A60800);
PPC_WEAK_FUNC(sub_82A60800) { __imp__sub_82A60800(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60800) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,22240
	ctx.r3.s64 = ctx.r11.s64 + 22240;
	// bl 0x8285fe48
	ctx.lr = 0x82A60818;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,2216
	ctx.r3.s64 = ctx.r11.s64 + 2216;
	// bl 0x829ffa48
	ctx.lr = 0x82A60824;
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

PPC_FUNC_IMPL(__imp__sub_82A60838);
PPC_WEAK_FUNC(sub_82A60838) { __imp__sub_82A60838(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60838) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,22272
	ctx.r3.s64 = ctx.r11.s64 + 22272;
	// bl 0x8285fe48
	ctx.lr = 0x82A60850;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,2232
	ctx.r3.s64 = ctx.r11.s64 + 2232;
	// bl 0x829ffa48
	ctx.lr = 0x82A6085C;
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

PPC_FUNC_IMPL(__imp__sub_82A60870);
PPC_WEAK_FUNC(sub_82A60870) { __imp__sub_82A60870(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60870) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// li r10,15
	ctx.r10.s64 = 15;
	// addi r7,r11,22304
	ctx.r7.s64 = ctx.r11.s64 + 22304;
	// li r8,-1
	ctx.r8.s64 = -1;
	// addi r11,r7,8
	ctx.r11.s64 = ctx.r7.s64 + 8;
	// li r9,0
	ctx.r9.s64 = 0;
loc_82A60888:
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
	// stw r8,-8(r11)
	PPC_STORE_U32(ctx.r11.u32 + -8, ctx.r8.u32);
	// stw r9,-4(r11)
	PPC_STORE_U32(ctx.r11.u32 + -4, ctx.r9.u32);
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// stw r8,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r8.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// addi r11,r11,20
	ctx.r11.s64 = ctx.r11.s64 + 20;
	// bge cr6,0x82a60888
	if (!ctx.cr6.lt) goto loc_82A60888;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// stw r9,320(r7)
	PPC_STORE_U32(ctx.r7.u32 + 320, ctx.r9.u32);
	// addi r3,r11,2448
	ctx.r3.s64 = ctx.r11.s64 + 2448;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A608C0);
PPC_WEAK_FUNC(sub_82A608C0) { __imp__sub_82A608C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A608C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,2248
	ctx.r3.s64 = ctx.r11.s64 + 2248;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A608D0);
PPC_WEAK_FUNC(sub_82A608D0) { __imp__sub_82A608D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A608D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,2464
	ctx.r3.s64 = ctx.r11.s64 + 2464;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A608E0);
PPC_WEAK_FUNC(sub_82A608E0) { __imp__sub_82A608E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A608E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,2496
	ctx.r3.s64 = ctx.r11.s64 + 2496;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A608F0);
PPC_WEAK_FUNC(sub_82A608F0) { __imp__sub_82A608F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A608F0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7cc
	ctx.lr = 0x82A608F8;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// li r30,31
	ctx.r30.s64 = 31;
	// addi r11,r11,25792
	ctx.r11.s64 = ctx.r11.s64 + 25792;
	// addi r31,r11,144
	ctx.r31.s64 = ctx.r11.s64 + 144;
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r29,r11,65535
	ctx.r29.u64 = ctx.r11.u64 | 65535;
loc_82A60914:
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// li r5,17
	ctx.r5.s64 = 17;
	// addi r6,r11,-13680
	ctx.r6.s64 = ctx.r11.s64 + -13680;
	// li r4,16
	ctx.r4.s64 = 16;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82140220
	ctx.lr = 0x82A6092C;
	sub_82140220(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// li r5,8
	ctx.r5.s64 = 8;
	// addi r6,r11,-13680
	ctx.r6.s64 = ctx.r11.s64 + -13680;
	// li r4,32
	ctx.r4.s64 = 32;
	// addi r3,r31,416
	ctx.r3.s64 = ctx.r31.s64 + 416;
	// bl 0x82140220
	ctx.lr = 0x82A60944;
	sub_82140220(ctx, base);
	// lwz r11,688(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 688);
	// addi r3,r31,768
	ctx.r3.s64 = ctx.r31.s64 + 768;
	// sth r29,692(r31)
	PPC_STORE_U16(ctx.r31.u32 + 692, ctx.r29.u16);
	// clrlwi r11,r11,4
	ctx.r11.u64 = ctx.r11.u32 & 0xFFFFFFF;
	// stw r11,688(r31)
	PPC_STORE_U32(ctx.r31.u32 + 688, ctx.r11.u32);
	// bl 0x826905e8
	ctx.lr = 0x82A6095C;
	sub_826905E8(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// addi r31,r31,976
	ctx.r31.s64 = ctx.r31.s64 + 976;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a60914
	if (!ctx.cr6.lt) goto loc_82A60914;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,2304
	ctx.r3.s64 = ctx.r11.s64 + 2304;
	// bl 0x829ffa48
	ctx.lr = 0x82A60978;
	sub_829FFA48(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60980);
PPC_WEAK_FUNC(sub_82A60980) { __imp__sub_82A60980(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60980) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32004
	ctx.r11.s64 = -2097414144;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,-8512
	ctx.r11.s64 = ctx.r11.s64 + -8512;
	// lis r9,-32089
	ctx.r9.s64 = -2102984704;
	// addi r3,r9,2312
	ctx.r3.s64 = ctx.r9.s64 + 2312;
	// stw r10,56(r11)
	PPC_STORE_U32(ctx.r11.u32 + 56, ctx.r10.u32);
	// stw r10,648(r11)
	PPC_STORE_U32(ctx.r11.u32 + 648, ctx.r10.u32);
	// stw r10,1240(r11)
	PPC_STORE_U32(ctx.r11.u32 + 1240, ctx.r10.u32);
	// stw r10,1832(r11)
	PPC_STORE_U32(ctx.r11.u32 + 1832, ctx.r10.u32);
	// stw r10,2424(r11)
	PPC_STORE_U32(ctx.r11.u32 + 2424, ctx.r10.u32);
	// stw r10,3016(r11)
	PPC_STORE_U32(ctx.r11.u32 + 3016, ctx.r10.u32);
	// stw r10,3608(r11)
	PPC_STORE_U32(ctx.r11.u32 + 3608, ctx.r10.u32);
	// stw r10,4200(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4200, ctx.r10.u32);
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A609B8);
PPC_WEAK_FUNC(sub_82A609B8) { __imp__sub_82A609B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A609B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,2320
	ctx.r3.s64 = ctx.r11.s64 + 2320;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A609C8);
PPC_WEAK_FUNC(sub_82A609C8) { __imp__sub_82A609C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A609C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,2328
	ctx.r3.s64 = ctx.r11.s64 + 2328;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A609D8);
PPC_WEAK_FUNC(sub_82A609D8) { __imp__sub_82A609D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A609D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,2336
	ctx.r3.s64 = ctx.r11.s64 + 2336;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A609E8);
PPC_WEAK_FUNC(sub_82A609E8) { __imp__sub_82A609E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A609E8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32004
	ctx.r11.s64 = -2097414144;
	// addi r3,r11,11968
	ctx.r3.s64 = ctx.r11.s64 + 11968;
	// bl 0x826998c0
	ctx.lr = 0x82A60A00;
	sub_826998C0(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,2344
	ctx.r3.s64 = ctx.r11.s64 + 2344;
	// bl 0x829ffa48
	ctx.lr = 0x82A60A0C;
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

PPC_FUNC_IMPL(__imp__sub_82A60A20);
PPC_WEAK_FUNC(sub_82A60A20) { __imp__sub_82A60A20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60A20) {
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
	// li r31,15
	ctx.r31.s64 = 15;
	// addi r30,r11,16336
	ctx.r30.s64 = ctx.r11.s64 + 16336;
loc_82A60A40:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x822bca90
	ctx.lr = 0x82A60A48;
	sub_822BCA90(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,32
	ctx.r30.s64 = ctx.r30.s64 + 32;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x82a60a40
	if (!ctx.cr6.lt) goto loc_82A60A40;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,2360
	ctx.r3.s64 = ctx.r11.s64 + 2360;
	// bl 0x829ffa48
	ctx.lr = 0x82A60A64;
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

PPC_FUNC_IMPL(__imp__sub_82A60A80);
PPC_WEAK_FUNC(sub_82A60A80) { __imp__sub_82A60A80(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60A80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,16232
	ctx.r5.s64 = ctx.r11.s64 + 16232;
	// lis r11,-32004
	ctx.r11.s64 = -2097414144;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,20096
	ctx.r3.s64 = ctx.r11.s64 + 20096;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60AA0);
PPC_WEAK_FUNC(sub_82A60AA0) { __imp__sub_82A60AA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60AA0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32004
	ctx.r11.s64 = -2097414144;
	// li r9,-1
	ctx.r9.s64 = -1;
	// addi r11,r11,20120
	ctx.r11.s64 = ctx.r11.s64 + 20120;
	// li r10,800
	ctx.r10.s64 = 800;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_82A60AB4:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x82a60ab4
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_82A60AB4;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60AC8);
PPC_WEAK_FUNC(sub_82A60AC8) { __imp__sub_82A60AC8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60AC8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32004
	ctx.r11.s64 = -2097414144;
	// addi r31,r11,23552
	ctx.r31.s64 = ctx.r11.s64 + 23552;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82902b70
	ctx.lr = 0x82A60AE8;
	sub_82902B70(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,17192
	ctx.r11.s64 = ctx.r11.s64 + 17192;
	// addi r3,r10,2528
	ctx.r3.s64 = ctx.r10.s64 + 2528;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,92(r31)
	PPC_STORE_U32(ctx.r31.u32 + 92, ctx.r11.u32);
	// sth r11,96(r31)
	PPC_STORE_U16(ctx.r31.u32 + 96, ctx.r11.u16);
	// sth r11,98(r31)
	PPC_STORE_U16(ctx.r31.u32 + 98, ctx.r11.u16);
	// bl 0x829ffa48
	ctx.lr = 0x82A60B10;
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

PPC_FUNC_IMPL(__imp__sub_82A60B28);
PPC_WEAK_FUNC(sub_82A60B28) { __imp__sub_82A60B28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60B28) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,18760
	ctx.r3.s64 = ctx.r11.s64 + 18760;
	// bl 0x8284e060
	ctx.lr = 0x82A60B48;
	sub_8284E060(ctx, base);
	// lis r10,-32003
	ctx.r10.s64 = -2097348608;
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r31,r10,-27904
	ctx.r31.s64 = ctx.r10.s64 + -27904;
	// addi r11,r11,18736
	ctx.r11.s64 = ctx.r11.s64 + 18736;
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A60B68;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18708
	ctx.r11.s64 = ctx.r11.s64 + 18708;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A60B80;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18688
	ctx.r11.s64 = ctx.r11.s64 + 18688;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A60B98;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18660
	ctx.r11.s64 = ctx.r11.s64 + 18660;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A60BB0;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18080
	ctx.r11.s64 = ctx.r11.s64 + 18080;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A60BC8;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,20(r31)
	PPC_STORE_U32(ctx.r31.u32 + 20, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18064
	ctx.r11.s64 = ctx.r11.s64 + 18064;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A60BE0;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,24(r31)
	PPC_STORE_U32(ctx.r31.u32 + 24, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18128
	ctx.r11.s64 = ctx.r11.s64 + 18128;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A60BF8;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,28(r31)
	PPC_STORE_U32(ctx.r31.u32 + 28, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18044
	ctx.r11.s64 = ctx.r11.s64 + 18044;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A60C10;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,32(r31)
	PPC_STORE_U32(ctx.r31.u32 + 32, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18096
	ctx.r11.s64 = ctx.r11.s64 + 18096;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A60C28;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,36(r31)
	PPC_STORE_U32(ctx.r31.u32 + 36, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18644
	ctx.r11.s64 = ctx.r11.s64 + 18644;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A60C40;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18628
	ctx.r11.s64 = ctx.r11.s64 + 18628;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A60C58;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,44(r31)
	PPC_STORE_U32(ctx.r31.u32 + 44, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18604
	ctx.r11.s64 = ctx.r11.s64 + 18604;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A60C70;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,48(r31)
	PPC_STORE_U32(ctx.r31.u32 + 48, ctx.r3.u32);
	// addi r11,r11,18580
	ctx.r11.s64 = ctx.r11.s64 + 18580;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A60C88;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,52(r31)
	PPC_STORE_U32(ctx.r31.u32 + 52, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18556
	ctx.r11.s64 = ctx.r11.s64 + 18556;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A60CA0;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,56(r31)
	PPC_STORE_U32(ctx.r31.u32 + 56, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18520
	ctx.r11.s64 = ctx.r11.s64 + 18520;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A60CB8;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,60(r31)
	PPC_STORE_U32(ctx.r31.u32 + 60, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,18500
	ctx.r3.s64 = ctx.r11.s64 + 18500;
	// bl 0x8284e060
	ctx.lr = 0x82A60CCC;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,64(r31)
	PPC_STORE_U32(ctx.r31.u32 + 64, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18480
	ctx.r11.s64 = ctx.r11.s64 + 18480;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A60CE4;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,68(r31)
	PPC_STORE_U32(ctx.r31.u32 + 68, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18460
	ctx.r11.s64 = ctx.r11.s64 + 18460;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A60CFC;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,72(r31)
	PPC_STORE_U32(ctx.r31.u32 + 72, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18444
	ctx.r11.s64 = ctx.r11.s64 + 18444;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A60D14;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,76(r31)
	PPC_STORE_U32(ctx.r31.u32 + 76, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18420
	ctx.r11.s64 = ctx.r11.s64 + 18420;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A60D2C;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,80(r31)
	PPC_STORE_U32(ctx.r31.u32 + 80, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18404
	ctx.r11.s64 = ctx.r11.s64 + 18404;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A60D44;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,84(r31)
	PPC_STORE_U32(ctx.r31.u32 + 84, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18388
	ctx.r11.s64 = ctx.r11.s64 + 18388;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A60D5C;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,88(r31)
	PPC_STORE_U32(ctx.r31.u32 + 88, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18372
	ctx.r11.s64 = ctx.r11.s64 + 18372;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A60D74;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,92(r31)
	PPC_STORE_U32(ctx.r31.u32 + 92, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18356
	ctx.r11.s64 = ctx.r11.s64 + 18356;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A60D8C;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,96(r31)
	PPC_STORE_U32(ctx.r31.u32 + 96, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18112
	ctx.r11.s64 = ctx.r11.s64 + 18112;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A60DA4;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,100(r31)
	PPC_STORE_U32(ctx.r31.u32 + 100, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18336
	ctx.r11.s64 = ctx.r11.s64 + 18336;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A60DBC;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r11,r11,18316
	ctx.r11.s64 = ctx.r11.s64 + 18316;
	// stw r3,104(r31)
	PPC_STORE_U32(ctx.r31.u32 + 104, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A60DD4;
	sub_8284E060(ctx, base);
	// stw r3,108(r31)
	PPC_STORE_U32(ctx.r31.u32 + 108, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60DF0);
PPC_WEAK_FUNC(sub_82A60DF0) { __imp__sub_82A60DF0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60DF0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,19008
	ctx.r5.s64 = ctx.r11.s64 + 19008;
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-27508
	ctx.r3.s64 = ctx.r11.s64 + -27508;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60E10);
PPC_WEAK_FUNC(sub_82A60E10) { __imp__sub_82A60E10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60E10) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// li r9,-1
	ctx.r9.s64 = -1;
	// addi r11,r11,-26720
	ctx.r11.s64 = ctx.r11.s64 + -26720;
	// li r10,32
	ctx.r10.s64 = 32;
	// addi r11,r11,6852
	ctx.r11.s64 = ctx.r11.s64 + 6852;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_82A60E28:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x82a60e28
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_82A60E28;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60E38);
PPC_WEAK_FUNC(sub_82A60E38) { __imp__sub_82A60E38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60E38) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,19252
	ctx.r5.s64 = ctx.r11.s64 + 19252;
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-19540
	ctx.r3.s64 = ctx.r11.s64 + -19540;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60E58);
PPC_WEAK_FUNC(sub_82A60E58) { __imp__sub_82A60E58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60E58) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// li r9,-1
	ctx.r9.s64 = -1;
	// addi r11,r11,-16960
	ctx.r11.s64 = ctx.r11.s64 + -16960;
	// li r10,5000
	ctx.r10.s64 = 5000;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_82A60E6C:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x82a60e6c
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_82A60E6C;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60E80);
PPC_WEAK_FUNC(sub_82A60E80) { __imp__sub_82A60E80(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60E80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// li r9,-1
	ctx.r9.s64 = -1;
	// addi r10,r11,3040
	ctx.r10.s64 = ctx.r11.s64 + 3040;
	// li r11,63
	ctx.r11.s64 = 63;
loc_82A60E90:
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// addi r10,r10,8
	ctx.r10.s64 = ctx.r10.s64 + 8;
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// bge cr6,0x82a60e90
	if (!ctx.cr6.lt) goto loc_82A60E90;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60EA8);
PPC_WEAK_FUNC(sub_82A60EA8) { __imp__sub_82A60EA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60EA8) {
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
	// li r31,3
	ctx.r31.s64 = 3;
	// addi r30,r11,7756
	ctx.r30.s64 = ctx.r11.s64 + 7756;
loc_82A60EC8:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82259f50
	ctx.lr = 0x82A60ED0;
	sub_82259F50(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,16
	ctx.r30.s64 = ctx.r30.s64 + 16;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x82a60ec8
	if (!ctx.cr6.lt) goto loc_82A60EC8;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,2624
	ctx.r3.s64 = ctx.r11.s64 + 2624;
	// bl 0x829ffa48
	ctx.lr = 0x82A60EEC;
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

PPC_FUNC_IMPL(__imp__sub_82A60F08);
PPC_WEAK_FUNC(sub_82A60F08) { __imp__sub_82A60F08(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60F08) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,2712
	ctx.r3.s64 = ctx.r11.s64 + 2712;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60F18);
PPC_WEAK_FUNC(sub_82A60F18) { __imp__sub_82A60F18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60F18) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,29940
	ctx.r5.s64 = ctx.r11.s64 + 29940;
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,16184
	ctx.r3.s64 = ctx.r11.s64 + 16184;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60F38);
PPC_WEAK_FUNC(sub_82A60F38) { __imp__sub_82A60F38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60F38) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,29948
	ctx.r5.s64 = ctx.r11.s64 + 29948;
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,16204
	ctx.r3.s64 = ctx.r11.s64 + 16204;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60F58);
PPC_WEAK_FUNC(sub_82A60F58) { __imp__sub_82A60F58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60F58) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,29960
	ctx.r5.s64 = ctx.r11.s64 + 29960;
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,16640
	ctx.r3.s64 = ctx.r11.s64 + 16640;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60F78);
PPC_WEAK_FUNC(sub_82A60F78) { __imp__sub_82A60F78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60F78) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,29972
	ctx.r5.s64 = ctx.r11.s64 + 29972;
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,16660
	ctx.r3.s64 = ctx.r11.s64 + 16660;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60F98);
PPC_WEAK_FUNC(sub_82A60F98) { __imp__sub_82A60F98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60F98) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// lfs f0,10752(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 10752);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// fmuls f0,f0,f0
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f0.f64));
	// stfs f0,16680(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 16680, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60FB0);
PPC_WEAK_FUNC(sub_82A60FB0) { __imp__sub_82A60FB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60FB0) {
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
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfd f1,-31840(r11)
	ctx.fpscr.disableFlushMode();
	ctx.f1.u64 = PPC_LOAD_U64(ctx.r11.u32 + -31840);
	// bl 0x829ffe18
	ctx.lr = 0x82A60FC8;
	sub_829FFE18(ctx, base);
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// frsp f0,f1
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(float(ctx.f1.f64));
	// stfs f0,16748(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 16748, temp.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60FE8);
PPC_WEAK_FUNC(sub_82A60FE8) { __imp__sub_82A60FE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60FE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,2720
	ctx.r3.s64 = ctx.r11.s64 + 2720;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60FF8);
PPC_WEAK_FUNC(sub_82A60FF8) { __imp__sub_82A60FF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60FF8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,16960
	ctx.r3.s64 = ctx.r11.s64 + 16960;
	// bl 0x82400e28
	ctx.lr = 0x82A61010;
	sub_82400E28(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,2728
	ctx.r3.s64 = ctx.r11.s64 + 2728;
	// bl 0x829ffa48
	ctx.lr = 0x82A6101C;
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

PPC_FUNC_IMPL(__imp__sub_82A61030);
PPC_WEAK_FUNC(sub_82A61030) { __imp__sub_82A61030(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61030) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,17688
	ctx.r3.s64 = ctx.r11.s64 + 17688;
	// bl 0x82412980
	ctx.lr = 0x82A61048;
	sub_82412980(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,2736
	ctx.r3.s64 = ctx.r11.s64 + 2736;
	// bl 0x829ffa48
	ctx.lr = 0x82A61054;
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

PPC_FUNC_IMPL(__imp__sub_82A61068);
PPC_WEAK_FUNC(sub_82A61068) { __imp__sub_82A61068(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61068) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// lfs f0,30016(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 30016);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32003
	ctx.r11.s64 = -2097348608;
	// stfs f0,18028(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 18028, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61080);
PPC_WEAK_FUNC(sub_82A61080) { __imp__sub_82A61080(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61080) {
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
	// li r31,255
	ctx.r31.s64 = 255;
	// addi r11,r11,18048
	ctx.r11.s64 = ctx.r11.s64 + 18048;
	// addi r30,r11,388
	ctx.r30.s64 = ctx.r11.s64 + 388;
loc_82A610A4:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8268a1e0
	ctx.lr = 0x82A610AC;
	sub_8268A1E0(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,224
	ctx.r30.s64 = ctx.r30.s64 + 224;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x82a610a4
	if (!ctx.cr6.lt) goto loc_82A610A4;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,2744
	ctx.r3.s64 = ctx.r11.s64 + 2744;
	// bl 0x829ffa48
	ctx.lr = 0x82A610C8;
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

PPC_FUNC_IMPL(__imp__sub_82A610E0);
PPC_WEAK_FUNC(sub_82A610E0) { __imp__sub_82A610E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A610E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c4
	ctx.lr = 0x82A610E8;
	__savegprlr_27(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32002
	ctx.r11.s64 = -2097283072;
	// li r27,127
	ctx.r27.s64 = 127;
	// addi r11,r11,27328
	ctx.r11.s64 = ctx.r11.s64 + 27328;
	// li r30,0
	ctx.r30.s64 = 0;
	// addi r31,r11,20
	ctx.r31.s64 = ctx.r11.s64 + 20;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// addi r29,r11,-24732
	ctx.r29.s64 = ctx.r11.s64 + -24732;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// addi r28,r11,-23716
	ctx.r28.s64 = ctx.r11.s64 + -23716;
loc_82A61110:
	// addi r3,r31,-20
	ctx.r3.s64 = ctx.r31.s64 + -20;
	// bl 0x82412290
	ctx.lr = 0x82A61118;
	sub_82412290(ctx, base);
	// stw r28,-20(r31)
	PPC_STORE_U32(ctx.r31.u32 + -20, ctx.r28.u32);
	// addi r11,r31,4
	ctx.r11.s64 = ctx.r31.s64 + 4;
	// stw r29,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r29.u32);
	// mr r9,r30
	ctx.r9.u64 = ctx.r30.u64;
	// li r10,16
	ctx.r10.s64 = 16;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_82A61130:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x82a61130
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_82A61130;
	// addi r27,r27,-1
	ctx.r27.s64 = ctx.r27.s64 + -1;
	// stw r30,68(r31)
	PPC_STORE_U32(ctx.r31.u32 + 68, ctx.r30.u32);
	// stw r30,72(r31)
	PPC_STORE_U32(ctx.r31.u32 + 72, ctx.r30.u32);
	// stw r30,76(r31)
	PPC_STORE_U32(ctx.r31.u32 + 76, ctx.r30.u32);
	// cmpwi cr6,r27,0
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 0, ctx.xer);
	// stb r30,80(r31)
	PPC_STORE_U8(ctx.r31.u32 + 80, ctx.r30.u8);
	// stw r30,84(r31)
	PPC_STORE_U32(ctx.r31.u32 + 84, ctx.r30.u32);
	// addi r31,r31,108
	ctx.r31.s64 = ctx.r31.s64 + 108;
	// bge cr6,0x82a61110
	if (!ctx.cr6.lt) goto loc_82A61110;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,2760
	ctx.r3.s64 = ctx.r11.s64 + 2760;
	// bl 0x829ffa48
	ctx.lr = 0x82A6116C;
	sub_829FFA48(ctx, base);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x829ff814
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61178);
PPC_WEAK_FUNC(sub_82A61178) { __imp__sub_82A61178(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61178) {
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
	// li r31,5
	ctx.r31.s64 = 5;
	// addi r30,r11,-24000
	ctx.r30.s64 = ctx.r11.s64 + -24000;
loc_82A61198:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82915508
	ctx.lr = 0x82A611A0;
	sub_82915508(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,80
	ctx.r30.s64 = ctx.r30.s64 + 80;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x82a61198
	if (!ctx.cr6.lt) goto loc_82A61198;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,2880
	ctx.r3.s64 = ctx.r11.s64 + 2880;
	// bl 0x829ffa48
	ctx.lr = 0x82A611BC;
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

PPC_FUNC_IMPL(__imp__sub_82A611D8);
PPC_WEAK_FUNC(sub_82A611D8) { __imp__sub_82A611D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A611D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,-23480
	ctx.r3.s64 = ctx.r11.s64 + -23480;
	// bl 0x8279a748
	ctx.lr = 0x82A611F0;
	sub_8279A748(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,2968
	ctx.r3.s64 = ctx.r11.s64 + 2968;
	// bl 0x829ffa48
	ctx.lr = 0x82A611FC;
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

PPC_FUNC_IMPL(__imp__sub_82A61210);
PPC_WEAK_FUNC(sub_82A61210) { __imp__sub_82A61210(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61210) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,-23440
	ctx.r3.s64 = ctx.r11.s64 + -23440;
	// bl 0x827987a0
	ctx.lr = 0x82A61228;
	sub_827987A0(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,2984
	ctx.r3.s64 = ctx.r11.s64 + 2984;
	// bl 0x829ffa48
	ctx.lr = 0x82A61234;
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

PPC_FUNC_IMPL(__imp__sub_82A61248);
PPC_WEAK_FUNC(sub_82A61248) { __imp__sub_82A61248(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61248) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3000
	ctx.r3.s64 = ctx.r11.s64 + 3000;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61258);
PPC_WEAK_FUNC(sub_82A61258) { __imp__sub_82A61258(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61258) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c8
	ctx.lr = 0x82A61260;
	__savegprlr_28(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// li r29,47
	ctx.r29.s64 = 47;
	// addi r11,r11,-528
	ctx.r11.s64 = ctx.r11.s64 + -528;
	// li r28,3
	ctx.r28.s64 = 3;
	// addi r31,r11,86
	ctx.r31.s64 = ctx.r11.s64 + 86;
	// li r30,0
	ctx.r30.s64 = 0;
loc_82A6127C:
	// addi r3,r31,-78
	ctx.r3.s64 = ctx.r31.s64 + -78;
	// bl 0x8244f280
	ctx.lr = 0x82A61284;
	sub_8244F280(ctx, base);
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
	// bl 0x826b60d8
	ctx.lr = 0x82A612B0;
	sub_826B60D8(ctx, base);
	// addi r29,r29,-1
	ctx.r29.s64 = ctx.r29.s64 + -1;
	// stw r3,-6(r31)
	PPC_STORE_U32(ctx.r31.u32 + -6, ctx.r3.u32);
	// addi r31,r31,92
	ctx.r31.s64 = ctx.r31.s64 + 92;
	// cmpwi cr6,r29,0
	ctx.cr6.compare<int32_t>(ctx.r29.s32, 0, ctx.xer);
	// bge cr6,0x82a6127c
	if (!ctx.cr6.lt) goto loc_82A6127C;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3032
	ctx.r3.s64 = ctx.r11.s64 + 3032;
	// bl 0x829ffa48
	ctx.lr = 0x82A612D0;
	sub_829FFA48(ctx, base);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x829ff818
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A612D8);
PPC_WEAK_FUNC(sub_82A612D8) { __imp__sub_82A612D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A612D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,7920
	ctx.r3.s64 = ctx.r11.s64 + 7920;
	// bl 0x82460b98
	ctx.lr = 0x82A612F0;
	sub_82460B98(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3136
	ctx.r3.s64 = ctx.r11.s64 + 3136;
	// bl 0x829ffa48
	ctx.lr = 0x82A612FC;
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

PPC_FUNC_IMPL(__imp__sub_82A61310);
PPC_WEAK_FUNC(sub_82A61310) { __imp__sub_82A61310(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61310) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17480
	ctx.r3.s64 = ctx.r11.s64 + -17480;
	// bl 0x8284e060
	ctx.lr = 0x82A6132C;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8608(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8608, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61348);
PPC_WEAK_FUNC(sub_82A61348) { __imp__sub_82A61348(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61348) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17460
	ctx.r3.s64 = ctx.r11.s64 + -17460;
	// bl 0x8284e060
	ctx.lr = 0x82A61364;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8612(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8612, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61380);
PPC_WEAK_FUNC(sub_82A61380) { __imp__sub_82A61380(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61380) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17440
	ctx.r3.s64 = ctx.r11.s64 + -17440;
	// bl 0x8284e060
	ctx.lr = 0x82A6139C;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8616(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8616, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A613B8);
PPC_WEAK_FUNC(sub_82A613B8) { __imp__sub_82A613B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A613B8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,19420
	ctx.r3.s64 = ctx.r11.s64 + 19420;
	// bl 0x8284e060
	ctx.lr = 0x82A613D4;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8620(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8620, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A613F0);
PPC_WEAK_FUNC(sub_82A613F0) { __imp__sub_82A613F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A613F0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,21380
	ctx.r3.s64 = ctx.r11.s64 + 21380;
	// bl 0x8284e060
	ctx.lr = 0x82A6140C;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8624(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8624, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61428);
PPC_WEAK_FUNC(sub_82A61428) { __imp__sub_82A61428(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61428) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,19408
	ctx.r3.s64 = ctx.r11.s64 + 19408;
	// bl 0x8284e060
	ctx.lr = 0x82A61444;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8628(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8628, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61460);
PPC_WEAK_FUNC(sub_82A61460) { __imp__sub_82A61460(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61460) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-20852
	ctx.r3.s64 = ctx.r11.s64 + -20852;
	// bl 0x8284e060
	ctx.lr = 0x82A6147C;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8632(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8632, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61498);
PPC_WEAK_FUNC(sub_82A61498) { __imp__sub_82A61498(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61498) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17532
	ctx.r3.s64 = ctx.r11.s64 + -17532;
	// bl 0x8284e060
	ctx.lr = 0x82A614B4;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8636(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8636, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A614D0);
PPC_WEAK_FUNC(sub_82A614D0) { __imp__sub_82A614D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A614D0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,2724
	ctx.r3.s64 = ctx.r11.s64 + 2724;
	// bl 0x8284e060
	ctx.lr = 0x82A614EC;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8640(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8640, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61508);
PPC_WEAK_FUNC(sub_82A61508) { __imp__sub_82A61508(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61508) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17428
	ctx.r3.s64 = ctx.r11.s64 + -17428;
	// bl 0x8284e060
	ctx.lr = 0x82A61524;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8644(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8644, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61540);
PPC_WEAK_FUNC(sub_82A61540) { __imp__sub_82A61540(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61540) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17544
	ctx.r3.s64 = ctx.r11.s64 + -17544;
	// bl 0x8284e060
	ctx.lr = 0x82A6155C;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8648(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8648, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61578);
PPC_WEAK_FUNC(sub_82A61578) { __imp__sub_82A61578(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61578) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24352
	ctx.r3.s64 = ctx.r11.s64 + -24352;
	// bl 0x8284e060
	ctx.lr = 0x82A61594;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8652(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8652, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A615B0);
PPC_WEAK_FUNC(sub_82A615B0) { __imp__sub_82A615B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A615B0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17416
	ctx.r3.s64 = ctx.r11.s64 + -17416;
	// bl 0x8284e060
	ctx.lr = 0x82A615CC;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8656(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8656, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A615E8);
PPC_WEAK_FUNC(sub_82A615E8) { __imp__sub_82A615E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A615E8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17408
	ctx.r3.s64 = ctx.r11.s64 + -17408;
	// bl 0x8284e060
	ctx.lr = 0x82A61604;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8660(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8660, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61620);
PPC_WEAK_FUNC(sub_82A61620) { __imp__sub_82A61620(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61620) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17396
	ctx.r3.s64 = ctx.r11.s64 + -17396;
	// bl 0x8284e060
	ctx.lr = 0x82A6163C;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8664(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8664, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61658);
PPC_WEAK_FUNC(sub_82A61658) { __imp__sub_82A61658(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61658) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17384
	ctx.r3.s64 = ctx.r11.s64 + -17384;
	// bl 0x8284e060
	ctx.lr = 0x82A61674;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8668(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8668, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61690);
PPC_WEAK_FUNC(sub_82A61690) { __imp__sub_82A61690(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61690) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17376
	ctx.r3.s64 = ctx.r11.s64 + -17376;
	// bl 0x8284e060
	ctx.lr = 0x82A616AC;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8672(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8672, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A616C8);
PPC_WEAK_FUNC(sub_82A616C8) { __imp__sub_82A616C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A616C8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17360
	ctx.r3.s64 = ctx.r11.s64 + -17360;
	// bl 0x8284e060
	ctx.lr = 0x82A616E4;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8676(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8676, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61700);
PPC_WEAK_FUNC(sub_82A61700) { __imp__sub_82A61700(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61700) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17344
	ctx.r3.s64 = ctx.r11.s64 + -17344;
	// bl 0x8284e060
	ctx.lr = 0x82A6171C;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8680(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8680, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61738);
PPC_WEAK_FUNC(sub_82A61738) { __imp__sub_82A61738(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61738) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17336
	ctx.r3.s64 = ctx.r11.s64 + -17336;
	// bl 0x8284e060
	ctx.lr = 0x82A61754;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8684(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8684, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61770);
PPC_WEAK_FUNC(sub_82A61770) { __imp__sub_82A61770(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61770) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17324
	ctx.r3.s64 = ctx.r11.s64 + -17324;
	// bl 0x8284e060
	ctx.lr = 0x82A6178C;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8688(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8688, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A617A8);
PPC_WEAK_FUNC(sub_82A617A8) { __imp__sub_82A617A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A617A8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17312
	ctx.r3.s64 = ctx.r11.s64 + -17312;
	// bl 0x8284e060
	ctx.lr = 0x82A617C4;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8692(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8692, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A617E0);
PPC_WEAK_FUNC(sub_82A617E0) { __imp__sub_82A617E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A617E0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17304
	ctx.r3.s64 = ctx.r11.s64 + -17304;
	// bl 0x8284e060
	ctx.lr = 0x82A617FC;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8696(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8696, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61818);
PPC_WEAK_FUNC(sub_82A61818) { __imp__sub_82A61818(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61818) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17288
	ctx.r3.s64 = ctx.r11.s64 + -17288;
	// bl 0x8284e060
	ctx.lr = 0x82A61834;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8700(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8700, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61850);
PPC_WEAK_FUNC(sub_82A61850) { __imp__sub_82A61850(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61850) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17276
	ctx.r3.s64 = ctx.r11.s64 + -17276;
	// bl 0x8284e060
	ctx.lr = 0x82A6186C;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8704(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8704, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61888);
PPC_WEAK_FUNC(sub_82A61888) { __imp__sub_82A61888(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61888) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17264
	ctx.r3.s64 = ctx.r11.s64 + -17264;
	// bl 0x8284e060
	ctx.lr = 0x82A618A4;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8708(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8708, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A618C0);
PPC_WEAK_FUNC(sub_82A618C0) { __imp__sub_82A618C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A618C0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17248
	ctx.r3.s64 = ctx.r11.s64 + -17248;
	// bl 0x8284e060
	ctx.lr = 0x82A618DC;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8712(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8712, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A618F8);
PPC_WEAK_FUNC(sub_82A618F8) { __imp__sub_82A618F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A618F8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17240
	ctx.r3.s64 = ctx.r11.s64 + -17240;
	// bl 0x8284e060
	ctx.lr = 0x82A61914;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8716(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8716, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61930);
PPC_WEAK_FUNC(sub_82A61930) { __imp__sub_82A61930(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61930) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17228
	ctx.r3.s64 = ctx.r11.s64 + -17228;
	// bl 0x8284e060
	ctx.lr = 0x82A6194C;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8720(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8720, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61968);
PPC_WEAK_FUNC(sub_82A61968) { __imp__sub_82A61968(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61968) {
	PPC_FUNC_PROLOGUE();
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
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-23408
	ctx.r3.s64 = ctx.r11.s64 + -23408;
	// bl 0x8284e060
	ctx.lr = 0x82A61984;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8724(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8724, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A619A0);
PPC_WEAK_FUNC(sub_82A619A0) { __imp__sub_82A619A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A619A0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17216
	ctx.r3.s64 = ctx.r11.s64 + -17216;
	// bl 0x8284e060
	ctx.lr = 0x82A619BC;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8728(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8728, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A619D8);
PPC_WEAK_FUNC(sub_82A619D8) { __imp__sub_82A619D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A619D8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17192
	ctx.r3.s64 = ctx.r11.s64 + -17192;
	// bl 0x8284e060
	ctx.lr = 0x82A619F4;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8732(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8732, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61A10);
PPC_WEAK_FUNC(sub_82A61A10) { __imp__sub_82A61A10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61A10) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17164
	ctx.r3.s64 = ctx.r11.s64 + -17164;
	// bl 0x8284e060
	ctx.lr = 0x82A61A2C;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8736(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8736, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61A48);
PPC_WEAK_FUNC(sub_82A61A48) { __imp__sub_82A61A48(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61A48) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17136
	ctx.r3.s64 = ctx.r11.s64 + -17136;
	// bl 0x8284e060
	ctx.lr = 0x82A61A64;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8740(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8740, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61A80);
PPC_WEAK_FUNC(sub_82A61A80) { __imp__sub_82A61A80(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61A80) {
	PPC_FUNC_PROLOGUE();
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
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-7400
	ctx.r3.s64 = ctx.r11.s64 + -7400;
	// bl 0x8284e060
	ctx.lr = 0x82A61A9C;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8744(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8744, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61AB8);
PPC_WEAK_FUNC(sub_82A61AB8) { __imp__sub_82A61AB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61AB8) {
	PPC_FUNC_PROLOGUE();
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
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-7500
	ctx.r3.s64 = ctx.r11.s64 + -7500;
	// bl 0x8284e060
	ctx.lr = 0x82A61AD4;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8748(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8748, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61AF0);
PPC_WEAK_FUNC(sub_82A61AF0) { __imp__sub_82A61AF0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61AF0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-4804
	ctx.r3.s64 = ctx.r11.s64 + -4804;
	// bl 0x8284e060
	ctx.lr = 0x82A61B0C;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8752(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8752, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61B28);
PPC_WEAK_FUNC(sub_82A61B28) { __imp__sub_82A61B28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61B28) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-4776
	ctx.r3.s64 = ctx.r11.s64 + -4776;
	// bl 0x8284e060
	ctx.lr = 0x82A61B44;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8756(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8756, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61B60);
PPC_WEAK_FUNC(sub_82A61B60) { __imp__sub_82A61B60(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61B60) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17560
	ctx.r3.s64 = ctx.r11.s64 + -17560;
	// bl 0x8284e060
	ctx.lr = 0x82A61B7C;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8760(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8760, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61B98);
PPC_WEAK_FUNC(sub_82A61B98) { __imp__sub_82A61B98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61B98) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17108
	ctx.r3.s64 = ctx.r11.s64 + -17108;
	// bl 0x8284e060
	ctx.lr = 0x82A61BB4;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8764(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8764, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61BD0);
PPC_WEAK_FUNC(sub_82A61BD0) { __imp__sub_82A61BD0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61BD0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17524
	ctx.r3.s64 = ctx.r11.s64 + -17524;
	// bl 0x8284e060
	ctx.lr = 0x82A61BEC;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8768(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8768, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61C08);
PPC_WEAK_FUNC(sub_82A61C08) { __imp__sub_82A61C08(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61C08) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-27276
	ctx.r3.s64 = ctx.r11.s64 + -27276;
	// bl 0x8284e060
	ctx.lr = 0x82A61C24;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8772(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8772, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61C40);
PPC_WEAK_FUNC(sub_82A61C40) { __imp__sub_82A61C40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61C40) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17088
	ctx.r3.s64 = ctx.r11.s64 + -17088;
	// bl 0x8284e060
	ctx.lr = 0x82A61C5C;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8776(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8776, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61C78);
PPC_WEAK_FUNC(sub_82A61C78) { __imp__sub_82A61C78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61C78) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-27428
	ctx.r3.s64 = ctx.r11.s64 + -27428;
	// bl 0x8284e060
	ctx.lr = 0x82A61C94;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8780(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8780, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61CB0);
PPC_WEAK_FUNC(sub_82A61CB0) { __imp__sub_82A61CB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61CB0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-27464
	ctx.r3.s64 = ctx.r11.s64 + -27464;
	// bl 0x8284e060
	ctx.lr = 0x82A61CCC;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8784(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8784, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61CE8);
PPC_WEAK_FUNC(sub_82A61CE8) { __imp__sub_82A61CE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61CE8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17572
	ctx.r3.s64 = ctx.r11.s64 + -17572;
	// bl 0x8284e060
	ctx.lr = 0x82A61D04;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8788(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8788, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61D20);
PPC_WEAK_FUNC(sub_82A61D20) { __imp__sub_82A61D20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61D20) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17588
	ctx.r3.s64 = ctx.r11.s64 + -17588;
	// bl 0x8284e060
	ctx.lr = 0x82A61D3C;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8792(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8792, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61D58);
PPC_WEAK_FUNC(sub_82A61D58) { __imp__sub_82A61D58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61D58) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-27536
	ctx.r3.s64 = ctx.r11.s64 + -27536;
	// bl 0x8284e060
	ctx.lr = 0x82A61D74;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8796(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8796, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61D90);
PPC_WEAK_FUNC(sub_82A61D90) { __imp__sub_82A61D90(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61D90) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17596
	ctx.r3.s64 = ctx.r11.s64 + -17596;
	// bl 0x8284e060
	ctx.lr = 0x82A61DAC;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8800(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8800, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61DC8);
PPC_WEAK_FUNC(sub_82A61DC8) { __imp__sub_82A61DC8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61DC8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17612
	ctx.r3.s64 = ctx.r11.s64 + -17612;
	// bl 0x8284e060
	ctx.lr = 0x82A61DE4;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8804(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8804, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61E00);
PPC_WEAK_FUNC(sub_82A61E00) { __imp__sub_82A61E00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61E00) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17068
	ctx.r3.s64 = ctx.r11.s64 + -17068;
	// bl 0x8284e060
	ctx.lr = 0x82A61E1C;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8808(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8808, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61E38);
PPC_WEAK_FUNC(sub_82A61E38) { __imp__sub_82A61E38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61E38) {
	PPC_FUNC_PROLOGUE();
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
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-7536
	ctx.r3.s64 = ctx.r11.s64 + -7536;
	// bl 0x8284e060
	ctx.lr = 0x82A61E54;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8812(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8812, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61E70);
PPC_WEAK_FUNC(sub_82A61E70) { __imp__sub_82A61E70(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61E70) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17060
	ctx.r3.s64 = ctx.r11.s64 + -17060;
	// bl 0x8284e060
	ctx.lr = 0x82A61E8C;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8816(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8816, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61EA8);
PPC_WEAK_FUNC(sub_82A61EA8) { __imp__sub_82A61EA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61EA8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17048
	ctx.r3.s64 = ctx.r11.s64 + -17048;
	// bl 0x8284e060
	ctx.lr = 0x82A61EC4;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8820(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8820, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61EE0);
PPC_WEAK_FUNC(sub_82A61EE0) { __imp__sub_82A61EE0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61EE0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17036
	ctx.r3.s64 = ctx.r11.s64 + -17036;
	// bl 0x8284e060
	ctx.lr = 0x82A61EFC;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8824(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8824, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61F18);
PPC_WEAK_FUNC(sub_82A61F18) { __imp__sub_82A61F18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61F18) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-17016
	ctx.r3.s64 = ctx.r11.s64 + -17016;
	// bl 0x8284e060
	ctx.lr = 0x82A61F34;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8828(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8828, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61F50);
PPC_WEAK_FUNC(sub_82A61F50) { __imp__sub_82A61F50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61F50) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-18452
	ctx.r3.s64 = ctx.r11.s64 + -18452;
	// bl 0x8284e060
	ctx.lr = 0x82A61F6C;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8832(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8832, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61F88);
PPC_WEAK_FUNC(sub_82A61F88) { __imp__sub_82A61F88(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61F88) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-18472
	ctx.r3.s64 = ctx.r11.s64 + -18472;
	// bl 0x8284e060
	ctx.lr = 0x82A61FA4;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8836(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8836, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61FC0);
PPC_WEAK_FUNC(sub_82A61FC0) { __imp__sub_82A61FC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61FC0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-18412
	ctx.r3.s64 = ctx.r11.s64 + -18412;
	// bl 0x8284e060
	ctx.lr = 0x82A61FDC;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8840(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8840, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A61FF8);
PPC_WEAK_FUNC(sub_82A61FF8) { __imp__sub_82A61FF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A61FF8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-18440
	ctx.r3.s64 = ctx.r11.s64 + -18440;
	// bl 0x8284e060
	ctx.lr = 0x82A62014;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8844(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8844, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A62030);
PPC_WEAK_FUNC(sub_82A62030) { __imp__sub_82A62030(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62030) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,4300
	ctx.r3.s64 = ctx.r11.s64 + 4300;
	// bl 0x8284e060
	ctx.lr = 0x82A6204C;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8848(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8848, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A62068);
PPC_WEAK_FUNC(sub_82A62068) { __imp__sub_82A62068(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62068) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,2420
	ctx.r3.s64 = ctx.r11.s64 + 2420;
	// bl 0x8284e060
	ctx.lr = 0x82A62084;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,8852(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8852, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A620A0);
PPC_WEAK_FUNC(sub_82A620A0) { __imp__sub_82A620A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A620A0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,8856
	ctx.r3.s64 = ctx.r11.s64 + 8856;
	// bl 0x8290c330
	ctx.lr = 0x82A620B8;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3152
	ctx.r3.s64 = ctx.r11.s64 + 3152;
	// bl 0x829ffa48
	ctx.lr = 0x82A620C4;
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

PPC_FUNC_IMPL(__imp__sub_82A620D8);
PPC_WEAK_FUNC(sub_82A620D8) { __imp__sub_82A620D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A620D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,8896
	ctx.r3.s64 = ctx.r11.s64 + 8896;
	// bl 0x8290c330
	ctx.lr = 0x82A620F0;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3160
	ctx.r3.s64 = ctx.r11.s64 + 3160;
	// bl 0x829ffa48
	ctx.lr = 0x82A620FC;
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

PPC_FUNC_IMPL(__imp__sub_82A62110);
PPC_WEAK_FUNC(sub_82A62110) { __imp__sub_82A62110(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62110) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,8936
	ctx.r3.s64 = ctx.r11.s64 + 8936;
	// bl 0x8290c330
	ctx.lr = 0x82A62128;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3168
	ctx.r3.s64 = ctx.r11.s64 + 3168;
	// bl 0x829ffa48
	ctx.lr = 0x82A62134;
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

PPC_FUNC_IMPL(__imp__sub_82A62148);
PPC_WEAK_FUNC(sub_82A62148) { __imp__sub_82A62148(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62148) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,8976
	ctx.r3.s64 = ctx.r11.s64 + 8976;
	// bl 0x8290c330
	ctx.lr = 0x82A62160;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3176
	ctx.r3.s64 = ctx.r11.s64 + 3176;
	// bl 0x829ffa48
	ctx.lr = 0x82A6216C;
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

PPC_FUNC_IMPL(__imp__sub_82A62180);
PPC_WEAK_FUNC(sub_82A62180) { __imp__sub_82A62180(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62180) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-16988
	ctx.r3.s64 = ctx.r11.s64 + -16988;
	// bl 0x8284e060
	ctx.lr = 0x82A6219C;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,9016(r11)
	PPC_STORE_U32(ctx.r11.u32 + 9016, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A621B8);
PPC_WEAK_FUNC(sub_82A621B8) { __imp__sub_82A621B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A621B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-16228
	ctx.r5.s64 = ctx.r11.s64 + -16228;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,21372
	ctx.r3.s64 = ctx.r11.s64 + 21372;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A621D8);
PPC_WEAK_FUNC(sub_82A621D8) { __imp__sub_82A621D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A621D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-16212
	ctx.r5.s64 = ctx.r11.s64 + -16212;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,21504
	ctx.r3.s64 = ctx.r11.s64 + 21504;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A621F8);
PPC_WEAK_FUNC(sub_82A621F8) { __imp__sub_82A621F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A621F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// li r9,11
	ctx.r9.s64 = 11;
	// addi r11,r11,21536
	ctx.r11.s64 = ctx.r11.s64 + 21536;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
loc_82A6220C:
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
	// bge cr6,0x82a6220c
	if (!ctx.cr6.lt) goto loc_82A6220C;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A62238);
PPC_WEAK_FUNC(sub_82A62238) { __imp__sub_82A62238(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62238) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,3256
	ctx.r11.s64 = ctx.r11.s64 + 3256;
	// lfs f0,12(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stfs f0,21904(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 21904, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A62250);
PPC_WEAK_FUNC(sub_82A62250) { __imp__sub_82A62250(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62250) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3248
	ctx.r3.s64 = ctx.r11.s64 + 3248;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A62260);
PPC_WEAK_FUNC(sub_82A62260) { __imp__sub_82A62260(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62260) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3280
	ctx.r3.s64 = ctx.r11.s64 + 3280;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A62270);
PPC_WEAK_FUNC(sub_82A62270) { __imp__sub_82A62270(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62270) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,25792
	ctx.r3.s64 = ctx.r11.s64 + 25792;
	// bl 0x8290c330
	ctx.lr = 0x82A62288;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3312
	ctx.r3.s64 = ctx.r11.s64 + 3312;
	// bl 0x829ffa48
	ctx.lr = 0x82A62294;
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

PPC_FUNC_IMPL(__imp__sub_82A622A8);
PPC_WEAK_FUNC(sub_82A622A8) { __imp__sub_82A622A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A622A8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,25832
	ctx.r3.s64 = ctx.r11.s64 + 25832;
	// bl 0x8290c330
	ctx.lr = 0x82A622C0;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3320
	ctx.r3.s64 = ctx.r11.s64 + 3320;
	// bl 0x829ffa48
	ctx.lr = 0x82A622CC;
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

PPC_FUNC_IMPL(__imp__sub_82A622E0);
PPC_WEAK_FUNC(sub_82A622E0) { __imp__sub_82A622E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A622E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,25872
	ctx.r3.s64 = ctx.r11.s64 + 25872;
	// bl 0x8290c330
	ctx.lr = 0x82A622F8;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3328
	ctx.r3.s64 = ctx.r11.s64 + 3328;
	// bl 0x829ffa48
	ctx.lr = 0x82A62304;
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

PPC_FUNC_IMPL(__imp__sub_82A62318);
PPC_WEAK_FUNC(sub_82A62318) { __imp__sub_82A62318(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62318) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,25912
	ctx.r3.s64 = ctx.r11.s64 + 25912;
	// bl 0x8290c330
	ctx.lr = 0x82A62330;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3336
	ctx.r3.s64 = ctx.r11.s64 + 3336;
	// bl 0x829ffa48
	ctx.lr = 0x82A6233C;
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

PPC_FUNC_IMPL(__imp__sub_82A62350);
PPC_WEAK_FUNC(sub_82A62350) { __imp__sub_82A62350(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62350) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,25952
	ctx.r3.s64 = ctx.r11.s64 + 25952;
	// bl 0x8290c330
	ctx.lr = 0x82A62368;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3344
	ctx.r3.s64 = ctx.r11.s64 + 3344;
	// bl 0x829ffa48
	ctx.lr = 0x82A62374;
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

PPC_FUNC_IMPL(__imp__sub_82A62388);
PPC_WEAK_FUNC(sub_82A62388) { __imp__sub_82A62388(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62388) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,25992
	ctx.r3.s64 = ctx.r11.s64 + 25992;
	// bl 0x8290c330
	ctx.lr = 0x82A623A0;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3352
	ctx.r3.s64 = ctx.r11.s64 + 3352;
	// bl 0x829ffa48
	ctx.lr = 0x82A623AC;
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

PPC_FUNC_IMPL(__imp__sub_82A623C0);
PPC_WEAK_FUNC(sub_82A623C0) { __imp__sub_82A623C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A623C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,26032
	ctx.r3.s64 = ctx.r11.s64 + 26032;
	// bl 0x8290c330
	ctx.lr = 0x82A623D8;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3360
	ctx.r3.s64 = ctx.r11.s64 + 3360;
	// bl 0x829ffa48
	ctx.lr = 0x82A623E4;
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

PPC_FUNC_IMPL(__imp__sub_82A623F8);
PPC_WEAK_FUNC(sub_82A623F8) { __imp__sub_82A623F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A623F8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,26072
	ctx.r3.s64 = ctx.r11.s64 + 26072;
	// bl 0x8290c330
	ctx.lr = 0x82A62410;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3368
	ctx.r3.s64 = ctx.r11.s64 + 3368;
	// bl 0x829ffa48
	ctx.lr = 0x82A6241C;
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

PPC_FUNC_IMPL(__imp__sub_82A62430);
PPC_WEAK_FUNC(sub_82A62430) { __imp__sub_82A62430(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62430) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,26112
	ctx.r3.s64 = ctx.r11.s64 + 26112;
	// bl 0x8290c330
	ctx.lr = 0x82A62448;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3376
	ctx.r3.s64 = ctx.r11.s64 + 3376;
	// bl 0x829ffa48
	ctx.lr = 0x82A62454;
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

PPC_FUNC_IMPL(__imp__sub_82A62468);
PPC_WEAK_FUNC(sub_82A62468) { __imp__sub_82A62468(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62468) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,26152
	ctx.r3.s64 = ctx.r11.s64 + 26152;
	// bl 0x8290c330
	ctx.lr = 0x82A62480;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3384
	ctx.r3.s64 = ctx.r11.s64 + 3384;
	// bl 0x829ffa48
	ctx.lr = 0x82A6248C;
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

PPC_FUNC_IMPL(__imp__sub_82A624A0);
PPC_WEAK_FUNC(sub_82A624A0) { __imp__sub_82A624A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A624A0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,26192
	ctx.r3.s64 = ctx.r11.s64 + 26192;
	// bl 0x8290c330
	ctx.lr = 0x82A624B8;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3392
	ctx.r3.s64 = ctx.r11.s64 + 3392;
	// bl 0x829ffa48
	ctx.lr = 0x82A624C4;
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

PPC_FUNC_IMPL(__imp__sub_82A624D8);
PPC_WEAK_FUNC(sub_82A624D8) { __imp__sub_82A624D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A624D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,26232
	ctx.r3.s64 = ctx.r11.s64 + 26232;
	// bl 0x8290c330
	ctx.lr = 0x82A624F0;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3400
	ctx.r3.s64 = ctx.r11.s64 + 3400;
	// bl 0x829ffa48
	ctx.lr = 0x82A624FC;
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

PPC_FUNC_IMPL(__imp__sub_82A62510);
PPC_WEAK_FUNC(sub_82A62510) { __imp__sub_82A62510(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62510) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,26272
	ctx.r3.s64 = ctx.r11.s64 + 26272;
	// bl 0x8290c330
	ctx.lr = 0x82A62528;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3408
	ctx.r3.s64 = ctx.r11.s64 + 3408;
	// bl 0x829ffa48
	ctx.lr = 0x82A62534;
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

PPC_FUNC_IMPL(__imp__sub_82A62548);
PPC_WEAK_FUNC(sub_82A62548) { __imp__sub_82A62548(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62548) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,26312
	ctx.r3.s64 = ctx.r11.s64 + 26312;
	// bl 0x8290c330
	ctx.lr = 0x82A62560;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3416
	ctx.r3.s64 = ctx.r11.s64 + 3416;
	// bl 0x829ffa48
	ctx.lr = 0x82A6256C;
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

PPC_FUNC_IMPL(__imp__sub_82A62580);
PPC_WEAK_FUNC(sub_82A62580) { __imp__sub_82A62580(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62580) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,26352
	ctx.r3.s64 = ctx.r11.s64 + 26352;
	// bl 0x826bdd68
	ctx.lr = 0x82A62598;
	sub_826BDD68(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3424
	ctx.r3.s64 = ctx.r11.s64 + 3424;
	// bl 0x829ffa48
	ctx.lr = 0x82A625A4;
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

PPC_FUNC_IMPL(__imp__sub_82A625B8);
PPC_WEAK_FUNC(sub_82A625B8) { __imp__sub_82A625B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A625B8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,2420
	ctx.r3.s64 = ctx.r11.s64 + 2420;
	// bl 0x8284e060
	ctx.lr = 0x82A625D4;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,26368(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26368, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A625F0);
PPC_WEAK_FUNC(sub_82A625F0) { __imp__sub_82A625F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A625F0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,2432
	ctx.r3.s64 = ctx.r11.s64 + 2432;
	// bl 0x8284e060
	ctx.lr = 0x82A6260C;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,26372(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26372, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A62628);
PPC_WEAK_FUNC(sub_82A62628) { __imp__sub_82A62628(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62628) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,2448
	ctx.r3.s64 = ctx.r11.s64 + 2448;
	// bl 0x8284e060
	ctx.lr = 0x82A62644;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,26376(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26376, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A62660);
PPC_WEAK_FUNC(sub_82A62660) { __imp__sub_82A62660(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62660) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,2460
	ctx.r3.s64 = ctx.r11.s64 + 2460;
	// bl 0x8284e060
	ctx.lr = 0x82A6267C;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,26380(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26380, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A62698);
PPC_WEAK_FUNC(sub_82A62698) { __imp__sub_82A62698(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62698) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,1684
	ctx.r3.s64 = ctx.r11.s64 + 1684;
	// bl 0x8284e060
	ctx.lr = 0x82A626B4;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,26384(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26384, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A626D0);
PPC_WEAK_FUNC(sub_82A626D0) { __imp__sub_82A626D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A626D0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,1656
	ctx.r3.s64 = ctx.r11.s64 + 1656;
	// bl 0x8284e060
	ctx.lr = 0x82A626EC;
	sub_8284E060(ctx, base);
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// stw r3,26388(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26388, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A62708);
PPC_WEAK_FUNC(sub_82A62708) { __imp__sub_82A62708(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62708) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// addi r4,r11,2488
	ctx.r4.s64 = ctx.r11.s64 + 2488;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,26392
	ctx.r3.s64 = ctx.r11.s64 + 26392;
	// b 0x822d24c0
	sub_822D24C0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A62720);
PPC_WEAK_FUNC(sub_82A62720) { __imp__sub_82A62720(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62720) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// addi r4,r11,2500
	ctx.r4.s64 = ctx.r11.s64 + 2500;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,26404
	ctx.r3.s64 = ctx.r11.s64 + 26404;
	// b 0x822d24c0
	sub_822D24C0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A62738);
PPC_WEAK_FUNC(sub_82A62738) { __imp__sub_82A62738(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62738) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7cc
	ctx.lr = 0x82A62740;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r29,r11,2580
	ctx.r29.s64 = ctx.r11.s64 + 2580;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62758;
	sub_8284E060(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r30,r11,2560
	ctx.r30.s64 = ctx.r11.s64 + 2560;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r31,r11,26416
	ctx.r31.s64 = ctx.r11.s64 + 26416;
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62778;
	sub_8284E060(ctx, base);
	// stw r3,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62788;
	sub_8284E060(ctx, base);
	// stw r3,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62798;
	sub_8284E060(ctx, base);
	// stw r3,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A627A8;
	sub_8284E060(ctx, base);
	// stw r3,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A627B8;
	sub_8284E060(ctx, base);
	// stw r3,20(r31)
	PPC_STORE_U32(ctx.r31.u32 + 20, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A627C8;
	sub_8284E060(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,24(r31)
	PPC_STORE_U32(ctx.r31.u32 + 24, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,2540
	ctx.r11.s64 = ctx.r11.s64 + 2540;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A627E0;
	sub_8284E060(ctx, base);
	// stw r3,28(r31)
	PPC_STORE_U32(ctx.r31.u32 + 28, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A627F0;
	sub_8284E060(ctx, base);
	// stw r3,32(r31)
	PPC_STORE_U32(ctx.r31.u32 + 32, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62800;
	sub_8284E060(ctx, base);
	// stw r3,36(r31)
	PPC_STORE_U32(ctx.r31.u32 + 36, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62810;
	sub_8284E060(ctx, base);
	// stw r3,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62820;
	sub_8284E060(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,44(r31)
	PPC_STORE_U32(ctx.r31.u32 + 44, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,2520
	ctx.r11.s64 = ctx.r11.s64 + 2520;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62838;
	sub_8284E060(ctx, base);
	// stw r3,48(r31)
	PPC_STORE_U32(ctx.r31.u32 + 48, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62848;
	sub_8284E060(ctx, base);
	// stw r3,52(r31)
	PPC_STORE_U32(ctx.r31.u32 + 52, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62858;
	sub_8284E060(ctx, base);
	// stw r3,56(r31)
	PPC_STORE_U32(ctx.r31.u32 + 56, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62868;
	sub_8284E060(ctx, base);
	// stw r3,60(r31)
	PPC_STORE_U32(ctx.r31.u32 + 60, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62878;
	sub_8284E060(ctx, base);
	// stw r3,64(r31)
	PPC_STORE_U32(ctx.r31.u32 + 64, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62888;
	sub_8284E060(ctx, base);
	// stw r3,68(r31)
	PPC_STORE_U32(ctx.r31.u32 + 68, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62898;
	sub_8284E060(ctx, base);
	// stw r3,72(r31)
	PPC_STORE_U32(ctx.r31.u32 + 72, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A628A8;
	sub_8284E060(ctx, base);
	// stw r3,76(r31)
	PPC_STORE_U32(ctx.r31.u32 + 76, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A628B8;
	sub_8284E060(ctx, base);
	// stw r3,80(r31)
	PPC_STORE_U32(ctx.r31.u32 + 80, ctx.r3.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A628C8);
PPC_WEAK_FUNC(sub_82A628C8) { __imp__sub_82A628C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A628C8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c8
	ctx.lr = 0x82A628D0;
	__savegprlr_28(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r29,r11,2640
	ctx.r29.s64 = ctx.r11.s64 + 2640;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A628E8;
	sub_8284E060(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r30,r11,2616
	ctx.r30.s64 = ctx.r11.s64 + 2616;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r31,r11,26504
	ctx.r31.s64 = ctx.r11.s64 + 26504;
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62908;
	sub_8284E060(ctx, base);
	// stw r3,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62918;
	sub_8284E060(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r28,r11,2580
	ctx.r28.s64 = ctx.r11.s64 + 2580;
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62930;
	sub_8284E060(ctx, base);
	// stw r3,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62940;
	sub_8284E060(ctx, base);
	// stw r3,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62950;
	sub_8284E060(ctx, base);
	// stw r3,20(r31)
	PPC_STORE_U32(ctx.r31.u32 + 20, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62960;
	sub_8284E060(ctx, base);
	// stw r3,24(r31)
	PPC_STORE_U32(ctx.r31.u32 + 24, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62970;
	sub_8284E060(ctx, base);
	// stw r3,28(r31)
	PPC_STORE_U32(ctx.r31.u32 + 28, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62980;
	sub_8284E060(ctx, base);
	// stw r3,32(r31)
	PPC_STORE_U32(ctx.r31.u32 + 32, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62990;
	sub_8284E060(ctx, base);
	// stw r3,36(r31)
	PPC_STORE_U32(ctx.r31.u32 + 36, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A629A0;
	sub_8284E060(ctx, base);
	// stw r3,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A629B0;
	sub_8284E060(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,44(r31)
	PPC_STORE_U32(ctx.r31.u32 + 44, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,2592
	ctx.r11.s64 = ctx.r11.s64 + 2592;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A629C8;
	sub_8284E060(ctx, base);
	// stw r3,48(r31)
	PPC_STORE_U32(ctx.r31.u32 + 48, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A629D8;
	sub_8284E060(ctx, base);
	// stw r3,52(r31)
	PPC_STORE_U32(ctx.r31.u32 + 52, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A629E8;
	sub_8284E060(ctx, base);
	// stw r3,56(r31)
	PPC_STORE_U32(ctx.r31.u32 + 56, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A629F8;
	sub_8284E060(ctx, base);
	// stw r3,60(r31)
	PPC_STORE_U32(ctx.r31.u32 + 60, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62A08;
	sub_8284E060(ctx, base);
	// stw r3,64(r31)
	PPC_STORE_U32(ctx.r31.u32 + 64, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62A18;
	sub_8284E060(ctx, base);
	// stw r3,68(r31)
	PPC_STORE_U32(ctx.r31.u32 + 68, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62A28;
	sub_8284E060(ctx, base);
	// stw r3,72(r31)
	PPC_STORE_U32(ctx.r31.u32 + 72, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62A38;
	sub_8284E060(ctx, base);
	// stw r3,76(r31)
	PPC_STORE_U32(ctx.r31.u32 + 76, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62A48;
	sub_8284E060(ctx, base);
	// stw r3,80(r31)
	PPC_STORE_U32(ctx.r31.u32 + 80, ctx.r3.u32);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x829ff818
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A62A58);
PPC_WEAK_FUNC(sub_82A62A58) { __imp__sub_82A62A58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62A58) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r3,r11,3764
	ctx.r3.s64 = ctx.r11.s64 + 3764;
	// bl 0x8284e060
	ctx.lr = 0x82A62A78;
	sub_8284E060(ctx, base);
	// lis r10,-32001
	ctx.r10.s64 = -2097217536;
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r31,r10,26776
	ctx.r31.s64 = ctx.r10.s64 + 26776;
	// addi r11,r11,18580
	ctx.r11.s64 = ctx.r11.s64 + 18580;
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62A98;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18500
	ctx.r11.s64 = ctx.r11.s64 + 18500;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62AB0;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18556
	ctx.r11.s64 = ctx.r11.s64 + 18556;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62AC8;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18388
	ctx.r11.s64 = ctx.r11.s64 + 18388;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62AE0;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18660
	ctx.r11.s64 = ctx.r11.s64 + 18660;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62AF8;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,20(r31)
	PPC_STORE_U32(ctx.r31.u32 + 20, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18520
	ctx.r11.s64 = ctx.r11.s64 + 18520;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62B10;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,24(r31)
	PPC_STORE_U32(ctx.r31.u32 + 24, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18336
	ctx.r11.s64 = ctx.r11.s64 + 18336;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62B28;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,28(r31)
	PPC_STORE_U32(ctx.r31.u32 + 28, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18112
	ctx.r11.s64 = ctx.r11.s64 + 18112;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62B40;
	sub_8284E060(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,32(r31)
	PPC_STORE_U32(ctx.r31.u32 + 32, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,3740
	ctx.r11.s64 = ctx.r11.s64 + 3740;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62B58;
	sub_8284E060(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,36(r31)
	PPC_STORE_U32(ctx.r31.u32 + 36, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,3712
	ctx.r11.s64 = ctx.r11.s64 + 3712;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62B70;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18628
	ctx.r11.s64 = ctx.r11.s64 + 18628;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62B88;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,44(r31)
	PPC_STORE_U32(ctx.r31.u32 + 44, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18644
	ctx.r11.s64 = ctx.r11.s64 + 18644;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62BA0;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,48(r31)
	PPC_STORE_U32(ctx.r31.u32 + 48, ctx.r3.u32);
	// addi r11,r11,18404
	ctx.r11.s64 = ctx.r11.s64 + 18404;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62BB8;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,52(r31)
	PPC_STORE_U32(ctx.r31.u32 + 52, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18444
	ctx.r11.s64 = ctx.r11.s64 + 18444;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62BD0;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,56(r31)
	PPC_STORE_U32(ctx.r31.u32 + 56, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18460
	ctx.r11.s64 = ctx.r11.s64 + 18460;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62BE8;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,60(r31)
	PPC_STORE_U32(ctx.r31.u32 + 60, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,18480
	ctx.r3.s64 = ctx.r11.s64 + 18480;
	// bl 0x8284e060
	ctx.lr = 0x82A62BFC;
	sub_8284E060(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,64(r31)
	PPC_STORE_U32(ctx.r31.u32 + 64, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,3692
	ctx.r11.s64 = ctx.r11.s64 + 3692;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62C14;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,68(r31)
	PPC_STORE_U32(ctx.r31.u32 + 68, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18604
	ctx.r11.s64 = ctx.r11.s64 + 18604;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62C2C;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,72(r31)
	PPC_STORE_U32(ctx.r31.u32 + 72, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18736
	ctx.r11.s64 = ctx.r11.s64 + 18736;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62C44;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,76(r31)
	PPC_STORE_U32(ctx.r31.u32 + 76, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18760
	ctx.r11.s64 = ctx.r11.s64 + 18760;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62C5C;
	sub_8284E060(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,80(r31)
	PPC_STORE_U32(ctx.r31.u32 + 80, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,3672
	ctx.r11.s64 = ctx.r11.s64 + 3672;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62C74;
	sub_8284E060(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,84(r31)
	PPC_STORE_U32(ctx.r31.u32 + 84, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,3648
	ctx.r11.s64 = ctx.r11.s64 + 3648;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62C8C;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,88(r31)
	PPC_STORE_U32(ctx.r31.u32 + 88, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18356
	ctx.r11.s64 = ctx.r11.s64 + 18356;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62CA4;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,92(r31)
	PPC_STORE_U32(ctx.r31.u32 + 92, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18372
	ctx.r11.s64 = ctx.r11.s64 + 18372;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62CBC;
	sub_8284E060(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,96(r31)
	PPC_STORE_U32(ctx.r31.u32 + 96, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,3624
	ctx.r11.s64 = ctx.r11.s64 + 3624;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62CD4;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,100(r31)
	PPC_STORE_U32(ctx.r31.u32 + 100, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18708
	ctx.r11.s64 = ctx.r11.s64 + 18708;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62CEC;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r11,r11,18316
	ctx.r11.s64 = ctx.r11.s64 + 18316;
	// stw r3,104(r31)
	PPC_STORE_U32(ctx.r31.u32 + 104, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62D04;
	sub_8284E060(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stw r3,108(r31)
	PPC_STORE_U32(ctx.r31.u32 + 108, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,18420
	ctx.r11.s64 = ctx.r11.s64 + 18420;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62D1C;
	sub_8284E060(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,112(r31)
	PPC_STORE_U32(ctx.r31.u32 + 112, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,3600
	ctx.r11.s64 = ctx.r11.s64 + 3600;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62D34;
	sub_8284E060(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,116(r31)
	PPC_STORE_U32(ctx.r31.u32 + 116, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,3584
	ctx.r11.s64 = ctx.r11.s64 + 3584;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A62D4C;
	sub_8284E060(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A62D68);
PPC_WEAK_FUNC(sub_82A62D68) { __imp__sub_82A62D68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62D68) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r3,r11,26904
	ctx.r3.s64 = ctx.r11.s64 + 26904;
	// bl 0x8285fe48
	ctx.lr = 0x82A62D80;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3440
	ctx.r3.s64 = ctx.r11.s64 + 3440;
	// bl 0x829ffa48
	ctx.lr = 0x82A62D8C;
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

PPC_FUNC_IMPL(__imp__sub_82A62DA0);
PPC_WEAK_FUNC(sub_82A62DA0) { __imp__sub_82A62DA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62DA0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3456
	ctx.r3.s64 = ctx.r11.s64 + 3456;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A62DB0);
PPC_WEAK_FUNC(sub_82A62DB0) { __imp__sub_82A62DB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62DB0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// addi r31,r11,26992
	ctx.r31.s64 = ctx.r11.s64 + 26992;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82902b70
	ctx.lr = 0x82A62DD0;
	sub_82902B70(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// addi r3,r31,28476
	ctx.r3.s64 = ctx.r31.s64 + 28476;
	// addi r11,r11,4316
	ctx.r11.s64 = ctx.r11.s64 + 4316;
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
	// bl 0x8285fe48
	ctx.lr = 0x82A62E00;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3512
	ctx.r3.s64 = ctx.r11.s64 + 3512;
	// bl 0x829ffa48
	ctx.lr = 0x82A62E0C;
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

PPC_FUNC_IMPL(__imp__sub_82A62E20);
PPC_WEAK_FUNC(sub_82A62E20) { __imp__sub_82A62E20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62E20) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// addi r3,r11,-10032
	ctx.r3.s64 = ctx.r11.s64 + -10032;
	// bl 0x8290c330
	ctx.lr = 0x82A62E38;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3472
	ctx.r3.s64 = ctx.r11.s64 + 3472;
	// bl 0x829ffa48
	ctx.lr = 0x82A62E44;
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

PPC_FUNC_IMPL(__imp__sub_82A62E58);
PPC_WEAK_FUNC(sub_82A62E58) { __imp__sub_82A62E58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62E58) {
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
	// li r31,1
	ctx.r31.s64 = 1;
	// addi r30,r11,-9992
	ctx.r30.s64 = ctx.r11.s64 + -9992;
loc_82A62E78:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8290c330
	ctx.lr = 0x82A62E80;
	sub_8290C330(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,40
	ctx.r30.s64 = ctx.r30.s64 + 40;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x82a62e78
	if (!ctx.cr6.lt) goto loc_82A62E78;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3496
	ctx.r3.s64 = ctx.r11.s64 + 3496;
	// bl 0x829ffa48
	ctx.lr = 0x82A62E9C;
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

PPC_FUNC_IMPL(__imp__sub_82A62EB8);
PPC_WEAK_FUNC(sub_82A62EB8) { __imp__sub_82A62EB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62EB8) {
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
	// li r31,1
	ctx.r31.s64 = 1;
	// addi r30,r11,-9912
	ctx.r30.s64 = ctx.r11.s64 + -9912;
loc_82A62ED8:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8290c330
	ctx.lr = 0x82A62EE0;
	sub_8290C330(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,40
	ctx.r30.s64 = ctx.r30.s64 + 40;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x82a62ed8
	if (!ctx.cr6.lt) goto loc_82A62ED8;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3488
	ctx.r3.s64 = ctx.r11.s64 + 3488;
	// bl 0x829ffa48
	ctx.lr = 0x82A62EFC;
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

PPC_FUNC_IMPL(__imp__sub_82A62F18);
PPC_WEAK_FUNC(sub_82A62F18) { __imp__sub_82A62F18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62F18) {
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
	// li r31,1
	ctx.r31.s64 = 1;
	// addi r30,r11,-9832
	ctx.r30.s64 = ctx.r11.s64 + -9832;
loc_82A62F38:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8290c330
	ctx.lr = 0x82A62F40;
	sub_8290C330(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,40
	ctx.r30.s64 = ctx.r30.s64 + 40;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x82a62f38
	if (!ctx.cr6.lt) goto loc_82A62F38;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3480
	ctx.r3.s64 = ctx.r11.s64 + 3480;
	// bl 0x829ffa48
	ctx.lr = 0x82A62F5C;
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

PPC_FUNC_IMPL(__imp__sub_82A62F78);
PPC_WEAK_FUNC(sub_82A62F78) { __imp__sub_82A62F78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62F78) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// addi r3,r11,-9752
	ctx.r3.s64 = ctx.r11.s64 + -9752;
	// bl 0x8290c330
	ctx.lr = 0x82A62F90;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3504
	ctx.r3.s64 = ctx.r11.s64 + 3504;
	// bl 0x829ffa48
	ctx.lr = 0x82A62F9C;
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

PPC_FUNC_IMPL(__imp__sub_82A62FB0);
PPC_WEAK_FUNC(sub_82A62FB0) { __imp__sub_82A62FB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62FB0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32000
	ctx.r11.s64 = -2097152000;
	// addi r3,r11,-8272
	ctx.r3.s64 = ctx.r11.s64 + -8272;
	// bl 0x824f13d0
	ctx.lr = 0x82A62FC8;
	sub_824F13D0(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3528
	ctx.r3.s64 = ctx.r11.s64 + 3528;
	// bl 0x829ffa48
	ctx.lr = 0x82A62FD4;
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

PPC_FUNC_IMPL(__imp__sub_82A62FE8);
PPC_WEAK_FUNC(sub_82A62FE8) { __imp__sub_82A62FE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62FE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3544
	ctx.r3.s64 = ctx.r11.s64 + 3544;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A62FF8);
PPC_WEAK_FUNC(sub_82A62FF8) { __imp__sub_82A62FF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A62FF8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31999
	ctx.r11.s64 = -2097086464;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r11,r11,27616
	ctx.r11.s64 = ctx.r11.s64 + 27616;
	// li r10,330
	ctx.r10.s64 = 330;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_82A6300C:
	// std r9,0(r11)
	PPC_STORE_U64(ctx.r11.u32 + 0, ctx.r9.u64);
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// bdnz 0x82a6300c
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_82A6300C;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3576
	ctx.r3.s64 = ctx.r11.s64 + 3576;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63028);
PPC_WEAK_FUNC(sub_82A63028) { __imp__sub_82A63028(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63028) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31999
	ctx.r11.s64 = -2097086464;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r8,r11,30256
	ctx.r8.s64 = ctx.r11.s64 + 30256;
	// li r11,16
	ctx.r11.s64 = 16;
	// addi r10,r8,40
	ctx.r10.s64 = ctx.r8.s64 + 40;
loc_82A6303C:
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// addi r10,r10,44
	ctx.r10.s64 = ctx.r10.s64 + 44;
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// bge cr6,0x82a6303c
	if (!ctx.cr6.lt) goto loc_82A6303C;
	// mr r11,r9
	ctx.r11.u64 = ctx.r9.u64;
	// stw r11,748(r8)
	PPC_STORE_U32(ctx.r8.u32 + 748, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63060);
PPC_WEAK_FUNC(sub_82A63060) { __imp__sub_82A63060(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63060) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3584
	ctx.r3.s64 = ctx.r11.s64 + 3584;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63070);
PPC_WEAK_FUNC(sub_82A63070) { __imp__sub_82A63070(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63070) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// li r9,-1
	ctx.r9.s64 = -1;
	// addi r11,r11,8312
	ctx.r11.s64 = ctx.r11.s64 + 8312;
	// li r10,196
	ctx.r10.s64 = 196;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_82A63084:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x82a63084
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_82A63084;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63098);
PPC_WEAK_FUNC(sub_82A63098) { __imp__sub_82A63098(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63098) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r3,r11,12948
	ctx.r3.s64 = ctx.r11.s64 + 12948;
	// bl 0x8284e060
	ctx.lr = 0x82A630B8;
	sub_8284E060(ctx, base);
	// lis r10,-31997
	ctx.r10.s64 = -2096955392;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// addi r31,r10,10032
	ctx.r31.s64 = ctx.r10.s64 + 10032;
	// addi r11,r11,12940
	ctx.r11.s64 = ctx.r11.s64 + 12940;
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A630D8;
	sub_8284E060(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,12932
	ctx.r11.s64 = ctx.r11.s64 + 12932;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A630F0;
	sub_8284E060(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,12928
	ctx.r11.s64 = ctx.r11.s64 + 12928;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A63108;
	sub_8284E060(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,12920
	ctx.r11.s64 = ctx.r11.s64 + 12920;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A63120;
	sub_8284E060(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A63138);
PPC_WEAK_FUNC(sub_82A63138) { __imp__sub_82A63138(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63138) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,9956
	ctx.r3.s64 = ctx.r11.s64 + 9956;
	// bl 0x8290c330
	ctx.lr = 0x82A63150;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3616
	ctx.r3.s64 = ctx.r11.s64 + 3616;
	// bl 0x829ffa48
	ctx.lr = 0x82A6315C;
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

PPC_FUNC_IMPL(__imp__sub_82A63170);
PPC_WEAK_FUNC(sub_82A63170) { __imp__sub_82A63170(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63170) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,9996
	ctx.r3.s64 = ctx.r11.s64 + 9996;
	// bl 0x82915100
	ctx.lr = 0x82A63188;
	sub_82915100(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3624
	ctx.r3.s64 = ctx.r11.s64 + 3624;
	// bl 0x829ffa48
	ctx.lr = 0x82A63194;
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

PPC_FUNC_IMPL(__imp__sub_82A631A8);
PPC_WEAK_FUNC(sub_82A631A8) { __imp__sub_82A631A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A631A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3632
	ctx.r3.s64 = ctx.r11.s64 + 3632;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A631B8);
PPC_WEAK_FUNC(sub_82A631B8) { __imp__sub_82A631B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A631B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3664
	ctx.r3.s64 = ctx.r11.s64 + 3664;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A631C8);
PPC_WEAK_FUNC(sub_82A631C8) { __imp__sub_82A631C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A631C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3680
	ctx.r3.s64 = ctx.r11.s64 + 3680;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A631D8);
PPC_WEAK_FUNC(sub_82A631D8) { __imp__sub_82A631D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A631D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3696
	ctx.r3.s64 = ctx.r11.s64 + 3696;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A631E8);
PPC_WEAK_FUNC(sub_82A631E8) { __imp__sub_82A631E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A631E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3728
	ctx.r3.s64 = ctx.r11.s64 + 3728;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A631F8);
PPC_WEAK_FUNC(sub_82A631F8) { __imp__sub_82A631F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A631F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3760
	ctx.r3.s64 = ctx.r11.s64 + 3760;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63208);
PPC_WEAK_FUNC(sub_82A63208) { __imp__sub_82A63208(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63208) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3792
	ctx.r3.s64 = ctx.r11.s64 + 3792;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63218);
PPC_WEAK_FUNC(sub_82A63218) { __imp__sub_82A63218(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63218) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3824
	ctx.r3.s64 = ctx.r11.s64 + 3824;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63228);
PPC_WEAK_FUNC(sub_82A63228) { __imp__sub_82A63228(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63228) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3840
	ctx.r3.s64 = ctx.r11.s64 + 3840;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63238);
PPC_WEAK_FUNC(sub_82A63238) { __imp__sub_82A63238(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63238) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3856
	ctx.r3.s64 = ctx.r11.s64 + 3856;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63248);
PPC_WEAK_FUNC(sub_82A63248) { __imp__sub_82A63248(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63248) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3872
	ctx.r3.s64 = ctx.r11.s64 + 3872;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63258);
PPC_WEAK_FUNC(sub_82A63258) { __imp__sub_82A63258(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63258) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,10736
	ctx.r3.s64 = ctx.r11.s64 + 10736;
	// bl 0x82525be8
	ctx.lr = 0x82A63270;
	sub_82525BE8(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3888
	ctx.r3.s64 = ctx.r11.s64 + 3888;
	// bl 0x829ffa48
	ctx.lr = 0x82A6327C;
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

PPC_FUNC_IMPL(__imp__sub_82A63290);
PPC_WEAK_FUNC(sub_82A63290) { __imp__sub_82A63290(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63290) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// lis r7,-32256
	ctx.r7.s64 = -2113929216;
	// lis r8,-32256
	ctx.r8.s64 = -2113929216;
	// addi r11,r11,10976
	ctx.r11.s64 = ctx.r11.s64 + 10976;
	// li r9,116
	ctx.r9.s64 = 116;
	// addi r11,r11,24
	ctx.r11.s64 = ctx.r11.s64 + 24;
	// lfs f13,3400(r7)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + 3400);
	ctx.f13.f64 = double(temp.f32);
	// li r10,-1
	ctx.r10.s64 = -1;
	// lfs f0,2612(r8)
	temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// li r4,0
	ctx.r4.s64 = 0;
	// li r5,1
	ctx.r5.s64 = 1;
	// li r6,1000
	ctx.r6.s64 = 1000;
loc_82A632C0:
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
	// bge cr6,0x82a632c0
	if (!ctx.cr6.lt) goto loc_82A632C0;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3912
	ctx.r3.s64 = ctx.r11.s64 + 3912;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63308);
PPC_WEAK_FUNC(sub_82A63308) { __imp__sub_82A63308(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63308) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18816
	ctx.r11.s64 = ctx.r11.s64 + -18816;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// addi r11,r11,16592
	ctx.r11.s64 = ctx.r11.s64 + 16592;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63328);
PPC_WEAK_FUNC(sub_82A63328) { __imp__sub_82A63328(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63328) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,16812
	ctx.r5.s64 = ctx.r11.s64 + 16812;
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,16752
	ctx.r3.s64 = ctx.r11.s64 + 16752;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63348);
PPC_WEAK_FUNC(sub_82A63348) { __imp__sub_82A63348(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63348) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c4
	ctx.lr = 0x82A63350;
	__savegprlr_27(ctx, base);
	// stfd f31,-56(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -56, ctx.f31.u64);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31997
	ctx.r11.s64 = -2096955392;
	// li r30,39
	ctx.r30.s64 = 39;
	// addi r11,r11,16784
	ctx.r11.s64 = ctx.r11.s64 + 16784;
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
	// lfs f31,2612(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2612);
	ctx.f31.f64 = double(temp.f32);
loc_82A6337C:
	// lis r11,-32173
	ctx.r11.s64 = -2108489728;
	// li r5,10
	ctx.r5.s64 = 10;
	// addi r6,r11,-19896
	ctx.r6.s64 = ctx.r11.s64 + -19896;
	// li r4,48
	ctx.r4.s64 = 48;
	// addi r3,r31,-484
	ctx.r3.s64 = ctx.r31.s64 + -484;
	// bl 0x82140220
	ctx.lr = 0x82A63394;
	sub_82140220(ctx, base);
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
	// bge cr6,0x82a6337c
	if (!ctx.cr6.lt) goto loc_82A6337C;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3920
	ctx.r3.s64 = ctx.r11.s64 + 3920;
	// bl 0x829ffa48
	ctx.lr = 0x82A633C4;
	sub_829FFA48(ctx, base);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lfd f31,-56(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -56);
	// b 0x829ff814
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A633D0);
PPC_WEAK_FUNC(sub_82A633D0) { __imp__sub_82A633D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A633D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31996
	ctx.r11.s64 = -2096889856;
	// vspltisw v0,0
	simde_mm_store_si128((simde__m128i*)ctx.v0.u32, simde_mm_set1_epi32(int(0x0)));
	// li r9,78
	ctx.r9.s64 = 78;
	// addi r11,r11,-28272
	ctx.r11.s64 = ctx.r11.s64 + -28272;
	// addi r10,r11,16
	ctx.r10.s64 = ctx.r11.s64 + 16;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// mtctr r9
	ctx.ctr.u64 = ctx.r9.u64;
loc_82A633EC:
	// ld r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U64(ctx.r11.u32 + 0);
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// std r9,0(r10)
	PPC_STORE_U64(ctx.r10.u32 + 0, ctx.r9.u64);
	// addi r10,r10,8
	ctx.r10.s64 = ctx.r10.s64 + 8;
	// bdnz 0x82a633ec
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_82A633EC;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3928
	ctx.r3.s64 = ctx.r11.s64 + 3928;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63410);
PPC_WEAK_FUNC(sub_82A63410) { __imp__sub_82A63410(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63410) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r10,r11,3256
	ctx.r10.s64 = ctx.r11.s64 + 3256;
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// addi r11,r11,-21680
	ctx.r11.s64 = ctx.r11.s64 + -21680;
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
	// stfs f0,508(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 508, temp.u32);
	// stfs f13,512(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 512, temp.u32);
	// stfs f0,548(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 548, temp.u32);
	// stfs f13,552(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 552, temp.u32);
	// stfs f0,588(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 588, temp.u32);
	// stfs f13,592(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 592, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63460);
PPC_WEAK_FUNC(sub_82A63460) { __imp__sub_82A63460(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63460) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// lfs f0,5288(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 5288);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31996
	ctx.r11.s64 = -2096889856;
	// stfs f0,-27456(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -27456, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63478);
PPC_WEAK_FUNC(sub_82A63478) { __imp__sub_82A63478(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63478) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31996
	ctx.r11.s64 = -2096889856;
	// lfs f0,-27456(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -27456);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// addi r11,r11,-18976
	ctx.r11.s64 = ctx.r11.s64 + -18976;
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63490);
PPC_WEAK_FUNC(sub_82A63490) { __imp__sub_82A63490(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63490) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,19236
	ctx.r3.s64 = ctx.r11.s64 + 19236;
	// bl 0x8284e060
	ctx.lr = 0x82A634AC;
	sub_8284E060(ctx, base);
	// lis r11,-31996
	ctx.r11.s64 = -2096889856;
	// stw r3,-27452(r11)
	PPC_STORE_U32(ctx.r11.u32 + -27452, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A634C8);
PPC_WEAK_FUNC(sub_82A634C8) { __imp__sub_82A634C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A634C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,19272
	ctx.r5.s64 = ctx.r11.s64 + 19272;
	// lis r11,-31996
	ctx.r11.s64 = -2096889856;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-27448
	ctx.r3.s64 = ctx.r11.s64 + -27448;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A634E8);
PPC_WEAK_FUNC(sub_82A634E8) { __imp__sub_82A634E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A634E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,19292
	ctx.r5.s64 = ctx.r11.s64 + 19292;
	// lis r11,-31996
	ctx.r11.s64 = -2096889856;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-27428
	ctx.r3.s64 = ctx.r11.s64 + -27428;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63508);
PPC_WEAK_FUNC(sub_82A63508) { __imp__sub_82A63508(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63508) {
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
	// li r31,204
	ctx.r31.s64 = 204;
	// addi r30,r11,-27408
	ctx.r30.s64 = ctx.r11.s64 + -27408;
loc_82A63528:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x825353c0
	ctx.lr = 0x82A63530;
	sub_825353C0(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,640
	ctx.r30.s64 = ctx.r30.s64 + 640;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x82a63528
	if (!ctx.cr6.lt) goto loc_82A63528;
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

PPC_FUNC_IMPL(__imp__sub_82A63558);
PPC_WEAK_FUNC(sub_82A63558) { __imp__sub_82A63558(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63558) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// lis r10,-31994
	ctx.r10.s64 = -2096758784;
	// addi r11,r11,-23740
	ctx.r11.s64 = ctx.r11.s64 + -23740;
	// lwz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// stw r11,-27280(r10)
	PPC_STORE_U32(ctx.r10.u32 + -27280, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63570);
PPC_WEAK_FUNC(sub_82A63570) { __imp__sub_82A63570(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63570) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,20204
	ctx.r3.s64 = ctx.r11.s64 + 20204;
	// bl 0x8284e060
	ctx.lr = 0x82A6358C;
	sub_8284E060(ctx, base);
	// lis r11,-31994
	ctx.r11.s64 = -2096758784;
	// stw r3,-27240(r11)
	PPC_STORE_U32(ctx.r11.u32 + -27240, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A635A8);
PPC_WEAK_FUNC(sub_82A635A8) { __imp__sub_82A635A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A635A8) {
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
	// lis r11,-31994
	ctx.r11.s64 = -2096758784;
	// li r31,4
	ctx.r31.s64 = 4;
	// addi r30,r11,-27232
	ctx.r30.s64 = ctx.r11.s64 + -27232;
loc_82A635C8:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8236b400
	ctx.lr = 0x82A635D0;
	sub_8236B400(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,160
	ctx.r30.s64 = ctx.r30.s64 + 160;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x82a635c8
	if (!ctx.cr6.lt) goto loc_82A635C8;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3936
	ctx.r3.s64 = ctx.r11.s64 + 3936;
	// bl 0x829ffa48
	ctx.lr = 0x82A635EC;
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

PPC_FUNC_IMPL(__imp__sub_82A63608);
PPC_WEAK_FUNC(sub_82A63608) { __imp__sub_82A63608(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63608) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// lfs f0,5288(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 5288);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// addi r11,r11,-18736
	ctx.r11.s64 = ctx.r11.s64 + -18736;
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63620);
PPC_WEAK_FUNC(sub_82A63620) { __imp__sub_82A63620(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63620) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,3944
	ctx.r3.s64 = ctx.r11.s64 + 3944;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63630);
PPC_WEAK_FUNC(sub_82A63630) { __imp__sub_82A63630(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63630) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31994
	ctx.r11.s64 = -2096758784;
	// li r9,2623
	ctx.r9.s64 = 2623;
	// addi r11,r11,-14856
	ctx.r11.s64 = ctx.r11.s64 + -14856;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,20
	ctx.r11.s64 = ctx.r11.s64 + 20;
loc_82A63644:
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
	// bge cr6,0x82a63644
	if (!ctx.cr6.lt) goto loc_82A63644;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63670);
PPC_WEAK_FUNC(sub_82A63670) { __imp__sub_82A63670(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63670) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4000
	ctx.r3.s64 = ctx.r11.s64 + 4000;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63680);
PPC_WEAK_FUNC(sub_82A63680) { __imp__sub_82A63680(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63680) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// lwz r11,-18184(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -18184);
	// rlwinm r10,r11,1,0,30
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// add r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
	// lis r10,-31993
	ctx.r10.s64 = -2096693248;
	// rlwinm r11,r11,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// stw r11,14076(r10)
	PPC_STORE_U32(ctx.r10.u32 + 14076, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A636A0);
PPC_WEAK_FUNC(sub_82A636A0) { __imp__sub_82A636A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A636A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4032
	ctx.r3.s64 = ctx.r11.s64 + 4032;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A636B0);
PPC_WEAK_FUNC(sub_82A636B0) { __imp__sub_82A636B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A636B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4016
	ctx.r3.s64 = ctx.r11.s64 + 4016;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A636C0);
PPC_WEAK_FUNC(sub_82A636C0) { __imp__sub_82A636C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A636C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31993
	ctx.r11.s64 = -2096693248;
	// li r10,15
	ctx.r10.s64 = 15;
	// addi r11,r11,14608
	ctx.r11.s64 = ctx.r11.s64 + 14608;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r11,r11,33
	ctx.r11.s64 = ctx.r11.s64 + 33;
loc_82A636D4:
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
	// bge cr6,0x82a636d4
	if (!ctx.cr6.lt) goto loc_82A636D4;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4024
	ctx.r3.s64 = ctx.r11.s64 + 4024;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63700);
PPC_WEAK_FUNC(sub_82A63700) { __imp__sub_82A63700(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63700) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// lis r10,-32085
	ctx.r10.s64 = -2102722560;
	// lfs f0,-18208(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -18208);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31993
	ctx.r11.s64 = -2096693248;
	// addi r11,r11,15376
	ctx.r11.s64 = ctx.r11.s64 + 15376;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// lfs f0,-18204(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -18204);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63728);
PPC_WEAK_FUNC(sub_82A63728) { __imp__sub_82A63728(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63728) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// lis r10,-32085
	ctx.r10.s64 = -2102722560;
	// lfs f0,-18208(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -18208);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31993
	ctx.r11.s64 = -2096693248;
	// addi r11,r11,15392
	ctx.r11.s64 = ctx.r11.s64 + 15392;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// lfs f0,-18204(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -18204);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63750);
PPC_WEAK_FUNC(sub_82A63750) { __imp__sub_82A63750(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63750) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7cc
	ctx.lr = 0x82A63758;
	__savegprlr_29(ctx, base);
	// stfd f31,-40(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -40, ctx.f31.u64);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31993
	ctx.r11.s64 = -2096693248;
	// li r30,49
	ctx.r30.s64 = 49;
	// addi r11,r11,15408
	ctx.r11.s64 = ctx.r11.s64 + 15408;
	// li r29,0
	ctx.r29.s64 = 0;
	// addi r31,r11,600
	ctx.r31.s64 = ctx.r11.s64 + 600;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f31,2612(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2612);
	ctx.f31.f64 = double(temp.f32);
loc_82A6377C:
	// lis r11,-32228
	ctx.r11.s64 = -2112094208;
	// li r5,12
	ctx.r5.s64 = 12;
	// addi r6,r11,26400
	ctx.r6.s64 = ctx.r11.s64 + 26400;
	// li r4,48
	ctx.r4.s64 = 48;
	// addi r3,r31,-600
	ctx.r3.s64 = ctx.r31.s64 + -600;
	// bl 0x82140220
	ctx.lr = 0x82A63794;
	sub_82140220(ctx, base);
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
	// bge cr6,0x82a6377c
	if (!ctx.cr6.lt) goto loc_82A6377C;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// lfd f31,-40(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -40);
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A637D8);
PPC_WEAK_FUNC(sub_82A637D8) { __imp__sub_82A637D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A637D8) {
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
	// addi r31,r11,1568
	ctx.r31.s64 = ctx.r11.s64 + 1568;
loc_82A637FC:
	// lis r11,-32171
	ctx.r11.s64 = -2108358656;
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r6,r11,21616
	ctx.r6.s64 = ctx.r11.s64 + 21616;
	// li r4,1552
	ctx.r4.s64 = 1552;
	// addi r3,r31,-1552
	ctx.r3.s64 = ctx.r31.s64 + -1552;
	// bl 0x82140220
	ctx.lr = 0x82A63814;
	sub_82140220(ctx, base);
	// lis r11,-32171
	ctx.r11.s64 = -2108358656;
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r6,r11,25168
	ctx.r6.s64 = ctx.r11.s64 + 25168;
	// li r4,2496
	ctx.r4.s64 = 2496;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82140220
	ctx.lr = 0x82A6382C;
	sub_82140220(ctx, base);
	// addi r3,r31,2500
	ctx.r3.s64 = ctx.r31.s64 + 2500;
	// bl 0x8285d0d8
	ctx.lr = 0x82A63834;
	sub_8285D0D8(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// addi r31,r31,4240
	ctx.r31.s64 = ctx.r31.s64 + 4240;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a637fc
	if (!ctx.cr6.lt) goto loc_82A637FC;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4096
	ctx.r3.s64 = ctx.r11.s64 + 4096;
	// bl 0x829ffa48
	ctx.lr = 0x82A63850;
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

PPC_FUNC_IMPL(__imp__sub_82A63868);
PPC_WEAK_FUNC(sub_82A63868) { __imp__sub_82A63868(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63868) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-2016
	ctx.r3.s64 = ctx.r11.s64 + -2016;
	// bl 0x8285fe48
	ctx.lr = 0x82A63880;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4064
	ctx.r3.s64 = ctx.r11.s64 + 4064;
	// bl 0x829ffa48
	ctx.lr = 0x82A6388C;
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

PPC_FUNC_IMPL(__imp__sub_82A638A0);
PPC_WEAK_FUNC(sub_82A638A0) { __imp__sub_82A638A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A638A0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-1984
	ctx.r3.s64 = ctx.r11.s64 + -1984;
	// bl 0x8285fe48
	ctx.lr = 0x82A638B8;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4080
	ctx.r3.s64 = ctx.r11.s64 + 4080;
	// bl 0x829ffa48
	ctx.lr = 0x82A638C4;
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

PPC_FUNC_IMPL(__imp__sub_82A638D8);
PPC_WEAK_FUNC(sub_82A638D8) { __imp__sub_82A638D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A638D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4200
	ctx.r3.s64 = ctx.r11.s64 + 4200;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A638E8);
PPC_WEAK_FUNC(sub_82A638E8) { __imp__sub_82A638E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A638E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,26000
	ctx.r5.s64 = ctx.r11.s64 + 26000;
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-1244
	ctx.r3.s64 = ctx.r11.s64 + -1244;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63908);
PPC_WEAK_FUNC(sub_82A63908) { __imp__sub_82A63908(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63908) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4232
	ctx.r3.s64 = ctx.r11.s64 + 4232;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63918);
PPC_WEAK_FUNC(sub_82A63918) { __imp__sub_82A63918(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63918) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4264
	ctx.r3.s64 = ctx.r11.s64 + 4264;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63928);
PPC_WEAK_FUNC(sub_82A63928) { __imp__sub_82A63928(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63928) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-24748
	ctx.r5.s64 = ctx.r11.s64 + -24748;
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,4076
	ctx.r3.s64 = ctx.r11.s64 + 4076;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63948);
PPC_WEAK_FUNC(sub_82A63948) { __imp__sub_82A63948(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63948) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-15636
	ctx.r5.s64 = ctx.r11.s64 + -15636;
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,4156
	ctx.r3.s64 = ctx.r11.s64 + 4156;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63968);
PPC_WEAK_FUNC(sub_82A63968) { __imp__sub_82A63968(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63968) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-15620
	ctx.r5.s64 = ctx.r11.s64 + -15620;
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,4176
	ctx.r3.s64 = ctx.r11.s64 + 4176;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63988);
PPC_WEAK_FUNC(sub_82A63988) { __imp__sub_82A63988(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63988) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// li r10,127
	ctx.r10.s64 = 127;
	// addi r11,r11,4200
	ctx.r11.s64 = ctx.r11.s64 + 4200;
	// li r7,-1
	ctx.r7.s64 = -1;
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// li r9,0
	ctx.r9.s64 = 0;
	// li r8,3
	ctx.r8.s64 = 3;
loc_82A639A4:
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
	// bge cr6,0x82a639a4
	if (!ctx.cr6.lt) goto loc_82A639A4;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4272
	ctx.r3.s64 = ctx.r11.s64 + 4272;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A639D8);
PPC_WEAK_FUNC(sub_82A639D8) { __imp__sub_82A639D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A639D8) {
	PPC_FUNC_PROLOGUE();
loc_82A639D8:
	// mftb r11
	ctx.r11.u64 = PPC_QUERY_TIMEBASE();
	// rotlwi r10,r11,0
	ctx.r10.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x82a639d8
	if (ctx.cr6.eq) goto loc_82A639D8;
	// lis r10,-31992
	ctx.r10.s64 = -2096627712;
	// std r11,6776(r10)
	PPC_STORE_U64(ctx.r10.u32 + 6776, ctx.r11.u64);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A639F8);
PPC_WEAK_FUNC(sub_82A639F8) { __imp__sub_82A639F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A639F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// li r8,2400
	ctx.r8.s64 = 2400;
	// addi r11,r11,18128
	ctx.r11.s64 = ctx.r11.s64 + 18128;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r11,r11,3
	ctx.r11.s64 = ctx.r11.s64 + 3;
	// li r10,-128
	ctx.r10.s64 = -128;
loc_82A63A10:
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
	// bne cr6,0x82a63a10
	if (!ctx.cr6.eq) goto loc_82A63A10;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63A58);
PPC_WEAK_FUNC(sub_82A63A58) { __imp__sub_82A63A58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63A58) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-18544
	ctx.r3.s64 = ctx.r11.s64 + -18544;
	// bl 0x8290c330
	ctx.lr = 0x82A63A70;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4280
	ctx.r3.s64 = ctx.r11.s64 + 4280;
	// bl 0x829ffa48
	ctx.lr = 0x82A63A7C;
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

PPC_FUNC_IMPL(__imp__sub_82A63A90);
PPC_WEAK_FUNC(sub_82A63A90) { __imp__sub_82A63A90(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63A90) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-12428
	ctx.r5.s64 = ctx.r11.s64 + -12428;
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-18504
	ctx.r3.s64 = ctx.r11.s64 + -18504;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63AB0);
PPC_WEAK_FUNC(sub_82A63AB0) { __imp__sub_82A63AB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63AB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-12416
	ctx.r5.s64 = ctx.r11.s64 + -12416;
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-18484
	ctx.r3.s64 = ctx.r11.s64 + -18484;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63AD0);
PPC_WEAK_FUNC(sub_82A63AD0) { __imp__sub_82A63AD0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63AD0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,24720
	ctx.r3.s64 = ctx.r11.s64 + 24720;
	// bl 0x8285fe48
	ctx.lr = 0x82A63AE8;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4288
	ctx.r3.s64 = ctx.r11.s64 + 4288;
	// bl 0x829ffa48
	ctx.lr = 0x82A63AF4;
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

PPC_FUNC_IMPL(__imp__sub_82A63B08);
PPC_WEAK_FUNC(sub_82A63B08) { __imp__sub_82A63B08(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63B08) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// addi r3,r11,24752
	ctx.r3.s64 = ctx.r11.s64 + 24752;
	// b 0x82478a80
	sub_82478A80(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63B18);
PPC_WEAK_FUNC(sub_82A63B18) { __imp__sub_82A63B18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63B18) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18816
	ctx.r11.s64 = ctx.r11.s64 + -18816;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31991
	ctx.r11.s64 = -2096562176;
	// addi r11,r11,24800
	ctx.r11.s64 = ctx.r11.s64 + 24800;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63B38);
PPC_WEAK_FUNC(sub_82A63B38) { __imp__sub_82A63B38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63B38) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,-17508
	ctx.r3.s64 = ctx.r11.s64 + -17508;
	// bl 0x8290c330
	ctx.lr = 0x82A63B50;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4304
	ctx.r3.s64 = ctx.r11.s64 + 4304;
	// bl 0x829ffa48
	ctx.lr = 0x82A63B5C;
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

PPC_FUNC_IMPL(__imp__sub_82A63B70);
PPC_WEAK_FUNC(sub_82A63B70) { __imp__sub_82A63B70(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63B70) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,-17468
	ctx.r3.s64 = ctx.r11.s64 + -17468;
	// bl 0x8290c330
	ctx.lr = 0x82A63B88;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4312
	ctx.r3.s64 = ctx.r11.s64 + 4312;
	// bl 0x829ffa48
	ctx.lr = 0x82A63B94;
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

PPC_FUNC_IMPL(__imp__sub_82A63BA8);
PPC_WEAK_FUNC(sub_82A63BA8) { __imp__sub_82A63BA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63BA8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,-17428
	ctx.r3.s64 = ctx.r11.s64 + -17428;
	// bl 0x8290c330
	ctx.lr = 0x82A63BC0;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4320
	ctx.r3.s64 = ctx.r11.s64 + 4320;
	// bl 0x829ffa48
	ctx.lr = 0x82A63BCC;
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

PPC_FUNC_IMPL(__imp__sub_82A63BE0);
PPC_WEAK_FUNC(sub_82A63BE0) { __imp__sub_82A63BE0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63BE0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,-17388
	ctx.r3.s64 = ctx.r11.s64 + -17388;
	// bl 0x8290c330
	ctx.lr = 0x82A63BF8;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4328
	ctx.r3.s64 = ctx.r11.s64 + 4328;
	// bl 0x829ffa48
	ctx.lr = 0x82A63C04;
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

PPC_FUNC_IMPL(__imp__sub_82A63C18);
PPC_WEAK_FUNC(sub_82A63C18) { __imp__sub_82A63C18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63C18) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,-17348
	ctx.r3.s64 = ctx.r11.s64 + -17348;
	// bl 0x8290c330
	ctx.lr = 0x82A63C30;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4336
	ctx.r3.s64 = ctx.r11.s64 + 4336;
	// bl 0x829ffa48
	ctx.lr = 0x82A63C3C;
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

PPC_FUNC_IMPL(__imp__sub_82A63C50);
PPC_WEAK_FUNC(sub_82A63C50) { __imp__sub_82A63C50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63C50) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,-17308
	ctx.r3.s64 = ctx.r11.s64 + -17308;
	// bl 0x8290c330
	ctx.lr = 0x82A63C68;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4344
	ctx.r3.s64 = ctx.r11.s64 + 4344;
	// bl 0x829ffa48
	ctx.lr = 0x82A63C74;
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

PPC_FUNC_IMPL(__imp__sub_82A63C88);
PPC_WEAK_FUNC(sub_82A63C88) { __imp__sub_82A63C88(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63C88) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,-17268
	ctx.r3.s64 = ctx.r11.s64 + -17268;
	// bl 0x8290c330
	ctx.lr = 0x82A63CA0;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4352
	ctx.r3.s64 = ctx.r11.s64 + 4352;
	// bl 0x829ffa48
	ctx.lr = 0x82A63CAC;
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

PPC_FUNC_IMPL(__imp__sub_82A63CC0);
PPC_WEAK_FUNC(sub_82A63CC0) { __imp__sub_82A63CC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63CC0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,-17228
	ctx.r3.s64 = ctx.r11.s64 + -17228;
	// bl 0x8290c330
	ctx.lr = 0x82A63CD8;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4360
	ctx.r3.s64 = ctx.r11.s64 + 4360;
	// bl 0x829ffa48
	ctx.lr = 0x82A63CE4;
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

PPC_FUNC_IMPL(__imp__sub_82A63CF8);
PPC_WEAK_FUNC(sub_82A63CF8) { __imp__sub_82A63CF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63CF8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r31,r11,-15056
	ctx.r31.s64 = ctx.r11.s64 + -15056;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x823d4038
	ctx.lr = 0x82A63D18;
	sub_823D4038(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r11,r11,-10448
	ctx.r11.s64 = ctx.r11.s64 + -10448;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x826083a0
	ctx.lr = 0x82A63D2C;
	sub_826083A0(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4368
	ctx.r3.s64 = ctx.r11.s64 + 4368;
	// bl 0x829ffa48
	ctx.lr = 0x82A63D38;
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

PPC_FUNC_IMPL(__imp__sub_82A63D50);
PPC_WEAK_FUNC(sub_82A63D50) { __imp__sub_82A63D50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63D50) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32244
	ctx.r11.s64 = -2113142784;
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// lfs f0,-4416(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -4416);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r11,r11,-14416
	ctx.r11.s64 = ctx.r11.s64 + -14416;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// lfs f0,-27024(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -27024);
	ctx.f0.f64 = double(temp.f32);
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// lfs f0,-5180(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -5180);
	ctx.f0.f64 = double(temp.f32);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// lfs f0,3556(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 3556);
	ctx.f0.f64 = double(temp.f32);
	// lis r10,-32244
	ctx.r10.s64 = -2113142784;
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// lfs f0,-4180(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -4180);
	ctx.f0.f64 = double(temp.f32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// stfs f0,16(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 16, temp.u32);
	// lfs f0,2612(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// stfs f0,20(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 20, temp.u32);
	// lfs f0,3400(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 3400);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,24(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 24, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63DB0);
PPC_WEAK_FUNC(sub_82A63DB0) { __imp__sub_82A63DB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63DB0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7a8
	ctx.lr = 0x82A63DB8;
	__savegprlr_20(ctx, base);
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// lis r26,-32244
	ctx.r26.s64 = -2113142784;
	// lis r27,-32244
	ctx.r27.s64 = -2113142784;
	// lis r28,-32254
	ctx.r28.s64 = -2113798144;
	// lis r29,-32254
	ctx.r29.s64 = -2113798144;
	// lis r30,-32256
	ctx.r30.s64 = -2113929216;
	// lis r31,-32255
	ctx.r31.s64 = -2113863680;
	// lfs f1,-4864(r26)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r26.u32 + -4864);
	ctx.f1.f64 = double(temp.f32);
	// lis r3,-32256
	ctx.r3.s64 = -2113929216;
	// lfs f2,-4156(r27)
	temp.u32 = PPC_LOAD_U32(ctx.r27.u32 + -4156);
	ctx.f2.f64 = double(temp.f32);
	// lis r4,-32256
	ctx.r4.s64 = -2113929216;
	// lfs f3,-26308(r28)
	temp.u32 = PPC_LOAD_U32(ctx.r28.u32 + -26308);
	ctx.f3.f64 = double(temp.f32);
	// lis r5,-32256
	ctx.r5.s64 = -2113929216;
	// lfs f4,-27020(r29)
	temp.u32 = PPC_LOAD_U32(ctx.r29.u32 + -27020);
	ctx.f4.f64 = double(temp.f32);
	// lis r6,-32254
	ctx.r6.s64 = -2113798144;
	// lfs f5,2612(r30)
	temp.u32 = PPC_LOAD_U32(ctx.r30.u32 + 2612);
	ctx.f5.f64 = double(temp.f32);
	// lis r7,-32254
	ctx.r7.s64 = -2113798144;
	// lfs f6,-27700(r31)
	temp.u32 = PPC_LOAD_U32(ctx.r31.u32 + -27700);
	ctx.f6.f64 = double(temp.f32);
	// lis r8,-32244
	ctx.r8.s64 = -2113142784;
	// lfs f7,3444(r3)
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 3444);
	ctx.f7.f64 = double(temp.f32);
	// lis r9,-32244
	ctx.r9.s64 = -2113142784;
	// lfs f8,11616(r4)
	temp.u32 = PPC_LOAD_U32(ctx.r4.u32 + 11616);
	ctx.f8.f64 = double(temp.f32);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// lfs f9,3528(r5)
	temp.u32 = PPC_LOAD_U32(ctx.r5.u32 + 3528);
	ctx.f9.f64 = double(temp.f32);
	// addi r11,r11,-14384
	ctx.r11.s64 = ctx.r11.s64 + -14384;
	// lfs f10,-23432(r6)
	temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + -23432);
	ctx.f10.f64 = double(temp.f32);
	// li r20,1
	ctx.r20.s64 = 1;
	// lfs f11,-22844(r7)
	temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + -22844);
	ctx.f11.f64 = double(temp.f32);
	// lfs f12,-4908(r8)
	temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + -4908);
	ctx.f12.f64 = double(temp.f32);
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// lfs f13,-4624(r9)
	temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + -4624);
	ctx.f13.f64 = double(temp.f32);
	// mr r25,r20
	ctx.r25.u64 = ctx.r20.u64;
	// lfs f0,-26300(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -26300);
	ctx.f0.f64 = double(temp.f32);
	// li r24,0
	ctx.r24.s64 = 0;
	// li r21,800
	ctx.r21.s64 = 800;
	// li r22,200
	ctx.r22.s64 = 200;
	// li r23,700
	ctx.r23.s64 = 700;
loc_82A63E4C:
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
	// bge cr6,0x82a63e4c
	if (!ctx.cr6.lt) goto loc_82A63E4C;
	// b 0x829ff7f8
	__restgprlr_20(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63EB0);
PPC_WEAK_FUNC(sub_82A63EB0) { __imp__sub_82A63EB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63EB0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r10,2
	ctx.r10.s64 = 2;
	// lfs f0,5764(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 5764);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r11,r11,-14216
	ctx.r11.s64 = ctx.r11.s64 + -14216;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stw r10,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r10.u32);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// lfs f0,-31684(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -31684);
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
	// lfs f0,11616(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 11616);
	ctx.f0.f64 = double(temp.f32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// lfs f0,3400(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 3400);
	ctx.f0.f64 = double(temp.f32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// stfs f0,20(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 20, temp.u32);
	// lfs f0,3444(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 3444);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,24(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 24, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A63F10);
PPC_WEAK_FUNC(sub_82A63F10) { __imp__sub_82A63F10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A63F10) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7a4
	ctx.lr = 0x82A63F18;
	__savegprlr_19(ctx, base);
	// stfd f30,-128(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -128, ctx.f30.u64);
	// stfd f31,-120(r1)
	PPC_STORE_U64(ctx.r1.u32 + -120, ctx.f31.u64);
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// lis r23,-32255
	ctx.r23.s64 = -2113863680;
	// lis r24,-32256
	ctx.r24.s64 = -2113929216;
	// lis r25,-32255
	ctx.r25.s64 = -2113863680;
	// lis r26,-32256
	ctx.r26.s64 = -2113929216;
	// lis r27,-32255
	ctx.r27.s64 = -2113863680;
	// lis r28,-32256
	ctx.r28.s64 = -2113929216;
	// lfs f30,-27700(r23)
	temp.u32 = PPC_LOAD_U32(ctx.r23.u32 + -27700);
	ctx.f30.f64 = double(temp.f32);
	// lis r29,-32244
	ctx.r29.s64 = -2113142784;
	// lfs f31,2612(r24)
	temp.u32 = PPC_LOAD_U32(ctx.r24.u32 + 2612);
	ctx.f31.f64 = double(temp.f32);
	// lis r30,-32256
	ctx.r30.s64 = -2113929216;
	// lfs f1,14400(r25)
	temp.u32 = PPC_LOAD_U32(ctx.r25.u32 + 14400);
	ctx.f1.f64 = double(temp.f32);
	// lis r31,-32256
	ctx.r31.s64 = -2113929216;
	// lfs f2,2604(r26)
	temp.u32 = PPC_LOAD_U32(ctx.r26.u32 + 2604);
	ctx.f2.f64 = double(temp.f32);
	// lis r3,-32256
	ctx.r3.s64 = -2113929216;
	// lfs f3,8748(r27)
	temp.u32 = PPC_LOAD_U32(ctx.r27.u32 + 8748);
	ctx.f3.f64 = double(temp.f32);
	// lis r4,-32254
	ctx.r4.s64 = -2113798144;
	// lfs f4,9088(r28)
	temp.u32 = PPC_LOAD_U32(ctx.r28.u32 + 9088);
	ctx.f4.f64 = double(temp.f32);
	// lis r5,-32254
	ctx.r5.s64 = -2113798144;
	// lfs f5,-4864(r29)
	temp.u32 = PPC_LOAD_U32(ctx.r29.u32 + -4864);
	ctx.f5.f64 = double(temp.f32);
	// lis r6,-32254
	ctx.r6.s64 = -2113798144;
	// lfs f9,3444(r30)
	temp.u32 = PPC_LOAD_U32(ctx.r30.u32 + 3444);
	ctx.f9.f64 = double(temp.f32);
	// lis r7,-32254
	ctx.r7.s64 = -2113798144;
	// lfs f10,3436(r31)
	temp.u32 = PPC_LOAD_U32(ctx.r31.u32 + 3436);
	ctx.f10.f64 = double(temp.f32);
	// lis r8,-32256
	ctx.r8.s64 = -2113929216;
	// lfs f11,3528(r3)
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 3528);
	ctx.f11.f64 = double(temp.f32);
	// lis r9,-32254
	ctx.r9.s64 = -2113798144;
	// lfs f6,-22852(r4)
	temp.u32 = PPC_LOAD_U32(ctx.r4.u32 + -22852);
	ctx.f6.f64 = double(temp.f32);
	// addi r11,r11,-14184
	ctx.r11.s64 = ctx.r11.s64 + -14184;
	// lfs f7,-27016(r5)
	temp.u32 = PPC_LOAD_U32(ctx.r5.u32 + -27016);
	ctx.f7.f64 = double(temp.f32);
	// lfs f13,-26300(r6)
	temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + -26300);
	ctx.f13.f64 = double(temp.f32);
	// li r21,9
	ctx.r21.s64 = 9;
	// lfs f12,-27020(r7)
	temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + -27020);
	ctx.f12.f64 = double(temp.f32);
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// lfs f0,3400(r8)
	temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + 3400);
	ctx.f0.f64 = double(temp.f32);
	// li r19,6
	ctx.r19.s64 = 6;
	// lfs f8,-2004(r9)
	temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + -2004);
	ctx.f8.f64 = double(temp.f32);
	// li r20,400
	ctx.r20.s64 = 400;
	// li r22,1000
	ctx.r22.s64 = 1000;
	// li r10,0
	ctx.r10.s64 = 0;
loc_82A63FC0:
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
	// bge cr6,0x82a63fc0
	if (!ctx.cr6.lt) goto loc_82A63FC0;
	// lfd f30,-128(r1)
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -128);
	// lfd f31,-120(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -120);
	// b 0x829ff7f4
	__restgprlr_19(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A64068);
PPC_WEAK_FUNC(sub_82A64068) { __imp__sub_82A64068(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64068) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r10,0
	ctx.r10.s64 = 0;
	// lfs f0,3400(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3400);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r11,r11,-12624
	ctx.r11.s64 = ctx.r11.s64 + -12624;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stb r10,60(r11)
	PPC_STORE_U8(ctx.r11.u32 + 60, ctx.r10.u8);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// lfs f13,-22852(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -22852);
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
	// lfs f11,5764(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 5764);
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
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// lfs f10,-23432(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -23432);
	ctx.f10.f64 = double(temp.f32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stfs f10,16(r11)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r11.u32 + 16, temp.u32);
	// stb r10,125(r11)
	PPC_STORE_U8(ctx.r11.u32 + 125, ctx.r10.u8);
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// lfs f12,-5448(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -5448);
	ctx.f12.f64 = double(temp.f32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stfs f12,20(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 20, temp.u32);
	// stb r10,126(r11)
	PPC_STORE_U8(ctx.r11.u32 + 126, ctx.r10.u8);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// lfs f9,-27016(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -27016);
	ctx.f9.f64 = double(temp.f32);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// stfs f9,24(r11)
	temp.f32 = float(ctx.f9.f64);
	PPC_STORE_U32(ctx.r11.u32 + 24, temp.u32);
	// stfs f12,28(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 28, temp.u32);
	// lfs f8,-20596(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -20596);
	ctx.f8.f64 = double(temp.f32);
	// lis r10,-32255
	ctx.r10.s64 = -2113863680;
	// stfs f8,32(r11)
	temp.f32 = float(ctx.f8.f64);
	PPC_STORE_U32(ctx.r11.u32 + 32, temp.u32);
	// lfs f7,20884(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 20884);
	ctx.f7.f64 = double(temp.f32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// stfs f7,36(r11)
	temp.f32 = float(ctx.f7.f64);
	PPC_STORE_U32(ctx.r11.u32 + 36, temp.u32);
	// lfs f6,3444(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 3444);
	ctx.f6.f64 = double(temp.f32);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// stfs f6,40(r11)
	temp.f32 = float(ctx.f6.f64);
	PPC_STORE_U32(ctx.r11.u32 + 40, temp.u32);
	// stfs f13,44(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 44, temp.u32);
	// lfs f5,-27020(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -27020);
	ctx.f5.f64 = double(temp.f32);
	// lis r10,-32244
	ctx.r10.s64 = -2113142784;
	// stfs f5,48(r11)
	temp.f32 = float(ctx.f5.f64);
	PPC_STORE_U32(ctx.r11.u32 + 48, temp.u32);
	// lfs f4,-4908(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -4908);
	ctx.f4.f64 = double(temp.f32);
	// lis r10,-32255
	ctx.r10.s64 = -2113863680;
	// stfs f4,52(r11)
	temp.f32 = float(ctx.f4.f64);
	PPC_STORE_U32(ctx.r11.u32 + 52, temp.u32);
	// lfs f3,12864(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 12864);
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

PPC_FUNC_IMPL(__imp__sub_82A64178);
PPC_WEAK_FUNC(sub_82A64178) { __imp__sub_82A64178(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64178) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7a0
	ctx.lr = 0x82A64180;
	__savegprlr_18(ctx, base);
	// addi r12,r1,-120
	ctx.r12.s64 = ctx.r1.s64 + -120;
	// bl 0x82a01320
	ctx.lr = 0x82A64188;
	__savefpr_26(ctx, base);
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// lis r20,-32252
	ctx.r20.s64 = -2113667072;
	// lis r21,-32256
	ctx.r21.s64 = -2113929216;
	// lis r22,-32244
	ctx.r22.s64 = -2113142784;
	// lis r23,-32254
	ctx.r23.s64 = -2113798144;
	// lis r24,-32256
	ctx.r24.s64 = -2113929216;
	// lis r25,-32252
	ctx.r25.s64 = -2113667072;
	// lfs f12,-5448(r20)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r20.u32 + -5448);
	ctx.f12.f64 = double(temp.f32);
	// lis r26,-32244
	ctx.r26.s64 = -2113142784;
	// lfs f5,5808(r21)
	temp.u32 = PPC_LOAD_U32(ctx.r21.u32 + 5808);
	ctx.f5.f64 = double(temp.f32);
	// lis r27,-32256
	ctx.r27.s64 = -2113929216;
	// lfs f26,-4908(r22)
	temp.u32 = PPC_LOAD_U32(ctx.r22.u32 + -4908);
	ctx.f26.f64 = double(temp.f32);
	// lis r28,-32256
	ctx.r28.s64 = -2113929216;
	// lfs f27,-27016(r23)
	temp.u32 = PPC_LOAD_U32(ctx.r23.u32 + -27016);
	ctx.f27.f64 = double(temp.f32);
	// lis r29,-32254
	ctx.r29.s64 = -2113798144;
	// lfs f28,3444(r24)
	temp.u32 = PPC_LOAD_U32(ctx.r24.u32 + 3444);
	ctx.f28.f64 = double(temp.f32);
	// lis r30,-32254
	ctx.r30.s64 = -2113798144;
	// lfs f29,26448(r25)
	temp.u32 = PPC_LOAD_U32(ctx.r25.u32 + 26448);
	ctx.f29.f64 = double(temp.f32);
	// lis r31,-32256
	ctx.r31.s64 = -2113929216;
	// lfs f6,-4200(r26)
	temp.u32 = PPC_LOAD_U32(ctx.r26.u32 + -4200);
	ctx.f6.f64 = double(temp.f32);
	// lis r3,-32256
	ctx.r3.s64 = -2113929216;
	// lfs f7,3436(r27)
	temp.u32 = PPC_LOAD_U32(ctx.r27.u32 + 3436);
	ctx.f7.f64 = double(temp.f32);
	// lis r4,-32254
	ctx.r4.s64 = -2113798144;
	// lfs f30,11616(r28)
	temp.u32 = PPC_LOAD_U32(ctx.r28.u32 + 11616);
	ctx.f30.f64 = double(temp.f32);
	// lis r5,-32254
	ctx.r5.s64 = -2113798144;
	// lfs f31,-30104(r29)
	temp.u32 = PPC_LOAD_U32(ctx.r29.u32 + -30104);
	ctx.f31.f64 = double(temp.f32);
	// lis r6,-32254
	ctx.r6.s64 = -2113798144;
	// lfs f8,-27020(r30)
	temp.u32 = PPC_LOAD_U32(ctx.r30.u32 + -27020);
	ctx.f8.f64 = double(temp.f32);
	// lis r7,-32256
	ctx.r7.s64 = -2113929216;
	// lfs f1,19980(r31)
	temp.u32 = PPC_LOAD_U32(ctx.r31.u32 + 19980);
	ctx.f1.f64 = double(temp.f32);
	// lis r8,-32255
	ctx.r8.s64 = -2113863680;
	// lfs f13,2612(r3)
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 2612);
	ctx.f13.f64 = double(temp.f32);
	// lis r9,-32244
	ctx.r9.s64 = -2113142784;
	// lfs f9,-22852(r4)
	temp.u32 = PPC_LOAD_U32(ctx.r4.u32 + -22852);
	ctx.f9.f64 = double(temp.f32);
	// lis r10,-32244
	ctx.r10.s64 = -2113142784;
	// lfs f2,-20904(r5)
	temp.u32 = PPC_LOAD_U32(ctx.r5.u32 + -20904);
	ctx.f2.f64 = double(temp.f32);
	// addi r11,r11,-12496
	ctx.r11.s64 = ctx.r11.s64 + -12496;
	// lfs f10,-26300(r6)
	temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + -26300);
	ctx.f10.f64 = double(temp.f32);
	// lfs f0,3400(r7)
	temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + 3400);
	ctx.f0.f64 = double(temp.f32);
	// li r18,3
	ctx.r18.s64 = 3;
	// lfs f3,-27700(r8)
	temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + -27700);
	ctx.f3.f64 = double(temp.f32);
	// addi r11,r11,88
	ctx.r11.s64 = ctx.r11.s64 + 88;
	// lfs f11,-4624(r9)
	temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + -4624);
	ctx.f11.f64 = double(temp.f32);
	// li r19,1
	ctx.r19.s64 = 1;
	// lfs f4,-4216(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -4216);
	ctx.f4.f64 = double(temp.f32);
loc_82A6423C:
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
	// bge cr6,0x82a6423c
	if (!ctx.cr6.lt) goto loc_82A6423C;
	// addi r12,r1,-120
	ctx.r12.s64 = ctx.r1.s64 + -120;
	// bl 0x82a0136c
	ctx.lr = 0x82A642FC;
	__restfpr_26(ctx, base);
	// b 0x829ff7f0
	__restgprlr_18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A64300);
PPC_WEAK_FUNC(sub_82A64300) { __imp__sub_82A64300(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64300) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// li r10,500
	ctx.r10.s64 = 500;
	// lfs f0,-3544(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -3544);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r11,r11,-11792
	ctx.r11.s64 = ctx.r11.s64 + -11792;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stw r10,108(r11)
	PPC_STORE_U32(ctx.r11.u32 + 108, ctx.r10.u32);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// lfs f0,23188(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 23188);
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
	// lfs f0,3400(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 3400);
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
	// lfs f13,9088(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 9088);
	ctx.f13.f64 = double(temp.f32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// stfs f13,12(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// lfs f13,2612(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 2612);
	ctx.f13.f64 = double(temp.f32);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// stfs f13,16(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 16, temp.u32);
	// lfs f12,-27024(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -27024);
	ctx.f12.f64 = double(temp.f32);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// stfs f12,20(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 20, temp.u32);
	// lfs f11,-27020(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -27020);
	ctx.f11.f64 = double(temp.f32);
	// lis r10,-32244
	ctx.r10.s64 = -2113142784;
	// stfs f11,24(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 24, temp.u32);
	// stfs f12,28(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 28, temp.u32);
	// lfs f11,-4908(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -4908);
	ctx.f11.f64 = double(temp.f32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// stfs f11,32(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 32, temp.u32);
	// lfs f11,19980(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 19980);
	ctx.f11.f64 = double(temp.f32);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// stfs f11,36(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 36, temp.u32);
	// lfs f11,-20904(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -20904);
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
	// lfs f11,3444(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 3444);
	ctx.f11.f64 = double(temp.f32);
	// lis r10,-32244
	ctx.r10.s64 = -2113142784;
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
	// lfs f0,-4180(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -4180);
	ctx.f0.f64 = double(temp.f32);
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// stfs f12,88(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 88, temp.u32);
	// stfs f13,92(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 92, temp.u32);
	// stfs f0,96(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 96, temp.u32);
	// lfs f0,26464(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 26464);
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

PPC_FUNC_IMPL(__imp__sub_82A643F8);
PPC_WEAK_FUNC(sub_82A643F8) { __imp__sub_82A643F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A643F8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7b4
	ctx.lr = 0x82A64400;
	__savegprlr_23(ctx, base);
	// stfd f31,-88(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -88, ctx.f31.u64);
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// lis r25,-32255
	ctx.r25.s64 = -2113863680;
	// lis r26,-32255
	ctx.r26.s64 = -2113863680;
	// lis r27,-32244
	ctx.r27.s64 = -2113142784;
	// lis r28,-32254
	ctx.r28.s64 = -2113798144;
	// lis r29,-32244
	ctx.r29.s64 = -2113142784;
	// lis r30,-32244
	ctx.r30.s64 = -2113142784;
	// lfs f31,14396(r25)
	temp.u32 = PPC_LOAD_U32(ctx.r25.u32 + 14396);
	ctx.f31.f64 = double(temp.f32);
	// lis r31,-32256
	ctx.r31.s64 = -2113929216;
	// lfs f1,20884(r26)
	temp.u32 = PPC_LOAD_U32(ctx.r26.u32 + 20884);
	ctx.f1.f64 = double(temp.f32);
	// lis r3,-32256
	ctx.r3.s64 = -2113929216;
	// lfs f2,-4200(r27)
	temp.u32 = PPC_LOAD_U32(ctx.r27.u32 + -4200);
	ctx.f2.f64 = double(temp.f32);
	// lis r4,-32254
	ctx.r4.s64 = -2113798144;
	// lfs f10,-27024(r28)
	temp.u32 = PPC_LOAD_U32(ctx.r28.u32 + -27024);
	ctx.f10.f64 = double(temp.f32);
	// lis r5,-32244
	ctx.r5.s64 = -2113142784;
	// lfs f3,-4864(r29)
	temp.u32 = PPC_LOAD_U32(ctx.r29.u32 + -4864);
	ctx.f3.f64 = double(temp.f32);
	// lis r6,-32256
	ctx.r6.s64 = -2113929216;
	// lfs f4,-4880(r30)
	temp.u32 = PPC_LOAD_U32(ctx.r30.u32 + -4880);
	ctx.f4.f64 = double(temp.f32);
	// lis r7,-32244
	ctx.r7.s64 = -2113142784;
	// lfs f11,3528(r31)
	temp.u32 = PPC_LOAD_U32(ctx.r31.u32 + 3528);
	ctx.f11.f64 = double(temp.f32);
	// lis r8,-32244
	ctx.r8.s64 = -2113142784;
	// lfs f13,3444(r3)
	temp.u32 = PPC_LOAD_U32(ctx.r3.u32 + 3444);
	ctx.f13.f64 = double(temp.f32);
	// lis r9,-32254
	ctx.r9.s64 = -2113798144;
	// lfs f5,23188(r4)
	temp.u32 = PPC_LOAD_U32(ctx.r4.u32 + 23188);
	ctx.f5.f64 = double(temp.f32);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// lfs f12,-4180(r5)
	temp.u32 = PPC_LOAD_U32(ctx.r5.u32 + -4180);
	ctx.f12.f64 = double(temp.f32);
	// addi r11,r11,-11664
	ctx.r11.s64 = ctx.r11.s64 + -11664;
	// lfs f0,3400(r6)
	temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + 3400);
	ctx.f0.f64 = double(temp.f32);
	// lfs f6,-4624(r7)
	temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + -4624);
	ctx.f6.f64 = double(temp.f32);
	// li r24,1
	ctx.r24.s64 = 1;
	// lfs f7,-4636(r8)
	temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + -4636);
	ctx.f7.f64 = double(temp.f32);
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// lfs f8,-20904(r9)
	temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + -20904);
	ctx.f8.f64 = double(temp.f32);
	// li r23,1500
	ctx.r23.s64 = 1500;
	// lfs f9,-22844(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -22844);
	ctx.f9.f64 = double(temp.f32);
loc_82A64490:
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
	// bge cr6,0x82a64490
	if (!ctx.cr6.lt) goto loc_82A64490;
	// lfd f31,-88(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -88);
	// b 0x829ff804
	__restgprlr_23(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A64510);
PPC_WEAK_FUNC(sub_82A64510) { __imp__sub_82A64510(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64510) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r10,750
	ctx.r10.s64 = 750;
	// lfs f0,19980(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 19980);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r11,r11,-11456
	ctx.r11.s64 = ctx.r11.s64 + -11456;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stw r10,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r10.u32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lfs f0,9096(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 9096);
	ctx.f0.f64 = double(temp.f32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stb r10,68(r11)
	PPC_STORE_U8(ctx.r11.u32 + 68, ctx.r10.u8);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// lfs f0,-26300(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -26300);
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
	// lfs f0,5764(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 5764);
	ctx.f0.f64 = double(temp.f32);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// lfs f0,-27024(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -27024);
	ctx.f0.f64 = double(temp.f32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// stfs f0,20(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 20, temp.u32);
	// lfs f13,3400(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 3400);
	ctx.f13.f64 = double(temp.f32);
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// stfs f13,24(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 24, temp.u32);
	// lfs f12,-5448(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -5448);
	ctx.f12.f64 = double(temp.f32);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// stfs f12,28(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 28, temp.u32);
	// lfs f12,-27016(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -27016);
	ctx.f12.f64 = double(temp.f32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// stfs f12,32(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 32, temp.u32);
	// lfs f12,15552(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 15552);
	ctx.f12.f64 = double(temp.f32);
	// lis r10,-32244
	ctx.r10.s64 = -2113142784;
	// stfs f12,36(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 36, temp.u32);
	// lfs f12,-4864(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -4864);
	ctx.f12.f64 = double(temp.f32);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// stfs f12,40(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 40, temp.u32);
	// lfs f11,-22852(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -22852);
	ctx.f11.f64 = double(temp.f32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// stfs f11,44(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 44, temp.u32);
	// stfs f13,48(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 48, temp.u32);
	// lfs f13,3436(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 3436);
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

PPC_FUNC_IMPL(__imp__sub_82A645D0);
PPC_WEAK_FUNC(sub_82A645D0) { __imp__sub_82A645D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A645D0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r10,3
	ctx.r10.s64 = 3;
	// lfs f13,5764(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 5764);
	ctx.f13.f64 = double(temp.f32);
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r11,r11,-11384
	ctx.r11.s64 = ctx.r11.s64 + -11384;
	// stfs f13,0(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stw r10,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r10.u32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lfs f12,3400(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 3400);
	ctx.f12.f64 = double(temp.f32);
	// li r10,8
	ctx.r10.s64 = 8;
	// stfs f12,4(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stw r10,32(r11)
	PPC_STORE_U32(ctx.r11.u32 + 32, ctx.r10.u32);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// lfs f11,-22852(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -22852);
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
	// lfs f10,-5692(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -5692);
	ctx.f10.f64 = double(temp.f32);
	// li r10,1000
	ctx.r10.s64 = 1000;
	// stfs f10,12(r11)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// stw r10,40(r11)
	PPC_STORE_U32(ctx.r11.u32 + 40, ctx.r10.u32);
	// lis r10,-32254
	ctx.r10.s64 = -2113798144;
	// lfs f0,-27016(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -27016);
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

PPC_FUNC_IMPL(__imp__sub_82A647D0);
PPC_WEAK_FUNC(sub_82A647D0) { __imp__sub_82A647D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A647D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,-11040
	ctx.r3.s64 = ctx.r11.s64 + -11040;
	// bl 0x8290c330
	ctx.lr = 0x82A647E8;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4480
	ctx.r3.s64 = ctx.r11.s64 + 4480;
	// bl 0x829ffa48
	ctx.lr = 0x82A647F4;
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

PPC_FUNC_IMPL(__imp__sub_82A64808);
PPC_WEAK_FUNC(sub_82A64808) { __imp__sub_82A64808(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64808) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,-11000
	ctx.r3.s64 = ctx.r11.s64 + -11000;
	// bl 0x8290c330
	ctx.lr = 0x82A64820;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4472
	ctx.r3.s64 = ctx.r11.s64 + 4472;
	// bl 0x829ffa48
	ctx.lr = 0x82A6482C;
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

PPC_FUNC_IMPL(__imp__sub_82A64840);
PPC_WEAK_FUNC(sub_82A64840) { __imp__sub_82A64840(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64840) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,-10960
	ctx.r3.s64 = ctx.r11.s64 + -10960;
	// bl 0x8290c330
	ctx.lr = 0x82A64858;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4464
	ctx.r3.s64 = ctx.r11.s64 + 4464;
	// bl 0x829ffa48
	ctx.lr = 0x82A64864;
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

PPC_FUNC_IMPL(__imp__sub_82A64878);
PPC_WEAK_FUNC(sub_82A64878) { __imp__sub_82A64878(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64878) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,-8768
	ctx.r3.s64 = ctx.r11.s64 + -8768;
	// bl 0x8262a140
	ctx.lr = 0x82A64890;
	sub_8262A140(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4504
	ctx.r3.s64 = ctx.r11.s64 + 4504;
	// bl 0x829ffa48
	ctx.lr = 0x82A6489C;
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

PPC_FUNC_IMPL(__imp__sub_82A648B0);
PPC_WEAK_FUNC(sub_82A648B0) { __imp__sub_82A648B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A648B0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,6240
	ctx.r3.s64 = ctx.r11.s64 + 6240;
	// bl 0x826bdd68
	ctx.lr = 0x82A648C8;
	sub_826BDD68(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4488
	ctx.r3.s64 = ctx.r11.s64 + 4488;
	// bl 0x829ffa48
	ctx.lr = 0x82A648D4;
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

PPC_FUNC_IMPL(__imp__sub_82A648E8);
PPC_WEAK_FUNC(sub_82A648E8) { __imp__sub_82A648E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A648E8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,504
	ctx.r3.s64 = ctx.r11.s64 + 504;
	// bl 0x8284e060
	ctx.lr = 0x82A64904;
	sub_8284E060(ctx, base);
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// stw r3,-8776(r11)
	PPC_STORE_U32(ctx.r11.u32 + -8776, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A64920);
PPC_WEAK_FUNC(sub_82A64920) { __imp__sub_82A64920(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64920) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,536
	ctx.r3.s64 = ctx.r11.s64 + 536;
	// bl 0x8284e060
	ctx.lr = 0x82A6493C;
	sub_8284E060(ctx, base);
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// stw r3,-8772(r11)
	PPC_STORE_U32(ctx.r11.u32 + -8772, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A64958);
PPC_WEAK_FUNC(sub_82A64958) { __imp__sub_82A64958(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64958) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,560
	ctx.r3.s64 = ctx.r11.s64 + 560;
	// bl 0x8284e060
	ctx.lr = 0x82A64974;
	sub_8284E060(ctx, base);
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// stw r3,6256(r11)
	PPC_STORE_U32(ctx.r11.u32 + 6256, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A64990);
PPC_WEAK_FUNC(sub_82A64990) { __imp__sub_82A64990(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64990) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,6792
	ctx.r3.s64 = ctx.r11.s64 + 6792;
	// bl 0x8290c330
	ctx.lr = 0x82A649A8;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4576
	ctx.r3.s64 = ctx.r11.s64 + 4576;
	// bl 0x829ffa48
	ctx.lr = 0x82A649B4;
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

PPC_FUNC_IMPL(__imp__sub_82A649C8);
PPC_WEAK_FUNC(sub_82A649C8) { __imp__sub_82A649C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A649C8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,6832
	ctx.r3.s64 = ctx.r11.s64 + 6832;
	// bl 0x8290c330
	ctx.lr = 0x82A649E0;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4584
	ctx.r3.s64 = ctx.r11.s64 + 4584;
	// bl 0x829ffa48
	ctx.lr = 0x82A649EC;
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

PPC_FUNC_IMPL(__imp__sub_82A64A00);
PPC_WEAK_FUNC(sub_82A64A00) { __imp__sub_82A64A00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64A00) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,6872
	ctx.r3.s64 = ctx.r11.s64 + 6872;
	// bl 0x8290c330
	ctx.lr = 0x82A64A18;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4592
	ctx.r3.s64 = ctx.r11.s64 + 4592;
	// bl 0x829ffa48
	ctx.lr = 0x82A64A24;
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

PPC_FUNC_IMPL(__imp__sub_82A64A38);
PPC_WEAK_FUNC(sub_82A64A38) { __imp__sub_82A64A38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64A38) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,6912
	ctx.r3.s64 = ctx.r11.s64 + 6912;
	// bl 0x8290c330
	ctx.lr = 0x82A64A50;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4600
	ctx.r3.s64 = ctx.r11.s64 + 4600;
	// bl 0x829ffa48
	ctx.lr = 0x82A64A5C;
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

PPC_FUNC_IMPL(__imp__sub_82A64A70);
PPC_WEAK_FUNC(sub_82A64A70) { __imp__sub_82A64A70(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64A70) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,6952
	ctx.r3.s64 = ctx.r11.s64 + 6952;
	// bl 0x8290c330
	ctx.lr = 0x82A64A88;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4608
	ctx.r3.s64 = ctx.r11.s64 + 4608;
	// bl 0x829ffa48
	ctx.lr = 0x82A64A94;
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

PPC_FUNC_IMPL(__imp__sub_82A64AA8);
PPC_WEAK_FUNC(sub_82A64AA8) { __imp__sub_82A64AA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64AA8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,6992
	ctx.r3.s64 = ctx.r11.s64 + 6992;
	// bl 0x8290c330
	ctx.lr = 0x82A64AC0;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4616
	ctx.r3.s64 = ctx.r11.s64 + 4616;
	// bl 0x829ffa48
	ctx.lr = 0x82A64ACC;
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

PPC_FUNC_IMPL(__imp__sub_82A64AE0);
PPC_WEAK_FUNC(sub_82A64AE0) { __imp__sub_82A64AE0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64AE0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,7032
	ctx.r3.s64 = ctx.r11.s64 + 7032;
	// bl 0x8290c330
	ctx.lr = 0x82A64AF8;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4624
	ctx.r3.s64 = ctx.r11.s64 + 4624;
	// bl 0x829ffa48
	ctx.lr = 0x82A64B04;
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

PPC_FUNC_IMPL(__imp__sub_82A64B18);
PPC_WEAK_FUNC(sub_82A64B18) { __imp__sub_82A64B18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64B18) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,7072
	ctx.r3.s64 = ctx.r11.s64 + 7072;
	// bl 0x8290c330
	ctx.lr = 0x82A64B30;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4656
	ctx.r3.s64 = ctx.r11.s64 + 4656;
	// bl 0x829ffa48
	ctx.lr = 0x82A64B3C;
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

PPC_FUNC_IMPL(__imp__sub_82A64B50);
PPC_WEAK_FUNC(sub_82A64B50) { __imp__sub_82A64B50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64B50) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,7112
	ctx.r3.s64 = ctx.r11.s64 + 7112;
	// bl 0x8290c330
	ctx.lr = 0x82A64B68;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4648
	ctx.r3.s64 = ctx.r11.s64 + 4648;
	// bl 0x829ffa48
	ctx.lr = 0x82A64B74;
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

PPC_FUNC_IMPL(__imp__sub_82A64B88);
PPC_WEAK_FUNC(sub_82A64B88) { __imp__sub_82A64B88(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64B88) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,7152
	ctx.r3.s64 = ctx.r11.s64 + 7152;
	// bl 0x8290c330
	ctx.lr = 0x82A64BA0;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4640
	ctx.r3.s64 = ctx.r11.s64 + 4640;
	// bl 0x829ffa48
	ctx.lr = 0x82A64BAC;
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

PPC_FUNC_IMPL(__imp__sub_82A64BC0);
PPC_WEAK_FUNC(sub_82A64BC0) { __imp__sub_82A64BC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64BC0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,7192
	ctx.r3.s64 = ctx.r11.s64 + 7192;
	// bl 0x8290c330
	ctx.lr = 0x82A64BD8;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4632
	ctx.r3.s64 = ctx.r11.s64 + 4632;
	// bl 0x829ffa48
	ctx.lr = 0x82A64BE4;
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

PPC_FUNC_IMPL(__imp__sub_82A64BF8);
PPC_WEAK_FUNC(sub_82A64BF8) { __imp__sub_82A64BF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64BF8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,7232
	ctx.r3.s64 = ctx.r11.s64 + 7232;
	// bl 0x8290c330
	ctx.lr = 0x82A64C10;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4664
	ctx.r3.s64 = ctx.r11.s64 + 4664;
	// bl 0x829ffa48
	ctx.lr = 0x82A64C1C;
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

PPC_FUNC_IMPL(__imp__sub_82A64C30);
PPC_WEAK_FUNC(sub_82A64C30) { __imp__sub_82A64C30(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64C30) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,7272
	ctx.r3.s64 = ctx.r11.s64 + 7272;
	// bl 0x8290c330
	ctx.lr = 0x82A64C48;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4680
	ctx.r3.s64 = ctx.r11.s64 + 4680;
	// bl 0x829ffa48
	ctx.lr = 0x82A64C54;
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

PPC_FUNC_IMPL(__imp__sub_82A64C68);
PPC_WEAK_FUNC(sub_82A64C68) { __imp__sub_82A64C68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64C68) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,7312
	ctx.r3.s64 = ctx.r11.s64 + 7312;
	// bl 0x8290c330
	ctx.lr = 0x82A64C80;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4672
	ctx.r3.s64 = ctx.r11.s64 + 4672;
	// bl 0x829ffa48
	ctx.lr = 0x82A64C8C;
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

PPC_FUNC_IMPL(__imp__sub_82A64CA0);
PPC_WEAK_FUNC(sub_82A64CA0) { __imp__sub_82A64CA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64CA0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,7352
	ctx.r3.s64 = ctx.r11.s64 + 7352;
	// bl 0x8290c330
	ctx.lr = 0x82A64CB8;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4712
	ctx.r3.s64 = ctx.r11.s64 + 4712;
	// bl 0x829ffa48
	ctx.lr = 0x82A64CC4;
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

PPC_FUNC_IMPL(__imp__sub_82A64CD8);
PPC_WEAK_FUNC(sub_82A64CD8) { __imp__sub_82A64CD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64CD8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,7392
	ctx.r3.s64 = ctx.r11.s64 + 7392;
	// bl 0x8290c330
	ctx.lr = 0x82A64CF0;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4704
	ctx.r3.s64 = ctx.r11.s64 + 4704;
	// bl 0x829ffa48
	ctx.lr = 0x82A64CFC;
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

PPC_FUNC_IMPL(__imp__sub_82A64D10);
PPC_WEAK_FUNC(sub_82A64D10) { __imp__sub_82A64D10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64D10) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,7432
	ctx.r3.s64 = ctx.r11.s64 + 7432;
	// bl 0x8290c330
	ctx.lr = 0x82A64D28;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4696
	ctx.r3.s64 = ctx.r11.s64 + 4696;
	// bl 0x829ffa48
	ctx.lr = 0x82A64D34;
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

PPC_FUNC_IMPL(__imp__sub_82A64D48);
PPC_WEAK_FUNC(sub_82A64D48) { __imp__sub_82A64D48(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64D48) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,7472
	ctx.r3.s64 = ctx.r11.s64 + 7472;
	// bl 0x8290c330
	ctx.lr = 0x82A64D60;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4688
	ctx.r3.s64 = ctx.r11.s64 + 4688;
	// bl 0x829ffa48
	ctx.lr = 0x82A64D6C;
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

PPC_FUNC_IMPL(__imp__sub_82A64D80);
PPC_WEAK_FUNC(sub_82A64D80) { __imp__sub_82A64D80(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64D80) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,7512
	ctx.r3.s64 = ctx.r11.s64 + 7512;
	// bl 0x8290c330
	ctx.lr = 0x82A64D98;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4720
	ctx.r3.s64 = ctx.r11.s64 + 4720;
	// bl 0x829ffa48
	ctx.lr = 0x82A64DA4;
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

PPC_FUNC_IMPL(__imp__sub_82A64DB8);
PPC_WEAK_FUNC(sub_82A64DB8) { __imp__sub_82A64DB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64DB8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,7552
	ctx.r3.s64 = ctx.r11.s64 + 7552;
	// bl 0x8290c330
	ctx.lr = 0x82A64DD0;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4744
	ctx.r3.s64 = ctx.r11.s64 + 4744;
	// bl 0x829ffa48
	ctx.lr = 0x82A64DDC;
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

PPC_FUNC_IMPL(__imp__sub_82A64DF0);
PPC_WEAK_FUNC(sub_82A64DF0) { __imp__sub_82A64DF0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64DF0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,7592
	ctx.r3.s64 = ctx.r11.s64 + 7592;
	// bl 0x8290c330
	ctx.lr = 0x82A64E08;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4736
	ctx.r3.s64 = ctx.r11.s64 + 4736;
	// bl 0x829ffa48
	ctx.lr = 0x82A64E14;
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

PPC_FUNC_IMPL(__imp__sub_82A64E28);
PPC_WEAK_FUNC(sub_82A64E28) { __imp__sub_82A64E28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64E28) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,7632
	ctx.r3.s64 = ctx.r11.s64 + 7632;
	// bl 0x8290c330
	ctx.lr = 0x82A64E40;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4728
	ctx.r3.s64 = ctx.r11.s64 + 4728;
	// bl 0x829ffa48
	ctx.lr = 0x82A64E4C;
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

PPC_FUNC_IMPL(__imp__sub_82A64E60);
PPC_WEAK_FUNC(sub_82A64E60) { __imp__sub_82A64E60(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64E60) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,7672
	ctx.r3.s64 = ctx.r11.s64 + 7672;
	// bl 0x8290c330
	ctx.lr = 0x82A64E78;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4760
	ctx.r3.s64 = ctx.r11.s64 + 4760;
	// bl 0x829ffa48
	ctx.lr = 0x82A64E84;
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

PPC_FUNC_IMPL(__imp__sub_82A64E98);
PPC_WEAK_FUNC(sub_82A64E98) { __imp__sub_82A64E98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64E98) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,7712
	ctx.r3.s64 = ctx.r11.s64 + 7712;
	// bl 0x8290c330
	ctx.lr = 0x82A64EB0;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4752
	ctx.r3.s64 = ctx.r11.s64 + 4752;
	// bl 0x829ffa48
	ctx.lr = 0x82A64EBC;
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

PPC_FUNC_IMPL(__imp__sub_82A64ED0);
PPC_WEAK_FUNC(sub_82A64ED0) { __imp__sub_82A64ED0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64ED0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,7752
	ctx.r3.s64 = ctx.r11.s64 + 7752;
	// bl 0x8290c330
	ctx.lr = 0x82A64EE8;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4776
	ctx.r3.s64 = ctx.r11.s64 + 4776;
	// bl 0x829ffa48
	ctx.lr = 0x82A64EF4;
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

PPC_FUNC_IMPL(__imp__sub_82A64F08);
PPC_WEAK_FUNC(sub_82A64F08) { __imp__sub_82A64F08(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64F08) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,7792
	ctx.r3.s64 = ctx.r11.s64 + 7792;
	// bl 0x8290c330
	ctx.lr = 0x82A64F20;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4768
	ctx.r3.s64 = ctx.r11.s64 + 4768;
	// bl 0x829ffa48
	ctx.lr = 0x82A64F2C;
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

PPC_FUNC_IMPL(__imp__sub_82A64F40);
PPC_WEAK_FUNC(sub_82A64F40) { __imp__sub_82A64F40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64F40) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,7832
	ctx.r3.s64 = ctx.r11.s64 + 7832;
	// bl 0x8290c330
	ctx.lr = 0x82A64F58;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4800
	ctx.r3.s64 = ctx.r11.s64 + 4800;
	// bl 0x829ffa48
	ctx.lr = 0x82A64F64;
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

PPC_FUNC_IMPL(__imp__sub_82A64F78);
PPC_WEAK_FUNC(sub_82A64F78) { __imp__sub_82A64F78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64F78) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,7872
	ctx.r3.s64 = ctx.r11.s64 + 7872;
	// bl 0x8290c330
	ctx.lr = 0x82A64F90;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4792
	ctx.r3.s64 = ctx.r11.s64 + 4792;
	// bl 0x829ffa48
	ctx.lr = 0x82A64F9C;
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

PPC_FUNC_IMPL(__imp__sub_82A64FB0);
PPC_WEAK_FUNC(sub_82A64FB0) { __imp__sub_82A64FB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64FB0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,7912
	ctx.r3.s64 = ctx.r11.s64 + 7912;
	// bl 0x8290c330
	ctx.lr = 0x82A64FC8;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4784
	ctx.r3.s64 = ctx.r11.s64 + 4784;
	// bl 0x829ffa48
	ctx.lr = 0x82A64FD4;
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

PPC_FUNC_IMPL(__imp__sub_82A64FE8);
PPC_WEAK_FUNC(sub_82A64FE8) { __imp__sub_82A64FE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A64FE8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,7952
	ctx.r3.s64 = ctx.r11.s64 + 7952;
	// bl 0x8290c330
	ctx.lr = 0x82A65000;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4816
	ctx.r3.s64 = ctx.r11.s64 + 4816;
	// bl 0x829ffa48
	ctx.lr = 0x82A6500C;
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

PPC_FUNC_IMPL(__imp__sub_82A65020);
PPC_WEAK_FUNC(sub_82A65020) { __imp__sub_82A65020(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65020) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,7992
	ctx.r3.s64 = ctx.r11.s64 + 7992;
	// bl 0x8290c330
	ctx.lr = 0x82A65038;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4808
	ctx.r3.s64 = ctx.r11.s64 + 4808;
	// bl 0x829ffa48
	ctx.lr = 0x82A65044;
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

PPC_FUNC_IMPL(__imp__sub_82A65058);
PPC_WEAK_FUNC(sub_82A65058) { __imp__sub_82A65058(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65058) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,8032
	ctx.r3.s64 = ctx.r11.s64 + 8032;
	// bl 0x8290c330
	ctx.lr = 0x82A65070;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4840
	ctx.r3.s64 = ctx.r11.s64 + 4840;
	// bl 0x829ffa48
	ctx.lr = 0x82A6507C;
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

PPC_FUNC_IMPL(__imp__sub_82A65090);
PPC_WEAK_FUNC(sub_82A65090) { __imp__sub_82A65090(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65090) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,8072
	ctx.r3.s64 = ctx.r11.s64 + 8072;
	// bl 0x8290c330
	ctx.lr = 0x82A650A8;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4832
	ctx.r3.s64 = ctx.r11.s64 + 4832;
	// bl 0x829ffa48
	ctx.lr = 0x82A650B4;
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

PPC_FUNC_IMPL(__imp__sub_82A650C8);
PPC_WEAK_FUNC(sub_82A650C8) { __imp__sub_82A650C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A650C8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,8112
	ctx.r3.s64 = ctx.r11.s64 + 8112;
	// bl 0x8290c330
	ctx.lr = 0x82A650E0;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4824
	ctx.r3.s64 = ctx.r11.s64 + 4824;
	// bl 0x829ffa48
	ctx.lr = 0x82A650EC;
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

PPC_FUNC_IMPL(__imp__sub_82A65100);
PPC_WEAK_FUNC(sub_82A65100) { __imp__sub_82A65100(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65100) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,8152
	ctx.r3.s64 = ctx.r11.s64 + 8152;
	// bl 0x8290c330
	ctx.lr = 0x82A65118;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4848
	ctx.r3.s64 = ctx.r11.s64 + 4848;
	// bl 0x829ffa48
	ctx.lr = 0x82A65124;
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

PPC_FUNC_IMPL(__imp__sub_82A65138);
PPC_WEAK_FUNC(sub_82A65138) { __imp__sub_82A65138(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65138) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,8192
	ctx.r3.s64 = ctx.r11.s64 + 8192;
	// bl 0x8290c330
	ctx.lr = 0x82A65150;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4856
	ctx.r3.s64 = ctx.r11.s64 + 4856;
	// bl 0x829ffa48
	ctx.lr = 0x82A6515C;
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

PPC_FUNC_IMPL(__imp__sub_82A65170);
PPC_WEAK_FUNC(sub_82A65170) { __imp__sub_82A65170(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65170) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,8232
	ctx.r3.s64 = ctx.r11.s64 + 8232;
	// bl 0x8290c330
	ctx.lr = 0x82A65188;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4864
	ctx.r3.s64 = ctx.r11.s64 + 4864;
	// bl 0x829ffa48
	ctx.lr = 0x82A65194;
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

PPC_FUNC_IMPL(__imp__sub_82A651A8);
PPC_WEAK_FUNC(sub_82A651A8) { __imp__sub_82A651A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A651A8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,8272
	ctx.r3.s64 = ctx.r11.s64 + 8272;
	// bl 0x8290c330
	ctx.lr = 0x82A651C0;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4872
	ctx.r3.s64 = ctx.r11.s64 + 4872;
	// bl 0x829ffa48
	ctx.lr = 0x82A651CC;
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

PPC_FUNC_IMPL(__imp__sub_82A651E0);
PPC_WEAK_FUNC(sub_82A651E0) { __imp__sub_82A651E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A651E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,8312
	ctx.r3.s64 = ctx.r11.s64 + 8312;
	// bl 0x8290c330
	ctx.lr = 0x82A651F8;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4880
	ctx.r3.s64 = ctx.r11.s64 + 4880;
	// bl 0x829ffa48
	ctx.lr = 0x82A65204;
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

PPC_FUNC_IMPL(__imp__sub_82A65218);
PPC_WEAK_FUNC(sub_82A65218) { __imp__sub_82A65218(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65218) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,8352
	ctx.r3.s64 = ctx.r11.s64 + 8352;
	// bl 0x8290c330
	ctx.lr = 0x82A65230;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4888
	ctx.r3.s64 = ctx.r11.s64 + 4888;
	// bl 0x829ffa48
	ctx.lr = 0x82A6523C;
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

PPC_FUNC_IMPL(__imp__sub_82A65250);
PPC_WEAK_FUNC(sub_82A65250) { __imp__sub_82A65250(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65250) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,8392
	ctx.r3.s64 = ctx.r11.s64 + 8392;
	// bl 0x8290c330
	ctx.lr = 0x82A65268;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4904
	ctx.r3.s64 = ctx.r11.s64 + 4904;
	// bl 0x829ffa48
	ctx.lr = 0x82A65274;
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

PPC_FUNC_IMPL(__imp__sub_82A65288);
PPC_WEAK_FUNC(sub_82A65288) { __imp__sub_82A65288(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65288) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,8432
	ctx.r3.s64 = ctx.r11.s64 + 8432;
	// bl 0x8290c330
	ctx.lr = 0x82A652A0;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4896
	ctx.r3.s64 = ctx.r11.s64 + 4896;
	// bl 0x829ffa48
	ctx.lr = 0x82A652AC;
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

PPC_FUNC_IMPL(__imp__sub_82A652C0);
PPC_WEAK_FUNC(sub_82A652C0) { __imp__sub_82A652C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A652C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,8472
	ctx.r3.s64 = ctx.r11.s64 + 8472;
	// bl 0x8290c330
	ctx.lr = 0x82A652D8;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4912
	ctx.r3.s64 = ctx.r11.s64 + 4912;
	// bl 0x829ffa48
	ctx.lr = 0x82A652E4;
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

PPC_FUNC_IMPL(__imp__sub_82A652F8);
PPC_WEAK_FUNC(sub_82A652F8) { __imp__sub_82A652F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A652F8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,8512
	ctx.r3.s64 = ctx.r11.s64 + 8512;
	// bl 0x8290c330
	ctx.lr = 0x82A65310;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4920
	ctx.r3.s64 = ctx.r11.s64 + 4920;
	// bl 0x829ffa48
	ctx.lr = 0x82A6531C;
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

PPC_FUNC_IMPL(__imp__sub_82A65330);
PPC_WEAK_FUNC(sub_82A65330) { __imp__sub_82A65330(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65330) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,8552
	ctx.r3.s64 = ctx.r11.s64 + 8552;
	// bl 0x8290c330
	ctx.lr = 0x82A65348;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4928
	ctx.r3.s64 = ctx.r11.s64 + 4928;
	// bl 0x829ffa48
	ctx.lr = 0x82A65354;
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

PPC_FUNC_IMPL(__imp__sub_82A65368);
PPC_WEAK_FUNC(sub_82A65368) { __imp__sub_82A65368(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65368) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,8592
	ctx.r3.s64 = ctx.r11.s64 + 8592;
	// bl 0x8290c330
	ctx.lr = 0x82A65380;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4944
	ctx.r3.s64 = ctx.r11.s64 + 4944;
	// bl 0x829ffa48
	ctx.lr = 0x82A6538C;
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

PPC_FUNC_IMPL(__imp__sub_82A653A0);
PPC_WEAK_FUNC(sub_82A653A0) { __imp__sub_82A653A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A653A0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,8632
	ctx.r3.s64 = ctx.r11.s64 + 8632;
	// bl 0x8290c330
	ctx.lr = 0x82A653B8;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4936
	ctx.r3.s64 = ctx.r11.s64 + 4936;
	// bl 0x829ffa48
	ctx.lr = 0x82A653C4;
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

PPC_FUNC_IMPL(__imp__sub_82A653D8);
PPC_WEAK_FUNC(sub_82A653D8) { __imp__sub_82A653D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A653D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,8672
	ctx.r3.s64 = ctx.r11.s64 + 8672;
	// bl 0x8290c330
	ctx.lr = 0x82A653F0;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4960
	ctx.r3.s64 = ctx.r11.s64 + 4960;
	// bl 0x829ffa48
	ctx.lr = 0x82A653FC;
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

PPC_FUNC_IMPL(__imp__sub_82A65410);
PPC_WEAK_FUNC(sub_82A65410) { __imp__sub_82A65410(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65410) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,8712
	ctx.r3.s64 = ctx.r11.s64 + 8712;
	// bl 0x8290c330
	ctx.lr = 0x82A65428;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4952
	ctx.r3.s64 = ctx.r11.s64 + 4952;
	// bl 0x829ffa48
	ctx.lr = 0x82A65434;
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

PPC_FUNC_IMPL(__imp__sub_82A65448);
PPC_WEAK_FUNC(sub_82A65448) { __imp__sub_82A65448(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65448) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,8752
	ctx.r3.s64 = ctx.r11.s64 + 8752;
	// bl 0x8290c330
	ctx.lr = 0x82A65460;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4992
	ctx.r3.s64 = ctx.r11.s64 + 4992;
	// bl 0x829ffa48
	ctx.lr = 0x82A6546C;
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

PPC_FUNC_IMPL(__imp__sub_82A65480);
PPC_WEAK_FUNC(sub_82A65480) { __imp__sub_82A65480(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65480) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,8792
	ctx.r3.s64 = ctx.r11.s64 + 8792;
	// bl 0x8290c330
	ctx.lr = 0x82A65498;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4984
	ctx.r3.s64 = ctx.r11.s64 + 4984;
	// bl 0x829ffa48
	ctx.lr = 0x82A654A4;
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

PPC_FUNC_IMPL(__imp__sub_82A654B8);
PPC_WEAK_FUNC(sub_82A654B8) { __imp__sub_82A654B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A654B8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,8832
	ctx.r3.s64 = ctx.r11.s64 + 8832;
	// bl 0x8290c330
	ctx.lr = 0x82A654D0;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4976
	ctx.r3.s64 = ctx.r11.s64 + 4976;
	// bl 0x829ffa48
	ctx.lr = 0x82A654DC;
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

PPC_FUNC_IMPL(__imp__sub_82A654F0);
PPC_WEAK_FUNC(sub_82A654F0) { __imp__sub_82A654F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A654F0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,8872
	ctx.r3.s64 = ctx.r11.s64 + 8872;
	// bl 0x8290c330
	ctx.lr = 0x82A65508;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,4968
	ctx.r3.s64 = ctx.r11.s64 + 4968;
	// bl 0x829ffa48
	ctx.lr = 0x82A65514;
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

PPC_FUNC_IMPL(__imp__sub_82A65528);
PPC_WEAK_FUNC(sub_82A65528) { __imp__sub_82A65528(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65528) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,8912
	ctx.r3.s64 = ctx.r11.s64 + 8912;
	// bl 0x8290c330
	ctx.lr = 0x82A65540;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5024
	ctx.r3.s64 = ctx.r11.s64 + 5024;
	// bl 0x829ffa48
	ctx.lr = 0x82A6554C;
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

PPC_FUNC_IMPL(__imp__sub_82A65560);
PPC_WEAK_FUNC(sub_82A65560) { __imp__sub_82A65560(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65560) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,8952
	ctx.r3.s64 = ctx.r11.s64 + 8952;
	// bl 0x8290c330
	ctx.lr = 0x82A65578;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5016
	ctx.r3.s64 = ctx.r11.s64 + 5016;
	// bl 0x829ffa48
	ctx.lr = 0x82A65584;
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

PPC_FUNC_IMPL(__imp__sub_82A65598);
PPC_WEAK_FUNC(sub_82A65598) { __imp__sub_82A65598(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65598) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,8992
	ctx.r3.s64 = ctx.r11.s64 + 8992;
	// bl 0x8290c330
	ctx.lr = 0x82A655B0;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5008
	ctx.r3.s64 = ctx.r11.s64 + 5008;
	// bl 0x829ffa48
	ctx.lr = 0x82A655BC;
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

PPC_FUNC_IMPL(__imp__sub_82A655D0);
PPC_WEAK_FUNC(sub_82A655D0) { __imp__sub_82A655D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A655D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,9032
	ctx.r3.s64 = ctx.r11.s64 + 9032;
	// bl 0x8290c330
	ctx.lr = 0x82A655E8;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5000
	ctx.r3.s64 = ctx.r11.s64 + 5000;
	// bl 0x829ffa48
	ctx.lr = 0x82A655F4;
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

PPC_FUNC_IMPL(__imp__sub_82A65608);
PPC_WEAK_FUNC(sub_82A65608) { __imp__sub_82A65608(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65608) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,9072
	ctx.r3.s64 = ctx.r11.s64 + 9072;
	// bl 0x8290c330
	ctx.lr = 0x82A65620;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5056
	ctx.r3.s64 = ctx.r11.s64 + 5056;
	// bl 0x829ffa48
	ctx.lr = 0x82A6562C;
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

PPC_FUNC_IMPL(__imp__sub_82A65640);
PPC_WEAK_FUNC(sub_82A65640) { __imp__sub_82A65640(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65640) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,9112
	ctx.r3.s64 = ctx.r11.s64 + 9112;
	// bl 0x8290c330
	ctx.lr = 0x82A65658;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5048
	ctx.r3.s64 = ctx.r11.s64 + 5048;
	// bl 0x829ffa48
	ctx.lr = 0x82A65664;
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

PPC_FUNC_IMPL(__imp__sub_82A65678);
PPC_WEAK_FUNC(sub_82A65678) { __imp__sub_82A65678(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65678) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,9152
	ctx.r3.s64 = ctx.r11.s64 + 9152;
	// bl 0x8290c330
	ctx.lr = 0x82A65690;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5040
	ctx.r3.s64 = ctx.r11.s64 + 5040;
	// bl 0x829ffa48
	ctx.lr = 0x82A6569C;
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

PPC_FUNC_IMPL(__imp__sub_82A656B0);
PPC_WEAK_FUNC(sub_82A656B0) { __imp__sub_82A656B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A656B0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,9192
	ctx.r3.s64 = ctx.r11.s64 + 9192;
	// bl 0x8290c330
	ctx.lr = 0x82A656C8;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5032
	ctx.r3.s64 = ctx.r11.s64 + 5032;
	// bl 0x829ffa48
	ctx.lr = 0x82A656D4;
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

PPC_FUNC_IMPL(__imp__sub_82A656E8);
PPC_WEAK_FUNC(sub_82A656E8) { __imp__sub_82A656E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A656E8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,9232
	ctx.r3.s64 = ctx.r11.s64 + 9232;
	// bl 0x8290c330
	ctx.lr = 0x82A65700;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5088
	ctx.r3.s64 = ctx.r11.s64 + 5088;
	// bl 0x829ffa48
	ctx.lr = 0x82A6570C;
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

PPC_FUNC_IMPL(__imp__sub_82A65720);
PPC_WEAK_FUNC(sub_82A65720) { __imp__sub_82A65720(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65720) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,9272
	ctx.r3.s64 = ctx.r11.s64 + 9272;
	// bl 0x8290c330
	ctx.lr = 0x82A65738;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5080
	ctx.r3.s64 = ctx.r11.s64 + 5080;
	// bl 0x829ffa48
	ctx.lr = 0x82A65744;
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

PPC_FUNC_IMPL(__imp__sub_82A65758);
PPC_WEAK_FUNC(sub_82A65758) { __imp__sub_82A65758(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65758) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,9312
	ctx.r3.s64 = ctx.r11.s64 + 9312;
	// bl 0x8290c330
	ctx.lr = 0x82A65770;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5072
	ctx.r3.s64 = ctx.r11.s64 + 5072;
	// bl 0x829ffa48
	ctx.lr = 0x82A6577C;
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

PPC_FUNC_IMPL(__imp__sub_82A65790);
PPC_WEAK_FUNC(sub_82A65790) { __imp__sub_82A65790(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65790) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,9352
	ctx.r3.s64 = ctx.r11.s64 + 9352;
	// bl 0x8290c330
	ctx.lr = 0x82A657A8;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5064
	ctx.r3.s64 = ctx.r11.s64 + 5064;
	// bl 0x829ffa48
	ctx.lr = 0x82A657B4;
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

PPC_FUNC_IMPL(__imp__sub_82A657C8);
PPC_WEAK_FUNC(sub_82A657C8) { __imp__sub_82A657C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A657C8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,9392
	ctx.r3.s64 = ctx.r11.s64 + 9392;
	// bl 0x8290c330
	ctx.lr = 0x82A657E0;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5120
	ctx.r3.s64 = ctx.r11.s64 + 5120;
	// bl 0x829ffa48
	ctx.lr = 0x82A657EC;
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

PPC_FUNC_IMPL(__imp__sub_82A65800);
PPC_WEAK_FUNC(sub_82A65800) { __imp__sub_82A65800(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65800) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,9432
	ctx.r3.s64 = ctx.r11.s64 + 9432;
	// bl 0x8290c330
	ctx.lr = 0x82A65818;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5112
	ctx.r3.s64 = ctx.r11.s64 + 5112;
	// bl 0x829ffa48
	ctx.lr = 0x82A65824;
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

PPC_FUNC_IMPL(__imp__sub_82A65838);
PPC_WEAK_FUNC(sub_82A65838) { __imp__sub_82A65838(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65838) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,9472
	ctx.r3.s64 = ctx.r11.s64 + 9472;
	// bl 0x8290c330
	ctx.lr = 0x82A65850;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5104
	ctx.r3.s64 = ctx.r11.s64 + 5104;
	// bl 0x829ffa48
	ctx.lr = 0x82A6585C;
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

PPC_FUNC_IMPL(__imp__sub_82A65870);
PPC_WEAK_FUNC(sub_82A65870) { __imp__sub_82A65870(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65870) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,9512
	ctx.r3.s64 = ctx.r11.s64 + 9512;
	// bl 0x8290c330
	ctx.lr = 0x82A65888;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5096
	ctx.r3.s64 = ctx.r11.s64 + 5096;
	// bl 0x829ffa48
	ctx.lr = 0x82A65894;
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

PPC_FUNC_IMPL(__imp__sub_82A658A8);
PPC_WEAK_FUNC(sub_82A658A8) { __imp__sub_82A658A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A658A8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,9552
	ctx.r3.s64 = ctx.r11.s64 + 9552;
	// bl 0x8290c330
	ctx.lr = 0x82A658C0;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5128
	ctx.r3.s64 = ctx.r11.s64 + 5128;
	// bl 0x829ffa48
	ctx.lr = 0x82A658CC;
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

PPC_FUNC_IMPL(__imp__sub_82A658E0);
PPC_WEAK_FUNC(sub_82A658E0) { __imp__sub_82A658E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A658E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,9592
	ctx.r3.s64 = ctx.r11.s64 + 9592;
	// bl 0x8290c330
	ctx.lr = 0x82A658F8;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5136
	ctx.r3.s64 = ctx.r11.s64 + 5136;
	// bl 0x829ffa48
	ctx.lr = 0x82A65904;
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

PPC_FUNC_IMPL(__imp__sub_82A65918);
PPC_WEAK_FUNC(sub_82A65918) { __imp__sub_82A65918(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65918) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,9632
	ctx.r3.s64 = ctx.r11.s64 + 9632;
	// bl 0x8290c330
	ctx.lr = 0x82A65930;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5152
	ctx.r3.s64 = ctx.r11.s64 + 5152;
	// bl 0x829ffa48
	ctx.lr = 0x82A6593C;
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

PPC_FUNC_IMPL(__imp__sub_82A65950);
PPC_WEAK_FUNC(sub_82A65950) { __imp__sub_82A65950(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65950) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,9672
	ctx.r3.s64 = ctx.r11.s64 + 9672;
	// bl 0x8290c330
	ctx.lr = 0x82A65968;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5144
	ctx.r3.s64 = ctx.r11.s64 + 5144;
	// bl 0x829ffa48
	ctx.lr = 0x82A65974;
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

PPC_FUNC_IMPL(__imp__sub_82A65988);
PPC_WEAK_FUNC(sub_82A65988) { __imp__sub_82A65988(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65988) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,9712
	ctx.r3.s64 = ctx.r11.s64 + 9712;
	// bl 0x8290c330
	ctx.lr = 0x82A659A0;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5160
	ctx.r3.s64 = ctx.r11.s64 + 5160;
	// bl 0x829ffa48
	ctx.lr = 0x82A659AC;
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

PPC_FUNC_IMPL(__imp__sub_82A659C0);
PPC_WEAK_FUNC(sub_82A659C0) { __imp__sub_82A659C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A659C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,9752
	ctx.r3.s64 = ctx.r11.s64 + 9752;
	// bl 0x8290c330
	ctx.lr = 0x82A659D8;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5168
	ctx.r3.s64 = ctx.r11.s64 + 5168;
	// bl 0x829ffa48
	ctx.lr = 0x82A659E4;
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

PPC_FUNC_IMPL(__imp__sub_82A659F8);
PPC_WEAK_FUNC(sub_82A659F8) { __imp__sub_82A659F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A659F8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,9792
	ctx.r3.s64 = ctx.r11.s64 + 9792;
	// bl 0x8290c330
	ctx.lr = 0x82A65A10;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5176
	ctx.r3.s64 = ctx.r11.s64 + 5176;
	// bl 0x829ffa48
	ctx.lr = 0x82A65A1C;
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

PPC_FUNC_IMPL(__imp__sub_82A65A30);
PPC_WEAK_FUNC(sub_82A65A30) { __imp__sub_82A65A30(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65A30) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,1648
	ctx.r3.s64 = ctx.r11.s64 + 1648;
	// bl 0x8284e060
	ctx.lr = 0x82A65A4C;
	sub_8284E060(ctx, base);
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// stw r3,9832(r11)
	PPC_STORE_U32(ctx.r11.u32 + 9832, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A65A68);
PPC_WEAK_FUNC(sub_82A65A68) { __imp__sub_82A65A68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65A68) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,6952
	ctx.r3.s64 = ctx.r11.s64 + 6952;
	// bl 0x8284e060
	ctx.lr = 0x82A65A84;
	sub_8284E060(ctx, base);
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// stw r3,9836(r11)
	PPC_STORE_U32(ctx.r11.u32 + 9836, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A65AA0);
PPC_WEAK_FUNC(sub_82A65AA0) { __imp__sub_82A65AA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65AA0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,6992
	ctx.r3.s64 = ctx.r11.s64 + 6992;
	// bl 0x8284e060
	ctx.lr = 0x82A65ABC;
	sub_8284E060(ctx, base);
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// stw r3,9840(r11)
	PPC_STORE_U32(ctx.r11.u32 + 9840, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A65AD8);
PPC_WEAK_FUNC(sub_82A65AD8) { __imp__sub_82A65AD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65AD8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,7028
	ctx.r3.s64 = ctx.r11.s64 + 7028;
	// bl 0x8284e060
	ctx.lr = 0x82A65AF4;
	sub_8284E060(ctx, base);
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// stw r3,9844(r11)
	PPC_STORE_U32(ctx.r11.u32 + 9844, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A65B10);
PPC_WEAK_FUNC(sub_82A65B10) { __imp__sub_82A65B10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65B10) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,7064
	ctx.r3.s64 = ctx.r11.s64 + 7064;
	// bl 0x8284e060
	ctx.lr = 0x82A65B2C;
	sub_8284E060(ctx, base);
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// stw r3,9848(r11)
	PPC_STORE_U32(ctx.r11.u32 + 9848, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A65B48);
PPC_WEAK_FUNC(sub_82A65B48) { __imp__sub_82A65B48(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65B48) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,7092
	ctx.r3.s64 = ctx.r11.s64 + 7092;
	// bl 0x8284e060
	ctx.lr = 0x82A65B64;
	sub_8284E060(ctx, base);
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// stw r3,9852(r11)
	PPC_STORE_U32(ctx.r11.u32 + 9852, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A65B80);
PPC_WEAK_FUNC(sub_82A65B80) { __imp__sub_82A65B80(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65B80) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-224
	ctx.r3.s64 = ctx.r11.s64 + -224;
	// bl 0x8284e060
	ctx.lr = 0x82A65B9C;
	sub_8284E060(ctx, base);
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// stw r3,9856(r11)
	PPC_STORE_U32(ctx.r11.u32 + 9856, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A65BB8);
PPC_WEAK_FUNC(sub_82A65BB8) { __imp__sub_82A65BB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65BB8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,7104
	ctx.r3.s64 = ctx.r11.s64 + 7104;
	// bl 0x8284e060
	ctx.lr = 0x82A65BD4;
	sub_8284E060(ctx, base);
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// stw r3,9860(r11)
	PPC_STORE_U32(ctx.r11.u32 + 9860, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A65BF0);
PPC_WEAK_FUNC(sub_82A65BF0) { __imp__sub_82A65BF0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65BF0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r3,r11,7392
	ctx.r3.s64 = ctx.r11.s64 + 7392;
	// bl 0x8284e060
	ctx.lr = 0x82A65C10;
	sub_8284E060(ctx, base);
	// lis r10,-31990
	ctx.r10.s64 = -2096496640;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,10104
	ctx.r31.s64 = ctx.r10.s64 + 10104;
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// addi r11,r11,7372
	ctx.r11.s64 = ctx.r11.s64 + 7372;
	// addi r10,r10,7336
	ctx.r10.s64 = ctx.r10.s64 + 7336;
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// stw r10,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r10.u32);
	// bl 0x8284e060
	ctx.lr = 0x82A65C3C;
	sub_8284E060(ctx, base);
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// stw r3,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r3.u32);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r10,r10,7308
	ctx.r10.s64 = ctx.r10.s64 + 7308;
	// addi r11,r11,7288
	ctx.r11.s64 = ctx.r11.s64 + 7288;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// stw r10,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r10.u32);
	// bl 0x8284e060
	ctx.lr = 0x82A65C60;
	sub_8284E060(ctx, base);
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// stw r3,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r3.u32);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r10,r10,7260
	ctx.r10.s64 = ctx.r10.s64 + 7260;
	// addi r11,r11,7240
	ctx.r11.s64 = ctx.r11.s64 + 7240;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// stw r10,20(r31)
	PPC_STORE_U32(ctx.r31.u32 + 20, ctx.r10.u32);
	// bl 0x8284e060
	ctx.lr = 0x82A65C84;
	sub_8284E060(ctx, base);
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// stw r3,24(r31)
	PPC_STORE_U32(ctx.r31.u32 + 24, ctx.r3.u32);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r10,r10,7212
	ctx.r10.s64 = ctx.r10.s64 + 7212;
	// addi r11,r11,7192
	ctx.r11.s64 = ctx.r11.s64 + 7192;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// stw r10,28(r31)
	PPC_STORE_U32(ctx.r31.u32 + 28, ctx.r10.u32);
	// bl 0x8284e060
	ctx.lr = 0x82A65CA8;
	sub_8284E060(ctx, base);
	// lis r10,-32252
	ctx.r10.s64 = -2113667072;
	// stw r3,32(r31)
	PPC_STORE_U32(ctx.r31.u32 + 32, ctx.r3.u32);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r10,r10,7164
	ctx.r10.s64 = ctx.r10.s64 + 7164;
	// addi r11,r11,7144
	ctx.r11.s64 = ctx.r11.s64 + 7144;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// stw r10,36(r31)
	PPC_STORE_U32(ctx.r31.u32 + 36, ctx.r10.u32);
	// bl 0x8284e060
	ctx.lr = 0x82A65CCC;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r3.u32);
	// addi r11,r11,7116
	ctx.r11.s64 = ctx.r11.s64 + 7116;
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

PPC_FUNC_IMPL(__imp__sub_82A65CF0);
PPC_WEAK_FUNC(sub_82A65CF0) { __imp__sub_82A65CF0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65CF0) {
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
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// li r31,5
	ctx.r31.s64 = 5;
	// addi r30,r11,9864
	ctx.r30.s64 = ctx.r11.s64 + 9864;
loc_82A65D10:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8290c330
	ctx.lr = 0x82A65D18;
	sub_8290C330(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,40
	ctx.r30.s64 = ctx.r30.s64 + 40;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x82a65d10
	if (!ctx.cr6.lt) goto loc_82A65D10;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5184
	ctx.r3.s64 = ctx.r11.s64 + 5184;
	// bl 0x829ffa48
	ctx.lr = 0x82A65D34;
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

PPC_FUNC_IMPL(__imp__sub_82A65D50);
PPC_WEAK_FUNC(sub_82A65D50) { __imp__sub_82A65D50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65D50) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5192
	ctx.r3.s64 = ctx.r11.s64 + 5192;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A65D60);
PPC_WEAK_FUNC(sub_82A65D60) { __imp__sub_82A65D60(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65D60) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// li r10,31
	ctx.r10.s64 = 31;
	// addi r11,r11,15616
	ctx.r11.s64 = ctx.r11.s64 + 15616;
	// li r8,-1
	ctx.r8.s64 = -1;
	// addi r9,r11,96
	ctx.r9.s64 = ctx.r11.s64 + 96;
loc_82A65D74:
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
	// stw r8,0(r9)
	PPC_STORE_U32(ctx.r9.u32 + 0, ctx.r8.u32);
	// addi r9,r9,112
	ctx.r9.s64 = ctx.r9.s64 + 112;
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// bge cr6,0x82a65d74
	if (!ctx.cr6.lt) goto loc_82A65D74;
	// li r10,0
	ctx.r10.s64 = 0;
	// lis r9,-32089
	ctx.r9.s64 = -2102984704;
	// addi r3,r9,5224
	ctx.r3.s64 = ctx.r9.s64 + 5224;
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
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A65DB0);
PPC_WEAK_FUNC(sub_82A65DB0) { __imp__sub_82A65DB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65DB0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31990
	ctx.r11.s64 = -2096496640;
	// addi r3,r11,19408
	ctx.r3.s64 = ctx.r11.s64 + 19408;
	// bl 0x82651cb0
	ctx.lr = 0x82A65DC8;
	sub_82651CB0(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5232
	ctx.r3.s64 = ctx.r11.s64 + 5232;
	// bl 0x829ffa48
	ctx.lr = 0x82A65DD4;
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

PPC_FUNC_IMPL(__imp__sub_82A65DE8);
PPC_WEAK_FUNC(sub_82A65DE8) { __imp__sub_82A65DE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65DE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5312
	ctx.r3.s64 = ctx.r11.s64 + 5312;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A65DF8);
PPC_WEAK_FUNC(sub_82A65DF8) { __imp__sub_82A65DF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65DF8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,10276
	ctx.r3.s64 = ctx.r11.s64 + 10276;
	// bl 0x8284e060
	ctx.lr = 0x82A65E14;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,24336(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24336, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A65E30);
PPC_WEAK_FUNC(sub_82A65E30) { __imp__sub_82A65E30(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65E30) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,10292
	ctx.r3.s64 = ctx.r11.s64 + 10292;
	// bl 0x8284e060
	ctx.lr = 0x82A65E4C;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,24340(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24340, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A65E68);
PPC_WEAK_FUNC(sub_82A65E68) { __imp__sub_82A65E68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65E68) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// addi r31,r11,24344
	ctx.r31.s64 = ctx.r11.s64 + 24344;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82902b70
	ctx.lr = 0x82A65E88;
	sub_82902B70(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,10008
	ctx.r11.s64 = ctx.r11.s64 + 10008;
	// addi r3,r10,5344
	ctx.r3.s64 = ctx.r10.s64 + 5344;
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
	// bl 0x829ffa48
	ctx.lr = 0x82A65EB0;
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

PPC_FUNC_IMPL(__imp__sub_82A65EC8);
PPC_WEAK_FUNC(sub_82A65EC8) { __imp__sub_82A65EC8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A65EC8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r3,r11,11276
	ctx.r3.s64 = ctx.r11.s64 + 11276;
	// bl 0x8284e060
	ctx.lr = 0x82A65EE8;
	sub_8284E060(ctx, base);
	// lis r10,-31989
	ctx.r10.s64 = -2096431104;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r31,r10,24368
	ctx.r31.s64 = ctx.r10.s64 + 24368;
	// addi r11,r11,11244
	ctx.r11.s64 = ctx.r11.s64 + 11244;
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A65F08;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,11212
	ctx.r11.s64 = ctx.r11.s64 + 11212;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A65F20;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,11180
	ctx.r11.s64 = ctx.r11.s64 + 11180;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A65F38;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,11148
	ctx.r11.s64 = ctx.r11.s64 + 11148;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A65F50;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,11116
	ctx.r11.s64 = ctx.r11.s64 + 11116;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A65F68;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,20(r31)
	PPC_STORE_U32(ctx.r31.u32 + 20, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,11084
	ctx.r11.s64 = ctx.r11.s64 + 11084;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A65F80;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,24(r31)
	PPC_STORE_U32(ctx.r31.u32 + 24, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,11052
	ctx.r11.s64 = ctx.r11.s64 + 11052;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A65F98;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,28(r31)
	PPC_STORE_U32(ctx.r31.u32 + 28, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,11020
	ctx.r11.s64 = ctx.r11.s64 + 11020;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A65FB0;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,32(r31)
	PPC_STORE_U32(ctx.r31.u32 + 32, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,10988
	ctx.r11.s64 = ctx.r11.s64 + 10988;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A65FC8;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,36(r31)
	PPC_STORE_U32(ctx.r31.u32 + 36, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,10956
	ctx.r11.s64 = ctx.r11.s64 + 10956;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A65FE0;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,10924
	ctx.r11.s64 = ctx.r11.s64 + 10924;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A65FF8;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,44(r31)
	PPC_STORE_U32(ctx.r31.u32 + 44, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,10892
	ctx.r11.s64 = ctx.r11.s64 + 10892;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A66010;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,48(r31)
	PPC_STORE_U32(ctx.r31.u32 + 48, ctx.r3.u32);
	// addi r11,r11,10860
	ctx.r11.s64 = ctx.r11.s64 + 10860;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A66028;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,52(r31)
	PPC_STORE_U32(ctx.r31.u32 + 52, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,10828
	ctx.r11.s64 = ctx.r11.s64 + 10828;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A66040;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,56(r31)
	PPC_STORE_U32(ctx.r31.u32 + 56, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,10796
	ctx.r11.s64 = ctx.r11.s64 + 10796;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A66058;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,60(r31)
	PPC_STORE_U32(ctx.r31.u32 + 60, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,10764
	ctx.r3.s64 = ctx.r11.s64 + 10764;
	// bl 0x8284e060
	ctx.lr = 0x82A6606C;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,64(r31)
	PPC_STORE_U32(ctx.r31.u32 + 64, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,10732
	ctx.r11.s64 = ctx.r11.s64 + 10732;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A66084;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,68(r31)
	PPC_STORE_U32(ctx.r31.u32 + 68, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,10700
	ctx.r11.s64 = ctx.r11.s64 + 10700;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A6609C;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,72(r31)
	PPC_STORE_U32(ctx.r31.u32 + 72, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,10664
	ctx.r11.s64 = ctx.r11.s64 + 10664;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A660B4;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,76(r31)
	PPC_STORE_U32(ctx.r31.u32 + 76, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,10632
	ctx.r11.s64 = ctx.r11.s64 + 10632;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A660CC;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,80(r31)
	PPC_STORE_U32(ctx.r31.u32 + 80, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,10596
	ctx.r11.s64 = ctx.r11.s64 + 10596;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A660E4;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,84(r31)
	PPC_STORE_U32(ctx.r31.u32 + 84, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,10564
	ctx.r11.s64 = ctx.r11.s64 + 10564;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A660FC;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,88(r31)
	PPC_STORE_U32(ctx.r31.u32 + 88, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,10528
	ctx.r11.s64 = ctx.r11.s64 + 10528;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A66114;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,92(r31)
	PPC_STORE_U32(ctx.r31.u32 + 92, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,10496
	ctx.r11.s64 = ctx.r11.s64 + 10496;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A6612C;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,96(r31)
	PPC_STORE_U32(ctx.r31.u32 + 96, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,10460
	ctx.r11.s64 = ctx.r11.s64 + 10460;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A66144;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,100(r31)
	PPC_STORE_U32(ctx.r31.u32 + 100, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,10428
	ctx.r11.s64 = ctx.r11.s64 + 10428;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A6615C;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,10392
	ctx.r11.s64 = ctx.r11.s64 + 10392;
	// stw r3,104(r31)
	PPC_STORE_U32(ctx.r31.u32 + 104, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A66174;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,108(r31)
	PPC_STORE_U32(ctx.r31.u32 + 108, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,10360
	ctx.r11.s64 = ctx.r11.s64 + 10360;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A6618C;
	sub_8284E060(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// stw r3,112(r31)
	PPC_STORE_U32(ctx.r31.u32 + 112, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,10324
	ctx.r11.s64 = ctx.r11.s64 + 10324;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x8284e060
	ctx.lr = 0x82A661A4;
	sub_8284E060(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A661C0);
PPC_WEAK_FUNC(sub_82A661C0) { __imp__sub_82A661C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A661C0) {
	PPC_FUNC_PROLOGUE();
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
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-8960
	ctx.r3.s64 = ctx.r11.s64 + -8960;
	// bl 0x8284e060
	ctx.lr = 0x82A661DC;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28724(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28724, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A661F8);
PPC_WEAK_FUNC(sub_82A661F8) { __imp__sub_82A661F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A661F8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,2216
	ctx.r3.s64 = ctx.r11.s64 + 2216;
	// bl 0x8284e060
	ctx.lr = 0x82A66214;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28728(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28728, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66230);
PPC_WEAK_FUNC(sub_82A66230) { __imp__sub_82A66230(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66230) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12420
	ctx.r3.s64 = ctx.r11.s64 + 12420;
	// bl 0x8284e060
	ctx.lr = 0x82A6624C;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28732(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28732, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66268);
PPC_WEAK_FUNC(sub_82A66268) { __imp__sub_82A66268(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66268) {
	PPC_FUNC_PROLOGUE();
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
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-9476
	ctx.r3.s64 = ctx.r11.s64 + -9476;
	// bl 0x8284e060
	ctx.lr = 0x82A66284;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28736(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28736, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A662A0);
PPC_WEAK_FUNC(sub_82A662A0) { __imp__sub_82A662A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A662A0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12428
	ctx.r3.s64 = ctx.r11.s64 + 12428;
	// bl 0x8284e060
	ctx.lr = 0x82A662BC;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28740(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28740, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A662D8);
PPC_WEAK_FUNC(sub_82A662D8) { __imp__sub_82A662D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A662D8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12436
	ctx.r3.s64 = ctx.r11.s64 + 12436;
	// bl 0x8284e060
	ctx.lr = 0x82A662F4;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28744(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28744, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66310);
PPC_WEAK_FUNC(sub_82A66310) { __imp__sub_82A66310(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66310) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12444
	ctx.r3.s64 = ctx.r11.s64 + 12444;
	// bl 0x8284e060
	ctx.lr = 0x82A6632C;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28748(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28748, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66348);
PPC_WEAK_FUNC(sub_82A66348) { __imp__sub_82A66348(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66348) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12460
	ctx.r3.s64 = ctx.r11.s64 + 12460;
	// bl 0x8284e060
	ctx.lr = 0x82A66364;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28752(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28752, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66380);
PPC_WEAK_FUNC(sub_82A66380) { __imp__sub_82A66380(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66380) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12472
	ctx.r3.s64 = ctx.r11.s64 + 12472;
	// bl 0x8284e060
	ctx.lr = 0x82A6639C;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28756(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28756, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A663B8);
PPC_WEAK_FUNC(sub_82A663B8) { __imp__sub_82A663B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A663B8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12480
	ctx.r3.s64 = ctx.r11.s64 + 12480;
	// bl 0x8284e060
	ctx.lr = 0x82A663D4;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28760(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28760, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A663F0);
PPC_WEAK_FUNC(sub_82A663F0) { __imp__sub_82A663F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A663F0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12488
	ctx.r3.s64 = ctx.r11.s64 + 12488;
	// bl 0x8284e060
	ctx.lr = 0x82A6640C;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28764(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28764, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66428);
PPC_WEAK_FUNC(sub_82A66428) { __imp__sub_82A66428(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66428) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12500
	ctx.r3.s64 = ctx.r11.s64 + 12500;
	// bl 0x8284e060
	ctx.lr = 0x82A66444;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28768(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28768, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66460);
PPC_WEAK_FUNC(sub_82A66460) { __imp__sub_82A66460(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66460) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12512
	ctx.r3.s64 = ctx.r11.s64 + 12512;
	// bl 0x8284e060
	ctx.lr = 0x82A6647C;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28772(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28772, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66498);
PPC_WEAK_FUNC(sub_82A66498) { __imp__sub_82A66498(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66498) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12524
	ctx.r3.s64 = ctx.r11.s64 + 12524;
	// bl 0x8284e060
	ctx.lr = 0x82A664B4;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28776(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28776, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A664D0);
PPC_WEAK_FUNC(sub_82A664D0) { __imp__sub_82A664D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A664D0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12536
	ctx.r3.s64 = ctx.r11.s64 + 12536;
	// bl 0x8284e060
	ctx.lr = 0x82A664EC;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28780(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28780, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66508);
PPC_WEAK_FUNC(sub_82A66508) { __imp__sub_82A66508(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66508) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12552
	ctx.r3.s64 = ctx.r11.s64 + 12552;
	// bl 0x8284e060
	ctx.lr = 0x82A66524;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28784(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28784, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66540);
PPC_WEAK_FUNC(sub_82A66540) { __imp__sub_82A66540(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66540) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12568
	ctx.r3.s64 = ctx.r11.s64 + 12568;
	// bl 0x8284e060
	ctx.lr = 0x82A6655C;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28788(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28788, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66578);
PPC_WEAK_FUNC(sub_82A66578) { __imp__sub_82A66578(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66578) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12588
	ctx.r3.s64 = ctx.r11.s64 + 12588;
	// bl 0x8284e060
	ctx.lr = 0x82A66594;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28792(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28792, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A665B0);
PPC_WEAK_FUNC(sub_82A665B0) { __imp__sub_82A665B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A665B0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12612
	ctx.r3.s64 = ctx.r11.s64 + 12612;
	// bl 0x8284e060
	ctx.lr = 0x82A665CC;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28796(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28796, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A665E8);
PPC_WEAK_FUNC(sub_82A665E8) { __imp__sub_82A665E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A665E8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12640
	ctx.r3.s64 = ctx.r11.s64 + 12640;
	// bl 0x8284e060
	ctx.lr = 0x82A66604;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28800(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28800, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66620);
PPC_WEAK_FUNC(sub_82A66620) { __imp__sub_82A66620(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66620) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12672
	ctx.r3.s64 = ctx.r11.s64 + 12672;
	// bl 0x8284e060
	ctx.lr = 0x82A6663C;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28804(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28804, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66658);
PPC_WEAK_FUNC(sub_82A66658) { __imp__sub_82A66658(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66658) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12692
	ctx.r3.s64 = ctx.r11.s64 + 12692;
	// bl 0x8284e060
	ctx.lr = 0x82A66674;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28808(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28808, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66690);
PPC_WEAK_FUNC(sub_82A66690) { __imp__sub_82A66690(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66690) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12720
	ctx.r3.s64 = ctx.r11.s64 + 12720;
	// bl 0x8284e060
	ctx.lr = 0x82A666AC;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28812(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28812, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A666C8);
PPC_WEAK_FUNC(sub_82A666C8) { __imp__sub_82A666C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A666C8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12740
	ctx.r3.s64 = ctx.r11.s64 + 12740;
	// bl 0x8284e060
	ctx.lr = 0x82A666E4;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28816(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28816, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66700);
PPC_WEAK_FUNC(sub_82A66700) { __imp__sub_82A66700(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66700) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12768
	ctx.r3.s64 = ctx.r11.s64 + 12768;
	// bl 0x8284e060
	ctx.lr = 0x82A6671C;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28820(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28820, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66738);
PPC_WEAK_FUNC(sub_82A66738) { __imp__sub_82A66738(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66738) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12792
	ctx.r3.s64 = ctx.r11.s64 + 12792;
	// bl 0x8284e060
	ctx.lr = 0x82A66754;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28824(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28824, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66770);
PPC_WEAK_FUNC(sub_82A66770) { __imp__sub_82A66770(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66770) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12820
	ctx.r3.s64 = ctx.r11.s64 + 12820;
	// bl 0x8284e060
	ctx.lr = 0x82A6678C;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28828(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28828, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A667A8);
PPC_WEAK_FUNC(sub_82A667A8) { __imp__sub_82A667A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A667A8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12852
	ctx.r3.s64 = ctx.r11.s64 + 12852;
	// bl 0x8284e060
	ctx.lr = 0x82A667C4;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28832(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28832, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A667E0);
PPC_WEAK_FUNC(sub_82A667E0) { __imp__sub_82A667E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A667E0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12888
	ctx.r3.s64 = ctx.r11.s64 + 12888;
	// bl 0x8284e060
	ctx.lr = 0x82A667FC;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28836(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28836, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66818);
PPC_WEAK_FUNC(sub_82A66818) { __imp__sub_82A66818(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66818) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12912
	ctx.r3.s64 = ctx.r11.s64 + 12912;
	// bl 0x8284e060
	ctx.lr = 0x82A66834;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28840(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28840, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66850);
PPC_WEAK_FUNC(sub_82A66850) { __imp__sub_82A66850(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66850) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12940
	ctx.r3.s64 = ctx.r11.s64 + 12940;
	// bl 0x8284e060
	ctx.lr = 0x82A6686C;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28844(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28844, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66888);
PPC_WEAK_FUNC(sub_82A66888) { __imp__sub_82A66888(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66888) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12972
	ctx.r3.s64 = ctx.r11.s64 + 12972;
	// bl 0x8284e060
	ctx.lr = 0x82A668A4;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28848(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28848, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A668C0);
PPC_WEAK_FUNC(sub_82A668C0) { __imp__sub_82A668C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A668C0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,13008
	ctx.r3.s64 = ctx.r11.s64 + 13008;
	// bl 0x8284e060
	ctx.lr = 0x82A668DC;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28852(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28852, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A668F8);
PPC_WEAK_FUNC(sub_82A668F8) { __imp__sub_82A668F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A668F8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,13040
	ctx.r3.s64 = ctx.r11.s64 + 13040;
	// bl 0x8284e060
	ctx.lr = 0x82A66914;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28856(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28856, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66930);
PPC_WEAK_FUNC(sub_82A66930) { __imp__sub_82A66930(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66930) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,13076
	ctx.r3.s64 = ctx.r11.s64 + 13076;
	// bl 0x8284e060
	ctx.lr = 0x82A6694C;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28860(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28860, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66968);
PPC_WEAK_FUNC(sub_82A66968) { __imp__sub_82A66968(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66968) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,13116
	ctx.r3.s64 = ctx.r11.s64 + 13116;
	// bl 0x8284e060
	ctx.lr = 0x82A66984;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28864(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28864, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A669A0);
PPC_WEAK_FUNC(sub_82A669A0) { __imp__sub_82A669A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A669A0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12420
	ctx.r3.s64 = ctx.r11.s64 + 12420;
	// bl 0x8284e060
	ctx.lr = 0x82A669BC;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28868(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28868, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A669D8);
PPC_WEAK_FUNC(sub_82A669D8) { __imp__sub_82A669D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A669D8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-27304
	ctx.r3.s64 = ctx.r11.s64 + -27304;
	// bl 0x8284e060
	ctx.lr = 0x82A669F4;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28872(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28872, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66A10);
PPC_WEAK_FUNC(sub_82A66A10) { __imp__sub_82A66A10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66A10) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,32744
	ctx.r3.s64 = ctx.r11.s64 + 32744;
	// bl 0x8284e060
	ctx.lr = 0x82A66A2C;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28876(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28876, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66A48);
PPC_WEAK_FUNC(sub_82A66A48) { __imp__sub_82A66A48(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66A48) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,13160
	ctx.r3.s64 = ctx.r11.s64 + 13160;
	// bl 0x8284e060
	ctx.lr = 0x82A66A64;
	sub_8284E060(ctx, base);
	// lis r11,-31989
	ctx.r11.s64 = -2096431104;
	// stw r3,28880(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28880, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66A80);
PPC_WEAK_FUNC(sub_82A66A80) { __imp__sub_82A66A80(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66A80) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// addi r3,r11,-29464
	ctx.r3.s64 = ctx.r11.s64 + -29464;
	// bl 0x8290c330
	ctx.lr = 0x82A66A98;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5368
	ctx.r3.s64 = ctx.r11.s64 + 5368;
	// bl 0x829ffa48
	ctx.lr = 0x82A66AA4;
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

PPC_FUNC_IMPL(__imp__sub_82A66AB8);
PPC_WEAK_FUNC(sub_82A66AB8) { __imp__sub_82A66AB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66AB8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// addi r3,r11,-29424
	ctx.r3.s64 = ctx.r11.s64 + -29424;
	// bl 0x8290c330
	ctx.lr = 0x82A66AD0;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5376
	ctx.r3.s64 = ctx.r11.s64 + 5376;
	// bl 0x829ffa48
	ctx.lr = 0x82A66ADC;
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

PPC_FUNC_IMPL(__imp__sub_82A66AF0);
PPC_WEAK_FUNC(sub_82A66AF0) { __imp__sub_82A66AF0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66AF0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// addi r3,r11,-29384
	ctx.r3.s64 = ctx.r11.s64 + -29384;
	// bl 0x82667e60
	ctx.lr = 0x82A66B08;
	sub_82667E60(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5384
	ctx.r3.s64 = ctx.r11.s64 + 5384;
	// bl 0x829ffa48
	ctx.lr = 0x82A66B14;
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

PPC_FUNC_IMPL(__imp__sub_82A66B28);
PPC_WEAK_FUNC(sub_82A66B28) { __imp__sub_82A66B28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66B28) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,16664
	ctx.r3.s64 = ctx.r11.s64 + 16664;
	// bl 0x8284e060
	ctx.lr = 0x82A66B44;
	sub_8284E060(ctx, base);
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// stw r3,-26804(r11)
	PPC_STORE_U32(ctx.r11.u32 + -26804, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66B60);
PPC_WEAK_FUNC(sub_82A66B60) { __imp__sub_82A66B60(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66B60) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,16688
	ctx.r3.s64 = ctx.r11.s64 + 16688;
	// bl 0x8284e060
	ctx.lr = 0x82A66B7C;
	sub_8284E060(ctx, base);
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// stw r3,-26800(r11)
	PPC_STORE_U32(ctx.r11.u32 + -26800, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66B98);
PPC_WEAK_FUNC(sub_82A66B98) { __imp__sub_82A66B98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66B98) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// addi r3,r11,-26752
	ctx.r3.s64 = ctx.r11.s64 + -26752;
	// bl 0x8266ae48
	ctx.lr = 0x82A66BB0;
	sub_8266AE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5448
	ctx.r3.s64 = ctx.r11.s64 + 5448;
	// bl 0x829ffa48
	ctx.lr = 0x82A66BBC;
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

PPC_FUNC_IMPL(__imp__sub_82A66BD0);
PPC_WEAK_FUNC(sub_82A66BD0) { __imp__sub_82A66BD0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66BD0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// addi r3,r11,-26796
	ctx.r3.s64 = ctx.r11.s64 + -26796;
	// bl 0x8290c330
	ctx.lr = 0x82A66BE8;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5416
	ctx.r3.s64 = ctx.r11.s64 + 5416;
	// bl 0x829ffa48
	ctx.lr = 0x82A66BF4;
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

PPC_FUNC_IMPL(__imp__sub_82A66C08);
PPC_WEAK_FUNC(sub_82A66C08) { __imp__sub_82A66C08(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66C08) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// addi r3,r11,-20992
	ctx.r3.s64 = ctx.r11.s64 + -20992;
	// bl 0x8290c330
	ctx.lr = 0x82A66C20;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5408
	ctx.r3.s64 = ctx.r11.s64 + 5408;
	// bl 0x829ffa48
	ctx.lr = 0x82A66C2C;
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

PPC_FUNC_IMPL(__imp__sub_82A66C40);
PPC_WEAK_FUNC(sub_82A66C40) { __imp__sub_82A66C40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66C40) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// addi r3,r11,-20952
	ctx.r3.s64 = ctx.r11.s64 + -20952;
	// bl 0x8290c330
	ctx.lr = 0x82A66C58;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5400
	ctx.r3.s64 = ctx.r11.s64 + 5400;
	// bl 0x829ffa48
	ctx.lr = 0x82A66C64;
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

PPC_FUNC_IMPL(__imp__sub_82A66C78);
PPC_WEAK_FUNC(sub_82A66C78) { __imp__sub_82A66C78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66C78) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// addi r3,r11,-20912
	ctx.r3.s64 = ctx.r11.s64 + -20912;
	// bl 0x8290c330
	ctx.lr = 0x82A66C90;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5432
	ctx.r3.s64 = ctx.r11.s64 + 5432;
	// bl 0x829ffa48
	ctx.lr = 0x82A66C9C;
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

PPC_FUNC_IMPL(__imp__sub_82A66CB0);
PPC_WEAK_FUNC(sub_82A66CB0) { __imp__sub_82A66CB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66CB0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// addi r3,r11,-20872
	ctx.r3.s64 = ctx.r11.s64 + -20872;
	// bl 0x8290c330
	ctx.lr = 0x82A66CC8;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5424
	ctx.r3.s64 = ctx.r11.s64 + 5424;
	// bl 0x829ffa48
	ctx.lr = 0x82A66CD4;
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

PPC_FUNC_IMPL(__imp__sub_82A66CE8);
PPC_WEAK_FUNC(sub_82A66CE8) { __imp__sub_82A66CE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66CE8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// addi r3,r11,-20832
	ctx.r3.s64 = ctx.r11.s64 + -20832;
	// bl 0x8290c330
	ctx.lr = 0x82A66D00;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5440
	ctx.r3.s64 = ctx.r11.s64 + 5440;
	// bl 0x829ffa48
	ctx.lr = 0x82A66D0C;
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

PPC_FUNC_IMPL(__imp__sub_82A66D20);
PPC_WEAK_FUNC(sub_82A66D20) { __imp__sub_82A66D20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66D20) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,5504
	ctx.r3.s64 = ctx.r11.s64 + 5504;
	// bl 0x8284e060
	ctx.lr = 0x82A66D3C;
	sub_8284E060(ctx, base);
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// stw r3,-20428(r11)
	PPC_STORE_U32(ctx.r11.u32 + -20428, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66D58);
PPC_WEAK_FUNC(sub_82A66D58) { __imp__sub_82A66D58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66D58) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,16720
	ctx.r3.s64 = ctx.r11.s64 + 16720;
	// bl 0x8284e060
	ctx.lr = 0x82A66D74;
	sub_8284E060(ctx, base);
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// stw r3,-20424(r11)
	PPC_STORE_U32(ctx.r11.u32 + -20424, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66D90);
PPC_WEAK_FUNC(sub_82A66D90) { __imp__sub_82A66D90(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66D90) {
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
	// li r31,4
	ctx.r31.s64 = 4;
	// addi r11,r11,-20368
	ctx.r11.s64 = ctx.r11.s64 + -20368;
	// addi r30,r11,60
	ctx.r30.s64 = ctx.r11.s64 + 60;
loc_82A66DB4:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82259bb8
	ctx.lr = 0x82A66DBC;
	sub_82259BB8(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,8
	ctx.r30.s64 = ctx.r30.s64 + 8;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x82a66db4
	if (!ctx.cr6.lt) goto loc_82A66DB4;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5808
	ctx.r3.s64 = ctx.r11.s64 + 5808;
	// bl 0x829ffa48
	ctx.lr = 0x82A66DD8;
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

PPC_FUNC_IMPL(__imp__sub_82A66DF0);
PPC_WEAK_FUNC(sub_82A66DF0) { __imp__sub_82A66DF0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66DF0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// addi r3,r11,-20160
	ctx.r3.s64 = ctx.r11.s64 + -20160;
	// bl 0x827d5b10
	ctx.lr = 0x82A66E08;
	sub_827D5B10(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5488
	ctx.r3.s64 = ctx.r11.s64 + 5488;
	// bl 0x829ffa48
	ctx.lr = 0x82A66E14;
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

PPC_FUNC_IMPL(__imp__sub_82A66E28);
PPC_WEAK_FUNC(sub_82A66E28) { __imp__sub_82A66E28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66E28) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r3,r10,5600
	ctx.r3.s64 = ctx.r10.s64 + 5600;
	// lis r10,-31988
	ctx.r10.s64 = -2096365568;
	// addi r11,r11,17956
	ctx.r11.s64 = ctx.r11.s64 + 17956;
	// addi r10,r10,-19808
	ctx.r10.s64 = ctx.r10.s64 + -19808;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// stw r11,208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 208, ctx.r11.u32);
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66E50);
PPC_WEAK_FUNC(sub_82A66E50) { __imp__sub_82A66E50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66E50) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// lis r4,-32256
	ctx.r4.s64 = -2113929216;
	// lis r5,-32256
	ctx.r5.s64 = -2113929216;
	// lis r6,-32256
	ctx.r6.s64 = -2113929216;
	// lis r7,-32256
	ctx.r7.s64 = -2113929216;
	// lis r9,-32252
	ctx.r9.s64 = -2113667072;
	// addi r11,r11,-19392
	ctx.r11.s64 = ctx.r11.s64 + -19392;
	// lfs f12,3444(r4)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r4.u32 + 3444);
	ctx.f12.f64 = double(temp.f32);
	// li r3,1
	ctx.r3.s64 = 1;
	// lfs f11,3528(r5)
	temp.u32 = PPC_LOAD_U32(ctx.r5.u32 + 3528);
	ctx.f11.f64 = double(temp.f32);
	// addi r8,r9,17948
	ctx.r8.s64 = ctx.r9.s64 + 17948;
	// lfs f0,3400(r6)
	temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + 3400);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,2612(r7)
	temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + 2612);
	ctx.f13.f64 = double(temp.f32);
	// mr r10,r3
	ctx.r10.u64 = ctx.r3.u64;
	// addi r11,r11,52
	ctx.r11.s64 = ctx.r11.s64 + 52;
	// li r9,0
	ctx.r9.s64 = 0;
loc_82A66E90:
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
	// bge cr6,0x82a66e90
	if (!ctx.cr6.lt) goto loc_82A66E90;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5688
	ctx.r3.s64 = ctx.r11.s64 + 5688;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66F08);
PPC_WEAK_FUNC(sub_82A66F08) { __imp__sub_82A66F08(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66F08) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5768
	ctx.r3.s64 = ctx.r11.s64 + 5768;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66F18);
PPC_WEAK_FUNC(sub_82A66F18) { __imp__sub_82A66F18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66F18) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// addi r3,r11,-19184
	ctx.r3.s64 = ctx.r11.s64 + -19184;
	// bl 0x828be650
	ctx.lr = 0x82A66F30;
	sub_828BE650(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5472
	ctx.r3.s64 = ctx.r11.s64 + 5472;
	// bl 0x829ffa48
	ctx.lr = 0x82A66F3C;
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

PPC_FUNC_IMPL(__imp__sub_82A66F50);
PPC_WEAK_FUNC(sub_82A66F50) { __imp__sub_82A66F50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66F50) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,19652
	ctx.r3.s64 = ctx.r11.s64 + 19652;
	// bl 0x8284e060
	ctx.lr = 0x82A66F6C;
	sub_8284E060(ctx, base);
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// stw r3,26156(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26156, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66F88);
PPC_WEAK_FUNC(sub_82A66F88) { __imp__sub_82A66F88(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66F88) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,19656
	ctx.r3.s64 = ctx.r11.s64 + 19656;
	// bl 0x8284e060
	ctx.lr = 0x82A66FA4;
	sub_8284E060(ctx, base);
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// stw r3,26160(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26160, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66FC0);
PPC_WEAK_FUNC(sub_82A66FC0) { __imp__sub_82A66FC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66FC0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,19668
	ctx.r3.s64 = ctx.r11.s64 + 19668;
	// bl 0x8284e060
	ctx.lr = 0x82A66FDC;
	sub_8284E060(ctx, base);
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// stw r3,26164(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26164, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A66FF8);
PPC_WEAK_FUNC(sub_82A66FF8) { __imp__sub_82A66FF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A66FF8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,19680
	ctx.r3.s64 = ctx.r11.s64 + 19680;
	// bl 0x8284e060
	ctx.lr = 0x82A67014;
	sub_8284E060(ctx, base);
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// stw r3,26168(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26168, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A67030);
PPC_WEAK_FUNC(sub_82A67030) { __imp__sub_82A67030(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67030) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,19692
	ctx.r3.s64 = ctx.r11.s64 + 19692;
	// bl 0x8284e060
	ctx.lr = 0x82A6704C;
	sub_8284E060(ctx, base);
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// stw r3,26172(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26172, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A67068);
PPC_WEAK_FUNC(sub_82A67068) { __imp__sub_82A67068(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67068) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,19704
	ctx.r3.s64 = ctx.r11.s64 + 19704;
	// bl 0x8284e060
	ctx.lr = 0x82A67084;
	sub_8284E060(ctx, base);
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// stw r3,26176(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26176, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A670A0);
PPC_WEAK_FUNC(sub_82A670A0) { __imp__sub_82A670A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A670A0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31988
	ctx.r10.s64 = -2096365568;
	// li r11,-1
	ctx.r11.s64 = -1;
	// stw r11,26204(r10)
	PPC_STORE_U32(ctx.r10.u32 + 26204, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A670B0);
PPC_WEAK_FUNC(sub_82A670B0) { __imp__sub_82A670B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A670B0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,21172
	ctx.r3.s64 = ctx.r11.s64 + 21172;
	// bl 0x8284e060
	ctx.lr = 0x82A670CC;
	sub_8284E060(ctx, base);
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// stw r3,26992(r11)
	PPC_STORE_U32(ctx.r11.u32 + 26992, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A670E8);
PPC_WEAK_FUNC(sub_82A670E8) { __imp__sub_82A670E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A670E8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31988
	ctx.r11.s64 = -2096365568;
	// addi r3,r11,27052
	ctx.r3.s64 = ctx.r11.s64 + 27052;
	// bl 0x8290c330
	ctx.lr = 0x82A67100;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5824
	ctx.r3.s64 = ctx.r11.s64 + 5824;
	// bl 0x829ffa48
	ctx.lr = 0x82A6710C;
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

PPC_FUNC_IMPL(__imp__sub_82A67120);
PPC_WEAK_FUNC(sub_82A67120) { __imp__sub_82A67120(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67120) {
	PPC_FUNC_PROLOGUE();
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
	// bl 0x821b3510
	ctx.lr = 0x82A67134;
	sub_821B3510(ctx, base);
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// addi r3,r10,5832
	ctx.r3.s64 = ctx.r10.s64 + 5832;
	// lis r10,-32085
	ctx.r10.s64 = -2102722560;
	// stw r11,-1680(r10)
	PPC_STORE_U32(ctx.r10.u32 + -1680, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6714C;
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

PPC_FUNC_IMPL(__imp__sub_82A67160);
PPC_WEAK_FUNC(sub_82A67160) { __imp__sub_82A67160(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67160) {
	PPC_FUNC_PROLOGUE();
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
	// bl 0x821b3510
	ctx.lr = 0x82A67174;
	sub_821B3510(ctx, base);
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// addi r3,r10,5864
	ctx.r3.s64 = ctx.r10.s64 + 5864;
	// lis r10,-32085
	ctx.r10.s64 = -2102722560;
	// stw r11,-1672(r10)
	PPC_STORE_U32(ctx.r10.u32 + -1672, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6718C;
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

PPC_FUNC_IMPL(__imp__sub_82A671A0);
PPC_WEAK_FUNC(sub_82A671A0) { __imp__sub_82A671A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A671A0) {
	PPC_FUNC_PROLOGUE();
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
	// bl 0x821b3510
	ctx.lr = 0x82A671B4;
	sub_821B3510(ctx, base);
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// addi r3,r10,5896
	ctx.r3.s64 = ctx.r10.s64 + 5896;
	// lis r10,-32085
	ctx.r10.s64 = -2102722560;
	// stw r11,-1664(r10)
	PPC_STORE_U32(ctx.r10.u32 + -1664, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A671CC;
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

PPC_FUNC_IMPL(__imp__sub_82A671E0);
PPC_WEAK_FUNC(sub_82A671E0) { __imp__sub_82A671E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A671E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5960
	ctx.r3.s64 = ctx.r11.s64 + 5960;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A671F0);
PPC_WEAK_FUNC(sub_82A671F0) { __imp__sub_82A671F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A671F0) {
	PPC_FUNC_PROLOGUE();
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
	// bl 0x821b3510
	ctx.lr = 0x82A67204;
	sub_821B3510(ctx, base);
	// lis r10,-32085
	ctx.r10.s64 = -2102722560;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// stw r3,-400(r10)
	PPC_STORE_U32(ctx.r10.u32 + -400, ctx.r3.u32);
	// addi r3,r11,5928
	ctx.r3.s64 = ctx.r11.s64 + 5928;
	// bl 0x829ffa48
	ctx.lr = 0x82A67218;
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

PPC_FUNC_IMPL(__imp__sub_82A67228);
PPC_WEAK_FUNC(sub_82A67228) { __imp__sub_82A67228(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67228) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31987
	ctx.r11.s64 = -2096300032;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r11,r11,-21584
	ctx.r11.s64 = ctx.r11.s64 + -21584;
	// li r10,24
	ctx.r10.s64 = 24;
	// addi r11,r11,400
	ctx.r11.s64 = ctx.r11.s64 + 400;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_82A67240:
	// stb r9,0(r11)
	PPC_STORE_U8(ctx.r11.u32 + 0, ctx.r9.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bdnz 0x82a67240
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_82A67240;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5984
	ctx.r3.s64 = ctx.r11.s64 + 5984;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A67258);
PPC_WEAK_FUNC(sub_82A67258) { __imp__sub_82A67258(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67258) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31987
	ctx.r11.s64 = -2096300032;
	// addi r3,r11,-21152
	ctx.r3.s64 = ctx.r11.s64 + -21152;
	// bl 0x82434ea8
	ctx.lr = 0x82A67270;
	sub_82434EA8(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,5992
	ctx.r3.s64 = ctx.r11.s64 + 5992;
	// bl 0x829ffa48
	ctx.lr = 0x82A6727C;
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

PPC_FUNC_IMPL(__imp__sub_82A67290);
PPC_WEAK_FUNC(sub_82A67290) { __imp__sub_82A67290(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67290) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31987
	ctx.r11.s64 = -2096300032;
	// addi r3,r11,-21024
	ctx.r3.s64 = ctx.r11.s64 + -21024;
	// bl 0x82434ea8
	ctx.lr = 0x82A672A8;
	sub_82434EA8(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6000
	ctx.r3.s64 = ctx.r11.s64 + 6000;
	// bl 0x829ffa48
	ctx.lr = 0x82A672B4;
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

PPC_FUNC_IMPL(__imp__sub_82A672C8);
PPC_WEAK_FUNC(sub_82A672C8) { __imp__sub_82A672C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A672C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6008
	ctx.r3.s64 = ctx.r11.s64 + 6008;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A672D8);
PPC_WEAK_FUNC(sub_82A672D8) { __imp__sub_82A672D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A672D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,10320
	ctx.r5.s64 = ctx.r11.s64 + 10320;
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-12780
	ctx.r3.s64 = ctx.r11.s64 + -12780;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A672F8);
PPC_WEAK_FUNC(sub_82A672F8) { __imp__sub_82A672F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A672F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,10332
	ctx.r5.s64 = ctx.r11.s64 + 10332;
	// lis r11,-31978
	ctx.r11.s64 = -2095710208;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-2196
	ctx.r3.s64 = ctx.r11.s64 + -2196;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A67318);
PPC_WEAK_FUNC(sub_82A67318) { __imp__sub_82A67318(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67318) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,10344
	ctx.r5.s64 = ctx.r11.s64 + 10344;
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-12856
	ctx.r3.s64 = ctx.r11.s64 + -12856;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A67338);
PPC_WEAK_FUNC(sub_82A67338) { __imp__sub_82A67338(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67338) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,10352
	ctx.r5.s64 = ctx.r11.s64 + 10352;
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-12320
	ctx.r3.s64 = ctx.r11.s64 + -12320;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A67358);
PPC_WEAK_FUNC(sub_82A67358) { __imp__sub_82A67358(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67358) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,10364
	ctx.r5.s64 = ctx.r11.s64 + 10364;
	// lis r11,-31984
	ctx.r11.s64 = -2096103424;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-16568
	ctx.r3.s64 = ctx.r11.s64 + -16568;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A67378);
PPC_WEAK_FUNC(sub_82A67378) { __imp__sub_82A67378(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67378) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,10384
	ctx.r5.s64 = ctx.r11.s64 + 10384;
	// lis r11,-31978
	ctx.r11.s64 = -2095710208;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-2148
	ctx.r3.s64 = ctx.r11.s64 + -2148;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A67398);
PPC_WEAK_FUNC(sub_82A67398) { __imp__sub_82A67398(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67398) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,10388
	ctx.r5.s64 = ctx.r11.s64 + 10388;
	// lis r11,-31978
	ctx.r11.s64 = -2095710208;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-4964
	ctx.r3.s64 = ctx.r11.s64 + -4964;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A673B8);
PPC_WEAK_FUNC(sub_82A673B8) { __imp__sub_82A673B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A673B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,10400
	ctx.r5.s64 = ctx.r11.s64 + 10400;
	// lis r11,-31978
	ctx.r11.s64 = -2095710208;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-2096
	ctx.r3.s64 = ctx.r11.s64 + -2096;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A673D8);
PPC_WEAK_FUNC(sub_82A673D8) { __imp__sub_82A673D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A673D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r5,r11,10156
	ctx.r5.s64 = ctx.r11.s64 + 10156;
	// lis r11,-31978
	ctx.r11.s64 = -2095710208;
	// addi r31,r11,-5000
	ctx.r31.s64 = ctx.r11.s64 + -5000;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6740C;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6741C;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A67424;
	sub_829DC040(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,10152
	ctx.r11.s64 = ctx.r11.s64 + 10152;
	// addi r3,r10,6400
	ctx.r3.s64 = ctx.r10.s64 + 6400;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6743C;
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

PPC_FUNC_IMPL(__imp__sub_82A67450);
PPC_WEAK_FUNC(sub_82A67450) { __imp__sub_82A67450(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67450) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r5,r11,10180
	ctx.r5.s64 = ctx.r11.s64 + 10180;
	// lis r11,-31978
	ctx.r11.s64 = -2095710208;
	// addi r31,r11,-2128
	ctx.r31.s64 = ctx.r11.s64 + -2128;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A67484;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A67494;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6749C;
	sub_829DC040(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,10176
	ctx.r11.s64 = ctx.r11.s64 + 10176;
	// addi r3,r10,6480
	ctx.r3.s64 = ctx.r10.s64 + 6480;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A674B4;
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

PPC_FUNC_IMPL(__imp__sub_82A674C8);
PPC_WEAK_FUNC(sub_82A674C8) { __imp__sub_82A674C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A674C8) {
	PPC_FUNC_PROLOGUE();
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
	// li r6,1
	ctx.r6.s64 = 1;
	// addi r4,r11,-20704
	ctx.r4.s64 = ctx.r11.s64 + -20704;
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// lis r5,2
	ctx.r5.s64 = 131072;
	// addi r3,r11,-17456
	ctx.r3.s64 = ctx.r11.s64 + -17456;
	// bl 0x828495c8
	ctx.lr = 0x82A674F0;
	sub_828495C8(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6040
	ctx.r3.s64 = ctx.r11.s64 + 6040;
	// bl 0x829ffa48
	ctx.lr = 0x82A674FC;
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

PPC_FUNC_IMPL(__imp__sub_82A67510);
PPC_WEAK_FUNC(sub_82A67510) { __imp__sub_82A67510(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67510) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31987
	ctx.r11.s64 = -2096300032;
	// li r6,1
	ctx.r6.s64 = 1;
	// addi r4,r11,-20832
	ctx.r4.s64 = ctx.r11.s64 + -20832;
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// lis r5,1
	ctx.r5.s64 = 65536;
	// addi r3,r11,-15152
	ctx.r3.s64 = ctx.r11.s64 + -15152;
	// bl 0x828495c8
	ctx.lr = 0x82A67538;
	sub_828495C8(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6056
	ctx.r3.s64 = ctx.r11.s64 + 6056;
	// bl 0x829ffa48
	ctx.lr = 0x82A67544;
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

PPC_FUNC_IMPL(__imp__sub_82A67558);
PPC_WEAK_FUNC(sub_82A67558) { __imp__sub_82A67558(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67558) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31978
	ctx.r11.s64 = -2095710208;
	// addi r3,r11,-2020
	ctx.r3.s64 = ctx.r11.s64 + -2020;
	// bl 0x829e5c50
	ctx.lr = 0x82A67570;
	sub_829E5C50(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6072
	ctx.r3.s64 = ctx.r11.s64 + 6072;
	// bl 0x829ffa48
	ctx.lr = 0x82A6757C;
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

PPC_FUNC_IMPL(__imp__sub_82A67590);
PPC_WEAK_FUNC(sub_82A67590) { __imp__sub_82A67590(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67590) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31978
	ctx.r11.s64 = -2095710208;
	// addi r3,r11,-4944
	ctx.r3.s64 = ctx.r11.s64 + -4944;
	// bl 0x829ec928
	ctx.lr = 0x82A675A8;
	sub_829EC928(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6088
	ctx.r3.s64 = ctx.r11.s64 + 6088;
	// bl 0x829ffa48
	ctx.lr = 0x82A675B4;
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

PPC_FUNC_IMPL(__imp__sub_82A675C8);
PPC_WEAK_FUNC(sub_82A675C8) { __imp__sub_82A675C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A675C8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r3,r11,-12292
	ctx.r3.s64 = ctx.r11.s64 + -12292;
	// bl 0x826f4448
	ctx.lr = 0x82A675E0;
	sub_826F4448(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6104
	ctx.r3.s64 = ctx.r11.s64 + 6104;
	// bl 0x829ffa48
	ctx.lr = 0x82A675EC;
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

PPC_FUNC_IMPL(__imp__sub_82A67600);
PPC_WEAK_FUNC(sub_82A67600) { __imp__sub_82A67600(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67600) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31984
	ctx.r11.s64 = -2096103424;
	// addi r3,r11,-15084
	ctx.r3.s64 = ctx.r11.s64 + -15084;
	// bl 0x826df530
	ctx.lr = 0x82A67618;
	sub_826DF530(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6120
	ctx.r3.s64 = ctx.r11.s64 + 6120;
	// bl 0x829ffa48
	ctx.lr = 0x82A67624;
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

PPC_FUNC_IMPL(__imp__sub_82A67638);
PPC_WEAK_FUNC(sub_82A67638) { __imp__sub_82A67638(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67638) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31978
	ctx.r11.s64 = -2095710208;
	// addi r3,r11,-1936
	ctx.r3.s64 = ctx.r11.s64 + -1936;
	// bl 0x826dc658
	ctx.lr = 0x82A67650;
	sub_826DC658(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6136
	ctx.r3.s64 = ctx.r11.s64 + 6136;
	// bl 0x829ffa48
	ctx.lr = 0x82A6765C;
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

PPC_FUNC_IMPL(__imp__sub_82A67670);
PPC_WEAK_FUNC(sub_82A67670) { __imp__sub_82A67670(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67670) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31984
	ctx.r11.s64 = -2096103424;
	// addi r3,r11,-16528
	ctx.r3.s64 = ctx.r11.s64 + -16528;
	// bl 0x829df838
	ctx.lr = 0x82A67688;
	sub_829DF838(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6152
	ctx.r3.s64 = ctx.r11.s64 + 6152;
	// bl 0x829ffa48
	ctx.lr = 0x82A67694;
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

PPC_FUNC_IMPL(__imp__sub_82A676A8);
PPC_WEAK_FUNC(sub_82A676A8) { __imp__sub_82A676A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A676A8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31978
	ctx.r11.s64 = -2095710208;
	// addi r3,r11,-2224
	ctx.r3.s64 = ctx.r11.s64 + -2224;
	// bl 0x829de7a8
	ctx.lr = 0x82A676C0;
	sub_829DE7A8(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6168
	ctx.r3.s64 = ctx.r11.s64 + 6168;
	// bl 0x829ffa48
	ctx.lr = 0x82A676CC;
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

PPC_FUNC_IMPL(__imp__sub_82A676E0);
PPC_WEAK_FUNC(sub_82A676E0) { __imp__sub_82A676E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A676E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r3,r11,-25492
	ctx.r3.s64 = ctx.r11.s64 + -25492;
	// bl 0x829de7a8
	ctx.lr = 0x82A676F8;
	sub_829DE7A8(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6184
	ctx.r3.s64 = ctx.r11.s64 + 6184;
	// bl 0x829ffa48
	ctx.lr = 0x82A67704;
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

PPC_FUNC_IMPL(__imp__sub_82A67718);
PPC_WEAK_FUNC(sub_82A67718) { __imp__sub_82A67718(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67718) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31978
	ctx.r11.s64 = -2095710208;
	// addi r3,r11,-2176
	ctx.r3.s64 = ctx.r11.s64 + -2176;
	// bl 0x829de7a8
	ctx.lr = 0x82A67730;
	sub_829DE7A8(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6200
	ctx.r3.s64 = ctx.r11.s64 + 6200;
	// bl 0x829ffa48
	ctx.lr = 0x82A6773C;
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

PPC_FUNC_IMPL(__imp__sub_82A67750);
PPC_WEAK_FUNC(sub_82A67750) { __imp__sub_82A67750(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67750) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6352
	ctx.r3.s64 = ctx.r11.s64 + 6352;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A67760);
PPC_WEAK_FUNC(sub_82A67760) { __imp__sub_82A67760(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67760) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r3,r11,-25464
	ctx.r3.s64 = ctx.r11.s64 + -25464;
	// bl 0x82701ce8
	ctx.lr = 0x82A67778;
	sub_82701CE8(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6216
	ctx.r3.s64 = ctx.r11.s64 + 6216;
	// bl 0x829ffa48
	ctx.lr = 0x82A67784;
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

PPC_FUNC_IMPL(__imp__sub_82A67798);
PPC_WEAK_FUNC(sub_82A67798) { __imp__sub_82A67798(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67798) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31978
	ctx.r11.s64 = -2095710208;
	// addi r3,r11,-5952
	ctx.r3.s64 = ctx.r11.s64 + -5952;
	// bl 0x829ea868
	ctx.lr = 0x82A677B0;
	sub_829EA868(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6232
	ctx.r3.s64 = ctx.r11.s64 + 6232;
	// bl 0x829ffa48
	ctx.lr = 0x82A677BC;
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

PPC_FUNC_IMPL(__imp__sub_82A677D0);
PPC_WEAK_FUNC(sub_82A677D0) { __imp__sub_82A677D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A677D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31978
	ctx.r11.s64 = -2095710208;
	// addi r3,r11,-1352
	ctx.r3.s64 = ctx.r11.s64 + -1352;
	// bl 0x826fd5d8
	ctx.lr = 0x82A677E8;
	sub_826FD5D8(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6560
	ctx.r3.s64 = ctx.r11.s64 + 6560;
	// bl 0x829ffa48
	ctx.lr = 0x82A677F4;
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

PPC_FUNC_IMPL(__imp__sub_82A67808);
PPC_WEAK_FUNC(sub_82A67808) { __imp__sub_82A67808(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67808) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// li r5,56
	ctx.r5.s64 = 56;
	// addi r31,r11,-12504
	ctx.r31.s64 = ctx.r11.s64 + -12504;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a11d08
	ctx.lr = 0x82A67830;
	rexcrt_memset(ctx, base);
	// li r11,-1
	ctx.r11.s64 = -1;
	// stw r11,24(r31)
	PPC_STORE_U32(ctx.r31.u32 + 24, ctx.r11.u32);
	// stw r11,28(r31)
	PPC_STORE_U32(ctx.r31.u32 + 28, ctx.r11.u32);
	// stw r11,32(r31)
	PPC_STORE_U32(ctx.r31.u32 + 32, ctx.r11.u32);
	// stw r11,36(r31)
	PPC_STORE_U32(ctx.r31.u32 + 36, ctx.r11.u32);
	// stw r11,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r11.u32);
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

PPC_FUNC_IMPL(__imp__sub_82A67860);
PPC_WEAK_FUNC(sub_82A67860) { __imp__sub_82A67860(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67860) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31978
	ctx.r11.s64 = -2095710208;
	// li r5,56
	ctx.r5.s64 = 56;
	// addi r31,r11,-2076
	ctx.r31.s64 = ctx.r11.s64 + -2076;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a11d08
	ctx.lr = 0x82A67888;
	rexcrt_memset(ctx, base);
	// li r11,-1
	ctx.r11.s64 = -1;
	// stw r11,24(r31)
	PPC_STORE_U32(ctx.r31.u32 + 24, ctx.r11.u32);
	// stw r11,28(r31)
	PPC_STORE_U32(ctx.r31.u32 + 28, ctx.r11.u32);
	// stw r11,32(r31)
	PPC_STORE_U32(ctx.r31.u32 + 32, ctx.r11.u32);
	// stw r11,36(r31)
	PPC_STORE_U32(ctx.r31.u32 + 36, ctx.r11.u32);
	// stw r11,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r11.u32);
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

PPC_FUNC_IMPL(__imp__sub_82A678B8);
PPC_WEAK_FUNC(sub_82A678B8) { __imp__sub_82A678B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A678B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6248
	ctx.r3.s64 = ctx.r11.s64 + 6248;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A678C8);
PPC_WEAK_FUNC(sub_82A678C8) { __imp__sub_82A678C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A678C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6256
	ctx.r3.s64 = ctx.r11.s64 + 6256;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A678D8);
PPC_WEAK_FUNC(sub_82A678D8) { __imp__sub_82A678D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A678D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6264
	ctx.r3.s64 = ctx.r11.s64 + 6264;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A678E8);
PPC_WEAK_FUNC(sub_82A678E8) { __imp__sub_82A678E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A678E8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r4,r11,10416
	ctx.r4.s64 = ctx.r11.s64 + 10416;
	// lis r11,-31978
	ctx.r11.s64 = -2095710208;
	// addi r3,r11,29600
	ctx.r3.s64 = ctx.r11.s64 + 29600;
	// bl 0x822bca90
	ctx.lr = 0x82A67908;
	sub_822BCA90(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6272
	ctx.r3.s64 = ctx.r11.s64 + 6272;
	// bl 0x829ffa48
	ctx.lr = 0x82A67914;
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

PPC_FUNC_IMPL(__imp__sub_82A67928);
PPC_WEAK_FUNC(sub_82A67928) { __imp__sub_82A67928(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67928) {
	PPC_FUNC_PROLOGUE();
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
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r3,r11,-25760
	ctx.r3.s64 = ctx.r11.s64 + -25760;
	// bl 0x822bca90
	ctx.lr = 0x82A67948;
	sub_822BCA90(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6288
	ctx.r3.s64 = ctx.r11.s64 + 6288;
	// bl 0x829ffa48
	ctx.lr = 0x82A67954;
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

PPC_FUNC_IMPL(__imp__sub_82A67968);
PPC_WEAK_FUNC(sub_82A67968) { __imp__sub_82A67968(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67968) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// lis r10,-31978
	ctx.r10.s64 = -2095710208;
	// lwz r11,10772(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 10772);
	// stw r11,-4968(r10)
	PPC_STORE_U32(ctx.r10.u32 + -4968, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A67980);
PPC_WEAK_FUNC(sub_82A67980) { __imp__sub_82A67980(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67980) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r31,r11,-12448
	ctx.r31.s64 = ctx.r11.s64 + -12448;
	// addi r3,r31,4
	ctx.r3.s64 = ctx.r31.s64 + 4;
	// bl 0x829e95e8
	ctx.lr = 0x82A679A0;
	sub_829E95E8(ctx, base);
	// li r11,-1
	ctx.r11.s64 = -1;
	// addi r3,r31,4
	ctx.r3.s64 = ctx.r31.s64 + 4;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829e9558
	ctx.lr = 0x82A679B0;
	sub_829E9558(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82A679D0);
PPC_WEAK_FUNC(sub_82A679D0) { __imp__sub_82A679D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A679D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r3,r11,-12376
	ctx.r3.s64 = ctx.r11.s64 + -12376;
	// bl 0x829e9da8
	ctx.lr = 0x82A679E8;
	sub_829E9DA8(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6360
	ctx.r3.s64 = ctx.r11.s64 + 6360;
	// bl 0x829ffa48
	ctx.lr = 0x82A679F4;
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

PPC_FUNC_IMPL(__imp__sub_82A67A08);
PPC_WEAK_FUNC(sub_82A67A08) { __imp__sub_82A67A08(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67A08) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r3,r11,-12836
	ctx.r3.s64 = ctx.r11.s64 + -12836;
	// bl 0x829e9da8
	ctx.lr = 0x82A67A20;
	sub_829E9DA8(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6304
	ctx.r3.s64 = ctx.r11.s64 + 6304;
	// bl 0x829ffa48
	ctx.lr = 0x82A67A2C;
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

PPC_FUNC_IMPL(__imp__sub_82A67A40);
PPC_WEAK_FUNC(sub_82A67A40) { __imp__sub_82A67A40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67A40) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31978
	ctx.r11.s64 = -2095710208;
	// addi r3,r11,-2280
	ctx.r3.s64 = ctx.r11.s64 + -2280;
	// bl 0x829e9da8
	ctx.lr = 0x82A67A58;
	sub_829E9DA8(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6320
	ctx.r3.s64 = ctx.r11.s64 + 6320;
	// bl 0x829ffa48
	ctx.lr = 0x82A67A64;
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

PPC_FUNC_IMPL(__imp__sub_82A67A78);
PPC_WEAK_FUNC(sub_82A67A78) { __imp__sub_82A67A78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67A78) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31984
	ctx.r11.s64 = -2096103424;
	// addi r3,r11,-15140
	ctx.r3.s64 = ctx.r11.s64 + -15140;
	// bl 0x829e9da8
	ctx.lr = 0x82A67A90;
	sub_829E9DA8(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6336
	ctx.r3.s64 = ctx.r11.s64 + 6336;
	// bl 0x829ffa48
	ctx.lr = 0x82A67A9C;
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

PPC_FUNC_IMPL(__imp__sub_82A67AB0);
PPC_WEAK_FUNC(sub_82A67AB0) { __imp__sub_82A67AB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67AB0) {
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
	// li r31,9
	ctx.r31.s64 = 9;
	// addi r30,r11,-20208
	ctx.r30.s64 = ctx.r11.s64 + -20208;
loc_82A67AD0:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x829ea318
	ctx.lr = 0x82A67AD8;
	sub_829EA318(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,364
	ctx.r30.s64 = ctx.r30.s64 + 364;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x82a67ad0
	if (!ctx.cr6.lt) goto loc_82A67AD0;
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

PPC_FUNC_IMPL(__imp__sub_82A67B00);
PPC_WEAK_FUNC(sub_82A67B00) { __imp__sub_82A67B00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67B00) {
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
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// li r31,31
	ctx.r31.s64 = 31;
	// addi r30,r11,-12760
	ctx.r30.s64 = ctx.r11.s64 + -12760;
loc_82A67B20:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x829db688
	ctx.lr = 0x82A67B28;
	sub_829DB688(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,8
	ctx.r30.s64 = ctx.r30.s64 + 8;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x82a67b20
	if (!ctx.cr6.lt) goto loc_82A67B20;
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

PPC_FUNC_IMPL(__imp__sub_82A67B50);
PPC_WEAK_FUNC(sub_82A67B50) { __imp__sub_82A67B50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67B50) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,11128
	ctx.r5.s64 = ctx.r11.s64 + 11128;
	// lis r11,-31976
	ctx.r11.s64 = -2095579136;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-2392
	ctx.r3.s64 = ctx.r11.s64 + -2392;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A67B70);
PPC_WEAK_FUNC(sub_82A67B70) { __imp__sub_82A67B70(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67B70) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-9852
	ctx.r3.s64 = ctx.r11.s64 + -9852;
	// bl 0x829efa80
	ctx.lr = 0x82A67B88;
	sub_829EFA80(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6576
	ctx.r3.s64 = ctx.r11.s64 + 6576;
	// bl 0x829ffa48
	ctx.lr = 0x82A67B94;
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

PPC_FUNC_IMPL(__imp__sub_82A67BA8);
PPC_WEAK_FUNC(sub_82A67BA8) { __imp__sub_82A67BA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67BA8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31976
	ctx.r11.s64 = -2095579136;
	// addi r3,r11,-2336
	ctx.r3.s64 = ctx.r11.s64 + -2336;
	// bl 0x829e9478
	ctx.lr = 0x82A67BC0;
	sub_829E9478(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6592
	ctx.r3.s64 = ctx.r11.s64 + 6592;
	// bl 0x829ffa48
	ctx.lr = 0x82A67BCC;
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

PPC_FUNC_IMPL(__imp__sub_82A67BE0);
PPC_WEAK_FUNC(sub_82A67BE0) { __imp__sub_82A67BE0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67BE0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31978
	ctx.r11.s64 = -2095710208;
	// lis r5,1
	ctx.r5.s64 = 65536;
	// addi r4,r11,29920
	ctx.r4.s64 = ctx.r11.s64 + 29920;
	// lis r11,-31976
	ctx.r11.s64 = -2095579136;
	// li r6,1
	ctx.r6.s64 = 1;
	// addi r3,r11,152
	ctx.r3.s64 = ctx.r11.s64 + 152;
	// ori r5,r5,32768
	ctx.r5.u64 = ctx.r5.u64 | 32768;
	// bl 0x828495c8
	ctx.lr = 0x82A67C0C;
	sub_828495C8(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6608
	ctx.r3.s64 = ctx.r11.s64 + 6608;
	// bl 0x829ffa48
	ctx.lr = 0x82A67C18;
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

PPC_FUNC_IMPL(__imp__sub_82A67C28);
PPC_WEAK_FUNC(sub_82A67C28) { __imp__sub_82A67C28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67C28) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31976
	ctx.r11.s64 = -2095579136;
	// addi r3,r11,-2448
	ctx.r3.s64 = ctx.r11.s64 + -2448;
	// bl 0x829e7010
	ctx.lr = 0x82A67C40;
	sub_829E7010(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6624
	ctx.r3.s64 = ctx.r11.s64 + 6624;
	// bl 0x829ffa48
	ctx.lr = 0x82A67C4C;
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

PPC_FUNC_IMPL(__imp__sub_82A67C60);
PPC_WEAK_FUNC(sub_82A67C60) { __imp__sub_82A67C60(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67C60) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31976
	ctx.r11.s64 = -2095579136;
	// addi r3,r11,-1992
	ctx.r3.s64 = ctx.r11.s64 + -1992;
	// bl 0x829ee9f8
	ctx.lr = 0x82A67C78;
	sub_829EE9F8(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6640
	ctx.r3.s64 = ctx.r11.s64 + 6640;
	// bl 0x829ffa48
	ctx.lr = 0x82A67C84;
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

PPC_FUNC_IMPL(__imp__sub_82A67C98);
PPC_WEAK_FUNC(sub_82A67C98) { __imp__sub_82A67C98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67C98) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-9960
	ctx.r3.s64 = ctx.r11.s64 + -9960;
	// bl 0x8278ef70
	ctx.lr = 0x82A67CB0;
	sub_8278EF70(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6656
	ctx.r3.s64 = ctx.r11.s64 + 6656;
	// bl 0x829ffa48
	ctx.lr = 0x82A67CBC;
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

PPC_FUNC_IMPL(__imp__sub_82A67CD0);
PPC_WEAK_FUNC(sub_82A67CD0) { __imp__sub_82A67CD0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67CD0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,-21768
	ctx.r3.s64 = ctx.r11.s64 + -21768;
	// b 0x829dbba8
	sub_829DBBA8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A67CE0);
PPC_WEAK_FUNC(sub_82A67CE0) { __imp__sub_82A67CE0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67CE0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31976
	ctx.r11.s64 = -2095579136;
	// addi r3,r11,-1920
	ctx.r3.s64 = ctx.r11.s64 + -1920;
	// bl 0x829e3e18
	ctx.lr = 0x82A67CF8;
	sub_829E3E18(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6672
	ctx.r3.s64 = ctx.r11.s64 + 6672;
	// bl 0x829ffa48
	ctx.lr = 0x82A67D04;
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

PPC_FUNC_IMPL(__imp__sub_82A67D18);
PPC_WEAK_FUNC(sub_82A67D18) { __imp__sub_82A67D18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67D18) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,7000
	ctx.r3.s64 = ctx.r11.s64 + 7000;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A67D28);
PPC_WEAK_FUNC(sub_82A67D28) { __imp__sub_82A67D28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67D28) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31976
	ctx.r11.s64 = -2095579136;
	// addi r3,r11,22048
	ctx.r3.s64 = ctx.r11.s64 + 22048;
	// bl 0x829ef570
	ctx.lr = 0x82A67D40;
	sub_829EF570(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6688
	ctx.r3.s64 = ctx.r11.s64 + 6688;
	// bl 0x829ffa48
	ctx.lr = 0x82A67D4C;
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

PPC_FUNC_IMPL(__imp__sub_82A67D60);
PPC_WEAK_FUNC(sub_82A67D60) { __imp__sub_82A67D60(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67D60) {
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
	// li r31,99
	ctx.r31.s64 = 99;
	// addi r30,r11,2448
	ctx.r30.s64 = ctx.r11.s64 + 2448;
loc_82A67D80:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x829e8ab8
	ctx.lr = 0x82A67D88;
	sub_829E8AB8(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,196
	ctx.r30.s64 = ctx.r30.s64 + 196;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x82a67d80
	if (!ctx.cr6.lt) goto loc_82A67D80;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6704
	ctx.r3.s64 = ctx.r11.s64 + 6704;
	// bl 0x829ffa48
	ctx.lr = 0x82A67DA4;
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

PPC_FUNC_IMPL(__imp__sub_82A67DC0);
PPC_WEAK_FUNC(sub_82A67DC0) { __imp__sub_82A67DC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67DC0) {
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
	// li r31,99
	ctx.r31.s64 = 99;
	// addi r30,r11,24168
	ctx.r30.s64 = ctx.r11.s64 + 24168;
loc_82A67DE0:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x829e8ab8
	ctx.lr = 0x82A67DE8;
	sub_829E8AB8(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,196
	ctx.r30.s64 = ctx.r30.s64 + 196;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x82a67de0
	if (!ctx.cr6.lt) goto loc_82A67DE0;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6792
	ctx.r3.s64 = ctx.r11.s64 + 6792;
	// bl 0x829ffa48
	ctx.lr = 0x82A67E04;
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

PPC_FUNC_IMPL(__imp__sub_82A67E20);
PPC_WEAK_FUNC(sub_82A67E20) { __imp__sub_82A67E20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67E20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6880
	ctx.r3.s64 = ctx.r11.s64 + 6880;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A67E30);
PPC_WEAK_FUNC(sub_82A67E30) { __imp__sub_82A67E30(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67E30) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31976
	ctx.r11.s64 = -2095579136;
	// addi r31,r11,-2184
	ctx.r31.s64 = ctx.r11.s64 + -2184;
	// addi r3,r31,4
	ctx.r3.s64 = ctx.r31.s64 + 4;
	// bl 0x829e95e8
	ctx.lr = 0x82A67E50;
	sub_829E95E8(ctx, base);
	// addi r3,r31,64
	ctx.r3.s64 = ctx.r31.s64 + 64;
	// bl 0x829dbba8
	ctx.lr = 0x82A67E58;
	sub_829DBBA8(ctx, base);
	// addi r3,r31,160
	ctx.r3.s64 = ctx.r31.s64 + 160;
	// bl 0x829e50d0
	ctx.lr = 0x82A67E60;
	sub_829E50D0(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// addi r3,r31,4
	ctx.r3.s64 = ctx.r31.s64 + 4;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829e9558
	ctx.lr = 0x82A67E70;
	sub_829E9558(ctx, base);
	// addi r3,r31,64
	ctx.r3.s64 = ctx.r31.s64 + 64;
	// bl 0x829db8d0
	ctx.lr = 0x82A67E78;
	sub_829DB8D0(ctx, base);
	// addi r3,r31,160
	ctx.r3.s64 = ctx.r31.s64 + 160;
	// bl 0x829e4ec8
	ctx.lr = 0x82A67E80;
	sub_829E4EC8(ctx, base);
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
	// stw r11,188(r31)
	PPC_STORE_U32(ctx.r31.u32 + 188, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A67EB0);
PPC_WEAK_FUNC(sub_82A67EB0) { __imp__sub_82A67EB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67EB0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r31,r11,-21672
	ctx.r31.s64 = ctx.r11.s64 + -21672;
	// addi r3,r31,4
	ctx.r3.s64 = ctx.r31.s64 + 4;
	// bl 0x829e95e8
	ctx.lr = 0x82A67ED0;
	sub_829E95E8(ctx, base);
	// addi r3,r31,64
	ctx.r3.s64 = ctx.r31.s64 + 64;
	// bl 0x829dbba8
	ctx.lr = 0x82A67ED8;
	sub_829DBBA8(ctx, base);
	// addi r3,r31,160
	ctx.r3.s64 = ctx.r31.s64 + 160;
	// bl 0x829e50d0
	ctx.lr = 0x82A67EE0;
	sub_829E50D0(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// addi r3,r31,4
	ctx.r3.s64 = ctx.r31.s64 + 4;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829e9558
	ctx.lr = 0x82A67EF0;
	sub_829E9558(ctx, base);
	// addi r3,r31,64
	ctx.r3.s64 = ctx.r31.s64 + 64;
	// bl 0x829db8d0
	ctx.lr = 0x82A67EF8;
	sub_829DB8D0(ctx, base);
	// addi r3,r31,160
	ctx.r3.s64 = ctx.r31.s64 + 160;
	// bl 0x829e4ec8
	ctx.lr = 0x82A67F00;
	sub_829E4EC8(ctx, base);
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
	// stw r11,188(r31)
	PPC_STORE_U32(ctx.r31.u32 + 188, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A67F30);
PPC_WEAK_FUNC(sub_82A67F30) { __imp__sub_82A67F30(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67F30) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c4
	ctx.lr = 0x82A67F38;
	__savegprlr_27(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31976
	ctx.r11.s64 = -2095579136;
	// li r29,7
	ctx.r29.s64 = 7;
	// addi r11,r11,22168
	ctx.r11.s64 = ctx.r11.s64 + 22168;
	// li r30,0
	ctx.r30.s64 = 0;
	// addi r31,r11,64
	ctx.r31.s64 = ctx.r11.s64 + 64;
	// li r27,-1
	ctx.r27.s64 = -1;
loc_82A67F54:
	// addi r28,r31,-64
	ctx.r28.s64 = ctx.r31.s64 + -64;
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// bl 0x829e95e8
	ctx.lr = 0x82A67F60;
	sub_829E95E8(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829e50d0
	ctx.lr = 0x82A67F68;
	sub_829E50D0(ctx, base);
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// stw r30,176(r31)
	PPC_STORE_U32(ctx.r31.u32 + 176, ctx.r30.u32);
	// stw r30,180(r31)
	PPC_STORE_U32(ctx.r31.u32 + 180, ctx.r30.u32);
	// bl 0x829e9558
	ctx.lr = 0x82A67F78;
	sub_829E9558(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829e4ec8
	ctx.lr = 0x82A67F80;
	sub_829E4EC8(ctx, base);
	// addi r29,r29,-1
	ctx.r29.s64 = ctx.r29.s64 + -1;
	// stw r27,20(r31)
	PPC_STORE_U32(ctx.r31.u32 + 20, ctx.r27.u32);
	// stw r27,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r27.u32);
	// cmpwi cr6,r29,0
	ctx.cr6.compare<int32_t>(ctx.r29.s32, 0, ctx.xer);
	// stw r30,24(r31)
	PPC_STORE_U32(ctx.r31.u32 + 24, ctx.r30.u32);
	// stb r30,28(r31)
	PPC_STORE_U8(ctx.r31.u32 + 28, ctx.r30.u8);
	// stb r30,44(r31)
	PPC_STORE_U8(ctx.r31.u32 + 44, ctx.r30.u8);
	// stw r30,172(r31)
	PPC_STORE_U32(ctx.r31.u32 + 172, ctx.r30.u32);
	// addi r31,r31,248
	ctx.r31.s64 = ctx.r31.s64 + 248;
	// bge cr6,0x82a67f54
	if (!ctx.cr6.lt) goto loc_82A67F54;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x829ff814
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A67FB0);
PPC_WEAK_FUNC(sub_82A67FB0) { __imp__sub_82A67FB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67FB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,7008
	ctx.r3.s64 = ctx.r11.s64 + 7008;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A67FC0);
PPC_WEAK_FUNC(sub_82A67FC0) { __imp__sub_82A67FC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67FC0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6888
	ctx.r3.s64 = ctx.r11.s64 + 6888;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A67FD0);
PPC_WEAK_FUNC(sub_82A67FD0) { __imp__sub_82A67FD0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A67FD0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31976
	ctx.r11.s64 = -2095579136;
	// addi r3,r11,-2372
	ctx.r3.s64 = ctx.r11.s64 + -2372;
	// bl 0x829e3f70
	ctx.lr = 0x82A67FE8;
	sub_829E3F70(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6896
	ctx.r3.s64 = ctx.r11.s64 + 6896;
	// bl 0x829ffa48
	ctx.lr = 0x82A67FF4;
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

PPC_FUNC_IMPL(__imp__sub_82A68008);
PPC_WEAK_FUNC(sub_82A68008) { __imp__sub_82A68008(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68008) {
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
	// li r31,79
	ctx.r31.s64 = 79;
	// addi r30,r11,-21480
	ctx.r30.s64 = ctx.r11.s64 + -21480;
loc_82A68028:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x829ee358
	ctx.lr = 0x82A68030;
	sub_829EE358(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,144
	ctx.r30.s64 = ctx.r30.s64 + 144;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x82a68028
	if (!ctx.cr6.lt) goto loc_82A68028;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,6912
	ctx.r3.s64 = ctx.r11.s64 + 6912;
	// bl 0x829ffa48
	ctx.lr = 0x82A6804C;
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

PPC_FUNC_IMPL(__imp__sub_82A68068);
PPC_WEAK_FUNC(sub_82A68068) { __imp__sub_82A68068(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68068) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r11,r11,-9784
	ctx.r11.s64 = ctx.r11.s64 + -9784;
	// li r10,16
	ctx.r10.s64 = 16;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_82A6807C:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x82a6807c
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_82A6807C;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A68090);
PPC_WEAK_FUNC(sub_82A68090) { __imp__sub_82A68090(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68090) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r5,r11,16536
	ctx.r5.s64 = ctx.r11.s64 + 16536;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-9716
	ctx.r31.s64 = ctx.r11.s64 + -9716;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A680C4;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A680D4;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A680DC;
	sub_829DC040(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,16532
	ctx.r11.s64 = ctx.r11.s64 + 16532;
	// addi r3,r10,7024
	ctx.r3.s64 = ctx.r10.s64 + 7024;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A680F4;
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

PPC_FUNC_IMPL(__imp__sub_82A68108);
PPC_WEAK_FUNC(sub_82A68108) { __imp__sub_82A68108(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68108) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r5,r11,16564
	ctx.r5.s64 = ctx.r11.s64 + 16564;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-9684
	ctx.r31.s64 = ctx.r11.s64 + -9684;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6813C;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6814C;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A68154;
	sub_829DC040(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,16560
	ctx.r11.s64 = ctx.r11.s64 + 16560;
	// addi r3,r10,7104
	ctx.r3.s64 = ctx.r10.s64 + 7104;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6816C;
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

PPC_FUNC_IMPL(__imp__sub_82A68180);
PPC_WEAK_FUNC(sub_82A68180) { __imp__sub_82A68180(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68180) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r5,r11,17276
	ctx.r5.s64 = ctx.r11.s64 + 17276;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-9580
	ctx.r31.s64 = ctx.r11.s64 + -9580;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A681B4;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A681C4;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A681CC;
	sub_829DC040(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,17272
	ctx.r11.s64 = ctx.r11.s64 + 17272;
	// addi r3,r10,7184
	ctx.r3.s64 = ctx.r10.s64 + 7184;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A681E4;
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

PPC_FUNC_IMPL(__imp__sub_82A681F8);
PPC_WEAK_FUNC(sub_82A681F8) { __imp__sub_82A681F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A681F8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r5,r11,17300
	ctx.r5.s64 = ctx.r11.s64 + 17300;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-9612
	ctx.r31.s64 = ctx.r11.s64 + -9612;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6822C;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6823C;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A68244;
	sub_829DC040(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,17296
	ctx.r11.s64 = ctx.r11.s64 + 17296;
	// addi r3,r10,7264
	ctx.r3.s64 = ctx.r10.s64 + 7264;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6825C;
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

PPC_FUNC_IMPL(__imp__sub_82A68270);
PPC_WEAK_FUNC(sub_82A68270) { __imp__sub_82A68270(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68270) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r5,r11,17332
	ctx.r5.s64 = ctx.r11.s64 + 17332;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-9644
	ctx.r31.s64 = ctx.r11.s64 + -9644;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A682A4;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A682B4;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A682BC;
	sub_829DC040(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,17328
	ctx.r11.s64 = ctx.r11.s64 + 17328;
	// addi r3,r10,7344
	ctx.r3.s64 = ctx.r10.s64 + 7344;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A682D4;
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

PPC_FUNC_IMPL(__imp__sub_82A682E8);
PPC_WEAK_FUNC(sub_82A682E8) { __imp__sub_82A682E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A682E8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r5,r11,25284
	ctx.r5.s64 = ctx.r11.s64 + 25284;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-7952
	ctx.r31.s64 = ctx.r11.s64 + -7952;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6831C;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6832C;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A68334;
	sub_829DC040(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,25280
	ctx.r11.s64 = ctx.r11.s64 + 25280;
	// addi r3,r10,7424
	ctx.r3.s64 = ctx.r10.s64 + 7424;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6834C;
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

PPC_FUNC_IMPL(__imp__sub_82A68360);
PPC_WEAK_FUNC(sub_82A68360) { __imp__sub_82A68360(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68360) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r5,r11,25308
	ctx.r5.s64 = ctx.r11.s64 + 25308;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-7888
	ctx.r31.s64 = ctx.r11.s64 + -7888;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A68394;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A683A4;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A683AC;
	sub_829DC040(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,25304
	ctx.r11.s64 = ctx.r11.s64 + 25304;
	// addi r3,r10,7504
	ctx.r3.s64 = ctx.r10.s64 + 7504;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A683C4;
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

PPC_FUNC_IMPL(__imp__sub_82A683D8);
PPC_WEAK_FUNC(sub_82A683D8) { __imp__sub_82A683D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A683D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r5,r11,25332
	ctx.r5.s64 = ctx.r11.s64 + 25332;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-7856
	ctx.r31.s64 = ctx.r11.s64 + -7856;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6840C;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6841C;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A68424;
	sub_829DC040(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,25328
	ctx.r11.s64 = ctx.r11.s64 + 25328;
	// addi r3,r10,7584
	ctx.r3.s64 = ctx.r10.s64 + 7584;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6843C;
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

PPC_FUNC_IMPL(__imp__sub_82A68450);
PPC_WEAK_FUNC(sub_82A68450) { __imp__sub_82A68450(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68450) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r5,r11,25368
	ctx.r5.s64 = ctx.r11.s64 + 25368;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-7920
	ctx.r31.s64 = ctx.r11.s64 + -7920;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A68484;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A68494;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6849C;
	sub_829DC040(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,25364
	ctx.r11.s64 = ctx.r11.s64 + 25364;
	// addi r3,r10,7664
	ctx.r3.s64 = ctx.r10.s64 + 7664;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A684B4;
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

PPC_FUNC_IMPL(__imp__sub_82A684C8);
PPC_WEAK_FUNC(sub_82A684C8) { __imp__sub_82A684C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A684C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,26056
	ctx.r5.s64 = ctx.r11.s64 + 26056;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-7824
	ctx.r3.s64 = ctx.r11.s64 + -7824;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A684E8);
PPC_WEAK_FUNC(sub_82A684E8) { __imp__sub_82A684E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A684E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,26068
	ctx.r5.s64 = ctx.r11.s64 + 26068;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-7804
	ctx.r3.s64 = ctx.r11.s64 + -7804;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A68508);
PPC_WEAK_FUNC(sub_82A68508) { __imp__sub_82A68508(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68508) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32250
	ctx.r11.s64 = -2113536000;
	// addi r4,r11,-30764
	ctx.r4.s64 = ctx.r11.s64 + -30764;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r3,r11,-6416
	ctx.r3.s64 = ctx.r11.s64 + -6416;
	// bl 0x822bca90
	ctx.lr = 0x82A68528;
	sub_822BCA90(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,7744
	ctx.r3.s64 = ctx.r11.s64 + 7744;
	// bl 0x829ffa48
	ctx.lr = 0x82A68534;
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

PPC_FUNC_IMPL(__imp__sub_82A68548);
PPC_WEAK_FUNC(sub_82A68548) { __imp__sub_82A68548(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68548) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r10,-1
	ctx.r10.s64 = -1;
	// addi r11,r11,-5456
	ctx.r11.s64 = ctx.r11.s64 + -5456;
	// lis r9,-32089
	ctx.r9.s64 = -2102984704;
	// addi r3,r9,7760
	ctx.r3.s64 = ctx.r9.s64 + 7760;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// stw r10,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r10,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r10.u32);
	// li r10,-1
	ctx.r10.s64 = -1;
	// stw r10,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r10.u32);
	// stw r10,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r10.u32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r10,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r10.u32);
	// li r10,-1
	ctx.r10.s64 = -1;
	// stw r10,24(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24, ctx.r10.u32);
	// stw r10,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r10.u32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r10,32(r11)
	PPC_STORE_U32(ctx.r11.u32 + 32, ctx.r10.u32);
	// li r10,-1
	ctx.r10.s64 = -1;
	// stw r10,36(r11)
	PPC_STORE_U32(ctx.r11.u32 + 36, ctx.r10.u32);
	// stw r10,40(r11)
	PPC_STORE_U32(ctx.r11.u32 + 40, ctx.r10.u32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r10,44(r11)
	PPC_STORE_U32(ctx.r11.u32 + 44, ctx.r10.u32);
	// li r10,-1
	ctx.r10.s64 = -1;
	// stw r10,48(r11)
	PPC_STORE_U32(ctx.r11.u32 + 48, ctx.r10.u32);
	// stw r10,52(r11)
	PPC_STORE_U32(ctx.r11.u32 + 52, ctx.r10.u32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r10,56(r11)
	PPC_STORE_U32(ctx.r11.u32 + 56, ctx.r10.u32);
	// li r10,-1
	ctx.r10.s64 = -1;
	// stw r10,60(r11)
	PPC_STORE_U32(ctx.r11.u32 + 60, ctx.r10.u32);
	// stw r10,64(r11)
	PPC_STORE_U32(ctx.r11.u32 + 64, ctx.r10.u32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r10,68(r11)
	PPC_STORE_U32(ctx.r11.u32 + 68, ctx.r10.u32);
	// li r10,-1
	ctx.r10.s64 = -1;
	// stw r10,72(r11)
	PPC_STORE_U32(ctx.r11.u32 + 72, ctx.r10.u32);
	// stw r10,76(r11)
	PPC_STORE_U32(ctx.r11.u32 + 76, ctx.r10.u32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r10,80(r11)
	PPC_STORE_U32(ctx.r11.u32 + 80, ctx.r10.u32);
	// li r10,-1
	ctx.r10.s64 = -1;
	// stw r10,84(r11)
	PPC_STORE_U32(ctx.r11.u32 + 84, ctx.r10.u32);
	// stw r10,88(r11)
	PPC_STORE_U32(ctx.r11.u32 + 88, ctx.r10.u32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r10,92(r11)
	PPC_STORE_U32(ctx.r11.u32 + 92, ctx.r10.u32);
	// li r10,-1
	ctx.r10.s64 = -1;
	// stw r10,96(r11)
	PPC_STORE_U32(ctx.r11.u32 + 96, ctx.r10.u32);
	// stw r10,100(r11)
	PPC_STORE_U32(ctx.r11.u32 + 100, ctx.r10.u32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r10,104(r11)
	PPC_STORE_U32(ctx.r11.u32 + 104, ctx.r10.u32);
	// li r10,-1
	ctx.r10.s64 = -1;
	// stw r10,108(r11)
	PPC_STORE_U32(ctx.r11.u32 + 108, ctx.r10.u32);
	// stw r10,112(r11)
	PPC_STORE_U32(ctx.r11.u32 + 112, ctx.r10.u32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r10,116(r11)
	PPC_STORE_U32(ctx.r11.u32 + 116, ctx.r10.u32);
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A68628);
PPC_WEAK_FUNC(sub_82A68628) { __imp__sub_82A68628(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68628) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// li r10,24
	ctx.r10.s64 = 24;
	// addi r11,r11,-5760
	ctx.r11.s64 = ctx.r11.s64 + -5760;
	// li r9,-1
	ctx.r9.s64 = -1;
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// li r8,0
	ctx.r8.s64 = 0;
loc_82A68640:
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
	// stw r9,-8(r11)
	PPC_STORE_U32(ctx.r11.u32 + -8, ctx.r9.u32);
	// stw r9,-4(r11)
	PPC_STORE_U32(ctx.r11.u32 + -4, ctx.r9.u32);
	// stw r8,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r8.u32);
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// addi r11,r11,12
	ctx.r11.s64 = ctx.r11.s64 + 12;
	// bge cr6,0x82a68640
	if (!ctx.cr6.lt) goto loc_82A68640;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,7768
	ctx.r3.s64 = ctx.r11.s64 + 7768;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A68668);
PPC_WEAK_FUNC(sub_82A68668) { __imp__sub_82A68668(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68668) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r5,r11,19220
	ctx.r5.s64 = ctx.r11.s64 + 19220;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-5028
	ctx.r31.s64 = ctx.r11.s64 + -5028;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6869C;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A686AC;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A686B4;
	sub_829DC040(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,19216
	ctx.r11.s64 = ctx.r11.s64 + 19216;
	// addi r3,r10,7776
	ctx.r3.s64 = ctx.r10.s64 + 7776;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A686CC;
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

PPC_FUNC_IMPL(__imp__sub_82A686E0);
PPC_WEAK_FUNC(sub_82A686E0) { __imp__sub_82A686E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A686E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r5,r11,19244
	ctx.r5.s64 = ctx.r11.s64 + 19244;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-4996
	ctx.r31.s64 = ctx.r11.s64 + -4996;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A68714;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A68724;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6872C;
	sub_829DC040(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,19240
	ctx.r11.s64 = ctx.r11.s64 + 19240;
	// addi r3,r10,7856
	ctx.r3.s64 = ctx.r10.s64 + 7856;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A68744;
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

PPC_FUNC_IMPL(__imp__sub_82A68758);
PPC_WEAK_FUNC(sub_82A68758) { __imp__sub_82A68758(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68758) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r5,r11,19268
	ctx.r5.s64 = ctx.r11.s64 + 19268;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-5092
	ctx.r31.s64 = ctx.r11.s64 + -5092;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6878C;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6879C;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A687A4;
	sub_829DC040(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,19264
	ctx.r11.s64 = ctx.r11.s64 + 19264;
	// addi r3,r10,7936
	ctx.r3.s64 = ctx.r10.s64 + 7936;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A687BC;
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

PPC_FUNC_IMPL(__imp__sub_82A687D0);
PPC_WEAK_FUNC(sub_82A687D0) { __imp__sub_82A687D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A687D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r5,r11,19292
	ctx.r5.s64 = ctx.r11.s64 + 19292;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-5188
	ctx.r31.s64 = ctx.r11.s64 + -5188;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A68804;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A68814;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6881C;
	sub_829DC040(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,19288
	ctx.r11.s64 = ctx.r11.s64 + 19288;
	// addi r3,r10,8016
	ctx.r3.s64 = ctx.r10.s64 + 8016;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A68834;
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

PPC_FUNC_IMPL(__imp__sub_82A68848);
PPC_WEAK_FUNC(sub_82A68848) { __imp__sub_82A68848(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68848) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r5,r11,19328
	ctx.r5.s64 = ctx.r11.s64 + 19328;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-5060
	ctx.r31.s64 = ctx.r11.s64 + -5060;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6887C;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6888C;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A68894;
	sub_829DC040(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,19324
	ctx.r11.s64 = ctx.r11.s64 + 19324;
	// addi r3,r10,8096
	ctx.r3.s64 = ctx.r10.s64 + 8096;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A688AC;
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

PPC_FUNC_IMPL(__imp__sub_82A688C0);
PPC_WEAK_FUNC(sub_82A688C0) { __imp__sub_82A688C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A688C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r5,r11,19368
	ctx.r5.s64 = ctx.r11.s64 + 19368;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-5156
	ctx.r31.s64 = ctx.r11.s64 + -5156;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A688F4;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A68904;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A6890C;
	sub_829DC040(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,19364
	ctx.r11.s64 = ctx.r11.s64 + 19364;
	// addi r3,r10,8176
	ctx.r3.s64 = ctx.r10.s64 + 8176;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A68924;
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

PPC_FUNC_IMPL(__imp__sub_82A68938);
PPC_WEAK_FUNC(sub_82A68938) { __imp__sub_82A68938(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A68938) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r5,r11,19396
	ctx.r5.s64 = ctx.r11.s64 + 19396;
	// lis r11,-31975
	ctx.r11.s64 = -2095513600;
	// addi r31,r11,-5124
	ctx.r31.s64 = ctx.r11.s64 + -5124;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A6896C;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// addi r11,r11,10144
	ctx.r11.s64 = ctx.r11.s64 + 10144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8216e1a8
	ctx.lr = 0x82A6897C;
	sub_8216E1A8(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A68984;
	sub_829DC040(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,19392
	ctx.r11.s64 = ctx.r11.s64 + 19392;
	// addi r3,r10,8256
	ctx.r3.s64 = ctx.r10.s64 + 8256;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A6899C;
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

