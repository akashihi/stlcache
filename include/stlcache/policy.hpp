//
// Copyright (C) 2011-2017 Denis V Chapligin, Martin Hrabovsky
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STLCACHE_POLICY_HPP_INCLUDED
#define STLCACHE_POLICY_HPP_INCLUDED

#include <set>

using namespace std;

#include <stlcache/exceptions.hpp>
#include <stlcache/victim.hpp>

namespace stlcache {
    /*! \brief Abstract interface of a cache entries expiration policy.

     *
     * policy class defines, what methods are expected by \link stlcache::cache cache \endlink from the policy. Usually you will never be using this class directly,
     * but it is you key for writing your custom policies. Actually the policy classes, that you are using with the \link stlcache::cache cache \endlink
     * are just wrappers over real policy implementation (derived from the policy interface), used for binding. So you are able to add your own
     * template parameters to your policies, like we are doing it in a \link stlcache::policy_lfuaging LFU-Aging policy \endlink
     *
     * The goal of writing such policy classes is to provide a \link stlcache::cache cache \endlink with a several, configurable cache expiration
     * implementations. The policy should know about keys of the entries in the cache and it must be able to give a cache one or several entries that are 'expired'
     * and could be removed from it. So the interface is targeted to this goal.
     *
     * For example, the \link stlcache::policy_none None policy \endlink is implemented in the following way:
     *
     * \code
     *    template <class Key, template <typename T> class Allocator> class _policy_none_type : public policy<Key,Allocator> {
     *         //Skipping actual implementation of abstract methods
     *     };
     *
     *     struct policy_none {
     *       template <typename Key, template <typename T> class Allocator>
     *          struct bind : _policy_none_type<Key,Allocator> {
     *             bind(const bind& x) : _policy_none_type<Key,Allocator>(x)  { }
     *             bind(const size_t& size) : _policy_none_type<Key,Allocator>(size) { }
     *         };
     *     };
     *
     *     \endcode
     *
     *     Details of the implementation of the any of the policies are not documented and subject to change, but you still may read the sources.
     *
     *     All policies are configured with the cache's key type and used allocator. It is expected, that policy uses supplied allocator type when doing internal
     *     memory allocations.
     *
     *     \tparam <Key> The cache's Key data type
     *     \tparam <Allocator> Type of the allocator object used to define the allocation model.
     *
     *     \see stlcache::cache
     *     \see stlcache::policy_none
     *     \see stlcache::policy_lru
     *     \see stlcache::policy_mru
     *     \see stlcache::policy_lfu
     *     \see stlcache::policy_lfustar
     *     \see stlcache::policy_lfuaging
     *     \see stlcache::policy_lfuagingstar
     *     \see stlcache::policy_adaptive
     *
     * \author chollya (5/19/2011)
     */
    template <class Key,template <typename T> class Allocator> class policy {
    public:
        /*!
         * \brief handles insertion of a key to the cache
         *
         *  Cache calls this method before a key insertion, but only if the cache have some free space (or have cleared some free space). Implementation
         *  could store the key somewhere (and it is safe, to keep only a reference to it) and keep it for future use.
         *
         *  \param <_k> reference to a key value, being inserted
         *
         *  \throw <exception_invalid_key> Thrown when key could not be accepted by the policy (for example when it's not unique or there is some other issue). The insertion into main cache will be cancelled too.
         *
         *  \see cache::insert
         *
         */
        virtual void insert(const Key& _k) =0;
        /*!
         * \brief handles deletion of a entry from the cache
         *
         * After removal of a key from the cache storage itself, the cache will notify a policy of entry removal, by calling this method. Implementation
         * must erase (or forgot) about the key and any data that belongs to it.
         *
         * \param <_k> reference to a key being deleted
         *
         * \see cache::erase
         *
         */
        virtual void remove(const Key& _k) =0;
        /*!
         * \brief marks that the specified entry is used by cache
         *
         * Several cache operations are calling this method. In general words, touch() is a way, to tell the policy, that entry with specified key is being used
         * somehow. The policy implementation may (or may not) use this knowledge for expired items selection.
         *
         * \param <_k> reference to key being used
         *
         * \see cache::touch
         * \see cache::fetch
         * \see cache::check
         *
         */
        virtual void touch(const Key& _k) =0;
        /*!
         * \brief clear call helper
         *
         * Called by cache during it's own \link cache::clear clear \endlink call. Implemention of this method should drop all it's knowledge about the
         * keys and their usage and start from scratch.
         *
         * \see cache::clear
         *
         */
        virtual void clear() =0;
        /*!
         * \brief swap call helper
         *
         * Called by cache during it's own \link cache::swap swap \endlink call. Implementation of this method should swap it's data with the _p data
         * in some way.
         *
         * \param <_p>  Another policy of the same type as this policy cache is swapped with that of this policy.
         *
         * \throw <exception_invalid_policy> Thrown when the supplied policy have a wrong type or can't be swapped.
         *
         * \see cache::swap
         *
         */
        virtual void swap(policy<Key,Allocator>& _p)=0;

        /*!
         * \brief Returns a key of the entry, that is expired by the policy and could be removed.
         *
         * Called by cache, when it's out of space and need to find some. Usually before insertion, but we could not guarantee it. The implementation should
         *  make a decision, what key to expire this time, and return it, using wrapper, to the cache. It is allowed to not return any key, if the
         *  policy can't make a choice, that's why we have a to wrap a value.
         *  The \link stlcache::_victim wrapper \endlink is able to carry some optional value or be empty and provides some information, whether it
         *  contains some data or not.
         *
         * \return wrapped optional expired key value
         *
         * \see cache::insert
         */
        virtual const _victim<Key> victim()  =0;
    };

    template <class Key, template <typename T> class Allocator> class _policy_none_type : public policy<Key,Allocator> {
        set<Key,less<Key>,Allocator<Key> > _entries;
    public:
        _policy_none_type<Key,Allocator>& operator= ( const _policy_none_type<Key,Allocator>& x) {
            this->_entries=x._entries;
            return *this;
        }
        _policy_none_type(const _policy_none_type<Key,Allocator>& x) {
            *this=x;
        }
        _policy_none_type(const size_t& size ) { }

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
        virtual void swap(policy<Key,Allocator>& _p) {
            try {
                _policy_none_type<Key,Allocator>& _pn=dynamic_cast<_policy_none_type<Key,Allocator>& >(_p);
                _entries.swap(_pn._entries);
            } catch (const std::bad_cast& ) {
                throw exception_invalid_policy("Attempted to swap incompatible policies");
            }
        }

        virtual const _victim<Key> victim() {
            if (_entries.rbegin()==_entries.rend()) {
                return _victim<Key>();
            }
            return _victim<Key>(*(_entries.rbegin()));
        }
    };

    /*!
     * \brief A 'random' expiration policy implementation
     *
     * A implementation of the cache expiration policy, that expires entries randomly. It is fast and simple and always able to select a victim.
     *
     * \author chollya (5/19/2011)
     */
    struct policy_none {
        template <typename Key, template <typename T> class Allocator>
            struct bind : _policy_none_type<Key,Allocator> {
                bind(const bind& x) : _policy_none_type<Key,Allocator>(x)  { }
                bind(const size_t& size) : _policy_none_type<Key,Allocator>(size) { }
            };
    };
}

#endif /* STLCACHE_POLICY_HPP_INCLUDED */
