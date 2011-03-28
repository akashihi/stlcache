#ifndef STLCACHE_POLICY_LRU_HPP_INCLUDED
#define STLCACHE_POLICY_LRU_HPP_INCLUDED

#include <list>

using namespace std;

#include <policy.hpp>

namespace stlcache {
    template <class Key> class policy_lru : public policy<Key> {
        list<Key> _entries;
    public:
        virtual void insert(const Key& _k) throw() {
            _entries.push_front(_k);
        }
        virtual void remove(const Key& _k) throw() {
            _entries.remove(_k);
        }
        virtual void touch(const Key& _k) throw() { 
            _entries.remove(_k);
            _entries.push_front(_k);
        }
        virtual void clear() throw() {
            _entries.clear();
        }
        virtual void swap(policy<Key>& _p) throw(stlcache_invalid_policy) {
            try {
                policy_lru<Key>& _pn=dynamic_cast<policy_lru<Key>& >(_p);
                _entries.swap(_pn._entries);
            } catch (const std::bad_cast& e) {
                throw stlcache_invalid_policy("Attempted to swap incompatible policies");
            }
        }

        virtual const _victim<Key> victim() throw()  {
            return _victim<Key>(_entries.back());
        }

    protected:
        const list<Key>& entries() const  { return this->_entries; }
    };
}

#endif /* STLCACHE_POLICY_LRU_HPP_INCLUDED */
