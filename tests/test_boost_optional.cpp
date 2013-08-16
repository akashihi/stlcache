//
// Copyright (C) 2011 Denis V Chapligin
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#define BOOST_TEST_MODULE "STLCacheBoostOptional"
#include <boost/test/unit_test.hpp>

#define USE_BOOST_OPTIONAL
#include <stlcache/stlcache.hpp>

using namespace stlcache;

BOOST_AUTO_TEST_SUITE(STLCacheSuite)

BOOST_AUTO_TEST_CASE(haveData) {
    cache<string,string,policy_none> c(10);

    BOOST_CHECK(c.insert(string("key"),string("test"))); //Insert returns true for new entries

    BOOST_CHECK(c.get("key")); //Wrapper is not empty.
    BOOST_CHECK(!c.get("key")->compare("test"));
}

BOOST_AUTO_TEST_CASE(noData) {
    cache<string,string,policy_none> c(10);

    BOOST_CHECK(c.insert(string("key"),string("test"))); //Insert returns true for new entries

    BOOST_CHECK(!c.get("data")); //Wrapper is empty.
}

BOOST_AUTO_TEST_SUITE_END();
