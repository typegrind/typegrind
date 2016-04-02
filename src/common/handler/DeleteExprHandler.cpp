
#include "DeleteExprHandler.h"

namespace typegrind {

    DeleteExprHandler::DeleteExprHandler(clang::Rewriter *&rewriter)
            : mRewriter(rewriter) {
    }

    void DeleteExprHandler::run(const clang::ast_matchers::MatchFinder::MatchResult &result) {
        const clang::CXXDeleteExpr* deleteExpr = result.Nodes.getNodeAs<clang::CXXDeleteExpr>("deleteStmt");
        if (nullptr == deleteExpr)
        {
            llvm::errs() << "Couldn't convert MatcherResult to CXXDeleteExpr!\n";
            return;
        }

        auto& sm = result.Context->getSourceManager();

        clang::SourceLocation startLoc = deleteExpr->getArgument()->getLocStart();
        // only instrument a source location once
        if(mAlreadyEncoded.find(startLoc.getRawEncoding()) != mAlreadyEncoded.end()) return;
        mAlreadyEncoded.insert(startLoc.getRawEncoding());

        std::string macroStart = "TYPEGRIND_LOG_DELETE";

        if (deleteExpr->isArrayForm()) {
            macroStart += "_ARRAY";
        }
        macroStart += "(";

	// 1st parameter: type
	auto deletedType = deleteExpr->getArgument()->getType();
	if (deletedType.getTypePtr()->isTemplateTypeParmType()) {
		// TODO: somehow add compiler specific demangle ...
		// We could generate a (manual) specialization for this function, but for now, this is enough
		// Let's just use type_info::name(), and demangle it somehow runtime...
		// or maybe create an initialization code dumping the mangled names for all affected specializations? (the ones skipped by the previous return)
		macroStart += "TYPEGRIND_DEMANGLE(typeid(" + deletedType.getAsString() + ").name()), \"TODO\"";
	} else {
		macroStart += "\"" + deletedType.getAsString() + "\""; // + (int)deletedType.getTypePtr()->getAsTagDecl();
		macroStart += ", \"" + deletedType.getCanonicalType()->getPointeeType().getAsString() + "\""; // + (int)deletedType.getTypePtr()->getAsTagDecl();
	}
        macroStart += ", ";

        // 2nd parameter: source location
        auto ploc = sm.getPresumedLoc(deleteExpr->getLocEnd());
        macroStart += "\"";
        macroStart += ploc.getFilename();
        macroStart += ":";
        macroStart += std::to_string(ploc.getLine());
        macroStart += "\", ";

        mRewriter->InsertText(startLoc, macroStart);

        // 3rd parameter: the expression itself
        // end added function call
        std::string macroEnd = ")";

        clang::SourceLocation endLoc = deleteExpr->getArgument()->getLocEnd();
        mRewriter->InsertTextAfterToken(endLoc, macroEnd);
    }

    clang::StringRef DeleteExprHandler::getID() const {
        return "typegrind";
    }
}
