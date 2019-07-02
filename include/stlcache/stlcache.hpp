//
// Copyright (C) 2011-2018 Denis V Chapligin, Martin Hrabovsky, Vojtech Ondruj
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STLCACHE_STLCACHE_HPP_INCLUDED
#define STLCACHE_STLCACHE_HPP_INCLUDED

#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif /* _MSC_VER */

#include <stdexcept>
#include <typeinfo>
#include <map>

using namespace std;

#ifdef USE_BOOST_OPTIONAL
#include <boost/optional.hpp>
#endif /* USE_BOOST_OPTIONAL */

#include <stlcache/exceptions.hpp>
#include <stlcache/policy.hpp>
#include <stlcache/policy_lru.hpp>
#include <stlcache/policy_mru.hpp>
#include <stlcache/policy_lfu.hpp>
#include <stlcache/policy_lfustar.hpp>
#include <stlcache/policy_lfuaging.hpp>
#include <stlcache/policy_lfuagingstar.hpp>
#include <stlcache/policy_adaptive.hpp>
#include <stlcache/lock_none.hpp>
#include <stlcache/lock_exclusive.hpp>
#include <stlcache/lock_shared.hpp>
#include <stlcache/policy_lfu_multi.hpp>

namespace stlcache {

    /*!
      \mainpage STL::Cache - in-memory cache for C++ applications

      \section Introduction

     STL::Cache is just a simple wrapper over standard map, that implements some cache algorithms, thus allowing you to limit the storage size
     and automatically remove unused items from it. (Right now the std::map interface is partially implemented, so it can't be used as a drop-in replacement for the std::map).

     It is intented to be used for keeping any key/value data, especially when data's size are too big, to just put it into the map and keep the whole thing.
     With STL::Cache you could put unlimited (really unlimited) amount of data into it, but it will store only some small part of your data. So re-usable
     data will be kept near your code and not so popular data will not spend expensive memory.

     STL::Cache uses configurable policies, for decisions, whether data are good, to be kept in cache or they should be thrown away. It is shipped with
     8 policies and you are free to implement your own.

     \section GaI Getting and installing

     The STL::Cache source code is hosted on <a href="https://github.com/akashihi/stlcache">github</a>, so you can download the source code
     using wget or other download tool:

     \li https://github.com/akashihi/stlcache/tarball/master (tar.gz format)
     \li https://github.com/akashihi/stlcache/zipball/master (or zip format)

     You could also clone the repository and check some unstable code:

     \code
     git clone git://github.com/akashihi/stlcache.git
     \endcode

     The 'master' branch is always pointing to the latest stable release and the 'next' branch is an unstable development branch.

     The STL::Cache is header only and does not require any building. Just copy the include/stlcache directory to your system's include directories or
     add it to your project's  include directories. Alternativaly you could build the sources and then install it, using our build system.

     Yes, as i told you, the STL::Cache themself is a header-only library and doesn't requires building. Indeed, it is shipped with tests, documentation and
     other stuff, that could be built and used. For STL::Cache building you need:

     \li <a href="http://www.cmake.org/">Recent CMake (required)</a>
     \li <a href="http://www.boost.org/">Boost library Boost library (required for tests otherwise optional)</a>
     \li <a href="http://www.doxygen.org/">Doxygen (optional, only for documentation processing)</a>

     After getting this stuff up and running, select a directory for building and issue the following commands:

     \code
        cmake /path/to/the/stlcache/sources
        make
        make test
        make doc
        make install
    \endcode

     The CMake is using a <a href="http://www.cmake.org/Wiki/CMake_FAQ#Out-of-source_build_trees">out-of-source</a> build approach,
     so it is recommended to create a temporary build directory somewhere and remove it after installation.

     The generated documentation will be put into the \<build\>/doc directory and tests will be built and run directly in the \<build\> directory. They are NOT
     installed by 'make install' command, so you have to copy them manually, if you really need them.

     \section Usage

     Select a \link stlcache::policy cache expiration policy \endlink, configure \link stlcache::cache cache \endlink with it, construct the cache, specifying it's size, and start putting data into the cache:

     \code
     typedef CacheLRU stlcache::cache<int,string,stlcache::policy_lru>;
     CacheLRU myCache(10);
     \endcode

     The policy, key data type and value data type are passed as parameters to the \link stlcache::cache \endlink class template. In the example above
     we have constructed cache, that keeps data of type std::string and keys are of type int. It uses a \link stlcache::policy_lru LRU \endlink policy
     for removal of excessive items. Cache object 'myCache' is able to keep only 10 items. Cache size is configured at the construction time and
     cannot be changed in the future (with exceptions for assignment and swap operations). Additionally you could specify a comparision object and
     allocator type, see the \link stlcache::cache cache class \endlink documentation for the details.

     You could put objects into your cache object using \link cache::insert insert \endlink call:

     \code
     myCache.insert(1,"One)";
     \endcode

     Note, that the \link cache::insert insert \endlink call could throw a \link stlcache::exception_cache_full cache full exception \endlink or
     \link stlcache::exception_invalid_key invalid key exception \endlink As keys have to be unique, the insert call may do nothing, if it meets the
     duplicate key. Don't forget to check it's return value.

     If you are not sure, whether you have element in the cache or not, you can use a safe \link cache::insert_or_assign insert_or_assign \endlink call,
     that will either add new element or update exiting one with the new value.

     Now, when you have some data in the cache, you may want to retrieve it back:

     \code
     string myOne = myCache.fetch(1);
     \endcode

     The \link cache::fetch fetch \endlink call will return a data value, associated with the supplied key or throw the \link stlcache::exception_invalid_key invalid key exception \endlink
     when the key doesn't exists in the cache. For safier operations, you could \link cache::check check \endlink in advance, whether key is in the cache or not:
     \code
     string myOne;
     if (myCache.check(1)) {
       myOne = myCache.fetch(1);
     }
     \endcode

     The \link cache::check check \endlink is exception-safe.

     Both \link cache::check check \endlink and \link cache::fetch fetch \endlink calls are increasing internal reference count for the key, so, depending
     on the used policy, it will increase or decrease the entry's chance to become an expiration victim. Under some circumstances you may
     need to manually change those reference counter, without actually accesing the entry. Something like \link cache::touch touching \endlink it:
     \code
     myCache.touch(1);
     \endcode

     The \link cache::touch \endlink call is exception-safe and could be used even on non-existent keys.

     When you have done your work, you may manually delete the entry:

     \code
     myCache.erase(1);
     \endcode

     Check it's return value, to get sure, whether something was deleted or not.

     \section THREADS Thread safety.

     \link stlcache::cache cache \endlink can be configured with different \link stlcache::lock locking \endlink implementations, thus implementing thread safety.
     By default a \link stlcache::lock_none non-locking \endlink approach is used and \link stlcache::cache cache \endlink is not thread-safe, allowing user to implement external locking.
     STL::Cache is shipped with the following locking implementations:
     \li \link stlcache::lock_none non-locking \endlink - No locking will be done with that implementation, leaving \link stlcache::cache cache \endlink non thread-safe.
     \li \link stlcache::lock_exclusive exclusive locking \endlink - \link stlcache::cache cache \endlink will be locked exclusively on almost every call, thus limiting parallel usage to a single thread.
     \li \link stlcache::lock_shared shared locking \endlink - Some calls will be allowed to be run in parallel with this policy. But, due to nature of the \link stlcache::cache cache \endlink, even operations, that seems to be non-modifying, require exclusive lock to update access tracking data. This implementation is only available when \link BI Boost extensions \endlink are enabled.

     The locking implementation must be specified as a last parameter of \link stlcache::cache cache \endlink type and it is optional.
     \section BI Boost integration

     Since version 0.3 stlcache includes some Boost specific extensions: optional values, multi-map based policies and not really effective
     thread safety.

     \subsection BIO boost::optional

     boost::optional allows user to safely \link cache::get() get\endlink values from the cache for any key, whether key exists or not.
     You will need boost:optional headers available and have to define USE_BOOST_OPTIONAL macro.

     Example:
      \code
          cache<string,string,policy_lru> cache_lru(3);
          cache_lru.insert("key","value");
          cache_lru.touch("key");
          optional<string> value = cache_lru.get("key");
          if (name) {
            cout<<"We have some value in the cache: "<<*name;
          }
          cache_lru.erase("key");
      \endcode
     \subsection BIT lock_shared

     \link stlcache::lock_shared lock_shared \endlink locking implementation allows user to execute some cache to calls in parallel and thread-safe way.
     You have to define USE_BOOST_OPTIONAL macro to access that locking implementation.

    \subsection BIM lfu_multi_index

    \link stlcache:lfu_multi_index lfu_multi_index \endlink implementes LFU algorithm using a Boost MultiIndex map, which is more slower, but uses less ram, comparing to the typical LFU implementation.
    You have to define USE_BOOST_OPTIONAL macro to access that policy.

    \section Policies

     The \link stlcache::policy policy \endlink is a pluggable implementation of a cache algorithms, used for finding and removing excessive
     cache entries. STL::Cache is shipped with the following policies:

     \li \link stlcache::policy_none None \endlink - A random expiration policy. Removes some random entry on request
     \li \link stlcache::policy_lru LRU \endlink - 'Least recently used' policy.
     \li \link stlcache::policy_mru MRU \endlink - 'Most recently used' policy
     \li \link stlcache::policy_lfu LFU \endlink - 'Least frequently used' policy
     \li \link stlcache::policy_lfu_multi LFU (Multi-index) \endlink - 'Least frequently used' policy implemented on top of the multi index map. Requires Boost.
     \li \link stlcache::policy_lfustar LFU* \endlink - 'Least frequently used' policy, that expires only items with refcount 1, as proposed by M.Arlitt
     \li \link stlcache::policy_lfuaging LFU-Aging \endlink - 'Least frequently used' policy with time-based decreasing of usage count
     \li \link stlcache::policy_lfuagingstar LFU*-Aging \endlink - Combination of \link stlcache::policy_lfustar LFU* \endlink and \link stlcache::policy_lfuagingstar LFU*-Aging \endlink policies
     \li \link stlcache::policy_adaptive Adaptive Replacement \endlink - 'Adaptive Replacement' policy

     The cache expiration policy must be specified as a third parameter of \link stlcache::cache cache \endlink type and it is mandatory.

     \section Writing your own policy

     The policy implementation should keep track of entries in the cache and it must be able to tell the cache, what item should be expired at the moment.
     There is not any limitations on policy internal structure and stuff, but it is expected, that policy conforms to some predefined interfaces.

     First of all - every policy is built in two classes, one class is the policy itself, and another one is a 'bind wrapper':

     \note All code examples in this section are from \link stlcache::policy_none  policy none \endlink

     \code
        struct policy_none {
        template <typename Key, template <typename T> class Allocator>
            struct bind : _policy_none_type<Key,Allocator> {
                bind(const bind& x) : _policy_none_type<Key,Allocator>(x)  { }
                bind(const size_t& size) : _policy_none_type<Key,Allocator>(size) { }
            };
        };
     \endcode

     As you may see, the policy itself is automatically configured with caches's Key type and Allocator type. Of course, you could also pass
     your own template parameters and partially instantiate the policy implementation template:

     \code
        template <class R> struct policy_none {
            template <typename Key, template <typename T> class Allocator>
                struct bind : _policy_none_type<R,Key,Allocator> {
                    bind(const bind& x) : _policy_none_type<R,Key,Allocator>(x) { }
                    bind(const size_t& size) : _policy_none_type<R,Key,Allocator>(size) { }
                };
        };
     \endcode

     You could pass some implementation of R during cache type definition:

     \code
         stlcache::cache<int,int,stlcache::policy_none<Randomizer> > c;
     \endcode

     Well, the actual implementation must implement the \link stlcache::policy policy interface \endlink :

     \code
        template <class Key,template <typename T> class Allocator> class policy {
            public:
            virtual void insert(const Key& _k) throw(exception_invalid_key) =0;
            virtual void remove(const Key& _k) throw() =0;
            virtual void touch(const Key& _k) throw() =0;
            virtual void clear() throw() =0;
            virtual void swap(policy<Key,Allocator>& _p) throw(exception_invalid_policy)=0;
            virtual const _victim<Key> victim() throw()  =0;
        };
     \endcode

     So, the policy could be asked for a \link policy::victim victim \endlink, entries could be \link policy::insert inserted \endlink ,
     \link policy::remove removed \endlink and \link policy::touch touched \endlink. It's contents could also be \link policy::clear cleared \endlink
     or \link policy::swap swapped \endlink with another policy. Concrete policy implementation should be CopyConstructible, Assignable and
     must provide a constructor, for specifiying policy size:

     \code
        template <class Key, template <typename T> class Allocator> class _policy_none_type : public policy<Key,Allocator> {
        public:
            _policy_none_type<Key,Allocator>& operator= ( const _policy_none_type<Key,Allocator>& x) throw() { }
            _policy_none_type(const _policy_none_type<Key,Allocator>& x) throw() {}
            _policy_none_type(const size_t& size ) throw() { }
            virtual void insert(const Key& _k) throw(exception_invalid_key) {}
            virtual void remove(const Key& _k) throw() {}
            virtual void touch(const Key& _k) throw() {}
            virtual void clear() throw() {}
            virtual void swap(policy<Key,Allocator>& _p) throw(exception_invalid_policy) {}
            virtual const _victim<Key> victim() throw() {}
        };
     \endcode

     It's up to you, how you will implement those methods and so on. The only importatn thing, we haven't mentioned yet, is a
     \link stlcache::_victim victim \endlink class. It is a way to return optional value from a function. So, when your policy implementatiton
     cannot find any entry to remove, it will return empty victim object.

     \section AaL  Authors and Licensing

     Copyright (C) 2011-2013 Denis V Chapligin
     Distributed under the Boost Software License, Version 1.0.
     (See accompanying file LICENSE_1_0.txt or copy at
     http://www.boost.org/LICENSE_1_0.txt)

      */

