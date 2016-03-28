//
// Created by dutow on 10/7/15.
//

#include "OpDeleteExprHandler.h"

namespace typegrind {
    OpDeleteExprHandler::OpDeleteExprHandler(clang::Rewriter*& rewriter)
            : mRewriter(rewriter)
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

        std::string macroStart = "TYPEGRIND_LOG_OP_DELETE";
        if (funDecl->getNameInfo().getName().getAsString()=="operator delete[]") {
          macroStart += "_ARRAY";
        }

        macroStart += "(";

        // 1st argument: pointerAddr, which is the first argument to the call
        // TODO: extract it to a variable!
        llvm::raw_string_ostream os(macroStart);
        deleteExpr->getArg(0)->printPretty(os, nullptr, clang::PrintingPolicy(result.Context->getPrintingPolicy()));
        os.flush();
        macroStart += ", ";

        // 2nd parameter: source location. At least this is easy
        auto& sm = result.Context->getSourceManager();
        auto ploc = sm.getPresumedLoc(deleteExpr->getLocEnd());
        macroStart += "\"";
        macroStart += ploc.getFilename();
        macroStart += ":";
        macroStart += std::to_string(ploc.getLine());
        macroStart += "\", ";

        // 3rd parameter: the delete call. It's the expression itself

        clang::SourceLocation startLoc = deleteExpr->getLocStart();
        mRewriter->InsertText(startLoc, macroStart);

        // end added macro
        std::string macroEnd = ")";

        clang::SourceLocation endLoc = deleteExpr->getLocEnd();
        mRewriter->InsertTextAfterToken(endLoc, macroEnd);
    }

    clang::StringRef OpDeleteExprHandler::getID() const {
        return "typegrind";
    }
}
