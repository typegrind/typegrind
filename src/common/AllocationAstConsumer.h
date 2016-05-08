
#ifndef TYPEGRIND_CLANG_ALLOCATIONASTCONSUMER_H
#define TYPEGRIND_CLANG_ALLOCATIONASTCONSUMER_H

#include <set>
#include <unordered_set>

#include <clang/Frontend/ASTUnit.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Rewrite/Core/Rewriter.h>

#include "common/handler/NewExprHandler.h"
#include "common/handler/OpNewExprHandler.h"
#include "common/handler/OpDeleteExprHandler.h"
#include "common/handler/DeleteExprHandler.h"
#include "common/handler/FunctionDeclHandler.h"
#include "common/handler/RecordDeclHandler.h"
#include "AppConfig.h"

namespace typegrind
{

  class AllocationASTConsumer : public clang::ASTConsumer, public SpecializationHandler
  {
  public:
    AllocationASTConsumer(clang::Rewriter*& rewriter, AppConfig const& appConfig);

    void HandleTranslationUnit(clang::ASTContext& context) override;

    void handleSpecializedType(clang::QualType const& typeInfo, unsigned specificUniqId, SpecializationHandler::PointeeConversion convertToPointee=SpecializationHandler::KEEP_ORIGINAL_TYPE) override;

  protected:
    virtual void processRewriterData(clang::Rewriter*& rewriter);
    void insertSpecializedInformation(clang::Rewriter& rewriter, clang::SourceLocation pos) const;

  private:
    clang::ast_matchers::MatchFinder mMatcher;

    clang::Rewriter*& mRewriter;
    AppConfig const& mAppConfig;

    NewExprHandler mNewExprHandler;
    OpNewExprHandler mOpNewExprHandler;
    OpDeleteExprHandler mOpDeleteExprHandler;
    DeleteExprHandler mDeleteExprHandler;
    FunctionDeclHandler mFunctionDeclHandler;
    RecordDeclHandler mRecordDeclHandler;

    std::unordered_set<std::string> mCanonicalSpecializations;
    std::set< std::pair<std::string, unsigned> > mSpecificSpecializations;

  };
}

#endif //TYPEGRIND_CLANG_ALLOCATIONASTCONSUMER_H
