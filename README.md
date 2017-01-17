# ulid [![Build Status](https://travis-ci.org/suyash/ulid.svg?branch=master)](https://travis-ci.org/suyash/ulid)

C++ port of [oklog/ulid](https://github.com/oklog/ulid) and [alizain/ulid](https://github.com/alizain/ulid).

include header to use.

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

### std::string Marshal(const ULID&)

Marshals and generates std::string.

### void ulid::UnmarshalFrom(const char[26], ULID&)

Unmarshals the passed character array into the ulid.

### ULID Unmarshal(const std::string&)

Creates a new ULID by Unmarshaling the passed string.
