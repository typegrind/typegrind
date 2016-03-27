
#include "MethodMatcher.h"
#include "gtest/gtest.h"

TEST(MethodMatcher, AcceptsValidRegex)
{
  typegrind::MethodMatcher mm;
  EXPECT_TRUE(!!mm.add({ "asdf", "", 0 }));
}

TEST(MethodMatcher, RejectsInvalidRegex)
{
  typegrind::MethodMatcher mm;
  EXPECT_FALSE(!!mm.add({ "*", "", 0 }));
}

TEST(MethodMatcher, MatchesValidRegex)
{
  typegrind::MethodMatcher mm;
  mm.add({ "asdf.*", "", 0 });
  EXPECT_TRUE(!!mm.matches("asdf_fdsa"));
}

TEST(MethodMatcher, WontMatchNonMatching)
{
  typegrind::MethodMatcher mm;
  mm.add({ "asdf.*", "", 0 });
  EXPECT_FALSE(!!mm.matches("fdsa_asdf"));
}

TEST(MethodMatcher, ReturnsCorrectCustomName)
{
  typegrind::MethodMatcher mm;
  mm.add({ "asdf.*", "thisIsAName", 0 });
  mm.add({ "fdsa.*", "thisIsAnotherName", 0 });
  EXPECT_EQ(mm.matches("asdf_fdsa")->custom_name, "thisIsAName");
}

TEST(MethodMatcher, ReturnsCorrectFlags)
{
  typegrind::MethodMatcher mm;
  mm.add({ "fdsa.*", "", 314 });
  mm.add({ "asdf.*", "", 1337 });
  EXPECT_EQ(mm.matches("asdf_fdsa")->flags, 1337);
}

