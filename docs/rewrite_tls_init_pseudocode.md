# TLS Slot Allocation — sub_82478AF8 Phase 1

**Source files:**
- `gta4_recomp.36.cpp` lines 24814–24998 — sub_826225E0, sub_82622648, sub_826226B0
- `gta4_recomp.24.cpp` lines 63960–64057 — sub_82507368
- `gta4_recomp.2.cpp`  lines 83060–83084  — sub_821B3510

These three functions are called at the very start of `sub_82478AF8` (audio/streaming init) to set up per-thread ring buffer contexts via TLS.

---

## sub_821B3510 — RAGE pool allocator (vtable dispatch)

**Called with:** r3 = size in bytes

```
r11 = *(r13 + 0)          ; r13 = Xbox 360 TIB base (thread info block)
r10 = 1676
r4  = r3                   ; save size
r5  = 16                   ; alignment
r6  = 0                    ; flags
r3  = *(r11 + r10)         ; heap context ptr = TIB[1676]
r11 = *r3                  ; vtable
r11 = *(r11 + 8)           ; vtable[2] = alloc method
ctr = r11
bctr                        ; call vtable->alloc(heap_ctx, size, align=16, flags=0)
; returns: r3 = allocated pointer, or 0 on failure
```

**Semantics:** reads the RAGE allocator context out of TLS offset 1676, then virtual-dispatches through vtable slot 2 (byte offset 8) to allocate `size` bytes with 16-byte alignment.

---

## sub_82507368 — Ring buffer initializer

**Signature:** `sub_82507368(struct_ptr r3, element_count r4, config_ptr r5, element_size r6)`

```
r31 = r3    ; output struct
r30 = r4    ; element_count

; First alloc: buffer array
r3  = r30 * r6             ; total = count * element_size
[r31+12] = r6              ; store element_size
call sub_821B3510(total)
[r31+0]  = r3              ; buffer_array ptr

; Second alloc: index array
r3  = r30                  ; count (one byte per slot)
call sub_821B3510(count)
[r31+4]  = r3              ; index_array ptr

; Write struct header
[r31+8]  = r30             ; element_count
[r31+16] = 0xFFFFFFFF      ; head = -1 (empty)
[r31+20] = 0               ; tail = 0
[r31+24] = 1  (byte)       ; active flag

; Init loop: mark all index slots as free (0x81)
for i in 0..element_count:
    index[i] = (index[i] | 0x80)   ; set free bit
    index[i] = (index[i] & ~0x7E) | 0x01  ; set used=1 flag
    ; net result: index[i] = 0x81
```

**Ring buffer struct layout (at r3 on return):**

| offset | size | value | meaning |
|-|-|-|-|
| 0 | 4 | ptr | buffer_array (count * elem_size bytes) |
| 4 | 4 | ptr | index_array (count bytes) |
| 8 | 4 | count | element_count |
| 12 | 4 | size | element_size |
| 16 | 4 | 0xFFFFFFFF | head (ring head, -1 = empty) |
| 20 | 4 | 0 | tail |
| 24 | 1 | 1 | active |

---

## sub_826225E0 — TLS slot init A

**No arguments. No return value used by caller.**

```
; Step 1: allocate 28-byte TLS slot object
r3 = 28
call sub_821B3510(28)     ; → r3 = ptr or 0

if r3 == 0:
    [0x8309D584] = 0      ; store null → init failed
    return

; Step 2: init ring buffer A
r3 = ptr                  ; struct_ptr
r4 = 4690                 ; element_count
r5 = 0x8203FE4C           ; config struct (r11=-32252 addi -436)
r6 = 96                   ; element_size
call sub_82507368(ptr, 4690, 0x8203FE4C, 96)

; Step 3: write global
[0x8309D584] = r3         ; r11 = -31990 → 0x830A0000, offset -10876
return
```

**Global written:** `0x8309D584` ← ring buffer A pointer

**Ring buffer A:** 4690 entries × 96 bytes = 450,240 bytes (~439 KB)

---

## sub_82622648 — TLS slot init B

**No arguments. No return value used by caller.**

```
r3 = 28
call sub_821B3510(28)

if r3 == 0:
    [0x8309D588] = 0
    return

r3 = ptr
r4 = 864
r5 = 0x8203FE58           ; config struct (addi -424)
r6 = 176
call sub_82507368(ptr, 864, 0x8203FE58, 176)

[0x8309D588] = r3
return
```

**Global written:** `0x8309D588` ← ring buffer B pointer

**Ring buffer B:** 864 entries × 176 bytes = 152,064 bytes (~148 KB)

---

## sub_826226B0 — TLS slot init C

**No arguments. No return value used by caller.**

```
r3 = 28
call sub_821B3510(28)

if r3 == 0:
    [0x8309D58C] = 0
    return

r3 = ptr
r4 = 90
r5 = 0x8203FE64           ; config struct (addi -412)
r6 = 752
call sub_82507368(ptr, 90, 0x8203FE64, 752)

[0x8309D58C] = r3
return
```

**Global written:** `0x8309D58C` ← ring buffer C pointer

**Ring buffer C:** 90 entries × 752 bytes = 67,680 bytes (~66 KB)

---

## Global State Written by Phase 1

| address | value | set by |
|-|-|-|
| `0x8309D584` | ring_buf_A ptr | sub_826225E0 |
| `0x8309D588` | ring_buf_B ptr | sub_82622648 |
| `0x8309D58C` | ring_buf_C ptr | sub_826226B0 |

**On allocation failure** each function stores 0 at the respective global and returns without calling sub_82507368.

---

## TLS / Memory Architecture Notes

- `r13` is the Xbox 360 thread information block (TIB) base register, equivalent to x86 `fs`/`gs`. Always holds thread-local base.
- `r13+0` points to the XTHREAD structure.
- `TIB[1676]` (`r13[0][1676]`) is the RAGE allocator context pointer for the current thread (heap TLS slot).
- `sub_821B3510` is a thin dispatcher: reads heap context from TLS, calls `vtable[2]` (alloc). This is the same function documented as the "ALLOC FALLBACK" path in earlier analysis — when TLS[1676] is null, it hits the fallback host page allocator.
- The config struct addresses (`0x8203FE4C`, `0x8203FE58`, `0x8203FE64`) are in the `.rodata` / static data segment near `0x82040000`.

---

## Buffer Size Arithmetic (Python-verified)

```python
# Globals: r11 = -2096496640 = 0x830A0000
# offsets: -10876, -10872, -10868
0x830A0000 - 10876 = 0x8309D584  # A
0x830A0000 - 10872 = 0x8309D588  # B
0x830A0000 - 10868 = 0x8309D58C  # C

# Config ptrs: r11 = -2113667072 = 0x82040000
0x82040000 - 436 = 0x8203FE4C  # A
0x82040000 - 424 = 0x8203FE58  # B
0x82040000 - 412 = 0x8203FE64  # C

# Ring buffer total sizes
4690 * 96  = 450240  # A (~439 KB)
864  * 176 = 152064  # B (~148 KB)
90   * 752 = 67680   # C (~66 KB)
```
