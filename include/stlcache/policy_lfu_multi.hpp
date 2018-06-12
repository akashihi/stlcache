//
// Copyright (C) 2011-2017 Denis V Chapligin, Martin Hrabovsky
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STLCACHE_POLICY_LFU_MULTI_HPP_INCLUDED
#define STLCACHE_POLICY_LFU_MULTI_HPP_INCLUDED

#include <set>
#include <map>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>

using namespace std;

#include <stlcache/policy.hpp>

namespace stlcache {
    template <class Key,template <typename T> class Allocator> class _policy_lfu_multi_type : public policy<Key,Allocator> {
    protected:
        struct Reference {
            Key key;
            unsigned int refCount;
            Reference(Key k, unsigned int r): key(k), refCount(r) { };
            bool operator<(const Reference& r) const { return refCount < r.refCount; }
        };

        struct IndexKey{};
        struct IndexRefCount{};

        typedef boost::multi_index_container<
            Reference,
            boost::multi_index::indexed_by<
                boost::multi_index::ordered_unique<
                    boost::multi_index::tag<IndexKey>,
                    boost::multi_index::member<Reference, Key, &Reference::key>
                >,
                boost::multi_index::ordered_non_unique<
                    boost::multi_index::tag<IndexRefCount>,
                    boost::multi_index::member<Reference, unsigned int, &Reference::refCount>
                >
            >
        > entriesType;

        entriesType _entries;        

    public:
        _policy_lfu_multi_type<Key,Allocator>& operator= ( const _policy_lfu_multi_type<Key,Allocator>& x) throw() {
            this->_entries=x._entries;
            return *this;
        }
        _policy_lfu_multi_type(const _policy_lfu_multi_type<Key,Allocator>& x) throw() {
            *this=x;
        }
        _policy_lfu_multi_type(const size_t& size ) throw() { }

        virtual void insert(const Key& _k,unsigned int refCount) throw(exception_invalid_key) {
            //1 - is initial reference value
            _entries.insert({_k,refCount});
        }
        virtual void insert(const Key& _k) throw(exception_invalid_key) {
            //1 - is initial reference value
            this->insert(_k,1);
        }

        virtual void remove(const Key& _k) throw() {
            auto itFound = _entries.find(_k);
            if(itFound == _entries.end())
            {
                return;
            }
            _entries.erase(_k);
        }
        virtual void touch(const Key& _k) throw() {
            auto itFound = _entries.find(_k);
            if(itFound == _entries.end())
            {
                return;
            }
            Reference r = *itFound;
            r.refCount+=1;
            _entries.replace(itFound, r);         
        }
        virtual void clear() throw() {
            _entries.clear();
        }
        virtual void swap(policy<Key,Allocator>& _p) throw(exception_invalid_policy) {
            try {
                _policy_lfu_multi_type<Key,Allocator>& _pn=dynamic_cast<_policy_lfu_multi_type<Key,Allocator>& >(_p);
                _entries.swap(_pn._entries);
            } catch (const std::bad_cast& ) {
                throw exception_invalid_policy("Attempted to swap incompatible policies");
            }
        }

        virtual const _victim<Key> victim() throw()  {
            //
            if (_entries.begin()==_entries.end()) {
                return _victim<Key>();
            }

            const auto& smallest = _entries.template get<IndexRefCount>().begin();;
            auto itFound = _entries.template project<0>(smallest);
            return _victim<Key>(itFound->key);
            
        }

    protected:
        const entriesType& entries() const {
            return this->_entries;
        }
        virtual unsigned long long untouch(const Key& _k) throw() {
            auto itFound = _entries.find(_k);
            Reference r = *itFound;
            if(itFound == _entries.end())
            {
                return 0;
            }
            unsigned int refCount = itFound->refCount;
            if (!(refCount>1))
            {
                return refCount;
            }
            r.refCount-=1;
            _entries.replace(itFound,r);
            return refCount;
        }
    };

    /*!
     * \brief A 'Least Frequently Used' policy, Multi-index implementation
     * 
     * Implements <a href="http://en.wikipedia.org/wiki/Least_frequently_used">'Least Frequently Used'</a> cache algorithm. 
     *  
     * The LFU policy tracks how many times entry was used and selects entries with the smallest usage count for expiration. There is a big difference 
     * between LFU and \link stlcache::policy_lru LRU \endlink policies - the LRU policy only tracks the fact of entry usage, but the LFU also takes in
     * the account the number of entry usages. 
     * \link cache::touch Touching \endlink the entry greatly decreases item's expiration probability. This policy is always able to expire any amount of entries. 
     *  
     * No additional configuration is required. 
     *
     * This policy usese multi index map by Boost and, therefore, is only available, when Boost is available. Comparing to the \link stlcache::policy_lfu LFU \endlink
     * implementation, this one is about 2.5 times slower, but requires about 2 times less
     *  
     * \see policy_lfu
     * \see policy_lfustar
     * \see policy_lfuaging 
     * \see policy_lfuagingstar 
     *  
     */
    struct policy_lfu_multi {
        template <typename Key, template <typename T> class Allocator>
            struct bind : _policy_lfu_multi_type<Key,Allocator> { 
                bind(const bind& x) : _policy_lfu_multi_type<Key,Allocator>(x)  { }
                bind(const size_t& size) : _policy_lfu_multi_type<Key,Allocator>(size) { }
            };
    };
}

#endif /* STLCACHE_POLICY_LFU_MULTI_HPP_INCLUDED */
