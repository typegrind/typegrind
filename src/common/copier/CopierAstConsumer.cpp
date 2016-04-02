
#include "CopierAstConsumer.h"

using namespace clang;

#include <fstream>
#include "FileUtils.h"

namespace typegrind
{
  CopierAstConsumer::CopierAstConsumer(clang::Rewriter*& rewriter, DirectoryMapper mapper, AppConfig const& appConfig) : AllocationASTConsumer(rewriter, appConfig), mapper(mapper)
  {
  }

  void CopierAstConsumer::processRewriterData(clang::Rewriter*& rewriter)
  {
    llvm::outs() << "processing!\n";
    for (auto it = rewriter->buffer_begin(), end = rewriter->buffer_end(); it != end; ++it)
    {
      const FileEntry *Entry = rewriter->getSourceMgr().getFileEntryForID(it->first);
      if(Entry && Entry->isValid())
      {
        std::string originalFileName(Entry->getName());

        std::string outputFileName;
        if(!typegrind::file_utils::canonize_path(originalFileName, outputFileName))
        {
          llvm::errs() << "Error while processing entry: " << Entry->getName() << "\n";
          return;
        }

        if (mapper.apply(outputFileName))
        {
          llvm::outs() << "Writing " << outputFileName << "\n";

          if(!typegrind::file_utils::ensure_directory_exists(outputFileName))
          {
            llvm::errs() << "Error while processing entry: " << Entry->getName() << "\n";
            return;
          }

          std::error_code ErrInfo;
          llvm::raw_fd_ostream ofs(outputFileName.c_str(), ErrInfo, llvm::sys::fs::F_None);
          it->second.write(ofs);
        }
        else
        {
          llvm::errs() << "File not found in mapping, ignoring: " << outputFileName << "\n";
        }
      }
    }
  }
}