    /*! \brief Cache is a kind of a map that have limited number of elements to store and configurable policy for autoremoval of excessive elements.
     *
     * The cache behaviour resembles a std::map behaviour (but not yet fully compatible with usual std::map interface), but it adds a limitation of
     * maximum number of entries, stored in cache and  provides a configurable policy, which is used to remove entries when the cache is full.
     *
     * Internally it is built as a wrapper over a std::map with some additional checking for cache size and entry removal logic. Unfortunately not all interfaces
     * of std::map are supperted by now, but it is a work in progress.
     *
     * You can use the cache in a simple and obvious way:
     * \code
     *     cache<string,string,policy_lru> cache_lru(3);
     *     cache_lru.insert("key","value");
     *     cache_lru.touch("key");
     *     if (cache_lru.check("key")) {
     *         cout<<"We have some value in the cache: "<<cache_lru.fetch("key");
     *     }
     *     cache_lru.erase("key");
     * \endcode
     *
     * So you define a cache instance, specifying a Key type, a Data type and a Policy, passing maximum number of entries to the constructor.
     * You could \link cache::insert insert \endlink as
     * many entries as you like, even more entries, that cache instance can hold. When the cache starts to overflow, it will automatically
     * remove entries, depending on specified Policy.
     * You can \link cache::touch touch \endlink some entries, to advance their usage count, that may (or may not, depending on Policy)
     * help them survive the removal on cache overflow. You can \link cache::erase \endlink entries manually too.
     * For the data retrieval you should first \link cache::check check \endlink , whether Key exists in
     * cache or not, and \link cache::fetch fetch \endlink it, if it exists.
     * The preliminary \link cache::fetch fetch \endlink step is not neccessary, but fetching non-existent key will throw an \link 	exception_invalid_key exception \endlink .
     *
     * cache class have three mandatory template parameters and two optional:
     * \tparam <Key> The key data type. We have no limitations on this type, so it could be any built-in type, custom class etc. But it is recommended to supply a less-than operation for a Key type.
     * \tparam <Data> The value data type. You a free to use any type here without any restrictions, requirements and recommendations.
     * \tparam <Policy> The expiration policy type. Must implement \link stlcache::policy policy interface \endlink STL::Cache provides several expiration policies out-of-box and you are free to define new policies.
     * \tparam <Compare> Comparison class: A class that takes two arguments of the key type and returns a bool. The expression comp(a,b), where comp is an object of this comparison class and a and b are key values, shall return true if a is to be placed at an earlier position than b in a strict weak ordering operation. This can either be a class implementing a function call operator or a pointer to a function (see constructor for an example). This defaults to less<Key>, which returns the same as applying the less-than operator (a<b). This is required for the underlying std::map
     * \tparam <Allocator> Type of the allocator object used to define the storage allocation model. Unlike std::map you should pass a unspecialized Allocator type.
     * \tparam Lock Type of the lock implementation used to define the thread safety behaviour of the cache. Default implementation does no locking and is not thread-safe.
     *
     * So, the full example of cache instantiation will be:
     * \code
     *     cache<int,string,policy_none,less<string>,std::allocator, lock_none> cache_none(100500);
     * \endcode
     *
     * \see policy
     *
     * \author chollya (5/19/2011)
     */
    template<
        class Key,
        class Data,
        class Policy,
        class Compare = less<Key>,
        template <typename T> class Allocator = allocator,
        class Lock = lock_none
    >
    class cache {
        typedef map<Key,Data,Compare,Allocator<pair<const Key, Data> > > storageType;
         storageType _storage;
         std::size_t _maxEntries;
         std::size_t _currEntries;
         typedef typename Policy::template bind<Key,Allocator> policy_type;
         policy_type* _policy;
         Allocator<policy_type> policyAlloc;
         Lock lock;
         typedef typename Lock::read read_lock_type;
         typedef typename Lock::write write_lock_type;

