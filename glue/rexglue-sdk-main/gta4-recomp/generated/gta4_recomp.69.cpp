#include "gta4_init.h"

PPC_FUNC_IMPL(__imp__sub_829E9188);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9188) { __imp__sub_829E9188(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9188) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23384
	ctx.r3.s64 = ctx.r11.s64 + -23384;
	// bl 0x827df490
	ctx.lr = 0x829E91A4;
	sub_827DF490(ctx, base);
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// stw r3,-28016(r11)
	PPC_STORE_U32(ctx.r11.u32 + -28016, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E91C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829E91C0) { __imp__sub_829E91C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E91C0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23360
	ctx.r3.s64 = ctx.r11.s64 + -23360;
	// bl 0x827df490
	ctx.lr = 0x829E91DC;
	sub_827DF490(ctx, base);
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// stw r3,-28012(r11)
	PPC_STORE_U32(ctx.r11.u32 + -28012, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E91F8);  /* forward decl */
PPC_WEAK_FUNC(sub_829E91F8) { __imp__sub_829E91F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E91F8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23336
	ctx.r3.s64 = ctx.r11.s64 + -23336;
	// bl 0x827df490
	ctx.lr = 0x829E9214;
	sub_827DF490(ctx, base);
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// stw r3,-28008(r11)
	PPC_STORE_U32(ctx.r11.u32 + -28008, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9230);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9230) { __imp__sub_829E9230(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9230) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23312
	ctx.r3.s64 = ctx.r11.s64 + -23312;
	// bl 0x827df490
	ctx.lr = 0x829E924C;
	sub_827DF490(ctx, base);
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// stw r3,-28004(r11)
	PPC_STORE_U32(ctx.r11.u32 + -28004, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9268);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9268) { __imp__sub_829E9268(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9268) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23288
	ctx.r3.s64 = ctx.r11.s64 + -23288;
	// bl 0x827df490
	ctx.lr = 0x829E9284;
	sub_827DF490(ctx, base);
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// stw r3,-28000(r11)
	PPC_STORE_U32(ctx.r11.u32 + -28000, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E92A0);  /* forward decl */
PPC_WEAK_FUNC(sub_829E92A0) { __imp__sub_829E92A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E92A0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23264
	ctx.r3.s64 = ctx.r11.s64 + -23264;
	// bl 0x827df490
	ctx.lr = 0x829E92BC;
	sub_827DF490(ctx, base);
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// stw r3,-27996(r11)
	PPC_STORE_U32(ctx.r11.u32 + -27996, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E92D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829E92D8) { __imp__sub_829E92D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E92D8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23240
	ctx.r3.s64 = ctx.r11.s64 + -23240;
	// bl 0x827df490
	ctx.lr = 0x829E92F4;
	sub_827DF490(ctx, base);
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// stw r3,-27992(r11)
	PPC_STORE_U32(ctx.r11.u32 + -27992, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9310);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9310) { __imp__sub_829E9310(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9310) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23216
	ctx.r3.s64 = ctx.r11.s64 + -23216;
	// bl 0x827df490
	ctx.lr = 0x829E932C;
	sub_827DF490(ctx, base);
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// stw r3,-27988(r11)
	PPC_STORE_U32(ctx.r11.u32 + -27988, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9348);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9348) { __imp__sub_829E9348(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9348) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23204
	ctx.r3.s64 = ctx.r11.s64 + -23204;
	// bl 0x827df490
	ctx.lr = 0x829E9364;
	sub_827DF490(ctx, base);
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// stw r3,-27984(r11)
	PPC_STORE_U32(ctx.r11.u32 + -27984, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9380);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9380) { __imp__sub_829E9380(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9380) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23188
	ctx.r3.s64 = ctx.r11.s64 + -23188;
	// bl 0x827df490
	ctx.lr = 0x829E939C;
	sub_827DF490(ctx, base);
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// stw r3,-27980(r11)
	PPC_STORE_U32(ctx.r11.u32 + -27980, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E93B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829E93B8) { __imp__sub_829E93B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E93B8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23172
	ctx.r3.s64 = ctx.r11.s64 + -23172;
	// bl 0x827df490
	ctx.lr = 0x829E93D4;
	sub_827DF490(ctx, base);
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// stw r3,-27976(r11)
	PPC_STORE_U32(ctx.r11.u32 + -27976, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E93F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829E93F0) { __imp__sub_829E93F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E93F0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23156
	ctx.r3.s64 = ctx.r11.s64 + -23156;
	// bl 0x827df490
	ctx.lr = 0x829E940C;
	sub_827DF490(ctx, base);
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// stw r3,-27972(r11)
	PPC_STORE_U32(ctx.r11.u32 + -27972, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9428);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9428) { __imp__sub_829E9428(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9428) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23140
	ctx.r3.s64 = ctx.r11.s64 + -23140;
	// bl 0x827df490
	ctx.lr = 0x829E9444;
	sub_827DF490(ctx, base);
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// stw r3,-27968(r11)
	PPC_STORE_U32(ctx.r11.u32 + -27968, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9460);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9460) { __imp__sub_829E9460(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9460) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23124
	ctx.r3.s64 = ctx.r11.s64 + -23124;
	// bl 0x827df490
	ctx.lr = 0x829E947C;
	sub_827DF490(ctx, base);
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// stw r3,-27964(r11)
	PPC_STORE_U32(ctx.r11.u32 + -27964, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9498);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9498) { __imp__sub_829E9498(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9498) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23108
	ctx.r3.s64 = ctx.r11.s64 + -23108;
	// bl 0x827df490
	ctx.lr = 0x829E94B4;
	sub_827DF490(ctx, base);
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// stw r3,-27960(r11)
	PPC_STORE_U32(ctx.r11.u32 + -27960, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E94D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829E94D0) { __imp__sub_829E94D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E94D0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23092
	ctx.r3.s64 = ctx.r11.s64 + -23092;
	// bl 0x827df490
	ctx.lr = 0x829E94EC;
	sub_827DF490(ctx, base);
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// stw r3,-27956(r11)
	PPC_STORE_U32(ctx.r11.u32 + -27956, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9508);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9508) { __imp__sub_829E9508(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9508) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-12624
	ctx.r3.s64 = ctx.r11.s64 + -12624;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9518);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9518) { __imp__sub_829E9518(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9518) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-12592
	ctx.r3.s64 = ctx.r11.s64 + -12592;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9528);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9528) { __imp__sub_829E9528(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9528) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-12560
	ctx.r3.s64 = ctx.r11.s64 + -12560;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9538);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9538) { __imp__sub_829E9538(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9538) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-12528
	ctx.r3.s64 = ctx.r11.s64 + -12528;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9548);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9548) { __imp__sub_829E9548(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9548) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-12496
	ctx.r3.s64 = ctx.r11.s64 + -12496;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9558);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9558) { __imp__sub_829E9558(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9558) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// addi r3,r11,-27896
	ctx.r3.s64 = ctx.r11.s64 + -27896;
	// bl 0x8222b860
	ctx.lr = 0x829E9570;
	sub_8222B860(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-12464
	ctx.r3.s64 = ctx.r11.s64 + -12464;
	// bl 0x8298ed98
	ctx.lr = 0x829E957C;
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

PPC_FUNC_IMPL(__imp__sub_829E9590);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9590) { __imp__sub_829E9590(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9590) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// addi r3,r11,-27884
	ctx.r3.s64 = ctx.r11.s64 + -27884;
	// bl 0x8222b860
	ctx.lr = 0x829E95A8;
	sub_8222B860(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-12448
	ctx.r3.s64 = ctx.r11.s64 + -12448;
	// bl 0x8298ed98
	ctx.lr = 0x829E95B4;
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

PPC_FUNC_IMPL(__imp__sub_829E95C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829E95C8) { __imp__sub_829E95C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E95C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-12432
	ctx.r3.s64 = ctx.r11.s64 + -12432;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E95D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829E95D8) { __imp__sub_829E95D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E95D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// li r10,479
	ctx.r10.s64 = 479;
	// addi r11,r11,-27856
	ctx.r11.s64 = ctx.r11.s64 + -27856;
	// li r9,0
	ctx.r9.s64 = 0;
loc_829E95E8:
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
	// bge cr6,0x829e95e8
	if (!ctx.cr6.lt) goto loc_829E95E8;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-12344
	ctx.r3.s64 = ctx.r11.s64 + -12344;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9610);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9610) { __imp__sub_829E9610(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9610) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-12216
	ctx.r3.s64 = ctx.r11.s64 + -12216;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9620);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9620) { __imp__sub_829E9620(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9620) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-12144
	ctx.r3.s64 = ctx.r11.s64 + -12144;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9630);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9630) { __imp__sub_829E9630(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9630) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-12112
	ctx.r3.s64 = ctx.r11.s64 + -12112;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9640);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9640) { __imp__sub_829E9640(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9640) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-12080
	ctx.r3.s64 = ctx.r11.s64 + -12080;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9650);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9650) { __imp__sub_829E9650(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9650) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// li r10,34
	ctx.r10.s64 = 34;
	// addi r11,r11,10584
	ctx.r11.s64 = ctx.r11.s64 + 10584;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r11,r11,10
	ctx.r11.s64 = ctx.r11.s64 + 10;
loc_829E9664:
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
	// bge cr6,0x829e9664
	if (!ctx.cr6.lt) goto loc_829E9664;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-12024
	ctx.r3.s64 = ctx.r11.s64 + -12024;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9690);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9690) { __imp__sub_829E9690(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9690) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// li r10,54
	ctx.r10.s64 = 54;
	// addi r11,r11,11008
	ctx.r11.s64 = ctx.r11.s64 + 11008;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r11,r11,22
	ctx.r11.s64 = ctx.r11.s64 + 22;
loc_829E96A4:
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
	// bge cr6,0x829e96a4
	if (!ctx.cr6.lt) goto loc_829E96A4;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-11928
	ctx.r3.s64 = ctx.r11.s64 + -11928;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E96D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829E96D0) { __imp__sub_829E96D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E96D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// addi r31,r11,12328
	ctx.r31.s64 = ctx.r11.s64 + 12328;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x826a13b8
	ctx.lr = 0x829E96F0;
	sub_826A13B8(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-15476
	ctx.r11.s64 = ctx.r11.s64 + -15476;
	// addi r3,r10,-11832
	ctx.r3.s64 = ctx.r10.s64 + -11832;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829E9708;
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

PPC_FUNC_IMPL(__imp__sub_829E9720);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9720) { __imp__sub_829E9720(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9720) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x829E9728;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// li r31,15
	ctx.r31.s64 = 15;
	// addi r29,r11,29072
	ctx.r29.s64 = ctx.r11.s64 + 29072;
	// mr r30,r29
	ctx.r30.u64 = ctx.r29.u64;
loc_829E973C:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8296be18
	ctx.lr = 0x829E9744;
	sub_8296BE18(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,8
	ctx.r30.s64 = ctx.r30.s64 + 8;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x829e973c
	if (!ctx.cr6.lt) goto loc_829E973C;
	// li r11,0
	ctx.r11.s64 = 0;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r3,r10,-11816
	ctx.r3.s64 = ctx.r10.s64 + -11816;
	// stw r11,128(r29)
	PPC_STORE_U32(ctx.r29.u32 + 128, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829E9768;
	sub_8298ED98(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9770);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9770) { __imp__sub_829E9770(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9770) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-11808
	ctx.r3.s64 = ctx.r11.s64 + -11808;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9780);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9780) { __imp__sub_829E9780(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9780) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-11800
	ctx.r3.s64 = ctx.r11.s64 + -11800;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9790);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9790) { __imp__sub_829E9790(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9790) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// addi r3,r11,29044
	ctx.r3.s64 = ctx.r11.s64 + 29044;
	// bl 0x8296efa0
	ctx.lr = 0x829E97A8;
	sub_8296EFA0(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-11792
	ctx.r3.s64 = ctx.r11.s64 + -11792;
	// bl 0x8298ed98
	ctx.lr = 0x829E97B4;
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

PPC_FUNC_IMPL(__imp__sub_829E97C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829E97C8) { __imp__sub_829E97C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E97C8) {
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// addi r5,r11,-15464
	ctx.r5.s64 = ctx.r11.s64 + -15464;
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// addi r31,r11,29808
	ctx.r31.s64 = ctx.r11.s64 + 29808;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829E9800;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// lis r10,-31980
	ctx.r10.s64 = -2095841280;
	// addi r9,r11,29028
	ctx.r9.s64 = ctx.r11.s64 + 29028;
	// lwz r11,31992(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 31992);
	// stw r9,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r9.u32);
	// lis r9,-31980
	ctx.r9.s64 = -2095841280;
	// clrlwi r8,r11,31
	ctx.r8.u64 = ctx.r11.u32 & 0x1;
	// addi r30,r9,31968
	ctx.r30.s64 = ctx.r9.s64 + 31968;
	// cmplwi cr6,r8,0
	ctx.cr6.compare<uint32_t>(ctx.r8.u32, 0, ctx.xer);
	// bne cr6,0x829e9844
	if (!ctx.cr6.eq) goto loc_829E9844;
	// ori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 | 1;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// stw r11,31992(r10)
	PPC_STORE_U32(ctx.r10.u32 + 31992, ctx.r11.u32);
	// bl 0x8296ca40
	ctx.lr = 0x829E9838;
	sub_8296CA40(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-11760
	ctx.r3.s64 = ctx.r11.s64 + -11760;
	// bl 0x8298ed98
	ctx.lr = 0x829E9844;
	sub_8298ED98(ctx, base);
loc_829E9844:
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8296c788
	ctx.lr = 0x829E9850;
	sub_8296C788(ctx, base);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-15432
	ctx.r11.s64 = ctx.r11.s64 + -15432;
	// addi r3,r10,-11776
	ctx.r3.s64 = ctx.r10.s64 + -11776;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829E9868;
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

PPC_FUNC_IMPL(__imp__sub_829E9880);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9880) { __imp__sub_829E9880(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9880) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// lwz r10,-26376(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + -26376);
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r11,r11,1804
	ctx.r11.s64 = ctx.r11.s64 + 1804;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// lis r10,-32093
	ctx.r10.s64 = -2103246848;
	// lwz r10,-26372(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -26372);
	// stw r10,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
	// lis r10,-32093
	ctx.r10.s64 = -2103246848;
	// lwz r10,-26368(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -26368);
	// stw r10,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r10.u32);
	// lis r10,-32093
	ctx.r10.s64 = -2103246848;
	// lwz r10,-26364(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -26364);
	// stw r10,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r10.u32);
	// lis r10,-32093
	ctx.r10.s64 = -2103246848;
	// lwz r10,-26360(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -26360);
	// stw r10,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r10.u32);
	// lis r10,-32093
	ctx.r10.s64 = -2103246848;
	// lwz r10,-26356(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -26356);
	// stw r10,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r10.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E98D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829E98D8) { __imp__sub_829E98D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E98D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-11744
	ctx.r3.s64 = ctx.r11.s64 + -11744;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E98E8);  /* forward decl */
PPC_WEAK_FUNC(sub_829E98E8) { __imp__sub_829E98E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E98E8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x8218be28
	ctx.lr = 0x829E9900;
	sub_8218BE28(ctx, base);
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r31,r11,1712
	ctx.r31.s64 = ctx.r11.s64 + 1712;
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// li r3,16
	ctx.r3.s64 = 16;
	// bl 0x8218be28
	ctx.lr = 0x829E9914;
	sub_8218BE28(ctx, base);
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
	// b 0x829e9944
	goto loc_829E9944;
loc_829E9940:
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4);
loc_829E9944:
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
	// blt cr6,0x829e9940
	if (ctx.cr6.lt) goto loc_829E9940;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-11600
	ctx.r3.s64 = ctx.r11.s64 + -11600;
	// bl 0x8298ed98
	ctx.lr = 0x829E9978;
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

PPC_FUNC_IMPL(__imp__sub_829E9990);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9990) { __imp__sub_829E9990(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9990) {
	PPC_FUNC_PROLOGUE();
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
	// addi r11,r11,1740
	ctx.r11.s64 = ctx.r11.s64 + 1740;
	// addi r3,r11,4
	ctx.r3.s64 = ctx.r11.s64 + 4;
	// bl 0x82a0269c
	ctx.lr = 0x829E99AC;
	__imp__RtlInitializeCriticalSection(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-11400
	ctx.r3.s64 = ctx.r11.s64 + -11400;
	// bl 0x8298ed98
	ctx.lr = 0x829E99B8;
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

PPC_FUNC_IMPL(__imp__sub_829E99C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829E99C8) { __imp__sub_829E99C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E99C8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r11,r11,1772
	ctx.r11.s64 = ctx.r11.s64 + 1772;
	// addi r3,r11,4
	ctx.r3.s64 = ctx.r11.s64 + 4;
	// bl 0x82a0269c
	ctx.lr = 0x829E99E4;
	__imp__RtlInitializeCriticalSection(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-11376
	ctx.r3.s64 = ctx.r11.s64 + -11376;
	// bl 0x8298ed98
	ctx.lr = 0x829E99F0;
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

PPC_FUNC_IMPL(__imp__sub_829E9A00);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9A00) { __imp__sub_829E9A00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9A00) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// bl 0x8218c088
	ctx.lr = 0x829E9A10;
	sub_8218C088(ctx, base);
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
	ctx.lr = 0x829E9A2C;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-11304
	ctx.r3.s64 = ctx.r11.s64 + -11304;
	// bl 0x8298ed98
	ctx.lr = 0x829E9A38;
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

PPC_FUNC_IMPL(__imp__sub_829E9A48);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9A48) { __imp__sub_829E9A48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9A48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,2536
	ctx.r5.s64 = ctx.r11.s64 + 2536;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,8816
	ctx.r3.s64 = ctx.r11.s64 + 8816;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9A68);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9A68) { __imp__sub_829E9A68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9A68) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-11240
	ctx.r3.s64 = ctx.r11.s64 + -11240;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9A78);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9A78) { __imp__sub_829E9A78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9A78) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-11264
	ctx.r3.s64 = ctx.r11.s64 + -11264;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9A88);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9A88) { __imp__sub_829E9A88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9A88) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-11128
	ctx.r3.s64 = ctx.r11.s64 + -11128;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9A98);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9A98) { __imp__sub_829E9A98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9A98) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-11104
	ctx.r3.s64 = ctx.r11.s64 + -11104;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9AA8);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9AA8) { __imp__sub_829E9AA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9AA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-11080
	ctx.r3.s64 = ctx.r11.s64 + -11080;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9AB8);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9AB8) { __imp__sub_829E9AB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9AB8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-11056
	ctx.r3.s64 = ctx.r11.s64 + -11056;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9AC8);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9AC8) { __imp__sub_829E9AC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9AC8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-11032
	ctx.r3.s64 = ctx.r11.s64 + -11032;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9AD8);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9AD8) { __imp__sub_829E9AD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9AD8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,11136
	ctx.r3.s64 = ctx.r11.s64 + 11136;
	// bl 0x827f0848
	ctx.lr = 0x829E9AF0;
	sub_827F0848(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-11176
	ctx.r3.s64 = ctx.r11.s64 + -11176;
	// bl 0x8298ed98
	ctx.lr = 0x829E9AFC;
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

PPC_FUNC_IMPL(__imp__sub_829E9B10);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9B10) { __imp__sub_829E9B10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9B10) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12280
	ctx.r3.s64 = ctx.r11.s64 + 12280;
	// bl 0x827f0848
	ctx.lr = 0x829E9B28;
	sub_827F0848(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-11160
	ctx.r3.s64 = ctx.r11.s64 + -11160;
	// bl 0x8298ed98
	ctx.lr = 0x829E9B34;
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

PPC_FUNC_IMPL(__imp__sub_829E9B48);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9B48) { __imp__sub_829E9B48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9B48) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,13424
	ctx.r3.s64 = ctx.r11.s64 + 13424;
	// bl 0x827f0848
	ctx.lr = 0x829E9B60;
	sub_827F0848(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-11144
	ctx.r3.s64 = ctx.r11.s64 + -11144;
	// bl 0x8298ed98
	ctx.lr = 0x829E9B6C;
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

PPC_FUNC_IMPL(__imp__sub_829E9B80);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9B80) { __imp__sub_829E9B80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9B80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,4980
	ctx.r5.s64 = ctx.r11.s64 + 4980;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,14568
	ctx.r3.s64 = ctx.r11.s64 + 14568;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9BA0);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9BA0) { __imp__sub_829E9BA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9BA0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,4988
	ctx.r5.s64 = ctx.r11.s64 + 4988;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,14588
	ctx.r3.s64 = ctx.r11.s64 + 14588;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9BC0);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9BC0) { __imp__sub_829E9BC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9BC0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,5000
	ctx.r5.s64 = ctx.r11.s64 + 5000;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,14608
	ctx.r3.s64 = ctx.r11.s64 + 14608;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9BE0);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9BE0) { __imp__sub_829E9BE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9BE0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,5012
	ctx.r5.s64 = ctx.r11.s64 + 5012;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,14628
	ctx.r3.s64 = ctx.r11.s64 + 14628;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9C00);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9C00) { __imp__sub_829E9C00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9C00) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,5024
	ctx.r5.s64 = ctx.r11.s64 + 5024;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,14648
	ctx.r3.s64 = ctx.r11.s64 + 14648;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9C20);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9C20) { __imp__sub_829E9C20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9C20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,5036
	ctx.r5.s64 = ctx.r11.s64 + 5036;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,14668
	ctx.r3.s64 = ctx.r11.s64 + 14668;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9C40);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9C40) { __imp__sub_829E9C40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9C40) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,5048
	ctx.r5.s64 = ctx.r11.s64 + 5048;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,14688
	ctx.r3.s64 = ctx.r11.s64 + 14688;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9C60);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9C60) { __imp__sub_829E9C60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9C60) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,5064
	ctx.r5.s64 = ctx.r11.s64 + 5064;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,14708
	ctx.r3.s64 = ctx.r11.s64 + 14708;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9C80);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9C80) { __imp__sub_829E9C80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9C80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,5076
	ctx.r5.s64 = ctx.r11.s64 + 5076;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,14728
	ctx.r3.s64 = ctx.r11.s64 + 14728;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9CA0);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9CA0) { __imp__sub_829E9CA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9CA0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,5204
	ctx.r5.s64 = ctx.r11.s64 + 5204;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,15792
	ctx.r3.s64 = ctx.r11.s64 + 15792;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9CC0);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9CC0) { __imp__sub_829E9CC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9CC0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,5212
	ctx.r5.s64 = ctx.r11.s64 + 5212;
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,15812
	ctx.r3.s64 = ctx.r11.s64 + 15812;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9CE0);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9CE0) { __imp__sub_829E9CE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9CE0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,15832
	ctx.r3.s64 = ctx.r11.s64 + 15832;
	// bl 0x82192a78
	ctx.lr = 0x829E9CF8;
	sub_82192A78(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10992
	ctx.r3.s64 = ctx.r11.s64 + -10992;
	// bl 0x8298ed98
	ctx.lr = 0x829E9D04;
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

PPC_FUNC_IMPL(__imp__sub_829E9D18);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9D18) { __imp__sub_829E9D18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9D18) {
	PPC_FUNC_PROLOGUE();
loc_829E9D18:
	// mftb r11
	ctx.r11.u64 = PPC_QUERY_TIMEBASE();
	// rotlwi r10,r11,0
	ctx.r10.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x829e9d18
	if (ctx.cr6.eq) goto loc_829E9D18;
	// lis r10,-32086
	ctx.r10.s64 = -2102788096;
	// std r11,23920(r10)
	PPC_STORE_U64(ctx.r10.u32 + 23920, ctx.r11.u64);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9D38);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9D38) { __imp__sub_829E9D38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9D38) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,23928
	ctx.r3.s64 = ctx.r11.s64 + 23928;
	// bl 0x827f1320
	ctx.lr = 0x829E9D50;
	sub_827F1320(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-11008
	ctx.r3.s64 = ctx.r11.s64 + -11008;
	// bl 0x8298ed98
	ctx.lr = 0x829E9D5C;
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

PPC_FUNC_IMPL(__imp__sub_829E9D70);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9D70) { __imp__sub_829E9D70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9D70) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,23984
	ctx.r3.s64 = ctx.r11.s64 + 23984;
	// bl 0x82195528
	ctx.lr = 0x829E9D88;
	sub_82195528(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10912
	ctx.r3.s64 = ctx.r11.s64 + -10912;
	// bl 0x8298ed98
	ctx.lr = 0x829E9D94;
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

PPC_FUNC_IMPL(__imp__sub_829E9DA8);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9DA8) { __imp__sub_829E9DA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9DA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,5460
	ctx.r5.s64 = ctx.r11.s64 + 5460;
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-19792
	ctx.r3.s64 = ctx.r11.s64 + -19792;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9DC8);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9DC8) { __imp__sub_829E9DC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9DC8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// addi r3,r11,-19744
	ctx.r3.s64 = ctx.r11.s64 + -19744;
	// bl 0x82197088
	ctx.lr = 0x829E9DE0;
	sub_82197088(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10808
	ctx.r3.s64 = ctx.r11.s64 + -10808;
	// bl 0x8298ed98
	ctx.lr = 0x829E9DEC;
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

PPC_FUNC_IMPL(__imp__sub_829E9E00);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9E00) { __imp__sub_829E9E00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9E00) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// addi r11,r11,-19772
	ctx.r11.s64 = ctx.r11.s64 + -19772;
	// addi r3,r11,20
	ctx.r3.s64 = ctx.r11.s64 + 20;
	// bl 0x824b86f8
	ctx.lr = 0x829E9E1C;
	sub_824B86F8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10816
	ctx.r3.s64 = ctx.r11.s64 + -10816;
	// bl 0x8298ed98
	ctx.lr = 0x829E9E28;
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

PPC_FUNC_IMPL(__imp__sub_829E9E38);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9E38) { __imp__sub_829E9E38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9E38) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// addi r3,r11,-12248
	ctx.r3.s64 = ctx.r11.s64 + -12248;
	// b 0x827ee2d8
	sub_827EE2D8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9E48);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9E48) { __imp__sub_829E9E48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9E48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10792
	ctx.r3.s64 = ctx.r11.s64 + -10792;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9E58);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9E58) { __imp__sub_829E9E58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9E58) {
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
	// lis r11,-32083
	ctx.r11.s64 = -2102591488;
	// li r31,3
	ctx.r31.s64 = 3;
	// addi r30,r11,-17056
	ctx.r30.s64 = ctx.r11.s64 + -17056;
loc_829E9E78:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8230d730
	ctx.lr = 0x829E9E80;
	sub_8230D730(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,188
	ctx.r30.s64 = ctx.r30.s64 + 188;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x829e9e78
	if (!ctx.cr6.lt) goto loc_829E9E78;
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

PPC_FUNC_IMPL(__imp__sub_829E9EA8);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9EA8) { __imp__sub_829E9EA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9EA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32083
	ctx.r11.s64 = -2102591488;
	// addi r3,r11,-16304
	ctx.r3.s64 = ctx.r11.s64 + -16304;
	// b 0x8230d730
	sub_8230D730(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829E9EB8);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9EB8) { __imp__sub_829E9EB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9EB8) {
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
	// lis r11,-32083
	ctx.r11.s64 = -2102591488;
	// li r31,3
	ctx.r31.s64 = 3;
	// addi r30,r11,-16072
	ctx.r30.s64 = ctx.r11.s64 + -16072;
loc_829E9ED8:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x821ec5c8
	ctx.lr = 0x829E9EE0;
	sub_821EC5C8(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,4220
	ctx.r30.s64 = ctx.r30.s64 + 4220;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x829e9ed8
	if (!ctx.cr6.lt) goto loc_829E9ED8;
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

PPC_FUNC_IMPL(__imp__sub_829E9F08);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9F08) { __imp__sub_829E9F08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9F08) {
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
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,2744
	ctx.r5.s64 = ctx.r11.s64 + 2744;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829E9F30;
	sub_827DB118(ctx, base);
	// lis r9,-32238
	ctx.r9.s64 = -2112749568;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r9,r9,12776
	ctx.r9.s64 = ctx.r9.s64 + 12776;
	// lis r10,-32083
	ctx.r10.s64 = -2102591488;
	// addi r10,r10,808
	ctx.r10.s64 = ctx.r10.s64 + 808;
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

PPC_FUNC_IMPL(__imp__sub_829E9F78);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9F78) { __imp__sub_829E9F78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9F78) {
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
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,2744
	ctx.r5.s64 = ctx.r11.s64 + 2744;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829E9FA0;
	sub_827DB118(ctx, base);
	// lis r9,-32238
	ctx.r9.s64 = -2112749568;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r9,r9,12776
	ctx.r9.s64 = ctx.r9.s64 + 12776;
	// lis r10,-32083
	ctx.r10.s64 = -2102591488;
	// addi r10,r10,824
	ctx.r10.s64 = ctx.r10.s64 + 824;
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

PPC_FUNC_IMPL(__imp__sub_829E9FE8);  /* forward decl */
PPC_WEAK_FUNC(sub_829E9FE8) { __imp__sub_829E9FE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829E9FE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32083
	ctx.r11.s64 = -2102591488;
	// li r9,-1
	ctx.r9.s64 = -1;
	// addi r11,r11,9056
	ctx.r11.s64 = ctx.r11.s64 + 9056;
	// li r10,512
	ctx.r10.s64 = 512;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_829E9FFC:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x829e9ffc
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_829E9FFC;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EA010);  /* forward decl */
PPC_WEAK_FUNC(sub_829EA010) { __imp__sub_829EA010(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EA010) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32083
	ctx.r11.s64 = -2102591488;
	// li r10,255
	ctx.r10.s64 = 255;
	// addi r11,r11,24408
	ctx.r11.s64 = ctx.r11.s64 + 24408;
	// li r9,0
	ctx.r9.s64 = 0;
loc_829EA020:
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
	// bge cr6,0x829ea020
	if (!ctx.cr6.lt) goto loc_829EA020;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EA040);  /* forward decl */
PPC_WEAK_FUNC(sub_829EA040) { __imp__sub_829EA040(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EA040) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32083
	ctx.r11.s64 = -2102591488;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r11,r11,26456
	ctx.r11.s64 = ctx.r11.s64 + 26456;
	// li r10,128
	ctx.r10.s64 = 128;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_829EA054:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x829ea054
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_829EA054;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10760
	ctx.r3.s64 = ctx.r11.s64 + -10760;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EA070);  /* forward decl */
PPC_WEAK_FUNC(sub_829EA070) { __imp__sub_829EA070(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EA070) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32083
	ctx.r11.s64 = -2102591488;
	// addi r3,r11,27056
	ctx.r3.s64 = ctx.r11.s64 + 27056;
	// b 0x821a9e38
	sub_821A9E38(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EA080);  /* forward decl */
PPC_WEAK_FUNC(sub_829EA080) { __imp__sub_829EA080(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EA080) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10656
	ctx.r3.s64 = ctx.r11.s64 + -10656;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EA090);  /* forward decl */
PPC_WEAK_FUNC(sub_829EA090) { __imp__sub_829EA090(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EA090) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32081
	ctx.r11.s64 = -2102460416;
	// addi r7,r10,-8560
	ctx.r7.s64 = ctx.r10.s64 + -8560;
	// lis r6,-32256
	ctx.r6.s64 = -2113929216;
	// addi r11,r11,-18944
	ctx.r11.s64 = ctx.r11.s64 + -18944;
	// lis r8,0
	ctx.r8.s64 = 0;
	// li r9,15
	ctx.r9.s64 = 15;
	// lvx128 v0,r0,r7
	ea = (ctx.r7.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r11,32
	ctx.r11.s64 = ctx.r11.s64 + 32;
	// lfs f0,3068(r6)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + 3068);
	ctx.f0.f64 = double(temp.f32);
	// li r10,0
	ctx.r10.s64 = 0;
	// li r4,-16
	ctx.r4.s64 = -16;
	// li r5,16
	ctx.r5.s64 = 16;
	// ori r8,r8,65535
	ctx.r8.u64 = ctx.r8.u64 | 65535;
loc_829EA0C8:
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
	// bge cr6,0x829ea0c8
	if (!ctx.cr6.lt) goto loc_829EA0C8;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EA108);  /* forward decl */
PPC_WEAK_FUNC(sub_829EA108) { __imp__sub_829EA108(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EA108) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32081
	ctx.r11.s64 = -2102460416;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r9,r11,-16256
	ctx.r9.s64 = ctx.r11.s64 + -16256;
	// addi r11,r9,64
	ctx.r11.s64 = ctx.r9.s64 + 64;
loc_829EA118:
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
	// blt cr6,0x829ea118
	if (ctx.cr6.lt) goto loc_829EA118;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EA138);  /* forward decl */
PPC_WEAK_FUNC(sub_829EA138) { __imp__sub_829EA138(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EA138) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32081
	ctx.r11.s64 = -2102460416;
	// addi r3,r11,-15856
	ctx.r3.s64 = ctx.r11.s64 + -15856;
	// bl 0x821b5d48
	ctx.lr = 0x829EA150;
	sub_821B5D48(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10648
	ctx.r3.s64 = ctx.r11.s64 + -10648;
	// bl 0x8298ed98
	ctx.lr = 0x829EA15C;
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

PPC_FUNC_IMPL(__imp__sub_829EA170);  /* forward decl */
PPC_WEAK_FUNC(sub_829EA170) { __imp__sub_829EA170(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EA170) {
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
	// lis r11,-32079
	ctx.r11.s64 = -2102329344;
	// li r31,9
	ctx.r31.s64 = 9;
	// addi r30,r11,4544
	ctx.r30.s64 = ctx.r11.s64 + 4544;
loc_829EA190:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x821abaf0
	ctx.lr = 0x829EA198;
	sub_821ABAF0(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,96
	ctx.r30.s64 = ctx.r30.s64 + 96;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x829ea190
	if (!ctx.cr6.lt) goto loc_829EA190;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10576
	ctx.r3.s64 = ctx.r11.s64 + -10576;
	// bl 0x8298ed98
	ctx.lr = 0x829EA1B4;
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

PPC_FUNC_IMPL(__imp__sub_829EA1D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EA1D0) { __imp__sub_829EA1D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EA1D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32079
	ctx.r11.s64 = -2102329344;
	// addi r3,r11,13616
	ctx.r3.s64 = ctx.r11.s64 + 13616;
	// b 0x821bcd78
	sub_821BCD78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EA1E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EA1E0) { __imp__sub_829EA1E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EA1E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32074
	ctx.r11.s64 = -2102001664;
	// addi r3,r11,-17616
	ctx.r3.s64 = ctx.r11.s64 + -17616;
	// bl 0x827f1320
	ctx.lr = 0x829EA1F8;
	sub_827F1320(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10488
	ctx.r3.s64 = ctx.r11.s64 + -10488;
	// bl 0x8298ed98
	ctx.lr = 0x829EA204;
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

PPC_FUNC_IMPL(__imp__sub_829EA218);  /* forward decl */
PPC_WEAK_FUNC(sub_829EA218) { __imp__sub_829EA218(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EA218) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32074
	ctx.r11.s64 = -2102001664;
	// li r9,-1
	ctx.r9.s64 = -1;
	// addi r11,r11,-9392
	ctx.r11.s64 = ctx.r11.s64 + -9392;
	// li r10,128
	ctx.r10.s64 = 128;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_829EA22C:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x829ea22c
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_829EA22C;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EA240);  /* forward decl */
PPC_WEAK_FUNC(sub_829EA240) { __imp__sub_829EA240(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EA240) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32074
	ctx.r11.s64 = -2102001664;
	// li r9,-1
	ctx.r9.s64 = -1;
	// addi r11,r11,-8880
	ctx.r11.s64 = ctx.r11.s64 + -8880;
	// li r10,128
	ctx.r10.s64 = 128;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_829EA254:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x829ea254
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_829EA254;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EA268);  /* forward decl */
PPC_WEAK_FUNC(sub_829EA268) { __imp__sub_829EA268(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EA268) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,13956
	ctx.r5.s64 = ctx.r11.s64 + 13956;
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,1896
	ctx.r3.s64 = ctx.r11.s64 + 1896;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EA288);  /* forward decl */
PPC_WEAK_FUNC(sub_829EA288) { __imp__sub_829EA288(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EA288) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// li r9,255
	ctx.r9.s64 = 255;
	// addi r11,r11,3712
	ctx.r11.s64 = ctx.r11.s64 + 3712;
	// addi r10,r11,48
	ctx.r10.s64 = ctx.r11.s64 + 48;
	// li r11,-1
	ctx.r11.s64 = -1;
loc_829EA29C:
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
	// bge cr6,0x829ea29c
	if (!ctx.cr6.lt) goto loc_829EA29C;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EA2C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EA2C0) { __imp__sub_829EA2C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EA2C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32071
	ctx.r11.s64 = -2101805056;
	// addi r3,r11,-30080
	ctx.r3.s64 = ctx.r11.s64 + -30080;
	// b 0x821d7f80
	sub_821D7F80(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EA2D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EA2D0) { __imp__sub_829EA2D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EA2D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32071
	ctx.r11.s64 = -2101805056;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r11,r11,-6912
	ctx.r11.s64 = ctx.r11.s64 + -6912;
	// li r10,41
	ctx.r10.s64 = 41;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_829EA2E4:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x829ea2e4
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_829EA2E4;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10472
	ctx.r3.s64 = ctx.r11.s64 + -10472;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EA300);  /* forward decl */
PPC_WEAK_FUNC(sub_829EA300) { __imp__sub_829EA300(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EA300) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32071
	ctx.r11.s64 = -2101805056;
	// li r10,-1
	ctx.r10.s64 = -1;
	// addi r11,r11,-6744
	ctx.r11.s64 = ctx.r11.s64 + -6744;
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

PPC_FUNC_IMPL(__imp__sub_829EA328);  /* forward decl */
PPC_WEAK_FUNC(sub_829EA328) { __imp__sub_829EA328(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EA328) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,20364
	ctx.r5.s64 = ctx.r11.s64 + 20364;
	// lis r11,-32071
	ctx.r11.s64 = -2101805056;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,17772
	ctx.r3.s64 = ctx.r11.s64 + 17772;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EA348);  /* forward decl */
PPC_WEAK_FUNC(sub_829EA348) { __imp__sub_829EA348(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EA348) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,20380
	ctx.r5.s64 = ctx.r11.s64 + 20380;
	// lis r11,-32071
	ctx.r11.s64 = -2101805056;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,17792
	ctx.r3.s64 = ctx.r11.s64 + 17792;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EA368);  /* forward decl */
PPC_WEAK_FUNC(sub_829EA368) { __imp__sub_829EA368(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EA368) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32071
	ctx.r11.s64 = -2101805056;
	// addi r3,r11,17816
	ctx.r3.s64 = ctx.r11.s64 + 17816;
	// b 0x8296be18
	sub_8296BE18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EA378);  /* forward decl */
PPC_WEAK_FUNC(sub_829EA378) { __imp__sub_829EA378(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EA378) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,3068(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3068);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32094
	ctx.r11.s64 = -2103312384;
	// stfs f0,-32(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -32, temp.u32);
	// stfs f0,-28(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -28, temp.u32);
	// addi r11,r11,20112
	ctx.r11.s64 = ctx.r11.s64 + 20112;
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

PPC_FUNC_IMPL(__imp__sub_829EAE10);  /* forward decl */
PPC_WEAK_FUNC(sub_829EAE10) { __imp__sub_829EAE10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EAE10) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10384
	ctx.r3.s64 = ctx.r11.s64 + -10384;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EAE20);  /* forward decl */
PPC_WEAK_FUNC(sub_829EAE20) { __imp__sub_829EAE20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EAE20) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// lis r9,-32070
	ctx.r9.s64 = -2101739520;
	// addi r11,r11,-13792
	ctx.r11.s64 = ctx.r11.s64 + -13792;
	// lwz r10,-11744(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -11744);
	// rlwinm r10,r10,10,0,21
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 10) & 0xFFFFFC00;
	// add r11,r10,r11
	ctx.r11.u64 = ctx.r10.u64 + ctx.r11.u64;
	// stw r11,-11712(r9)
	PPC_STORE_U32(ctx.r9.u32 + -11712, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EAE48);  /* forward decl */
PPC_WEAK_FUNC(sub_829EAE48) { __imp__sub_829EAE48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EAE48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r3,r11,-11688
	ctx.r3.s64 = ctx.r11.s64 + -11688;
	// b 0x8296c2f0
	sub_8296C2F0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EAE58);  /* forward decl */
PPC_WEAK_FUNC(sub_829EAE58) { __imp__sub_829EAE58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EAE58) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,29632
	ctx.r5.s64 = ctx.r11.s64 + 29632;
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-11708
	ctx.r3.s64 = ctx.r11.s64 + -11708;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EAE78);  /* forward decl */
PPC_WEAK_FUNC(sub_829EAE78) { __imp__sub_829EAE78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EAE78) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,29648
	ctx.r5.s64 = ctx.r11.s64 + 29648;
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-11592
	ctx.r3.s64 = ctx.r11.s64 + -11592;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EAE98);  /* forward decl */
PPC_WEAK_FUNC(sub_829EAE98) { __imp__sub_829EAE98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EAE98) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,29668
	ctx.r5.s64 = ctx.r11.s64 + 29668;
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-11572
	ctx.r3.s64 = ctx.r11.s64 + -11572;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EAEB8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EAEB8) { __imp__sub_829EAEB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EAEB8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// lwz r11,21172(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 21172);
	// stw r11,-11552(r10)
	PPC_STORE_U32(ctx.r10.u32 + -11552, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EAED0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EAED0) { __imp__sub_829EAED0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EAED0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-11548
	ctx.r3.s64 = ctx.r11.s64 + -11548;
	// bl 0x827f1320
	ctx.lr = 0x829EAEE8;
	sub_827F1320(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10376
	ctx.r3.s64 = ctx.r11.s64 + -10376;
	// bl 0x8298ed98
	ctx.lr = 0x829EAEF4;
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

PPC_FUNC_IMPL(__imp__sub_829EAF08);  /* forward decl */
PPC_WEAK_FUNC(sub_829EAF08) { __imp__sub_829EAF08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EAF08) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r3,r11,-11512
	ctx.r3.s64 = ctx.r11.s64 + -11512;
	// b 0x8296c2f0
	sub_8296C2F0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EAF18);  /* forward decl */
PPC_WEAK_FUNC(sub_829EAF18) { __imp__sub_829EAF18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EAF18) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10360
	ctx.r3.s64 = ctx.r11.s64 + -10360;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EAF28);  /* forward decl */
PPC_WEAK_FUNC(sub_829EAF28) { __imp__sub_829EAF28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EAF28) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10264
	ctx.r3.s64 = ctx.r11.s64 + -10264;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EAF38);  /* forward decl */
PPC_WEAK_FUNC(sub_829EAF38) { __imp__sub_829EAF38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EAF38) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10328
	ctx.r3.s64 = ctx.r11.s64 + -10328;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EAF48);  /* forward decl */
PPC_WEAK_FUNC(sub_829EAF48) { __imp__sub_829EAF48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EAF48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10248
	ctx.r3.s64 = ctx.r11.s64 + -10248;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EAF58);  /* forward decl */
PPC_WEAK_FUNC(sub_829EAF58) { __imp__sub_829EAF58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EAF58) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10232
	ctx.r3.s64 = ctx.r11.s64 + -10232;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EAF68);  /* forward decl */
PPC_WEAK_FUNC(sub_829EAF68) { __imp__sub_829EAF68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EAF68) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10296
	ctx.r3.s64 = ctx.r11.s64 + -10296;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EAF78);  /* forward decl */
PPC_WEAK_FUNC(sub_829EAF78) { __imp__sub_829EAF78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EAF78) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10216
	ctx.r3.s64 = ctx.r11.s64 + -10216;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EAF88);  /* forward decl */
PPC_WEAK_FUNC(sub_829EAF88) { __imp__sub_829EAF88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EAF88) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10208
	ctx.r3.s64 = ctx.r11.s64 + -10208;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EAF98);  /* forward decl */
PPC_WEAK_FUNC(sub_829EAF98) { __imp__sub_829EAF98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EAF98) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10200
	ctx.r3.s64 = ctx.r11.s64 + -10200;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EAFA8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EAFA8) { __imp__sub_829EAFA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EAFA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10192
	ctx.r3.s64 = ctx.r11.s64 + -10192;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EAFB8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EAFB8) { __imp__sub_829EAFB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EAFB8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10184
	ctx.r3.s64 = ctx.r11.s64 + -10184;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EAFC8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EAFC8) { __imp__sub_829EAFC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EAFC8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10176
	ctx.r3.s64 = ctx.r11.s64 + -10176;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EAFD8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EAFD8) { __imp__sub_829EAFD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EAFD8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10160
	ctx.r3.s64 = ctx.r11.s64 + -10160;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EAFE8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EAFE8) { __imp__sub_829EAFE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EAFE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10168
	ctx.r3.s64 = ctx.r11.s64 + -10168;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EAFF8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EAFF8) { __imp__sub_829EAFF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EAFF8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8544
	ctx.r11.s64 = ctx.r11.s64 + -8544;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r11,r11,-5248
	ctx.r11.s64 = ctx.r11.s64 + -5248;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB018);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB018) { __imp__sub_829EB018(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB018) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-5216
	ctx.r3.s64 = ctx.r11.s64 + -5216;
	// bl 0x825149c0
	ctx.lr = 0x829EB030;
	sub_825149C0(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10128
	ctx.r3.s64 = ctx.r11.s64 + -10128;
	// bl 0x8298ed98
	ctx.lr = 0x829EB03C;
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

PPC_FUNC_IMPL(__imp__sub_829EB050);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB050) { __imp__sub_829EB050(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB050) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25856
	ctx.r3.s64 = ctx.r11.s64 + -25856;
	// bl 0x827df490
	ctx.lr = 0x829EB06C;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5112(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5112, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB088);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB088) { __imp__sub_829EB088(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB088) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25836
	ctx.r3.s64 = ctx.r11.s64 + -25836;
	// bl 0x827df490
	ctx.lr = 0x829EB0A4;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5108(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5108, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB0C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB0C0) { __imp__sub_829EB0C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB0C0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25812
	ctx.r3.s64 = ctx.r11.s64 + -25812;
	// bl 0x827df490
	ctx.lr = 0x829EB0DC;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5104(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5104, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB0F8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB0F8) { __imp__sub_829EB0F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB0F8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25792
	ctx.r3.s64 = ctx.r11.s64 + -25792;
	// bl 0x827df490
	ctx.lr = 0x829EB114;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5100(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5100, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB130);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB130) { __imp__sub_829EB130(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB130) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25752
	ctx.r3.s64 = ctx.r11.s64 + -25752;
	// bl 0x827df490
	ctx.lr = 0x829EB14C;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5096(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5096, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB168);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB168) { __imp__sub_829EB168(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB168) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25704
	ctx.r3.s64 = ctx.r11.s64 + -25704;
	// bl 0x827df490
	ctx.lr = 0x829EB184;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5092(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5092, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB1A0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB1A0) { __imp__sub_829EB1A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB1A0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25680
	ctx.r3.s64 = ctx.r11.s64 + -25680;
	// bl 0x827df490
	ctx.lr = 0x829EB1BC;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5088(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5088, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB1D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB1D8) { __imp__sub_829EB1D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB1D8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25656
	ctx.r3.s64 = ctx.r11.s64 + -25656;
	// bl 0x827df490
	ctx.lr = 0x829EB1F4;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5084(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5084, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB210);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB210) { __imp__sub_829EB210(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB210) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25624
	ctx.r3.s64 = ctx.r11.s64 + -25624;
	// bl 0x827df490
	ctx.lr = 0x829EB22C;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5080(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5080, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB248);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB248) { __imp__sub_829EB248(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB248) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25592
	ctx.r3.s64 = ctx.r11.s64 + -25592;
	// bl 0x827df490
	ctx.lr = 0x829EB264;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5076(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5076, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB280);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB280) { __imp__sub_829EB280(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB280) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25568
	ctx.r3.s64 = ctx.r11.s64 + -25568;
	// bl 0x827df490
	ctx.lr = 0x829EB29C;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5072(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5072, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB2B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB2B8) { __imp__sub_829EB2B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB2B8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25540
	ctx.r3.s64 = ctx.r11.s64 + -25540;
	// bl 0x827df490
	ctx.lr = 0x829EB2D4;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5068(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5068, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB2F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB2F0) { __imp__sub_829EB2F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB2F0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25508
	ctx.r3.s64 = ctx.r11.s64 + -25508;
	// bl 0x827df490
	ctx.lr = 0x829EB30C;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5064(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5064, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB328);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB328) { __imp__sub_829EB328(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB328) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25472
	ctx.r3.s64 = ctx.r11.s64 + -25472;
	// bl 0x827df490
	ctx.lr = 0x829EB344;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5060(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5060, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB360);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB360) { __imp__sub_829EB360(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB360) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25452
	ctx.r3.s64 = ctx.r11.s64 + -25452;
	// bl 0x827df490
	ctx.lr = 0x829EB37C;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5056(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5056, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB398);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB398) { __imp__sub_829EB398(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB398) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25428
	ctx.r3.s64 = ctx.r11.s64 + -25428;
	// bl 0x827df490
	ctx.lr = 0x829EB3B4;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5052(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5052, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB3D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB3D0) { __imp__sub_829EB3D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB3D0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25400
	ctx.r3.s64 = ctx.r11.s64 + -25400;
	// bl 0x827df490
	ctx.lr = 0x829EB3EC;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5048(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5048, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB408);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB408) { __imp__sub_829EB408(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB408) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25368
	ctx.r3.s64 = ctx.r11.s64 + -25368;
	// bl 0x827df490
	ctx.lr = 0x829EB424;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5044(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5044, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB440);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB440) { __imp__sub_829EB440(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB440) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25348
	ctx.r3.s64 = ctx.r11.s64 + -25348;
	// bl 0x827df490
	ctx.lr = 0x829EB45C;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5040(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5040, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB478);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB478) { __imp__sub_829EB478(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB478) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25324
	ctx.r3.s64 = ctx.r11.s64 + -25324;
	// bl 0x827df490
	ctx.lr = 0x829EB494;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5036(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5036, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB4B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB4B0) { __imp__sub_829EB4B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB4B0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25296
	ctx.r3.s64 = ctx.r11.s64 + -25296;
	// bl 0x827df490
	ctx.lr = 0x829EB4CC;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5032(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5032, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB4E8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB4E8) { __imp__sub_829EB4E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB4E8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25264
	ctx.r3.s64 = ctx.r11.s64 + -25264;
	// bl 0x827df490
	ctx.lr = 0x829EB504;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5028(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5028, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB520);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB520) { __imp__sub_829EB520(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB520) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25244
	ctx.r3.s64 = ctx.r11.s64 + -25244;
	// bl 0x827df490
	ctx.lr = 0x829EB53C;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5024(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5024, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB558);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB558) { __imp__sub_829EB558(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB558) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25220
	ctx.r3.s64 = ctx.r11.s64 + -25220;
	// bl 0x827df490
	ctx.lr = 0x829EB574;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5020(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5020, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB590);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB590) { __imp__sub_829EB590(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB590) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25192
	ctx.r3.s64 = ctx.r11.s64 + -25192;
	// bl 0x827df490
	ctx.lr = 0x829EB5AC;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5016(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5016, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB5C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB5C8) { __imp__sub_829EB5C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB5C8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25160
	ctx.r3.s64 = ctx.r11.s64 + -25160;
	// bl 0x827df490
	ctx.lr = 0x829EB5E4;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5012(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5012, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB600);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB600) { __imp__sub_829EB600(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB600) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25140
	ctx.r3.s64 = ctx.r11.s64 + -25140;
	// bl 0x827df490
	ctx.lr = 0x829EB61C;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5008(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5008, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB638);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB638) { __imp__sub_829EB638(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB638) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25120
	ctx.r3.s64 = ctx.r11.s64 + -25120;
	// bl 0x827df490
	ctx.lr = 0x829EB654;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5004(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5004, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB670);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB670) { __imp__sub_829EB670(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB670) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25100
	ctx.r3.s64 = ctx.r11.s64 + -25100;
	// bl 0x827df490
	ctx.lr = 0x829EB68C;
	sub_827DF490(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stw r3,-5000(r11)
	PPC_STORE_U32(ctx.r11.u32 + -5000, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB6A8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB6A8) { __imp__sub_829EB6A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB6A8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-1808
	ctx.r3.s64 = ctx.r11.s64 + -1808;
	// bl 0x828a1588
	ctx.lr = 0x829EB6C0;
	sub_828A1588(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10112
	ctx.r3.s64 = ctx.r11.s64 + -10112;
	// bl 0x8298ed98
	ctx.lr = 0x829EB6CC;
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

PPC_FUNC_IMPL(__imp__sub_829EB6E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB6E0) { __imp__sub_829EB6E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB6E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-9992
	ctx.r3.s64 = ctx.r11.s64 + -9992;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB6F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB6F0) { __imp__sub_829EB6F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB6F0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,10272
	ctx.r3.s64 = ctx.r11.s64 + 10272;
	// bl 0x825343e8
	ctx.lr = 0x829EB708;
	sub_825343E8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10096
	ctx.r3.s64 = ctx.r11.s64 + -10096;
	// bl 0x8298ed98
	ctx.lr = 0x829EB714;
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

PPC_FUNC_IMPL(__imp__sub_829EB728);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB728) { __imp__sub_829EB728(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB728) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,10244
	ctx.r3.s64 = ctx.r11.s64 + 10244;
	// bl 0x828a2de0
	ctx.lr = 0x829EB740;
	sub_828A2DE0(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10080
	ctx.r3.s64 = ctx.r11.s64 + -10080;
	// bl 0x8298ed98
	ctx.lr = 0x829EB74C;
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

PPC_FUNC_IMPL(__imp__sub_829EB760);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB760) { __imp__sub_829EB760(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB760) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,23088
	ctx.r3.s64 = ctx.r11.s64 + 23088;
	// bl 0x828a2de0
	ctx.lr = 0x829EB778;
	sub_828A2DE0(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10072
	ctx.r3.s64 = ctx.r11.s64 + -10072;
	// bl 0x8298ed98
	ctx.lr = 0x829EB784;
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

PPC_FUNC_IMPL(__imp__sub_829EB798);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB798) { __imp__sub_829EB798(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB798) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,23116
	ctx.r3.s64 = ctx.r11.s64 + 23116;
	// bl 0x828a2de0
	ctx.lr = 0x829EB7B0;
	sub_828A2DE0(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10064
	ctx.r3.s64 = ctx.r11.s64 + -10064;
	// bl 0x8298ed98
	ctx.lr = 0x829EB7BC;
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

PPC_FUNC_IMPL(__imp__sub_829EB7D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB7D0) { __imp__sub_829EB7D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB7D0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,23144
	ctx.r3.s64 = ctx.r11.s64 + 23144;
	// bl 0x828a2de0
	ctx.lr = 0x829EB7E8;
	sub_828A2DE0(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10056
	ctx.r3.s64 = ctx.r11.s64 + -10056;
	// bl 0x8298ed98
	ctx.lr = 0x829EB7F4;
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

PPC_FUNC_IMPL(__imp__sub_829EB808);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB808) { __imp__sub_829EB808(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB808) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,23172
	ctx.r3.s64 = ctx.r11.s64 + 23172;
	// bl 0x828a2de0
	ctx.lr = 0x829EB820;
	sub_828A2DE0(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10048
	ctx.r3.s64 = ctx.r11.s64 + -10048;
	// bl 0x8298ed98
	ctx.lr = 0x829EB82C;
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

PPC_FUNC_IMPL(__imp__sub_829EB840);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB840) { __imp__sub_829EB840(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB840) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,23200
	ctx.r3.s64 = ctx.r11.s64 + 23200;
	// bl 0x828a2de0
	ctx.lr = 0x829EB858;
	sub_828A2DE0(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10040
	ctx.r3.s64 = ctx.r11.s64 + -10040;
	// bl 0x8298ed98
	ctx.lr = 0x829EB864;
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

PPC_FUNC_IMPL(__imp__sub_829EB878);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB878) { __imp__sub_829EB878(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB878) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,23228
	ctx.r3.s64 = ctx.r11.s64 + 23228;
	// bl 0x828a2de0
	ctx.lr = 0x829EB890;
	sub_828A2DE0(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10032
	ctx.r3.s64 = ctx.r11.s64 + -10032;
	// bl 0x8298ed98
	ctx.lr = 0x829EB89C;
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

PPC_FUNC_IMPL(__imp__sub_829EB8B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB8B0) { __imp__sub_829EB8B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB8B0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,23256
	ctx.r3.s64 = ctx.r11.s64 + 23256;
	// bl 0x8289df08
	ctx.lr = 0x829EB8C8;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10024
	ctx.r3.s64 = ctx.r11.s64 + -10024;
	// bl 0x8298ed98
	ctx.lr = 0x829EB8D4;
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

PPC_FUNC_IMPL(__imp__sub_829EB8E8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB8E8) { __imp__sub_829EB8E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB8E8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,23296
	ctx.r3.s64 = ctx.r11.s64 + 23296;
	// bl 0x8289df08
	ctx.lr = 0x829EB900;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10016
	ctx.r3.s64 = ctx.r11.s64 + -10016;
	// bl 0x8298ed98
	ctx.lr = 0x829EB90C;
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

PPC_FUNC_IMPL(__imp__sub_829EB920);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB920) { __imp__sub_829EB920(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB920) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,23336
	ctx.r3.s64 = ctx.r11.s64 + 23336;
	// bl 0x8289df08
	ctx.lr = 0x829EB938;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10008
	ctx.r3.s64 = ctx.r11.s64 + -10008;
	// bl 0x8298ed98
	ctx.lr = 0x829EB944;
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

PPC_FUNC_IMPL(__imp__sub_829EB958);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB958) { __imp__sub_829EB958(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB958) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,23376
	ctx.r3.s64 = ctx.r11.s64 + 23376;
	// bl 0x8289df08
	ctx.lr = 0x829EB970;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-10000
	ctx.r3.s64 = ctx.r11.s64 + -10000;
	// bl 0x8298ed98
	ctx.lr = 0x829EB97C;
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

PPC_FUNC_IMPL(__imp__sub_829EB990);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB990) { __imp__sub_829EB990(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB990) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-23972
	ctx.r5.s64 = ctx.r11.s64 + -23972;
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,23416
	ctx.r3.s64 = ctx.r11.s64 + 23416;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB9B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB9B0) { __imp__sub_829EB9B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB9B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32066
	ctx.r11.s64 = -2101477376;
	// lis r10,-32068
	ctx.r10.s64 = -2101608448;
	// lbz r11,22256(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 22256);
	// stb r11,2608(r10)
	PPC_STORE_U8(ctx.r10.u32 + 2608, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EB9C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EB9C8) { __imp__sub_829EB9C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EB9C8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,5664
	ctx.r3.s64 = ctx.r11.s64 + 5664;
	// bl 0x8284f880
	ctx.lr = 0x829EB9E0;
	sub_8284F880(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-9968
	ctx.r3.s64 = ctx.r11.s64 + -9968;
	// bl 0x8298ed98
	ctx.lr = 0x829EB9EC;
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

PPC_FUNC_IMPL(__imp__sub_829EBA00);  /* forward decl */
PPC_WEAK_FUNC(sub_829EBA00) { __imp__sub_829EBA00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EBA00) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32066
	ctx.r11.s64 = -2101477376;
	// addi r3,r11,9560
	ctx.r3.s64 = ctx.r11.s64 + 9560;
	// bl 0x8222b888
	ctx.lr = 0x829EBA18;
	sub_8222B888(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-9952
	ctx.r3.s64 = ctx.r11.s64 + -9952;
	// bl 0x8298ed98
	ctx.lr = 0x829EBA24;
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

PPC_FUNC_IMPL(__imp__sub_829EBA38);  /* forward decl */
PPC_WEAK_FUNC(sub_829EBA38) { __imp__sub_829EBA38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EBA38) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x829EBA40;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32066
	ctx.r11.s64 = -2101477376;
	// li r30,7
	ctx.r30.s64 = 7;
	// addi r11,r11,10064
	ctx.r11.s64 = ctx.r11.s64 + 10064;
	// li r29,-1
	ctx.r29.s64 = -1;
	// addi r31,r11,4
	ctx.r31.s64 = ctx.r11.s64 + 4;
loc_829EBA58:
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// li r5,5
	ctx.r5.s64 = 5;
	// addi r6,r11,2744
	ctx.r6.s64 = ctx.r11.s64 + 2744;
	// li r4,64
	ctx.r4.s64 = 64;
	// addi r3,r31,12
	ctx.r3.s64 = ctx.r31.s64 + 12;
	// bl 0x82120220
	ctx.lr = 0x829EBA70;
	sub_82120220(ctx, base);
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
	// bge cr6,0x829eba58
	if (!ctx.cr6.lt) goto loc_829EBA58;
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EBA90);  /* forward decl */
PPC_WEAK_FUNC(sub_829EBA90) { __imp__sub_829EBA90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EBA90) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32066
	ctx.r11.s64 = -2101477376;
	// addi r31,r11,12896
	ctx.r31.s64 = ctx.r11.s64 + 12896;
	// addi r3,r31,64
	ctx.r3.s64 = ctx.r31.s64 + 64;
	// bl 0x824b86f8
	ctx.lr = 0x829EBAB0;
	sub_824B86F8(ctx, base);
	// addi r3,r31,72
	ctx.r3.s64 = ctx.r31.s64 + 72;
	// bl 0x824b86f8
	ctx.lr = 0x829EBAB8;
	sub_824B86F8(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// addi r3,r31,52
	ctx.r3.s64 = ctx.r31.s64 + 52;
	// stb r11,60(r31)
	PPC_STORE_U8(ctx.r31.u32 + 60, ctx.r11.u8);
	// bl 0x823e0270
	ctx.lr = 0x829EBAC8;
	sub_823E0270(ctx, base);
	// addi r3,r31,80
	ctx.r3.s64 = ctx.r31.s64 + 80;
	// bl 0x8284f880
	ctx.lr = 0x829EBAD0;
	sub_8284F880(ctx, base);
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
	// bl 0x8222d350
	ctx.lr = 0x829EBAF8;
	sub_8222D350(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-9944
	ctx.r3.s64 = ctx.r11.s64 + -9944;
	// bl 0x8298ed98
	ctx.lr = 0x829EBB04;
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

PPC_FUNC_IMPL(__imp__sub_829EBB18);  /* forward decl */
PPC_WEAK_FUNC(sub_829EBB18) { __imp__sub_829EBB18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EBB18) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x829EBB20;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32066
	ctx.r11.s64 = -2101477376;
	// li r30,1
	ctx.r30.s64 = 1;
	// addi r31,r11,14032
	ctx.r31.s64 = ctx.r11.s64 + 14032;
	// li r29,-1
	ctx.r29.s64 = -1;
loc_829EBB34:
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// li r5,200
	ctx.r5.s64 = 200;
	// addi r6,r11,2744
	ctx.r6.s64 = ctx.r11.s64 + 2744;
	// li r4,16
	ctx.r4.s64 = 16;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82120220
	ctx.lr = 0x829EBB4C;
	sub_82120220(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// stw r29,3444(r31)
	PPC_STORE_U32(ctx.r31.u32 + 3444, ctx.r29.u32);
	// addi r31,r31,3504
	ctx.r31.s64 = ctx.r31.s64 + 3504;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829ebb34
	if (!ctx.cr6.lt) goto loc_829EBB34;
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EBB68);  /* forward decl */
PPC_WEAK_FUNC(sub_829EBB68) { __imp__sub_829EBB68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EBB68) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32066
	ctx.r11.s64 = -2101477376;
	// li r10,215
	ctx.r10.s64 = 215;
	// addi r11,r11,22264
	ctx.r11.s64 = ctx.r11.s64 + 22264;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r11,r11,326
	ctx.r11.s64 = ctx.r11.s64 + 326;
loc_829EBB7C:
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
	// bge cr6,0x829ebb7c
	if (!ctx.cr6.lt) goto loc_829EBB7C;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-9928
	ctx.r3.s64 = ctx.r11.s64 + -9928;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EBBA8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EBBA8) { __imp__sub_829EBBA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EBBA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32066
	ctx.r11.s64 = -2101477376;
	// lis r10,-32066
	ctx.r10.s64 = -2101477376;
	// lbz r11,22256(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 22256);
	// stb r11,22257(r10)
	PPC_STORE_U8(ctx.r10.u32 + 22257, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EBBC0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EBBC0) { __imp__sub_829EBBC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EBBC0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32066
	ctx.r11.s64 = -2101477376;
	// lis r10,-32066
	ctx.r10.s64 = -2101477376;
	// lbz r11,22256(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 22256);
	// stb r11,22258(r10)
	PPC_STORE_U8(ctx.r10.u32 + 22258, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EBBD8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EBBD8) { __imp__sub_829EBBD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EBBD8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// lfs f0,-21608(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -21608);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// fsqrts f13,f0
	ctx.f13.f64 = double(float(sqrt(ctx.f0.f64)));
	// lfs f0,10900(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 10900);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32066
	ctx.r11.s64 = -2101477376;
	// fmuls f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
	// stfs f0,28876(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 28876, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EBC00);  /* forward decl */
PPC_WEAK_FUNC(sub_829EBC00) { __imp__sub_829EBC00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EBC00) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// lfs f0,-21608(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -21608);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// fsqrts f13,f0
	ctx.f13.f64 = double(float(sqrt(ctx.f0.f64)));
	// lfs f0,10900(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 10900);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32066
	ctx.r11.s64 = -2101477376;
	// fmuls f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
	// stfs f0,30928(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 30928, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EBC28);  /* forward decl */
PPC_WEAK_FUNC(sub_829EBC28) { __imp__sub_829EBC28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EBC28) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x829EBC30;
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
	// bl 0x8218be28
	ctx.lr = 0x829EBC40;
	sub_8218BE28(ctx, base);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// li r29,15
	ctx.r29.s64 = 15;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// lfs f31,2592(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2592);
	ctx.f31.f64 = double(temp.f32);
	// beq cr6,0x829ebc80
	if (ctx.cr6.eq) goto loc_829EBC80;
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// stfs f31,0(r30)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r30.u32 + 0, temp.u32);
	// addi r3,r30,8
	ctx.r3.s64 = ctx.r30.s64 + 8;
	// stw r29,4(r30)
	PPC_STORE_U32(ctx.r30.u32 + 4, ctx.r29.u32);
	// addi r4,r11,-21360
	ctx.r4.s64 = ctx.r11.s64 + -21360;
	// bl 0x827f9f70
	ctx.lr = 0x829EBC70;
	sub_827F9F70(ctx, base);
	// lis r11,-32066
	ctx.r11.s64 = -2101477376;
	// addi r31,r11,30940
	ctx.r31.s64 = ctx.r11.s64 + 30940;
	// stw r30,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r30.u32);
	// b 0x829ebc90
	goto loc_829EBC90;
loc_829EBC80:
	// lis r11,-32066
	ctx.r11.s64 = -2101477376;
	// addi r31,r11,30940
	ctx.r31.s64 = ctx.r11.s64 + 30940;
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
loc_829EBC90:
	// li r3,48
	ctx.r3.s64 = 48;
	// bl 0x8218be28
	ctx.lr = 0x829EBC98;
	sub_8218BE28(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// beq cr6,0x829ebcc4
	if (ctx.cr6.eq) goto loc_829EBCC4;
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
	// addi r4,r11,-21380
	ctx.r4.s64 = ctx.r11.s64 + -21380;
	// bl 0x827f9f70
	ctx.lr = 0x829EBCBC;
	sub_827F9F70(ctx, base);
	// stw r30,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r30.u32);
	// b 0x829ebccc
	goto loc_829EBCCC;
loc_829EBCC4:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r11.u32);
loc_829EBCCC:
	// li r3,48
	ctx.r3.s64 = 48;
	// bl 0x8218be28
	ctx.lr = 0x829EBCD4;
	sub_8218BE28(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// li r28,2
	ctx.r28.s64 = 2;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// beq cr6,0x829ebd04
	if (ctx.cr6.eq) goto loc_829EBD04;
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
	// addi r4,r11,-21404
	ctx.r4.s64 = ctx.r11.s64 + -21404;
	// bl 0x827f9f70
	ctx.lr = 0x829EBCFC;
	sub_827F9F70(ctx, base);
	// stw r30,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r30.u32);
	// b 0x829ebd0c
	goto loc_829EBD0C;
loc_829EBD04:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r11.u32);
loc_829EBD0C:
	// li r3,48
	ctx.r3.s64 = 48;
	// bl 0x8218be28
	ctx.lr = 0x829EBD14;
	sub_8218BE28(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// li r27,10
	ctx.r27.s64 = 10;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// beq cr6,0x829ebd44
	if (ctx.cr6.eq) goto loc_829EBD44;
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
	// addi r4,r11,-21420
	ctx.r4.s64 = ctx.r11.s64 + -21420;
	// bl 0x827f9f70
	ctx.lr = 0x829EBD3C;
	sub_827F9F70(ctx, base);
	// stw r30,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r30.u32);
	// b 0x829ebd4c
	goto loc_829EBD4C;
loc_829EBD44:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r11.u32);
loc_829EBD4C:
	// li r3,48
	ctx.r3.s64 = 48;
	// bl 0x8218be28
	ctx.lr = 0x829EBD54;
	sub_8218BE28(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// beq cr6,0x829ebd80
	if (ctx.cr6.eq) goto loc_829EBD80;
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
	// addi r4,r11,-21448
	ctx.r4.s64 = ctx.r11.s64 + -21448;
	// bl 0x827f9f70
	ctx.lr = 0x829EBD78;
	sub_827F9F70(ctx, base);
	// stw r30,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r30.u32);
	// b 0x829ebd88
	goto loc_829EBD88;
loc_829EBD80:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r11.u32);
loc_829EBD88:
	// li r3,48
	ctx.r3.s64 = 48;
	// bl 0x8218be28
	ctx.lr = 0x829EBD90;
	sub_8218BE28(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// beq cr6,0x829ebdbc
	if (ctx.cr6.eq) goto loc_829EBDBC;
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
	// addi r4,r11,-21476
	ctx.r4.s64 = ctx.r11.s64 + -21476;
	// bl 0x827f9f70
	ctx.lr = 0x829EBDB4;
	sub_827F9F70(ctx, base);
	// stw r30,20(r31)
	PPC_STORE_U32(ctx.r31.u32 + 20, ctx.r30.u32);
	// b 0x829ebdc4
	goto loc_829EBDC4;
loc_829EBDBC:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,20(r31)
	PPC_STORE_U32(ctx.r31.u32 + 20, ctx.r11.u32);
loc_829EBDC4:
	// li r3,48
	ctx.r3.s64 = 48;
	// bl 0x8218be28
	ctx.lr = 0x829EBDCC;
	sub_8218BE28(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// beq cr6,0x829ebdfc
	if (ctx.cr6.eq) goto loc_829EBDFC;
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
	// addi r4,r11,-21508
	ctx.r4.s64 = ctx.r11.s64 + -21508;
	// stw r10,4(r30)
	PPC_STORE_U32(ctx.r30.u32 + 4, ctx.r10.u32);
	// bl 0x827f9f70
	ctx.lr = 0x829EBDF4;
	sub_827F9F70(ctx, base);
	// stw r30,24(r31)
	PPC_STORE_U32(ctx.r31.u32 + 24, ctx.r30.u32);
	// b 0x829ebe04
	goto loc_829EBE04;
loc_829EBDFC:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,24(r31)
	PPC_STORE_U32(ctx.r31.u32 + 24, ctx.r11.u32);
loc_829EBE04:
	// li r3,48
	ctx.r3.s64 = 48;
	// bl 0x8218be28
	ctx.lr = 0x829EBE0C;
	sub_8218BE28(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// beq cr6,0x829ebe3c
	if (ctx.cr6.eq) goto loc_829EBE3C;
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
	// addi r4,r11,-21528
	ctx.r4.s64 = ctx.r11.s64 + -21528;
	// stw r10,4(r30)
	PPC_STORE_U32(ctx.r30.u32 + 4, ctx.r10.u32);
	// bl 0x827f9f70
	ctx.lr = 0x829EBE34;
	sub_827F9F70(ctx, base);
	// stw r30,28(r31)
	PPC_STORE_U32(ctx.r31.u32 + 28, ctx.r30.u32);
	// b 0x829ebe44
	goto loc_829EBE44;
loc_829EBE3C:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,28(r31)
	PPC_STORE_U32(ctx.r31.u32 + 28, ctx.r11.u32);
loc_829EBE44:
	// li r3,48
	ctx.r3.s64 = 48;
	// bl 0x8218be28
	ctx.lr = 0x829EBE4C;
	sub_8218BE28(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// beq cr6,0x829ebe78
	if (ctx.cr6.eq) goto loc_829EBE78;
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
	// addi r4,r11,-21544
	ctx.r4.s64 = ctx.r11.s64 + -21544;
	// bl 0x827f9f70
	ctx.lr = 0x829EBE70;
	sub_827F9F70(ctx, base);
	// stw r30,32(r31)
	PPC_STORE_U32(ctx.r31.u32 + 32, ctx.r30.u32);
	// b 0x829ebe80
	goto loc_829EBE80;
loc_829EBE78:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,32(r31)
	PPC_STORE_U32(ctx.r31.u32 + 32, ctx.r11.u32);
loc_829EBE80:
	// li r3,48
	ctx.r3.s64 = 48;
	// bl 0x8218be28
	ctx.lr = 0x829EBE88;
	sub_8218BE28(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// beq cr6,0x829ebeb4
	if (ctx.cr6.eq) goto loc_829EBEB4;
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
	// addi r4,r11,-21564
	ctx.r4.s64 = ctx.r11.s64 + -21564;
	// bl 0x827f9f70
	ctx.lr = 0x829EBEAC;
	sub_827F9F70(ctx, base);
	// stw r30,36(r31)
	PPC_STORE_U32(ctx.r31.u32 + 36, ctx.r30.u32);
	// b 0x829ebebc
	goto loc_829EBEBC;
loc_829EBEB4:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,36(r31)
	PPC_STORE_U32(ctx.r31.u32 + 36, ctx.r11.u32);
loc_829EBEBC:
	// li r3,48
	ctx.r3.s64 = 48;
	// bl 0x8218be28
	ctx.lr = 0x829EBEC4;
	sub_8218BE28(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// beq cr6,0x829ebef4
	if (ctx.cr6.eq) goto loc_829EBEF4;
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
	// addi r4,r11,-21580
	ctx.r4.s64 = ctx.r11.s64 + -21580;
	// stw r10,4(r30)
	PPC_STORE_U32(ctx.r30.u32 + 4, ctx.r10.u32);
	// bl 0x827f9f70
	ctx.lr = 0x829EBEEC;
	sub_827F9F70(ctx, base);
	// stw r30,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r30.u32);
	// b 0x829ebefc
	goto loc_829EBEFC;
loc_829EBEF4:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
loc_829EBEFC:
	// li r3,48
	ctx.r3.s64 = 48;
	// bl 0x8218be28
	ctx.lr = 0x829EBF04;
	sub_8218BE28(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// beq cr6,0x829ebf38
	if (ctx.cr6.eq) goto loc_829EBF38;
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
	// addi r4,r11,-21604
	ctx.r4.s64 = ctx.r11.s64 + -21604;
	// bl 0x827f9f70
	ctx.lr = 0x829EBF28;
	sub_827F9F70(ctx, base);
	// stw r30,44(r31)
	PPC_STORE_U32(ctx.r31.u32 + 44, ctx.r30.u32);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lfd f31,-56(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -56);
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_829EBF38:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,44(r31)
	PPC_STORE_U32(ctx.r31.u32 + 44, ctx.r11.u32);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lfd f31,-56(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -56);
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EBF50);  /* forward decl */
PPC_WEAK_FUNC(sub_829EBF50) { __imp__sub_829EBF50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EBF50) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-9912
	ctx.r3.s64 = ctx.r11.s64 + -9912;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EBF60);  /* forward decl */
PPC_WEAK_FUNC(sub_829EBF60) { __imp__sub_829EBF60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EBF60) {
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
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// li r31,39
	ctx.r31.s64 = 39;
	// addi r11,r11,-20940
	ctx.r11.s64 = ctx.r11.s64 + -20940;
	// addi r30,r11,92
	ctx.r30.s64 = ctx.r11.s64 + 92;
loc_829EBF84:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82614210
	ctx.lr = 0x829EBF8C;
	sub_82614210(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,108
	ctx.r30.s64 = ctx.r30.s64 + 108;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x829ebf84
	if (!ctx.cr6.lt) goto loc_829EBF84;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-9880
	ctx.r3.s64 = ctx.r11.s64 + -9880;
	// bl 0x8298ed98
	ctx.lr = 0x829EBFA8;
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

PPC_FUNC_IMPL(__imp__sub_829EBFC0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EBFC0) { __imp__sub_829EBFC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EBFC0) {
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
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// li r31,19
	ctx.r31.s64 = 19;
	// addi r30,r11,-16616
	ctx.r30.s64 = ctx.r11.s64 + -16616;
loc_829EBFE0:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826141f0
	ctx.lr = 0x829EBFE8;
	sub_826141F0(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,72
	ctx.r30.s64 = ctx.r30.s64 + 72;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x829ebfe0
	if (!ctx.cr6.lt) goto loc_829EBFE0;
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

PPC_FUNC_IMPL(__imp__sub_829EC010);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC010) { __imp__sub_829EC010(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC010) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// li r9,10
	ctx.r9.s64 = 10;
	// addi r11,r11,-14232
	ctx.r11.s64 = ctx.r11.s64 + -14232;
	// li r10,-1
	ctx.r10.s64 = -1;
	// addi r11,r11,44
	ctx.r11.s64 = ctx.r11.s64 + 44;
loc_829EC024:
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
	// bge cr6,0x829ec024
	if (!ctx.cr6.lt) goto loc_829EC024;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC040);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC040) { __imp__sub_829EC040(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC040) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,-13688
	ctx.r11.s64 = ctx.r11.s64 + -13688;
	// lis r9,-32096
	ctx.r9.s64 = -2103443456;
	// addi r3,r9,-9808
	ctx.r3.s64 = ctx.r9.s64 + -9808;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// stw r10,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
	// stw r10,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r10.u32);
	// stw r10,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r10.u32);
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC068);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC068) { __imp__sub_829EC068(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC068) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-9720
	ctx.r3.s64 = ctx.r11.s64 + -9720;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC078);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC078) { __imp__sub_829EC078(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC078) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-20340
	ctx.r5.s64 = ctx.r11.s64 + -20340;
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-13668
	ctx.r3.s64 = ctx.r11.s64 + -13668;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC098);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC098) { __imp__sub_829EC098(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC098) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r31,r11,-28104
	ctx.r31.s64 = ctx.r11.s64 + -28104;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82893a58
	ctx.lr = 0x829EC0B8;
	sub_82893A58(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// lis r10,-32255
	ctx.r10.s64 = -2113863680;
	// addi r11,r11,-19572
	ctx.r11.s64 = ctx.r11.s64 + -19572;
	// addi r10,r10,-19644
	ctx.r10.s64 = ctx.r10.s64 + -19644;
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
	// bl 0x828a2de0
	ctx.lr = 0x829EC0E0;
	sub_828A2DE0(ctx, base);
	// addi r3,r31,76
	ctx.r3.s64 = ctx.r31.s64 + 76;
	// bl 0x828a2de0
	ctx.lr = 0x829EC0E8;
	sub_828A2DE0(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-9704
	ctx.r3.s64 = ctx.r11.s64 + -9704;
	// bl 0x8298ed98
	ctx.lr = 0x829EC0F4;
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

PPC_FUNC_IMPL(__imp__sub_829EC108);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC108) { __imp__sub_829EC108(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC108) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-19440
	ctx.r3.s64 = ctx.r11.s64 + -19440;
	// bl 0x827df490
	ctx.lr = 0x829EC124;
	sub_827DF490(ctx, base);
	// lis r11,-32062
	ctx.r11.s64 = -2101215232;
	// stw r3,-27968(r11)
	PPC_STORE_U32(ctx.r11.u32 + -27968, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC140);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC140) { __imp__sub_829EC140(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC140) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-19432
	ctx.r5.s64 = ctx.r11.s64 + -19432;
	// lis r11,-32062
	ctx.r11.s64 = -2101215232;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-27964
	ctx.r3.s64 = ctx.r11.s64 + -27964;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC160);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC160) { __imp__sub_829EC160(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC160) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x829EC168;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32062
	ctx.r11.s64 = -2101215232;
	// li r30,31
	ctx.r30.s64 = 31;
	// addi r11,r11,-27848
	ctx.r11.s64 = ctx.r11.s64 + -27848;
	// li r29,-1
	ctx.r29.s64 = -1;
	// addi r31,r11,196
	ctx.r31.s64 = ctx.r11.s64 + 196;
loc_829EC180:
	// lis r11,-32238
	ctx.r11.s64 = -2112749568;
	// stw r29,-196(r31)
	PPC_STORE_U32(ctx.r31.u32 + -196, ctx.r29.u32);
	// li r5,7
	ctx.r5.s64 = 7;
	// addi r6,r11,1760
	ctx.r6.s64 = ctx.r11.s64 + 1760;
	// li r4,4
	ctx.r4.s64 = 4;
	// addi r3,r31,-56
	ctx.r3.s64 = ctx.r31.s64 + -56;
	// bl 0x82120220
	ctx.lr = 0x829EC19C;
	sub_82120220(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// li r5,7
	ctx.r5.s64 = 7;
	// addi r6,r11,2744
	ctx.r6.s64 = ctx.r11.s64 + 2744;
	// li r4,8
	ctx.r4.s64 = 8;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82120220
	ctx.lr = 0x829EC1B4;
	sub_82120220(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// addi r31,r31,252
	ctx.r31.s64 = ctx.r31.s64 + 252;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829ec180
	if (!ctx.cr6.lt) goto loc_829EC180;
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC1D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC1D0) { __imp__sub_829EC1D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC1D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-9664
	ctx.r3.s64 = ctx.r11.s64 + -9664;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC1E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC1E0) { __imp__sub_829EC1E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC1E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-9656
	ctx.r3.s64 = ctx.r11.s64 + -9656;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC1F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC1F0) { __imp__sub_829EC1F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC1F0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32062
	ctx.r11.s64 = -2101215232;
	// addi r3,r11,16672
	ctx.r3.s64 = ctx.r11.s64 + 16672;
	// bl 0x823d8b30
	ctx.lr = 0x829EC208;
	sub_823D8B30(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-9648
	ctx.r3.s64 = ctx.r11.s64 + -9648;
	// bl 0x8298ed98
	ctx.lr = 0x829EC214;
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

PPC_FUNC_IMPL(__imp__sub_829EC228);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC228) { __imp__sub_829EC228(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC228) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-18156
	ctx.r5.s64 = ctx.r11.s64 + -18156;
	// lis r11,-32062
	ctx.r11.s64 = -2101215232;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,16968
	ctx.r3.s64 = ctx.r11.s64 + 16968;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC248);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC248) { __imp__sub_829EC248(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC248) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-9632
	ctx.r3.s64 = ctx.r11.s64 + -9632;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC258);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC258) { __imp__sub_829EC258(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC258) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// lfs f0,7384(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 7384);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32062
	ctx.r11.s64 = -2101215232;
	// stfs f0,16988(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 16988, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC270);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC270) { __imp__sub_829EC270(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC270) {
	PPC_FUNC_PROLOGUE();
loc_829EC270:
	// mftb r11
	ctx.r11.u64 = PPC_QUERY_TIMEBASE();
	// rotlwi r10,r11,0
	ctx.r10.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x829ec270
	if (ctx.cr6.eq) goto loc_829EC270;
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// std r11,16992(r10)
	PPC_STORE_U64(ctx.r10.u32 + 16992, ctx.r11.u64);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC290);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC290) { __imp__sub_829EC290(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC290) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14324
	ctx.r11.s64 = ctx.r11.s64 + -14324;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC2B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC2B0) { __imp__sub_829EC2B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC2B0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14312
	ctx.r11.s64 = ctx.r11.s64 + -14312;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC2D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC2D0) { __imp__sub_829EC2D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC2D0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14300
	ctx.r11.s64 = ctx.r11.s64 + -14300;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC2F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC2F0) { __imp__sub_829EC2F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC2F0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14288
	ctx.r11.s64 = ctx.r11.s64 + -14288;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC310);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC310) { __imp__sub_829EC310(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC310) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14276
	ctx.r11.s64 = ctx.r11.s64 + -14276;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC330);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC330) { __imp__sub_829EC330(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC330) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14264
	ctx.r11.s64 = ctx.r11.s64 + -14264;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC350);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC350) { __imp__sub_829EC350(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC350) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14252
	ctx.r11.s64 = ctx.r11.s64 + -14252;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC370);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC370) { __imp__sub_829EC370(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC370) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14240
	ctx.r11.s64 = ctx.r11.s64 + -14240;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC390);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC390) { __imp__sub_829EC390(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC390) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14228
	ctx.r11.s64 = ctx.r11.s64 + -14228;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC3B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC3B0) { __imp__sub_829EC3B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC3B0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14216
	ctx.r11.s64 = ctx.r11.s64 + -14216;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC3D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC3D0) { __imp__sub_829EC3D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC3D0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14204
	ctx.r11.s64 = ctx.r11.s64 + -14204;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC3F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC3F0) { __imp__sub_829EC3F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC3F0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14192
	ctx.r11.s64 = ctx.r11.s64 + -14192;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC410);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC410) { __imp__sub_829EC410(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC410) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14180
	ctx.r11.s64 = ctx.r11.s64 + -14180;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC430);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC430) { __imp__sub_829EC430(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC430) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14168
	ctx.r11.s64 = ctx.r11.s64 + -14168;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC450);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC450) { __imp__sub_829EC450(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC450) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14156
	ctx.r11.s64 = ctx.r11.s64 + -14156;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC470);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC470) { __imp__sub_829EC470(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC470) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14144
	ctx.r11.s64 = ctx.r11.s64 + -14144;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC490);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC490) { __imp__sub_829EC490(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC490) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14132
	ctx.r11.s64 = ctx.r11.s64 + -14132;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC4B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC4B0) { __imp__sub_829EC4B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC4B0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14120
	ctx.r11.s64 = ctx.r11.s64 + -14120;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC4D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC4D0) { __imp__sub_829EC4D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC4D0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14108
	ctx.r11.s64 = ctx.r11.s64 + -14108;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC4F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC4F0) { __imp__sub_829EC4F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC4F0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14096
	ctx.r11.s64 = ctx.r11.s64 + -14096;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC510);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC510) { __imp__sub_829EC510(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC510) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14084
	ctx.r11.s64 = ctx.r11.s64 + -14084;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC530);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC530) { __imp__sub_829EC530(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC530) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14072
	ctx.r11.s64 = ctx.r11.s64 + -14072;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC550);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC550) { __imp__sub_829EC550(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC550) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14060
	ctx.r11.s64 = ctx.r11.s64 + -14060;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC570);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC570) { __imp__sub_829EC570(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC570) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14048
	ctx.r11.s64 = ctx.r11.s64 + -14048;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC590);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC590) { __imp__sub_829EC590(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC590) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14036
	ctx.r11.s64 = ctx.r11.s64 + -14036;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC5B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC5B0) { __imp__sub_829EC5B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC5B0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14024
	ctx.r11.s64 = ctx.r11.s64 + -14024;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC5D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC5D0) { __imp__sub_829EC5D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC5D0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14012
	ctx.r11.s64 = ctx.r11.s64 + -14012;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC5F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC5F0) { __imp__sub_829EC5F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC5F0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-14000
	ctx.r11.s64 = ctx.r11.s64 + -14000;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC610);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC610) { __imp__sub_829EC610(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC610) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13988
	ctx.r11.s64 = ctx.r11.s64 + -13988;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC630);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC630) { __imp__sub_829EC630(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC630) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13976
	ctx.r11.s64 = ctx.r11.s64 + -13976;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC650);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC650) { __imp__sub_829EC650(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC650) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13964
	ctx.r11.s64 = ctx.r11.s64 + -13964;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC670);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC670) { __imp__sub_829EC670(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC670) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13952
	ctx.r11.s64 = ctx.r11.s64 + -13952;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC690);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC690) { __imp__sub_829EC690(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC690) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13940
	ctx.r11.s64 = ctx.r11.s64 + -13940;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC6B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC6B0) { __imp__sub_829EC6B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC6B0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13928
	ctx.r11.s64 = ctx.r11.s64 + -13928;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC6D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC6D0) { __imp__sub_829EC6D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC6D0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13916
	ctx.r11.s64 = ctx.r11.s64 + -13916;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC6F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC6F0) { __imp__sub_829EC6F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC6F0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13904
	ctx.r11.s64 = ctx.r11.s64 + -13904;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC710);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC710) { __imp__sub_829EC710(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC710) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13892
	ctx.r11.s64 = ctx.r11.s64 + -13892;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC730);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC730) { __imp__sub_829EC730(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC730) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13880
	ctx.r11.s64 = ctx.r11.s64 + -13880;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC750);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC750) { __imp__sub_829EC750(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC750) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13868
	ctx.r11.s64 = ctx.r11.s64 + -13868;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC770);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC770) { __imp__sub_829EC770(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC770) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13856
	ctx.r11.s64 = ctx.r11.s64 + -13856;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC790);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC790) { __imp__sub_829EC790(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC790) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13844
	ctx.r11.s64 = ctx.r11.s64 + -13844;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC7B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC7B0) { __imp__sub_829EC7B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC7B0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13832
	ctx.r11.s64 = ctx.r11.s64 + -13832;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC7D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC7D0) { __imp__sub_829EC7D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC7D0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13820
	ctx.r11.s64 = ctx.r11.s64 + -13820;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC7F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC7F0) { __imp__sub_829EC7F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC7F0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13808
	ctx.r11.s64 = ctx.r11.s64 + -13808;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC810);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC810) { __imp__sub_829EC810(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC810) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13796
	ctx.r11.s64 = ctx.r11.s64 + -13796;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC830);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC830) { __imp__sub_829EC830(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC830) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13784
	ctx.r11.s64 = ctx.r11.s64 + -13784;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC850);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC850) { __imp__sub_829EC850(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC850) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13772
	ctx.r11.s64 = ctx.r11.s64 + -13772;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC870);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC870) { __imp__sub_829EC870(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC870) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13760
	ctx.r11.s64 = ctx.r11.s64 + -13760;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC890);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC890) { __imp__sub_829EC890(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC890) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13748
	ctx.r11.s64 = ctx.r11.s64 + -13748;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC8B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC8B0) { __imp__sub_829EC8B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC8B0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13736
	ctx.r11.s64 = ctx.r11.s64 + -13736;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC8D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC8D0) { __imp__sub_829EC8D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC8D0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13724
	ctx.r11.s64 = ctx.r11.s64 + -13724;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC8F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC8F0) { __imp__sub_829EC8F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC8F0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13712
	ctx.r11.s64 = ctx.r11.s64 + -13712;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC910);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC910) { __imp__sub_829EC910(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC910) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13700
	ctx.r11.s64 = ctx.r11.s64 + -13700;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC930);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC930) { __imp__sub_829EC930(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC930) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13688
	ctx.r11.s64 = ctx.r11.s64 + -13688;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC950);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC950) { __imp__sub_829EC950(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC950) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13676
	ctx.r11.s64 = ctx.r11.s64 + -13676;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC970);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC970) { __imp__sub_829EC970(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC970) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13664
	ctx.r11.s64 = ctx.r11.s64 + -13664;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC990);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC990) { __imp__sub_829EC990(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC990) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13652
	ctx.r11.s64 = ctx.r11.s64 + -13652;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC9B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC9B0) { __imp__sub_829EC9B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC9B0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13640
	ctx.r11.s64 = ctx.r11.s64 + -13640;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC9D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC9D0) { __imp__sub_829EC9D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC9D0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13628
	ctx.r11.s64 = ctx.r11.s64 + -13628;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EC9F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EC9F0) { __imp__sub_829EC9F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EC9F0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13616
	ctx.r11.s64 = ctx.r11.s64 + -13616;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECA10);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECA10) { __imp__sub_829ECA10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECA10) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13604
	ctx.r11.s64 = ctx.r11.s64 + -13604;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECA30);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECA30) { __imp__sub_829ECA30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECA30) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13592
	ctx.r11.s64 = ctx.r11.s64 + -13592;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECA50);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECA50) { __imp__sub_829ECA50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECA50) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13580
	ctx.r11.s64 = ctx.r11.s64 + -13580;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECA70);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECA70) { __imp__sub_829ECA70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECA70) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13568
	ctx.r11.s64 = ctx.r11.s64 + -13568;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECA90);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECA90) { __imp__sub_829ECA90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECA90) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13556
	ctx.r11.s64 = ctx.r11.s64 + -13556;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECAB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECAB0) { __imp__sub_829ECAB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECAB0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13544
	ctx.r11.s64 = ctx.r11.s64 + -13544;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECAD0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECAD0) { __imp__sub_829ECAD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECAD0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13532
	ctx.r11.s64 = ctx.r11.s64 + -13532;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECAF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECAF0) { __imp__sub_829ECAF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECAF0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13520
	ctx.r11.s64 = ctx.r11.s64 + -13520;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECB10);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECB10) { __imp__sub_829ECB10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECB10) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13508
	ctx.r11.s64 = ctx.r11.s64 + -13508;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECB30);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECB30) { __imp__sub_829ECB30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECB30) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13496
	ctx.r11.s64 = ctx.r11.s64 + -13496;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECB50);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECB50) { __imp__sub_829ECB50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECB50) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13484
	ctx.r11.s64 = ctx.r11.s64 + -13484;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECB70);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECB70) { __imp__sub_829ECB70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECB70) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13472
	ctx.r11.s64 = ctx.r11.s64 + -13472;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECB90);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECB90) { __imp__sub_829ECB90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECB90) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13460
	ctx.r11.s64 = ctx.r11.s64 + -13460;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECBB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECBB0) { __imp__sub_829ECBB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECBB0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13448
	ctx.r11.s64 = ctx.r11.s64 + -13448;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECBD0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECBD0) { __imp__sub_829ECBD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECBD0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13436
	ctx.r11.s64 = ctx.r11.s64 + -13436;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECBF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECBF0) { __imp__sub_829ECBF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECBF0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13424
	ctx.r11.s64 = ctx.r11.s64 + -13424;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECC10);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECC10) { __imp__sub_829ECC10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECC10) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13412
	ctx.r11.s64 = ctx.r11.s64 + -13412;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECC30);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECC30) { __imp__sub_829ECC30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECC30) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13400
	ctx.r11.s64 = ctx.r11.s64 + -13400;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECC50);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECC50) { __imp__sub_829ECC50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECC50) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13388
	ctx.r11.s64 = ctx.r11.s64 + -13388;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECC70);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECC70) { __imp__sub_829ECC70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECC70) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13376
	ctx.r11.s64 = ctx.r11.s64 + -13376;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECC90);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECC90) { __imp__sub_829ECC90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECC90) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13364
	ctx.r11.s64 = ctx.r11.s64 + -13364;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECCB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECCB0) { __imp__sub_829ECCB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECCB0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13352
	ctx.r11.s64 = ctx.r11.s64 + -13352;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECCD0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECCD0) { __imp__sub_829ECCD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECCD0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13340
	ctx.r11.s64 = ctx.r11.s64 + -13340;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECCF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECCF0) { __imp__sub_829ECCF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECCF0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13328
	ctx.r11.s64 = ctx.r11.s64 + -13328;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECD10);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECD10) { __imp__sub_829ECD10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECD10) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13316
	ctx.r11.s64 = ctx.r11.s64 + -13316;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECD30);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECD30) { __imp__sub_829ECD30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECD30) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13304
	ctx.r11.s64 = ctx.r11.s64 + -13304;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECD50);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECD50) { __imp__sub_829ECD50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECD50) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13292
	ctx.r11.s64 = ctx.r11.s64 + -13292;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECD70);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECD70) { __imp__sub_829ECD70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECD70) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13280
	ctx.r11.s64 = ctx.r11.s64 + -13280;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECD90);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECD90) { __imp__sub_829ECD90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECD90) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13268
	ctx.r11.s64 = ctx.r11.s64 + -13268;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECDB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECDB0) { __imp__sub_829ECDB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECDB0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13256
	ctx.r11.s64 = ctx.r11.s64 + -13256;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECDD0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECDD0) { __imp__sub_829ECDD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECDD0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13244
	ctx.r11.s64 = ctx.r11.s64 + -13244;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECDF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECDF0) { __imp__sub_829ECDF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECDF0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13232
	ctx.r11.s64 = ctx.r11.s64 + -13232;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECE10);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECE10) { __imp__sub_829ECE10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECE10) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13220
	ctx.r11.s64 = ctx.r11.s64 + -13220;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECE30);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECE30) { __imp__sub_829ECE30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECE30) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13208
	ctx.r11.s64 = ctx.r11.s64 + -13208;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECE50);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECE50) { __imp__sub_829ECE50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECE50) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13196
	ctx.r11.s64 = ctx.r11.s64 + -13196;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECE70);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECE70) { __imp__sub_829ECE70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECE70) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13184
	ctx.r11.s64 = ctx.r11.s64 + -13184;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECE90);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECE90) { __imp__sub_829ECE90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECE90) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13172
	ctx.r11.s64 = ctx.r11.s64 + -13172;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECEB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECEB0) { __imp__sub_829ECEB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECEB0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13160
	ctx.r11.s64 = ctx.r11.s64 + -13160;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECED0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECED0) { __imp__sub_829ECED0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECED0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13148
	ctx.r11.s64 = ctx.r11.s64 + -13148;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECEF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECEF0) { __imp__sub_829ECEF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECEF0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13136
	ctx.r11.s64 = ctx.r11.s64 + -13136;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECF10);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECF10) { __imp__sub_829ECF10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECF10) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13124
	ctx.r11.s64 = ctx.r11.s64 + -13124;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECF30);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECF30) { __imp__sub_829ECF30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECF30) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13112
	ctx.r11.s64 = ctx.r11.s64 + -13112;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECF50);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECF50) { __imp__sub_829ECF50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECF50) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13100
	ctx.r11.s64 = ctx.r11.s64 + -13100;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECF70);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECF70) { __imp__sub_829ECF70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECF70) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13088
	ctx.r11.s64 = ctx.r11.s64 + -13088;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECF90);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECF90) { __imp__sub_829ECF90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECF90) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13076
	ctx.r11.s64 = ctx.r11.s64 + -13076;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECFB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECFB0) { __imp__sub_829ECFB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECFB0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13064
	ctx.r11.s64 = ctx.r11.s64 + -13064;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECFD0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECFD0) { __imp__sub_829ECFD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECFD0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13052
	ctx.r11.s64 = ctx.r11.s64 + -13052;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ECFF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ECFF0) { __imp__sub_829ECFF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ECFF0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13040
	ctx.r11.s64 = ctx.r11.s64 + -13040;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED010);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED010) { __imp__sub_829ED010(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED010) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13028
	ctx.r11.s64 = ctx.r11.s64 + -13028;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED030);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED030) { __imp__sub_829ED030(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED030) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13016
	ctx.r11.s64 = ctx.r11.s64 + -13016;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED050);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED050) { __imp__sub_829ED050(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED050) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-13004
	ctx.r11.s64 = ctx.r11.s64 + -13004;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED070);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED070) { __imp__sub_829ED070(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED070) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-12992
	ctx.r11.s64 = ctx.r11.s64 + -12992;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED090);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED090) { __imp__sub_829ED090(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED090) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-12980
	ctx.r11.s64 = ctx.r11.s64 + -12980;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED0B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED0B0) { __imp__sub_829ED0B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED0B0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-12968
	ctx.r11.s64 = ctx.r11.s64 + -12968;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED0D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED0D0) { __imp__sub_829ED0D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED0D0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32062
	ctx.r10.s64 = -2101215232;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-12956
	ctx.r11.s64 = ctx.r11.s64 + -12956;
	// lwz r9,17000(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 17000);
	// stw r11,17000(r10)
	PPC_STORE_U32(ctx.r10.u32 + 17000, ctx.r11.u32);
	// stw r9,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED0F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED0F0) { __imp__sub_829ED0F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED0F0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lis r11,-32062
	ctx.r11.s64 = -2101215232;
	// lis r7,-32256
	ctx.r7.s64 = -2113929216;
	// lis r8,-32256
	ctx.r8.s64 = -2113929216;
	// lis r9,-32255
	ctx.r9.s64 = -2113863680;
	// addi r11,r11,17024
	ctx.r11.s64 = ctx.r11.s64 + 17024;
	// lfs f0,3068(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 3068);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,-16(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -16, temp.u32);
	// li r6,159
	ctx.r6.s64 = 159;
	// lfs f12,10260(r7)
	temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + 10260);
	ctx.f12.f64 = double(temp.f32);
	// addi r11,r11,20
	ctx.r11.s64 = ctx.r11.s64 + 20;
	// lfs f13,2592(r8)
	temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + 2592);
	ctx.f13.f64 = double(temp.f32);
	// li r5,12
	ctx.r5.s64 = 12;
	// lfs f11,-16824(r9)
	temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + -16824);
	ctx.f11.f64 = double(temp.f32);
	// li r10,0
	ctx.r10.s64 = 0;
	// li r4,10000
	ctx.r4.s64 = 10000;
