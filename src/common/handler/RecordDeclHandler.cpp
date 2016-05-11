
#include "RecordDeclHandler.h"

#include <regex>

namespace typegrind
{

  RecordDeclHandler::RecordDeclHandler(clang::Rewriter*& rewriter, SpecializationHandler& specializationHandler)
          : BaseExprHandler(rewriter, specializationHandler)
  {
  }

  void RecordDeclHandler::run(const clang::ast_matchers::MatchFinder::MatchResult &result)
  {
    const clang::CXXRecordDecl* decl = result.Nodes.getNodeAs<clang::CXXRecordDecl>("decl");
    if(!decl) return;
    if (!decl->isThisDeclarationADefinition()) return;
    if (decl->isLocalClass()) return;

    auto insertPosition = decl->getRBraceLoc();
    if (!insertPosition.isValid()) return;
    if (!processingLocation(insertPosition)) return;

    std::string name = decl->isExternCContext() ? "TYPEGRIND_RECORD_APPEND_C" : "TYPEGRIND_RECORD_APPEND";

    MacroAdder recordEndMacroAdder(
            name,
            insertPosition,
            insertPosition,
            mRewriter
    );

    recordEndMacroAdder.commitBeforeStartLocation();
  }
}