    public:
        /*! \brief The Key type
         */
        typedef Key                                                                key_type;
        /*!  \brief The Data type
         */
        typedef Data                                                               mapped_type;
        /*! \brief Combined key,value type
          */
        typedef pair<const Key, Data>                                         value_type;
        /*! \brief Compare type used by this instance
          */
        typedef Compare                                                          key_compare;
        /*! \brief Allocator type used by this instance
          */
        typedef Allocator<pair<const Key, Data> >                          allocator_type;
        /*! \brief Nested class to compare elements (see member function value_comp)
          */
        typedef typename storageType::value_compare                                value_compare;
        /*! \brief Allocator::reference
          */
        typedef typename storageType::reference                                        reference;
        /*! \brief Allocator::const_reference
          */
        typedef typename storageType::const_reference                               const_reference;
        /*! \brief Type used for storing object sizes, specific to a current platform (usually a size_t)
          */
        typedef typename storageType::size_type                                       size_type;
        /*! \brief Type used for address calculations, specific to a current platform (usually a ptrdiff_t)
          */
        typedef typename storageType::difference_type                               difference_type;
        /*! \brief Allocator::pointer
          */
        typedef typename storageType::pointer                                          pointer;
        /*! \brief Allocator::const_pointer
          */
        typedef typename storageType::const_pointer                                 const_pointer;

