#define BOOST_TEST_MODULE "STLCachePolicyLFU"
#include <boost/test/unit_test.hpp>

#include <stlcache/stlcache.hpp>

using namespace stlcache;

BOOST_AUTO_TEST_SUITE(STLCacheSuite)

BOOST_AUTO_TEST_CASE(lastInserted) {
    cache<int,string,policy_lfu<int> > c1(3);

    c1.insert(1,"data1");
    c1.insert(2,"data2");
    c1.insert(3,"data3");
    c1.insert(4,"data4");

    BOOST_CHECK(c1.size()==3);
}

BOOST_AUTO_TEST_CASE(touch) {
    cache<int,string,policy_lfu<int> > c1(3);

    c1.insert(1,"data1");
    c1.insert(2,"data2");
    c1.insert(3,"data3");

    c1.touch(1);
    c1.touch(2);

    c1.insert(4,"data4");

    BOOST_REQUIRE_THROW(c1.fetch(3),stlcache_invalid_key); //Must be removed by LFU policy (cause 1&2 are touched)
}

BOOST_AUTO_TEST_SUITE_END();
