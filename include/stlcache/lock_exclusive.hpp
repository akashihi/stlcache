//
// Copyright (C) 2011-2023 Denis V Chapligin, Martin Hrabovsky, Vojtech Ondruj
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

namespace stlcache {
    /*!
     * \brief An exclusive only implementation of locking.
     * 
     * This implementation supports only mutually exclusive locking, so only a single thread at a time can access the \link stlcache::cache cache \endlink
     */

class lock_exclusive : public lock<std::unique_lock<std::mutex>, std::unique_lock<std::mutex> > {
        mutable std::mutex mtx;
    public:
		write lockWrite() const override {
            return std::unique_lock<std::mutex>(mtx);
        }

		read lockRead() const override {
            return std::unique_lock<std::mutex>(mtx);
        }
    };
}

#endif /* STLCACHE_LOCK_EXCLUSIVE_HPP_INCLUDED */
