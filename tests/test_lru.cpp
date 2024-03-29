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

TEST_CASE("LRU", "[policy,lru]") {
    SECTION("First item evicted for untouched entries") {
        cache<int,std::string,policy_lru> c1(3);

        c1.insert(1,"data1");
        c1.insert(2,"data2");
        c1.insert(3,"data3");
        c1.insert(4,"data4");

        CHECK_THROWS_AS(c1.fetch(1),exception_invalid_key); //Must be removed by LRU policy
    }

    SECTION("Touching an entry prevents eviction") {
        cache<int,std::string,policy_lru> c1(3);

        c1.insert(1,"data1");
        c1.insert(2,"data2");
        c1.insert(3,"data3");

        c1.touch(1);

        c1.insert(4,"data4");

        CHECK_THROWS_AS(c1.fetch(2),exception_invalid_key); //Must be removed by LRU policy (cause 1 is touched)
    }

    SECTION("Touch information is copied") {
        cache<int,std::string,policy_lru> c1(3);

        c1.insert(1,"data1");
        c1.insert(2,"data2");
        c1.insert(3,"data3");

        cache<int,std::string,policy_lru> c2(10);
        c2=c1;
        c2.touch(1);

        c2.insert(4,"data4");

        CHECK_THROWS_AS(c2.fetch(2),exception_invalid_key); //Must be removed by LRU policy (cause 1 is touched)

        c1.insert(4,"data4");
        CHECK_THROWS_AS(c1.fetch(1),exception_invalid_key); //Must be removed by LRU policy as no elements in c1 has been ever touched
    }

    SECTION("Clearing cache clears tracking") {
        cache<int,std::string,policy_lru> c1(3);

        c1.insert(1,"data1");
        c1.insert(2,"data2");
        c1.insert(3,"data3");

        c1.clear();

        CHECK_THROWS_AS(c1.fetch(1),exception_invalid_key);

        c1.insert(4,"data4");
        c1.insert(5,"data5");

        REQUIRE_NOTHROW(c1.fetch(4));
        REQUIRE_NOTHROW(c1.fetch(5));
    }

    SECTION("LRU handles insertion of duplicates to the cache properly") {
        // https://github.com/akashihi/stlcache/issues/20
        const int capacity = 10;
        const int duplicateEvery = 4;
        const int duplcateValue = -1;
        cache<int,std::string,policy_lru> cache(capacity);

        for (int i = 0; i < capacity * 10; i++) {
            bool doDuplicate = (i % duplicateEvery) == 0;
            int key = doDuplicate ? duplcateValue : i;
            std::string value = std::to_string(static_cast<unsigned long long>(key));
            cache.insert(key, value);
        }
    }
}
