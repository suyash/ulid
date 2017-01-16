#include <gtest/gtest.h>

#include <ctime>

#include "ulid.hh"

TEST(ulid, 1) {
	ulid::ULID ulid;
	ulid::Encode(std::time(nullptr), []() { return 4; }, ulid);
	std::string str = ulid::Marshal(ulid);

	ASSERT_EQ(26, str.size());
	for (char c : str) {
		ASSERT_NE(std::string::npos, ulid::Encoding.find(c));
	}
}
