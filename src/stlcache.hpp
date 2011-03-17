#ifndef STLCACHE_STLCACHE_HPP_INCLUDED
#define STLCACHE_STLCACHE_HPP_INCLUDED

#include <map>

using namespace std;

#include "exceptions.hpp"

namespace stlcache {
    template<class Key, class Data, class Policy, class Compare = less<Key>, class Allocator = allocator<pair<const Key, Data> > >
    class cache {
        typedef map<Key,Data,Compare,Allocator> storageType; 
         storageType _storage;
         std::size_t _maxEntries;

    public:
        typedef Key                                                                key_type;
        typedef Data                                                               mapped_type;
        typedef pair<const Key, Data>                                         value_type;
        typedef Compare                                                          key_compare;
        typedef Allocator                                                          allocator_type;
        typedef typename storageType::value_compare                                value_compare;
        typedef typename storageType::reference                                        reference;
        typedef typename storageType::const_reference                               const_reference;
        typedef typename storageType::size_type                                       size_type;
        typedef typename storageType::difference_type                               difference_type;
        typedef typename storageType::pointer                                          pointer;
        typedef typename storageType::const_pointer                                 const_pointer;

        //map functions wrappers
        allocator_type get_allocator() const throw() {
            return _storage.get_allocator();
        }

        size_type count ( const key_type& x ) const throw() {
            return _storage.count(x);
        }

        value_compare value_comp ( ) const throw() {
            return _storage.value_comp();
        }

        key_compare key_comp ( ) const throw() {
            return _storage.key_comp();
        }

        //Cache API
        void clear() throw() {
            _storage.clear();
        }

        void swap ( cache<Key,Data,Policy,Compare,Allocator>& mp ) throw() {
            _storage.swap(mp._storage);
            std::size_t m=this->_maxEntries;
            this->_maxEntries=mp._maxEntries;
            mp._maxEntries=m;
        }

        size_type erase ( const key_type& x ) throw() {
            return _storage.erase(x);
        }

        bool insert(Key _k, Data _d) throw() {
            return _storage.insert(value_type(_k,_d)).second;
        }

        size_type max_size() const throw() {
            return this->_maxEntries;
        }

        size_type size() const throw() {
            return _storage.size();
        }

        bool empty() const throw() {
            return _storage.empty();
        }

        const Data& fetch(const Key& _k) throw(stlcache_invalid_key) {
            if (!check(_k)) {
                throw stlcache_invalid_key("Key is not in cache",_k);
            }
            return (*(_storage.find(_k))).second;
        }

        const bool check(const Key& _k) throw() {
            return _storage.find(_k)!=_storage.end();
        }

        //Container interface
        cache<Key,Data,Policy,Compare,Allocator>& operator= ( const cache<Key,Data,Policy,Compare,Allocator>& x) throw() {
            this->_storage=x._storage;
            this->_maxEntries=x._maxEntries;
        }
        explicit cache(const size_type size, const Compare& comp = Compare(), const Allocator& alloc = Allocator()) throw() {
            this->_storage=storageType(comp,alloc);
            this->_maxEntries=size;
        }
        cache(const cache<Key,Data,Policy,Compare,Allocator>& x) throw() {
            *this=x;
        }
    };
}

#endif /* STLCACHE_STLCACHE_HPP_INCLUDED */
