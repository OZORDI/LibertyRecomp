#include "gta4_init.h"

PPC_FUNC_IMPL(__imp__sub_82A5A2E8);
PPC_WEAK_FUNC(sub_82A5A2E8) { __imp__sub_82A5A2E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A2E8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23696
	ctx.r3.s64 = ctx.r11.s64 + -23696;
	// bl 0x8284e060
	ctx.lr = 0x82A5A304;
	sub_8284E060(ctx, base);
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// stw r3,-21788(r11)
	PPC_STORE_U32(ctx.r11.u32 + -21788, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A320);
PPC_WEAK_FUNC(sub_82A5A320) { __imp__sub_82A5A320(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A320) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23672
	ctx.r3.s64 = ctx.r11.s64 + -23672;
	// bl 0x8284e060
	ctx.lr = 0x82A5A33C;
	sub_8284E060(ctx, base);
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// stw r3,-21784(r11)
	PPC_STORE_U32(ctx.r11.u32 + -21784, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A358);
PPC_WEAK_FUNC(sub_82A5A358) { __imp__sub_82A5A358(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A358) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23648
	ctx.r3.s64 = ctx.r11.s64 + -23648;
	// bl 0x8284e060
	ctx.lr = 0x82A5A374;
	sub_8284E060(ctx, base);
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// stw r3,-21780(r11)
	PPC_STORE_U32(ctx.r11.u32 + -21780, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A390);
PPC_WEAK_FUNC(sub_82A5A390) { __imp__sub_82A5A390(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A390) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23624
	ctx.r3.s64 = ctx.r11.s64 + -23624;
	// bl 0x8284e060
	ctx.lr = 0x82A5A3AC;
	sub_8284E060(ctx, base);
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// stw r3,-21776(r11)
	PPC_STORE_U32(ctx.r11.u32 + -21776, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A3C8);
PPC_WEAK_FUNC(sub_82A5A3C8) { __imp__sub_82A5A3C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A3C8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23600
	ctx.r3.s64 = ctx.r11.s64 + -23600;
	// bl 0x8284e060
	ctx.lr = 0x82A5A3E4;
	sub_8284E060(ctx, base);
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// stw r3,-21772(r11)
	PPC_STORE_U32(ctx.r11.u32 + -21772, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A400);
PPC_WEAK_FUNC(sub_82A5A400) { __imp__sub_82A5A400(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A400) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23576
	ctx.r3.s64 = ctx.r11.s64 + -23576;
	// bl 0x8284e060
	ctx.lr = 0x82A5A41C;
	sub_8284E060(ctx, base);
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// stw r3,-21768(r11)
	PPC_STORE_U32(ctx.r11.u32 + -21768, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A438);
PPC_WEAK_FUNC(sub_82A5A438) { __imp__sub_82A5A438(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A438) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23552
	ctx.r3.s64 = ctx.r11.s64 + -23552;
	// bl 0x8284e060
	ctx.lr = 0x82A5A454;
	sub_8284E060(ctx, base);
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// stw r3,-21764(r11)
	PPC_STORE_U32(ctx.r11.u32 + -21764, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A470);
PPC_WEAK_FUNC(sub_82A5A470) { __imp__sub_82A5A470(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A470) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23528
	ctx.r3.s64 = ctx.r11.s64 + -23528;
	// bl 0x8284e060
	ctx.lr = 0x82A5A48C;
	sub_8284E060(ctx, base);
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// stw r3,-21760(r11)
	PPC_STORE_U32(ctx.r11.u32 + -21760, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A4A8);
PPC_WEAK_FUNC(sub_82A5A4A8) { __imp__sub_82A5A4A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A4A8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23504
	ctx.r3.s64 = ctx.r11.s64 + -23504;
	// bl 0x8284e060
	ctx.lr = 0x82A5A4C4;
	sub_8284E060(ctx, base);
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// stw r3,-21756(r11)
	PPC_STORE_U32(ctx.r11.u32 + -21756, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A4E0);
PPC_WEAK_FUNC(sub_82A5A4E0) { __imp__sub_82A5A4E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A4E0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23480
	ctx.r3.s64 = ctx.r11.s64 + -23480;
	// bl 0x8284e060
	ctx.lr = 0x82A5A4FC;
	sub_8284E060(ctx, base);
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// stw r3,-21752(r11)
	PPC_STORE_U32(ctx.r11.u32 + -21752, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A518);
PPC_WEAK_FUNC(sub_82A5A518) { __imp__sub_82A5A518(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A518) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23456
	ctx.r3.s64 = ctx.r11.s64 + -23456;
	// bl 0x8284e060
	ctx.lr = 0x82A5A534;
	sub_8284E060(ctx, base);
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// stw r3,-21748(r11)
	PPC_STORE_U32(ctx.r11.u32 + -21748, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A550);
PPC_WEAK_FUNC(sub_82A5A550) { __imp__sub_82A5A550(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A550) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23432
	ctx.r3.s64 = ctx.r11.s64 + -23432;
	// bl 0x8284e060
	ctx.lr = 0x82A5A56C;
	sub_8284E060(ctx, base);
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// stw r3,-21744(r11)
	PPC_STORE_U32(ctx.r11.u32 + -21744, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A588);
PPC_WEAK_FUNC(sub_82A5A588) { __imp__sub_82A5A588(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A588) {
	PPC_FUNC_PROLOGUE();
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
	ctx.lr = 0x82A5A5A4;
	sub_8284E060(ctx, base);
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// stw r3,-21740(r11)
	PPC_STORE_U32(ctx.r11.u32 + -21740, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A5C0);
PPC_WEAK_FUNC(sub_82A5A5C0) { __imp__sub_82A5A5C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A5C0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23396
	ctx.r3.s64 = ctx.r11.s64 + -23396;
	// bl 0x8284e060
	ctx.lr = 0x82A5A5DC;
	sub_8284E060(ctx, base);
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// stw r3,-21736(r11)
	PPC_STORE_U32(ctx.r11.u32 + -21736, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A5F8);
PPC_WEAK_FUNC(sub_82A5A5F8) { __imp__sub_82A5A5F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A5F8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23380
	ctx.r3.s64 = ctx.r11.s64 + -23380;
	// bl 0x8284e060
	ctx.lr = 0x82A5A614;
	sub_8284E060(ctx, base);
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// stw r3,-21732(r11)
	PPC_STORE_U32(ctx.r11.u32 + -21732, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A630);
PPC_WEAK_FUNC(sub_82A5A630) { __imp__sub_82A5A630(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A630) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23364
	ctx.r3.s64 = ctx.r11.s64 + -23364;
	// bl 0x8284e060
	ctx.lr = 0x82A5A64C;
	sub_8284E060(ctx, base);
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// stw r3,-21728(r11)
	PPC_STORE_U32(ctx.r11.u32 + -21728, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A668);
PPC_WEAK_FUNC(sub_82A5A668) { __imp__sub_82A5A668(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A668) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23348
	ctx.r3.s64 = ctx.r11.s64 + -23348;
	// bl 0x8284e060
	ctx.lr = 0x82A5A684;
	sub_8284E060(ctx, base);
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// stw r3,-21724(r11)
	PPC_STORE_U32(ctx.r11.u32 + -21724, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A6A0);
PPC_WEAK_FUNC(sub_82A5A6A0) { __imp__sub_82A5A6A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A6A0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23332
	ctx.r3.s64 = ctx.r11.s64 + -23332;
	// bl 0x8284e060
	ctx.lr = 0x82A5A6BC;
	sub_8284E060(ctx, base);
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// stw r3,-21720(r11)
	PPC_STORE_U32(ctx.r11.u32 + -21720, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A6D8);
PPC_WEAK_FUNC(sub_82A5A6D8) { __imp__sub_82A5A6D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A6D8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23316
	ctx.r3.s64 = ctx.r11.s64 + -23316;
	// bl 0x8284e060
	ctx.lr = 0x82A5A6F4;
	sub_8284E060(ctx, base);
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// stw r3,-21716(r11)
	PPC_STORE_U32(ctx.r11.u32 + -21716, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A710);
PPC_WEAK_FUNC(sub_82A5A710) { __imp__sub_82A5A710(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A710) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23300
	ctx.r3.s64 = ctx.r11.s64 + -23300;
	// bl 0x8284e060
	ctx.lr = 0x82A5A72C;
	sub_8284E060(ctx, base);
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// stw r3,-21712(r11)
	PPC_STORE_U32(ctx.r11.u32 + -21712, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A748);
PPC_WEAK_FUNC(sub_82A5A748) { __imp__sub_82A5A748(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A748) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23284
	ctx.r3.s64 = ctx.r11.s64 + -23284;
	// bl 0x8284e060
	ctx.lr = 0x82A5A764;
	sub_8284E060(ctx, base);
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// stw r3,-21708(r11)
	PPC_STORE_U32(ctx.r11.u32 + -21708, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A780);
PPC_WEAK_FUNC(sub_82A5A780) { __imp__sub_82A5A780(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A780) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,-21704
	ctx.r11.s64 = ctx.r11.s64 + -21704;
	// lis r9,-32089
	ctx.r9.s64 = -2102984704;
	// addi r3,r9,-3352
	ctx.r3.s64 = ctx.r9.s64 + -3352;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// sth r10,4(r11)
	PPC_STORE_U16(ctx.r11.u32 + 4, ctx.r10.u16);
	// sth r10,6(r11)
	PPC_STORE_U16(ctx.r11.u32 + 6, ctx.r10.u16);
	// stw r10,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r10.u32);
	// sth r10,12(r11)
	PPC_STORE_U16(ctx.r11.u32 + 12, ctx.r10.u16);
	// sth r10,14(r11)
	PPC_STORE_U16(ctx.r11.u32 + 14, ctx.r10.u16);
	// stw r10,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r10.u32);
	// sth r10,20(r11)
	PPC_STORE_U16(ctx.r11.u32 + 20, ctx.r10.u16);
	// sth r10,22(r11)
	PPC_STORE_U16(ctx.r11.u32 + 22, ctx.r10.u16);
	// stw r10,24(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24, ctx.r10.u32);
	// sth r10,28(r11)
	PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r10.u16);
	// sth r10,30(r11)
	PPC_STORE_U16(ctx.r11.u32 + 30, ctx.r10.u16);
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A7C8);
PPC_WEAK_FUNC(sub_82A5A7C8) { __imp__sub_82A5A7C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A7C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,-21672
	ctx.r11.s64 = ctx.r11.s64 + -21672;
	// lis r9,-32089
	ctx.r9.s64 = -2102984704;
	// addi r3,r9,-3248
	ctx.r3.s64 = ctx.r9.s64 + -3248;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// sth r10,4(r11)
	PPC_STORE_U16(ctx.r11.u32 + 4, ctx.r10.u16);
	// sth r10,6(r11)
	PPC_STORE_U16(ctx.r11.u32 + 6, ctx.r10.u16);
	// stw r10,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r10.u32);
	// sth r10,12(r11)
	PPC_STORE_U16(ctx.r11.u32 + 12, ctx.r10.u16);
	// sth r10,14(r11)
	PPC_STORE_U16(ctx.r11.u32 + 14, ctx.r10.u16);
	// stw r10,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r10.u32);
	// sth r10,20(r11)
	PPC_STORE_U16(ctx.r11.u32 + 20, ctx.r10.u16);
	// sth r10,22(r11)
	PPC_STORE_U16(ctx.r11.u32 + 22, ctx.r10.u16);
	// stw r10,24(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24, ctx.r10.u32);
	// sth r10,28(r11)
	PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r10.u16);
	// sth r10,30(r11)
	PPC_STORE_U16(ctx.r11.u32 + 30, ctx.r10.u16);
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A810);
PPC_WEAK_FUNC(sub_82A5A810) { __imp__sub_82A5A810(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A810) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,-21640
	ctx.r11.s64 = ctx.r11.s64 + -21640;
	// lis r9,-32089
	ctx.r9.s64 = -2102984704;
	// addi r3,r9,-3152
	ctx.r3.s64 = ctx.r9.s64 + -3152;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// sth r10,4(r11)
	PPC_STORE_U16(ctx.r11.u32 + 4, ctx.r10.u16);
	// sth r10,6(r11)
	PPC_STORE_U16(ctx.r11.u32 + 6, ctx.r10.u16);
	// stw r10,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r10.u32);
	// sth r10,12(r11)
	PPC_STORE_U16(ctx.r11.u32 + 12, ctx.r10.u16);
	// sth r10,14(r11)
	PPC_STORE_U16(ctx.r11.u32 + 14, ctx.r10.u16);
	// stw r10,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r10.u32);
	// sth r10,20(r11)
	PPC_STORE_U16(ctx.r11.u32 + 20, ctx.r10.u16);
	// sth r10,22(r11)
	PPC_STORE_U16(ctx.r11.u32 + 22, ctx.r10.u16);
	// stw r10,24(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24, ctx.r10.u32);
	// sth r10,28(r11)
	PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r10.u16);
	// sth r10,30(r11)
	PPC_STORE_U16(ctx.r11.u32 + 30, ctx.r10.u16);
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A858);
PPC_WEAK_FUNC(sub_82A5A858) { __imp__sub_82A5A858(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A858) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,-21608
	ctx.r11.s64 = ctx.r11.s64 + -21608;
	// lis r9,-32089
	ctx.r9.s64 = -2102984704;
	// addi r3,r9,-3048
	ctx.r3.s64 = ctx.r9.s64 + -3048;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// sth r10,4(r11)
	PPC_STORE_U16(ctx.r11.u32 + 4, ctx.r10.u16);
	// sth r10,6(r11)
	PPC_STORE_U16(ctx.r11.u32 + 6, ctx.r10.u16);
	// stw r10,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r10.u32);
	// sth r10,12(r11)
	PPC_STORE_U16(ctx.r11.u32 + 12, ctx.r10.u16);
	// sth r10,14(r11)
	PPC_STORE_U16(ctx.r11.u32 + 14, ctx.r10.u16);
	// stw r10,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r10.u32);
	// sth r10,20(r11)
	PPC_STORE_U16(ctx.r11.u32 + 20, ctx.r10.u16);
	// sth r10,22(r11)
	PPC_STORE_U16(ctx.r11.u32 + 22, ctx.r10.u16);
	// stw r10,24(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24, ctx.r10.u32);
	// sth r10,28(r11)
	PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r10.u16);
	// sth r10,30(r11)
	PPC_STORE_U16(ctx.r11.u32 + 30, ctx.r10.u16);
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A8A0);
PPC_WEAK_FUNC(sub_82A5A8A0) { __imp__sub_82A5A8A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A8A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// li r10,51
	ctx.r10.s64 = 51;
	// addi r11,r11,-21576
	ctx.r11.s64 = ctx.r11.s64 + -21576;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r11,r11,10
	ctx.r11.s64 = ctx.r11.s64 + 10;
loc_82A5A8B4:
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
	// stw r9,-6(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6, ctx.r9.u32);
	// sth r9,-2(r11)
	PPC_STORE_U16(ctx.r11.u32 + -2, ctx.r9.u16);
	// sth r9,0(r11)
	PPC_STORE_U16(ctx.r11.u32 + 0, ctx.r9.u16);
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// addi r11,r11,12
	ctx.r11.s64 = ctx.r11.s64 + 12;
	// bge cr6,0x82a5a8b4
	if (!ctx.cr6.lt) goto loc_82A5A8B4;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-2952
	ctx.r3.s64 = ctx.r11.s64 + -2952;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A8E0);
PPC_WEAK_FUNC(sub_82A5A8E0) { __imp__sub_82A5A8E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A8E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// li r10,54
	ctx.r10.s64 = 54;
	// addi r11,r11,-20952
	ctx.r11.s64 = ctx.r11.s64 + -20952;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r11,r11,22
	ctx.r11.s64 = ctx.r11.s64 + 22;
loc_82A5A8F4:
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
	// stw r9,-6(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6, ctx.r9.u32);
	// sth r9,-2(r11)
	PPC_STORE_U16(ctx.r11.u32 + -2, ctx.r9.u16);
	// sth r9,0(r11)
	PPC_STORE_U16(ctx.r11.u32 + 0, ctx.r9.u16);
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// addi r11,r11,24
	ctx.r11.s64 = ctx.r11.s64 + 24;
	// bge cr6,0x82a5a8f4
	if (!ctx.cr6.lt) goto loc_82A5A8F4;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-2856
	ctx.r3.s64 = ctx.r11.s64 + -2856;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A920);
PPC_WEAK_FUNC(sub_82A5A920) { __imp__sub_82A5A920(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A920) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// li r10,479
	ctx.r10.s64 = 479;
	// addi r11,r11,-19632
	ctx.r11.s64 = ctx.r11.s64 + -19632;
	// li r9,0
	ctx.r9.s64 = 0;
loc_82A5A930:
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// sth r9,64(r11)
	PPC_STORE_U16(ctx.r11.u32 + 64, ctx.r9.u16);
	// addi r11,r11,80
	ctx.r11.s64 = ctx.r11.s64 + 80;
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// bge cr6,0x82a5a930
	if (!ctx.cr6.lt) goto loc_82A5A930;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-2760
	ctx.r3.s64 = ctx.r11.s64 + -2760;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A958);
PPC_WEAK_FUNC(sub_82A5A958) { __imp__sub_82A5A958(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A958) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-2632
	ctx.r3.s64 = ctx.r11.s64 + -2632;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A968);
PPC_WEAK_FUNC(sub_82A5A968) { __imp__sub_82A5A968(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A968) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-2560
	ctx.r3.s64 = ctx.r11.s64 + -2560;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A978);
PPC_WEAK_FUNC(sub_82A5A978) { __imp__sub_82A5A978(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A978) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-2544
	ctx.r3.s64 = ctx.r11.s64 + -2544;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A988);
PPC_WEAK_FUNC(sub_82A5A988) { __imp__sub_82A5A988(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A988) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r11,r11,18776
	ctx.r11.s64 = ctx.r11.s64 + 18776;
	// li r10,34
	ctx.r10.s64 = 34;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_82A5A99C:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x82a5a99c
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_82A5A99C;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-2528
	ctx.r3.s64 = ctx.r11.s64 + -2528;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5A9B8);
PPC_WEAK_FUNC(sub_82A5A9B8) { __imp__sub_82A5A9B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5A9B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// lwz r10,-28176(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + -28176);
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// addi r11,r11,-31880
	ctx.r11.s64 = ctx.r11.s64 + -31880;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// lis r10,-32086
	ctx.r10.s64 = -2102788096;
	// lwz r10,-28172(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -28172);
	// stw r10,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
	// lis r10,-32086
	ctx.r10.s64 = -2102788096;
	// lwz r10,-28168(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -28168);
	// stw r10,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r10.u32);
	// lis r10,-32086
	ctx.r10.s64 = -2102788096;
	// lwz r10,-28164(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -28164);
	// stw r10,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r10.u32);
	// lis r10,-32086
	ctx.r10.s64 = -2102788096;
	// lwz r10,-28160(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -28160);
	// stw r10,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r10.u32);
	// lis r10,-32086
	ctx.r10.s64 = -2102788096;
	// lwz r10,-28156(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -28156);
	// stw r10,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r10.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5AA10);
PPC_WEAK_FUNC(sub_82A5AA10) { __imp__sub_82A5AA10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5AA10) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-2440
	ctx.r3.s64 = ctx.r11.s64 + -2440;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5AA20);
PPC_WEAK_FUNC(sub_82A5AA20) { __imp__sub_82A5AA20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5AA20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,18920
	ctx.r11.s64 = ctx.r11.s64 + 18920;
	// stb r10,0(r11)
	PPC_STORE_U8(ctx.r11.u32 + 0, ctx.r10.u8);
	// stb r10,1(r11)
	PPC_STORE_U8(ctx.r11.u32 + 1, ctx.r10.u8);
	// stb r10,2(r11)
	PPC_STORE_U8(ctx.r11.u32 + 2, ctx.r10.u8);
	// stb r10,3(r11)
	PPC_STORE_U8(ctx.r11.u32 + 3, ctx.r10.u8);
	// stb r10,4(r11)
	PPC_STORE_U8(ctx.r11.u32 + 4, ctx.r10.u8);
	// stb r10,5(r11)
	PPC_STORE_U8(ctx.r11.u32 + 5, ctx.r10.u8);
	// stb r10,6(r11)
	PPC_STORE_U8(ctx.r11.u32 + 6, ctx.r10.u8);
	// stb r10,7(r11)
	PPC_STORE_U8(ctx.r11.u32 + 7, ctx.r10.u8);
	// stb r10,10(r11)
	PPC_STORE_U8(ctx.r11.u32 + 10, ctx.r10.u8);
	// stb r10,11(r11)
	PPC_STORE_U8(ctx.r11.u32 + 11, ctx.r10.u8);
	// stb r10,12(r11)
	PPC_STORE_U8(ctx.r11.u32 + 12, ctx.r10.u8);
	// stb r10,13(r11)
	PPC_STORE_U8(ctx.r11.u32 + 13, ctx.r10.u8);
	// stb r10,14(r11)
	PPC_STORE_U8(ctx.r11.u32 + 14, ctx.r10.u8);
	// stb r10,15(r11)
	PPC_STORE_U8(ctx.r11.u32 + 15, ctx.r10.u8);
	// stb r10,18(r11)
	PPC_STORE_U8(ctx.r11.u32 + 18, ctx.r10.u8);
	// stb r10,19(r11)
	PPC_STORE_U8(ctx.r11.u32 + 19, ctx.r10.u8);
	// stb r10,20(r11)
	PPC_STORE_U8(ctx.r11.u32 + 20, ctx.r10.u8);
	// stb r10,21(r11)
	PPC_STORE_U8(ctx.r11.u32 + 21, ctx.r10.u8);
	// stb r10,22(r11)
	PPC_STORE_U8(ctx.r11.u32 + 22, ctx.r10.u8);
	// stb r10,23(r11)
	PPC_STORE_U8(ctx.r11.u32 + 23, ctx.r10.u8);
	// stb r10,24(r11)
	PPC_STORE_U8(ctx.r11.u32 + 24, ctx.r10.u8);
	// stb r10,25(r11)
	PPC_STORE_U8(ctx.r11.u32 + 25, ctx.r10.u8);
	// stb r10,26(r11)
	PPC_STORE_U8(ctx.r11.u32 + 26, ctx.r10.u8);
	// stb r10,27(r11)
	PPC_STORE_U8(ctx.r11.u32 + 27, ctx.r10.u8);
	// stb r10,30(r11)
	PPC_STORE_U8(ctx.r11.u32 + 30, ctx.r10.u8);
	// stb r10,31(r11)
	PPC_STORE_U8(ctx.r11.u32 + 31, ctx.r10.u8);
	// stb r10,32(r11)
	PPC_STORE_U8(ctx.r11.u32 + 32, ctx.r10.u8);
	// stb r10,33(r11)
	PPC_STORE_U8(ctx.r11.u32 + 33, ctx.r10.u8);
	// stb r10,34(r11)
	PPC_STORE_U8(ctx.r11.u32 + 34, ctx.r10.u8);
	// stb r10,35(r11)
	PPC_STORE_U8(ctx.r11.u32 + 35, ctx.r10.u8);
	// stb r10,38(r11)
	PPC_STORE_U8(ctx.r11.u32 + 38, ctx.r10.u8);
	// stb r10,39(r11)
	PPC_STORE_U8(ctx.r11.u32 + 39, ctx.r10.u8);
	// stb r10,40(r11)
	PPC_STORE_U8(ctx.r11.u32 + 40, ctx.r10.u8);
	// stb r10,41(r11)
	PPC_STORE_U8(ctx.r11.u32 + 41, ctx.r10.u8);
	// stb r10,42(r11)
	PPC_STORE_U8(ctx.r11.u32 + 42, ctx.r10.u8);
	// stb r10,43(r11)
	PPC_STORE_U8(ctx.r11.u32 + 43, ctx.r10.u8);
	// stb r10,44(r11)
	PPC_STORE_U8(ctx.r11.u32 + 44, ctx.r10.u8);
	// stb r10,45(r11)
	PPC_STORE_U8(ctx.r11.u32 + 45, ctx.r10.u8);
	// stb r10,46(r11)
	PPC_STORE_U8(ctx.r11.u32 + 46, ctx.r10.u8);
	// stb r10,47(r11)
	PPC_STORE_U8(ctx.r11.u32 + 47, ctx.r10.u8);
	// stb r10,50(r11)
	PPC_STORE_U8(ctx.r11.u32 + 50, ctx.r10.u8);
	// stb r10,51(r11)
	PPC_STORE_U8(ctx.r11.u32 + 51, ctx.r10.u8);
	// stb r10,52(r11)
	PPC_STORE_U8(ctx.r11.u32 + 52, ctx.r10.u8);
	// stb r10,53(r11)
	PPC_STORE_U8(ctx.r11.u32 + 53, ctx.r10.u8);
	// stb r10,54(r11)
	PPC_STORE_U8(ctx.r11.u32 + 54, ctx.r10.u8);
	// stb r10,55(r11)
	PPC_STORE_U8(ctx.r11.u32 + 55, ctx.r10.u8);
	// stb r10,58(r11)
	PPC_STORE_U8(ctx.r11.u32 + 58, ctx.r10.u8);
	// stb r10,59(r11)
	PPC_STORE_U8(ctx.r11.u32 + 59, ctx.r10.u8);
	// stb r10,60(r11)
	PPC_STORE_U8(ctx.r11.u32 + 60, ctx.r10.u8);
	// stb r10,61(r11)
	PPC_STORE_U8(ctx.r11.u32 + 61, ctx.r10.u8);
	// stb r10,62(r11)
	PPC_STORE_U8(ctx.r11.u32 + 62, ctx.r10.u8);
	// stb r10,63(r11)
	PPC_STORE_U8(ctx.r11.u32 + 63, ctx.r10.u8);
	// stb r10,64(r11)
	PPC_STORE_U8(ctx.r11.u32 + 64, ctx.r10.u8);
	// stb r10,65(r11)
	PPC_STORE_U8(ctx.r11.u32 + 65, ctx.r10.u8);
	// stb r10,66(r11)
	PPC_STORE_U8(ctx.r11.u32 + 66, ctx.r10.u8);
	// stb r10,67(r11)
	PPC_STORE_U8(ctx.r11.u32 + 67, ctx.r10.u8);
	// stb r10,70(r11)
	PPC_STORE_U8(ctx.r11.u32 + 70, ctx.r10.u8);
	// stb r10,71(r11)
	PPC_STORE_U8(ctx.r11.u32 + 71, ctx.r10.u8);
	// stb r10,72(r11)
	PPC_STORE_U8(ctx.r11.u32 + 72, ctx.r10.u8);
	// stb r10,73(r11)
	PPC_STORE_U8(ctx.r11.u32 + 73, ctx.r10.u8);
	// stb r10,74(r11)
	PPC_STORE_U8(ctx.r11.u32 + 74, ctx.r10.u8);
	// stb r10,75(r11)
	PPC_STORE_U8(ctx.r11.u32 + 75, ctx.r10.u8);
	// stb r10,78(r11)
	PPC_STORE_U8(ctx.r11.u32 + 78, ctx.r10.u8);
	// stb r10,79(r11)
	PPC_STORE_U8(ctx.r11.u32 + 79, ctx.r10.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5AB30);
PPC_WEAK_FUNC(sub_82A5AB30) { __imp__sub_82A5AB30(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5AB30) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// addi r3,r11,19000
	ctx.r3.s64 = ctx.r11.s64 + 19000;
	// bl 0x82259640
	ctx.lr = 0x82A5AB48;
	sub_82259640(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-2408
	ctx.r3.s64 = ctx.r11.s64 + -2408;
	// bl 0x829ffa48
	ctx.lr = 0x82A5AB54;
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

PPC_FUNC_IMPL(__imp__sub_82A5AB68);
PPC_WEAK_FUNC(sub_82A5AB68) { __imp__sub_82A5AB68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5AB68) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// addi r3,r11,19012
	ctx.r3.s64 = ctx.r11.s64 + 19012;
	// bl 0x82259640
	ctx.lr = 0x82A5AB80;
	sub_82259640(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-2392
	ctx.r3.s64 = ctx.r11.s64 + -2392;
	// bl 0x829ffa48
	ctx.lr = 0x82A5AB8C;
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

PPC_FUNC_IMPL(__imp__sub_82A5ABA0);
PPC_WEAK_FUNC(sub_82A5ABA0) { __imp__sub_82A5ABA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5ABA0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-2376
	ctx.r3.s64 = ctx.r11.s64 + -2376;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5ABB0);
PPC_WEAK_FUNC(sub_82A5ABB0) { __imp__sub_82A5ABB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5ABB0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7cc
	ctx.lr = 0x82A5ABB8;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// li r31,15
	ctx.r31.s64 = 15;
	// addi r29,r11,19032
	ctx.r29.s64 = ctx.r11.s64 + 19032;
	// mr r30,r29
	ctx.r30.u64 = ctx.r29.u64;
loc_82A5ABCC:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x829db688
	ctx.lr = 0x82A5ABD4;
	sub_829DB688(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,8
	ctx.r30.s64 = ctx.r30.s64 + 8;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x82a5abcc
	if (!ctx.cr6.lt) goto loc_82A5ABCC;
	// li r11,0
	ctx.r11.s64 = 0;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r3,r10,-2288
	ctx.r3.s64 = ctx.r10.s64 + -2288;
	// stw r11,128(r29)
	PPC_STORE_U32(ctx.r29.u32 + 128, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A5ABF8;
	sub_829FFA48(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5AC00);
PPC_WEAK_FUNC(sub_82A5AC00) { __imp__sub_82A5AC00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5AC00) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-2280
	ctx.r3.s64 = ctx.r11.s64 + -2280;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5AC10);
PPC_WEAK_FUNC(sub_82A5AC10) { __imp__sub_82A5AC10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5AC10) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-2272
	ctx.r3.s64 = ctx.r11.s64 + -2272;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5AC20);
PPC_WEAK_FUNC(sub_82A5AC20) { __imp__sub_82A5AC20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5AC20) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// addi r3,r11,19784
	ctx.r3.s64 = ctx.r11.s64 + 19784;
	// bl 0x829de7a8
	ctx.lr = 0x82A5AC38;
	sub_829DE7A8(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-2264
	ctx.r3.s64 = ctx.r11.s64 + -2264;
	// bl 0x829ffa48
	ctx.lr = 0x82A5AC44;
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

PPC_FUNC_IMPL(__imp__sub_82A5AC58);
PPC_WEAK_FUNC(sub_82A5AC58) { __imp__sub_82A5AC58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5AC58) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-7296
	ctx.r6.s64 = ctx.r11.s64 + -7296;
	// lis r11,-32244
	ctx.r11.s64 = -2113142784;
	// addi r5,r11,-13448
	ctx.r5.s64 = ctx.r11.s64 + -13448;
	// lis r11,-31970
	ctx.r11.s64 = -2095185920;
	// addi r31,r11,19812
	ctx.r31.s64 = ctx.r11.s64 + 19812;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x829dbfd0
	ctx.lr = 0x82A5AC90;
	sub_829DBFD0(ctx, base);
	// lis r11,-32251
	ctx.r11.s64 = -2113601536;
	// lis r10,-31970
	ctx.r10.s64 = -2095185920;
	// addi r9,r11,10144
	ctx.r9.s64 = ctx.r11.s64 + 10144;
	// lwz r11,22032(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 22032);
	// stw r9,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r9.u32);
	// lis r9,-31970
	ctx.r9.s64 = -2095185920;
	// clrlwi r8,r11,31
	ctx.r8.u64 = ctx.r11.u32 & 0x1;
	// addi r30,r9,22008
	ctx.r30.s64 = ctx.r9.s64 + 22008;
	// cmplwi cr6,r8,0
	ctx.cr6.compare<uint32_t>(ctx.r8.u32, 0, ctx.xer);
	// bne cr6,0x82a5acd4
	if (!ctx.cr6.eq) goto loc_82A5ACD4;
	// ori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 | 1;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// stw r11,22032(r10)
	PPC_STORE_U32(ctx.r10.u32 + 22032, ctx.r11.u32);
	// bl 0x829dc2f8
	ctx.lr = 0x82A5ACC8;
	sub_829DC2F8(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-2232
	ctx.r3.s64 = ctx.r11.s64 + -2232;
	// bl 0x829ffa48
	ctx.lr = 0x82A5ACD4;
	sub_829FFA48(ctx, base);
loc_82A5ACD4:
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x829dc040
	ctx.lr = 0x82A5ACE0;
	sub_829DC040(ctx, base);
	// lis r11,-32244
	ctx.r11.s64 = -2113142784;
	// lis r10,-32089
	ctx.r10.s64 = -2102984704;
	// addi r11,r11,-13416
	ctx.r11.s64 = ctx.r11.s64 + -13416;
	// addi r3,r10,-2248
	ctx.r3.s64 = ctx.r10.s64 + -2248;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x829ffa48
	ctx.lr = 0x82A5ACF8;
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

PPC_FUNC_IMPL(__imp__sub_82A5AD10);
PPC_WEAK_FUNC(sub_82A5AD10) { __imp__sub_82A5AD10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5AD10) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-2216
	ctx.r3.s64 = ctx.r11.s64 + -2216;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5AD20);
PPC_WEAK_FUNC(sub_82A5AD20) { __imp__sub_82A5AD20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5AD20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-2184
	ctx.r3.s64 = ctx.r11.s64 + -2184;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5AD30);
PPC_WEAK_FUNC(sub_82A5AD30) { __imp__sub_82A5AD30(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5AD30) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-2152
	ctx.r3.s64 = ctx.r11.s64 + -2152;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5AD40);
PPC_WEAK_FUNC(sub_82A5AD40) { __imp__sub_82A5AD40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5AD40) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-2096
	ctx.r3.s64 = ctx.r11.s64 + -2096;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5AD50);
PPC_WEAK_FUNC(sub_82A5AD50) { __imp__sub_82A5AD50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5AD50) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r3,21440
	ctx.r3.s64 = 21440;
	// bl 0x821b3510
	ctx.lr = 0x82A5AD68;
	sub_821B3510(ctx, base);
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// addi r31,r11,-31972
	ctx.r31.s64 = ctx.r11.s64 + -31972;
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// li r3,16
	ctx.r3.s64 = 16;
	// bl 0x821b3510
	ctx.lr = 0x82A5AD7C;
	sub_821B3510(ctx, base);
	// li r9,1
	ctx.r9.s64 = 1;
	// stw r3,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r3.u32);
	// mr r11,r9
	ctx.r11.u64 = ctx.r9.u64;
	// stb r11,24(r31)
	PPC_STORE_U8(ctx.r31.u32 + 24, ctx.r11.u8);
	// li r11,16
	ctx.r11.s64 = 16;
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
	// b 0x82a5adac
	goto loc_82A5ADAC;
loc_82A5ADA8:
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4);
loc_82A5ADAC:
	// lbzx r10,r3,r11
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r3.u32 + ctx.r11.u32);
	// ori r10,r10,128
	ctx.r10.u64 = ctx.r10.u64 | 128;
	// stbx r10,r3,r11
	PPC_STORE_U8(ctx.r3.u32 + ctx.r11.u32, ctx.r10.u8);
	// lwz r10,4(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4);
	// lbzx r8,r10,r11
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r10.u32 + ctx.r11.u32);
	// rlwimi r8,r9,0,25,23
	ctx.r8.u64 = (__builtin_rotateleft32(ctx.r9.u32, 0) & 0xFFFFFFFFFFFFFF7F) | (ctx.r8.u64 & 0x80);
	// stbx r8,r10,r11
	PPC_STORE_U8(ctx.r10.u32 + ctx.r11.u32, ctx.r8.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// cmpwi cr6,r11,16
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 16, ctx.xer);
	// blt cr6,0x82a5ada8
	if (ctx.cr6.lt) goto loc_82A5ADA8;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-1952
	ctx.r3.s64 = ctx.r11.s64 + -1952;
	// bl 0x829ffa48
	ctx.lr = 0x82A5ADE0;
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

PPC_FUNC_IMPL(__imp__sub_82A5ADF8);
PPC_WEAK_FUNC(sub_82A5ADF8) { __imp__sub_82A5ADF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5ADF8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// addi r11,r11,-31944
	ctx.r11.s64 = ctx.r11.s64 + -31944;
	// addi r3,r11,4
	ctx.r3.s64 = ctx.r11.s64 + 4;
	// bl 0x82a74d74
	ctx.lr = 0x82A5AE14;
	__imp__RtlInitializeCriticalSection(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-1744
	ctx.r3.s64 = ctx.r11.s64 + -1744;
	// bl 0x829ffa48
	ctx.lr = 0x82A5AE20;
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

PPC_FUNC_IMPL(__imp__sub_82A5AE30);
PPC_WEAK_FUNC(sub_82A5AE30) { __imp__sub_82A5AE30(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5AE30) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// addi r11,r11,-31912
	ctx.r11.s64 = ctx.r11.s64 + -31912;
	// addi r3,r11,4
	ctx.r3.s64 = ctx.r11.s64 + 4;
	// bl 0x82a74d74
	ctx.lr = 0x82A5AE4C;
	__imp__RtlInitializeCriticalSection(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-1720
	ctx.r3.s64 = ctx.r11.s64 + -1720;
	// bl 0x829ffa48
	ctx.lr = 0x82A5AE58;
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

PPC_FUNC_IMPL(__imp__sub_82A5AE68);
PPC_WEAK_FUNC(sub_82A5AE68) { __imp__sub_82A5AE68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5AE68) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// bl 0x821b3770
	ctx.lr = 0x82A5AE78;
	sub_821B3770(ctx, base);
	// lwz r11,0(r13)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r13.u32 + 0);
	// li r10,1676
	ctx.r10.s64 = 1676;
	// lwzx r3,r10,r11
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,44(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x82A5AE94;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-1648
	ctx.r3.s64 = ctx.r11.s64 + -1648;
	// bl 0x829ffa48
	ctx.lr = 0x82A5AEA0;
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

PPC_FUNC_IMPL(__imp__sub_82A5AEB0);
PPC_WEAK_FUNC(sub_82A5AEB0) { __imp__sub_82A5AEB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5AEB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,2568
	ctx.r5.s64 = ctx.r11.s64 + 2568;
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-24848
	ctx.r3.s64 = ctx.r11.s64 + -24848;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5AED0);
PPC_WEAK_FUNC(sub_82A5AED0) { __imp__sub_82A5AED0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5AED0) {
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
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// li r31,3
	ctx.r31.s64 = 3;
	// addi r30,r11,-24808
	ctx.r30.s64 = ctx.r11.s64 + -24808;
loc_82A5AEF0:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x822094c8
	ctx.lr = 0x82A5AEF8;
	sub_822094C8(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,188
	ctx.r30.s64 = ctx.r30.s64 + 188;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x82a5aef0
	if (!ctx.cr6.lt) goto loc_82A5AEF0;
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

PPC_FUNC_IMPL(__imp__sub_82A5AF20);
PPC_WEAK_FUNC(sub_82A5AF20) { __imp__sub_82A5AF20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5AF20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// addi r3,r11,-24056
	ctx.r3.s64 = ctx.r11.s64 + -24056;
	// b 0x822094c8
	sub_822094C8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5AF30);
PPC_WEAK_FUNC(sub_82A5AF30) { __imp__sub_82A5AF30(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5AF30) {
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
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// li r31,3
	ctx.r31.s64 = 3;
	// addi r30,r11,-23824
	ctx.r30.s64 = ctx.r11.s64 + -23824;
loc_82A5AF50:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x822b5958
	ctx.lr = 0x82A5AF58;
	sub_822B5958(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,4220
	ctx.r30.s64 = ctx.r30.s64 + 4220;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x82a5af50
	if (!ctx.cr6.lt) goto loc_82A5AF50;
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

PPC_FUNC_IMPL(__imp__sub_82A5AF80);
PPC_WEAK_FUNC(sub_82A5AF80) { __imp__sub_82A5AF80(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5AF80) {
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
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-13680
	ctx.r5.s64 = ctx.r11.s64 + -13680;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A5AFA8;
	sub_8284D220(ctx, base);
	// lis r9,-32236
	ctx.r9.s64 = -2112618496;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r9,r9,14840
	ctx.r9.s64 = ctx.r9.s64 + 14840;
	// lis r10,-32077
	ctx.r10.s64 = -2102198272;
	// addi r10,r10,-6944
	ctx.r10.s64 = ctx.r10.s64 + -6944;
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

PPC_FUNC_IMPL(__imp__sub_82A5AFF0);
PPC_WEAK_FUNC(sub_82A5AFF0) { __imp__sub_82A5AFF0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5AFF0) {
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
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-13680
	ctx.r5.s64 = ctx.r11.s64 + -13680;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8284d220
	ctx.lr = 0x82A5B018;
	sub_8284D220(ctx, base);
	// lis r9,-32236
	ctx.r9.s64 = -2112618496;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r9,r9,14840
	ctx.r9.s64 = ctx.r9.s64 + 14840;
	// lis r10,-32077
	ctx.r10.s64 = -2102198272;
	// addi r10,r10,-6928
	ctx.r10.s64 = ctx.r10.s64 + -6928;
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

PPC_FUNC_IMPL(__imp__sub_82A5B060);
PPC_WEAK_FUNC(sub_82A5B060) { __imp__sub_82A5B060(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B060) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-1560
	ctx.r3.s64 = ctx.r11.s64 + -1560;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B070);
PPC_WEAK_FUNC(sub_82A5B070) { __imp__sub_82A5B070(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B070) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-1536
	ctx.r3.s64 = ctx.r11.s64 + -1536;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B080);
PPC_WEAK_FUNC(sub_82A5B080) { __imp__sub_82A5B080(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B080) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-1512
	ctx.r3.s64 = ctx.r11.s64 + -1512;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B090);
PPC_WEAK_FUNC(sub_82A5B090) { __imp__sub_82A5B090(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B090) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-1488
	ctx.r3.s64 = ctx.r11.s64 + -1488;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B0A0);
PPC_WEAK_FUNC(sub_82A5B0A0) { __imp__sub_82A5B0A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B0A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-1464
	ctx.r3.s64 = ctx.r11.s64 + -1464;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B0B0);
PPC_WEAK_FUNC(sub_82A5B0B0) { __imp__sub_82A5B0B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B0B0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// addi r3,r11,-6776
	ctx.r3.s64 = ctx.r11.s64 + -6776;
	// bl 0x8285f370
	ctx.lr = 0x82A5B0C8;
	sub_8285F370(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-1608
	ctx.r3.s64 = ctx.r11.s64 + -1608;
	// bl 0x829ffa48
	ctx.lr = 0x82A5B0D4;
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

PPC_FUNC_IMPL(__imp__sub_82A5B0E8);
PPC_WEAK_FUNC(sub_82A5B0E8) { __imp__sub_82A5B0E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B0E8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// addi r3,r11,-5632
	ctx.r3.s64 = ctx.r11.s64 + -5632;
	// bl 0x8285f370
	ctx.lr = 0x82A5B100;
	sub_8285F370(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-1592
	ctx.r3.s64 = ctx.r11.s64 + -1592;
	// bl 0x829ffa48
	ctx.lr = 0x82A5B10C;
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

PPC_FUNC_IMPL(__imp__sub_82A5B120);
PPC_WEAK_FUNC(sub_82A5B120) { __imp__sub_82A5B120(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B120) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// addi r3,r11,-4488
	ctx.r3.s64 = ctx.r11.s64 + -4488;
	// bl 0x8285f370
	ctx.lr = 0x82A5B138;
	sub_8285F370(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-1576
	ctx.r3.s64 = ctx.r11.s64 + -1576;
	// bl 0x829ffa48
	ctx.lr = 0x82A5B144;
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

PPC_FUNC_IMPL(__imp__sub_82A5B158);
PPC_WEAK_FUNC(sub_82A5B158) { __imp__sub_82A5B158(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B158) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,3280
	ctx.r5.s64 = ctx.r11.s64 + 3280;
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-3344
	ctx.r3.s64 = ctx.r11.s64 + -3344;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B178);
PPC_WEAK_FUNC(sub_82A5B178) { __imp__sub_82A5B178(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B178) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,3288
	ctx.r5.s64 = ctx.r11.s64 + 3288;
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-3324
	ctx.r3.s64 = ctx.r11.s64 + -3324;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B198);
PPC_WEAK_FUNC(sub_82A5B198) { __imp__sub_82A5B198(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B198) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,3300
	ctx.r5.s64 = ctx.r11.s64 + 3300;
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-3304
	ctx.r3.s64 = ctx.r11.s64 + -3304;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B1B8);
PPC_WEAK_FUNC(sub_82A5B1B8) { __imp__sub_82A5B1B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B1B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,3312
	ctx.r5.s64 = ctx.r11.s64 + 3312;
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-3284
	ctx.r3.s64 = ctx.r11.s64 + -3284;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B1D8);
PPC_WEAK_FUNC(sub_82A5B1D8) { __imp__sub_82A5B1D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B1D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,3324
	ctx.r5.s64 = ctx.r11.s64 + 3324;
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-3264
	ctx.r3.s64 = ctx.r11.s64 + -3264;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B1F8);
PPC_WEAK_FUNC(sub_82A5B1F8) { __imp__sub_82A5B1F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B1F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,3336
	ctx.r5.s64 = ctx.r11.s64 + 3336;
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-3244
	ctx.r3.s64 = ctx.r11.s64 + -3244;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B218);
PPC_WEAK_FUNC(sub_82A5B218) { __imp__sub_82A5B218(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B218) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,3348
	ctx.r5.s64 = ctx.r11.s64 + 3348;
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-3224
	ctx.r3.s64 = ctx.r11.s64 + -3224;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B238);
PPC_WEAK_FUNC(sub_82A5B238) { __imp__sub_82A5B238(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B238) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,3364
	ctx.r5.s64 = ctx.r11.s64 + 3364;
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-3204
	ctx.r3.s64 = ctx.r11.s64 + -3204;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B258);
PPC_WEAK_FUNC(sub_82A5B258) { __imp__sub_82A5B258(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B258) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,3376
	ctx.r5.s64 = ctx.r11.s64 + 3376;
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-3184
	ctx.r3.s64 = ctx.r11.s64 + -3184;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B278);
PPC_WEAK_FUNC(sub_82A5B278) { __imp__sub_82A5B278(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B278) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,3552
	ctx.r5.s64 = ctx.r11.s64 + 3552;
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-2120
	ctx.r3.s64 = ctx.r11.s64 + -2120;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B298);
PPC_WEAK_FUNC(sub_82A5B298) { __imp__sub_82A5B298(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B298) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,3560
	ctx.r5.s64 = ctx.r11.s64 + 3560;
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-2100
	ctx.r3.s64 = ctx.r11.s64 + -2100;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B2B8);
PPC_WEAK_FUNC(sub_82A5B2B8) { __imp__sub_82A5B2B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B2B8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// addi r3,r11,-2080
	ctx.r3.s64 = ctx.r11.s64 + -2080;
	// bl 0x821b56d0
	ctx.lr = 0x82A5B2D0;
	sub_821B56D0(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-1424
	ctx.r3.s64 = ctx.r11.s64 + -1424;
	// bl 0x829ffa48
	ctx.lr = 0x82A5B2DC;
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

PPC_FUNC_IMPL(__imp__sub_82A5B2F0);
PPC_WEAK_FUNC(sub_82A5B2F0) { __imp__sub_82A5B2F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B2F0) {
	PPC_FUNC_PROLOGUE();
loc_82A5B2F0:
	// mftb r11
	ctx.r11.u64 = PPC_QUERY_TIMEBASE();
	// rotlwi r10,r11,0
	ctx.r10.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x82a5b2f0
	if (ctx.cr6.eq) goto loc_82A5B2F0;
	// lis r10,-32077
	ctx.r10.s64 = -2102198272;
	// std r11,6008(r10)
	PPC_STORE_U64(ctx.r10.u32 + 6008, ctx.r11.u64);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B310);
PPC_WEAK_FUNC(sub_82A5B310) { __imp__sub_82A5B310(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B310) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// addi r3,r11,6016
	ctx.r3.s64 = ctx.r11.s64 + 6016;
	// bl 0x8285fe48
	ctx.lr = 0x82A5B328;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-1440
	ctx.r3.s64 = ctx.r11.s64 + -1440;
	// bl 0x829ffa48
	ctx.lr = 0x82A5B334;
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

PPC_FUNC_IMPL(__imp__sub_82A5B348);
PPC_WEAK_FUNC(sub_82A5B348) { __imp__sub_82A5B348(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B348) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// addi r3,r11,6496
	ctx.r3.s64 = ctx.r11.s64 + 6496;
	// bl 0x821b8ed0
	ctx.lr = 0x82A5B360;
	sub_821B8ED0(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-1344
	ctx.r3.s64 = ctx.r11.s64 + -1344;
	// bl 0x829ffa48
	ctx.lr = 0x82A5B36C;
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

PPC_FUNC_IMPL(__imp__sub_82A5B380);
PPC_WEAK_FUNC(sub_82A5B380) { __imp__sub_82A5B380(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B380) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,3828
	ctx.r5.s64 = ctx.r11.s64 + 3828;
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,28256
	ctx.r3.s64 = ctx.r11.s64 + 28256;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B3A0);
PPC_WEAK_FUNC(sub_82A5B3A0) { __imp__sub_82A5B3A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B3A0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// addi r3,r11,28304
	ctx.r3.s64 = ctx.r11.s64 + 28304;
	// bl 0x821baa50
	ctx.lr = 0x82A5B3B8;
	sub_821BAA50(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-1240
	ctx.r3.s64 = ctx.r11.s64 + -1240;
	// bl 0x829ffa48
	ctx.lr = 0x82A5B3C4;
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

PPC_FUNC_IMPL(__imp__sub_82A5B3D8);
PPC_WEAK_FUNC(sub_82A5B3D8) { __imp__sub_82A5B3D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B3D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32077
	ctx.r11.s64 = -2102198272;
	// addi r11,r11,28276
	ctx.r11.s64 = ctx.r11.s64 + 28276;
	// addi r3,r11,20
	ctx.r3.s64 = ctx.r11.s64 + 20;
	// bl 0x824f3278
	ctx.lr = 0x82A5B3F4;
	sub_824F3278(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-1248
	ctx.r3.s64 = ctx.r11.s64 + -1248;
	// bl 0x829ffa48
	ctx.lr = 0x82A5B400;
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

PPC_FUNC_IMPL(__imp__sub_82A5B410);
PPC_WEAK_FUNC(sub_82A5B410) { __imp__sub_82A5B410(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B410) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-1200
	ctx.r3.s64 = ctx.r11.s64 + -1200;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B420);
PPC_WEAK_FUNC(sub_82A5B420) { __imp__sub_82A5B420(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B420) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-1224
	ctx.r3.s64 = ctx.r11.s64 + -1224;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B430);
PPC_WEAK_FUNC(sub_82A5B430) { __imp__sub_82A5B430(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B430) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32076
	ctx.r11.s64 = -2102132736;
	// addi r3,r11,-27584
	ctx.r3.s64 = ctx.r11.s64 + -27584;
	// b 0x8285d0d8
	sub_8285D0D8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B440);
PPC_WEAK_FUNC(sub_82A5B440) { __imp__sub_82A5B440(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B440) {
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
	// li r31,10
	ctx.r31.s64 = 10;
	// addi r30,r11,-27312
	ctx.r30.s64 = ctx.r11.s64 + -27312;
loc_82A5B460:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82267d90
	ctx.lr = 0x82A5B468;
	sub_82267D90(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,96
	ctx.r30.s64 = ctx.r30.s64 + 96;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x82a5b460
	if (!ctx.cr6.lt) goto loc_82A5B460;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-1136
	ctx.r3.s64 = ctx.r11.s64 + -1136;
	// bl 0x829ffa48
	ctx.lr = 0x82A5B484;
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

PPC_FUNC_IMPL(__imp__sub_82A5B4A0);
PPC_WEAK_FUNC(sub_82A5B4A0) { __imp__sub_82A5B4A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B4A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-1048
	ctx.r3.s64 = ctx.r11.s64 + -1048;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B4B0);
PPC_WEAK_FUNC(sub_82A5B4B0) { __imp__sub_82A5B4B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B4B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-1016
	ctx.r3.s64 = ctx.r11.s64 + -1016;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B4C0);
PPC_WEAK_FUNC(sub_82A5B4C0) { __imp__sub_82A5B4C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B4C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-1008
	ctx.r3.s64 = ctx.r11.s64 + -1008;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B4D0);
PPC_WEAK_FUNC(sub_82A5B4D0) { __imp__sub_82A5B4D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B4D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-1000
	ctx.r3.s64 = ctx.r11.s64 + -1000;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B4E0);
PPC_WEAK_FUNC(sub_82A5B4E0) { __imp__sub_82A5B4E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B4E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-992
	ctx.r3.s64 = ctx.r11.s64 + -992;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B4F0);
PPC_WEAK_FUNC(sub_82A5B4F0) { __imp__sub_82A5B4F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B4F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-984
	ctx.r3.s64 = ctx.r11.s64 + -984;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B500);
PPC_WEAK_FUNC(sub_82A5B500) { __imp__sub_82A5B500(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B500) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-976
	ctx.r3.s64 = ctx.r11.s64 + -976;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B510);
PPC_WEAK_FUNC(sub_82A5B510) { __imp__sub_82A5B510(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B510) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-968
	ctx.r3.s64 = ctx.r11.s64 + -968;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B520);
PPC_WEAK_FUNC(sub_82A5B520) { __imp__sub_82A5B520(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B520) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-952
	ctx.r3.s64 = ctx.r11.s64 + -952;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B530);
PPC_WEAK_FUNC(sub_82A5B530) { __imp__sub_82A5B530(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B530) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-960
	ctx.r3.s64 = ctx.r11.s64 + -960;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B540);
PPC_WEAK_FUNC(sub_82A5B540) { __imp__sub_82A5B540(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B540) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31973
	ctx.r11.s64 = -2095382528;
	// addi r11,r11,-18816
	ctx.r11.s64 = ctx.r11.s64 + -18816;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-32074
	ctx.r11.s64 = -2102001664;
	// addi r11,r11,-23408
	ctx.r11.s64 = ctx.r11.s64 + -23408;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B560);
PPC_WEAK_FUNC(sub_82A5B560) { __imp__sub_82A5B560(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B560) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-920
	ctx.r3.s64 = ctx.r11.s64 + -920;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B570);
PPC_WEAK_FUNC(sub_82A5B570) { __imp__sub_82A5B570(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B570) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// lis r10,-32073
	ctx.r10.s64 = -2101936128;
	// lbz r11,-31648(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + -31648);
	// stb r11,17328(r10)
	PPC_STORE_U8(ctx.r10.u32 + 17328, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B588);
PPC_WEAK_FUNC(sub_82A5B588) { __imp__sub_82A5B588(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B588) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// addi r3,r11,20384
	ctx.r3.s64 = ctx.r11.s64 + 20384;
	// bl 0x828be650
	ctx.lr = 0x82A5B5A0;
	sub_828BE650(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-888
	ctx.r3.s64 = ctx.r11.s64 + -888;
	// bl 0x829ffa48
	ctx.lr = 0x82A5B5AC;
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

PPC_FUNC_IMPL(__imp__sub_82A5B5C0);
PPC_WEAK_FUNC(sub_82A5B5C0) { __imp__sub_82A5B5C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B5C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-872
	ctx.r3.s64 = ctx.r11.s64 + -872;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B5D0);
PPC_WEAK_FUNC(sub_82A5B5D0) { __imp__sub_82A5B5D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B5D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32071
	ctx.r11.s64 = -2101805056;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,30720
	ctx.r11.s64 = ctx.r11.s64 + 30720;
	// lis r9,-32089
	ctx.r9.s64 = -2102984704;
	// addi r3,r9,-856
	ctx.r3.s64 = ctx.r9.s64 + -856;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// stw r10,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
	// stw r10,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r10.u32);
	// stw r10,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r10.u32);
	// stw r10,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r10.u32);
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B600);
PPC_WEAK_FUNC(sub_82A5B600) { __imp__sub_82A5B600(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B600) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32071
	ctx.r11.s64 = -2101805056;
	// addi r3,r11,30896
	ctx.r3.s64 = ctx.r11.s64 + 30896;
	// bl 0x82370e70
	ctx.lr = 0x82A5B618;
	sub_82370E70(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-768
	ctx.r3.s64 = ctx.r11.s64 + -768;
	// bl 0x829ffa48
	ctx.lr = 0x82A5B624;
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

PPC_FUNC_IMPL(__imp__sub_82A5B638);
PPC_WEAK_FUNC(sub_82A5B638) { __imp__sub_82A5B638(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B638) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// li r10,215
	ctx.r10.s64 = 215;
	// addi r11,r11,-31640
	ctx.r11.s64 = ctx.r11.s64 + -31640;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r11,r11,326
	ctx.r11.s64 = ctx.r11.s64 + 326;
loc_82A5B64C:
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
	// stw r9,-6(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6, ctx.r9.u32);
	// sth r9,-2(r11)
	PPC_STORE_U16(ctx.r11.u32 + -2, ctx.r9.u16);
	// sth r9,0(r11)
	PPC_STORE_U16(ctx.r11.u32 + 0, ctx.r9.u16);
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// bge cr6,0x82a5b64c
	if (!ctx.cr6.lt) goto loc_82A5B64C;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-752
	ctx.r3.s64 = ctx.r11.s64 + -752;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B678);
PPC_WEAK_FUNC(sub_82A5B678) { __imp__sub_82A5B678(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B678) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lbz r11,-31648(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + -31648);
	// stb r11,-31647(r10)
	PPC_STORE_U8(ctx.r10.u32 + -31647, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B690);
PPC_WEAK_FUNC(sub_82A5B690) { __imp__sub_82A5B690(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B690) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lbz r11,-31648(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + -31648);
	// stb r11,-31646(r10)
	PPC_STORE_U8(ctx.r10.u32 + -31646, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B6A8);
PPC_WEAK_FUNC(sub_82A5B6A8) { __imp__sub_82A5B6A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B6A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r11,r11,-24496
	ctx.r11.s64 = ctx.r11.s64 + -24496;
	// li r10,42
	ctx.r10.s64 = 42;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_82A5B6BC:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x82a5b6bc
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_82A5B6BC;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-720
	ctx.r3.s64 = ctx.r11.s64 + -720;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B6D8);
PPC_WEAK_FUNC(sub_82A5B6D8) { __imp__sub_82A5B6D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B6D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r3,r11,-24328
	ctx.r3.s64 = ctx.r11.s64 + -24328;
	// bl 0x8285fe48
	ctx.lr = 0x82A5B6F0;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-736
	ctx.r3.s64 = ctx.r11.s64 + -736;
	// bl 0x829ffa48
	ctx.lr = 0x82A5B6FC;
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

PPC_FUNC_IMPL(__imp__sub_82A5B710);
PPC_WEAK_FUNC(sub_82A5B710) { __imp__sub_82A5B710(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B710) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// li r10,-1
	ctx.r10.s64 = -1;
	// addi r11,r11,-24296
	ctx.r11.s64 = ctx.r11.s64 + -24296;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// stw r10,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r10.u32);
	// stw r10,48(r11)
	PPC_STORE_U32(ctx.r11.u32 + 48, ctx.r10.u32);
	// stw r10,68(r11)
	PPC_STORE_U32(ctx.r11.u32 + 68, ctx.r10.u32);
	// stw r10,96(r11)
	PPC_STORE_U32(ctx.r11.u32 + 96, ctx.r10.u32);
	// stw r10,116(r11)
	PPC_STORE_U32(ctx.r11.u32 + 116, ctx.r10.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B738);
PPC_WEAK_FUNC(sub_82A5B738) { __imp__sub_82A5B738(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B738) {
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
	// li r31,39
	ctx.r31.s64 = 39;
	// addi r11,r11,-24108
	ctx.r11.s64 = ctx.r11.s64 + -24108;
	// addi r30,r11,92
	ctx.r30.s64 = ctx.r11.s64 + 92;
loc_82A5B75C:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82544b78
	ctx.lr = 0x82A5B764;
	sub_82544B78(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,108
	ctx.r30.s64 = ctx.r30.s64 + 108;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x82a5b75c
	if (!ctx.cr6.lt) goto loc_82A5B75C;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-632
	ctx.r3.s64 = ctx.r11.s64 + -632;
	// bl 0x829ffa48
	ctx.lr = 0x82A5B780;
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

PPC_FUNC_IMPL(__imp__sub_82A5B798);
PPC_WEAK_FUNC(sub_82A5B798) { __imp__sub_82A5B798(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B798) {
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
	// li r31,19
	ctx.r31.s64 = 19;
	// addi r30,r11,-19784
	ctx.r30.s64 = ctx.r11.s64 + -19784;
loc_82A5B7B8:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82544b58
	ctx.lr = 0x82A5B7C0;
	sub_82544B58(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,72
	ctx.r30.s64 = ctx.r30.s64 + 72;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x82a5b7b8
	if (!ctx.cr6.lt) goto loc_82A5B7B8;
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

PPC_FUNC_IMPL(__imp__sub_82A5B7E8);
PPC_WEAK_FUNC(sub_82A5B7E8) { __imp__sub_82A5B7E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B7E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15144
	ctx.r5.s64 = ctx.r11.s64 + 15144;
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,7616
	ctx.r3.s64 = ctx.r11.s64 + 7616;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B808);
PPC_WEAK_FUNC(sub_82A5B808) { __imp__sub_82A5B808(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B808) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15160
	ctx.r5.s64 = ctx.r11.s64 + 15160;
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,7636
	ctx.r3.s64 = ctx.r11.s64 + 7636;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B828);
PPC_WEAK_FUNC(sub_82A5B828) { __imp__sub_82A5B828(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B828) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// li r9,255
	ctx.r9.s64 = 255;
	// addi r11,r11,32256
	ctx.r11.s64 = ctx.r11.s64 + 32256;
	// addi r10,r11,48
	ctx.r10.s64 = ctx.r11.s64 + 48;
	// li r11,-1
	ctx.r11.s64 = -1;
loc_82A5B83C:
	// stw r11,-28(r10)
	PPC_STORE_U32(ctx.r10.u32 + -28, ctx.r11.u32);
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// stw r11,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r11.u32);
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// addi r10,r10,124
	ctx.r10.s64 = ctx.r10.s64 + 124;
	// bge cr6,0x82a5b83c
	if (!ctx.cr6.lt) goto loc_82A5B83C;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B860);
PPC_WEAK_FUNC(sub_82A5B860) { __imp__sub_82A5B860(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B860) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r3,r11,7656
	ctx.r3.s64 = ctx.r11.s64 + 7656;
	// b 0x82204ad0
	sub_82204AD0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B870);
PPC_WEAK_FUNC(sub_82A5B870) { __imp__sub_82A5B870(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B870) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15180
	ctx.r5.s64 = ctx.r11.s64 + 15180;
	// lis r11,-32069
	ctx.r11.s64 = -2101673984;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-1460
	ctx.r3.s64 = ctx.r11.s64 + -1460;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B890);
PPC_WEAK_FUNC(sub_82A5B890) { __imp__sub_82A5B890(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B890) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-560
	ctx.r3.s64 = ctx.r11.s64 + -560;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B8A0);
PPC_WEAK_FUNC(sub_82A5B8A0) { __imp__sub_82A5B8A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B8A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32068
	ctx.r11.s64 = -2101608448;
	// li r5,3200
	ctx.r5.s64 = 3200;
	// addi r3,r11,31552
	ctx.r3.s64 = ctx.r11.s64 + 31552;
	// li r4,0
	ctx.r4.s64 = 0;
	// b 0x829ff840
	sub_829FF840(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B8B8);
PPC_WEAK_FUNC(sub_82A5B8B8) { __imp__sub_82A5B8B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B8B8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32067
	ctx.r10.s64 = -2101542912;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// lis r9,-32067
	ctx.r9.s64 = -2101542912;
	// addi r11,r11,-22784
	ctx.r11.s64 = ctx.r11.s64 + -22784;
	// lwz r10,-18688(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -18688);
	// rlwinm r10,r10,11,0,20
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 11) & 0xFFFFF800;
	// add r11,r10,r11
	ctx.r11.u64 = ctx.r10.u64 + ctx.r11.u64;
	// stw r11,-18656(r9)
	PPC_STORE_U32(ctx.r9.u32 + -18656, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B8E0);
PPC_WEAK_FUNC(sub_82A5B8E0) { __imp__sub_82A5B8E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B8E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// addi r3,r11,-18632
	ctx.r3.s64 = ctx.r11.s64 + -18632;
	// b 0x829dbba8
	sub_829DBBA8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B8F0);
PPC_WEAK_FUNC(sub_82A5B8F0) { __imp__sub_82A5B8F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B8F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15928
	ctx.r5.s64 = ctx.r11.s64 + 15928;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-18652
	ctx.r3.s64 = ctx.r11.s64 + -18652;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B910);
PPC_WEAK_FUNC(sub_82A5B910) { __imp__sub_82A5B910(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B910) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15944
	ctx.r5.s64 = ctx.r11.s64 + 15944;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-18536
	ctx.r3.s64 = ctx.r11.s64 + -18536;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B930);
PPC_WEAK_FUNC(sub_82A5B930) { __imp__sub_82A5B930(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B930) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15964
	ctx.r5.s64 = ctx.r11.s64 + 15964;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-18516
	ctx.r3.s64 = ctx.r11.s64 + -18516;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B950);
PPC_WEAK_FUNC(sub_82A5B950) { __imp__sub_82A5B950(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B950) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// lis r10,-32067
	ctx.r10.s64 = -2101542912;
	// lwz r11,20684(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20684);
	// stw r11,-18496(r10)
	PPC_STORE_U32(ctx.r10.u32 + -18496, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B968);
PPC_WEAK_FUNC(sub_82A5B968) { __imp__sub_82A5B968(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B968) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// addi r3,r11,-18492
	ctx.r3.s64 = ctx.r11.s64 + -18492;
	// bl 0x8285fe48
	ctx.lr = 0x82A5B980;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-552
	ctx.r3.s64 = ctx.r11.s64 + -552;
	// bl 0x829ffa48
	ctx.lr = 0x82A5B98C;
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

PPC_FUNC_IMPL(__imp__sub_82A5B9A0);
PPC_WEAK_FUNC(sub_82A5B9A0) { __imp__sub_82A5B9A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B9A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// addi r3,r11,-18456
	ctx.r3.s64 = ctx.r11.s64 + -18456;
	// b 0x829dbba8
	sub_829DBBA8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B9B0);
PPC_WEAK_FUNC(sub_82A5B9B0) { __imp__sub_82A5B9B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B9B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-536
	ctx.r3.s64 = ctx.r11.s64 + -536;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5B9C0);
PPC_WEAK_FUNC(sub_82A5B9C0) { __imp__sub_82A5B9C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B9C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// addi r3,r11,-15880
	ctx.r3.s64 = ctx.r11.s64 + -15880;
	// bl 0x82215ce0
	ctx.lr = 0x82A5B9D8;
	sub_82215CE0(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-520
	ctx.r3.s64 = ctx.r11.s64 + -520;
	// bl 0x829ffa48
	ctx.lr = 0x82A5B9E4;
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

PPC_FUNC_IMPL(__imp__sub_82A5B9F8);
PPC_WEAK_FUNC(sub_82A5B9F8) { __imp__sub_82A5B9F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5B9F8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// addi r3,r11,-14856
	ctx.r3.s64 = ctx.r11.s64 + -14856;
	// bl 0x8290c330
	ctx.lr = 0x82A5BA10;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-528
	ctx.r3.s64 = ctx.r11.s64 + -528;
	// bl 0x829ffa48
	ctx.lr = 0x82A5BA1C;
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

PPC_FUNC_IMPL(__imp__sub_82A5BA30);
PPC_WEAK_FUNC(sub_82A5BA30) { __imp__sub_82A5BA30(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BA30) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,18712
	ctx.r3.s64 = ctx.r11.s64 + 18712;
	// bl 0x8284e060
	ctx.lr = 0x82A5BA4C;
	sub_8284E060(ctx, base);
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// stw r3,-14816(r11)
	PPC_STORE_U32(ctx.r11.u32 + -14816, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BA68);
PPC_WEAK_FUNC(sub_82A5BA68) { __imp__sub_82A5BA68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BA68) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,18740
	ctx.r3.s64 = ctx.r11.s64 + 18740;
	// bl 0x8284e060
	ctx.lr = 0x82A5BA84;
	sub_8284E060(ctx, base);
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// stw r3,-14812(r11)
	PPC_STORE_U32(ctx.r11.u32 + -14812, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BAA0);
PPC_WEAK_FUNC(sub_82A5BAA0) { __imp__sub_82A5BAA0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BAA0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,18768
	ctx.r3.s64 = ctx.r11.s64 + 18768;
	// bl 0x8284e060
	ctx.lr = 0x82A5BABC;
	sub_8284E060(ctx, base);
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// stw r3,-14808(r11)
	PPC_STORE_U32(ctx.r11.u32 + -14808, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BAD8);
PPC_WEAK_FUNC(sub_82A5BAD8) { __imp__sub_82A5BAD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BAD8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-496
	ctx.r3.s64 = ctx.r11.s64 + -496;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BAE8);
PPC_WEAK_FUNC(sub_82A5BAE8) { __imp__sub_82A5BAE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BAE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,19364
	ctx.r5.s64 = ctx.r11.s64 + 19364;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-9816
	ctx.r3.s64 = ctx.r11.s64 + -9816;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BB08);
PPC_WEAK_FUNC(sub_82A5BB08) { __imp__sub_82A5BB08(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BB08) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7cc
	ctx.lr = 0x82A5BB10;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// li r30,31
	ctx.r30.s64 = 31;
	// addi r11,r11,-9792
	ctx.r11.s64 = ctx.r11.s64 + -9792;
	// li r29,-1
	ctx.r29.s64 = -1;
	// addi r31,r11,196
	ctx.r31.s64 = ctx.r11.s64 + 196;
loc_82A5BB28:
	// lis r11,-32236
	ctx.r11.s64 = -2112618496;
	// stw r29,-196(r31)
	PPC_STORE_U32(ctx.r31.u32 + -196, ctx.r29.u32);
	// li r5,7
	ctx.r5.s64 = 7;
	// addi r6,r11,1672
	ctx.r6.s64 = ctx.r11.s64 + 1672;
	// li r4,4
	ctx.r4.s64 = 4;
	// addi r3,r31,-56
	ctx.r3.s64 = ctx.r31.s64 + -56;
	// bl 0x82140220
	ctx.lr = 0x82A5BB44;
	sub_82140220(ctx, base);
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// li r5,7
	ctx.r5.s64 = 7;
	// addi r6,r11,-13680
	ctx.r6.s64 = ctx.r11.s64 + -13680;
	// li r4,8
	ctx.r4.s64 = 8;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82140220
	ctx.lr = 0x82A5BB5C;
	sub_82140220(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// addi r31,r31,252
	ctx.r31.s64 = ctx.r31.s64 + 252;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a5bb28
	if (!ctx.cr6.lt) goto loc_82A5BB28;
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BB78);
PPC_WEAK_FUNC(sub_82A5BB78) { __imp__sub_82A5BB78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BB78) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// addi r3,r11,-1640
	ctx.r3.s64 = ctx.r11.s64 + -1640;
	// bl 0x8221fa08
	ctx.lr = 0x82A5BB90;
	sub_8221FA08(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-488
	ctx.r3.s64 = ctx.r11.s64 + -488;
	// bl 0x829ffa48
	ctx.lr = 0x82A5BB9C;
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

PPC_FUNC_IMPL(__imp__sub_82A5BBB0);
PPC_WEAK_FUNC(sub_82A5BBB0) { __imp__sub_82A5BBB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BBB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r4,r11,20412
	ctx.r4.s64 = ctx.r11.s64 + 20412;
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// addi r3,r11,-9920
	ctx.r3.s64 = ctx.r11.s64 + -9920;
	// b 0x822d24c0
	sub_822D24C0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BBC8);
PPC_WEAK_FUNC(sub_82A5BBC8) { __imp__sub_82A5BBC8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BBC8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r4,r11,20424
	ctx.r4.s64 = ctx.r11.s64 + 20424;
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// addi r3,r11,-9908
	ctx.r3.s64 = ctx.r11.s64 + -9908;
	// b 0x822d24c0
	sub_822D24C0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BBE0);
PPC_WEAK_FUNC(sub_82A5BBE0) { __imp__sub_82A5BBE0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BBE0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r4,r11,20436
	ctx.r4.s64 = ctx.r11.s64 + 20436;
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// addi r3,r11,-9896
	ctx.r3.s64 = ctx.r11.s64 + -9896;
	// b 0x822d24c0
	sub_822D24C0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BBF8);
PPC_WEAK_FUNC(sub_82A5BBF8) { __imp__sub_82A5BBF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BBF8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r4,r11,20452
	ctx.r4.s64 = ctx.r11.s64 + 20452;
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// addi r3,r11,-9884
	ctx.r3.s64 = ctx.r11.s64 + -9884;
	// b 0x822d24c0
	sub_822D24C0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BC10);
PPC_WEAK_FUNC(sub_82A5BC10) { __imp__sub_82A5BC10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BC10) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r4,r11,20472
	ctx.r4.s64 = ctx.r11.s64 + 20472;
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// addi r3,r11,-9872
	ctx.r3.s64 = ctx.r11.s64 + -9872;
	// b 0x822d24c0
	sub_822D24C0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BC28);
PPC_WEAK_FUNC(sub_82A5BC28) { __imp__sub_82A5BC28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BC28) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// addi r31,r11,-1472
	ctx.r31.s64 = ctx.r11.s64 + -1472;
	// addi r3,r31,64
	ctx.r3.s64 = ctx.r31.s64 + 64;
	// bl 0x824f3278
	ctx.lr = 0x82A5BC48;
	sub_824F3278(ctx, base);
	// addi r3,r31,72
	ctx.r3.s64 = ctx.r31.s64 + 72;
	// bl 0x824f3278
	ctx.lr = 0x82A5BC50;
	sub_824F3278(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// addi r3,r31,52
	ctx.r3.s64 = ctx.r31.s64 + 52;
	// stb r11,60(r31)
	PPC_STORE_U8(ctx.r31.u32 + 60, ctx.r11.u8);
	// bl 0x8239c0e8
	ctx.lr = 0x82A5BC60;
	sub_8239C0E8(ctx, base);
	// addi r3,r31,80
	ctx.r3.s64 = ctx.r31.s64 + 80;
	// bl 0x828be650
	ctx.lr = 0x82A5BC68;
	sub_828BE650(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stb r11,1116(r31)
	PPC_STORE_U8(ctx.r31.u32 + 1116, ctx.r11.u8);
	// stw r11,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r11.u32);
	// stw r11,20(r31)
	PPC_STORE_U32(ctx.r31.u32 + 20, ctx.r11.u32);
	// stw r11,24(r31)
	PPC_STORE_U32(ctx.r31.u32 + 24, ctx.r11.u32);
	// stw r11,28(r31)
	PPC_STORE_U32(ctx.r31.u32 + 28, ctx.r11.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// stw r11,32(r31)
	PPC_STORE_U32(ctx.r31.u32 + 32, ctx.r11.u32);
	// bl 0x8222e4b8
	ctx.lr = 0x82A5BC90;
	sub_8222E4B8(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-472
	ctx.r3.s64 = ctx.r11.s64 + -472;
	// bl 0x829ffa48
	ctx.lr = 0x82A5BC9C;
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

PPC_FUNC_IMPL(__imp__sub_82A5BCB0);
PPC_WEAK_FUNC(sub_82A5BCB0) { __imp__sub_82A5BCB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BCB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// li r10,255
	ctx.r10.s64 = 255;
	// addi r11,r11,3680
	ctx.r11.s64 = ctx.r11.s64 + 3680;
	// li r9,0
	ctx.r9.s64 = 0;
loc_82A5BCC0:
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// bge cr6,0x82a5bcc0
	if (!ctx.cr6.lt) goto loc_82A5BCC0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BCE0);
PPC_WEAK_FUNC(sub_82A5BCE0) { __imp__sub_82A5BCE0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BCE0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r11,r11,5728
	ctx.r11.s64 = ctx.r11.s64 + 5728;
	// li r10,128
	ctx.r10.s64 = 128;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_82A5BCF4:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x82a5bcf4
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_82A5BCF4;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-456
	ctx.r3.s64 = ctx.r11.s64 + -456;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BD10);
PPC_WEAK_FUNC(sub_82A5BD10) { __imp__sub_82A5BD10(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BD10) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7cc
	ctx.lr = 0x82A5BD18;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// li r30,1
	ctx.r30.s64 = 1;
	// addi r31,r11,6272
	ctx.r31.s64 = ctx.r11.s64 + 6272;
	// li r29,-1
	ctx.r29.s64 = -1;
loc_82A5BD2C:
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// li r5,200
	ctx.r5.s64 = 200;
	// addi r6,r11,-13680
	ctx.r6.s64 = ctx.r11.s64 + -13680;
	// li r4,16
	ctx.r4.s64 = 16;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82140220
	ctx.lr = 0x82A5BD44;
	sub_82140220(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// stw r29,3444(r31)
	PPC_STORE_U32(ctx.r31.u32 + 3444, ctx.r29.u32);
	// addi r31,r31,3504
	ctx.r31.s64 = ctx.r31.s64 + 3504;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x82a5bd2c
	if (!ctx.cr6.lt) goto loc_82A5BD2C;
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BD60);
PPC_WEAK_FUNC(sub_82A5BD60) { __imp__sub_82A5BD60(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BD60) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,22112
	ctx.r5.s64 = ctx.r11.s64 + 22112;
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,14604
	ctx.r3.s64 = ctx.r11.s64 + 14604;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BD80);
PPC_WEAK_FUNC(sub_82A5BD80) { __imp__sub_82A5BD80(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BD80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-352
	ctx.r3.s64 = ctx.r11.s64 + -352;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BD90);
PPC_WEAK_FUNC(sub_82A5BD90) { __imp__sub_82A5BD90(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BD90) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// lfs f0,5288(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 5288);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// stfs f0,14624(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 14624, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BDA8);
PPC_WEAK_FUNC(sub_82A5BDA8) { __imp__sub_82A5BDA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BDA8) {
	PPC_FUNC_PROLOGUE();
loc_82A5BDA8:
	// mftb r11
	ctx.r11.u64 = PPC_QUERY_TIMEBASE();
	// rotlwi r10,r11,0
	ctx.r10.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x82a5bda8
	if (ctx.cr6.eq) goto loc_82A5BDA8;
	// lis r10,-32065
	ctx.r10.s64 = -2101411840;
	// std r11,14632(r10)
	PPC_STORE_U64(ctx.r10.u32 + 14632, ctx.r11.u64);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BDC8);
PPC_WEAK_FUNC(sub_82A5BDC8) { __imp__sub_82A5BDC8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BDC8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,27156
	ctx.r5.s64 = ctx.r11.s64 + 27156;
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-26512
	ctx.r3.s64 = ctx.r11.s64 + -26512;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BDE8);
PPC_WEAK_FUNC(sub_82A5BDE8) { __imp__sub_82A5BDE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BDE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,27172
	ctx.r5.s64 = ctx.r11.s64 + 27172;
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-26492
	ctx.r3.s64 = ctx.r11.s64 + -26492;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BE08);
PPC_WEAK_FUNC(sub_82A5BE08) { __imp__sub_82A5BE08(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BE08) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// addi r3,r11,-26472
	ctx.r3.s64 = ctx.r11.s64 + -26472;
	// b 0x829db688
	sub_829DB688(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BE18);
PPC_WEAK_FUNC(sub_82A5BE18) { __imp__sub_82A5BE18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BE18) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-344
	ctx.r3.s64 = ctx.r11.s64 + -344;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BE28);
PPC_WEAK_FUNC(sub_82A5BE28) { __imp__sub_82A5BE28(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BE28) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-248
	ctx.r3.s64 = ctx.r11.s64 + -248;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BE38);
PPC_WEAK_FUNC(sub_82A5BE38) { __imp__sub_82A5BE38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BE38) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-312
	ctx.r3.s64 = ctx.r11.s64 + -312;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BE48);
PPC_WEAK_FUNC(sub_82A5BE48) { __imp__sub_82A5BE48(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BE48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-232
	ctx.r3.s64 = ctx.r11.s64 + -232;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BE58);
PPC_WEAK_FUNC(sub_82A5BE58) { __imp__sub_82A5BE58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BE58) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-216
	ctx.r3.s64 = ctx.r11.s64 + -216;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BE68);
PPC_WEAK_FUNC(sub_82A5BE68) { __imp__sub_82A5BE68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BE68) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-280
	ctx.r3.s64 = ctx.r11.s64 + -280;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BE78);
PPC_WEAK_FUNC(sub_82A5BE78) { __imp__sub_82A5BE78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BE78) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// li r9,10
	ctx.r9.s64 = 10;
	// addi r11,r11,-24248
	ctx.r11.s64 = ctx.r11.s64 + -24248;
	// li r10,-1
	ctx.r10.s64 = -1;
	// addi r11,r11,44
	ctx.r11.s64 = ctx.r11.s64 + 44;
loc_82A5BE8C:
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r10,-4(r11)
	PPC_STORE_U32(ctx.r11.u32 + -4, ctx.r10.u32);
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r11,r11,48
	ctx.r11.s64 = ctx.r11.s64 + 48;
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// bge cr6,0x82a5be8c
	if (!ctx.cr6.lt) goto loc_82A5BE8C;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BEA8);
PPC_WEAK_FUNC(sub_82A5BEA8) { __imp__sub_82A5BEA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BEA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,-23704
	ctx.r11.s64 = ctx.r11.s64 + -23704;
	// lis r9,-32089
	ctx.r9.s64 = -2102984704;
	// addi r3,r9,-200
	ctx.r3.s64 = ctx.r9.s64 + -200;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// stw r10,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
	// stw r10,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r10.u32);
	// stw r10,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r10.u32);
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BED0);
PPC_WEAK_FUNC(sub_82A5BED0) { __imp__sub_82A5BED0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BED0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-112
	ctx.r3.s64 = ctx.r11.s64 + -112;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BEE0);
PPC_WEAK_FUNC(sub_82A5BEE0) { __imp__sub_82A5BEE0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BEE0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-28120
	ctx.r5.s64 = ctx.r11.s64 + -28120;
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-23684
	ctx.r3.s64 = ctx.r11.s64 + -23684;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BF00);
PPC_WEAK_FUNC(sub_82A5BF00) { __imp__sub_82A5BF00(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BF00) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// addi r3,r11,-23528
	ctx.r3.s64 = ctx.r11.s64 + -23528;
	// bl 0x82259668
	ctx.lr = 0x82A5BF18;
	sub_82259668(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-96
	ctx.r3.s64 = ctx.r11.s64 + -96;
	// bl 0x829ffa48
	ctx.lr = 0x82A5BF24;
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

PPC_FUNC_IMPL(__imp__sub_82A5BF38);
PPC_WEAK_FUNC(sub_82A5BF38) { __imp__sub_82A5BF38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BF38) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7cc
	ctx.lr = 0x82A5BF40;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// li r30,7
	ctx.r30.s64 = 7;
	// addi r11,r11,-23024
	ctx.r11.s64 = ctx.r11.s64 + -23024;
	// li r29,-1
	ctx.r29.s64 = -1;
	// addi r31,r11,4
	ctx.r31.s64 = ctx.r11.s64 + 4;
loc_82A5BF58:
	// lis r11,-32212
	ctx.r11.s64 = -2111045632;
	// li r5,5
	ctx.r5.s64 = 5;
	// addi r6,r11,-13680
	ctx.r6.s64 = ctx.r11.s64 + -13680;
	// li r4,64
	ctx.r4.s64 = 64;
	// addi r3,r31,12
	ctx.r3.s64 = ctx.r31.s64 + 12;
	// bl 0x82140220
	ctx.lr = 0x82A5BF70;
	sub_82140220(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// stw r29,-4(r31)
	PPC_STORE_U32(ctx.r31.u32 + -4, ctx.r29.u32);
	// stw r29,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r29.u32);
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// addi r31,r31,336
	ctx.r31.s64 = ctx.r31.s64 + 336;
	// bge cr6,0x82a5bf58
	if (!ctx.cr6.lt) goto loc_82A5BF58;
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BF90);
PPC_WEAK_FUNC(sub_82A5BF90) { __imp__sub_82A5BF90(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BF90) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32064
	ctx.r11.s64 = -2101346304;
	// li r10,14399
	ctx.r10.s64 = 14399;
	// addi r11,r11,7424
	ctx.r11.s64 = ctx.r11.s64 + 7424;
	// li r9,0
	ctx.r9.s64 = 0;
loc_82A5BFA0:
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// addi r11,r11,12
	ctx.r11.s64 = ctx.r11.s64 + 12;
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// bge cr6,0x82a5bfa0
	if (!ctx.cr6.lt) goto loc_82A5BFA0;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,16
	ctx.r3.s64 = ctx.r11.s64 + 16;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5BFC8);
PPC_WEAK_FUNC(sub_82A5BFC8) { __imp__sub_82A5BFC8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5BFC8) {
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
	// li r31,255
	ctx.r31.s64 = 255;
	// addi r30,r11,-16384
	ctx.r30.s64 = ctx.r11.s64 + -16384;
loc_82A5BFE8:
	// lis r11,-32141
	ctx.r11.s64 = -2106392576;
	// li r5,5
	ctx.r5.s64 = 5;
	// addi r6,r11,-2592
	ctx.r6.s64 = ctx.r11.s64 + -2592;
	// li r4,4
	ctx.r4.s64 = 4;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82140220
	ctx.lr = 0x82A5C000;
	sub_82140220(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,20
	ctx.r30.s64 = ctx.r30.s64 + 20;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x82a5bfe8
	if (!ctx.cr6.lt) goto loc_82A5BFE8;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,112
	ctx.r3.s64 = ctx.r11.s64 + 112;
	// bl 0x829ffa48
	ctx.lr = 0x82A5C01C;
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

PPC_FUNC_IMPL(__imp__sub_82A5C038);
PPC_WEAK_FUNC(sub_82A5C038) { __imp__sub_82A5C038(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C038) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r11,r11,-11264
	ctx.r11.s64 = ctx.r11.s64 + -11264;
	// li r10,900
	ctx.r10.s64 = 900;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_82A5C04C:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x82a5c04c
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_82A5C04C;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,-88
	ctx.r3.s64 = ctx.r11.s64 + -88;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5C068);
PPC_WEAK_FUNC(sub_82A5C068) { __imp__sub_82A5C068(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C068) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,0
	ctx.r3.s64 = ctx.r11.s64 + 0;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5C078);
PPC_WEAK_FUNC(sub_82A5C078) { __imp__sub_82A5C078(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C078) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,216
	ctx.r3.s64 = ctx.r11.s64 + 216;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5C088);
PPC_WEAK_FUNC(sub_82A5C088) { __imp__sub_82A5C088(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C088) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// li r9,-1
	ctx.r9.s64 = -1;
	// addi r11,r11,9840
	ctx.r11.s64 = ctx.r11.s64 + 9840;
	// li r10,512
	ctx.r10.s64 = 512;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_82A5C09C:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x82a5c09c
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_82A5C09C;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5C0B0);
PPC_WEAK_FUNC(sub_82A5C0B0) { __imp__sub_82A5C0B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C0B0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// addi r3,r11,11904
	ctx.r3.s64 = ctx.r11.s64 + 11904;
	// bl 0x82265310
	ctx.lr = 0x82A5C0C8;
	sub_82265310(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,248
	ctx.r3.s64 = ctx.r11.s64 + 248;
	// bl 0x829ffa48
	ctx.lr = 0x82A5C0D4;
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

PPC_FUNC_IMPL(__imp__sub_82A5C0E8);
PPC_WEAK_FUNC(sub_82A5C0E8) { __imp__sub_82A5C0E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C0E8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32059
	ctx.r11.s64 = -2101018624;
	// addi r7,r10,-18832
	ctx.r7.s64 = ctx.r10.s64 + -18832;
	// lis r6,-32256
	ctx.r6.s64 = -2113929216;
	// addi r11,r11,32240
	ctx.r11.s64 = ctx.r11.s64 + 32240;
	// lis r8,0
	ctx.r8.s64 = 0;
	// li r9,15
	ctx.r9.s64 = 15;
	// lvx128 v0,r0,r7
	ea = (ctx.r7.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r11,32
	ctx.r11.s64 = ctx.r11.s64 + 32;
	// lfs f0,2612(r6)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// li r10,0
	ctx.r10.s64 = 0;
	// li r4,-16
	ctx.r4.s64 = -16;
	// li r5,16
	ctx.r5.s64 = 16;
	// ori r8,r8,65535
	ctx.r8.u64 = ctx.r8.u64 | 65535;
loc_82A5C120:
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stfs f0,32(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 32, temp.u32);
	// stfs f0,36(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 36, temp.u32);
	// stw r10,-32(r11)
	PPC_STORE_U32(ctx.r11.u32 + -32, ctx.r10.u32);
	// stvx128 v0,r11,r4
	ea = (ctx.r11.u32 + ctx.r4.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// sth r8,44(r11)
	PPC_STORE_U16(ctx.r11.u32 + 44, ctx.r8.u16);
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// sth r10,46(r11)
	PPC_STORE_U16(ctx.r11.u32 + 46, ctx.r10.u16);
	// stvx128 v0,r11,r5
	ea = (ctx.r11.u32 + ctx.r5.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stb r10,48(r11)
	PPC_STORE_U8(ctx.r11.u32 + 48, ctx.r10.u8);
	// sth r10,50(r11)
	PPC_STORE_U16(ctx.r11.u32 + 50, ctx.r10.u16);
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// stw r10,40(r11)
	PPC_STORE_U32(ctx.r11.u32 + 40, ctx.r10.u32);
	// addi r11,r11,96
	ctx.r11.s64 = ctx.r11.s64 + 96;
	// bge cr6,0x82a5c120
	if (!ctx.cr6.lt) goto loc_82A5C120;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5C160);
PPC_WEAK_FUNC(sub_82A5C160) { __imp__sub_82A5C160(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C160) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32057
	ctx.r11.s64 = -2100887552;
	// addi r3,r11,-17760
	ctx.r3.s64 = ctx.r11.s64 + -17760;
	// b 0x82279618
	sub_82279618(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5C170);
PPC_WEAK_FUNC(sub_82A5C170) { __imp__sub_82A5C170(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C170) {
	PPC_FUNC_PROLOGUE();
loc_82A5C170:
	// mftb r11
	ctx.r11.u64 = PPC_QUERY_TIMEBASE();
	// rotlwi r10,r11,0
	ctx.r10.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x82a5c170
	if (ctx.cr6.eq) goto loc_82A5C170;
	// lis r10,-32057
	ctx.r10.s64 = -2100887552;
	// std r11,-15672(r10)
	PPC_STORE_U64(ctx.r10.u32 + -15672, ctx.r11.u64);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5C190);
PPC_WEAK_FUNC(sub_82A5C190) { __imp__sub_82A5C190(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C190) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32053
	ctx.r11.s64 = -2100625408;
	// addi r3,r11,-2416
	ctx.r3.s64 = ctx.r11.s64 + -2416;
	// bl 0x8285fe48
	ctx.lr = 0x82A5C1A8;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,320
	ctx.r3.s64 = ctx.r11.s64 + 320;
	// bl 0x829ffa48
	ctx.lr = 0x82A5C1B4;
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

PPC_FUNC_IMPL(__imp__sub_82A5C1C8);
PPC_WEAK_FUNC(sub_82A5C1C8) { __imp__sub_82A5C1C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C1C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32053
	ctx.r11.s64 = -2100625408;
	// li r9,-1
	ctx.r9.s64 = -1;
	// addi r11,r11,5808
	ctx.r11.s64 = ctx.r11.s64 + 5808;
	// li r10,128
	ctx.r10.s64 = 128;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_82A5C1DC:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x82a5c1dc
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_82A5C1DC;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5C1F0);
PPC_WEAK_FUNC(sub_82A5C1F0) { __imp__sub_82A5C1F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C1F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32053
	ctx.r11.s64 = -2100625408;
	// li r9,-1
	ctx.r9.s64 = -1;
	// addi r11,r11,6320
	ctx.r11.s64 = ctx.r11.s64 + 6320;
	// li r10,128
	ctx.r10.s64 = 128;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_82A5C204:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x82a5c204
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_82A5C204;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5C218);
PPC_WEAK_FUNC(sub_82A5C218) { __imp__sub_82A5C218(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C218) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,336
	ctx.r3.s64 = ctx.r11.s64 + 336;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5C228);
PPC_WEAK_FUNC(sub_82A5C228) { __imp__sub_82A5C228(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C228) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32053
	ctx.r11.s64 = -2100625408;
	// addi r3,r11,10232
	ctx.r3.s64 = ctx.r11.s64 + 10232;
	// bl 0x8290d490
	ctx.lr = 0x82A5C240;
	sub_8290D490(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,368
	ctx.r3.s64 = ctx.r11.s64 + 368;
	// bl 0x829ffa48
	ctx.lr = 0x82A5C24C;
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

PPC_FUNC_IMPL(__imp__sub_82A5C260);
PPC_WEAK_FUNC(sub_82A5C260) { __imp__sub_82A5C260(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C260) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,488
	ctx.r3.s64 = ctx.r11.s64 + 488;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5C270);
PPC_WEAK_FUNC(sub_82A5C270) { __imp__sub_82A5C270(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C270) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32053
	ctx.r11.s64 = -2100625408;
	// addi r3,r11,22304
	ctx.r3.s64 = ctx.r11.s64 + 22304;
	// bl 0x825f8030
	ctx.lr = 0x82A5C288;
	sub_825F8030(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,384
	ctx.r3.s64 = ctx.r11.s64 + 384;
	// bl 0x829ffa48
	ctx.lr = 0x82A5C294;
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

PPC_FUNC_IMPL(__imp__sub_82A5C2A8);
PPC_WEAK_FUNC(sub_82A5C2A8) { __imp__sub_82A5C2A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C2A8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// addi r3,r11,-30416
	ctx.r3.s64 = ctx.r11.s64 + -30416;
	// bl 0x82915100
	ctx.lr = 0x82A5C2C0;
	sub_82915100(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,400
	ctx.r3.s64 = ctx.r11.s64 + 400;
	// bl 0x829ffa48
	ctx.lr = 0x82A5C2CC;
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

PPC_FUNC_IMPL(__imp__sub_82A5C2E0);
PPC_WEAK_FUNC(sub_82A5C2E0) { __imp__sub_82A5C2E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C2E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// addi r3,r11,-30388
	ctx.r3.s64 = ctx.r11.s64 + -30388;
	// bl 0x82915100
	ctx.lr = 0x82A5C2F8;
	sub_82915100(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,408
	ctx.r3.s64 = ctx.r11.s64 + 408;
	// bl 0x829ffa48
	ctx.lr = 0x82A5C304;
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

PPC_FUNC_IMPL(__imp__sub_82A5C318);
PPC_WEAK_FUNC(sub_82A5C318) { __imp__sub_82A5C318(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C318) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// addi r3,r11,-30360
	ctx.r3.s64 = ctx.r11.s64 + -30360;
	// bl 0x82915100
	ctx.lr = 0x82A5C330;
	sub_82915100(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,416
	ctx.r3.s64 = ctx.r11.s64 + 416;
	// bl 0x829ffa48
	ctx.lr = 0x82A5C33C;
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

PPC_FUNC_IMPL(__imp__sub_82A5C350);
PPC_WEAK_FUNC(sub_82A5C350) { __imp__sub_82A5C350(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C350) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// addi r3,r11,-30332
	ctx.r3.s64 = ctx.r11.s64 + -30332;
	// bl 0x82915100
	ctx.lr = 0x82A5C368;
	sub_82915100(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,424
	ctx.r3.s64 = ctx.r11.s64 + 424;
	// bl 0x829ffa48
	ctx.lr = 0x82A5C374;
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

PPC_FUNC_IMPL(__imp__sub_82A5C388);
PPC_WEAK_FUNC(sub_82A5C388) { __imp__sub_82A5C388(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C388) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// addi r3,r11,-30304
	ctx.r3.s64 = ctx.r11.s64 + -30304;
	// bl 0x82915100
	ctx.lr = 0x82A5C3A0;
	sub_82915100(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,432
	ctx.r3.s64 = ctx.r11.s64 + 432;
	// bl 0x829ffa48
	ctx.lr = 0x82A5C3AC;
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

PPC_FUNC_IMPL(__imp__sub_82A5C3C0);
PPC_WEAK_FUNC(sub_82A5C3C0) { __imp__sub_82A5C3C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C3C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// addi r3,r11,-30276
	ctx.r3.s64 = ctx.r11.s64 + -30276;
	// bl 0x82915100
	ctx.lr = 0x82A5C3D8;
	sub_82915100(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,440
	ctx.r3.s64 = ctx.r11.s64 + 440;
	// bl 0x829ffa48
	ctx.lr = 0x82A5C3E4;
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

PPC_FUNC_IMPL(__imp__sub_82A5C3F8);
PPC_WEAK_FUNC(sub_82A5C3F8) { __imp__sub_82A5C3F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C3F8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// addi r3,r11,-30248
	ctx.r3.s64 = ctx.r11.s64 + -30248;
	// bl 0x82915100
	ctx.lr = 0x82A5C410;
	sub_82915100(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,448
	ctx.r3.s64 = ctx.r11.s64 + 448;
	// bl 0x829ffa48
	ctx.lr = 0x82A5C41C;
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

PPC_FUNC_IMPL(__imp__sub_82A5C430);
PPC_WEAK_FUNC(sub_82A5C430) { __imp__sub_82A5C430(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C430) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// addi r3,r11,-30220
	ctx.r3.s64 = ctx.r11.s64 + -30220;
	// bl 0x8290c330
	ctx.lr = 0x82A5C448;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,456
	ctx.r3.s64 = ctx.r11.s64 + 456;
	// bl 0x829ffa48
	ctx.lr = 0x82A5C454;
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

PPC_FUNC_IMPL(__imp__sub_82A5C468);
PPC_WEAK_FUNC(sub_82A5C468) { __imp__sub_82A5C468(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C468) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// addi r3,r11,-30180
	ctx.r3.s64 = ctx.r11.s64 + -30180;
	// bl 0x8290c330
	ctx.lr = 0x82A5C480;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,464
	ctx.r3.s64 = ctx.r11.s64 + 464;
	// bl 0x829ffa48
	ctx.lr = 0x82A5C48C;
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

PPC_FUNC_IMPL(__imp__sub_82A5C4A0);
PPC_WEAK_FUNC(sub_82A5C4A0) { __imp__sub_82A5C4A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C4A0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// addi r3,r11,-30140
	ctx.r3.s64 = ctx.r11.s64 + -30140;
	// bl 0x8290c330
	ctx.lr = 0x82A5C4B8;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,472
	ctx.r3.s64 = ctx.r11.s64 + 472;
	// bl 0x829ffa48
	ctx.lr = 0x82A5C4C4;
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

PPC_FUNC_IMPL(__imp__sub_82A5C4D8);
PPC_WEAK_FUNC(sub_82A5C4D8) { __imp__sub_82A5C4D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C4D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// addi r3,r11,-30100
	ctx.r3.s64 = ctx.r11.s64 + -30100;
	// bl 0x8290c330
	ctx.lr = 0x82A5C4F0;
	sub_8290C330(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,480
	ctx.r3.s64 = ctx.r11.s64 + 480;
	// bl 0x829ffa48
	ctx.lr = 0x82A5C4FC;
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

PPC_FUNC_IMPL(__imp__sub_82A5C510);
PPC_WEAK_FUNC(sub_82A5C510) { __imp__sub_82A5C510(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C510) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-21832
	ctx.r5.s64 = ctx.r11.s64 + -21832;
	// lis r11,-32053
	ctx.r11.s64 = -2100625408;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,22284
	ctx.r3.s64 = ctx.r11.s64 + 22284;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5C530);
PPC_WEAK_FUNC(sub_82A5C530) { __imp__sub_82A5C530(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C530) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c4
	ctx.lr = 0x82A5C538;
	__savegprlr_27(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// addi r8,r10,-18816
	ctx.r8.s64 = ctx.r10.s64 + -18816;
	// lis r4,-32254
	ctx.r4.s64 = -2113798144;
	// lis r5,-32256
	ctx.r5.s64 = -2113929216;
	// lis r6,-32256
	ctx.r6.s64 = -2113929216;
	// addi r11,r11,-30032
	ctx.r11.s64 = ctx.r11.s64 + -30032;
	// lvx128 v0,r0,r8
	ea = (ctx.r8.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r7,59
	ctx.r7.s64 = 59;
	// lfs f12,-27024(r4)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r4.u32 + -27024);
	ctx.f12.f64 = double(temp.f32);
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// lfs f13,3400(r5)
	temp.u32 = PPC_LOAD_U32(ctx.r5.u32 + 3400);
	ctx.f13.f64 = double(temp.f32);
	// li r10,0
	ctx.r10.s64 = 0;
	// lfs f0,2612(r6)
	temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// li r9,-1
	ctx.r9.s64 = -1;
	// li r27,56
	ctx.r27.s64 = 56;
	// li r28,72
	ctx.r28.s64 = 72;
	// li r29,25000
	ctx.r29.s64 = 25000;
	// li r30,5
	ctx.r30.s64 = 5;
	// li r31,168
	ctx.r31.s64 = 168;
	// li r3,184
	ctx.r3.s64 = 184;
loc_82A5C590:
	// stfs f0,12(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// stw r10,-8(r11)
	PPC_STORE_U32(ctx.r11.u32 + -8, ctx.r10.u32);
	// stfs f0,16(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 16, temp.u32);
	// stw r10,-4(r11)
	PPC_STORE_U32(ctx.r11.u32 + -4, ctx.r10.u32);
	// stfs f13,36(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 36, temp.u32);
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// stfs f13,40(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 40, temp.u32);
	// stw r10,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
	// stfs f13,232(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 232, temp.u32);
	// stw r10,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r10.u32);
	// stfs f13,236(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 236, temp.u32);
	// stw r9,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r9.u32);
	// stfs f13,44(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 44, temp.u32);
	// stw r10,24(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24, ctx.r10.u32);
	// stfs f13,48(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 48, temp.u32);
	// stw r10,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r10.u32);
	// stw r9,32(r11)
	PPC_STORE_U32(ctx.r11.u32 + 32, ctx.r9.u32);
	// addi r8,r11,248
	ctx.r8.s64 = ctx.r11.s64 + 248;
	// sth r10,122(r11)
	PPC_STORE_U16(ctx.r11.u32 + 122, ctx.r10.u16);
	// addi r7,r7,-1
	ctx.r7.s64 = ctx.r7.s64 + -1;
	// sth r10,124(r11)
	PPC_STORE_U16(ctx.r11.u32 + 124, ctx.r10.u16);
	// stvx128 v0,r11,r27
	ea = (ctx.r11.u32 + ctx.r27.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx128 v0,r11,r28
	ea = (ctx.r11.u32 + ctx.r28.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stw r10,104(r11)
	PPC_STORE_U32(ctx.r11.u32 + 104, ctx.r10.u32);
	// stfs f0,88(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 88, temp.u32);
	// stw r10,112(r11)
	PPC_STORE_U32(ctx.r11.u32 + 112, ctx.r10.u32);
	// stfs f13,92(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 92, temp.u32);
	// stw r9,116(r11)
	PPC_STORE_U32(ctx.r11.u32 + 116, ctx.r9.u32);
	// stfs f13,96(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 96, temp.u32);
	// sth r10,120(r11)
	PPC_STORE_U16(ctx.r11.u32 + 120, ctx.r10.u16);
	// stfs f0,100(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 100, temp.u32);
	// sth r10,126(r11)
	PPC_STORE_U16(ctx.r11.u32 + 126, ctx.r10.u16);
	// stfs f0,108(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 108, temp.u32);
	// sth r29,128(r11)
	PPC_STORE_U16(ctx.r11.u32 + 128, ctx.r29.u16);
	// stfs f12,136(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 136, temp.u32);
	// stw r10,132(r11)
	PPC_STORE_U32(ctx.r11.u32 + 132, ctx.r10.u32);
	// stw r10,140(r11)
	PPC_STORE_U32(ctx.r11.u32 + 140, ctx.r10.u32);
	// cmpwi cr6,r7,0
	ctx.cr6.compare<int32_t>(ctx.r7.s32, 0, ctx.xer);
	// stw r10,144(r11)
	PPC_STORE_U32(ctx.r11.u32 + 144, ctx.r10.u32);
	// stw r10,148(r11)
	PPC_STORE_U32(ctx.r11.u32 + 148, ctx.r10.u32);
	// stw r30,152(r11)
	PPC_STORE_U32(ctx.r11.u32 + 152, ctx.r30.u32);
	// stw r10,156(r11)
	PPC_STORE_U32(ctx.r11.u32 + 156, ctx.r10.u32);
	// stw r10,160(r11)
	PPC_STORE_U32(ctx.r11.u32 + 160, ctx.r10.u32);
	// stw r9,164(r11)
	PPC_STORE_U32(ctx.r11.u32 + 164, ctx.r9.u32);
	// stvx128 v0,r11,r31
	ea = (ctx.r11.u32 + ctx.r31.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx128 v0,r11,r3
	ea = (ctx.r11.u32 + ctx.r3.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stw r9,212(r11)
	PPC_STORE_U32(ctx.r11.u32 + 212, ctx.r9.u32);
	// stfs f0,200(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 200, temp.u32);
	// stw r9,216(r11)
	PPC_STORE_U32(ctx.r11.u32 + 216, ctx.r9.u32);
	// stfs f0,204(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 204, temp.u32);
	// stw r10,220(r11)
	PPC_STORE_U32(ctx.r11.u32 + 220, ctx.r10.u32);
	// stfs f0,208(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 208, temp.u32);
	// stw r10,224(r11)
	PPC_STORE_U32(ctx.r11.u32 + 224, ctx.r10.u32);
	// stw r10,228(r11)
	PPC_STORE_U32(ctx.r11.u32 + 228, ctx.r10.u32);
	// addi r11,r11,272
	ctx.r11.s64 = ctx.r11.s64 + 272;
	// stw r10,0(r8)
	PPC_STORE_U32(ctx.r8.u32 + 0, ctx.r10.u32);
	// stw r10,4(r8)
	PPC_STORE_U32(ctx.r8.u32 + 4, ctx.r10.u32);
	// stw r10,8(r8)
	PPC_STORE_U32(ctx.r8.u32 + 8, ctx.r10.u32);
	// bge cr6,0x82a5c590
	if (!ctx.cr6.lt) goto loc_82A5C590;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,512
	ctx.r3.s64 = ctx.r11.s64 + 512;
	// bl 0x829ffa48
	ctx.lr = 0x82A5C688;
	sub_829FFA48(ctx, base);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x829ff814
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5C690);
PPC_WEAK_FUNC(sub_82A5C690) { __imp__sub_82A5C690(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5C690) {
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
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r30,r11,-27672
	ctx.r30.s64 = ctx.r11.s64 + -27672;
	// lwz r3,0(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// bl 0x8284e060
	ctx.lr = 0x82A5C6B8;
	sub_8284E060(ctx, base);
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r31,r11,-13712
	ctx.r31.s64 = ctx.r11.s64 + -13712;
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// lwz r3,4(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 4);
	// bl 0x8284e060
	ctx.lr = 0x82A5C6D0;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r3.u32);
	// lwz r3,8(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 8);
	// bl 0x8284e060
	ctx.lr = 0x82A5C6E0;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r3.u32);
	// lwz r3,12(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 12);
	// bl 0x8284e060
	ctx.lr = 0x82A5C6F0;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r3.u32);
	// lwz r3,16(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 16);
	// bl 0x8284e060
	ctx.lr = 0x82A5C700;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r3.u32);
	// lwz r3,20(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 20);
	// bl 0x8284e060
	ctx.lr = 0x82A5C710;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,20(r31)
	PPC_STORE_U32(ctx.r31.u32 + 20, ctx.r3.u32);
	// lwz r3,24(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 24);
	// bl 0x8284e060
	ctx.lr = 0x82A5C720;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,24(r31)
	PPC_STORE_U32(ctx.r31.u32 + 24, ctx.r3.u32);
	// lwz r3,28(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 28);
	// bl 0x8284e060
	ctx.lr = 0x82A5C730;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,28(r31)
	PPC_STORE_U32(ctx.r31.u32 + 28, ctx.r3.u32);
	// lwz r3,32(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 32);
	// bl 0x8284e060
	ctx.lr = 0x82A5C740;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,32(r31)
	PPC_STORE_U32(ctx.r31.u32 + 32, ctx.r3.u32);
	// lwz r3,36(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 36);
	// bl 0x8284e060
	ctx.lr = 0x82A5C750;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,36(r31)
	PPC_STORE_U32(ctx.r31.u32 + 36, ctx.r3.u32);
	// lwz r3,40(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 40);
	// bl 0x8284e060
	ctx.lr = 0x82A5C760;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r3.u32);
	// lwz r3,44(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 44);
	// bl 0x8284e060
	ctx.lr = 0x82A5C770;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,44(r31)
	PPC_STORE_U32(ctx.r31.u32 + 44, ctx.r3.u32);
	// lwz r3,48(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 48);
	// bl 0x8284e060
	ctx.lr = 0x82A5C780;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,48(r31)
	PPC_STORE_U32(ctx.r31.u32 + 48, ctx.r3.u32);
	// lwz r3,52(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 52);
	// bl 0x8284e060
	ctx.lr = 0x82A5C790;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,52(r31)
	PPC_STORE_U32(ctx.r31.u32 + 52, ctx.r3.u32);
	// lwz r3,56(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 56);
	// bl 0x8284e060
	ctx.lr = 0x82A5C7A0;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,56(r31)
	PPC_STORE_U32(ctx.r31.u32 + 56, ctx.r3.u32);
	// lwz r3,60(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 60);
	// bl 0x8284e060
	ctx.lr = 0x82A5C7B0;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,60(r31)
	PPC_STORE_U32(ctx.r31.u32 + 60, ctx.r3.u32);
	// lwz r3,64(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 64);
	// bl 0x8284e060
	ctx.lr = 0x82A5C7C0;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,64(r31)
	PPC_STORE_U32(ctx.r31.u32 + 64, ctx.r3.u32);
	// lwz r3,68(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 68);
	// bl 0x8284e060
	ctx.lr = 0x82A5C7D0;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,68(r31)
	PPC_STORE_U32(ctx.r31.u32 + 68, ctx.r3.u32);
	// lwz r3,72(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 72);
	// bl 0x8284e060
	ctx.lr = 0x82A5C7E0;
	sub_8284E060(ctx, base);
	// stw r3,72(r31)
	PPC_STORE_U32(ctx.r31.u32 + 72, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// lwz r3,76(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 76);
	// bl 0x8284e060
	ctx.lr = 0x82A5C7F0;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,76(r31)
	PPC_STORE_U32(ctx.r31.u32 + 76, ctx.r3.u32);
	// lwz r3,80(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 80);
	// bl 0x8284e060
	ctx.lr = 0x82A5C800;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,80(r31)
	PPC_STORE_U32(ctx.r31.u32 + 80, ctx.r3.u32);
	// lwz r3,84(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 84);
	// bl 0x8284e060
	ctx.lr = 0x82A5C810;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,84(r31)
	PPC_STORE_U32(ctx.r31.u32 + 84, ctx.r3.u32);
	// lwz r3,88(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 88);
	// bl 0x8284e060
	ctx.lr = 0x82A5C820;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,88(r31)
	PPC_STORE_U32(ctx.r31.u32 + 88, ctx.r3.u32);
	// lwz r3,92(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 92);
	// bl 0x8284e060
	ctx.lr = 0x82A5C830;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,92(r31)
	PPC_STORE_U32(ctx.r31.u32 + 92, ctx.r3.u32);
	// lwz r3,96(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 96);
	// bl 0x8284e060
	ctx.lr = 0x82A5C840;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,96(r31)
	PPC_STORE_U32(ctx.r31.u32 + 96, ctx.r3.u32);
	// lwz r3,100(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 100);
	// bl 0x8284e060
	ctx.lr = 0x82A5C850;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,100(r31)
	PPC_STORE_U32(ctx.r31.u32 + 100, ctx.r3.u32);
	// lwz r3,104(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 104);
	// bl 0x8284e060
	ctx.lr = 0x82A5C860;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,104(r31)
	PPC_STORE_U32(ctx.r31.u32 + 104, ctx.r3.u32);
	// lwz r3,108(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 108);
	// bl 0x8284e060
	ctx.lr = 0x82A5C870;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,108(r31)
	PPC_STORE_U32(ctx.r31.u32 + 108, ctx.r3.u32);
	// lwz r3,112(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 112);
	// bl 0x8284e060
	ctx.lr = 0x82A5C880;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,112(r31)
	PPC_STORE_U32(ctx.r31.u32 + 112, ctx.r3.u32);
	// lwz r3,116(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 116);
	// bl 0x8284e060
	ctx.lr = 0x82A5C890;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,116(r31)
	PPC_STORE_U32(ctx.r31.u32 + 116, ctx.r3.u32);
	// lwz r3,120(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 120);
	// bl 0x8284e060
	ctx.lr = 0x82A5C8A0;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,120(r31)
	PPC_STORE_U32(ctx.r31.u32 + 120, ctx.r3.u32);
	// lwz r3,124(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 124);
	// bl 0x8284e060
	ctx.lr = 0x82A5C8B0;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,124(r31)
	PPC_STORE_U32(ctx.r31.u32 + 124, ctx.r3.u32);
	// lwz r3,128(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 128);
	// bl 0x8284e060
	ctx.lr = 0x82A5C8C0;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,128(r31)
	PPC_STORE_U32(ctx.r31.u32 + 128, ctx.r3.u32);
	// lwz r3,132(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 132);
	// bl 0x8284e060
	ctx.lr = 0x82A5C8D0;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,132(r31)
	PPC_STORE_U32(ctx.r31.u32 + 132, ctx.r3.u32);
	// lwz r3,136(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 136);
	// bl 0x8284e060
	ctx.lr = 0x82A5C8E0;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,136(r31)
	PPC_STORE_U32(ctx.r31.u32 + 136, ctx.r3.u32);
	// lwz r3,140(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 140);
	// bl 0x8284e060
	ctx.lr = 0x82A5C8F0;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,140(r31)
	PPC_STORE_U32(ctx.r31.u32 + 140, ctx.r3.u32);
	// lwz r3,144(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 144);
	// bl 0x8284e060
	ctx.lr = 0x82A5C900;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,144(r31)
	PPC_STORE_U32(ctx.r31.u32 + 144, ctx.r3.u32);
	// lwz r3,148(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 148);
	// bl 0x8284e060
	ctx.lr = 0x82A5C910;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,148(r31)
	PPC_STORE_U32(ctx.r31.u32 + 148, ctx.r3.u32);
	// lwz r3,152(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 152);
	// bl 0x8284e060
	ctx.lr = 0x82A5C920;
	sub_8284E060(ctx, base);
	// stw r3,152(r31)
	PPC_STORE_U32(ctx.r31.u32 + 152, ctx.r3.u32);
	// lwz r3,156(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 156);
	// li r4,0
	ctx.r4.s64 = 0;
	// bl 0x8284e060
	ctx.lr = 0x82A5C930;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,156(r31)
	PPC_STORE_U32(ctx.r31.u32 + 156, ctx.r3.u32);
	// lwz r3,160(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 160);
	// bl 0x8284e060
	ctx.lr = 0x82A5C940;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,160(r31)
	PPC_STORE_U32(ctx.r31.u32 + 160, ctx.r3.u32);
	// lwz r3,164(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 164);
	// bl 0x8284e060
	ctx.lr = 0x82A5C950;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,164(r31)
	PPC_STORE_U32(ctx.r31.u32 + 164, ctx.r3.u32);
	// lwz r3,168(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 168);
	// bl 0x8284e060
	ctx.lr = 0x82A5C960;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,168(r31)
	PPC_STORE_U32(ctx.r31.u32 + 168, ctx.r3.u32);
	// lwz r3,172(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 172);
	// bl 0x8284e060
	ctx.lr = 0x82A5C970;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,172(r31)
	PPC_STORE_U32(ctx.r31.u32 + 172, ctx.r3.u32);
	// lwz r3,176(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 176);
	// bl 0x8284e060
	ctx.lr = 0x82A5C980;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,176(r31)
	PPC_STORE_U32(ctx.r31.u32 + 176, ctx.r3.u32);
	// lwz r3,180(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 180);
	// bl 0x8284e060
	ctx.lr = 0x82A5C990;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,180(r31)
	PPC_STORE_U32(ctx.r31.u32 + 180, ctx.r3.u32);
	// lwz r3,184(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 184);
	// bl 0x8284e060
	ctx.lr = 0x82A5C9A0;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,184(r31)
	PPC_STORE_U32(ctx.r31.u32 + 184, ctx.r3.u32);
	// lwz r3,188(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 188);
	// bl 0x8284e060
	ctx.lr = 0x82A5C9B0;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,188(r31)
	PPC_STORE_U32(ctx.r31.u32 + 188, ctx.r3.u32);
	// lwz r3,192(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 192);
	// bl 0x8284e060
	ctx.lr = 0x82A5C9C0;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,192(r31)
	PPC_STORE_U32(ctx.r31.u32 + 192, ctx.r3.u32);
	// lwz r3,196(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 196);
	// bl 0x8284e060
	ctx.lr = 0x82A5C9D0;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,196(r31)
	PPC_STORE_U32(ctx.r31.u32 + 196, ctx.r3.u32);
	// lwz r3,200(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 200);
	// bl 0x8284e060
	ctx.lr = 0x82A5C9E0;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,200(r31)
	PPC_STORE_U32(ctx.r31.u32 + 200, ctx.r3.u32);
	// lwz r3,204(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 204);
	// bl 0x8284e060
	ctx.lr = 0x82A5C9F0;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,204(r31)
	PPC_STORE_U32(ctx.r31.u32 + 204, ctx.r3.u32);
	// lwz r3,208(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 208);
	// bl 0x8284e060
	ctx.lr = 0x82A5CA00;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,208(r31)
	PPC_STORE_U32(ctx.r31.u32 + 208, ctx.r3.u32);
	// lwz r3,212(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 212);
	// bl 0x8284e060
	ctx.lr = 0x82A5CA10;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,212(r31)
	PPC_STORE_U32(ctx.r31.u32 + 212, ctx.r3.u32);
	// lwz r3,216(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 216);
	// bl 0x8284e060
	ctx.lr = 0x82A5CA20;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,216(r31)
	PPC_STORE_U32(ctx.r31.u32 + 216, ctx.r3.u32);
	// lwz r3,220(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 220);
	// bl 0x8284e060
	ctx.lr = 0x82A5CA30;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,220(r31)
	PPC_STORE_U32(ctx.r31.u32 + 220, ctx.r3.u32);
	// lwz r3,224(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 224);
	// bl 0x8284e060
	ctx.lr = 0x82A5CA40;
	sub_8284E060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,224(r31)
	PPC_STORE_U32(ctx.r31.u32 + 224, ctx.r3.u32);
	// lwz r3,228(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 228);
	// bl 0x8284e060
	ctx.lr = 0x82A5CA50;
	sub_8284E060(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r3,228(r31)
	PPC_STORE_U32(ctx.r31.u32 + 228, ctx.r3.u32);
	// std r11,232(r31)
	PPC_STORE_U64(ctx.r31.u32 + 232, ctx.r11.u64);
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

PPC_FUNC_IMPL(__imp__sub_82A5CA78);
PPC_WEAK_FUNC(sub_82A5CA78) { __imp__sub_82A5CA78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5CA78) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,520
	ctx.r3.s64 = ctx.r11.s64 + 520;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5CA88);
PPC_WEAK_FUNC(sub_82A5CA88) { __imp__sub_82A5CA88(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5CA88) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// addi r3,r11,-11720
	ctx.r3.s64 = ctx.r11.s64 + -11720;
	// b 0x824e2f68
	sub_824E2F68(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5CA98);
PPC_WEAK_FUNC(sub_82A5CA98) { __imp__sub_82A5CA98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5CA98) {
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
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// li r31,1549
	ctx.r31.s64 = 1549;
	// addi r30,r11,-11704
	ctx.r30.s64 = ctx.r11.s64 + -11704;
loc_82A5CAB8:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x827189d8
	ctx.lr = 0x82A5CAC0;
	sub_827189D8(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,28
	ctx.r30.s64 = ctx.r30.s64 + 28;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x82a5cab8
	if (!ctx.cr6.lt) goto loc_82A5CAB8;
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

PPC_FUNC_IMPL(__imp__sub_82A5CAE8);
PPC_WEAK_FUNC(sub_82A5CAE8) { __imp__sub_82A5CAE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5CAE8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// addi r3,r11,31720
	ctx.r3.s64 = ctx.r11.s64 + 31720;
	// bl 0x8285fe48
	ctx.lr = 0x82A5CB00;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,552
	ctx.r3.s64 = ctx.r11.s64 + 552;
	// bl 0x829ffa48
	ctx.lr = 0x82A5CB0C;
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

PPC_FUNC_IMPL(__imp__sub_82A5CB20);
PPC_WEAK_FUNC(sub_82A5CB20) { __imp__sub_82A5CB20(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5CB20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r9,r11,32232
	ctx.r9.s64 = ctx.r11.s64 + 32232;
	// addi r11,r9,64
	ctx.r11.s64 = ctx.r9.s64 + 64;
loc_82A5CB30:
	// stw r10,-64(r11)
	PPC_STORE_U32(ctx.r11.u32 + -64, ctx.r10.u32);
	// addi r8,r9,128
	ctx.r8.s64 = ctx.r9.s64 + 128;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// cmpw cr6,r11,r8
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r8.s32, ctx.xer);
	// blt cr6,0x82a5cb30
	if (ctx.cr6.lt) goto loc_82A5CB30;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5CB50);
PPC_WEAK_FUNC(sub_82A5CB50) { __imp__sub_82A5CB50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5CB50) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-16928
	ctx.r5.s64 = ctx.r11.s64 + -16928;
	// lis r11,-32049
	ctx.r11.s64 = -2100363264;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,9824
	ctx.r3.s64 = ctx.r11.s64 + 9824;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5CB70);
PPC_WEAK_FUNC(sub_82A5CB70) { __imp__sub_82A5CB70(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5CB70) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32049
	ctx.r11.s64 = -2100363264;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r9,r11,9872
	ctx.r9.s64 = ctx.r11.s64 + 9872;
	// addi r11,r9,32
	ctx.r11.s64 = ctx.r9.s64 + 32;
loc_82A5CB80:
	// stw r10,-32(r11)
	PPC_STORE_U32(ctx.r11.u32 + -32, ctx.r10.u32);
	// addi r8,r9,64
	ctx.r8.s64 = ctx.r9.s64 + 64;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// cmpw cr6,r11,r8
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r8.s32, ctx.xer);
	// blt cr6,0x82a5cb80
	if (ctx.cr6.lt) goto loc_82A5CB80;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,568
	ctx.r3.s64 = ctx.r11.s64 + 568;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5CBA8);
PPC_WEAK_FUNC(sub_82A5CBA8) { __imp__sub_82A5CBA8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5CBA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,576
	ctx.r3.s64 = ctx.r11.s64 + 576;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5CBB8);
PPC_WEAK_FUNC(sub_82A5CBB8) { __imp__sub_82A5CBB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5CBB8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r8,r1,96
	ctx.r8.s64 = ctx.r1.s64 + 96;
	// addi r10,r1,96
	ctx.r10.s64 = ctx.r1.s64 + 96;
	// lis r11,-32047
	ctx.r11.s64 = -2100232192;
	// lis r7,-32089
	ctx.r7.s64 = -2102984704;
	// addi r11,r11,-8016
	ctx.r11.s64 = ctx.r11.s64 + -8016;
	// std r6,0(r8)
	PPC_STORE_U64(ctx.r8.u32 + 0, ctx.r6.u64);
	// addi r3,r7,664
	ctx.r3.s64 = ctx.r7.s64 + 664;
	// std r6,8(r8)
	PPC_STORE_U64(ctx.r8.u32 + 8, ctx.r6.u64);
	// lwz r8,0(r10)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// mr r9,r11
	ctx.r9.u64 = ctx.r11.u64;
	// lwz r7,4(r10)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// lwz r5,8(r10)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r10.u32 + 8);
	// lwz r10,12(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 12);
	// stw r8,0(r9)
	PPC_STORE_U32(ctx.r9.u32 + 0, ctx.r8.u32);
	// stw r7,4(r9)
	PPC_STORE_U32(ctx.r9.u32 + 4, ctx.r7.u32);
	// stw r5,8(r9)
	PPC_STORE_U32(ctx.r9.u32 + 8, ctx.r5.u32);
	// stw r10,12(r9)
	PPC_STORE_U32(ctx.r9.u32 + 12, ctx.r10.u32);
	// mr r10,r6
	ctx.r10.u64 = ctx.r6.u64;
	// stb r10,0(r11)
	PPC_STORE_U8(ctx.r11.u32 + 0, ctx.r10.u8);
	// stw r10,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
	// lbz r10,80(r1)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r1.u32 + 80);
	// stw r11,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r11.u32);
	// stw r11,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r11.u32);
	// stb r10,20(r11)
	PPC_STORE_U8(ctx.r11.u32 + 20, ctx.r10.u8);
	// bl 0x829ffa48
	ctx.lr = 0x82A5CC2C;
	sub_829FFA48(ctx, base);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5CC40);
PPC_WEAK_FUNC(sub_82A5CC40) { __imp__sub_82A5CC40(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5CC40) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32047
	ctx.r11.s64 = -2100232192;
	// addi r3,r11,-7216
	ctx.r3.s64 = ctx.r11.s64 + -7216;
	// b 0x822b94c8
	sub_822B94C8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5CC50);
PPC_WEAK_FUNC(sub_82A5CC50) { __imp__sub_82A5CC50(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5CC50) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32047
	ctx.r11.s64 = -2100232192;
	// addi r3,r11,18400
	ctx.r3.s64 = ctx.r11.s64 + 18400;
	// bl 0x822b97d0
	ctx.lr = 0x82A5CC68;
	sub_822B97D0(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,768
	ctx.r3.s64 = ctx.r11.s64 + 768;
	// bl 0x829ffa48
	ctx.lr = 0x82A5CC74;
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

PPC_FUNC_IMPL(__imp__sub_82A5CC88);
PPC_WEAK_FUNC(sub_82A5CC88) { __imp__sub_82A5CC88(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5CC88) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32046
	ctx.r11.s64 = -2100166656;
	// addi r3,r11,-13808
	ctx.r3.s64 = ctx.r11.s64 + -13808;
	// bl 0x8285fe48
	ctx.lr = 0x82A5CCA0;
	sub_8285FE48(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,752
	ctx.r3.s64 = ctx.r11.s64 + 752;
	// bl 0x829ffa48
	ctx.lr = 0x82A5CCAC;
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

PPC_FUNC_IMPL(__imp__sub_82A5CCC0);
PPC_WEAK_FUNC(sub_82A5CCC0) { __imp__sub_82A5CCC0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5CCC0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lis r11,-32046
	ctx.r11.s64 = -2100166656;
	// lis r7,-32256
	ctx.r7.s64 = -2113929216;
	// lis r8,-32256
	ctx.r8.s64 = -2113929216;
	// lis r9,-32255
	ctx.r9.s64 = -2113863680;
	// addi r11,r11,-13760
	ctx.r11.s64 = ctx.r11.s64 + -13760;
	// lfs f0,2612(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,-16(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -16, temp.u32);
	// li r6,159
	ctx.r6.s64 = 159;
	// lfs f12,9096(r7)
	temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + 9096);
	ctx.f12.f64 = double(temp.f32);
	// addi r11,r11,20
	ctx.r11.s64 = ctx.r11.s64 + 20;
	// lfs f13,3400(r8)
	temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + 3400);
	ctx.f13.f64 = double(temp.f32);
	// li r5,12
	ctx.r5.s64 = 12;
	// lfs f11,-16300(r9)
	temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + -16300);
	ctx.f11.f64 = double(temp.f32);
	// li r10,0
	ctx.r10.s64 = 0;
	// li r4,10000
	ctx.r4.s64 = 10000;
loc_82A5CD00:
	// addi r9,r1,-16
	ctx.r9.s64 = ctx.r1.s64 + -16;
	// stfs f11,-4(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + -4, temp.u32);
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stw r10,28(r11)
	PPC_STORE_U32(ctx.r11.u32 + 28, ctx.r10.u32);
	// stfs f0,32(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 32, temp.u32);
	// stb r10,44(r11)
	PPC_STORE_U8(ctx.r11.u32 + 44, ctx.r10.u8);
	// stfs f0,36(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 36, temp.u32);
	// li r8,8
	ctx.r8.s64 = 8;
	// stfs f0,40(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 40, temp.u32);
	// lvlx v0,0,r9
	temp.u32 = ctx.r9.u32;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
	// addi r9,r11,64
	ctx.r9.s64 = ctx.r11.s64 + 64;
	// vspltw v0,v0,0
	simde_mm_store_si128((simde__m128i*)ctx.v0.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)ctx.v0.u32), 0xFF));
	// stfs f0,48(r11)
	temp.f32 = float(ctx.f0.f64);
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
	// stvx128 v0,r11,r5
	ea = (ctx.r11.u32 + ctx.r5.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// mtctr r8
	ctx.ctr.u64 = ctx.r8.u64;
loc_82A5CD48:
	// stw r10,0(r9)
	PPC_STORE_U32(ctx.r9.u32 + 0, ctx.r10.u32);
	// addi r9,r9,4
	ctx.r9.s64 = ctx.r9.s64 + 4;
	// bdnz 0x82a5cd48
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_82A5CD48;
	// addi r6,r6,-1
	ctx.r6.s64 = ctx.r6.s64 + -1;
	// stfs f0,96(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 96, temp.u32);
	// stfs f0,100(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 100, temp.u32);
	// stw r4,212(r11)
	PPC_STORE_U32(ctx.r11.u32 + 212, ctx.r4.u32);
	// stfs f0,104(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 104, temp.u32);
	// stw r10,216(r11)
	PPC_STORE_U32(ctx.r11.u32 + 216, ctx.r10.u32);
	// stfs f0,108(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 108, temp.u32);
	// stw r10,220(r11)
	PPC_STORE_U32(ctx.r11.u32 + 220, ctx.r10.u32);
	// stfs f0,112(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 112, temp.u32);
	// stb r10,224(r11)
	PPC_STORE_U8(ctx.r11.u32 + 224, ctx.r10.u8);
	// stfs f0,120(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 120, temp.u32);
	// stw r10,228(r11)
	PPC_STORE_U32(ctx.r11.u32 + 228, ctx.r10.u32);
	// stfs f0,124(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 124, temp.u32);
	// stw r10,232(r11)
	PPC_STORE_U32(ctx.r11.u32 + 232, ctx.r10.u32);
	// stfs f0,128(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 128, temp.u32);
	// stw r10,236(r11)
	PPC_STORE_U32(ctx.r11.u32 + 236, ctx.r10.u32);
	// stfs f0,136(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 136, temp.u32);
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// stfs f0,144(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 144, temp.u32);
	// stfs f0,148(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 148, temp.u32);
	// stfs f0,152(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 152, temp.u32);
	// stfs f0,156(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 156, temp.u32);
	// stfs f0,160(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 160, temp.u32);
	// stfs f0,164(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 164, temp.u32);
	// stfs f0,168(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 168, temp.u32);
	// stfs f0,172(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 172, temp.u32);
	// stfs f0,176(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 176, temp.u32);
	// stfs f0,180(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 180, temp.u32);
	// stfs f0,184(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 184, temp.u32);
	// stfs f0,188(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 188, temp.u32);
	// stfs f0,192(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 192, temp.u32);
	// stfs f13,196(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 196, temp.u32);
	// stfs f13,200(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 200, temp.u32);
	// stfs f12,204(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 204, temp.u32);
	// stfs f0,208(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 208, temp.u32);
	// addi r11,r11,272
	ctx.r11.s64 = ctx.r11.s64 + 272;
	// bge cr6,0x82a5cd00
	if (!ctx.cr6.lt) goto loc_82A5CD00;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5CDF0);
PPC_WEAK_FUNC(sub_82A5CDF0) { __imp__sub_82A5CDF0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5CDF0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32046
	ctx.r11.s64 = -2100166656;
	// lis r8,-32256
	ctx.r8.s64 = -2113929216;
	// lis r9,-32255
	ctx.r9.s64 = -2113863680;
	// addi r11,r11,29760
	ctx.r11.s64 = ctx.r11.s64 + 29760;
	// li r10,39
	ctx.r10.s64 = 39;
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// lfs f0,2612(r8)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,-16300(r9)
	temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + -16300);
	ctx.f13.f64 = double(temp.f32);
loc_82A5CE10:
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
	// stfs f13,-8(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + -8, temp.u32);
	// stfs f0,-4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -4, temp.u32);
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
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
	// stfs f0,24(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 24, temp.u32);
	// stfs f0,28(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 28, temp.u32);
	// stfs f0,32(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 32, temp.u32);
	// stfs f0,36(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 36, temp.u32);
	// stfs f0,40(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 40, temp.u32);
	// stfs f0,44(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 44, temp.u32);
	// stfs f0,48(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 48, temp.u32);
	// stfs f0,52(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 52, temp.u32);
	// addi r11,r11,64
	ctx.r11.s64 = ctx.r11.s64 + 64;
	// bge cr6,0x82a5ce10
	if (!ctx.cr6.lt) goto loc_82A5CE10;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5CE68);
PPC_WEAK_FUNC(sub_82A5CE68) { __imp__sub_82A5CE68(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5CE68) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lis r11,-32046
	ctx.r11.s64 = -2100166656;
	// lis r8,-32255
	ctx.r8.s64 = -2113863680;
	// addi r11,r11,32320
	ctx.r11.s64 = ctx.r11.s64 + 32320;
	// li r9,56
	ctx.r9.s64 = 56;
	// lfs f0,2612(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// stfs f0,-16(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -16, temp.u32);
	// li r10,39
	ctx.r10.s64 = 39;
	// lfs f13,-16300(r8)
	temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + -16300);
	ctx.f13.f64 = double(temp.f32);
loc_82A5CE90:
	// addi r8,r1,-16
	ctx.r8.s64 = ctx.r1.s64 + -16;
	// stfs f13,-8(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + -8, temp.u32);
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
	// stfs f0,-4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -4, temp.u32);
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// lvlx v0,0,r8
	temp.u32 = ctx.r8.u32;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
	// vspltw v0,v0,0
	simde_mm_store_si128((simde__m128i*)ctx.v0.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)ctx.v0.u32), 0xFF));
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// stfs f0,16(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 16, temp.u32);
	// stfs f0,20(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 20, temp.u32);
	// stfs f0,24(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 24, temp.u32);
	// stfs f0,28(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 28, temp.u32);
	// stfs f0,32(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 32, temp.u32);
	// stfs f0,36(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 36, temp.u32);
	// stfs f0,40(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 40, temp.u32);
	// stfs f0,44(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 44, temp.u32);
	// stfs f0,48(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 48, temp.u32);
	// stfs f0,52(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 52, temp.u32);
	// stvx128 v0,r11,r9
	ea = (ctx.r11.u32 + ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r11,96
	ctx.r11.s64 = ctx.r11.s64 + 96;
	// bge cr6,0x82a5ce90
	if (!ctx.cr6.lt) goto loc_82A5CE90;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5CEF8);
PPC_WEAK_FUNC(sub_82A5CEF8) { __imp__sub_82A5CEF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5CEF8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// lis r7,-32256
	ctx.r7.s64 = -2113929216;
	// lis r8,-32252
	ctx.r8.s64 = -2113667072;
	// lis r9,-32255
	ctx.r9.s64 = -2113863680;
	// addi r11,r11,-29376
	ctx.r11.s64 = ctx.r11.s64 + -29376;
	// lfs f0,2612(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,-16(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -16, temp.u32);
	// li r10,39
	ctx.r10.s64 = 39;
	// lfs f11,3444(r7)
	temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + 3444);
	ctx.f11.f64 = double(temp.f32);
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// lfs f12,26436(r8)
	temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + 26436);
	ctx.f12.f64 = double(temp.f32);
	// li r4,40
	ctx.r4.s64 = 40;
	// lfs f13,-16300(r9)
	temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + -16300);
	ctx.f13.f64 = double(temp.f32);
	// li r5,56
	ctx.r5.s64 = 56;
	// stfs f0,-12(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -12, temp.u32);
	// li r6,0
	ctx.r6.s64 = 0;
loc_82A5CF3C:
	// addi r9,r1,-16
	ctx.r9.s64 = ctx.r1.s64 + -16;
	// stfs f13,-8(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + -8, temp.u32);
	// addi r8,r1,-12
	ctx.r8.s64 = ctx.r1.s64 + -12;
	// stfs f0,-4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -4, temp.u32);
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stw r6,76(r11)
	PPC_STORE_U32(ctx.r11.u32 + 76, ctx.r6.u32);
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// lvlx v0,0,r9
	temp.u32 = ctx.r9.u32;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
	// lvlx v13,0,r8
	temp.u32 = ctx.r8.u32;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
	// vspltw v0,v0,0
	simde_mm_store_si128((simde__m128i*)ctx.v0.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)ctx.v0.u32), 0xFF));
	// vspltw v13,v13,0
	simde_mm_store_si128((simde__m128i*)ctx.v13.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)ctx.v13.u32), 0xFF));
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// stfs f0,16(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 16, temp.u32);
	// stfs f0,20(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 20, temp.u32);
	// stfs f0,24(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 24, temp.u32);
	// stfs f0,28(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 28, temp.u32);
	// stfs f0,32(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 32, temp.u32);
	// stfs f0,36(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 36, temp.u32);
	// stfs f0,72(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 72, temp.u32);
	// stfs f12,208(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 208, temp.u32);
	// stfs f11,212(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 212, temp.u32);
	// stvx128 v0,r11,r4
	ea = (ctx.r11.u32 + ctx.r4.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx128 v13,r11,r5
	ea = (ctx.r11.u32 + ctx.r5.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r11,224
	ctx.r11.s64 = ctx.r11.s64 + 224;
	// bge cr6,0x82a5cf3c
	if (!ctx.cr6.lt) goto loc_82A5CF3C;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5CFB0);
PPC_WEAK_FUNC(sub_82A5CFB0) { __imp__sub_82A5CFB0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5CFB0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// addi r3,r11,-17928
	ctx.r3.s64 = ctx.r11.s64 + -17928;
	// bl 0x8265d108
	ctx.lr = 0x82A5CFC8;
	sub_8265D108(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,888
	ctx.r3.s64 = ctx.r11.s64 + 888;
	// bl 0x829ffa48
	ctx.lr = 0x82A5CFD4;
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

PPC_FUNC_IMPL(__imp__sub_82A5CFE8);
PPC_WEAK_FUNC(sub_82A5CFE8) { __imp__sub_82A5CFE8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5CFE8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-15160
	ctx.r3.s64 = ctx.r11.s64 + -15160;
	// bl 0x8284e060
	ctx.lr = 0x82A5D004;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17824(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17824, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D020);
PPC_WEAK_FUNC(sub_82A5D020) { __imp__sub_82A5D020(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D020) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-15140
	ctx.r3.s64 = ctx.r11.s64 + -15140;
	// bl 0x8284e060
	ctx.lr = 0x82A5D03C;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17820(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17820, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D058);
PPC_WEAK_FUNC(sub_82A5D058) { __imp__sub_82A5D058(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D058) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-15116
	ctx.r3.s64 = ctx.r11.s64 + -15116;
	// bl 0x8284e060
	ctx.lr = 0x82A5D074;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17816(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17816, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D090);
PPC_WEAK_FUNC(sub_82A5D090) { __imp__sub_82A5D090(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D090) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-15092
	ctx.r3.s64 = ctx.r11.s64 + -15092;
	// bl 0x8284e060
	ctx.lr = 0x82A5D0AC;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17812(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17812, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D0C8);
PPC_WEAK_FUNC(sub_82A5D0C8) { __imp__sub_82A5D0C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D0C8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-15072
	ctx.r3.s64 = ctx.r11.s64 + -15072;
	// bl 0x8284e060
	ctx.lr = 0x82A5D0E4;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17808(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17808, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D100);
PPC_WEAK_FUNC(sub_82A5D100) { __imp__sub_82A5D100(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D100) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-15032
	ctx.r3.s64 = ctx.r11.s64 + -15032;
	// bl 0x8284e060
	ctx.lr = 0x82A5D11C;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17804(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17804, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D138);
PPC_WEAK_FUNC(sub_82A5D138) { __imp__sub_82A5D138(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D138) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-14984
	ctx.r3.s64 = ctx.r11.s64 + -14984;
	// bl 0x8284e060
	ctx.lr = 0x82A5D154;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17800(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17800, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D170);
PPC_WEAK_FUNC(sub_82A5D170) { __imp__sub_82A5D170(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D170) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-14960
	ctx.r3.s64 = ctx.r11.s64 + -14960;
	// bl 0x8284e060
	ctx.lr = 0x82A5D18C;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17796(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17796, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D1A8);
PPC_WEAK_FUNC(sub_82A5D1A8) { __imp__sub_82A5D1A8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D1A8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-14936
	ctx.r3.s64 = ctx.r11.s64 + -14936;
	// bl 0x8284e060
	ctx.lr = 0x82A5D1C4;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17792(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17792, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D1E0);
PPC_WEAK_FUNC(sub_82A5D1E0) { __imp__sub_82A5D1E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D1E0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-14904
	ctx.r3.s64 = ctx.r11.s64 + -14904;
	// bl 0x8284e060
	ctx.lr = 0x82A5D1FC;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17788(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17788, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D218);
PPC_WEAK_FUNC(sub_82A5D218) { __imp__sub_82A5D218(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D218) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-14872
	ctx.r3.s64 = ctx.r11.s64 + -14872;
	// bl 0x8284e060
	ctx.lr = 0x82A5D234;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17784(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17784, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D250);
PPC_WEAK_FUNC(sub_82A5D250) { __imp__sub_82A5D250(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D250) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-14848
	ctx.r3.s64 = ctx.r11.s64 + -14848;
	// bl 0x8284e060
	ctx.lr = 0x82A5D26C;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17780(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17780, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D288);
PPC_WEAK_FUNC(sub_82A5D288) { __imp__sub_82A5D288(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D288) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-14820
	ctx.r3.s64 = ctx.r11.s64 + -14820;
	// bl 0x8284e060
	ctx.lr = 0x82A5D2A4;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17776(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17776, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D2C0);
PPC_WEAK_FUNC(sub_82A5D2C0) { __imp__sub_82A5D2C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D2C0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-14788
	ctx.r3.s64 = ctx.r11.s64 + -14788;
	// bl 0x8284e060
	ctx.lr = 0x82A5D2DC;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17772(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17772, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D2F8);
PPC_WEAK_FUNC(sub_82A5D2F8) { __imp__sub_82A5D2F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D2F8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-14752
	ctx.r3.s64 = ctx.r11.s64 + -14752;
	// bl 0x8284e060
	ctx.lr = 0x82A5D314;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17768(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17768, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D330);
PPC_WEAK_FUNC(sub_82A5D330) { __imp__sub_82A5D330(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D330) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-14732
	ctx.r3.s64 = ctx.r11.s64 + -14732;
	// bl 0x8284e060
	ctx.lr = 0x82A5D34C;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17764(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17764, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D368);
PPC_WEAK_FUNC(sub_82A5D368) { __imp__sub_82A5D368(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D368) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-14708
	ctx.r3.s64 = ctx.r11.s64 + -14708;
	// bl 0x8284e060
	ctx.lr = 0x82A5D384;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17760(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17760, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D3A0);
PPC_WEAK_FUNC(sub_82A5D3A0) { __imp__sub_82A5D3A0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D3A0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-14680
	ctx.r3.s64 = ctx.r11.s64 + -14680;
	// bl 0x8284e060
	ctx.lr = 0x82A5D3BC;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17756(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17756, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D3D8);
PPC_WEAK_FUNC(sub_82A5D3D8) { __imp__sub_82A5D3D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D3D8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-14648
	ctx.r3.s64 = ctx.r11.s64 + -14648;
	// bl 0x8284e060
	ctx.lr = 0x82A5D3F4;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17752(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17752, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D410);
PPC_WEAK_FUNC(sub_82A5D410) { __imp__sub_82A5D410(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D410) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-14628
	ctx.r3.s64 = ctx.r11.s64 + -14628;
	// bl 0x8284e060
	ctx.lr = 0x82A5D42C;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17748(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17748, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D448);
PPC_WEAK_FUNC(sub_82A5D448) { __imp__sub_82A5D448(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D448) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-14604
	ctx.r3.s64 = ctx.r11.s64 + -14604;
	// bl 0x8284e060
	ctx.lr = 0x82A5D464;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17744(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17744, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D480);
PPC_WEAK_FUNC(sub_82A5D480) { __imp__sub_82A5D480(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D480) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-14576
	ctx.r3.s64 = ctx.r11.s64 + -14576;
	// bl 0x8284e060
	ctx.lr = 0x82A5D49C;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17740(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17740, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D4B8);
PPC_WEAK_FUNC(sub_82A5D4B8) { __imp__sub_82A5D4B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D4B8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-14544
	ctx.r3.s64 = ctx.r11.s64 + -14544;
	// bl 0x8284e060
	ctx.lr = 0x82A5D4D4;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17736(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17736, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D4F0);
PPC_WEAK_FUNC(sub_82A5D4F0) { __imp__sub_82A5D4F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D4F0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-14524
	ctx.r3.s64 = ctx.r11.s64 + -14524;
	// bl 0x8284e060
	ctx.lr = 0x82A5D50C;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17732(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17732, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D528);
PPC_WEAK_FUNC(sub_82A5D528) { __imp__sub_82A5D528(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D528) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-14500
	ctx.r3.s64 = ctx.r11.s64 + -14500;
	// bl 0x8284e060
	ctx.lr = 0x82A5D544;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17728(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17728, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D560);
PPC_WEAK_FUNC(sub_82A5D560) { __imp__sub_82A5D560(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D560) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-14472
	ctx.r3.s64 = ctx.r11.s64 + -14472;
	// bl 0x8284e060
	ctx.lr = 0x82A5D57C;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17724(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17724, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D598);
PPC_WEAK_FUNC(sub_82A5D598) { __imp__sub_82A5D598(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D598) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-14440
	ctx.r3.s64 = ctx.r11.s64 + -14440;
	// bl 0x8284e060
	ctx.lr = 0x82A5D5B4;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17720(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17720, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D5D0);
PPC_WEAK_FUNC(sub_82A5D5D0) { __imp__sub_82A5D5D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D5D0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-14420
	ctx.r3.s64 = ctx.r11.s64 + -14420;
	// bl 0x8284e060
	ctx.lr = 0x82A5D5EC;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17716(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17716, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D608);
PPC_WEAK_FUNC(sub_82A5D608) { __imp__sub_82A5D608(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D608) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-14400
	ctx.r3.s64 = ctx.r11.s64 + -14400;
	// bl 0x8284e060
	ctx.lr = 0x82A5D624;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17712(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17712, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D640);
PPC_WEAK_FUNC(sub_82A5D640) { __imp__sub_82A5D640(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D640) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-14380
	ctx.r3.s64 = ctx.r11.s64 + -14380;
	// bl 0x8284e060
	ctx.lr = 0x82A5D65C;
	sub_8284E060(ctx, base);
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// stw r3,-17708(r11)
	PPC_STORE_U32(ctx.r11.u32 + -17708, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D678);
PPC_WEAK_FUNC(sub_82A5D678) { __imp__sub_82A5D678(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D678) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// li r9,199
	ctx.r9.s64 = 199;
	// addi r11,r11,-17128
	ctx.r11.s64 = ctx.r11.s64 + -17128;
	// addi r10,r11,44
	ctx.r10.s64 = ctx.r11.s64 + 44;
	// li r11,-1
	ctx.r11.s64 = -1;
loc_82A5D68C:
	// stw r11,-8(r10)
	PPC_STORE_U32(ctx.r10.u32 + -8, ctx.r11.u32);
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// stw r11,20(r10)
	PPC_STORE_U32(ctx.r10.u32 + 20, ctx.r11.u32);
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// stw r11,32(r10)
	PPC_STORE_U32(ctx.r10.u32 + 32, ctx.r11.u32);
	// stw r11,36(r10)
	PPC_STORE_U32(ctx.r10.u32 + 36, ctx.r11.u32);
	// addi r10,r10,188
	ctx.r10.s64 = ctx.r10.s64 + 188;
	// bge cr6,0x82a5d68c
	if (!ctx.cr6.lt) goto loc_82A5D68C;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D6B8);
PPC_WEAK_FUNC(sub_82A5D6B8) { __imp__sub_82A5D6B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D6B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32045
	ctx.r11.s64 = -2100101120;
	// li r9,98
	ctx.r9.s64 = 98;
	// addi r11,r11,20496
	ctx.r11.s64 = ctx.r11.s64 + 20496;
	// addi r10,r11,8
	ctx.r10.s64 = ctx.r11.s64 + 8;
	// li r11,-1
	ctx.r11.s64 = -1;
loc_82A5D6CC:
	// stw r11,-8(r10)
	PPC_STORE_U32(ctx.r10.u32 + -8, ctx.r11.u32);
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stw r11,-4(r10)
	PPC_STORE_U32(ctx.r10.u32 + -4, ctx.r11.u32);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// stw r11,36(r10)
	PPC_STORE_U32(ctx.r10.u32 + 36, ctx.r11.u32);
	// stw r11,40(r10)
	PPC_STORE_U32(ctx.r10.u32 + 40, ctx.r11.u32);
	// stw r11,80(r10)
	PPC_STORE_U32(ctx.r10.u32 + 80, ctx.r11.u32);
	// stw r11,84(r10)
	PPC_STORE_U32(ctx.r10.u32 + 84, ctx.r11.u32);
	// stw r11,88(r10)
	PPC_STORE_U32(ctx.r10.u32 + 88, ctx.r11.u32);
	// stw r11,144(r10)
	PPC_STORE_U32(ctx.r10.u32 + 144, ctx.r11.u32);
	// stw r11,148(r10)
	PPC_STORE_U32(ctx.r10.u32 + 148, ctx.r11.u32);
	// addi r10,r10,528
	ctx.r10.s64 = ctx.r10.s64 + 528;
	// bge cr6,0x82a5d6cc
	if (!ctx.cr6.lt) goto loc_82A5D6CC;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D708);
PPC_WEAK_FUNC(sub_82A5D708) { __imp__sub_82A5D708(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D708) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32044
	ctx.r11.s64 = -2100035584;
	// li r10,-1
	ctx.r10.s64 = -1;
	// addi r11,r11,7232
	ctx.r11.s64 = ctx.r11.s64 + 7232;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// stw r10,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
	// stw r10,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r10.u32);
	// stw r10,44(r11)
	PPC_STORE_U32(ctx.r11.u32 + 44, ctx.r10.u32);
	// stw r10,48(r11)
	PPC_STORE_U32(ctx.r11.u32 + 48, ctx.r10.u32);
	// stw r10,88(r11)
	PPC_STORE_U32(ctx.r11.u32 + 88, ctx.r10.u32);
	// stw r10,92(r11)
	PPC_STORE_U32(ctx.r11.u32 + 92, ctx.r10.u32);
	// stw r10,96(r11)
	PPC_STORE_U32(ctx.r11.u32 + 96, ctx.r10.u32);
	// stw r10,152(r11)
	PPC_STORE_U32(ctx.r11.u32 + 152, ctx.r10.u32);
	// stw r10,156(r11)
	PPC_STORE_U32(ctx.r11.u32 + 156, ctx.r10.u32);
	// stw r10,528(r11)
	PPC_STORE_U32(ctx.r11.u32 + 528, ctx.r10.u32);
	// stw r10,532(r11)
	PPC_STORE_U32(ctx.r11.u32 + 532, ctx.r10.u32);
	// stw r10,536(r11)
	PPC_STORE_U32(ctx.r11.u32 + 536, ctx.r10.u32);
	// stw r10,572(r11)
	PPC_STORE_U32(ctx.r11.u32 + 572, ctx.r10.u32);
	// stw r10,576(r11)
	PPC_STORE_U32(ctx.r11.u32 + 576, ctx.r10.u32);
	// stw r10,616(r11)
	PPC_STORE_U32(ctx.r11.u32 + 616, ctx.r10.u32);
	// stw r10,620(r11)
	PPC_STORE_U32(ctx.r11.u32 + 620, ctx.r10.u32);
	// stw r10,624(r11)
	PPC_STORE_U32(ctx.r11.u32 + 624, ctx.r10.u32);
	// stw r10,680(r11)
	PPC_STORE_U32(ctx.r11.u32 + 680, ctx.r10.u32);
	// stw r10,684(r11)
	PPC_STORE_U32(ctx.r11.u32 + 684, ctx.r10.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D768);
PPC_WEAK_FUNC(sub_82A5D768) { __imp__sub_82A5D768(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D768) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// lis r10,-32045
	ctx.r10.s64 = -2100101120;
	// lwz r11,-660(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -660);
	// stw r11,-17132(r10)
	PPC_STORE_U32(ctx.r10.u32 + -17132, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D780);
PPC_WEAK_FUNC(sub_82A5D780) { __imp__sub_82A5D780(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D780) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32001
	ctx.r11.s64 = -2097217536;
	// lis r10,-32045
	ctx.r10.s64 = -2100101120;
	// lwz r11,-664(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -664);
	// stw r11,20472(r10)
	PPC_STORE_U32(ctx.r10.u32 + 20472, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D798);
PPC_WEAK_FUNC(sub_82A5D798) { __imp__sub_82A5D798(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D798) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32044
	ctx.r11.s64 = -2100035584;
	// li r8,254
	ctx.r8.s64 = 254;
	// addi r11,r11,17920
	ctx.r11.s64 = ctx.r11.s64 + 17920;
	// li r7,-1
	ctx.r7.s64 = -1;
	// addi r11,r11,36
	ctx.r11.s64 = ctx.r11.s64 + 36;
	// li r9,255
	ctx.r9.s64 = 255;
	// li r6,0
	ctx.r6.s64 = 0;
loc_82A5D7B4:
	// addi r10,r11,4
	ctx.r10.s64 = ctx.r11.s64 + 4;
	// stw r7,-4(r11)
	PPC_STORE_U32(ctx.r11.u32 + -4, ctx.r7.u32);
	// sth r7,0(r11)
	PPC_STORE_U16(ctx.r11.u32 + 0, ctx.r7.u16);
	// addi r8,r8,-1
	ctx.r8.s64 = ctx.r8.s64 + -1;
	// stb r9,2(r11)
	PPC_STORE_U8(ctx.r11.u32 + 2, ctx.r9.u8);
	// stb r6,3(r11)
	PPC_STORE_U8(ctx.r11.u32 + 3, ctx.r6.u8);
	// cmpwi cr6,r8,0
	ctx.cr6.compare<int32_t>(ctx.r8.s32, 0, ctx.xer);
	// addi r11,r11,48
	ctx.r11.s64 = ctx.r11.s64 + 48;
	// stb r9,0(r10)
	PPC_STORE_U8(ctx.r10.u32 + 0, ctx.r9.u8);
	// stb r9,1(r10)
	PPC_STORE_U8(ctx.r10.u32 + 1, ctx.r9.u8);
	// stb r9,2(r10)
	PPC_STORE_U8(ctx.r10.u32 + 2, ctx.r9.u8);
	// bge cr6,0x82a5d7b4
	if (!ctx.cr6.lt) goto loc_82A5D7B4;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D7E8);
PPC_WEAK_FUNC(sub_82A5D7E8) { __imp__sub_82A5D7E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D7E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,904
	ctx.r3.s64 = ctx.r11.s64 + 904;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D7F8);
PPC_WEAK_FUNC(sub_82A5D7F8) { __imp__sub_82A5D7F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D7F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-11756
	ctx.r5.s64 = ctx.r11.s64 + -11756;
	// lis r11,-32044
	ctx.r11.s64 = -2100035584;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,30168
	ctx.r3.s64 = ctx.r11.s64 + 30168;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D818);
PPC_WEAK_FUNC(sub_82A5D818) { __imp__sub_82A5D818(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D818) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24352
	ctx.r11.s64 = ctx.r11.s64 + -24352;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D838);
PPC_WEAK_FUNC(sub_82A5D838) { __imp__sub_82A5D838(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D838) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24340
	ctx.r11.s64 = ctx.r11.s64 + -24340;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D858);
PPC_WEAK_FUNC(sub_82A5D858) { __imp__sub_82A5D858(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D858) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24328
	ctx.r11.s64 = ctx.r11.s64 + -24328;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D878);
PPC_WEAK_FUNC(sub_82A5D878) { __imp__sub_82A5D878(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D878) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24316
	ctx.r11.s64 = ctx.r11.s64 + -24316;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D898);
PPC_WEAK_FUNC(sub_82A5D898) { __imp__sub_82A5D898(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D898) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24304
	ctx.r11.s64 = ctx.r11.s64 + -24304;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D8B8);
PPC_WEAK_FUNC(sub_82A5D8B8) { __imp__sub_82A5D8B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D8B8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24292
	ctx.r11.s64 = ctx.r11.s64 + -24292;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D8D8);
PPC_WEAK_FUNC(sub_82A5D8D8) { __imp__sub_82A5D8D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D8D8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24280
	ctx.r11.s64 = ctx.r11.s64 + -24280;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D8F8);
PPC_WEAK_FUNC(sub_82A5D8F8) { __imp__sub_82A5D8F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D8F8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24268
	ctx.r11.s64 = ctx.r11.s64 + -24268;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D918);
PPC_WEAK_FUNC(sub_82A5D918) { __imp__sub_82A5D918(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D918) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24256
	ctx.r11.s64 = ctx.r11.s64 + -24256;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D938);
PPC_WEAK_FUNC(sub_82A5D938) { __imp__sub_82A5D938(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D938) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24244
	ctx.r11.s64 = ctx.r11.s64 + -24244;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D958);
PPC_WEAK_FUNC(sub_82A5D958) { __imp__sub_82A5D958(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D958) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24232
	ctx.r11.s64 = ctx.r11.s64 + -24232;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D978);
PPC_WEAK_FUNC(sub_82A5D978) { __imp__sub_82A5D978(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D978) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24220
	ctx.r11.s64 = ctx.r11.s64 + -24220;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D998);
PPC_WEAK_FUNC(sub_82A5D998) { __imp__sub_82A5D998(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D998) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24208
	ctx.r11.s64 = ctx.r11.s64 + -24208;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D9B8);
PPC_WEAK_FUNC(sub_82A5D9B8) { __imp__sub_82A5D9B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D9B8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24196
	ctx.r11.s64 = ctx.r11.s64 + -24196;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D9D8);
PPC_WEAK_FUNC(sub_82A5D9D8) { __imp__sub_82A5D9D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D9D8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24184
	ctx.r11.s64 = ctx.r11.s64 + -24184;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5D9F8);
PPC_WEAK_FUNC(sub_82A5D9F8) { __imp__sub_82A5D9F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5D9F8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24172
	ctx.r11.s64 = ctx.r11.s64 + -24172;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DA18);
PPC_WEAK_FUNC(sub_82A5DA18) { __imp__sub_82A5DA18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DA18) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24160
	ctx.r11.s64 = ctx.r11.s64 + -24160;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DA38);
PPC_WEAK_FUNC(sub_82A5DA38) { __imp__sub_82A5DA38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DA38) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24148
	ctx.r11.s64 = ctx.r11.s64 + -24148;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DA58);
PPC_WEAK_FUNC(sub_82A5DA58) { __imp__sub_82A5DA58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DA58) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24136
	ctx.r11.s64 = ctx.r11.s64 + -24136;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DA78);
PPC_WEAK_FUNC(sub_82A5DA78) { __imp__sub_82A5DA78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DA78) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24124
	ctx.r11.s64 = ctx.r11.s64 + -24124;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DA98);
PPC_WEAK_FUNC(sub_82A5DA98) { __imp__sub_82A5DA98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DA98) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24112
	ctx.r11.s64 = ctx.r11.s64 + -24112;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DAB8);
PPC_WEAK_FUNC(sub_82A5DAB8) { __imp__sub_82A5DAB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DAB8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24100
	ctx.r11.s64 = ctx.r11.s64 + -24100;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DAD8);
PPC_WEAK_FUNC(sub_82A5DAD8) { __imp__sub_82A5DAD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DAD8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24088
	ctx.r11.s64 = ctx.r11.s64 + -24088;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DAF8);
PPC_WEAK_FUNC(sub_82A5DAF8) { __imp__sub_82A5DAF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DAF8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24076
	ctx.r11.s64 = ctx.r11.s64 + -24076;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DB18);
PPC_WEAK_FUNC(sub_82A5DB18) { __imp__sub_82A5DB18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DB18) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24064
	ctx.r11.s64 = ctx.r11.s64 + -24064;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DB38);
PPC_WEAK_FUNC(sub_82A5DB38) { __imp__sub_82A5DB38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DB38) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24052
	ctx.r11.s64 = ctx.r11.s64 + -24052;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DB58);
PPC_WEAK_FUNC(sub_82A5DB58) { __imp__sub_82A5DB58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DB58) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24040
	ctx.r11.s64 = ctx.r11.s64 + -24040;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DB78);
PPC_WEAK_FUNC(sub_82A5DB78) { __imp__sub_82A5DB78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DB78) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24028
	ctx.r11.s64 = ctx.r11.s64 + -24028;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DB98);
PPC_WEAK_FUNC(sub_82A5DB98) { __imp__sub_82A5DB98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DB98) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24016
	ctx.r11.s64 = ctx.r11.s64 + -24016;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DBB8);
PPC_WEAK_FUNC(sub_82A5DBB8) { __imp__sub_82A5DBB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DBB8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-24004
	ctx.r11.s64 = ctx.r11.s64 + -24004;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DBD8);
PPC_WEAK_FUNC(sub_82A5DBD8) { __imp__sub_82A5DBD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DBD8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23992
	ctx.r11.s64 = ctx.r11.s64 + -23992;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DBF8);
PPC_WEAK_FUNC(sub_82A5DBF8) { __imp__sub_82A5DBF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DBF8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23980
	ctx.r11.s64 = ctx.r11.s64 + -23980;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DC18);
PPC_WEAK_FUNC(sub_82A5DC18) { __imp__sub_82A5DC18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DC18) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23968
	ctx.r11.s64 = ctx.r11.s64 + -23968;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DC38);
PPC_WEAK_FUNC(sub_82A5DC38) { __imp__sub_82A5DC38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DC38) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23956
	ctx.r11.s64 = ctx.r11.s64 + -23956;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DC58);
PPC_WEAK_FUNC(sub_82A5DC58) { __imp__sub_82A5DC58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DC58) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23944
	ctx.r11.s64 = ctx.r11.s64 + -23944;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DC78);
PPC_WEAK_FUNC(sub_82A5DC78) { __imp__sub_82A5DC78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DC78) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23932
	ctx.r11.s64 = ctx.r11.s64 + -23932;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DC98);
PPC_WEAK_FUNC(sub_82A5DC98) { __imp__sub_82A5DC98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DC98) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23920
	ctx.r11.s64 = ctx.r11.s64 + -23920;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DCB8);
PPC_WEAK_FUNC(sub_82A5DCB8) { __imp__sub_82A5DCB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DCB8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23908
	ctx.r11.s64 = ctx.r11.s64 + -23908;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DCD8);
PPC_WEAK_FUNC(sub_82A5DCD8) { __imp__sub_82A5DCD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DCD8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23896
	ctx.r11.s64 = ctx.r11.s64 + -23896;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DCF8);
PPC_WEAK_FUNC(sub_82A5DCF8) { __imp__sub_82A5DCF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DCF8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23884
	ctx.r11.s64 = ctx.r11.s64 + -23884;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DD18);
PPC_WEAK_FUNC(sub_82A5DD18) { __imp__sub_82A5DD18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DD18) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23872
	ctx.r11.s64 = ctx.r11.s64 + -23872;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DD38);
PPC_WEAK_FUNC(sub_82A5DD38) { __imp__sub_82A5DD38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DD38) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23860
	ctx.r11.s64 = ctx.r11.s64 + -23860;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DD58);
PPC_WEAK_FUNC(sub_82A5DD58) { __imp__sub_82A5DD58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DD58) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23848
	ctx.r11.s64 = ctx.r11.s64 + -23848;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DD78);
PPC_WEAK_FUNC(sub_82A5DD78) { __imp__sub_82A5DD78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DD78) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23836
	ctx.r11.s64 = ctx.r11.s64 + -23836;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DD98);
PPC_WEAK_FUNC(sub_82A5DD98) { __imp__sub_82A5DD98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DD98) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23824
	ctx.r11.s64 = ctx.r11.s64 + -23824;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DDB8);
PPC_WEAK_FUNC(sub_82A5DDB8) { __imp__sub_82A5DDB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DDB8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23812
	ctx.r11.s64 = ctx.r11.s64 + -23812;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DDD8);
PPC_WEAK_FUNC(sub_82A5DDD8) { __imp__sub_82A5DDD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DDD8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23800
	ctx.r11.s64 = ctx.r11.s64 + -23800;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DDF8);
PPC_WEAK_FUNC(sub_82A5DDF8) { __imp__sub_82A5DDF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DDF8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23788
	ctx.r11.s64 = ctx.r11.s64 + -23788;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DE18);
PPC_WEAK_FUNC(sub_82A5DE18) { __imp__sub_82A5DE18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DE18) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23776
	ctx.r11.s64 = ctx.r11.s64 + -23776;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DE38);
PPC_WEAK_FUNC(sub_82A5DE38) { __imp__sub_82A5DE38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DE38) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23764
	ctx.r11.s64 = ctx.r11.s64 + -23764;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DE58);
PPC_WEAK_FUNC(sub_82A5DE58) { __imp__sub_82A5DE58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DE58) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23752
	ctx.r11.s64 = ctx.r11.s64 + -23752;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DE78);
PPC_WEAK_FUNC(sub_82A5DE78) { __imp__sub_82A5DE78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DE78) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23740
	ctx.r11.s64 = ctx.r11.s64 + -23740;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DE98);
PPC_WEAK_FUNC(sub_82A5DE98) { __imp__sub_82A5DE98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DE98) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23728
	ctx.r11.s64 = ctx.r11.s64 + -23728;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DEB8);
PPC_WEAK_FUNC(sub_82A5DEB8) { __imp__sub_82A5DEB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DEB8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23716
	ctx.r11.s64 = ctx.r11.s64 + -23716;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DED8);
PPC_WEAK_FUNC(sub_82A5DED8) { __imp__sub_82A5DED8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DED8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23704
	ctx.r11.s64 = ctx.r11.s64 + -23704;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DEF8);
PPC_WEAK_FUNC(sub_82A5DEF8) { __imp__sub_82A5DEF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DEF8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23692
	ctx.r11.s64 = ctx.r11.s64 + -23692;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DF18);
PPC_WEAK_FUNC(sub_82A5DF18) { __imp__sub_82A5DF18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DF18) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23680
	ctx.r11.s64 = ctx.r11.s64 + -23680;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DF38);
PPC_WEAK_FUNC(sub_82A5DF38) { __imp__sub_82A5DF38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DF38) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23668
	ctx.r11.s64 = ctx.r11.s64 + -23668;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DF58);
PPC_WEAK_FUNC(sub_82A5DF58) { __imp__sub_82A5DF58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DF58) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23656
	ctx.r11.s64 = ctx.r11.s64 + -23656;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DF78);
PPC_WEAK_FUNC(sub_82A5DF78) { __imp__sub_82A5DF78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DF78) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23644
	ctx.r11.s64 = ctx.r11.s64 + -23644;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DF98);
PPC_WEAK_FUNC(sub_82A5DF98) { __imp__sub_82A5DF98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DF98) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23632
	ctx.r11.s64 = ctx.r11.s64 + -23632;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DFB8);
PPC_WEAK_FUNC(sub_82A5DFB8) { __imp__sub_82A5DFB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DFB8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23620
	ctx.r11.s64 = ctx.r11.s64 + -23620;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DFD8);
PPC_WEAK_FUNC(sub_82A5DFD8) { __imp__sub_82A5DFD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DFD8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23608
	ctx.r11.s64 = ctx.r11.s64 + -23608;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5DFF8);
PPC_WEAK_FUNC(sub_82A5DFF8) { __imp__sub_82A5DFF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5DFF8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23596
	ctx.r11.s64 = ctx.r11.s64 + -23596;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E018);
PPC_WEAK_FUNC(sub_82A5E018) { __imp__sub_82A5E018(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E018) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23584
	ctx.r11.s64 = ctx.r11.s64 + -23584;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E038);
PPC_WEAK_FUNC(sub_82A5E038) { __imp__sub_82A5E038(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E038) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23572
	ctx.r11.s64 = ctx.r11.s64 + -23572;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E058);
PPC_WEAK_FUNC(sub_82A5E058) { __imp__sub_82A5E058(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E058) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23560
	ctx.r11.s64 = ctx.r11.s64 + -23560;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E078);
PPC_WEAK_FUNC(sub_82A5E078) { __imp__sub_82A5E078(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E078) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23548
	ctx.r11.s64 = ctx.r11.s64 + -23548;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E098);
PPC_WEAK_FUNC(sub_82A5E098) { __imp__sub_82A5E098(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E098) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23536
	ctx.r11.s64 = ctx.r11.s64 + -23536;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E0B8);
PPC_WEAK_FUNC(sub_82A5E0B8) { __imp__sub_82A5E0B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E0B8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23524
	ctx.r11.s64 = ctx.r11.s64 + -23524;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E0D8);
PPC_WEAK_FUNC(sub_82A5E0D8) { __imp__sub_82A5E0D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E0D8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23512
	ctx.r11.s64 = ctx.r11.s64 + -23512;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E0F8);
PPC_WEAK_FUNC(sub_82A5E0F8) { __imp__sub_82A5E0F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E0F8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23500
	ctx.r11.s64 = ctx.r11.s64 + -23500;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E118);
PPC_WEAK_FUNC(sub_82A5E118) { __imp__sub_82A5E118(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E118) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23488
	ctx.r11.s64 = ctx.r11.s64 + -23488;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E138);
PPC_WEAK_FUNC(sub_82A5E138) { __imp__sub_82A5E138(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E138) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23476
	ctx.r11.s64 = ctx.r11.s64 + -23476;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E158);
PPC_WEAK_FUNC(sub_82A5E158) { __imp__sub_82A5E158(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E158) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23464
	ctx.r11.s64 = ctx.r11.s64 + -23464;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E178);
PPC_WEAK_FUNC(sub_82A5E178) { __imp__sub_82A5E178(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E178) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23452
	ctx.r11.s64 = ctx.r11.s64 + -23452;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E198);
PPC_WEAK_FUNC(sub_82A5E198) { __imp__sub_82A5E198(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E198) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23440
	ctx.r11.s64 = ctx.r11.s64 + -23440;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E1B8);
PPC_WEAK_FUNC(sub_82A5E1B8) { __imp__sub_82A5E1B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E1B8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23428
	ctx.r11.s64 = ctx.r11.s64 + -23428;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E1D8);
PPC_WEAK_FUNC(sub_82A5E1D8) { __imp__sub_82A5E1D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E1D8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23416
	ctx.r11.s64 = ctx.r11.s64 + -23416;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E1F8);
PPC_WEAK_FUNC(sub_82A5E1F8) { __imp__sub_82A5E1F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E1F8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23404
	ctx.r11.s64 = ctx.r11.s64 + -23404;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E218);
PPC_WEAK_FUNC(sub_82A5E218) { __imp__sub_82A5E218(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E218) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23392
	ctx.r11.s64 = ctx.r11.s64 + -23392;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E238);
PPC_WEAK_FUNC(sub_82A5E238) { __imp__sub_82A5E238(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E238) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23380
	ctx.r11.s64 = ctx.r11.s64 + -23380;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E258);
PPC_WEAK_FUNC(sub_82A5E258) { __imp__sub_82A5E258(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E258) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23368
	ctx.r11.s64 = ctx.r11.s64 + -23368;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E278);
PPC_WEAK_FUNC(sub_82A5E278) { __imp__sub_82A5E278(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E278) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23356
	ctx.r11.s64 = ctx.r11.s64 + -23356;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E298);
PPC_WEAK_FUNC(sub_82A5E298) { __imp__sub_82A5E298(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E298) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23344
	ctx.r11.s64 = ctx.r11.s64 + -23344;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E2B8);
PPC_WEAK_FUNC(sub_82A5E2B8) { __imp__sub_82A5E2B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E2B8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23332
	ctx.r11.s64 = ctx.r11.s64 + -23332;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E2D8);
PPC_WEAK_FUNC(sub_82A5E2D8) { __imp__sub_82A5E2D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E2D8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23320
	ctx.r11.s64 = ctx.r11.s64 + -23320;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E2F8);
PPC_WEAK_FUNC(sub_82A5E2F8) { __imp__sub_82A5E2F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E2F8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23308
	ctx.r11.s64 = ctx.r11.s64 + -23308;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E318);
PPC_WEAK_FUNC(sub_82A5E318) { __imp__sub_82A5E318(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E318) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23296
	ctx.r11.s64 = ctx.r11.s64 + -23296;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E338);
PPC_WEAK_FUNC(sub_82A5E338) { __imp__sub_82A5E338(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E338) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23284
	ctx.r11.s64 = ctx.r11.s64 + -23284;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E358);
PPC_WEAK_FUNC(sub_82A5E358) { __imp__sub_82A5E358(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E358) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23272
	ctx.r11.s64 = ctx.r11.s64 + -23272;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E378);
PPC_WEAK_FUNC(sub_82A5E378) { __imp__sub_82A5E378(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E378) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23260
	ctx.r11.s64 = ctx.r11.s64 + -23260;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E398);
PPC_WEAK_FUNC(sub_82A5E398) { __imp__sub_82A5E398(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E398) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23248
	ctx.r11.s64 = ctx.r11.s64 + -23248;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E3B8);
PPC_WEAK_FUNC(sub_82A5E3B8) { __imp__sub_82A5E3B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E3B8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23236
	ctx.r11.s64 = ctx.r11.s64 + -23236;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E3D8);
PPC_WEAK_FUNC(sub_82A5E3D8) { __imp__sub_82A5E3D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E3D8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23224
	ctx.r11.s64 = ctx.r11.s64 + -23224;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E3F8);
PPC_WEAK_FUNC(sub_82A5E3F8) { __imp__sub_82A5E3F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E3F8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23212
	ctx.r11.s64 = ctx.r11.s64 + -23212;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E418);
PPC_WEAK_FUNC(sub_82A5E418) { __imp__sub_82A5E418(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E418) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23200
	ctx.r11.s64 = ctx.r11.s64 + -23200;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E438);
PPC_WEAK_FUNC(sub_82A5E438) { __imp__sub_82A5E438(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E438) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23188
	ctx.r11.s64 = ctx.r11.s64 + -23188;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E458);
PPC_WEAK_FUNC(sub_82A5E458) { __imp__sub_82A5E458(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E458) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23176
	ctx.r11.s64 = ctx.r11.s64 + -23176;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E478);
PPC_WEAK_FUNC(sub_82A5E478) { __imp__sub_82A5E478(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E478) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23164
	ctx.r11.s64 = ctx.r11.s64 + -23164;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E498);
PPC_WEAK_FUNC(sub_82A5E498) { __imp__sub_82A5E498(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E498) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23152
	ctx.r11.s64 = ctx.r11.s64 + -23152;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E4B8);
PPC_WEAK_FUNC(sub_82A5E4B8) { __imp__sub_82A5E4B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E4B8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23140
	ctx.r11.s64 = ctx.r11.s64 + -23140;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E4D8);
PPC_WEAK_FUNC(sub_82A5E4D8) { __imp__sub_82A5E4D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E4D8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23128
	ctx.r11.s64 = ctx.r11.s64 + -23128;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E4F8);
PPC_WEAK_FUNC(sub_82A5E4F8) { __imp__sub_82A5E4F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E4F8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23116
	ctx.r11.s64 = ctx.r11.s64 + -23116;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E518);
PPC_WEAK_FUNC(sub_82A5E518) { __imp__sub_82A5E518(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E518) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23104
	ctx.r11.s64 = ctx.r11.s64 + -23104;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E538);
PPC_WEAK_FUNC(sub_82A5E538) { __imp__sub_82A5E538(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E538) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23092
	ctx.r11.s64 = ctx.r11.s64 + -23092;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E558);
PPC_WEAK_FUNC(sub_82A5E558) { __imp__sub_82A5E558(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E558) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23080
	ctx.r11.s64 = ctx.r11.s64 + -23080;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E578);
PPC_WEAK_FUNC(sub_82A5E578) { __imp__sub_82A5E578(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E578) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23068
	ctx.r11.s64 = ctx.r11.s64 + -23068;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E598);
PPC_WEAK_FUNC(sub_82A5E598) { __imp__sub_82A5E598(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E598) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23056
	ctx.r11.s64 = ctx.r11.s64 + -23056;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E5B8);
PPC_WEAK_FUNC(sub_82A5E5B8) { __imp__sub_82A5E5B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E5B8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23044
	ctx.r11.s64 = ctx.r11.s64 + -23044;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E5D8);
PPC_WEAK_FUNC(sub_82A5E5D8) { __imp__sub_82A5E5D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E5D8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23032
	ctx.r11.s64 = ctx.r11.s64 + -23032;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E5F8);
PPC_WEAK_FUNC(sub_82A5E5F8) { __imp__sub_82A5E5F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E5F8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23020
	ctx.r11.s64 = ctx.r11.s64 + -23020;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E618);
PPC_WEAK_FUNC(sub_82A5E618) { __imp__sub_82A5E618(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E618) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-23008
	ctx.r11.s64 = ctx.r11.s64 + -23008;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E638);
PPC_WEAK_FUNC(sub_82A5E638) { __imp__sub_82A5E638(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E638) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22996
	ctx.r11.s64 = ctx.r11.s64 + -22996;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E658);
PPC_WEAK_FUNC(sub_82A5E658) { __imp__sub_82A5E658(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E658) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22984
	ctx.r11.s64 = ctx.r11.s64 + -22984;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E678);
PPC_WEAK_FUNC(sub_82A5E678) { __imp__sub_82A5E678(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E678) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22972
	ctx.r11.s64 = ctx.r11.s64 + -22972;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E698);
PPC_WEAK_FUNC(sub_82A5E698) { __imp__sub_82A5E698(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E698) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22960
	ctx.r11.s64 = ctx.r11.s64 + -22960;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E6B8);
PPC_WEAK_FUNC(sub_82A5E6B8) { __imp__sub_82A5E6B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E6B8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22948
	ctx.r11.s64 = ctx.r11.s64 + -22948;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E6D8);
PPC_WEAK_FUNC(sub_82A5E6D8) { __imp__sub_82A5E6D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E6D8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22936
	ctx.r11.s64 = ctx.r11.s64 + -22936;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E6F8);
PPC_WEAK_FUNC(sub_82A5E6F8) { __imp__sub_82A5E6F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E6F8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22924
	ctx.r11.s64 = ctx.r11.s64 + -22924;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E718);
PPC_WEAK_FUNC(sub_82A5E718) { __imp__sub_82A5E718(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E718) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22912
	ctx.r11.s64 = ctx.r11.s64 + -22912;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E738);
PPC_WEAK_FUNC(sub_82A5E738) { __imp__sub_82A5E738(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E738) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22900
	ctx.r11.s64 = ctx.r11.s64 + -22900;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E758);
PPC_WEAK_FUNC(sub_82A5E758) { __imp__sub_82A5E758(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E758) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22888
	ctx.r11.s64 = ctx.r11.s64 + -22888;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E778);
PPC_WEAK_FUNC(sub_82A5E778) { __imp__sub_82A5E778(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E778) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22876
	ctx.r11.s64 = ctx.r11.s64 + -22876;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E798);
PPC_WEAK_FUNC(sub_82A5E798) { __imp__sub_82A5E798(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E798) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22864
	ctx.r11.s64 = ctx.r11.s64 + -22864;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E7B8);
PPC_WEAK_FUNC(sub_82A5E7B8) { __imp__sub_82A5E7B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E7B8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22852
	ctx.r11.s64 = ctx.r11.s64 + -22852;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E7D8);
PPC_WEAK_FUNC(sub_82A5E7D8) { __imp__sub_82A5E7D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E7D8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22840
	ctx.r11.s64 = ctx.r11.s64 + -22840;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E7F8);
PPC_WEAK_FUNC(sub_82A5E7F8) { __imp__sub_82A5E7F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E7F8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22828
	ctx.r11.s64 = ctx.r11.s64 + -22828;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E818);
PPC_WEAK_FUNC(sub_82A5E818) { __imp__sub_82A5E818(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E818) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22816
	ctx.r11.s64 = ctx.r11.s64 + -22816;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E838);
PPC_WEAK_FUNC(sub_82A5E838) { __imp__sub_82A5E838(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E838) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22804
	ctx.r11.s64 = ctx.r11.s64 + -22804;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E858);
PPC_WEAK_FUNC(sub_82A5E858) { __imp__sub_82A5E858(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E858) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22792
	ctx.r11.s64 = ctx.r11.s64 + -22792;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E878);
PPC_WEAK_FUNC(sub_82A5E878) { __imp__sub_82A5E878(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E878) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22780
	ctx.r11.s64 = ctx.r11.s64 + -22780;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E898);
PPC_WEAK_FUNC(sub_82A5E898) { __imp__sub_82A5E898(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E898) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22768
	ctx.r11.s64 = ctx.r11.s64 + -22768;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E8B8);
PPC_WEAK_FUNC(sub_82A5E8B8) { __imp__sub_82A5E8B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E8B8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22756
	ctx.r11.s64 = ctx.r11.s64 + -22756;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E8D8);
PPC_WEAK_FUNC(sub_82A5E8D8) { __imp__sub_82A5E8D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E8D8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22744
	ctx.r11.s64 = ctx.r11.s64 + -22744;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E8F8);
PPC_WEAK_FUNC(sub_82A5E8F8) { __imp__sub_82A5E8F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E8F8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22732
	ctx.r11.s64 = ctx.r11.s64 + -22732;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E918);
PPC_WEAK_FUNC(sub_82A5E918) { __imp__sub_82A5E918(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E918) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22720
	ctx.r11.s64 = ctx.r11.s64 + -22720;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E938);
PPC_WEAK_FUNC(sub_82A5E938) { __imp__sub_82A5E938(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E938) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22708
	ctx.r11.s64 = ctx.r11.s64 + -22708;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E958);
PPC_WEAK_FUNC(sub_82A5E958) { __imp__sub_82A5E958(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E958) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22696
	ctx.r11.s64 = ctx.r11.s64 + -22696;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E978);
PPC_WEAK_FUNC(sub_82A5E978) { __imp__sub_82A5E978(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E978) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22684
	ctx.r11.s64 = ctx.r11.s64 + -22684;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E998);
PPC_WEAK_FUNC(sub_82A5E998) { __imp__sub_82A5E998(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E998) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22672
	ctx.r11.s64 = ctx.r11.s64 + -22672;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E9B8);
PPC_WEAK_FUNC(sub_82A5E9B8) { __imp__sub_82A5E9B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E9B8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22660
	ctx.r11.s64 = ctx.r11.s64 + -22660;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E9D8);
PPC_WEAK_FUNC(sub_82A5E9D8) { __imp__sub_82A5E9D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E9D8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22648
	ctx.r11.s64 = ctx.r11.s64 + -22648;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5E9F8);
PPC_WEAK_FUNC(sub_82A5E9F8) { __imp__sub_82A5E9F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5E9F8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22636
	ctx.r11.s64 = ctx.r11.s64 + -22636;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EA18);
PPC_WEAK_FUNC(sub_82A5EA18) { __imp__sub_82A5EA18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EA18) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22624
	ctx.r11.s64 = ctx.r11.s64 + -22624;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EA38);
PPC_WEAK_FUNC(sub_82A5EA38) { __imp__sub_82A5EA38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EA38) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22612
	ctx.r11.s64 = ctx.r11.s64 + -22612;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EA58);
PPC_WEAK_FUNC(sub_82A5EA58) { __imp__sub_82A5EA58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EA58) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22600
	ctx.r11.s64 = ctx.r11.s64 + -22600;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EA78);
PPC_WEAK_FUNC(sub_82A5EA78) { __imp__sub_82A5EA78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EA78) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22588
	ctx.r11.s64 = ctx.r11.s64 + -22588;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EA98);
PPC_WEAK_FUNC(sub_82A5EA98) { __imp__sub_82A5EA98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EA98) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22576
	ctx.r11.s64 = ctx.r11.s64 + -22576;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EAB8);
PPC_WEAK_FUNC(sub_82A5EAB8) { __imp__sub_82A5EAB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EAB8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22564
	ctx.r11.s64 = ctx.r11.s64 + -22564;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EAD8);
PPC_WEAK_FUNC(sub_82A5EAD8) { __imp__sub_82A5EAD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EAD8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22552
	ctx.r11.s64 = ctx.r11.s64 + -22552;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EAF8);
PPC_WEAK_FUNC(sub_82A5EAF8) { __imp__sub_82A5EAF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EAF8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22540
	ctx.r11.s64 = ctx.r11.s64 + -22540;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EB18);
PPC_WEAK_FUNC(sub_82A5EB18) { __imp__sub_82A5EB18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EB18) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22528
	ctx.r11.s64 = ctx.r11.s64 + -22528;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EB38);
PPC_WEAK_FUNC(sub_82A5EB38) { __imp__sub_82A5EB38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EB38) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22516
	ctx.r11.s64 = ctx.r11.s64 + -22516;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EB58);
PPC_WEAK_FUNC(sub_82A5EB58) { __imp__sub_82A5EB58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EB58) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22504
	ctx.r11.s64 = ctx.r11.s64 + -22504;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EB78);
PPC_WEAK_FUNC(sub_82A5EB78) { __imp__sub_82A5EB78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EB78) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22492
	ctx.r11.s64 = ctx.r11.s64 + -22492;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EB98);
PPC_WEAK_FUNC(sub_82A5EB98) { __imp__sub_82A5EB98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EB98) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22480
	ctx.r11.s64 = ctx.r11.s64 + -22480;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EBB8);
PPC_WEAK_FUNC(sub_82A5EBB8) { __imp__sub_82A5EBB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EBB8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22468
	ctx.r11.s64 = ctx.r11.s64 + -22468;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EBD8);
PPC_WEAK_FUNC(sub_82A5EBD8) { __imp__sub_82A5EBD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EBD8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22456
	ctx.r11.s64 = ctx.r11.s64 + -22456;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EBF8);
PPC_WEAK_FUNC(sub_82A5EBF8) { __imp__sub_82A5EBF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EBF8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22444
	ctx.r11.s64 = ctx.r11.s64 + -22444;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EC18);
PPC_WEAK_FUNC(sub_82A5EC18) { __imp__sub_82A5EC18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EC18) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22432
	ctx.r11.s64 = ctx.r11.s64 + -22432;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EC38);
PPC_WEAK_FUNC(sub_82A5EC38) { __imp__sub_82A5EC38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EC38) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22420
	ctx.r11.s64 = ctx.r11.s64 + -22420;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EC58);
PPC_WEAK_FUNC(sub_82A5EC58) { __imp__sub_82A5EC58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EC58) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22408
	ctx.r11.s64 = ctx.r11.s64 + -22408;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EC78);
PPC_WEAK_FUNC(sub_82A5EC78) { __imp__sub_82A5EC78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EC78) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22396
	ctx.r11.s64 = ctx.r11.s64 + -22396;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EC98);
PPC_WEAK_FUNC(sub_82A5EC98) { __imp__sub_82A5EC98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EC98) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22384
	ctx.r11.s64 = ctx.r11.s64 + -22384;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5ECB8);
PPC_WEAK_FUNC(sub_82A5ECB8) { __imp__sub_82A5ECB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5ECB8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22372
	ctx.r11.s64 = ctx.r11.s64 + -22372;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5ECD8);
PPC_WEAK_FUNC(sub_82A5ECD8) { __imp__sub_82A5ECD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5ECD8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22360
	ctx.r11.s64 = ctx.r11.s64 + -22360;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5ECF8);
PPC_WEAK_FUNC(sub_82A5ECF8) { __imp__sub_82A5ECF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5ECF8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22348
	ctx.r11.s64 = ctx.r11.s64 + -22348;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5ED18);
PPC_WEAK_FUNC(sub_82A5ED18) { __imp__sub_82A5ED18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5ED18) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22336
	ctx.r11.s64 = ctx.r11.s64 + -22336;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5ED38);
PPC_WEAK_FUNC(sub_82A5ED38) { __imp__sub_82A5ED38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5ED38) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22324
	ctx.r11.s64 = ctx.r11.s64 + -22324;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5ED58);
PPC_WEAK_FUNC(sub_82A5ED58) { __imp__sub_82A5ED58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5ED58) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22312
	ctx.r11.s64 = ctx.r11.s64 + -22312;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5ED78);
PPC_WEAK_FUNC(sub_82A5ED78) { __imp__sub_82A5ED78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5ED78) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22300
	ctx.r11.s64 = ctx.r11.s64 + -22300;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5ED98);
PPC_WEAK_FUNC(sub_82A5ED98) { __imp__sub_82A5ED98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5ED98) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22288
	ctx.r11.s64 = ctx.r11.s64 + -22288;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EDB8);
PPC_WEAK_FUNC(sub_82A5EDB8) { __imp__sub_82A5EDB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EDB8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22276
	ctx.r11.s64 = ctx.r11.s64 + -22276;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EDD8);
PPC_WEAK_FUNC(sub_82A5EDD8) { __imp__sub_82A5EDD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EDD8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22264
	ctx.r11.s64 = ctx.r11.s64 + -22264;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EDF8);
PPC_WEAK_FUNC(sub_82A5EDF8) { __imp__sub_82A5EDF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EDF8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22252
	ctx.r11.s64 = ctx.r11.s64 + -22252;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EE18);
PPC_WEAK_FUNC(sub_82A5EE18) { __imp__sub_82A5EE18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EE18) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22240
	ctx.r11.s64 = ctx.r11.s64 + -22240;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EE38);
PPC_WEAK_FUNC(sub_82A5EE38) { __imp__sub_82A5EE38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EE38) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22228
	ctx.r11.s64 = ctx.r11.s64 + -22228;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EE58);
PPC_WEAK_FUNC(sub_82A5EE58) { __imp__sub_82A5EE58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EE58) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22216
	ctx.r11.s64 = ctx.r11.s64 + -22216;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EE78);
PPC_WEAK_FUNC(sub_82A5EE78) { __imp__sub_82A5EE78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EE78) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22204
	ctx.r11.s64 = ctx.r11.s64 + -22204;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EE98);
PPC_WEAK_FUNC(sub_82A5EE98) { __imp__sub_82A5EE98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EE98) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22192
	ctx.r11.s64 = ctx.r11.s64 + -22192;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EEB8);
PPC_WEAK_FUNC(sub_82A5EEB8) { __imp__sub_82A5EEB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EEB8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22180
	ctx.r11.s64 = ctx.r11.s64 + -22180;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EED8);
PPC_WEAK_FUNC(sub_82A5EED8) { __imp__sub_82A5EED8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EED8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22168
	ctx.r11.s64 = ctx.r11.s64 + -22168;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EEF8);
PPC_WEAK_FUNC(sub_82A5EEF8) { __imp__sub_82A5EEF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EEF8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22156
	ctx.r11.s64 = ctx.r11.s64 + -22156;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EF18);
PPC_WEAK_FUNC(sub_82A5EF18) { __imp__sub_82A5EF18(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EF18) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22144
	ctx.r11.s64 = ctx.r11.s64 + -22144;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EF38);
PPC_WEAK_FUNC(sub_82A5EF38) { __imp__sub_82A5EF38(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EF38) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22132
	ctx.r11.s64 = ctx.r11.s64 + -22132;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EF58);
PPC_WEAK_FUNC(sub_82A5EF58) { __imp__sub_82A5EF58(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EF58) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22120
	ctx.r11.s64 = ctx.r11.s64 + -22120;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EF78);
PPC_WEAK_FUNC(sub_82A5EF78) { __imp__sub_82A5EF78(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EF78) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22108
	ctx.r11.s64 = ctx.r11.s64 + -22108;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EF98);
PPC_WEAK_FUNC(sub_82A5EF98) { __imp__sub_82A5EF98(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EF98) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22096
	ctx.r11.s64 = ctx.r11.s64 + -22096;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EFB8);
PPC_WEAK_FUNC(sub_82A5EFB8) { __imp__sub_82A5EFB8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EFB8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22084
	ctx.r11.s64 = ctx.r11.s64 + -22084;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EFD8);
PPC_WEAK_FUNC(sub_82A5EFD8) { __imp__sub_82A5EFD8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EFD8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22072
	ctx.r11.s64 = ctx.r11.s64 + -22072;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5EFF8);
PPC_WEAK_FUNC(sub_82A5EFF8) { __imp__sub_82A5EFF8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5EFF8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22060
	ctx.r11.s64 = ctx.r11.s64 + -22060;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F018);
PPC_WEAK_FUNC(sub_82A5F018) { __imp__sub_82A5F018(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F018) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22048
	ctx.r11.s64 = ctx.r11.s64 + -22048;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F038);
PPC_WEAK_FUNC(sub_82A5F038) { __imp__sub_82A5F038(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F038) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22036
	ctx.r11.s64 = ctx.r11.s64 + -22036;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F058);
PPC_WEAK_FUNC(sub_82A5F058) { __imp__sub_82A5F058(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F058) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22024
	ctx.r11.s64 = ctx.r11.s64 + -22024;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F078);
PPC_WEAK_FUNC(sub_82A5F078) { __imp__sub_82A5F078(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F078) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22012
	ctx.r11.s64 = ctx.r11.s64 + -22012;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F098);
PPC_WEAK_FUNC(sub_82A5F098) { __imp__sub_82A5F098(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F098) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-22000
	ctx.r11.s64 = ctx.r11.s64 + -22000;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F0B8);
PPC_WEAK_FUNC(sub_82A5F0B8) { __imp__sub_82A5F0B8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F0B8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32044
	ctx.r10.s64 = -2100035584;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,-21988
	ctx.r11.s64 = ctx.r11.s64 + -21988;
	// lwz r9,30208(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 30208);
	// stw r11,30208(r10)
	PPC_STORE_U32(ctx.r10.u32 + 30208, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F0D8);
PPC_WEAK_FUNC(sub_82A5F0D8) { __imp__sub_82A5F0D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F0D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,976
	ctx.r3.s64 = ctx.r11.s64 + 976;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F0E8);
PPC_WEAK_FUNC(sub_82A5F0E8) { __imp__sub_82A5F0E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F0E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,6696
	ctx.r5.s64 = ctx.r11.s64 + 6696;
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,5540
	ctx.r3.s64 = ctx.r11.s64 + 5540;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F108);
PPC_WEAK_FUNC(sub_82A5F108) { __imp__sub_82A5F108(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F108) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-8764
	ctx.r5.s64 = ctx.r11.s64 + -8764;
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,8064
	ctx.r3.s64 = ctx.r11.s64 + 8064;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F128);
PPC_WEAK_FUNC(sub_82A5F128) { __imp__sub_82A5F128(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F128) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r31,r11,8096
	ctx.r31.s64 = ctx.r11.s64 + 8096;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82902b70
	ctx.lr = 0x82A5F148;
	sub_82902B70(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// lis r10,-32255
	ctx.r10.s64 = -2113863680;
	// addi r11,r11,-8500
	ctx.r11.s64 = ctx.r11.s64 + -8500;
	// addi r10,r10,-8572
	ctx.r10.s64 = ctx.r10.s64 + -8572;
	// addi r3,r31,48
	ctx.r3.s64 = ctx.r31.s64 + 48;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r10,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r10.u32);
	// stw r11,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r11.u32);
	// bl 0x82915100
	ctx.lr = 0x82A5F170;
	sub_82915100(ctx, base);
	// addi r3,r31,76
	ctx.r3.s64 = ctx.r31.s64 + 76;
	// bl 0x82915100
	ctx.lr = 0x82A5F178;
	sub_82915100(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,1000
	ctx.r3.s64 = ctx.r11.s64 + 1000;
	// bl 0x829ffa48
	ctx.lr = 0x82A5F184;
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

PPC_FUNC_IMPL(__imp__sub_82A5F198);
PPC_WEAK_FUNC(sub_82A5F198) { __imp__sub_82A5F198(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F198) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-8344
	ctx.r3.s64 = ctx.r11.s64 + -8344;
	// bl 0x8284e060
	ctx.lr = 0x82A5F1B4;
	sub_8284E060(ctx, base);
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// stw r3,8232(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8232, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F1D0);
PPC_WEAK_FUNC(sub_82A5F1D0) { __imp__sub_82A5F1D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F1D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-8336
	ctx.r5.s64 = ctx.r11.s64 + -8336;
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,8236
	ctx.r3.s64 = ctx.r11.s64 + 8236;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F1F0);
PPC_WEAK_FUNC(sub_82A5F1F0) { __imp__sub_82A5F1F0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F1F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-8276
	ctx.r5.s64 = ctx.r11.s64 + -8276;
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,8276
	ctx.r3.s64 = ctx.r11.s64 + 8276;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F210);
PPC_WEAK_FUNC(sub_82A5F210) { __imp__sub_82A5F210(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F210) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// vspltisw v0,0
	simde_mm_store_si128((simde__m128i*)ctx.v0.u32, simde_mm_set1_epi32(int(0x0)));
	// lis r8,-32256
	ctx.r8.s64 = -2113929216;
	// lis r9,-32057
	ctx.r9.s64 = -2100887552;
	// addi r11,r11,8320
	ctx.r11.s64 = ctx.r11.s64 + 8320;
	// li r7,31
	ctx.r7.s64 = 31;
	// addi r11,r11,64
	ctx.r11.s64 = ctx.r11.s64 + 64;
	// lfs f0,2612(r8)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// li r10,0
	ctx.r10.s64 = 0;
	// lwz r6,-15708(r9)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r9.u32 + -15708);
	// li r4,-48
	ctx.r4.s64 = -48;
	// li r5,1
	ctx.r5.s64 = 1;
loc_82A5F240:
	// stw r10,-8(r11)
	PPC_STORE_U32(ctx.r11.u32 + -8, ctx.r10.u32);
	// stw r10,-4(r11)
	PPC_STORE_U32(ctx.r11.u32 + -4, ctx.r10.u32);
	// stb r10,0(r11)
	PPC_STORE_U8(ctx.r11.u32 + 0, ctx.r10.u8);
	// stb r10,1(r11)
	PPC_STORE_U8(ctx.r11.u32 + 1, ctx.r10.u8);
	// stfs f0,-32(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -32, temp.u32);
	// stw r10,-64(r11)
	PPC_STORE_U32(ctx.r11.u32 + -64, ctx.r10.u32);
	// stvx128 v0,r11,r4
	ea = (ctx.r11.u32 + ctx.r4.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// sth r10,-28(r11)
	PPC_STORE_U16(ctx.r11.u32 + -28, ctx.r10.u16);
	// stb r10,-26(r11)
	PPC_STORE_U8(ctx.r11.u32 + -26, ctx.r10.u8);
	// stb r10,-25(r11)
	PPC_STORE_U8(ctx.r11.u32 + -25, ctx.r10.u8);
	// stw r10,-24(r11)
	PPC_STORE_U32(ctx.r11.u32 + -24, ctx.r10.u32);
	// stw r10,-20(r11)
	PPC_STORE_U32(ctx.r11.u32 + -20, ctx.r10.u32);
	// stw r10,-16(r11)
	PPC_STORE_U32(ctx.r11.u32 + -16, ctx.r10.u32);
	// stw r10,-12(r11)
	PPC_STORE_U32(ctx.r11.u32 + -12, ctx.r10.u32);
	// lbz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// beq cr6,0x82a5f29c
	if (ctx.cr6.eq) goto loc_82A5F29C;
	// lwz r9,-8(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + -8);
	// lwz r8,-4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + -4);
	// subf r9,r9,r6
	ctx.r9.u64 = ctx.r6.u64 - ctx.r9.u64;
	// stb r5,1(r11)
	PPC_STORE_U8(ctx.r11.u32 + 1, ctx.r5.u8);
	// subf r9,r9,r8
	ctx.r9.u64 = ctx.r8.u64 - ctx.r9.u64;
	// stw r9,-4(r11)
	PPC_STORE_U32(ctx.r11.u32 + -4, ctx.r9.u32);
loc_82A5F29C:
	// addi r7,r7,-1
	ctx.r7.s64 = ctx.r7.s64 + -1;
	// stw r10,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
	// addi r11,r11,80
	ctx.r11.s64 = ctx.r11.s64 + 80;
	// cmpwi cr6,r7,0
	ctx.cr6.compare<int32_t>(ctx.r7.s32, 0, ctx.xer);
	// bge cr6,0x82a5f240
	if (!ctx.cr6.lt) goto loc_82A5F240;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,1040
	ctx.r3.s64 = ctx.r11.s64 + 1040;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F2C0);
PPC_WEAK_FUNC(sub_82A5F2C0) { __imp__sub_82A5F2C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F2C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c0
	ctx.lr = 0x82A5F2C8;
	__savegprlr_26(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// li r29,30
	ctx.r29.s64 = 30;
	// addi r11,r11,10880
	ctx.r11.s64 = ctx.r11.s64 + 10880;
	// li r30,0
	ctx.r30.s64 = 0;
	// addi r31,r11,4
	ctx.r31.s64 = ctx.r11.s64 + 4;
	// li r26,-1
	ctx.r26.s64 = -1;
	// li r27,1000
	ctx.r27.s64 = 1000;
	// li r28,2000
	ctx.r28.s64 = 2000;
loc_82A5F2EC:
	// lis r11,-32210
	ctx.r11.s64 = -2110914560;
	// li r5,4
	ctx.r5.s64 = 4;
	// addi r6,r11,-24320
	ctx.r6.s64 = ctx.r11.s64 + -24320;
	// li r4,6
	ctx.r4.s64 = 6;
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x82140220
	ctx.lr = 0x82A5F304;
	sub_82140220(ctx, base);
	// addi r29,r29,-1
	ctx.r29.s64 = ctx.r29.s64 + -1;
	// stw r30,-4(r31)
	PPC_STORE_U32(ctx.r31.u32 + -4, ctx.r30.u32);
	// stw r26,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r26.u32);
	// cmpwi cr6,r29,0
	ctx.cr6.compare<int32_t>(ctx.r29.s32, 0, ctx.xer);
	// stw r30,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r30.u32);
	// stw r30,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r30.u32);
	// stw r30,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r30.u32);
	// stw r27,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r27.u32);
	// stw r28,20(r31)
	PPC_STORE_U32(ctx.r31.u32 + 20, ctx.r28.u32);
	// addi r31,r31,52
	ctx.r31.s64 = ctx.r31.s64 + 52;
	// bge cr6,0x82a5f2ec
	if (!ctx.cr6.lt) goto loc_82A5F2EC;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F338);
PPC_WEAK_FUNC(sub_82A5F338) { __imp__sub_82A5F338(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F338) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,1152
	ctx.r3.s64 = ctx.r11.s64 + 1152;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F348);
PPC_WEAK_FUNC(sub_82A5F348) { __imp__sub_82A5F348(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F348) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,1240
	ctx.r3.s64 = ctx.r11.s64 + 1240;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F358);
PPC_WEAK_FUNC(sub_82A5F358) { __imp__sub_82A5F358(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F358) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,1160
	ctx.r3.s64 = ctx.r11.s64 + 1160;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F368);
PPC_WEAK_FUNC(sub_82A5F368) { __imp__sub_82A5F368(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F368) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r3,0
	ctx.r3.s64 = 0;
	// bl 0x821b3560
	ctx.lr = 0x82A5F37C;
	sub_821B3560(ctx, base);
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,12672
	ctx.r11.s64 = ctx.r11.s64 + 12672;
	// lis r9,-32089
	ctx.r9.s64 = -2102984704;
	// addi r3,r9,1168
	ctx.r3.s64 = ctx.r9.s64 + 1168;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// sth r10,4(r11)
	PPC_STORE_U16(ctx.r11.u32 + 4, ctx.r10.u16);
	// sth r10,6(r11)
	PPC_STORE_U16(ctx.r11.u32 + 6, ctx.r10.u16);
	// bl 0x829ffa48
	ctx.lr = 0x82A5F3A0;
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

PPC_FUNC_IMPL(__imp__sub_82A5F3B0);
PPC_WEAK_FUNC(sub_82A5F3B0) { __imp__sub_82A5F3B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F3B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r4,r11,-4452
	ctx.r4.s64 = ctx.r11.s64 + -4452;
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// addi r3,r11,14624
	ctx.r3.s64 = ctx.r11.s64 + 14624;
	// b 0x822d24c0
	sub_822D24C0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F3C8);
PPC_WEAK_FUNC(sub_82A5F3C8) { __imp__sub_82A5F3C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F3C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r4,r11,-4436
	ctx.r4.s64 = ctx.r11.s64 + -4436;
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// addi r3,r11,14636
	ctx.r3.s64 = ctx.r11.s64 + 14636;
	// b 0x822d24c0
	sub_822D24C0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F3E0);
PPC_WEAK_FUNC(sub_82A5F3E0) { __imp__sub_82A5F3E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F3E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r4,r11,-4420
	ctx.r4.s64 = ctx.r11.s64 + -4420;
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// addi r3,r11,14648
	ctx.r3.s64 = ctx.r11.s64 + 14648;
	// b 0x822d24c0
	sub_822D24C0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F3F8);
PPC_WEAK_FUNC(sub_82A5F3F8) { __imp__sub_82A5F3F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F3F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,1256
	ctx.r3.s64 = ctx.r11.s64 + 1256;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F408);
PPC_WEAK_FUNC(sub_82A5F408) { __imp__sub_82A5F408(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F408) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,1288
	ctx.r3.s64 = ctx.r11.s64 + 1288;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F418);
PPC_WEAK_FUNC(sub_82A5F418) { __imp__sub_82A5F418(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F418) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,1320
	ctx.r3.s64 = ctx.r11.s64 + 1320;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F428);
PPC_WEAK_FUNC(sub_82A5F428) { __imp__sub_82A5F428(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F428) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,1352
	ctx.r3.s64 = ctx.r11.s64 + 1352;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F438);
PPC_WEAK_FUNC(sub_82A5F438) { __imp__sub_82A5F438(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F438) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// lfs f0,-3160(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -3160);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// fsqrts f13,f0
	ctx.f13.f64 = double(float(sqrt(ctx.f0.f64)));
	// lfs f0,3428(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3428);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// fmuls f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
	// stfs f0,19148(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 19148, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F460);
PPC_WEAK_FUNC(sub_82A5F460) { __imp__sub_82A5F460(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F460) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// lfs f0,-3160(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -3160);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// fsqrts f13,f0
	ctx.f13.f64 = double(float(sqrt(ctx.f0.f64)));
	// lfs f0,3428(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3428);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// fmuls f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
	// stfs f0,21200(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 21200, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F488);
PPC_WEAK_FUNC(sub_82A5F488) { __imp__sub_82A5F488(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F488) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c4
	ctx.lr = 0x82A5F490;
	__savegprlr_27(ctx, base);
	// stfd f31,-56(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -56, ctx.f31.u64);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r3,48
	ctx.r3.s64 = 48;
	// bl 0x821b3510
	ctx.lr = 0x82A5F4A0;
	sub_821B3510(ctx, base);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// li r29,15
	ctx.r29.s64 = 15;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// lfs f31,3400(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3400);
	ctx.f31.f64 = double(temp.f32);
	// beq cr6,0x82a5f4e0
	if (ctx.cr6.eq) goto loc_82A5F4E0;
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stfs f31,0(r30)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r30.u32 + 0, temp.u32);
	// addi r3,r30,8
	ctx.r3.s64 = ctx.r30.s64 + 8;
	// stw r29,4(r30)
	PPC_STORE_U32(ctx.r30.u32 + 4, ctx.r29.u32);
	// addi r4,r11,-2912
	ctx.r4.s64 = ctx.r11.s64 + -2912;
	// bl 0x8286a370
	ctx.lr = 0x82A5F4D0;
	sub_8286A370(ctx, base);
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// addi r31,r11,21212
	ctx.r31.s64 = ctx.r11.s64 + 21212;
	// stw r30,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r30.u32);
	// b 0x82a5f4f0
	goto loc_82A5F4F0;
loc_82A5F4E0:
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// addi r31,r11,21212
	ctx.r31.s64 = ctx.r11.s64 + 21212;
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
loc_82A5F4F0:
	// li r3,48
	ctx.r3.s64 = 48;
	// bl 0x821b3510
	ctx.lr = 0x82A5F4F8;
	sub_821B3510(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// beq cr6,0x82a5f524
	if (ctx.cr6.eq) goto loc_82A5F524;
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stfs f31,0(r30)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r30.u32 + 0, temp.u32);
	// addi r3,r30,8
	ctx.r3.s64 = ctx.r30.s64 + 8;
	// stw r29,4(r30)
	PPC_STORE_U32(ctx.r30.u32 + 4, ctx.r29.u32);
	// addi r4,r11,-2932
	ctx.r4.s64 = ctx.r11.s64 + -2932;
	// bl 0x8286a370
	ctx.lr = 0x82A5F51C;
	sub_8286A370(ctx, base);
	// stw r30,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r30.u32);
	// b 0x82a5f52c
	goto loc_82A5F52C;
loc_82A5F524:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r11.u32);
loc_82A5F52C:
	// li r3,48
	ctx.r3.s64 = 48;
	// bl 0x821b3510
	ctx.lr = 0x82A5F534;
	sub_821B3510(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// li r28,2
	ctx.r28.s64 = 2;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// beq cr6,0x82a5f564
	if (ctx.cr6.eq) goto loc_82A5F564;
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stfs f31,0(r30)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r30.u32 + 0, temp.u32);
	// addi r3,r30,8
	ctx.r3.s64 = ctx.r30.s64 + 8;
	// stw r28,4(r30)
	PPC_STORE_U32(ctx.r30.u32 + 4, ctx.r28.u32);
	// addi r4,r11,-2956
	ctx.r4.s64 = ctx.r11.s64 + -2956;
	// bl 0x8286a370
	ctx.lr = 0x82A5F55C;
	sub_8286A370(ctx, base);
	// stw r30,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r30.u32);
	// b 0x82a5f56c
	goto loc_82A5F56C;
loc_82A5F564:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r11.u32);
loc_82A5F56C:
	// li r3,48
	ctx.r3.s64 = 48;
	// bl 0x821b3510
	ctx.lr = 0x82A5F574;
	sub_821B3510(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// li r27,10
	ctx.r27.s64 = 10;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// beq cr6,0x82a5f5a4
	if (ctx.cr6.eq) goto loc_82A5F5A4;
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stfs f31,0(r30)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r30.u32 + 0, temp.u32);
	// addi r3,r30,8
	ctx.r3.s64 = ctx.r30.s64 + 8;
	// stw r27,4(r30)
	PPC_STORE_U32(ctx.r30.u32 + 4, ctx.r27.u32);
	// addi r4,r11,-2972
	ctx.r4.s64 = ctx.r11.s64 + -2972;
	// bl 0x8286a370
	ctx.lr = 0x82A5F59C;
	sub_8286A370(ctx, base);
	// stw r30,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r30.u32);
	// b 0x82a5f5ac
	goto loc_82A5F5AC;
loc_82A5F5A4:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r11.u32);
loc_82A5F5AC:
	// li r3,48
	ctx.r3.s64 = 48;
	// bl 0x821b3510
	ctx.lr = 0x82A5F5B4;
	sub_821B3510(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// beq cr6,0x82a5f5e0
	if (ctx.cr6.eq) goto loc_82A5F5E0;
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stfs f31,0(r30)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r30.u32 + 0, temp.u32);
	// addi r3,r30,8
	ctx.r3.s64 = ctx.r30.s64 + 8;
	// stw r29,4(r30)
	PPC_STORE_U32(ctx.r30.u32 + 4, ctx.r29.u32);
	// addi r4,r11,-3000
	ctx.r4.s64 = ctx.r11.s64 + -3000;
	// bl 0x8286a370
	ctx.lr = 0x82A5F5D8;
	sub_8286A370(ctx, base);
	// stw r30,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r30.u32);
	// b 0x82a5f5e8
	goto loc_82A5F5E8;
loc_82A5F5E0:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r11.u32);
loc_82A5F5E8:
	// li r3,48
	ctx.r3.s64 = 48;
	// bl 0x821b3510
	ctx.lr = 0x82A5F5F0;
	sub_821B3510(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// beq cr6,0x82a5f61c
	if (ctx.cr6.eq) goto loc_82A5F61C;
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stfs f31,0(r30)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r30.u32 + 0, temp.u32);
	// addi r3,r30,8
	ctx.r3.s64 = ctx.r30.s64 + 8;
	// stw r29,4(r30)
	PPC_STORE_U32(ctx.r30.u32 + 4, ctx.r29.u32);
	// addi r4,r11,-3028
	ctx.r4.s64 = ctx.r11.s64 + -3028;
	// bl 0x8286a370
	ctx.lr = 0x82A5F614;
	sub_8286A370(ctx, base);
	// stw r30,20(r31)
	PPC_STORE_U32(ctx.r31.u32 + 20, ctx.r30.u32);
	// b 0x82a5f624
	goto loc_82A5F624;
loc_82A5F61C:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,20(r31)
	PPC_STORE_U32(ctx.r31.u32 + 20, ctx.r11.u32);
loc_82A5F624:
	// li r3,48
	ctx.r3.s64 = 48;
	// bl 0x821b3510
	ctx.lr = 0x82A5F62C;
	sub_821B3510(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// beq cr6,0x82a5f65c
	if (ctx.cr6.eq) goto loc_82A5F65C;
	// li r10,3
	ctx.r10.s64 = 3;
	// stfs f31,0(r30)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r30.u32 + 0, temp.u32);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r3,r30,8
	ctx.r3.s64 = ctx.r30.s64 + 8;
	// addi r4,r11,-3060
	ctx.r4.s64 = ctx.r11.s64 + -3060;
	// stw r10,4(r30)
	PPC_STORE_U32(ctx.r30.u32 + 4, ctx.r10.u32);
	// bl 0x8286a370
	ctx.lr = 0x82A5F654;
	sub_8286A370(ctx, base);
	// stw r30,24(r31)
	PPC_STORE_U32(ctx.r31.u32 + 24, ctx.r30.u32);
	// b 0x82a5f664
	goto loc_82A5F664;
loc_82A5F65C:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,24(r31)
	PPC_STORE_U32(ctx.r31.u32 + 24, ctx.r11.u32);
loc_82A5F664:
	// li r3,48
	ctx.r3.s64 = 48;
	// bl 0x821b3510
	ctx.lr = 0x82A5F66C;
	sub_821B3510(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// beq cr6,0x82a5f69c
	if (ctx.cr6.eq) goto loc_82A5F69C;
	// li r10,60
	ctx.r10.s64 = 60;
	// stfs f31,0(r30)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r30.u32 + 0, temp.u32);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r3,r30,8
	ctx.r3.s64 = ctx.r30.s64 + 8;
	// addi r4,r11,-3080
	ctx.r4.s64 = ctx.r11.s64 + -3080;
	// stw r10,4(r30)
	PPC_STORE_U32(ctx.r30.u32 + 4, ctx.r10.u32);
	// bl 0x8286a370
	ctx.lr = 0x82A5F694;
	sub_8286A370(ctx, base);
	// stw r30,28(r31)
	PPC_STORE_U32(ctx.r31.u32 + 28, ctx.r30.u32);
	// b 0x82a5f6a4
	goto loc_82A5F6A4;
loc_82A5F69C:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,28(r31)
	PPC_STORE_U32(ctx.r31.u32 + 28, ctx.r11.u32);
loc_82A5F6A4:
	// li r3,48
	ctx.r3.s64 = 48;
	// bl 0x821b3510
	ctx.lr = 0x82A5F6AC;
	sub_821B3510(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// beq cr6,0x82a5f6d8
	if (ctx.cr6.eq) goto loc_82A5F6D8;
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stfs f31,0(r30)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r30.u32 + 0, temp.u32);
	// addi r3,r30,8
	ctx.r3.s64 = ctx.r30.s64 + 8;
	// stw r29,4(r30)
	PPC_STORE_U32(ctx.r30.u32 + 4, ctx.r29.u32);
	// addi r4,r11,-3096
	ctx.r4.s64 = ctx.r11.s64 + -3096;
	// bl 0x8286a370
	ctx.lr = 0x82A5F6D0;
	sub_8286A370(ctx, base);
	// stw r30,32(r31)
	PPC_STORE_U32(ctx.r31.u32 + 32, ctx.r30.u32);
	// b 0x82a5f6e0
	goto loc_82A5F6E0;
loc_82A5F6D8:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,32(r31)
	PPC_STORE_U32(ctx.r31.u32 + 32, ctx.r11.u32);
loc_82A5F6E0:
	// li r3,48
	ctx.r3.s64 = 48;
	// bl 0x821b3510
	ctx.lr = 0x82A5F6E8;
	sub_821B3510(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// beq cr6,0x82a5f714
	if (ctx.cr6.eq) goto loc_82A5F714;
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stfs f31,0(r30)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r30.u32 + 0, temp.u32);
	// addi r3,r30,8
	ctx.r3.s64 = ctx.r30.s64 + 8;
	// stw r28,4(r30)
	PPC_STORE_U32(ctx.r30.u32 + 4, ctx.r28.u32);
	// addi r4,r11,-3116
	ctx.r4.s64 = ctx.r11.s64 + -3116;
	// bl 0x8286a370
	ctx.lr = 0x82A5F70C;
	sub_8286A370(ctx, base);
	// stw r30,36(r31)
	PPC_STORE_U32(ctx.r31.u32 + 36, ctx.r30.u32);
	// b 0x82a5f71c
	goto loc_82A5F71C;
loc_82A5F714:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,36(r31)
	PPC_STORE_U32(ctx.r31.u32 + 36, ctx.r11.u32);
loc_82A5F71C:
	// li r3,48
	ctx.r3.s64 = 48;
	// bl 0x821b3510
	ctx.lr = 0x82A5F724;
	sub_821B3510(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// beq cr6,0x82a5f754
	if (ctx.cr6.eq) goto loc_82A5F754;
	// li r10,30
	ctx.r10.s64 = 30;
	// stfs f31,0(r30)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r30.u32 + 0, temp.u32);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r3,r30,8
	ctx.r3.s64 = ctx.r30.s64 + 8;
	// addi r4,r11,-3132
	ctx.r4.s64 = ctx.r11.s64 + -3132;
	// stw r10,4(r30)
	PPC_STORE_U32(ctx.r30.u32 + 4, ctx.r10.u32);
	// bl 0x8286a370
	ctx.lr = 0x82A5F74C;
	sub_8286A370(ctx, base);
	// stw r30,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r30.u32);
	// b 0x82a5f75c
	goto loc_82A5F75C;
loc_82A5F754:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
loc_82A5F75C:
	// li r3,48
	ctx.r3.s64 = 48;
	// bl 0x821b3510
	ctx.lr = 0x82A5F764;
	sub_821B3510(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// beq cr6,0x82a5f798
	if (ctx.cr6.eq) goto loc_82A5F798;
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stfs f31,0(r30)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r30.u32 + 0, temp.u32);
	// addi r3,r30,8
	ctx.r3.s64 = ctx.r30.s64 + 8;
	// stw r27,4(r30)
	PPC_STORE_U32(ctx.r30.u32 + 4, ctx.r27.u32);
	// addi r4,r11,-3156
	ctx.r4.s64 = ctx.r11.s64 + -3156;
	// bl 0x8286a370
	ctx.lr = 0x82A5F788;
	sub_8286A370(ctx, base);
	// stw r30,44(r31)
	PPC_STORE_U32(ctx.r31.u32 + 44, ctx.r30.u32);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lfd f31,-56(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -56);
	// b 0x829ff814
	__restgprlr_27(ctx, base);
	return;
loc_82A5F798:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,44(r31)
	PPC_STORE_U32(ctx.r31.u32 + 44, ctx.r11.u32);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lfd f31,-56(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -56);
	// b 0x829ff814
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F7B0);
PPC_WEAK_FUNC(sub_82A5F7B0) { __imp__sub_82A5F7B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F7B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,1384
	ctx.r3.s64 = ctx.r11.s64 + 1384;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F7C0);
PPC_WEAK_FUNC(sub_82A5F7C0) { __imp__sub_82A5F7C0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F7C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-2808
	ctx.r5.s64 = ctx.r11.s64 + -2808;
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,21440
	ctx.r3.s64 = ctx.r11.s64 + 21440;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F7E0);
PPC_WEAK_FUNC(sub_82A5F7E0) { __imp__sub_82A5F7E0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F7E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// lis r9,-32255
	ctx.r9.s64 = -2113863680;
	// addi r11,r11,23732
	ctx.r11.s64 = ctx.r11.s64 + 23732;
	// addi r8,r9,-2596
	ctx.r8.s64 = ctx.r9.s64 + -2596;
	// li r10,31
	ctx.r10.s64 = 31;
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// li r9,0
	ctx.r9.s64 = 0;
loc_82A5F7FC:
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
	// stw r9,-4(r11)
	PPC_STORE_U32(ctx.r11.u32 + -4, ctx.r9.u32);
	// stw r8,-8(r11)
	PPC_STORE_U32(ctx.r11.u32 + -8, ctx.r8.u32);
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// addi r11,r11,16
	ctx.r11.s64 = ctx.r11.s64 + 16;
	// bge cr6,0x82a5f7fc
	if (!ctx.cr6.lt) goto loc_82A5F7FC;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,1416
	ctx.r3.s64 = ctx.r11.s64 + 1416;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F828);
PPC_WEAK_FUNC(sub_82A5F828) { __imp__sub_82A5F828(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F828) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// lis r10,-32255
	ctx.r10.s64 = -2113863680;
	// addi r3,r11,1552
	ctx.r3.s64 = ctx.r11.s64 + 1552;
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// addi r10,r10,-2588
	ctx.r10.s64 = ctx.r10.s64 + -2588;
	// addi r11,r11,24244
	ctx.r11.s64 = ctx.r11.s64 + 24244;
	// li r9,0
	ctx.r9.s64 = 0;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// stw r9,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r9.u32);
	// stw r9,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r9.u32);
	// stw r10,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r10.u32);
	// stw r9,24(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24, ctx.r9.u32);
	// stw r9,32(r11)
	PPC_STORE_U32(ctx.r11.u32 + 32, ctx.r9.u32);
	// stw r9,36(r11)
	PPC_STORE_U32(ctx.r11.u32 + 36, ctx.r9.u32);
	// stw r10,40(r11)
	PPC_STORE_U32(ctx.r11.u32 + 40, ctx.r10.u32);
	// stw r9,44(r11)
	PPC_STORE_U32(ctx.r11.u32 + 44, ctx.r9.u32);
	// stw r9,52(r11)
	PPC_STORE_U32(ctx.r11.u32 + 52, ctx.r9.u32);
	// stw r9,56(r11)
	PPC_STORE_U32(ctx.r11.u32 + 56, ctx.r9.u32);
	// stw r10,60(r11)
	PPC_STORE_U32(ctx.r11.u32 + 60, ctx.r10.u32);
	// stw r9,64(r11)
	PPC_STORE_U32(ctx.r11.u32 + 64, ctx.r9.u32);
	// stw r9,72(r11)
	PPC_STORE_U32(ctx.r11.u32 + 72, ctx.r9.u32);
	// stw r9,76(r11)
	PPC_STORE_U32(ctx.r11.u32 + 76, ctx.r9.u32);
	// stw r10,80(r11)
	PPC_STORE_U32(ctx.r11.u32 + 80, ctx.r10.u32);
	// stw r9,84(r11)
	PPC_STORE_U32(ctx.r11.u32 + 84, ctx.r9.u32);
	// stw r9,92(r11)
	PPC_STORE_U32(ctx.r11.u32 + 92, ctx.r9.u32);
	// stw r9,96(r11)
	PPC_STORE_U32(ctx.r11.u32 + 96, ctx.r9.u32);
	// stw r10,100(r11)
	PPC_STORE_U32(ctx.r11.u32 + 100, ctx.r10.u32);
	// stw r9,104(r11)
	PPC_STORE_U32(ctx.r11.u32 + 104, ctx.r9.u32);
	// stw r9,112(r11)
	PPC_STORE_U32(ctx.r11.u32 + 112, ctx.r9.u32);
	// stw r9,116(r11)
	PPC_STORE_U32(ctx.r11.u32 + 116, ctx.r9.u32);
	// stw r10,120(r11)
	PPC_STORE_U32(ctx.r11.u32 + 120, ctx.r10.u32);
	// stw r9,124(r11)
	PPC_STORE_U32(ctx.r11.u32 + 124, ctx.r9.u32);
	// stw r9,132(r11)
	PPC_STORE_U32(ctx.r11.u32 + 132, ctx.r9.u32);
	// stw r9,136(r11)
	PPC_STORE_U32(ctx.r11.u32 + 136, ctx.r9.u32);
	// stw r10,140(r11)
	PPC_STORE_U32(ctx.r11.u32 + 140, ctx.r10.u32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r10,144(r11)
	PPC_STORE_U32(ctx.r11.u32 + 144, ctx.r10.u32);
	// stw r10,152(r11)
	PPC_STORE_U32(ctx.r11.u32 + 152, ctx.r10.u32);
	// stw r10,156(r11)
	PPC_STORE_U32(ctx.r11.u32 + 156, ctx.r10.u32);
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A5F8D0);
PPC_WEAK_FUNC(sub_82A5F8D0) { __imp__sub_82A5F8D0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A5F8D0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,2612(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// stfs f0,-32(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -32, temp.u32);
	// stfs f0,-28(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -28, temp.u32);
	// addi r11,r11,-17312
	ctx.r11.s64 = ctx.r11.s64 + -17312;
	// stfs f0,-24(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -24, temp.u32);
	// addi r10,r1,-32
	ctx.r10.s64 = ctx.r1.s64 + -32;
	// stfs f0,-80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -80, temp.u32);
	// stfs f0,-76(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -76, temp.u32);
	// stfs f0,-72(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -72, temp.u32);
	// stfs f0,-48(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -48, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,160
	ctx.r10.s64 = 160;
	// stfs f0,-44(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -44, temp.u32);
	// stfs f0,-40(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -40, temp.u32);
	// stfs f0,-16(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -16, temp.u32);
	// stfs f0,-12(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -12, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-80
	ctx.r10.s64 = ctx.r1.s64 + -80;
	// stfs f0,-8(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -8, temp.u32);
	// stfs f0,-64(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -64, temp.u32);
	// stfs f0,-60(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -60, temp.u32);
	// stfs f0,-56(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -56, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,336
	ctx.r10.s64 = 336;
	// stfs f0,-192(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -192, temp.u32);
	// stfs f0,-188(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -188, temp.u32);
	// stfs f0,-184(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -184, temp.u32);
	// stfs f0,-96(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -96, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-48
	ctx.r10.s64 = ctx.r1.s64 + -48;
	// stfs f0,-92(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -92, temp.u32);
	// stfs f0,-88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -88, temp.u32);
	// stfs f0,-160(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -160, temp.u32);
	// stfs f0,-156(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -156, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,512
	ctx.r10.s64 = 512;
	// stfs f0,-152(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -152, temp.u32);
	// stfs f0,-240(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -240, temp.u32);
	// stfs f0,-236(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -236, temp.u32);
	// stfs f0,-232(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -232, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-16
	ctx.r10.s64 = ctx.r1.s64 + -16;
	// stfs f0,-128(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -128, temp.u32);
	// stfs f0,-124(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -124, temp.u32);
	// stfs f0,-120(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -120, temp.u32);
	// stfs f0,-224(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -224, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,688
	ctx.r10.s64 = 688;
	// stfs f0,-220(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -220, temp.u32);
	// stfs f0,-216(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -216, temp.u32);
	// stfs f0,-208(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -208, temp.u32);
	// stfs f0,-204(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -204, temp.u32);
	// stfs f0,-200(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -200, temp.u32);
	// stfs f0,-176(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -176, temp.u32);
	// stfs f0,-172(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -172, temp.u32);
	// stfs f0,-168(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -168, temp.u32);
	// stfs f0,-144(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -144, temp.u32);
	// stfs f0,-140(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -140, temp.u32);
	// stfs f0,-136(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -136, temp.u32);
	// stfs f0,-112(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -112, temp.u32);
	// stfs f0,-108(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -108, temp.u32);
	// stfs f0,-104(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -104, temp.u32);
	// stfs f0,-256(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -256, temp.u32);
	// stfs f0,-252(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -252, temp.u32);
	// stfs f0,-248(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -248, temp.u32);
	// stfs f0,-272(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -272, temp.u32);
	// stfs f0,-268(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -268, temp.u32);
	// stfs f0,-264(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -264, temp.u32);
	// stfs f0,-288(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -288, temp.u32);
	// stfs f0,-284(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -284, temp.u32);
	// stfs f0,-280(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -280, temp.u32);
	// stfs f0,-32(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -32, temp.u32);
	// stfs f0,-28(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -28, temp.u32);
	// stfs f0,-24(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -24, temp.u32);
	// stfs f0,-80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -80, temp.u32);
	// stfs f0,-76(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -76, temp.u32);
	// stfs f0,-72(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -72, temp.u32);
	// stfs f0,-48(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -48, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-64
	ctx.r10.s64 = ctx.r1.s64 + -64;
	// stfs f0,-44(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -44, temp.u32);
	// stfs f0,-40(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -40, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,864
	ctx.r10.s64 = 864;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-192
	ctx.r10.s64 = ctx.r1.s64 + -192;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,1040
	ctx.r10.s64 = 1040;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-96
	ctx.r10.s64 = ctx.r1.s64 + -96;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,1216
	ctx.r10.s64 = 1216;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-160
	ctx.r10.s64 = ctx.r1.s64 + -160;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,1392
	ctx.r10.s64 = 1392;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-240
	ctx.r10.s64 = ctx.r1.s64 + -240;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,1568
	ctx.r10.s64 = 1568;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-128
	ctx.r10.s64 = ctx.r1.s64 + -128;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,1744
	ctx.r10.s64 = 1744;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-224
	ctx.r10.s64 = ctx.r1.s64 + -224;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,1920
	ctx.r10.s64 = 1920;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-208
	ctx.r10.s64 = ctx.r1.s64 + -208;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,2096
	ctx.r10.s64 = 2096;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-176
	ctx.r10.s64 = ctx.r1.s64 + -176;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,2272
	ctx.r10.s64 = 2272;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-144
	ctx.r10.s64 = ctx.r1.s64 + -144;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,2448
	ctx.r10.s64 = 2448;
	// stfs f0,-144(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -144, temp.u32);
	// stfs f0,-140(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -140, temp.u32);
	// stfs f0,-136(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -136, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-112
	ctx.r10.s64 = ctx.r1.s64 + -112;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,2624
	ctx.r10.s64 = 2624;
	// stfs f0,-112(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -112, temp.u32);
	// stfs f0,-108(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -108, temp.u32);
	// stfs f0,-104(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -104, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-256
	ctx.r10.s64 = ctx.r1.s64 + -256;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,2800
	ctx.r10.s64 = 2800;
	// stfs f0,-256(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -256, temp.u32);
	// stfs f0,-252(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -252, temp.u32);
	// stfs f0,-248(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -248, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-272
	ctx.r10.s64 = ctx.r1.s64 + -272;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,2976
	ctx.r10.s64 = 2976;
	// stfs f0,-272(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -272, temp.u32);
	// stfs f0,-268(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -268, temp.u32);
	// stfs f0,-264(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -264, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-288
	ctx.r10.s64 = ctx.r1.s64 + -288;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,3152
	ctx.r10.s64 = 3152;
	// stfs f0,-288(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -288, temp.u32);
	// stfs f0,-284(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -284, temp.u32);
	// stfs f0,-280(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -280, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-32
	ctx.r10.s64 = ctx.r1.s64 + -32;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,3328
	ctx.r10.s64 = 3328;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-80
	ctx.r10.s64 = ctx.r1.s64 + -80;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,3504
	ctx.r10.s64 = 3504;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-48
	ctx.r10.s64 = ctx.r1.s64 + -48;
	// stfs f0,-176(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -176, temp.u32);
	// stfs f0,-172(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -172, temp.u32);
	// stfs f0,-168(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -168, temp.u32);
	// stfs f0,-208(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -208, temp.u32);
	// stfs f0,-204(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -204, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,3680
	ctx.r10.s64 = 3680;
	// stfs f0,-200(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -200, temp.u32);
	// stfs f0,-224(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -224, temp.u32);
	// stfs f0,-220(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -220, temp.u32);
	// stfs f0,-216(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -216, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-288
	ctx.r10.s64 = ctx.r1.s64 + -288;
	// stfs f0,-128(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -128, temp.u32);
	// stfs f0,-124(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -124, temp.u32);
	// stfs f0,-120(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -120, temp.u32);
	// stfs f0,-240(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -240, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,3856
	ctx.r10.s64 = 3856;
	// stfs f0,-236(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -236, temp.u32);
	// stfs f0,-232(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -232, temp.u32);
	// stfs f0,-160(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -160, temp.u32);
	// stfs f0,-156(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -156, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-272
	ctx.r10.s64 = ctx.r1.s64 + -272;
	// stfs f0,-152(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -152, temp.u32);
	// stfs f0,-96(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -96, temp.u32);
	// stfs f0,-92(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -92, temp.u32);
	// stfs f0,-88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -88, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,4032
	ctx.r10.s64 = 4032;
	// stfs f0,-192(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -192, temp.u32);
	// stfs f0,-188(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -188, temp.u32);
	// stfs f0,-184(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -184, temp.u32);
	// stfs f0,-64(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -64, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-256
	ctx.r10.s64 = ctx.r1.s64 + -256;
	// stfs f0,-60(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -60, temp.u32);
	// stfs f0,-56(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -56, temp.u32);
	// stfs f0,-16(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -16, temp.u32);
	// stfs f0,-12(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -12, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,4208
	ctx.r10.s64 = 4208;
	// stfs f0,-8(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -8, temp.u32);
	// stfs f0,-80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -80, temp.u32);
	// stfs f0,-76(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -76, temp.u32);
	// stfs f0,-72(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -72, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-112
	ctx.r10.s64 = ctx.r1.s64 + -112;
	// stfs f0,-32(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -32, temp.u32);
	// stfs f0,-28(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -28, temp.u32);
	// stfs f0,-24(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -24, temp.u32);
	// stfs f0,-48(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -48, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,4384
	ctx.r10.s64 = 4384;
	// stfs f0,-44(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -44, temp.u32);
	// stfs f0,-40(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -40, temp.u32);
	// stfs f0,-288(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -288, temp.u32);
	// stfs f0,-284(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -284, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-144
	ctx.r10.s64 = ctx.r1.s64 + -144;
	// stfs f0,-280(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -280, temp.u32);
	// stfs f0,-272(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -272, temp.u32);
	// stfs f0,-268(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -268, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,4560
	ctx.r10.s64 = 4560;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-176
	ctx.r10.s64 = ctx.r1.s64 + -176;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,4736
	ctx.r10.s64 = 4736;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-208
	ctx.r10.s64 = ctx.r1.s64 + -208;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,4912
	ctx.r10.s64 = 4912;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-224
	ctx.r10.s64 = ctx.r1.s64 + -224;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,5088
	ctx.r10.s64 = 5088;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-128
	ctx.r10.s64 = ctx.r1.s64 + -128;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,5264
	ctx.r10.s64 = 5264;
	// stfs f0,-264(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -264, temp.u32);
	// stfs f0,-256(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -256, temp.u32);
	// stfs f0,-252(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -252, temp.u32);
	// stfs f0,-248(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -248, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-240
	ctx.r10.s64 = ctx.r1.s64 + -240;
	// stfs f0,-112(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -112, temp.u32);
	// stfs f0,-108(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -108, temp.u32);
	// stfs f0,-104(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -104, temp.u32);
	// stfs f0,-144(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -144, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,5440
	ctx.r10.s64 = 5440;
	// stfs f0,-140(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -140, temp.u32);
	// stfs f0,-136(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -136, temp.u32);
	// stfs f0,-176(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -176, temp.u32);
	// stfs f0,-172(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -172, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-160
	ctx.r10.s64 = ctx.r1.s64 + -160;
	// stfs f0,-168(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -168, temp.u32);
	// stfs f0,-208(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -208, temp.u32);
	// stfs f0,-204(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -204, temp.u32);
	// stfs f0,-200(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -200, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,5616
	ctx.r10.s64 = 5616;
	// stfs f0,-224(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -224, temp.u32);
	// stfs f0,-220(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -220, temp.u32);
	// stfs f0,-216(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -216, temp.u32);
	// stfs f0,-128(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -128, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-96
	ctx.r10.s64 = ctx.r1.s64 + -96;
	// stfs f0,-124(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -124, temp.u32);
	// stfs f0,-120(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -120, temp.u32);
	// stfs f0,-240(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -240, temp.u32);
	// stfs f0,-236(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -236, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,5792
	ctx.r10.s64 = 5792;
	// stfs f0,-232(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -232, temp.u32);
	// stfs f0,-160(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -160, temp.u32);
	// stfs f0,-156(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -156, temp.u32);
	// stfs f0,-152(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -152, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-192
	ctx.r10.s64 = ctx.r1.s64 + -192;
	// stfs f0,-96(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -96, temp.u32);
	// stfs f0,-92(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -92, temp.u32);
	// stfs f0,-88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -88, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,5968
	ctx.r10.s64 = 5968;
	// stfs f0,-192(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -192, temp.u32);
	// stfs f0,-188(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -188, temp.u32);
	// stfs f0,-184(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -184, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-64
	ctx.r10.s64 = ctx.r1.s64 + -64;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,6144
	ctx.r10.s64 = 6144;
	// stfs f0,-64(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -64, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-16
	ctx.r10.s64 = ctx.r1.s64 + -16;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,6320
	ctx.r10.s64 = 6320;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-80
	ctx.r10.s64 = ctx.r1.s64 + -80;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,6496
	ctx.r10.s64 = 6496;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-32
	ctx.r10.s64 = ctx.r1.s64 + -32;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,6672
	ctx.r10.s64 = 6672;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-48
	ctx.r10.s64 = ctx.r1.s64 + -48;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,6848
	ctx.r10.s64 = 6848;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-288
	ctx.r10.s64 = ctx.r1.s64 + -288;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,7024
	ctx.r10.s64 = 7024;
	// stfs f0,-288(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -288, temp.u32);
	// stfs f0,-284(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -284, temp.u32);
	// stfs f0,-280(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -280, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-272
	ctx.r10.s64 = ctx.r1.s64 + -272;
	// stfs f0,-60(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -60, temp.u32);
	// stfs f0,-56(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -56, temp.u32);
	// stfs f0,-16(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -16, temp.u32);
	// stfs f0,-12(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -12, temp.u32);
	// stfs f0,-8(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -8, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,7200
	ctx.r10.s64 = 7200;
	// stfs f0,-48(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -48, temp.u32);
	// stfs f0,-44(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -44, temp.u32);
	// stfs f0,-40(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -40, temp.u32);
	// stfs f0,-80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -80, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-288
	ctx.r10.s64 = ctx.r1.s64 + -288;
	// stfs f0,-76(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -76, temp.u32);
	// stfs f0,-72(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -72, temp.u32);
	// stfs f0,-32(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -32, temp.u32);
	// stfs f0,-28(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -28, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,7376
	ctx.r10.s64 = 7376;
	// stfs f0,-24(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -24, temp.u32);
	// stfs f0,-272(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -272, temp.u32);
	// stfs f0,-268(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -268, temp.u32);
	// stfs f0,-264(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -264, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-256
	ctx.r10.s64 = ctx.r1.s64 + -256;
	// stfs f0,-288(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -288, temp.u32);
	// stfs f0,-284(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -284, temp.u32);
	// stfs f0,-280(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -280, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,7552
	ctx.r10.s64 = 7552;
	// stfs f0,-256(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -256, temp.u32);
	// stfs f0,-252(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -252, temp.u32);
	// stfs f0,-248(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -248, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-112
	ctx.r10.s64 = ctx.r1.s64 + -112;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,7728
	ctx.r10.s64 = 7728;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-144
	ctx.r10.s64 = ctx.r1.s64 + -144;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,7904
	ctx.r10.s64 = 7904;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-176
	ctx.r10.s64 = ctx.r1.s64 + -176;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,8080
	ctx.r10.s64 = 8080;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-208
	ctx.r10.s64 = ctx.r1.s64 + -208;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,8256
	ctx.r10.s64 = 8256;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-224
	ctx.r10.s64 = ctx.r1.s64 + -224;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,8432
	ctx.r10.s64 = 8432;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-128
	ctx.r10.s64 = ctx.r1.s64 + -128;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,8608
	ctx.r10.s64 = 8608;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-240
	ctx.r10.s64 = ctx.r1.s64 + -240;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,8784
	ctx.r10.s64 = 8784;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-160
	ctx.r10.s64 = ctx.r1.s64 + -160;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,8960
	ctx.r10.s64 = 8960;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-96
	ctx.r10.s64 = ctx.r1.s64 + -96;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,9136
	ctx.r10.s64 = 9136;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-192
	ctx.r10.s64 = ctx.r1.s64 + -192;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,9312
	ctx.r10.s64 = 9312;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-64
	ctx.r10.s64 = ctx.r1.s64 + -64;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,9488
	ctx.r10.s64 = 9488;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-16
	ctx.r10.s64 = ctx.r1.s64 + -16;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,9664
	ctx.r10.s64 = 9664;
	// stfs f0,-112(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -112, temp.u32);
	// stfs f0,-108(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -108, temp.u32);
	// stfs f0,-104(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -104, temp.u32);
	// stfs f0,-144(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -144, temp.u32);
	// stfs f0,-140(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -140, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-48
	ctx.r10.s64 = ctx.r1.s64 + -48;
	// stfs f0,-136(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -136, temp.u32);
	// stfs f0,-176(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -176, temp.u32);
	// stfs f0,-172(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -172, temp.u32);
	// stfs f0,-168(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -168, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,9840
	ctx.r10.s64 = 9840;
	// stfs f0,-208(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -208, temp.u32);
	// stfs f0,-204(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -204, temp.u32);
	// stfs f0,-200(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -200, temp.u32);
	// stfs f0,-224(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -224, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-80
	ctx.r10.s64 = ctx.r1.s64 + -80;
	// stfs f0,-220(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -220, temp.u32);
	// stfs f0,-216(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -216, temp.u32);
	// stfs f0,-128(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -128, temp.u32);
	// stfs f0,-124(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -124, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,10016
	ctx.r10.s64 = 10016;
	// stfs f0,-120(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -120, temp.u32);
	// stfs f0,-240(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -240, temp.u32);
	// stfs f0,-236(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -236, temp.u32);
	// stfs f0,-232(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -232, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-32
	ctx.r10.s64 = ctx.r1.s64 + -32;
	// stfs f0,-160(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -160, temp.u32);
	// stfs f0,-156(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -156, temp.u32);
	// stfs f0,-152(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -152, temp.u32);
	// stfs f0,-96(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -96, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,10192
	ctx.r10.s64 = 10192;
	// stfs f0,-92(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -92, temp.u32);
	// stfs f0,-88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -88, temp.u32);
	// stfs f0,-192(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -192, temp.u32);
	// stfs f0,-188(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -188, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-272
	ctx.r10.s64 = ctx.r1.s64 + -272;
	// stfs f0,-184(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -184, temp.u32);
	// stfs f0,-64(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -64, temp.u32);
	// stfs f0,-60(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -60, temp.u32);
	// stfs f0,-56(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -56, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,10368
	ctx.r10.s64 = 10368;
	// stfs f0,-272(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -272, temp.u32);
	// stfs f0,-268(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -268, temp.u32);
	// stfs f0,-264(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -264, temp.u32);
	// stfs f0,-16(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -16, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-288
	ctx.r10.s64 = ctx.r1.s64 + -288;
	// stfs f0,-12(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -12, temp.u32);
	// stfs f0,-8(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -8, temp.u32);
	// stfs f0,-48(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -48, temp.u32);
	// stfs f0,-44(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -44, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,10544
	ctx.r10.s64 = 10544;
	// stfs f0,-288(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -288, temp.u32);
	// stfs f0,-284(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -284, temp.u32);
	// stfs f0,-280(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -280, temp.u32);
	// stfs f0,-40(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -40, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-256
	ctx.r10.s64 = ctx.r1.s64 + -256;
	// stfs f0,-80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -80, temp.u32);
	// stfs f0,-76(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -76, temp.u32);
	// stfs f0,-72(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -72, temp.u32);
	// stfs f0,-32(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -32, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,10720
	ctx.r10.s64 = 10720;
	// stfs f0,-256(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -256, temp.u32);
	// stfs f0,-252(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -252, temp.u32);
	// stfs f0,-248(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -248, temp.u32);
	// stfs f0,-28(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -28, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-288
	ctx.r10.s64 = ctx.r1.s64 + -288;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,10896
	ctx.r10.s64 = 10896;
	// stfs f0,-24(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -24, temp.u32);
	// stfs f0,-288(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -288, temp.u32);
	// stfs f0,-284(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -284, temp.u32);
	// stfs f0,-280(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -280, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-272
	ctx.r10.s64 = ctx.r1.s64 + -272;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,11072
	ctx.r10.s64 = 11072;
	// stfs f0,-272(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -272, temp.u32);
	// stfs f0,-268(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -268, temp.u32);
	// stfs f0,-264(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -264, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-256
	ctx.r10.s64 = ctx.r1.s64 + -256;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,11248
	ctx.r10.s64 = 11248;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-112
	ctx.r10.s64 = ctx.r1.s64 + -112;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,11424
	ctx.r10.s64 = 11424;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-144
	ctx.r10.s64 = ctx.r1.s64 + -144;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,11600
	ctx.r10.s64 = 11600;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-176
	ctx.r10.s64 = ctx.r1.s64 + -176;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,11776
	ctx.r10.s64 = 11776;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-208
	ctx.r10.s64 = ctx.r1.s64 + -208;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,11952
	ctx.r10.s64 = 11952;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-224
	ctx.r10.s64 = ctx.r1.s64 + -224;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,12128
	ctx.r10.s64 = 12128;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-128
	ctx.r10.s64 = ctx.r1.s64 + -128;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,12304
	ctx.r10.s64 = 12304;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-240
	ctx.r10.s64 = ctx.r1.s64 + -240;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,12480
	ctx.r10.s64 = 12480;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-160
	ctx.r10.s64 = ctx.r1.s64 + -160;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,12656
	ctx.r10.s64 = 12656;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-96
	ctx.r10.s64 = ctx.r1.s64 + -96;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,12832
	ctx.r10.s64 = 12832;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-192
	ctx.r10.s64 = ctx.r1.s64 + -192;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,13008
	ctx.r10.s64 = 13008;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-64
	ctx.r10.s64 = ctx.r1.s64 + -64;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,13184
	ctx.r10.s64 = 13184;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-16
	ctx.r10.s64 = ctx.r1.s64 + -16;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,13360
	ctx.r10.s64 = 13360;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-48
	ctx.r10.s64 = ctx.r1.s64 + -48;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,13536
	ctx.r10.s64 = 13536;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-80
	ctx.r10.s64 = ctx.r1.s64 + -80;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,13712
	ctx.r10.s64 = 13712;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-32
	ctx.r10.s64 = ctx.r1.s64 + -32;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,13888
	ctx.r10.s64 = 13888;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-288
	ctx.r10.s64 = ctx.r1.s64 + -288;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,14064
	ctx.r10.s64 = 14064;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-272
	ctx.r10.s64 = ctx.r1.s64 + -272;
	// stfs f0,-288(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -288, temp.u32);
	// stfs f0,-284(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -284, temp.u32);
	// stfs f0,-280(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -280, temp.u32);
	// stfs f0,-256(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -256, temp.u32);
	// stfs f0,-252(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -252, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,14240
	ctx.r10.s64 = 14240;
	// stfs f0,-272(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -272, temp.u32);
	// stfs f0,-268(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -268, temp.u32);
	// stfs f0,-264(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -264, temp.u32);
	// stfs f0,-248(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -248, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-288
	ctx.r10.s64 = ctx.r1.s64 + -288;
	// stfs f0,-112(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -112, temp.u32);
	// stfs f0,-108(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -108, temp.u32);
	// stfs f0,-104(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -104, temp.u32);
	// stfs f0,-144(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -144, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,14416
	ctx.r10.s64 = 14416;
	// stfs f0,-140(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -140, temp.u32);
	// stfs f0,-136(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -136, temp.u32);
	// stfs f0,-176(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -176, temp.u32);
	// stfs f0,-172(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -172, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-272
	ctx.r10.s64 = ctx.r1.s64 + -272;
	// stfs f0,-168(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -168, temp.u32);
	// stfs f0,-208(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -208, temp.u32);
	// stfs f0,-204(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -204, temp.u32);
	// stfs f0,-200(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -200, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,14592
	ctx.r10.s64 = 14592;
	// stfs f0,-224(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -224, temp.u32);
	// stfs f0,-220(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -220, temp.u32);
	// stfs f0,-216(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -216, temp.u32);
	// stfs f0,-128(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -128, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-256
	ctx.r10.s64 = ctx.r1.s64 + -256;
	// stfs f0,-124(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -124, temp.u32);
	// stfs f0,-120(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -120, temp.u32);
	// stfs f0,-240(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -240, temp.u32);
	// stfs f0,-236(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -236, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,14768
	ctx.r10.s64 = 14768;
	// stfs f0,-232(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -232, temp.u32);
	// stfs f0,-160(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -160, temp.u32);
	// stfs f0,-156(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -156, temp.u32);
	// stfs f0,-152(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -152, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-112
	ctx.r10.s64 = ctx.r1.s64 + -112;
	// stfs f0,-96(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -96, temp.u32);
	// stfs f0,-92(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -92, temp.u32);
	// stfs f0,-88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -88, temp.u32);
	// stfs f0,-192(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -192, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,14944
	ctx.r10.s64 = 14944;
	// stfs f0,-188(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -188, temp.u32);
	// stfs f0,-184(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -184, temp.u32);
	// stfs f0,-64(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -64, temp.u32);
	// stfs f0,-60(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -60, temp.u32);
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-144
	ctx.r10.s64 = ctx.r1.s64 + -144;
	// stfs f0,-56(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -56, temp.u32);
	// stfs f0,-16(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -16, temp.u32);
	// stfs f0,-12(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -12, temp.u32);
	// stfs f0,-8(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -8, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,15120
	ctx.r10.s64 = 15120;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-176
	ctx.r10.s64 = ctx.r1.s64 + -176;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,15296
	ctx.r10.s64 = 15296;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-208
	ctx.r10.s64 = ctx.r1.s64 + -208;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,15472
	ctx.r10.s64 = 15472;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-224
	ctx.r10.s64 = ctx.r1.s64 + -224;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,15648
	ctx.r10.s64 = 15648;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-128
	ctx.r10.s64 = ctx.r1.s64 + -128;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,15824
	ctx.r10.s64 = 15824;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-240
	ctx.r10.s64 = ctx.r1.s64 + -240;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,16000
	ctx.r10.s64 = 16000;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-160
	ctx.r10.s64 = ctx.r1.s64 + -160;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,16176
	ctx.r10.s64 = 16176;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-96
	ctx.r10.s64 = ctx.r1.s64 + -96;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,16352
	ctx.r10.s64 = 16352;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-192
	ctx.r10.s64 = ctx.r1.s64 + -192;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,16528
	ctx.r10.s64 = 16528;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-64
	ctx.r10.s64 = ctx.r1.s64 + -64;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,16704
	ctx.r10.s64 = 16704;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,-16
	ctx.r10.s64 = ctx.r1.s64 + -16;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,16880
	ctx.r10.s64 = 16880;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60368);
PPC_WEAK_FUNC(sub_82A60368) { __imp__sub_82A60368(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60368) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,1688
	ctx.r3.s64 = ctx.r11.s64 + 1688;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60378);
PPC_WEAK_FUNC(sub_82A60378) { __imp__sub_82A60378(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60378) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32043
	ctx.r11.s64 = -2099970048;
	// addi r3,r11,25216
	ctx.r3.s64 = ctx.r11.s64 + 25216;
	// bl 0x8231fbd8
	ctx.lr = 0x82A60390;
	sub_8231FBD8(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,1696
	ctx.r3.s64 = ctx.r11.s64 + 1696;
	// bl 0x829ffa48
	ctx.lr = 0x82A6039C;
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

PPC_FUNC_IMPL(__imp__sub_82A603B0);
PPC_WEAK_FUNC(sub_82A603B0) { __imp__sub_82A603B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A603B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32034
	ctx.r11.s64 = -2099380224;
	// li r6,11
	ctx.r6.s64 = 11;
	// addi r11,r11,20832
	ctx.r11.s64 = ctx.r11.s64 + 20832;
	// li r7,-1
	ctx.r7.s64 = -1;
	// addi r9,r11,16
	ctx.r9.s64 = ctx.r11.s64 + 16;
	// li r8,0
	ctx.r8.s64 = 0;
loc_82A603C8:
	// mr r11,r8
	ctx.r11.u64 = ctx.r8.u64;
	// stb r7,-16(r9)
	PPC_STORE_U8(ctx.r9.u32 + -16, ctx.r7.u8);
	// addi r10,r9,-12
	ctx.r10.s64 = ctx.r9.s64 + -12;
loc_82A603D4:
	// stw r8,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r8.u32);
	// addi r10,r10,4
	ctx.r10.s64 = ctx.r10.s64 + 4;
	// stbx r8,r9,r11
	PPC_STORE_U8(ctx.r9.u32 + ctx.r11.u32, ctx.r8.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// cmpwi cr6,r11,3
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 3, ctx.xer);
	// blt cr6,0x82a603d4
	if (ctx.cr6.lt) goto loc_82A603D4;
	// addi r6,r6,-1
	ctx.r6.s64 = ctx.r6.s64 + -1;
	// addi r9,r9,20
	ctx.r9.s64 = ctx.r9.s64 + 20;
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// bge cr6,0x82a603c8
	if (!ctx.cr6.lt) goto loc_82A603C8;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,1712
	ctx.r3.s64 = ctx.r11.s64 + 1712;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60408);
PPC_WEAK_FUNC(sub_82A60408) { __imp__sub_82A60408(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60408) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,1720
	ctx.r3.s64 = ctx.r11.s64 + 1720;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60418);
PPC_WEAK_FUNC(sub_82A60418) { __imp__sub_82A60418(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60418) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r4,r11,7456
	ctx.r4.s64 = ctx.r11.s64 + 7456;
	// lis r11,-32034
	ctx.r11.s64 = -2099380224;
	// addi r3,r11,23180
	ctx.r3.s64 = ctx.r11.s64 + 23180;
	// b 0x822d24c0
	sub_822D24C0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60430);
PPC_WEAK_FUNC(sub_82A60430) { __imp__sub_82A60430(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60430) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,7608
	ctx.r5.s64 = ctx.r11.s64 + 7608;
	// lis r11,-32033
	ctx.r11.s64 = -2099314688;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-8228
	ctx.r3.s64 = ctx.r11.s64 + -8228;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60450);
PPC_WEAK_FUNC(sub_82A60450) { __imp__sub_82A60450(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60450) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32033
	ctx.r11.s64 = -2099314688;
	// addi r3,r11,7008
	ctx.r3.s64 = ctx.r11.s64 + 7008;
	// bl 0x828be650
	ctx.lr = 0x82A60468;
	sub_828BE650(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,1736
	ctx.r3.s64 = ctx.r11.s64 + 1736;
	// bl 0x829ffa48
	ctx.lr = 0x82A60474;
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

PPC_FUNC_IMPL(__imp__sub_82A60488);
PPC_WEAK_FUNC(sub_82A60488) { __imp__sub_82A60488(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60488) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,1752
	ctx.r3.s64 = ctx.r11.s64 + 1752;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60498);
PPC_WEAK_FUNC(sub_82A60498) { __imp__sub_82A60498(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60498) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32033
	ctx.r11.s64 = -2099314688;
	// li r9,-1
	ctx.r9.s64 = -1;
	// addi r10,r11,8032
	ctx.r10.s64 = ctx.r11.s64 + 8032;
	// li r11,148
	ctx.r11.s64 = 148;
	// addi r10,r10,36
	ctx.r10.s64 = ctx.r10.s64 + 36;
loc_82A604AC:
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// addi r10,r10,44
	ctx.r10.s64 = ctx.r10.s64 + 44;
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// bge cr6,0x82a604ac
	if (!ctx.cr6.lt) goto loc_82A604AC;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A604C8);
PPC_WEAK_FUNC(sub_82A604C8) { __imp__sub_82A604C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A604C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32033
	ctx.r11.s64 = -2099314688;
	// addi r3,r11,14768
	ctx.r3.s64 = ctx.r11.s64 + 14768;
	// b 0x82341b00
	sub_82341B00(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A604D8);
PPC_WEAK_FUNC(sub_82A604D8) { __imp__sub_82A604D8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A604D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32012
	ctx.r11.s64 = -2097938432;
	// addi r3,r11,20016
	ctx.r3.s64 = ctx.r11.s64 + 20016;
	// b 0x823476d8
	sub_823476D8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A604E8);
PPC_WEAK_FUNC(sub_82A604E8) { __imp__sub_82A604E8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A604E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32010
	ctx.r11.s64 = -2097807360;
	// addi r3,r11,-25968
	ctx.r3.s64 = ctx.r11.s64 + -25968;
	// b 0x82347b90
	sub_82347B90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A604F8);
PPC_WEAK_FUNC(sub_82A604F8) { __imp__sub_82A604F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A604F8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32010
	ctx.r11.s64 = -2097807360;
	// addi r3,r11,-3120
	ctx.r3.s64 = ctx.r11.s64 + -3120;
	// bl 0x8234ca18
	ctx.lr = 0x82A60510;
	sub_8234CA18(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,1816
	ctx.r3.s64 = ctx.r11.s64 + 1816;
	// bl 0x829ffa48
	ctx.lr = 0x82A6051C;
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

PPC_FUNC_IMPL(__imp__sub_82A60530);
PPC_WEAK_FUNC(sub_82A60530) { __imp__sub_82A60530(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60530) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,11212
	ctx.r3.s64 = ctx.r11.s64 + 11212;
	// bl 0x8284e060
	ctx.lr = 0x82A6054C;
	sub_8284E060(ctx, base);
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// stw r3,-26320(r11)
	PPC_STORE_U32(ctx.r11.u32 + -26320, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60568);
PPC_WEAK_FUNC(sub_82A60568) { __imp__sub_82A60568(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60568) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	ctx.lr = 0x82A60588;
	sub_828DCA28(ctx, base);
	// addi r3,r31,16
	ctx.r3.s64 = ctx.r31.s64 + 16;
	// bl 0x8234ef90
	ctx.lr = 0x82A60590;
	sub_8234EF90(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,1832
	ctx.r3.s64 = ctx.r11.s64 + 1832;
	// bl 0x829ffa48
	ctx.lr = 0x82A6059C;
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

PPC_FUNC_IMPL(__imp__sub_82A605B0);
PPC_WEAK_FUNC(sub_82A605B0) { __imp__sub_82A605B0(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A605B0) {
	PPC_FUNC_PROLOGUE();
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
	// li r6,16
	ctx.r6.s64 = 16;
	// addi r5,r11,12024
	ctx.r5.s64 = ctx.r11.s64 + 12024;
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// li r4,16
	ctx.r4.s64 = 16;
	// addi r3,r11,-8828
	ctx.r3.s64 = ctx.r11.s64 + -8828;
	// bl 0x82507368
	ctx.lr = 0x82A605D8;
	sub_82507368(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,1912
	ctx.r3.s64 = ctx.r11.s64 + 1912;
	// bl 0x829ffa48
	ctx.lr = 0x82A605E4;
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

PPC_FUNC_IMPL(__imp__sub_82A605F8);
PPC_WEAK_FUNC(sub_82A605F8) { __imp__sub_82A605F8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A605F8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-8688
	ctx.r3.s64 = ctx.r11.s64 + -8688;
	// bl 0x828ca888
	ctx.lr = 0x82A60610;
	sub_828CA888(ctx, base);
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,2032
	ctx.r3.s64 = ctx.r11.s64 + 2032;
	// bl 0x829ffa48
	ctx.lr = 0x82A6061C;
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

PPC_FUNC_IMPL(__imp__sub_82A60630);
PPC_WEAK_FUNC(sub_82A60630) { __imp__sub_82A60630(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60630) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,12712
	ctx.r5.s64 = ctx.r11.s64 + 12712;
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-8524
	ctx.r3.s64 = ctx.r11.s64 + -8524;
	// b 0x8285dbb8
	sub_8285DBB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60650);
PPC_WEAK_FUNC(sub_82A60650) { __imp__sub_82A60650(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60650) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// lis r10,-31973
	ctx.r10.s64 = -2095382528;
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r7,r10,-18832
	ctx.r7.s64 = ctx.r10.s64 + -18832;
	// lis r6,-32256
	ctx.r6.s64 = -2113929216;
	// addi r11,r11,-8368
	ctx.r11.s64 = ctx.r11.s64 + -8368;
	// lis r8,0
	ctx.r8.s64 = 0;
	// li r9,31
	ctx.r9.s64 = 31;
	// lvx128 v0,r0,r7
	ea = (ctx.r7.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r11,32
	ctx.r11.s64 = ctx.r11.s64 + 32;
	// lfs f0,2612(r6)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + 2612);
	ctx.f0.f64 = double(temp.f32);
	// li r10,0
	ctx.r10.s64 = 0;
	// li r4,-16
	ctx.r4.s64 = -16;
	// li r5,16
	ctx.r5.s64 = 16;
	// ori r8,r8,65535
	ctx.r8.u64 = ctx.r8.u64 | 65535;
loc_82A60688:
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// stfs f0,32(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 32, temp.u32);
	// stfs f0,36(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 36, temp.u32);
	// stw r10,-32(r11)
	PPC_STORE_U32(ctx.r11.u32 + -32, ctx.r10.u32);
	// stvx128 v0,r11,r4
	ea = (ctx.r11.u32 + ctx.r4.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// sth r8,44(r11)
	PPC_STORE_U16(ctx.r11.u32 + 44, ctx.r8.u16);
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// sth r10,46(r11)
	PPC_STORE_U16(ctx.r11.u32 + 46, ctx.r10.u16);
	// stvx128 v0,r11,r5
	ea = (ctx.r11.u32 + ctx.r5.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stb r10,48(r11)
	PPC_STORE_U8(ctx.r11.u32 + 48, ctx.r10.u8);
	// sth r10,50(r11)
	PPC_STORE_U16(ctx.r11.u32 + 50, ctx.r10.u16);
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// stw r10,40(r11)
	PPC_STORE_U32(ctx.r11.u32 + 40, ctx.r10.u32);
	// addi r11,r11,96
	ctx.r11.s64 = ctx.r11.s64 + 96;
	// bge cr6,0x82a60688
	if (!ctx.cr6.lt) goto loc_82A60688;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A606C8);
PPC_WEAK_FUNC(sub_82A606C8) { __imp__sub_82A606C8(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A606C8) {
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
	// li r31,31
	ctx.r31.s64 = 31;
	// addi r11,r11,-5248
	ctx.r11.s64 = ctx.r11.s64 + -5248;
	// addi r30,r11,48
	ctx.r30.s64 = ctx.r11.s64 + 48;
loc_82A606EC:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82915508
	ctx.lr = 0x82A606F4;
	sub_82915508(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,208
	ctx.r30.s64 = ctx.r30.s64 + 208;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x82a606ec
	if (!ctx.cr6.lt) goto loc_82A606EC;
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,2048
	ctx.r3.s64 = ctx.r11.s64 + 2048;
	// bl 0x829ffa48
	ctx.lr = 0x82A60710;
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

PPC_FUNC_IMPL(__imp__sub_82A60728);
PPC_WEAK_FUNC(sub_82A60728) { __imp__sub_82A60728(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60728) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// li r9,-1
	ctx.r9.s64 = -1;
	// addi r10,r11,2144
	ctx.r10.s64 = ctx.r11.s64 + 2144;
	// li r11,63
	ctx.r11.s64 = 63;
	// addi r10,r10,64
	ctx.r10.s64 = ctx.r10.s64 + 64;
loc_82A6073C:
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// addi r10,r10,80
	ctx.r10.s64 = ctx.r10.s64 + 80;
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// bge cr6,0x82a6073c
	if (!ctx.cr6.lt) goto loc_82A6073C;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82A60758);
PPC_WEAK_FUNC(sub_82A60758) { __imp__sub_82A60758(ctx, base); }
PPC_FUNC_IMPL(__imp__sub_82A60758) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32089
	ctx.r11.s64 = -2102984704;
	// addi r3,r11,2136
	ctx.r3.s64 = ctx.r11.s64 + 2136;
	// b 0x829ffa48
	sub_829FFA48(ctx, base);
	return;
}

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

