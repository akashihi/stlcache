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

TEST_CASE("Empty victim", "[victim]") {
    _victim<int> v;

    CHECK_FALSE(v.isInitialized());
    CHECK_FALSE(v);

    CHECK_THROWS_AS(v.value(),exception_empty_victim);
    CHECK_THROWS_AS(*v,exception_empty_victim);

}

TEST_CASE("Victim with value", "[victim]") {
    _victim<int> v(1);

    CHECK(v.isInitialized());
    CHECK(v);

    REQUIRE_NOTHROW(v.value());
    REQUIRE_NOTHROW(*v);

    REQUIRE(v.value()==1);
    REQUIRE(*v==1);
}

TEST_CASE("Victim copy", "[victim]") {
    _victim<int> v;

    CHECK_FALSE(v.isInitialized());
    CHECK_FALSE(v);

    CHECK_THROWS_AS(v.value(),exception_empty_victim);
    CHECK_THROWS_AS(*v,exception_empty_victim);

    _victim<int> v1(1);

    v=v1;

    CHECK(v.isInitialized());
    CHECK(v);

    REQUIRE_NOTHROW(v.value());
    REQUIRE_NOTHROW(*v);

    REQUIRE(v.value()==1);
    REQUIRE(*v==1);

    _victim<int> v2(v);

    CHECK(v2.isInitialized());
    CHECK(v2);

    REQUIRE_NOTHROW(v2.value());
    REQUIRE_NOTHROW(*v2);

    REQUIRE(v2.value()==1);
    REQUIRE(*v2==1);
}
