//
// Copyright (C) 2011 Denis V Chapligin
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STLCACHE_VICTIM_HPP_INCLUDED
#define STLCACHE_VICTIM_HPP_INCLUDED

using namespace std;

#include <stlcache/exceptions.hpp>

namespace stlcache {
    template <class Key> class _victim {
        Key _value;
        bool _isInitialized;
    public:
        _victim() : _value(NULL), _isInitialized(false) { };
        _victim(const Key& _v) : _value(const_cast<Key&>(_v)),_isInitialized(true) { };

        _victim(const _victim& _source) : _value(_source._value),_isInitialized(_source._isInitialized)  { }
        _victim<Key>& operator= ( const _victim<Key>& _source) throw() {
            this->_value=_source._value;
            this->_isInitialized=_source._isInitialized;
            return *this;
        }

        const bool isInitialized() const throw() {
            return this->_isInitialized;
        }
        operator const bool() const throw() {
            return this->_isInitialized;
        }

        const Key& value() const throw(exception_empty_victim) {
            if(!this->_isInitialized) {
                throw exception_empty_victim("Tried to access empty victim");
            }
            return this->_value;
        }
        const Key& operator*() const throw(exception_empty_victim) {
            return this->value();
        }
    };
}

#endif /* STLCACHE_VICTIM_HPP_INCLUDED */
