
#include "FunctionDeclHandler.h"

#include <regex>

namespace typegrind {

FunctionDeclHandler::FunctionDeclHandler(clang::Rewriter*& rewriter,
                                         SpecializationHandler& specializationHandler,
                                         MethodMatcher const& matchers)
    : BaseExprHandler(rewriter, specializationHandler), mMatchers(matchers) {}

void FunctionDeclHandler::run(const clang::ast_matchers::MatchFinder::MatchResult& result) {
  const clang::FunctionDecl* decl = result.Nodes.getNodeAs<clang::FunctionDecl>("decl");
  if (!decl) return;
  if (!decl->hasBody()) return;

  if (decl->getTemplateSpecializationKind() == clang::TSK_ImplicitInstantiation) return;

  std::string prettyName;
  {
    llvm::raw_string_ostream prettyNameStream(prettyName);
    decl->getCanonicalDecl()->getNameForDiagnostic(
        prettyNameStream, clang::PrintingPolicy(result.Context->getPrintingPolicy()), true);
    prettyNameStream.flush();
  }
  auto match = mMatchers.matches(prettyName);

  if (!match) {
    // TODO: extract this to a policy class.
    if (decl->isInlined() || decl->isInlineSpecified() ||
    decl->hasTrivialBody()) return;
  }

  clang::Stmt* funcBody = decl->getBody();
  auto insertPosition = funcBody->getSourceRange().getBegin();
  if (!processingLocation(insertPosition)) return;

  MacroAdder functionEnterMacro(
      match ? "TYPEGRIND_LOG_FUNCTION_ENTER" : "TYPEGRIND_LOG_FUNCTION_AUTO_ENTER", insertPosition,
      insertPosition, mRewriter);

  functionEnterMacro.addParameterAsString(prettyName);

  if (match) {
    functionEnterMacro.addParameterAsString(match->custom_name);
    functionEnterMacro.addParameter(std::to_string(match->flags));
  }

  functionEnterMacro.commitAfterStartLocation();
}
}
