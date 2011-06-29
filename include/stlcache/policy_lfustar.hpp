//
// Copyright (C) 2011 Denis V Chapligin
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STLCACHE_POLICY_LFUSTAR_HPP_INCLUDED
#define STLCACHE_POLICY_LFUSTAR_HPP_INCLUDED

#include <set>
#include <map>

using namespace std;

#include <stlcache/policy.hpp>

namespace stlcache {
    template <class Key,template <typename T> class Allocator> class _policy_lfustar_type : public virtual _policy_lfu_type<Key,Allocator> {
        typedef set<Key,less<Key>,Allocator<Key> > keySet;
    public:
        _policy_lfustar_type(const size_t& size ) throw() : _policy_lfu_type<Key,Allocator>(size) { }
        _policy_lfustar_type(const _policy_lfustar_type& x ) throw() : _policy_lfu_type<Key,Allocator>(x) { }

        virtual const _victim<Key> victim() throw()  {
            //LFU* only operates on entries with references count equal to 1
            entriesType::const_iterator entriesIter = this->entries().find(1);
            if (entriesIter==this->entries().end()) {
                return _victim<Key>();
            }

            return _victim<Key>(entriesIter->second);
        }
    };
    struct policy_lfustar {
        template <typename Key, template <typename T> class Allocator>
            struct bind : _policy_lfustar_type<Key,Allocator> { 
                bind(const bind& x) : _policy_lfustar_type<Key,Allocator>(x),_policy_lfu_type<Key,Allocator>(x)  { }
                bind(const size_t& size) : _policy_lfustar_type<Key,Allocator>(size),_policy_lfu_type<Key,Allocator>(size) { }
            };
    };
}

#endif /* STLCACHE_POLICY_LFUSTAR_HPP_INCLUDED */
