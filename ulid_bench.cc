#include <benchmark/benchmark.h>

#ifdef ULIDUINT128
#include "ulid_uint128.hh"
#else
#include "ulid_struct.hh"
#endif // ULIDUINT128

static void EncodeTime(benchmark::State& state) {
	ulid::ULID ulid;
	while (state.KeepRunning()) {
		ulid::EncodeTime(1484581420, ulid);
	}
}

BENCHMARK(EncodeTime);

static void EncodeTimeNow(benchmark::State& state) {
	ulid::ULID ulid;
	while (state.KeepRunning()) {
		ulid::EncodeTimeNow(ulid);
	}
}

BENCHMARK(EncodeTimeNow);

static void EncodeTimeSystemClockNow(benchmark::State& state) {
	ulid::ULID ulid;
	while (state.KeepRunning()) {
		ulid::EncodeTimeSystemClockNow(ulid);
	}
}

BENCHMARK(EncodeTimeSystemClockNow);

static void EncodeEntropy(benchmark::State& state) {
	ulid::ULID ulid;
	while (state.KeepRunning()) {
		ulid::EncodeEntropy([]() { return 4; }, ulid);
	}
}

BENCHMARK(EncodeEntropy);

static void EncodeEntropyRand(benchmark::State& state) {
	ulid::ULID ulid;
	while (state.KeepRunning()) {
		ulid::EncodeEntropyRand(ulid);
	}
}

BENCHMARK(EncodeEntropyRand);

static void EncodeEntropyMt19937(benchmark::State& state) {
	ulid::ULID ulid;
	std::mt19937 gen(4);

	while (state.KeepRunning()) {
		ulid::EncodeEntropyMt19937(gen, ulid);
	}
}

BENCHMARK(EncodeEntropyMt19937);

static void Encode(benchmark::State& state) {
	ulid::ULID ulid;
	while (state.KeepRunning()) {
		ulid::Encode(1484581420, []() { return 4; }, ulid);
	}
}

BENCHMARK(Encode);

static void EncodeNowRand(benchmark::State& state) {
	ulid::ULID ulid;
	while (state.KeepRunning()) {
		ulid::EncodeNowRand(ulid);
	}
}

BENCHMARK(EncodeNowRand);

static void Create(benchmark::State& state) {
	while (state.KeepRunning()) {
		ulid::ULID ulid = ulid::Create(1484581420, []() { return 4; });
	}
}

BENCHMARK(Create);

static void CreateNowRand(benchmark::State& state) {
	while (state.KeepRunning()) {
		ulid::ULID ulid = ulid::CreateNowRand();
	}
}

BENCHMARK(CreateNowRand);

static void MarshalTo(benchmark::State& state) {
	char a[27];
	ulid::ULID ulid = ulid::CreateNowRand();
	while (state.KeepRunning()) {
		ulid::MarshalTo(ulid, a);
	}
}

BENCHMARK(MarshalTo);

static void Marshal(benchmark::State& state) {
	ulid::ULID ulid = ulid::CreateNowRand();
	while (state.KeepRunning()) {
		std::string m = ulid::Marshal(ulid);
	}
}

BENCHMARK(Marshal);

static void MarshalBinaryTo(benchmark::State& state) {
	uint8_t a[16];
	ulid::ULID ulid = ulid::CreateNowRand();
	while (state.KeepRunning()) {
		ulid::MarshalBinaryTo(ulid, a);
	}
}

BENCHMARK(MarshalBinaryTo);

static void MarshalBinary(benchmark::State& state) {
	ulid::ULID ulid = ulid::CreateNowRand();
	while (state.KeepRunning()) {
		std::vector<uint8_t> m = ulid::MarshalBinary(ulid);
	}
}

BENCHMARK(MarshalBinary);

static void UnmarshalFrom(benchmark::State& state) {
	ulid::ULID ulid;
	while (state.KeepRunning()) {
		ulid::UnmarshalFrom("0001C7STHC0G2081040G208104", ulid);
	}
}

BENCHMARK(UnmarshalFrom);

static void Unmarshal(benchmark::State& state) {
	while (state.KeepRunning()) {
		ulid::ULID ulid = ulid::Unmarshal("0001C7STHC0G2081040G208104");
	}
}

BENCHMARK(Unmarshal);

static void UnmarshalBinaryFrom(benchmark::State& state) {
	ulid::ULID ulid;
	uint8_t b[16] = {0x1, 0xc0, 0x73, 0x62, 0x4a, 0xaf, 0x39, 0x78, 0x51, 0x4e, 0xf8, 0x44, 0x3b, 0xb2, 0xa8, 0x59};
	while (state.KeepRunning()) {
		ulid::UnmarshalBinaryFrom(b, ulid);
	}
}

BENCHMARK(UnmarshalBinaryFrom);

static void UnmarshalBinary(benchmark::State& state) {
	while (state.KeepRunning()) {
		ulid::ULID ulid = ulid::UnmarshalBinary({0x1, 0xc0, 0x73, 0x62, 0x4a, 0xaf, 0x39, 0x78, 0x51, 0x4e, 0xf8, 0x44, 0x3b, 0xb2, 0xa8, 0x59});
	}
}

BENCHMARK(UnmarshalBinary);

static void Time(benchmark::State& state) {
	ulid::ULID ulid = ulid::CreateNowRand();
	while (state.KeepRunning()) {
		time_t t = ulid::Time(ulid);
	}
}

BENCHMARK(Time);

static void CompareULIDs(benchmark::State& state) {
	ulid::ULID ulid1 = ulid::CreateNowRand();
	ulid::ULID ulid2 = ulid1;
	while (state.KeepRunning()) {
		int res = ulid::CompareULIDs(ulid1, ulid2);
	}
}

BENCHMARK(CompareULIDs);

BENCHMARK_MAIN();
