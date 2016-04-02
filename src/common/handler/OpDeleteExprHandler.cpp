//
// Created by dutow on 10/7/15.
//

#include "OpDeleteExprHandler.h"

namespace typegrind
{
  OpDeleteExprHandler::OpDeleteExprHandler(clang::Rewriter*& rewriter)
          : BaseExprHandler(rewriter)
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
      if (!processingLocation(startLoc)) return;

      MacroAdder deleteLoggerMacro(
              funDecl->getNameInfo().getName().getAsString()=="operator delete[]" ? "TYPEGRIND_LOG_OP_DELETE_ARRAY" : "TYPEGRIND_LOG_OP_DELETE",
              startLoc,
              endLoc,
              mRewriter
      );

      const clang::Expr* pointerExpr = deleteExpr->getArg(0)->IgnoreCasts();

      // 2nd and 3rd paramter: name of the type.
      auto deletedType = pointerExpr->getType();
      addTypeInformationParameters(deleteLoggerMacro, deletedType);

      // last parameter: the pointer expression

      deleteLoggerMacro.commit();
  }
}
