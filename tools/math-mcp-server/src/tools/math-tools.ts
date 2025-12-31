// Math Tools for LibertyRecomp MCP Server
// Specialized calculations for PPC recompilation work

export class MathTools {
  // ==================== HEX/ADDRESS MATH ====================
  
  hexToDec(hex: string, signed: boolean = true, bits: number = 32): { decimal: string; unsigned: string; signed: string; hex: string } {
    // Remove 0x prefix if present
    const cleanHex = hex.replace(/^0x/i, '');
    const value = BigInt('0x' + cleanHex);
    
    const maxUnsigned = (1n << BigInt(bits)) - 1n;
    const signBit = 1n << BigInt(bits - 1);
    
    // Mask to bit width
    const masked = value & maxUnsigned;
    
    // Calculate signed value
    let signedValue: bigint;
    if (masked >= signBit) {
      signedValue = masked - (1n << BigInt(bits));
    } else {
      signedValue = masked;
    }
    
    return {
      decimal: signed ? signedValue.toString() : masked.toString(),
      unsigned: masked.toString(),
      signed: signedValue.toString(),
      hex: '0x' + masked.toString(16).toUpperCase().padStart(bits / 4, '0'),
    };
  }

  decToHex(decimal: string, bits: number = 32): { hex: string; unsigned: string; signed: string } {
    let value = BigInt(decimal);
    const maxUnsigned = (1n << BigInt(bits)) - 1n;
    const signBit = 1n << BigInt(bits - 1);
    
    // Handle negative numbers (two's complement)
    if (value < 0n) {
      value = (1n << BigInt(bits)) + value;
    }
    
    const masked = value & maxUnsigned;
    
    // Calculate signed interpretation
    let signedValue: bigint;
    if (masked >= signBit) {
      signedValue = masked - (1n << BigInt(bits));
    } else {
      signedValue = masked;
    }
    
    return {
      hex: '0x' + masked.toString(16).toUpperCase().padStart(bits / 4, '0'),
      unsigned: masked.toString(),
      signed: signedValue.toString(),
    };
  }

  addOffset(base: string, offset: string): { 
    result: string; 
    resultHex: string;
    base: string;
    offset: string;
    calculation: string;
  } {
    // Parse base (can be hex or decimal)
    const baseValue = base.startsWith('0x') || base.startsWith('0X') 
      ? BigInt(base) 
      : BigInt(base);
    
    // Parse offset (can be hex or decimal, including negative)
    const offsetValue = offset.startsWith('0x') || offset.startsWith('0X')
      ? BigInt(offset)
      : BigInt(offset);
    
    const result = baseValue + offsetValue;
    
    // Mask to 32-bit for address calculations
    const masked = result & 0xFFFFFFFFn;
    
    return {
      result: masked.toString(),
      resultHex: '0x' + masked.toString(16).toUpperCase().padStart(8, '0'),
      base: '0x' + (baseValue & 0xFFFFFFFFn).toString(16).toUpperCase(),
      offset: offsetValue.toString(),
      calculation: `${base} + ${offset} = 0x${masked.toString(16).toUpperCase()}`,
    };
  }

  addressRange(start: string, size: string): {
    start: string;
    end: string;
    size: string;
    startHex: string;
    endHex: string;
    sizeHex: string;
  } {
    const startValue = start.startsWith('0x') ? BigInt(start) : BigInt(start);
    const sizeValue = size.startsWith('0x') ? BigInt(size) : BigInt(size);
    const endValue = startValue + sizeValue - 1n;
    
    return {
      start: startValue.toString(),
      end: endValue.toString(),
      size: sizeValue.toString(),
      startHex: '0x' + startValue.toString(16).toUpperCase().padStart(8, '0'),
      endHex: '0x' + endValue.toString(16).toUpperCase().padStart(8, '0'),
      sizeHex: '0x' + sizeValue.toString(16).toUpperCase(),
    };
  }

  // ==================== PPC-SPECIFIC MATH ====================

  lisCalculation(immediate: string): {
    immediate: number;
    result: string;
    resultHex: string;
    signedResult: string;
    explanation: string;
  } {
    // lis loads a 16-bit immediate into the upper 16 bits of a register
    // lis rD, imm => rD = imm << 16 (sign-extended to 64-bit)
    const imm = immediate.startsWith('0x') 
      ? parseInt(immediate, 16) 
      : parseInt(immediate);
    
    // Sign-extend 16-bit immediate
    const signedImm = imm > 0x7FFF ? imm - 0x10000 : imm;
    
    // Shift left by 16
    const result = signedImm * 0x10000;
    
    // Convert to unsigned 32-bit representation
    const unsigned32 = result < 0 ? (result + 0x100000000) : result;
    
    return {
      immediate: imm,
      result: unsigned32.toString(),
      resultHex: '0x' + (unsigned32 >>> 0).toString(16).toUpperCase().padStart(8, '0'),
      signedResult: result.toString(),
      explanation: `lis loads ${imm} (0x${imm.toString(16)}) << 16 = ${result} (0x${(unsigned32 >>> 0).toString(16).toUpperCase()})`,
    };
  }