loc_829ED130:
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
loc_829ED178:
	// stw r10,0(r9)
	PPC_STORE_U32(ctx.r9.u32 + 0, ctx.r10.u32);
	// addi r9,r9,4
	ctx.r9.s64 = ctx.r9.s64 + 4;
	// bdnz 0x829ed178
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_829ED178;
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
	// bge cr6,0x829ed130
	if (!ctx.cr6.lt) goto loc_829ED130;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED220);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED220) { __imp__sub_829ED220(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED220) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// lis r8,-32256
	ctx.r8.s64 = -2113929216;
	// lis r9,-32255
	ctx.r9.s64 = -2113863680;
	// addi r11,r11,-4992
	ctx.r11.s64 = ctx.r11.s64 + -4992;
	// li r10,39
	ctx.r10.s64 = 39;
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// lfs f0,3068(r8)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + 3068);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,-16824(r9)
	temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + -16824);
	ctx.f13.f64 = double(temp.f32);
loc_829ED240:
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
	// bge cr6,0x829ed240
	if (!ctx.cr6.lt) goto loc_829ED240;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED298);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED298) { __imp__sub_829ED298(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED298) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// lis r8,-32255
	ctx.r8.s64 = -2113863680;
	// addi r11,r11,-2432
	ctx.r11.s64 = ctx.r11.s64 + -2432;
	// li r9,56
	ctx.r9.s64 = 56;
	// lfs f0,3068(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 3068);
	ctx.f0.f64 = double(temp.f32);
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// stfs f0,-16(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -16, temp.u32);
	// li r10,39
	ctx.r10.s64 = 39;
	// lfs f13,-16824(r8)
	temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + -16824);
	ctx.f13.f64 = double(temp.f32);
