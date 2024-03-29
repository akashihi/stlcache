//
// Copyright (C) 2011-2023 Denis V Chapligin, Martin Hrabovsky, Vojtech Ondruj
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//


// This test will only be compiles and executed if the Boost library is detected.

#ifdef USE_BOOST
#include <catch2/catch_all.hpp>
#include <stlcache/stlcache.hpp>

using namespace stlcache;
using namespace Catch::Matchers;

TEST_CASE("LFU Multi", "[policy,lfu]") {
    SECTION("Default eviction") {
        cache<int,std::string,policy_lfu_multi> c1(3);

        c1.insert(1,"data1");
        c1.insert(2,"data2");
        c1.insert(3,"data3");
        c1.insert(4,"data4");

        REQUIRE(c1.size()==3);
    }

    SECTION("String keys") {
        cache<std::string, int, policy_lfu_multi> c1(3);

        c1.insert("data1",1);
        c1.insert("data2",2);
        c1.insert("data3",3);
        c1.insert("data4",4);

        REQUIRE(c1.size()==3);
    }

    SECTION("LFU eviction") {
        cache<int,std::string,policy_lfu_multi> c1(3);

        c1.insert(1,"data1");
        c1.insert(2,"data2");
        c1.insert(3,"data3");

        c1.touch(1);
        c1.touch(3);

        c1.insert(4,"data4");

        CHECK_THROWS_AS(c1.fetch(2),exception_invalid_key); //Must be removed by LFU policy (cause 1&2 are touched)
    }
}
#endif /* USE_BOOST */