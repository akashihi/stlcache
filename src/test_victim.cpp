#define BOOST_TEST_MODULE "STLCacheVictim"
#include <boost/test/unit_test.hpp>

#include "stlcache.hpp"

using namespace stlcache;

BOOST_AUTO_TEST_SUITE(STLCacheSuite)

BOOST_AUTO_TEST_CASE(empty) {
    victim<int> v;

    BOOST_CHECK(v.isInitialized()==false);
    BOOST_CHECK(!v);

    BOOST_REQUIRE_THROW(v.value(),stlcache_empty_victim);
    BOOST_REQUIRE_THROW(*v,stlcache_empty_victim);
}

BOOST_AUTO_TEST_CASE(filled) {
    victim<int> v(1);

    BOOST_CHECK(v.isInitialized()==true);
    BOOST_CHECK(v);

    BOOST_REQUIRE_NO_THROW(v.value());
    BOOST_REQUIRE_NO_THROW(*v);

    BOOST_CHECK(v.value()==1);
    BOOST_CHECK(*v==1);
}

BOOST_AUTO_TEST_SUITE_END();
