# ulid

![.github/workflows/test.yml](https://github.com/suyash/ulid/workflows/.github/workflows/test.yml/badge.svg)

C++ port of [oklog/ulid](https://github.com/oklog/ulid) and [alizain/ulid](https://github.com/alizain/ulid).

There are 2 implementations, the first in `ulid_uint128.hh` uses a `__uint128_t` to represent a ULID.

The second in `ulid_struct.hh` encapsulates a 16 byte array in a struct, and uses that to represent a ULID

`ulid.hh` essentially (tries to) see if `__uint128_t` exists, in which case it imports the definition in `ulid_uint128.hh`, otherwise it imports the implementation in `ulid_struct.hh`.

The `__uint128_t` version seems to be faster, some benchmarks below and more extensive ones on travis.

- [Usage](#usage)
- [API](#api)
- [Benchmarks](#benchmarks)

## Usage

```c++
#include <ulid.hh>

...

ulid::ULID ulid = ulid::Create(1484581420, []() { return 4; });
std::string str = ulid::Marshal(ulid);
std::cout << str << '\n'; // 0001C7STHC0G2081040G208104
```

## API

### ulid::ULID

The wrapper type for a `__uint128_t` / 16 byte array representing a ULID.

### void ulid::EncodeTime(time_t, ULID&)

encodes passed time in first 6 bytes of a ULID.

### void ulid::EncodeTimeNow(ULID&)

encodes `std::time(nullptr)` as time.

### void ulid::EncodeTimeSystemClockNow(ULID&)

encodes `std::chrono::system_clock::now()` by taking the timestamp in milliseconds.

### void ulid::EncodeEntropy(const std::function<uint8_t()>&, ULID&)

sets the last 10 bytes as the values generated using the passed random byte generator.

### void ulid::EncodeEntropyRand(ULID&)

sets the entropy using `std::rand`.

### void ulid::EncodeEntropyMt19937(std::mt19937& generator, ULID& ulid)

sets the entropy by generating values from a `std::mt19937` generator, initializes a `std::uniform_int_distribution` on each invocation.

### void ulid::Encode(time_t, const std::function<uint8_t()>&, ULID&)

EncodeTime + EncodeEntropy

### void ulid::EncodeNowRand(time_t, const std::function<uint8_t()>&, ULID&)

EncodeTimeNow + EncodeEntropyRand

### ulid::ULID ulid::Create(time_t, const std::function<uint8_t()>&)

creates a fresh ULID using the passed timestamp and generator.

### void ulid::CreateNowRand()

Create:Encode::CreateNowRand:EncodeNowRand

### void ulid::MarshalTo(const ULID&, char[26])

Marshals the ulid into the passed character array.

### std::string ulid::Marshal(const ULID&)

Marshals and generates std::string.

### void ulid::MarshalBinaryTo(const ULID&, uint8_t[16])

Marshals the ulid into the passed byte array.

### std::vector<uint8_t> ulid::MarshalBinary(const ULID&)

Marshals and generates std::vector<uint8_t>.

### void ulid::UnmarshalFrom(const char[26], ULID&)

Unmarshals the passed character array into the ulid.

### ULID ulid::Unmarshal(const std::string&)

Creates a new ULID by Unmarshaling the passed string.

### void ulid::UnmarshalBinaryFrom(const uint8_t[26], ULID&)

Unmarshals the passed byte array into the ulid.

### ULID ulid::UnmarshalBinary(const std::vector<uint8_t>&)

Creates a new ULID by Unmarshaling the passed vector.

### time_t ulid::Time(const ULID&)

Extracts the timestamp used to create the ULID.

## Benchmarks

__Ubuntu Xenial (16.04), clang++-8__

From https://travis-ci.org/suyash/ulid/jobs/475187043

**ulid_uint128**

```
./ulid_uint128_bench.out --benchmark_out_format=console
2019-01-04 07:11:24
Running ./ulid_uint128_bench.out
Run on (2 X 2500 MHz CPU s)
CPU Caches:
  L1 Data 32K (x1)
  L1 Instruction 32K (x1)
  L2 Unified 256K (x1)
  L3 Unified 30720K (x1)
Load Average: 0.98, 0.51, 0.21
--------------------------------------------------------------------
Benchmark                         Time             CPU   Iterations
--------------------------------------------------------------------
EncodeTime                     23.2 ns         23.2 ns     30323833
EncodeTimeNow                  26.8 ns         26.7 ns     25925285
EncodeTimeSystemClockNow       96.4 ns         96.4 ns      7366207
EncodeEntropy                   209 ns          209 ns      3363959
EncodeEntropyRand               100 ns          100 ns      6875073
EncodeEntropyMt19937            470 ns          469 ns      1481254
Encode                          231 ns          231 ns      3022655
EncodeNowRand                   128 ns          128 ns      5346738
Create                          238 ns          238 ns      2970710
CreateNowRand                   133 ns          133 ns      5384520
MarshalTo                      37.5 ns         37.5 ns     18655071
Marshal                        83.7 ns         83.7 ns      8261322
MarshalBinaryTo                16.7 ns         16.7 ns     41790384
MarshalBinary                   157 ns          157 ns      4464307
UnmarshalFrom                  64.0 ns         63.9 ns     10822292
Unmarshal                       113 ns          113 ns      6146163
UnmarshalBinaryFrom            64.8 ns         64.8 ns     10942429
UnmarshalBinary                 248 ns          248 ns      2832217
Time                           15.0 ns         15.0 ns     46389021
CompareULIDs                   8.44 ns         8.44 ns     83106152
```

**ulid_struct**

```
./ulid_struct_bench.out --benchmark_out_format=console
2019-01-04 07:11:43
Running ./ulid_struct_bench.out
Run on (2 X 2500 MHz CPU s)
CPU Caches:
  L1 Data 32K (x1)
  L1 Instruction 32K (x1)
  L2 Unified 256K (x1)
  L3 Unified 30720K (x1)
Load Average: 0.99, 0.54, 0.22
--------------------------------------------------------------------
Benchmark                         Time             CPU   Iterations
--------------------------------------------------------------------
EncodeTime                     9.14 ns         9.13 ns     76851483
EncodeTimeNow                  12.6 ns         12.6 ns     55315496
EncodeTimeSystemClockNow       73.5 ns         73.5 ns      9515669
EncodeEntropy                   201 ns          201 ns      3480980
EncodeEntropyRand               196 ns          195 ns      3602452
EncodeEntropyMt19937            449 ns          449 ns      1560469
Encode                          211 ns          211 ns      3352107
EncodeNowRand                   204 ns          204 ns      3493016
Create                          217 ns          217 ns      3214903
CreateNowRand                   213 ns          213 ns      3271363
MarshalTo                      34.1 ns         34.1 ns     20540465
Marshal                        80.6 ns         80.5 ns      8743422
MarshalBinaryTo                17.6 ns         17.6 ns     39780449
MarshalBinary                   154 ns          154 ns      4503322
UnmarshalFrom                  33.3 ns         33.3 ns     20997275
Unmarshal                      88.5 ns         88.4 ns      7957903
UnmarshalBinaryFrom            17.0 ns         17.0 ns     41314026
UnmarshalBinary                 198 ns          198 ns      3541637
Time                           14.9 ns         14.9 ns     46747210
CompareULIDs                   20.6 ns         20.6 ns     34150306
```

For a comparison across latest 3 versions of g++ and clang++ on linux and clang++ on mac, see https://travis-ci.org/suyash/ulid/builds/475187039

## Hacking

The library uses [googletest](https://github.com/google/googletest) for tests and [benchmark](https://github.com/google/benchmark) as benchmarks, which are built as submodules. In order to get those dependencies, clone recursively, i.e.

```
git clone -r https://github.com/suyash/ulid
```
