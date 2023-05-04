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

TEST_CASE("Empty iterator", "[iterator]") {
    cache<int,string,policy_none> c(10);

    CHECK(c.empty());
    REQUIRE(c.begin() == c.end());

    c.insert(1, string("test"));

    REQUIRE(c.begin() != c.end());

}


TEST_CASE("Iterator operations", "[itarator]") {
    cache<int,string,policy_lru> c(3);

    c.insert(1,"data1");
    c.insert(2,"data2");
    c.insert(3,"data3");

    SECTION("Iterator movement") {
        auto it = c.begin();

        REQUIRE(it->second == "data1");
        ++it;
        REQUIRE(it->second == "data2");
        it++;
        REQUIRE(it->second == "data3");
        --it;
        REQUIRE(it->second == "data2");
        it--;
        REQUIRE(it->second == "data1");
    }

    SECTION("Iterator comparison") {

        auto first = c.begin();
        auto middle = c.begin();
        auto last = c.end();

        REQUIRE(first == middle);
        REQUIRE(first != last);
    }

    SECTION("Iterator access touches element") {
        auto it = c.begin();
        REQUIRE(it->second == "data1"); // Accessing first element "touches" it.

        c.insert(4,"data4");

        CHECK_THROWS_AS(c.fetch(2),exception_invalid_key); //Must be removed by LRU policy (cause 1 is touched)

        auto it2 = c.begin();
        ++it;
        REQUIRE((*it).second == "data3"); //Accessing 3rd element "touches" it/

        c.insert(5,"data5");

        CHECK_THROWS_AS(c.fetch(1),exception_invalid_key); //Must be removed by LRU policy (cause 3 is touched)
    }
}
