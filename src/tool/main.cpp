
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include "clang/Frontend/TextDiagnosticPrinter.h"

#include "common/AllocationDecoratorAction.h"
#include "common/copier/CopierAstConsumer.h"
#include "AppConfig.h"

using namespace clang;
using namespace clang::tooling;
using namespace llvm;


static llvm::cl::OptionCategory typegrindCategory("typegrind");
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
static cl::extrahelp MoreHelp(R"(
Typegrind preprocesses C++ source files and instruments them with type preserving heap logging instructions.

TODO
)");

class MyAction : public typegrind::AllocationDecoratorAction
{
public:
  MyAction(typegrind::DirectoryMapper mapper, AppConfig const& appConfig) : typegrind::AllocationDecoratorAction(appConfig), mapper(mapper)
  {
  }

protected:
  std::unique_ptr<clang::ASTConsumer> internalCreateConsumer(clang::Rewriter*& rewriter) override
  {
    return make_unique<typegrind::CopierAstConsumer>(rewriter, mapper, mAppConfig);
  }
private:
  typegrind::DirectoryMapper mapper;
};

int main(int argc, const char **argv) {
  CommonOptionsParser OptionsParser(argc, argv, typegrindCategory);

  AppConfig appConfig("typegrind.json");

  if(!appConfig.isValid())
  {
    llvm::errs() << appConfig.getErrorMessage() << "\n";
    return -1;
  }

  ClangTool Tool(
          OptionsParser.getCompilations(),
          OptionsParser.getSourcePathList()
  );

  MyAction action(appConfig.getDirectoryMapping(), appConfig);
  auto factory = newFrontendActionFactory(&action);
  Tool.run(factory.get());
}
