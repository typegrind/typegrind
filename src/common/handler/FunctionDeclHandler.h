
#pragma once

#include "MethodMatcher.h"
#include "BaseExprHandler.h"

namespace typegrind
{
  class FunctionDeclHandler : public BaseExprHandler
  {
  public:

    FunctionDeclHandler(clang::Rewriter*& rewriter, SpecializationHandler& specializationHandler, MethodMatcher const& matchers);
    void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;
  private:
    MethodMatcher const& mMatchers;
  };
}
