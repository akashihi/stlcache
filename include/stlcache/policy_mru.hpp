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
    template <class Key> class policy_mru : public policy_lru<Key> {
    public:
        policy_mru(const size_t& size ) throw() : policy_lru<Key>(size) { }

        virtual const _victim<Key> victim() throw()  {
            return _victim<Key>(this->entries().front());
        }
    };
}

#endif /* STLCACHE_POLICY_MRU_HPP_INCLUDED */
