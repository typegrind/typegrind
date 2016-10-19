//
// Created by dutow on 10/7/15.
//

#ifndef TPHP_CLANG_OPNEWEXPRHANDLER_H
#define TPHP_CLANG_OPNEWEXPRHANDLER_H

#include "BaseExprHandler.h"

namespace typegrind {
class OpNewExprHandler : public BaseExprHandler {
 public:
  OpNewExprHandler(clang::Rewriter*& rewriter, SpecializationHandler& specializationHandler);
  void run(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
};
}

#endif  // TPHP_CLANG_OPNEWEXPRHANDLER_H
