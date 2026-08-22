#include "gta4_init.h"

DEFINE_REX_FUNC(sub_82A7F7B8) {
	REX_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	REX_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	REX_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	REX_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r11,244(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 244);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a7f7e0
	if (ctx.cr6.eq) goto loc_82A7F7E0;
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x82A7F7E0;
	REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
loc_82A7F7E0:
	// lwz r11,120(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 120);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82a7f7f4
	if (!ctx.cr6.eq) goto loc_82A7F7F4;
	// lwz r3,84(r31)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r31.u32 + 84);
	// bl 0x82a11f68
	ctx.lr = 0x82A7F7F4;
	CloseHandle(ctx, base);
loc_82A7F7F4:
	// lwz r11,252(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 252);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a7f80c
	if (ctx.cr6.eq) goto loc_82A7F80C;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x82A7F80C;
	REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
loc_82A7F80C:
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = REX_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = REX_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

DEFINE_REX_FUNC(sub_82A7F820) {
	REX_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7b8
	ctx.lr = 0x82A7F828;
	__savegprlr_24(ctx, base);
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	REX_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// li r27,0
	ctx.r27.s64 = 0;
	// lwz r24,44(r31)
	ctx.r24.u64 = REX_LOAD_U32(ctx.r31.u32 + 44);
	// lwz r11,32(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 32);
	// stw r27,80(r1)
	REX_STORE_U32(ctx.r1.u32 + 80, ctx.r27.u32);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a7f854
	if (ctx.cr6.eq) goto loc_82A7F854;
loc_82A7F848:
	// li r3,0
	ctx.r3.s64 = 0;
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// b 0x829ff808
	__restgprlr_24(ctx, base);
	return;
loc_82A7F854:
	// lwz r11,80(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 80);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82a7f848
	if (!ctx.cr6.eq) goto loc_82A7F848;
	// lwz r11,248(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 248);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a7fa64
	if (ctx.cr6.eq) goto loc_82A7FA64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x82A7F878;
	REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// cmpwi cr6,r3,0
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 0, ctx.xer);
	// beq cr6,0x82a7fa64
	if (ctx.cr6.eq) goto loc_82A7FA64;
	// lwz r11,128(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 128);
	// lis r28,2
	ctx.r28.s64 = 131072;
	// lwz r10,88(r31)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r31.u32 + 88);
	// addi r25,r31,100
	ctx.r25.s64 = ctx.r31.s64 + 100;
	// lwz r9,100(r31)
	ctx.r9.u64 = REX_LOAD_U32(ctx.r31.u32 + 100);
	// subf r29,r10,r11
	ctx.r29.u64 = ctx.r11.u64 - ctx.r10.u64;
	// cmplw cr6,r9,r28
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, ctx.r28.u32, ctx.xer);
	// blt cr6,0x82a7fa34
	if (ctx.cr6.lt) goto loc_82A7FA34;
	// cmplwi cr6,r29,0
	ctx.cr6.compare<uint32_t>(ctx.r29.u32, 0, ctx.xer);
	// beq cr6,0x82a7fa34
	if (ctx.cr6.eq) goto loc_82A7FA34;
	// mr r30,r28
	ctx.r30.u64 = ctx.r28.u64;
	// cmplw cr6,r29,r28
	ctx.cr6.compare<uint32_t>(ctx.r29.u32, ctx.r28.u32, ctx.xer);
	// bge cr6,0x82a7f8b8
	if (!ctx.cr6.lt) goto loc_82A7F8B8;
	// mr r30,r29
	ctx.r30.u64 = ctx.r29.u64;
loc_82A7F8B8:
	// bl 0x82a7a318
	ctx.lr = 0x82A7F8BC;
	sub_82A7A318(ctx, base);
	// lwz r11,88(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 88);
	// mr r26,r3
	ctx.r26.u64 = ctx.r3.u64;
	// clrlwi r11,r11,15
	ctx.r11.u64 = ctx.r11.u32 & 0x1FFFF;
	// cmplw cr6,r11,r30
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, ctx.r30.u32, ctx.xer);
	// bgt cr6,0x82a7f8d8
	if (ctx.cr6.gt) goto loc_82A7F8D8;
	// cmplw cr6,r30,r29
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, ctx.r29.u32, ctx.xer);
	// bne cr6,0x82a7f8e0
	if (!ctx.cr6.eq) goto loc_82A7F8E0;
loc_82A7F8D8:
	// mr r11,r27
	ctx.r11.u64 = ctx.r27.u64;
	// b 0x82a7f8f4
	goto loc_82A7F8F4;
loc_82A7F8E0:
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a7f8f4
	if (ctx.cr6.eq) goto loc_82A7F8F4;
	// lwz r10,96(r31)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r31.u32 + 96);
	// add r10,r10,r11
	ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
	// stw r10,96(r31)
	REX_STORE_U32(ctx.r31.u32 + 96, ctx.r10.u32);
loc_82A7F8F4:
	// li r29,1
	ctx.r29.s64 = 1;
	// subf r30,r11,r30
	ctx.r30.u64 = ctx.r30.u64 - ctx.r11.u64;
	// li r7,0
	ctx.r7.s64 = 0;
	// addi r6,r1,80
	ctx.r6.s64 = ctx.r1.s64 + 80;
	// mr r5,r30
	ctx.r5.u64 = ctx.r30.u64;
	// stw r29,36(r31)
	REX_STORE_U32(ctx.r31.u32 + 36, ctx.r29.u32);
	// lwz r10,116(r31)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r31.u32 + 116);
	// lwz r3,84(r31)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r31.u32 + 84);
	// add r4,r10,r11
	ctx.r4.u64 = ctx.r10.u64 + ctx.r11.u64;
	// bl 0x82a12958
	ctx.lr = 0x82A7F91C;
	ReadFile(ctx, base);
	// lwz r3,80(r1)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r1.u32 + 80);
	// stw r27,36(r31)
	REX_STORE_U32(ctx.r31.u32 + 36, ctx.r27.u32);
	// cmplw cr6,r3,r30
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, ctx.r30.u32, ctx.xer);
	// beq cr6,0x82a7f930
	if (ctx.cr6.eq) goto loc_82A7F930;
	// stw r29,32(r31)
	REX_STORE_U32(ctx.r31.u32 + 32, ctx.r29.u32);
loc_82A7F930:
	// cmplwi cr6,r3,0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6,0x82a7fa40
	if (ctx.cr6.eq) goto loc_82A7FA40;
	// lwz r11,40(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 40);
	// add r11,r11,r3
	ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
	// stw r11,40(r31)
	REX_STORE_U32(ctx.r31.u32 + 40, ctx.r11.u32);
	// lwz r11,88(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 88);
	// add r11,r11,r3
	ctx.r11.u64 = ctx.r11.u64 + ctx.r3.u64;
	// stw r11,88(r31)
	REX_STORE_U32(ctx.r31.u32 + 88, ctx.r11.u32);
	// lwz r11,116(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 116);
	// add r11,r11,r28
	ctx.r11.u64 = ctx.r11.u64 + ctx.r28.u64;
	// stw r11,116(r31)
	REX_STORE_U32(ctx.r31.u32 + 116, ctx.r11.u32);
	// lwz r11,112(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 112);
	// lwz r10,116(r31)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r31.u32 + 116);
	// cmplw cr6,r10,r11
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, ctx.r11.u32, ctx.xer);
	// blt cr6,0x82a7f974
	if (ctx.cr6.lt) goto loc_82A7F974;
	// lwz r11,108(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 108);
	// stw r11,116(r31)
	REX_STORE_U32(ctx.r31.u32 + 116, ctx.r11.u32);
loc_82A7F974:
	// neg r11,r3
	ctx.r11.s64 = static_cast<int64_t>(-ctx.r3.u64);
loc_82A7F978:
	// mfmsr r8
	std::atomic_thread_fence(std::memory_order_seq_cst);
	ctx.r8.u64 = REX_CHECK_GLOBAL_LOCK();
	// mtmsrd r13,1
	std::atomic_thread_fence(std::memory_order_seq_cst);
	ctx.msr = (ctx.r13.u32 & 0x8020) | (ctx.msr & ~0x8020);
	REX_ENTER_GLOBAL_LOCK();
	// lwarx r10,0,r25
	ea = ctx.r25.u32;
	ctx.reserved.u32 = *(uint32_t*)REX_RAW_ADDR(ea);
	ctx.r10.u64 = __builtin_bswap32(ctx.reserved.u32);
	// add r9,r11,r10
	ctx.r9.u64 = ctx.r11.u64 + ctx.r10.u64;
	// stwcx. r9,0,r25
	ea = ctx.r25.u32;
	ctx.cr0.lt = 0;
	ctx.cr0.gt = 0;
	ctx.cr0.eq = __sync_bool_compare_and_swap(reinterpret_cast<uint32_t*>(REX_RAW_ADDR(ea)), ctx.reserved.s32, __builtin_bswap32(ctx.r9.s32));
	ctx.cr0.so = ctx.xer.so;
	// mtmsrd r8,1
	std::atomic_thread_fence(std::memory_order_seq_cst);
	ctx.msr = (ctx.r8.u32 & 0x8020) | (ctx.msr & ~0x8020);
	REX_LEAVE_GLOBAL_LOCK();
	// bne 0x82a7f978
	if (!ctx.cr0.eq) goto loc_82A7F978;
	// lwz r7,80(r1)
	ctx.r7.u64 = REX_LOAD_U32(ctx.r1.u32 + 80);
	// addi r11,r31,76
	ctx.r11.s64 = ctx.r31.s64 + 76;
loc_82A7F99C:
	// mfmsr r8
	std::atomic_thread_fence(std::memory_order_seq_cst);
	ctx.r8.u64 = REX_CHECK_GLOBAL_LOCK();
	// mtmsrd r13,1
	std::atomic_thread_fence(std::memory_order_seq_cst);
	ctx.msr = (ctx.r13.u32 & 0x8020) | (ctx.msr & ~0x8020);
	REX_ENTER_GLOBAL_LOCK();
	// lwarx r10,0,r11
	ea = ctx.r11.u32;
	ctx.reserved.u32 = *(uint32_t*)REX_RAW_ADDR(ea);
	ctx.r10.u64 = __builtin_bswap32(ctx.reserved.u32);
	// add r9,r7,r10
	ctx.r9.u64 = ctx.r7.u64 + ctx.r10.u64;
	// stwcx. r9,0,r11
	ea = ctx.r11.u32;
	ctx.cr0.lt = 0;
	ctx.cr0.gt = 0;
	ctx.cr0.eq = __sync_bool_compare_and_swap(reinterpret_cast<uint32_t*>(REX_RAW_ADDR(ea)), ctx.reserved.s32, __builtin_bswap32(ctx.r9.s32));
	ctx.cr0.so = ctx.xer.so;
	// mtmsrd r8,1
	std::atomic_thread_fence(std::memory_order_seq_cst);
	ctx.msr = (ctx.r8.u32 & 0x8020) | (ctx.msr & ~0x8020);
	REX_LEAVE_GLOBAL_LOCK();
	// bne 0x82a7f99c
	if (!ctx.cr0.eq) goto loc_82A7F99C;
	// lwz r10,76(r31)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r31.u32 + 76);
	// lwz r9,68(r31)
	ctx.r9.u64 = REX_LOAD_U32(ctx.r31.u32 + 68);
	// cmplw cr6,r10,r9
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, ctx.r9.u32, ctx.xer);
	// ble cr6,0x82a7f9d0
	if (!ctx.cr6.gt) goto loc_82A7F9D0;
	// lwz r11,0(r11)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r11.u32 + 0);
	// stw r11,68(r31)
	REX_STORE_U32(ctx.r31.u32 + 68, ctx.r11.u32);
loc_82A7F9D0:
	// lwz r11,132(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 132);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a7f9ec
	if (ctx.cr6.eq) goto loc_82A7F9EC;
	// mr r5,r26
	ctx.r5.u64 = ctx.r26.u64;
	// lwz r4,80(r1)
	ctx.r4.u64 = REX_LOAD_U32(ctx.r1.u32 + 80);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a7f2d8
	ctx.lr = 0x82A7F9EC;
	sub_82A7F2D8(ctx, base);
loc_82A7F9EC:
	// bl 0x82a7a318
	ctx.lr = 0x82A7F9F0;
	sub_82A7A318(ctx, base);
	// lwz r10,48(r31)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r31.u32 + 48);
	// subf r11,r26,r3
	ctx.r11.u64 = ctx.r3.u64 - ctx.r26.u64;
	// cmpwi cr6,r24,0
	ctx.cr6.compare<int32_t>(ctx.r24.s32, 0, ctx.xer);
	// add r10,r10,r11
	ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
	// stw r10,48(r31)
	REX_STORE_U32(ctx.r31.u32 + 48, ctx.r10.u32);
	// bne cr6,0x82a7fa24
	if (!ctx.cr6.eq) goto loc_82A7FA24;
	// lwz r10,44(r31)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r31.u32 + 44);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// bne cr6,0x82a7fa24
	if (!ctx.cr6.eq) goto loc_82A7FA24;
	// lwz r10,56(r31)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r31.u32 + 56);
	// add r11,r10,r11
	ctx.r11.u64 = ctx.r10.u64 + ctx.r11.u64;
	// stw r11,56(r31)
	REX_STORE_U32(ctx.r31.u32 + 56, ctx.r11.u32);
	// b 0x82a7fa3c
	goto loc_82A7FA3C;
loc_82A7FA24:
	// lwz r10,60(r31)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r31.u32 + 60);
	// add r11,r10,r11
	ctx.r11.u64 = ctx.r10.u64 + ctx.r11.u64;
	// stw r11,60(r31)
	REX_STORE_U32(ctx.r31.u32 + 60, ctx.r11.u32);
	// b 0x82a7fa3c
	goto loc_82A7FA3C;
loc_82A7FA34:
	// lwz r11,76(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 76);
	// stw r11,72(r31)
	REX_STORE_U32(ctx.r31.u32 + 72, ctx.r11.u32);
loc_82A7FA3C:
	// lwz r3,80(r1)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r1.u32 + 80);
loc_82A7FA40:
	// lwz r11,252(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 252);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a7fa80
	if (ctx.cr6.eq) goto loc_82A7FA80;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x82A7FA58;
	REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// lwz r3,80(r1)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r1.u32 + 80);
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// b 0x829ff808
	__restgprlr_24(ctx, base);
	return;
loc_82A7FA64:
	// lwz r11,256(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 256);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a7fa7c
	if (ctx.cr6.eq) goto loc_82A7FA7C;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x82A7FA7C;
	REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
loc_82A7FA7C:
	// li r3,-1
	ctx.r3.s64 = -1;
loc_82A7FA80:
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// b 0x829ff808
	__restgprlr_24(ctx, base);
	return;
}

DEFINE_REX_FUNC(sub_82A7FA88) {
	REX_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	REX_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31,-16(r1)
	REX_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	REX_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// clrlwi r11,r4,31
	ctx.r11.u64 = ctx.r4.u32 & 0x1;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a7fafc
	if (ctx.cr6.eq) goto loc_82A7FAFC;
	// lwz r11,80(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 80);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82a7fabc
	if (!ctx.cr6.eq) goto loc_82A7FABC;
	// li r11,1
	ctx.r11.s64 = 1;
	// stw r11,80(r31)
	REX_STORE_U32(ctx.r31.u32 + 80, ctx.r11.u32);
loc_82A7FABC:
	// rlwinm r11,r4,0,0,0
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 0) & 0x80000000;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a7fb30
	if (ctx.cr6.eq) goto loc_82A7FB30;
	// lwz r11,244(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 244);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a7fae0
	if (ctx.cr6.eq) goto loc_82A7FAE0;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x82A7FAE0;
	REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
loc_82A7FAE0:
	// lwz r11,252(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 252);
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a7fb30
	if (ctx.cr6.eq) goto loc_82A7FB30;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl 
	ctx.lr = 0x82A7FAF8;
	REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// b 0x82a7fb30
	goto loc_82A7FB30;
loc_82A7FAFC:
	// rlwinm r11,r4,0,30,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 0) & 0x2;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a7fb30
	if (ctx.cr6.eq) goto loc_82A7FB30;
	// lwz r11,80(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 80);
	// cmplwi cr6,r11,1
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 1, ctx.xer);
	// bne cr6,0x82a7fb1c
	if (!ctx.cr6.eq) goto loc_82A7FB1C;
	// li r11,0
	ctx.r11.s64 = 0;
	// stw r11,80(r31)
	REX_STORE_U32(ctx.r31.u32 + 80, ctx.r11.u32);
loc_82A7FB1C:
	// rlwinm r11,r4,0,0,0
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 0) & 0x80000000;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a7fb30
	if (ctx.cr6.eq) goto loc_82A7FB30;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a7f820
	ctx.lr = 0x82A7FB30;
	sub_82A7F820(ctx, base);
loc_82A7FB30:
	// lwz r3,80(r31)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r31.u32 + 80);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = REX_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31,-16(r1)
	ctx.r31.u64 = REX_LOAD_U64(ctx.r1.u32 + -16);
	// blr 
	return;
}

DEFINE_REX_FUNC(sub_82A7FB48) {
	REX_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7cc
	ctx.lr = 0x82A7FB50;
	__savegprlr_29(ctx, base);
	// stwu r1,-112(r1)
	ea = -112 + ctx.r1.u32;
	REX_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// li r5,324
	ctx.r5.s64 = 324;
	// li r4,0
	ctx.r4.s64 = 0;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// bl 0x829ff840
	ctx.lr = 0x82A7FB6C;
	rexcrt_memset(ctx, base);
	// rlwinm r11,r29,0,8,8
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 0) & 0x800000;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a7fbfc
	if (ctx.cr6.eq) goto loc_82A7FBFC;
	// li r11,1
	ctx.r11.s64 = 1;
	// stw r30,84(r31)
	REX_STORE_U32(ctx.r31.u32 + 84, ctx.r30.u32);
	// li r6,1
	ctx.r6.s64 = 1;
	// li r5,0
	ctx.r5.s64 = 0;
	// li r4,0
	ctx.r4.s64 = 0;
	// stw r11,120(r31)
	REX_STORE_U32(ctx.r31.u32 + 120, ctx.r11.u32);
	// bl 0x82a127e0
	ctx.lr = 0x82A7FB98;
	SetFilePointer(ctx, base);
	// stw r3,124(r31)
	REX_STORE_U32(ctx.r31.u32 + 124, ctx.r3.u32);
loc_82A7FB9C:
	// lis r5,-32088
	ctx.r5.s64 = -2102919168;
	// lis r6,-32088
	ctx.r6.s64 = -2102919168;
	// lis r7,-32088
	ctx.r7.s64 = -2102919168;
	// lis r8,-32088
	ctx.r8.s64 = -2102919168;
	// lis r9,-32088
	ctx.r9.s64 = -2102919168;
	// lis r10,-32088
	ctx.r10.s64 = -2102919168;
	// lis r11,-32088
	ctx.r11.s64 = -2102919168;
	// addi r5,r5,-3640
	ctx.r5.s64 = ctx.r5.s64 + -3640;
	// addi r6,r6,-3224
	ctx.r6.s64 = ctx.r6.s64 + -3224;
	// addi r7,r7,-2304
	ctx.r7.s64 = ctx.r7.s64 + -2304;
	// addi r8,r8,-2264
	ctx.r8.s64 = ctx.r8.s64 + -2264;
	// addi r9,r9,-2016
	ctx.r9.s64 = ctx.r9.s64 + -2016;
	// addi r10,r10,-2120
	ctx.r10.s64 = ctx.r10.s64 + -2120;
	// stw r5,0(r31)
	REX_STORE_U32(ctx.r31.u32 + 0, ctx.r5.u32);
	// addi r11,r11,-1400
	ctx.r11.s64 = ctx.r11.s64 + -1400;
	// stw r6,4(r31)
	REX_STORE_U32(ctx.r31.u32 + 4, ctx.r6.u32);
	// stw r7,8(r31)
	REX_STORE_U32(ctx.r31.u32 + 8, ctx.r7.u32);
	// li r3,1
	ctx.r3.s64 = 1;
	// stw r8,12(r31)
	REX_STORE_U32(ctx.r31.u32 + 12, ctx.r8.u32);
	// stw r9,16(r31)
	REX_STORE_U32(ctx.r31.u32 + 16, ctx.r9.u32);
	// stw r10,20(r31)
	REX_STORE_U32(ctx.r31.u32 + 20, ctx.r10.u32);
	// stw r11,24(r31)
	REX_STORE_U32(ctx.r31.u32 + 24, ctx.r11.u32);
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
loc_82A7FBFC:
	// lis r8,2048
	ctx.r8.s64 = 134217728;
	// li r9,0
	ctx.r9.s64 = 0;
	// ori r8,r8,128
	ctx.r8.u64 = ctx.r8.u64 | 128;
	// li r7,3
	ctx.r7.s64 = 3;
	// li r6,0
	ctx.r6.s64 = 0;
	// li r5,1
	ctx.r5.s64 = 1;
	// lis r4,-32768
	ctx.r4.s64 = -2147483648;
	// bl 0x82a131b0
	ctx.lr = 0x82A7FC1C;
	CreateFileA(ctx, base);
	// stw r3,84(r31)
	REX_STORE_U32(ctx.r31.u32 + 84, ctx.r3.u32);
	// lwz r11,84(r31)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r31.u32 + 84);
	// cmpwi cr6,r11,-1
	ctx.cr6.compare<int32_t>(ctx.r11.s32, -1, ctx.xer);
	// bne cr6,0x82a7fb9c
	if (!ctx.cr6.eq) goto loc_82A7FB9C;
	// li r3,0
	ctx.r3.s64 = 0;
	// addi r1,r1,112
	ctx.r1.s64 = ctx.r1.s64 + 112;
	// b 0x829ff81c
	__restgprlr_29(ctx, base);
	return;
}

DEFINE_REX_FUNC(sub_82A7FC38) {
	REX_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c8
	ctx.lr = 0x82A7FC40;
	__savegprlr_28(ctx, base);
	// addi r12,r1,-40
	ctx.r12.s64 = ctx.r1.s64 + -40;
	// bl 0x82a01320
	ctx.lr = 0x82A7FC48;
	__savefpr_26(ctx, base);
	// stwu r1,-176(r1)
	ea = -176 + ctx.r1.u32;
	REX_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// li r11,1
	ctx.r11.s64 = 1;
	// stw r3,0(r4)
	REX_STORE_U32(ctx.r4.u32 + 0, ctx.r3.u32);
	// mr r30,r5
	ctx.r30.u64 = ctx.r5.u64;
	// cmpwi cr6,r3,2
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 2, ctx.xer);
	// stw r11,4(r4)
	REX_STORE_U32(ctx.r4.u32 + 4, ctx.r11.u32);
	// ble cr6,0x82a7fe48
	if (!ctx.cr6.gt) goto loc_82A7FE48;
	// srawi r31,r3,1
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0x1) != 0);
	ctx.r31.s64 = ctx.r3.s32 >> 1;
	// extsw r11,r31
	ctx.r11.s64 = ctx.r31.s32;
	// std r11,80(r1)
	REX_STORE_U64(ctx.r1.u32 + 80, ctx.r11.u64);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f13,-30204(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -30204);
	ctx.f13.f64 = double(temp.f32);
	// lfd f0,80(r1)
	ctx.f0.u64 = REX_LOAD_U64(ctx.r1.u32 + 80);
	// fcfid f0,f0
	ctx.f0.f64 = double(ctx.f0.s64);
	// frsp f0,f0
	ctx.f0.f64 = double(float(ctx.f0.f64));
	// fdivs f30,f13,f0
	ctx.f30.f64 = double(float(ctx.f13.f64 / ctx.f0.f64));
	// fmuls f1,f0,f30
	ctx.f1.f64 = double(float(ctx.f0.f64 * ctx.f30.f64));
	// bl 0x829ffe18
	ctx.lr = 0x82A7FC90;
	sub_829FFE18(ctx, base);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// frsp f26,f1
	ctx.fpscr.disableFlushMode();
	ctx.f26.f64 = double(float(ctx.f1.f64));
	// stfs f26,4(r30)
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r30.u32 + 4, temp.u32);
	// cmpwi cr6,r31,4
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 4, ctx.xer);
	// lfs f27,3400(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 3400);
	ctx.f27.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// stfs f27,0(r30)
	temp.f32 = float(ctx.f27.f64);
	REX_STORE_U32(ctx.r30.u32 + 0, temp.u32);
	// lfs f28,3444(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 3444);
	ctx.f28.f64 = double(temp.f32);
	// blt cr6,0x82a7fcf0
	if (ctx.cr6.lt) goto loc_82A7FCF0;
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f0,3528(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 3528);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f1,f30,f0
	ctx.f1.f64 = double(float(ctx.f30.f64 * ctx.f0.f64));
	// bl 0x829ffe18
	ctx.lr = 0x82A7FCC4;
	sub_829FFE18(ctx, base);
	// fmr f13,f1
	ctx.fpscr.disableFlushMode();
	ctx.f13.f64 = ctx.f1.f64;
	// lis r11,-32252
	ctx.r11.s64 = -2113667072;
	// lfs f0,26808(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 26808);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f1,f30,f0
	ctx.f1.f64 = double(float(ctx.f30.f64 * ctx.f0.f64));
	// frsp f0,f13
	ctx.f0.f64 = double(float(ctx.f13.f64));
	// fdivs f0,f28,f0
	ctx.f0.f64 = double(float(ctx.f28.f64 / ctx.f0.f64));
	// stfs f0,8(r30)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r30.u32 + 8, temp.u32);
	// bl 0x829ffe18
	ctx.lr = 0x82A7FCE4;
	sub_829FFE18(ctx, base);
	// frsp f0,f1
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(float(ctx.f1.f64));
	// fdivs f0,f28,f0
	ctx.f0.f64 = double(float(ctx.f28.f64 / ctx.f0.f64));
	// stfs f0,12(r30)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r30.u32 + 12, temp.u32);
loc_82A7FCF0:
	// li r28,4
	ctx.r28.s64 = 4;
	// cmpwi cr6,r31,4
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 4, ctx.xer);
	// ble cr6,0x82a7fd7c
	if (!ctx.cr6.gt) goto loc_82A7FD7C;
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// addi r29,r30,24
	ctx.r29.s64 = ctx.r30.s64 + 24;
	// lfs f29,-26300(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -26300);
	ctx.f29.f64 = double(temp.f32);
loc_82A7FD08:
	// extsw r11,r28
	ctx.r11.s64 = ctx.r28.s32;
	// std r11,80(r1)
	REX_STORE_U64(ctx.r1.u32 + 80, ctx.r11.u64);
	// lfd f0,80(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f0.u64 = REX_LOAD_U64(ctx.r1.u32 + 80);
	// fcfid f0,f0
	ctx.f0.f64 = double(ctx.f0.s64);
	// frsp f0,f0
	ctx.f0.f64 = double(float(ctx.f0.f64));
	// fmuls f31,f0,f30
	ctx.f31.f64 = double(float(ctx.f0.f64 * ctx.f30.f64));
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// bl 0x829ffe18
	ctx.lr = 0x82A7FD28;
	sub_829FFE18(ctx, base);
	// fmr f0,f1
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = ctx.f1.f64;
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// frsp f0,f0
	ctx.f0.f64 = double(float(ctx.f0.f64));
	// stfs f0,-8(r29)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r29.u32 + -8, temp.u32);
	// bl 0x829ffd48
	ctx.lr = 0x82A7FD3C;
	sub_829FFD48(ctx, base);
	// fmuls f31,f31,f29
	ctx.fpscr.disableFlushMode();
	ctx.f31.f64 = double(float(ctx.f31.f64 * ctx.f29.f64));
	// frsp f0,f1
	ctx.f0.f64 = double(float(ctx.f1.f64));
	// stfs f0,-4(r29)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r29.u32 + -4, temp.u32);
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// bl 0x829ffe18
	ctx.lr = 0x82A7FD50;
	sub_829FFE18(ctx, base);
	// fmr f0,f1
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = ctx.f1.f64;
	// fmr f1,f31
	ctx.f1.f64 = ctx.f31.f64;
	// frsp f0,f0
	ctx.f0.f64 = double(float(ctx.f0.f64));
	// stfs f0,0(r29)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r29.u32 + 0, temp.u32);
	// bl 0x829ffd48
	ctx.lr = 0x82A7FD64;
	sub_829FFD48(ctx, base);
	// addi r28,r28,4
	ctx.r28.s64 = ctx.r28.s64 + 4;
	// frsp f0,f1
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(float(ctx.f1.f64));
	// stfs f0,4(r29)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r29.u32 + 4, temp.u32);
	// addi r29,r29,16
	ctx.r29.s64 = ctx.r29.s64 + 16;
	// cmpw cr6,r28,r31
	ctx.cr6.compare<int32_t>(ctx.r28.s32, ctx.r31.s32, ctx.xer);
	// blt cr6,0x82a7fd08
	if (ctx.cr6.lt) goto loc_82A7FD08;
loc_82A7FD7C:
	// li r11,0
	ctx.r11.s64 = 0;
	// cmpwi cr6,r31,2
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 2, ctx.xer);
	// ble cr6,0x82a7fe48
	if (!ctx.cr6.gt) goto loc_82A7FE48;
loc_82A7FD88:
	// add r8,r11,r31
	ctx.r8.u64 = ctx.r11.u64 + ctx.r31.u64;
	// srawi r31,r31,1
	ctx.xer.ca = (ctx.r31.s32 < 0) & ((ctx.r31.u32 & 0x1) != 0);
	ctx.r31.s64 = ctx.r31.s32 >> 1;
	// rlwinm r10,r8,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 2) & 0xFFFFFFFC;
	// cmpwi cr6,r31,4
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 4, ctx.xer);
	// add r10,r10,r30
	ctx.r10.u64 = ctx.r10.u64 + ctx.r30.u64;
	// stfs f27,0(r10)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f27.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// stfs f26,4(r10)
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// blt cr6,0x82a7fe3c
	if (ctx.cr6.lt) goto loc_82A7FE3C;
	// addi r10,r11,6
	ctx.r10.s64 = ctx.r11.s64 + 6;
	// addi r9,r11,4
	ctx.r9.s64 = ctx.r11.s64 + 4;
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r9,r9,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r7,r8,2
	ctx.r7.s64 = ctx.r8.s64 + 2;
	// addi r6,r8,3
	ctx.r6.s64 = ctx.r8.s64 + 3;
	// rlwinm r7,r7,2,0,29
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
	// lfsx f0,r10,r30
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + ctx.r30.u32);
	ctx.f0.f64 = double(temp.f32);
	// rlwinm r10,r6,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
	// lfsx f13,r9,r30
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + ctx.r30.u32);
	ctx.f13.f64 = double(temp.f32);
	// fdivs f0,f28,f0
	ctx.f0.f64 = double(float(ctx.f28.f64 / ctx.f0.f64));
	// fdivs f13,f28,f13
	ctx.f13.f64 = double(float(ctx.f28.f64 / ctx.f13.f64));
	// stfsx f13,r7,r30
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r7.u32 + ctx.r30.u32, temp.u32);
	// stfsx f0,r10,r30
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r10.u32 + ctx.r30.u32, temp.u32);
	// ble cr6,0x82a7fe3c
	if (!ctx.cr6.gt) goto loc_82A7FE3C;
	// addi r11,r11,10
	ctx.r11.s64 = ctx.r11.s64 + 10;
	// addi r10,r8,4
	ctx.r10.s64 = ctx.r8.s64 + 4;
	// addi r9,r31,-5
	ctx.r9.s64 = ctx.r31.s64 + -5;
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r9,r9,30,2,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 30) & 0x3FFFFFFF;
	// add r10,r10,r30
	ctx.r10.u64 = ctx.r10.u64 + ctx.r30.u64;
	// add r11,r11,r30
	ctx.r11.u64 = ctx.r11.u64 + ctx.r30.u64;
	// addi r9,r9,1
	ctx.r9.s64 = ctx.r9.s64 + 1;
loc_82A7FE08:
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
	// lfs f0,-4(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -4);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,0(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,4(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// lfs f11,-8(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -8);
	ctx.f11.f64 = double(temp.f32);
	// addi r11,r11,32
	ctx.r11.s64 = ctx.r11.s64 + 32;
	// stfs f11,0(r10)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// stfs f0,4(r10)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// stfs f13,8(r10)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r10.u32 + 8, temp.u32);
	// stfs f12,12(r10)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r10.u32 + 12, temp.u32);
	// addi r10,r10,16
	ctx.r10.s64 = ctx.r10.s64 + 16;
	// bne cr6,0x82a7fe08
	if (!ctx.cr6.eq) goto loc_82A7FE08;
loc_82A7FE3C:
	// mr r11,r8
	ctx.r11.u64 = ctx.r8.u64;
	// cmpwi cr6,r31,2
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 2, ctx.xer);
	// bgt cr6,0x82a7fd88
	if (ctx.cr6.gt) goto loc_82A7FD88;
loc_82A7FE48:
	// addi r1,r1,176
	ctx.r1.s64 = ctx.r1.s64 + 176;
	// addi r12,r1,-40
	ctx.r12.s64 = ctx.r1.s64 + -40;
	// bl 0x82a0136c
	ctx.lr = 0x82A7FE54;
	__restfpr_26(ctx, base);
	// b 0x829ff818
	__restgprlr_28(ctx, base);
	return;
}

DEFINE_REX_FUNC(sub_82A7FE58) {
	REX_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c8
	ctx.lr = 0x82A7FE60;
	__savegprlr_28(ctx, base);
	// stfd f29,-64(r1)
	ctx.fpscr.disableFlushMode();
	REX_STORE_U64(ctx.r1.u32 + -64, ctx.f29.u64);
	// stfd f30,-56(r1)
	REX_STORE_U64(ctx.r1.u32 + -56, ctx.f30.u64);
	// stfd f31,-48(r1)
	REX_STORE_U64(ctx.r1.u32 + -48, ctx.f31.u64);
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	REX_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r30,r3
	ctx.r30.u64 = ctx.r3.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// cmpwi cr6,r30,1
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 1, ctx.xer);
	// stw r30,4(r4)
	REX_STORE_U32(ctx.r4.u32 + 4, ctx.r30.u32);
	// ble cr6,0x82a7ff40
	if (!ctx.cr6.gt) goto loc_82A7FF40;
	// srawi r28,r30,1
	ctx.xer.ca = (ctx.r30.s32 < 0) & ((ctx.r30.u32 & 0x1) != 0);
	ctx.r28.s64 = ctx.r30.s32 >> 1;
	// extsw r11,r28
	ctx.r11.s64 = ctx.r28.s32;
	// std r11,80(r1)
	REX_STORE_U64(ctx.r1.u32 + 80, ctx.r11.u64);
	// lis r11,-32254
	ctx.r11.s64 = -2113798144;
	// lfs f13,-30204(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -30204);
	ctx.f13.f64 = double(temp.f32);
	// lfd f0,80(r1)
	ctx.f0.u64 = REX_LOAD_U64(ctx.r1.u32 + 80);
	// fcfid f0,f0
	ctx.f0.f64 = double(ctx.f0.s64);
	// frsp f0,f0
	ctx.f0.f64 = double(float(ctx.f0.f64));
	// fdivs f29,f13,f0
	ctx.f29.f64 = double(float(ctx.f13.f64 / ctx.f0.f64));
	// fmuls f1,f0,f29
	ctx.f1.f64 = double(float(ctx.f0.f64 * ctx.f29.f64));
	// bl 0x829ffe18
	ctx.lr = 0x82A7FEB0;
	sub_829FFE18(ctx, base);
	// frsp f0,f1
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(float(ctx.f1.f64));
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// rlwinm r10,r28,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r28.u32 | (ctx.r28.u64 << 32), 2) & 0xFFFFFFFC;
	// stfs f0,0(r29)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r29.u32 + 0, temp.u32);
	// li r31,1
	ctx.r31.s64 = 1;
	// cmpwi cr6,r28,1
	ctx.cr6.compare<int32_t>(ctx.r28.s32, 1, ctx.xer);
	// lfs f31,3444(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 3444);
	ctx.f31.f64 = double(temp.f32);
	// fmuls f0,f0,f31
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f31.f64));
	// stfsx f0,r10,r29
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r10.u32 + ctx.r29.u32, temp.u32);
	// ble cr6,0x82a7ff40
	if (!ctx.cr6.gt) goto loc_82A7FF40;
	// rlwinm r11,r30,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r30,r29,4
	ctx.r30.s64 = ctx.r29.s64 + 4;
	// add r11,r11,r29
	ctx.r11.u64 = ctx.r11.u64 + ctx.r29.u64;
	// addi r29,r11,-4
	ctx.r29.s64 = ctx.r11.s64 + -4;
loc_82A7FEE8:
	// extsw r11,r31
	ctx.r11.s64 = ctx.r31.s32;
	// std r11,80(r1)
	REX_STORE_U64(ctx.r1.u32 + 80, ctx.r11.u64);
	// lfd f0,80(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f0.u64 = REX_LOAD_U64(ctx.r1.u32 + 80);
	// fcfid f0,f0
	ctx.f0.f64 = double(ctx.f0.s64);
	// frsp f0,f0
	ctx.f0.f64 = double(float(ctx.f0.f64));
	// fmuls f30,f0,f29
	ctx.f30.f64 = double(float(ctx.f0.f64 * ctx.f29.f64));
	// fmr f1,f30
	ctx.f1.f64 = ctx.f30.f64;
	// bl 0x829ffe18
	ctx.lr = 0x82A7FF08;
	sub_829FFE18(ctx, base);
	// fmr f0,f1
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = ctx.f1.f64;
	// fmr f1,f30
	ctx.f1.f64 = ctx.f30.f64;
	// frsp f0,f0
	ctx.f0.f64 = double(float(ctx.f0.f64));
	// fmuls f0,f0,f31
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f31.f64));
	// stfs f0,0(r30)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r30.u32 + 0, temp.u32);
	// bl 0x829ffd48
	ctx.lr = 0x82A7FF20;
	sub_829FFD48(ctx, base);
	// frsp f0,f1
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(float(ctx.f1.f64));
	// addi r31,r31,1
	ctx.r31.s64 = ctx.r31.s64 + 1;
	// addi r30,r30,4
	ctx.r30.s64 = ctx.r30.s64 + 4;
	// cmpw cr6,r31,r28
	ctx.cr6.compare<int32_t>(ctx.r31.s32, ctx.r28.s32, ctx.xer);
	// fmuls f0,f0,f31
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f31.f64));
	// stfs f0,0(r29)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r29.u32 + 0, temp.u32);
	// addi r29,r29,-4
	ctx.r29.s64 = ctx.r29.s64 + -4;
	// blt cr6,0x82a7fee8
	if (ctx.cr6.lt) goto loc_82A7FEE8;
loc_82A7FF40:
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// lfd f29,-64(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f29.u64 = REX_LOAD_U64(ctx.r1.u32 + -64);
	// lfd f30,-56(r1)
	ctx.f30.u64 = REX_LOAD_U64(ctx.r1.u32 + -56);
	// lfd f31,-48(r1)
	ctx.f31.u64 = REX_LOAD_U64(ctx.r1.u32 + -48);
	// b 0x829ff818
	__restgprlr_28(ctx, base);
	return;
}

DEFINE_REX_FUNC(sub_82A7FF58) {
	REX_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7b4
	ctx.lr = 0x82A7FF60;
	__savegprlr_23(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// mr r7,r3
	ctx.r7.u64 = ctx.r3.u64;
	// li r23,1
	ctx.r23.s64 = 1;
	// cmpwi cr6,r7,8
	ctx.cr6.compare<int32_t>(ctx.r7.s32, 8, ctx.xer);
	// stw r11,0(r4)
	REX_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
	// ble cr6,0x82a7ffc4
	if (!ctx.cr6.gt) goto loc_82A7FFC4;
loc_82A7FF78:
	// cmpwi cr6,r23,0
	ctx.cr6.compare<int32_t>(ctx.r23.s32, 0, ctx.xer);
	// srawi r7,r7,1
	ctx.xer.ca = (ctx.r7.s32 < 0) & ((ctx.r7.u32 & 0x1) != 0);
	ctx.r7.s64 = ctx.r7.s32 >> 1;
	// ble cr6,0x82a7ffb4
	if (!ctx.cr6.gt) goto loc_82A7FFB4;
	// rlwinm r11,r23,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r23.u32 | (ctx.r23.u64 << 32), 2) & 0xFFFFFFFC;
	// mr r10,r4
	ctx.r10.u64 = ctx.r4.u64;
	// add r9,r11,r4
	ctx.r9.u64 = ctx.r11.u64 + ctx.r4.u64;
	// mr r11,r23
	ctx.r11.u64 = ctx.r23.u64;
loc_82A7FF94:
	// lwz r8,0(r10)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r10.u32 + 0);
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// addi r10,r10,4
	ctx.r10.s64 = ctx.r10.s64 + 4;
	// add r8,r8,r7
	ctx.r8.u64 = ctx.r8.u64 + ctx.r7.u64;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// stw r8,0(r9)
	REX_STORE_U32(ctx.r9.u32 + 0, ctx.r8.u32);
	// addi r9,r9,4
	ctx.r9.s64 = ctx.r9.s64 + 4;
	// bne cr6,0x82a7ff94
	if (!ctx.cr6.eq) goto loc_82A7FF94;
loc_82A7FFB4:
	// rlwinm r23,r23,1,0,30
	ctx.r23.u64 = __builtin_rotateleft64(ctx.r23.u32 | (ctx.r23.u64 << 32), 1) & 0xFFFFFFFE;
	// rlwinm r11,r23,3,0,28
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r23.u32 | (ctx.r23.u64 << 32), 3) & 0xFFFFFFF8;
	// cmpw cr6,r11,r7
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r7.s32, ctx.xer);
	// blt cr6,0x82a7ff78
	if (ctx.cr6.lt) goto loc_82A7FF78;
loc_82A7FFC4:
	// rlwinm r10,r23,3,0,28
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r23.u32 | (ctx.r23.u64 << 32), 3) & 0xFFFFFFF8;
	// rlwinm r11,r23,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r23.u32 | (ctx.r23.u64 << 32), 1) & 0xFFFFFFFE;
	// cmpw cr6,r10,r7
	ctx.cr6.compare<int32_t>(ctx.r10.s32, ctx.r7.s32, ctx.xer);
	// bne cr6,0x82a80154
	if (!ctx.cr6.eq) goto loc_82A80154;
	// li r24,0
	ctx.r24.s64 = 0;
	// cmpwi cr6,r23,0
	ctx.cr6.compare<int32_t>(ctx.r23.s32, 0, ctx.xer);
	// ble cr6,0x82a80444
	if (!ctx.cr6.gt) goto loc_82A80444;
	// li r25,0
	ctx.r25.s64 = 0;
	// mr r26,r4
	ctx.r26.u64 = ctx.r4.u64;
loc_82A7FFE8:
	// cmpwi cr6,r24,0
	ctx.cr6.compare<int32_t>(ctx.r24.s32, 0, ctx.xer);
	// ble cr6,0x82a800fc
	if (!ctx.cr6.gt) goto loc_82A800FC;
	// rlwinm r30,r11,1,0,30
	ctx.r30.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// li r6,0
	ctx.r6.s64 = 0;
	// mr r7,r4
	ctx.r7.u64 = ctx.r4.u64;
	// mr r8,r24
	ctx.r8.u64 = ctx.r24.u64;
loc_82A80000:
	// lwz r9,0(r7)
	ctx.r9.u64 = REX_LOAD_U32(ctx.r7.u32 + 0);
	// addi r8,r8,-1
	ctx.r8.s64 = ctx.r8.s64 + -1;
	// lwz r10,0(r26)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r26.u32 + 0);
	// addi r7,r7,4
	ctx.r7.s64 = ctx.r7.s64 + 4;
	// add r3,r25,r9
	ctx.r3.u64 = ctx.r25.u64 + ctx.r9.u64;
	// add r31,r6,r10
	ctx.r31.u64 = ctx.r6.u64 + ctx.r10.u64;
	// rlwinm r10,r3,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r9,r31,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 2) & 0xFFFFFFFC;
	// add r10,r10,r5
	ctx.r10.u64 = ctx.r10.u64 + ctx.r5.u64;
	// add r9,r9,r5
	ctx.r9.u64 = ctx.r9.u64 + ctx.r5.u64;
	// add r3,r30,r3
	ctx.r3.u64 = ctx.r30.u64 + ctx.r3.u64;
	// add r31,r31,r11
	ctx.r31.u64 = ctx.r31.u64 + ctx.r11.u64;
	// rlwinm r28,r3,2,0,29
	ctx.r28.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f0,4(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// rlwinm r29,r31,2,0,29
	ctx.r29.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f13,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// subf r3,r11,r3
	ctx.r3.u64 = ctx.r3.u64 - ctx.r11.u64;
	// lfs f12,4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// add r31,r31,r11
	ctx.r31.u64 = ctx.r31.u64 + ctx.r11.u64;
	// lfs f11,0(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// rlwinm r27,r3,2,0,29
	ctx.r27.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// stfs f11,0(r9)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// add r3,r30,r3
	ctx.r3.u64 = ctx.r30.u64 + ctx.r3.u64;
	// stfs f0,4(r9)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r9.u32 + 4, temp.u32);
	// add r9,r29,r5
	ctx.r9.u64 = ctx.r29.u64 + ctx.r5.u64;
	// stfs f13,0(r10)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// rlwinm r29,r3,2,0,29
	ctx.r29.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// stfs f12,4(r10)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// add r10,r28,r5
	ctx.r10.u64 = ctx.r28.u64 + ctx.r5.u64;
	// rlwinm r28,r31,2,0,29
	ctx.r28.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 2) & 0xFFFFFFFC;
	// add r31,r31,r11
	ctx.r31.u64 = ctx.r31.u64 + ctx.r11.u64;
	// lfs f13,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// addi r6,r6,2
	ctx.r6.s64 = ctx.r6.s64 + 2;
	// lfs f12,4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// rlwinm r3,r31,2,0,29
	ctx.r3.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f0,4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// cmplwi cr6,r8,0
	ctx.cr6.compare<uint32_t>(ctx.r8.u32, 0, ctx.xer);
	// lfs f11,0(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// stfs f11,0(r9)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// stfs f0,4(r9)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r9.u32 + 4, temp.u32);
	// add r9,r28,r5
	ctx.r9.u64 = ctx.r28.u64 + ctx.r5.u64;
	// stfs f13,0(r10)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// stfs f12,4(r10)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// add r10,r27,r5
	ctx.r10.u64 = ctx.r27.u64 + ctx.r5.u64;
	// lfs f13,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// lfs f0,4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lfs f11,0(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// stfs f11,0(r9)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// stfs f0,4(r9)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r9.u32 + 4, temp.u32);
	// add r9,r3,r5
	ctx.r9.u64 = ctx.r3.u64 + ctx.r5.u64;
	// stfs f13,0(r10)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// stfs f12,4(r10)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// add r10,r29,r5
	ctx.r10.u64 = ctx.r29.u64 + ctx.r5.u64;
	// lfs f13,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// lfs f0,4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lfs f11,0(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// stfs f11,0(r9)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// stfs f0,4(r9)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r9.u32 + 4, temp.u32);
	// stfs f13,0(r10)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// stfs f12,4(r10)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// bne cr6,0x82a80000
	if (!ctx.cr6.eq) goto loc_82A80000;
loc_82A800FC:
	// lwz r10,0(r26)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r26.u32 + 0);
	// addi r24,r24,1
	ctx.r24.s64 = ctx.r24.s64 + 1;
	// addi r26,r26,4
	ctx.r26.s64 = ctx.r26.s64 + 4;
	// add r10,r25,r10
	ctx.r10.u64 = ctx.r25.u64 + ctx.r10.u64;
	// addi r25,r25,2
	ctx.r25.s64 = ctx.r25.s64 + 2;
	// add r10,r10,r11
	ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
	// cmpw cr6,r24,r23
	ctx.cr6.compare<int32_t>(ctx.r24.s32, ctx.r23.s32, ctx.xer);
	// add r9,r10,r11
	ctx.r9.u64 = ctx.r10.u64 + ctx.r11.u64;
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r9,r9,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// add r10,r10,r5
	ctx.r10.u64 = ctx.r10.u64 + ctx.r5.u64;
	// add r9,r9,r5
	ctx.r9.u64 = ctx.r9.u64 + ctx.r5.u64;
	// lfs f0,0(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// lfs f11,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// stfs f11,0(r10)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// stfs f12,4(r10)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// stfs f0,0(r9)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// stfs f13,4(r9)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r9.u32 + 4, temp.u32);
	// blt cr6,0x82a7ffe8
	if (ctx.cr6.lt) goto loc_82A7FFE8;
	// b 0x829ff804
	__restgprlr_23(ctx, base);
	return;
loc_82A80154:
	// li r25,1
	ctx.r25.s64 = 1;
	// cmpwi cr6,r23,1
	ctx.cr6.compare<int32_t>(ctx.r23.s32, 1, ctx.xer);
	// ble cr6,0x82a80444
	if (!ctx.cr6.gt) goto loc_82A80444;
	// li r3,2
	ctx.r3.s64 = 2;
	// addi r6,r4,4
	ctx.r6.s64 = ctx.r4.s64 + 4;
loc_82A80168:
	// li r26,0
	ctx.r26.s64 = 0;
	// cmpwi cr6,r25,4
	ctx.cr6.compare<int32_t>(ctx.r25.s32, 4, ctx.xer);
	// blt cr6,0x82a8038c
	if (ctx.cr6.lt) goto loc_82A8038C;
	// addi r10,r25,-4
	ctx.r10.s64 = ctx.r25.s64 + -4;
	// li r9,0
	ctx.r9.s64 = 0;
	// rlwinm r8,r10,30,2,31
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 30) & 0x3FFFFFFF;
	// addi r10,r4,8
	ctx.r10.s64 = ctx.r4.s64 + 8;
	// addi r31,r8,1
	ctx.r31.s64 = ctx.r8.s64 + 1;
	// rlwinm r26,r31,2,0,29
	ctx.r26.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 2) & 0xFFFFFFFC;
loc_82A8018C:
	// lwz r7,-8(r10)
	ctx.r7.u64 = REX_LOAD_U32(ctx.r10.u32 + -8);
	// lwz r8,0(r6)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r6.u32 + 0);
	// add r30,r7,r3
	ctx.r30.u64 = ctx.r7.u64 + ctx.r3.u64;
	// add r29,r8,r9
	ctx.r29.u64 = ctx.r8.u64 + ctx.r9.u64;
	// rlwinm r8,r30,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r7,r29,2,0,29
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 2) & 0xFFFFFFFC;
	// add r8,r8,r5
	ctx.r8.u64 = ctx.r8.u64 + ctx.r5.u64;
	// add r7,r7,r5
	ctx.r7.u64 = ctx.r7.u64 + ctx.r5.u64;
	// add r30,r30,r11
	ctx.r30.u64 = ctx.r30.u64 + ctx.r11.u64;
	// add r29,r29,r11
	ctx.r29.u64 = ctx.r29.u64 + ctx.r11.u64;
	// rlwinm r28,r30,2,0,29
	ctx.r28.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f0,4(r8)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// rlwinm r30,r29,2,0,29
	ctx.r30.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f13,0(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,4(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// lfs f11,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// stfs f11,0(r7)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r7.u32 + 0, temp.u32);
	// stfs f0,4(r7)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r7.u32 + 4, temp.u32);
	// add r7,r30,r5
	ctx.r7.u64 = ctx.r30.u64 + ctx.r5.u64;
	// stfs f13,0(r8)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// stfs f12,4(r8)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r8.u32 + 4, temp.u32);
	// add r8,r28,r5
	ctx.r8.u64 = ctx.r28.u64 + ctx.r5.u64;
	// lfs f13,0(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,4(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// lfs f0,4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lfs f11,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// stfs f11,0(r7)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r7.u32 + 0, temp.u32);
	// stfs f0,4(r7)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r7.u32 + 4, temp.u32);
	// stfs f13,0(r8)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// stfs f12,4(r8)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r8.u32 + 4, temp.u32);
	// lwz r8,0(r6)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r6.u32 + 0);
	// lwz r7,-4(r10)
	ctx.r7.u64 = REX_LOAD_U32(ctx.r10.u32 + -4);
	// add r8,r8,r9
	ctx.r8.u64 = ctx.r8.u64 + ctx.r9.u64;
	// add r30,r7,r3
	ctx.r30.u64 = ctx.r7.u64 + ctx.r3.u64;
	// addi r29,r8,2
	ctx.r29.s64 = ctx.r8.s64 + 2;
	// rlwinm r8,r30,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r7,r29,2,0,29
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 2) & 0xFFFFFFFC;
	// add r8,r8,r5
	ctx.r8.u64 = ctx.r8.u64 + ctx.r5.u64;
	// add r7,r7,r5
	ctx.r7.u64 = ctx.r7.u64 + ctx.r5.u64;
	// add r30,r30,r11
	ctx.r30.u64 = ctx.r30.u64 + ctx.r11.u64;
	// add r29,r29,r11
	ctx.r29.u64 = ctx.r29.u64 + ctx.r11.u64;
	// rlwinm r28,r30,2,0,29
	ctx.r28.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f0,4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// rlwinm r30,r29,2,0,29
	ctx.r30.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f13,0(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,4(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// lfs f11,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// stfs f11,0(r7)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r7.u32 + 0, temp.u32);
	// stfs f0,4(r7)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r7.u32 + 4, temp.u32);
	// add r7,r30,r5
	ctx.r7.u64 = ctx.r30.u64 + ctx.r5.u64;
	// stfs f13,0(r8)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// addi r30,r9,6
	ctx.r30.s64 = ctx.r9.s64 + 6;
	// stfs f12,4(r8)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r8.u32 + 4, temp.u32);
	// add r8,r28,r5
	ctx.r8.u64 = ctx.r28.u64 + ctx.r5.u64;
	// lfs f13,0(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,4(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// lfs f0,4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lfs f11,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// stfs f11,0(r7)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r7.u32 + 0, temp.u32);
	// stfs f0,4(r7)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r7.u32 + 4, temp.u32);
	// stfs f13,0(r8)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// stfs f12,4(r8)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r8.u32 + 4, temp.u32);
	// lwz r7,0(r6)
	ctx.r7.u64 = REX_LOAD_U32(ctx.r6.u32 + 0);
	// lwz r8,0(r10)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r10.u32 + 0);
	// add r7,r7,r30
	ctx.r7.u64 = ctx.r7.u64 + ctx.r30.u64;
	// add r8,r8,r3
	ctx.r8.u64 = ctx.r8.u64 + ctx.r3.u64;
	// addi r7,r7,-2
	ctx.r7.s64 = ctx.r7.s64 + -2;
	// rlwinm r28,r8,2,0,29
	ctx.r28.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 2) & 0xFFFFFFFC;
	// add r29,r8,r11
	ctx.r29.u64 = ctx.r8.u64 + ctx.r11.u64;
	// rlwinm r27,r7,2,0,29
	ctx.r27.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
	// add r8,r28,r5
	ctx.r8.u64 = ctx.r28.u64 + ctx.r5.u64;
	// add r28,r7,r11
	ctx.r28.u64 = ctx.r7.u64 + ctx.r11.u64;
	// add r7,r27,r5
	ctx.r7.u64 = ctx.r27.u64 + ctx.r5.u64;
	// rlwinm r29,r29,2,0,29
	ctx.r29.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r28,r28,2,0,29
	ctx.r28.u64 = __builtin_rotateleft64(ctx.r28.u32 | (ctx.r28.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f0,4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lfs f11,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// lfs f13,0(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,4(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// stfs f11,0(r7)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r7.u32 + 0, temp.u32);
	// stfs f0,4(r7)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r7.u32 + 4, temp.u32);
	// stfs f13,0(r8)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// add r7,r28,r5
	ctx.r7.u64 = ctx.r28.u64 + ctx.r5.u64;
	// stfs f12,4(r8)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r8.u32 + 4, temp.u32);
	// add r8,r29,r5
	ctx.r8.u64 = ctx.r29.u64 + ctx.r5.u64;
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// addi r9,r9,8
	ctx.r9.s64 = ctx.r9.s64 + 8;
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// lfs f0,0(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,4(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// lfs f11,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// stfs f11,0(r7)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r7.u32 + 0, temp.u32);
	// stfs f12,4(r7)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r7.u32 + 4, temp.u32);
	// stfs f0,0(r8)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// stfs f13,4(r8)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r8.u32 + 4, temp.u32);
	// lwz r7,4(r10)
	ctx.r7.u64 = REX_LOAD_U32(ctx.r10.u32 + 4);
	// lwz r8,0(r6)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r6.u32 + 0);
	// addi r10,r10,16
	ctx.r10.s64 = ctx.r10.s64 + 16;
	// add r29,r7,r3
	ctx.r29.u64 = ctx.r7.u64 + ctx.r3.u64;
	// add r30,r8,r30
	ctx.r30.u64 = ctx.r8.u64 + ctx.r30.u64;
	// rlwinm r8,r29,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r7,r30,2,0,29
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// add r8,r8,r5
	ctx.r8.u64 = ctx.r8.u64 + ctx.r5.u64;
	// add r7,r7,r5
	ctx.r7.u64 = ctx.r7.u64 + ctx.r5.u64;
	// add r29,r29,r11
	ctx.r29.u64 = ctx.r29.u64 + ctx.r11.u64;
	// add r30,r30,r11
	ctx.r30.u64 = ctx.r30.u64 + ctx.r11.u64;
	// rlwinm r29,r29,2,0,29
	ctx.r29.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f0,4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// rlwinm r30,r30,2,0,29
	ctx.r30.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f13,0(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,4(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// lfs f11,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// stfs f11,0(r7)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r7.u32 + 0, temp.u32);
	// stfs f0,4(r7)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r7.u32 + 4, temp.u32);
	// add r7,r30,r5
	ctx.r7.u64 = ctx.r30.u64 + ctx.r5.u64;
	// stfs f13,0(r8)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// stfs f12,4(r8)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r8.u32 + 4, temp.u32);
	// add r8,r29,r5
	ctx.r8.u64 = ctx.r29.u64 + ctx.r5.u64;
	// lfs f13,0(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,4(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// lfs f0,4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lfs f11,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// stfs f11,0(r7)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r7.u32 + 0, temp.u32);
	// stfs f0,4(r7)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r7.u32 + 4, temp.u32);
	// stfs f13,0(r8)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// stfs f12,4(r8)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r8.u32 + 4, temp.u32);
	// bne cr6,0x82a8018c
	if (!ctx.cr6.eq) goto loc_82A8018C;
loc_82A8038C:
	// cmpw cr6,r26,r25
	ctx.cr6.compare<int32_t>(ctx.r26.s32, ctx.r25.s32, ctx.xer);
	// bge cr6,0x82a80430
	if (!ctx.cr6.lt) goto loc_82A80430;
	// rlwinm r10,r26,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r26.u32 | (ctx.r26.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r31,r26,1,0,30
	ctx.r31.u64 = __builtin_rotateleft64(ctx.r26.u32 | (ctx.r26.u64 << 32), 1) & 0xFFFFFFFE;
	// add r7,r10,r4
	ctx.r7.u64 = ctx.r10.u64 + ctx.r4.u64;
	// subf r8,r26,r25
	ctx.r8.u64 = ctx.r25.u64 - ctx.r26.u64;
loc_82A803A4:
	// lwz r9,0(r7)
	ctx.r9.u64 = REX_LOAD_U32(ctx.r7.u32 + 0);
	// addi r8,r8,-1
	ctx.r8.s64 = ctx.r8.s64 + -1;
	// lwz r10,0(r6)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r6.u32 + 0);
	// addi r7,r7,4
	ctx.r7.s64 = ctx.r7.s64 + 4;
	// add r30,r3,r9
	ctx.r30.u64 = ctx.r3.u64 + ctx.r9.u64;
	// add r29,r10,r31
	ctx.r29.u64 = ctx.r10.u64 + ctx.r31.u64;
	// rlwinm r10,r30,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r9,r29,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 2) & 0xFFFFFFFC;
	// add r10,r10,r5
	ctx.r10.u64 = ctx.r10.u64 + ctx.r5.u64;
	// add r9,r9,r5
	ctx.r9.u64 = ctx.r9.u64 + ctx.r5.u64;
	// add r30,r30,r11
	ctx.r30.u64 = ctx.r30.u64 + ctx.r11.u64;
	// add r29,r29,r11
	ctx.r29.u64 = ctx.r29.u64 + ctx.r11.u64;
	// rlwinm r28,r30,2,0,29
	ctx.r28.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f0,4(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// rlwinm r30,r29,2,0,29
	ctx.r30.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f13,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// addi r31,r31,2
	ctx.r31.s64 = ctx.r31.s64 + 2;
	// lfs f12,4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// cmplwi cr6,r8,0
	ctx.cr6.compare<uint32_t>(ctx.r8.u32, 0, ctx.xer);
	// lfs f11,0(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// stfs f11,0(r9)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// stfs f0,4(r9)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r9.u32 + 4, temp.u32);
	// add r9,r30,r5
	ctx.r9.u64 = ctx.r30.u64 + ctx.r5.u64;
	// stfs f13,0(r10)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// stfs f12,4(r10)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// add r10,r28,r5
	ctx.r10.u64 = ctx.r28.u64 + ctx.r5.u64;
	// lfs f13,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// lfs f0,4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lfs f11,0(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// stfs f11,0(r9)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// stfs f0,4(r9)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r9.u32 + 4, temp.u32);
	// stfs f13,0(r10)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// stfs f12,4(r10)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// bne cr6,0x82a803a4
	if (!ctx.cr6.eq) goto loc_82A803A4;
loc_82A80430:
	// addi r25,r25,1
	ctx.r25.s64 = ctx.r25.s64 + 1;
	// addi r6,r6,4
	ctx.r6.s64 = ctx.r6.s64 + 4;
	// addi r3,r3,2
	ctx.r3.s64 = ctx.r3.s64 + 2;
	// cmpw cr6,r25,r23
	ctx.cr6.compare<int32_t>(ctx.r25.s32, ctx.r23.s32, ctx.xer);
	// blt cr6,0x82a80168
	if (ctx.cr6.lt) goto loc_82A80168;
loc_82A80444:
	// b 0x829ff804
	__restgprlr_23(ctx, base);
	return;
}

DEFINE_REX_FUNC(sub_82A80448) {
	REX_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7b4
	ctx.lr = 0x82A80450;
	__savegprlr_23(ctx, base);
	// li r11,0
	ctx.r11.s64 = 0;
	// mr r7,r3
	ctx.r7.u64 = ctx.r3.u64;
	// li r23,1
	ctx.r23.s64 = 1;
	// cmpwi cr6,r7,8
	ctx.cr6.compare<int32_t>(ctx.r7.s32, 8, ctx.xer);
	// stw r11,0(r4)
	REX_STORE_U32(ctx.r4.u32 + 0, ctx.r11.u32);
	// ble cr6,0x82a804b4
	if (!ctx.cr6.gt) goto loc_82A804B4;
loc_82A80468:
	// cmpwi cr6,r23,0
	ctx.cr6.compare<int32_t>(ctx.r23.s32, 0, ctx.xer);
	// srawi r7,r7,1
	ctx.xer.ca = (ctx.r7.s32 < 0) & ((ctx.r7.u32 & 0x1) != 0);
	ctx.r7.s64 = ctx.r7.s32 >> 1;
	// ble cr6,0x82a804a4
	if (!ctx.cr6.gt) goto loc_82A804A4;
	// rlwinm r11,r23,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r23.u32 | (ctx.r23.u64 << 32), 2) & 0xFFFFFFFC;
	// mr r10,r4
	ctx.r10.u64 = ctx.r4.u64;
	// add r9,r11,r4
	ctx.r9.u64 = ctx.r11.u64 + ctx.r4.u64;
	// mr r11,r23
	ctx.r11.u64 = ctx.r23.u64;
loc_82A80484:
	// lwz r8,0(r10)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r10.u32 + 0);
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// addi r10,r10,4
	ctx.r10.s64 = ctx.r10.s64 + 4;
	// add r8,r8,r7
	ctx.r8.u64 = ctx.r8.u64 + ctx.r7.u64;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// stw r8,0(r9)
	REX_STORE_U32(ctx.r9.u32 + 0, ctx.r8.u32);
	// addi r9,r9,4
	ctx.r9.s64 = ctx.r9.s64 + 4;
	// bne cr6,0x82a80484
	if (!ctx.cr6.eq) goto loc_82A80484;
loc_82A804A4:
	// rlwinm r23,r23,1,0,30
	ctx.r23.u64 = __builtin_rotateleft64(ctx.r23.u32 | (ctx.r23.u64 << 32), 1) & 0xFFFFFFFE;
	// rlwinm r11,r23,3,0,28
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r23.u32 | (ctx.r23.u64 << 32), 3) & 0xFFFFFFF8;
	// cmpw cr6,r11,r7
	ctx.cr6.compare<int32_t>(ctx.r11.s32, ctx.r7.s32, ctx.xer);
	// blt cr6,0x82a80468
	if (ctx.cr6.lt) goto loc_82A80468;
loc_82A804B4:
	// rlwinm r10,r23,3,0,28
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r23.u32 | (ctx.r23.u64 << 32), 3) & 0xFFFFFFF8;
	// rlwinm r11,r23,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r23.u32 | (ctx.r23.u64 << 32), 1) & 0xFFFFFFFE;
	// cmpw cr6,r10,r7
	ctx.cr6.compare<int32_t>(ctx.r10.s32, ctx.r7.s32, ctx.xer);
	// bne cr6,0x82a80698
	if (!ctx.cr6.eq) goto loc_82A80698;
	// li r24,0
	ctx.r24.s64 = 0;
	// cmpwi cr6,r23,0
	ctx.cr6.compare<int32_t>(ctx.r23.s32, 0, ctx.xer);
	// ble cr6,0x82a80a2c
	if (!ctx.cr6.gt) goto loc_82A80A2C;
	// li r25,0
	ctx.r25.s64 = 0;
	// mr r26,r4
	ctx.r26.u64 = ctx.r4.u64;
loc_82A804D8:
	// cmpwi cr6,r24,0
	ctx.cr6.compare<int32_t>(ctx.r24.s32, 0, ctx.xer);
	// ble cr6,0x82a8060c
	if (!ctx.cr6.gt) goto loc_82A8060C;
	// rlwinm r30,r11,1,0,30
	ctx.r30.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// li r7,0
	ctx.r7.s64 = 0;
	// mr r6,r4
	ctx.r6.u64 = ctx.r4.u64;
	// mr r8,r24
	ctx.r8.u64 = ctx.r24.u64;
loc_82A804F0:
	// lwz r9,0(r6)
	ctx.r9.u64 = REX_LOAD_U32(ctx.r6.u32 + 0);
	// addi r8,r8,-1
	ctx.r8.s64 = ctx.r8.s64 + -1;
	// lwz r10,0(r26)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r26.u32 + 0);
	// addi r6,r6,4
	ctx.r6.s64 = ctx.r6.s64 + 4;
	// add r3,r9,r25
	ctx.r3.u64 = ctx.r9.u64 + ctx.r25.u64;
	// add r31,r10,r7
	ctx.r31.u64 = ctx.r10.u64 + ctx.r7.u64;
	// rlwinm r10,r3,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r9,r31,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 2) & 0xFFFFFFFC;
	// add r10,r10,r5
	ctx.r10.u64 = ctx.r10.u64 + ctx.r5.u64;
	// add r9,r9,r5
	ctx.r9.u64 = ctx.r9.u64 + ctx.r5.u64;
	// add r3,r30,r3
	ctx.r3.u64 = ctx.r30.u64 + ctx.r3.u64;
	// add r31,r31,r11
	ctx.r31.u64 = ctx.r31.u64 + ctx.r11.u64;
	// rlwinm r28,r3,2,0,29
	ctx.r28.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f0,4(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// rlwinm r29,r31,2,0,29
	ctx.r29.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f12,4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// fneg f0,f0
	ctx.f0.u64 = ctx.f0.u64 ^ 0x8000000000000000;
	// lfs f13,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// fneg f12,f12
	ctx.f12.u64 = ctx.f12.u64 ^ 0x8000000000000000;
	// lfs f11,0(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// subf r3,r11,r3
	ctx.r3.u64 = ctx.r3.u64 - ctx.r11.u64;
	// stfs f0,4(r9)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r9.u32 + 4, temp.u32);
	// add r31,r31,r11
	ctx.r31.u64 = ctx.r31.u64 + ctx.r11.u64;
	// stfs f11,0(r9)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// add r9,r29,r5
	ctx.r9.u64 = ctx.r29.u64 + ctx.r5.u64;
	// stfs f12,4(r10)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// rlwinm r27,r3,2,0,29
	ctx.r27.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// stfs f13,0(r10)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// add r10,r28,r5
	ctx.r10.u64 = ctx.r28.u64 + ctx.r5.u64;
	// rlwinm r28,r31,2,0,29
	ctx.r28.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 2) & 0xFFFFFFFC;
	// add r3,r30,r3
	ctx.r3.u64 = ctx.r30.u64 + ctx.r3.u64;
	// lfs f12,4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// add r31,r31,r11
	ctx.r31.u64 = ctx.r31.u64 + ctx.r11.u64;
	// lfs f13,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// fneg f12,f12
	ctx.f12.u64 = ctx.f12.u64 ^ 0x8000000000000000;
	// lfs f0,4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// rlwinm r29,r3,2,0,29
	ctx.r29.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f11,0(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// fneg f0,f0
	ctx.f0.u64 = ctx.f0.u64 ^ 0x8000000000000000;
	// stfs f0,4(r9)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r9.u32 + 4, temp.u32);
	// rlwinm r3,r31,2,0,29
	ctx.r3.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 2) & 0xFFFFFFFC;
	// stfs f11,0(r9)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// add r9,r28,r5
	ctx.r9.u64 = ctx.r28.u64 + ctx.r5.u64;
	// stfs f12,4(r10)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// addi r7,r7,2
	ctx.r7.s64 = ctx.r7.s64 + 2;
	// stfs f13,0(r10)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// add r10,r27,r5
	ctx.r10.u64 = ctx.r27.u64 + ctx.r5.u64;
	// cmplwi cr6,r8,0
	ctx.cr6.compare<uint32_t>(ctx.r8.u32, 0, ctx.xer);
	// lfs f12,4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// lfs f13,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// fneg f12,f12
	ctx.f12.u64 = ctx.f12.u64 ^ 0x8000000000000000;
	// lfs f0,4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lfs f11,0(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// fneg f0,f0
	ctx.f0.u64 = ctx.f0.u64 ^ 0x8000000000000000;
	// stfs f0,4(r9)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r9.u32 + 4, temp.u32);
	// stfs f11,0(r9)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// add r9,r3,r5
	ctx.r9.u64 = ctx.r3.u64 + ctx.r5.u64;
	// stfs f12,4(r10)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// stfs f13,0(r10)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// add r10,r29,r5
	ctx.r10.u64 = ctx.r29.u64 + ctx.r5.u64;
	// lfs f12,4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// lfs f13,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// fneg f12,f12
	ctx.f12.u64 = ctx.f12.u64 ^ 0x8000000000000000;
	// lfs f0,4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lfs f11,0(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// fneg f0,f0
	ctx.f0.u64 = ctx.f0.u64 ^ 0x8000000000000000;
	// stfs f11,0(r9)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// stfs f0,4(r9)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r9.u32 + 4, temp.u32);
	// stfs f13,0(r10)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// stfs f12,4(r10)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// bne cr6,0x82a804f0
	if (!ctx.cr6.eq) goto loc_82A804F0;
loc_82A8060C:
	// lwz r10,0(r26)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r26.u32 + 0);
	// addi r24,r24,1
	ctx.r24.s64 = ctx.r24.s64 + 1;
	// addi r26,r26,4
	ctx.r26.s64 = ctx.r26.s64 + 4;
	// add r10,r10,r25
	ctx.r10.u64 = ctx.r10.u64 + ctx.r25.u64;
	// addi r25,r25,2
	ctx.r25.s64 = ctx.r25.s64 + 2;
	// add r9,r10,r11
	ctx.r9.u64 = ctx.r10.u64 + ctx.r11.u64;
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// add r8,r9,r11
	ctx.r8.u64 = ctx.r9.u64 + ctx.r11.u64;
	// rlwinm r7,r10,2,0,29
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r10,r9,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r9,r8,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 2) & 0xFFFFFFFC;
	// add r8,r8,r11
	ctx.r8.u64 = ctx.r8.u64 + ctx.r11.u64;
	// add r9,r9,r5
	ctx.r9.u64 = ctx.r9.u64 + ctx.r5.u64;
	// add r10,r10,r5
	ctx.r10.u64 = ctx.r10.u64 + ctx.r5.u64;
	// lfsx f0,r7,r5
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + ctx.r5.u32);
	ctx.f0.f64 = double(temp.f32);
	// addi r8,r8,1
	ctx.r8.s64 = ctx.r8.s64 + 1;
	// fneg f0,f0
	ctx.f0.u64 = ctx.f0.u64 ^ 0x8000000000000000;
	// stfsx f0,r7,r5
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r7.u32 + ctx.r5.u32, temp.u32);
	// cmpw cr6,r24,r23
	ctx.cr6.compare<int32_t>(ctx.r24.s32, ctx.r23.s32, ctx.xer);
	// rlwinm r8,r8,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f12,4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// lfs f13,4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// fneg f12,f12
	ctx.f12.u64 = ctx.f12.u64 ^ 0x8000000000000000;
	// lfs f0,0(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// fneg f13,f13
	ctx.f13.u64 = ctx.f13.u64 ^ 0x8000000000000000;
	// lfs f11,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// stfs f11,0(r10)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// stfs f12,4(r10)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// stfs f0,0(r9)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// stfs f13,4(r9)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r9.u32 + 4, temp.u32);
	// lfsx f0,r8,r5
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + ctx.r5.u32);
	ctx.f0.f64 = double(temp.f32);
	// fneg f0,f0
	ctx.f0.u64 = ctx.f0.u64 ^ 0x8000000000000000;
	// stfsx f0,r8,r5
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r8.u32 + ctx.r5.u32, temp.u32);
	// blt cr6,0x82a804d8
	if (ctx.cr6.lt) goto loc_82A804D8;
	// b 0x829ff804
	__restgprlr_23(ctx, base);
	return;
loc_82A80698:
	// addi r10,r11,1
	ctx.r10.s64 = ctx.r11.s64 + 1;
	// lfs f0,4(r5)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r5.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// fneg f0,f0
	ctx.f0.u64 = ctx.f0.u64 ^ 0x8000000000000000;
	// stfs f0,4(r5)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r5.u32 + 4, temp.u32);
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// li r25,1
	ctx.r25.s64 = 1;
	// cmpwi cr6,r23,1
	ctx.cr6.compare<int32_t>(ctx.r23.s32, 1, ctx.xer);
	// lfsx f0,r10,r5
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + ctx.r5.u32);
	ctx.f0.f64 = double(temp.f32);
	// fneg f0,f0
	ctx.f0.u64 = ctx.f0.u64 ^ 0x8000000000000000;
	// stfsx f0,r10,r5
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r10.u32 + ctx.r5.u32, temp.u32);
	// ble cr6,0x82a80a2c
	if (!ctx.cr6.gt) goto loc_82A80A2C;
	// li r3,2
	ctx.r3.s64 = 2;
	// addi r6,r4,4
	ctx.r6.s64 = ctx.r4.s64 + 4;
loc_82A806CC:
	// li r26,0
	ctx.r26.s64 = 0;
	// cmpwi cr6,r25,4
	ctx.cr6.compare<int32_t>(ctx.r25.s32, 4, ctx.xer);
	// blt cr6,0x82a80930
	if (ctx.cr6.lt) goto loc_82A80930;
	// addi r10,r25,-4
	ctx.r10.s64 = ctx.r25.s64 + -4;
	// li r9,0
	ctx.r9.s64 = 0;
	// rlwinm r8,r10,30,2,31
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 30) & 0x3FFFFFFF;
	// addi r10,r4,8
	ctx.r10.s64 = ctx.r4.s64 + 8;
	// addi r31,r8,1
	ctx.r31.s64 = ctx.r8.s64 + 1;
	// rlwinm r26,r31,2,0,29
	ctx.r26.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 2) & 0xFFFFFFFC;
loc_82A806F0:
	// lwz r7,-8(r10)
	ctx.r7.u64 = REX_LOAD_U32(ctx.r10.u32 + -8);
	// lwz r8,0(r6)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r6.u32 + 0);
	// add r30,r7,r3
	ctx.r30.u64 = ctx.r7.u64 + ctx.r3.u64;
	// add r29,r9,r8
	ctx.r29.u64 = ctx.r9.u64 + ctx.r8.u64;
	// rlwinm r8,r30,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r7,r29,2,0,29
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 2) & 0xFFFFFFFC;
	// add r8,r8,r5
	ctx.r8.u64 = ctx.r8.u64 + ctx.r5.u64;
	// add r7,r7,r5
	ctx.r7.u64 = ctx.r7.u64 + ctx.r5.u64;
	// add r30,r30,r11
	ctx.r30.u64 = ctx.r30.u64 + ctx.r11.u64;
	// add r29,r29,r11
	ctx.r29.u64 = ctx.r29.u64 + ctx.r11.u64;
	// rlwinm r28,r30,2,0,29
	ctx.r28.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f0,4(r8)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// rlwinm r30,r29,2,0,29
	ctx.r30.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f12,4(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// fneg f0,f0
	ctx.f0.u64 = ctx.f0.u64 ^ 0x8000000000000000;
	// lfs f13,0(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// fneg f12,f12
	ctx.f12.u64 = ctx.f12.u64 ^ 0x8000000000000000;
	// lfs f11,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// stfs f11,0(r7)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r7.u32 + 0, temp.u32);
	// stfs f0,4(r7)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r7.u32 + 4, temp.u32);
	// add r7,r30,r5
	ctx.r7.u64 = ctx.r30.u64 + ctx.r5.u64;
	// stfs f13,0(r8)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// stfs f12,4(r8)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r8.u32 + 4, temp.u32);
	// add r8,r28,r5
	ctx.r8.u64 = ctx.r28.u64 + ctx.r5.u64;
	// lfs f12,4(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// lfs f13,0(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// fneg f12,f12
	ctx.f12.u64 = ctx.f12.u64 ^ 0x8000000000000000;
	// lfs f0,4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lfs f11,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// fneg f0,f0
	ctx.f0.u64 = ctx.f0.u64 ^ 0x8000000000000000;
	// stfs f11,0(r7)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r7.u32 + 0, temp.u32);
	// stfs f0,4(r7)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r7.u32 + 4, temp.u32);
	// stfs f13,0(r8)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// stfs f12,4(r8)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r8.u32 + 4, temp.u32);
	// lwz r8,0(r6)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r6.u32 + 0);
	// lwz r7,-4(r10)
	ctx.r7.u64 = REX_LOAD_U32(ctx.r10.u32 + -4);
	// add r8,r9,r8
	ctx.r8.u64 = ctx.r9.u64 + ctx.r8.u64;
	// add r30,r7,r3
	ctx.r30.u64 = ctx.r7.u64 + ctx.r3.u64;
	// addi r29,r8,2
	ctx.r29.s64 = ctx.r8.s64 + 2;
	// rlwinm r8,r30,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r7,r29,2,0,29
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 2) & 0xFFFFFFFC;
	// add r8,r8,r5
	ctx.r8.u64 = ctx.r8.u64 + ctx.r5.u64;
	// add r7,r7,r5
	ctx.r7.u64 = ctx.r7.u64 + ctx.r5.u64;
	// add r30,r30,r11
	ctx.r30.u64 = ctx.r30.u64 + ctx.r11.u64;
	// add r29,r29,r11
	ctx.r29.u64 = ctx.r29.u64 + ctx.r11.u64;
	// rlwinm r30,r30,2,0,29
	ctx.r30.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f0,4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// rlwinm r29,r29,2,0,29
	ctx.r29.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f12,4(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// fneg f0,f0
	ctx.f0.u64 = ctx.f0.u64 ^ 0x8000000000000000;
	// lfs f13,0(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// fneg f12,f12
	ctx.f12.u64 = ctx.f12.u64 ^ 0x8000000000000000;
	// lfs f11,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// stfs f11,0(r7)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r7.u32 + 0, temp.u32);
	// stfs f0,4(r7)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r7.u32 + 4, temp.u32);
	// add r7,r30,r5
	ctx.r7.u64 = ctx.r30.u64 + ctx.r5.u64;
	// stfs f13,0(r8)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// addi r30,r9,6
	ctx.r30.s64 = ctx.r9.s64 + 6;
	// stfs f12,4(r8)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r8.u32 + 4, temp.u32);
	// add r8,r29,r5
	ctx.r8.u64 = ctx.r29.u64 + ctx.r5.u64;
	// lfs f12,4(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// lfs f11,0(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// fneg f12,f12
	ctx.f12.u64 = ctx.f12.u64 ^ 0x8000000000000000;
	// lfs f0,4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// fneg f0,f0
	ctx.f0.u64 = ctx.f0.u64 ^ 0x8000000000000000;
	// stfs f11,0(r8)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// stfs f12,4(r8)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r8.u32 + 4, temp.u32);
	// stfs f13,0(r7)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r7.u32 + 0, temp.u32);
	// stfs f0,4(r7)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r7.u32 + 4, temp.u32);
	// lwz r7,0(r6)
	ctx.r7.u64 = REX_LOAD_U32(ctx.r6.u32 + 0);
	// lwz r8,0(r10)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r10.u32 + 0);
	// add r7,r30,r7
	ctx.r7.u64 = ctx.r30.u64 + ctx.r7.u64;
	// add r8,r8,r3
	ctx.r8.u64 = ctx.r8.u64 + ctx.r3.u64;
	// addi r7,r7,-2
	ctx.r7.s64 = ctx.r7.s64 + -2;
	// rlwinm r28,r8,2,0,29
	ctx.r28.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 2) & 0xFFFFFFFC;
	// add r29,r8,r11
	ctx.r29.u64 = ctx.r8.u64 + ctx.r11.u64;
	// rlwinm r27,r7,2,0,29
	ctx.r27.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
	// add r8,r28,r5
	ctx.r8.u64 = ctx.r28.u64 + ctx.r5.u64;
	// add r28,r7,r11
	ctx.r28.u64 = ctx.r7.u64 + ctx.r11.u64;
	// add r7,r27,r5
	ctx.r7.u64 = ctx.r27.u64 + ctx.r5.u64;
	// lfs f0,4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// rlwinm r29,r29,2,0,29
	ctx.r29.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f11,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// fneg f0,f0
	ctx.f0.u64 = ctx.f0.u64 ^ 0x8000000000000000;
	// lfs f12,4(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// rlwinm r28,r28,2,0,29
	ctx.r28.u64 = __builtin_rotateleft64(ctx.r28.u32 | (ctx.r28.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f13,0(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// fneg f12,f12
	ctx.f12.u64 = ctx.f12.u64 ^ 0x8000000000000000;
	// stfs f11,0(r7)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r7.u32 + 0, temp.u32);
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// stfs f0,4(r7)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r7.u32 + 4, temp.u32);
	// add r7,r28,r5
	ctx.r7.u64 = ctx.r28.u64 + ctx.r5.u64;
	// stfs f13,0(r8)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// addi r9,r9,8
	ctx.r9.s64 = ctx.r9.s64 + 8;
	// stfs f12,4(r8)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r8.u32 + 4, temp.u32);
	// add r8,r29,r5
	ctx.r8.u64 = ctx.r29.u64 + ctx.r5.u64;
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// lfs f13,4(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f0,0(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// fneg f13,f13
	ctx.f13.u64 = ctx.f13.u64 ^ 0x8000000000000000;
	// lfs f12,4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// lfs f11,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// fneg f12,f12
	ctx.f12.u64 = ctx.f12.u64 ^ 0x8000000000000000;
	// stfs f11,0(r7)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r7.u32 + 0, temp.u32);
	// stfs f12,4(r7)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r7.u32 + 4, temp.u32);
	// stfs f0,0(r8)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// stfs f13,4(r8)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r8.u32 + 4, temp.u32);
	// lwz r7,4(r10)
	ctx.r7.u64 = REX_LOAD_U32(ctx.r10.u32 + 4);
	// lwz r8,0(r6)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r6.u32 + 0);
	// addi r10,r10,16
	ctx.r10.s64 = ctx.r10.s64 + 16;
	// add r29,r7,r3
	ctx.r29.u64 = ctx.r7.u64 + ctx.r3.u64;
	// add r30,r30,r8
	ctx.r30.u64 = ctx.r30.u64 + ctx.r8.u64;
	// rlwinm r8,r29,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r7,r30,2,0,29
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// add r8,r8,r5
	ctx.r8.u64 = ctx.r8.u64 + ctx.r5.u64;
	// add r7,r7,r5
	ctx.r7.u64 = ctx.r7.u64 + ctx.r5.u64;
	// add r29,r29,r11
	ctx.r29.u64 = ctx.r29.u64 + ctx.r11.u64;
	// add r30,r30,r11
	ctx.r30.u64 = ctx.r30.u64 + ctx.r11.u64;
	// lfs f0,4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// rlwinm r30,r30,2,0,29
	ctx.r30.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f11,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// fneg f0,f0
	ctx.f0.u64 = ctx.f0.u64 ^ 0x8000000000000000;
	// lfs f12,4(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// lfs f13,0(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// fneg f12,f12
	ctx.f12.u64 = ctx.f12.u64 ^ 0x8000000000000000;
	// stfs f11,0(r7)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r7.u32 + 0, temp.u32);
	// stfs f0,4(r7)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r7.u32 + 4, temp.u32);
	// add r7,r30,r5
	ctx.r7.u64 = ctx.r30.u64 + ctx.r5.u64;
	// stfs f13,0(r8)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// stfs f12,4(r8)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r8.u32 + 4, temp.u32);
	// rlwinm r8,r29,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 2) & 0xFFFFFFFC;
	// add r8,r8,r5
	ctx.r8.u64 = ctx.r8.u64 + ctx.r5.u64;
	// lfs f13,4(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f0,0(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// fneg f13,f13
	ctx.f13.u64 = ctx.f13.u64 ^ 0x8000000000000000;
	// lfs f12,4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// lfs f11,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// fneg f12,f12
	ctx.f12.u64 = ctx.f12.u64 ^ 0x8000000000000000;
	// stfs f11,0(r7)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r7.u32 + 0, temp.u32);
	// stfs f12,4(r7)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r7.u32 + 4, temp.u32);
	// stfs f0,0(r8)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// stfs f13,4(r8)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r8.u32 + 4, temp.u32);
	// bne cr6,0x82a806f0
	if (!ctx.cr6.eq) goto loc_82A806F0;
loc_82A80930:
	// cmpw cr6,r26,r25
	ctx.cr6.compare<int32_t>(ctx.r26.s32, ctx.r25.s32, ctx.xer);
	// bge cr6,0x82a809e4
	if (!ctx.cr6.lt) goto loc_82A809E4;
	// rlwinm r10,r26,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r26.u32 | (ctx.r26.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r31,r26,1,0,30
	ctx.r31.u64 = __builtin_rotateleft64(ctx.r26.u32 | (ctx.r26.u64 << 32), 1) & 0xFFFFFFFE;
	// add r7,r10,r4
	ctx.r7.u64 = ctx.r10.u64 + ctx.r4.u64;
	// subf r8,r26,r25
	ctx.r8.u64 = ctx.r25.u64 - ctx.r26.u64;
loc_82A80948:
	// lwz r9,0(r7)
	ctx.r9.u64 = REX_LOAD_U32(ctx.r7.u32 + 0);
	// addi r8,r8,-1
	ctx.r8.s64 = ctx.r8.s64 + -1;
	// lwz r10,0(r6)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r6.u32 + 0);
	// addi r7,r7,4
	ctx.r7.s64 = ctx.r7.s64 + 4;
	// add r30,r3,r9
	ctx.r30.u64 = ctx.r3.u64 + ctx.r9.u64;
	// add r29,r31,r10
	ctx.r29.u64 = ctx.r31.u64 + ctx.r10.u64;
	// rlwinm r10,r30,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r9,r29,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 2) & 0xFFFFFFFC;
	// add r10,r10,r5
	ctx.r10.u64 = ctx.r10.u64 + ctx.r5.u64;
	// add r9,r9,r5
	ctx.r9.u64 = ctx.r9.u64 + ctx.r5.u64;
	// add r30,r30,r11
	ctx.r30.u64 = ctx.r30.u64 + ctx.r11.u64;
	// add r29,r29,r11
	ctx.r29.u64 = ctx.r29.u64 + ctx.r11.u64;
	// rlwinm r28,r30,2,0,29
	ctx.r28.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f0,4(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// rlwinm r30,r29,2,0,29
	ctx.r30.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f12,4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// fneg f0,f0
	ctx.f0.u64 = ctx.f0.u64 ^ 0x8000000000000000;
	// lfs f13,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// fneg f12,f12
	ctx.f12.u64 = ctx.f12.u64 ^ 0x8000000000000000;
	// lfs f11,0(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// addi r31,r31,2
	ctx.r31.s64 = ctx.r31.s64 + 2;
	// stfs f0,4(r9)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r9.u32 + 4, temp.u32);
	// cmplwi cr6,r8,0
	ctx.cr6.compare<uint32_t>(ctx.r8.u32, 0, ctx.xer);
	// stfs f11,0(r9)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// add r9,r30,r5
	ctx.r9.u64 = ctx.r30.u64 + ctx.r5.u64;
	// stfs f12,4(r10)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// stfs f13,0(r10)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// add r10,r28,r5
	ctx.r10.u64 = ctx.r28.u64 + ctx.r5.u64;
	// lfs f12,4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// lfs f13,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// fneg f12,f12
	ctx.f12.u64 = ctx.f12.u64 ^ 0x8000000000000000;
	// lfs f0,4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lfs f11,0(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// fneg f0,f0
	ctx.f0.u64 = ctx.f0.u64 ^ 0x8000000000000000;
	// stfs f11,0(r9)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// stfs f0,4(r9)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r9.u32 + 4, temp.u32);
	// stfs f13,0(r10)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// stfs f12,4(r10)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// bne cr6,0x82a80948
	if (!ctx.cr6.eq) goto loc_82A80948;
loc_82A809E4:
	// lwz r10,0(r6)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r6.u32 + 0);
	// addi r25,r25,1
	ctx.r25.s64 = ctx.r25.s64 + 1;
	// addi r6,r6,4
	ctx.r6.s64 = ctx.r6.s64 + 4;
	// add r10,r3,r10
	ctx.r10.u64 = ctx.r3.u64 + ctx.r10.u64;
	// addi r3,r3,2
	ctx.r3.s64 = ctx.r3.s64 + 2;
	// addi r9,r10,1
	ctx.r9.s64 = ctx.r10.s64 + 1;
	// add r10,r10,r11
	ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
	// rlwinm r9,r9,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// cmpw cr6,r25,r23
	ctx.cr6.compare<int32_t>(ctx.r25.s32, ctx.r23.s32, ctx.xer);
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// lfsx f0,r9,r5
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + ctx.r5.u32);
	ctx.f0.f64 = double(temp.f32);
	// fneg f0,f0
	ctx.f0.u64 = ctx.f0.u64 ^ 0x8000000000000000;
	// stfsx f0,r9,r5
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r9.u32 + ctx.r5.u32, temp.u32);
	// lfsx f0,r10,r5
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + ctx.r5.u32);
	ctx.f0.f64 = double(temp.f32);
	// fneg f0,f0
	ctx.f0.u64 = ctx.f0.u64 ^ 0x8000000000000000;
	// stfsx f0,r10,r5
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r10.u32 + ctx.r5.u32, temp.u32);
	// blt cr6,0x82a806cc
	if (ctx.cr6.lt) goto loc_82A806CC;
loc_82A80A2C:
	// b 0x829ff804
	__restgprlr_23(ctx, base);
	return;
}

DEFINE_REX_FUNC(sub_82A80A30) {
	REX_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	REX_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// addi r12,r1,-8
	ctx.r12.s64 = ctx.r1.s64 + -8;
	// bl 0x82a012fc
	ctx.lr = 0x82A80A40;
	__savefpr_17(ctx, base);
	// lfs f0,8(r3)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// lfs f2,64(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 64);
	ctx.f2.f64 = double(temp.f32);
	// stfs f0,64(r3)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r3.u32 + 64, temp.u32);
	// lfs f13,12(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 12);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,24(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 24);
	ctx.f12.f64 = double(temp.f32);
	// lfs f11,28(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 28);
	ctx.f11.f64 = double(temp.f32);
	// lfs f10,40(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 40);
	ctx.f10.f64 = double(temp.f32);
	// lfs f9,44(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 44);
	ctx.f9.f64 = double(temp.f32);
	// lfs f27,56(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 56);
	ctx.f27.f64 = double(temp.f32);
	// lfs f26,60(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 60);
	ctx.f26.f64 = double(temp.f32);
	// lfs f25,72(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 72);
	ctx.f25.f64 = double(temp.f32);
	// lfs f8,16(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 16);
	ctx.f8.f64 = double(temp.f32);
	// lfs f7,20(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 20);
	ctx.f7.f64 = double(temp.f32);
	// lfs f6,32(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 32);
	ctx.f6.f64 = double(temp.f32);
	// lfs f5,36(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 36);
	ctx.f5.f64 = double(temp.f32);
	// lfs f4,48(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 48);
	ctx.f4.f64 = double(temp.f32);
	// lfs f3,52(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 52);
	ctx.f3.f64 = double(temp.f32);
	// lfs f1,68(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 68);
	ctx.f1.f64 = double(temp.f32);
	// lfs f24,120(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 120);
	ctx.f24.f64 = double(temp.f32);
	// lfs f23,124(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 124);
	ctx.f23.f64 = double(temp.f32);
	// lfs f22,88(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 88);
	ctx.f22.f64 = double(temp.f32);
	// lfs f21,92(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 92);
	ctx.f21.f64 = double(temp.f32);
	// lfs f20,104(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 104);
	ctx.f20.f64 = double(temp.f32);
	// lfs f19,108(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 108);
	ctx.f19.f64 = double(temp.f32);
	// lfs f18,76(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 76);
	ctx.f18.f64 = double(temp.f32);
	// lfs f17,112(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 112);
	ctx.f17.f64 = double(temp.f32);
	// lfs f31,80(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 80);
	ctx.f31.f64 = double(temp.f32);
	// lfs f30,84(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 84);
	ctx.f30.f64 = double(temp.f32);
	// lfs f29,96(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 96);
	ctx.f29.f64 = double(temp.f32);
	// lfs f28,100(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 100);
	ctx.f28.f64 = double(temp.f32);
	// lfs f0,116(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 116);
	ctx.f0.f64 = double(temp.f32);
	// stfs f24,8(r3)
	temp.f32 = float(ctx.f24.f64);
	REX_STORE_U32(ctx.r3.u32 + 8, temp.u32);
	// stfs f23,12(r3)
	temp.f32 = float(ctx.f23.f64);
	REX_STORE_U32(ctx.r3.u32 + 12, temp.u32);
	// stfs f27,16(r3)
	temp.f32 = float(ctx.f27.f64);
	REX_STORE_U32(ctx.r3.u32 + 16, temp.u32);
	// stfs f26,20(r3)
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r3.u32 + 20, temp.u32);
	// stfs f22,24(r3)
	temp.f32 = float(ctx.f22.f64);
	REX_STORE_U32(ctx.r3.u32 + 24, temp.u32);
	// stfs f21,28(r3)
	temp.f32 = float(ctx.f21.f64);
	REX_STORE_U32(ctx.r3.u32 + 28, temp.u32);
	// stfs f20,40(r3)
	temp.f32 = float(ctx.f20.f64);
	REX_STORE_U32(ctx.r3.u32 + 40, temp.u32);
	// stfs f19,44(r3)
	temp.f32 = float(ctx.f19.f64);
	REX_STORE_U32(ctx.r3.u32 + 44, temp.u32);
	// stfs f25,56(r3)
	temp.f32 = float(ctx.f25.f64);
	REX_STORE_U32(ctx.r3.u32 + 56, temp.u32);
	// stfs f18,60(r3)
	temp.f32 = float(ctx.f18.f64);
	REX_STORE_U32(ctx.r3.u32 + 60, temp.u32);
	// stfs f17,72(r3)
	temp.f32 = float(ctx.f17.f64);
	REX_STORE_U32(ctx.r3.u32 + 72, temp.u32);
	// stfs f13,68(r3)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r3.u32 + 68, temp.u32);
	// stfs f12,32(r3)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r3.u32 + 32, temp.u32);
	// stfs f11,36(r3)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r3.u32 + 36, temp.u32);
	// stfs f10,48(r3)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r3.u32 + 48, temp.u32);
	// stfs f9,52(r3)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r3.u32 + 52, temp.u32);
	// stfs f0,76(r3)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r3.u32 + 76, temp.u32);
	// stfs f4,80(r3)
	temp.f32 = float(ctx.f4.f64);
	REX_STORE_U32(ctx.r3.u32 + 80, temp.u32);
	// stfs f3,84(r3)
	temp.f32 = float(ctx.f3.f64);
	REX_STORE_U32(ctx.r3.u32 + 84, temp.u32);
	// stfs f31,88(r3)
	temp.f32 = float(ctx.f31.f64);
	REX_STORE_U32(ctx.r3.u32 + 88, temp.u32);
	// stfs f30,92(r3)
	temp.f32 = float(ctx.f30.f64);
	REX_STORE_U32(ctx.r3.u32 + 92, temp.u32);
	// stfs f8,96(r3)
	temp.f32 = float(ctx.f8.f64);
	REX_STORE_U32(ctx.r3.u32 + 96, temp.u32);
	// stfs f7,100(r3)
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r3.u32 + 100, temp.u32);
	// stfs f29,104(r3)
	temp.f32 = float(ctx.f29.f64);
	REX_STORE_U32(ctx.r3.u32 + 104, temp.u32);
	// stfs f28,108(r3)
	temp.f32 = float(ctx.f28.f64);
	REX_STORE_U32(ctx.r3.u32 + 108, temp.u32);
	// stfs f6,112(r3)
	temp.f32 = float(ctx.f6.f64);
	REX_STORE_U32(ctx.r3.u32 + 112, temp.u32);
	// stfs f5,116(r3)
	temp.f32 = float(ctx.f5.f64);
	REX_STORE_U32(ctx.r3.u32 + 116, temp.u32);
	// stfs f2,120(r3)
	temp.f32 = float(ctx.f2.f64);
	REX_STORE_U32(ctx.r3.u32 + 120, temp.u32);
	// stfs f1,124(r3)
	temp.f32 = float(ctx.f1.f64);
	REX_STORE_U32(ctx.r3.u32 + 124, temp.u32);
	// addi r12,r1,-8
	ctx.r12.s64 = ctx.r1.s64 + -8;
	// bl 0x82a01348
	ctx.lr = 0x82A80B38;
	__restfpr_17(ctx, base);
	// lwz r12,-8(r1)
	ctx.r12.u64 = REX_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

DEFINE_REX_FUNC(sub_82A80B48) {
	REX_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7bc
	ctx.lr = 0x82A80B50;
	__savegprlr_25(ctx, base);
	// addi r12,r1,-64
	ctx.r12.s64 = ctx.r1.s64 + -64;
	// bl 0x82a012f0
	ctx.lr = 0x82A80B58;
	__savefpr_14(ctx, base);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f10,0(r4)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r4.u32 + 0);
	ctx.f10.f64 = double(temp.f32);
	// srawi r29,r3,3
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0x7) != 0);
	ctx.r29.s64 = ctx.r3.s32 >> 3;
	// lfs f9,4(r4)
	temp.u32 = REX_LOAD_U32(ctx.r4.u32 + 4);
	ctx.f9.f64 = double(temp.f32);
	// addi r30,r5,8
	ctx.r30.s64 = ctx.r5.s64 + 8;
	// addi r27,r29,-2
	ctx.r27.s64 = ctx.r29.s64 + -2;
	// lfs f0,3400(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 3400);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// fmr f12,f0
	ctx.f12.f64 = ctx.f0.f64;
	// cmpwi cr6,r27,2
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 2, ctx.xer);
	// lfs f13,2612(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 2612);
	ctx.f13.f64 = double(temp.f32);
	// rlwinm r11,r29,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 1) & 0xFFFFFFFE;
	// fmr f11,f13
	ctx.f11.f64 = ctx.f13.f64;
	// rlwinm r9,r11,1,0,30
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// rlwinm r10,r11,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r8,r9,r11
	ctx.r8.u64 = ctx.r9.u64 + ctx.r11.u64;
	// rlwinm r9,r9,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r8,r8,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 2) & 0xFFFFFFFC;
	// add r9,r9,r4
	ctx.r9.u64 = ctx.r9.u64 + ctx.r4.u64;
	// add r10,r10,r4
	ctx.r10.u64 = ctx.r10.u64 + ctx.r4.u64;
	// add r8,r8,r4
	ctx.r8.u64 = ctx.r8.u64 + ctx.r4.u64;
	// lfs f6,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f6.f64 = double(temp.f32);
	// lfs f5,4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 4);
	ctx.f5.f64 = double(temp.f32);
	// fadds f2,f10,f6
	ctx.f2.f64 = double(float(ctx.f10.f64 + ctx.f6.f64));
	// lfs f4,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f4.f64 = double(temp.f32);
	// fsubs f10,f10,f6
	ctx.f10.f64 = double(float(ctx.f10.f64 - ctx.f6.f64));
	// lfs f8,0(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f8.f64 = double(temp.f32);
	// fadds f1,f9,f5
	ctx.f1.f64 = double(float(ctx.f9.f64 + ctx.f5.f64));
	// lfs f7,4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f7.f64 = double(temp.f32);
	// fadds f6,f8,f4
	ctx.f6.f64 = double(float(ctx.f8.f64 + ctx.f4.f64));
	// lfs f3,4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f3.f64 = double(temp.f32);
	// fsubs f9,f9,f5
	ctx.f9.f64 = double(float(ctx.f9.f64 - ctx.f5.f64));
	// fadds f5,f7,f3
	ctx.f5.f64 = double(float(ctx.f7.f64 + ctx.f3.f64));
	// fsubs f7,f7,f3
	ctx.f7.f64 = double(float(ctx.f7.f64 - ctx.f3.f64));
	// fsubs f8,f8,f4
	ctx.f8.f64 = double(float(ctx.f8.f64 - ctx.f4.f64));
	// fadds f4,f6,f2
	ctx.f4.f64 = double(float(ctx.f6.f64 + ctx.f2.f64));
	// stfs f4,0(r4)
	temp.f32 = float(ctx.f4.f64);
	REX_STORE_U32(ctx.r4.u32 + 0, temp.u32);
	// fsubs f6,f2,f6
	ctx.f6.f64 = double(float(ctx.f2.f64 - ctx.f6.f64));
	// fadds f4,f5,f1
	ctx.f4.f64 = double(float(ctx.f5.f64 + ctx.f1.f64));
	// stfs f4,4(r4)
	temp.f32 = float(ctx.f4.f64);
	REX_STORE_U32(ctx.r4.u32 + 4, temp.u32);
	// stfs f6,0(r10)
	temp.f32 = float(ctx.f6.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// fsubs f6,f1,f5
	ctx.f6.f64 = double(float(ctx.f1.f64 - ctx.f5.f64));
	// stfs f6,4(r10)
	temp.f32 = float(ctx.f6.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// fsubs f6,f10,f7
	ctx.f6.f64 = double(float(ctx.f10.f64 - ctx.f7.f64));
	// stfs f6,0(r9)
	temp.f32 = float(ctx.f6.f64);
	REX_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// fadds f6,f8,f9
	ctx.f6.f64 = double(float(ctx.f8.f64 + ctx.f9.f64));
	// stfs f6,4(r9)
	temp.f32 = float(ctx.f6.f64);
	REX_STORE_U32(ctx.r9.u32 + 4, temp.u32);
	// fadds f10,f7,f10
	ctx.f10.f64 = double(float(ctx.f7.f64 + ctx.f10.f64));
	// stfs f10,0(r8)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// fsubs f10,f9,f8
	ctx.f10.f64 = double(float(ctx.f9.f64 - ctx.f8.f64));
	// stfs f10,4(r8)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r8.u32 + 4, temp.u32);
	// lfs f10,4(r5)
	temp.u32 = REX_LOAD_U32(ctx.r5.u32 + 4);
	ctx.f10.f64 = double(temp.f32);
	// lfs f15,8(r5)
	temp.u32 = REX_LOAD_U32(ctx.r5.u32 + 8);
	ctx.f15.f64 = double(temp.f32);
	// lfs f14,12(r5)
	temp.u32 = REX_LOAD_U32(ctx.r5.u32 + 12);
	ctx.f14.f64 = double(temp.f32);
	// stfs f10,-224(r1)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r1.u32 + -224, temp.u32);
	// ble cr6,0x82a80fb4
	if (!ctx.cr6.gt) goto loc_82A80FB4;
	// addi r9,r11,4
	ctx.r9.s64 = ctx.r11.s64 + 4;
	// addi r8,r11,-4
	ctx.r8.s64 = ctx.r11.s64 + -4;
	// addi r7,r11,2
	ctx.r7.s64 = ctx.r11.s64 + 2;
	// addi r5,r11,-2
	ctx.r5.s64 = ctx.r11.s64 + -2;
	// rlwinm r3,r9,2,0,29
	ctx.r3.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r10,r11,4,0,27
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 4) & 0xFFFFFFF0;
	// addi r31,r27,-3
	ctx.r31.s64 = ctx.r27.s64 + -3;
	// rlwinm r9,r8,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r6,r7,3,0,28
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 3) & 0xFFFFFFF8;
	// rlwinm r8,r5,3,0,28
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 3) & 0xFFFFFFF8;
	// add r7,r10,r4
	ctx.r7.u64 = ctx.r10.u64 + ctx.r4.u64;
	// rlwinm r5,r31,30,2,31
	ctx.r5.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 30) & 0x3FFFFFFF;
	// add r31,r3,r4
	ctx.r31.u64 = ctx.r3.u64 + ctx.r4.u64;
	// add r3,r6,r4
	ctx.r3.u64 = ctx.r6.u64 + ctx.r4.u64;
	// addi r6,r7,-16
	ctx.r6.s64 = ctx.r7.s64 + -16;
	// rlwinm r7,r11,1,0,30
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// addi r28,r5,1
	ctx.r28.s64 = ctx.r5.s64 + 1;
	// add r7,r11,r7
	ctx.r7.u64 = ctx.r11.u64 + ctx.r7.u64;
	// addi r10,r4,16
	ctx.r10.s64 = ctx.r4.s64 + 16;
	// rlwinm r7,r7,2,0,29
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
	// add r9,r9,r4
	ctx.r9.u64 = ctx.r9.u64 + ctx.r4.u64;
	// add r7,r7,r4
	ctx.r7.u64 = ctx.r7.u64 + ctx.r4.u64;
	// add r8,r8,r4
	ctx.r8.u64 = ctx.r8.u64 + ctx.r4.u64;
	// addi r5,r7,16
	ctx.r5.s64 = ctx.r7.s64 + 16;
	// addi r7,r7,-16
	ctx.r7.s64 = ctx.r7.s64 + -16;
loc_82A80C9C:
	// addi r30,r30,16
	ctx.r30.s64 = ctx.r30.s64 + 16;
	// lfs f10,-8(r3)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + -8);
	ctx.f10.f64 = double(temp.f32);
	// lfs f8,-8(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + -8);
	ctx.f8.f64 = double(temp.f32);
	// lfs f6,-4(r5)
	temp.u32 = REX_LOAD_U32(ctx.r5.u32 + -4);
	ctx.f6.f64 = double(temp.f32);
	// fadds f23,f8,f10
	ctx.f23.f64 = double(float(ctx.f8.f64 + ctx.f10.f64));
	// lfs f5,-4(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + -4);
	ctx.f5.f64 = double(temp.f32);
	// fsubs f19,f8,f10
	ctx.f19.f64 = double(float(ctx.f8.f64 - ctx.f10.f64));
	// lfs f9,-8(r5)
	temp.u32 = REX_LOAD_U32(ctx.r5.u32 + -8);
	ctx.f9.f64 = double(temp.f32);
	// fadds f8,f5,f6
	ctx.f8.f64 = double(float(ctx.f5.f64 + ctx.f6.f64));
	// lfs f7,-8(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + -8);
	ctx.f7.f64 = double(temp.f32);
	// fsubs f18,f5,f6
	ctx.f18.f64 = double(float(ctx.f5.f64 - ctx.f6.f64));
	// lfs f4,-4(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + -4);
	ctx.f4.f64 = double(temp.f32);
	// fadds f21,f7,f9
	ctx.f21.f64 = double(float(ctx.f7.f64 + ctx.f9.f64));
	// lfs f3,0(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 0);
	ctx.f3.f64 = double(temp.f32);
	// fsubs f6,f7,f9
	ctx.f6.f64 = double(float(ctx.f7.f64 - ctx.f9.f64));
	// lfs f30,-4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + -4);
	ctx.f30.f64 = double(temp.f32);
	// lfs f29,0(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f29.f64 = double(temp.f32);
	// fadds f9,f30,f4
	ctx.f9.f64 = double(float(ctx.f30.f64 + ctx.f4.f64));
	// lfs f24,-4(r30)
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + -4);
	ctx.f24.f64 = double(temp.f32);
	// fadds f7,f3,f29
	ctx.f7.f64 = double(float(ctx.f3.f64 + ctx.f29.f64));
	// lfs f1,0(r5)
	temp.u32 = REX_LOAD_U32(ctx.r5.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// fsubs f3,f29,f3
	ctx.f3.f64 = double(float(ctx.f29.f64 - ctx.f3.f64));
	// lfs f27,0(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 0);
	ctx.f27.f64 = double(temp.f32);
	// fadds f29,f24,f13
	ctx.f29.f64 = double(float(ctx.f24.f64 + ctx.f13.f64));
	// lfs f31,4(r5)
	temp.u32 = REX_LOAD_U32(ctx.r5.u32 + 4);
	ctx.f31.f64 = double(temp.f32);
	// fadds f17,f27,f1
	ctx.f17.f64 = double(float(ctx.f27.f64 + ctx.f1.f64));
	// lfs f2,4(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// fsubs f4,f30,f4
	ctx.f4.f64 = double(float(ctx.f30.f64 - ctx.f4.f64));
	// lfs f28,4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f28.f64 = double(temp.f32);
	// fsubs f30,f19,f18
	ctx.f30.f64 = double(float(ctx.f19.f64 - ctx.f18.f64));
	// lfs f26,4(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 4);
	ctx.f26.f64 = double(temp.f32);
	// fadds f5,f28,f2
	ctx.f5.f64 = double(float(ctx.f28.f64 + ctx.f2.f64));
	// fadds f16,f26,f31
	ctx.f16.f64 = double(float(ctx.f26.f64 + ctx.f31.f64));
	// lfs f25,-8(r30)
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + -8);
	ctx.f25.f64 = double(temp.f32);
	// fsubs f31,f26,f31
	ctx.f31.f64 = double(float(ctx.f26.f64 - ctx.f31.f64));
	// lfs f22,0(r30)
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + 0);
	ctx.f22.f64 = double(temp.f32);
	// fadds f26,f8,f9
	ctx.f26.f64 = double(float(ctx.f8.f64 + ctx.f9.f64));
	// lfs f20,4(r30)
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + 4);
	ctx.f20.f64 = double(temp.f32);
	// fsubs f8,f9,f8
	ctx.f8.f64 = double(float(ctx.f9.f64 - ctx.f8.f64));
	// stfs f26,-4(r10)
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r10.u32 + -4, temp.u32);
	// fadds f10,f25,f0
	ctx.f10.f64 = double(float(ctx.f25.f64 + ctx.f0.f64));
	// fmuls f9,f29,f15
	ctx.f9.f64 = double(float(ctx.f29.f64 * ctx.f15.f64));
	// fadds f29,f17,f7
	ctx.f29.f64 = double(float(ctx.f17.f64 + ctx.f7.f64));
	// stfs f29,0(r10)
	temp.f32 = float(ctx.f29.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// fadds f0,f21,f23
	ctx.f0.f64 = double(float(ctx.f21.f64 + ctx.f23.f64));
	// stfs f0,-8(r10)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r10.u32 + -8, temp.u32);
	// fsubs f2,f28,f2
	ctx.f2.f64 = double(float(ctx.f28.f64 - ctx.f2.f64));
	// fadds f28,f22,f12
	ctx.f28.f64 = double(float(ctx.f22.f64 + ctx.f12.f64));
	// fadds f29,f16,f5
	ctx.f29.f64 = double(float(ctx.f16.f64 + ctx.f5.f64));
	// stfs f29,4(r10)
	temp.f32 = float(ctx.f29.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// fsubs f5,f5,f16
	ctx.f5.f64 = double(float(ctx.f5.f64 - ctx.f16.f64));
	// stfs f5,4(r31)
	temp.f32 = float(ctx.f5.f64);
	REX_STORE_U32(ctx.r31.u32 + 4, temp.u32);
	// fadds f5,f6,f4
	ctx.f5.f64 = double(float(ctx.f6.f64 + ctx.f4.f64));
	// stfs f8,-4(r31)
	temp.f32 = float(ctx.f8.f64);
	REX_STORE_U32(ctx.r31.u32 + -4, temp.u32);
	// fsubs f1,f27,f1
	ctx.f1.f64 = double(float(ctx.f27.f64 - ctx.f1.f64));
	// fsubs f27,f23,f21
	ctx.f27.f64 = double(float(ctx.f23.f64 - ctx.f21.f64));
	// stfs f27,-8(r31)
	temp.f32 = float(ctx.f27.f64);
	REX_STORE_U32(ctx.r31.u32 + -8, temp.u32);
	// fmuls f10,f10,f15
	ctx.f10.f64 = double(float(ctx.f10.f64 * ctx.f15.f64));
	// fmr f29,f30
	ctx.f29.f64 = ctx.f30.f64;
	// fmuls f26,f9,f30
	ctx.f26.f64 = double(float(ctx.f9.f64 * ctx.f30.f64));
	// fsubs f11,f11,f20
	ctx.f11.f64 = double(float(ctx.f11.f64 - ctx.f20.f64));
	// fmuls f8,f28,f14
	ctx.f8.f64 = double(float(ctx.f28.f64 * ctx.f14.f64));
	// fmr f13,f24
	ctx.f13.f64 = ctx.f24.f64;
	// fsubs f7,f7,f17
	ctx.f7.f64 = double(float(ctx.f7.f64 - ctx.f17.f64));
	// stfs f7,0(r31)
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r31.u32 + 0, temp.u32);
	// fmuls f28,f9,f5
	ctx.f28.f64 = double(float(ctx.f9.f64 * ctx.f5.f64));
	// fmr f27,f5
	ctx.f27.f64 = ctx.f5.f64;
	// fsubs f5,f3,f31
	ctx.f5.f64 = double(float(ctx.f3.f64 - ctx.f31.f64));
	// fadds f30,f1,f2
	ctx.f30.f64 = double(float(ctx.f1.f64 + ctx.f2.f64));
	// fmr f0,f25
	ctx.f0.f64 = ctx.f25.f64;
	// fmr f12,f22
	ctx.f12.f64 = ctx.f22.f64;
	// fmuls f7,f11,f14
	ctx.f7.f64 = double(float(ctx.f11.f64 * ctx.f14.f64));
	// fneg f11,f20
	ctx.f11.u64 = ctx.f20.u64 ^ 0x8000000000000000;
	// fmsubs f29,f10,f29,f28
	ctx.f29.f64 = double(float(std::fma(ctx.f10.f64, ctx.f29.f64, -ctx.f28.f64)));
	// stfs f29,-8(r3)
	temp.f32 = float(ctx.f29.f64);
	REX_STORE_U32(ctx.r3.u32 + -8, temp.u32);
	// fmadds f29,f10,f27,f26
	ctx.f29.f64 = double(float(std::fma(ctx.f10.f64, ctx.f27.f64, ctx.f26.f64)));
	// stfs f29,-4(r3)
	temp.f32 = float(ctx.f29.f64);
	REX_STORE_U32(ctx.r3.u32 + -4, temp.u32);
	// fmr f29,f5
	ctx.f29.f64 = ctx.f5.f64;
	// fmuls f27,f13,f5
	ctx.f27.f64 = double(float(ctx.f13.f64 * ctx.f5.f64));
	// fmuls f28,f13,f30
	ctx.f28.f64 = double(float(ctx.f13.f64 * ctx.f30.f64));
	// fadds f5,f18,f19
	ctx.f5.f64 = double(float(ctx.f18.f64 + ctx.f19.f64));
	// fsubs f6,f4,f6
	ctx.f6.f64 = double(float(ctx.f4.f64 - ctx.f6.f64));
	// addi r28,r28,-1
	ctx.r28.s64 = ctx.r28.s64 + -1;
	// fmsubs f4,f0,f29,f28
	ctx.f4.f64 = double(float(std::fma(ctx.f0.f64, ctx.f29.f64, -ctx.f28.f64)));
	// stfs f4,0(r3)
	temp.f32 = float(ctx.f4.f64);
	REX_STORE_U32(ctx.r3.u32 + 0, temp.u32);
	// fmadds f4,f0,f30,f27
	ctx.f4.f64 = double(float(std::fma(ctx.f0.f64, ctx.f30.f64, ctx.f27.f64)));
	// stfs f4,4(r3)
	temp.f32 = float(ctx.f4.f64);
	REX_STORE_U32(ctx.r3.u32 + 4, temp.u32);
	// fmuls f4,f8,f5
	ctx.f4.f64 = double(float(ctx.f8.f64 * ctx.f5.f64));
	// addi r3,r3,16
	ctx.r3.s64 = ctx.r3.s64 + 16;
	// fmuls f28,f7,f5
	ctx.f28.f64 = double(float(ctx.f7.f64 * ctx.f5.f64));
	// addi r31,r31,16
	ctx.r31.s64 = ctx.r31.s64 + 16;
	// fsubs f5,f2,f1
	ctx.f5.f64 = double(float(ctx.f2.f64 - ctx.f1.f64));
	// addi r10,r10,16
	ctx.r10.s64 = ctx.r10.s64 + 16;
	// cmplwi cr6,r28,0
	ctx.cr6.compare<uint32_t>(ctx.r28.u32, 0, ctx.xer);
	// fmr f30,f6
	ctx.f30.f64 = ctx.f6.f64;
	// fmr f29,f6
	ctx.f29.f64 = ctx.f6.f64;
	// fadds f6,f31,f3
	ctx.f6.f64 = double(float(ctx.f31.f64 + ctx.f3.f64));
	// fmadds f4,f7,f30,f4
	ctx.f4.f64 = double(float(std::fma(ctx.f7.f64, ctx.f30.f64, ctx.f4.f64)));
	// stfs f4,-8(r5)
	temp.f32 = float(ctx.f4.f64);
	REX_STORE_U32(ctx.r5.u32 + -8, temp.u32);
	// fmsubs f4,f8,f29,f28
	ctx.f4.f64 = double(float(std::fma(ctx.f8.f64, ctx.f29.f64, -ctx.f28.f64)));
	// stfs f4,-4(r5)
	temp.f32 = float(ctx.f4.f64);
	REX_STORE_U32(ctx.r5.u32 + -4, temp.u32);
	// fmuls f4,f12,f6
	ctx.f4.f64 = double(float(ctx.f12.f64 * ctx.f6.f64));
	// fmuls f6,f11,f6
	ctx.f6.f64 = double(float(ctx.f11.f64 * ctx.f6.f64));
	// fmadds f4,f11,f5,f4
	ctx.f4.f64 = double(float(std::fma(ctx.f11.f64, ctx.f5.f64, ctx.f4.f64)));
	// stfs f4,0(r5)
	temp.f32 = float(ctx.f4.f64);
	REX_STORE_U32(ctx.r5.u32 + 0, temp.u32);
	// fmsubs f6,f12,f5,f6
	ctx.f6.f64 = double(float(std::fma(ctx.f12.f64, ctx.f5.f64, -ctx.f6.f64)));
	// stfs f6,4(r5)
	temp.f32 = float(ctx.f6.f64);
	REX_STORE_U32(ctx.r5.u32 + 4, temp.u32);
	// lfs f5,8(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 8);
	ctx.f5.f64 = double(temp.f32);
	// addi r5,r5,16
	ctx.r5.s64 = ctx.r5.s64 + 16;
	// lfs f6,8(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 8);
	ctx.f6.f64 = double(temp.f32);
	// lfs f3,12(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 12);
	ctx.f3.f64 = double(temp.f32);
	// fadds f23,f6,f5
	ctx.f23.f64 = double(float(ctx.f6.f64 + ctx.f5.f64));
	// lfs f4,12(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 12);
	ctx.f4.f64 = double(temp.f32);
	// fsubs f6,f6,f5
	ctx.f6.f64 = double(float(ctx.f6.f64 - ctx.f5.f64));
	// lfs f1,0(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// fadds f5,f3,f4
	ctx.f5.f64 = double(float(ctx.f3.f64 + ctx.f4.f64));
	// lfs f2,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f2.f64 = double(temp.f32);
	// fsubs f4,f4,f3
	ctx.f4.f64 = double(float(ctx.f4.f64 - ctx.f3.f64));
	// lfs f30,4(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 4);
	ctx.f30.f64 = double(temp.f32);
	// fadds f3,f1,f2
	ctx.f3.f64 = double(float(ctx.f1.f64 + ctx.f2.f64));
	// lfs f31,4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 4);
	ctx.f31.f64 = double(temp.f32);
	// fsubs f2,f2,f1
	ctx.f2.f64 = double(float(ctx.f2.f64 - ctx.f1.f64));
	// lfs f29,8(r6)
	temp.u32 = REX_LOAD_U32(ctx.r6.u32 + 8);
	ctx.f29.f64 = double(temp.f32);
	// fadds f1,f30,f31
	ctx.f1.f64 = double(float(ctx.f30.f64 + ctx.f31.f64));
	// lfs f28,8(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 8);
	ctx.f28.f64 = double(temp.f32);
	// fsubs f31,f31,f30
	ctx.f31.f64 = double(float(ctx.f31.f64 - ctx.f30.f64));
	// fadds f30,f29,f28
	ctx.f30.f64 = double(float(ctx.f29.f64 + ctx.f28.f64));
	// lfs f27,12(r6)
	temp.u32 = REX_LOAD_U32(ctx.r6.u32 + 12);
	ctx.f27.f64 = double(temp.f32);
	// lfs f26,12(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 12);
	ctx.f26.f64 = double(temp.f32);
	// fsubs f29,f28,f29
	ctx.f29.f64 = double(float(ctx.f28.f64 - ctx.f29.f64));
	// fadds f28,f26,f27
	ctx.f28.f64 = double(float(ctx.f26.f64 + ctx.f27.f64));
	// lfs f25,0(r6)
	temp.u32 = REX_LOAD_U32(ctx.r6.u32 + 0);
	ctx.f25.f64 = double(temp.f32);
	// fsubs f27,f26,f27
	ctx.f27.f64 = double(float(ctx.f26.f64 - ctx.f27.f64));
	// lfs f24,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f24.f64 = double(temp.f32);
	// fadds f26,f25,f24
	ctx.f26.f64 = double(float(ctx.f25.f64 + ctx.f24.f64));
	// lfs f22,4(r6)
	temp.u32 = REX_LOAD_U32(ctx.r6.u32 + 4);
	ctx.f22.f64 = double(temp.f32);
	// lfs f21,4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f21.f64 = double(temp.f32);
	// fadds f20,f30,f23
	ctx.f20.f64 = double(float(ctx.f30.f64 + ctx.f23.f64));
	// stfs f20,8(r9)
	temp.f32 = float(ctx.f20.f64);
	REX_STORE_U32(ctx.r9.u32 + 8, temp.u32);
	// fsubs f23,f23,f30
	ctx.f23.f64 = double(float(ctx.f23.f64 - ctx.f30.f64));
	// fadds f30,f29,f4
	ctx.f30.f64 = double(float(ctx.f29.f64 + ctx.f4.f64));
	// fadds f19,f28,f5
	ctx.f19.f64 = double(float(ctx.f28.f64 + ctx.f5.f64));
	// stfs f19,12(r9)
	temp.f32 = float(ctx.f19.f64);
	REX_STORE_U32(ctx.r9.u32 + 12, temp.u32);
	// fsubs f18,f5,f28
	ctx.f18.f64 = double(float(ctx.f5.f64 - ctx.f28.f64));
	// fsubs f5,f6,f27
	ctx.f5.f64 = double(float(ctx.f6.f64 - ctx.f27.f64));
	// fsubs f28,f24,f25
	ctx.f28.f64 = double(float(ctx.f24.f64 - ctx.f25.f64));
	// fadds f25,f26,f3
	ctx.f25.f64 = double(float(ctx.f26.f64 + ctx.f3.f64));
	// stfs f25,0(r9)
	temp.f32 = float(ctx.f25.f64);
	REX_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// fsubs f25,f3,f26
	ctx.f25.f64 = double(float(ctx.f3.f64 - ctx.f26.f64));
	// fadds f26,f22,f21
	ctx.f26.f64 = double(float(ctx.f22.f64 + ctx.f21.f64));
	// fsubs f3,f21,f22
	ctx.f3.f64 = double(float(ctx.f21.f64 - ctx.f22.f64));
	// fmuls f24,f10,f30
	ctx.f24.f64 = double(float(ctx.f10.f64 * ctx.f30.f64));
	// fmuls f22,f10,f5
	ctx.f22.f64 = double(float(ctx.f10.f64 * ctx.f5.f64));
	// fadds f10,f28,f31
	ctx.f10.f64 = double(float(ctx.f28.f64 + ctx.f31.f64));
	// fmsubs f5,f9,f5,f24
	ctx.f5.f64 = double(float(std::fma(ctx.f9.f64, ctx.f5.f64, -ctx.f24.f64)));
	// fadds f24,f26,f1
	ctx.f24.f64 = double(float(ctx.f26.f64 + ctx.f1.f64));
	// stfs f24,4(r9)
	temp.f32 = float(ctx.f24.f64);
	REX_STORE_U32(ctx.r9.u32 + 4, temp.u32);
	// stfs f23,8(r8)
	temp.f32 = float(ctx.f23.f64);
	REX_STORE_U32(ctx.r8.u32 + 8, temp.u32);
	// fsubs f1,f1,f26
	ctx.f1.f64 = double(float(ctx.f1.f64 - ctx.f26.f64));
	// fmadds f30,f9,f30,f22
	ctx.f30.f64 = double(float(std::fma(ctx.f9.f64, ctx.f30.f64, ctx.f22.f64)));
	// stfs f18,12(r8)
	temp.f32 = float(ctx.f18.f64);
	REX_STORE_U32(ctx.r8.u32 + 12, temp.u32);
	// fsubs f9,f2,f3
	ctx.f9.f64 = double(float(ctx.f2.f64 - ctx.f3.f64));
	// stfs f1,4(r8)
	temp.f32 = float(ctx.f1.f64);
	REX_STORE_U32(ctx.r8.u32 + 4, temp.u32);
	// fmr f1,f10
	ctx.f1.f64 = ctx.f10.f64;
	// stfs f25,0(r8)
	temp.f32 = float(ctx.f25.f64);
	REX_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// addi r9,r9,-16
	ctx.r9.s64 = ctx.r9.s64 + -16;
	// stfs f5,8(r7)
	temp.f32 = float(ctx.f5.f64);
	REX_STORE_U32(ctx.r7.u32 + 8, temp.u32);
	// fmuls f5,f0,f10
	ctx.f5.f64 = double(float(ctx.f0.f64 * ctx.f10.f64));
	// fsubs f10,f4,f29
	ctx.f10.f64 = double(float(ctx.f4.f64 - ctx.f29.f64));
	// stfs f30,12(r7)
	temp.f32 = float(ctx.f30.f64);
	REX_STORE_U32(ctx.r7.u32 + 12, temp.u32);
	// fmr f4,f9
	ctx.f4.f64 = ctx.f9.f64;
	// addi r8,r8,-16
	ctx.r8.s64 = ctx.r8.s64 + -16;
	// fmuls f30,f0,f9
	ctx.f30.f64 = double(float(ctx.f0.f64 * ctx.f9.f64));
	// fadds f9,f27,f6
	ctx.f9.f64 = double(float(ctx.f27.f64 + ctx.f6.f64));
	// fmr f6,f10
	ctx.f6.f64 = ctx.f10.f64;
	// fmsubs f5,f13,f4,f5
	ctx.f5.f64 = double(float(std::fma(ctx.f13.f64, ctx.f4.f64, -ctx.f5.f64)));
	// stfs f5,0(r7)
	temp.f32 = float(ctx.f5.f64);
	REX_STORE_U32(ctx.r7.u32 + 0, temp.u32);
	// fmadds f5,f13,f1,f30
	ctx.f5.f64 = double(float(std::fma(ctx.f13.f64, ctx.f1.f64, ctx.f30.f64)));
	// stfs f5,4(r7)
	temp.f32 = float(ctx.f5.f64);
	REX_STORE_U32(ctx.r7.u32 + 4, temp.u32);
	// fmuls f5,f7,f9
	ctx.f5.f64 = double(float(ctx.f7.f64 * ctx.f9.f64));
	// addi r7,r7,-16
	ctx.r7.s64 = ctx.r7.s64 + -16;
	// fmuls f4,f8,f9
	ctx.f4.f64 = double(float(ctx.f8.f64 * ctx.f9.f64));
	// fmr f29,f10
	ctx.f29.f64 = ctx.f10.f64;
	// fadds f9,f3,f2
	ctx.f9.f64 = double(float(ctx.f3.f64 + ctx.f2.f64));
	// fsubs f10,f31,f28
	ctx.f10.f64 = double(float(ctx.f31.f64 - ctx.f28.f64));
	// fmadds f8,f8,f6,f5
	ctx.f8.f64 = double(float(std::fma(ctx.f8.f64, ctx.f6.f64, ctx.f5.f64)));
	// stfs f8,8(r6)
	temp.f32 = float(ctx.f8.f64);
	REX_STORE_U32(ctx.r6.u32 + 8, temp.u32);
	// fmsubs f8,f7,f29,f4
	ctx.f8.f64 = double(float(std::fma(ctx.f7.f64, ctx.f29.f64, -ctx.f4.f64)));
	// stfs f8,12(r6)
	temp.f32 = float(ctx.f8.f64);
	REX_STORE_U32(ctx.r6.u32 + 12, temp.u32);
	// fmuls f8,f11,f9
	ctx.f8.f64 = double(float(ctx.f11.f64 * ctx.f9.f64));
	// fmuls f9,f12,f9
	ctx.f9.f64 = double(float(ctx.f12.f64 * ctx.f9.f64));
	// fmadds f8,f12,f10,f8
	ctx.f8.f64 = double(float(std::fma(ctx.f12.f64, ctx.f10.f64, ctx.f8.f64)));
	// stfs f8,0(r6)
	temp.f32 = float(ctx.f8.f64);
	REX_STORE_U32(ctx.r6.u32 + 0, temp.u32);
	// fmsubs f10,f11,f10,f9
	ctx.f10.f64 = double(float(std::fma(ctx.f11.f64, ctx.f10.f64, -ctx.f9.f64)));
	// stfs f10,4(r6)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r6.u32 + 4, temp.u32);
	// addi r6,r6,-16
	ctx.r6.s64 = ctx.r6.s64 + -16;
	// bne cr6,0x82a80c9c
	if (!ctx.cr6.eq) goto loc_82A80C9C;
	// lfs f10,-224(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -224);
	ctx.f10.f64 = double(temp.f32);
loc_82A80FB4:
	// add r7,r11,r29
	ctx.r7.u64 = ctx.r11.u64 + ctx.r29.u64;
	// fadds f13,f13,f10
	ctx.fpscr.disableFlushMode();
	ctx.f13.f64 = double(float(ctx.f13.f64 + ctx.f10.f64));
	// fadds f9,f0,f10
	ctx.f9.f64 = double(float(ctx.f0.f64 + ctx.f10.f64));
	// rlwinm r10,r29,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r8,r7,-2
	ctx.r8.s64 = ctx.r7.s64 + -2;
	// fsubs f12,f12,f10
	ctx.f12.f64 = double(float(ctx.f12.f64 - ctx.f10.f64));
	// add r6,r7,r11
	ctx.r6.u64 = ctx.r7.u64 + ctx.r11.u64;
	// fsubs f11,f11,f10
	ctx.f11.f64 = double(float(ctx.f11.f64 - ctx.f10.f64));
	// rlwinm r30,r8,2,0,29
	ctx.r30.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r8,r6,-2
	ctx.r8.s64 = ctx.r6.s64 + -2;
	// add r5,r6,r11
	ctx.r5.u64 = ctx.r6.u64 + ctx.r11.u64;
	// rlwinm r11,r6,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r31,r8,2,0,29
	ctx.r31.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r9,r7,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
	// lfsx f6,r30,r4
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + ctx.r4.u32);
	ctx.f6.f64 = double(temp.f32);
	// rlwinm r8,r5,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 2) & 0xFFFFFFFC;
	// fmuls f0,f13,f15
	ctx.f0.f64 = double(float(ctx.f13.f64 * ctx.f15.f64));
	// add r10,r10,r4
	ctx.r10.u64 = ctx.r10.u64 + ctx.r4.u64;
	// fmuls f13,f9,f15
	ctx.f13.f64 = double(float(ctx.f9.f64 * ctx.f15.f64));
	// add r11,r11,r4
	ctx.r11.u64 = ctx.r11.u64 + ctx.r4.u64;
	// fmuls f12,f12,f14
	ctx.f12.f64 = double(float(ctx.f12.f64 * ctx.f14.f64));
	// addi r3,r5,-2
	ctx.r3.s64 = ctx.r5.s64 + -2;
	// lfsx f4,r31,r4
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + ctx.r4.u32);
	ctx.f4.f64 = double(temp.f32);
	// rlwinm r28,r27,2,0,29
	ctx.r28.u64 = __builtin_rotateleft64(ctx.r27.u32 | (ctx.r27.u64 << 32), 2) & 0xFFFFFFFC;
	// fmuls f11,f11,f14
	ctx.f11.f64 = double(float(ctx.f11.f64 * ctx.f14.f64));
	// add r8,r8,r4
	ctx.r8.u64 = ctx.r8.u64 + ctx.r4.u64;
	// add r9,r9,r4
	ctx.r9.u64 = ctx.r9.u64 + ctx.r4.u64;
	// lfs f8,-4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + -4);
	ctx.f8.f64 = double(temp.f32);
	// rlwinm r3,r3,2,0,29
	ctx.r3.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f5,-4(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -4);
	ctx.f5.f64 = double(temp.f32);
	// fadds f1,f8,f5
	ctx.f1.f64 = double(float(ctx.f8.f64 + ctx.f5.f64));
	// addi r26,r5,2
	ctx.r26.s64 = ctx.r5.s64 + 2;
	// lfsx f9,r28,r4
	temp.u32 = REX_LOAD_U32(ctx.r28.u32 + ctx.r4.u32);
	ctx.f9.f64 = double(temp.f32);
	// fsubs f8,f8,f5
	ctx.f8.f64 = double(float(ctx.f8.f64 - ctx.f5.f64));
	// lfs f3,-4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + -4);
	ctx.f3.f64 = double(temp.f32);
	// fadds f31,f9,f4
	ctx.f31.f64 = double(float(ctx.f9.f64 + ctx.f4.f64));
	// lfs f7,-4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + -4);
	ctx.f7.f64 = double(temp.f32);
	// fsubs f9,f9,f4
	ctx.f9.f64 = double(float(ctx.f9.f64 - ctx.f4.f64));
	// lfsx f2,r3,r4
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + ctx.r4.u32);
	ctx.f2.f64 = double(temp.f32);
	// fadds f5,f7,f3
	ctx.f5.f64 = double(float(ctx.f7.f64 + ctx.f3.f64));
	// fadds f4,f6,f2
	ctx.f4.f64 = double(float(ctx.f6.f64 + ctx.f2.f64));
	// addi r25,r7,2
	ctx.r25.s64 = ctx.r7.s64 + 2;
	// fsubs f6,f6,f2
	ctx.f6.f64 = double(float(ctx.f6.f64 - ctx.f2.f64));
	// fsubs f7,f7,f3
	ctx.f7.f64 = double(float(ctx.f7.f64 - ctx.f3.f64));
	// fadds f3,f5,f1
	ctx.f3.f64 = double(float(ctx.f5.f64 + ctx.f1.f64));
	// stfs f3,-4(r10)
	temp.f32 = float(ctx.f3.f64);
	REX_STORE_U32(ctx.r10.u32 + -4, temp.u32);
	// fadds f3,f4,f31
	ctx.f3.f64 = double(float(ctx.f4.f64 + ctx.f31.f64));
	// stfsx f3,r28,r4
	temp.f32 = float(ctx.f3.f64);
	REX_STORE_U32(ctx.r28.u32 + ctx.r4.u32, temp.u32);
	// fsubs f4,f31,f4
	ctx.f4.f64 = double(float(ctx.f31.f64 - ctx.f4.f64));
	// stfsx f4,r30,r4
	temp.f32 = float(ctx.f4.f64);
	REX_STORE_U32(ctx.r30.u32 + ctx.r4.u32, temp.u32);
	// fadds f4,f6,f8
	ctx.f4.f64 = double(float(ctx.f6.f64 + ctx.f8.f64));
	// addi r30,r29,3
	ctx.r30.s64 = ctx.r29.s64 + 3;
	// fsubs f5,f1,f5
	ctx.f5.f64 = double(float(ctx.f1.f64 - ctx.f5.f64));
	// stfs f5,-4(r9)
	temp.f32 = float(ctx.f5.f64);
	REX_STORE_U32(ctx.r9.u32 + -4, temp.u32);
	// fsubs f5,f9,f7
	ctx.f5.f64 = double(float(ctx.f9.f64 - ctx.f7.f64));
	// fadds f9,f7,f9
	ctx.f9.f64 = double(float(ctx.f7.f64 + ctx.f9.f64));
	// fsubs f8,f8,f6
	ctx.f8.f64 = double(float(ctx.f8.f64 - ctx.f6.f64));
	// fmuls f3,f0,f4
	ctx.f3.f64 = double(float(ctx.f0.f64 * ctx.f4.f64));
	// fmuls f2,f0,f5
	ctx.f2.f64 = double(float(ctx.f0.f64 * ctx.f5.f64));
	// fmsubs f7,f13,f5,f3
	ctx.f7.f64 = double(float(std::fma(ctx.f13.f64, ctx.f5.f64, -ctx.f3.f64)));
	// stfsx f7,r31,r4
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r31.u32 + ctx.r4.u32, temp.u32);
	// addi r31,r6,2
	ctx.r31.s64 = ctx.r6.s64 + 2;
	// fmadds f7,f13,f4,f2
	ctx.f7.f64 = double(float(std::fma(ctx.f13.f64, ctx.f4.f64, ctx.f2.f64)));
	// stfs f7,-4(r11)
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r11.u32 + -4, temp.u32);
	// fmuls f7,f12,f9
	ctx.f7.f64 = double(float(ctx.f12.f64 * ctx.f9.f64));
	// lfs f5,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f5.f64 = double(temp.f32);
	// fmuls f9,f11,f9
	ctx.f9.f64 = double(float(ctx.f11.f64 * ctx.f9.f64));
	// lfs f3,4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f3.f64 = double(temp.f32);
	// addi r6,r6,3
	ctx.r6.s64 = ctx.r6.s64 + 3;
	// rlwinm r28,r31,2,0,29
	ctx.r28.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 2) & 0xFFFFFFFC;
	// fmadds f7,f11,f8,f7
	ctx.f7.f64 = double(float(std::fma(ctx.f11.f64, ctx.f8.f64, ctx.f7.f64)));
	// stfsx f7,r3,r4
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r3.u32 + ctx.r4.u32, temp.u32);
	// addi r3,r29,2
	ctx.r3.s64 = ctx.r29.s64 + 2;
	// fmsubs f9,f12,f8,f9
	ctx.f9.f64 = double(float(std::fma(ctx.f12.f64, ctx.f8.f64, -ctx.f9.f64)));
	// stfs f9,-4(r8)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r8.u32 + -4, temp.u32);
	// lfs f9,0(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f9.f64 = double(temp.f32);
	// rlwinm r27,r3,2,0,29
	ctx.r27.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f8,0(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f8.f64 = double(temp.f32);
	// lfs f7,4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f7.f64 = double(temp.f32);
	// lfs f6,4(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f6.f64 = double(temp.f32);
	// lfs f4,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f4.f64 = double(temp.f32);
	// fadds f31,f9,f8
	ctx.f31.f64 = double(float(ctx.f9.f64 + ctx.f8.f64));
	// lfs f2,4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// fsubs f9,f9,f8
	ctx.f9.f64 = double(float(ctx.f9.f64 - ctx.f8.f64));
	// rlwinm r29,r30,2,0,29
	ctx.r29.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// fadds f8,f7,f6
	ctx.f8.f64 = double(float(ctx.f7.f64 + ctx.f6.f64));
	// rlwinm r30,r6,2,0,29
	ctx.r30.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
	// fsubs f7,f7,f6
	ctx.f7.f64 = double(float(ctx.f7.f64 - ctx.f6.f64));
	// rlwinm r31,r26,2,0,29
	ctx.r31.u64 = __builtin_rotateleft64(ctx.r26.u32 | (ctx.r26.u64 << 32), 2) & 0xFFFFFFFC;
	// fadds f6,f4,f5
	ctx.f6.f64 = double(float(ctx.f4.f64 + ctx.f5.f64));
	// rlwinm r3,r25,2,0,29
	ctx.r3.u64 = __builtin_rotateleft64(ctx.r25.u32 | (ctx.r25.u64 << 32), 2) & 0xFFFFFFFC;
	// fsubs f5,f4,f5
	ctx.f5.f64 = double(float(ctx.f4.f64 - ctx.f5.f64));
	// addi r6,r5,3
	ctx.r6.s64 = ctx.r5.s64 + 3;
	// fadds f4,f2,f3
	ctx.f4.f64 = double(float(ctx.f2.f64 + ctx.f3.f64));
	// addi r7,r7,3
	ctx.r7.s64 = ctx.r7.s64 + 3;
	// fsubs f3,f2,f3
	ctx.f3.f64 = double(float(ctx.f2.f64 - ctx.f3.f64));
	// rlwinm r6,r6,2,0,29
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
	// fneg f1,f10
	ctx.f1.u64 = ctx.f10.u64 ^ 0x8000000000000000;
	// rlwinm r7,r7,2,0,29
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
	// fadds f2,f6,f31
	ctx.f2.f64 = double(float(ctx.f6.f64 + ctx.f31.f64));
	// stfs f2,0(r10)
	temp.f32 = float(ctx.f2.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// fsubs f6,f31,f6
	ctx.f6.f64 = double(float(ctx.f31.f64 - ctx.f6.f64));
	// fadds f2,f4,f8
	ctx.f2.f64 = double(float(ctx.f4.f64 + ctx.f8.f64));
	// stfs f2,4(r10)
	temp.f32 = float(ctx.f2.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// stfs f6,0(r9)
	temp.f32 = float(ctx.f6.f64);
	REX_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// fsubs f4,f8,f4
	ctx.f4.f64 = double(float(ctx.f8.f64 - ctx.f4.f64));
	// fsubs f8,f9,f3
	ctx.f8.f64 = double(float(ctx.f9.f64 - ctx.f3.f64));
	// stfs f4,4(r9)
	temp.f32 = float(ctx.f4.f64);
	REX_STORE_U32(ctx.r9.u32 + 4, temp.u32);
	// fadds f6,f5,f7
	ctx.f6.f64 = double(float(ctx.f5.f64 + ctx.f7.f64));
	// fadds f9,f3,f9
	ctx.f9.f64 = double(float(ctx.f3.f64 + ctx.f9.f64));
	// fsubs f4,f8,f6
	ctx.f4.f64 = double(float(ctx.f8.f64 - ctx.f6.f64));
	// fadds f6,f6,f8
	ctx.f6.f64 = double(float(ctx.f6.f64 + ctx.f8.f64));
	// fsubs f8,f7,f5
	ctx.f8.f64 = double(float(ctx.f7.f64 - ctx.f5.f64));
	// fmuls f7,f4,f10
	ctx.f7.f64 = double(float(ctx.f4.f64 * ctx.f10.f64));
	// stfs f7,0(r11)
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// fmuls f10,f6,f10
	ctx.f10.f64 = double(float(ctx.f6.f64 * ctx.f10.f64));
	// stfs f10,4(r11)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// fadds f7,f8,f9
	ctx.f7.f64 = double(float(ctx.f8.f64 + ctx.f9.f64));
	// lfsx f10,r31,r4
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + ctx.r4.u32);
	ctx.f10.f64 = double(temp.f32);
	// fsubs f9,f8,f9
	ctx.f9.f64 = double(float(ctx.f8.f64 - ctx.f9.f64));
	// lfsx f3,r6,r4
	temp.u32 = REX_LOAD_U32(ctx.r6.u32 + ctx.r4.u32);
	ctx.f3.f64 = double(temp.f32);
	// fmuls f8,f7,f1
	ctx.f8.f64 = double(float(ctx.f7.f64 * ctx.f1.f64));
	// stfs f8,0(r8)
	temp.f32 = float(ctx.f8.f64);
	REX_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// fmuls f9,f9,f1
	ctx.f9.f64 = double(float(ctx.f9.f64 * ctx.f1.f64));
	// stfs f9,4(r8)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r8.u32 + 4, temp.u32);
	// lfsx f8,r28,r4
	temp.u32 = REX_LOAD_U32(ctx.r28.u32 + ctx.r4.u32);
	ctx.f8.f64 = double(temp.f32);
	// lfsx f9,r27,r4
	temp.u32 = REX_LOAD_U32(ctx.r27.u32 + ctx.r4.u32);
	ctx.f9.f64 = double(temp.f32);
	// lfsx f6,r30,r4
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + ctx.r4.u32);
	ctx.f6.f64 = double(temp.f32);
	// fadds f4,f9,f8
	ctx.f4.f64 = double(float(ctx.f9.f64 + ctx.f8.f64));
	// lfsx f7,r29,r4
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + ctx.r4.u32);
	ctx.f7.f64 = double(temp.f32);
	// fsubs f9,f9,f8
	ctx.f9.f64 = double(float(ctx.f9.f64 - ctx.f8.f64));
	// lfsx f5,r3,r4
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + ctx.r4.u32);
	ctx.f5.f64 = double(temp.f32);
	// fadds f8,f7,f6
	ctx.f8.f64 = double(float(ctx.f7.f64 + ctx.f6.f64));
	// fsubs f7,f7,f6
	ctx.f7.f64 = double(float(ctx.f7.f64 - ctx.f6.f64));
	// fadds f6,f5,f10
	ctx.f6.f64 = double(float(ctx.f5.f64 + ctx.f10.f64));
	// fsubs f10,f5,f10
	ctx.f10.f64 = double(float(ctx.f5.f64 - ctx.f10.f64));
	// lfsx f5,r7,r4
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + ctx.r4.u32);
	ctx.f5.f64 = double(temp.f32);
	// fadds f2,f6,f4
	ctx.f2.f64 = double(float(ctx.f6.f64 + ctx.f4.f64));
	// stfsx f2,r27,r4
	temp.f32 = float(ctx.f2.f64);
	REX_STORE_U32(ctx.r27.u32 + ctx.r4.u32, temp.u32);
	// fsubs f4,f4,f6
	ctx.f4.f64 = double(float(ctx.f4.f64 - ctx.f6.f64));
	// fadds f6,f5,f3
	ctx.f6.f64 = double(float(ctx.f5.f64 + ctx.f3.f64));
	// fsubs f5,f5,f3
	ctx.f5.f64 = double(float(ctx.f5.f64 - ctx.f3.f64));
	// fadds f3,f6,f8
	ctx.f3.f64 = double(float(ctx.f6.f64 + ctx.f8.f64));
	// stfsx f3,r29,r4
	temp.f32 = float(ctx.f3.f64);
	REX_STORE_U32(ctx.r29.u32 + ctx.r4.u32, temp.u32);
	// fsubs f6,f8,f6
	ctx.f6.f64 = double(float(ctx.f8.f64 - ctx.f6.f64));
	// stfsx f6,r7,r4
	temp.f32 = float(ctx.f6.f64);
	REX_STORE_U32(ctx.r7.u32 + ctx.r4.u32, temp.u32);
	// fadds f6,f10,f7
	ctx.f6.f64 = double(float(ctx.f10.f64 + ctx.f7.f64));
	// stfsx f4,r3,r4
	temp.f32 = float(ctx.f4.f64);
	REX_STORE_U32(ctx.r3.u32 + ctx.r4.u32, temp.u32);
	// fsubs f8,f9,f5
	ctx.f8.f64 = double(float(ctx.f9.f64 - ctx.f5.f64));
	// fsubs f10,f7,f10
	ctx.f10.f64 = double(float(ctx.f7.f64 - ctx.f10.f64));
	// fmuls f4,f13,f6
	ctx.f4.f64 = double(float(ctx.f13.f64 * ctx.f6.f64));
	// fmuls f3,f13,f8
	ctx.f3.f64 = double(float(ctx.f13.f64 * ctx.f8.f64));
	// fadds f13,f5,f9
	ctx.f13.f64 = double(float(ctx.f5.f64 + ctx.f9.f64));
	// fmsubs f9,f0,f8,f4
	ctx.f9.f64 = double(float(std::fma(ctx.f0.f64, ctx.f8.f64, -ctx.f4.f64)));
	// stfsx f9,r28,r4
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r28.u32 + ctx.r4.u32, temp.u32);
	// fmadds f0,f0,f6,f3
	ctx.f0.f64 = double(float(std::fma(ctx.f0.f64, ctx.f6.f64, ctx.f3.f64)));
	// stfsx f0,r30,r4
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r30.u32 + ctx.r4.u32, temp.u32);
	// fmuls f0,f11,f13
	ctx.f0.f64 = double(float(ctx.f11.f64 * ctx.f13.f64));
	// fmuls f13,f12,f13
	ctx.f13.f64 = double(float(ctx.f12.f64 * ctx.f13.f64));
	// fmadds f0,f12,f10,f0
	ctx.f0.f64 = double(float(std::fma(ctx.f12.f64, ctx.f10.f64, ctx.f0.f64)));
	// stfsx f0,r31,r4
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + ctx.r4.u32, temp.u32);
	// fmsubs f0,f11,f10,f13
	ctx.f0.f64 = double(float(std::fma(ctx.f11.f64, ctx.f10.f64, -ctx.f13.f64)));
	// stfsx f0,r6,r4
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r6.u32 + ctx.r4.u32, temp.u32);
	// addi r12,r1,-64
	ctx.r12.s64 = ctx.r1.s64 + -64;
	// bl 0x82a0133c
	ctx.lr = 0x82A81248;
	__restfpr_14(ctx, base);
	// b 0x829ff80c
	__restgprlr_25(ctx, base);
	return;
}

DEFINE_REX_FUNC(sub_82A81250) {
	REX_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7bc
	ctx.lr = 0x82A81258;
	__savegprlr_25(ctx, base);
	// addi r12,r1,-64
	ctx.r12.s64 = ctx.r1.s64 + -64;
	// bl 0x82a012f0
	ctx.lr = 0x82A81260;
	__savefpr_14(ctx, base);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f10,4(r4)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r4.u32 + 4);
	ctx.f10.f64 = double(temp.f32);
	// srawi r29,r3,3
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0x7) != 0);
	ctx.r29.s64 = ctx.r3.s32 >> 3;
	// fneg f1,f10
	ctx.f1.u64 = ctx.f10.u64 ^ 0x8000000000000000;
	// lfs f9,0(r4)
	temp.u32 = REX_LOAD_U32(ctx.r4.u32 + 0);
	ctx.f9.f64 = double(temp.f32);
	// addi r30,r5,8
	ctx.r30.s64 = ctx.r5.s64 + 8;
	// addi r27,r29,-2
	ctx.r27.s64 = ctx.r29.s64 + -2;
	// lfs f0,3400(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 3400);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// fmr f12,f0
	ctx.f12.f64 = ctx.f0.f64;
	// cmpwi cr6,r27,2
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 2, ctx.xer);
	// lfs f13,2612(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 2612);
	ctx.f13.f64 = double(temp.f32);
	// rlwinm r11,r29,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 1) & 0xFFFFFFFE;
	// fmr f11,f13
	ctx.f11.f64 = ctx.f13.f64;
	// rlwinm r9,r11,1,0,30
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// rlwinm r10,r11,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r8,r9,r11
	ctx.r8.u64 = ctx.r9.u64 + ctx.r11.u64;
	// rlwinm r9,r9,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r8,r8,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 2) & 0xFFFFFFFC;
	// add r9,r9,r4
	ctx.r9.u64 = ctx.r9.u64 + ctx.r4.u64;
	// add r10,r10,r4
	ctx.r10.u64 = ctx.r10.u64 + ctx.r4.u64;
	// add r8,r8,r4
	ctx.r8.u64 = ctx.r8.u64 + ctx.r4.u64;
	// lfs f6,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f6.f64 = double(temp.f32);
	// lfs f8,0(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f8.f64 = double(temp.f32);
	// fadds f2,f9,f6
	ctx.f2.f64 = double(float(ctx.f9.f64 + ctx.f6.f64));
	// lfs f4,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f4.f64 = double(temp.f32);
	// fsubs f9,f9,f6
	ctx.f9.f64 = double(float(ctx.f9.f64 - ctx.f6.f64));
	// lfs f5,4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 4);
	ctx.f5.f64 = double(temp.f32);
	// fadds f6,f8,f4
	ctx.f6.f64 = double(float(ctx.f8.f64 + ctx.f4.f64));
	// lfs f7,4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f7.f64 = double(temp.f32);
	// fsubs f10,f5,f10
	ctx.f10.f64 = double(float(ctx.f5.f64 - ctx.f10.f64));
	// lfs f3,4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f3.f64 = double(temp.f32);
	// fsubs f1,f1,f5
	ctx.f1.f64 = double(float(ctx.f1.f64 - ctx.f5.f64));
	// fadds f5,f7,f3
	ctx.f5.f64 = double(float(ctx.f7.f64 + ctx.f3.f64));
	// fsubs f7,f7,f3
	ctx.f7.f64 = double(float(ctx.f7.f64 - ctx.f3.f64));
	// fsubs f8,f8,f4
	ctx.f8.f64 = double(float(ctx.f8.f64 - ctx.f4.f64));
	// fadds f4,f6,f2
	ctx.f4.f64 = double(float(ctx.f6.f64 + ctx.f2.f64));
	// stfs f4,0(r4)
	temp.f32 = float(ctx.f4.f64);
	REX_STORE_U32(ctx.r4.u32 + 0, temp.u32);
	// fsubs f6,f2,f6
	ctx.f6.f64 = double(float(ctx.f2.f64 - ctx.f6.f64));
	// fsubs f4,f1,f5
	ctx.f4.f64 = double(float(ctx.f1.f64 - ctx.f5.f64));
	// stfs f4,4(r4)
	temp.f32 = float(ctx.f4.f64);
	REX_STORE_U32(ctx.r4.u32 + 4, temp.u32);
	// stfs f6,0(r10)
	temp.f32 = float(ctx.f6.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// fadds f6,f5,f1
	ctx.f6.f64 = double(float(ctx.f5.f64 + ctx.f1.f64));
	// stfs f6,4(r10)
	temp.f32 = float(ctx.f6.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// fadds f6,f7,f9
	ctx.f6.f64 = double(float(ctx.f7.f64 + ctx.f9.f64));
	// stfs f6,0(r9)
	temp.f32 = float(ctx.f6.f64);
	REX_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// fadds f6,f8,f10
	ctx.f6.f64 = double(float(ctx.f8.f64 + ctx.f10.f64));
	// stfs f6,4(r9)
	temp.f32 = float(ctx.f6.f64);
	REX_STORE_U32(ctx.r9.u32 + 4, temp.u32);
	// fsubs f10,f10,f8
	ctx.f10.f64 = double(float(ctx.f10.f64 - ctx.f8.f64));
	// fsubs f9,f9,f7
	ctx.f9.f64 = double(float(ctx.f9.f64 - ctx.f7.f64));
	// stfs f10,4(r8)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r8.u32 + 4, temp.u32);
	// stfs f9,0(r8)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// lfs f10,4(r5)
	temp.u32 = REX_LOAD_U32(ctx.r5.u32 + 4);
	ctx.f10.f64 = double(temp.f32);
	// lfs f9,8(r5)
	temp.u32 = REX_LOAD_U32(ctx.r5.u32 + 8);
	ctx.f9.f64 = double(temp.f32);
	// lfs f8,12(r5)
	temp.u32 = REX_LOAD_U32(ctx.r5.u32 + 12);
	ctx.f8.f64 = double(temp.f32);
	// stfs f10,-212(r1)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r1.u32 + -212, temp.u32);
	// stfs f9,-220(r1)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r1.u32 + -220, temp.u32);
	// stfs f8,-224(r1)
	temp.f32 = float(ctx.f8.f64);
	REX_STORE_U32(ctx.r1.u32 + -224, temp.u32);
	// ble cr6,0x82a816f4
	if (!ctx.cr6.gt) goto loc_82A816F4;
	// addi r8,r11,-4
	ctx.r8.s64 = ctx.r11.s64 + -4;
	// addi r7,r11,2
	ctx.r7.s64 = ctx.r11.s64 + 2;
	// addi r5,r11,-2
	ctx.r5.s64 = ctx.r11.s64 + -2;
	// addi r3,r27,-3
	ctx.r3.s64 = ctx.r27.s64 + -3;
	// rlwinm r9,r11,4,0,27
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 4) & 0xFFFFFFF0;
	// rlwinm r6,r8,2,0,29
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r7,r7,3,0,28
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 3) & 0xFFFFFFF8;
	// rlwinm r8,r5,3,0,28
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 3) & 0xFFFFFFF8;
	// add r9,r9,r4
	ctx.r9.u64 = ctx.r9.u64 + ctx.r4.u64;
	// rlwinm r5,r3,30,2,31
	ctx.r5.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 30) & 0x3FFFFFFF;
	// add r3,r7,r4
	ctx.r3.u64 = ctx.r7.u64 + ctx.r4.u64;
	// add r7,r8,r4
	ctx.r7.u64 = ctx.r8.u64 + ctx.r4.u64;
	// addi r8,r9,-16
	ctx.r8.s64 = ctx.r9.s64 + -16;
	// rlwinm r9,r11,1,0,30
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// addi r10,r11,4
	ctx.r10.s64 = ctx.r11.s64 + 4;
	// add r9,r11,r9
	ctx.r9.u64 = ctx.r11.u64 + ctx.r9.u64;
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r9,r9,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r28,r5,1
	ctx.r28.s64 = ctx.r5.s64 + 1;
	// add r9,r9,r4
	ctx.r9.u64 = ctx.r9.u64 + ctx.r4.u64;
	// addi r31,r4,16
	ctx.r31.s64 = ctx.r4.s64 + 16;
	// addi r5,r9,16
	ctx.r5.s64 = ctx.r9.s64 + 16;
	// add r10,r10,r4
	ctx.r10.u64 = ctx.r10.u64 + ctx.r4.u64;
	// add r6,r6,r4
	ctx.r6.u64 = ctx.r6.u64 + ctx.r4.u64;
	// addi r9,r9,-16
	ctx.r9.s64 = ctx.r9.s64 + -16;
loc_82A813B0:
	// lfs f7,-4(r3)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + -4);
	ctx.f7.f64 = double(temp.f32);
	// addi r30,r30,16
	ctx.r30.s64 = ctx.r30.s64 + 16;
	// lfs f10,-4(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + -4);
	ctx.f10.f64 = double(temp.f32);
	// lfs f1,4(r5)
	temp.u32 = REX_LOAD_U32(ctx.r5.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// fsubs f21,f7,f10
	ctx.f21.f64 = double(float(ctx.f7.f64 - ctx.f10.f64));
	// lfs f26,4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f26.f64 = double(temp.f32);
	// fneg f18,f10
	ctx.f18.u64 = ctx.f10.u64 ^ 0x8000000000000000;
	// fadds f10,f1,f26
	ctx.f10.f64 = double(float(ctx.f1.f64 + ctx.f26.f64));
	// stfs f10,-216(r1)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r1.u32 + -216, temp.u32);
	// lfs f9,4(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 4);
	ctx.f9.f64 = double(temp.f32);
	// fsubs f1,f26,f1
	ctx.f1.f64 = double(float(ctx.f26.f64 - ctx.f1.f64));
	// fneg f16,f9
	ctx.f16.u64 = ctx.f9.u64 ^ 0x8000000000000000;
	// lfs f8,-8(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + -8);
	ctx.f8.f64 = double(temp.f32);
	// lfs f31,-8(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + -8);
	ctx.f31.f64 = double(temp.f32);
	// lfs f25,-8(r30)
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + -8);
	ctx.f25.f64 = double(temp.f32);
	// fadds f19,f8,f31
	ctx.f19.f64 = double(float(ctx.f8.f64 + ctx.f31.f64));
	// lfs f6,0(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 0);
	ctx.f6.f64 = double(temp.f32);
	// fsubs f31,f31,f8
	ctx.f31.f64 = double(float(ctx.f31.f64 - ctx.f8.f64));
	// lfs f30,0(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 0);
	ctx.f30.f64 = double(temp.f32);
	// fadds f10,f0,f25
	ctx.f10.f64 = double(float(ctx.f0.f64 + ctx.f25.f64));
	// lfs f24,-4(r30)
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + -4);
	ctx.f24.f64 = double(temp.f32);
	// fadds f17,f30,f6
	ctx.f17.f64 = double(float(ctx.f30.f64 + ctx.f6.f64));
	// lfs f5,4(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 4);
	ctx.f5.f64 = double(temp.f32);
	// fsubs f6,f30,f6
	ctx.f6.f64 = double(float(ctx.f30.f64 - ctx.f6.f64));
	// lfs f23,0(r30)
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + 0);
	ctx.f23.f64 = double(temp.f32);
	// fsubs f20,f5,f9
	ctx.f20.f64 = double(float(ctx.f5.f64 - ctx.f9.f64));
	// lfs f4,-8(r5)
	temp.u32 = REX_LOAD_U32(ctx.r5.u32 + -8);
	ctx.f4.f64 = double(temp.f32);
	// fadds f8,f13,f24
	ctx.f8.f64 = double(float(ctx.f13.f64 + ctx.f24.f64));
	// lfs f29,-8(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + -8);
	ctx.f29.f64 = double(temp.f32);
	// fadds f30,f23,f12
	ctx.f30.f64 = double(float(ctx.f23.f64 + ctx.f12.f64));
	// fsubs f5,f16,f5
	ctx.f5.f64 = double(float(ctx.f16.f64 - ctx.f5.f64));
	// lfs f3,-4(r5)
	temp.u32 = REX_LOAD_U32(ctx.r5.u32 + -4);
	ctx.f3.f64 = double(temp.f32);
	// lfs f28,-4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + -4);
	ctx.f28.f64 = double(temp.f32);
	// fadds f16,f4,f29
	ctx.f16.f64 = double(float(ctx.f4.f64 + ctx.f29.f64));
	// lfs f2,0(r5)
	temp.u32 = REX_LOAD_U32(ctx.r5.u32 + 0);
	ctx.f2.f64 = double(temp.f32);
	// fsubs f18,f18,f7
	ctx.f18.f64 = double(float(ctx.f18.f64 - ctx.f7.f64));
	// lfs f27,0(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f27.f64 = double(temp.f32);
	// fadds f15,f3,f28
	ctx.f15.f64 = double(float(ctx.f3.f64 + ctx.f28.f64));
	// fadds f14,f27,f2
	ctx.f14.f64 = double(float(ctx.f27.f64 + ctx.f2.f64));
	// lfs f22,4(r30)
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + 4);
	ctx.f22.f64 = double(temp.f32);
	// fsubs f4,f29,f4
	ctx.f4.f64 = double(float(ctx.f29.f64 - ctx.f4.f64));
	// lfs f9,-220(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -220);
	ctx.f9.f64 = double(temp.f32);
	// fsubs f29,f11,f22
	ctx.f29.f64 = double(float(ctx.f11.f64 - ctx.f22.f64));
	// lfs f7,-224(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -224);
	ctx.f7.f64 = double(temp.f32);
	// fmuls f10,f10,f9
	ctx.f10.f64 = double(float(ctx.f10.f64 * ctx.f9.f64));
	// fmuls f9,f8,f9
	ctx.f9.f64 = double(float(ctx.f8.f64 * ctx.f9.f64));
	// fmuls f8,f30,f7
	ctx.f8.f64 = double(float(ctx.f30.f64 * ctx.f7.f64));
	// fsubs f3,f28,f3
	ctx.f3.f64 = double(float(ctx.f28.f64 - ctx.f3.f64));
	// fadds f30,f16,f19
	ctx.f30.f64 = double(float(ctx.f16.f64 + ctx.f19.f64));
	// stfs f30,-8(r31)
	temp.f32 = float(ctx.f30.f64);
	REX_STORE_U32(ctx.r31.u32 + -8, temp.u32);
	// fsubs f2,f27,f2
	ctx.f2.f64 = double(float(ctx.f27.f64 - ctx.f2.f64));
	// fsubs f30,f18,f15
	ctx.f30.f64 = double(float(ctx.f18.f64 - ctx.f15.f64));
	// stfs f30,-4(r31)
	temp.f32 = float(ctx.f30.f64);
	REX_STORE_U32(ctx.r31.u32 + -4, temp.u32);
	// fadds f30,f14,f17
	ctx.f30.f64 = double(float(ctx.f14.f64 + ctx.f17.f64));
	// stfs f30,0(r31)
	temp.f32 = float(ctx.f30.f64);
	REX_STORE_U32(ctx.r31.u32 + 0, temp.u32);
	// fmr f0,f25
	ctx.f0.f64 = ctx.f25.f64;
	// fmuls f7,f29,f7
	ctx.f7.f64 = double(float(ctx.f29.f64 * ctx.f7.f64));
	// fmr f13,f24
	ctx.f13.f64 = ctx.f24.f64;
	// fmr f12,f23
	ctx.f12.f64 = ctx.f23.f64;
	// fneg f11,f22
	ctx.f11.u64 = ctx.f22.u64 ^ 0x8000000000000000;
	// lfs f30,-216(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -216);
	ctx.f30.f64 = double(temp.f32);
	// fsubs f29,f5,f30
	ctx.f29.f64 = double(float(ctx.f5.f64 - ctx.f30.f64));
	// stfs f29,4(r31)
	temp.f32 = float(ctx.f29.f64);
	REX_STORE_U32(ctx.r31.u32 + 4, temp.u32);
	// fadds f5,f30,f5
	ctx.f5.f64 = double(float(ctx.f30.f64 + ctx.f5.f64));
	// stfs f5,4(r10)
	temp.f32 = float(ctx.f5.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// fadds f5,f3,f31
	ctx.f5.f64 = double(float(ctx.f3.f64 + ctx.f31.f64));
	// fadds f30,f4,f21
	ctx.f30.f64 = double(float(ctx.f4.f64 + ctx.f21.f64));
	// fsubs f29,f19,f16
	ctx.f29.f64 = double(float(ctx.f19.f64 - ctx.f16.f64));
	// stfs f29,-8(r10)
	temp.f32 = float(ctx.f29.f64);
	REX_STORE_U32(ctx.r10.u32 + -8, temp.u32);
	// fadds f29,f15,f18
	ctx.f29.f64 = double(float(ctx.f15.f64 + ctx.f18.f64));
	// stfs f29,-4(r10)
	temp.f32 = float(ctx.f29.f64);
	REX_STORE_U32(ctx.r10.u32 + -4, temp.u32);
	// fsubs f29,f17,f14
	ctx.f29.f64 = double(float(ctx.f17.f64 - ctx.f14.f64));
	// stfs f29,0(r10)
	temp.f32 = float(ctx.f29.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// fmr f29,f5
	ctx.f29.f64 = ctx.f5.f64;
	// fmuls f28,f9,f30
	ctx.f28.f64 = double(float(ctx.f9.f64 * ctx.f30.f64));
	// fmr f27,f30
	ctx.f27.f64 = ctx.f30.f64;
	// fmuls f26,f9,f5
	ctx.f26.f64 = double(float(ctx.f9.f64 * ctx.f5.f64));
	// fadds f5,f1,f6
	ctx.f5.f64 = double(float(ctx.f1.f64 + ctx.f6.f64));
	// fadds f30,f2,f20
	ctx.f30.f64 = double(float(ctx.f2.f64 + ctx.f20.f64));
	// fmsubs f29,f10,f29,f28
	ctx.f29.f64 = double(float(std::fma(ctx.f10.f64, ctx.f29.f64, -ctx.f28.f64)));
	// stfs f29,-8(r3)
	temp.f32 = float(ctx.f29.f64);
	REX_STORE_U32(ctx.r3.u32 + -8, temp.u32);
	// fmadds f29,f10,f27,f26
	ctx.f29.f64 = double(float(std::fma(ctx.f10.f64, ctx.f27.f64, ctx.f26.f64)));
	// stfs f29,-4(r3)
	temp.f32 = float(ctx.f29.f64);
	REX_STORE_U32(ctx.r3.u32 + -4, temp.u32);
	// fmr f29,f5
	ctx.f29.f64 = ctx.f5.f64;
	// addi r10,r10,16
	ctx.r10.s64 = ctx.r10.s64 + 16;
	// fmuls f27,f13,f5
	ctx.f27.f64 = double(float(ctx.f13.f64 * ctx.f5.f64));
	// addi r31,r31,16
	ctx.r31.s64 = ctx.r31.s64 + 16;
	// fmuls f28,f13,f30
	ctx.f28.f64 = double(float(ctx.f13.f64 * ctx.f30.f64));
	// fsubs f5,f21,f4
	ctx.f5.f64 = double(float(ctx.f21.f64 - ctx.f4.f64));
	// fsubs f4,f31,f3
	ctx.f4.f64 = double(float(ctx.f31.f64 - ctx.f3.f64));
	// fsubs f6,f6,f1
	ctx.f6.f64 = double(float(ctx.f6.f64 - ctx.f1.f64));
	// fmsubs f3,f0,f29,f28
	ctx.f3.f64 = double(float(std::fma(ctx.f0.f64, ctx.f29.f64, -ctx.f28.f64)));
	// stfs f3,0(r3)
	temp.f32 = float(ctx.f3.f64);
	REX_STORE_U32(ctx.r3.u32 + 0, temp.u32);
	// fmadds f3,f0,f30,f27
	ctx.f3.f64 = double(float(std::fma(ctx.f0.f64, ctx.f30.f64, ctx.f27.f64)));
	// stfs f3,4(r3)
	temp.f32 = float(ctx.f3.f64);
	REX_STORE_U32(ctx.r3.u32 + 4, temp.u32);
	// fmuls f31,f8,f4
	ctx.f31.f64 = double(float(ctx.f8.f64 * ctx.f4.f64));
	// addi r3,r3,16
	ctx.r3.s64 = ctx.r3.s64 + 16;
	// fmr f30,f5
	ctx.f30.f64 = ctx.f5.f64;
	// fmuls f4,f7,f4
	ctx.f4.f64 = double(float(ctx.f7.f64 * ctx.f4.f64));
	// fmr f3,f5
	ctx.f3.f64 = ctx.f5.f64;
	// fsubs f5,f20,f2
	ctx.f5.f64 = double(float(ctx.f20.f64 - ctx.f2.f64));
	// fmsubs f4,f8,f30,f4
	ctx.f4.f64 = double(float(std::fma(ctx.f8.f64, ctx.f30.f64, -ctx.f4.f64)));
	// stfs f4,-4(r5)
	temp.f32 = float(ctx.f4.f64);
	REX_STORE_U32(ctx.r5.u32 + -4, temp.u32);
	// fmuls f4,f12,f6
	ctx.f4.f64 = double(float(ctx.f12.f64 * ctx.f6.f64));
	// fmuls f6,f11,f6
	ctx.f6.f64 = double(float(ctx.f11.f64 * ctx.f6.f64));
	// fmadds f3,f7,f3,f31
	ctx.f3.f64 = double(float(std::fma(ctx.f7.f64, ctx.f3.f64, ctx.f31.f64)));
	// stfs f3,-8(r5)
	temp.f32 = float(ctx.f3.f64);
	REX_STORE_U32(ctx.r5.u32 + -8, temp.u32);
	// fmadds f4,f11,f5,f4
	ctx.f4.f64 = double(float(std::fma(ctx.f11.f64, ctx.f5.f64, ctx.f4.f64)));
	// stfs f4,0(r5)
	temp.f32 = float(ctx.f4.f64);
	REX_STORE_U32(ctx.r5.u32 + 0, temp.u32);
	// fmsubs f6,f12,f5,f6
	ctx.f6.f64 = double(float(std::fma(ctx.f12.f64, ctx.f5.f64, -ctx.f6.f64)));
	// stfs f6,4(r5)
	temp.f32 = float(ctx.f6.f64);
	REX_STORE_U32(ctx.r5.u32 + 4, temp.u32);
	// lfs f6,12(r6)
	temp.u32 = REX_LOAD_U32(ctx.r6.u32 + 12);
	ctx.f6.f64 = double(temp.f32);
	// addi r5,r5,16
	ctx.r5.s64 = ctx.r5.s64 + 16;
	// fneg f24,f6
	ctx.f24.u64 = ctx.f6.u64 ^ 0x8000000000000000;
	// lfs f3,8(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 8);
	ctx.f3.f64 = double(temp.f32);
	// lfs f4,8(r6)
	temp.u32 = REX_LOAD_U32(ctx.r6.u32 + 8);
	ctx.f4.f64 = double(temp.f32);
	// lfs f31,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f31.f64 = double(temp.f32);
	// fadds f25,f3,f4
	ctx.f25.f64 = double(float(ctx.f3.f64 + ctx.f4.f64));
	// lfs f2,12(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 12);
	ctx.f2.f64 = double(temp.f32);
	// fsubs f4,f4,f3
	ctx.f4.f64 = double(float(ctx.f4.f64 - ctx.f3.f64));
	// lfs f1,0(r6)
	temp.u32 = REX_LOAD_U32(ctx.r6.u32 + 0);
	ctx.f1.f64 = double(temp.f32);
	// fsubs f6,f2,f6
	ctx.f6.f64 = double(float(ctx.f2.f64 - ctx.f6.f64));
	// lfs f5,4(r6)
	temp.u32 = REX_LOAD_U32(ctx.r6.u32 + 4);
	ctx.f5.f64 = double(temp.f32);
	// fadds f3,f31,f1
	ctx.f3.f64 = double(float(ctx.f31.f64 + ctx.f1.f64));
	// lfs f28,8(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 8);
	ctx.f28.f64 = double(temp.f32);
	// fneg f23,f5
	ctx.f23.u64 = ctx.f5.u64 ^ 0x8000000000000000;
	// lfs f27,12(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 12);
	ctx.f27.f64 = double(temp.f32);
	// fsubs f1,f1,f31
	ctx.f1.f64 = double(float(ctx.f1.f64 - ctx.f31.f64));
	// lfs f26,12(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 12);
	ctx.f26.f64 = double(temp.f32);
	// lfs f29,8(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 8);
	ctx.f29.f64 = double(temp.f32);
	// fsubs f2,f24,f2
	ctx.f2.f64 = double(float(ctx.f24.f64 - ctx.f2.f64));
	// lfs f30,4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 4);
	ctx.f30.f64 = double(temp.f32);
	// fadds f24,f26,f27
	ctx.f24.f64 = double(float(ctx.f26.f64 + ctx.f27.f64));
	// lfs f22,4(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 4);
	ctx.f22.f64 = double(temp.f32);
	// fadds f31,f28,f29
	ctx.f31.f64 = double(float(ctx.f28.f64 + ctx.f29.f64));
	// fsubs f29,f28,f29
	ctx.f29.f64 = double(float(ctx.f28.f64 - ctx.f29.f64));
	// fsubs f28,f26,f27
	ctx.f28.f64 = double(float(ctx.f26.f64 - ctx.f27.f64));
	// lfs f27,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f27.f64 = double(temp.f32);
	// fsubs f5,f30,f5
	ctx.f5.f64 = double(float(ctx.f30.f64 - ctx.f5.f64));
	// lfs f26,4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f26.f64 = double(temp.f32);
	// fsubs f30,f23,f30
	ctx.f30.f64 = double(float(ctx.f23.f64 - ctx.f30.f64));
	// lfs f23,0(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 0);
	ctx.f23.f64 = double(temp.f32);
	// fsubs f20,f2,f24
	ctx.f20.f64 = double(float(ctx.f2.f64 - ctx.f24.f64));
	// stfs f20,12(r6)
	temp.f32 = float(ctx.f20.f64);
	REX_STORE_U32(ctx.r6.u32 + 12, temp.u32);
	// fadds f18,f24,f2
	ctx.f18.f64 = double(float(ctx.f24.f64 + ctx.f2.f64));
	// fadds f21,f31,f25
	ctx.f21.f64 = double(float(ctx.f31.f64 + ctx.f25.f64));
	// stfs f21,8(r6)
	temp.f32 = float(ctx.f21.f64);
	REX_STORE_U32(ctx.r6.u32 + 8, temp.u32);
	// fsubs f19,f25,f31
	ctx.f19.f64 = double(float(ctx.f25.f64 - ctx.f31.f64));
	// fadds f2,f29,f6
	ctx.f2.f64 = double(float(ctx.f29.f64 + ctx.f6.f64));
	// fsubs f25,f23,f27
	ctx.f25.f64 = double(float(ctx.f23.f64 - ctx.f27.f64));
	// fadds f31,f28,f4
	ctx.f31.f64 = double(float(ctx.f28.f64 + ctx.f4.f64));
	// fadds f27,f27,f23
	ctx.f27.f64 = double(float(ctx.f27.f64 + ctx.f23.f64));
	// fsubs f24,f22,f26
	ctx.f24.f64 = double(float(ctx.f22.f64 - ctx.f26.f64));
	// fmuls f23,f10,f2
	ctx.f23.f64 = double(float(ctx.f10.f64 * ctx.f2.f64));
	// fmr f21,f2
	ctx.f21.f64 = ctx.f2.f64;
	// fadds f2,f22,f26
	ctx.f2.f64 = double(float(ctx.f22.f64 + ctx.f26.f64));
	// fmuls f22,f10,f31
	ctx.f22.f64 = double(float(ctx.f10.f64 * ctx.f31.f64));
	// fmr f26,f31
	ctx.f26.f64 = ctx.f31.f64;
	// fadds f20,f27,f3
	ctx.f20.f64 = double(float(ctx.f27.f64 + ctx.f3.f64));
	// stfs f20,0(r6)
	temp.f32 = float(ctx.f20.f64);
	REX_STORE_U32(ctx.r6.u32 + 0, temp.u32);
	// fadds f10,f25,f5
	ctx.f10.f64 = double(float(ctx.f25.f64 + ctx.f5.f64));
	// fadds f31,f24,f1
	ctx.f31.f64 = double(float(ctx.f24.f64 + ctx.f1.f64));
	// fsubs f3,f3,f27
	ctx.f3.f64 = double(float(ctx.f3.f64 - ctx.f27.f64));
	// addi r28,r28,-1
	ctx.r28.s64 = ctx.r28.s64 + -1;
	// fsubs f27,f30,f2
	ctx.f27.f64 = double(float(ctx.f30.f64 - ctx.f2.f64));
	// stfs f27,4(r6)
	temp.f32 = float(ctx.f27.f64);
	REX_STORE_U32(ctx.r6.u32 + 4, temp.u32);
	// stfs f3,0(r7)
	temp.f32 = float(ctx.f3.f64);
	REX_STORE_U32(ctx.r7.u32 + 0, temp.u32);
	// fadds f2,f2,f30
	ctx.f2.f64 = double(float(ctx.f2.f64 + ctx.f30.f64));
	// stfs f2,4(r7)
	temp.f32 = float(ctx.f2.f64);
	REX_STORE_U32(ctx.r7.u32 + 4, temp.u32);
	// fmsubs f3,f9,f26,f23
	ctx.f3.f64 = double(float(std::fma(ctx.f9.f64, ctx.f26.f64, -ctx.f23.f64)));
	// stfs f19,8(r7)
	temp.f32 = float(ctx.f19.f64);
	REX_STORE_U32(ctx.r7.u32 + 8, temp.u32);
	// fmr f2,f10
	ctx.f2.f64 = ctx.f10.f64;
	// stfs f18,12(r7)
	temp.f32 = float(ctx.f18.f64);
	REX_STORE_U32(ctx.r7.u32 + 12, temp.u32);
	// fmadds f9,f9,f21,f22
	ctx.f9.f64 = double(float(std::fma(ctx.f9.f64, ctx.f21.f64, ctx.f22.f64)));
	// stfs f3,8(r9)
	temp.f32 = float(ctx.f3.f64);
	REX_STORE_U32(ctx.r9.u32 + 8, temp.u32);
	// fmuls f3,f0,f10
	ctx.f3.f64 = double(float(ctx.f0.f64 * ctx.f10.f64));
	// fmuls f30,f0,f31
	ctx.f30.f64 = double(float(ctx.f0.f64 * ctx.f31.f64));
	// stfs f9,12(r9)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r9.u32 + 12, temp.u32);
	// fsubs f9,f6,f29
	ctx.f9.f64 = double(float(ctx.f6.f64 - ctx.f29.f64));
	// addi r7,r7,-16
	ctx.r7.s64 = ctx.r7.s64 + -16;
	// fsubs f10,f4,f28
	ctx.f10.f64 = double(float(ctx.f4.f64 - ctx.f28.f64));
	// addi r6,r6,-16
	ctx.r6.s64 = ctx.r6.s64 + -16;
	// cmplwi cr6,r28,0
	ctx.cr6.compare<uint32_t>(ctx.r28.u32, 0, ctx.xer);
	// fmsubs f6,f13,f31,f3
	ctx.f6.f64 = double(float(std::fma(ctx.f13.f64, ctx.f31.f64, -ctx.f3.f64)));
	// stfs f6,0(r9)
	temp.f32 = float(ctx.f6.f64);
	REX_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// fmadds f6,f13,f2,f30
	ctx.f6.f64 = double(float(std::fma(ctx.f13.f64, ctx.f2.f64, ctx.f30.f64)));
	// stfs f6,4(r9)
	temp.f32 = float(ctx.f6.f64);
	REX_STORE_U32(ctx.r9.u32 + 4, temp.u32);
	// fmr f4,f9
	ctx.f4.f64 = ctx.f9.f64;
	// addi r9,r9,-16
	ctx.r9.s64 = ctx.r9.s64 + -16;
	// fmuls f6,f7,f10
	ctx.f6.f64 = double(float(ctx.f7.f64 * ctx.f10.f64));
	// fmuls f2,f8,f10
	ctx.f2.f64 = double(float(ctx.f8.f64 * ctx.f10.f64));
	// fmr f3,f9
	ctx.f3.f64 = ctx.f9.f64;
	// fsubs f10,f1,f24
	ctx.f10.f64 = double(float(ctx.f1.f64 - ctx.f24.f64));
	// fsubs f9,f5,f25
	ctx.f9.f64 = double(float(ctx.f5.f64 - ctx.f25.f64));
	// fmadds f8,f8,f4,f6
	ctx.f8.f64 = double(float(std::fma(ctx.f8.f64, ctx.f4.f64, ctx.f6.f64)));
	// stfs f8,8(r8)
	temp.f32 = float(ctx.f8.f64);
	REX_STORE_U32(ctx.r8.u32 + 8, temp.u32);
	// fmsubs f8,f7,f3,f2
	ctx.f8.f64 = double(float(std::fma(ctx.f7.f64, ctx.f3.f64, -ctx.f2.f64)));
	// stfs f8,12(r8)
	temp.f32 = float(ctx.f8.f64);
	REX_STORE_U32(ctx.r8.u32 + 12, temp.u32);
	// fmuls f8,f11,f10
	ctx.f8.f64 = double(float(ctx.f11.f64 * ctx.f10.f64));
	// fmuls f10,f12,f10
	ctx.f10.f64 = double(float(ctx.f12.f64 * ctx.f10.f64));
	// fmadds f8,f12,f9,f8
	ctx.f8.f64 = double(float(std::fma(ctx.f12.f64, ctx.f9.f64, ctx.f8.f64)));
	// stfs f8,0(r8)
	temp.f32 = float(ctx.f8.f64);
	REX_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// fmsubs f10,f11,f9,f10
	ctx.f10.f64 = double(float(std::fma(ctx.f11.f64, ctx.f9.f64, -ctx.f10.f64)));
	// stfs f10,4(r8)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r8.u32 + 4, temp.u32);
	// addi r8,r8,-16
	ctx.r8.s64 = ctx.r8.s64 + -16;
	// bne cr6,0x82a813b0
	if (!ctx.cr6.eq) goto loc_82A813B0;
	// lfs f8,-224(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -224);
	ctx.f8.f64 = double(temp.f32);
	// lfs f9,-220(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -220);
	ctx.f9.f64 = double(temp.f32);
	// lfs f10,-212(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -212);
	ctx.f10.f64 = double(temp.f32);
loc_82A816F4:
	// fadds f13,f13,f10
	ctx.fpscr.disableFlushMode();
	ctx.f13.f64 = double(float(ctx.f13.f64 + ctx.f10.f64));
	// rlwinm r10,r29,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 2) & 0xFFFFFFFC;
	// fadds f7,f0,f10
	ctx.f7.f64 = double(float(ctx.f0.f64 + ctx.f10.f64));
	// add r7,r11,r29
	ctx.r7.u64 = ctx.r11.u64 + ctx.r29.u64;
	// add r10,r10,r4
	ctx.r10.u64 = ctx.r10.u64 + ctx.r4.u64;
	// fsubs f12,f12,f10
	ctx.f12.f64 = double(float(ctx.f12.f64 - ctx.f10.f64));
	// addi r8,r7,-2
	ctx.r8.s64 = ctx.r7.s64 + -2;
	// fsubs f11,f11,f10
	ctx.f11.f64 = double(float(ctx.f11.f64 - ctx.f10.f64));
	// add r6,r7,r11
	ctx.r6.u64 = ctx.r7.u64 + ctx.r11.u64;
	// rlwinm r30,r8,2,0,29
	ctx.r30.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r8,r6,-2
	ctx.r8.s64 = ctx.r6.s64 + -2;
	// add r5,r6,r11
	ctx.r5.u64 = ctx.r6.u64 + ctx.r11.u64;
	// rlwinm r31,r8,2,0,29
	ctx.r31.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r11,r6,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
	// fmuls f0,f13,f9
	ctx.f0.f64 = double(float(ctx.f13.f64 * ctx.f9.f64));
	// rlwinm r9,r7,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
	// fmuls f13,f7,f9
	ctx.f13.f64 = double(float(ctx.f7.f64 * ctx.f9.f64));
	// lfs f9,-4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + -4);
	ctx.f9.f64 = double(temp.f32);
	// fneg f1,f9
	ctx.f1.u64 = ctx.f9.u64 ^ 0x8000000000000000;
	// rlwinm r8,r5,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 2) & 0xFFFFFFFC;
	// add r11,r11,r4
	ctx.r11.u64 = ctx.r11.u64 + ctx.r4.u64;
	// fmuls f12,f12,f8
	ctx.f12.f64 = double(float(ctx.f12.f64 * ctx.f8.f64));
	// addi r3,r5,-2
	ctx.r3.s64 = ctx.r5.s64 + -2;
	// fmuls f11,f11,f8
	ctx.f11.f64 = double(float(ctx.f11.f64 * ctx.f8.f64));
	// add r9,r9,r4
	ctx.r9.u64 = ctx.r9.u64 + ctx.r4.u64;
	// lfsx f4,r31,r4
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + ctx.r4.u32);
	ctx.f4.f64 = double(temp.f32);
	// add r8,r8,r4
	ctx.r8.u64 = ctx.r8.u64 + ctx.r4.u64;
	// lfsx f6,r30,r4
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + ctx.r4.u32);
	ctx.f6.f64 = double(temp.f32);
	// rlwinm r28,r27,2,0,29
	ctx.r28.u64 = __builtin_rotateleft64(ctx.r27.u32 | (ctx.r27.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r3,r3,2,0,29
	ctx.r3.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f5,-4(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -4);
	ctx.f5.f64 = double(temp.f32);
	// fsubs f9,f5,f9
	ctx.f9.f64 = double(float(ctx.f5.f64 - ctx.f9.f64));
	// lfs f7,-4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + -4);
	ctx.f7.f64 = double(temp.f32);
	// lfs f3,-4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + -4);
	ctx.f3.f64 = double(temp.f32);
	// lfsx f8,r28,r4
	temp.u32 = REX_LOAD_U32(ctx.r28.u32 + ctx.r4.u32);
	ctx.f8.f64 = double(temp.f32);
	// fsubs f1,f1,f5
	ctx.f1.f64 = double(float(ctx.f1.f64 - ctx.f5.f64));
	// lfsx f2,r3,r4
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + ctx.r4.u32);
	ctx.f2.f64 = double(temp.f32);
	// fadds f5,f7,f3
	ctx.f5.f64 = double(float(ctx.f7.f64 + ctx.f3.f64));
	// fadds f31,f8,f4
	ctx.f31.f64 = double(float(ctx.f8.f64 + ctx.f4.f64));
	// fsubs f8,f8,f4
	ctx.f8.f64 = double(float(ctx.f8.f64 - ctx.f4.f64));
	// fadds f4,f6,f2
	ctx.f4.f64 = double(float(ctx.f6.f64 + ctx.f2.f64));
	// fsubs f6,f6,f2
	ctx.f6.f64 = double(float(ctx.f6.f64 - ctx.f2.f64));
	// fsubs f7,f7,f3
	ctx.f7.f64 = double(float(ctx.f7.f64 - ctx.f3.f64));
	// fsubs f3,f1,f5
	ctx.f3.f64 = double(float(ctx.f1.f64 - ctx.f5.f64));
	// stfs f3,-4(r10)
	temp.f32 = float(ctx.f3.f64);
	REX_STORE_U32(ctx.r10.u32 + -4, temp.u32);
	// fadds f5,f5,f1
	ctx.f5.f64 = double(float(ctx.f5.f64 + ctx.f1.f64));
	// fadds f3,f4,f31
	ctx.f3.f64 = double(float(ctx.f4.f64 + ctx.f31.f64));
	// stfsx f3,r28,r4
	temp.f32 = float(ctx.f3.f64);
	REX_STORE_U32(ctx.r28.u32 + ctx.r4.u32, temp.u32);
	// fsubs f4,f31,f4
	ctx.f4.f64 = double(float(ctx.f31.f64 - ctx.f4.f64));
	// stfsx f4,r30,r4
	temp.f32 = float(ctx.f4.f64);
	REX_STORE_U32(ctx.r30.u32 + ctx.r4.u32, temp.u32);
	// fadds f4,f6,f9
	ctx.f4.f64 = double(float(ctx.f6.f64 + ctx.f9.f64));
	// stfs f5,-4(r9)
	temp.f32 = float(ctx.f5.f64);
	REX_STORE_U32(ctx.r9.u32 + -4, temp.u32);
	// fadds f5,f7,f8
	ctx.f5.f64 = double(float(ctx.f7.f64 + ctx.f8.f64));
	// fsubs f8,f8,f7
	ctx.f8.f64 = double(float(ctx.f8.f64 - ctx.f7.f64));
	// fsubs f9,f9,f6
	ctx.f9.f64 = double(float(ctx.f9.f64 - ctx.f6.f64));
	// fmuls f3,f0,f4
	ctx.f3.f64 = double(float(ctx.f0.f64 * ctx.f4.f64));
	// fmuls f2,f0,f5
	ctx.f2.f64 = double(float(ctx.f0.f64 * ctx.f5.f64));
	// fmsubs f7,f13,f5,f3
	ctx.f7.f64 = double(float(std::fma(ctx.f13.f64, ctx.f5.f64, -ctx.f3.f64)));
	// stfsx f7,r31,r4
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r31.u32 + ctx.r4.u32, temp.u32);
	// fmadds f7,f13,f4,f2
	ctx.f7.f64 = double(float(std::fma(ctx.f13.f64, ctx.f4.f64, ctx.f2.f64)));
	// stfs f7,-4(r11)
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r11.u32 + -4, temp.u32);
	// fmuls f7,f12,f8
	ctx.f7.f64 = double(float(ctx.f12.f64 * ctx.f8.f64));
	// addi r31,r6,2
	ctx.r31.s64 = ctx.r6.s64 + 2;
	// fmuls f8,f11,f8
	ctx.f8.f64 = double(float(ctx.f11.f64 * ctx.f8.f64));
	// lfs f4,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f4.f64 = double(temp.f32);
	// lfs f2,4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f2.f64 = double(temp.f32);
	// addi r6,r6,3
	ctx.r6.s64 = ctx.r6.s64 + 3;
	// fmadds f7,f11,f9,f7
	ctx.f7.f64 = double(float(std::fma(ctx.f11.f64, ctx.f9.f64, ctx.f7.f64)));
	// stfsx f7,r3,r4
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r3.u32 + ctx.r4.u32, temp.u32);
	// addi r3,r29,3
	ctx.r3.s64 = ctx.r29.s64 + 3;
	// fmsubs f9,f12,f9,f8
	ctx.f9.f64 = double(float(std::fma(ctx.f12.f64, ctx.f9.f64, -ctx.f8.f64)));
	// stfs f9,-4(r8)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r8.u32 + -4, temp.u32);
	// fneg f7,f10
	ctx.f7.u64 = ctx.f10.u64 ^ 0x8000000000000000;
	// rlwinm r27,r3,2,0,29
	ctx.r27.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f9,4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f9.f64 = double(temp.f32);
	// lfs f8,0(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f8.f64 = double(temp.f32);
	// addi r3,r29,2
	ctx.r3.s64 = ctx.r29.s64 + 2;
	// lfs f6,0(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f6.f64 = double(temp.f32);
	// lfs f5,4(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f5.f64 = double(temp.f32);
	// lfs f3,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f3.f64 = double(temp.f32);
	// lfs f1,4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// fneg f30,f9
	ctx.f30.u64 = ctx.f9.u64 ^ 0x8000000000000000;
	// addi r26,r5,2
	ctx.r26.s64 = ctx.r5.s64 + 2;
	// fadds f31,f8,f6
	ctx.f31.f64 = double(float(ctx.f8.f64 + ctx.f6.f64));
	// addi r25,r7,2
	ctx.r25.s64 = ctx.r7.s64 + 2;
	// fsubs f8,f8,f6
	ctx.f8.f64 = double(float(ctx.f8.f64 - ctx.f6.f64));
	// rlwinm r28,r3,2,0,29
	ctx.r28.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// fadds f6,f3,f4
	ctx.f6.f64 = double(float(ctx.f3.f64 + ctx.f4.f64));
	// rlwinm r29,r31,2,0,29
	ctx.r29.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 2) & 0xFFFFFFFC;
	// fsubs f9,f5,f9
	ctx.f9.f64 = double(float(ctx.f5.f64 - ctx.f9.f64));
	// rlwinm r3,r25,2,0,29
	ctx.r3.u64 = __builtin_rotateleft64(ctx.r25.u32 | (ctx.r25.u64 << 32), 2) & 0xFFFFFFFC;
	// fsubs f4,f3,f4
	ctx.f4.f64 = double(float(ctx.f3.f64 - ctx.f4.f64));
	// rlwinm r31,r26,2,0,29
	ctx.r31.u64 = __builtin_rotateleft64(ctx.r26.u32 | (ctx.r26.u64 << 32), 2) & 0xFFFFFFFC;
	// fadds f3,f1,f2
	ctx.f3.f64 = double(float(ctx.f1.f64 + ctx.f2.f64));
	// rlwinm r30,r6,2,0,29
	ctx.r30.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
	// fsubs f2,f1,f2
	ctx.f2.f64 = double(float(ctx.f1.f64 - ctx.f2.f64));
	// addi r6,r5,3
	ctx.r6.s64 = ctx.r5.s64 + 3;
	// rlwinm r6,r6,2,0,29
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
	// fsubs f5,f30,f5
	ctx.f5.f64 = double(float(ctx.f30.f64 - ctx.f5.f64));
	// fadds f1,f6,f31
	ctx.f1.f64 = double(float(ctx.f6.f64 + ctx.f31.f64));
	// stfs f1,0(r10)
	temp.f32 = float(ctx.f1.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// fsubs f1,f31,f6
	ctx.f1.f64 = double(float(ctx.f31.f64 - ctx.f6.f64));
	// fadds f6,f2,f8
	ctx.f6.f64 = double(float(ctx.f2.f64 + ctx.f8.f64));
	// fsubs f8,f8,f2
	ctx.f8.f64 = double(float(ctx.f8.f64 - ctx.f2.f64));
	// fsubs f31,f5,f3
	ctx.f31.f64 = double(float(ctx.f5.f64 - ctx.f3.f64));
	// stfs f31,4(r10)
	temp.f32 = float(ctx.f31.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// fadds f3,f3,f5
	ctx.f3.f64 = double(float(ctx.f3.f64 + ctx.f5.f64));
	// stfs f3,4(r9)
	temp.f32 = float(ctx.f3.f64);
	REX_STORE_U32(ctx.r9.u32 + 4, temp.u32);
	// fadds f5,f4,f9
	ctx.f5.f64 = double(float(ctx.f4.f64 + ctx.f9.f64));
	// stfs f1,0(r9)
	temp.f32 = float(ctx.f1.f64);
	REX_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// fsubs f9,f9,f4
	ctx.f9.f64 = double(float(ctx.f9.f64 - ctx.f4.f64));
	// fsubs f3,f6,f5
	ctx.f3.f64 = double(float(ctx.f6.f64 - ctx.f5.f64));
	// fadds f6,f5,f6
	ctx.f6.f64 = double(float(ctx.f5.f64 + ctx.f6.f64));
	// fmuls f5,f3,f10
	ctx.f5.f64 = double(float(ctx.f3.f64 * ctx.f10.f64));
	// stfs f5,0(r11)
	temp.f32 = float(ctx.f5.f64);
	REX_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// fmuls f10,f6,f10
	ctx.f10.f64 = double(float(ctx.f6.f64 * ctx.f10.f64));
	// stfs f10,4(r11)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// fadds f6,f9,f8
	ctx.f6.f64 = double(float(ctx.f9.f64 + ctx.f8.f64));
	// lfsx f10,r31,r4
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + ctx.r4.u32);
	ctx.f10.f64 = double(temp.f32);
	// fsubs f9,f9,f8
	ctx.f9.f64 = double(float(ctx.f9.f64 - ctx.f8.f64));
	// addi r11,r7,3
	ctx.r11.s64 = ctx.r7.s64 + 3;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// fmuls f8,f6,f7
	ctx.f8.f64 = double(float(ctx.f6.f64 * ctx.f7.f64));
	// stfs f8,0(r8)
	temp.f32 = float(ctx.f8.f64);
	REX_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// fmuls f9,f9,f7
	ctx.f9.f64 = double(float(ctx.f9.f64 * ctx.f7.f64));
	// stfs f9,4(r8)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r8.u32 + 4, temp.u32);
	// lfsx f7,r29,r4
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + ctx.r4.u32);
	ctx.f7.f64 = double(temp.f32);
	// lfsx f8,r28,r4
	temp.u32 = REX_LOAD_U32(ctx.r28.u32 + ctx.r4.u32);
	ctx.f8.f64 = double(temp.f32);
	// lfsx f5,r3,r4
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + ctx.r4.u32);
	ctx.f5.f64 = double(temp.f32);
	// fadds f4,f8,f7
	ctx.f4.f64 = double(float(ctx.f8.f64 + ctx.f7.f64));
	// lfsx f9,r27,r4
	temp.u32 = REX_LOAD_U32(ctx.r27.u32 + ctx.r4.u32);
	ctx.f9.f64 = double(temp.f32);
	// fsubs f8,f8,f7
	ctx.f8.f64 = double(float(ctx.f8.f64 - ctx.f7.f64));
	// fadds f7,f5,f10
	ctx.f7.f64 = double(float(ctx.f5.f64 + ctx.f10.f64));
	// lfsx f6,r30,r4
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + ctx.r4.u32);
	ctx.f6.f64 = double(temp.f32);
	// fneg f3,f9
	ctx.f3.u64 = ctx.f9.u64 ^ 0x8000000000000000;
	// fsubs f10,f5,f10
	ctx.f10.f64 = double(float(ctx.f5.f64 - ctx.f10.f64));
	// fsubs f9,f6,f9
	ctx.f9.f64 = double(float(ctx.f6.f64 - ctx.f9.f64));
	// fadds f5,f7,f4
	ctx.f5.f64 = double(float(ctx.f7.f64 + ctx.f4.f64));
	// fsubs f6,f3,f6
	ctx.f6.f64 = double(float(ctx.f3.f64 - ctx.f6.f64));
	// lfsx f3,r6,r4
	temp.u32 = REX_LOAD_U32(ctx.r6.u32 + ctx.r4.u32);
	ctx.f3.f64 = double(temp.f32);
	// fsubs f4,f4,f7
	ctx.f4.f64 = double(float(ctx.f4.f64 - ctx.f7.f64));
	// lfsx f7,r11,r4
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + ctx.r4.u32);
	ctx.f7.f64 = double(temp.f32);
	// stfsx f5,r28,r4
	temp.f32 = float(ctx.f5.f64);
	REX_STORE_U32(ctx.r28.u32 + ctx.r4.u32, temp.u32);
	// fadds f5,f7,f3
	ctx.f5.f64 = double(float(ctx.f7.f64 + ctx.f3.f64));
	// fsubs f7,f7,f3
	ctx.f7.f64 = double(float(ctx.f7.f64 - ctx.f3.f64));
	// fsubs f3,f6,f5
	ctx.f3.f64 = double(float(ctx.f6.f64 - ctx.f5.f64));
	// stfsx f3,r27,r4
	temp.f32 = float(ctx.f3.f64);
	REX_STORE_U32(ctx.r27.u32 + ctx.r4.u32, temp.u32);
	// fadds f5,f5,f6
	ctx.f5.f64 = double(float(ctx.f5.f64 + ctx.f6.f64));
	// stfsx f5,r11,r4
	temp.f32 = float(ctx.f5.f64);
	REX_STORE_U32(ctx.r11.u32 + ctx.r4.u32, temp.u32);
	// fadds f5,f10,f9
	ctx.f5.f64 = double(float(ctx.f10.f64 + ctx.f9.f64));
	// stfsx f4,r3,r4
	temp.f32 = float(ctx.f4.f64);
	REX_STORE_U32(ctx.r3.u32 + ctx.r4.u32, temp.u32);
	// fadds f6,f7,f8
	ctx.f6.f64 = double(float(ctx.f7.f64 + ctx.f8.f64));
	// fsubs f10,f9,f10
	ctx.f10.f64 = double(float(ctx.f9.f64 - ctx.f10.f64));
	// fmuls f4,f13,f5
	ctx.f4.f64 = double(float(ctx.f13.f64 * ctx.f5.f64));
	// fmuls f3,f13,f6
	ctx.f3.f64 = double(float(ctx.f13.f64 * ctx.f6.f64));
	// fsubs f13,f8,f7
	ctx.f13.f64 = double(float(ctx.f8.f64 - ctx.f7.f64));
	// fmsubs f9,f0,f6,f4
	ctx.f9.f64 = double(float(std::fma(ctx.f0.f64, ctx.f6.f64, -ctx.f4.f64)));
	// stfsx f9,r29,r4
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r29.u32 + ctx.r4.u32, temp.u32);
	// fmadds f0,f0,f5,f3
	ctx.f0.f64 = double(float(std::fma(ctx.f0.f64, ctx.f5.f64, ctx.f3.f64)));
	// stfsx f0,r30,r4
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r30.u32 + ctx.r4.u32, temp.u32);
	// fmuls f0,f11,f13
	ctx.f0.f64 = double(float(ctx.f11.f64 * ctx.f13.f64));
	// fmuls f13,f12,f13
	ctx.f13.f64 = double(float(ctx.f12.f64 * ctx.f13.f64));
	// fmadds f0,f12,f10,f0
	ctx.f0.f64 = double(float(std::fma(ctx.f12.f64, ctx.f10.f64, ctx.f0.f64)));
	// stfsx f0,r31,r4
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + ctx.r4.u32, temp.u32);
	// fmsubs f0,f11,f10,f13
	ctx.f0.f64 = double(float(std::fma(ctx.f11.f64, ctx.f10.f64, -ctx.f13.f64)));
	// stfsx f0,r6,r4
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r6.u32 + ctx.r4.u32, temp.u32);
	// addi r12,r1,-64
	ctx.r12.s64 = ctx.r1.s64 + -64;
	// bl 0x82a0133c
	ctx.lr = 0x82A81994;
	__restfpr_14(ctx, base);
	// b 0x829ff80c
	__restgprlr_25(ctx, base);
	return;
}

DEFINE_REX_FUNC(sub_82A81998) {
	REX_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c8
	ctx.lr = 0x82A819A0;
	__savegprlr_28(ctx, base);
	// stfd f31,-48(r1)
	ctx.fpscr.disableFlushMode();
	REX_STORE_U64(ctx.r1.u32 + -48, ctx.f31.u64);
	// srawi r28,r3,3
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0x7) != 0);
	ctx.r28.s64 = ctx.r3.s32 >> 3;
	// lfs f0,0(r4)
	temp.u32 = REX_LOAD_U32(ctx.r4.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,4(r4)
	temp.u32 = REX_LOAD_U32(ctx.r4.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// rlwinm r11,r28,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r28.u32 | (ctx.r28.u64 << 32), 1) & 0xFFFFFFFE;
	// cmpwi cr6,r28,2
	ctx.cr6.compare<int32_t>(ctx.r28.s32, 2, ctx.xer);
	// rlwinm r9,r11,1,0,30
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// rlwinm r10,r11,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r8,r9,r11
	ctx.r8.u64 = ctx.r9.u64 + ctx.r11.u64;
	// rlwinm r9,r9,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r8,r8,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 2) & 0xFFFFFFFC;
	// add r9,r9,r4
	ctx.r9.u64 = ctx.r9.u64 + ctx.r4.u64;
	// add r10,r10,r4
	ctx.r10.u64 = ctx.r10.u64 + ctx.r4.u64;
	// add r8,r8,r4
	ctx.r8.u64 = ctx.r8.u64 + ctx.r4.u64;
	// lfs f10,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f10.f64 = double(temp.f32);
	// lfs f9,4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 4);
	ctx.f9.f64 = double(temp.f32);
	// fadds f6,f0,f10
	ctx.f6.f64 = double(float(ctx.f0.f64 + ctx.f10.f64));
	// lfs f8,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f8.f64 = double(temp.f32);
	// fsubs f0,f0,f10
	ctx.f0.f64 = double(float(ctx.f0.f64 - ctx.f10.f64));
	// lfs f12,0(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f12.f64 = double(temp.f32);
	// fadds f5,f13,f9
	ctx.f5.f64 = double(float(ctx.f13.f64 + ctx.f9.f64));
	// lfs f11,4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f11.f64 = double(temp.f32);
	// fadds f10,f12,f8
	ctx.f10.f64 = double(float(ctx.f12.f64 + ctx.f8.f64));
	// lfs f7,4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f7.f64 = double(temp.f32);
	// fsubs f13,f13,f9
	ctx.f13.f64 = double(float(ctx.f13.f64 - ctx.f9.f64));
	// fadds f9,f11,f7
	ctx.f9.f64 = double(float(ctx.f11.f64 + ctx.f7.f64));
	// fsubs f11,f11,f7
	ctx.f11.f64 = double(float(ctx.f11.f64 - ctx.f7.f64));
	// fsubs f12,f12,f8
	ctx.f12.f64 = double(float(ctx.f12.f64 - ctx.f8.f64));
	// fadds f8,f10,f6
	ctx.f8.f64 = double(float(ctx.f10.f64 + ctx.f6.f64));
	// stfs f8,0(r4)
	temp.f32 = float(ctx.f8.f64);
	REX_STORE_U32(ctx.r4.u32 + 0, temp.u32);
	// fsubs f10,f6,f10
	ctx.f10.f64 = double(float(ctx.f6.f64 - ctx.f10.f64));
	// fadds f8,f9,f5
	ctx.f8.f64 = double(float(ctx.f9.f64 + ctx.f5.f64));
	// stfs f8,4(r4)
	temp.f32 = float(ctx.f8.f64);
	REX_STORE_U32(ctx.r4.u32 + 4, temp.u32);
	// stfs f10,0(r10)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// fsubs f10,f5,f9
	ctx.f10.f64 = double(float(ctx.f5.f64 - ctx.f9.f64));
	// stfs f10,4(r10)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// fsubs f10,f0,f11
	ctx.f10.f64 = double(float(ctx.f0.f64 - ctx.f11.f64));
	// stfs f10,0(r9)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// fadds f10,f12,f13
	ctx.f10.f64 = double(float(ctx.f12.f64 + ctx.f13.f64));
	// stfs f10,4(r9)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r9.u32 + 4, temp.u32);
	// fadds f0,f11,f0
	ctx.f0.f64 = double(float(ctx.f11.f64 + ctx.f0.f64));
	// stfs f0,0(r8)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// fsubs f0,f13,f12
	ctx.f0.f64 = double(float(ctx.f13.f64 - ctx.f12.f64));
	// stfs f0,4(r8)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r8.u32 + 4, temp.u32);
	// lfs f31,4(r5)
	temp.u32 = REX_LOAD_U32(ctx.r5.u32 + 4);
	ctx.f31.f64 = double(temp.f32);
	// ble cr6,0x82a81c40
	if (!ctx.cr6.gt) goto loc_82A81C40;
	// rlwinm r8,r11,4,0,27
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 4) & 0xFFFFFFF0;
	// addi r3,r5,8
	ctx.r3.s64 = ctx.r5.s64 + 8;
	// add r8,r8,r4
	ctx.r8.u64 = ctx.r8.u64 + ctx.r4.u64;
	// addi r6,r28,-3
	ctx.r6.s64 = ctx.r28.s64 + -3;
	// addi r5,r8,-8
	ctx.r5.s64 = ctx.r8.s64 + -8;
	// rlwinm r8,r11,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// rlwinm r6,r6,31,1,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 31) & 0x7FFFFFFF;
	// add r8,r11,r8
	ctx.r8.u64 = ctx.r11.u64 + ctx.r8.u64;
	// addi r29,r6,1
	ctx.r29.s64 = ctx.r6.s64 + 1;
	// rlwinm r10,r11,3,0,28
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 3) & 0xFFFFFFF8;
	// rlwinm r6,r8,2,0,29
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r9,r11,2
	ctx.r9.s64 = ctx.r11.s64 + 2;
	// addi r7,r11,-2
	ctx.r7.s64 = ctx.r11.s64 + -2;
	// add r10,r10,r4
	ctx.r10.u64 = ctx.r10.u64 + ctx.r4.u64;
	// add r6,r6,r4
	ctx.r6.u64 = ctx.r6.u64 + ctx.r4.u64;
	// rlwinm r9,r9,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r7,r7,2,0,29
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r31,r10,8
	ctx.r31.s64 = ctx.r10.s64 + 8;
	// addi r8,r10,-8
	ctx.r8.s64 = ctx.r10.s64 + -8;
	// addi r10,r6,8
	ctx.r10.s64 = ctx.r6.s64 + 8;
	// addi r30,r4,8
	ctx.r30.s64 = ctx.r4.s64 + 8;
	// add r9,r9,r4
	ctx.r9.u64 = ctx.r9.u64 + ctx.r4.u64;
	// add r7,r7,r4
	ctx.r7.u64 = ctx.r7.u64 + ctx.r4.u64;
	// addi r6,r6,-8
	ctx.r6.s64 = ctx.r6.s64 + -8;
loc_82A81AB8:
	// lfs f13,0(r30)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// addi r3,r3,16
	ctx.r3.s64 = ctx.r3.s64 + 16;
	// lfs f12,0(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 0);
	ctx.f12.f64 = double(temp.f32);
	// addi r29,r29,-1
	ctx.r29.s64 = ctx.r29.s64 + -1;
	// lfs f8,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f8.f64 = double(temp.f32);
	// fadds f5,f12,f13
	ctx.f5.f64 = double(float(ctx.f12.f64 + ctx.f13.f64));
	// lfs f9,0(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f9.f64 = double(temp.f32);
	// fsubs f4,f13,f12
	ctx.f4.f64 = double(float(ctx.f13.f64 - ctx.f12.f64));
	// fadds f2,f9,f8
	ctx.f2.f64 = double(float(ctx.f9.f64 + ctx.f8.f64));
	// lfs f11,4(r30)
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + 4);
	ctx.f11.f64 = double(temp.f32);
	// lfs f6,4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 4);
	ctx.f6.f64 = double(temp.f32);
	// fsubs f9,f8,f9
	ctx.f9.f64 = double(float(ctx.f8.f64 - ctx.f9.f64));
	// lfs f10,4(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 4);
	ctx.f10.f64 = double(temp.f32);
	// cmplwi cr6,r29,0
	ctx.cr6.compare<uint32_t>(ctx.r29.u32, 0, ctx.xer);
	// lfs f7,4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f7.f64 = double(temp.f32);
	// fadds f3,f10,f11
	ctx.f3.f64 = double(float(ctx.f10.f64 + ctx.f11.f64));
	// fadds f8,f7,f6
	ctx.f8.f64 = double(float(ctx.f7.f64 + ctx.f6.f64));
	// lfs f0,-4(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + -4);
	ctx.f0.f64 = double(temp.f32);
	// fsubs f7,f6,f7
	ctx.f7.f64 = double(float(ctx.f6.f64 - ctx.f7.f64));
	// lfs f1,4(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// fsubs f10,f11,f10
	ctx.f10.f64 = double(float(ctx.f11.f64 - ctx.f10.f64));
	// lfs f13,-8(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + -8);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,0(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 0);
	ctx.f12.f64 = double(temp.f32);
	// fneg f11,f1
	ctx.f11.u64 = ctx.f1.u64 ^ 0x8000000000000000;
	// fadds f6,f2,f5
	ctx.f6.f64 = double(float(ctx.f2.f64 + ctx.f5.f64));
	// stfs f6,0(r30)
	temp.f32 = float(ctx.f6.f64);
	REX_STORE_U32(ctx.r30.u32 + 0, temp.u32);
	// fsubs f6,f5,f2
	ctx.f6.f64 = double(float(ctx.f5.f64 - ctx.f2.f64));
	// fadds f5,f8,f3
	ctx.f5.f64 = double(float(ctx.f8.f64 + ctx.f3.f64));
	// stfs f5,4(r30)
	temp.f32 = float(ctx.f5.f64);
	REX_STORE_U32(ctx.r30.u32 + 4, temp.u32);
	// stfs f6,0(r9)
	temp.f32 = float(ctx.f6.f64);
	REX_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// fsubs f5,f3,f8
	ctx.f5.f64 = double(float(ctx.f3.f64 - ctx.f8.f64));
	// fadds f6,f9,f10
	ctx.f6.f64 = double(float(ctx.f9.f64 + ctx.f10.f64));
	// stfs f5,4(r9)
	temp.f32 = float(ctx.f5.f64);
	REX_STORE_U32(ctx.r9.u32 + 4, temp.u32);
	// fsubs f8,f4,f7
	ctx.f8.f64 = double(float(ctx.f4.f64 - ctx.f7.f64));
	// addi r9,r9,8
	ctx.r9.s64 = ctx.r9.s64 + 8;
	// fsubs f10,f10,f9
	ctx.f10.f64 = double(float(ctx.f10.f64 - ctx.f9.f64));
	// addi r30,r30,8
	ctx.r30.s64 = ctx.r30.s64 + 8;
	// fmuls f3,f0,f6
	ctx.f3.f64 = double(float(ctx.f0.f64 * ctx.f6.f64));
	// fmr f5,f8
	ctx.f5.f64 = ctx.f8.f64;
	// fmuls f2,f0,f8
	ctx.f2.f64 = double(float(ctx.f0.f64 * ctx.f8.f64));
	// fadds f8,f7,f4
	ctx.f8.f64 = double(float(ctx.f7.f64 + ctx.f4.f64));
	// fmsubs f9,f13,f5,f3
	ctx.f9.f64 = double(float(std::fma(ctx.f13.f64, ctx.f5.f64, -ctx.f3.f64)));
	// stfs f9,0(r31)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r31.u32 + 0, temp.u32);
	// fmadds f9,f13,f6,f2
	ctx.f9.f64 = double(float(std::fma(ctx.f13.f64, ctx.f6.f64, ctx.f2.f64)));
	// stfs f9,4(r31)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r31.u32 + 4, temp.u32);
	// fmuls f9,f12,f8
	ctx.f9.f64 = double(float(ctx.f12.f64 * ctx.f8.f64));
	// addi r31,r31,8
	ctx.r31.s64 = ctx.r31.s64 + 8;
	// fmuls f8,f11,f8
	ctx.f8.f64 = double(float(ctx.f11.f64 * ctx.f8.f64));
	// fmadds f9,f11,f10,f9
	ctx.f9.f64 = double(float(std::fma(ctx.f11.f64, ctx.f10.f64, ctx.f9.f64)));
	// stfs f9,0(r10)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// fmsubs f10,f12,f10,f8
	ctx.f10.f64 = double(float(std::fma(ctx.f12.f64, ctx.f10.f64, -ctx.f8.f64)));
	// stfs f10,4(r10)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// lfs f9,0(r6)
	temp.u32 = REX_LOAD_U32(ctx.r6.u32 + 0);
	ctx.f9.f64 = double(temp.f32);
	// addi r10,r10,8
	ctx.r10.s64 = ctx.r10.s64 + 8;
	// lfs f10,0(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 0);
	ctx.f10.f64 = double(temp.f32);
	// lfs f7,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f7.f64 = double(temp.f32);
	// fadds f2,f9,f10
	ctx.f2.f64 = double(float(ctx.f9.f64 + ctx.f10.f64));
	// lfs f8,0(r5)
	temp.u32 = REX_LOAD_U32(ctx.r5.u32 + 0);
	ctx.f8.f64 = double(temp.f32);
	// fsubs f10,f10,f9
	ctx.f10.f64 = double(float(ctx.f10.f64 - ctx.f9.f64));
	// lfs f4,4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f4.f64 = double(temp.f32);
	// fadds f9,f8,f7
	ctx.f9.f64 = double(float(ctx.f8.f64 + ctx.f7.f64));
	// lfs f3,4(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 4);
	ctx.f3.f64 = double(temp.f32);
	// fsubs f8,f7,f8
	ctx.f8.f64 = double(float(ctx.f7.f64 - ctx.f8.f64));
	// lfs f6,4(r6)
	temp.u32 = REX_LOAD_U32(ctx.r6.u32 + 4);
	ctx.f6.f64 = double(temp.f32);
	// lfs f5,4(r5)
	temp.u32 = REX_LOAD_U32(ctx.r5.u32 + 4);
	ctx.f5.f64 = double(temp.f32);
	// fadds f1,f6,f3
	ctx.f1.f64 = double(float(ctx.f6.f64 + ctx.f3.f64));
	// fadds f7,f5,f4
	ctx.f7.f64 = double(float(ctx.f5.f64 + ctx.f4.f64));
	// fsubs f5,f4,f5
	ctx.f5.f64 = double(float(ctx.f4.f64 - ctx.f5.f64));
	// fsubs f6,f3,f6
	ctx.f6.f64 = double(float(ctx.f3.f64 - ctx.f6.f64));
	// fadds f4,f9,f2
	ctx.f4.f64 = double(float(ctx.f9.f64 + ctx.f2.f64));
	// stfs f4,0(r7)
	temp.f32 = float(ctx.f4.f64);
	REX_STORE_U32(ctx.r7.u32 + 0, temp.u32);
	// fsubs f3,f2,f9
	ctx.f3.f64 = double(float(ctx.f2.f64 - ctx.f9.f64));
	// fadds f4,f7,f1
	ctx.f4.f64 = double(float(ctx.f7.f64 + ctx.f1.f64));
	// stfs f4,4(r7)
	temp.f32 = float(ctx.f4.f64);
	REX_STORE_U32(ctx.r7.u32 + 4, temp.u32);
	// fsubs f2,f1,f7
	ctx.f2.f64 = double(float(ctx.f1.f64 - ctx.f7.f64));
	// stfs f3,0(r8)
	temp.f32 = float(ctx.f3.f64);
	REX_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// stfs f2,4(r8)
	temp.f32 = float(ctx.f2.f64);
	REX_STORE_U32(ctx.r8.u32 + 4, temp.u32);
	// fsubs f9,f10,f5
	ctx.f9.f64 = double(float(ctx.f10.f64 - ctx.f5.f64));
	// fadds f7,f8,f6
	ctx.f7.f64 = double(float(ctx.f8.f64 + ctx.f6.f64));
	// addi r8,r8,-8
	ctx.r8.s64 = ctx.r8.s64 + -8;
	// addi r7,r7,-8
	ctx.r7.s64 = ctx.r7.s64 + -8;
	// fmuls f4,f13,f7
	ctx.f4.f64 = double(float(ctx.f13.f64 * ctx.f7.f64));
	// fmuls f3,f13,f9
	ctx.f3.f64 = double(float(ctx.f13.f64 * ctx.f9.f64));
	// fadds f13,f5,f10
	ctx.f13.f64 = double(float(ctx.f5.f64 + ctx.f10.f64));
	// fsubs f10,f6,f8
	ctx.f10.f64 = double(float(ctx.f6.f64 - ctx.f8.f64));
	// fmsubs f9,f0,f9,f4
	ctx.f9.f64 = double(float(std::fma(ctx.f0.f64, ctx.f9.f64, -ctx.f4.f64)));
	// stfs f9,0(r6)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r6.u32 + 0, temp.u32);
	// fmadds f0,f0,f7,f3
	ctx.f0.f64 = double(float(std::fma(ctx.f0.f64, ctx.f7.f64, ctx.f3.f64)));
	// stfs f0,4(r6)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r6.u32 + 4, temp.u32);
	// fmuls f0,f11,f13
	ctx.f0.f64 = double(float(ctx.f11.f64 * ctx.f13.f64));
	// addi r6,r6,-8
	ctx.r6.s64 = ctx.r6.s64 + -8;
	// fmuls f13,f12,f13
	ctx.f13.f64 = double(float(ctx.f12.f64 * ctx.f13.f64));
	// fmadds f0,f12,f10,f0
	ctx.f0.f64 = double(float(std::fma(ctx.f12.f64, ctx.f10.f64, ctx.f0.f64)));
	// stfs f0,0(r5)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r5.u32 + 0, temp.u32);
	// fmsubs f0,f11,f10,f13
	ctx.f0.f64 = double(float(std::fma(ctx.f11.f64, ctx.f10.f64, -ctx.f13.f64)));
	// stfs f0,4(r5)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r5.u32 + 4, temp.u32);
	// addi r5,r5,-8
	ctx.r5.s64 = ctx.r5.s64 + -8;
	// bne cr6,0x82a81ab8
	if (!ctx.cr6.eq) goto loc_82A81AB8;
loc_82A81C40:
	// add r9,r11,r28
	ctx.r9.u64 = ctx.r11.u64 + ctx.r28.u64;
	// fneg f0,f31
	ctx.fpscr.disableFlushMode();
	ctx.f0.u64 = ctx.f31.u64 ^ 0x8000000000000000;
	// rlwinm r10,r28,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r28.u32 | (ctx.r28.u64 << 32), 2) & 0xFFFFFFFC;
	// add r8,r9,r11
	ctx.r8.u64 = ctx.r9.u64 + ctx.r11.u64;
	// rlwinm r9,r9,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// add r7,r8,r11
	ctx.r7.u64 = ctx.r8.u64 + ctx.r11.u64;
	// rlwinm r11,r8,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r8,r7,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
	// add r10,r10,r4
	ctx.r10.u64 = ctx.r10.u64 + ctx.r4.u64;
	// add r11,r11,r4
	ctx.r11.u64 = ctx.r11.u64 + ctx.r4.u64;
	// add r9,r9,r4
	ctx.r9.u64 = ctx.r9.u64 + ctx.r4.u64;
	// add r8,r8,r4
	ctx.r8.u64 = ctx.r8.u64 + ctx.r4.u64;
	// lfs f13,0(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// lfs f9,0(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f9.f64 = double(temp.f32);
	// lfs f8,4(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f8.f64 = double(temp.f32);
	// fadds f5,f13,f9
	ctx.f5.f64 = double(float(ctx.f13.f64 + ctx.f9.f64));
	// lfs f7,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f7.f64 = double(temp.f32);
	// fsubs f13,f13,f9
	ctx.f13.f64 = double(float(ctx.f13.f64 - ctx.f9.f64));
	// lfs f12,4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// lfs f11,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// fadds f4,f12,f8
	ctx.f4.f64 = double(float(ctx.f12.f64 + ctx.f8.f64));
	// lfs f10,4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 4);
	ctx.f10.f64 = double(temp.f32);
	// fadds f9,f11,f7
	ctx.f9.f64 = double(float(ctx.f11.f64 + ctx.f7.f64));
	// lfs f6,4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f6.f64 = double(temp.f32);
	// fsubs f12,f12,f8
	ctx.f12.f64 = double(float(ctx.f12.f64 - ctx.f8.f64));
	// fadds f8,f10,f6
	ctx.f8.f64 = double(float(ctx.f10.f64 + ctx.f6.f64));
	// fsubs f11,f11,f7
	ctx.f11.f64 = double(float(ctx.f11.f64 - ctx.f7.f64));
	// fsubs f10,f10,f6
	ctx.f10.f64 = double(float(ctx.f10.f64 - ctx.f6.f64));
	// fadds f7,f9,f5
	ctx.f7.f64 = double(float(ctx.f9.f64 + ctx.f5.f64));
	// stfs f7,0(r10)
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// fsubs f9,f5,f9
	ctx.f9.f64 = double(float(ctx.f5.f64 - ctx.f9.f64));
	// fadds f7,f8,f4
	ctx.f7.f64 = double(float(ctx.f8.f64 + ctx.f4.f64));
	// stfs f7,4(r10)
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// stfs f9,0(r9)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// fsubs f9,f4,f8
	ctx.f9.f64 = double(float(ctx.f4.f64 - ctx.f8.f64));
	// stfs f9,4(r9)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r9.u32 + 4, temp.u32);
	// fadds f8,f11,f12
	ctx.f8.f64 = double(float(ctx.f11.f64 + ctx.f12.f64));
	// fsubs f9,f13,f10
	ctx.f9.f64 = double(float(ctx.f13.f64 - ctx.f10.f64));
	// fsubs f12,f12,f11
	ctx.f12.f64 = double(float(ctx.f12.f64 - ctx.f11.f64));
	// fadds f13,f10,f13
	ctx.f13.f64 = double(float(ctx.f10.f64 + ctx.f13.f64));
	// fsubs f7,f9,f8
	ctx.f7.f64 = double(float(ctx.f9.f64 - ctx.f8.f64));
	// fadds f9,f8,f9
	ctx.f9.f64 = double(float(ctx.f8.f64 + ctx.f9.f64));
	// fmuls f11,f7,f31
	ctx.f11.f64 = double(float(ctx.f7.f64 * ctx.f31.f64));
	// stfs f11,0(r11)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// fmuls f11,f9,f31
	ctx.f11.f64 = double(float(ctx.f9.f64 * ctx.f31.f64));
	// stfs f11,4(r11)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// fadds f11,f12,f13
	ctx.f11.f64 = double(float(ctx.f12.f64 + ctx.f13.f64));
	// fsubs f13,f12,f13
	ctx.f13.f64 = double(float(ctx.f12.f64 - ctx.f13.f64));
	// fmuls f12,f11,f0
	ctx.f12.f64 = double(float(ctx.f11.f64 * ctx.f0.f64));
	// stfs f12,0(r8)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// fmuls f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 * ctx.f0.f64));
	// stfs f0,4(r8)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r8.u32 + 4, temp.u32);
	// lfd f31,-48(r1)
	ctx.f31.u64 = REX_LOAD_U64(ctx.r1.u32 + -48);
	// b 0x829ff818
	__restgprlr_28(ctx, base);
	return;
}

DEFINE_REX_FUNC(sub_82A81D18) {
	REX_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7bc
	ctx.lr = 0x82A81D20;
	__savegprlr_25(ctx, base);
	// addi r12,r1,-64
	ctx.r12.s64 = ctx.r1.s64 + -64;
	// bl 0x82a01320
	ctx.lr = 0x82A81D28;
	__savefpr_26(ctx, base);
	// srawi r26,r3,3
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0x7) != 0);
	ctx.r26.s64 = ctx.r3.s32 >> 3;
	// lfs f13,0(r4)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r4.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,4(r4)
	temp.u32 = REX_LOAD_U32(ctx.r4.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// rlwinm r11,r26,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r26.u32 | (ctx.r26.u64 << 32), 1) & 0xFFFFFFFE;
	// lfs f0,4(r5)
	temp.u32 = REX_LOAD_U32(ctx.r5.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// cmpwi cr6,r26,2
	ctx.cr6.compare<int32_t>(ctx.r26.s32, 2, ctx.xer);
	// rlwinm r10,r11,1,0,30
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// rlwinm r25,r11,2,0,29
	ctx.r25.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r9,r10,r11
	ctx.r9.u64 = ctx.r10.u64 + ctx.r11.u64;
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r9,r9,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// add r10,r10,r4
	ctx.r10.u64 = ctx.r10.u64 + ctx.r4.u64;
	// add r9,r9,r4
	ctx.r9.u64 = ctx.r9.u64 + ctx.r4.u64;
	// lfsx f11,r25,r4
	temp.u32 = REX_LOAD_U32(ctx.r25.u32 + ctx.r4.u32);
	ctx.f11.f64 = double(temp.f32);
	// add r8,r25,r4
	ctx.r8.u64 = ctx.r25.u64 + ctx.r4.u64;
	// rlwinm r7,r11,1,0,30
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// lfs f9,4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f9.f64 = double(temp.f32);
	// lfs f8,0(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f8.f64 = double(temp.f32);
	// fsubs f5,f13,f9
	ctx.f5.f64 = double(float(ctx.f13.f64 - ctx.f9.f64));
	// lfs f7,4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 4);
	ctx.f7.f64 = double(temp.f32);
	// fadds f4,f12,f8
	ctx.f4.f64 = double(float(ctx.f12.f64 + ctx.f8.f64));
	// lfs f6,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f6.f64 = double(temp.f32);
	// fadds f13,f9,f13
	ctx.f13.f64 = double(float(ctx.f9.f64 + ctx.f13.f64));
	// lfs f10,4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f10.f64 = double(temp.f32);
	// fsubs f12,f12,f8
	ctx.f12.f64 = double(float(ctx.f12.f64 - ctx.f8.f64));
	// fadds f8,f6,f10
	ctx.f8.f64 = double(float(ctx.f6.f64 + ctx.f10.f64));
	// fsubs f9,f11,f7
	ctx.f9.f64 = double(float(ctx.f11.f64 - ctx.f7.f64));
	// fadds f11,f7,f11
	ctx.f11.f64 = double(float(ctx.f7.f64 + ctx.f11.f64));
	// fsubs f10,f10,f6
	ctx.f10.f64 = double(float(ctx.f10.f64 - ctx.f6.f64));
	// fsubs f7,f9,f8
	ctx.f7.f64 = double(float(ctx.f9.f64 - ctx.f8.f64));
	// fadds f9,f8,f9
	ctx.f9.f64 = double(float(ctx.f8.f64 + ctx.f9.f64));
	// fadds f6,f10,f11
	ctx.f6.f64 = double(float(ctx.f10.f64 + ctx.f11.f64));
	// fsubs f8,f11,f10
	ctx.f8.f64 = double(float(ctx.f11.f64 - ctx.f10.f64));
	// fmuls f11,f7,f0
	ctx.f11.f64 = double(float(ctx.f7.f64 * ctx.f0.f64));
	// fmuls f10,f9,f0
	ctx.f10.f64 = double(float(ctx.f9.f64 * ctx.f0.f64));
	// fadds f9,f11,f5
	ctx.f9.f64 = double(float(ctx.f11.f64 + ctx.f5.f64));
	// stfs f9,0(r4)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r4.u32 + 0, temp.u32);
	// fadds f9,f10,f4
	ctx.f9.f64 = double(float(ctx.f10.f64 + ctx.f4.f64));
	// stfs f9,4(r4)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r4.u32 + 4, temp.u32);
	// fsubs f11,f5,f11
	ctx.f11.f64 = double(float(ctx.f5.f64 - ctx.f11.f64));
	// stfsx f11,r25,r4
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r25.u32 + ctx.r4.u32, temp.u32);
	// fsubs f11,f4,f10
	ctx.f11.f64 = double(float(ctx.f4.f64 - ctx.f10.f64));
	// stfs f11,4(r8)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r8.u32 + 4, temp.u32);
	// fmuls f11,f8,f0
	ctx.f11.f64 = double(float(ctx.f8.f64 * ctx.f0.f64));
	// fmuls f0,f6,f0
	ctx.f0.f64 = double(float(ctx.f6.f64 * ctx.f0.f64));
	// fadds f10,f11,f12
	ctx.f10.f64 = double(float(ctx.f11.f64 + ctx.f12.f64));
	// stfs f10,4(r10)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// fsubs f10,f13,f0
	ctx.f10.f64 = double(float(ctx.f13.f64 - ctx.f0.f64));
	// stfs f10,0(r10)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// fadds f0,f0,f13
	ctx.f0.f64 = double(float(ctx.f0.f64 + ctx.f13.f64));
	// stfs f0,0(r9)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// fsubs f0,f12,f11
	ctx.f0.f64 = double(float(ctx.f12.f64 - ctx.f11.f64));
	// stfs f0,4(r9)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r9.u32 + 4, temp.u32);
	// ble cr6,0x82a82048
	if (!ctx.cr6.gt) goto loc_82A82048;
	// addi r6,r7,2
	ctx.r6.s64 = ctx.r7.s64 + 2;
	// rlwinm r7,r11,4,0,27
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 4) & 0xFFFFFFF0;
	// addi r31,r26,-3
	ctx.r31.s64 = ctx.r26.s64 + -3;
	// add r7,r7,r4
	ctx.r7.u64 = ctx.r7.u64 + ctx.r4.u64;
	// rlwinm r31,r31,31,1,31
	ctx.r31.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 31) & 0x7FFFFFFF;
	// addi r29,r7,-8
	ctx.r29.s64 = ctx.r7.s64 + -8;
	// rlwinm r7,r11,1,0,30
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// addi r27,r31,1
	ctx.r27.s64 = ctx.r31.s64 + 1;
	// add r31,r11,r7
	ctx.r31.u64 = ctx.r11.u64 + ctx.r7.u64;
	// addi r9,r11,2
	ctx.r9.s64 = ctx.r11.s64 + 2;
	// rlwinm r10,r11,3,0,28
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 3) & 0xFFFFFFF8;
	// rlwinm r28,r31,2,0,29
	ctx.r28.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r3,r11,-2
	ctx.r3.s64 = ctx.r11.s64 + -2;
	// rlwinm r8,r9,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// add r10,r10,r4
	ctx.r10.u64 = ctx.r10.u64 + ctx.r4.u64;
	// rlwinm r9,r6,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
	// add r28,r28,r4
	ctx.r28.u64 = ctx.r28.u64 + ctx.r4.u64;
	// rlwinm r30,r3,2,0,29
	ctx.r30.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// add r6,r8,r4
	ctx.r6.u64 = ctx.r8.u64 + ctx.r4.u64;
	// addi r7,r10,8
	ctx.r7.s64 = ctx.r10.s64 + 8;
	// addi r31,r10,-8
	ctx.r31.s64 = ctx.r10.s64 + -8;
	// add r8,r9,r5
	ctx.r8.u64 = ctx.r9.u64 + ctx.r5.u64;
	// addi r10,r28,8
	ctx.r10.s64 = ctx.r28.s64 + 8;
	// addi r3,r4,8
	ctx.r3.s64 = ctx.r4.s64 + 8;
	// addi r9,r5,8
	ctx.r9.s64 = ctx.r5.s64 + 8;
	// add r30,r30,r4
	ctx.r30.u64 = ctx.r30.u64 + ctx.r4.u64;
	// addi r28,r28,-8
	ctx.r28.s64 = ctx.r28.s64 + -8;
loc_82A81E6C:
	// lfs f9,4(r3)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 4);
	ctx.f9.f64 = double(temp.f32);
	// addi r9,r9,16
	ctx.r9.s64 = ctx.r9.s64 + 16;
	// lfs f8,0(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 0);
	ctx.f8.f64 = double(temp.f32);
	// addi r8,r8,-16
	ctx.r8.s64 = ctx.r8.s64 + -16;
	// lfs f11,0(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// fadds f31,f8,f9
	ctx.f31.f64 = double(float(ctx.f8.f64 + ctx.f9.f64));
	// lfs f10,4(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 4);
	ctx.f10.f64 = double(temp.f32);
	// fsubs f30,f9,f8
	ctx.f30.f64 = double(float(ctx.f9.f64 - ctx.f8.f64));
	// lfs f4,0(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f4.f64 = double(temp.f32);
	// fsubs f2,f11,f10
	ctx.f2.f64 = double(float(ctx.f11.f64 - ctx.f10.f64));
	// lfs f3,4(r6)
	temp.u32 = REX_LOAD_U32(ctx.r6.u32 + 4);
	ctx.f3.f64 = double(temp.f32);
	// fadds f1,f10,f11
	ctx.f1.f64 = double(float(ctx.f10.f64 + ctx.f11.f64));
	// lfs f5,4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f5.f64 = double(temp.f32);
	// fadds f28,f4,f3
	ctx.f28.f64 = double(float(ctx.f4.f64 + ctx.f3.f64));
	// lfs f6,0(r6)
	temp.u32 = REX_LOAD_U32(ctx.r6.u32 + 0);
	ctx.f6.f64 = double(temp.f32);
	// addi r27,r27,-1
	ctx.r27.s64 = ctx.r27.s64 + -1;
	// fsubs f29,f6,f5
	ctx.f29.f64 = double(float(ctx.f6.f64 - ctx.f5.f64));
	// lfs f7,4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 4);
	ctx.f7.f64 = double(temp.f32);
	// lfs f13,-4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + -4);
	ctx.f13.f64 = double(temp.f32);
	// fadds f6,f5,f6
	ctx.f6.f64 = double(float(ctx.f5.f64 + ctx.f6.f64));
	// lfs f27,4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f27.f64 = double(temp.f32);
	// fsubs f5,f3,f4
	ctx.f5.f64 = double(float(ctx.f3.f64 - ctx.f4.f64));
	// lfs f12,-8(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + -8);
	ctx.f12.f64 = double(temp.f32);
	// fneg f9,f7
	ctx.f9.u64 = ctx.f7.u64 ^ 0x8000000000000000;
	// fneg f7,f27
	ctx.f7.u64 = ctx.f27.u64 ^ 0x8000000000000000;
	// lfs f0,-8(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + -8);
	ctx.f0.f64 = double(temp.f32);
	// fmuls f4,f13,f31
	ctx.f4.f64 = double(float(ctx.f13.f64 * ctx.f31.f64));
	// lfs f11,-4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + -4);
	ctx.f11.f64 = double(temp.f32);
	// fmuls f3,f13,f2
	ctx.f3.f64 = double(float(ctx.f13.f64 * ctx.f2.f64));
	// lfs f10,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f10.f64 = double(temp.f32);
	// lfs f8,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f8.f64 = double(temp.f32);
	// fmuls f27,f12,f28
	ctx.f27.f64 = double(float(ctx.f12.f64 * ctx.f28.f64));
	// fmuls f26,f12,f29
	ctx.f26.f64 = double(float(ctx.f12.f64 * ctx.f29.f64));
	// fmsubs f4,f0,f2,f4
	ctx.f4.f64 = double(float(std::fma(ctx.f0.f64, ctx.f2.f64, -ctx.f4.f64)));
	// fmadds f3,f0,f31,f3
	ctx.f3.f64 = double(float(std::fma(ctx.f0.f64, ctx.f31.f64, ctx.f3.f64)));
	// fmuls f31,f10,f1
	ctx.f31.f64 = double(float(ctx.f10.f64 * ctx.f1.f64));
	// fmsubs f2,f11,f29,f27
	ctx.f2.f64 = double(float(std::fma(ctx.f11.f64, ctx.f29.f64, -ctx.f27.f64)));
	// fmuls f29,f9,f1
	ctx.f29.f64 = double(float(ctx.f9.f64 * ctx.f1.f64));
	// fmadds f1,f11,f28,f26
	ctx.f1.f64 = double(float(std::fma(ctx.f11.f64, ctx.f28.f64, ctx.f26.f64)));
	// fmuls f28,f7,f6
	ctx.f28.f64 = double(float(ctx.f7.f64 * ctx.f6.f64));
	// fmuls f27,f8,f6
	ctx.f27.f64 = double(float(ctx.f8.f64 * ctx.f6.f64));
	// fadds f6,f2,f4
	ctx.f6.f64 = double(float(ctx.f2.f64 + ctx.f4.f64));
	// stfs f6,0(r3)
	temp.f32 = float(ctx.f6.f64);
	REX_STORE_U32(ctx.r3.u32 + 0, temp.u32);
	// fsubs f4,f4,f2
	ctx.f4.f64 = double(float(ctx.f4.f64 - ctx.f2.f64));
	// fadds f2,f1,f3
	ctx.f2.f64 = double(float(ctx.f1.f64 + ctx.f3.f64));
	// stfs f2,4(r3)
	temp.f32 = float(ctx.f2.f64);
	REX_STORE_U32(ctx.r3.u32 + 4, temp.u32);
	// fsubs f3,f3,f1
	ctx.f3.f64 = double(float(ctx.f3.f64 - ctx.f1.f64));
	// stfs f3,4(r6)
	temp.f32 = float(ctx.f3.f64);
	REX_STORE_U32(ctx.r6.u32 + 4, temp.u32);
	// fmadds f3,f8,f5,f28
	ctx.f3.f64 = double(float(std::fma(ctx.f8.f64, ctx.f5.f64, ctx.f28.f64)));
	// stfs f4,0(r6)
	temp.f32 = float(ctx.f4.f64);
	REX_STORE_U32(ctx.r6.u32 + 0, temp.u32);
	// fmadds f6,f9,f30,f31
	ctx.f6.f64 = double(float(std::fma(ctx.f9.f64, ctx.f30.f64, ctx.f31.f64)));
	// addi r6,r6,8
	ctx.r6.s64 = ctx.r6.s64 + 8;
	// fmsubs f4,f10,f30,f29
	ctx.f4.f64 = double(float(std::fma(ctx.f10.f64, ctx.f30.f64, -ctx.f29.f64)));
	// addi r3,r3,8
	ctx.r3.s64 = ctx.r3.s64 + 8;
	// fmsubs f5,f7,f5,f27
	ctx.f5.f64 = double(float(std::fma(ctx.f7.f64, ctx.f5.f64, -ctx.f27.f64)));
	// fadds f2,f3,f6
	ctx.f2.f64 = double(float(ctx.f3.f64 + ctx.f6.f64));
	// stfs f2,0(r7)
	temp.f32 = float(ctx.f2.f64);
	REX_STORE_U32(ctx.r7.u32 + 0, temp.u32);
	// fsubs f6,f6,f3
	ctx.f6.f64 = double(float(ctx.f6.f64 - ctx.f3.f64));
	// fadds f2,f5,f4
	ctx.f2.f64 = double(float(ctx.f5.f64 + ctx.f4.f64));
	// stfs f2,4(r7)
	temp.f32 = float(ctx.f2.f64);
	REX_STORE_U32(ctx.r7.u32 + 4, temp.u32);
	// stfs f6,0(r10)
	temp.f32 = float(ctx.f6.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// fsubs f6,f4,f5
	ctx.f6.f64 = double(float(ctx.f4.f64 - ctx.f5.f64));
	// stfs f6,4(r10)
	temp.f32 = float(ctx.f6.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// addi r7,r7,8
	ctx.r7.s64 = ctx.r7.s64 + 8;
	// lfs f5,4(r28)
	temp.u32 = REX_LOAD_U32(ctx.r28.u32 + 4);
	ctx.f5.f64 = double(temp.f32);
	// addi r10,r10,8
	ctx.r10.s64 = ctx.r10.s64 + 8;
	// lfs f6,0(r30)
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + 0);
	ctx.f6.f64 = double(temp.f32);
	// lfs f3,0(r28)
	temp.u32 = REX_LOAD_U32(ctx.r28.u32 + 0);
	ctx.f3.f64 = double(temp.f32);
	// fsubs f29,f6,f5
	ctx.f29.f64 = double(float(ctx.f6.f64 - ctx.f5.f64));
	// lfs f4,4(r30)
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + 4);
	ctx.f4.f64 = double(temp.f32);
	// fadds f6,f5,f6
	ctx.f6.f64 = double(float(ctx.f5.f64 + ctx.f6.f64));
	// lfs f1,4(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 4);
	ctx.f1.f64 = double(temp.f32);
	// fadds f5,f3,f4
	ctx.f5.f64 = double(float(ctx.f3.f64 + ctx.f4.f64));
	// lfs f2,0(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 0);
	ctx.f2.f64 = double(temp.f32);
	// fsubs f4,f4,f3
	ctx.f4.f64 = double(float(ctx.f4.f64 - ctx.f3.f64));
	// lfs f31,0(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 0);
	ctx.f31.f64 = double(temp.f32);
	// fsubs f3,f2,f1
	ctx.f3.f64 = double(float(ctx.f2.f64 - ctx.f1.f64));
	// lfs f30,4(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 4);
	ctx.f30.f64 = double(temp.f32);
	// fadds f2,f1,f2
	ctx.f2.f64 = double(float(ctx.f1.f64 + ctx.f2.f64));
	// fadds f1,f31,f30
	ctx.f1.f64 = double(float(ctx.f31.f64 + ctx.f30.f64));
	// fsubs f31,f30,f31
	ctx.f31.f64 = double(float(ctx.f30.f64 - ctx.f31.f64));
	// fmuls f30,f11,f29
	ctx.f30.f64 = double(float(ctx.f11.f64 * ctx.f29.f64));
	// cmplwi cr6,r27,0
	ctx.cr6.compare<uint32_t>(ctx.r27.u32, 0, ctx.xer);
	// fmuls f11,f11,f5
	ctx.f11.f64 = double(float(ctx.f11.f64 * ctx.f5.f64));
	// fmuls f27,f0,f1
	ctx.f27.f64 = double(float(ctx.f0.f64 * ctx.f1.f64));
	// fmuls f28,f0,f3
	ctx.f28.f64 = double(float(ctx.f0.f64 * ctx.f3.f64));
	// fmuls f26,f8,f6
	ctx.f26.f64 = double(float(ctx.f8.f64 * ctx.f6.f64));
	// fmuls f6,f7,f6
	ctx.f6.f64 = double(float(ctx.f7.f64 * ctx.f6.f64));
	// fmsubs f0,f12,f29,f11
	ctx.f0.f64 = double(float(std::fma(ctx.f12.f64, ctx.f29.f64, -ctx.f11.f64)));
	// fmsubs f11,f13,f3,f27
	ctx.f11.f64 = double(float(std::fma(ctx.f13.f64, ctx.f3.f64, -ctx.f27.f64)));
	// fmadds f12,f12,f5,f30
	ctx.f12.f64 = double(float(std::fma(ctx.f12.f64, ctx.f5.f64, ctx.f30.f64)));
	// fmadds f13,f13,f1,f28
	ctx.f13.f64 = double(float(std::fma(ctx.f13.f64, ctx.f1.f64, ctx.f28.f64)));
	// fmuls f5,f9,f2
	ctx.f5.f64 = double(float(ctx.f9.f64 * ctx.f2.f64));
	// fmuls f3,f10,f2
	ctx.f3.f64 = double(float(ctx.f10.f64 * ctx.f2.f64));
	// fadds f2,f11,f0
	ctx.f2.f64 = double(float(ctx.f11.f64 + ctx.f0.f64));
	// stfs f2,0(r30)
	temp.f32 = float(ctx.f2.f64);
	REX_STORE_U32(ctx.r30.u32 + 0, temp.u32);
	// fsubs f0,f0,f11
	ctx.f0.f64 = double(float(ctx.f0.f64 - ctx.f11.f64));
	// fadds f2,f13,f12
	ctx.f2.f64 = double(float(ctx.f13.f64 + ctx.f12.f64));
	// stfs f2,4(r30)
	temp.f32 = float(ctx.f2.f64);
	REX_STORE_U32(ctx.r30.u32 + 4, temp.u32);
	// stfs f0,0(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 0, temp.u32);
	// fsubs f0,f12,f13
	ctx.f0.f64 = double(float(ctx.f12.f64 - ctx.f13.f64));
	// stfs f0,4(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 4, temp.u32);
	// fmadds f12,f10,f31,f5
	ctx.f12.f64 = double(float(std::fma(ctx.f10.f64, ctx.f31.f64, ctx.f5.f64)));
	// fmadds f0,f7,f4,f26
	ctx.f0.f64 = double(float(std::fma(ctx.f7.f64, ctx.f4.f64, ctx.f26.f64)));
	// addi r31,r31,-8
	ctx.r31.s64 = ctx.r31.s64 + -8;
	// fmsubs f13,f8,f4,f6
	ctx.f13.f64 = double(float(std::fma(ctx.f8.f64, ctx.f4.f64, -ctx.f6.f64)));
	// addi r30,r30,-8
	ctx.r30.s64 = ctx.r30.s64 + -8;
	// fmsubs f11,f9,f31,f3
	ctx.f11.f64 = double(float(std::fma(ctx.f9.f64, ctx.f31.f64, -ctx.f3.f64)));
	// fadds f10,f12,f0
	ctx.f10.f64 = double(float(ctx.f12.f64 + ctx.f0.f64));
	// stfs f10,0(r28)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r28.u32 + 0, temp.u32);
	// fsubs f0,f0,f12
	ctx.f0.f64 = double(float(ctx.f0.f64 - ctx.f12.f64));
	// fadds f10,f11,f13
	ctx.f10.f64 = double(float(ctx.f11.f64 + ctx.f13.f64));
	// stfs f10,4(r28)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r28.u32 + 4, temp.u32);
	// stfs f0,0(r29)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r29.u32 + 0, temp.u32);
	// fsubs f0,f13,f11
	ctx.f0.f64 = double(float(ctx.f13.f64 - ctx.f11.f64));
	// stfs f0,4(r29)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r29.u32 + 4, temp.u32);
	// addi r28,r28,-8
	ctx.r28.s64 = ctx.r28.s64 + -8;
	// addi r29,r29,-8
	ctx.r29.s64 = ctx.r29.s64 + -8;
	// bne cr6,0x82a81e6c
	if (!ctx.cr6.eq) goto loc_82A81E6C;
loc_82A82048:
	// add r7,r25,r5
	ctx.r7.u64 = ctx.r25.u64 + ctx.r5.u64;
	// lfsx f0,r25,r5
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r25.u32 + ctx.r5.u32);
	ctx.f0.f64 = double(temp.f32);
	// add r9,r11,r26
	ctx.r9.u64 = ctx.r11.u64 + ctx.r26.u64;
	// rlwinm r10,r26,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r26.u32 | (ctx.r26.u64 << 32), 2) & 0xFFFFFFFC;
	// add r8,r9,r11
	ctx.r8.u64 = ctx.r9.u64 + ctx.r11.u64;
	// add r10,r10,r4
	ctx.r10.u64 = ctx.r10.u64 + ctx.r4.u64;
	// lfs f13,4(r7)
	temp.u32 = REX_LOAD_U32(ctx.r7.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// add r7,r8,r11
	ctx.r7.u64 = ctx.r8.u64 + ctx.r11.u64;
	// rlwinm r11,r8,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r9,r9,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// add r11,r11,r4
	ctx.r11.u64 = ctx.r11.u64 + ctx.r4.u64;
	// lfs f11,4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f11.f64 = double(temp.f32);
	// rlwinm r8,r7,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f12,0(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f12.f64 = double(temp.f32);
	// add r9,r9,r4
	ctx.r9.u64 = ctx.r9.u64 + ctx.r4.u64;
	// add r8,r8,r4
	ctx.r8.u64 = ctx.r8.u64 + ctx.r4.u64;
	// lfs f7,0(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f7.f64 = double(temp.f32);
	// lfs f8,4(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f8.f64 = double(temp.f32);
	// fadds f3,f7,f11
	ctx.f3.f64 = double(float(ctx.f7.f64 + ctx.f11.f64));
	// fsubs f4,f12,f8
	ctx.f4.f64 = double(float(ctx.f12.f64 - ctx.f8.f64));
	// lfs f9,4(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 4);
	ctx.f9.f64 = double(temp.f32);
	// lfs f6,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f6.f64 = double(temp.f32);
	// fadds f12,f8,f12
	ctx.f12.f64 = double(float(ctx.f8.f64 + ctx.f12.f64));
	// lfs f5,4(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 4);
	ctx.f5.f64 = double(temp.f32);
	// fadds f8,f9,f6
	ctx.f8.f64 = double(float(ctx.f9.f64 + ctx.f6.f64));
	// lfs f10,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f10.f64 = double(temp.f32);
	// fsubs f11,f11,f7
	ctx.f11.f64 = double(float(ctx.f11.f64 - ctx.f7.f64));
	// fsubs f7,f10,f5
	ctx.f7.f64 = double(float(ctx.f10.f64 - ctx.f5.f64));
	// fsubs f9,f9,f6
	ctx.f9.f64 = double(float(ctx.f9.f64 - ctx.f6.f64));
	// fadds f10,f5,f10
	ctx.f10.f64 = double(float(ctx.f5.f64 + ctx.f10.f64));
	// fmuls f2,f13,f3
	ctx.f2.f64 = double(float(ctx.f13.f64 * ctx.f3.f64));
	// fmuls f1,f13,f4
	ctx.f1.f64 = double(float(ctx.f13.f64 * ctx.f4.f64));
	// fmuls f30,f0,f12
	ctx.f30.f64 = double(float(ctx.f0.f64 * ctx.f12.f64));
	// fmuls f31,f0,f11
	ctx.f31.f64 = double(float(ctx.f0.f64 * ctx.f11.f64));
	// fmsubs f6,f0,f4,f2
	ctx.f6.f64 = double(float(std::fma(ctx.f0.f64, ctx.f4.f64, -ctx.f2.f64)));
	// fmuls f4,f0,f8
	ctx.f4.f64 = double(float(ctx.f0.f64 * ctx.f8.f64));
	// fmadds f5,f0,f3,f1
	ctx.f5.f64 = double(float(std::fma(ctx.f0.f64, ctx.f3.f64, ctx.f1.f64)));
	// fmuls f3,f0,f7
	ctx.f3.f64 = double(float(ctx.f0.f64 * ctx.f7.f64));
	// fmuls f2,f13,f9
	ctx.f2.f64 = double(float(ctx.f13.f64 * ctx.f9.f64));
	// fmuls f1,f13,f10
	ctx.f1.f64 = double(float(ctx.f13.f64 * ctx.f10.f64));
	// fmsubs f12,f13,f12,f31
	ctx.f12.f64 = double(float(std::fma(ctx.f13.f64, ctx.f12.f64, -ctx.f31.f64)));
	// fmsubs f7,f13,f7,f4
	ctx.f7.f64 = double(float(std::fma(ctx.f13.f64, ctx.f7.f64, -ctx.f4.f64)));
	// fmadds f8,f13,f8,f3
	ctx.f8.f64 = double(float(std::fma(ctx.f13.f64, ctx.f8.f64, ctx.f3.f64)));
	// fmadds f13,f13,f11,f30
	ctx.f13.f64 = double(float(std::fma(ctx.f13.f64, ctx.f11.f64, ctx.f30.f64)));
	// fmsubs f11,f0,f10,f2
	ctx.f11.f64 = double(float(std::fma(ctx.f0.f64, ctx.f10.f64, -ctx.f2.f64)));
	// fmadds f0,f0,f9,f1
	ctx.f0.f64 = double(float(std::fma(ctx.f0.f64, ctx.f9.f64, ctx.f1.f64)));
	// fadds f4,f7,f6
	ctx.f4.f64 = double(float(ctx.f7.f64 + ctx.f6.f64));
	// stfs f4,0(r10)
	temp.f32 = float(ctx.f4.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// fsubs f7,f6,f7
	ctx.f7.f64 = double(float(ctx.f6.f64 - ctx.f7.f64));
	// fadds f6,f8,f5
	ctx.f6.f64 = double(float(ctx.f8.f64 + ctx.f5.f64));
	// stfs f6,4(r10)
	temp.f32 = float(ctx.f6.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// fsubs f8,f5,f8
	ctx.f8.f64 = double(float(ctx.f5.f64 - ctx.f8.f64));
	// stfs f7,0(r9)
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r9.u32 + 0, temp.u32);
	// stfs f8,4(r9)
	temp.f32 = float(ctx.f8.f64);
	REX_STORE_U32(ctx.r9.u32 + 4, temp.u32);
	// fsubs f10,f12,f11
	ctx.f10.f64 = double(float(ctx.f12.f64 - ctx.f11.f64));
	// stfs f10,0(r11)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// fsubs f10,f13,f0
	ctx.f10.f64 = double(float(ctx.f13.f64 - ctx.f0.f64));
	// stfs f10,4(r11)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// fadds f12,f11,f12
	ctx.f12.f64 = double(float(ctx.f11.f64 + ctx.f12.f64));
	// fadds f0,f0,f13
	ctx.f0.f64 = double(float(ctx.f0.f64 + ctx.f13.f64));
	// stfs f12,0(r8)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r8.u32 + 0, temp.u32);
	// stfs f0,4(r8)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r8.u32 + 4, temp.u32);
	// addi r12,r1,-64
	ctx.r12.s64 = ctx.r1.s64 + -64;
	// bl 0x82a0136c
	ctx.lr = 0x82A82148;
	__restfpr_26(ctx, base);
	// b 0x829ff80c
	__restgprlr_25(ctx, base);
	return;
}

DEFINE_REX_FUNC(sub_82A82150) {
	REX_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	REX_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// addi r12,r1,-8
	ctx.r12.s64 = ctx.r1.s64 + -8;
	// bl 0x82a012f0
	ctx.lr = 0x82A82160;
	__savefpr_14(ctx, base);
	// lfs f10,4(r3)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 4);
	ctx.f10.f64 = double(temp.f32);
	// lfs f11,68(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 68);
	ctx.f11.f64 = double(temp.f32);
	// lfs f13,64(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 64);
	ctx.f13.f64 = double(temp.f32);
	// fadds f26,f10,f11
	ctx.f26.f64 = double(float(ctx.f10.f64 + ctx.f11.f64));
	// lfs f8,32(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 32);
	ctx.f8.f64 = double(temp.f32);
	// fsubs f11,f10,f11
	ctx.f11.f64 = double(float(ctx.f10.f64 - ctx.f11.f64));
	// lfs f12,0(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 0);
	ctx.f12.f64 = double(temp.f32);
	// lfs f9,96(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 96);
	ctx.f9.f64 = double(temp.f32);
	// fadds f27,f12,f13
	ctx.f27.f64 = double(float(ctx.f12.f64 + ctx.f13.f64));
	// lfs f6,36(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 36);
	ctx.f6.f64 = double(temp.f32);
	// fadds f10,f8,f9
	ctx.f10.f64 = double(float(ctx.f8.f64 + ctx.f9.f64));
	// lfs f7,100(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 100);
	ctx.f7.f64 = double(temp.f32);
	// fsubs f9,f8,f9
	ctx.f9.f64 = double(float(ctx.f8.f64 - ctx.f9.f64));
	// fadds f8,f6,f7
	ctx.f8.f64 = double(float(ctx.f6.f64 + ctx.f7.f64));
	// lfs f4,8(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 8);
	ctx.f4.f64 = double(temp.f32);
	// fsubs f7,f6,f7
	ctx.f7.f64 = double(float(ctx.f6.f64 - ctx.f7.f64));
	// lfs f5,72(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 72);
	ctx.f5.f64 = double(temp.f32);
	// lfs f3,76(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 76);
	ctx.f3.f64 = double(temp.f32);
	// fsubs f12,f12,f13
	ctx.f12.f64 = double(float(ctx.f12.f64 - ctx.f13.f64));
	// lfs f2,12(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 12);
	ctx.f2.f64 = double(temp.f32);
	// lfs f28,8(r4)
	temp.u32 = REX_LOAD_U32(ctx.r4.u32 + 8);
	ctx.f28.f64 = double(temp.f32);
	// fadds f25,f2,f3
	ctx.f25.f64 = double(float(ctx.f2.f64 + ctx.f3.f64));
	// lfs f0,4(r4)
	temp.u32 = REX_LOAD_U32(ctx.r4.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// lfs f30,108(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 108);
	ctx.f30.f64 = double(temp.f32);
	// fmuls f13,f28,f0
	ctx.f13.f64 = double(float(ctx.f28.f64 * ctx.f0.f64));
	// lfs f29,44(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 44);
	ctx.f29.f64 = double(temp.f32);
	// fadds f6,f10,f27
	ctx.f6.f64 = double(float(ctx.f10.f64 + ctx.f27.f64));
	// lfs f1,104(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 104);
	ctx.f1.f64 = double(temp.f32);
	// fsubs f10,f27,f10
	ctx.f10.f64 = double(float(ctx.f27.f64 - ctx.f10.f64));
	// lfs f31,40(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 40);
	ctx.f31.f64 = double(temp.f32);
	// fadds f27,f8,f26
	ctx.f27.f64 = double(float(ctx.f8.f64 + ctx.f26.f64));
	// lfs f21,48(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 48);
	ctx.f21.f64 = double(temp.f32);
	// fsubs f8,f26,f8
	ctx.f8.f64 = double(float(ctx.f26.f64 - ctx.f8.f64));
	// lfs f20,116(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 116);
	ctx.f20.f64 = double(temp.f32);
	// fadds f26,f4,f5
	ctx.f26.f64 = double(float(ctx.f4.f64 + ctx.f5.f64));
	// lfs f19,52(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 52);
	ctx.f19.f64 = double(temp.f32);
	// fsubs f5,f4,f5
	ctx.f5.f64 = double(float(ctx.f4.f64 - ctx.f5.f64));
	// lfs f18,88(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 88);
	ctx.f18.f64 = double(temp.f32);
	// fsubs f4,f2,f3
	ctx.f4.f64 = double(float(ctx.f2.f64 - ctx.f3.f64));
	// lfs f17,24(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 24);
	ctx.f17.f64 = double(temp.f32);
	// fsubs f2,f29,f30
	ctx.f2.f64 = double(float(ctx.f29.f64 - ctx.f30.f64));
	// lfs f16,92(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 92);
	ctx.f16.f64 = double(temp.f32);
	// fsubs f3,f31,f1
	ctx.f3.f64 = double(float(ctx.f31.f64 - ctx.f1.f64));
	// fsubs f24,f12,f7
	ctx.f24.f64 = double(float(ctx.f12.f64 - ctx.f7.f64));
	// fadds f7,f7,f12
	ctx.f7.f64 = double(float(ctx.f7.f64 + ctx.f12.f64));
	// fadds f12,f28,f13
	ctx.f12.f64 = double(float(ctx.f28.f64 + ctx.f13.f64));
	// fadds f22,f29,f30
	ctx.f22.f64 = double(float(ctx.f29.f64 + ctx.f30.f64));
	// fadds f23,f9,f11
	ctx.f23.f64 = double(float(ctx.f9.f64 + ctx.f11.f64));
	// fsubs f11,f11,f9
	ctx.f11.f64 = double(float(ctx.f11.f64 - ctx.f9.f64));
	// fadds f9,f31,f1
	ctx.f9.f64 = double(float(ctx.f31.f64 + ctx.f1.f64));
	// fsubs f28,f5,f2
	ctx.f28.f64 = double(float(ctx.f5.f64 - ctx.f2.f64));
	// fadds f29,f3,f4
	ctx.f29.f64 = double(float(ctx.f3.f64 + ctx.f4.f64));
	// fsubs f4,f4,f3
	ctx.f4.f64 = double(float(ctx.f4.f64 - ctx.f3.f64));
	// fadds f5,f2,f5
	ctx.f5.f64 = double(float(ctx.f2.f64 + ctx.f5.f64));
	// fadds f31,f22,f25
	ctx.f31.f64 = double(float(ctx.f22.f64 + ctx.f25.f64));
	// fsubs f30,f25,f22
	ctx.f30.f64 = double(float(ctx.f25.f64 - ctx.f22.f64));
	// lfs f22,112(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 112);
	ctx.f22.f64 = double(temp.f32);
	// fadds f1,f9,f26
	ctx.f1.f64 = double(float(ctx.f9.f64 + ctx.f26.f64));
	// fsubs f9,f26,f9
	ctx.f9.f64 = double(float(ctx.f26.f64 - ctx.f9.f64));
	// fmuls f25,f28,f13
	ctx.f25.f64 = double(float(ctx.f28.f64 * ctx.f13.f64));
	// fmuls f26,f29,f13
	ctx.f26.f64 = double(float(ctx.f29.f64 * ctx.f13.f64));
	// fmuls f15,f4,f13
	ctx.f15.f64 = double(float(ctx.f4.f64 * ctx.f13.f64));
	// fmadds f2,f29,f12,f25
	ctx.f2.f64 = double(float(std::fma(ctx.f29.f64, ctx.f12.f64, ctx.f25.f64)));
	// lfs f25,20(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 20);
	ctx.f25.f64 = double(temp.f32);
	// fmuls f29,f4,f12
	ctx.f29.f64 = double(float(ctx.f4.f64 * ctx.f12.f64));
	// lfs f4,16(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 16);
	ctx.f4.f64 = double(temp.f32);
	// fmsubs f3,f28,f12,f26
	ctx.f3.f64 = double(float(std::fma(ctx.f28.f64, ctx.f12.f64, -ctx.f26.f64)));
	// lfs f28,80(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 80);
	ctx.f28.f64 = double(temp.f32);
	// lfs f26,84(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 84);
	ctx.f26.f64 = double(temp.f32);
	// fadds f14,f4,f28
	ctx.f14.f64 = double(float(ctx.f4.f64 + ctx.f28.f64));
	// fsubs f28,f4,f28
	ctx.f28.f64 = double(float(ctx.f4.f64 - ctx.f28.f64));
	// stfs f28,-188(r1)
	temp.f32 = float(ctx.f28.f64);
	REX_STORE_U32(ctx.r1.u32 + -188, temp.u32);
	// fadds f4,f25,f26
	ctx.f4.f64 = double(float(ctx.f25.f64 + ctx.f26.f64));
	// fsubs f26,f25,f26
	ctx.f26.f64 = double(float(ctx.f25.f64 - ctx.f26.f64));
	// stfs f26,-192(r1)
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r1.u32 + -192, temp.u32);
	// fadds f26,f21,f22
	ctx.f26.f64 = double(float(ctx.f21.f64 + ctx.f22.f64));
	// fsubs f22,f21,f22
	ctx.f22.f64 = double(float(ctx.f21.f64 - ctx.f22.f64));
	// fadds f25,f19,f20
	ctx.f25.f64 = double(float(ctx.f19.f64 + ctx.f20.f64));
	// fmsubs f29,f5,f13,f29
	ctx.f29.f64 = double(float(std::fma(ctx.f5.f64, ctx.f13.f64, -ctx.f29.f64)));
	// fmadds f5,f5,f12,f15
	ctx.f5.f64 = double(float(std::fma(ctx.f5.f64, ctx.f12.f64, ctx.f15.f64)));
	// lfs f15,28(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 28);
	ctx.f15.f64 = double(temp.f32);
	// fsubs f21,f19,f20
	ctx.f21.f64 = double(float(ctx.f19.f64 - ctx.f20.f64));
	// fadds f20,f26,f14
	ctx.f20.f64 = double(float(ctx.f26.f64 + ctx.f14.f64));
	// stfs f20,-168(r1)
	temp.f32 = float(ctx.f20.f64);
	REX_STORE_U32(ctx.r1.u32 + -168, temp.u32);
	// fsubs f26,f14,f26
	ctx.f26.f64 = double(float(ctx.f14.f64 - ctx.f26.f64));
	// stfs f26,-172(r1)
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r1.u32 + -172, temp.u32);
	// fadds f26,f25,f4
	ctx.f26.f64 = double(float(ctx.f25.f64 + ctx.f4.f64));
	// stfs f26,-164(r1)
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r1.u32 + -164, temp.u32);
	// fsubs f4,f4,f25
	ctx.f4.f64 = double(float(ctx.f4.f64 - ctx.f25.f64));
	// stfs f4,-176(r1)
	temp.f32 = float(ctx.f4.f64);
	REX_STORE_U32(ctx.r1.u32 + -176, temp.u32);
	// lfs f4,-192(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -192);
	ctx.f4.f64 = double(temp.f32);
	// fadds f19,f22,f4
	ctx.f19.f64 = double(float(ctx.f22.f64 + ctx.f4.f64));
	// lfs f4,120(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 120);
	ctx.f4.f64 = double(temp.f32);
	// lfs f25,60(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 60);
	ctx.f25.f64 = double(temp.f32);
	// fsubs f20,f28,f21
	ctx.f20.f64 = double(float(ctx.f28.f64 - ctx.f21.f64));
	// lfs f28,56(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 56);
	ctx.f28.f64 = double(temp.f32);
	// fadds f26,f28,f4
	ctx.f26.f64 = double(float(ctx.f28.f64 + ctx.f4.f64));
	// stfs f26,-184(r1)
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r1.u32 + -184, temp.u32);
	// lfs f26,124(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 124);
	ctx.f26.f64 = double(temp.f32);
	// fsubs f4,f28,f4
	ctx.f4.f64 = double(float(ctx.f28.f64 - ctx.f4.f64));
	// fadds f14,f25,f26
	ctx.f14.f64 = double(float(ctx.f25.f64 + ctx.f26.f64));
	// stfs f14,-180(r1)
	temp.f32 = float(ctx.f14.f64);
	REX_STORE_U32(ctx.r1.u32 + -180, temp.u32);
	// fsubs f26,f25,f26
	ctx.f26.f64 = double(float(ctx.f25.f64 - ctx.f26.f64));
	// fsubs f25,f15,f16
	ctx.f25.f64 = double(float(ctx.f15.f64 - ctx.f16.f64));
	// fsubs f14,f20,f19
	ctx.f14.f64 = double(float(ctx.f20.f64 - ctx.f19.f64));
	// fadds f19,f19,f20
	ctx.f19.f64 = double(float(ctx.f19.f64 + ctx.f20.f64));
	// lfs f20,-188(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -188);
	ctx.f20.f64 = double(temp.f32);
	// fadds f21,f21,f20
	ctx.f21.f64 = double(float(ctx.f21.f64 + ctx.f20.f64));
	// lfs f20,-192(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -192);
	ctx.f20.f64 = double(temp.f32);
	// fsubs f22,f20,f22
	ctx.f22.f64 = double(float(ctx.f20.f64 - ctx.f22.f64));
	// fsubs f20,f17,f18
	ctx.f20.f64 = double(float(ctx.f17.f64 - ctx.f18.f64));
	// fmuls f28,f14,f0
	ctx.f28.f64 = double(float(ctx.f14.f64 * ctx.f0.f64));
	// fmuls f19,f19,f0
	ctx.f19.f64 = double(float(ctx.f19.f64 * ctx.f0.f64));
	// fadds f14,f22,f21
	ctx.f14.f64 = double(float(ctx.f22.f64 + ctx.f21.f64));
	// fsubs f22,f22,f21
	ctx.f22.f64 = double(float(ctx.f22.f64 - ctx.f21.f64));
	// stfs f22,-188(r1)
	temp.f32 = float(ctx.f22.f64);
	REX_STORE_U32(ctx.r1.u32 + -188, temp.u32);
	// fadds f22,f17,f18
	ctx.f22.f64 = double(float(ctx.f17.f64 + ctx.f18.f64));
	// fadds f21,f15,f16
	ctx.f21.f64 = double(float(ctx.f15.f64 + ctx.f16.f64));
	// fmuls f18,f14,f0
	ctx.f18.f64 = double(float(ctx.f14.f64 * ctx.f0.f64));
	// lfs f15,-184(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -184);
	ctx.f15.f64 = double(temp.f32);
	// fadds f16,f15,f22
	ctx.f16.f64 = double(float(ctx.f15.f64 + ctx.f22.f64));
	// fsubs f22,f22,f15
	ctx.f22.f64 = double(float(ctx.f22.f64 - ctx.f15.f64));
	// stfs f22,-184(r1)
	temp.f32 = float(ctx.f22.f64);
	REX_STORE_U32(ctx.r1.u32 + -184, temp.u32);
	// lfs f22,-180(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -180);
	ctx.f22.f64 = double(temp.f32);
	// fadds f15,f22,f21
	ctx.f15.f64 = double(float(ctx.f22.f64 + ctx.f21.f64));
	// fsubs f22,f21,f22
	ctx.f22.f64 = double(float(ctx.f21.f64 - ctx.f22.f64));
	// stfs f22,-180(r1)
	temp.f32 = float(ctx.f22.f64);
	REX_STORE_U32(ctx.r1.u32 + -180, temp.u32);
	// fadds f21,f4,f25
	ctx.f21.f64 = double(float(ctx.f4.f64 + ctx.f25.f64));
	// fsubs f22,f20,f26
	ctx.f22.f64 = double(float(ctx.f20.f64 - ctx.f26.f64));
	// fadds f26,f26,f20
	ctx.f26.f64 = double(float(ctx.f26.f64 + ctx.f20.f64));
	// fsubs f4,f25,f4
	ctx.f4.f64 = double(float(ctx.f25.f64 - ctx.f4.f64));
	// fmr f20,f26
	ctx.f20.f64 = ctx.f26.f64;
	// lfs f14,-188(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -188);
	ctx.f14.f64 = double(temp.f32);
	// fmuls f17,f14,f0
	ctx.f17.f64 = double(float(ctx.f14.f64 * ctx.f0.f64));
	// stfs f15,-188(r1)
	temp.f32 = float(ctx.f15.f64);
	REX_STORE_U32(ctx.r1.u32 + -188, temp.u32);
	// fmuls f14,f21,f12
	ctx.f14.f64 = double(float(ctx.f21.f64 * ctx.f12.f64));
	// fmuls f21,f21,f13
	ctx.f21.f64 = double(float(ctx.f21.f64 * ctx.f13.f64));
	// fmr f15,f22
	ctx.f15.f64 = ctx.f22.f64;
	// fadds f25,f17,f11
	ctx.f25.f64 = double(float(ctx.f17.f64 + ctx.f11.f64));
	// fmsubs f22,f22,f13,f14
	ctx.f22.f64 = double(float(std::fma(ctx.f22.f64, ctx.f13.f64, -ctx.f14.f64)));
	// fmadds f21,f15,f12,f21
	ctx.f21.f64 = double(float(std::fma(ctx.f15.f64, ctx.f12.f64, ctx.f21.f64)));
	// fmuls f15,f26,f13
	ctx.f15.f64 = double(float(ctx.f26.f64 * ctx.f13.f64));
	// fsubs f26,f7,f18
	ctx.f26.f64 = double(float(ctx.f7.f64 - ctx.f18.f64));
	// fadds f7,f18,f7
	ctx.f7.f64 = double(float(ctx.f18.f64 + ctx.f7.f64));
	// fmuls f18,f4,f13
	ctx.f18.f64 = double(float(ctx.f4.f64 * ctx.f13.f64));
	// fsubs f13,f11,f17
	ctx.f13.f64 = double(float(ctx.f11.f64 - ctx.f17.f64));
	// fmsubs f11,f20,f12,f18
	ctx.f11.f64 = double(float(std::fma(ctx.f20.f64, ctx.f12.f64, -ctx.f18.f64)));
	// fmadds f12,f4,f12,f15
	ctx.f12.f64 = double(float(std::fma(ctx.f4.f64, ctx.f12.f64, ctx.f15.f64)));
	// fsubs f4,f29,f11
	ctx.f4.f64 = double(float(ctx.f29.f64 - ctx.f11.f64));
	// fsubs f20,f5,f12
	ctx.f20.f64 = double(float(ctx.f5.f64 - ctx.f12.f64));
	// fadds f12,f12,f5
	ctx.f12.f64 = double(float(ctx.f12.f64 + ctx.f5.f64));
	// fadds f11,f11,f29
	ctx.f11.f64 = double(float(ctx.f11.f64 + ctx.f29.f64));
	// fadds f5,f4,f26
	ctx.f5.f64 = double(float(ctx.f4.f64 + ctx.f26.f64));
	// stfs f5,96(r3)
	temp.f32 = float(ctx.f5.f64);
	REX_STORE_U32(ctx.r3.u32 + 96, temp.u32);
	// fadds f5,f20,f13
	ctx.f5.f64 = double(float(ctx.f20.f64 + ctx.f13.f64));
	// stfs f5,100(r3)
	temp.f32 = float(ctx.f5.f64);
	REX_STORE_U32(ctx.r3.u32 + 100, temp.u32);
	// fsubs f5,f26,f4
	ctx.f5.f64 = double(float(ctx.f26.f64 - ctx.f4.f64));
	// stfs f5,104(r3)
	temp.f32 = float(ctx.f5.f64);
	REX_STORE_U32(ctx.r3.u32 + 104, temp.u32);
	// fsubs f13,f13,f20
	ctx.f13.f64 = double(float(ctx.f13.f64 - ctx.f20.f64));
	// stfs f13,108(r3)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r3.u32 + 108, temp.u32);
	// fadds f13,f28,f24
	ctx.f13.f64 = double(float(ctx.f28.f64 + ctx.f24.f64));
	// fadds f5,f19,f23
	ctx.f5.f64 = double(float(ctx.f19.f64 + ctx.f23.f64));
	// fadds f4,f22,f3
	ctx.f4.f64 = double(float(ctx.f22.f64 + ctx.f3.f64));
	// fadds f29,f21,f2
	ctx.f29.f64 = double(float(ctx.f21.f64 + ctx.f2.f64));
	// fsubs f26,f7,f12
	ctx.f26.f64 = double(float(ctx.f7.f64 - ctx.f12.f64));
	// stfs f26,112(r3)
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r3.u32 + 112, temp.u32);
	// fadds f12,f12,f7
	ctx.f12.f64 = double(float(ctx.f12.f64 + ctx.f7.f64));
	// stfs f12,120(r3)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r3.u32 + 120, temp.u32);
	// fsubs f7,f3,f22
	ctx.f7.f64 = double(float(ctx.f3.f64 - ctx.f22.f64));
	// fsubs f3,f2,f21
	ctx.f3.f64 = double(float(ctx.f2.f64 - ctx.f21.f64));
	// fsubs f12,f25,f11
	ctx.f12.f64 = double(float(ctx.f25.f64 - ctx.f11.f64));
	// stfs f12,124(r3)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r3.u32 + 124, temp.u32);
	// fadds f26,f11,f25
	ctx.f26.f64 = double(float(ctx.f11.f64 + ctx.f25.f64));
	// stfs f26,116(r3)
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r3.u32 + 116, temp.u32);
	// fsubs f11,f23,f19
	ctx.f11.f64 = double(float(ctx.f23.f64 - ctx.f19.f64));
	// fsubs f12,f24,f28
	ctx.f12.f64 = double(float(ctx.f24.f64 - ctx.f28.f64));
	// fadds f2,f4,f13
	ctx.f2.f64 = double(float(ctx.f4.f64 + ctx.f13.f64));
	// stfs f2,64(r3)
	temp.f32 = float(ctx.f2.f64);
	REX_STORE_U32(ctx.r3.u32 + 64, temp.u32);
	// fsubs f13,f13,f4
	ctx.f13.f64 = double(float(ctx.f13.f64 - ctx.f4.f64));
	// stfs f13,72(r3)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r3.u32 + 72, temp.u32);
	// fadds f2,f29,f5
	ctx.f2.f64 = double(float(ctx.f29.f64 + ctx.f5.f64));
	// stfs f2,68(r3)
	temp.f32 = float(ctx.f2.f64);
	REX_STORE_U32(ctx.r3.u32 + 68, temp.u32);
	// fsubs f13,f5,f29
	ctx.f13.f64 = double(float(ctx.f5.f64 - ctx.f29.f64));
	// lfs f2,-184(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -184);
	ctx.f2.f64 = double(temp.f32);
	// lfs f29,-180(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -180);
	ctx.f29.f64 = double(temp.f32);
	// fadds f5,f2,f30
	ctx.f5.f64 = double(float(ctx.f2.f64 + ctx.f30.f64));
	// stfs f13,76(r3)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r3.u32 + 76, temp.u32);
	// fsubs f13,f9,f29
	ctx.f13.f64 = double(float(ctx.f9.f64 - ctx.f29.f64));
	// fadds f4,f7,f11
	ctx.f4.f64 = double(float(ctx.f7.f64 + ctx.f11.f64));
	// stfs f4,84(r3)
	temp.f32 = float(ctx.f4.f64);
	REX_STORE_U32(ctx.r3.u32 + 84, temp.u32);
	// fsubs f4,f12,f3
	ctx.f4.f64 = double(float(ctx.f12.f64 - ctx.f3.f64));
	// stfs f4,80(r3)
	temp.f32 = float(ctx.f4.f64);
	REX_STORE_U32(ctx.r3.u32 + 80, temp.u32);
	// fadds f12,f3,f12
	ctx.f12.f64 = double(float(ctx.f3.f64 + ctx.f12.f64));
	// stfs f12,88(r3)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r3.u32 + 88, temp.u32);
	// fsubs f12,f11,f7
	ctx.f12.f64 = double(float(ctx.f11.f64 - ctx.f7.f64));
	// lfs f3,-176(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -176);
	ctx.f3.f64 = double(temp.f32);
	// lfs f4,-172(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -172);
	ctx.f4.f64 = double(temp.f32);
	// stfs f12,92(r3)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r3.u32 + 92, temp.u32);
	// fadds f12,f3,f10
	ctx.f12.f64 = double(float(ctx.f3.f64 + ctx.f10.f64));
	// fsubs f11,f8,f4
	ctx.f11.f64 = double(float(ctx.f8.f64 - ctx.f4.f64));
	// fsubs f7,f13,f5
	ctx.f7.f64 = double(float(ctx.f13.f64 - ctx.f5.f64));
	// fadds f5,f5,f13
	ctx.f5.f64 = double(float(ctx.f5.f64 + ctx.f13.f64));
	// fadds f13,f29,f9
	ctx.f13.f64 = double(float(ctx.f29.f64 + ctx.f9.f64));
	// fsubs f9,f30,f2
	ctx.f9.f64 = double(float(ctx.f30.f64 - ctx.f2.f64));
	// fmuls f7,f7,f0
	ctx.f7.f64 = double(float(ctx.f7.f64 * ctx.f0.f64));
	// fmuls f5,f5,f0
	ctx.f5.f64 = double(float(ctx.f5.f64 * ctx.f0.f64));
	// fsubs f2,f13,f9
	ctx.f2.f64 = double(float(ctx.f13.f64 - ctx.f9.f64));
	// fadds f30,f9,f13
	ctx.f30.f64 = double(float(ctx.f9.f64 + ctx.f13.f64));
	// fsubs f13,f10,f3
	ctx.f13.f64 = double(float(ctx.f10.f64 - ctx.f3.f64));
	// fadds f10,f4,f8
	ctx.f10.f64 = double(float(ctx.f4.f64 + ctx.f8.f64));
	// fmuls f9,f2,f0
	ctx.f9.f64 = double(float(ctx.f2.f64 * ctx.f0.f64));
	// fmuls f0,f30,f0
	ctx.f0.f64 = double(float(ctx.f30.f64 * ctx.f0.f64));
	// fadds f8,f7,f13
	ctx.f8.f64 = double(float(ctx.f7.f64 + ctx.f13.f64));
	// stfs f8,32(r3)
	temp.f32 = float(ctx.f8.f64);
	REX_STORE_U32(ctx.r3.u32 + 32, temp.u32);
	// fsubs f13,f13,f7
	ctx.f13.f64 = double(float(ctx.f13.f64 - ctx.f7.f64));
	// stfs f13,40(r3)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r3.u32 + 40, temp.u32);
	// fsubs f13,f10,f5
	ctx.f13.f64 = double(float(ctx.f10.f64 - ctx.f5.f64));
	// stfs f13,44(r3)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r3.u32 + 44, temp.u32);
	// fadds f8,f5,f10
	ctx.f8.f64 = double(float(ctx.f5.f64 + ctx.f10.f64));
	// lfs f7,-188(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -188);
	ctx.f7.f64 = double(temp.f32);
	// fadds f10,f16,f1
	ctx.f10.f64 = double(float(ctx.f16.f64 + ctx.f1.f64));
	// stfs f8,36(r3)
	temp.f32 = float(ctx.f8.f64);
	REX_STORE_U32(ctx.r3.u32 + 36, temp.u32);
	// fsubs f8,f1,f16
	ctx.f8.f64 = double(float(ctx.f1.f64 - ctx.f16.f64));
	// fsubs f13,f12,f0
	ctx.f13.f64 = double(float(ctx.f12.f64 - ctx.f0.f64));
	// stfs f13,48(r3)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r3.u32 + 48, temp.u32);
	// fadds f0,f0,f12
	ctx.f0.f64 = double(float(ctx.f0.f64 + ctx.f12.f64));
	// stfs f0,56(r3)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r3.u32 + 56, temp.u32);
	// lfs f12,-168(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -168);
	ctx.f12.f64 = double(temp.f32);
	// fsubs f0,f11,f9
	ctx.f0.f64 = double(float(ctx.f11.f64 - ctx.f9.f64));
	// fadds f13,f9,f11
	ctx.f13.f64 = double(float(ctx.f9.f64 + ctx.f11.f64));
	// stfs f0,60(r3)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r3.u32 + 60, temp.u32);
	// lfs f11,-164(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -164);
	ctx.f11.f64 = double(temp.f32);
	// fadds f0,f12,f6
	ctx.f0.f64 = double(float(ctx.f12.f64 + ctx.f6.f64));
	// stfs f13,52(r3)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r3.u32 + 52, temp.u32);
	// fadds f9,f7,f31
	ctx.f9.f64 = double(float(ctx.f7.f64 + ctx.f31.f64));
	// fadds f13,f11,f27
	ctx.f13.f64 = double(float(ctx.f11.f64 + ctx.f27.f64));
	// fsubs f12,f6,f12
	ctx.f12.f64 = double(float(ctx.f6.f64 - ctx.f12.f64));
	// fsubs f11,f27,f11
	ctx.f11.f64 = double(float(ctx.f27.f64 - ctx.f11.f64));
	// fsubs f7,f31,f7
	ctx.f7.f64 = double(float(ctx.f31.f64 - ctx.f7.f64));
	// fadds f6,f10,f0
	ctx.f6.f64 = double(float(ctx.f10.f64 + ctx.f0.f64));
	// stfs f6,0(r3)
	temp.f32 = float(ctx.f6.f64);
	REX_STORE_U32(ctx.r3.u32 + 0, temp.u32);
	// fadds f6,f9,f13
	ctx.f6.f64 = double(float(ctx.f9.f64 + ctx.f13.f64));
	// stfs f6,4(r3)
	temp.f32 = float(ctx.f6.f64);
	REX_STORE_U32(ctx.r3.u32 + 4, temp.u32);
	// fsubs f0,f0,f10
	ctx.f0.f64 = double(float(ctx.f0.f64 - ctx.f10.f64));
	// stfs f0,8(r3)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r3.u32 + 8, temp.u32);
	// fsubs f0,f13,f9
	ctx.f0.f64 = double(float(ctx.f13.f64 - ctx.f9.f64));
	// stfs f0,12(r3)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r3.u32 + 12, temp.u32);
	// fsubs f0,f12,f7
	ctx.f0.f64 = double(float(ctx.f12.f64 - ctx.f7.f64));
	// stfs f0,16(r3)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r3.u32 + 16, temp.u32);
	// fadds f0,f8,f11
	ctx.f0.f64 = double(float(ctx.f8.f64 + ctx.f11.f64));
	// stfs f0,20(r3)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r3.u32 + 20, temp.u32);
	// fadds f0,f7,f12
	ctx.f0.f64 = double(float(ctx.f7.f64 + ctx.f12.f64));
	// stfs f0,24(r3)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r3.u32 + 24, temp.u32);
	// fsubs f0,f11,f8
	ctx.f0.f64 = double(float(ctx.f11.f64 - ctx.f8.f64));
	// stfs f0,28(r3)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r3.u32 + 28, temp.u32);
	// addi r12,r1,-8
	ctx.r12.s64 = ctx.r1.s64 + -8;
	// bl 0x82a0133c
	ctx.lr = 0x82A82564;
	__restfpr_14(ctx, base);
	// lwz r12,-8(r1)
	ctx.r12.u64 = REX_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

DEFINE_REX_FUNC(sub_82A82570) {
	REX_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	REX_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// addi r12,r1,-8
	ctx.r12.s64 = ctx.r1.s64 + -8;
	// bl 0x82a012f0
	ctx.lr = 0x82A82580;
	__savefpr_14(ctx, base);
	// lfs f6,100(r3)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 100);
	ctx.f6.f64 = double(temp.f32);
	// lfs f5,96(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 96);
	ctx.f5.f64 = double(temp.f32);
	// lfs f4,36(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 36);
	ctx.f4.f64 = double(temp.f32);
	// lfs f7,32(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 32);
	ctx.f7.f64 = double(temp.f32);
	// fadds f21,f4,f5
	ctx.f21.f64 = double(float(ctx.f4.f64 + ctx.f5.f64));
	// fsubs f22,f7,f6
	ctx.f22.f64 = double(float(ctx.f7.f64 - ctx.f6.f64));
	// lfs f0,4(r4)
	temp.u32 = REX_LOAD_U32(ctx.r4.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// fadds f7,f6,f7
	ctx.f7.f64 = double(float(ctx.f6.f64 + ctx.f7.f64));
	// lfs f28,64(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 64);
	ctx.f28.f64 = double(temp.f32);
	// fsubs f6,f4,f5
	ctx.f6.f64 = double(float(ctx.f4.f64 - ctx.f5.f64));
	// lfs f27,4(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 4);
	ctx.f27.f64 = double(temp.f32);
	// fadds f19,f27,f28
	ctx.f19.f64 = double(float(ctx.f27.f64 + ctx.f28.f64));
	// lfs f3,8(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 8);
	ctx.f3.f64 = double(temp.f32);
	// lfs f2,76(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 76);
	ctx.f2.f64 = double(temp.f32);
	// lfs f1,72(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 72);
	ctx.f1.f64 = double(temp.f32);
	// lfs f31,12(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 12);
	ctx.f31.f64 = double(temp.f32);
	// lfs f29,68(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 68);
	ctx.f29.f64 = double(temp.f32);
	// lfs f30,0(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 0);
	ctx.f30.f64 = double(temp.f32);
	// fsubs f20,f30,f29
	ctx.f20.f64 = double(float(ctx.f30.f64 - ctx.f29.f64));
	// lfs f13,16(r4)
	temp.u32 = REX_LOAD_U32(ctx.r4.u32 + 16);
	ctx.f13.f64 = double(temp.f32);
	// fsubs f18,f22,f21
	ctx.f18.f64 = double(float(ctx.f22.f64 - ctx.f21.f64));
	// lfs f12,20(r4)
	temp.u32 = REX_LOAD_U32(ctx.r4.u32 + 20);
	ctx.f12.f64 = double(temp.f32);
	// fadds f22,f21,f22
	ctx.f22.f64 = double(float(ctx.f21.f64 + ctx.f22.f64));
	// lfs f24,104(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 104);
	ctx.f24.f64 = double(temp.f32);
	// fadds f17,f6,f7
	ctx.f17.f64 = double(float(ctx.f6.f64 + ctx.f7.f64));
	// lfs f23,44(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 44);
	ctx.f23.f64 = double(temp.f32);
	// fadds f30,f29,f30
	ctx.f30.f64 = double(float(ctx.f29.f64 + ctx.f30.f64));
	// lfs f26,40(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 40);
	ctx.f26.f64 = double(temp.f32);
	// fsubs f29,f27,f28
	ctx.f29.f64 = double(float(ctx.f27.f64 - ctx.f28.f64));
	// lfs f25,108(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 108);
	ctx.f25.f64 = double(temp.f32);
	// lfs f11,24(r4)
	temp.u32 = REX_LOAD_U32(ctx.r4.u32 + 24);
	ctx.f11.f64 = double(temp.f32);
	// lfs f10,28(r4)
	temp.u32 = REX_LOAD_U32(ctx.r4.u32 + 28);
	ctx.f10.f64 = double(temp.f32);
	// lfs f9,32(r4)
	temp.u32 = REX_LOAD_U32(ctx.r4.u32 + 32);
	ctx.f9.f64 = double(temp.f32);
	// lfs f8,36(r4)
	temp.u32 = REX_LOAD_U32(ctx.r4.u32 + 36);
	ctx.f8.f64 = double(temp.f32);
	// lfs f15,116(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 116);
	ctx.f15.f64 = double(temp.f32);
	// fmuls f5,f18,f0
	ctx.f5.f64 = double(float(ctx.f18.f64 * ctx.f0.f64));
	// fsubs f18,f7,f6
	ctx.f18.f64 = double(float(ctx.f7.f64 - ctx.f6.f64));
	// fmuls f4,f22,f0
	ctx.f4.f64 = double(float(ctx.f22.f64 * ctx.f0.f64));
	// fsubs f7,f3,f2
	ctx.f7.f64 = double(float(ctx.f3.f64 - ctx.f2.f64));
	// fadds f6,f31,f1
	ctx.f6.f64 = double(float(ctx.f31.f64 + ctx.f1.f64));
	// fmuls f27,f17,f0
	ctx.f27.f64 = double(float(ctx.f17.f64 * ctx.f0.f64));
	// fadds f22,f5,f20
	ctx.f22.f64 = double(float(ctx.f5.f64 + ctx.f20.f64));
	// fmuls f28,f18,f0
	ctx.f28.f64 = double(float(ctx.f18.f64 * ctx.f0.f64));
	// fadds f21,f4,f19
	ctx.f21.f64 = double(float(ctx.f4.f64 + ctx.f19.f64));
	// fsubs f4,f19,f4
	ctx.f4.f64 = double(float(ctx.f19.f64 - ctx.f4.f64));
	// fmr f19,f7
	ctx.f19.f64 = ctx.f7.f64;
	// fmuls f18,f6,f12
	ctx.f18.f64 = double(float(ctx.f6.f64 * ctx.f12.f64));
	// fmuls f16,f6,f13
	ctx.f16.f64 = double(float(ctx.f6.f64 * ctx.f13.f64));
	// fadds f6,f23,f24
	ctx.f6.f64 = double(float(ctx.f23.f64 + ctx.f24.f64));
	// fsubs f5,f20,f5
	ctx.f5.f64 = double(float(ctx.f20.f64 - ctx.f5.f64));
	// fmr f17,f7
	ctx.f17.f64 = ctx.f7.f64;
	// fsubs f7,f26,f25
	ctx.f7.f64 = double(float(ctx.f26.f64 - ctx.f25.f64));
	// fadds f20,f28,f29
	ctx.f20.f64 = double(float(ctx.f28.f64 + ctx.f29.f64));
	// fsubs f29,f29,f28
	ctx.f29.f64 = double(float(ctx.f29.f64 - ctx.f28.f64));
	// fsubs f28,f30,f27
	ctx.f28.f64 = double(float(ctx.f30.f64 - ctx.f27.f64));
	// fadds f30,f27,f30
	ctx.f30.f64 = double(float(ctx.f27.f64 + ctx.f30.f64));
	// fmsubs f27,f19,f13,f18
	ctx.f27.f64 = double(float(std::fma(ctx.f19.f64, ctx.f13.f64, -ctx.f18.f64)));
	// fmuls f18,f6,f11
	ctx.f18.f64 = double(float(ctx.f6.f64 * ctx.f11.f64));
	// fmadds f19,f17,f12,f16
	ctx.f19.f64 = double(float(std::fma(ctx.f17.f64, ctx.f12.f64, ctx.f16.f64)));
	// fmuls f16,f7,f11
	ctx.f16.f64 = double(float(ctx.f7.f64 * ctx.f11.f64));
	// fmr f17,f6
	ctx.f17.f64 = ctx.f6.f64;
	// fadds f6,f2,f3
	ctx.f6.f64 = double(float(ctx.f2.f64 + ctx.f3.f64));
	// stfs f6,-200(r1)
	temp.f32 = float(ctx.f6.f64);
	REX_STORE_U32(ctx.r1.u32 + -200, temp.u32);
	// fsubs f2,f31,f1
	ctx.f2.f64 = double(float(ctx.f31.f64 - ctx.f1.f64));
	// lfs f3,16(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 16);
	ctx.f3.f64 = double(temp.f32);
	// lfs f1,84(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 84);
	ctx.f1.f64 = double(temp.f32);
	// lfs f31,80(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 80);
	ctx.f31.f64 = double(temp.f32);
	// fmsubs f7,f7,f10,f18
	ctx.f7.f64 = double(float(std::fma(ctx.f7.f64, ctx.f10.f64, -ctx.f18.f64)));
	// fmadds f18,f17,f10,f16
	ctx.f18.f64 = double(float(std::fma(ctx.f17.f64, ctx.f10.f64, ctx.f16.f64)));
	// lfs f17,20(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 20);
	ctx.f17.f64 = double(temp.f32);
	// lfs f16,48(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 48);
	ctx.f16.f64 = double(temp.f32);
	// fadds f14,f7,f27
	ctx.f14.f64 = double(float(ctx.f7.f64 + ctx.f27.f64));
	// fsubs f7,f27,f7
	ctx.f7.f64 = double(float(ctx.f27.f64 - ctx.f7.f64));
	// stfs f7,-192(r1)
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r1.u32 + -192, temp.u32);
	// fmr f7,f6
	ctx.f7.f64 = ctx.f6.f64;
	// stfs f7,-204(r1)
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r1.u32 + -204, temp.u32);
	// fmuls f27,f2,f10
	ctx.f27.f64 = double(float(ctx.f2.f64 * ctx.f10.f64));
	// fmuls f2,f2,f11
	ctx.f2.f64 = double(float(ctx.f2.f64 * ctx.f11.f64));
	// stfs f2,-196(r1)
	temp.f32 = float(ctx.f2.f64);
	REX_STORE_U32(ctx.r1.u32 + -196, temp.u32);
	// fadds f7,f25,f26
	ctx.f7.f64 = double(float(ctx.f25.f64 + ctx.f26.f64));
	// fsubs f6,f23,f24
	ctx.f6.f64 = double(float(ctx.f23.f64 - ctx.f24.f64));
	// stfs f27,-208(r1)
	temp.f32 = float(ctx.f27.f64);
	REX_STORE_U32(ctx.r1.u32 + -208, temp.u32);
	// fadds f2,f18,f19
	ctx.f2.f64 = double(float(ctx.f18.f64 + ctx.f19.f64));
	// lfs f26,-204(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -204);
	ctx.f26.f64 = double(temp.f32);
	// fsubs f27,f19,f18
	ctx.f27.f64 = double(float(ctx.f19.f64 - ctx.f18.f64));
	// lfs f24,-196(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -196);
	ctx.f24.f64 = double(temp.f32);
	// fmuls f19,f7,f12
	ctx.f19.f64 = double(float(ctx.f7.f64 * ctx.f12.f64));
	// fmr f23,f6
	ctx.f23.f64 = ctx.f6.f64;
	// lfs f25,-208(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -208);
	ctx.f25.f64 = double(temp.f32);
	// fmsubs f25,f26,f11,f25
	ctx.f25.f64 = double(float(std::fma(ctx.f26.f64, ctx.f11.f64, -ctx.f25.f64)));
	// stfs f25,-204(r1)
	temp.f32 = float(ctx.f25.f64);
	REX_STORE_U32(ctx.r1.u32 + -204, temp.u32);
	// lfs f26,-200(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -200);
	ctx.f26.f64 = double(temp.f32);
	// fmadds f26,f26,f10,f24
	ctx.f26.f64 = double(float(std::fma(ctx.f26.f64, ctx.f10.f64, ctx.f24.f64)));
	// stfs f26,-208(r1)
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r1.u32 + -208, temp.u32);
	// fmuls f26,f7,f13
	ctx.f26.f64 = double(float(ctx.f7.f64 * ctx.f13.f64));
	// fmr f24,f6
	ctx.f24.f64 = ctx.f6.f64;
	// fsubs f7,f3,f1
	ctx.f7.f64 = double(float(ctx.f3.f64 - ctx.f1.f64));
	// fadds f6,f17,f31
	ctx.f6.f64 = double(float(ctx.f17.f64 + ctx.f31.f64));
	// fadds f3,f1,f3
	ctx.f3.f64 = double(float(ctx.f1.f64 + ctx.f3.f64));
	// fsubs f1,f17,f31
	ctx.f1.f64 = double(float(ctx.f17.f64 - ctx.f31.f64));
	// fmadds f26,f24,f12,f26
	ctx.f26.f64 = double(float(std::fma(ctx.f24.f64, ctx.f12.f64, ctx.f26.f64)));
	// fmsubs f24,f23,f13,f19
	ctx.f24.f64 = double(float(std::fma(ctx.f23.f64, ctx.f13.f64, -ctx.f19.f64)));
	// stfs f24,-196(r1)
	temp.f32 = float(ctx.f24.f64);
	REX_STORE_U32(ctx.r1.u32 + -196, temp.u32);
	// fmr f18,f7
	ctx.f18.f64 = ctx.f7.f64;
	// fmr f23,f7
	ctx.f23.f64 = ctx.f7.f64;
	// fmuls f7,f6,f9
	ctx.f7.f64 = double(float(ctx.f6.f64 * ctx.f9.f64));
	// stfs f7,-200(r1)
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r1.u32 + -200, temp.u32);
	// fmuls f19,f6,f8
	ctx.f19.f64 = double(float(ctx.f6.f64 * ctx.f8.f64));
	// lfs f6,52(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 52);
	ctx.f6.f64 = double(temp.f32);
	// lfs f7,112(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 112);
	ctx.f7.f64 = double(temp.f32);
	// fmr f17,f1
	ctx.f17.f64 = ctx.f1.f64;
	// fsubs f25,f25,f26
	ctx.f25.f64 = double(float(ctx.f25.f64 - ctx.f26.f64));
	// stfs f25,-176(r1)
	temp.f32 = float(ctx.f25.f64);
	REX_STORE_U32(ctx.r1.u32 + -176, temp.u32);
	// fmsubs f23,f23,f9,f19
	ctx.f23.f64 = double(float(std::fma(ctx.f23.f64, ctx.f9.f64, -ctx.f19.f64)));
	// lfs f25,-204(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -204);
	ctx.f25.f64 = double(temp.f32);
	// fadds f26,f26,f25
	ctx.f26.f64 = double(float(ctx.f26.f64 + ctx.f25.f64));
	// stfs f26,-188(r1)
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r1.u32 + -188, temp.u32);
	// fmr f26,f24
	ctx.f26.f64 = ctx.f24.f64;
	// lfs f25,-208(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -208);
	ctx.f25.f64 = double(temp.f32);
	// fsubs f25,f25,f26
	ctx.f25.f64 = double(float(ctx.f25.f64 - ctx.f26.f64));
	// stfs f25,-172(r1)
	temp.f32 = float(ctx.f25.f64);
	REX_STORE_U32(ctx.r1.u32 + -172, temp.u32);
	// lfs f25,-208(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -208);
	ctx.f25.f64 = double(temp.f32);
	// fadds f24,f26,f25
	ctx.f24.f64 = double(float(ctx.f26.f64 + ctx.f25.f64));
	// fadds f25,f6,f7
	ctx.f25.f64 = double(float(ctx.f6.f64 + ctx.f7.f64));
	// fsubs f7,f6,f7
	ctx.f7.f64 = double(float(ctx.f6.f64 - ctx.f7.f64));
	// lfs f19,-200(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -200);
	ctx.f19.f64 = double(temp.f32);
	// fmadds f26,f18,f8,f19
	ctx.f26.f64 = double(float(std::fma(ctx.f18.f64, ctx.f8.f64, ctx.f19.f64)));
	// stfs f26,-208(r1)
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r1.u32 + -208, temp.u32);
	// fsubs f26,f16,f15
	ctx.f26.f64 = double(float(ctx.f16.f64 - ctx.f15.f64));
	// fmuls f19,f25,f9
	ctx.f19.f64 = double(float(ctx.f25.f64 * ctx.f9.f64));
	// fmuls f18,f26,f9
	ctx.f18.f64 = double(float(ctx.f26.f64 * ctx.f9.f64));
	// fmsubs f31,f26,f8,f19
	ctx.f31.f64 = double(float(std::fma(ctx.f26.f64, ctx.f8.f64, -ctx.f19.f64)));
	// fmr f19,f3
	ctx.f19.f64 = ctx.f3.f64;
	// fmuls f3,f3,f9
	ctx.f3.f64 = double(float(ctx.f3.f64 * ctx.f9.f64));
	// stfs f3,-196(r1)
	temp.f32 = float(ctx.f3.f64);
	REX_STORE_U32(ctx.r1.u32 + -196, temp.u32);
	// fadds f3,f15,f16
	ctx.f3.f64 = double(float(ctx.f15.f64 + ctx.f16.f64));
	// fmuls f16,f7,f8
	ctx.f16.f64 = double(float(ctx.f7.f64 * ctx.f8.f64));
	// fmadds f26,f25,f8,f18
	ctx.f26.f64 = double(float(std::fma(ctx.f25.f64, ctx.f8.f64, ctx.f18.f64)));
	// fmuls f18,f1,f9
	ctx.f18.f64 = double(float(ctx.f1.f64 * ctx.f9.f64));
	// fmuls f15,f7,f9
	ctx.f15.f64 = double(float(ctx.f7.f64 * ctx.f9.f64));
	// fadds f6,f31,f23
	ctx.f6.f64 = double(float(ctx.f31.f64 + ctx.f23.f64));
	// fsubs f1,f23,f31
	ctx.f1.f64 = double(float(ctx.f23.f64 - ctx.f31.f64));
	// fmsubs f7,f3,f9,f16
	ctx.f7.f64 = double(float(std::fma(ctx.f3.f64, ctx.f9.f64, -ctx.f16.f64)));
	// lfs f9,28(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 28);
	ctx.f9.f64 = double(temp.f32);
	// lfs f25,-208(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -208);
	ctx.f25.f64 = double(temp.f32);
	// fadds f31,f26,f25
	ctx.f31.f64 = double(float(ctx.f26.f64 + ctx.f25.f64));
	// fsubs f26,f25,f26
	ctx.f26.f64 = double(float(ctx.f25.f64 - ctx.f26.f64));
	// fmsubs f25,f19,f8,f18
	ctx.f25.f64 = double(float(std::fma(ctx.f19.f64, ctx.f8.f64, -ctx.f18.f64)));
	// lfs f18,92(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 92);
	ctx.f18.f64 = double(temp.f32);
	// lfs f19,-196(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -196);
	ctx.f19.f64 = double(temp.f32);
	// fmadds f23,f17,f8,f19
	ctx.f23.f64 = double(float(std::fma(ctx.f17.f64, ctx.f8.f64, ctx.f19.f64)));
	// lfs f19,24(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 24);
	ctx.f19.f64 = double(temp.f32);
	// fmadds f8,f3,f8,f15
	ctx.f8.f64 = double(float(std::fma(ctx.f3.f64, ctx.f8.f64, ctx.f15.f64)));
	// lfs f17,88(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 88);
	ctx.f17.f64 = double(temp.f32);
	// fsubs f15,f25,f7
	ctx.f15.f64 = double(float(ctx.f25.f64 - ctx.f7.f64));
	// stfs f15,-184(r1)
	temp.f32 = float(ctx.f15.f64);
	REX_STORE_U32(ctx.r1.u32 + -184, temp.u32);
	// fadds f7,f7,f25
	ctx.f7.f64 = double(float(ctx.f7.f64 + ctx.f25.f64));
	// stfs f7,-164(r1)
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r1.u32 + -164, temp.u32);
	// fsubs f3,f19,f18
	ctx.f3.f64 = double(float(ctx.f19.f64 - ctx.f18.f64));
	// fadds f16,f9,f17
	ctx.f16.f64 = double(float(ctx.f9.f64 + ctx.f17.f64));
	// fsubs f7,f23,f8
	ctx.f7.f64 = double(float(ctx.f23.f64 - ctx.f8.f64));
	// stfs f7,-180(r1)
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r1.u32 + -180, temp.u32);
	// fadds f8,f8,f23
	ctx.f8.f64 = double(float(ctx.f8.f64 + ctx.f23.f64));
	// stfs f8,-168(r1)
	temp.f32 = float(ctx.f8.f64);
	REX_STORE_U32(ctx.r1.u32 + -168, temp.u32);
	// fmuls f8,f16,f10
	ctx.f8.f64 = double(float(ctx.f16.f64 * ctx.f10.f64));
	// stfs f8,-196(r1)
	temp.f32 = float(ctx.f8.f64);
	REX_STORE_U32(ctx.r1.u32 + -196, temp.u32);
	// fmr f25,f3
	ctx.f25.f64 = ctx.f3.f64;
	// lfs f7,120(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 120);
	ctx.f7.f64 = double(temp.f32);
	// fmuls f16,f16,f11
	ctx.f16.f64 = double(float(ctx.f16.f64 * ctx.f11.f64));
	// lfs f8,56(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 56);
	ctx.f8.f64 = double(temp.f32);
	// fmr f15,f3
	ctx.f15.f64 = ctx.f3.f64;
	// lfs f3,60(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 60);
	ctx.f3.f64 = double(temp.f32);
	// fadds f23,f3,f7
	ctx.f23.f64 = double(float(ctx.f3.f64 + ctx.f7.f64));
	// stfs f23,-200(r1)
	temp.f32 = float(ctx.f23.f64);
	REX_STORE_U32(ctx.r1.u32 + -200, temp.u32);
	// fsubs f9,f9,f17
	ctx.f9.f64 = double(float(ctx.f9.f64 - ctx.f17.f64));
	// fmuls f17,f9,f13
	ctx.f17.f64 = double(float(ctx.f9.f64 * ctx.f13.f64));
	// lfs f3,-196(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -196);
	ctx.f3.f64 = double(temp.f32);
	// fmsubs f3,f25,f11,f3
	ctx.f3.f64 = double(float(std::fma(ctx.f25.f64, ctx.f11.f64, -ctx.f3.f64)));
	// stfs f3,-204(r1)
	temp.f32 = float(ctx.f3.f64);
	REX_STORE_U32(ctx.r1.u32 + -204, temp.u32);
	// fmadds f3,f15,f10,f16
	ctx.f3.f64 = double(float(std::fma(ctx.f15.f64, ctx.f10.f64, ctx.f16.f64)));
	// stfs f3,-208(r1)
	temp.f32 = float(ctx.f3.f64);
	REX_STORE_U32(ctx.r1.u32 + -208, temp.u32);
	// lfs f3,124(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 124);
	ctx.f3.f64 = double(temp.f32);
	// fmuls f15,f23,f13
	ctx.f15.f64 = double(float(ctx.f23.f64 * ctx.f13.f64));
	// fsubs f25,f8,f3
	ctx.f25.f64 = double(float(ctx.f8.f64 - ctx.f3.f64));
	// fmr f16,f25
	ctx.f16.f64 = ctx.f25.f64;
	// fmuls f25,f25,f13
	ctx.f25.f64 = double(float(ctx.f25.f64 * ctx.f13.f64));
	// stfs f25,-196(r1)
	temp.f32 = float(ctx.f25.f64);
	REX_STORE_U32(ctx.r1.u32 + -196, temp.u32);
	// fadds f25,f18,f19
	ctx.f25.f64 = double(float(ctx.f18.f64 + ctx.f19.f64));
	// fmsubs f23,f16,f12,f15
	ctx.f23.f64 = double(float(std::fma(ctx.f16.f64, ctx.f12.f64, -ctx.f15.f64)));
	// lfs f15,-200(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -200);
	ctx.f15.f64 = double(temp.f32);
	// fmr f18,f25
	ctx.f18.f64 = ctx.f25.f64;
	// lfs f16,-196(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -196);
	ctx.f16.f64 = double(temp.f32);
	// fmadds f19,f15,f12,f16
	ctx.f19.f64 = double(float(std::fma(ctx.f15.f64, ctx.f12.f64, ctx.f16.f64)));
	// fmr f16,f9
	ctx.f16.f64 = ctx.f9.f64;
	// lfs f9,60(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 60);
	ctx.f9.f64 = double(temp.f32);
	// fsubs f9,f9,f7
	ctx.f9.f64 = double(float(ctx.f9.f64 - ctx.f7.f64));
	// lfs f7,-204(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -204);
	ctx.f7.f64 = double(temp.f32);
	// fmuls f15,f25,f13
	ctx.f15.f64 = double(float(ctx.f25.f64 * ctx.f13.f64));
	// lfs f25,-208(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -208);
	ctx.f25.f64 = double(temp.f32);
	// fadds f13,f3,f8
	ctx.f13.f64 = double(float(ctx.f3.f64 + ctx.f8.f64));
	// fadds f8,f23,f7
	ctx.f8.f64 = double(float(ctx.f23.f64 + ctx.f7.f64));
	// fsubs f7,f7,f23
	ctx.f7.f64 = double(float(ctx.f7.f64 - ctx.f23.f64));
	// fmadds f23,f18,f12,f17
	ctx.f23.f64 = double(float(std::fma(ctx.f18.f64, ctx.f12.f64, ctx.f17.f64)));
	// fadds f3,f19,f25
	ctx.f3.f64 = double(float(ctx.f19.f64 + ctx.f25.f64));
	// fsubs f25,f25,f19
	ctx.f25.f64 = double(float(ctx.f25.f64 - ctx.f19.f64));
	// fmuls f19,f9,f11
	ctx.f19.f64 = double(float(ctx.f9.f64 * ctx.f11.f64));
	// fmsubs f12,f16,f12,f15
	ctx.f12.f64 = double(float(std::fma(ctx.f16.f64, ctx.f12.f64, -ctx.f15.f64)));
	// lfs f15,-192(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -192);
	ctx.f15.f64 = double(temp.f32);
	// fmuls f11,f13,f11
	ctx.f11.f64 = double(float(ctx.f13.f64 * ctx.f11.f64));
	// fadds f18,f3,f2
	ctx.f18.f64 = double(float(ctx.f3.f64 + ctx.f2.f64));
	// fsubs f17,f15,f25
	ctx.f17.f64 = double(float(ctx.f15.f64 - ctx.f25.f64));
	// fmsubs f13,f13,f10,f19
	ctx.f13.f64 = double(float(std::fma(ctx.f13.f64, ctx.f10.f64, -ctx.f19.f64)));
	// fmadds f11,f9,f10,f11
	ctx.f11.f64 = double(float(std::fma(ctx.f9.f64, ctx.f10.f64, ctx.f11.f64)));
	// fadds f9,f6,f22
	ctx.f9.f64 = double(float(ctx.f6.f64 + ctx.f22.f64));
	// fadds f10,f13,f23
	ctx.f10.f64 = double(float(ctx.f13.f64 + ctx.f23.f64));
	// fsubs f13,f23,f13
	ctx.f13.f64 = double(float(ctx.f23.f64 - ctx.f13.f64));
	// fadds f23,f8,f14
	ctx.f23.f64 = double(float(ctx.f8.f64 + ctx.f14.f64));
	// fadds f19,f11,f12
	ctx.f19.f64 = double(float(ctx.f11.f64 + ctx.f12.f64));
	// fsubs f12,f12,f11
	ctx.f12.f64 = double(float(ctx.f12.f64 - ctx.f11.f64));
	// fadds f11,f31,f21
	ctx.f11.f64 = double(float(ctx.f31.f64 + ctx.f21.f64));
	// fsubs f8,f14,f8
	ctx.f8.f64 = double(float(ctx.f14.f64 - ctx.f8.f64));
	// fadds f16,f23,f9
	ctx.f16.f64 = double(float(ctx.f23.f64 + ctx.f9.f64));
	// stfs f16,0(r3)
	temp.f32 = float(ctx.f16.f64);
	REX_STORE_U32(ctx.r3.u32 + 0, temp.u32);
	// fsubs f9,f9,f23
	ctx.f9.f64 = double(float(ctx.f9.f64 - ctx.f23.f64));
	// stfs f9,8(r3)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r3.u32 + 8, temp.u32);
	// fadds f16,f7,f27
	ctx.f16.f64 = double(float(ctx.f7.f64 + ctx.f27.f64));
	// fadds f9,f18,f11
	ctx.f9.f64 = double(float(ctx.f18.f64 + ctx.f11.f64));
	// stfs f9,4(r3)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r3.u32 + 4, temp.u32);
	// fsubs f9,f21,f31
	ctx.f9.f64 = double(float(ctx.f21.f64 - ctx.f31.f64));
	// fsubs f11,f11,f18
	ctx.f11.f64 = double(float(ctx.f11.f64 - ctx.f18.f64));
	// stfs f11,12(r3)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r3.u32 + 12, temp.u32);
	// fsubs f11,f22,f6
	ctx.f11.f64 = double(float(ctx.f22.f64 - ctx.f6.f64));
	// fsubs f6,f2,f3
	ctx.f6.f64 = double(float(ctx.f2.f64 - ctx.f3.f64));
	// fsubs f7,f27,f7
	ctx.f7.f64 = double(float(ctx.f27.f64 - ctx.f7.f64));
	// fadds f3,f25,f15
	ctx.f3.f64 = double(float(ctx.f25.f64 + ctx.f15.f64));
	// fsubs f2,f17,f16
	ctx.f2.f64 = double(float(ctx.f17.f64 - ctx.f16.f64));
	// fadds f31,f16,f17
	ctx.f31.f64 = double(float(ctx.f16.f64 + ctx.f17.f64));
	// fadds f27,f8,f9
	ctx.f27.f64 = double(float(ctx.f8.f64 + ctx.f9.f64));
	// stfs f27,20(r3)
	temp.f32 = float(ctx.f27.f64);
	REX_STORE_U32(ctx.r3.u32 + 20, temp.u32);
	// fsubs f9,f9,f8
	ctx.f9.f64 = double(float(ctx.f9.f64 - ctx.f8.f64));
	// stfs f9,28(r3)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r3.u32 + 28, temp.u32);
	// fsubs f9,f11,f6
	ctx.f9.f64 = double(float(ctx.f11.f64 - ctx.f6.f64));
	// stfs f9,16(r3)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r3.u32 + 16, temp.u32);
	// fadds f11,f6,f11
	ctx.f11.f64 = double(float(ctx.f6.f64 + ctx.f11.f64));
	// stfs f11,24(r3)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r3.u32 + 24, temp.u32);
	// fsubs f11,f5,f26
	ctx.f11.f64 = double(float(ctx.f5.f64 - ctx.f26.f64));
	// fmuls f8,f2,f0
	ctx.f8.f64 = double(float(ctx.f2.f64 * ctx.f0.f64));
	// fadds f9,f1,f4
	ctx.f9.f64 = double(float(ctx.f1.f64 + ctx.f4.f64));
	// fmuls f6,f31,f0
	ctx.f6.f64 = double(float(ctx.f31.f64 * ctx.f0.f64));
	// lfs f31,-188(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -188);
	ctx.f31.f64 = double(temp.f32);
	// fsubs f2,f3,f7
	ctx.f2.f64 = double(float(ctx.f3.f64 - ctx.f7.f64));
	// fadds f27,f7,f3
	ctx.f27.f64 = double(float(ctx.f7.f64 + ctx.f3.f64));
	// fsubs f3,f24,f13
	ctx.f3.f64 = double(float(ctx.f24.f64 - ctx.f13.f64));
	// fadds f7,f12,f31
	ctx.f7.f64 = double(float(ctx.f12.f64 + ctx.f31.f64));
	// fadds f13,f13,f24
	ctx.f13.f64 = double(float(ctx.f13.f64 + ctx.f24.f64));
	// fsubs f12,f31,f12
	ctx.f12.f64 = double(float(ctx.f31.f64 - ctx.f12.f64));
	// fadds f25,f8,f11
	ctx.f25.f64 = double(float(ctx.f8.f64 + ctx.f11.f64));
	// stfs f25,32(r3)
	temp.f32 = float(ctx.f25.f64);
	REX_STORE_U32(ctx.r3.u32 + 32, temp.u32);
	// fsubs f11,f11,f8
	ctx.f11.f64 = double(float(ctx.f11.f64 - ctx.f8.f64));
	// stfs f11,40(r3)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r3.u32 + 40, temp.u32);
	// fadds f11,f6,f9
	ctx.f11.f64 = double(float(ctx.f6.f64 + ctx.f9.f64));
	// stfs f11,36(r3)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r3.u32 + 36, temp.u32);
	// fsubs f11,f9,f6
	ctx.f11.f64 = double(float(ctx.f9.f64 - ctx.f6.f64));
	// stfs f11,44(r3)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r3.u32 + 44, temp.u32);
	// fsubs f9,f4,f1
	ctx.f9.f64 = double(float(ctx.f4.f64 - ctx.f1.f64));
	// lfs f4,-180(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -180);
	ctx.f4.f64 = double(temp.f32);
	// fmuls f8,f2,f0
	ctx.f8.f64 = double(float(ctx.f2.f64 * ctx.f0.f64));
	// lfs f2,-176(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -176);
	ctx.f2.f64 = double(temp.f32);
	// fadds f11,f26,f5
	ctx.f11.f64 = double(float(ctx.f26.f64 + ctx.f5.f64));
	// lfs f1,-172(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -172);
	ctx.f1.f64 = double(temp.f32);
	// fmuls f6,f27,f0
	ctx.f6.f64 = double(float(ctx.f27.f64 * ctx.f0.f64));
	// fsubs f27,f7,f3
	ctx.f27.f64 = double(float(ctx.f7.f64 - ctx.f3.f64));
	// fadds f7,f3,f7
	ctx.f7.f64 = double(float(ctx.f3.f64 + ctx.f7.f64));
	// fadds f5,f8,f9
	ctx.f5.f64 = double(float(ctx.f8.f64 + ctx.f9.f64));
	// stfs f5,52(r3)
	temp.f32 = float(ctx.f5.f64);
	REX_STORE_U32(ctx.r3.u32 + 52, temp.u32);
	// fsubs f9,f9,f8
	ctx.f9.f64 = double(float(ctx.f9.f64 - ctx.f8.f64));
	// stfs f9,60(r3)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r3.u32 + 60, temp.u32);
	// fsubs f9,f11,f6
	ctx.f9.f64 = double(float(ctx.f11.f64 - ctx.f6.f64));
	// lfs f5,-184(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -184);
	ctx.f5.f64 = double(temp.f32);
	// fadds f11,f6,f11
	ctx.f11.f64 = double(float(ctx.f6.f64 + ctx.f11.f64));
	// stfs f11,56(r3)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r3.u32 + 56, temp.u32);
	// fsubs f8,f2,f10
	ctx.f8.f64 = double(float(ctx.f2.f64 - ctx.f10.f64));
	// stfs f9,48(r3)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r3.u32 + 48, temp.u32);
	// fadds f11,f5,f28
	ctx.f11.f64 = double(float(ctx.f5.f64 + ctx.f28.f64));
	// fadds f9,f4,f20
	ctx.f9.f64 = double(float(ctx.f4.f64 + ctx.f20.f64));
	// fsubs f6,f1,f19
	ctx.f6.f64 = double(float(ctx.f1.f64 - ctx.f19.f64));
	// fadds f10,f10,f2
	ctx.f10.f64 = double(float(ctx.f10.f64 + ctx.f2.f64));
	// fadds f26,f8,f11
	ctx.f26.f64 = double(float(ctx.f8.f64 + ctx.f11.f64));
	// stfs f26,64(r3)
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r3.u32 + 64, temp.u32);
	// fsubs f11,f11,f8
	ctx.f11.f64 = double(float(ctx.f11.f64 - ctx.f8.f64));
	// stfs f11,72(r3)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r3.u32 + 72, temp.u32);
	// fadds f11,f6,f9
	ctx.f11.f64 = double(float(ctx.f6.f64 + ctx.f9.f64));
	// stfs f11,68(r3)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r3.u32 + 68, temp.u32);
	// fsubs f11,f9,f6
	ctx.f11.f64 = double(float(ctx.f9.f64 - ctx.f6.f64));
	// stfs f11,76(r3)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r3.u32 + 76, temp.u32);
	// fsubs f9,f20,f4
	ctx.f9.f64 = double(float(ctx.f20.f64 - ctx.f4.f64));
	// fsubs f11,f28,f5
	ctx.f11.f64 = double(float(ctx.f28.f64 - ctx.f5.f64));
	// fadds f8,f19,f1
	ctx.f8.f64 = double(float(ctx.f19.f64 + ctx.f1.f64));
	// fadds f6,f10,f9
	ctx.f6.f64 = double(float(ctx.f10.f64 + ctx.f9.f64));
	// stfs f6,84(r3)
	temp.f32 = float(ctx.f6.f64);
	REX_STORE_U32(ctx.r3.u32 + 84, temp.u32);
	// fsubs f10,f9,f10
	ctx.f10.f64 = double(float(ctx.f9.f64 - ctx.f10.f64));
	// stfs f10,92(r3)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r3.u32 + 92, temp.u32);
	// fsubs f10,f11,f8
	ctx.f10.f64 = double(float(ctx.f11.f64 - ctx.f8.f64));
	// lfs f6,-168(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -168);
	ctx.f6.f64 = double(temp.f32);
	// fadds f11,f8,f11
	ctx.f11.f64 = double(float(ctx.f8.f64 + ctx.f11.f64));
	// stfs f11,88(r3)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r3.u32 + 88, temp.u32);
	// fsubs f11,f30,f6
	ctx.f11.f64 = double(float(ctx.f30.f64 - ctx.f6.f64));
	// lfs f8,-164(r1)
	temp.u32 = REX_LOAD_U32(ctx.r1.u32 + -164);
	ctx.f8.f64 = double(temp.f32);
	// fmuls f9,f27,f0
	ctx.f9.f64 = double(float(ctx.f27.f64 * ctx.f0.f64));
	// stfs f10,80(r3)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r3.u32 + 80, temp.u32);
	// fadds f10,f8,f29
	ctx.f10.f64 = double(float(ctx.f8.f64 + ctx.f29.f64));
	// fadds f5,f9,f11
	ctx.f5.f64 = double(float(ctx.f9.f64 + ctx.f11.f64));
	// stfs f5,96(r3)
	temp.f32 = float(ctx.f5.f64);
	REX_STORE_U32(ctx.r3.u32 + 96, temp.u32);
	// fsubs f11,f11,f9
	ctx.f11.f64 = double(float(ctx.f11.f64 - ctx.f9.f64));
	// stfs f11,104(r3)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r3.u32 + 104, temp.u32);
	// fmuls f9,f7,f0
	ctx.f9.f64 = double(float(ctx.f7.f64 * ctx.f0.f64));
	// fadds f11,f6,f30
	ctx.f11.f64 = double(float(ctx.f6.f64 + ctx.f30.f64));
	// fsubs f7,f12,f13
	ctx.f7.f64 = double(float(ctx.f12.f64 - ctx.f13.f64));
	// fadds f6,f13,f12
	ctx.f6.f64 = double(float(ctx.f13.f64 + ctx.f12.f64));
	// fadds f13,f9,f10
	ctx.f13.f64 = double(float(ctx.f9.f64 + ctx.f10.f64));
	// stfs f13,100(r3)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r3.u32 + 100, temp.u32);
	// fsubs f13,f10,f9
	ctx.f13.f64 = double(float(ctx.f10.f64 - ctx.f9.f64));
	// stfs f13,108(r3)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r3.u32 + 108, temp.u32);
	// fsubs f13,f29,f8
	ctx.f13.f64 = double(float(ctx.f29.f64 - ctx.f8.f64));
	// fmuls f12,f7,f0
	ctx.f12.f64 = double(float(ctx.f7.f64 * ctx.f0.f64));
	// fmuls f0,f6,f0
	ctx.f0.f64 = double(float(ctx.f6.f64 * ctx.f0.f64));
	// fadds f10,f12,f13
	ctx.f10.f64 = double(float(ctx.f12.f64 + ctx.f13.f64));
	// stfs f10,116(r3)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r3.u32 + 116, temp.u32);
	// fsubs f10,f11,f0
	ctx.f10.f64 = double(float(ctx.f11.f64 - ctx.f0.f64));
	// stfs f10,112(r3)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r3.u32 + 112, temp.u32);
	// fadds f0,f0,f11
	ctx.f0.f64 = double(float(ctx.f0.f64 + ctx.f11.f64));
	// stfs f0,120(r3)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r3.u32 + 120, temp.u32);
	// fsubs f0,f13,f12
	ctx.f0.f64 = double(float(ctx.f13.f64 - ctx.f12.f64));
	// stfs f0,124(r3)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r3.u32 + 124, temp.u32);
	// addi r12,r1,-8
	ctx.r12.s64 = ctx.r1.s64 + -8;
	// bl 0x82a0133c
	ctx.lr = 0x82A82AC0;
	__restfpr_14(ctx, base);
	// lwz r12,-8(r1)
	ctx.r12.u64 = REX_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

DEFINE_REX_FUNC(sub_82A82AD0) {
	REX_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	REX_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// addi r12,r1,-8
	ctx.r12.s64 = ctx.r1.s64 + -8;
	// bl 0x82a01318
	ctx.lr = 0x82A82AE0;
	__savefpr_24(ctx, base);
	// lfs f11,0(r3)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// lfs f10,4(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 4);
	ctx.f10.f64 = double(temp.f32);
	// lfs f13,32(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 32);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,36(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 36);
	ctx.f12.f64 = double(temp.f32);
	// fadds f28,f11,f13
	ctx.f28.f64 = double(float(ctx.f11.f64 + ctx.f13.f64));
	// lfs f7,52(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 52);
	ctx.f7.f64 = double(temp.f32);
	// fadds f27,f10,f12
	ctx.f27.f64 = double(float(ctx.f10.f64 + ctx.f12.f64));
	// lfs f9,48(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 48);
	ctx.f9.f64 = double(temp.f32);
	// fsubs f13,f11,f13
	ctx.f13.f64 = double(float(ctx.f11.f64 - ctx.f13.f64));
	// lfs f8,16(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 16);
	ctx.f8.f64 = double(temp.f32);
	// fsubs f12,f10,f12
	ctx.f12.f64 = double(float(ctx.f10.f64 - ctx.f12.f64));
	// lfs f6,20(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 20);
	ctx.f6.f64 = double(temp.f32);
	// fadds f11,f8,f9
	ctx.f11.f64 = double(float(ctx.f8.f64 + ctx.f9.f64));
	// fadds f10,f6,f7
	ctx.f10.f64 = double(float(ctx.f6.f64 + ctx.f7.f64));
	// lfs f5,40(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 40);
	ctx.f5.f64 = double(temp.f32);
	// fsubs f9,f8,f9
	ctx.f9.f64 = double(float(ctx.f8.f64 - ctx.f9.f64));
	// lfs f3,8(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 8);
	ctx.f3.f64 = double(temp.f32);
	// fsubs f8,f6,f7
	ctx.f8.f64 = double(float(ctx.f6.f64 - ctx.f7.f64));
	// lfs f4,44(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 44);
	ctx.f4.f64 = double(temp.f32);
	// lfs f2,12(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 12);
	ctx.f2.f64 = double(temp.f32);
	// lfs f1,56(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 56);
	ctx.f1.f64 = double(temp.f32);
	// fadds f26,f2,f4
	ctx.f26.f64 = double(float(ctx.f2.f64 + ctx.f4.f64));
	// lfs f30,60(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 60);
	ctx.f30.f64 = double(temp.f32);
	// lfs f31,24(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 24);
	ctx.f31.f64 = double(temp.f32);
	// lfs f29,28(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 28);
	ctx.f29.f64 = double(temp.f32);
	// fadds f25,f31,f1
	ctx.f25.f64 = double(float(ctx.f31.f64 + ctx.f1.f64));
	// fadds f24,f29,f30
	ctx.f24.f64 = double(float(ctx.f29.f64 + ctx.f30.f64));
	// lfs f0,4(r4)
	temp.u32 = REX_LOAD_U32(ctx.r4.u32 + 4);
	ctx.f0.f64 = double(temp.f32);
	// fadds f7,f11,f28
	ctx.f7.f64 = double(float(ctx.f11.f64 + ctx.f28.f64));
	// fadds f6,f10,f27
	ctx.f6.f64 = double(float(ctx.f10.f64 + ctx.f27.f64));
	// fsubs f11,f28,f11
	ctx.f11.f64 = double(float(ctx.f28.f64 - ctx.f11.f64));
	// fsubs f10,f27,f10
	ctx.f10.f64 = double(float(ctx.f27.f64 - ctx.f10.f64));
	// fsubs f28,f13,f8
	ctx.f28.f64 = double(float(ctx.f13.f64 - ctx.f8.f64));
	// fadds f27,f9,f12
	ctx.f27.f64 = double(float(ctx.f9.f64 + ctx.f12.f64));
	// fsubs f12,f12,f9
	ctx.f12.f64 = double(float(ctx.f12.f64 - ctx.f9.f64));
	// fadds f13,f8,f13
	ctx.f13.f64 = double(float(ctx.f8.f64 + ctx.f13.f64));
	// fadds f8,f3,f5
	ctx.f8.f64 = double(float(ctx.f3.f64 + ctx.f5.f64));
	// fsubs f9,f3,f5
	ctx.f9.f64 = double(float(ctx.f3.f64 - ctx.f5.f64));
	// fsubs f5,f2,f4
	ctx.f5.f64 = double(float(ctx.f2.f64 - ctx.f4.f64));
	// fsubs f3,f29,f30
	ctx.f3.f64 = double(float(ctx.f29.f64 - ctx.f30.f64));
	// fsubs f4,f31,f1
	ctx.f4.f64 = double(float(ctx.f31.f64 - ctx.f1.f64));
	// fadds f1,f24,f26
	ctx.f1.f64 = double(float(ctx.f24.f64 + ctx.f26.f64));
	// fsubs f31,f26,f24
	ctx.f31.f64 = double(float(ctx.f26.f64 - ctx.f24.f64));
	// fadds f2,f25,f8
	ctx.f2.f64 = double(float(ctx.f25.f64 + ctx.f8.f64));
	// fsubs f8,f8,f25
	ctx.f8.f64 = double(float(ctx.f8.f64 - ctx.f25.f64));
	// fsubs f29,f9,f3
	ctx.f29.f64 = double(float(ctx.f9.f64 - ctx.f3.f64));
	// fadds f30,f4,f5
	ctx.f30.f64 = double(float(ctx.f4.f64 + ctx.f5.f64));
	// fadds f9,f3,f9
	ctx.f9.f64 = double(float(ctx.f3.f64 + ctx.f9.f64));
	// fsubs f5,f5,f4
	ctx.f5.f64 = double(float(ctx.f5.f64 - ctx.f4.f64));
	// fsubs f4,f29,f30
	ctx.f4.f64 = double(float(ctx.f29.f64 - ctx.f30.f64));
	// fadds f3,f30,f29
	ctx.f3.f64 = double(float(ctx.f30.f64 + ctx.f29.f64));
	// fadds f30,f5,f9
	ctx.f30.f64 = double(float(ctx.f5.f64 + ctx.f9.f64));
	// fsubs f29,f9,f5
	ctx.f29.f64 = double(float(ctx.f9.f64 - ctx.f5.f64));
	// fmuls f9,f4,f0
	ctx.f9.f64 = double(float(ctx.f4.f64 * ctx.f0.f64));
	// fmuls f5,f3,f0
	ctx.f5.f64 = double(float(ctx.f3.f64 * ctx.f0.f64));
	// fmuls f4,f30,f0
	ctx.f4.f64 = double(float(ctx.f30.f64 * ctx.f0.f64));
	// fmuls f0,f29,f0
	ctx.f0.f64 = double(float(ctx.f29.f64 * ctx.f0.f64));
	// fadds f3,f9,f28
	ctx.f3.f64 = double(float(ctx.f9.f64 + ctx.f28.f64));
	// stfs f3,32(r3)
	temp.f32 = float(ctx.f3.f64);
	REX_STORE_U32(ctx.r3.u32 + 32, temp.u32);
	// fsubs f9,f28,f9
	ctx.f9.f64 = double(float(ctx.f28.f64 - ctx.f9.f64));
	// stfs f9,40(r3)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r3.u32 + 40, temp.u32);
	// fsubs f9,f27,f5
	ctx.f9.f64 = double(float(ctx.f27.f64 - ctx.f5.f64));
	// stfs f9,44(r3)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r3.u32 + 44, temp.u32);
	// fsubs f9,f13,f4
	ctx.f9.f64 = double(float(ctx.f13.f64 - ctx.f4.f64));
	// stfs f9,48(r3)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r3.u32 + 48, temp.u32);
	// fadds f13,f4,f13
	ctx.f13.f64 = double(float(ctx.f4.f64 + ctx.f13.f64));
	// stfs f13,56(r3)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r3.u32 + 56, temp.u32);
	// fadds f3,f5,f27
	ctx.f3.f64 = double(float(ctx.f5.f64 + ctx.f27.f64));
	// stfs f3,36(r3)
	temp.f32 = float(ctx.f3.f64);
	REX_STORE_U32(ctx.r3.u32 + 36, temp.u32);
	// fadds f5,f0,f12
	ctx.f5.f64 = double(float(ctx.f0.f64 + ctx.f12.f64));
	// stfs f5,52(r3)
	temp.f32 = float(ctx.f5.f64);
	REX_STORE_U32(ctx.r3.u32 + 52, temp.u32);
	// fsubs f0,f12,f0
	ctx.f0.f64 = double(float(ctx.f12.f64 - ctx.f0.f64));
	// stfs f0,60(r3)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r3.u32 + 60, temp.u32);
	// fadds f13,f2,f7
	ctx.f13.f64 = double(float(ctx.f2.f64 + ctx.f7.f64));
	// stfs f13,0(r3)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r3.u32 + 0, temp.u32);
	// fadds f0,f1,f6
	ctx.f0.f64 = double(float(ctx.f1.f64 + ctx.f6.f64));
	// stfs f0,4(r3)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r3.u32 + 4, temp.u32);
	// fsubs f13,f7,f2
	ctx.f13.f64 = double(float(ctx.f7.f64 - ctx.f2.f64));
	// stfs f13,8(r3)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r3.u32 + 8, temp.u32);
	// fsubs f0,f6,f1
	ctx.f0.f64 = double(float(ctx.f6.f64 - ctx.f1.f64));
	// fsubs f13,f11,f31
	ctx.f13.f64 = double(float(ctx.f11.f64 - ctx.f31.f64));
	// stfs f0,12(r3)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r3.u32 + 12, temp.u32);
	// fadds f0,f8,f10
	ctx.f0.f64 = double(float(ctx.f8.f64 + ctx.f10.f64));
	// stfs f13,16(r3)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r3.u32 + 16, temp.u32);
	// fadds f13,f31,f11
	ctx.f13.f64 = double(float(ctx.f31.f64 + ctx.f11.f64));
	// stfs f0,20(r3)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r3.u32 + 20, temp.u32);
	// fsubs f0,f10,f8
	ctx.f0.f64 = double(float(ctx.f10.f64 - ctx.f8.f64));
	// stfs f13,24(r3)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r3.u32 + 24, temp.u32);
	// stfs f0,28(r3)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r3.u32 + 28, temp.u32);
	// addi r12,r1,-8
	ctx.r12.s64 = ctx.r1.s64 + -8;
	// bl 0x82a01364
	ctx.lr = 0x82A82C4C;
	__restfpr_24(ctx, base);
	// lwz r12,-8(r1)
	ctx.r12.u64 = REX_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

DEFINE_REX_FUNC(sub_82A82C58) {
	REX_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	REX_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// addi r12,r1,-8
	ctx.r12.s64 = ctx.r1.s64 + -8;
	// bl 0x82a0130c
	ctx.lr = 0x82A82C68;
	__savefpr_21(ctx, base);
	// lfs f10,52(r3)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 52);
	ctx.f10.f64 = double(temp.f32);
	// lfs f9,48(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 48);
	ctx.f9.f64 = double(temp.f32);
	// lfs f8,20(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 20);
	ctx.f8.f64 = double(temp.f32);
	// lfs f11,16(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 16);
	ctx.f11.f64 = double(temp.f32);
	// fadds f25,f8,f9
	ctx.f25.f64 = double(float(ctx.f8.f64 + ctx.f9.f64));
	// fsubs f26,f11,f10
	ctx.f26.f64 = double(float(ctx.f11.f64 - ctx.f10.f64));
	// lfs f30,0(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 0);
	ctx.f30.f64 = double(temp.f32);
	// lfs f29,36(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 36);
	ctx.f29.f64 = double(temp.f32);
	// fadds f11,f10,f11
	ctx.f11.f64 = double(float(ctx.f10.f64 + ctx.f11.f64));
	// lfs f27,4(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 4);
	ctx.f27.f64 = double(temp.f32);
	// fsubs f24,f30,f29
	ctx.f24.f64 = double(float(ctx.f30.f64 - ctx.f29.f64));
	// lfs f28,32(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 32);
	ctx.f28.f64 = double(temp.f32);
	// fadds f30,f29,f30
	ctx.f30.f64 = double(float(ctx.f29.f64 + ctx.f30.f64));
	// fadds f29,f27,f28
	ctx.f29.f64 = double(float(ctx.f27.f64 + ctx.f28.f64));
	// lfs f12,4(r4)
	temp.u32 = REX_LOAD_U32(ctx.r4.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// fsubs f28,f27,f28
	ctx.f28.f64 = double(float(ctx.f27.f64 - ctx.f28.f64));
	// lfs f7,8(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 8);
	ctx.f7.f64 = double(temp.f32);
	// fsubs f10,f8,f9
	ctx.f10.f64 = double(float(ctx.f8.f64 - ctx.f9.f64));
	// lfs f4,12(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 12);
	ctx.f4.f64 = double(temp.f32);
	// lfs f6,44(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 44);
	ctx.f6.f64 = double(temp.f32);
	// lfs f5,40(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 40);
	ctx.f5.f64 = double(temp.f32);
	// lfs f0,16(r4)
	temp.u32 = REX_LOAD_U32(ctx.r4.u32 + 16);
	ctx.f0.f64 = double(temp.f32);
	// fsubs f27,f26,f25
	ctx.f27.f64 = double(float(ctx.f26.f64 - ctx.f25.f64));
	// lfs f13,20(r4)
	temp.u32 = REX_LOAD_U32(ctx.r4.u32 + 20);
	ctx.f13.f64 = double(temp.f32);
	// fadds f26,f25,f26
	ctx.f26.f64 = double(float(ctx.f25.f64 + ctx.f26.f64));
	// lfs f3,24(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 24);
	ctx.f3.f64 = double(temp.f32);
	// lfs f2,60(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 60);
	ctx.f2.f64 = double(temp.f32);
	// lfs f1,56(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 56);
	ctx.f1.f64 = double(temp.f32);
	// lfs f31,28(r3)
	temp.u32 = REX_LOAD_U32(ctx.r3.u32 + 28);
	ctx.f31.f64 = double(temp.f32);
	// fmuls f9,f27,f12
	ctx.f9.f64 = double(float(ctx.f27.f64 * ctx.f12.f64));
	// fsubs f27,f11,f10
	ctx.f27.f64 = double(float(ctx.f11.f64 - ctx.f10.f64));
	// fmuls f8,f26,f12
	ctx.f8.f64 = double(float(ctx.f26.f64 * ctx.f12.f64));
	// fadds f26,f10,f11
	ctx.f26.f64 = double(float(ctx.f10.f64 + ctx.f11.f64));
	// fsubs f11,f7,f6
	ctx.f11.f64 = double(float(ctx.f7.f64 - ctx.f6.f64));
	// fadds f10,f4,f5
	ctx.f10.f64 = double(float(ctx.f4.f64 + ctx.f5.f64));
	// fmuls f27,f27,f12
	ctx.f27.f64 = double(float(ctx.f27.f64 * ctx.f12.f64));
	// fmuls f12,f26,f12
	ctx.f12.f64 = double(float(ctx.f26.f64 * ctx.f12.f64));
	// fmr f26,f11
	ctx.f26.f64 = ctx.f11.f64;
	// fmr f25,f11
	ctx.f25.f64 = ctx.f11.f64;
	// fmuls f23,f10,f13
	ctx.f23.f64 = double(float(ctx.f10.f64 * ctx.f13.f64));
	// fmuls f22,f10,f0
	ctx.f22.f64 = double(float(ctx.f10.f64 * ctx.f0.f64));
	// fadds f11,f6,f7
	ctx.f11.f64 = double(float(ctx.f6.f64 + ctx.f7.f64));
	// fsubs f10,f4,f5
	ctx.f10.f64 = double(float(ctx.f4.f64 - ctx.f5.f64));
	// fmsubs f7,f26,f0,f23
	ctx.f7.f64 = double(float(std::fma(ctx.f26.f64, ctx.f0.f64, -ctx.f23.f64)));
	// fmadds f6,f25,f13,f22
	ctx.f6.f64 = double(float(std::fma(ctx.f25.f64, ctx.f13.f64, ctx.f22.f64)));
	// fmuls f4,f11,f0
	ctx.f4.f64 = double(float(ctx.f11.f64 * ctx.f0.f64));
	// fmr f5,f11
	ctx.f5.f64 = ctx.f11.f64;
	// fmuls f26,f10,f0
	ctx.f26.f64 = double(float(ctx.f10.f64 * ctx.f0.f64));
	// fmr f25,f10
	ctx.f25.f64 = ctx.f10.f64;
	// fsubs f11,f3,f2
	ctx.f11.f64 = double(float(ctx.f3.f64 - ctx.f2.f64));
	// fadds f10,f31,f1
	ctx.f10.f64 = double(float(ctx.f31.f64 + ctx.f1.f64));
	// fmsubs f5,f5,f13,f26
	ctx.f5.f64 = double(float(std::fma(ctx.f5.f64, ctx.f13.f64, -ctx.f26.f64)));
	// fmadds f4,f25,f13,f4
	ctx.f4.f64 = double(float(std::fma(ctx.f25.f64, ctx.f13.f64, ctx.f4.f64)));
	// fmr f26,f11
	ctx.f26.f64 = ctx.f11.f64;
	// fmuls f23,f10,f0
	ctx.f23.f64 = double(float(ctx.f10.f64 * ctx.f0.f64));
	// fmuls f25,f11,f0
	ctx.f25.f64 = double(float(ctx.f11.f64 * ctx.f0.f64));
	// fadds f11,f2,f3
	ctx.f11.f64 = double(float(ctx.f2.f64 + ctx.f3.f64));
	// fsubs f2,f31,f1
	ctx.f2.f64 = double(float(ctx.f31.f64 - ctx.f1.f64));
	// fmsubs f3,f26,f13,f23
	ctx.f3.f64 = double(float(std::fma(ctx.f26.f64, ctx.f13.f64, -ctx.f23.f64)));
	// fmadds f10,f10,f13,f25
	ctx.f10.f64 = double(float(std::fma(ctx.f10.f64, ctx.f13.f64, ctx.f25.f64)));
	// fmr f26,f11
	ctx.f26.f64 = ctx.f11.f64;
	// fmr f25,f11
	ctx.f25.f64 = ctx.f11.f64;
	// fadds f11,f9,f24
	ctx.f11.f64 = double(float(ctx.f9.f64 + ctx.f24.f64));
	// fmuls f23,f2,f13
	ctx.f23.f64 = double(float(ctx.f2.f64 * ctx.f13.f64));
	// fmuls f22,f2,f0
	ctx.f22.f64 = double(float(ctx.f2.f64 * ctx.f0.f64));
	// fadds f2,f8,f29
	ctx.f2.f64 = double(float(ctx.f8.f64 + ctx.f29.f64));
	// fsubs f8,f29,f8
	ctx.f8.f64 = double(float(ctx.f29.f64 - ctx.f8.f64));
	// fadds f1,f3,f7
	ctx.f1.f64 = double(float(ctx.f3.f64 + ctx.f7.f64));
	// fadds f31,f10,f6
	ctx.f31.f64 = double(float(ctx.f10.f64 + ctx.f6.f64));
	// fmsubs f0,f26,f0,f23
	ctx.f0.f64 = double(float(std::fma(ctx.f26.f64, ctx.f0.f64, -ctx.f23.f64)));
	// fmadds f13,f25,f13,f22
	ctx.f13.f64 = double(float(std::fma(ctx.f25.f64, ctx.f13.f64, ctx.f22.f64)));
	// fadds f21,f1,f11
	ctx.f21.f64 = double(float(ctx.f1.f64 + ctx.f11.f64));
	// stfs f21,0(r3)
	temp.f32 = float(ctx.f21.f64);
	REX_STORE_U32(ctx.r3.u32 + 0, temp.u32);
	// fsubs f11,f11,f1
	ctx.f11.f64 = double(float(ctx.f11.f64 - ctx.f1.f64));
	// stfs f11,8(r3)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r3.u32 + 8, temp.u32);
	// fsubs f11,f24,f9
	ctx.f11.f64 = double(float(ctx.f24.f64 - ctx.f9.f64));
	// fsubs f9,f7,f3
	ctx.f9.f64 = double(float(ctx.f7.f64 - ctx.f3.f64));
	// fadds f7,f31,f2
	ctx.f7.f64 = double(float(ctx.f31.f64 + ctx.f2.f64));
	// stfs f7,4(r3)
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r3.u32 + 4, temp.u32);
	// fsubs f7,f2,f31
	ctx.f7.f64 = double(float(ctx.f2.f64 - ctx.f31.f64));
	// stfs f7,12(r3)
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r3.u32 + 12, temp.u32);
	// fsubs f10,f6,f10
	ctx.f10.f64 = double(float(ctx.f6.f64 - ctx.f10.f64));
	// fadds f7,f9,f8
	ctx.f7.f64 = double(float(ctx.f9.f64 + ctx.f8.f64));
	// stfs f7,20(r3)
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r3.u32 + 20, temp.u32);
	// fsubs f7,f11,f10
	ctx.f7.f64 = double(float(ctx.f11.f64 - ctx.f10.f64));
	// stfs f7,16(r3)
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r3.u32 + 16, temp.u32);
	// fadds f11,f10,f11
	ctx.f11.f64 = double(float(ctx.f10.f64 + ctx.f11.f64));
	// stfs f11,24(r3)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r3.u32 + 24, temp.u32);
	// fsubs f11,f8,f9
	ctx.f11.f64 = double(float(ctx.f8.f64 - ctx.f9.f64));
	// stfs f11,28(r3)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r3.u32 + 28, temp.u32);
	// fsubs f9,f5,f0
	ctx.f9.f64 = double(float(ctx.f5.f64 - ctx.f0.f64));
	// fsubs f11,f30,f12
	ctx.f11.f64 = double(float(ctx.f30.f64 - ctx.f12.f64));
	// fsubs f8,f4,f13
	ctx.f8.f64 = double(float(ctx.f4.f64 - ctx.f13.f64));
	// fadds f10,f27,f28
	ctx.f10.f64 = double(float(ctx.f27.f64 + ctx.f28.f64));
	// fadds f0,f0,f5
	ctx.f0.f64 = double(float(ctx.f0.f64 + ctx.f5.f64));
	// fadds f12,f12,f30
	ctx.f12.f64 = double(float(ctx.f12.f64 + ctx.f30.f64));
	// fadds f13,f13,f4
	ctx.f13.f64 = double(float(ctx.f13.f64 + ctx.f4.f64));
	// fadds f7,f9,f11
	ctx.f7.f64 = double(float(ctx.f9.f64 + ctx.f11.f64));
	// stfs f7,32(r3)
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r3.u32 + 32, temp.u32);
	// fsubs f11,f11,f9
	ctx.f11.f64 = double(float(ctx.f11.f64 - ctx.f9.f64));
	// stfs f11,40(r3)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r3.u32 + 40, temp.u32);
	// fsubs f11,f10,f8
	ctx.f11.f64 = double(float(ctx.f10.f64 - ctx.f8.f64));
	// stfs f11,44(r3)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r3.u32 + 44, temp.u32);
	// fsubs f11,f28,f27
	ctx.f11.f64 = double(float(ctx.f28.f64 - ctx.f27.f64));
	// fadds f7,f8,f10
	ctx.f7.f64 = double(float(ctx.f8.f64 + ctx.f10.f64));
	// stfs f7,36(r3)
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r3.u32 + 36, temp.u32);
	// fadds f10,f0,f11
	ctx.f10.f64 = double(float(ctx.f0.f64 + ctx.f11.f64));
	// stfs f10,52(r3)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r3.u32 + 52, temp.u32);
	// fsubs f10,f12,f13
	ctx.f10.f64 = double(float(ctx.f12.f64 - ctx.f13.f64));
	// stfs f10,48(r3)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r3.u32 + 48, temp.u32);
	// fadds f13,f13,f12
	ctx.f13.f64 = double(float(ctx.f13.f64 + ctx.f12.f64));
	// stfs f13,56(r3)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r3.u32 + 56, temp.u32);
	// fsubs f0,f11,f0
	ctx.f0.f64 = double(float(ctx.f11.f64 - ctx.f0.f64));
	// stfs f0,60(r3)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r3.u32 + 60, temp.u32);
	// addi r12,r1,-8
	ctx.r12.s64 = ctx.r1.s64 + -8;
	// bl 0x82a01358
	ctx.lr = 0x82A82E38;
	__restfpr_21(ctx, base);
	// lwz r12,-8(r1)
	ctx.r12.u64 = REX_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

DEFINE_REX_FUNC(sub_82A82E48) {
	REX_FUNC_PROLOGUE();
	PPCRegister temp{};
	// srawi r10,r3,1
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0x1) != 0);
	ctx.r10.s64 = ctx.r3.s32 >> 1;
	// rlwinm r11,r5,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 1) & 0xFFFFFFFE;
	// mr r8,r6
	ctx.r8.u64 = ctx.r6.u64;
	// divw r11,r11,r10
	ctx.r11.u64 = uint32_t((ctx.r10.s32 && !(ctx.r11.s32 == INT32_MIN && ctx.r10.s32 == -1)) ? ctx.r11.s32 / ctx.r10.s32 : 0);
	// cmpwi cr6,r10,2
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 2, ctx.xer);
	// blelr cr6
	if (!ctx.cr6.gt) return;
	// addi r9,r3,-2
	ctx.r9.s64 = ctx.r3.s64 + -2;
	// neg r3,r11
	ctx.r3.s64 = static_cast<int64_t>(-ctx.r11.u64);
	// rlwinm r6,r9,2,0,29
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r9,r5,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r5,r11,2,0,29
	ctx.r5.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r11,r6,r4
	ctx.r11.u64 = ctx.r6.u64 + ctx.r4.u64;
	// addi r10,r10,-3
	ctx.r10.s64 = ctx.r10.s64 + -3;
	// lis r6,-32256
	ctx.r6.s64 = -2113929216;
	// rlwinm r7,r10,31,1,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// addi r10,r4,8
	ctx.r10.s64 = ctx.r4.s64 + 8;
	// rlwinm r3,r3,2,0,29
	ctx.r3.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// add r9,r9,r8
	ctx.r9.u64 = ctx.r9.u64 + ctx.r8.u64;
	// lfs f8,3444(r6)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r6.u32 + 3444);
	ctx.f8.f64 = double(temp.f32);
	// addi r7,r7,1
	ctx.r7.s64 = ctx.r7.s64 + 1;
loc_82A82E98:
	// add r9,r3,r9
	ctx.r9.u64 = ctx.r3.u64 + ctx.r9.u64;
	// lfs f13,4(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f9,4(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f9.f64 = double(temp.f32);
	// add r8,r5,r8
	ctx.r8.u64 = ctx.r5.u64 + ctx.r8.u64;
	// fadds f9,f9,f13
	ctx.f9.f64 = double(float(ctx.f9.f64 + ctx.f13.f64));
	// lfs f0,0(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f12,0(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f12.f64 = double(temp.f32);
	// addi r7,r7,-1
	ctx.r7.s64 = ctx.r7.s64 + -1;
	// fsubs f12,f0,f12
	ctx.f12.f64 = double(float(ctx.f0.f64 - ctx.f12.f64));
	// lfs f10,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f10.f64 = double(temp.f32);
	// cmplwi cr6,r7,0
	ctx.cr6.compare<uint32_t>(ctx.r7.u32, 0, ctx.xer);
	// fsubs f10,f8,f10
	ctx.f10.f64 = double(float(ctx.f8.f64 - ctx.f10.f64));
	// lfs f11,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// fmuls f7,f9,f11
	ctx.f7.f64 = double(float(ctx.f9.f64 * ctx.f11.f64));
	// fmuls f9,f9,f10
	ctx.f9.f64 = double(float(ctx.f9.f64 * ctx.f10.f64));
	// fmsubs f10,f12,f10,f7
	ctx.f10.f64 = double(float(std::fma(ctx.f12.f64, ctx.f10.f64, -ctx.f7.f64)));
	// fmadds f12,f12,f11,f9
	ctx.f12.f64 = double(float(std::fma(ctx.f12.f64, ctx.f11.f64, ctx.f9.f64)));
	// fsubs f0,f0,f10
	ctx.f0.f64 = double(float(ctx.f0.f64 - ctx.f10.f64));
	// stfs f0,0(r10)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// fsubs f0,f13,f12
	ctx.f0.f64 = double(float(ctx.f13.f64 - ctx.f12.f64));
	// stfs f0,4(r10)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// lfs f0,0(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// addi r10,r10,8
	ctx.r10.s64 = ctx.r10.s64 + 8;
	// lfs f13,4(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// fadds f0,f0,f10
	ctx.f0.f64 = double(float(ctx.f0.f64 + ctx.f10.f64));
	// fsubs f13,f13,f12
	ctx.f13.f64 = double(float(ctx.f13.f64 - ctx.f12.f64));
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f13,4(r11)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// addi r11,r11,-8
	ctx.r11.s64 = ctx.r11.s64 + -8;
	// bne cr6,0x82a82e98
	if (!ctx.cr6.eq) goto loc_82A82E98;
	// blr 
	return;
}

DEFINE_REX_FUNC(sub_82A82F18) {
	REX_FUNC_PROLOGUE();
	PPCRegister temp{};
	// srawi r10,r3,1
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0x1) != 0);
	ctx.r10.s64 = ctx.r3.s32 >> 1;
	// rlwinm r11,r5,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 1) & 0xFFFFFFFE;
	// mr r8,r6
	ctx.r8.u64 = ctx.r6.u64;
	// divw r11,r11,r10
	ctx.r11.u64 = uint32_t((ctx.r10.s32 && !(ctx.r11.s32 == INT32_MIN && ctx.r10.s32 == -1)) ? ctx.r11.s32 / ctx.r10.s32 : 0);
	// cmpwi cr6,r10,2
	ctx.cr6.compare<int32_t>(ctx.r10.s32, 2, ctx.xer);
	// blelr cr6
	if (!ctx.cr6.gt) return;
	// addi r9,r3,-2
	ctx.r9.s64 = ctx.r3.s64 + -2;
	// neg r3,r11
	ctx.r3.s64 = static_cast<int64_t>(-ctx.r11.u64);
	// rlwinm r6,r9,2,0,29
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r9,r5,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r5,r11,2,0,29
	ctx.r5.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r11,r6,r4
	ctx.r11.u64 = ctx.r6.u64 + ctx.r4.u64;
	// addi r10,r10,-3
	ctx.r10.s64 = ctx.r10.s64 + -3;
	// lis r6,-32256
	ctx.r6.s64 = -2113929216;
	// rlwinm r7,r10,31,1,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// addi r10,r4,8
	ctx.r10.s64 = ctx.r4.s64 + 8;
	// rlwinm r3,r3,2,0,29
	ctx.r3.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// add r9,r9,r8
	ctx.r9.u64 = ctx.r9.u64 + ctx.r8.u64;
	// lfs f8,3444(r6)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r6.u32 + 3444);
	ctx.f8.f64 = double(temp.f32);
	// addi r7,r7,1
	ctx.r7.s64 = ctx.r7.s64 + 1;
loc_82A82F68:
	// add r9,r3,r9
	ctx.r9.u64 = ctx.r3.u64 + ctx.r9.u64;
	// lfs f0,0(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f12,0(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f12.f64 = double(temp.f32);
	// add r8,r5,r8
	ctx.r8.u64 = ctx.r5.u64 + ctx.r8.u64;
	// fsubs f12,f0,f12
	ctx.f12.f64 = double(float(ctx.f0.f64 - ctx.f12.f64));
	// lfs f13,4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// lfs f9,4(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f9.f64 = double(temp.f32);
	// addi r7,r7,-1
	ctx.r7.s64 = ctx.r7.s64 + -1;
	// fadds f9,f9,f13
	ctx.f9.f64 = double(float(ctx.f9.f64 + ctx.f13.f64));
	// lfs f10,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f10.f64 = double(temp.f32);
	// cmplwi cr6,r7,0
	ctx.cr6.compare<uint32_t>(ctx.r7.u32, 0, ctx.xer);
	// fsubs f10,f8,f10
	ctx.f10.f64 = double(float(ctx.f8.f64 - ctx.f10.f64));
	// lfs f11,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// fmuls f6,f12,f11
	ctx.f6.f64 = double(float(ctx.f12.f64 * ctx.f11.f64));
	// fmuls f7,f12,f10
	ctx.f7.f64 = double(float(ctx.f12.f64 * ctx.f10.f64));
	// fmadds f12,f9,f11,f7
	ctx.f12.f64 = double(float(std::fma(ctx.f9.f64, ctx.f11.f64, ctx.f7.f64)));
	// fmsubs f11,f9,f10,f6
	ctx.f11.f64 = double(float(std::fma(ctx.f9.f64, ctx.f10.f64, -ctx.f6.f64)));
	// fsubs f0,f0,f12
	ctx.f0.f64 = double(float(ctx.f0.f64 - ctx.f12.f64));
	// stfs f0,0(r10)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// fsubs f0,f13,f11
	ctx.f0.f64 = double(float(ctx.f13.f64 - ctx.f11.f64));
	// stfs f0,4(r10)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// lfs f0,0(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// addi r10,r10,8
	ctx.r10.s64 = ctx.r10.s64 + 8;
	// lfs f13,4(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// fadds f0,f0,f12
	ctx.f0.f64 = double(float(ctx.f0.f64 + ctx.f12.f64));
	// fsubs f13,f13,f11
	ctx.f13.f64 = double(float(ctx.f13.f64 - ctx.f11.f64));
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f13,4(r11)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// addi r11,r11,-8
	ctx.r11.s64 = ctx.r11.s64 + -8;
	// bne cr6,0x82a82f68
	if (!ctx.cr6.eq) goto loc_82A82F68;
	// blr 
	return;
}

DEFINE_REX_FUNC(sub_82A82FE8) {
	REX_FUNC_PROLOGUE();
	PPCRegister temp{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c0
	ctx.lr = 0x82A82FF0;
	__savegprlr_26(ctx, base);
	// srawi r26,r3,1
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0x1) != 0);
	ctx.r26.s64 = ctx.r3.s32 >> 1;
	// divw r9,r5,r3
	ctx.r9.u64 = uint32_t((ctx.r3.s32 && !(ctx.r5.s32 == INT32_MIN && ctx.r3.s32 == -1)) ? ctx.r5.s32 / ctx.r3.s32 : 0);
	// addi r11,r26,-1
	ctx.r11.s64 = ctx.r26.s64 + -1;
	// li r28,0
	ctx.r28.s64 = 0;
	// li r27,1
	ctx.r27.s64 = 1;
	// cmpwi cr6,r11,4
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 4, ctx.xer);
	// blt cr6,0x82a8314c
	if (ctx.cr6.lt) goto loc_82A8314C;
	// addi r11,r26,-5
	ctx.r11.s64 = ctx.r26.s64 + -5;
	// addi r10,r3,-3
	ctx.r10.s64 = ctx.r3.s64 + -3;
	// rlwinm r11,r11,30,2,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 30) & 0x3FFFFFFF;
	// rlwinm r29,r5,2,0,29
	ctx.r29.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r31,r11,1
	ctx.r31.s64 = ctx.r11.s64 + 1;
	// neg r11,r9
	ctx.r11.s64 = static_cast<int64_t>(-ctx.r9.u64);
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r27,r31,2,0,29
	ctx.r27.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r7,r11,2,0,29
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r8,r9,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// mr r30,r6
	ctx.r30.u64 = ctx.r6.u64;
	// addi r11,r4,12
	ctx.r11.s64 = ctx.r4.s64 + 12;
	// add r29,r29,r6
	ctx.r29.u64 = ctx.r29.u64 + ctx.r6.u64;
	// add r10,r10,r4
	ctx.r10.u64 = ctx.r10.u64 + ctx.r4.u64;
	// addi r27,r27,1
	ctx.r27.s64 = ctx.r27.s64 + 1;
loc_82A83048:
	// add r29,r7,r29
	ctx.r29.u64 = ctx.r7.u64 + ctx.r29.u64;
	// lfs f0,8(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// add r30,r30,r8
	ctx.r30.u64 = ctx.r30.u64 + ctx.r8.u64;
	// lfs f13,-8(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -8);
	ctx.f13.f64 = double(temp.f32);
	// add r28,r28,r9
	ctx.r28.u64 = ctx.r28.u64 + ctx.r9.u64;
	// addi r31,r31,-1
	ctx.r31.s64 = ctx.r31.s64 + -1;
	// add r28,r28,r9
	ctx.r28.u64 = ctx.r28.u64 + ctx.r9.u64;
	// lfs f12,0(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 0);
	ctx.f12.f64 = double(temp.f32);
	// add r29,r7,r29
	ctx.r29.u64 = ctx.r7.u64 + ctx.r29.u64;
	// lfs f11,0(r30)
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// add r30,r30,r8
	ctx.r30.u64 = ctx.r30.u64 + ctx.r8.u64;
	// fsubs f10,f11,f12
	ctx.f10.f64 = double(float(ctx.f11.f64 - ctx.f12.f64));
	// add r28,r28,r9
	ctx.r28.u64 = ctx.r28.u64 + ctx.r9.u64;
	// fadds f12,f11,f12
	ctx.f12.f64 = double(float(ctx.f11.f64 + ctx.f12.f64));
	// cmplwi cr6,r31,0
	ctx.cr6.compare<uint32_t>(ctx.r31.u32, 0, ctx.xer);
	// add r28,r28,r9
	ctx.r28.u64 = ctx.r28.u64 + ctx.r9.u64;
	// fmuls f11,f0,f10
	ctx.f11.f64 = double(float(ctx.f0.f64 * ctx.f10.f64));
	// fmuls f9,f0,f12
	ctx.f9.f64 = double(float(ctx.f0.f64 * ctx.f12.f64));
	// fmsubs f0,f13,f12,f11
	ctx.f0.f64 = double(float(std::fma(ctx.f13.f64, ctx.f12.f64, -ctx.f11.f64)));
	// fmadds f13,f13,f10,f9
	ctx.f13.f64 = double(float(std::fma(ctx.f13.f64, ctx.f10.f64, ctx.f9.f64)));
	// stfs f13,-8(r11)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r11.u32 + -8, temp.u32);
	// stfs f0,8(r10)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r10.u32 + 8, temp.u32);
	// lfs f13,0(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// add r29,r7,r29
	ctx.r29.u64 = ctx.r7.u64 + ctx.r29.u64;
	// lfs f0,0(r30)
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// add r30,r30,r8
	ctx.r30.u64 = ctx.r30.u64 + ctx.r8.u64;
	// fsubs f10,f0,f13
	ctx.f10.f64 = double(float(ctx.f0.f64 - ctx.f13.f64));
	// lfs f12,4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// fadds f0,f0,f13
	ctx.f0.f64 = double(float(ctx.f0.f64 + ctx.f13.f64));
	// lfs f11,-4(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -4);
	ctx.f11.f64 = double(temp.f32);
	// fmuls f13,f12,f10
	ctx.f13.f64 = double(float(ctx.f12.f64 * ctx.f10.f64));
	// fmuls f12,f12,f0
	ctx.f12.f64 = double(float(ctx.f12.f64 * ctx.f0.f64));
	// fmsubs f0,f11,f0,f13
	ctx.f0.f64 = double(float(std::fma(ctx.f11.f64, ctx.f0.f64, -ctx.f13.f64)));
	// fmadds f13,f11,f10,f12
	ctx.f13.f64 = double(float(std::fma(ctx.f11.f64, ctx.f10.f64, ctx.f12.f64)));
	// stfs f13,-4(r11)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r11.u32 + -4, temp.u32);
	// stfs f0,4(r10)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r10.u32 + 4, temp.u32);
	// lfs f13,0(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// add r29,r7,r29
	ctx.r29.u64 = ctx.r7.u64 + ctx.r29.u64;
	// lfs f0,0(r30)
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// add r30,r30,r8
	ctx.r30.u64 = ctx.r30.u64 + ctx.r8.u64;
	// fsubs f10,f0,f13
	ctx.f10.f64 = double(float(ctx.f0.f64 - ctx.f13.f64));
	// lfs f11,0(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// lfs f12,0(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f12.f64 = double(temp.f32);
	// fadds f0,f0,f13
	ctx.f0.f64 = double(float(ctx.f0.f64 + ctx.f13.f64));
	// fmuls f13,f11,f10
	ctx.f13.f64 = double(float(ctx.f11.f64 * ctx.f10.f64));
	// fmuls f10,f12,f10
	ctx.f10.f64 = double(float(ctx.f12.f64 * ctx.f10.f64));
	// fmsubs f13,f12,f0,f13
	ctx.f13.f64 = double(float(std::fma(ctx.f12.f64, ctx.f0.f64, -ctx.f13.f64)));
	// fmadds f0,f11,f0,f10
	ctx.f0.f64 = double(float(std::fma(ctx.f11.f64, ctx.f0.f64, ctx.f10.f64)));
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f13,0(r10)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// lfs f13,0(r29)
	temp.u32 = REX_LOAD_U32(ctx.r29.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// lfs f0,0(r30)
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// fsubs f10,f0,f13
	ctx.f10.f64 = double(float(ctx.f0.f64 - ctx.f13.f64));
	// lfs f11,-4(r10)
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + -4);
	ctx.f11.f64 = double(temp.f32);
	// fadds f0,f0,f13
	ctx.f0.f64 = double(float(ctx.f0.f64 + ctx.f13.f64));
	// lfs f12,4(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// fmuls f13,f11,f10
	ctx.f13.f64 = double(float(ctx.f11.f64 * ctx.f10.f64));
	// fmsubs f13,f12,f0,f13
	ctx.f13.f64 = double(float(std::fma(ctx.f12.f64, ctx.f0.f64, -ctx.f13.f64)));
	// fmuls f0,f11,f0
	ctx.f0.f64 = double(float(ctx.f11.f64 * ctx.f0.f64));
	// fmadds f0,f12,f10,f0
	ctx.f0.f64 = double(float(std::fma(ctx.f12.f64, ctx.f10.f64, ctx.f0.f64)));
	// stfs f0,4(r11)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// stfs f13,-4(r10)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r10.u32 + -4, temp.u32);
	// addi r11,r11,16
	ctx.r11.s64 = ctx.r11.s64 + 16;
	// addi r10,r10,-16
	ctx.r10.s64 = ctx.r10.s64 + -16;
	// bne cr6,0x82a83048
	if (!ctx.cr6.eq) goto loc_82A83048;
loc_82A8314C:
	// cmpw cr6,r27,r26
	ctx.cr6.compare<int32_t>(ctx.r27.s32, ctx.r26.s32, ctx.xer);
	// bge cr6,0x82a831d8
	if (!ctx.cr6.lt) goto loc_82A831D8;
	// subf r7,r28,r5
	ctx.r7.u64 = ctx.r5.u64 - ctx.r28.u64;
	// subf r10,r27,r3
	ctx.r10.u64 = ctx.r3.u64 - ctx.r27.u64;
	// rlwinm r7,r7,2,0,29
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
	// neg r3,r9
	ctx.r3.s64 = static_cast<int64_t>(-ctx.r9.u64);
	// rlwinm r11,r27,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r27.u32 | (ctx.r27.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r8,r28,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r28.u32 | (ctx.r28.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r5,r9,2,0,29
	ctx.r5.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// add r9,r7,r6
	ctx.r9.u64 = ctx.r7.u64 + ctx.r6.u64;
	// add r11,r11,r4
	ctx.r11.u64 = ctx.r11.u64 + ctx.r4.u64;
	// add r8,r8,r6
	ctx.r8.u64 = ctx.r8.u64 + ctx.r6.u64;
	// rlwinm r3,r3,2,0,29
	ctx.r3.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 2) & 0xFFFFFFFC;
	// add r10,r10,r4
	ctx.r10.u64 = ctx.r10.u64 + ctx.r4.u64;
	// subf r7,r27,r26
	ctx.r7.u64 = ctx.r26.u64 - ctx.r27.u64;
loc_82A8318C:
	// add r9,r9,r3
	ctx.r9.u64 = ctx.r9.u64 + ctx.r3.u64;
	// lfs f13,0(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// add r8,r8,r5
	ctx.r8.u64 = ctx.r8.u64 + ctx.r5.u64;
	// lfs f0,0(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// addi r7,r7,-1
	ctx.r7.s64 = ctx.r7.s64 + -1;
	// cmplwi cr6,r7,0
	ctx.cr6.compare<uint32_t>(ctx.r7.u32, 0, ctx.xer);
	// lfs f12,0(r9)
	temp.u32 = REX_LOAD_U32(ctx.r9.u32 + 0);
	ctx.f12.f64 = double(temp.f32);
	// lfs f11,0(r8)
	temp.u32 = REX_LOAD_U32(ctx.r8.u32 + 0);
	ctx.f11.f64 = double(temp.f32);
	// fsubs f10,f11,f12
	ctx.f10.f64 = double(float(ctx.f11.f64 - ctx.f12.f64));
	// fadds f12,f12,f11
	ctx.f12.f64 = double(float(ctx.f12.f64 + ctx.f11.f64));
	// fmuls f11,f13,f10
	ctx.f11.f64 = double(float(ctx.f13.f64 * ctx.f10.f64));
	// fmuls f10,f0,f10
	ctx.f10.f64 = double(float(ctx.f0.f64 * ctx.f10.f64));
	// fmsubs f0,f0,f12,f11
	ctx.f0.f64 = double(float(std::fma(ctx.f0.f64, ctx.f12.f64, -ctx.f11.f64)));
	// fmadds f13,f13,f12,f10
	ctx.f13.f64 = double(float(std::fma(ctx.f13.f64, ctx.f12.f64, ctx.f10.f64)));
	// stfs f13,0(r11)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// stfs f0,0(r10)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r10.u32 + 0, temp.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// addi r10,r10,-4
	ctx.r10.s64 = ctx.r10.s64 + -4;
	// bne cr6,0x82a8318c
	if (!ctx.cr6.eq) goto loc_82A8318C;
loc_82A831D8:
	// rlwinm r11,r26,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r26.u32 | (ctx.r26.u64 << 32), 2) & 0xFFFFFFFC;
	// lfs f0,0(r6)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r6.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfsx f13,r11,r4
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + ctx.r4.u32);
	ctx.f13.f64 = double(temp.f32);
	// fmuls f0,f0,f13
	ctx.f0.f64 = double(float(ctx.f0.f64 * ctx.f13.f64));
	// stfsx f0,r11,r4
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r11.u32 + ctx.r4.u32, temp.u32);
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
}

DEFINE_REX_FUNC(sub_82A831F0) {
	REX_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12,-8(r1)
	REX_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1,-96(r1)
	ea = -96 + ctx.r1.u32;
	REX_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r10,r4
	ctx.r10.u64 = ctx.r4.u64;
	// cmpwi cr6,r3,128
	ctx.cr6.compare<int32_t>(ctx.r3.s32, 128, ctx.xer);
	// mr r3,r10
	ctx.r3.u64 = ctx.r10.u64;
	// bne cr6,0x82a83258
	if (!ctx.cr6.eq) goto loc_82A83258;
	// addi r11,r5,-8
	ctx.r11.s64 = ctx.r5.s64 + -8;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r9,r11,r6
	ctx.r9.u64 = ctx.r11.u64 + ctx.r6.u64;
	// mr r4,r9
	ctx.r4.u64 = ctx.r9.u64;
	// bl 0x82a82150
	ctx.lr = 0x82A83220;
	sub_82A82150(ctx, base);
	// addi r11,r5,-32
	ctx.r11.s64 = ctx.r5.s64 + -32;
	// addi r3,r10,128
	ctx.r3.s64 = ctx.r10.s64 + 128;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r4,r11,r6
	ctx.r4.u64 = ctx.r11.u64 + ctx.r6.u64;
	// bl 0x82a82570
	ctx.lr = 0x82A83234;
	sub_82A82570(ctx, base);
	// mr r4,r9
	ctx.r4.u64 = ctx.r9.u64;
	// addi r3,r10,256
	ctx.r3.s64 = ctx.r10.s64 + 256;
	// bl 0x82a82150
	ctx.lr = 0x82A83240;
	sub_82A82150(ctx, base);
	// addi r3,r10,384
	ctx.r3.s64 = ctx.r10.s64 + 384;
	// bl 0x82a82150
	ctx.lr = 0x82A83248;
	sub_82A82150(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = REX_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
loc_82A83258:
	// addi r11,r5,-16
	ctx.r11.s64 = ctx.r5.s64 + -16;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r4,r11,r6
	ctx.r4.u64 = ctx.r11.u64 + ctx.r6.u64;
	// bl 0x82a82ad0
	ctx.lr = 0x82A83268;
	sub_82A82AD0(ctx, base);
	// addi r3,r10,64
	ctx.r3.s64 = ctx.r10.s64 + 64;
	// bl 0x82a82c58
	ctx.lr = 0x82A83270;
	sub_82A82C58(ctx, base);
	// addi r3,r10,128
	ctx.r3.s64 = ctx.r10.s64 + 128;
	// bl 0x82a82ad0
	ctx.lr = 0x82A83278;
	sub_82A82AD0(ctx, base);
	// addi r3,r10,192
	ctx.r3.s64 = ctx.r10.s64 + 192;
	// bl 0x82a82ad0
	ctx.lr = 0x82A83280;
	sub_82A82AD0(ctx, base);
	// addi r1,r1,96
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12,-8(r1)
	ctx.r12.u64 = REX_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr 
	return;
}

DEFINE_REX_FUNC(sub_82A83290) {
	REX_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7a0
	ctx.lr = 0x82A83298;
	__savegprlr_18(ctx, base);
	// stwu r1,-208(r1)
	ea = -208 + ctx.r1.u32;
	REX_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r18,r3
	ctx.r18.u64 = ctx.r3.u64;
	// mr r19,r4
	ctx.r19.u64 = ctx.r4.u64;
	// srawi r31,r18,2
	ctx.xer.ca = (ctx.r18.s32 < 0) & ((ctx.r18.u32 & 0x3) != 0);
	ctx.r31.s64 = ctx.r18.s32 >> 2;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// cmpwi cr6,r31,128
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 128, ctx.xer);
	// ble cr6,0x82a8339c
	if (!ctx.cr6.gt) goto loc_82A8339C;
loc_82A832B8:
	// mr r23,r31
	ctx.r23.u64 = ctx.r31.u64;
	// cmpw cr6,r31,r18
	ctx.cr6.compare<int32_t>(ctx.r31.s32, ctx.r18.s32, ctx.xer);
	// bge cr6,0x82a8336c
	if (!ctx.cr6.lt) goto loc_82A8336C;
loc_82A832C4:
	// subf r30,r31,r23
	ctx.r30.u64 = ctx.r23.u64 - ctx.r31.u64;
	// cmpw cr6,r30,r18
	ctx.cr6.compare<int32_t>(ctx.r30.s32, ctx.r18.s32, ctx.xer);
	// bge cr6,0x82a83360
	if (!ctx.cr6.lt) goto loc_82A83360;
	// srawi r10,r31,1
	ctx.xer.ca = (ctx.r31.s32 < 0) & ((ctx.r31.u32 & 0x1) != 0);
	ctx.r10.s64 = ctx.r31.s32 >> 1;
	// rlwinm r11,r23,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r23.u32 | (ctx.r23.u64 << 32), 1) & 0xFFFFFFFE;
	// subf r10,r10,r29
	ctx.r10.u64 = ctx.r29.u64 - ctx.r10.u64;
	// subf r9,r31,r29
	ctx.r9.u64 = ctx.r29.u64 - ctx.r31.u64;
	// add r11,r11,r30
	ctx.r11.u64 = ctx.r11.u64 + ctx.r30.u64;
	// add r6,r30,r23
	ctx.r6.u64 = ctx.r30.u64 + ctx.r23.u64;
	// rlwinm r8,r10,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r7,r9,2,0,29
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r10,r11,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r9,r30,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r11,r6,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
	// add r21,r7,r28
	ctx.r21.u64 = ctx.r7.u64 + ctx.r28.u64;
	// add r22,r8,r28
	ctx.r22.u64 = ctx.r8.u64 + ctx.r28.u64;
	// rlwinm r20,r23,2,0,29
	ctx.r20.u64 = __builtin_rotateleft64(ctx.r23.u32 | (ctx.r23.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r24,r23,4,0,27
	ctx.r24.u64 = __builtin_rotateleft64(ctx.r23.u32 | (ctx.r23.u64 << 32), 4) & 0xFFFFFFF0;
	// add r27,r9,r19
	ctx.r27.u64 = ctx.r9.u64 + ctx.r19.u64;
	// add r25,r10,r19
	ctx.r25.u64 = ctx.r10.u64 + ctx.r19.u64;
	// add r26,r11,r19
	ctx.r26.u64 = ctx.r11.u64 + ctx.r19.u64;
loc_82A83318:
	// mr r5,r22
	ctx.r5.u64 = ctx.r22.u64;
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a81998
	ctx.lr = 0x82A83328;
	sub_82A81998(ctx, base);
	// mr r5,r21
	ctx.r5.u64 = ctx.r21.u64;
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a81d18
	ctx.lr = 0x82A83338;
	sub_82A81D18(ctx, base);
	// mr r5,r22
	ctx.r5.u64 = ctx.r22.u64;
	// mr r4,r25
	ctx.r4.u64 = ctx.r25.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a81998
	ctx.lr = 0x82A83348;
	sub_82A81998(ctx, base);
	// add r30,r20,r30
	ctx.r30.u64 = ctx.r20.u64 + ctx.r30.u64;
	// add r27,r24,r27
	ctx.r27.u64 = ctx.r24.u64 + ctx.r27.u64;
	// add r26,r24,r26
	ctx.r26.u64 = ctx.r24.u64 + ctx.r26.u64;
	// add r25,r24,r25
	ctx.r25.u64 = ctx.r24.u64 + ctx.r25.u64;
	// cmpw cr6,r30,r18
	ctx.cr6.compare<int32_t>(ctx.r30.s32, ctx.r18.s32, ctx.xer);
	// blt cr6,0x82a83318
	if (ctx.cr6.lt) goto loc_82A83318;
loc_82A83360:
	// rlwinm r23,r23,2,0,29
	ctx.r23.u64 = __builtin_rotateleft64(ctx.r23.u32 | (ctx.r23.u64 << 32), 2) & 0xFFFFFFFC;
	// cmpw cr6,r23,r18
	ctx.cr6.compare<int32_t>(ctx.r23.s32, ctx.r18.s32, ctx.xer);
	// blt cr6,0x82a832c4
	if (ctx.cr6.lt) goto loc_82A832C4;
loc_82A8336C:
	// subf r11,r31,r18
	ctx.r11.u64 = ctx.r18.u64 - ctx.r31.u64;
	// srawi r10,r31,1
	ctx.xer.ca = (ctx.r31.s32 < 0) & ((ctx.r31.u32 & 0x1) != 0);
	ctx.r10.s64 = ctx.r31.s32 >> 1;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// subf r10,r10,r29
	ctx.r10.u64 = ctx.r29.u64 - ctx.r10.u64;
	// add r4,r11,r19
	ctx.r4.u64 = ctx.r11.u64 + ctx.r19.u64;
	// rlwinm r11,r10,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// add r5,r11,r28
	ctx.r5.u64 = ctx.r11.u64 + ctx.r28.u64;
	// bl 0x82a81998
	ctx.lr = 0x82A83390;
	sub_82A81998(ctx, base);
	// srawi r31,r31,2
	ctx.xer.ca = (ctx.r31.s32 < 0) & ((ctx.r31.u32 & 0x3) != 0);
	ctx.r31.s64 = ctx.r31.s32 >> 2;
	// cmpwi cr6,r31,128
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 128, ctx.xer);
	// bgt cr6,0x82a832b8
	if (ctx.cr6.gt) goto loc_82A832B8;
loc_82A8339C:
	// mr r24,r31
	ctx.r24.u64 = ctx.r31.u64;
	// cmpw cr6,r31,r18
	ctx.cr6.compare<int32_t>(ctx.r31.s32, ctx.r18.s32, ctx.xer);
	// bge cr6,0x82a835c0
	if (!ctx.cr6.lt) goto loc_82A835C0;
loc_82A833A8:
	// subf r25,r31,r24
	ctx.r25.u64 = ctx.r24.u64 - ctx.r31.u64;
	// cmpw cr6,r25,r18
	ctx.cr6.compare<int32_t>(ctx.r25.s32, ctx.r18.s32, ctx.xer);
	// bge cr6,0x82a835b4
	if (!ctx.cr6.lt) goto loc_82A835B4;
	// addi r11,r24,48
	ctx.r11.s64 = ctx.r24.s64 + 48;
	// srawi r9,r31,1
	ctx.xer.ca = (ctx.r31.s32 < 0) & ((ctx.r31.u32 & 0x1) != 0);
	ctx.r9.s64 = ctx.r31.s32 >> 1;
	// rlwinm r10,r11,1,0,30
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// add r11,r25,r24
	ctx.r11.u64 = ctx.r25.u64 + ctx.r24.u64;
	// subf r8,r31,r29
	ctx.r8.u64 = ctx.r29.u64 - ctx.r31.u64;
	// subf r9,r9,r29
	ctx.r9.u64 = ctx.r29.u64 - ctx.r9.u64;
	// addi r6,r25,96
	ctx.r6.s64 = ctx.r25.s64 + 96;
	// add r10,r10,r25
	ctx.r10.u64 = ctx.r10.u64 + ctx.r25.u64;
	// addi r11,r11,96
	ctx.r11.s64 = ctx.r11.s64 + 96;
	// rlwinm r7,r8,2,0,29
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r8,r9,2,0,29
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r9,r6,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r10,r10,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r20,r7,r28
	ctx.r20.u64 = ctx.r7.u64 + ctx.r28.u64;
	// add r22,r8,r28
	ctx.r22.u64 = ctx.r8.u64 + ctx.r28.u64;
	// rlwinm r21,r24,2,0,29
	ctx.r21.u64 = __builtin_rotateleft64(ctx.r24.u32 | (ctx.r24.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r23,r24,4,0,27
	ctx.r23.u64 = __builtin_rotateleft64(ctx.r24.u32 | (ctx.r24.u64 << 32), 4) & 0xFFFFFFF0;
	// add r27,r9,r19
	ctx.r27.u64 = ctx.r9.u64 + ctx.r19.u64;
	// add r26,r10,r19
	ctx.r26.u64 = ctx.r10.u64 + ctx.r19.u64;
	// add r30,r11,r19
	ctx.r30.u64 = ctx.r11.u64 + ctx.r19.u64;
loc_82A83408:
	// addi r4,r27,-384
	ctx.r4.s64 = ctx.r27.s64 + -384;
	// mr r5,r22
	ctx.r5.u64 = ctx.r22.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a81998
	ctx.lr = 0x82A83418;
	sub_82A81998(ctx, base);
	// cmpwi cr6,r31,128
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 128, ctx.xer);
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bne cr6,0x82a83464
	if (!ctx.cr6.eq) goto loc_82A83464;
	// addi r11,r29,-8
	ctx.r11.s64 = ctx.r29.s64 + -8;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r10,r11,r28
	ctx.r10.u64 = ctx.r11.u64 + ctx.r28.u64;
	// mr r4,r10
	ctx.r4.u64 = ctx.r10.u64;
	// bl 0x82a82150
	ctx.lr = 0x82A83438;
	sub_82A82150(ctx, base);
	// addi r11,r29,-32
	ctx.r11.s64 = ctx.r29.s64 + -32;
	// addi r3,r27,-256
	ctx.r3.s64 = ctx.r27.s64 + -256;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r4,r11,r28
	ctx.r4.u64 = ctx.r11.u64 + ctx.r28.u64;
	// bl 0x82a82570
	ctx.lr = 0x82A8344C;
	sub_82A82570(ctx, base);
	// mr r4,r10
	ctx.r4.u64 = ctx.r10.u64;
	// addi r3,r27,-128
	ctx.r3.s64 = ctx.r27.s64 + -128;
	// bl 0x82a82150
	ctx.lr = 0x82A83458;
	sub_82A82150(ctx, base);
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// bl 0x82a82150
	ctx.lr = 0x82A83460;
	sub_82A82150(ctx, base);
	// b 0x82a8348c
	goto loc_82A8348C;
loc_82A83464:
	// addi r11,r29,-16
	ctx.r11.s64 = ctx.r29.s64 + -16;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r4,r11,r28
	ctx.r4.u64 = ctx.r11.u64 + ctx.r28.u64;
	// bl 0x82a82ad0
	ctx.lr = 0x82A83474;
	sub_82A82AD0(ctx, base);
	// addi r3,r27,-320
	ctx.r3.s64 = ctx.r27.s64 + -320;
	// bl 0x82a82c58
	ctx.lr = 0x82A8347C;
	sub_82A82C58(ctx, base);
	// addi r3,r27,-256
	ctx.r3.s64 = ctx.r27.s64 + -256;
	// bl 0x82a82ad0
	ctx.lr = 0x82A83484;
	sub_82A82AD0(ctx, base);
	// addi r3,r27,-192
	ctx.r3.s64 = ctx.r27.s64 + -192;
	// bl 0x82a82ad0
	ctx.lr = 0x82A8348C;
	sub_82A82AD0(ctx, base);
loc_82A8348C:
	// addi r4,r30,-384
	ctx.r4.s64 = ctx.r30.s64 + -384;
	// mr r5,r20
	ctx.r5.u64 = ctx.r20.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a81d18
	ctx.lr = 0x82A8349C;
	sub_82A81D18(ctx, base);
	// cmpwi cr6,r31,128
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 128, ctx.xer);
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bne cr6,0x82a834f0
	if (!ctx.cr6.eq) goto loc_82A834F0;
	// addi r11,r29,-8
	ctx.r11.s64 = ctx.r29.s64 + -8;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r10,r11,r28
	ctx.r10.u64 = ctx.r11.u64 + ctx.r28.u64;
	// mr r4,r10
	ctx.r4.u64 = ctx.r10.u64;
	// bl 0x82a82150
	ctx.lr = 0x82A834BC;
	sub_82A82150(ctx, base);
	// addi r11,r29,-32
	ctx.r11.s64 = ctx.r29.s64 + -32;
	// addi r3,r30,-256
	ctx.r3.s64 = ctx.r30.s64 + -256;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r9,r11,r28
	ctx.r9.u64 = ctx.r11.u64 + ctx.r28.u64;
	// mr r4,r9
	ctx.r4.u64 = ctx.r9.u64;
	// bl 0x82a82570
	ctx.lr = 0x82A834D4;
	sub_82A82570(ctx, base);
	// mr r4,r10
	ctx.r4.u64 = ctx.r10.u64;
	// addi r3,r30,-128
	ctx.r3.s64 = ctx.r30.s64 + -128;
	// bl 0x82a82150
	ctx.lr = 0x82A834E0;
	sub_82A82150(ctx, base);
	// mr r4,r9
	ctx.r4.u64 = ctx.r9.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82a82570
	ctx.lr = 0x82A834EC;
	sub_82A82570(ctx, base);
	// b 0x82a83518
	goto loc_82A83518;
loc_82A834F0:
	// addi r11,r29,-16
	ctx.r11.s64 = ctx.r29.s64 + -16;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r4,r11,r28
	ctx.r4.u64 = ctx.r11.u64 + ctx.r28.u64;
	// bl 0x82a82ad0
	ctx.lr = 0x82A83500;
	sub_82A82AD0(ctx, base);
	// addi r3,r30,-320
	ctx.r3.s64 = ctx.r30.s64 + -320;
	// bl 0x82a82c58
	ctx.lr = 0x82A83508;
	sub_82A82C58(ctx, base);
	// addi r3,r30,-256
	ctx.r3.s64 = ctx.r30.s64 + -256;
	// bl 0x82a82ad0
	ctx.lr = 0x82A83510;
	sub_82A82AD0(ctx, base);
	// addi r3,r30,-192
	ctx.r3.s64 = ctx.r30.s64 + -192;
	// bl 0x82a82c58
	ctx.lr = 0x82A83518;
	sub_82A82C58(ctx, base);
loc_82A83518:
	// addi r4,r26,-384
	ctx.r4.s64 = ctx.r26.s64 + -384;
	// mr r5,r22
	ctx.r5.u64 = ctx.r22.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a81998
	ctx.lr = 0x82A83528;
	sub_82A81998(ctx, base);
	// cmpwi cr6,r31,128
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 128, ctx.xer);
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bne cr6,0x82a83574
	if (!ctx.cr6.eq) goto loc_82A83574;
	// addi r11,r29,-8
	ctx.r11.s64 = ctx.r29.s64 + -8;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r10,r11,r28
	ctx.r10.u64 = ctx.r11.u64 + ctx.r28.u64;
	// mr r4,r10
	ctx.r4.u64 = ctx.r10.u64;
	// bl 0x82a82150
	ctx.lr = 0x82A83548;
	sub_82A82150(ctx, base);
	// addi r11,r29,-32
	ctx.r11.s64 = ctx.r29.s64 + -32;
	// addi r3,r26,-256
	ctx.r3.s64 = ctx.r26.s64 + -256;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r4,r11,r28
	ctx.r4.u64 = ctx.r11.u64 + ctx.r28.u64;
	// bl 0x82a82570
	ctx.lr = 0x82A8355C;
	sub_82A82570(ctx, base);
	// mr r4,r10
	ctx.r4.u64 = ctx.r10.u64;
	// addi r3,r26,-128
	ctx.r3.s64 = ctx.r26.s64 + -128;
	// bl 0x82a82150
	ctx.lr = 0x82A83568;
	sub_82A82150(ctx, base);
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// bl 0x82a82150
	ctx.lr = 0x82A83570;
	sub_82A82150(ctx, base);
	// b 0x82a8359c
	goto loc_82A8359C;
loc_82A83574:
	// addi r11,r29,-16
	ctx.r11.s64 = ctx.r29.s64 + -16;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r4,r11,r28
	ctx.r4.u64 = ctx.r11.u64 + ctx.r28.u64;
	// bl 0x82a82ad0
	ctx.lr = 0x82A83584;
	sub_82A82AD0(ctx, base);
	// addi r3,r26,-320
	ctx.r3.s64 = ctx.r26.s64 + -320;
	// bl 0x82a82c58
	ctx.lr = 0x82A8358C;
	sub_82A82C58(ctx, base);
	// addi r3,r26,-256
	ctx.r3.s64 = ctx.r26.s64 + -256;
	// bl 0x82a82ad0
	ctx.lr = 0x82A83594;
	sub_82A82AD0(ctx, base);
	// addi r3,r26,-192
	ctx.r3.s64 = ctx.r26.s64 + -192;
	// bl 0x82a82ad0
	ctx.lr = 0x82A8359C;
	sub_82A82AD0(ctx, base);
loc_82A8359C:
	// add r25,r21,r25
	ctx.r25.u64 = ctx.r21.u64 + ctx.r25.u64;
	// add r27,r27,r23
	ctx.r27.u64 = ctx.r27.u64 + ctx.r23.u64;
	// add r30,r30,r23
	ctx.r30.u64 = ctx.r30.u64 + ctx.r23.u64;
	// add r26,r26,r23
	ctx.r26.u64 = ctx.r26.u64 + ctx.r23.u64;
	// cmpw cr6,r25,r18
	ctx.cr6.compare<int32_t>(ctx.r25.s32, ctx.r18.s32, ctx.xer);
	// blt cr6,0x82a83408
	if (ctx.cr6.lt) goto loc_82A83408;
loc_82A835B4:
	// rlwinm r24,r24,2,0,29
	ctx.r24.u64 = __builtin_rotateleft64(ctx.r24.u32 | (ctx.r24.u64 << 32), 2) & 0xFFFFFFFC;
	// cmpw cr6,r24,r18
	ctx.cr6.compare<int32_t>(ctx.r24.s32, ctx.r18.s32, ctx.xer);
	// blt cr6,0x82a833a8
	if (ctx.cr6.lt) goto loc_82A833A8;
loc_82A835C0:
	// subf r11,r31,r18
	ctx.r11.u64 = ctx.r18.u64 - ctx.r31.u64;
	// srawi r10,r31,1
	ctx.xer.ca = (ctx.r31.s32 < 0) & ((ctx.r31.u32 & 0x1) != 0);
	ctx.r10.s64 = ctx.r31.s32 >> 1;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// add r30,r11,r19
	ctx.r30.u64 = ctx.r11.u64 + ctx.r19.u64;
	// subf r11,r10,r29
	ctx.r11.u64 = ctx.r29.u64 - ctx.r10.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r5,r11,r28
	ctx.r5.u64 = ctx.r11.u64 + ctx.r28.u64;
	// bl 0x82a81998
	ctx.lr = 0x82A835E8;
	sub_82A81998(ctx, base);
	// cmpwi cr6,r31,128
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 128, ctx.xer);
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bne cr6,0x82a83638
	if (!ctx.cr6.eq) goto loc_82A83638;
	// addi r11,r29,-8
	ctx.r11.s64 = ctx.r29.s64 + -8;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r10,r11,r28
	ctx.r10.u64 = ctx.r11.u64 + ctx.r28.u64;
	// mr r4,r10
	ctx.r4.u64 = ctx.r10.u64;
	// bl 0x82a82150
	ctx.lr = 0x82A83608;
	sub_82A82150(ctx, base);
	// addi r11,r29,-32
	ctx.r11.s64 = ctx.r29.s64 + -32;
	// addi r3,r30,128
	ctx.r3.s64 = ctx.r30.s64 + 128;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r4,r11,r28
	ctx.r4.u64 = ctx.r11.u64 + ctx.r28.u64;
	// bl 0x82a82570
	ctx.lr = 0x82A8361C;
	sub_82A82570(ctx, base);
	// mr r4,r10
	ctx.r4.u64 = ctx.r10.u64;
	// addi r3,r30,256
	ctx.r3.s64 = ctx.r30.s64 + 256;
	// bl 0x82a82150
	ctx.lr = 0x82A83628;
	sub_82A82150(ctx, base);
	// addi r3,r30,384
	ctx.r3.s64 = ctx.r30.s64 + 384;
	// bl 0x82a82150
	ctx.lr = 0x82A83630;
	sub_82A82150(ctx, base);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x829ff7f0
	__restgprlr_18(ctx, base);
	return;
loc_82A83638:
	// addi r11,r29,-16
	ctx.r11.s64 = ctx.r29.s64 + -16;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r4,r11,r28
	ctx.r4.u64 = ctx.r11.u64 + ctx.r28.u64;
	// bl 0x82a82ad0
	ctx.lr = 0x82A83648;
	sub_82A82AD0(ctx, base);
	// addi r3,r30,64
	ctx.r3.s64 = ctx.r30.s64 + 64;
	// bl 0x82a82c58
	ctx.lr = 0x82A83650;
	sub_82A82C58(ctx, base);
	// addi r3,r30,128
	ctx.r3.s64 = ctx.r30.s64 + 128;
	// bl 0x82a82ad0
	ctx.lr = 0x82A83658;
	sub_82A82AD0(ctx, base);
	// addi r3,r30,192
	ctx.r3.s64 = ctx.r30.s64 + 192;
	// bl 0x82a82ad0
	ctx.lr = 0x82A83660;
	sub_82A82AD0(ctx, base);
	// addi r1,r1,208
	ctx.r1.s64 = ctx.r1.s64 + 208;
	// b 0x829ff7f0
	__restgprlr_18(ctx, base);
	return;
}

DEFINE_REX_FUNC(sub_82A83668) {
	REX_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7a8
	ctx.lr = 0x82A83670;
	__savegprlr_20(ctx, base);
	// stwu r1,-192(r1)
	ea = -192 + ctx.r1.u32;
	REX_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// srawi r22,r3,1
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0x1) != 0);
	ctx.r22.s64 = ctx.r3.s32 >> 1;
	// srawi r31,r3,2
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0x3) != 0);
	ctx.r31.s64 = ctx.r3.s32 >> 2;
	// mr r20,r4
	ctx.r20.u64 = ctx.r4.u64;
	// mr r27,r5
	ctx.r27.u64 = ctx.r5.u64;
	// mr r26,r6
	ctx.r26.u64 = ctx.r6.u64;
	// cmpwi cr6,r31,128
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 128, ctx.xer);
	// ble cr6,0x82a83784
	if (!ctx.cr6.gt) goto loc_82A83784;
loc_82A83690:
	// mr r23,r31
	ctx.r23.u64 = ctx.r31.u64;
	// cmpw cr6,r31,r22
	ctx.cr6.compare<int32_t>(ctx.r31.s32, ctx.r22.s32, ctx.xer);
	// bge cr6,0x82a83778
	if (!ctx.cr6.lt) goto loc_82A83778;
loc_82A8369C:
	// subf r30,r31,r23
	ctx.r30.u64 = ctx.r23.u64 - ctx.r31.u64;
	// cmpw cr6,r30,r22
	ctx.cr6.compare<int32_t>(ctx.r30.s32, ctx.r22.s32, ctx.xer);
	// bge cr6,0x82a83708
	if (!ctx.cr6.lt) goto loc_82A83708;
	// srawi r9,r31,1
	ctx.xer.ca = (ctx.r31.s32 < 0) & ((ctx.r31.u32 & 0x1) != 0);
	ctx.r9.s64 = ctx.r31.s32 >> 1;
	// add r11,r30,r22
	ctx.r11.u64 = ctx.r30.u64 + ctx.r22.u64;
	// subf r9,r9,r27
	ctx.r9.u64 = ctx.r27.u64 - ctx.r9.u64;
	// rlwinm r10,r30,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r9,r9,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r21,r23,1,0,30
	ctx.r21.u64 = __builtin_rotateleft64(ctx.r23.u32 | (ctx.r23.u64 << 32), 1) & 0xFFFFFFFE;
	// add r25,r9,r26
	ctx.r25.u64 = ctx.r9.u64 + ctx.r26.u64;
	// rlwinm r24,r23,3,0,28
	ctx.r24.u64 = __builtin_rotateleft64(ctx.r23.u32 | (ctx.r23.u64 << 32), 3) & 0xFFFFFFF8;
	// add r29,r10,r20
	ctx.r29.u64 = ctx.r10.u64 + ctx.r20.u64;
	// add r28,r11,r20
	ctx.r28.u64 = ctx.r11.u64 + ctx.r20.u64;
loc_82A836D4:
	// mr r5,r25
	ctx.r5.u64 = ctx.r25.u64;
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a81998
	ctx.lr = 0x82A836E4;
	sub_82A81998(ctx, base);
	// mr r5,r25
	ctx.r5.u64 = ctx.r25.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a81998
	ctx.lr = 0x82A836F4;
	sub_82A81998(ctx, base);
	// add r30,r21,r30
	ctx.r30.u64 = ctx.r21.u64 + ctx.r30.u64;
	// add r29,r24,r29
	ctx.r29.u64 = ctx.r24.u64 + ctx.r29.u64;
	// add r28,r24,r28
	ctx.r28.u64 = ctx.r24.u64 + ctx.r28.u64;
	// cmpw cr6,r30,r22
	ctx.cr6.compare<int32_t>(ctx.r30.s32, ctx.r22.s32, ctx.xer);
	// blt cr6,0x82a836d4
	if (ctx.cr6.lt) goto loc_82A836D4;
loc_82A83708:
	// rlwinm r11,r23,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r23.u32 | (ctx.r23.u64 << 32), 1) & 0xFFFFFFFE;
	// subf r30,r31,r11
	ctx.r30.u64 = ctx.r11.u64 - ctx.r31.u64;
	// cmpw cr6,r30,r22
	ctx.cr6.compare<int32_t>(ctx.r30.s32, ctx.r22.s32, ctx.xer);
	// bge cr6,0x82a8376c
	if (!ctx.cr6.lt) goto loc_82A8376C;
	// subf r11,r31,r27
	ctx.r11.u64 = ctx.r27.u64 - ctx.r31.u64;
	// add r8,r30,r22
	ctx.r8.u64 = ctx.r30.u64 + ctx.r22.u64;
	// rlwinm r9,r11,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r10,r30,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r11,r8,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 2) & 0xFFFFFFFC;
	// add r5,r9,r26
	ctx.r5.u64 = ctx.r9.u64 + ctx.r26.u64;
	// rlwinm r24,r23,2,0,29
	ctx.r24.u64 = __builtin_rotateleft64(ctx.r23.u32 | (ctx.r23.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r25,r23,4,0,27
	ctx.r25.u64 = __builtin_rotateleft64(ctx.r23.u32 | (ctx.r23.u64 << 32), 4) & 0xFFFFFFF0;
	// add r29,r10,r20
	ctx.r29.u64 = ctx.r10.u64 + ctx.r20.u64;
	// add r28,r11,r20
	ctx.r28.u64 = ctx.r11.u64 + ctx.r20.u64;
loc_82A83740:
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a81d18
	ctx.lr = 0x82A8374C;
	sub_82A81D18(ctx, base);
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a81d18
	ctx.lr = 0x82A83758;
	sub_82A81D18(ctx, base);
	// add r30,r24,r30
	ctx.r30.u64 = ctx.r24.u64 + ctx.r30.u64;
	// add r29,r25,r29
	ctx.r29.u64 = ctx.r25.u64 + ctx.r29.u64;
	// add r28,r25,r28
	ctx.r28.u64 = ctx.r25.u64 + ctx.r28.u64;
	// cmpw cr6,r30,r22
	ctx.cr6.compare<int32_t>(ctx.r30.s32, ctx.r22.s32, ctx.xer);
	// blt cr6,0x82a83740
	if (ctx.cr6.lt) goto loc_82A83740;
loc_82A8376C:
	// rlwinm r23,r23,2,0,29
	ctx.r23.u64 = __builtin_rotateleft64(ctx.r23.u32 | (ctx.r23.u64 << 32), 2) & 0xFFFFFFFC;
	// cmpw cr6,r23,r22
	ctx.cr6.compare<int32_t>(ctx.r23.s32, ctx.r22.s32, ctx.xer);
	// blt cr6,0x82a8369c
	if (ctx.cr6.lt) goto loc_82A8369C;
loc_82A83778:
	// srawi r31,r31,2
	ctx.xer.ca = (ctx.r31.s32 < 0) & ((ctx.r31.u32 & 0x3) != 0);
	ctx.r31.s64 = ctx.r31.s32 >> 2;
	// cmpwi cr6,r31,128
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 128, ctx.xer);
	// bgt cr6,0x82a83690
	if (ctx.cr6.gt) goto loc_82A83690;
loc_82A83784:
	// mr r21,r31
	ctx.r21.u64 = ctx.r31.u64;
	// cmpw cr6,r31,r22
	ctx.cr6.compare<int32_t>(ctx.r31.s32, ctx.r22.s32, ctx.xer);
	// bge cr6,0x82a83a5c
	if (!ctx.cr6.lt) goto loc_82A83A5C;
loc_82A83790:
	// subf r28,r31,r21
	ctx.r28.u64 = ctx.r21.u64 - ctx.r31.u64;
	// cmpw cr6,r28,r22
	ctx.cr6.compare<int32_t>(ctx.r28.s32, ctx.r22.s32, ctx.xer);
	// bge cr6,0x82a838ec
	if (!ctx.cr6.lt) goto loc_82A838EC;
	// srawi r10,r31,1
	ctx.xer.ca = (ctx.r31.s32 < 0) & ((ctx.r31.u32 & 0x1) != 0);
	ctx.r10.s64 = ctx.r31.s32 >> 1;
	// add r11,r28,r22
	ctx.r11.u64 = ctx.r28.u64 + ctx.r22.u64;
	// subf r10,r10,r27
	ctx.r10.u64 = ctx.r27.u64 - ctx.r10.u64;
	// addi r8,r28,96
	ctx.r8.s64 = ctx.r28.s64 + 96;
	// addi r11,r11,96
	ctx.r11.s64 = ctx.r11.s64 + 96;
	// rlwinm r9,r10,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r10,r8,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r24,r9,r26
	ctx.r24.u64 = ctx.r9.u64 + ctx.r26.u64;
	// rlwinm r23,r21,1,0,30
	ctx.r23.u64 = __builtin_rotateleft64(ctx.r21.u32 | (ctx.r21.u64 << 32), 1) & 0xFFFFFFFE;
	// rlwinm r25,r21,3,0,28
	ctx.r25.u64 = __builtin_rotateleft64(ctx.r21.u32 | (ctx.r21.u64 << 32), 3) & 0xFFFFFFF8;
	// add r30,r10,r20
	ctx.r30.u64 = ctx.r10.u64 + ctx.r20.u64;
	// add r29,r11,r20
	ctx.r29.u64 = ctx.r11.u64 + ctx.r20.u64;
loc_82A837D0:
	// addi r4,r30,-384
	ctx.r4.s64 = ctx.r30.s64 + -384;
	// mr r5,r24
	ctx.r5.u64 = ctx.r24.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a81998
	ctx.lr = 0x82A837E0;
	sub_82A81998(ctx, base);
	// cmpwi cr6,r31,128
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 128, ctx.xer);
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bne cr6,0x82a8382c
	if (!ctx.cr6.eq) goto loc_82A8382C;
	// addi r11,r27,-8
	ctx.r11.s64 = ctx.r27.s64 + -8;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r10,r11,r26
	ctx.r10.u64 = ctx.r11.u64 + ctx.r26.u64;
	// mr r4,r10
	ctx.r4.u64 = ctx.r10.u64;
	// bl 0x82a82150
	ctx.lr = 0x82A83800;
	sub_82A82150(ctx, base);
	// addi r11,r27,-32
	ctx.r11.s64 = ctx.r27.s64 + -32;
	// addi r3,r30,-256
	ctx.r3.s64 = ctx.r30.s64 + -256;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r4,r11,r26
	ctx.r4.u64 = ctx.r11.u64 + ctx.r26.u64;
	// bl 0x82a82570
	ctx.lr = 0x82A83814;
	sub_82A82570(ctx, base);
	// mr r4,r10
	ctx.r4.u64 = ctx.r10.u64;
	// addi r3,r30,-128
	ctx.r3.s64 = ctx.r30.s64 + -128;
	// bl 0x82a82150
	ctx.lr = 0x82A83820;
	sub_82A82150(ctx, base);
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82a82150
	ctx.lr = 0x82A83828;
	sub_82A82150(ctx, base);
	// b 0x82a83854
	goto loc_82A83854;
loc_82A8382C:
	// addi r11,r27,-16
	ctx.r11.s64 = ctx.r27.s64 + -16;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r4,r11,r26
	ctx.r4.u64 = ctx.r11.u64 + ctx.r26.u64;
	// bl 0x82a82ad0
	ctx.lr = 0x82A8383C;
	sub_82A82AD0(ctx, base);
	// addi r3,r30,-320
	ctx.r3.s64 = ctx.r30.s64 + -320;
	// bl 0x82a82c58
	ctx.lr = 0x82A83844;
	sub_82A82C58(ctx, base);
	// addi r3,r30,-256
	ctx.r3.s64 = ctx.r30.s64 + -256;
	// bl 0x82a82ad0
	ctx.lr = 0x82A8384C;
	sub_82A82AD0(ctx, base);
	// addi r3,r30,-192
	ctx.r3.s64 = ctx.r30.s64 + -192;
	// bl 0x82a82ad0
	ctx.lr = 0x82A83854;
	sub_82A82AD0(ctx, base);
loc_82A83854:
	// addi r4,r29,-384
	ctx.r4.s64 = ctx.r29.s64 + -384;
	// mr r5,r24
	ctx.r5.u64 = ctx.r24.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a81998
	ctx.lr = 0x82A83864;
	sub_82A81998(ctx, base);
	// cmpwi cr6,r31,128
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 128, ctx.xer);
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bne cr6,0x82a838b0
	if (!ctx.cr6.eq) goto loc_82A838B0;
	// addi r11,r27,-8
	ctx.r11.s64 = ctx.r27.s64 + -8;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r10,r11,r26
	ctx.r10.u64 = ctx.r11.u64 + ctx.r26.u64;
	// mr r4,r10
	ctx.r4.u64 = ctx.r10.u64;
	// bl 0x82a82150
	ctx.lr = 0x82A83884;
	sub_82A82150(ctx, base);
	// addi r11,r27,-32
	ctx.r11.s64 = ctx.r27.s64 + -32;
	// addi r3,r29,-256
	ctx.r3.s64 = ctx.r29.s64 + -256;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r4,r11,r26
	ctx.r4.u64 = ctx.r11.u64 + ctx.r26.u64;
	// bl 0x82a82570
	ctx.lr = 0x82A83898;
	sub_82A82570(ctx, base);
	// mr r4,r10
	ctx.r4.u64 = ctx.r10.u64;
	// addi r3,r29,-128
	ctx.r3.s64 = ctx.r29.s64 + -128;
	// bl 0x82a82150
	ctx.lr = 0x82A838A4;
	sub_82A82150(ctx, base);
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x82a82150
	ctx.lr = 0x82A838AC;
	sub_82A82150(ctx, base);
	// b 0x82a838d8
	goto loc_82A838D8;
loc_82A838B0:
	// addi r11,r27,-16
	ctx.r11.s64 = ctx.r27.s64 + -16;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r4,r11,r26
	ctx.r4.u64 = ctx.r11.u64 + ctx.r26.u64;
	// bl 0x82a82ad0
	ctx.lr = 0x82A838C0;
	sub_82A82AD0(ctx, base);
	// addi r3,r29,-320
	ctx.r3.s64 = ctx.r29.s64 + -320;
	// bl 0x82a82c58
	ctx.lr = 0x82A838C8;
	sub_82A82C58(ctx, base);
	// addi r3,r29,-256
	ctx.r3.s64 = ctx.r29.s64 + -256;
	// bl 0x82a82ad0
	ctx.lr = 0x82A838D0;
	sub_82A82AD0(ctx, base);
	// addi r3,r29,-192
	ctx.r3.s64 = ctx.r29.s64 + -192;
	// bl 0x82a82ad0
	ctx.lr = 0x82A838D8;
	sub_82A82AD0(ctx, base);
loc_82A838D8:
	// add r28,r23,r28
	ctx.r28.u64 = ctx.r23.u64 + ctx.r28.u64;
	// add r30,r30,r25
	ctx.r30.u64 = ctx.r30.u64 + ctx.r25.u64;
	// add r29,r29,r25
	ctx.r29.u64 = ctx.r29.u64 + ctx.r25.u64;
	// cmpw cr6,r28,r22
	ctx.cr6.compare<int32_t>(ctx.r28.s32, ctx.r22.s32, ctx.xer);
	// blt cr6,0x82a837d0
	if (ctx.cr6.lt) goto loc_82A837D0;
loc_82A838EC:
	// rlwinm r11,r21,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r21.u32 | (ctx.r21.u64 << 32), 1) & 0xFFFFFFFE;
	// subf r28,r31,r11
	ctx.r28.u64 = ctx.r11.u64 - ctx.r31.u64;
	// cmpw cr6,r28,r22
	ctx.cr6.compare<int32_t>(ctx.r28.s32, ctx.r22.s32, ctx.xer);
	// bge cr6,0x82a83a50
	if (!ctx.cr6.lt) goto loc_82A83A50;
	// add r11,r28,r22
	ctx.r11.u64 = ctx.r28.u64 + ctx.r22.u64;
	// subf r10,r31,r27
	ctx.r10.u64 = ctx.r27.u64 - ctx.r31.u64;
	// addi r8,r28,96
	ctx.r8.s64 = ctx.r28.s64 + 96;
	// addi r11,r11,96
	ctx.r11.s64 = ctx.r11.s64 + 96;
	// rlwinm r9,r10,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r10,r8,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r5,r9,r26
	ctx.r5.u64 = ctx.r9.u64 + ctx.r26.u64;
	// rlwinm r24,r21,2,0,29
	ctx.r24.u64 = __builtin_rotateleft64(ctx.r21.u32 | (ctx.r21.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r25,r21,4,0,27
	ctx.r25.u64 = __builtin_rotateleft64(ctx.r21.u32 | (ctx.r21.u64 << 32), 4) & 0xFFFFFFF0;
	// add r30,r10,r20
	ctx.r30.u64 = ctx.r10.u64 + ctx.r20.u64;
	// add r29,r11,r20
	ctx.r29.u64 = ctx.r11.u64 + ctx.r20.u64;
loc_82A8392C:
	// addi r4,r30,-384
	ctx.r4.s64 = ctx.r30.s64 + -384;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a81d18
	ctx.lr = 0x82A83938;
	sub_82A81D18(ctx, base);
	// cmpwi cr6,r31,128
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 128, ctx.xer);
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bne cr6,0x82a8398c
	if (!ctx.cr6.eq) goto loc_82A8398C;
	// addi r11,r27,-8
	ctx.r11.s64 = ctx.r27.s64 + -8;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r10,r11,r26
	ctx.r10.u64 = ctx.r11.u64 + ctx.r26.u64;
	// mr r4,r10
	ctx.r4.u64 = ctx.r10.u64;
	// bl 0x82a82150
	ctx.lr = 0x82A83958;
	sub_82A82150(ctx, base);
	// addi r11,r27,-32
	ctx.r11.s64 = ctx.r27.s64 + -32;
	// addi r3,r30,-256
	ctx.r3.s64 = ctx.r30.s64 + -256;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r9,r11,r26
	ctx.r9.u64 = ctx.r11.u64 + ctx.r26.u64;
	// mr r4,r9
	ctx.r4.u64 = ctx.r9.u64;
	// bl 0x82a82570
	ctx.lr = 0x82A83970;
	sub_82A82570(ctx, base);
	// mr r4,r10
	ctx.r4.u64 = ctx.r10.u64;
	// addi r3,r30,-128
	ctx.r3.s64 = ctx.r30.s64 + -128;
	// bl 0x82a82150
	ctx.lr = 0x82A8397C;
	sub_82A82150(ctx, base);
	// mr r4,r9
	ctx.r4.u64 = ctx.r9.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82a82570
	ctx.lr = 0x82A83988;
	sub_82A82570(ctx, base);
	// b 0x82a839b4
	goto loc_82A839B4;
loc_82A8398C:
	// addi r11,r27,-16
	ctx.r11.s64 = ctx.r27.s64 + -16;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r4,r11,r26
	ctx.r4.u64 = ctx.r11.u64 + ctx.r26.u64;
	// bl 0x82a82ad0
	ctx.lr = 0x82A8399C;
	sub_82A82AD0(ctx, base);
	// addi r3,r30,-320
	ctx.r3.s64 = ctx.r30.s64 + -320;
	// bl 0x82a82c58
	ctx.lr = 0x82A839A4;
	sub_82A82C58(ctx, base);
	// addi r3,r30,-256
	ctx.r3.s64 = ctx.r30.s64 + -256;
	// bl 0x82a82ad0
	ctx.lr = 0x82A839AC;
	sub_82A82AD0(ctx, base);
	// addi r3,r30,-192
	ctx.r3.s64 = ctx.r30.s64 + -192;
	// bl 0x82a82c58
	ctx.lr = 0x82A839B4;
	sub_82A82C58(ctx, base);
loc_82A839B4:
	// addi r4,r29,-384
	ctx.r4.s64 = ctx.r29.s64 + -384;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a81d18
	ctx.lr = 0x82A839C0;
	sub_82A81D18(ctx, base);
	// cmpwi cr6,r31,128
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 128, ctx.xer);
	// mr r3,r4
	ctx.r3.u64 = ctx.r4.u64;
	// bne cr6,0x82a83a14
	if (!ctx.cr6.eq) goto loc_82A83A14;
	// addi r11,r27,-8
	ctx.r11.s64 = ctx.r27.s64 + -8;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r10,r11,r26
	ctx.r10.u64 = ctx.r11.u64 + ctx.r26.u64;
	// mr r4,r10
	ctx.r4.u64 = ctx.r10.u64;
	// bl 0x82a82150
	ctx.lr = 0x82A839E0;
	sub_82A82150(ctx, base);
	// addi r11,r27,-32
	ctx.r11.s64 = ctx.r27.s64 + -32;
	// addi r3,r29,-256
	ctx.r3.s64 = ctx.r29.s64 + -256;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r9,r11,r26
	ctx.r9.u64 = ctx.r11.u64 + ctx.r26.u64;
	// mr r4,r9
	ctx.r4.u64 = ctx.r9.u64;
	// bl 0x82a82570
	ctx.lr = 0x82A839F8;
	sub_82A82570(ctx, base);
	// mr r4,r10
	ctx.r4.u64 = ctx.r10.u64;
	// addi r3,r29,-128
	ctx.r3.s64 = ctx.r29.s64 + -128;
	// bl 0x82a82150
	ctx.lr = 0x82A83A04;
	sub_82A82150(ctx, base);
	// mr r4,r9
	ctx.r4.u64 = ctx.r9.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x82a82570
	ctx.lr = 0x82A83A10;
	sub_82A82570(ctx, base);
	// b 0x82a83a3c
	goto loc_82A83A3C;
loc_82A83A14:
	// addi r11,r27,-16
	ctx.r11.s64 = ctx.r27.s64 + -16;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r4,r11,r26
	ctx.r4.u64 = ctx.r11.u64 + ctx.r26.u64;
	// bl 0x82a82ad0
	ctx.lr = 0x82A83A24;
	sub_82A82AD0(ctx, base);
	// addi r3,r29,-320
	ctx.r3.s64 = ctx.r29.s64 + -320;
	// bl 0x82a82c58
	ctx.lr = 0x82A83A2C;
	sub_82A82C58(ctx, base);
	// addi r3,r29,-256
	ctx.r3.s64 = ctx.r29.s64 + -256;
	// bl 0x82a82ad0
	ctx.lr = 0x82A83A34;
	sub_82A82AD0(ctx, base);
	// addi r3,r29,-192
	ctx.r3.s64 = ctx.r29.s64 + -192;
	// bl 0x82a82c58
	ctx.lr = 0x82A83A3C;
	sub_82A82C58(ctx, base);
loc_82A83A3C:
	// add r28,r24,r28
	ctx.r28.u64 = ctx.r24.u64 + ctx.r28.u64;
	// add r30,r25,r30
	ctx.r30.u64 = ctx.r25.u64 + ctx.r30.u64;
	// add r29,r25,r29
	ctx.r29.u64 = ctx.r25.u64 + ctx.r29.u64;
	// cmpw cr6,r28,r22
	ctx.cr6.compare<int32_t>(ctx.r28.s32, ctx.r22.s32, ctx.xer);
	// blt cr6,0x82a8392c
	if (ctx.cr6.lt) goto loc_82A8392C;
loc_82A83A50:
	// rlwinm r21,r21,2,0,29
	ctx.r21.u64 = __builtin_rotateleft64(ctx.r21.u32 | (ctx.r21.u64 << 32), 2) & 0xFFFFFFFC;
	// cmpw cr6,r21,r22
	ctx.cr6.compare<int32_t>(ctx.r21.s32, ctx.r22.s32, ctx.xer);
	// blt cr6,0x82a83790
	if (ctx.cr6.lt) goto loc_82A83790;
loc_82A83A5C:
	// addi r1,r1,192
	ctx.r1.s64 = ctx.r1.s64 + 192;
	// b 0x829ff7f8
	__restgprlr_20(ctx, base);
	return;
}

DEFINE_REX_FUNC(sub_82A83A68) {
	REX_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c4
	ctx.lr = 0x82A83A70;
	__savegprlr_27(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	REX_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// mr r29,r5
	ctx.r29.u64 = ctx.r5.u64;
	// srawi r31,r27,2
	ctx.xer.ca = (ctx.r27.s32 < 0) & ((ctx.r27.u32 & 0x3) != 0);
	ctx.r31.s64 = ctx.r27.s32 >> 2;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// rlwinm r11,r31,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 1) & 0xFFFFFFFE;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// subf r11,r11,r29
	ctx.r11.u64 = ctx.r29.u64 - ctx.r11.u64;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r5,r11,r28
	ctx.r5.u64 = ctx.r11.u64 + ctx.r28.u64;
	// bl 0x82a81998
	ctx.lr = 0x82A83A9C;
	sub_82A81998(ctx, base);
	// cmpwi cr6,r27,512
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 512, ctx.xer);
	// ble cr6,0x82a83b28
	if (!ctx.cr6.gt) goto loc_82A83B28;
loc_82A83AA4:
	// mr r6,r28
	ctx.r6.u64 = ctx.r28.u64;
	// mr r5,r29
	ctx.r5.u64 = ctx.r29.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a83a68
	ctx.lr = 0x82A83AB8;
	sub_82A83A68(ctx, base);
	// rlwinm r11,r31,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 2) & 0xFFFFFFFC;
	// mr r6,r28
	ctx.r6.u64 = ctx.r28.u64;
	// mr r5,r29
	ctx.r5.u64 = ctx.r29.u64;
	// add r4,r11,r30
	ctx.r4.u64 = ctx.r11.u64 + ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a83b48
	ctx.lr = 0x82A83AD0;
	sub_82A83B48(ctx, base);
	// rlwinm r11,r31,3,0,28
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 3) & 0xFFFFFFF8;
	// mr r6,r28
	ctx.r6.u64 = ctx.r28.u64;
	// mr r5,r29
	ctx.r5.u64 = ctx.r29.u64;
	// add r4,r11,r30
	ctx.r4.u64 = ctx.r11.u64 + ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a83a68
	ctx.lr = 0x82A83AE8;
	sub_82A83A68(ctx, base);
	// mr r11,r31
	ctx.r11.u64 = ctx.r31.u64;
	// mr r27,r31
	ctx.r27.u64 = ctx.r31.u64;
	// rlwinm r10,r11,1,0,30
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// srawi r31,r31,2
	ctx.xer.ca = (ctx.r31.s32 < 0) & ((ctx.r31.u32 & 0x3) != 0);
	ctx.r31.s64 = ctx.r31.s32 >> 2;
	// add r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
	// rlwinm r10,r31,1,0,30
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 1) & 0xFFFFFFFE;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// subf r10,r10,r29
	ctx.r10.u64 = ctx.r29.u64 - ctx.r10.u64;
	// add r30,r11,r30
	ctx.r30.u64 = ctx.r11.u64 + ctx.r30.u64;
	// rlwinm r11,r10,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// add r5,r11,r28
	ctx.r5.u64 = ctx.r11.u64 + ctx.r28.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// bl 0x82a81998
	ctx.lr = 0x82A83B20;
	sub_82A81998(ctx, base);
	// cmpwi cr6,r27,512
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 512, ctx.xer);
	// bgt cr6,0x82a83aa4
	if (ctx.cr6.gt) goto loc_82A83AA4;
loc_82A83B28:
	// mr r6,r28
	ctx.r6.u64 = ctx.r28.u64;
	// mr r5,r29
	ctx.r5.u64 = ctx.r29.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// bl 0x82a83290
	ctx.lr = 0x82A83B3C;
	sub_82A83290(ctx, base);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x829ff814
	__restgprlr_27(ctx, base);
	return;
}

DEFINE_REX_FUNC(sub_82A83B48) {
	REX_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c4
	ctx.lr = 0x82A83B50;
	__savegprlr_27(ctx, base);
	// stwu r1,-128(r1)
	ea = -128 + ctx.r1.u32;
	REX_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r29,r3
	ctx.r29.u64 = ctx.r3.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// subf r11,r29,r28
	ctx.r11.u64 = ctx.r28.u64 - ctx.r29.u64;
	// mr r30,r4
	ctx.r30.u64 = ctx.r4.u64;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// srawi r31,r29,2
	ctx.xer.ca = (ctx.r29.s32 < 0) & ((ctx.r29.u32 & 0x3) != 0);
	ctx.r31.s64 = ctx.r29.s32 >> 2;
	// add r5,r11,r27
	ctx.r5.u64 = ctx.r11.u64 + ctx.r27.u64;
	// bl 0x82a81d18
	ctx.lr = 0x82A83B78;
	sub_82A81D18(ctx, base);
	// cmpwi cr6,r29,512
	ctx.cr6.compare<int32_t>(ctx.r29.s32, 512, ctx.xer);
	// ble cr6,0x82a83c00
	if (!ctx.cr6.gt) goto loc_82A83C00;
loc_82A83B80:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a83a68
	ctx.lr = 0x82A83B94;
	sub_82A83A68(ctx, base);
	// rlwinm r11,r31,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 2) & 0xFFFFFFFC;
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// add r4,r11,r30
	ctx.r4.u64 = ctx.r11.u64 + ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a83b48
	ctx.lr = 0x82A83BAC;
	sub_82A83B48(ctx, base);
	// rlwinm r11,r31,3,0,28
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 3) & 0xFFFFFFF8;
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// add r4,r11,r30
	ctx.r4.u64 = ctx.r11.u64 + ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a83a68
	ctx.lr = 0x82A83BC4;
	sub_82A83A68(ctx, base);
	// mr r11,r31
	ctx.r11.u64 = ctx.r31.u64;
	// mr r29,r31
	ctx.r29.u64 = ctx.r31.u64;
	// rlwinm r10,r11,1,0,30
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 1) & 0xFFFFFFFE;
	// subf r9,r29,r28
	ctx.r9.u64 = ctx.r28.u64 - ctx.r29.u64;
	// add r11,r11,r10
	ctx.r11.u64 = ctx.r11.u64 + ctx.r10.u64;
	// rlwinm r10,r9,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r5,r10,r27
	ctx.r5.u64 = ctx.r10.u64 + ctx.r27.u64;
	// add r30,r11,r30
	ctx.r30.u64 = ctx.r11.u64 + ctx.r30.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// srawi r31,r31,2
	ctx.xer.ca = (ctx.r31.s32 < 0) & ((ctx.r31.u32 & 0x3) != 0);
	ctx.r31.s64 = ctx.r31.s32 >> 2;
	// bl 0x82a81d18
	ctx.lr = 0x82A83BF8;
	sub_82A81D18(ctx, base);
	// cmpwi cr6,r29,512
	ctx.cr6.compare<int32_t>(ctx.r29.s32, 512, ctx.xer);
	// bgt cr6,0x82a83b80
	if (ctx.cr6.gt) goto loc_82A83B80;
loc_82A83C00:
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x82a83668
	ctx.lr = 0x82A83C14;
	sub_82A83668(ctx, base);
	// addi r1,r1,128
	ctx.r1.s64 = ctx.r1.s64 + 128;
	// b 0x829ff814
	__restgprlr_27(ctx, base);
	return;
}

DEFINE_REX_FUNC(sub_82A83C20) {
	REX_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c0
	ctx.lr = 0x82A83C28;
	__savegprlr_26(ctx, base);
	// addi r12,r1,-56
	ctx.r12.s64 = ctx.r1.s64 + -56;
	// bl 0x82a01310
	ctx.lr = 0x82A83C30;
	__savefpr_22(ctx, base);
	// stwu r1,-224(r1)
	ea = -224 + ctx.r1.u32;
	REX_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// mr r31,r4
	ctx.r31.u64 = ctx.r4.u64;
	// mr r26,r5
	ctx.r26.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r29,r7
	ctx.r29.u64 = ctx.r7.u64;
	// cmpwi cr6,r27,32
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 32, ctx.xer);
	// ble cr6,0x82a83d40
	if (!ctx.cr6.gt) goto loc_82A83D40;
	// srawi r30,r27,2
	ctx.xer.ca = (ctx.r27.s32 < 0) & ((ctx.r27.u32 & 0x3) != 0);
	ctx.r30.s64 = ctx.r27.s32 >> 2;
	// subf r11,r30,r28
	ctx.r11.u64 = ctx.r28.u64 - ctx.r30.u64;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r5,r11,r29
	ctx.r5.u64 = ctx.r11.u64 + ctx.r29.u64;
	// bl 0x82a80b48
	ctx.lr = 0x82A83C64;
	sub_82A80B48(ctx, base);
	// cmpwi cr6,r27,512
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 512, ctx.xer);
	// mr r6,r29
	ctx.r6.u64 = ctx.r29.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// ble cr6,0x82a83cec
	if (!ctx.cr6.gt) goto loc_82A83CEC;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82a83a68
	ctx.lr = 0x82A83C7C;
	sub_82A83A68(ctx, base);
	// rlwinm r11,r30,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// mr r6,r29
	ctx.r6.u64 = ctx.r29.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// add r4,r11,r31
	ctx.r4.u64 = ctx.r11.u64 + ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82a83b48
	ctx.lr = 0x82A83C94;
	sub_82A83B48(ctx, base);
	// rlwinm r11,r30,3,0,28
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 3) & 0xFFFFFFF8;
	// mr r6,r29
	ctx.r6.u64 = ctx.r29.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// add r4,r11,r31
	ctx.r4.u64 = ctx.r11.u64 + ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82a83a68
	ctx.lr = 0x82A83CAC;
	sub_82A83A68(ctx, base);
	// rlwinm r11,r30,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 1) & 0xFFFFFFFE;
	// mr r6,r29
	ctx.r6.u64 = ctx.r29.u64;
	// add r11,r30,r11
	ctx.r11.u64 = ctx.r30.u64 + ctx.r11.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// add r4,r11,r31
	ctx.r4.u64 = ctx.r11.u64 + ctx.r31.u64;
	// bl 0x82a83a68
	ctx.lr = 0x82A83CCC;
	sub_82A83A68(ctx, base);
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// bl 0x82a7ff58
	ctx.lr = 0x82A83CDC;
	sub_82A7FF58(ctx, base);
	// addi r1,r1,224
	ctx.r1.s64 = ctx.r1.s64 + 224;
	// addi r12,r1,-56
	ctx.r12.s64 = ctx.r1.s64 + -56;
	// bl 0x82a0135c
	ctx.lr = 0x82A83CE8;
	__restfpr_22(ctx, base);
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
loc_82A83CEC:
	// cmpwi cr6,r30,32
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 32, ctx.xer);
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// ble cr6,0x82a83d1c
	if (!ctx.cr6.gt) goto loc_82A83D1C;
	// bl 0x82a83290
	ctx.lr = 0x82A83CFC;
	sub_82A83290(ctx, base);
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// bl 0x82a7ff58
	ctx.lr = 0x82A83D0C;
	sub_82A7FF58(ctx, base);
	// addi r1,r1,224
	ctx.r1.s64 = ctx.r1.s64 + 224;
	// addi r12,r1,-56
	ctx.r12.s64 = ctx.r1.s64 + -56;
	// bl 0x82a0135c
	ctx.lr = 0x82A83D18;
	__restfpr_22(ctx, base);
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
loc_82A83D1C:
	// bl 0x82a831f0
	ctx.lr = 0x82A83D20;
	sub_82A831F0(ctx, base);
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// bl 0x82a7ff58
	ctx.lr = 0x82A83D30;
	sub_82A7FF58(ctx, base);
	// addi r1,r1,224
	ctx.r1.s64 = ctx.r1.s64 + 224;
	// addi r12,r1,-56
	ctx.r12.s64 = ctx.r1.s64 + -56;
	// bl 0x82a0135c
	ctx.lr = 0x82A83D3C;
	__restfpr_22(ctx, base);
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
loc_82A83D40:
	// cmpwi cr6,r27,8
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 8, ctx.xer);
	// ble cr6,0x82a83e8c
	if (!ctx.cr6.gt) goto loc_82A83E8C;
	// cmpwi cr6,r27,32
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 32, ctx.xer);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bne cr6,0x82a83e34
	if (!ctx.cr6.eq) goto loc_82A83E34;
	// addi r11,r28,-8
	ctx.r11.s64 = ctx.r28.s64 + -8;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r4,r11,r29
	ctx.r4.u64 = ctx.r11.u64 + ctx.r29.u64;
	// bl 0x82a82150
	ctx.lr = 0x82A83D64;
	sub_82A82150(ctx, base);
	// lfs f0,8(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,12(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 12);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,16(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 16);
	ctx.f12.f64 = double(temp.f32);
	// lfs f11,20(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 20);
	ctx.f11.f64 = double(temp.f32);
	// lfs f10,24(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 24);
	ctx.f10.f64 = double(temp.f32);
	// lfs f9,40(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 40);
	ctx.f9.f64 = double(temp.f32);
	// lfs f8,44(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 44);
	ctx.f8.f64 = double(temp.f32);
	// lfs f7,28(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 28);
	ctx.f7.f64 = double(temp.f32);
	// lfs f2,64(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 64);
	ctx.f2.f64 = double(temp.f32);
	// lfs f1,68(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 68);
	ctx.f1.f64 = double(temp.f32);
	// lfs f31,32(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 32);
	ctx.f31.f64 = double(temp.f32);
	// lfs f30,36(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 36);
	ctx.f30.f64 = double(temp.f32);
	// lfs f29,96(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 96);
	ctx.f29.f64 = double(temp.f32);
	// lfs f28,80(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 80);
	ctx.f28.f64 = double(temp.f32);
	// lfs f27,84(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 84);
	ctx.f27.f64 = double(temp.f32);
	// lfs f6,56(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 56);
	ctx.f6.f64 = double(temp.f32);
	// lfs f5,60(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 60);
	ctx.f5.f64 = double(temp.f32);
	// lfs f4,88(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 88);
	ctx.f4.f64 = double(temp.f32);
	// lfs f3,92(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 92);
	ctx.f3.f64 = double(temp.f32);
	// lfs f26,100(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 100);
	ctx.f26.f64 = double(temp.f32);
	// lfs f25,112(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 112);
	ctx.f25.f64 = double(temp.f32);
	// lfs f24,116(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 116);
	ctx.f24.f64 = double(temp.f32);
	// lfs f23,104(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 104);
	ctx.f23.f64 = double(temp.f32);
	// lfs f22,108(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 108);
	ctx.f22.f64 = double(temp.f32);
	// stfs f2,8(r31)
	temp.f32 = float(ctx.f2.f64);
	REX_STORE_U32(ctx.r31.u32 + 8, temp.u32);
	// stfs f1,12(r31)
	temp.f32 = float(ctx.f1.f64);
	REX_STORE_U32(ctx.r31.u32 + 12, temp.u32);
	// stfs f31,16(r31)
	temp.f32 = float(ctx.f31.f64);
	REX_STORE_U32(ctx.r31.u32 + 16, temp.u32);
	// stfs f30,20(r31)
	temp.f32 = float(ctx.f30.f64);
	REX_STORE_U32(ctx.r31.u32 + 20, temp.u32);
	// stfs f29,24(r31)
	temp.f32 = float(ctx.f29.f64);
	REX_STORE_U32(ctx.r31.u32 + 24, temp.u32);
	// stfs f26,28(r31)
	temp.f32 = float(ctx.f26.f64);
	REX_STORE_U32(ctx.r31.u32 + 28, temp.u32);
	// stfs f28,40(r31)
	temp.f32 = float(ctx.f28.f64);
	REX_STORE_U32(ctx.r31.u32 + 40, temp.u32);
	// stfs f27,44(r31)
	temp.f32 = float(ctx.f27.f64);
	REX_STORE_U32(ctx.r31.u32 + 44, temp.u32);
	// stfs f25,56(r31)
	temp.f32 = float(ctx.f25.f64);
	REX_STORE_U32(ctx.r31.u32 + 56, temp.u32);
	// stfs f24,60(r31)
	temp.f32 = float(ctx.f24.f64);
	REX_STORE_U32(ctx.r31.u32 + 60, temp.u32);
	// stfs f23,88(r31)
	temp.f32 = float(ctx.f23.f64);
	REX_STORE_U32(ctx.r31.u32 + 88, temp.u32);
	// stfs f22,92(r31)
	temp.f32 = float(ctx.f22.f64);
	REX_STORE_U32(ctx.r31.u32 + 92, temp.u32);
	// stfs f0,64(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 64, temp.u32);
	// stfs f13,68(r31)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r31.u32 + 68, temp.u32);
	// stfs f12,32(r31)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r31.u32 + 32, temp.u32);
	// stfs f11,36(r31)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r31.u32 + 36, temp.u32);
	// stfs f10,96(r31)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r31.u32 + 96, temp.u32);
	// stfs f9,80(r31)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r31.u32 + 80, temp.u32);
	// stfs f8,84(r31)
	temp.f32 = float(ctx.f8.f64);
	REX_STORE_U32(ctx.r31.u32 + 84, temp.u32);
	// stfs f7,100(r31)
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r31.u32 + 100, temp.u32);
	// stfs f4,104(r31)
	temp.f32 = float(ctx.f4.f64);
	REX_STORE_U32(ctx.r31.u32 + 104, temp.u32);
	// stfs f3,108(r31)
	temp.f32 = float(ctx.f3.f64);
	REX_STORE_U32(ctx.r31.u32 + 108, temp.u32);
	// stfs f6,112(r31)
	temp.f32 = float(ctx.f6.f64);
	REX_STORE_U32(ctx.r31.u32 + 112, temp.u32);
	// stfs f5,116(r31)
	temp.f32 = float(ctx.f5.f64);
	REX_STORE_U32(ctx.r31.u32 + 116, temp.u32);
	// addi r1,r1,224
	ctx.r1.s64 = ctx.r1.s64 + 224;
	// addi r12,r1,-56
	ctx.r12.s64 = ctx.r1.s64 + -56;
	// bl 0x82a0135c
	ctx.lr = 0x82A83E30;
	__restfpr_22(ctx, base);
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
loc_82A83E34:
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// bl 0x82a82ad0
	ctx.lr = 0x82A83E3C;
	sub_82A82AD0(ctx, base);
	// lfs f0,8(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,12(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 12);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,24(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 24);
	ctx.f12.f64 = double(temp.f32);
	// lfs f11,28(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 28);
	ctx.f11.f64 = double(temp.f32);
	// lfs f10,32(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 32);
	ctx.f10.f64 = double(temp.f32);
	// lfs f9,36(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 36);
	ctx.f9.f64 = double(temp.f32);
	// lfs f8,48(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 48);
	ctx.f8.f64 = double(temp.f32);
	// lfs f7,52(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 52);
	ctx.f7.f64 = double(temp.f32);
	// stfs f9,12(r31)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r31.u32 + 12, temp.u32);
	// stfs f8,24(r31)
	temp.f32 = float(ctx.f8.f64);
	REX_STORE_U32(ctx.r31.u32 + 24, temp.u32);
	// stfs f7,28(r31)
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r31.u32 + 28, temp.u32);
	// stfs f0,32(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 32, temp.u32);
	// stfs f13,36(r31)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r31.u32 + 36, temp.u32);
	// stfs f12,48(r31)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r31.u32 + 48, temp.u32);
	// stfs f11,52(r31)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r31.u32 + 52, temp.u32);
	// stfs f10,8(r31)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r31.u32 + 8, temp.u32);
	// addi r1,r1,224
	ctx.r1.s64 = ctx.r1.s64 + 224;
	// addi r12,r1,-56
	ctx.r12.s64 = ctx.r1.s64 + -56;
	// bl 0x82a0135c
	ctx.lr = 0x82A83E88;
	__restfpr_22(ctx, base);
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
loc_82A83E8C:
	// bne cr6,0x82a83f20
	if (!ctx.cr6.eq) goto loc_82A83F20;
	// lfs f13,0(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// lfs f0,16(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 16);
	ctx.f0.f64 = double(temp.f32);
	// fadds f6,f13,f0
	ctx.f6.f64 = double(float(ctx.f13.f64 + ctx.f0.f64));
	// lfs f11,4(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 4);
	ctx.f11.f64 = double(temp.f32);
	// lfs f12,20(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 20);
	ctx.f12.f64 = double(temp.f32);
	// fsubs f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 - ctx.f0.f64));
	// lfs f9,8(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 8);
	ctx.f9.f64 = double(temp.f32);
	// fadds f13,f11,f12
	ctx.f13.f64 = double(float(ctx.f11.f64 + ctx.f12.f64));
	// lfs f10,24(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 24);
	ctx.f10.f64 = double(temp.f32);
	// fsubs f12,f11,f12
	ctx.f12.f64 = double(float(ctx.f11.f64 - ctx.f12.f64));
	// fadds f11,f10,f9
	ctx.f11.f64 = double(float(ctx.f10.f64 + ctx.f9.f64));
	// lfs f7,12(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 12);
	ctx.f7.f64 = double(temp.f32);
	// lfs f8,28(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 28);
	ctx.f8.f64 = double(temp.f32);
	// fsubs f10,f9,f10
	ctx.f10.f64 = double(float(ctx.f9.f64 - ctx.f10.f64));
	// fadds f9,f8,f7
	ctx.f9.f64 = double(float(ctx.f8.f64 + ctx.f7.f64));
	// fsubs f8,f7,f8
	ctx.f8.f64 = double(float(ctx.f7.f64 - ctx.f8.f64));
	// fadds f7,f11,f6
	ctx.f7.f64 = double(float(ctx.f11.f64 + ctx.f6.f64));
	// stfs f7,0(r31)
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r31.u32 + 0, temp.u32);
	// fsubs f11,f6,f11
	ctx.f11.f64 = double(float(ctx.f6.f64 - ctx.f11.f64));
	// stfs f11,16(r31)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r31.u32 + 16, temp.u32);
	// fadds f11,f9,f13
	ctx.f11.f64 = double(float(ctx.f9.f64 + ctx.f13.f64));
	// stfs f11,4(r31)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r31.u32 + 4, temp.u32);
	// fsubs f13,f13,f9
	ctx.f13.f64 = double(float(ctx.f13.f64 - ctx.f9.f64));
	// stfs f13,20(r31)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r31.u32 + 20, temp.u32);
	// fsubs f13,f0,f8
	ctx.f13.f64 = double(float(ctx.f0.f64 - ctx.f8.f64));
	// stfs f13,8(r31)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r31.u32 + 8, temp.u32);
	// fadds f0,f8,f0
	ctx.f0.f64 = double(float(ctx.f8.f64 + ctx.f0.f64));
	// stfs f0,24(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 24, temp.u32);
	// fadds f13,f10,f12
	ctx.f13.f64 = double(float(ctx.f10.f64 + ctx.f12.f64));
	// stfs f13,12(r31)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r31.u32 + 12, temp.u32);
	// fsubs f0,f12,f10
	ctx.f0.f64 = double(float(ctx.f12.f64 - ctx.f10.f64));
	// stfs f0,28(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 28, temp.u32);
	// addi r1,r1,224
	ctx.r1.s64 = ctx.r1.s64 + 224;
	// addi r12,r1,-56
	ctx.r12.s64 = ctx.r1.s64 + -56;
	// bl 0x82a0135c
	ctx.lr = 0x82A83F1C;
	__restfpr_22(ctx, base);
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
loc_82A83F20:
	// cmpwi cr6,r27,4
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 4, ctx.xer);
	// bne cr6,0x82a83f58
	if (!ctx.cr6.eq) goto loc_82A83F58;
	// lfs f13,8(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 8);
	ctx.f13.f64 = double(temp.f32);
	// lfs f0,0(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// fsubs f10,f0,f13
	ctx.f10.f64 = double(float(ctx.f0.f64 - ctx.f13.f64));
	// lfs f12,4(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// lfs f11,12(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 12);
	ctx.f11.f64 = double(temp.f32);
	// fadds f0,f0,f13
	ctx.f0.f64 = double(float(ctx.f0.f64 + ctx.f13.f64));
	// stfs f0,0(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 0, temp.u32);
	// fadds f13,f12,f11
	ctx.f13.f64 = double(float(ctx.f12.f64 + ctx.f11.f64));
	// fsubs f0,f12,f11
	ctx.f0.f64 = double(float(ctx.f12.f64 - ctx.f11.f64));
	// stfs f13,4(r31)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r31.u32 + 4, temp.u32);
	// stfs f0,12(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 12, temp.u32);
	// stfs f10,8(r31)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r31.u32 + 8, temp.u32);
loc_82A83F58:
	// addi r1,r1,224
	ctx.r1.s64 = ctx.r1.s64 + 224;
	// addi r12,r1,-56
	ctx.r12.s64 = ctx.r1.s64 + -56;
	// bl 0x82a0135c
	ctx.lr = 0x82A83F64;
	__restfpr_22(ctx, base);
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
}

DEFINE_REX_FUNC(sub_82A83F68) {
	REX_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7c0
	ctx.lr = 0x82A83F70;
	__savegprlr_26(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	REX_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r27,r3
	ctx.r27.u64 = ctx.r3.u64;
	// mr r31,r4
	ctx.r31.u64 = ctx.r4.u64;
	// mr r26,r5
	ctx.r26.u64 = ctx.r5.u64;
	// mr r28,r6
	ctx.r28.u64 = ctx.r6.u64;
	// mr r29,r7
	ctx.r29.u64 = ctx.r7.u64;
	// cmpwi cr6,r27,32
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 32, ctx.xer);
	// ble cr6,0x82a84068
	if (!ctx.cr6.gt) goto loc_82A84068;
	// srawi r30,r27,2
	ctx.xer.ca = (ctx.r27.s32 < 0) & ((ctx.r27.u32 & 0x3) != 0);
	ctx.r30.s64 = ctx.r27.s32 >> 2;
	// subf r11,r30,r28
	ctx.r11.u64 = ctx.r28.u64 - ctx.r30.u64;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r5,r11,r29
	ctx.r5.u64 = ctx.r11.u64 + ctx.r29.u64;
	// bl 0x82a81250
	ctx.lr = 0x82A83FA4;
	sub_82A81250(ctx, base);
	// cmpwi cr6,r27,512
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 512, ctx.xer);
	// mr r6,r29
	ctx.r6.u64 = ctx.r29.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// ble cr6,0x82a84024
	if (!ctx.cr6.gt) goto loc_82A84024;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82a83a68
	ctx.lr = 0x82A83FBC;
	sub_82A83A68(ctx, base);
	// rlwinm r11,r30,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// mr r6,r29
	ctx.r6.u64 = ctx.r29.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// add r4,r11,r31
	ctx.r4.u64 = ctx.r11.u64 + ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82a83b48
	ctx.lr = 0x82A83FD4;
	sub_82A83B48(ctx, base);
	// rlwinm r11,r30,3,0,28
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 3) & 0xFFFFFFF8;
	// mr r6,r29
	ctx.r6.u64 = ctx.r29.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// add r4,r11,r31
	ctx.r4.u64 = ctx.r11.u64 + ctx.r31.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82a83a68
	ctx.lr = 0x82A83FEC;
	sub_82A83A68(ctx, base);
	// rlwinm r11,r30,1,0,30
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 1) & 0xFFFFFFFE;
	// mr r6,r29
	ctx.r6.u64 = ctx.r29.u64;
	// add r11,r30,r11
	ctx.r11.u64 = ctx.r30.u64 + ctx.r11.u64;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// add r4,r11,r31
	ctx.r4.u64 = ctx.r11.u64 + ctx.r31.u64;
	// bl 0x82a83a68
	ctx.lr = 0x82A8400C;
	sub_82A83A68(ctx, base);
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// bl 0x82a80448
	ctx.lr = 0x82A8401C;
	sub_82A80448(ctx, base);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
loc_82A84024:
	// cmpwi cr6,r30,32
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 32, ctx.xer);
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// ble cr6,0x82a8404c
	if (!ctx.cr6.gt) goto loc_82A8404C;
	// bl 0x82a83290
	ctx.lr = 0x82A84034;
	sub_82A83290(ctx, base);
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// bl 0x82a80448
	ctx.lr = 0x82A84044;
	sub_82A80448(ctx, base);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
loc_82A8404C:
	// bl 0x82a831f0
	ctx.lr = 0x82A84050;
	sub_82A831F0(ctx, base);
	// mr r5,r31
	ctx.r5.u64 = ctx.r31.u64;
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// mr r3,r27
	ctx.r3.u64 = ctx.r27.u64;
	// bl 0x82a80448
	ctx.lr = 0x82A84060;
	sub_82A80448(ctx, base);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
loc_82A84068:
	// cmpwi cr6,r27,8
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 8, ctx.xer);
	// ble cr6,0x82a84118
	if (!ctx.cr6.gt) goto loc_82A84118;
	// cmpwi cr6,r27,32
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 32, ctx.xer);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bne cr6,0x82a84098
	if (!ctx.cr6.eq) goto loc_82A84098;
	// addi r11,r28,-8
	ctx.r11.s64 = ctx.r28.s64 + -8;
	// rlwinm r11,r11,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// add r4,r11,r29
	ctx.r4.u64 = ctx.r11.u64 + ctx.r29.u64;
	// bl 0x82a82150
	ctx.lr = 0x82A8408C;
	sub_82A82150(ctx, base);
	// bl 0x82a80a30
	ctx.lr = 0x82A84090;
	sub_82A80A30(ctx, base);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
loc_82A84098:
	// mr r4,r29
	ctx.r4.u64 = ctx.r29.u64;
	// bl 0x82a82ad0
	ctx.lr = 0x82A840A0;
	sub_82A82AD0(ctx, base);
	// lfs f0,8(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 8);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,12(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 12);
	ctx.f13.f64 = double(temp.f32);
	// lfs f12,16(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 16);
	ctx.f12.f64 = double(temp.f32);
	// lfs f11,20(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 20);
	ctx.f11.f64 = double(temp.f32);
	// lfs f10,32(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 32);
	ctx.f10.f64 = double(temp.f32);
	// lfs f9,36(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 36);
	ctx.f9.f64 = double(temp.f32);
	// lfs f8,56(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 56);
	ctx.f8.f64 = double(temp.f32);
	// lfs f7,60(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 60);
	ctx.f7.f64 = double(temp.f32);
	// lfs f6,24(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 24);
	ctx.f6.f64 = double(temp.f32);
	// lfs f5,28(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 28);
	ctx.f5.f64 = double(temp.f32);
	// lfs f4,40(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 40);
	ctx.f4.f64 = double(temp.f32);
	// lfs f3,44(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 44);
	ctx.f3.f64 = double(temp.f32);
	// lfs f2,48(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 48);
	ctx.f2.f64 = double(temp.f32);
	// lfs f1,52(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 52);
	ctx.f1.f64 = double(temp.f32);
	// stfs f8,8(r31)
	temp.f32 = float(ctx.f8.f64);
	REX_STORE_U32(ctx.r31.u32 + 8, temp.u32);
	// stfs f7,12(r31)
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r31.u32 + 12, temp.u32);
	// stfs f6,16(r31)
	temp.f32 = float(ctx.f6.f64);
	REX_STORE_U32(ctx.r31.u32 + 16, temp.u32);
	// stfs f5,20(r31)
	temp.f32 = float(ctx.f5.f64);
	REX_STORE_U32(ctx.r31.u32 + 20, temp.u32);
	// stfs f4,24(r31)
	temp.f32 = float(ctx.f4.f64);
	REX_STORE_U32(ctx.r31.u32 + 24, temp.u32);
	// stfs f3,28(r31)
	temp.f32 = float(ctx.f3.f64);
	REX_STORE_U32(ctx.r31.u32 + 28, temp.u32);
	// stfs f2,40(r31)
	temp.f32 = float(ctx.f2.f64);
	REX_STORE_U32(ctx.r31.u32 + 40, temp.u32);
	// stfs f1,44(r31)
	temp.f32 = float(ctx.f1.f64);
	REX_STORE_U32(ctx.r31.u32 + 44, temp.u32);
	// stfs f0,32(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 32, temp.u32);
	// stfs f13,36(r31)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r31.u32 + 36, temp.u32);
	// stfs f12,48(r31)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r31.u32 + 48, temp.u32);
	// stfs f11,52(r31)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r31.u32 + 52, temp.u32);
	// stfs f10,56(r31)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r31.u32 + 56, temp.u32);
	// stfs f9,60(r31)
	temp.f32 = float(ctx.f9.f64);
	REX_STORE_U32(ctx.r31.u32 + 60, temp.u32);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
loc_82A84118:
	// bne cr6,0x82a841a4
	if (!ctx.cr6.eq) goto loc_82A841A4;
	// lfs f13,0(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 0);
	ctx.f13.f64 = double(temp.f32);
	// lfs f0,16(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 16);
	ctx.f0.f64 = double(temp.f32);
	// fadds f6,f13,f0
	ctx.f6.f64 = double(float(ctx.f13.f64 + ctx.f0.f64));
	// lfs f11,4(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 4);
	ctx.f11.f64 = double(temp.f32);
	// lfs f12,20(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 20);
	ctx.f12.f64 = double(temp.f32);
	// fsubs f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 - ctx.f0.f64));
	// lfs f9,8(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 8);
	ctx.f9.f64 = double(temp.f32);
	// fadds f13,f11,f12
	ctx.f13.f64 = double(float(ctx.f11.f64 + ctx.f12.f64));
	// lfs f10,24(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 24);
	ctx.f10.f64 = double(temp.f32);
	// fsubs f12,f11,f12
	ctx.f12.f64 = double(float(ctx.f11.f64 - ctx.f12.f64));
	// fadds f11,f10,f9
	ctx.f11.f64 = double(float(ctx.f10.f64 + ctx.f9.f64));
	// lfs f7,12(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 12);
	ctx.f7.f64 = double(temp.f32);
	// lfs f8,28(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 28);
	ctx.f8.f64 = double(temp.f32);
	// fsubs f10,f9,f10
	ctx.f10.f64 = double(float(ctx.f9.f64 - ctx.f10.f64));
	// fadds f9,f8,f7
	ctx.f9.f64 = double(float(ctx.f8.f64 + ctx.f7.f64));
	// fsubs f8,f7,f8
	ctx.f8.f64 = double(float(ctx.f7.f64 - ctx.f8.f64));
	// fadds f7,f11,f6
	ctx.f7.f64 = double(float(ctx.f11.f64 + ctx.f6.f64));
	// stfs f7,0(r31)
	temp.f32 = float(ctx.f7.f64);
	REX_STORE_U32(ctx.r31.u32 + 0, temp.u32);
	// fsubs f11,f6,f11
	ctx.f11.f64 = double(float(ctx.f6.f64 - ctx.f11.f64));
	// stfs f11,16(r31)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r31.u32 + 16, temp.u32);
	// fadds f11,f9,f13
	ctx.f11.f64 = double(float(ctx.f9.f64 + ctx.f13.f64));
	// stfs f11,4(r31)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r31.u32 + 4, temp.u32);
	// fsubs f13,f13,f9
	ctx.f13.f64 = double(float(ctx.f13.f64 - ctx.f9.f64));
	// stfs f13,20(r31)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r31.u32 + 20, temp.u32);
	// fadds f13,f8,f0
	ctx.f13.f64 = double(float(ctx.f8.f64 + ctx.f0.f64));
	// stfs f13,8(r31)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r31.u32 + 8, temp.u32);
	// fsubs f0,f0,f8
	ctx.f0.f64 = double(float(ctx.f0.f64 - ctx.f8.f64));
	// stfs f0,24(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 24, temp.u32);
	// fsubs f13,f12,f10
	ctx.f13.f64 = double(float(ctx.f12.f64 - ctx.f10.f64));
	// stfs f13,12(r31)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r31.u32 + 12, temp.u32);
	// fadds f0,f10,f12
	ctx.f0.f64 = double(float(ctx.f10.f64 + ctx.f12.f64));
	// stfs f0,28(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 28, temp.u32);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
loc_82A841A4:
	// cmpwi cr6,r27,4
	ctx.cr6.compare<int32_t>(ctx.r27.s32, 4, ctx.xer);
	// bne cr6,0x82a841dc
	if (!ctx.cr6.eq) goto loc_82A841DC;
	// lfs f13,8(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 8);
	ctx.f13.f64 = double(temp.f32);
	// lfs f0,0(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// fsubs f10,f0,f13
	ctx.f10.f64 = double(float(ctx.f0.f64 - ctx.f13.f64));
	// lfs f12,4(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 4);
	ctx.f12.f64 = double(temp.f32);
	// lfs f11,12(r31)
	temp.u32 = REX_LOAD_U32(ctx.r31.u32 + 12);
	ctx.f11.f64 = double(temp.f32);
	// fadds f0,f0,f13
	ctx.f0.f64 = double(float(ctx.f0.f64 + ctx.f13.f64));
	// stfs f0,0(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 0, temp.u32);
	// fadds f13,f12,f11
	ctx.f13.f64 = double(float(ctx.f12.f64 + ctx.f11.f64));
	// fsubs f0,f12,f11
	ctx.f0.f64 = double(float(ctx.f12.f64 - ctx.f11.f64));
	// stfs f13,4(r31)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r31.u32 + 4, temp.u32);
	// stfs f10,8(r31)
	temp.f32 = float(ctx.f10.f64);
	REX_STORE_U32(ctx.r31.u32 + 8, temp.u32);
	// stfs f0,12(r31)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r31.u32 + 12, temp.u32);
loc_82A841DC:
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x829ff810
	__restgprlr_26(ctx, base);
	return;
}

DEFINE_REX_FUNC(sub_82A841E8) {
	REX_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7bc
	ctx.lr = 0x82A841F0;
	__savegprlr_25(ctx, base);
	// stwu r1,-144(r1)
	ea = -144 + ctx.r1.u32;
	REX_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r27,r6
	ctx.r27.u64 = ctx.r6.u64;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// mr r25,r4
	ctx.r25.u64 = ctx.r4.u64;
	// mr r28,r5
	ctx.r28.u64 = ctx.r5.u64;
	// mr r29,r7
	ctx.r29.u64 = ctx.r7.u64;
	// lwz r30,0(r27)
	ctx.r30.u64 = REX_LOAD_U32(ctx.r27.u32 + 0);
	// rlwinm r11,r30,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// cmpw cr6,r31,r11
	ctx.cr6.compare<int32_t>(ctx.r31.s32, ctx.r11.s32, ctx.xer);
	// ble cr6,0x82a8422c
	if (!ctx.cr6.gt) goto loc_82A8422C;
	// srawi r30,r31,2
	ctx.xer.ca = (ctx.r31.s32 < 0) & ((ctx.r31.u32 & 0x3) != 0);
	ctx.r30.s64 = ctx.r31.s32 >> 2;
	// mr r5,r29
	ctx.r5.u64 = ctx.r29.u64;
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// mr r3,r30
	ctx.r3.u64 = ctx.r30.u64;
	// bl 0x82a7fc38
	ctx.lr = 0x82A8422C;
	sub_82A7FC38(ctx, base);
loc_82A8422C:
	// lwz r26,4(r27)
	ctx.r26.u64 = REX_LOAD_U32(ctx.r27.u32 + 4);
	// rlwinm r11,r26,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r26.u32 | (ctx.r26.u64 << 32), 2) & 0xFFFFFFFC;
	// cmpw cr6,r31,r11
	ctx.cr6.compare<int32_t>(ctx.r31.s32, ctx.r11.s32, ctx.xer);
	// ble cr6,0x82a84254
	if (!ctx.cr6.gt) goto loc_82A84254;
	// rlwinm r11,r30,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// srawi r26,r31,2
	ctx.xer.ca = (ctx.r31.s32 < 0) & ((ctx.r31.u32 & 0x3) != 0);
	ctx.r26.s64 = ctx.r31.s32 >> 2;
	// add r5,r11,r29
	ctx.r5.u64 = ctx.r11.u64 + ctx.r29.u64;
	// mr r4,r27
	ctx.r4.u64 = ctx.r27.u64;
	// mr r3,r26
	ctx.r3.u64 = ctx.r26.u64;
	// bl 0x82a7fe58
	ctx.lr = 0x82A84254;
	sub_82A7FE58(ctx, base);
loc_82A84254:
	// cmpwi cr6,r25,0
	ctx.cr6.compare<int32_t>(ctx.r25.s32, 0, ctx.xer);
	// blt cr6,0x82a842d4
	if (ctx.cr6.lt) goto loc_82A842D4;
	// cmpwi cr6,r31,4
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 4, ctx.xer);
	// ble cr6,0x82a84298
	if (!ctx.cr6.gt) goto loc_82A84298;
	// mr r7,r29
	ctx.r7.u64 = ctx.r29.u64;
	// mr r6,r30
	ctx.r6.u64 = ctx.r30.u64;
	// addi r5,r27,8
	ctx.r5.s64 = ctx.r27.s64 + 8;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a83c20
	ctx.lr = 0x82A8427C;
	sub_82A83C20(ctx, base);
	// rlwinm r11,r30,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// mr r5,r26
	ctx.r5.u64 = ctx.r26.u64;
	// add r6,r11,r29
	ctx.r6.u64 = ctx.r11.u64 + ctx.r29.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a82e48
	ctx.lr = 0x82A84294;
	sub_82A82E48(ctx, base);
	// b 0x82a842b4
	goto loc_82A842B4;
loc_82A84298:
	// bne cr6,0x82a842b4
	if (!ctx.cr6.eq) goto loc_82A842B4;
	// mr r7,r29
	ctx.r7.u64 = ctx.r29.u64;
	// mr r6,r30
	ctx.r6.u64 = ctx.r30.u64;
	// addi r5,r27,8
	ctx.r5.s64 = ctx.r27.s64 + 8;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// li r3,4
	ctx.r3.s64 = 4;
	// bl 0x82a83c20
	ctx.lr = 0x82A842B4;
	sub_82A83C20(ctx, base);
loc_82A842B4:
	// lfs f0,0(r28)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r28.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lfs f13,4(r28)
	temp.u32 = REX_LOAD_U32(ctx.r28.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// fsubs f12,f0,f13
	ctx.f12.f64 = double(float(ctx.f0.f64 - ctx.f13.f64));
	// stfs f12,4(r28)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r28.u32 + 4, temp.u32);
	// fadds f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 + ctx.f0.f64));
	// stfs f0,0(r28)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r28.u32 + 0, temp.u32);
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x829ff80c
	__restgprlr_25(ctx, base);
	return;
loc_82A842D4:
	// lfs f0,0(r28)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r28.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// lis r11,-32256
	ctx.r11.s64 = -2113929216;
	// lfs f13,4(r28)
	temp.u32 = REX_LOAD_U32(ctx.r28.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// cmpwi cr6,r31,4
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 4, ctx.xer);
	// fsubs f12,f0,f13
	ctx.f12.f64 = double(float(ctx.f0.f64 - ctx.f13.f64));
	// lfs f13,3444(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 3444);
	ctx.f13.f64 = double(temp.f32);
	// fmuls f13,f12,f13
	ctx.f13.f64 = double(float(ctx.f12.f64 * ctx.f13.f64));
	// stfs f13,4(r28)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r28.u32 + 4, temp.u32);
	// fsubs f0,f0,f13
	ctx.f0.f64 = double(float(ctx.f0.f64 - ctx.f13.f64));
	// stfs f0,0(r28)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r28.u32 + 0, temp.u32);
	// ble cr6,0x82a84320
	if (!ctx.cr6.gt) goto loc_82A84320;
	// rlwinm r11,r30,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r30.u32 | (ctx.r30.u64 << 32), 2) & 0xFFFFFFFC;
	// mr r5,r26
	ctx.r5.u64 = ctx.r26.u64;
	// add r6,r11,r29
	ctx.r6.u64 = ctx.r11.u64 + ctx.r29.u64;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a82f18
	ctx.lr = 0x82A84318;
	sub_82A82F18(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// b 0x82a84328
	goto loc_82A84328;
loc_82A84320:
	// bne cr6,0x82a8433c
	if (!ctx.cr6.eq) goto loc_82A8433C;
	// li r3,4
	ctx.r3.s64 = 4;
loc_82A84328:
	// mr r7,r29
	ctx.r7.u64 = ctx.r29.u64;
	// mr r6,r30
	ctx.r6.u64 = ctx.r30.u64;
	// addi r5,r27,8
	ctx.r5.s64 = ctx.r27.s64 + 8;
	// mr r4,r28
	ctx.r4.u64 = ctx.r28.u64;
	// bl 0x82a83f68
	ctx.lr = 0x82A8433C;
	sub_82A83F68(ctx, base);
loc_82A8433C:
	// addi r1,r1,144
	ctx.r1.s64 = ctx.r1.s64 + 144;
	// b 0x829ff80c
	__restgprlr_25(ctx, base);
	return;
}

DEFINE_REX_FUNC(sub_82A84348) {
	REX_FUNC_PROLOGUE();
	PPCRegister temp{};
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7b8
	ctx.lr = 0x82A84350;
	__savegprlr_24(ctx, base);
	// stwu r1,-160(r1)
	ea = -160 + ctx.r1.u32;
	REX_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r26,r6
	ctx.r26.u64 = ctx.r6.u64;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// mr r24,r4
	ctx.r24.u64 = ctx.r4.u64;
	// mr r30,r5
	ctx.r30.u64 = ctx.r5.u64;
	// mr r28,r7
	ctx.r28.u64 = ctx.r7.u64;
	// lwz r29,0(r26)
	ctx.r29.u64 = REX_LOAD_U32(ctx.r26.u32 + 0);
	// rlwinm r11,r29,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 2) & 0xFFFFFFFC;
	// cmpw cr6,r31,r11
	ctx.cr6.compare<int32_t>(ctx.r31.s32, ctx.r11.s32, ctx.xer);
	// ble cr6,0x82a8438c
	if (!ctx.cr6.gt) goto loc_82A8438C;
	// srawi r29,r31,2
	ctx.xer.ca = (ctx.r31.s32 < 0) & ((ctx.r31.u32 & 0x3) != 0);
	ctx.r29.s64 = ctx.r31.s32 >> 2;
	// mr r5,r28
	ctx.r5.u64 = ctx.r28.u64;
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// mr r3,r29
	ctx.r3.u64 = ctx.r29.u64;
	// bl 0x82a7fc38
	ctx.lr = 0x82A8438C;
	sub_82A7FC38(ctx, base);
loc_82A8438C:
	// lwz r25,4(r26)
	ctx.r25.u64 = REX_LOAD_U32(ctx.r26.u32 + 4);
	// cmpw cr6,r31,r25
	ctx.cr6.compare<int32_t>(ctx.r31.s32, ctx.r25.s32, ctx.xer);
	// ble cr6,0x82a843b0
	if (!ctx.cr6.gt) goto loc_82A843B0;
	// rlwinm r11,r29,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 2) & 0xFFFFFFFC;
	// mr r4,r26
	ctx.r4.u64 = ctx.r26.u64;
	// add r5,r11,r28
	ctx.r5.u64 = ctx.r11.u64 + ctx.r28.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r25,r31
	ctx.r25.u64 = ctx.r31.u64;
	// bl 0x82a7fe58
	ctx.lr = 0x82A843B0;
	sub_82A7FE58(ctx, base);
loc_82A843B0:
	// cmpwi cr6,r24,0
	ctx.cr6.compare<int32_t>(ctx.r24.s32, 0, ctx.xer);
	// bge cr6,0x82a8445c
	if (!ctx.cr6.lt) goto loc_82A8445C;
	// rlwinm r10,r31,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r11,r31,-2
	ctx.r11.s64 = ctx.r31.s64 + -2;
	// add r10,r10,r30
	ctx.r10.u64 = ctx.r10.u64 + ctx.r30.u64;
	// cmpwi cr6,r11,2
	ctx.cr6.compare<int32_t>(ctx.r11.s32, 2, ctx.xer);
	// lfs f12,-4(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r10.u32 + -4);
	ctx.f12.f64 = double(temp.f32);
	// blt cr6,0x82a84404
	if (ctx.cr6.lt) goto loc_82A84404;
	// rlwinm r9,r11,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 2) & 0xFFFFFFFC;
	// rlwinm r10,r11,31,1,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 31) & 0x7FFFFFFF;
	// add r11,r9,r30
	ctx.r11.u64 = ctx.r9.u64 + ctx.r30.u64;
loc_82A843DC:
	// lfs f0,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
	// lfs f13,-4(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + -4);
	ctx.f13.f64 = double(temp.f32);
	// fsubs f11,f0,f13
	ctx.f11.f64 = double(float(ctx.f0.f64 - ctx.f13.f64));
	// stfs f11,4(r11)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r11.u32 + 4, temp.u32);
	// fadds f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 + ctx.f0.f64));
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// addi r11,r11,-8
	ctx.r11.s64 = ctx.r11.s64 + -8;
	// bne cr6,0x82a843dc
	if (!ctx.cr6.eq) goto loc_82A843DC;
loc_82A84404:
	// lfs f0,0(r30)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// cmpwi cr6,r31,4
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 4, ctx.xer);
	// fsubs f13,f0,f12
	ctx.f13.f64 = double(float(ctx.f0.f64 - ctx.f12.f64));
	// stfs f13,4(r30)
	temp.f32 = float(ctx.f13.f64);
	REX_STORE_U32(ctx.r30.u32 + 4, temp.u32);
	// fadds f0,f0,f12
	ctx.f0.f64 = double(float(ctx.f0.f64 + ctx.f12.f64));
	// stfs f0,0(r30)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r30.u32 + 0, temp.u32);
	// ble cr6,0x82a84440
	if (!ctx.cr6.gt) goto loc_82A84440;
	// rlwinm r11,r29,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 2) & 0xFFFFFFFC;
	// mr r5,r25
	ctx.r5.u64 = ctx.r25.u64;
	// add r6,r11,r28
	ctx.r6.u64 = ctx.r11.u64 + ctx.r28.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a82f18
	ctx.lr = 0x82A84438;
	sub_82A82F18(ctx, base);
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// b 0x82a84448
	goto loc_82A84448;
loc_82A84440:
	// bne cr6,0x82a8445c
	if (!ctx.cr6.eq) goto loc_82A8445C;
	// li r3,4
	ctx.r3.s64 = 4;
loc_82A84448:
	// mr r7,r28
	ctx.r7.u64 = ctx.r28.u64;
	// mr r6,r29
	ctx.r6.u64 = ctx.r29.u64;
	// addi r5,r26,8
	ctx.r5.s64 = ctx.r26.s64 + 8;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// bl 0x82a83f68
	ctx.lr = 0x82A8445C;
	sub_82A83F68(ctx, base);
loc_82A8445C:
	// rlwinm r11,r29,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r29.u32 | (ctx.r29.u64 << 32), 2) & 0xFFFFFFFC;
	// mr r5,r25
	ctx.r5.u64 = ctx.r25.u64;
	// add r27,r11,r28
	ctx.r27.u64 = ctx.r11.u64 + ctx.r28.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// bl 0x82a82fe8
	ctx.lr = 0x82A84478;
	sub_82A82FE8(ctx, base);
	// cmpwi cr6,r24,0
	ctx.cr6.compare<int32_t>(ctx.r24.s32, 0, ctx.xer);
	// blt cr6,0x82a84530
	if (ctx.cr6.lt) goto loc_82A84530;
	// cmpwi cr6,r31,4
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 4, ctx.xer);
	// ble cr6,0x82a844b4
	if (!ctx.cr6.gt) goto loc_82A844B4;
	// mr r7,r28
	ctx.r7.u64 = ctx.r28.u64;
	// mr r6,r29
	ctx.r6.u64 = ctx.r29.u64;
	// addi r5,r26,8
	ctx.r5.s64 = ctx.r26.s64 + 8;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a83c20
	ctx.lr = 0x82A8449C;
	sub_82A83C20(ctx, base);
	// mr r6,r27
	ctx.r6.u64 = ctx.r27.u64;
	// mr r5,r25
	ctx.r5.u64 = ctx.r25.u64;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// mr r3,r31
	ctx.r3.u64 = ctx.r31.u64;
	// bl 0x82a82e48
	ctx.lr = 0x82A844B0;
	sub_82A82E48(ctx, base);
	// b 0x82a844d0
	goto loc_82A844D0;
loc_82A844B4:
	// bne cr6,0x82a844d0
	if (!ctx.cr6.eq) goto loc_82A844D0;
	// mr r7,r28
	ctx.r7.u64 = ctx.r28.u64;
	// mr r6,r29
	ctx.r6.u64 = ctx.r29.u64;
	// addi r5,r26,8
	ctx.r5.s64 = ctx.r26.s64 + 8;
	// mr r4,r30
	ctx.r4.u64 = ctx.r30.u64;
	// li r3,4
	ctx.r3.s64 = 4;
	// bl 0x82a83c20
	ctx.lr = 0x82A844D0;
	sub_82A83C20(ctx, base);
loc_82A844D0:
	// lfs f0,0(r30)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// cmpwi cr6,r31,2
	ctx.cr6.compare<int32_t>(ctx.r31.s32, 2, ctx.xer);
	// lfs f13,4(r30)
	temp.u32 = REX_LOAD_U32(ctx.r30.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// fadds f12,f13,f0
	ctx.f12.f64 = double(float(ctx.f13.f64 + ctx.f0.f64));
	// stfs f12,0(r30)
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r30.u32 + 0, temp.u32);
	// fsubs f12,f0,f13
	ctx.f12.f64 = double(float(ctx.f0.f64 - ctx.f13.f64));
	// ble cr6,0x82a84524
	if (!ctx.cr6.gt) goto loc_82A84524;
	// addi r10,r31,-3
	ctx.r10.s64 = ctx.r31.s64 + -3;
	// addi r11,r30,8
	ctx.r11.s64 = ctx.r30.s64 + 8;
	// rlwinm r10,r10,31,1,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
loc_82A844FC:
	// lfs f0,0(r11)
	ctx.fpscr.disableFlushMode();
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 0);
	ctx.f0.f64 = double(temp.f32);
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
	// lfs f13,4(r11)
	temp.u32 = REX_LOAD_U32(ctx.r11.u32 + 4);
	ctx.f13.f64 = double(temp.f32);
	// fsubs f11,f0,f13
	ctx.f11.f64 = double(float(ctx.f0.f64 - ctx.f13.f64));
	// stfs f11,-4(r11)
	temp.f32 = float(ctx.f11.f64);
	REX_STORE_U32(ctx.r11.u32 + -4, temp.u32);
	// fadds f0,f13,f0
	ctx.f0.f64 = double(float(ctx.f13.f64 + ctx.f0.f64));
	// stfs f0,0(r11)
	temp.f32 = float(ctx.f0.f64);
	REX_STORE_U32(ctx.r11.u32 + 0, temp.u32);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// addi r11,r11,8
	ctx.r11.s64 = ctx.r11.s64 + 8;
	// bne cr6,0x82a844fc
	if (!ctx.cr6.eq) goto loc_82A844FC;
loc_82A84524:
	// rlwinm r11,r31,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 2) & 0xFFFFFFFC;
	// add r11,r11,r30
	ctx.r11.u64 = ctx.r11.u64 + ctx.r30.u64;
	// stfs f12,-4(r11)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f12.f64);
	REX_STORE_U32(ctx.r11.u32 + -4, temp.u32);
loc_82A84530:
	// addi r1,r1,160
	ctx.r1.s64 = ctx.r1.s64 + 160;
	// b 0x829ff808
	__restgprlr_24(ctx, base);
	return;
}

DEFINE_REX_FUNC(sub_82A84538) {
	REX_FUNC_PROLOGUE();
	// clrlwi r11,r4,16
	ctx.r11.u64 = ctx.r4.u32 & 0xFFFF;
	// rlwinm r10,r5,31,1,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 31) & 0x7FFFFFFF;
	// rlwinm r8,r11,16,0,15
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 16) & 0xFFFF0000;
	// mr r9,r3
	ctx.r9.u64 = ctx.r3.u64;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// or r8,r8,r11
	ctx.r8.u64 = ctx.r8.u64 | ctx.r11.u64;
	// beq cr6,0x82a84570
	if (ctx.cr6.eq) goto loc_82A84570;
	// mr r11,r3
	ctx.r11.u64 = ctx.r3.u64;
	// mtctr r10
	ctx.ctr.u64 = ctx.r10.u64;
loc_82A8455C:
	// stw r8,0(r11)
	REX_STORE_U32(ctx.r11.u32 + 0, ctx.r8.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// bdnz 0x82a8455c
	--ctx.ctr.u64;
	if (ctx.ctr.u32 != 0) goto loc_82A8455C;
	// rlwinm r11,r10,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 2) & 0xFFFFFFFC;
	// add r9,r11,r3
	ctx.r9.u64 = ctx.r11.u64 + ctx.r3.u64;
loc_82A84570:
	// clrlwi r11,r5,31
	ctx.r11.u64 = ctx.r5.u32 & 0x1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beqlr cr6
	if (ctx.cr6.eq) return;
	// sth r8,0(r9)
	REX_STORE_U16(ctx.r9.u32 + 0, ctx.r8.u16);
	// blr 
	return;
}

DEFINE_REX_FUNC(sub_82A84588) {
	REX_FUNC_PROLOGUE();
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7a8
	ctx.lr = 0x82A84590;
	__savegprlr_20(ctx, base);
	// mr r10,r5
	ctx.r10.u64 = ctx.r5.u64;
	// addi r11,r1,-368
	ctx.r11.s64 = ctx.r1.s64 + -368;
	// li r25,8
	ctx.r25.s64 = 8;
loc_82A8459C:
	// lhz r7,48(r10)
	ctx.r7.u64 = REX_LOAD_U16(ctx.r10.u32 + 48);
	// lhz r8,16(r10)
	ctx.r8.u64 = REX_LOAD_U16(ctx.r10.u32 + 16);
	// lhz r9,32(r10)
	ctx.r9.u64 = REX_LOAD_U16(ctx.r10.u32 + 32);
	// extsh r30,r7
	ctx.r30.s64 = ctx.r7.s16;
	// lhz r7,80(r10)
	ctx.r7.u64 = REX_LOAD_U16(ctx.r10.u32 + 80);
	// extsh r31,r8
	ctx.r31.s64 = ctx.r8.s16;
	// lhz r5,96(r10)
	ctx.r5.u64 = REX_LOAD_U16(ctx.r10.u32 + 96);
	// extsh r9,r9
	ctx.r9.s64 = ctx.r9.s16;
	// extsh r29,r7
	ctx.r29.s64 = ctx.r7.s16;
	// lhz r8,64(r10)
	ctx.r8.u64 = REX_LOAD_U16(ctx.r10.u32 + 64);
	// extsh r7,r5
	ctx.r7.s64 = ctx.r5.s16;
	// lhz r28,112(r10)
	ctx.r28.u64 = REX_LOAD_U16(ctx.r10.u32 + 112);
	// or r5,r31,r9
	ctx.r5.u64 = ctx.r31.u64 | ctx.r9.u64;
	// extsh r8,r8
	ctx.r8.s64 = ctx.r8.s16;
	// or r5,r5,r30
	ctx.r5.u64 = ctx.r5.u64 | ctx.r30.u64;
	// extsh r28,r28
	ctx.r28.s64 = ctx.r28.s16;
	// or r5,r5,r8
	ctx.r5.u64 = ctx.r5.u64 | ctx.r8.u64;
	// or r5,r5,r29
	ctx.r5.u64 = ctx.r5.u64 | ctx.r29.u64;
	// or r5,r5,r7
	ctx.r5.u64 = ctx.r5.u64 | ctx.r7.u64;
	// or r5,r5,r28
	ctx.r5.u64 = ctx.r5.u64 | ctx.r28.u64;
	// extsh r5,r5
	ctx.r5.s64 = ctx.r5.s16;
	// cmpwi cr6,r5,0
	ctx.cr6.compare<int32_t>(ctx.r5.s32, 0, ctx.xer);
	// bne cr6,0x82a84634
	if (!ctx.cr6.eq) goto loc_82A84634;
	// lhz r9,0(r10)
	ctx.r9.u64 = REX_LOAD_U16(ctx.r10.u32 + 0);
	// addi r10,r10,2
	ctx.r10.s64 = ctx.r10.s64 + 2;
	// lwz r8,0(r6)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r6.u32 + 0);
	// addi r6,r6,4
	ctx.r6.s64 = ctx.r6.s64 + 4;
	// extsh r9,r9
	ctx.r9.s64 = ctx.r9.s16;
	// mullw r9,r9,r8
	ctx.r9.s64 = int64_t(ctx.r9.s32) * int64_t(ctx.r8.s32);
	// srawi r9,r9,11
	ctx.xer.ca = (ctx.r9.s32 < 0) & ((ctx.r9.u32 & 0x7FF) != 0);
	ctx.r9.s64 = ctx.r9.s32 >> 11;
	// stw r9,0(r11)
	REX_STORE_U32(ctx.r11.u32 + 0, ctx.r9.u32);
	// stw r9,32(r11)
	REX_STORE_U32(ctx.r11.u32 + 32, ctx.r9.u32);
	// stw r9,64(r11)
	REX_STORE_U32(ctx.r11.u32 + 64, ctx.r9.u32);
	// stw r9,128(r11)
	REX_STORE_U32(ctx.r11.u32 + 128, ctx.r9.u32);
	// stw r9,160(r11)
	REX_STORE_U32(ctx.r11.u32 + 160, ctx.r9.u32);
	// stw r9,192(r11)
	REX_STORE_U32(ctx.r11.u32 + 192, ctx.r9.u32);
	// stw r9,224(r11)
	REX_STORE_U32(ctx.r11.u32 + 224, ctx.r9.u32);
	// b 0x82a8475c
	goto loc_82A8475C;
loc_82A84634:
	// lhz r5,0(r10)
	ctx.r5.u64 = REX_LOAD_U16(ctx.r10.u32 + 0);
	// addi r10,r10,2
	ctx.r10.s64 = ctx.r10.s64 + 2;
	// lwz r27,0(r6)
	ctx.r27.u64 = REX_LOAD_U32(ctx.r6.u32 + 0);
	// extsh r5,r5
	ctx.r5.s64 = ctx.r5.s16;
	// lwz r26,64(r6)
	ctx.r26.u64 = REX_LOAD_U32(ctx.r6.u32 + 64);
	// lwz r24,128(r6)
	ctx.r24.u64 = REX_LOAD_U32(ctx.r6.u32 + 128);
	// mullw r5,r5,r27
	ctx.r5.s64 = int64_t(ctx.r5.s32) * int64_t(ctx.r27.s32);
	// lwz r23,192(r6)
	ctx.r23.u64 = REX_LOAD_U32(ctx.r6.u32 + 192);
	// lwz r27,32(r6)
	ctx.r27.u64 = REX_LOAD_U32(ctx.r6.u32 + 32);
	// lwz r22,96(r6)
	ctx.r22.u64 = REX_LOAD_U32(ctx.r6.u32 + 96);
	// lwz r21,160(r6)
	ctx.r21.u64 = REX_LOAD_U32(ctx.r6.u32 + 160);
	// lwz r20,224(r6)
	ctx.r20.u64 = REX_LOAD_U32(ctx.r6.u32 + 224);
	// mullw r9,r26,r9
	ctx.r9.s64 = int64_t(ctx.r26.s32) * int64_t(ctx.r9.s32);
	// mullw r26,r24,r8
	ctx.r26.s64 = int64_t(ctx.r24.s32) * int64_t(ctx.r8.s32);
	// srawi r8,r5,11
	ctx.xer.ca = (ctx.r5.s32 < 0) & ((ctx.r5.u32 & 0x7FF) != 0);
	ctx.r8.s64 = ctx.r5.s32 >> 11;
	// mullw r5,r23,r7
	ctx.r5.s64 = int64_t(ctx.r23.s32) * int64_t(ctx.r7.s32);
	// srawi r9,r9,11
	ctx.xer.ca = (ctx.r9.s32 < 0) & ((ctx.r9.u32 & 0x7FF) != 0);
	ctx.r9.s64 = ctx.r9.s32 >> 11;
	// srawi r7,r26,11
	ctx.xer.ca = (ctx.r26.s32 < 0) & ((ctx.r26.u32 & 0x7FF) != 0);
	ctx.r7.s64 = ctx.r26.s32 >> 11;
	// srawi r5,r5,11
	ctx.xer.ca = (ctx.r5.s32 < 0) & ((ctx.r5.u32 & 0x7FF) != 0);
	ctx.r5.s64 = ctx.r5.s32 >> 11;
	// mullw r27,r27,r31
	ctx.r27.s64 = int64_t(ctx.r27.s32) * int64_t(ctx.r31.s32);
	// subf r26,r5,r9
	ctx.r26.u64 = ctx.r9.u64 - ctx.r5.u64;
	// add r9,r5,r9
	ctx.r9.u64 = ctx.r5.u64 + ctx.r9.u64;
	// mulli r5,r26,2896
	ctx.r5.s64 = static_cast<int64_t>(ctx.r26.u64 * static_cast<uint64_t>(2896));
	// mullw r30,r22,r30
	ctx.r30.s64 = int64_t(ctx.r22.s32) * int64_t(ctx.r30.s32);
	// srawi r26,r5,11
	ctx.xer.ca = (ctx.r5.s32 < 0) & ((ctx.r5.u32 & 0x7FF) != 0);
	ctx.r26.s64 = ctx.r5.s32 >> 11;
	// mullw r29,r21,r29
	ctx.r29.s64 = int64_t(ctx.r21.s32) * int64_t(ctx.r29.s32);
	// add r31,r7,r8
	ctx.r31.u64 = ctx.r7.u64 + ctx.r8.u64;
	// mullw r28,r20,r28
	ctx.r28.s64 = int64_t(ctx.r20.s32) * int64_t(ctx.r28.s32);
	// srawi r5,r27,11
	ctx.xer.ca = (ctx.r27.s32 < 0) & ((ctx.r27.u32 & 0x7FF) != 0);
	ctx.r5.s64 = ctx.r27.s32 >> 11;
	// srawi r30,r30,11
	ctx.xer.ca = (ctx.r30.s32 < 0) & ((ctx.r30.u32 & 0x7FF) != 0);
	ctx.r30.s64 = ctx.r30.s32 >> 11;
	// srawi r29,r29,11
	ctx.xer.ca = (ctx.r29.s32 < 0) & ((ctx.r29.u32 & 0x7FF) != 0);
	ctx.r29.s64 = ctx.r29.s32 >> 11;
	// subf r8,r7,r8
	ctx.r8.u64 = ctx.r8.u64 - ctx.r7.u64;
	// add r27,r9,r31
	ctx.r27.u64 = ctx.r9.u64 + ctx.r31.u64;
	// srawi r28,r28,11
	ctx.xer.ca = (ctx.r28.s32 < 0) & ((ctx.r28.u32 & 0x7FF) != 0);
	ctx.r28.s64 = ctx.r28.s32 >> 11;
	// subf r7,r9,r26
	ctx.r7.u64 = ctx.r26.u64 - ctx.r9.u64;
	// subf r31,r9,r31
	ctx.r31.u64 = ctx.r31.u64 - ctx.r9.u64;
	// subf r9,r30,r29
	ctx.r9.u64 = ctx.r29.u64 - ctx.r30.u64;
	// subf r26,r28,r5
	ctx.r26.u64 = ctx.r5.u64 - ctx.r28.u64;
	// add r30,r29,r30
	ctx.r30.u64 = ctx.r29.u64 + ctx.r30.u64;
	// add r29,r7,r8
	ctx.r29.u64 = ctx.r7.u64 + ctx.r8.u64;
	// add r5,r28,r5
	ctx.r5.u64 = ctx.r28.u64 + ctx.r5.u64;
	// subf r7,r7,r8
	ctx.r7.u64 = ctx.r8.u64 - ctx.r7.u64;
	// add r8,r26,r9
	ctx.r8.u64 = ctx.r26.u64 + ctx.r9.u64;
	// mulli r28,r9,-5352
	ctx.r28.s64 = static_cast<int64_t>(ctx.r9.u64 * static_cast<uint64_t>(-5352));
	// add r9,r5,r30
	ctx.r9.u64 = ctx.r5.u64 + ctx.r30.u64;
	// subf r5,r30,r5
	ctx.r5.u64 = ctx.r5.u64 - ctx.r30.u64;
	// mulli r8,r8,3784
	ctx.r8.s64 = static_cast<int64_t>(ctx.r8.u64 * static_cast<uint64_t>(3784));
	// srawi r8,r8,11
	ctx.xer.ca = (ctx.r8.s32 < 0) & ((ctx.r8.u32 & 0x7FF) != 0);
	ctx.r8.s64 = ctx.r8.s32 >> 11;
	// mulli r5,r5,2896
	ctx.r5.s64 = static_cast<int64_t>(ctx.r5.u64 * static_cast<uint64_t>(2896));
	// srawi r28,r28,11
	ctx.xer.ca = (ctx.r28.s32 < 0) & ((ctx.r28.u32 & 0x7FF) != 0);
	ctx.r28.s64 = ctx.r28.s32 >> 11;
	// mulli r30,r26,2217
	ctx.r30.s64 = static_cast<int64_t>(ctx.r26.u64 * static_cast<uint64_t>(2217));
	// srawi r26,r5,11
	ctx.xer.ca = (ctx.r5.s32 < 0) & ((ctx.r5.u32 & 0x7FF) != 0);
	ctx.r26.s64 = ctx.r5.s32 >> 11;
	// subf r5,r9,r28
	ctx.r5.u64 = ctx.r28.u64 - ctx.r9.u64;
	// add r28,r9,r27
	ctx.r28.u64 = ctx.r9.u64 + ctx.r27.u64;
	// subf r9,r9,r27
	ctx.r9.u64 = ctx.r27.u64 - ctx.r9.u64;
	// srawi r30,r30,11
	ctx.xer.ca = (ctx.r30.s32 < 0) & ((ctx.r30.u32 & 0x7FF) != 0);
	ctx.r30.s64 = ctx.r30.s32 >> 11;
	// addi r6,r6,4
	ctx.r6.s64 = ctx.r6.s64 + 4;
	// stw r28,0(r11)
	REX_STORE_U32(ctx.r11.u32 + 0, ctx.r28.u32);
	// stw r9,224(r11)
	REX_STORE_U32(ctx.r11.u32 + 224, ctx.r9.u32);
	// add r9,r5,r8
	ctx.r9.u64 = ctx.r5.u64 + ctx.r8.u64;
	// subf r5,r8,r30
	ctx.r5.u64 = ctx.r30.u64 - ctx.r8.u64;
	// subf r8,r9,r26
	ctx.r8.u64 = ctx.r26.u64 - ctx.r9.u64;
	// add r30,r9,r29
	ctx.r30.u64 = ctx.r9.u64 + ctx.r29.u64;
	// subf r9,r9,r29
	ctx.r9.u64 = ctx.r29.u64 - ctx.r9.u64;
	// stw r30,32(r11)
	REX_STORE_U32(ctx.r11.u32 + 32, ctx.r30.u32);
	// stw r9,192(r11)
	REX_STORE_U32(ctx.r11.u32 + 192, ctx.r9.u32);
	// add r9,r5,r8
	ctx.r9.u64 = ctx.r5.u64 + ctx.r8.u64;
	// add r5,r8,r7
	ctx.r5.u64 = ctx.r8.u64 + ctx.r7.u64;
	// subf r8,r8,r7
	ctx.r8.u64 = ctx.r7.u64 - ctx.r8.u64;
	// stw r5,64(r11)
	REX_STORE_U32(ctx.r11.u32 + 64, ctx.r5.u32);
	// stw r8,160(r11)
	REX_STORE_U32(ctx.r11.u32 + 160, ctx.r8.u32);
	// add r8,r9,r31
	ctx.r8.u64 = ctx.r9.u64 + ctx.r31.u64;
	// subf r9,r9,r31
	ctx.r9.u64 = ctx.r31.u64 - ctx.r9.u64;
	// stw r8,128(r11)
	REX_STORE_U32(ctx.r11.u32 + 128, ctx.r8.u32);
loc_82A8475C:
	// addi r25,r25,-1
	ctx.r25.s64 = ctx.r25.s64 + -1;
	// stw r9,96(r11)
	REX_STORE_U32(ctx.r11.u32 + 96, ctx.r9.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// cmpwi cr6,r25,0
	ctx.cr6.compare<int32_t>(ctx.r25.s32, 0, ctx.xer);
	// bgt cr6,0x82a8459c
	if (ctx.cr6.gt) goto loc_82A8459C;
	// addi r10,r3,1
	ctx.r10.s64 = ctx.r3.s64 + 1;
	// addi r11,r1,-344
	ctx.r11.s64 = ctx.r1.s64 + -344;
	// li r7,8
	ctx.r7.s64 = 8;
loc_82A8477C:
	// lwz r9,-4(r11)
	ctx.r9.u64 = REX_LOAD_U32(ctx.r11.u32 + -4);
	// addi r7,r7,-1
	ctx.r7.s64 = ctx.r7.s64 + -1;
	// lwz r8,-12(r11)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r11.u32 + -12);
	// lwz r5,-20(r11)
	ctx.r5.u64 = REX_LOAD_U32(ctx.r11.u32 + -20);
	// cmplwi cr6,r7,0
	ctx.cr6.compare<uint32_t>(ctx.r7.u32, 0, ctx.xer);
	// lwz r6,4(r11)
	ctx.r6.u64 = REX_LOAD_U32(ctx.r11.u32 + 4);
	// subf r28,r8,r9
	ctx.r28.u64 = ctx.r9.u64 - ctx.r8.u64;
	// lwz r3,0(r11)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r11.u32 + 0);
	// add r27,r8,r9
	ctx.r27.u64 = ctx.r8.u64 + ctx.r9.u64;
	// lwz r31,-16(r11)
	ctx.r31.u64 = REX_LOAD_U32(ctx.r11.u32 + -16);
	// subf r26,r6,r5
	ctx.r26.u64 = ctx.r5.u64 - ctx.r6.u64;
	// lwz r30,-8(r11)
	ctx.r30.u64 = REX_LOAD_U32(ctx.r11.u32 + -8);
	// add r6,r5,r6
	ctx.r6.u64 = ctx.r5.u64 + ctx.r6.u64;
	// add r9,r3,r31
	ctx.r9.u64 = ctx.r3.u64 + ctx.r31.u64;
	// lwz r29,-24(r11)
	ctx.r29.u64 = REX_LOAD_U32(ctx.r11.u32 + -24);
	// subf r8,r3,r31
	ctx.r8.u64 = ctx.r31.u64 - ctx.r3.u64;
	// add r31,r26,r28
	ctx.r31.u64 = ctx.r26.u64 + ctx.r28.u64;
	// mulli r8,r8,2896
	ctx.r8.s64 = static_cast<int64_t>(ctx.r8.u64 * static_cast<uint64_t>(2896));
	// subf r3,r30,r29
	ctx.r3.u64 = ctx.r29.u64 - ctx.r30.u64;
	// add r5,r29,r30
	ctx.r5.u64 = ctx.r29.u64 + ctx.r30.u64;
	// mulli r31,r31,3784
	ctx.r31.s64 = static_cast<int64_t>(ctx.r31.u64 * static_cast<uint64_t>(3784));
	// mulli r30,r28,-5352
	ctx.r30.s64 = static_cast<int64_t>(ctx.r28.u64 * static_cast<uint64_t>(-5352));
	// srawi r25,r8,11
	ctx.xer.ca = (ctx.r8.s32 < 0) & ((ctx.r8.u32 & 0x7FF) != 0);
	ctx.r25.s64 = ctx.r8.s32 >> 11;
	// srawi r31,r31,11
	ctx.xer.ca = (ctx.r31.s32 < 0) & ((ctx.r31.u32 & 0x7FF) != 0);
	ctx.r31.s64 = ctx.r31.s32 >> 11;
	// srawi r29,r30,11
	ctx.xer.ca = (ctx.r30.s32 < 0) & ((ctx.r30.u32 & 0x7FF) != 0);
	ctx.r29.s64 = ctx.r30.s32 >> 11;
	// subf r30,r27,r6
	ctx.r30.u64 = ctx.r6.u64 - ctx.r27.u64;
	// add r8,r6,r27
	ctx.r8.u64 = ctx.r6.u64 + ctx.r27.u64;
	// mulli r30,r30,2896
	ctx.r30.s64 = static_cast<int64_t>(ctx.r30.u64 * static_cast<uint64_t>(2896));
	// srawi r28,r30,11
	ctx.xer.ca = (ctx.r30.s32 < 0) & ((ctx.r30.u32 & 0x7FF) != 0);
	ctx.r28.s64 = ctx.r30.s32 >> 11;
	// mulli r27,r26,2217
	ctx.r27.s64 = static_cast<int64_t>(ctx.r26.u64 * static_cast<uint64_t>(2217));
	// add r30,r9,r5
	ctx.r30.u64 = ctx.r9.u64 + ctx.r5.u64;
	// subf r6,r9,r25
	ctx.r6.u64 = ctx.r25.u64 - ctx.r9.u64;
	// subf r5,r9,r5
	ctx.r5.u64 = ctx.r5.u64 - ctx.r9.u64;
	// subf r9,r8,r29
	ctx.r9.u64 = ctx.r29.u64 - ctx.r8.u64;
	// srawi r27,r27,11
	ctx.xer.ca = (ctx.r27.s32 < 0) & ((ctx.r27.u32 & 0x7FF) != 0);
	ctx.r27.s64 = ctx.r27.s32 >> 11;
	// add r9,r9,r31
	ctx.r9.u64 = ctx.r9.u64 + ctx.r31.u64;
	// subf r29,r31,r27
	ctx.r29.u64 = ctx.r27.u64 - ctx.r31.u64;
	// add r31,r6,r3
	ctx.r31.u64 = ctx.r6.u64 + ctx.r3.u64;
	// subf r6,r6,r3
	ctx.r6.u64 = ctx.r3.u64 - ctx.r6.u64;
	// add r3,r8,r30
	ctx.r3.u64 = ctx.r8.u64 + ctx.r30.u64;
	// subf r8,r8,r30
	ctx.r8.u64 = ctx.r30.u64 - ctx.r8.u64;
	// addi r3,r3,127
	ctx.r3.s64 = ctx.r3.s64 + 127;
	// addi r8,r8,127
	ctx.r8.s64 = ctx.r8.s64 + 127;
	// srawi r3,r3,8
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0xFF) != 0);
	ctx.r3.s64 = ctx.r3.s32 >> 8;
	// srawi r8,r8,8
	ctx.xer.ca = (ctx.r8.s32 < 0) & ((ctx.r8.u32 & 0xFF) != 0);
	ctx.r8.s64 = ctx.r8.s32 >> 8;
	// addi r11,r11,32
	ctx.r11.s64 = ctx.r11.s64 + 32;
	// stb r3,-1(r10)
	REX_STORE_U8(ctx.r10.u32 + -1, ctx.r3.u8);
	// add r3,r9,r31
	ctx.r3.u64 = ctx.r9.u64 + ctx.r31.u64;
	// stb r8,6(r10)
	REX_STORE_U8(ctx.r10.u32 + 6, ctx.r8.u8);
	// subf r8,r9,r28
	ctx.r8.u64 = ctx.r28.u64 - ctx.r9.u64;
	// subf r9,r9,r31
	ctx.r9.u64 = ctx.r31.u64 - ctx.r9.u64;
	// addi r3,r3,127
	ctx.r3.s64 = ctx.r3.s64 + 127;
	// addi r9,r9,127
	ctx.r9.s64 = ctx.r9.s64 + 127;
	// srawi r3,r3,8
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0xFF) != 0);
	ctx.r3.s64 = ctx.r3.s32 >> 8;
	// srawi r9,r9,8
	ctx.xer.ca = (ctx.r9.s32 < 0) & ((ctx.r9.u32 & 0xFF) != 0);
	ctx.r9.s64 = ctx.r9.s32 >> 8;
	// stb r3,0(r10)
	REX_STORE_U8(ctx.r10.u32 + 0, ctx.r3.u8);
	// add r3,r8,r6
	ctx.r3.u64 = ctx.r8.u64 + ctx.r6.u64;
	// stb r9,5(r10)
	REX_STORE_U8(ctx.r10.u32 + 5, ctx.r9.u8);
	// add r9,r29,r8
	ctx.r9.u64 = ctx.r29.u64 + ctx.r8.u64;
	// subf r8,r8,r6
	ctx.r8.u64 = ctx.r6.u64 - ctx.r8.u64;
	// addi r6,r3,127
	ctx.r6.s64 = ctx.r3.s64 + 127;
	// addi r8,r8,127
	ctx.r8.s64 = ctx.r8.s64 + 127;
	// srawi r6,r6,8
	ctx.xer.ca = (ctx.r6.s32 < 0) & ((ctx.r6.u32 & 0xFF) != 0);
	ctx.r6.s64 = ctx.r6.s32 >> 8;
	// srawi r8,r8,8
	ctx.xer.ca = (ctx.r8.s32 < 0) & ((ctx.r8.u32 & 0xFF) != 0);
	ctx.r8.s64 = ctx.r8.s32 >> 8;
	// stb r6,1(r10)
	REX_STORE_U8(ctx.r10.u32 + 1, ctx.r6.u8);
	// stb r8,4(r10)
	REX_STORE_U8(ctx.r10.u32 + 4, ctx.r8.u8);
	// add r8,r9,r5
	ctx.r8.u64 = ctx.r9.u64 + ctx.r5.u64;
	// subf r9,r9,r5
	ctx.r9.u64 = ctx.r5.u64 - ctx.r9.u64;
	// addi r8,r8,127
	ctx.r8.s64 = ctx.r8.s64 + 127;
	// addi r9,r9,127
	ctx.r9.s64 = ctx.r9.s64 + 127;
	// srawi r8,r8,8
	ctx.xer.ca = (ctx.r8.s32 < 0) & ((ctx.r8.u32 & 0xFF) != 0);
	ctx.r8.s64 = ctx.r8.s32 >> 8;
	// srawi r9,r9,8
	ctx.xer.ca = (ctx.r9.s32 < 0) & ((ctx.r9.u32 & 0xFF) != 0);
	ctx.r9.s64 = ctx.r9.s32 >> 8;
	// stb r8,3(r10)
	REX_STORE_U8(ctx.r10.u32 + 3, ctx.r8.u8);
	// stb r9,2(r10)
	REX_STORE_U8(ctx.r10.u32 + 2, ctx.r9.u8);
	// add r10,r10,r4
	ctx.r10.u64 = ctx.r10.u64 + ctx.r4.u64;
	// bne cr6,0x82a8477c
	if (!ctx.cr6.eq) goto loc_82A8477C;
	// b 0x829ff7f8
	__restgprlr_20(ctx, base);
	return;
}

DEFINE_REX_FUNC(sub_82A848B0) {
	REX_FUNC_PROLOGUE();
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7a4
	ctx.lr = 0x82A848B8;
	__savegprlr_19(ctx, base);
	// mr r10,r5
	ctx.r10.u64 = ctx.r5.u64;
	// add r9,r3,r4
	ctx.r9.u64 = ctx.r3.u64 + ctx.r4.u64;
	// rlwinm r25,r4,1,0,30
	ctx.r25.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 1) & 0xFFFFFFFE;
	// addi r11,r1,-368
	ctx.r11.s64 = ctx.r1.s64 + -368;
	// li r24,8
	ctx.r24.s64 = 8;
loc_82A848CC:
	// lhz r5,48(r10)
	ctx.r5.u64 = REX_LOAD_U16(ctx.r10.u32 + 48);
	// lhz r7,16(r10)
	ctx.r7.u64 = REX_LOAD_U16(ctx.r10.u32 + 16);
	// lhz r8,32(r10)
	ctx.r8.u64 = REX_LOAD_U16(ctx.r10.u32 + 32);
	// extsh r30,r5
	ctx.r30.s64 = ctx.r5.s16;
	// lhz r5,80(r10)
	ctx.r5.u64 = REX_LOAD_U16(ctx.r10.u32 + 80);
	// extsh r31,r7
	ctx.r31.s64 = ctx.r7.s16;
	// lhz r4,96(r10)
	ctx.r4.u64 = REX_LOAD_U16(ctx.r10.u32 + 96);
	// extsh r8,r8
	ctx.r8.s64 = ctx.r8.s16;
	// extsh r29,r5
	ctx.r29.s64 = ctx.r5.s16;
	// lhz r7,64(r10)
	ctx.r7.u64 = REX_LOAD_U16(ctx.r10.u32 + 64);
	// extsh r5,r4
	ctx.r5.s64 = ctx.r4.s16;
	// lhz r28,112(r10)
	ctx.r28.u64 = REX_LOAD_U16(ctx.r10.u32 + 112);
	// or r4,r31,r8
	ctx.r4.u64 = ctx.r31.u64 | ctx.r8.u64;
	// extsh r7,r7
	ctx.r7.s64 = ctx.r7.s16;
	// or r4,r4,r30
	ctx.r4.u64 = ctx.r4.u64 | ctx.r30.u64;
	// extsh r28,r28
	ctx.r28.s64 = ctx.r28.s16;
	// or r4,r4,r7
	ctx.r4.u64 = ctx.r4.u64 | ctx.r7.u64;
	// or r4,r4,r29
	ctx.r4.u64 = ctx.r4.u64 | ctx.r29.u64;
	// or r4,r4,r5
	ctx.r4.u64 = ctx.r4.u64 | ctx.r5.u64;
	// or r4,r4,r28
	ctx.r4.u64 = ctx.r4.u64 | ctx.r28.u64;
	// extsh r4,r4
	ctx.r4.s64 = ctx.r4.s16;
	// cmpwi cr6,r4,0
	ctx.cr6.compare<int32_t>(ctx.r4.s32, 0, ctx.xer);
	// bne cr6,0x82a84964
	if (!ctx.cr6.eq) goto loc_82A84964;
	// lhz r8,0(r10)
	ctx.r8.u64 = REX_LOAD_U16(ctx.r10.u32 + 0);
	// addi r10,r10,2
	ctx.r10.s64 = ctx.r10.s64 + 2;
	// lwz r7,0(r6)
	ctx.r7.u64 = REX_LOAD_U32(ctx.r6.u32 + 0);
	// addi r6,r6,4
	ctx.r6.s64 = ctx.r6.s64 + 4;
	// extsh r8,r8
	ctx.r8.s64 = ctx.r8.s16;
	// mullw r8,r8,r7
	ctx.r8.s64 = int64_t(ctx.r8.s32) * int64_t(ctx.r7.s32);
	// srawi r8,r8,11
	ctx.xer.ca = (ctx.r8.s32 < 0) & ((ctx.r8.u32 & 0x7FF) != 0);
	ctx.r8.s64 = ctx.r8.s32 >> 11;
	// stw r8,0(r11)
	REX_STORE_U32(ctx.r11.u32 + 0, ctx.r8.u32);
	// stw r8,32(r11)
	REX_STORE_U32(ctx.r11.u32 + 32, ctx.r8.u32);
	// stw r8,64(r11)
	REX_STORE_U32(ctx.r11.u32 + 64, ctx.r8.u32);
	// stw r8,128(r11)
	REX_STORE_U32(ctx.r11.u32 + 128, ctx.r8.u32);
	// stw r8,160(r11)
	REX_STORE_U32(ctx.r11.u32 + 160, ctx.r8.u32);
	// stw r8,192(r11)
	REX_STORE_U32(ctx.r11.u32 + 192, ctx.r8.u32);
	// stw r8,224(r11)
	REX_STORE_U32(ctx.r11.u32 + 224, ctx.r8.u32);
	// b 0x82a84a8c
	goto loc_82A84A8C;
loc_82A84964:
	// lhz r4,0(r10)
	ctx.r4.u64 = REX_LOAD_U16(ctx.r10.u32 + 0);
	// addi r10,r10,2
	ctx.r10.s64 = ctx.r10.s64 + 2;
	// lwz r27,0(r6)
	ctx.r27.u64 = REX_LOAD_U32(ctx.r6.u32 + 0);
	// extsh r4,r4
	ctx.r4.s64 = ctx.r4.s16;
	// lwz r26,64(r6)
	ctx.r26.u64 = REX_LOAD_U32(ctx.r6.u32 + 64);
	// lwz r23,128(r6)
	ctx.r23.u64 = REX_LOAD_U32(ctx.r6.u32 + 128);
	// mullw r4,r4,r27
	ctx.r4.s64 = int64_t(ctx.r4.s32) * int64_t(ctx.r27.s32);
	// lwz r22,192(r6)
	ctx.r22.u64 = REX_LOAD_U32(ctx.r6.u32 + 192);
	// lwz r27,32(r6)
	ctx.r27.u64 = REX_LOAD_U32(ctx.r6.u32 + 32);
	// lwz r21,96(r6)
	ctx.r21.u64 = REX_LOAD_U32(ctx.r6.u32 + 96);
	// lwz r20,160(r6)
	ctx.r20.u64 = REX_LOAD_U32(ctx.r6.u32 + 160);
	// lwz r19,224(r6)
	ctx.r19.u64 = REX_LOAD_U32(ctx.r6.u32 + 224);
	// mullw r8,r26,r8
	ctx.r8.s64 = int64_t(ctx.r26.s32) * int64_t(ctx.r8.s32);
	// mullw r26,r23,r7
	ctx.r26.s64 = int64_t(ctx.r23.s32) * int64_t(ctx.r7.s32);
	// srawi r7,r4,11
	ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0x7FF) != 0);
	ctx.r7.s64 = ctx.r4.s32 >> 11;
	// mullw r4,r22,r5
	ctx.r4.s64 = int64_t(ctx.r22.s32) * int64_t(ctx.r5.s32);
	// srawi r8,r8,11
	ctx.xer.ca = (ctx.r8.s32 < 0) & ((ctx.r8.u32 & 0x7FF) != 0);
	ctx.r8.s64 = ctx.r8.s32 >> 11;
	// srawi r5,r26,11
	ctx.xer.ca = (ctx.r26.s32 < 0) & ((ctx.r26.u32 & 0x7FF) != 0);
	ctx.r5.s64 = ctx.r26.s32 >> 11;
	// srawi r4,r4,11
	ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0x7FF) != 0);
	ctx.r4.s64 = ctx.r4.s32 >> 11;
	// mullw r27,r27,r31
	ctx.r27.s64 = int64_t(ctx.r27.s32) * int64_t(ctx.r31.s32);
	// subf r26,r4,r8
	ctx.r26.u64 = ctx.r8.u64 - ctx.r4.u64;
	// add r8,r4,r8
	ctx.r8.u64 = ctx.r4.u64 + ctx.r8.u64;
	// mulli r4,r26,2896
	ctx.r4.s64 = static_cast<int64_t>(ctx.r26.u64 * static_cast<uint64_t>(2896));
	// mullw r30,r21,r30
	ctx.r30.s64 = int64_t(ctx.r21.s32) * int64_t(ctx.r30.s32);
	// srawi r26,r4,11
	ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0x7FF) != 0);
	ctx.r26.s64 = ctx.r4.s32 >> 11;
	// mullw r29,r20,r29
	ctx.r29.s64 = int64_t(ctx.r20.s32) * int64_t(ctx.r29.s32);
	// add r31,r5,r7
	ctx.r31.u64 = ctx.r5.u64 + ctx.r7.u64;
	// mullw r28,r19,r28
	ctx.r28.s64 = int64_t(ctx.r19.s32) * int64_t(ctx.r28.s32);
	// srawi r4,r27,11
	ctx.xer.ca = (ctx.r27.s32 < 0) & ((ctx.r27.u32 & 0x7FF) != 0);
	ctx.r4.s64 = ctx.r27.s32 >> 11;
	// srawi r30,r30,11
	ctx.xer.ca = (ctx.r30.s32 < 0) & ((ctx.r30.u32 & 0x7FF) != 0);
	ctx.r30.s64 = ctx.r30.s32 >> 11;
	// srawi r29,r29,11
	ctx.xer.ca = (ctx.r29.s32 < 0) & ((ctx.r29.u32 & 0x7FF) != 0);
	ctx.r29.s64 = ctx.r29.s32 >> 11;
	// subf r7,r5,r7
	ctx.r7.u64 = ctx.r7.u64 - ctx.r5.u64;
	// add r27,r8,r31
	ctx.r27.u64 = ctx.r8.u64 + ctx.r31.u64;
	// srawi r28,r28,11
	ctx.xer.ca = (ctx.r28.s32 < 0) & ((ctx.r28.u32 & 0x7FF) != 0);
	ctx.r28.s64 = ctx.r28.s32 >> 11;
	// subf r5,r8,r26
	ctx.r5.u64 = ctx.r26.u64 - ctx.r8.u64;
	// subf r31,r8,r31
	ctx.r31.u64 = ctx.r31.u64 - ctx.r8.u64;
	// subf r8,r30,r29
	ctx.r8.u64 = ctx.r29.u64 - ctx.r30.u64;
	// subf r26,r28,r4
	ctx.r26.u64 = ctx.r4.u64 - ctx.r28.u64;
	// add r30,r29,r30
	ctx.r30.u64 = ctx.r29.u64 + ctx.r30.u64;
	// add r29,r5,r7
	ctx.r29.u64 = ctx.r5.u64 + ctx.r7.u64;
	// add r4,r28,r4
	ctx.r4.u64 = ctx.r28.u64 + ctx.r4.u64;
	// subf r5,r5,r7
	ctx.r5.u64 = ctx.r7.u64 - ctx.r5.u64;
	// add r7,r26,r8
	ctx.r7.u64 = ctx.r26.u64 + ctx.r8.u64;
	// mulli r28,r8,-5352
	ctx.r28.s64 = static_cast<int64_t>(ctx.r8.u64 * static_cast<uint64_t>(-5352));
	// add r8,r4,r30
	ctx.r8.u64 = ctx.r4.u64 + ctx.r30.u64;
	// subf r4,r30,r4
	ctx.r4.u64 = ctx.r4.u64 - ctx.r30.u64;
	// mulli r7,r7,3784
	ctx.r7.s64 = static_cast<int64_t>(ctx.r7.u64 * static_cast<uint64_t>(3784));
	// srawi r7,r7,11
	ctx.xer.ca = (ctx.r7.s32 < 0) & ((ctx.r7.u32 & 0x7FF) != 0);
	ctx.r7.s64 = ctx.r7.s32 >> 11;
	// mulli r4,r4,2896
	ctx.r4.s64 = static_cast<int64_t>(ctx.r4.u64 * static_cast<uint64_t>(2896));
	// srawi r28,r28,11
	ctx.xer.ca = (ctx.r28.s32 < 0) & ((ctx.r28.u32 & 0x7FF) != 0);
	ctx.r28.s64 = ctx.r28.s32 >> 11;
	// mulli r30,r26,2217
	ctx.r30.s64 = static_cast<int64_t>(ctx.r26.u64 * static_cast<uint64_t>(2217));
	// srawi r26,r4,11
	ctx.xer.ca = (ctx.r4.s32 < 0) & ((ctx.r4.u32 & 0x7FF) != 0);
	ctx.r26.s64 = ctx.r4.s32 >> 11;
	// subf r4,r8,r28
	ctx.r4.u64 = ctx.r28.u64 - ctx.r8.u64;
	// add r28,r8,r27
	ctx.r28.u64 = ctx.r8.u64 + ctx.r27.u64;
	// subf r8,r8,r27
	ctx.r8.u64 = ctx.r27.u64 - ctx.r8.u64;
	// srawi r30,r30,11
	ctx.xer.ca = (ctx.r30.s32 < 0) & ((ctx.r30.u32 & 0x7FF) != 0);
	ctx.r30.s64 = ctx.r30.s32 >> 11;
	// addi r6,r6,4
	ctx.r6.s64 = ctx.r6.s64 + 4;
	// stw r28,0(r11)
	REX_STORE_U32(ctx.r11.u32 + 0, ctx.r28.u32);
	// stw r8,224(r11)
	REX_STORE_U32(ctx.r11.u32 + 224, ctx.r8.u32);
	// add r8,r4,r7
	ctx.r8.u64 = ctx.r4.u64 + ctx.r7.u64;
	// subf r4,r7,r30
	ctx.r4.u64 = ctx.r30.u64 - ctx.r7.u64;
	// subf r7,r8,r26
	ctx.r7.u64 = ctx.r26.u64 - ctx.r8.u64;
	// add r30,r8,r29
	ctx.r30.u64 = ctx.r8.u64 + ctx.r29.u64;
	// subf r8,r8,r29
	ctx.r8.u64 = ctx.r29.u64 - ctx.r8.u64;
	// stw r30,32(r11)
	REX_STORE_U32(ctx.r11.u32 + 32, ctx.r30.u32);
	// stw r8,192(r11)
	REX_STORE_U32(ctx.r11.u32 + 192, ctx.r8.u32);
	// add r8,r4,r7
	ctx.r8.u64 = ctx.r4.u64 + ctx.r7.u64;
	// add r4,r7,r5
	ctx.r4.u64 = ctx.r7.u64 + ctx.r5.u64;
	// subf r7,r7,r5
	ctx.r7.u64 = ctx.r5.u64 - ctx.r7.u64;
	// stw r4,64(r11)
	REX_STORE_U32(ctx.r11.u32 + 64, ctx.r4.u32);
	// stw r7,160(r11)
	REX_STORE_U32(ctx.r11.u32 + 160, ctx.r7.u32);
	// add r7,r8,r31
	ctx.r7.u64 = ctx.r8.u64 + ctx.r31.u64;
	// subf r8,r8,r31
	ctx.r8.u64 = ctx.r31.u64 - ctx.r8.u64;
	// stw r7,128(r11)
	REX_STORE_U32(ctx.r11.u32 + 128, ctx.r7.u32);
loc_82A84A8C:
	// addi r24,r24,-1
	ctx.r24.s64 = ctx.r24.s64 + -1;
	// stw r8,96(r11)
	REX_STORE_U32(ctx.r11.u32 + 96, ctx.r8.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// cmpwi cr6,r24,0
	ctx.cr6.compare<int32_t>(ctx.r24.s32, 0, ctx.xer);
	// bgt cr6,0x82a848cc
	if (ctx.cr6.gt) goto loc_82A848CC;
	// addi r11,r1,-344
	ctx.r11.s64 = ctx.r1.s64 + -344;
	// addi r10,r3,8
	ctx.r10.s64 = ctx.r3.s64 + 8;
	// li r6,8
	ctx.r6.s64 = 8;
loc_82A84AAC:
	// lwz r8,-4(r11)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r11.u32 + -4);
	// lwz r7,-12(r11)
	ctx.r7.u64 = REX_LOAD_U32(ctx.r11.u32 + -12);
	// lwz r4,-20(r11)
	ctx.r4.u64 = REX_LOAD_U32(ctx.r11.u32 + -20);
	// lwz r5,4(r11)
	ctx.r5.u64 = REX_LOAD_U32(ctx.r11.u32 + 4);
	// subf r28,r7,r8
	ctx.r28.u64 = ctx.r8.u64 - ctx.r7.u64;
	// lwz r3,0(r11)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r11.u32 + 0);
	// add r27,r7,r8
	ctx.r27.u64 = ctx.r7.u64 + ctx.r8.u64;
	// lwz r31,-16(r11)
	ctx.r31.u64 = REX_LOAD_U32(ctx.r11.u32 + -16);
	// subf r26,r5,r4
	ctx.r26.u64 = ctx.r4.u64 - ctx.r5.u64;
	// lwz r30,-8(r11)
	ctx.r30.u64 = REX_LOAD_U32(ctx.r11.u32 + -8);
	// add r5,r4,r5
	ctx.r5.u64 = ctx.r4.u64 + ctx.r5.u64;
	// add r8,r31,r3
	ctx.r8.u64 = ctx.r31.u64 + ctx.r3.u64;
	// lwz r29,-24(r11)
	ctx.r29.u64 = REX_LOAD_U32(ctx.r11.u32 + -24);
	// subf r7,r3,r31
	ctx.r7.u64 = ctx.r31.u64 - ctx.r3.u64;
	// add r31,r26,r28
	ctx.r31.u64 = ctx.r26.u64 + ctx.r28.u64;
	// mulli r7,r7,2896
	ctx.r7.s64 = static_cast<int64_t>(ctx.r7.u64 * static_cast<uint64_t>(2896));
	// subf r3,r30,r29
	ctx.r3.u64 = ctx.r29.u64 - ctx.r30.u64;
	// add r4,r29,r30
	ctx.r4.u64 = ctx.r29.u64 + ctx.r30.u64;
	// mulli r31,r31,3784
	ctx.r31.s64 = static_cast<int64_t>(ctx.r31.u64 * static_cast<uint64_t>(3784));
	// mulli r30,r28,-5352
	ctx.r30.s64 = static_cast<int64_t>(ctx.r28.u64 * static_cast<uint64_t>(-5352));
	// srawi r24,r7,11
	ctx.xer.ca = (ctx.r7.s32 < 0) & ((ctx.r7.u32 & 0x7FF) != 0);
	ctx.r24.s64 = ctx.r7.s32 >> 11;
	// srawi r31,r31,11
	ctx.xer.ca = (ctx.r31.s32 < 0) & ((ctx.r31.u32 & 0x7FF) != 0);
	ctx.r31.s64 = ctx.r31.s32 >> 11;
	// srawi r29,r30,11
	ctx.xer.ca = (ctx.r30.s32 < 0) & ((ctx.r30.u32 & 0x7FF) != 0);
	ctx.r29.s64 = ctx.r30.s32 >> 11;
	// subf r30,r27,r5
	ctx.r30.u64 = ctx.r5.u64 - ctx.r27.u64;
	// add r7,r5,r27
	ctx.r7.u64 = ctx.r5.u64 + ctx.r27.u64;
	// mulli r30,r30,2896
	ctx.r30.s64 = static_cast<int64_t>(ctx.r30.u64 * static_cast<uint64_t>(2896));
	// srawi r28,r30,11
	ctx.xer.ca = (ctx.r30.s32 < 0) & ((ctx.r30.u32 & 0x7FF) != 0);
	ctx.r28.s64 = ctx.r30.s32 >> 11;
	// mulli r30,r26,2217
	ctx.r30.s64 = static_cast<int64_t>(ctx.r26.u64 * static_cast<uint64_t>(2217));
	// srawi r27,r30,11
	ctx.xer.ca = (ctx.r30.s32 < 0) & ((ctx.r30.u32 & 0x7FF) != 0);
	ctx.r27.s64 = ctx.r30.s32 >> 11;
	// add r30,r8,r4
	ctx.r30.u64 = ctx.r8.u64 + ctx.r4.u64;
	// subf r4,r8,r4
	ctx.r4.u64 = ctx.r4.u64 - ctx.r8.u64;
	// subf r5,r8,r24
	ctx.r5.u64 = ctx.r24.u64 - ctx.r8.u64;
	// subf r8,r7,r29
	ctx.r8.u64 = ctx.r29.u64 - ctx.r7.u64;
	// subf r29,r31,r27
	ctx.r29.u64 = ctx.r27.u64 - ctx.r31.u64;
	// add r8,r8,r31
	ctx.r8.u64 = ctx.r8.u64 + ctx.r31.u64;
	// add r31,r5,r3
	ctx.r31.u64 = ctx.r5.u64 + ctx.r3.u64;
	// subf r5,r5,r3
	ctx.r5.u64 = ctx.r3.u64 - ctx.r5.u64;
	// add r3,r7,r30
	ctx.r3.u64 = ctx.r7.u64 + ctx.r30.u64;
	// subf r7,r7,r30
	ctx.r7.u64 = ctx.r30.u64 - ctx.r7.u64;
	// addi r3,r3,127
	ctx.r3.s64 = ctx.r3.s64 + 127;
	// addi r30,r7,127
	ctx.r30.s64 = ctx.r7.s64 + 127;
	// srawi r7,r3,8
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0xFF) != 0);
	ctx.r7.s64 = ctx.r3.s32 >> 8;
	// add r3,r8,r31
	ctx.r3.u64 = ctx.r8.u64 + ctx.r31.u64;
	// rlwinm r27,r7,16,8,15
	ctx.r27.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 16) & 0xFF0000;
	// subf r7,r8,r28
	ctx.r7.u64 = ctx.r28.u64 - ctx.r8.u64;
	// subf r8,r8,r31
	ctx.r8.u64 = ctx.r31.u64 - ctx.r8.u64;
	// addi r3,r3,127
	ctx.r3.s64 = ctx.r3.s64 + 127;
	// addi r8,r8,127
	ctx.r8.s64 = ctx.r8.s64 + 127;
	// srawi r3,r3,8
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0xFF) != 0);
	ctx.r3.s64 = ctx.r3.s32 >> 8;
	// srawi r8,r8,8
	ctx.xer.ca = (ctx.r8.s32 < 0) & ((ctx.r8.u32 & 0xFF) != 0);
	ctx.r8.s64 = ctx.r8.s32 >> 8;
	// srawi r31,r30,8
	ctx.xer.ca = (ctx.r30.s32 < 0) & ((ctx.r30.u32 & 0xFF) != 0);
	ctx.r31.s64 = ctx.r30.s32 >> 8;
	// clrlwi r3,r3,24
	ctx.r3.u64 = ctx.r3.u32 & 0xFF;
	// rlwinm r30,r8,16,8,15
	ctx.r30.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 16) & 0xFF0000;
	// clrlwi r31,r31,24
	ctx.r31.u64 = ctx.r31.u32 & 0xFF;
	// or r8,r27,r3
	ctx.r8.u64 = ctx.r27.u64 | ctx.r3.u64;
	// or r3,r30,r31
	ctx.r3.u64 = ctx.r30.u64 | ctx.r31.u64;
	// add r31,r29,r7
	ctx.r31.u64 = ctx.r29.u64 + ctx.r7.u64;
	// subf r30,r7,r5
	ctx.r30.u64 = ctx.r5.u64 - ctx.r7.u64;
	// add r7,r7,r5
	ctx.r7.u64 = ctx.r7.u64 + ctx.r5.u64;
	// add r5,r31,r4
	ctx.r5.u64 = ctx.r31.u64 + ctx.r4.u64;
	// addi r29,r7,127
	ctx.r29.s64 = ctx.r7.s64 + 127;
	// addi r5,r5,127
	ctx.r5.s64 = ctx.r5.s64 + 127;
	// addi r30,r30,127
	ctx.r30.s64 = ctx.r30.s64 + 127;
	// subf r7,r31,r4
	ctx.r7.u64 = ctx.r4.u64 - ctx.r31.u64;
	// srawi r5,r5,8
	ctx.xer.ca = (ctx.r5.s32 < 0) & ((ctx.r5.u32 & 0xFF) != 0);
	ctx.r5.s64 = ctx.r5.s32 >> 8;
	// srawi r4,r30,8
	ctx.xer.ca = (ctx.r30.s32 < 0) & ((ctx.r30.u32 & 0xFF) != 0);
	ctx.r4.s64 = ctx.r30.s32 >> 8;
	// addi r7,r7,127
	ctx.r7.s64 = ctx.r7.s64 + 127;
	// clrlwi r4,r4,24
	ctx.r4.u64 = ctx.r4.u32 & 0xFF;
	// srawi r31,r29,8
	ctx.xer.ca = (ctx.r29.s32 < 0) & ((ctx.r29.u32 & 0xFF) != 0);
	ctx.r31.s64 = ctx.r29.s32 >> 8;
	// rlwinm r5,r5,16,8,15
	ctx.r5.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 16) & 0xFF0000;
	// srawi r30,r7,8
	ctx.xer.ca = (ctx.r7.s32 < 0) & ((ctx.r7.u32 & 0xFF) != 0);
	ctx.r30.s64 = ctx.r7.s32 >> 8;
	// or r7,r5,r4
	ctx.r7.u64 = ctx.r5.u64 | ctx.r4.u64;
	// clrlwi r4,r30,24
	ctx.r4.u64 = ctx.r30.u32 & 0xFF;
	// rlwinm r5,r31,16,8,15
	ctx.r5.u64 = __builtin_rotateleft64(ctx.r31.u32 | (ctx.r31.u64 << 32), 16) & 0xFF0000;
	// rlwinm r31,r8,8,0,23
	ctx.r31.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 8) & 0xFFFFFF00;
	// or r5,r5,r4
	ctx.r5.u64 = ctx.r5.u64 | ctx.r4.u64;
	// rlwinm r4,r3,8,0,23
	ctx.r4.u64 = __builtin_rotateleft64(ctx.r3.u32 | (ctx.r3.u64 << 32), 8) & 0xFFFFFF00;
	// rlwinm r30,r7,8,0,23
	ctx.r30.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 8) & 0xFFFFFF00;
	// or r8,r31,r8
	ctx.r8.u64 = ctx.r31.u64 | ctx.r8.u64;
	// or r4,r4,r3
	ctx.r4.u64 = ctx.r4.u64 | ctx.r3.u64;
	// or r7,r30,r7
	ctx.r7.u64 = ctx.r30.u64 | ctx.r7.u64;
	// rlwinm r3,r5,8,0,23
	ctx.r3.u64 = __builtin_rotateleft64(ctx.r5.u32 | (ctx.r5.u64 << 32), 8) & 0xFFFFFF00;
	// stw r8,-8(r10)
	REX_STORE_U32(ctx.r10.u32 + -8, ctx.r8.u32);
	// addi r6,r6,-1
	ctx.r6.s64 = ctx.r6.s64 + -1;
	// stw r4,4(r10)
	REX_STORE_U32(ctx.r10.u32 + 4, ctx.r4.u32);
	// or r5,r3,r5
	ctx.r5.u64 = ctx.r3.u64 | ctx.r5.u64;
	// stw r7,0(r10)
	REX_STORE_U32(ctx.r10.u32 + 0, ctx.r7.u32);
	// addi r11,r11,32
	ctx.r11.s64 = ctx.r11.s64 + 32;
	// cmplwi cr6,r6,0
	ctx.cr6.compare<uint32_t>(ctx.r6.u32, 0, ctx.xer);
	// stw r5,-4(r10)
	REX_STORE_U32(ctx.r10.u32 + -4, ctx.r5.u32);
	// add r10,r10,r25
	ctx.r10.u64 = ctx.r10.u64 + ctx.r25.u64;
	// stw r8,0(r9)
	REX_STORE_U32(ctx.r9.u32 + 0, ctx.r8.u32);
	// stw r5,4(r9)
	REX_STORE_U32(ctx.r9.u32 + 4, ctx.r5.u32);
	// stw r7,8(r9)
	REX_STORE_U32(ctx.r9.u32 + 8, ctx.r7.u32);
	// stw r4,12(r9)
	REX_STORE_U32(ctx.r9.u32 + 12, ctx.r4.u32);
	// add r9,r25,r9
	ctx.r9.u64 = ctx.r25.u64 + ctx.r9.u64;
	// bne cr6,0x82a84aac
	if (!ctx.cr6.eq) goto loc_82A84AAC;
	// b 0x829ff7f4
	__restgprlr_19(ctx, base);
	return;
}

DEFINE_REX_FUNC(sub_82A84C38) {
	REX_FUNC_PROLOGUE();
	// lis r11,-32236
	ctx.r11.s64 = -2112618496;
	// rlwinm r10,r6,8,0,23
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 8) & 0xFFFFFF00;
	// addi r11,r11,-28000
	ctx.r11.s64 = ctx.r11.s64 + -28000;
	// add r6,r10,r11
	ctx.r6.u64 = ctx.r10.u64 + ctx.r11.u64;
	// b 0x82a84588
	sub_82A84588(ctx, base);
	return;
}

DEFINE_REX_FUNC(sub_82A84C50) {
	REX_FUNC_PROLOGUE();
	// lis r11,-32236
	ctx.r11.s64 = -2112618496;
	// rlwinm r10,r6,8,0,23
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 8) & 0xFFFFFF00;
	// addi r11,r11,-28000
	ctx.r11.s64 = ctx.r11.s64 + -28000;
	// add r6,r10,r11
	ctx.r6.u64 = ctx.r10.u64 + ctx.r11.u64;
	// b 0x82a848b0
	sub_82A848B0(ctx, base);
	return;
}

DEFINE_REX_FUNC(sub_82A84C68) {
	REX_FUNC_PROLOGUE();
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7a0
	ctx.lr = 0x82A84C70;
	__savegprlr_18(ctx, base);
	// lis r11,-32236
	ctx.r11.s64 = -2112618496;
	// rlwinm r10,r6,8,0,23
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 8) & 0xFFFFFF00;
	// addi r11,r11,-23904
	ctx.r11.s64 = ctx.r11.s64 + -23904;
	// mr r9,r5
	ctx.r9.u64 = ctx.r5.u64;
	// add r10,r10,r11
	ctx.r10.u64 = ctx.r10.u64 + ctx.r11.u64;
	// addi r11,r1,-384
	ctx.r11.s64 = ctx.r1.s64 + -384;
	// li r23,8
	ctx.r23.s64 = 8;
loc_82A84C8C:
	// lhz r31,48(r9)
	ctx.r31.u64 = REX_LOAD_U16(ctx.r9.u32 + 48);
	// lhz r5,16(r9)
	ctx.r5.u64 = REX_LOAD_U16(ctx.r9.u32 + 16);
	// lhz r6,32(r9)
	ctx.r6.u64 = REX_LOAD_U16(ctx.r9.u32 + 32);
	// extsh r28,r31
	ctx.r28.s64 = ctx.r31.s16;
	// lhz r31,80(r9)
	ctx.r31.u64 = REX_LOAD_U16(ctx.r9.u32 + 80);
	// extsh r29,r5
	ctx.r29.s64 = ctx.r5.s16;
	// lhz r30,96(r9)
	ctx.r30.u64 = REX_LOAD_U16(ctx.r9.u32 + 96);
	// extsh r6,r6
	ctx.r6.s64 = ctx.r6.s16;
	// extsh r27,r31
	ctx.r27.s64 = ctx.r31.s16;
	// lhz r5,64(r9)
	ctx.r5.u64 = REX_LOAD_U16(ctx.r9.u32 + 64);
	// extsh r31,r30
	ctx.r31.s64 = ctx.r30.s16;
	// lhz r26,112(r9)
	ctx.r26.u64 = REX_LOAD_U16(ctx.r9.u32 + 112);
	// or r30,r29,r6
	ctx.r30.u64 = ctx.r29.u64 | ctx.r6.u64;
	// extsh r5,r5
	ctx.r5.s64 = ctx.r5.s16;
	// or r30,r30,r28
	ctx.r30.u64 = ctx.r30.u64 | ctx.r28.u64;
	// extsh r26,r26
	ctx.r26.s64 = ctx.r26.s16;
	// or r30,r30,r5
	ctx.r30.u64 = ctx.r30.u64 | ctx.r5.u64;
	// or r30,r30,r27
	ctx.r30.u64 = ctx.r30.u64 | ctx.r27.u64;
	// or r30,r30,r31
	ctx.r30.u64 = ctx.r30.u64 | ctx.r31.u64;
	// or r30,r30,r26
	ctx.r30.u64 = ctx.r30.u64 | ctx.r26.u64;
	// extsh r30,r30
	ctx.r30.s64 = ctx.r30.s16;
	// cmpwi cr6,r30,0
	ctx.cr6.compare<int32_t>(ctx.r30.s32, 0, ctx.xer);
	// bne cr6,0x82a84d24
	if (!ctx.cr6.eq) goto loc_82A84D24;
	// lhz r6,0(r9)
	ctx.r6.u64 = REX_LOAD_U16(ctx.r9.u32 + 0);
	// addi r9,r9,2
	ctx.r9.s64 = ctx.r9.s64 + 2;
	// lwz r5,0(r10)
	ctx.r5.u64 = REX_LOAD_U32(ctx.r10.u32 + 0);
	// addi r10,r10,4
	ctx.r10.s64 = ctx.r10.s64 + 4;
	// extsh r6,r6
	ctx.r6.s64 = ctx.r6.s16;
	// mullw r6,r6,r5
	ctx.r6.s64 = int64_t(ctx.r6.s32) * int64_t(ctx.r5.s32);
	// srawi r6,r6,11
	ctx.xer.ca = (ctx.r6.s32 < 0) & ((ctx.r6.u32 & 0x7FF) != 0);
	ctx.r6.s64 = ctx.r6.s32 >> 11;
	// stw r6,0(r11)
	REX_STORE_U32(ctx.r11.u32 + 0, ctx.r6.u32);
	// stw r6,32(r11)
	REX_STORE_U32(ctx.r11.u32 + 32, ctx.r6.u32);
	// stw r6,64(r11)
	REX_STORE_U32(ctx.r11.u32 + 64, ctx.r6.u32);
	// stw r6,128(r11)
	REX_STORE_U32(ctx.r11.u32 + 128, ctx.r6.u32);
	// stw r6,160(r11)
	REX_STORE_U32(ctx.r11.u32 + 160, ctx.r6.u32);
	// stw r6,192(r11)
	REX_STORE_U32(ctx.r11.u32 + 192, ctx.r6.u32);
	// stw r6,224(r11)
	REX_STORE_U32(ctx.r11.u32 + 224, ctx.r6.u32);
	// b 0x82a84e4c
	goto loc_82A84E4C;
loc_82A84D24:
	// lhz r30,0(r9)
	ctx.r30.u64 = REX_LOAD_U16(ctx.r9.u32 + 0);
	// addi r9,r9,2
	ctx.r9.s64 = ctx.r9.s64 + 2;
	// lwz r25,0(r10)
	ctx.r25.u64 = REX_LOAD_U32(ctx.r10.u32 + 0);
	// extsh r30,r30
	ctx.r30.s64 = ctx.r30.s16;
	// lwz r24,64(r10)
	ctx.r24.u64 = REX_LOAD_U32(ctx.r10.u32 + 64);
	// lwz r22,128(r10)
	ctx.r22.u64 = REX_LOAD_U32(ctx.r10.u32 + 128);
	// mullw r30,r30,r25
	ctx.r30.s64 = int64_t(ctx.r30.s32) * int64_t(ctx.r25.s32);
	// lwz r21,192(r10)
	ctx.r21.u64 = REX_LOAD_U32(ctx.r10.u32 + 192);
	// lwz r25,32(r10)
	ctx.r25.u64 = REX_LOAD_U32(ctx.r10.u32 + 32);
	// lwz r20,96(r10)
	ctx.r20.u64 = REX_LOAD_U32(ctx.r10.u32 + 96);
	// lwz r19,160(r10)
	ctx.r19.u64 = REX_LOAD_U32(ctx.r10.u32 + 160);
	// lwz r18,224(r10)
	ctx.r18.u64 = REX_LOAD_U32(ctx.r10.u32 + 224);
	// mullw r6,r24,r6
	ctx.r6.s64 = int64_t(ctx.r24.s32) * int64_t(ctx.r6.s32);
	// mullw r24,r22,r5
	ctx.r24.s64 = int64_t(ctx.r22.s32) * int64_t(ctx.r5.s32);
	// srawi r5,r30,11
	ctx.xer.ca = (ctx.r30.s32 < 0) & ((ctx.r30.u32 & 0x7FF) != 0);
	ctx.r5.s64 = ctx.r30.s32 >> 11;
	// mullw r30,r21,r31
	ctx.r30.s64 = int64_t(ctx.r21.s32) * int64_t(ctx.r31.s32);
	// srawi r6,r6,11
	ctx.xer.ca = (ctx.r6.s32 < 0) & ((ctx.r6.u32 & 0x7FF) != 0);
	ctx.r6.s64 = ctx.r6.s32 >> 11;
	// srawi r31,r24,11
	ctx.xer.ca = (ctx.r24.s32 < 0) & ((ctx.r24.u32 & 0x7FF) != 0);
	ctx.r31.s64 = ctx.r24.s32 >> 11;
	// srawi r30,r30,11
	ctx.xer.ca = (ctx.r30.s32 < 0) & ((ctx.r30.u32 & 0x7FF) != 0);
	ctx.r30.s64 = ctx.r30.s32 >> 11;
	// mullw r25,r25,r29
	ctx.r25.s64 = int64_t(ctx.r25.s32) * int64_t(ctx.r29.s32);
	// subf r24,r30,r6
	ctx.r24.u64 = ctx.r6.u64 - ctx.r30.u64;
	// add r6,r30,r6
	ctx.r6.u64 = ctx.r30.u64 + ctx.r6.u64;
	// mulli r30,r24,2896
	ctx.r30.s64 = static_cast<int64_t>(ctx.r24.u64 * static_cast<uint64_t>(2896));
	// mullw r28,r20,r28
	ctx.r28.s64 = int64_t(ctx.r20.s32) * int64_t(ctx.r28.s32);
	// srawi r24,r30,11
	ctx.xer.ca = (ctx.r30.s32 < 0) & ((ctx.r30.u32 & 0x7FF) != 0);
	ctx.r24.s64 = ctx.r30.s32 >> 11;
	// mullw r27,r19,r27
	ctx.r27.s64 = int64_t(ctx.r19.s32) * int64_t(ctx.r27.s32);
	// add r29,r31,r5
	ctx.r29.u64 = ctx.r31.u64 + ctx.r5.u64;
	// mullw r26,r18,r26
	ctx.r26.s64 = int64_t(ctx.r18.s32) * int64_t(ctx.r26.s32);
	// srawi r30,r25,11
	ctx.xer.ca = (ctx.r25.s32 < 0) & ((ctx.r25.u32 & 0x7FF) != 0);
	ctx.r30.s64 = ctx.r25.s32 >> 11;
	// srawi r28,r28,11
	ctx.xer.ca = (ctx.r28.s32 < 0) & ((ctx.r28.u32 & 0x7FF) != 0);
	ctx.r28.s64 = ctx.r28.s32 >> 11;
	// srawi r27,r27,11
	ctx.xer.ca = (ctx.r27.s32 < 0) & ((ctx.r27.u32 & 0x7FF) != 0);
	ctx.r27.s64 = ctx.r27.s32 >> 11;
	// subf r5,r31,r5
	ctx.r5.u64 = ctx.r5.u64 - ctx.r31.u64;
	// add r25,r6,r29
	ctx.r25.u64 = ctx.r6.u64 + ctx.r29.u64;
	// srawi r26,r26,11
	ctx.xer.ca = (ctx.r26.s32 < 0) & ((ctx.r26.u32 & 0x7FF) != 0);
	ctx.r26.s64 = ctx.r26.s32 >> 11;
	// subf r31,r6,r24
	ctx.r31.u64 = ctx.r24.u64 - ctx.r6.u64;
	// subf r29,r6,r29
	ctx.r29.u64 = ctx.r29.u64 - ctx.r6.u64;
	// subf r6,r28,r27
	ctx.r6.u64 = ctx.r27.u64 - ctx.r28.u64;
	// subf r24,r26,r30
	ctx.r24.u64 = ctx.r30.u64 - ctx.r26.u64;
	// add r28,r27,r28
	ctx.r28.u64 = ctx.r27.u64 + ctx.r28.u64;
	// add r27,r31,r5
	ctx.r27.u64 = ctx.r31.u64 + ctx.r5.u64;
	// add r30,r26,r30
	ctx.r30.u64 = ctx.r26.u64 + ctx.r30.u64;
	// subf r31,r31,r5
	ctx.r31.u64 = ctx.r5.u64 - ctx.r31.u64;
	// add r5,r24,r6
	ctx.r5.u64 = ctx.r24.u64 + ctx.r6.u64;
	// mulli r26,r6,-5352
	ctx.r26.s64 = static_cast<int64_t>(ctx.r6.u64 * static_cast<uint64_t>(-5352));
	// add r6,r30,r28
	ctx.r6.u64 = ctx.r30.u64 + ctx.r28.u64;
	// subf r30,r28,r30
	ctx.r30.u64 = ctx.r30.u64 - ctx.r28.u64;
	// mulli r5,r5,3784
	ctx.r5.s64 = static_cast<int64_t>(ctx.r5.u64 * static_cast<uint64_t>(3784));
	// srawi r5,r5,11
	ctx.xer.ca = (ctx.r5.s32 < 0) & ((ctx.r5.u32 & 0x7FF) != 0);
	ctx.r5.s64 = ctx.r5.s32 >> 11;
	// mulli r30,r30,2896
	ctx.r30.s64 = static_cast<int64_t>(ctx.r30.u64 * static_cast<uint64_t>(2896));
	// srawi r26,r26,11
	ctx.xer.ca = (ctx.r26.s32 < 0) & ((ctx.r26.u32 & 0x7FF) != 0);
	ctx.r26.s64 = ctx.r26.s32 >> 11;
	// mulli r28,r24,2217
	ctx.r28.s64 = static_cast<int64_t>(ctx.r24.u64 * static_cast<uint64_t>(2217));
	// srawi r24,r30,11
	ctx.xer.ca = (ctx.r30.s32 < 0) & ((ctx.r30.u32 & 0x7FF) != 0);
	ctx.r24.s64 = ctx.r30.s32 >> 11;
	// subf r30,r6,r26
	ctx.r30.u64 = ctx.r26.u64 - ctx.r6.u64;
	// add r26,r6,r25
	ctx.r26.u64 = ctx.r6.u64 + ctx.r25.u64;
	// subf r6,r6,r25
	ctx.r6.u64 = ctx.r25.u64 - ctx.r6.u64;
	// srawi r28,r28,11
	ctx.xer.ca = (ctx.r28.s32 < 0) & ((ctx.r28.u32 & 0x7FF) != 0);
	ctx.r28.s64 = ctx.r28.s32 >> 11;
	// addi r10,r10,4
	ctx.r10.s64 = ctx.r10.s64 + 4;
	// stw r26,0(r11)
	REX_STORE_U32(ctx.r11.u32 + 0, ctx.r26.u32);
	// stw r6,224(r11)
	REX_STORE_U32(ctx.r11.u32 + 224, ctx.r6.u32);
	// add r6,r30,r5
	ctx.r6.u64 = ctx.r30.u64 + ctx.r5.u64;
	// subf r30,r5,r28
	ctx.r30.u64 = ctx.r28.u64 - ctx.r5.u64;
	// subf r5,r6,r24
	ctx.r5.u64 = ctx.r24.u64 - ctx.r6.u64;
	// add r28,r6,r27
	ctx.r28.u64 = ctx.r6.u64 + ctx.r27.u64;
	// subf r6,r6,r27
	ctx.r6.u64 = ctx.r27.u64 - ctx.r6.u64;
	// stw r28,32(r11)
	REX_STORE_U32(ctx.r11.u32 + 32, ctx.r28.u32);
	// stw r6,192(r11)
	REX_STORE_U32(ctx.r11.u32 + 192, ctx.r6.u32);
	// add r6,r30,r5
	ctx.r6.u64 = ctx.r30.u64 + ctx.r5.u64;
	// add r30,r5,r31
	ctx.r30.u64 = ctx.r5.u64 + ctx.r31.u64;
	// subf r5,r5,r31
	ctx.r5.u64 = ctx.r31.u64 - ctx.r5.u64;
	// stw r30,64(r11)
	REX_STORE_U32(ctx.r11.u32 + 64, ctx.r30.u32);
	// stw r5,160(r11)
	REX_STORE_U32(ctx.r11.u32 + 160, ctx.r5.u32);
	// add r5,r6,r29
	ctx.r5.u64 = ctx.r6.u64 + ctx.r29.u64;
	// subf r6,r6,r29
	ctx.r6.u64 = ctx.r29.u64 - ctx.r6.u64;
	// stw r5,128(r11)
	REX_STORE_U32(ctx.r11.u32 + 128, ctx.r5.u32);
loc_82A84E4C:
	// addi r23,r23,-1
	ctx.r23.s64 = ctx.r23.s64 + -1;
	// stw r6,96(r11)
	REX_STORE_U32(ctx.r11.u32 + 96, ctx.r6.u32);
	// addi r11,r11,4
	ctx.r11.s64 = ctx.r11.s64 + 4;
	// cmpwi cr6,r23,0
	ctx.cr6.compare<int32_t>(ctx.r23.s32, 0, ctx.xer);
	// bgt cr6,0x82a84c8c
	if (ctx.cr6.gt) goto loc_82A84C8C;
	// addi r9,r3,1
	ctx.r9.s64 = ctx.r3.s64 + 1;
	// addi r10,r7,1
	ctx.r10.s64 = ctx.r7.s64 + 1;
	// addi r11,r1,-360
	ctx.r11.s64 = ctx.r1.s64 + -360;
	// li r5,8
	ctx.r5.s64 = 8;
loc_82A84E70:
	// lwz r7,-4(r11)
	ctx.r7.u64 = REX_LOAD_U32(ctx.r11.u32 + -4);
	// addi r5,r5,-1
	ctx.r5.s64 = ctx.r5.s64 + -1;
	// lwz r6,-12(r11)
	ctx.r6.u64 = REX_LOAD_U32(ctx.r11.u32 + -12);
	// lwz r30,0(r11)
	ctx.r30.u64 = REX_LOAD_U32(ctx.r11.u32 + 0);
	// lwz r29,-16(r11)
	ctx.r29.u64 = REX_LOAD_U32(ctx.r11.u32 + -16);
	// subf r26,r6,r7
	ctx.r26.u64 = ctx.r7.u64 - ctx.r6.u64;
	// add r25,r6,r7
	ctx.r25.u64 = ctx.r6.u64 + ctx.r7.u64;
	// lwz r31,-20(r11)
	ctx.r31.u64 = REX_LOAD_U32(ctx.r11.u32 + -20);
	// subf r6,r30,r29
	ctx.r6.u64 = ctx.r29.u64 - ctx.r30.u64;
	// lwz r3,4(r11)
	ctx.r3.u64 = REX_LOAD_U32(ctx.r11.u32 + 4);
	// add r7,r29,r30
	ctx.r7.u64 = ctx.r29.u64 + ctx.r30.u64;
	// lwz r28,-8(r11)
	ctx.r28.u64 = REX_LOAD_U32(ctx.r11.u32 + -8);
	// subf r24,r3,r31
	ctx.r24.u64 = ctx.r31.u64 - ctx.r3.u64;
	// lwz r27,-24(r11)
	ctx.r27.u64 = REX_LOAD_U32(ctx.r11.u32 + -24);
	// mulli r6,r6,2896
	ctx.r6.s64 = static_cast<int64_t>(ctx.r6.u64 * static_cast<uint64_t>(2896));
	// lbz r23,-1(r10)
	ctx.r23.u64 = REX_LOAD_U8(ctx.r10.u32 + -1);
	// add r3,r31,r3
	ctx.r3.u64 = ctx.r31.u64 + ctx.r3.u64;
	// srawi r22,r6,11
	ctx.xer.ca = (ctx.r6.s32 < 0) & ((ctx.r6.u32 & 0x7FF) != 0);
	ctx.r22.s64 = ctx.r6.s32 >> 11;
	// add r29,r24,r26
	ctx.r29.u64 = ctx.r24.u64 + ctx.r26.u64;
	// add r6,r3,r25
	ctx.r6.u64 = ctx.r3.u64 + ctx.r25.u64;
	// subf r3,r25,r3
	ctx.r3.u64 = ctx.r3.u64 - ctx.r25.u64;
	// mulli r29,r29,3784
	ctx.r29.s64 = static_cast<int64_t>(ctx.r29.u64 * static_cast<uint64_t>(3784));
	// mulli r21,r26,-5352
	ctx.r21.s64 = static_cast<int64_t>(ctx.r26.u64 * static_cast<uint64_t>(-5352));
	// add r31,r27,r28
	ctx.r31.u64 = ctx.r27.u64 + ctx.r28.u64;
	// mulli r3,r3,2896
	ctx.r3.s64 = static_cast<int64_t>(ctx.r3.u64 * static_cast<uint64_t>(2896));
	// srawi r29,r29,11
	ctx.xer.ca = (ctx.r29.s32 < 0) & ((ctx.r29.u32 & 0x7FF) != 0);
	ctx.r29.s64 = ctx.r29.s32 >> 11;
	// subf r30,r28,r27
	ctx.r30.u64 = ctx.r27.u64 - ctx.r28.u64;
	// srawi r26,r21,11
	ctx.xer.ca = (ctx.r21.s32 < 0) & ((ctx.r21.u32 & 0x7FF) != 0);
	ctx.r26.s64 = ctx.r21.s32 >> 11;
	// mulli r25,r24,2217
	ctx.r25.s64 = static_cast<int64_t>(ctx.r24.u64 * static_cast<uint64_t>(2217));
	// add r27,r7,r31
	ctx.r27.u64 = ctx.r7.u64 + ctx.r31.u64;
	// srawi r24,r3,11
	ctx.xer.ca = (ctx.r3.s32 < 0) & ((ctx.r3.u32 & 0x7FF) != 0);
	ctx.r24.s64 = ctx.r3.s32 >> 11;
	// subf r3,r7,r31
	ctx.r3.u64 = ctx.r31.u64 - ctx.r7.u64;
	// subf r28,r7,r22
	ctx.r28.u64 = ctx.r22.u64 - ctx.r7.u64;
	// subf r7,r6,r26
	ctx.r7.u64 = ctx.r26.u64 - ctx.r6.u64;
	// add r26,r6,r27
	ctx.r26.u64 = ctx.r6.u64 + ctx.r27.u64;
	// add r31,r28,r30
	ctx.r31.u64 = ctx.r28.u64 + ctx.r30.u64;
	// addi r26,r26,127
	ctx.r26.s64 = ctx.r26.s64 + 127;
	// subf r30,r28,r30
	ctx.r30.u64 = ctx.r30.u64 - ctx.r28.u64;
	// subf r28,r6,r27
	ctx.r28.u64 = ctx.r27.u64 - ctx.r6.u64;
	// srawi r25,r25,11
	ctx.xer.ca = (ctx.r25.s32 < 0) & ((ctx.r25.u32 & 0x7FF) != 0);
	ctx.r25.s64 = ctx.r25.s32 >> 11;
	// srawi r6,r26,8
	ctx.xer.ca = (ctx.r26.s32 < 0) & ((ctx.r26.u32 & 0xFF) != 0);
	ctx.r6.s64 = ctx.r26.s32 >> 8;
	// addi r28,r28,127
	ctx.r28.s64 = ctx.r28.s64 + 127;
	// add r27,r6,r23
	ctx.r27.u64 = ctx.r6.u64 + ctx.r23.u64;
	// srawi r6,r28,8
	ctx.xer.ca = (ctx.r28.s32 < 0) & ((ctx.r28.u32 & 0xFF) != 0);
	ctx.r6.s64 = ctx.r28.s32 >> 8;
	// add r7,r7,r29
	ctx.r7.u64 = ctx.r7.u64 + ctx.r29.u64;
	// subf r29,r29,r25
	ctx.r29.u64 = ctx.r25.u64 - ctx.r29.u64;
	// addi r11,r11,32
	ctx.r11.s64 = ctx.r11.s64 + 32;
	// stb r27,-1(r9)
	REX_STORE_U8(ctx.r9.u32 + -1, ctx.r27.u8);
	// add r27,r7,r31
	ctx.r27.u64 = ctx.r7.u64 + ctx.r31.u64;
	// lbz r28,6(r10)
	ctx.r28.u64 = REX_LOAD_U8(ctx.r10.u32 + 6);
	// add r6,r6,r28
	ctx.r6.u64 = ctx.r6.u64 + ctx.r28.u64;
	// stb r6,6(r9)
	REX_STORE_U8(ctx.r9.u32 + 6, ctx.r6.u8);
	// subf r6,r7,r24
	ctx.r6.u64 = ctx.r24.u64 - ctx.r7.u64;
	// subf r7,r7,r31
	ctx.r7.u64 = ctx.r31.u64 - ctx.r7.u64;
	// lbz r28,0(r10)
	ctx.r28.u64 = REX_LOAD_U8(ctx.r10.u32 + 0);
	// addi r31,r27,127
	ctx.r31.s64 = ctx.r27.s64 + 127;
	// addi r7,r7,127
	ctx.r7.s64 = ctx.r7.s64 + 127;
	// srawi r31,r31,8
	ctx.xer.ca = (ctx.r31.s32 < 0) & ((ctx.r31.u32 & 0xFF) != 0);
	ctx.r31.s64 = ctx.r31.s32 >> 8;
	// srawi r7,r7,8
	ctx.xer.ca = (ctx.r7.s32 < 0) & ((ctx.r7.u32 & 0xFF) != 0);
	ctx.r7.s64 = ctx.r7.s32 >> 8;
	// add r31,r31,r28
	ctx.r31.u64 = ctx.r31.u64 + ctx.r28.u64;
	// stb r31,0(r9)
	REX_STORE_U8(ctx.r9.u32 + 0, ctx.r31.u8);
	// lbz r31,5(r10)
	ctx.r31.u64 = REX_LOAD_U8(ctx.r10.u32 + 5);
	// add r7,r7,r31
	ctx.r7.u64 = ctx.r7.u64 + ctx.r31.u64;
	// stb r7,5(r9)
	REX_STORE_U8(ctx.r9.u32 + 5, ctx.r7.u8);
	// add r7,r6,r30
	ctx.r7.u64 = ctx.r6.u64 + ctx.r30.u64;
	// subf r30,r6,r30
	ctx.r30.u64 = ctx.r30.u64 - ctx.r6.u64;
	// lbz r31,1(r10)
	ctx.r31.u64 = REX_LOAD_U8(ctx.r10.u32 + 1);
	// addi r28,r7,127
	ctx.r28.s64 = ctx.r7.s64 + 127;
	// add r7,r29,r6
	ctx.r7.u64 = ctx.r29.u64 + ctx.r6.u64;
	// srawi r6,r28,8
	ctx.xer.ca = (ctx.r28.s32 < 0) & ((ctx.r28.u32 & 0xFF) != 0);
	ctx.r6.s64 = ctx.r28.s32 >> 8;
	// addi r30,r30,127
	ctx.r30.s64 = ctx.r30.s64 + 127;
	// add r6,r6,r31
	ctx.r6.u64 = ctx.r6.u64 + ctx.r31.u64;
	// srawi r31,r30,8
	ctx.xer.ca = (ctx.r30.s32 < 0) & ((ctx.r30.u32 & 0xFF) != 0);
	ctx.r31.s64 = ctx.r30.s32 >> 8;
	// stb r6,1(r9)
	REX_STORE_U8(ctx.r9.u32 + 1, ctx.r6.u8);
	// lbz r6,4(r10)
	ctx.r6.u64 = REX_LOAD_U8(ctx.r10.u32 + 4);
	// mr r30,r6
	ctx.r30.u64 = ctx.r6.u64;
	// subf r6,r7,r3
	ctx.r6.u64 = ctx.r3.u64 - ctx.r7.u64;
	// add r31,r31,r30
	ctx.r31.u64 = ctx.r31.u64 + ctx.r30.u64;
	// addi r6,r6,127
	ctx.r6.s64 = ctx.r6.s64 + 127;
	// add r7,r7,r3
	ctx.r7.u64 = ctx.r7.u64 + ctx.r3.u64;
	// srawi r6,r6,8
	ctx.xer.ca = (ctx.r6.s32 < 0) & ((ctx.r6.u32 & 0xFF) != 0);
	ctx.r6.s64 = ctx.r6.s32 >> 8;
	// stb r31,4(r9)
	REX_STORE_U8(ctx.r9.u32 + 4, ctx.r31.u8);
	// addi r7,r7,127
	ctx.r7.s64 = ctx.r7.s64 + 127;
	// lbz r3,2(r10)
	ctx.r3.u64 = REX_LOAD_U8(ctx.r10.u32 + 2);
	// cmplwi cr6,r5,0
	ctx.cr6.compare<uint32_t>(ctx.r5.u32, 0, ctx.xer);
	// srawi r7,r7,8
	ctx.xer.ca = (ctx.r7.s32 < 0) & ((ctx.r7.u32 & 0xFF) != 0);
	ctx.r7.s64 = ctx.r7.s32 >> 8;
	// add r6,r6,r3
	ctx.r6.u64 = ctx.r6.u64 + ctx.r3.u64;
	// stb r6,2(r9)
	REX_STORE_U8(ctx.r9.u32 + 2, ctx.r6.u8);
	// lbz r6,3(r10)
	ctx.r6.u64 = REX_LOAD_U8(ctx.r10.u32 + 3);
	// add r10,r10,r8
	ctx.r10.u64 = ctx.r10.u64 + ctx.r8.u64;
	// add r7,r7,r6
	ctx.r7.u64 = ctx.r7.u64 + ctx.r6.u64;
	// stb r7,3(r9)
	REX_STORE_U8(ctx.r9.u32 + 3, ctx.r7.u8);
	// add r9,r9,r4
	ctx.r9.u64 = ctx.r9.u64 + ctx.r4.u64;
	// bne cr6,0x82a84e70
	if (!ctx.cr6.eq) goto loc_82A84E70;
	// b 0x829ff7f0
	__restgprlr_18(ctx, base);
	return;
}

DEFINE_REX_FUNC(sub_82A84FF0) {
	REX_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7a0
	ctx.lr = 0x82A84FF8;
	__savegprlr_18(ctx, base);
	// vspltisw v0,0
	simde_mm_store_si128((simde__m128i*)ctx.v0.u32, simde_mm_set1_epi32(int(0x0)));
	// addi r9,r1,-384
	ctx.r9.s64 = ctx.r1.s64 + -384;
	// lwz r11,8(r4)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r4.u32 + 8);
	// lwz r5,4(r4)
	ctx.r5.u64 = REX_LOAD_U32(ctx.r4.u32 + 4);
	// lwz r10,0(r4)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r4.u32 + 0);
	// cmplwi cr6,r11,4
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 4, ctx.xer);
	// stvx128 v0,r0,r9
	ea = (ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r9,r1,-368
	ctx.r9.s64 = ctx.r1.s64 + -368;
	// stvx128 v0,r0,r9
	ea = (ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r9,r1,-352
	ctx.r9.s64 = ctx.r1.s64 + -352;
	// stvx128 v0,r0,r9
	ea = (ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r9,r1,-336
	ctx.r9.s64 = ctx.r1.s64 + -336;
	// stvx128 v0,r0,r9
	ea = (ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r9,r1,-320
	ctx.r9.s64 = ctx.r1.s64 + -320;
	// stvx128 v0,r0,r9
	ea = (ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r9,r1,-304
	ctx.r9.s64 = ctx.r1.s64 + -304;
	// stvx128 v0,r0,r9
	ea = (ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r9,r1,-288
	ctx.r9.s64 = ctx.r1.s64 + -288;
	// stvx128 v0,r0,r9
	ea = (ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r9,r1,-272
	ctx.r9.s64 = ctx.r1.s64 + -272;
	// stvx128 v0,r0,r9
	ea = (ctx.r9.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bge cr6,0x82a8507c
	if (!ctx.cr6.lt) goto loc_82A8507C;
	// lwz r9,0(r5)
	ctx.r9.u64 = REX_LOAD_U32(ctx.r5.u32 + 0);
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// subfic r8,r11,4
	ctx.xer.ca = ctx.r11.u32 <= 4;
	ctx.r8.u64 = static_cast<uint64_t>(4) - ctx.r11.u64;
	// addi r5,r5,4
	ctx.r5.s64 = ctx.r5.s64 + 4;
	// slw r7,r9,r11
	ctx.r7.u64 = ctx.r11.u8 & 0x20 ? 0 : (ctx.r9.u32 << (ctx.r11.u8 & 0x3F));
	// addi r11,r11,28
	ctx.r11.s64 = ctx.r11.s64 + 28;
	// clrlwi r7,r7,24
	ctx.r7.u64 = ctx.r7.u32 & 0xFF;
	// or r10,r7,r10
	ctx.r10.u64 = ctx.r7.u64 | ctx.r10.u64;
	// clrlwi r25,r10,28
	ctx.r25.u64 = ctx.r10.u32 & 0xF;
	// srw r10,r9,r8
	ctx.r10.u64 = ctx.r8.u8 & 0x20 ? 0 : (ctx.r9.u32 >> (ctx.r8.u8 & 0x3F));
	// b 0x82a8508c
	goto loc_82A8508C;
loc_82A8507C:
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// rlwinm r10,r10,28,4,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 28) & 0xFFFFFFF;
	// clrlwi r25,r9,28
	ctx.r25.u64 = ctx.r9.u32 & 0xF;
	// addi r11,r11,-4
	ctx.r11.s64 = ctx.r11.s64 + -4;
loc_82A8508C:
	// li r8,16
	ctx.r8.s64 = 16;
	// addi r9,r25,-1
	ctx.r9.s64 = ctx.r25.s64 + -1;
	// li r22,1
	ctx.r22.s64 = 1;
	// addi r24,r1,-188
	ctx.r24.s64 = ctx.r1.s64 + -188;
	// addi r23,r1,-182
	ctx.r23.s64 = ctx.r1.s64 + -182;
	// stb r8,-188(r1)
	REX_STORE_U8(ctx.r1.u32 + -188, ctx.r8.u8);
	// li r8,96
	ctx.r8.s64 = 96;
	// mr r7,r25
	ctx.r7.u64 = ctx.r25.u64;
	// cmplwi cr6,r25,1
	ctx.cr6.compare<uint32_t>(ctx.r25.u32, 1, ctx.xer);
	// li r21,0
	ctx.r21.s64 = 0;
	// stb r8,-187(r1)
	REX_STORE_U8(ctx.r1.u32 + -187, ctx.r8.u8);
	// li r8,176
	ctx.r8.s64 = 176;
	// stb r8,-186(r1)
	REX_STORE_U8(ctx.r1.u32 + -186, ctx.r8.u8);
	// li r8,7
	ctx.r8.s64 = 7;
	// slw r27,r22,r9
	ctx.r27.u64 = ctx.r9.u8 & 0x20 ? 0 : (ctx.r22.u32 << (ctx.r9.u8 & 0x3F));
	// stb r8,-185(r1)
	REX_STORE_U8(ctx.r1.u32 + -185, ctx.r8.u8);
	// li r8,11
	ctx.r8.s64 = 11;
	// stb r8,-184(r1)
	REX_STORE_U8(ctx.r1.u32 + -184, ctx.r8.u8);
	// li r8,15
	ctx.r8.s64 = 15;
	// stb r8,-183(r1)
	REX_STORE_U8(ctx.r1.u32 + -183, ctx.r8.u8);
	// ble cr6,0x82a85530
	if (!ctx.cr6.gt) goto loc_82A85530;
	// li r26,-1
	ctx.r26.s64 = -1;
loc_82A850E4:
	// addi r31,r7,1
	ctx.r31.s64 = ctx.r7.s64 + 1;
	// mr r30,r24
	ctx.r30.u64 = ctx.r24.u64;
	// addi r9,r31,-2
	ctx.r9.s64 = ctx.r31.s64 + -2;
	// cmplw cr6,r24,r23
	ctx.cr6.compare<uint32_t>(ctx.r24.u32, ctx.r23.u32, ctx.xer);
	// slw r29,r22,r9
	ctx.r29.u64 = ctx.r9.u8 & 0x20 ? 0 : (ctx.r22.u32 << (ctx.r9.u8 & 0x3F));
	// addi r28,r29,-1
	ctx.r28.s64 = ctx.r29.s64 + -1;
	// bge cr6,0x82a85520
	if (!ctx.cr6.lt) goto loc_82A85520;
loc_82A85100:
	// lbz r8,0(r30)
	ctx.r8.u64 = REX_LOAD_U8(ctx.r30.u32 + 0);
	// cmplwi cr6,r8,0
	ctx.cr6.compare<uint32_t>(ctx.r8.u32, 0, ctx.xer);
	// beq cr6,0x82a85514
	if (ctx.cr6.eq) goto loc_82A85514;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82a85128
	if (!ctx.cr6.eq) goto loc_82A85128;
	// lwz r9,0(r5)
	ctx.r9.u64 = REX_LOAD_U32(ctx.r5.u32 + 0);
	// li r11,31
	ctx.r11.s64 = 31;
	// addi r5,r5,4
	ctx.r5.s64 = ctx.r5.s64 + 4;
	// rlwinm r10,r9,31,1,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 31) & 0x7FFFFFFF;
	// b 0x82a85134
	goto loc_82A85134;
loc_82A85128:
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// rlwinm r10,r10,31,1,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
loc_82A85134:
	// clrlwi r9,r9,31
	ctx.r9.u64 = ctx.r9.u32 & 0x1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// beq cr6,0x82a85514
	if (ctx.cr6.eq) goto loc_82A85514;
	// clrlwi r9,r8,30
	ctx.r9.u64 = ctx.r8.u32 & 0x3;
	// cmplwi cr6,r9,3
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 3, ctx.xer);
	// bgt cr6,0x82a85514
	if (ctx.cr6.gt) goto loc_82A85514;
	// lis r12,-32088
	ctx.r12.s64 = -2102919168;
	// addi r12,r12,20836
	ctx.r12.s64 = ctx.r12.s64 + 20836;
	// rlwinm r0,r9,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = REX_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r9.u32) {
	case 0:
		goto loc_82A85174;
	case 1:
		goto loc_82A85188;
	case 2:
		goto loc_82A851C0;
	case 3:
		goto loc_82A8549C;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_82A85174:
	// rlwinm r6,r8,30,2,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 30) & 0x3FFFFFFF;
	// rlwinm r9,r6,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r9,r9,17
	ctx.r9.s64 = ctx.r9.s64 + 17;
	// stb r9,0(r30)
	REX_STORE_U8(ctx.r30.u32 + 0, ctx.r9.u8);
	// b 0x82a851cc
	goto loc_82A851CC;
loc_82A85188:
	// rlwinm r9,r8,0,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0xFFFFFFFC;
	// addi r8,r9,2
	ctx.r8.s64 = ctx.r9.s64 + 2;
	// addi r6,r9,18
	ctx.r6.s64 = ctx.r9.s64 + 18;
	// addi r20,r9,34
	ctx.r20.s64 = ctx.r9.s64 + 34;
	// addi r9,r9,50
	ctx.r9.s64 = ctx.r9.s64 + 50;
	// stb r8,0(r30)
	REX_STORE_U8(ctx.r30.u32 + 0, ctx.r8.u8);
	// mr r8,r9
	ctx.r8.u64 = ctx.r9.u64;
	// addi r9,r23,1
	ctx.r9.s64 = ctx.r23.s64 + 1;
	// stb r6,0(r23)
	REX_STORE_U8(ctx.r23.u32 + 0, ctx.r6.u8);
	// stb r20,0(r9)
	REX_STORE_U8(ctx.r9.u32 + 0, ctx.r20.u8);
	// addi r9,r9,1
	ctx.r9.s64 = ctx.r9.s64 + 1;
	// addi r23,r9,1
	ctx.r23.s64 = ctx.r9.s64 + 1;
	// stb r8,0(r9)
	REX_STORE_U8(ctx.r9.u32 + 0, ctx.r8.u8);
	// b 0x82a85518
	goto loc_82A85518;
loc_82A851C0:
	// stb r21,0(r30)
	REX_STORE_U8(ctx.r30.u32 + 0, ctx.r21.u8);
	// rlwinm r6,r8,30,2,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 30) & 0x3FFFFFFF;
	// addi r30,r30,1
	ctx.r30.s64 = ctx.r30.s64 + 1;
loc_82A851CC:
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82a85328
	if (!ctx.cr6.eq) goto loc_82A85328;
	// lwz r10,0(r5)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r5.u32 + 0);
	// addi r5,r5,4
	ctx.r5.s64 = ctx.r5.s64 + 4;
	// clrlwi r11,r10,31
	ctx.r11.u64 = ctx.r10.u32 & 0x1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82a8530c
	if (!ctx.cr6.eq) goto loc_82A8530C;
	// rlwinm r9,r10,31,1,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// subfic r11,r7,31
	ctx.xer.ca = ctx.r7.u32 <= 31;
	ctx.r11.u64 = static_cast<uint64_t>(31) - ctx.r7.u64;
loc_82A851F0:
	// srw r10,r10,r31
	ctx.r10.u64 = ctx.r31.u8 & 0x20 ? 0 : (ctx.r10.u32 >> (ctx.r31.u8 & 0x3F));
loc_82A851F4:
	// and r20,r9,r29
	ctx.r20.u64 = ctx.r9.u64 & ctx.r29.u64;
	// and r8,r9,r28
	ctx.r8.u64 = ctx.r9.u64 & ctx.r28.u64;
	// cmplwi cr6,r20,0
	ctx.cr6.compare<uint32_t>(ctx.r20.u32, 0, ctx.xer);
	// or r9,r8,r27
	ctx.r9.u64 = ctx.r8.u64 | ctx.r27.u64;
	// beq cr6,0x82a8520c
	if (ctx.cr6.eq) goto loc_82A8520C;
	// neg r9,r9
	ctx.r9.s64 = static_cast<int64_t>(-ctx.r9.u64);
loc_82A8520C:
	// rlwinm r8,r6,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 1) & 0xFFFFFFFE;
	// addi r20,r1,-384
	ctx.r20.s64 = ctx.r1.s64 + -384;
	// sthx r9,r8,r20
	REX_STORE_U16(ctx.r8.u32 + ctx.r20.u32, ctx.r9.u16);
loc_82A85218:
	// addi r6,r6,1
	ctx.r6.s64 = ctx.r6.s64 + 1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82a8538c
	if (!ctx.cr6.eq) goto loc_82A8538C;
	// lwz r10,0(r5)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r5.u32 + 0);
	// addi r5,r5,4
	ctx.r5.s64 = ctx.r5.s64 + 4;
	// clrlwi r11,r10,31
	ctx.r11.u64 = ctx.r10.u32 & 0x1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82a85370
	if (!ctx.cr6.eq) goto loc_82A85370;
	// rlwinm r9,r10,31,1,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// subfic r11,r7,31
	ctx.xer.ca = ctx.r7.u32 <= 31;
	ctx.r11.u64 = static_cast<uint64_t>(31) - ctx.r7.u64;
loc_82A85240:
	// srw r10,r10,r31
	ctx.r10.u64 = ctx.r31.u8 & 0x20 ? 0 : (ctx.r10.u32 >> (ctx.r31.u8 & 0x3F));
loc_82A85244:
	// and r20,r9,r29
	ctx.r20.u64 = ctx.r9.u64 & ctx.r29.u64;
	// and r8,r9,r28
	ctx.r8.u64 = ctx.r9.u64 & ctx.r28.u64;
	// cmplwi cr6,r20,0
	ctx.cr6.compare<uint32_t>(ctx.r20.u32, 0, ctx.xer);
	// or r9,r8,r27
	ctx.r9.u64 = ctx.r8.u64 | ctx.r27.u64;
	// beq cr6,0x82a8525c
	if (ctx.cr6.eq) goto loc_82A8525C;
	// neg r9,r9
	ctx.r9.s64 = static_cast<int64_t>(-ctx.r9.u64);
loc_82A8525C:
	// rlwinm r8,r6,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 1) & 0xFFFFFFFE;
	// addi r20,r1,-384
	ctx.r20.s64 = ctx.r1.s64 + -384;
	// sthx r9,r8,r20
	REX_STORE_U16(ctx.r8.u32 + ctx.r20.u32, ctx.r9.u16);
loc_82A85268:
	// addi r6,r6,1
	ctx.r6.s64 = ctx.r6.s64 + 1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82a853f0
	if (!ctx.cr6.eq) goto loc_82A853F0;
	// lwz r10,0(r5)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r5.u32 + 0);
	// addi r5,r5,4
	ctx.r5.s64 = ctx.r5.s64 + 4;
	// clrlwi r11,r10,31
	ctx.r11.u64 = ctx.r10.u32 & 0x1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82a853d4
	if (!ctx.cr6.eq) goto loc_82A853D4;
	// rlwinm r9,r10,31,1,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// subfic r11,r7,31
	ctx.xer.ca = ctx.r7.u32 <= 31;
	ctx.r11.u64 = static_cast<uint64_t>(31) - ctx.r7.u64;
loc_82A85290:
	// srw r10,r10,r31
	ctx.r10.u64 = ctx.r31.u8 & 0x20 ? 0 : (ctx.r10.u32 >> (ctx.r31.u8 & 0x3F));
loc_82A85294:
	// and r20,r9,r29
	ctx.r20.u64 = ctx.r9.u64 & ctx.r29.u64;
	// and r8,r9,r28
	ctx.r8.u64 = ctx.r9.u64 & ctx.r28.u64;
	// cmplwi cr6,r20,0
	ctx.cr6.compare<uint32_t>(ctx.r20.u32, 0, ctx.xer);
	// or r9,r8,r27
	ctx.r9.u64 = ctx.r8.u64 | ctx.r27.u64;
	// beq cr6,0x82a852ac
	if (ctx.cr6.eq) goto loc_82A852AC;
	// neg r9,r9
	ctx.r9.s64 = static_cast<int64_t>(-ctx.r9.u64);
loc_82A852AC:
	// rlwinm r8,r6,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 1) & 0xFFFFFFFE;
	// addi r20,r1,-384
	ctx.r20.s64 = ctx.r1.s64 + -384;
	// sthx r9,r8,r20
	REX_STORE_U16(ctx.r8.u32 + ctx.r20.u32, ctx.r9.u16);
loc_82A852B8:
	// addi r6,r6,1
	ctx.r6.s64 = ctx.r6.s64 + 1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82a85454
	if (!ctx.cr6.eq) goto loc_82A85454;
	// lwz r10,0(r5)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r5.u32 + 0);
	// addi r5,r5,4
	ctx.r5.s64 = ctx.r5.s64 + 4;
	// clrlwi r11,r10,31
	ctx.r11.u64 = ctx.r10.u32 & 0x1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82a85438
	if (!ctx.cr6.eq) goto loc_82A85438;
	// rlwinm r9,r10,31,1,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// subfic r11,r7,31
	ctx.xer.ca = ctx.r7.u32 <= 31;
	ctx.r11.u64 = static_cast<uint64_t>(31) - ctx.r7.u64;
loc_82A852E0:
	// srw r10,r10,r31
	ctx.r10.u64 = ctx.r31.u8 & 0x20 ? 0 : (ctx.r10.u32 >> (ctx.r31.u8 & 0x3F));
loc_82A852E4:
	// and r20,r9,r29
	ctx.r20.u64 = ctx.r9.u64 & ctx.r29.u64;
	// and r8,r9,r28
	ctx.r8.u64 = ctx.r9.u64 & ctx.r28.u64;
	// cmplwi cr6,r20,0
	ctx.cr6.compare<uint32_t>(ctx.r20.u32, 0, ctx.xer);
	// or r9,r8,r27
	ctx.r9.u64 = ctx.r8.u64 | ctx.r27.u64;
	// beq cr6,0x82a852fc
	if (ctx.cr6.eq) goto loc_82A852FC;
	// neg r9,r9
	ctx.r9.s64 = static_cast<int64_t>(-ctx.r9.u64);
loc_82A852FC:
	// rlwinm r8,r6,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 1) & 0xFFFFFFFE;
	// addi r6,r1,-384
	ctx.r6.s64 = ctx.r1.s64 + -384;
	// sthx r9,r8,r6
	REX_STORE_U16(ctx.r8.u32 + ctx.r6.u32, ctx.r9.u16);
	// b 0x82a85518
	goto loc_82A85518;
loc_82A8530C:
	// li r11,31
	ctx.r11.s64 = 31;
loc_82A85310:
	// rlwinm r9,r6,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r24,r24,-1
	ctx.r24.s64 = ctx.r24.s64 + -1;
	// addi r9,r9,3
	ctx.r9.s64 = ctx.r9.s64 + 3;
	// rlwinm r10,r10,31,1,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// stb r9,0(r24)
	REX_STORE_U8(ctx.r24.u32 + 0, ctx.r9.u8);
	// b 0x82a85218
	goto loc_82A85218;
loc_82A85328:
	// clrlwi r9,r10,31
	ctx.r9.u64 = ctx.r10.u32 & 0x1;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// bne cr6,0x82a85310
	if (!ctx.cr6.eq) goto loc_82A85310;
	// cmplw cr6,r11,r7
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, ctx.r7.u32, ctx.xer);
	// rlwinm r9,r10,31,1,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// bge cr6,0x82a85368
	if (!ctx.cr6.lt) goto loc_82A85368;
	// lwz r8,0(r5)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r5.u32 + 0);
	// subf r20,r11,r7
	ctx.r20.u64 = ctx.r7.u64 - ctx.r11.u64;
	// subf r10,r7,r11
	ctx.r10.u64 = ctx.r11.u64 - ctx.r7.u64;
	// addi r5,r5,4
	ctx.r5.s64 = ctx.r5.s64 + 4;
	// slw r19,r8,r11
	ctx.r19.u64 = ctx.r11.u8 & 0x20 ? 0 : (ctx.r8.u32 << (ctx.r11.u8 & 0x3F));
	// addi r11,r10,32
	ctx.r11.s64 = ctx.r10.s64 + 32;
	// or r9,r19,r9
	ctx.r9.u64 = ctx.r19.u64 | ctx.r9.u64;
	// srw r10,r8,r20
	ctx.r10.u64 = ctx.r20.u8 & 0x20 ? 0 : (ctx.r8.u32 >> (ctx.r20.u8 & 0x3F));
	// b 0x82a851f4
	goto loc_82A851F4;
loc_82A85368:
	// subf r11,r7,r11
	ctx.r11.u64 = ctx.r11.u64 - ctx.r7.u64;
	// b 0x82a851f0
	goto loc_82A851F0;
loc_82A85370:
	// li r11,31
	ctx.r11.s64 = 31;
loc_82A85374:
	// rlwinm r9,r6,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r24,r24,-1
	ctx.r24.s64 = ctx.r24.s64 + -1;
	// addi r9,r9,3
	ctx.r9.s64 = ctx.r9.s64 + 3;
	// rlwinm r10,r10,31,1,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// stb r9,0(r24)
	REX_STORE_U8(ctx.r24.u32 + 0, ctx.r9.u8);
	// b 0x82a85268
	goto loc_82A85268;
loc_82A8538C:
	// clrlwi r9,r10,31
	ctx.r9.u64 = ctx.r10.u32 & 0x1;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// bne cr6,0x82a85374
	if (!ctx.cr6.eq) goto loc_82A85374;
	// cmplw cr6,r11,r7
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, ctx.r7.u32, ctx.xer);
	// rlwinm r9,r10,31,1,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// bge cr6,0x82a853cc
	if (!ctx.cr6.lt) goto loc_82A853CC;
	// lwz r8,0(r5)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r5.u32 + 0);
	// subf r20,r11,r7
	ctx.r20.u64 = ctx.r7.u64 - ctx.r11.u64;
	// subf r10,r7,r11
	ctx.r10.u64 = ctx.r11.u64 - ctx.r7.u64;
	// addi r5,r5,4
	ctx.r5.s64 = ctx.r5.s64 + 4;
	// slw r19,r8,r11
	ctx.r19.u64 = ctx.r11.u8 & 0x20 ? 0 : (ctx.r8.u32 << (ctx.r11.u8 & 0x3F));
	// addi r11,r10,32
	ctx.r11.s64 = ctx.r10.s64 + 32;
	// or r9,r19,r9
	ctx.r9.u64 = ctx.r19.u64 | ctx.r9.u64;
	// srw r10,r8,r20
	ctx.r10.u64 = ctx.r20.u8 & 0x20 ? 0 : (ctx.r8.u32 >> (ctx.r20.u8 & 0x3F));
	// b 0x82a85244
	goto loc_82A85244;
loc_82A853CC:
	// subf r11,r7,r11
	ctx.r11.u64 = ctx.r11.u64 - ctx.r7.u64;
	// b 0x82a85240
	goto loc_82A85240;
loc_82A853D4:
	// li r11,31
	ctx.r11.s64 = 31;
loc_82A853D8:
	// rlwinm r9,r6,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r24,r24,-1
	ctx.r24.s64 = ctx.r24.s64 + -1;
	// addi r9,r9,3
	ctx.r9.s64 = ctx.r9.s64 + 3;
	// rlwinm r10,r10,31,1,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// stb r9,0(r24)
	REX_STORE_U8(ctx.r24.u32 + 0, ctx.r9.u8);
	// b 0x82a852b8
	goto loc_82A852B8;
loc_82A853F0:
	// clrlwi r9,r10,31
	ctx.r9.u64 = ctx.r10.u32 & 0x1;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// bne cr6,0x82a853d8
	if (!ctx.cr6.eq) goto loc_82A853D8;
	// cmplw cr6,r11,r7
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, ctx.r7.u32, ctx.xer);
	// rlwinm r9,r10,31,1,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// bge cr6,0x82a85430
	if (!ctx.cr6.lt) goto loc_82A85430;
	// lwz r8,0(r5)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r5.u32 + 0);
	// subf r20,r11,r7
	ctx.r20.u64 = ctx.r7.u64 - ctx.r11.u64;
	// subf r10,r7,r11
	ctx.r10.u64 = ctx.r11.u64 - ctx.r7.u64;
	// addi r5,r5,4
	ctx.r5.s64 = ctx.r5.s64 + 4;
	// slw r19,r8,r11
	ctx.r19.u64 = ctx.r11.u8 & 0x20 ? 0 : (ctx.r8.u32 << (ctx.r11.u8 & 0x3F));
	// addi r11,r10,32
	ctx.r11.s64 = ctx.r10.s64 + 32;
	// or r9,r19,r9
	ctx.r9.u64 = ctx.r19.u64 | ctx.r9.u64;
	// srw r10,r8,r20
	ctx.r10.u64 = ctx.r20.u8 & 0x20 ? 0 : (ctx.r8.u32 >> (ctx.r20.u8 & 0x3F));
	// b 0x82a85294
	goto loc_82A85294;
loc_82A85430:
	// subf r11,r7,r11
	ctx.r11.u64 = ctx.r11.u64 - ctx.r7.u64;
	// b 0x82a85290
	goto loc_82A85290;
loc_82A85438:
	// li r11,31
	ctx.r11.s64 = 31;
loc_82A8543C:
	// rlwinm r9,r6,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r24,r24,-1
	ctx.r24.s64 = ctx.r24.s64 + -1;
	// addi r9,r9,3
	ctx.r9.s64 = ctx.r9.s64 + 3;
	// rlwinm r10,r10,31,1,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// stb r9,0(r24)
	REX_STORE_U8(ctx.r24.u32 + 0, ctx.r9.u8);
	// b 0x82a85518
	goto loc_82A85518;
loc_82A85454:
	// clrlwi r9,r10,31
	ctx.r9.u64 = ctx.r10.u32 & 0x1;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// bne cr6,0x82a8543c
	if (!ctx.cr6.eq) goto loc_82A8543C;
	// cmplw cr6,r11,r7
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, ctx.r7.u32, ctx.xer);
	// rlwinm r9,r10,31,1,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// bge cr6,0x82a85494
	if (!ctx.cr6.lt) goto loc_82A85494;
	// lwz r8,0(r5)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r5.u32 + 0);
	// subf r20,r11,r7
	ctx.r20.u64 = ctx.r7.u64 - ctx.r11.u64;
	// subf r10,r7,r11
	ctx.r10.u64 = ctx.r11.u64 - ctx.r7.u64;
	// addi r5,r5,4
	ctx.r5.s64 = ctx.r5.s64 + 4;
	// slw r19,r8,r11
	ctx.r19.u64 = ctx.r11.u8 & 0x20 ? 0 : (ctx.r8.u32 << (ctx.r11.u8 & 0x3F));
	// addi r11,r10,32
	ctx.r11.s64 = ctx.r10.s64 + 32;
	// or r9,r19,r9
	ctx.r9.u64 = ctx.r19.u64 | ctx.r9.u64;
	// srw r10,r8,r20
	ctx.r10.u64 = ctx.r20.u8 & 0x20 ? 0 : (ctx.r8.u32 >> (ctx.r20.u8 & 0x3F));
	// b 0x82a852e4
	goto loc_82A852E4;
loc_82A85494:
	// subf r11,r7,r11
	ctx.r11.u64 = ctx.r11.u64 - ctx.r7.u64;
	// b 0x82a852e0
	goto loc_82A852E0;
loc_82A8549C:
	// rlwinm r6,r8,30,2,31
	ctx.r6.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 30) & 0x3FFFFFFF;
	// cmplw cr6,r11,r7
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, ctx.r7.u32, ctx.xer);
	// bge cr6,0x82a854d8
	if (!ctx.cr6.lt) goto loc_82A854D8;
	// lwz r9,0(r5)
	ctx.r9.u64 = REX_LOAD_U32(ctx.r5.u32 + 0);
	// subfic r20,r7,32
	ctx.xer.ca = ctx.r7.u32 <= 32;
	ctx.r20.u64 = static_cast<uint64_t>(32) - ctx.r7.u64;
	// subf r19,r11,r7
	ctx.r19.u64 = ctx.r7.u64 - ctx.r11.u64;
	// subf r8,r7,r11
	ctx.r8.u64 = ctx.r11.u64 - ctx.r7.u64;
	// addi r5,r5,4
	ctx.r5.s64 = ctx.r5.s64 + 4;
	// slw r18,r9,r11
	ctx.r18.u64 = ctx.r11.u8 & 0x20 ? 0 : (ctx.r9.u32 << (ctx.r11.u8 & 0x3F));
	// srw r20,r26,r20
	ctx.r20.u64 = ctx.r20.u8 & 0x20 ? 0 : (ctx.r26.u32 >> (ctx.r20.u8 & 0x3F));
	// or r10,r18,r10
	ctx.r10.u64 = ctx.r18.u64 | ctx.r10.u64;
	// addi r11,r8,32
	ctx.r11.s64 = ctx.r8.s64 + 32;
	// and r8,r20,r10
	ctx.r8.u64 = ctx.r20.u64 & ctx.r10.u64;
	// srw r10,r9,r19
	ctx.r10.u64 = ctx.r19.u8 & 0x20 ? 0 : (ctx.r9.u32 >> (ctx.r19.u8 & 0x3F));
	// b 0x82a854ec
	goto loc_82A854EC;
loc_82A854D8:
	// subfic r9,r7,32
	ctx.xer.ca = ctx.r7.u32 <= 32;
	ctx.r9.u64 = static_cast<uint64_t>(32) - ctx.r7.u64;
	// subf r11,r7,r11
	ctx.r11.u64 = ctx.r11.u64 - ctx.r7.u64;
	// srw r9,r26,r9
	ctx.r9.u64 = ctx.r9.u8 & 0x20 ? 0 : (ctx.r26.u32 >> (ctx.r9.u8 & 0x3F));
	// and r8,r9,r10
	ctx.r8.u64 = ctx.r9.u64 & ctx.r10.u64;
	// srw r10,r10,r7
	ctx.r10.u64 = ctx.r7.u8 & 0x20 ? 0 : (ctx.r10.u32 >> (ctx.r7.u8 & 0x3F));
loc_82A854EC:
	// and r9,r8,r28
	ctx.r9.u64 = ctx.r8.u64 & ctx.r28.u64;
	// and r8,r8,r29
	ctx.r8.u64 = ctx.r8.u64 & ctx.r29.u64;
	// or r9,r9,r27
	ctx.r9.u64 = ctx.r9.u64 | ctx.r27.u64;
	// cmplwi cr6,r8,0
	ctx.cr6.compare<uint32_t>(ctx.r8.u32, 0, ctx.xer);
	// beq cr6,0x82a85504
	if (ctx.cr6.eq) goto loc_82A85504;
	// neg r9,r9
	ctx.r9.s64 = static_cast<int64_t>(-ctx.r9.u64);
loc_82A85504:
	// rlwinm r8,r6,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r6.u32 | (ctx.r6.u64 << 32), 1) & 0xFFFFFFFE;
	// stb r21,0(r30)
	REX_STORE_U8(ctx.r30.u32 + 0, ctx.r21.u8);
	// addi r6,r1,-384
	ctx.r6.s64 = ctx.r1.s64 + -384;
	// sthx r9,r8,r6
	REX_STORE_U16(ctx.r8.u32 + ctx.r6.u32, ctx.r9.u16);
loc_82A85514:
	// addi r30,r30,1
	ctx.r30.s64 = ctx.r30.s64 + 1;
loc_82A85518:
	// cmplw cr6,r30,r23
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, ctx.r23.u32, ctx.xer);
	// blt cr6,0x82a85100
	if (ctx.cr6.lt) goto loc_82A85100;
loc_82A85520:
	// addi r7,r7,-1
	ctx.r7.s64 = ctx.r7.s64 + -1;
	// srawi r27,r27,1
	ctx.xer.ca = (ctx.r27.s32 < 0) & ((ctx.r27.u32 & 0x1) != 0);
	ctx.r27.s64 = ctx.r27.s32 >> 1;
	// cmplwi cr6,r7,1
	ctx.cr6.compare<uint32_t>(ctx.r7.u32, 1, ctx.xer);
	// bgt cr6,0x82a850e4
	if (ctx.cr6.gt) goto loc_82A850E4;
loc_82A85530:
	// cmplwi cr6,r25,0
	ctx.cr6.compare<uint32_t>(ctx.r25.u32, 0, ctx.xer);
	// beq cr6,0x82a85894
	if (ctx.cr6.eq) goto loc_82A85894;
	// mr r6,r24
	ctx.r6.u64 = ctx.r24.u64;
	// cmplw cr6,r24,r23
	ctx.cr6.compare<uint32_t>(ctx.r24.u32, ctx.r23.u32, ctx.xer);
	// bge cr6,0x82a85894
	if (!ctx.cr6.lt) goto loc_82A85894;
loc_82A85544:
	// lbz r8,0(r6)
	ctx.r8.u64 = REX_LOAD_U8(ctx.r6.u32 + 0);
	// cmplwi cr6,r8,0
	ctx.cr6.compare<uint32_t>(ctx.r8.u32, 0, ctx.xer);
	// beq cr6,0x82a85888
	if (ctx.cr6.eq) goto loc_82A85888;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82a8556c
	if (!ctx.cr6.eq) goto loc_82A8556C;
	// lwz r9,0(r5)
	ctx.r9.u64 = REX_LOAD_U32(ctx.r5.u32 + 0);
	// li r11,31
	ctx.r11.s64 = 31;
	// addi r5,r5,4
	ctx.r5.s64 = ctx.r5.s64 + 4;
	// rlwinm r10,r9,31,1,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 31) & 0x7FFFFFFF;
	// b 0x82a85578
	goto loc_82A85578;
loc_82A8556C:
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// rlwinm r10,r10,31,1,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
loc_82A85578:
	// clrlwi r9,r9,31
	ctx.r9.u64 = ctx.r9.u32 & 0x1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// beq cr6,0x82a85888
	if (ctx.cr6.eq) goto loc_82A85888;
	// clrlwi r9,r8,30
	ctx.r9.u64 = ctx.r8.u32 & 0x3;
	// cmplwi cr6,r9,3
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 3, ctx.xer);
	// bgt cr6,0x82a85888
	if (ctx.cr6.gt) goto loc_82A85888;
	// lis r12,-32088
	ctx.r12.s64 = -2102919168;
	// addi r12,r12,21928
	ctx.r12.s64 = ctx.r12.s64 + 21928;
	// rlwinm r0,r9,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = REX_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r9.u32) {
	case 0:
		goto loc_82A855B8;
	case 1:
		goto loc_82A855CC;
	case 2:
		goto loc_82A85604;
	case 3:
		goto loc_82A85840;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_82A855B8:
	// rlwinm r7,r8,30,2,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 30) & 0x3FFFFFFF;
	// rlwinm r9,r7,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r9,r9,17
	ctx.r9.s64 = ctx.r9.s64 + 17;
	// stb r9,0(r6)
	REX_STORE_U8(ctx.r6.u32 + 0, ctx.r9.u8);
	// b 0x82a85610
	goto loc_82A85610;
loc_82A855CC:
	// rlwinm r9,r8,0,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 0) & 0xFFFFFFFC;
	// addi r8,r9,2
	ctx.r8.s64 = ctx.r9.s64 + 2;
	// addi r7,r9,18
	ctx.r7.s64 = ctx.r9.s64 + 18;
	// addi r31,r9,34
	ctx.r31.s64 = ctx.r9.s64 + 34;
	// addi r9,r9,50
	ctx.r9.s64 = ctx.r9.s64 + 50;
	// stb r8,0(r6)
	REX_STORE_U8(ctx.r6.u32 + 0, ctx.r8.u8);
	// mr r8,r9
	ctx.r8.u64 = ctx.r9.u64;
	// addi r9,r23,1
	ctx.r9.s64 = ctx.r23.s64 + 1;
	// stb r7,0(r23)
	REX_STORE_U8(ctx.r23.u32 + 0, ctx.r7.u8);
	// stb r31,0(r9)
	REX_STORE_U8(ctx.r9.u32 + 0, ctx.r31.u8);
	// addi r9,r9,1
	ctx.r9.s64 = ctx.r9.s64 + 1;
	// addi r23,r9,1
	ctx.r23.s64 = ctx.r9.s64 + 1;
	// stb r8,0(r9)
	REX_STORE_U8(ctx.r9.u32 + 0, ctx.r8.u8);
	// b 0x82a8588c
	goto loc_82A8588C;
loc_82A85604:
	// stb r21,0(r6)
	REX_STORE_U8(ctx.r6.u32 + 0, ctx.r21.u8);
	// rlwinm r7,r8,30,2,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 30) & 0x3FFFFFFF;
	// addi r6,r6,1
	ctx.r6.s64 = ctx.r6.s64 + 1;
loc_82A85610:
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82a8562c
	if (!ctx.cr6.eq) goto loc_82A8562C;
	// lwz r8,0(r5)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r5.u32 + 0);
	// li r9,31
	ctx.r9.s64 = 31;
	// addi r5,r5,4
	ctx.r5.s64 = ctx.r5.s64 + 4;
	// rlwinm r10,r8,31,1,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 31) & 0x7FFFFFFF;
	// b 0x82a85638
	goto loc_82A85638;
loc_82A8562C:
	// mr r8,r10
	ctx.r8.u64 = ctx.r10.u64;
	// rlwinm r10,r10,31,1,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// addi r9,r11,-1
	ctx.r9.s64 = ctx.r11.s64 + -1;
loc_82A85638:
	// clrlwi r11,r8,31
	ctx.r11.u64 = ctx.r8.u32 & 0x1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// beq cr6,0x82a85658
	if (ctx.cr6.eq) goto loc_82A85658;
	// rlwinm r11,r7,2,0,29
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r24,r24,-1
	ctx.r24.s64 = ctx.r24.s64 + -1;
	// addi r11,r11,3
	ctx.r11.s64 = ctx.r11.s64 + 3;
	// stb r11,0(r24)
	REX_STORE_U8(ctx.r24.u32 + 0, ctx.r11.u8);
	// b 0x82a85698
	goto loc_82A85698;
loc_82A85658:
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// bne cr6,0x82a85674
	if (!ctx.cr6.eq) goto loc_82A85674;
	// lwz r11,0(r5)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r5.u32 + 0);
	// li r9,31
	ctx.r9.s64 = 31;
	// addi r5,r5,4
	ctx.r5.s64 = ctx.r5.s64 + 4;
	// rlwinm r10,r11,31,1,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 31) & 0x7FFFFFFF;
	// b 0x82a85680
	goto loc_82A85680;
loc_82A85674:
	// mr r11,r10
	ctx.r11.u64 = ctx.r10.u64;
	// rlwinm r10,r10,31,1,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
loc_82A85680:
	// clrlwi r11,r11,31
	ctx.r11.u64 = ctx.r11.u32 & 0x1;
	// rlwinm r8,r7,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 1) & 0xFFFFFFFE;
	// neg r11,r11
	ctx.r11.s64 = static_cast<int64_t>(-ctx.r11.u64);
	// addi r31,r1,-384
	ctx.r31.s64 = ctx.r1.s64 + -384;
	// rlwimi r11,r22,0,31,15
	ctx.r11.u64 = (__builtin_rotateleft64(ctx.r22.u32 | (ctx.r22.u64 << 32), 0) & 0xFFFFFFFFFFFF0001) | (ctx.r11.u64 & 0xFFFE);
	// sthx r11,r8,r31
	REX_STORE_U16(ctx.r8.u32 + ctx.r31.u32, ctx.r11.u16);
loc_82A85698:
	// addi r7,r7,1
	ctx.r7.s64 = ctx.r7.s64 + 1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// bne cr6,0x82a856b8
	if (!ctx.cr6.eq) goto loc_82A856B8;
	// lwz r8,0(r5)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r5.u32 + 0);
	// li r10,31
	ctx.r10.s64 = 31;
	// addi r5,r5,4
	ctx.r5.s64 = ctx.r5.s64 + 4;
	// rlwinm r11,r8,31,1,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 31) & 0x7FFFFFFF;
	// b 0x82a856c4
	goto loc_82A856C4;
loc_82A856B8:
	// mr r8,r10
	ctx.r8.u64 = ctx.r10.u64;
	// rlwinm r11,r10,31,1,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// addi r10,r9,-1
	ctx.r10.s64 = ctx.r9.s64 + -1;
loc_82A856C4:
	// clrlwi r9,r8,31
	ctx.r9.u64 = ctx.r8.u32 & 0x1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// beq cr6,0x82a856e4
	if (ctx.cr6.eq) goto loc_82A856E4;
	// rlwinm r9,r7,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r24,r24,-1
	ctx.r24.s64 = ctx.r24.s64 + -1;
	// addi r9,r9,3
	ctx.r9.s64 = ctx.r9.s64 + 3;
	// stb r9,0(r24)
	REX_STORE_U8(ctx.r24.u32 + 0, ctx.r9.u8);
	// b 0x82a85724
	goto loc_82A85724;
loc_82A856E4:
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// bne cr6,0x82a85700
	if (!ctx.cr6.eq) goto loc_82A85700;
	// lwz r9,0(r5)
	ctx.r9.u64 = REX_LOAD_U32(ctx.r5.u32 + 0);
	// li r10,31
	ctx.r10.s64 = 31;
	// addi r5,r5,4
	ctx.r5.s64 = ctx.r5.s64 + 4;
	// rlwinm r11,r9,31,1,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 31) & 0x7FFFFFFF;
	// b 0x82a8570c
	goto loc_82A8570C;
loc_82A85700:
	// mr r9,r11
	ctx.r9.u64 = ctx.r11.u64;
	// rlwinm r11,r11,31,1,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 31) & 0x7FFFFFFF;
	// addi r10,r10,-1
	ctx.r10.s64 = ctx.r10.s64 + -1;
loc_82A8570C:
	// clrlwi r9,r9,31
	ctx.r9.u64 = ctx.r9.u32 & 0x1;
	// rlwinm r8,r7,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 1) & 0xFFFFFFFE;
	// neg r9,r9
	ctx.r9.s64 = static_cast<int64_t>(-ctx.r9.u64);
	// addi r31,r1,-384
	ctx.r31.s64 = ctx.r1.s64 + -384;
	// rlwimi r9,r22,0,31,15
	ctx.r9.u64 = (__builtin_rotateleft64(ctx.r22.u32 | (ctx.r22.u64 << 32), 0) & 0xFFFFFFFFFFFF0001) | (ctx.r9.u64 & 0xFFFE);
	// sthx r9,r8,r31
	REX_STORE_U16(ctx.r8.u32 + ctx.r31.u32, ctx.r9.u16);
loc_82A85724:
	// addi r7,r7,1
	ctx.r7.s64 = ctx.r7.s64 + 1;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// bne cr6,0x82a85744
	if (!ctx.cr6.eq) goto loc_82A85744;
	// lwz r8,0(r5)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r5.u32 + 0);
	// li r9,31
	ctx.r9.s64 = 31;
	// addi r5,r5,4
	ctx.r5.s64 = ctx.r5.s64 + 4;
	// rlwinm r11,r8,31,1,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 31) & 0x7FFFFFFF;
	// b 0x82a85750
	goto loc_82A85750;
loc_82A85744:
	// mr r8,r11
	ctx.r8.u64 = ctx.r11.u64;
	// rlwinm r11,r11,31,1,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 31) & 0x7FFFFFFF;
	// addi r9,r10,-1
	ctx.r9.s64 = ctx.r10.s64 + -1;
loc_82A85750:
	// clrlwi r10,r8,31
	ctx.r10.u64 = ctx.r8.u32 & 0x1;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x82a85770
	if (ctx.cr6.eq) goto loc_82A85770;
	// rlwinm r10,r7,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r24,r24,-1
	ctx.r24.s64 = ctx.r24.s64 + -1;
	// addi r10,r10,3
	ctx.r10.s64 = ctx.r10.s64 + 3;
	// stb r10,0(r24)
	REX_STORE_U8(ctx.r24.u32 + 0, ctx.r10.u8);
	// b 0x82a857b0
	goto loc_82A857B0;
loc_82A85770:
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// bne cr6,0x82a8578c
	if (!ctx.cr6.eq) goto loc_82A8578C;
	// lwz r10,0(r5)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r5.u32 + 0);
	// li r9,31
	ctx.r9.s64 = 31;
	// addi r5,r5,4
	ctx.r5.s64 = ctx.r5.s64 + 4;
	// rlwinm r11,r10,31,1,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// b 0x82a85798
	goto loc_82A85798;
loc_82A8578C:
	// mr r10,r11
	ctx.r10.u64 = ctx.r11.u64;
	// rlwinm r11,r11,31,1,31
	ctx.r11.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 31) & 0x7FFFFFFF;
	// addi r9,r9,-1
	ctx.r9.s64 = ctx.r9.s64 + -1;
loc_82A85798:
	// clrlwi r10,r10,31
	ctx.r10.u64 = ctx.r10.u32 & 0x1;
	// rlwinm r8,r7,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 1) & 0xFFFFFFFE;
	// neg r10,r10
	ctx.r10.s64 = static_cast<int64_t>(-ctx.r10.u64);
	// addi r31,r1,-384
	ctx.r31.s64 = ctx.r1.s64 + -384;
	// rlwimi r10,r22,0,31,15
	ctx.r10.u64 = (__builtin_rotateleft64(ctx.r22.u32 | (ctx.r22.u64 << 32), 0) & 0xFFFFFFFFFFFF0001) | (ctx.r10.u64 & 0xFFFE);
	// sthx r10,r8,r31
	REX_STORE_U16(ctx.r8.u32 + ctx.r31.u32, ctx.r10.u16);
loc_82A857B0:
	// addi r7,r7,1
	ctx.r7.s64 = ctx.r7.s64 + 1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// bne cr6,0x82a857d0
	if (!ctx.cr6.eq) goto loc_82A857D0;
	// lwz r8,0(r5)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r5.u32 + 0);
	// li r11,31
	ctx.r11.s64 = 31;
	// addi r5,r5,4
	ctx.r5.s64 = ctx.r5.s64 + 4;
	// rlwinm r10,r8,31,1,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 31) & 0x7FFFFFFF;
	// b 0x82a857dc
	goto loc_82A857DC;
loc_82A857D0:
	// mr r8,r11
	ctx.r8.u64 = ctx.r11.u64;
	// rlwinm r10,r11,31,1,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r11.u32 | (ctx.r11.u64 << 32), 31) & 0x7FFFFFFF;
	// addi r11,r9,-1
	ctx.r11.s64 = ctx.r9.s64 + -1;
loc_82A857DC:
	// clrlwi r9,r8,31
	ctx.r9.u64 = ctx.r8.u32 & 0x1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// beq cr6,0x82a857fc
	if (ctx.cr6.eq) goto loc_82A857FC;
	// rlwinm r9,r7,2,0,29
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r24,r24,-1
	ctx.r24.s64 = ctx.r24.s64 + -1;
	// addi r9,r9,3
	ctx.r9.s64 = ctx.r9.s64 + 3;
	// stb r9,0(r24)
	REX_STORE_U8(ctx.r24.u32 + 0, ctx.r9.u8);
	// b 0x82a8588c
	goto loc_82A8588C;
loc_82A857FC:
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82a85818
	if (!ctx.cr6.eq) goto loc_82A85818;
	// lwz r9,0(r5)
	ctx.r9.u64 = REX_LOAD_U32(ctx.r5.u32 + 0);
	// li r11,31
	ctx.r11.s64 = 31;
	// addi r5,r5,4
	ctx.r5.s64 = ctx.r5.s64 + 4;
	// rlwinm r10,r9,31,1,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 31) & 0x7FFFFFFF;
	// b 0x82a85824
	goto loc_82A85824;
loc_82A85818:
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// rlwinm r10,r10,31,1,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
loc_82A85824:
	// clrlwi r9,r9,31
	ctx.r9.u64 = ctx.r9.u32 & 0x1;
	// rlwinm r8,r7,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 1) & 0xFFFFFFFE;
	// neg r9,r9
	ctx.r9.s64 = static_cast<int64_t>(-ctx.r9.u64);
	// addi r7,r1,-384
	ctx.r7.s64 = ctx.r1.s64 + -384;
	// rlwimi r9,r22,0,31,15
	ctx.r9.u64 = (__builtin_rotateleft64(ctx.r22.u32 | (ctx.r22.u64 << 32), 0) & 0xFFFFFFFFFFFF0001) | (ctx.r9.u64 & 0xFFFE);
	// sthx r9,r8,r7
	REX_STORE_U16(ctx.r8.u32 + ctx.r7.u32, ctx.r9.u16);
	// b 0x82a8588c
	goto loc_82A8588C;
loc_82A85840:
	// rlwinm r8,r8,30,2,31
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 30) & 0x3FFFFFFF;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82a85860
	if (!ctx.cr6.eq) goto loc_82A85860;
	// lwz r9,0(r5)
	ctx.r9.u64 = REX_LOAD_U32(ctx.r5.u32 + 0);
	// li r11,31
	ctx.r11.s64 = 31;
	// addi r5,r5,4
	ctx.r5.s64 = ctx.r5.s64 + 4;
	// rlwinm r10,r9,31,1,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 31) & 0x7FFFFFFF;
	// b 0x82a8586c
	goto loc_82A8586C;
loc_82A85860:
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// rlwinm r10,r10,31,1,31
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
loc_82A8586C:
	// clrlwi r9,r9,31
	ctx.r9.u64 = ctx.r9.u32 & 0x1;
	// stb r21,0(r6)
	REX_STORE_U8(ctx.r6.u32 + 0, ctx.r21.u8);
	// rlwinm r8,r8,1,0,30
	ctx.r8.u64 = __builtin_rotateleft64(ctx.r8.u32 | (ctx.r8.u64 << 32), 1) & 0xFFFFFFFE;
	// neg r9,r9
	ctx.r9.s64 = static_cast<int64_t>(-ctx.r9.u64);
	// addi r7,r1,-384
	ctx.r7.s64 = ctx.r1.s64 + -384;
	// rlwimi r9,r22,0,31,15
	ctx.r9.u64 = (__builtin_rotateleft64(ctx.r22.u32 | (ctx.r22.u64 << 32), 0) & 0xFFFFFFFFFFFF0001) | (ctx.r9.u64 & 0xFFFE);
	// sthx r9,r8,r7
	REX_STORE_U16(ctx.r8.u32 + ctx.r7.u32, ctx.r9.u16);
loc_82A85888:
	// addi r6,r6,1
	ctx.r6.s64 = ctx.r6.s64 + 1;
loc_82A8588C:
	// cmplw cr6,r6,r23
	ctx.cr6.compare<uint32_t>(ctx.r6.u32, ctx.r23.u32, ctx.xer);
	// blt cr6,0x82a85544
	if (ctx.cr6.lt) goto loc_82A85544;
loc_82A85894:
	// lhz r9,-382(r1)
	ctx.r9.u64 = REX_LOAD_U16(ctx.r1.u32 + -382);
	// lwz r8,-376(r1)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r1.u32 + -376);
	// stw r10,0(r4)
	REX_STORE_U32(ctx.r4.u32 + 0, ctx.r10.u32);
	// stw r11,8(r4)
	REX_STORE_U32(ctx.r4.u32 + 8, ctx.r11.u32);
	// lwz r7,-368(r1)
	ctx.r7.u64 = REX_LOAD_U32(ctx.r1.u32 + -368);
	// sth r9,2(r3)
	REX_STORE_U16(ctx.r3.u32 + 2, ctx.r9.u16);
	// stw r8,4(r3)
	REX_STORE_U32(ctx.r3.u32 + 4, ctx.r8.u32);
	// lwz r6,-360(r1)
	ctx.r6.u64 = REX_LOAD_U32(ctx.r1.u32 + -360);
	// lwz r10,-380(r1)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r1.u32 + -380);
	// lwz r11,-372(r1)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r1.u32 + -372);
	// lwz r9,-364(r1)
	ctx.r9.u64 = REX_LOAD_U32(ctx.r1.u32 + -364);
	// lwz r8,-356(r1)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r1.u32 + -356);
	// stw r7,8(r3)
	REX_STORE_U32(ctx.r3.u32 + 8, ctx.r7.u32);
	// stw r6,12(r3)
	REX_STORE_U32(ctx.r3.u32 + 12, ctx.r6.u32);
	// stw r10,16(r3)
	REX_STORE_U32(ctx.r3.u32 + 16, ctx.r10.u32);
	// stw r11,20(r3)
	REX_STORE_U32(ctx.r3.u32 + 20, ctx.r11.u32);
	// stw r9,24(r3)
	REX_STORE_U32(ctx.r3.u32 + 24, ctx.r9.u32);
	// stw r8,28(r3)
	REX_STORE_U32(ctx.r3.u32 + 28, ctx.r8.u32);
	// lwz r7,-336(r1)
	ctx.r7.u64 = REX_LOAD_U32(ctx.r1.u32 + -336);
	// lwz r6,-296(r1)
	ctx.r6.u64 = REX_LOAD_U32(ctx.r1.u32 + -296);
	// lwz r10,-352(r1)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r1.u32 + -352);
	// lwz r11,-344(r1)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r1.u32 + -344);
	// lwz r9,-332(r1)
	ctx.r9.u64 = REX_LOAD_U32(ctx.r1.u32 + -332);
	// lwz r8,-292(r1)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r1.u32 + -292);
	// stw r7,32(r3)
	REX_STORE_U32(ctx.r3.u32 + 32, ctx.r7.u32);
	// stw r6,36(r3)
	REX_STORE_U32(ctx.r3.u32 + 36, ctx.r6.u32);
	// stw r10,40(r3)
	REX_STORE_U32(ctx.r3.u32 + 40, ctx.r10.u32);
	// stw r11,44(r3)
	REX_STORE_U32(ctx.r3.u32 + 44, ctx.r11.u32);
	// stw r9,48(r3)
	REX_STORE_U32(ctx.r3.u32 + 48, ctx.r9.u32);
	// stw r8,52(r3)
	REX_STORE_U32(ctx.r3.u32 + 52, ctx.r8.u32);
	// lwz r7,-348(r1)
	ctx.r7.u64 = REX_LOAD_U32(ctx.r1.u32 + -348);
	// lwz r6,-340(r1)
	ctx.r6.u64 = REX_LOAD_U32(ctx.r1.u32 + -340);
	// lwz r10,-328(r1)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r1.u32 + -328);
	// lwz r11,-320(r1)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r1.u32 + -320);
	// lwz r9,-288(r1)
	ctx.r9.u64 = REX_LOAD_U32(ctx.r1.u32 + -288);
	// lwz r8,-280(r1)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r1.u32 + -280);
	// stw r7,56(r3)
	REX_STORE_U32(ctx.r3.u32 + 56, ctx.r7.u32);
	// stw r6,60(r3)
	REX_STORE_U32(ctx.r3.u32 + 60, ctx.r6.u32);
	// stw r10,64(r3)
	REX_STORE_U32(ctx.r3.u32 + 64, ctx.r10.u32);
	// stw r11,68(r3)
	REX_STORE_U32(ctx.r3.u32 + 68, ctx.r11.u32);
	// stw r9,72(r3)
	REX_STORE_U32(ctx.r3.u32 + 72, ctx.r9.u32);
	// stw r8,76(r3)
	REX_STORE_U32(ctx.r3.u32 + 76, ctx.r8.u32);
	// lwz r7,-324(r1)
	ctx.r7.u64 = REX_LOAD_U32(ctx.r1.u32 + -324);
	// lwz r6,-316(r1)
	ctx.r6.u64 = REX_LOAD_U32(ctx.r1.u32 + -316);
	// lwz r10,-284(r1)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r1.u32 + -284);
	// lwz r11,-276(r1)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r1.u32 + -276);
	// lwz r9,-312(r1)
	ctx.r9.u64 = REX_LOAD_U32(ctx.r1.u32 + -312);
	// lwz r8,-304(r1)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r1.u32 + -304);
	// stw r7,80(r3)
	REX_STORE_U32(ctx.r3.u32 + 80, ctx.r7.u32);
	// stw r6,84(r3)
	REX_STORE_U32(ctx.r3.u32 + 84, ctx.r6.u32);
	// stw r10,88(r3)
	REX_STORE_U32(ctx.r3.u32 + 88, ctx.r10.u32);
	// stw r11,92(r3)
	REX_STORE_U32(ctx.r3.u32 + 92, ctx.r11.u32);
	// stw r9,96(r3)
	REX_STORE_U32(ctx.r3.u32 + 96, ctx.r9.u32);
	// stw r8,100(r3)
	REX_STORE_U32(ctx.r3.u32 + 100, ctx.r8.u32);
	// lwz r7,-272(r1)
	ctx.r7.u64 = REX_LOAD_U32(ctx.r1.u32 + -272);
	// lwz r6,-264(r1)
	ctx.r6.u64 = REX_LOAD_U32(ctx.r1.u32 + -264);
	// lwz r10,-308(r1)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r1.u32 + -308);
	// lwz r11,-300(r1)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r1.u32 + -300);
	// lwz r9,-268(r1)
	ctx.r9.u64 = REX_LOAD_U32(ctx.r1.u32 + -268);
	// lwz r8,-260(r1)
	ctx.r8.u64 = REX_LOAD_U32(ctx.r1.u32 + -260);
	// stw r5,4(r4)
	REX_STORE_U32(ctx.r4.u32 + 4, ctx.r5.u32);
	// stw r7,104(r3)
	REX_STORE_U32(ctx.r3.u32 + 104, ctx.r7.u32);
	// stw r6,108(r3)
	REX_STORE_U32(ctx.r3.u32 + 108, ctx.r6.u32);
	// stw r10,112(r3)
	REX_STORE_U32(ctx.r3.u32 + 112, ctx.r10.u32);
	// stw r11,116(r3)
	REX_STORE_U32(ctx.r3.u32 + 116, ctx.r11.u32);
	// stw r9,120(r3)
	REX_STORE_U32(ctx.r3.u32 + 120, ctx.r9.u32);
	// stw r8,124(r3)
	REX_STORE_U32(ctx.r3.u32 + 124, ctx.r8.u32);
	// b 0x829ff7f0
	__restgprlr_18(ctx, base);
	return;
}

DEFINE_REX_FUNC(sub_82A859A8) {
	REX_FUNC_PROLOGUE();
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff7b0
	ctx.lr = 0x82A859B0;
	__savegprlr_22(ctx, base);
	// lwz r11,8(r4)
	ctx.r11.u64 = REX_LOAD_U32(ctx.r4.u32 + 8);
	// li r25,0
	ctx.r25.s64 = 0;
	// lwz r31,4(r4)
	ctx.r31.u64 = REX_LOAD_U32(ctx.r4.u32 + 4);
	// lwz r10,0(r4)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r4.u32 + 0);
	// mr r30,r25
	ctx.r30.u64 = ctx.r25.u64;
	// cmplwi cr6,r11,3
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 3, ctx.xer);
	// bge cr6,0x82a859f4
	if (!ctx.cr6.lt) goto loc_82A859F4;
	// lwz r9,0(r31)
	ctx.r9.u64 = REX_LOAD_U32(ctx.r31.u32 + 0);
	// subfic r8,r11,3
	ctx.xer.ca = ctx.r11.u32 <= 3;
	ctx.r8.u64 = static_cast<uint64_t>(3) - ctx.r11.u64;
	// clrlwi r10,r10,24
	ctx.r10.u64 = ctx.r10.u32 & 0xFF;
	// addi r31,r31,4
	ctx.r31.s64 = ctx.r31.s64 + 4;
	// slw r7,r9,r11
	ctx.r7.u64 = ctx.r11.u8 & 0x20 ? 0 : (ctx.r9.u32 << (ctx.r11.u8 & 0x3F));
	// addi r11,r11,29
	ctx.r11.s64 = ctx.r11.s64 + 29;
	// clrlwi r7,r7,24
	ctx.r7.u64 = ctx.r7.u32 & 0xFF;
	// or r10,r7,r10
	ctx.r10.u64 = ctx.r7.u64 | ctx.r10.u64;
	// srw r7,r9,r8
	ctx.r7.u64 = ctx.r8.u8 & 0x20 ? 0 : (ctx.r9.u32 >> (ctx.r8.u8 & 0x3F));
	// b 0x82a859fc
	goto loc_82A859FC;
loc_82A859F4:
	// rlwinm r7,r10,29,3,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 29) & 0x1FFFFFFF;
	// addi r11,r11,-3
	ctx.r11.s64 = ctx.r11.s64 + -3;
loc_82A859FC:
	// li r8,16
	ctx.r8.s64 = 16;
	// clrlwi r10,r10,29
	ctx.r10.u64 = ctx.r10.u32 & 0x7;
	// li r9,1
	ctx.r9.s64 = 1;
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r28,r1,-236
	ctx.r28.s64 = ctx.r1.s64 + -236;
	// stb r8,-236(r1)
	REX_STORE_U8(ctx.r1.u32 + -236, ctx.r8.u8);
	// li r8,96
	ctx.r8.s64 = 96;
	// addi r26,r1,-232
	ctx.r26.s64 = ctx.r1.s64 + -232;
	// mr r29,r25
	ctx.r29.u64 = ctx.r25.u64;
	// mr r24,r10
	ctx.r24.u64 = ctx.r10.u64;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// stb r8,-235(r1)
	REX_STORE_U8(ctx.r1.u32 + -235, ctx.r8.u8);
	// li r8,176
	ctx.r8.s64 = 176;
	// stb r8,-234(r1)
	REX_STORE_U8(ctx.r1.u32 + -234, ctx.r8.u8);
	// li r8,2
	ctx.r8.s64 = 2;
	// stb r8,-233(r1)
	REX_STORE_U8(ctx.r1.u32 + -233, ctx.r8.u8);
	// addi r8,r10,-1
	ctx.r8.s64 = ctx.r10.s64 + -1;
	// slw r27,r9,r8
	ctx.r27.u64 = ctx.r8.u8 & 0x20 ? 0 : (ctx.r9.u32 << (ctx.r8.u8 & 0x3F));
	// beq cr6,0x82a85eb4
	if (ctx.cr6.eq) goto loc_82A85EB4;
loc_82A85A48:
	// mr r6,r25
	ctx.r6.u64 = ctx.r25.u64;
	// cmpwi cr6,r29,0
	ctx.cr6.compare<int32_t>(ctx.r29.s32, 0, ctx.xer);
	// ble cr6,0x82a85ac8
	if (!ctx.cr6.gt) goto loc_82A85AC8;
loc_82A85A54:
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82a85a70
	if (!ctx.cr6.eq) goto loc_82A85A70;
	// lwz r10,0(r31)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r31.u32 + 0);
	// li r11,31
	ctx.r11.s64 = 31;
	// addi r31,r31,4
	ctx.r31.s64 = ctx.r31.s64 + 4;
	// rlwinm r7,r10,31,1,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// b 0x82a85a7c
	goto loc_82A85A7C;
loc_82A85A70:
	// mr r10,r7
	ctx.r10.u64 = ctx.r7.u64;
	// rlwinm r7,r7,31,1,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 31) & 0x7FFFFFFF;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
loc_82A85A7C:
	// clrlwi r10,r10,31
	ctx.r10.u64 = ctx.r10.u32 & 0x1;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x82a85abc
	if (ctx.cr6.eq) goto loc_82A85ABC;
	// addi r10,r1,-160
	ctx.r10.s64 = ctx.r1.s64 + -160;
	// lbzx r8,r6,r10
	ctx.r8.u64 = REX_LOAD_U8(ctx.r6.u32 + ctx.r10.u32);
	// lbzx r10,r8,r3
	ctx.r10.u64 = REX_LOAD_U8(ctx.r8.u32 + ctx.r3.u32);
	// extsb r9,r10
	ctx.r9.s64 = ctx.r10.s8;
	// neg r10,r27
	ctx.r10.s64 = static_cast<int64_t>(-ctx.r27.u64);
	// cmpwi cr6,r9,0
	ctx.cr6.compare<int32_t>(ctx.r9.s32, 0, ctx.xer);
	// blt cr6,0x82a85aa8
	if (ctx.cr6.lt) goto loc_82A85AA8;
	// mr r10,r27
	ctx.r10.u64 = ctx.r27.u64;
loc_82A85AA8:
	// add r10,r10,r9
	ctx.r10.u64 = ctx.r10.u64 + ctx.r9.u64;
	// cmplw cr6,r30,r5
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, ctx.r5.u32, ctx.xer);
	// addi r30,r30,1
	ctx.r30.s64 = ctx.r30.s64 + 1;
	// stbx r10,r8,r3
	REX_STORE_U8(ctx.r8.u32 + ctx.r3.u32, ctx.r10.u8);
	// beq cr6,0x82a85eb4
	if (ctx.cr6.eq) goto loc_82A85EB4;
loc_82A85ABC:
	// addi r6,r6,1
	ctx.r6.s64 = ctx.r6.s64 + 1;
	// cmpw cr6,r6,r29
	ctx.cr6.compare<int32_t>(ctx.r6.s32, ctx.r29.s32, ctx.xer);
	// blt cr6,0x82a85a54
	if (ctx.cr6.lt) goto loc_82A85A54;
loc_82A85AC8:
	// mr r6,r28
	ctx.r6.u64 = ctx.r28.u64;
	// cmplw cr6,r28,r26
	ctx.cr6.compare<uint32_t>(ctx.r28.u32, ctx.r26.u32, ctx.xer);
	// bge cr6,0x82a85ea4
	if (!ctx.cr6.lt) goto loc_82A85EA4;
	// addi r10,r1,-160
	ctx.r10.s64 = ctx.r1.s64 + -160;
	// add r8,r29,r10
	ctx.r8.u64 = ctx.r29.u64 + ctx.r10.u64;
loc_82A85ADC:
	// lbz r10,0(r6)
	ctx.r10.u64 = REX_LOAD_U8(ctx.r6.u32 + 0);
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x82a85e98
	if (ctx.cr6.eq) goto loc_82A85E98;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82a85b04
	if (!ctx.cr6.eq) goto loc_82A85B04;
	// lwz r9,0(r31)
	ctx.r9.u64 = REX_LOAD_U32(ctx.r31.u32 + 0);
	// li r11,31
	ctx.r11.s64 = 31;
	// addi r31,r31,4
	ctx.r31.s64 = ctx.r31.s64 + 4;
	// rlwinm r7,r9,31,1,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 31) & 0x7FFFFFFF;
	// b 0x82a85b10
	goto loc_82A85B10;
loc_82A85B04:
	// mr r9,r7
	ctx.r9.u64 = ctx.r7.u64;
	// rlwinm r7,r7,31,1,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 31) & 0x7FFFFFFF;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
loc_82A85B10:
	// clrlwi r9,r9,31
	ctx.r9.u64 = ctx.r9.u32 & 0x1;
	// cmplwi cr6,r9,0
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 0, ctx.xer);
	// beq cr6,0x82a85e98
	if (ctx.cr6.eq) goto loc_82A85E98;
	// clrlwi r9,r10,30
	ctx.r9.u64 = ctx.r10.u32 & 0x3;
	// cmplwi cr6,r9,3
	ctx.cr6.compare<uint32_t>(ctx.r9.u32, 3, ctx.xer);
	// bgt cr6,0x82a85e98
	if (ctx.cr6.gt) goto loc_82A85E98;
	// lis r12,-32088
	ctx.r12.s64 = -2102919168;
	// addi r12,r12,23360
	ctx.r12.s64 = ctx.r12.s64 + 23360;
	// rlwinm r0,r9,2,0,29
	ctx.r0.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// lwzx r0,r12,r0
	ctx.r0.u64 = REX_LOAD_U32(ctx.r12.u32 + ctx.r0.u32);
	// mtctr r0
	ctx.ctr.u64 = ctx.r0.u64;
	// bctr 
	switch (ctx.r9.u32) {
	case 0:
		goto loc_82A85B50;
	case 1:
		goto loc_82A85B64;
	case 2:
		goto loc_82A85B9C;
	case 3:
		goto loc_82A85E38;
	default:
		__builtin_trap(); // Switch case out of range
	}
loc_82A85B50:
	// rlwinm r9,r10,30,2,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 30) & 0x3FFFFFFF;
	// rlwinm r10,r9,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r10,r10,17
	ctx.r10.s64 = ctx.r10.s64 + 17;
	// stb r10,0(r6)
	REX_STORE_U8(ctx.r6.u32 + 0, ctx.r10.u8);
	// b 0x82a85ba8
	goto loc_82A85BA8;
loc_82A85B64:
	// rlwinm r10,r10,0,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 0) & 0xFFFFFFFC;
	// addi r9,r10,2
	ctx.r9.s64 = ctx.r10.s64 + 2;
	// addi r23,r10,18
	ctx.r23.s64 = ctx.r10.s64 + 18;
	// addi r22,r10,34
	ctx.r22.s64 = ctx.r10.s64 + 34;
	// addi r10,r10,50
	ctx.r10.s64 = ctx.r10.s64 + 50;
	// stb r9,0(r6)
	REX_STORE_U8(ctx.r6.u32 + 0, ctx.r9.u8);
	// mr r9,r10
	ctx.r9.u64 = ctx.r10.u64;
	// addi r10,r26,1
	ctx.r10.s64 = ctx.r26.s64 + 1;
	// stb r23,0(r26)
	REX_STORE_U8(ctx.r26.u32 + 0, ctx.r23.u8);
	// stb r22,0(r10)
	REX_STORE_U8(ctx.r10.u32 + 0, ctx.r22.u8);
	// addi r10,r10,1
	ctx.r10.s64 = ctx.r10.s64 + 1;
	// addi r26,r10,1
	ctx.r26.s64 = ctx.r10.s64 + 1;
	// stb r9,0(r10)
	REX_STORE_U8(ctx.r10.u32 + 0, ctx.r9.u8);
	// b 0x82a85e9c
	goto loc_82A85E9C;
loc_82A85B9C:
	// stb r25,0(r6)
	REX_STORE_U8(ctx.r6.u32 + 0, ctx.r25.u8);
	// rlwinm r9,r10,30,2,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 30) & 0x3FFFFFFF;
	// addi r6,r6,1
	ctx.r6.s64 = ctx.r6.s64 + 1;
loc_82A85BA8:
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82a85bc4
	if (!ctx.cr6.eq) goto loc_82A85BC4;
	// lwz r10,0(r31)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r31.u32 + 0);
	// li r11,31
	ctx.r11.s64 = 31;
	// addi r31,r31,4
	ctx.r31.s64 = ctx.r31.s64 + 4;
	// rlwinm r7,r10,31,1,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// b 0x82a85bd0
	goto loc_82A85BD0;
loc_82A85BC4:
	// mr r10,r7
	ctx.r10.u64 = ctx.r7.u64;
	// rlwinm r7,r7,31,1,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 31) & 0x7FFFFFFF;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
loc_82A85BD0:
	// clrlwi r10,r10,31
	ctx.r10.u64 = ctx.r10.u32 & 0x1;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x82a85bf0
	if (ctx.cr6.eq) goto loc_82A85BF0;
	// rlwinm r10,r9,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r28,r28,-1
	ctx.r28.s64 = ctx.r28.s64 + -1;
	// addi r10,r10,3
	ctx.r10.s64 = ctx.r10.s64 + 3;
	// stb r10,0(r28)
	REX_STORE_U8(ctx.r28.u32 + 0, ctx.r10.u8);
	// b 0x82a85c48
	goto loc_82A85C48;
loc_82A85BF0:
	// stb r9,0(r8)
	REX_STORE_U8(ctx.r8.u32 + 0, ctx.r9.u8);
	// addi r29,r29,1
	ctx.r29.s64 = ctx.r29.s64 + 1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// addi r8,r8,1
	ctx.r8.s64 = ctx.r8.s64 + 1;
	// bne cr6,0x82a85c18
	if (!ctx.cr6.eq) goto loc_82A85C18;
	// lwz r10,0(r31)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r31.u32 + 0);
	// li r11,31
	ctx.r11.s64 = 31;
	// addi r31,r31,4
	ctx.r31.s64 = ctx.r31.s64 + 4;
	// rlwinm r7,r10,31,1,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// b 0x82a85c24
	goto loc_82A85C24;
loc_82A85C18:
	// mr r10,r7
	ctx.r10.u64 = ctx.r7.u64;
	// rlwinm r7,r7,31,1,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 31) & 0x7FFFFFFF;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
loc_82A85C24:
	// clrlwi r10,r10,31
	ctx.r10.u64 = ctx.r10.u32 & 0x1;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// neg r10,r27
	ctx.r10.s64 = static_cast<int64_t>(-ctx.r27.u64);
	// bne cr6,0x82a85c38
	if (!ctx.cr6.eq) goto loc_82A85C38;
	// mr r10,r27
	ctx.r10.u64 = ctx.r27.u64;
loc_82A85C38:
	// cmplw cr6,r30,r5
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, ctx.r5.u32, ctx.xer);
	// stbx r10,r9,r3
	REX_STORE_U8(ctx.r9.u32 + ctx.r3.u32, ctx.r10.u8);
	// addi r30,r30,1
	ctx.r30.s64 = ctx.r30.s64 + 1;
	// beq cr6,0x82a85eb4
	if (ctx.cr6.eq) goto loc_82A85EB4;
loc_82A85C48:
	// addi r9,r9,1
	ctx.r9.s64 = ctx.r9.s64 + 1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82a85c68
	if (!ctx.cr6.eq) goto loc_82A85C68;
	// lwz r10,0(r31)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r31.u32 + 0);
	// li r11,31
	ctx.r11.s64 = 31;
	// addi r31,r31,4
	ctx.r31.s64 = ctx.r31.s64 + 4;
	// rlwinm r7,r10,31,1,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// b 0x82a85c74
	goto loc_82A85C74;
loc_82A85C68:
	// mr r10,r7
	ctx.r10.u64 = ctx.r7.u64;
	// rlwinm r7,r7,31,1,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 31) & 0x7FFFFFFF;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
loc_82A85C74:
	// clrlwi r10,r10,31
	ctx.r10.u64 = ctx.r10.u32 & 0x1;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x82a85c94
	if (ctx.cr6.eq) goto loc_82A85C94;
	// rlwinm r10,r9,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r28,r28,-1
	ctx.r28.s64 = ctx.r28.s64 + -1;
	// addi r10,r10,3
	ctx.r10.s64 = ctx.r10.s64 + 3;
	// stb r10,0(r28)
	REX_STORE_U8(ctx.r28.u32 + 0, ctx.r10.u8);
	// b 0x82a85cec
	goto loc_82A85CEC;
loc_82A85C94:
	// stb r9,0(r8)
	REX_STORE_U8(ctx.r8.u32 + 0, ctx.r9.u8);
	// addi r29,r29,1
	ctx.r29.s64 = ctx.r29.s64 + 1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// addi r8,r8,1
	ctx.r8.s64 = ctx.r8.s64 + 1;
	// bne cr6,0x82a85cbc
	if (!ctx.cr6.eq) goto loc_82A85CBC;
	// lwz r10,0(r31)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r31.u32 + 0);
	// li r11,31
	ctx.r11.s64 = 31;
	// addi r31,r31,4
	ctx.r31.s64 = ctx.r31.s64 + 4;
	// rlwinm r7,r10,31,1,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// b 0x82a85cc8
	goto loc_82A85CC8;
loc_82A85CBC:
	// mr r10,r7
	ctx.r10.u64 = ctx.r7.u64;
	// rlwinm r7,r7,31,1,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 31) & 0x7FFFFFFF;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
loc_82A85CC8:
	// clrlwi r10,r10,31
	ctx.r10.u64 = ctx.r10.u32 & 0x1;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// neg r10,r27
	ctx.r10.s64 = static_cast<int64_t>(-ctx.r27.u64);
	// bne cr6,0x82a85cdc
	if (!ctx.cr6.eq) goto loc_82A85CDC;
	// mr r10,r27
	ctx.r10.u64 = ctx.r27.u64;
loc_82A85CDC:
	// cmplw cr6,r30,r5
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, ctx.r5.u32, ctx.xer);
	// stbx r10,r9,r3
	REX_STORE_U8(ctx.r9.u32 + ctx.r3.u32, ctx.r10.u8);
	// addi r30,r30,1
	ctx.r30.s64 = ctx.r30.s64 + 1;
	// beq cr6,0x82a85eb4
	if (ctx.cr6.eq) goto loc_82A85EB4;
loc_82A85CEC:
	// addi r9,r9,1
	ctx.r9.s64 = ctx.r9.s64 + 1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82a85d0c
	if (!ctx.cr6.eq) goto loc_82A85D0C;
	// lwz r10,0(r31)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r31.u32 + 0);
	// li r11,31
	ctx.r11.s64 = 31;
	// addi r31,r31,4
	ctx.r31.s64 = ctx.r31.s64 + 4;
	// rlwinm r7,r10,31,1,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// b 0x82a85d18
	goto loc_82A85D18;
loc_82A85D0C:
	// mr r10,r7
	ctx.r10.u64 = ctx.r7.u64;
	// rlwinm r7,r7,31,1,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 31) & 0x7FFFFFFF;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
loc_82A85D18:
	// clrlwi r10,r10,31
	ctx.r10.u64 = ctx.r10.u32 & 0x1;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x82a85d38
	if (ctx.cr6.eq) goto loc_82A85D38;
	// rlwinm r10,r9,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r28,r28,-1
	ctx.r28.s64 = ctx.r28.s64 + -1;
	// addi r10,r10,3
	ctx.r10.s64 = ctx.r10.s64 + 3;
	// stb r10,0(r28)
	REX_STORE_U8(ctx.r28.u32 + 0, ctx.r10.u8);
	// b 0x82a85d90
	goto loc_82A85D90;
loc_82A85D38:
	// stb r9,0(r8)
	REX_STORE_U8(ctx.r8.u32 + 0, ctx.r9.u8);
	// addi r29,r29,1
	ctx.r29.s64 = ctx.r29.s64 + 1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// addi r8,r8,1
	ctx.r8.s64 = ctx.r8.s64 + 1;
	// bne cr6,0x82a85d60
	if (!ctx.cr6.eq) goto loc_82A85D60;
	// lwz r10,0(r31)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r31.u32 + 0);
	// li r11,31
	ctx.r11.s64 = 31;
	// addi r31,r31,4
	ctx.r31.s64 = ctx.r31.s64 + 4;
	// rlwinm r7,r10,31,1,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// b 0x82a85d6c
	goto loc_82A85D6C;
loc_82A85D60:
	// mr r10,r7
	ctx.r10.u64 = ctx.r7.u64;
	// rlwinm r7,r7,31,1,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 31) & 0x7FFFFFFF;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
loc_82A85D6C:
	// clrlwi r10,r10,31
	ctx.r10.u64 = ctx.r10.u32 & 0x1;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// neg r10,r27
	ctx.r10.s64 = static_cast<int64_t>(-ctx.r27.u64);
	// bne cr6,0x82a85d80
	if (!ctx.cr6.eq) goto loc_82A85D80;
	// mr r10,r27
	ctx.r10.u64 = ctx.r27.u64;
loc_82A85D80:
	// cmplw cr6,r30,r5
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, ctx.r5.u32, ctx.xer);
	// stbx r10,r9,r3
	REX_STORE_U8(ctx.r9.u32 + ctx.r3.u32, ctx.r10.u8);
	// addi r30,r30,1
	ctx.r30.s64 = ctx.r30.s64 + 1;
	// beq cr6,0x82a85eb4
	if (ctx.cr6.eq) goto loc_82A85EB4;
loc_82A85D90:
	// addi r9,r9,1
	ctx.r9.s64 = ctx.r9.s64 + 1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// bne cr6,0x82a85db0
	if (!ctx.cr6.eq) goto loc_82A85DB0;
	// lwz r10,0(r31)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r31.u32 + 0);
	// li r11,31
	ctx.r11.s64 = 31;
	// addi r31,r31,4
	ctx.r31.s64 = ctx.r31.s64 + 4;
	// rlwinm r7,r10,31,1,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// b 0x82a85dbc
	goto loc_82A85DBC;
loc_82A85DB0:
	// mr r10,r7
	ctx.r10.u64 = ctx.r7.u64;
	// rlwinm r7,r7,31,1,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 31) & 0x7FFFFFFF;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
loc_82A85DBC:
	// clrlwi r10,r10,31
	ctx.r10.u64 = ctx.r10.u32 & 0x1;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// beq cr6,0x82a85ddc
	if (ctx.cr6.eq) goto loc_82A85DDC;
	// rlwinm r10,r9,2,0,29
	ctx.r10.u64 = __builtin_rotateleft64(ctx.r9.u32 | (ctx.r9.u64 << 32), 2) & 0xFFFFFFFC;
	// addi r28,r28,-1
	ctx.r28.s64 = ctx.r28.s64 + -1;
	// addi r10,r10,3
	ctx.r10.s64 = ctx.r10.s64 + 3;
	// stb r10,0(r28)
	REX_STORE_U8(ctx.r28.u32 + 0, ctx.r10.u8);
	// b 0x82a85e9c
	goto loc_82A85E9C;
loc_82A85DDC:
	// stb r9,0(r8)
	REX_STORE_U8(ctx.r8.u32 + 0, ctx.r9.u8);
	// addi r29,r29,1
	ctx.r29.s64 = ctx.r29.s64 + 1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// addi r8,r8,1
	ctx.r8.s64 = ctx.r8.s64 + 1;
	// bne cr6,0x82a85e04
	if (!ctx.cr6.eq) goto loc_82A85E04;
	// lwz r10,0(r31)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r31.u32 + 0);
	// li r11,31
	ctx.r11.s64 = 31;
	// addi r31,r31,4
	ctx.r31.s64 = ctx.r31.s64 + 4;
	// rlwinm r7,r10,31,1,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// b 0x82a85e10
	goto loc_82A85E10;
loc_82A85E04:
	// mr r10,r7
	ctx.r10.u64 = ctx.r7.u64;
	// rlwinm r7,r7,31,1,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 31) & 0x7FFFFFFF;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
loc_82A85E10:
	// clrlwi r10,r10,31
	ctx.r10.u64 = ctx.r10.u32 & 0x1;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// neg r10,r27
	ctx.r10.s64 = static_cast<int64_t>(-ctx.r27.u64);
	// bne cr6,0x82a85e24
	if (!ctx.cr6.eq) goto loc_82A85E24;
	// mr r10,r27
	ctx.r10.u64 = ctx.r27.u64;
loc_82A85E24:
	// cmplw cr6,r30,r5
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, ctx.r5.u32, ctx.xer);
	// stbx r10,r9,r3
	REX_STORE_U8(ctx.r9.u32 + ctx.r3.u32, ctx.r10.u8);
	// addi r30,r30,1
	ctx.r30.s64 = ctx.r30.s64 + 1;
	// beq cr6,0x82a85eb4
	if (ctx.cr6.eq) goto loc_82A85EB4;
	// b 0x82a85e9c
	goto loc_82A85E9C;
loc_82A85E38:
	// rlwinm r9,r10,30,2,31
	ctx.r9.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 30) & 0x3FFFFFFF;
	// addi r29,r29,1
	ctx.r29.s64 = ctx.r29.s64 + 1;
	// cmplwi cr6,r11,0
	ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
	// stb r9,0(r8)
	REX_STORE_U8(ctx.r8.u32 + 0, ctx.r9.u8);
	// addi r8,r8,1
	ctx.r8.s64 = ctx.r8.s64 + 1;
	// bne cr6,0x82a85e64
	if (!ctx.cr6.eq) goto loc_82A85E64;
	// lwz r10,0(r31)
	ctx.r10.u64 = REX_LOAD_U32(ctx.r31.u32 + 0);
	// li r11,31
	ctx.r11.s64 = 31;
	// addi r31,r31,4
	ctx.r31.s64 = ctx.r31.s64 + 4;
	// rlwinm r7,r10,31,1,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r10.u32 | (ctx.r10.u64 << 32), 31) & 0x7FFFFFFF;
	// b 0x82a85e70
	goto loc_82A85E70;
loc_82A85E64:
	// mr r10,r7
	ctx.r10.u64 = ctx.r7.u64;
	// rlwinm r7,r7,31,1,31
	ctx.r7.u64 = __builtin_rotateleft64(ctx.r7.u32 | (ctx.r7.u64 << 32), 31) & 0x7FFFFFFF;
	// addi r11,r11,-1
	ctx.r11.s64 = ctx.r11.s64 + -1;
loc_82A85E70:
	// clrlwi r10,r10,31
	ctx.r10.u64 = ctx.r10.u32 & 0x1;
	// cmplwi cr6,r10,0
	ctx.cr6.compare<uint32_t>(ctx.r10.u32, 0, ctx.xer);
	// neg r10,r27
	ctx.r10.s64 = static_cast<int64_t>(-ctx.r27.u64);
	// bne cr6,0x82a85e84
	if (!ctx.cr6.eq) goto loc_82A85E84;
	// mr r10,r27
	ctx.r10.u64 = ctx.r27.u64;
loc_82A85E84:
	// cmplw cr6,r30,r5
	ctx.cr6.compare<uint32_t>(ctx.r30.u32, ctx.r5.u32, ctx.xer);
	// stbx r10,r9,r3
	REX_STORE_U8(ctx.r9.u32 + ctx.r3.u32, ctx.r10.u8);
	// addi r30,r30,1
	ctx.r30.s64 = ctx.r30.s64 + 1;
	// beq cr6,0x82a85eb4
	if (ctx.cr6.eq) goto loc_82A85EB4;
	// stb r25,0(r6)
	REX_STORE_U8(ctx.r6.u32 + 0, ctx.r25.u8);
loc_82A85E98:
	// addi r6,r6,1
	ctx.r6.s64 = ctx.r6.s64 + 1;
loc_82A85E9C:
	// cmplw cr6,r6,r26
	ctx.cr6.compare<uint32_t>(ctx.r6.u32, ctx.r26.u32, ctx.xer);
	// blt cr6,0x82a85adc
	if (ctx.cr6.lt) goto loc_82A85ADC;
loc_82A85EA4:
	// addi r24,r24,-1
	ctx.r24.s64 = ctx.r24.s64 + -1;
	// srawi r27,r27,1
	ctx.xer.ca = (ctx.r27.s32 < 0) & ((ctx.r27.u32 & 0x1) != 0);
	ctx.r27.s64 = ctx.r27.s32 >> 1;
	// cmplwi cr6,r24,0
	ctx.cr6.compare<uint32_t>(ctx.r24.u32, 0, ctx.xer);
	// bne cr6,0x82a85a48
	if (!ctx.cr6.eq) goto loc_82A85A48;
loc_82A85EB4:
	// stw r31,4(r4)
	REX_STORE_U32(ctx.r4.u32 + 4, ctx.r31.u32);
	// stw r7,0(r4)
	REX_STORE_U32(ctx.r4.u32 + 0, ctx.r7.u32);
	// stw r11,8(r4)
	REX_STORE_U32(ctx.r4.u32 + 8, ctx.r11.u32);
	// b 0x829ff800
	__restgprlr_22(ctx, base);
	return;
}

DEFINE_REX_FUNC(sub_82A85EC8) {
	REX_FUNC_PROLOGUE();
	uint32_t ea{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// bl 0x829ff79c
	ctx.lr = 0x82A85ED0;
	__savegprlr_17(ctx, base);
	// stwu r1,-272(r1)
	ea = -272 + ctx.r1.u32;
	REX_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// vspltisw v0,0
	simde_mm_store_si128((simde__m128i*)ctx.v0.u32, simde_mm_set1_epi32(int(0x0)));
	// addi r11,r1,80
	ctx.r11.s64 = ctx.r1.s64 + 80;
	// mr r29,r4
	ctx.r29.u64 = ctx.r4.u64;
	// mr r4,r5
	ctx.r4.u64 = ctx.r5.u64;
	// mr r31,r3
	ctx.r31.u64 = ctx.r3.u64;
	// mr r5,r6
	ctx.r5.u64 = ctx.r6.u64;
	// addi r3,r1,80
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// mr r30,r7
	ctx.r30.u64 = ctx.r7.u64;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,96
	ctx.r11.s64 = ctx.r1.s64 + 96;
	// mr r28,r8
	ctx.r28.u64 = ctx.r8.u64;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,112
	ctx.r11.s64 = ctx.r1.s64 + 112;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// addi r11,r1,128
	ctx.r11.s64 = ctx.r1.s64 + 128;
	// stvx128 v0,r0,r11
	ea = (ctx.r11.u32) & ~0xF;
	simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
	// bl 0x82a859a8
	ctx.lr = 0x82A85F18;
	sub_82A859A8(ctx, base);
	// lbz r4,6(r30)
	ctx.r4.u64 = REX_LOAD_U8(ctx.r30.u32 + 6);
	// add r11,r30,r28
	ctx.r11.u64 = ctx.r30.u64 + ctx.r28.u64;
	// lbz r3,92(r1)
	ctx.r3.u64 = REX_LOAD_U8(ctx.r1.u32 + 92);
	// add r10,r31,r29
	ctx.r10.u64 = ctx.r31.u64 + ctx.r29.u64;
	// lbz r8,5(r30)
	ctx.r8.u64 = REX_LOAD_U8(ctx.r30.u32 + 5);
	// lbz r18,0(r30)
	ctx.r18.u64 = REX_LOAD_U8(ctx.r30.u32 + 0);
	// add r4,r4,r3
	ctx.r4.u64 = ctx.r4.u64 + ctx.r3.u64;
	// lbz r20,1(r30)
	ctx.r20.u64 = REX_LOAD_U8(ctx.r30.u32 + 1);
	// lbz r22,2(r30)
	ctx.r22.u64 = REX_LOAD_U8(ctx.r30.u32 + 2);
	// lbz r24,3(r30)
	ctx.r24.u64 = REX_LOAD_U8(ctx.r30.u32 + 3);
	// lbz r26,4(r30)
	ctx.r26.u64 = REX_LOAD_U8(ctx.r30.u32 + 4);
	// lbz r9,7(r30)
	ctx.r9.u64 = REX_LOAD_U8(ctx.r30.u32 + 7);
	// mr r30,r8
	ctx.r30.u64 = ctx.r8.u64;
	// lbz r7,93(r1)
	ctx.r7.u64 = REX_LOAD_U8(ctx.r1.u32 + 93);
	// lbz r27,89(r1)
	ctx.r27.u64 = REX_LOAD_U8(ctx.r1.u32 + 89);
	// add r9,r9,r7
	ctx.r9.u64 = ctx.r9.u64 + ctx.r7.u64;
	// stb r4,6(r31)
	REX_STORE_U8(ctx.r31.u32 + 6, ctx.r4.u8);
	// add r30,r30,r27
	ctx.r30.u64 = ctx.r30.u64 + ctx.r27.u64;
	// lbz r17,80(r1)
	ctx.r17.u64 = REX_LOAD_U8(ctx.r1.u32 + 80);
	// lbz r19,81(r1)
	ctx.r19.u64 = REX_LOAD_U8(ctx.r1.u32 + 81);
	// lbz r21,84(r1)
	ctx.r21.u64 = REX_LOAD_U8(ctx.r1.u32 + 84);
	// add r18,r18,r17
	ctx.r18.u64 = ctx.r18.u64 + ctx.r17.u64;
	// lbz r23,85(r1)
	ctx.r23.u64 = REX_LOAD_U8(ctx.r1.u32 + 85);
	// add r20,r20,r19
	ctx.r20.u64 = ctx.r20.u64 + ctx.r19.u64;
	// lbz r25,88(r1)
	ctx.r25.u64 = REX_LOAD_U8(ctx.r1.u32 + 88);
	// add r22,r22,r21
	ctx.r22.u64 = ctx.r22.u64 + ctx.r21.u64;
	// lbz r4,0(r11)
	ctx.r4.u64 = REX_LOAD_U8(ctx.r11.u32 + 0);
	// add r24,r24,r23
	ctx.r24.u64 = ctx.r24.u64 + ctx.r23.u64;
	// lbz r3,2(r11)
	ctx.r3.u64 = REX_LOAD_U8(ctx.r11.u32 + 2);
	// add r26,r26,r25
	ctx.r26.u64 = ctx.r26.u64 + ctx.r25.u64;
	// lbz r6,83(r1)
	ctx.r6.u64 = REX_LOAD_U8(ctx.r1.u32 + 83);
	// lbz r8,86(r1)
	ctx.r8.u64 = REX_LOAD_U8(ctx.r1.u32 + 86);
	// lbz r7,1(r11)
	ctx.r7.u64 = REX_LOAD_U8(ctx.r11.u32 + 1);
	// lbz r5,82(r1)
	ctx.r5.u64 = REX_LOAD_U8(ctx.r1.u32 + 82);
	// stb r9,7(r31)
	REX_STORE_U8(ctx.r31.u32 + 7, ctx.r9.u8);
	// add r9,r3,r8
	ctx.r9.u64 = ctx.r3.u64 + ctx.r8.u64;
	// stb r30,5(r31)
	REX_STORE_U8(ctx.r31.u32 + 5, ctx.r30.u8);
	// add r5,r5,r4
	ctx.r5.u64 = ctx.r5.u64 + ctx.r4.u64;
	// add r7,r7,r6
	ctx.r7.u64 = ctx.r7.u64 + ctx.r6.u64;
	// lbz r30,3(r11)
	ctx.r30.u64 = REX_LOAD_U8(ctx.r11.u32 + 3);
	// lbz r27,87(r1)
	ctx.r27.u64 = REX_LOAD_U8(ctx.r1.u32 + 87);
	// stb r18,0(r31)
	REX_STORE_U8(ctx.r31.u32 + 0, ctx.r18.u8);
	// add r30,r30,r27
	ctx.r30.u64 = ctx.r30.u64 + ctx.r27.u64;
	// stb r20,1(r31)
	REX_STORE_U8(ctx.r31.u32 + 1, ctx.r20.u8);
	// stb r22,2(r31)
	REX_STORE_U8(ctx.r31.u32 + 2, ctx.r22.u8);
	// stb r24,3(r31)
	REX_STORE_U8(ctx.r31.u32 + 3, ctx.r24.u8);
	// stb r26,4(r31)
	REX_STORE_U8(ctx.r31.u32 + 4, ctx.r26.u8);
	// stb r5,0(r10)
	REX_STORE_U8(ctx.r10.u32 + 0, ctx.r5.u8);
	// stb r7,1(r10)
	REX_STORE_U8(ctx.r10.u32 + 1, ctx.r7.u8);
	// stb r9,2(r10)
	REX_STORE_U8(ctx.r10.u32 + 2, ctx.r9.u8);
	// lbz r3,4(r11)
	ctx.r3.u64 = REX_LOAD_U8(ctx.r11.u32 + 4);
	// lbz r5,5(r11)
	ctx.r5.u64 = REX_LOAD_U8(ctx.r11.u32 + 5);
	// lbz r7,6(r11)
	ctx.r7.u64 = REX_LOAD_U8(ctx.r11.u32 + 6);
	// lbz r9,7(r11)
	ctx.r9.u64 = REX_LOAD_U8(ctx.r11.u32 + 7);
	// add r11,r11,r28
	ctx.r11.u64 = ctx.r11.u64 + ctx.r28.u64;
	// lbz r31,90(r1)
	ctx.r31.u64 = REX_LOAD_U8(ctx.r1.u32 + 90);
	// lbz r4,91(r1)
	ctx.r4.u64 = REX_LOAD_U8(ctx.r1.u32 + 91);
	// lbz r6,94(r1)
	ctx.r6.u64 = REX_LOAD_U8(ctx.r1.u32 + 94);
	// add r3,r3,r31
	ctx.r3.u64 = ctx.r3.u64 + ctx.r31.u64;
	// lbz r8,95(r1)
	ctx.r8.u64 = REX_LOAD_U8(ctx.r1.u32 + 95);
	// add r5,r5,r4
	ctx.r5.u64 = ctx.r5.u64 + ctx.r4.u64;
	// add r7,r7,r6
	ctx.r7.u64 = ctx.r7.u64 + ctx.r6.u64;
	// stb r30,3(r10)
	REX_STORE_U8(ctx.r10.u32 + 3, ctx.r30.u8);
	// add r9,r9,r8
	ctx.r9.u64 = ctx.r9.u64 + ctx.r8.u64;
	// stb r7,6(r10)
	REX_STORE_U8(ctx.r10.u32 + 6, ctx.r7.u8);
	// stb r9,7(r10)
	REX_STORE_U8(ctx.r10.u32 + 7, ctx.r9.u8);
	// lbz r7,4(r11)
	ctx.r7.u64 = REX_LOAD_U8(ctx.r11.u32 + 4);
	// lbz r31,96(r1)
	ctx.r31.u64 = REX_LOAD_U8(ctx.r1.u32 + 96);
	// lbz r9,6(r11)
	ctx.r9.u64 = REX_LOAD_U8(ctx.r11.u32 + 6);
	// lbz r6,100(r1)
	ctx.r6.u64 = REX_LOAD_U8(ctx.r1.u32 + 100);
	// stb r3,4(r10)
	REX_STORE_U8(ctx.r10.u32 + 4, ctx.r3.u8);
	// add r3,r7,r31
	ctx.r3.u64 = ctx.r7.u64 + ctx.r31.u64;
	// stb r5,5(r10)
	REX_STORE_U8(ctx.r10.u32 + 5, ctx.r5.u8);
	// add r7,r9,r6
	ctx.r7.u64 = ctx.r9.u64 + ctx.r6.u64;
	// lbz r19,7(r11)
	ctx.r19.u64 = REX_LOAD_U8(ctx.r11.u32 + 7);
	// add r10,r10,r29
	ctx.r10.u64 = ctx.r10.u64 + ctx.r29.u64;
	// lbz r8,101(r1)
	ctx.r8.u64 = REX_LOAD_U8(ctx.r1.u32 + 101);
	// lbz r20,0(r11)
	ctx.r20.u64 = REX_LOAD_U8(ctx.r11.u32 + 0);
	// lbz r24,1(r11)
	ctx.r24.u64 = REX_LOAD_U8(ctx.r11.u32 + 1);
	// add r9,r19,r8
	ctx.r9.u64 = ctx.r19.u64 + ctx.r8.u64;
	// lbz r30,3(r11)
	ctx.r30.u64 = REX_LOAD_U8(ctx.r11.u32 + 3);
	// lbz r5,5(r11)
	ctx.r5.u64 = REX_LOAD_U8(ctx.r11.u32 + 5);
	// lbz r23,105(r1)
	ctx.r23.u64 = REX_LOAD_U8(ctx.r1.u32 + 105);
	// lbz r27,125(r1)
	ctx.r27.u64 = REX_LOAD_U8(ctx.r1.u32 + 125);
	// lbz r4,97(r1)
	ctx.r4.u64 = REX_LOAD_U8(ctx.r1.u32 + 97);
	// add r24,r24,r23
	ctx.r24.u64 = ctx.r24.u64 + ctx.r23.u64;
	// lbz r21,104(r1)
	ctx.r21.u64 = REX_LOAD_U8(ctx.r1.u32 + 104);
	// add r30,r30,r27
	ctx.r30.u64 = ctx.r30.u64 + ctx.r27.u64;
	// lbz r26,2(r11)
	ctx.r26.u64 = REX_LOAD_U8(ctx.r11.u32 + 2);
	// add r11,r11,r28
	ctx.r11.u64 = ctx.r11.u64 + ctx.r28.u64;
	// lbz r25,124(r1)
	ctx.r25.u64 = REX_LOAD_U8(ctx.r1.u32 + 124);
	// add r5,r5,r4
	ctx.r5.u64 = ctx.r5.u64 + ctx.r4.u64;
	// add r21,r21,r20
	ctx.r21.u64 = ctx.r21.u64 + ctx.r20.u64;
	// stb r9,7(r10)
	REX_STORE_U8(ctx.r10.u32 + 7, ctx.r9.u8);
	// add r26,r26,r25
	ctx.r26.u64 = ctx.r26.u64 + ctx.r25.u64;
	// stb r3,4(r10)
	REX_STORE_U8(ctx.r10.u32 + 4, ctx.r3.u8);
	// stb r30,3(r10)
	REX_STORE_U8(ctx.r10.u32 + 3, ctx.r30.u8);
	// stb r7,6(r10)
	REX_STORE_U8(ctx.r10.u32 + 6, ctx.r7.u8);
	// stb r24,1(r10)
	REX_STORE_U8(ctx.r10.u32 + 1, ctx.r24.u8);
	// stb r5,5(r10)
	REX_STORE_U8(ctx.r10.u32 + 5, ctx.r5.u8);
	// stb r21,0(r10)
	REX_STORE_U8(ctx.r10.u32 + 0, ctx.r21.u8);
	// lbz r9,2(r11)
	ctx.r9.u64 = REX_LOAD_U8(ctx.r11.u32 + 2);
	// lbz r25,126(r1)
	ctx.r25.u64 = REX_LOAD_U8(ctx.r1.u32 + 126);
	// lbz r21,0(r11)
	ctx.r21.u64 = REX_LOAD_U8(ctx.r11.u32 + 0);
	// lbz r24,1(r11)
	ctx.r24.u64 = REX_LOAD_U8(ctx.r11.u32 + 1);
	// lbz r30,3(r11)
	ctx.r30.u64 = REX_LOAD_U8(ctx.r11.u32 + 3);
	// lbz r3,4(r11)
	ctx.r3.u64 = REX_LOAD_U8(ctx.r11.u32 + 4);
	// lbz r5,5(r11)
	ctx.r5.u64 = REX_LOAD_U8(ctx.r11.u32 + 5);
	// lbz r7,6(r11)
	ctx.r7.u64 = REX_LOAD_U8(ctx.r11.u32 + 6);
	// lbz r20,7(r11)
	ctx.r20.u64 = REX_LOAD_U8(ctx.r11.u32 + 7);
	// add r11,r11,r28
	ctx.r11.u64 = ctx.r11.u64 + ctx.r28.u64;
	// lbz r27,127(r1)
	ctx.r27.u64 = REX_LOAD_U8(ctx.r1.u32 + 127);
	// lbz r4,99(r1)
	ctx.r4.u64 = REX_LOAD_U8(ctx.r1.u32 + 99);
	// lbz r6,102(r1)
	ctx.r6.u64 = REX_LOAD_U8(ctx.r1.u32 + 102);
	// add r30,r30,r27
	ctx.r30.u64 = ctx.r30.u64 + ctx.r27.u64;
	// lbz r8,103(r1)
	ctx.r8.u64 = REX_LOAD_U8(ctx.r1.u32 + 103);
	// add r5,r5,r4
	ctx.r5.u64 = ctx.r5.u64 + ctx.r4.u64;
	// lbz r22,106(r1)
	ctx.r22.u64 = REX_LOAD_U8(ctx.r1.u32 + 106);
	// add r7,r7,r6
	ctx.r7.u64 = ctx.r7.u64 + ctx.r6.u64;
	// lbz r23,107(r1)
	ctx.r23.u64 = REX_LOAD_U8(ctx.r1.u32 + 107);
	// lbz r31,98(r1)
	ctx.r31.u64 = REX_LOAD_U8(ctx.r1.u32 + 98);
	// add r22,r22,r21
	ctx.r22.u64 = ctx.r22.u64 + ctx.r21.u64;
	// stb r26,2(r10)
	REX_STORE_U8(ctx.r10.u32 + 2, ctx.r26.u8);
	// add r26,r9,r25
	ctx.r26.u64 = ctx.r9.u64 + ctx.r25.u64;
	// add r10,r10,r29
	ctx.r10.u64 = ctx.r10.u64 + ctx.r29.u64;
	// lbz r27,108(r1)
	ctx.r27.u64 = REX_LOAD_U8(ctx.r1.u32 + 108);
	// add r3,r3,r31
	ctx.r3.u64 = ctx.r3.u64 + ctx.r31.u64;
	// lbz r6,113(r1)
	ctx.r6.u64 = REX_LOAD_U8(ctx.r1.u32 + 113);
	// add r9,r20,r8
	ctx.r9.u64 = ctx.r20.u64 + ctx.r8.u64;
	// lbz r4,129(r1)
	ctx.r4.u64 = REX_LOAD_U8(ctx.r1.u32 + 129);
	// add r24,r24,r23
	ctx.r24.u64 = ctx.r24.u64 + ctx.r23.u64;
	// lbz r8,128(r1)
	ctx.r8.u64 = REX_LOAD_U8(ctx.r1.u32 + 128);
	// stb r26,2(r10)
	REX_STORE_U8(ctx.r10.u32 + 2, ctx.r26.u8);
	// stb r30,3(r10)
	REX_STORE_U8(ctx.r10.u32 + 3, ctx.r30.u8);
	// stb r3,4(r10)
	REX_STORE_U8(ctx.r10.u32 + 4, ctx.r3.u8);
	// stb r7,6(r10)
	REX_STORE_U8(ctx.r10.u32 + 6, ctx.r7.u8);
	// stb r22,0(r10)
	REX_STORE_U8(ctx.r10.u32 + 0, ctx.r22.u8);
	// stb r24,1(r10)
	REX_STORE_U8(ctx.r10.u32 + 1, ctx.r24.u8);
	// stb r5,5(r10)
	REX_STORE_U8(ctx.r10.u32 + 5, ctx.r5.u8);
	// stb r9,7(r10)
	REX_STORE_U8(ctx.r10.u32 + 7, ctx.r9.u8);
	// add r10,r10,r29
	ctx.r10.u64 = ctx.r10.u64 + ctx.r29.u64;
	// lbz r30,109(r1)
	ctx.r30.u64 = REX_LOAD_U8(ctx.r1.u32 + 109);
	// lbz r3,112(r1)
	ctx.r3.u64 = REX_LOAD_U8(ctx.r1.u32 + 112);
	// lbz r26,0(r11)
	ctx.r26.u64 = REX_LOAD_U8(ctx.r11.u32 + 0);
	// lbz r7,1(r11)
	ctx.r7.u64 = REX_LOAD_U8(ctx.r11.u32 + 1);
	// lbz r9,3(r11)
	ctx.r9.u64 = REX_LOAD_U8(ctx.r11.u32 + 3);
	// add r31,r7,r30
	ctx.r31.u64 = ctx.r7.u64 + ctx.r30.u64;
	// lbz r25,4(r11)
	ctx.r25.u64 = REX_LOAD_U8(ctx.r11.u32 + 4);
	// add r27,r27,r26
	ctx.r27.u64 = ctx.r27.u64 + ctx.r26.u64;
	// add r7,r9,r6
	ctx.r7.u64 = ctx.r9.u64 + ctx.r6.u64;
	// lbz r5,2(r11)
	ctx.r5.u64 = REX_LOAD_U8(ctx.r11.u32 + 2);
	// add r9,r25,r8
	ctx.r9.u64 = ctx.r25.u64 + ctx.r8.u64;
	// lbz r24,5(r11)
	ctx.r24.u64 = REX_LOAD_U8(ctx.r11.u32 + 5);
	// add r5,r5,r3
	ctx.r5.u64 = ctx.r5.u64 + ctx.r3.u64;
	// lbz r8,133(r1)
	ctx.r8.u64 = REX_LOAD_U8(ctx.r1.u32 + 133);
	// lbz r6,132(r1)
	ctx.r6.u64 = REX_LOAD_U8(ctx.r1.u32 + 132);
	// stb r27,0(r10)
	REX_STORE_U8(ctx.r10.u32 + 0, ctx.r27.u8);
	// stb r7,3(r10)
	REX_STORE_U8(ctx.r10.u32 + 3, ctx.r7.u8);
	// stb r9,4(r10)
	REX_STORE_U8(ctx.r10.u32 + 4, ctx.r9.u8);
	// lbz r9,7(r11)
	ctx.r9.u64 = REX_LOAD_U8(ctx.r11.u32 + 7);
	// stb r5,2(r10)
	REX_STORE_U8(ctx.r10.u32 + 2, ctx.r5.u8);
	// add r5,r24,r4
	ctx.r5.u64 = ctx.r24.u64 + ctx.r4.u64;
	// lbz r7,6(r11)
	ctx.r7.u64 = REX_LOAD_U8(ctx.r11.u32 + 6);
	// add r11,r11,r28
	ctx.r11.u64 = ctx.r11.u64 + ctx.r28.u64;
	// add r9,r9,r8
	ctx.r9.u64 = ctx.r9.u64 + ctx.r8.u64;
	// lbz r27,130(r1)
	ctx.r27.u64 = REX_LOAD_U8(ctx.r1.u32 + 130);
	// add r7,r7,r6
	ctx.r7.u64 = ctx.r7.u64 + ctx.r6.u64;
	// lbz r4,134(r1)
	ctx.r4.u64 = REX_LOAD_U8(ctx.r1.u32 + 134);
	// stb r31,1(r10)
	REX_STORE_U8(ctx.r10.u32 + 1, ctx.r31.u8);
	// stb r5,5(r10)
	REX_STORE_U8(ctx.r10.u32 + 5, ctx.r5.u8);
	// lbz r5,4(r11)
	ctx.r5.u64 = REX_LOAD_U8(ctx.r11.u32 + 4);
	// stb r9,7(r10)
	REX_STORE_U8(ctx.r10.u32 + 7, ctx.r9.u8);
	// lbz r9,6(r11)
	ctx.r9.u64 = REX_LOAD_U8(ctx.r11.u32 + 6);
	// add r30,r5,r27
	ctx.r30.u64 = ctx.r5.u64 + ctx.r27.u64;
	// stb r7,6(r10)
	REX_STORE_U8(ctx.r10.u32 + 6, ctx.r7.u8);
	// add r10,r10,r29
	ctx.r10.u64 = ctx.r10.u64 + ctx.r29.u64;
	// lbz r19,0(r11)
	ctx.r19.u64 = REX_LOAD_U8(ctx.r11.u32 + 0);
	// add r5,r9,r4
	ctx.r5.u64 = ctx.r9.u64 + ctx.r4.u64;
	// lbz r22,1(r11)
	ctx.r22.u64 = REX_LOAD_U8(ctx.r11.u32 + 1);
	// lbz r24,2(r11)
	ctx.r24.u64 = REX_LOAD_U8(ctx.r11.u32 + 2);
	// lbz r26,3(r11)
	ctx.r26.u64 = REX_LOAD_U8(ctx.r11.u32 + 3);
	// lbz r3,5(r11)
	ctx.r3.u64 = REX_LOAD_U8(ctx.r11.u32 + 5);
	// lbz r18,7(r11)
	ctx.r18.u64 = REX_LOAD_U8(ctx.r11.u32 + 7);
	// add r11,r11,r28
	ctx.r11.u64 = ctx.r11.u64 + ctx.r28.u64;
	// lbz r25,115(r1)
	ctx.r25.u64 = REX_LOAD_U8(ctx.r1.u32 + 115);
	// lbz r31,131(r1)
	ctx.r31.u64 = REX_LOAD_U8(ctx.r1.u32 + 131);
	// lbz r6,135(r1)
	ctx.r6.u64 = REX_LOAD_U8(ctx.r1.u32 + 135);
	// add r26,r26,r25
	ctx.r26.u64 = ctx.r26.u64 + ctx.r25.u64;
	// lbz r20,110(r1)
	ctx.r20.u64 = REX_LOAD_U8(ctx.r1.u32 + 110);
	// add r3,r3,r31
	ctx.r3.u64 = ctx.r3.u64 + ctx.r31.u64;
	// lbz r21,111(r1)
	ctx.r21.u64 = REX_LOAD_U8(ctx.r1.u32 + 111);
	// add r9,r18,r6
	ctx.r9.u64 = ctx.r18.u64 + ctx.r6.u64;
	// lbz r23,114(r1)
	ctx.r23.u64 = REX_LOAD_U8(ctx.r1.u32 + 114);
	// add r20,r20,r19
	ctx.r20.u64 = ctx.r20.u64 + ctx.r19.u64;
	// add r22,r22,r21
	ctx.r22.u64 = ctx.r22.u64 + ctx.r21.u64;
	// stb r5,6(r10)
	REX_STORE_U8(ctx.r10.u32 + 6, ctx.r5.u8);
	// add r24,r24,r23
	ctx.r24.u64 = ctx.r24.u64 + ctx.r23.u64;
	// lbz r6,0(r11)
	ctx.r6.u64 = REX_LOAD_U8(ctx.r11.u32 + 0);
	// lbz r5,1(r11)
	ctx.r5.u64 = REX_LOAD_U8(ctx.r11.u32 + 1);
	// lbz r8,117(r1)
	ctx.r8.u64 = REX_LOAD_U8(ctx.r1.u32 + 117);
	// lbz r7,116(r1)
	ctx.r7.u64 = REX_LOAD_U8(ctx.r1.u32 + 116);
	// stb r9,7(r10)
	REX_STORE_U8(ctx.r10.u32 + 7, ctx.r9.u8);
	// add r9,r5,r8
	ctx.r9.u64 = ctx.r5.u64 + ctx.r8.u64;
	// stb r26,3(r10)
	REX_STORE_U8(ctx.r10.u32 + 3, ctx.r26.u8);
	// add r7,r7,r6
	ctx.r7.u64 = ctx.r7.u64 + ctx.r6.u64;
	// stb r30,4(r10)
	REX_STORE_U8(ctx.r10.u32 + 4, ctx.r30.u8);
	// stb r3,5(r10)
	REX_STORE_U8(ctx.r10.u32 + 5, ctx.r3.u8);
	// stb r20,0(r10)
	REX_STORE_U8(ctx.r10.u32 + 0, ctx.r20.u8);
	// stb r22,1(r10)
	REX_STORE_U8(ctx.r10.u32 + 1, ctx.r22.u8);
	// stb r24,2(r10)
	REX_STORE_U8(ctx.r10.u32 + 2, ctx.r24.u8);
	// add r10,r10,r29
	ctx.r10.u64 = ctx.r10.u64 + ctx.r29.u64;
	// lbz r26,2(r11)
	ctx.r26.u64 = REX_LOAD_U8(ctx.r11.u32 + 2);
	// lbz r30,3(r11)
	ctx.r30.u64 = REX_LOAD_U8(ctx.r11.u32 + 3);
	// lbz r3,4(r11)
	ctx.r3.u64 = REX_LOAD_U8(ctx.r11.u32 + 4);
	// lbz r5,5(r11)
	ctx.r5.u64 = REX_LOAD_U8(ctx.r11.u32 + 5);
	// stb r7,0(r10)
	REX_STORE_U8(ctx.r10.u32 + 0, ctx.r7.u8);
	// stb r9,1(r10)
	REX_STORE_U8(ctx.r10.u32 + 1, ctx.r9.u8);
	// lbz r7,6(r11)
	ctx.r7.u64 = REX_LOAD_U8(ctx.r11.u32 + 6);
	// lbz r9,7(r11)
	ctx.r9.u64 = REX_LOAD_U8(ctx.r11.u32 + 7);
	// add r11,r11,r28
	ctx.r11.u64 = ctx.r11.u64 + ctx.r28.u64;
	// lbz r25,120(r1)
	ctx.r25.u64 = REX_LOAD_U8(ctx.r1.u32 + 120);
	// lbz r27,121(r1)
	ctx.r27.u64 = REX_LOAD_U8(ctx.r1.u32 + 121);
	// lbz r31,136(r1)
	ctx.r31.u64 = REX_LOAD_U8(ctx.r1.u32 + 136);
	// lbz r4,137(r1)
	ctx.r4.u64 = REX_LOAD_U8(ctx.r1.u32 + 137);
	// lbz r6,140(r1)
	ctx.r6.u64 = REX_LOAD_U8(ctx.r1.u32 + 140);
	// lbz r8,141(r1)
	ctx.r8.u64 = REX_LOAD_U8(ctx.r1.u32 + 141);
	// add r28,r26,r25
	ctx.r28.u64 = ctx.r26.u64 + ctx.r25.u64;
	// lbz r23,6(r11)
	ctx.r23.u64 = REX_LOAD_U8(ctx.r11.u32 + 6);
	// add r30,r30,r27
	ctx.r30.u64 = ctx.r30.u64 + ctx.r27.u64;
	// lbz r25,0(r11)
	ctx.r25.u64 = REX_LOAD_U8(ctx.r11.u32 + 0);
	// add r3,r3,r31
	ctx.r3.u64 = ctx.r3.u64 + ctx.r31.u64;
	// lbz r24,7(r11)
	ctx.r24.u64 = REX_LOAD_U8(ctx.r11.u32 + 7);
	// add r5,r5,r4
	ctx.r5.u64 = ctx.r5.u64 + ctx.r4.u64;
	// lbz r26,118(r1)
	ctx.r26.u64 = REX_LOAD_U8(ctx.r1.u32 + 118);
	// add r7,r7,r6
	ctx.r7.u64 = ctx.r7.u64 + ctx.r6.u64;
	// lbz r27,119(r1)
	ctx.r27.u64 = REX_LOAD_U8(ctx.r1.u32 + 119);
	// add r9,r9,r8
	ctx.r9.u64 = ctx.r9.u64 + ctx.r8.u64;
	// stb r28,2(r10)
	REX_STORE_U8(ctx.r10.u32 + 2, ctx.r28.u8);
	// stb r30,3(r10)
	REX_STORE_U8(ctx.r10.u32 + 3, ctx.r30.u8);
	// add r26,r26,r25
	ctx.r26.u64 = ctx.r26.u64 + ctx.r25.u64;
	// stb r3,4(r10)
	REX_STORE_U8(ctx.r10.u32 + 4, ctx.r3.u8);
	// stb r5,5(r10)
	REX_STORE_U8(ctx.r10.u32 + 5, ctx.r5.u8);
	// stb r7,6(r10)
	REX_STORE_U8(ctx.r10.u32 + 6, ctx.r7.u8);
	// stb r9,7(r10)
	REX_STORE_U8(ctx.r10.u32 + 7, ctx.r9.u8);
	// add r10,r10,r29
	ctx.r10.u64 = ctx.r10.u64 + ctx.r29.u64;
	// lbz r8,142(r1)
	ctx.r8.u64 = REX_LOAD_U8(ctx.r1.u32 + 142);
	// lbz r29,1(r11)
	ctx.r29.u64 = REX_LOAD_U8(ctx.r11.u32 + 1);
	// lbz r30,2(r11)
	ctx.r30.u64 = REX_LOAD_U8(ctx.r11.u32 + 2);
	// lbz r3,3(r11)
	ctx.r3.u64 = REX_LOAD_U8(ctx.r11.u32 + 3);
	// add r29,r29,r27
	ctx.r29.u64 = ctx.r29.u64 + ctx.r27.u64;
	// lbz r5,4(r11)
	ctx.r5.u64 = REX_LOAD_U8(ctx.r11.u32 + 4);
	// lbz r7,5(r11)
	ctx.r7.u64 = REX_LOAD_U8(ctx.r11.u32 + 5);
	// add r11,r23,r8
	ctx.r11.u64 = ctx.r23.u64 + ctx.r8.u64;
	// lbz r28,122(r1)
	ctx.r28.u64 = REX_LOAD_U8(ctx.r1.u32 + 122);
	// lbz r31,123(r1)
	ctx.r31.u64 = REX_LOAD_U8(ctx.r1.u32 + 123);
	// lbz r4,138(r1)
	ctx.r4.u64 = REX_LOAD_U8(ctx.r1.u32 + 138);
	// add r30,r30,r28
	ctx.r30.u64 = ctx.r30.u64 + ctx.r28.u64;
	// lbz r6,139(r1)
	ctx.r6.u64 = REX_LOAD_U8(ctx.r1.u32 + 139);
	// add r3,r3,r31
	ctx.r3.u64 = ctx.r3.u64 + ctx.r31.u64;
	// lbz r9,143(r1)
	ctx.r9.u64 = REX_LOAD_U8(ctx.r1.u32 + 143);
	// add r5,r5,r4
	ctx.r5.u64 = ctx.r5.u64 + ctx.r4.u64;
	// stb r11,6(r10)
	REX_STORE_U8(ctx.r10.u32 + 6, ctx.r11.u8);
	// add r7,r7,r6
	ctx.r7.u64 = ctx.r7.u64 + ctx.r6.u64;
	// add r11,r24,r9
	ctx.r11.u64 = ctx.r24.u64 + ctx.r9.u64;
	// stb r26,0(r10)
	REX_STORE_U8(ctx.r10.u32 + 0, ctx.r26.u8);
	// stb r29,1(r10)
	REX_STORE_U8(ctx.r10.u32 + 1, ctx.r29.u8);
	// stb r30,2(r10)
	REX_STORE_U8(ctx.r10.u32 + 2, ctx.r30.u8);
	// stb r3,3(r10)
	REX_STORE_U8(ctx.r10.u32 + 3, ctx.r3.u8);
	// stb r5,4(r10)
	REX_STORE_U8(ctx.r10.u32 + 4, ctx.r5.u8);
	// stb r7,5(r10)
	REX_STORE_U8(ctx.r10.u32 + 5, ctx.r7.u8);
	// stb r11,7(r10)
	REX_STORE_U8(ctx.r10.u32 + 7, ctx.r11.u8);
	// addi r1,r1,272
	ctx.r1.s64 = ctx.r1.s64 + 272;
	// b 0x829ff7ec
	__restgprlr_17(ctx, base);
	return;
}