  effectiveAddress(lisValue: string, offset: string): {
    lisValue: string;
    offset: string;
    effectiveAddress: string;
    effectiveAddressHex: string;
    breakdown: string;
  } {
    // Common pattern: lis rX, upper; lwz/addi rY, offset(rX)
    // EA = (lisValue << 16) + offset
    
    const lisImm = lisValue.startsWith('0x') || lisValue.startsWith('-')
      ? parseInt(lisValue)
      : parseInt(lisValue);
    
    const off = offset.startsWith('0x') 
      ? parseInt(offset, 16)
      : parseInt(offset);
    
    // Sign-extend offset if needed (16-bit signed)
    const signedOffset = off > 0x7FFF ? off - 0x10000 : off;
    
    // If lisValue is already the shifted value (large number), use it directly
    // Otherwise, apply the lis shift
    let baseValue: number;
    if (Math.abs(lisImm) > 0xFFFF) {
      // Already shifted or a full address
      baseValue = lisImm;
    } else {
      // Apply lis shift
      const signedLis = lisImm > 0x7FFF ? lisImm - 0x10000 : lisImm;
      baseValue = signedLis * 0x10000;
    }
    
    const ea = baseValue + signedOffset;
    const unsigned32 = ea < 0 ? (ea + 0x100000000) : ea;
    
    return {
      lisValue: lisValue,
      offset: offset,
      effectiveAddress: unsigned32.toString(),
      effectiveAddressHex: '0x' + (unsigned32 >>> 0).toString(16).toUpperCase().padStart(8, '0'),
      breakdown: `base(${baseValue}) + offset(${signedOffset}) = ${ea} (0x${(unsigned32 >>> 0).toString(16).toUpperCase()})`,
    };
  }

  ppcOffsetDecode(value: string): {
    original: string;
    as16BitSigned: number;
    as32BitSigned: number;
    hex: string;
    interpretation: string;
  } {
    const val = value.startsWith('0x') ? parseInt(value, 16) : parseInt(value);
    
    // 16-bit signed interpretation
    const signed16 = val > 0x7FFF ? val - 0x10000 : (val < -0x8000 ? val : val & 0xFFFF);
    const actual16 = (val & 0xFFFF) > 0x7FFF ? (val & 0xFFFF) - 0x10000 : (val & 0xFFFF);
    
    // 32-bit signed interpretation  
    const signed32 = val > 0x7FFFFFFF ? val - 0x100000000 : val;
    
    return {
      original: value,
      as16BitSigned: actual16,
      as32BitSigned: signed32,
      hex: '0x' + (val & 0xFFFF).toString(16).toUpperCase(),
      interpretation: `PPC offset ${value} = ${actual16} as 16-bit signed, ${signed32} as 32-bit signed`,
    };
  }

  // ==================== SIGNED/UNSIGNED CONVERSIONS ====================

  twosComplement(value: string, bits: number = 32): {
    input: string;
    bits: number;
    unsigned: string;
    signed: string;
    hex: string;
    binary: string;
  } {
    const val = BigInt(value.startsWith('0x') ? value : value);
    const maxVal = (1n << BigInt(bits)) - 1n;
    const signBit = 1n << BigInt(bits - 1);
    
    let unsigned: bigint;
    let signed: bigint;
    
    if (val < 0n) {
      // Input is negative, convert to unsigned
      unsigned = (1n << BigInt(bits)) + val;
      signed = val;
    } else {
      unsigned = val & maxVal;
      signed = unsigned >= signBit ? unsigned - (1n << BigInt(bits)) : unsigned;
    }
    
    return {
      input: value,
      bits,
      unsigned: unsigned.toString(),
      signed: signed.toString(),
      hex: '0x' + unsigned.toString(16).toUpperCase().padStart(bits / 4, '0'),
      binary: unsigned.toString(2).padStart(bits, '0'),
    };
  }

  signExtend(value: string, fromBits: number, toBits: number): {
    input: string;
    fromBits: number;
    toBits: number;
    result: string;
    resultHex: string;
    resultSigned: string;
  } {
    const val = BigInt(value.startsWith('0x') ? value : value);
    const fromMask = (1n << BigInt(fromBits)) - 1n;
    const fromSignBit = 1n << BigInt(fromBits - 1);
    const toMask = (1n << BigInt(toBits)) - 1n;
    
    const masked = val & fromMask;
    let extended: bigint;
    
    if (masked >= fromSignBit) {
      // Negative in source width, sign-extend with 1s
      const signExtension = toMask ^ fromMask;
      extended = masked | signExtension;
    } else {
      extended = masked;
    }
    
    const toSignBit = 1n << BigInt(toBits - 1);
    const signedResult = extended >= toSignBit ? extended - (1n << BigInt(toBits)) : extended;
    
    return {
      input: value,
      fromBits,
      toBits,
      result: extended.toString(),
      resultHex: '0x' + extended.toString(16).toUpperCase().padStart(toBits / 4, '0'),
      resultSigned: signedResult.toString(),
    };
  }

  // ==================== BIT OPERATIONS ====================

