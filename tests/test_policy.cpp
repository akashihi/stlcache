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

TEST_CASE("Policy size limiting", "[policy]") {
    cache<int,string,policy_none> c1(3);

    c1.insert(1,"data1");
    c1.insert(2,"data2");

    REQUIRE(c1.size()==2);
    REQUIRE(c1.max_size()==3);

    c1.insert(1,"data3");
    REQUIRE(c1.size()==2);
    REQUIRE(c1.max_size()==3);

    c1.insert(3,"data3");
    REQUIRE(c1.size()==3);
    REQUIRE(c1.max_size()==3);

    c1.insert(4,"data4");
    REQUIRE(c1.size()==3);
    REQUIRE(c1.max_size()==3);
}
