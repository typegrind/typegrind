
#include "typegrind/clang/TypegrindAction.h"
#include "typegrind/config/AppConfig.h"

#include "AllocationAstConsumer.h"

namespace typegrind {

TypegrindAction::~TypegrindAction() {
  if (mRewriter) {
    delete mRewriter;
  }
}

TypegrindAction::TypegrindAction() : mRewriter(nullptr) {}

std::unique_ptr<clang::ASTConsumer> TypegrindAction::CreateASTConsumer(clang::CompilerInstance& CI,
                                                                       llvm::StringRef InFile) {
  return std::make_unique<AllocationASTConsumer>(mRewriter, typegrind::config::AppConfig());
}
}
