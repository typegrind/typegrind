
#pragma once

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Rewrite/Core/Rewriter.h>

#include "MethodMatcher.h"
#include <unordered_set>

namespace typegrind
{
  class MethodDeclHandler : public clang::ast_matchers::MatchFinder::MatchCallback
  {
  public:

    MethodDeclHandler(clang::Rewriter*& rewriter, MethodMatcher const& matchers);
    void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;

    clang::StringRef getID() const override;

  private:
    clang::Rewriter*& mRewriter;
    std::unordered_set<unsigned> mAlreadyEncoded;
    MethodMatcher const& mMatchers;
  };
}
