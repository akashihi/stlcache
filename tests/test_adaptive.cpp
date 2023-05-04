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

TEST_CASE("Adaptive", "[policy,adaptive]") {
    SECTION("Default eviction") {
        cache<int,string,policy_adaptive> c1(3);

        c1.insert(1,"data1");
        c1.insert(2,"data2");
        c1.insert(3,"data3");
        c1.insert(4,"data4");

        REQUIRE(c1.size()==3);
    }

    SECTION("String keys") {
        cache<string, int, policy_adaptive > c1(3);

        c1.insert("data1",1);
        c1.insert("data2",2);
        c1.insert("data3",3);
        c1.insert("data4",4);

        REQUIRE(c1.size()==3);
    }

    SECTION("T1 -> T2 transition") {
        cache<int,string,policy_adaptive > c1(3);

        c1.insert(1,"data1");
        c1.insert(2,"data2");
        c1.insert(3,"data3");

        c1.touch(3); //At this stage key4 moved from T1 to T2 and T1 still bigger list and lru entry is key1;

        c1.insert(4,"data4");

        CHECK_THROWS_AS(c1.fetch(1),exception_invalid_key);
    }

    SECTION("T2 preservation") {
        cache<int,string,policy_adaptive> c1(3);

        c1.insert(1,"data1");
        c1.insert(2,"data2");
        c1.insert(3,"data3");

        c1.touch(3);
        c1.touch(3);
        c1.touch(1);// T2 list is bigger now and key1 have lower reference count

        c1.insert(4,"data4");

        CHECK_THROWS_AS(c1.fetch(1),exception_invalid_key);
    }

    SECTION("B1 restore") {
        cache<int,string,policy_adaptive> c1(4);

        c1.insert(1,"data1");
        c1.insert(2,"data2");
        c1.insert(3,"data3");
        c1.insert(4,"data4");
        c1.insert(5,"data5");
        c1.insert(1,"data3");

        CHECK_THROWS_AS(c1.fetch(2),exception_invalid_key); //key1 is restored from B1 and key2 moved to B1 (out of cache)
    }

    SECTION("B2 restore") {
        cache<int,string,policy_adaptive> c1(4);

        c1.insert(1,"data1");
        c1.insert(2,"data2");
        c1.insert(3,"data3");
        c1.insert(4,"data4");

        c1.touch(1);
        c1.touch(2);
        c1.touch(2);
        c1.touch(3);
        c1.touch(3);
        c1.touch(4);
        c1.touch(4);

        c1.insert(5,"data5");
        c1.insert(1,"data3");

        CHECK_THROWS_AS(c1.fetch(2),exception_invalid_key); //key1 is restored from B1 and key2 moved to B1 (out of cache)
    }
}