  bitMask(startBit: number, endBit: number, bits: number = 32): {
    startBit: number;
    endBit: number;
    mask: string;
    maskHex: string;
    maskBinary: string;
  } {
    // Create mask from startBit to endBit (inclusive, 0-indexed from LSB)
    let mask = 0n;
    for (let i = startBit; i <= endBit && i < bits; i++) {
      mask |= (1n << BigInt(i));
    }
    
    return {
      startBit,
      endBit,
      mask: mask.toString(),
      maskHex: '0x' + mask.toString(16).toUpperCase().padStart(bits / 4, '0'),
      maskBinary: mask.toString(2).padStart(bits, '0'),
    };
  }

  bitShift(value: string, amount: number, direction: 'left' | 'right', logical: boolean = true, bits: number = 32): {
    input: string;
    amount: number;
    direction: string;
    type: string;
    result: string;
    resultHex: string;
  } {
    let val = BigInt(value.startsWith('0x') ? value : value);
    const mask = (1n << BigInt(bits)) - 1n;
    val = val & mask;
    
    let result: bigint;
    
    if (direction === 'left') {
      result = (val << BigInt(amount)) & mask;
    } else {
      if (logical) {
        result = val >> BigInt(amount);
      } else {
        // Arithmetic right shift (sign-extend)
        const signBit = 1n << BigInt(bits - 1);
        if (val >= signBit) {
          const shifted = val >> BigInt(amount);
          const signExtension = mask ^ ((1n << BigInt(bits - amount)) - 1n);
          result = shifted | signExtension;
        } else {
          result = val >> BigInt(amount);
        }
      }
    }
    
    return {
      input: value,
      amount,
      direction,
      type: logical ? 'logical' : 'arithmetic',
      result: result.toString(),
      resultHex: '0x' + result.toString(16).toUpperCase().padStart(bits / 4, '0'),
    };
  }

  bitExtract(value: string, startBit: number, numBits: number): {
    input: string;
    startBit: number;
    numBits: number;
    extracted: string;
    extractedHex: string;
    extractedBinary: string;
  } {
    const val = BigInt(value.startsWith('0x') ? value : value);
    const mask = (1n << BigInt(numBits)) - 1n;
    const extracted = (val >> BigInt(startBit)) & mask;
    
    return {
      input: value,
      startBit,
      numBits,
      extracted: extracted.toString(),
      extractedHex: '0x' + extracted.toString(16).toUpperCase(),
      extractedBinary: extracted.toString(2).padStart(numBits, '0'),
    };
  }

  rlwinm(value: string, shift: number, maskBegin: number, maskEnd: number): {
    input: string;
    shift: number;
    maskBegin: number;
    maskEnd: number;
    rotated: string;
    mask: string;
    result: string;
    resultHex: string;
    explanation: string;
  } {
    // PPC rlwinm: Rotate Left Word Immediate then AND with Mask
    // rlwinm rA, rS, SH, MB, ME
    // rA = ROTL32(rS, SH) & MASK(MB, ME)
    
    let val = BigInt(value.startsWith('0x') ? value : value) & 0xFFFFFFFFn;
    
    // Rotate left by shift amount
    const rotated = ((val << BigInt(shift)) | (val >> BigInt(32 - shift))) & 0xFFFFFFFFn;
    
    // Create mask from MB to ME (PPC uses bit 0 = MSB convention)
    // Convert to standard LSB-0 convention
    let mask = 0n;
    if (maskBegin <= maskEnd) {
      // Contiguous mask
      for (let i = maskBegin; i <= maskEnd; i++) {
        mask |= (1n << BigInt(31 - i));
      }
    } else {
      // Wrapped mask
      for (let i = maskBegin; i <= 31; i++) {
        mask |= (1n << BigInt(31 - i));
      }
      for (let i = 0; i <= maskEnd; i++) {
        mask |= (1n << BigInt(31 - i));
      }
    }
    
    const result = rotated & mask;
    
    return {
      input: value,
      shift,
      maskBegin,
      maskEnd,
      rotated: '0x' + rotated.toString(16).toUpperCase().padStart(8, '0'),
      mask: '0x' + mask.toString(16).toUpperCase().padStart(8, '0'),
      result: result.toString(),
      resultHex: '0x' + result.toString(16).toUpperCase().padStart(8, '0'),
      explanation: `ROTL32(${value}, ${shift}) & MASK(${maskBegin}, ${maskEnd}) = 0x${result.toString(16).toUpperCase()}`,
    };
  }

  // ==================== TIMING MATH ====================

  hzToMs(hz: number): {
    hz: number;
    periodMs: number;
    periodUs: number;
    periodNs: number;
    framesPerSecond: number;
  } {
    const periodMs = 1000 / hz;
    return {
      hz,
      periodMs: Math.round(periodMs * 1000) / 1000,
      periodUs: Math.round(periodMs * 1000),
      periodNs: Math.round(periodMs * 1000000),
      framesPerSecond: hz,
    };
  }

  fpsCalculator(input: number, inputType: 'fps' | 'ms' | 'us'): {
    fps: number;
    frameTimeMs: number;
    frameTimeUs: number;
    framesIn1Second: number;
    framesIn60Seconds: number;
  } {
    let fps: number;
    let frameTimeMs: number;
    
    switch (inputType) {
      case 'fps':
        fps = input;
        frameTimeMs = 1000 / input;
        break;
      case 'ms':
        frameTimeMs = input;
        fps = 1000 / input;
        break;
      case 'us':
        frameTimeMs = input / 1000;
        fps = 1000000 / input;
        break;
    }
    
    return {
      fps: Math.round(fps * 100) / 100,
      frameTimeMs: Math.round(frameTimeMs * 1000) / 1000,
      frameTimeUs: Math.round(frameTimeMs * 1000),
      framesIn1Second: Math.round(fps),
      framesIn60Seconds: Math.round(fps * 60),
    };
  }

