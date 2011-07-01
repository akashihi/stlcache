//
// Copyright (C) 2011 Denis V Chapligin
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#define BOOST_TEST_MODULE "STLCacheInsertPerformance"
#include <boost/test/unit_test.hpp>

#ifdef _WIN32
#include "timeval.h"
#else 
#include <sys/time.h>
#endif /* _WIN32 */

#include <stlcache/stlcache.hpp>

using namespace stlcache;

BOOST_AUTO_TEST_SUITE(STLCacheSuite)

const unsigned int noItems = 65536;

BOOST_AUTO_TEST_CASE(insertNone) {
    struct timeval start,stop;

    gettimeofday(&start, NULL); 

    cache<unsigned int,unsigned int,policy_none> c(noItems);
    for(unsigned int indx = 0; indx<noItems; indx++) {
        c.insert(indx,indx);
    }

    gettimeofday(&stop, NULL); 

    cout<<"Insertion of "<<noItems<<" items into policy_none cache took "<<((stop.tv_sec-start.tv_sec)*1000)+((stop.tv_usec-start.tv_usec)/1000)<<" milliseconds"<<endl;
}

BOOST_AUTO_TEST_CASE(insertLRU) {
    struct timeval start,stop;

    gettimeofday(&start, NULL); 

    cache<unsigned int,unsigned int,policy_lru> c(noItems);
    for(unsigned int indx = 0; indx<noItems; indx++) {
        c.insert(indx,indx);
    }

    gettimeofday(&stop, NULL); 

    cout<<"Insertion of "<<noItems<<" items into policy_lru cache took "<<((stop.tv_sec-start.tv_sec)*1000)+((stop.tv_usec-start.tv_usec)/1000)<<" milliseconds"<<endl;
}

BOOST_AUTO_TEST_CASE(insertMRU) {
    struct timeval start,stop;

    gettimeofday(&start, NULL); 

    cache<unsigned int,unsigned int,policy_mru> c(noItems);
    for(unsigned int indx = 0; indx<noItems; indx++) {
        c.insert(indx,indx);
    }

    gettimeofday(&stop, NULL); 

    cout<<"Insertion of "<<noItems<<" items into policy_mru cache took "<<((stop.tv_sec-start.tv_sec)*1000)+((stop.tv_usec-start.tv_usec)/1000)<<" milliseconds"<<endl;
}

BOOST_AUTO_TEST_CASE(insertLFU) {
    struct timeval start,stop;

    gettimeofday(&start, NULL); 

    cache<unsigned int,unsigned int,policy_lfu> c(noItems);
    for(unsigned int indx = 0; indx<noItems; indx++) {
        c.insert(indx,indx);
    }

    gettimeofday(&stop, NULL); 

    cout<<"Insertion of "<<noItems<<" items into policy_lfu cache took "<<((stop.tv_sec-start.tv_sec)*1000)+((stop.tv_usec-start.tv_usec)/1000)<<" milliseconds"<<endl;
}

BOOST_AUTO_TEST_CASE(insertLFUStar) {
    struct timeval start,stop;

    gettimeofday(&start, NULL); 

    cache<unsigned int,unsigned int,policy_lfustar> c(noItems);
    for(unsigned int indx = 0; indx<noItems; indx++) {
        c.insert(indx,indx);
    }

    gettimeofday(&stop, NULL); 

    cout<<"Insertion of "<<noItems<<" items into policy_lfustar cache took "<<((stop.tv_sec-start.tv_sec)*1000)+((stop.tv_usec-start.tv_usec)/1000)<<" milliseconds"<<endl;
}

BOOST_AUTO_TEST_CASE(insertLFUAging) {
    struct timeval start,stop;

    gettimeofday(&start, NULL); 

    cache<unsigned int,unsigned int,policy_lfuaging<3600> > c(noItems);
    for(unsigned int indx = 0; indx<noItems; indx++) {
        c.insert(indx,indx);
    }

    gettimeofday(&stop, NULL); 

    cout<<"Insertion of "<<noItems<<" items into policy_lfuaging cache took "<<((stop.tv_sec-start.tv_sec)*1000)+((stop.tv_usec-start.tv_usec)/1000)<<" milliseconds"<<endl;
}

BOOST_AUTO_TEST_CASE(insertLFUAgingStar) {
    struct timeval start,stop;

    gettimeofday(&start, NULL); 

    cache<unsigned int,unsigned int,policy_lfuagingstar<3600> > c(noItems);
    for(unsigned int indx = 0; indx<noItems; indx++) {
        c.insert(indx,indx);
    }

    gettimeofday(&stop, NULL); 

    cout<<"Insertion of "<<noItems<<" items into policy_lfuagingstar cache took "<<((stop.tv_sec-start.tv_sec)*1000)+((stop.tv_usec-start.tv_usec)/1000)<<" milliseconds"<<endl;
}

BOOST_AUTO_TEST_CASE(insertAdaptive) {
    struct timeval start,stop;

    gettimeofday(&start, NULL); 

    cache<unsigned int,unsigned int,policy_adaptive> c(noItems);
    for(unsigned int indx = 0; indx<noItems; indx++) {
        c.insert(indx,indx);
    }

    gettimeofday(&stop, NULL); 

    cout<<"Insertion of "<<noItems<<" items into policy_adaptive cache took "<<((stop.tv_sec-start.tv_sec)*1000)+((stop.tv_usec-start.tv_usec)/1000)<<" milliseconds"<<endl;
}

BOOST_AUTO_TEST_SUITE_END();
