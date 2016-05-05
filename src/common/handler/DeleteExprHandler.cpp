
#include "DeleteExprHandler.h"

namespace typegrind
{

  DeleteExprHandler::DeleteExprHandler(clang::Rewriter *&rewriter)
          : BaseExprHandler(rewriter)
  {
  }

  void DeleteExprHandler::run(const clang::ast_matchers::MatchFinder::MatchResult &result)
  {
    const clang::CXXDeleteExpr* deleteExpr = result.Nodes.getNodeAs<clang::CXXDeleteExpr>("deleteStmt");
    if (nullptr == deleteExpr)
    {
      llvm::errs() << "Couldn't convert MatcherResult to CXXDeleteExpr!\n";
      return;
    }

    clang::SourceLocation startLoc = deleteExpr->getArgument()->getLocStart();
    clang::SourceLocation endLoc = deleteExpr->getArgument()->getLocEnd();
    if (!processingLocation(startLoc)) return;

    MacroAdder deleteLoggerMacro(
            deleteExpr->isArrayForm() ? "TYPEGRIND_LOG_DELETE_ARRAY" : "TYPEGRIND_LOG_DELETE",
            startLoc,
            endLoc,
            mRewriter
    );

    // 2nd and 3rd paramter: name of the type.
    auto deletedType = deleteExpr->getArgument()->getType();
    addTypeInformationParameters(deleteLoggerMacro, deletedType, CONVERT_TO_POINTEE);

    // last parameter: the pointer expression
    // parenthesis around it, for multiple template parameters
    deleteLoggerMacro.startBuffer() << "(";
    deleteLoggerMacro.endBuffer() << ")";

    deleteLoggerMacro.commitAroundLocations();
  }

}