  timingAnalysis(targetFps: number, actualFrameTimeMs: number): {
    targetFps: number;
    targetFrameTimeMs: number;
    actualFrameTimeMs: number;
    actualFps: number;
    difference: number;
    percentOfBudget: number;
    overUnder: string;
  } {
    const targetFrameTimeMs = 1000 / targetFps;
    const actualFps = 1000 / actualFrameTimeMs;
    const difference = actualFrameTimeMs - targetFrameTimeMs;
    const percentOfBudget = (actualFrameTimeMs / targetFrameTimeMs) * 100;
    
    return {
      targetFps,
      targetFrameTimeMs: Math.round(targetFrameTimeMs * 1000) / 1000,
      actualFrameTimeMs,
      actualFps: Math.round(actualFps * 100) / 100,
      difference: Math.round(difference * 1000) / 1000,
      percentOfBudget: Math.round(percentOfBudget * 10) / 10,
      overUnder: difference > 0 ? `${Math.round(difference * 1000) / 1000}ms OVER budget` : `${Math.round(-difference * 1000) / 1000}ms under budget`,
    };
  }

  // ==================== STRUCT/MEMORY LAYOUT ====================

  structSize(offsets: number[]): {
    offsets: number[];
    inferredSize: number;
    aligned4: number;
    aligned8: number;
    aligned16: number;
    offsetsHex: string[];
  } {
    const maxOffset = Math.max(...offsets);
    // Assume last field is at least 4 bytes
    const inferredSize = maxOffset + 4;
    
    return {
      offsets,
      inferredSize,
      aligned4: Math.ceil(inferredSize / 4) * 4,
      aligned8: Math.ceil(inferredSize / 8) * 8,
      aligned16: Math.ceil(inferredSize / 16) * 16,
      offsetsHex: offsets.map(o => '0x' + o.toString(16).toUpperCase()),
    };
  }

  alignAddress(address: string, alignment: number): {
    input: string;
    alignment: number;
    aligned: string;
    alignedHex: string;
    padding: number;
  } {
    const addr = BigInt(address.startsWith('0x') ? address : address);
    const alignBigInt = BigInt(alignment);
    const aligned = ((addr + alignBigInt - 1n) / alignBigInt) * alignBigInt;
    const padding = Number(aligned - addr);
    
    return {
      input: address,
      alignment,
      aligned: aligned.toString(),
      alignedHex: '0x' + aligned.toString(16).toUpperCase(),
      padding,
    };
  }

  pageAlign(size: string, pageSize: number = 4096): {
    input: string;
    pageSize: number;
    aligned: string;
    alignedHex: string;
    pages: number;
    wastedBytes: number;
  } {
    const sizeVal = BigInt(size.startsWith('0x') ? size : size);
    const pageSizeBigInt = BigInt(pageSize);
    const pages = Number((sizeVal + pageSizeBigInt - 1n) / pageSizeBigInt);
    const aligned = BigInt(pages) * pageSizeBigInt;
    const wasted = Number(aligned - sizeVal);
    
    return {
      input: size,
      pageSize,
      aligned: aligned.toString(),
      alignedHex: '0x' + aligned.toString(16).toUpperCase(),
      pages,
      wastedBytes: wasted,
    };
  }

  // ==================== GENERAL ARITHMETIC ====================

  calculate(expression: string): {
    expression: string;
    result: string;
    resultHex: string;
    resultBinary: string;
  } {
    // Safe expression evaluator for basic math
    // Supports: + - * / % ** << >> & | ^ ~
    // Also supports hex (0x) and binary (0b) literals
    
    // Replace hex literals with decimal
    let expr = expression.replace(/0x([0-9a-fA-F]+)/g, (_, hex) => BigInt('0x' + hex).toString());
    // Replace binary literals with decimal  
    expr = expr.replace(/0b([01]+)/g, (_, bin) => BigInt('0b' + bin).toString());
    
    // Only allow safe characters
    if (!/^[\d\s+\-*/%()<>&|^~n]+$/.test(expr)) {
      throw new Error('Invalid characters in expression');
    }
    
    // Add 'n' suffix for BigInt operations
    expr = expr.replace(/(\d+)/g, '$1n');
    
    // Evaluate using Function constructor (safer than eval, but still be careful)
    const fn = new Function(`return (${expr})`);
    const result = fn();
    
    const resultBigInt = BigInt(result);
    const absResult = resultBigInt < 0n ? -resultBigInt : resultBigInt;
    
    return {
      expression,
      result: resultBigInt.toString(),
      resultHex: (resultBigInt < 0n ? '-' : '') + '0x' + absResult.toString(16).toUpperCase(),
      resultBinary: (resultBigInt < 0n ? '-' : '') + '0b' + absResult.toString(2),
    };
  }

