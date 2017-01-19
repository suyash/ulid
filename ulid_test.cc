#include <gtest/gtest.h>

#include <thread>

#ifdef ULIDUINT128
#include "ulid_uint128.hh"
#else
#include "ulid_struct.hh"
#endif // ULIDUINT128

TEST(basic, 1) {
	ulid::ULID ulid = ulid::Create(std::time(nullptr), []() { return 4; });
	std::string str = ulid::Marshal(ulid);

	ASSERT_EQ(26, str.size());
	for (char c : str) {
		ASSERT_NE(std::string::npos, std::string(ulid::Encoding).find(c));
	}
}

TEST(Create, 1) {
	ulid::ULID ulid1 = 0;
	ulid::Encode(1484581420, []() { return 4; }, ulid1);

	auto ulid2 = ulid::Create(1484581420, []() { return 4; });

	ASSERT_EQ(0, ulid::CompareULIDs(ulid1, ulid2));
}

TEST(EncodeTimeNow, 1) {
	ulid::ULID ulid = 0;
	ulid::EncodeTimeNow(ulid);
	ulid::EncodeEntropy([]() { return 4; }, ulid);
	std::string str = ulid::Marshal(ulid);

	ASSERT_EQ(26, str.size());
	for (char c : str) {
		ASSERT_NE(std::string::npos, std::string(ulid::Encoding).find(c));
	}
}

TEST(EncodeTimeSystemClockNow, 1) {
	ulid::ULID ulid = 0;
	ulid::EncodeTimeSystemClockNow(ulid);
	ulid::EncodeEntropy([]() { return 4; }, ulid);
	std::string str = ulid::Marshal(ulid);

	ASSERT_EQ(26, str.size());
	for (char c : str) {
		ASSERT_NE(std::string::npos, std::string(ulid::Encoding).find(c));
	}
}

TEST(EncodeEntropyRand, 1) {
	ulid::ULID ulid = 0;
	ulid::EncodeTimeNow(ulid);
	ulid::EncodeEntropyRand(ulid);
	std::string str = ulid::Marshal(ulid);

	ASSERT_EQ(26, str.size());
	for (char c : str) {
		ASSERT_NE(std::string::npos, std::string(ulid::Encoding).find(c));
	}
}

TEST(EncodeEntropyRand, 2) {
	time_t timestamp = 1000000;
	auto duration = std::chrono::seconds(timestamp);
	auto nsduration = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
	auto msduration = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

	ulid::ULID ulid1 = 0;
	ulid::EncodeTime(msduration.count(), ulid1);

	std::srand(nsduration.count());
	ulid::EncodeEntropyRand(ulid1);

	ulid::ULID ulid2 = 0;
	ulid::EncodeTime(msduration.count(), ulid2);

	std::srand(nsduration.count());
	ulid::EncodeEntropyRand(ulid2);

	ASSERT_EQ(0, ulid::CompareULIDs(ulid1, ulid2));
}

TEST(EncodeEntropyMt19937, 1) {
	ulid::ULID ulid = 0;
	ulid::EncodeTimeNow(ulid);

	std::mt19937 generator(4);
	ulid::EncodeEntropyMt19937(generator, ulid);

	std::string str = ulid::Marshal(ulid);
	ASSERT_EQ(26, str.size());
	for (char c : str) {
		ASSERT_NE(std::string::npos, std::string(ulid::Encoding).find(c));
	}
}

TEST(EncodeNowRand, 1) {
	ulid::ULID ulid = 0;
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

TEST(MarshalBinary, 1) {
	ulid::ULID ulid = ulid::Create(1484581420, []() { return 4; });
	std::vector<uint8_t> b = ulid::MarshalBinary(ulid);

#ifdef ULIDUINT128
	for (int i = 15 ; i >= 0 ; i--) {
		ASSERT_EQ(static_cast<uint8_t>(ulid), b[i]);
		ulid >>= 8;
	}
#else
	for (int i = 0 ; i < 16 ; i++) {
		ASSERT_EQ(ulid.data[i], b[i]);
	}
#endif // ULIDUINT128
}

TEST(Unmarshal, 1) {
	ulid::ULID ulid = ulid::Unmarshal("0001C7STHC0G2081040G208104");
	ulid::ULID ulid_expected = ulid::Create(1484581420, []() { return 4; });
	ASSERT_EQ(0, ulid::CompareULIDs(ulid_expected, ulid));
}

TEST(UnmarshalBinary, 1) {
	ulid::ULID ulid_expected = ulid::Create(1484581420, []() { return 4; });
	std::vector<uint8_t> b = ulid::MarshalBinary(ulid_expected);
	ulid::ULID ulid = ulid::UnmarshalBinary(b);
	ASSERT_EQ(0, ulid::CompareULIDs(ulid_expected, ulid));
}

TEST(Time, 1) {
	ulid::ULID ulid = ulid::Create(1484581420, []() { return 4; });
	ASSERT_EQ(1484581420, ulid::Time(ulid));
}

// https://github.com/oklog/ulid/blob/master/ulid_test.go#L160-L169
TEST(AlizainCompatibility, 1) {
	ulid::ULID ulid_got = 0;
	ulid::EncodeTime(uint64_t(1469918176385), ulid_got);

	ulid::ULID ulid_want = ulid::Unmarshal("01ARYZ6S410000000000000000");
	ASSERT_EQ(0, ulid::CompareULIDs(ulid_want, ulid_got));
}

TEST(LexicographicalOrder, 1) {
	ulid::ULID ulid1 = ulid::CreateNowRand();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	ulid::ULID ulid2 = ulid::CreateNowRand();

	EXPECT_EQ(-1, ulid::CompareULIDs(ulid1, ulid2));
	EXPECT_EQ(1, ulid::CompareULIDs(ulid2, ulid1));
}
