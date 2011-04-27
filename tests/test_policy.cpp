//
// Copyright (C) 2011 Denis V Chapligin
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#define BOOST_TEST_MODULE "STLCachePolicyNone"
#include <boost/test/unit_test.hpp>

#include <stlcache/stlcache.hpp>

using namespace stlcache;

BOOST_AUTO_TEST_SUITE(STLCacheSuite)

BOOST_AUTO_TEST_CASE(sizeLimit) {
    cache<int,string,policy_none> c1(3);

    c1.insert(1,"data1");
    c1.insert(2,"data2");

    BOOST_CHECK(c1.size()==2);
    BOOST_CHECK(c1.max_size()==3);

    c1.insert(1,"data3");
    BOOST_CHECK(c1.size()==2);
    BOOST_CHECK(c1.max_size()==3);

    c1.insert(3,"data3");
    BOOST_CHECK(c1.size()==3);
    BOOST_CHECK(c1.max_size()==3);

    c1.insert(4,"data4");
    BOOST_CHECK(c1.size()==3);
    BOOST_CHECK(c1.max_size()==3);
}

BOOST_AUTO_TEST_SUITE_END();
