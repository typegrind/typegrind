
#include "AllocationDecoratorAction.h"

namespace typegrind {

AllocationDecoratorAction::~AllocationDecoratorAction() {
  if (mRewriter) {
    delete mRewriter;
  }
}

AllocationDecoratorAction::AllocationDecoratorAction(typegrind::config::AppConfig const& appConfig)
    : mRewriter(nullptr), mAppConfig(appConfig) {}

bool AllocationDecoratorAction::ParseArgs(const clang::CompilerInstance& CI,
                                          const std::vector<std::string>& arg) {
  return true;
}

// ..:: Entry point for plugins ::..
std::unique_ptr<clang::ASTConsumer> AllocationDecoratorAction::newASTConsumer() {
  return internalCreateConsumer(mRewriter);
}

std::unique_ptr<clang::ASTConsumer> AllocationDecoratorAction::CreateASTConsumer(
    clang::CompilerInstance& CI, llvm::StringRef InFile) {
  return internalCreateConsumer(mRewriter);
}

bool AllocationDecoratorAction::handleBeginSource(clang::CompilerInstance& CI,
                                                  llvm::StringRef fileName) {
  // process all files
  return true;
}
}
