# Based on the Makefile in googletest/make

# Points to the root of Google Test
GTEST_DIR = vendor/googletest

BENCHMARK_BUILD_DIR = vendor/benchmark_build

# Flags passed to the preprocessor.
CPPFLAGS += -isystem $(GTEST_DIR)/include

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread -std=c++11

# All tests produced by this Makefile.
TESTS = ulid_uint128_test ulid_struct_test

# All benchmarks produced by this Makefile.
BENCHS = ulid_uint128_bench ulid_struct_bench

# All Google Test headers.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# DO NOT TWEAK
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# General Tasks

all : test bench

clean :
	rm -f *_test.o *_test.out *_bench.o *_bench.out

clean-all : clean
	rm -f gtest.a gtest_main.a
	rm -rf $(BENCHMARK_BUILD_DIR)

test : $(TESTS)

bench : $(BENCHS)

# Tasks for gtest

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# Tasks to build benchmark

$(BENCHMARK_BUILD_DIR) : vendor/benchmark
	cd $< \
	&& cmake -DCMAKE_INSTALL_PREFIX=../../$@ -DCMAKE_BUILD_TYPE=Release . \
	&& make \
	&& make install

# Tasks for ulid_uint128_test

ulid_uint128_test.o : ulid_test.cc $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -DULIDUINT128 -c ulid_test.cc -o $@

ulid_uint128_test.out : gtest_main.a ulid_uint128_test.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

ulid_uint128_test : ulid_uint128_test.out
	./$<

# Tasks for ulid_struct_test

ulid_struct_test.o : ulid_test.cc $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c ulid_test.cc -o $@

ulid_struct_test.out : gtest_main.a ulid_struct_test.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

ulid_struct_test : ulid_struct_test.out
	./$<

# Tasks for ulid_uint128_bench

ulid_uint128_bench.o : ulid_bench.cc $(BENCHMARK_BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(BENCHMARK_BUILD_DIR)/include -DULIDUINT128 -c ulid_bench.cc -o $@

ulid_uint128_bench.out : ulid_uint128_bench.o
	$(CXX) $(CXXFLAGS) $^ -lpthread -L$(BENCHMARK_BUILD_DIR)/lib -lbenchmark -o $@

ulid_uint128_bench : ulid_uint128_bench.out
	./$< --benchmark_out_format=console

# Tasks for ulid_uint128_bench

ulid_struct_bench.o : ulid_bench.cc $(BENCHMARK_BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(BENCHMARK_BUILD_DIR)/include -c ulid_bench.cc -o $@

ulid_struct_bench.out : ulid_struct_bench.o
	$(CXX) $(CXXFLAGS) $^ -lpthread -L$(BENCHMARK_BUILD_DIR)/lib -lbenchmark -o $@

ulid_struct_bench : ulid_struct_bench.out
	./$< --benchmark_out_format=console
