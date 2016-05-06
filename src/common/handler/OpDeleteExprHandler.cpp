//
// Created by dutow on 10/7/15.
//

#include "OpDeleteExprHandler.h"

namespace typegrind
{
  OpDeleteExprHandler::OpDeleteExprHandler(clang::Rewriter*& rewriter, SpecializationHandler& specializationHandler)
          : BaseExprHandler(rewriter, specializationHandler)
  {
  }

  void OpDeleteExprHandler::run(const clang::ast_matchers::MatchFinder::MatchResult &result)
  {
      const clang::CallExpr* deleteExpr = result.Nodes.getNodeAs<clang::CallExpr>("deleteStmt");
      const clang::FunctionDecl* funDecl = result.Nodes.getNodeAs<clang::FunctionDecl>("fun");
      if (nullptr == deleteExpr)
      {
          llvm::errs() << "Couldn't convert MatcherResult to CallExpr!\n";
          return;
      }

      clang::SourceLocation startLoc = deleteExpr->getArg(0)->getLocStart();
      clang::SourceLocation endLoc = deleteExpr->getArg(0)->getLocEnd();

      const clang::Expr* pointerExpr = deleteExpr->getArg(0)->IgnoreCasts();


      auto deletedType = pointerExpr->getType();
      auto ptr = deletedType.getTypePtr();
      if (!ptr->isInstantiationDependentType() && !ptr->hasUnnamedOrLocalType())
      {
          handleSpecializedType(pointerExpr->getType(), startLoc.getRawEncoding(), CONVERT_TO_POINTEE);
      }

      if (!processingLocation(startLoc)) return;

      MacroAdder deleteLoggerMacro(
              funDecl->getNameInfo().getName().getAsString()=="operator delete[]" ? "TYPEGRIND_LOG_OP_DELETE_ARRAY" : "TYPEGRIND_LOG_OP_DELETE",
              startLoc,
              endLoc,
              mRewriter
      );

      // 2nd and 3rd paramter: name of the type.
      addTypeInformationParameters(deleteLoggerMacro, deletedType, startLoc.getRawEncoding(), CONVERT_TO_POINTEE);

      // last parameter: the pointer expression
      // parenthesis around it, for multiple template parameters
      deleteLoggerMacro.startBuffer() << "(";
      deleteLoggerMacro.endBuffer() << ")";

    deleteLoggerMacro.commitAroundLocations();
  }
}
