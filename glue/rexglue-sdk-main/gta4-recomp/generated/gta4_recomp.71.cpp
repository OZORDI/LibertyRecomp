#include "gta4_init.h"

PPC_FUNC_IMPL(__imp__sub_829F84D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F84D8) { __imp__sub_829F84D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F84D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r5,r11,2396
	ctx.r5.s64 = ctx.r11.s64 + 2396;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,12248
	ctx.r31.s64 = ctx.r11.s64 + 12248;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F850C;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F851C;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F8524;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,2392
	ctx.r11.s64 = ctx.r11.s64 + 2392;
	// addi r3,r10,1168
	ctx.r3.s64 = ctx.r10.s64 + 1168;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F853C;
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

PPC_FUNC_IMPL(__imp__sub_829F8550);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8550) { __imp__sub_829F8550(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8550) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r5,r11,2424
	ctx.r5.s64 = ctx.r11.s64 + 2424;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,11832
	ctx.r31.s64 = ctx.r11.s64 + 11832;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F8584;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F8594;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F859C;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,2420
	ctx.r11.s64 = ctx.r11.s64 + 2420;
	// addi r3,r10,1248
	ctx.r3.s64 = ctx.r10.s64 + 1248;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F85B4;
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

PPC_FUNC_IMPL(__imp__sub_829F85C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F85C8) { __imp__sub_829F85C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F85C8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r5,r11,2444
	ctx.r5.s64 = ctx.r11.s64 + 2444;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,12216
	ctx.r31.s64 = ctx.r11.s64 + 12216;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F85FC;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F860C;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F8614;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,2440
	ctx.r11.s64 = ctx.r11.s64 + 2440;
	// addi r3,r10,1328
	ctx.r3.s64 = ctx.r10.s64 + 1328;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F862C;
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

PPC_FUNC_IMPL(__imp__sub_829F8640);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8640) { __imp__sub_829F8640(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8640) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r5,r11,2472
	ctx.r5.s64 = ctx.r11.s64 + 2472;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,12152
	ctx.r31.s64 = ctx.r11.s64 + 12152;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F8674;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F8684;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F868C;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,2468
	ctx.r11.s64 = ctx.r11.s64 + 2468;
	// addi r3,r10,1408
	ctx.r3.s64 = ctx.r10.s64 + 1408;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F86A4;
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

PPC_FUNC_IMPL(__imp__sub_829F86B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F86B8) { __imp__sub_829F86B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F86B8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r5,r11,2500
	ctx.r5.s64 = ctx.r11.s64 + 2500;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,11896
	ctx.r31.s64 = ctx.r11.s64 + 11896;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F86EC;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F86FC;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F8704;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,2496
	ctx.r11.s64 = ctx.r11.s64 + 2496;
	// addi r3,r10,1488
	ctx.r3.s64 = ctx.r10.s64 + 1488;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F871C;
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

PPC_FUNC_IMPL(__imp__sub_829F8730);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8730) { __imp__sub_829F8730(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8730) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r5,r11,2536
	ctx.r5.s64 = ctx.r11.s64 + 2536;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,12056
	ctx.r31.s64 = ctx.r11.s64 + 12056;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F8764;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F8774;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F877C;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,2532
	ctx.r11.s64 = ctx.r11.s64 + 2532;
	// addi r3,r10,1568
	ctx.r3.s64 = ctx.r10.s64 + 1568;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F8794;
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

PPC_FUNC_IMPL(__imp__sub_829F87A8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F87A8) { __imp__sub_829F87A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F87A8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r5,r11,2724
	ctx.r5.s64 = ctx.r11.s64 + 2724;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,11768
	ctx.r31.s64 = ctx.r11.s64 + 11768;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F87DC;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F87EC;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F87F4;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,2720
	ctx.r11.s64 = ctx.r11.s64 + 2720;
	// addi r3,r10,1648
	ctx.r3.s64 = ctx.r10.s64 + 1648;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F880C;
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

PPC_FUNC_IMPL(__imp__sub_829F8820);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8820) { __imp__sub_829F8820(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8820) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r5,r11,2576
	ctx.r5.s64 = ctx.r11.s64 + 2576;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,11928
	ctx.r31.s64 = ctx.r11.s64 + 11928;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F8854;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F8864;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F886C;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,2572
	ctx.r11.s64 = ctx.r11.s64 + 2572;
	// addi r3,r10,1728
	ctx.r3.s64 = ctx.r10.s64 + 1728;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F8884;
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

PPC_FUNC_IMPL(__imp__sub_829F8898);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8898) { __imp__sub_829F8898(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8898) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r5,r11,2608
	ctx.r5.s64 = ctx.r11.s64 + 2608;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,12280
	ctx.r31.s64 = ctx.r11.s64 + 12280;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F88CC;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F88DC;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F88E4;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,2604
	ctx.r11.s64 = ctx.r11.s64 + 2604;
	// addi r3,r10,1808
	ctx.r3.s64 = ctx.r10.s64 + 1808;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F88FC;
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

PPC_FUNC_IMPL(__imp__sub_829F8910);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8910) { __imp__sub_829F8910(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8910) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r5,r11,2644
	ctx.r5.s64 = ctx.r11.s64 + 2644;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,12088
	ctx.r31.s64 = ctx.r11.s64 + 12088;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F8944;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F8954;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F895C;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,2640
	ctx.r11.s64 = ctx.r11.s64 + 2640;
	// addi r3,r10,1888
	ctx.r3.s64 = ctx.r10.s64 + 1888;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F8974;
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

PPC_FUNC_IMPL(__imp__sub_829F8988);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8988) { __imp__sub_829F8988(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8988) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r5,r11,2684
	ctx.r5.s64 = ctx.r11.s64 + 2684;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,11864
	ctx.r31.s64 = ctx.r11.s64 + 11864;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F89BC;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F89CC;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F89D4;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,2680
	ctx.r11.s64 = ctx.r11.s64 + 2680;
	// addi r3,r10,1968
	ctx.r3.s64 = ctx.r10.s64 + 1968;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F89EC;
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

PPC_FUNC_IMPL(__imp__sub_829F8A00);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8A00) { __imp__sub_829F8A00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8A00) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r5,r11,2752
	ctx.r5.s64 = ctx.r11.s64 + 2752;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,11992
	ctx.r31.s64 = ctx.r11.s64 + 11992;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F8A34;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F8A44;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F8A4C;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,2748
	ctx.r11.s64 = ctx.r11.s64 + 2748;
	// addi r3,r10,2048
	ctx.r3.s64 = ctx.r10.s64 + 2048;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F8A64;
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

PPC_FUNC_IMPL(__imp__sub_829F8A78);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8A78) { __imp__sub_829F8A78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8A78) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r5,r11,2788
	ctx.r5.s64 = ctx.r11.s64 + 2788;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,11800
	ctx.r31.s64 = ctx.r11.s64 + 11800;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F8AAC;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F8ABC;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F8AC4;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,2784
	ctx.r11.s64 = ctx.r11.s64 + 2784;
	// addi r3,r10,2128
	ctx.r3.s64 = ctx.r10.s64 + 2128;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F8ADC;
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

PPC_FUNC_IMPL(__imp__sub_829F8AF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8AF0) { __imp__sub_829F8AF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8AF0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r5,r11,2824
	ctx.r5.s64 = ctx.r11.s64 + 2824;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,12184
	ctx.r31.s64 = ctx.r11.s64 + 12184;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F8B24;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F8B34;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F8B3C;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,2820
	ctx.r11.s64 = ctx.r11.s64 + 2820;
	// addi r3,r10,2208
	ctx.r3.s64 = ctx.r10.s64 + 2208;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F8B54;
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

PPC_FUNC_IMPL(__imp__sub_829F8B68);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8B68) { __imp__sub_829F8B68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8B68) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r5,r11,2864
	ctx.r5.s64 = ctx.r11.s64 + 2864;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,12024
	ctx.r31.s64 = ctx.r11.s64 + 12024;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F8B9C;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F8BAC;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F8BB4;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,2860
	ctx.r11.s64 = ctx.r11.s64 + 2860;
	// addi r3,r10,2288
	ctx.r3.s64 = ctx.r10.s64 + 2288;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F8BCC;
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

PPC_FUNC_IMPL(__imp__sub_829F8BE0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8BE0) { __imp__sub_829F8BE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8BE0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// addi r5,r11,2896
	ctx.r5.s64 = ctx.r11.s64 + 2896;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r31,r11,11960
	ctx.r31.s64 = ctx.r11.s64 + 11960;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829F8C14;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829F8C24;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829F8C2C;
	sub_8296C788(ctx, base);
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,2892
	ctx.r11.s64 = ctx.r11.s64 + 2892;
	// addi r3,r10,2368
	ctx.r3.s64 = ctx.r10.s64 + 2368;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829F8C44;
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

PPC_FUNC_IMPL(__imp__sub_829F8C58);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8C58) { __imp__sub_829F8C58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8C58) {
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
	// addi r31,r11,12320
	ctx.r31.s64 = ctx.r11.s64 + 12320;
loc_829F8C78:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82974110
	ctx.lr = 0x829F8C80;
	sub_82974110(ctx, base);
	// addi r3,r31,36
	ctx.r3.s64 = ctx.r31.s64 + 36;
	// bl 0x82989be8
	ctx.lr = 0x829F8C88;
	sub_82989BE8(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// addi r31,r31,112
	ctx.r31.s64 = ctx.r31.s64 + 112;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829f8c78
	if (!ctx.cr6.lt) goto loc_829F8C78;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,2448
	ctx.r3.s64 = ctx.r11.s64 + 2448;
	// bl 0x8298ed98
	ctx.lr = 0x829F8CA4;
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

PPC_FUNC_IMPL(__imp__sub_829F8CC0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8CC0) { __imp__sub_829F8CC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8CC0) {
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
	// lis r11,-32134
	ctx.r11.s64 = -2105933824;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-20320
	ctx.r5.s64 = ctx.r11.s64 + -20320;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829F8CE8;
	sub_827DB118(ctx, base);
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// lis r9,-32137
	ctx.r9.s64 = -2106130432;
	// addi r10,r11,-9548
	ctx.r10.s64 = ctx.r11.s64 + -9548;
	// addi r9,r9,-19072
	ctx.r9.s64 = ctx.r9.s64 + -19072;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
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

PPC_FUNC_IMPL(__imp__sub_829F8D38);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8D38) { __imp__sub_829F8D38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8D38) {
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
	// lis r11,-32135
	ctx.r11.s64 = -2105999360;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-15424
	ctx.r5.s64 = ctx.r11.s64 + -15424;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829F8D60;
	sub_827DB118(ctx, base);
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// lis r9,-32137
	ctx.r9.s64 = -2106130432;
	// addi r10,r11,-9500
	ctx.r10.s64 = ctx.r11.s64 + -9500;
	// addi r9,r9,-19072
	ctx.r9.s64 = ctx.r9.s64 + -19072;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
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

PPC_FUNC_IMPL(__imp__sub_829F8DB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8DB0) { __imp__sub_829F8DB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8DB0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-9356
	ctx.r11.s64 = ctx.r11.s64 + -9356;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F8DD0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8DD0) { __imp__sub_829F8DD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8DD0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-9348
	ctx.r11.s64 = ctx.r11.s64 + -9348;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F8DF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8DF0) { __imp__sub_829F8DF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8DF0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-9340
	ctx.r11.s64 = ctx.r11.s64 + -9340;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F8E10);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8E10) { __imp__sub_829F8E10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8E10) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-9332
	ctx.r11.s64 = ctx.r11.s64 + -9332;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F8E30);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8E30) { __imp__sub_829F8E30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8E30) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-9324
	ctx.r11.s64 = ctx.r11.s64 + -9324;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F8E50);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8E50) { __imp__sub_829F8E50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8E50) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-9316
	ctx.r11.s64 = ctx.r11.s64 + -9316;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F8E70);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8E70) { __imp__sub_829F8E70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8E70) {
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
	// lis r11,-32135
	ctx.r11.s64 = -2105999360;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-15424
	ctx.r5.s64 = ctx.r11.s64 + -15424;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829F8E98;
	sub_827DB118(ctx, base);
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// lis r9,-32137
	ctx.r9.s64 = -2106130432;
	// addi r10,r11,-9104
	ctx.r10.s64 = ctx.r11.s64 + -9104;
	// addi r9,r9,-19072
	ctx.r9.s64 = ctx.r9.s64 + -19072;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
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

PPC_FUNC_IMPL(__imp__sub_829F8EE8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8EE8) { __imp__sub_829F8EE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8EE8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-9056
	ctx.r11.s64 = ctx.r11.s64 + -9056;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F8F08);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8F08) { __imp__sub_829F8F08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8F08) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-8468
	ctx.r11.s64 = ctx.r11.s64 + -8468;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F8F28);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8F28) { __imp__sub_829F8F28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8F28) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-8460
	ctx.r11.s64 = ctx.r11.s64 + -8460;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F8F48);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8F48) { __imp__sub_829F8F48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8F48) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-8452
	ctx.r11.s64 = ctx.r11.s64 + -8452;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F8F68);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8F68) { __imp__sub_829F8F68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8F68) {
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
	// lis r11,-32135
	ctx.r11.s64 = -2105999360;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-15424
	ctx.r5.s64 = ctx.r11.s64 + -15424;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829F8F90;
	sub_827DB118(ctx, base);
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// lis r9,-32137
	ctx.r9.s64 = -2106130432;
	// addi r10,r11,-6888
	ctx.r10.s64 = ctx.r11.s64 + -6888;
	// addi r9,r9,-19072
	ctx.r9.s64 = ctx.r9.s64 + -19072;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
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

PPC_FUNC_IMPL(__imp__sub_829F8FE0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F8FE0) { __imp__sub_829F8FE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F8FE0) {
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
	// lis r11,-32134
	ctx.r11.s64 = -2105933824;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-3416
	ctx.r5.s64 = ctx.r11.s64 + -3416;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829F9008;
	sub_827DB118(ctx, base);
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// lis r9,-32137
	ctx.r9.s64 = -2106130432;
	// addi r10,r11,-6840
	ctx.r10.s64 = ctx.r11.s64 + -6840;
	// addi r9,r9,-19072
	ctx.r9.s64 = ctx.r9.s64 + -19072;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
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

PPC_FUNC_IMPL(__imp__sub_829F9058);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9058) { __imp__sub_829F9058(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9058) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-6792
	ctx.r11.s64 = ctx.r11.s64 + -6792;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9078);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9078) { __imp__sub_829F9078(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9078) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-6784
	ctx.r11.s64 = ctx.r11.s64 + -6784;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9098);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9098) { __imp__sub_829F9098(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9098) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-6776
	ctx.r11.s64 = ctx.r11.s64 + -6776;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F90B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F90B8) { __imp__sub_829F90B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F90B8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-6768
	ctx.r11.s64 = ctx.r11.s64 + -6768;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F90D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F90D8) { __imp__sub_829F90D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F90D8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-6760
	ctx.r11.s64 = ctx.r11.s64 + -6760;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F90F8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F90F8) { __imp__sub_829F90F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F90F8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-3100
	ctx.r11.s64 = ctx.r11.s64 + -3100;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9118);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9118) { __imp__sub_829F9118(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9118) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-3092
	ctx.r11.s64 = ctx.r11.s64 + -3092;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9138);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9138) { __imp__sub_829F9138(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9138) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-3084
	ctx.r11.s64 = ctx.r11.s64 + -3084;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9158);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9158) { __imp__sub_829F9158(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9158) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-3076
	ctx.r11.s64 = ctx.r11.s64 + -3076;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9178);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9178) { __imp__sub_829F9178(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9178) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-3068
	ctx.r11.s64 = ctx.r11.s64 + -3068;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9198);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9198) { __imp__sub_829F9198(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9198) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-3060
	ctx.r11.s64 = ctx.r11.s64 + -3060;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F91B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F91B8) { __imp__sub_829F91B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F91B8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-3052
	ctx.r11.s64 = ctx.r11.s64 + -3052;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F91D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F91D8) { __imp__sub_829F91D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F91D8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-3044
	ctx.r11.s64 = ctx.r11.s64 + -3044;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F91F8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F91F8) { __imp__sub_829F91F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F91F8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-3036
	ctx.r11.s64 = ctx.r11.s64 + -3036;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9218);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9218) { __imp__sub_829F9218(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9218) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-2772
	ctx.r11.s64 = ctx.r11.s64 + -2772;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9238);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9238) { __imp__sub_829F9238(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9238) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,2536
	ctx.r3.s64 = ctx.r11.s64 + 2536;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9248);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9248) { __imp__sub_829F9248(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9248) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r9,-32249
	ctx.r9.s64 = -2113470464;
	// lis r10,-32131
	ctx.r10.s64 = -2105737216;
	// addi r5,r9,13324
	ctx.r5.s64 = ctx.r9.s64 + 13324;
	// lis r11,-32131
	ctx.r11.s64 = -2105737216;
	// lis r9,-31985
	ctx.r9.s64 = -2096168960;
	// addi r7,r10,-28040
	ctx.r7.s64 = ctx.r10.s64 + -28040;
	// addi r3,r9,20316
	ctx.r3.s64 = ctx.r9.s64 + 20316;
	// addi r6,r11,-28112
	ctx.r6.s64 = ctx.r11.s64 + -28112;
	// li r4,3
	ctx.r4.s64 = 3;
	// bl 0x827c9898
	ctx.lr = 0x829F927C;
	sub_827C9898(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,2552
	ctx.r3.s64 = ctx.r11.s64 + 2552;
	// bl 0x8298ed98
	ctx.lr = 0x829F9288;
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

PPC_FUNC_IMPL(__imp__sub_829F9298);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9298) { __imp__sub_829F9298(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9298) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r9,-32249
	ctx.r9.s64 = -2113470464;
	// lis r10,-32131
	ctx.r10.s64 = -2105737216;
	// addi r5,r9,13428
	ctx.r5.s64 = ctx.r9.s64 + 13428;
	// lis r11,-32131
	ctx.r11.s64 = -2105737216;
	// lis r9,-31985
	ctx.r9.s64 = -2096168960;
	// addi r7,r10,-26584
	ctx.r7.s64 = ctx.r10.s64 + -26584;
	// addi r3,r9,20332
	ctx.r3.s64 = ctx.r9.s64 + 20332;
	// addi r6,r11,-26704
	ctx.r6.s64 = ctx.r11.s64 + -26704;
	// li r4,4
	ctx.r4.s64 = 4;
	// bl 0x827c9898
	ctx.lr = 0x829F92CC;
	sub_827C9898(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,2568
	ctx.r3.s64 = ctx.r11.s64 + 2568;
	// bl 0x8298ed98
	ctx.lr = 0x829F92D8;
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

PPC_FUNC_IMPL(__imp__sub_829F92E8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F92E8) { __imp__sub_829F92E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F92E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,2584
	ctx.r3.s64 = ctx.r11.s64 + 2584;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F92F8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F92F8) { __imp__sub_829F92F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F92F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,2616
	ctx.r3.s64 = ctx.r11.s64 + 2616;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9308);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9308) { __imp__sub_829F9308(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9308) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,2640
	ctx.r3.s64 = ctx.r11.s64 + 2640;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9318);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9318) { __imp__sub_829F9318(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9318) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r9,-32249
	ctx.r9.s64 = -2113470464;
	// lis r10,-32131
	ctx.r10.s64 = -2105737216;
	// addi r5,r9,15428
	ctx.r5.s64 = ctx.r9.s64 + 15428;
	// lis r11,-32131
	ctx.r11.s64 = -2105737216;
	// lis r9,-31985
	ctx.r9.s64 = -2096168960;
	// addi r7,r10,-8664
	ctx.r7.s64 = ctx.r10.s64 + -8664;
	// addi r3,r9,20372
	ctx.r3.s64 = ctx.r9.s64 + 20372;
	// addi r6,r11,-9448
	ctx.r6.s64 = ctx.r11.s64 + -9448;
	// li r4,1
	ctx.r4.s64 = 1;
	// bl 0x827c9898
	ctx.lr = 0x829F934C;
	sub_827C9898(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,2672
	ctx.r3.s64 = ctx.r11.s64 + 2672;
	// bl 0x8298ed98
	ctx.lr = 0x829F9358;
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

PPC_FUNC_IMPL(__imp__sub_829F9368);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9368) { __imp__sub_829F9368(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9368) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r9,-32249
	ctx.r9.s64 = -2113470464;
	// lis r10,-32131
	ctx.r10.s64 = -2105737216;
	// addi r5,r9,15780
	ctx.r5.s64 = ctx.r9.s64 + 15780;
	// lis r11,-32131
	ctx.r11.s64 = -2105737216;
	// lis r9,-31985
	ctx.r9.s64 = -2096168960;
	// addi r7,r10,-6888
	ctx.r7.s64 = ctx.r10.s64 + -6888;
	// addi r3,r9,20388
	ctx.r3.s64 = ctx.r9.s64 + 20388;
	// addi r6,r11,-8328
	ctx.r6.s64 = ctx.r11.s64 + -8328;
	// li r4,2
	ctx.r4.s64 = 2;
	// bl 0x827c9898
	ctx.lr = 0x829F939C;
	sub_827C9898(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,2688
	ctx.r3.s64 = ctx.r11.s64 + 2688;
	// bl 0x8298ed98
	ctx.lr = 0x829F93A8;
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

PPC_FUNC_IMPL(__imp__sub_829F93B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F93B8) { __imp__sub_829F93B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F93B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,2760
	ctx.r3.s64 = ctx.r11.s64 + 2760;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F93C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F93C8) { __imp__sub_829F93C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F93C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,2792
	ctx.r3.s64 = ctx.r11.s64 + 2792;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F93D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F93D8) { __imp__sub_829F93D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F93D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,2824
	ctx.r3.s64 = ctx.r11.s64 + 2824;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F93E8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F93E8) { __imp__sub_829F93E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F93E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,2704
	ctx.r3.s64 = ctx.r11.s64 + 2704;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F93F8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F93F8) { __imp__sub_829F93F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F93F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,2872
	ctx.r3.s64 = ctx.r11.s64 + 2872;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9408);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9408) { __imp__sub_829F9408(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9408) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,18860
	ctx.r5.s64 = ctx.r11.s64 + 18860;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,20840
	ctx.r3.s64 = ctx.r11.s64 + 20840;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9428);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9428) { __imp__sub_829F9428(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9428) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,18876
	ctx.r5.s64 = ctx.r11.s64 + 18876;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,20780
	ctx.r3.s64 = ctx.r11.s64 + 20780;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9448);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9448) { __imp__sub_829F9448(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9448) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,18888
	ctx.r5.s64 = ctx.r11.s64 + 18888;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,20820
	ctx.r3.s64 = ctx.r11.s64 + 20820;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9468);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9468) { __imp__sub_829F9468(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9468) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,18900
	ctx.r5.s64 = ctx.r11.s64 + 18900;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,20800
	ctx.r3.s64 = ctx.r11.s64 + 20800;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9488);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9488) { __imp__sub_829F9488(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9488) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,18916
	ctx.r5.s64 = ctx.r11.s64 + 18916;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,20760
	ctx.r3.s64 = ctx.r11.s64 + 20760;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F94A8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F94A8) { __imp__sub_829F94A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F94A8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,20860
	ctx.r3.s64 = ctx.r11.s64 + 20860;
	// bl 0x827f1320
	ctx.lr = 0x829F94C0;
	sub_827F1320(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,2928
	ctx.r3.s64 = ctx.r11.s64 + 2928;
	// bl 0x8298ed98
	ctx.lr = 0x829F94CC;
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

PPC_FUNC_IMPL(__imp__sub_829F94E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F94E0) { __imp__sub_829F94E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F94E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,20896
	ctx.r3.s64 = ctx.r11.s64 + 20896;
	// bl 0x827f1320
	ctx.lr = 0x829F94F8;
	sub_827F1320(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,2944
	ctx.r3.s64 = ctx.r11.s64 + 2944;
	// bl 0x8298ed98
	ctx.lr = 0x829F9504;
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

PPC_FUNC_IMPL(__imp__sub_829F9518);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9518) { __imp__sub_829F9518(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9518) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,21664
	ctx.r3.s64 = ctx.r11.s64 + 21664;
	// bl 0x827f1320
	ctx.lr = 0x829F9530;
	sub_827F1320(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,2960
	ctx.r3.s64 = ctx.r11.s64 + 2960;
	// bl 0x8298ed98
	ctx.lr = 0x829F953C;
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

PPC_FUNC_IMPL(__imp__sub_829F9550);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9550) { __imp__sub_829F9550(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9550) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,21716
	ctx.r3.s64 = ctx.r11.s64 + 21716;
	// bl 0x827f1320
	ctx.lr = 0x829F9568;
	sub_827F1320(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,2976
	ctx.r3.s64 = ctx.r11.s64 + 2976;
	// bl 0x8298ed98
	ctx.lr = 0x829F9574;
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

PPC_FUNC_IMPL(__imp__sub_829F9588);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9588) { __imp__sub_829F9588(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9588) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,21620
	ctx.r5.s64 = ctx.r11.s64 + 21620;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,21696
	ctx.r3.s64 = ctx.r11.s64 + 21696;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F95A8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F95A8) { __imp__sub_829F95A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F95A8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,21760
	ctx.r3.s64 = ctx.r11.s64 + 21760;
	// bl 0x827de108
	ctx.lr = 0x829F95C0;
	sub_827DE108(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,2992
	ctx.r3.s64 = ctx.r11.s64 + 2992;
	// bl 0x8298ed98
	ctx.lr = 0x829F95CC;
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

PPC_FUNC_IMPL(__imp__sub_829F95E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F95E0) { __imp__sub_829F95E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F95E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// addi r3,r11,22648
	ctx.r3.s64 = ctx.r11.s64 + 22648;
	// bl 0x827f1320
	ctx.lr = 0x829F95F8;
	sub_827F1320(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3008
	ctx.r3.s64 = ctx.r11.s64 + 3008;
	// bl 0x8298ed98
	ctx.lr = 0x829F9604;
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

PPC_FUNC_IMPL(__imp__sub_829F9618);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9618) { __imp__sub_829F9618(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9618) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,22068
	ctx.r5.s64 = ctx.r11.s64 + 22068;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,22680
	ctx.r3.s64 = ctx.r11.s64 + 22680;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9638);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9638) { __imp__sub_829F9638(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9638) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,22080
	ctx.r5.s64 = ctx.r11.s64 + 22080;
	// lis r11,-31985
	ctx.r11.s64 = -2096168960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,22700
	ctx.r3.s64 = ctx.r11.s64 + 22700;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9658);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9658) { __imp__sub_829F9658(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9658) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x829F9660;
	__savegprlr_29(ctx, base);
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
	// li r29,0
	ctx.r29.s64 = 0;
	// addi r31,r11,24940
	ctx.r31.s64 = ctx.r11.s64 + 24940;
loc_829F9678:
	// addi r3,r31,-44
	ctx.r3.s64 = ctx.r31.s64 + -44;
	// bl 0x827f1320
	ctx.lr = 0x829F9680;
	sub_827F1320(ctx, base);
	// li r3,0
	ctx.r3.s64 = 0;
	// bl 0x827dac78
	ctx.lr = 0x829F9688;
	sub_827DAC78(ctx, base);
	// stw r29,-4(r31)
	PPC_STORE_U32(ctx.r31.u32 + -4, ctx.r29.u32);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// stw r29,-8(r31)
	PPC_STORE_U32(ctx.r31.u32 + -8, ctx.r29.u32);
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// stw r29,-12(r31)
	PPC_STORE_U32(ctx.r31.u32 + -12, ctx.r29.u32);
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// addi r31,r31,24944
	ctx.r31.s64 = ctx.r31.s64 + 24944;
	// bge cr6,0x829f9678
	if (!ctx.cr6.lt) goto loc_829F9678;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3024
	ctx.r3.s64 = ctx.r11.s64 + 3024;
	// bl 0x8298ed98
	ctx.lr = 0x829F96B4;
	sub_8298ED98(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F96C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F96C0) { __imp__sub_829F96C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F96C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x829F96C8;
	__savegprlr_29(ctx, base);
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
	// li r29,0
	ctx.r29.s64 = 0;
	// addi r31,r11,24940
	ctx.r31.s64 = ctx.r11.s64 + 24940;
loc_829F96E0:
	// addi r3,r31,-44
	ctx.r3.s64 = ctx.r31.s64 + -44;
	// bl 0x827f1320
	ctx.lr = 0x829F96E8;
	sub_827F1320(ctx, base);
	// li r3,0
	ctx.r3.s64 = 0;
	// bl 0x827dac78
	ctx.lr = 0x829F96F0;
	sub_827DAC78(ctx, base);
	// stw r29,-4(r31)
	PPC_STORE_U32(ctx.r31.u32 + -4, ctx.r29.u32);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// stw r29,-8(r31)
	PPC_STORE_U32(ctx.r31.u32 + -8, ctx.r29.u32);
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// stw r29,-12(r31)
	PPC_STORE_U32(ctx.r31.u32 + -12, ctx.r29.u32);
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// addi r31,r31,24944
	ctx.r31.s64 = ctx.r31.s64 + 24944;
	// bge cr6,0x829f96e0
	if (!ctx.cr6.lt) goto loc_829F96E0;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3120
	ctx.r3.s64 = ctx.r11.s64 + 3120;
	// bl 0x8298ed98
	ctx.lr = 0x829F971C;
	sub_8298ED98(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9728);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9728) { __imp__sub_829F9728(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9728) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-672
	ctx.r11.s64 = ctx.r11.s64 + -672;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8528
	ctx.r11.s64 = ctx.r11.s64 + -8528;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9748);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9748) { __imp__sub_829F9748(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9748) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// lfs f0,-31256(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -31256);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8304
	ctx.r11.s64 = ctx.r11.s64 + -8304;
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9768);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9768) { __imp__sub_829F9768(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9768) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// lfs f0,-31256(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -31256);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8256
	ctx.r11.s64 = ctx.r11.s64 + -8256;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9788);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9788) { __imp__sub_829F9788(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9788) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// lfs f0,-31256(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -31256);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8208
	ctx.r11.s64 = ctx.r11.s64 + -8208;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F97A8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F97A8) { __imp__sub_829F97A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F97A8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// lfs f0,-31256(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -31256);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8160
	ctx.r11.s64 = ctx.r11.s64 + -8160;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F97C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F97C8) { __imp__sub_829F97C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F97C8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// lfs f0,-31256(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -31256);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// stfs f0,-8272(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -8272, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F97E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F97E0) { __imp__sub_829F97E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F97E0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// lfs f0,-31256(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -31256);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8320
	ctx.r11.s64 = ctx.r11.s64 + -8320;
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F97F8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F97F8) { __imp__sub_829F97F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F97F8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// lfs f0,-31256(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -31256);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8224
	ctx.r11.s64 = ctx.r11.s64 + -8224;
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9810);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9810) { __imp__sub_829F9810(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9810) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// lfs f0,-31256(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -31256);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8176
	ctx.r11.s64 = ctx.r11.s64 + -8176;
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9828);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9828) { __imp__sub_829F9828(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9828) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// lfs f0,-31256(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -31256);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8240
	ctx.r11.s64 = ctx.r11.s64 + -8240;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9850);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9850) { __imp__sub_829F9850(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9850) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// lfs f0,-31248(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -31248);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8288
	ctx.r11.s64 = ctx.r11.s64 + -8288;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9878);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9878) { __imp__sub_829F9878(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9878) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// lfs f0,-31252(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -31252);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8192
	ctx.r11.s64 = ctx.r11.s64 + -8192;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F98A0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F98A0) { __imp__sub_829F98A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F98A0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8304
	ctx.r11.s64 = ctx.r11.s64 + -8304;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8400
	ctx.r11.s64 = ctx.r11.s64 + -8400;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F98C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F98C0) { __imp__sub_829F98C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F98C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8256
	ctx.r11.s64 = ctx.r11.s64 + -8256;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8496
	ctx.r11.s64 = ctx.r11.s64 + -8496;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F98E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F98E0) { __imp__sub_829F98E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F98E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8208
	ctx.r11.s64 = ctx.r11.s64 + -8208;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8384
	ctx.r11.s64 = ctx.r11.s64 + -8384;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9900);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9900) { __imp__sub_829F9900(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9900) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8160
	ctx.r11.s64 = ctx.r11.s64 + -8160;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8432
	ctx.r11.s64 = ctx.r11.s64 + -8432;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9920);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9920) { __imp__sub_829F9920(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9920) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8272
	ctx.r11.s64 = ctx.r11.s64 + -8272;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8336
	ctx.r11.s64 = ctx.r11.s64 + -8336;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9940);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9940) { __imp__sub_829F9940(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9940) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8320
	ctx.r11.s64 = ctx.r11.s64 + -8320;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8416
	ctx.r11.s64 = ctx.r11.s64 + -8416;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9960);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9960) { __imp__sub_829F9960(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9960) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8224
	ctx.r11.s64 = ctx.r11.s64 + -8224;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8480
	ctx.r11.s64 = ctx.r11.s64 + -8480;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9980);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9980) { __imp__sub_829F9980(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9980) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8176
	ctx.r11.s64 = ctx.r11.s64 + -8176;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8448
	ctx.r11.s64 = ctx.r11.s64 + -8448;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F99A0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F99A0) { __imp__sub_829F99A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F99A0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8240
	ctx.r11.s64 = ctx.r11.s64 + -8240;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8352
	ctx.r11.s64 = ctx.r11.s64 + -8352;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F99C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F99C0) { __imp__sub_829F99C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F99C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-736
	ctx.r11.s64 = ctx.r11.s64 + -736;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8464
	ctx.r11.s64 = ctx.r11.s64 + -8464;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F99E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F99E0) { __imp__sub_829F99E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F99E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8288
	ctx.r11.s64 = ctx.r11.s64 + -8288;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8368
	ctx.r11.s64 = ctx.r11.s64 + -8368;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9A00);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9A00) { __imp__sub_829F9A00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9A00) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8192
	ctx.r11.s64 = ctx.r11.s64 + -8192;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// addi r11,r11,-8512
	ctx.r11.s64 = ctx.r11.s64 + -8512;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9A20);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9A20) { __imp__sub_829F9A20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9A20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3216
	ctx.r3.s64 = ctx.r11.s64 + 3216;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9A30);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9A30) { __imp__sub_829F9A30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9A30) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,22416
	ctx.r5.s64 = ctx.r11.s64 + 22416;
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-8140
	ctx.r3.s64 = ctx.r11.s64 + -8140;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9A50);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9A50) { __imp__sub_829F9A50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9A50) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3224
	ctx.r3.s64 = ctx.r11.s64 + 3224;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9A60);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9A60) { __imp__sub_829F9A60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9A60) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3272
	ctx.r3.s64 = ctx.r11.s64 + 3272;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9A70);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9A70) { __imp__sub_829F9A70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9A70) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3248
	ctx.r3.s64 = ctx.r11.s64 + 3248;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9A80);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9A80) { __imp__sub_829F9A80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9A80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3304
	ctx.r3.s64 = ctx.r11.s64 + 3304;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9A90);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9A90) { __imp__sub_829F9A90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9A90) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3352
	ctx.r3.s64 = ctx.r11.s64 + 3352;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9AA0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9AA0) { __imp__sub_829F9AA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9AA0) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-7804
	ctx.r3.s64 = ctx.r11.s64 + -7804;
	// bl 0x827f1320
	ctx.lr = 0x829F9AB8;
	sub_827F1320(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3336
	ctx.r3.s64 = ctx.r11.s64 + 3336;
	// bl 0x8298ed98
	ctx.lr = 0x829F9AC4;
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

PPC_FUNC_IMPL(__imp__sub_829F9AD8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9AD8) { __imp__sub_829F9AD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9AD8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,23556
	ctx.r5.s64 = ctx.r11.s64 + 23556;
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-7824
	ctx.r3.s64 = ctx.r11.s64 + -7824;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9AF8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9AF8) { __imp__sub_829F9AF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9AF8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,24572
	ctx.r5.s64 = ctx.r11.s64 + 24572;
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-7764
	ctx.r3.s64 = ctx.r11.s64 + -7764;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9B18);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9B18) { __imp__sub_829F9B18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9B18) {
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
	// li r31,255
	ctx.r31.s64 = 255;
	// addi r11,r11,-7696
	ctx.r11.s64 = ctx.r11.s64 + -7696;
	// addi r30,r11,8
	ctx.r30.s64 = ctx.r11.s64 + 8;
loc_829F9B3C:
	// li r5,148
	ctx.r5.s64 = 148;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8298eb90
	ctx.lr = 0x829F9B4C;
	sub_8298EB90(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,160
	ctx.r30.s64 = ctx.r30.s64 + 160;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x829f9b3c
	if (!ctx.cr6.lt) goto loc_829F9B3C;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3360
	ctx.r3.s64 = ctx.r11.s64 + 3360;
	// bl 0x8298ed98
	ctx.lr = 0x829F9B68;
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

PPC_FUNC_IMPL(__imp__sub_829F9B80);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9B80) { __imp__sub_829F9B80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9B80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3368
	ctx.r3.s64 = ctx.r11.s64 + 3368;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9B90);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9B90) { __imp__sub_829F9B90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9B90) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// lfs f0,-31256(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -31256);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// addi r11,r11,-32160
	ctx.r11.s64 = ctx.r11.s64 + -32160;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9BB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9BB0) { __imp__sub_829F9BB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9BB0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// lfs f0,-31256(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -31256);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// addi r11,r11,-32144
	ctx.r11.s64 = ctx.r11.s64 + -32144;
	// stfs f0,12(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 12, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9BC8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9BC8) { __imp__sub_829F9BC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9BC8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,24932
	ctx.r5.s64 = ctx.r11.s64 + 24932;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-32128
	ctx.r3.s64 = ctx.r11.s64 + -32128;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9BE8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9BE8) { __imp__sub_829F9BE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9BE8) {
	PPC_FUNC_PROLOGUE();
loc_829F9BE8:
	// mftb r11
	ctx.r11.u64 = PPC_QUERY_TIMEBASE();
	// rotlwi r10,r11,0
	ctx.r10.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x829f9be8
	if (ctx.cr6.eq) goto loc_829F9BE8;
	// lis r10,-31982
	ctx.r10.s64 = -2095972352;
	// std r11,-32088(r10)
	PPC_STORE_U64(ctx.r10.u32 + -32088, ctx.r11.u64);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9C08);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9C08) { __imp__sub_829F9C08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9C08) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
