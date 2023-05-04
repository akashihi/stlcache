//
// Copyright (C) 2011-2023 Denis V Chapligin, Martin Hrabovsky
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <catch2/catch_all.hpp>
#include <stlcache/stlcache.hpp>

using namespace stlcache;
using namespace Catch::Matchers;

TEST_CASE("Map interface", "[map]") {
    cache<int,string,policy_none> c(4);
    CHECK(c.empty());
    c.insert(1,"data1");
    c.insert(2,"data2");

    SECTION("AT") {
        CHECK(c.insert(3,string("test"))); //Insert returns true for new entries

        REQUIRE_THAT(c.at(3), Equals("test"));
        CHECK_THROWS_AS(c.at(4),std::out_of_range);

        const std::string& value = c.at(3);
        REQUIRE_THAT(value, Equals("test"));
    }

    SECTION("BRACKETS") {
        REQUIRE_THAT(c[2], Equals("data2"));

        REQUIRE_THAT(c[4], Equals("")); //This one sets
        REQUIRE_THAT(c[4], Equals("")); //And this one retrieves back

        REQUIRE_THAT(c[5], Equals("")); //Insert new value and evict some existing value

        REQUIRE(c.size()==4);
    }

    SECTION("FIND") {
        auto actual = c.find(2);

        REQUIRE_THAT(actual->second, Equals("data2"));

        REQUIRE(c.find(6) == c.end());
    }

    SECTION("BOUNDS") {
        cache<int,string,policy_lfu> bounds_cache(10);

        bounds_cache.insert(1, "data1");
        bounds_cache.insert(2, "data2");
        bounds_cache.insert(3, "data3");
        bounds_cache.insert(4, "data4");
        bounds_cache.insert(5, "data5");

        REQUIRE_THAT(bounds_cache.lower_bound(2)->second, Equals("data2"));
        REQUIRE_THAT(bounds_cache.lower_bound(0)->second, Equals("data1"));
        REQUIRE(bounds_cache.lower_bound(6) == bounds_cache.end());

        REQUIRE_THAT(bounds_cache.upper_bound(4)->second, Equals("data5"));
        REQUIRE_THAT(bounds_cache.upper_bound(0)->second, Equals("data1"));
        REQUIRE(bounds_cache.upper_bound(6) == bounds_cache.end());
    }
}
