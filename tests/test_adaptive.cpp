#define BOOST_TEST_MODULE "STLCachePolicyAdaprive"
#include <boost/test/unit_test.hpp>

#include <stlcache/stlcache.hpp>

using namespace stlcache;

BOOST_AUTO_TEST_SUITE(STLCacheSuite)

BOOST_AUTO_TEST_CASE(checkVictim) {
    cache<int,string,policy_adaptive<int> > c1(3);

    c1.insert(1,"data1");
    c1.insert(2,"data2");
    c1.insert(3,"data3");
    c1.insert(4,"data4");

    BOOST_CHECK(c1.size()==3);
}

BOOST_AUTO_TEST_CASE(checkPointerMove) {
    cache<int,string,policy_adaptive<int> > c1(3);

    c1.insert(1,"data1");
    c1.insert(2,"data2");
    c1.insert(3,"data3");

    c1.touch(3); //At this stage key4 moved from T1 to T2 and T1 still bigger list and lru entry is key1;

    c1.insert(4,"data4");

    BOOST_REQUIRE_THROW(c1.fetch(1),stlcache_invalid_key);
}

BOOST_AUTO_TEST_CASE(checkPointerMoveT2) {
    cache<int,string,policy_adaptive<int> > c1(3);

    c1.insert(1,"data1");
    c1.insert(2,"data2");
    c1.insert(3,"data3");

    c1.touch(3);
	c1.touch(3);
	c1.touch(1);// T2 list is bigger now and key1 have lower reference count

    c1.insert(4,"data4");

    BOOST_REQUIRE_THROW(c1.fetch(1),stlcache_invalid_key);
}

BOOST_AUTO_TEST_CASE(checkPointerSaveB1) {
    cache<int,string,policy_adaptive<int> > c1(4);

    c1.insert(1,"data1");
    c1.insert(2,"data2");
    c1.insert(3,"data3");
	c1.insert(4,"data4");
	c1.insert(5,"data5");
	c1.insert(1,"data3");

    BOOST_REQUIRE_THROW(c1.fetch(2),stlcache_invalid_key); //key1 is restored from B1 and key2 moved to B1 (out of cache)
}

BOOST_AUTO_TEST_CASE(checkPointerSaveB2) {
    cache<int,string,policy_adaptive<int> > c1(4);

    c1.insert(1,"data1");
    c1.insert(2,"data2");
    c1.insert(3,"data3");
	c1.insert(4,"data4");

	c1.touch(1);
	c1.touch(2);
	c1.touch(2);
	c1.touch(3);
	c1.touch(3);
	c1.touch(4);
	c1.touch(4);

	c1.insert(5,"data5");
	c1.insert(1,"data3");

    BOOST_REQUIRE_THROW(c1.fetch(2),stlcache_invalid_key); //key1 is restored from B1 and key2 moved to B1 (out of cache)
}

BOOST_AUTO_TEST_SUITE_END();
