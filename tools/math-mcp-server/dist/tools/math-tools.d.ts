export declare class MathTools {
    hexToDec(hex: string, signed?: boolean, bits?: number): {
        decimal: string;
        unsigned: string;
        signed: string;
        hex: string;
    };
    decToHex(decimal: string, bits?: number): {
        hex: string;
        unsigned: string;
        signed: string;
    };
    addOffset(base: string, offset: string): {
        result: string;
        resultHex: string;
        base: string;
        offset: string;
        calculation: string;
    };
    addressRange(start: string, size: string): {
        start: string;
        end: string;
        size: string;
        startHex: string;
        endHex: string;
        sizeHex: string;
    };
    lisCalculation(immediate: string): {
        immediate: number;
        result: string;
        resultHex: string;
        signedResult: string;
        explanation: string;
    };
    effectiveAddress(lisValue: string, offset: string): {
        lisValue: string;
        offset: string;
        effectiveAddress: string;
        effectiveAddressHex: string;
        breakdown: string;
    };
    ppcOffsetDecode(value: string): {
        original: string;
        as16BitSigned: number;
        as32BitSigned: number;
        hex: string;
        interpretation: string;
    };
    twosComplement(value: string, bits?: number): {
        input: string;
        bits: number;
        unsigned: string;
        signed: string;
        hex: string;
        binary: string;
    };
    signExtend(value: string, fromBits: number, toBits: number): {
        input: string;
        fromBits: number;
        toBits: number;
        result: string;
        resultHex: string;
        resultSigned: string;
    };
    bitMask(startBit: number, endBit: number, bits?: number): {
        startBit: number;
        endBit: number;
        mask: string;
        maskHex: string;
        maskBinary: string;
    };
    bitShift(value: string, amount: number, direction: 'left' | 'right', logical?: boolean, bits?: number): {
        input: string;
        amount: number;
        direction: string;
        type: string;
        result: string;
        resultHex: string;
    };
    bitExtract(value: string, startBit: number, numBits: number): {
        input: string;
        startBit: number;
        numBits: number;
        extracted: string;
        extractedHex: string;
        extractedBinary: string;
    };
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
    };
    hzToMs(hz: number): {
        hz: number;
        periodMs: number;
        periodUs: number;
        periodNs: number;
        framesPerSecond: number;
    };
    fpsCalculator(input: number, inputType: 'fps' | 'ms' | 'us'): {
        fps: number;
        frameTimeMs: number;
        frameTimeUs: number;
        framesIn1Second: number;
        framesIn60Seconds: number;
    };
    timingAnalysis(targetFps: number, actualFrameTimeMs: number): {
        targetFps: number;
        targetFrameTimeMs: number;
        actualFrameTimeMs: number;
        actualFps: number;
        difference: number;
        percentOfBudget: number;
        overUnder: string;
    };
    structSize(offsets: number[]): {
        offsets: number[];
        inferredSize: number;
        aligned4: number;
        aligned8: number;
        aligned16: number;
        offsetsHex: string[];
    };
    alignAddress(address: string, alignment: number): {
        input: string;
        alignment: number;
        aligned: string;
        alignedHex: string;
        padding: number;
    };
    pageAlign(size: string, pageSize?: number): {
        input: string;
        pageSize: number;
        aligned: string;
        alignedHex: string;
        pages: number;
        wastedBytes: number;
    };
    calculate(expression: string): {
        expression: string;
        result: string;
        resultHex: string;
        resultBinary: string;
    };
    baseConvert(value: string, fromBase: number, toBase: number): {
        input: string;
        fromBase: number;
        toBase: number;
        result: string;
        decimal: string;
    };
    ppcMemoryMap(address: string): {
        address: string;
        addressHex: string;
        region: string;
        description: string;
        size: string;
        usage: string;
    };
    roundToPage(size: string, pageSize?: number): {
        input: string;
        inputHex: string;
        pageSize: number;
        pageSizeHex: string;
        rounded: string;
        roundedHex: string;
        paddingAdded: number;
        formula: string;
    };
    isValidPpcAddress(address: string): {
        address: string;
        addressHex: string;
        isValid: boolean;
        isCodeRegion: boolean;
        isDataRegion: boolean;
        isImportRegion: boolean;
        reason: string;
    };
    perfTicksToMs(ticks: string): {
        ticks: string;
        milliseconds: number;
        seconds: number;
        frequency: number;
        formula: string;
    };
    msToPerfTicks(ms: number): {
        milliseconds: number;
        ticks: string;
        ticksHex: string;
        frequency: number;
        formula: string;
    };
    timebaseToSeconds(timebase: string): {
        timebase: string;
        seconds: number;
        milliseconds: number;
        microseconds: number;
        frequency: number;
        note: string;
    };
    byteSwap16(value: string): {
        input: string;
        inputHex: string;
        swapped: string;
        swappedHex: string;
        inputBytes: string;
        swappedBytes: string;
    };
    byteSwap32(value: string): {
        input: string;
        inputHex: string;
        swapped: string;
        swappedHex: string;
        inputBytes: string;
        swappedBytes: string;
    };
    byteSwap64(value: string): {
        input: string;
        inputHex: string;
        swapped: string;
        swappedHex: string;
    };
    byteSwapFloat(value: string): {
        input: string;
        inputHex: string;
        swappedHex: string;
        inputFloat: number;
        swappedFloat: number;
        note: string;
    };
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
    };
    ntstatusIsError(status: string): {
        status: string;
        statusHex: string;
        isError: boolean;
        check: string;
    };
    ntstatusIsWarning(status: string): {
        status: string;
        statusHex: string;
        isWarning: boolean;
        check: string;
    };
    allocationUnits(bytes: string, sectorsPerUnit?: number, bytesPerSector?: number): {
        bytes: string;
        bytesHex: string;
        sectorsPerUnit: number;
        bytesPerSector: number;
        bytesPerUnit: number;
        allocationUnits: string;
        formula: string;
    };
    sectorsToBytes(sectors: string, bytesPerSector?: number): {
        sectors: string;
        bytesPerSector: number;
        bytes: string;
        bytesHex: string;
        kilobytes: number;
        megabytes: number;
        formula: string;
    };
}
