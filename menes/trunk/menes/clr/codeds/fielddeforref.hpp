/* Ninetjer - _Truly_ Free (non-GPL) Unix .NET Runtime
 * Copyright (C) 2004  Jay Freeman (saurik)
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

#ifndef NINETJER_FIELDDEFORREF_HPP
#define NINETJER_FIELDDEFORREF_HPP

#include "cxx/platform.hpp"
#include "clr/linkage.hpp"

#ifdef MENES_PRAGMA_ONCE
#pragma once
#endif

#include "cse/string.hpp"

namespace clr {

class Field;
class Signature;
class TypeDefOrRef;

class FieldDefOrRef {
  private:
    virtual Field *ResolveField_() const = 0;

  public:
    virtual const cse::String &GetName() const = 0;

    virtual const Signature &GetSignature() const = 0;
    virtual Signature &GetSignature() = 0;

    const Field *ResolveField() const;
    Field *ResolveField();

    virtual const TypeDefOrRef *GetType() const = 0;
    virtual TypeDefOrRef *GetType() = 0;
};

}

#endif//NINETJER_FIELDDEFORREF_HPP
