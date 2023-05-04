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

TEST_CASE("LFU*", "[policy,lfu]") {
    SECTION("Default eviction") {
        cache<int,string,policy_lfustar> c1(3);

        c1.insert(1,"data1");
        c1.insert(2,"data2");
        c1.insert(3,"data3");
        c1.insert(4,"data4");

        REQUIRE(c1.size()==3);
    }

    SECTION("String keys") {
        cache<string, int, policy_lfustar> c1(3);

        c1.insert("data1",1);
        c1.insert("data2",2);
        c1.insert("data3",3);
        c1.insert("data4",4);

        REQUIRE(c1.size()==3);
    }

    SECTION("LFU* eviction") {
        cache<int,string,policy_lfustar> c1(3);

        c1.insert(1,"data1");
        c1.insert(2,"data2");
        c1.insert(3,"data3");

        c1.touch(1);
        c1.touch(2);

        c1.insert(4,"data4");

        CHECK_THROWS_AS(c1.fetch(3),exception_invalid_key); //Must be removed by LFU policy (cause 1&2 are touched)
    }

    SECTION("Preserve very frequent entries") {
        cache<int,string,policy_lfustar> c1(3);

        c1.insert(1,"data1");
        c1.insert(2,"data2");
        c1.insert(3,"data3");

        c1.touch(1);
        c1.touch(2);
        c1.touch(3);

        //Because every entry in cache have reference counter bigger than one and
        //lfustar policy works only on entries with refcount equal to 1
        CHECK_THROWS_AS(c1.insert(4,"data4"),exception_cache_full);

        c1.erase(1);
        CHECK_NOTHROW(c1.insert(4,"data4"));

        CHECK_THROWS_AS(c1.fetch(1),exception_invalid_key);
        CHECK_NOTHROW(c1.fetch(4));
    }
}

