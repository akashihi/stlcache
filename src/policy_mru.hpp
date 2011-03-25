#ifndef STLCACHE_POLICY_MRU_HPP_INCLUDED
#define STLCACHE_POLICY_MRU_HPP_INCLUDED

#include <list>

using namespace std;

#include "policy_lru.hpp"

namespace stlcache {
    template <class Key> class policy_mru : public policy_lru<Key> {
    public:
        virtual const Key victim() const throw()  {
            const Key _victim=this->entries().front();
            return _victim;
        }
    };
}

#endif /* STLCACHE_POLICY_MRU_HPP_INCLUDED */
