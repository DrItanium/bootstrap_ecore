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
// Created by jwscoggins on 6/7/21.
//
#include <stdint.h>

template<typename T>
inline volatile T& memory(const uint32_t address) { return *reinterpret_cast<T*>(address); }

/**
 * @brief Perform the boot checksum on the initial memory image found in external memory to check for success or not
 * @return If the checksum was success
 */
bool
performCheckSum() {
    uint32_t sum = 0;
    for (uint32_t i = 0; i < 8; ++i) {
        sum += memory<uint32_t>(i);
    }
    return ((~sum) + 0xFFFFFFFF) == 0;
}
void
doIAC(uint8_t messageType, uint8_t field1 = 0, uint16_t field2 = 0, uint32_t field3 = 0, uint32_t field4 = 0, uint32_t field5 = 0) {
    union {
       uint32_t output;
       struct {
           uint16_t field2;
           uint8_t field1;
           uint8_t messageType;
       };
    } temporary;
    temporary.field2 = field2;
    temporary.field1 = field1;
    temporary.messageType = messageType;
    uint32_t IACPortAddress = 0xff000010;
    uint32_t message[4] = {
        temporary.output,
        field3,
        field4,
        field5,
    };
    asm volatile ("synmovq %0, %1" : : "r" (IACPortAddress), "r" (message));
}
void
jumpToExternalExecution() {
    // send the processor an IAC message to start normal processor execution
    // my processor impl has a custom IAC message to do the boot itself
    doIAC(0);
}
void
checksumFailProcessor() {
    doIAC(0x01);
}
extern "C" void bootstrapSystem() {
    if (!performCheckSum()) {
        checksumFailProcessor();
    } else {
        jumpToExternalExecution();
    }
}

