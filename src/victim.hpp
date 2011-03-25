#ifndef STLCACHE_VICTIM_HPP_INCLUDED
#define STLCACHE_VICTIM_HPP_INCLUDED

using namespace std;

#include "exceptions.hpp"

namespace stlcache {
    template <class Key> class _victim {
        Key* _value;
        bool _isInitialized;
    public:
        _victim() : _value(NULL), _isInitialized(false) { };
        _victim(const Key& _v) : _value(const_cast<Key*>(&_v)),_isInitialized(true) { };

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

        const Key& value() const throw(stlcache_empty_victim) {
            if(!this->_isInitialized) {
                throw stlcache_empty_victim("Tried to access empty victim");
            }
            return *this->_value;
        }
        const Key& operator*() const throw(stlcache_empty_victim) {
            return this->value();
        }
    };
}

#endif /* STLCACHE_VICTIM_HPP_INCLUDED */
