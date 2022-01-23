/*
bootstrap_ecore
Copyright (c) 2020-2022, Joshua Scoggins
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
//
// Created by jwscoggins on 8/28/21.
//

#ifndef HITAGIMON_SYSEXAMINE_H
#define HITAGIMON_SYSEXAMINE_H
#include <stdint.h>
namespace cortex {
    /**
     * @brief i960 specific arithmetic controls
     */
    union ArithmeticControls {
        uint32_t raw;
        struct {
            uint32_t cc : 3;
#ifdef __i960SB__
            uint32_t arithmeticStatus : 4;
            uint32_t unused0 : 1;
#else
            uint32_t unused0 : 5;
#endif
            uint32_t integerOverflowFlag : 1;
            uint32_t unused1 : 1;
            uint32_t integerOverflowMask : 1;
            uint32_t unused2 : 2;
            uint32_t noImpreciseFaults : 1;
#ifdef __i960SB__
            uint32_t floatingOverflowFlag : 1;
            uint32_t floatingUnderflowFlag : 1;
            uint32_t floatingInvalidOpFlag : 1;
            uint32_t floatingZeroDivideFlag : 1;
            uint32_t floatingInexactFlag : 1;
            uint32_t unused3 : 3;
            uint32_t floatingOverflowMask : 1;
            uint32_t floatingUnderflowMask : 1;
            uint32_t floatingInvalidOpMask : 1;
            uint32_t floatingZeroDivideMask : 1;
            uint32_t floatingInexactMask : 1;
            uint32_t floatingPointNormalizingMode : 1;
            uint32_t floatingPointRoundingControl : 2;
#else
            uint32_t unused3 : 16;
#endif
        };
    };
    union ProcessControls
    {
        uint32_t raw;
        struct {
            uint32_t traceEnable: 1;
            uint32_t executionMode: 1;
            uint32_t unused0: 7;
            uint32_t resume: 1;
            uint32_t traceFaultPending: 1;
            uint32_t unused1: 2;
            uint32_t state: 1;
            uint32_t unused2: 2;
            uint32_t priority: 5;
            uint32_t internalState: 10;
        };
    };
    union TraceControls {
        uint32_t raw;
        struct {
            uint32_t unused0 : 1;
            uint32_t instructionTraceMode : 1;
            uint32_t branchTraceMode : 1;
            uint32_t callTraceMode : 1;
            uint32_t returnTraceMode : 1;
            uint32_t prereturnTraceMode : 1;
            uint32_t supervisorTraceMode : 1;
            uint32_t breakpointTraceMode : 1;
            uint32_t unused1 : 9;
            uint32_t instructionTraceEvent : 1;
            uint32_t branchTraceEvent : 1;
            uint32_t callTraceEvent : 1;
            uint32_t returnTraceEvent : 1;
            uint32_t prereturnTraceEvent : 1;
            uint32_t supervisorTraceEvent : 1;
            uint32_t breakpointTraceEvent : 1;
            uint32_t unused2 : 8;
        };
    };
#define GetProcessControls(pc) asm volatile ("modpc 0, 0, %0" : "=r" (pc.raw))
#define GetArithmeticControls(ac) asm volatile ("modac 0, 0, %0" : "=r" (ac.raw))
#define GetTraceControls(tc) asm volatile ("modtc 0, 0, %0" : "=r" (tc.raw))
}
#endif //HITAGIMON_SYSEXAMINE_H
