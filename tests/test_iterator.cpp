//
// Copyright (C) 2011-2019 Denis V Chapligin
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#define BOOST_TEST_MODULE "STLCacheIterator"
#include <boost/test/unit_test.hpp>

#include <stlcache/stlcache.hpp>

using namespace stlcache;

BOOST_AUTO_TEST_SUITE(STLCacheSuite)

    BOOST_AUTO_TEST_CASE(empty) {
        cache<int,string,policy_none> c(10);

        BOOST_CHECK(c.empty());

        BOOST_CHECK(c.begin() == c.end());

        c.insert(1, string("test"));

        BOOST_CHECK(c.begin() != c.end());
    }

  BOOST_AUTO_TEST_CASE(incdec) {
    cache<int,string,policy_none> c(10);

    c.insert(1, string("one"));
    c.insert(2, string("two"));
    c.insert(3, string("three"));

    auto it = c.begin();

    BOOST_CHECK(it->second == "one");
    ++it;
    BOOST_CHECK(it->second == "two");
    it++;
    BOOST_CHECK(it->second == "three");
    --it;
    BOOST_CHECK(it->second == "two");
    it--;
    BOOST_CHECK(it->second == "one");
  }

  BOOST_AUTO_TEST_CASE(touch) {
    cache<int,string,policy_lru> c(3);

    c.insert(1,"data1");
    c.insert(2,"data2");
    c.insert(3,"data3");

    auto it = c.begin();
    BOOST_CHECK(it->second == "data1");

    c.insert(4,"data4");

    BOOST_REQUIRE_THROW(c.fetch(2),exception_invalid_key); //Must be removed by LRU policy (cause 1 is touched)

    auto it2 = c.begin();
    ++it;
    BOOST_CHECK((*it).second == "data3");

    c.insert(5,"data5");

    BOOST_REQUIRE_THROW(c.fetch(1),exception_invalid_key); //Must be removed by LRU policy (cause 3 is touched)
  }

BOOST_AUTO_TEST_SUITE_END()
