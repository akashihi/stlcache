//
// Copyright (C) 2011-2017 Denis V Chapligin, Martin Hrabovsky
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STLCACHE_POLICY_LFUAGINGSTAR_HPP_INCLUDED
#define STLCACHE_POLICY_LFUAGINGSTAR_HPP_INCLUDED

#include <set>
#include <map>
#include <ctime>

using namespace std;

#include <stlcache/policy.hpp>
#include <stlcache/policy_lfuaging.hpp>
#include <stlcache/policy_lfustar.hpp>

#ifdef _MSC_VER
#pragma warning( disable : 4250 )
#endif /* _MSC_VER */

namespace stlcache {
    template <time_t Age,class Key,template <typename T> class Allocator> class _policy_lfuagingstar_type : public virtual _policy_lfuaging_type<Age,Key,Allocator>, public virtual _policy_lfustar_type<Key,Allocator> {
    public:
        _policy_lfuagingstar_type(const size_t& size ) : _policy_lfuaging_type<Age,Key,Allocator>(size), _policy_lfustar_type<Key,Allocator>(size),_policy_lfu_type<Key,Allocator>(size) { }

        virtual const _victim<Key> victim()  {
			_policy_lfuaging_type<Age,Key,Allocator>::expire();
            return _policy_lfustar_type<Key,Allocator>::victim();
        }
    };
        /*!
     * \brief A 'LFU*-Aging' policy
     *
     * Combination of \link stlcache::policy_lfustar LFU* \endlink and \link stlcache::policy_lfuaging LFU-Aging \endlink policies.
     *
     * It is mostly the \link stlcache::policy_lfustar LFU* \endlink with expiration of entries. Primary difference with
     * the \link stlcache::policy_lfustar LFU* \endlink is that the entries, with reference count more then 1, will have
     *  a chance to expire, after several applies of the aging interval.
     *
     * \link cache::touch Touching \endlink the entry may not change item's expiration probability. The LFU*-Aging policy may not be able
     * to find a excessive entry, when cache is full, so the \link cache::insert insert call \endlink may throw a \link stlcache::exception_cache_full
     * cache full \endlink exception.
     *
     * The policy must be configured with the length of a aging interval:
     *
     * \tparam <Age>  aging interval in seconds
     *
     * \see policy_lfu
     * \see policu_lfuaging
     * \see policu_lfustar
     */
    template <time_t Age> struct policy_lfuagingstar {
        template <typename Key, template <typename T> class Allocator>
            struct bind : _policy_lfuagingstar_type<Age,Key,Allocator> {
                bind(const bind& x) : _policy_lfuagingstar_type<Age,Key,Allocator>(x),_policy_lfuaging_type<Age,Key,Allocator>(x),_policy_lfustar_type<Key,Allocator>(x),_policy_lfu_type<Key,Allocator>(x)  { }
                bind(const size_t& size) : _policy_lfuagingstar_type<Age,Key,Allocator>(size),_policy_lfuaging_type<Age,Key,Allocator>(size),_policy_lfustar_type<Key,Allocator>(size),_policy_lfu_type<Key,Allocator>(size)  { }
            };
    };
}

#endif /* STLCACHE_POLICY_LFUAGINGSTAR_HPP_INCLUDED */
