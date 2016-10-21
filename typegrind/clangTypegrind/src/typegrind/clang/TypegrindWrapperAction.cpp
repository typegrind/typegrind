
#include "typegrind/clang/TypegrindWrapperAction.h"

#include "clang/Lex/PreprocessorOptions.h"
#include "typegrind/clang/TypegrindAction.h"

#include <unordered_map>

namespace typegrind {

bool TypegrindWrapperAction::BeginInvocation(clang::CompilerInstance &CI) {
  std::unordered_map<std::string, std::string> RewrittenFiles;
  bool err = false;
  {
    const clang::FrontendOptions &FEOpts = CI.getFrontendOpts();
    auto Typegrind = std::make_unique<TypegrindAction>();

    if (Typegrind->BeginSourceFile(CI, FEOpts.Inputs[0])) {
      Typegrind->Execute();
      Typegrind->EndSourceFile();

      // TODO: process rewrites

      auto &rewriter = Typegrind->GetRewriter();
      for (auto it = rewriter.buffer_begin(), end = rewriter.buffer_end(); it != end; ++it) {
        const clang::FileEntry *Entry = rewriter.getSourceMgr().getFileEntryForID(it->first);
        if (Entry && Entry->isValid()) {
          llvm::raw_string_ostream ofs(RewrittenFiles[Entry->tryGetRealPathName()]);
          it->second.write(ofs);
        }
      }

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
  for (auto pair : RewrittenFiles) {
    PPOpts.addRemappedFile(pair.first,
                           llvm::MemoryBuffer::getMemBufferCopy(pair.second, pair.first).release());
  }

  return true;
}
}
