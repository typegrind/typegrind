
#include "AllocationDecoratorAction.h"

namespace typegrind {

  AllocationDecoratorAction::~AllocationDecoratorAction()
  {
    if(mRewriter)
    {
      delete mRewriter;
    }
  }


  AllocationDecoratorAction::AllocationDecoratorAction(AppConfig const& appConfig) : mRewriter(nullptr), mAppConfig(appConfig) { }

  bool AllocationDecoratorAction::isCpp(clang::CompilerInstance const& Compiler) {
    clang::LangOptions const Opts = Compiler.getLangOpts();
    return Opts.CPlusPlus;
  }

  bool AllocationDecoratorAction::ParseArgs (const clang::CompilerInstance &CI, const std::vector< std::string > &arg)
  {
    return true;
  }

  // ..:: Entry point for plugins ::..
  std::unique_ptr<clang::ASTConsumer> AllocationDecoratorAction::newASTConsumer() {
    return internalCreateConsumer(mRewriter);
  }

  std::unique_ptr<clang::ASTConsumer> AllocationDecoratorAction::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) {
    return internalCreateConsumer(mRewriter);
  }
}
