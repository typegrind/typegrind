#ifndef TYPEGRIND_DELETEEXPRHANDLER_H
#define TYPEGRIND_DELETEEXPRHANDLER_H

#include "BaseExprHandler.h"

namespace typegrind {
class DeleteExprHandler : public BaseExprHandler {
 public:
  DeleteExprHandler(clang::Rewriter*& rewriter, SpecializationHandler& specializationHandler);
  void run(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
};
}

#endif  // TYPEGRIND_DELETEEXPRHANDLER_H
