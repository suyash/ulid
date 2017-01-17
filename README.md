# ulid [![Build Status](https://travis-ci.org/suyash/ulid.svg?branch=master)](https://travis-ci.org/suyash/ulid)

C++ port of [oklog/ulid](https://github.com/oklog/ulid) and [alizain/ulid](https://github.com/alizain/ulid).

include header to use.

## Usage

```c++
#include <ulid.hh>

...

ulid::ULID ulid;
ulid::Encode(1484581420, []() { return 4; }, ulid);
std::string str = ulid::Marshal(ulid);
std::cout << str << '\n'; // 0001C7STHC0G2081040G208104
```

## API

### ulid::ULID

The wrapper type for a 16 byte array representing a ULID.

#### ulid::ULID::ULID()

#### ulid::ULID::ULID(const ULID&)

#### ulid::ULID::ULID(ULID&&)

### void ulid::EncodeTime(time_t, ULID&)

encodes passed time in first 6 bytes of a ULID.

### void ulid::EncodeEntropy(const std::function<uint8_t()>&, ULID&)

sets the last 10 bytes as the values generated using the passed random byte generator.

### void ulid::Encode(time_t, const std::function<uint8_t()>&, ULID&)

EncodeTime + EncodeEntropy

### void ulid::MarshalTo(const ULID&, char[26])

Marshals the ulid into the passed character array.

### std::string Marshal(const ULID&)

Marshals and generates std::string.