loc_829ED2C0:
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
	// bge cr6,0x829ed2c0
	if (!ctx.cr6.lt) goto loc_829ED2C0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED328);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED328) { __imp__sub_829ED328(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED328) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// lis r7,-32256
	ctx.r7.s64 = -2113929216;
	// lis r8,-32252
	ctx.r8.s64 = -2113667072;
	// lis r9,-32255
	ctx.r9.s64 = -2113863680;
	// addi r11,r11,1408
	ctx.r11.s64 = ctx.r11.s64 + 1408;
	// lfs f0,3068(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 3068);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,-16(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -16, temp.u32);
	// li r10,39
	ctx.r10.s64 = 39;
	// lfs f11,2980(r7)
	temp.u32 = PPC_LOAD_U32(ctx.r7.u32 + 2980);
	ctx.f11.f64 = double(temp.f32);
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// lfs f12,10508(r8)
	temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + 10508);
	ctx.f12.f64 = double(temp.f32);
	// li r4,40
	ctx.r4.s64 = 40;
	// lfs f13,-16824(r9)
	temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + -16824);
	ctx.f13.f64 = double(temp.f32);
	// li r5,56
	ctx.r5.s64 = 56;
	// stfs f0,-12(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -12, temp.u32);
	// li r6,0
	ctx.r6.s64 = 0;
