/* Ninetjer - _Truly_ Free (non-GPL) Unix .NET Runtime
 * Copyright (C) 2002-2003  Jay Freeman (saurik)
*/

/*
 *        Redistribution and use in source and binary
 * forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the
 *    above copyright notice, this list of conditions
 *    and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the
 *    above copyright notice, this list of conditions
 *    and the following disclaimer in the documentation
 *    and/or other materials provided with the
 *    distribution.
 * 3. The name of the author may not be used to endorse
 *    or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "cxx/standard.hh"
#pragma hdrstop("build/cxx/standard.pch")

#include "clr/metadata.hpp"

#include "clr/tokens/assemblyref.hpp"
#include "clr/tokens/assemblyrefos.hpp"

namespace clr {

void AssemblyRefOS::Parse(Metadata &metadata, uint32_t token) {
    uint32_t row[4];
    metadata.GetRow(token, row);

    platform_ = row[0];
    major_ = row[1];
    minor_ = row[2];
    ref_ = metadata.GetToken<AssemblyRef>(row[3]);
}

AssemblyRefOS::AssemblyRefOS() {
}

uint32_t AssemblyRefOS::GetOSPlatformId() const {
    return platform_;
}

uint16_t AssemblyRefOS::GetOSMajorVersion() const {
    return major_;
}

uint16_t AssemblyRefOS::GetOSMinorVersion() const {
    return minor_;
}

const AssemblyRef *AssemblyRefOS::GetAssemblyRef() const {
    return ref_;
}

AssemblyRef *AssemblyRefOS::GetAssemblyRef() {
    return ref_;
}

}
