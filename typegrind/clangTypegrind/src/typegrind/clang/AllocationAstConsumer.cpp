
#include "AllocationAstConsumer.h"

#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Basic/LangOptions.h>

using namespace clang;

namespace {
bool isCppFile(clang::LangOptions const& Opts) { return Opts.CPlusPlus; }
}

namespace typegrind {
bool operator==(SpecializedInfo const& a, SpecializedInfo const& b) {
  return a.canonicalName == b.canonicalName && a.specificName == b.specificName &&
         a.uniqueId == b.uniqueId;
}

AllocationASTConsumer::AllocationASTConsumer(clang::Rewriter*& rewriter,
                                             typegrind::config::AppConfig const& appConfig)
    : mRewriter(rewriter)
    , mAppConfig(appConfig)
    , mNewExprHandler(mRewriter, *this)
    , mOpNewExprHandler(mRewriter, *this)
    , mOpDeleteExprHandler(mRewriter, *this)
    , mDeleteExprHandler(mRewriter, *this)
    , mFunctionDeclHandler(mRewriter, *this, mAppConfig.getMethodMatcher())
    , mRecordDeclHandler(mRewriter, *this)

{
  using namespace ast_matchers;
  mMatcher.addMatcher(cxxNewExpr().bind("newStmt"), &mNewExprHandler);
  mMatcher.addMatcher(functionDecl().bind("decl"), &mFunctionDeclHandler);
  mMatcher.addMatcher(cxxRecordDecl().bind("decl"), &mRecordDeclHandler);
  mMatcher.addMatcher(callExpr(callee(functionDecl(hasName("operator new")).bind("fun")),
                               hasAncestor(cxxStaticCastExpr().bind("castExpr")))
                          .bind("newStmt"),
                      &mOpNewExprHandler);
  mMatcher.addMatcher(callExpr(callee(functionDecl(hasName("operator new")).bind("fun")),
                               hasAncestor(cxxReinterpretCastExpr().bind("castExpr")))
                          .bind("newStmt"),
                      &mOpNewExprHandler);
  mMatcher.addMatcher(callExpr(callee(functionDecl(hasName("operator new")).bind("fun")),
                               hasAncestor(cStyleCastExpr().bind("castExpr")))
                          .bind("newStmt"),
                      &mOpNewExprHandler);
  mMatcher.addMatcher(
      callExpr(callee(functionDecl(hasName("operator new")).bind("fun"))).bind("newStmt"),
      &mOpNewExprHandler);

  mMatcher.addMatcher(callExpr(callee(functionDecl(hasName("operator new[]")).bind("fun")),
                               hasAncestor(cxxReinterpretCastExpr().bind("castExpr")))
                          .bind("newStmt"),
                      &mOpNewExprHandler);
  mMatcher.addMatcher(callExpr(callee(functionDecl(hasName("operator new[]")).bind("fun")),
                               hasAncestor(cxxStaticCastExpr().bind("castExpr")))
                          .bind("newStmt"),
                      &mOpNewExprHandler);
  mMatcher.addMatcher(callExpr(callee(functionDecl(hasName("operator new[]")).bind("fun")),
                               hasAncestor(cStyleCastExpr().bind("castExpr")))
                          .bind("newStmt"),
                      &mOpNewExprHandler);
  mMatcher.addMatcher(
      callExpr(callee(functionDecl(hasName("operator new[]")).bind("fun"))).bind("newStmt"),
      &mOpNewExprHandler);

  mMatcher.addMatcher(cxxDeleteExpr().bind("deleteStmt"), &mDeleteExprHandler);
  mMatcher.addMatcher(
      callExpr(callee(functionDecl(hasName("operator delete")).bind("fun"))).bind("deleteStmt"),
      &mOpDeleteExprHandler);
  mMatcher.addMatcher(
      callExpr(callee(functionDecl(hasName("operator delete[]")).bind("fun"))).bind("deleteStmt"),
      &mOpDeleteExprHandler);
}

void AllocationASTConsumer::HandleTranslationUnit(ASTContext& context) {
  if (mRewriter == nullptr) {
    mRewriter = new clang::Rewriter(context.getSourceManager(), context.getLangOpts());
  }

  bool isCpp = isCppFile(context.getLangOpts());

  if (isCpp) mMatcher.matchAST(context);

  if (mAppConfig.shouldPrependInclude()) {
    auto mainFile = context.getSourceManager().getMainFileID();
    auto mainStartLocation = context.getSourceManager().getLocForStartOfFile(mainFile);
    std::string includeStmt;
    for (std::string const& inc :
         mAppConfig.getPrependInclude(isCpp ? FileType::CPP : FileType::C)) {
      includeStmt += "#include <" + inc + ">\n";
    }
    mRewriter->InsertText(mainStartLocation, includeStmt);
  }

  {
    auto mainFile = context.getSourceManager().getMainFileID();
    auto mainEndLocation = context.getSourceManager().getLocForEndOfFile(mainFile);
    insertSpecializedInformation(*mRewriter, mainEndLocation);
  }

  processRewriterData(mRewriter);
}

void AllocationASTConsumer::processRewriterData(clang::Rewriter*& rewriter) {
  // nop
}

void AllocationASTConsumer::handleSpecializedType(
    clang::QualType const& typeInfo, unsigned specificUniqId,
    SpecializationHandler::PointeeConversion convertToPointee /*=KEEP_ORIGINAL_TYPE*/) {
  clang::LangOptions options;
  clang::PrintingPolicy policy(options);
  policy.SuppressUnwrittenScope = true;
  policy.SuppressTagKeyword = true;

  clang::QualType st = typeInfo;
  if (convertToPointee == CONVERT_TO_POINTEE && !st->getPointeeType().isNull()) {
    st = st->getPointeeType();
  }

  clang::QualType ct = typeInfo;
  if (!ct.isCanonical()) ct = ct.getCanonicalType();
  if (convertToPointee == CONVERT_TO_POINTEE && !ct->getPointeeType().isNull()) {
    ct = ct->getPointeeType();
  }

  mCanonicalSpecializations.insert(ct.getAsString(policy));
  mSpecificSpecializations.insert(
      SpecializedInfo{ct.getAsString(policy), st.getAsString(policy), specificUniqId});
}

void AllocationASTConsumer::insertSpecializedInformation(clang::Rewriter& rewriter,
                                                         clang::SourceLocation pos) const {
  std::string str = "\n\n\n";
  for (std::string const& type : mCanonicalSpecializations) {
    str += "TYPEGRIND_CANONICAL_SPECIALIZATION(TYPEGRIND_TYPE(" + type + "));\n";
  }

  for (auto const& type : mSpecificSpecializations) {
    str += "TYPEGRIND_SPECIFIC_SPECIALIZATION(TYPEGRIND_TYPE(" + type.canonicalName +
           "), TYPEGRIND_TYPE(" + type.specificName + "), " + std::to_string(type.uniqueId) +
           ");\n";
  }

  rewriter.InsertText(pos, str);
}
}
