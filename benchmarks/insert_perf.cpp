//
// Copyright (C) 2011-2023 Denis V Chapligin, Martin Hrabovsky, Vojtech Ondruj
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <random>
#include <algorithm>
#include <vector>
#include "benchmark/benchmark.h"
#include "stlcache/stlcache.hpp"

using namespace stlcache;

const int NO_ITEMS = 65536;

static std::vector<int> get_data_vec() {
    std::random_device rnd_device;
    std::mt19937 mersenne_engine {rnd_device()};
    std::uniform_int_distribution<int> dist {-1*NO_ITEMS, NO_ITEMS};

    auto gen = [&dist, &mersenne_engine](){
        return dist(mersenne_engine);
    };

    std::vector<int> values(NO_ITEMS);
    generate(begin(values), end(values), gen);

    return values;
}

template <class P> void BM_INSERT(benchmark::State& state) {
    cache<unsigned int,unsigned int,P> c(NO_ITEMS);
    for (auto _ : state) {
        for (int v : get_data_vec()) {
            benchmark::DoNotOptimize(c.insert(v,v));
        }
    }
}
BENCHMARK(BM_INSERT<policy_none>);
BENCHMARK(BM_INSERT<policy_lru>);
BENCHMARK(BM_INSERT<policy_mru>);
BENCHMARK(BM_INSERT<policy_lfu>);
BENCHMARK(BM_INSERT<policy_lfu_multi>);
BENCHMARK(BM_INSERT<policy_lfustar>);
BENCHMARK(BM_INSERT<policy_lfuaging<3600>>);
BENCHMARK(BM_INSERT<policy_lfuagingstar<3600>>);
BENCHMARK(BM_INSERT<policy_adaptive>);

BENCHMARK_MAIN();
