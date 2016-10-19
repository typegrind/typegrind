
#ifndef TYPEGRIND_CLANG_ALLOCATIONASTCONSUMER_H
#define TYPEGRIND_CLANG_ALLOCATIONASTCONSUMER_H

#include <set>
#include <unordered_set>

#include <clang/AST/ASTConsumer.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Frontend/ASTUnit.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Rewrite/Core/Rewriter.h>

#include "handler/DeleteExprHandler.h"
#include "handler/FunctionDeclHandler.h"
#include "handler/NewExprHandler.h"
#include "handler/OpDeleteExprHandler.h"
#include "handler/OpNewExprHandler.h"
#include "handler/RecordDeclHandler.h"
#include "typegrind/config/AppConfig.h"

namespace typegrind {
struct SpecializedInfo {
  std::string canonicalName;
  std::string specificName;
  unsigned uniqueId;
};

struct SpecializedInfoHasher {
  int operator()(SpecializedInfo const& info) const noexcept {
    using namespace std;
    hash<std::string> hasher;
    return hasher(info.canonicalName) + hasher(info.specificName) + info.uniqueId;
  }
};

bool operator==(SpecializedInfo const& a, SpecializedInfo const& b);

class AllocationASTConsumer : public clang::ASTConsumer, public SpecializationHandler {
 public:
  AllocationASTConsumer(clang::Rewriter*& rewriter, typegrind::config::AppConfig const& appConfig);

  void HandleTranslationUnit(clang::ASTContext& context) override;

  void handleSpecializedType(clang::QualType const& typeInfo, unsigned specificUniqId,
                             SpecializationHandler::PointeeConversion convertToPointee =
                                 SpecializationHandler::KEEP_ORIGINAL_TYPE) override;

 protected:
  virtual void processRewriterData(clang::Rewriter*& rewriter);
  void insertSpecializedInformation(clang::Rewriter& rewriter, clang::SourceLocation pos) const;

 private:
  clang::ast_matchers::MatchFinder mMatcher;

  clang::Rewriter*& mRewriter;
  typegrind::config::AppConfig const& mAppConfig;

  NewExprHandler mNewExprHandler;
  OpNewExprHandler mOpNewExprHandler;
  OpDeleteExprHandler mOpDeleteExprHandler;
  DeleteExprHandler mDeleteExprHandler;
  FunctionDeclHandler mFunctionDeclHandler;
  RecordDeclHandler mRecordDeclHandler;

  std::unordered_set<std::string> mCanonicalSpecializations;

  std::unordered_set<SpecializedInfo, SpecializedInfoHasher> mSpecificSpecializations;
};
}

#endif  // TYPEGRIND_CLANG_ALLOCATIONASTCONSUMER_H
