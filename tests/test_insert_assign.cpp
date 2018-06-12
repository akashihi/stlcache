#define BOOST_TEST_MODULE "STLCachePolicyMRU"
#include <boost/test/unit_test.hpp>

#include <stlcache/stlcache.hpp>

using namespace stlcache;

BOOST_AUTO_TEST_SUITE(STLCacheSuite)

BOOST_AUTO_TEST_CASE(data) {
    cache<int,string,policy_none> c(10);

    c.insert_or_assign(1,"data1");
    BOOST_CHECK(c.fetch(1)=="data1");
    c.insert_or_assign(2,"data2");
    BOOST_CHECK(c.fetch(2)=="data2");
    c.insert_or_assign(3,"data3");
    BOOST_CHECK(c.fetch(3)=="data3");
    c.insert_or_assign(1,"data4");

    BOOST_CHECK(c.size()==3); //It shouldn't be bigger nor smaller

    BOOST_CHECK(c.fetch(1)=="data4");
    c.clear();
    BOOST_CHECK(c.size()==0);
}

BOOST_AUTO_TEST_CASE(dataMerge) {
    cache<int,string,policy_none> c(2);
    cache<int,string,policy_none> d(2);
    c.insert_or_assign(1,"data1");
    d.insert_or_assign(2,"data2");
    c.merge(d);
    BOOST_CHECK(c.fetch(1)=="data1");
    BOOST_CHECK(c.fetch(2)=="data2");
}
BOOST_AUTO_TEST_SUITE_END();