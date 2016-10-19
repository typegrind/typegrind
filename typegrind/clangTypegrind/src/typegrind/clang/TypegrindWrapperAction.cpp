
#include "typegrind/clang/TypegrindWrapperAction.h"

#include "clang/Lex/PreprocessorOptions.h"
#include "typegrind/clang/TypegrindAction.h"

namespace typegrind {

bool TypegrindWrapperAction::BeginInvocation(clang::CompilerInstance &CI) {
  std::vector<std::pair<std::string, std::string>> RewrittenFiles;
  bool err = false;
  {
    const clang::FrontendOptions &FEOpts = CI.getFrontendOpts();
    auto Typegrind = std::make_unique<TypegrindAction>();

    if (Typegrind->BeginSourceFile(CI, FEOpts.Inputs[0])) {
      Typegrind->Execute();
      Typegrind->EndSourceFile();

      // TODO: process rewrites

      CI.setSourceManager(nullptr);
      CI.setFileManager(nullptr);
    } else {
      err = true;
    }
  }
  if (err) return false;

  CI.getDiagnosticClient().clear();
  CI.getDiagnostics().Reset();

  clang::PreprocessorOptions &PPOpts = CI.getPreprocessorOpts();
  PPOpts.RemappedFiles.insert(PPOpts.RemappedFiles.end(), RewrittenFiles.begin(),
                              RewrittenFiles.end());
  PPOpts.RemappedFilesKeepOriginalName = false;

  return true;
}
}
