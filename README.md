# ulid

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