        /*! \name std::map-like iterator interface.
         *  Custom iterator implementation for STL::Cache, wrapping std::map iterators with added policy operations support.
         */
        class iterator {
          friend class cache;

          /*! \brief Type used for address calculations, specific to a current platform (usually a ptrdiff_t)
           */
          typedef typename storageType::difference_type                               difference_type;
          /*! \brief Combined key,value type
           */
          typedef pair<const Key, Data>                                         value_type;
          /*! \brief Allocator::reference
           */
          typedef typename storageType::reference                                        reference;
          /*! \brief Allocator::pointer
           */
          typedef typename storageType::pointer                                          pointer;

          typedef std::random_access_iterator_tag iterator_category;

          typename storageType::iterator _storageIterator;

          cache<Key, Data, Policy, Compare, Allocator, Lock>* _owningCache;

          explicit iterator(typename storageType::iterator _internal, cache<Key, Data, Policy, Compare, Allocator, Lock>* _cache): _storageIterator(_internal), _owningCache(_cache) { };

        public:
          iterator(const iterator& other): _storageIterator(other._storageIterator) {};
          ~iterator() =default;

          iterator& operator=(const iterator& other) {
            this->_storageIterator = other._storageIterator;
            this->_owningCache = other._owningCache;
          }
          bool operator==(const iterator& other) const {
            return this->_storageIterator == other._storageIterator;
          };
          bool operator!=(const iterator& other) const {
            return this->_storageIterator != other._storageIterator;
          }

