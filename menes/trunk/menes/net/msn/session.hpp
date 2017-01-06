/* Menes - C++ High-Level Utility Library
 * Copyright (C) 2005  Jay Freeman (saurik)
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

#ifndef MENES_NET_MSN_SESSION_HPP
#define MENES_NET_MSN_SESSION_HPP

#include "cxx/platform.hpp"
#include "net/linkage.hpp"

#ifdef MENES_PRAGMA_ONCE
#pragma once
#endif

#include "mta/locks.hpp"

namespace net {
namespace MsnMessenger {

EXT_HANDLESTUB(class Connection)
EXT_HANDLESTUB(class NotificationServer)
EXT_HANDLESTUB(class SwitchBoard)

class Session {
  private:
    cse::String email_;
    cse::String password_;

    mta::ConditionVariable<bool> online_;

    typedef ext::RedBlackSet< _R<Connection> > ConnectionSet_;
    typedef ext::RedBlackMap<const std::type_info *, ConnectionSet_> ConnectionMap_;
    mta::ConditionVariable<ConnectionMap_> connections_;

  public:
    Session(const cse::String &email, const cse::String &password);

    void InsertConnection(const _R<Connection> &connection);
    void RemoveConnection(const _R<Connection> &connection);
    void UpdateConnections();

    _R<Connection> GetConnection(const std::type_info &type);

    _R<NotificationServer> GetNotificationServer();
    _R<SwitchBoard> NewSwitchBoard();

    const cse::String &GetEmail() const;
};

} }

#endif//MENES_NET_MSN_SESSION_HPP