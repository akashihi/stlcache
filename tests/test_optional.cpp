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

TEST_CASE("Optional", "[map]") {
    cache<string,string,policy_none> c(10);

    CHECK(c.insert(string("key"),string("test"))); //Insert returns true for new entries

    SECTION("Fetch value") {
        CHECK(c.get("key")); //Wrapper is not empty.
        REQUIRE_THAT(*c.get("key"), Equals("test"));
    }

    SECTION("Fetch none") {
        CHECK_FALSE(c.get("data")); //Wrapper is empty.
    }
}
