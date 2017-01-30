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

From https://travis-ci.org/suyash/ulid/jobs/196587557

**ulid_uint128**

```
./ulid_uint128_bench.out --benchmark_out_format=console
Run on (32 X 2800.27 MHz CPU s)
2017-01-30 13:53:30
Benchmark                         Time           CPU Iterations
---------------------------------------------------------------
EncodeTime                       20 ns         20 ns   33878597
EncodeTimeNow                    22 ns         22 ns   32218636
EncodeTimeSystemClockNow        153 ns        153 ns    4490317
EncodeEntropy                   198 ns        198 ns    3562142
EncodeEntropyRand               121 ns        121 ns    5780498
EncodeEntropyMt19937            525 ns        525 ns    1331004
Encode                          213 ns        213 ns    3290697
EncodeNowRand                   139 ns        139 ns    5059356
Create                          232 ns        232 ns    3028591
CreateNowRand                   141 ns        141 ns    4943666
MarshalTo                        39 ns         39 ns   18024722
Marshal                          93 ns         93 ns    7460186
MarshalBinaryTo                  19 ns         19 ns   37735347
MarshalBinary                   133 ns        133 ns    5242484
UnmarshalFrom                    68 ns         68 ns   10145291
Unmarshal                       131 ns        129 ns    5659895
UnmarshalBinaryFrom              64 ns         64 ns   10402550
UnmarshalBinary                 216 ns        216 ns    3233510
Time                             12 ns         12 ns   57538180
CompareULIDs                     10 ns         10 ns   69390812
```

**ulid_struct**

```
./ulid_struct_bench.out --benchmark_out_format=console
Run on (32 X 2800.27 MHz CPU s)
2017-01-30 13:53:48
Benchmark                         Time           CPU Iterations
---------------------------------------------------------------
EncodeTime                       11 ns         11 ns   68024343
EncodeTimeNow                    14 ns         14 ns   53810497
EncodeTimeSystemClockNow        141 ns        141 ns    5011618
EncodeEntropy                   185 ns        185 ns    3756190
EncodeEntropyRand               212 ns        212 ns    3289057
EncodeEntropyMt19937            485 ns        485 ns    1442392
Encode                          197 ns        197 ns    3562101
EncodeNowRand                   220 ns        220 ns    3195564
Create                          204 ns        204 ns    3404090
CreateNowRand                   229 ns        229 ns    3057854
MarshalTo                        41 ns         41 ns   17229969
Marshal                          96 ns         96 ns    7278924
MarshalBinaryTo                  19 ns         19 ns   37625620
MarshalBinary                   134 ns        134 ns    5254147
UnmarshalFrom                    38 ns         38 ns   18270269
Unmarshal                        99 ns         99 ns    7048231
UnmarshalBinaryFrom              19 ns         19 ns   37479143
UnmarshalBinary                 175 ns        175 ns    4180657
Time                             18 ns         18 ns   39717910
CompareULIDs                     20 ns         20 ns   33980240
```
