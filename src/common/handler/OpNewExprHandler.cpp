//
// Created by dutow on 10/7/15.
//

#include "OpNewExprHandler.h"

namespace typegrind
{
  OpNewExprHandler::OpNewExprHandler(clang::Rewriter*& rewriter)
          : BaseExprHandler(rewriter)
  {
  }

  void OpNewExprHandler::run(const clang::ast_matchers::MatchFinder::MatchResult &result)
  {
    const clang::ExplicitCastExpr* castExpr = result.Nodes.getNodeAs<clang::ExplicitCastExpr>("castExpr");
    const clang::CallExpr* newExpr = result.Nodes.getNodeAs<clang::CallExpr>("newStmt");
    const clang::FunctionDecl* funDecl = result.Nodes.getNodeAs<clang::FunctionDecl>("fun");

    if (nullptr == newExpr)
    {
      llvm::errs() << "Couldn't convert MatcherResult to CallExpr!\n";
      return;
    }


    clang::SourceLocation startLoc = newExpr->getLocStart();
    clang::SourceLocation endLoc = newExpr->getLocEnd();

    if (!processingLocation(startLoc)) return;


    MacroAdder newLoggerMacro(
            funDecl->getNameInfo().getName().getAsString()=="operator new[]" ? "TYPEGRIND_LOG_OP_NEW_ARRAY" : "TYPEGRIND_LOG_OP_NEW",
            startLoc,
            endLoc,
            mRewriter
    );


    if (castExpr)
    {
      auto srcType = castExpr->getTypeInfoAsWritten()->getTypeLoc().getType();
      auto ptr = srcType.getTypePtr()->getAs<clang::SubstTemplateTypeParmType>();
      // Skipping substituted template types
      if (ptr)
      {
        // This is an automatic template specialization.
        // TODO: better condition? TEST IT!
        return;
      }

      addTypeInformationParameters(newLoggerMacro, srcType);

      auto allocatedType = castExpr->getType()->getPointeeType();
      // 4th parameter: sizeof type
      newLoggerMacro.addParameter("sizeof(" + allocatedType.getAsString() + ")");
    }
    else
    {
      // no idea about the type
      newLoggerMacro.addParameterAsString("void");
      newLoggerMacro.addParameterAsString("void");
      newLoggerMacro.addParameter("0");
    }

    // size, alias first parameter
    newExpr->getArg(0)->printPretty(newLoggerMacro.startBuffer(), nullptr, clang::PrintingPolicy(result.Context->getPrintingPolicy()));
    newLoggerMacro.startBuffer() << ", ";

    newLoggerMacro.commit();
  }
}
