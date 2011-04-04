#ifndef STLCACHE_POLICY_LFUAGINGSTAR_HPP_INCLUDED
#define STLCACHE_POLICY_LFUAGINGSTAR_HPP_INCLUDED

#include <set>
#include <map>
#include <ctime>

using namespace std;

#include <policy.hpp>
#include <policy_lfuaging.hpp>
#include <policy_lfustar.hpp>

namespace stlcache {
    template <class Key,time_t Age=3600> class policy_lfuagingstar : public virtual policy_lfuaging<Key,Age>, virtual policy_lfustar<Key> {
    public:
        virtual const _victim<Key> victim() throw()  {
			policy_lfuaging<Key,Age>::expire();
            return policy_lfustar<Key>::victim();
        }
    };
}

#endif /* STLCACHE_POLICY_LFUAGINGSTAR_HPP_INCLUDED */
