
#include "AllocationDecoratorAction.h"

namespace
{
  bool isCppFile(clang::CompilerInstance const& compiler)
  {
    clang::LangOptions const Opts = compiler.getLangOpts();
    return Opts.CPlusPlus;
  }
}

namespace typegrind
{

  AllocationDecoratorAction::~AllocationDecoratorAction()
  {
    if(mRewriter)
    {
      delete mRewriter;
    }
  }


  AllocationDecoratorAction::AllocationDecoratorAction(AppConfig const& appConfig) : mRewriter(nullptr), mAppConfig(appConfig) { }

  bool AllocationDecoratorAction::ParseArgs (const clang::CompilerInstance &CI, const std::vector< std::string > &arg)
  {
    return isCppFile(CI);
  }

  // ..:: Entry point for plugins ::..
  std::unique_ptr<clang::ASTConsumer> AllocationDecoratorAction::newASTConsumer()
  {
    return internalCreateConsumer(mRewriter);
  }

  std::unique_ptr<clang::ASTConsumer> AllocationDecoratorAction::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile)
  {
    if (isCppFile(CI)) return nullptr;

    return internalCreateConsumer(mRewriter);
  }

  bool AllocationDecoratorAction::handleBeginSource(clang::CompilerInstance &CI, llvm::StringRef fileName)
  {
    if(isCppFile(CI))
      llvm::outs() << "cpp\n";

    return isCppFile(CI);
  }
}
