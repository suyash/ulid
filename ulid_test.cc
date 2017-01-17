#include <gtest/gtest.h>

#include "ulid.hh"

TEST(basic, 1) {
	ulid::ULID ulid = ulid::Create(std::time(nullptr), []() { return 4; });
	std::string str = ulid::Marshal(ulid);

	ASSERT_EQ(26, str.size());
	for (char c : str) {
		ASSERT_NE(std::string::npos, std::string(ulid::Encoding).find(c));
	}
}

TEST(Create, 1) {
	ulid::ULID ulid1;
	ulid::Encode(1484581420, []() { return 4; }, ulid1);

	auto ulid2 = ulid::Create(1484581420, []() { return 4; });

	ASSERT_EQ(0, ulid::CompareULIDs(ulid1, ulid2));
}

TEST(EncodeNow, 1) {
	ulid::ULID ulid;
	ulid::EncodeTimeNow(ulid);
	ulid::EncodeEntropy([]() { return 4; }, ulid);
	std::string str = ulid::Marshal(ulid);

	ASSERT_EQ(26, str.size());
	for (char c : str) {
		ASSERT_NE(std::string::npos, std::string(ulid::Encoding).find(c));
	}
}

TEST(EncodeEntropyRand, 1) {
	ulid::ULID ulid;
	ulid::EncodeTimeNow(ulid);
	ulid::EncodeEntropyRand(ulid);
	std::string str = ulid::Marshal(ulid);

	ASSERT_EQ(26, str.size());
	for (char c : str) {
		ASSERT_NE(std::string::npos, std::string(ulid::Encoding).find(c));
	}
}

TEST(EncodeNowRand, 1) {
	ulid::ULID ulid;
	ulid::EncodeNowRand(ulid);
	std::string str = ulid::Marshal(ulid);

	ASSERT_EQ(26, str.size());
	for (char c : str) {
		ASSERT_NE(std::string::npos, std::string(ulid::Encoding).find(c));
	}
}

TEST(CreateNowRand, 1) {
	ulid::ULID ulid = ulid::CreateNowRand();
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

TEST(Time, 1) {
	ulid::ULID ulid = ulid::Create(1484581420, []() { return 4; });
	ASSERT_EQ(1484581420, ulid::Time(ulid));
}
