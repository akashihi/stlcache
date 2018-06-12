//
// Copyright (C) 2011-2017 Denis V Chapligin, Martin Hrabovsky
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STLCACHE_VICTIM_HPP_INCLUDED
#define STLCACHE_VICTIM_HPP_INCLUDED

using namespace std;

#include <stlcache/exceptions.hpp>

namespace stlcache {
    /*!
     * \brief Wrapper for optional values
     *
     * Used as interface between \link stlcache::policy policies \endlink and  \link stlcache::cache \endlink themself. The problem is that the policy
     * may not be able to select a entry to remove from the cache, when requested.  So it may return an entry from the \link policy::victim victim call \endlink
     * or may not return anything. Cause we do not have overloading based on return value, we have to use some wrapper.
     *
     * The _victim class is parameterized with the key type and initialized with  some value or without any value. Constructed _victim object
     * cannot be changed in the future, so you can't add a value to the empty object or clear filled object.
     *
     * \tparam <Key> a type to wrap
     *
     * \see stlcache::policy
     *
     * \author chollya (5/20/2011)
     */
    template <class Key> class _victim {
        Key _value;
        bool _isInitialized;
    public:
        /*!
         * \brief default constructor
         *
         * Constructs empty wrapper object, with no data in it.
         *
         */
        _victim() : _isInitialized(false) { };
        /*!
         * \brief initializing constructor
         *
         * Construct wrapper object and initializes it with supplied data.
         *
         * \param _v data to keep in the wrapper
         */
        _victim(const Key& _v) : _value(const_cast<Key&>(_v)),_isInitialized(true) { };

        /*!
         * \brief Copy constructor
         *
         * Constructs an object, exactly equal to the supplied source. Both data (or it's absence) and state are copied.
         * The source object must be of the same type (including template parameters) as the constructed object
         *
         * \param _source a _victim object with the same template parameters
         */
        _victim(const _victim<Key>& _source) : _value(_source._value),_isInitialized(_source._isInitialized)  { }
        /*!
         * \brief Copy victim content
         *
         *  Assigns a copy of the data and the state of _source as the new content for the wrapper. It may convert a
         *  empty object to the object with data and vice versa.
         *
         *  \param <_source> a _victim object with the same template parameters
         *
         * \return *this
         */
        _victim<Key>& operator= ( const _victim<Key>& _source) {
            this->_value=_source._value;
            this->_isInitialized=_source._isInitialized;
            return *this;
        }

        /*!
         * \brief Check for wrapper emptiness
         *
         * Provides information, whether wrapper is containing some data or not.
         *
         * \return true when wrapper object contains some data or false, when it's empty
         *
         */
        const bool isInitialized() const {
            return this->_isInitialized;
        }
        /*!
         * \brief cast to bool operator
         *
         * Wrapper over \link _victim::isInitialized \endlink call, just to make it's use more intuitive and clearier.
         * Usefull for checking _victim objects in the natural way:
         *
         * \code
         *   if (victimObject) {
         *       cout<<"Have some data"<<victimObject.value()<<endl;
         *   } else {
         *       cout<<"No data available"<<endl;
         *   }
         *
         *   \endcode
         *
         * \return true when wrapper object contains some data or false, when it's empty
         *
         * \see _victim::isInitialized
         *
         */
        operator const bool() const {
            return this->_isInitialized;
        }

        /*!
         * \brief Wrapped data accessor
         *
         * Provides access to the data, kept in the wrapper. Will throw exception, when called on the empty
         * wrapper object, so use \link _victim::isInitialized \endlink proir fetching.
         *
         * \throw <exception_empty_victim> Thrown when tried to access data in the empty wrapper object
         *
         * \return the wrapped value
         */
        const Key& value() const {
            if(!this->_isInitialized) {
                throw exception_empty_victim("Tried to access empty victim");
            }
            return this->_value;
        }
        /*!
         * \brief Dereferencing operator
         *
         * Wrapper over \link _victim::value \endlink call, to make it's use more clearier.
         * Usefull for getting the data in a natural way:
         *
         * \code
         *   if (victimObject) {
         *       cout<<"Have some data"<<*victimObject<<endl;
         *   } else {
         *       cout<<"No data available"<<endl;
         *   }
         *
         *  \endcode
         *
         * \throw <exception_empty_victim> Thrown when tried to access data in the empty wrapper object
         *
         * \return the wrapped value
         *
         * \see _victim::value
         */
        const Key& operator*() const {
            return this->value();
        }
    };
}

#endif /* STLCACHE_VICTIM_HPP_INCLUDED */
