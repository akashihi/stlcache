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

#ifndef STLCACHE_LOCK_HPP_INCLUDED
#define STLCACHE_LOCK_HPP_INCLUDED

using namespace std;

#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>

namespace stlcache {
    /*!
     * \brief Abstract interface for locking operations support.
     *  
	 * lock class defines, what methods are expected by \link stlcache::cache cache \endlink for locking. Usually you will never be using this class directly, 
     * but it is you key for writing your custom locking implementation classes. 
     * 
     * The goal of writing those implementation classes is to provide a \link stlcache::cache cache \endlink with a several, configurable locking approaches.
     * Abstract interface provides calls to aqcuire shared read-only locks and exclusive write locks. Implementations are not obliged to follow that 
     * style of locking and are allowed to use exclusive locks only for both calls.
     * 
     * 
     * All locks implementations shold declare some read and write type of lock, to be used by \link stlcache::cache cache \endlink in RAII style. 
     * On the other hand, \link stlcache::cache cache \endlink should not use any assumptions on those types and interact with values of that types,
     * but only acquire their instances.
     * 
     * 
     * \tparam <ReadLock> Type of a shared read-only lock object.
     * \tparam <WriteLock> Type of a exclusive lock object.
     * 
     * \see stlcache::cache
     * 
     * \author chollya (5/26/2017)
     */
    template <class ReadLock, class WriteLock> class lock {
    public:
    	typedef WriteLock write;
    	typedef ReadLock read;
		
		virtual write lockWrite() const =0;

		virtual read lockRead() const =0;
    };
}

#endif /* STLCACHE_LOCK_HPP_INCLUDED */
