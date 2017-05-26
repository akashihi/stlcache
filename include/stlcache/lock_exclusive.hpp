//
// Copyright (C) 2011-2017 Denis V Chapligin, Martin Hrabovsky
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

//
// Copyright (C) 2011 Denis V Chapligin
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STLCACHE_LOCK_NONE_HPP_INCLUDED
#define STLCACHE_LOCK_NONE_HPP_INCLUDED

#include <mutex>

using namespace std;

namespace stlcache {
    /*!
     * \brief An exclusive only implementation of locking.
     * 
     * This implementation supports only mutually exclusive locking, so only a single thread at a time can access the \link stlcache::cache cache \endlink
     */

    class lock_none : public lock<lock_guard<mutex>, lock_guard<mutex>> {
        mutex mtx
    public:
		write lockWrite() const { 
            return lock_guard(mtx); 
        }

		read lockRead() const { 
            lock_guard(mtx);
        }
    };
}

#endif /* STLCACHE_LOCK_NONE_HPP_INCLUDED */
