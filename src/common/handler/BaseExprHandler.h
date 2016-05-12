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

  class SpecializationHandler
  {
  public:
    enum PointeeConversion { KEEP_ORIGINAL_TYPE, CONVERT_TO_POINTEE };

    virtual void handleSpecializedType(clang::QualType const& typeInfo, unsigned specificUniqId, PointeeConversion convertToPointee=KEEP_ORIGINAL_TYPE) = 0;
  };

  class BaseExprHandler : public clang::ast_matchers::MatchFinder::MatchCallback, public SpecializationHandler
  {
  public:
    BaseExprHandler(clang::Rewriter*& rewriter, SpecializationHandler& specializationHandler);

    void handleSpecializedType(clang::QualType const& typeInfo, unsigned specificUniqId, PointeeConversion convertToPointee=KEEP_ORIGINAL_TYPE) override;

    clang::StringRef getID() const final;
  protected:
    clang::Rewriter*& mRewriter;

    /**
     * @return should we continue with processing, or is it already processed?
     */
    bool processingLocation(clang::SourceLocation loc);



    static bool isReallyDependentType(clang::QualType const& typeInfo, PointeeConversion convertToPointee=KEEP_ORIGINAL_TYPE);
    void addTypeInformationParameters(MacroAdder& macroAdder, clang::QualType const& typeInfo, unsigned specificUniqId, PointeeConversion convertToPointee=KEEP_ORIGINAL_TYPE);

    static void addSizeOfParameter(MacroAdder& macroAdder, clang::QualType const& typeInfo);

  private:
    std::unordered_set<unsigned> mAlreadyProcessed;
    SpecializationHandler& mSpecializationHandler;
  };
}

#endif //TYPEGRIND_BASEEXPRHANDLER_H
