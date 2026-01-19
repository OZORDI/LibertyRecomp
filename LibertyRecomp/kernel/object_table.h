/**
 * ObjectTable - Xenia-style kernel object handle management
 * 
 * Based on xenia/kernel/util/object_table.h
 * Provides proper handle allocation, lookup, and reference counting.
 */

#pragma once

#include <stdafx.h>
#include <cstdint>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <string>
#include <cassert>

#include "xdm.h"

namespace kernel {

// Handle base offset (matches Xenia for compatibility)
constexpr uint32_t kHandleBase = 0xF8000000;

// Object types for type-safe lookup
enum class ObjectType : uint32_t {
    Undefined = 0,
    Event,
    Semaphore,
    Mutant,
    Timer,
    Thread,
    File,
    Directory,
    Enumerator,
    NotifyListener,
    IoCompletion,
};

/**
 * ObjectTable manages kernel object handles.
 * 
 * Handles are allocated from a table and reference-counted.
 * Objects are deleted when their handle ref count reaches zero.
 */
class ObjectTable {
public:
    ObjectTable();
    ~ObjectTable();

    // Reset table (clears all handles)
    void Reset();

    // Add object to table, returns handle
    uint32_t AddHandle(KernelObject* object);
    
    // Duplicate handle (increments ref count)
    uint32_t DuplicateHandle(uint32_t handle);
    
    // Release handle (decrements ref count, may delete object)
    bool ReleaseHandle(uint32_t handle);
    
    // Lookup object by handle (returns nullptr if not found)
    KernelObject* LookupObject(uint32_t handle);
    
    // Typed lookup with type verification
    // Returns nullptr if handle invalid OR if object is not of type T
    template<typename T>
    T* LookupObject(uint32_t handle) {
        KernelObject* obj = LookupObject(handle);
        if (!obj) return nullptr;
        
        // Use dynamic_cast for runtime type checking
        // This is safer than blind static_cast and catches type mismatches
        return dynamic_cast<T*>(obj);
    }
    
    // Typed lookup with explicit type check (for performance-critical paths)
    // Caller must ensure type safety - no runtime check performed
    template<typename T>
    T* LookupObjectUnchecked(uint32_t handle) {
        return static_cast<T*>(LookupObject(handle));
    }
    
    // Check if handle is valid
    bool IsValidHandle(uint32_t handle) const;

    // Get handle from object pointer (for objects that store their handle)
    static uint32_t HandleFromSlot(uint32_t slot) {
        return kHandleBase + (slot << 2);
    }
    
    static uint32_t SlotFromHandle(uint32_t handle) {
        return (handle - kHandleBase) >> 2;
    }

private:
    struct TableEntry {
        int ref_count = 0;
        KernelObject* object = nullptr;
    };

    uint32_t FindFreeSlot();
    TableEntry* GetEntry(uint32_t handle);

    mutable std::mutex mutex_;
    std::vector<TableEntry> table_;
    uint32_t last_free_slot_ = 0;
    uint32_t table_capacity_ = 0;
};

// Global object table instance
extern ObjectTable g_objectTable;

} // namespace kernel
