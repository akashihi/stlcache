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

#ifndef STLCACHE_LOCK_EXCLUSIVE_HPP_INCLUDED
#define STLCACHE_LOCK_EXCLUSIVE_HPP_INCLUDED

#include <mutex>

using namespace std;

namespace stlcache {
    /*!
     * \brief An exclusive only implementation of locking.
     * 
     * This implementation supports only mutually exclusive locking, so only a single thread at a time can access the \link stlcache::cache cache \endlink
     */

    class lock_exclusive : public lock<unique_lock<mutex>, unique_lock<mutex> > {
        mutable mutex mtx;
    public:
		write lockWrite() const { 
            return unique_lock<mutex>(mtx); 
        }

		read lockRead() const { 
            return unique_lock<mutex>(mtx);
        }
    };
}

#endif /* STLCACHE_LOCK_EXCLUSIVE_HPP_INCLUDED */
