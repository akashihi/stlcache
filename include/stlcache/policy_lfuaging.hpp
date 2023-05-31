//
// Copyright (C) 2011-2023 Denis V Chapligin, Martin Hrabovsky, Vojtech Ondruj
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STLCACHE_POLICY_LFUAGING_HPP_INCLUDED
#define STLCACHE_POLICY_LFUAGING_HPP_INCLUDED

#include <map>
#include <chrono>

#include <stlcache/policy.hpp>

namespace stlcache {
    template <unsigned int Age,class Key,template <typename T> class Allocator> class _policy_lfuaging_type : public virtual _policy_lfu_type<Key,Allocator> {
        typedef std::map<Key,std::chrono::time_point<std::chrono::steady_clock>, std::less<Key>, Allocator<std::pair<const Key, std::chrono::time_point<std::chrono::steady_clock> > > > timeKeeperType;
        timeKeeperType _timeKeeper;
        std::chrono::time_point<std::chrono::steady_clock> _oldestEntry;
        std::chrono::seconds age;
    public:
        _policy_lfuaging_type<Age,Key,Allocator>& operator= ( const _policy_lfuaging_type<Age,Key,Allocator>& x) {
            _policy_lfu_type<Key,Allocator>::operator=(x);
            this->_timeKeeper=x._timeKeeper;
            this->_oldestEntry=x._oldestEntry;
            this->age=x.age;
            return *this;
        }
        _policy_lfuaging_type(const _policy_lfuaging_type<Age,Key,Allocator>& x)  : _policy_lfu_type<Key,Allocator>(x), _timeKeeper(x._timeKeeper), _oldestEntry(x._oldestEntry), age(x.age) { }
        explicit _policy_lfuaging_type(const size_t& size ) : _policy_lfu_type<Key,Allocator>(size), age(Age), _oldestEntry(std::chrono::steady_clock::now()), _timeKeeper(timeKeeperType()) { }

        virtual void insert(const Key& _k) {
            _policy_lfu_type<Key,Allocator>::insert(_k);
            _timeKeeper.insert(std::make_pair(_k,std::chrono::steady_clock::now()));
        }
        virtual void remove(const Key& _k) {
            _policy_lfu_type<Key,Allocator>::remove(_k);
            _timeKeeper.erase(_k);
        }
        virtual void touch(const Key& _k) {
            _policy_lfu_type<Key,Allocator>::touch(_k);
            auto item = _timeKeeper.find(_k);
            if (item != _timeKeeper.end()) {
                item->second = std::chrono::steady_clock::now();
            }
        }
        virtual void clear() {
            _policy_lfu_type<Key,Allocator>::clear();
            _timeKeeper.clear();
        }
        virtual void swap(policy<Key,Allocator>& _p) {
            try {
                auto& _pn=dynamic_cast<_policy_lfuaging_type<Age,Key,Allocator>& >(_p);
                _timeKeeper.swap(_pn._timeKeeper);

                std::chrono::time_point<std::chrono::steady_clock> _oldest=this->_oldestEntry;
                this->_oldestEntry=_pn._oldestEntry;
                _pn._oldestEntry=_oldest;

                std::chrono::seconds a = this->age;
                this->age=_pn.age;
                _pn.age=a;

                _policy_lfu_type<Key,Allocator>::swap(_pn);
            } catch (const std::bad_cast& ) {
                throw exception_invalid_policy("Attempted to swap incompatible policies");
            }
        }
        virtual const _victim<Key> victim()  {
			this->expire();
            return _policy_lfu_type<Key,Allocator>::victim();
        }
	protected:
		virtual void expire() {
            auto oldestEntryAge = std::chrono::steady_clock::now()-_oldestEntry;
            if (oldestEntryAge>age) {

                //Time to clean up
                this->_oldestEntry=std::chrono::steady_clock::now();
                auto it = _timeKeeper.begin();
                while (it != _timeKeeper.end()) {
                    if ((std::chrono::steady_clock::now()-(*it).second)>age) {
                        //Too old :(
                        unsigned long long currentRef=this->untouch((*it).first);

                        if (currentRef>1) {
                            it->second = std::chrono::steady_clock::now(); //For expired, but still referenced items - just update age
                            it++;
                        } else {
                            it = _timeKeeper.erase(it); //Otherwise - remove it
                        }
                    } else {//Use non-expired items as the _oldestEntry
                        if ((*it).second<this->_oldestEntry) {
                            this->_oldestEntry=(*it).second;
                        }
                        it++;
                    }
                }
            }
		}
    };

    /*!
     * \brief A 'LFU-Aging' policy
     *
     * Implements <a href="http://en.wikipedia.org/wiki/Least_frequently_used">'LFU-Aging'</a> cache algorithm.
     *
     * A modified \link stlcache::policy_lfu LFU \endlink policy implementation, that allows the reference count to move in both directions.
     * Opposed to the usual \link stlcache::policy_lfu LFU \endlink, that allows only growth of the entries references counts, the LFU-Aging
     * also decreases the reference count of an entry, that was put into the cache some time ago.
     *
     * So, when you put an entry into the cache and start using it, entry's reference count will increase on every usage.
     * At the same time, the LFU-Aging algorithm will be applying the 'aging interval' on the entries in cache and
     * decrease entry's reference count every time. So the entries, that were popular in the past, but not needed right now, will
     * get a better chance to get out of the cache, then with usual LFU algorithm.
     *
     * \link cache::touch Touching \endlink the entry may not change item's expiration probability. This policy is always able to expire any amount of entries.
     *
     * The policy must be configured with the length of a aging interval:
     *
     * \tparam <Age>  aging interval in seconds
     *
     * \see policy_lfu
     * \see policu_lfuagingstar
     */
    template <unsigned int Age> struct policy_lfuaging {
        template <typename Key, template <typename T> class Allocator>
            struct bind : _policy_lfuaging_type<Age,Key,Allocator> {
                bind(const bind& x) : _policy_lfuaging_type<Age,Key,Allocator>(x),_policy_lfu_type<Key,Allocator>(x)  { }
                explicit bind(const size_t& size) : _policy_lfuaging_type<Age,Key,Allocator>(size),_policy_lfu_type<Key,Allocator>(size) { }
            };
    };
}

#endif /* STLCACHE_POLICY_LFUAGING_HPP_INCLUDED */
