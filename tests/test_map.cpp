//
// Copyright (C) 2011 Denis V Chapligin
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#define BOOST_TEST_MODULE "STLCacheMapWrapper"
#include <boost/test/unit_test.hpp>

#include <stlcache/stlcache.hpp>

using namespace stlcache;

BOOST_AUTO_TEST_SUITE(STLCacheSuite)

BOOST_AUTO_TEST_CASE(construction) {
    cache<int,string,policy_none> cache1(10);

    cache<int,string,policy_none> cache2(5);
    cache2=cache1;

    cache<int,string,policy_none> cache3(cache1);

    BOOST_CHECK(cache1.max_size()==10);
    BOOST_CHECK(cache1.size()==0);
    BOOST_CHECK(cache1.empty());

    BOOST_CHECK(cache2.max_size()==10);
    BOOST_CHECK(cache2.size()==0);
    BOOST_CHECK(cache2.empty());

    BOOST_CHECK(cache3.max_size()==10);
    BOOST_CHECK(cache3.size()==0);
    BOOST_CHECK(cache3.empty());
}

BOOST_AUTO_TEST_CASE(data) {
    cache<int,string,policy_none> c(10);

    BOOST_CHECK(c.empty());
    BOOST_CHECK(c.size()==0);

    BOOST_CHECK(c.insert(1,string("test"))); //Insert returns true for new entries

    BOOST_CHECK(!c.empty());
    BOOST_CHECK(c.size()==1);

    BOOST_CHECK(c.check(1));
    BOOST_CHECK(!c.check(2));

    BOOST_CHECK(!c.fetch(1).compare("test"));
    BOOST_REQUIRE_THROW(c.fetch(2),std::exception);

    c.erase(1);
    BOOST_CHECK(c.empty());
    BOOST_CHECK(c.size()==0);
    BOOST_CHECK(!c.check(1));
    BOOST_REQUIRE_THROW(c.fetch(1),exception_invalid_key);

    BOOST_CHECK(c.insert(1,string("test")));
    BOOST_CHECK(!c.insert(1,string("newtest"))); //Insert returns false for existing entries and doesn't updates them
    BOOST_CHECK(c.insert(2,string("testagain"))); //Insert returns true for new entries

    BOOST_CHECK(!c.empty());
    BOOST_CHECK(c.size()==2);

    BOOST_CHECK(!c.fetch(1).compare("test"));
    BOOST_CHECK(!c.fetch(2).compare("testagain"));

    c.clear();
    BOOST_CHECK(c.empty());
    BOOST_CHECK(c.size()==0);
    BOOST_REQUIRE_THROW(c.fetch(1),exception_invalid_key);
    BOOST_REQUIRE_THROW(c.fetch(2),exception_invalid_key);
}

BOOST_AUTO_TEST_CASE(copy) {
    cache<int,string,policy_none> c1(10);

    c1.insert(1,"data1");
    c1.insert(2,"data2");

    cache<int,string,policy_none> c2(c1);

    BOOST_CHECK(!c2.fetch(1).compare("data1"));
    BOOST_CHECK(!c2.fetch(2).compare("data2"));

    cache<int,string,policy_none> c3(5);
    c3.insert(3,"data3");
    c3.insert(4,"data4");

    c1.swap(c3);

    BOOST_CHECK(c1.max_size()==5);
    BOOST_CHECK(c3.max_size()==10);

    BOOST_CHECK(!c3.fetch(1).compare("data1"));
    BOOST_CHECK(!c3.fetch(2).compare("data2"));

    BOOST_CHECK(!c1.fetch(3).compare("data3"));
    BOOST_CHECK(!c1.fetch(4).compare("data4"));

}

BOOST_AUTO_TEST_SUITE_END();
