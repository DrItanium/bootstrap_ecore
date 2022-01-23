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

#include "Faults.h"
namespace cortex
{
    void
    FaultData::display() {
        /// @todo reimplement if it makes sense
#if 0
        printf("Fault Type: %x\n", ftype);
        switch (ftype) {
            case 1: printf("\tTrace Fault\n"); break;
            case 2: printf("\tOperation Fault\n"); break;
            case 3: printf("\tArithmetic Fault\n"); break;
            case 4: printf("\tFloating-Point Fault\n"); break;
            case 5: printf("\tConstraint Fault\n"); break;
            case 7: printf("\tProtection Fault\n"); break;
            case 0xa: printf("\tType Fault\n"); break;
            default:
                break;
        }
        printf("Fault Subtype: %x\n", fsubtype);
        if (ftype == 2) {
            // load and display the operation in question
            if (fsubtype == 1) {
                printf("\tInvalid Opcode\n");
            } else if (fsubtype == 4) {
                printf("\tInvalid Operand\n");
            }
            volatile unsigned int* ptr = faddress;
            unsigned int container[8] = { 0 };
            for (int i = 0;i < 8; ++i, ++ptr) {
                container[i] = *ptr;
            }
            printf("\t\tContents: \n");
            for (int i = 0; i < 8; ++i) {
               printf("\t\t\t0x%x\n", container[i]);
            }
        }
        printf("Faulting Address: %p\n", faddress);
        printf("PC: %x\n", pc);
        printf("AC: %x\n", ac);
#endif
    }
    namespace {
        FaultHandler userReserved_ = 0;
        FaultHandler userTrace_ = 0;
        FaultHandler userOperation_ = 0;
        FaultHandler userArithmetic_ = 0;
        FaultHandler userRealArithmetic_ = 0;
        FaultHandler userConstraint_ = 0;
        FaultHandler userProtection_ = 0;
        FaultHandler userMachine_ = 0;
        FaultHandler userType_ = 0;
    }
    FaultHandler getUserReservedFaultHandler() { return userReserved_; }
    FaultHandler getUserTraceFaultHandler() { return userTrace_; }
    FaultHandler getUserOperationFaultHandler() { return userOperation_; }
    FaultHandler getUserArithmeticFaultHandler() { return userArithmetic_; }
    FaultHandler getUserRealArithmeticFaultHandler() { return userRealArithmetic_; }
    FaultHandler getUserConstraintFaultHandler() { return userConstraint_; }
    FaultHandler getUserProtectionFaultHandler() { return userProtection_; }
    FaultHandler getUserMachineFaultHandler() { return userMachine_; }
    FaultHandler getUserTypeFaultHandler() { return userType_; }
    void setUserReservedFaultHandler(FaultHandler handler) { userReserved_ = handler; }
    void setUserTraceFaultHandler(FaultHandler handler) { userTrace_ = handler; }
    void setUserOperationFaultHandler(FaultHandler handler) { userOperation_ = handler; }
    void setUserArithmeticFaultHandler(FaultHandler handler) { userArithmetic_ = handler; }
    void setUserRealArithmeticFaultHandler(FaultHandler handler) { userRealArithmetic_ = handler; }
    void setUserConstraintFaultHandler(FaultHandler handler) { userConstraint_ = handler; }
    void setUserProtectionFaultHandler(FaultHandler handler) { userProtection_ = handler; }
    void setUserMachineFaultHandler(FaultHandler handler) { userMachine_ = handler; }
    void setUserTypeFaultHandler(FaultHandler handler) { userType_ = handler; }
}