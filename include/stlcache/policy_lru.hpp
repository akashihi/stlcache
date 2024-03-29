//
// Copyright (C) 2011-2023 Denis V Chapligin, Martin Hrabovsky, Vojtech Ondruj
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STLCACHE_POLICY_LRU_HPP_INCLUDED
#define STLCACHE_POLICY_LRU_HPP_INCLUDED

#include <list>
#include <unordered_map>

#include <stlcache/policy.hpp>

namespace stlcache {
    template <class Key,template <typename T> class Allocator> class _policy_lru_type : public policy<Key,Allocator> {
        std::list<Key,Allocator<Key> > _entries;
        typedef typename std::list<Key,Allocator<Key> >::iterator entriesIterator;

        std::unordered_map<Key,entriesIterator,std::hash<Key>,std::equal_to<Key>,Allocator<std::pair<Key,entriesIterator> > > _entriesMap;
    public:
        _policy_lru_type<Key,Allocator>& operator= ( const _policy_lru_type<Key,Allocator>& x) {
            this->_entries=x._entries;
            _entriesMap.clear();
            for(auto entryIter=_entries.begin();entryIter!=_entries.end();++entryIter) {
              _entriesMap.insert(std::make_pair(*entryIter,entryIter));
            }
            return *this;
        }
        _policy_lru_type(const _policy_lru_type<Key,Allocator>& x): _entries(x._entries), _entriesMap(std::unordered_map<Key,entriesIterator,std::hash<Key>,std::equal_to<Key>,Allocator<std::pair<Key,entriesIterator> > >()) {
            *this = x;
        }
        explicit _policy_lru_type(const size_t& ): _entries(std::list<Key,Allocator<Key> >()), _entriesMap(std::unordered_map<Key,entriesIterator,std::hash<Key>,std::equal_to<Key>,Allocator<std::pair<Key,entriesIterator> > >()){ }

        virtual void insert(const Key& _k) {
            auto entryIter = _entries.insert(_entries.begin(),_k);
            _entriesMap.insert(std::make_pair(_k,entryIter));
        }
        virtual void remove(const Key& _k) {
            auto mapIter = _entriesMap.find(_k);
            if (mapIter==_entriesMap.end()) {
                return;
            }
            _entries.erase(mapIter->second);
            _entriesMap.erase(mapIter);
        }
        virtual void touch(const Key& _k) {
            auto mapIter = _entriesMap.find(_k);
            if (mapIter==_entriesMap.end()) {
                return;
            }
            _entries.erase(mapIter->second);
            auto entryIter = _entries.insert(_entries.begin(),_k);
            mapIter->second=entryIter;
        }
        virtual void clear() {
            _entries.clear();
            _entriesMap.clear();
        }
        virtual void swap(policy<Key,Allocator>& _p) {
            try {
                auto& _pn=dynamic_cast<_policy_lru_type<Key,Allocator>& >(_p);
                _entries.swap(_pn._entries);
                _entriesMap.swap(_pn._entriesMap);
            } catch (const std::bad_cast& ) {
                throw exception_invalid_policy("Attempted to swap incompatible policies");
            }
        }

        virtual const _victim<Key> victim()  {
            return _victim<Key>(_entries.back());
        }

    protected:
        const std::list<Key>& entries() const  { return this->_entries; }
    };

    /*!
     * \brief A 'Least Recently Used' policy
     *
     * Implements <a href="http://en.wikipedia.org/wiki/Cache_algorithms#Least_Recently_Used">'Least Recently Used'</a> cache expiration algorithm.
     *
     * The LRU policy tracks the usage of items and moves the recently used items to the front of the items stack and select items for the expiration from the end
     * of the stack. \link cache::touch Touching \endlink the entry decreases item's expiration probability. This policy is always able to expire any amount of entries.
     *
     * No additional configuration is required.
     */
    struct policy_lru {
        template <typename Key, template <typename T> class Allocator>
            struct bind : _policy_lru_type<Key,Allocator> {
                bind(const bind& x) : _policy_lru_type<Key,Allocator>(x)  { }
                explicit bind(const size_t& size) : _policy_lru_type<Key,Allocator>(size) { }
            };
    };
}

#endif /* STLCACHE_POLICY_LRU_HPP_INCLUDED */
