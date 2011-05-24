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
        typedef set<Key,less<Key>,Allocator<Key> > keySet;
        map<unsigned long long,keySet, less<unsigned long long>, Allocator<pair<const unsigned long long, keySet> > > _entries;
        map<Key, unsigned long long, less<Key>, Allocator<pair<const Key, unsigned long long> > > _backEntries;
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

        virtual void insert(const Key& _k) throw(exception_invalid_key) {
            //1 - is initial reference value
            keySet pad=_entries[1];
            pad.insert(_k);
            if (!_backEntries.insert(std::pair<Key,unsigned long long>(_k,1)).second) {
                throw exception_invalid_key("Key already cached!",_k);
            }

            _entries.erase(1);
            _entries.insert(std::pair<unsigned long long, keySet>(1,pad));
        }
        virtual void remove(const Key& _k) throw() {
            keySet pad = _entries[_backEntries[_k]];

            pad.erase(_k);
            _entries.erase(_backEntries[_k]);
            if (pad.size()>0) {
                _entries.insert(std::pair<unsigned long long, keySet>(_backEntries[_k],pad));
            }

            _backEntries.erase(_k);
        }
        virtual void touch(const Key& _k) throw() { 
            unsigned long long ref = _backEntries[_k];

            keySet pad = _entries[ref];
            pad.erase(_k);
            _entries.erase(ref);
			if (!pad.empty()) {
				_entries.insert(std::pair<unsigned long long, keySet>(ref,pad));
			}

            ref++;
			if (_entries.find(ref)!=_entries.end()) {
				pad = _entries[ref];
			} else {
				pad = keySet();
			}
            pad.insert(_k);
            _entries.erase(ref);
            _entries.insert(std::pair<unsigned long long, keySet>(ref,pad));

            _backEntries.erase(_k);
            _backEntries[_k]=ref;
        }
        virtual void clear() throw() {
            _entries.clear();
            _backEntries.clear();
        }
        virtual void swap(policy<Key,Allocator>& _p) throw(exception_invalid_policy) {
            try {
                _policy_lfu_type<Key,Allocator>& _pn=dynamic_cast<_policy_lfu_type<Key,Allocator>& >(_p);
                _entries.swap(_pn._entries);
                _backEntries.swap(_pn._backEntries);
            } catch (const std::bad_cast& ) {
                throw exception_invalid_policy("Attempted to swap incompatible policies");
            }
        }

        virtual const _victim<Key> victim() throw()  {
            if (_entries.begin()==_entries.end()) {
                return _victim<Key>();
            }
            keySet pad=(*(_entries.begin())).second; //Begin returns entry with lowest id, just what we need:)

            if (pad.begin()==pad.end()) {
                return _victim<Key>();
            }

            return _victim<Key>(*(pad.begin()));
        }

    protected:
        const map<unsigned long long,keySet >& entries() const {
            return this->_entries;
        }
        virtual unsigned long long untouch(const Key& _k) throw() { 
            unsigned long long ref = _backEntries[_k];

            if (!(ref>1)) {
                return ref; //1 is a minimal reference value
            }

            keySet pad = _entries[ref];
            pad.erase(_k);
            _entries.erase(ref);
            _entries.insert(std::pair<unsigned long long, keySet>(ref,pad));

            ref--;
            pad = _entries[ref];
            pad.insert(_k);
            _entries.erase(ref);
            _entries.insert(std::pair<unsigned long long, keySet>(ref,pad));

            _backEntries.erase(_k);
            _backEntries[_k]=ref;

            return ref;
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
    struct policy_lfu {
        template <typename Key, template <typename T> class Allocator>
            struct bind : _policy_lfu_type<Key,Allocator> { 
                bind(const bind& x) : _policy_lfu_type<Key,Allocator>(x)  { }
                bind(const size_t& size) : _policy_lfu_type<Key,Allocator>(size) { }
            };
    };
}

#endif /* STLCACHE_POLICY_LFU_HPP_INCLUDED */
