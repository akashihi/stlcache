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

TEST_CASE("Insert/Assign/Merge", "[map]") {
    SECTION("Insert/Assign") {
        cache<int, string, policy_none> c(10);

        CHECK(c.insert_or_assign(1, "data1"));
        REQUIRE(c.fetch(1) == "data1");
        CHECK(c.insert_or_assign(2, "data2"));
        REQUIRE(c.fetch(2) == "data2");
        CHECK(c.insert_or_assign(3, "data3"));
        REQUIRE(c.fetch(3) == "data3");
        CHECK_FALSE(c.insert_or_assign(1, "data4"));

        REQUIRE(c.size() == 3); //It shouldn't be bigger nor smaller

        REQUIRE(c.fetch(1) == "data4");
        c.clear();
        REQUIRE(c.empty());
    }

    SECTION("Merge") {
        cache<int, string, policy_none> c(2);
        cache<int, string, policy_none> d(2);
        c.insert_or_assign(1, "data1");
        d.insert_or_assign(2, "data2");
        c.merge(d);
        REQUIRE(c.fetch(1)=="data1");
        REQUIRE(c.fetch(2)=="data2");
    }
}