loc_829F9C08:
	// mftb r9
	ctx.r9.u64 = PPC_QUERY_TIMEBASE();
	// rotlwi r11,r9,0
	ctx.r11.u64 = __builtin_rotateleft32(ctx.r9.u32, 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x829f9c08
	if (ctx.cr6.eq) goto loc_829F9C08;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r3,r10,3376
	ctx.r3.s64 = ctx.r10.s64 + 3376;
	// addi r11,r11,3768
	ctx.r11.s64 = ctx.r11.s64 + 3768;
	// li r10,0
	ctx.r10.s64 = 0;
	// std r9,56(r11)
	PPC_STORE_U64(ctx.r11.u32 + 56, ctx.r9.u64);
	// stb r10,64(r11)
	PPC_STORE_U8(ctx.r11.u32 + 64, ctx.r10.u8);
	// stb r10,65(r11)
	PPC_STORE_U8(ctx.r11.u32 + 65, ctx.r10.u8);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lfs f0,3068(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 3068);
	ctx.f0.f64 = double(temp.f32);
	// li r10,0
	ctx.r10.s64 = 0;
	// stfs f0,72(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 72, temp.u32);
	// stw r10,68(r11)
	PPC_STORE_U32(ctx.r11.u32 + 68, ctx.r10.u32);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lfs f0,2592(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 2592);
	ctx.f0.f64 = double(temp.f32);
	// lis r10,-32255
	ctx.r10.s64 = -2113863680;
	// stfs f0,76(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 76, temp.u32);
	// lfs f0,-26548(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -26548);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,80(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 80, temp.u32);
	// stfs f0,84(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 84, temp.u32);
	// stfs f0,88(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 88, temp.u32);
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9C70);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9C70) { __imp__sub_829F9C70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9C70) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,3068(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3068);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// stfs f0,-16(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -16, temp.u32);
	// stfs f0,-12(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -12, temp.u32);
	// addi r11,r11,3952
	ctx.r11.s64 = ctx.r11.s64 + 3952;
	// stfs f0,-8(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -8, temp.u32);
	// addi r10,r1,-16
	ctx.r10.s64 = ctx.r1.s64 + -16;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r10,16
	ctx.r10.s64 = 16;
	// stvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9CA0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9CA0) { __imp__sub_829F9CA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9CA0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,30804
	ctx.r5.s64 = ctx.r11.s64 + 30804;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-32072
	ctx.r3.s64 = ctx.r11.s64 + -32072;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9CC0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9CC0) { __imp__sub_829F9CC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9CC0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,-32048
	ctx.r11.s64 = ctx.r11.s64 + -32048;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// stw r10,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
	// stw r10,8(r11)
	PPC_STORE_U32(ctx.r11.u32 + 8, ctx.r10.u32);
	// stw r10,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r10.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9CE0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9CE0) { __imp__sub_829F9CE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9CE0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r5,512
	ctx.r5.s64 = 512;
	// addi r3,r11,-31776
	ctx.r3.s64 = ctx.r11.s64 + -31776;
	// li r4,0
	ctx.r4.s64 = 0;
	// b 0x8298eb90
	sub_8298EB90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9CF8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9CF8) { __imp__sub_829F9CF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9CF8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r5,256
	ctx.r5.s64 = 256;
	// addi r3,r11,-32032
	ctx.r3.s64 = ctx.r11.s64 + -32032;
	// li r4,0
	ctx.r4.s64 = 0;
	// b 0x8298eb90
	sub_8298EB90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9D10);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9D10) { __imp__sub_829F9D10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9D10) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// li r9,1
	ctx.r9.s64 = 1;
	// addi r11,r11,4312
	ctx.r11.s64 = ctx.r11.s64 + 4312;
	// li r8,0
	ctx.r8.s64 = 0;
	// addi r10,r11,40
	ctx.r10.s64 = ctx.r11.s64 + 40;
