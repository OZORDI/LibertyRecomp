#include "gta4_init.h"

PPC_FUNC_IMPL(__imp__sub_8258EC70);  /* forward decl */
PPC_WEAK_FUNC(sub_8258EC70) { __imp__sub_8258EC70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258EC70) {
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
	// li r4,7
	ctx.r4.s64 = 7;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r30,4(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x821d64c0
	ctx.lr = 0x8258EC98;
	sub_821D64C0(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// cmplwi cr6,r31,53
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 53, ctx.xer);
	// bgt cr6,0x8258ecbc
	if (ctx.cr6.gt) goto loc_8258ECBC;
	// bl 0x821d4898
	ctx.lr = 0x8258ECA8;
	sub_821D4898(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x82539ec0
	ctx.lr = 0x8258ECB0;
	sub_82539EC0(ctx, base);
	// lwz r11,2292(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 2292);
	// rlwimi r11,r30,27,2,4
	ctx.r11.u64 = (__builtin_rotateleft32(ctx.r30.u32, 27) & 0x38000000) | (ctx.r11.u64 & 0xFFFFFFFFC7FFFFFF);
	// stw r11,2292(r3)
	PPC_STORE_U32(ctx.r3.u32 + 2292, ctx.r11.u32);
loc_8258ECBC:
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

PPC_FUNC_IMPL(__imp__sub_8258ECD8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258ECD8) { __imp__sub_8258ECD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258ECD8) {
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
	// li r4,7
	ctx.r4.s64 = 7;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r30,4(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x821d64c0
	ctx.lr = 0x8258ED00;
	sub_821D64C0(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// cmplwi cr6,r31,53
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 53, ctx.xer);
	// bgt cr6,0x8258ed24
	if (ctx.cr6.gt) goto loc_8258ED24;
	// bl 0x821d4898
	ctx.lr = 0x8258ED10;
	sub_821D4898(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x82539ec0
	ctx.lr = 0x8258ED18;
	sub_82539EC0(ctx, base);
	// lwz r11,2292(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 2292);
	// rlwimi r11,r30,25,5,6
	ctx.r11.u64 = (__builtin_rotateleft32(ctx.r30.u32, 25) & 0x6000000) | (ctx.r11.u64 & 0xFFFFFFFFF9FFFFFF);
	// stw r11,2292(r3)
	PPC_STORE_U32(ctx.r3.u32 + 2292, ctx.r11.u32);
loc_8258ED24:
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

PPC_FUNC_IMPL(__imp__sub_8258ED40);  /* forward decl */
PPC_WEAK_FUNC(sub_8258ED40) { __imp__sub_8258ED40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258ED40) {
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
	// li r4,7
	ctx.r4.s64 = 7;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r30,4(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x821d64c0
	ctx.lr = 0x8258ED68;
	sub_821D64C0(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// cmplwi cr6,r31,53
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 53, ctx.xer);
	// bgt cr6,0x8258ed8c
	if (ctx.cr6.gt) goto loc_8258ED8C;
	// bl 0x821d4898
	ctx.lr = 0x8258ED78;
	sub_821D4898(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x82539ec0
	ctx.lr = 0x8258ED80;
	sub_82539EC0(ctx, base);
	// lwz r11,2292(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 2292);
	// rlwimi r11,r30,23,7,8
	ctx.r11.u64 = (__builtin_rotateleft32(ctx.r30.u32, 23) & 0x1800000) | (ctx.r11.u64 & 0xFFFFFFFFFE7FFFFF);
	// stw r11,2292(r3)
	PPC_STORE_U32(ctx.r3.u32 + 2292, ctx.r11.u32);
loc_8258ED8C:
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

PPC_FUNC_IMPL(__imp__sub_8258EDA8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258EDA8) { __imp__sub_8258EDA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258EDA8) {
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
	// li r4,7
	ctx.r4.s64 = 7;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r30,4(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x821d64c0
	ctx.lr = 0x8258EDD0;
	sub_821D64C0(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// cmplwi cr6,r31,53
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 53, ctx.xer);
	// bgt cr6,0x8258edf4
	if (ctx.cr6.gt) goto loc_8258EDF4;
	// bl 0x821d4898
	ctx.lr = 0x8258EDE0;
	sub_821D4898(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x82539ec0
	ctx.lr = 0x8258EDE8;
	sub_82539EC0(ctx, base);
	// lwz r11,2292(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 2292);
	// rlwimi r11,r30,19,11,12
	ctx.r11.u64 = (__builtin_rotateleft32(ctx.r30.u32, 19) & 0x180000) | (ctx.r11.u64 & 0xFFFFFFFFFFE7FFFF);
	// stw r11,2292(r3)
	PPC_STORE_U32(ctx.r3.u32 + 2292, ctx.r11.u32);
loc_8258EDF4:
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

PPC_FUNC_IMPL(__imp__sub_8258EE10);  /* forward decl */
PPC_WEAK_FUNC(sub_8258EE10) { __imp__sub_8258EE10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258EE10) {
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
	// li r4,7
	ctx.r4.s64 = 7;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r30,4(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x821d64c0
	ctx.lr = 0x8258EE38;
	sub_821D64C0(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// cmplwi cr6,r31,53
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 53, ctx.xer);
	// bgt cr6,0x8258ee5c
	if (ctx.cr6.gt) goto loc_8258EE5C;
	// bl 0x821d4898
	ctx.lr = 0x8258EE48;
	sub_821D4898(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x82539ec0
	ctx.lr = 0x8258EE50;
	sub_82539EC0(ctx, base);
	// lwz r11,2292(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 2292);
	// rlwimi r11,r30,17,13,14
	ctx.r11.u64 = (__builtin_rotateleft32(ctx.r30.u32, 17) & 0x60000) | (ctx.r11.u64 & 0xFFFFFFFFFFF9FFFF);
	// stw r11,2292(r3)
	PPC_STORE_U32(ctx.r3.u32 + 2292, ctx.r11.u32);
loc_8258EE5C:
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

PPC_FUNC_IMPL(__imp__sub_8258EE78);  /* forward decl */
PPC_WEAK_FUNC(sub_8258EE78) { __imp__sub_8258EE78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258EE78) {
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
	// li r4,7
	ctx.r4.s64 = 7;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r30,4(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x821d64c0
	ctx.lr = 0x8258EEA0;
	sub_821D64C0(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// cmplwi cr6,r31,53
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 53, ctx.xer);
	// bgt cr6,0x8258eec4
	if (ctx.cr6.gt) goto loc_8258EEC4;
	// bl 0x821d4898
	ctx.lr = 0x8258EEB0;
	sub_821D4898(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x82539ec0
	ctx.lr = 0x8258EEB8;
	sub_82539EC0(ctx, base);
	// lwz r11,2292(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 2292);
	// rlwimi r11,r30,13,17,18
	ctx.r11.u64 = (__builtin_rotateleft32(ctx.r30.u32, 13) & 0x6000) | (ctx.r11.u64 & 0xFFFFFFFFFFFF9FFF);
	// stw r11,2292(r3)
	PPC_STORE_U32(ctx.r3.u32 + 2292, ctx.r11.u32);
loc_8258EEC4:
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

PPC_FUNC_IMPL(__imp__sub_8258EEE0);  /* forward decl */
PPC_WEAK_FUNC(sub_8258EEE0) { __imp__sub_8258EEE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258EEE0) {
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
	// li r4,7
	ctx.r4.s64 = 7;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r30,4(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x821d64c0
	ctx.lr = 0x8258EF08;
	sub_821D64C0(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// cmplwi cr6,r31,53
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 53, ctx.xer);
	// bgt cr6,0x8258ef24
	if (ctx.cr6.gt) goto loc_8258EF24;
	// bl 0x821d4898
	ctx.lr = 0x8258EF18;
	sub_821D4898(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x82539ec0
	ctx.lr = 0x8258EF20;
	sub_82539EC0(ctx, base);
	// stb r30,2299(r3)
	PPC_STORE_U8(ctx.r3.u32 + 2299, ctx.r30.u8);
loc_8258EF24:
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

PPC_FUNC_IMPL(__imp__sub_8258EF40);  /* forward decl */
PPC_WEAK_FUNC(sub_8258EF40) { __imp__sub_8258EF40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258EF40) {
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
	// li r4,7
	ctx.r4.s64 = 7;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r30,4(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x821d64c0
	ctx.lr = 0x8258EF68;
	sub_821D64C0(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// cmplwi cr6,r31,53
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 53, ctx.xer);
	// bgt cr6,0x8258ef84
	if (ctx.cr6.gt) goto loc_8258EF84;
	// bl 0x821d4898
	ctx.lr = 0x8258EF78;
	sub_821D4898(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x82539ec0
	ctx.lr = 0x8258EF80;
	sub_82539EC0(ctx, base);
	// stb r30,2298(r3)
	PPC_STORE_U8(ctx.r3.u32 + 2298, ctx.r30.u8);
loc_8258EF84:
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

PPC_FUNC_IMPL(__imp__sub_8258EFA0);  /* forward decl */
PPC_WEAK_FUNC(sub_8258EFA0) { __imp__sub_8258EFA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258EFA0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r7,r3
	ctx.r7.u64 = ctx.r3.u64;
	// lwz r11,8(r7)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x8258d010
	ctx.lr = 0x8258EFBC;
	sub_8258D010(ctx, base);
	// lwz r11,0(r7)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258EFD8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258EFD8) { __imp__sub_8258EFD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258EFD8) {
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
	// lfs f3,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f3.f64 = double(temp.f32);
	// lfs f2,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f2.f64 = double(temp.f32);
	// b 0x8258d4e0
	sub_8258D4E0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258EFF0);  /* forward decl */
PPC_WEAK_FUNC(sub_8258EFF0) { __imp__sub_8258EFF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258EFF0) {
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
	// b 0x8258d5b8
	sub_8258D5B8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F018);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F018) { __imp__sub_8258F018(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F018) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x8258F020;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// li r4,7
	ctx.r4.s64 = 7;
	// li r29,0
	ctx.r29.s64 = 0;
	// lwz r11,8(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x821d64c0
	ctx.lr = 0x8258F03C;
	sub_821D64C0(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// cmplwi cr6,r31,53
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 53, ctx.xer);
	// bgt cr6,0x8258f064
	if (ctx.cr6.gt) goto loc_8258F064;
	// bl 0x821d4898
	ctx.lr = 0x8258F04C;
	sub_821D4898(ctx, base);
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// addi r11,r11,-13852
	ctx.r11.s64 = ctx.r11.s64 + -13852;
	// lbzx r11,r31,r11
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r31.u32 + ctx.r11.u32);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8258f064
	if (ctx.cr6.eq) goto loc_8258F064;
	// li r29,1
	ctx.r29.s64 = 1;
loc_8258F064:
	// lwz r11,0(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// clrlwi r10,r29,24
	ctx.r10.u64 = ctx.r29.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F078);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F078) { __imp__sub_8258F078(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F078) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x8257f3f8
	ctx.lr = 0x8258F098;
	sub_8257F3F8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8258F0B8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F0B8) { __imp__sub_8258F0B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F0B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,13480
	ctx.r3.s64 = ctx.r11.s64 + 13480;
	// b 0x82555cb0
	sub_82555CB0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F0C8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F0C8) { __imp__sub_8258F0C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F0C8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,-7968
	ctx.r3.s64 = ctx.r11.s64 + -7968;
	// b 0x8257cc18
	sub_8257CC18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F0D8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F0D8) { __imp__sub_8258F0D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F0D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,-7672
	ctx.r3.s64 = ctx.r11.s64 + -7672;
	// b 0x8258be98
	sub_8258BE98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F0E8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F0E8) { __imp__sub_8258F0E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F0E8) {
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
	// lfs f12,12(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f12.f64 = double(temp.f32);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stfs f12,88(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// lwz r10,88(r1)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
	// lfs f0,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f13.f64 = double(temp.f32);
	// rldicr r5,r10,32,63
	ctx.r5.u64 = __builtin_rotateleft64(ctx.r10.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// ld r4,80(r1)
	ctx.r4.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
	// lfs f1,16(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x82583b00
	ctx.lr = 0x8258F128;
	sub_82583B00(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F138);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F138) { __imp__sub_8258F138(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F138) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,16080
	ctx.r3.s64 = ctx.r11.s64 + 16080;
	// b 0x8258bf60
	sub_8258BF60(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F148);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F148) { __imp__sub_8258F148(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F148) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,-7304
	ctx.r3.s64 = ctx.r11.s64 + -7304;
	// b 0x8258bff0
	sub_8258BFF0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F158);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F158) { __imp__sub_8258F158(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F158) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,16376
	ctx.r3.s64 = ctx.r11.s64 + 16376;
	// b 0x8258bf60
	sub_8258BF60(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F168);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F168) { __imp__sub_8258F168(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F168) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,-6984
	ctx.r3.s64 = ctx.r11.s64 + -6984;
	// b 0x8258bff0
	sub_8258BFF0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F178);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F178) { __imp__sub_8258F178(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F178) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,-6664
	ctx.r3.s64 = ctx.r11.s64 + -6664;
	// b 0x8258c080
	sub_8258C080(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F188);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F188) { __imp__sub_8258F188(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F188) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,16632
	ctx.r3.s64 = ctx.r11.s64 + 16632;
	// b 0x8242d710
	sub_8242D710(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F198);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F198) { __imp__sub_8258F198(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F198) {
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
	// lfs f0,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// lfs f0,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,84(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// ld r4,80(r1)
	ctx.r4.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
	// lfs f0,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// lwz r11,88(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
	// rldicr r5,r11,32,63
	ctx.r5.u64 = __builtin_rotateleft64(ctx.r11.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// bl 0x8258e710
	ctx.lr = 0x8258F1D4;
	sub_8258E710(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F1E8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F1E8) { __imp__sub_8258F1E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F1E8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r10,-32005
	ctx.r10.s64 = -2097479680;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,-12368(r10)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r10.u32 + -12368);
	// lfs f1,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f2,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f2.f64 = double(temp.f32);
	// lfs f3,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f3.f64 = double(temp.f32);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r11,r11,28376
	ctx.r11.s64 = ctx.r11.s64 + 28376;
	// lwzx r4,r10,r11
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// b 0x82809790
	sub_82809790(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F220);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F220) { __imp__sub_8258F220(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F220) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F228);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F228) { __imp__sub_8258F228(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F228) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,18920
	ctx.r3.s64 = ctx.r11.s64 + 18920;
	// b 0x8258be20
	sub_8258BE20(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F238);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F238) { __imp__sub_8258F238(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F238) {
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
	// li r7,17
	ctx.r7.s64 = 17;
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// lfs f0,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lwz r6,16(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lfs f13,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f12.f64 = double(temp.f32);
	// lfs f1,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f1.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f12,88(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
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
	// bl 0x823926e8
	ctx.lr = 0x8258F288;
	sub_823926E8(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F298);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F298) { __imp__sub_8258F298(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F298) {
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
	// lfs f0,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// lfs f0,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,84(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// ld r4,80(r1)
	ctx.r4.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
	// lfs f0,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// lwz r11,88(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
	// rldicr r5,r11,32,63
	ctx.r5.u64 = __builtin_rotateleft64(ctx.r11.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// bl 0x82584f00
	ctx.lr = 0x8258F2D4;
	sub_82584F00(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F2E8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F2E8) { __imp__sub_8258F2E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F2E8) {
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
	// b 0x8258e838
	sub_8258E838(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F308);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F308) { __imp__sub_8258F308(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F308) {
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
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lfs f12,12(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f12.f64 = double(temp.f32);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f0,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f13.f64 = double(temp.f32);
	// lfs f1,16(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f1.f64 = double(temp.f32);
	// stfs f12,88(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// lwz r11,88(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// rldicr r5,r11,32,63
	ctx.r5.u64 = __builtin_rotateleft64(ctx.r11.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// ld r4,80(r1)
	ctx.r4.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
	// bl 0x825852e8
	ctx.lr = 0x8258F350;
	sub_825852E8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8258F370);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F370) { __imp__sub_8258F370(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F370) {
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
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
	// lwz r9,16(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// cntlzw r9,r9
	ctx.r9.u64 = ctx.r9.u32 == 0 ? 32 : __builtin_clz(ctx.r9.u32);
	// lwz r8,20(r10)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 20);
	// lfs f3,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f3.f64 = double(temp.f32);
	// lfs f4,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f4.f64 = double(temp.f32);
	// rlwinm r11,r9,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// lwz r9,24(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 24);
	// cntlzw r10,r8
	ctx.r10.u64 = ctx.r8.u32 == 0 ? 32 : __builtin_clz(ctx.r8.u32);
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// rlwinm r10,r10,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// clrlwi r7,r11,24
	ctx.r7.u64 = ctx.r11.u32 & 0xFF;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// clrlwi r8,r10,24
	ctx.r8.u64 = ctx.r10.u32 & 0xFF;
	// mtctr r3
	ctx.ctr.u64 = ctx.r3.u64;
	// bctrl 
	ctx.lr = 0x8258F3D0;
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

PPC_FUNC_IMPL(__imp__sub_8258F3F0);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F3F0) { __imp__sub_8258F3F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F3F0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x8258F3F8;
	__savegprlr_28(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// std r8,200(r1)
	PPC_STORE_U64(ctx.r1.u32 + 200, ctx.r8.u64);
	// mr r10,r3
	ctx.r10.u64 = ctx.r3.u64;
	// std r7,192(r1)
	PPC_STORE_U64(ctx.r1.u32 + 192, ctx.r7.u64);
	// mr r30,r5
	ctx.r30.u64 = ctx.r5.u64;
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
	// bne cr6,0x8258f440
	if (!ctx.cr6.eq) goto loc_8258F440;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r9
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r9.s32);
	// add r28,r11,r10
	ctx.r28.u64 = ctx.r11.u64 + ctx.r10.u64;
	// b 0x8258f444
	goto loc_8258F444;
loc_8258F440:
	// li r28,0
	ctx.r28.s64 = 0;
loc_8258F444:
	// lfs f0,192(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 192);
	ctx.f0.f64 = double(temp.f32);
	// li r29,0
	ctx.r29.s64 = 0;
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// cmpwi cr6,r6,-1
	ctx.cr6.compare<int32_t>(ctx.r6.s32, -1, ctx.xer);
	// lfs f0,196(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 196);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,84(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// lfs f0,200(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 200);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
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
	// ble cr6,0x8258f4b0
	if (!ctx.cr6.gt) goto loc_8258F4B0;
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// srawi r10,r6,8
	ctx.xer.ca = (ctx.r6.s32 < 0) & ((ctx.r6.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r6.s32 >> 8;
	// clrlwi r8,r6,24
	ctx.r8.u64 = ctx.r6.u32 & 0xFF;
	// lwz r11,24544(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24544);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x8258f4ac
	if (!ctx.cr6.eq) goto loc_8258F4AC;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r29,r11,r9
	ctx.r29.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x8258f4b0
	goto loc_8258F4B0;
loc_8258F4AC:
	// li r29,0
	ctx.r29.s64 = 0;
loc_8258F4B0:
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bl 0x82318f60
	ctx.lr = 0x8258F4B8;
	sub_82318F60(ctx, base);
	// bl 0x821ba030
	ctx.lr = 0x8258F4BC;
	sub_821BA030(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82318f60
	ctx.lr = 0x8258F4C8;
	sub_82318F60(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// cmplwi cr6,r28,0
	ctx.cr6.compare<uint32_t>(ctx.r28.u32, 0, ctx.xer);
	// beq cr6,0x8258f538
	if (ctx.cr6.eq) goto loc_8258F538;
	// lwz r11,540(r28)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r28.u32 + 540);
	// li r4,2118
	ctx.r4.s64 = 2118;
	// addi r3,r11,68
	ctx.r3.s64 = ctx.r11.s64 + 68;
	// bl 0x821282b0
	ctx.lr = 0x8258F4E4;
	sub_821282B0(ctx, base);
	// mr r28,r3
	ctx.r28.u64 = ctx.r3.u64;
	// cmplwi cr6,r28,0
	ctx.cr6.compare<uint32_t>(ctx.r28.u32, 0, ctx.xer);
	// beq cr6,0x8258f538
	if (ctx.cr6.eq) goto loc_8258F538;
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// lwz r3,-21944(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + -21944);
	// bl 0x821250b0
	ctx.lr = 0x8258F4FC;
	sub_821250B0(ctx, base);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x8258f52c
	if (ctx.cr6.eq) goto loc_8258F52C;
	// addi r7,r1,80
	ctx.r7.s64 = ctx.r1.s64 + 80;
	// mr r6,r29
	ctx.r6.u64 = ctx.r29.u64;
	// mr r5,r30
	ctx.r5.u64 = ctx.r30.u64;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x824d0120
	ctx.lr = 0x8258F518;
	sub_824D0120(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// bl 0x824c9928
	ctx.lr = 0x8258F524;
	sub_824C9928(ctx, base);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
loc_8258F52C:
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// bl 0x824c9928
	ctx.lr = 0x8258F538;
	sub_824C9928(ctx, base);
loc_8258F538:
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F540);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F540) { __imp__sub_8258F540(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F540) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r10,24(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// lfs f1,8(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f1.f64 = double(temp.f32);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f2,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f2.f64 = double(temp.f32);
	// cntlzw r10,r10
	ctx.r10.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lfs f3,16(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f3.f64 = double(temp.f32);
	// rlwinm r11,r10,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r9,r11,24
	ctx.r9.u64 = ctx.r11.u32 & 0xFF;
	// b 0x8258c118
	sub_8258C118(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F578);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F578) { __imp__sub_8258F578(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F578) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8257e398
	sub_8257E398(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F588);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F588) { __imp__sub_8258F588(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F588) {
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
	// lwz r10,36(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// lfs f1,8(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f1.f64 = double(temp.f32);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f2,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f2.f64 = double(temp.f32);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lfs f3,16(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f3.f64 = double(temp.f32);
	// lfs f4,28(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	ctx.f4.f64 = double(temp.f32);
	// lwz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lfs f5,32(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 32);
	ctx.f5.f64 = double(temp.f32);
	// lwz r9,24(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// stw r10,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r10.u32);
	// bl 0x8258c340
	ctx.lr = 0x8258F5C8;
	sub_8258C340(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F5D8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F5D8) { __imp__sub_8258F5D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F5D8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8257e490
	sub_8257E490(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F5E8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F5E8) { __imp__sub_8258F5E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F5E8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// b 0x8258c508
	sub_8258C508(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F600);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F600) { __imp__sub_8258F600(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F600) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x8257ea08
	ctx.lr = 0x8258F624;
	sub_8257EA08(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8258F648);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F648) { __imp__sub_8258F648(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F648) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8257ead8
	sub_8257EAD8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F658);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F658) { __imp__sub_8258F658(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F658) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8257efe0
	sub_8257EFE0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F668);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F668) { __imp__sub_8258F668(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F668) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// b 0x8258c690
	sub_8258C690(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F688);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F688) { __imp__sub_8258F688(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F688) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
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
	// bne cr6,0x8258f6d4
	if (!ctx.cr6.eq) goto loc_8258F6D4;
	// lwz r7,12(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r7,r10
	ctx.r11.s64 = int64_t(ctx.r7.s32) * int64_t(ctx.r10.s32);
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// lfs f0,484(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 484);
	ctx.f0.f64 = double(temp.f32);
	// fctiwz f0,f0
	ctx.f0.s64 = (ctx.f0.f64 > double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&ctx.f0.f64));
	// stfiwx f0,0,r8
	PPC_STORE_U32(ctx.r8.u32, ctx.f0.u32);
	// blr 
	return;
loc_8258F6D4:
	// li r11,0
	ctx.r11.s64 = 0;
	// lfs f0,484(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 484);
	ctx.f0.f64 = double(temp.f32);
	// fctiwz f0,f0
	ctx.f0.s64 = (ctx.f0.f64 > double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&ctx.f0.f64));
	// stfiwx f0,0,r8
	PPC_STORE_U32(ctx.r8.u32, ctx.f0.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F6E8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F6E8) { __imp__sub_8258F6E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F6E8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8257fac0
	sub_8257FAC0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F6F8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F6F8) { __imp__sub_8258F6F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F6F8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r8,0
	ctx.r8.s64 = 0;
	// li r7,0
	ctx.r7.s64 = 0;
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
	// b 0x8258c828
	sub_8258C828(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F718);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F718) { __imp__sub_8258F718(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F718) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r8,0
	ctx.r8.s64 = 0;
	// li r7,1
	ctx.r7.s64 = 1;
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
	// b 0x8258c828
	sub_8258C828(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F738);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F738) { __imp__sub_8258F738(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F738) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r8,1
	ctx.r8.s64 = 1;
	// li r7,0
	ctx.r7.s64 = 0;
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
	// b 0x8258c828
	sub_8258C828(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F758);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F758) { __imp__sub_8258F758(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F758) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8258cad8
	sub_8258CAD8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F768);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F768) { __imp__sub_8258F768(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F768) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x8258cc18
	sub_8258CC18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F780);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F780) { __imp__sub_8258F780(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F780) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stfd f31,-16(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.f31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r10,31824(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 31824);
	// lfs f31,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f31.f64 = double(temp.f32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// srawi r8,r9,8
	ctx.xer.ca = (ctx.r9.s32 < 0) & ((ctx.r9.u32 & 0xFF) != 0);
	ctx.r8.s64 = ctx.r9.s32 >> 8;
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// clrlwi r11,r9,24
	ctx.r11.u64 = ctx.r9.u32 & 0xFF;
	// lwz r7,4(r10)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// lbzx r9,r7,r8
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r7.u32 + ctx.r8.u32);
	// cmpw cr6,r9,r11
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r11.s32, ctx.xer);
	// bne cr6,0x8258f7d8
	if (!ctx.cr6.eq) goto loc_8258F7D8;
	// lwz r11,12(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 12);
	// lwz r10,0(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// mullw r11,r11,r8
	ctx.r11.s64 = int64_t(ctx.r11.s32) * int64_t(ctx.r8.s32);
	// add r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
	// b 0x8258f7dc
	goto loc_8258F7DC;
loc_8258F7D8:
	// li r11,0
	ctx.r11.s64 = 0;
loc_8258F7DC:
	// lwz r3,120(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 120);
	// bl 0x823e9418
	ctx.lr = 0x8258F7E4;
	sub_823E9418(ctx, base);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x8258f7f0
	if (ctx.cr6.eq) goto loc_8258F7F0;
	// stfs f31,84(r3)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r3.u32 + 84, temp.u32);
loc_8258F7F0:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f31,-16(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F808);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F808) { __imp__sub_8258F808(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F808) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x82580018
	sub_82580018(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F818);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F818) { __imp__sub_8258F818(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F818) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8258cd40
	sub_8258CD40(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F858);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F858) { __imp__sub_8258F858(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F858) {
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
	// lis r11,-32055
	ctx.r11.s64 = -2100756480;
	// addi r5,r1,84
	ctx.r5.s64 = ctx.r1.s64 + 84;
	// addi r3,r11,-27888
	ctx.r3.s64 = ctx.r11.s64 + -27888;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// lwz r31,0(r10)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x822898d8
	ctx.lr = 0x8258F884;
	sub_822898D8(ctx, base);
	// lwz r10,80(r1)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// addi r11,r11,-10032
	ctx.r11.s64 = ctx.r11.s64 + -10032;
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lwz r11,60(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 60);
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

PPC_FUNC_IMPL(__imp__sub_8258F8B8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F8B8) { __imp__sub_8258F8B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F8B8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825804f0
	sub_825804F0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F8C8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F8C8) { __imp__sub_8258F8C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F8C8) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r10,31824(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 31824);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// srawi r8,r9,8
	ctx.xer.ca = (ctx.r9.s32 < 0) & ((ctx.r9.u32 & 0xFF) != 0);
	ctx.r8.s64 = ctx.r9.s32 >> 8;
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// clrlwi r11,r9,24
	ctx.r11.u64 = ctx.r9.u32 & 0xFF;
	// lwz r7,4(r10)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// lbzx r9,r7,r8
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r7.u32 + ctx.r8.u32);
	// cmpw cr6,r9,r11
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r11.s32, ctx.xer);
	// bne cr6,0x8258f910
	if (!ctx.cr6.eq) goto loc_8258F910;
	// lwz r11,12(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 12);
	// lwz r10,0(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// mullw r11,r11,r8
	ctx.r11.s64 = int64_t(ctx.r11.s32) * int64_t(ctx.r8.s32);
	// add r3,r11,r10
	ctx.r3.u64 = ctx.r11.u64 + ctx.r10.u64;
	// b 0x8258f914
	goto loc_8258F914;
loc_8258F910:
	// li r3,0
	ctx.r3.s64 = 0;
loc_8258F914:
	// b 0x8227af10
	sub_8227AF10(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F918);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F918) { __imp__sub_8258F918(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F918) {
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
	// lwz r9,24(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// lfs f3,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f3.f64 = double(temp.f32);
	// lwz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// b 0x825807c0
	sub_825807C0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F940);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F940) { __imp__sub_8258F940(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F940) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
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
	// bne cr6,0x8258f98c
	if (!ctx.cr6.eq) goto loc_8258F98C;
	// lwz r7,12(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r7,r10
	ctx.r11.s64 = int64_t(ctx.r7.s32) * int64_t(ctx.r10.s32);
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// lfs f0,2488(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2488);
	ctx.f0.f64 = double(temp.f32);
	// fctiwz f0,f0
	ctx.f0.s64 = (ctx.f0.f64 > double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&ctx.f0.f64));
	// stfiwx f0,0,r8
	PPC_STORE_U32(ctx.r8.u32, ctx.f0.u32);
	// blr 
	return;
loc_8258F98C:
	// li r11,0
	ctx.r11.s64 = 0;
	// lfs f0,2488(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2488);
	ctx.f0.f64 = double(temp.f32);
	// fctiwz f0,f0
	ctx.f0.s64 = (ctx.f0.f64 > double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&ctx.f0.f64));
	// stfiwx f0,0,r8
	PPC_STORE_U32(ctx.r8.u32, ctx.f0.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258F9A0);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F9A0) { __imp__sub_8258F9A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F9A0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x82580b28
	ctx.lr = 0x8258F9C4;
	sub_82580B28(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8258F9E8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258F9E8) { __imp__sub_8258F9E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258F9E8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// bl 0x825810e8
	ctx.lr = 0x8258FA10;
	sub_825810E8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8258FA30);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FA30) { __imp__sub_8258FA30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FA30) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// bl 0x82581160
	ctx.lr = 0x8258FA58;
	sub_82581160(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8258FA78);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FA78) { __imp__sub_8258FA78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FA78) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r10,r10
	ctx.r10.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// rlwinm r10,r10,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// clrlwi r6,r10,24
	ctx.r6.u64 = ctx.r10.u32 & 0xFF;
	// b 0x82581200
	sub_82581200(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FAA0);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FAA0) { __imp__sub_8258FAA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FAA0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x825812a0
	sub_825812A0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FAB8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FAB8) { __imp__sub_8258FAB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FAB8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stfd f31,-16(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.f31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r10,-31981
	ctx.r10.s64 = -2095906816;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r10,31824(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 31824);
	// lfs f31,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f31.f64 = double(temp.f32);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// srawi r8,r9,8
	ctx.xer.ca = (ctx.r9.s32 < 0) & ((ctx.r9.u32 & 0xFF) != 0);
	ctx.r8.s64 = ctx.r9.s32 >> 8;
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// clrlwi r11,r9,24
	ctx.r11.u64 = ctx.r9.u32 & 0xFF;
	// lwz r7,4(r10)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// lbzx r9,r7,r8
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r7.u32 + ctx.r8.u32);
	// cmpw cr6,r9,r11
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r11.s32, ctx.xer);
	// bne cr6,0x8258fb10
	if (!ctx.cr6.eq) goto loc_8258FB10;
	// lwz r11,12(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 12);
	// lwz r10,0(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// mullw r11,r11,r8
	ctx.r11.s64 = int64_t(ctx.r11.s32) * int64_t(ctx.r8.s32);
	// add r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
	// b 0x8258fb14
	goto loc_8258FB14;
loc_8258FB10:
	// li r11,0
	ctx.r11.s64 = 0;
loc_8258FB14:
	// lwz r3,120(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 120);
	// bl 0x823e9418
	ctx.lr = 0x8258FB1C;
	sub_823E9418(ctx, base);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x8258fb2c
	if (ctx.cr6.eq) goto loc_8258FB2C;
	// fmr f1,f31
	ctx.fpscr.disableFlushMode();
	ctx.f1.f64 = ctx.f31.f64;
	// bl 0x82419f18
	ctx.lr = 0x8258FB2C;
	sub_82419F18(ctx, base);
loc_8258FB2C:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f31,-16(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FB40);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FB40) { __imp__sub_8258FB40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FB40) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x82581320
	sub_82581320(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FB58);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FB58) { __imp__sub_8258FB58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FB58) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x825813b0
	ctx.lr = 0x8258FB84;
	sub_825813B0(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8258FBA8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FBA8) { __imp__sub_8258FBA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FBA8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// bl 0x82581440
	ctx.lr = 0x8258FBD8;
	sub_82581440(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8258FBF8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FBF8) { __imp__sub_8258FBF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FBF8) {
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
	// b 0x8257c0b8
	sub_8257C0B8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FC20);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FC20) { __imp__sub_8258FC20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FC20) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
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
	// bne cr6,0x8258fc60
	if (!ctx.cr6.eq) goto loc_8258FC60;
	// lwz r7,12(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r7,r10
	ctx.r11.s64 = int64_t(ctx.r7.s32) * int64_t(ctx.r10.s32);
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x8258fc64
	goto loc_8258FC64;
loc_8258FC60:
	// li r11,0
	ctx.r11.s64 = 0;
loc_8258FC64:
	// lhz r10,46(r11)
	ctx.r10.u64 = PPC_LOAD_U16(ctx.r11.u32 + 46);
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
	// lwz r11,60(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 60);
	// stw r11,0(r8)
	PPC_STORE_U32(ctx.r8.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FC88);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FC88) { __imp__sub_8258FC88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FC88) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825816e8
	sub_825816E8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FC98);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FC98) { __imp__sub_8258FC98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FC98) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8258d0b8
	sub_8258D0B8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FCA8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FCA8) { __imp__sub_8258FCA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FCA8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8258d248
	sub_8258D248(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FCB8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FCB8) { __imp__sub_8258FCB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FCB8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8258d180
	sub_8258D180(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FCC8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FCC8) { __imp__sub_8258FCC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FCC8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8258d310
	sub_8258D310(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FCD8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FCD8) { __imp__sub_8258FCD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FCD8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8258cdd8
	sub_8258CDD8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FCE8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FCE8) { __imp__sub_8258FCE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FCE8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8258ce90
	sub_8258CE90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FCF8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FCF8) { __imp__sub_8258FCF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FCF8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x8257c338
	sub_8257C338(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FD10);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FD10) { __imp__sub_8258FD10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FD10) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825820d0
	sub_825820D0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FD20);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FD20) { __imp__sub_8258FD20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FD20) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8257c3b8
	sub_8257C3B8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FD30);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FD30) { __imp__sub_8258FD30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FD30) {
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
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r4,8
	ctx.r4.s64 = 8;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r31,4(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x821d64c0
	ctx.lr = 0x8258FD54;
	sub_821D64C0(ctx, base);
	// cmplwi cr6,r3,47
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 47, ctx.xer);
	// bgt cr6,0x8258fd78
	if (ctx.cr6.gt) goto loc_8258FD78;
	// lis r11,-32014
	ctx.r11.s64 = -2098069504;
	// mulli r10,r3,92
	ctx.r10.s64 = static_cast<int64_t>(ctx.r3.u64 * static_cast<uint64_t>(92));
	// addi r11,r11,-12384
	ctx.r11.s64 = ctx.r11.s64 + -12384;
	// addi r11,r11,80
	ctx.r11.s64 = ctx.r11.s64 + 80;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lfs f0,8(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// b 0x8258fd80
	goto loc_8258FD80;
loc_8258FD78:
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,3068(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3068);
	ctx.f0.f64 = double(temp.f32);
loc_8258FD80:
	// stfs f0,0(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r31.u32 + 0, temp.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FD98);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FD98) { __imp__sub_8258FD98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FD98) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-31981
	ctx.r11.s64 = -2095906816;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
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
	// bne cr6,0x8258fde4
	if (!ctx.cr6.eq) goto loc_8258FDE4;
	// lwz r7,12(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r7,r10
	ctx.r11.s64 = int64_t(ctx.r7.s32) * int64_t(ctx.r10.s32);
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// lwz r11,540(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 540);
	// lbz r11,240(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 240);
	// stw r11,0(r8)
	PPC_STORE_U32(ctx.r8.u32 + 0, ctx.r11.u32);
	// blr 
	return;
loc_8258FDE4:
	// li r11,0
	ctx.r11.s64 = 0;
	// lwz r11,540(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 540);
	// lbz r11,240(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 240);
	// stw r11,0(r8)
	PPC_STORE_U32(ctx.r8.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FDF8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FDF8) { __imp__sub_8258FDF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FDF8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8258d3d8
	sub_8258D3D8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FE08);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FE08) { __imp__sub_8258FE08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FE08) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825822c8
	sub_825822C8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FE18);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FE18) { __imp__sub_8258FE18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FE18) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x825823e8
	sub_825823E8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FE30);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FE30) { __imp__sub_8258FE30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FE30) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x82582488
	sub_82582488(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FE94);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FE94) { __imp__sub_8258FE94(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FE94) {
	PPC_FUNC_PROLOGUE();
	// li r11,0
	ctx.r11.s64 = 0;
	// lwz r11,532(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 532);
	// lwz r11,284(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 284);
	// stw r11,0(r8)
	PPC_STORE_U32(ctx.r8.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FEA8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FEA8) { __imp__sub_8258FEA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FEA8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x8258d718
	sub_8258D718(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FEC0);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FEC0) { __imp__sub_8258FEC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FEC0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x8258d7c0
	sub_8258D7C0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FED8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FED8) { __imp__sub_8258FED8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FED8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8258d868
	sub_8258D868(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FEE8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FEE8) { __imp__sub_8258FEE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FEE8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x82582860
	sub_82582860(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FEF8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FEF8) { __imp__sub_8258FEF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FEF8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8258d900
	sub_8258D900(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FF08);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FF08) { __imp__sub_8258FF08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FF08) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8258d9d0
	sub_8258D9D0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FF18);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FF18) { __imp__sub_8258FF18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FF18) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r10,0
	ctx.r10.s64 = 0;
	// lwz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FF30);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FF30) { __imp__sub_8258FF30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FF30) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x82582cb0
	sub_82582CB0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FF40);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FF40) { __imp__sub_8258FF40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FF40) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r9,0(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// lwz r4,4(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
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
	// bne cr6,0x8258ff90
	if (!ctx.cr6.eq) goto loc_8258FF90;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r31,r11,r9
	ctx.r31.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x8258ff94
	goto loc_8258FF94;
loc_8258FF90:
	// li r31,0
	ctx.r31.s64 = 0;
loc_8258FF94:
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// lwz r3,-11384(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + -11384);
	// bl 0x8220a0a8
	ctx.lr = 0x8258FFA0;
	sub_8220A0A8(ctx, base);
	// stw r3,2728(r31)
	PPC_STORE_U32(ctx.r31.u32 + 2728, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8258FFB8);  /* forward decl */
PPC_WEAK_FUNC(sub_8258FFB8) { __imp__sub_8258FFB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8258FFB8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r9,0(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// lwz r4,4(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
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
	// bne cr6,0x82590008
	if (!ctx.cr6.eq) goto loc_82590008;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r31,r11,r9
	ctx.r31.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x8259000c
	goto loc_8259000C;
loc_82590008:
	// li r31,0
	ctx.r31.s64 = 0;
loc_8259000C:
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// lwz r3,-11384(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + -11384);
	// bl 0x8220a0a8
	ctx.lr = 0x82590018;
	sub_8220A0A8(ctx, base);
	// stw r3,2732(r31)
	PPC_STORE_U32(ctx.r31.u32 + 2732, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82590030);  /* forward decl */
PPC_WEAK_FUNC(sub_82590030) { __imp__sub_82590030(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82590030) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r10,31824(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 31824);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r31,8(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// srawi r11,r9,8
	ctx.xer.ca = (ctx.r9.s32 < 0) & ((ctx.r9.u32 & 0xFF) != 0);
	ctx.r11.s64 = ctx.r9.s32 >> 8;
	// clrlwi r9,r9,24
	ctx.r9.u64 = ctx.r9.u32 & 0xFF;
	// lwz r8,4(r10)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// lbzx r8,r8,r11
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r8.u32 + ctx.r11.u32);
	// cmpw cr6,r8,r9
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r9.s32, ctx.xer);
	// bne cr6,0x82590084
	if (!ctx.cr6.eq) goto loc_82590084;
	// lwz r9,12(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 12);
	// lwz r10,0(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// mullw r11,r9,r11
	ctx.r11.s64 = int64_t(ctx.r9.s32) * int64_t(ctx.r11.s32);
	// add r3,r11,r10
	ctx.r3.u64 = ctx.r11.u64 + ctx.r10.u64;
	// b 0x82590088
	goto loc_82590088;
loc_82590084:
	// li r3,0
	ctx.r3.s64 = 0;
loc_82590088:
	// bl 0x823e13c0
	ctx.lr = 0x8259008C;
	sub_823E13C0(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825900A8);  /* forward decl */
PPC_WEAK_FUNC(sub_825900A8) { __imp__sub_825900A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825900A8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x82583020
	sub_82583020(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825900B8);  /* forward decl */
PPC_WEAK_FUNC(sub_825900B8) { __imp__sub_825900B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825900B8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825831a8
	sub_825831A8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825900C8);  /* forward decl */
PPC_WEAK_FUNC(sub_825900C8) { __imp__sub_825900C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825900C8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8258daa0
	sub_8258DAA0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825900D8);  /* forward decl */
PPC_WEAK_FUNC(sub_825900D8) { __imp__sub_825900D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825900D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x825900E0;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,4(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r31,0(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// bl 0x82318f60
	ctx.lr = 0x825900F4;
	sub_82318F60(ctx, base);
	// lis r11,-32039
	ctx.r11.s64 = -2099707904;
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// lwz r29,15072(r11)
	ctx.r29.u64 = PPC_LOAD_U32(ctx.r11.u32 + 15072);
	// bl 0x821d2088
	ctx.lr = 0x82590104;
	sub_821D2088(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// li r5,3
	ctx.r5.s64 = 3;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// mr r7,r30
	ctx.r7.u64 = ctx.r30.u64;
	// bl 0x82308980
	ctx.lr = 0x8259011C;
	sub_82308980(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82590128);  /* forward decl */
PPC_WEAK_FUNC(sub_82590128) { __imp__sub_82590128(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82590128) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x82583340
	sub_82583340(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82590138);  /* forward decl */
PPC_WEAK_FUNC(sub_82590138) { __imp__sub_82590138(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82590138) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8258db40
	sub_8258DB40(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82590170);  /* forward decl */
PPC_WEAK_FUNC(sub_82590170) { __imp__sub_82590170(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82590170) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,-8296
	ctx.r3.s64 = ctx.r11.s64 + -8296;
	// b 0x8258f370
	sub_8258F370(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82590180);  /* forward decl */
PPC_WEAK_FUNC(sub_82590180) { __imp__sub_82590180(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82590180) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// mr r11,r10
	ctx.r11.u64 = ctx.r10.u64;
	// lwz r9,4(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// lwz r3,0(r10)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// cntlzw r10,r9
	ctx.r10.u64 = ctx.r9.u32 == 0 ? 32 : __builtin_clz(ctx.r9.u32);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// rlwinm r10,r10,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// clrlwi r4,r10,24
	ctx.r4.u64 = ctx.r10.u32 & 0xFF;
	// b 0x82583d18
	sub_82583D18(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82590214);  /* forward decl */
PPC_WEAK_FUNC(sub_82590214) { __imp__sub_82590214(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82590214) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82590218);  /* forward decl */
PPC_WEAK_FUNC(sub_82590218) { __imp__sub_82590218(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82590218) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f1,12(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f1.f64 = double(temp.f32);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x82584890
	sub_82584890(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82590230);  /* forward decl */
PPC_WEAK_FUNC(sub_82590230) { __imp__sub_82590230(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82590230) {
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
	// lfs f0,16(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f0.f64 = double(temp.f32);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lfs f0,20(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	ctx.f0.f64 = double(temp.f32);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// stfs f0,84(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lfs f0,24(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	ctx.f0.f64 = double(temp.f32);
	// ld r7,80(r1)
	ctx.r7.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
	// stfs f0,88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// lwz r10,88(r1)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
	// rldicr r8,r10,32,63
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// bl 0x8258f3f0
	ctx.lr = 0x82590278;
	sub_8258F3F0(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82590288);  /* forward decl */
PPC_WEAK_FUNC(sub_82590288) { __imp__sub_82590288(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82590288) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x82584a70
	ctx.lr = 0x825902AC;
	sub_82584A70(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825902D0);  /* forward decl */
PPC_WEAK_FUNC(sub_825902D0) { __imp__sub_825902D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825902D0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x825851b8
	sub_825851B8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825902F0);  /* forward decl */
PPC_WEAK_FUNC(sub_825902F0) { __imp__sub_825902F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825902F0) {
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
	// mr r11,r4
	ctx.r11.u64 = ctx.r4.u64;
	// mr r8,r3
	ctx.r8.u64 = ctx.r3.u64;
	// lwz r9,12(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r10,8(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// addi r9,r9,4
	ctx.r9.s64 = ctx.r9.s64 + 4;
	// rlwinm r7,r9,2,0,29
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// lwz r9,20(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 20);
	// lfs f12,16(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 16);
	ctx.f12.f64 = double(temp.f32);
	// lfs f0,8(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// lwz r3,0(r10)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// lfs f13,12(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 12);
	ctx.f13.f64 = double(temp.f32);
	// lwz r4,4(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// stfs f12,88(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// lwz r10,88(r1)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// rldicr r6,r10,32,63
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r10.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// stwx r9,r7,r11
	PPC_STORE_U32(ctx.r7.u32 + ctx.r11.u32, ctx.r9.u32);
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lfs f12,8(r9)
	temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + 8);
	ctx.f12.f64 = double(temp.f32);
	// lfs f11,4(r9)
	temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + 4);
	ctx.f11.f64 = double(temp.f32);
	// addi r10,r10,2
	ctx.r10.s64 = ctx.r10.s64 + 2;
	// lfs f0,0(r9)
	temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// ld r5,80(r1)
	ctx.r5.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
	// rlwinm r10,r10,4,0,27
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
	// add r10,r10,r11
	ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
	// stfs f0,0(r10)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// stfs f11,4(r10)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// stfs f12,8(r10)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r10.u32 + 8, temp.u32);
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// addi r31,r10,1
	ctx.r31.s64 = ctx.r10.s64 + 1;
	// addi r9,r10,2
	ctx.r9.s64 = ctx.r10.s64 + 2;
	// rlwinm r10,r9,4,0,27
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
	// add r7,r10,r11
	ctx.r7.u64 = ctx.r10.u64 + ctx.r11.u64;
	// stw r31,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r31.u32);
	// mtctr r8
	ctx.ctr.u64 = ctx.r8.u64;
	// bctrl 
	ctx.lr = 0x82590394;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
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

PPC_FUNC_IMPL(__imp__sub_825903A8);  /* forward decl */
PPC_WEAK_FUNC(sub_825903A8) { __imp__sub_825903A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825903A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,17560
	ctx.r3.s64 = ctx.r11.s64 + 17560;
	// b 0x825902f0
	sub_825902F0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825903B8);  /* forward decl */
PPC_WEAK_FUNC(sub_825903B8) { __imp__sub_825903B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825903B8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-2752
	ctx.r4.s64 = ctx.r11.s64 + -2752;
	// addi r3,r10,27508
	ctx.r3.s64 = ctx.r10.s64 + 27508;
	// bl 0x827d6c90
	ctx.lr = 0x825903D8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-2696
	ctx.r4.s64 = ctx.r11.s64 + -2696;
	// addi r3,r10,27496
	ctx.r3.s64 = ctx.r10.s64 + 27496;
	// bl 0x827d6c90
	ctx.lr = 0x825903EC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-2680
	ctx.r4.s64 = ctx.r11.s64 + -2680;
	// addi r3,r10,27476
	ctx.r3.s64 = ctx.r10.s64 + 27476;
	// bl 0x827d6c90
	ctx.lr = 0x82590400;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-2600
	ctx.r4.s64 = ctx.r11.s64 + -2600;
	// addi r3,r10,27456
	ctx.r3.s64 = ctx.r10.s64 + 27456;
	// bl 0x827d6c90
	ctx.lr = 0x82590414;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-464
	ctx.r4.s64 = ctx.r11.s64 + -464;
	// addi r3,r10,27432
	ctx.r3.s64 = ctx.r10.s64 + 27432;
	// bl 0x827d6c90
	ctx.lr = 0x82590428;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21680
	ctx.r4.s64 = ctx.r11.s64 + 21680;
	// addi r3,r10,27408
	ctx.r3.s64 = ctx.r10.s64 + 27408;
	// bl 0x827d6c90
	ctx.lr = 0x8259043C;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21768
	ctx.r4.s64 = ctx.r11.s64 + 21768;
	// addi r3,r10,27380
	ctx.r3.s64 = ctx.r10.s64 + 27380;
	// bl 0x827d6c90
	ctx.lr = 0x82590450;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21864
	ctx.r4.s64 = ctx.r11.s64 + 21864;
	// addi r3,r10,27360
	ctx.r3.s64 = ctx.r10.s64 + 27360;
	// bl 0x827d6c90
	ctx.lr = 0x82590464;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21984
	ctx.r4.s64 = ctx.r11.s64 + 21984;
	// addi r3,r10,27340
	ctx.r3.s64 = ctx.r10.s64 + 27340;
	// bl 0x827d6c90
	ctx.lr = 0x82590478;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,27316
	ctx.r3.s64 = ctx.r10.s64 + 27316;
	// bl 0x827d6c90
	ctx.lr = 0x8259048C;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22000
	ctx.r4.s64 = ctx.r11.s64 + 22000;
	// addi r3,r10,27300
	ctx.r3.s64 = ctx.r10.s64 + 27300;
	// bl 0x827d6c90
	ctx.lr = 0x825904A0;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22064
	ctx.r4.s64 = ctx.r11.s64 + 22064;
	// addi r3,r10,27280
	ctx.r3.s64 = ctx.r10.s64 + 27280;
	// bl 0x827d6c90
	ctx.lr = 0x825904B4;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22136
	ctx.r4.s64 = ctx.r11.s64 + 22136;
	// addi r3,r10,27260
	ctx.r3.s64 = ctx.r10.s64 + 27260;
	// bl 0x827d6c90
	ctx.lr = 0x825904C8;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22256
	ctx.r4.s64 = ctx.r11.s64 + 22256;
	// addi r3,r10,27232
	ctx.r3.s64 = ctx.r10.s64 + 27232;
	// bl 0x827d6c90
	ctx.lr = 0x825904DC;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22280
	ctx.r4.s64 = ctx.r11.s64 + 22280;
	// addi r3,r10,27204
	ctx.r3.s64 = ctx.r10.s64 + 27204;
	// bl 0x827d6c90
	ctx.lr = 0x825904F0;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22400
	ctx.r4.s64 = ctx.r11.s64 + 22400;
	// addi r3,r10,27180
	ctx.r3.s64 = ctx.r10.s64 + 27180;
	// bl 0x827d6c90
	ctx.lr = 0x82590504;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22520
	ctx.r4.s64 = ctx.r11.s64 + 22520;
	// addi r3,r10,27156
	ctx.r3.s64 = ctx.r10.s64 + 27156;
	// bl 0x827d6c90
	ctx.lr = 0x82590518;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22640
	ctx.r4.s64 = ctx.r11.s64 + 22640;
	// addi r3,r10,27124
	ctx.r3.s64 = ctx.r10.s64 + 27124;
	// bl 0x827d6c90
	ctx.lr = 0x8259052C;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22752
	ctx.r4.s64 = ctx.r11.s64 + 22752;
	// addi r3,r10,27096
	ctx.r3.s64 = ctx.r10.s64 + 27096;
	// bl 0x827d6c90
	ctx.lr = 0x82590540;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22864
	ctx.r4.s64 = ctx.r11.s64 + 22864;
	// addi r3,r10,27068
	ctx.r3.s64 = ctx.r10.s64 + 27068;
	// bl 0x827d6c90
	ctx.lr = 0x82590554;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22976
	ctx.r4.s64 = ctx.r11.s64 + 22976;
	// addi r3,r10,27036
	ctx.r3.s64 = ctx.r10.s64 + 27036;
	// bl 0x827d6c90
	ctx.lr = 0x82590568;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,23088
	ctx.r4.s64 = ctx.r11.s64 + 23088;
	// addi r3,r10,27008
	ctx.r3.s64 = ctx.r10.s64 + 27008;
	// bl 0x827d6c90
	ctx.lr = 0x8259057C;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,23200
	ctx.r4.s64 = ctx.r11.s64 + 23200;
	// addi r3,r10,26980
	ctx.r3.s64 = ctx.r10.s64 + 26980;
	// bl 0x827d6c90
	ctx.lr = 0x82590590;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,23312
	ctx.r4.s64 = ctx.r11.s64 + 23312;
	// addi r3,r10,26948
	ctx.r3.s64 = ctx.r10.s64 + 26948;
	// bl 0x827d6c90
	ctx.lr = 0x825905A4;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,23424
	ctx.r4.s64 = ctx.r11.s64 + 23424;
	// addi r3,r10,26916
	ctx.r3.s64 = ctx.r10.s64 + 26916;
	// bl 0x827d6c90
	ctx.lr = 0x825905B8;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,23536
	ctx.r4.s64 = ctx.r11.s64 + 23536;
	// addi r3,r10,26884
	ctx.r3.s64 = ctx.r10.s64 + 26884;
	// bl 0x827d6c90
	ctx.lr = 0x825905CC;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,23648
	ctx.r4.s64 = ctx.r11.s64 + 23648;
	// addi r3,r10,26856
	ctx.r3.s64 = ctx.r10.s64 + 26856;
	// bl 0x827d6c90
	ctx.lr = 0x825905E0;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,23664
	ctx.r4.s64 = ctx.r11.s64 + 23664;
	// addi r3,r10,26832
	ctx.r3.s64 = ctx.r10.s64 + 26832;
	// bl 0x827d6c90
	ctx.lr = 0x825905F4;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,23680
	ctx.r4.s64 = ctx.r11.s64 + 23680;
	// addi r3,r10,26808
	ctx.r3.s64 = ctx.r10.s64 + 26808;
	// bl 0x827d6c90
	ctx.lr = 0x82590608;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,23696
	ctx.r4.s64 = ctx.r11.s64 + 23696;
	// addi r3,r10,26776
	ctx.r3.s64 = ctx.r10.s64 + 26776;
	// bl 0x827d6c90
	ctx.lr = 0x8259061C;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,23800
	ctx.r4.s64 = ctx.r11.s64 + 23800;
	// addi r3,r10,26748
	ctx.r3.s64 = ctx.r10.s64 + 26748;
	// bl 0x827d6c90
	ctx.lr = 0x82590630;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,23904
	ctx.r4.s64 = ctx.r11.s64 + 23904;
	// addi r3,r10,26720
	ctx.r3.s64 = ctx.r10.s64 + 26720;
	// bl 0x827d6c90
	ctx.lr = 0x82590644;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// addi r4,r11,24008
	ctx.r4.s64 = ctx.r11.s64 + 24008;
	// lis r11,-32253
	ctx.r11.s64 = -2113732608;
	// addi r3,r11,26688
	ctx.r3.s64 = ctx.r11.s64 + 26688;
	// bl 0x827d6c90
	ctx.lr = 0x82590658;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24112
	ctx.r4.s64 = ctx.r11.s64 + 24112;
	// addi r3,r10,26660
	ctx.r3.s64 = ctx.r10.s64 + 26660;
	// bl 0x827d6c90
	ctx.lr = 0x8259066C;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24216
	ctx.r4.s64 = ctx.r11.s64 + 24216;
	// addi r3,r10,26632
	ctx.r3.s64 = ctx.r10.s64 + 26632;
	// bl 0x827d6c90
	ctx.lr = 0x82590680;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24320
	ctx.r4.s64 = ctx.r11.s64 + 24320;
	// addi r3,r10,26600
	ctx.r3.s64 = ctx.r10.s64 + 26600;
	// bl 0x827d6c90
	ctx.lr = 0x82590694;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24424
	ctx.r4.s64 = ctx.r11.s64 + 24424;
	// addi r3,r10,26568
	ctx.r3.s64 = ctx.r10.s64 + 26568;
	// bl 0x827d6c90
	ctx.lr = 0x825906A8;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24528
	ctx.r4.s64 = ctx.r11.s64 + 24528;
	// addi r3,r10,26536
	ctx.r3.s64 = ctx.r10.s64 + 26536;
	// bl 0x827d6c90
	ctx.lr = 0x825906BC;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24632
	ctx.r4.s64 = ctx.r11.s64 + 24632;
	// addi r3,r10,26520
	ctx.r3.s64 = ctx.r10.s64 + 26520;
	// bl 0x827d6c90
	ctx.lr = 0x825906D0;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24696
	ctx.r4.s64 = ctx.r11.s64 + 24696;
	// addi r3,r10,26504
	ctx.r3.s64 = ctx.r10.s64 + 26504;
	// bl 0x827d6c90
	ctx.lr = 0x825906E4;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24752
	ctx.r4.s64 = ctx.r11.s64 + 24752;
	// addi r3,r10,26480
	ctx.r3.s64 = ctx.r10.s64 + 26480;
	// bl 0x827d6c90
	ctx.lr = 0x825906F8;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24808
	ctx.r4.s64 = ctx.r11.s64 + 24808;
	// addi r3,r10,26452
	ctx.r3.s64 = ctx.r10.s64 + 26452;
	// bl 0x827d6c90
	ctx.lr = 0x8259070C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-2584
	ctx.r4.s64 = ctx.r11.s64 + -2584;
	// addi r3,r10,26428
	ctx.r3.s64 = ctx.r10.s64 + 26428;
	// bl 0x827d6c90
	ctx.lr = 0x82590720;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-2560
	ctx.r4.s64 = ctx.r11.s64 + -2560;
	// addi r3,r10,26412
	ctx.r3.s64 = ctx.r10.s64 + 26412;
	// bl 0x827d6c90
	ctx.lr = 0x82590734;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-2488
	ctx.r4.s64 = ctx.r11.s64 + -2488;
	// addi r3,r10,26392
	ctx.r3.s64 = ctx.r10.s64 + 26392;
	// bl 0x827d6c90
	ctx.lr = 0x82590748;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24896
	ctx.r4.s64 = ctx.r11.s64 + 24896;
	// addi r3,r10,26372
	ctx.r3.s64 = ctx.r10.s64 + 26372;
	// bl 0x827d6c90
	ctx.lr = 0x8259075C;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24912
	ctx.r4.s64 = ctx.r11.s64 + 24912;
	// addi r3,r10,26344
	ctx.r3.s64 = ctx.r10.s64 + 26344;
	// bl 0x827d6c90
	ctx.lr = 0x82590770;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24928
	ctx.r4.s64 = ctx.r11.s64 + 24928;
	// addi r3,r10,26324
	ctx.r3.s64 = ctx.r10.s64 + 26324;
	// bl 0x827d6c90
	ctx.lr = 0x82590784;
	sub_827D6C90(ctx, base);
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// addi r3,r10,26300
	ctx.r3.s64 = ctx.r10.s64 + 26300;
	// addi r4,r11,25000
	ctx.r4.s64 = ctx.r11.s64 + 25000;
	// bl 0x827d6c90
	ctx.lr = 0x82590798;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,25072
	ctx.r4.s64 = ctx.r11.s64 + 25072;
	// addi r3,r10,26276
	ctx.r3.s64 = ctx.r10.s64 + 26276;
	// bl 0x827d6c90
	ctx.lr = 0x825907AC;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,25200
	ctx.r4.s64 = ctx.r11.s64 + 25200;
	// addi r3,r10,26252
	ctx.r3.s64 = ctx.r10.s64 + 26252;
	// bl 0x827d6c90
	ctx.lr = 0x825907C0;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,25264
	ctx.r4.s64 = ctx.r11.s64 + 25264;
	// addi r3,r10,26224
	ctx.r3.s64 = ctx.r10.s64 + 26224;
	// bl 0x827d6c90
	ctx.lr = 0x825907D4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-2472
	ctx.r4.s64 = ctx.r11.s64 + -2472;
	// addi r3,r10,26192
	ctx.r3.s64 = ctx.r10.s64 + 26192;
	// bl 0x827d6c90
	ctx.lr = 0x825907E8;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,25384
	ctx.r4.s64 = ctx.r11.s64 + 25384;
	// addi r3,r10,26172
	ctx.r3.s64 = ctx.r10.s64 + 26172;
	// bl 0x827d6c90
	ctx.lr = 0x825907FC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-2456
	ctx.r4.s64 = ctx.r11.s64 + -2456;
	// addi r3,r10,26144
	ctx.r3.s64 = ctx.r10.s64 + 26144;
	// bl 0x827d6c90
	ctx.lr = 0x82590810;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,25472
	ctx.r4.s64 = ctx.r11.s64 + 25472;
	// addi r3,r10,26128
	ctx.r3.s64 = ctx.r10.s64 + 26128;
	// bl 0x827d6c90
	ctx.lr = 0x82590824;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-2424
	ctx.r4.s64 = ctx.r11.s64 + -2424;
	// addi r3,r10,26112
	ctx.r3.s64 = ctx.r10.s64 + 26112;
	// bl 0x827d6c90
	ctx.lr = 0x82590838;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,25488
	ctx.r4.s64 = ctx.r11.s64 + 25488;
	// addi r3,r10,26080
	ctx.r3.s64 = ctx.r10.s64 + 26080;
	// bl 0x827d6c90
	ctx.lr = 0x8259084C;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,25560
	ctx.r4.s64 = ctx.r11.s64 + 25560;
	// addi r3,r10,26064
	ctx.r3.s64 = ctx.r10.s64 + 26064;
	// bl 0x827d6c90
	ctx.lr = 0x82590860;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,25624
	ctx.r4.s64 = ctx.r11.s64 + 25624;
	// addi r3,r10,26032
	ctx.r3.s64 = ctx.r10.s64 + 26032;
	// bl 0x827d6c90
	ctx.lr = 0x82590874;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,25752
	ctx.r4.s64 = ctx.r11.s64 + 25752;
	// addi r3,r10,25988
	ctx.r3.s64 = ctx.r10.s64 + 25988;
	// bl 0x827d6c90
	ctx.lr = 0x82590888;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,25904
	ctx.r4.s64 = ctx.r11.s64 + 25904;
	// addi r3,r10,25944
	ctx.r3.s64 = ctx.r10.s64 + 25944;
	// bl 0x827d6c90
	ctx.lr = 0x8259089C;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,26056
	ctx.r4.s64 = ctx.r11.s64 + 26056;
	// addi r3,r10,25928
	ctx.r3.s64 = ctx.r10.s64 + 25928;
	// bl 0x827d6c90
	ctx.lr = 0x825908B0;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,26072
	ctx.r4.s64 = ctx.r11.s64 + 26072;
	// addi r3,r10,25908
	ctx.r3.s64 = ctx.r10.s64 + 25908;
	// bl 0x827d6c90
	ctx.lr = 0x825908C4;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,26136
	ctx.r4.s64 = ctx.r11.s64 + 26136;
	// addi r3,r10,25880
	ctx.r3.s64 = ctx.r10.s64 + 25880;
	// bl 0x827d6c90
	ctx.lr = 0x825908D8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-5856
	ctx.r4.s64 = ctx.r11.s64 + -5856;
	// addi r3,r10,25848
	ctx.r3.s64 = ctx.r10.s64 + 25848;
	// bl 0x827d6c90
	ctx.lr = 0x825908EC;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,26240
	ctx.r4.s64 = ctx.r11.s64 + 26240;
	// addi r3,r10,25828
	ctx.r3.s64 = ctx.r10.s64 + 25828;
	// bl 0x827d6c90
	ctx.lr = 0x82590900;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,26344
	ctx.r4.s64 = ctx.r11.s64 + 26344;
	// addi r3,r10,25808
	ctx.r3.s64 = ctx.r10.s64 + 25808;
	// bl 0x827d6c90
	ctx.lr = 0x82590914;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,26424
	ctx.r4.s64 = ctx.r11.s64 + 26424;
	// addi r3,r10,25792
	ctx.r3.s64 = ctx.r10.s64 + 25792;
	// bl 0x827d6c90
	ctx.lr = 0x82590928;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,26496
	ctx.r4.s64 = ctx.r11.s64 + 26496;
	// addi r3,r10,25772
	ctx.r3.s64 = ctx.r10.s64 + 25772;
	// bl 0x827d6c90
	ctx.lr = 0x8259093C;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,26512
	ctx.r4.s64 = ctx.r11.s64 + 26512;
	// addi r3,r10,25752
	ctx.r3.s64 = ctx.r10.s64 + 25752;
	// bl 0x827d6c90
	ctx.lr = 0x82590950;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,26640
	ctx.r4.s64 = ctx.r11.s64 + 26640;
	// addi r3,r10,25732
	ctx.r3.s64 = ctx.r10.s64 + 25732;
	// bl 0x827d6c90
	ctx.lr = 0x82590964;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,26784
	ctx.r4.s64 = ctx.r11.s64 + 26784;
	// addi r3,r10,25716
	ctx.r3.s64 = ctx.r10.s64 + 25716;
	// bl 0x827d6c90
	ctx.lr = 0x82590978;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-2328
	ctx.r4.s64 = ctx.r11.s64 + -2328;
	// addi r3,r10,25692
	ctx.r3.s64 = ctx.r10.s64 + 25692;
	// bl 0x827d6c90
	ctx.lr = 0x8259098C;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,26888
	ctx.r4.s64 = ctx.r11.s64 + 26888;
	// addi r3,r10,25672
	ctx.r3.s64 = ctx.r10.s64 + 25672;
	// bl 0x827d6c90
	ctx.lr = 0x825909A0;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,26904
	ctx.r4.s64 = ctx.r11.s64 + 26904;
	// addi r3,r10,25644
	ctx.r3.s64 = ctx.r10.s64 + 25644;
	// bl 0x827d6c90
	ctx.lr = 0x825909B4;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,26928
	ctx.r4.s64 = ctx.r11.s64 + 26928;
	// addi r3,r10,25620
	ctx.r3.s64 = ctx.r10.s64 + 25620;
	// bl 0x827d6c90
	ctx.lr = 0x825909C8;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,27000
	ctx.r4.s64 = ctx.r11.s64 + 27000;
	// addi r3,r10,25600
	ctx.r3.s64 = ctx.r10.s64 + 25600;
	// bl 0x827d6c90
	ctx.lr = 0x825909DC;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,27016
	ctx.r4.s64 = ctx.r11.s64 + 27016;
	// addi r3,r10,25576
	ctx.r3.s64 = ctx.r10.s64 + 25576;
	// bl 0x827d6c90
	ctx.lr = 0x825909F0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-2312
	ctx.r4.s64 = ctx.r11.s64 + -2312;
	// addi r3,r10,25556
	ctx.r3.s64 = ctx.r10.s64 + 25556;
	// bl 0x827d6c90
	ctx.lr = 0x82590A04;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-2280
	ctx.r4.s64 = ctx.r11.s64 + -2280;
	// addi r3,r10,25532
	ctx.r3.s64 = ctx.r10.s64 + 25532;
	// bl 0x827d6c90
	ctx.lr = 0x82590A18;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-2248
	ctx.r4.s64 = ctx.r11.s64 + -2248;
	// addi r3,r10,25504
	ctx.r3.s64 = ctx.r10.s64 + 25504;
	// bl 0x827d6c90
	ctx.lr = 0x82590A2C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-2216
	ctx.r4.s64 = ctx.r11.s64 + -2216;
	// addi r3,r10,25472
	ctx.r3.s64 = ctx.r10.s64 + 25472;
	// bl 0x827d6c90
	ctx.lr = 0x82590A40;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-2200
	ctx.r4.s64 = ctx.r11.s64 + -2200;
	// addi r3,r10,25440
	ctx.r3.s64 = ctx.r10.s64 + 25440;
	// bl 0x827d6c90
	ctx.lr = 0x82590A54;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-2176
	ctx.r4.s64 = ctx.r11.s64 + -2176;
	// addi r3,r10,25420
	ctx.r3.s64 = ctx.r10.s64 + 25420;
	// bl 0x827d6c90
	ctx.lr = 0x82590A68;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,27120
	ctx.r4.s64 = ctx.r11.s64 + 27120;
	// addi r3,r10,25392
	ctx.r3.s64 = ctx.r10.s64 + 25392;
	// bl 0x827d6c90
	ctx.lr = 0x82590A7C;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,27136
	ctx.r4.s64 = ctx.r11.s64 + 27136;
	// addi r3,r10,25356
	ctx.r3.s64 = ctx.r10.s64 + 25356;
	// bl 0x827d6c90
	ctx.lr = 0x82590A90;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-2040
	ctx.r4.s64 = ctx.r11.s64 + -2040;
	// addi r3,r10,25320
	ctx.r3.s64 = ctx.r10.s64 + 25320;
	// bl 0x827d6c90
	ctx.lr = 0x82590AA4;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,27224
	ctx.r4.s64 = ctx.r11.s64 + 27224;
	// addi r3,r10,25292
	ctx.r3.s64 = ctx.r10.s64 + 25292;
	// bl 0x827d6c90
	ctx.lr = 0x82590AB8;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,27312
	ctx.r4.s64 = ctx.r11.s64 + 27312;
	// addi r3,r10,25260
	ctx.r3.s64 = ctx.r10.s64 + 25260;
	// bl 0x827d6c90
	ctx.lr = 0x82590ACC;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,27440
	ctx.r4.s64 = ctx.r11.s64 + 27440;
	// addi r3,r10,25244
	ctx.r3.s64 = ctx.r10.s64 + 25244;
	// bl 0x827d6c90
	ctx.lr = 0x82590AE0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-2024
	ctx.r4.s64 = ctx.r11.s64 + -2024;
	// addi r3,r10,25212
	ctx.r3.s64 = ctx.r10.s64 + 25212;
	// bl 0x827d6c90
	ctx.lr = 0x82590AF4;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,27496
	ctx.r4.s64 = ctx.r11.s64 + 27496;
	// addi r3,r10,25184
	ctx.r3.s64 = ctx.r10.s64 + 25184;
	// bl 0x827d6c90
	ctx.lr = 0x82590B08;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,27512
	ctx.r4.s64 = ctx.r11.s64 + 27512;
	// addi r3,r10,25148
	ctx.r3.s64 = ctx.r10.s64 + 25148;
	// bl 0x827d6c90
	ctx.lr = 0x82590B1C;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,27528
	ctx.r4.s64 = ctx.r11.s64 + 27528;
	// addi r3,r10,25124
	ctx.r3.s64 = ctx.r10.s64 + 25124;
	// bl 0x827d6c90
	ctx.lr = 0x82590B30;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-2008
	ctx.r4.s64 = ctx.r11.s64 + -2008;
	// addi r3,r10,25096
	ctx.r3.s64 = ctx.r10.s64 + 25096;
	// bl 0x827d6c90
	ctx.lr = 0x82590B44;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-1960
	ctx.r4.s64 = ctx.r11.s64 + -1960;
	// addi r3,r10,25072
	ctx.r3.s64 = ctx.r10.s64 + 25072;
	// bl 0x827d6c90
	ctx.lr = 0x82590B58;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-5840
	ctx.r4.s64 = ctx.r11.s64 + -5840;
	// addi r3,r10,25040
	ctx.r3.s64 = ctx.r10.s64 + 25040;
	// bl 0x827d6c90
	ctx.lr = 0x82590B6C;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-13528
	ctx.r4.s64 = ctx.r11.s64 + -13528;
	// addi r3,r10,25004
	ctx.r3.s64 = ctx.r10.s64 + 25004;
	// bl 0x827d6c90
	ctx.lr = 0x82590B80;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-5776
	ctx.r4.s64 = ctx.r11.s64 + -5776;
	// addi r3,r10,24976
	ctx.r3.s64 = ctx.r10.s64 + 24976;
	// bl 0x827d6c90
	ctx.lr = 0x82590B94;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-13520
	ctx.r4.s64 = ctx.r11.s64 + -13520;
	// addi r3,r10,24948
	ctx.r3.s64 = ctx.r10.s64 + 24948;
	// bl 0x827d6c90
	ctx.lr = 0x82590BA8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-5760
	ctx.r4.s64 = ctx.r11.s64 + -5760;
	// addi r3,r10,24920
	ctx.r3.s64 = ctx.r10.s64 + 24920;
	// bl 0x827d6c90
	ctx.lr = 0x82590BBC;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-13512
	ctx.r4.s64 = ctx.r11.s64 + -13512;
	// addi r3,r10,24892
	ctx.r3.s64 = ctx.r10.s64 + 24892;
	// bl 0x827d6c90
	ctx.lr = 0x82590BD0;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,27544
	ctx.r4.s64 = ctx.r11.s64 + 27544;
	// addi r3,r10,24876
	ctx.r3.s64 = ctx.r10.s64 + 24876;
	// bl 0x827d6c90
	ctx.lr = 0x82590BE4;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,27640
	ctx.r4.s64 = ctx.r11.s64 + 27640;
	// addi r3,r10,24860
	ctx.r3.s64 = ctx.r10.s64 + 24860;
	// bl 0x827d6c90
	ctx.lr = 0x82590BF8;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,27736
	ctx.r4.s64 = ctx.r11.s64 + 27736;
	// addi r3,r10,24824
	ctx.r3.s64 = ctx.r10.s64 + 24824;
	// bl 0x827d6c90
	ctx.lr = 0x82590C0C;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,27760
	ctx.r4.s64 = ctx.r11.s64 + 27760;
	// addi r3,r10,24792
	ctx.r3.s64 = ctx.r10.s64 + 24792;
	// bl 0x827d6c90
	ctx.lr = 0x82590C20;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,27784
	ctx.r4.s64 = ctx.r11.s64 + 27784;
	// addi r3,r10,24760
	ctx.r3.s64 = ctx.r10.s64 + 24760;
	// bl 0x827d6c90
	ctx.lr = 0x82590C34;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,27912
	ctx.r4.s64 = ctx.r11.s64 + 27912;
	// addi r3,r10,24728
	ctx.r3.s64 = ctx.r10.s64 + 24728;
	// bl 0x827d6c90
	ctx.lr = 0x82590C48;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,28032
	ctx.r4.s64 = ctx.r11.s64 + 28032;
	// addi r3,r10,24696
	ctx.r3.s64 = ctx.r10.s64 + 24696;
	// bl 0x827d6c90
	ctx.lr = 0x82590C5C;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,28136
	ctx.r4.s64 = ctx.r11.s64 + 28136;
	// addi r3,r10,24664
	ctx.r3.s64 = ctx.r10.s64 + 24664;
	// bl 0x827d6c90
	ctx.lr = 0x82590C70;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,28256
	ctx.r4.s64 = ctx.r11.s64 + 28256;
	// addi r3,r10,24632
	ctx.r3.s64 = ctx.r10.s64 + 24632;
	// bl 0x827d6c90
	ctx.lr = 0x82590C84;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,28360
	ctx.r4.s64 = ctx.r11.s64 + 28360;
	// addi r3,r10,24604
	ctx.r3.s64 = ctx.r10.s64 + 24604;
	// bl 0x827d6c90
	ctx.lr = 0x82590C98;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// addi r4,r11,28480
	ctx.r4.s64 = ctx.r11.s64 + 28480;
	// lis r11,-32253
	ctx.r11.s64 = -2113732608;
	// addi r3,r11,24576
	ctx.r3.s64 = ctx.r11.s64 + 24576;
	// bl 0x827d6c90
	ctx.lr = 0x82590CAC;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,28584
	ctx.r4.s64 = ctx.r11.s64 + 28584;
	// addi r3,r10,24548
	ctx.r3.s64 = ctx.r10.s64 + 24548;
	// bl 0x827d6c90
	ctx.lr = 0x82590CC0;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,28704
	ctx.r4.s64 = ctx.r11.s64 + 28704;
	// addi r3,r10,24520
	ctx.r3.s64 = ctx.r10.s64 + 24520;
	// bl 0x827d6c90
	ctx.lr = 0x82590CD4;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,28808
	ctx.r4.s64 = ctx.r11.s64 + 28808;
	// addi r3,r10,24492
	ctx.r3.s64 = ctx.r10.s64 + 24492;
	// bl 0x827d6c90
	ctx.lr = 0x82590CE8;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,28928
	ctx.r4.s64 = ctx.r11.s64 + 28928;
	// addi r3,r10,24464
	ctx.r3.s64 = ctx.r10.s64 + 24464;
	// bl 0x827d6c90
	ctx.lr = 0x82590CFC;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,29032
	ctx.r4.s64 = ctx.r11.s64 + 29032;
	// addi r3,r10,24424
	ctx.r3.s64 = ctx.r10.s64 + 24424;
	// bl 0x827d6c90
	ctx.lr = 0x82590D10;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,29184
	ctx.r4.s64 = ctx.r11.s64 + 29184;
	// addi r3,r10,24384
	ctx.r3.s64 = ctx.r10.s64 + 24384;
	// bl 0x827d6c90
	ctx.lr = 0x82590D24;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,29256
	ctx.r4.s64 = ctx.r11.s64 + 29256;
	// addi r3,r10,24360
	ctx.r3.s64 = ctx.r10.s64 + 24360;
	// bl 0x827d6c90
	ctx.lr = 0x82590D38;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,29328
	ctx.r4.s64 = ctx.r11.s64 + 29328;
	// addi r3,r10,24332
	ctx.r3.s64 = ctx.r10.s64 + 24332;
	// bl 0x827d6c90
	ctx.lr = 0x82590D4C;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,29392
	ctx.r4.s64 = ctx.r11.s64 + 29392;
	// addi r3,r10,24316
	ctx.r3.s64 = ctx.r10.s64 + 24316;
	// bl 0x827d6c90
	ctx.lr = 0x82590D60;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-1864
	ctx.r4.s64 = ctx.r11.s64 + -1864;
	// addi r3,r10,24292
	ctx.r3.s64 = ctx.r10.s64 + 24292;
	// bl 0x827d6c90
	ctx.lr = 0x82590D74;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,29456
	ctx.r4.s64 = ctx.r11.s64 + 29456;
	// addi r3,r10,24272
	ctx.r3.s64 = ctx.r10.s64 + 24272;
	// bl 0x827d6c90
	ctx.lr = 0x82590D88;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,29584
	ctx.r4.s64 = ctx.r11.s64 + 29584;
	// addi r3,r10,24248
	ctx.r3.s64 = ctx.r10.s64 + 24248;
	// bl 0x827d6c90
	ctx.lr = 0x82590D9C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-1848
	ctx.r4.s64 = ctx.r11.s64 + -1848;
	// addi r3,r10,24220
	ctx.r3.s64 = ctx.r10.s64 + 24220;
	// bl 0x827d6c90
	ctx.lr = 0x82590DB0;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,29656
	ctx.r4.s64 = ctx.r11.s64 + 29656;
	// addi r3,r10,24200
	ctx.r3.s64 = ctx.r10.s64 + 24200;
	// bl 0x827d6c90
	ctx.lr = 0x82590DC4;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,29776
	ctx.r4.s64 = ctx.r11.s64 + 29776;
	// addi r3,r10,24180
	ctx.r3.s64 = ctx.r10.s64 + 24180;
	// bl 0x827d6c90
	ctx.lr = 0x82590DD8;
	sub_827D6C90(ctx, base);
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// addi r3,r10,24156
	ctx.r3.s64 = ctx.r10.s64 + 24156;
	// addi r4,r11,29896
	ctx.r4.s64 = ctx.r11.s64 + 29896;
	// bl 0x827d6c90
	ctx.lr = 0x82590DEC;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,30016
	ctx.r4.s64 = ctx.r11.s64 + 30016;
	// addi r3,r10,24136
	ctx.r3.s64 = ctx.r10.s64 + 24136;
	// bl 0x827d6c90
	ctx.lr = 0x82590E00;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-1768
	ctx.r4.s64 = ctx.r11.s64 + -1768;
	// addi r3,r10,24096
	ctx.r3.s64 = ctx.r10.s64 + 24096;
	// bl 0x827d6c90
	ctx.lr = 0x82590E14;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,30080
	ctx.r4.s64 = ctx.r11.s64 + 30080;
	// addi r3,r10,24068
	ctx.r3.s64 = ctx.r10.s64 + 24068;
	// bl 0x827d6c90
	ctx.lr = 0x82590E28;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,30208
	ctx.r4.s64 = ctx.r11.s64 + 30208;
	// addi r3,r10,24040
	ctx.r3.s64 = ctx.r10.s64 + 24040;
	// bl 0x827d6c90
	ctx.lr = 0x82590E3C;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,30336
	ctx.r4.s64 = ctx.r11.s64 + 30336;
	// addi r3,r10,24016
	ctx.r3.s64 = ctx.r10.s64 + 24016;
	// bl 0x827d6c90
	ctx.lr = 0x82590E50;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,30432
	ctx.r4.s64 = ctx.r11.s64 + 30432;
	// addi r3,r10,23988
	ctx.r3.s64 = ctx.r10.s64 + 23988;
	// bl 0x827d6c90
	ctx.lr = 0x82590E64;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,30560
	ctx.r4.s64 = ctx.r11.s64 + 30560;
	// addi r3,r10,23952
	ctx.r3.s64 = ctx.r10.s64 + 23952;
	// bl 0x827d6c90
	ctx.lr = 0x82590E78;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-1728
	ctx.r4.s64 = ctx.r11.s64 + -1728;
	// addi r3,r10,23936
	ctx.r3.s64 = ctx.r10.s64 + 23936;
	// bl 0x827d6c90
	ctx.lr = 0x82590E8C;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,30688
	ctx.r4.s64 = ctx.r11.s64 + 30688;
	// addi r3,r10,23900
	ctx.r3.s64 = ctx.r10.s64 + 23900;
	// bl 0x827d6c90
	ctx.lr = 0x82590EA0;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,30792
	ctx.r4.s64 = ctx.r11.s64 + 30792;
	// addi r3,r10,23868
	ctx.r3.s64 = ctx.r10.s64 + 23868;
	// bl 0x827d6c90
	ctx.lr = 0x82590EB4;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,30872
	ctx.r4.s64 = ctx.r11.s64 + 30872;
	// addi r3,r10,23836
	ctx.r3.s64 = ctx.r10.s64 + 23836;
	// bl 0x827d6c90
	ctx.lr = 0x82590EC8;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,30936
	ctx.r4.s64 = ctx.r11.s64 + 30936;
	// addi r3,r10,23800
	ctx.r3.s64 = ctx.r10.s64 + 23800;
	// bl 0x827d6c90
	ctx.lr = 0x82590EDC;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,31064
	ctx.r4.s64 = ctx.r11.s64 + 31064;
	// addi r3,r10,23772
	ctx.r3.s64 = ctx.r10.s64 + 23772;
	// bl 0x827d6c90
	ctx.lr = 0x82590EF0;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,31136
	ctx.r4.s64 = ctx.r11.s64 + 31136;
	// addi r3,r10,23740
	ctx.r3.s64 = ctx.r10.s64 + 23740;
	// bl 0x827d6c90
	ctx.lr = 0x82590F04;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,31264
	ctx.r4.s64 = ctx.r11.s64 + 31264;
	// addi r3,r10,23708
	ctx.r3.s64 = ctx.r10.s64 + 23708;
	// bl 0x827d6c90
	ctx.lr = 0x82590F18;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-1632
	ctx.r4.s64 = ctx.r11.s64 + -1632;
	// addi r3,r10,23680
	ctx.r3.s64 = ctx.r10.s64 + 23680;
	// bl 0x827d6c90
	ctx.lr = 0x82590F2C;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,31344
	ctx.r4.s64 = ctx.r11.s64 + 31344;
	// addi r3,r10,23652
	ctx.r3.s64 = ctx.r10.s64 + 23652;
	// bl 0x827d6c90
	ctx.lr = 0x82590F40;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,31424
	ctx.r4.s64 = ctx.r11.s64 + 31424;
	// addi r3,r10,23624
	ctx.r3.s64 = ctx.r10.s64 + 23624;
	// bl 0x827d6c90
	ctx.lr = 0x82590F54;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,31552
	ctx.r4.s64 = ctx.r11.s64 + 31552;
	// addi r3,r10,23592
	ctx.r3.s64 = ctx.r10.s64 + 23592;
	// bl 0x827d6c90
	ctx.lr = 0x82590F68;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,31608
	ctx.r4.s64 = ctx.r11.s64 + 31608;
	// addi r3,r10,23576
	ctx.r3.s64 = ctx.r10.s64 + 23576;
	// bl 0x827d6c90
	ctx.lr = 0x82590F7C;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,31704
	ctx.r4.s64 = ctx.r11.s64 + 31704;
	// addi r3,r10,23528
	ctx.r3.s64 = ctx.r10.s64 + 23528;
	// bl 0x827d6c90
	ctx.lr = 0x82590F90;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,31736
	ctx.r4.s64 = ctx.r11.s64 + 31736;
	// addi r3,r10,23492
	ctx.r3.s64 = ctx.r10.s64 + 23492;
	// bl 0x827d6c90
	ctx.lr = 0x82590FA4;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,31824
	ctx.r4.s64 = ctx.r11.s64 + 31824;
	// addi r3,r10,23472
	ctx.r3.s64 = ctx.r10.s64 + 23472;
	// bl 0x827d6c90
	ctx.lr = 0x82590FB8;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,31856
	ctx.r4.s64 = ctx.r11.s64 + 31856;
	// addi r3,r10,23452
	ctx.r3.s64 = ctx.r10.s64 + 23452;
	// bl 0x827d6c90
	ctx.lr = 0x82590FCC;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,31952
	ctx.r4.s64 = ctx.r11.s64 + 31952;
	// addi r3,r10,23428
	ctx.r3.s64 = ctx.r10.s64 + 23428;
	// bl 0x827d6c90
	ctx.lr = 0x82590FE0;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,32032
	ctx.r4.s64 = ctx.r11.s64 + 32032;
	// addi r3,r10,23412
	ctx.r3.s64 = ctx.r10.s64 + 23412;
	// bl 0x827d6c90
	ctx.lr = 0x82590FF4;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,32168
	ctx.r4.s64 = ctx.r11.s64 + 32168;
	// addi r3,r10,23384
	ctx.r3.s64 = ctx.r10.s64 + 23384;
	// bl 0x827d6c90
	ctx.lr = 0x82591008;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,32288
	ctx.r4.s64 = ctx.r11.s64 + 32288;
	// addi r3,r10,23356
	ctx.r3.s64 = ctx.r10.s64 + 23356;
	// bl 0x827d6c90
	ctx.lr = 0x8259101C;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,32304
	ctx.r4.s64 = ctx.r11.s64 + 32304;
	// addi r3,r10,23340
	ctx.r3.s64 = ctx.r10.s64 + 23340;
	// bl 0x827d6c90
	ctx.lr = 0x82591030;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,32368
	ctx.r4.s64 = ctx.r11.s64 + 32368;
	// addi r3,r10,23324
	ctx.r3.s64 = ctx.r10.s64 + 23324;
	// bl 0x827d6c90
	ctx.lr = 0x82591044;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-1560
	ctx.r4.s64 = ctx.r11.s64 + -1560;
	// addi r3,r10,23300
	ctx.r3.s64 = ctx.r10.s64 + 23300;
	// bl 0x827d6c90
	ctx.lr = 0x82591058;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-1488
	ctx.r4.s64 = ctx.r11.s64 + -1488;
	// addi r3,r10,23276
	ctx.r3.s64 = ctx.r10.s64 + 23276;
	// bl 0x827d6c90
	ctx.lr = 0x8259106C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-1416
	ctx.r4.s64 = ctx.r11.s64 + -1416;
	// addi r3,r10,23248
	ctx.r3.s64 = ctx.r10.s64 + 23248;
	// bl 0x827d6c90
	ctx.lr = 0x82591080;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-1376
	ctx.r4.s64 = ctx.r11.s64 + -1376;
	// addi r3,r10,23220
	ctx.r3.s64 = ctx.r10.s64 + 23220;
	// bl 0x827d6c90
	ctx.lr = 0x82591094;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-1352
	ctx.r4.s64 = ctx.r11.s64 + -1352;
	// addi r3,r10,23192
	ctx.r3.s64 = ctx.r10.s64 + 23192;
	// bl 0x827d6c90
	ctx.lr = 0x825910A8;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,32384
	ctx.r4.s64 = ctx.r11.s64 + 32384;
	// addi r3,r10,23172
	ctx.r3.s64 = ctx.r10.s64 + 23172;
	// bl 0x827d6c90
	ctx.lr = 0x825910BC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-1216
	ctx.r4.s64 = ctx.r11.s64 + -1216;
	// addi r3,r10,23144
	ctx.r3.s64 = ctx.r10.s64 + 23144;
	// bl 0x827d6c90
	ctx.lr = 0x825910D0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-1192
	ctx.r4.s64 = ctx.r11.s64 + -1192;
	// addi r3,r10,23120
	ctx.r3.s64 = ctx.r10.s64 + 23120;
	// bl 0x827d6c90
	ctx.lr = 0x825910E4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-1112
	ctx.r4.s64 = ctx.r11.s64 + -1112;
	// addi r3,r10,23092
	ctx.r3.s64 = ctx.r10.s64 + 23092;
	// bl 0x827d6c90
	ctx.lr = 0x825910F8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-1032
	ctx.r4.s64 = ctx.r11.s64 + -1032;
	// addi r3,r10,23076
	ctx.r3.s64 = ctx.r10.s64 + 23076;
	// bl 0x827d6c90
	ctx.lr = 0x8259110C;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,32512
	ctx.r4.s64 = ctx.r11.s64 + 32512;
	// addi r3,r10,23056
	ctx.r3.s64 = ctx.r10.s64 + 23056;
	// bl 0x827d6c90
	ctx.lr = 0x82591120;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,32528
	ctx.r4.s64 = ctx.r11.s64 + 32528;
	// addi r3,r10,23036
	ctx.r3.s64 = ctx.r10.s64 + 23036;
	// bl 0x827d6c90
	ctx.lr = 0x82591134;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,32544
	ctx.r4.s64 = ctx.r11.s64 + 32544;
	// addi r3,r10,23020
	ctx.r3.s64 = ctx.r10.s64 + 23020;
	// bl 0x827d6c90
	ctx.lr = 0x82591148;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,32560
	ctx.r4.s64 = ctx.r11.s64 + 32560;
	// addi r3,r10,22996
	ctx.r3.s64 = ctx.r10.s64 + 22996;
	// bl 0x827d6c90
	ctx.lr = 0x8259115C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-992
	ctx.r4.s64 = ctx.r11.s64 + -992;
	// addi r3,r10,22980
	ctx.r3.s64 = ctx.r10.s64 + 22980;
	// bl 0x827d6c90
	ctx.lr = 0x82591170;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-888
	ctx.r4.s64 = ctx.r11.s64 + -888;
	// addi r3,r10,22964
	ctx.r3.s64 = ctx.r10.s64 + 22964;
	// bl 0x827d6c90
	ctx.lr = 0x82591184;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,32576
	ctx.r4.s64 = ctx.r11.s64 + 32576;
	// addi r3,r10,22940
	ctx.r3.s64 = ctx.r10.s64 + 22940;
	// bl 0x827d6c90
	ctx.lr = 0x82591198;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,32592
	ctx.r4.s64 = ctx.r11.s64 + 32592;
	// addi r3,r10,22924
	ctx.r3.s64 = ctx.r10.s64 + 22924;
	// bl 0x827d6c90
	ctx.lr = 0x825911AC;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,32664
	ctx.r4.s64 = ctx.r11.s64 + 32664;
	// addi r3,r10,22908
	ctx.r3.s64 = ctx.r10.s64 + 22908;
	// bl 0x827d6c90
	ctx.lr = 0x825911C0;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,32736
	ctx.r4.s64 = ctx.r11.s64 + 32736;
	// addi r3,r10,22880
	ctx.r3.s64 = ctx.r10.s64 + 22880;
	// bl 0x827d6c90
	ctx.lr = 0x825911D4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-32688
	ctx.r4.s64 = ctx.r11.s64 + -32688;
	// addi r3,r10,22856
	ctx.r3.s64 = ctx.r10.s64 + 22856;
	// bl 0x827d6c90
	ctx.lr = 0x825911E8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-5744
	ctx.r4.s64 = ctx.r11.s64 + -5744;
	// addi r3,r10,22812
	ctx.r3.s64 = ctx.r10.s64 + 22812;
	// bl 0x827d6c90
	ctx.lr = 0x825911FC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-5648
	ctx.r4.s64 = ctx.r11.s64 + -5648;
	// addi r3,r10,22772
	ctx.r3.s64 = ctx.r10.s64 + 22772;
	// bl 0x827d6c90
	ctx.lr = 0x82591210;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-5744
	ctx.r4.s64 = ctx.r11.s64 + -5744;
	// addi r3,r10,22728
	ctx.r3.s64 = ctx.r10.s64 + 22728;
	// bl 0x827d6c90
	ctx.lr = 0x82591224;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-5648
	ctx.r4.s64 = ctx.r11.s64 + -5648;
	// addi r3,r10,22684
	ctx.r3.s64 = ctx.r10.s64 + 22684;
	// bl 0x827d6c90
	ctx.lr = 0x82591238;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-872
	ctx.r4.s64 = ctx.r11.s64 + -872;
	// addi r3,r10,22656
	ctx.r3.s64 = ctx.r10.s64 + 22656;
	// bl 0x827d6c90
	ctx.lr = 0x8259124C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-856
	ctx.r4.s64 = ctx.r11.s64 + -856;
	// addi r3,r10,22628
	ctx.r3.s64 = ctx.r10.s64 + 22628;
	// bl 0x827d6c90
	ctx.lr = 0x82591260;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-840
	ctx.r4.s64 = ctx.r11.s64 + -840;
	// addi r3,r10,22596
	ctx.r3.s64 = ctx.r10.s64 + 22596;
	// bl 0x827d6c90
	ctx.lr = 0x82591274;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-824
	ctx.r4.s64 = ctx.r11.s64 + -824;
	// addi r3,r10,22560
	ctx.r3.s64 = ctx.r10.s64 + 22560;
	// bl 0x827d6c90
	ctx.lr = 0x82591288;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-808
	ctx.r4.s64 = ctx.r11.s64 + -808;
	// addi r3,r10,22532
	ctx.r3.s64 = ctx.r10.s64 + 22532;
	// bl 0x827d6c90
	ctx.lr = 0x8259129C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-792
	ctx.r4.s64 = ctx.r11.s64 + -792;
	// addi r3,r10,22504
	ctx.r3.s64 = ctx.r10.s64 + 22504;
	// bl 0x827d6c90
	ctx.lr = 0x825912B0;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,22476
	ctx.r3.s64 = ctx.r10.s64 + 22476;
	// bl 0x827d6c90
	ctx.lr = 0x825912C4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-32624
	ctx.r4.s64 = ctx.r11.s64 + -32624;
	// addi r3,r10,22452
	ctx.r3.s64 = ctx.r10.s64 + 22452;
	// bl 0x827d6c90
	ctx.lr = 0x825912D8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-32608
	ctx.r4.s64 = ctx.r11.s64 + -32608;
	// addi r3,r10,22416
	ctx.r3.s64 = ctx.r10.s64 + 22416;
	// bl 0x827d6c90
	ctx.lr = 0x825912EC;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// lis r11,-32253
	ctx.r11.s64 = -2113732608;
	// addi r3,r11,22388
	ctx.r3.s64 = ctx.r11.s64 + 22388;
	// bl 0x827d6c90
	ctx.lr = 0x82591300;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-32528
	ctx.r4.s64 = ctx.r11.s64 + -32528;
	// addi r3,r10,22360
	ctx.r3.s64 = ctx.r10.s64 + 22360;
	// bl 0x827d6c90
	ctx.lr = 0x82591314;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-32512
	ctx.r4.s64 = ctx.r11.s64 + -32512;
	// addi r3,r10,22328
	ctx.r3.s64 = ctx.r10.s64 + 22328;
	// bl 0x827d6c90
	ctx.lr = 0x82591328;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-32496
	ctx.r4.s64 = ctx.r11.s64 + -32496;
	// addi r3,r10,22296
	ctx.r3.s64 = ctx.r10.s64 + 22296;
	// bl 0x827d6c90
	ctx.lr = 0x8259133C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-5632
	ctx.r4.s64 = ctx.r11.s64 + -5632;
	// addi r3,r10,22260
	ctx.r3.s64 = ctx.r10.s64 + 22260;
	// bl 0x827d6c90
	ctx.lr = 0x82591350;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-5528
	ctx.r4.s64 = ctx.r11.s64 + -5528;
	// addi r3,r10,22208
	ctx.r3.s64 = ctx.r10.s64 + 22208;
	// bl 0x827d6c90
	ctx.lr = 0x82591364;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-5424
	ctx.r4.s64 = ctx.r11.s64 + -5424;
	// addi r3,r10,22160
	ctx.r3.s64 = ctx.r10.s64 + 22160;
	// bl 0x827d6c90
	ctx.lr = 0x82591378;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-5312
	ctx.r4.s64 = ctx.r11.s64 + -5312;
	// addi r3,r10,22120
	ctx.r3.s64 = ctx.r10.s64 + 22120;
	// bl 0x827d6c90
	ctx.lr = 0x8259138C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-5208
	ctx.r4.s64 = ctx.r11.s64 + -5208;
	// addi r3,r10,22080
	ctx.r3.s64 = ctx.r10.s64 + 22080;
	// bl 0x827d6c90
	ctx.lr = 0x825913A0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-5104
	ctx.r4.s64 = ctx.r11.s64 + -5104;
	// addi r3,r10,22040
	ctx.r3.s64 = ctx.r10.s64 + 22040;
	// bl 0x827d6c90
	ctx.lr = 0x825913B4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-5008
	ctx.r4.s64 = ctx.r11.s64 + -5008;
	// addi r3,r10,21996
	ctx.r3.s64 = ctx.r10.s64 + 21996;
	// bl 0x827d6c90
	ctx.lr = 0x825913C8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-4904
	ctx.r4.s64 = ctx.r11.s64 + -4904;
	// addi r3,r10,21952
	ctx.r3.s64 = ctx.r10.s64 + 21952;
	// bl 0x827d6c90
	ctx.lr = 0x825913DC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-4800
	ctx.r4.s64 = ctx.r11.s64 + -4800;
	// addi r3,r10,21904
	ctx.r3.s64 = ctx.r10.s64 + 21904;
	// bl 0x827d6c90
	ctx.lr = 0x825913F0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-4696
	ctx.r4.s64 = ctx.r11.s64 + -4696;
	// addi r3,r10,21852
	ctx.r3.s64 = ctx.r10.s64 + 21852;
	// bl 0x827d6c90
	ctx.lr = 0x82591404;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-4592
	ctx.r4.s64 = ctx.r11.s64 + -4592;
	// addi r3,r10,21812
	ctx.r3.s64 = ctx.r10.s64 + 21812;
	// bl 0x827d6c90
	ctx.lr = 0x82591418;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-4488
	ctx.r4.s64 = ctx.r11.s64 + -4488;
	// addi r3,r10,21756
	ctx.r3.s64 = ctx.r10.s64 + 21756;
	// bl 0x827d6c90
	ctx.lr = 0x8259142C;
	sub_827D6C90(ctx, base);
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// addi r3,r10,21708
	ctx.r3.s64 = ctx.r10.s64 + 21708;
	// addi r4,r11,-4384
	ctx.r4.s64 = ctx.r11.s64 + -4384;
	// bl 0x827d6c90
	ctx.lr = 0x82591440;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-4288
	ctx.r4.s64 = ctx.r11.s64 + -4288;
	// addi r3,r10,21664
	ctx.r3.s64 = ctx.r10.s64 + 21664;
	// bl 0x827d6c90
	ctx.lr = 0x82591454;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-32480
	ctx.r4.s64 = ctx.r11.s64 + -32480;
	// addi r3,r10,21644
	ctx.r3.s64 = ctx.r10.s64 + 21644;
	// bl 0x827d6c90
	ctx.lr = 0x82591468;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-32416
	ctx.r4.s64 = ctx.r11.s64 + -32416;
	// addi r3,r10,21620
	ctx.r3.s64 = ctx.r10.s64 + 21620;
	// bl 0x827d6c90
	ctx.lr = 0x8259147C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-32352
	ctx.r4.s64 = ctx.r11.s64 + -32352;
	// addi r3,r10,21604
	ctx.r3.s64 = ctx.r10.s64 + 21604;
	// bl 0x827d6c90
	ctx.lr = 0x82591490;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-32216
	ctx.r4.s64 = ctx.r11.s64 + -32216;
	// addi r3,r10,21584
	ctx.r3.s64 = ctx.r10.s64 + 21584;
	// bl 0x827d6c90
	ctx.lr = 0x825914A4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-32080
	ctx.r4.s64 = ctx.r11.s64 + -32080;
	// addi r3,r10,21564
	ctx.r3.s64 = ctx.r10.s64 + 21564;
	// bl 0x827d6c90
	ctx.lr = 0x825914B8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-32016
	ctx.r4.s64 = ctx.r11.s64 + -32016;
	// addi r3,r10,21548
	ctx.r3.s64 = ctx.r10.s64 + 21548;
	// bl 0x827d6c90
	ctx.lr = 0x825914CC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-31952
	ctx.r4.s64 = ctx.r11.s64 + -31952;
	// addi r3,r10,21532
	ctx.r3.s64 = ctx.r10.s64 + 21532;
	// bl 0x827d6c90
	ctx.lr = 0x825914E0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-31888
	ctx.r4.s64 = ctx.r11.s64 + -31888;
	// addi r3,r10,21512
	ctx.r3.s64 = ctx.r10.s64 + 21512;
	// bl 0x827d6c90
	ctx.lr = 0x825914F4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-31768
	ctx.r4.s64 = ctx.r11.s64 + -31768;
	// addi r3,r10,21488
	ctx.r3.s64 = ctx.r10.s64 + 21488;
	// bl 0x827d6c90
	ctx.lr = 0x82591508;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,21456
	ctx.r3.s64 = ctx.r10.s64 + 21456;
	// bl 0x827d6c90
	ctx.lr = 0x8259151C;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,21424
	ctx.r3.s64 = ctx.r10.s64 + 21424;
	// bl 0x827d6c90
	ctx.lr = 0x82591530;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-4208
	ctx.r4.s64 = ctx.r11.s64 + -4208;
	// addi r3,r10,21400
	ctx.r3.s64 = ctx.r10.s64 + 21400;
	// bl 0x827d6c90
	ctx.lr = 0x82591544;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-4208
	ctx.r4.s64 = ctx.r11.s64 + -4208;
	// addi r3,r10,21376
	ctx.r3.s64 = ctx.r10.s64 + 21376;
	// bl 0x827d6c90
	ctx.lr = 0x82591558;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-4192
	ctx.r4.s64 = ctx.r11.s64 + -4192;
	// addi r3,r10,21348
	ctx.r3.s64 = ctx.r10.s64 + 21348;
	// bl 0x827d6c90
	ctx.lr = 0x8259156C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-31720
	ctx.r4.s64 = ctx.r11.s64 + -31720;
	// addi r3,r10,21320
	ctx.r3.s64 = ctx.r10.s64 + 21320;
	// bl 0x827d6c90
	ctx.lr = 0x82591580;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-31664
	ctx.r4.s64 = ctx.r11.s64 + -31664;
	// addi r3,r10,21296
	ctx.r3.s64 = ctx.r10.s64 + 21296;
	// bl 0x827d6c90
	ctx.lr = 0x82591594;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-31648
	ctx.r4.s64 = ctx.r11.s64 + -31648;
	// addi r3,r10,21272
	ctx.r3.s64 = ctx.r10.s64 + 21272;
	// bl 0x827d6c90
	ctx.lr = 0x825915A8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-31608
	ctx.r4.s64 = ctx.r11.s64 + -31608;
	// addi r3,r10,21244
	ctx.r3.s64 = ctx.r10.s64 + 21244;
	// bl 0x827d6c90
	ctx.lr = 0x825915BC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-31512
	ctx.r4.s64 = ctx.r11.s64 + -31512;
	// addi r3,r10,21220
	ctx.r3.s64 = ctx.r10.s64 + 21220;
	// bl 0x827d6c90
	ctx.lr = 0x825915D0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-31408
	ctx.r4.s64 = ctx.r11.s64 + -31408;
	// addi r3,r10,21192
	ctx.r3.s64 = ctx.r10.s64 + 21192;
	// bl 0x827d6c90
	ctx.lr = 0x825915E4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-31336
	ctx.r4.s64 = ctx.r11.s64 + -31336;
	// addi r3,r10,21136
	ctx.r3.s64 = ctx.r10.s64 + 21136;
	// bl 0x827d6c90
	ctx.lr = 0x825915F8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-31304
	ctx.r4.s64 = ctx.r11.s64 + -31304;
	// addi r3,r10,21116
	ctx.r3.s64 = ctx.r10.s64 + 21116;
	// bl 0x827d6c90
	ctx.lr = 0x8259160C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-31224
	ctx.r4.s64 = ctx.r11.s64 + -31224;
	// addi r3,r10,21096
	ctx.r3.s64 = ctx.r10.s64 + 21096;
	// bl 0x827d6c90
	ctx.lr = 0x82591620;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-31144
	ctx.r4.s64 = ctx.r11.s64 + -31144;
	// addi r3,r10,21072
	ctx.r3.s64 = ctx.r10.s64 + 21072;
	// bl 0x827d6c90
	ctx.lr = 0x82591634;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-31064
	ctx.r4.s64 = ctx.r11.s64 + -31064;
	// addi r3,r10,21048
	ctx.r3.s64 = ctx.r10.s64 + 21048;
	// bl 0x827d6c90
	ctx.lr = 0x82591648;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-30984
	ctx.r4.s64 = ctx.r11.s64 + -30984;
	// addi r3,r10,21016
	ctx.r3.s64 = ctx.r10.s64 + 21016;
	// bl 0x827d6c90
	ctx.lr = 0x8259165C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-30912
	ctx.r4.s64 = ctx.r11.s64 + -30912;
	// addi r3,r10,20984
	ctx.r3.s64 = ctx.r10.s64 + 20984;
	// bl 0x827d6c90
	ctx.lr = 0x82591670;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-5744
	ctx.r4.s64 = ctx.r11.s64 + -5744;
	// addi r3,r10,20940
	ctx.r3.s64 = ctx.r10.s64 + 20940;
	// bl 0x827d6c90
	ctx.lr = 0x82591684;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-5648
	ctx.r4.s64 = ctx.r11.s64 + -5648;
	// addi r3,r10,20900
	ctx.r3.s64 = ctx.r10.s64 + 20900;
	// bl 0x827d6c90
	ctx.lr = 0x82591698;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-30784
	ctx.r4.s64 = ctx.r11.s64 + -30784;
	// addi r3,r10,20868
	ctx.r3.s64 = ctx.r10.s64 + 20868;
	// bl 0x827d6c90
	ctx.lr = 0x825916AC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-30680
	ctx.r4.s64 = ctx.r11.s64 + -30680;
	// addi r3,r10,20840
	ctx.r3.s64 = ctx.r10.s64 + 20840;
	// bl 0x827d6c90
	ctx.lr = 0x825916C0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-30552
	ctx.r4.s64 = ctx.r11.s64 + -30552;
	// addi r3,r10,20808
	ctx.r3.s64 = ctx.r10.s64 + 20808;
	// bl 0x827d6c90
	ctx.lr = 0x825916D4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-30424
	ctx.r4.s64 = ctx.r11.s64 + -30424;
	// addi r3,r10,20780
	ctx.r3.s64 = ctx.r10.s64 + 20780;
	// bl 0x827d6c90
	ctx.lr = 0x825916E8;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,20744
	ctx.r3.s64 = ctx.r10.s64 + 20744;
	// bl 0x827d6c90
	ctx.lr = 0x825916FC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-30296
	ctx.r4.s64 = ctx.r11.s64 + -30296;
	// addi r3,r10,20712
	ctx.r3.s64 = ctx.r10.s64 + 20712;
	// bl 0x827d6c90
	ctx.lr = 0x82591710;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-30168
	ctx.r4.s64 = ctx.r11.s64 + -30168;
	// addi r3,r10,20692
	ctx.r3.s64 = ctx.r10.s64 + 20692;
	// bl 0x827d6c90
	ctx.lr = 0x82591724;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-30088
	ctx.r4.s64 = ctx.r11.s64 + -30088;
	// addi r3,r10,20672
	ctx.r3.s64 = ctx.r10.s64 + 20672;
	// bl 0x827d6c90
	ctx.lr = 0x82591738;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-776
	ctx.r4.s64 = ctx.r11.s64 + -776;
	// addi r3,r10,20656
	ctx.r3.s64 = ctx.r10.s64 + 20656;
	// bl 0x827d6c90
	ctx.lr = 0x8259174C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-30016
	ctx.r4.s64 = ctx.r11.s64 + -30016;
	// addi r3,r10,20636
	ctx.r3.s64 = ctx.r10.s64 + 20636;
	// bl 0x827d6c90
	ctx.lr = 0x82591760;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-752
	ctx.r4.s64 = ctx.r11.s64 + -752;
	// addi r3,r10,20616
	ctx.r3.s64 = ctx.r10.s64 + 20616;
	// bl 0x827d6c90
	ctx.lr = 0x82591774;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-29920
	ctx.r4.s64 = ctx.r11.s64 + -29920;
	// addi r3,r10,20600
	ctx.r3.s64 = ctx.r10.s64 + 20600;
	// bl 0x827d6c90
	ctx.lr = 0x82591788;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-736
	ctx.r4.s64 = ctx.r11.s64 + -736;
	// addi r3,r10,20580
	ctx.r3.s64 = ctx.r10.s64 + 20580;
	// bl 0x827d6c90
	ctx.lr = 0x8259179C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-29792
	ctx.r4.s64 = ctx.r11.s64 + -29792;
	// addi r3,r10,20560
	ctx.r3.s64 = ctx.r10.s64 + 20560;
	// bl 0x827d6c90
	ctx.lr = 0x825917B0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-720
	ctx.r4.s64 = ctx.r11.s64 + -720;
	// addi r3,r10,20532
	ctx.r3.s64 = ctx.r10.s64 + 20532;
	// bl 0x827d6c90
	ctx.lr = 0x825917C4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-29680
	ctx.r4.s64 = ctx.r11.s64 + -29680;
	// addi r3,r10,20504
	ctx.r3.s64 = ctx.r10.s64 + 20504;
	// bl 0x827d6c90
	ctx.lr = 0x825917D8;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,20476
	ctx.r3.s64 = ctx.r10.s64 + 20476;
	// bl 0x827d6c90
	ctx.lr = 0x825917EC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-616
	ctx.r4.s64 = ctx.r11.s64 + -616;
	// addi r3,r10,20444
	ctx.r3.s64 = ctx.r10.s64 + 20444;
	// bl 0x827d6c90
	ctx.lr = 0x82591800;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-520
	ctx.r4.s64 = ctx.r11.s64 + -520;
	// addi r3,r10,20420
	ctx.r3.s64 = ctx.r10.s64 + 20420;
	// bl 0x827d6c90
	ctx.lr = 0x82591814;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-29568
	ctx.r4.s64 = ctx.r11.s64 + -29568;
	// addi r3,r10,20404
	ctx.r3.s64 = ctx.r10.s64 + 20404;
	// bl 0x827d6c90
	ctx.lr = 0x82591828;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-504
	ctx.r4.s64 = ctx.r11.s64 + -504;
	// addi r3,r10,20372
	ctx.r3.s64 = ctx.r10.s64 + 20372;
	// bl 0x827d6c90
	ctx.lr = 0x8259183C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-29504
	ctx.r4.s64 = ctx.r11.s64 + -29504;
	// addi r3,r10,20352
	ctx.r3.s64 = ctx.r10.s64 + 20352;
	// bl 0x827d6c90
	ctx.lr = 0x82591850;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-29472
	ctx.r4.s64 = ctx.r11.s64 + -29472;
	// addi r3,r10,20332
	ctx.r3.s64 = ctx.r10.s64 + 20332;
	// bl 0x827d6c90
	ctx.lr = 0x82591864;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-29376
	ctx.r4.s64 = ctx.r11.s64 + -29376;
	// addi r3,r10,20312
	ctx.r3.s64 = ctx.r10.s64 + 20312;
	// bl 0x827d6c90
	ctx.lr = 0x82591878;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-488
	ctx.r4.s64 = ctx.r11.s64 + -488;
	// addi r3,r10,20292
	ctx.r3.s64 = ctx.r10.s64 + 20292;
	// bl 0x827d6c90
	ctx.lr = 0x8259188C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-4136
	ctx.r4.s64 = ctx.r11.s64 + -4136;
	// addi r3,r10,20272
	ctx.r3.s64 = ctx.r10.s64 + 20272;
	// bl 0x827d6c90
	ctx.lr = 0x825918A0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-4112
	ctx.r4.s64 = ctx.r11.s64 + -4112;
	// addi r3,r10,20260
	ctx.r3.s64 = ctx.r10.s64 + 20260;
	// bl 0x827d6c90
	ctx.lr = 0x825918B4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-29248
	ctx.r4.s64 = ctx.r11.s64 + -29248;
	// addi r3,r10,20232
	ctx.r3.s64 = ctx.r10.s64 + 20232;
	// bl 0x827d6c90
	ctx.lr = 0x825918C8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-29120
	ctx.r4.s64 = ctx.r11.s64 + -29120;
	// addi r3,r10,20204
	ctx.r3.s64 = ctx.r10.s64 + 20204;
	// bl 0x827d6c90
	ctx.lr = 0x825918DC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-464
	ctx.r4.s64 = ctx.r11.s64 + -464;
	// addi r3,r10,20176
	ctx.r3.s64 = ctx.r10.s64 + 20176;
	// bl 0x827d6c90
	ctx.lr = 0x825918F0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-4208
	ctx.r4.s64 = ctx.r11.s64 + -4208;
	// addi r3,r10,20136
	ctx.r3.s64 = ctx.r10.s64 + 20136;
	// bl 0x827d6c90
	ctx.lr = 0x82591904;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,20116
	ctx.r3.s64 = ctx.r10.s64 + 20116;
	// bl 0x827d6c90
	ctx.lr = 0x82591918;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-440
	ctx.r4.s64 = ctx.r11.s64 + -440;
	// addi r3,r10,20100
	ctx.r3.s64 = ctx.r10.s64 + 20100;
	// bl 0x827d6c90
	ctx.lr = 0x8259192C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-28992
	ctx.r4.s64 = ctx.r11.s64 + -28992;
	// addi r3,r10,20080
	ctx.r3.s64 = ctx.r10.s64 + 20080;
	// bl 0x827d6c90
	ctx.lr = 0x82591940;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// addi r4,r11,-28896
	ctx.r4.s64 = ctx.r11.s64 + -28896;
	// lis r11,-32253
	ctx.r11.s64 = -2113732608;
	// addi r3,r11,20052
	ctx.r3.s64 = ctx.r11.s64 + 20052;
	// bl 0x827d6c90
	ctx.lr = 0x82591954;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-28880
	ctx.r4.s64 = ctx.r11.s64 + -28880;
	// addi r3,r10,20020
	ctx.r3.s64 = ctx.r10.s64 + 20020;
	// bl 0x827d6c90
	ctx.lr = 0x82591968;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-28864
	ctx.r4.s64 = ctx.r11.s64 + -28864;
	// addi r3,r10,19984
	ctx.r3.s64 = ctx.r10.s64 + 19984;
	// bl 0x827d6c90
	ctx.lr = 0x8259197C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-28768
	ctx.r4.s64 = ctx.r11.s64 + -28768;
	// addi r3,r10,19948
	ctx.r3.s64 = ctx.r10.s64 + 19948;
	// bl 0x827d6c90
	ctx.lr = 0x82591990;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-28656
	ctx.r4.s64 = ctx.r11.s64 + -28656;
	// addi r3,r10,19912
	ctx.r3.s64 = ctx.r10.s64 + 19912;
	// bl 0x827d6c90
	ctx.lr = 0x825919A4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-28584
	ctx.r4.s64 = ctx.r11.s64 + -28584;
	// addi r3,r10,19892
	ctx.r3.s64 = ctx.r10.s64 + 19892;
	// bl 0x827d6c90
	ctx.lr = 0x825919B8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-28568
	ctx.r4.s64 = ctx.r11.s64 + -28568;
	// addi r3,r10,19856
	ctx.r3.s64 = ctx.r10.s64 + 19856;
	// bl 0x827d6c90
	ctx.lr = 0x825919CC;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,19828
	ctx.r3.s64 = ctx.r10.s64 + 19828;
	// bl 0x827d6c90
	ctx.lr = 0x825919E0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-344
	ctx.r4.s64 = ctx.r11.s64 + -344;
	// addi r3,r10,19808
	ctx.r3.s64 = ctx.r10.s64 + 19808;
	// bl 0x827d6c90
	ctx.lr = 0x825919F4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-320
	ctx.r4.s64 = ctx.r11.s64 + -320;
	// addi r3,r10,19784
	ctx.r3.s64 = ctx.r10.s64 + 19784;
	// bl 0x827d6c90
	ctx.lr = 0x82591A08;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-296
	ctx.r4.s64 = ctx.r11.s64 + -296;
	// addi r3,r10,19764
	ctx.r3.s64 = ctx.r10.s64 + 19764;
	// bl 0x827d6c90
	ctx.lr = 0x82591A1C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-28480
	ctx.r4.s64 = ctx.r11.s64 + -28480;
	// addi r3,r10,19736
	ctx.r3.s64 = ctx.r10.s64 + 19736;
	// bl 0x827d6c90
	ctx.lr = 0x82591A30;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-28360
	ctx.r4.s64 = ctx.r11.s64 + -28360;
	// addi r3,r10,19704
	ctx.r3.s64 = ctx.r10.s64 + 19704;
	// bl 0x827d6c90
	ctx.lr = 0x82591A44;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-28280
	ctx.r4.s64 = ctx.r11.s64 + -28280;
	// addi r3,r10,19680
	ctx.r3.s64 = ctx.r10.s64 + 19680;
	// bl 0x827d6c90
	ctx.lr = 0x82591A58;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-28168
	ctx.r4.s64 = ctx.r11.s64 + -28168;
	// addi r3,r10,19636
	ctx.r3.s64 = ctx.r10.s64 + 19636;
	// bl 0x827d6c90
	ctx.lr = 0x82591A6C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-28048
	ctx.r4.s64 = ctx.r11.s64 + -28048;
	// addi r3,r10,19612
	ctx.r3.s64 = ctx.r10.s64 + 19612;
	// bl 0x827d6c90
	ctx.lr = 0x82591A80;
	sub_827D6C90(ctx, base);
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// addi r3,r10,19592
	ctx.r3.s64 = ctx.r10.s64 + 19592;
	// addi r4,r11,-27944
	ctx.r4.s64 = ctx.r11.s64 + -27944;
	// bl 0x827d6c90
	ctx.lr = 0x82591A94;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-27824
	ctx.r4.s64 = ctx.r11.s64 + -27824;
	// addi r3,r10,19572
	ctx.r3.s64 = ctx.r10.s64 + 19572;
	// bl 0x827d6c90
	ctx.lr = 0x82591AA8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-280
	ctx.r4.s64 = ctx.r11.s64 + -280;
	// addi r3,r10,19552
	ctx.r3.s64 = ctx.r10.s64 + 19552;
	// bl 0x827d6c90
	ctx.lr = 0x82591ABC;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,19528
	ctx.r3.s64 = ctx.r10.s64 + 19528;
	// bl 0x827d6c90
	ctx.lr = 0x82591AD0;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,19504
	ctx.r3.s64 = ctx.r10.s64 + 19504;
	// bl 0x827d6c90
	ctx.lr = 0x82591AE4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-27680
	ctx.r4.s64 = ctx.r11.s64 + -27680;
	// addi r3,r10,19472
	ctx.r3.s64 = ctx.r10.s64 + 19472;
	// bl 0x827d6c90
	ctx.lr = 0x82591AF8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-264
	ctx.r4.s64 = ctx.r11.s64 + -264;
	// addi r3,r10,19440
	ctx.r3.s64 = ctx.r10.s64 + 19440;
	// bl 0x827d6c90
	ctx.lr = 0x82591B0C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-248
	ctx.r4.s64 = ctx.r11.s64 + -248;
	// addi r3,r10,19404
	ctx.r3.s64 = ctx.r10.s64 + 19404;
	// bl 0x827d6c90
	ctx.lr = 0x82591B20;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-27592
	ctx.r4.s64 = ctx.r11.s64 + -27592;
	// addi r3,r10,19376
	ctx.r3.s64 = ctx.r10.s64 + 19376;
	// bl 0x827d6c90
	ctx.lr = 0x82591B34;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-27472
	ctx.r4.s64 = ctx.r11.s64 + -27472;
	// addi r3,r10,19352
	ctx.r3.s64 = ctx.r10.s64 + 19352;
	// bl 0x827d6c90
	ctx.lr = 0x82591B48;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-27424
	ctx.r4.s64 = ctx.r11.s64 + -27424;
	// addi r3,r10,19328
	ctx.r3.s64 = ctx.r10.s64 + 19328;
	// bl 0x827d6c90
	ctx.lr = 0x82591B5C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-27368
	ctx.r4.s64 = ctx.r11.s64 + -27368;
	// addi r3,r10,19308
	ctx.r3.s64 = ctx.r10.s64 + 19308;
	// bl 0x827d6c90
	ctx.lr = 0x82591B70;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-27248
	ctx.r4.s64 = ctx.r11.s64 + -27248;
	// addi r3,r10,19284
	ctx.r3.s64 = ctx.r10.s64 + 19284;
	// bl 0x827d6c90
	ctx.lr = 0x82591B84;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-27136
	ctx.r4.s64 = ctx.r11.s64 + -27136;
	// addi r3,r10,19264
	ctx.r3.s64 = ctx.r10.s64 + 19264;
	// bl 0x827d6c90
	ctx.lr = 0x82591B98;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-27016
	ctx.r4.s64 = ctx.r11.s64 + -27016;
	// addi r3,r10,19236
	ctx.r3.s64 = ctx.r10.s64 + 19236;
	// bl 0x827d6c90
	ctx.lr = 0x82591BAC;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-4208
	ctx.r4.s64 = ctx.r11.s64 + -4208;
	// addi r3,r10,19212
	ctx.r3.s64 = ctx.r10.s64 + 19212;
	// bl 0x827d6c90
	ctx.lr = 0x82591BC0;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,19168
	ctx.r3.s64 = ctx.r10.s64 + 19168;
	// bl 0x827d6c90
	ctx.lr = 0x82591BD4;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,19120
	ctx.r3.s64 = ctx.r10.s64 + 19120;
	// bl 0x827d6c90
	ctx.lr = 0x82591BE8;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,19068
	ctx.r3.s64 = ctx.r10.s64 + 19068;
	// bl 0x827d6c90
	ctx.lr = 0x82591BFC;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,19024
	ctx.r3.s64 = ctx.r10.s64 + 19024;
	// bl 0x827d6c90
	ctx.lr = 0x82591C10;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-26896
	ctx.r4.s64 = ctx.r11.s64 + -26896;
	// addi r3,r10,19000
	ctx.r3.s64 = ctx.r10.s64 + 19000;
	// bl 0x827d6c90
	ctx.lr = 0x82591C24;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-26776
	ctx.r4.s64 = ctx.r11.s64 + -26776;
	// addi r3,r10,18972
	ctx.r3.s64 = ctx.r10.s64 + 18972;
	// bl 0x827d6c90
	ctx.lr = 0x82591C38;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-26656
	ctx.r4.s64 = ctx.r11.s64 + -26656;
	// addi r3,r10,18944
	ctx.r3.s64 = ctx.r10.s64 + 18944;
	// bl 0x827d6c90
	ctx.lr = 0x82591C4C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-26536
	ctx.r4.s64 = ctx.r11.s64 + -26536;
	// addi r3,r10,18896
	ctx.r3.s64 = ctx.r10.s64 + 18896;
	// bl 0x827d6c90
	ctx.lr = 0x82591C60;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-26448
	ctx.r4.s64 = ctx.r11.s64 + -26448;
	// addi r3,r10,18856
	ctx.r3.s64 = ctx.r10.s64 + 18856;
	// bl 0x827d6c90
	ctx.lr = 0x82591C74;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-26432
	ctx.r4.s64 = ctx.r11.s64 + -26432;
	// addi r3,r10,18828
	ctx.r3.s64 = ctx.r10.s64 + 18828;
	// bl 0x827d6c90
	ctx.lr = 0x82591C88;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-26368
	ctx.r4.s64 = ctx.r11.s64 + -26368;
	// addi r3,r10,18788
	ctx.r3.s64 = ctx.r10.s64 + 18788;
	// bl 0x827d6c90
	ctx.lr = 0x82591C9C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-232
	ctx.r4.s64 = ctx.r11.s64 + -232;
	// addi r3,r10,18764
	ctx.r3.s64 = ctx.r10.s64 + 18764;
	// bl 0x827d6c90
	ctx.lr = 0x82591CB0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-26304
	ctx.r4.s64 = ctx.r11.s64 + -26304;
	// addi r3,r10,18732
	ctx.r3.s64 = ctx.r10.s64 + 18732;
	// bl 0x827d6c90
	ctx.lr = 0x82591CC4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-4072
	ctx.r4.s64 = ctx.r11.s64 + -4072;
	// addi r3,r10,18704
	ctx.r3.s64 = ctx.r10.s64 + 18704;
	// bl 0x827d6c90
	ctx.lr = 0x82591CD8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-26128
	ctx.r4.s64 = ctx.r11.s64 + -26128;
	// addi r3,r10,18668
	ctx.r3.s64 = ctx.r10.s64 + 18668;
	// bl 0x827d6c90
	ctx.lr = 0x82591CEC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-26016
	ctx.r4.s64 = ctx.r11.s64 + -26016;
	// addi r3,r10,18620
	ctx.r3.s64 = ctx.r10.s64 + 18620;
	// bl 0x827d6c90
	ctx.lr = 0x82591D00;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-25904
	ctx.r4.s64 = ctx.r11.s64 + -25904;
	// addi r3,r10,18596
	ctx.r3.s64 = ctx.r10.s64 + 18596;
	// bl 0x827d6c90
	ctx.lr = 0x82591D14;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-25832
	ctx.r4.s64 = ctx.r11.s64 + -25832;
	// addi r3,r10,18580
	ctx.r3.s64 = ctx.r10.s64 + 18580;
	// bl 0x827d6c90
	ctx.lr = 0x82591D28;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-208
	ctx.r4.s64 = ctx.r11.s64 + -208;
	// addi r3,r10,18544
	ctx.r3.s64 = ctx.r10.s64 + 18544;
	// bl 0x827d6c90
	ctx.lr = 0x82591D3C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-192
	ctx.r4.s64 = ctx.r11.s64 + -192;
	// addi r3,r10,18520
	ctx.r3.s64 = ctx.r10.s64 + 18520;
	// bl 0x827d6c90
	ctx.lr = 0x82591D50;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-3976
	ctx.r4.s64 = ctx.r11.s64 + -3976;
	// addi r3,r10,18492
	ctx.r3.s64 = ctx.r10.s64 + 18492;
	// bl 0x827d6c90
	ctx.lr = 0x82591D64;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-25816
	ctx.r4.s64 = ctx.r11.s64 + -25816;
	// addi r3,r10,18444
	ctx.r3.s64 = ctx.r10.s64 + 18444;
	// bl 0x827d6c90
	ctx.lr = 0x82591D78;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-72
	ctx.r4.s64 = ctx.r11.s64 + -72;
	// addi r3,r10,18420
	ctx.r3.s64 = ctx.r10.s64 + 18420;
	// bl 0x827d6c90
	ctx.lr = 0x82591D8C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-25696
	ctx.r4.s64 = ctx.r11.s64 + -25696;
	// addi r3,r10,18404
	ctx.r3.s64 = ctx.r10.s64 + 18404;
	// bl 0x827d6c90
	ctx.lr = 0x82591DA0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-25672
	ctx.r4.s64 = ctx.r11.s64 + -25672;
	// addi r3,r10,18376
	ctx.r3.s64 = ctx.r10.s64 + 18376;
	// bl 0x827d6c90
	ctx.lr = 0x82591DB4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-25544
	ctx.r4.s64 = ctx.r11.s64 + -25544;
	// addi r3,r10,18336
	ctx.r3.s64 = ctx.r10.s64 + 18336;
	// bl 0x827d6c90
	ctx.lr = 0x82591DC8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-25480
	ctx.r4.s64 = ctx.r11.s64 + -25480;
	// addi r3,r10,18308
	ctx.r3.s64 = ctx.r10.s64 + 18308;
	// bl 0x827d6c90
	ctx.lr = 0x82591DDC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-25352
	ctx.r4.s64 = ctx.r11.s64 + -25352;
	// addi r3,r10,18268
	ctx.r3.s64 = ctx.r10.s64 + 18268;
	// bl 0x827d6c90
	ctx.lr = 0x82591DF0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-25280
	ctx.r4.s64 = ctx.r11.s64 + -25280;
	// addi r3,r10,18236
	ctx.r3.s64 = ctx.r10.s64 + 18236;
	// bl 0x827d6c90
	ctx.lr = 0x82591E04;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-25184
	ctx.r4.s64 = ctx.r11.s64 + -25184;
	// addi r3,r10,18200
	ctx.r3.s64 = ctx.r10.s64 + 18200;
	// bl 0x827d6c90
	ctx.lr = 0x82591E18;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-25096
	ctx.r4.s64 = ctx.r11.s64 + -25096;
	// addi r3,r10,18160
	ctx.r3.s64 = ctx.r10.s64 + 18160;
	// bl 0x827d6c90
	ctx.lr = 0x82591E2C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,48
	ctx.r4.s64 = ctx.r11.s64 + 48;
	// addi r3,r10,18140
	ctx.r3.s64 = ctx.r10.s64 + 18140;
	// bl 0x827d6c90
	ctx.lr = 0x82591E40;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-25016
	ctx.r4.s64 = ctx.r11.s64 + -25016;
	// addi r3,r10,18120
	ctx.r3.s64 = ctx.r10.s64 + 18120;
	// bl 0x827d6c90
	ctx.lr = 0x82591E54;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-24920
	ctx.r4.s64 = ctx.r11.s64 + -24920;
	// addi r3,r10,18104
	ctx.r3.s64 = ctx.r10.s64 + 18104;
	// bl 0x827d6c90
	ctx.lr = 0x82591E68;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-24840
	ctx.r4.s64 = ctx.r11.s64 + -24840;
	// addi r3,r10,18080
	ctx.r3.s64 = ctx.r10.s64 + 18080;
	// bl 0x827d6c90
	ctx.lr = 0x82591E7C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-24768
	ctx.r4.s64 = ctx.r11.s64 + -24768;
	// addi r3,r10,18040
	ctx.r3.s64 = ctx.r10.s64 + 18040;
	// bl 0x827d6c90
	ctx.lr = 0x82591E90;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-24648
	ctx.r4.s64 = ctx.r11.s64 + -24648;
	// addi r3,r10,18008
	ctx.r3.s64 = ctx.r10.s64 + 18008;
	// bl 0x827d6c90
	ctx.lr = 0x82591EA4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-24528
	ctx.r4.s64 = ctx.r11.s64 + -24528;
	// addi r3,r10,17992
	ctx.r3.s64 = ctx.r10.s64 + 17992;
	// bl 0x827d6c90
	ctx.lr = 0x82591EB8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-24400
	ctx.r4.s64 = ctx.r11.s64 + -24400;
	// addi r3,r10,17960
	ctx.r3.s64 = ctx.r10.s64 + 17960;
	// bl 0x827d6c90
	ctx.lr = 0x82591ECC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-24368
	ctx.r4.s64 = ctx.r11.s64 + -24368;
	// addi r3,r10,17924
	ctx.r3.s64 = ctx.r10.s64 + 17924;
	// bl 0x827d6c90
	ctx.lr = 0x82591EE0;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,17884
	ctx.r3.s64 = ctx.r10.s64 + 17884;
	// bl 0x827d6c90
	ctx.lr = 0x82591EF4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,168
	ctx.r4.s64 = ctx.r11.s64 + 168;
	// addi r3,r10,17856
	ctx.r3.s64 = ctx.r10.s64 + 17856;
	// bl 0x827d6c90
	ctx.lr = 0x82591F08;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-24280
	ctx.r4.s64 = ctx.r11.s64 + -24280;
	// addi r3,r10,17828
	ctx.r3.s64 = ctx.r10.s64 + 17828;
	// bl 0x827d6c90
	ctx.lr = 0x82591F1C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-24208
	ctx.r4.s64 = ctx.r11.s64 + -24208;
	// addi r3,r10,17808
	ctx.r3.s64 = ctx.r10.s64 + 17808;
	// bl 0x827d6c90
	ctx.lr = 0x82591F30;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,184
	ctx.r4.s64 = ctx.r11.s64 + 184;
	// addi r3,r10,17780
	ctx.r3.s64 = ctx.r10.s64 + 17780;
	// bl 0x827d6c90
	ctx.lr = 0x82591F44;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,200
	ctx.r4.s64 = ctx.r11.s64 + 200;
	// addi r3,r10,17756
	ctx.r3.s64 = ctx.r10.s64 + 17756;
	// bl 0x827d6c90
	ctx.lr = 0x82591F58;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,216
	ctx.r4.s64 = ctx.r11.s64 + 216;
	// addi r3,r10,17724
	ctx.r3.s64 = ctx.r10.s64 + 17724;
	// bl 0x827d6c90
	ctx.lr = 0x82591F6C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-24192
	ctx.r4.s64 = ctx.r11.s64 + -24192;
	// addi r3,r10,17692
	ctx.r3.s64 = ctx.r10.s64 + 17692;
	// bl 0x827d6c90
	ctx.lr = 0x82591F80;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-24120
	ctx.r4.s64 = ctx.r11.s64 + -24120;
	// addi r3,r10,17664
	ctx.r3.s64 = ctx.r10.s64 + 17664;
	// bl 0x827d6c90
	ctx.lr = 0x82591F94;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// addi r4,r11,296
	ctx.r4.s64 = ctx.r11.s64 + 296;
	// lis r11,-32253
	ctx.r11.s64 = -2113732608;
	// addi r3,r11,17632
	ctx.r3.s64 = ctx.r11.s64 + 17632;
	// bl 0x827d6c90
	ctx.lr = 0x82591FA8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,312
	ctx.r4.s64 = ctx.r11.s64 + 312;
	// addi r3,r10,17600
	ctx.r3.s64 = ctx.r10.s64 + 17600;
	// bl 0x827d6c90
	ctx.lr = 0x82591FBC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,328
	ctx.r4.s64 = ctx.r11.s64 + 328;
	// addi r3,r10,17560
	ctx.r3.s64 = ctx.r10.s64 + 17560;
	// bl 0x827d6c90
	ctx.lr = 0x82591FD0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,368
	ctx.r4.s64 = ctx.r11.s64 + 368;
	// addi r3,r10,17540
	ctx.r3.s64 = ctx.r10.s64 + 17540;
	// bl 0x827d6c90
	ctx.lr = 0x82591FE4;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-13504
	ctx.r4.s64 = ctx.r11.s64 + -13504;
	// addi r3,r10,17512
	ctx.r3.s64 = ctx.r10.s64 + 17512;
	// bl 0x827d6c90
	ctx.lr = 0x82591FF8;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-13464
	ctx.r4.s64 = ctx.r11.s64 + -13464;
	// addi r3,r10,17484
	ctx.r3.s64 = ctx.r10.s64 + 17484;
	// bl 0x827d6c90
	ctx.lr = 0x8259200C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-24104
	ctx.r4.s64 = ctx.r11.s64 + -24104;
	// addi r3,r10,17436
	ctx.r3.s64 = ctx.r10.s64 + 17436;
	// bl 0x827d6c90
	ctx.lr = 0x82592020;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-24048
	ctx.r4.s64 = ctx.r11.s64 + -24048;
	// addi r3,r10,17388
	ctx.r3.s64 = ctx.r10.s64 + 17388;
	// bl 0x827d6c90
	ctx.lr = 0x82592034;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-23992
	ctx.r4.s64 = ctx.r11.s64 + -23992;
	// addi r3,r10,17344
	ctx.r3.s64 = ctx.r10.s64 + 17344;
	// bl 0x827d6c90
	ctx.lr = 0x82592048;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-23936
	ctx.r4.s64 = ctx.r11.s64 + -23936;
	// addi r3,r10,17300
	ctx.r3.s64 = ctx.r10.s64 + 17300;
	// bl 0x827d6c90
	ctx.lr = 0x8259205C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-23880
	ctx.r4.s64 = ctx.r11.s64 + -23880;
	// addi r3,r10,17248
	ctx.r3.s64 = ctx.r10.s64 + 17248;
	// bl 0x827d6c90
	ctx.lr = 0x82592070;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-3912
	ctx.r4.s64 = ctx.r11.s64 + -3912;
	// addi r3,r10,17228
	ctx.r3.s64 = ctx.r10.s64 + 17228;
	// bl 0x827d6c90
	ctx.lr = 0x82592084;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-3896
	ctx.r4.s64 = ctx.r11.s64 + -3896;
	// addi r3,r10,17204
	ctx.r3.s64 = ctx.r10.s64 + 17204;
	// bl 0x827d6c90
	ctx.lr = 0x82592098;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-3880
	ctx.r4.s64 = ctx.r11.s64 + -3880;
	// addi r3,r10,17172
	ctx.r3.s64 = ctx.r10.s64 + 17172;
	// bl 0x827d6c90
	ctx.lr = 0x825920AC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-23832
	ctx.r4.s64 = ctx.r11.s64 + -23832;
	// addi r3,r10,17148
	ctx.r3.s64 = ctx.r10.s64 + 17148;
	// bl 0x827d6c90
	ctx.lr = 0x825920C0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-23800
	ctx.r4.s64 = ctx.r11.s64 + -23800;
	// addi r3,r10,17132
	ctx.r3.s64 = ctx.r10.s64 + 17132;
	// bl 0x827d6c90
	ctx.lr = 0x825920D4;
	sub_827D6C90(ctx, base);
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// addi r3,r10,17116
	ctx.r3.s64 = ctx.r10.s64 + 17116;
	// addi r4,r11,-23696
	ctx.r4.s64 = ctx.r11.s64 + -23696;
	// bl 0x827d6c90
	ctx.lr = 0x825920E8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-23664
	ctx.r4.s64 = ctx.r11.s64 + -23664;
	// addi r3,r10,17088
	ctx.r3.s64 = ctx.r10.s64 + 17088;
	// bl 0x827d6c90
	ctx.lr = 0x825920FC;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,17068
	ctx.r3.s64 = ctx.r10.s64 + 17068;
	// bl 0x827d6c90
	ctx.lr = 0x82592110;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,17044
	ctx.r3.s64 = ctx.r10.s64 + 17044;
	// bl 0x827d6c90
	ctx.lr = 0x82592124;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-23648
	ctx.r4.s64 = ctx.r11.s64 + -23648;
	// addi r3,r10,17020
	ctx.r3.s64 = ctx.r10.s64 + 17020;
	// bl 0x827d6c90
	ctx.lr = 0x82592138;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-23528
	ctx.r4.s64 = ctx.r11.s64 + -23528;
	// addi r3,r10,16984
	ctx.r3.s64 = ctx.r10.s64 + 16984;
	// bl 0x827d6c90
	ctx.lr = 0x8259214C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-23408
	ctx.r4.s64 = ctx.r11.s64 + -23408;
	// addi r3,r10,16956
	ctx.r3.s64 = ctx.r10.s64 + 16956;
	// bl 0x827d6c90
	ctx.lr = 0x82592160;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-23288
	ctx.r4.s64 = ctx.r11.s64 + -23288;
	// addi r3,r10,16920
	ctx.r3.s64 = ctx.r10.s64 + 16920;
	// bl 0x827d6c90
	ctx.lr = 0x82592174;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-23168
	ctx.r4.s64 = ctx.r11.s64 + -23168;
	// addi r3,r10,16880
	ctx.r3.s64 = ctx.r10.s64 + 16880;
	// bl 0x827d6c90
	ctx.lr = 0x82592188;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-23048
	ctx.r4.s64 = ctx.r11.s64 + -23048;
	// addi r3,r10,16840
	ctx.r3.s64 = ctx.r10.s64 + 16840;
	// bl 0x827d6c90
	ctx.lr = 0x8259219C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-22944
	ctx.r4.s64 = ctx.r11.s64 + -22944;
	// addi r3,r10,16808
	ctx.r3.s64 = ctx.r10.s64 + 16808;
	// bl 0x827d6c90
	ctx.lr = 0x825921B0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-3864
	ctx.r4.s64 = ctx.r11.s64 + -3864;
	// addi r3,r10,16776
	ctx.r3.s64 = ctx.r10.s64 + 16776;
	// bl 0x827d6c90
	ctx.lr = 0x825921C4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-22904
	ctx.r4.s64 = ctx.r11.s64 + -22904;
	// addi r3,r10,16736
	ctx.r3.s64 = ctx.r10.s64 + 16736;
	// bl 0x827d6c90
	ctx.lr = 0x825921D8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-22808
	ctx.r4.s64 = ctx.r11.s64 + -22808;
	// addi r3,r10,16696
	ctx.r3.s64 = ctx.r10.s64 + 16696;
	// bl 0x827d6c90
	ctx.lr = 0x825921EC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-22712
	ctx.r4.s64 = ctx.r11.s64 + -22712;
	// addi r3,r10,16668
	ctx.r3.s64 = ctx.r10.s64 + 16668;
	// bl 0x827d6c90
	ctx.lr = 0x82592200;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-22632
	ctx.r4.s64 = ctx.r11.s64 + -22632;
	// addi r3,r10,16636
	ctx.r3.s64 = ctx.r10.s64 + 16636;
	// bl 0x827d6c90
	ctx.lr = 0x82592214;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-22512
	ctx.r4.s64 = ctx.r11.s64 + -22512;
	// addi r3,r10,16616
	ctx.r3.s64 = ctx.r10.s64 + 16616;
	// bl 0x827d6c90
	ctx.lr = 0x82592228;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,16596
	ctx.r3.s64 = ctx.r10.s64 + 16596;
	// bl 0x827d6c90
	ctx.lr = 0x8259223C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,384
	ctx.r4.s64 = ctx.r11.s64 + 384;
	// addi r3,r10,16568
	ctx.r3.s64 = ctx.r10.s64 + 16568;
	// bl 0x827d6c90
	ctx.lr = 0x82592250;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,432
	ctx.r4.s64 = ctx.r11.s64 + 432;
	// addi r3,r10,16536
	ctx.r3.s64 = ctx.r10.s64 + 16536;
	// bl 0x827d6c90
	ctx.lr = 0x82592264;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-22432
	ctx.r4.s64 = ctx.r11.s64 + -22432;
	// addi r3,r10,16512
	ctx.r3.s64 = ctx.r10.s64 + 16512;
	// bl 0x827d6c90
	ctx.lr = 0x82592278;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-22320
	ctx.r4.s64 = ctx.r11.s64 + -22320;
	// addi r3,r10,16484
	ctx.r3.s64 = ctx.r10.s64 + 16484;
	// bl 0x827d6c90
	ctx.lr = 0x8259228C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-3784
	ctx.r4.s64 = ctx.r11.s64 + -3784;
	// addi r3,r10,16464
	ctx.r3.s64 = ctx.r10.s64 + 16464;
	// bl 0x827d6c90
	ctx.lr = 0x825922A0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-3768
	ctx.r4.s64 = ctx.r11.s64 + -3768;
	// addi r3,r10,16432
	ctx.r3.s64 = ctx.r10.s64 + 16432;
	// bl 0x827d6c90
	ctx.lr = 0x825922B4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-3752
	ctx.r4.s64 = ctx.r11.s64 + -3752;
	// addi r3,r10,16408
	ctx.r3.s64 = ctx.r10.s64 + 16408;
	// bl 0x827d6c90
	ctx.lr = 0x825922C8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-3736
	ctx.r4.s64 = ctx.r11.s64 + -3736;
	// addi r3,r10,16376
	ctx.r3.s64 = ctx.r10.s64 + 16376;
	// bl 0x827d6c90
	ctx.lr = 0x825922DC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-3720
	ctx.r4.s64 = ctx.r11.s64 + -3720;
	// addi r3,r10,16344
	ctx.r3.s64 = ctx.r10.s64 + 16344;
	// bl 0x827d6c90
	ctx.lr = 0x825922F0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-3704
	ctx.r4.s64 = ctx.r11.s64 + -3704;
	// addi r3,r10,16304
	ctx.r3.s64 = ctx.r10.s64 + 16304;
	// bl 0x827d6c90
	ctx.lr = 0x82592304;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-22200
	ctx.r4.s64 = ctx.r11.s64 + -22200;
	// addi r3,r10,16292
	ctx.r3.s64 = ctx.r10.s64 + 16292;
	// bl 0x827d6c90
	ctx.lr = 0x82592318;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-22200
	ctx.r4.s64 = ctx.r11.s64 + -22200;
	// addi r3,r10,16264
	ctx.r3.s64 = ctx.r10.s64 + 16264;
	// bl 0x827d6c90
	ctx.lr = 0x8259232C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-22168
	ctx.r4.s64 = ctx.r11.s64 + -22168;
	// addi r3,r10,16228
	ctx.r3.s64 = ctx.r10.s64 + 16228;
	// bl 0x827d6c90
	ctx.lr = 0x82592340;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-3688
	ctx.r4.s64 = ctx.r11.s64 + -3688;
	// addi r3,r10,16196
	ctx.r3.s64 = ctx.r10.s64 + 16196;
	// bl 0x827d6c90
	ctx.lr = 0x82592354;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-22080
	ctx.r4.s64 = ctx.r11.s64 + -22080;
	// addi r3,r10,16168
	ctx.r3.s64 = ctx.r10.s64 + 16168;
	// bl 0x827d6c90
	ctx.lr = 0x82592368;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-22024
	ctx.r4.s64 = ctx.r11.s64 + -22024;
	// addi r3,r10,16140
	ctx.r3.s64 = ctx.r10.s64 + 16140;
	// bl 0x827d6c90
	ctx.lr = 0x8259237C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-21960
	ctx.r4.s64 = ctx.r11.s64 + -21960;
	// addi r3,r10,16116
	ctx.r3.s64 = ctx.r10.s64 + 16116;
	// bl 0x827d6c90
	ctx.lr = 0x82592390;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-21864
	ctx.r4.s64 = ctx.r11.s64 + -21864;
	// addi r3,r10,16092
	ctx.r3.s64 = ctx.r10.s64 + 16092;
	// bl 0x827d6c90
	ctx.lr = 0x825923A4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-21760
	ctx.r4.s64 = ctx.r11.s64 + -21760;
	// addi r3,r10,16064
	ctx.r3.s64 = ctx.r10.s64 + 16064;
	// bl 0x827d6c90
	ctx.lr = 0x825923B8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-21664
	ctx.r4.s64 = ctx.r11.s64 + -21664;
	// addi r3,r10,16036
	ctx.r3.s64 = ctx.r10.s64 + 16036;
	// bl 0x827d6c90
	ctx.lr = 0x825923CC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,936
	ctx.r4.s64 = ctx.r11.s64 + 936;
	// addi r3,r10,16012
	ctx.r3.s64 = ctx.r10.s64 + 16012;
	// bl 0x827d6c90
	ctx.lr = 0x825923E0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-21584
	ctx.r4.s64 = ctx.r11.s64 + -21584;
	// addi r3,r10,15992
	ctx.r3.s64 = ctx.r10.s64 + 15992;
	// bl 0x827d6c90
	ctx.lr = 0x825923F4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-21568
	ctx.r4.s64 = ctx.r11.s64 + -21568;
	// addi r3,r10,15976
	ctx.r3.s64 = ctx.r10.s64 + 15976;
	// bl 0x827d6c90
	ctx.lr = 0x82592408;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-21552
	ctx.r4.s64 = ctx.r11.s64 + -21552;
	// addi r3,r10,15952
	ctx.r3.s64 = ctx.r10.s64 + 15952;
	// bl 0x827d6c90
	ctx.lr = 0x8259241C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-21496
	ctx.r4.s64 = ctx.r11.s64 + -21496;
	// addi r3,r10,15932
	ctx.r3.s64 = ctx.r10.s64 + 15932;
	// bl 0x827d6c90
	ctx.lr = 0x82592430;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-21440
	ctx.r4.s64 = ctx.r11.s64 + -21440;
	// addi r3,r10,15912
	ctx.r3.s64 = ctx.r10.s64 + 15912;
	// bl 0x827d6c90
	ctx.lr = 0x82592444;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-3608
	ctx.r4.s64 = ctx.r11.s64 + -3608;
	// addi r3,r10,15892
	ctx.r3.s64 = ctx.r10.s64 + 15892;
	// bl 0x827d6c90
	ctx.lr = 0x82592458;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-21368
	ctx.r4.s64 = ctx.r11.s64 + -21368;
	// addi r3,r10,15860
	ctx.r3.s64 = ctx.r10.s64 + 15860;
	// bl 0x827d6c90
	ctx.lr = 0x8259246C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,480
	ctx.r4.s64 = ctx.r11.s64 + 480;
	// addi r3,r10,15836
	ctx.r3.s64 = ctx.r10.s64 + 15836;
	// bl 0x827d6c90
	ctx.lr = 0x82592480;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,536
	ctx.r4.s64 = ctx.r11.s64 + 536;
	// addi r3,r10,15816
	ctx.r3.s64 = ctx.r10.s64 + 15816;
	// bl 0x827d6c90
	ctx.lr = 0x82592494;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,560
	ctx.r4.s64 = ctx.r11.s64 + 560;
	// addi r3,r10,15792
	ctx.r3.s64 = ctx.r10.s64 + 15792;
	// bl 0x827d6c90
	ctx.lr = 0x825924A8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-21344
	ctx.r4.s64 = ctx.r11.s64 + -21344;
	// addi r3,r10,15772
	ctx.r3.s64 = ctx.r10.s64 + 15772;
	// bl 0x827d6c90
	ctx.lr = 0x825924BC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-3544
	ctx.r4.s64 = ctx.r11.s64 + -3544;
	// addi r3,r10,15744
	ctx.r3.s64 = ctx.r10.s64 + 15744;
	// bl 0x827d6c90
	ctx.lr = 0x825924D0;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-13432
	ctx.r4.s64 = ctx.r11.s64 + -13432;
	// addi r3,r10,15712
	ctx.r3.s64 = ctx.r10.s64 + 15712;
	// bl 0x827d6c90
	ctx.lr = 0x825924E4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-3528
	ctx.r4.s64 = ctx.r11.s64 + -3528;
	// addi r3,r10,15676
	ctx.r3.s64 = ctx.r10.s64 + 15676;
	// bl 0x827d6c90
	ctx.lr = 0x825924F8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,648
	ctx.r4.s64 = ctx.r11.s64 + 648;
	// addi r3,r10,15652
	ctx.r3.s64 = ctx.r10.s64 + 15652;
	// bl 0x827d6c90
	ctx.lr = 0x8259250C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-21272
	ctx.r4.s64 = ctx.r11.s64 + -21272;
	// addi r3,r10,15624
	ctx.r3.s64 = ctx.r10.s64 + 15624;
	// bl 0x827d6c90
	ctx.lr = 0x82592520;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-21208
	ctx.r4.s64 = ctx.r11.s64 + -21208;
	// addi r3,r10,15572
	ctx.r3.s64 = ctx.r10.s64 + 15572;
	// bl 0x827d6c90
	ctx.lr = 0x82592534;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-21080
	ctx.r4.s64 = ctx.r11.s64 + -21080;
	// addi r3,r10,15552
	ctx.r3.s64 = ctx.r10.s64 + 15552;
	// bl 0x827d6c90
	ctx.lr = 0x82592548;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-20936
	ctx.r4.s64 = ctx.r11.s64 + -20936;
	// addi r3,r10,15532
	ctx.r3.s64 = ctx.r10.s64 + 15532;
	// bl 0x827d6c90
	ctx.lr = 0x8259255C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-20832
	ctx.r4.s64 = ctx.r11.s64 + -20832;
	// addi r3,r10,15504
	ctx.r3.s64 = ctx.r10.s64 + 15504;
	// bl 0x827d6c90
	ctx.lr = 0x82592570;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-20728
	ctx.r4.s64 = ctx.r11.s64 + -20728;
	// addi r3,r10,15480
	ctx.r3.s64 = ctx.r10.s64 + 15480;
	// bl 0x827d6c90
	ctx.lr = 0x82592584;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-20624
	ctx.r4.s64 = ctx.r11.s64 + -20624;
	// addi r3,r10,15452
	ctx.r3.s64 = ctx.r10.s64 + 15452;
	// bl 0x827d6c90
	ctx.lr = 0x82592598;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-20520
	ctx.r4.s64 = ctx.r11.s64 + -20520;
	// addi r3,r10,15428
	ctx.r3.s64 = ctx.r10.s64 + 15428;
	// bl 0x827d6c90
	ctx.lr = 0x825925AC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-20416
	ctx.r4.s64 = ctx.r11.s64 + -20416;
	// addi r3,r10,15396
	ctx.r3.s64 = ctx.r10.s64 + 15396;
	// bl 0x827d6c90
	ctx.lr = 0x825925C0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-20312
	ctx.r4.s64 = ctx.r11.s64 + -20312;
	// addi r3,r10,15372
	ctx.r3.s64 = ctx.r10.s64 + 15372;
	// bl 0x827d6c90
	ctx.lr = 0x825925D4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-20240
	ctx.r4.s64 = ctx.r11.s64 + -20240;
	// addi r3,r10,15348
	ctx.r3.s64 = ctx.r10.s64 + 15348;
	// bl 0x827d6c90
	ctx.lr = 0x825925E8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// addi r4,r11,-20168
	ctx.r4.s64 = ctx.r11.s64 + -20168;
	// lis r11,-32253
	ctx.r11.s64 = -2113732608;
	// addi r3,r11,15320
	ctx.r3.s64 = ctx.r11.s64 + 15320;
	// bl 0x827d6c90
	ctx.lr = 0x825925FC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-20096
	ctx.r4.s64 = ctx.r11.s64 + -20096;
	// addi r3,r10,15300
	ctx.r3.s64 = ctx.r10.s64 + 15300;
	// bl 0x827d6c90
	ctx.lr = 0x82592610;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-19992
	ctx.r4.s64 = ctx.r11.s64 + -19992;
	// addi r3,r10,15260
	ctx.r3.s64 = ctx.r10.s64 + 15260;
	// bl 0x827d6c90
	ctx.lr = 0x82592624;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-19888
	ctx.r4.s64 = ctx.r11.s64 + -19888;
	// addi r3,r10,15220
	ctx.r3.s64 = ctx.r10.s64 + 15220;
	// bl 0x827d6c90
	ctx.lr = 0x82592638;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-19784
	ctx.r4.s64 = ctx.r11.s64 + -19784;
	// addi r3,r10,15172
	ctx.r3.s64 = ctx.r10.s64 + 15172;
	// bl 0x827d6c90
	ctx.lr = 0x8259264C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-19696
	ctx.r4.s64 = ctx.r11.s64 + -19696;
	// addi r3,r10,15132
	ctx.r3.s64 = ctx.r10.s64 + 15132;
	// bl 0x827d6c90
	ctx.lr = 0x82592660;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-19592
	ctx.r4.s64 = ctx.r11.s64 + -19592;
	// addi r3,r10,15100
	ctx.r3.s64 = ctx.r10.s64 + 15100;
	// bl 0x827d6c90
	ctx.lr = 0x82592674;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-3432
	ctx.r4.s64 = ctx.r11.s64 + -3432;
	// addi r3,r10,15084
	ctx.r3.s64 = ctx.r10.s64 + 15084;
	// bl 0x827d6c90
	ctx.lr = 0x82592688;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-19488
	ctx.r4.s64 = ctx.r11.s64 + -19488;
	// addi r3,r10,15064
	ctx.r3.s64 = ctx.r10.s64 + 15064;
	// bl 0x827d6c90
	ctx.lr = 0x8259269C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-19424
	ctx.r4.s64 = ctx.r11.s64 + -19424;
	// addi r3,r10,15040
	ctx.r3.s64 = ctx.r10.s64 + 15040;
	// bl 0x827d6c90
	ctx.lr = 0x825926B0;
	sub_827D6C90(ctx, base);
	// lis r11,-32168
	ctx.r11.s64 = -2108162048;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-13416
	ctx.r4.s64 = ctx.r11.s64 + -13416;
	// addi r3,r10,15008
	ctx.r3.s64 = ctx.r10.s64 + 15008;
	// bl 0x827d6c90
	ctx.lr = 0x825926C4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-19360
	ctx.r4.s64 = ctx.r11.s64 + -19360;
	// addi r3,r10,14980
	ctx.r3.s64 = ctx.r10.s64 + 14980;
	// bl 0x827d6c90
	ctx.lr = 0x825926D8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-19256
	ctx.r4.s64 = ctx.r11.s64 + -19256;
	// addi r3,r10,14944
	ctx.r3.s64 = ctx.r10.s64 + 14944;
	// bl 0x827d6c90
	ctx.lr = 0x825926EC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-19224
	ctx.r4.s64 = ctx.r11.s64 + -19224;
	// addi r3,r10,14896
	ctx.r3.s64 = ctx.r10.s64 + 14896;
	// bl 0x827d6c90
	ctx.lr = 0x82592700;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-19120
	ctx.r4.s64 = ctx.r11.s64 + -19120;
	// addi r3,r10,14848
	ctx.r3.s64 = ctx.r10.s64 + 14848;
	// bl 0x827d6c90
	ctx.lr = 0x82592714;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-19016
	ctx.r4.s64 = ctx.r11.s64 + -19016;
	// addi r3,r10,14832
	ctx.r3.s64 = ctx.r10.s64 + 14832;
	// bl 0x827d6c90
	ctx.lr = 0x82592728;
	sub_827D6C90(ctx, base);
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// addi r3,r10,14804
	ctx.r3.s64 = ctx.r10.s64 + 14804;
	// addi r4,r11,-19000
	ctx.r4.s64 = ctx.r11.s64 + -19000;
	// bl 0x827d6c90
	ctx.lr = 0x8259273C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-3352
	ctx.r4.s64 = ctx.r11.s64 + -3352;
	// addi r3,r10,14784
	ctx.r3.s64 = ctx.r10.s64 + 14784;
	// bl 0x827d6c90
	ctx.lr = 0x82592750;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-18968
	ctx.r4.s64 = ctx.r11.s64 + -18968;
	// addi r3,r10,14752
	ctx.r3.s64 = ctx.r10.s64 + 14752;
	// bl 0x827d6c90
	ctx.lr = 0x82592764;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-18888
	ctx.r4.s64 = ctx.r11.s64 + -18888;
	// addi r3,r10,14732
	ctx.r3.s64 = ctx.r10.s64 + 14732;
	// bl 0x827d6c90
	ctx.lr = 0x82592778;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-18784
	ctx.r4.s64 = ctx.r11.s64 + -18784;
	// addi r3,r10,14708
	ctx.r3.s64 = ctx.r10.s64 + 14708;
	// bl 0x827d6c90
	ctx.lr = 0x8259278C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-18752
	ctx.r4.s64 = ctx.r11.s64 + -18752;
	// addi r3,r10,14684
	ctx.r3.s64 = ctx.r10.s64 + 14684;
	// bl 0x827d6c90
	ctx.lr = 0x825927A0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-18648
	ctx.r4.s64 = ctx.r11.s64 + -18648;
	// addi r3,r10,14640
	ctx.r3.s64 = ctx.r10.s64 + 14640;
	// bl 0x827d6c90
	ctx.lr = 0x825927B4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-18544
	ctx.r4.s64 = ctx.r11.s64 + -18544;
	// addi r3,r10,14616
	ctx.r3.s64 = ctx.r10.s64 + 14616;
	// bl 0x827d6c90
	ctx.lr = 0x825927C8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-18440
	ctx.r4.s64 = ctx.r11.s64 + -18440;
	// addi r3,r10,14592
	ctx.r3.s64 = ctx.r10.s64 + 14592;
	// bl 0x827d6c90
	ctx.lr = 0x825927DC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-18336
	ctx.r4.s64 = ctx.r11.s64 + -18336;
	// addi r3,r10,14564
	ctx.r3.s64 = ctx.r10.s64 + 14564;
	// bl 0x827d6c90
	ctx.lr = 0x825927F0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-18304
	ctx.r4.s64 = ctx.r11.s64 + -18304;
	// addi r3,r10,14516
	ctx.r3.s64 = ctx.r10.s64 + 14516;
	// bl 0x827d6c90
	ctx.lr = 0x82592804;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-18200
	ctx.r4.s64 = ctx.r11.s64 + -18200;
	// addi r3,r10,14468
	ctx.r3.s64 = ctx.r10.s64 + 14468;
	// bl 0x827d6c90
	ctx.lr = 0x82592818;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-18072
	ctx.r4.s64 = ctx.r11.s64 + -18072;
	// addi r3,r10,14444
	ctx.r3.s64 = ctx.r10.s64 + 14444;
	// bl 0x827d6c90
	ctx.lr = 0x8259282C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,720
	ctx.r4.s64 = ctx.r11.s64 + 720;
	// addi r3,r10,14416
	ctx.r3.s64 = ctx.r10.s64 + 14416;
	// bl 0x827d6c90
	ctx.lr = 0x82592840;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-17944
	ctx.r4.s64 = ctx.r11.s64 + -17944;
	// addi r3,r10,14380
	ctx.r3.s64 = ctx.r10.s64 + 14380;
	// bl 0x827d6c90
	ctx.lr = 0x82592854;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-17864
	ctx.r4.s64 = ctx.r11.s64 + -17864;
	// addi r3,r10,14344
	ctx.r3.s64 = ctx.r10.s64 + 14344;
	// bl 0x827d6c90
	ctx.lr = 0x82592868;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-17832
	ctx.r4.s64 = ctx.r11.s64 + -17832;
	// addi r3,r10,14328
	ctx.r3.s64 = ctx.r10.s64 + 14328;
	// bl 0x827d6c90
	ctx.lr = 0x8259287C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-17752
	ctx.r4.s64 = ctx.r11.s64 + -17752;
	// addi r3,r10,14308
	ctx.r3.s64 = ctx.r10.s64 + 14308;
	// bl 0x827d6c90
	ctx.lr = 0x82592890;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-17616
	ctx.r4.s64 = ctx.r11.s64 + -17616;
	// addi r3,r10,14280
	ctx.r3.s64 = ctx.r10.s64 + 14280;
	// bl 0x827d6c90
	ctx.lr = 0x825928A4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-17512
	ctx.r4.s64 = ctx.r11.s64 + -17512;
	// addi r3,r10,14244
	ctx.r3.s64 = ctx.r10.s64 + 14244;
	// bl 0x827d6c90
	ctx.lr = 0x825928B8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-17408
	ctx.r4.s64 = ctx.r11.s64 + -17408;
	// addi r3,r10,14216
	ctx.r3.s64 = ctx.r10.s64 + 14216;
	// bl 0x827d6c90
	ctx.lr = 0x825928CC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-3320
	ctx.r4.s64 = ctx.r11.s64 + -3320;
	// addi r3,r10,14192
	ctx.r3.s64 = ctx.r10.s64 + 14192;
	// bl 0x827d6c90
	ctx.lr = 0x825928E0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-17328
	ctx.r4.s64 = ctx.r11.s64 + -17328;
	// addi r3,r10,14156
	ctx.r3.s64 = ctx.r10.s64 + 14156;
	// bl 0x827d6c90
	ctx.lr = 0x825928F4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-17248
	ctx.r4.s64 = ctx.r11.s64 + -17248;
	// addi r3,r10,14116
	ctx.r3.s64 = ctx.r10.s64 + 14116;
	// bl 0x827d6c90
	ctx.lr = 0x82592908;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-17168
	ctx.r4.s64 = ctx.r11.s64 + -17168;
	// addi r3,r10,14084
	ctx.r3.s64 = ctx.r10.s64 + 14084;
	// bl 0x827d6c90
	ctx.lr = 0x8259291C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-17088
	ctx.r4.s64 = ctx.r11.s64 + -17088;
	// addi r3,r10,14068
	ctx.r3.s64 = ctx.r10.s64 + 14068;
	// bl 0x827d6c90
	ctx.lr = 0x82592930;
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

PPC_FUNC_IMPL(__imp__sub_82592940);  /* forward decl */
PPC_WEAK_FUNC(sub_82592940) { __imp__sub_82592940(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82592940) {
	PPC_FUNC_PROLOGUE();
	// cmpwi cr6,r3,4
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 4, ctx.xer);
	// blt cr6,0x82592954
	if (ctx.cr6.lt) goto loc_82592954;
	// lis r11,-32083
	ctx.r11.s64 = -2102591488;
	// addi r11,r11,-16304
	ctx.r11.s64 = ctx.r11.s64 + -16304;
	// b 0x82592984
	goto loc_82592984;
loc_82592954:
	// lis r11,-32094
	ctx.r11.s64 = -2103312384;
	// lwz r10,11524(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 11524);
	// cmpwi cr6,r10,-1
	ctx.cr6.compare<int32_t>(ctx.r10.s32, -1, ctx.xer);
	// beq cr6,0x82592978
	if (ctx.cr6.eq) goto loc_82592978;
	// lis r11,-32083
	ctx.r11.s64 = -2102591488;
	// mulli r10,r10,188
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(188));
	// addi r11,r11,-17056
	ctx.r11.s64 = ctx.r11.s64 + -17056;
	// add r11,r10,r11
	ctx.r11.u64 = ctx.r10.u64 + ctx.r11.u64;
	// b 0x8259297c
	goto loc_8259297C;
loc_82592978:
	// li r11,0
	ctx.r11.s64 = 0;
loc_8259297C:
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82592a94
	if (ctx.cr6.eq) goto loc_82592A94;
loc_82592984:
	// cmplwi cr6,r4,19
	ctx.cr6.compare<uint32_t>(ctx.r4.u32, 19, ctx.xer);
	// bgt cr6,0x82592a94
	if (ctx.cr6.gt) goto loc_82592A94;
	// lis r12,-32167
	ctx.r12.s64 = -2108096512;
	// addi r12,r12,10660
	ctx.r12.s64 = ctx.r12.s64 + 10660;
	// rlwinm r0,r4,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r4.u32) {
	case 0:
		goto loc_825929F4;
	case 1:
		goto loc_825929FC;
	case 2:
		goto loc_82592A04;
	case 3:
		goto loc_82592A0C;
	case 4:
		goto loc_82592A14;
	case 5:
		goto loc_82592A1C;
	case 6:
		goto loc_82592A24;
	case 7:
		goto loc_82592A2C;
	case 8:
		goto loc_82592A34;
	case 9:
		goto loc_82592A3C;
	case 10:
		goto loc_82592A44;
	case 11:
		goto loc_82592A4C;
	case 12:
		goto loc_82592A54;
	case 13:
		goto loc_82592A5C;
	case 14:
		goto loc_82592A64;
	case 15:
		goto loc_82592A6C;
	case 16:
		goto loc_82592A74;
	case 17:
		goto loc_82592A7C;
	case 18:
		goto loc_82592A84;
	case 19:
		goto loc_82592A8C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_825929F4:
	// lwz r3,4(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// blr 
	return;
loc_825929FC:
	// lwz r3,8(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// blr 
	return;
loc_82592A04:
	// lwz r3,12(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// blr 
	return;
loc_82592A0C:
	// lwz r3,16(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// blr 
	return;
loc_82592A14:
	// lwz r3,20(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// blr 
	return;
loc_82592A1C:
	// lwz r3,24(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// blr 
	return;
loc_82592A24:
	// lwz r3,32(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 32);
	// blr 
	return;
loc_82592A2C:
	// lwz r3,36(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	// blr 
	return;
loc_82592A34:
	// lwz r3,60(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 60);
	// blr 
	return;
loc_82592A3C:
	// lwz r3,64(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 64);
	// blr 
	return;
loc_82592A44:
	// lwz r3,68(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 68);
	// blr 
	return;
loc_82592A4C:
	// lwz r3,72(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 72);
	// blr 
	return;
loc_82592A54:
	// lwz r3,76(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 76);
	// blr 
	return;
loc_82592A5C:
	// lwz r3,80(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 80);
	// blr 
	return;
loc_82592A64:
	// lwz r3,44(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
	// blr 
	return;
loc_82592A6C:
	// lwz r3,48(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 48);
	// blr 
	return;
loc_82592A74:
	// lwz r3,56(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 56);
	// blr 
	return;
loc_82592A7C:
	// lwz r3,52(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 52);
	// blr 
	return;
loc_82592A84:
	// lwz r3,28(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	// blr 
	return;
loc_82592A8C:
	// lwz r3,40(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 40);
	// blr 
	return;
loc_82592A94:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82592AA0);  /* forward decl */
PPC_WEAK_FUNC(sub_82592AA0) { __imp__sub_82592AA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82592AA0) {
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
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// cmpwi cr6,r3,4
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 4, ctx.xer);
	// blt cr6,0x82592acc
	if (ctx.cr6.lt) goto loc_82592ACC;
	// lis r11,-32083
	ctx.r11.s64 = -2102591488;
	// addi r31,r11,-16304
	ctx.r31.s64 = ctx.r11.s64 + -16304;
	// b 0x82592afc
	goto loc_82592AFC;
loc_82592ACC:
	// lis r11,-32094
	ctx.r11.s64 = -2103312384;
	// lwz r10,11524(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 11524);
	// cmpwi cr6,r10,-1
	ctx.cr6.compare<int32_t>(ctx.r10.s32, -1, ctx.xer);
	// beq cr6,0x82592af0
	if (ctx.cr6.eq) goto loc_82592AF0;
	// lis r11,-32083
	ctx.r11.s64 = -2102591488;
	// mulli r10,r10,188
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(188));
	// addi r11,r11,-17056
	ctx.r11.s64 = ctx.r11.s64 + -17056;
	// add r31,r10,r11
	ctx.r31.u64 = ctx.r10.u64 + ctx.r11.u64;
	// b 0x82592af4
	goto loc_82592AF4;
loc_82592AF0:
	// li r31,0
	ctx.r31.s64 = 0;
loc_82592AF4:
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x82592e00
	if (ctx.cr6.eq) goto loc_82592E00;
loc_82592AFC:
	// cmpwi cr6,r3,4
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 4, ctx.xer);
	// blt cr6,0x82592b0c
	if (ctx.cr6.lt) goto loc_82592B0C;
	// bl 0x8219f938
	ctx.lr = 0x82592B08;
	sub_8219F938(ctx, base);
	// b 0x82592b10
	goto loc_82592B10;
loc_82592B0C:
	// bl 0x8219f808
	ctx.lr = 0x82592B10;
	sub_8219F808(ctx, base);
loc_82592B10:
	// addi r11,r30,-4
	ctx.r11.s64 = ctx.r30.s64 + -4;
	// cmplwi cr6,r11,15
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 15, ctx.xer);
	// bgt cr6,0x82592e00
	if (ctx.cr6.gt) goto loc_82592E00;
	// lis r12,-32167
	ctx.r12.s64 = -2108096512;
	// addi r12,r12,11060
	ctx.r12.s64 = ctx.r12.s64 + 11060;
	// rlwinm r0,r11,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = PPC_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r11.u32) {
	case 0:
		goto loc_82592B74;
	case 1:
		goto loc_82592BA8;
	case 2:
		goto loc_82592BD0;
	case 3:
		goto loc_82592BF8;
	case 4:
		goto loc_82592C20;
	case 5:
		goto loc_82592C48;
	case 6:
		goto loc_82592C70;
	case 7:
		goto loc_82592C98;
	case 8:
		goto loc_82592CC0;
	case 9:
		goto loc_82592CE8;
	case 10:
		goto loc_82592D10;
	case 11:
		goto loc_82592D38;
	case 12:
		goto loc_82592D60;
	case 13:
		goto loc_82592D88;
	case 14:
		goto loc_82592DB0;
	case 15:
		goto loc_82592DD8;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_82592B74:
	// lbz r11,3408(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 3408);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82592e00
	if (!ctx.cr6.eq) goto loc_82592E00;
	// lwz r11,20(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 20);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x82592b9c
	if (ctx.cr6.eq) goto loc_82592B9C;
	// lwz r11,100(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 100);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// beq cr6,0x82592ba0
	if (ctx.cr6.eq) goto loc_82592BA0;
loc_82592B9C:
	// li r11,0
	ctx.r11.s64 = 0;
loc_82592BA0:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82592e04
	goto loc_82592E04;
loc_82592BA8:
	// lwz r11,24(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 24);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x82592bc4
	if (ctx.cr6.eq) goto loc_82592BC4;
	// lwz r11,104(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 104);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// beq cr6,0x82592bc8
	if (ctx.cr6.eq) goto loc_82592BC8;
loc_82592BC4:
	// li r11,0
	ctx.r11.s64 = 0;
loc_82592BC8:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82592e04
	goto loc_82592E04;
loc_82592BD0:
	// lwz r11,32(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 32);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x82592bec
	if (ctx.cr6.eq) goto loc_82592BEC;
	// lwz r11,112(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 112);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// beq cr6,0x82592bf0
	if (ctx.cr6.eq) goto loc_82592BF0;
loc_82592BEC:
	// li r11,0
	ctx.r11.s64 = 0;
loc_82592BF0:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82592e04
	goto loc_82592E04;
loc_82592BF8:
	// lwz r11,36(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 36);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x82592c14
	if (ctx.cr6.eq) goto loc_82592C14;
	// lwz r11,116(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 116);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// beq cr6,0x82592c18
	if (ctx.cr6.eq) goto loc_82592C18;
loc_82592C14:
	// li r11,0
	ctx.r11.s64 = 0;
loc_82592C18:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82592e04
	goto loc_82592E04;
loc_82592C20:
	// lwz r11,60(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 60);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x82592c3c
	if (ctx.cr6.eq) goto loc_82592C3C;
	// lwz r11,140(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 140);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// beq cr6,0x82592c40
	if (ctx.cr6.eq) goto loc_82592C40;
loc_82592C3C:
	// li r11,0
	ctx.r11.s64 = 0;
loc_82592C40:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82592e04
	goto loc_82592E04;
loc_82592C48:
	// lwz r11,64(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 64);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x82592c64
	if (ctx.cr6.eq) goto loc_82592C64;
	// lwz r11,144(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 144);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// beq cr6,0x82592c68
	if (ctx.cr6.eq) goto loc_82592C68;
loc_82592C64:
	// li r11,0
	ctx.r11.s64 = 0;
loc_82592C68:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82592e04
	goto loc_82592E04;
loc_82592C70:
	// lwz r11,68(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 68);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x82592c8c
	if (ctx.cr6.eq) goto loc_82592C8C;
	// lwz r11,148(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 148);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// beq cr6,0x82592c90
	if (ctx.cr6.eq) goto loc_82592C90;
loc_82592C8C:
	// li r11,0
	ctx.r11.s64 = 0;
loc_82592C90:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82592e04
	goto loc_82592E04;
loc_82592C98:
	// lwz r11,72(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 72);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x82592cb4
	if (ctx.cr6.eq) goto loc_82592CB4;
	// lwz r11,152(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 152);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// beq cr6,0x82592cb8
	if (ctx.cr6.eq) goto loc_82592CB8;
loc_82592CB4:
	// li r11,0
	ctx.r11.s64 = 0;
loc_82592CB8:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82592e04
	goto loc_82592E04;
loc_82592CC0:
	// lwz r11,76(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 76);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x82592cdc
	if (ctx.cr6.eq) goto loc_82592CDC;
	// lwz r11,156(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 156);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// beq cr6,0x82592ce0
	if (ctx.cr6.eq) goto loc_82592CE0;
loc_82592CDC:
	// li r11,0
	ctx.r11.s64 = 0;
loc_82592CE0:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82592e04
	goto loc_82592E04;
loc_82592CE8:
	// lwz r11,80(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 80);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x82592d04
	if (ctx.cr6.eq) goto loc_82592D04;
	// lwz r11,160(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 160);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// beq cr6,0x82592d08
	if (ctx.cr6.eq) goto loc_82592D08;
loc_82592D04:
	// li r11,0
	ctx.r11.s64 = 0;
loc_82592D08:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82592e04
	goto loc_82592E04;
loc_82592D10:
	// lwz r11,44(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 44);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x82592d2c
	if (ctx.cr6.eq) goto loc_82592D2C;
	// lwz r11,124(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 124);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// beq cr6,0x82592d30
	if (ctx.cr6.eq) goto loc_82592D30;
loc_82592D2C:
	// li r11,0
	ctx.r11.s64 = 0;
loc_82592D30:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82592e04
	goto loc_82592E04;
loc_82592D38:
	// lwz r11,48(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 48);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x82592d54
	if (ctx.cr6.eq) goto loc_82592D54;
	// lwz r11,128(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 128);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// beq cr6,0x82592d58
	if (ctx.cr6.eq) goto loc_82592D58;
loc_82592D54:
	// li r11,0
	ctx.r11.s64 = 0;
loc_82592D58:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82592e04
	goto loc_82592E04;
loc_82592D60:
	// lwz r11,56(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 56);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x82592d7c
	if (ctx.cr6.eq) goto loc_82592D7C;
	// lwz r11,136(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 136);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// beq cr6,0x82592d80
	if (ctx.cr6.eq) goto loc_82592D80;
loc_82592D7C:
	// li r11,0
	ctx.r11.s64 = 0;
loc_82592D80:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82592e04
	goto loc_82592E04;
loc_82592D88:
	// lwz r11,52(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 52);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x82592da4
	if (ctx.cr6.eq) goto loc_82592DA4;
	// lwz r11,132(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 132);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// beq cr6,0x82592da8
	if (ctx.cr6.eq) goto loc_82592DA8;
loc_82592DA4:
	// li r11,0
	ctx.r11.s64 = 0;
loc_82592DA8:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82592e04
	goto loc_82592E04;
loc_82592DB0:
	// lwz r11,28(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 28);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x82592dcc
	if (ctx.cr6.eq) goto loc_82592DCC;
	// lwz r11,108(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 108);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// beq cr6,0x82592dd0
	if (ctx.cr6.eq) goto loc_82592DD0;
loc_82592DCC:
	// li r11,0
	ctx.r11.s64 = 0;
loc_82592DD0:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82592e04
	goto loc_82592E04;
loc_82592DD8:
	// lwz r11,40(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 40);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x82592df4
	if (ctx.cr6.eq) goto loc_82592DF4;
	// lwz r11,120(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 120);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// beq cr6,0x82592df8
	if (ctx.cr6.eq) goto loc_82592DF8;
loc_82592DF4:
	// li r11,0
	ctx.r11.s64 = 0;
loc_82592DF8:
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82592e04
	goto loc_82592E04;
loc_82592E00:
	// li r3,0
	ctx.r3.s64 = 0;
loc_82592E04:
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

PPC_FUNC_IMPL(__imp__sub_82592E20);  /* forward decl */
PPC_WEAK_FUNC(sub_82592E20) { __imp__sub_82592E20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82592E20) {
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
	// bl 0x8259aed8
	ctx.lr = 0x82592E40;
	sub_8259AED8(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82592e64
	if (ctx.cr6.eq) goto loc_82592E64;
	// bl 0x82671538
	ctx.lr = 0x82592E50;
	sub_82671538(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82592e64
	if (ctx.cr6.eq) goto loc_82592E64;
	// li r3,0
	ctx.r3.s64 = 0;
	// b 0x82592e84
	goto loc_82592E84;
loc_82592E64:
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// li r9,0
	ctx.r9.s64 = 0;
	// bl 0x82592940
	ctx.lr = 0x82592E74;
	sub_82592940(ctx, base);
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// beq cr6,0x82592e80
	if (ctx.cr6.eq) goto loc_82592E80;
	// li r9,1
	ctx.r9.s64 = 1;
loc_82592E80:
	// mr r3,r9
	ctx.r3.u64 = ctx.r9.u64;
loc_82592E84:
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

PPC_FUNC_IMPL(__imp__sub_82592EA0);  /* forward decl */
PPC_WEAK_FUNC(sub_82592EA0) { __imp__sub_82592EA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82592EA0) {
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
	// bl 0x8259aed8
	ctx.lr = 0x82592EC0;
	sub_8259AED8(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82592ee4
	if (ctx.cr6.eq) goto loc_82592EE4;
	// bl 0x82671538
	ctx.lr = 0x82592ED0;
	sub_82671538(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82592ee4
	if (ctx.cr6.eq) goto loc_82592EE4;
	// li r3,0
	ctx.r3.s64 = 0;
	// b 0x82592f08
	goto loc_82592F08;
loc_82592EE4:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// li r31,0
	ctx.r31.s64 = 0;
	// bl 0x82592aa0
	ctx.lr = 0x82592EF4;
	sub_82592AA0(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82592f04
	if (ctx.cr6.eq) goto loc_82592F04;
	// li r31,1
	ctx.r31.s64 = 1;
loc_82592F04:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
loc_82592F08:
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

PPC_FUNC_IMPL(__imp__sub_82592F20);  /* forward decl */
PPC_WEAK_FUNC(sub_82592F20) { __imp__sub_82592F20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82592F20) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32083
	ctx.r10.s64 = -2102591488;
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-17071(r10)
	PPC_STORE_U8(ctx.r10.u32 + -17071, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82592F30);  /* forward decl */
PPC_WEAK_FUNC(sub_82592F30) { __imp__sub_82592F30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82592F30) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32083
	ctx.r10.s64 = -2102591488;
	// li r11,0
	ctx.r11.s64 = 0;
	// stb r11,-17071(r10)
	PPC_STORE_U8(ctx.r10.u32 + -17071, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82592F40);  /* forward decl */
PPC_WEAK_FUNC(sub_82592F40) { __imp__sub_82592F40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82592F40) {
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
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// bne cr6,0x82592f64
	if (!ctx.cr6.eq) goto loc_82592F64;
loc_82592F5C:
	// bl 0x8219f808
	ctx.lr = 0x82592F60;
	sub_8219F808(ctx, base);
	// b 0x82592f74
	goto loc_82592F74;
loc_82592F64:
	// cmpwi cr6,r3,1
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 1, ctx.xer);
	// beq cr6,0x82592f5c
	if (ctx.cr6.eq) goto loc_82592F5C;
	// li r3,1
	ctx.r3.s64 = 1;
	// bl 0x8219f8d8
	ctx.lr = 0x82592F74;
	sub_8219F8D8(ctx, base);
loc_82592F74:
	// addi r11,r31,194
	ctx.r11.s64 = ctx.r31.s64 + 194;
	// rlwinm r10,r11,1,0,30
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// add r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r11,r11,r3
	ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
	// lbz r10,2(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 2);
	// lbz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// xor r11,r10,r11
	ctx.r11.u64 = ctx.r10.u64 ^ ctx.r11.u64;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// subfic r11,r11,127
	ctx.xer.ca = ctx.r11.u32 <= 127;
	ctx.r11.u64 = static_cast<uint64_t>(127) - ctx.r11.u64;
	// subfe r11,r11,r11
	temp.u8 = (~ctx.r11.u32 + ctx.r11.u32 < ~ctx.r11.u32) | (~ctx.r11.u32 + ctx.r11.u32 + ctx.xer.ca < ctx.xer.ca);
	ctx.r11.u64 = ~ctx.r11.u64 + ctx.r11.u64 + ctx.xer.ca;
	ctx.xer.ca = temp.u8;
	// clrlwi r3,r11,31
	ctx.r3.u64 = ctx.r11.u32 & 0x1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82592FB8);  /* forward decl */
PPC_WEAK_FUNC(sub_82592FB8) { __imp__sub_82592FB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82592FB8) {
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
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// bne cr6,0x82592fdc
	if (!ctx.cr6.eq) goto loc_82592FDC;
	// bl 0x8219f808
	ctx.lr = 0x82592FD8;
	sub_8219F808(ctx, base);
	// b 0x82592ff4
	goto loc_82592FF4;
loc_82592FDC:
	// cmpwi cr6,r3,1
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 1, ctx.xer);
	// bne cr6,0x82592fec
	if (!ctx.cr6.eq) goto loc_82592FEC;
	// bl 0x8219f808
	ctx.lr = 0x82592FE8;
	sub_8219F808(ctx, base);
	// b 0x82592ff4
	goto loc_82592FF4;
loc_82592FEC:
	// li r3,1
	ctx.r3.s64 = 1;
	// bl 0x8219f8d8
	ctx.lr = 0x82592FF4;
	sub_8219F8D8(ctx, base);
loc_82592FF4:
	// addi r11,r31,194
	ctx.r11.s64 = ctx.r31.s64 + 194;
	// rlwinm r10,r11,1,0,30
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// add r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r11,r11,r3
	ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// lbz r9,2(r11)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 2);
	// xor r9,r9,r10
	ctx.r9.u64 = ctx.r9.u64 ^ ctx.r10.u64;
	// clrlwi r9,r9,24
	ctx.r9.u64 = ctx.r9.u32 & 0xFF;
	// subfic r9,r9,127
	ctx.xer.ca = ctx.r9.u32 <= 127;
	ctx.r9.u64 = static_cast<uint64_t>(127) - ctx.r9.u64;
	// subfe r9,r9,r9
	temp.u8 = (~ctx.r9.u32 + ctx.r9.u32 < ~ctx.r9.u32) | (~ctx.r9.u32 + ctx.r9.u32 + ctx.xer.ca < ctx.xer.ca);
	ctx.r9.u64 = ~ctx.r9.u64 + ctx.r9.u64 + ctx.xer.ca;
	ctx.xer.ca = temp.u8;
	// clrlwi r9,r9,31
	ctx.r9.u64 = ctx.r9.u32 & 0x1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// beq cr6,0x82593050
	if (ctx.cr6.eq) goto loc_82593050;
	// lbz r11,3(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 3);
	// xor r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 ^ ctx.r10.u64;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// subfic r11,r11,127
	ctx.xer.ca = ctx.r11.u32 <= 127;
	ctx.r11.u64 = static_cast<uint64_t>(127) - ctx.r11.u64;
	// subfe r11,r11,r11
	temp.u8 = (~ctx.r11.u32 + ctx.r11.u32 < ~ctx.r11.u32) | (~ctx.r11.u32 + ctx.r11.u32 + ctx.xer.ca < ctx.xer.ca);
	ctx.r11.u64 = ~ctx.r11.u64 + ctx.r11.u64 + ctx.xer.ca;
	ctx.xer.ca = temp.u8;
	// clrlwi r11,r11,31
	ctx.r11.u64 = ctx.r11.u32 & 0x1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// beq cr6,0x82593054
	if (ctx.cr6.eq) goto loc_82593054;
loc_82593050:
	// li r11,0
	ctx.r11.s64 = 0;
loc_82593054:
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
}

PPC_FUNC_IMPL(__imp__sub_82593070);  /* forward decl */
PPC_WEAK_FUNC(sub_82593070) { __imp__sub_82593070(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593070) {
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
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// lwz r11,-12344(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -12344);
	// cmpwi cr6,r11,-1
	ctx.cr6.compare<int32_t>(ctx.r11.s32, -1, ctx.xer);
	// bne cr6,0x82593094
	if (!ctx.cr6.eq) goto loc_82593094;
	// li r11,0
	ctx.r11.s64 = 0;
	// b 0x825930a4
	goto loc_825930A4;
loc_82593094:
	// lis r10,-32059
	ctx.r10.s64 = -2101018624;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r10,r10,7184
	ctx.r10.s64 = ctx.r10.s64 + 7184;
	// lwzx r11,r11,r10
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + ctx.r10.u32);
loc_825930A4:
	// lwz r11,1384(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 1384);
	// lwz r11,568(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 568);
	// rlwinm r11,r11,0,2,2
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x20000000;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82593118
	if (ctx.cr6.eq) goto loc_82593118;
	// bl 0x8219f808
	ctx.lr = 0x825930BC;
	sub_8219F808(ctx, base);
	// lbz r11,2784(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 2784);
	// lbz r10,2786(r3)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r3.u32 + 2786);
	// xor r10,r10,r11
	ctx.r10.u64 = ctx.r10.u64 ^ ctx.r11.u64;
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// subfic r10,r10,127
	ctx.xer.ca = ctx.r10.u32 <= 127;
	ctx.r10.u64 = static_cast<uint64_t>(127) - ctx.r10.u64;
	// subfe r10,r10,r10
	temp.u8 = (~ctx.r10.u32 + ctx.r10.u32 < ~ctx.r10.u32) | (~ctx.r10.u32 + ctx.r10.u32 + ctx.xer.ca < ctx.xer.ca);
	ctx.r10.u64 = ~ctx.r10.u64 + ctx.r10.u64 + ctx.xer.ca;
	ctx.xer.ca = temp.u8;
	// clrlwi r10,r10,31
	ctx.r10.u64 = ctx.r10.u32 & 0x1;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x82593104
	if (ctx.cr6.eq) goto loc_82593104;
	// lbz r10,2787(r3)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r3.u32 + 2787);
	// xor r11,r10,r11
	ctx.r11.u64 = ctx.r10.u64 ^ ctx.r11.u64;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// subfic r11,r11,127
	ctx.xer.ca = ctx.r11.u32 <= 127;
	ctx.r11.u64 = static_cast<uint64_t>(127) - ctx.r11.u64;
	// subfe r11,r11,r11
	temp.u8 = (~ctx.r11.u32 + ctx.r11.u32 < ~ctx.r11.u32) | (~ctx.r11.u32 + ctx.r11.u32 + ctx.xer.ca < ctx.xer.ca);
	ctx.r11.u64 = ~ctx.r11.u64 + ctx.r11.u64 + ctx.xer.ca;
	ctx.xer.ca = temp.u8;
	// clrlwi r11,r11,31
	ctx.r11.u64 = ctx.r11.u32 & 0x1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// beq cr6,0x82593108
	if (ctx.cr6.eq) goto loc_82593108;
loc_82593104:
	// li r11,0
	ctx.r11.s64 = 0;
loc_82593108:
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// li r3,1
	ctx.r3.s64 = 1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8259311c
	if (!ctx.cr6.eq) goto loc_8259311C;
loc_82593118:
	// li r3,0
	ctx.r3.s64 = 0;
loc_8259311C:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82593130);  /* forward decl */
PPC_WEAK_FUNC(sub_82593130) { __imp__sub_82593130(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593130) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x82592e20
	ctx.lr = 0x82593154;
	sub_82592E20(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82593178);  /* forward decl */
PPC_WEAK_FUNC(sub_82593178) { __imp__sub_82593178(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593178) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x82592ea0
	ctx.lr = 0x8259319C;
	sub_82592EA0(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825931C0);  /* forward decl */
PPC_WEAK_FUNC(sub_825931C0) { __imp__sub_825931C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825931C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x82592f40
	ctx.lr = 0x825931E4;
	sub_82592F40(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82593208);  /* forward decl */
PPC_WEAK_FUNC(sub_82593208) { __imp__sub_82593208(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593208) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x82592fb8
	ctx.lr = 0x8259322C;
	sub_82592FB8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82593250);  /* forward decl */
PPC_WEAK_FUNC(sub_82593250) { __imp__sub_82593250(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593250) {
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
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r30,4(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// bne cr6,0x82593284
	if (!ctx.cr6.eq) goto loc_82593284;
loc_8259327C:
	// bl 0x8219f808
	ctx.lr = 0x82593280;
	sub_8219F808(ctx, base);
	// b 0x82593294
	goto loc_82593294;
loc_82593284:
	// cmpwi cr6,r10,1
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 1, ctx.xer);
	// beq cr6,0x8259327c
	if (ctx.cr6.eq) goto loc_8259327C;
	// li r3,1
	ctx.r3.s64 = 1;
	// bl 0x8219f8d8
	ctx.lr = 0x82593294;
	sub_8219F8D8(ctx, base);
loc_82593294:
	// addi r11,r30,194
	ctx.r11.s64 = ctx.r30.s64 + 194;
	// lwz r9,0(r31)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// rlwinm r10,r11,1,0,30
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// add r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r11,r11,r3
	ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
	// lbz r10,2(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 2);
	// lbz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// xor r11,r10,r11
	ctx.r11.u64 = ctx.r10.u64 ^ ctx.r11.u64;
	// stw r11,0(r9)
	PPC_STORE_U32(ctx.r9.u32 + 0, ctx.r11.u32);
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

PPC_FUNC_IMPL(__imp__sub_825932D8);  /* forward decl */
PPC_WEAK_FUNC(sub_825932D8) { __imp__sub_825932D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825932D8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r3,0
	ctx.r3.s64 = 0;
	// bl 0x8219f8d8
	ctx.lr = 0x825932F4;
	sub_8219F8D8(ctx, base);
	// lwz r11,4192(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 4192);
	// lwz r10,0(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// addi r11,r11,-64
	ctx.r11.s64 = ctx.r11.s64 + -64;
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// addi r11,r11,16
	ctx.r11.s64 = ctx.r11.s64 + 16;
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

PPC_FUNC_IMPL(__imp__sub_82593328);  /* forward decl */
PPC_WEAK_FUNC(sub_82593328) { __imp__sub_82593328(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593328) {
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
	// lis r11,-32253
	ctx.r11.s64 = -2113732608;
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r6,r11,27532
	ctx.r6.s64 = ctx.r11.s64 + 27532;
	// lis r11,-32083
	ctx.r11.s64 = -2102591488;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// li r30,0
	ctx.r30.s64 = 0;
	// addi r3,r11,-16116
	ctx.r3.s64 = ctx.r11.s64 + -16116;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824c16c8
	ctx.lr = 0x82593364;
	sub_824C16C8(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82593374
	if (ctx.cr6.eq) goto loc_82593374;
	// li r30,1
	ctx.r30.s64 = 1;
loc_82593374:
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r30,24
	ctx.r10.u64 = ctx.r30.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
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

PPC_FUNC_IMPL(__imp__sub_82593398);  /* forward decl */
PPC_WEAK_FUNC(sub_82593398) { __imp__sub_82593398(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593398) {
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
	// lis r11,-32253
	ctx.r11.s64 = -2113732608;
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r6,r11,27532
	ctx.r6.s64 = ctx.r11.s64 + 27532;
	// lis r11,-32083
	ctx.r11.s64 = -2102591488;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// li r30,0
	ctx.r30.s64 = 0;
	// addi r3,r11,-16116
	ctx.r3.s64 = ctx.r11.s64 + -16116;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824c1668
	ctx.lr = 0x825933D4;
	sub_824C1668(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825933e4
	if (ctx.cr6.eq) goto loc_825933E4;
	// li r30,1
	ctx.r30.s64 = 1;
loc_825933E4:
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r30,24
	ctx.r10.u64 = ctx.r30.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
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

PPC_FUNC_IMPL(__imp__sub_82593408);  /* forward decl */
PPC_WEAK_FUNC(sub_82593408) { __imp__sub_82593408(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593408) {
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
	// lis r11,-32253
	ctx.r11.s64 = -2113732608;
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r6,r11,27548
	ctx.r6.s64 = ctx.r11.s64 + 27548;
	// lis r11,-32083
	ctx.r11.s64 = -2102591488;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// li r30,0
	ctx.r30.s64 = 0;
	// addi r3,r11,-16116
	ctx.r3.s64 = ctx.r11.s64 + -16116;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824c16c8
	ctx.lr = 0x82593444;
	sub_824C16C8(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82593454
	if (ctx.cr6.eq) goto loc_82593454;
	// li r30,1
	ctx.r30.s64 = 1;
loc_82593454:
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r30,24
	ctx.r10.u64 = ctx.r30.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
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

PPC_FUNC_IMPL(__imp__sub_82593478);  /* forward decl */
PPC_WEAK_FUNC(sub_82593478) { __imp__sub_82593478(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593478) {
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
	// lis r11,-32253
	ctx.r11.s64 = -2113732608;
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r6,r11,27548
	ctx.r6.s64 = ctx.r11.s64 + 27548;
	// lis r11,-32083
	ctx.r11.s64 = -2102591488;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// li r30,0
	ctx.r30.s64 = 0;
	// addi r3,r11,-16116
	ctx.r3.s64 = ctx.r11.s64 + -16116;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x824c1668
	ctx.lr = 0x825934B4;
	sub_824C1668(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825934c4
	if (ctx.cr6.eq) goto loc_825934C4;
	// li r30,1
	ctx.r30.s64 = 1;
loc_825934C4:
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r30,24
	ctx.r10.u64 = ctx.r30.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
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

PPC_FUNC_IMPL(__imp__sub_825934E8);  /* forward decl */
PPC_WEAK_FUNC(sub_825934E8) { __imp__sub_825934E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825934E8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// cmpwi cr6,r10,4
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 4, ctx.xer);
	// blt cr6,0x8259350c
	if (ctx.cr6.lt) goto loc_8259350C;
	// lis r11,-32083
	ctx.r11.s64 = -2102591488;
	// addi r3,r11,-16304
	ctx.r3.s64 = ctx.r11.s64 + -16304;
	// b 0x82593540
	goto loc_82593540;
loc_8259350C:
	// lis r11,-32094
	ctx.r11.s64 = -2103312384;
	// lwz r10,11524(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 11524);
	// cmpwi cr6,r10,-1
	ctx.cr6.compare<int32_t>(ctx.r10.s32, -1, ctx.xer);
	// beq cr6,0x82593530
	if (ctx.cr6.eq) goto loc_82593530;
	// lis r11,-32083
	ctx.r11.s64 = -2102591488;
	// mulli r10,r10,188
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(188));
	// addi r11,r11,-17056
	ctx.r11.s64 = ctx.r11.s64 + -17056;
	// add r11,r10,r11
	ctx.r11.u64 = ctx.r10.u64 + ctx.r11.u64;
	// b 0x82593534
	goto loc_82593534;
loc_82593530:
	// li r11,0
	ctx.r11.s64 = 0;
loc_82593534:
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
loc_82593540:
	// li r8,0
	ctx.r8.s64 = 0;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// b 0x8230d898
	sub_8230D898(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82593550);  /* forward decl */
PPC_WEAK_FUNC(sub_82593550) { __imp__sub_82593550(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593550) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82593558);  /* forward decl */
PPC_WEAK_FUNC(sub_82593558) { __imp__sub_82593558(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593558) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// addi r11,r11,-14792
	ctx.r11.s64 = ctx.r11.s64 + -14792;
	// lwz r11,24(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// addi r11,r11,-9
	ctx.r11.s64 = ctx.r11.s64 + -9;
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
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82593588);  /* forward decl */
PPC_WEAK_FUNC(sub_82593588) { __imp__sub_82593588(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593588) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// addi r11,r11,-14792
	ctx.r11.s64 = ctx.r11.s64 + -14792;
	// lwz r11,28(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
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
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825935B0);  /* forward decl */
PPC_WEAK_FUNC(sub_825935B0) { __imp__sub_825935B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825935B0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x82593070
	ctx.lr = 0x825935C8;
	sub_82593070(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825935E8);  /* forward decl */
PPC_WEAK_FUNC(sub_825935E8) { __imp__sub_825935E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825935E8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// addi r11,r11,-14792
	ctx.r11.s64 = ctx.r11.s64 + -14792;
	// lwz r11,32(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 32);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82593610);  /* forward decl */
PPC_WEAK_FUNC(sub_82593610) { __imp__sub_82593610(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593610) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// addi r11,r11,-14792
	ctx.r11.s64 = ctx.r11.s64 + -14792;
	// lwz r11,44(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 44);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82593630);  /* forward decl */
PPC_WEAK_FUNC(sub_82593630) { __imp__sub_82593630(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593630) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r9,8(r3)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r9)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
	// lwz r4,4(r9)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r9.u32 + 4);
	// bl 0x82592940
	ctx.lr = 0x8259364C;
	sub_82592940(ctx, base);
	// lwz r11,8(r9)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r9.u32 + 8);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82593668);  /* forward decl */
PPC_WEAK_FUNC(sub_82593668) { __imp__sub_82593668(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593668) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// cmpwi cr6,r10,4
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 4, ctx.xer);
	// lwz r6,4(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// blt cr6,0x825936b4
	if (ctx.cr6.lt) goto loc_825936B4;
	// lis r11,-32083
	ctx.r11.s64 = -2102591488;
	// addi r11,r11,-16304
	ctx.r11.s64 = ctx.r11.s64 + -16304;
loc_82593690:
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// stw r10,0(r6)
	PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r10.u32);
	// lwz r10,8(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// stw r10,0(r5)
	PPC_STORE_U32(ctx.r5.u32 + 0, ctx.r10.u32);
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// stw r10,0(r8)
	PPC_STORE_U32(ctx.r8.u32 + 0, ctx.r10.u32);
	// lwz r11,16(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// stw r11,0(r7)
	PPC_STORE_U32(ctx.r7.u32 + 0, ctx.r11.u32);
	// blr 
	return;
loc_825936B4:
	// lis r11,-32094
	ctx.r11.s64 = -2103312384;
	// li r10,0
	ctx.r10.s64 = 0;
	// lwz r11,11524(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 11524);
	// cmpwi cr6,r11,-1
	ctx.cr6.compare<int32_t>(ctx.r11.s32, -1, ctx.xer);
	// beq cr6,0x825936dc
	if (ctx.cr6.eq) goto loc_825936DC;
	// lis r9,-32083
	ctx.r9.s64 = -2102591488;
	// mulli r11,r11,188
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(188));
	// addi r9,r9,-17056
	ctx.r9.s64 = ctx.r9.s64 + -17056;
	// add r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x825936e0
	goto loc_825936E0;
loc_825936DC:
	// mr r11,r10
	ctx.r11.u64 = ctx.r10.u64;
loc_825936E0:
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82593690
	if (!ctx.cr6.eq) goto loc_82593690;
	// stw r10,0(r6)
	PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r10.u32);
	// stw r10,0(r5)
	PPC_STORE_U32(ctx.r5.u32 + 0, ctx.r10.u32);
	// stw r10,0(r8)
	PPC_STORE_U32(ctx.r8.u32 + 0, ctx.r10.u32);
	// stw r10,0(r7)
	PPC_STORE_U32(ctx.r7.u32 + 0, ctx.r10.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82593700);  /* forward decl */
PPC_WEAK_FUNC(sub_82593700) { __imp__sub_82593700(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593700) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r9,0
	ctx.r9.s64 = 0;
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stb r9,0(r11)
	PPC_STORE_U8(ctx.r11.u32 + 0, ctx.r9.u8);
	// stb r9,0(r10)
	PPC_STORE_U8(ctx.r10.u32 + 0, ctx.r9.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82593720);  /* forward decl */
PPC_WEAK_FUNC(sub_82593720) { __imp__sub_82593720(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593720) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,12592
	ctx.r4.s64 = ctx.r11.s64 + 12592;
	// addi r3,r10,28332
	ctx.r3.s64 = ctx.r10.s64 + 28332;
	// bl 0x827d6c90
	ctx.lr = 0x82593740;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,12664
	ctx.r4.s64 = ctx.r11.s64 + 12664;
	// addi r3,r10,28308
	ctx.r3.s64 = ctx.r10.s64 + 28308;
	// bl 0x827d6c90
	ctx.lr = 0x82593754;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,12736
	ctx.r4.s64 = ctx.r11.s64 + 12736;
	// addi r3,r10,28288
	ctx.r3.s64 = ctx.r10.s64 + 28288;
	// bl 0x827d6c90
	ctx.lr = 0x82593768;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,12808
	ctx.r4.s64 = ctx.r11.s64 + 12808;
	// addi r3,r10,28264
	ctx.r3.s64 = ctx.r10.s64 + 28264;
	// bl 0x827d6c90
	ctx.lr = 0x8259377C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,12880
	ctx.r4.s64 = ctx.r11.s64 + 12880;
	// addi r3,r10,28244
	ctx.r3.s64 = ctx.r10.s64 + 28244;
	// bl 0x827d6c90
	ctx.lr = 0x82593790;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,13016
	ctx.r4.s64 = ctx.r11.s64 + 13016;
	// addi r3,r10,28224
	ctx.r3.s64 = ctx.r10.s64 + 28224;
	// bl 0x827d6c90
	ctx.lr = 0x825937A4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,13872
	ctx.r4.s64 = ctx.r11.s64 + 13872;
	// addi r3,r10,28208
	ctx.r3.s64 = ctx.r10.s64 + 28208;
	// bl 0x827d6c90
	ctx.lr = 0x825937B8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,13928
	ctx.r4.s64 = ctx.r11.s64 + 13928;
	// addi r3,r10,28176
	ctx.r3.s64 = ctx.r10.s64 + 28176;
	// bl 0x827d6c90
	ctx.lr = 0x825937CC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,13096
	ctx.r4.s64 = ctx.r11.s64 + 13096;
	// addi r3,r10,28152
	ctx.r3.s64 = ctx.r10.s64 + 28152;
	// bl 0x827d6c90
	ctx.lr = 0x825937E0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,13208
	ctx.r4.s64 = ctx.r11.s64 + 13208;
	// addi r3,r10,28120
	ctx.r3.s64 = ctx.r10.s64 + 28120;
	// bl 0x827d6c90
	ctx.lr = 0x825937F4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,13320
	ctx.r4.s64 = ctx.r11.s64 + 13320;
	// addi r3,r10,28088
	ctx.r3.s64 = ctx.r10.s64 + 28088;
	// bl 0x827d6c90
	ctx.lr = 0x82593808;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,13432
	ctx.r4.s64 = ctx.r11.s64 + 13432;
	// addi r3,r10,28052
	ctx.r3.s64 = ctx.r10.s64 + 28052;
	// bl 0x827d6c90
	ctx.lr = 0x8259381C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24160
	ctx.r4.s64 = ctx.r11.s64 + 24160;
	// addi r3,r10,28032
	ctx.r3.s64 = ctx.r10.s64 + 28032;
	// bl 0x827d6c90
	ctx.lr = 0x82593830;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,13544
	ctx.r4.s64 = ctx.r11.s64 + 13544;
	// addi r3,r10,28020
	ctx.r3.s64 = ctx.r10.s64 + 28020;
	// bl 0x827d6c90
	ctx.lr = 0x82593844;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,13656
	ctx.r4.s64 = ctx.r11.s64 + 13656;
	// addi r3,r10,27984
	ctx.r3.s64 = ctx.r10.s64 + 27984;
	// bl 0x827d6c90
	ctx.lr = 0x82593858;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,13704
	ctx.r4.s64 = ctx.r11.s64 + 13704;
	// addi r3,r10,27964
	ctx.r3.s64 = ctx.r10.s64 + 27964;
	// bl 0x827d6c90
	ctx.lr = 0x8259386C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,13744
	ctx.r4.s64 = ctx.r11.s64 + 13744;
	// addi r3,r10,27932
	ctx.r3.s64 = ctx.r10.s64 + 27932;
	// bl 0x827d6c90
	ctx.lr = 0x82593880;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,13800
	ctx.r4.s64 = ctx.r11.s64 + 13800;
	// addi r3,r10,27912
	ctx.r3.s64 = ctx.r10.s64 + 27912;
	// bl 0x827d6c90
	ctx.lr = 0x82593894;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,12064
	ctx.r4.s64 = ctx.r11.s64 + 12064;
	// addi r3,r10,27868
	ctx.r3.s64 = ctx.r10.s64 + 27868;
	// bl 0x827d6c90
	ctx.lr = 0x825938A8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,12080
	ctx.r4.s64 = ctx.r11.s64 + 12080;
	// addi r3,r10,27820
	ctx.r3.s64 = ctx.r10.s64 + 27820;
	// bl 0x827d6c90
	ctx.lr = 0x825938BC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,13840
	ctx.r4.s64 = ctx.r11.s64 + 13840;
	// addi r3,r10,27796
	ctx.r3.s64 = ctx.r10.s64 + 27796;
	// bl 0x827d6c90
	ctx.lr = 0x825938D0;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-4208
	ctx.r4.s64 = ctx.r11.s64 + -4208;
	// addi r3,r10,27768
	ctx.r3.s64 = ctx.r10.s64 + 27768;
	// bl 0x827d6c90
	ctx.lr = 0x825938E4;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-4208
	ctx.r4.s64 = ctx.r11.s64 + -4208;
	// addi r3,r10,27748
	ctx.r3.s64 = ctx.r10.s64 + 27748;
	// bl 0x827d6c90
	ctx.lr = 0x825938F8;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-4208
	ctx.r4.s64 = ctx.r11.s64 + -4208;
	// addi r3,r10,27728
	ctx.r3.s64 = ctx.r10.s64 + 27728;
	// bl 0x827d6c90
	ctx.lr = 0x8259390C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,14080
	ctx.r4.s64 = ctx.r11.s64 + 14080;
	// addi r3,r10,27700
	ctx.r3.s64 = ctx.r10.s64 + 27700;
	// bl 0x827d6c90
	ctx.lr = 0x82593920;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-4208
	ctx.r4.s64 = ctx.r11.s64 + -4208;
	// addi r3,r10,27664
	ctx.r3.s64 = ctx.r10.s64 + 27664;
	// bl 0x827d6c90
	ctx.lr = 0x82593934;
	sub_827D6C90(ctx, base);
	// lis r11,-32114
	ctx.r11.s64 = -2104623104;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,2744
	ctx.r4.s64 = ctx.r11.s64 + 2744;
	// addi r3,r10,27620
	ctx.r3.s64 = ctx.r10.s64 + 27620;
	// bl 0x827d6c90
	ctx.lr = 0x82593948;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-4208
	ctx.r4.s64 = ctx.r11.s64 + -4208;
	// addi r3,r10,27588
	ctx.r3.s64 = ctx.r10.s64 + 27588;
	// bl 0x827d6c90
	ctx.lr = 0x8259395C;
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

PPC_FUNC_IMPL(__imp__sub_82593970);  /* forward decl */
PPC_WEAK_FUNC(sub_82593970) { __imp__sub_82593970(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593970) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stfd f29,-40(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -40, ctx.f29.u64);
	// stfd f30,-32(r1)
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f30.u64);
	// stfd f31,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f31.u64);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// mr r31,r7
	ctx.r31.u64 = ctx.r7.u64;
	// fmr f29,f4
	ctx.f29.f64 = ctx.f4.f64;
	// lfs f0,12120(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12120);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f3,f0
	ctx.cr6.compare(ctx.f3.f64, ctx.f0.f64);
	// bgt cr6,0x825939b8
	if (ctx.cr6.gt) goto loc_825939B8;
	// li r5,4
	ctx.r5.s64 = 4;
	// bl 0x8231fbb0
	ctx.lr = 0x825939B4;
	sub_8231FBB0(ctx, base);
	// fmr f3,f1
	ctx.fpscr.disableFlushMode();
	ctx.f3.f64 = ctx.f1.f64;
loc_825939B8:
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// stfs f31,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// stfs f30,84(r1)
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f3,88(r1)
	temp.f32 = float(ctx.f3.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// fmr f1,f29
	ctx.f1.f64 = ctx.f29.f64;
	// bl 0x822718a0
	ctx.lr = 0x825939D4;
	sub_822718A0(ctx, base);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f29,-40(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f29.u64 = PPC_LOAD_U64(ctx.r1.u32 + -40);
	// lfd f30,-32(r1)
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// lfd f31,-24(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825939F8);  /* forward decl */
PPC_WEAK_FUNC(sub_825939F8) { __imp__sub_825939F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825939F8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stfd f29,-40(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -40, ctx.f29.u64);
	// stfd f30,-32(r1)
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f30.u64);
	// stfd f31,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f31.u64);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// mr r31,r7
	ctx.r31.u64 = ctx.r7.u64;
	// fmr f29,f4
	ctx.f29.f64 = ctx.f4.f64;
	// lfs f0,12120(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12120);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f3,f0
	ctx.cr6.compare(ctx.f3.f64, ctx.f0.f64);
	// bgt cr6,0x82593a40
	if (ctx.cr6.gt) goto loc_82593A40;
	// li r5,4
	ctx.r5.s64 = 4;
	// bl 0x8231fbb0
	ctx.lr = 0x82593A3C;
	sub_8231FBB0(ctx, base);
	// fmr f3,f1
	ctx.fpscr.disableFlushMode();
	ctx.f3.f64 = ctx.f1.f64;
loc_82593A40:
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// stfs f31,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// stfs f30,84(r1)
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f3,88(r1)
	temp.f32 = float(ctx.f3.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// fmr f1,f29
	ctx.f1.f64 = ctx.f29.f64;
	// bl 0x822718e8
	ctx.lr = 0x82593A5C;
	sub_822718E8(ctx, base);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f29,-40(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f29.u64 = PPC_LOAD_U64(ctx.r1.u32 + -40);
	// lfd f30,-32(r1)
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// lfd f31,-24(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82593A80);  /* forward decl */
PPC_WEAK_FUNC(sub_82593A80) { __imp__sub_82593A80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593A80) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb08
	ctx.lr = 0x82593A88;
	__savegprlr_24(ctx, base);
	// stwu r1,-176(r1)
	ea = -176 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// addi r4,r11,-23332
	ctx.r4.s64 = ctx.r11.s64 + -23332;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// mr r26,r8
	ctx.r26.u64 = ctx.r8.u64;
	// mr r25,r9
	ctx.r25.u64 = ctx.r9.u64;
	// mr r24,r10
	ctx.r24.u64 = ctx.r10.u64;
	// bl 0x8298f040
	ctx.lr = 0x82593AB8;
	sub_8298F040(ctx, base);
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// bne cr6,0x82593ac8
	if (!ctx.cr6.eq) goto loc_82593AC8;
	// li r7,0
	ctx.r7.s64 = 0;
	// b 0x82593adc
	goto loc_82593ADC;
loc_82593AC8:
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// addi r3,r11,12544
	ctx.r3.s64 = ctx.r11.s64 + 12544;
	// bl 0x82273620
	ctx.lr = 0x82593AD8;
	sub_82273620(ctx, base);
	// mr r7,r3
	ctx.r7.u64 = ctx.r3.u64;
loc_82593ADC:
	// lbz r11,263(r1)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r1.u32 + 263);
	// mr r10,r24
	ctx.r10.u64 = ctx.r24.u64;
	// mr r9,r25
	ctx.r9.u64 = ctx.r25.u64;
	// mr r8,r26
	ctx.r8.u64 = ctx.r26.u64;
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// stb r11,87(r1)
	PPC_STORE_U8(ctx.r1.u32 + 87, ctx.r11.u8);
	// li r11,0
	ctx.r11.s64 = 0;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// stb r11,95(r1)
	PPC_STORE_U8(ctx.r1.u32 + 95, ctx.r11.u8);
	// bl 0x82538610
	ctx.lr = 0x82593B0C;
	sub_82538610(ctx, base);
	// addi r1,r1,176
	ctx.r1.s64 = ctx.r1.s64 + 176;
	// b 0x8298eb58
	__restgprlr_24(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82593B18);  /* forward decl */
PPC_WEAK_FUNC(sub_82593B18) { __imp__sub_82593B18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593B18) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// addi r12,r1,-8
	ctx.r12.s64 = ctx.r1.s64 + -8;
	// bl 0x82990140
	ctx.lr = 0x82593B28;
	__savefpr_26(ctx, base);
	// stwu r1,-176(r1)
	ea = -176 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// clrlwi r11,r9,24
	ctx.r11.u64 = ctx.r9.u32 & 0xFF;
	// fmr f27,f1
	ctx.fpscr.disableFlushMode();
	ctx.f27.f64 = ctx.f1.f64;
	// fmr f29,f2
	ctx.f29.f64 = ctx.f2.f64;
	// li r5,0
	ctx.r5.s64 = 0;
	// fmr f31,f3
	ctx.f31.f64 = ctx.f3.f64;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// fmr f26,f4
	ctx.f26.f64 = ctx.f4.f64;
	// fmr f28,f5
	ctx.f28.f64 = ctx.f5.f64;
	// fmr f30,f6
	ctx.f30.f64 = ctx.f6.f64;
	// beq cr6,0x82593b5c
	if (ctx.cr6.eq) goto loc_82593B5C;
	// bl 0x8227f650
	ctx.lr = 0x82593B58;
	sub_8227F650(ctx, base);
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
loc_82593B5C:
	// fcmpu cr6,f27,f26
	ctx.fpscr.disableFlushMode();
	ctx.cr6.compare(ctx.f27.f64, ctx.f26.f64);
	// ble cr6,0x82593b70
	if (!ctx.cr6.gt) goto loc_82593B70;
	// fmr f0,f27
	ctx.f0.f64 = ctx.f27.f64;
	// fmr f27,f26
	ctx.f27.f64 = ctx.f26.f64;
	// fmr f26,f0
	ctx.f26.f64 = ctx.f0.f64;
loc_82593B70:
	// fcmpu cr6,f29,f28
	ctx.fpscr.disableFlushMode();
	ctx.cr6.compare(ctx.f29.f64, ctx.f28.f64);
	// ble cr6,0x82593b84
	if (!ctx.cr6.gt) goto loc_82593B84;
	// fmr f0,f29
	ctx.f0.f64 = ctx.f29.f64;
	// fmr f29,f28
	ctx.f29.f64 = ctx.f28.f64;
	// fmr f28,f0
	ctx.f28.f64 = ctx.f0.f64;
loc_82593B84:
	// fcmpu cr6,f31,f30
	ctx.fpscr.disableFlushMode();
	ctx.cr6.compare(ctx.f31.f64, ctx.f30.f64);
	// ble cr6,0x82593b98
	if (!ctx.cr6.gt) goto loc_82593B98;
	// fmr f0,f31
	ctx.f0.f64 = ctx.f31.f64;
	// fmr f31,f30
	ctx.f31.f64 = ctx.f30.f64;
	// fmr f30,f0
	ctx.f30.f64 = ctx.f0.f64;
loc_82593B98:
	// addi r4,r1,96
	ctx.r4.s64 = ctx.r1.s64 + 96;
	// stfs f27,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f27.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// stfs f29,84(r1)
	temp.f32 = float(ctx.f29.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f31,88(r1)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// stfs f26,96(r1)
	temp.f32 = float(ctx.f26.f64);
	PPC_STORE_U32(ctx.r1.u32 + 96, temp.u32);
	// stfs f28,100(r1)
	temp.f32 = float(ctx.f28.f64);
	PPC_STORE_U32(ctx.r1.u32 + 100, temp.u32);
	// stfs f30,104(r1)
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r1.u32 + 104, temp.u32);
	// bl 0x82317dc8
	ctx.lr = 0x82593BBC;
	sub_82317DC8(ctx, base);
	// addi r1,r1,176
	ctx.r1.s64 = ctx.r1.s64 + 176;
	// addi r12,r1,-8
	ctx.r12.s64 = ctx.r1.s64 + -8;
	// bl 0x8299018c
	ctx.lr = 0x82593BC8;
	__restfpr_26(ctx, base);
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82593BD8);  /* forward decl */
PPC_WEAK_FUNC(sub_82593BD8) { __imp__sub_82593BD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593BD8) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,0(r4)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r4.u32 + 0);
	// lwz r11,4(r4)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 4);
	// cmpw cr6,r10,r11
	ctx.cr6.compare<int32_t>(ctx.r10.s32, ctx.r11.s32, ctx.xer);
	// bge cr6,0x82593c08
	if (!ctx.cr6.lt) goto loc_82593C08;
	// lwz r11,8(r4)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 8);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82593bfc
	if (ctx.cr6.eq) goto loc_82593BFC;
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// stwx r3,r10,r11
	PPC_STORE_U32(ctx.r10.u32 + ctx.r11.u32, ctx.r3.u32);
loc_82593BFC:
	// lwz r11,0(r4)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 0);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// stw r11,0(r4)
	PPC_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
loc_82593C08:
	// li r3,1
	ctx.r3.s64 = 1;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82593C10);  /* forward decl */
PPC_WEAK_FUNC(sub_82593C10) { __imp__sub_82593C10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593C10) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stfd f29,-40(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -40, ctx.f29.u64);
	// stfd f30,-32(r1)
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f30.u64);
	// stfd f31,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f31.u64);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// mr r31,r7
	ctx.r31.u64 = ctx.r7.u64;
	// fmr f29,f4
	ctx.f29.f64 = ctx.f4.f64;
	// lfs f0,12120(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12120);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f3,f0
	ctx.cr6.compare(ctx.f3.f64, ctx.f0.f64);
	// bgt cr6,0x82593c58
	if (ctx.cr6.gt) goto loc_82593C58;
	// li r5,4
	ctx.r5.s64 = 4;
	// bl 0x8231fbb0
	ctx.lr = 0x82593C54;
	sub_8231FBB0(ctx, base);
	// fmr f3,f1
	ctx.fpscr.disableFlushMode();
	ctx.f3.f64 = ctx.f1.f64;
loc_82593C58:
	// li r8,0
	ctx.r8.s64 = 0;
	// stfs f31,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// li r7,0
	ctx.r7.s64 = 0;
	// stfs f30,84(r1)
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// li r6,0
	ctx.r6.s64 = 0;
	// stfs f3,88(r1)
	temp.f32 = float(ctx.f3.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// fmr f1,f29
	ctx.f1.f64 = ctx.f29.f64;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x82284058
	ctx.lr = 0x82593C80;
	sub_82284058(ctx, base);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f29,-40(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f29.u64 = PPC_LOAD_U64(ctx.r1.u32 + -40);
	// lfd f30,-32(r1)
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// lfd f31,-24(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82593CA0);  /* forward decl */
PPC_WEAK_FUNC(sub_82593CA0) { __imp__sub_82593CA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593CA0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// std r3,160(r1)
	PPC_STORE_U64(ctx.r1.u32 + 160, ctx.r3.u64);
	// addi r11,r1,112
	ctx.r11.s64 = ctx.r1.s64 + 112;
	// std r4,168(r1)
	PPC_STORE_U64(ctx.r1.u32 + 168, ctx.r4.u64);
	// addi r3,r1,112
	ctx.r3.s64 = ctx.r1.s64 + 112;
	// std r5,176(r1)
	PPC_STORE_U64(ctx.r1.u32 + 176, ctx.r5.u64);
	// addi r4,r1,96
	ctx.r4.s64 = ctx.r1.s64 + 96;
	// std r6,184(r1)
	PPC_STORE_U64(ctx.r1.u32 + 184, ctx.r6.u64);
	// li r6,0
	ctx.r6.s64 = 0;
	// lfs f0,160(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 160);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// lfs f0,164(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 164);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,84(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// lfs f0,168(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 168);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// addi r10,r1,80
	ctx.r10.s64 = ctx.r1.s64 + 80;
	// lfs f0,176(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 176);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,96(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 96, temp.u32);
	// lfs f0,180(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 180);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,100(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 100, temp.u32);
	// lfs f0,184(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 184);
	ctx.f0.f64 = double(temp.f32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stfs f0,104(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 104, temp.u32);
	// addi r10,r1,96
	ctx.r10.s64 = ctx.r1.s64 + 96;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,96
	ctx.r11.s64 = ctx.r1.s64 + 96;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x82281ad0
	ctx.lr = 0x82593D1C;
	sub_82281AD0(ctx, base);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82593D30);  /* forward decl */
PPC_WEAK_FUNC(sub_82593D30) { __imp__sub_82593D30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593D30) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stfd f29,-32(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f29.u64);
	// stfd f30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f30.u64);
	// stfd f31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.f31.u64);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// fmr f29,f4
	ctx.f29.f64 = ctx.f4.f64;
	// lfs f0,12120(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12120);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f3,f0
	ctx.cr6.compare(ctx.f3.f64, ctx.f0.f64);
	// bgt cr6,0x82593d70
	if (ctx.cr6.gt) goto loc_82593D70;
	// li r5,4
	ctx.r5.s64 = 4;
	// bl 0x8231fbb0
	ctx.lr = 0x82593D6C;
	sub_8231FBB0(ctx, base);
	// fmr f3,f1
	ctx.fpscr.disableFlushMode();
	ctx.f3.f64 = ctx.f1.f64;
loc_82593D70:
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// stfs f31,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f30,84(r1)
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// fmr f1,f29
	ctx.f1.f64 = ctx.f29.f64;
	// stfs f3,88(r1)
	temp.f32 = float(ctx.f3.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// bl 0x822d00b0
	ctx.lr = 0x82593D88;
	sub_822D00B0(ctx, base);
	// li r7,0
	ctx.r7.s64 = 0;
	// fmr f1,f29
	ctx.fpscr.disableFlushMode();
	ctx.f1.f64 = ctx.f29.f64;
	// li r6,0
	ctx.r6.s64 = 0;
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x823cc3f0
	ctx.lr = 0x82593DA0;
	sub_823CC3F0(ctx, base);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f29,-32(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f29.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// lfd f30,-24(r1)
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// lfd f31,-16(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82593DC0);  /* forward decl */
PPC_WEAK_FUNC(sub_82593DC0) { __imp__sub_82593DC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593DC0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stfd f31,-24(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f31.u64);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// bl 0x8227f650
	ctx.lr = 0x82593DD8;
	sub_8227F650(ctx, base);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x82593e48
	if (ctx.cr6.eq) goto loc_82593E48;
	// bl 0x8227f650
	ctx.lr = 0x82593DE4;
	sub_8227F650(ctx, base);
	// lwz r11,568(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 568);
	// rlwinm r11,r11,0,2,2
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 0) & 0x20000000;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82593e48
	if (!ctx.cr6.eq) goto loc_82593E48;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r31,r1,80
	ctx.r31.s64 = ctx.r1.s64 + 80;
	// lfs f31,3068(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3068);
	ctx.f31.f64 = double(temp.f32);
	// stfs f31,80(r1)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f31,84(r1)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f31,88(r1)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// bl 0x8227f650
	ctx.lr = 0x82593E10;
	sub_8227F650(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x82434440
	ctx.lr = 0x82593E18;
	sub_82434440(ctx, base);
	// stfs f31,96(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 96, temp.u32);
	// stfs f31,100(r1)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 100, temp.u32);
	// addi r31,r1,96
	ctx.r31.s64 = ctx.r1.s64 + 96;
	// stfs f31,104(r1)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 104, temp.u32);
	// bl 0x8227f650
	ctx.lr = 0x82593E2C;
	sub_8227F650(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x82434598
	ctx.lr = 0x82593E34;
	sub_82434598(ctx, base);
	// lis r10,-32071
	ctx.r10.s64 = -2101805056;
	// li r11,0
	ctx.r11.s64 = 0;
	// li r3,11
	ctx.r3.s64 = 11;
	// stb r11,17737(r10)
	PPC_STORE_U8(ctx.r10.u32 + 17737, ctx.r11.u8);
	// bl 0x8224b980
	ctx.lr = 0x82593E48;
	sub_8224B980(ctx, base);
loc_82593E48:
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f31,-24(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82593E60);  /* forward decl */
PPC_WEAK_FUNC(sub_82593E60) { __imp__sub_82593E60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593E60) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stfd f31,-24(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f31.u64);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// bl 0x8227f650
	ctx.lr = 0x82593E78;
	sub_8227F650(ctx, base);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x82593ec8
	if (ctx.cr6.eq) goto loc_82593EC8;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r31,r1,80
	ctx.r31.s64 = ctx.r1.s64 + 80;
	// lfs f31,3068(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3068);
	ctx.f31.f64 = double(temp.f32);
	// stfs f31,80(r1)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f31,84(r1)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f31,88(r1)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// bl 0x8227f650
	ctx.lr = 0x82593E9C;
	sub_8227F650(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x82434440
	ctx.lr = 0x82593EA4;
	sub_82434440(ctx, base);
	// stfs f31,96(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 96, temp.u32);
	// stfs f31,100(r1)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 100, temp.u32);
	// addi r31,r1,96
	ctx.r31.s64 = ctx.r1.s64 + 96;
	// stfs f31,104(r1)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 104, temp.u32);
	// bl 0x8227f650
	ctx.lr = 0x82593EB8;
	sub_8227F650(ctx, base);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x82434598
	ctx.lr = 0x82593EC0;
	sub_82434598(ctx, base);
	// li r3,13
	ctx.r3.s64 = 13;
	// bl 0x8224b980
	ctx.lr = 0x82593EC8;
	sub_8224B980(ctx, base);
loc_82593EC8:
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f31,-24(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82593EE0);  /* forward decl */
PPC_WEAK_FUNC(sub_82593EE0) { __imp__sub_82593EE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593EE0) {
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
	// fmuls f0,f2,f2
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(float(ctx.f2.f64 * ctx.f2.f64));
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// fmuls f13,f4,f4
	ctx.f13.f64 = double(float(ctx.f4.f64 * ctx.f4.f64));
	// mr r31,r7
	ctx.r31.u64 = ctx.r7.u64;
	// fmuls f12,f2,f4
	ctx.f12.f64 = double(float(ctx.f2.f64 * ctx.f4.f64));
	// fmadds f0,f1,f1,f0
	ctx.f0.f64 = double(float(ctx.f1.f64 * ctx.f1.f64 + ctx.f0.f64));
	// fmadds f13,f3,f3,f13
	ctx.f13.f64 = double(float(ctx.f3.f64 * ctx.f3.f64 + ctx.f13.f64));
	// fmadds f12,f1,f3,f12
	ctx.f12.f64 = double(float(ctx.f1.f64 * ctx.f3.f64 + ctx.f12.f64));
	// fsqrts f0,f0
	ctx.f0.f64 = double(float(sqrt(ctx.f0.f64)));
	// fsqrts f13,f13
	ctx.f13.f64 = double(float(sqrt(ctx.f13.f64)));
	// fmuls f0,f0,f13
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f13.f64));
	// lfs f13,-6944(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -6944);
	ctx.f13.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// fdivs f0,f12,f0
	ctx.f0.f64 = double(float(ctx.f12.f64 / ctx.f0.f64));
	// stfs f0,0(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r31.u32 + 0, temp.u32);
	// fsubs f12,f13,f0
	ctx.f12.f64 = double(float(ctx.f13.f64 - ctx.f0.f64));
	// fsel f0,f12,f13,f0
	ctx.f0.f64 = ctx.f12.f64 >= 0.0 ? ctx.f13.f64 : ctx.f0.f64;
	// lfs f13,2592(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2592);
	ctx.f13.f64 = double(temp.f32);
	// stfs f0,0(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r31.u32 + 0, temp.u32);
	// fsubs f12,f13,f0
	ctx.f12.f64 = double(float(ctx.f13.f64 - ctx.f0.f64));
	// fsel f1,f12,f0,f13
	ctx.f1.f64 = ctx.f12.f64 >= 0.0 ? ctx.f0.f64 : ctx.f13.f64;
	// stfs f1,0(r31)
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r31.u32 + 0, temp.u32);
	// bl 0x82991598
	ctx.lr = 0x82593F4C;
	sub_82991598(ctx, base);
	// frsp f13,f1
	ctx.fpscr.disableFlushMode();
	ctx.f13.f64 = double(float(ctx.f1.f64));
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// lfs f0,-23712(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -23712);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
	// stfs f0,0(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r31.u32 + 0, temp.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82593F78);  /* forward decl */
PPC_WEAK_FUNC(sub_82593F78) { __imp__sub_82593F78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593F78) {
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
	// mr r31,r5
	ctx.r31.u64 = ctx.r5.u64;
	// bl 0x8241def8
	ctx.lr = 0x82593F90;
	sub_8241DEF8(ctx, base);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// lfs f0,-23712(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -23712);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// lfs f13,22804(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 22804);
	ctx.f13.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// fmsubs f0,f1,f0,f13
	ctx.f0.f64 = double(float(ctx.f1.f64 * ctx.f0.f64 - ctx.f13.f64));
	// lfs f13,3068(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3068);
	ctx.f13.f64 = double(temp.f32);
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// bge cr6,0x82593fc8
	if (!ctx.cr6.lt) goto loc_82593FC8;
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// lfs f12,-22820(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -22820);
	ctx.f12.f64 = double(temp.f32);
loc_82593FBC:
	// fadds f0,f0,f12
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(float(ctx.f0.f64 + ctx.f12.f64));
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// blt cr6,0x82593fbc
	if (ctx.cr6.lt) goto loc_82593FBC;
loc_82593FC8:
	// stfs f0,0(r31)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r31.u32 + 0, temp.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82593FE0);  /* forward decl */
PPC_WEAK_FUNC(sub_82593FE0) { __imp__sub_82593FE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82593FE0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-320(r1)
	ea = -320 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// addi r3,r1,208
	ctx.r3.s64 = ctx.r1.s64 + 208;
	// stfs f1,112(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r1.u32 + 112, temp.u32);
	// stfs f2,116(r1)
	temp.f32 = float(ctx.f2.f64);
	PPC_STORE_U32(ctx.r1.u32 + 116, temp.u32);
	// mr r31,r9
	ctx.r31.u64 = ctx.r9.u64;
	// stfs f3,120(r1)
	temp.f32 = float(ctx.f3.f64);
	PPC_STORE_U32(ctx.r1.u32 + 120, temp.u32);
	// stfs f4,128(r1)
	temp.f32 = float(ctx.f4.f64);
	PPC_STORE_U32(ctx.r1.u32 + 128, temp.u32);
	// stfs f5,132(r1)
	temp.f32 = float(ctx.f5.f64);
	PPC_STORE_U32(ctx.r1.u32 + 132, temp.u32);
	// stfs f6,136(r1)
	temp.f32 = float(ctx.f6.f64);
	PPC_STORE_U32(ctx.r1.u32 + 136, temp.u32);
	// bl 0x821abaf0
	ctx.lr = 0x82594014;
	sub_821ABAF0(ctx, base);
	// li r6,0
	ctx.r6.s64 = 0;
	// li r5,1
	ctx.r5.s64 = 1;
	// li r4,14
	ctx.r4.s64 = 14;
	// addi r3,r1,208
	ctx.r3.s64 = ctx.r1.s64 + 208;
	// bl 0x821abb48
	ctx.lr = 0x82594028;
	sub_821ABB48(ctx, base);
	// addi r11,r1,112
	ctx.r11.s64 = ctx.r1.s64 + 112;
	// vspltisw v13,0
	simde_mm_store_si128((simde__m128i*)ctx.v13.u32, simde_mm_set1_epi32(int(0x0)));
	// li r10,0
	ctx.r10.s64 = 0;
	// li r9,0
	ctx.r9.s64 = 0;
	// li r8,0
	ctx.r8.s64 = 0;
	// addi r7,r1,128
	ctx.r7.s64 = ctx.r1.s64 + 128;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,128
	ctx.r11.s64 = ctx.r1.s64 + 128;
	// addi r6,r1,112
	ctx.r6.s64 = ctx.r1.s64 + 112;
	// addi r5,r1,144
	ctx.r5.s64 = ctx.r1.s64 + 144;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,208
	ctx.r3.s64 = ctx.r1.s64 + 208;
	// lvx128 v9,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v9.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-31983
	ctx.r11.s64 = -2096037888;
	// vsubfp v9,v9,v0
	ctx.fpscr.enableFlushMode();
	simde_mm_store_ps(ctx.v9.f32, simde_mm_sub_ps(simde_mm_load_ps(ctx.v9.f32), simde_mm_load_ps(ctx.v0.f32)));
	// addi r11,r11,-8288
	ctx.r11.s64 = ctx.r11.s64 + -8288;
	// lvx128 v12,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,3068(r11)
	ctx.fpscr.disableFlushModeUnconditional();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3068);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// stfs f0,96(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 96, temp.u32);
	// addi r11,r11,-704
	ctx.r11.s64 = ctx.r11.s64 + -704;
	// stfs f0,100(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 100, temp.u32);
	// lvx128 v11,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v11.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r11,r11,-720
	ctx.r11.s64 = ctx.r11.s64 + -720;
	// lvx128 v10,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v10.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,2592(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2592);
	ctx.f0.f64 = double(temp.f32);
	// addi r11,r1,192
	ctx.r11.s64 = ctx.r1.s64 + 192;
	// stfs f0,104(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 104, temp.u32);
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// vmsum3fp128 v0,v9,v9
	ctx.fpscr.enableFlushModeUnconditional();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_dp_ps(simde_mm_load_ps(ctx.v9.f32), simde_mm_load_ps(ctx.v9.f32), 0xEF));
	// addi r11,r1,96
	ctx.r11.s64 = ctx.r1.s64 + 96;
	// lvx128 v8,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v8.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,160
	ctx.r11.s64 = ctx.r1.s64 + 160;
	// vpermwi128 v6,v8,135
	simde_mm_store_si128((simde__m128i*)ctx.v6.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)ctx.v8.u32), 0x78));
	// vpermwi128 v4,v8,99
	simde_mm_store_si128((simde__m128i*)ctx.v4.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)ctx.v8.u32), 0x9C));
	// vor v8,v0,v0
	simde_mm_store_si128((simde__m128i*)ctx.v8.u8, simde_mm_load_si128((simde__m128i*)ctx.v0.u8));
	// vrsqrtefp v0,v0
	simde_mm_store_ps(ctx.v0.f32, simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(simde_mm_load_ps(ctx.v0.f32))));
	// vor v3,v0,v0
	simde_mm_store_si128((simde__m128i*)ctx.v3.u8, simde_mm_load_si128((simde__m128i*)ctx.v0.u8));
	// vcmpeqfp v0,v0,v12
	simde_mm_store_ps(ctx.v0.f32, simde_mm_cmpeq_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v12.f32)));
	// vsel v0,v3,v13,v0
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)ctx.v3.u8)), simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)ctx.v13.u8))));
	// vmulfp128 v7,v0,v0
	simde_mm_store_ps(ctx.v7.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v0.f32)));
	// vmulfp128 v3,v11,v0
	simde_mm_store_ps(ctx.v3.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v11.f32), simde_mm_load_ps(ctx.v0.f32)));
	// vnmsubfp v8,v8,v7,v10
	simde_mm_store_ps(ctx.v8.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(ctx.v8.f32), simde_mm_load_ps(ctx.v7.f32)), simde_mm_load_ps(ctx.v10.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
	// vmaddfp v0,v8,v3,v0
	simde_mm_store_ps(ctx.v0.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(ctx.v8.f32), simde_mm_load_ps(ctx.v3.f32)), simde_mm_load_ps(ctx.v0.f32)));
	// vor v3,v13,v13
	simde_mm_store_si128((simde__m128i*)ctx.v3.u8, simde_mm_load_si128((simde__m128i*)ctx.v13.u8));
	// vmulfp128 v0,v9,v0
	simde_mm_store_ps(ctx.v0.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v9.f32), simde_mm_load_ps(ctx.v0.f32)));
	// vpermwi128 v8,v0,99
	simde_mm_store_si128((simde__m128i*)ctx.v8.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)ctx.v0.u32), 0x9C));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// vpermwi128 v7,v0,135
	simde_mm_store_si128((simde__m128i*)ctx.v7.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)ctx.v0.u32), 0x78));
	// extsw r11,r31
	ctx.r11.s64 = ctx.r31.s32;
	// li r31,0
	ctx.r31.s64 = 0;
	// vmulfp128 v9,v6,v8
	simde_mm_store_ps(ctx.v9.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v6.f32), simde_mm_load_ps(ctx.v8.f32)));
	// std r11,96(r1)
	PPC_STORE_U64(ctx.r1.u32 + 96, ctx.r11.u64);
	// stb r31,87(r1)
	PPC_STORE_U8(ctx.r1.u32 + 87, ctx.r31.u8);
	// vnmsubfp v9,v4,v7,v9
	simde_mm_store_ps(ctx.v9.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(ctx.v4.f32), simde_mm_load_ps(ctx.v7.f32)), simde_mm_load_ps(ctx.v9.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
	// vmsum3fp128 v6,v9,v9
	simde_mm_store_ps(ctx.v6.f32, simde_mm_dp_ps(simde_mm_load_ps(ctx.v9.f32), simde_mm_load_ps(ctx.v9.f32), 0xEF));
	// vrsqrtefp v0,v6
	simde_mm_store_ps(ctx.v0.f32, simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(simde_mm_load_ps(ctx.v6.f32))));
	// vor v4,v0,v0
	simde_mm_store_si128((simde__m128i*)ctx.v4.u8, simde_mm_load_si128((simde__m128i*)ctx.v0.u8));
	// vcmpeqfp v0,v0,v12
	simde_mm_store_ps(ctx.v0.f32, simde_mm_cmpeq_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v12.f32)));
	// vsel v0,v4,v13,v0
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)ctx.v4.u8)), simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)ctx.v13.u8))));
	// vmulfp128 v5,v0,v0
	simde_mm_store_ps(ctx.v5.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v0.f32)));
	// vmulfp128 v4,v11,v0
	simde_mm_store_ps(ctx.v4.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v11.f32), simde_mm_load_ps(ctx.v0.f32)));
	// vnmsubfp v13,v6,v5,v10
	simde_mm_store_ps(ctx.v13.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(ctx.v6.f32), simde_mm_load_ps(ctx.v5.f32)), simde_mm_load_ps(ctx.v10.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
	// vmaddfp v0,v13,v4,v0
	simde_mm_store_ps(ctx.v0.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v4.f32)), simde_mm_load_ps(ctx.v0.f32)));
	// vmulfp128 v0,v9,v0
	simde_mm_store_ps(ctx.v0.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v9.f32), simde_mm_load_ps(ctx.v0.f32)));
	// addi r11,r1,144
	ctx.r11.s64 = ctx.r1.s64 + 144;
	// lfd f0,96(r1)
	ctx.fpscr.disableFlushModeUnconditional();
	ctx.f0.u64 = PPC_LOAD_U64(ctx.r1.u32 + 96);
	// fcfid f0,f0
	ctx.f0.f64 = double(ctx.f0.s64);
	// vpermwi128 v13,v0,99
	simde_mm_store_si128((simde__m128i*)ctx.v13.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)ctx.v0.u32), 0x9C));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// vpermwi128 v0,v0,135
	simde_mm_store_si128((simde__m128i*)ctx.v0.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)ctx.v0.u32), 0x78));
	// addi r11,r1,176
	ctx.r11.s64 = ctx.r1.s64 + 176;
	// frsp f1,f0
	ctx.f1.f64 = double(float(ctx.f0.f64));
	// vmulfp128 v13,v7,v13
	ctx.fpscr.enableFlushModeUnconditional();
	simde_mm_store_ps(ctx.v13.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v7.f32), simde_mm_load_ps(ctx.v13.f32)));
	// vnmsubfp v13,v8,v0,v13
	simde_mm_store_ps(ctx.v13.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(ctx.v8.f32), simde_mm_load_ps(ctx.v0.f32)), simde_mm_load_ps(ctx.v13.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
	// vmsum3fp128 v0,v13,v13
	simde_mm_store_ps(ctx.v0.f32, simde_mm_dp_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v13.f32), 0xEF));
	// stvx128 v13,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,176
	ctx.r11.s64 = ctx.r1.s64 + 176;
	// vor v9,v0,v0
	simde_mm_store_si128((simde__m128i*)ctx.v9.u8, simde_mm_load_si128((simde__m128i*)ctx.v0.u8));
	// vrsqrtefp v0,v0
	simde_mm_store_ps(ctx.v0.f32, simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(simde_mm_load_ps(ctx.v0.f32))));
	// vor v8,v0,v0
	simde_mm_store_si128((simde__m128i*)ctx.v8.u8, simde_mm_load_si128((simde__m128i*)ctx.v0.u8));
	// vcmpeqfp v0,v0,v12
	simde_mm_store_ps(ctx.v0.f32, simde_mm_cmpeq_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v12.f32)));
	// vsel v0,v8,v3,v0
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)ctx.v8.u8)), simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)ctx.v3.u8))));
	// vmulfp128 v12,v0,v0
	simde_mm_store_ps(ctx.v12.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v0.f32)));
	// vmulfp128 v11,v11,v0
	simde_mm_store_ps(ctx.v11.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v11.f32), simde_mm_load_ps(ctx.v0.f32)));
	// vnmsubfp v12,v9,v12,v10
	simde_mm_store_ps(ctx.v12.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(ctx.v9.f32), simde_mm_load_ps(ctx.v12.f32)), simde_mm_load_ps(ctx.v10.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
	// vmaddfp v0,v12,v11,v0
	simde_mm_store_ps(ctx.v0.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(ctx.v12.f32), simde_mm_load_ps(ctx.v11.f32)), simde_mm_load_ps(ctx.v0.f32)));
	// vmulfp128 v0,v13,v0
	simde_mm_store_ps(ctx.v0.f32, simde_mm_mul_ps(simde_mm_load_ps(ctx.v13.f32), simde_mm_load_ps(ctx.v0.f32)));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x821b27e0
	ctx.lr = 0x8259419C;
	sub_821B27E0(ctx, base);
	// addi r3,r1,208
	ctx.r3.s64 = ctx.r1.s64 + 208;
	// bl 0x821ab1c0
	ctx.lr = 0x825941A4;
	sub_821AB1C0(ctx, base);
	// addi r1,r1,320
	ctx.r1.s64 = ctx.r1.s64 + 320;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825941B8);  /* forward decl */
PPC_WEAK_FUNC(sub_825941B8) { __imp__sub_825941B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825941B8) {
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
	// mr r31,r4
	ctx.r31.u64 = ctx.r4.u64;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r30,r5
	ctx.r30.u64 = ctx.r5.u64;
	// bl 0x8219ea28
	ctx.lr = 0x825941DC;
	sub_8219EA28(ctx, base);
	// li r11,32
	ctx.r11.s64 = 32;
	// li r10,48
	ctx.r10.s64 = 48;
	// lvx128 v0,r3,r11
	ea = (ctx.r3.u32 + ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx128 v0,r0,r31
	ea = (ctx.r31.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lvx128 v0,r3,r10
	ea = (ctx.r3.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx128 v0,r0,r30
	ea = (ctx.r30.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
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

PPC_FUNC_IMPL(__imp__sub_82594210);  /* forward decl */
PPC_WEAK_FUNC(sub_82594210) { __imp__sub_82594210(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594210) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82594278
	if (ctx.cr6.eq) goto loc_82594278;
	// bl 0x8219f808
	ctx.lr = 0x8259422C;
	sub_8219F808(ctx, base);
	// bl 0x821ee130
	ctx.lr = 0x82594230;
	sub_821EE130(ctx, base);
	// bl 0x821d2088
	ctx.lr = 0x82594234;
	sub_821D2088(ctx, base);
	// lbz r11,133(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 133);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82594288
	if (!ctx.cr6.eq) goto loc_82594288;
	// bl 0x821d2088
	ctx.lr = 0x82594244;
	sub_821D2088(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,133(r3)
	PPC_STORE_U8(ctx.r3.u32 + 133, ctx.r11.u8);
	// bl 0x821d2088
	ctx.lr = 0x82594250;
	sub_821D2088(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// stb r11,134(r3)
	PPC_STORE_U8(ctx.r3.u32 + 134, ctx.r11.u8);
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// lwz r10,1772(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 1772);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// stw r10,1772(r11)
	PPC_STORE_U32(ctx.r11.u32 + 1772, ctx.r10.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_82594278:
	// bl 0x821d2088
	ctx.lr = 0x8259427C;
	sub_821D2088(ctx, base);
	// bl 0x825a77f8
	ctx.lr = 0x82594280;
	sub_825A77F8(ctx, base);
	// bl 0x8219f808
	ctx.lr = 0x82594284;
	sub_8219F808(ctx, base);
	// bl 0x821ee130
	ctx.lr = 0x82594288;
	sub_821EE130(ctx, base);
loc_82594288:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594298);  /* forward decl */
PPC_WEAK_FUNC(sub_82594298) { __imp__sub_82594298(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594298) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-368(r1)
	ea = -368 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// lis r10,-32061
	ctx.r10.s64 = -2101149696;
	// mr r31,r4
	ctx.r31.u64 = ctx.r4.u64;
	// addi r3,r10,12544
	ctx.r3.s64 = ctx.r10.s64 + 12544;
	// mr r4,r11
	ctx.r4.u64 = ctx.r11.u64;
	// bl 0x82273620
	ctx.lr = 0x825942C0;
	sub_82273620(ctx, base);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r10,r1,96
	ctx.r10.s64 = ctx.r1.s64 + 96;
	// li r9,-1
	ctx.r9.s64 = -1;
	// li r8,-1
	ctx.r8.s64 = -1;
	// li r7,-1
	ctx.r7.s64 = -1;
	// lfs f1,3068(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3068);
	ctx.f1.f64 = double(temp.f32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// li r6,-1
	ctx.r6.s64 = -1;
	// stw r10,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r10.u32);
	// li r5,-1
	ctx.r5.s64 = -1;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x8221dea0
	ctx.lr = 0x825942F4;
	sub_8221DEA0(ctx, base);
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// bl 0x821d2f50
	ctx.lr = 0x82594300;
	sub_821D2F50(ctx, base);
	// addi r1,r1,368
	ctx.r1.s64 = ctx.r1.s64 + 368;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594318);  /* forward decl */
PPC_WEAK_FUNC(sub_82594318) { __imp__sub_82594318(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594318) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-608(r1)
	ea = -608 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// lis r10,-32061
	ctx.r10.s64 = -2101149696;
	// mr r31,r4
	ctx.r31.u64 = ctx.r4.u64;
	// addi r3,r10,12544
	ctx.r3.s64 = ctx.r10.s64 + 12544;
	// mr r4,r11
	ctx.r4.u64 = ctx.r11.u64;
	// bl 0x82273620
	ctx.lr = 0x82594340;
	sub_82273620(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// li r5,256
	ctx.r5.s64 = 256;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8221dca0
	ctx.lr = 0x82594350;
	sub_8221DCA0(ctx, base);
	// li r5,256
	ctx.r5.s64 = 256;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// addi r3,r1,336
	ctx.r3.s64 = ctx.r1.s64 + 336;
	// bl 0x8221dd28
	ctx.lr = 0x82594360;
	sub_8221DD28(ctx, base);
	// li r9,-1
	ctx.r9.s64 = -1;
	// li r8,-1
	ctx.r8.s64 = -1;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r4,r1,336
	ctx.r4.s64 = ctx.r1.s64 + 336;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8221eba0
	ctx.lr = 0x82594380;
	sub_8221EBA0(ctx, base);
	// li r4,1
	ctx.r4.s64 = 1;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x821d2f50
	ctx.lr = 0x8259438C;
	sub_821D2F50(ctx, base);
	// addi r1,r1,608
	ctx.r1.s64 = ctx.r1.s64 + 608;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825943A0);  /* forward decl */
PPC_WEAK_FUNC(sub_825943A0) { __imp__sub_825943A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825943A0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// fmr f0,f4
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = ctx.f4.f64;
	// fmr f4,f5
	ctx.f4.f64 = ctx.f5.f64;
	// fcmpu cr6,f1,f0
	ctx.cr6.compare(ctx.f1.f64, ctx.f0.f64);
	// ble cr6,0x825943cc
	if (!ctx.cr6.gt) goto loc_825943CC;
	// fmr f13,f1
	ctx.f13.f64 = ctx.f1.f64;
	// fmr f1,f0
	ctx.f1.f64 = ctx.f0.f64;
	// fmr f0,f13
	ctx.f0.f64 = ctx.f13.f64;
loc_825943CC:
	// fcmpu cr6,f2,f4
	ctx.fpscr.disableFlushMode();
	ctx.cr6.compare(ctx.f2.f64, ctx.f4.f64);
	// ble cr6,0x825943e0
	if (!ctx.cr6.gt) goto loc_825943E0;
	// fmr f13,f2
	ctx.f13.f64 = ctx.f2.f64;
	// fmr f2,f4
	ctx.f2.f64 = ctx.f4.f64;
	// fmr f4,f13
	ctx.f4.f64 = ctx.f13.f64;
loc_825943E0:
	// fcmpu cr6,f3,f6
	ctx.fpscr.disableFlushMode();
	ctx.cr6.compare(ctx.f3.f64, ctx.f6.f64);
	// ble cr6,0x825943f4
	if (!ctx.cr6.gt) goto loc_825943F4;
	// fmr f13,f3
	ctx.f13.f64 = ctx.f3.f64;
	// fmr f3,f6
	ctx.f3.f64 = ctx.f6.f64;
	// fmr f6,f13
	ctx.f6.f64 = ctx.f13.f64;
loc_825943F4:
	// fmr f5,f3
	ctx.fpscr.disableFlushMode();
	ctx.f5.f64 = ctx.f3.f64;
	// li r31,0
	ctx.r31.s64 = 0;
	// fmr f3,f2
	ctx.f3.f64 = ctx.f2.f64;
	// fmr f2,f0
	ctx.f2.f64 = ctx.f0.f64;
	// bl 0x8250d018
	ctx.lr = 0x82594408;
	sub_8250D018(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// li r3,1
	ctx.r3.s64 = 1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8259441c
	if (!ctx.cr6.eq) goto loc_8259441C;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
loc_8259441C:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594430);  /* forward decl */
PPC_WEAK_FUNC(sub_82594430) { __imp__sub_82594430(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594430) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// fmr f0,f4
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = ctx.f4.f64;
	// fmr f4,f5
	ctx.f4.f64 = ctx.f5.f64;
	// fcmpu cr6,f1,f0
	ctx.cr6.compare(ctx.f1.f64, ctx.f0.f64);
	// ble cr6,0x8259445c
	if (!ctx.cr6.gt) goto loc_8259445C;
	// fmr f13,f1
	ctx.f13.f64 = ctx.f1.f64;
	// fmr f1,f0
	ctx.f1.f64 = ctx.f0.f64;
	// fmr f0,f13
	ctx.f0.f64 = ctx.f13.f64;
loc_8259445C:
	// fcmpu cr6,f2,f4
	ctx.fpscr.disableFlushMode();
	ctx.cr6.compare(ctx.f2.f64, ctx.f4.f64);
	// ble cr6,0x82594470
	if (!ctx.cr6.gt) goto loc_82594470;
	// fmr f13,f2
	ctx.f13.f64 = ctx.f2.f64;
	// fmr f2,f4
	ctx.f2.f64 = ctx.f4.f64;
	// fmr f4,f13
	ctx.f4.f64 = ctx.f13.f64;
loc_82594470:
	// fcmpu cr6,f3,f6
	ctx.fpscr.disableFlushMode();
	ctx.cr6.compare(ctx.f3.f64, ctx.f6.f64);
	// ble cr6,0x82594484
	if (!ctx.cr6.gt) goto loc_82594484;
	// fmr f13,f3
	ctx.f13.f64 = ctx.f3.f64;
	// fmr f3,f6
	ctx.f3.f64 = ctx.f6.f64;
	// fmr f6,f13
	ctx.f6.f64 = ctx.f13.f64;
loc_82594484:
	// fmr f5,f3
	ctx.fpscr.disableFlushMode();
	ctx.f5.f64 = ctx.f3.f64;
	// li r9,0
	ctx.r9.s64 = 0;
	// fmr f3,f2
	ctx.f3.f64 = ctx.f2.f64;
	// li r31,0
	ctx.r31.s64 = 0;
	// fmr f2,f0
	ctx.f2.f64 = ctx.f0.f64;
	// bl 0x823b6ae8
	ctx.lr = 0x8259449C;
	sub_823B6AE8(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// li r3,1
	ctx.r3.s64 = 1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825944b0
	if (!ctx.cr6.eq) goto loc_825944B0;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
loc_825944B0:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825944C8);  /* forward decl */
PPC_WEAK_FUNC(sub_825944C8) { __imp__sub_825944C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825944C8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// li r11,-999
	ctx.r11.s64 = -999;
	// li r9,0
	ctx.r9.s64 = 0;
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// bne cr6,0x82594508
	if (!ctx.cr6.eq) goto loc_82594508;
loc_825944F0:
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
loc_82594508:
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_82594510:
	// lbz r8,0(r11)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// cmplwi cr6,r8,0
	ctx.cr6.compare<uint32_t>(ctx.r8.u32, 0, ctx.xer);
	// bne cr6,0x82594510
	if (!ctx.cr6.eq) goto loc_82594510;
	// subf r11,r10,r11
	ctx.r11.u64 = ctx.r11.u64 - ctx.r10.u64;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// rotlwi r10,r11,0
	ctx.r10.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// beq cr6,0x825944f0
	if (ctx.cr6.eq) goto loc_825944F0;
	// lbz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 0);
	// extsb r11,r11
	ctx.r11.s64 = ctx.r11.s8;
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x82594580
	if (ctx.cr6.eq) goto loc_82594580;
loc_82594544:
	// cmpwi cr6,r11,48
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 48, ctx.xer);
	// blt cr6,0x82594554
	if (ctx.cr6.lt) goto loc_82594554;
	// cmpwi cr6,r11,57
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 57, ctx.xer);
	// ble cr6,0x8259456c
	if (!ctx.cr6.gt) goto loc_8259456C;
loc_82594554:
	// cmpwi cr6,r11,45
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 45, ctx.xer);
	// bne cr6,0x825944f0
	if (!ctx.cr6.eq) goto loc_825944F0;
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// bne cr6,0x825944f0
	if (!ctx.cr6.eq) goto loc_825944F0;
	// cmpwi cr6,r10,1
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 1, ctx.xer);
	// ble cr6,0x825944f0
	if (!ctx.cr6.gt) goto loc_825944F0;
loc_8259456C:
	// addi r9,r9,1
	ctx.r9.s64 = ctx.r9.s64 + 1;
	// lbzx r11,r9,r3
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r3.u32);
	// extsb r11,r11
	ctx.r11.s64 = ctx.r11.s8;
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// bne cr6,0x82594544
	if (!ctx.cr6.eq) goto loc_82594544;
loc_82594580:
	// bl 0x8298f378
	ctx.lr = 0x82594584;
	sub_8298F378(ctx, base);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// li r3,1
	ctx.r3.s64 = 1;
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

PPC_FUNC_IMPL(__imp__sub_825945A8);  /* forward decl */
PPC_WEAK_FUNC(sub_825945A8) { __imp__sub_825945A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825945A8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// li r9,1
	ctx.r9.s64 = 1;
	// addi r11,r11,-16004
	ctx.r11.s64 = ctx.r11.s64 + -16004;
	// mr r10,r4
	ctx.r10.u64 = ctx.r4.u64;
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// stb r9,0(r11)
	PPC_STORE_U8(ctx.r11.u32 + 0, ctx.r9.u8);
	// stw r3,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r3.u32);
	// bne cr6,0x825945d8
	if (!ctx.cr6.eq) goto loc_825945D8;
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r10,24(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24, ctx.r10.u32);
	// stb r10,8(r11)
	PPC_STORE_U8(ctx.r11.u32 + 8, ctx.r10.u8);
	// blr 
	return;
loc_825945D8:
	// stw r5,24(r11)
	PPC_STORE_U32(ctx.r11.u32 + 24, ctx.r5.u32);
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// subf r9,r10,r11
	ctx.r9.u64 = ctx.r11.u64 - ctx.r10.u64;
loc_825945E4:
	// lbz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// stbx r11,r9,r10
	PPC_STORE_U8(ctx.r9.u32 + ctx.r10.u32, ctx.r11.u8);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// bne cr6,0x825945e4
	if (!ctx.cr6.eq) goto loc_825945E4;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594600);  /* forward decl */
PPC_WEAK_FUNC(sub_82594600) { __imp__sub_82594600(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594600) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// std r3,224(r1)
	PPC_STORE_U64(ctx.r1.u32 + 224, ctx.r3.u64);
	// addi r11,r1,160
	ctx.r11.s64 = ctx.r1.s64 + 160;
	// std r4,232(r1)
	PPC_STORE_U64(ctx.r1.u32 + 232, ctx.r4.u64);
	// addi r3,r1,160
	ctx.r3.s64 = ctx.r1.s64 + 160;
	// std r5,240(r1)
	PPC_STORE_U64(ctx.r1.u32 + 240, ctx.r5.u64);
	// addi r4,r1,176
	ctx.r4.s64 = ctx.r1.s64 + 176;
	// std r6,248(r1)
	PPC_STORE_U64(ctx.r1.u32 + 248, ctx.r6.u64);
	// addi r5,r1,112
	ctx.r5.s64 = ctx.r1.s64 + 112;
	// std r7,256(r1)
	PPC_STORE_U64(ctx.r1.u32 + 256, ctx.r7.u64);
	// addi r6,r1,128
	ctx.r6.s64 = ctx.r1.s64 + 128;
	// std r8,264(r1)
	PPC_STORE_U64(ctx.r1.u32 + 264, ctx.r8.u64);
	// addi r7,r1,144
	ctx.r7.s64 = ctx.r1.s64 + 144;
	// std r9,272(r1)
	PPC_STORE_U64(ctx.r1.u32 + 272, ctx.r9.u64);
	// std r10,280(r1)
	PPC_STORE_U64(ctx.r1.u32 + 280, ctx.r10.u64);
	// lwz r8,308(r1)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r1.u32 + 308);
	// lfs f0,224(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 224);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// lfs f0,228(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 228);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,84(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// lfs f0,232(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 232);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// addi r10,r1,80
	ctx.r10.s64 = ctx.r1.s64 + 80;
	// lfs f0,240(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 240);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,96(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 96, temp.u32);
	// lfs f0,244(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 244);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,100(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 100, temp.u32);
	// lfs f0,248(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 248);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,104(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 104, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,96
	ctx.r10.s64 = ctx.r1.s64 + 96;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,176
	ctx.r11.s64 = ctx.r1.s64 + 176;
	// lfs f0,256(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 256);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,112(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 112, temp.u32);
	// lfs f0,260(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 260);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,116(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 116, temp.u32);
	// lfs f0,264(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 264);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,120(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 120, temp.u32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r10,r1,112
	ctx.r10.s64 = ctx.r1.s64 + 112;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,112
	ctx.r11.s64 = ctx.r1.s64 + 112;
	// lfs f0,272(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 272);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,128(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 128, temp.u32);
	// lfs f0,276(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 276);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,132(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 132, temp.u32);
	// lfs f0,280(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 280);
	ctx.f0.f64 = double(temp.f32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stfs f0,136(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 136, temp.u32);
	// addi r10,r1,128
	ctx.r10.s64 = ctx.r1.s64 + 128;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,128
	ctx.r11.s64 = ctx.r1.s64 + 128;
	// lfs f0,288(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 288);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,144(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 144, temp.u32);
	// lfs f0,292(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 292);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,148(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 148, temp.u32);
	// lfs f0,296(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 296);
	ctx.f0.f64 = double(temp.f32);
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stfs f0,152(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 152, temp.u32);
	// addi r10,r1,144
	ctx.r10.s64 = ctx.r1.s64 + 144;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,144
	ctx.r11.s64 = ctx.r1.s64 + 144;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x8225d690
	ctx.lr = 0x82594710;
	sub_8225D690(ctx, base);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594720);  /* forward decl */
PPC_WEAK_FUNC(sub_82594720) { __imp__sub_82594720(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594720) {
	PPC_FUNC_PROLOGUE();
	// b 0x823ec960
	sub_823EC960(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594728);  /* forward decl */
PPC_WEAK_FUNC(sub_82594728) { __imp__sub_82594728(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594728) {
	PPC_FUNC_PROLOGUE();
	// b 0x82593dc0
	sub_82593DC0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594730);  /* forward decl */
PPC_WEAK_FUNC(sub_82594730) { __imp__sub_82594730(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594730) {
	PPC_FUNC_PROLOGUE();
	// b 0x82593e60
	sub_82593E60(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594738);  /* forward decl */
PPC_WEAK_FUNC(sub_82594738) { __imp__sub_82594738(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594738) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// lbz r11,-14262(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + -14262);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// li r3,2
	ctx.r3.s64 = 2;
	// b 0x8224b980
	sub_8224B980(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594750);  /* forward decl */
PPC_WEAK_FUNC(sub_82594750) { __imp__sub_82594750(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594750) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594758);  /* forward decl */
PPC_WEAK_FUNC(sub_82594758) { __imp__sub_82594758(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594758) {
	PPC_FUNC_PROLOGUE();
	// b 0x821b8a38
	sub_821B8A38(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594760);  /* forward decl */
PPC_WEAK_FUNC(sub_82594760) { __imp__sub_82594760(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594760) {
	PPC_FUNC_PROLOGUE();
	// b 0x821b8528
	sub_821B8528(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594768);  /* forward decl */
PPC_WEAK_FUNC(sub_82594768) { __imp__sub_82594768(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594768) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// bl 0x825a7c30
	ctx.lr = 0x82594778;
	sub_825A7C30(ctx, base);
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// addi r3,r11,-6248
	ctx.r3.s64 = ctx.r11.s64 + -6248;
	// bl 0x82396160
	ctx.lr = 0x82594784;
	sub_82396160(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594798);  /* forward decl */
PPC_WEAK_FUNC(sub_82594798) { __imp__sub_82594798(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594798) {
	PPC_FUNC_PROLOGUE();
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
	ctx.lr = 0x825947A8;
	sub_821D2088(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,129(r3)
	PPC_STORE_U8(ctx.r3.u32 + 129, ctx.r11.u8);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825947C0);  /* forward decl */
PPC_WEAK_FUNC(sub_825947C0) { __imp__sub_825947C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825947C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32039
	ctx.r11.s64 = -2099707904;
	// lwz r3,15072(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 15072);
	// b 0x82308248
	sub_82308248(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825947D0);  /* forward decl */
PPC_WEAK_FUNC(sub_825947D0) { __imp__sub_825947D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825947D0) {
	PPC_FUNC_PROLOGUE();
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
	ctx.lr = 0x825947E0;
	sub_821D2088(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,130(r3)
	PPC_STORE_U8(ctx.r3.u32 + 130, ctx.r11.u8);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825947F8);  /* forward decl */
PPC_WEAK_FUNC(sub_825947F8) { __imp__sub_825947F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825947F8) {
	PPC_FUNC_PROLOGUE();
	// li r3,0
	ctx.r3.s64 = 0;
	// b 0x828e0ab8
	sub_828E0AB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594800);  /* forward decl */
PPC_WEAK_FUNC(sub_82594800) { __imp__sub_82594800(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594800) {
	PPC_FUNC_PROLOGUE();
	// b 0x82271e60
	sub_82271E60(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594808);  /* forward decl */
PPC_WEAK_FUNC(sub_82594808) { __imp__sub_82594808(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594808) {
	PPC_FUNC_PROLOGUE();
	// li r3,-1
	ctx.r3.s64 = -1;
	// b 0x821e4730
	sub_821E4730(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594810);  /* forward decl */
PPC_WEAK_FUNC(sub_82594810) { __imp__sub_82594810(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594810) {
	PPC_FUNC_PROLOGUE();
	// b 0x82538928
	sub_82538928(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594818);  /* forward decl */
PPC_WEAK_FUNC(sub_82594818) { __imp__sub_82594818(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594818) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// bl 0x822860d8
	ctx.lr = 0x82594828;
	sub_822860D8(ctx, base);
	// bl 0x821d2088
	ctx.lr = 0x8259482C;
	sub_821D2088(ctx, base);
	// bl 0x825a7cb8
	ctx.lr = 0x82594830;
	sub_825A7CB8(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594840);  /* forward decl */
PPC_WEAK_FUNC(sub_82594840) { __imp__sub_82594840(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594840) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// bl 0x822860e8
	ctx.lr = 0x82594850;
	sub_822860E8(ctx, base);
	// bl 0x825a7d38
	ctx.lr = 0x82594854;
	sub_825A7D38(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594868);  /* forward decl */
PPC_WEAK_FUNC(sub_82594868) { __imp__sub_82594868(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594868) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// bl 0x82329880
	ctx.lr = 0x82594878;
	sub_82329880(ctx, base);
	// bl 0x8238acd0
	ctx.lr = 0x8259487C;
	sub_8238ACD0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594890);  /* forward decl */
PPC_WEAK_FUNC(sub_82594890) { __imp__sub_82594890(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594890) {
	PPC_FUNC_PROLOGUE();
	// b 0x821ea530
	sub_821EA530(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594898);  /* forward decl */
PPC_WEAK_FUNC(sub_82594898) { __imp__sub_82594898(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594898) {
	PPC_FUNC_PROLOGUE();
	// b 0x823f44f0
	sub_823F44F0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825948A0);  /* forward decl */
PPC_WEAK_FUNC(sub_825948A0) { __imp__sub_825948A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825948A0) {
	PPC_FUNC_PROLOGUE();
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
	ctx.lr = 0x825948B0;
	sub_821D2088(ctx, base);
	// lbz r11,128(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 128);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825948c8
	if (ctx.cr6.eq) goto loc_825948C8;
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// addi r3,r11,-23280
	ctx.r3.s64 = ctx.r11.s64 + -23280;
	// bl 0x821d2d78
	ctx.lr = 0x825948C8;
	sub_821D2D78(ctx, base);
loc_825948C8:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825948D8);  /* forward decl */
PPC_WEAK_FUNC(sub_825948D8) { __imp__sub_825948D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825948D8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32048
	ctx.r11.s64 = -2100297728;
	// li r10,1
	ctx.r10.s64 = 1;
	// lwz r11,-25100(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -25100);
	// stb r10,12(r11)
	PPC_STORE_U8(ctx.r11.u32 + 12, ctx.r10.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825948F0);  /* forward decl */
PPC_WEAK_FUNC(sub_825948F0) { __imp__sub_825948F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825948F0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32055
	ctx.r11.s64 = -2100756480;
	// addi r3,r11,-27888
	ctx.r3.s64 = ctx.r11.s64 + -27888;
	// b 0x8228b0a0
	sub_8228B0A0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594900);  /* forward decl */
PPC_WEAK_FUNC(sub_82594900) { __imp__sub_82594900(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594900) {
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
	// mr r31,r4
	ctx.r31.u64 = ctx.r4.u64;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
	// lwz r9,24(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// cntlzw r9,r9
	ctx.r9.u64 = ctx.r9.u32 == 0 ? 32 : __builtin_clz(ctx.r9.u32);
	// lfs f3,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f3.f64 = double(temp.f32);
	// lfs f4,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f4.f64 = double(temp.f32);
	// lwz r7,36(r10)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r10.u32 + 36);
	// rlwinm r9,r9,27,31,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// lfs f5,16(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f5.f64 = double(temp.f32);
	// lfs f6,20(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	ctx.f6.f64 = double(temp.f32);
	// lwz r8,32(r10)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 32);
	// xori r11,r9,1
	ctx.r11.u64 = ctx.r9.u64 ^ 1;
	// lwz r9,28(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 28);
	// lwz r6,40(r10)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r10.u32 + 40);
	// cntlzw r10,r7
	ctx.r10.u64 = ctx.r7.u32 == 0 ? 32 : __builtin_clz(ctx.r7.u32);
	// cntlzw r9,r9
	ctx.r9.u64 = ctx.r9.u32 == 0 ? 32 : __builtin_clz(ctx.r9.u32);
	// rlwinm r7,r10,27,31,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// rlwinm r9,r9,27,31,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// cntlzw r8,r8
	ctx.r8.u64 = ctx.r8.u32 == 0 ? 32 : __builtin_clz(ctx.r8.u32);
	// cntlzw r6,r6
	ctx.r6.u64 = ctx.r6.u32 == 0 ? 32 : __builtin_clz(ctx.r6.u32);
	// xori r10,r9,1
	ctx.r10.u64 = ctx.r9.u64 ^ 1;
	// clrlwi r9,r11,24
	ctx.r9.u64 = ctx.r11.u32 & 0xFF;
	// rlwinm r8,r8,27,31,31
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 27) & 0x1;
	// rlwinm r11,r6,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 27) & 0x1;
	// xori r8,r8,1
	ctx.r8.u64 = ctx.r8.u64 ^ 1;
	// xori r7,r7,1
	ctx.r7.u64 = ctx.r7.u64 ^ 1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// stb r8,87(r1)
	PPC_STORE_U8(ctx.r1.u32 + 87, ctx.r8.u8);
	// stb r7,95(r1)
	PPC_STORE_U8(ctx.r1.u32 + 95, ctx.r7.u8);
	// stb r11,103(r1)
	PPC_STORE_U8(ctx.r1.u32 + 103, ctx.r11.u8);
	// mtctr r3
	ctx.ctr.u64 = ctx.r3.u64;
	// bctrl 
	ctx.lr = 0x825949A0;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
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

PPC_FUNC_IMPL(__imp__sub_825949C0);  /* forward decl */
PPC_WEAK_FUNC(sub_825949C0) { __imp__sub_825949C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825949C0) {
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
	// fcmpu cr6,f1,f4
	ctx.fpscr.disableFlushMode();
	ctx.cr6.compare(ctx.f1.f64, ctx.f4.f64);
	// ble cr6,0x825949e0
	if (!ctx.cr6.gt) goto loc_825949E0;
	// fmr f0,f1
	ctx.f0.f64 = ctx.f1.f64;
	// fmr f1,f4
	ctx.f1.f64 = ctx.f4.f64;
	// fmr f4,f0
	ctx.f4.f64 = ctx.f0.f64;
loc_825949E0:
	// fcmpu cr6,f2,f5
	ctx.fpscr.disableFlushMode();
	ctx.cr6.compare(ctx.f2.f64, ctx.f5.f64);
	// ble cr6,0x825949f4
	if (!ctx.cr6.gt) goto loc_825949F4;
	// fmr f0,f2
	ctx.f0.f64 = ctx.f2.f64;
	// fmr f2,f5
	ctx.f2.f64 = ctx.f5.f64;
	// fmr f5,f0
	ctx.f5.f64 = ctx.f0.f64;
loc_825949F4:
	// fcmpu cr6,f3,f6
	ctx.fpscr.disableFlushMode();
	ctx.cr6.compare(ctx.f3.f64, ctx.f6.f64);
	// ble cr6,0x82594a08
	if (!ctx.cr6.gt) goto loc_82594A08;
	// fmr f0,f3
	ctx.f0.f64 = ctx.f3.f64;
	// fmr f3,f6
	ctx.f3.f64 = ctx.f6.f64;
	// fmr f6,f0
	ctx.f6.f64 = ctx.f0.f64;
loc_82594A08:
	// clrlwi r11,r9,24
	ctx.r11.u64 = ctx.r9.u32 & 0xFF;
	// stfs f1,96(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r1.u32 + 96, temp.u32);
	// stfs f2,100(r1)
	temp.f32 = float(ctx.f2.f64);
	PPC_STORE_U32(ctx.r1.u32 + 100, temp.u32);
	// li r6,0
	ctx.r6.s64 = 0;
	// stfs f3,104(r1)
	temp.f32 = float(ctx.f3.f64);
	PPC_STORE_U32(ctx.r1.u32 + 104, temp.u32);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// addi r11,r1,96
	ctx.r11.s64 = ctx.r1.s64 + 96;
	// stfs f4,128(r1)
	temp.f32 = float(ctx.f4.f64);
	PPC_STORE_U32(ctx.r1.u32 + 128, temp.u32);
	// stfs f5,132(r1)
	temp.f32 = float(ctx.f5.f64);
	PPC_STORE_U32(ctx.r1.u32 + 132, temp.u32);
	// li r9,2
	ctx.r9.s64 = 2;
	// stfs f6,136(r1)
	temp.f32 = float(ctx.f6.f64);
	PPC_STORE_U32(ctx.r1.u32 + 136, temp.u32);
	// stw r6,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r6.u32);
	// stw r6,88(r1)
	PPC_STORE_U32(ctx.r1.u32 + 88, ctx.r6.u32);
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,96
	ctx.r11.s64 = ctx.r1.s64 + 96;
	// stw r9,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r9.u32);
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,128
	ctx.r11.s64 = ctx.r1.s64 + 128;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,112
	ctx.r11.s64 = ctx.r1.s64 + 112;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// beq cr6,0x82594a64
	if (ctx.cr6.eq) goto loc_82594A64;
	// li r6,2
	ctx.r6.s64 = 2;
loc_82594A64:
	// clrlwi r11,r10,24
	ctx.r11.u64 = ctx.r10.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82594a74
	if (ctx.cr6.eq) goto loc_82594A74;
	// ori r6,r6,4
	ctx.r6.u64 = ctx.r6.u64 | 4;
loc_82594A74:
	// lbz r11,247(r1)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r1.u32 + 247);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82594a84
	if (ctx.cr6.eq) goto loc_82594A84;
	// ori r6,r6,8
	ctx.r6.u64 = ctx.r6.u64 | 8;
loc_82594A84:
	// lbz r11,255(r1)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r1.u32 + 255);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82594a94
	if (ctx.cr6.eq) goto loc_82594A94;
	// ori r6,r6,16
	ctx.r6.u64 = ctx.r6.u64 | 16;
loc_82594A94:
	// lbz r11,263(r1)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r1.u32 + 263);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82594aa4
	if (ctx.cr6.eq) goto loc_82594AA4;
	// ori r6,r6,32
	ctx.r6.u64 = ctx.r6.u64 | 32;
loc_82594AA4:
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// li r7,13
	ctx.r7.s64 = 13;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r4,r11,15320
	ctx.r4.s64 = ctx.r11.s64 + 15320;
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// bl 0x82283c48
	ctx.lr = 0x82594ABC;
	sub_82283C48(ctx, base);
	// lwz r11,80(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// li r3,1
	ctx.r3.s64 = 1;
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// bgt cr6,0x82594ad0
	if (ctx.cr6.gt) goto loc_82594AD0;
	// li r3,0
	ctx.r3.s64 = 0;
loc_82594AD0:
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594AE0);  /* forward decl */
PPC_WEAK_FUNC(sub_82594AE0) { __imp__sub_82594AE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594AE0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-176(r1)
	ea = -176 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// fsubs f0,f1,f4
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(float(ctx.f1.f64 - ctx.f4.f64));
	// fadds f13,f1,f4
	ctx.f13.f64 = double(float(ctx.f1.f64 + ctx.f4.f64));
	// fsubs f10,f2,f5
	ctx.f10.f64 = double(float(ctx.f2.f64 - ctx.f5.f64));
	// fadds f9,f2,f5
	ctx.f9.f64 = double(float(ctx.f2.f64 + ctx.f5.f64));
	// fsubs f12,f3,f6
	ctx.f12.f64 = double(float(ctx.f3.f64 - ctx.f6.f64));
	// fadds f11,f3,f6
	ctx.f11.f64 = double(float(ctx.f3.f64 + ctx.f6.f64));
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// ble cr6,0x82594b18
	if (!ctx.cr6.gt) goto loc_82594B18;
	// fmr f8,f0
	ctx.f8.f64 = ctx.f0.f64;
	// fmr f0,f13
	ctx.f0.f64 = ctx.f13.f64;
	// fmr f13,f8
	ctx.f13.f64 = ctx.f8.f64;
loc_82594B18:
	// fcmpu cr6,f10,f9
	ctx.fpscr.disableFlushMode();
	ctx.cr6.compare(ctx.f10.f64, ctx.f9.f64);
	// ble cr6,0x82594b2c
	if (!ctx.cr6.gt) goto loc_82594B2C;
	// fmr f8,f10
	ctx.f8.f64 = ctx.f10.f64;
	// fmr f10,f9
	ctx.f10.f64 = ctx.f9.f64;
	// fmr f9,f8
	ctx.f9.f64 = ctx.f8.f64;
loc_82594B2C:
	// fcmpu cr6,f12,f11
	ctx.fpscr.disableFlushMode();
	ctx.cr6.compare(ctx.f12.f64, ctx.f11.f64);
	// ble cr6,0x82594b40
	if (!ctx.cr6.gt) goto loc_82594B40;
	// fmr f8,f12
	ctx.f8.f64 = ctx.f12.f64;
	// fmr f12,f11
	ctx.f12.f64 = ctx.f11.f64;
	// fmr f11,f8
	ctx.f11.f64 = ctx.f8.f64;
loc_82594B40:
	// lis r11,-32227
	ctx.r11.s64 = -2112028672;
	// stfs f0,96(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 96, temp.u32);
	// stfs f10,100(r1)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r1.u32 + 100, temp.u32);
	// li r7,13
	ctx.r7.s64 = 13;
	// addi r4,r11,6152
	ctx.r4.s64 = ctx.r11.s64 + 6152;
	// stfs f12,104(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 104, temp.u32);
	// li r11,0
	ctx.r11.s64 = 0;
	// stfs f13,112(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 112, temp.u32);
	// stfs f9,116(r1)
	temp.f32 = float(ctx.f9.f64);
	PPC_STORE_U32(ctx.r1.u32 + 116, temp.u32);
	// li r6,28
	ctx.r6.s64 = 28;
	// stfs f11,120(r1)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r1.u32 + 120, temp.u32);
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r3,r1,128
	ctx.r3.s64 = ctx.r1.s64 + 128;
	// stw r11,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r11.u32);
	// addi r11,r1,96
	ctx.r11.s64 = ctx.r1.s64 + 96;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,128
	ctx.r11.s64 = ctx.r1.s64 + 128;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,112
	ctx.r11.s64 = ctx.r1.s64 + 112;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,144
	ctx.r11.s64 = ctx.r1.s64 + 144;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x82283c48
	ctx.lr = 0x82594B9C;
	sub_82283C48(ctx, base);
	// lwz r11,80(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// li r3,1
	ctx.r3.s64 = 1;
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// bgt cr6,0x82594bb0
	if (ctx.cr6.gt) goto loc_82594BB0;
	// li r3,0
	ctx.r3.s64 = 0;
loc_82594BB0:
	// addi r1,r1,176
	ctx.r1.s64 = ctx.r1.s64 + 176;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594BC0);  /* forward decl */
PPC_WEAK_FUNC(sub_82594BC0) { __imp__sub_82594BC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594BC0) {
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
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// std r4,192(r1)
	PPC_STORE_U64(ctx.r1.u32 + 192, ctx.r4.u64);
	// lis r11,-32015
	ctx.r11.s64 = -2098135040;
	// std r5,200(r1)
	PPC_STORE_U64(ctx.r1.u32 + 200, ctx.r5.u64);
	// srawi r10,r3,8
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r3.s32 >> 8;
	// clrlwi r8,r3,24
	ctx.r8.u64 = ctx.r3.u32 & 0xFF;
	// mr r30,r6
	ctx.r30.u64 = ctx.r6.u64;
	// lwz r11,-3324(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -3324);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// lfs f0,192(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 192);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// lfs f0,196(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 196);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,84(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// lfs f0,200(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 200);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// addi r7,r1,80
	ctx.r7.s64 = ctx.r1.s64 + 80;
	// lvx128 v0,r0,r7
	ea = (ctx.r7.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r7,r1,96
	ctx.r7.s64 = ctx.r1.s64 + 96;
	// stvx128 v0,r0,r7
	ea = (ctx.r7.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bne cr6,0x82594c9c
	if (!ctx.cr6.eq) goto loc_82594C9C;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r31,r11,r9
	ctx.r31.u64 = ctx.r11.u64 + ctx.r9.u64;
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// beq cr6,0x82594c9c
	if (ctx.cr6.eq) goto loc_82594C9C;
	// lfs f0,300(r31)
	temp.u32 = PPC_LOAD_U32(ctx.r31.u32 + 300);
	ctx.f0.f64 = double(temp.f32);
	// addi r11,r1,112
	ctx.r11.s64 = ctx.r1.s64 + 112;
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// addi r5,r1,96
	ctx.r5.s64 = ctx.r1.s64 + 96;
	// lfs f0,304(r31)
	temp.u32 = PPC_LOAD_U32(ctx.r31.u32 + 304);
	ctx.f0.f64 = double(temp.f32);
	// addi r4,r1,112
	ctx.r4.s64 = ctx.r1.s64 + 112;
	// stfs f0,84(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// lfs f0,308(r31)
	temp.u32 = PPC_LOAD_U32(ctx.r31.u32 + 308);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// lfs f0,312(r31)
	temp.u32 = PPC_LOAD_U32(ctx.r31.u32 + 312);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,92(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 92, temp.u32);
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x823d9d40
	ctx.lr = 0x82594C78;
	sub_823D9D40(ctx, base);
	// lwz r11,32(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 32);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// addi r11,r11,48
	ctx.r11.s64 = ctx.r11.s64 + 48;
	// bne cr6,0x82594c8c
	if (!ctx.cr6.eq) goto loc_82594C8C;
	// addi r11,r31,16
	ctx.r11.s64 = ctx.r31.s64 + 16;
loc_82594C8C:
	// addi r10,r1,96
	ctx.r10.s64 = ctx.r1.s64 + 96;
	// lvx128 v13,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// vaddfp v0,v0,v13
	ctx.fpscr.enableFlushMode();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_add_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v13.f32)));
loc_82594C9C:
	// stvx128 v0,r0,r30
	ea = (ctx.r30.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
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

PPC_FUNC_IMPL(__imp__sub_82594CB8);  /* forward decl */
PPC_WEAK_FUNC(sub_82594CB8) { __imp__sub_82594CB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594CB8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-176(r1)
	ea = -176 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32015
	ctx.r11.s64 = -2098135040;
	// srawi r10,r3,8
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r3.s32 >> 8;
	// clrlwi r8,r3,24
	ctx.r8.u64 = ctx.r3.u32 & 0xFF;
	// mr r31,r4
	ctx.r31.u64 = ctx.r4.u64;
	// lwz r11,-3324(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -3324);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x82594d88
	if (!ctx.cr6.eq) goto loc_82594D88;
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
	// beq cr6,0x82594d88
	if (ctx.cr6.eq) goto loc_82594D88;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// lfs f0,312(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 312);
	ctx.f0.f64 = double(temp.f32);
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// lfs f13,308(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 308);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,304(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 304);
	ctx.f12.f64 = double(temp.f32);
	// lfs f11,300(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 300);
	ctx.f11.f64 = double(temp.f32);
	// stfs f11,80(r1)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f0,92(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 92, temp.u32);
	// stfs f13,88(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// stfs f12,84(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// bl 0x827eef10
	ctx.lr = 0x82594D30;
	sub_827EEF10(ctx, base);
	// lfs f0,112(r1)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 112);
	ctx.f0.f64 = double(temp.f32);
	// lfs f2,116(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + 116);
	ctx.f2.f64 = double(temp.f32);
	// fneg f1,f0
	ctx.f1.u64 = ctx.f0.u64 ^ 0x8000000000000000;
	// bl 0x8298fd78
	ctx.lr = 0x82594D40;
	sub_8298FD78(ctx, base);
	// frsp f13,f1
	ctx.fpscr.disableFlushMode();
	ctx.f13.f64 = double(float(ctx.f1.f64));
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// lfs f0,-23712(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -23712);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// fmuls f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
	// stfs f0,0(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r31.u32 + 0, temp.u32);
	// lfs f13,3068(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3068);
	ctx.f13.f64 = double(temp.f32);
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// lfs f13,-22820(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -22820);
	ctx.f13.f64 = double(temp.f32);
	// bge cr6,0x82594d74
	if (!ctx.cr6.lt) goto loc_82594D74;
	// fadds f0,f0,f13
	ctx.f0.f64 = double(float(ctx.f0.f64 + ctx.f13.f64));
	// stfs f0,0(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r31.u32 + 0, temp.u32);
loc_82594D74:
	// lfs f0,0(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// ble cr6,0x82594d88
	if (!ctx.cr6.gt) goto loc_82594D88;
	// fsubs f0,f0,f13
	ctx.f0.f64 = double(float(ctx.f0.f64 - ctx.f13.f64));
	// stfs f0,0(r31)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r31.u32 + 0, temp.u32);
loc_82594D88:
	// addi r1,r1,176
	ctx.r1.s64 = ctx.r1.s64 + 176;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594DA0);  /* forward decl */
PPC_WEAK_FUNC(sub_82594DA0) { __imp__sub_82594DA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594DA0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r10,-32093
	ctx.r10.s64 = -2103246848;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r9,0
	ctx.r9.s64 = 0;
	// addi r10,r10,-5984
	ctx.r10.s64 = ctx.r10.s64 + -5984;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stb r9,8(r10)
	PPC_STORE_U8(ctx.r10.u32 + 8, ctx.r9.u8);
	// lis r9,-32256
	ctx.r9.s64 = -2113929216;
	// rlwinm r8,r11,16,16,31
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 16) & 0xFFFF;
	// rlwinm r7,r11,16,0,15
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 16) & 0xFFFF0000;
	// or r8,r8,r7
	ctx.r8.u64 = ctx.r8.u64 | ctx.r7.u64;
	// lfs f0,3068(r9)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + 3068);
	ctx.f0.f64 = double(temp.f32);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// stfs f0,12(r10)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + 12, temp.u32);
	// rlwinm r9,r9,27,31,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// add r9,r9,r11
	ctx.r9.u64 = ctx.r9.u64 + ctx.r11.u64;
	// xor r11,r8,r11
	ctx.r11.u64 = ctx.r8.u64 ^ ctx.r11.u64;
	// stw r9,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r9.u32);
	// stw r11,4(r10)
	PPC_STORE_U32(ctx.r10.u32 + 4, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594DF0);  /* forward decl */
PPC_WEAK_FUNC(sub_82594DF0) { __imp__sub_82594DF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594DF0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// lfs f0,0(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,-12328(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -12328, temp.u32);
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// stfs f0,-12332(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -12332, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594E10);  /* forward decl */
PPC_WEAK_FUNC(sub_82594E10) { __imp__sub_82594E10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594E10) {
	PPC_FUNC_PROLOGUE();
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
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82594e54
	if (ctx.cr6.eq) goto loc_82594E54;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-16082(r10)
	PPC_STORE_U8(ctx.r10.u32 + -16082, ctx.r11.u8);
	// bl 0x821d2088
	ctx.lr = 0x82594E4C;
	sub_821D2088(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,128(r3)
	PPC_STORE_U8(ctx.r3.u32 + 128, ctx.r11.u8);
loc_82594E54:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594E68);  /* forward decl */
PPC_WEAK_FUNC(sub_82594E68) { __imp__sub_82594E68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594E68) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x821d1cc8
	ctx.lr = 0x82594E80;
	sub_821D1CC8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82594EA0);  /* forward decl */
PPC_WEAK_FUNC(sub_82594EA0) { __imp__sub_82594EA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594EA0) {
	PPC_FUNC_PROLOGUE();
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
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// extsh r3,r11
	ctx.r3.s64 = ctx.r11.s16;
	// bl 0x823ec8c0
	ctx.lr = 0x82594EBC;
	sub_823EC8C0(ctx, base);
	// bl 0x826745d8
	ctx.lr = 0x82594EC0;
	sub_826745D8(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82594efc
	if (ctx.cr6.eq) goto loc_82594EFC;
	// bl 0x82674650
	ctx.lr = 0x82594ED0;
	sub_82674650(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82594efc
	if (ctx.cr6.eq) goto loc_82594EFC;
	// lis r11,-32048
	ctx.r11.s64 = -2100297728;
	// lbz r11,-23916(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + -23916);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82594efc
	if (ctx.cr6.eq) goto loc_82594EFC;
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,22588
	ctx.r3.s64 = ctx.r11.s64 + 22588;
	// bl 0x8268f6c0
	ctx.lr = 0x82594EFC;
	sub_8268F6C0(ctx, base);
loc_82594EFC:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594F10);  /* forward decl */
PPC_WEAK_FUNC(sub_82594F10) { __imp__sub_82594F10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594F10) {
	PPC_FUNC_PROLOGUE();
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
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// extsh r3,r11
	ctx.r3.s64 = ctx.r11.s16;
	// bl 0x823ec8d0
	ctx.lr = 0x82594F2C;
	sub_823EC8D0(ctx, base);
	// bl 0x826745d8
	ctx.lr = 0x82594F30;
	sub_826745D8(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82594f6c
	if (ctx.cr6.eq) goto loc_82594F6C;
	// bl 0x82674650
	ctx.lr = 0x82594F40;
	sub_82674650(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82594f6c
	if (ctx.cr6.eq) goto loc_82594F6C;
	// lis r11,-32048
	ctx.r11.s64 = -2100297728;
	// lbz r11,-23916(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + -23916);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82594f6c
	if (ctx.cr6.eq) goto loc_82594F6C;
	// lis r11,-31992
	ctx.r11.s64 = -2096627712;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r11,22588
	ctx.r3.s64 = ctx.r11.s64 + 22588;
	// bl 0x8268f6c0
	ctx.lr = 0x82594F6C;
	sub_8268F6C0(ctx, base);
loc_82594F6C:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594F80);  /* forward decl */
PPC_WEAK_FUNC(sub_82594F80) { __imp__sub_82594F80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594F80) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32015
	ctx.r11.s64 = -2098135040;
	// lfs f0,0(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,19660(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 19660, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594F98);  /* forward decl */
PPC_WEAK_FUNC(sub_82594F98) { __imp__sub_82594F98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594F98) {
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
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x829914d0
	ctx.lr = 0x82594FB8;
	sub_829914D0(ctx, base);
	// frsp f13,f1
	ctx.fpscr.disableFlushMode();
	ctx.f13.f64 = double(float(ctx.f1.f64));
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// lwz r10,0(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// lfs f0,-23712(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -23712);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
	// stfs f0,0(r10)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82594FE8);  /* forward decl */
PPC_WEAK_FUNC(sub_82594FE8) { __imp__sub_82594FE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82594FE8) {
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
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x82991598
	ctx.lr = 0x82595008;
	sub_82991598(ctx, base);
	// frsp f13,f1
	ctx.fpscr.disableFlushMode();
	ctx.f13.f64 = double(float(ctx.f1.f64));
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// lwz r10,0(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// lfs f0,-23712(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -23712);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
	// stfs f0,0(r10)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595038);  /* forward decl */
PPC_WEAK_FUNC(sub_82595038) { __imp__sub_82595038(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595038) {
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
	// lfs f0,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lfs f13,-6516(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -6516);
	ctx.f13.f64 = double(temp.f32);
	// fmuls f1,f0,f13
	ctx.f1.f64 = double(float(ctx.f0.f64 * ctx.f13.f64));
	// bl 0x82991e50
	ctx.lr = 0x82595064;
	sub_82991E50(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// frsp f0,f1
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(float(ctx.f1.f64));
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595088);  /* forward decl */
PPC_WEAK_FUNC(sub_82595088) { __imp__sub_82595088(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595088) {
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
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lfd f2,-6912(r11)
	ctx.fpscr.disableFlushMode();
	ctx.f2.u64 = PPC_LOAD_U64(ctx.r11.u32 + -6912);
	// lfs f1,0(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x8298fd78
	ctx.lr = 0x825950B0;
	sub_8298FD78(ctx, base);
	// frsp f13,f1
	ctx.fpscr.disableFlushMode();
	ctx.f13.f64 = double(float(ctx.f1.f64));
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// lwz r10,0(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// lfs f0,-23712(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -23712);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
	// stfs f0,0(r10)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825950E0);  /* forward decl */
PPC_WEAK_FUNC(sub_825950E0) { __imp__sub_825950E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825950E0) {
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
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// bl 0x8298fd78
	ctx.lr = 0x82595104;
	sub_8298FD78(ctx, base);
	// frsp f13,f1
	ctx.fpscr.disableFlushMode();
	ctx.f13.f64 = double(float(ctx.f1.f64));
	// lis r11,-32255
	ctx.r11.s64 = -2113863680;
	// lwz r10,0(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// lfs f0,-23712(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + -23712);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
	// stfs f0,0(r10)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595130);  /* forward decl */
PPC_WEAK_FUNC(sub_82595130) { __imp__sub_82595130(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595130) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,18880
	ctx.r3.s64 = ctx.r11.s64 + 18880;
	// b 0x82594900
	sub_82594900(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595140);  /* forward decl */
PPC_WEAK_FUNC(sub_82595140) { __imp__sub_82595140(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595140) {
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
	// bl 0x82594ae0
	ctx.lr = 0x82595174;
	sub_82594AE0(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82595198);  /* forward decl */
PPC_WEAK_FUNC(sub_82595198) { __imp__sub_82595198(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595198) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// cntlzw r10,r10
	ctx.r10.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// lfs f3,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f3.f64 = double(temp.f32);
	// lfs f4,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f4.f64 = double(temp.f32);
	// rlwinm r11,r10,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r7,r11,24
	ctx.r7.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82593c10
	sub_82593C10(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825951C8);  /* forward decl */
PPC_WEAK_FUNC(sub_825951C8) { __imp__sub_825951C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825951C8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stfd f29,-32(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f29.u64);
	// stfd f30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f30.u64);
	// stfd f31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.f31.u64);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f1,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f1.f64 = double(temp.f32);
	// lfs f31,0(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f31.f64 = double(temp.f32);
	// lfs f30,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f30.f64 = double(temp.f32);
	// lfs f29,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f29.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,12120(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12120);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f1,f0
	ctx.cr6.compare(ctx.f1.f64, ctx.f0.f64);
	// bgt cr6,0x82595214
	if (ctx.cr6.gt) goto loc_82595214;
	// li r5,4
	ctx.r5.s64 = 4;
	// fmr f2,f30
	ctx.f2.f64 = ctx.f30.f64;
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// bl 0x8231fbb0
	ctx.lr = 0x82595214;
	sub_8231FBB0(ctx, base);
loc_82595214:
	// li r5,0
	ctx.r5.s64 = 0;
	// stfs f1,88(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// stfs f31,80(r1)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f30,84(r1)
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// fmr f1,f29
	ctx.f1.f64 = ctx.f29.f64;
	// bl 0x82281a10
	ctx.lr = 0x82595230;
	sub_82281A10(ctx, base);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f29,-32(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f29.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// lfd f30,-24(r1)
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// lfd f31,-16(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595250);  /* forward decl */
PPC_WEAK_FUNC(sub_82595250) { __imp__sub_82595250(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595250) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// lfs f4,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f4.f64 = double(temp.f32);
	// lfs f3,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f3.f64 = double(temp.f32);
	// b 0x82593d30
	sub_82593D30(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595268);  /* forward decl */
PPC_WEAK_FUNC(sub_82595268) { __imp__sub_82595268(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595268) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stfd f29,-32(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f29.u64);
	// stfd f30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f30.u64);
	// stfd f31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.f31.u64);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f1,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f1.f64 = double(temp.f32);
	// lfs f31,0(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f31.f64 = double(temp.f32);
	// lfs f30,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f30.f64 = double(temp.f32);
	// lfs f29,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f29.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,12120(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12120);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f1,f0
	ctx.cr6.compare(ctx.f1.f64, ctx.f0.f64);
	// bgt cr6,0x825952b4
	if (ctx.cr6.gt) goto loc_825952B4;
	// li r5,4
	ctx.r5.s64 = 4;
	// fmr f2,f30
	ctx.f2.f64 = ctx.f30.f64;
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// bl 0x8231fbb0
	ctx.lr = 0x825952B4;
	sub_8231FBB0(ctx, base);
loc_825952B4:
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// stfs f1,88(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// stfs f31,80(r1)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// fmr f1,f29
	ctx.f1.f64 = ctx.f29.f64;
	// stfs f30,84(r1)
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// bl 0x82282038
	ctx.lr = 0x825952CC;
	sub_82282038(ctx, base);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f29,-32(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f29.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// lfd f30,-24(r1)
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// lfd f31,-16(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825952E8);  /* forward decl */
PPC_WEAK_FUNC(sub_825952E8) { __imp__sub_825952E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825952E8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stfd f29,-32(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f29.u64);
	// stfd f30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f30.u64);
	// stfd f31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.f31.u64);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f1,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f1.f64 = double(temp.f32);
	// lfs f31,0(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f31.f64 = double(temp.f32);
	// lfs f30,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f30.f64 = double(temp.f32);
	// lfs f29,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f29.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,12120(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12120);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f1,f0
	ctx.cr6.compare(ctx.f1.f64, ctx.f0.f64);
	// bgt cr6,0x82595334
	if (ctx.cr6.gt) goto loc_82595334;
	// li r5,4
	ctx.r5.s64 = 4;
	// fmr f2,f30
	ctx.f2.f64 = ctx.f30.f64;
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// bl 0x8231fbb0
	ctx.lr = 0x82595334;
	sub_8231FBB0(ctx, base);
loc_82595334:
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// stfs f1,88(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// stfs f31,80(r1)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// fmr f1,f29
	ctx.f1.f64 = ctx.f29.f64;
	// stfs f30,84(r1)
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// bl 0x822820a0
	ctx.lr = 0x8259534C;
	sub_822820A0(ctx, base);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f29,-32(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f29.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// lfd f30,-24(r1)
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// lfd f31,-16(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595368);  /* forward decl */
PPC_WEAK_FUNC(sub_82595368) { __imp__sub_82595368(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595368) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32071
	ctx.r11.s64 = -2101805056;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lbz r11,17737(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 17737);
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
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595390);  /* forward decl */
PPC_WEAK_FUNC(sub_82595390) { __imp__sub_82595390(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595390) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32079
	ctx.r11.s64 = -2102329344;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lbz r11,13517(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 13517);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825953B0);  /* forward decl */
PPC_WEAK_FUNC(sub_825953B0) { __imp__sub_825953B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825953B0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32079
	ctx.r10.s64 = -2102329344;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,13516(r10)
	PPC_STORE_U8(ctx.r10.u32 + 13516, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825953D0);  /* forward decl */
PPC_WEAK_FUNC(sub_825953D0) { __imp__sub_825953D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825953D0) {
	PPC_FUNC_PROLOGUE();
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
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82595418
	if (ctx.cr6.eq) goto loc_82595418;
	// bl 0x821d2088
	ctx.lr = 0x82595400;
	sub_821D2088(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,131(r3)
	PPC_STORE_U8(ctx.r3.u32 + 131, ctx.r11.u8);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_82595418:
	// bl 0x821d2088
	ctx.lr = 0x8259541C;
	sub_821D2088(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// stb r11,131(r3)
	PPC_STORE_U8(ctx.r3.u32 + 131, ctx.r11.u8);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595438);  /* forward decl */
PPC_WEAK_FUNC(sub_82595438) { __imp__sub_82595438(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595438) {
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
	// li r31,0
	ctx.r31.s64 = 0;
	// bl 0x821d2088
	ctx.lr = 0x82595458;
	sub_821D2088(ctx, base);
	// lbz r11,135(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 135);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82595470
	if (!ctx.cr6.eq) goto loc_82595470;
	// bl 0x821d2088
	ctx.lr = 0x82595468;
	sub_821D2088(ctx, base);
	// li r31,1
	ctx.r31.s64 = 1;
	// stb r31,135(r3)
	PPC_STORE_U8(ctx.r3.u32 + 135, ctx.r31.u8);
loc_82595470:
	// lwz r11,0(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// clrlwi r10,r31,24
	ctx.r10.u64 = ctx.r31.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
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

PPC_FUNC_IMPL(__imp__sub_82595498);  /* forward decl */
PPC_WEAK_FUNC(sub_82595498) { __imp__sub_82595498(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595498) {
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
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// lfs f1,12(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f1.f64 = double(temp.f32);
	// lfs f0,0(r11)
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
	// bl 0x828e0ab8
	ctx.lr = 0x825954D8;
	sub_828E0AB8(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825954E8);  /* forward decl */
PPC_WEAK_FUNC(sub_825954E8) { __imp__sub_825954E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825954E8) {
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
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// lfs f1,12(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f1.f64 = double(temp.f32);
	// lfs f0,0(r11)
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
	// bl 0x828e0ab8
	ctx.lr = 0x82595528;
	sub_828E0AB8(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595538);  /* forward decl */
PPC_WEAK_FUNC(sub_82595538) { __imp__sub_82595538(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595538) {
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
	// lwz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
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
	// srawi r9,r10,8
	ctx.xer.ca = (ctx.r10.s32 < 0) & ((ctx.r10.u32 & 0xFF) != 0);
	ctx.r9.s64 = ctx.r10.s32 >> 8;
	// lfs f1,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f1.f64 = double(temp.f32);
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f12,88(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// lwz r11,24544(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24544);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r8,r8,r9
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r8.u32 + ctx.r9.u32);
	// cmpw cr6,r8,r10
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r10.s32, ctx.xer);
	// bne cr6,0x8259559c
	if (!ctx.cr6.eq) goto loc_8259559C;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r9
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r9.s32);
	// add r6,r11,r10
	ctx.r6.u64 = ctx.r11.u64 + ctx.r10.u64;
	// b 0x825955a0
	goto loc_825955A0;
loc_8259559C:
	// li r6,0
	ctx.r6.s64 = 0;
loc_825955A0:
	// li r7,0
	ctx.r7.s64 = 0;
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x828e0ab8
	ctx.lr = 0x825955B0;
	sub_828E0AB8(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825955C0);  /* forward decl */
PPC_WEAK_FUNC(sub_825955C0) { __imp__sub_825955C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825955C0) {
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
	// li r7,1
	ctx.r7.s64 = 1;
	// li r6,0
	ctx.r6.s64 = 0;
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// lfs f1,12(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f1.f64 = double(temp.f32);
	// lfs f0,0(r11)
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
	// bl 0x828e0ab8
	ctx.lr = 0x82595600;
	sub_828E0AB8(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595610);  /* forward decl */
PPC_WEAK_FUNC(sub_82595610) { __imp__sub_82595610(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595610) {
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
	// lwz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
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
	// srawi r9,r10,8
	ctx.xer.ca = (ctx.r10.s32 < 0) & ((ctx.r10.u32 & 0xFF) != 0);
	ctx.r9.s64 = ctx.r10.s32 >> 8;
	// lfs f1,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f1.f64 = double(temp.f32);
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f12,88(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// lwz r11,24544(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24544);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r8,r8,r9
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r8.u32 + ctx.r9.u32);
	// cmpw cr6,r8,r10
	ctx.cr6.compare<int32_t>(ctx.r8.s32, ctx.r10.s32, ctx.xer);
	// bne cr6,0x82595674
	if (!ctx.cr6.eq) goto loc_82595674;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r9
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r9.s32);
	// add r6,r11,r10
	ctx.r6.u64 = ctx.r11.u64 + ctx.r10.u64;
	// b 0x82595678
	goto loc_82595678;
loc_82595674:
	// li r6,0
	ctx.r6.s64 = 0;
loc_82595678:
	// li r7,1
	ctx.r7.s64 = 1;
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x828e0ab8
	ctx.lr = 0x82595688;
	sub_828E0AB8(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595698);  /* forward decl */
PPC_WEAK_FUNC(sub_82595698) { __imp__sub_82595698(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595698) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x826f26b8
	ctx.lr = 0x825956B0;
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

PPC_FUNC_IMPL(__imp__sub_825956D0);  /* forward decl */
PPC_WEAK_FUNC(sub_825956D0) { __imp__sub_825956D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825956D0) {
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
	// b 0x82593970
	sub_82593970(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825956F0);  /* forward decl */
PPC_WEAK_FUNC(sub_825956F0) { __imp__sub_825956F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825956F0) {
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
	// b 0x825939f8
	sub_825939F8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595710);  /* forward decl */
PPC_WEAK_FUNC(sub_82595710) { __imp__sub_82595710(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595710) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stfd f29,-32(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f29.u64);
	// stfd f30,-24(r1)
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f30.u64);
	// stfd f31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.f31.u64);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f1,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f1.f64 = double(temp.f32);
	// lfs f31,0(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f31.f64 = double(temp.f32);
	// lfs f30,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f30.f64 = double(temp.f32);
	// lfs f29,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f29.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,12120(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12120);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f1,f0
	ctx.cr6.compare(ctx.f1.f64, ctx.f0.f64);
	// bgt cr6,0x8259575c
	if (ctx.cr6.gt) goto loc_8259575C;
	// li r5,4
	ctx.r5.s64 = 4;
	// fmr f2,f30
	ctx.f2.f64 = ctx.f30.f64;
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// bl 0x8231fbb0
	ctx.lr = 0x8259575C;
	sub_8231FBB0(ctx, base);
loc_8259575C:
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// stfs f1,88(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// stfs f31,80(r1)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// fmr f1,f29
	ctx.f1.f64 = ctx.f29.f64;
	// stfs f30,84(r1)
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// bl 0x82271e38
	ctx.lr = 0x82595774;
	sub_82271E38(ctx, base);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f29,-32(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f29.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// lfd f30,-24(r1)
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// lfd f31,-16(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595790);  /* forward decl */
PPC_WEAK_FUNC(sub_82595790) { __imp__sub_82595790(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595790) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
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
	// lfs f11,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f11.f64 = double(temp.f32);
	// lfs f10,16(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f10.f64 = double(temp.f32);
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// stfs f0,-16(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -16, temp.u32);
	// stfs f13,-12(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + -12, temp.u32);
	// stfs f12,-8(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + -8, temp.u32);
	// addi r10,r1,-16
	ctx.r10.s64 = ctx.r1.s64 + -16;
	// stfs f11,10384(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 10384, temp.u32);
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stfs f10,10380(r11)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r11.u32 + 10380, temp.u32);
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// addi r11,r11,12432
	ctx.r11.s64 = ctx.r11.s64 + 12432;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825957E0);  /* forward decl */
PPC_WEAK_FUNC(sub_825957E0) { __imp__sub_825957E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825957E0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
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
	// lfs f11,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f11.f64 = double(temp.f32);
	// lfs f10,16(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f10.f64 = double(temp.f32);
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// stfs f0,-16(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -16, temp.u32);
	// stfs f13,-12(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + -12, temp.u32);
	// stfs f12,-8(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + -8, temp.u32);
	// addi r10,r1,-16
	ctx.r10.s64 = ctx.r1.s64 + -16;
	// stfs f11,10376(r11)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r11.u32 + 10376, temp.u32);
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// lvx128 v0,r0,r10
	ea = (ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// stfs f10,10372(r11)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r11.u32 + 10372, temp.u32);
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// addi r11,r11,12448
	ctx.r11.s64 = ctx.r11.s64 + 12448;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595830);  /* forward decl */
PPC_WEAK_FUNC(sub_82595830) { __imp__sub_82595830(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595830) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32061
	ctx.r10.s64 = -2101149696;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,11032(r10)
	PPC_STORE_U8(ctx.r10.u32 + 11032, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595850);  /* forward decl */
PPC_WEAK_FUNC(sub_82595850) { __imp__sub_82595850(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595850) {
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
	// b 0x822719a8
	sub_822719A8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595870);  /* forward decl */
PPC_WEAK_FUNC(sub_82595870) { __imp__sub_82595870(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595870) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x821e0008
	ctx.lr = 0x82595888;
	sub_821E0008(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825958B8);  /* forward decl */
PPC_WEAK_FUNC(sub_825958B8) { __imp__sub_825958B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825958B8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// addi r11,r11,-14792
	ctx.r11.s64 = ctx.r11.s64 + -14792;
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// bne cr6,0x825958e0
	if (!ctx.cr6.eq) goto loc_825958E0;
	// li r11,1
	ctx.r11.s64 = 1;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
loc_825958E0:
	// lis r11,-32071
	ctx.r11.s64 = -2101805056;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// addi r11,r11,-6604
	ctx.r11.s64 = ctx.r11.s64 + -6604;
	// lbz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 4);
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
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595908);  /* forward decl */
PPC_WEAK_FUNC(sub_82595908) { __imp__sub_82595908(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595908) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x821e0020
	ctx.lr = 0x82595920;
	sub_821E0020(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82595950);  /* forward decl */
PPC_WEAK_FUNC(sub_82595950) { __imp__sub_82595950(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595950) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32094
	ctx.r10.s64 = -2103312384;
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
	// stb r11,17362(r10)
	PPC_STORE_U8(ctx.r10.u32 + 17362, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595980);  /* forward decl */
PPC_WEAK_FUNC(sub_82595980) { __imp__sub_82595980(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595980) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lwz r9,24(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
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
	// b 0x82593fe0
	sub_82593FE0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825959A8);  /* forward decl */
PPC_WEAK_FUNC(sub_825959A8) { __imp__sub_825959A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825959A8) {
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
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r30,-7564(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + -7564);
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// lwz r31,0(r10)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// stw r31,1876(r11)
	PPC_STORE_U32(ctx.r11.u32 + 1876, ctx.r31.u32);
	// bl 0x8227f788
	ctx.lr = 0x825959D8;
	sub_8227F788(ctx, base);
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// stw r30,8(r3)
	PPC_STORE_U32(ctx.r3.u32 + 8, ctx.r30.u32);
	// bne cr6,0x825959f0
	if (!ctx.cr6.eq) goto loc_825959F0;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// li r11,0
	ctx.r11.s64 = 0;
	// stb r11,1875(r10)
	PPC_STORE_U8(ctx.r10.u32 + 1875, ctx.r11.u8);
loc_825959F0:
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

PPC_FUNC_IMPL(__imp__sub_82595A08);  /* forward decl */
PPC_WEAK_FUNC(sub_82595A08) { __imp__sub_82595A08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595A08) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
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
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,1875(r10)
	PPC_STORE_U8(ctx.r10.u32 + 1875, ctx.r11.u8);
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// stfs f0,-6296(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -6296, temp.u32);
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// stfs f13,-6300(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + -6300, temp.u32);
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// stfs f12,-6304(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + -6304, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595A40);  /* forward decl */
PPC_WEAK_FUNC(sub_82595A40) { __imp__sub_82595A40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595A40) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x82538380
	ctx.lr = 0x82595A58;
	sub_82538380(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82595A78);  /* forward decl */
PPC_WEAK_FUNC(sub_82595A78) { __imp__sub_82595A78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595A78) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r10,1
	ctx.r10.s64 = 1;
	// lwz r9,0(r3)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// slw r10,r10,r8
	ctx.r10.u64 = ctx.r8.u8 & 0x20 ? 0 : (ctx.r10.u32 << (ctx.r8.u8 & 0x3F));
	// and r11,r10,r11
	ctx.r11.u64 = ctx.r10.u64 & ctx.r11.u64;
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stw r11,0(r9)
	PPC_STORE_U32(ctx.r9.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595AA8);  /* forward decl */
PPC_WEAK_FUNC(sub_82595AA8) { __imp__sub_82595AA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595AA8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82594210
	sub_82594210(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595AC8);  /* forward decl */
PPC_WEAK_FUNC(sub_82595AC8) { __imp__sub_82595AC8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595AC8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// lwz r11,1772(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 1772);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// bgt cr6,0x82595ae0
	if (ctx.cr6.gt) goto loc_82595AE0;
	// li r11,0
	ctx.r11.s64 = 0;
loc_82595AE0:
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595AF0);  /* forward decl */
PPC_WEAK_FUNC(sub_82595AF0) { __imp__sub_82595AF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595AF0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x821d2088
	ctx.lr = 0x82595B08;
	sub_821D2088(ctx, base);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x82595b24
	if (ctx.cr6.eq) goto loc_82595B24;
	// bl 0x821d2088
	ctx.lr = 0x82595B14;
	sub_821D2088(ctx, base);
	// lbz r11,133(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 133);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// bne cr6,0x82595b28
	if (!ctx.cr6.eq) goto loc_82595B28;
loc_82595B24:
	// li r11,0
	ctx.r11.s64 = 0;
loc_82595B28:
	// lwz r10,0(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
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

PPC_FUNC_IMPL(__imp__sub_82595B48);  /* forward decl */
PPC_WEAK_FUNC(sub_82595B48) { __imp__sub_82595B48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595B48) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// addi r11,r11,-14792
	ctx.r11.s64 = ctx.r11.s64 + -14792;
	// lwz r11,40(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 40);
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
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595B70);  /* forward decl */
PPC_WEAK_FUNC(sub_82595B70) { __imp__sub_82595B70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595B70) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// addi r11,r11,-14792
	ctx.r11.s64 = ctx.r11.s64 + -14792;
	// lwz r11,128(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 128);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595B88);  /* forward decl */
PPC_WEAK_FUNC(sub_82595B88) { __imp__sub_82595B88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595B88) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-32050
	ctx.r11.s64 = -2100428800;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,-9924(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + -9924);
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x822ef2c0
	ctx.lr = 0x82595BB0;
	sub_822EF2C0(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82595BD0);  /* forward decl */
PPC_WEAK_FUNC(sub_82595BD0) { __imp__sub_82595BD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595BD0) {
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
	// bl 0x821dd228
	ctx.lr = 0x82595BE8;
	sub_821DD228(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595C08);  /* forward decl */
PPC_WEAK_FUNC(sub_82595C08) { __imp__sub_82595C08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595C08) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// bge cr6,0x82595c1c
	if (!ctx.cr6.lt) goto loc_82595C1C;
	// neg r11,r11
	ctx.r11.s64 = static_cast<int64_t>(-ctx.r11.u64);
loc_82595C1C:
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595C28);  /* forward decl */
PPC_WEAK_FUNC(sub_82595C28) { __imp__sub_82595C28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595C28) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lfs f0,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// fabs f0,f0
	ctx.f0.u64 = ctx.f0.u64 & ~0x8000000000000000;
	// stfs f0,0(r10)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595C40);  /* forward decl */
PPC_WEAK_FUNC(sub_82595C40) { __imp__sub_82595C40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595C40) {
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
	// bl 0x825943a0
	ctx.lr = 0x82595C74;
	sub_825943A0(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82595C98);  /* forward decl */
PPC_WEAK_FUNC(sub_82595C98) { __imp__sub_82595C98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595C98) {
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
	// bl 0x82594430
	ctx.lr = 0x82595CCC;
	sub_82594430(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82595CF0);  /* forward decl */
PPC_WEAK_FUNC(sub_82595CF0) { __imp__sub_82595CF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595CF0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// addi r12,r1,-16
	ctx.r12.s64 = ctx.r1.s64 + -16;
	// bl 0x82990148
	ctx.lr = 0x82595D04;
	__savefpr_28(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// li r5,0
	ctx.r5.s64 = 0;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lfs f31,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f31.f64 = double(temp.f32);
	// lfs f30,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f30.f64 = double(temp.f32);
	// cntlzw r10,r10
	ctx.r10.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// lfs f29,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f29.f64 = double(temp.f32);
	// lfs f28,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f28.f64 = double(temp.f32);
	// rlwinm r11,r10,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82595d48
	if (ctx.cr6.eq) goto loc_82595D48;
	// bl 0x8227f650
	ctx.lr = 0x82595D44;
	sub_8227F650(ctx, base);
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
loc_82595D48:
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// stfs f31,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f30,84(r1)
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// fmr f1,f28
	ctx.f1.f64 = ctx.f28.f64;
	// stfs f29,88(r1)
	temp.f32 = float(ctx.f29.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// bl 0x82317ce0
	ctx.lr = 0x82595D60;
	sub_82317CE0(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// clrlwi r10,r3,24
	ctx.r10.u64 = ctx.r3.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// addi r12,r1,-16
	ctx.r12.s64 = ctx.r1.s64 + -16;
	// bl 0x82990194
	ctx.lr = 0x82595D78;
	__restfpr_28(ctx, base);
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595D88);  /* forward decl */
PPC_WEAK_FUNC(sub_82595D88) { __imp__sub_82595D88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595D88) {
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
	// lwz r10,24(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// cntlzw r10,r10
	ctx.r10.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// lfs f3,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f3.f64 = double(temp.f32);
	// lfs f4,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f4.f64 = double(temp.f32);
	// rlwinm r10,r10,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// lfs f5,16(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f5.f64 = double(temp.f32);
	// lfs f6,20(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	ctx.f6.f64 = double(temp.f32);
	// xori r11,r10,1
	ctx.r11.u64 = ctx.r10.u64 ^ 1;
	// clrlwi r9,r11,24
	ctx.r9.u64 = ctx.r11.u32 & 0xFF;
	// bl 0x82593b18
	ctx.lr = 0x82595DD0;
	sub_82593B18(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82595DF0);  /* forward decl */
PPC_WEAK_FUNC(sub_82595DF0) { __imp__sub_82595DF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595DF0) {
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
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82595e48
	if (ctx.cr6.eq) goto loc_82595E48;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// lfs f0,3068(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3068);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f0,84(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// stfs f0,88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// bl 0x8242e9a0
	ctx.lr = 0x82595E38;
	sub_8242E9A0(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_82595E48:
	// bl 0x8242e9f0
	ctx.lr = 0x82595E4C;
	sub_8242E9F0(ctx, base);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595E60);  /* forward decl */
PPC_WEAK_FUNC(sub_82595E60) { __imp__sub_82595E60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595E60) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// li r10,1
	ctx.r10.s64 = 1;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595E70);  /* forward decl */
PPC_WEAK_FUNC(sub_82595E70) { __imp__sub_82595E70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595E70) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32015
	ctx.r10.s64 = -2098135040;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,-13865(r10)
	PPC_STORE_U8(ctx.r10.u32 + -13865, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595E90);  /* forward decl */
PPC_WEAK_FUNC(sub_82595E90) { __imp__sub_82595E90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595E90) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32015
	ctx.r11.s64 = -2098135040;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lbz r11,-13865(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + -13865);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595EA8);  /* forward decl */
PPC_WEAK_FUNC(sub_82595EA8) { __imp__sub_82595EA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595EA8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// addi r10,r10,-16004
	ctx.r10.s64 = ctx.r10.s64 + -16004;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,28(r10)
	PPC_STORE_U8(ctx.r10.u32 + 28, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595ED0);  /* forward decl */
PPC_WEAK_FUNC(sub_82595ED0) { __imp__sub_82595ED0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595ED0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// addi r10,r10,-16004
	ctx.r10.s64 = ctx.r10.s64 + -16004;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,29(r10)
	PPC_STORE_U8(ctx.r10.u32 + 29, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595EF8);  /* forward decl */
PPC_WEAK_FUNC(sub_82595EF8) { __imp__sub_82595EF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595EF8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r8,1
	ctx.r8.s64 = 1;
	// lwz r7,0(r3)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// subf r11,r10,r11
	ctx.r11.u64 = ctx.r11.u64 - ctx.r10.u64;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// sraw r10,r9,r10
	temp.u32 = ctx.r10.u32 & 0x3F;
	if (temp.u32 > 0x1F) temp.u32 = 0x1F;
	ctx.xer.ca = (ctx.r9.s32 < 0) & (((ctx.r9.s32 >> temp.u32) << temp.u32) != ctx.r9.s32);
	ctx.r10.s64 = ctx.r9.s32 >> temp.u32;
	// slw r11,r8,r11
	ctx.r11.u64 = ctx.r11.u8 & 0x20 ? 0 : (ctx.r8.u32 << (ctx.r11.u8 & 0x3F));
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// and r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 & ctx.r10.u64;
	// stw r11,0(r7)
	PPC_STORE_U32(ctx.r7.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595F30);  /* forward decl */
PPC_WEAK_FUNC(sub_82595F30) { __imp__sub_82595F30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595F30) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x8225cc08
	sub_8225CC08(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595F50);  /* forward decl */
PPC_WEAK_FUNC(sub_82595F50) { __imp__sub_82595F50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595F50) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r31,r11,1
	ctx.r31.u64 = ctx.r11.u64 ^ 1;
	// bl 0x821d2088
	ctx.lr = 0x82595F78;
	sub_821D2088(ctx, base);
	// stb r31,137(r3)
	PPC_STORE_U8(ctx.r3.u32 + 137, ctx.r31.u8);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595F90);  /* forward decl */
PPC_WEAK_FUNC(sub_82595F90) { __imp__sub_82595F90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595F90) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r31,r11,1
	ctx.r31.u64 = ctx.r11.u64 ^ 1;
	// bl 0x821d2088
	ctx.lr = 0x82595FB8;
	sub_821D2088(ctx, base);
	// stb r31,144(r3)
	PPC_STORE_U8(ctx.r3.u32 + 144, ctx.r31.u8);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595FD0);  /* forward decl */
PPC_WEAK_FUNC(sub_82595FD0) { __imp__sub_82595FD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595FD0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x821da680
	sub_821DA680(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82595FE0);  /* forward decl */
PPC_WEAK_FUNC(sub_82595FE0) { __imp__sub_82595FE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82595FE0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x821d83d0
	ctx.lr = 0x82596004;
	sub_821D83D0(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82596028);  /* forward decl */
PPC_WEAK_FUNC(sub_82596028) { __imp__sub_82596028(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596028) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x8242ea30
	sub_8242EA30(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596048);  /* forward decl */
PPC_WEAK_FUNC(sub_82596048) { __imp__sub_82596048(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596048) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x8227ea40
	sub_8227EA40(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596068);  /* forward decl */
PPC_WEAK_FUNC(sub_82596068) { __imp__sub_82596068(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596068) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32093
	ctx.r10.s64 = -2103246848;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stw r11,9056(r10)
	PPC_STORE_U32(ctx.r10.u32 + 9056, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596080);  /* forward decl */
PPC_WEAK_FUNC(sub_82596080) { __imp__sub_82596080(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596080) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x829a1bd0
	sub_829A1BD0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825960A0);  /* forward decl */
PPC_WEAK_FUNC(sub_825960A0) { __imp__sub_825960A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825960A0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x8242eb78
	sub_8242EB78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825960C0);  /* forward decl */
PPC_WEAK_FUNC(sub_825960C0) { __imp__sub_825960C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825960C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r3,r11,-14232
	ctx.r3.s64 = ctx.r11.s64 + -14232;
	// lbz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825960f0
	if (ctx.cr6.eq) goto loc_825960F0;
	// li r11,1
	ctx.r11.s64 = 1;
	// b 0x82596104
	goto loc_82596104;
loc_825960F0:
	// bl 0x8224b230
	ctx.lr = 0x825960F4;
	sub_8224B230(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
loc_82596104:
	// lwz r10,0(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
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

PPC_FUNC_IMPL(__imp__sub_82596128);  /* forward decl */
PPC_WEAK_FUNC(sub_82596128) { __imp__sub_82596128(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596128) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32006
	ctx.r11.s64 = -2097545216;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lbz r11,18401(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 18401);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596140);  /* forward decl */
PPC_WEAK_FUNC(sub_82596140) { __imp__sub_82596140(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596140) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32079
	ctx.r11.s64 = -2102329344;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,4468(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4468);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596158);  /* forward decl */
PPC_WEAK_FUNC(sub_82596158) { __imp__sub_82596158(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596158) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,29880(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 29880);
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x82269b08
	ctx.lr = 0x82596180;
	sub_82269B08(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825961A0);  /* forward decl */
PPC_WEAK_FUNC(sub_825961A0) { __imp__sub_825961A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825961A0) {
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
	// lwz r11,8(r4)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 8);
	// mr r10,r3
	ctx.r10.u64 = ctx.r3.u64;
	// lfs f12,8(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f12.f64 = double(temp.f32);
	// lfs f9,20(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	ctx.f9.f64 = double(temp.f32);
	// lfs f0,0(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f11,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f11.f64 = double(temp.f32);
	// lfs f10,16(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f10.f64 = double(temp.f32);
	// lfs f1,24(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	ctx.f1.f64 = double(temp.f32);
	// stfs f9,104(r1)
	temp.f32 = float(ctx.f9.f64);
	PPC_STORE_U32(ctx.r1.u32 + 104, temp.u32);
	// lwz r9,104(r1)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 104);
	// stfs f12,88(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// lwz r11,88(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 88);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// rldicr r6,r9,32,63
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r9.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// stfs f13,84(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// ld r3,80(r1)
	ctx.r3.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
	// stfs f11,96(r1)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r1.u32 + 96, temp.u32);
	// rldicr r4,r11,32,63
	ctx.r4.u64 = __builtin_rotateleft64(ctx.r11.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// stfs f10,100(r1)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r1.u32 + 100, temp.u32);
	// ld r5,96(r1)
	ctx.r5.u64 = PPC_LOAD_U64(ctx.r1.u32 + 96);
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
	// bctrl 
	ctx.lr = 0x82596208;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596218);  /* forward decl */
PPC_WEAK_FUNC(sub_82596218) { __imp__sub_82596218(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596218) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stfd f31,-24(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -24, ctx.f31.u64);
	// stwu r1,-176(r1)
	ea = -176 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,8(r4)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + 8);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lfs f31,56(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 56);
	ctx.f31.f64 = double(temp.f32);
	// lwz r10,60(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 60);
	// lfs f0,0(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f12.f64 = double(temp.f32);
	// lfs f11,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f11.f64 = double(temp.f32);
	// lfs f10,16(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f10.f64 = double(temp.f32);
	// stw r10,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r10.u32);
	// lfs f9,20(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	ctx.f9.f64 = double(temp.f32);
	// lfs f8,24(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	ctx.f8.f64 = double(temp.f32);
	// lfs f7,28(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	ctx.f7.f64 = double(temp.f32);
	// lfs f6,32(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 32);
	ctx.f6.f64 = double(temp.f32);
	// lfs f5,36(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	ctx.f5.f64 = double(temp.f32);
	// lfs f4,40(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 40);
	ctx.f4.f64 = double(temp.f32);
	// lfs f3,44(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 44);
	ctx.f3.f64 = double(temp.f32);
	// lfs f2,48(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 48);
	ctx.f2.f64 = double(temp.f32);
	// lfs f1,52(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 52);
	ctx.f1.f64 = double(temp.f32);
	// stfs f31,136(r1)
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 136, temp.u32);
	// lwz r11,136(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 136);
	// stfs f12,120(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + 120, temp.u32);
	// stfs f0,112(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 112, temp.u32);
	// stfs f13,116(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + 116, temp.u32);
	// ld r3,112(r1)
	ctx.r3.u64 = PPC_LOAD_U64(ctx.r1.u32 + 112);
	// stfs f2,128(r1)
	temp.f32 = float(ctx.f2.f64);
	PPC_STORE_U32(ctx.r1.u32 + 128, temp.u32);
	// stw r11,88(r1)
	PPC_STORE_U32(ctx.r1.u32 + 88, ctx.r11.u32);
	// stfs f1,132(r1)
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r1.u32 + 132, temp.u32);
	// lwz r11,120(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 120);
	// stfs f9,120(r1)
	temp.f32 = float(ctx.f9.f64);
	PPC_STORE_U32(ctx.r1.u32 + 120, temp.u32);
	// stfs f11,112(r1)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r1.u32 + 112, temp.u32);
	// rldicr r4,r11,32,63
	ctx.r4.u64 = __builtin_rotateleft64(ctx.r11.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// lwz r11,120(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 120);
	// stfs f6,120(r1)
	temp.f32 = float(ctx.f6.f64);
	PPC_STORE_U32(ctx.r1.u32 + 120, temp.u32);
	// rldicr r6,r11,32,63
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r11.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// lwz r11,120(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 120);
	// stfs f3,120(r1)
	temp.f32 = float(ctx.f3.f64);
	PPC_STORE_U32(ctx.r1.u32 + 120, temp.u32);
	// rldicr r8,r11,32,63
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r11.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// lwz r11,120(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 120);
	// stfs f10,116(r1)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r1.u32 + 116, temp.u32);
	// ld r5,112(r1)
	ctx.r5.u64 = PPC_LOAD_U64(ctx.r1.u32 + 112);
	// rldicr r10,r11,32,63
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u64, 32) & 0xFFFFFFFFFFFFFFFF;
	// ld r11,128(r1)
	ctx.r11.u64 = PPC_LOAD_U64(ctx.r1.u32 + 128);
	// stfs f8,112(r1)
	temp.f32 = float(ctx.f8.f64);
	PPC_STORE_U32(ctx.r1.u32 + 112, temp.u32);
	// stfs f7,116(r1)
	temp.f32 = float(ctx.f7.f64);
	PPC_STORE_U32(ctx.r1.u32 + 116, temp.u32);
	// ld r7,112(r1)
	ctx.r7.u64 = PPC_LOAD_U64(ctx.r1.u32 + 112);
	// stfs f5,112(r1)
	temp.f32 = float(ctx.f5.f64);
	PPC_STORE_U32(ctx.r1.u32 + 112, temp.u32);
	// stfs f4,116(r1)
	temp.f32 = float(ctx.f4.f64);
	PPC_STORE_U32(ctx.r1.u32 + 116, temp.u32);
	// ld r9,112(r1)
	ctx.r9.u64 = PPC_LOAD_U64(ctx.r1.u32 + 112);
	// std r11,80(r1)
	PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r11.u64);
	// mtctr r31
	ctx.ctr.u64 = ctx.r31.u64;
	// bctrl 
	ctx.lr = 0x825962FC;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// addi r1,r1,176
	ctx.r1.s64 = ctx.r1.s64 + 176;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f31,-24(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596318);  /* forward decl */
PPC_WEAK_FUNC(sub_82596318) { __imp__sub_82596318(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596318) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,15520
	ctx.r3.s64 = ctx.r11.s64 + 15520;
	// b 0x825961a0
	sub_825961A0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596328);  /* forward decl */
PPC_WEAK_FUNC(sub_82596328) { __imp__sub_82596328(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596328) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,17920
	ctx.r3.s64 = ctx.r11.s64 + 17920;
	// b 0x82596218
	sub_82596218(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596338);  /* forward decl */
PPC_WEAK_FUNC(sub_82596338) { __imp__sub_82596338(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596338) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r11,-31980
	ctx.r11.s64 = -2095841280;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,29880(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 29880);
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x82269d18
	ctx.lr = 0x82596360;
	sub_82269D18(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82596380);  /* forward decl */
PPC_WEAK_FUNC(sub_82596380) { __imp__sub_82596380(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596380) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// lwz r7,8(r3)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,23772
	ctx.r10.s64 = 1557921792;
	// addi r11,r11,-5984
	ctx.r11.s64 = ctx.r11.s64 + -5984;
	// ori r6,r10,64167
	ctx.r6.u64 = ctx.r10.u64 | 64167;
	// lwz r8,0(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// mulld r10,r8,r6
	ctx.r10.s64 = static_cast<int64_t>(ctx.r8.u64 * ctx.r6.u64);
	// lwz r8,0(r7)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
	// add r10,r10,r9
	ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
	// rotlwi r9,r10,0
	ctx.r9.u64 = __builtin_rotateleft32(ctx.r10.u32, 0);
	// rldicl r10,r10,32,32
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u64, 32) & 0xFFFFFFFF;
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// clrlwi r9,r9,9
	ctx.r9.u64 = ctx.r9.u32 & 0x7FFFFF;
	// stw r10,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// std r9,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r9.u64);
	// lfd f0,-16(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f0.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// fcfid f0,f0
	ctx.f0.f64 = double(ctx.f0.s64);
	// frsp f13,f0
	ctx.f13.f64 = double(float(ctx.f0.f64));
	// lfs f0,9932(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 9932);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
	// stfs f0,0(r8)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825963E0);  /* forward decl */
PPC_WEAK_FUNC(sub_825963E0) { __imp__sub_825963E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825963E0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// lwz r7,8(r3)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,23772
	ctx.r10.s64 = 1557921792;
	// addi r11,r11,-5984
	ctx.r11.s64 = ctx.r11.s64 + -5984;
	// ori r6,r10,64167
	ctx.r6.u64 = ctx.r10.u64 | 64167;
	// lwz r8,0(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// mulld r10,r8,r6
	ctx.r10.s64 = static_cast<int64_t>(ctx.r8.u64 * ctx.r6.u64);
	// lwz r8,0(r7)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
	// add r10,r10,r9
	ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
	// rotlwi r9,r10,0
	ctx.r9.u64 = __builtin_rotateleft32(ctx.r10.u32, 0);
	// rldicl r10,r10,32,32
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u64, 32) & 0xFFFFFFFF;
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// clrlwi r9,r9,17
	ctx.r9.u64 = ctx.r9.u32 & 0x7FFF;
	// stw r10,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r10.u32);
	// stw r9,0(r8)
	PPC_STORE_U32(ctx.r8.u32 + 0, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596428);  /* forward decl */
PPC_WEAK_FUNC(sub_82596428) { __imp__sub_82596428(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596428) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32015
	ctx.r11.s64 = -2098135040;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f0,19680(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 19680);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f13,2980(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2980);
	ctx.f13.f64 = double(temp.f32);
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// bge cr6,0x82596458
	if (!ctx.cr6.lt) goto loc_82596458;
	// lis r10,-32015
	ctx.r10.s64 = -2098135040;
	// lwz r10,19700(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 19700);
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// blr 
	return;
loc_82596458:
	// lis r10,-32015
	ctx.r10.s64 = -2098135040;
	// lwz r10,19696(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 19696);
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596468);  /* forward decl */
PPC_WEAK_FUNC(sub_82596468) { __imp__sub_82596468(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596468) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32015
	ctx.r10.s64 = -2098135040;
	// lwz r9,8(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lfs f0,19680(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 19680);
	ctx.f0.f64 = double(temp.f32);
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stfs f0,0(r9)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// lis r9,-32015
	ctx.r9.s64 = -2098135040;
	// lwz r9,19700(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + 19700);
	// stw r9,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// lis r11,-32015
	ctx.r11.s64 = -2098135040;
	// lwz r11,19696(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 19696);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825964A0);  /* forward decl */
PPC_WEAK_FUNC(sub_825964A0) { __imp__sub_825964A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825964A0) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32056
	ctx.r11.s64 = -2100822016;
	// lwz r11,-7564(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -7564);
	// lwz r10,0(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825964B8);  /* forward decl */
PPC_WEAK_FUNC(sub_825964B8) { __imp__sub_825964B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825964B8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32056
	ctx.r11.s64 = -2100822016;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// addi r11,r11,-7564
	ctx.r11.s64 = ctx.r11.s64 + -7564;
	// lfs f0,8(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825964D8);  /* forward decl */
PPC_WEAK_FUNC(sub_825964D8) { __imp__sub_825964D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825964D8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r9,23772
	ctx.r9.s64 = 1557921792;
	// addi r11,r11,-5984
	ctx.r11.s64 = ctx.r11.s64 + -5984;
	// ori r6,r9,64167
	ctx.r6.u64 = ctx.r9.u64 | 64167;
	// lfs f0,0(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,4(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lwz r7,0(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// fsubs f12,f13,f0
	ctx.f12.f64 = double(float(ctx.f13.f64 - ctx.f0.f64));
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// mulld r9,r7,r6
	ctx.r9.s64 = static_cast<int64_t>(ctx.r7.u64 * ctx.r6.u64);
	// add r9,r9,r8
	ctx.r9.u64 = ctx.r9.u64 + ctx.r8.u64;
	// lwz r8,8(r10)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 8);
	// rotlwi r10,r9,0
	ctx.r10.u64 = __builtin_rotateleft32(ctx.r9.u32, 0);
	// rldicl r9,r9,32,32
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u64, 32) & 0xFFFFFFFF;
	// clrlwi r7,r10,9
	ctx.r7.u64 = ctx.r10.u32 & 0x7FFFFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// std r7,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r7.u64);
	// lfd f13,-16(r1)
	ctx.f13.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// fcfid f13,f13
	ctx.f13.f64 = double(ctx.f13.s64);
	// frsp f11,f13
	ctx.f11.f64 = double(float(ctx.f13.f64));
	// lfs f13,9932(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 9932);
	ctx.f13.f64 = double(temp.f32);
	// fmuls f13,f11,f13
	ctx.f13.f64 = double(float(ctx.f11.f64 * ctx.f13.f64));
	// fmadds f0,f13,f12,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 * ctx.f12.f64 + ctx.f0.f64));
	// stfs f0,0(r8)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596548);  /* forward decl */
PPC_WEAK_FUNC(sub_82596548) { __imp__sub_82596548(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596548) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r9,23772
	ctx.r9.s64 = 1557921792;
	// addi r11,r11,-5984
	ctx.r11.s64 = ctx.r11.s64 + -5984;
	// ori r6,r9,64167
	ctx.r6.u64 = ctx.r9.u64 | 64167;
	// lwz r7,0(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r8,4(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// mulld r9,r7,r6
	ctx.r9.s64 = static_cast<int64_t>(ctx.r7.u64 * ctx.r6.u64);
	// lwz r7,4(r10)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// lwz r6,8(r10)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r10.u32 + 8);
	// add r9,r9,r8
	ctx.r9.u64 = ctx.r9.u64 + ctx.r8.u64;
	// lwz r8,0(r10)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// rotlwi r10,r9,0
	ctx.r10.u64 = __builtin_rotateleft32(ctx.r9.u32, 0);
	// rldicl r9,r9,32,32
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u64, 32) & 0xFFFFFFFF;
	// clrldi r5,r10,33
	ctx.r5.u64 = ctx.r10.u64 & 0x7FFFFFFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// stw r9,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r9.u32);
	// subf r11,r8,r7
	ctx.r11.u64 = ctx.r7.u64 - ctx.r8.u64;
	// extsw r11,r11
	ctx.r11.s64 = ctx.r11.s32;
	// mulld r11,r5,r11
	ctx.r11.s64 = static_cast<int64_t>(ctx.r5.u64 * ctx.r11.u64);
	// rldicl r11,r11,33,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u64, 33) & 0x1FFFFFFFF;
	// rotlwi r11,r11,0
	ctx.r11.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// add r11,r11,r8
	ctx.r11.u64 = ctx.r11.u64 + ctx.r8.u64;
	// stw r11,0(r6)
	PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825965B0);  /* forward decl */
PPC_WEAK_FUNC(sub_825965B0) { __imp__sub_825965B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825965B0) {
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
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// li r8,4
	ctx.r8.s64 = 4;
	// li r7,0
	ctx.r7.s64 = 0;
	// addi r6,r1,80
	ctx.r6.s64 = ctx.r1.s64 + 80;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lwz r30,12(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// lfs f3,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f3.f64 = double(temp.f32);
	// bl 0x8231fcc8
	ctx.lr = 0x825965EC;
	sub_8231FCC8(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stfs f1,0(r30)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r30.u32 + 0, temp.u32);
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
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596618);  /* forward decl */
PPC_WEAK_FUNC(sub_82596618) { __imp__sub_82596618(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596618) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x825a7b90
	sub_825A7B90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596628);  /* forward decl */
PPC_WEAK_FUNC(sub_82596628) { __imp__sub_82596628(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596628) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f0,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lwz r10,16(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lfs f11,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f11.f64 = double(temp.f32);
	// fsubs f0,f0,f11
	ctx.f0.f64 = double(float(ctx.f0.f64 - ctx.f11.f64));
	// lfs f13,0(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f12.f64 = double(temp.f32);
	// fsubs f13,f13,f12
	ctx.f13.f64 = double(float(ctx.f13.f64 - ctx.f12.f64));
	// fmuls f0,f0,f0
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f0.f64));
	// fmadds f0,f13,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 * ctx.f13.f64 + ctx.f0.f64));
	// fsqrts f0,f0
	ctx.f0.f64 = double(float(sqrt(ctx.f0.f64)));
	// stfs f0,0(r10)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596660);  /* forward decl */
PPC_WEAK_FUNC(sub_82596660) { __imp__sub_82596660(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596660) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lfs f0,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lwz r10,24(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// lfs f13,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f12.f64 = double(temp.f32);
	// lfs f11,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f11.f64 = double(temp.f32);
	// lfs f10,16(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	ctx.f10.f64 = double(temp.f32);
	// lfs f9,20(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	ctx.f9.f64 = double(temp.f32);
	// stfs f0,-32(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + -32, temp.u32);
	// stfs f13,-28(r1)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r1.u32 + -28, temp.u32);
	// stfs f12,-24(r1)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r1.u32 + -24, temp.u32);
	// addi r11,r1,-32
	ctx.r11.s64 = ctx.r1.s64 + -32;
	// stfs f9,-8(r1)
	temp.f32 = float(ctx.f9.f64);
	PPC_STORE_U32(ctx.r1.u32 + -8, temp.u32);
	// stfs f11,-16(r1)
	temp.f32 = float(ctx.f11.f64);
	PPC_STORE_U32(ctx.r1.u32 + -16, temp.u32);
	// stfs f10,-12(r1)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r1.u32 + -12, temp.u32);
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,-16
	ctx.r11.s64 = ctx.r1.s64 + -16;
	// lvx128 v13,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,-16
	ctx.r11.s64 = ctx.r1.s64 + -16;
	// vsubfp v0,v0,v13
	ctx.fpscr.enableFlushModeUnconditional();
	simde_mm_store_ps(ctx.v0.f32, simde_mm_sub_ps(simde_mm_load_ps(ctx.v0.f32), simde_mm_load_ps(ctx.v13.f32)));
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// lfs f13,-12(r1)
	ctx.fpscr.disableFlushModeUnconditional();
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + -12);
	ctx.f13.f64 = double(temp.f32);
	// fmuls f12,f13,f13
	ctx.f12.f64 = double(float(ctx.f13.f64 * ctx.f13.f64));
	// lfs f0,-16(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + -16);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,-8(r1)
	temp.u32 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	ctx.f13.f64 = double(temp.f32);
	// fmadds f0,f0,f0,f12
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f0.f64 + ctx.f12.f64));
	// fmadds f0,f13,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 * ctx.f13.f64 + ctx.f0.f64));
	// fsqrts f0,f0
	ctx.f0.f64 = double(float(sqrt(ctx.f0.f64)));
	// stfs f0,0(r10)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825966D8);  /* forward decl */
PPC_WEAK_FUNC(sub_825966D8) { __imp__sub_825966D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825966D8) {
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
	// b 0x82593ee0
	sub_82593EE0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825966F8);  /* forward decl */
PPC_WEAK_FUNC(sub_825966F8) { __imp__sub_825966F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825966F8) {
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
	// b 0x82593f78
	sub_82593F78(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596710);  /* forward decl */
PPC_WEAK_FUNC(sub_82596710) { __imp__sub_82596710(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596710) {
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
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r5,16(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lfs f1,12(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f1.f64 = double(temp.f32);
	// lwz r6,20(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lfs f0,0(r11)
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
	// bl 0x82271930
	ctx.lr = 0x82596754;
	sub_82271930(ctx, base);
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
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596770);  /* forward decl */
PPC_WEAK_FUNC(sub_82596770) { __imp__sub_82596770(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596770) {
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
	// lwz r10,32(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 32);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// cntlzw r10,r10
	ctx.r10.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// rlwinm r10,r10,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lwz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// xori r31,r10,1
	ctx.r31.u64 = ctx.r10.u64 ^ 1;
	// lwz r9,24(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	// lwz r10,28(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stb r31,87(r1)
	PPC_STORE_U8(ctx.r1.u32 + 87, ctx.r31.u8);
	// bl 0x82593a80
	ctx.lr = 0x825967BC;
	sub_82593A80(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_825967D0);  /* forward decl */
PPC_WEAK_FUNC(sub_825967D0) { __imp__sub_825967D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825967D0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r9,1
	ctx.r9.s64 = 1;
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// slw r11,r9,r11
	ctx.r11.u64 = ctx.r11.u8 & 0x20 ? 0 : (ctx.r9.u32 << (ctx.r11.u8 & 0x3F));
	// lwz r9,0(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// or r11,r11,r9
	ctx.r11.u64 = ctx.r11.u64 | ctx.r9.u64;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825967F8);  /* forward decl */
PPC_WEAK_FUNC(sub_825967F8) { __imp__sub_825967F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825967F8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r9,1
	ctx.r9.s64 = 1;
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// slw r11,r9,r11
	ctx.r11.u64 = ctx.r11.u8 & 0x20 ? 0 : (ctx.r9.u32 << (ctx.r11.u8 & 0x3F));
	// lwz r9,0(r10)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// andc r11,r9,r11
	ctx.r11.u64 = ctx.r9.u64 & ~ctx.r11.u64;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596820);  /* forward decl */
PPC_WEAK_FUNC(sub_82596820) { __imp__sub_82596820(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596820) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x82318f60
	ctx.lr = 0x82596840;
	sub_82318F60(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82596860);  /* forward decl */
PPC_WEAK_FUNC(sub_82596860) { __imp__sub_82596860(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596860) {
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
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// addi r3,r11,12544
	ctx.r3.s64 = ctx.r11.s64 + 12544;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// bl 0x82273620
	ctx.lr = 0x82596888;
	sub_82273620(ctx, base);
	// li r4,1
	ctx.r4.s64 = 1;
	// bl 0x821d2f50
	ctx.lr = 0x82596890;
	sub_821D2F50(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825968B0);  /* forward decl */
PPC_WEAK_FUNC(sub_825968B0) { __imp__sub_825968B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825968B0) {
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
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// bne cr6,0x825968e0
	if (!ctx.cr6.eq) goto loc_825968E0;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f1,3068(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3068);
	ctx.f1.f64 = double(temp.f32);
	// b 0x825968e8
	goto loc_825968E8;
loc_825968E0:
	// li r4,1
	ctx.r4.s64 = 1;
	// bl 0x821d2f50
	ctx.lr = 0x825968E8;
	sub_821D2F50(ctx, base);
loc_825968E8:
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596908);  /* forward decl */
PPC_WEAK_FUNC(sub_82596908) { __imp__sub_82596908(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596908) {
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
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x82594298
	ctx.lr = 0x8259692C;
	sub_82594298(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596948);  /* forward decl */
PPC_WEAK_FUNC(sub_82596948) { __imp__sub_82596948(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596948) {
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
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// bl 0x82594318
	ctx.lr = 0x8259696C;
	sub_82594318(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596988);  /* forward decl */
PPC_WEAK_FUNC(sub_82596988) { __imp__sub_82596988(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596988) {
	PPC_FUNC_PROLOGUE();
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// lwz r10,4(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x825969b0
	if (!ctx.cr6.eq) goto loc_825969B0;
loc_8259699C:
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
loc_825969B0:
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x8259699c
	if (ctx.cr6.eq) goto loc_8259699C;
loc_825969B8:
	// lbz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// lbz r8,0(r10)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// subf r8,r8,r9
	ctx.r8.u64 = ctx.r9.u64 - ctx.r8.u64;
	// beq cr6,0x825969dc
	if (ctx.cr6.eq) goto loc_825969DC;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// cmpwi cr6,r8,0
	ctx.cr6.compare<int32_t>(ctx.r8.s32, 0, ctx.xer);
	// beq cr6,0x825969b8
	if (ctx.cr6.eq) goto loc_825969B8;
loc_825969DC:
	// cntlzw r11,r8
	ctx.r11.u64 = ctx.r8.u32 == 0 ? 32 : __builtin_clz(ctx.r8.u32);
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_825969F8);  /* forward decl */
PPC_WEAK_FUNC(sub_825969F8) { __imp__sub_825969F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825969F8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lfs f0,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lwz r11,4(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lfs f13,3068(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 3068);
	ctx.f13.f64 = double(temp.f32);
	// lis r10,-32255
	ctx.r10.s64 = -2113863680;
	// fcmpu cr6,f0,f13
	ctx.cr6.compare(ctx.f0.f64, ctx.f13.f64);
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// lfs f0,-22820(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + -22820);
	ctx.f0.f64 = double(temp.f32);
	// bge cr6,0x82596a34
	if (!ctx.cr6.lt) goto loc_82596A34;
loc_82596A20:
	// lfs f12,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f12.f64 = double(temp.f32);
	// fadds f12,f12,f0
	ctx.f12.f64 = double(float(ctx.f12.f64 + ctx.f0.f64));
	// stfs f12,0(r11)
	temp.f32 = float(ctx.f12.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// fcmpu cr6,f12,f13
	ctx.cr6.compare(ctx.f12.f64, ctx.f13.f64);
	// blt cr6,0x82596a20
	if (ctx.cr6.lt) goto loc_82596A20;
loc_82596A34:
	// lfs f13,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// fcmpu cr6,f13,f0
	ctx.cr6.compare(ctx.f13.f64, ctx.f0.f64);
	// blelr cr6
	if (!ctx.cr6.gt) return;
loc_82596A40:
	// lfs f13,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// fsubs f13,f13,f0
	ctx.f13.f64 = double(float(ctx.f13.f64 - ctx.f0.f64));
	// stfs f13,0(r11)
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// fcmpu cr6,f13,f0
	ctx.cr6.compare(ctx.f13.f64, ctx.f0.f64);
	// bgt cr6,0x82596a40
	if (ctx.cr6.gt) goto loc_82596A40;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596A58);  /* forward decl */
PPC_WEAK_FUNC(sub_82596A58) { __imp__sub_82596A58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596A58) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x82594cb8
	sub_82594CB8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596A68);  /* forward decl */
PPC_WEAK_FUNC(sub_82596A68) { __imp__sub_82596A68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596A68) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596A88);  /* forward decl */
PPC_WEAK_FUNC(sub_82596A88) { __imp__sub_82596A88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596A88) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x825944c8
	ctx.lr = 0x82596AAC;
	sub_825944C8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_82596AD0);  /* forward decl */
PPC_WEAK_FUNC(sub_82596AD0) { __imp__sub_82596AD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596AD0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x825945a8
	sub_825945A8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596AE8);  /* forward decl */
PPC_WEAK_FUNC(sub_82596AE8) { __imp__sub_82596AE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596AE8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r7,1
	ctx.r7.s64 = 1;
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r9,12(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r8,8(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// subf r8,r10,r8
	ctx.r8.u64 = ctx.r8.u64 - ctx.r10.u64;
	// slw r6,r9,r10
	ctx.r6.u64 = ctx.r10.u8 & 0x20 ? 0 : (ctx.r9.u32 << (ctx.r10.u8 & 0x3F));
	// addi r9,r8,1
	ctx.r9.s64 = ctx.r8.s64 + 1;
	// slw r9,r7,r9
	ctx.r9.u64 = ctx.r9.u8 & 0x20 ? 0 : (ctx.r7.u32 << (ctx.r9.u8 & 0x3F));
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// slw r10,r9,r10
	ctx.r10.u64 = ctx.r10.u8 & 0x20 ? 0 : (ctx.r9.u32 << (ctx.r10.u8 & 0x3F));
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// andc r10,r9,r10
	ctx.r10.u64 = ctx.r9.u64 & ~ctx.r10.u64;
	// or r10,r10,r6
	ctx.r10.u64 = ctx.r10.u64 | ctx.r6.u64;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596B30);  /* forward decl */
PPC_WEAK_FUNC(sub_82596B30) { __imp__sub_82596B30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596B30) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x821ea550
	sub_821EA550(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596B40);  /* forward decl */
PPC_WEAK_FUNC(sub_82596B40) { __imp__sub_82596B40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596B40) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mr r11,r4
	ctx.r11.u64 = ctx.r4.u64;
	// mr r8,r3
	ctx.r8.u64 = ctx.r3.u64;
	// lwz r10,12(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,8(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// addi r10,r10,4
	ctx.r10.s64 = ctx.r10.s64 + 4;
	// rlwinm r7,r10,2,0,29
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwz r10,4(r9)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r9.u32 + 4);
	// lwz r3,0(r9)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
	// lfs f0,8(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// stwx r10,r7,r11
	PPC_STORE_U32(ctx.r7.u32 + ctx.r11.u32, ctx.r10.u32);
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
	// lwz r7,8(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// addi r9,r10,1
	ctx.r9.s64 = ctx.r10.s64 + 1;
	// addi r10,r10,2
	ctx.r10.s64 = ctx.r10.s64 + 2;
	// rlwinm r10,r10,4,0,27
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
	// add r4,r10,r11
	ctx.r4.u64 = ctx.r10.u64 + ctx.r11.u64;
	// stw r9,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r9.u32);
	// addi r10,r9,4
	ctx.r10.s64 = ctx.r9.s64 + 4;
	// rlwinm r6,r10,2,0,29
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwz r10,8(r7)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r7.u32 + 8);
	// lfs f0,8(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// stwx r10,r6,r11
	PPC_STORE_U32(ctx.r6.u32 + ctx.r11.u32, ctx.r10.u32);
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
	// addi r7,r10,1
	ctx.r7.s64 = ctx.r10.s64 + 1;
	// addi r9,r10,2
	ctx.r9.s64 = ctx.r10.s64 + 2;
	// rlwinm r10,r9,4,0,27
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 4) & 0xFFFFFFF0;
	// add r5,r10,r11
	ctx.r5.u64 = ctx.r10.u64 + ctx.r11.u64;
	// stw r7,12(r11)
	PPC_STORE_U32(ctx.r11.u32 + 12, ctx.r7.u32);
	// mtctr r8
	ctx.ctr.u64 = ctx.r8.u64;
	// bctr 
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82596C20);  /* forward decl */
PPC_WEAK_FUNC(sub_82596C20) { __imp__sub_82596C20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82596C20) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,19872
	ctx.r4.s64 = ctx.r11.s64 + 19872;
	// addi r3,r10,31880
	ctx.r3.s64 = ctx.r10.s64 + 31880;
	// bl 0x827d6c90
	ctx.lr = 0x82596C40;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,25472
	ctx.r4.s64 = ctx.r11.s64 + 25472;
	// addi r3,r10,31856
	ctx.r3.s64 = ctx.r10.s64 + 31856;
	// bl 0x827d6c90
	ctx.lr = 0x82596C54;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,25568
	ctx.r4.s64 = ctx.r11.s64 + 25568;
	// addi r3,r10,31836
	ctx.r3.s64 = ctx.r10.s64 + 31836;
	// bl 0x827d6c90
	ctx.lr = 0x82596C68;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,19952
	ctx.r4.s64 = ctx.r11.s64 + 19952;
	// addi r3,r10,31820
	ctx.r3.s64 = ctx.r10.s64 + 31820;
	// bl 0x827d6c90
	ctx.lr = 0x82596C7C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,19984
	ctx.r4.s64 = ctx.r11.s64 + 19984;
	// addi r3,r10,31800
	ctx.r3.s64 = ctx.r10.s64 + 31800;
	// bl 0x827d6c90
	ctx.lr = 0x82596C90;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,20072
	ctx.r4.s64 = ctx.r11.s64 + 20072;
	// addi r3,r10,31780
	ctx.r3.s64 = ctx.r10.s64 + 31780;
	// bl 0x827d6c90
	ctx.lr = 0x82596CA4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,20128
	ctx.r4.s64 = ctx.r11.s64 + 20128;
	// addi r3,r10,31764
	ctx.r3.s64 = ctx.r10.s64 + 31764;
	// bl 0x827d6c90
	ctx.lr = 0x82596CB8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,20240
	ctx.r4.s64 = ctx.r11.s64 + 20240;
	// addi r3,r10,31744
	ctx.r3.s64 = ctx.r10.s64 + 31744;
	// bl 0x827d6c90
	ctx.lr = 0x82596CCC;
	sub_827D6C90(ctx, base);
	// lis r11,-32141
	ctx.r11.s64 = -2106392576;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-16408
	ctx.r4.s64 = ctx.r11.s64 + -16408;
	// addi r3,r10,31724
	ctx.r3.s64 = ctx.r10.s64 + 31724;
	// bl 0x827d6c90
	ctx.lr = 0x82596CE0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18208
	ctx.r4.s64 = ctx.r11.s64 + 18208;
	// addi r3,r10,31708
	ctx.r3.s64 = ctx.r10.s64 + 31708;
	// bl 0x827d6c90
	ctx.lr = 0x82596CF4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,25640
	ctx.r4.s64 = ctx.r11.s64 + 25640;
	// addi r3,r10,31688
	ctx.r3.s64 = ctx.r10.s64 + 31688;
	// bl 0x827d6c90
	ctx.lr = 0x82596D08;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,25704
	ctx.r4.s64 = ctx.r11.s64 + 25704;
	// addi r3,r10,31660
	ctx.r3.s64 = ctx.r10.s64 + 31660;
	// bl 0x827d6c90
	ctx.lr = 0x82596D1C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,20352
	ctx.r4.s64 = ctx.r11.s64 + 20352;
	// addi r3,r10,31648
	ctx.r3.s64 = ctx.r10.s64 + 31648;
	// bl 0x827d6c90
	ctx.lr = 0x82596D30;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,25760
	ctx.r4.s64 = ctx.r11.s64 + 25760;
	// addi r3,r10,31632
	ctx.r3.s64 = ctx.r10.s64 + 31632;
	// bl 0x827d6c90
	ctx.lr = 0x82596D44;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,25784
	ctx.r4.s64 = ctx.r11.s64 + 25784;
	// addi r3,r10,31616
	ctx.r3.s64 = ctx.r10.s64 + 31616;
	// bl 0x827d6c90
	ctx.lr = 0x82596D58;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,25816
	ctx.r4.s64 = ctx.r11.s64 + 25816;
	// addi r3,r10,31584
	ctx.r3.s64 = ctx.r10.s64 + 31584;
	// bl 0x827d6c90
	ctx.lr = 0x82596D6C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,25928
	ctx.r4.s64 = ctx.r11.s64 + 25928;
	// addi r3,r10,31552
	ctx.r3.s64 = ctx.r10.s64 + 31552;
	// bl 0x827d6c90
	ctx.lr = 0x82596D80;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,26032
	ctx.r4.s64 = ctx.r11.s64 + 26032;
	// addi r3,r10,31524
	ctx.r3.s64 = ctx.r10.s64 + 31524;
	// bl 0x827d6c90
	ctx.lr = 0x82596D94;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,20376
	ctx.r4.s64 = ctx.r11.s64 + 20376;
	// addi r3,r10,31516
	ctx.r3.s64 = ctx.r10.s64 + 31516;
	// bl 0x827d6c90
	ctx.lr = 0x82596DA8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,20456
	ctx.r4.s64 = ctx.r11.s64 + 20456;
	// addi r3,r10,31508
	ctx.r3.s64 = ctx.r10.s64 + 31508;
	// bl 0x827d6c90
	ctx.lr = 0x82596DBC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,20536
	ctx.r4.s64 = ctx.r11.s64 + 20536;
	// addi r3,r10,31504
	ctx.r3.s64 = ctx.r10.s64 + 31504;
	// bl 0x827d6c90
	ctx.lr = 0x82596DD0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,20616
	ctx.r4.s64 = ctx.r11.s64 + 20616;
	// addi r3,r10,31496
	ctx.r3.s64 = ctx.r10.s64 + 31496;
	// bl 0x827d6c90
	ctx.lr = 0x82596DE4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,20704
	ctx.r4.s64 = ctx.r11.s64 + 20704;
	// addi r3,r10,31488
	ctx.r3.s64 = ctx.r10.s64 + 31488;
	// bl 0x827d6c90
	ctx.lr = 0x82596DF8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,20784
	ctx.r4.s64 = ctx.r11.s64 + 20784;
	// addi r3,r10,31468
	ctx.r3.s64 = ctx.r10.s64 + 31468;
	// bl 0x827d6c90
	ctx.lr = 0x82596E0C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,20800
	ctx.r4.s64 = ctx.r11.s64 + 20800;
	// addi r3,r10,31428
	ctx.r3.s64 = ctx.r10.s64 + 31428;
	// bl 0x827d6c90
	ctx.lr = 0x82596E20;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,20888
	ctx.r4.s64 = ctx.r11.s64 + 20888;
	// addi r3,r10,31416
	ctx.r3.s64 = ctx.r10.s64 + 31416;
	// bl 0x827d6c90
	ctx.lr = 0x82596E34;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,20936
	ctx.r4.s64 = ctx.r11.s64 + 20936;
	// addi r3,r10,31396
	ctx.r3.s64 = ctx.r10.s64 + 31396;
	// bl 0x827d6c90
	ctx.lr = 0x82596E48;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,25368
	ctx.r4.s64 = ctx.r11.s64 + 25368;
	// addi r3,r10,31368
	ctx.r3.s64 = ctx.r10.s64 + 31368;
	// bl 0x827d6c90
	ctx.lr = 0x82596E5C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21072
	ctx.r4.s64 = ctx.r11.s64 + 21072;
	// addi r3,r10,31344
	ctx.r3.s64 = ctx.r10.s64 + 31344;
	// bl 0x827d6c90
	ctx.lr = 0x82596E70;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21096
	ctx.r4.s64 = ctx.r11.s64 + 21096;
	// addi r3,r10,31324
	ctx.r3.s64 = ctx.r10.s64 + 31324;
	// bl 0x827d6c90
	ctx.lr = 0x82596E84;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21224
	ctx.r4.s64 = ctx.r11.s64 + 21224;
	// addi r3,r10,31304
	ctx.r3.s64 = ctx.r10.s64 + 31304;
	// bl 0x827d6c90
	ctx.lr = 0x82596E98;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18216
	ctx.r4.s64 = ctx.r11.s64 + 18216;
	// addi r3,r10,31284
	ctx.r3.s64 = ctx.r10.s64 + 31284;
	// bl 0x827d6c90
	ctx.lr = 0x82596EAC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// addi r4,r11,21352
	ctx.r4.s64 = ctx.r11.s64 + 21352;
	// lis r11,-32253
	ctx.r11.s64 = -2113732608;
	// addi r3,r11,31252
	ctx.r3.s64 = ctx.r11.s64 + 31252;
	// bl 0x827d6c90
	ctx.lr = 0x82596EC0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18224
	ctx.r4.s64 = ctx.r11.s64 + 18224;
	// addi r3,r10,31220
	ctx.r3.s64 = ctx.r10.s64 + 31220;
	// bl 0x827d6c90
	ctx.lr = 0x82596ED4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18232
	ctx.r4.s64 = ctx.r11.s64 + 18232;
	// addi r3,r10,31184
	ctx.r3.s64 = ctx.r10.s64 + 31184;
	// bl 0x827d6c90
	ctx.lr = 0x82596EE8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18264
	ctx.r4.s64 = ctx.r11.s64 + 18264;
	// addi r3,r10,31168
	ctx.r3.s64 = ctx.r10.s64 + 31168;
	// bl 0x827d6c90
	ctx.lr = 0x82596EFC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18272
	ctx.r4.s64 = ctx.r11.s64 + 18272;
	// addi r3,r10,31152
	ctx.r3.s64 = ctx.r10.s64 + 31152;
	// bl 0x827d6c90
	ctx.lr = 0x82596F10;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21392
	ctx.r4.s64 = ctx.r11.s64 + 21392;
	// addi r3,r10,31128
	ctx.r3.s64 = ctx.r10.s64 + 31128;
	// bl 0x827d6c90
	ctx.lr = 0x82596F24;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21424
	ctx.r4.s64 = ctx.r11.s64 + 21424;
	// addi r3,r10,31092
	ctx.r3.s64 = ctx.r10.s64 + 31092;
	// bl 0x827d6c90
	ctx.lr = 0x82596F38;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,26136
	ctx.r4.s64 = ctx.r11.s64 + 26136;
	// addi r3,r10,31052
	ctx.r3.s64 = ctx.r10.s64 + 31052;
	// bl 0x827d6c90
	ctx.lr = 0x82596F4C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18280
	ctx.r4.s64 = ctx.r11.s64 + 18280;
	// addi r3,r10,31012
	ctx.r3.s64 = ctx.r10.s64 + 31012;
	// bl 0x827d6c90
	ctx.lr = 0x82596F60;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18328
	ctx.r4.s64 = ctx.r11.s64 + 18328;
	// addi r3,r10,30972
	ctx.r3.s64 = ctx.r10.s64 + 30972;
	// bl 0x827d6c90
	ctx.lr = 0x82596F74;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18368
	ctx.r4.s64 = ctx.r11.s64 + 18368;
	// addi r3,r10,30912
	ctx.r3.s64 = ctx.r10.s64 + 30912;
	// bl 0x827d6c90
	ctx.lr = 0x82596F88;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18384
	ctx.r4.s64 = ctx.r11.s64 + 18384;
	// addi r3,r10,30884
	ctx.r3.s64 = ctx.r10.s64 + 30884;
	// bl 0x827d6c90
	ctx.lr = 0x82596F9C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21456
	ctx.r4.s64 = ctx.r11.s64 + 21456;
	// addi r3,r10,30840
	ctx.r3.s64 = ctx.r10.s64 + 30840;
	// bl 0x827d6c90
	ctx.lr = 0x82596FB0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21560
	ctx.r4.s64 = ctx.r11.s64 + 21560;
	// addi r3,r10,30804
	ctx.r3.s64 = ctx.r10.s64 + 30804;
	// bl 0x827d6c90
	ctx.lr = 0x82596FC4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,26152
	ctx.r4.s64 = ctx.r11.s64 + 26152;
	// addi r3,r10,30772
	ctx.r3.s64 = ctx.r10.s64 + 30772;
	// bl 0x827d6c90
	ctx.lr = 0x82596FD8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,26208
	ctx.r4.s64 = ctx.r11.s64 + 26208;
	// addi r3,r10,30740
	ctx.r3.s64 = ctx.r10.s64 + 30740;
	// bl 0x827d6c90
	ctx.lr = 0x82596FEC;
	sub_827D6C90(ctx, base);
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// addi r3,r10,30708
	ctx.r3.s64 = ctx.r10.s64 + 30708;
	// addi r4,r11,26328
	ctx.r4.s64 = ctx.r11.s64 + 26328;
	// bl 0x827d6c90
	ctx.lr = 0x82597000;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,26360
	ctx.r4.s64 = ctx.r11.s64 + 26360;
	// addi r3,r10,30680
	ctx.r3.s64 = ctx.r10.s64 + 30680;
	// bl 0x827d6c90
	ctx.lr = 0x82597014;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21656
	ctx.r4.s64 = ctx.r11.s64 + 21656;
	// addi r3,r10,30660
	ctx.r3.s64 = ctx.r10.s64 + 30660;
	// bl 0x827d6c90
	ctx.lr = 0x82597028;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18424
	ctx.r4.s64 = ctx.r11.s64 + 18424;
	// addi r3,r10,30640
	ctx.r3.s64 = ctx.r10.s64 + 30640;
	// bl 0x827d6c90
	ctx.lr = 0x8259703C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21736
	ctx.r4.s64 = ctx.r11.s64 + 21736;
	// addi r3,r10,30608
	ctx.r3.s64 = ctx.r10.s64 + 30608;
	// bl 0x827d6c90
	ctx.lr = 0x82597050;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21816
	ctx.r4.s64 = ctx.r11.s64 + 21816;
	// addi r3,r10,30568
	ctx.r3.s64 = ctx.r10.s64 + 30568;
	// bl 0x827d6c90
	ctx.lr = 0x82597064;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,21952
	ctx.r4.s64 = ctx.r11.s64 + 21952;
	// addi r3,r10,30524
	ctx.r3.s64 = ctx.r10.s64 + 30524;
	// bl 0x827d6c90
	ctx.lr = 0x82597078;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22032
	ctx.r4.s64 = ctx.r11.s64 + 22032;
	// addi r3,r10,30476
	ctx.r3.s64 = ctx.r10.s64 + 30476;
	// bl 0x827d6c90
	ctx.lr = 0x8259708C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22168
	ctx.r4.s64 = ctx.r11.s64 + 22168;
	// addi r3,r10,30432
	ctx.r3.s64 = ctx.r10.s64 + 30432;
	// bl 0x827d6c90
	ctx.lr = 0x825970A0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22224
	ctx.r4.s64 = ctx.r11.s64 + 22224;
	// addi r3,r10,30408
	ctx.r3.s64 = ctx.r10.s64 + 30408;
	// bl 0x827d6c90
	ctx.lr = 0x825970B4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22256
	ctx.r4.s64 = ctx.r11.s64 + 22256;
	// addi r3,r10,30388
	ctx.r3.s64 = ctx.r10.s64 + 30388;
	// bl 0x827d6c90
	ctx.lr = 0x825970C8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22288
	ctx.r4.s64 = ctx.r11.s64 + 22288;
	// addi r3,r10,30364
	ctx.r3.s64 = ctx.r10.s64 + 30364;
	// bl 0x827d6c90
	ctx.lr = 0x825970DC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18432
	ctx.r4.s64 = ctx.r11.s64 + 18432;
	// addi r3,r10,30340
	ctx.r3.s64 = ctx.r10.s64 + 30340;
	// bl 0x827d6c90
	ctx.lr = 0x825970F0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22416
	ctx.r4.s64 = ctx.r11.s64 + 22416;
	// addi r3,r10,30304
	ctx.r3.s64 = ctx.r10.s64 + 30304;
	// bl 0x827d6c90
	ctx.lr = 0x82597104;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22496
	ctx.r4.s64 = ctx.r11.s64 + 22496;
	// addi r3,r10,30264
	ctx.r3.s64 = ctx.r10.s64 + 30264;
	// bl 0x827d6c90
	ctx.lr = 0x82597118;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22576
	ctx.r4.s64 = ctx.r11.s64 + 22576;
	// addi r3,r10,30228
	ctx.r3.s64 = ctx.r10.s64 + 30228;
	// bl 0x827d6c90
	ctx.lr = 0x8259712C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,26384
	ctx.r4.s64 = ctx.r11.s64 + 26384;
	// addi r3,r10,30208
	ctx.r3.s64 = ctx.r10.s64 + 30208;
	// bl 0x827d6c90
	ctx.lr = 0x82597140;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22608
	ctx.r4.s64 = ctx.r11.s64 + 22608;
	// addi r3,r10,30188
	ctx.r3.s64 = ctx.r10.s64 + 30188;
	// bl 0x827d6c90
	ctx.lr = 0x82597154;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18440
	ctx.r4.s64 = ctx.r11.s64 + 18440;
	// addi r3,r10,30172
	ctx.r3.s64 = ctx.r10.s64 + 30172;
	// bl 0x827d6c90
	ctx.lr = 0x82597168;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22640
	ctx.r4.s64 = ctx.r11.s64 + 22640;
	// addi r3,r10,30144
	ctx.r3.s64 = ctx.r10.s64 + 30144;
	// bl 0x827d6c90
	ctx.lr = 0x8259717C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22712
	ctx.r4.s64 = ctx.r11.s64 + 22712;
	// addi r3,r10,30112
	ctx.r3.s64 = ctx.r10.s64 + 30112;
	// bl 0x827d6c90
	ctx.lr = 0x82597190;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22792
	ctx.r4.s64 = ctx.r11.s64 + 22792;
	// addi r3,r10,30088
	ctx.r3.s64 = ctx.r10.s64 + 30088;
	// bl 0x827d6c90
	ctx.lr = 0x825971A4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22864
	ctx.r4.s64 = ctx.r11.s64 + 22864;
	// addi r3,r10,30064
	ctx.r3.s64 = ctx.r10.s64 + 30064;
	// bl 0x827d6c90
	ctx.lr = 0x825971B8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22912
	ctx.r4.s64 = ctx.r11.s64 + 22912;
	// addi r3,r10,30044
	ctx.r3.s64 = ctx.r10.s64 + 30044;
	// bl 0x827d6c90
	ctx.lr = 0x825971CC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,27648
	ctx.r4.s64 = ctx.r11.s64 + 27648;
	// addi r3,r10,30020
	ctx.r3.s64 = ctx.r10.s64 + 30020;
	// bl 0x827d6c90
	ctx.lr = 0x825971E0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,22952
	ctx.r4.s64 = ctx.r11.s64 + 22952;
	// addi r3,r10,29996
	ctx.r3.s64 = ctx.r10.s64 + 29996;
	// bl 0x827d6c90
	ctx.lr = 0x825971F4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,23048
	ctx.r4.s64 = ctx.r11.s64 + 23048;
	// addi r3,r10,29972
	ctx.r3.s64 = ctx.r10.s64 + 29972;
	// bl 0x827d6c90
	ctx.lr = 0x82597208;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,26480
	ctx.r4.s64 = ctx.r11.s64 + 26480;
	// addi r3,r10,29952
	ctx.r3.s64 = ctx.r10.s64 + 29952;
	// bl 0x827d6c90
	ctx.lr = 0x8259721C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,23104
	ctx.r4.s64 = ctx.r11.s64 + 23104;
	// addi r3,r10,29928
	ctx.r3.s64 = ctx.r10.s64 + 29928;
	// bl 0x827d6c90
	ctx.lr = 0x82597230;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18448
	ctx.r4.s64 = ctx.r11.s64 + 18448;
	// addi r3,r10,29908
	ctx.r3.s64 = ctx.r10.s64 + 29908;
	// bl 0x827d6c90
	ctx.lr = 0x82597244;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,23160
	ctx.r4.s64 = ctx.r11.s64 + 23160;
	// addi r3,r10,29896
	ctx.r3.s64 = ctx.r10.s64 + 29896;
	// bl 0x827d6c90
	ctx.lr = 0x82597258;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,26576
	ctx.r4.s64 = ctx.r11.s64 + 26576;
	// addi r3,r10,29888
	ctx.r3.s64 = ctx.r10.s64 + 29888;
	// bl 0x827d6c90
	ctx.lr = 0x8259726C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,26616
	ctx.r4.s64 = ctx.r11.s64 + 26616;
	// addi r3,r10,29876
	ctx.r3.s64 = ctx.r10.s64 + 29876;
	// bl 0x827d6c90
	ctx.lr = 0x82597280;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,26656
	ctx.r4.s64 = ctx.r11.s64 + 26656;
	// addi r3,r10,29860
	ctx.r3.s64 = ctx.r10.s64 + 29860;
	// bl 0x827d6c90
	ctx.lr = 0x82597294;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,23208
	ctx.r4.s64 = ctx.r11.s64 + 23208;
	// addi r3,r10,29832
	ctx.r3.s64 = ctx.r10.s64 + 29832;
	// bl 0x827d6c90
	ctx.lr = 0x825972A8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,23240
	ctx.r4.s64 = ctx.r11.s64 + 23240;
	// addi r3,r10,29808
	ctx.r3.s64 = ctx.r10.s64 + 29808;
	// bl 0x827d6c90
	ctx.lr = 0x825972BC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,23280
	ctx.r4.s64 = ctx.r11.s64 + 23280;
	// addi r3,r10,29780
	ctx.r3.s64 = ctx.r10.s64 + 29780;
	// bl 0x827d6c90
	ctx.lr = 0x825972D0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,23368
	ctx.r4.s64 = ctx.r11.s64 + 23368;
	// addi r3,r10,29760
	ctx.r3.s64 = ctx.r10.s64 + 29760;
	// bl 0x827d6c90
	ctx.lr = 0x825972E4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,23408
	ctx.r4.s64 = ctx.r11.s64 + 23408;
	// addi r3,r10,29736
	ctx.r3.s64 = ctx.r10.s64 + 29736;
	// bl 0x827d6c90
	ctx.lr = 0x825972F8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,23432
	ctx.r4.s64 = ctx.r11.s64 + 23432;
	// addi r3,r10,29716
	ctx.r3.s64 = ctx.r10.s64 + 29716;
	// bl 0x827d6c90
	ctx.lr = 0x8259730C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,23504
	ctx.r4.s64 = ctx.r11.s64 + 23504;
	// addi r3,r10,29700
	ctx.r3.s64 = ctx.r10.s64 + 29700;
	// bl 0x827d6c90
	ctx.lr = 0x82597320;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,26720
	ctx.r4.s64 = ctx.r11.s64 + 26720;
	// addi r3,r10,29680
	ctx.r3.s64 = ctx.r10.s64 + 29680;
	// bl 0x827d6c90
	ctx.lr = 0x82597334;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,26800
	ctx.r4.s64 = ctx.r11.s64 + 26800;
	// addi r3,r10,29652
	ctx.r3.s64 = ctx.r10.s64 + 29652;
	// bl 0x827d6c90
	ctx.lr = 0x82597348;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,26888
	ctx.r4.s64 = ctx.r11.s64 + 26888;
	// addi r3,r10,29620
	ctx.r3.s64 = ctx.r10.s64 + 29620;
	// bl 0x827d6c90
	ctx.lr = 0x8259735C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,26952
	ctx.r4.s64 = ctx.r11.s64 + 26952;
	// addi r3,r10,29588
	ctx.r3.s64 = ctx.r10.s64 + 29588;
	// bl 0x827d6c90
	ctx.lr = 0x82597370;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,27016
	ctx.r4.s64 = ctx.r11.s64 + 27016;
	// addi r3,r10,29572
	ctx.r3.s64 = ctx.r10.s64 + 29572;
	// bl 0x827d6c90
	ctx.lr = 0x82597384;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,23560
	ctx.r4.s64 = ctx.r11.s64 + 23560;
	// addi r3,r10,29564
	ctx.r3.s64 = ctx.r10.s64 + 29564;
	// bl 0x827d6c90
	ctx.lr = 0x82597398;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,23592
	ctx.r4.s64 = ctx.r11.s64 + 23592;
	// addi r3,r10,29556
	ctx.r3.s64 = ctx.r10.s64 + 29556;
	// bl 0x827d6c90
	ctx.lr = 0x825973AC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,23616
	ctx.r4.s64 = ctx.r11.s64 + 23616;
	// addi r3,r10,29528
	ctx.r3.s64 = ctx.r10.s64 + 29528;
	// bl 0x827d6c90
	ctx.lr = 0x825973C0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,23704
	ctx.r4.s64 = ctx.r11.s64 + 23704;
	// addi r3,r10,29504
	ctx.r3.s64 = ctx.r10.s64 + 29504;
	// bl 0x827d6c90
	ctx.lr = 0x825973D4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,23792
	ctx.r4.s64 = ctx.r11.s64 + 23792;
	// addi r3,r10,29484
	ctx.r3.s64 = ctx.r10.s64 + 29484;
	// bl 0x827d6c90
	ctx.lr = 0x825973E8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,23944
	ctx.r4.s64 = ctx.r11.s64 + 23944;
	// addi r3,r10,29464
	ctx.r3.s64 = ctx.r10.s64 + 29464;
	// bl 0x827d6c90
	ctx.lr = 0x825973FC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,27128
	ctx.r4.s64 = ctx.r11.s64 + 27128;
	// addi r3,r10,29452
	ctx.r3.s64 = ctx.r10.s64 + 29452;
	// bl 0x827d6c90
	ctx.lr = 0x82597410;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24048
	ctx.r4.s64 = ctx.r11.s64 + 24048;
	// addi r3,r10,29436
	ctx.r3.s64 = ctx.r10.s64 + 29436;
	// bl 0x827d6c90
	ctx.lr = 0x82597424;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24160
	ctx.r4.s64 = ctx.r11.s64 + 24160;
	// addi r3,r10,29416
	ctx.r3.s64 = ctx.r10.s64 + 29416;
	// bl 0x827d6c90
	ctx.lr = 0x82597438;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-4208
	ctx.r4.s64 = ctx.r11.s64 + -4208;
	// addi r3,r10,29396
	ctx.r3.s64 = ctx.r10.s64 + 29396;
	// bl 0x827d6c90
	ctx.lr = 0x8259744C;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-4208
	ctx.r4.s64 = ctx.r11.s64 + -4208;
	// addi r3,r10,29376
	ctx.r3.s64 = ctx.r10.s64 + 29376;
	// bl 0x827d6c90
	ctx.lr = 0x82597460;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-4208
	ctx.r4.s64 = ctx.r11.s64 + -4208;
	// addi r3,r10,29356
	ctx.r3.s64 = ctx.r10.s64 + 29356;
	// bl 0x827d6c90
	ctx.lr = 0x82597474;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,27664
	ctx.r4.s64 = ctx.r11.s64 + 27664;
	// addi r3,r10,29312
	ctx.r3.s64 = ctx.r10.s64 + 29312;
	// bl 0x827d6c90
	ctx.lr = 0x82597488;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,27224
	ctx.r4.s64 = ctx.r11.s64 + 27224;
	// addi r3,r10,29288
	ctx.r3.s64 = ctx.r10.s64 + 29288;
	// bl 0x827d6c90
	ctx.lr = 0x8259749C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,27240
	ctx.r4.s64 = ctx.r11.s64 + 27240;
	// addi r3,r10,29272
	ctx.r3.s64 = ctx.r10.s64 + 29272;
	// bl 0x827d6c90
	ctx.lr = 0x825974B0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,27272
	ctx.r4.s64 = ctx.r11.s64 + 27272;
	// addi r3,r10,29256
	ctx.r3.s64 = ctx.r10.s64 + 29256;
	// bl 0x827d6c90
	ctx.lr = 0x825974C4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,27344
	ctx.r4.s64 = ctx.r11.s64 + 27344;
	// addi r3,r10,29236
	ctx.r3.s64 = ctx.r10.s64 + 29236;
	// bl 0x827d6c90
	ctx.lr = 0x825974D8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24176
	ctx.r4.s64 = ctx.r11.s64 + 24176;
	// addi r3,r10,29184
	ctx.r3.s64 = ctx.r10.s64 + 29184;
	// bl 0x827d6c90
	ctx.lr = 0x825974EC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24208
	ctx.r4.s64 = ctx.r11.s64 + 24208;
	// addi r3,r10,29132
	ctx.r3.s64 = ctx.r10.s64 + 29132;
	// bl 0x827d6c90
	ctx.lr = 0x82597500;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// addi r4,r11,24232
	ctx.r4.s64 = ctx.r11.s64 + 24232;
	// lis r11,-32253
	ctx.r11.s64 = -2113732608;
	// addi r3,r11,29108
	ctx.r3.s64 = ctx.r11.s64 + 29108;
	// bl 0x827d6c90
	ctx.lr = 0x82597514;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24272
	ctx.r4.s64 = ctx.r11.s64 + 24272;
	// addi r3,r10,29084
	ctx.r3.s64 = ctx.r10.s64 + 29084;
	// bl 0x827d6c90
	ctx.lr = 0x82597528;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,27368
	ctx.r4.s64 = ctx.r11.s64 + 27368;
	// addi r3,r10,29064
	ctx.r3.s64 = ctx.r10.s64 + 29064;
	// bl 0x827d6c90
	ctx.lr = 0x8259753C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24312
	ctx.r4.s64 = ctx.r11.s64 + 24312;
	// addi r3,r10,29044
	ctx.r3.s64 = ctx.r10.s64 + 29044;
	// bl 0x827d6c90
	ctx.lr = 0x82597550;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,25384
	ctx.r4.s64 = ctx.r11.s64 + 25384;
	// addi r3,r10,29028
	ctx.r3.s64 = ctx.r10.s64 + 29028;
	// bl 0x827d6c90
	ctx.lr = 0x82597564;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24368
	ctx.r4.s64 = ctx.r11.s64 + 24368;
	// addi r3,r10,28996
	ctx.r3.s64 = ctx.r10.s64 + 28996;
	// bl 0x827d6c90
	ctx.lr = 0x82597578;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18456
	ctx.r4.s64 = ctx.r11.s64 + 18456;
	// addi r3,r10,28984
	ctx.r3.s64 = ctx.r10.s64 + 28984;
	// bl 0x827d6c90
	ctx.lr = 0x8259758C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18496
	ctx.r4.s64 = ctx.r11.s64 + 18496;
	// addi r3,r10,28968
	ctx.r3.s64 = ctx.r10.s64 + 28968;
	// bl 0x827d6c90
	ctx.lr = 0x825975A0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24400
	ctx.r4.s64 = ctx.r11.s64 + 24400;
	// addi r3,r10,28936
	ctx.r3.s64 = ctx.r10.s64 + 28936;
	// bl 0x827d6c90
	ctx.lr = 0x825975B4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24464
	ctx.r4.s64 = ctx.r11.s64 + 24464;
	// addi r3,r10,28880
	ctx.r3.s64 = ctx.r10.s64 + 28880;
	// bl 0x827d6c90
	ctx.lr = 0x825975C8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18536
	ctx.r4.s64 = ctx.r11.s64 + 18536;
	// addi r3,r10,28856
	ctx.r3.s64 = ctx.r10.s64 + 28856;
	// bl 0x827d6c90
	ctx.lr = 0x825975DC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18576
	ctx.r4.s64 = ctx.r11.s64 + 18576;
	// addi r3,r10,28832
	ctx.r3.s64 = ctx.r10.s64 + 28832;
	// bl 0x827d6c90
	ctx.lr = 0x825975F0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,27440
	ctx.r4.s64 = ctx.r11.s64 + 27440;
	// addi r3,r10,28800
	ctx.r3.s64 = ctx.r10.s64 + 28800;
	// bl 0x827d6c90
	ctx.lr = 0x82597604;
	sub_827D6C90(ctx, base);
	// lis r11,-32166
	ctx.r11.s64 = -2108030976;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,-4208
	ctx.r4.s64 = ctx.r11.s64 + -4208;
	// addi r3,r10,28780
	ctx.r3.s64 = ctx.r10.s64 + 28780;
	// bl 0x827d6c90
	ctx.lr = 0x82597618;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24528
	ctx.r4.s64 = ctx.r11.s64 + 24528;
	// addi r3,r10,28764
	ctx.r3.s64 = ctx.r10.s64 + 28764;
	// bl 0x827d6c90
	ctx.lr = 0x8259762C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24544
	ctx.r4.s64 = ctx.r11.s64 + 24544;
	// addi r3,r10,28740
	ctx.r3.s64 = ctx.r10.s64 + 28740;
	// bl 0x827d6c90
	ctx.lr = 0x82597640;
	sub_827D6C90(ctx, base);
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// addi r3,r10,28720
	ctx.r3.s64 = ctx.r10.s64 + 28720;
	// addi r4,r11,24616
	ctx.r4.s64 = ctx.r11.s64 + 24616;
	// bl 0x827d6c90
	ctx.lr = 0x82597654;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24648
	ctx.r4.s64 = ctx.r11.s64 + 24648;
	// addi r3,r10,28696
	ctx.r3.s64 = ctx.r10.s64 + 28696;
	// bl 0x827d6c90
	ctx.lr = 0x82597668;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24680
	ctx.r4.s64 = ctx.r11.s64 + 24680;
	// addi r3,r10,28664
	ctx.r3.s64 = ctx.r10.s64 + 28664;
	// bl 0x827d6c90
	ctx.lr = 0x8259767C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18584
	ctx.r4.s64 = ctx.r11.s64 + 18584;
	// addi r3,r10,28624
	ctx.r3.s64 = ctx.r10.s64 + 28624;
	// bl 0x827d6c90
	ctx.lr = 0x82597690;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24704
	ctx.r4.s64 = ctx.r11.s64 + 24704;
	// addi r3,r10,28596
	ctx.r3.s64 = ctx.r10.s64 + 28596;
	// bl 0x827d6c90
	ctx.lr = 0x825976A4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18592
	ctx.r4.s64 = ctx.r11.s64 + 18592;
	// addi r3,r10,28564
	ctx.r3.s64 = ctx.r10.s64 + 28564;
	// bl 0x827d6c90
	ctx.lr = 0x825976B8;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24736
	ctx.r4.s64 = ctx.r11.s64 + 24736;
	// addi r3,r10,28532
	ctx.r3.s64 = ctx.r10.s64 + 28532;
	// bl 0x827d6c90
	ctx.lr = 0x825976CC;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24768
	ctx.r4.s64 = ctx.r11.s64 + 24768;
	// addi r3,r10,28512
	ctx.r3.s64 = ctx.r10.s64 + 28512;
	// bl 0x827d6c90
	ctx.lr = 0x825976E0;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24872
	ctx.r4.s64 = ctx.r11.s64 + 24872;
	// addi r3,r10,28492
	ctx.r3.s64 = ctx.r10.s64 + 28492;
	// bl 0x827d6c90
	ctx.lr = 0x825976F4;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18648
	ctx.r4.s64 = ctx.r11.s64 + 18648;
	// addi r3,r10,28444
	ctx.r3.s64 = ctx.r10.s64 + 28444;
	// bl 0x827d6c90
	ctx.lr = 0x82597708;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,18672
	ctx.r4.s64 = ctx.r11.s64 + 18672;
	// addi r3,r10,28428
	ctx.r3.s64 = ctx.r10.s64 + 28428;
	// bl 0x827d6c90
	ctx.lr = 0x8259771C;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24896
	ctx.r4.s64 = ctx.r11.s64 + 24896;
	// addi r3,r10,28408
	ctx.r3.s64 = ctx.r10.s64 + 28408;
	// bl 0x827d6c90
	ctx.lr = 0x82597730;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,24920
	ctx.r4.s64 = ctx.r11.s64 + 24920;
	// addi r3,r10,28384
	ctx.r3.s64 = ctx.r10.s64 + 28384;
	// bl 0x827d6c90
	ctx.lr = 0x82597744;
	sub_827D6C90(ctx, base);
	// lis r11,-32167
	ctx.r11.s64 = -2108096512;
	// lis r10,-32253
	ctx.r10.s64 = -2113732608;
	// addi r4,r11,25400
	ctx.r4.s64 = ctx.r11.s64 + 25400;
	// addi r3,r10,28364
	ctx.r3.s64 = ctx.r10.s64 + 28364;
	// bl 0x827d6c90
	ctx.lr = 0x82597758;
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

PPC_FUNC_IMPL(__imp__sub_82597768);  /* forward decl */
PPC_WEAK_FUNC(sub_82597768) { __imp__sub_82597768(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82597768) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x82597770;
	__savegprlr_28(ctx, base);
	// stwu r1,-192(r1)
	ea = -192 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,12544
	ctx.r31.s64 = ctx.r11.s64 + 12544;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// bl 0x82273620
	ctx.lr = 0x82597790;
	sub_82273620(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// li r10,0
	ctx.r10.s64 = 0;
	// lwz r4,352(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
	// mr r8,r28
	ctx.r8.u64 = ctx.r28.u64;
	// mr r7,r29
	ctx.r7.u64 = ctx.r29.u64;
	// li r6,-1
	ctx.r6.s64 = -1;
	// lbz r11,-22652(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -22652);
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,0
	ctx.r9.s64 = 0;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x8221ffa0
	ctx.lr = 0x825977EC;
	sub_8221FFA0(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-22652(r30)
	PPC_STORE_U8(ctx.r30.u32 + -22652, ctx.r11.u8);
	// addi r1,r1,192
	ctx.r1.s64 = ctx.r1.s64 + 192;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82597800);  /* forward decl */
PPC_WEAK_FUNC(sub_82597800) { __imp__sub_82597800(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82597800) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x82597808;
	__savegprlr_28(ctx, base);
	// stwu r1,-192(r1)
	ea = -192 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,12544
	ctx.r31.s64 = ctx.r11.s64 + 12544;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// bl 0x82273620
	ctx.lr = 0x82597828;
	sub_82273620(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// li r10,0
	ctx.r10.s64 = 0;
	// lwz r4,352(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
	// mr r8,r28
	ctx.r8.u64 = ctx.r28.u64;
	// mr r7,r29
	ctx.r7.u64 = ctx.r29.u64;
	// li r6,-1
	ctx.r6.s64 = -1;
	// lbz r11,-22652(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -22652);
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,1
	ctx.r9.s64 = 1;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x8221ffa0
	ctx.lr = 0x82597884;
	sub_8221FFA0(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-22652(r30)
	PPC_STORE_U8(ctx.r30.u32 + -22652, ctx.r11.u8);
	// addi r1,r1,192
	ctx.r1.s64 = ctx.r1.s64 + 192;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82597898);  /* forward decl */
PPC_WEAK_FUNC(sub_82597898) { __imp__sub_82597898(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82597898) {
	PPC_FUNC_PROLOGUE();
	// mr r10,r3
	ctx.r10.u64 = ctx.r3.u64;
	// mr r11,r4
	ctx.r11.u64 = ctx.r4.u64;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// lis r9,-32072
	ctx.r9.s64 = -2101870592;
	// beq cr6,0x825978cc
	if (ctx.cr6.eq) goto loc_825978CC;
	// addi r9,r9,-6276
	ctx.r9.s64 = ctx.r9.s64 + -6276;
	// subf r8,r10,r9
	ctx.r8.u64 = ctx.r9.u64 - ctx.r10.u64;
loc_825978B4:
	// lbz r9,0(r10)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// stbx r9,r8,r10
	PPC_STORE_U8(ctx.r8.u32 + ctx.r10.u32, ctx.r9.u8);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// bne cr6,0x825978b4
	if (!ctx.cr6.eq) goto loc_825978B4;
	// b 0x825978d4
	goto loc_825978D4;
loc_825978CC:
	// li r10,0
	ctx.r10.s64 = 0;
	// stb r10,-6276(r9)
	PPC_STORE_U8(ctx.r9.u32 + -6276, ctx.r10.u8);
loc_825978D4:
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// beq cr6,0x82597900
	if (ctx.cr6.eq) goto loc_82597900;
	// addi r10,r10,-6292
	ctx.r10.s64 = ctx.r10.s64 + -6292;
	// subf r9,r11,r10
	ctx.r9.u64 = ctx.r10.u64 - ctx.r11.u64;
loc_825978E8:
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r9,r11
	PPC_STORE_U8(ctx.r9.u32 + ctx.r11.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x825978e8
	if (!ctx.cr6.eq) goto loc_825978E8;
	// blr 
	return;
loc_82597900:
	// li r11,0
	ctx.r11.s64 = 0;
	// stb r11,-6292(r10)
	PPC_STORE_U8(ctx.r10.u32 + -6292, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82597910);  /* forward decl */
PPC_WEAK_FUNC(sub_82597910) { __imp__sub_82597910(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82597910) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x82597918;
	__savegprlr_27(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,12544
	ctx.r31.s64 = ctx.r11.s64 + 12544;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// bl 0x82273620
	ctx.lr = 0x8259793C;
	sub_82273620(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// lwz r4,352(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r29,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r29.u32);
	// mr r8,r27
	ctx.r8.u64 = ctx.r27.u64;
	// mr r7,r28
	ctx.r7.u64 = ctx.r28.u64;
	// li r6,-1
	ctx.r6.s64 = -1;
	// lbz r11,-22652(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -22652);
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,0
	ctx.r9.s64 = 0;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// bl 0x8221ffa0
	ctx.lr = 0x82597998;
	sub_8221FFA0(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-22652(r30)
	PPC_STORE_U8(ctx.r30.u32 + -22652, ctx.r11.u8);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825979A8);  /* forward decl */
PPC_WEAK_FUNC(sub_825979A8) { __imp__sub_825979A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825979A8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x825979B0;
	__savegprlr_27(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,12544
	ctx.r31.s64 = ctx.r11.s64 + 12544;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// bl 0x82273620
	ctx.lr = 0x825979D4;
	sub_82273620(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// lwz r4,352(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r29,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r29.u32);
	// mr r8,r27
	ctx.r8.u64 = ctx.r27.u64;
	// mr r7,r28
	ctx.r7.u64 = ctx.r28.u64;
	// li r6,-1
	ctx.r6.s64 = -1;
	// lbz r11,-22652(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -22652);
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,1
	ctx.r9.s64 = 1;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// bl 0x8221ffa0
	ctx.lr = 0x82597A30;
	sub_8221FFA0(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-22652(r30)
	PPC_STORE_U8(ctx.r30.u32 + -22652, ctx.r11.u8);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82597A40);  /* forward decl */
PPC_WEAK_FUNC(sub_82597A40) { __imp__sub_82597A40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82597A40) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb10
	ctx.lr = 0x82597A48;
	__savegprlr_26(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,12544
	ctx.r31.s64 = ctx.r11.s64 + 12544;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// mr r26,r7
	ctx.r26.u64 = ctx.r7.u64;
	// bl 0x82273620
	ctx.lr = 0x82597A70;
	sub_82273620(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// lwz r4,352(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r28,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r28.u32);
	// mr r8,r26
	ctx.r8.u64 = ctx.r26.u64;
	// stw r29,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r29.u32);
	// mr r7,r27
	ctx.r7.u64 = ctx.r27.u64;
	// li r6,-1
	ctx.r6.s64 = -1;
	// lbz r11,-22652(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -22652);
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,0
	ctx.r9.s64 = 0;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// bl 0x8221ffa0
	ctx.lr = 0x82597ACC;
	sub_8221FFA0(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-22652(r30)
	PPC_STORE_U8(ctx.r30.u32 + -22652, ctx.r11.u8);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82597AE0);  /* forward decl */
PPC_WEAK_FUNC(sub_82597AE0) { __imp__sub_82597AE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82597AE0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb10
	ctx.lr = 0x82597AE8;
	__savegprlr_26(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,12544
	ctx.r31.s64 = ctx.r11.s64 + 12544;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// mr r26,r7
	ctx.r26.u64 = ctx.r7.u64;
	// bl 0x82273620
	ctx.lr = 0x82597B10;
	sub_82273620(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// lwz r4,352(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r28,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r28.u32);
	// mr r8,r26
	ctx.r8.u64 = ctx.r26.u64;
	// stw r29,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r29.u32);
	// mr r7,r27
	ctx.r7.u64 = ctx.r27.u64;
	// li r6,-1
	ctx.r6.s64 = -1;
	// lbz r11,-22652(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -22652);
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,1
	ctx.r9.s64 = 1;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// bl 0x8221ffa0
	ctx.lr = 0x82597B6C;
	sub_8221FFA0(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-22652(r30)
	PPC_STORE_U8(ctx.r30.u32 + -22652, ctx.r11.u8);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82597B80);  /* forward decl */
PPC_WEAK_FUNC(sub_82597B80) { __imp__sub_82597B80(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82597B80) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x82597B88;
	__savegprlr_25(ctx, base);
	// stwu r1,-224(r1)
	ea = -224 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,12544
	ctx.r31.s64 = ctx.r11.s64 + 12544;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// mr r26,r7
	ctx.r26.u64 = ctx.r7.u64;
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// bl 0x82273620
	ctx.lr = 0x82597BB4;
	sub_82273620(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// lwz r4,352(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r27,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r27.u32);
	// mr r8,r25
	ctx.r8.u64 = ctx.r25.u64;
	// stw r28,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r28.u32);
	// mr r7,r26
	ctx.r7.u64 = ctx.r26.u64;
	// stw r29,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r29.u32);
	// li r6,-1
	ctx.r6.s64 = -1;
	// lbz r11,-22652(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -22652);
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,0
	ctx.r9.s64 = 0;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// bl 0x8221ffa0
	ctx.lr = 0x82597C10;
	sub_8221FFA0(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-22652(r30)
	PPC_STORE_U8(ctx.r30.u32 + -22652, ctx.r11.u8);
	// addi r1,r1,224
	ctx.r1.s64 = ctx.r1.s64 + 224;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82597C20);  /* forward decl */
PPC_WEAK_FUNC(sub_82597C20) { __imp__sub_82597C20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82597C20) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x82597C28;
	__savegprlr_25(ctx, base);
	// stwu r1,-224(r1)
	ea = -224 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,12544
	ctx.r31.s64 = ctx.r11.s64 + 12544;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// mr r26,r7
	ctx.r26.u64 = ctx.r7.u64;
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// bl 0x82273620
	ctx.lr = 0x82597C54;
	sub_82273620(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// lwz r4,352(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r27,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r27.u32);
	// mr r8,r25
	ctx.r8.u64 = ctx.r25.u64;
	// stw r28,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r28.u32);
	// mr r7,r26
	ctx.r7.u64 = ctx.r26.u64;
	// stw r29,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r29.u32);
	// li r6,-1
	ctx.r6.s64 = -1;
	// lbz r11,-22652(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -22652);
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,1
	ctx.r9.s64 = 1;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// bl 0x8221ffa0
	ctx.lr = 0x82597CB0;
	sub_8221FFA0(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-22652(r30)
	PPC_STORE_U8(ctx.r30.u32 + -22652, ctx.r11.u8);
	// addi r1,r1,224
	ctx.r1.s64 = ctx.r1.s64 + 224;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82597CC0);  /* forward decl */
PPC_WEAK_FUNC(sub_82597CC0) { __imp__sub_82597CC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82597CC0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb08
	ctx.lr = 0x82597CC8;
	__savegprlr_24(ctx, base);
	// stwu r1,-224(r1)
	ea = -224 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,12544
	ctx.r31.s64 = ctx.r11.s64 + 12544;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// mr r26,r7
	ctx.r26.u64 = ctx.r7.u64;
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// mr r24,r9
	ctx.r24.u64 = ctx.r9.u64;
	// bl 0x82273620
	ctx.lr = 0x82597CF8;
	sub_82273620(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// lwz r4,352(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r26,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r26.u32);
	// mr r8,r24
	ctx.r8.u64 = ctx.r24.u64;
	// stw r27,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r27.u32);
	// mr r7,r25
	ctx.r7.u64 = ctx.r25.u64;
	// stw r28,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r28.u32);
	// li r6,-1
	ctx.r6.s64 = -1;
	// stw r29,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r29.u32);
	// lbz r11,-22652(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -22652);
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,0
	ctx.r9.s64 = 0;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// bl 0x8221ffa0
	ctx.lr = 0x82597D54;
	sub_8221FFA0(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-22652(r30)
	PPC_STORE_U8(ctx.r30.u32 + -22652, ctx.r11.u8);
	// addi r1,r1,224
	ctx.r1.s64 = ctx.r1.s64 + 224;
	// b 0x8298eb58
	__restgprlr_24(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82597D68);  /* forward decl */
PPC_WEAK_FUNC(sub_82597D68) { __imp__sub_82597D68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82597D68) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb08
	ctx.lr = 0x82597D70;
	__savegprlr_24(ctx, base);
	// stwu r1,-224(r1)
	ea = -224 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,12544
	ctx.r31.s64 = ctx.r11.s64 + 12544;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// mr r26,r7
	ctx.r26.u64 = ctx.r7.u64;
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// mr r24,r9
	ctx.r24.u64 = ctx.r9.u64;
	// bl 0x82273620
	ctx.lr = 0x82597DA0;
	sub_82273620(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// lwz r4,352(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r26,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r26.u32);
	// mr r8,r24
	ctx.r8.u64 = ctx.r24.u64;
	// stw r27,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r27.u32);
	// mr r7,r25
	ctx.r7.u64 = ctx.r25.u64;
	// stw r28,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r28.u32);
	// li r6,-1
	ctx.r6.s64 = -1;
	// stw r29,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r29.u32);
	// lbz r11,-22652(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -22652);
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,1
	ctx.r9.s64 = 1;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// bl 0x8221ffa0
	ctx.lr = 0x82597DFC;
	sub_8221FFA0(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-22652(r30)
	PPC_STORE_U8(ctx.r30.u32 + -22652, ctx.r11.u8);
	// addi r1,r1,224
	ctx.r1.s64 = ctx.r1.s64 + 224;
	// b 0x8298eb58
	__restgprlr_24(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82597E10);  /* forward decl */
PPC_WEAK_FUNC(sub_82597E10) { __imp__sub_82597E10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82597E10) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb04
	ctx.lr = 0x82597E18;
	__savegprlr_23(ctx, base);
	// stwu r1,-240(r1)
	ea = -240 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,12544
	ctx.r31.s64 = ctx.r11.s64 + 12544;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// mr r26,r7
	ctx.r26.u64 = ctx.r7.u64;
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// mr r24,r9
	ctx.r24.u64 = ctx.r9.u64;
	// mr r23,r10
	ctx.r23.u64 = ctx.r10.u64;
	// bl 0x82273620
	ctx.lr = 0x82597E4C;
	sub_82273620(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// lwz r4,352(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r25,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r25.u32);
	// mr r7,r24
	ctx.r7.u64 = ctx.r24.u64;
	// stw r26,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r26.u32);
	// li r6,-1
	ctx.r6.s64 = -1;
	// stw r27,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r27.u32);
	// li r5,0
	ctx.r5.s64 = 0;
	// stw r28,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r28.u32);
	// lbz r11,-22652(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -22652);
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// stw r29,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r29.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r8,r9,27,31,31
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,0
	ctx.r9.s64 = 0;
	// xori r10,r8,1
	ctx.r10.u64 = ctx.r8.u64 ^ 1;
	// mr r8,r23
	ctx.r8.u64 = ctx.r23.u64;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// bl 0x8221ffa0
	ctx.lr = 0x82597EA8;
	sub_8221FFA0(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-22652(r30)
	PPC_STORE_U8(ctx.r30.u32 + -22652, ctx.r11.u8);
	// addi r1,r1,240
	ctx.r1.s64 = ctx.r1.s64 + 240;
	// b 0x8298eb54
	__restgprlr_23(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82597EB8);  /* forward decl */
PPC_WEAK_FUNC(sub_82597EB8) { __imp__sub_82597EB8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82597EB8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb04
	ctx.lr = 0x82597EC0;
	__savegprlr_23(ctx, base);
	// stwu r1,-240(r1)
	ea = -240 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,12544
	ctx.r31.s64 = ctx.r11.s64 + 12544;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// mr r26,r7
	ctx.r26.u64 = ctx.r7.u64;
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// mr r24,r9
	ctx.r24.u64 = ctx.r9.u64;
	// mr r23,r10
	ctx.r23.u64 = ctx.r10.u64;
	// bl 0x82273620
	ctx.lr = 0x82597EF4;
	sub_82273620(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// lwz r4,352(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r25,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r25.u32);
	// mr r7,r24
	ctx.r7.u64 = ctx.r24.u64;
	// stw r26,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r26.u32);
	// li r6,-1
	ctx.r6.s64 = -1;
	// stw r27,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r27.u32);
	// li r5,0
	ctx.r5.s64 = 0;
	// stw r28,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r28.u32);
	// lbz r11,-22652(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -22652);
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// stw r29,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r29.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r8,r9,27,31,31
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,1
	ctx.r9.s64 = 1;
	// xori r10,r8,1
	ctx.r10.u64 = ctx.r8.u64 ^ 1;
	// mr r8,r23
	ctx.r8.u64 = ctx.r23.u64;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// bl 0x8221ffa0
	ctx.lr = 0x82597F50;
	sub_8221FFA0(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-22652(r30)
	PPC_STORE_U8(ctx.r30.u32 + -22652, ctx.r11.u8);
	// addi r1,r1,240
	ctx.r1.s64 = ctx.r1.s64 + 240;
	// b 0x8298eb54
	__restgprlr_23(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82597F60);  /* forward decl */
PPC_WEAK_FUNC(sub_82597F60) { __imp__sub_82597F60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82597F60) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb04
	ctx.lr = 0x82597F68;
	__savegprlr_23(ctx, base);
	// stwu r1,-240(r1)
	ea = -240 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,12544
	ctx.r31.s64 = ctx.r11.s64 + 12544;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// mr r26,r7
	ctx.r26.u64 = ctx.r7.u64;
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// mr r24,r9
	ctx.r24.u64 = ctx.r9.u64;
	// mr r23,r10
	ctx.r23.u64 = ctx.r10.u64;
	// bl 0x82273620
	ctx.lr = 0x82597F9C;
	sub_82273620(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// lwz r4,352(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r24,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r24.u32);
	// mr r7,r23
	ctx.r7.u64 = ctx.r23.u64;
	// stw r25,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r25.u32);
	// li r6,-1
	ctx.r6.s64 = -1;
	// stw r26,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r26.u32);
	// li r5,0
	ctx.r5.s64 = 0;
	// stw r27,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r27.u32);
	// lbz r11,-22652(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -22652);
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// stw r28,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r28.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// stw r29,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r29.u32);
	// rlwinm r8,r9,27,31,31
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,0
	ctx.r9.s64 = 0;
	// xori r10,r8,1
	ctx.r10.u64 = ctx.r8.u64 ^ 1;
	// lwz r8,324(r1)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r1.u32 + 324);
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// bl 0x8221ffa0
	ctx.lr = 0x82597FF8;
	sub_8221FFA0(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-22652(r30)
	PPC_STORE_U8(ctx.r30.u32 + -22652, ctx.r11.u8);
	// addi r1,r1,240
	ctx.r1.s64 = ctx.r1.s64 + 240;
	// b 0x8298eb54
	__restgprlr_23(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82598008);  /* forward decl */
PPC_WEAK_FUNC(sub_82598008) { __imp__sub_82598008(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82598008) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb04
	ctx.lr = 0x82598010;
	__savegprlr_23(ctx, base);
	// stwu r1,-240(r1)
	ea = -240 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,12544
	ctx.r31.s64 = ctx.r11.s64 + 12544;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// mr r26,r7
	ctx.r26.u64 = ctx.r7.u64;
	// mr r25,r8
	ctx.r25.u64 = ctx.r8.u64;
	// mr r24,r9
	ctx.r24.u64 = ctx.r9.u64;
	// mr r23,r10
	ctx.r23.u64 = ctx.r10.u64;
	// bl 0x82273620
	ctx.lr = 0x82598044;
	sub_82273620(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// lwz r4,352(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
	// li r10,0
	ctx.r10.s64 = 0;
	// stw r24,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r24.u32);
	// mr r7,r23
	ctx.r7.u64 = ctx.r23.u64;
	// stw r25,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r25.u32);
	// li r6,-1
	ctx.r6.s64 = -1;
	// stw r26,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r26.u32);
	// li r5,0
	ctx.r5.s64 = 0;
	// stw r27,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r27.u32);
	// lbz r11,-22652(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -22652);
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// stw r28,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r28.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// stw r29,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r29.u32);
	// rlwinm r8,r9,27,31,31
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,1
	ctx.r9.s64 = 1;
	// xori r10,r8,1
	ctx.r10.u64 = ctx.r8.u64 ^ 1;
	// lwz r8,324(r1)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r1.u32 + 324);
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// bl 0x8221ffa0
	ctx.lr = 0x825980A0;
	sub_8221FFA0(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-22652(r30)
	PPC_STORE_U8(ctx.r30.u32 + -22652, ctx.r11.u8);
	// addi r1,r1,240
	ctx.r1.s64 = ctx.r1.s64 + 240;
	// b 0x8298eb54
	__restgprlr_23(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825980B0);  /* forward decl */
PPC_WEAK_FUNC(sub_825980B0) { __imp__sub_825980B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825980B0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb10
	ctx.lr = 0x825980B8;
	__savegprlr_26(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// addi r31,r11,12544
	ctx.r31.s64 = ctx.r11.s64 + 12544;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// bl 0x82273620
	ctx.lr = 0x825980DC;
	sub_82273620(ctx, base);
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// lwz r26,352(r31)
	ctx.r26.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82273620
	ctx.lr = 0x825980F0;
	sub_82273620(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// li r10,0
	ctx.r10.s64 = 0;
	// lwz r6,352(r31)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
	// mr r8,r28
	ctx.r8.u64 = ctx.r28.u64;
	// mr r7,r29
	ctx.r7.u64 = ctx.r29.u64;
	// lbz r11,-22652(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -22652);
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,0
	ctx.r9.s64 = 0;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x8221ffa0
	ctx.lr = 0x82598150;
	sub_8221FFA0(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-22652(r30)
	PPC_STORE_U8(ctx.r30.u32 + -22652, ctx.r11.u8);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82598160);  /* forward decl */
PPC_WEAK_FUNC(sub_82598160) { __imp__sub_82598160(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82598160) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb10
	ctx.lr = 0x82598168;
	__savegprlr_26(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// addi r31,r11,12544
	ctx.r31.s64 = ctx.r11.s64 + 12544;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// bl 0x82273620
	ctx.lr = 0x8259818C;
	sub_82273620(ctx, base);
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// lwz r26,352(r31)
	ctx.r26.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82273620
	ctx.lr = 0x825981A0;
	sub_82273620(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// li r10,0
	ctx.r10.s64 = 0;
	// lwz r6,352(r31)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
	// mr r8,r28
	ctx.r8.u64 = ctx.r28.u64;
	// mr r7,r29
	ctx.r7.u64 = ctx.r29.u64;
	// lbz r11,-22652(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -22652);
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,1
	ctx.r9.s64 = 1;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x8221ffa0
	ctx.lr = 0x82598200;
	sub_8221FFA0(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-22652(r30)
	PPC_STORE_U8(ctx.r30.u32 + -22652, ctx.r11.u8);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82598210);  /* forward decl */
PPC_WEAK_FUNC(sub_82598210) { __imp__sub_82598210(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82598210) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb10
	ctx.lr = 0x82598218;
	__savegprlr_26(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// addi r31,r11,12544
	ctx.r31.s64 = ctx.r11.s64 + 12544;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// bl 0x82273620
	ctx.lr = 0x8259823C;
	sub_82273620(ctx, base);
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// addi r11,r11,-15968
	ctx.r11.s64 = ctx.r11.s64 + -15968;
	// li r5,1
	ctx.r5.s64 = 1;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x821d1bb8
	ctx.lr = 0x82598258;
	sub_821D1BB8(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// li r10,0
	ctx.r10.s64 = 0;
	// lwz r4,352(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// mr r8,r28
	ctx.r8.u64 = ctx.r28.u64;
	// mr r7,r29
	ctx.r7.u64 = ctx.r29.u64;
	// lbz r11,-22652(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -22652);
	// li r6,-1
	ctx.r6.s64 = -1;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// li r5,0
	ctx.r5.s64 = 0;
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// stw r26,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r26.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,0
	ctx.r9.s64 = 0;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x8221ffa0
	ctx.lr = 0x825982BC;
	sub_8221FFA0(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-22652(r30)
	PPC_STORE_U8(ctx.r30.u32 + -22652, ctx.r11.u8);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825982D0);  /* forward decl */
PPC_WEAK_FUNC(sub_825982D0) { __imp__sub_825982D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825982D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb10
	ctx.lr = 0x825982D8;
	__savegprlr_26(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// addi r31,r11,12544
	ctx.r31.s64 = ctx.r11.s64 + 12544;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// bl 0x82273620
	ctx.lr = 0x825982FC;
	sub_82273620(ctx, base);
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// addi r11,r11,-15968
	ctx.r11.s64 = ctx.r11.s64 + -15968;
	// li r5,1
	ctx.r5.s64 = 1;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// bl 0x821d1bb8
	ctx.lr = 0x82598318;
	sub_821D1BB8(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// li r10,0
	ctx.r10.s64 = 0;
	// lwz r4,352(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// mr r8,r28
	ctx.r8.u64 = ctx.r28.u64;
	// mr r7,r29
	ctx.r7.u64 = ctx.r29.u64;
	// lbz r11,-22652(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -22652);
	// li r6,-1
	ctx.r6.s64 = -1;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// li r5,0
	ctx.r5.s64 = 0;
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// stw r26,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r26.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,1
	ctx.r9.s64 = 1;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// stw r11,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r11.u32);
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x8221ffa0
	ctx.lr = 0x8259837C;
	sub_8221FFA0(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-22652(r30)
	PPC_STORE_U8(ctx.r30.u32 + -22652, ctx.r11.u8);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82598390);  /* forward decl */
PPC_WEAK_FUNC(sub_82598390) { __imp__sub_82598390(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82598390) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x82598398;
	__savegprlr_25(ctx, base);
	// stwu r1,-224(r1)
	ea = -224 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,12544
	ctx.r31.s64 = ctx.r11.s64 + 12544;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// mr r26,r7
	ctx.r26.u64 = ctx.r7.u64;
	// bl 0x82273620
	ctx.lr = 0x825983C0;
	sub_82273620(ctx, base);
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// mr r25,r3
	ctx.r25.u64 = ctx.r3.u64;
	// addi r30,r11,-15968
	ctx.r30.s64 = ctx.r11.s64 + -15968;
	// li r5,1
	ctx.r5.s64 = 1;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x821d1bb8
	ctx.lr = 0x825983DC;
	sub_821D1BB8(ctx, base);
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// li r5,1
	ctx.r5.s64 = 1;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x821d1bb8
	ctx.lr = 0x825983F0;
	sub_821D1BB8(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// li r10,0
	ctx.r10.s64 = 0;
	// lwz r4,352(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
	// mr r28,r3
	ctx.r28.u64 = ctx.r3.u64;
	// stw r29,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r29.u32);
	// mr r8,r26
	ctx.r8.u64 = ctx.r26.u64;
	// mr r7,r27
	ctx.r7.u64 = ctx.r27.u64;
	// lbz r11,-22652(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -22652);
	// li r6,-1
	ctx.r6.s64 = -1;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// li r5,0
	ctx.r5.s64 = 0;
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// stw r28,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r28.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,0
	ctx.r9.s64 = 0;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x8221ffa0
	ctx.lr = 0x82598454;
	sub_8221FFA0(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-22652(r30)
	PPC_STORE_U8(ctx.r30.u32 + -22652, ctx.r11.u8);
	// addi r1,r1,224
	ctx.r1.s64 = ctx.r1.s64 + 224;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82598468);  /* forward decl */
PPC_WEAK_FUNC(sub_82598468) { __imp__sub_82598468(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82598468) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x82598470;
	__savegprlr_25(ctx, base);
	// stwu r1,-224(r1)
	ea = -224 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r31,r11,12544
	ctx.r31.s64 = ctx.r11.s64 + 12544;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// mr r26,r7
	ctx.r26.u64 = ctx.r7.u64;
	// bl 0x82273620
	ctx.lr = 0x82598498;
	sub_82273620(ctx, base);
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// mr r25,r3
	ctx.r25.u64 = ctx.r3.u64;
	// addi r30,r11,-15968
	ctx.r30.s64 = ctx.r11.s64 + -15968;
	// li r5,1
	ctx.r5.s64 = 1;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x821d1bb8
	ctx.lr = 0x825984B4;
	sub_821D1BB8(ctx, base);
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// li r5,1
	ctx.r5.s64 = 1;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x821d1bb8
	ctx.lr = 0x825984C8;
	sub_821D1BB8(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// li r10,0
	ctx.r10.s64 = 0;
	// lwz r4,352(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
	// mr r28,r3
	ctx.r28.u64 = ctx.r3.u64;
	// stw r29,132(r1)
	PPC_STORE_U32(ctx.r1.u32 + 132, ctx.r29.u32);
	// mr r8,r26
	ctx.r8.u64 = ctx.r26.u64;
	// mr r7,r27
	ctx.r7.u64 = ctx.r27.u64;
	// lbz r11,-22652(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -22652);
	// li r6,-1
	ctx.r6.s64 = -1;
	// stb r10,151(r1)
	PPC_STORE_U8(ctx.r1.u32 + 151, ctx.r10.u8);
	// li r5,0
	ctx.r5.s64 = 0;
	// cntlzw r9,r11
	ctx.r9.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// stw r28,140(r1)
	PPC_STORE_U32(ctx.r1.u32 + 140, ctx.r28.u32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// rlwinm r10,r9,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// li r9,1
	ctx.r9.s64 = 1;
	// xori r10,r10,1
	ctx.r10.u64 = ctx.r10.u64 ^ 1;
	// mr r3,r25
	ctx.r3.u64 = ctx.r25.u64;
	// stw r11,124(r1)
	PPC_STORE_U32(ctx.r1.u32 + 124, ctx.r11.u32);
	// stw r11,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r11.u32);
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x8221ffa0
	ctx.lr = 0x8259852C;
	sub_8221FFA0(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-22652(r30)
	PPC_STORE_U8(ctx.r30.u32 + -22652, ctx.r11.u8);
	// addi r1,r1,224
	ctx.r1.s64 = ctx.r1.s64 + 224;
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82598540);  /* forward decl */
PPC_WEAK_FUNC(sub_82598540) { __imp__sub_82598540(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82598540) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eaf8
	ctx.lr = 0x82598548;
	__savegprlr_20(ctx, base);
	// stwu r1,-1808(r1)
	ea = -1808 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r29,r11,-12072
	ctx.r29.s64 = ctx.r11.s64 + -12072;
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// addi r28,r11,-11808
	ctx.r28.s64 = ctx.r11.s64 + -11808;
	// mr r23,r5
	ctx.r23.u64 = ctx.r5.u64;
	// mr r22,r6
	ctx.r22.u64 = ctx.r6.u64;
	// mr r21,r7
	ctx.r21.u64 = ctx.r7.u64;
	// mr r27,r8
	ctx.r27.u64 = ctx.r8.u64;
	// lwz r11,196(r28)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r28.u32 + 196);
	// mr r26,r9
	ctx.r26.u64 = ctx.r9.u64;
	// mr r25,r10
	ctx.r25.u64 = ctx.r10.u64;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// li r20,0
	ctx.r20.s64 = 0;
	// lwzx r3,r11,r29
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + ctx.r29.u32);
	// bl 0x82197b80
	ctx.lr = 0x82598590;
	sub_82197B80(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82598700
	if (ctx.cr6.eq) goto loc_82598700;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// addi r24,r11,12544
	ctx.r24.s64 = ctx.r11.s64 + 12544;
	// mr r3,r24
	ctx.r3.u64 = ctx.r24.u64;
	// bl 0x82273620
	ctx.lr = 0x825985B0;
	sub_82273620(ctx, base);
	// addi r11,r30,-4
	ctx.r11.s64 = ctx.r30.s64 + -4;
	// cmplwi cr6,r11,5
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 5, ctx.xer);
	// bgt cr6,0x825985f4
	if (ctx.cr6.gt) goto loc_825985F4;
	// li r4,-1
	ctx.r4.s64 = -1;
	// lwz r9,1908(r1)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 1908);
	// addi r10,r1,96
	ctx.r10.s64 = ctx.r1.s64 + 96;
	// lwz r8,1900(r1)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r1.u32 + 1900);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lwz r7,1892(r1)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r1.u32 + 1892);
	// mr r6,r25
	ctx.r6.u64 = ctx.r25.u64;
	// mr r5,r26
	ctx.r5.u64 = ctx.r26.u64;
	// stw r4,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r4.u32);
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// stw r10,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r10.u32);
	// lfs f1,3068(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3068);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x8221dea0
	ctx.lr = 0x825985F0;
	sub_8221DEA0(ctx, base);
	// b 0x82598604
	goto loc_82598604;
loc_825985F4:
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// li r5,400
	ctx.r5.s64 = 400;
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// bl 0x8221dca0
	ctx.lr = 0x82598604;
	sub_8221DCA0(ctx, base);
loc_82598604:
	// cmpwi cr6,r30,1
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 1, ctx.xer);
	// bne cr6,0x8259863c
	if (!ctx.cr6.eq) goto loc_8259863C;
	// mr r4,r23
	ctx.r4.u64 = ctx.r23.u64;
	// mr r3,r24
	ctx.r3.u64 = ctx.r24.u64;
	// bl 0x82273620
	ctx.lr = 0x82598618;
	sub_82273620(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// li r9,-1
	ctx.r9.s64 = -1;
	// li r8,-1
	ctx.r8.s64 = -1;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// bl 0x8221eba0
	ctx.lr = 0x82598638;
	sub_8221EBA0(ctx, base);
	// b 0x8259869c
	goto loc_8259869C;
loc_8259863C:
	// cmpwi cr6,r30,2
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 2, ctx.xer);
	// beq cr6,0x8259864c
	if (ctx.cr6.eq) goto loc_8259864C;
	// cmpwi cr6,r30,3
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 3, ctx.xer);
	// bne cr6,0x8259869c
	if (!ctx.cr6.eq) goto loc_8259869C;
loc_8259864C:
	// cmplwi cr6,r22,0
	ctx.cr6.compare<uint32_t>(ctx.r22.u32, 0, ctx.xer);
	// beq cr6,0x82598668
	if (ctx.cr6.eq) goto loc_82598668;
	// li r5,400
	ctx.r5.s64 = 400;
	// mr r4,r22
	ctx.r4.u64 = ctx.r22.u64;
	// addi r3,r1,896
	ctx.r3.s64 = ctx.r1.s64 + 896;
	// bl 0x8221dd28
	ctx.lr = 0x82598664;
	sub_8221DD28(ctx, base);
	// b 0x8259866c
	goto loc_8259866C;
loc_82598668:
	// stb r20,896(r1)
	PPC_STORE_U8(ctx.r1.u32 + 896, ctx.r20.u8);
loc_8259866C:
	// cmplwi cr6,r21,0
	ctx.cr6.compare<uint32_t>(ctx.r21.u32, 0, ctx.xer);
	// beq cr6,0x82598688
	if (ctx.cr6.eq) goto loc_82598688;
	// li r5,400
	ctx.r5.s64 = 400;
	// mr r4,r21
	ctx.r4.u64 = ctx.r21.u64;
	// addi r3,r1,1296
	ctx.r3.s64 = ctx.r1.s64 + 1296;
	// bl 0x8221dd28
	ctx.lr = 0x82598684;
	sub_8221DD28(ctx, base);
	// b 0x8259868c
	goto loc_8259868C;
loc_82598688:
	// stb r20,1296(r1)
	PPC_STORE_U8(ctx.r1.u32 + 1296, ctx.r20.u8);
loc_8259868C:
	// addi r5,r1,1296
	ctx.r5.s64 = ctx.r1.s64 + 1296;
	// addi r4,r1,896
	ctx.r4.s64 = ctx.r1.s64 + 896;
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// bl 0x8221e0a8
	ctx.lr = 0x8259869C;
	sub_8221E0A8(ctx, base);
loc_8259869C:
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// bl 0x828e0ab8
	ctx.lr = 0x825986A4;
	sub_828E0AB8(ctx, base);
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// bl 0x8221dc58
	ctx.lr = 0x825986AC;
	sub_8221DC58(ctx, base);
	// lwz r11,196(r28)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r28.u32 + 196);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r11,r11,r29
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + ctx.r29.u32);
	// addi r3,r11,108
	ctx.r3.s64 = ctx.r11.s64 + 108;
	// bl 0x8221dc58
	ctx.lr = 0x825986C4;
	sub_8221DC58(ctx, base);
	// clrlwi r11,r31,16
	ctx.r11.u64 = ctx.r31.u32 & 0xFFFF;
	// clrlwi r10,r3,16
	ctx.r10.u64 = ctx.r3.u32 & 0xFFFF;
	// cmplw cr6,r11,r10
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, ctx.r10.u32, ctx.xer);
	// bne cr6,0x82598700
	if (!ctx.cr6.eq) goto loc_82598700;
	// lwz r11,196(r28)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r28.u32 + 196);
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r11,r11,r29
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + ctx.r29.u32);
	// addi r4,r11,108
	ctx.r4.s64 = ctx.r11.s64 + 108;
	// bl 0x8221ddb0
	ctx.lr = 0x825986F0;
	sub_8221DDB0(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// li r3,1
	ctx.r3.s64 = 1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82598704
	if (!ctx.cr6.eq) goto loc_82598704;
loc_82598700:
	// mr r3,r20
	ctx.r3.u64 = ctx.r20.u64;
loc_82598704:
	// addi r1,r1,1808
	ctx.r1.s64 = ctx.r1.s64 + 1808;
	// b 0x8298eb48
	__restgprlr_20(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82598710);  /* forward decl */
PPC_WEAK_FUNC(sub_82598710) { __imp__sub_82598710(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82598710) {
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
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r30,r11,12544
	ctx.r30.s64 = ctx.r11.s64 + 12544;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82273168
	ctx.lr = 0x8259873C;
	sub_82273168(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82598760
	if (ctx.cr6.eq) goto loc_82598760;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82273620
	ctx.lr = 0x82598754;
	sub_82273620(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// bl 0x82272858
	ctx.lr = 0x8259875C;
	sub_82272858(ctx, base);
	// b 0x82598768
	goto loc_82598768;
loc_82598760:
	// lis r11,-32092
	ctx.r11.s64 = -2103181312;
	// lwz r3,-22384(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + -22384);
loc_82598768:
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

PPC_FUNC_IMPL(__imp__sub_82598780);  /* forward decl */
PPC_WEAK_FUNC(sub_82598780) { __imp__sub_82598780(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82598780) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x82598788;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// cmpwi cr6,r29,0
	ctx.cr6.compare<int32_t>(ctx.r29.s32, 0, ctx.xer);
	// blt cr6,0x82598830
	if (ctx.cr6.lt) goto loc_82598830;
	// cmpwi cr6,r29,1024
	ctx.cr6.compare<int32_t>(ctx.r29.s32, 1024, ctx.xer);
	// bge cr6,0x82598830
	if (!ctx.cr6.lt) goto loc_82598830;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// addi r31,r11,12544
	ctx.r31.s64 = ctx.r11.s64 + 12544;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82273168
	ctx.lr = 0x825987B8;
	sub_82273168(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82598830
	if (ctx.cr6.eq) goto loc_82598830;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82273620
	ctx.lr = 0x825987D0;
	sub_82273620(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// bl 0x82272858
	ctx.lr = 0x825987D8;
	sub_82272858(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r11,r4
	ctx.r11.u64 = ctx.r4.u64;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_825987E4:
	// lbz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// bne cr6,0x825987e4
	if (!ctx.cr6.eq) goto loc_825987E4;
	// subf r11,r10,r11
	ctx.r11.u64 = ctx.r11.u64 - ctx.r10.u64;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// rotlwi r11,r11,0
	ctx.r11.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// cmpw cr6,r29,r11
	ctx.cr6.compare<int32_t>(ctx.r29.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x82598830
	if (ctx.cr6.gt) goto loc_82598830;
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// mr r5,r29
	ctx.r5.u64 = ctx.r29.u64;
	// addi r31,r11,-12360
	ctx.r31.s64 = ctx.r11.s64 + -12360;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8298fef0
	ctx.lr = 0x8259881C;
	sub_8298FEF0(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stbx r11,r31,r29
	PPC_STORE_U8(ctx.r31.u32 + ctx.r29.u32, ctx.r11.u8);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
loc_82598830:
	// lis r11,-32092
	ctx.r11.s64 = -2103181312;
	// lwz r3,-22384(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + -22384);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82598840);  /* forward decl */
PPC_WEAK_FUNC(sub_82598840) { __imp__sub_82598840(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82598840) {
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
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bge cr6,0x8259886c
	if (!ctx.cr6.lt) goto loc_8259886C;
loc_82598860:
	// lis r11,-32092
	ctx.r11.s64 = -2103181312;
	// lwz r3,-22384(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + -22384);
	// b 0x825988c4
	goto loc_825988C4;
loc_8259886C:
	// cmpwi cr6,r30,1024
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 1024, ctx.xer);
	// bge cr6,0x82598860
	if (!ctx.cr6.lt) goto loc_82598860;
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_8259887C:
	// lbz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// bne cr6,0x8259887c
	if (!ctx.cr6.eq) goto loc_8259887C;
	// subf r11,r10,r11
	ctx.r11.u64 = ctx.r11.u64 - ctx.r10.u64;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// rotlwi r11,r11,0
	ctx.r11.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// cmpw cr6,r30,r11
	ctx.cr6.compare<int32_t>(ctx.r30.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x82598860
	if (ctx.cr6.gt) goto loc_82598860;
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r31,r11,-11336
	ctx.r31.s64 = ctx.r11.s64 + -11336;
	// mr r5,r30
	ctx.r5.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x8298fef0
	ctx.lr = 0x825988B8;
	sub_8298FEF0(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// stbx r11,r31,r30
	PPC_STORE_U8(ctx.r31.u32 + ctx.r30.u32, ctx.r11.u8);
loc_825988C4:
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

PPC_FUNC_IMPL(__imp__sub_825988E0);  /* forward decl */
PPC_WEAK_FUNC(sub_825988E0) { __imp__sub_825988E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825988E0) {
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
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r30,r11,12544
	ctx.r30.s64 = ctx.r11.s64 + 12544;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82273168
	ctx.lr = 0x8259890C;
	sub_82273168(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82598950
	if (ctx.cr6.eq) goto loc_82598950;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82273620
	ctx.lr = 0x82598924;
	sub_82273620(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// bl 0x82272858
	ctx.lr = 0x8259892C;
	sub_82272858(ctx, base);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
loc_82598930:
	// lbz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r3.u32 + 0);
	// addi r3,r3,1
	ctx.r3.s64 = ctx.r3.s64 + 1;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// bne cr6,0x82598930
	if (!ctx.cr6.eq) goto loc_82598930;
	// subf r11,r11,r3
	ctx.r11.u64 = ctx.r3.u64 - ctx.r11.u64;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// rotlwi r3,r11,0
	ctx.r3.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// b 0x82598954
	goto loc_82598954;
loc_82598950:
	// li r3,0
	ctx.r3.s64 = 0;
loc_82598954:
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

PPC_FUNC_IMPL(__imp__sub_82598970);  /* forward decl */
PPC_WEAK_FUNC(sub_82598970) { __imp__sub_82598970(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82598970) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x82598978;
	__savegprlr_28(ctx, base);
	// stwu r1,-1152(r1)
	ea = -1152 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// mr r31,r5
	ctx.r31.u64 = ctx.r5.u64;
	// cmpwi cr6,r29,0
	ctx.cr6.compare<int32_t>(ctx.r29.s32, 0, ctx.xer);
	// bge cr6,0x8259899c
	if (!ctx.cr6.lt) goto loc_8259899C;
loc_8259898C:
	// lis r11,-32092
	ctx.r11.s64 = -2103181312;
	// lwz r3,-22384(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + -22384);
	// addi r1,r1,1152
	ctx.r1.s64 = ctx.r1.s64 + 1152;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
loc_8259899C:
	// cmpwi cr6,r31,1024
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 1024, ctx.xer);
	// bge cr6,0x8259898c
	if (!ctx.cr6.lt) goto loc_8259898C;
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_825989AC:
	// lbz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// bne cr6,0x825989ac
	if (!ctx.cr6.eq) goto loc_825989AC;
	// subf r11,r10,r11
	ctx.r11.u64 = ctx.r11.u64 - ctx.r10.u64;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// rotlwi r11,r11,0
	ctx.r11.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// cmpw cr6,r31,r11
	ctx.cr6.compare<int32_t>(ctx.r31.s32, ctx.r11.s32, ctx.xer);
	// bgt cr6,0x8259898c
	if (ctx.cr6.gt) goto loc_8259898C;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8298fef0
	ctx.lr = 0x825989E0;
	sub_8298FEF0(ctx, base);
	// addi r10,r1,80
	ctx.r10.s64 = ctx.r1.s64 + 80;
	// li r28,0
	ctx.r28.s64 = 0;
	// cmpw cr6,r29,r31
	ctx.cr6.compare<int32_t>(ctx.r29.s32, ctx.r31.s32, ctx.xer);
	// mr r11,r28
	ctx.r11.u64 = ctx.r28.u64;
	// stbx r28,r31,r10
	PPC_STORE_U8(ctx.r31.u32 + ctx.r10.u32, ctx.r28.u8);
	// lis r10,-32005
	ctx.r10.s64 = -2097479680;
	// addi r30,r10,-10312
	ctx.r30.s64 = ctx.r10.s64 + -10312;
	// bgt cr6,0x82598a20
	if (ctx.cr6.gt) goto loc_82598A20;
	// subf r11,r29,r31
	ctx.r11.u64 = ctx.r31.u64 - ctx.r29.u64;
	// addi r10,r1,80
	ctx.r10.s64 = ctx.r1.s64 + 80;
	// addi r31,r11,1
	ctx.r31.s64 = ctx.r11.s64 + 1;
	// add r4,r29,r10
	ctx.r4.u64 = ctx.r29.u64 + ctx.r10.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// bl 0x82990cb8
	ctx.lr = 0x82598A1C;
	sub_82990CB8(ctx, base);
	// mr r11,r31
	ctx.r11.u64 = ctx.r31.u64;
loc_82598A20:
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// stbx r28,r11,r30
	PPC_STORE_U8(ctx.r11.u32 + ctx.r30.u32, ctx.r28.u8);
	// addi r1,r1,1152
	ctx.r1.s64 = ctx.r1.s64 + 1152;
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82598A30);  /* forward decl */
PPC_WEAK_FUNC(sub_82598A30) { __imp__sub_82598A30(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82598A30) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x82598A38;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// addi r30,r11,12544
	ctx.r30.s64 = ctx.r11.s64 + 12544;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82273168
	ctx.lr = 0x82598A58;
	sub_82273168(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82598ac8
	if (ctx.cr6.eq) goto loc_82598AC8;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82273620
	ctx.lr = 0x82598A70;
	sub_82273620(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// bl 0x82272858
	ctx.lr = 0x82598A78;
	sub_82272858(ctx, base);
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_82598A80:
	// lbz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// bne cr6,0x82598a80
	if (!ctx.cr6.eq) goto loc_82598A80;
	// subf r11,r10,r11
	ctx.r11.u64 = ctx.r11.u64 - ctx.r10.u64;
	// cmpwi cr6,r29,0
	ctx.cr6.compare<int32_t>(ctx.r29.s32, 0, ctx.xer);
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// rotlwi r11,r11,0
	ctx.r11.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// blt cr6,0x82598ac8
	if (ctx.cr6.lt) goto loc_82598AC8;
	// cmpw cr6,r29,r11
	ctx.cr6.compare<int32_t>(ctx.r29.s32, ctx.r11.s32, ctx.xer);
	// bge cr6,0x82598ac8
	if (!ctx.cr6.lt) goto loc_82598AC8;
	// lbzx r11,r3,r29
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + ctx.r29.u32);
	// extsb r11,r11
	ctx.r11.s64 = ctx.r11.s8;
	// cmpwi cr6,r11,48
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 48, ctx.xer);
	// blt cr6,0x82598ac8
	if (ctx.cr6.lt) goto loc_82598AC8;
	// cmpwi cr6,r11,57
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 57, ctx.xer);
	// addi r3,r11,-48
	ctx.r3.s64 = ctx.r11.s64 + -48;
	// ble cr6,0x82598acc
	if (!ctx.cr6.gt) goto loc_82598ACC;
loc_82598AC8:
	// li r3,-1
	ctx.r3.s64 = -1;
loc_82598ACC:
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82598AD8);  /* forward decl */
PPC_WEAK_FUNC(sub_82598AD8) { __imp__sub_82598AD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82598AD8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x82598AE0;
	__savegprlr_29(ctx, base);
	// stfd f30,-48(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -48, ctx.f30.u64);
	// stfd f31,-40(r1)
	PPC_STORE_U64(ctx.r1.u32 + -40, ctx.f31.u64);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// addi r31,r10,3712
	ctx.r31.s64 = ctx.r10.s64 + 3712;
	// lwz r11,-6256(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// addi r10,r31,25
	ctx.r10.s64 = ctx.r31.s64 + 25;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lbzx r10,r11,r10
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + ctx.r10.u32);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x82598b24
	if (ctx.cr6.eq) goto loc_82598B24;
	// li r3,2
	ctx.r3.s64 = 2;
	// b 0x82598b54
	goto loc_82598B54;
loc_82598B24:
	// addi r10,r31,26
	ctx.r10.s64 = ctx.r31.s64 + 26;
	// lbzx r10,r11,r10
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + ctx.r10.u32);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x82598b3c
	if (ctx.cr6.eq) goto loc_82598B3C;
	// li r3,0
	ctx.r3.s64 = 0;
	// b 0x82598b54
	goto loc_82598B54;
loc_82598B3C:
	// addi r10,r31,24
	ctx.r10.s64 = ctx.r31.s64 + 24;
	// li r3,3
	ctx.r3.s64 = 3;
	// lbzx r11,r11,r10
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + ctx.r10.u32);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82598b54
	if (!ctx.cr6.eq) goto loc_82598B54;
	// li r3,1
	ctx.r3.s64 = 1;
loc_82598B54:
	// bl 0x821dd1e8
	ctx.lr = 0x82598B58;
	sub_821DD1E8(ctx, base);
	// lwz r11,-6256(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// addi r10,r31,36
	ctx.r10.s64 = ctx.r31.s64 + 36;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lfsx f2,r11,r10
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + ctx.r10.u32);
	ctx.f2.f64 = double(temp.f32);
	// addi r9,r31,32
	ctx.r9.s64 = ctx.r31.s64 + 32;
	// lfsx f1,r11,r9
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + ctx.r9.u32);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x821dce40
	ctx.lr = 0x82598B74;
	sub_821DCE40(ctx, base);
	// lwz r11,-6256(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// addi r10,r31,29
	ctx.r10.s64 = ctx.r31.s64 + 29;
	// li r4,0
	ctx.r4.s64 = 0;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lbzx r3,r11,r10
	ctx.r3.u64 = PPC_LOAD_U8(ctx.r11.u32 + ctx.r10.u32);
	// bl 0x821dd120
	ctx.lr = 0x82598B8C;
	sub_821DD120(ctx, base);
	// lwz r11,-6256(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// addi r10,r31,27
	ctx.r10.s64 = ctx.r31.s64 + 27;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lbzx r3,r11,r10
	ctx.r3.u64 = PPC_LOAD_U8(ctx.r11.u32 + ctx.r10.u32);
	// bl 0x821dd0e0
	ctx.lr = 0x82598BA0;
	sub_821DD0E0(ctx, base);
	// lwz r11,-6256(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// addi r10,r31,57
	ctx.r10.s64 = ctx.r31.s64 + 57;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lbzx r10,r11,r10
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + ctx.r10.u32);
	// extsb r10,r10
	ctx.r10.s64 = ctx.r10.s8;
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// beq cr6,0x82598bd8
	if (ctx.cr6.eq) goto loc_82598BD8;
	// extsw r11,r10
	ctx.r11.s64 = ctx.r10.s32;
	// std r11,80(r1)
	PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r11.u64);
	// lfd f0,80(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f0.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
	// fcfid f0,f0
	ctx.f0.f64 = double(ctx.f0.s64);
	// frsp f1,f0
	ctx.f1.f64 = double(float(ctx.f0.f64));
	// bl 0x821dcfc8
	ctx.lr = 0x82598BD4;
	sub_821DCFC8(ctx, base);
	// b 0x82598bf8
	goto loc_82598BF8;
loc_82598BD8:
	// addi r10,r31,56
	ctx.r10.s64 = ctx.r31.s64 + 56;
	// lbzx r11,r11,r10
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + ctx.r10.u32);
	// extsb r11,r11
	ctx.r11.s64 = ctx.r11.s8;
	// std r11,80(r1)
	PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r11.u64);
	// lfd f0,80(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f0.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
	// fcfid f0,f0
	ctx.f0.f64 = double(ctx.f0.s64);
	// frsp f1,f0
	ctx.f1.f64 = double(float(ctx.f0.f64));
	// bl 0x821dcf68
	ctx.lr = 0x82598BF8;
	sub_821DCF68(ctx, base);
loc_82598BF8:
	// lwz r11,-6256(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// addi r10,r31,60
	ctx.r10.s64 = ctx.r31.s64 + 60;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lwzx r11,r11,r10
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + ctx.r10.u32);
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// bl 0x821dcd38
	ctx.lr = 0x82598C10;
	sub_821DCD38(ctx, base);
	// lwz r11,-6256(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// addi r10,r31,8
	ctx.r10.s64 = ctx.r31.s64 + 8;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lfsx f2,r11,r10
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + ctx.r10.u32);
	ctx.f2.f64 = double(temp.f32);
	// addi r9,r31,4
	ctx.r9.s64 = ctx.r31.s64 + 4;
	// lfsx f1,r11,r9
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + ctx.r9.u32);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x821dcba0
	ctx.lr = 0x82598C2C;
	sub_821DCBA0(ctx, base);
	// mr r5,r29
	ctx.r5.u64 = ctx.r29.u64;
	// fmr f2,f30
	ctx.fpscr.disableFlushMode();
	ctx.f2.f64 = ctx.f30.f64;
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// bl 0x821df770
	ctx.lr = 0x82598C3C;
	sub_821DF770(ctx, base);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lfd f30,-48(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -48);
	// lfd f31,-40(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -40);
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82598C50);  /* forward decl */
PPC_WEAK_FUNC(sub_82598C50) { __imp__sub_82598C50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82598C50) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x82598C58;
	__savegprlr_27(ctx, base);
	// stfd f30,-64(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -64, ctx.f30.u64);
	// stfd f31,-56(r1)
	PPC_STORE_U64(ctx.r1.u32 + -56, ctx.f31.u64);
	// stwu r1,-544(r1)
	ea = -544 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// mr r4,r5
	ctx.r4.u64 = ctx.r5.u64;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// addi r30,r11,12544
	ctx.r30.s64 = ctx.r11.s64 + 12544;
	// mr r31,r6
	ctx.r31.u64 = ctx.r6.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// mr r29,r7
	ctx.r29.u64 = ctx.r7.u64;
	// bl 0x82273620
	ctx.lr = 0x82598C88;
	sub_82273620(ctx, base);
	// mr r11,r31
	ctx.r11.u64 = ctx.r31.u64;
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// li r28,0
	ctx.r28.s64 = 0;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_82598C98:
	// lbz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// bne cr6,0x82598c98
	if (!ctx.cr6.eq) goto loc_82598C98;
	// subf r11,r10,r11
	ctx.r11.u64 = ctx.r11.u64 - ctx.r10.u64;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// rotlwi r11,r11,0
	ctx.r11.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82598ccc
	if (ctx.cr6.eq) goto loc_82598CCC;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82273620
	ctx.lr = 0x82598CC8;
	sub_82273620(ctx, base);
	// mr r28,r3
	ctx.r28.u64 = ctx.r3.u64;
loc_82598CCC:
	// mr r11,r29
	ctx.r11.u64 = ctx.r29.u64;
	// li r31,0
	ctx.r31.s64 = 0;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_82598CD8:
	// lbz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// bne cr6,0x82598cd8
	if (!ctx.cr6.eq) goto loc_82598CD8;
	// subf r11,r10,r11
	ctx.r11.u64 = ctx.r11.u64 - ctx.r10.u64;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// rotlwi r11,r11,0
	ctx.r11.u64 = __builtin_rotateleft32(ctx.r11.u32, 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82598d0c
	if (ctx.cr6.eq) goto loc_82598D0C;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82273620
	ctx.lr = 0x82598D08;
	sub_82273620(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
loc_82598D0C:
	// li r5,400
	ctx.r5.s64 = 400;
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8221dca0
	ctx.lr = 0x82598D1C;
	sub_8221DCA0(ctx, base);
	// li r9,-1
	ctx.r9.s64 = -1;
	// li r8,-1
	ctx.r8.s64 = -1;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8221eba0
	ctx.lr = 0x82598D3C;
	sub_8221EBA0(ctx, base);
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// fmr f2,f30
	ctx.fpscr.disableFlushMode();
	ctx.f2.f64 = ctx.f30.f64;
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// bl 0x82598ad8
	ctx.lr = 0x82598D4C;
	sub_82598AD8(ctx, base);
	// addi r1,r1,544
	ctx.r1.s64 = ctx.r1.s64 + 544;
	// lfd f30,-64(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -64);
	// lfd f31,-56(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -56);
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82598D60);  /* forward decl */
PPC_WEAK_FUNC(sub_82598D60) { __imp__sub_82598D60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82598D60) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x82598D68;
	__savegprlr_28(ctx, base);
	// stfd f30,-56(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -56, ctx.f30.u64);
	// stfd f31,-48(r1)
	PPC_STORE_U64(ctx.r1.u32 + -48, ctx.f31.u64);
	// stwu r1,-544(r1)
	ea = -544 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// mr r4,r5
	ctx.r4.u64 = ctx.r5.u64;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// addi r3,r11,12544
	ctx.r3.s64 = ctx.r11.s64 + 12544;
	// mr r30,r6
	ctx.r30.u64 = ctx.r6.u64;
	// mr r29,r7
	ctx.r29.u64 = ctx.r7.u64;
	// bl 0x82273620
	ctx.lr = 0x82598D94;
	sub_82273620(ctx, base);
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// mr r28,r3
	ctx.r28.u64 = ctx.r3.u64;
	// addi r31,r11,-15968
	ctx.r31.s64 = ctx.r11.s64 + -15968;
	// li r5,0
	ctx.r5.s64 = 0;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x821d1bb8
	ctx.lr = 0x82598DB0;
	sub_821D1BB8(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// li r5,0
	ctx.r5.s64 = 0;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x821d1bb8
	ctx.lr = 0x82598DC4;
	sub_821D1BB8(ctx, base);
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// li r5,400
	ctx.r5.s64 = 400;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8221dca0
	ctx.lr = 0x82598DD8;
	sub_8221DCA0(ctx, base);
	// mr r9,r29
	ctx.r9.u64 = ctx.r29.u64;
	// mr r8,r30
	ctx.r8.u64 = ctx.r30.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,1
	ctx.r6.s64 = 1;
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8221eba0
	ctx.lr = 0x82598DF8;
	sub_8221EBA0(ctx, base);
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// fmr f2,f30
	ctx.fpscr.disableFlushMode();
	ctx.f2.f64 = ctx.f30.f64;
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// bl 0x82598ad8
	ctx.lr = 0x82598E08;
	sub_82598AD8(ctx, base);
	// mr r28,r3
	ctx.r28.u64 = ctx.r3.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x821d1b28
	ctx.lr = 0x82598E18;
	sub_821D1B28(ctx, base);
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x821d1b28
	ctx.lr = 0x82598E24;
	sub_821D1B28(ctx, base);
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// addi r1,r1,544
	ctx.r1.s64 = ctx.r1.s64 + 544;
	// lfd f30,-56(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -56);
	// lfd f31,-48(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -48);
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82598E38);  /* forward decl */
PPC_WEAK_FUNC(sub_82598E38) { __imp__sub_82598E38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82598E38) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x82598E40;
	__savegprlr_27(ctx, base);
	// stfd f30,-64(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -64, ctx.f30.u64);
	// stfd f31,-56(r1)
	PPC_STORE_U64(ctx.r1.u32 + -56, ctx.f31.u64);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// addi r3,r11,12544
	ctx.r3.s64 = ctx.r11.s64 + 12544;
	// bl 0x82273620
	ctx.lr = 0x82598E68;
	sub_82273620(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lis r9,-32072
	ctx.r9.s64 = -2101870592;
	// addi r31,r10,3712
	ctx.r31.s64 = ctx.r10.s64 + 3712;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r11,-6256(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// addi r7,r31,64
	ctx.r7.s64 = ctx.r31.s64 + 64;
	// addi r6,r31,68
	ctx.r6.s64 = ctx.r31.s64 + 68;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lbz r9,1864(r9)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + 1864);
	// lwz r10,-6260(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -6260);
	// stfsx f31,r11,r7
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, temp.u32);
	// stfsx f30,r11,r6
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, temp.u32);
	// addi r28,r31,12
	ctx.r28.s64 = ctx.r31.s64 + 12;
	// addi r27,r31,58
	ctx.r27.s64 = ctx.r31.s64 + 58;
	// addi r8,r31,72
	ctx.r8.s64 = ctx.r31.s64 + 72;
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// add r3,r11,r8
	ctx.r3.u64 = ctx.r11.u64 + ctx.r8.u64;
	// stwx r10,r11,r28
	PPC_STORE_U32(ctx.r11.u32 + ctx.r28.u32, ctx.r10.u32);
	// stbx r9,r11,r27
	PPC_STORE_U8(ctx.r11.u32 + ctx.r27.u32, ctx.r9.u8);
	// bl 0x8298fef0
	ctx.lr = 0x82598EC0;
	sub_8298FEF0(ctx, base);
	// lwz r3,-6256(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// bl 0x821d1ab0
	ctx.lr = 0x82598EC8;
	sub_821D1AB0(ctx, base);
	// lwz r11,-6256(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lis r9,-32072
	ctx.r9.s64 = -2101870592;
	// mulli r8,r11,124
	ctx.r8.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lwz r10,1860(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 1860);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// stwx r10,r8,r31
	PPC_STORE_U32(ctx.r8.u32 + ctx.r31.u32, ctx.r10.u32);
	// stw r11,-6256(r30)
	PPC_STORE_U32(ctx.r30.u32 + -6256, ctx.r11.u32);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-22634(r9)
	PPC_STORE_U8(ctx.r9.u32 + -22634, ctx.r11.u8);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lfd f30,-64(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -64);
	// lfd f31,-56(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -56);
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82598F00);  /* forward decl */
PPC_WEAK_FUNC(sub_82598F00) { __imp__sub_82598F00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82598F00) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb10
	ctx.lr = 0x82598F08;
	__savegprlr_26(ctx, base);
	// stfd f30,-72(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -72, ctx.f30.u64);
	// stfd f31,-64(r1)
	PPC_STORE_U64(ctx.r1.u32 + -64, ctx.f31.u64);
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// addi r3,r11,12544
	ctx.r3.s64 = ctx.r11.s64 + 12544;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// bl 0x82273620
	ctx.lr = 0x82598F34;
	sub_82273620(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lis r9,-32072
	ctx.r9.s64 = -2101870592;
	// addi r31,r10,3712
	ctx.r31.s64 = ctx.r10.s64 + 3712;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r11,-6256(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// addi r7,r31,64
	ctx.r7.s64 = ctx.r31.s64 + 64;
	// addi r6,r31,68
	ctx.r6.s64 = ctx.r31.s64 + 68;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lbz r9,1864(r9)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + 1864);
	// lwz r10,-6260(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -6260);
	// stfsx f31,r11,r7
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, temp.u32);
	// stfsx f30,r11,r6
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, temp.u32);
	// addi r27,r31,12
	ctx.r27.s64 = ctx.r31.s64 + 12;
	// addi r26,r31,58
	ctx.r26.s64 = ctx.r31.s64 + 58;
	// addi r8,r31,72
	ctx.r8.s64 = ctx.r31.s64 + 72;
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// add r3,r11,r8
	ctx.r3.u64 = ctx.r11.u64 + ctx.r8.u64;
	// stwx r10,r11,r27
	PPC_STORE_U32(ctx.r11.u32 + ctx.r27.u32, ctx.r10.u32);
	// stbx r9,r11,r26
	PPC_STORE_U8(ctx.r11.u32 + ctx.r26.u32, ctx.r9.u8);
	// bl 0x8298fef0
	ctx.lr = 0x82598F8C;
	sub_8298FEF0(ctx, base);
	// lwz r3,-6256(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// bl 0x821d1ab0
	ctx.lr = 0x82598F94;
	sub_821D1AB0(ctx, base);
	// lwz r11,-6256(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// lis r9,-32072
	ctx.r9.s64 = -2101870592;
	// addi r7,r31,96
	ctx.r7.s64 = ctx.r31.s64 + 96;
	// mulli r10,r11,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lwz r9,1860(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + 1860);
	// stwx r28,r10,r7
	PPC_STORE_U32(ctx.r10.u32 + ctx.r7.u32, ctx.r28.u32);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// lis r8,-32072
	ctx.r8.s64 = -2101870592;
	// stwx r9,r10,r31
	PPC_STORE_U32(ctx.r10.u32 + ctx.r31.u32, ctx.r9.u32);
	// stw r11,-6256(r30)
	PPC_STORE_U32(ctx.r30.u32 + -6256, ctx.r11.u32);
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-22634(r8)
	PPC_STORE_U8(ctx.r8.u32 + -22634, ctx.r11.u8);
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// lfd f30,-72(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -72);
	// lfd f31,-64(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -64);
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82598FD8);  /* forward decl */
PPC_WEAK_FUNC(sub_82598FD8) { __imp__sub_82598FD8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82598FD8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x82598FE0;
	__savegprlr_25(ctx, base);
	// stfd f30,-80(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -80, ctx.f30.u64);
	// stfd f31,-72(r1)
	PPC_STORE_U64(ctx.r1.u32 + -72, ctx.f31.u64);
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// addi r3,r11,12544
	ctx.r3.s64 = ctx.r11.s64 + 12544;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// bl 0x82273620
	ctx.lr = 0x82599010;
	sub_82273620(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lis r9,-32072
	ctx.r9.s64 = -2101870592;
	// addi r31,r10,3712
	ctx.r31.s64 = ctx.r10.s64 + 3712;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r11,-6256(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// addi r7,r31,64
	ctx.r7.s64 = ctx.r31.s64 + 64;
	// addi r6,r31,68
	ctx.r6.s64 = ctx.r31.s64 + 68;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lbz r9,1864(r9)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + 1864);
	// lwz r10,-6260(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -6260);
	// stfsx f31,r11,r7
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, temp.u32);
	// stfsx f30,r11,r6
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, temp.u32);
	// addi r26,r31,12
	ctx.r26.s64 = ctx.r31.s64 + 12;
	// addi r25,r31,58
	ctx.r25.s64 = ctx.r31.s64 + 58;
	// addi r8,r31,72
	ctx.r8.s64 = ctx.r31.s64 + 72;
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// add r3,r11,r8
	ctx.r3.u64 = ctx.r11.u64 + ctx.r8.u64;
	// stwx r10,r11,r26
	PPC_STORE_U32(ctx.r11.u32 + ctx.r26.u32, ctx.r10.u32);
	// stbx r9,r11,r25
	PPC_STORE_U8(ctx.r11.u32 + ctx.r25.u32, ctx.r9.u8);
	// bl 0x8298fef0
	ctx.lr = 0x82599068;
	sub_8298FEF0(ctx, base);
	// lwz r3,-6256(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// bl 0x821d1ab0
	ctx.lr = 0x82599070;
	sub_821D1AB0(ctx, base);
	// lwz r10,-6256(r30)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// lis r9,-32072
	ctx.r9.s64 = -2101870592;
	// addi r7,r31,96
	ctx.r7.s64 = ctx.r31.s64 + 96;
	// mulli r11,r10,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// lwz r9,1860(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + 1860);
	// stwx r28,r11,r7
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, ctx.r28.u32);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r6,r31,100
	ctx.r6.s64 = ctx.r31.s64 + 100;
	// lis r8,-32072
	ctx.r8.s64 = -2101870592;
	// stw r10,-6256(r30)
	PPC_STORE_U32(ctx.r30.u32 + -6256, ctx.r10.u32);
	// li r10,1
	ctx.r10.s64 = 1;
	// stwx r27,r11,r6
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, ctx.r27.u32);
	// stwx r9,r11,r31
	PPC_STORE_U32(ctx.r11.u32 + ctx.r31.u32, ctx.r9.u32);
	// stb r10,-22634(r8)
	PPC_STORE_U8(ctx.r8.u32 + -22634, ctx.r10.u8);
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// lfd f30,-80(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -80);
	// lfd f31,-72(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -72);
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825990B8);  /* forward decl */
PPC_WEAK_FUNC(sub_825990B8) { __imp__sub_825990B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825990B8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb08
	ctx.lr = 0x825990C0;
	__savegprlr_24(ctx, base);
	// stfd f30,-88(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -88, ctx.f30.u64);
	// stfd f31,-80(r1)
	PPC_STORE_U64(ctx.r1.u32 + -80, ctx.f31.u64);
	// stwu r1,-176(r1)
	ea = -176 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// addi r3,r11,12544
	ctx.r3.s64 = ctx.r11.s64 + 12544;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// mr r26,r8
	ctx.r26.u64 = ctx.r8.u64;
	// bl 0x82273620
	ctx.lr = 0x825990F4;
	sub_82273620(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lis r9,-32072
	ctx.r9.s64 = -2101870592;
	// addi r31,r10,3712
	ctx.r31.s64 = ctx.r10.s64 + 3712;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r11,-6256(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// addi r7,r31,64
	ctx.r7.s64 = ctx.r31.s64 + 64;
	// addi r6,r31,68
	ctx.r6.s64 = ctx.r31.s64 + 68;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lbz r9,1864(r9)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + 1864);
	// lwz r10,-6260(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -6260);
	// stfsx f31,r11,r7
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, temp.u32);
	// stfsx f30,r11,r6
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, temp.u32);
	// addi r25,r31,12
	ctx.r25.s64 = ctx.r31.s64 + 12;
	// addi r24,r31,58
	ctx.r24.s64 = ctx.r31.s64 + 58;
	// addi r8,r31,72
	ctx.r8.s64 = ctx.r31.s64 + 72;
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// add r3,r11,r8
	ctx.r3.u64 = ctx.r11.u64 + ctx.r8.u64;
	// stwx r10,r11,r25
	PPC_STORE_U32(ctx.r11.u32 + ctx.r25.u32, ctx.r10.u32);
	// stbx r9,r11,r24
	PPC_STORE_U8(ctx.r11.u32 + ctx.r24.u32, ctx.r9.u8);
	// bl 0x8298fef0
	ctx.lr = 0x8259914C;
	sub_8298FEF0(ctx, base);
	// lwz r3,-6256(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// bl 0x821d1ab0
	ctx.lr = 0x82599154;
	sub_821D1AB0(ctx, base);
	// lwz r10,-6256(r30)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// lis r9,-32072
	ctx.r9.s64 = -2101870592;
	// addi r7,r31,96
	ctx.r7.s64 = ctx.r31.s64 + 96;
	// mulli r11,r10,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// lwz r9,1860(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + 1860);
	// stwx r28,r11,r7
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, ctx.r28.u32);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r6,r31,100
	ctx.r6.s64 = ctx.r31.s64 + 100;
	// addi r5,r31,104
	ctx.r5.s64 = ctx.r31.s64 + 104;
	// lis r8,-32072
	ctx.r8.s64 = -2101870592;
	// stw r10,-6256(r30)
	PPC_STORE_U32(ctx.r30.u32 + -6256, ctx.r10.u32);
	// li r10,1
	ctx.r10.s64 = 1;
	// stwx r27,r11,r6
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, ctx.r27.u32);
	// stwx r26,r11,r5
	PPC_STORE_U32(ctx.r11.u32 + ctx.r5.u32, ctx.r26.u32);
	// stwx r9,r11,r31
	PPC_STORE_U32(ctx.r11.u32 + ctx.r31.u32, ctx.r9.u32);
	// stb r10,-22634(r8)
	PPC_STORE_U8(ctx.r8.u32 + -22634, ctx.r10.u8);
	// addi r1,r1,176
	ctx.r1.s64 = ctx.r1.s64 + 176;
	// lfd f30,-88(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -88);
	// lfd f31,-80(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -80);
	// b 0x8298eb58
	__restgprlr_24(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825991A8);  /* forward decl */
PPC_WEAK_FUNC(sub_825991A8) { __imp__sub_825991A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825991A8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb10
	ctx.lr = 0x825991B0;
	__savegprlr_26(ctx, base);
	// stfd f29,-80(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -80, ctx.f29.u64);
	// stfd f30,-72(r1)
	PPC_STORE_U64(ctx.r1.u32 + -72, ctx.f30.u64);
	// stfd f31,-64(r1)
	PPC_STORE_U64(ctx.r1.u32 + -64, ctx.f31.u64);
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// fmr f29,f3
	ctx.f29.f64 = ctx.f3.f64;
	// addi r3,r11,12544
	ctx.r3.s64 = ctx.r11.s64 + 12544;
	// mr r28,r7
	ctx.r28.u64 = ctx.r7.u64;
	// bl 0x82273620
	ctx.lr = 0x825991E4;
	sub_82273620(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lis r9,-32072
	ctx.r9.s64 = -2101870592;
	// addi r31,r10,3712
	ctx.r31.s64 = ctx.r10.s64 + 3712;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r11,-6256(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// addi r7,r31,64
	ctx.r7.s64 = ctx.r31.s64 + 64;
	// addi r6,r31,68
	ctx.r6.s64 = ctx.r31.s64 + 68;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lbz r9,1864(r9)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + 1864);
	// lwz r10,-6260(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -6260);
	// stfsx f31,r11,r7
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, temp.u32);
	// stfsx f30,r11,r6
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, temp.u32);
	// addi r27,r31,12
	ctx.r27.s64 = ctx.r31.s64 + 12;
	// addi r26,r31,58
	ctx.r26.s64 = ctx.r31.s64 + 58;
	// addi r8,r31,72
	ctx.r8.s64 = ctx.r31.s64 + 72;
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// add r3,r11,r8
	ctx.r3.u64 = ctx.r11.u64 + ctx.r8.u64;
	// stwx r10,r11,r27
	PPC_STORE_U32(ctx.r11.u32 + ctx.r27.u32, ctx.r10.u32);
	// stbx r9,r11,r26
	PPC_STORE_U8(ctx.r11.u32 + ctx.r26.u32, ctx.r9.u8);
	// bl 0x8298fef0
	ctx.lr = 0x8259923C;
	sub_8298FEF0(ctx, base);
	// lwz r3,-6256(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// bl 0x821d1ab0
	ctx.lr = 0x82599244;
	sub_821D1AB0(ctx, base);
	// lwz r10,-6256(r30)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// lis r9,-32072
	ctx.r9.s64 = -2101870592;
	// addi r7,r31,108
	ctx.r7.s64 = ctx.r31.s64 + 108;
	// mulli r11,r10,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// lwz r9,1860(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + 1860);
	// stfsx f29,r11,r7
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f29.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, temp.u32);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r6,r31,112
	ctx.r6.s64 = ctx.r31.s64 + 112;
	// lis r8,-32072
	ctx.r8.s64 = -2101870592;
	// stw r10,-6256(r30)
	PPC_STORE_U32(ctx.r30.u32 + -6256, ctx.r10.u32);
	// li r10,1
	ctx.r10.s64 = 1;
	// stwx r28,r11,r6
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, ctx.r28.u32);
	// stwx r9,r11,r31
	PPC_STORE_U32(ctx.r11.u32 + ctx.r31.u32, ctx.r9.u32);
	// stb r10,-22634(r8)
	PPC_STORE_U8(ctx.r8.u32 + -22634, ctx.r10.u8);
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// lfd f29,-80(r1)
	ctx.f29.u64 = PPC_LOAD_U64(ctx.r1.u32 + -80);
	// lfd f30,-72(r1)
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -72);
	// lfd f31,-64(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -64);
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82599290);  /* forward decl */
PPC_WEAK_FUNC(sub_82599290) { __imp__sub_82599290(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82599290) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb10
	ctx.lr = 0x82599298;
	__savegprlr_26(ctx, base);
	// stfd f30,-72(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -72, ctx.f30.u64);
	// stfd f31,-64(r1)
	PPC_STORE_U64(ctx.r1.u32 + -64, ctx.f31.u64);
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// addi r3,r11,12544
	ctx.r3.s64 = ctx.r11.s64 + 12544;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// bl 0x82273620
	ctx.lr = 0x825992C4;
	sub_82273620(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lis r9,-32072
	ctx.r9.s64 = -2101870592;
	// addi r31,r10,3712
	ctx.r31.s64 = ctx.r10.s64 + 3712;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r11,-6256(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// addi r7,r31,64
	ctx.r7.s64 = ctx.r31.s64 + 64;
	// addi r6,r31,68
	ctx.r6.s64 = ctx.r31.s64 + 68;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lbz r9,1864(r9)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + 1864);
	// lwz r10,-6260(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -6260);
	// stfsx f31,r11,r7
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, temp.u32);
	// stfsx f30,r11,r6
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, temp.u32);
	// addi r27,r31,12
	ctx.r27.s64 = ctx.r31.s64 + 12;
	// addi r26,r31,58
	ctx.r26.s64 = ctx.r31.s64 + 58;
	// addi r8,r31,72
	ctx.r8.s64 = ctx.r31.s64 + 72;
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// add r3,r11,r8
	ctx.r3.u64 = ctx.r11.u64 + ctx.r8.u64;
	// stwx r10,r11,r27
	PPC_STORE_U32(ctx.r11.u32 + ctx.r27.u32, ctx.r10.u32);
	// stbx r9,r11,r26
	PPC_STORE_U8(ctx.r11.u32 + ctx.r26.u32, ctx.r9.u8);
	// bl 0x8298fef0
	ctx.lr = 0x8259931C;
	sub_8298FEF0(ctx, base);
	// lwz r3,-6256(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// bl 0x821d1ab0
	ctx.lr = 0x82599324;
	sub_821D1AB0(ctx, base);
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r3,r11,-15968
	ctx.r3.s64 = ctx.r11.s64 + -15968;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// bl 0x821d1bb8
	ctx.lr = 0x82599338;
	sub_821D1BB8(ctx, base);
	// lwz r11,-6256(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// lis r9,-32072
	ctx.r9.s64 = -2101870592;
	// mulli r10,r11,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lwz r9,1860(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + 1860);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// addi r7,r31,116
	ctx.r7.s64 = ctx.r31.s64 + 116;
	// lis r8,-32072
	ctx.r8.s64 = -2101870592;
	// stw r11,-6256(r30)
	PPC_STORE_U32(ctx.r30.u32 + -6256, ctx.r11.u32);
	// li r11,1
	ctx.r11.s64 = 1;
	// stwx r3,r10,r7
	PPC_STORE_U32(ctx.r10.u32 + ctx.r7.u32, ctx.r3.u32);
	// stwx r9,r10,r31
	PPC_STORE_U32(ctx.r10.u32 + ctx.r31.u32, ctx.r9.u32);
	// stb r11,-22634(r8)
	PPC_STORE_U8(ctx.r8.u32 + -22634, ctx.r11.u8);
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// lfd f30,-72(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -72);
	// lfd f31,-64(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -64);
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82599378);  /* forward decl */
PPC_WEAK_FUNC(sub_82599378) { __imp__sub_82599378(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82599378) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x82599380;
	__savegprlr_25(ctx, base);
	// stfd f30,-80(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -80, ctx.f30.u64);
	// stfd f31,-72(r1)
	PPC_STORE_U64(ctx.r1.u32 + -72, ctx.f31.u64);
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// addi r3,r11,12544
	ctx.r3.s64 = ctx.r11.s64 + 12544;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r27,r7
	ctx.r27.u64 = ctx.r7.u64;
	// bl 0x82273620
	ctx.lr = 0x825993B0;
	sub_82273620(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lis r9,-32072
	ctx.r9.s64 = -2101870592;
	// addi r31,r10,3712
	ctx.r31.s64 = ctx.r10.s64 + 3712;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r11,-6256(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// addi r7,r31,64
	ctx.r7.s64 = ctx.r31.s64 + 64;
	// addi r6,r31,68
	ctx.r6.s64 = ctx.r31.s64 + 68;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lbz r9,1864(r9)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + 1864);
	// lwz r10,-6260(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -6260);
	// stfsx f31,r11,r7
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, temp.u32);
	// stfsx f30,r11,r6
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, temp.u32);
	// addi r26,r31,12
	ctx.r26.s64 = ctx.r31.s64 + 12;
	// addi r25,r31,58
	ctx.r25.s64 = ctx.r31.s64 + 58;
	// addi r8,r31,72
	ctx.r8.s64 = ctx.r31.s64 + 72;
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// add r3,r11,r8
	ctx.r3.u64 = ctx.r11.u64 + ctx.r8.u64;
	// stwx r10,r11,r26
	PPC_STORE_U32(ctx.r11.u32 + ctx.r26.u32, ctx.r10.u32);
	// stbx r9,r11,r25
	PPC_STORE_U8(ctx.r11.u32 + ctx.r25.u32, ctx.r9.u8);
	// bl 0x8298fef0
	ctx.lr = 0x82599408;
	sub_8298FEF0(ctx, base);
	// lwz r3,-6256(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// bl 0x821d1ab0
	ctx.lr = 0x82599410;
	sub_821D1AB0(ctx, base);
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// li r5,0
	ctx.r5.s64 = 0;
	// addi r29,r11,-15968
	ctx.r29.s64 = ctx.r11.s64 + -15968;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x821d1bb8
	ctx.lr = 0x82599428;
	sub_821D1BB8(ctx, base);
	// mr r28,r3
	ctx.r28.u64 = ctx.r3.u64;
	// li r5,0
	ctx.r5.s64 = 0;
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x821d1bb8
	ctx.lr = 0x8259943C;
	sub_821D1BB8(ctx, base);
	// lwz r10,-6256(r30)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// lis r9,-32072
	ctx.r9.s64 = -2101870592;
	// mulli r11,r10,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// lwz r9,1860(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + 1860);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r7,r31,116
	ctx.r7.s64 = ctx.r31.s64 + 116;
	// addi r6,r31,120
	ctx.r6.s64 = ctx.r31.s64 + 120;
	// lis r8,-32072
	ctx.r8.s64 = -2101870592;
	// stw r10,-6256(r30)
	PPC_STORE_U32(ctx.r30.u32 + -6256, ctx.r10.u32);
	// li r10,1
	ctx.r10.s64 = 1;
	// stwx r28,r11,r7
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, ctx.r28.u32);
	// stwx r3,r11,r6
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, ctx.r3.u32);
	// stwx r9,r11,r31
	PPC_STORE_U32(ctx.r11.u32 + ctx.r31.u32, ctx.r9.u32);
	// stb r10,-22634(r8)
	PPC_STORE_U8(ctx.r8.u32 + -22634, ctx.r10.u8);
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// lfd f30,-80(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -80);
	// lfd f31,-72(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -72);
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82599488);  /* forward decl */
PPC_WEAK_FUNC(sub_82599488) { __imp__sub_82599488(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82599488) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb10
	ctx.lr = 0x82599490;
	__savegprlr_26(ctx, base);
	// stfd f30,-72(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -72, ctx.f30.u64);
	// stfd f31,-64(r1)
	PPC_STORE_U64(ctx.r1.u32 + -64, ctx.f31.u64);
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// addi r31,r11,12544
	ctx.r31.s64 = ctx.r11.s64 + 12544;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r30,r6
	ctx.r30.u64 = ctx.r6.u64;
	// bl 0x82273620
	ctx.lr = 0x825994C0;
	sub_82273620(ctx, base);
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82273620
	ctx.lr = 0x825994CC;
	sub_82273620(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lis r9,-32072
	ctx.r9.s64 = -2101870592;
	// addi r31,r10,3712
	ctx.r31.s64 = ctx.r10.s64 + 3712;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r11,-6256(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// addi r7,r31,64
	ctx.r7.s64 = ctx.r31.s64 + 64;
	// addi r6,r31,68
	ctx.r6.s64 = ctx.r31.s64 + 68;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lbz r9,1864(r9)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + 1864);
	// lwz r10,-6260(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -6260);
	// stfsx f31,r11,r7
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, temp.u32);
	// stfsx f30,r11,r6
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, temp.u32);
	// addi r27,r31,12
	ctx.r27.s64 = ctx.r31.s64 + 12;
	// addi r26,r31,58
	ctx.r26.s64 = ctx.r31.s64 + 58;
	// addi r8,r31,72
	ctx.r8.s64 = ctx.r31.s64 + 72;
	// mr r28,r3
	ctx.r28.u64 = ctx.r3.u64;
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// add r3,r11,r8
	ctx.r3.u64 = ctx.r11.u64 + ctx.r8.u64;
	// stwx r10,r11,r27
	PPC_STORE_U32(ctx.r11.u32 + ctx.r27.u32, ctx.r10.u32);
	// stbx r9,r11,r26
	PPC_STORE_U8(ctx.r11.u32 + ctx.r26.u32, ctx.r9.u8);
	// bl 0x8298fef0
	ctx.lr = 0x82599528;
	sub_8298FEF0(ctx, base);
	// lwz r3,-6256(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// bl 0x821d1ab0
	ctx.lr = 0x82599530;
	sub_821D1AB0(ctx, base);
	// lwz r11,-6256(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// lis r9,-32072
	ctx.r9.s64 = -2101870592;
	// mulli r10,r11,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lwz r9,1860(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + 1860);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// addi r7,r31,88
	ctx.r7.s64 = ctx.r31.s64 + 88;
	// lis r8,-32072
	ctx.r8.s64 = -2101870592;
	// stw r11,-6256(r30)
	PPC_STORE_U32(ctx.r30.u32 + -6256, ctx.r11.u32);
	// li r11,1
	ctx.r11.s64 = 1;
	// stwx r28,r10,r7
	PPC_STORE_U32(ctx.r10.u32 + ctx.r7.u32, ctx.r28.u32);
	// stwx r9,r10,r31
	PPC_STORE_U32(ctx.r10.u32 + ctx.r31.u32, ctx.r9.u32);
	// stb r11,-22634(r8)
	PPC_STORE_U8(ctx.r8.u32 + -22634, ctx.r11.u8);
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// lfd f30,-72(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -72);
	// lfd f31,-64(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -64);
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82599570);  /* forward decl */
PPC_WEAK_FUNC(sub_82599570) { __imp__sub_82599570(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82599570) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb0c
	ctx.lr = 0x82599578;
	__savegprlr_25(ctx, base);
	// stfd f30,-80(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -80, ctx.f30.u64);
	// stfd f31,-72(r1)
	PPC_STORE_U64(ctx.r1.u32 + -72, ctx.f31.u64);
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// addi r31,r11,12544
	ctx.r31.s64 = ctx.r11.s64 + 12544;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r30,r6
	ctx.r30.u64 = ctx.r6.u64;
	// mr r28,r7
	ctx.r28.u64 = ctx.r7.u64;
	// bl 0x82273620
	ctx.lr = 0x825995AC;
	sub_82273620(ctx, base);
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82273620
	ctx.lr = 0x825995B8;
	sub_82273620(ctx, base);
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82273620
	ctx.lr = 0x825995C8;
	sub_82273620(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lis r9,-32072
	ctx.r9.s64 = -2101870592;
	// addi r31,r10,3712
	ctx.r31.s64 = ctx.r10.s64 + 3712;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r11,-6256(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// addi r7,r31,64
	ctx.r7.s64 = ctx.r31.s64 + 64;
	// addi r6,r31,68
	ctx.r6.s64 = ctx.r31.s64 + 68;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// lbz r9,1864(r9)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + 1864);
	// lwz r10,-6260(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -6260);
	// stfsx f31,r11,r7
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, temp.u32);
	// stfsx f30,r11,r6
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, temp.u32);
	// addi r26,r31,12
	ctx.r26.s64 = ctx.r31.s64 + 12;
	// addi r25,r31,58
	ctx.r25.s64 = ctx.r31.s64 + 58;
	// addi r8,r31,72
	ctx.r8.s64 = ctx.r31.s64 + 72;
	// mr r28,r3
	ctx.r28.u64 = ctx.r3.u64;
	// li r5,16
	ctx.r5.s64 = 16;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// add r3,r11,r8
	ctx.r3.u64 = ctx.r11.u64 + ctx.r8.u64;
	// stwx r10,r11,r26
	PPC_STORE_U32(ctx.r11.u32 + ctx.r26.u32, ctx.r10.u32);
	// stbx r9,r11,r25
	PPC_STORE_U8(ctx.r11.u32 + ctx.r25.u32, ctx.r9.u8);
	// bl 0x8298fef0
	ctx.lr = 0x82599624;
	sub_8298FEF0(ctx, base);
	// lwz r3,-6256(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// bl 0x821d1ab0
	ctx.lr = 0x8259962C;
	sub_821D1AB0(ctx, base);
	// lwz r10,-6256(r30)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r30.u32 + -6256);
	// lis r9,-32072
	ctx.r9.s64 = -2101870592;
	// mulli r11,r10,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// lwz r9,1860(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + 1860);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r7,r31,88
	ctx.r7.s64 = ctx.r31.s64 + 88;
	// addi r6,r31,92
	ctx.r6.s64 = ctx.r31.s64 + 92;
	// lis r8,-32072
	ctx.r8.s64 = -2101870592;
	// stw r10,-6256(r30)
	PPC_STORE_U32(ctx.r30.u32 + -6256, ctx.r10.u32);
	// li r10,1
	ctx.r10.s64 = 1;
	// stwx r27,r11,r7
	PPC_STORE_U32(ctx.r11.u32 + ctx.r7.u32, ctx.r27.u32);
	// stwx r28,r11,r6
	PPC_STORE_U32(ctx.r11.u32 + ctx.r6.u32, ctx.r28.u32);
	// stwx r9,r11,r31
	PPC_STORE_U32(ctx.r11.u32 + ctx.r31.u32, ctx.r9.u32);
	// stb r10,-22634(r8)
	PPC_STORE_U8(ctx.r8.u32 + -22634, ctx.r10.u8);
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// lfd f30,-80(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -80);
	// lfd f31,-72(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -72);
	// b 0x8298eb5c
	__restgprlr_25(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82599678);  /* forward decl */
PPC_WEAK_FUNC(sub_82599678) { __imp__sub_82599678(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82599678) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb10
	ctx.lr = 0x82599680;
	__savegprlr_26(ctx, base);
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// lwz r11,-6256(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -6256);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x825997b8
	if (ctx.cr6.eq) goto loc_825997B8;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// mulli r11,r11,124
	ctx.r11.s64 = static_cast<int64_t>(ctx.r11.u64 * static_cast<uint64_t>(124));
	// addi r10,r10,3712
	ctx.r10.s64 = ctx.r10.s64 + 3712;
	// addi r9,r10,4
	ctx.r9.s64 = ctx.r10.s64 + 4;
	// addi r8,r10,8
	ctx.r8.s64 = ctx.r10.s64 + 8;
	// add r9,r11,r9
	ctx.r9.u64 = ctx.r11.u64 + ctx.r9.u64;
	// addi r7,r10,20
	ctx.r7.s64 = ctx.r10.s64 + 20;
	// add r8,r11,r8
	ctx.r8.u64 = ctx.r11.u64 + ctx.r8.u64;
	// add r7,r11,r7
	ctx.r7.u64 = ctx.r11.u64 + ctx.r7.u64;
	// addi r6,r10,24
	ctx.r6.s64 = ctx.r10.s64 + 24;
	// lfs f0,-124(r9)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + -124);
	ctx.f0.f64 = double(temp.f32);
	// addi r5,r10,25
	ctx.r5.s64 = ctx.r10.s64 + 25;
	// stfs f0,0(r9)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// add r6,r11,r6
	ctx.r6.u64 = ctx.r11.u64 + ctx.r6.u64;
	// lfs f0,-124(r8)
	temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + -124);
	ctx.f0.f64 = double(temp.f32);
	// add r5,r11,r5
	ctx.r5.u64 = ctx.r11.u64 + ctx.r5.u64;
	// stfs f0,0(r8)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// lwz r9,-124(r7)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r7.u32 + -124);
	// addi r4,r10,26
	ctx.r4.s64 = ctx.r10.s64 + 26;
	// addi r3,r10,27
	ctx.r3.s64 = ctx.r10.s64 + 27;
	// add r4,r11,r4
	ctx.r4.u64 = ctx.r11.u64 + ctx.r4.u64;
	// add r3,r11,r3
	ctx.r3.u64 = ctx.r11.u64 + ctx.r3.u64;
	// stw r9,0(r7)
	PPC_STORE_U32(ctx.r7.u32 + 0, ctx.r9.u32);
	// addi r31,r10,28
	ctx.r31.s64 = ctx.r10.s64 + 28;
	// lbz r9,-124(r6)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r6.u32 + -124);
	// addi r30,r10,29
	ctx.r30.s64 = ctx.r10.s64 + 29;
	// add r31,r11,r31
	ctx.r31.u64 = ctx.r11.u64 + ctx.r31.u64;
	// add r30,r11,r30
	ctx.r30.u64 = ctx.r11.u64 + ctx.r30.u64;
	// addi r29,r10,30
	ctx.r29.s64 = ctx.r10.s64 + 30;
	// addi r28,r10,32
	ctx.r28.s64 = ctx.r10.s64 + 32;
	// stb r9,0(r6)
	PPC_STORE_U8(ctx.r6.u32 + 0, ctx.r9.u8);
	// add r29,r11,r29
	ctx.r29.u64 = ctx.r11.u64 + ctx.r29.u64;
	// lbz r9,-124(r5)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r5.u32 + -124);
	// addi r27,r10,36
	ctx.r27.s64 = ctx.r10.s64 + 36;
	// add r28,r11,r28
	ctx.r28.u64 = ctx.r11.u64 + ctx.r28.u64;
	// addi r26,r10,48
	ctx.r26.s64 = ctx.r10.s64 + 48;
	// add r27,r11,r27
	ctx.r27.u64 = ctx.r11.u64 + ctx.r27.u64;
	// add r26,r11,r26
	ctx.r26.u64 = ctx.r11.u64 + ctx.r26.u64;
	// stb r9,0(r5)
	PPC_STORE_U8(ctx.r5.u32 + 0, ctx.r9.u8);
	// addi r8,r10,56
	ctx.r8.s64 = ctx.r10.s64 + 56;
	// lbz r9,-124(r4)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r4.u32 + -124);
	// addi r7,r10,57
	ctx.r7.s64 = ctx.r10.s64 + 57;
	// addi r6,r10,60
	ctx.r6.s64 = ctx.r10.s64 + 60;
	// add r8,r11,r8
	ctx.r8.u64 = ctx.r11.u64 + ctx.r8.u64;
	// add r7,r11,r7
	ctx.r7.u64 = ctx.r11.u64 + ctx.r7.u64;
	// add r6,r11,r6
	ctx.r6.u64 = ctx.r11.u64 + ctx.r6.u64;
	// stb r9,0(r4)
	PPC_STORE_U8(ctx.r4.u32 + 0, ctx.r9.u8);
	// lbz r9,-124(r3)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r3.u32 + -124);
	// stb r9,0(r3)
	PPC_STORE_U8(ctx.r3.u32 + 0, ctx.r9.u8);
	// lbz r9,-124(r31)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r31.u32 + -124);
	// stb r9,0(r31)
	PPC_STORE_U8(ctx.r31.u32 + 0, ctx.r9.u8);
	// lbz r9,-124(r30)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r30.u32 + -124);
	// stb r9,0(r30)
	PPC_STORE_U8(ctx.r30.u32 + 0, ctx.r9.u8);
	// lbz r9,-124(r29)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r29.u32 + -124);
	// stb r9,0(r29)
	PPC_STORE_U8(ctx.r29.u32 + 0, ctx.r9.u8);
	// lfs f0,-124(r28)
	temp.u32 = PPC_LOAD_U32(ctx.r28.u32 + -124);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r28)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r28.u32 + 0, temp.u32);
	// lfs f0,-124(r27)
	temp.u32 = PPC_LOAD_U32(ctx.r27.u32 + -124);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,0(r27)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r27.u32 + 0, temp.u32);
	// lwz r9,-124(r26)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r26.u32 + -124);
	// stw r9,0(r26)
	PPC_STORE_U32(ctx.r26.u32 + 0, ctx.r9.u32);
	// addi r9,r10,52
	ctx.r9.s64 = ctx.r10.s64 + 52;
	// add r9,r11,r9
	ctx.r9.u64 = ctx.r11.u64 + ctx.r9.u64;
	// add r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
	// lwz r10,-124(r9)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r9.u32 + -124);
	// stw r10,0(r9)
	PPC_STORE_U32(ctx.r9.u32 + 0, ctx.r10.u32);
	// lbz r10,-124(r8)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r8.u32 + -124);
	// stb r10,0(r8)
	PPC_STORE_U8(ctx.r8.u32 + 0, ctx.r10.u8);
	// lbz r10,-124(r7)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r7.u32 + -124);
	// stb r10,0(r7)
	PPC_STORE_U8(ctx.r7.u32 + 0, ctx.r10.u8);
	// lwz r10,-124(r6)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r6.u32 + -124);
	// stw r10,0(r6)
	PPC_STORE_U32(ctx.r6.u32 + 0, ctx.r10.u32);
	// lwz r10,-124(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + -124);
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
loc_825997B8:
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_825997C0);  /* forward decl */
PPC_WEAK_FUNC(sub_825997C0) { __imp__sub_825997C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_825997C0) {
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
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// lbz r11,0(r4)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r4.u32 + 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82599864
	if (ctx.cr6.eq) goto loc_82599864;
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r30,r11,-15968
	ctx.r30.s64 = ctx.r11.s64 + -15968;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x821d1bb8
	ctx.lr = 0x825997F8;
	sub_821D1BB8(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// cmpwi cr6,r31,-1
	ctx.cr6.compare<int32_t>(ctx.r31.s32, -1, ctx.xer);
	// beq cr6,0x82599864
	if (ctx.cr6.eq) goto loc_82599864;
	// li r5,0
	ctx.r5.s64 = 0;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x821d1b50
	ctx.lr = 0x82599814;
	sub_821D1B50(ctx, base);
	// lis r11,-32253
	ctx.r11.s64 = -2113732608;
	// addi r4,r11,31908
	ctx.r4.s64 = ctx.r11.s64 + 31908;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// addi r3,r11,12544
	ctx.r3.s64 = ctx.r11.s64 + 12544;
	// bl 0x82273620
	ctx.lr = 0x82599828;
	sub_82273620(ctx, base);
	// li r8,0
	ctx.r8.s64 = 0;
	// li r11,-1
	ctx.r11.s64 = -1;
	// stw r31,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r31.u32);
	// li r10,-1
	ctx.r10.s64 = -1;
	// li r9,-1
	ctx.r9.s64 = -1;
	// li r7,-1
	ctx.r7.s64 = -1;
	// stb r8,119(r1)
	PPC_STORE_U8(ctx.r1.u32 + 119, ctx.r8.u8);
	// li r6,-1
	ctx.r6.s64 = -1;
	// li r8,-1
	ctx.r8.s64 = -1;
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// li r5,0
	ctx.r5.s64 = 0;
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// li r4,-1
	ctx.r4.s64 = -1;
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x8221f2f0
	ctx.lr = 0x82599864;
	sub_8221F2F0(ctx, base);
loc_82599864:
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

PPC_FUNC_IMPL(__imp__sub_82599880);  /* forward decl */
PPC_WEAK_FUNC(sub_82599880) { __imp__sub_82599880(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82599880) {
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
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// lbz r11,0(r4)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r4.u32 + 0);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82599924
	if (ctx.cr6.eq) goto loc_82599924;
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// li r5,1
	ctx.r5.s64 = 1;
	// addi r30,r11,-15968
	ctx.r30.s64 = ctx.r11.s64 + -15968;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x821d1bb8
	ctx.lr = 0x825998B8;
	sub_821D1BB8(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// cmpwi cr6,r31,-1
	ctx.cr6.compare<int32_t>(ctx.r31.s32, -1, ctx.xer);
	// beq cr6,0x82599924
	if (ctx.cr6.eq) goto loc_82599924;
	// li r5,0
	ctx.r5.s64 = 0;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x821d1b50
	ctx.lr = 0x825998D4;
	sub_821D1B50(ctx, base);
	// lis r11,-32253
	ctx.r11.s64 = -2113732608;
	// addi r4,r11,31908
	ctx.r4.s64 = ctx.r11.s64 + 31908;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// addi r3,r11,12544
	ctx.r3.s64 = ctx.r11.s64 + 12544;
	// bl 0x82273620
	ctx.lr = 0x825998E8;
	sub_82273620(ctx, base);
	// li r8,1
	ctx.r8.s64 = 1;
	// li r11,-1
	ctx.r11.s64 = -1;
	// stw r31,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r31.u32);
	// li r10,-1
	ctx.r10.s64 = -1;
	// li r9,-1
	ctx.r9.s64 = -1;
	// li r7,-1
	ctx.r7.s64 = -1;
	// stb r8,119(r1)
	PPC_STORE_U8(ctx.r1.u32 + 119, ctx.r8.u8);
	// li r6,-1
	ctx.r6.s64 = -1;
	// li r8,-1
	ctx.r8.s64 = -1;
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// li r5,0
	ctx.r5.s64 = 0;
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// li r4,-1
	ctx.r4.s64 = -1;
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x8221f2f0
	ctx.lr = 0x82599924;
	sub_8221F2F0(ctx, base);
loc_82599924:
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

PPC_FUNC_IMPL(__imp__sub_82599940);  /* forward decl */
PPC_WEAK_FUNC(sub_82599940) { __imp__sub_82599940(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82599940) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x82599948;
	__savegprlr_27(ctx, base);
	// stwu r1,-176(r1)
	ea = -176 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// addi r27,r11,12544
	ctx.r27.s64 = ctx.r11.s64 + 12544;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// mr r31,r5
	ctx.r31.u64 = ctx.r5.u64;
	// bl 0x82273620
	ctx.lr = 0x82599968;
	sub_82273620(ctx, base);
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// bl 0x821d2088
	ctx.lr = 0x82599970;
	sub_821D2088(ctx, base);
	// lbz r11,133(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 133);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8259999c
	if (!ctx.cr6.eq) goto loc_8259999C;
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// lwz r11,1772(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 1772);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x8259999c
	if (ctx.cr6.eq) goto loc_8259999C;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r10,1768(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 1768);
	// cmpw cr6,r11,r10
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r10.s32, ctx.xer);
	// bne cr6,0x82599a3c
	if (!ctx.cr6.eq) goto loc_82599A3C;
loc_8259999C:
	// stb r31,87(r1)
	PPC_STORE_U8(ctx.r1.u32 + 87, ctx.r31.u8);
	// li r31,0
	ctx.r31.s64 = 0;
	// li r28,1
	ctx.r28.s64 = 1;
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r3,r11,23448
	ctx.r3.s64 = ctx.r11.s64 + 23448;
	// li r9,0
	ctx.r9.s64 = 0;
	// stb r31,119(r1)
	PPC_STORE_U8(ctx.r1.u32 + 119, ctx.r31.u8);
	// li r8,0
	ctx.r8.s64 = 0;
	// stb r28,127(r1)
	PPC_STORE_U8(ctx.r1.u32 + 127, ctx.r28.u8);
	// li r7,0
	ctx.r7.s64 = 0;
	// stb r28,111(r1)
	PPC_STORE_U8(ctx.r1.u32 + 111, ctx.r28.u8);
	// mr r6,r30
	ctx.r6.u64 = ctx.r30.u64;
	// stb r31,103(r1)
	PPC_STORE_U8(ctx.r1.u32 + 103, ctx.r31.u8);
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r31,95(r1)
	PPC_STORE_U8(ctx.r1.u32 + 95, ctx.r31.u8);
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// bl 0x8221f458
	ctx.lr = 0x825999E4;
	sub_8221F458(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// lbz r11,-22652(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -22652);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82599a30
	if (ctx.cr6.eq) goto loc_82599A30;
	// li r11,-1
	ctx.r11.s64 = -1;
	// lwz r4,352(r27)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r27.u32 + 352);
	// li r10,-1
	ctx.r10.s64 = -1;
	// stb r31,119(r1)
	PPC_STORE_U8(ctx.r1.u32 + 119, ctx.r31.u8);
	// li r9,-1
	ctx.r9.s64 = -1;
	// li r8,-1
	ctx.r8.s64 = -1;
	// li r7,-1
	ctx.r7.s64 = -1;
	// li r6,-1
	ctx.r6.s64 = -1;
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// li r5,0
	ctx.r5.s64 = 0;
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x8221f2f0
	ctx.lr = 0x82599A30;
	sub_8221F2F0(ctx, base);
loc_82599A30:
	// stb r28,-22652(r30)
	PPC_STORE_U8(ctx.r30.u32 + -22652, ctx.r28.u8);
	// addi r1,r1,176
	ctx.r1.s64 = ctx.r1.s64 + 176;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
loc_82599A3C:
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-22652(r10)
	PPC_STORE_U8(ctx.r10.u32 + -22652, ctx.r11.u8);
	// addi r1,r1,176
	ctx.r1.s64 = ctx.r1.s64 + 176;
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82599A50);  /* forward decl */
PPC_WEAK_FUNC(sub_82599A50) { __imp__sub_82599A50(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82599A50) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb10
	ctx.lr = 0x82599A58;
	__savegprlr_26(ctx, base);
	// stwu r1,-192(r1)
	ea = -192 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// addi r26,r11,12544
	ctx.r26.s64 = ctx.r11.s64 + 12544;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// mr r30,r5
	ctx.r30.u64 = ctx.r5.u64;
	// bl 0x82273620
	ctx.lr = 0x82599A78;
	sub_82273620(ctx, base);
	// mr r28,r3
	ctx.r28.u64 = ctx.r3.u64;
	// bl 0x821d2088
	ctx.lr = 0x82599A80;
	sub_821D2088(ctx, base);
	// lbz r11,133(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 133);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82599aac
	if (!ctx.cr6.eq) goto loc_82599AAC;
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// lwz r11,1772(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 1772);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x82599aac
	if (ctx.cr6.eq) goto loc_82599AAC;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r10,1768(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 1768);
	// cmpw cr6,r11,r10
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r10.s32, ctx.xer);
	// bne cr6,0x82599b5c
	if (!ctx.cr6.eq) goto loc_82599B5C;
loc_82599AAC:
	// li r31,0
	ctx.r31.s64 = 0;
	// li r27,1
	ctx.r27.s64 = 1;
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// li r10,0
	ctx.r10.s64 = 0;
	// addi r3,r11,23448
	ctx.r3.s64 = ctx.r11.s64 + 23448;
	// li r9,0
	ctx.r9.s64 = 0;
	// stb r31,119(r1)
	PPC_STORE_U8(ctx.r1.u32 + 119, ctx.r31.u8);
	// mr r8,r29
	ctx.r8.u64 = ctx.r29.u64;
	// stb r27,127(r1)
	PPC_STORE_U8(ctx.r1.u32 + 127, ctx.r27.u8);
	// li r7,1
	ctx.r7.s64 = 1;
	// stb r27,111(r1)
	PPC_STORE_U8(ctx.r1.u32 + 111, ctx.r27.u8);
	// mr r6,r30
	ctx.r6.u64 = ctx.r30.u64;
	// stb r31,103(r1)
	PPC_STORE_U8(ctx.r1.u32 + 103, ctx.r31.u8);
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r31,95(r1)
	PPC_STORE_U8(ctx.r1.u32 + 95, ctx.r31.u8);
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// stb r31,87(r1)
	PPC_STORE_U8(ctx.r1.u32 + 87, ctx.r31.u8);
	// bl 0x8221f458
	ctx.lr = 0x82599AF4;
	sub_8221F458(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// lbz r11,-22652(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -22652);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82599b50
	if (ctx.cr6.eq) goto loc_82599B50;
	// li r11,-1
	ctx.r11.s64 = -1;
	// lwz r4,352(r26)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r26.u32 + 352);
	// li r10,-1
	ctx.r10.s64 = -1;
	// stb r31,119(r1)
	PPC_STORE_U8(ctx.r1.u32 + 119, ctx.r31.u8);
	// li r9,-1
	ctx.r9.s64 = -1;
	// li r8,-1
	ctx.r8.s64 = -1;
	// mr r7,r29
	ctx.r7.u64 = ctx.r29.u64;
	// li r6,-1
	ctx.r6.s64 = -1;
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// li r5,0
	ctx.r5.s64 = 0;
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// mr r3,r28
	ctx.r3.u64 = ctx.r28.u64;
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x8221f2f0
	ctx.lr = 0x82599B40;
	sub_8221F2F0(ctx, base);
	// mr r11,r27
	ctx.r11.u64 = ctx.r27.u64;
	// stb r11,-22652(r30)
	PPC_STORE_U8(ctx.r30.u32 + -22652, ctx.r11.u8);
	// addi r1,r1,192
	ctx.r1.s64 = ctx.r1.s64 + 192;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
loc_82599B50:
	// stb r27,-22652(r30)
	PPC_STORE_U8(ctx.r30.u32 + -22652, ctx.r27.u8);
	// addi r1,r1,192
	ctx.r1.s64 = ctx.r1.s64 + 192;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
loc_82599B5C:
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-22652(r10)
	PPC_STORE_U8(ctx.r10.u32 + -22652, ctx.r11.u8);
	// addi r1,r1,192
	ctx.r1.s64 = ctx.r1.s64 + 192;
	// b 0x8298eb60
	__restgprlr_26(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82599B70);  /* forward decl */
PPC_WEAK_FUNC(sub_82599B70) { __imp__sub_82599B70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82599B70) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb08
	ctx.lr = 0x82599B78;
	__savegprlr_24(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// addi r31,r11,12544
	ctx.r31.s64 = ctx.r11.s64 + 12544;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r27,r5
	ctx.r27.u64 = ctx.r5.u64;
	// mr r26,r6
	ctx.r26.u64 = ctx.r6.u64;
	// bl 0x82273620
	ctx.lr = 0x82599B9C;
	sub_82273620(ctx, base);
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// lwz r25,352(r31)
	ctx.r25.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82273620
	ctx.lr = 0x82599BB0;
	sub_82273620(ctx, base);
	// lwz r24,352(r31)
	ctx.r24.u64 = PPC_LOAD_U32(ctx.r31.u32 + 352);
	// mr r28,r3
	ctx.r28.u64 = ctx.r3.u64;
	// bl 0x821d2088
	ctx.lr = 0x82599BBC;
	sub_821D2088(ctx, base);
	// lbz r11,133(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 133);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82599be8
	if (!ctx.cr6.eq) goto loc_82599BE8;
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// lwz r11,1772(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 1772);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// beq cr6,0x82599be8
	if (ctx.cr6.eq) goto loc_82599BE8;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r10,1768(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 1768);
	// cmpw cr6,r11,r10
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r10.s32, ctx.xer);
	// bne cr6,0x82599c8c
	if (!ctx.cr6.eq) goto loc_82599C8C;
loc_82599BE8:
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// stb r26,111(r1)
	PPC_STORE_U8(ctx.r1.u32 + 111, ctx.r26.u8);
	// li r31,0
	ctx.r31.s64 = 0;
	// addi r3,r11,23448
	ctx.r3.s64 = ctx.r11.s64 + 23448;
	// li r11,1
	ctx.r11.s64 = 1;
	// li r10,0
	ctx.r10.s64 = 0;
	// mr r9,r28
	ctx.r9.u64 = ctx.r28.u64;
	// li r8,0
	ctx.r8.s64 = 0;
	// stb r31,119(r1)
	PPC_STORE_U8(ctx.r1.u32 + 119, ctx.r31.u8);
	// li r7,0
	ctx.r7.s64 = 0;
	// stb r31,103(r1)
	PPC_STORE_U8(ctx.r1.u32 + 103, ctx.r31.u8);
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// stb r11,127(r1)
	PPC_STORE_U8(ctx.r1.u32 + 127, ctx.r11.u8);
	// li r5,0
	ctx.r5.s64 = 0;
	// stb r31,95(r1)
	PPC_STORE_U8(ctx.r1.u32 + 95, ctx.r31.u8);
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// stb r31,87(r1)
	PPC_STORE_U8(ctx.r1.u32 + 87, ctx.r31.u8);
	// bl 0x8221f458
	ctx.lr = 0x82599C30;
	sub_8221F458(ctx, base);
	// lis r30,-32072
	ctx.r30.s64 = -2101870592;
	// lbz r11,-22652(r30)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r30.u32 + -22652);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82599c7c
	if (ctx.cr6.eq) goto loc_82599C7C;
	// li r11,-1
	ctx.r11.s64 = -1;
	// stb r31,119(r1)
	PPC_STORE_U8(ctx.r1.u32 + 119, ctx.r31.u8);
	// li r10,-1
	ctx.r10.s64 = -1;
	// li r9,-1
	ctx.r9.s64 = -1;
	// li r8,-1
	ctx.r8.s64 = -1;
	// li r7,-1
	ctx.r7.s64 = -1;
	// mr r6,r24
	ctx.r6.u64 = ctx.r24.u64;
	// stw r11,108(r1)
	PPC_STORE_U32(ctx.r1.u32 + 108, ctx.r11.u32);
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// stw r11,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r11.u32);
	// mr r4,r25
	ctx.r4.u64 = ctx.r25.u64;
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x8221f2f0
	ctx.lr = 0x82599C7C;
	sub_8221F2F0(ctx, base);
loc_82599C7C:
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-22652(r30)
	PPC_STORE_U8(ctx.r30.u32 + -22652, ctx.r11.u8);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x8298eb58
	__restgprlr_24(ctx, base);
	return;
loc_82599C8C:
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,-22652(r10)
	PPC_STORE_U8(ctx.r10.u32 + -22652, ctx.r11.u8);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x8298eb58
	__restgprlr_24(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82599CA0);  /* forward decl */
PPC_WEAK_FUNC(sub_82599CA0) { __imp__sub_82599CA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82599CA0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x822d4d00
	ctx.lr = 0x82599CB8;
	sub_822D4D00(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82599cdc
	if (ctx.cr6.eq) goto loc_82599CDC;
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
loc_82599CDC:
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// addi r3,r11,12544
	ctx.r3.s64 = ctx.r11.s64 + 12544;
	// bl 0x82273620
	ctx.lr = 0x82599CEC;
	sub_82273620(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// addi r3,r11,23448
	ctx.r3.s64 = ctx.r11.s64 + 23448;
	// li r31,0
	ctx.r31.s64 = 0;
	// bl 0x8221e968
	ctx.lr = 0x82599D00;
	sub_8221E968(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82599d10
	if (ctx.cr6.eq) goto loc_82599D10;
	// li r31,1
	ctx.r31.s64 = 1;
loc_82599D10:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82599D28);  /* forward decl */
PPC_WEAK_FUNC(sub_82599D28) { __imp__sub_82599D28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82599D28) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-912(r1)
	ea = -912 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// lis r10,-32061
	ctx.r10.s64 = -2101149696;
	// mr r31,r4
	ctx.r31.u64 = ctx.r4.u64;
	// addi r3,r10,12544
	ctx.r3.s64 = ctx.r10.s64 + 12544;
	// mr r4,r11
	ctx.r4.u64 = ctx.r11.u64;
	// bl 0x82273620
	ctx.lr = 0x82599D50;
	sub_82273620(ctx, base);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// addi r10,r1,96
	ctx.r10.s64 = ctx.r1.s64 + 96;
	// li r9,-1
	ctx.r9.s64 = -1;
	// li r8,-1
	ctx.r8.s64 = -1;
	// li r7,-1
	ctx.r7.s64 = -1;
	// lfs f1,3068(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3068);
	ctx.f1.f64 = double(temp.f32);
	// li r11,-1
	ctx.r11.s64 = -1;
	// li r6,-1
	ctx.r6.s64 = -1;
	// stw r10,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r10.u32);
	// li r5,-1
	ctx.r5.s64 = -1;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x8221dea0
	ctx.lr = 0x82599D84;
	sub_8221DEA0(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r4,r1,96
	ctx.r4.s64 = ctx.r1.s64 + 96;
	// addi r3,r11,23448
	ctx.r3.s64 = ctx.r11.s64 + 23448;
	// li r31,0
	ctx.r31.s64 = 0;
	// bl 0x8221e968
	ctx.lr = 0x82599D98;
	sub_8221E968(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// li r3,1
	ctx.r3.s64 = 1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82599dac
	if (!ctx.cr6.eq) goto loc_82599DAC;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
loc_82599DAC:
	// addi r1,r1,912
	ctx.r1.s64 = ctx.r1.s64 + 912;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82599DC0);  /* forward decl */
PPC_WEAK_FUNC(sub_82599DC0) { __imp__sub_82599DC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82599DC0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x82599DC8;
	__savegprlr_29(ctx, base);
	// stwu r1,-912(r1)
	ea = -912 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// addi r31,r11,12544
	ctx.r31.s64 = ctx.r11.s64 + 12544;
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82273620
	ctx.lr = 0x82599DE4;
	sub_82273620(ctx, base);
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82273620
	ctx.lr = 0x82599DF4;
	sub_82273620(ctx, base);
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// li r5,400
	ctx.r5.s64 = 400;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8221dca0
	ctx.lr = 0x82599E08;
	sub_8221DCA0(ctx, base);
	// li r9,-1
	ctx.r9.s64 = -1;
	// li r8,-1
	ctx.r8.s64 = -1;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,0
	ctx.r6.s64 = 0;
	// li r5,0
	ctx.r5.s64 = 0;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8221eba0
	ctx.lr = 0x82599E28;
	sub_8221EBA0(ctx, base);
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// addi r3,r11,23448
	ctx.r3.s64 = ctx.r11.s64 + 23448;
	// li r31,0
	ctx.r31.s64 = 0;
	// bl 0x8221e968
	ctx.lr = 0x82599E3C;
	sub_8221E968(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// li r3,1
	ctx.r3.s64 = 1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82599e50
	if (!ctx.cr6.eq) goto loc_82599E50;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
loc_82599E50:
	// addi r1,r1,912
	ctx.r1.s64 = ctx.r1.s64 + 912;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82599E58);  /* forward decl */
PPC_WEAK_FUNC(sub_82599E58) { __imp__sub_82599E58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82599E58) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x821d2088
	ctx.lr = 0x82599E70;
	sub_821D2088(ctx, base);
	// lbz r11,133(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 133);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82599ee8
	if (ctx.cr6.eq) goto loc_82599EE8;
	// clrlwi r11,r31,24
	ctx.r11.u64 = ctx.r31.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82599eb4
	if (ctx.cr6.eq) goto loc_82599EB4;
	// bl 0x821d2088
	ctx.lr = 0x82599E8C;
	sub_821D2088(ctx, base);
	// lbz r11,134(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 134);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82599ee8
	if (!ctx.cr6.eq) goto loc_82599EE8;
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// lwz r10,1768(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 1768);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// stw r10,1768(r11)
	PPC_STORE_U32(ctx.r11.u32 + 1768, ctx.r10.u32);
	// bl 0x821d2088
	ctx.lr = 0x82599EAC;
	sub_821D2088(ctx, base);
	// li r11,1
	ctx.r11.s64 = 1;
	// b 0x82599ee4
	goto loc_82599EE4;
loc_82599EB4:
	// bl 0x821d2088
	ctx.lr = 0x82599EB8;
	sub_821D2088(ctx, base);
	// lbz r11,134(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 134);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82599ee8
	if (ctx.cr6.eq) goto loc_82599EE8;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r11,1768(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 1768);
	// cmpwi cr6,r11,0
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
	// ble cr6,0x82599edc
	if (!ctx.cr6.gt) goto loc_82599EDC;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// stw r11,1768(r10)
	PPC_STORE_U32(ctx.r10.u32 + 1768, ctx.r11.u32);
loc_82599EDC:
	// bl 0x821d2088
	ctx.lr = 0x82599EE0;
	sub_821D2088(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
loc_82599EE4:
	// stb r11,134(r3)
	PPC_STORE_U8(ctx.r3.u32 + 134, ctx.r11.u8);
loc_82599EE8:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_82599F00);  /* forward decl */
PPC_WEAK_FUNC(sub_82599F00) { __imp__sub_82599F00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82599F00) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb14
	ctx.lr = 0x82599F08;
	__savegprlr_27(ctx, base);
	// stfd f29,-72(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -72, ctx.f29.u64);
	// stfd f30,-64(r1)
	PPC_STORE_U64(ctx.r1.u32 + -64, ctx.f30.u64);
	// stfd f31,-56(r1)
	PPC_STORE_U64(ctx.r1.u32 + -56, ctx.f31.u64);
	// stwu r1,-176(r1)
	ea = -176 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// addi r4,r11,-23332
	ctx.r4.s64 = ctx.r11.s64 + -23332;
	// fmr f29,f3
	ctx.f29.f64 = ctx.f3.f64;
	// li r11,0
	ctx.r11.s64 = 0;
	// mr r30,r7
	ctx.r30.u64 = ctx.r7.u64;
	// mr r29,r8
	ctx.r29.u64 = ctx.r8.u64;
	// mr r28,r9
	ctx.r28.u64 = ctx.r9.u64;
	// mr r27,r10
	ctx.r27.u64 = ctx.r10.u64;
	// stb r11,88(r1)
	PPC_STORE_U8(ctx.r1.u32 + 88, ctx.r11.u8);
	// bl 0x8298f040
	ctx.lr = 0x82599F4C;
	sub_8298F040(ctx, base);
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// beq cr6,0x82599f74
	if (ctx.cr6.eq) goto loc_82599F74;
	// addi r10,r1,88
	ctx.r10.s64 = ctx.r1.s64 + 88;
	// mr r11,r31
	ctx.r11.u64 = ctx.r31.u64;
	// subf r9,r31,r10
	ctx.r9.u64 = ctx.r10.u64 - ctx.r31.u64;
loc_82599F60:
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r9,r11
	PPC_STORE_U8(ctx.r9.u32 + ctx.r11.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x82599f60
	if (!ctx.cr6.eq) goto loc_82599F60;
loc_82599F74:
	// mr r10,r27
	ctx.r10.u64 = ctx.r27.u64;
	// stfs f31,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// mr r9,r28
	ctx.r9.u64 = ctx.r28.u64;
	// stfs f30,84(r1)
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// mr r8,r29
	ctx.r8.u64 = ctx.r29.u64;
	// fmr f1,f29
	ctx.f1.f64 = ctx.f29.f64;
	// mr r7,r30
	ctx.r7.u64 = ctx.r30.u64;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r4,r1,88
	ctx.r4.s64 = ctx.r1.s64 + 88;
	// li r3,0
	ctx.r3.s64 = 0;
	// bl 0x82260778
	ctx.lr = 0x82599FA0;
	sub_82260778(ctx, base);
	// lwz r11,260(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 260);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
	// addi r1,r1,176
	ctx.r1.s64 = ctx.r1.s64 + 176;
	// lfd f29,-72(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f29.u64 = PPC_LOAD_U64(ctx.r1.u32 + -72);
	// lfd f30,-64(r1)
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -64);
	// lfd f31,-56(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -56);
	// b 0x8298eb64
	__restgprlr_27(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_82599FC0);  /* forward decl */
PPC_WEAK_FUNC(sub_82599FC0) { __imp__sub_82599FC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_82599FC0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x82599FC8;
	__savegprlr_29(ctx, base);
	// stwu r1,-864(r1)
	ea = -864 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// addi r11,r1,140
	ctx.r11.s64 = ctx.r1.s64 + 140;
	// li r5,12
	ctx.r5.s64 = 12;
	// li r30,0
	ctx.r30.s64 = 0;
	// li r31,-1
	ctx.r31.s64 = -1;
loc_82599FDC:
	// addi r5,r5,-1
	ctx.r5.s64 = ctx.r5.s64 + -1;
	// stb r30,-60(r11)
	PPC_STORE_U8(ctx.r11.u32 + -60, ctx.r30.u8);
	// stw r31,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r31.u32);
	// stw r31,4(r11)
	PPC_STORE_U32(ctx.r11.u32 + 4, ctx.r31.u32);
	// cmplwi cr6,r5,0
	ctx.cr6.compare<uint32_t>(ctx.r5.u32, 0, ctx.xer);
	// addi r11,r11,68
	ctx.r11.s64 = ctx.r11.s64 + 68;
	// bne cr6,0x82599fdc
	if (!ctx.cr6.eq) goto loc_82599FDC;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// addi r29,r11,-23332
	ctx.r29.s64 = ctx.r11.s64 + -23332;
	// mr r11,r6
	ctx.r11.u64 = ctx.r6.u64;
	// mr r5,r29
	ctx.r5.u64 = ctx.r29.u64;
loc_8259A008:
	// lbz r31,0(r11)
	ctx.r31.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// lbz r30,0(r5)
	ctx.r30.u64 = PPC_LOAD_U8(ctx.r5.u32 + 0);
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// subf r30,r30,r31
	ctx.r30.u64 = ctx.r31.u64 - ctx.r30.u64;
	// beq cr6,0x8259a02c
	if (ctx.cr6.eq) goto loc_8259A02C;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// addi r5,r5,1
	ctx.r5.s64 = ctx.r5.s64 + 1;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// beq cr6,0x8259a008
	if (ctx.cr6.eq) goto loc_8259A008;
loc_8259A02C:
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// beq cr6,0x8259a054
	if (ctx.cr6.eq) goto loc_8259A054;
	// addi r5,r1,148
	ctx.r5.s64 = ctx.r1.s64 + 148;
	// mr r11,r6
	ctx.r11.u64 = ctx.r6.u64;
	// subf r5,r6,r5
	ctx.r5.u64 = ctx.r5.u64 - ctx.r6.u64;
loc_8259A040:
	// lbz r6,0(r11)
	ctx.r6.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r6,0
	ctx.cr6.compare<uint32_t>(ctx.r6.u32, 0, ctx.xer);
	// stbx r6,r5,r11
	PPC_STORE_U8(ctx.r5.u32 + ctx.r11.u32, ctx.r6.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x8259a040
	if (!ctx.cr6.eq) goto loc_8259A040;
loc_8259A054:
	// mr r11,r7
	ctx.r11.u64 = ctx.r7.u64;
	// mr r6,r29
	ctx.r6.u64 = ctx.r29.u64;
loc_8259A05C:
	// lbz r5,0(r11)
	ctx.r5.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// lbz r31,0(r6)
	ctx.r31.u64 = PPC_LOAD_U8(ctx.r6.u32 + 0);
	// cmpwi cr6,r5,0
	ctx.cr6.compare<int32_t>(ctx.r5.s32, 0, ctx.xer);
	// subf r31,r31,r5
	ctx.r31.u64 = ctx.r5.u64 - ctx.r31.u64;
	// beq cr6,0x8259a080
	if (ctx.cr6.eq) goto loc_8259A080;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// addi r6,r6,1
	ctx.r6.s64 = ctx.r6.s64 + 1;
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// beq cr6,0x8259a05c
	if (ctx.cr6.eq) goto loc_8259A05C;
loc_8259A080:
	// cmpwi cr6,r31,0
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 0, ctx.xer);
	// beq cr6,0x8259a0a8
	if (ctx.cr6.eq) goto loc_8259A0A8;
	// addi r6,r1,216
	ctx.r6.s64 = ctx.r1.s64 + 216;
	// mr r11,r7
	ctx.r11.u64 = ctx.r7.u64;
	// subf r6,r7,r6
	ctx.r6.u64 = ctx.r6.u64 - ctx.r7.u64;
loc_8259A094:
	// lbz r7,0(r11)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r7,0
	ctx.cr6.compare<uint32_t>(ctx.r7.u32, 0, ctx.xer);
	// stbx r7,r6,r11
	PPC_STORE_U8(ctx.r6.u32 + ctx.r11.u32, ctx.r7.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x8259a094
	if (!ctx.cr6.eq) goto loc_8259A094;
loc_8259A0A8:
	// mr r11,r8
	ctx.r11.u64 = ctx.r8.u64;
	// mr r7,r29
	ctx.r7.u64 = ctx.r29.u64;
loc_8259A0B0:
	// lbz r6,0(r11)
	ctx.r6.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// lbz r5,0(r7)
	ctx.r5.u64 = PPC_LOAD_U8(ctx.r7.u32 + 0);
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// subf r5,r5,r6
	ctx.r5.u64 = ctx.r6.u64 - ctx.r5.u64;
	// beq cr6,0x8259a0d4
	if (ctx.cr6.eq) goto loc_8259A0D4;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// addi r7,r7,1
	ctx.r7.s64 = ctx.r7.s64 + 1;
	// cmpwi cr6,r5,0
	ctx.cr6.compare<int32_t>(ctx.r5.s32, 0, ctx.xer);
	// beq cr6,0x8259a0b0
	if (ctx.cr6.eq) goto loc_8259A0B0;
loc_8259A0D4:
	// cmpwi cr6,r5,0
	ctx.cr6.compare<int32_t>(ctx.r5.s32, 0, ctx.xer);
	// beq cr6,0x8259a0fc
	if (ctx.cr6.eq) goto loc_8259A0FC;
	// addi r7,r1,284
	ctx.r7.s64 = ctx.r1.s64 + 284;
	// mr r11,r8
	ctx.r11.u64 = ctx.r8.u64;
	// subf r7,r8,r7
	ctx.r7.u64 = ctx.r7.u64 - ctx.r8.u64;
loc_8259A0E8:
	// lbz r8,0(r11)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r8,0
	ctx.cr6.compare<uint32_t>(ctx.r8.u32, 0, ctx.xer);
	// stbx r8,r7,r11
	PPC_STORE_U8(ctx.r7.u32 + ctx.r11.u32, ctx.r8.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x8259a0e8
	if (!ctx.cr6.eq) goto loc_8259A0E8;
loc_8259A0FC:
	// mr r11,r9
	ctx.r11.u64 = ctx.r9.u64;
	// mr r8,r29
	ctx.r8.u64 = ctx.r29.u64;
loc_8259A104:
	// lbz r7,0(r11)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// lbz r6,0(r8)
	ctx.r6.u64 = PPC_LOAD_U8(ctx.r8.u32 + 0);
	// cmpwi cr6,r7,0
	ctx.cr6.compare<int32_t>(ctx.r7.s32, 0, ctx.xer);
	// subf r6,r6,r7
	ctx.r6.u64 = ctx.r7.u64 - ctx.r6.u64;
	// beq cr6,0x8259a128
	if (ctx.cr6.eq) goto loc_8259A128;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// addi r8,r8,1
	ctx.r8.s64 = ctx.r8.s64 + 1;
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x8259a104
	if (ctx.cr6.eq) goto loc_8259A104;
loc_8259A128:
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x8259a150
	if (ctx.cr6.eq) goto loc_8259A150;
	// addi r8,r1,352
	ctx.r8.s64 = ctx.r1.s64 + 352;
	// mr r11,r9
	ctx.r11.u64 = ctx.r9.u64;
	// subf r7,r9,r8
	ctx.r7.u64 = ctx.r8.u64 - ctx.r9.u64;
loc_8259A13C:
	// lbz r8,0(r11)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r8,0
	ctx.cr6.compare<uint32_t>(ctx.r8.u32, 0, ctx.xer);
	// stbx r8,r7,r11
	PPC_STORE_U8(ctx.r7.u32 + ctx.r11.u32, ctx.r8.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x8259a13c
	if (!ctx.cr6.eq) goto loc_8259A13C;
loc_8259A150:
	// mr r11,r10
	ctx.r11.u64 = ctx.r10.u64;
	// mr r8,r29
	ctx.r8.u64 = ctx.r29.u64;
loc_8259A158:
	// lbz r7,0(r11)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// lbz r9,0(r8)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r8.u32 + 0);
	// cmpwi cr6,r7,0
	ctx.cr6.compare<int32_t>(ctx.r7.s32, 0, ctx.xer);
	// subf r6,r9,r7
	ctx.r6.u64 = ctx.r7.u64 - ctx.r9.u64;
	// beq cr6,0x8259a17c
	if (ctx.cr6.eq) goto loc_8259A17C;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// addi r8,r8,1
	ctx.r8.s64 = ctx.r8.s64 + 1;
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x8259a158
	if (ctx.cr6.eq) goto loc_8259A158;
loc_8259A17C:
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x8259a1a4
	if (ctx.cr6.eq) goto loc_8259A1A4;
	// addi r9,r1,420
	ctx.r9.s64 = ctx.r1.s64 + 420;
	// mr r11,r10
	ctx.r11.u64 = ctx.r10.u64;
	// subf r8,r10,r9
	ctx.r8.u64 = ctx.r9.u64 - ctx.r10.u64;
loc_8259A190:
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r8,r11
	PPC_STORE_U8(ctx.r8.u32 + ctx.r11.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x8259a190
	if (!ctx.cr6.eq) goto loc_8259A190;
loc_8259A1A4:
	// lwz r11,948(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 948);
	// mr r8,r29
	ctx.r8.u64 = ctx.r29.u64;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_8259A1B0:
	// lbz r7,0(r10)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
	// lbz r9,0(r8)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r8.u32 + 0);
	// cmpwi cr6,r7,0
	ctx.cr6.compare<int32_t>(ctx.r7.s32, 0, ctx.xer);
	// subf r6,r9,r7
	ctx.r6.u64 = ctx.r7.u64 - ctx.r9.u64;
	// beq cr6,0x8259a1d4
	if (ctx.cr6.eq) goto loc_8259A1D4;
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r8,r8,1
	ctx.r8.s64 = ctx.r8.s64 + 1;
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x8259a1b0
	if (ctx.cr6.eq) goto loc_8259A1B0;
loc_8259A1D4:
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x8259a1f8
	if (ctx.cr6.eq) goto loc_8259A1F8;
	// addi r10,r1,488
	ctx.r10.s64 = ctx.r1.s64 + 488;
	// subf r8,r11,r10
	ctx.r8.u64 = ctx.r10.u64 - ctx.r11.u64;
loc_8259A1E4:
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r8,r11
	PPC_STORE_U8(ctx.r8.u32 + ctx.r11.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x8259a1e4
	if (!ctx.cr6.eq) goto loc_8259A1E4;
loc_8259A1F8:
	// lwz r11,956(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 956);
	// mr r8,r29
	ctx.r8.u64 = ctx.r29.u64;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_8259A204:
	// lbz r7,0(r10)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
	// lbz r9,0(r8)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r8.u32 + 0);
	// cmpwi cr6,r7,0
	ctx.cr6.compare<int32_t>(ctx.r7.s32, 0, ctx.xer);
	// subf r6,r9,r7
	ctx.r6.u64 = ctx.r7.u64 - ctx.r9.u64;
	// beq cr6,0x8259a228
	if (ctx.cr6.eq) goto loc_8259A228;
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r8,r8,1
	ctx.r8.s64 = ctx.r8.s64 + 1;
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x8259a204
	if (ctx.cr6.eq) goto loc_8259A204;
loc_8259A228:
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x8259a24c
	if (ctx.cr6.eq) goto loc_8259A24C;
	// addi r10,r1,556
	ctx.r10.s64 = ctx.r1.s64 + 556;
	// subf r8,r11,r10
	ctx.r8.u64 = ctx.r10.u64 - ctx.r11.u64;
loc_8259A238:
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r8,r11
	PPC_STORE_U8(ctx.r8.u32 + ctx.r11.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x8259a238
	if (!ctx.cr6.eq) goto loc_8259A238;
loc_8259A24C:
	// lwz r11,964(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 964);
	// mr r8,r29
	ctx.r8.u64 = ctx.r29.u64;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_8259A258:
	// lbz r7,0(r10)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
	// lbz r9,0(r8)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r8.u32 + 0);
	// cmpwi cr6,r7,0
	ctx.cr6.compare<int32_t>(ctx.r7.s32, 0, ctx.xer);
	// subf r6,r9,r7
	ctx.r6.u64 = ctx.r7.u64 - ctx.r9.u64;
	// beq cr6,0x8259a27c
	if (ctx.cr6.eq) goto loc_8259A27C;
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r8,r8,1
	ctx.r8.s64 = ctx.r8.s64 + 1;
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x8259a258
	if (ctx.cr6.eq) goto loc_8259A258;
loc_8259A27C:
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x8259a2a0
	if (ctx.cr6.eq) goto loc_8259A2A0;
	// addi r10,r1,624
	ctx.r10.s64 = ctx.r1.s64 + 624;
	// subf r8,r11,r10
	ctx.r8.u64 = ctx.r10.u64 - ctx.r11.u64;
loc_8259A28C:
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r8,r11
	PPC_STORE_U8(ctx.r8.u32 + ctx.r11.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x8259a28c
	if (!ctx.cr6.eq) goto loc_8259A28C;
loc_8259A2A0:
	// lwz r11,972(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 972);
	// mr r8,r29
	ctx.r8.u64 = ctx.r29.u64;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_8259A2AC:
	// lbz r7,0(r10)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
	// lbz r9,0(r8)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r8.u32 + 0);
	// cmpwi cr6,r7,0
	ctx.cr6.compare<int32_t>(ctx.r7.s32, 0, ctx.xer);
	// subf r6,r9,r7
	ctx.r6.u64 = ctx.r7.u64 - ctx.r9.u64;
	// beq cr6,0x8259a2d0
	if (ctx.cr6.eq) goto loc_8259A2D0;
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r8,r8,1
	ctx.r8.s64 = ctx.r8.s64 + 1;
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x8259a2ac
	if (ctx.cr6.eq) goto loc_8259A2AC;
loc_8259A2D0:
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x8259a2f4
	if (ctx.cr6.eq) goto loc_8259A2F4;
	// addi r10,r1,692
	ctx.r10.s64 = ctx.r1.s64 + 692;
	// subf r8,r11,r10
	ctx.r8.u64 = ctx.r10.u64 - ctx.r11.u64;
loc_8259A2E0:
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r8,r11
	PPC_STORE_U8(ctx.r8.u32 + ctx.r11.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x8259a2e0
	if (!ctx.cr6.eq) goto loc_8259A2E0;
loc_8259A2F4:
	// lwz r11,980(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 980);
	// mr r8,r29
	ctx.r8.u64 = ctx.r29.u64;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_8259A300:
	// lbz r7,0(r10)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
	// lbz r9,0(r8)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r8.u32 + 0);
	// cmpwi cr6,r7,0
	ctx.cr6.compare<int32_t>(ctx.r7.s32, 0, ctx.xer);
	// subf r6,r9,r7
	ctx.r6.u64 = ctx.r7.u64 - ctx.r9.u64;
	// beq cr6,0x8259a324
	if (ctx.cr6.eq) goto loc_8259A324;
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r8,r8,1
	ctx.r8.s64 = ctx.r8.s64 + 1;
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x8259a300
	if (ctx.cr6.eq) goto loc_8259A300;
loc_8259A324:
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x8259a348
	if (ctx.cr6.eq) goto loc_8259A348;
	// addi r10,r1,760
	ctx.r10.s64 = ctx.r1.s64 + 760;
	// subf r8,r11,r10
	ctx.r8.u64 = ctx.r10.u64 - ctx.r11.u64;
loc_8259A334:
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r8,r11
	PPC_STORE_U8(ctx.r8.u32 + ctx.r11.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x8259a334
	if (!ctx.cr6.eq) goto loc_8259A334;
loc_8259A348:
	// lwz r11,988(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 988);
	// mr r8,r29
	ctx.r8.u64 = ctx.r29.u64;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_8259A354:
	// lbz r7,0(r10)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
	// lbz r9,0(r8)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r8.u32 + 0);
	// cmpwi cr6,r7,0
	ctx.cr6.compare<int32_t>(ctx.r7.s32, 0, ctx.xer);
	// subf r6,r9,r7
	ctx.r6.u64 = ctx.r7.u64 - ctx.r9.u64;
	// beq cr6,0x8259a378
	if (ctx.cr6.eq) goto loc_8259A378;
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r8,r8,1
	ctx.r8.s64 = ctx.r8.s64 + 1;
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x8259a354
	if (ctx.cr6.eq) goto loc_8259A354;
loc_8259A378:
	// cmpwi cr6,r6,0
	ctx.cr6.compare<int32_t>(ctx.r6.s32, 0, ctx.xer);
	// beq cr6,0x8259a39c
	if (ctx.cr6.eq) goto loc_8259A39C;
	// addi r10,r1,828
	ctx.r10.s64 = ctx.r1.s64 + 828;
	// subf r8,r11,r10
	ctx.r8.u64 = ctx.r10.u64 - ctx.r11.u64;
loc_8259A388:
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r8,r11
	PPC_STORE_U8(ctx.r8.u32 + ctx.r11.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x8259a388
	if (!ctx.cr6.eq) goto loc_8259A388;
loc_8259A39C:
	// lwz r11,996(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 996);
	// mr r9,r29
	ctx.r9.u64 = ctx.r29.u64;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
loc_8259A3A8:
	// lbz r8,0(r10)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
	// lbz r7,0(r9)
	ctx.r7.u64 = PPC_LOAD_U8(ctx.r9.u32 + 0);
	// cmpwi cr6,r8,0
	ctx.cr6.compare<int32_t>(ctx.r8.s32, 0, ctx.xer);
	// subf r7,r7,r8
	ctx.r7.u64 = ctx.r8.u64 - ctx.r7.u64;
	// beq cr6,0x8259a3cc
	if (ctx.cr6.eq) goto loc_8259A3CC;
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r9,r9,1
	ctx.r9.s64 = ctx.r9.s64 + 1;
	// cmpwi cr6,r7,0
	ctx.cr6.compare<int32_t>(ctx.r7.s32, 0, ctx.xer);
	// beq cr6,0x8259a3a8
	if (ctx.cr6.eq) goto loc_8259A3A8;
loc_8259A3CC:
	// cmpwi cr6,r7,0
	ctx.cr6.compare<int32_t>(ctx.r7.s32, 0, ctx.xer);
	// beq cr6,0x8259a3f0
	if (ctx.cr6.eq) goto loc_8259A3F0;
	// addi r10,r1,896
	ctx.r10.s64 = ctx.r1.s64 + 896;
	// subf r9,r11,r10
	ctx.r9.u64 = ctx.r10.u64 - ctx.r11.u64;
loc_8259A3DC:
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r9,r11
	PPC_STORE_U8(ctx.r9.u32 + ctx.r11.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x8259a3dc
	if (!ctx.cr6.eq) goto loc_8259A3DC;
loc_8259A3F0:
	// li r7,12
	ctx.r7.s64 = 12;
	// addi r6,r1,80
	ctx.r6.s64 = ctx.r1.s64 + 80;
	// li r5,0
	ctx.r5.s64 = 0;
	// bl 0x8225e978
	ctx.lr = 0x8259A400;
	sub_8225E978(ctx, base);
	// addi r1,r1,864
	ctx.r1.s64 = ctx.r1.s64 + 864;
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259A408);  /* forward decl */
PPC_WEAK_FUNC(sub_8259A408) { __imp__sub_8259A408(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259A408) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r4,-32072
	ctx.r4.s64 = -2101870592;
	// lwz r11,-17172(r4)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r4.u32 + -17172);
	// cmpwi cr6,r11,1500
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 1500, ctx.xer);
	// bge cr6,0x8259a4c8
	if (!ctx.cr6.lt) goto loc_8259A4C8;
	// lis r10,-32052
	ctx.r10.s64 = -2100559872;
	// lis r8,-32093
	ctx.r8.s64 = -2103246848;
	// addi r6,r10,-14720
	ctx.r6.s64 = ctx.r10.s64 + -14720;
	// rlwinm r10,r11,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lis r9,-32093
	ctx.r9.s64 = -2103246848;
	// add r7,r10,r6
	ctx.r7.u64 = ctx.r10.u64 + ctx.r6.u64;
	// lis r10,-32093
	ctx.r10.s64 = -2103246848;
	// lwz r8,-7520(r8)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r8.u32 + -7520);
	// lwz r9,-7512(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + -7512);
	// lwz r5,-7528(r10)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7528);
loc_8259A450:
	// lwz r10,0(r7)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r7.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x8259a498
	if (ctx.cr6.eq) goto loc_8259A498;
	// lbz r31,8(r10)
	ctx.r31.u64 = PPC_LOAD_U8(ctx.r10.u32 + 8);
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// bne cr6,0x8259a470
	if (!ctx.cr6.eq) goto loc_8259A470;
	// rlwinm r10,r5,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r10,r10,r6
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r6.u32);
loc_8259A470:
	// lwz r10,92(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 92);
	// lwz r10,4(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 4);
	// cmpw cr6,r10,r3
	ctx.cr6.compare<int32_t>(ctx.r10.s32, ctx.r3.s32, ctx.xer);
	// bne cr6,0x8259a498
	if (!ctx.cr6.eq) goto loc_8259A498;
	// cmpw cr6,r11,r5
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r5.s32, ctx.xer);
	// beq cr6,0x8259a498
	if (ctx.cr6.eq) goto loc_8259A498;
	// cmpw cr6,r11,r9
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r9.s32, ctx.xer);
	// beq cr6,0x8259a498
	if (ctx.cr6.eq) goto loc_8259A498;
	// cmpw cr6,r11,r8
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x8259a4b0
	if (!ctx.cr6.eq) goto loc_8259A4B0;
loc_8259A498:
	// addi r7,r7,4
	ctx.r7.s64 = ctx.r7.s64 + 4;
	// addi r10,r6,6000
	ctx.r10.s64 = ctx.r6.s64 + 6000;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// cmpw cr6,r7,r10
	ctx.cr6.compare<int32_t>(ctx.r7.s32, ctx.r10.s32, ctx.xer);
	// blt cr6,0x8259a450
	if (ctx.cr6.lt) goto loc_8259A450;
	// b 0x8259a4c8
	goto loc_8259A4C8;
loc_8259A4B0:
	// mr r3,r11
	ctx.r3.u64 = ctx.r11.u64;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// stw r11,-17172(r4)
	PPC_STORE_U32(ctx.r4.u32 + -17172, ctx.r11.u32);
	// bl 0x822c5f58
	ctx.lr = 0x8259A4C0;
	sub_822C5F58(ctx, base);
	// cmpwi cr6,r3,-1
	ctx.cr6.compare<int32_t>(ctx.r3.s32, -1, ctx.xer);
	// bne cr6,0x8259a4cc
	if (!ctx.cr6.eq) goto loc_8259A4CC;
loc_8259A4C8:
	// li r3,0
	ctx.r3.s64 = 0;
loc_8259A4CC:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259A4E0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259A4E0) { __imp__sub_8259A4E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259A4E0) {
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
	// mr r31,r4
	ctx.r31.u64 = ctx.r4.u64;
	// bl 0x822c5f18
	ctx.lr = 0x8259A4F8;
	sub_822C5F18(ctx, base);
	// cmpwi cr6,r3,-1
	ctx.cr6.compare<int32_t>(ctx.r3.s32, -1, ctx.xer);
	// beq cr6,0x8259a544
	if (ctx.cr6.eq) goto loc_8259A544;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// rlwinm r10,r3,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r11,r11,-14720
	ctx.r11.s64 = ctx.r11.s64 + -14720;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lbz r10,8(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 8);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x8259a528
	if (ctx.cr6.eq) goto loc_8259A528;
	// li r10,48
	ctx.r10.s64 = 48;
	// lvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// b 0x8259a560
	goto loc_8259A560;
loc_8259A528:
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lfs f0,28(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// lfs f0,3068(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 3068);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// lfs f0,32(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 32);
	ctx.f0.f64 = double(temp.f32);
	// b 0x8259a554
	goto loc_8259A554;
loc_8259A544:
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,3068(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 3068);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// stfs f0,88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
loc_8259A554:
	// stfs f0,84(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
loc_8259A560:
	// stvx128 v0,r0,r31
	ea = (ctx.r31.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
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

PPC_FUNC_IMPL(__imp__sub_8259A578);  /* forward decl */
PPC_WEAK_FUNC(sub_8259A578) { __imp__sub_8259A578(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259A578) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// bl 0x822c5f18
	ctx.lr = 0x8259A588;
	sub_822C5F18(ctx, base);
	// cmpwi cr6,r3,-1
	ctx.cr6.compare<int32_t>(ctx.r3.s32, -1, ctx.xer);
	// beq cr6,0x8259a5e4
	if (ctx.cr6.eq) goto loc_8259A5E4;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// rlwinm r10,r3,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r11,r11,-14720
	ctx.r11.s64 = ctx.r11.s64 + -14720;
	// lwzx r10,r10,r11
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lbz r9,8(r10)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r10.u32 + 8);
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// beq cr6,0x8259a5c0
	if (ctx.cr6.eq) goto loc_8259A5C0;
	// lwz r3,76(r10)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r10.u32 + 76);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_8259A5C0:
	// lis r10,-32093
	ctx.r10.s64 = -2103246848;
	// lwz r10,-7528(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7528);
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lwz r3,76(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 76);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_8259A5E4:
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

PPC_FUNC_IMPL(__imp__sub_8259A5F8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259A5F8) { __imp__sub_8259A5F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259A5F8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// bl 0x822c5f18
	ctx.lr = 0x8259A608;
	sub_822C5F18(ctx, base);
	// cmpwi cr6,r3,-1
	ctx.cr6.compare<int32_t>(ctx.r3.s32, -1, ctx.xer);
	// beq cr6,0x8259a664
	if (ctx.cr6.eq) goto loc_8259A664;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// rlwinm r10,r3,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r11,r11,-14720
	ctx.r11.s64 = ctx.r11.s64 + -14720;
	// lwzx r10,r10,r11
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lbz r9,8(r10)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r10.u32 + 8);
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// beq cr6,0x8259a640
	if (ctx.cr6.eq) goto loc_8259A640;
	// lwz r3,72(r10)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r10.u32 + 72);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_8259A640:
	// lis r10,-32093
	ctx.r10.s64 = -2103246848;
	// lwz r10,-7528(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7528);
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lwz r3,72(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 72);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_8259A664:
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

PPC_FUNC_IMPL(__imp__sub_8259A678);  /* forward decl */
PPC_WEAK_FUNC(sub_8259A678) { __imp__sub_8259A678(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259A678) {
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
	// mr r31,r4
	ctx.r31.u64 = ctx.r4.u64;
	// bl 0x822790e0
	ctx.lr = 0x8259A694;
	sub_822790E0(ctx, base);
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,4
	ctx.r6.s64 = 4;
	// li r4,6
	ctx.r4.s64 = 6;
	// li r3,1
	ctx.r3.s64 = 1;
	// mr r5,r30
	ctx.r5.u64 = ctx.r30.u64;
	// bl 0x822cc6e0
	ctx.lr = 0x8259A6B0;
	sub_822CC6E0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r3,0
	ctx.r3.s64 = 0;
	// stw r4,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r4.u32);
	// lfs f1,2592(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2592);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x822c75e0
	ctx.lr = 0x8259A6C8;
	sub_822C75E0(ctx, base);
	// li r5,2
	ctx.r5.s64 = 2;
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// li r3,5
	ctx.r3.s64 = 5;
	// bl 0x822cc888
	ctx.lr = 0x8259A6D8;
	sub_822CC888(ctx, base);
	// rlwinm r10,r30,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// add r10,r30,r10
	ctx.r10.u64 = ctx.r30.u64 + ctx.r10.u64;
	// addi r11,r11,19504
	ctx.r11.s64 = ctx.r11.s64 + 19504;
	// rlwinm r10,r10,4,0,27
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 4) & 0xFFFFFFF0;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// add r11,r10,r11
	ctx.r11.u64 = ctx.r10.u64 + ctx.r11.u64;
	// li r3,2
	ctx.r3.s64 = 2;
	// lfs f0,28(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// lfs f0,32(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 32);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,84(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// lfs f0,36(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 36);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// bl 0x822c7658
	ctx.lr = 0x8259A718;
	sub_822C7658(ctx, base);
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x822c7480
	ctx.lr = 0x8259A720;
	sub_822C7480(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8259a734
	if (!ctx.cr6.eq) goto loc_8259A734;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// b 0x8259a75c
	goto loc_8259A75C;
loc_8259A734:
	// lis r11,-32039
	ctx.r11.s64 = -2099707904;
	// lwz r30,15072(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 15072);
	// bl 0x821d2088
	ctx.lr = 0x8259A740;
	sub_821D2088(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// li r5,18
	ctx.r5.s64 = 18;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r8,0
	ctx.r8.s64 = 0;
	// bl 0x823093f8
	ctx.lr = 0x8259A75C;
	sub_823093F8(ctx, base);
loc_8259A75C:
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

PPC_FUNC_IMPL(__imp__sub_8259A778);  /* forward decl */
PPC_WEAK_FUNC(sub_8259A778) { __imp__sub_8259A778(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259A778) {
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
	// bl 0x822790e0
	ctx.lr = 0x8259A790;
	sub_822790E0(ctx, base);
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
	// addi r31,r11,19504
	ctx.r31.s64 = ctx.r11.s64 + 19504;
	// rlwinm r11,r5,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r10,r31,28
	ctx.r10.s64 = ctx.r31.s64 + 28;
	// add r11,r5,r11
	ctx.r11.u64 = ctx.r5.u64 + ctx.r11.u64;
	// addi r9,r31,32
	ctx.r9.s64 = ctx.r31.s64 + 32;
	// rlwinm r30,r11,4,0,27
	ctx.r30.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 4) & 0xFFFFFFF0;
	// addi r8,r31,36
	ctx.r8.s64 = ctx.r31.s64 + 36;
	// li r4,0
	ctx.r4.s64 = 0;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// lfsx f0,r30,r10
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r30.u32 + ctx.r10.u32);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// lfsx f0,r30,r9
	temp.u32 = PPC_LOAD_U32(ctx.r30.u32 + ctx.r9.u32);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,84(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// lfsx f0,r30,r8
	temp.u32 = PPC_LOAD_U32(ctx.r30.u32 + ctx.r8.u32);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// bl 0x822cc738
	ctx.lr = 0x8259A7D8;
	sub_822CC738(ctx, base);
	// addi r11,r31,20
	ctx.r11.s64 = ctx.r31.s64 + 20;
	// stwx r3,r30,r11
	PPC_STORE_U32(ctx.r30.u32 + ctx.r11.u32, ctx.r3.u32);
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

PPC_FUNC_IMPL(__imp__sub_8259A7F8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259A7F8) { __imp__sub_8259A7F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259A7F8) {
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
	// mr r31,r4
	ctx.r31.u64 = ctx.r4.u64;
	// bl 0x822c5f18
	ctx.lr = 0x8259A810;
	sub_822C5F18(ctx, base);
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// blt cr6,0x8259a868
	if (ctx.cr6.lt) goto loc_8259A868;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// rlwinm r10,r3,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r11,r11,-14720
	ctx.r11.s64 = ctx.r11.s64 + -14720;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lbz r10,8(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 8);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x8259a840
	if (ctx.cr6.eq) goto loc_8259A840;
	// li r10,48
	ctx.r10.s64 = 48;
	// lvx128 v0,r11,r10
	ea = (ctx.r11.u32 + ctx.r10.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// b 0x8259a86c
	goto loc_8259A86C;
loc_8259A840:
	// lis r10,-32256
	ctx.r10.s64 = -2113929216;
	// lfs f0,28(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,80(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// lfs f0,3068(r10)
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 3068);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,88(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// lfs f0,32(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 32);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,84(r1)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// lvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)ctx.v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// b 0x8259a86c
	goto loc_8259A86C;
loc_8259A868:
	// vspltisw v0,0
	simde_mm_store_si128((simde__m128i*)ctx.v0.u32, simde_mm_set1_epi32(int(0x0)));
loc_8259A86C:
	// stvx128 v0,r0,r31
	ea = (ctx.r31.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
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

PPC_FUNC_IMPL(__imp__sub_8259A888);  /* forward decl */
PPC_WEAK_FUNC(sub_8259A888) { __imp__sub_8259A888(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259A888) {
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
	// stfd f30,-40(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -40, ctx.f30.u64);
	// stfd f31,-32(r1)
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f31.u64);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// mr r31,r6
	ctx.r31.u64 = ctx.r6.u64;
	// lfs f0,12120(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12120);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f3,f0
	ctx.cr6.compare(ctx.f3.f64, ctx.f0.f64);
	// bgt cr6,0x8259a8cc
	if (ctx.cr6.gt) goto loc_8259A8CC;
	// li r5,4
	ctx.r5.s64 = 4;
	// bl 0x8231fbb0
	ctx.lr = 0x8259A8C8;
	sub_8231FBB0(ctx, base);
	// fmr f3,f1
	ctx.fpscr.disableFlushMode();
	ctx.f3.f64 = ctx.f1.f64;
loc_8259A8CC:
	// li r7,0
	ctx.r7.s64 = 0;
	// stfs f31,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// li r6,4
	ctx.r6.s64 = 4;
	// stfs f30,84(r1)
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// stfs f3,88(r1)
	temp.f32 = float(ctx.f3.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// li r4,4
	ctx.r4.s64 = 4;
	// li r3,1
	ctx.r3.s64 = 1;
	// bl 0x822cc720
	ctx.lr = 0x8259A8F0;
	sub_822CC720(ctx, base);
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// bl 0x822c7480
	ctx.lr = 0x8259A8F8;
	sub_822C7480(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8259a90c
	if (!ctx.cr6.eq) goto loc_8259A90C;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// b 0x8259a934
	goto loc_8259A934;
loc_8259A90C:
	// lis r11,-32039
	ctx.r11.s64 = -2099707904;
	// lwz r30,15072(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 15072);
	// bl 0x821d2088
	ctx.lr = 0x8259A918;
	sub_821D2088(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// li r5,18
	ctx.r5.s64 = 18;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r8,0
	ctx.r8.s64 = 0;
	// bl 0x823093f8
	ctx.lr = 0x8259A934;
	sub_823093F8(ctx, base);
loc_8259A934:
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f30,-40(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -40);
	// lfd f31,-32(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259A958);  /* forward decl */
PPC_WEAK_FUNC(sub_8259A958) { __imp__sub_8259A958(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259A958) {
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
	// stfd f30,-40(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -40, ctx.f30.u64);
	// stfd f31,-32(r1)
	PPC_STORE_U64(ctx.r1.u32 + -32, ctx.f31.u64);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// fmr f31,f1
	ctx.f31.f64 = ctx.f1.f64;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// mr r31,r6
	ctx.r31.u64 = ctx.r6.u64;
	// lfs f0,12120(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12120);
	ctx.f0.f64 = double(temp.f32);
	// fcmpu cr6,f3,f0
	ctx.cr6.compare(ctx.f3.f64, ctx.f0.f64);
	// bgt cr6,0x8259a99c
	if (ctx.cr6.gt) goto loc_8259A99C;
	// li r5,4
	ctx.r5.s64 = 4;
	// bl 0x8231fbb0
	ctx.lr = 0x8259A998;
	sub_8231FBB0(ctx, base);
	// fmr f3,f1
	ctx.fpscr.disableFlushMode();
	ctx.f3.f64 = ctx.f1.f64;
loc_8259A99C:
	// li r7,0
	ctx.r7.s64 = 0;
	// stfs f31,80(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f31.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// li r6,4
	ctx.r6.s64 = 4;
	// stfs f30,84(r1)
	temp.f32 = float(ctx.f30.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// stfs f3,88(r1)
	temp.f32 = float(ctx.f3.f64);
	PPC_STORE_U32(ctx.r1.u32 + 88, temp.u32);
	// li r4,5
	ctx.r4.s64 = 5;
	// li r3,1
	ctx.r3.s64 = 1;
	// bl 0x822cc720
	ctx.lr = 0x8259A9C0;
	sub_822CC720(ctx, base);
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// bl 0x822c7480
	ctx.lr = 0x8259A9C8;
	sub_822C7480(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8259a9dc
	if (!ctx.cr6.eq) goto loc_8259A9DC;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// b 0x8259aa04
	goto loc_8259AA04;
loc_8259A9DC:
	// lis r11,-32039
	ctx.r11.s64 = -2099707904;
	// lwz r30,15072(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 15072);
	// bl 0x821d2088
	ctx.lr = 0x8259A9E8;
	sub_821D2088(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// li r5,18
	ctx.r5.s64 = 18;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r8,0
	ctx.r8.s64 = 0;
	// bl 0x823093f8
	ctx.lr = 0x8259AA04;
	sub_823093F8(ctx, base);
loc_8259AA04:
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// lfd f30,-40(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -40);
	// lfd f31,-32(r1)
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -32);
	// ld r30,-24(r1)
	ctx.r30.u64 = PPC_LOAD_U64(ctx.r1.u32 + -24);
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259AA28);  /* forward decl */
PPC_WEAK_FUNC(sub_8259AA28) { __imp__sub_8259AA28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259AA28) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// bl 0x822c5f18
	ctx.lr = 0x8259AA38;
	sub_822C5F18(ctx, base);
	// mr r10,r3
	ctx.r10.u64 = ctx.r3.u64;
	// li r3,-1
	ctx.r3.s64 = -1;
	// cmpwi cr6,r10,0
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 0, ctx.xer);
	// blt cr6,0x8259aa94
	if (ctx.cr6.lt) goto loc_8259AA94;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r11,r11,-14720
	ctx.r11.s64 = ctx.r11.s64 + -14720;
	// lwzx r10,r10,r11
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lbz r9,8(r10)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r10.u32 + 8);
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// beq cr6,0x8259aa7c
	if (ctx.cr6.eq) goto loc_8259AA7C;
	// lwz r11,92(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 92);
	// lwz r3,4(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_8259AA7C:
	// lis r10,-32093
	ctx.r10.s64 = -2103246848;
	// lwz r10,-7528(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7528);
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lwz r11,92(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 92);
	// lwz r3,4(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
loc_8259AA94:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259AAA8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259AAA8) { __imp__sub_8259AAA8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259AAA8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// bl 0x822c5f18
	ctx.lr = 0x8259AABC;
	sub_822C5F18(ctx, base);
	// lis r11,-32005
	ctx.r11.s64 = -2097479680;
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// addi r31,r11,-9288
	ctx.r31.s64 = ctx.r11.s64 + -9288;
	// li r11,0
	ctx.r11.s64 = 0;
	// stb r11,0(r31)
	PPC_STORE_U8(ctx.r31.u32 + 0, ctx.r11.u8);
	// blt cr6,0x8259aafc
	if (ctx.cr6.lt) goto loc_8259AAFC;
	// bl 0x822c65e8
	ctx.lr = 0x8259AAD8;
	sub_822C65E8(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// bl 0x82272858
	ctx.lr = 0x8259AAE0;
	sub_82272858(ctx, base);
	// mr r11,r31
	ctx.r11.u64 = ctx.r31.u64;
loc_8259AAE4:
	// lbz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r3.u32 + 0);
	// addi r3,r3,1
	ctx.r3.s64 = ctx.r3.s64 + 1;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stb r10,0(r11)
	PPC_STORE_U8(ctx.r11.u32 + 0, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x8259aae4
	if (!ctx.cr6.eq) goto loc_8259AAE4;
loc_8259AAFC:
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259AB18);  /* forward decl */
PPC_WEAK_FUNC(sub_8259AB18) { __imp__sub_8259AB18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259AB18) {
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
	// mr r31,r4
	ctx.r31.u64 = ctx.r4.u64;
	// bl 0x822c5f18
	ctx.lr = 0x8259AB30;
	sub_822C5F18(ctx, base);
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// blt cr6,0x8259abc0
	if (ctx.cr6.lt) goto loc_8259ABC0;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// lwz r9,0(r31)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// rlwinm r10,r3,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r11,r11,-14720
	ctx.r11.s64 = ctx.r11.s64 + -14720;
	// stw r9,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r9.u32);
	// lis r9,-32093
	ctx.r9.s64 = -2103246848;
	// lwzx r10,r10,r11
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lwz r9,-7528(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + -7528);
	// lbz r8,8(r10)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r10.u32 + 8);
	// cmplwi cr6,r8,0
	ctx.cr6.compare<uint32_t>(ctx.r8.u32, 0, ctx.xer);
	// beq cr6,0x8259ab6c
	if (ctx.cr6.eq) goto loc_8259AB6C;
	// lhz r7,28(r10)
	ctx.r7.u64 = PPC_LOAD_U16(ctx.r10.u32 + 28);
	// b 0x8259ab78
	goto loc_8259AB78;
loc_8259AB6C:
	// rlwinm r7,r9,2,0,29
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r7,r7,r11
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r7.u32 + ctx.r11.u32);
	// lhz r7,28(r7)
	ctx.r7.u64 = PPC_LOAD_U16(ctx.r7.u32 + 28);
loc_8259AB78:
	// rlwinm r7,r7,31,1,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 31) & 0x7FFFFFFF;
	// cmplwi cr6,r8,0
	ctx.cr6.compare<uint32_t>(ctx.r8.u32, 0, ctx.xer);
	// clrlwi r5,r7,31
	ctx.r5.u64 = ctx.r7.u32 & 0x1;
	// beq cr6,0x8259aba0
	if (ctx.cr6.eq) goto loc_8259ABA0;
	// addi r6,r1,80
	ctx.r6.s64 = ctx.r1.s64 + 80;
	// lwz r4,84(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 84);
	// addi r3,r1,84
	ctx.r3.s64 = ctx.r1.s64 + 84;
	// bl 0x822ca328
	ctx.lr = 0x8259AB98;
	sub_822CA328(ctx, base);
	// lwz r11,80(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// b 0x8259abc4
	goto loc_8259ABC4;
loc_8259ABA0:
	// rlwinm r10,r9,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r6,r1,80
	ctx.r6.s64 = ctx.r1.s64 + 80;
	// addi r3,r1,84
	ctx.r3.s64 = ctx.r1.s64 + 84;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lwz r4,84(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 84);
	// bl 0x822ca328
	ctx.lr = 0x8259ABB8;
	sub_822CA328(ctx, base);
	// lwz r11,80(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
	// b 0x8259abc4
	goto loc_8259ABC4;
loc_8259ABC0:
	// li r11,0
	ctx.r11.s64 = 0;
loc_8259ABC4:
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

PPC_FUNC_IMPL(__imp__sub_8259ABE0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259ABE0) { __imp__sub_8259ABE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259ABE0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x822c5f18
	ctx.lr = 0x8259ABF8;
	sub_822C5F18(ctx, base);
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// blt cr6,0x8259ac3c
	if (ctx.cr6.lt) goto loc_8259AC3C;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// rlwinm r10,r3,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r11,r11,-14720
	ctx.r11.s64 = ctx.r11.s64 + -14720;
	// lwzx r10,r10,r11
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lbz r9,8(r10)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r10.u32 + 8);
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// beq cr6,0x8259ac24
	if (ctx.cr6.eq) goto loc_8259AC24;
	// lbz r11,88(r10)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r10.u32 + 88);
	// b 0x8259ac40
	goto loc_8259AC40;
loc_8259AC24:
	// lis r10,-32093
	ctx.r10.s64 = -2103246848;
	// lwz r10,-7528(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7528);
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lbz r11,88(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + 88);
	// b 0x8259ac40
	goto loc_8259AC40;
loc_8259AC3C:
	// li r11,255
	ctx.r11.s64 = 255;
loc_8259AC40:
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

PPC_FUNC_IMPL(__imp__sub_8259AC58);  /* forward decl */
PPC_WEAK_FUNC(sub_8259AC58) { __imp__sub_8259AC58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259AC58) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// bl 0x822c5f18
	ctx.lr = 0x8259AC68;
	sub_822C5F18(ctx, base);
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// blt cr6,0x8259acd4
	if (ctx.cr6.lt) goto loc_8259ACD4;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// rlwinm r10,r3,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r11,r11,-14720
	ctx.r11.s64 = ctx.r11.s64 + -14720;
	// lwzx r10,r10,r11
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lbz r9,8(r10)
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r10.u32 + 8);
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// beq cr6,0x8259aca8
	if (ctx.cr6.eq) goto loc_8259ACA8;
	// lhz r11,28(r10)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r10.u32 + 28);
	// rlwinm r11,r11,29,3,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 29) & 0x1FFFFFFF;
	// clrlwi r3,r11,31
	ctx.r3.u64 = ctx.r11.u32 & 0x1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_8259ACA8:
	// lis r10,-32093
	ctx.r10.s64 = -2103246848;
	// lwz r10,-7528(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -7528);
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lhz r11,28(r11)
	ctx.r11.u64 = PPC_LOAD_U16(ctx.r11.u32 + 28);
	// rlwinm r11,r11,29,3,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 29) & 0x1FFFFFFF;
	// clrlwi r3,r11,31
	ctx.r3.u64 = ctx.r11.u32 & 0x1;
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_8259ACD4:
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

PPC_FUNC_IMPL(__imp__sub_8259ACE8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259ACE8) { __imp__sub_8259ACE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259ACE8) {
	PPC_FUNC_PROLOGUE();
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
loc_8259ACF4:
	// lbz r8,0(r10)
	ctx.r8.u64 = PPC_LOAD_U8(ctx.r10.u32 + 0);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// cmplwi cr6,r8,0
	ctx.cr6.compare<uint32_t>(ctx.r8.u32, 0, ctx.xer);
	// bne cr6,0x8259acf4
	if (!ctx.cr6.eq) goto loc_8259ACF4;
	// subf r10,r9,r10
	ctx.r10.u64 = ctx.r10.u64 - ctx.r9.u64;
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
	// rotlwi r10,r10,0
	ctx.r10.u64 = __builtin_rotateleft32(ctx.r10.u32, 0);
	// cmplwi cr6,r10,10
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 10, ctx.xer);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// ble cr6,0x8259ad28
	if (!ctx.cr6.gt) goto loc_8259AD28;
	// li r11,0
	ctx.r11.s64 = 0;
	// stb r11,-16072(r10)
	PPC_STORE_U8(ctx.r10.u32 + -16072, ctx.r11.u8);
	// blr 
	return;
loc_8259AD28:
	// addi r10,r10,-16072
	ctx.r10.s64 = ctx.r10.s64 + -16072;
	// subf r9,r11,r10
	ctx.r9.u64 = ctx.r10.u64 - ctx.r11.u64;
loc_8259AD30:
	// lbz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r9,r11
	PPC_STORE_U8(ctx.r9.u32 + ctx.r11.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x8259ad30
	if (!ctx.cr6.eq) goto loc_8259AD30;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259AD48);  /* forward decl */
PPC_WEAK_FUNC(sub_8259AD48) { __imp__sub_8259AD48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259AD48) {
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
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// clrlwi r9,r3,24
	ctx.r9.u64 = ctx.r3.u32 & 0xFF;
	// addi r31,r11,-16060
	ctx.r31.s64 = ctx.r11.s64 + -16060;
	// li r11,1
	ctx.r11.s64 = 1;
	// mr r10,r4
	ctx.r10.u64 = ctx.r4.u64;
	// mr r30,r5
	ctx.r30.u64 = ctx.r5.u64;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// stb r11,0(r31)
	PPC_STORE_U8(ctx.r31.u32 + 0, ctx.r11.u8);
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// addi r11,r11,-14792
	ctx.r11.s64 = ctx.r11.s64 + -14792;
	// lwz r11,128(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 128);
	// beq cr6,0x8259adb4
	if (ctx.cr6.eq) goto loc_8259ADB4;
	// cmpwi cr6,r11,4
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 4, ctx.xer);
	// beq cr6,0x8259ada8
	if (ctx.cr6.eq) goto loc_8259ADA8;
	// cmpwi cr6,r11,1
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 1, ctx.xer);
	// beq cr6,0x8259ada8
	if (ctx.cr6.eq) goto loc_8259ADA8;
	// lis r11,-32253
	ctx.r11.s64 = -2113732608;
	// addi r4,r11,9580
	ctx.r4.s64 = ctx.r11.s64 + 9580;
	// b 0x8259add8
	goto loc_8259ADD8;
loc_8259ADA8:
	// lis r11,-32253
	ctx.r11.s64 = -2113732608;
	// addi r4,r11,9572
	ctx.r4.s64 = ctx.r11.s64 + 9572;
	// b 0x8259add8
	goto loc_8259ADD8;
loc_8259ADB4:
	// cmpwi cr6,r11,4
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 4, ctx.xer);
	// beq cr6,0x8259add0
	if (ctx.cr6.eq) goto loc_8259ADD0;
	// cmpwi cr6,r11,1
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 1, ctx.xer);
	// beq cr6,0x8259add0
	if (ctx.cr6.eq) goto loc_8259ADD0;
	// lis r11,-32253
	ctx.r11.s64 = -2113732608;
	// addi r4,r11,9564
	ctx.r4.s64 = ctx.r11.s64 + 9564;
	// b 0x8259add8
	goto loc_8259ADD8;
loc_8259ADD0:
	// lis r11,-32253
	ctx.r11.s64 = -2113732608;
	// addi r4,r11,9556
	ctx.r4.s64 = ctx.r11.s64 + 9556;
loc_8259ADD8:
	// mr r5,r10
	ctx.r5.u64 = ctx.r10.u64;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x8298f240
	ctx.lr = 0x8259ADE4;
	sub_8298F240(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
loc_8259ADE8:
	// addi r10,r1,80
	ctx.r10.s64 = ctx.r1.s64 + 80;
	// addi r9,r31,1
	ctx.r9.s64 = ctx.r31.s64 + 1;
	// lbzx r10,r11,r10
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + ctx.r10.u32);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stbx r10,r11,r9
	PPC_STORE_U8(ctx.r11.u32 + ctx.r9.u32, ctx.r10.u8);
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// bne cr6,0x8259ade8
	if (!ctx.cr6.eq) goto loc_8259ADE8;
	// stw r30,52(r31)
	PPC_STORE_U32(ctx.r31.u32 + 52, ctx.r30.u32);
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
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

PPC_FUNC_IMPL(__imp__sub_8259AE20);  /* forward decl */
PPC_WEAK_FUNC(sub_8259AE20) { __imp__sub_8259AE20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259AE20) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// lwz r11,-14248(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -14248);
	// cmpwi cr6,r11,2
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 2, ctx.xer);
	// beq cr6,0x8259aec4
	if (ctx.cr6.eq) goto loc_8259AEC4;
	// cmpwi cr6,r11,6
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 6, ctx.xer);
	// beq cr6,0x8259aec4
	if (ctx.cr6.eq) goto loc_8259AEC4;
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// lbz r11,-14240(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + -14240);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8259aec4
	if (ctx.cr6.eq) goto loc_8259AEC4;
	// bl 0x82671538
	ctx.lr = 0x8259AE58;
	sub_82671538(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8259aec4
	if (!ctx.cr6.eq) goto loc_8259AEC4;
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// lbz r11,-14264(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + -14264);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8259aec4
	if (!ctx.cr6.eq) goto loc_8259AEC4;
	// bl 0x82676100
	ctx.lr = 0x8259AE78;
	sub_82676100(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8259aebc
	if (ctx.cr6.eq) goto loc_8259AEBC;
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// lbz r11,-14262(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + -14262);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8259ae9c
	if (ctx.cr6.eq) goto loc_8259AE9C;
	// li r3,0
	ctx.r3.s64 = 0;
	// bl 0x8213d2c8
	ctx.lr = 0x8259AE9C;
	sub_8213D2C8(ctx, base);
loc_8259AE9C:
	// bl 0x82130448
	ctx.lr = 0x8259AEA0;
	sub_82130448(ctx, base);
	// lis r11,-32086
	ctx.r11.s64 = -2102788096;
	// addi r3,r11,15832
	ctx.r3.s64 = ctx.r11.s64 + 15832;
	// bl 0x82192e00
	ctx.lr = 0x8259AEAC;
	sub_82192E00(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_8259AEBC:
	// li r3,2
	ctx.r3.s64 = 2;
	// bl 0x8224b980
	ctx.lr = 0x8259AEC4;
	sub_8224B980(ctx, base);
loc_8259AEC4:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259AED8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259AED8) { __imp__sub_8259AED8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259AED8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// lwz r11,-14248(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -14248);
	// cmpwi cr6,r11,2
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 2, ctx.xer);
	// beq cr6,0x8259aefc
	if (ctx.cr6.eq) goto loc_8259AEFC;
	// cmpwi cr6,r11,6
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 6, ctx.xer);
	// beq cr6,0x8259aefc
	if (ctx.cr6.eq) goto loc_8259AEFC;
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// lbz r3,-14240(r11)
	ctx.r3.u64 = PPC_LOAD_U8(ctx.r11.u32 + -14240);
	// blr 
	return;
loc_8259AEFC:
	// li r3,0
	ctx.r3.s64 = 0;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259AF08);  /* forward decl */
PPC_WEAK_FUNC(sub_8259AF08) { __imp__sub_8259AF08(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259AF08) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb1c
	ctx.lr = 0x8259AF10;
	__savegprlr_29(ctx, base);
	// addi r12,r1,-32
	ctx.r12.s64 = ctx.r1.s64 + -32;
	// bl 0x82990144
	ctx.lr = 0x8259AF18;
	__savefpr_27(ctx, base);
	// stwu r1,-192(r1)
	ea = -192 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// fmr f31,f1
	ctx.fpscr.disableFlushMode();
	ctx.f31.f64 = ctx.f1.f64;
	// mr r29,r7
	ctx.r29.u64 = ctx.r7.u64;
	// fmr f30,f2
	ctx.f30.f64 = ctx.f2.f64;
	// mr r31,r8
	ctx.r31.u64 = ctx.r8.u64;
	// fmr f29,f3
	ctx.f29.f64 = ctx.f3.f64;
	// mr r30,r10
	ctx.r30.u64 = ctx.r10.u64;
	// fmr f28,f4
	ctx.f28.f64 = ctx.f4.f64;
	// fmr f27,f5
	ctx.f27.f64 = ctx.f5.f64;
	// bl 0x82264048
	ctx.lr = 0x8259AF40;
	sub_82264048(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8259afcc
	if (ctx.cr6.eq) goto loc_8259AFCC;
	// clrlwi r3,r31,24
	ctx.r3.u64 = ctx.r31.u32 & 0xFF;
	// bl 0x825fa798
	ctx.lr = 0x8259AF54;
	sub_825FA798(ctx, base);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x8259afcc
	if (ctx.cr6.eq) goto loc_8259AFCC;
	// lwz r11,5544(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 5544);
	// li r3,0
	ctx.r3.s64 = 0;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8259af70
	if (ctx.cr6.eq) goto loc_8259AF70;
	// lwz r3,10(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 10);
loc_8259AF70:
	// bl 0x82263d28
	ctx.lr = 0x8259AF74;
	sub_82263D28(ctx, base);
	// clrlwi r11,r30,24
	ctx.r11.u64 = ctx.r30.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// addi r11,r3,4
	ctx.r11.s64 = ctx.r3.s64 + 4;
	// bne cr6,0x8259af88
	if (!ctx.cr6.eq) goto loc_8259AF88;
	// addi r11,r3,8
	ctx.r11.s64 = ctx.r3.s64 + 8;
loc_8259AF88:
	// li r9,6
	ctx.r9.s64 = 6;
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// li r11,255
	ctx.r11.s64 = 255;
	// stw r29,100(r1)
	PPC_STORE_U32(ctx.r1.u32 + 100, ctx.r29.u32);
	// li r10,255
	ctx.r10.s64 = 255;
	// fmr f5,f27
	ctx.fpscr.disableFlushMode();
	ctx.f5.f64 = ctx.f27.f64;
	// li r4,-2
	ctx.r4.s64 = -2;
	// fmr f4,f28
	ctx.f4.f64 = ctx.f28.f64;
	// fmr f3,f29
	ctx.f3.f64 = ctx.f29.f64;
	// stw r9,116(r1)
	PPC_STORE_U32(ctx.r1.u32 + 116, ctx.r9.u32);
	// li r9,0
	ctx.r9.s64 = 0;
	// fmr f2,f30
	ctx.f2.f64 = ctx.f30.f64;
	// stw r11,92(r1)
	PPC_STORE_U32(ctx.r1.u32 + 92, ctx.r11.u32);
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// stb r9,111(r1)
	PPC_STORE_U8(ctx.r1.u32 + 111, ctx.r9.u8);
	// bl 0x8242bad8
	ctx.lr = 0x8259AFCC;
	sub_8242BAD8(ctx, base);
loc_8259AFCC:
	// addi r1,r1,192
	ctx.r1.s64 = ctx.r1.s64 + 192;
	// addi r12,r1,-32
	ctx.r12.s64 = ctx.r1.s64 + -32;
	// bl 0x82990190
	ctx.lr = 0x8259AFD8;
	__restfpr_27(ctx, base);
	// b 0x8298eb6c
	__restgprlr_29(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259AFE0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259AFE0) { __imp__sub_8259AFE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259AFE0) {
	PPC_FUNC_PROLOGUE();
	// b 0x8221eac0
	sub_8221EAC0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259AFE8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259AFE8) { __imp__sub_8259AFE8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259AFE8) {
	PPC_FUNC_PROLOGUE();
	// b 0x8221f8e8
	sub_8221F8E8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259AFF0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259AFF0) { __imp__sub_8259AFF0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259AFF0) {
	PPC_FUNC_PROLOGUE();
	// b 0x8221e2d8
	sub_8221E2D8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259AFF8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259AFF8) { __imp__sub_8259AFF8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259AFF8) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// lbz r11,-14240(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + -14240);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bnelr cr6
	if (!ctx.cr6.eq) return;
	// li r3,15
	ctx.r3.s64 = 15;
	// b 0x8224b980
	sub_8224B980(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B010);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B010) { __imp__sub_8259B010(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B010) {
	PPC_FUNC_PROLOGUE();
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B018);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B018) { __imp__sub_8259B018(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B018) {
	PPC_FUNC_PROLOGUE();
	// b 0x8259ae20
	sub_8259AE20(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B020);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B020) { __imp__sub_8259B020(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B020) {
	PPC_FUNC_PROLOGUE();
	// b 0x82599678
	sub_82599678(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B028);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B028) { __imp__sub_8259B028(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B028) {
	PPC_FUNC_PROLOGUE();
	// b 0x821dd2c0
	sub_821DD2C0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B030);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B030) { __imp__sub_8259B030(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B030) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r9,1
	ctx.r9.s64 = 1;
	// li r11,0
	ctx.r11.s64 = 0;
	// lis r10,-32070
	ctx.r10.s64 = -2101739520;
	// li r8,0
	ctx.r8.s64 = 0;
	// addi r3,r10,23448
	ctx.r3.s64 = ctx.r10.s64 + 23448;
	// stb r9,127(r1)
	PPC_STORE_U8(ctx.r1.u32 + 127, ctx.r9.u8);
	// li r10,0
	ctx.r10.s64 = 0;
	// stb r9,111(r1)
	PPC_STORE_U8(ctx.r1.u32 + 111, ctx.r9.u8);
	// li r7,0
	ctx.r7.s64 = 0;
	// li r9,0
	ctx.r9.s64 = 0;
	// stb r11,119(r1)
	PPC_STORE_U8(ctx.r1.u32 + 119, ctx.r11.u8);
	// li r6,0
	ctx.r6.s64 = 0;
	// stb r11,103(r1)
	PPC_STORE_U8(ctx.r1.u32 + 103, ctx.r11.u8);
	// li r5,1
	ctx.r5.s64 = 1;
	// stb r11,95(r1)
	PPC_STORE_U8(ctx.r1.u32 + 95, ctx.r11.u8);
	// li r4,0
	ctx.r4.s64 = 0;
	// stb r11,87(r1)
	PPC_STORE_U8(ctx.r1.u32 + 87, ctx.r11.u8);
	// bl 0x8221f458
	ctx.lr = 0x8259B084;
	sub_8221F458(ctx, base);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B098);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B098) { __imp__sub_8259B098(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B098) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// bl 0x8224c6f0
	ctx.lr = 0x8259B0A8;
	sub_8224C6F0(ctx, base);
	// bl 0x826745d8
	ctx.lr = 0x8259B0AC;
	sub_826745D8(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8259b0bc
	if (ctx.cr6.eq) goto loc_8259B0BC;
	// bl 0x826996a8
	ctx.lr = 0x8259B0BC;
	sub_826996A8(ctx, base);
loc_8259B0BC:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B0D0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B0D0) { __imp__sub_8259B0D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B0D0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32094
	ctx.r10.s64 = -2103312384;
	// li r11,0
	ctx.r11.s64 = 0;
	// stb r11,13004(r10)
	PPC_STORE_U8(ctx.r10.u32 + 13004, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B0E0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B0E0) { __imp__sub_8259B0E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B0E0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,1871(r10)
	PPC_STORE_U8(ctx.r10.u32 + 1871, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B0F0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B0F0) { __imp__sub_8259B0F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B0F0) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,1870(r10)
	PPC_STORE_U8(ctx.r10.u32 + 1870, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B100);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B100) { __imp__sub_8259B100(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B100) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,1869(r10)
	PPC_STORE_U8(ctx.r10.u32 + 1869, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B110);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B110) { __imp__sub_8259B110(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B110) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// li r11,1
	ctx.r11.s64 = 1;
	// stb r11,1872(r10)
	PPC_STORE_U8(ctx.r10.u32 + 1872, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B120);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B120) { __imp__sub_8259B120(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B120) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// li r10,1
	ctx.r10.s64 = 1;
	// addi r11,r11,23448
	ctx.r11.s64 = ctx.r11.s64 + 23448;
	// stb r10,820(r11)
	PPC_STORE_U8(ctx.r11.u32 + 820, ctx.r10.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B138);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B138) { __imp__sub_8259B138(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B138) {
	PPC_FUNC_PROLOGUE();
	// li r3,1
	ctx.r3.s64 = 1;
	// b 0x82265c60
	sub_82265C60(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B140);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B140) { __imp__sub_8259B140(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B140) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32066
	ctx.r11.s64 = -2101477376;
	// addi r10,r11,12896
	ctx.r10.s64 = ctx.r11.s64 + 12896;
	// li r11,0
	ctx.r11.s64 = 0;
	// lhz r9,4(r10)
	ctx.r9.u64 = PPC_LOAD_U16(ctx.r10.u32 + 4);
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// ble cr6,0x8259b180
	if (!ctx.cr6.gt) goto loc_8259B180;
	// lwz r8,0(r10)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// mr r10,r8
	ctx.r10.u64 = ctx.r8.u64;
loc_8259B160:
	// lwz r7,0(r10)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// lwz r7,1340(r7)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r7.u32 + 1340);
	// cmpw cr6,r7,r3
	ctx.cr6.compare<int32_t>(ctx.r7.s32, ctx.r3.s32, ctx.xer);
	// beq cr6,0x8259b1a0
	if (ctx.cr6.eq) goto loc_8259B1A0;
	// addi r11,r11,1
	ctx.r11.s64 = ctx.r11.s64 + 1;
	// addi r10,r10,4
	ctx.r10.s64 = ctx.r10.s64 + 4;
	// cmpw cr6,r11,r9
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r9.s32, ctx.xer);
	// blt cr6,0x8259b160
	if (ctx.cr6.lt) goto loc_8259B160;
loc_8259B180:
	// li r11,0
	ctx.r11.s64 = 0;
loc_8259B184:
	// lwz r11,1344(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 1344);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// stw r11,-6260(r10)
	PPC_STORE_U32(ctx.r10.u32 + -6260, ctx.r11.u32);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,1860(r10)
	PPC_STORE_U32(ctx.r10.u32 + 1860, ctx.r11.u32);
	// blr 
	return;
loc_8259B1A0:
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r11,r11,r8
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + ctx.r8.u32);
	// b 0x8259b184
	goto loc_8259B184;
}

PPC_FUNC_IMPL(__imp__sub_8259B1B0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B1B0) { __imp__sub_8259B1B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B1B0) {
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
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
	// mr r31,r4
	ctx.r31.u64 = ctx.r4.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,4
	ctx.r6.s64 = 4;
	// li r4,1
	ctx.r4.s64 = 1;
	// li r3,1
	ctx.r3.s64 = 1;
	// bl 0x822cc6e0
	ctx.lr = 0x8259B1E0;
	sub_822CC6E0(ctx, base);
	// mr r4,r3
	ctx.r4.u64 = ctx.r3.u64;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// li r3,0
	ctx.r3.s64 = 0;
	// stw r4,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r4.u32);
	// lfs f1,2592(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 2592);
	ctx.f1.f64 = double(temp.f32);
	// bl 0x822c75e0
	ctx.lr = 0x8259B1F8;
	sub_822C75E0(ctx, base);
	// lwz r3,0(r31)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// bl 0x822c7480
	ctx.lr = 0x8259B200;
	sub_822C7480(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8259b214
	if (!ctx.cr6.eq) goto loc_8259B214;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// b 0x8259b23c
	goto loc_8259B23C;
loc_8259B214:
	// lis r11,-32039
	ctx.r11.s64 = -2099707904;
	// lwz r30,15072(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 15072);
	// bl 0x821d2088
	ctx.lr = 0x8259B220;
	sub_821D2088(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// li r5,18
	ctx.r5.s64 = 18;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r8,0
	ctx.r8.s64 = 0;
	// bl 0x823093f8
	ctx.lr = 0x8259B23C;
	sub_823093F8(ctx, base);
loc_8259B23C:
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

PPC_FUNC_IMPL(__imp__sub_8259B258);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B258) { __imp__sub_8259B258(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B258) {
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
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// srawi r10,r5,8
	ctx.xer.ca = (ctx.r5.s32 < 0) & ((ctx.r5.u32 & 0xFF) != 0);
	ctx.r10.s64 = ctx.r5.s32 >> 8;
	// clrlwi r8,r5,24
	ctx.r8.u64 = ctx.r5.u32 & 0xFF;
	// lwz r11,31824(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 31824);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lbzx r9,r9,r10
	ctx.r9.u64 = PPC_LOAD_U8(ctx.r9.u32 + ctx.r10.u32);
	// cmpw cr6,r9,r8
	ctx.cr6.compare<int32_t>(ctx.r9.s32, ctx.r8.s32, ctx.xer);
	// bne cr6,0x8259b2a8
	if (!ctx.cr6.eq) goto loc_8259B2A8;
	// lwz r8,12(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,0(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mullw r11,r8,r10
	ctx.r11.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r10.s32);
	// add r31,r11,r9
	ctx.r31.u64 = ctx.r11.u64 + ctx.r9.u64;
	// b 0x8259b2ac
	goto loc_8259B2AC;
loc_8259B2A8:
	// li r31,0
	ctx.r31.s64 = 0;
loc_8259B2AC:
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,4
	ctx.r6.s64 = 4;
	// li r4,2
	ctx.r4.s64 = 2;
	// li r3,1
	ctx.r3.s64 = 1;
	// bl 0x822cc6e0
	ctx.lr = 0x8259B2C0;
	sub_822CC6E0(ctx, base);
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// stw r3,0(r30)
	PPC_STORE_U32(ctx.r30.u32 + 0, ctx.r3.u32);
	// beq cr6,0x8259b2d8
	if (ctx.cr6.eq) goto loc_8259B2D8;
	// lwz r11,564(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 564);
	// oris r11,r11,8192
	ctx.r11.u64 = ctx.r11.u64 | 536870912;
	// stw r11,564(r31)
	PPC_STORE_U32(ctx.r31.u32 + 564, ctx.r11.u32);
loc_8259B2D8:
	// lwz r3,0(r30)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// bl 0x822c7480
	ctx.lr = 0x8259B2E0;
	sub_822C7480(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8259b2f4
	if (!ctx.cr6.eq) goto loc_8259B2F4;
	// stw r11,0(r30)
	PPC_STORE_U32(ctx.r30.u32 + 0, ctx.r11.u32);
	// b 0x8259b31c
	goto loc_8259B31C;
loc_8259B2F4:
	// lis r11,-32039
	ctx.r11.s64 = -2099707904;
	// lwz r31,15072(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 15072);
	// bl 0x821d2088
	ctx.lr = 0x8259B300;
	sub_821D2088(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// lwz r4,0(r30)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// li r5,18
	ctx.r5.s64 = 18;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r8,0
	ctx.r8.s64 = 0;
	// bl 0x823093f8
	ctx.lr = 0x8259B31C;
	sub_823093F8(ctx, base);
loc_8259B31C:
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

PPC_FUNC_IMPL(__imp__sub_8259B338);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B338) { __imp__sub_8259B338(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B338) {
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
	// mr r5,r3
	ctx.r5.u64 = ctx.r3.u64;
	// mr r31,r4
	ctx.r31.u64 = ctx.r4.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r6,4
	ctx.r6.s64 = 4;
	// li r4,3
	ctx.r4.s64 = 3;
	// li r3,1
	ctx.r3.s64 = 1;
	// bl 0x822cc6e0
	ctx.lr = 0x8259B368;
	sub_822CC6E0(ctx, base);
	// stw r3,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r3.u32);
	// bl 0x822c7480
	ctx.lr = 0x8259B370;
	sub_822C7480(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8259b384
	if (!ctx.cr6.eq) goto loc_8259B384;
	// stw r11,0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
	// b 0x8259b3ac
	goto loc_8259B3AC;
loc_8259B384:
	// lis r11,-32039
	ctx.r11.s64 = -2099707904;
	// lwz r30,15072(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 15072);
	// bl 0x821d2088
	ctx.lr = 0x8259B390;
	sub_821D2088(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// lwz r4,0(r31)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// li r5,18
	ctx.r5.s64 = 18;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// li r8,0
	ctx.r8.s64 = 0;
	// bl 0x823093f8
	ctx.lr = 0x8259B3AC;
	sub_823093F8(ctx, base);
loc_8259B3AC:
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

PPC_FUNC_IMPL(__imp__sub_8259B3C8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B3C8) { __imp__sub_8259B3C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B3C8) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x8298eb18
	ctx.lr = 0x8259B3D0;
	__savegprlr_28(ctx, base);
	// stfd f31,-48(r1)
	ctx.fpscr.disableFlushMode();
	PPC_STORE_U64(ctx.r1.u32 + -48, ctx.f31.u64);
	// stwu r1,-176(r1)
	ea = -176 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// lis r10,-1
	ctx.r10.s64 = -65536;
	// clrlwi r9,r5,24
	ctx.r9.u64 = ctx.r5.u32 & 0xFF;
	// rlwimi r10,r11,8,16,23
	ctx.r10.u64 = (__builtin_rotateleft32(ctx.r11.u32, 8) & 0xFF00) | (ctx.r10.u64 & 0xFFFFFFFFFFFF00FF);
	// clrlwi r11,r4,24
	ctx.r11.u64 = ctx.r4.u32 & 0xFF;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// or r11,r10,r11
	ctx.r11.u64 = ctx.r10.u64 | ctx.r11.u64;
	// addi r3,r1,96
	ctx.r3.s64 = ctx.r1.s64 + 96;
	// rlwimi r9,r11,8,0,23
	ctx.r9.u64 = (__builtin_rotateleft32(ctx.r11.u32, 8) & 0xFFFFFF00) | (ctx.r9.u64 & 0xFFFFFFFF000000FF);
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// mr r30,r6
	ctx.r30.u64 = ctx.r6.u64;
	// mr r29,r7
	ctx.r29.u64 = ctx.r7.u64;
	// mr r28,r8
	ctx.r28.u64 = ctx.r8.u64;
	// stw r9,80(r1)
	PPC_STORE_U32(ctx.r1.u32 + 80, ctx.r9.u32);
	// lwz r31,4376(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4376);
	// lfs f31,664(r31)
	temp.u32 = PPC_LOAD_U32(ctx.r31.u32 + 664);
	ctx.f31.f64 = double(temp.f32);
	// bl 0x827fb7b0
	ctx.lr = 0x8259B41C;
	sub_827FB7B0(ctx, base);
	// addi r5,r1,112
	ctx.r5.s64 = ctx.r1.s64 + 112;
	// fmr f1,f31
	ctx.fpscr.disableFlushMode();
	ctx.f1.f64 = ctx.f31.f64;
	// addi r4,r1,96
	ctx.r4.s64 = ctx.r1.s64 + 96;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x822e2990
	ctx.lr = 0x8259B430;
	sub_822E2990(ctx, base);
	// addi r4,r1,112
	ctx.r4.s64 = ctx.r1.s64 + 112;
	// addi r3,r1,84
	ctx.r3.s64 = ctx.r1.s64 + 84;
	// bl 0x827fb5f0
	ctx.lr = 0x8259B43C;
	sub_827FB5F0(ctx, base);
	// lwz r11,0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// rlwinm r10,r11,16,24,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 16) & 0xFF;
	// rlwinm r9,r11,24,24,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 24) & 0xFF;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r10,0(r30)
	PPC_STORE_U32(ctx.r30.u32 + 0, ctx.r10.u32);
	// stw r9,0(r29)
	PPC_STORE_U32(ctx.r29.u32 + 0, ctx.r9.u32);
	// stw r11,0(r28)
	PPC_STORE_U32(ctx.r28.u32 + 0, ctx.r11.u32);
	// addi r1,r1,176
	ctx.r1.s64 = ctx.r1.s64 + 176;
	// lfd f31,-48(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -48);
	// b 0x8298eb68
	__restgprlr_28(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B468);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B468) { __imp__sub_8259B468(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B468) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// addi r3,r11,-1
	ctx.r3.s64 = ctx.r11.s64 + -1;
	// b 0x8221e768
	sub_8221E768(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B478);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B478) { __imp__sub_8259B478(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B478) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x82272da0
	ctx.lr = 0x8259B4A0;
	sub_82272DA0(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259B4C0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B4C0) { __imp__sub_8259B4C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B4C0) {
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
	// b 0x8221f968
	sub_8221F968(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B4E0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B4E0) { __imp__sub_8259B4E0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B4E0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x82272e00
	ctx.lr = 0x8259B508;
	sub_82272E00(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259B528);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B528) { __imp__sub_8259B528(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B528) {
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
	// lis r10,-32085
	ctx.r10.s64 = -2102722560;
	// lis r11,-32085
	ctx.r11.s64 = -2102722560;
	// addi r10,r10,-11808
	ctx.r10.s64 = ctx.r10.s64 + -11808;
	// addi r11,r11,-12072
	ctx.r11.s64 = ctx.r11.s64 + -12072;
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// li r31,0
	ctx.r31.s64 = 0;
	// lwz r10,196(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + 196);
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r3,r10,r11
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// bl 0x82197b80
	ctx.lr = 0x8259B564;
	sub_82197B80(ctx, base);
	// clrlwi r11,r3,24
	ctx.r11.u64 = ctx.r3.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8259b574
	if (ctx.cr6.eq) goto loc_8259B574;
	// li r31,1
	ctx.r31.s64 = 1;
loc_8259B574:
	// lwz r11,0(r30)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r30.u32 + 0);
	// clrlwi r10,r31,24
	ctx.r10.u64 = ctx.r31.u32 & 0xFF;
	// stw r10,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r10.u32);
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

PPC_FUNC_IMPL(__imp__sub_8259B598);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B598) { __imp__sub_8259B598(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B598) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// addi r11,r11,-14792
	ctx.r11.s64 = ctx.r11.s64 + -14792;
	// lwz r11,68(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 68);
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
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B5C0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B5C0) { __imp__sub_8259B5C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B5C0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// addi r11,r11,-14792
	ctx.r11.s64 = ctx.r11.s64 + -14792;
	// lwz r11,72(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 72);
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
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B5E8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B5E8) { __imp__sub_8259B5E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B5E8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
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
	// stb r11,-22650(r10)
	PPC_STORE_U8(ctx.r10.u32 + -22650, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B618);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B618) { __imp__sub_8259B618(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B618) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
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
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8259b640
	if (!ctx.cr6.eq) goto loc_8259B640;
	// li r11,0
	ctx.r11.s64 = 0;
loc_8259B640:
	// stb r11,-22651(r10)
	PPC_STORE_U8(ctx.r10.u32 + -22651, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B648);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B648) { __imp__sub_8259B648(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B648) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
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
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8259b670
	if (!ctx.cr6.eq) goto loc_8259B670;
	// li r11,0
	ctx.r11.s64 = 0;
loc_8259B670:
	// stb r11,1873(r10)
	PPC_STORE_U8(ctx.r10.u32 + 1873, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B678);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B678) { __imp__sub_8259B678(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B678) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r31,r11,1
	ctx.r31.u64 = ctx.r11.u64 ^ 1;
	// bl 0x82197e98
	ctx.lr = 0x8259B6A0;
	sub_82197E98(ctx, base);
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// addi r11,r11,-7504
	ctx.r11.s64 = ctx.r11.s64 + -7504;
	// stb r31,41(r11)
	PPC_STORE_U8(ctx.r11.u32 + 41, ctx.r31.u8);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B6C0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B6C0) { __imp__sub_8259B6C0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B6C0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r31,r11,1
	ctx.r31.u64 = ctx.r11.u64 ^ 1;
	// bl 0x82197e98
	ctx.lr = 0x8259B6E8;
	sub_82197E98(ctx, base);
	// lis r10,-32066
	ctx.r10.s64 = -2101477376;
	// stb r31,14022(r10)
	PPC_STORE_U8(ctx.r10.u32 + 14022, ctx.r31.u8);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B708);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B708) { __imp__sub_8259B708(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B708) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,8
	ctx.r3.s64 = 8;
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
	// b 0x822cc888
	sub_822CC888(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B730);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B730) { __imp__sub_8259B730(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B730) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r8,-32072
	ctx.r8.s64 = -2101870592;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r10,r11,1
	ctx.r10.u64 = ctx.r11.u64 ^ 1;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// cntlzw r10,r10
	ctx.r10.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// rlwinm r9,r10,27,31,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// lwz r10,4(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// xori r9,r9,1
	ctx.r9.u64 = ctx.r9.u64 ^ 1;
	// stb r9,-22635(r8)
	PPC_STORE_U8(ctx.r8.u32 + -22635, ctx.r9.u8);
	// lis r9,-32072
	ctx.r9.s64 = -2101870592;
	// sth r10,-16080(r9)
	PPC_STORE_U16(ctx.r9.u32 + -16080, ctx.r10.u16);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// sth r11,-16084(r10)
	PPC_STORE_U16(ctx.r10.u32 + -16084, ctx.r11.u16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B780);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B780) { __imp__sub_8259B780(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B780) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,-22652(r10)
	PPC_STORE_U8(ctx.r10.u32 + -22652, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B7A0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B7A0) { __imp__sub_8259B7A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B7A0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lbz r11,-14240(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + -14240);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x8259b7c4
	if (!ctx.cr6.eq) goto loc_8259B7C4;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stw r11,1764(r10)
	PPC_STORE_U32(ctx.r10.u32 + 1764, ctx.r11.u32);
	// blr 
	return;
loc_8259B7C4:
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,1764(r10)
	PPC_STORE_U32(ctx.r10.u32 + 1764, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B7D0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B7D0) { __imp__sub_8259B7D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B7D0) {
	PPC_FUNC_PROLOGUE();
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// lwz r11,-14248(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + -14248);
	// cmpwi cr6,r11,2
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 2, ctx.xer);
	// beq cr6,0x8259b800
	if (ctx.cr6.eq) goto loc_8259B800;
	// cmpwi cr6,r11,6
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 6, ctx.xer);
	// beq cr6,0x8259b800
	if (ctx.cr6.eq) goto loc_8259B800;
	// lis r11,-32065
	ctx.r11.s64 = -2101411840;
	// lwz r10,0(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lbz r11,-14240(r11)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r11.u32 + -14240);
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// stw r11,0(r10)
	PPC_STORE_U32(ctx.r10.u32 + 0, ctx.r11.u32);
	// blr 
	return;
loc_8259B800:
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

PPC_FUNC_IMPL(__imp__sub_8259B818);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B818) { __imp__sub_8259B818(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B818) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r9,-32072
	ctx.r9.s64 = -2101870592;
	// addi r9,r9,3712
	ctx.r9.s64 = ctx.r9.s64 + 3712;
	// lwz r10,-6256(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -6256);
	// addi r8,r9,4
	ctx.r8.s64 = ctx.r9.s64 + 4;
	// addi r9,r9,8
	ctx.r9.s64 = ctx.r9.s64 + 8;
	// lfs f0,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// lfs f13,0(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// stfsx f13,r10,r8
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r10.u32 + ctx.r8.u32, temp.u32);
	// stfsx f0,r10,r9
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + ctx.r9.u32, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B850);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B850) { __imp__sub_8259B850(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B850) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r9,-32072
	ctx.r9.s64 = -2101870592;
	// addi r9,r9,3712
	ctx.r9.s64 = ctx.r9.s64 + 3712;
	// lwz r10,-6256(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -6256);
	// addi r8,r9,16
	ctx.r8.s64 = ctx.r9.s64 + 16;
	// addi r7,r9,17
	ctx.r7.s64 = ctx.r9.s64 + 17;
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// stbx r11,r10,r8
	PPC_STORE_U8(ctx.r10.u32 + ctx.r8.u32, ctx.r11.u8);
	// stbx r9,r10,r7
	PPC_STORE_U8(ctx.r10.u32 + ctx.r7.u32, ctx.r9.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B888);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B888) { __imp__sub_8259B888(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B888) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// li r9,1
	ctx.r9.s64 = 1;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r10,-6256(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -6256);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// addi r11,r11,3712
	ctx.r11.s64 = ctx.r11.s64 + 3712;
	// addi r11,r11,24
	ctx.r11.s64 = ctx.r11.s64 + 24;
	// bne cr6,0x8259b8c8
	if (!ctx.cr6.eq) goto loc_8259B8C8;
	// li r9,0
	ctx.r9.s64 = 0;
loc_8259B8C8:
	// stbx r9,r10,r11
	PPC_STORE_U8(ctx.r10.u32 + ctx.r11.u32, ctx.r9.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B8D0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B8D0) { __imp__sub_8259B8D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B8D0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// li r9,1
	ctx.r9.s64 = 1;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r10,-6256(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -6256);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// addi r11,r11,3712
	ctx.r11.s64 = ctx.r11.s64 + 3712;
	// addi r11,r11,26
	ctx.r11.s64 = ctx.r11.s64 + 26;
	// bne cr6,0x8259b910
	if (!ctx.cr6.eq) goto loc_8259B910;
	// li r9,0
	ctx.r9.s64 = 0;
loc_8259B910:
	// stbx r9,r10,r11
	PPC_STORE_U8(ctx.r10.u32 + ctx.r11.u32, ctx.r9.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B918);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B918) { __imp__sub_8259B918(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B918) {
	PPC_FUNC_PROLOGUE();
	// lwz r9,8(r3)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// addi r11,r11,3712
	ctx.r11.s64 = ctx.r11.s64 + 3712;
	// lwz r9,0(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
	// addi r8,r11,44
	ctx.r8.s64 = ctx.r11.s64 + 44;
	// lwz r10,-6256(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -6256);
	// cntlzw r9,r9
	ctx.r9.u64 = ctx.r9.u32 == 0 ? 32 : __builtin_clz(ctx.r9.u32);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// rlwinm r9,r9,27,31,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 27) & 0x1;
	// xori r11,r9,1
	ctx.r11.u64 = ctx.r9.u64 ^ 1;
	// stbx r11,r10,r8
	PPC_STORE_U8(ctx.r10.u32 + ctx.r8.u32, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B950);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B950) { __imp__sub_8259B950(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B950) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r9,8(r3)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// addi r11,r11,3712
	ctx.r11.s64 = ctx.r11.s64 + 3712;
	// lwz r10,-6256(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -6256);
	// addi r11,r11,40
	ctx.r11.s64 = ctx.r11.s64 + 40;
	// lfs f0,0(r9)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// stfsx f0,r10,r11
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + ctx.r11.u32, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B978);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B978) { __imp__sub_8259B978(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B978) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r9,-32072
	ctx.r9.s64 = -2101870592;
	// addi r9,r9,3712
	ctx.r9.s64 = ctx.r9.s64 + 3712;
	// lwz r10,-6256(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -6256);
	// addi r8,r9,32
	ctx.r8.s64 = ctx.r9.s64 + 32;
	// addi r9,r9,36
	ctx.r9.s64 = ctx.r9.s64 + 36;
	// lfs f0,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// lfs f13,0(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// stfsx f13,r10,r8
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r10.u32 + ctx.r8.u32, temp.u32);
	// stfsx f0,r10,r9
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + ctx.r9.u32, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B9B0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B9B0) { __imp__sub_8259B9B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B9B0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r9,8(r3)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// addi r11,r11,3712
	ctx.r11.s64 = ctx.r11.s64 + 3712;
	// lwz r10,-6256(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -6256);
	// addi r11,r11,36
	ctx.r11.s64 = ctx.r11.s64 + 36;
	// lfs f0,0(r9)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// stfsx f0,r10,r11
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r10.u32 + ctx.r11.u32, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259B9D8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259B9D8) { __imp__sub_8259B9D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259B9D8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// li r9,1
	ctx.r9.s64 = 1;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r10,-6256(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -6256);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// addi r11,r11,3712
	ctx.r11.s64 = ctx.r11.s64 + 3712;
	// addi r11,r11,29
	ctx.r11.s64 = ctx.r11.s64 + 29;
	// bne cr6,0x8259ba18
	if (!ctx.cr6.eq) goto loc_8259BA18;
	// li r9,0
	ctx.r9.s64 = 0;
loc_8259BA18:
	// stbx r9,r10,r11
	PPC_STORE_U8(ctx.r10.u32 + ctx.r11.u32, ctx.r9.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259BA20);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BA20) { __imp__sub_8259BA20(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BA20) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// li r9,1
	ctx.r9.s64 = 1;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r10,-6256(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -6256);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// addi r11,r11,3712
	ctx.r11.s64 = ctx.r11.s64 + 3712;
	// addi r11,r11,28
	ctx.r11.s64 = ctx.r11.s64 + 28;
	// bne cr6,0x8259ba60
	if (!ctx.cr6.eq) goto loc_8259BA60;
	// li r9,0
	ctx.r9.s64 = 0;
loc_8259BA60:
	// stbx r9,r10,r11
	PPC_STORE_U8(ctx.r10.u32 + ctx.r11.u32, ctx.r9.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259BA68);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BA68) { __imp__sub_8259BA68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BA68) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// li r9,1
	ctx.r9.s64 = 1;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r10,-6256(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -6256);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// addi r11,r11,3712
	ctx.r11.s64 = ctx.r11.s64 + 3712;
	// addi r11,r11,27
	ctx.r11.s64 = ctx.r11.s64 + 27;
	// bne cr6,0x8259baa8
	if (!ctx.cr6.eq) goto loc_8259BAA8;
	// li r9,0
	ctx.r9.s64 = 0;
loc_8259BAA8:
	// stbx r9,r10,r11
	PPC_STORE_U8(ctx.r10.u32 + ctx.r11.u32, ctx.r9.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259BAB0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BAB0) { __imp__sub_8259BAB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BAB0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x821de240
	sub_821DE240(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259BAC0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BAC0) { __imp__sub_8259BAC0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BAC0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x821da8a0
	ctx.lr = 0x8259BAE0;
	sub_821DA8A0(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259BB00);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BB00) { __imp__sub_8259BB00(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BB00) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x821da8e0
	ctx.lr = 0x8259BB20;
	sub_821DA8E0(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259BB40);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BB40) { __imp__sub_8259BB40(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BB40) {
	PPC_FUNC_PROLOGUE();
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r9,8(r3)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// addi r11,r11,3712
	ctx.r11.s64 = ctx.r11.s64 + 3712;
	// lwz r10,-6256(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -6256);
	// addi r11,r11,60
	ctx.r11.s64 = ctx.r11.s64 + 60;
	// lwz r9,0(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + 0);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// stwx r9,r10,r11
	PPC_STORE_U32(ctx.r10.u32 + ctx.r11.u32, ctx.r9.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259BB68);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BB68) { __imp__sub_8259BB68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BB68) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,1864(r10)
	PPC_STORE_U8(ctx.r10.u32 + 1864, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259BB88);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BB88) { __imp__sub_8259BB88(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BB88) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// li r9,1
	ctx.r9.s64 = 1;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r10,-6256(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -6256);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// addi r11,r11,3712
	ctx.r11.s64 = ctx.r11.s64 + 3712;
	// addi r11,r11,25
	ctx.r11.s64 = ctx.r11.s64 + 25;
	// bne cr6,0x8259bbc8
	if (!ctx.cr6.eq) goto loc_8259BBC8;
	// li r9,0
	ctx.r9.s64 = 0;
loc_8259BBC8:
	// stbx r9,r10,r11
	PPC_STORE_U8(ctx.r10.u32 + ctx.r11.u32, ctx.r9.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259BBD0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BBD0) { __imp__sub_8259BBD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BBD0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8259b140
	sub_8259B140(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259BBE0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BBE0) { __imp__sub_8259BBE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BBE0) {
	PPC_FUNC_PROLOGUE();
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
	// lis r11,-32017
	ctx.r11.s64 = -2098266112;
	// addi r3,r11,-29456
	ctx.r3.s64 = ctx.r11.s64 + -29456;
	// lis r11,-32072
	ctx.r11.s64 = -2101870592;
	// lwz r4,0(r10)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// stw r4,-6260(r11)
	PPC_STORE_U32(ctx.r11.u32 + -6260, ctx.r4.u32);
	// bl 0x823192b0
	ctx.lr = 0x8259BC08;
	sub_823192B0(ctx, base);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,1860(r10)
	PPC_STORE_U32(ctx.r10.u32 + 1860, ctx.r11.u32);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259BC28);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BC28) { __imp__sub_8259BC28(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BC28) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// addi r3,r11,23448
	ctx.r3.s64 = ctx.r11.s64 + 23448;
	// bl 0x8221e948
	ctx.lr = 0x8259BC48;
	sub_8221E948(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259BC68);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BC68) { __imp__sub_8259BC68(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BC68) {
	PPC_FUNC_PROLOGUE();
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
	// lis r11,-32070
	ctx.r11.s64 = -2101739520;
	// addi r3,r11,23448
	ctx.r3.s64 = ctx.r11.s64 + 23448;
	// lwz r11,0(r10)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// extsw r11,r11
	ctx.r11.s64 = ctx.r11.s32;
	// std r11,80(r1)
	PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r11.u64);
	// lfd f0,80(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f0.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
	// fcfid f0,f0
	ctx.f0.f64 = double(ctx.f0.s64);
	// frsp f1,f0
	ctx.f1.f64 = double(float(ctx.f0.f64));
	// bl 0x8221e960
	ctx.lr = 0x8259BC9C;
	sub_8221E960(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259BCB0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BCB0) { __imp__sub_8259BCB0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BCB0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x82599e58
	sub_82599E58(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259BCD0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BCD0) { __imp__sub_8259BCD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BCD0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x821d2088
	ctx.lr = 0x8259BCE8;
	sub_821D2088(ctx, base);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x8259bd14
	if (ctx.cr6.eq) goto loc_8259BD14;
	// bl 0x821d2088
	ctx.lr = 0x8259BCF4;
	sub_821D2088(ctx, base);
	// lbz r11,133(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 133);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8259bd14
	if (ctx.cr6.eq) goto loc_8259BD14;
	// bl 0x821d2088
	ctx.lr = 0x8259BD04;
	sub_821D2088(ctx, base);
	// lbz r11,134(r3)
	ctx.r11.u64 = PPC_LOAD_U8(ctx.r3.u32 + 134);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// li r11,1
	ctx.r11.s64 = 1;
	// bne cr6,0x8259bd18
	if (!ctx.cr6.eq) goto loc_8259BD18;
loc_8259BD14:
	// li r11,0
	ctx.r11.s64 = 0;
loc_8259BD18:
	// lwz r10,0(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
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

PPC_FUNC_IMPL(__imp__sub_8259BD38);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BD38) { __imp__sub_8259BD38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BD38) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r31,r11,1
	ctx.r31.u64 = ctx.r11.u64 ^ 1;
	// bl 0x821d2088
	ctx.lr = 0x8259BD60;
	sub_821D2088(ctx, base);
	// stb r31,132(r3)
	PPC_STORE_U8(ctx.r3.u32 + 132, ctx.r31.u8);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259BD78);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BD78) { __imp__sub_8259BD78(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BD78) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x8225ee38
	sub_8225EE38(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259BD90);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BD90) { __imp__sub_8259BD90(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BD90) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x8225e2d0
	ctx.lr = 0x8259BDB0;
	sub_8225E2D0(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259BDD0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BDD0) { __imp__sub_8259BDD0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BDD0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x8225e288
	ctx.lr = 0x8259BDF0;
	sub_8225E288(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259BE10);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BE10) { __imp__sub_8259BE10(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BE10) {
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
	// b 0x8225e3e8
	sub_8225E3E8(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259BE38);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BE38) { __imp__sub_8259BE38(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BE38) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8225df98
	sub_8225DF98(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259BE48);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BE48) { __imp__sub_8259BE48(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BE48) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8225ddd0
	sub_8225DDD0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259BE58);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BE58) { __imp__sub_8259BE58(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BE58) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// lfs f1,8(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f1.f64 = double(temp.f32);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x8225df68
	sub_8225DF68(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259BE70);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BE70) { __imp__sub_8259BE70(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BE70) {
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
	// b 0x8225e518
	sub_8225E518(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259BE98);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BE98) { __imp__sub_8259BE98(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BE98) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r3,-7528(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + -7528);
	// bl 0x822c5eb8
	ctx.lr = 0x8259BEB8;
	sub_822C5EB8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259BED8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BED8) { __imp__sub_8259BED8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BED8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x8259a408
	ctx.lr = 0x8259BEF8;
	sub_8259A408(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259BF18);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BF18) { __imp__sub_8259BF18(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BF18) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r11,8(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,-17172(r10)
	PPC_STORE_U32(ctx.r10.u32 + -17172, ctx.r11.u32);
	// bl 0x8259a408
	ctx.lr = 0x8259BF44;
	sub_8259A408(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259BF60);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BF60) { __imp__sub_8259BF60(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BF60) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x8259a578
	ctx.lr = 0x8259BF80;
	sub_8259A578(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259BFA0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BFA0) { __imp__sub_8259BFA0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BFA0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x8259a5f8
	ctx.lr = 0x8259BFC0;
	sub_8259A5F8(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259BFE0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259BFE0) { __imp__sub_8259BFE0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259BFE0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x822c5f18
	ctx.lr = 0x8259C000;
	sub_822C5F18(ctx, base);
	// cmpwi cr6,r3,-1
	ctx.cr6.compare<int32_t>(ctx.r3.s32, -1, ctx.xer);
	// beq cr6,0x8259c02c
	if (ctx.cr6.eq) goto loc_8259C02C;
	// lis r11,-32052
	ctx.r11.s64 = -2100559872;
	// rlwinm r10,r3,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r11,r11,-14720
	ctx.r11.s64 = ctx.r11.s64 + -14720;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// lbz r10,8(r11)
	ctx.r10.u64 = PPC_LOAD_U8(ctx.r11.u32 + 8);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x8259c02c
	if (ctx.cr6.eq) goto loc_8259C02C;
	// lwz r11,32(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 32);
	// b 0x8259c030
	goto loc_8259C030;
loc_8259C02C:
	// li r11,-1
	ctx.r11.s64 = -1;
loc_8259C030:
	// lwz r10,0(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
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

PPC_FUNC_IMPL(__imp__sub_8259C050);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C050) { __imp__sub_8259C050(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C050) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8259a778
	sub_8259A778(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C060);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C060) { __imp__sub_8259C060(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C060) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x8259aa28
	ctx.lr = 0x8259C080;
	sub_8259AA28(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259C0A0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C0A0) { __imp__sub_8259C0A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C0A0) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,6
	ctx.r3.s64 = 6;
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x822cc888
	sub_822CC888(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C0B8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C0B8) { __imp__sub_8259C0B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C0B8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,5
	ctx.r3.s64 = 5;
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x822cc888
	sub_822CC888(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C0D0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C0D0) { __imp__sub_8259C0D0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C0D0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x8259ac58
	ctx.lr = 0x8259C0F0;
	sub_8259AC58(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259C110);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C110) { __imp__sub_8259C110(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C110) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,14
	ctx.r3.s64 = 14;
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
	// b 0x822cc888
	sub_822CC888(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C138);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C138) { __imp__sub_8259C138(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C138) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,11
	ctx.r3.s64 = 11;
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
	// b 0x822cc888
	sub_822CC888(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C160);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C160) { __imp__sub_8259C160(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C160) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,12
	ctx.r3.s64 = 12;
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
	// b 0x822cc888
	sub_822CC888(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C188);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C188) { __imp__sub_8259C188(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C188) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,13
	ctx.r3.s64 = 13;
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
	// b 0x822cc888
	sub_822CC888(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C1B0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C1B0) { __imp__sub_8259C1B0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C1B0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,0
	ctx.r3.s64 = 0;
	// lfs f1,4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x822c75e0
	sub_822C75E0(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C1C8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C1C8) { __imp__sub_8259C1C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C1C8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmpwi cr6,r5,3
	ctx.cr6.compare<int32_t>(ctx.r5.s32, 3, ctx.xer);
	// bne cr6,0x8259c1e0
	if (!ctx.cr6.eq) goto loc_8259C1E0;
	// li r5,2
	ctx.r5.s64 = 2;
loc_8259C1E0:
	// li r3,15
	ctx.r3.s64 = 15;
	// b 0x822cc888
	sub_822CC888(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C1E8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C1E8) { __imp__sub_8259C1E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C1E8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,1
	ctx.r3.s64 = 1;
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x822cc888
	sub_822CC888(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C200);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C200) { __imp__sub_8259C200(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C200) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,3
	ctx.r3.s64 = 3;
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x822cc888
	sub_822CC888(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C218);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C218) { __imp__sub_8259C218(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C218) {
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
	// li r5,0
	ctx.r5.s64 = 0;
	// li r3,4
	ctx.r3.s64 = 4;
	// lwz r31,0(r11)
	ctx.r31.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// bl 0x822cc888
	ctx.lr = 0x8259C244;
	sub_822CC888(ctx, base);
	// lis r11,-32039
	ctx.r11.s64 = -2099707904;
	// lwz r30,15072(r11)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r11.u32 + 15072);
	// bl 0x821d2088
	ctx.lr = 0x8259C250;
	sub_821D2088(ctx, base);
	// mr r6,r3
	ctx.r6.u64 = ctx.r3.u64;
	// li r5,18
	ctx.r5.s64 = 18;
	// mr r4,r31
	ctx.r4.u64 = ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// bl 0x823086d8
	ctx.lr = 0x8259C268;
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

PPC_FUNC_IMPL(__imp__sub_8259C280);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C280) { __imp__sub_8259C280(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C280) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,7
	ctx.r3.s64 = 7;
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
	// b 0x822cc888
	sub_822CC888(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C2A8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C2A8) { __imp__sub_8259C2A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C2A8) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x822c7480
	ctx.lr = 0x8259C2C8;
	sub_822C7480(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259C2E8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C2E8) { __imp__sub_8259C2E8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C2E8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,9
	ctx.r3.s64 = 9;
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x822cc888
	sub_822CC888(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C300);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C300) { __imp__sub_8259C300(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C300) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// li r3,10
	ctx.r3.s64 = 10;
	// lwz r5,4(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,0(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x822cc888
	sub_822CC888(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C318);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C318) { __imp__sub_8259C318(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C318) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32081
	ctx.r10.s64 = -2102460416;
	// addi r10,r10,-16736
	ctx.r10.s64 = ctx.r10.s64 + -16736;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,341(r10)
	PPC_STORE_U8(ctx.r10.u32 + 341, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C340);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C340) { __imp__sub_8259C340(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C340) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stw r11,1860(r10)
	PPC_STORE_U32(ctx.r10.u32 + 1860, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C358);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C358) { __imp__sub_8259C358(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C358) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
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
	// stb r11,1866(r10)
	PPC_STORE_U8(ctx.r10.u32 + 1866, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C388);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C388) { __imp__sub_8259C388(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C388) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,1867(r10)
	PPC_STORE_U8(ctx.r10.u32 + 1867, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C3A8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C3A8) { __imp__sub_8259C3A8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C3A8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// stb r11,1868(r10)
	PPC_STORE_U8(ctx.r10.u32 + 1868, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C3C8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C3C8) { __imp__sub_8259C3C8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C3C8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
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
	// stb r11,-22636(r10)
	PPC_STORE_U8(ctx.r10.u32 + -22636, ctx.r11.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C3F8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C3F8) { __imp__sub_8259C3F8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C3F8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stw r11,-16076(r10)
	PPC_STORE_U32(ctx.r10.u32 + -16076, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C410);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C410) { __imp__sub_8259C410(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C410) {
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
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// clrlwi r3,r10,24
	ctx.r3.u64 = ctx.r10.u32 & 0xFF;
	// lwz r5,8(r11)
	ctx.r5.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r4,4(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// b 0x8259ad48
	sub_8259AD48(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C438);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C438) { __imp__sub_8259C438(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C438) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r3,0(r11)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// b 0x8222dc90
	sub_8222DC90(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C448);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C448) { __imp__sub_8259C448(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C448) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cntlzw r11,r11
	ctx.r11.u64 = ctx.r11.u32 == 0 ? 32 : __builtin_clz(ctx.r11.u32);
	// rlwinm r11,r11,27,31,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 27) & 0x1;
	// xori r11,r11,1
	ctx.r11.u64 = ctx.r11.u64 ^ 1;
	// clrlwi r3,r11,24
	ctx.r3.u64 = ctx.r11.u32 & 0xFF;
	// b 0x8222dd08
	sub_8222DD08(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C468);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C468) { __imp__sub_8259C468(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C468) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lwz r10,28(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 28);
	// lfs f1,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// lwz r7,16(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// lfs f2,4(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// cntlzw r10,r10
	ctx.r10.u64 = ctx.r10.u32 == 0 ? 32 : __builtin_clz(ctx.r10.u32);
	// lwz r8,20(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 20);
	// lfs f3,8(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	ctx.f3.f64 = double(temp.f32);
	// rlwinm r10,r10,27,31,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 27) & 0x1;
	// lfs f4,12(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	ctx.f4.f64 = double(temp.f32);
	// lfs f5,24(r11)
	temp.u32 = PPC_LOAD_U32(ctx.r11.u32 + 24);
	ctx.f5.f64 = double(temp.f32);
	// xori r11,r10,1
	ctx.r11.u64 = ctx.r10.u64 ^ 1;
	// clrlwi r10,r11,24
	ctx.r10.u64 = ctx.r11.u32 & 0xFF;
	// b 0x8259af08
	sub_8259AF08(ctx, base);
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C4A0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C4A0) { __imp__sub_8259C4A0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C4A0) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
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
	// bl 0x82264048
	ctx.lr = 0x8259C4B8;
	sub_82264048(ctx, base);
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

PPC_FUNC_IMPL(__imp__sub_8259C4D8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C4D8) { __imp__sub_8259C4D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C4D8) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stw r11,1880(r10)
	PPC_STORE_U32(ctx.r10.u32 + 1880, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C4F0);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C4F0) { __imp__sub_8259C4F0(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C4F0) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	// lwz r10,8(r3)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r11,-32093
	ctx.r11.s64 = -2103246848;
	// lfs f0,0(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// stfs f0,-12368(r11)
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r11.u32 + -12368, temp.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C508);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C508) { __imp__sub_8259C508(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C508) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32061
	ctx.r10.s64 = -2101149696;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// cmpwi cr6,r11,3
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 3, ctx.xer);
	// bne cr6,0x8259c520
	if (!ctx.cr6.eq) goto loc_8259C520;
	// li r11,2
	ctx.r11.s64 = 2;
loc_8259C520:
	// stw r11,19488(r10)
	PPC_STORE_U32(ctx.r10.u32 + 19488, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C528);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C528) { __imp__sub_8259C528(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C528) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32061
	ctx.r10.s64 = -2101149696;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stw r11,19484(r10)
	PPC_STORE_U32(ctx.r10.u32 + 19484, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C540);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C540) { __imp__sub_8259C540(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C540) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32061
	ctx.r10.s64 = -2101149696;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stw r11,19492(r10)
	PPC_STORE_U32(ctx.r10.u32 + 19492, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C558);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C558) { __imp__sub_8259C558(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C558) {
	PPC_FUNC_PROLOGUE();
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32061
	ctx.r10.s64 = -2101149696;
	// lwz r11,0(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// stw r11,19496(r10)
	PPC_STORE_U32(ctx.r10.u32 + 19496, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C570);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C570) { __imp__sub_8259C570(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C570) {
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
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,0(r13)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r13.u32 + 0);
	// li r10,20
	ctx.r10.s64 = 20;
	// fmr f31,f3
	ctx.f31.f64 = ctx.f3.f64;
	// mr r31,r7
	ctx.r31.u64 = ctx.r7.u64;
	// fmr f30,f4
	ctx.f30.f64 = ctx.f4.f64;
	// lwzx r11,r10,r11
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x8259c5ec
	if (ctx.cr6.eq) goto loc_8259C5EC;
	// li r4,0
	ctx.r4.s64 = 0;
	// stfs f1,80(r1)
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r1.u32 + 80, temp.u32);
	// li r3,28
	ctx.r3.s64 = 28;
	// stfs f2,84(r1)
	temp.f32 = float(ctx.f2.f64);
	PPC_STORE_U32(ctx.r1.u32 + 84, temp.u32);
	// bl 0x8218cb30
	ctx.lr = 0x8259C5BC;
	sub_8218CB30(ctx, base);
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x8259c5e0
	if (ctx.cr6.eq) goto loc_8259C5E0;
	// addi r4,r1,80
	ctx.r4.s64 = ctx.r1.s64 + 80;
	// fmr f2,f30
	ctx.fpscr.disableFlushMode();
	ctx.f2.f64 = ctx.f30.f64;
	// rlwinm r7,r31,24,0,7
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 24) & 0xFF000000;
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// bl 0x82190bb0
	ctx.lr = 0x8259C5D8;
	sub_82190BB0(ctx, base);
	// bl 0x821251a0
	ctx.lr = 0x8259C5DC;
	sub_821251A0(ctx, base);
	// b 0x8259c5fc
	goto loc_8259C5FC;
loc_8259C5E0:
	// li r3,0
	ctx.r3.s64 = 0;
	// bl 0x821251a0
	ctx.lr = 0x8259C5E8;
	sub_821251A0(ctx, base);
	// b 0x8259c5fc
	goto loc_8259C5FC;
loc_8259C5EC:
	// rlwinm r7,r31,24,0,7
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 24) & 0xFF000000;
	// fmr f4,f30
	ctx.fpscr.disableFlushMode();
	ctx.f4.f64 = ctx.f30.f64;
	// fmr f3,f31
	ctx.f3.f64 = ctx.f31.f64;
	// bl 0x821b31e8
	ctx.lr = 0x8259C5FC;
	sub_821B31E8(ctx, base);
loc_8259C5FC:
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
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

PPC_FUNC_IMPL(__imp__sub_8259C618);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C618) { __imp__sub_8259C618(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C618) {
	PPC_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lwz r11,276(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 276);
	// addi r7,r1,96
	ctx.r7.s64 = ctx.r1.s64 + 96;
	// lwz r10,252(r1)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r1.u32 + 252);
	// addi r6,r1,104
	ctx.r6.s64 = ctx.r1.s64 + 104;
	// lwz r9,260(r1)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r1.u32 + 260);
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// lwz r8,268(r1)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r1.u32 + 268);
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// clrlwi r9,r9,24
	ctx.r9.u64 = ctx.r9.u32 & 0xFF;
	// stfs f1,128(r1)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f1.f64);
	PPC_STORE_U32(ctx.r1.u32 + 128, temp.u32);
	// clrlwi r8,r8,24
	ctx.r8.u64 = ctx.r8.u32 & 0xFF;
	// stfs f2,132(r1)
	temp.f32 = float(ctx.f2.f64);
	PPC_STORE_U32(ctx.r1.u32 + 132, temp.u32);
	// rlwimi r10,r11,8,16,23
	ctx.r10.u64 = (__builtin_rotateleft32(ctx.r11.u32, 8) & 0xFF00) | (ctx.r10.u64 & 0xFFFFFFFFFFFF00FF);
	// stfs f3,120(r1)
	temp.f32 = float(ctx.f3.f64);
	PPC_STORE_U32(ctx.r1.u32 + 120, temp.u32);
	// rlwimi r8,r9,8,16,23
	ctx.r8.u64 = (__builtin_rotateleft32(ctx.r9.u32, 8) & 0xFF00) | (ctx.r8.u64 & 0xFFFFFFFFFFFF00FF);
	// stfs f4,124(r1)
	temp.f32 = float(ctx.f4.f64);
	PPC_STORE_U32(ctx.r1.u32 + 124, temp.u32);
	// mr r11,r10
	ctx.r11.u64 = ctx.r10.u64;
	// lwz r10,284(r1)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r1.u32 + 284);
	// mr r9,r8
	ctx.r9.u64 = ctx.r8.u64;
	// lwz r8,244(r1)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r1.u32 + 244);
	// addi r5,r1,112
	ctx.r5.s64 = ctx.r1.s64 + 112;
	// stfs f5,112(r1)
	temp.f32 = float(ctx.f5.f64);
	PPC_STORE_U32(ctx.r1.u32 + 112, temp.u32);
	// rlwimi r9,r11,16,0,15
	ctx.r9.u64 = (__builtin_rotateleft32(ctx.r11.u32, 16) & 0xFFFF0000) | (ctx.r9.u64 & 0xFFFFFFFF0000FFFF);
	// lwz r11,292(r1)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r1.u32 + 292);
	// addi r4,r1,120
	ctx.r4.s64 = ctx.r1.s64 + 120;
	// stfs f6,116(r1)
	temp.f32 = float(ctx.f6.f64);
	PPC_STORE_U32(ctx.r1.u32 + 116, temp.u32);
	// addi r3,r1,128
	ctx.r3.s64 = ctx.r1.s64 + 128;
	// stfs f7,104(r1)
	temp.f32 = float(ctx.f7.f64);
	PPC_STORE_U32(ctx.r1.u32 + 104, temp.u32);
	// stfs f8,108(r1)
	temp.f32 = float(ctx.f8.f64);
	PPC_STORE_U32(ctx.r1.u32 + 108, temp.u32);
	// stfs f9,96(r1)
	temp.f32 = float(ctx.f9.f64);
	PPC_STORE_U32(ctx.r1.u32 + 96, temp.u32);
	// stfs f10,100(r1)
	temp.f32 = float(ctx.f10.f64);
	PPC_STORE_U32(ctx.r1.u32 + 100, temp.u32);
	// stw r11,84(r1)
	PPC_STORE_U32(ctx.r1.u32 + 84, ctx.r11.u32);
	// bl 0x821b3cf0
	ctx.lr = 0x8259C6A8;
	sub_821B3CF0(ctx, base);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// lwz r12,-8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C6B8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C6B8) { __imp__sub_8259C6B8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C6B8) {
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
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lis r11,-32245
	ctx.r11.s64 = -2113208320;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// addi r4,r11,-27176
	ctx.r4.s64 = ctx.r11.s64 + -27176;
	// lwz r10,8(r31)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r31.u32 + 8);
	// lwz r30,0(r10)
	ctx.r30.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);
	// mr r5,r30
	ctx.r5.u64 = ctx.r30.u64;
	// bl 0x8298f240
	ctx.lr = 0x8259C6EC;
	sub_8298F240(ctx, base);
	// lis r11,-32061
	ctx.r11.s64 = -2101149696;
	// addi r5,r1,80
	ctx.r5.s64 = ctx.r1.s64 + 80;
	// addi r3,r11,12544
	ctx.r3.s64 = ctx.r11.s64 + 12544;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// bl 0x82273060
	ctx.lr = 0x8259C700;
	sub_82273060(ctx, base);
	// li r4,0
	ctx.r4.s64 = 0;
	// bl 0x82272858
	ctx.lr = 0x8259C708;
	sub_82272858(ctx, base);
	// lwz r11,0(r31)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + 0);
	// stw r3,0(r11)
	PPC_STORE_U32(ctx.r11.u32 + 0, ctx.r3.u32);
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

PPC_FUNC_IMPL(__imp__sub_8259C728);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C728) { __imp__sub_8259C728(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C728) {
	PPC_FUNC_PROLOGUE();
	// lis r9,-32072
	ctx.r9.s64 = -2101870592;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// addi r10,r10,3712
	ctx.r10.s64 = ctx.r10.s64 + 3712;
	// lwz r9,-6256(r9)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r9.u32 + -6256);
	// addi r7,r10,20
	ctx.r7.s64 = ctx.r10.s64 + 20;
	// lwz r10,0(r11)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// mulli r8,r9,124
	ctx.r8.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(124));
	// lwz r6,12(r11)
	ctx.r6.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r11,8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// clrlwi r6,r6,24
	ctx.r6.u64 = ctx.r6.u32 & 0xFF;
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// clrlwi r9,r9,24
	ctx.r9.u64 = ctx.r9.u32 & 0xFF;
	// rlwimi r10,r6,8,16,23
	ctx.r10.u64 = (__builtin_rotateleft32(ctx.r6.u32, 8) & 0xFF00) | (ctx.r10.u64 & 0xFFFFFFFFFFFF00FF);
	// rlwimi r11,r9,8,16,23
	ctx.r11.u64 = (__builtin_rotateleft32(ctx.r9.u32, 8) & 0xFF00) | (ctx.r11.u64 & 0xFFFFFFFFFFFF00FF);
	// rlwimi r11,r10,16,0,15
	ctx.r11.u64 = (__builtin_rotateleft32(ctx.r10.u32, 16) & 0xFFFF0000) | (ctx.r11.u64 & 0xFFFFFFFF0000FFFF);
	// stwx r11,r8,r7
	PPC_STORE_U32(ctx.r8.u32 + ctx.r7.u32, ctx.r11.u32);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C778);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C778) { __imp__sub_8259C778(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C778) {
	PPC_FUNC_PROLOGUE();
	// lis r9,-32072
	ctx.r9.s64 = -2101870592;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// addi r9,r9,3712
	ctx.r9.s64 = ctx.r9.s64 + 3712;
	// addi r6,r9,52
	ctx.r6.s64 = ctx.r9.s64 + 52;
	// addi r5,r9,56
	ctx.r5.s64 = ctx.r9.s64 + 56;
	// lwz r8,8(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,16(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// clrlwi r8,r8,24
	ctx.r8.u64 = ctx.r8.u32 & 0xFF;
	// lwz r7,0(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// clrlwi r9,r9,24
	ctx.r9.u64 = ctx.r9.u32 & 0xFF;
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// clrlwi r4,r4,24
	ctx.r4.u64 = ctx.r4.u32 & 0xFF;
	// lwz r10,-6256(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -6256);
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// rlwimi r9,r4,8,16,23
	ctx.r9.u64 = (__builtin_rotateleft32(ctx.r4.u32, 8) & 0xFF00) | (ctx.r9.u64 & 0xFFFFFFFFFFFF00FF);
	// rlwimi r11,r8,8,16,23
	ctx.r11.u64 = (__builtin_rotateleft32(ctx.r8.u32, 8) & 0xFF00) | (ctx.r11.u64 & 0xFFFFFFFFFFFF00FF);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// rlwimi r11,r9,16,0,15
	ctx.r11.u64 = (__builtin_rotateleft32(ctx.r9.u32, 16) & 0xFFFF0000) | (ctx.r11.u64 & 0xFFFFFFFF0000FFFF);
	// stwx r11,r10,r6
	PPC_STORE_U32(ctx.r10.u32 + ctx.r6.u32, ctx.r11.u32);
	// stbx r7,r10,r5
	PPC_STORE_U8(ctx.r10.u32 + ctx.r5.u32, ctx.r7.u8);
	// blr 
	return;
}

PPC_FUNC_IMPL(__imp__sub_8259C7D8);  /* forward decl */
PPC_WEAK_FUNC(sub_8259C7D8) { __imp__sub_8259C7D8(ctx, base); } /* macOS: alias→wrapper */
PPC_FUNC_IMPL(__imp__sub_8259C7D8) {
	PPC_FUNC_PROLOGUE();
	// lis r9,-32072
	ctx.r9.s64 = -2101870592;
	// lwz r11,8(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 8);
	// lis r10,-32072
	ctx.r10.s64 = -2101870592;
	// addi r9,r9,3712
	ctx.r9.s64 = ctx.r9.s64 + 3712;
	// addi r6,r9,52
	ctx.r6.s64 = ctx.r9.s64 + 52;
	// addi r5,r9,57
	ctx.r5.s64 = ctx.r9.s64 + 57;
	// lwz r8,8(r11)
	ctx.r8.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// lwz r9,4(r11)
	ctx.r9.u64 = PPC_LOAD_U32(ctx.r11.u32 + 4);
	// lwz r4,16(r11)
	ctx.r4.u64 = PPC_LOAD_U32(ctx.r11.u32 + 16);
	// clrlwi r8,r8,24
	ctx.r8.u64 = ctx.r8.u32 & 0xFF;
	// lwz r7,0(r11)
	ctx.r7.u64 = PPC_LOAD_U32(ctx.r11.u32 + 0);
	// clrlwi r9,r9,24
	ctx.r9.u64 = ctx.r9.u32 & 0xFF;
	// lwz r11,12(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 12);
	// clrlwi r4,r4,24
	ctx.r4.u64 = ctx.r4.u32 & 0xFF;
	// lwz r10,-6256(r10)
	ctx.r10.u64 = PPC_LOAD_U32(ctx.r10.u32 + -6256);
	// clrlwi r11,r11,24
	ctx.r11.u64 = ctx.r11.u32 & 0xFF;
	// rlwimi r9,r4,8,16,23
	ctx.r9.u64 = (__builtin_rotateleft32(ctx.r4.u32, 8) & 0xFF00) | (ctx.r9.u64 & 0xFFFFFFFFFFFF00FF);
	// rlwimi r11,r8,8,16,23
	ctx.r11.u64 = (__builtin_rotateleft32(ctx.r8.u32, 8) & 0xFF00) | (ctx.r11.u64 & 0xFFFFFFFFFFFF00FF);
	// mulli r10,r10,124
	ctx.r10.s64 = static_cast<int64_t>(ctx.r10.u64 * static_cast<uint64_t>(124));
	// rlwimi r11,r9,16,0,15
	ctx.r11.u64 = (__builtin_rotateleft32(ctx.r9.u32, 16) & 0xFFFF0000) | (ctx.r11.u64 & 0xFFFFFFFF0000FFFF);
	// stwx r11,r10,r6
	PPC_STORE_U32(ctx.r10.u32 + ctx.r6.u32, ctx.r11.u32);
	// stbx r7,r10,r5
	PPC_STORE_U8(ctx.r10.u32 + ctx.r5.u32, ctx.r7.u8);
	// blr 
	return;
}