loc_829ED36C:
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
	// bge cr6,0x829ed36c
	if (!ctx.cr6.lt) goto loc_829ED36C;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED3E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED3E0) { __imp__sub_829ED3E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED3E0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,12544
	ctx.r3.s64 = ctx.r11.s64 + 12544;
	// bl 0x82273228
	ctx.lr = 0x829ED3F8;
	sub_82273228(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-9624
	ctx.r3.s64 = ctx.r11.s64 + -9624;
	// bl 0x8298ed98
	ctx.lr = 0x829ED404;
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

PPC_FUNC_IMPL(__imp__sub_829ED418);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED418) { __imp__sub_829ED418(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED418) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// vspltisw v0,0
	simde_mm_store_si128((simde__m128i*)ctx.v0.u32, simde_mm_set1_epi32(int(0x0)));
	// lis r8,-32256
	ctx.r8.s64 = -2113929216;
	// lis r9,-32056
	ctx.r9.s64 = -2100822016;
	// addi r11,r11,14976
	ctx.r11.s64 = ctx.r11.s64 + 14976;
	// li r7,31
	ctx.r7.s64 = 31;
	// addi r11,r11,64
	ctx.r11.s64 = ctx.r11.s64 + 64;
	// lfs f0,3068(r8)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + 3068);
	ctx.f0.f64 = double(temp.f32);
	// li r10,0
	ctx.r10.s64 = 0;
	// lwz r6,-7564(r9)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r9.u32 + -7564);
	// li r4,-48
	ctx.r4.s64 = -48;
	// li r5,1
	ctx.r5.s64 = 1;
loc_829ED448:
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
	// beq cr6,0x829ed4a4
	if (ctx.cr6.eq) goto loc_829ED4A4;
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
loc_829ED4A4:
	// addi r7,r7,-1
	ctx.r7.s64 = ctx.r7.s64 + -1;
	// stw r10,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
	// addi r11,r11,80
	ctx.r11.s64 = ctx.r11.s64 + 80;
	// cmpwi cr6,r7,0
	ctx.cr6.compare<int32_t>(ctx.r7.s32, 0, ctx.xer);
	// bge cr6,0x829ed448
	if (!ctx.cr6.lt) goto loc_829ED448;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-9608
	ctx.r3.s64 = ctx.r11.s64 + -9608;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED4C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED4C8) { __imp__sub_829ED4C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED4C8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb10
	ctx.lr = 0x829ED4D0;
	__savegprlr_26(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// li r29,30
	ctx.r29.s64 = 30;
	// addi r11,r11,17536
	ctx.r11.s64 = ctx.r11.s64 + 17536;
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
loc_829ED4F4:
	// lis r11,-32217
	ctx.r11.s64 = -2111373312;
	// li r5,4
	ctx.r5.s64 = 4;
	// addi r6,r11,16472
	ctx.r6.s64 = ctx.r11.s64 + 16472;
	// li r4,6
	ctx.r4.s64 = 6;
	// addi r3,r31,24
	ctx.r3.s64 = ctx.r31.s64 + 24;
	// bl 0x82120220
	ctx.lr = 0x829ED50C;
	sub_82120220(ctx, base);
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
	// bge cr6,0x829ed4f4
	if (!ctx.cr6.lt) goto loc_829ED4F4;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED540);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED540) { __imp__sub_829ED540(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED540) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r4,r11,-15220
	ctx.r4.s64 = ctx.r11.s64 + -15220;
	// lis r11,-32059
	ctx.r11.s64 = -2101018624;
	// addi r3,r11,-7568
	ctx.r3.s64 = ctx.r11.s64 + -7568;
	// b 0x8226f518
	sub_8226F518(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED558);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED558) { __imp__sub_829ED558(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED558) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r4,r11,-15208
	ctx.r4.s64 = ctx.r11.s64 + -15208;
	// lis r11,-32059
	ctx.r11.s64 = -2101018624;
	// addi r3,r11,-7556
	ctx.r3.s64 = ctx.r11.s64 + -7556;
	// b 0x8226f518
	sub_8226F518(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED570);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED570) { __imp__sub_829ED570(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED570) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r4,r11,-15196
	ctx.r4.s64 = ctx.r11.s64 + -15196;
	// lis r11,-32059
	ctx.r11.s64 = -2101018624;
	// addi r3,r11,-7544
	ctx.r3.s64 = ctx.r11.s64 + -7544;
	// b 0x8226f518
	sub_8226F518(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED588);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED588) { __imp__sub_829ED588(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED588) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r4,r11,-15180
	ctx.r4.s64 = ctx.r11.s64 + -15180;
	// lis r11,-32059
	ctx.r11.s64 = -2101018624;
	// addi r3,r11,-7532
	ctx.r3.s64 = ctx.r11.s64 + -7532;
	// b 0x8226f518
	sub_8226F518(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED5A0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED5A0) { __imp__sub_829ED5A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED5A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r4,r11,-15160
	ctx.r4.s64 = ctx.r11.s64 + -15160;
	// lis r11,-32059
	ctx.r11.s64 = -2101018624;
	// addi r3,r11,-7520
	ctx.r3.s64 = ctx.r11.s64 + -7520;
	// b 0x8226f518
	sub_8226F518(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED5B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED5B8) { __imp__sub_829ED5B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED5B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32059
	ctx.r11.s64 = -2101018624;
	// li r10,14399
	ctx.r10.s64 = 14399;
	// addi r11,r11,7328
	ctx.r11.s64 = ctx.r11.s64 + 7328;
	// li r9,0
	ctx.r9.s64 = 0;
loc_829ED5C8:
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
	// bge cr6,0x829ed5c8
	if (!ctx.cr6.lt) goto loc_829ED5C8;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-9392
	ctx.r3.s64 = ctx.r11.s64 + -9392;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED5F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED5F0) { __imp__sub_829ED5F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED5F0) {
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
	// lis r11,-32056
	ctx.r11.s64 = -2100822016;
	// li r31,255
	ctx.r31.s64 = 255;
	// addi r30,r11,-16480
	ctx.r30.s64 = ctx.r11.s64 + -16480;
loc_829ED610:
	// lis r11,-32123
	ctx.r11.s64 = -2105212928;
	// li r5,5
	ctx.r5.s64 = 5;
	// addi r6,r11,21328
	ctx.r6.s64 = ctx.r11.s64 + 21328;
	// li r4,4
	ctx.r4.s64 = 4;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82120220
	ctx.lr = 0x829ED628;
	sub_82120220(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,20
	ctx.r30.s64 = ctx.r30.s64 + 20;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x829ed610
	if (!ctx.cr6.lt) goto loc_829ED610;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-9296
	ctx.r3.s64 = ctx.r11.s64 + -9296;
	// bl 0x8298ed98
	ctx.lr = 0x829ED644;
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

PPC_FUNC_IMPL(__imp__sub_829ED660);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED660) { __imp__sub_829ED660(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED660) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32056
	ctx.r11.s64 = -2100822016;
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r11,r11,-11360
	ctx.r11.s64 = ctx.r11.s64 + -11360;
	// li r10,900
	ctx.r10.s64 = 900;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_829ED674:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x829ed674
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_829ED674;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-9496
	ctx.r3.s64 = ctx.r11.s64 + -9496;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED690);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED690) { __imp__sub_829ED690(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED690) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-9408
	ctx.r3.s64 = ctx.r11.s64 + -9408;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED6A0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED6A0) { __imp__sub_829ED6A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED6A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-9192
	ctx.r3.s64 = ctx.r11.s64 + -9192;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED6B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED6B0) { __imp__sub_829ED6B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED6B0) {
	PPC_FUNC_PROLOGUE();
loc_829ED6B0:
	// mftb r11
	ctx.r11.u64 = PPC_QUERY_TIMEBASE();
	// rotlwi r10,r11,0
	ctx.r10.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x829ed6b0
	if (ctx.cr6.eq) goto loc_829ED6B0;
	// lis r10,-32056
	ctx.r10.s64 = -2100822016;
	// std r11,-7528(r10)
	PPC_STORE_U64(ctx.r10.u32 + -7528, ctx.r11.u64);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED6D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED6D0) { __imp__sub_829ED6D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED6D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32056
	ctx.r11.s64 = -2100822016;
	// addi r3,r11,-5768
	ctx.r3.s64 = ctx.r11.s64 + -5768;
	// b 0x824a8790
	sub_824A8790(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED6E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED6E0) { __imp__sub_829ED6E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED6E0) {
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
	// lis r11,-32056
	ctx.r11.s64 = -2100822016;
	// li r31,1549
	ctx.r31.s64 = 1549;
	// addi r30,r11,-5752
	ctx.r30.s64 = ctx.r11.s64 + -5752;
loc_829ED700:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x826cf0c8
	ctx.lr = 0x829ED708;
	sub_826CF0C8(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,28
	ctx.r30.s64 = ctx.r30.s64 + 28;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x829ed700
	if (!ctx.cr6.lt) goto loc_829ED700;
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

PPC_FUNC_IMPL(__imp__sub_829ED730);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED730) { __imp__sub_829ED730(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED730) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32055
	ctx.r11.s64 = -2100756480;
	// addi r3,r11,-27888
	ctx.r3.s64 = ctx.r11.s64 + -27888;
	// b 0x8228b0a8
	sub_8228B0A8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED740);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED740) { __imp__sub_829ED740(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED740) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r4,r11,-13196
	ctx.r4.s64 = ctx.r11.s64 + -13196;
	// lis r11,-32055
	ctx.r11.s64 = -2100756480;
	// addi r3,r11,-24136
	ctx.r3.s64 = ctx.r11.s64 + -24136;
	// b 0x8226f518
	sub_8226F518(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED758);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED758) { __imp__sub_829ED758(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED758) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r4,r11,-13180
	ctx.r4.s64 = ctx.r11.s64 + -13180;
	// lis r11,-32055
	ctx.r11.s64 = -2100756480;
	// addi r3,r11,-24124
	ctx.r3.s64 = ctx.r11.s64 + -24124;
	// b 0x8226f518
	sub_8226F518(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED770);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED770) { __imp__sub_829ED770(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED770) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r4,r11,-13164
	ctx.r4.s64 = ctx.r11.s64 + -13164;
	// lis r11,-32055
	ctx.r11.s64 = -2100756480;
	// addi r3,r11,-24112
	ctx.r3.s64 = ctx.r11.s64 + -24112;
	// b 0x8226f518
	sub_8226F518(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED788);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED788) { __imp__sub_829ED788(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED788) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-9160
	ctx.r3.s64 = ctx.r11.s64 + -9160;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED798);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED798) { __imp__sub_829ED798(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED798) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-9128
	ctx.r3.s64 = ctx.r11.s64 + -9128;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED7A8);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED7A8) { __imp__sub_829ED7A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED7A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-9096
	ctx.r3.s64 = ctx.r11.s64 + -9096;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED7B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED7B8) { __imp__sub_829ED7B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED7B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-9064
	ctx.r3.s64 = ctx.r11.s64 + -9064;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED7C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED7C8) { __imp__sub_829ED7C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED7C8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32055
	ctx.r11.s64 = -2100756480;
	// addi r3,r11,-7792
	ctx.r3.s64 = ctx.r11.s64 + -7792;
	// bl 0x827f1320
	ctx.lr = 0x829ED7E0;
	sub_827F1320(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-9032
	ctx.r3.s64 = ctx.r11.s64 + -9032;
	// bl 0x8298ed98
	ctx.lr = 0x829ED7EC;
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

PPC_FUNC_IMPL(__imp__sub_829ED800);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED800) { __imp__sub_829ED800(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED800) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32055
	ctx.r11.s64 = -2100756480;
	// addi r3,r11,-7760
	ctx.r3.s64 = ctx.r11.s64 + -7760;
	// bl 0x827f1320
	ctx.lr = 0x829ED818;
	sub_827F1320(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-9016
	ctx.r3.s64 = ctx.r11.s64 + -9016;
	// bl 0x8298ed98
	ctx.lr = 0x829ED824;
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

PPC_FUNC_IMPL(__imp__sub_829ED838);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED838) { __imp__sub_829ED838(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED838) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32055
	ctx.r11.s64 = -2100756480;
	// addi r3,r11,-7728
	ctx.r3.s64 = ctx.r11.s64 + -7728;
	// bl 0x827f1320
	ctx.lr = 0x829ED850;
	sub_827F1320(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-9000
	ctx.r3.s64 = ctx.r11.s64 + -9000;
	// bl 0x8298ed98
	ctx.lr = 0x829ED85C;
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

PPC_FUNC_IMPL(__imp__sub_829ED870);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED870) { __imp__sub_829ED870(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED870) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32055
	ctx.r11.s64 = -2100756480;
	// addi r3,r11,-7696
	ctx.r3.s64 = ctx.r11.s64 + -7696;
	// bl 0x827f1320
	ctx.lr = 0x829ED888;
	sub_827F1320(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-8984
	ctx.r3.s64 = ctx.r11.s64 + -8984;
	// bl 0x8298ed98
	ctx.lr = 0x829ED894;
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

PPC_FUNC_IMPL(__imp__sub_829ED8A8);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED8A8) { __imp__sub_829ED8A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED8A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32055
	ctx.r11.s64 = -2100756480;
	// li r10,15
	ctx.r10.s64 = 15;
	// addi r7,r11,-7664
	ctx.r7.s64 = ctx.r11.s64 + -7664;
	// li r8,-1
	ctx.r8.s64 = -1;
	// addi r11,r7,8
	ctx.r11.s64 = ctx.r7.s64 + 8;
	// li r9,0
	ctx.r9.s64 = 0;
loc_829ED8C0:
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
	// bge cr6,0x829ed8c0
	if (!ctx.cr6.lt) goto loc_829ED8C0;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// stw r9,320(r7)
	PPC_STORE_U32(ctx.r7.u32 + 320, ctx.r9.u32);
	// addi r3,r11,-8768
	ctx.r3.s64 = ctx.r11.s64 + -8768;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED8F8);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED8F8) { __imp__sub_829ED8F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED8F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-8968
	ctx.r3.s64 = ctx.r11.s64 + -8968;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED908);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED908) { __imp__sub_829ED908(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED908) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-8752
	ctx.r3.s64 = ctx.r11.s64 + -8752;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED918);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED918) { __imp__sub_829ED918(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED918) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-8720
	ctx.r3.s64 = ctx.r11.s64 + -8720;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED928);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED928) { __imp__sub_829ED928(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED928) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x829ED930;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32055
	ctx.r11.s64 = -2100756480;
	// li r30,31
	ctx.r30.s64 = 31;
	// addi r11,r11,-4176
	ctx.r11.s64 = ctx.r11.s64 + -4176;
	// addi r31,r11,144
	ctx.r31.s64 = ctx.r11.s64 + 144;
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r29,r11,65535
	ctx.r29.u64 = ctx.r11.u64 | 65535;
loc_829ED94C:
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// li r5,17
	ctx.r5.s64 = 17;
	// addi r6,r11,2744
	ctx.r6.s64 = ctx.r11.s64 + 2744;
	// li r4,16
	ctx.r4.s64 = 16;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82120220
	ctx.lr = 0x829ED964;
	sub_82120220(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// li r5,8
	ctx.r5.s64 = 8;
	// addi r6,r11,2744
	ctx.r6.s64 = ctx.r11.s64 + 2744;
	// li r4,32
	ctx.r4.s64 = 32;
	// addi r3,r31,416
	ctx.r3.s64 = ctx.r31.s64 + 416;
	// bl 0x82120220
	ctx.lr = 0x829ED97C;
	sub_82120220(ctx, base);
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
	// bl 0x826337b0
	ctx.lr = 0x829ED994;
	sub_826337B0(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// addi r31,r31,976
	ctx.r31.s64 = ctx.r31.s64 + 976;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829ed94c
	if (!ctx.cr6.lt) goto loc_829ED94C;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-8912
	ctx.r3.s64 = ctx.r11.s64 + -8912;
	// bl 0x8298ed98
	ctx.lr = 0x829ED9B0;
	sub_8298ED98(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED9B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED9B8) { __imp__sub_829ED9B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED9B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32055
	ctx.r11.s64 = -2100756480;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,27056
	ctx.r11.s64 = ctx.r11.s64 + 27056;
	// lis r9,-32096
	ctx.r9.s64 = -2103443456;
	// addi r3,r9,-8904
	ctx.r3.s64 = ctx.r9.s64 + -8904;
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
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829ED9F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829ED9F0) { __imp__sub_829ED9F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829ED9F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-8896
	ctx.r3.s64 = ctx.r11.s64 + -8896;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EDA00);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDA00) { __imp__sub_829EDA00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDA00) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-8888
	ctx.r3.s64 = ctx.r11.s64 + -8888;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EDA10);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDA10) { __imp__sub_829EDA10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDA10) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-8880
	ctx.r3.s64 = ctx.r11.s64 + -8880;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EDA20);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDA20) { __imp__sub_829EDA20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDA20) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32054
	ctx.r11.s64 = -2100690944;
	// addi r3,r11,-18000
	ctx.r3.s64 = ctx.r11.s64 + -18000;
	// bl 0x8263c040
	ctx.lr = 0x829EDA38;
	sub_8263C040(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-8872
	ctx.r3.s64 = ctx.r11.s64 + -8872;
	// bl 0x8298ed98
	ctx.lr = 0x829EDA44;
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

PPC_FUNC_IMPL(__imp__sub_829EDA58);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDA58) { __imp__sub_829EDA58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDA58) {
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
	// lis r11,-32054
	ctx.r11.s64 = -2100690944;
	// li r31,15
	ctx.r31.s64 = 15;
	// addi r30,r11,-13632
	ctx.r30.s64 = ctx.r11.s64 + -13632;
loc_829EDA78:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x828e0ab8
	ctx.lr = 0x829EDA80;
	sub_828E0AB8(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,32
	ctx.r30.s64 = ctx.r30.s64 + 32;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x829eda78
	if (!ctx.cr6.lt) goto loc_829EDA78;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-8856
	ctx.r3.s64 = ctx.r11.s64 + -8856;
	// bl 0x8298ed98
	ctx.lr = 0x829EDA9C;
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

PPC_FUNC_IMPL(__imp__sub_829EDAB8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDAB8) { __imp__sub_829EDAB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDAB8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32054
	ctx.r11.s64 = -2100690944;
	// li r5,3200
	ctx.r5.s64 = 3200;
	// addi r3,r11,-10240
	ctx.r3.s64 = ctx.r11.s64 + -10240;
	// li r4,0
	ctx.r4.s64 = 0;
	// b 0x8298eb90
	sub_8298EB90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EDAD0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDAD0) { __imp__sub_829EDAD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDAD0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-8688
	ctx.r3.s64 = ctx.r11.s64 + -8688;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EDAE0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDAE0) { __imp__sub_829EDAE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDAE0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32053
	ctx.r11.s64 = -2100625408;
	// lis r9,-32255
	ctx.r9.s64 = -2113863680;
	// addi r11,r11,-9308
	ctx.r11.s64 = ctx.r11.s64 + -9308;
	// addi r8,r9,-9676
	ctx.r8.s64 = ctx.r9.s64 + -9676;
	// li r10,31
	ctx.r10.s64 = 31;
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// li r9,0
	ctx.r9.s64 = 0;
loc_829EDAFC:
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
	// bge cr6,0x829edafc
	if (!ctx.cr6.lt) goto loc_829EDAFC;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-8656
	ctx.r3.s64 = ctx.r11.s64 + -8656;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EDB28);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDB28) { __imp__sub_829EDB28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDB28) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// lis r10,-32255
	ctx.r10.s64 = -2113863680;
	// addi r3,r11,-8520
	ctx.r3.s64 = ctx.r11.s64 + -8520;
	// lis r11,-32053
	ctx.r11.s64 = -2100625408;
	// addi r10,r10,-9668
	ctx.r10.s64 = ctx.r10.s64 + -9668;
	// addi r11,r11,-8796
	ctx.r11.s64 = ctx.r11.s64 + -8796;
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
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EDBD0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDBD0) { __imp__sub_829EDBD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDBD0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-5616
	ctx.r5.s64 = ctx.r11.s64 + -5616;
	// lis r11,-32053
	ctx.r11.s64 = -2100625408;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-8584
	ctx.r3.s64 = ctx.r11.s64 + -8584;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EDBF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDBF0) { __imp__sub_829EDBF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDBF0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32053
	ctx.r11.s64 = -2100625408;
	// addi r31,r11,-8564
	ctx.r31.s64 = ctx.r11.s64 + -8564;
	// addi r3,r31,4
	ctx.r3.s64 = ctx.r31.s64 + 4;
	// bl 0x822de5f0
	ctx.lr = 0x829EDC10;
	sub_822DE5F0(ctx, base);
	// addi r3,r31,16
	ctx.r3.s64 = ctx.r31.s64 + 16;
	// bl 0x822c1ba8
	ctx.lr = 0x829EDC18;
	sub_822C1BA8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-8384
	ctx.r3.s64 = ctx.r11.s64 + -8384;
	// bl 0x8298ed98
	ctx.lr = 0x829EDC24;
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

PPC_FUNC_IMPL(__imp__sub_829EDC38);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDC38) { __imp__sub_829EDC38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDC38) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-8528
	ctx.r3.s64 = ctx.r11.s64 + -8528;
	// bl 0x822c1f08
	ctx.lr = 0x829EDC50;
	sub_822C1F08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-8288
	ctx.r3.s64 = ctx.r11.s64 + -8288;
	// bl 0x8298ed98
	ctx.lr = 0x829EDC5C;
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

