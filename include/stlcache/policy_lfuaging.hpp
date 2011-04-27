//
// Copyright (C) 2011 Denis V Chapligin
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
        _policy_lfuaging_type<Age,Key,Allocator>& operator= ( const _policy_lfuaging_type<Age,Key,Allocator>& x) throw() {
            _policy_lfu_type<Key,Allocator>::operator=(x);
            this->_timeKeeper=x._timeKeeper;
            this->_oldestEntry=x._oldestEntry;
            this->age=x.age;
            return *this;
        }
        _policy_lfuaging_type(const _policy_lfuaging_type<Age,Key,Allocator>& x)  throw() : _policy_lfu_type<Key,Allocator>(x) {
            *this=x;
        }
        _policy_lfuaging_type(const size_t& size ) throw() : _policy_lfu_type<Key,Allocator>(size) { 
            this->age=Age;
            this->_oldestEntry=time(NULL);
        }

        virtual void remove(const Key& _k) throw() {
            _timeKeeper.erase(_k);
            _policy_lfu_type<Key,Allocator>::remove(_k);
        }
        virtual void touch(const Key& _k) throw() { 
            _timeKeeper.erase(_k);
            _timeKeeper.insert(std::pair<Key,time_t>(_k,time(NULL))); //Because touch always increases the refcount, so it couldn't be 1 after touch
            _policy_lfu_type<Key,Allocator>::touch(_k);
        }   
        virtual void clear() throw() {
            _timeKeeper.clear();
            _policy_lfu_type<Key,Allocator>::clear();
        }
        virtual void swap(policy<Key,Allocator>& _p) throw(stlcache_invalid_policy) {
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
                throw stlcache_invalid_policy("Attempted to swap incompatible policies");
            }
        }
        virtual const _victim<Key> victim() throw()  {
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
                        unsigned long long currentRef=this->untouch((*it).first);
                        toErase.push_front((*it).first);
                        
                        if (currentRef>1) {
                            toInsert.push_front((*it).first);
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
    template <time_t Age> struct policy_lfuaging {
        template <typename Key, template <typename T> class Allocator>
            struct bind : _policy_lfuaging_type<Age,Key,Allocator> { 
                bind(const bind& x) : _policy_lfuaging_type<Age,Key,Allocator>(x),_policy_lfu_type<Key,Allocator>(x)  { }
                bind(const size_t& size) : _policy_lfuaging_type<Age,Key,Allocator>(size),_policy_lfu_type<Key,Allocator>(size) { }
            };
    };
}

#endif /* STLCACHE_POLICY_LFUAGING_HPP_INCLUDED */
