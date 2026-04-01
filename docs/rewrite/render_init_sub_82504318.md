# sub_82504318 -- Pool Allocator Init (NOT Scene Registration)

## Summary

sub_82504318 is **not** related to the scene pointer at 0x831C2458. It creates a RAGE
pool allocator and stores it at global 0x830248C4. The scene pointer remains NULL for
a different reason entirely.

## sub_82504318 Decompiled Logic

```
void sub_82504318() {
    void* pool_struct = sub_821B3510(28);     // alloc 28-byte pool header
    if (pool_struct == NULL) {
        *(0x830248C4) = 0;                    // null fallback
        return;
    }
    // sub_82507368: RAGE pool allocator constructor
    // params: this=pool_struct, count=32000, class=0x82022584, elem_size=112
    sub_82507368(pool_struct, 32000, 0x82022584, 112);
    *(0x830248C4) = pool_struct;              // store initialized pool
}
```

| Field | Value |
|-|-|
| Element count | 32,000 |
| Element size | 112 bytes |
| Total pool | 3,584,000 bytes (3.4 MB) |
| Global address | 0x830248C4 |
| Class descriptor | 0x82022584 (passed but unused by constructor) |

sub_82507368 internally allocates two buffers via sub_821B3510:
1. Data pool: 32000 * 112 = 3.4 MB (stores actual objects)
2. Bitmap: 32000 bytes (tracks free/used slots, each byte has bit 7 = allocated flag)

The pool header (28 bytes) stores: data_ptr[0], bitmap_ptr[4], capacity[8],
elem_size[12], min_used[16], free_count[20], active_flag[24].

## Position in sub_821FC1F8 Init Chain

sub_821FC1F8 is the master engine init function (47 sequential calls). sub_82504318
is call #5. The full chain with INIT_PROBE IDs:

| # | Function | Probe ID | Purpose |
|-|-|-|-|
| 1 | sub_8251BA08 | 2801 | Early memory/heap setup |
| 2 | sub_8251BA70 | - | Memory setup continued |
| 3 | sub_8254EE48 | - | System init |
| 4 | sub_825256F8 | - | System init |
| **5** | **sub_82504318** | **2805** | **Pool allocator init (32K x 112B, stored at 0x830248C4)** |
| 6 | sub_82326E10 | - | System init |
| 7 | sub_82504900 | - | System init (likely another pool) |
| 8 | sub_82374720 | - | System init |
| 9 | sub_82523CF0 | - | System init |
| 10 | sub_82446BA8 | 2810 | System init |
| 11-16 | sub_82446C78..E18 | 2815 | Six sequential inits (0x82446xxx range) |
| 17 | sub_82412560 | - | System init |
| 18 | sub_8254D230 | - | System init |
| 19 | sub_8230AF58 | - | System init |
| 20 | sub_8254A610 | 2820 | System init |
| 21-22 | sub_8254A6E0/678 | - | System init |
| 23 | sub_823D46A0 | - | System init |
| 24 | sub_82549D88 | - | System init |
| 25 | sub_82163F38 | 2825 | System init |
| 26 | sub_824E8C00 | - | System init |
| 27 | sub_823B31B8 | - | System init |
| 28 | sub_8247E4C0 | - | System init (takes addr 0x82FF5518) |
| 29 | sub_823C04B0 | - | System init |
| 30 | sub_82478AF8 | 2830 | **Audio/scene engine init** (creates grcSceneList, stores at 0x82FF5368) |
| 31 | sub_8225C010 | - | System init |
| 32 | sub_82556190 | - | System init |
| 33 | sub_822F3740 | - | System init |
| 34 | sub_8237C250 | - | System init |
| 35 | sub_822B2010 | 2835 | System init |
| 36 | sub_823BFA98 | - | System init (takes addr 0x82FC9720) |
| 37 | sub_8228E3A0 | - | System init |
| 38 | sub_822BCC20 | - | Network/multiplayer init |
| 39 | sub_822BDD98 | - | Network init continued |
| 40 | sub_823A2108 | 2840 | System init (params: r3=2, r4-r7=0) |
| 41 | sub_822BCA90 | - | Network tick |
| 42 | sub_82552D08 | - | System init |
| 43 | sub_821CA7E8 | - | System init |
| 44 | sub_825169C8 | - | System init |
| 45 | sub_825030B8 | 2845 | System init |
| 46 | sub_822BCA90 | - | Network tick (again) |
| 47 | sub_8251CA08 | - | Final init |

## Why 0x831C2458 is NULL

### The scene creation path works:
1. sub_82478AF8 (call #30) creates a grcSceneList via sub_827ADB48
2. The constructed scene object is stored at global **0x82FF5368** (g_scene)

### The scene registration path is missing:
1. The render dispatch (sub_828C15C8) reads `*(0x831C2458)` for the scene list pointer
2. **No generated PPC code writes to 0x831C2458** -- confirmed by exhaustive grep
3. The only reference to offset 9304 from base 0x831C0000 is the **read** in sub_828C15C8
4. The registration must happen inside D3D device initialization (grcDevice::Create)
5. All D3D device init is stubbed via VdInitializeEngines and related Vd* imports

### Two distinct addresses:
| Address | Purpose | Status |
|-|-|-|
| 0x82FF5368 | g_scene -- where the constructed grcSceneList is stored | Written by sub_82478AF8 |
| 0x831C2458 | Device scene list array slot -- where the render dispatch reads from | **Never written (NULL)** |

### The missing bridge:
Somewhere in the original D3D device creation path (likely inside a function called
by VdInitializeEngines or the RAGE grcDevice constructor), code like this ran:

```
*(0x831C2458) = *(0x82FF5368);   // register scene into device array
```

This code is inside a Vd* kernel import or a dependent D3D function that was stubbed.

## Relation to sub_82504318

sub_82504318 is **25 calls before** sub_82478AF8 (scene creation) in the init chain.
It creates a generic pool allocator at 0x830248C4. This pool is likely used by
subsequent initialization (particle systems, render objects, etc.) but has no direct
relationship to the scene pointer at 0x831C2458.

## Actionable Fix

To bridge the gap, a hook should copy the scene pointer after sub_82478AF8 returns:

```cpp
PPC_FUNC_HOOK(sub_82478AF8) {
    __imp__sub_82478AF8(ctx, base);
    uint32_t scene = PPC_LOAD_U32(0x82FF5368);
    if (scene != 0) {
        PPC_STORE_U32(0x831C2458, scene);
        printf("[SCENE-BRIDGE] Registered scene 0x%08X into device array\n", scene);
    }
}
```

**Warning**: This alone is insufficient for rendering. The scene object also needs
valid render targets, shaders, and vertex/index buffers. See render_frame_loop.md
section 6 for the full list of missing pieces.
