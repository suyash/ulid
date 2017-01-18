# ulid [![Build Status](https://travis-ci.org/suyash/ulid.svg?branch=master)](https://travis-ci.org/suyash/ulid)

C++ port of [oklog/ulid](https://github.com/oklog/ulid) and [alizain/ulid](https://github.com/alizain/ulid).

include header to use.

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

From https://travis-ci.org/suyash/ulid/jobs/193076107

```
Run on (32 X 2793.49 MHz CPU s)
2017-01-18 15:24:38
Benchmark                    Time           CPU Iterations
----------------------------------------------------------
EncodeTime                  21 ns         21 ns   34730355
EncodeTimeNow               27 ns         27 ns   26099807
EncodeEntropy              390 ns        388 ns    1835261
EncodeEntropyRand          331 ns        323 ns    2212419
Encode                     392 ns        387 ns    1686274
EncodeNowRand              323 ns        315 ns    2118726
Create                     409 ns        397 ns    1764046
CreateNowRand              363 ns        361 ns    1970109
MarshalTo                   84 ns         81 ns    8512228
Marshal                    179 ns        174 ns    4078253
MarshalBinaryTo             36 ns         34 ns   21849073
MarshalBinary              284 ns        256 ns    2552790
UnmarshalFrom               74 ns         72 ns   10660505
Unmarshal                  206 ns        198 ns    3518455
UnmarshalBinaryFrom         35 ns         34 ns   21132692
UnmarshalBinary            349 ns        324 ns    2261062
Time                        37 ns         36 ns   19297337
CompareULIDs                29 ns         29 ns   20533156
```
