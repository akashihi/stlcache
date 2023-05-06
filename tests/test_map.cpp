//
// Copyright (C) 2011-2023 Denis V Chapligin, Martin Hrabovsky, Vojtech Ondruj
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <catch2/catch_all.hpp>
#include <stlcache/stlcache.hpp>

using namespace stlcache;
using namespace Catch::Matchers;

TEST_CASE("Cache creation", "[map]") {
    cache<int,string,policy_none> cache1(10);
    SECTION("Construction with parameters") {
        REQUIRE(cache1.max_size() == 10);
        REQUIRE(cache1.size() == 0);
        CHECK(cache1.empty());
    }
    SECTION("Copying caches") {
        cache<int,string,policy_none> cache2(5);
        cache2=cache1;

        REQUIRE(cache2.max_size()==10);
        REQUIRE(cache2.size()==0);
        CHECK(cache2.empty());
    }
}

TEST_CASE("Cache data management", "[map") {
    cache<int,string,policy_none> c(10);
    SECTION("New cache is empty") {
        CHECK(c.empty());
        REQUIRE(c.size()==0);
    }
    SECTION("Data storage") {
        CHECK(c.insert(1,string("test"))); //Insert returns true for new entries
        CHECK_FALSE(c.insert(1,string("test2"))); //Insert returns false for existing keys and leaves value intact

        CHECK_FALSE(c.empty());
        REQUIRE(c.size()==1);

        CHECK(c.check(1));
        CHECK_FALSE(c.check(2));

        CHECK_FALSE(c.fetch(1).compare("test"));
        CHECK_THROWS(c.fetch(2));
    }

    SECTION("Data removal") {
        c.erase(1);
        CHECK(c.empty());
        REQUIRE(c.size()==0);
        CHECK_FALSE(c.check(1));
        CHECK_THROWS_AS(c.fetch(1),exception_invalid_key);
    }

    SECTION("Key management") {
        CHECK(c.insert(1,string("test")));
        CHECK_FALSE(c.insert(1,string("newtest"))); //Insert returns false for existing entries and doesn't updates them
        CHECK(c.insert(2,string("testagain"))); //Insert returns true for new entries

        CHECK_FALSE(c.empty());
        REQUIRE(c.size()==2);

        REQUIRE_THAT(c.fetch(1), Equals("test"));
        REQUIRE_THAT(c.fetch(2), Equals("testagain"));
    }

    SECTION("Cache clean-up") {
        c.clear();
        CHECK(c.empty());
        REQUIRE(c.size()==0);
        CHECK_THROWS_AS(c.fetch(1),exception_invalid_key);
        CHECK_THROWS_AS(c.fetch(2),exception_invalid_key);
    }
}

TEST_CASE("Cache copying", "[map]") {
    cache<int,string,policy_none> c1(10);
    c1.insert(1,"data1");
    c1.insert(2,"data2");

    SECTION("Copying caches") {
        cache<int,string,policy_none> c2(c1);

        REQUIRE_THAT(c2.fetch(1), Equals("data1"));
        REQUIRE_THAT(c2.fetch(2), Equals("data2"));
    }

    SECTION("Swapping caches") {
        cache<int,string,policy_none> c3(5);
        c3.insert(3,"data3");
        c3.insert(4,"data4");

        c1.swap(c3);

        REQUIRE(c1.max_size()==5);
        REQUIRE(c3.max_size()==10);

        REQUIRE_THAT(c3.fetch(1), Equals("data1"));
        REQUIRE_THAT(c3.fetch(2), Equals("data2"));

        REQUIRE_THAT(c1.fetch(3), Equals("data3"));
        REQUIRE_THAT(c1.fetch(4), Equals("data4"));
    }
}
