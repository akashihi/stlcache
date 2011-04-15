#ifndef STLCACHE_POLICY_MRU_HPP_INCLUDED
#define STLCACHE_POLICY_MRU_HPP_INCLUDED

#include <list>

using namespace std;

#include <policy_lru.hpp>

namespace stlcache {
    template <class Key> class policy_mru : public policy_lru<Key> {
    public:
        policy_mru(const size_t& size ) throw() : policy_lru<Key>(size) { }

        virtual const _victim<Key> victim() throw()  {
            return _victim<Key>(this->entries().front());
        }
    };
}

#endif /* STLCACHE_POLICY_MRU_HPP_INCLUDED */
