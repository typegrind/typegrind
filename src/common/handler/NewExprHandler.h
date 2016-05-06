//
// Created by dutow on 10/4/15.
//

#ifndef TYPEGRIND_CLANG_NEWEXPRHANDLER_H
#define TYPEGRIND_CLANG_NEWEXPRHANDLER_H

#include "BaseExprHandler.h"

namespace typegrind
{
  class NewExprHandler : public BaseExprHandler
  {
  public:

    NewExprHandler(clang::Rewriter*& rewriter, SpecializationHandler& specializationHandler);
    void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;
  };
}

#endif //TYPEGRIND_CLANG_NEWEXPRHANDLER_H