PPC_FUNC_IMPL(__imp__sub_829EDC70);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDC70) { __imp__sub_829EDC70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDC70) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,24800
	ctx.r3.s64 = ctx.r11.s64 + 24800;
	// bl 0x827f1320
	ctx.lr = 0x829EDC88;
	sub_827F1320(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-8304
	ctx.r3.s64 = ctx.r11.s64 + -8304;
	// bl 0x8298ed98
	ctx.lr = 0x829EDC94;
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

PPC_FUNC_IMPL(__imp__sub_829EDCA8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDCA8) { __imp__sub_829EDCA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDCA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32053
	ctx.r11.s64 = -2100625408;
	// addi r3,r11,24912
	ctx.r3.s64 = ctx.r11.s64 + 24912;
	// b 0x822c5568
	sub_822C5568(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EDCB8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDCB8) { __imp__sub_829EDCB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDCB8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// li r6,11
	ctx.r6.s64 = 11;
	// addi r11,r11,-15016
	ctx.r11.s64 = ctx.r11.s64 + -15016;
	// li r7,-1
	ctx.r7.s64 = -1;
	// addi r9,r11,16
	ctx.r9.s64 = ctx.r11.s64 + 16;
	// li r8,0
	ctx.r8.s64 = 0;
loc_829EDCD0:
	// mr r11,r8
	ctx.r11.u64 = ctx.r8.u64;
	// stb r7,-16(r9)
	PPC_STORE_U8(ctx.r9.u32 + -16, ctx.r7.u8);
	// addi r10,r9,-12
	ctx.r10.s64 = ctx.r9.s64 + -12;
loc_829EDCDC:
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
	// blt cr6,0x829edcdc
	if (ctx.cr6.lt) goto loc_829EDCDC;
	// addi r6,r6,-1
	ctx.r6.s64 = ctx.r6.s64 + -1;
	// addi r9,r9,20
	ctx.r9.s64 = ctx.r9.s64 + 20;
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// bge cr6,0x829edcd0
	if (!ctx.cr6.lt) goto loc_829EDCD0;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-8168
	ctx.r3.s64 = ctx.r11.s64 + -8168;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EDD10);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDD10) { __imp__sub_829EDD10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDD10) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-8256
	ctx.r3.s64 = ctx.r11.s64 + -8256;
	// bl 0x8284f880
	ctx.lr = 0x829EDD28;
	sub_8284F880(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-8160
	ctx.r3.s64 = ctx.r11.s64 + -8160;
	// bl 0x8298ed98
	ctx.lr = 0x829EDD34;
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

PPC_FUNC_IMPL(__imp__sub_829EDD48);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDD48) { __imp__sub_829EDD48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDD48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-8144
	ctx.r3.s64 = ctx.r11.s64 + -8144;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EDD58);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDD58) { __imp__sub_829EDD58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDD58) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// li r9,-1
	ctx.r9.s64 = -1;
	// addi r10,r11,-7232
	ctx.r10.s64 = ctx.r11.s64 + -7232;
	// li r11,130
	ctx.r11.s64 = 130;
	// addi r10,r10,36
	ctx.r10.s64 = ctx.r10.s64 + 36;
