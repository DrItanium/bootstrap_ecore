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

#ifndef HITAGIMON_FAULTS_H
#define HITAGIMON_FAULTS_H
namespace cortex
{
    struct FaultData
    {
        volatile unsigned reserved;
        volatile unsigned override[3];
        volatile unsigned fdata[3];
        volatile unsigned override_data;
        volatile unsigned pc;
        volatile unsigned ac;
        volatile unsigned int fsubtype: 8,
                freserved: 8,
                ftype: 8,
                fflags: 8;
        volatile unsigned int *faddress;
        void display();
    } __attribute__((packed));

    typedef void (*FaultHandler)(FaultData *data);
    namespace UserFaultKind {
        enum UserFaultKind {
            Reserved,
            Trace,
            Operation,
            Arithmetic,
            RealArithmetic,
            Constraint,
            Protection,
            Machine,
            Type,
        };
    }

    FaultHandler getUserReservedFaultHandler();
    FaultHandler getUserTraceFaultHandler();
    FaultHandler getUserOperationFaultHandler();
    FaultHandler getUserArithmeticFaultHandler();
    FaultHandler getUserRealArithmeticFaultHandler();
    FaultHandler getUserConstraintFaultHandler();
    FaultHandler getUserProtectionFaultHandler();
    FaultHandler getUserMachineFaultHandler();
    FaultHandler getUserTypeFaultHandler();
    template<UserFaultKind::UserFaultKind kind>
    inline FaultHandler getUserFaultHandler() {
        switch (kind) {
            case UserFaultKind::Reserved: return getUserReservedFaultHandler();
            case UserFaultKind::Trace: return getUserTraceFaultHandler();
            case UserFaultKind::Operation: return getUserOperationFaultHandler();
            case UserFaultKind::Arithmetic: return getUserArithmeticFaultHandler();
            case UserFaultKind::RealArithmetic: return getUserRealArithmeticFaultHandler();
            case UserFaultKind::Constraint: return getUserConstraintFaultHandler();
            case UserFaultKind::Protection: return getUserProtectionFaultHandler();
            case UserFaultKind::Machine: return getUserMachineFaultHandler();
            case UserFaultKind::Type: return getUserTypeFaultHandler();
            default: return 0;
        }
    }

    void setUserReservedFaultHandler(FaultHandler);
    void setUserTraceFaultHandler(FaultHandler);
    void setUserOperationFaultHandler(FaultHandler);
    void setUserArithmeticFaultHandler(FaultHandler);
    void setUserRealArithmeticFaultHandler(FaultHandler);
    void setUserConstraintFaultHandler(FaultHandler);
    void setUserProtectionFaultHandler(FaultHandler);
    void setUserMachineFaultHandler(FaultHandler);
    void setUserTypeFaultHandler(FaultHandler);

    template<UserFaultKind::UserFaultKind kind>
    inline void setUserFaultHandler(FaultHandler handler) {
        switch (kind) {
            case UserFaultKind::Reserved: setUserReservedFaultHandler(handler); break;
            case UserFaultKind::Trace: setUserTraceFaultHandler(handler); break;
            case UserFaultKind::Operation: setUserOperationFaultHandler(handler); break;
            case UserFaultKind::Arithmetic: setUserArithmeticFaultHandler(handler); break;
            case UserFaultKind::RealArithmetic: setUserRealArithmeticFaultHandler(handler); break;
            case UserFaultKind::Constraint: setUserConstraintFaultHandler(handler); break;
            case UserFaultKind::Protection: setUserProtectionFaultHandler(handler); break;
            case UserFaultKind::Machine: setUserMachineFaultHandler(handler); break;
            case UserFaultKind::Type: setUserTypeFaultHandler(handler); break;
            default: break;
        }
    }
}
#endif //HITAGIMON_FAULTS_H
