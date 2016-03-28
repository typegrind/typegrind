
#include "AllocationAstConsumer.h"

#include <clang/ASTMatchers/ASTMatchers.h>


using namespace clang;

namespace typegrind{

    AllocationASTConsumer::AllocationASTConsumer(clang::Rewriter*& rewriter, AppConfig const& appConfig)
            : mRewriter(rewriter)
            , mAppConfig(appConfig)
            , mNewExprHandler(mRewriter)
            , mOpNewExprHandler(mRewriter)
            , mOpDeleteExprHandler(mRewriter)
            , mDeleteExprHandler(mRewriter)
            , mMethodDeclHandler(mRewriter, appConfig.getMethodMatcher())

    {
        using namespace ast_matchers;
        mMatcher.addMatcher(cxxNewExpr().bind("newStmt"), &mNewExprHandler);
        mMatcher.addMatcher(cxxMethodDecl().bind("decl"), &mMethodDeclHandler);
        mMatcher.addMatcher(
                callExpr(callee(functionDecl(hasName("operator new"))), hasAncestor(cxxStaticCastExpr().bind("castExpr"))).bind("newStmt") ,
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
        if (mRewriter == nullptr) {
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

        processRewriterData(mRewriter);
    }

    void AllocationASTConsumer::processRewriterData(clang::Rewriter*& rewriter)
    {
        // nop
    }
}
