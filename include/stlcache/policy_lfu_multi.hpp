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
        };

        struct IndexKey { };
        struct IndexRefCount { };

        typedef boost::multi_index_container<
            boost::multi_index::hashed_unique<
                boost::multi_index::tag<IndexKey>,
                boost::multi_index::member<Reference, Key, Reference::key>
            >,
            boost::multi_index::ordered_non_unique<
                boost::multi_index::tag<IndexRefCount>,
                boost::multi_index::member<Reference, unsigned int, Reference::refCount>
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
            /*entriesIterator newEntryIter = _entries.insert(entriesPair(refCount,_k));
            _backEntries.insert(backEntriesPair(_k,newEntryIter));*/
        }
        virtual void insert(const Key& _k) throw(exception_invalid_key) {
            //1 - is initial reference value
            //this->insert(_k,1);
        }

        virtual void remove(const Key& _k) throw() {
            /*backEntriesIterator backIter = _backEntries.find(_k);
            if (backIter==_backEntries.end()) {
                return;
            }

            _entries.erase(backIter->second);
            _backEntries.erase(_k);*/
        }
        virtual void touch(const Key& _k) throw() { 
            /*backEntriesIterator backIter = _backEntries.find(_k);
            if (backIter==_backEntries.end()) {
                return;
            }

            unsigned int refCount=backIter->second->first;
            
            _entries.erase(backIter->second);
            entriesIterator entryIter=_entries.insert(entriesPair(refCount+1,_k));
            backIter->second=entryIter;*/
        }
        virtual void clear() throw() {
            /*_entries.clear();
            _backEntries.clear();*/
        }
        virtual void swap(policy<Key,Allocator>& _p) throw(exception_invalid_policy) {
            /*try {
                _policy_lfu_multi_type<Key,Allocator>& _pn=dynamic_cast<_policy_lfu_multi_type<Key,Allocator>& >(_p);
                _entries.swap(_pn._entries);
                _backEntries.swap(_pn._backEntries);
            } catch (const std::bad_cast& ) {
                throw exception_invalid_policy("Attempted to swap incompatible policies");
            }*/
        }

        virtual const _victim<Key> victim() throw()  {
            /*if (_entries.begin()==_entries.end()) {
                return _victim<Key>();
            }

            return _victim<Key>(_entries.begin()->second);*/
        }

    protected:
        const entriesType& entries() const {
            return this->_entries;
        }
        virtual unsigned long long untouch(const Key& _k) throw() { 
            /*backEntriesIterator backIter = _backEntries.find(_k);
            if (backIter==_backEntries.end()) {
                return 0;
            }
            unsigned int refCount=backIter->second->first;


            if (!(refCount>1)) {
                return refCount; //1 is a minimal reference value
            }


            _entries.erase(backIter->second);
            entriesIterator entryIter=_entries.insert(entriesPair(refCount-1,_k));
            backIter->second=entryIter;

            return refCount;*/
        }
    };

    /*!
     * \brief A 'Least Frequently Used' policy
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