loc_829EDD6C:
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// addi r10,r10,44
	ctx.r10.s64 = ctx.r10.s64 + 44;
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// bge cr6,0x829edd6c
	if (!ctx.cr6.lt) goto loc_829EDD6C;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EDD88);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDD88) { __imp__sub_829EDD88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDD88) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-8080
	ctx.r3.s64 = ctx.r11.s64 + -8080;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EDD98);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDD98) { __imp__sub_829EDD98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDD98) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r4,r11,-4512
	ctx.r4.s64 = ctx.r11.s64 + -4512;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// addi r3,r11,148
	ctx.r3.s64 = ctx.r11.s64 + 148;
	// b 0x8226f518
	sub_8226F518(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EDDB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDDB0) { __imp__sub_829EDDB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDDB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,4156
	ctx.r11.s64 = ctx.r11.s64 + 4156;
	// lis r9,-32096
	ctx.r9.s64 = -2103443456;
	// addi r3,r9,-8064
	ctx.r3.s64 = ctx.r9.s64 + -8064;
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
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EDDE0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDDE0) { __imp__sub_829EDDE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDDE0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-7976
	ctx.r3.s64 = ctx.r11.s64 + -7976;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EDDF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDDF0) { __imp__sub_829EDDF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDDF0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-7888
	ctx.r3.s64 = ctx.r11.s64 + -7888;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EDE00);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDE00) { __imp__sub_829EDE00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDE00) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-7968
	ctx.r3.s64 = ctx.r11.s64 + -7968;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EDE10);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDE10) { __imp__sub_829EDE10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDE10) {
	PPC_FUNC_PROLOGUE();
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
	// bl 0x8218be78
	ctx.lr = 0x829EDE24;
	sub_8218BE78(ctx, base);
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,4360
	ctx.r11.s64 = ctx.r11.s64 + 4360;
	// lis r9,-32096
	ctx.r9.s64 = -2103443456;
	// addi r3,r9,-7960
	ctx.r3.s64 = ctx.r9.s64 + -7960;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// sth r10,4(r11)
	PPC_STORE_U16(ctx.r11.u32 + 4, ctx.r10.u16);
	// sth r10,6(r11)
	PPC_STORE_U16(ctx.r11.u32 + 6, ctx.r10.u16);
	// bl 0x8298ed98
	ctx.lr = 0x829EDE48;
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

PPC_FUNC_IMPL(__imp__sub_829EDE58);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDE58) { __imp__sub_829EDE58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDE58) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-1676
	ctx.r5.s64 = ctx.r11.s64 + -1676;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,4380
	ctx.r3.s64 = ctx.r11.s64 + 4380;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EDE78);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDE78) { __imp__sub_829EDE78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDE78) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-836
	ctx.r5.s64 = ctx.r11.s64 + -836;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,4656
	ctx.r3.s64 = ctx.r11.s64 + 4656;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EDE98);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDE98) { __imp__sub_829EDE98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDE98) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r9,r11,4704
	ctx.r9.s64 = ctx.r11.s64 + 4704;
	// addi r11,r9,32
	ctx.r11.s64 = ctx.r9.s64 + 32;
loc_829EDEA8:
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
	// blt cr6,0x829edea8
	if (ctx.cr6.lt) goto loc_829EDEA8;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-7872
	ctx.r3.s64 = ctx.r11.s64 + -7872;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EDED0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDED0) { __imp__sub_829EDED0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDED0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-7864
	ctx.r3.s64 = ctx.r11.s64 + -7864;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EDEE0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDEE0) { __imp__sub_829EDEE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDEE0) {
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
	// lis r11,-32050
	ctx.r11.s64 = -2100428800;
	// lis r7,-32096
	ctx.r7.s64 = -2103443456;
	// addi r11,r11,-13184
	ctx.r11.s64 = ctx.r11.s64 + -13184;
	// std r6,0(r8)
	PPC_STORE_U64(ctx.r8.u32 + 0, ctx.r6.u64);
	// addi r3,r7,-7776
	ctx.r3.s64 = ctx.r7.s64 + -7776;
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
	// bl 0x8298ed98
	ctx.lr = 0x829EDF54;
	sub_8298ED98(ctx, base);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EDF68);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDF68) { __imp__sub_829EDF68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDF68) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-824
	ctx.r5.s64 = ctx.r11.s64 + -824;
	// lis r11,-32050
	ctx.r11.s64 = -2100428800;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-9916
	ctx.r3.s64 = ctx.r11.s64 + -9916;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EDF88);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDF88) { __imp__sub_829EDF88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDF88) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-7688
	ctx.r3.s64 = ctx.r11.s64 + -7688;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EDF98);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDF98) { __imp__sub_829EDF98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDF98) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32050
	ctx.r11.s64 = -2100428800;
	// li r9,199
	ctx.r9.s64 = 199;
	// addi r11,r11,-9112
	ctx.r11.s64 = ctx.r11.s64 + -9112;
	// addi r10,r11,44
	ctx.r10.s64 = ctx.r11.s64 + 44;
	// li r11,-1
	ctx.r11.s64 = -1;