  baseConvert(value: string, fromBase: number, toBase: number): {
    input: string;
    fromBase: number;
    toBase: number;
    result: string;
    decimal: string;
  } {
    // Handle prefixes
    let cleanValue = value;
    if (fromBase === 16 && value.toLowerCase().startsWith('0x')) {
      cleanValue = value.slice(2);
    } else if (fromBase === 2 && value.toLowerCase().startsWith('0b')) {
      cleanValue = value.slice(2);
    }
    
    const decimal = BigInt(parseInt(cleanValue, fromBase));
    let result: string;
    
    if (toBase === 16) {
      result = '0x' + decimal.toString(16).toUpperCase();
    } else if (toBase === 2) {
      result = '0b' + decimal.toString(2);
    } else if (toBase === 8) {
      result = '0o' + decimal.toString(8);
    } else {
      result = decimal.toString(toBase);
    }
    
    return {
      input: value,
      fromBase,
      toBase,
      result,
      decimal: decimal.toString(),
    };
  }

  // ==================== MEMORY REGION ANALYSIS ====================

  ppcMemoryMap(address: string): {
    address: string;
    addressHex: string;
    region: string;
    description: string;
    size: string;
    usage: string;
  } {
    const addr = BigInt(address.startsWith('0x') ? address : '0x' + address);
    const addrNum = Number(addr & 0xFFFFFFFFn);
    
    const regions = [
      { start: 0x82000000, end: 0x82020000, name: 'Stream Pool', size: '128 KB', usage: 'Streaming system object allocation, zeroed on init' },
      { start: 0x82020000, end: 0x82120000, name: 'XEX Data Region', size: '1 MB', usage: 'Initialization tables, XEX metadata, loader data' },
      { start: 0x82120000, end: 0x82A00000, name: 'Game Code', size: '~9 MB', usage: 'Recompiled PPC functions (sub_82XXXXXX)' },
      { start: 0x82A00000, end: 0x82A90000, name: 'Import Region', size: '576 KB', usage: 'System-managed imports (__imp__ functions)' },
      { start: 0x82A90000, end: 0x82AA0000, name: 'Kernel Runtime', size: '64 KB', usage: 'TLS indices, thread pool, callback lists, kernel structures' },
      { start: 0x82AA0000, end: 0x83000000, name: 'Extended Code', size: '~5.4 MB', usage: 'Additional game code and data' },
      { start: 0x83000000, end: 0x831F0000, name: 'Static Data (BSS)', size: '1.99 MB', usage: 'Global variables and static data, zeroed per C/C++ BSS contract' },
      { start: 0x831F0000, end: 0x83200000, name: 'Function Table', size: '64 KB', usage: 'Protected function table region' },
      { start: 0x83200000, end: 0x84000000, name: 'Heap Region', size: '14 MB', usage: 'Dynamic allocations, game heap' },
    ];
    
    for (const region of regions) {
      if (addrNum >= region.start && addrNum < region.end) {
        return {
          address: addrNum.toString(),
          addressHex: '0x' + addrNum.toString(16).toUpperCase().padStart(8, '0'),
          region: region.name,
          description: region.name + ' (0x' + region.start.toString(16).toUpperCase() + '-0x' + region.end.toString(16).toUpperCase() + ')',
          size: region.size,
          usage: region.usage,
        };
      }
    }
    
    if (addrNum < 0x82000000) {
      return {
        address: addrNum.toString(),
        addressHex: '0x' + addrNum.toString(16).toUpperCase().padStart(8, '0'),
        region: 'Low Memory',
        description: 'Below PPC base',
        size: 'N/A',
        usage: 'Invalid for PPC code - may be host memory or NULL region',
      };
    }
    
    return {
      address: addrNum.toString(),
      addressHex: '0x' + addrNum.toString(16).toUpperCase().padStart(8, '0'),
      region: 'Unknown/High Memory',
      description: 'Above mapped regions',
      size: 'N/A',
      usage: 'May be dynamic allocation or unmapped',
    };
  }

  roundToPage(size: string, pageSize: number = 4096): {
    input: string;
    inputHex: string;
    pageSize: number;
    pageSizeHex: string;
    rounded: string;
    roundedHex: string;
    paddingAdded: number;
    formula: string;
  } {
    const sizeVal = BigInt(size.startsWith('0x') ? size : size);
    const pageSizeBigInt = BigInt(pageSize);
    const rounded = (sizeVal + (pageSizeBigInt - 1n)) & ~(pageSizeBigInt - 1n);
    const padding = Number(rounded - sizeVal);
    
    return {
      input: sizeVal.toString(),
      inputHex: '0x' + sizeVal.toString(16).toUpperCase(),
      pageSize,
      pageSizeHex: '0x' + pageSize.toString(16).toUpperCase(),
      rounded: rounded.toString(),
      roundedHex: '0x' + rounded.toString(16).toUpperCase(),
      paddingAdded: padding,
      formula: '(' + sizeVal + ' + ' + (pageSize - 1) + ') & ~' + (pageSize - 1) + ' = ' + rounded,
    };
  }