          iterator& operator++() {
            ++_storageIterator;
            return *this;
          }
          iterator operator++(int) {
            iterator tmp = *this;
            ++_storageIterator;
            return tmp;
          }
          iterator& operator--() {
            _storageIterator--;
            return *this;
          }
          iterator operator--(int) {
            iterator tmp = *this;
            --_storageIterator;
            return tmp;
          }

          reference operator*() {
            _owningCache->touch(_storageIterator->first);
            return *_storageIterator;
          };
          pointer operator->() {
            _owningCache->touch(_storageIterator->first);
            return _storageIterator.operator->();
          }
        };

      /*! \name std::map interface wrappers
       *  Simple wrappers for std::map calls, that we are using only for mimicking the map interface
       */
      iterator begin() {
        return iterator(_storage.begin(), this);
      }
      iterator end() {
        return iterator(_storage.end(), this);
      }

      //@{
        /*! \brief Allocator object accessor
          *
          *  Provides access to the allocator object, used to constuct the container.
          *
          *  \return The allocator object of type Allocator<pair<const Key, Data> >.
          */
        allocator_type get_allocator() const noexcept {
            return _storage.get_allocator();
        }

        /*! \brief Counts entries with specified key in the cache.
          *
          *  Provides information on number of cache entries with specific key. It is not very usefull, as keys are unique in the cache.
          *  The difference between thic member and \link cache::check check member \endlink is that this one returns number of keys
          *  without touching the entry usage count.
          *
          *  \param <x> key to count
          *
          *  \return Number of objects with specified key in the cache (ie 1 if object is on the cache and 0 for non-existent object)
          *
          *  \see check
          *
          */
        size_type count ( const key_type& x ) const noexcept {
            read_lock_type l = lock.lockRead();
            return _storage.count(x);
        }

        /*! \brief Value comparision object accessor
         *
         *   Provides access to a comparison object that can be used to compare two element values (pairs) to get whether the key of the first goes before the second. The mapped value, although part of the pair, is not taken into consideration in this comparison - only the key value.
         *
         *   \return The value comparison object of type value_compare, defined as described above.
         */
        value_compare value_comp ( ) const noexcept {
            return _storage.value_comp();
        }

        /*! \brief Key comparision object accessor
          *
          *  Provides access to the  comparison object associated with the container, which can be used to compare the keys of two elements in the container.
          *  This comparison object is set on object construction, and may either be a pointer to a function or an object of a class with a function call operator.
          *  In both cases it takes two arguments of the same type as the element keys,
          *  and returns true if the first argument is considered to go before the second in the strict weak ordering for the map object,
          *  and false otherwise
          *  Notice that the returned comparison object takes as parameters the element keys themselves, not entire element values (pairs).
          *
          *  \return The key comparison object of type key_compare, defined to Compare, which is the fourth template parameter in the map class template.
          */
        key_compare key_comp ( ) const noexcept {
            return _storage.key_comp();
        }

        /*! \brief Test whether cache is empty
          *
          *  Tells whether the map container is empty, i.e. whether its size is 0. This function does not modify the content of the container in any way. In terms of performance it is not equal to call
          *  \code
          *      cache.size()==0
          *   \endcode
          *
          *   \return true if the container size is 0, false otherwise.
          *
          *   \see size
          */
        bool empty() const noexcept {
            read_lock_type l = lock.lockRead();
            return _storage.empty();
        }
        //@}

        /*! \name cache api
         *  members that are specific to cache or implemented with some cache specific things
         */
        //@{
        /*!
         * \brief Clear the cache
         *
         * Removes all cache entries, drops all usage count data and so on.
         *
         * \see size
         * \see empty
         *
         */
        void clear() noexcept {
            write_lock_type l = lock.lockWrite();
            _storage.clear();
            _policy->clear();
            this->_currEntries=0;
        }

        /*!
         * \brief Swaps contents of two caches
         *
         * Exchanges the content of the cache with the content of mp, which is another cache object containing elements of the same type and using the same expiration policy.
         * Sizes may differ. Maximum number of entries may differ too.
         *
         * \param <mp> Another cache of the same type as this whose cache is swapped with that of this cache.
         *
         * \throw <exception_invalid_policy> Thrown when the policies of the caches to swap are incompatible.
         *
         * \see cache::operator=
         */
        void swap ( cache<Key,Data,Policy,Compare,Allocator>& mp ) noexcept(false) {
            write_lock_type l = lock.lockWrite();
            _storage.swap(mp._storage);
            _policy->swap(*mp._policy);

            std::size_t m=this->_maxEntries;
            this->_maxEntries=mp._maxEntries;
            mp._maxEntries=m;

            this->_currEntries=this->_size();
            mp._currEntries=mp.size();
        }

        /*!
         * \brief Removes a entry from cache
         *
         * The entry with the specified key value will be removed from cache and it's usage count information will be erased. Size of the cache will be decreased.
         *  For non-existing entries nothing will be done.
         *
         * \param <x> Key to remove.
         *
         * \return 1 when entry is removed (ie number of removed emtries, which is always 1, as keys are unique) or zero when nothing was done.
         */
        size_type erase ( const key_type& x ) noexcept {
            write_lock_type l = lock.lockWrite();
            return this->_erase(x);
        }

