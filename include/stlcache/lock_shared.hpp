//
// Copyright (C) 2011-2017 Denis V Chapligin, Martin Hrabovsky
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STLCACHE_LOCK_SHARED_HPP_INCLUDED
#define STLCACHE_LOCK_SHARED_HPP_INCLUDED

#ifdef USE_BOOST_OPTIONAL

#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>

using namespace std;

#include <stlcache/lock.hpp>

namespace stlcache {
    /*!
     * \brief A shared implementation of locking.
     * 
     * This implementation supports shared read/write locking, so reading threads will be able to access cache in
     * parallel, while writing threads will be locked exclusively and only one writing thread will be able 
     * to modify the cache.
     */
    class lock_shared : public lock<boost::shared_lock<boost::shared_mutex>, boost::unique_lock<boost::shared_mutex> > {
         mutable boost::shared_mutex mtx;
    public:
		write lockWrite() const {
			return write(mtx);
		}

		read lockRead() const {
			return read(mtx);
		}
    };
}

#endif /* USE_BOOST_OPTIONAL */

#endif /* STLCACHE_LOCK_SHARED_HPP_INCLUDED */