  isValidPpcAddress(address: string): {
    address: string;
    addressHex: string;
    isValid: boolean;
    isCodeRegion: boolean;
    isDataRegion: boolean;
    isImportRegion: boolean;
    reason: string;
  } {
    const addr = BigInt(address.startsWith('0x') ? address : '0x' + address);
    const addrNum = Number(addr & 0xFFFFFFFFn);
    
    const PPC_MIN = 0x82000000;
    const PPC_MAX = 0x84000000;
    const CODE_START = 0x82120000;
    const CODE_END = 0x82A00000;
    const IMPORT_START = 0x82A00000;
    const IMPORT_END = 0x82B00000;
    const DATA_START = 0x83000000;
    const DATA_END = 0x83200000;
    
    const isValid = addrNum >= PPC_MIN && addrNum < PPC_MAX;
    const isCodeRegion = addrNum >= CODE_START && addrNum < CODE_END;
    const isImportRegion = addrNum >= IMPORT_START && addrNum < IMPORT_END;
    const isDataRegion = addrNum >= DATA_START && addrNum < DATA_END;
    
    let reason: string;
    if (!isValid) {
      reason = addrNum < PPC_MIN ? 'Below PPC base (0x82000000)' : 'Above PPC range (0x84000000)';
    } else if (isCodeRegion) {
      reason = 'Valid game code region (sub_82XXXXXX functions)';
    } else if (isImportRegion) {
      reason = 'Valid import region (__imp__ functions)';
    } else if (isDataRegion) {
      reason = 'Valid static data/BSS region';
    } else {
      reason = 'Valid PPC address (heap, stream pool, or other)';
    }
    
    return {
      address: addrNum.toString(),
      addressHex: '0x' + addrNum.toString(16).toUpperCase().padStart(8, '0'),
      isValid,
      isCodeRegion,
      isDataRegion,
      isImportRegion,
      reason,
    };
  }

  // ==================== XBOX 360 PERFORMANCE COUNTER ====================

  perfTicksToMs(ticks: string): {
    ticks: string;
    milliseconds: number;
    seconds: number;
    frequency: number;
    formula: string;
  } {
    const PERF_FREQ = 49875000;
    const ticksVal = BigInt(ticks.startsWith('0x') ? ticks : ticks);
    const ms = Number(ticksVal) / PERF_FREQ * 1000;
    const seconds = Number(ticksVal) / PERF_FREQ;
    
    return {
      ticks: ticksVal.toString(),
      milliseconds: Math.round(ms * 1000) / 1000,
      seconds: Math.round(seconds * 1000000) / 1000000,
      frequency: PERF_FREQ,
      formula: ticksVal + ' ticks / ' + PERF_FREQ + ' Hz * 1000 = ' + ms.toFixed(3) + ' ms',
    };
  }

  msToPerfTicks(ms: number): {
    milliseconds: number;
    ticks: string;
    ticksHex: string;
    frequency: number;
    formula: string;
  } {
    const PERF_FREQ = 49875000;
    const ticks = BigInt(Math.round(ms * PERF_FREQ / 1000));
    
    return {
      milliseconds: ms,
      ticks: ticks.toString(),
      ticksHex: '0x' + ticks.toString(16).toUpperCase(),
      frequency: PERF_FREQ,
      formula: ms + ' ms * ' + PERF_FREQ + ' Hz / 1000 = ' + ticks + ' ticks',
    };
  }

  timebaseToSeconds(timebase: string): {
    timebase: string;
    seconds: number;
    milliseconds: number;
    microseconds: number;
    frequency: number;
    note: string;
  } {
    const TIMEBASE_FREQ = 49875000;
    const tbVal = BigInt(timebase.startsWith('0x') ? timebase : timebase);
    const seconds = Number(tbVal) / TIMEBASE_FREQ;
    
    return {
      timebase: tbVal.toString(),
      seconds: Math.round(seconds * 1000000) / 1000000,
      milliseconds: Math.round(seconds * 1000 * 1000) / 1000,
      microseconds: Math.round(seconds * 1000000),
      frequency: TIMEBASE_FREQ,
      note: 'Xbox 360 timebase from mftb instruction, 49.875 MHz',
    };
  }

  // ==================== BYTE SWAPPING / ENDIANNESS ====================

  byteSwap16(value: string): {
    input: string;
    inputHex: string;
    swapped: string;
    swappedHex: string;
    inputBytes: string;
    swappedBytes: string;
  } {
    const val = Number(BigInt(value.startsWith('0x') ? value : value) & 0xFFFFn);
    const swapped = ((val & 0xFF) << 8) | ((val >> 8) & 0xFF);
    
    return {
      input: val.toString(),
      inputHex: '0x' + val.toString(16).toUpperCase().padStart(4, '0'),
      swapped: swapped.toString(),
      swappedHex: '0x' + swapped.toString(16).toUpperCase().padStart(4, '0'),
      inputBytes: '[' + ((val >> 8) & 0xFF).toString(16).padStart(2, '0') + ', ' + (val & 0xFF).toString(16).padStart(2, '0') + ']',
      swappedBytes: '[' + ((swapped >> 8) & 0xFF).toString(16).padStart(2, '0') + ', ' + (swapped & 0xFF).toString(16).padStart(2, '0') + ']',
    };
  }

