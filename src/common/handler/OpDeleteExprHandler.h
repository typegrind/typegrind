//
// Created by dutow on 10/7/15.
//

#ifndef TPHP_CLANG_OPDELETEEXPRHANDLER_H
#define TPHP_CLANG_OPDELETEEXPRHANDLER_H

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <unordered_set>

namespace typegrind {
  class OpDeleteExprHandler : public clang::ast_matchers::MatchFinder::MatchCallback {
  public:

    OpDeleteExprHandler(clang::Rewriter*& rewriter);
    void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;

    clang::StringRef getID() const override;

  private:
    clang::Rewriter*& mRewriter;
    std::unordered_set<unsigned> mAlreadyEncoded;
  };
}

#endif //TPHP_CLANG_OPDELETEEXPRHANDLER_H
