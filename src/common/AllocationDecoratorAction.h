
#ifndef TYPEGRIND_CLANG_ALLOCATIONDECORATORACTION_H
#define TYPEGRIND_CLANG_ALLOCATIONDECORATORACTION_H

#include <memory>

#include <llvm/Support/CommandLine.h>

#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendPluginRegistry.h>
#include <clang/Tooling/Tooling.h>

#include "common/AllocationAstConsumer.h"
#include "typegrind/config/AppConfig.h"

namespace typegrind {

class AllocationDecoratorAction : public clang::PluginASTAction,
                                  public clang::tooling::SourceFileCallbacks {
 public:
  typedef clang::PluginASTAction base_type;

  AllocationDecoratorAction& operator=(AllocationDecoratorAction const&) = delete;

  AllocationDecoratorAction(typegrind::config::AppConfig const& appConfig);
  ~AllocationDecoratorAction();

  std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& CI,
                                                        llvm::StringRef InFile) override;  // plugin

  std::unique_ptr<clang::ASTConsumer> newASTConsumer();  // tool

  bool ParseArgs(const clang::CompilerInstance& CI, const std::vector<std::string>& arg) override;

  bool handleBeginSource(clang::CompilerInstance& CI, llvm::StringRef fileName) override;

 protected:
  virtual std::unique_ptr<clang::ASTConsumer> internalCreateConsumer(
      clang::Rewriter*& rewriter) = 0;

  clang::Rewriter* mRewriter;

 protected:
  typegrind::config::AppConfig const& mAppConfig;
};
}

#endif  // TYPEGRIND_CLANG_ALLOCATIONDECORATORACTION_H
