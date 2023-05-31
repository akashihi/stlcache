//
// Copyright (C) 2011-2023 Denis V Chapligin, Martin Hrabovsky, Vojtech Ondruj
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <chrono>
#include <thread>
#include <catch2/catch_all.hpp>
#include <stlcache/stlcache.hpp>

using namespace stlcache;
using namespace Catch::Matchers;
using namespace std::chrono_literals;

TEST_CASE("LFU Aging", "[policy,lfu]") {
    SECTION("Default eviction") {
        cache<int,std::string,policy_lfuaging<3600> > c1(3);

        c1.insert(1,"data1");
        c1.insert(2,"data2");
        c1.insert(3,"data3");
        c1.insert(4,"data4");

        REQUIRE(c1.size()==3);
    }

    SECTION("String keys") {
        cache<std::string, int, policy_lfuaging<3600> > c1(3);

        c1.insert("data1",1);
        c1.insert("data2",2);
        c1.insert("data3",3);
        c1.insert("data4",4);

        REQUIRE(c1.size()==3);
    }

    SECTION("LFU eviction") {
        cache<int,std::string,policy_lfuaging<3600> > c1(3);

        c1.insert(1,"data1");
        c1.insert(2,"data2");
        c1.insert(3,"data3");

        c1.touch(1);
        c1.touch(2);

        c1.insert(4,"data4");

        CHECK_THROWS_AS(c1.fetch(3),exception_invalid_key); //Must be removed by LFU policy (cause 1&2 are touched)
    }

    SECTION("Items aging") {
        cache<int,std::string,policy_lfuaging<1> > c1(3);

        c1.insert(1,"data1");
        c1.insert(2,"data2");
        c1.insert(3,"data3");

        c1.touch(1); //For key one refcount is 3 now
        c1.touch(1);
        c1.touch(1);
        c1.touch(2); //For key two refcount is 3 now
        c1.touch(2);
        c1.touch(2);
        c1.touch(3); //For key three refcount is 2 now
        c1.touch(3);

        std::this_thread::sleep_for(1100ms);

        c1.insert(4,"data4");

        CHECK_THROWS_AS(c1.fetch(3),exception_invalid_key); //Must be removed by LFU policy (cause every item have been touched and refcount for key 3 is 2)
    }

    SECTION("String keys aging") {
        cache<std::string, int, policy_lfuaging<1> > c1(3);

        c1.insert("data1",1);
        c1.insert("data2",2);
        c1.insert("data3",3);

        c1.touch("data1"); //For key one refcount is 3 now
        c1.touch("data1");
        c1.touch("data1");
        c1.touch("data2"); //For key two refcount is 3 now
        c1.touch("data2");
        c1.touch("data2");
        c1.touch("data3"); //For key three refcount is 2 now
        c1.touch("data3");

        std::this_thread::sleep_for(1100ms);

        c1.insert("data4", 4);

        CHECK_THROWS_AS(c1.fetch("data3"),exception_invalid_key); //Must be removed by LFU policy (cause every item have been touched and refcount for key 3 is 2)
    }
}
