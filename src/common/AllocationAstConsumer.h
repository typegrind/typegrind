
#ifndef TYPEGRIND_CLANG_ALLOCATIONASTCONSUMER_H
#define TYPEGRIND_CLANG_ALLOCATIONASTCONSUMER_H

#include <clang/Frontend/ASTUnit.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Rewrite/Core/Rewriter.h>

#include "common/handler/NewExprHandler.h"
#include "common/handler/OpNewExprHandler.h"
#include "common/handler/DeleteExprHandler.h"
#include "common/handler/MethodDeclHandler.h"
#include "AppConfig.h"

namespace typegrind {
    class AllocationASTConsumer : public clang::ASTConsumer {
    public:
        AllocationASTConsumer(clang::Rewriter*& rewriter, AppConfig const& appConfig);

        void HandleTranslationUnit(clang::ASTContext& context) override;

    protected:
        virtual void processRewriterData(clang::Rewriter*& rewriter);

    private:
        clang::ast_matchers::MatchFinder mMatcher;

        clang::Rewriter*& mRewriter;

        AppConfig const& mAppConfig;

        NewExprHandler mNewExprHandler;
        OpNewExprHandler mOpNewExprHandler;
        DeleteExprHandler mDeleteExprHandler;
        MethodDeclHandler mMethodDeclHandler;

    };
}

#endif //TYPEGRIND_CLANG_ALLOCATIONASTCONSUMER_H
