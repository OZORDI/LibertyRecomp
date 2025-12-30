import * as path from 'path';
// Render path functions from RENDERING_RESEARCH.md
const RENDER_PATH = {
    mainLoopEntry: 'sub_82856F08',
    orchestrator: 'sub_828529B0',
    framePresent: 'sub_828507F8',
    d3dPresent: 'sub_829D5388',
    vdSwap: '__imp__VdSwap',
};
// Key GPU/D3D functions from video.cpp hooks
const GPU_FUNCTIONS = {
    // Draw calls
    'sub_829D8860': { category: 'draw', description: 'DrawPrimitive', params: 'primType, startVert, primCount' },
    'sub_829D4EE0': { category: 'draw', description: 'DrawIndexedPrimitive (UnifiedDraw)', params: 'primType, baseVert, minIdx, numVert, startIdx, primCount' },
    'sub_826FEC28': { category: 'draw', description: 'DrawPrimitive (alt)', params: 'primType, startVert, primCount' },
    'sub_826FF030': { category: 'draw', description: 'DrawIndexedPrimitive (alt)', params: 'see above' },
    'sub_826FE5C0': { category: 'draw', description: 'DrawPrimitiveUP', params: 'primType, primCount, vertData, vertStride' },
    // Resource binding
    'sub_829C96D0': { category: 'bind', description: 'SetIndices', params: 'device+13580, indexBuffer' },
    'sub_829C9070': { category: 'bind', description: 'SetStreamSource', params: 'device+12020, stream, buffer, offset, stride' },
    'sub_829D3728': { category: 'bind', description: 'SetTexture', params: 'stage, texture (called ~20x/frame)' },
    'sub_829C9440': { category: 'bind', description: 'SetVertexDeclaration', params: 'device+10456' },
    // Shader binding
    'sub_829CD350': { category: 'shader', description: 'SetVertexShader (also sets PS)', params: 'device+10932, device+10936' },
    'sub_82546EE0': { category: 'shader', description: 'SetVertexShader', params: 'shader' },
    'sub_82546BD8': { category: 'shader', description: 'SetPixelShader', params: 'shader' },
    // Resource creation
    'sub_829D3400': { category: 'create', description: 'CreateTexture', params: 'device, width, height, levels, usage, format, pool' },
    'sub_829D3520': { category: 'create', description: 'CreateVertexBuffer', params: 'device, length, usage, pool' },
    'sub_829D3648': { category: 'create', description: 'GetSurfaceDesc', params: 'surface, desc' },
    // Texture operations
    'sub_829D6560': { category: 'texture', description: 'LockTextureRect', params: 'texture, level, rect, flags' },
    'sub_829D6690': { category: 'texture', description: 'UnlockTextureRect', params: 'texture, level' },
    // Buffer operations
    'sub_829D6830': { category: 'buffer', description: 'LockVertexBuffer', params: 'buffer, offset, size, flags' },
    'sub_829D69D8': { category: 'buffer', description: 'UnlockVertexBuffer', params: 'buffer' },
    // Render targets
    'sub_82543EE0': { category: 'rendertarget', description: 'SetRenderTarget', params: 'index, surface' },
    'sub_82544210': { category: 'rendertarget', description: 'SetDepthStencilSurface', params: 'surface' },
    'sub_82543B58': { category: 'rendertarget', description: 'GetBackBuffer', params: '' },
    'sub_82543BA0': { category: 'rendertarget', description: 'GetDepthStencil', params: '' },
    // Presentation
    'sub_829D5388': { category: 'present', description: 'D3D Present (calls VdSwap)', params: '' },
    '__imp__VdSwap': { category: 'present', description: 'VdSwap - Frame presentation', params: '' },
    // State
    'sub_82555B30': { category: 'state', description: 'Clear', params: 'flags, color, z, stencil' },
    'sub_825436F0': { category: 'state', description: 'SetViewport', params: 'viewport' },
    'sub_82543628': { category: 'state', description: 'SetScissorRect', params: 'rect' },
    // GPU memory
    'sub_829DFAD8': { category: 'memory', description: 'GPU Memory Allocator', params: 'size, alignment' },
};
// Init chain functions
const INIT_CHAIN = {
    gameMainEntry: 'sub_8218BEA8',
    frameTick: 'sub_827D89B8',
    initWrapper: 'sub_8218BEB0',
    gameInit: 'sub_82120000',
    coreInit: 'sub_8218C600',
    renderSetup: 'sub_82856C90',
    gpuStateSetup: 'sub_82857240',
    subsystemInit: 'sub_82120FB8',
};
const BUILD_PRESETS = {
    'windows-x64-debug': { preset: 'x64-Clang-Debug', buildDir: 'out/build/x64-Clang-Debug' },
    'windows-x64-relwithdebinfo': { preset: 'x64-Clang-RelWithDebInfo', buildDir: 'out/build/x64-Clang-RelWithDebInfo' },
    'windows-x64-release': { preset: 'x64-Clang-Release', buildDir: 'out/build/x64-Clang-Release' },
    'windows-arm64-debug': { preset: 'arm64-Clang-Debug', buildDir: 'out/build/arm64-Clang-Debug' },
    'windows-arm64-relwithdebinfo': { preset: 'arm64-Clang-RelWithDebInfo', buildDir: 'out/build/arm64-Clang-RelWithDebInfo' },
    'windows-arm64-release': { preset: 'arm64-Clang-Release', buildDir: 'out/build/arm64-Clang-Release' },
    'linux-x64-debug': { preset: 'linux-debug', buildDir: 'out/build/linux-debug' },
    'linux-x64-relwithdebinfo': { preset: 'linux-relwithdebinfo', buildDir: 'out/build/linux-relwithdebinfo' },
    'linux-x64-release': { preset: 'linux-release', buildDir: 'out/build/linux-release' },
    'linux-arm64-debug': { preset: 'linux-debug', buildDir: 'out/build/linux-debug' },
    'linux-arm64-relwithdebinfo': { preset: 'linux-relwithdebinfo', buildDir: 'out/build/linux-relwithdebinfo' },
    'linux-arm64-release': { preset: 'linux-release', buildDir: 'out/build/linux-release' },
    'macos-arm64-debug': { preset: 'macos-debug', buildDir: 'out/build/macos-debug' },
    'macos-arm64-relwithdebinfo': { preset: 'macos-relwithdebinfo', buildDir: 'out/build/macos-relwithdebinfo' },
    'macos-arm64-release': { preset: 'macos-release', buildDir: 'out/build/macos-release' },
    'macos-x64-debug': { preset: 'macos-debug', buildDir: 'out/build/macos-debug' },
    'macos-x64-relwithdebinfo': { preset: 'macos-relwithdebinfo', buildDir: 'out/build/macos-relwithdebinfo' },
    'macos-x64-release': { preset: 'macos-release', buildDir: 'out/build/macos-release' },
};
export class RenderingTools {
    index;
    ppcDir;
    projectRoot;
    constructor(index, ppcDir) {
        this.index = index;
        this.ppcDir = ppcDir;
        this.projectRoot = path.resolve(ppcDir, '../..');
    }
    // ========== RENDER PATH TRACER ==========
    traceRenderPath() {
        const renderPath = [];
        // Main render path
        const pathFuncs = [
            { name: RENDER_PATH.mainLoopEntry, desc: 'Main Loop Entry' },
            { name: RENDER_PATH.orchestrator, desc: 'Orchestrator' },
            { name: RENDER_PATH.framePresent, desc: 'Frame Present' },
            { name: RENDER_PATH.d3dPresent, desc: 'D3D Present' },
            { name: RENDER_PATH.vdSwap, desc: 'VdSwap (Frame Presentation)' },
        ];
        for (const pf of pathFuncs) {
            const func = this.index.functionsByName.get(pf.name);
            const hook = this.index.hooks.get(pf.name);
            renderPath.push({
                function: pf.name,
                description: pf.desc,
                file: func?.file,
                hooked: !!hook,
            });
        }
        // Init chain
        const initChain = [
            { function: INIT_CHAIN.gameMainEntry, description: 'Game Main Entry' },
            { function: INIT_CHAIN.frameTick, description: 'Frame Tick' },
            { function: INIT_CHAIN.initWrapper, description: 'Init Wrapper' },
            { function: INIT_CHAIN.gameInit, description: 'Game Init' },
            { function: INIT_CHAIN.coreInit, description: 'Core Init', blocking: 'vtable[1] call to sub_82857240' },
            { function: INIT_CHAIN.renderSetup, description: 'Render Setup' },
            { function: INIT_CHAIN.gpuStateSetup, description: 'GPU State Setup (BLOCKING)', blocking: 'Sync primitive waits' },
            { function: INIT_CHAIN.subsystemInit, description: '63-Subsystem Init' },
        ];
        // GPU functions usage
        const gpuFunctions = [];
        for (const [funcName, info] of Object.entries(GPU_FUNCTIONS)) {
            const func = this.index.functionsByName.get(funcName);
            gpuFunctions.push({
                function: funcName,
                category: info.category,
                description: info.description,
                calledBy: func?.calledBy.slice(0, 10) || [],
            });
        }
        return { renderPath, initChain, gpuFunctions };
    }
    // ========== VDSWAP ANALYZER ==========
    analyzeVdSwap() {
        const vdSwapFunc = this.index.functionsByName.get('__imp__VdSwap');
        const hook = this.index.hooks.get('VdSwap') || this.index.hooks.get('__imp__VdSwap');
        // Frame flow based on VdSwap implementation in imports.cpp
        const frameFlow = [
            { step: '1. Frame count increment', description: 'Track frame number for logging' },
            { step: '2. Video::Present()', description: 'Call host present to swap buffers' },
            { step: '3. Signal tracked events', description: 'KeSetEvent on all tracked event handles to unblock workers' },
            { step: '4. Signal worker semaphores', description: 'KeReleaseSemaphore on worker semaphores' },
            { step: '5. Update fence value', description: 'Increment GPU fence for sync' },
        ];
        const knownIssues = [
            'Workers may block on KeWaitForSingleObject if events not tracked',
            'Frame timing depends on VBlank simulation (16.67ms for 60Hz)',
            'Semaphore signaling must match what workers expect',
            'GPU command buffer sync requires fence updates',
        ];
        return {
            vdSwapFunction: '__imp__VdSwap',
            hookStatus: {
                isHooked: !!hook,
                hookType: hook?.type,
            },
            callers: vdSwapFunc?.calledBy || ['sub_829D5388 (D3D Present)'],
            events: ['g_trackedEventHandles (dynamic)', 'Worker semaphores (per-thread)'],
            frameFlow,
            knownIssues,
        };
    }
    // ========== GPU COMMAND INSPECTOR ==========
    inspectGpuCommands(category) {
        const categories = [...new Set(Object.values(GPU_FUNCTIONS).map(f => f.category))];
        const commands = [];
        const drawCallFunctions = [];
        const resourceFunctions = [];
        const stateFunctions = [];
        for (const [funcName, info] of Object.entries(GPU_FUNCTIONS)) {
            if (category && info.category !== category)
                continue;
            const hook = this.index.hooks.get(funcName);
            commands.push({
                function: funcName,
                category: info.category,
                description: info.description,
                params: info.params,
                hookStatus: hook ? `Hooked (${hook.type})` : 'Not hooked',
            });
            if (info.category === 'draw')
                drawCallFunctions.push(funcName);
            if (info.category === 'create' || info.category === 'texture' || info.category === 'buffer') {
                resourceFunctions.push(funcName);
            }
            if (info.category === 'state' || info.category === 'bind') {
                stateFunctions.push(funcName);
            }
        }
        return { categories, commands, drawCallFunctions, resourceFunctions, stateFunctions };
    }
    // ========== RENDER STATE TRACKER ==========
    trackRenderState(funcName) {
        const func = this.index.functionsByName.get(funcName);
        if (!func)
            return null;
        // Known device context offsets from video.cpp analysis
        const deviceOffsets = [
            { offset: 10456, usage: 'Vertex Declaration' },
            { offset: 10932, usage: 'Vertex Shader' },
            { offset: 10936, usage: 'Pixel Shader' },
            { offset: 11000, usage: 'GPU Command Sync Flag' },
            { offset: 12020, usage: 'Stream Source 0' },
            { offset: 13580, usage: 'Index Buffer' },
        ];
        // Check kernel calls for render operations
        const renderTargetOps = func.kernelCalls.filter(k => k.includes('RenderTarget') || k.includes('DepthStencil') || k.includes('Surface'));
        const shaderOps = func.kernelCalls.filter(k => k.includes('Shader') || k.includes('shader'));
        return {
            function: funcName,
            stateAccess: [],
            deviceOffsets,
            renderTargetOps,
            shaderOps,
        };
    }
    // ========== SHADER USAGE ANALYZER ==========
    analyzeShaderUsage() {
        const vsSetters = [];
        const psSetters = [];
        const shaderCreation = [];
        const shaderBinding = [];
        for (const [funcName, info] of Object.entries(GPU_FUNCTIONS)) {
            if (info.category === 'shader') {
                const func = this.index.functionsByName.get(funcName);
                const callerCount = func?.calledBy.length || 0;
                if (info.description.includes('Vertex')) {
                    vsSetters.push({ name: funcName, callers: callerCount });
                }
                if (info.description.includes('Pixel')) {
                    psSetters.push({ name: funcName, callers: callerCount });
                }
                if (info.description.includes('Create')) {
                    shaderCreation.push({ function: funcName, type: info.description });
                }
                else {
                    shaderBinding.push({ function: funcName, description: info.description });
                }
            }
        }
        // Add creation functions
        shaderCreation.push({ function: 'sub_82548700', type: 'CreateVertexShader' });
        shaderCreation.push({ function: 'sub_82548608', type: 'CreatePixelShader' });
        return {
            vertexShaderFunctions: vsSetters,
            pixelShaderFunctions: psSetters,
            shaderCreation,
            shaderBinding,
        };
    }
    // ========== FRAME TIMING ANALYZER ==========
    analyzeFrameTiming() {
        return {
            vblankTarget: '60Hz (16.67ms)',
            frameTimeMs: 16.67,
            timingFunctions: [
                { function: 'mftb', description: 'Move From Time Base - reads CPU cycle counter' },
                { function: '__imp__KeQueryPerformanceCounter', description: 'High-resolution timer' },
                { function: '__imp__KeQueryPerformanceFrequency', description: 'Timer frequency' },
                { function: '__imp__KeDelayExecutionThread', description: 'Thread sleep' },
            ],
            syncPoints: [
                { location: 'VdSwap', type: 'VBlank', description: 'Frame presentation sync' },
                { location: 'sub_82856F08', type: 'Frame Start', description: 'Main loop entry - reads timestamp' },
                { location: 'sub_828529B0', type: 'Frame Orchestrator', description: 'Coordinates frame work' },
                { location: 'Device+11000', type: 'GPU Sync', description: 'GPU command buffer sync flag' },
            ],
            magicValues: [
                { value: 996, name: 'NO_PROGRESS', effect: 'Exit early without advancing state' },
                { value: 997, name: 'PENDING', effect: 'Store pending state and return' },
                { value: 258, name: 'WAIT_TIMEOUT', effect: 'Maps to 996 in handlers' },
                { value: 259, name: 'STATUS_PENDING', effect: 'Triggers explicit wait path' },
            ],
        };
    }
    // ========== RENDER BLOCKING ANALYZER ==========
    analyzeRenderBlocking() {
        return {
            blockingPoints: [
                {
                    function: 'sub_82857240',
                    reason: 'GPU state setup calls blocking sync primitives',
                    solution: 'Stub or reimplement to skip Xbox GPU hardware calls',
                },
                {
                    function: 'sub_8218C600',
                    reason: 'Core init blocked at vtable[1] call',
                    solution: 'Ensure sub_82857240 returns without blocking',
                },
                {
                    function: 'sub_827DB988',
                    reason: 'Semaphore create/wait in resource lookup',
                    solution: 'Hook to return immediately with success',
                },
            ],
            initBlockers: [
                {
                    function: 'sub_821A8868',
                    blocksAt: 'sub_82300C78 (string lookup)',
                    workaround: 'Stub entire function to return success',
                },
                {
                    function: 'sub_8220E108',
                    blocksAt: 'sub_82430C60 (wanted system)',
                    workaround: 'Stub entire function',
                },
            ],
            syncPrimitiveIssues: [
                {
                    primitive: 'Event (KeWaitForSingleObject)',
                    problem: 'Workers block waiting for events never signaled',
                    fix: 'Track event handles and signal in VdSwap',
                },
                {
                    primitive: 'Semaphore (KeWaitForSingleObject)',
                    problem: 'Semaphore count depleted',
                    fix: 'Release semaphores in VdSwap frame sync',
                },
                {
                    primitive: 'Spinlock (device+11000)',
                    problem: 'GPU sync spin loop never exits',
                    fix: 'Stub to clear flag immediately',
                },
            ],
        };
    }
    // ========== BUILD PROJECT ==========
    async buildProject(platform, mode) {
        const key = `${platform}-${mode}`;
        const config = BUILD_PRESETS[key];
        if (!config) {
            return {
                success: false,
                preset: 'unknown',
                buildDir: '',
                commands: [],
                error: `Unknown platform/mode combination: ${key}. Valid options: ${Object.keys(BUILD_PRESETS).join(', ')}`,
            };
        }
        const commands = [
            `cmake . --preset ${config.preset}`,
            `cmake --build ./${config.buildDir} --target LibertyRecomp`,
        ];
        // For macOS, need to set VCPKG_ROOT
        const envSetup = platform.startsWith('macos')
            ? 'export VCPKG_ROOT=$(pwd)/thirdparty/vcpkg && '
            : '';
        return {
            success: true,
            preset: config.preset,
            buildDir: config.buildDir,
            commands: commands.map(c => envSetup + c),
            output: `Build configured for ${platform} in ${mode} mode.\n\nRun these commands in ${this.projectRoot}:\n\n${commands.join('\n')}`,
        };
    }
    // ========== LIST BUILD TARGETS ==========
    listBuildTargets() {
        const currentOS = process.platform === 'darwin' ? 'macos'
            : process.platform === 'linux' ? 'linux'
                : 'windows';
        const arch = process.arch === 'arm64' ? 'arm64' : 'x64';
        return {
            platforms: [
                { name: 'windows-x64', presets: ['x64-Clang-Debug', 'x64-Clang-RelWithDebInfo', 'x64-Clang-Release'] },
                { name: 'windows-arm64', presets: ['arm64-Clang-Debug', 'arm64-Clang-RelWithDebInfo', 'arm64-Clang-Release'] },
                { name: 'linux-x64', presets: ['linux-debug', 'linux-relwithdebinfo', 'linux-release'] },
                { name: 'linux-arm64', presets: ['linux-debug', 'linux-relwithdebinfo', 'linux-release'] },
                { name: 'macos-arm64', presets: ['macos-debug', 'macos-relwithdebinfo', 'macos-release'] },
                { name: 'macos-x64', presets: ['macos-debug', 'macos-relwithdebinfo', 'macos-release'] },
            ],
            modes: ['debug', 'relwithdebinfo', 'release'],
            currentOS: `${currentOS}-${arch}`,
            recommendedPreset: BUILD_PRESETS[`${currentOS}-${arch}-relwithdebinfo`]?.preset || 'unknown',
        };
    }
    // ========== DEVICE CONTEXT INSPECTOR ==========
    inspectDeviceContext() {
        return {
            tlsOffset: 1676,
            deviceSize: '0x5000 bytes (from GuestDevice struct)',
            knownOffsets: [
                { offset: 0, name: 'vtable', type: 'pointer', usage: 'Virtual function table' },
                { offset: 10456, name: 'vertexDeclaration', type: 'pointer', usage: 'Current vertex declaration' },
                { offset: 10932, name: 'vertexShader', type: 'pointer', usage: 'Current vertex shader' },
                { offset: 10936, name: 'pixelShader', type: 'pointer', usage: 'Current pixel shader' },
                { offset: 11000, name: 'gpuSyncFlag', type: 'uint32', usage: 'GPU command buffer sync' },
                { offset: 12020, name: 'streamSource0', type: 'pointer', usage: 'Vertex buffer binding' },
                { offset: 13580, name: 'indexBuffer', type: 'pointer', usage: 'Index buffer binding' },
            ],
            criticalFields: [
                { offset: 11000, description: 'GPU Sync Flag', impact: 'Spin loop blocks if not cleared' },
                { offset: 10932, description: 'Vertex Shader', impact: 'Draw calls fail without valid shader' },
                { offset: 10936, description: 'Pixel Shader', impact: 'Draw calls fail without valid shader' },
            ],
        };
    }
    // ========== RENDER CONTEXT INSPECTOR ==========  
    inspectRenderContext() {
        return {
            globalAddress: '0x83042DEC',
            structure: [
                { offset: 0, name: 'vtable', type: 'pointer', description: 'Virtual function table pointer' },
                { offset: 16, name: 'timestamp', type: 'uint64', description: 'Last frame timestamp (from mftb)' },
                { offset: 36, name: 'frameTime', type: 'float', description: 'Accumulated frame time' },
            ],
            vtableFunctions: [
                { index: 0, function: 'Unknown', description: 'Constructor/destructor?', blocking: false },
                { index: 1, function: 'sub_82857240', description: 'GPU State Setup', blocking: true },
                { index: 16, function: 'Render call', description: 'Main render dispatch', blocking: false },
            ],
        };
    }
}
