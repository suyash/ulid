#include <gtest/gtest.h>

#include <ctime>

#include "ulid.hh"

TEST(basic, 1) {
	ulid::ULID ulid;
	ulid::Encode(std::time(nullptr), []() { return 4; }, ulid);
	std::string str = ulid::Marshal(ulid);

	ASSERT_EQ(26, str.size());
	for (char c : str) {
		ASSERT_NE(std::string::npos, std::string(ulid::Encoding).find(c));
	}
}

TEST(Unmarshal, 1) {
	ulid::ULID ulid = ulid::Unmarshal("0001C7STHC0G2081040G208104");

	ulid::ULID ulid_expected;
	ulid::Encode(1484581420, []() { return 4; }, ulid_expected);

	ASSERT_EQ(0, ulid::CompareULIDs(ulid_expected, ulid));
}
