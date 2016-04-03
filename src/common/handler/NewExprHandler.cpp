
#include "NewExprHandler.h"

namespace
{
  /**
   * True if placement new (we don't care about that)
   */
  bool isPlacementNew(const clang::CXXNewExpr* expr)
  {
    // Regular new expressions aren't placement new
    if (expr->getNumPlacementArgs() == 0)
      return false;
    return true;
  }

}

namespace typegrind
{

  NewExprHandler::NewExprHandler(clang::Rewriter*& rewriter)
          : BaseExprHandler(rewriter)
  {
  }

  void NewExprHandler::run(const clang::ast_matchers::MatchFinder::MatchResult &result)
  {
    const clang::CXXNewExpr* newExpr = result.Nodes.getNodeAs<clang::CXXNewExpr>("newStmt");
    if (nullptr == newExpr)
    {
      llvm::errs() << "Internal error: couldn't convert newStmt to newExpr!\n";
      return;
    }

    clang::SourceLocation startLoc = newExpr->getStartLoc();
    clang::SourceLocation endLoc = newExpr->getEndLoc();
    if (!processingLocation(startLoc)) return;

    if (isPlacementNew(newExpr)) return;

    MacroAdder newLoggerMacro(
            newExpr->isArray() ? "TYPEGRIND_LOG_NEW_ARRAY" : "TYPEGRIND_LOG_NEW",
            startLoc,
            endLoc,
            mRewriter
    );

    // Skipping substituted template types
    auto srcType = newExpr->getAllocatedTypeSourceInfo()->getTypeLoc();
    auto ptr = srcType.getType().getTypePtr()->getAs<clang::SubstTemplateTypeParmType>();
    if (ptr)
    {
      // This is an automatic template specialization.
      // TODO: better condition? TEST IT!
      return;
    }

    // 2nd and 3rd paramter: name of the type.
    addTypeInformationParameters(newLoggerMacro, newExpr->getAllocatedType());

    // 4th parameter: sizeof type
    addSizeOfParameter(newLoggerMacro, newExpr->getAllocatedType());

    // if this is as array, there is an 5th parameter, the array size
    if (newExpr->isArray())
    {
      // TODO: this assumes that the size expression is const, which isn't always true! => extract expression to a variable ?
      newExpr->getArraySize()->printPretty(newLoggerMacro.startBuffer(), nullptr, clang::PrintingPolicy(result.Context->getPrintingPolicy()));
      newLoggerMacro.startBuffer() << ", ";
    }

    // last parameter: the constructor expression

    newLoggerMacro.commitAroundLocations();
  }
}
