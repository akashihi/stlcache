//
// Copyright (C) 2011 Denis V Chapligin
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#define BOOST_TEST_MODULE "STLCacheVictim"
#include <boost/test/unit_test.hpp>

#include <stlcache/stlcache.hpp>

using namespace stlcache;

BOOST_AUTO_TEST_SUITE(STLCacheSuite)

BOOST_AUTO_TEST_CASE(empty) {
    _victim<int> v;

    BOOST_CHECK(v.isInitialized()==false);
    BOOST_CHECK(!v);

    BOOST_REQUIRE_THROW(v.value(),exception_empty_victim);
    BOOST_REQUIRE_THROW(*v,exception_empty_victim);
}

BOOST_AUTO_TEST_CASE(filled) {
    _victim<int> v(1);

    BOOST_CHECK(v.isInitialized()==true);
    BOOST_CHECK(v);

    BOOST_REQUIRE_NO_THROW(v.value());
    BOOST_REQUIRE_NO_THROW(*v);

    BOOST_CHECK(v.value()==1);
    BOOST_CHECK(*v==1);
}

BOOST_AUTO_TEST_CASE(copy) {
    _victim<int> v;

    BOOST_CHECK(v.isInitialized()==false);
    BOOST_CHECK(!v);

    BOOST_REQUIRE_THROW(v.value(),exception_empty_victim);
    BOOST_REQUIRE_THROW(*v,exception_empty_victim);

    _victim<int> v1(1);

    v=v1;

    BOOST_CHECK(v.isInitialized()==true);
    BOOST_CHECK(v);

    BOOST_REQUIRE_NO_THROW(v.value());
    BOOST_REQUIRE_NO_THROW(*v);

    BOOST_CHECK(v.value()==1);
    BOOST_CHECK(*v==1);

    _victim<int> v2(v);

    BOOST_CHECK(v2.isInitialized()==true);
    BOOST_CHECK(v2);

    BOOST_REQUIRE_NO_THROW(v2.value());
    BOOST_REQUIRE_NO_THROW(*v2);

    BOOST_CHECK(v2.value()==1);
    BOOST_CHECK(*v2==1);
}

BOOST_AUTO_TEST_SUITE_END();