loc_829F9D24:
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// sth r8,2(r10)
	PPC_STORE_U16(ctx.r10.u32 + 2, ctx.r8.u16);
	// sth r8,0(r10)
	PPC_STORE_U16(ctx.r10.u32 + 0, ctx.r8.u16);
	// addi r10,r10,1540
	ctx.r10.s64 = ctx.r10.s64 + 1540;
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// bge cr6,0x829f9d24
	if (!ctx.cr6.lt) goto loc_829F9D24;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r3,r10,3400
	ctx.r3.s64 = ctx.r10.s64 + 3400;
	// mr r10,r8
	ctx.r10.u64 = ctx.r8.u64;
	// stw r10,3120(r11)
	PPC_STORE_U32(ctx.r11.u32 + 3120, ctx.r10.u32);
	// stw r10,3124(r11)
	PPC_STORE_U32(ctx.r11.u32 + 3124, ctx.r10.u32);
	// stb r10,3128(r11)
	PPC_STORE_U8(ctx.r11.u32 + 3128, ctx.r10.u8);
	// lis r10,-31982
	ctx.r10.s64 = -2095972352;
	// stw r11,-31264(r10)
	PPC_STORE_U32(ctx.r10.u32 + -31264, ctx.r11.u32);
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9D60);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9D60) { __imp__sub_829F9D60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9D60) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31982
	ctx.r10.s64 = -2095972352;
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,-31244(r10)
	PPC_STORE_U32(ctx.r10.u32 + -31244, ctx.r11.u32);
	// lis r10,-31982
	ctx.r10.s64 = -2095972352;
	// li r11,-1
	ctx.r11.s64 = -1;
	// stw r11,-31256(r10)
	PPC_STORE_U32(ctx.r10.u32 + -31256, ctx.r11.u32);
	// lis r10,-31982
	ctx.r10.s64 = -2095972352;
	// lis r11,32640
	ctx.r11.s64 = 2139095040;
	// stw r11,-31248(r10)
	PPC_STORE_U32(ctx.r10.u32 + -31248, ctx.r11.u32);
	// lis r10,-31982
	ctx.r10.s64 = -2095972352;
	// lis r11,32704
	ctx.r11.s64 = 2143289344;
	// stw r11,-31252(r10)
	PPC_STORE_U32(ctx.r10.u32 + -31252, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9D98);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9D98) { __imp__sub_829F9D98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9D98) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3448
	ctx.r3.s64 = ctx.r11.s64 + 3448;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9DA8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9DA8) { __imp__sub_829F9DA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9DA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// li r6,5842
	ctx.r6.s64 = 5842;
	// addi r5,r11,10144
	ctx.r5.s64 = ctx.r11.s64 + 10144;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r4,r11,12924
	ctx.r4.s64 = ctx.r11.s64 + 12924;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// addi r3,r11,20096
	ctx.r3.s64 = ctx.r11.s64 + 20096;
	// b 0x827fe360
	sub_827FE360(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9DE8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9DE8) { __imp__sub_829F9DE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9DE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,12952
	ctx.r5.s64 = ctx.r11.s64 + 12952;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,20112
	ctx.r3.s64 = ctx.r11.s64 + 20112;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9E08);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9E08) { __imp__sub_829F9E08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9E08) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,12968
	ctx.r5.s64 = ctx.r11.s64 + 12968;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,19968
	ctx.r3.s64 = ctx.r11.s64 + 19968;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9E28);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9E28) { __imp__sub_829F9E28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9E28) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,12972
	ctx.r5.s64 = ctx.r11.s64 + 12972;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,19704
	ctx.r3.s64 = ctx.r11.s64 + 19704;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9E48);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9E48) { __imp__sub_829F9E48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9E48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,12988
	ctx.r5.s64 = ctx.r11.s64 + 12988;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,19908
	ctx.r3.s64 = ctx.r11.s64 + 19908;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9E68);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9E68) { __imp__sub_829F9E68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9E68) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,13004
	ctx.r5.s64 = ctx.r11.s64 + 13004;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,19948
	ctx.r3.s64 = ctx.r11.s64 + 19948;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9E88);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9E88) { __imp__sub_829F9E88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9E88) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,13012
	ctx.r5.s64 = ctx.r11.s64 + 13012;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,19664
	ctx.r3.s64 = ctx.r11.s64 + 19664;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9EA8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9EA8) { __imp__sub_829F9EA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9EA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,13024
	ctx.r5.s64 = ctx.r11.s64 + 13024;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,19824
	ctx.r3.s64 = ctx.r11.s64 + 19824;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9EC8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9EC8) { __imp__sub_829F9EC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9EC8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,13040
	ctx.r5.s64 = ctx.r11.s64 + 13040;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,19888
	ctx.r3.s64 = ctx.r11.s64 + 19888;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9EE8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9EE8) { __imp__sub_829F9EE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9EE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,13056
	ctx.r5.s64 = ctx.r11.s64 + 13056;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,19844
	ctx.r3.s64 = ctx.r11.s64 + 19844;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9F08);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9F08) { __imp__sub_829F9F08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9F08) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,13068
	ctx.r5.s64 = ctx.r11.s64 + 13068;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,19988
	ctx.r3.s64 = ctx.r11.s64 + 19988;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9F28);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9F28) { __imp__sub_829F9F28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9F28) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-21904
	ctx.r5.s64 = ctx.r11.s64 + -21904;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,19684
	ctx.r3.s64 = ctx.r11.s64 + 19684;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9F48);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9F48) { __imp__sub_829F9F48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9F48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-21896
	ctx.r5.s64 = ctx.r11.s64 + -21896;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,19864
	ctx.r3.s64 = ctx.r11.s64 + 19864;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9F68);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9F68) { __imp__sub_829F9F68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9F68) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,13072
	ctx.r5.s64 = ctx.r11.s64 + 13072;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,19928
	ctx.r3.s64 = ctx.r11.s64 + 19928;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9F88);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9F88) { __imp__sub_829F9F88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9F88) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// addi r3,r11,20008
	ctx.r3.s64 = ctx.r11.s64 + 20008;
	// bl 0x8285bbf0
	ctx.lr = 0x829F9FA0;
	sub_8285BBF0(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3544
	ctx.r3.s64 = ctx.r11.s64 + 3544;
	// bl 0x8298ed98
	ctx.lr = 0x829F9FAC;
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

PPC_FUNC_IMPL(__imp__sub_829F9FC0);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9FC0) { __imp__sub_829F9FC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9FC0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32249
	ctx.r11.s64 = -2113470464;
	// lis r10,-31982
	ctx.r10.s64 = -2095972352;
	// lwz r11,21176(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 21176);
	// stw r11,19884(r10)
	PPC_STORE_U32(ctx.r10.u32 + 19884, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829F9FD8);  /* forward decl */
PPC_WEAK_FUNC(sub_829F9FD8) { __imp__sub_829F9FD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829F9FD8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,4
	ctx.r4.s64 = 4;
	// addi r3,r11,20576
	ctx.r3.s64 = ctx.r11.s64 + 20576;
	// bl 0x82871770
	ctx.lr = 0x829F9FF4;
	sub_82871770(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3560
	ctx.r3.s64 = ctx.r11.s64 + 3560;
	// bl 0x8298ed98
	ctx.lr = 0x829FA000;
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

PPC_FUNC_IMPL(__imp__sub_829FA010);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA010) { __imp__sub_829FA010(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA010) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// li r6,5825
	ctx.r6.s64 = 5825;
	// addi r5,r11,16456
	ctx.r5.s64 = ctx.r11.s64 + 16456;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// addi r4,r11,14872
	ctx.r4.s64 = ctx.r11.s64 + 14872;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// addi r3,r11,22012
	ctx.r3.s64 = ctx.r11.s64 + 22012;
	// b 0x827fe360
	sub_827FE360(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA030);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA030) { __imp__sub_829FA030(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA030) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x8285bbf0
	ctx.lr = 0x829FA050;
	sub_8285BBF0(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r3,r10,3568
	ctx.r3.s64 = ctx.r10.s64 + 3568;
	// stb r11,112(r31)
	PPC_STORE_U8(ctx.r31.u32 + 112, ctx.r11.u8);
	// bl 0x8298ed98
	ctx.lr = 0x829FA064;
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

PPC_FUNC_IMPL(__imp__sub_829FA078);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA078) { __imp__sub_829FA078(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA078) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,22184
	ctx.r11.s64 = ctx.r11.s64 + 22184;
	// lis r9,-32096
	ctx.r9.s64 = -2103443456;
	// addi r3,r9,3632
	ctx.r3.s64 = ctx.r9.s64 + 3632;
	// stw r10,24(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24, ctx.r10.u32);
	// stw r10,20(r11)
	PPC_STORE_U32(ctx.r11.u32 + 20, ctx.r10.u32);
	// stw r10,16(r11)
	PPC_STORE_U32(ctx.r11.u32 + 16, ctx.r10.u32);
	// stw r10,52(r11)
	PPC_STORE_U32(ctx.r11.u32 + 52, ctx.r10.u32);
	// stw r10,48(r11)
	PPC_STORE_U32(ctx.r11.u32 + 48, ctx.r10.u32);
	// stw r10,44(r11)
	PPC_STORE_U32(ctx.r11.u32 + 44, ctx.r10.u32);
	// stw r10,80(r11)
	PPC_STORE_U32(ctx.r11.u32 + 80, ctx.r10.u32);
	// stw r10,76(r11)
	PPC_STORE_U32(ctx.r11.u32 + 76, ctx.r10.u32);
	// stw r10,72(r11)
	PPC_STORE_U32(ctx.r11.u32 + 72, ctx.r10.u32);
	// stw r10,108(r11)
	PPC_STORE_U32(ctx.r11.u32 + 108, ctx.r10.u32);
	// stw r10,104(r11)
	PPC_STORE_U32(ctx.r11.u32 + 104, ctx.r10.u32);
	// stw r10,100(r11)
	PPC_STORE_U32(ctx.r11.u32 + 100, ctx.r10.u32);
	// stw r10,136(r11)
	PPC_STORE_U32(ctx.r11.u32 + 136, ctx.r10.u32);
	// stw r10,132(r11)
	PPC_STORE_U32(ctx.r11.u32 + 132, ctx.r10.u32);
	// stw r10,128(r11)
	PPC_STORE_U32(ctx.r11.u32 + 128, ctx.r10.u32);
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA0D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA0D0) { __imp__sub_829FA0D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA0D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3640
	ctx.r3.s64 = ctx.r11.s64 + 3640;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA0E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA0E0) { __imp__sub_829FA0E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA0E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3672
	ctx.r3.s64 = ctx.r11.s64 + 3672;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA0F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA0F0) { __imp__sub_829FA0F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA0F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3704
	ctx.r3.s64 = ctx.r11.s64 + 3704;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA100);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA100) { __imp__sub_829FA100(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA100) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3720
	ctx.r3.s64 = ctx.r11.s64 + 3720;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA110);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA110) { __imp__sub_829FA110(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA110) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15200
	ctx.r5.s64 = ctx.r11.s64 + 15200;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,22444
	ctx.r3.s64 = ctx.r11.s64 + 22444;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA130);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA130) { __imp__sub_829FA130(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA130) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15568
	ctx.r5.s64 = ctx.r11.s64 + 15568;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,22764
	ctx.r3.s64 = ctx.r11.s64 + 22764;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA150);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA150) { __imp__sub_829FA150(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA150) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15580
	ctx.r5.s64 = ctx.r11.s64 + 15580;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,22704
	ctx.r3.s64 = ctx.r11.s64 + 22704;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA170);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA170) { __imp__sub_829FA170(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA170) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15592
	ctx.r5.s64 = ctx.r11.s64 + 15592;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,22624
	ctx.r3.s64 = ctx.r11.s64 + 22624;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA190);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA190) { __imp__sub_829FA190(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA190) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15596
	ctx.r5.s64 = ctx.r11.s64 + 15596;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,22544
	ctx.r3.s64 = ctx.r11.s64 + 22544;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA1B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA1B0) { __imp__sub_829FA1B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA1B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15608
	ctx.r5.s64 = ctx.r11.s64 + 15608;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,22664
	ctx.r3.s64 = ctx.r11.s64 + 22664;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA1D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA1D0) { __imp__sub_829FA1D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA1D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15616
	ctx.r5.s64 = ctx.r11.s64 + 15616;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,22604
	ctx.r3.s64 = ctx.r11.s64 + 22604;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA1F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA1F0) { __imp__sub_829FA1F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA1F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15624
	ctx.r5.s64 = ctx.r11.s64 + 15624;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,22644
	ctx.r3.s64 = ctx.r11.s64 + 22644;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA210);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA210) { __imp__sub_829FA210(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA210) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15636
	ctx.r5.s64 = ctx.r11.s64 + 15636;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,22744
	ctx.r3.s64 = ctx.r11.s64 + 22744;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA230);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA230) { __imp__sub_829FA230(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA230) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15648
	ctx.r5.s64 = ctx.r11.s64 + 15648;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,22564
	ctx.r3.s64 = ctx.r11.s64 + 22564;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA250);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA250) { __imp__sub_829FA250(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA250) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15664
	ctx.r5.s64 = ctx.r11.s64 + 15664;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,22724
	ctx.r3.s64 = ctx.r11.s64 + 22724;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA270);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA270) { __imp__sub_829FA270(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA270) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15680
	ctx.r5.s64 = ctx.r11.s64 + 15680;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,22684
	ctx.r3.s64 = ctx.r11.s64 + 22684;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA290);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA290) { __imp__sub_829FA290(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA290) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15696
	ctx.r5.s64 = ctx.r11.s64 + 15696;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,22524
	ctx.r3.s64 = ctx.r11.s64 + 22524;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA2B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA2B0) { __imp__sub_829FA2B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA2B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15712
	ctx.r5.s64 = ctx.r11.s64 + 15712;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,22504
	ctx.r3.s64 = ctx.r11.s64 + 22504;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA2D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA2D0) { __imp__sub_829FA2D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA2D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15724
	ctx.r5.s64 = ctx.r11.s64 + 15724;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,22484
	ctx.r3.s64 = ctx.r11.s64 + 22484;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA2F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA2F0) { __imp__sub_829FA2F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA2F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,15736
	ctx.r5.s64 = ctx.r11.s64 + 15736;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,22584
	ctx.r3.s64 = ctx.r11.s64 + 22584;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA310);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA310) { __imp__sub_829FA310(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA310) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,17028
	ctx.r5.s64 = ctx.r11.s64 + 17028;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,24068
	ctx.r3.s64 = ctx.r11.s64 + 24068;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA330);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA330) { __imp__sub_829FA330(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA330) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3736
	ctx.r3.s64 = ctx.r11.s64 + 3736;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA340);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA340) { __imp__sub_829FA340(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA340) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3744
	ctx.r3.s64 = ctx.r11.s64 + 3744;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA350);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA350) { __imp__sub_829FA350(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA350) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// addi r3,r11,26168
	ctx.r3.s64 = ctx.r11.s64 + 26168;
	// bl 0x8285af28
	ctx.lr = 0x829FA368;
	sub_8285AF28(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3752
	ctx.r3.s64 = ctx.r11.s64 + 3752;
	// bl 0x8298ed98
	ctx.lr = 0x829FA374;
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

PPC_FUNC_IMPL(__imp__sub_829FA388);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA388) { __imp__sub_829FA388(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA388) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3800
	ctx.r3.s64 = ctx.r11.s64 + 3800;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA398);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA398) { __imp__sub_829FA398(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA398) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3832
	ctx.r3.s64 = ctx.r11.s64 + 3832;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA3A8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA3A8) { __imp__sub_829FA3A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA3A8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// addi r3,r11,30824
	ctx.r3.s64 = ctx.r11.s64 + 30824;
	// bl 0x827f1320
	ctx.lr = 0x829FA3C0;
	sub_827F1320(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3768
	ctx.r3.s64 = ctx.r11.s64 + 3768;
	// bl 0x8298ed98
	ctx.lr = 0x829FA3CC;
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

PPC_FUNC_IMPL(__imp__sub_829FA3E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA3E0) { __imp__sub_829FA3E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA3E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// addi r3,r11,30784
	ctx.r3.s64 = ctx.r11.s64 + 30784;
	// bl 0x827f1320
	ctx.lr = 0x829FA3F8;
	sub_827F1320(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3784
	ctx.r3.s64 = ctx.r11.s64 + 3784;
	// bl 0x8298ed98
	ctx.lr = 0x829FA404;
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

PPC_FUNC_IMPL(__imp__sub_829FA418);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA418) { __imp__sub_829FA418(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA418) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// li r4,3
	ctx.r4.s64 = 3;
	// addi r11,r11,30864
	ctx.r11.s64 = ctx.r11.s64 + 30864;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// li r8,128
	ctx.r8.s64 = 128;
	// li r5,512
	ctx.r5.s64 = 512;
	// li r6,1
	ctx.r6.s64 = 1;
loc_829FA438:
	// stw r10,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
	// addi r9,r11,12
	ctx.r9.s64 = ctx.r11.s64 + 12;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// li r7,12
	ctx.r7.s64 = 12;
	// stb r8,11(r11)
	PPC_STORE_U8(ctx.r11.u32 + 11, ctx.r8.u8);
	// stb r8,10(r11)
	PPC_STORE_U8(ctx.r11.u32 + 10, ctx.r8.u8);
	// stb r8,9(r11)
	PPC_STORE_U8(ctx.r11.u32 + 9, ctx.r8.u8);
	// stb r8,8(r11)
	PPC_STORE_U8(ctx.r11.u32 + 8, ctx.r8.u8);
	// mtctr r7
	ctx.ctr.u64 = ctx.r7.u64;
loc_829FA45C:
	// stb r10,0(r9)
	PPC_STORE_U8(ctx.r9.u32 + 0, ctx.r10.u8);
	// addi r9,r9,1
	ctx.r9.s64 = ctx.r9.s64 + 1;
	// bdnz 0x829fa45c
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_829FA45C;
	// sth r5,24(r11)
	PPC_STORE_U16(ctx.r11.u32 + 24, ctx.r5.u16);
	// addi r4,r4,-1
	ctx.r4.s64 = ctx.r4.s64 + -1;
	// sth r5,26(r11)
	PPC_STORE_U16(ctx.r11.u32 + 26, ctx.r5.u16);
	// sth r5,28(r11)
	PPC_STORE_U16(ctx.r11.u32 + 28, ctx.r5.u16);
	// cmpwi cr6,r4,0
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 0, ctx.xer);
	// sth r5,30(r11)
	PPC_STORE_U16(ctx.r11.u32 + 30, ctx.r5.u16);
	// stb r6,32(r11)
	PPC_STORE_U8(ctx.r11.u32 + 32, ctx.r6.u8);
	// stb r6,33(r11)
	PPC_STORE_U8(ctx.r11.u32 + 33, ctx.r6.u8);
	// stb r6,34(r11)
	PPC_STORE_U8(ctx.r11.u32 + 34, ctx.r6.u8);
	// sth r10,48(r11)
	PPC_STORE_U16(ctx.r11.u32 + 48, ctx.r10.u16);
	// sth r10,46(r11)
	PPC_STORE_U16(ctx.r11.u32 + 46, ctx.r10.u16);
	// stw r10,40(r11)
	PPC_STORE_U32(ctx.r11.u32 + 40, ctx.r10.u32);
	// stb r10,35(r11)
	PPC_STORE_U8(ctx.r11.u32 + 35, ctx.r10.u8);
	// stb r10,36(r11)
	PPC_STORE_U8(ctx.r11.u32 + 36, ctx.r10.u8);
	// stb r10,37(r11)
	PPC_STORE_U8(ctx.r11.u32 + 37, ctx.r10.u8);
	// stb r6,44(r11)
	PPC_STORE_U8(ctx.r11.u32 + 44, ctx.r6.u8);
	// addi r11,r11,56
	ctx.r11.s64 = ctx.r11.s64 + 56;
	// bge cr6,0x829fa438
	if (!ctx.cr6.lt) goto loc_829FA438;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3864
	ctx.r3.s64 = ctx.r11.s64 + 3864;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA4C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA4C0) { __imp__sub_829FA4C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA4C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31982
	ctx.r11.s64 = -2095972352;
	// addi r3,r11,31280
	ctx.r3.s64 = ctx.r11.s64 + 31280;
	// bl 0x82862ba0
	ctx.lr = 0x829FA4D8;
	sub_82862BA0(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3872
	ctx.r3.s64 = ctx.r11.s64 + 3872;
	// bl 0x8298ed98
	ctx.lr = 0x829FA4E4;
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

PPC_FUNC_IMPL(__imp__sub_829FA4F8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA4F8) { __imp__sub_829FA4F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA4F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,3880
	ctx.r3.s64 = ctx.r11.s64 + 3880;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA508);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA508) { __imp__sub_829FA508(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA508) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// lis r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,25512
	ctx.r5.s64 = ctx.r11.s64 + 25512;
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// ori r6,r6,45900
	ctx.r6.u64 = ctx.r6.u64 | 45900;
	// addi r4,r11,28176
	ctx.r4.s64 = ctx.r11.s64 + 28176;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r3,r11,-31160
	ctx.r3.s64 = ctx.r11.s64 + -31160;
	// b 0x827fe360
	sub_827FE360(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA530);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA530) { __imp__sub_829FA530(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA530) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,12968
	ctx.r5.s64 = ctx.r11.s64 + 12968;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-31180
	ctx.r3.s64 = ctx.r11.s64 + -31180;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA550);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA550) { __imp__sub_829FA550(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA550) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32248
	ctx.r11.s64 = -2113404928;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,32112
	ctx.r5.s64 = ctx.r11.s64 + 32112;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-30008
	ctx.r3.s64 = ctx.r11.s64 + -30008;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA570);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA570) { __imp__sub_829FA570(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA570) {
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
	// lis r11,-32158
	ctx.r11.s64 = -2107506688;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-8488
	ctx.r5.s64 = ctx.r11.s64 + -8488;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829FA598;
	sub_827DB118(ctx, base);
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// lis r9,-32137
	ctx.r9.s64 = -2106130432;
	// addi r10,r11,6592
	ctx.r10.s64 = ctx.r11.s64 + 6592;
	// addi r9,r9,-19072
	ctx.r9.s64 = ctx.r9.s64 + -19072;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
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

PPC_FUNC_IMPL(__imp__sub_829FA5E8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA5E8) { __imp__sub_829FA5E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA5E8) {
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
	// lis r11,-32135
	ctx.r11.s64 = -2105999360;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-15424
	ctx.r5.s64 = ctx.r11.s64 + -15424;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829FA610;
	sub_827DB118(ctx, base);
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// lis r9,-32137
	ctx.r9.s64 = -2106130432;
	// addi r10,r11,6640
	ctx.r10.s64 = ctx.r11.s64 + 6640;
	// addi r9,r9,-19072
	ctx.r9.s64 = ctx.r9.s64 + -19072;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
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

PPC_FUNC_IMPL(__imp__sub_829FA660);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA660) { __imp__sub_829FA660(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA660) {
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
	// lis r11,-32135
	ctx.r11.s64 = -2105999360;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-15424
	ctx.r5.s64 = ctx.r11.s64 + -15424;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829FA688;
	sub_827DB118(ctx, base);
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// lis r9,-32137
	ctx.r9.s64 = -2106130432;
	// addi r10,r11,6688
	ctx.r10.s64 = ctx.r11.s64 + 6688;
	// addi r9,r9,-19072
	ctx.r9.s64 = ctx.r9.s64 + -19072;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
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

PPC_FUNC_IMPL(__imp__sub_829FA6D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA6D8) { __imp__sub_829FA6D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA6D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// addi r10,r11,6736
	ctx.r10.s64 = ctx.r11.s64 + 6736;
	// lwz r9,12(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 12);
	// lwz r11,4(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
loc_829FA6E8:
	// rlwinm r11,r11,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// addi r9,r9,1
	ctx.r9.s64 = ctx.r9.s64 + 1;
	// cmpwi cr6,r11,16
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 16, ctx.xer);
	// stw r11,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r11.u32);
	// blt cr6,0x829fa6e8
	if (ctx.cr6.lt) goto loc_829FA6E8;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// stw r9,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r9.u32);
	// lis r9,-32096
	ctx.r9.s64 = -2103443456;
	// addi r3,r9,3984
	ctx.r3.s64 = ctx.r9.s64 + 3984;
	// stw r11,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r11.u32);
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA718);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA718) { __imp__sub_829FA718(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA718) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// addi r10,r11,6904
	ctx.r10.s64 = ctx.r11.s64 + 6904;
	// lwz r9,12(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 12);
	// lwz r11,4(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
loc_829FA728:
	// rlwinm r11,r11,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// addi r9,r9,1
	ctx.r9.s64 = ctx.r9.s64 + 1;
	// cmpwi cr6,r11,16
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 16, ctx.xer);
	// stw r11,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r11.u32);
	// blt cr6,0x829fa728
	if (ctx.cr6.lt) goto loc_829FA728;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// stw r9,12(r10)
	PPC_STORE_U32(ctx.r10.u32 + 12, ctx.r9.u32);
	// lis r9,-32096
	ctx.r9.s64 = -2103443456;
	// addi r3,r9,4000
	ctx.r3.s64 = ctx.r9.s64 + 4000;
	// stw r11,8(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8, ctx.r11.u32);
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA758);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA758) { __imp__sub_829FA758(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA758) {
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
	// lis r11,-32120
	ctx.r11.s64 = -2105016320;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-5960
	ctx.r5.s64 = ctx.r11.s64 + -5960;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829FA780;
	sub_827DB118(ctx, base);
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// lis r9,-32137
	ctx.r9.s64 = -2106130432;
	// addi r10,r11,7564
	ctx.r10.s64 = ctx.r11.s64 + 7564;
	// addi r9,r9,-19072
	ctx.r9.s64 = ctx.r9.s64 + -19072;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
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

PPC_FUNC_IMPL(__imp__sub_829FA7D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA7D0) { __imp__sub_829FA7D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA7D0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// addi r11,r11,7612
	ctx.r11.s64 = ctx.r11.s64 + 7612;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA7F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA7F0) { __imp__sub_829FA7F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA7F0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// addi r11,r11,7620
	ctx.r11.s64 = ctx.r11.s64 + 7620;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA810);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA810) { __imp__sub_829FA810(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA810) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31983
	ctx.r10.s64 = -2096037888;
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// addi r11,r11,7628
	ctx.r11.s64 = ctx.r11.s64 + 7628;
	// lwz r9,-7772(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7772);
	// stw r11,-7772(r10)
	PPC_STORE_U32(ctx.r10.u32 + -7772, ctx.r11.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FA830);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA830) { __imp__sub_829FA830(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA830) {
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
	// lis r11,-32135
	ctx.r11.s64 = -2105999360;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-15424
	ctx.r5.s64 = ctx.r11.s64 + -15424;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829FA858;
	sub_827DB118(ctx, base);
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// lis r9,-32137
	ctx.r9.s64 = -2106130432;
	// addi r10,r11,9460
	ctx.r10.s64 = ctx.r11.s64 + 9460;
	// addi r9,r9,-19072
	ctx.r9.s64 = ctx.r9.s64 + -19072;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
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

PPC_FUNC_IMPL(__imp__sub_829FA8A8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA8A8) { __imp__sub_829FA8A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA8A8) {
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
	// lis r11,-32119
	ctx.r11.s64 = -2104950784;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-5744
	ctx.r5.s64 = ctx.r11.s64 + -5744;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829FA8D0;
	sub_827DB118(ctx, base);
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// lis r9,-32137
	ctx.r9.s64 = -2106130432;
	// addi r10,r11,9508
	ctx.r10.s64 = ctx.r11.s64 + 9508;
	// addi r9,r9,-19072
	ctx.r9.s64 = ctx.r9.s64 + -19072;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
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

PPC_FUNC_IMPL(__imp__sub_829FA920);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA920) { __imp__sub_829FA920(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA920) {
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
	// lis r11,-32119
	ctx.r11.s64 = -2104950784;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,144
	ctx.r5.s64 = ctx.r11.s64 + 144;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829FA948;
	sub_827DB118(ctx, base);
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// lis r9,-32137
	ctx.r9.s64 = -2106130432;
	// addi r10,r11,9776
	ctx.r10.s64 = ctx.r11.s64 + 9776;
	// addi r9,r9,-19072
	ctx.r9.s64 = ctx.r9.s64 + -19072;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
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

PPC_FUNC_IMPL(__imp__sub_829FA998);  /* forward decl */
PPC_WEAK_FUNC(sub_829FA998) { __imp__sub_829FA998(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FA998) {
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
	// lis r11,-32119
	ctx.r11.s64 = -2104950784;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,144
	ctx.r5.s64 = ctx.r11.s64 + 144;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829FA9C0;
	sub_827DB118(ctx, base);
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// lis r9,-32137
	ctx.r9.s64 = -2106130432;
	// addi r10,r11,10096
	ctx.r10.s64 = ctx.r11.s64 + 10096;
	// addi r9,r9,-19072
	ctx.r9.s64 = ctx.r9.s64 + -19072;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
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

PPC_FUNC_IMPL(__imp__sub_829FAA10);  /* forward decl */
PPC_WEAK_FUNC(sub_829FAA10) { __imp__sub_829FAA10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FAA10) {
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
	// lis r11,-32158
	ctx.r11.s64 = -2107506688;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-8488
	ctx.r5.s64 = ctx.r11.s64 + -8488;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829FAA38;
	sub_827DB118(ctx, base);
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// lis r9,-32137
	ctx.r9.s64 = -2106130432;
	// addi r10,r11,10424
	ctx.r10.s64 = ctx.r11.s64 + 10424;
	// addi r9,r9,-19072
	ctx.r9.s64 = ctx.r9.s64 + -19072;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
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

PPC_FUNC_IMPL(__imp__sub_829FAA88);  /* forward decl */
PPC_WEAK_FUNC(sub_829FAA88) { __imp__sub_829FAA88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FAA88) {
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
	// lis r11,-32135
	ctx.r11.s64 = -2105999360;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-15424
	ctx.r5.s64 = ctx.r11.s64 + -15424;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829FAAB0;
	sub_827DB118(ctx, base);
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// lis r9,-32137
	ctx.r9.s64 = -2106130432;
	// addi r10,r11,10472
	ctx.r10.s64 = ctx.r11.s64 + 10472;
	// addi r9,r9,-19072
	ctx.r9.s64 = ctx.r9.s64 + -19072;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
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

PPC_FUNC_IMPL(__imp__sub_829FAB00);  /* forward decl */
PPC_WEAK_FUNC(sub_829FAB00) { __imp__sub_829FAB00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FAB00) {
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
	// lis r11,-32119
	ctx.r11.s64 = -2104950784;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,1392
	ctx.r5.s64 = ctx.r11.s64 + 1392;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x827db118
	ctx.lr = 0x829FAB28;
	sub_827DB118(ctx, base);
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// lis r9,-32137
	ctx.r9.s64 = -2106130432;
	// addi r10,r11,10616
	ctx.r10.s64 = ctx.r11.s64 + 10616;
	// addi r9,r9,-19072
	ctx.r9.s64 = ctx.r9.s64 + -19072;
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// addi r10,r10,32
	ctx.r10.s64 = ctx.r10.s64 + 32;
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

PPC_FUNC_IMPL(__imp__sub_829FAB78);  /* forward decl */
PPC_WEAK_FUNC(sub_829FAB78) { __imp__sub_829FAB78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FAB78) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,2592(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2592);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// lfs f13,10792(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 10792);
	ctx.f13.f64 = double(temp.f32);
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// fdivs f0,f0,f13
	ctx.f0.f64 = double(float(ctx.f0.f64 / ctx.f13.f64));
	// stfs f0,-28796(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -28796, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FAB98);  /* forward decl */
PPC_WEAK_FUNC(sub_829FAB98) { __imp__sub_829FAB98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FAB98) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-28832
	ctx.r3.s64 = ctx.r11.s64 + -28832;
	// bl 0x828e0ab8
	ctx.lr = 0x829FABB0;
	sub_828E0AB8(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,4016
	ctx.r3.s64 = ctx.r11.s64 + 4016;
	// bl 0x8298ed98
	ctx.lr = 0x829FABBC;
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

PPC_FUNC_IMPL(__imp__sub_829FABD0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FABD0) { __imp__sub_829FABD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FABD0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-19532
	ctx.r5.s64 = ctx.r11.s64 + -19532;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-28792
	ctx.r3.s64 = ctx.r11.s64 + -28792;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FABF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FABF0) { __imp__sub_829FABF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FABF0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,4032
	ctx.r3.s64 = ctx.r11.s64 + 4032;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FAC00);  /* forward decl */
PPC_WEAK_FUNC(sub_829FAC00) { __imp__sub_829FAC00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FAC00) {
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
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,-28768
	ctx.r31.s64 = ctx.r11.s64 + -28768;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82894d90
	ctx.lr = 0x829FAC20;
	sub_82894D90(ctx, base);
	// addi r3,r31,100
	ctx.r3.s64 = ctx.r31.s64 + 100;
	// bl 0x828a7c58
	ctx.lr = 0x829FAC28;
	sub_828A7C58(ctx, base);
	// addi r3,r31,172
	ctx.r3.s64 = ctx.r31.s64 + 172;
	// bl 0x828a3430
	ctx.lr = 0x829FAC30;
	sub_828A3430(ctx, base);
	// addi r3,r31,380
	ctx.r3.s64 = ctx.r31.s64 + 380;
	// bl 0x828b0550
	ctx.lr = 0x829FAC38;
	sub_828B0550(ctx, base);
	// addi r3,r31,700
	ctx.r3.s64 = ctx.r31.s64 + 700;
	// bl 0x8289da88
	ctx.lr = 0x829FAC40;
	sub_8289DA88(ctx, base);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lis r6,-32247
	ctx.r6.s64 = -2113339392;
	// lis r5,-32245
	ctx.r5.s64 = -2113208320;
	// lis r9,-32247
	ctx.r9.s64 = -2113339392;
	// lfs f0,3068(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3068);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// lis r8,-32247
	ctx.r8.s64 = -2113339392;
	// stfs f0,84(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// lis r7,-32247
	ctx.r7.s64 = -2113339392;
	// stfs f0,96(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 96, temp.u32);
	// addi r9,r9,-19056
	ctx.r9.s64 = ctx.r9.s64 + -19056;
	// stfs f0,104(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 104, temp.u32);
	// addi r8,r8,-19152
	ctx.r8.s64 = ctx.r8.s64 + -19152;
	// lfs f13,-6944(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -6944);
	ctx.f13.f64 = double(temp.f32);
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// stfs f13,88(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// addi r7,r7,-19112
	ctx.r7.s64 = ctx.r7.s64 + -19112;
	// lfs f13,2592(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 2592);
	ctx.f13.f64 = double(temp.f32);
	// lis r10,-32247
	ctx.r10.s64 = -2113339392;
	// stfs f13,1232(r31)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r31.u32 + 1232, temp.u32);
	// addi r11,r11,-19076
	ctx.r11.s64 = ctx.r11.s64 + -19076;
	// lfs f0,-19032(r6)
	temp.u32 = PPC_LOAD_U32(ctx.r6.u32 + -19032);
	ctx.f0.f64 = double(temp.f32);
	// addi r10,r10,-19096
	ctx.r10.s64 = ctx.r10.s64 + -19096;
	// stfs f0,1236(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r31.u32 + 1236, temp.u32);
	// lis r6,-32247
	ctx.r6.s64 = -2113339392;
	// lfs f0,-6928(r5)
	temp.u32 = PPC_LOAD_U32(ctx.r5.u32 + -6928);
	ctx.f0.f64 = double(temp.f32);
	// li r5,0
	ctx.r5.s64 = 0;
	// stfs f0,1240(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r31.u32 + 1240, temp.u32);
	// addi r6,r6,-19132
	ctx.r6.s64 = ctx.r6.s64 + -19132;
	// stfs f13,100(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 100, temp.u32);
	// addi r3,r31,1328
	ctx.r3.s64 = ctx.r31.s64 + 1328;
	// stw r5,1284(r31)
	PPC_STORE_U32(ctx.r31.u32 + 1284, ctx.r5.u32);
	// stw r5,1288(r31)
	PPC_STORE_U32(ctx.r31.u32 + 1288, ctx.r5.u32);
	// stw r11,1244(r31)
	PPC_STORE_U32(ctx.r31.u32 + 1244, ctx.r11.u32);
	// li r11,255
	ctx.r11.s64 = 255;
	// stw r10,1248(r31)
	PPC_STORE_U32(ctx.r31.u32 + 1248, ctx.r10.u32);
	// stw r9,1252(r31)
	PPC_STORE_U32(ctx.r31.u32 + 1252, ctx.r9.u32);
	// stw r8,1256(r31)
	PPC_STORE_U32(ctx.r31.u32 + 1256, ctx.r8.u32);
	// stw r7,1260(r31)
	PPC_STORE_U32(ctx.r31.u32 + 1260, ctx.r7.u32);
	// stw r6,1264(r31)
	PPC_STORE_U32(ctx.r31.u32 + 1264, ctx.r6.u32);
	// stw r11,1268(r31)
	PPC_STORE_U32(ctx.r31.u32 + 1268, ctx.r11.u32);
	// li r11,96
	ctx.r11.s64 = 96;
	// stw r11,1276(r31)
	PPC_STORE_U32(ctx.r31.u32 + 1276, ctx.r11.u32);
	// li r11,24
	ctx.r11.s64 = 24;
	// stw r11,1272(r31)
	PPC_STORE_U32(ctx.r31.u32 + 1272, ctx.r11.u32);
	// li r11,4
	ctx.r11.s64 = 4;
	// stw r11,1280(r31)
	PPC_STORE_U32(ctx.r31.u32 + 1280, ctx.r11.u32);
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r11,832
	ctx.r11.s64 = 832;
	// stvx128 v0,r31,r11
	ea = (ctx.r31.u32 + ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,96
	ctx.r11.s64 = ctx.r1.s64 + 96;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// li r11,816
	ctx.r11.s64 = 816;
	// stvx128 v0,r31,r11
	ea = (ctx.r31.u32 + ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x82896300
	ctx.lr = 0x829FAD28;
	sub_82896300(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,4048
	ctx.r3.s64 = ctx.r11.s64 + 4048;
	// bl 0x8298ed98
	ctx.lr = 0x829FAD34;
	sub_8298ED98(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829FAD48);  /* forward decl */
PPC_WEAK_FUNC(sub_829FAD48) { __imp__sub_829FAD48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FAD48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-18980
	ctx.r5.s64 = ctx.r11.s64 + -18980;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-21020
	ctx.r3.s64 = ctx.r11.s64 + -21020;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FAD68);  /* forward decl */
PPC_WEAK_FUNC(sub_829FAD68) { __imp__sub_829FAD68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FAD68) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-18972
	ctx.r5.s64 = ctx.r11.s64 + -18972;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-21040
	ctx.r3.s64 = ctx.r11.s64 + -21040;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FAD88);  /* forward decl */
PPC_WEAK_FUNC(sub_829FAD88) { __imp__sub_829FAD88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FAD88) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-18964
	ctx.r5.s64 = ctx.r11.s64 + -18964;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-21000
	ctx.r3.s64 = ctx.r11.s64 + -21000;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FADA8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FADA8) { __imp__sub_829FADA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FADA8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-18948
	ctx.r5.s64 = ctx.r11.s64 + -18948;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-20980
	ctx.r3.s64 = ctx.r11.s64 + -20980;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FADC8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FADC8) { __imp__sub_829FADC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FADC8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-5040
	ctx.r3.s64 = ctx.r11.s64 + -5040;
	// bl 0x827df490
	ctx.lr = 0x829FADE4;
	sub_827DF490(ctx, base);
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// stw r3,-20960(r11)
	PPC_STORE_U32(ctx.r11.u32 + -20960, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FAE00);  /* forward decl */
PPC_WEAK_FUNC(sub_829FAE00) { __imp__sub_829FAE00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FAE00) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-16428
	ctx.r5.s64 = ctx.r11.s64 + -16428;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-20748
	ctx.r3.s64 = ctx.r11.s64 + -20748;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FAE20);  /* forward decl */
PPC_WEAK_FUNC(sub_829FAE20) { __imp__sub_829FAE20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FAE20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-16412
	ctx.r5.s64 = ctx.r11.s64 + -16412;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-20844
	ctx.r3.s64 = ctx.r11.s64 + -20844;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FAE40);  /* forward decl */
PPC_WEAK_FUNC(sub_829FAE40) { __imp__sub_829FAE40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FAE40) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,4200
	ctx.r3.s64 = ctx.r11.s64 + 4200;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FAE50);  /* forward decl */
PPC_WEAK_FUNC(sub_829FAE50) { __imp__sub_829FAE50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FAE50) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,4184
	ctx.r3.s64 = ctx.r11.s64 + 4184;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FAE60);  /* forward decl */
PPC_WEAK_FUNC(sub_829FAE60) { __imp__sub_829FAE60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FAE60) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-20780
	ctx.r3.s64 = ctx.r11.s64 + -20780;
	// bl 0x827f1320
	ctx.lr = 0x829FAE78;
	sub_827F1320(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,4152
	ctx.r3.s64 = ctx.r11.s64 + 4152;
	// bl 0x8298ed98
	ctx.lr = 0x829FAE84;
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

PPC_FUNC_IMPL(__imp__sub_829FAE98);  /* forward decl */
PPC_WEAK_FUNC(sub_829FAE98) { __imp__sub_829FAE98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FAE98) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-20812
	ctx.r3.s64 = ctx.r11.s64 + -20812;
	// bl 0x827f1320
	ctx.lr = 0x829FAEB0;
	sub_827F1320(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,4168
	ctx.r3.s64 = ctx.r11.s64 + 4168;
	// bl 0x8298ed98
	ctx.lr = 0x829FAEBC;
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

PPC_FUNC_IMPL(__imp__sub_829FAED0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FAED0) { __imp__sub_829FAED0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FAED0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-16040
	ctx.r5.s64 = ctx.r11.s64 + -16040;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-19844
	ctx.r3.s64 = ctx.r11.s64 + -19844;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FAEF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FAEF0) { __imp__sub_829FAEF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FAEF0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,4216
	ctx.r3.s64 = ctx.r11.s64 + 4216;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FAF00);  /* forward decl */
PPC_WEAK_FUNC(sub_829FAF00) { __imp__sub_829FAF00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FAF00) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-15668
	ctx.r5.s64 = ctx.r11.s64 + -15668;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-19568
	ctx.r3.s64 = ctx.r11.s64 + -19568;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FAF20);  /* forward decl */
PPC_WEAK_FUNC(sub_829FAF20) { __imp__sub_829FAF20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FAF20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-14420
	ctx.r5.s64 = ctx.r11.s64 + -14420;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-18928
	ctx.r3.s64 = ctx.r11.s64 + -18928;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FAF40);  /* forward decl */
PPC_WEAK_FUNC(sub_829FAF40) { __imp__sub_829FAF40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FAF40) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-14404
	ctx.r5.s64 = ctx.r11.s64 + -14404;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-18908
	ctx.r3.s64 = ctx.r11.s64 + -18908;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FAF60);  /* forward decl */
PPC_WEAK_FUNC(sub_829FAF60) { __imp__sub_829FAF60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FAF60) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-13156
	ctx.r5.s64 = ctx.r11.s64 + -13156;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-18856
	ctx.r3.s64 = ctx.r11.s64 + -18856;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FAF80);  /* forward decl */
PPC_WEAK_FUNC(sub_829FAF80) { __imp__sub_829FAF80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FAF80) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-13144
	ctx.r5.s64 = ctx.r11.s64 + -13144;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-18876
	ctx.r3.s64 = ctx.r11.s64 + -18876;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FAFA0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FAFA0) { __imp__sub_829FAFA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FAFA0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,4232
	ctx.r3.s64 = ctx.r11.s64 + 4232;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FAFB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FAFB0) { __imp__sub_829FAFB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FAFB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,4264
	ctx.r3.s64 = ctx.r11.s64 + 4264;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FAFC0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FAFC0) { __imp__sub_829FAFC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FAFC0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-9140
	ctx.r5.s64 = ctx.r11.s64 + -9140;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-18760
	ctx.r3.s64 = ctx.r11.s64 + -18760;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FAFE0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FAFE0) { __imp__sub_829FAFE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FAFE0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-7204
	ctx.r5.s64 = ctx.r11.s64 + -7204;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-18716
	ctx.r3.s64 = ctx.r11.s64 + -18716;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FB000);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB000) { __imp__sub_829FB000(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB000) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-7192
	ctx.r5.s64 = ctx.r11.s64 + -7192;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-18696
	ctx.r3.s64 = ctx.r11.s64 + -18696;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FB020);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB020) { __imp__sub_829FB020(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB020) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-6752
	ctx.r5.s64 = ctx.r11.s64 + -6752;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,-18460
	ctx.r3.s64 = ctx.r11.s64 + -18460;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FB040);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB040) { __imp__sub_829FB040(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB040) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// addi r4,r11,5388
	ctx.r4.s64 = ctx.r11.s64 + 5388;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r3,r11,-3832
	ctx.r3.s64 = ctx.r11.s64 + -3832;
	// b 0x827f9f70
	sub_827F9F70(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FB058);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB058) { __imp__sub_829FB058(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB058) {
	PPC_FUNC_PROLOGUE();
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
	// addi r4,r11,-27212
	ctx.r4.s64 = ctx.r11.s64 + -27212;
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// addi r5,r4,1
	ctx.r5.s64 = ctx.r4.s64 + 1;
	// addi r3,r11,13844
	ctx.r3.s64 = ctx.r11.s64 + 13844;
	// bl 0x82736b28
	ctx.lr = 0x829FB07C;
	sub_82736B28(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,4296
	ctx.r3.s64 = ctx.r11.s64 + 4296;
	// bl 0x8298ed98
	ctx.lr = 0x829FB088;
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

PPC_FUNC_IMPL(__imp__sub_829FB098);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB098) { __imp__sub_829FB098(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB098) {
	PPC_FUNC_PROLOGUE();
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
	// addi r4,r11,24232
	ctx.r4.s64 = ctx.r11.s64 + 24232;
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// addi r5,r4,4
	ctx.r5.s64 = ctx.r4.s64 + 4;
	// addi r3,r11,13888
	ctx.r3.s64 = ctx.r11.s64 + 13888;
	// bl 0x82736b28
	ctx.lr = 0x829FB0BC;
	sub_82736B28(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,4344
	ctx.r3.s64 = ctx.r11.s64 + 4344;
	// bl 0x8298ed98
	ctx.lr = 0x829FB0C8;
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

PPC_FUNC_IMPL(__imp__sub_829FB0D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB0D8) { __imp__sub_829FB0D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB0D8) {
	PPC_FUNC_PROLOGUE();
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
	// addi r4,r11,24224
	ctx.r4.s64 = ctx.r11.s64 + 24224;
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// addi r5,r4,5
	ctx.r5.s64 = ctx.r4.s64 + 5;
	// addi r3,r11,13912
	ctx.r3.s64 = ctx.r11.s64 + 13912;
	// bl 0x82736b28
	ctx.lr = 0x829FB0FC;
	sub_82736B28(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,4392
	ctx.r3.s64 = ctx.r11.s64 + 4392;
	// bl 0x8298ed98
	ctx.lr = 0x829FB108;
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

PPC_FUNC_IMPL(__imp__sub_829FB118);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB118) { __imp__sub_829FB118(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB118) {
	PPC_FUNC_PROLOGUE();
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
	// addi r5,r11,-28372
	ctx.r5.s64 = ctx.r11.s64 + -28372;
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// mr r4,r5
	ctx.r4.u64 = ctx.r5.u64;
	// addi r3,r11,13936
	ctx.r3.s64 = ctx.r11.s64 + 13936;
	// bl 0x82736b28
	ctx.lr = 0x829FB13C;
	sub_82736B28(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,4440
	ctx.r3.s64 = ctx.r11.s64 + 4440;
	// bl 0x8298ed98
	ctx.lr = 0x829FB148;
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

PPC_FUNC_IMPL(__imp__sub_829FB158);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB158) { __imp__sub_829FB158(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB158) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// addi r31,r11,6320
	ctx.r31.s64 = ctx.r11.s64 + 6320;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82993598
	ctx.lr = 0x829FB178;
	sub_82993598(ctx, base);
	// rlwinm r11,r3,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 1) & 0xFFFFFFFE;
	// lis r10,-32087
	ctx.r10.s64 = -2102853632;
	// add r5,r11,r31
	ctx.r5.u64 = ctx.r11.u64 + ctx.r31.u64;
	// addi r3,r10,13960
	ctx.r3.s64 = ctx.r10.s64 + 13960;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x828ca210
	ctx.lr = 0x829FB190;
	sub_828CA210(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,4488
	ctx.r3.s64 = ctx.r11.s64 + 4488;
	// bl 0x8298ed98
	ctx.lr = 0x829FB19C;
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

PPC_FUNC_IMPL(__imp__sub_829FB1B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB1B0) { __imp__sub_829FB1B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB1B0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// addi r31,r11,6332
	ctx.r31.s64 = ctx.r11.s64 + 6332;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82993598
	ctx.lr = 0x829FB1D0;
	sub_82993598(ctx, base);
	// rlwinm r11,r3,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 1) & 0xFFFFFFFE;
	// lis r10,-32087
	ctx.r10.s64 = -2102853632;
	// add r5,r11,r31
	ctx.r5.u64 = ctx.r11.u64 + ctx.r31.u64;
	// addi r3,r10,14000
	ctx.r3.s64 = ctx.r10.s64 + 14000;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x828ca210
	ctx.lr = 0x829FB1E8;
	sub_828CA210(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,4536
	ctx.r3.s64 = ctx.r11.s64 + 4536;
	// bl 0x8298ed98
	ctx.lr = 0x829FB1F4;
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

PPC_FUNC_IMPL(__imp__sub_829FB208);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB208) { __imp__sub_829FB208(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB208) {
	PPC_FUNC_PROLOGUE();
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
	// addi r5,r11,-28372
	ctx.r5.s64 = ctx.r11.s64 + -28372;
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// mr r4,r5
	ctx.r4.u64 = ctx.r5.u64;
	// addi r3,r11,14040
	ctx.r3.s64 = ctx.r11.s64 + 14040;
	// bl 0x82736b28
	ctx.lr = 0x829FB22C;
	sub_82736B28(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,4584
	ctx.r3.s64 = ctx.r11.s64 + 4584;
	// bl 0x8298ed98
	ctx.lr = 0x829FB238;
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

PPC_FUNC_IMPL(__imp__sub_829FB248);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB248) { __imp__sub_829FB248(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB248) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,4632
	ctx.r3.s64 = ctx.r11.s64 + 4632;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FB258);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB258) { __imp__sub_829FB258(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB258) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,4680
	ctx.r3.s64 = ctx.r11.s64 + 4680;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FB268);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB268) { __imp__sub_829FB268(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB268) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,-3688
	ctx.r3.s64 = ctx.r11.s64 + -3688;
	// bl 0x827f1320
	ctx.lr = 0x829FB280;
	sub_827F1320(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,4728
	ctx.r3.s64 = ctx.r11.s64 + 4728;
	// bl 0x8298ed98
	ctx.lr = 0x829FB28C;
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

PPC_FUNC_IMPL(__imp__sub_829FB2A0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB2A0) { __imp__sub_829FB2A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB2A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,11052
	ctx.r5.s64 = ctx.r11.s64 + 11052;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,656
	ctx.r3.s64 = ctx.r11.s64 + 656;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FB2C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB2C0) { __imp__sub_829FB2C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB2C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,11064
	ctx.r5.s64 = ctx.r11.s64 + 11064;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,636
	ctx.r3.s64 = ctx.r11.s64 + 636;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FB2E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB2E0) { __imp__sub_829FB2E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB2E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,16924
	ctx.r5.s64 = ctx.r11.s64 + 16924;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,804
	ctx.r3.s64 = ctx.r11.s64 + 804;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FB300);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB300) { __imp__sub_829FB300(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB300) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,16948
	ctx.r5.s64 = ctx.r11.s64 + 16948;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,784
	ctx.r3.s64 = ctx.r11.s64 + 784;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FB320);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB320) { __imp__sub_829FB320(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB320) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,16968
	ctx.r5.s64 = ctx.r11.s64 + 16968;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,864
	ctx.r3.s64 = ctx.r11.s64 + 864;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FB340);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB340) { __imp__sub_829FB340(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB340) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,16988
	ctx.r5.s64 = ctx.r11.s64 + 16988;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,844
	ctx.r3.s64 = ctx.r11.s64 + 844;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FB360);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB360) { __imp__sub_829FB360(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB360) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,17000
	ctx.r5.s64 = ctx.r11.s64 + 17000;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,824
	ctx.r3.s64 = ctx.r11.s64 + 824;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FB380);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB380) { __imp__sub_829FB380(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB380) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32087
	ctx.r11.s64 = -2102853632;
	// lfs f0,17656(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 17656);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r11,r11,1408
	ctx.r11.s64 = ctx.r11.s64 + 1408;
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stfs f0,8(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 8, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FB3A0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB3A0) { __imp__sub_829FB3A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB3A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,4744
	ctx.r3.s64 = ctx.r11.s64 + 4744;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FB3B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB3B0) { __imp__sub_829FB3B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB3B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,4768
	ctx.r3.s64 = ctx.r11.s64 + 4768;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FB3C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB3C0) { __imp__sub_829FB3C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB3C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,4792
	ctx.r3.s64 = ctx.r11.s64 + 4792;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FB3D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB3D0) { __imp__sub_829FB3D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB3D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,4816
	ctx.r3.s64 = ctx.r11.s64 + 4816;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FB3E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB3E0) { __imp__sub_829FB3E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB3E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,4840
	ctx.r3.s64 = ctx.r11.s64 + 4840;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FB3F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB3F0) { __imp__sub_829FB3F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB3F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,4928
	ctx.r3.s64 = ctx.r11.s64 + 4928;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FB400);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB400) { __imp__sub_829FB400(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB400) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,5016
	ctx.r3.s64 = ctx.r11.s64 + 5016;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FB410);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB410) { __imp__sub_829FB410(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB410) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,5040
	ctx.r3.s64 = ctx.r11.s64 + 5040;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FB420);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB420) { __imp__sub_829FB420(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB420) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,5064
	ctx.r3.s64 = ctx.r11.s64 + 5064;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FB430);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB430) { __imp__sub_829FB430(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB430) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r3,r11,3080
	ctx.r3.s64 = ctx.r11.s64 + 3080;
	// b 0x82973ee0
	sub_82973EE0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FB440);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB440) { __imp__sub_829FB440(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB440) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,32072
	ctx.r5.s64 = ctx.r11.s64 + 32072;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r31,r11,3088
	ctx.r31.s64 = ctx.r11.s64 + 3088;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829FB470;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FB480;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FB488;
	sub_8296C788(ctx, base);
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,32068
	ctx.r11.s64 = ctx.r11.s64 + 32068;
	// addi r3,r10,5088
	ctx.r3.s64 = ctx.r10.s64 + 5088;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FB4A0;
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

PPC_FUNC_IMPL(__imp__sub_829FB4B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB4B8) { __imp__sub_829FB4B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB4B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,32092
	ctx.r5.s64 = ctx.r11.s64 + 32092;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,3144
	ctx.r3.s64 = ctx.r11.s64 + 3144;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FB4D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB4D8) { __imp__sub_829FB4D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB4D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,32144
	ctx.r5.s64 = ctx.r11.s64 + 32144;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,3164
	ctx.r3.s64 = ctx.r11.s64 + 3164;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FB4F8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB4F8) { __imp__sub_829FB4F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB4F8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,32492
	ctx.r5.s64 = ctx.r11.s64 + 32492;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r31,r11,3424
	ctx.r31.s64 = ctx.r11.s64 + 3424;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829FB528;
	sub_8296C718(ctx, base);
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// addi r11,r11,32480
	ctx.r11.s64 = ctx.r11.s64 + 32480;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82974d60
	ctx.lr = 0x829FB538;
	sub_82974D60(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FB540;
	sub_8296C788(ctx, base);
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,32488
	ctx.r11.s64 = ctx.r11.s64 + 32488;
	// addi r3,r10,5184
	ctx.r3.s64 = ctx.r10.s64 + 5184;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FB558;
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

PPC_FUNC_IMPL(__imp__sub_829FB570);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB570) { __imp__sub_829FB570(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB570) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r7,r11,3424
	ctx.r7.s64 = ctx.r11.s64 + 3424;
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,32516
	ctx.r6.s64 = ctx.r11.s64 + 32516;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,3264
	ctx.r31.s64 = ctx.r11.s64 + 3264;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FB5A8;
	sub_8296C750(ctx, base);
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// addi r11,r11,32480
	ctx.r11.s64 = ctx.r11.s64 + 32480;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82974d60
	ctx.lr = 0x829FB5B8;
	sub_82974D60(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FB5C0;
	sub_8296C788(ctx, base);
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,32512
	ctx.r11.s64 = ctx.r11.s64 + 32512;
	// addi r3,r10,5264
	ctx.r3.s64 = ctx.r10.s64 + 5264;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FB5D8;
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

PPC_FUNC_IMPL(__imp__sub_829FB5F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB5F0) { __imp__sub_829FB5F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB5F0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r7,r11,3424
	ctx.r7.s64 = ctx.r11.s64 + 3424;
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,32560
	ctx.r6.s64 = ctx.r11.s64 + 32560;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,3296
	ctx.r31.s64 = ctx.r11.s64 + 3296;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FB628;
	sub_8296C750(ctx, base);
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// addi r11,r11,32480
	ctx.r11.s64 = ctx.r11.s64 + 32480;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82974d60
	ctx.lr = 0x829FB638;
	sub_82974D60(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FB640;
	sub_8296C788(ctx, base);
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,32556
	ctx.r11.s64 = ctx.r11.s64 + 32556;
	// addi r3,r10,5344
	ctx.r3.s64 = ctx.r10.s64 + 5344;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FB658;
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

PPC_FUNC_IMPL(__imp__sub_829FB670);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB670) { __imp__sub_829FB670(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB670) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,2
	ctx.r5.s64 = 2;
	// addi r7,r11,3424
	ctx.r7.s64 = ctx.r11.s64 + 3424;
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,32600
	ctx.r6.s64 = ctx.r11.s64 + 32600;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,3328
	ctx.r31.s64 = ctx.r11.s64 + 3328;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FB6A8;
	sub_8296C750(ctx, base);
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// addi r11,r11,32480
	ctx.r11.s64 = ctx.r11.s64 + 32480;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82974d60
	ctx.lr = 0x829FB6B8;
	sub_82974D60(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FB6C0;
	sub_8296C788(ctx, base);
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,32596
	ctx.r11.s64 = ctx.r11.s64 + 32596;
	// addi r3,r10,5424
	ctx.r3.s64 = ctx.r10.s64 + 5424;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FB6D8;
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

PPC_FUNC_IMPL(__imp__sub_829FB6F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB6F0) { __imp__sub_829FB6F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB6F0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,3
	ctx.r5.s64 = 3;
	// addi r7,r11,3424
	ctx.r7.s64 = ctx.r11.s64 + 3424;
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,32644
	ctx.r6.s64 = ctx.r11.s64 + 32644;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,3392
	ctx.r31.s64 = ctx.r11.s64 + 3392;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FB728;
	sub_8296C750(ctx, base);
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// addi r11,r11,32480
	ctx.r11.s64 = ctx.r11.s64 + 32480;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82974d60
	ctx.lr = 0x829FB738;
	sub_82974D60(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FB740;
	sub_8296C788(ctx, base);
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,32640
	ctx.r11.s64 = ctx.r11.s64 + 32640;
	// addi r3,r10,5504
	ctx.r3.s64 = ctx.r10.s64 + 5504;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FB758;
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

PPC_FUNC_IMPL(__imp__sub_829FB770);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB770) { __imp__sub_829FB770(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB770) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,4
	ctx.r5.s64 = 4;
	// addi r7,r11,3424
	ctx.r7.s64 = ctx.r11.s64 + 3424;
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,32684
	ctx.r6.s64 = ctx.r11.s64 + 32684;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,3232
	ctx.r31.s64 = ctx.r11.s64 + 3232;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FB7A8;
	sub_8296C750(ctx, base);
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// addi r11,r11,32480
	ctx.r11.s64 = ctx.r11.s64 + 32480;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82974d60
	ctx.lr = 0x829FB7B8;
	sub_82974D60(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FB7C0;
	sub_8296C788(ctx, base);
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,32680
	ctx.r11.s64 = ctx.r11.s64 + 32680;
	// addi r3,r10,5584
	ctx.r3.s64 = ctx.r10.s64 + 5584;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FB7D8;
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

PPC_FUNC_IMPL(__imp__sub_829FB7F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB7F0) { __imp__sub_829FB7F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB7F0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,5
	ctx.r5.s64 = 5;
	// addi r7,r11,3424
	ctx.r7.s64 = ctx.r11.s64 + 3424;
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,32728
	ctx.r6.s64 = ctx.r11.s64 + 32728;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,3360
	ctx.r31.s64 = ctx.r11.s64 + 3360;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FB828;
	sub_8296C750(ctx, base);
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// addi r11,r11,32480
	ctx.r11.s64 = ctx.r11.s64 + 32480;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82974d60
	ctx.lr = 0x829FB838;
	sub_82974D60(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FB840;
	sub_8296C788(ctx, base);
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,32724
	ctx.r11.s64 = ctx.r11.s64 + 32724;
	// addi r3,r10,5664
	ctx.r3.s64 = ctx.r10.s64 + 5664;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FB858;
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

PPC_FUNC_IMPL(__imp__sub_829FB870);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB870) { __imp__sub_829FB870(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB870) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,6
	ctx.r5.s64 = 6;
	// addi r7,r11,3424
	ctx.r7.s64 = ctx.r11.s64 + 3424;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-32768
	ctx.r6.s64 = ctx.r11.s64 + -32768;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,3456
	ctx.r31.s64 = ctx.r11.s64 + 3456;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FB8A8;
	sub_8296C750(ctx, base);
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// addi r11,r11,32480
	ctx.r11.s64 = ctx.r11.s64 + 32480;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x82974d60
	ctx.lr = 0x829FB8B8;
	sub_82974D60(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FB8C0;
	sub_8296C788(ctx, base);
	// lis r11,-32247
	ctx.r11.s64 = -2113339392;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,32764
	ctx.r11.s64 = ctx.r11.s64 + 32764;
	// addi r3,r10,5744
	ctx.r3.s64 = ctx.r10.s64 + 5744;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FB8D8;
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

PPC_FUNC_IMPL(__imp__sub_829FB8F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB8F0) { __imp__sub_829FB8F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB8F0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r5,r11,-32444
	ctx.r5.s64 = ctx.r11.s64 + -32444;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,3488
	ctx.r31.s64 = ctx.r11.s64 + 3488;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829FB924;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FB934;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FB93C;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-32448
	ctx.r11.s64 = ctx.r11.s64 + -32448;
	// addi r3,r10,5824
	ctx.r3.s64 = ctx.r10.s64 + 5824;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FB954;
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

PPC_FUNC_IMPL(__imp__sub_829FB968);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB968) { __imp__sub_829FB968(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB968) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r5,r11,-32140
	ctx.r5.s64 = ctx.r11.s64 + -32140;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,3552
	ctx.r31.s64 = ctx.r11.s64 + 3552;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829FB99C;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FB9AC;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FB9B4;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-32144
	ctx.r11.s64 = ctx.r11.s64 + -32144;
	// addi r3,r10,5904
	ctx.r3.s64 = ctx.r10.s64 + 5904;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FB9CC;
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

PPC_FUNC_IMPL(__imp__sub_829FB9E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FB9E0) { __imp__sub_829FB9E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FB9E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r5,r11,-32112
	ctx.r5.s64 = ctx.r11.s64 + -32112;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,3520
	ctx.r31.s64 = ctx.r11.s64 + 3520;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829FBA14;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FBA24;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FBA2C;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-32116
	ctx.r11.s64 = ctx.r11.s64 + -32116;
	// addi r3,r10,5984
	ctx.r3.s64 = ctx.r10.s64 + 5984;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FBA44;
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

PPC_FUNC_IMPL(__imp__sub_829FBA58);  /* forward decl */
PPC_WEAK_FUNC(sub_829FBA58) { __imp__sub_829FBA58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FBA58) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-32092
	ctx.r5.s64 = ctx.r11.s64 + -32092;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,3584
	ctx.r3.s64 = ctx.r11.s64 + 3584;
	// b 0x827eedb8
	sub_827EEDB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FBA78);  /* forward decl */
PPC_WEAK_FUNC(sub_829FBA78) { __imp__sub_829FBA78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FBA78) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r5,r11,-31800
	ctx.r5.s64 = ctx.r11.s64 + -31800;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,3604
	ctx.r31.s64 = ctx.r11.s64 + 3604;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829FBAAC;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FBABC;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FBAC4;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-31804
	ctx.r11.s64 = ctx.r11.s64 + -31804;
	// addi r3,r10,6064
	ctx.r3.s64 = ctx.r10.s64 + 6064;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FBADC;
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

PPC_FUNC_IMPL(__imp__sub_829FBAF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FBAF0) { __imp__sub_829FBAF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FBAF0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r5,r11,-30992
	ctx.r5.s64 = ctx.r11.s64 + -30992;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,3640
	ctx.r31.s64 = ctx.r11.s64 + 3640;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829FBB24;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FBB34;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FBB3C;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-30996
	ctx.r11.s64 = ctx.r11.s64 + -30996;
	// addi r3,r10,6144
	ctx.r3.s64 = ctx.r10.s64 + 6144;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FBB54;
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

PPC_FUNC_IMPL(__imp__sub_829FBB68);  /* forward decl */
PPC_WEAK_FUNC(sub_829FBB68) { __imp__sub_829FBB68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FBB68) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,3088
	ctx.r6.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r5,r11,-30960
	ctx.r5.s64 = ctx.r11.s64 + -30960;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,3672
	ctx.r31.s64 = ctx.r11.s64 + 3672;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829FBB9C;
	sub_8296C718(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FBBAC;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FBBB4;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-30964
	ctx.r11.s64 = ctx.r11.s64 + -30964;
	// addi r3,r10,6224
	ctx.r3.s64 = ctx.r10.s64 + 6224;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FBBCC;
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

PPC_FUNC_IMPL(__imp__sub_829FBBE0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FBBE0) { __imp__sub_829FBBE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FBBE0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r6,0
	ctx.r6.s64 = 0;
	// addi r5,r11,-30664
	ctx.r5.s64 = ctx.r11.s64 + -30664;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r31,r11,3764
	ctx.r31.s64 = ctx.r11.s64 + 3764;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c718
	ctx.lr = 0x829FBC10;
	sub_8296C718(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r11,r11,-30676
	ctx.r11.s64 = ctx.r11.s64 + -30676;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298a440
	ctx.lr = 0x829FBC20;
	sub_8298A440(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FBC28;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-30668
	ctx.r11.s64 = ctx.r11.s64 + -30668;
	// addi r3,r10,6320
	ctx.r3.s64 = ctx.r10.s64 + 6320;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FBC40;
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

PPC_FUNC_IMPL(__imp__sub_829FBC58);  /* forward decl */
PPC_WEAK_FUNC(sub_829FBC58) { __imp__sub_829FBC58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FBC58) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r7,r11,3764
	ctx.r7.s64 = ctx.r11.s64 + 3764;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-30604
	ctx.r6.s64 = ctx.r11.s64 + -30604;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,3860
	ctx.r31.s64 = ctx.r11.s64 + 3860;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FBC90;
	sub_8296C750(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r11,r11,-30676
	ctx.r11.s64 = ctx.r11.s64 + -30676;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298a440
	ctx.lr = 0x829FBCA0;
	sub_8298A440(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FBCA8;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-30608
	ctx.r11.s64 = ctx.r11.s64 + -30608;
	// addi r3,r10,6400
	ctx.r3.s64 = ctx.r10.s64 + 6400;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FBCC0;
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

PPC_FUNC_IMPL(__imp__sub_829FBCD8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FBCD8) { __imp__sub_829FBCD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FBCD8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r7,r11,3764
	ctx.r7.s64 = ctx.r11.s64 + 3764;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-30644
	ctx.r6.s64 = ctx.r11.s64 + -30644;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,3796
	ctx.r31.s64 = ctx.r11.s64 + 3796;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FBD10;
	sub_8296C750(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r11,r11,-30676
	ctx.r11.s64 = ctx.r11.s64 + -30676;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298a440
	ctx.lr = 0x829FBD20;
	sub_8298A440(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FBD28;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-30648
	ctx.r11.s64 = ctx.r11.s64 + -30648;
	// addi r3,r10,6480
	ctx.r3.s64 = ctx.r10.s64 + 6480;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FBD40;
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

PPC_FUNC_IMPL(__imp__sub_829FBD58);  /* forward decl */
PPC_WEAK_FUNC(sub_829FBD58) { __imp__sub_829FBD58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FBD58) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,2
	ctx.r5.s64 = 2;
	// addi r7,r11,3764
	ctx.r7.s64 = ctx.r11.s64 + 3764;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-30568
	ctx.r6.s64 = ctx.r11.s64 + -30568;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,3956
	ctx.r31.s64 = ctx.r11.s64 + 3956;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FBD90;
	sub_8296C750(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r11,r11,-30676
	ctx.r11.s64 = ctx.r11.s64 + -30676;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298a440
	ctx.lr = 0x829FBDA0;
	sub_8298A440(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FBDA8;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-30572
	ctx.r11.s64 = ctx.r11.s64 + -30572;
	// addi r3,r10,6560
	ctx.r3.s64 = ctx.r10.s64 + 6560;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FBDC0;
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

PPC_FUNC_IMPL(__imp__sub_829FBDD8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FBDD8) { __imp__sub_829FBDD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FBDD8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,3
	ctx.r5.s64 = 3;
	// addi r7,r11,3764
	ctx.r7.s64 = ctx.r11.s64 + 3764;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-30536
	ctx.r6.s64 = ctx.r11.s64 + -30536;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,3828
	ctx.r31.s64 = ctx.r11.s64 + 3828;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FBE10;
	sub_8296C750(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r11,r11,-30676
	ctx.r11.s64 = ctx.r11.s64 + -30676;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298a440
	ctx.lr = 0x829FBE20;
	sub_8298A440(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FBE28;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-30540
	ctx.r11.s64 = ctx.r11.s64 + -30540;
	// addi r3,r10,6640
	ctx.r3.s64 = ctx.r10.s64 + 6640;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FBE40;
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

PPC_FUNC_IMPL(__imp__sub_829FBE58);  /* forward decl */
PPC_WEAK_FUNC(sub_829FBE58) { __imp__sub_829FBE58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FBE58) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,4
	ctx.r5.s64 = 4;
	// addi r7,r11,3764
	ctx.r7.s64 = ctx.r11.s64 + 3764;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-30500
	ctx.r6.s64 = ctx.r11.s64 + -30500;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,3732
	ctx.r31.s64 = ctx.r11.s64 + 3732;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FBE90;
	sub_8296C750(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r11,r11,-30676
	ctx.r11.s64 = ctx.r11.s64 + -30676;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298a440
	ctx.lr = 0x829FBEA0;
	sub_8298A440(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FBEA8;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-30504
	ctx.r11.s64 = ctx.r11.s64 + -30504;
	// addi r3,r10,6720
	ctx.r3.s64 = ctx.r10.s64 + 6720;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FBEC0;
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

PPC_FUNC_IMPL(__imp__sub_829FBED8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FBED8) { __imp__sub_829FBED8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FBED8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,5
	ctx.r5.s64 = 5;
	// addi r7,r11,3764
	ctx.r7.s64 = ctx.r11.s64 + 3764;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-30468
	ctx.r6.s64 = ctx.r11.s64 + -30468;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,3892
	ctx.r31.s64 = ctx.r11.s64 + 3892;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FBF10;
	sub_8296C750(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r11,r11,-30676
	ctx.r11.s64 = ctx.r11.s64 + -30676;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298a440
	ctx.lr = 0x829FBF20;
	sub_8298A440(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FBF28;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-30472
	ctx.r11.s64 = ctx.r11.s64 + -30472;
	// addi r3,r10,6800
	ctx.r3.s64 = ctx.r10.s64 + 6800;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FBF40;
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

PPC_FUNC_IMPL(__imp__sub_829FBF58);  /* forward decl */
PPC_WEAK_FUNC(sub_829FBF58) { __imp__sub_829FBF58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FBF58) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,6
	ctx.r5.s64 = 6;
	// addi r7,r11,3764
	ctx.r7.s64 = ctx.r11.s64 + 3764;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-30440
	ctx.r6.s64 = ctx.r11.s64 + -30440;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,3988
	ctx.r31.s64 = ctx.r11.s64 + 3988;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FBF90;
	sub_8296C750(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r11,r11,-30676
	ctx.r11.s64 = ctx.r11.s64 + -30676;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298a440
	ctx.lr = 0x829FBFA0;
	sub_8298A440(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FBFA8;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-30444
	ctx.r11.s64 = ctx.r11.s64 + -30444;
	// addi r3,r10,6880
	ctx.r3.s64 = ctx.r10.s64 + 6880;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FBFC0;
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

PPC_FUNC_IMPL(__imp__sub_829FBFD8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FBFD8) { __imp__sub_829FBFD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FBFD8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,7
	ctx.r5.s64 = 7;
	// addi r7,r11,3764
	ctx.r7.s64 = ctx.r11.s64 + 3764;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-30404
	ctx.r6.s64 = ctx.r11.s64 + -30404;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,3924
	ctx.r31.s64 = ctx.r11.s64 + 3924;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FC010;
	sub_8296C750(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// addi r11,r11,-30676
	ctx.r11.s64 = ctx.r11.s64 + -30676;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298a440
	ctx.lr = 0x829FC020;
	sub_8298A440(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FC028;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-30408
	ctx.r11.s64 = ctx.r11.s64 + -30408;
	// addi r3,r10,6960
	ctx.r3.s64 = ctx.r10.s64 + 6960;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FC040;
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

PPC_FUNC_IMPL(__imp__sub_829FC058);  /* forward decl */
PPC_WEAK_FUNC(sub_829FC058) { __imp__sub_829FC058(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FC058) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,100
	ctx.r5.s64 = 100;
	// addi r7,r11,3088
	ctx.r7.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29924
	ctx.r6.s64 = ctx.r11.s64 + -29924;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,5720
	ctx.r31.s64 = ctx.r11.s64 + 5720;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FC090;
	sub_8296C750(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FC0A0;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FC0A8;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-29928
	ctx.r11.s64 = ctx.r11.s64 + -29928;
	// addi r3,r10,7128
	ctx.r3.s64 = ctx.r10.s64 + 7128;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FC0C0;
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

PPC_FUNC_IMPL(__imp__sub_829FC0D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FC0D8) { __imp__sub_829FC0D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FC0D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,101
	ctx.r5.s64 = 101;
	// addi r7,r11,3088
	ctx.r7.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29896
	ctx.r6.s64 = ctx.r11.s64 + -29896;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,5496
	ctx.r31.s64 = ctx.r11.s64 + 5496;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FC110;
	sub_8296C750(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FC120;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FC128;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-29900
	ctx.r11.s64 = ctx.r11.s64 + -29900;
	// addi r3,r10,7208
	ctx.r3.s64 = ctx.r10.s64 + 7208;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FC140;
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

PPC_FUNC_IMPL(__imp__sub_829FC158);  /* forward decl */
PPC_WEAK_FUNC(sub_829FC158) { __imp__sub_829FC158(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FC158) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,102
	ctx.r5.s64 = 102;
	// addi r7,r11,3088
	ctx.r7.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29856
	ctx.r6.s64 = ctx.r11.s64 + -29856;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,5560
	ctx.r31.s64 = ctx.r11.s64 + 5560;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FC190;
	sub_8296C750(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FC1A0;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FC1A8;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-29860
	ctx.r11.s64 = ctx.r11.s64 + -29860;
	// addi r3,r10,7288
	ctx.r3.s64 = ctx.r10.s64 + 7288;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FC1C0;
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

PPC_FUNC_IMPL(__imp__sub_829FC1D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FC1D8) { __imp__sub_829FC1D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FC1D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,103
	ctx.r5.s64 = 103;
	// addi r7,r11,3088
	ctx.r7.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29816
	ctx.r6.s64 = ctx.r11.s64 + -29816;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,5304
	ctx.r31.s64 = ctx.r11.s64 + 5304;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FC210;
	sub_8296C750(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FC220;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FC228;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-29820
	ctx.r11.s64 = ctx.r11.s64 + -29820;
	// addi r3,r10,7368
	ctx.r3.s64 = ctx.r10.s64 + 7368;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FC240;
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

PPC_FUNC_IMPL(__imp__sub_829FC258);  /* forward decl */
PPC_WEAK_FUNC(sub_829FC258) { __imp__sub_829FC258(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FC258) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,104
	ctx.r5.s64 = 104;
	// addi r7,r11,3088
	ctx.r7.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29776
	ctx.r6.s64 = ctx.r11.s64 + -29776;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,5176
	ctx.r31.s64 = ctx.r11.s64 + 5176;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FC290;
	sub_8296C750(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FC2A0;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FC2A8;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-29780
	ctx.r11.s64 = ctx.r11.s64 + -29780;
	// addi r3,r10,7448
	ctx.r3.s64 = ctx.r10.s64 + 7448;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FC2C0;
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

PPC_FUNC_IMPL(__imp__sub_829FC2D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FC2D8) { __imp__sub_829FC2D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FC2D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,105
	ctx.r5.s64 = 105;
	// addi r7,r11,3088
	ctx.r7.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29740
	ctx.r6.s64 = ctx.r11.s64 + -29740;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,5400
	ctx.r31.s64 = ctx.r11.s64 + 5400;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FC310;
	sub_8296C750(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FC320;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FC328;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-29744
	ctx.r11.s64 = ctx.r11.s64 + -29744;
	// addi r3,r10,7528
	ctx.r3.s64 = ctx.r10.s64 + 7528;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FC340;
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

PPC_FUNC_IMPL(__imp__sub_829FC358);  /* forward decl */
PPC_WEAK_FUNC(sub_829FC358) { __imp__sub_829FC358(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FC358) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,106
	ctx.r5.s64 = 106;
	// addi r7,r11,3088
	ctx.r7.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29692
	ctx.r6.s64 = ctx.r11.s64 + -29692;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,4180
	ctx.r31.s64 = ctx.r11.s64 + 4180;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FC390;
	sub_8296C750(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FC3A0;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FC3A8;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-29696
	ctx.r11.s64 = ctx.r11.s64 + -29696;
	// addi r3,r10,7608
	ctx.r3.s64 = ctx.r10.s64 + 7608;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FC3C0;
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

PPC_FUNC_IMPL(__imp__sub_829FC3D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FC3D8) { __imp__sub_829FC3D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FC3D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,107
	ctx.r5.s64 = 107;
	// addi r7,r11,3088
	ctx.r7.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29648
	ctx.r6.s64 = ctx.r11.s64 + -29648;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,5624
	ctx.r31.s64 = ctx.r11.s64 + 5624;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FC410;
	sub_8296C750(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FC420;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FC428;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-29652
	ctx.r11.s64 = ctx.r11.s64 + -29652;
	// addi r3,r10,7688
	ctx.r3.s64 = ctx.r10.s64 + 7688;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FC440;
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

PPC_FUNC_IMPL(__imp__sub_829FC458);  /* forward decl */
PPC_WEAK_FUNC(sub_829FC458) { __imp__sub_829FC458(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FC458) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,108
	ctx.r5.s64 = 108;
	// addi r7,r11,3088
	ctx.r7.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29604
	ctx.r6.s64 = ctx.r11.s64 + -29604;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,5208
	ctx.r31.s64 = ctx.r11.s64 + 5208;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FC490;
	sub_8296C750(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FC4A0;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FC4A8;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-29608
	ctx.r11.s64 = ctx.r11.s64 + -29608;
	// addi r3,r10,7768
	ctx.r3.s64 = ctx.r10.s64 + 7768;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FC4C0;
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

PPC_FUNC_IMPL(__imp__sub_829FC4D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FC4D8) { __imp__sub_829FC4D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FC4D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,109
	ctx.r5.s64 = 109;
	// addi r7,r11,3088
	ctx.r7.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29560
	ctx.r6.s64 = ctx.r11.s64 + -29560;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,5784
	ctx.r31.s64 = ctx.r11.s64 + 5784;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FC510;
	sub_8296C750(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FC520;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FC528;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-29564
	ctx.r11.s64 = ctx.r11.s64 + -29564;
	// addi r3,r10,7848
	ctx.r3.s64 = ctx.r10.s64 + 7848;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FC540;
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

PPC_FUNC_IMPL(__imp__sub_829FC558);  /* forward decl */
PPC_WEAK_FUNC(sub_829FC558) { __imp__sub_829FC558(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FC558) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,110
	ctx.r5.s64 = 110;
	// addi r7,r11,3088
	ctx.r7.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29516
	ctx.r6.s64 = ctx.r11.s64 + -29516;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,5144
	ctx.r31.s64 = ctx.r11.s64 + 5144;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FC590;
	sub_8296C750(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FC5A0;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FC5A8;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-29520
	ctx.r11.s64 = ctx.r11.s64 + -29520;
	// addi r3,r10,7928
	ctx.r3.s64 = ctx.r10.s64 + 7928;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FC5C0;
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

PPC_FUNC_IMPL(__imp__sub_829FC5D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FC5D8) { __imp__sub_829FC5D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FC5D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,111
	ctx.r5.s64 = 111;
	// addi r7,r11,3088
	ctx.r7.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29472
	ctx.r6.s64 = ctx.r11.s64 + -29472;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,5592
	ctx.r31.s64 = ctx.r11.s64 + 5592;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FC610;
	sub_8296C750(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FC620;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FC628;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-29476
	ctx.r11.s64 = ctx.r11.s64 + -29476;
	// addi r3,r10,8008
	ctx.r3.s64 = ctx.r10.s64 + 8008;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FC640;
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

PPC_FUNC_IMPL(__imp__sub_829FC658);  /* forward decl */
PPC_WEAK_FUNC(sub_829FC658) { __imp__sub_829FC658(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FC658) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,112
	ctx.r5.s64 = 112;
	// addi r7,r11,3088
	ctx.r7.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29432
	ctx.r6.s64 = ctx.r11.s64 + -29432;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,4212
	ctx.r31.s64 = ctx.r11.s64 + 4212;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FC690;
	sub_8296C750(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FC6A0;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FC6A8;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-29436
	ctx.r11.s64 = ctx.r11.s64 + -29436;
	// addi r3,r10,8088
	ctx.r3.s64 = ctx.r10.s64 + 8088;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FC6C0;
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

PPC_FUNC_IMPL(__imp__sub_829FC6D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FC6D8) { __imp__sub_829FC6D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FC6D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,113
	ctx.r5.s64 = 113;
	// addi r7,r11,3088
	ctx.r7.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29396
	ctx.r6.s64 = ctx.r11.s64 + -29396;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,5752
	ctx.r31.s64 = ctx.r11.s64 + 5752;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FC710;
	sub_8296C750(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FC720;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FC728;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-29400
	ctx.r11.s64 = ctx.r11.s64 + -29400;
	// addi r3,r10,8168
	ctx.r3.s64 = ctx.r10.s64 + 8168;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FC740;
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

PPC_FUNC_IMPL(__imp__sub_829FC758);  /* forward decl */
PPC_WEAK_FUNC(sub_829FC758) { __imp__sub_829FC758(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FC758) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,114
	ctx.r5.s64 = 114;
	// addi r7,r11,3088
	ctx.r7.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29356
	ctx.r6.s64 = ctx.r11.s64 + -29356;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,5656
	ctx.r31.s64 = ctx.r11.s64 + 5656;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FC790;
	sub_8296C750(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FC7A0;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FC7A8;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-29360
	ctx.r11.s64 = ctx.r11.s64 + -29360;
	// addi r3,r10,8248
	ctx.r3.s64 = ctx.r10.s64 + 8248;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FC7C0;
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

PPC_FUNC_IMPL(__imp__sub_829FC7D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FC7D8) { __imp__sub_829FC7D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FC7D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,115
	ctx.r5.s64 = 115;
	// addi r7,r11,3088
	ctx.r7.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29320
	ctx.r6.s64 = ctx.r11.s64 + -29320;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,4116
	ctx.r31.s64 = ctx.r11.s64 + 4116;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FC810;
	sub_8296C750(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FC820;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FC828;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-29324
	ctx.r11.s64 = ctx.r11.s64 + -29324;
	// addi r3,r10,8328
	ctx.r3.s64 = ctx.r10.s64 + 8328;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FC840;
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

PPC_FUNC_IMPL(__imp__sub_829FC858);  /* forward decl */
PPC_WEAK_FUNC(sub_829FC858) { __imp__sub_829FC858(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FC858) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,116
	ctx.r5.s64 = 116;
	// addi r7,r11,3088
	ctx.r7.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29280
	ctx.r6.s64 = ctx.r11.s64 + -29280;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,4148
	ctx.r31.s64 = ctx.r11.s64 + 4148;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FC890;
	sub_8296C750(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FC8A0;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FC8A8;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-29284
	ctx.r11.s64 = ctx.r11.s64 + -29284;
	// addi r3,r10,8408
	ctx.r3.s64 = ctx.r10.s64 + 8408;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FC8C0;
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

PPC_FUNC_IMPL(__imp__sub_829FC8D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FC8D8) { __imp__sub_829FC8D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FC8D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,117
	ctx.r5.s64 = 117;
	// addi r7,r11,3088
	ctx.r7.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29240
	ctx.r6.s64 = ctx.r11.s64 + -29240;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,5336
	ctx.r31.s64 = ctx.r11.s64 + 5336;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FC910;
	sub_8296C750(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FC920;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FC928;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-29244
	ctx.r11.s64 = ctx.r11.s64 + -29244;
	// addi r3,r10,8488
	ctx.r3.s64 = ctx.r10.s64 + 8488;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FC940;
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

PPC_FUNC_IMPL(__imp__sub_829FC958);  /* forward decl */
PPC_WEAK_FUNC(sub_829FC958) { __imp__sub_829FC958(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FC958) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,118
	ctx.r5.s64 = 118;
	// addi r7,r11,3088
	ctx.r7.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29200
	ctx.r6.s64 = ctx.r11.s64 + -29200;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,5240
	ctx.r31.s64 = ctx.r11.s64 + 5240;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FC990;
	sub_8296C750(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FC9A0;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FC9A8;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-29204
	ctx.r11.s64 = ctx.r11.s64 + -29204;
	// addi r3,r10,8568
	ctx.r3.s64 = ctx.r10.s64 + 8568;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FC9C0;
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

PPC_FUNC_IMPL(__imp__sub_829FC9D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FC9D8) { __imp__sub_829FC9D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FC9D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,119
	ctx.r5.s64 = 119;
	// addi r7,r11,3088
	ctx.r7.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29164
	ctx.r6.s64 = ctx.r11.s64 + -29164;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,5272
	ctx.r31.s64 = ctx.r11.s64 + 5272;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FCA10;
	sub_8296C750(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FCA20;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FCA28;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-29168
	ctx.r11.s64 = ctx.r11.s64 + -29168;
	// addi r3,r10,8648
	ctx.r3.s64 = ctx.r10.s64 + 8648;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FCA40;
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

PPC_FUNC_IMPL(__imp__sub_829FCA58);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCA58) { __imp__sub_829FCA58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCA58) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,120
	ctx.r5.s64 = 120;
	// addi r7,r11,3088
	ctx.r7.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29124
	ctx.r6.s64 = ctx.r11.s64 + -29124;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,5688
	ctx.r31.s64 = ctx.r11.s64 + 5688;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FCA90;
	sub_8296C750(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FCAA0;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FCAA8;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-29128
	ctx.r11.s64 = ctx.r11.s64 + -29128;
	// addi r3,r10,8728
	ctx.r3.s64 = ctx.r10.s64 + 8728;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FCAC0;
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

PPC_FUNC_IMPL(__imp__sub_829FCAD8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCAD8) { __imp__sub_829FCAD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCAD8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,121
	ctx.r5.s64 = 121;
	// addi r7,r11,3088
	ctx.r7.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29084
	ctx.r6.s64 = ctx.r11.s64 + -29084;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,5432
	ctx.r31.s64 = ctx.r11.s64 + 5432;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FCB10;
	sub_8296C750(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FCB20;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FCB28;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-29088
	ctx.r11.s64 = ctx.r11.s64 + -29088;
	// addi r3,r10,8808
	ctx.r3.s64 = ctx.r10.s64 + 8808;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FCB40;
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

PPC_FUNC_IMPL(__imp__sub_829FCB58);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCB58) { __imp__sub_829FCB58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCB58) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,122
	ctx.r5.s64 = 122;
	// addi r7,r11,3088
	ctx.r7.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29044
	ctx.r6.s64 = ctx.r11.s64 + -29044;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,5464
	ctx.r31.s64 = ctx.r11.s64 + 5464;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FCB90;
	sub_8296C750(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FCBA0;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FCBA8;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-29048
	ctx.r11.s64 = ctx.r11.s64 + -29048;
	// addi r3,r10,8888
	ctx.r3.s64 = ctx.r10.s64 + 8888;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FCBC0;
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

PPC_FUNC_IMPL(__imp__sub_829FCBD8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCBD8) { __imp__sub_829FCBD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCBD8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,123
	ctx.r5.s64 = 123;
	// addi r7,r11,3088
	ctx.r7.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-29004
	ctx.r6.s64 = ctx.r11.s64 + -29004;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,5368
	ctx.r31.s64 = ctx.r11.s64 + 5368;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FCC10;
	sub_8296C750(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FCC20;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FCC28;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-29008
	ctx.r11.s64 = ctx.r11.s64 + -29008;
	// addi r3,r10,8968
	ctx.r3.s64 = ctx.r10.s64 + 8968;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FCC40;
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

PPC_FUNC_IMPL(__imp__sub_829FCC58);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCC58) { __imp__sub_829FCC58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCC58) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r5,124
	ctx.r5.s64 = 124;
	// addi r7,r11,3088
	ctx.r7.s64 = ctx.r11.s64 + 3088;
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r6,r11,-28960
	ctx.r6.s64 = ctx.r11.s64 + -28960;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// addi r31,r11,5528
	ctx.r31.s64 = ctx.r11.s64 + 5528;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8296c750
	ctx.lr = 0x829FCC90;
	sub_8296C750(ctx, base);
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// addi r11,r11,29028
	ctx.r11.s64 = ctx.r11.s64 + 29028;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8214b360
	ctx.lr = 0x829FCCA0;
	sub_8214B360(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x8296c788
	ctx.lr = 0x829FCCA8;
	sub_8296C788(ctx, base);
	// lis r11,-32246
	ctx.r11.s64 = -2113273856;
	// lis r10,-32096
	ctx.r10.s64 = -2103443456;
	// addi r11,r11,-28964
	ctx.r11.s64 = ctx.r11.s64 + -28964;
	// addi r3,r10,9048
	ctx.r3.s64 = ctx.r10.s64 + 9048;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bl 0x8298ed98
	ctx.lr = 0x829FCCC0;
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

PPC_FUNC_IMPL(__imp__sub_829FCCD8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCCD8) { __imp__sub_829FCCD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCCD8) {
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
	// li r31,3
	ctx.r31.s64 = 3;
	// addi r30,r11,4248
	ctx.r30.s64 = ctx.r11.s64 + 4248;
loc_829FCCF8:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82973d08
	ctx.lr = 0x829FCD00;
	sub_82973D08(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,224
	ctx.r30.s64 = ctx.r30.s64 + 224;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// bge cr6,0x829fccf8
	if (!ctx.cr6.lt) goto loc_829FCCF8;
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,7040
	ctx.r3.s64 = ctx.r11.s64 + 7040;
	// bl 0x8298ed98
	ctx.lr = 0x829FCD1C;
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

PPC_FUNC_IMPL(__imp__sub_829FCD38);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCD38) { __imp__sub_829FCD38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCD38) {
	PPC_FUNC_PROLOGUE();
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
	// addi r11,r11,-32328
	ctx.r11.s64 = ctx.r11.s64 + -32328;
	// addi r3,r11,4
	ctx.r3.s64 = ctx.r11.s64 + 4;
	// bl 0x82a0269c
	ctx.lr = 0x829FCD54;
	__imp__RtlInitializeCriticalSection(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,9128
	ctx.r3.s64 = ctx.r11.s64 + 9128;
	// bl 0x8298ed98
	ctx.lr = 0x829FCD60;
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

PPC_FUNC_IMPL(__imp__sub_829FCD70);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCD70) { __imp__sub_829FCD70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCD70) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// li r5,44
	ctx.r5.s64 = 44;
	// addi r3,r11,7928
	ctx.r3.s64 = ctx.r11.s64 + 7928;
	// li r4,0
	ctx.r4.s64 = 0;
	// b 0x8298eb90
	sub_8298EB90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FCD88);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCD88) { __imp__sub_829FCD88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCD88) {
	PPC_FUNC_PROLOGUE();
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
	// addi r3,r11,29988
	ctx.r3.s64 = ctx.r11.s64 + 29988;
	// bl 0x827f1320
	ctx.lr = 0x829FCDA0;
	sub_827F1320(ctx, base);
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,9152
	ctx.r3.s64 = ctx.r11.s64 + 9152;
	// bl 0x8298ed98
	ctx.lr = 0x829FCDAC;
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

PPC_FUNC_IMPL(__imp__sub_829FCDC0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCDC0) { __imp__sub_829FCDC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCDC0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32096
	ctx.r11.s64 = -2103443456;
	// addi r3,r11,9168
	ctx.r3.s64 = ctx.r11.s64 + 9168;
	// b 0x8298ed98
	sub_8298ED98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FCDD0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCDD0) { __imp__sub_829FCDD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCDD0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// addi r3,r11,-28240
	ctx.r3.s64 = ctx.r11.s64 + -28240;
	// b 0x821eb658
	sub_821EB658(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FCDE0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCDE0) { __imp__sub_829FCDE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCDE0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// addi r3,r11,-28236
	ctx.r3.s64 = ctx.r11.s64 + -28236;
	// b 0x821eb658
	sub_821EB658(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FCDF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCDF0) { __imp__sub_829FCDF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCDF0) {
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
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// li r30,26
	ctx.r30.s64 = 26;
	// addi r11,r11,-28232
	ctx.r11.s64 = ctx.r11.s64 + -28232;
	// addi r31,r11,108
	ctx.r31.s64 = ctx.r11.s64 + 108;
loc_829FCE14:
	// addi r31,r31,-4
	ctx.r31.s64 = ctx.r31.s64 + -4;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x821eb658
	ctx.lr = 0x829FCE20;
	sub_821EB658(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829fce14
	if (!ctx.cr6.lt) goto loc_829FCE14;
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

PPC_FUNC_IMPL(__imp__sub_829FCE48);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCE48) { __imp__sub_829FCE48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCE48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// addi r3,r11,-28124
	ctx.r3.s64 = ctx.r11.s64 + -28124;
	// b 0x823d6f68
	sub_823D6F68(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FCE58);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCE58) { __imp__sub_829FCE58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCE58) {
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
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// li r30,5
	ctx.r30.s64 = 5;
	// addi r11,r11,-28084
	ctx.r11.s64 = ctx.r11.s64 + -28084;
	// addi r31,r11,24
	ctx.r31.s64 = ctx.r11.s64 + 24;
loc_829FCE7C:
	// addi r31,r31,-4
	ctx.r31.s64 = ctx.r31.s64 + -4;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x821eb658
	ctx.lr = 0x829FCE88;
	sub_821EB658(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829fce7c
	if (!ctx.cr6.lt) goto loc_829FCE7C;
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

PPC_FUNC_IMPL(__imp__sub_829FCEB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCEB0) { __imp__sub_829FCEB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCEB0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// addi r11,r11,-27952
	ctx.r11.s64 = ctx.r11.s64 + -27952;
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

PPC_FUNC_IMPL(__imp__sub_829FCECC);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCECC) { __imp__sub_829FCECC(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCECC) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FCED0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCED0) { __imp__sub_829FCED0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCED0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// addi r11,r11,-27944
	ctx.r11.s64 = ctx.r11.s64 + -27944;
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

PPC_FUNC_IMPL(__imp__sub_829FCEEC);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCEEC) { __imp__sub_829FCEEC(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCEEC) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FCEF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCEF0) { __imp__sub_829FCEF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCEF0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// addi r11,r11,-27936
	ctx.r11.s64 = ctx.r11.s64 + -27936;
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

PPC_FUNC_IMPL(__imp__sub_829FCF0C);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCF0C) { __imp__sub_829FCF0C(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCF0C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FCF10);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCF10) { __imp__sub_829FCF10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCF10) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// addi r11,r11,-27928
	ctx.r11.s64 = ctx.r11.s64 + -27928;
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

PPC_FUNC_IMPL(__imp__sub_829FCF2C);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCF2C) { __imp__sub_829FCF2C(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCF2C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FCF30);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCF30) { __imp__sub_829FCF30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCF30) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// addi r11,r11,-27920
	ctx.r11.s64 = ctx.r11.s64 + -27920;
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

PPC_FUNC_IMPL(__imp__sub_829FCF4C);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCF4C) { __imp__sub_829FCF4C(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCF4C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FCF50);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCF50) { __imp__sub_829FCF50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCF50) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// addi r3,r11,-27896
	ctx.r3.s64 = ctx.r11.s64 + -27896;
	// b 0x8222c048
	sub_8222C048(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FCF60);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCF60) { __imp__sub_829FCF60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCF60) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// addi r3,r11,-27884
	ctx.r3.s64 = ctx.r11.s64 + -27884;
	// b 0x8222c048
	sub_8222C048(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FCF70);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCF70) { __imp__sub_829FCF70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCF70) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x829FCF78;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// addi r11,r11,-27872
	ctx.r11.s64 = ctx.r11.s64 + -27872;
	// lhz r31,6(r11)
	ctx.r31.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x829fcfc0
	if (ctx.cr6.eq) goto loc_829FCFC0;
	// lwz r29,0(r11)
	ctx.r29.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// ble cr6,0x829fcfb8
	if (!ctx.cr6.gt) goto loc_829FCFB8;
	// mr r30,r29
	ctx.r30.u64 = ctx.r29.u64;
loc_829FCFA0:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x821eb658
	ctx.lr = 0x829FCFA8;
	sub_821EB658(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,4
	ctx.r30.s64 = ctx.r30.s64 + 4;
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// bne cr6,0x829fcfa0
	if (!ctx.cr6.eq) goto loc_829FCFA0;
loc_829FCFB8:
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x8218be78
	ctx.lr = 0x829FCFC0;
	sub_8218BE78(ctx, base);
loc_829FCFC0:
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FCFC8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FCFC8) { __imp__sub_829FCFC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FCFC8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x829FCFD0;
	__savegprlr_28(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// li r30,479
	ctx.r30.s64 = 479;
	// addi r11,r11,-27856
	ctx.r11.s64 = ctx.r11.s64 + -27856;
	// li r28,-1
	ctx.r28.s64 = -1;
	// addis r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 65536;
	// li r29,0
	ctx.r29.s64 = 0;
	// addi r31,r11,-27136
	ctx.r31.s64 = ctx.r11.s64 + -27136;
loc_829FCFF0:
	// addi r31,r31,-80
	ctx.r31.s64 = ctx.r31.s64 + -80;
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x829fd030
	if (ctx.cr6.eq) goto loc_829FD030;
	// lwz r10,532(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 532);
	// lwz r10,284(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 284);
	// cmpwi cr6,r10,2
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 2, ctx.xer);
	// bne cr6,0x829fd018
	if (!ctx.cr6.eq) goto loc_829FD018;
	// lwz r11,548(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 548);
	// stw r28,84(r11)
	PPC_STORE_U32(ctx.r11.u32 + 84, ctx.r28.u32);
loc_829FD018:
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x829fd02c
	if (ctx.cr6.eq) goto loc_829FD02C;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x823e56d0
	ctx.lr = 0x829FD02C;
	sub_823E56D0(ctx, base);
loc_829FD02C:
	// stw r29,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r29.u32);
loc_829FD030:
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// sth r29,64(r31)
	PPC_STORE_U16(ctx.r31.u32 + 64, ctx.r29.u16);
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829fcff0
	if (!ctx.cr6.lt) goto loc_829FCFF0;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD048);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD048) { __imp__sub_829FD048(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD048) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r31,r11,31952
	ctx.r31.s64 = ctx.r11.s64 + 31952;
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x829fd078
	if (ctx.cr6.eq) goto loc_829FD078;
	// bl 0x822a1bd0
	ctx.lr = 0x829FD070;
	sub_822A1BD0(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r11.u32);
loc_829FD078:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD090);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD090) { __imp__sub_829FD090(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD090) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// addi r11,r11,10544
	ctx.r11.s64 = ctx.r11.s64 + 10544;
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

PPC_FUNC_IMPL(__imp__sub_829FD0AC);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD0AC) { __imp__sub_829FD0AC(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD0AC) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD0B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD0B0) { __imp__sub_829FD0B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD0B0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// addi r11,r11,10552
	ctx.r11.s64 = ctx.r11.s64 + 10552;
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

PPC_FUNC_IMPL(__imp__sub_829FD0CC);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD0CC) { __imp__sub_829FD0CC(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD0CC) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD0D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD0D0) { __imp__sub_829FD0D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD0D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// addi r11,r11,10576
	ctx.r11.s64 = ctx.r11.s64 + 10576;
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

PPC_FUNC_IMPL(__imp__sub_829FD0EC);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD0EC) { __imp__sub_829FD0EC(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD0EC) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD0F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD0F0) { __imp__sub_829FD0F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD0F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lis r10,-31980
	ctx.r10.s64 = -2095841280;
	// addi r11,r11,4388
	ctx.r11.s64 = ctx.r11.s64 + 4388;
	// stw r11,32004(r10)
	PPC_STORE_U32(ctx.r10.u32 + 32004, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD108);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD108) { __imp__sub_829FD108(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD108) {
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
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// li r30,34
	ctx.r30.s64 = 34;
	// addi r11,r11,10584
	ctx.r11.s64 = ctx.r11.s64 + 10584;
	// addi r31,r11,424
	ctx.r31.s64 = ctx.r11.s64 + 424;
loc_829FD12C:
	// addi r31,r31,-12
	ctx.r31.s64 = ctx.r31.s64 + -12;
	// lhz r11,6(r31)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r31.u32 + 6);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x829fd144
	if (ctx.cr6.eq) goto loc_829FD144;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x8218be78
	ctx.lr = 0x829FD144;
	sub_8218BE78(ctx, base);
loc_829FD144:
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829fd12c
	if (!ctx.cr6.lt) goto loc_829FD12C;
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

PPC_FUNC_IMPL(__imp__sub_829FD168);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD168) { __imp__sub_829FD168(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD168) {
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
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// li r30,54
	ctx.r30.s64 = 54;
	// addi r11,r11,11008
	ctx.r11.s64 = ctx.r11.s64 + 11008;
	// addi r31,r11,1336
	ctx.r31.s64 = ctx.r11.s64 + 1336;
loc_829FD18C:
	// addi r31,r31,-24
	ctx.r31.s64 = ctx.r31.s64 + -24;
	// lhz r11,6(r31)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r31.u32 + 6);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x829fd1a4
	if (ctx.cr6.eq) goto loc_829FD1A4;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x8218be78
	ctx.lr = 0x829FD1A4;
	sub_8218BE78(ctx, base);
loc_829FD1A4:
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829fd18c
	if (!ctx.cr6.lt) goto loc_829FD18C;
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

PPC_FUNC_IMPL(__imp__sub_829FD1C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD1C8) { __imp__sub_829FD1C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD1C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// addi r3,r11,12328
	ctx.r3.s64 = ctx.r11.s64 + 12328;
	// b 0x826a0688
	sub_826A0688(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD1D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD1D8) { __imp__sub_829FD1D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD1D8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD1E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD1E0) { __imp__sub_829FD1E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD1E0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD1E8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD1E8) { __imp__sub_829FD1E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD1E8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD1F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD1F0) { __imp__sub_829FD1F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD1F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// addi r3,r11,29044
	ctx.r3.s64 = ctx.r11.s64 + 29044;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD200);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD200) { __imp__sub_829FD200(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD200) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// addi r3,r11,29808
	ctx.r3.s64 = ctx.r11.s64 + 29808;
	// b 0x8214b280
	sub_8214B280(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD210);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD210) { __imp__sub_829FD210(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD210) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// addi r3,r11,31968
	ctx.r3.s64 = ctx.r11.s64 + 31968;
	// b 0x8296ca60
	sub_8296CA60(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD220);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD220) { __imp__sub_829FD220(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD220) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x829FD228;
	__savegprlr_27(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// li r27,0
	ctx.r27.s64 = 0;
	// addi r29,r11,29840
	ctx.r29.s64 = ctx.r11.s64 + 29840;
	// lhz r11,4(r29)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r29.u32 + 4);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x829fd290
	if (ctx.cr6.eq) goto loc_829FD290;
	// li r28,0
	ctx.r28.s64 = 0;
loc_829FD248:
	// lwz r10,0(r29)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r29.u32 + 0);
	// lwzx r31,r28,r10
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r28.u32 + ctx.r10.u32);
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x829fd27c
	if (ctx.cr6.eq) goto loc_829FD27C;
loc_829FD258:
	// mr r30,r31
	ctx.r30.u64 = ctx.r31.u64;
	// lwz r31,12(r31)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r31.u32 + 12);
	// lwz r3,0(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// bl 0x8218be78
	ctx.lr = 0x829FD268;
	sub_8218BE78(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x8218be78
	ctx.lr = 0x829FD270;
	sub_8218BE78(ctx, base);
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// bne cr6,0x829fd258
	if (!ctx.cr6.eq) goto loc_829FD258;
	// lhz r11,4(r29)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r29.u32 + 4);
loc_829FD27C:
	// addi r27,r27,1
	ctx.r27.s64 = ctx.r27.s64 + 1;
	// clrlwi r10,r11,16
	ctx.r10.u64 = ctx.r11.u32 & 0xFFFF;
	// addi r28,r28,4
	ctx.r28.s64 = ctx.r28.s64 + 4;
	// cmplw cr6,r27,r10
	ctx.cr6.compare<uint32_t>(ctx.r27.u32, ctx.r10.u32, ctx.xer);
	// blt cr6,0x829fd248
	if (ctx.cr6.lt) goto loc_829FD248;
loc_829FD290:
	// lwz r3,0(r29)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r29.u32 + 0);
	// bl 0x8218be78
	ctx.lr = 0x829FD298;
	sub_8218BE78(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,0(r29)
	PPC_STORE_U32(ctx.r29.u32 + 0, ctx.r11.u32);
	// sth r11,6(r29)
	PPC_STORE_U16(ctx.r29.u32 + 6, ctx.r11.u16);
	// sth r11,4(r29)
	PPC_STORE_U16(ctx.r29.u32 + 4, ctx.r11.u16);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD2B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD2B0) { __imp__sub_829FD2B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD2B0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r31,r11,1712
	ctx.r31.s64 = ctx.r11.s64 + 1712;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// ble cr6,0x829fd308
	if (!ctx.cr6.gt) goto loc_829FD308;
	// lbz r11,24(r31)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r31.u32 + 24);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x829fd2f0
	if (ctx.cr6.eq) goto loc_829FD2F0;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x8218be78
	ctx.lr = 0x829FD2E8;
	sub_8218BE78(ctx, base);
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4);
	// bl 0x8218be78
	ctx.lr = 0x829FD2F0;
	sub_8218BE78(ctx, base);
loc_829FD2F0:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// stw r11,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r11.u32);
	// stw r11,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r11.u32);
	// stw r11,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r11.u32);
	// stw r11,20(r31)
	PPC_STORE_U32(ctx.r31.u32 + 20, ctx.r11.u32);
loc_829FD308:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD320);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD320) { __imp__sub_829FD320(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD320) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x829FD328;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// addi r11,r11,31996
	ctx.r11.s64 = ctx.r11.s64 + 31996;
	// lhz r31,6(r11)
	ctx.r31.u64 = PPC_LOAD_U16(ctx.r11.u32 + 6);
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x829fd370
	if (ctx.cr6.eq) goto loc_829FD370;
	// lwz r29,0(r11)
	ctx.r29.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// ble cr6,0x829fd368
	if (!ctx.cr6.gt) goto loc_829FD368;
	// mr r30,r29
	ctx.r30.u64 = ctx.r29.u64;
loc_829FD350:
	// lwz r3,0(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// bl 0x8218be78
	ctx.lr = 0x829FD358;
	sub_8218BE78(ctx, base);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r30,r30,8
	ctx.r30.s64 = ctx.r30.s64 + 8;
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// bne cr6,0x829fd350
	if (!ctx.cr6.eq) goto loc_829FD350;
loc_829FD368:
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x8218be78
	ctx.lr = 0x829FD370;
	sub_8218BE78(ctx, base);
loc_829FD370:
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD378);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD378) { __imp__sub_829FD378(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD378) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32086
	ctx.r10.s64 = -2102788096;
	// addi r11,r11,-8564
	ctx.r11.s64 = ctx.r11.s64 + -8564;
	// stw r11,1740(r10)
	PPC_STORE_U32(ctx.r10.u32 + 1740, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD390);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD390) { __imp__sub_829FD390(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD390) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lis r10,-32086
	ctx.r10.s64 = -2102788096;
	// addi r11,r11,-8564
	ctx.r11.s64 = ctx.r11.s64 + -8564;
	// stw r11,1772(r10)
	PPC_STORE_U32(ctx.r10.u32 + 1772, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD3A8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD3A8) { __imp__sub_829FD3A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD3A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r3,r11,6272
	ctx.r3.s64 = ctx.r11.s64 + 6272;
	// b 0x827d8ac8
	sub_827D8AC8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD3B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD3B8) { __imp__sub_829FD3B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD3B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r3,r11,6312
	ctx.r3.s64 = ctx.r11.s64 + 6312;
	// b 0x827d9170
	sub_827D9170(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD3C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD3C8) { __imp__sub_829FD3C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD3C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r3,r11,6508
	ctx.r3.s64 = ctx.r11.s64 + 6508;
	// b 0x827d9bb0
	sub_827D9BB0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD3D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD3D8) { __imp__sub_829FD3D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD3D8) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,0(r13)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r13.u32 + 0);
	// li r9,1676
	ctx.r9.s64 = 1676;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r4,r11,2376
	ctx.r4.s64 = ctx.r11.s64 + 2376;
	// lwzx r3,r9,r10
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r9.u32 + ctx.r10.u32);
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,48(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 48);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctr 
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD418);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD418) { __imp__sub_829FD418(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD418) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r31,r11,8856
	ctx.r31.s64 = ctx.r11.s64 + 8856;
	// addi r3,r31,60
	ctx.r3.s64 = ctx.r31.s64 + 60;
	// bl 0x822de5f0
	ctx.lr = 0x829FD438;
	sub_822DE5F0(ctx, base);
	// addi r3,r31,48
	ctx.r3.s64 = ctx.r31.s64 + 48;
	// bl 0x822de5f0
	ctx.lr = 0x829FD440;
	sub_822DE5F0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD458);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD458) { __imp__sub_829FD458(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD458) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r3,r11,11136
	ctx.r3.s64 = ctx.r11.s64 + 11136;
	// b 0x827f0128
	sub_827F0128(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD468);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD468) { __imp__sub_829FD468(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD468) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r3,r11,12280
	ctx.r3.s64 = ctx.r11.s64 + 12280;
	// b 0x827f0128
	sub_827F0128(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD478);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD478) { __imp__sub_829FD478(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD478) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r3,r11,13424
	ctx.r3.s64 = ctx.r11.s64 + 13424;
	// b 0x827f0128
	sub_827F0128(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD488);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD488) { __imp__sub_829FD488(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD488) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lis r10,-32094
	ctx.r10.s64 = -2103312384;
	// addi r11,r11,4388
	ctx.r11.s64 = ctx.r11.s64 + 4388;
	// stw r11,7608(r10)
	PPC_STORE_U32(ctx.r10.u32 + 7608, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD4A0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD4A0) { __imp__sub_829FD4A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD4A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lis r10,-32094
	ctx.r10.s64 = -2103312384;
	// addi r11,r11,4388
	ctx.r11.s64 = ctx.r11.s64 + 4388;
	// stw r11,7880(r10)
	PPC_STORE_U32(ctx.r10.u32 + 7880, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD4B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD4B8) { __imp__sub_829FD4B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD4B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lis r10,-32094
	ctx.r10.s64 = -2103312384;
	// addi r11,r11,4388
	ctx.r11.s64 = ctx.r11.s64 + 4388;
	// stw r11,8152(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8152, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD4D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD4D0) { __imp__sub_829FD4D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD4D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lis r10,-32094
	ctx.r10.s64 = -2103312384;
	// addi r11,r11,4388
	ctx.r11.s64 = ctx.r11.s64 + 4388;
	// stw r11,8424(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8424, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD4E8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD4E8) { __imp__sub_829FD4E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD4E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lis r10,-32094
	ctx.r10.s64 = -2103312384;
	// addi r11,r11,4388
	ctx.r11.s64 = ctx.r11.s64 + 4388;
	// stw r11,8696(r10)
	PPC_STORE_U32(ctx.r10.u32 + 8696, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD500);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD500) { __imp__sub_829FD500(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD500) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r3,r11,23928
	ctx.r3.s64 = ctx.r11.s64 + 23928;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD510);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD510) { __imp__sub_829FD510(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD510) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r31,r11,15832
	ctx.r31.s64 = ctx.r11.s64 + 15832;
	// lwz r3,4012(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4012);
	// bl 0x827dadb0
	ctx.lr = 0x829FD530;
	sub_827DADB0(ctx, base);
	// lwz r3,4016(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4016);
	// bl 0x827dadb0
	ctx.lr = 0x829FD538;
	sub_827DADB0(ctx, base);
	// lwz r3,4020(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4020);
	// bl 0x827dadb0
	ctx.lr = 0x829FD540;
	sub_827DADB0(ctx, base);
	// lwz r3,4024(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4024);
	// bl 0x827dadb0
	ctx.lr = 0x829FD548;
	sub_827DADB0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD560);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD560) { __imp__sub_829FD560(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD560) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r31,r11,23984
	ctx.r31.s64 = ctx.r11.s64 + 23984;
	// lwz r3,21732(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 21732);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x829fd594
	if (ctx.cr6.eq) goto loc_829FD594;
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x829FD594;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
loc_829FD594:
	// lhz r11,7198(r31)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r31.u32 + 7198);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x829fd5a8
	if (ctx.cr6.eq) goto loc_829FD5A8;
	// lwz r3,7192(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 7192);
	// bl 0x8218be78
	ctx.lr = 0x829FD5A8;
	sub_8218BE78(ctx, base);
loc_829FD5A8:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD5C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD5C0) { __imp__sub_829FD5C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD5C0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD5C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD5C8) { __imp__sub_829FD5C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD5C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// addi r3,r11,-19744
	ctx.r3.s64 = ctx.r11.s64 + -19744;
	// b 0x82197298
	sub_82197298(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD5D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD5D8) { __imp__sub_829FD5D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD5D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32094
	ctx.r11.s64 = -2103312384;
	// addi r11,r11,11512
	ctx.r11.s64 = ctx.r11.s64 + 11512;
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

PPC_FUNC_IMPL(__imp__sub_829FD5F4);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD5F4) { __imp__sub_829FD5F4(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD5F4) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD5F8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD5F8) { __imp__sub_829FD5F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD5F8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x829FD600;
	__savegprlr_27(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32083
	ctx.r11.s64 = -2102591488;
	// li r27,15
	ctx.r27.s64 = 15;
	// addi r11,r11,26456
	ctx.r11.s64 = ctx.r11.s64 + 26456;
	// li r28,0
	ctx.r28.s64 = 0;
	// addi r29,r11,512
	ctx.r29.s64 = ctx.r11.s64 + 512;
loc_829FD618:
	// addi r29,r29,-32
	ctx.r29.s64 = ctx.r29.s64 + -32;
	// li r30,8
	ctx.r30.s64 = 8;
	// mr r31,r29
	ctx.r31.u64 = ctx.r29.u64;
loc_829FD624:
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x829fd63c
	if (ctx.cr6.eq) goto loc_829FD63C;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x823e56d0
	ctx.lr = 0x829FD638;
	sub_823E56D0(ctx, base);
	// stw r28,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r28.u32);
loc_829FD63C:
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// addi r31,r31,4
	ctx.r31.s64 = ctx.r31.s64 + 4;
	// cmplwi cr6,r30,0
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, 0, ctx.xer);
	// bne cr6,0x829fd624
	if (!ctx.cr6.eq) goto loc_829FD624;
	// addi r27,r27,-1
	ctx.r27.s64 = ctx.r27.s64 + -1;
	// cmpwi cr6,r27,0
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 0, ctx.xer);
	// bge cr6,0x829fd618
	if (!ctx.cr6.lt) goto loc_829FD618;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD660);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD660) { __imp__sub_829FD660(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD660) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD668);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD668) { __imp__sub_829FD668(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD668) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x829FD670;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32081
	ctx.r11.s64 = -2102460416;
	// li r30,39
	ctx.r30.s64 = 39;
	// addi r11,r11,-15856
	ctx.r11.s64 = ctx.r11.s64 + -15856;
	// li r29,0
	ctx.r29.s64 = 0;
	// addi r31,r11,3928
	ctx.r31.s64 = ctx.r11.s64 + 3928;
loc_829FD688:
	// addi r31,r31,-96
	ctx.r31.s64 = ctx.r31.s64 + -96;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x8218be78
	ctx.lr = 0x829FD694;
	sub_8218BE78(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// stw r29,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r29.u32);
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829fd688
	if (!ctx.cr6.lt) goto loc_829FD688;
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD6B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD6B0) { __imp__sub_829FD6B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD6B0) {
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
	// li r30,9
	ctx.r30.s64 = 9;
	// addi r11,r11,4544
	ctx.r11.s64 = ctx.r11.s64 + 4544;
	// addi r31,r11,960
	ctx.r31.s64 = ctx.r11.s64 + 960;
loc_829FD6D4:
	// addi r31,r31,-96
	ctx.r31.s64 = ctx.r31.s64 + -96;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x821ab1c0
	ctx.lr = 0x829FD6E0;
	sub_821AB1C0(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829fd6d4
	if (!ctx.cr6.lt) goto loc_829FD6D4;
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

PPC_FUNC_IMPL(__imp__sub_829FD708);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD708) { __imp__sub_829FD708(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD708) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32074
	ctx.r11.s64 = -2102001664;
	// addi r3,r11,-17616
	ctx.r3.s64 = ctx.r11.s64 + -17616;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD718);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD718) { __imp__sub_829FD718(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD718) {
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
	// li r30,40
	ctx.r30.s64 = 40;
	// addi r11,r11,-6912
	ctx.r11.s64 = ctx.r11.s64 + -6912;
	// addi r31,r11,164
	ctx.r31.s64 = ctx.r11.s64 + 164;
loc_829FD73C:
	// addi r31,r31,-4
	ctx.r31.s64 = ctx.r31.s64 + -4;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x821eb658
	ctx.lr = 0x829FD748;
	sub_821EB658(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829fd73c
	if (!ctx.cr6.lt) goto loc_829FD73C;
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

PPC_FUNC_IMPL(__imp__sub_829FD770);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD770) { __imp__sub_829FD770(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD770) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD778);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD778) { __imp__sub_829FD778(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD778) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r3,r11,-11548
	ctx.r3.s64 = ctx.r11.s64 + -11548;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD788);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD788) { __imp__sub_829FD788(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD788) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r11,r11,-11376
	ctx.r11.s64 = ctx.r11.s64 + -11376;
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

PPC_FUNC_IMPL(__imp__sub_829FD7A4);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD7A4) { __imp__sub_829FD7A4(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD7A4) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD7A8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD7A8) { __imp__sub_829FD7A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD7A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r11,r11,-11360
	ctx.r11.s64 = ctx.r11.s64 + -11360;
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

PPC_FUNC_IMPL(__imp__sub_829FD7C4);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD7C4) { __imp__sub_829FD7C4(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD7C4) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD7C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD7C8) { __imp__sub_829FD7C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD7C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r11,r11,-11328
	ctx.r11.s64 = ctx.r11.s64 + -11328;
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

PPC_FUNC_IMPL(__imp__sub_829FD7E4);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD7E4) { __imp__sub_829FD7E4(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD7E4) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD7E8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD7E8) { __imp__sub_829FD7E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD7E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r3,r11,-11368
	ctx.r3.s64 = ctx.r11.s64 + -11368;
	// b 0x8220a4d8
	sub_8220A4D8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD7F8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD7F8) { __imp__sub_829FD7F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD7F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r3,r11,-11352
	ctx.r3.s64 = ctx.r11.s64 + -11352;
	// b 0x8220b068
	sub_8220B068(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD808);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD808) { __imp__sub_829FD808(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD808) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r3,r11,-11340
	ctx.r3.s64 = ctx.r11.s64 + -11340;
	// b 0x8220ded0
	sub_8220DED0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD818);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD818) { __imp__sub_829FD818(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD818) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD820);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD820) { __imp__sub_829FD820(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD820) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD828);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD828) { __imp__sub_829FD828(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD828) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD830);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD830) { __imp__sub_829FD830(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD830) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD838);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD838) { __imp__sub_829FD838(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD838) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD840);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD840) { __imp__sub_829FD840(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD840) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD848);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD848) { __imp__sub_829FD848(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD848) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD850);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD850) { __imp__sub_829FD850(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD850) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r11,r11,-5284
	ctx.r11.s64 = ctx.r11.s64 + -5284;
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

PPC_FUNC_IMPL(__imp__sub_829FD86C);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD86C) { __imp__sub_829FD86C(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD86C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD870);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD870) { __imp__sub_829FD870(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD870) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r3,r11,-5216
	ctx.r3.s64 = ctx.r11.s64 + -5216;
	// b 0x82517c60
	sub_82517C60(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD880);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD880) { __imp__sub_829FD880(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD880) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r3,r11,-1808
	ctx.r3.s64 = ctx.r11.s64 + -1808;
	// b 0x828a1610
	sub_828A1610(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD890);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD890) { __imp__sub_829FD890(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD890) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r3,r11,10272
	ctx.r3.s64 = ctx.r11.s64 + 10272;
	// b 0x8252d1c0
	sub_8252D1C0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD8A0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD8A0) { __imp__sub_829FD8A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD8A0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD8A8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD8A8) { __imp__sub_829FD8A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD8A8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD8B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD8B0) { __imp__sub_829FD8B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD8B0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD8B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD8B8) { __imp__sub_829FD8B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD8B8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD8C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD8C0) { __imp__sub_829FD8C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD8C0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD8C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD8C8) { __imp__sub_829FD8C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD8C8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD8D0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD8D0) { __imp__sub_829FD8D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD8D0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD8D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD8D8) { __imp__sub_829FD8D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD8D8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD8E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD8E0) { __imp__sub_829FD8E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD8E0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD8E8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD8E8) { __imp__sub_829FD8E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD8E8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD8F0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD8F0) { __imp__sub_829FD8F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD8F0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD8F8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD8F8) { __imp__sub_829FD8F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD8F8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// lis r10,-32093
	ctx.r10.s64 = -2103246848;
	// addi r11,r11,-24960
	ctx.r11.s64 = ctx.r11.s64 + -24960;
	// stw r11,-17192(r10)
	PPC_STORE_U32(ctx.r10.u32 + -17192, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD910);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD910) { __imp__sub_829FD910(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD910) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32067
	ctx.r11.s64 = -2101542912;
	// addi r3,r11,5664
	ctx.r3.s64 = ctx.r11.s64 + 5664;
	// b 0x8284ece8
	sub_8284ECE8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD920);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD920) { __imp__sub_829FD920(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD920) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD928);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD928) { __imp__sub_829FD928(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD928) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32066
	ctx.r11.s64 = -2101477376;
	// addi r3,r11,12896
	ctx.r3.s64 = ctx.r11.s64 + 12896;
	// b 0x8222d410
	sub_8222D410(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD938);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD938) { __imp__sub_829FD938(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD938) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32066
	ctx.r11.s64 = -2101477376;
	// addi r3,r11,22264
	ctx.r3.s64 = ctx.r11.s64 + 22264;
	// b 0x82231508
	sub_82231508(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD948);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD948) { __imp__sub_829FD948(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD948) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32066
	ctx.r11.s64 = -2101477376;
	// addi r11,r11,30932
	ctx.r11.s64 = ctx.r11.s64 + 30932;
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

PPC_FUNC_IMPL(__imp__sub_829FD964);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD964) { __imp__sub_829FD964(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD964) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD968);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD968) { __imp__sub_829FD968(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD968) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x829FD970;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// li r30,39
	ctx.r30.s64 = 39;
	// addi r11,r11,-20940
	ctx.r11.s64 = ctx.r11.s64 + -20940;
	// addi r31,r11,4412
	ctx.r31.s64 = ctx.r11.s64 + 4412;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r29,r11,9888
	ctx.r29.s64 = ctx.r11.s64 + 9888;
loc_829FD98C:
	// addi r31,r31,-108
	ctx.r31.s64 = ctx.r31.s64 + -108;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stw r29,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r29.u32);
	// bl 0x82893c88
	ctx.lr = 0x829FD99C;
	sub_82893C88(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829fd98c
	if (!ctx.cr6.lt) goto loc_829FD98C;
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FD9B0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FD9B0) { __imp__sub_829FD9B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FD9B0) {
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
	// li r30,3
	ctx.r30.s64 = 3;
	// addi r11,r11,-13688
	ctx.r11.s64 = ctx.r11.s64 + -13688;
	// addi r31,r11,16
	ctx.r31.s64 = ctx.r11.s64 + 16;
loc_829FD9D4:
	// addi r31,r31,-4
	ctx.r31.s64 = ctx.r31.s64 + -4;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x821eb658
	ctx.lr = 0x829FD9E0;
	sub_821EB658(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829fd9d4
	if (!ctx.cr6.lt) goto loc_829FD9D4;
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

PPC_FUNC_IMPL(__imp__sub_829FDA08);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDA08) { __imp__sub_829FDA08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDA08) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// addi r3,r11,-13672
	ctx.r3.s64 = ctx.r11.s64 + -13672;
	// b 0x821eb658
	sub_821EB658(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDA18);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDA18) { __imp__sub_829FDA18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDA18) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32062
	ctx.r11.s64 = -2101215232;
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// addi r3,r11,-28104
	ctx.r3.s64 = ctx.r11.s64 + -28104;
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r10,r10,9888
	ctx.r10.s64 = ctx.r10.s64 + 9888;
	// addi r11,r11,-19748
	ctx.r11.s64 = ctx.r11.s64 + -19748;
	// stw r11,8(r3)
	PPC_STORE_U32(ctx.r3.u32 + 8, ctx.r11.u32);
	// stw r10,0(r3)
	PPC_STORE_U32(ctx.r3.u32 + 0, ctx.r10.u32);
	// b 0x82893c88
	sub_82893C88(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDA40);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDA40) { __imp__sub_829FDA40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDA40) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDA48);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDA48) { __imp__sub_829FDA48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDA48) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDA50);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDA50) { __imp__sub_829FDA50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDA50) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32062
	ctx.r11.s64 = -2101215232;
	// addi r3,r11,16672
	ctx.r3.s64 = ctx.r11.s64 + 16672;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDA60);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDA60) { __imp__sub_829FDA60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDA60) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDA68);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDA68) { __imp__sub_829FDA68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDA68) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// addi r3,r11,12544
	ctx.r3.s64 = ctx.r11.s64 + 12544;
	// b 0x822733b8
	sub_822733B8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDA78);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDA78) { __imp__sub_829FDA78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDA78) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x829FDA80;
	__savegprlr_28(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// li r29,31
	ctx.r29.s64 = 31;
	// addi r11,r11,14976
	ctx.r11.s64 = ctx.r11.s64 + 14976;
	// li r28,0
	ctx.r28.s64 = 0;
	// addi r31,r11,2600
	ctx.r31.s64 = ctx.r11.s64 + 2600;
loc_829FDA98:
	// addi r31,r31,-80
	ctx.r31.s64 = ctx.r31.s64 + -80;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x829fdab0
	if (ctx.cr6.eq) goto loc_829FDAB0;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x823e56d0
	ctx.lr = 0x829FDAB0;
	sub_823E56D0(ctx, base);
loc_829FDAB0:
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4);
	// addi r30,r31,4
	ctx.r30.s64 = ctx.r31.s64 + 4;
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x829fdac8
	if (ctx.cr6.eq) goto loc_829FDAC8;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// bl 0x823e56d0
	ctx.lr = 0x829FDAC8;
	sub_823E56D0(ctx, base);
loc_829FDAC8:
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
	// bge cr6,0x829fda98
	if (!ctx.cr6.lt) goto loc_829FDA98;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDAE8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDAE8) { __imp__sub_829FDAE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDAE8) {
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
	// li r30,899
	ctx.r30.s64 = 899;
	// addi r11,r11,-11360
	ctx.r11.s64 = ctx.r11.s64 + -11360;
	// addi r31,r11,3600
	ctx.r31.s64 = ctx.r11.s64 + 3600;
loc_829FDB0C:
	// addi r31,r31,-4
	ctx.r31.s64 = ctx.r31.s64 + -4;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x825b5ab8
	ctx.lr = 0x829FDB18;
	sub_825B5AB8(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829fdb0c
	if (!ctx.cr6.lt) goto loc_829FDB0C;
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

PPC_FUNC_IMPL(__imp__sub_829FDB40);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDB40) { __imp__sub_829FDB40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDB40) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32056
	ctx.r11.s64 = -2100822016;
	// addi r3,r11,-7760
	ctx.r3.s64 = ctx.r11.s64 + -7760;
	// b 0x825b5b78
	sub_825B5B78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDB50);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDB50) { __imp__sub_829FDB50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDB50) {
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
	// lis r11,-32059
	ctx.r11.s64 = -2101018624;
	// li r30,14399
	ctx.r30.s64 = 14399;
	// addi r11,r11,7328
	ctx.r11.s64 = ctx.r11.s64 + 7328;
	// addis r11,r11,3
	ctx.r11.s64 = ctx.r11.s64 + 196608;
	// addi r31,r11,-23808
	ctx.r31.s64 = ctx.r11.s64 + -23808;
loc_829FDB78:
	// addi r31,r31,-12
	ctx.r31.s64 = ctx.r31.s64 + -12;
	// addi r3,r31,4
	ctx.r3.s64 = ctx.r31.s64 + 4;
	// bl 0x825b5b78
	ctx.lr = 0x829FDB84;
	sub_825B5B78(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x825b5ab8
	ctx.lr = 0x829FDB8C;
	sub_825B5AB8(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829fdb78
	if (!ctx.cr6.lt) goto loc_829FDB78;
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

PPC_FUNC_IMPL(__imp__sub_829FDBB0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDBB0) { __imp__sub_829FDBB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDBB0) {
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
	// li r30,255
	ctx.r30.s64 = 255;
	// addi r11,r11,-16480
	ctx.r11.s64 = ctx.r11.s64 + -16480;
	// addi r31,r11,5120
	ctx.r31.s64 = ctx.r11.s64 + 5120;
loc_829FDBD4:
	// lis r11,-32216
	ctx.r11.s64 = -2111307776;
	// addi r31,r31,-20
	ctx.r31.s64 = ctx.r31.s64 + -20;
	// li r5,5
	ctx.r5.s64 = 5;
	// addi r6,r11,-6600
	ctx.r6.s64 = ctx.r11.s64 + -6600;
	// li r4,4
	ctx.r4.s64 = 4;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82120268
	ctx.lr = 0x829FDBF0;
	sub_82120268(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829fdbd4
	if (!ctx.cr6.lt) goto loc_829FDBD4;
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

PPC_FUNC_IMPL(__imp__sub_829FDC18);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDC18) { __imp__sub_829FDC18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDC18) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32056
	ctx.r11.s64 = -2100822016;
	// addi r11,r11,-7756
	ctx.r11.s64 = ctx.r11.s64 + -7756;
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

PPC_FUNC_IMPL(__imp__sub_829FDC34);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDC34) { __imp__sub_829FDC34(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDC34) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDC38);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDC38) { __imp__sub_829FDC38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDC38) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32055
	ctx.r11.s64 = -2100756480;
	// addi r11,r11,-24100
	ctx.r11.s64 = ctx.r11.s64 + -24100;
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

PPC_FUNC_IMPL(__imp__sub_829FDC54);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDC54) { __imp__sub_829FDC54(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDC54) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDC58);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDC58) { __imp__sub_829FDC58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDC58) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32055
	ctx.r11.s64 = -2100756480;
	// addi r11,r11,-24092
	ctx.r11.s64 = ctx.r11.s64 + -24092;
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

PPC_FUNC_IMPL(__imp__sub_829FDC74);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDC74) { __imp__sub_829FDC74(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDC74) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDC78);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDC78) { __imp__sub_829FDC78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDC78) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32055
	ctx.r11.s64 = -2100756480;
	// addi r11,r11,-24084
	ctx.r11.s64 = ctx.r11.s64 + -24084;
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

PPC_FUNC_IMPL(__imp__sub_829FDC94);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDC94) { __imp__sub_829FDC94(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDC94) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDC98);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDC98) { __imp__sub_829FDC98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDC98) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32055
	ctx.r11.s64 = -2100756480;
	// addi r11,r11,-24076
	ctx.r11.s64 = ctx.r11.s64 + -24076;
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

PPC_FUNC_IMPL(__imp__sub_829FDCB4);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDCB4) { __imp__sub_829FDCB4(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDCB4) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDCB8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDCB8) { __imp__sub_829FDCB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDCB8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32055
	ctx.r11.s64 = -2100756480;
	// addi r3,r11,-7792
	ctx.r3.s64 = ctx.r11.s64 + -7792;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDCC8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDCC8) { __imp__sub_829FDCC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDCC8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32055
	ctx.r11.s64 = -2100756480;
	// addi r3,r11,-7760
	ctx.r3.s64 = ctx.r11.s64 + -7760;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDCD8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDCD8) { __imp__sub_829FDCD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDCD8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32055
	ctx.r11.s64 = -2100756480;
	// addi r3,r11,-7728
	ctx.r3.s64 = ctx.r11.s64 + -7728;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDCE8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDCE8) { __imp__sub_829FDCE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDCE8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32055
	ctx.r11.s64 = -2100756480;
	// addi r3,r11,-7696
	ctx.r3.s64 = ctx.r11.s64 + -7696;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDCF8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDCF8) { __imp__sub_829FDCF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDCF8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r31,-32055
	ctx.r31.s64 = -2100756480;
	// lwz r3,-7340(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + -7340);
	// bl 0x8218be78
	ctx.lr = 0x829FDD14;
	sub_8218BE78(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,-7340(r31)
	PPC_STORE_U32(ctx.r31.u32 + -7340, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDD30);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDD30) { __imp__sub_829FDD30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDD30) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDD38);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDD38) { __imp__sub_829FDD38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDD38) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDD40);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDD40) { __imp__sub_829FDD40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDD40) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDD48);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDD48) { __imp__sub_829FDD48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDD48) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDD50);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDD50) { __imp__sub_829FDD50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDD50) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDD58);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDD58) { __imp__sub_829FDD58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDD58) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32054
	ctx.r11.s64 = -2100690944;
	// addi r3,r11,-18000
	ctx.r3.s64 = ctx.r11.s64 + -18000;
	// b 0x8263c0a8
	sub_8263C0A8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDD68);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDD68) { __imp__sub_829FDD68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDD68) {
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
	// li r30,15
	ctx.r30.s64 = 15;
	// addi r11,r11,-13632
	ctx.r11.s64 = ctx.r11.s64 + -13632;
	// addi r31,r11,512
	ctx.r31.s64 = ctx.r11.s64 + 512;
loc_829FDD8C:
	// addi r31,r31,-32
	ctx.r31.s64 = ctx.r31.s64 + -32;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x828e0ab8
	ctx.lr = 0x829FDD98;
	sub_828E0AB8(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829fdd8c
	if (!ctx.cr6.lt) goto loc_829FDD8C;
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

PPC_FUNC_IMPL(__imp__sub_829FDDC0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDDC0) { __imp__sub_829FDDC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDDC0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32055
	ctx.r11.s64 = -2100756480;
	// addi r3,r11,-7664
	ctx.r3.s64 = ctx.r11.s64 + -7664;
	// b 0x822a0eb8
	sub_822A0EB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDDD0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDDD0) { __imp__sub_829FDDD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDDD0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32055
	ctx.r11.s64 = -2100756480;
	// addi r11,r11,-4192
	ctx.r11.s64 = ctx.r11.s64 + -4192;
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

PPC_FUNC_IMPL(__imp__sub_829FDDEC);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDDEC) { __imp__sub_829FDDEC(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDDEC) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDDF0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDDF0) { __imp__sub_829FDDF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDDF0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32055
	ctx.r11.s64 = -2100756480;
	// addi r11,r11,-4184
	ctx.r11.s64 = ctx.r11.s64 + -4184;
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

PPC_FUNC_IMPL(__imp__sub_829FDE0C);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDE0C) { __imp__sub_829FDE0C(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDE0C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDE10);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDE10) { __imp__sub_829FDE10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDE10) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32053
	ctx.r11.s64 = -2100625408;
	// addi r11,r11,-11588
	ctx.r11.s64 = ctx.r11.s64 + -11588;
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

PPC_FUNC_IMPL(__imp__sub_829FDE2C);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDE2C) { __imp__sub_829FDE2C(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDE2C) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDE30);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDE30) { __imp__sub_829FDE30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDE30) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x829FDE38;
	__savegprlr_28(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32053
	ctx.r11.s64 = -2100625408;
	// li r29,31
	ctx.r29.s64 = 31;
	// addi r11,r11,-9308
	ctx.r11.s64 = ctx.r11.s64 + -9308;
	// li r30,0
	ctx.r30.s64 = 0;
	// addi r31,r11,516
	ctx.r31.s64 = ctx.r11.s64 + 516;
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r28,r11,-9676
	ctx.r28.s64 = ctx.r11.s64 + -9676;
loc_829FDE58:
	// addi r31,r31,-16
	ctx.r31.s64 = ctx.r31.s64 + -16;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r28,-4(r31)
	PPC_STORE_U32(ctx.r31.u32 + -4, ctx.r28.u32);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x829fde78
	if (ctx.cr6.eq) goto loc_829FDE78;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x823e56d0
	ctx.lr = 0x829FDE74;
	sub_823E56D0(ctx, base);
	// stw r30,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r30.u32);
loc_829FDE78:
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x829fde9c
	if (ctx.cr6.eq) goto loc_829FDE9C;
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// li r4,1
	ctx.r4.s64 = 1;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x829FDE98;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// stw r30,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r30.u32);
loc_829FDE9C:
	// addi r29,r29,-1
	ctx.r29.s64 = ctx.r29.s64 + -1;
	// stw r30,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r30.u32);
	// cmpwi cr6,r29,0
	ctx.cr6.compare<int32_t>(ctx.r29.s32, 0, ctx.xer);
	// bge cr6,0x829fde58
	if (!ctx.cr6.lt) goto loc_829FDE58;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDEB8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDEB8) { __imp__sub_829FDEB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDEB8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x829FDEC0;
	__savegprlr_28(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32053
	ctx.r11.s64 = -2100625408;
	// li r29,7
	ctx.r29.s64 = 7;
	// addi r11,r11,-8796
	ctx.r11.s64 = ctx.r11.s64 + -8796;
	// li r30,0
	ctx.r30.s64 = 0;
	// addi r31,r11,164
	ctx.r31.s64 = ctx.r11.s64 + 164;
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r28,r11,-9668
	ctx.r28.s64 = ctx.r11.s64 + -9668;
loc_829FDEE0:
	// addi r31,r31,-20
	ctx.r31.s64 = ctx.r31.s64 + -20;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r28,-4(r31)
	PPC_STORE_U32(ctx.r31.u32 + -4, ctx.r28.u32);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x829fdf00
	if (ctx.cr6.eq) goto loc_829FDF00;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x823e56d0
	ctx.lr = 0x829FDEFC;
	sub_823E56D0(ctx, base);
	// stw r30,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r30.u32);
loc_829FDF00:
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x829fdf24
	if (ctx.cr6.eq) goto loc_829FDF24;
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// li r4,1
	ctx.r4.s64 = 1;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x829FDF20;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// stw r30,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r30.u32);
loc_829FDF24:
	// addi r29,r29,-1
	ctx.r29.s64 = ctx.r29.s64 + -1;
	// stw r30,12(r31)
	PPC_STORE_U32(ctx.r31.u32 + 12, ctx.r30.u32);
	// cmpwi cr6,r29,0
	ctx.cr6.compare<int32_t>(ctx.r29.s32, 0, ctx.xer);
	// bge cr6,0x829fdee0
	if (!ctx.cr6.lt) goto loc_829FDEE0;
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDF40);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDF40) { __imp__sub_829FDF40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDF40) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	ctx.lr = 0x829FDF60;
	sub_822DE5F0(ctx, base);
	// addi r3,r31,16
	ctx.r3.s64 = ctx.r31.s64 + 16;
	// bl 0x822c1ba8
	ctx.lr = 0x829FDF68;
	sub_822C1BA8(ctx, base);
	// addi r3,r31,16
	ctx.r3.s64 = ctx.r31.s64 + 16;
	// bl 0x822c1ba8
	ctx.lr = 0x829FDF70;
	sub_822C1BA8(ctx, base);
	// addi r3,r31,4
	ctx.r3.s64 = ctx.r31.s64 + 4;
	// bl 0x822de5f0
	ctx.lr = 0x829FDF78;
	sub_822DE5F0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDF90);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDF90) { __imp__sub_829FDF90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDF90) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32053
	ctx.r11.s64 = -2100625408;
	// addi r3,r11,24800
	ctx.r3.s64 = ctx.r11.s64 + 24800;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FDFA0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FDFA0) { __imp__sub_829FDFA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FDFA0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r31,r11,-8528
	ctx.r31.s64 = ctx.r11.s64 + -8528;
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,33326
	ctx.r11.u64 = ctx.r11.u64 | 33326;
	// lhzx r11,r31,r11
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r31.u32 + ctx.r11.u32);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x829fdfdc
	if (ctx.cr6.eq) goto loc_829FDFDC;
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,33320
	ctx.r11.u64 = ctx.r11.u64 | 33320;
	// lwzx r3,r31,r11
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + ctx.r11.u32);
	// bl 0x8218be78
	ctx.lr = 0x829FDFDC;
	sub_8218BE78(ctx, base);
loc_829FDFDC:
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,33318
	ctx.r11.u64 = ctx.r11.u64 | 33318;
	// lhzx r11,r31,r11
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r31.u32 + ctx.r11.u32);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x829fe000
	if (ctx.cr6.eq) goto loc_829FE000;
	// lis r11,0
	ctx.r11.s64 = 0;
	// ori r11,r11,33312
	ctx.r11.u64 = ctx.r11.u64 | 33312;
	// lwzx r3,r31,r11
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + ctx.r11.u32);
	// bl 0x8218be78
	ctx.lr = 0x829FE000;
	sub_8218BE78(ctx, base);
loc_829FE000:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE018);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE018) { __imp__sub_829FE018(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE018) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE020);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE020) { __imp__sub_829FE020(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE020) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// addi r3,r11,-8256
	ctx.r3.s64 = ctx.r11.s64 + -8256;
	// b 0x8284ece8
	sub_8284ECE8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE030);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE030) { __imp__sub_829FE030(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE030) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r31,r11,-7504
	ctx.r31.s64 = ctx.r11.s64 + -7504;
	// addi r3,r31,36
	ctx.r3.s64 = ctx.r31.s64 + 36;
	// bl 0x821eb658
	ctx.lr = 0x829FE050;
	sub_821EB658(ctx, base);
	// addi r3,r31,32
	ctx.r3.s64 = ctx.r31.s64 + 32;
	// bl 0x821eb658
	ctx.lr = 0x829FE058;
	sub_821EB658(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE070);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE070) { __imp__sub_829FE070(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE070) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// addi r3,r11,100
	ctx.r3.s64 = ctx.r11.s64 + 100;
	// b 0x822d4c18
	sub_822D4C18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE080);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE080) { __imp__sub_829FE080(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE080) {
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
	// li r30,4
	ctx.r30.s64 = 4;
	// addi r11,r11,4156
	ctx.r11.s64 = ctx.r11.s64 + 4156;
	// addi r31,r11,20
	ctx.r31.s64 = ctx.r11.s64 + 20;
loc_829FE0A4:
	// addi r31,r31,-4
	ctx.r31.s64 = ctx.r31.s64 + -4;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x821eb658
	ctx.lr = 0x829FE0B0;
	sub_821EB658(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829fe0a4
	if (!ctx.cr6.lt) goto loc_829FE0A4;
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

PPC_FUNC_IMPL(__imp__sub_829FE0D8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE0D8) { __imp__sub_829FE0D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE0D8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE0E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE0E0) { __imp__sub_829FE0E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE0E0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE0E8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE0E8) { __imp__sub_829FE0E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE0E8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r31,r11,4360
	ctx.r31.s64 = ctx.r11.s64 + 4360;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x8218be78
	ctx.lr = 0x829FE108;
	sub_8218BE78(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_829FE130);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE130) { __imp__sub_829FE130(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE130) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// addi r3,r11,4220
	ctx.r3.s64 = ctx.r11.s64 + 4220;
	// b 0x822de5f0
	sub_822DE5F0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE140);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE140) { __imp__sub_829FE140(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE140) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE148);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE148) { __imp__sub_829FE148(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE148) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r31,r11,4676
	ctx.r31.s64 = ctx.r11.s64 + 4676;
	// lhz r11,18(r31)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r31.u32 + 18);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x829fe174
	if (ctx.cr6.eq) goto loc_829FE174;
	// lwz r3,12(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 12);
	// bl 0x8218be78
	ctx.lr = 0x829FE174;
	sub_8218BE78(ctx, base);
loc_829FE174:
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x829fe188
	if (ctx.cr6.eq) goto loc_829FE188;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x8218be78
	ctx.lr = 0x829FE188;
	sub_8218BE78(ctx, base);
loc_829FE188:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE1A0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE1A0) { __imp__sub_829FE1A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE1A0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32050
	ctx.r11.s64 = -2100428800;
	// addi r31,r11,-13184
	ctx.r31.s64 = ctx.r11.s64 + -13184;
	// lwz r11,16(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 16);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x829fe1e4
	if (ctx.cr6.eq) goto loc_829FE1E4;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// lwz r4,4(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4);
	// bl 0x822ead68
	ctx.lr = 0x829FE1D0;
	sub_822EAD68(ctx, base);
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
loc_829FE1E4:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE1F8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE1F8) { __imp__sub_829FE1F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE1F8) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE200);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE200) { __imp__sub_829FE200(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE200) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32050
	ctx.r11.s64 = -2100428800;
	// addi r31,r11,28492
	ctx.r31.s64 = ctx.r11.s64 + 28492;
	// lwz r3,8(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// bl 0x8218be78
	ctx.lr = 0x829FE220;
	sub_8218BE78(ctx, base);
	// lhz r11,6(r31)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r31.u32 + 6);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x829fe234
	if (ctx.cr6.eq) goto loc_829FE234;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x8218be78
	ctx.lr = 0x829FE234;
	sub_8218BE78(ctx, base);
loc_829FE234:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE248);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE248) { __imp__sub_829FE248(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE248) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE250);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE250) { __imp__sub_829FE250(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE250) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32048
	ctx.r11.s64 = -2100297728;
	// addi r3,r11,-25088
	ctx.r3.s64 = ctx.r11.s64 + -25088;
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

PPC_FUNC_IMPL(__imp__sub_829FE268);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE268) { __imp__sub_829FE268(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE268) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lis r10,-32093
	ctx.r10.s64 = -2103246848;
	// addi r11,r11,4388
	ctx.r11.s64 = ctx.r11.s64 + 4388;
	// addi r10,r10,-4428
	ctx.r10.s64 = ctx.r10.s64 + -4428;
	// stw r11,72(r10)
	PPC_STORE_U32(ctx.r10.u32 + 72, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE280);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE280) { __imp__sub_829FE280(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE280) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32048
	ctx.r11.s64 = -2100297728;
	// addi r3,r11,19456
	ctx.r3.s64 = ctx.r11.s64 + 19456;
	// b 0x82301870
	sub_82301870(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE290);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE290) { __imp__sub_829FE290(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE290) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE298);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE298) { __imp__sub_829FE298(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE298) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32018
	ctx.r11.s64 = -2098331648;
	// addi r11,r11,20468
	ctx.r11.s64 = ctx.r11.s64 + 20468;
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

PPC_FUNC_IMPL(__imp__sub_829FE2B4);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE2B4) { __imp__sub_829FE2B4(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE2B4) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE2B8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE2B8) { __imp__sub_829FE2B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE2B8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r31,r11,-29456
	ctx.r31.s64 = ctx.r11.s64 + -29456;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// ble cr6,0x829fe310
	if (!ctx.cr6.gt) goto loc_829FE310;
	// lbz r11,24(r31)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r31.u32 + 24);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x829fe2f8
	if (ctx.cr6.eq) goto loc_829FE2F8;
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x8218be78
	ctx.lr = 0x829FE2F0;
	sub_8218BE78(ctx, base);
	// lwz r3,4(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 4);
	// bl 0x8218be78
	ctx.lr = 0x829FE2F8;
	sub_8218BE78(ctx, base);
loc_829FE2F8:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// stw r11,4(r31)
	PPC_STORE_U32(ctx.r31.u32 + 4, ctx.r11.u32);
	// stw r11,8(r31)
	PPC_STORE_U32(ctx.r31.u32 + 8, ctx.r11.u32);
	// stw r11,16(r31)
	PPC_STORE_U32(ctx.r31.u32 + 16, ctx.r11.u32);
	// stw r11,20(r31)
	PPC_STORE_U32(ctx.r31.u32 + 20, ctx.r11.u32);
loc_829FE310:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE328);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE328) { __imp__sub_829FE328(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE328) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// addi r3,r11,-29312
	ctx.r3.s64 = ctx.r11.s64 + -29312;
	// b 0x8285bce8
	sub_8285BCE8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE338);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE338) { __imp__sub_829FE338(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE338) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE340);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE340) { __imp__sub_829FE340(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE340) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE348);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE348) { __imp__sub_829FE348(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE348) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb10
	ctx.lr = 0x829FE350;
	__savegprlr_26(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// li r29,127
	ctx.r29.s64 = 127;
	// addi r11,r11,10304
	ctx.r11.s64 = ctx.r11.s64 + 10304;
	// li r30,0
	ctx.r30.s64 = 0;
	// addi r31,r11,13844
	ctx.r31.s64 = ctx.r11.s64 + 13844;
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r27,r11,16500
	ctx.r27.s64 = ctx.r11.s64 + 16500;
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// addi r26,r11,17532
	ctx.r26.s64 = ctx.r11.s64 + 17532;
loc_829FE378:
	// addi r31,r31,-108
	ctx.r31.s64 = ctx.r31.s64 + -108;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r28,r31,-20
	ctx.r28.s64 = ctx.r31.s64 + -20;
	// stw r26,-20(r31)
	PPC_STORE_U32(ctx.r31.u32 + -20, ctx.r26.u32);
	// bl 0x8235a068
	ctx.lr = 0x829FE38C;
	sub_8235A068(ctx, base);
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
	// bl 0x8235a068
	ctx.lr = 0x829FE3A4;
	sub_8235A068(ctx, base);
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// bl 0x82331480
	ctx.lr = 0x829FE3AC;
	sub_82331480(ctx, base);
	// addi r29,r29,-1
	ctx.r29.s64 = ctx.r29.s64 + -1;
	// cmpwi cr6,r29,0
	ctx.cr6.compare<int32_t>(ctx.r29.s32, 0, ctx.xer);
	// bge cr6,0x829fe378
	if (!ctx.cr6.lt) goto loc_829FE378;
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE3C0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE3C0) { __imp__sub_829FE3C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE3C0) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE3C8);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE3C8) { __imp__sub_829FE3C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE3C8) {
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
	// li r30,5
	ctx.r30.s64 = 5;
	// addi r11,r11,-31648
	ctx.r11.s64 = ctx.r11.s64 + -31648;
	// addi r31,r11,480
	ctx.r31.s64 = ctx.r11.s64 + 480;
loc_829FE3EC:
	// addi r31,r31,-80
	ctx.r31.s64 = ctx.r31.s64 + -80;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x828e0ab8
	ctx.lr = 0x829FE3F8;
	sub_828E0AB8(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829fe3ec
	if (!ctx.cr6.lt) goto loc_829FE3EC;
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

PPC_FUNC_IMPL(__imp__sub_829FE420);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE420) { __imp__sub_829FE420(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE420) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32016
	ctx.r11.s64 = -2098200576;
	// addi r3,r11,-31152
	ctx.r3.s64 = ctx.r11.s64 + -31152;
	// b 0x8239bbd0
	sub_8239BBD0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE430);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE430) { __imp__sub_829FE430(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE430) {
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
	// li r30,31
	ctx.r30.s64 = 31;
	// addi r11,r11,-18528
	ctx.r11.s64 = ctx.r11.s64 + -18528;
	// addi r31,r11,4656
	ctx.r31.s64 = ctx.r11.s64 + 4656;
loc_829FE454:
	// addi r31,r31,-144
	ctx.r31.s64 = ctx.r31.s64 + -144;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x828e0ab8
	ctx.lr = 0x829FE460;
	sub_828E0AB8(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829fe454
	if (!ctx.cr6.lt) goto loc_829FE454;
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

PPC_FUNC_IMPL(__imp__sub_829FE488);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE488) { __imp__sub_829FE488(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE488) {
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
	// li r30,3
	ctx.r30.s64 = 3;
	// addi r11,r11,-13852
	ctx.r11.s64 = ctx.r11.s64 + -13852;
	// addi r31,r11,64
	ctx.r31.s64 = ctx.r11.s64 + 64;
loc_829FE4AC:
	// addi r31,r31,-16
	ctx.r31.s64 = ctx.r31.s64 + -16;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8222c298
	ctx.lr = 0x829FE4B8;
	sub_8222C298(ctx, base);
	// addi r30,r30,-1
	ctx.r30.s64 = ctx.r30.s64 + -1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x829fe4ac
	if (!ctx.cr6.lt) goto loc_829FE4AC;
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

PPC_FUNC_IMPL(__imp__sub_829FE4E0);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE4E0) { __imp__sub_829FE4E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE4E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r3,r11,7372
	ctx.r3.s64 = ctx.r11.s64 + 7372;
	// bl 0x823d5a90
	ctx.lr = 0x829FE4F8;
	sub_823D5A90(ctx, base);
	// bl 0x8274d1c0
	ctx.lr = 0x829FE4FC;
	sub_8274D1C0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE510);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE510) { __imp__sub_829FE510(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE510) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32015
	ctx.r11.s64 = -2098135040;
	// addi r3,r11,-3120
	ctx.r3.s64 = ctx.r11.s64 + -3120;
	// b 0x8272c6c0
	sub_8272C6C0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_829FE520);  /* forward decl */
PPC_WEAK_FUNC(sub_829FE520) { __imp__sub_829FE520(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_829FE520) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32015
	ctx.r11.s64 = -2098135040;
	// addi r3,r11,-3080
	ctx.r3.s64 = ctx.r11.s64 + -3080;
	// b 0x8272a6c0
	sub_8272A6C0(ctx, base);
	return;
}

