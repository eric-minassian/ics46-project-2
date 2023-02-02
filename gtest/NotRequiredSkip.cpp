#include "SkipList.hpp"
#include "gtest/gtest.h"
#include <cstdint>
#include <vector>

/*
NOTE:  these are not intended as exhaustive tests.
         You should make your own additional tests
         VERY IMPORTANT:  if your SkipList is not templated, or if
                        it is not built as a linked structure, your score for
this project will be zero.  Be sure your SkipList compiles and runs with
non-numeric data types.


NOTE ALSO:  "Not Required" does not mean "optional" or "extra credit."
"Not required" only means that we will still grade your project, even
if these exact tests do not pass.
*/

namespace {

TEST(SampleTests, SimpleHeightsTest) {
  SkipList<unsigned, unsigned> sl;
  std::vector<unsigned> heights;
  for (unsigned i = 0; i < 10; i++) {
    sl.insert(i, i);
    heights.push_back(sl.height(i));
  }
  std::vector<unsigned> expectedHeights = {1, 2, 1, 3, 1, 2, 1, 4, 1, 2};
  EXPECT_TRUE(heights == expectedHeights);
}

TEST(SampleTests, SimpleFindTest) {
  SkipList<unsigned, unsigned> sl;
  for (unsigned i = 0; i < 10; i++) {
    sl.insert(i, (100 + i));
  }
  for (unsigned i = 0; i < 10; i++) {
    EXPECT_TRUE((i + 100) == sl.find(i));
  }
}

TEST(SampleTests, SimpleLargestAndSmallest) {
  SkipList<unsigned, unsigned> sl;
  for (unsigned i = 0; i < 10; i++) {
    sl.insert(i, (100 + i));
  }
  EXPECT_TRUE(sl.isSmallestKey(0) and sl.isLargestKey(9));
}

TEST(SampleTests, InvolvedHeightsTest) {
  SkipList<unsigned, unsigned> sl;
  std::vector<unsigned> heights;
  for (unsigned i = 0; i < 10; i++) {
    sl.insert(i, i);
    heights.push_back(sl.height(i));
  }

  // The coinFlip function will always return heads
  // for 255 regardless of the current layer.
  // You can use this value to test your threshold for halting
  // the insertion procedure. If this results in an infinite loop inside
  // of your insert function you have not implemented a cutoff threshold.
  unsigned const MAGIC_VAL = 255;
  sl.insert(MAGIC_VAL, MAGIC_VAL);

  heights.push_back(sl.height(MAGIC_VAL));

  // The expected height for 255 is 12 because there are fewer than 16 nodes in
  // the skip list when 12 is added.
  std::vector<unsigned> expectedHeights = {1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 12};
  EXPECT_TRUE(heights == expectedHeights);

  // At this point there should be 13 layers
  // (because the fast lane is not included in the height calculation).
  EXPECT_TRUE(sl.numLayers() == 13);
}

TEST(SampleTests, Capacity17Test) {
  SkipList<unsigned, unsigned> sl;
  std::vector<unsigned> heights;

  // First insert 16 values into the skip list [0, 15].
  for (unsigned i = 0; i < 16; i++) {
    sl.insert(i, i);
    heights.push_back(sl.height(i));
  }

  // Same value used in SimpleHeightsTest for testing the threshold.
  unsigned const MAGIC_VAL = 255;
  sl.insert(MAGIC_VAL, MAGIC_VAL);

  heights.push_back(sl.height(MAGIC_VAL));

  // The expected height for 255 is 15 because 3 * ceil(log_2(17)) = 15
  // meaning the maximum height of any node should be 15 for a skip list with 17
  // nodes.
  std::vector<unsigned> expectedHeights = {1, 2, 1, 3, 1, 2, 1, 4, 1,
                                           2, 1, 3, 1, 2, 1, 5, 15};
  EXPECT_TRUE(heights == expectedHeights);

  // At this point there should be 16 layers
  // (because the fast lane is not included in the height calculation).
  EXPECT_TRUE(sl.numLayers() == 16);
}

TEST(Additional, Test1) {
  SkipList<std::string, std::string> sl;
  EXPECT_TRUE(sl.insert("A", "alligator"));
  EXPECT_TRUE(sl.insert("B", "bear"));
  EXPECT_TRUE(sl.insert("C", "cat"));
  EXPECT_TRUE(sl.insert("D", "dog"));
  EXPECT_TRUE(sl.insert("E", "elephant"));
  EXPECT_TRUE(sl.insert("F", "fox"));
  EXPECT_TRUE(sl.insert("G", "goat"));
  EXPECT_TRUE(sl.insert("H", "horse"));

  EXPECT_FALSE(sl.insert("A", "all"));
  EXPECT_FALSE(sl.insert("B", "baby"));
  EXPECT_FALSE(sl.insert("G", "giraffe"));

  EXPECT_TRUE(sl.find("A") == "alligator");
  EXPECT_TRUE(sl.find("B") == "bear");
  EXPECT_TRUE(sl.find("C") == "cat");
  EXPECT_TRUE(sl.find("D") == "dog");
  EXPECT_TRUE(sl.find("E") == "elephant");
  EXPECT_TRUE(sl.find("F") == "fox");
  EXPECT_TRUE(sl.find("G") == "goat");
  EXPECT_TRUE(sl.find("H") == "horse");
}

TEST(Additional, Test2) {
  SkipList<std::string, std::string> sl;
  EXPECT_TRUE(sl.insert("A", "alligator"));
  EXPECT_TRUE(sl.insert("B", "bear"));
  EXPECT_TRUE(sl.insert("C", "cat"));
  EXPECT_TRUE(sl.insert("D", "dog"));
  EXPECT_TRUE(sl.insert("E", "elephant"));
  EXPECT_TRUE(sl.insert("F", "fox"));
  EXPECT_TRUE(sl.insert("G", "goat"));
  EXPECT_TRUE(sl.insert("H", "horse"));

  EXPECT_TRUE(sl.isSmallestKey("A"));
  EXPECT_TRUE(sl.isLargestKey("H"));

  EXPECT_FALSE(sl.isSmallestKey("B"));
  EXPECT_FALSE(sl.isLargestKey("G"));
}

TEST(Additional, Test3) {
  SkipList<std::string, std::string> sl;
  EXPECT_TRUE(sl.insert("A", "alligator"));
  EXPECT_TRUE(sl.insert("B", "bear"));
  EXPECT_TRUE(sl.insert("C", "cat"));
  EXPECT_TRUE(sl.insert("D", "dog"));
  EXPECT_TRUE(sl.insert("E", "elephant"));
  EXPECT_TRUE(sl.insert("F", "fox"));
  EXPECT_TRUE(sl.insert("G", "goat"));
  EXPECT_TRUE(sl.insert("H", "horse"));

  EXPECT_TRUE(sl.height("A") == 2);
  EXPECT_TRUE(sl.height("B") == 1);
  EXPECT_TRUE(sl.height("C") == 3);
  EXPECT_TRUE(sl.height("D") == 1);
  EXPECT_TRUE(sl.height("E") == 2);
  EXPECT_TRUE(sl.height("F") == 1);
  EXPECT_TRUE(sl.height("G") == 4);
  EXPECT_TRUE(sl.height("H") == 1);
}

TEST(Additional, Test4) {
  SkipList<std::string, std::string> sl;
  EXPECT_TRUE(sl.insert("A", "alligator"));
  EXPECT_TRUE(sl.insert("B", "bear"));
  EXPECT_TRUE(sl.insert("C", "cat"));
  EXPECT_TRUE(sl.insert("D", "dog"));
  EXPECT_TRUE(sl.insert("E", "elephant"));
  EXPECT_TRUE(sl.insert("F", "fox"));
  EXPECT_TRUE(sl.insert("G", "goat"));
  EXPECT_TRUE(sl.insert("H", "horse"));

  EXPECT_TRUE(sl.numLayers() == 5);
}

TEST(Additional, Test5) {
  SkipList<unsigned, unsigned> sl;
  for (unsigned i = 0; i < 10; i++) {
    sl.insert(i, (100 + i));
  }

  EXPECT_EQ(sl.find(0), 100);
  EXPECT_EQ(sl.find(1), 101);
  EXPECT_EQ(sl.find(2), 102);
  EXPECT_EQ(sl.find(5), 105);
  EXPECT_EQ(sl.find(9), 109);
}

TEST(Additional, Test6) {
  SkipList<unsigned, unsigned> sl;
  for (unsigned i = 0; i < 10; i++) {
    sl.insert(i, (100 + i));
  }

  EXPECT_TRUE(sl.isSmallestKey(0));
  EXPECT_TRUE(sl.isLargestKey(9));

  EXPECT_FALSE(sl.isSmallestKey(1));
  EXPECT_FALSE(sl.isLargestKey(8));
}

TEST(Additional, Test7) {
  SkipList<unsigned, unsigned> sl;
  for (unsigned i = 0; i < 10; i++) {
    sl.insert(i, (100 + i));
  }

  EXPECT_TRUE(sl.height(0) == 1);
  EXPECT_TRUE(sl.height(1) == 2);
  EXPECT_TRUE(sl.height(2) == 1);
  EXPECT_TRUE(sl.height(3) == 3);
  EXPECT_TRUE(sl.height(5) == 2);
  EXPECT_TRUE(sl.height(7) == 4);
  EXPECT_TRUE(sl.height(9) == 2);
}

TEST(Additional, Test8) {
  SkipList<std::string, unsigned> sl;

  EXPECT_EQ(sl.size(), 0);
  EXPECT_TRUE(sl.isEmpty());

  EXPECT_TRUE(sl.insert("A", 100));
  EXPECT_TRUE(sl.insert("B", 200));
  EXPECT_TRUE(sl.insert("C", 300));
  EXPECT_TRUE(sl.insert("D", 400));
  EXPECT_TRUE(sl.insert("E", 500));
  EXPECT_TRUE(sl.insert("F", 600));
  EXPECT_TRUE(sl.insert("G", 700));
  EXPECT_TRUE(sl.insert("AB", 800));
  EXPECT_TRUE(sl.insert("AC", 900));
  EXPECT_TRUE(sl.insert("AD", 1000));
  EXPECT_TRUE(sl.insert("AE", 1100));

  EXPECT_FALSE(sl.insert("A", 1200));
  EXPECT_FALSE(sl.insert("B", 1300));
  EXPECT_FALSE(sl.insert("C", 1400));
  EXPECT_FALSE(sl.insert("AE", 1500));

  EXPECT_TRUE(sl.find("A") == 100);
  EXPECT_TRUE(sl.find("B") == 200);
  EXPECT_TRUE(sl.find("C") == 300);
  EXPECT_TRUE(sl.find("D") == 400);
  EXPECT_TRUE(sl.find("E") == 500);
  EXPECT_TRUE(sl.find("F") == 600);
  EXPECT_TRUE(sl.find("G") == 700);
  EXPECT_TRUE(sl.find("AB") == 800);
  EXPECT_TRUE(sl.find("AC") == 900);
  EXPECT_TRUE(sl.find("AD") == 1000);
  EXPECT_TRUE(sl.find("AE") == 1100);

  EXPECT_TRUE(sl.isSmallestKey("A"));
  EXPECT_TRUE(sl.isLargestKey("G"));

  EXPECT_FALSE(sl.isSmallestKey("AB"));
  EXPECT_FALSE(sl.isLargestKey("F"));

  EXPECT_EQ(sl.size(), 11);
  EXPECT_FALSE(sl.isEmpty());
}

TEST(Additional, Test9) {
  SkipList<unsigned, unsigned> sl;
  for (unsigned i = 0; i < 1000; i++) {
    EXPECT_TRUE(sl.insert(i, (100 + i)));
  }

  EXPECT_EQ(sl.size(), 1000);
  EXPECT_FALSE(sl.isEmpty());

  for (unsigned i = 0; i < 1000; i++) {
    EXPECT_TRUE(sl.find(i) == (100 + i));
  }

  EXPECT_TRUE(sl.isSmallestKey(0));
  EXPECT_TRUE(sl.isLargestKey(999));

  EXPECT_FALSE(sl.isSmallestKey(1));
  EXPECT_FALSE(sl.isLargestKey(998));
}

TEST(Additional, Test10) {
  SkipList<unsigned, unsigned> sl;
  for (unsigned i = 1; i < 11; i++) {
    EXPECT_TRUE(sl.insert(i, (100 + i)));
  }

  EXPECT_EQ(sl.numLayers(), 5);
  EXPECT_EQ(sl.size(), 10);
  EXPECT_FALSE(sl.isEmpty());

  for (unsigned i = 1; i < 11; i++) {
    EXPECT_TRUE(sl.find(i) == (100 + i));
  }

  EXPECT_THROW(sl.find(11), RuntimeException);
  EXPECT_THROW(sl.find(12), RuntimeException);
  EXPECT_THROW(sl.find(0), RuntimeException);

  EXPECT_THROW(sl.height(11), RuntimeException);
  EXPECT_THROW(sl.height(12), RuntimeException);
  EXPECT_THROW(sl.height(0), RuntimeException);
}

TEST(Additional, Test11) {
  SkipList<unsigned, unsigned> sl;
  for (unsigned i = 1; i < 11; i++) {
    EXPECT_TRUE(sl.insert(i, (100 + i)));
  }

  EXPECT_EQ(sl.nextKey(1), 2);
  EXPECT_EQ(sl.nextKey(2), 3);
  EXPECT_EQ(sl.nextKey(3), 4);

  EXPECT_EQ(sl.previousKey(10), 9);
  EXPECT_EQ(sl.previousKey(9), 8);
  EXPECT_EQ(sl.previousKey(8), 7);

  EXPECT_THROW(sl.nextKey(10), RuntimeException);
  EXPECT_THROW(sl.nextKey(11), RuntimeException);

  EXPECT_THROW(sl.previousKey(1), RuntimeException);
  EXPECT_THROW(sl.previousKey(0), RuntimeException);

  EXPECT_THROW(sl.nextKey(0), RuntimeException);
  EXPECT_THROW(sl.previousKey(11), RuntimeException);
}

TEST(Additional, Test12) {
  SkipList<unsigned, unsigned> sl;
  for (unsigned i = 0; i < 10; i++) {
    EXPECT_TRUE(sl.insert(i, (100 + i)));
  }

  EXPECT_EQ(sl.nextKey(0), 1);
  EXPECT_EQ(sl.nextKey(1), 2);

  EXPECT_EQ(sl.previousKey(9), 8);
  EXPECT_EQ(sl.previousKey(8), 7);

  EXPECT_THROW(sl.nextKey(10), RuntimeException);
  EXPECT_THROW(sl.nextKey(11), RuntimeException);

  EXPECT_THROW(sl.previousKey(0), RuntimeException);
  EXPECT_THROW(sl.previousKey(-1), RuntimeException);

  EXPECT_THROW(sl.nextKey(-1), RuntimeException);
  EXPECT_THROW(sl.previousKey(10), RuntimeException);
}

TEST(Additional, Test13) {
  SkipList<unsigned, unsigned> sl;

  EXPECT_EQ(sl.size(), 0);
  EXPECT_TRUE(sl.isEmpty());

  for (unsigned i = 0; i < 10; i++) {
    EXPECT_TRUE(sl.insert(i, (100 + i)));
    EXPECT_EQ(sl.size(), i + 1);
    EXPECT_FALSE(sl.isEmpty());
  }
}

TEST(Additional, Test14) {
  SkipList<unsigned, unsigned> sl;

  for (unsigned i = 0; i < 10; i++) {
    EXPECT_TRUE(sl.insert(i, (100 + i)));
  }

  EXPECT_EQ(sl.numLayers(), 5);

  for (unsigned i = 20; i < 30; i++) {
    EXPECT_TRUE(sl.insert(i, (100 + i)));
  }

  EXPECT_EQ(sl.numLayers(), 5);
}

TEST(Additional, Test15) {
  SkipList<unsigned, unsigned> sl;

  EXPECT_EQ(sl.numLayers(), 2);

  for (unsigned i = 0; i < 100; i++) {
    EXPECT_TRUE(sl.insert(i, (100 + i)));
  }

  EXPECT_EQ(sl.numLayers(), 8);
}

TEST(Additional, Test16) {
  SkipList<unsigned, unsigned> sl;

  for (unsigned i = 1; i < 101; i++) {
    EXPECT_TRUE(sl.insert(i, (100 + i)));
  }

  EXPECT_THROW(sl.height(0), RuntimeException);
  EXPECT_THROW(sl.height(101), RuntimeException);

  for (unsigned i = 101; i < 200; i++) {
    EXPECT_THROW(sl.height(i), RuntimeException);
  }

  EXPECT_EQ(sl.height(63), 7);
  EXPECT_EQ(sl.height(95), 6);
  EXPECT_EQ(sl.height(31), 6);
  EXPECT_EQ(sl.height(15), 5);
  EXPECT_EQ(sl.height(47), 5);

  EXPECT_EQ(sl.height(1), 2);
  EXPECT_EQ(sl.height(2), 1);
  EXPECT_EQ(sl.height(3), 3);

  EXPECT_EQ(sl.height(97), 2);
  EXPECT_EQ(sl.height(98), 1);
  EXPECT_EQ(sl.height(99), 3);
}

TEST(Additional, Test17) {
  SkipList<unsigned, unsigned> sl;

  for (unsigned i = 1; i < 101; i++) {
    EXPECT_TRUE(sl.insert(i, (100 + i)));
  }

  EXPECT_THROW(sl.nextKey(0), RuntimeException);
  EXPECT_THROW(sl.nextKey(101), RuntimeException);

  sl.insert(0, 100);

  for (unsigned i = 0; i < 100; i++) {
    EXPECT_EQ(sl.nextKey(i), i + 1);
  }

  EXPECT_THROW(sl.nextKey(100), RuntimeException);
}

TEST(Additional, Test18) {
  SkipList<unsigned, unsigned> sl;

  for (unsigned i = 1; i < 101; i++) {
    EXPECT_TRUE(sl.insert(i, (100 + i)));
  }

  EXPECT_THROW(sl.previousKey(0), RuntimeException);
  EXPECT_THROW(sl.previousKey(101), RuntimeException);

  sl.insert(0, 100);

  for (unsigned i = 1; i < 101; i++) {
    EXPECT_EQ(sl.previousKey(i), i - 1);
  }

  EXPECT_THROW(sl.previousKey(0), RuntimeException);
}

TEST(Additional, Test19) {
  SkipList<unsigned, unsigned> sl;

  for (unsigned i = 1; i < 101; i++) {
    EXPECT_TRUE(sl.insert(i, (100 + i)));
  }

  EXPECT_THROW(sl.find(0), RuntimeException);
  EXPECT_THROW(sl.find(101), RuntimeException);

  sl.insert(0, 100);

  for (unsigned i = 0; i < 101; i++) {
    EXPECT_EQ(sl.find(i), 100 + i);
  }

  EXPECT_THROW(sl.find(101), RuntimeException);

  sl.insert(101, 201);

  for (unsigned i = 0; i < 102; i++) {
    EXPECT_EQ(sl.find(i), 100 + i);
  }
}

TEST(Additional, Test20) {
  SkipList<unsigned, unsigned> sl;

  for (unsigned i = 0; i < 101; i++) {
    EXPECT_TRUE(sl.insert(i, (100 + i)));
  }

  for (unsigned i = 0; i < 101; i++) {
    EXPECT_EQ(sl.find(i), 100 + i);
    unsigned &value = sl.find(i);
    value = 200 + i;
  }

  for (unsigned i = 0; i < 101; i++) {
    EXPECT_EQ(sl.find(i), 200 + i);
  }
}

TEST(Additional, Test21) {
  SkipList<unsigned, unsigned> sl;

  for (unsigned i = 0; i < 101; i++) {
    EXPECT_TRUE(sl.insert(i, (100 + i)));
  }

  const SkipList<unsigned, unsigned> &csl = sl;

  for (unsigned i = 0; i < 101; i++) {
    EXPECT_EQ(csl.find(i), 100 + i);
  }
}

TEST(Additional, Test22) {
  SkipList<unsigned, unsigned> sl;

  EXPECT_THROW(sl.isSmallestKey(0), RuntimeException);
  EXPECT_THROW(sl.isSmallestKey(1), RuntimeException);

  EXPECT_THROW(sl.isLargestKey(0), RuntimeException);
  EXPECT_THROW(sl.isLargestKey(1), RuntimeException);

  for (unsigned i = 1; i < 101; i++) {
    EXPECT_TRUE(sl.insert(i, (100 + i)));
  }

  EXPECT_THROW(sl.isSmallestKey(0), RuntimeException);
  EXPECT_THROW(sl.isSmallestKey(101), RuntimeException);

  EXPECT_THROW(sl.isLargestKey(0), RuntimeException);
  EXPECT_THROW(sl.isLargestKey(101), RuntimeException);

  EXPECT_TRUE(sl.isSmallestKey(1));
  EXPECT_TRUE(sl.isLargestKey(100));

  sl.insert(0, 100);
  sl.insert(101, 201);

  EXPECT_TRUE(sl.isSmallestKey(0));
  EXPECT_TRUE(sl.isLargestKey(101));

  EXPECT_FALSE(sl.isSmallestKey(1));
  EXPECT_FALSE(sl.isLargestKey(100));
}

TEST(Additional, Test23) {
  SkipList<unsigned, unsigned> sl;
  std::vector<unsigned> keys;

  for (unsigned i = 100; i < 200; i++) {
    EXPECT_TRUE(sl.insert(i, (100 + i)));
  }

  for (unsigned i = 0; i < 100; i++) {
    EXPECT_TRUE(sl.insert(i, (100 + i)));
  }

  for (unsigned i = 0; i < 200; i++) {
    keys.push_back(i);
  }

  EXPECT_EQ(sl.allKeysInOrder(), keys);
}

// Test 23 but keys of strings instead of unsigned integers
TEST(Additional, Test24) {
  SkipList<std::string, unsigned> sl;
  std::vector<std::string> keys;

  for (unsigned i = 100; i < 200; i++) {
    EXPECT_TRUE(sl.insert(std::to_string(i), (100 + i)));
  }

  for (unsigned i = 0; i < 100; i++) {
    EXPECT_TRUE(sl.insert(std::to_string(i), (100 + i)));
  }

  for (unsigned i = 0; i < 200; i++) {
    keys.push_back(std::to_string(i));
  }

  for (unsigned i = 0; i < 200; i++) {
    EXPECT_EQ(sl.find(std::to_string(i)), 100 + i);
  }
}

TEST(Additional, Test25) {
  SkipList<std::string, std::string> sl;
  std::vector<std::string> keys;

  EXPECT_TRUE(sl.insert("a", "alpha"));
  EXPECT_TRUE(sl.insert("b", "beta"));
  EXPECT_TRUE(sl.insert("bb", "beta"));
  EXPECT_TRUE(sl.insert("e", "epsilon"));
  EXPECT_TRUE(sl.insert("k", "lambda"));
  EXPECT_TRUE(sl.insert("j", "kappa"));
  EXPECT_TRUE(sl.insert("i", "iota"));
  EXPECT_TRUE(sl.insert("f", "zeta"));
  EXPECT_TRUE(sl.insert("g", "eta"));
  EXPECT_TRUE(sl.insert("h", "theta"));
  EXPECT_TRUE(sl.insert("c", "gamma"));
  EXPECT_TRUE(sl.insert("d", "delta"));
  EXPECT_TRUE(sl.insert("cc", "gamma"));
  EXPECT_TRUE(sl.insert("aa", "alpha"));
  EXPECT_TRUE(sl.insert("dd", "delta"));

  keys.push_back("a");
  keys.push_back("aa");
  keys.push_back("b");
  keys.push_back("bb");
  keys.push_back("c");
  keys.push_back("cc");
  keys.push_back("d");
  keys.push_back("dd");
  keys.push_back("e");
  keys.push_back("f");
  keys.push_back("g");
  keys.push_back("h");
  keys.push_back("i");
  keys.push_back("j");
  keys.push_back("k");

  EXPECT_EQ(sl.allKeysInOrder(), keys);
}

TEST(Additional, Test26) {
  SkipList<unsigned, unsigned> sl;

  for (unsigned i = 1; i < 16; i++) {
    EXPECT_TRUE(sl.insert(i, (100 + i)));
  }

  EXPECT_EQ(sl.numLayers(), 6);

  sl.insert(255, 355);

  EXPECT_EQ(sl.numLayers(), 13);
  EXPECT_EQ(sl.size(), 16);
}

TEST(Additional, Test27) {
  SkipList<unsigned, unsigned> sl;

  for (unsigned i = 0; i < 16; i++) {
    EXPECT_TRUE(sl.insert(i, (100 + i)));
  }

  sl.insert(255, 355);

  EXPECT_EQ(sl.numLayers(), 16);
}

TEST(Additional, Test28) {
  SkipList<unsigned, unsigned> sl;

  for (unsigned i = 0; i < 100; i++) {
    EXPECT_TRUE(sl.insert(i, (100 + i)));
  }

  sl.insert(255, 355);

  EXPECT_EQ(sl.numLayers(), 22);
}

TEST(Additional, Test29) {
  SkipList<unsigned, unsigned> sl;

  for (unsigned i = 0; i < 1000; i++) {
    EXPECT_TRUE(sl.insert(i, (100 + i)));
  }

  sl.insert(255, 355);

  EXPECT_EQ(sl.numLayers(), 31);
}

TEST(Additional, Test30) {
  SkipList<unsigned, unsigned> sl;

  for (unsigned i = 0; i < 17; i++) {
    EXPECT_TRUE(sl.insert(i, (100 + i)));
  }

  sl.insert(255, 355);

  EXPECT_EQ(sl.numLayers(), 16);
}

TEST(Additional, Test31) {
  SkipList<unsigned, unsigned> sl;

  for (unsigned i = 0; i < 18; i++) {
    EXPECT_TRUE(sl.insert(i, (100 + i)));
  }

  sl.insert(255, 355);

  EXPECT_EQ(sl.numLayers(), 16);
}

TEST(Additional, Test32) {
  SkipList<unsigned, unsigned> sl;

  for (unsigned i = 0; i < 31; i++) {
    EXPECT_TRUE(sl.insert(i, (100 + i)));
  }

  sl.insert(255, 355);

  EXPECT_EQ(sl.numLayers(), 16);
}

TEST(Additional, Test33) {
  SkipList<unsigned, unsigned> sl;

  for (unsigned i = 0; i < 32; i++) {
    EXPECT_TRUE(sl.insert(i, (100 + i)));
  }

  sl.insert(255, 355);

  EXPECT_EQ(sl.numLayers(), 19);
}

TEST(Additional, Test34) {
  SkipList<unsigned, unsigned> sl;

  for (unsigned i = 0; i < 12; i++) {
    EXPECT_TRUE(sl.insert(i, (100 + i)));
  }

  sl.insert(255, 355);

  EXPECT_EQ(sl.numLayers(), 13);
}

TEST(Additional, Test35) {
  SkipList<unsigned, unsigned> sl;

  sl.insert(255, 355);

  EXPECT_EQ(sl.numLayers(), 13);
}

TEST(Additional, Test36) {
  SkipList<std::string, std::string> sl;

  EXPECT_TRUE(sl.insert("a", "alpha"));
  EXPECT_TRUE(sl.insert("b", "beta"));
  EXPECT_TRUE(sl.insert("bb", "beta"));
  EXPECT_TRUE(sl.insert("e", "epsilon"));
  EXPECT_TRUE(sl.insert("k", "lambda"));
  EXPECT_TRUE(sl.insert("j", "kappa"));
  EXPECT_TRUE(sl.insert("i", "iota"));
  EXPECT_TRUE(sl.insert("f", "zeta"));
  EXPECT_TRUE(sl.insert("g", "eta"));
  EXPECT_TRUE(sl.insert("h", "theta"));
  EXPECT_TRUE(sl.insert("c", "gamma"));
  EXPECT_TRUE(sl.insert("d", "delta"));
  EXPECT_TRUE(sl.insert("cc", "gamma"));
  EXPECT_TRUE(sl.insert("aa", "alpha"));
  EXPECT_TRUE(sl.insert("dd", "delta"));

  EXPECT_FALSE(sl.insert("a", "alpha"));
  EXPECT_FALSE(sl.insert("b", "beta"));
  EXPECT_FALSE(sl.insert("bb", "beta"));
  EXPECT_FALSE(sl.insert("e", "epsilon"));
  EXPECT_FALSE(sl.insert("k", "lambda"));

  EXPECT_EQ(sl.find("a"), "alpha");
  EXPECT_EQ(sl.find("b"), "beta");
  EXPECT_EQ(sl.find("bb"), "beta");
  EXPECT_EQ(sl.find("e"), "epsilon");
  EXPECT_EQ(sl.find("k"), "lambda");

  EXPECT_THROW(sl.find("l"), RuntimeException);
  EXPECT_THROW(sl.find("m"), RuntimeException);
  EXPECT_THROW(sl.find("n"), RuntimeException);
}

TEST(Additional, Test37) {
  SkipList<unsigned, unsigned> sl;

  for (unsigned i = 0; i < 1000000; i++) {
    EXPECT_TRUE(sl.insert(i, i));
  }

  for (unsigned i = 0; i < 1000000; i++) {
    EXPECT_EQ(sl.find(i), i);
  }

  EXPECT_LE(sl.numLayers(), 61);

  EXPECT_THROW(sl.find(1000000), RuntimeException);
}

} // namespace
