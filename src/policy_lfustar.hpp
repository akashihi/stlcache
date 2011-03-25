#ifndef STLCACHE_POLICY_LFUSTAR_HPP_INCLUDED
#define STLCACHE_POLICY_LFUSTAR_HPP_INCLUDED

#include <set>
#include <map>

using namespace std;

#include "policy.hpp"

namespace stlcache {
    template <class Key> class policy_lfustar : public policy_lfu<Key> {
        typedef set<Key> keySet;
    public:
        virtual const _victim<Key> victim() const throw()  {
            //LFU* only operates on entries with references count equal to 1


            if (this->entries().find(1)==this->entries().end()) {
                return _victim<Key>();
            }
            keySet pad=(*(this->entries().find(1))).second;

            if (pad.begin()==pad.end()) {
                return _victim<Key>();
            }

            return _victim<Key>(*(pad.begin()));
        }
    };
}

#endif /* STLCACHE_POLICY_LFUSTAR_HPP_INCLUDED */