        /*!
         * \brief Insert element to the cache
         *
         * The cache is extended by inserting a single new element. This effectively increases the cache size. Because cache do not allow for duplicate key values, the insertion operation checks for each element inserted whether another element exists already in the container with the same key value, if so, the element is not inserted and its mapped value is not changed in any way.
         * Extension of cache could result in removal of some elements, depending of the cache fullness and used policy. It is also possible, that removal of excessive entries
         * will fail, therefore insert operation will fail too.
         *
         * \throw <exception_cache_full>  Thrown when there are no available space in the cache and policy doesn't allows removal of elements.
         * \throw <exception_invalid_key> Thrown when the policy doesn't accepts the key
         *
         * \return true if the new elemented was inserted or false if an element with the same key existed.
         */
        bool insert(Key _k, Data _d) noexcept(false) {
            write_lock_type l = lock.lockWrite();

            this->_policyEvictInsert(_k);

            bool result=_storage.insert(value_type(_k,_d)).second;
            if (result) {
                _currEntries++;
            }

            return result;
        }

        /*!
         * \brief Insert new element to the cache or replace value of the existing one
         *
         * Will check, whether element with the specified key exists in the map and, in case it exists, will update it's value and increase reference count of the element.
         *
         * Otherwise it will insert single new element. This effectively increases the cache size. Because cache do not allow for duplicate key values, the insertion operation checks
         * for each element inserted whether another element exists already in the container with the same key value, if so, the element is not inserted and its mapped value is not changed in any way.
         * Extension of cache could result in removal of some elements, depending of the cache fullness and used policy. It is also possible, that removal of excessive entries
         * will fail, therefore insert operation will fail too.
         *
         * \throw <exception_cache_full>  Thrown when there are no available space in the cache and policy doesn't allows removal of elements.
         * \throw <exception_invalid_key> Thrown when the policy doesn't accepts the key
         *
         * \return true if the new elemented was inserted or false if an element with the same key existed.
         */
        bool insert_or_assign(Key _k, Data _d) {
            if(this->_storage.find(_k)==this->_storage.end()){
                this->insert(_k, _d);
                return true;
            }

            _policy->touch(_k);
            _storage.erase(_k);
            _storage.insert(value_type(_k,_d)).second;
            return false;
        }

        /*
         * \bried Merge two caches
         *
         * Inserts items, missing in *this, but existing in the second to *this.
         * For the existing items, reference count will be increased.
         */
        void merge(const cache<Key, Data, Policy, Compare, Allocator>& second){
            for (auto it = second._storage.begin(); it != second._storage.end(); it++) {
                if (!this->check(it->first)) {
                    this->insert(it->first, it->second);
                } else {
                    this->touch(it->first);
                }
            }
        }

        /*!
         * \brief Maximum cache size accessor
         *
         * Returns the maximum number of elements that the cache object can hold. This is the maximum potential size the cache can reach. It is specified at
         * construction time and cannot be modified (with excaption of \link cache::swap swap operation \endlink)
         *
         * \return The maximum number of elements a cache can have as its content.
         *
         * \see size
         */
        size_type max_size() const throw() {
            return this->_maxEntries;
        }

        /*! \brief Counts entries in the cache.
          *
          *  Provides information on number of cache entries. For checking, whether the cache empty or not, please use the \link cache::empty empty function \endlink
          *
          *  \return Number of object in the cache (size of cache)
          *
          *  \see empty
          *  \see clear
          */
        size_type size() const throw() {
            write_lock_type l = lock.lockWrite();
            return this->_size();
        }

      /*!
         * \brief Access cache data
         *
         * Returns a reference to the mapped value of the element with key equivalent to key. If no such element exists, an exception of type std::out_of_range is thrown.
         * If the specified key exists in the cache, it's usage count will be touched and reference to the element is returned.
         * The data object itself is kept in the cache, so the reference will be valid until it is removed (either manually or due to cache overflow) or cache object destroyed.
         *
         * \param <_k> key to the data
         *
         * \throw  <std::out_of_range> Thrown when non-existent key is supplied. You could use \link cache::check check member \endlink or \link cache::count count member \endlink to check cache existence prior to fetching the data
         *
         * \return reference to the data, mapped by the key. of type Data of course.
         *
         * \see fetch
         */
        Data& at(const Key& _k) {
          write_lock_type l = lock.lockWrite();
          if (!this->_check(_k)) { //Call to the _check automatically touches entry.
            throw out_of_range("Key is not in cache");
          }
          return (*(_storage.find(_k))).second;
        }

      /*!
         * \brief Access cache data
         *
         * Returns a reference to the mapped value of the element with key equivalent to key. If no such element exists, an exception of type std::out_of_range is thrown.
         * If the specified key exists in the cache, it's usage count will be touched and reference to the element is returned.
         * The data object itself is kept in the cache, so the reference will be valid until it is removed (either manually or due to cache overflow) or cache object destroyed.
         *
         * \param <_k> key to the data
         *
         * \throw  <std::out_of_range> Thrown when non-existent key is supplied. You could use \link cache::check check member \endlink or \link cache::count count member \endlink to check cache existence prior to fetching the data
         *
         * \return constant reference to the data, mapped by the key. of type Data of course.
         *
         * \see fetch
         */
        const Data& at(const Key& _k) const {
          return const_cast<const Data&>(this->at(_k));
        }