  byteSwap32(value: string): {
    input: string;
    inputHex: string;
    swapped: string;
    swappedHex: string;
    inputBytes: string;
    swappedBytes: string;
  } {
    const val = Number(BigInt(value.startsWith('0x') ? value : value) & 0xFFFFFFFFn);
    const swapped = 
      ((val & 0xFF) << 24) |
      ((val & 0xFF00) << 8) |
      ((val >> 8) & 0xFF00) |
      ((val >> 24) & 0xFF);
    
    const getBytes = (v: number) => [
      ((v >> 24) & 0xFF).toString(16).padStart(2, '0'),
      ((v >> 16) & 0xFF).toString(16).padStart(2, '0'),
      ((v >> 8) & 0xFF).toString(16).padStart(2, '0'),
      (v & 0xFF).toString(16).padStart(2, '0'),
    ];
    
    return {
      input: val.toString(),
      inputHex: '0x' + val.toString(16).toUpperCase().padStart(8, '0'),
      swapped: (swapped >>> 0).toString(),
      swappedHex: '0x' + (swapped >>> 0).toString(16).toUpperCase().padStart(8, '0'),
      inputBytes: '[' + getBytes(val).join(', ') + ']',
      swappedBytes: '[' + getBytes(swapped >>> 0).join(', ') + ']',
    };
  }

  byteSwap64(value: string): {
    input: string;
    inputHex: string;
    swapped: string;
    swappedHex: string;
  } {
    const val = BigInt(value.startsWith('0x') ? value : value) & 0xFFFFFFFFFFFFFFFFn;
    
    const swapped = 
      ((val & 0xFFn) << 56n) |
      ((val & 0xFF00n) << 40n) |
      ((val & 0xFF0000n) << 24n) |
      ((val & 0xFF000000n) << 8n) |
      ((val >> 8n) & 0xFF000000n) |
      ((val >> 24n) & 0xFF0000n) |
      ((val >> 40n) & 0xFF00n) |
      ((val >> 56n) & 0xFFn);
    
    return {
      input: val.toString(),
      inputHex: '0x' + val.toString(16).toUpperCase().padStart(16, '0'),
      swapped: swapped.toString(),
      swappedHex: '0x' + swapped.toString(16).toUpperCase().padStart(16, '0'),
    };
  }

  byteSwapFloat(value: string): {
    input: string;
    inputHex: string;
    swappedHex: string;
    inputFloat: number;
    swappedFloat: number;
    note: string;
  } {
    const val = Number(BigInt(value.startsWith('0x') ? value : '0x' + value) & 0xFFFFFFFFn);
    
    const swapped = 
      ((val & 0xFF) << 24) |
      ((val & 0xFF00) << 8) |
      ((val >> 8) & 0xFF00) |
      ((val >> 24) & 0xFF);
    
    const buf = new ArrayBuffer(4);
    const view = new DataView(buf);
    view.setUint32(0, val, false);
    const inputFloat = view.getFloat32(0, false);
    view.setUint32(0, swapped >>> 0, false);
    const swappedFloat = view.getFloat32(0, false);
    
    return {
      input: val.toString(),
      inputHex: '0x' + val.toString(16).toUpperCase().padStart(8, '0'),
      swappedHex: '0x' + (swapped >>> 0).toString(16).toUpperCase().padStart(8, '0'),
      inputFloat: inputFloat,
      swappedFloat: swappedFloat,
      note: 'Swaps bytes of IEEE 754 single-precision float',
    };
  }

  // ==================== NTSTATUS / ERROR CODE ANALYSIS ====================

