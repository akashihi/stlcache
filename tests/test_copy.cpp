//
// Copyright (C) 2011 Denis V Chapligin
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#define BOOST_TEST_MODULE "STLCacheCopy"
#include <boost/test/unit_test.hpp>

#ifndef _MSC_VER
#include <unistd.h>
#define WAIT_A_SECOND sleep(2);
#else
#include <windows.h>
#define WAIT_A_SECOND Sleep(2000);
#endif /* _MSC_VER */

#include <stlcache/stlcache.hpp>

using namespace stlcache;

BOOST_AUTO_TEST_SUITE(STLCacheSuite)

BOOST_AUTO_TEST_CASE(copyNone) {
    cache<int,string,policy_none> c1(3);

    c1.insert(1,"data1");
    c1.insert(2,"data2");

    c1.insert(3,"data3");
    BOOST_CHECK(c1.size()==3);
    BOOST_CHECK(c1.max_size()==3);

    cache<int,string,policy_none> c2(c1);

    c2.insert(4,"data4");
    BOOST_CHECK(c2.size()==3);
    BOOST_CHECK(c2.max_size()==3);

    //We cannot predict which items will be removed, so wedo not check it
}

BOOST_AUTO_TEST_CASE(copyLRU) {
    cache<int,string,policy_lru> c1(3);

    c1.insert(1,"data1");
    c1.insert(2,"data2");

    c1.insert(3,"data3");
    BOOST_CHECK(c1.size()==3);
    BOOST_CHECK(c1.max_size()==3);

    cache<int,string,policy_lru> c2(c1);

    c2.insert(4,"data4");
    BOOST_CHECK(c2.size()==3);
    BOOST_CHECK(c2.max_size()==3);

    BOOST_REQUIRE_THROW(c2.fetch(1),exception_invalid_key); //Must be removed by LRU policy
}

BOOST_AUTO_TEST_CASE(copyMRU) {
    cache<int,string,policy_mru> c1(3);

    c1.insert(1,"data1");
    c1.insert(2,"data2");

    c1.insert(3,"data3");
    BOOST_CHECK(c1.size()==3);
    BOOST_CHECK(c1.max_size()==3);

    cache<int,string,policy_mru> c2(c1);

    c2.insert(4,"data4");
    BOOST_CHECK(c2.size()==3);
    BOOST_CHECK(c2.max_size()==3);

    BOOST_REQUIRE_THROW(c2.fetch(3),exception_invalid_key); //Must be removed by MRU policy
}

BOOST_AUTO_TEST_CASE(copyLFU) {
    cache<int,string,policy_lfu> c1(3);

    c1.insert(1,"data1");
    c1.insert(2,"data2");
    c1.insert(3,"data3");

    cache<int,string,policy_lfu> c2(c1);

    c2.touch(1);
    c2.touch(2);

    c2.insert(4,"data4");

    BOOST_REQUIRE_THROW(c2.fetch(3),exception_invalid_key); //Must be removed by LFU policy (cause 1&2 are touched)
}

BOOST_AUTO_TEST_CASE(copyLFUStar) {
    cache<int,string,policy_lfustar> c1(3);

    c1.insert(1,"data1");
    c1.insert(2,"data2");
    c1.insert(3,"data3");

    c1.touch(1);
    c1.touch(2);

    cache<int,string,policy_lfustar> c2(c1);

    c2.insert(4,"data4");

    BOOST_REQUIRE_THROW(c2.fetch(3),exception_invalid_key); //Must be removed by LFU policy (cause 1&2 are touched)
}

BOOST_AUTO_TEST_CASE(copyLFUAging) {
    cache<int,string,policy_lfuaging<3600> > c1(3);

    c1.insert(1,"data1");
    c1.insert(2,"data2");
    c1.insert(3,"data3");

    cache<int,string,policy_lfuaging<3600> > c2(c1);

    c2.touch(1);
    c2.touch(2);

    c2.insert(4,"data4");

    BOOST_REQUIRE_THROW(c2.fetch(3),exception_invalid_key); //Must be removed by LFU policy (cause 1&2 are touched)
}

BOOST_AUTO_TEST_CASE(copyLFUAgingStar) {
    cache<int,string,policy_lfuagingstar<1> > c1(3);

    c1.insert(1,"data1");
    c1.insert(2,"data2");
    c1.insert(3,"data3");

    c1.touch(1); //For key one refcount is 3 now
    c1.touch(1); 
    c1.touch(2); //For key two refcount is 3 now
    c1.touch(2);
    c1.touch(3); //For key three refcount is 2 now

    cache<int,string,policy_lfuagingstar<1> > c2(c1);

    WAIT_A_SECOND;

    c2.insert(4,"data4");

    BOOST_REQUIRE_THROW(c2.fetch(3),exception_invalid_key); //Must be removed by LFU policy (cause every item have been touched and refcount for key 3 is 1)
}

BOOST_AUTO_TEST_CASE(copyAdaptive) {
    cache<int,string,policy_adaptive> c1(3);

    c1.insert(1,"data1");
    c1.insert(2,"data2");
    c1.insert(3,"data3");

    c1.touch(3); //At this stage key4 moved from T1 to T2 and T1 still bigger list and lru entry is key1;

    cache<int,string,policy_adaptive> c2(c1);

    c2.insert(4,"data4");

    BOOST_REQUIRE_THROW(c2.fetch(1),exception_invalid_key);
}

BOOST_AUTO_TEST_SUITE_END();
