
#ifndef TYPEGRIND_CLANG_ALLOCATIONDECORATORACTION_H
#define TYPEGRIND_CLANG_ALLOCATIONDECORATORACTION_H

#include <memory>

#include <llvm/Support/CommandLine.h>

#include <clang/Frontend/FrontendPluginRegistry.h>
#include <clang/Frontend/CompilerInstance.h>

#include "common/AllocationAstConsumer.h"
#include "RegexSet.h"

namespace typegrind {

    class AllocationDecoratorAction : public clang::PluginASTAction {
    public:
        AllocationDecoratorAction & operator=(AllocationDecoratorAction const &) = delete;

        AllocationDecoratorAction(RegexSet const& regexes);
        ~AllocationDecoratorAction();

        std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) override; // plugin

        std::unique_ptr<clang::ASTConsumer> newASTConsumer(); // tool

        bool ParseArgs (const clang::CompilerInstance &CI, const std::vector< std::string > &arg) override;

    protected:
        virtual std::unique_ptr<clang::ASTConsumer> internalCreateConsumer(clang::Rewriter*& rewriter) = 0;
    private:

        static bool isCpp(clang::CompilerInstance const& Compiler);

        clang::Rewriter* mRewriter;
      protected:
        RegexSet const& mRegexes;
    };

}

#endif //TYPEGRIND_CLANG_ALLOCATIONDECORATORACTION_H
