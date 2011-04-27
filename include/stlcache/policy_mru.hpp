//
// Copyright (C) 2011 Denis V Chapligin
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STLCACHE_POLICY_MRU_HPP_INCLUDED
#define STLCACHE_POLICY_MRU_HPP_INCLUDED

#include <list>

using namespace std;

#include <stlcache/policy_lru.hpp>

namespace stlcache {
    template <class Key,template <typename T> class Allocator> class _policy_mru_type : public _policy_lru_type<Key,Allocator> {
    public:
        _policy_mru_type(const size_t& size ) throw() : _policy_lru_type<Key,Allocator>(size) { }

        virtual const _victim<Key> victim() throw()  {
            return _victim<Key>(this->entries().front());
        }
    };
    struct policy_mru {
        template <typename Key, template <typename T> class Allocator>
            struct bind : _policy_mru_type<Key,Allocator> { 
                bind(const bind& x) : _policy_mru_type<Key,Allocator>(x)  { }
                bind(const size_t& size) : _policy_mru_type<Key,Allocator>(size) { }
            };
    };
}

#endif /* STLCACHE_POLICY_MRU_HPP_INCLUDED */
