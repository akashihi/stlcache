#ifndef STLCACHE_POLICY_LFUAGING_HPP_INCLUDED
#define STLCACHE_POLICY_LFUAGING_HPP_INCLUDED

#include <set>
#include <map>
#include <ctime>

using namespace std;

#include <policy.hpp>

namespace stlcache {
    template <class Key,time_t Age=3600> class policy_lfuaging : public virtual policy_lfu<Key> {
        typedef set<Key> keySet;
        map<Key,time_t> _timeKeeper;
        time_t _oldestEntry;
        time_t age;
    public:
        policy_lfuaging() {
            this->age=Age;
            this->_oldestEntry=time(NULL);
        }
        virtual void remove(const Key& _k) throw() {
            _timeKeeper.erase(_k);
            policy_lfu<Key>::remove(_k);
        }
        virtual void touch(const Key& _k) throw() { 
            _timeKeeper.erase(_k);
            _timeKeeper.insert(std::pair<Key,time_t>(_k,time(NULL))); //Because touch always increases the refcount, so it couldn't be 1 after touch
            policy_lfu<Key>::touch(_k);
        }   
        virtual void clear() throw() {
            _timeKeeper.clear();
            policy_lfu<Key>::clear();
        }
        virtual void swap(policy<Key>& _p) throw(stlcache_invalid_policy) {
            try {
                policy_lfuaging<Key,Age>& _pn=dynamic_cast<policy_lfuaging<Key,Age>& >(_p);
                _timeKeeper.swap(_pn._timeKeeper);

                time_t _oldest=this->_oldestEntry;
                this->_oldestEntry=_pn._oldestEntry;
                _pn._oldestEntry=_oldest;

                time_t a = this->age;
                this->age=_pn.age;
                _pn.age=a;

                policy_lfu<Key>::swap(_pn);
            } catch (const std::bad_cast& ) {
                throw stlcache_invalid_policy("Attempted to swap incompatible policies");
            }
        }
        virtual const _victim<Key> victim() throw()  {
			this->expire();
            return policy_lfu<Key>::victim();
        }
	protected:
		virtual void expire() {
            if ((_oldestEntry+age)<time(NULL)) {
                list<Key> toErase;
                list<Key> toInsert;

                //Time to clean up
                this->_oldestEntry=time(NULL);
                typedef typename map<Key,time_t>::iterator timeMapIterator;
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
                typedef typename list<Key>::iterator listIterator;
                for(listIterator it=toErase.begin();it!=toErase.end();++it) {
                    _timeKeeper.erase(*it);
                }
                for(listIterator it=toInsert.begin();it!=toInsert.end();++it) {
                    _timeKeeper.insert(std::pair<Key,time_t>(*it,time(NULL)));
                }
            }
		}
    };
}

#endif /* STLCACHE_POLICY_LFUAGING_HPP_INCLUDED */
