// This file is part of the FidelityFX SDK.
//
// Copyright (C) 2025 Advanced Micro Devices, Inc.
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <stdlib.h>
#include <locale.h>
#include <stdio.h>
#include <tchar.h>

#include <process.h>
#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <dbghelp.h>
#include <intrin.h>
#include <vector>
#include <mutex>
#include <format>

namespace fsr
{
    const int MaxNameLen = 256;
#pragma comment(lib, "Dbghelp.lib")

    std::mutex cacheMutex;
    std::vector<std::string> cachedBacktrace;

    namespace dbg
    {
        /*
        Windows stack backtrace code by Sean Farrel
        https://gist.github.com/rioki/85ca8295d51a5e0b7c56e5005b0ba8b4
        */

        std::string basename(const std::string& file)
        {
            unsigned int i = file.find_last_of("\\/");
            if (i == std::string::npos)
            {
                return file;
            }
            else
            {
                return file.substr(i + 1);
            }
        }

        struct StackFrame
        {
            DWORD64 address;
            std::string name;
            std::string module;
            int line;
            std::string file;
        };

        inline std::vector<StackFrame> stack_trace()
        {
#if _WIN64
            DWORD machine = IMAGE_FILE_MACHINE_AMD64;
#else
            DWORD machine = IMAGE_FILE_MACHINE_I386;
#endif
            HANDLE process = GetCurrentProcess();
            HANDLE thread = GetCurrentThread();

            if (SymInitialize(process, NULL, TRUE) == FALSE)
            {
                std::printf(__FUNCTION__ ": Failed to call SymInitialize.");
                return std::vector<StackFrame>();
            }

            SymSetOptions(SYMOPT_LOAD_LINES);

            CONTEXT    context = {};
            context.ContextFlags = CONTEXT_FULL;
            RtlCaptureContext(&context);

#if _WIN64
            STACKFRAME frame = {};
            frame.AddrPC.Offset = context.Rip;
            frame.AddrPC.Mode = AddrModeFlat;
            frame.AddrFrame.Offset = context.Rbp;
            frame.AddrFrame.Mode = AddrModeFlat;
            frame.AddrStack.Offset = context.Rsp;
            frame.AddrStack.Mode = AddrModeFlat;
#else
            STACKFRAME frame = {};
            frame.AddrPC.Offset = context.Eip;
            frame.AddrPC.Mode = AddrModeFlat;
            frame.AddrFrame.Offset = context.Ebp;
            frame.AddrFrame.Mode = AddrModeFlat;
            frame.AddrStack.Offset = context.Esp;
            frame.AddrStack.Mode = AddrModeFlat;
#endif

            bool first = true;

            std::vector<StackFrame> frames;
            while (StackWalk(machine, process, thread, &frame, &context, NULL, SymFunctionTableAccess, SymGetModuleBase, NULL))
            {
                StackFrame f = {};
                f.address = frame.AddrPC.Offset;

#if _WIN64
                DWORD64 moduleBase = 0;
#else
                DWORD moduleBase = 0;
#endif

                moduleBase = SymGetModuleBase(process, frame.AddrPC.Offset);

                char moduelBuff[MAX_PATH];
                if (moduleBase && GetModuleFileNameA((HINSTANCE)moduleBase, moduelBuff, MAX_PATH))
                {
                    f.module = basename(moduelBuff);
                }
                else
                {
                    f.module = "Unknown Module";
                }
#if _WIN64
                DWORD64 offset = 0;
#else
                DWORD offset = 0;
#endif
                char symbolBuffer[sizeof(IMAGEHLP_SYMBOL) + 255];
                PIMAGEHLP_SYMBOL symbol = (PIMAGEHLP_SYMBOL)symbolBuffer;
                symbol->SizeOfStruct = (sizeof IMAGEHLP_SYMBOL) + 255;
                symbol->MaxNameLength = 254;

                if (SymGetSymFromAddr(process, frame.AddrPC.Offset, &offset, symbol))
                {
                    f.name = symbol->Name;
                }
                else
                {
                    DWORD error = GetLastError();
                    f.name = std::format("Failed to resolve address 0x%X: %u\n", frame.AddrPC.Offset, error);
                }

                IMAGEHLP_LINE line;
                line.SizeOfStruct = sizeof(IMAGEHLP_LINE);

                DWORD offset_ln = 0;
                if (SymGetLineFromAddr(process, frame.AddrPC.Offset, &offset_ln, &line))
                {
                    f.file = line.FileName;
                    f.line = line.LineNumber;
                }
                else
                {
                    f.file = "[unknown file]";
                    f.line = -1;
                }

                if (!first)
                {
                    frames.push_back(f);
                }
                first = false;
            }

            SymCleanup(process);

            return frames;
        }
    }

