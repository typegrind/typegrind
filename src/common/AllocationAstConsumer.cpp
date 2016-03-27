//===- PrintFunctionNames.cpp ---------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Example clang plugin which simply prints the names of all the top-level decls
// in the input file.
//
//===----------------------------------------------------------------------===//

#include "AllocationAstConsumer.h"

#include <clang/ASTMatchers/ASTMatchers.h>


using namespace clang;

namespace typegrind{

    AllocationASTConsumer::AllocationASTConsumer(clang::Rewriter*& rewriter, MethodMatcher const& matchers)
            : mRewriter(rewriter)
            , mMatchers(matchers)
            , mNewExprHandler(mRewriter)
            , mOpNewExprHandler(mRewriter)
            , mDeleteExprHandler(mRewriter)
            , mMethodDeclHandler(mRewriter, mMatchers)

    {
        using namespace ast_matchers;
        mMatcher.addMatcher(cxxNewExpr().bind("newStmt"), &mNewExprHandler);
        mMatcher.addMatcher(cxxMethodDecl().bind("decl"), &mMethodDeclHandler);
        mMatcher.addMatcher(
                callExpr(callee(functionDecl(hasName("operator new"))), hasAncestor(cxxStaticCastExpr().bind("castExpr"))).bind("newStmt") ,
                &mOpNewExprHandler
        );
        mMatcher.addMatcher(
                callExpr(callee(functionDecl(hasName("operator new"))), hasAncestor(cxxReinterpretCastExpr().bind("castExpr"))).bind("newStmt"),
                &mOpNewExprHandler
        );
        mMatcher.addMatcher(cxxDeleteExpr().bind("deleteStmt"), &mDeleteExprHandler);
    }

    void AllocationASTConsumer::HandleTranslationUnit(ASTContext& context)
    {
        if (mRewriter == nullptr) {
            mRewriter = new clang::Rewriter(context.getSourceManager(), context.getLangOpts());
        }
        mMatcher.matchAST(context);

        processRewriterData(mRewriter);
    }

    void AllocationASTConsumer::processRewriterData(clang::Rewriter*& rewriter)
    {
        // nop
    }
}
