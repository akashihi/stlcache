//
// Copyright (C) 2011 Denis V Chapligin
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STLCACHE_POLICY_LFU_HPP_INCLUDED
#define STLCACHE_POLICY_LFU_HPP_INCLUDED

#include <set>
#include <map>

using namespace std;

#include <stlcache/policy.hpp>

namespace stlcache {
    template <class Key,template <typename T> class Allocator> class _policy_lfu_type : public policy<Key,Allocator> {
    protected:
        typedef pair<const unsigned int, Key> entriesPair;
        typedef multimap<unsigned int, Key,less<Key> ,Allocator<entriesPair> > entriesType;
        entriesType _entries;

        typedef typename entriesType::iterator entriesIterator;
        typedef pair<const Key,entriesIterator> backEntriesPair;
        typedef map<Key,entriesIterator,less<Key>,Allocator<backEntriesPair> > backEntriesType;
        backEntriesType _backEntries;
        typedef typename backEntriesType::iterator backEntriesIterator;
        

    public:
        _policy_lfu_type<Key,Allocator>& operator= ( const _policy_lfu_type<Key,Allocator>& x) throw() {
            this->_entries=x._entries;
            this->_backEntries=x._backEntries;
            return *this;
        }
        _policy_lfu_type(const _policy_lfu_type<Key,Allocator>& x) throw() {
            *this=x;
        }
        _policy_lfu_type(const size_t& size ) throw() { }

        virtual void insert(const Key& _k,unsigned int refCount) throw(stlcache_invalid_key) {
            //1 - is initial reference value
            entriesIterator newEntryIter = _entries.insert(entriesPair(refCount,_k));
            _backEntries.insert(backEntriesPair(_k,newEntryIter));
        }
        virtual void insert(const Key& _k) throw(stlcache_invalid_key) {
            //1 - is initial reference value
            this->insert(_k,1);
        }

        virtual void remove(const Key& _k) throw() {
            backEntriesIterator backIter = _backEntries.find(_k);
            if (backIter==_backEntries.end()) {
                return;
            }

            entriesIterator oldEntryIter = backIter->second;
            _backEntries.erase(_k);
            _entries.erase(oldEntryIter);
        }
        virtual void touch(const Key& _k) throw() { 
            backEntriesIterator backIter = _backEntries.find(_k);
            if (backIter==_backEntries.end()) {
                return;
            }

            unsigned int refCount=backIter->second->first;
            
            this->remove(_k);
            this->insert(_k,refCount+1);
        }
        virtual void clear() throw() {
            _entries.clear();
            _backEntries.clear();
        }
        virtual void swap(policy<Key,Allocator>& _p) throw(stlcache_invalid_policy) {
            try {
                _policy_lfu_type<Key,Allocator>& _pn=dynamic_cast<_policy_lfu_type<Key,Allocator>& >(_p);
                _entries.swap(_pn._entries);
                _backEntries.swap(_pn._backEntries);
            } catch (const std::bad_cast& ) {
                throw stlcache_invalid_policy("Attempted to swap incompatible policies");
            }
        }

        virtual const _victim<Key> victim() throw()  {
            if (_entries.begin()==_entries.end()) {
                return _victim<Key>();
            }

            return _victim<Key>(_entries.begin()->second);
        }

    protected:
        const entriesType& entries() const {
            return this->_entries;
        }
        virtual unsigned long long untouch(const Key& _k) throw() { 
            backEntriesIterator backIter = _backEntries.find(_k);
            if (backIter==_backEntries.end()) {
                return 0;
            }
            unsigned int refCount=backIter->second->first;


            if (!(refCount>1)) {
                return refCount; //1 is a minimal reference value
            }

            this->remove(_k);
            this->insert(_k,refCount-1);

            return refCount;
        }
    };
    struct policy_lfu {
        template <typename Key, template <typename T> class Allocator>
            struct bind : _policy_lfu_type<Key,Allocator> { 
                bind(const bind& x) : _policy_lfu_type<Key,Allocator>(x)  { }
                bind(const size_t& size) : _policy_lfu_type<Key,Allocator>(size) { }
            };
    };
}

#endif /* STLCACHE_POLICY_LFU_HPP_INCLUDED */
