//
// Copyright (C) 2011-2017 Denis V Chapligin, Martin Hrabovsky
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STLCACHE_EXCEPTIONS_HPP_INCLUDED
#define STLCACHE_EXCEPTIONS_HPP_INCLUDED

namespace stlcache{
    /*!
     * \brief Base class for STL::Cache exceptions
     *
     * Wrapper over std::runtime_error, base for all other STL::Cache exceptions classes. Since it's derived from the standard library exception class,
     * it could be catched like other standard exceptions. Also usefull for catching  all cache specific exceptions.
     *
     * \see exception_empty_victim
     * \see exception_invalid_key
     * \see exception_invalid_policy
     * \see exception_cache_full
     *
     * \author chollya (5/20/2011)
     */
    class exception_base : public std::runtime_error {
    public:
        /*!
         * \brief Exception constructor
         *
         * The constructor takes a standard string object as parameter. This value is stored in the object, and its value is used to generate the C-string returned by its inherited member what.
         *
         * \param what exception message
         */
        exception_base(const std::string &what) : std::runtime_error(what) {  }
    };

    /*!
     * \brief Empty victim access error
     *
     * Used by \link stlcache::_victim victim wrapper \endlink and thrown when someone tries to get access to the wrapper's data on a empty wrapper.
     * Usually this exception is handled by the cache itself and not very frequently used at the wild nature.
     *
     * \see stlcache::_victim
     *
     * \author chollya (5/20/2011)
     */
    class exception_empty_victim : public std::runtime_error {
    public:
        /*!
         * \brief Exception constructor
         *
         * The constructor takes a standard string object as parameter. This value is stored in the object, and its value is used to generate the C-string returned by its inherited member what.
         *
         * \param what exception message
         */
        exception_empty_victim(const std::string &what) : std::runtime_error(what) {  }
    };

    /*!
     * \brief Non-existent or non-acceptable key error
     *
     * Used by policies and cache. Could be thrown when someone tries to \link cache::fetch fetch \endlink key, that doesn't exists in the cache.
     * Policy could rise this exception too, when supplied with the key, that couldn't be accepted by the policy due to some reasons (you could inspect
     * message, carried with the exception, for details). So the \link cache::insert insert operation \endlink could result in this exception too.
     *
     * \see cache::insert
     * \see cache::fetch
     *
     * \author chollya (5/20/2011)
     */
    class exception_invalid_key : public std::runtime_error {
        const void* k;
    public:
        /*!
         * \brief Exception constructor
         *
         * The constructor takes a standard string object and a problematic key value as parameters. The string value is stored in the object and  is used to generate the C-string returned by its inherited member what.
         *
         * \tparam  <Key> type of problematic key
         *
         * \param what exception message
         * \param _k problematic key
         */
        template <class Key>  exception_invalid_key(const std::string &what, const Key& _k) : std::runtime_error(what),k(&_k) {  }
        /*!
         * \brief Accessor for a problematic key
         *
         * This exception carries the value of the key, that was the reason of throwing this exception, and key function is used to examine its value.
         *
         * \tparam <Key> type of problematic key
         *
         * \return the problematic key
         *
         */
        template <class Key> const Key& key() { return *((Key*)k); }
    };

    /*!
     * \brief Incompatible policy error
     *
     * Thrown by \link cache::swap swap \endlink method, when someone tries to swap two caches with different policies or policies, configured in a different ways.
     *
     * \see cache::swap
     *
     * \author chollya (5/20/2011)
     */
    class exception_invalid_policy : public std::runtime_error {
    public:
        /*!
         * \brief Exception constructor
         *
         * The constructor takes a standard string object as parameter. This value is stored in the object, and its value is used to generate the C-string returned by its inherited member what.
         *
         * \param what exception message
         */
        exception_invalid_policy(const std::string &what) : std::runtime_error(what) {  }
    };

    /*!
     * \brief Cache overflow error
     *
     * Thrown when the cache is full and no entry could be expired at the moment.
     *
     * \see stlcache::policy
     * \see cache::insert
     *
     * \author chollya (5/20/2011)
     */
    class exception_cache_full : public std::runtime_error {
    public:
        /*!
         * \brief Exception constructor
         *
         * The constructor takes a standard string object as parameter. This value is stored in the object, and its value is used to generate the C-string returned by its inherited member what.
         *
         * \param what exception message
         */
        exception_cache_full(const std::string &what) : std::runtime_error(what) {  }
    };

}

#endif /* STLCACHE_EXCEPTIONS_HPP_INCLUDED */

