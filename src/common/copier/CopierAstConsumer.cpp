
#include "CopierAstConsumer.h"

using namespace clang;

#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <fstream>

namespace typegrind
{
  CopierAstConsumer::CopierAstConsumer(clang::Rewriter*& rewriter, DirectoryMapper mapper, RegexSet const& regexes) : AllocationASTConsumer(rewriter, regexes), mapper(mapper)
  {
  }

  void CopierAstConsumer::processRewriterData(clang::Rewriter*& rewriter)
  {
    llvm::outs() << "processing!\n";
    for (auto it = rewriter->buffer_begin(), end = rewriter->buffer_end(); it != end; ++it) {
      const FileEntry *Entry = rewriter->getSourceMgr().getFileEntryForID(it->first);
      if(Entry && Entry->isValid())
      {
        boost::system::error_code fs_error;
        std::string originalFileName(Entry->getName());

        boost::filesystem::path canonicalPath = boost::filesystem::canonical(Entry->getName(), fs_error);
        if(fs_error)  
        {
          llvm::errs() << "Error while processing entry: " << Entry->getName() << "\n";
          return;
        }

        std::string outputFileName = canonicalPath.string();
        if (mapper.apply(outputFileName))
        {
            llvm::outs() << "Writing " << outputFileName << "\n";
            boost::filesystem::path targetPath(outputFileName);

            boost::filesystem::create_directories(targetPath.parent_path(), fs_error);
            if(fs_error)  
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
