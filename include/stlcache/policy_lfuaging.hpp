//
// Copyright (C) 2011-2017 Denis V Chapligin, Martin Hrabovsky
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STLCACHE_POLICY_LFUAGING_HPP_INCLUDED
#define STLCACHE_POLICY_LFUAGING_HPP_INCLUDED

#include <set>
#include <map>
#include <ctime>

using namespace std;

#include <stlcache/policy.hpp>

namespace stlcache {
    template <time_t Age,class Key,template <typename T> class Allocator> class _policy_lfuaging_type : public virtual _policy_lfu_type<Key,Allocator> {
        typedef set<Key, less<Key>, Allocator<Key> > keySet;
        map<Key,time_t, less<Key>, Allocator<pair<const Key, time_t> > > _timeKeeper;
        time_t _oldestEntry;
        time_t age;
    public:
        _policy_lfuaging_type<Age,Key,Allocator>& operator= ( const _policy_lfuaging_type<Age,Key,Allocator>& x) {
            _policy_lfu_type<Key,Allocator>::operator=(x);
            this->_timeKeeper=x._timeKeeper;
            this->_oldestEntry=x._oldestEntry;
            this->age=x.age;
            return *this;
        }
        _policy_lfuaging_type(const _policy_lfuaging_type<Age,Key,Allocator>& x)  : _policy_lfu_type<Key,Allocator>(x) {
            *this=x;
        }
        _policy_lfuaging_type(const size_t& size ) : _policy_lfu_type<Key,Allocator>(size) {
            this->age=Age;
            this->_oldestEntry=time(NULL);
        }

        virtual void insert(const Key& _k) {
            _policy_lfu_type<Key,Allocator>::insert(_k);
            _timeKeeper.insert(std::pair<Key,time_t>(_k,time(NULL))); //Because touch always increases the refcount, so it couldn't be 1 after touch
        }
        virtual void remove(const Key& _k) {
            _policy_lfu_type<Key,Allocator>::remove(_k);
            _timeKeeper.erase(_k);
        }
        virtual void touch(const Key& _k) {
            _policy_lfu_type<Key,Allocator>::touch(_k);
            _timeKeeper.erase(_k);
            _timeKeeper.insert(std::pair<Key,time_t>(_k,time(NULL))); //Because touch always increases the refcount, so it couldn't be 1 after touch
        }
        virtual void clear() {
            _policy_lfu_type<Key,Allocator>::clear();
            _timeKeeper.clear();
        }
        virtual void swap(policy<Key,Allocator>& _p) {
            try {
                _policy_lfuaging_type<Age,Key,Allocator>& _pn=dynamic_cast<_policy_lfuaging_type<Age,Key,Allocator>& >(_p);
                _timeKeeper.swap(_pn._timeKeeper);

                time_t _oldest=this->_oldestEntry;
                this->_oldestEntry=_pn._oldestEntry;
                _pn._oldestEntry=_oldest;

                time_t a = this->age;
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
            if ((_oldestEntry+age)<time(NULL)) {
                list<Key,Allocator<Key> > toErase;
                list<Key,Allocator<Key> > toInsert;

                //Time to clean up
                this->_oldestEntry=time(NULL);
                typedef typename map<Key,time_t,less<Key>,Allocator<pair<const Key,time_t> > >::iterator timeMapIterator;
                for (timeMapIterator it=_timeKeeper.begin();it!=_timeKeeper.end();++it) {
                    if ((*it).second+age<time(NULL)) {
                        //Too old :(
                        Key itCopy=(*it).first;
                        unsigned long long currentRef=this->untouch((*it).first);
                        toErase.push_front(itCopy);

                        if (currentRef>1) {
                            toInsert.push_front(itCopy);
                        }
                    } else {
                        if ((*it).second<this->_oldestEntry) {
                            this->_oldestEntry=(*it).second;
                        }
                    }
                }

                //Delete entries
                typedef typename list<Key,Allocator<Key> >::iterator listIterator;
                for(listIterator it=toErase.begin();it!=toErase.end();++it) {
                    _timeKeeper.erase(*it);
                }
                for(listIterator it=toInsert.begin();it!=toInsert.end();++it) {
                    _timeKeeper.insert(std::pair<Key,time_t>(*it,time(NULL)));
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
    template <time_t Age> struct policy_lfuaging {
        template <typename Key, template <typename T> class Allocator>
            struct bind : _policy_lfuaging_type<Age,Key,Allocator> {
                bind(const bind& x) : _policy_lfuaging_type<Age,Key,Allocator>(x),_policy_lfu_type<Key,Allocator>(x)  { }
                bind(const size_t& size) : _policy_lfuaging_type<Age,Key,Allocator>(size),_policy_lfu_type<Key,Allocator>(size) { }
            };
    };
}

#endif /* STLCACHE_POLICY_LFUAGING_HPP_INCLUDED */
