
#pragma once

#include "common/AllocationAstConsumer.h"
#include "DirectoryMapper.h"

namespace typegrind
{

  class CopierAstConsumer : public AllocationASTConsumer
  {
  public:
    CopierAstConsumer(clang::Rewriter*& rewriter, DirectoryMapper mapper, RegexSet const& regexes);
  protected:
    void processRewriterData(clang::Rewriter*& rewriter) override;

    DirectoryMapper mapper;
  };
}