      /*!
         * \brief Access cache data
         *
         * Accessor to the data (values) stored in cache. If the specified key exists in the cache, it's usage count will be touched and reference to the element is returned.
         * The data object itself is kept in the cache, so the reference will be valid until it is removed (either manually or due to cache overflow) or cache object destroyed.
         *
         * \param <_k> key to the data
         *
         * \throw  <exception_invalid_key> Thrown when non-existent key is supplied. You could use \link cache::check check member \endlink or \link cache::count count member \endlink to check cache existence prior to fetching the data
         *
         * \return constant reference to the data, mapped by the key. of type Data of course.
         *
         * \see check
         */
        const Data& fetch(const Key& _k) noexcept(false) {
            write_lock_type l = lock.lockWrite();
            if (!this->_check(_k)) { //Call to the _check automatically touches entry.
                throw exception_invalid_key("Key is not in cache",_k);
            }
            return (*(_storage.find(_k))).second;
        }

      /*!
         * \brief Access cache data
         *
         * Returns a reference to the value that is mapped to a key equivalent to key, performing an insertion if such key does not already exist.
         * If the specified key exists in the cache, it's usage count will be touched.
         * The data object itself is kept in the cache, so the reference will be valid until it is removed (either manually or due to cache overflow) or cache object destroyed.
         *
         * \param <_k> key to the data
         *
         * \return reference to the data, mapped by the key. of type Data of course.
         */
        Data& operator[](const Key& _k) noexcept {
          write_lock_type l = lock.lockWrite();
          if (!this->_check(_k)) {
            this->_policyEvictInsert(_k);
            _currEntries++;
          }
          return _storage[_k];
        }

      /*!
         * \brief Access cache data
         *
         * Returns a reference to the value that is mapped to a key equivalent to key, performing an insertion if such key does not already exist.
         * If the specified key exists in the cache, it's usage count will be touched.
         * The data object itself is kept in the cache, so the reference will be valid until it is removed (either manually or due to cache overflow) or cache object destroyed.
         *
         * \param <_k> key to the data
         *
         * \return reference to the data, mapped by the key. of type Data of course.
         */
        Data& operator[](Key&& _k) {
            return (*this)[const_cast<const Key&&>(_k)];
        }

        /*!
         * \brief Finds an element with key equivalent to key.
         *
         * Returns an iterator, pointing to first found element. In case of non-existing key, a past-the-end (\see end()) iterator will be returned.
         * As key is not actually accessed, it's usage count will not be updated.
         *
         * \param <_k> key to the data
         * @return Iterator pointing to first found element or past-the-end iterator.
         */
        iterator find( const Key& _k ) {
          read_lock_type l = lock.lockRead();
          auto _it = _storage.find(_k);
          if ( _it == _storage.end() ) {
            return this->end();
          }
          return iterator(_it, this);
        }

        /*!
         * \brief Finds an element with key not less than specified
         *
         * Returns an iterator, pointing to first element which is not less then specified. In case of non-existing key, a past-the-end (\see end()) iterator will be returned.
         * As key is not actually accessed, it's usage count will not be updated.

         * @param _k key to compare
         * @return Iterator pointing to the first element that is not less than key. If no such element is found, a past-the-end iterator (\see end()) is returned.
         */
        iterator lower_bound( const Key& _k ) {
          read_lock_type l = lock.lockRead();
          auto _it = _storage.lower_bound(_k);
          if ( _it == _storage.end()) {
            return this->end();
          }
          return iterator(_it, this);
        }

        /*!
         * \brief Finds an element with key greater than specified
         *
         * Returns an iterator, pointing to first element which is greater than specified. In case of non-existing key, a past-the-end (\see end()) iterator will be returned.
         * As key is not actually accessed, it's usage count will not be updated.

         * @param _k key to compare
         * @return Iterator pointing to the first element that is greater than key. If no such element is found, a past-the-end iterator (\see end()) is returned.
         */
        iterator upper_bound( const Key& _k ) {
          read_lock_type l = lock.lockRead();
          auto _it = _storage.upper_bound(_k);
          if ( _it == _storage.end()) {
            return this->end();
          }
          return iterator(_it, this);
        }

