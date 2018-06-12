//
// Copyright (C) 2011-2017 Denis V Chapligin, Martin Hrabovsky
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STLCACHE_POLICY_ADAPTIVE_HPP_INCLUDED
#define STLCACHE_POLICY_ADAPTIVE_HPP_INCLUDED

#include <list>

using namespace std;

#include <stlcache/policy.hpp>

namespace stlcache {
    template <class Key,template <typename T> class Allocator> class _policy_adaptive_type : public policy<Key,Allocator> {
        size_t _size;
        _policy_lru_type<Key,Allocator> T1;
        set<Key,less<Key>,Allocator<Key> > t1Entries;
        _policy_lfu_type<Key,Allocator> T2;
        set<Key,less<Key>,Allocator<Key> > t2Entries;

        _policy_lru_type<Key,Allocator> B1;
        set<Key,less<Key>,Allocator<Key> > b1Entries;
        _policy_lfu_type<Key,Allocator> B2;
        set<Key,less<Key>,Allocator<Key> > b2Entries;

    public:
        _policy_adaptive_type<Key,Allocator>& operator= ( const _policy_adaptive_type<Key,Allocator>& x) {
            this->_size=x._size;

            this->T1=x.T1;
            this->t1Entries=x.t1Entries;
            this->B1=x.B1;
            this->b1Entries=x.b1Entries;
            this->T2=x.T2;
            this->t2Entries=x.t2Entries;
            this->B2=x.B2;
            this->b2Entries=x.b2Entries;

            return *this;
        }
        _policy_adaptive_type(const _policy_adaptive_type<Key,Allocator>& x) : T1(x.T1),T2(x.T2),B1(x.B1),B2(x.B2) {
            this->_size=x._size;

            this->t1Entries=x.t1Entries;
            this->b1Entries=x.b1Entries;
            this->t2Entries=x.t2Entries;
            this->b2Entries=x.b2Entries;
        }
        _policy_adaptive_type(const size_t& size ) : T1(size),T2(size),B1(size),B2(size) {
            this->_size=size;
        }

        virtual void insert(const Key& _k) {
            if (b1Entries.find(_k)!=b1Entries.end()) {
                //Check, whether we are outsized
                b1Entries.erase(_k);
                B1.remove(_k);

                t1Entries.insert(_k);
                T1.insert(_k);
            } else if (b2Entries.find(_k)!=b2Entries.end()) {
                B2.remove(_k);
                b2Entries.erase(_k);

                t2Entries.insert(_k);
                T2.insert(_k);
            } else {
                //Completely new!

                t1Entries.insert(_k);
                T1.insert(_k);
            }
        }
        virtual void remove(const Key& _k) {
            if (t1Entries.find(_k)!=t1Entries.end()) {
                B1.insert(_k);
                b1Entries.insert(_k);

                if (b1Entries.size()>=_size/2) {
                    _victim<Key> bKey=B1.victim();
                    B1.remove(*bKey);
                    b1Entries.erase(*bKey);
                }
                t1Entries.erase(_k);
                T1.remove(_k);
            } else {
                B2.insert(_k);
                b2Entries.insert(_k);

                if (b2Entries.size()>=_size/2) {
                    _victim<Key> bKey=B2.victim();
                    B2.remove(*bKey);
                    b2Entries.erase(*bKey);
                }
                t2Entries.erase(_k);
                T2.remove(_k);
            }
        }
        virtual void touch(const Key& _k) {
            if (t1Entries.find(_k)!=t1Entries.end()) {
                t1Entries.erase(_k);
                T1.remove(_k);
                t2Entries.insert(_k);
                T2.insert(_k);
            } else {
                T2.touch(_k);
            }
        }
        virtual void clear() {
            T1.clear();
            t1Entries.clear();
            T2.clear();
            t2Entries.clear();
            B1.clear();
            b1Entries.clear();
            B2.clear();
            b2Entries.clear();
        }
        virtual void swap(policy<Key,Allocator>& _p) {
            try {
                _policy_adaptive_type<Key,Allocator>& _pn=dynamic_cast<_policy_adaptive_type<Key,Allocator>& >(_p);
                T1.swap(_pn.T1);
                T2.swap(_pn.T2);
                B1.swap(_pn.B1);
                B2.swap(_pn.B2);

                size_t _oldSize=_size;
                _size=_pn._size;
                _size=_oldSize;
            } catch (const std::bad_cast& ) {
                throw exception_invalid_policy("Attempted to swap incompatible policies");
            }
        }

        virtual const _victim<Key> victim()  {
            if (t1Entries.size()>t2Entries.size()) {
                return T1.victim();
            } else {
                return T2.victim();
            }
        }
    };

    /*!
     * \brief A 'Adaptive replacement' policy
     *
     * Implements <a href="http://en.wikipedia.org/wiki/Adaptive_replacement_cache">'Adaptive replacement'</a> cache algorithm.
     *
     * The adaptive cache algorithm is balancing between internal LRU and LFU caches, trying to adapt to the external expectations on it.
     *
     * \link cache::touch Touching \endlink the entry decreases item's expiration probability. This policy is always able to expire any amount of entries.
     *
     * No additional configuration is required.
     *
     */
    struct policy_adaptive {
        template <typename Key, template <typename T> class Allocator>
            struct bind : _policy_adaptive_type<Key,Allocator> {
                bind(const bind& x) : _policy_adaptive_type<Key,Allocator>(x)  { }
                bind(const size_t& size) : _policy_adaptive_type<Key,Allocator>(size) { }
            };
    };
}

#endif /* STLCACHE_POLICY_ADAPTIVE_HPP_INCLUDED */
