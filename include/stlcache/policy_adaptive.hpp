#ifndef STLCACHE_POLICY_ADAPTIVE_HPP_INCLUDED
#define STLCACHE_POLICY_ADAPTIVE_HPP_INCLUDED

#include <list>

using namespace std;

#include <stlcache/policy.hpp>

namespace stlcache {
    template <class Key> class policy_adaptive : public policy<Key> {
        size_t _size;
        policy_lru<Key> T1;
        set<Key> t1Entries;
        policy_lru<Key> B1;
        set<Key> b1Entries;

        policy_lfu<Key> T2;
        set<Key> t2Entries;
        policy_lfu<Key> B2;
        set<Key> b2Entries;

    public:
        policy_adaptive<Key>& operator= ( const policy_adaptive<Key>& x) throw() {
            this->_size=x._size;

            this->T1=x.T1;
            this->t1Entries=x.t1Entries;
            this->B1=x.B1;
            this->b1Entries=x.b1Entries;
            this->T2=x.T2;
            this->t2Entries=x.t2Entries;
            this->B2=x.B2;
            this->b2Entries=x.b2Entries;

            return *this;
        }
        policy_adaptive(const policy_adaptive<Key>& x) throw() : T1(x.T1),T2(x.T2),B1(x.B1),B2(x.B2) {
            this->_size=x._size;

            this->t1Entries=x.t1Entries;
            this->b1Entries=x.b1Entries;
            this->t2Entries=x.t2Entries;
            this->b2Entries=x.b2Entries;
        }
        policy_adaptive(const size_t& size ) throw() : T1(size),T2(size),B1(size),B2(size) { 
            this->_size=size;
        }

        virtual void insert(const Key& _k) throw(stlcache_invalid_key) {
            if (b1Entries.find(_k)!=b1Entries.end()) {
                //Check, whether we are outsized
                b1Entries.erase(_k);
                B1.remove(_k);

                t1Entries.insert(_k);
                T1.insert(_k);
            } else if (b2Entries.find(_k)!=b2Entries.end()) {
                B2.remove(_k);
                b2Entries.erase(_k);

                t2Entries.insert(_k);
                T2.insert(_k);
            } else {
                //Completely new!

                t1Entries.insert(_k);
                T1.insert(_k);
            }
        }
        virtual void remove(const Key& _k) throw() {
            if (t1Entries.find(_k)!=t1Entries.end()) {
                B1.insert(_k);
                b1Entries.insert(_k);

                if (b1Entries.size()>=_size/2) {
                    _victim<Key> bKey=B1.victim();
                    B1.remove(*bKey);
                    b1Entries.erase(*bKey);
                }
                t1Entries.erase(_k);
                T1.remove(_k);
            } else {
                B2.insert(_k);
                b2Entries.insert(_k);

                if (b2Entries.size()>=_size/2) {
                    _victim<Key> bKey=B2.victim();
                    B2.remove(*bKey);
                    b2Entries.erase(*bKey);
                }
                t2Entries.erase(_k);
                T2.remove(_k);
            }
        }
        virtual void touch(const Key& _k) throw() { 
            if (t1Entries.find(_k)!=t1Entries.end()) {
                t1Entries.erase(_k);
                T1.remove(_k);
                t2Entries.insert(_k);
                T2.insert(_k);
            } else {
                T2.touch(_k);
            }
        }
        virtual void clear() throw() {
            T1.clear();
            t1Entries.clear();
            T2.clear();
            t2Entries.clear();
            B1.clear();
            b1Entries.clear();
            B2.clear();
            b2Entries.clear();
        }
        virtual void swap(policy<Key>& _p) throw(stlcache_invalid_policy) {
            try {
                policy_adaptive<Key>& _pn=dynamic_cast<policy_adaptive<Key>& >(_p);
                T1.swap(_pn.T1);
                T2.swap(_pn.T2);
                B1.swap(_pn.B1);
                B2.swap(_pn.B2);

                size_t _oldSize=_size;
                _size=_pn._size;
                _size=_oldSize;
            } catch (const std::bad_cast& ) {
                throw stlcache_invalid_policy("Attempted to swap incompatible policies");
            }
        }

        virtual const _victim<Key> victim() throw()  {
            if (t1Entries.size()>t2Entries.size()) {
                return T1.victim();
            } else {
                return T2.victim();
            }
        }
    };
}

#endif /* STLCACHE_POLICY_ADAPTIVE_HPP_INCLUDED */
