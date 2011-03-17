#ifndef STLCACHE_POLICY_HPP_INCLUDED
#define STLCACHE_POLICY_HPP_INCLUDED

#include <set>

using namespace std;

#include "exceptions.hpp"

namespace stlcache {
    template <class Key> class policy {
    public:
        virtual void insert(const Key& _k)=0;
        virtual void remove(const Key& _k)=0;
        virtual void touch(const Key& _k)=0;
        virtual void clear()=0;
        virtual void swap(policy<Key>& _p)=0;

        virtual const Key& victim()=0;
    };

    template <class Key> class policy_none : public policy<Key> {
        set<Key> _entries;
    public:
        virtual void insert(const Key& _k) {
            _entries.insert(_k);
        }
        virtual void remove(const Key& _k) {
            _entries.erase(_k);
        }
        virtual void touch(const Key& _k) { /* Not used here */  }
        virtual void clear() {
            _entries.clear();
        }
        virtual void swap(policy<Key>& _p) {
            try {
                policy_none<Key>& _pn=dynamic_cast<policy_none<Key>& >(_p);
                _entries.swap(_pn._entries);
            } catch (const std::bad_cast& e) {
                throw stlcache_invalid_policy("Attempted to swap incompatible policies");
            }
        }

        virtual const Key& victim() {
            return *(_entries.rbegin());
        }
    };
}

#endif /* STLCACHE_POLICY_HPP_INCLUDED */