  ntstatusDecode(status: string): {
    status: string;
    statusHex: string;
    name: string;
    severity: string;
    isError: boolean;
    isWarning: boolean;
    isSuccess: boolean;
    facility: number;
    code: number;
    description: string;
  } {
    const val = Number(BigInt(status.startsWith('0x') ? status : '0x' + status) & 0xFFFFFFFFn);
    
    const knownCodes: { [key: number]: { name: string; description: string } } = {
      0x00000000: { name: 'STATUS_SUCCESS', description: 'Operation completed successfully' },
      0x00000102: { name: 'STATUS_TIMEOUT', description: 'Wait timed out' },
      0x00000103: { name: 'STATUS_PENDING', description: 'Operation is pending' },
      0x80000005: { name: 'STATUS_BUFFER_OVERFLOW', description: 'Buffer too small, partial data returned' },
      0x80000006: { name: 'STATUS_NO_MORE_FILES', description: 'No more files in directory enumeration' },
      0xC0000001: { name: 'STATUS_UNSUCCESSFUL', description: 'Generic failure' },
      0xC0000002: { name: 'STATUS_NOT_IMPLEMENTED', description: 'Not implemented' },
      0xC0000004: { name: 'STATUS_INFO_LENGTH_MISMATCH', description: 'Information length mismatch' },
      0xC0000005: { name: 'STATUS_ACCESS_VIOLATION', description: 'Access violation' },
      0xC0000008: { name: 'STATUS_INVALID_HANDLE', description: 'Invalid handle' },
      0xC000000D: { name: 'STATUS_INVALID_PARAMETER', description: 'Invalid parameter' },
      0xC0000011: { name: 'STATUS_END_OF_FILE', description: 'End of file reached' },
      0xC0000017: { name: 'STATUS_NO_MEMORY', description: 'Insufficient memory' },
      0xC0000022: { name: 'STATUS_ACCESS_DENIED', description: 'Access denied' },
      0xC0000033: { name: 'STATUS_OBJECT_NAME_INVALID', description: 'Object name is invalid' },
      0xC0000034: { name: 'STATUS_OBJECT_NAME_NOT_FOUND', description: 'Object name not found' },
      0xC0000035: { name: 'STATUS_OBJECT_NAME_COLLISION', description: 'Object name already exists' },
      0xC000003A: { name: 'STATUS_OBJECT_PATH_NOT_FOUND', description: 'Object path not found' },
      0xC000003B: { name: 'STATUS_OBJECT_PATH_SYNTAX_BAD', description: 'Bad object path syntax' },
      0xC0000043: { name: 'STATUS_SHARING_VIOLATION', description: 'Sharing violation' },
      0xC00000BA: { name: 'STATUS_FILE_IS_A_DIRECTORY', description: 'File is a directory' },
      0xC0000120: { name: 'STATUS_CANCELLED', description: 'Operation was cancelled' },
      0xC000013E: { name: 'STATUS_FAIL_CHECK', description: 'Fail check' },
    };
    
    const severity = (val >> 30) & 0x3;
    const isError = severity === 3;
    const isWarning = severity === 2;
    const isSuccess = severity === 0;
    const facility = (val >> 16) & 0xFFF;
    const code = val & 0xFFFF;
    
    const known = knownCodes[val];
    const severityStr = isError ? 'Error' : isWarning ? 'Warning' : isSuccess ? 'Success' : 'Informational';
    
    return {
      status: val.toString(),
      statusHex: '0x' + val.toString(16).toUpperCase().padStart(8, '0'),
      name: known?.name || 'UNKNOWN_STATUS_' + val.toString(16).toUpperCase(),
      severity: severityStr,
      isError,
      isWarning,
      isSuccess,
      facility,
      code,
      description: known?.description || 'Unknown status code. Severity=' + severityStr + ', Facility=0x' + facility.toString(16) + ', Code=0x' + code.toString(16),
    };
  }

  ntstatusIsError(status: string): {
    status: string;
    statusHex: string;
    isError: boolean;
    check: string;
  } {
    const val = Number(BigInt(status.startsWith('0x') ? status : '0x' + status) & 0xFFFFFFFFn);
    const isError = (val & 0xC0000000) === 0xC0000000;
    
    return {
      status: val.toString(),
      statusHex: '0x' + val.toString(16).toUpperCase().padStart(8, '0'),
      isError,
      check: '(0x' + val.toString(16).toUpperCase() + ' & 0xC0000000) === 0xC0000000 -> ' + isError,
    };
  }

  ntstatusIsWarning(status: string): {
    status: string;
    statusHex: string;
    isWarning: boolean;
    check: string;
  } {
    const val = Number(BigInt(status.startsWith('0x') ? status : '0x' + status) & 0xFFFFFFFFn);
    const isWarning = (val & 0x80000000) !== 0 && (val & 0xC0000000) !== 0xC0000000;
    
    return {
      status: val.toString(),
      statusHex: '0x' + val.toString(16).toUpperCase().padStart(8, '0'),
      isWarning,
      check: '(0x' + val.toString(16).toUpperCase() + ' & 0x80000000) && !(& 0xC0000000 === 0xC0000000) -> ' + isWarning,
    };
  }

  // ==================== ALLOCATION / POOL MATH ====================

  allocationUnits(bytes: string, sectorsPerUnit: number = 8, bytesPerSector: number = 512): {
    bytes: string;
    bytesHex: string;
    sectorsPerUnit: number;
    bytesPerSector: number;
    bytesPerUnit: number;
    allocationUnits: string;
    formula: string;
  } {
    const bytesVal = BigInt(bytes.startsWith('0x') ? bytes : bytes);
    const bytesPerUnit = sectorsPerUnit * bytesPerSector;
    const units = (bytesVal + BigInt(bytesPerUnit - 1)) / BigInt(bytesPerUnit);
    
    return {
      bytes: bytesVal.toString(),
      bytesHex: '0x' + bytesVal.toString(16).toUpperCase(),
      sectorsPerUnit,
      bytesPerSector,
      bytesPerUnit,
      allocationUnits: units.toString(),
      formula: 'ceil(' + bytesVal + ' / (' + sectorsPerUnit + ' * ' + bytesPerSector + ')) = ' + units + ' allocation units',
    };
  }

  sectorsToBytes(sectors: string, bytesPerSector: number = 512): {
    sectors: string;
    bytesPerSector: number;
    bytes: string;
    bytesHex: string;
    kilobytes: number;
    megabytes: number;
    formula: string;
  } {
    const sectorsVal = BigInt(sectors.startsWith('0x') ? sectors : sectors);
    const bytes = sectorsVal * BigInt(bytesPerSector);
    
    return {
      sectors: sectorsVal.toString(),
      bytesPerSector,
      bytes: bytes.toString(),
      bytesHex: '0x' + bytes.toString(16).toUpperCase(),
      kilobytes: Number(bytes) / 1024,
      megabytes: Number(bytes) / (1024 * 1024),
      formula: sectorsVal + ' sectors * ' + bytesPerSector + ' bytes/sector = ' + bytes + ' bytes',
    };
  }
}