    namespace StackBacktrace
    {
        void Cache()
        {
            const auto stack = dbg::stack_trace();

            std::lock_guard<std::mutex> lock(cacheMutex);
            cachedBacktrace.clear();
            for (int i = 0; i < int(stack.size()) - 1; ++i)
            {
                const auto& frame = stack[i + 1];
                cachedBacktrace.push_back(std::format("%i: 0x%x: %s in %s (%i)\n", i, frame.address, frame.name, frame.file, frame.line));
            }
        }

        void Clear()
        {
            std::lock_guard<std::mutex> lock(cacheMutex);
            cachedBacktrace.clear();
        }

        std::vector<std::string> Get()
        {
            std::lock_guard<std::mutex> lock(cacheMutex);
            return cachedBacktrace;
        }

        void Print()
        {
            const auto backtrace = StackBacktrace::Get();
            if (!backtrace.empty())
            {
                std::cout << "Stack backtrace:" << std::endl;
                for (const auto& frame : backtrace)
                {
                    std::cout << frame;
                }
            }
        }

        inline std::string ExceptionBacktrace(_EXCEPTION_POINTERS* exPtrs)
        {
            CONTEXT* ctx = exPtrs->ContextRecord;

            BOOL    result;
            HANDLE  process;
            HANDLE  thread;
            HMODULE hModule;

            STACKFRAME64        stack;
            ULONG               frame;
            DWORD64             displacement;

            DWORD disp;
            IMAGEHLP_LINE64* line;

            char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
            char name[MaxNameLen];
            char module[MaxNameLen];
            PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;

            memset(&stack, 0, sizeof(STACKFRAME64));

            process = GetCurrentProcess();
            thread = GetCurrentThread();
            displacement = 0;
#if !defined(_M_AMD64)
            stack.AddrPC.Offset = (*ctx).Eip;
            stack.AddrPC.Mode = AddrModeFlat;
            stack.AddrStack.Offset = (*ctx).Esp;
            stack.AddrStack.Mode = AddrModeFlat;
            stack.AddrFrame.Offset = (*ctx).Ebp;
            stack.AddrFrame.Mode = AddrModeFlat;
#endif

            SymInitialize(process, NULL, TRUE); //load symbols

            for (frame = 0; ; frame++)
            {
                //get next call from stack
                result = StackWalk64
                (
#if defined(_M_AMD64)
                    IMAGE_FILE_MACHINE_AMD64
#else
                    IMAGE_FILE_MACHINE_I386
#endif
                    ,
                    process,
                    thread,
                    &stack,
                    ctx,
                    NULL,
                    SymFunctionTableAccess64,
                    SymGetModuleBase64,
                    NULL
                );

                if (!result) break;

                //get symbol name for address
                pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
                pSymbol->MaxNameLen = MAX_SYM_NAME;
                SymFromAddr(process, (ULONG64)stack.AddrPC.Offset, &displacement, pSymbol);

                line = (IMAGEHLP_LINE64*)malloc(sizeof(IMAGEHLP_LINE64));
                line->SizeOfStruct = sizeof(IMAGEHLP_LINE64);

                //try to get line
                if (SymGetLineFromAddr64(process, stack.AddrPC.Offset, &disp, line))
                {
                    printf("\tat %s in %s: line: %lu: address: 0x%0X\n", pSymbol->Name, line->FileName, line->LineNumber, pSymbol->Address);
                }
                else
                {
                    //failed to get line
                    printf("\tat %s, address 0x%0X.\n", pSymbol->Name, pSymbol->Address);
                    hModule = NULL;
                    lstrcpyA(module, "");
                    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                        (LPCTSTR)(stack.AddrPC.Offset), &hModule);

                    //at least print module name
                    if (hModule != NULL)GetModuleFileNameA(hModule, module, MaxNameLen);

                    printf("in %s\n", module);
                }

                free(line);
                line = NULL;
            }

            return std::string();
        }
    }
}