        /*!
         * \brief Finds an range with elements matching the key.
         *
         * Returns pair of iterators, where first element points to the first element, matching specified key and second point to first element greater then the key.
         * As map keys are unique, distance between those iterators will always be one.
         * In case of non-existing key, a pair of past-the-end (\see end()) iterators will be returned.
         * As key is not actually accessed, it's usage count will not be updated.

         * @param _k key to compare
         * @return pair of iterators, pointing to the key and the next element or pair of past-the-end iterators.
         */
        std::pair<iterator,iterator> equal_range( const Key& _k ) {
          read_lock_type l = lock.lockRead();
          return std::make_pair(this->lower_bound(_k), this->upper_bound(_k));
        }

#ifdef USE_BOOST_OPTIONAL
        /*!
         * \brief Safe cache data access
         *
         * Accessor to the data (values) stored in cache. If the specified key exists in the cache, it's usage count will be touched
         * and reference to the element, wrapped to boost::optional  is returned.  For non-exsitent key empty boost::optional container is returned
         * The data object itself is kept in the cache, so the reference will be valid until it is removed (either manually or due to cache overflow) or cache object destroyed.
         *
         *  This function is only available if USE_BOOST_OPTIONAL macro is defined
         *
         * \param <_k> key to the data
         *
         * \return constant boost::optional wrapper, holding constant reference to the data, in case when key were in the cache,
         * or empty constant boost::optional wrapper for non-existent key.
         *
         * \see check, fetch
         */
        const boost::optional<const Data&> get(const Key& _k) throw() {
            write_lock_type l = lock.lockWrite();
            if (!this->_check(_k)) {
                return boost::optional<const Data&>();
            }
            _policy->touch(_k);
            return boost::optional<const Data&>((*(_storage.find(_k))).second);
        }
#endif /* USE_BOOST_OPTIONAL */

        /*!
         * \brief Check for the key presence in cache
         *
         *  Tests, whether cache is exist in the cache or not. During check it's usage count is increased, as opposed to \link cache::count count member \endlink
         *
         *  \param <_k> key to test
         *
         * \return true if key exists in the cache and false otherwise
         *
         * \see count
         */
        const bool check(const Key& _k) noexcept {
            write_lock_type l = lock.lockWrite();
            return this->_check(_k);
        }

        /*!
         * \brief Increase usage count for entry
         *
         *  Touches an entry in cache, simulating access to it. Usefull to promote (or devote, depending on policy) item in the cache, reducing(or increasing)
         *  risk for expiration for it.
         *
         *  \param _k key to touch
         *
         */
        void touch(const Key& _k) noexcept {
            write_lock_type l = lock.lockWrite();
            _policy->touch(_k);
        }
        //@}

        //@{
        /*!
         * \brief Copy cache content
         *
         * Assigns a copy of the elements in x as the new content for the cache. Usage counts for entries are copied too.
         * The elements contained in the object before the call are dropped, and replaced by copies of those in cache x, if any.
         * After a call to this member function, both the map object and x will have the same size and compare equal to each other.
         *
         * \param <x> a cache object with the same template parameters
         *
         * \return *this
         *
         * \see swap
         */
        cache<Key,Data,Policy,Compare,Allocator>& operator= ( const cache<Key,Data,Policy,Compare,Allocator>& x) noexcept {
            this->_storage=x._storage;
            this->_maxEntries=x._maxEntries;
            this->_currEntries=this->_storage.size();

            policy_type localPolicy(*x._policy);
            this->_policy = policyAlloc.allocate(1);
            policyAlloc.construct(this->_policy,localPolicy);
            return *this;
        }



        /*!
         * \brief A copy constructor
         *
         *  The object is initialized to have the same contents and properties as the x cache object.
         *
         *  \param <x> a cache object with the same template parameters
         */
        cache(const cache<Key,Data,Policy,Compare,Allocator>& x) noexcept {
            *this=x;
        }
        /*!
         * \brief Primary constructor.
         *
         * Constructs an empty cache object and sets a maximum size for it. It is the only way to set size for a cache and it can't be changed later.
         * You could also  pass optional comparator object, compatible with Compare.
         *
         * \param <size> Maximum number of entries, allowed in the cache.
         * \param <comp> Comparator object, compatible with Compare type. Defaults to Compare()
         *
         */
        explicit cache(const size_type size, const Compare& comp = Compare()) noexcept {
            this->_storage=storageType(comp, Allocator<pair<const Key, Data> >());
            this->_maxEntries=size;
            this->_currEntries=0;

            policy_type localPolicy(size);
            this->_policy = policyAlloc.allocate(1);
            policyAlloc.construct(this->_policy,localPolicy);
        }

        /*!
         * \brief destructor
         *
         * Destructs the cache object. This calls each of the cache element's destructors, and deallocates all the storage capacity allocated by the cache.
         *
         */
        ~cache() {
            policyAlloc.destroy(this->_policy);
            policyAlloc.deallocate(this->_policy,1);
        }
        //@}
    protected:
        const bool _check(const Key& _k) noexcept {
            _policy->touch(_k);
            return _storage.count(_k)==1;
        }

        size_type _erase ( const key_type& x ) noexcept {
            size_type ret=_storage.erase(x);
            _policy->remove(x);

            _currEntries-=ret;

            return ret;
        }

        void _policyEvictInsert(const key_type& _k) noexcept(false) {
          while (this->_currEntries >= this->_maxEntries) {
            _victim<Key> victim=_policy->victim();
            if (!victim) {
              throw exception_cache_full("The cache is full and no element can be expired at the moment. Remove some elements manually");
            }
            this->_erase(*victim);
          }

          _policy->insert(_k);
        }

        size_type _size() const noexcept {
            assert(this->_currEntries==_storage.size());
            return this->_currEntries;
        }
    };
}

#endif /* STLCACHE_STLCACHE_HPP_INCLUDED */
