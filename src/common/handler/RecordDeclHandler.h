
#pragma once

#include "MethodMatcher.h"
#include "BaseExprHandler.h"

namespace typegrind
{
  class RecordDeclHandler : public BaseExprHandler
  {
  public:

    RecordDeclHandler(clang::Rewriter*& rewriter, SpecializationHandler& specializationHandler);
    void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;
  };
}
