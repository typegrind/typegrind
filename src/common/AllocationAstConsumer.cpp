
#include "AllocationAstConsumer.h"

#include <clang/ASTMatchers/ASTMatchers.h>


using namespace clang;

namespace typegrind
{

  AllocationASTConsumer::AllocationASTConsumer(clang::Rewriter*& rewriter, AppConfig const& appConfig)
          : mRewriter(rewriter)
          , mAppConfig(appConfig)
          , mNewExprHandler(mRewriter, *this)
          , mOpNewExprHandler(mRewriter, *this)
          , mOpDeleteExprHandler(mRewriter, *this)
          , mDeleteExprHandler(mRewriter, *this)
          , mFunctionDeclHandler(mRewriter, *this, appConfig.getMethodMatcher())

  {
    using namespace ast_matchers;
    mMatcher.addMatcher(cxxNewExpr().bind("newStmt"), &mNewExprHandler);
    mMatcher.addMatcher(functionDecl().bind("decl"), &mFunctionDeclHandler);
    mMatcher.addMatcher(
            callExpr(callee(functionDecl(hasName("operator new")).bind("fun")), hasAncestor(cxxStaticCastExpr().bind("castExpr"))).bind("newStmt") ,
            &mOpNewExprHandler
    );
    mMatcher.addMatcher(
            callExpr(callee(functionDecl(hasName("operator new")).bind("fun")), hasAncestor(cxxReinterpretCastExpr().bind("castExpr"))).bind("newStmt"),
            &mOpNewExprHandler
    );
    mMatcher.addMatcher(
            callExpr(callee(functionDecl(hasName("operator new[]")).bind("fun")), hasAncestor(cxxReinterpretCastExpr().bind("castExpr"))).bind("newStmt"),
            &mOpNewExprHandler
    );

    mMatcher.addMatcher(cxxDeleteExpr().bind("deleteStmt"), &mDeleteExprHandler);
    mMatcher.addMatcher(
            callExpr(callee(functionDecl(hasName("operator delete")).bind("fun"))).bind("deleteStmt"),
            &mOpDeleteExprHandler
    );
    mMatcher.addMatcher(
            callExpr(callee(functionDecl(hasName("operator delete[]")).bind("fun"))).bind("deleteStmt"),
            &mOpDeleteExprHandler
    );
  }

  void AllocationASTConsumer::HandleTranslationUnit(ASTContext& context)
  {
    if (mRewriter == nullptr)
    {
      mRewriter = new clang::Rewriter(context.getSourceManager(), context.getLangOpts());
    }
    mMatcher.matchAST(context);

    if(mAppConfig.shouldPrependInclude())
    {
      auto mainFile = context.getSourceManager().getMainFileID();
      auto mainStartLocation = context.getSourceManager().getLocForStartOfFile(mainFile);
      std::string includeStmt;
      for(std::string const& inc: mAppConfig.getPrependInclude())
      {
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

  void AllocationASTConsumer::processRewriterData(clang::Rewriter*& rewriter)
  {
    // nop
  }


  void AllocationASTConsumer::handleSpecializedType(clang::QualType const& typeInfo, unsigned specificUniqId, SpecializationHandler::PointeeConversion convertToPointee/*=KEEP_ORIGINAL_TYPE*/)
  {
    clang::LangOptions options;
    clang::PrintingPolicy policy(options);
    policy.SuppressUnwrittenScope = true;
    policy.SuppressTagKeyword = true;

    clang::QualType ct = typeInfo;
    if (!ct.isCanonical()) ct = ct.getCanonicalType();
    if (convertToPointee == CONVERT_TO_POINTEE && !ct->getPointeeType().isNull())
    {
      ct = ct->getPointeeType();
    }

    mCanonicalSpecializations.insert(ct.getAsString(policy));
    mSpecificSpecializations.insert(std::pair<std::string, unsigned>(ct.getAsString(policy), specificUniqId));
  }

  void AllocationASTConsumer::insertSpecializedInformation(clang::Rewriter& rewriter, clang::SourceLocation pos) const
  {
    std::string str = "\n\n\n";
    for(std::string const& type: mCanonicalSpecializations)
    {
      str += "TYPEGRIND_CANONICAL_SPECIALIZATION((" + type + "));\n";
    }

    for(std::pair<std::string, unsigned> const& type: mSpecificSpecializations)
    {
      str += "TYPEGRIND_SPECIFIC_SPECIALIZATION((" + type.first + "), " + std::to_string(type.second) + ");\n";
    }

    rewriter.InsertText(pos, str);
  }
}
