//
// Copyright (C) 2011-2017 Denis V Chapligin, Martin Hrabovsky
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//


#ifndef STLCACHE_LOCK_NONE_HPP_INCLUDED
#define STLCACHE_LOCK_NONE_HPP_INCLUDED

using namespace std;

#include <stlcache/lock.hpp>

namespace stlcache {
    /*!
     * \brief A non-locking implementation of locking.
     * 
     * This implementation makes no actual locking and is not thread safe. On the other hand, it allows \link stlcache::cache cache \endlink user to disable built-in locking and implement it on it's own.
     */

    class lock_none : public lock<void*,void*> {
    public:
		write lockWrite() const { return NULL; }

		read lockRead() const { return NULL; }
    };
}

#endif /* STLCACHE_LOCK_NONE_HPP_INCLUDED */
