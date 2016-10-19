
#pragma once

#include "common/AllocationAstConsumer.h"
#include "typegrind/config/DirectoryMapper.h"

namespace typegrind {

class CopierAstConsumer : public AllocationASTConsumer {
 public:
  CopierAstConsumer(clang::Rewriter*& rewriter, typegrind::config::DirectoryMapper mapper,
                    typegrind::config::AppConfig const& appConfig);

 protected:
  void processRewriterData(clang::Rewriter*& rewriter) override;

  typegrind::config::DirectoryMapper mapper;
};
}
