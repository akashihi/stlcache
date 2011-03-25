#ifndef STLCACHE_VICTIM_HPP_INCLUDED
#define STLCACHE_VICTIM_HPP_INCLUDED

using namespace std;

#include "exceptions.hpp"

namespace stlcache {
    template <class Key> class victim {
        const Key& _value;
        const bool _isInitialized;
    public:
        victim() : _value(NULL), _isInitialized(false) { };
        victim(const Key& _v) : _value(_v),_isInitialized(true) { };

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
            return this->_value;
        }
        const Key& operator*() const throw(stlcache_empty_victim) {
            return this->value();
        }
    };
}

#endif /* STLCACHE_VICTIM_HPP_INCLUDED */
