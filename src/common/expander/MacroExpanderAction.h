
#ifndef TYPEGRIND_CLANG_MACROEXPANDERACTION_H
#define TYPEGRIND_CLANG_MACROEXPANDERACTION_H

#include <memory>

#include <llvm/Support/CommandLine.h>

#include <clang/Frontend/FrontendPluginRegistry.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Tooling/Tooling.h>

namespace typegrind
{

  class MacroExpanderAction : public clang::PluginASTAction, public clang::tooling::SourceFileCallbacks
  {
  public:
    typedef clang::PluginASTAction base_type;

    MacroExpanderAction & operator=(MacroExpanderAction const &) = delete;

    MacroExpanderAction(clang::tooling::ClangTool &targetTool);
    ~MacroExpanderAction();

    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) override; // plugin

    std::unique_ptr<clang::ASTConsumer> newASTConsumer(); // tool

    bool ParseArgs (const clang::CompilerInstance &CI, const std::vector< std::string > &arg) override;

    bool handleBeginSource(clang::CompilerInstance &CI, llvm::StringRef fileName) override;

  private:
    void expand();

    clang::tooling::ClangTool &targetTool;
    clang::CompilerInstance* mCI;
  };

}

#endif //TYPEGRIND_CLANG_MACROEXPANDERACTION_H
