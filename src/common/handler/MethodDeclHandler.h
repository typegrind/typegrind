
#pragma once

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Rewrite/Core/Rewriter.h>

#include "RegexSet.h"

namespace typegrind {
    class MethodDeclHandler : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:

        MethodDeclHandler(clang::Rewriter*& rewriter, RegexSet const& regexes);
        void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;

        clang::StringRef getID() const override;

    private:
        clang::Rewriter*& mRewriter;
        std::set<std::string> mVisited;
        RegexSet const& mRegexes;
    };
}
