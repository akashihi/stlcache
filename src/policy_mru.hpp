#ifndef STLCACHE_POLICY_MRU_HPP_INCLUDED
#define STLCACHE_POLICY_MRU_HPP_INCLUDED

#include <list>

using namespace std;

#include "policy_lru.hpp"

namespace stlcache {
    template <class Key> class policy_mru : public policy_lru<Key> {
    public:
        virtual const _victim<Key> victim() const throw()  {
            return _victim<Key>(this->entries().front());
        }
    };
}

#endif /* STLCACHE_POLICY_MRU_HPP_INCLUDED */
