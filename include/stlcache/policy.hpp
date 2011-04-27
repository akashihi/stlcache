//
// Copyright (C) 2011 Denis V Chapligin
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STLCACHE_POLICY_HPP_INCLUDED
#define STLCACHE_POLICY_HPP_INCLUDED

#include <set>

using namespace std;

#include <stlcache/exceptions.hpp>
#include <stlcache/victim.hpp>

namespace stlcache {
    template <class Key,template <typename T> class Allocator> class policy {
    public:
        virtual void insert(const Key& _k) throw(stlcache_invalid_key) =0;
        virtual void remove(const Key& _k) throw() =0;
        virtual void touch(const Key& _k) throw() =0;
        virtual void clear() throw() =0;
        virtual void swap(policy<Key,Allocator>& _p) throw(stlcache_invalid_policy)=0;

        virtual const _victim<Key> victim() throw()  =0;
    };

    template <class Key, template <typename T> class Allocator> class _policy_none_type : public policy<Key,Allocator> {
        set<Key,less<Key>,Allocator<Key> > _entries;
    public:
        _policy_none_type<Key,Allocator>& operator= ( const _policy_none_type<Key,Allocator>& x) throw() {
            this->_entries=x._entries;
            return *this;
        }
        _policy_none_type(const _policy_none_type<Key,Allocator>& x) throw() {
            *this=x;
        }
        _policy_none_type(const size_t& size ) throw() { }

        virtual void insert(const Key& _k) throw(stlcache_invalid_key) {
            _entries.insert(_k);
        }
        virtual void remove(const Key& _k) throw() {
            _entries.erase(_k);
        }
        virtual void touch(const Key& _k) throw() { /* Not used here */  }
        virtual void clear() throw() {
            _entries.clear();
        }
        virtual void swap(policy<Key,Allocator>& _p) throw(stlcache_invalid_policy) {
            try {
                _policy_none_type<Key,Allocator>& _pn=dynamic_cast<_policy_none_type<Key,Allocator>& >(_p);
                _entries.swap(_pn._entries);
            } catch (const std::bad_cast& ) {
                throw stlcache_invalid_policy("Attempted to swap incompatible policies");
            }
        }

        virtual const _victim<Key> victim() throw() {
            if (_entries.rbegin()==_entries.rend()) {
                return _victim<Key>();
            }
            return _victim<Key>(*(_entries.rbegin()));
        }
    };

    struct policy_none {
        template <typename Key, template <typename T> class Allocator>
            struct bind : _policy_none_type<Key,Allocator> { 
                bind(const bind& x) : _policy_none_type<Key,Allocator>(x)  { }
                bind(const size_t& size) : _policy_none_type<Key,Allocator>(size) { }
            };
    };
}

#endif /* STLCACHE_POLICY_HPP_INCLUDED */
