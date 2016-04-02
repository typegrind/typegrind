//
// Created by dutow on 10/7/15.
//

#ifndef TPHP_CLANG_OPDELETEEXPRHANDLER_H
#define TPHP_CLANG_OPDELETEEXPRHANDLER_H

#include "BaseExprHandler.h"

namespace typegrind
{
  class OpDeleteExprHandler : public BaseExprHandler
  {
  public:

    OpDeleteExprHandler(clang::Rewriter*& rewriter);
    void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;
  };
}

#endif //TPHP_CLANG_OPDELETEEXPRHANDLER_H
