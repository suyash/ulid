# Based on the Makefile in googletest/make

# Points to the root of Google Test
GTEST_DIR = vendor/googletest

BENCHMARK_BUILD_DIR = vendor/benchmark_build

# Flags passed to the preprocessor.
CPPFLAGS += -isystem $(GTEST_DIR)/include -std=c++11

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread -std=c++11

# All tests produced by this Makefile.
TESTS = ulid_test

# All benchmarks produced by this Makefile.
BENCHS = ulid_bench

# All Google Test headers.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# DO NOT TWEAK
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# General Tasks

all : test

clean :
	rm -f $(TESTS) gtest.a gtest_main.a *.o *.out

test : $(TESTS)

bench : $(BENCHS)

# Tasks for gtest

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# Tasks to build benchmark

$(BENCHMARK_BUILD_DIR) : vendor/benchmark
	cd $< \
	&& cmake -D CMAKE_INSTALL_PREFIX=../../$@ . \
	&& make \
	&& make install

# Tasks for ulid_test

ulid_test.o : ulid_test.cc $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c ulid_test.cc

ulid_test.out : gtest_main.a ulid_test.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

ulid_test : ulid_test.out
	./$<

# Tasks for ulid_bench

ulid_bench.o : ulid_bench.cc $(BENCHMARK_BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(BENCHMARK_BUILD_DIR)/include -c ulid_bench.cc

ulid_bench.out : ulid_bench.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -lpthread -L$(BENCHMARK_BUILD_DIR)/lib -lbenchmark -o $@

ulid_bench : ulid_bench.out
	./$<
