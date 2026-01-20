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

#pragma once

#include <chrono>
#include <functional>
#include "Log.h"

namespace fsr
{
    class HighResTimer
    {
    public:
        HighResTimer() : m_startTime(std::chrono::high_resolution_clock::now()) {}

        HighResTimer(std::function<std::string(float)> lambda) :
            m_lambda(lambda),
            m_startTime(std::chrono::high_resolution_clock::now()) {
        }

        ~HighResTimer()
        {
            if (m_lambda)
            {
                Log::Debug("%s\n", m_lambda(Get()).c_str());
            }
        }

        inline float Get() const
        {
            return float(std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - m_startTime).count());
        }

        inline void Reset()
        {
            m_startTime = std::chrono::high_resolution_clock::now();
        }

        inline void Write(const std::string& format) const
        {
            Log::Debug(format, Get());
        }

    private:
        const std::string m_message;
        std::chrono::time_point<std::chrono::high_resolution_clock>  m_startTime;
        std::function<std::string(float)> m_lambda;
    };
}
