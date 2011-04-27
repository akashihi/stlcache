//
// Copyright (C) 2011 Denis V Chapligin
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#define BOOST_TEST_MODULE "STLCachePolicyMRU"
#include <boost/test/unit_test.hpp>

#include <stlcache/stlcache.hpp>

using namespace stlcache;

BOOST_AUTO_TEST_SUITE(STLCacheSuite)

BOOST_AUTO_TEST_CASE(lastInserted) {
    cache<int,string,policy_mru> c1(3);

    c1.insert(1,"data1");
    c1.insert(2,"data2");
    c1.insert(3,"data3");
    c1.insert(4,"data4");

    BOOST_REQUIRE_THROW(c1.fetch(3),stlcache_invalid_key); //Must be removed by MRU policy
}

BOOST_AUTO_TEST_CASE(touch) {
    cache<int,string,policy_mru> c1(3);

    c1.insert(1,"data1");
    c1.insert(2,"data2");
    c1.insert(3,"data3");

    c1.touch(1);

    c1.insert(4,"data4");

    BOOST_REQUIRE_THROW(c1.fetch(1),stlcache_invalid_key); //Must be removed by MRU policy (cause 1 is touched)
}

BOOST_AUTO_TEST_SUITE_END();
