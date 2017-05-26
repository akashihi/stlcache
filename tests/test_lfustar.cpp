//
// Copyright (C) 2011-2017 Denis V Chapligin, Martin Hrabovsky
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#define BOOST_TEST_MODULE "STLCachePolicyLFU*"
#include <boost/test/unit_test.hpp>

#include <stlcache/stlcache.hpp>

using namespace stlcache;

BOOST_AUTO_TEST_SUITE(STLCacheSuite)

BOOST_AUTO_TEST_CASE(lastInserted) {
    cache<int,string,policy_lfustar> c1(3);

    c1.insert(1,"data1");
    c1.insert(2,"data2");
    c1.insert(3,"data3");
    c1.insert(4,"data4");

    BOOST_CHECK(c1.size()==3);
}

BOOST_AUTO_TEST_CASE(stringAsKey) {
    cache<string, int, policy_lfustar> c1(3);

    c1.insert("data1",1);
    c1.insert("data2",2);
    c1.insert("data3",3);
    c1.insert("data4",4);

    BOOST_CHECK(c1.size()==3);
}

BOOST_AUTO_TEST_CASE(touch) {
    cache<int,string,policy_lfustar> c1(3);

    c1.insert(1,"data1");
    c1.insert(2,"data2");
    c1.insert(3,"data3");

    c1.touch(1);
    c1.touch(2);

    c1.insert(4,"data4");

    BOOST_REQUIRE_THROW(c1.fetch(3),exception_invalid_key); //Must be removed by LFU policy (cause 1&2 are touched)
}

BOOST_AUTO_TEST_CASE(veryfrequent) {
    cache<int,string,policy_lfustar> c1(3);

    c1.insert(1,"data1");
    c1.insert(2,"data2");
    c1.insert(3,"data3");

    c1.touch(1);
    c1.touch(2);
    c1.touch(3);

    BOOST_REQUIRE_THROW(c1.insert(4,"data4"),exception_cache_full); //Because every entry in cache have reference counter bigger then one and 
                                                                                                    //lfustar policy works only on entries with refcount equal to 1

    c1.erase(1);
    BOOST_REQUIRE_NO_THROW(c1.insert(4,"data4"));

    BOOST_REQUIRE_THROW(c1.fetch(1),exception_invalid_key);
    BOOST_REQUIRE_NO_THROW(c1.fetch(4));
}

BOOST_AUTO_TEST_SUITE_END();
