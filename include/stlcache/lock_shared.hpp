//
// Copyright (C) 2011-2023 Denis V Chapligin, Martin Hrabovsky, Vojtech Ondruj
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STLCACHE_LOCK_SHARED_HPP_INCLUDED
#define STLCACHE_LOCK_SHARED_HPP_INCLUDED

#include <shared_mutex>

#include <stlcache/lock.hpp>

namespace stlcache {
    /*!
     * \brief A shared implementation of locking.
     * 
     * This implementation supports shared read/write locking, so reading threads will be able to access cache in
     * parallel, while writing threads will be locked exclusively and only one writing thread will be able 
     * to modify the cache.
     */
    class lock_shared : public lock<std::shared_lock<std::shared_mutex>, std::unique_lock<std::shared_mutex> > {
         mutable std::shared_mutex mtx;
    public:
		write lockWrite() const override {
			return write(mtx);
		}

		read lockRead() const override {
			return read(mtx);
		}
    };
}

#endif /* STLCACHE_LOCK_SHARED_HPP_INCLUDED */