loc_829EDFAC:
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
	// bge cr6,0x829edfac
	if (!ctx.cr6.lt) goto loc_829EDFAC;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EDFD8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EDFD8) { __imp__sub_829EDFD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EDFD8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32050
	ctx.r11.s64 = -2100428800;
	// li r9,98
	ctx.r9.s64 = 98;
	// addi r11,r11,28512
	ctx.r11.s64 = ctx.r11.s64 + 28512;
	// addi r10,r11,8
	ctx.r10.s64 = ctx.r11.s64 + 8;
	// li r11,-1
	ctx.r11.s64 = -1;
loc_829EDFEC:
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
	// bge cr6,0x829edfec
	if (!ctx.cr6.lt) goto loc_829EDFEC;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EE028);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE028) { __imp__sub_829EE028(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE028) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32049
	ctx.r11.s64 = -2100363264;
	// li r10,-1
	ctx.r10.s64 = -1;
	// addi r11,r11,15248
	ctx.r11.s64 = ctx.r11.s64 + 15248;
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

PPC_FUNC_IMPL(__imp__sub_829EE088);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE088) { __imp__sub_829EE088(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE088) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32015
	ctx.r11.s64 = -2098135040;
	// lis r10,-32050
	ctx.r10.s64 = -2100428800;
	// lwz r11,19700(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 19700);
	// stw r11,-9116(r10)
	PPC_STORE_U32(ctx.r10.u32 + -9116, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EE0A0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE0A0) { __imp__sub_829EE0A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE0A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32015
	ctx.r11.s64 = -2098135040;
	// lis r10,-32050
	ctx.r10.s64 = -2100428800;
	// lwz r11,19696(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 19696);
	// stw r11,28488(r10)
	PPC_STORE_U32(ctx.r10.u32 + 28488, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EE0B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE0B8) { __imp__sub_829EE0B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE0B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32049
	ctx.r11.s64 = -2100363264;
	// li r8,254
	ctx.r8.s64 = 254;
	// addi r11,r11,25936
	ctx.r11.s64 = ctx.r11.s64 + 25936;
	// li r7,-1
	ctx.r7.s64 = -1;
	// addi r11,r11,36
	ctx.r11.s64 = ctx.r11.s64 + 36;
	// li r9,255
	ctx.r9.s64 = 255;
	// li r6,0
	ctx.r6.s64 = 0;
loc_829EE0D4:
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
	// bge cr6,0x829ee0d4
	if (!ctx.cr6.lt) goto loc_829EE0D4;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EE108);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE108) { __imp__sub_829EE108(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE108) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-7680
	ctx.r3.s64 = ctx.r11.s64 + -7680;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EE118);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE118) { __imp__sub_829EE118(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE118) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32048
	ctx.r11.s64 = -2100297728;
	// addi r3,r11,-25088
	ctx.r3.s64 = ctx.r11.s64 + -25088;
	// bl 0x822faae0
	ctx.lr = 0x829EE130;
	sub_822FAAE0(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-7600
	ctx.r3.s64 = ctx.r11.s64 + -7600;
	// bl 0x8298ed98
	ctx.lr = 0x829EE13C;
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

PPC_FUNC_IMPL(__imp__sub_829EE150);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE150) { __imp__sub_829EE150(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE150) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32048
	ctx.r11.s64 = -2100297728;
	// addi r3,r11,-24064
	ctx.r3.s64 = ctx.r11.s64 + -24064;
	// bl 0x8289df08
	ctx.lr = 0x829EE168;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-7608
	ctx.r3.s64 = ctx.r11.s64 + -7608;
	// bl 0x8298ed98
	ctx.lr = 0x829EE174;
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

PPC_FUNC_IMPL(__imp__sub_829EE188);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE188) { __imp__sub_829EE188(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE188) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,2988
	ctx.r3.s64 = ctx.r11.s64 + 2988;
	// bl 0x827df490
	ctx.lr = 0x829EE1A4;
	sub_827DF490(ctx, base);
	// lis r11,-32048
	ctx.r11.s64 = -2100297728;
	// stw r3,-24024(r11)
	PPC_STORE_U32(ctx.r11.u32 + -24024, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EE1C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE1C0) { __imp__sub_829EE1C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE1C0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,3016
	ctx.r3.s64 = ctx.r11.s64 + 3016;
	// bl 0x827df490
	ctx.lr = 0x829EE1DC;
	sub_827DF490(ctx, base);
	// lis r11,-32048
	ctx.r11.s64 = -2100297728;
	// stw r3,-24020(r11)
	PPC_STORE_U32(ctx.r11.u32 + -24020, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EE1F8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE1F8) { __imp__sub_829EE1F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE1F8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,3044
	ctx.r3.s64 = ctx.r11.s64 + 3044;
	// bl 0x827df490
	ctx.lr = 0x829EE214;
	sub_827DF490(ctx, base);
	// lis r11,-32048
	ctx.r11.s64 = -2100297728;
	// stw r3,-24016(r11)
	PPC_STORE_U32(ctx.r11.u32 + -24016, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EE230);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE230) { __imp__sub_829EE230(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE230) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-7576
	ctx.r3.s64 = ctx.r11.s64 + -7576;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EE240);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE240) { __imp__sub_829EE240(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE240) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,5904
	ctx.r5.s64 = ctx.r11.s64 + 5904;
	// lis r11,-32048
	ctx.r11.s64 = -2100297728;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,16972
	ctx.r3.s64 = ctx.r11.s64 + 16972;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EE260);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE260) { __imp__sub_829EE260(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE260) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,3916
	ctx.r5.s64 = ctx.r11.s64 + 3916;
	// lis r11,-32048
	ctx.r11.s64 = -2100297728;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,19396
	ctx.r3.s64 = ctx.r11.s64 + 19396;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EE280);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE280) { __imp__sub_829EE280(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE280) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32048
	ctx.r11.s64 = -2100297728;
	// addi r3,r11,19456
	ctx.r3.s64 = ctx.r11.s64 + 19456;
	// bl 0x823056b0
	ctx.lr = 0x829EE298;
	sub_823056B0(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-7552
	ctx.r3.s64 = ctx.r11.s64 + -7552;
	// bl 0x8298ed98
	ctx.lr = 0x829EE2A4;
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

PPC_FUNC_IMPL(__imp__sub_829EE2B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE2B8) { __imp__sub_829EE2B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE2B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32039
	ctx.r11.s64 = -2099707904;
	// addi r3,r11,15216
	ctx.r3.s64 = ctx.r11.s64 + 15216;
	// b 0x823128e8
	sub_823128E8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EE2C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE2C8) { __imp__sub_829EE2C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE2C8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb10
	ctx.lr = 0x829EE2D0;
	__savegprlr_26(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32018
	ctx.r11.s64 = -2098331648;
	// addi r7,r10,-8544
	ctx.r7.s64 = ctx.r10.s64 + -8544;
	// lis r4,-32255
	ctx.r4.s64 = -2113863680;
	// lis r5,-32256
	ctx.r5.s64 = -2113929216;
	// lis r6,-32256
	ctx.r6.s64 = -2113929216;
	// addi r11,r11,20480
	ctx.r11.s64 = ctx.r11.s64 + 20480;
	// lvx128 v0,r0,r7
	ea = (ctx.r7.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r8,59
	ctx.r8.s64 = 59;
	// lfs f12,29496(r4)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r4.u32 + 29496);
	ctx.f12.f64 = double(temp.f32);
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// lfs f13,2592(r5)
	temp.u32 = PPC_LOAD_U32(ctx.r5.u32 + 2592);
	ctx.f13.f64 = double(temp.f32);
	// li r10,0
	ctx.r10.s64 = 0;
	// lfs f0,3068(r6)
	temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + 3068);
	ctx.f0.f64 = double(temp.f32);
	// li r9,-1
	ctx.r9.s64 = -1;
	// li r26,56
	ctx.r26.s64 = 56;
	// li r27,72
	ctx.r27.s64 = 72;
	// li r28,25000
	ctx.r28.s64 = 25000;
	// li r29,4
	ctx.r29.s64 = 4;
	// li r30,5000
	ctx.r30.s64 = 5000;
	// li r31,168
	ctx.r31.s64 = 168;
	// li r3,184
	ctx.r3.s64 = 184;
loc_829EE32C:
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
	// addi r8,r8,-1
	ctx.r8.s64 = ctx.r8.s64 + -1;
	// sth r10,122(r11)
	PPC_STORE_U16(ctx.r11.u32 + 122, ctx.r10.u16);
	// sth r10,124(r11)
	PPC_STORE_U16(ctx.r11.u32 + 124, ctx.r10.u16);
	// stvx128 v0,r11,r26
	ea = (ctx.r11.u32 + ctx.r26.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx128 v0,r11,r27
	ea = (ctx.r11.u32 + ctx.r27.u32) & ~0xF;
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
	// sth r28,128(r11)
	PPC_STORE_U16(ctx.r11.u32 + 128, ctx.r28.u16);
	// stfs f12,136(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 136, temp.u32);
	// stw r10,132(r11)
	PPC_STORE_U32(ctx.r11.u32 + 132, ctx.r10.u32);
	// stw r10,140(r11)
	PPC_STORE_U32(ctx.r11.u32 + 140, ctx.r10.u32);
	// cmpwi cr6,r8,0
	ctx.cr6.compare<int32_t>(ctx.r8.s32, 0, ctx.xer);
	// stw r10,144(r11)
	PPC_STORE_U32(ctx.r11.u32 + 144, ctx.r10.u32);
	// stw r10,148(r11)
	PPC_STORE_U32(ctx.r11.u32 + 148, ctx.r10.u32);
	// stw r29,152(r11)
	PPC_STORE_U32(ctx.r11.u32 + 152, ctx.r29.u32);
	// stw r30,156(r11)
	PPC_STORE_U32(ctx.r11.u32 + 156, ctx.r30.u32);
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
	// addi r11,r11,256
	ctx.r11.s64 = ctx.r11.s64 + 256;
	// bge cr6,0x829ee32c
	if (!ctx.cr6.lt) goto loc_829EE32C;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-7536
	ctx.r3.s64 = ctx.r11.s64 + -7536;
	// bl 0x8298ed98
	ctx.lr = 0x829EE414;
	sub_8298ED98(ctx, base);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EE420);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE420) { __imp__sub_829EE420(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE420) {
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
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r30,r11,-3328
	ctx.r30.s64 = ctx.r11.s64 + -3328;
	// lwz r3,0(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// bl 0x827df490
	ctx.lr = 0x829EE448;
	sub_827DF490(ctx, base);
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r31,r11,-29696
	ctx.r31.s64 = ctx.r11.s64 + -29696;
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// lwz r3,4(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 4);
	// bl 0x827df490
	ctx.lr = 0x829EE460;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r3.u32);
	// lwz r3,8(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 8);
	// bl 0x827df490
	ctx.lr = 0x829EE470;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r3.u32);
	// lwz r3,12(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 12);
	// bl 0x827df490
	ctx.lr = 0x829EE480;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r3.u32);
	// lwz r3,16(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 16);
	// bl 0x827df490
	ctx.lr = 0x829EE490;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r3.u32);
	// lwz r3,20(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 20);
	// bl 0x827df490
	ctx.lr = 0x829EE4A0;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,20(r31)
	PPC_STORE_U32(ctx.r31.u32 + 20, ctx.r3.u32);
	// lwz r3,24(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 24);
	// bl 0x827df490
	ctx.lr = 0x829EE4B0;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,24(r31)
	PPC_STORE_U32(ctx.r31.u32 + 24, ctx.r3.u32);
	// lwz r3,28(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 28);
	// bl 0x827df490
	ctx.lr = 0x829EE4C0;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,28(r31)
	PPC_STORE_U32(ctx.r31.u32 + 28, ctx.r3.u32);
	// lwz r3,32(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 32);
	// bl 0x827df490
	ctx.lr = 0x829EE4D0;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,32(r31)
	PPC_STORE_U32(ctx.r31.u32 + 32, ctx.r3.u32);
	// lwz r3,36(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 36);
	// bl 0x827df490
	ctx.lr = 0x829EE4E0;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,36(r31)
	PPC_STORE_U32(ctx.r31.u32 + 36, ctx.r3.u32);
	// lwz r3,40(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 40);
	// bl 0x827df490
	ctx.lr = 0x829EE4F0;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,40(r31)
	PPC_STORE_U32(ctx.r31.u32 + 40, ctx.r3.u32);
	// lwz r3,44(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 44);
	// bl 0x827df490
	ctx.lr = 0x829EE500;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,44(r31)
	PPC_STORE_U32(ctx.r31.u32 + 44, ctx.r3.u32);
	// lwz r3,48(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 48);
	// bl 0x827df490
	ctx.lr = 0x829EE510;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,48(r31)
	PPC_STORE_U32(ctx.r31.u32 + 48, ctx.r3.u32);
	// lwz r3,52(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 52);
	// bl 0x827df490
	ctx.lr = 0x829EE520;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,52(r31)
	PPC_STORE_U32(ctx.r31.u32 + 52, ctx.r3.u32);
	// lwz r3,56(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 56);
	// bl 0x827df490
	ctx.lr = 0x829EE530;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,56(r31)
	PPC_STORE_U32(ctx.r31.u32 + 56, ctx.r3.u32);
	// lwz r3,60(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 60);
	// bl 0x827df490
	ctx.lr = 0x829EE540;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,60(r31)
	PPC_STORE_U32(ctx.r31.u32 + 60, ctx.r3.u32);
	// lwz r3,64(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 64);
	// bl 0x827df490
	ctx.lr = 0x829EE550;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,64(r31)
	PPC_STORE_U32(ctx.r31.u32 + 64, ctx.r3.u32);
	// lwz r3,68(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 68);
	// bl 0x827df490
	ctx.lr = 0x829EE560;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,68(r31)
	PPC_STORE_U32(ctx.r31.u32 + 68, ctx.r3.u32);
	// lwz r3,72(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 72);
	// bl 0x827df490
	ctx.lr = 0x829EE570;
	sub_827DF490(ctx, base);
	// stw r3,72(r31)
	PPC_STORE_U32(ctx.r31.u32 + 72, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// lwz r3,76(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 76);
	// bl 0x827df490
	ctx.lr = 0x829EE580;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,76(r31)
	PPC_STORE_U32(ctx.r31.u32 + 76, ctx.r3.u32);
	// lwz r3,80(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 80);
	// bl 0x827df490
	ctx.lr = 0x829EE590;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,80(r31)
	PPC_STORE_U32(ctx.r31.u32 + 80, ctx.r3.u32);
	// lwz r3,84(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 84);
	// bl 0x827df490
	ctx.lr = 0x829EE5A0;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,84(r31)
	PPC_STORE_U32(ctx.r31.u32 + 84, ctx.r3.u32);
	// lwz r3,88(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 88);
	// bl 0x827df490
	ctx.lr = 0x829EE5B0;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,88(r31)
	PPC_STORE_U32(ctx.r31.u32 + 88, ctx.r3.u32);
	// lwz r3,92(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 92);
	// bl 0x827df490
	ctx.lr = 0x829EE5C0;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,92(r31)
	PPC_STORE_U32(ctx.r31.u32 + 92, ctx.r3.u32);
	// lwz r3,96(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 96);
	// bl 0x827df490
	ctx.lr = 0x829EE5D0;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,96(r31)
	PPC_STORE_U32(ctx.r31.u32 + 96, ctx.r3.u32);
	// lwz r3,100(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 100);
	// bl 0x827df490
	ctx.lr = 0x829EE5E0;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,100(r31)
	PPC_STORE_U32(ctx.r31.u32 + 100, ctx.r3.u32);
	// lwz r3,104(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 104);
	// bl 0x827df490
	ctx.lr = 0x829EE5F0;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,104(r31)
	PPC_STORE_U32(ctx.r31.u32 + 104, ctx.r3.u32);
	// lwz r3,108(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 108);
	// bl 0x827df490
	ctx.lr = 0x829EE600;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,108(r31)
	PPC_STORE_U32(ctx.r31.u32 + 108, ctx.r3.u32);
	// lwz r3,112(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 112);
	// bl 0x827df490
	ctx.lr = 0x829EE610;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,112(r31)
	PPC_STORE_U32(ctx.r31.u32 + 112, ctx.r3.u32);
	// lwz r3,116(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 116);
	// bl 0x827df490
	ctx.lr = 0x829EE620;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,116(r31)
	PPC_STORE_U32(ctx.r31.u32 + 116, ctx.r3.u32);
	// lwz r3,120(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 120);
	// bl 0x827df490
	ctx.lr = 0x829EE630;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,120(r31)
	PPC_STORE_U32(ctx.r31.u32 + 120, ctx.r3.u32);
	// lwz r3,124(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 124);
	// bl 0x827df490
	ctx.lr = 0x829EE640;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,124(r31)
	PPC_STORE_U32(ctx.r31.u32 + 124, ctx.r3.u32);
	// lwz r3,128(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 128);
	// bl 0x827df490
	ctx.lr = 0x829EE650;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,128(r31)
	PPC_STORE_U32(ctx.r31.u32 + 128, ctx.r3.u32);
	// lwz r3,132(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 132);
	// bl 0x827df490
	ctx.lr = 0x829EE660;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,132(r31)
	PPC_STORE_U32(ctx.r31.u32 + 132, ctx.r3.u32);
	// lwz r3,136(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 136);
	// bl 0x827df490
	ctx.lr = 0x829EE670;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,136(r31)
	PPC_STORE_U32(ctx.r31.u32 + 136, ctx.r3.u32);
	// lwz r3,140(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 140);
	// bl 0x827df490
	ctx.lr = 0x829EE680;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,140(r31)
	PPC_STORE_U32(ctx.r31.u32 + 140, ctx.r3.u32);
	// lwz r3,144(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 144);
	// bl 0x827df490
	ctx.lr = 0x829EE690;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,144(r31)
	PPC_STORE_U32(ctx.r31.u32 + 144, ctx.r3.u32);
	// lwz r3,148(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 148);
	// bl 0x827df490
	ctx.lr = 0x829EE6A0;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,148(r31)
	PPC_STORE_U32(ctx.r31.u32 + 148, ctx.r3.u32);
	// lwz r3,152(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 152);
	// bl 0x827df490
	ctx.lr = 0x829EE6B0;
	sub_827DF490(ctx, base);
	// stw r3,152(r31)
	PPC_STORE_U32(ctx.r31.u32 + 152, ctx.r3.u32);
	// lwz r3,156(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 156);
	// li r4,0
	ctx.r4.s64 = 0;
	// bl 0x827df490
	ctx.lr = 0x829EE6C0;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,156(r31)
	PPC_STORE_U32(ctx.r31.u32 + 156, ctx.r3.u32);
	// lwz r3,160(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 160);
	// bl 0x827df490
	ctx.lr = 0x829EE6D0;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,160(r31)
	PPC_STORE_U32(ctx.r31.u32 + 160, ctx.r3.u32);
	// lwz r3,164(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 164);
	// bl 0x827df490
	ctx.lr = 0x829EE6E0;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,164(r31)
	PPC_STORE_U32(ctx.r31.u32 + 164, ctx.r3.u32);
	// lwz r3,168(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 168);
	// bl 0x827df490
	ctx.lr = 0x829EE6F0;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,168(r31)
	PPC_STORE_U32(ctx.r31.u32 + 168, ctx.r3.u32);
	// lwz r3,172(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 172);
	// bl 0x827df490
	ctx.lr = 0x829EE700;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,172(r31)
	PPC_STORE_U32(ctx.r31.u32 + 172, ctx.r3.u32);
	// lwz r3,176(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 176);
	// bl 0x827df490
	ctx.lr = 0x829EE710;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,176(r31)
	PPC_STORE_U32(ctx.r31.u32 + 176, ctx.r3.u32);
	// lwz r3,180(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 180);
	// bl 0x827df490
	ctx.lr = 0x829EE720;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,180(r31)
	PPC_STORE_U32(ctx.r31.u32 + 180, ctx.r3.u32);
	// lwz r3,184(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 184);
	// bl 0x827df490
	ctx.lr = 0x829EE730;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,184(r31)
	PPC_STORE_U32(ctx.r31.u32 + 184, ctx.r3.u32);
	// lwz r3,188(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 188);
	// bl 0x827df490
	ctx.lr = 0x829EE740;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,188(r31)
	PPC_STORE_U32(ctx.r31.u32 + 188, ctx.r3.u32);
	// lwz r3,192(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 192);
	// bl 0x827df490
	ctx.lr = 0x829EE750;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,192(r31)
	PPC_STORE_U32(ctx.r31.u32 + 192, ctx.r3.u32);
	// lwz r3,196(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 196);
	// bl 0x827df490
	ctx.lr = 0x829EE760;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,196(r31)
	PPC_STORE_U32(ctx.r31.u32 + 196, ctx.r3.u32);
	// lwz r3,200(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 200);
	// bl 0x827df490
	ctx.lr = 0x829EE770;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,200(r31)
	PPC_STORE_U32(ctx.r31.u32 + 200, ctx.r3.u32);
	// lwz r3,204(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 204);
	// bl 0x827df490
	ctx.lr = 0x829EE780;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,204(r31)
	PPC_STORE_U32(ctx.r31.u32 + 204, ctx.r3.u32);
	// lwz r3,208(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 208);
	// bl 0x827df490
	ctx.lr = 0x829EE790;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,208(r31)
	PPC_STORE_U32(ctx.r31.u32 + 208, ctx.r3.u32);
	// lwz r3,212(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 212);
	// bl 0x827df490
	ctx.lr = 0x829EE7A0;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,212(r31)
	PPC_STORE_U32(ctx.r31.u32 + 212, ctx.r3.u32);
	// lwz r3,216(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 216);
	// bl 0x827df490
	ctx.lr = 0x829EE7B0;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,216(r31)
	PPC_STORE_U32(ctx.r31.u32 + 216, ctx.r3.u32);
	// lwz r3,220(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 220);
	// bl 0x827df490
	ctx.lr = 0x829EE7C0;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,220(r31)
	PPC_STORE_U32(ctx.r31.u32 + 220, ctx.r3.u32);
	// lwz r3,224(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 224);
	// bl 0x827df490
	ctx.lr = 0x829EE7D0;
	sub_827DF490(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,224(r31)
	PPC_STORE_U32(ctx.r31.u32 + 224, ctx.r3.u32);
	// lwz r3,228(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 228);
	// bl 0x827df490
	ctx.lr = 0x829EE7E0;
	sub_827DF490(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829EE808);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE808) { __imp__sub_829EE808(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE808) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-7528
	ctx.r3.s64 = ctx.r11.s64 + -7528;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EE818);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE818) { __imp__sub_829EE818(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE818) {
	PPC_FUNC_PROLOGUE();
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
	// addi r5,r11,6540
	ctx.r5.s64 = ctx.r11.s64 + 6540;
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// li r4,16
	ctx.r4.s64 = 16;
	// addi r3,r11,-29456
	ctx.r3.s64 = ctx.r11.s64 + -29456;
	// bl 0x825ee000
	ctx.lr = 0x829EE840;
	sub_825EE000(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-7496
	ctx.r3.s64 = ctx.r11.s64 + -7496;
	// bl 0x8298ed98
	ctx.lr = 0x829EE84C;
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

PPC_FUNC_IMPL(__imp__sub_829EE860);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE860) { __imp__sub_829EE860(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE860) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// addi r3,r11,-29312
	ctx.r3.s64 = ctx.r11.s64 + -29312;
	// bl 0x8285bbf0
	ctx.lr = 0x829EE878;
	sub_8285BBF0(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-7384
	ctx.r3.s64 = ctx.r11.s64 + -7384;
	// bl 0x8298ed98
	ctx.lr = 0x829EE884;
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

PPC_FUNC_IMPL(__imp__sub_829EE898);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE898) { __imp__sub_829EE898(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE898) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,7232
	ctx.r5.s64 = ctx.r11.s64 + 7232;
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-29148
	ctx.r3.s64 = ctx.r11.s64 + -29148;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EE8B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE8B8) { __imp__sub_829EE8B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE8B8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// addi r7,r10,-8560
	ctx.r7.s64 = ctx.r10.s64 + -8560;
	// lis r6,-32256
	ctx.r6.s64 = -2113929216;
	// addi r11,r11,-28992
	ctx.r11.s64 = ctx.r11.s64 + -28992;
	// lis r8,0
	ctx.r8.s64 = 0;
	// li r9,31
	ctx.r9.s64 = 31;
	// lvx128 v0,r0,r7
	ea = (ctx.r7.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r11,32
	ctx.r11.s64 = ctx.r11.s64 + 32;
	// lfs f0,3068(r6)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + 3068);
	ctx.f0.f64 = double(temp.f32);
	// li r10,0
	ctx.r10.s64 = 0;
	// li r4,-16
	ctx.r4.s64 = -16;
	// li r5,16
	ctx.r5.s64 = 16;
	// ori r8,r8,65535
	ctx.r8.u64 = ctx.r8.u64 | 65535;
loc_829EE8F0:
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
	// bge cr6,0x829ee8f0
	if (!ctx.cr6.lt) goto loc_829EE8F0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EE930);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE930) { __imp__sub_829EE930(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE930) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,7424
	ctx.r5.s64 = ctx.r11.s64 + 7424;
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-25376
	ctx.r3.s64 = ctx.r11.s64 + -25376;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EE950);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE950) { __imp__sub_829EE950(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE950) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// li r9,-1
	ctx.r9.s64 = -1;
	// addi r11,r11,-25352
	ctx.r11.s64 = ctx.r11.s64 + -25352;
	// li r10,800
	ctx.r10.s64 = 800;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_829EE964:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x829ee964
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_829EE964;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EE978);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE978) { __imp__sub_829EE978(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE978) {
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
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// lfd f1,11952(r11)
	ctx.fpscr.disableFlushMode();
	ctx.f1.u64 = PPC_LOAD_U64(ctx.r11.u32 + 11952);
	// bl 0x8298f168
	ctx.lr = 0x829EE990;
	sub_8298F168(ctx, base);
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// frsp f0,f1
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(float(ctx.f1.f64));
	// stfs f0,-21892(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -21892, temp.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EE9B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE9B0) { __imp__sub_829EE9B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE9B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-7368
	ctx.r3.s64 = ctx.r11.s64 + -7368;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EE9C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE9C0) { __imp__sub_829EE9C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE9C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// addi r3,r11,-21680
	ctx.r3.s64 = ctx.r11.s64 + -21680;
	// bl 0x82334f70
	ctx.lr = 0x829EE9D8;
	sub_82334F70(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-7360
	ctx.r3.s64 = ctx.r11.s64 + -7360;
	// bl 0x8298ed98
	ctx.lr = 0x829EE9E4;
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

PPC_FUNC_IMPL(__imp__sub_829EE9F8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EE9F8) { __imp__sub_829EE9F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EE9F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// li r9,-1
	ctx.r9.s64 = -1;
	// addi r11,r11,-20792
	ctx.r11.s64 = ctx.r11.s64 + -20792;
	// li r10,32
	ctx.r10.s64 = 32;
	// addi r11,r11,6852
	ctx.r11.s64 = ctx.r11.s64 + 6852;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_829EEA10:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x829eea10
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_829EEA10;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EEA20);  /* forward decl */
PPC_WEAK_FUNC(sub_829EEA20) { __imp__sub_829EEA20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EEA20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15112
	ctx.r5.s64 = ctx.r11.s64 + 15112;
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-13640
	ctx.r3.s64 = ctx.r11.s64 + -13640;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EEA40);  /* forward decl */
PPC_WEAK_FUNC(sub_829EEA40) { __imp__sub_829EEA40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EEA40) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// li r9,-1
	ctx.r9.s64 = -1;
	// addi r11,r11,-11056
	ctx.r11.s64 = ctx.r11.s64 + -11056;
	// li r10,5000
	ctx.r10.s64 = 5000;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_829EEA54:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x829eea54
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_829EEA54;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EEA68);  /* forward decl */
PPC_WEAK_FUNC(sub_829EEA68) { __imp__sub_829EEA68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EEA68) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// li r9,-1
	ctx.r9.s64 = -1;
	// addi r10,r11,8944
	ctx.r10.s64 = ctx.r11.s64 + 8944;
	// li r11,63
	ctx.r11.s64 = 63;
loc_829EEA78:
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// addi r10,r10,8
	ctx.r10.s64 = ctx.r10.s64 + 8;
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// bge cr6,0x829eea78
	if (!ctx.cr6.lt) goto loc_829EEA78;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EEA90);  /* forward decl */
PPC_WEAK_FUNC(sub_829EEA90) { __imp__sub_829EEA90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EEA90) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x829EEA98;
	__savegprlr_27(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// li r27,127
	ctx.r27.s64 = 127;
	// addi r11,r11,10304
	ctx.r11.s64 = ctx.r11.s64 + 10304;
	// li r30,0
	ctx.r30.s64 = 0;
	// addi r31,r11,20
	ctx.r31.s64 = ctx.r11.s64 + 20;
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r29,r11,16500
	ctx.r29.s64 = ctx.r11.s64 + 16500;
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r28,r11,17532
	ctx.r28.s64 = ctx.r11.s64 + 17532;
loc_829EEAC0:
	// addi r3,r31,-20
	ctx.r3.s64 = ctx.r31.s64 + -20;
	// bl 0x82331448
	ctx.lr = 0x829EEAC8;
	sub_82331448(ctx, base);
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
loc_829EEAE0:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x829eeae0
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_829EEAE0;
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
	// bge cr6,0x829eeac0
	if (!ctx.cr6.lt) goto loc_829EEAC0;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-7352
	ctx.r3.s64 = ctx.r11.s64 + -7352;
	// bl 0x8298ed98
	ctx.lr = 0x829EEB1C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EEB28);  /* forward decl */
PPC_WEAK_FUNC(sub_829EEB28) { __imp__sub_829EEB28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EEB28) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// lfs f0,8092(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8092);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// stfs f0,24200(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 24200, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EEB40);  /* forward decl */
PPC_WEAK_FUNC(sub_829EEB40) { __imp__sub_829EEB40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EEB40) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-7232
	ctx.r3.s64 = ctx.r11.s64 + -7232;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EEB50);  /* forward decl */
PPC_WEAK_FUNC(sub_829EEB50) { __imp__sub_829EEB50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EEB50) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,22860
	ctx.r5.s64 = ctx.r11.s64 + 22860;
	// lis r11,-32016
	ctx.r11.s64 = -2098200576;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-32048
	ctx.r3.s64 = ctx.r11.s64 + -32048;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EEB70);  /* forward decl */
PPC_WEAK_FUNC(sub_829EEB70) { __imp__sub_829EEB70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EEB70) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,22868
	ctx.r5.s64 = ctx.r11.s64 + 22868;
	// lis r11,-32016
	ctx.r11.s64 = -2098200576;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-32028
	ctx.r3.s64 = ctx.r11.s64 + -32028;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EEB90);  /* forward decl */
PPC_WEAK_FUNC(sub_829EEB90) { __imp__sub_829EEB90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EEB90) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,22880
	ctx.r5.s64 = ctx.r11.s64 + 22880;
	// lis r11,-32016
	ctx.r11.s64 = -2098200576;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-32008
	ctx.r3.s64 = ctx.r11.s64 + -32008;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EEBB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EEBB0) { __imp__sub_829EEBB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EEBB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,22892
	ctx.r5.s64 = ctx.r11.s64 + 22892;
	// lis r11,-32016
	ctx.r11.s64 = -2098200576;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-31988
	ctx.r3.s64 = ctx.r11.s64 + -31988;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EEBD0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EEBD0) { __imp__sub_829EEBD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EEBD0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// lfs f0,3892(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3892);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32016
	ctx.r11.s64 = -2098200576;
	// fmuls f0,f0,f0
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f0.f64));
	// stfs f0,-31968(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -31968, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EEBE8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EEBE8) { __imp__sub_829EEBE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EEBE8) {
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
	// lis r11,-32016
	ctx.r11.s64 = -2098200576;
	// li r31,5
	ctx.r31.s64 = 5;
	// addi r30,r11,-31648
	ctx.r30.s64 = ctx.r11.s64 + -31648;
