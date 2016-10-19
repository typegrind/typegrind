
#pragma once

#include "clang/Frontend/FrontendAction.h"

namespace typegrind {
/// \brief Executes typegrind and runs the wrapped action on it's output
class TypegrindWrapperAction : public clang::WrapperFrontendAction {
 public:
  TypegrindWrapperAction(std::unique_ptr<clang::FrontendAction> WrappedAction)
      : WrapperFrontendAction(std::move(WrappedAction)) {}

 protected:
  bool BeginInvocation(clang::CompilerInstance &CI) override;
};
}
