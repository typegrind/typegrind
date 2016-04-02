//
// Created by vagrant on 4/2/16.
//

#ifndef TYPEGRIND_BASEEXPRHANDLER_H
#define TYPEGRIND_BASEEXPRHANDLER_H

#include <unordered_set>

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Rewrite/Core/Rewriter.h>

#include "MacroAdder.h"

namespace typegrind
{
  class BaseExprHandler : public clang::ast_matchers::MatchFinder::MatchCallback
  {
  public:
    BaseExprHandler(clang::Rewriter*& rewriter);

    clang::StringRef getID() const final;
  protected:
    clang::Rewriter*& mRewriter;

    /**
     * @return should we continue with processing, or is it already processed?
     */
    bool processingLocation(clang::SourceLocation loc);

    static void addTypeInformationParameters(MacroAdder& macroAdder, clang::QualType const& typeInfo);

  private:
    std::unordered_set<unsigned> mAlreadyProcessed;
  };
}

#endif //TYPEGRIND_BASEEXPRHANDLER_H