loc_829EEC08:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x828a31e8
	ctx.lr = 0x829EEC10;
	sub_828A31E8(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,80
	ctx.r30.s64 = ctx.r30.s64 + 80;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x829eec08
	if (!ctx.cr6.lt) goto loc_829EEC08;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-7224
	ctx.r3.s64 = ctx.r11.s64 + -7224;
	// bl 0x8298ed98
	ctx.lr = 0x829EEC2C;
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

PPC_FUNC_IMPL(__imp__sub_829EEC48);  /* forward decl */
PPC_WEAK_FUNC(sub_829EEC48) { __imp__sub_829EEC48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EEC48) {
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
	// lis r11,-32016
	ctx.r11.s64 = -2098200576;
	// li r31,255
	ctx.r31.s64 = 255;
	// addi r11,r11,-31152
	ctx.r11.s64 = ctx.r11.s64 + -31152;
	// addi r30,r11,388
	ctx.r30.s64 = ctx.r11.s64 + 388;
loc_829EEC6C:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82656ed8
	ctx.lr = 0x829EEC74;
	sub_82656ED8(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,224
	ctx.r30.s64 = ctx.r30.s64 + 224;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x829eec6c
	if (!ctx.cr6.lt) goto loc_829EEC6C;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-7136
	ctx.r3.s64 = ctx.r11.s64 + -7136;
	// bl 0x8298ed98
	ctx.lr = 0x829EEC90;
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

PPC_FUNC_IMPL(__imp__sub_829EECA8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EECA8) { __imp__sub_829EECA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EECA8) {
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
	// lis r11,-32015
	ctx.r11.s64 = -2098135040;
	// li r31,31
	ctx.r31.s64 = 31;
	// addi r11,r11,-18528
	ctx.r11.s64 = ctx.r11.s64 + -18528;
	// addi r30,r11,48
	ctx.r30.s64 = ctx.r11.s64 + 48;
loc_829EECCC:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x828a31e8
	ctx.lr = 0x829EECD4;
	sub_828A31E8(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,144
	ctx.r30.s64 = ctx.r30.s64 + 144;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x829eeccc
	if (!ctx.cr6.lt) goto loc_829EECCC;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-7120
	ctx.r3.s64 = ctx.r11.s64 + -7120;
	// bl 0x8298ed98
	ctx.lr = 0x829EECF0;
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

PPC_FUNC_IMPL(__imp__sub_829EED08);  /* forward decl */
PPC_WEAK_FUNC(sub_829EED08) { __imp__sub_829EED08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EED08) {
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
	// lis r11,-32015
	ctx.r11.s64 = -2098135040;
	// li r31,3
	ctx.r31.s64 = 3;
	// addi r30,r11,-13852
	ctx.r30.s64 = ctx.r11.s64 + -13852;
loc_829EED28:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8222c170
	ctx.lr = 0x829EED30;
	sub_8222C170(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,16
	ctx.r30.s64 = ctx.r30.s64 + 16;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x829eed28
	if (!ctx.cr6.lt) goto loc_829EED28;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-7032
	ctx.r3.s64 = ctx.r11.s64 + -7032;
	// bl 0x8298ed98
	ctx.lr = 0x829EED4C;
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

PPC_FUNC_IMPL(__imp__sub_829EED68);  /* forward decl */
PPC_WEAK_FUNC(sub_829EED68) { __imp__sub_829EED68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EED68) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32015
	ctx.r11.s64 = -2098135040;
	// li r9,-1
	ctx.r9.s64 = -1;
	// addi r10,r11,-13664
	ctx.r10.s64 = ctx.r11.s64 + -13664;
	// li r11,63
	ctx.r11.s64 = 63;
	// addi r10,r10,64
	ctx.r10.s64 = ctx.r10.s64 + 64;
loc_829EED7C:
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// addi r10,r10,80
	ctx.r10.s64 = ctx.r10.s64 + 80;
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// bge cr6,0x829eed7c
	if (!ctx.cr6.lt) goto loc_829EED7C;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EED98);  /* forward decl */
PPC_WEAK_FUNC(sub_829EED98) { __imp__sub_829EED98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EED98) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6944
	ctx.r3.s64 = ctx.r11.s64 + -6944;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EEDA8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EEDA8) { __imp__sub_829EEDA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EEDA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32015
	ctx.r11.s64 = -2098135040;
	// li r9,-1
	ctx.r9.s64 = -1;
	// addi r11,r11,-3568
	ctx.r11.s64 = ctx.r11.s64 + -3568;
	// li r10,57
	ctx.r10.s64 = 57;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_829EEDBC:
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x829eedbc
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_829EEDBC;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EEDD0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EEDD0) { __imp__sub_829EEDD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EEDD0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32015
	ctx.r11.s64 = -2098135040;
	// addi r3,r11,-3120
	ctx.r3.s64 = ctx.r11.s64 + -3120;
	// bl 0x8272c310
	ctx.lr = 0x829EEDE8;
	sub_8272C310(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6896
	ctx.r3.s64 = ctx.r11.s64 + -6896;
	// bl 0x8298ed98
	ctx.lr = 0x829EEDF4;
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

PPC_FUNC_IMPL(__imp__sub_829EEE08);  /* forward decl */
PPC_WEAK_FUNC(sub_829EEE08) { __imp__sub_829EEE08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EEE08) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32015
	ctx.r11.s64 = -2098135040;
	// addi r3,r11,-3080
	ctx.r3.s64 = ctx.r11.s64 + -3080;
	// bl 0x8272a360
	ctx.lr = 0x829EEE20;
	sub_8272A360(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6880
	ctx.r3.s64 = ctx.r11.s64 + -6880;
	// bl 0x8298ed98
	ctx.lr = 0x829EEE2C;
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

PPC_FUNC_IMPL(__imp__sub_829EEE40);  /* forward decl */
PPC_WEAK_FUNC(sub_829EEE40) { __imp__sub_829EEE40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EEE40) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6864
	ctx.r3.s64 = ctx.r11.s64 + -6864;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EEE50);  /* forward decl */
PPC_WEAK_FUNC(sub_829EEE50) { __imp__sub_829EEE50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EEE50) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32015
	ctx.r11.s64 = -2098135040;
	// addi r31,r11,19744
	ctx.r31.s64 = ctx.r11.s64 + 19744;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82893a58
	ctx.lr = 0x829EEE70;
	sub_82893A58(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-30436
	ctx.r11.s64 = ctx.r11.s64 + -30436;
	// addi r3,r10,-6832
	ctx.r3.s64 = ctx.r10.s64 + -6832;
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
	// bl 0x8298ed98
	ctx.lr = 0x829EEE98;
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

PPC_FUNC_IMPL(__imp__sub_829EEEB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EEEB0) { __imp__sub_829EEEB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EEEB0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r3,r11,-28864
	ctx.r3.s64 = ctx.r11.s64 + -28864;
	// bl 0x827df490
	ctx.lr = 0x829EEED0;
	sub_827DF490(ctx, base);
	// lis r10,-32014
	ctx.r10.s64 = -2098069504;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// addi r31,r10,-31712
	ctx.r31.s64 = ctx.r10.s64 + -31712;
	// addi r11,r11,-28888
	ctx.r11.s64 = ctx.r11.s64 + -28888;
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829EEEF0;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-28916
	ctx.r11.s64 = ctx.r11.s64 + -28916;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829EEF08;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-28936
	ctx.r11.s64 = ctx.r11.s64 + -28936;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829EEF20;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-28964
	ctx.r11.s64 = ctx.r11.s64 + -28964;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829EEF38;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29544
	ctx.r11.s64 = ctx.r11.s64 + -29544;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829EEF50;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,20(r31)
	PPC_STORE_U32(ctx.r31.u32 + 20, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29560
	ctx.r11.s64 = ctx.r11.s64 + -29560;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829EEF68;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,24(r31)
	PPC_STORE_U32(ctx.r31.u32 + 24, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29496
	ctx.r11.s64 = ctx.r11.s64 + -29496;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829EEF80;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,28(r31)
	PPC_STORE_U32(ctx.r31.u32 + 28, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29580
	ctx.r11.s64 = ctx.r11.s64 + -29580;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829EEF98;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,32(r31)
	PPC_STORE_U32(ctx.r31.u32 + 32, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29528
	ctx.r11.s64 = ctx.r11.s64 + -29528;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829EEFB0;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,36(r31)
	PPC_STORE_U32(ctx.r31.u32 + 36, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-28980
	ctx.r11.s64 = ctx.r11.s64 + -28980;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829EEFC8;
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
	ctx.lr = 0x829EEFE0;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,44(r31)
	PPC_STORE_U32(ctx.r31.u32 + 44, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29020
	ctx.r11.s64 = ctx.r11.s64 + -29020;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829EEFF8;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,48(r31)
	PPC_STORE_U32(ctx.r31.u32 + 48, ctx.r3.u32);
	// addi r11,r11,-29044
	ctx.r11.s64 = ctx.r11.s64 + -29044;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829EF010;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,52(r31)
	PPC_STORE_U32(ctx.r31.u32 + 52, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29068
	ctx.r11.s64 = ctx.r11.s64 + -29068;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829EF028;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,56(r31)
	PPC_STORE_U32(ctx.r31.u32 + 56, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29104
	ctx.r11.s64 = ctx.r11.s64 + -29104;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829EF040;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,60(r31)
	PPC_STORE_U32(ctx.r31.u32 + 60, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-29124
	ctx.r3.s64 = ctx.r11.s64 + -29124;
	// bl 0x827df490
	ctx.lr = 0x829EF054;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,64(r31)
	PPC_STORE_U32(ctx.r31.u32 + 64, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29144
	ctx.r11.s64 = ctx.r11.s64 + -29144;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829EF06C;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,68(r31)
	PPC_STORE_U32(ctx.r31.u32 + 68, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29164
	ctx.r11.s64 = ctx.r11.s64 + -29164;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829EF084;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,72(r31)
	PPC_STORE_U32(ctx.r31.u32 + 72, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29180
	ctx.r11.s64 = ctx.r11.s64 + -29180;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829EF09C;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,76(r31)
	PPC_STORE_U32(ctx.r31.u32 + 76, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29204
	ctx.r11.s64 = ctx.r11.s64 + -29204;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829EF0B4;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,80(r31)
	PPC_STORE_U32(ctx.r31.u32 + 80, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29220
	ctx.r11.s64 = ctx.r11.s64 + -29220;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829EF0CC;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,84(r31)
	PPC_STORE_U32(ctx.r31.u32 + 84, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29236
	ctx.r11.s64 = ctx.r11.s64 + -29236;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829EF0E4;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,88(r31)
	PPC_STORE_U32(ctx.r31.u32 + 88, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29252
	ctx.r11.s64 = ctx.r11.s64 + -29252;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829EF0FC;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,92(r31)
	PPC_STORE_U32(ctx.r31.u32 + 92, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29268
	ctx.r11.s64 = ctx.r11.s64 + -29268;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829EF114;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,96(r31)
	PPC_STORE_U32(ctx.r31.u32 + 96, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29512
	ctx.r11.s64 = ctx.r11.s64 + -29512;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829EF12C;
	sub_827DF490(ctx, base);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// stw r3,100(r31)
	PPC_STORE_U32(ctx.r31.u32 + 100, ctx.r3.u32);
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r11,r11,-29288
	ctx.r11.s64 = ctx.r11.s64 + -29288;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x827df490
	ctx.lr = 0x829EF144;
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
	ctx.lr = 0x829EF15C;
	sub_827DF490(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829EF178);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF178) { __imp__sub_829EF178(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF178) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-28616
	ctx.r5.s64 = ctx.r11.s64 + -28616;
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-31364
	ctx.r3.s64 = ctx.r11.s64 + -31364;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF198);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF198) { __imp__sub_829EF198(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF198) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-30736
	ctx.r3.s64 = ctx.r11.s64 + -30736;
	// bl 0x823f8408
	ctx.lr = 0x829EF1B0;
	sub_823F8408(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6736
	ctx.r3.s64 = ctx.r11.s64 + -6736;
	// bl 0x8298ed98
	ctx.lr = 0x829EF1BC;
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

PPC_FUNC_IMPL(__imp__sub_829EF1D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF1D0) { __imp__sub_829EF1D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF1D0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-30400
	ctx.r3.s64 = ctx.r11.s64 + -30400;
	// bl 0x82409340
	ctx.lr = 0x829EF1E8;
	sub_82409340(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6728
	ctx.r3.s64 = ctx.r11.s64 + -6728;
	// bl 0x8298ed98
	ctx.lr = 0x829EF1F4;
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

PPC_FUNC_IMPL(__imp__sub_829EF208);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF208) { __imp__sub_829EF208(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF208) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24860
	ctx.r3.s64 = ctx.r11.s64 + -24860;
	// bl 0x827df490
	ctx.lr = 0x829EF224;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29712(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29712, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF240);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF240) { __imp__sub_829EF240(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF240) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24840
	ctx.r3.s64 = ctx.r11.s64 + -24840;
	// bl 0x827df490
	ctx.lr = 0x829EF25C;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29708(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29708, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF278);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF278) { __imp__sub_829EF278(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF278) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24820
	ctx.r3.s64 = ctx.r11.s64 + -24820;
	// bl 0x827df490
	ctx.lr = 0x829EF294;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29704(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29704, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF2B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF2B0) { __imp__sub_829EF2B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF2B0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,16888
	ctx.r3.s64 = ctx.r11.s64 + 16888;
	// bl 0x827df490
	ctx.lr = 0x829EF2CC;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29700(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29700, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF2E8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF2E8) { __imp__sub_829EF2E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF2E8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25524
	ctx.r3.s64 = ctx.r11.s64 + -25524;
	// bl 0x827df490
	ctx.lr = 0x829EF304;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29696(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29696, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF320);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF320) { __imp__sub_829EF320(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF320) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,21616
	ctx.r3.s64 = ctx.r11.s64 + 21616;
	// bl 0x827df490
	ctx.lr = 0x829EF33C;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29692(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29692, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF358);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF358) { __imp__sub_829EF358(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF358) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,5416
	ctx.r3.s64 = ctx.r11.s64 + 5416;
	// bl 0x827df490
	ctx.lr = 0x829EF374;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29688(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29688, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF390);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF390) { __imp__sub_829EF390(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF390) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24912
	ctx.r3.s64 = ctx.r11.s64 + -24912;
	// bl 0x827df490
	ctx.lr = 0x829EF3AC;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29684(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29684, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF3C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF3C8) { __imp__sub_829EF3C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF3C8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,23028
	ctx.r3.s64 = ctx.r11.s64 + 23028;
	// bl 0x827df490
	ctx.lr = 0x829EF3E4;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29680(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29680, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF400);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF400) { __imp__sub_829EF400(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF400) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24808
	ctx.r3.s64 = ctx.r11.s64 + -24808;
	// bl 0x827df490
	ctx.lr = 0x829EF41C;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29676(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29676, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF438);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF438) { __imp__sub_829EF438(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF438) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24924
	ctx.r3.s64 = ctx.r11.s64 + -24924;
	// bl 0x827df490
	ctx.lr = 0x829EF454;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29672(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29672, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF470);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF470) { __imp__sub_829EF470(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF470) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,16880
	ctx.r3.s64 = ctx.r11.s64 + 16880;
	// bl 0x827df490
	ctx.lr = 0x829EF48C;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29668(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29668, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF4A8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF4A8) { __imp__sub_829EF4A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF4A8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24796
	ctx.r3.s64 = ctx.r11.s64 + -24796;
	// bl 0x827df490
	ctx.lr = 0x829EF4C4;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29664(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29664, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF4E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF4E0) { __imp__sub_829EF4E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF4E0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24788
	ctx.r3.s64 = ctx.r11.s64 + -24788;
	// bl 0x827df490
	ctx.lr = 0x829EF4FC;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29660(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29660, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF518);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF518) { __imp__sub_829EF518(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF518) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24776
	ctx.r3.s64 = ctx.r11.s64 + -24776;
	// bl 0x827df490
	ctx.lr = 0x829EF534;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29656(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29656, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF550);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF550) { __imp__sub_829EF550(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF550) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24764
	ctx.r3.s64 = ctx.r11.s64 + -24764;
	// bl 0x827df490
	ctx.lr = 0x829EF56C;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29652(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29652, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF588);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF588) { __imp__sub_829EF588(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF588) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24756
	ctx.r3.s64 = ctx.r11.s64 + -24756;
	// bl 0x827df490
	ctx.lr = 0x829EF5A4;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29648(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29648, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF5C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF5C0) { __imp__sub_829EF5C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF5C0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24740
	ctx.r3.s64 = ctx.r11.s64 + -24740;
	// bl 0x827df490
	ctx.lr = 0x829EF5DC;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29644(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29644, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF5F8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF5F8) { __imp__sub_829EF5F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF5F8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24728
	ctx.r3.s64 = ctx.r11.s64 + -24728;
	// bl 0x827df490
	ctx.lr = 0x829EF614;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29640(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29640, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF630);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF630) { __imp__sub_829EF630(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF630) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24716
	ctx.r3.s64 = ctx.r11.s64 + -24716;
	// bl 0x827df490
	ctx.lr = 0x829EF64C;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29636(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29636, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF668);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF668) { __imp__sub_829EF668(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF668) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24700
	ctx.r3.s64 = ctx.r11.s64 + -24700;
	// bl 0x827df490
	ctx.lr = 0x829EF684;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29632(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29632, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF6A0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF6A0) { __imp__sub_829EF6A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF6A0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24692
	ctx.r3.s64 = ctx.r11.s64 + -24692;
	// bl 0x827df490
	ctx.lr = 0x829EF6BC;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29628(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29628, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF6D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF6D8) { __imp__sub_829EF6D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF6D8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24680
	ctx.r3.s64 = ctx.r11.s64 + -24680;
	// bl 0x827df490
	ctx.lr = 0x829EF6F4;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29624(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29624, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF710);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF710) { __imp__sub_829EF710(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF710) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-23216
	ctx.r3.s64 = ctx.r11.s64 + -23216;
	// bl 0x827df490
	ctx.lr = 0x829EF72C;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29620(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29620, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF748);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF748) { __imp__sub_829EF748(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF748) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24668
	ctx.r3.s64 = ctx.r11.s64 + -24668;
	// bl 0x827df490
	ctx.lr = 0x829EF764;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29616(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29616, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF780);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF780) { __imp__sub_829EF780(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF780) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24644
	ctx.r3.s64 = ctx.r11.s64 + -24644;
	// bl 0x827df490
	ctx.lr = 0x829EF79C;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29612(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29612, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF7B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF7B8) { __imp__sub_829EF7B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF7B8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24616
	ctx.r3.s64 = ctx.r11.s64 + -24616;
	// bl 0x827df490
	ctx.lr = 0x829EF7D4;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29608(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29608, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF7F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF7F0) { __imp__sub_829EF7F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF7F0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24588
	ctx.r3.s64 = ctx.r11.s64 + -24588;
	// bl 0x827df490
	ctx.lr = 0x829EF80C;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29604(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29604, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF828);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF828) { __imp__sub_829EF828(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF828) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-10080
	ctx.r3.s64 = ctx.r11.s64 + -10080;
	// bl 0x827df490
	ctx.lr = 0x829EF844;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29600(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29600, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF860);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF860) { __imp__sub_829EF860(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF860) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-10180
	ctx.r3.s64 = ctx.r11.s64 + -10180;
	// bl 0x827df490
	ctx.lr = 0x829EF87C;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29596(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29596, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF898);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF898) { __imp__sub_829EF898(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF898) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-13548
	ctx.r3.s64 = ctx.r11.s64 + -13548;
	// bl 0x827df490
	ctx.lr = 0x829EF8B4;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29592(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29592, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF8D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF8D0) { __imp__sub_829EF8D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF8D0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-13520
	ctx.r3.s64 = ctx.r11.s64 + -13520;
	// bl 0x827df490
	ctx.lr = 0x829EF8EC;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29588(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29588, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF908);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF908) { __imp__sub_829EF908(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF908) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24940
	ctx.r3.s64 = ctx.r11.s64 + -24940;
	// bl 0x827df490
	ctx.lr = 0x829EF924;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29584(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29584, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF940);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF940) { __imp__sub_829EF940(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF940) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24560
	ctx.r3.s64 = ctx.r11.s64 + -24560;
	// bl 0x827df490
	ctx.lr = 0x829EF95C;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29580(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29580, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF978);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF978) { __imp__sub_829EF978(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF978) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24904
	ctx.r3.s64 = ctx.r11.s64 + -24904;
	// bl 0x827df490
	ctx.lr = 0x829EF994;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29576(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29576, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF9B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF9B0) { __imp__sub_829EF9B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF9B0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,10208
	ctx.r3.s64 = ctx.r11.s64 + 10208;
	// bl 0x827df490
	ctx.lr = 0x829EF9CC;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29572(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29572, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EF9E8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EF9E8) { __imp__sub_829EF9E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EF9E8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24540
	ctx.r3.s64 = ctx.r11.s64 + -24540;
	// bl 0x827df490
	ctx.lr = 0x829EFA04;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29568(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29568, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EFA20);  /* forward decl */
PPC_WEAK_FUNC(sub_829EFA20) { __imp__sub_829EFA20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EFA20) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,21648
	ctx.r3.s64 = ctx.r11.s64 + 21648;
	// bl 0x827df490
	ctx.lr = 0x829EFA3C;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29564(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29564, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EFA58);  /* forward decl */
PPC_WEAK_FUNC(sub_829EFA58) { __imp__sub_829EFA58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EFA58) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,21600
	ctx.r3.s64 = ctx.r11.s64 + 21600;
	// bl 0x827df490
	ctx.lr = 0x829EFA74;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29560(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29560, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EFA90);  /* forward decl */
PPC_WEAK_FUNC(sub_829EFA90) { __imp__sub_829EFA90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EFA90) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24952
	ctx.r3.s64 = ctx.r11.s64 + -24952;
	// bl 0x827df490
	ctx.lr = 0x829EFAAC;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29556(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29556, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EFAC8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EFAC8) { __imp__sub_829EFAC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EFAC8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24968
	ctx.r3.s64 = ctx.r11.s64 + -24968;
	// bl 0x827df490
	ctx.lr = 0x829EFAE4;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29552(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29552, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EFB00);  /* forward decl */
PPC_WEAK_FUNC(sub_829EFB00) { __imp__sub_829EFB00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EFB00) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,21528
	ctx.r3.s64 = ctx.r11.s64 + 21528;
	// bl 0x827df490
	ctx.lr = 0x829EFB1C;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29548(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29548, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EFB38);  /* forward decl */
PPC_WEAK_FUNC(sub_829EFB38) { __imp__sub_829EFB38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EFB38) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24976
	ctx.r3.s64 = ctx.r11.s64 + -24976;
	// bl 0x827df490
	ctx.lr = 0x829EFB54;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29544(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29544, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EFB70);  /* forward decl */
PPC_WEAK_FUNC(sub_829EFB70) { __imp__sub_829EFB70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EFB70) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24992
	ctx.r3.s64 = ctx.r11.s64 + -24992;
	// bl 0x827df490
	ctx.lr = 0x829EFB8C;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29540(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29540, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EFBA8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EFBA8) { __imp__sub_829EFBA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EFBA8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24520
	ctx.r3.s64 = ctx.r11.s64 + -24520;
	// bl 0x827df490
	ctx.lr = 0x829EFBC4;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29536(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29536, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EFBE0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EFBE0) { __imp__sub_829EFBE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EFBE0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-10216
	ctx.r3.s64 = ctx.r11.s64 + -10216;
	// bl 0x827df490
	ctx.lr = 0x829EFBFC;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29532(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29532, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EFC18);  /* forward decl */
PPC_WEAK_FUNC(sub_829EFC18) { __imp__sub_829EFC18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EFC18) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24512
	ctx.r3.s64 = ctx.r11.s64 + -24512;
	// bl 0x827df490
	ctx.lr = 0x829EFC34;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29528(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29528, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EFC50);  /* forward decl */
PPC_WEAK_FUNC(sub_829EFC50) { __imp__sub_829EFC50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EFC50) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24500
	ctx.r3.s64 = ctx.r11.s64 + -24500;
	// bl 0x827df490
	ctx.lr = 0x829EFC6C;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29524(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29524, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EFC88);  /* forward decl */
PPC_WEAK_FUNC(sub_829EFC88) { __imp__sub_829EFC88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EFC88) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24488
	ctx.r3.s64 = ctx.r11.s64 + -24488;
	// bl 0x827df490
	ctx.lr = 0x829EFCA4;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29520(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29520, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EFCC0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EFCC0) { __imp__sub_829EFCC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EFCC0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-24468
	ctx.r3.s64 = ctx.r11.s64 + -24468;
	// bl 0x827df490
	ctx.lr = 0x829EFCDC;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29516(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29516, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EFCF8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EFCF8) { __imp__sub_829EFCF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EFCF8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25448
	ctx.r3.s64 = ctx.r11.s64 + -25448;
	// bl 0x827df490
	ctx.lr = 0x829EFD14;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29512(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29512, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EFD30);  /* forward decl */
PPC_WEAK_FUNC(sub_829EFD30) { __imp__sub_829EFD30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EFD30) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25468
	ctx.r3.s64 = ctx.r11.s64 + -25468;
	// bl 0x827df490
	ctx.lr = 0x829EFD4C;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29508(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29508, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EFD68);  /* forward decl */
PPC_WEAK_FUNC(sub_829EFD68) { __imp__sub_829EFD68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EFD68) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25408
	ctx.r3.s64 = ctx.r11.s64 + -25408;
	// bl 0x827df490
	ctx.lr = 0x829EFD84;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29504(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29504, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EFDA0);  /* forward decl */
PPC_WEAK_FUNC(sub_829EFDA0) { __imp__sub_829EFDA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EFDA0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25436
	ctx.r3.s64 = ctx.r11.s64 + -25436;
	// bl 0x827df490
	ctx.lr = 0x829EFDBC;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29500(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29500, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EFDD8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EFDD8) { __imp__sub_829EFDD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EFDD8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,24620
	ctx.r3.s64 = ctx.r11.s64 + 24620;
	// bl 0x827df490
	ctx.lr = 0x829EFDF4;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29496(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29496, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EFE10);  /* forward decl */
PPC_WEAK_FUNC(sub_829EFE10) { __imp__sub_829EFE10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EFE10) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-25176
	ctx.r3.s64 = ctx.r11.s64 + -25176;
	// bl 0x827df490
	ctx.lr = 0x829EFE2C;
	sub_827DF490(ctx, base);
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// stw r3,-29492(r11)
	PPC_STORE_U32(ctx.r11.u32 + -29492, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829EFE48);  /* forward decl */
PPC_WEAK_FUNC(sub_829EFE48) { __imp__sub_829EFE48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EFE48) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-29488
	ctx.r3.s64 = ctx.r11.s64 + -29488;
	// bl 0x8289df08
	ctx.lr = 0x829EFE60;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6712
	ctx.r3.s64 = ctx.r11.s64 + -6712;
	// bl 0x8298ed98
	ctx.lr = 0x829EFE6C;
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

PPC_FUNC_IMPL(__imp__sub_829EFE80);  /* forward decl */
PPC_WEAK_FUNC(sub_829EFE80) { __imp__sub_829EFE80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EFE80) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-29448
	ctx.r3.s64 = ctx.r11.s64 + -29448;
	// bl 0x8289df08
	ctx.lr = 0x829EFE98;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6704
	ctx.r3.s64 = ctx.r11.s64 + -6704;
	// bl 0x8298ed98
	ctx.lr = 0x829EFEA4;
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

PPC_FUNC_IMPL(__imp__sub_829EFEB8);  /* forward decl */
PPC_WEAK_FUNC(sub_829EFEB8) { __imp__sub_829EFEB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829EFEB8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-29408
	ctx.r3.s64 = ctx.r11.s64 + -29408;
	// bl 0x8289df08
	ctx.lr = 0x829EFED0;
	sub_8289DF08(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,-6696
	ctx.r3.s64 = ctx.r11.s64 + -6696;
	// bl 0x8298ed98
	ctx.lr = 0x829EFEDC;
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

