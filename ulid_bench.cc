#include <benchmark/benchmark_api.h>

#include "ulid.hh"

static void ULIDCreate(benchmark::State& state) {
	while (state.KeepRunning()) {
		ulid::ULID ulid = ulid::Create(1484581420, []() { return 4; });
	}
}

BENCHMARK(ULIDCreate);

BENCHMARK_MAIN();
