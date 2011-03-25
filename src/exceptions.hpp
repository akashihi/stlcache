#ifndef STLCACHE_EXCEPTIONS_HPP_INCLUDED
#define STLCACHE_EXCEPTIONS_HPP_INCLUDED

namespace stlcache{
    class stlcache_error : public std::runtime_error {
    public:
        stlcache_error(const std::string &what) : std::runtime_error(what) {  }
    };
    
    class stlcache_empty_victim : public std::runtime_error {
    public:
        stlcache_empty_victim(const std::string &what) : std::runtime_error(what) {  }
    };

    class stlcache_invalid_key : public std::runtime_error {
        const void* k;
    public:
        template <class Key>  stlcache_invalid_key(const std::string &what, const Key& _k) : std::runtime_error(what),k(&_k) {  }
        template <class Key> const Key& key() { return *((Key*)k); }
    };

    class stlcache_invalid_policy : public std::runtime_error {
    public:
        stlcache_invalid_policy(const std::string &what) : std::runtime_error(what) {  }
    };

}

#endif /* STLCACHE_EXCEPTIONS_HPP_INCLUDED */

