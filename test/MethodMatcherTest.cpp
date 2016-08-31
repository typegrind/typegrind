
#include "typegrind/config/MethodMatcher.h"
#include "gtest/gtest.h"

using namespace typegrind::config;

TEST(MethodMatcher, AcceptsValidRegex) {
  MethodMatcher mm;
  EXPECT_TRUE(!!mm.add({"asdf", "", 0}));
}

TEST(MethodMatcher, RejectsInvalidRegex) {
  MethodMatcher mm;
  EXPECT_FALSE(!!mm.add({"*", "", 0}));
}

TEST(MethodMatcher, MatchesValidRegex) {
  MethodMatcher mm;
  mm.add({"asdf.*", "", 0});
  EXPECT_TRUE(!!mm.matches("asdf_fdsa"));
}

TEST(MethodMatcher, WontMatchNonMatching) {
  MethodMatcher mm;
  mm.add({"asdf.*", "", 0});
  EXPECT_FALSE(!!mm.matches("fdsa_asdf"));
}

TEST(MethodMatcher, ReturnsCorrectCustomName) {
  MethodMatcher mm;
  mm.add({"asdf.*", "thisIsAName", 0});
  mm.add({"fdsa.*", "thisIsAnotherName", 0});
  EXPECT_EQ(mm.matches("asdf_fdsa")->custom_name, "thisIsAName");
}

TEST(MethodMatcher, ReturnsCorrectFlags) {
  MethodMatcher mm;
  mm.add({"fdsa.*", "", 314});
  mm.add({"asdf.*", "", 1337});
  EXPECT_EQ(mm.matches("asdf_fdsa")->flags, 1337);
}
