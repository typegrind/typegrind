//
// Created by dutow on 10/7/15.
//

#include "OpNewExprHandler.h"

namespace typegrind
{
  OpNewExprHandler::OpNewExprHandler(clang::Rewriter*& rewriter, SpecializationHandler& specializationHandler)
          : BaseExprHandler(rewriter, specializationHandler)
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

    if (castExpr)
    {
      // Add info for every non dependent type
      auto ptr = castExpr->getTypeInfoAsWritten()->getTypeLoc().getType().getTypePtr();
      if (!ptr->isInstantiationDependentType() && !ptr->hasUnnamedOrLocalType())
      {
        handleSpecializedType(castExpr->getTypeInfoAsWritten()->getTypeLoc().getType(), startLoc.getRawEncoding(), CONVERT_TO_POINTEE);
      }
    }

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

      addTypeInformationParameters(newLoggerMacro, srcType, startLoc.getRawEncoding(), CONVERT_TO_POINTEE);

      auto allocatedType = castExpr->getType()->getPointeeType();
      // 4th parameter: sizeof type
      addSizeOfParameter(newLoggerMacro, allocatedType);
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

    // parenthesis around it, for multiple template parameters
    newLoggerMacro.startBuffer() << "(";
    newLoggerMacro.endBuffer() << ")";

    newLoggerMacro.commitAroundLocations();
  }
}
