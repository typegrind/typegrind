
#include "MacroExpanderAction.h"

#include <clang/Basic/SourceManager.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Lex/PreprocessorOptions.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Rewrite/Frontend/Rewriters.h>
#include <llvm/Support/Path.h>
#include <llvm/Support/raw_ostream.h>
#include <unordered_map>

using namespace clang;

namespace {
bool isCppFile(clang::CompilerInstance const& compiler) {
  clang::LangOptions const Opts = compiler.getLangOpts();
  return Opts.CPlusPlus;
}

/// RewriteMacrosInInput - Implement -rewrite-macros mode.
void expandMacrosInInput(Preprocessor& PP,
                         std::unordered_map<std::string, std::string>& expandedFiles) {
  SourceManager& SM = PP.getSourceManager();

  Rewriter Rewrite;
  Rewrite.setSourceMgr(SM, PP.getLangOpts());

  // Get the first preprocessing token.
  PP.EnterMainSourceFile();
  Token PPTok;
  PP.Lex(PPTok);

  bool inExpansion = false;
  SourceLocation MacroEndLoc;
  StringRef lastMacro;
  std::string Expansion = " */ ";

  while (PPTok.isNot(tok::eof)) {
    SourceLocation PPLoc1 = PPTok.getLocation();

    if (inExpansion) {
      Expansion += ' ' + PP.getSpelling(PPTok);
    }
    SourceLocation PPLoc2 = SM.getExpansionLoc(PPLoc1);

    if (PPLoc1.isMacroID()) {
      if (PPTok.isAnyIdentifier()) {
        MacroInfo* mi = PP.getMacroInfo(PPTok.getIdentifierInfo());
        if (mi) PP.DumpMacro(*mi);
      }

      if (PPLoc1.isMacroID()) lastMacro = PP.getImmediateMacroName(PPLoc1);
      // if (lastMacro == "PAR") {
      // TODO: implement condition
      if (PP.isAtStartOfMacroExpansion(PPLoc1)) {
        Rewrite.InsertTextAfter(PPLoc2, " /* ");
        inExpansion = true;
        Expansion += ' ' + PP.getSpelling(PPTok);
      }

      if (PP.isAtEndOfMacroExpansion(PPLoc1)) {
        inExpansion = false;
        auto sr = SM.getExpansionRange(PPLoc1);
        SourceLocation PPLoc3 = sr.second;
        Rewrite.InsertTextAfterToken(PPLoc3, Expansion);
        Expansion = " */ ";
      }
      //}
    }

    PP.Lex(PPTok);
  }

  auto end = Rewrite.buffer_end();
  for (auto it = Rewrite.buffer_begin(); it != end; ++it) {
    const FileEntry* Entry = Rewrite.getSourceMgr().getFileEntryForID(it->first);
    if (Entry && Entry->isValid()) {
      std::string originalFileName(Entry->getName());
      expandedFiles.insert(std::pair<std::string, std::string>(
          originalFileName, std::string(it->second.begin(), it->second.end())));
    }
  }
}
}

namespace typegrind {

MacroExpanderAction::~MacroExpanderAction() {}

MacroExpanderAction::MacroExpanderAction(clang::tooling::ClangTool& targetTool)
    : targetTool(targetTool) {}

bool MacroExpanderAction::ParseArgs(const clang::CompilerInstance& CI,
                                    const std::vector<std::string>& arg) {
  return isCppFile(CI);
}

std::unique_ptr<clang::ASTConsumer> MacroExpanderAction::newASTConsumer() {
  expand();
  return std::unique_ptr<clang::ASTConsumer>();
}

std::unique_ptr<clang::ASTConsumer> MacroExpanderAction::CreateASTConsumer(
    clang::CompilerInstance& CI, llvm::StringRef InFile) {
  mCI = &CI;
  expand();
  return std::unique_ptr<clang::ASTConsumer>();
}

bool MacroExpanderAction::handleBeginSource(clang::CompilerInstance& CI, llvm::StringRef fileName) {
  if (!isCppFile(CI)) return false;

  mCI = &CI;

  return true;
}

void MacroExpanderAction::expand() {
  clang::Preprocessor& PP = mCI->getPreprocessor();

  std::unordered_map<std::string, std::string> fileMap;
  expandMacrosInInput(PP, fileMap);
  for (auto& p : fileMap) {
    targetTool.mapVirtualFile(p.first, p.second);
  }
}
}
