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
    template <class Key,template <typename T> class Allocator> class _policy_lru_type : public policy<Key,Allocator> {
        list<Key,Allocator<Key> > _entries;
    public:
        _policy_lru_type<Key,Allocator>& operator= ( const _policy_lru_type<Key,Allocator>& x) throw() {
            this->_entries=x._entries;
            return *this;
        }
        _policy_lru_type(const _policy_lru_type<Key,Allocator>& x) throw() {
            *this=x;
        }
        _policy_lru_type(const size_t& size ) throw() { }

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
                _policy_lru_type<Key,Allocator>& _pn=dynamic_cast<_policy_lru_type<Key,Allocator>& >(_p);
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

    struct policy_lru {
        template <typename Key, template <typename T> class Allocator>
            struct bind : _policy_lru_type<Key,Allocator> { 
                bind(const bind& x) : _policy_lru_type<Key,Allocator>(x)  { }
                bind(const size_t& size) : _policy_lru_type<Key,Allocator>(size) { }
            };
    };
}

#endif /* STLCACHE_POLICY_LRU_HPP_INCLUDED */
