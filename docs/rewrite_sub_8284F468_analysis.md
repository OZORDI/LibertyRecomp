# Analysis: sub_8284F468 (Streaming Resource Path Resolver)

## r3 Argument Computation

From `sub_82852DD0` (line 31088-31097):
```
lis r11, -32080       → r11 = 0x82B00000
addi r3, r11, 29304   → r3 = 0x82B07278
```
Python: `((-32080 & 0xFFFF) << 16) + 29304 = 0x82B07278`

**0x82B07278** is a global streaming device manager object. Its field at +3076 (`0xC04`) holds the count of registered device handlers, and field +3072 (`0xC00`) holds a device index used for path resolution.

## sub_8284F468 — Streaming Resource Path Resolver + Handler Lookup

**Signature**: `sub_8284F468(device_mgr, path_buf, buf_size=256, ext_filter, should_open=1)`

From `sub_82852DD0`: r3=0x82B07278, r4=path_str, r5=256 (set in sub_8284F0C0), r6=0, r7=1.

**Algorithm**:
1. Saves r3→r30 (device_mgr), r4→r29 (path), r5→r28 (extension filter), r7→r27 (should_open flag).
2. Sets `r3 = 0` (result), `r31 = 0` (loop counter).
3. Reads `device_mgr[3076]` → handler count. If <= 0, skips loop.
4. **Loop** (r31 from 0 to handler_count, breaks when r3 != 0):
   - Calls `sub_8284F0C0(device_mgr, stack_buf, 256, ext_filter, r28, r31)` — resolves the path through device handler #r31. This function:
     - Checks if path starts with `/`, `\`, or contains `:` (absolute path detection).
     - If relative: copies device name from `device_mgr[3072]`-indexed slot into buffer, then appends the path.
     - Calls `sub_8284E690` to check if a device recognizes the path.
     - Calls `sub_8284ED70` to build the final resolved path (with `\`→`/` normalization, extension appending via strrchr+stricmp).
   - Calls `sub_8285AA68(stack_buf, should_open)` — attempts to open/find the resource via the resolved path. This function:
     - Calls `sub_82855460(stack_buf)` to identify which device handler type matches (via strncmp against "common:", "platform:", "update:" prefixes).
     - Calls vtable[4] on the matched handler with the path → returns a resource index (or -1 on failure).
     - If found and a callback pointer exists at a global, invokes callback for filtering.
     - On success, calls `sub_8285A968` which registers the resource in a global slot table (28-byte entries, up to a global max at `0x82B08234`).
     - Returns the slot pointer, or 0 on failure.
5. Returns the resource slot pointer in r3 (or 0 if no handler could resolve the path).

**Does NOT block on sync primitives.** No critical section enter/leave within sub_8284F468 itself. However, `sub_82852A50` (called afterward in `sub_82852D18`) does acquire a lock via `sub_828470E0`/`sub_82847120` around the resource lookup.

## sub_8284FA58 — Resource Slot Destructor

**Signature**: `sub_8284FA58(resource_slot)`

**Algorithm**:
1. Reads `slot[8]` (u16 count). Iterates `slot[4]` array, calling `sub_821B3560` (TLS-based allocator free) on each 4-byte pointer entry.
2. Reads `slot[0]` (main buffer ptr). If non-null: calls `sub_82863628` (unlinks from parent's linked list via fields +20/+24/+28), then `sub_821B3560` to free it.
3. Reads `slot[10]` (u16 owns-array flag). If non-zero, frees the array pointer at `slot[4]`.

This is the **teardown/deallocation counterpart** to the resource slot allocated by `sub_8285A968` (inside `sub_8285AA68`).

## sub_8285B088 — Resource Handle Close

Called in `sub_82852DD0` after `sub_82852D18` returns, on the resource slot from sub_8284F468.

**Algorithm**:
1. Checks `slot[20]` (pending bytes) and `slot[16]` (completed bytes). If pending != 0 but completed == 0, calls `sub_8285A8B0` to **flush any in-flight I/O** (calls vtable[36] on the device to submit partial reads, then vtable[52] to finalize).
2. Calls vtable[40] on `slot[0]` device with `slot[4]` handle to **close the resource handle**.
3. Zeroes `slot[0]` and `slot[4]`, sets `slot[4] = -1`.

## Relationship Summary

| Function | Role |
|-|-|
| sub_8284F468 | Resolve path across device handlers, open resource, return slot |
| sub_8285AA68 | Inner: try one resolved path against device vtable[4], register slot |
| sub_8285A968 | Register resource in global 28-byte slot table |
| sub_82852A50 | Look up resource by path (with critical section), call vtable for data |
| sub_8285B088 | Flush pending I/O + close handle via vtable[40] |
| sub_8284FA58 | Free slot memory: array elements, main buffer, unlink from list |
| sub_821B3560 | TLS allocator free (reads TLS[1676] → vtable[12]) |
| sub_82863628 | Unlink node from intrusive linked list (+20/+24/+28 fields) |

## Global State

- **0x82B07278**: Streaming device manager (field +0xC04 = handler count, +0xC00 = active device index)
- **0x831E55EC**: Streaming flags word (bit 17 = critical section needed for resource ops)
- **0x82B08234**: Global slot table high-water mark for registered resources

## Call Flow in sub_82852DD0

```
sub_82852DD0(path, ...) {
    slot = sub_8284F468(0x82B07278, path, 256, 0, 1);  // resolve + open
    if (slot) {
        result = sub_82852D18(path, slot, ...);          // read resource data (with lock)
        sub_8285B088(slot);                               // flush I/O + close handle
    }
    return result;
}
```

In `sub_82852D18`, after reading data, the cleanup path calls:
```
sub_8284FA58(resource_slot);  // free slot arrays and buffers
sub_821B3560(resource_slot);  // free the slot allocation itself
```
