
#ifndef TYPEGRIND_CLANG_ALLOCATIONDECORATORACTION_H
#define TYPEGRIND_CLANG_ALLOCATIONDECORATORACTION_H

#include <memory>

#include <llvm/Support/CommandLine.h>

#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Tooling/Tooling.h>

#include "typegrind/config/AppConfig.h"

namespace typegrind {

class TypegrindAction : public clang::SyntaxOnlyAction {
 public:
  TypegrindAction& operator=(TypegrindAction const&) = delete;

  TypegrindAction();
  ~TypegrindAction();

  clang::Rewriter const& GetRewriter() const { return *mRewriter; }

 protected:
  std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& CI,
                                                        llvm::StringRef InFile) override;
  clang::Rewriter* mRewriter;

 private:
  typedef clang::SyntaxOnlyAction base_type;
};
}

#endif  // TYPEGRIND_CLANG_ALLOCATIONDECORATORACTION_H
