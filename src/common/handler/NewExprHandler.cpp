
#include "NewExprHandler.h"

namespace {
/**
 * True if placement new (we don't care about that)
 */
bool isPlacementNew(const clang::CXXNewExpr* expr) {
  // Regular new expressions aren't placement new
  if (expr->getNumPlacementArgs() == 0) return false;
  return true;
}
}

namespace typegrind {

NewExprHandler::NewExprHandler(clang::Rewriter*& rewriter,
                               SpecializationHandler& specializationHandler)
    : BaseExprHandler(rewriter, specializationHandler) {}

void NewExprHandler::run(const clang::ast_matchers::MatchFinder::MatchResult& result) {
  const clang::CXXNewExpr* newExpr = result.Nodes.getNodeAs<clang::CXXNewExpr>("newStmt");
  if (nullptr == newExpr) {
    llvm::errs() << "Internal error: couldn't convert newStmt to newExpr!\n";
    return;
  }

  if (isPlacementNew(newExpr)) return;

  clang::SourceLocation startLoc = newExpr->getStartLoc();
  clang::SourceLocation endLoc = newExpr->getEndLoc();

  // Add info for every non dependent type
  auto ptr = newExpr->getAllocatedType().getTypePtr();
  if (!ptr->isInstantiationDependentType() && !ptr->hasUnnamedOrLocalType()) {
    handleSpecializedType(newExpr->getAllocatedType(), startLoc.getRawEncoding());
  }

  if (!processingLocation(startLoc)) {
    return;
  }

  bool isDependentType = isReallyDependentType(newExpr->getAllocatedType());
  std::string newMacroName(newExpr->isArray() ? "TYPEGRIND_LOG_NEW_ARRAY" : "TYPEGRIND_LOG_NEW");

  MacroAdder newLoggerMacro(isDependentType ? "TYPEGRIND_EXPAND_DECLTYPE_SIZEOF" : newMacroName,
                            startLoc, endLoc, mRewriter);

  if (isDependentType) {
    newLoggerMacro.addParameter(newMacroName);
    newLoggerMacro.addParameter(std::to_string(startLoc.getRawEncoding()));
  } else {
    addTypeInformationParameters(newLoggerMacro, newExpr->getAllocatedType(),
                                 startLoc.getRawEncoding());
  }

  // last parameter: the constructor expression
  // parenthesis around it, for multiple template parameters
  newLoggerMacro.startBuffer() << "(";
  newLoggerMacro.endBuffer() << ")";

  if (!isDependentType) {
    addSizeOfParameter(newLoggerMacro, newExpr->getAllocatedType());
  }

  // if this is as array, there is an 5th parameter, the array size
  if (newExpr->isArray()) {
    // TODO: this assumes that the size expression is const, which isn't always
    // true! => extract expression to a variable ?
    newLoggerMacro.endBuffer() << ", ";
    newExpr->getArraySize()->printPretty(
        newLoggerMacro.endBuffer(), nullptr,
        clang::PrintingPolicy(result.Context->getPrintingPolicy()));
  }

  newLoggerMacro.commitAroundLocations();
}
}
