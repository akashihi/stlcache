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

TEST_CASE("MRU", "[policy,mru]") {
    SECTION("Last item evicted for untouched entries") {
        cache<int,string,policy_mru> c1(3);

        c1.insert(1,"data1");
        c1.insert(2,"data2");
        c1.insert(3,"data3");
        c1.insert(4,"data4");

        CHECK_THROWS_AS(c1.fetch(3),exception_invalid_key); //Must be removed by MRU policy
    }

    SECTION("Touching an entry causes eviction") {
        cache<int,string,policy_mru> c1(3);

        c1.insert(1,"data1");
        c1.insert(2,"data2");
        c1.insert(3,"data3");

        c1.touch(1);

        c1.insert(4,"data4");

        CHECK_THROWS_AS(c1.fetch(1),exception_invalid_key); //Must be removed by MRU policy (cause 1 is touched)
    }
}
