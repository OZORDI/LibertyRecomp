# Scene Pointer Readers: 0x831C2458 Trace

## Key Address Computation

- Base: `lis -31972` = `0x831C0000`
- Scene ptr array: base + 9304 = `0x831C2458` (0x831C2458)
- Gfx device: base + 8868 = `0x831C22A4`
- Frame index: base + 9160 = `0x831C23C8`
- Scene param: base + 9232 = `0x831C2410`

## sub_828C15C8: Render Dispatch Vtable Call

```
r11 = [0x831C2458]       // load scene object pointer
IF r11 != NULL:
  vtable = [r11 + 0]     // load vtable
  method = [vtable + 64] // vtable[16] at offset 0x40
  call method(r3=scene_obj)
```

- **Vtable offset**: 0x40 (64 bytes) = method index 16
- **Gfx device** loaded from `0x831C22A4` (used for D3D calls)

## Addresses Accessed in 0x831C2400-0x831C2500 Range

| Address | Functions | Access |
|-|-|-|
| `0x831C2410` | sub_822CFC00, sub_828BF898, sub_828C04E0, sub_828C1228, sub_828C15C8 | LOAD/STORE |
| `0x831C2414` | sub_828BEA58, sub_828BEAA8 | STORE |
| `0x831C2460` | sub_828BF3C8, sub_828BF420 | LOAD/STORE |
| `0x831C2470` | sub_8233E928 | LOAD |
| `0x831C24A0` | sub_828BF4F8 | STORE |

## Functions That READ from 0x831C2458 (offset 9304)

- **sub_828BD648**: 1 loads, 0 stores
- **sub_828BF4F8**: 1 loads, 1 stores
- **sub_828C0688**: 3 loads, 0 stores
- **sub_828C0848**: 3 loads, 0 stores
- **sub_828C15C8**: 2 loads, 0 stores
- **sub_828C19C0**: 1 loads, 0 stores
- **sub_828DA540**: 2 loads, 0 stores
- **sub_828DAD60**: 2 loads, 0 stores
- **sub_828DB160**: 1 loads, 0 stores
- **sub_828DBDA0**: 2 loads, 0 stores

## Functions That WRITE to 0x831C2458

No direct stores to offset 9304 found. The scene object may be written via:
- Indirect store through computed address
- addi to form base, then store at offset 0

Functions computing base+9304 (potential indirect writers):
- **sub_82170DE0**
- **sub_822BCC20**
- **sub_822BDD98**
- **sub_82548728**
- **sub_82674FF0**
- **sub_827AB9A0**
- **sub_828C15C8**
- **sub_828C73F0**
- **sub_828C7A30**
- **sub_828D85C8**
- **sub_828FB590**

## Functions Using Gfx Device (0x831C22A4)

- sub_82272240
- sub_82293878
- sub_8231FD20
- sub_8233ED88
- sub_8233F608
- sub_8235D388
- sub_824F50F0
- sub_824F6208
- sub_824F6478
- sub_824F64C0
- sub_824F6AF0
- sub_824F6EA8
- sub_824F7328
- sub_824F76C8
- sub_827C02A8
- sub_827C0760
- sub_828BCFA8
- sub_828BEA58
- sub_828BEAA8
- sub_828BEAF0
- sub_828BEBB8
- sub_828BEBD8
- sub_828BEC10
- sub_828BF0A0
- sub_828BF0E0
- sub_828BF120
- sub_828BF1F8
- sub_828BF248
- sub_828BF270
- sub_828BF280
- sub_828BF420
- sub_828BF788
- sub_828BF7D0
- sub_828BF880
- sub_828BF898
- sub_828BF930
- sub_828BF9F0
- sub_828BFA50
- sub_828BFA60
- sub_828BFAA8
- sub_828BFBE8
- sub_828BFC00
- sub_828BFD68
- sub_828BFDD8
- sub_828BFE48
- sub_828BFEA8
- sub_828BFF18
- sub_828BFFD8
- sub_828C0160
- sub_828C01E0
- sub_828C0338
- sub_828C0488
- sub_828C04E0
- sub_828C05A8
- sub_828C0688
- sub_828C0848
- sub_828C1228
- sub_828C15C8
- sub_828C8108
- sub_828CA318
- sub_828DA540
- sub_828DAD60
- sub_828DB160
- sub_828DFF00
- sub_828DFF28
- sub_828DFF50
- sub_828E0048
- sub_828E01E0
- sub_828E02E8

## Functions Calling vtable+0x40 (Same Dispatch Pattern)

- sub_82198C20
- sub_821BC900
- sub_8280BC58
- sub_8280E618
- sub_828944B8
- sub_828BEBB8
- sub_828E02E8
- sub_82960650
- sub_829A59E0

## Constructor Search

To find the vtable address, examine writers to 0x831C2458.
The vtable is stored at object+0 in the constructor.
Vtable method 16 (offset 0x40) is the render/draw dispatch.
