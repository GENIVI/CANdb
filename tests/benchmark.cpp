#include "dbcparser.h"
#include "test_helper.hpp"
#include <benchmark/benchmark.h>

static void gm_global_a_chassis(benchmark::State& state)
{
    CANdb::DBCParser parser;
    const auto comfort = test_helper::loadDBCFile("gm_global_a_chassis.dbc");
    // Perform setup here
    for (auto _ : state) {
        // This code gets timed
        parser.parse(comfort);
    }
}
// Register the function as a benchmark
BENCHMARK(gm_global_a_chassis);
// Run the benchmark
BENCHMARK_MAIN();
