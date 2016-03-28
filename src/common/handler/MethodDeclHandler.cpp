
#include "MethodDeclHandler.h"

#include <regex>

namespace typegrind
{

      MethodDeclHandler::MethodDeclHandler(clang::Rewriter*& rewriter, MethodMatcher const& matchers)
              : mRewriter(rewriter)
              , mMatchers(matchers)
      {
      }

      clang::StringRef MethodDeclHandler::getID() const {
          return "typegrind";
      }

      void MethodDeclHandler::run(const clang::ast_matchers::MatchFinder::MatchResult &result)
      {
        const clang::CXXMethodDecl* decl = result.Nodes.getNodeAs<clang::CXXMethodDecl>("decl");
        if(!decl) return;
        if (!decl->hasBody()) return;

        if(decl->getTemplateSpecializationKind() == clang::TSK_ImplicitInstantiation) return;

        //llvm::outs() << decl->getNameInfo().getAsString() << "\n";
        std::string prettyName;
        {
          llvm::raw_string_ostream prettyNameStream(prettyName);
          decl->getCanonicalDecl()->getNameForDiagnostic(prettyNameStream, clang::PrintingPolicy(result.Context->getPrintingPolicy()), true);
          prettyNameStream.flush();
        }

        auto match = mMatchers.matches(prettyName);
        if(!match) return;

        auto& sm = result.Context->getSourceManager();

        clang::Stmt *funcBody = decl->getBody();
        auto insertPosition = funcBody->getSourceRange().getBegin();
        auto ploc = sm.getPresumedLoc(insertPosition);

        // only instrument a source location once
        if(mAlreadyEncoded.find(insertPosition.getRawEncoding()) != mAlreadyEncoded.end()) return;
        mAlreadyEncoded.insert(insertPosition.getRawEncoding());

        std::string locStr = "\"";
        locStr += ploc.getFilename();
        locStr += ":";
        locStr += std::to_string(ploc.getLine());
        locStr += "\"";

        mRewriter->InsertTextAfterToken(insertPosition, " TYPEGRIND_LOG_METHOD_ENTER(\""+prettyName+"\", "+locStr+", \""+match->custom_name+"\", "+std::to_string(match->flags)+") ");

        // if constructor - initializer lists
        {
          const clang::CXXConstructorDecl* constrDecl = result.Nodes.getNodeAs<clang::CXXConstructorDecl>("decl");
          if(constrDecl)
          {
            for(auto initer: constrDecl->inits())
            {
              if(!initer->isMemberInitializer()) continue;
              mRewriter->InsertTextAfterToken(initer->getLParenLoc(), "TYPEGRIND_LOG_METHOD_INITIALIZER(\""+prettyName+"\", "+locStr+", \""+match->custom_name+"\", "+std::to_string(match->flags)+", (");
              mRewriter->InsertTextBefore(initer->getRParenLoc(), "))");
            }
          }
        }
      }
}
