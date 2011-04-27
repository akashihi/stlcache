//
// Copyright (C) 2011 Denis V Chapligin
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#define BOOST_TEST_MODULE "STLCachePolicyLFU*"

#ifndef _MSC_VER
#include <unistd.h>
#define WAIT_A_SECOND sleep(2);
#else
#include <windows.h>
#define WAIT_A_SECOND Sleep(2000);
#endif /* _MSC_VER */

#include <boost/test/unit_test.hpp>

#include <stlcache/stlcache.hpp>

using namespace stlcache;

BOOST_AUTO_TEST_SUITE(STLCacheSuite)

template <typename Key, template <typename T> class Allocator> struct policy_lfuaging3600 : public policy_lfuaging<3600,Key,Allocator> {};

BOOST_AUTO_TEST_CASE(lastInserted) {

    cache<int,string,policy_lfuaging3600> c1(3);

    c1.insert(1,"data1");
    c1.insert(2,"data2");
    c1.insert(3,"data3");
    c1.insert(4,"data4");

    BOOST_CHECK(c1.size()==3);
}

BOOST_AUTO_TEST_CASE(touch) {
    cache<int,string,policy_lfuaging3600> c1(3);

    c1.insert(1,"data1");
    c1.insert(2,"data2");
    c1.insert(3,"data3");

    c1.touch(1);
    c1.touch(2);

    c1.insert(4,"data4");

    BOOST_REQUIRE_THROW(c1.fetch(3),stlcache_invalid_key); //Must be removed by LFU policy (cause 1&2 are touched)
}

template <typename Key, template <typename T> class Allocator> struct policy_lfuaging1 : policy_lfuaging<1,Key,Allocator> {};

BOOST_AUTO_TEST_CASE(expire) {

    cache<int,string,policy_lfuaging1 > c1(3);

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

    WAIT_A_SECOND;

    c1.insert(4,"data4");

    BOOST_REQUIRE_THROW(c1.fetch(3),stlcache_invalid_key); //Must be removed by LFU policy (cause every item have been touched and refcount for key 3 is 2)
}

BOOST_AUTO_TEST_SUITE_END();
