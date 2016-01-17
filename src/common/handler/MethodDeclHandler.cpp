
#include "MethodDeclHandler.h"

#include <regex>

namespace typegrind
{

      MethodDeclHandler::MethodDeclHandler(clang::Rewriter*& rewriter, RegexSet const& regexes)
              : mRewriter(rewriter)
              , mRegexes(regexes)
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

        try
        {
          if(!mRegexes.matches(prettyName)) return;
        }
        catch(...)
        {
          return;
        }

        //if(mVisited.find(prettyName) != mVisited.end()) return;
        //mVisited.insert(prettyName);

        clang::Stmt *funcBody = decl->getBody();
        mRewriter->InsertTextAfterToken(funcBody->getSourceRange().getBegin(), " TYPEGRIND_METHOD_ENTER(\""+prettyName+"\") ");

        // if constructor - initializer lists
        {
          const clang::CXXConstructorDecl* constrDecl = result.Nodes.getNodeAs<clang::CXXConstructorDecl>("decl");
          if(constrDecl)
          {
            for(auto initer: constrDecl->inits())
            {
              if(!initer->isMemberInitializer()) continue;
              mRewriter->InsertTextAfterToken(initer->getLParenLoc(), "TYPEGRIND_METHOD_INITIALIZER(\""+prettyName+"\", (");
              mRewriter->InsertTextBefore(initer->getRParenLoc(), "))");
            }
          }
        }
      }
}
