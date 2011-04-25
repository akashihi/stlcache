//
// Copyright (C) 2011 Denis V Chapligin
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STLCACHE_POLICY_LRU_HPP_INCLUDED
#define STLCACHE_POLICY_LRU_HPP_INCLUDED

#include <list>

using namespace std;

#include <stlcache/policy.hpp>

namespace stlcache {
    template <class Key,template <typename T> class Allocator> class policy_lru : public policy<Key,Allocator> {
        list<Key,Allocator<Key> > _entries;
    public:
        policy_lru<Key,Allocator>& operator= ( const policy_lru<Key,Allocator>& x) throw() {
            this->_entries=x._entries;
            return *this;
        }
        policy_lru(const policy_lru<Key,Allocator>& x) throw() {
            *this=x;
        }
        policy_lru(const size_t& size ) throw() { }

        virtual void insert(const Key& _k) throw(stlcache_invalid_key) {
            _entries.push_front(_k);
        }
        virtual void remove(const Key& _k) throw() {
            _entries.remove(_k);
        }
        virtual void touch(const Key& _k) throw() { 
            _entries.remove(_k);
            _entries.push_front(_k);
        }
        virtual void clear() throw() {
            _entries.clear();
        }
        virtual void swap(policy<Key,Allocator>& _p) throw(stlcache_invalid_policy) {
            try {
                policy_lru<Key,Allocator>& _pn=dynamic_cast<policy_lru<Key,Allocator>& >(_p);
                _entries.swap(_pn._entries);
            } catch (const std::bad_cast& ) {
                throw stlcache_invalid_policy("Attempted to swap incompatible policies");
            }
        }

        virtual const _victim<Key> victim() throw()  {
            return _victim<Key>(_entries.back());
        }

    protected:
        const list<Key>& entries() const  { return this->_entries; }
    };
}

#endif /* STLCACHE_POLICY_LRU_HPP_INCLUDED */
