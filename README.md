# ulid [![Build Status](https://travis-ci.org/suyash/ulid.svg?branch=master)](https://travis-ci.org/suyash/ulid)

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

The wrapper type for a 16 byte array representing a ULID.

#### ulid::ULID::ULID()

#### ulid::ULID::ULID(const ULID&)

#### ULID& ulid::ULID::operator=(const ULID&)

#### ulid::ULID::ULID(ULID&&)

#### ULID& ulid::ULID::operator=(ULID&&)

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

From https://travis-ci.org/suyash/ulid/jobs/193279794

**ulid_uint128**

```
./ulid_uint128_bench.out --benchmark_out_format=console
Run on (32 X 2800.1 MHz CPU s)
2017-01-19 05:16:59
Benchmark                         Time           CPU Iterations
---------------------------------------------------------------
EncodeTime                       38 ns         38 ns   18927606
EncodeTimeNow                    45 ns         42 ns   16704891
EncodeTimeSystemClockNow        231 ns        228 ns    3026251
EncodeEntropy                   393 ns        390 ns    1641169
EncodeEntropyRand               211 ns        206 ns    3240195
EncodeEntropyMt19937            920 ns        909 ns     801258
Encode                          457 ns        452 ns    1573754
EncodeNowRand                   255 ns        252 ns    2957128
Create                          439 ns        429 ns    1659683
CreateNowRand                   264 ns        254 ns    2679398
MarshalTo                        75 ns         75 ns    9336127
Marshal                         181 ns        181 ns    3728188
MarshalBinaryTo                  33 ns         33 ns   19939924
MarshalBinary                   293 ns        287 ns    2403615
UnmarshalFrom                   121 ns        118 ns    6025547
Unmarshal                       232 ns        220 ns    2876427
UnmarshalBinaryFrom             107 ns        104 ns    7564454
UnmarshalBinary                 410 ns        408 ns    1847557
Time                             25 ns         24 ns   27947130
CompareULIDs                     20 ns         20 ns   35872754
```

**ulid_struct**

```
./ulid_struct_bench.out --benchmark_out_format=console
Run on (32 X 2800.1 MHz CPU s)
2017-01-19 05:17:21
Benchmark                         Time           CPU Iterations
---------------------------------------------------------------
EncodeTime                       21 ns         21 ns   34823230
EncodeTimeNow                    27 ns         27 ns   26231584
EncodeTimeSystemClockNow        218 ns        214 ns    3208824
EncodeEntropy                   380 ns        378 ns    1837909
EncodeEntropyRand               303 ns        291 ns    2420240
EncodeEntropyMt19937            911 ns        899 ns     800270
Encode                          388 ns        381 ns    1840987
EncodeNowRand                   324 ns        324 ns    2320273
Create                          406 ns        405 ns    1805857
CreateNowRand                   347 ns        343 ns    2017926
MarshalTo                        84 ns         83 ns    8653849
Marshal                         175 ns        173 ns    4034733
MarshalBinaryTo                  35 ns         35 ns   20956150
MarshalBinary                   258 ns        258 ns    2753799
UnmarshalFrom                    75 ns         74 ns    9177808
Unmarshal                       195 ns        194 ns    3927169
UnmarshalBinaryFrom              36 ns         35 ns   20781593
UnmarshalBinary                 317 ns        309 ns    2215915
Time                             36 ns         35 ns   19426495
CompareULIDs                     37 ns         35 ns   18717433
```
