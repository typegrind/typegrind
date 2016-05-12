
#include "DeleteExprHandler.h"

namespace typegrind
{

  DeleteExprHandler::DeleteExprHandler(clang::Rewriter *&rewriter, SpecializationHandler& specializationHandler)
          : BaseExprHandler(rewriter, specializationHandler)
  {
  }

  void DeleteExprHandler::run(const clang::ast_matchers::MatchFinder::MatchResult &result)
  {
    // Adds: TYPEGRIND_LOG_DELETE(_ARRAY)(locationStr, typeStr, canonicalStr, (deleteExpr))
    // Or:   TYPEGRIND_EXPAND_DECLTYPE(locationStr, TYPEGRIND_LOG_DELETE(_ARRAY), positionEncoding, (deleteExpr))
    const clang::CXXDeleteExpr* deleteExpr = result.Nodes.getNodeAs<clang::CXXDeleteExpr>("deleteStmt");
    if (nullptr == deleteExpr)
    {
      llvm::errs() << "Couldn't convert MatcherResult to CXXDeleteExpr!\n";
      return;
    }

    clang::SourceLocation startLoc = deleteExpr->getArgument()->getLocStart();
    clang::SourceLocation endLoc = deleteExpr->getArgument()->getLocEnd();

    auto deletedType = deleteExpr->getArgument()->getType();
    auto ptr = deletedType.getTypePtr();
    if (!ptr->isDependentType() && !ptr->hasUnnamedOrLocalType())
    {
      handleSpecializedType(deletedType, startLoc.getRawEncoding(), CONVERT_TO_POINTEE);
    }

    if (!processingLocation(startLoc)) return;

    bool isDependentType = isReallyDependentType(deletedType, CONVERT_TO_POINTEE);

    std::string deleteMacroName(deleteExpr->isArrayForm() ? "TYPEGRIND_LOG_DELETE_ARRAY" : "TYPEGRIND_LOG_DELETE");

    MacroAdder deleteLoggerMacro(
            isDependentType ? "TYPEGRIND_EXPAND_DECLTYPE" : deleteMacroName,
            startLoc,
            endLoc,
            mRewriter
    );

    // 2nd and 3rd paramter: name of the type.

    if (isDependentType)
    {
      deleteLoggerMacro.addParameter(deleteMacroName);
      deleteLoggerMacro.addParameter(std::to_string(startLoc.getRawEncoding()));
    } 
    else
    {
      addTypeInformationParameters(deleteLoggerMacro, deletedType, startLoc.getRawEncoding(), CONVERT_TO_POINTEE);
    }

    // last parameter: the pointer expression
    // parenthesis around it, for multiple template parameters
    deleteLoggerMacro.startBuffer() << "(";
    deleteLoggerMacro.endBuffer() << ")";

    deleteLoggerMacro.commitAroundLocations();
  }

}
