
#pragma once

#include "BaseExprHandler.h"
#include "typegrind/config/MethodMatcher.h"

namespace typegrind {
class FunctionDeclHandler : public BaseExprHandler {
 public:
  FunctionDeclHandler(clang::Rewriter*& rewriter, SpecializationHandler& specializationHandler,
                      typegrind::config::MethodMatcher const& matchers);
  void run(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;

 private:
  typegrind::config::MethodMatcher const& mMatchers;
};
}
