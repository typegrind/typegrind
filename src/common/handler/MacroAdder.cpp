//
// Created by vagrant on 4/2/16.
//

#include "MacroAdder.h"

namespace
{
  /**
   * Expands the location if it's a macro, and returns the location at the start
   * of the original source (before expansion).
   */
  clang::SourceLocation getLocationAtExpansionStart(clang::SourceLocation original, clang::SourceManager& sm)
  {
    if (original.isMacroID())
    {
      auto expansionRange = sm.getImmediateExpansionRange(original);
      return expansionRange.first;
    }
    return original;
  }

  /**
   * Expands the location if it's a macro, and returns the location at the end
   * of the original source (before expansion).
   */
  clang::SourceLocation getLocationAtExpansionEnd(clang::SourceLocation original, clang::SourceManager& sm)
  {
    if (original.isMacroID())
    {
      auto expansionRange = sm.getImmediateExpansionRange(original);
      return expansionRange.second;
    }
    return original;
  }
}

namespace typegrind
{

  MacroAdder::MacroAdder(std::string macroName, clang::SourceLocation startLocation, clang::SourceLocation endLocation, clang::Rewriter*& rewriter)
          : mRewriter(rewriter)
          , mSourceManager(mRewriter->getSourceMgr())
          , mStartLocation(getLocationAtExpansionStart(startLocation, mSourceManager))
          , mEndLocation(getLocationAtExpansionEnd(endLocation, mSourceManager))
          , mStartBuffer(mStartBufferInternal)
          , mEndBuffer(mEndBufferInternal)
  {
    mStartBuffer << macroName << "(";
    // first parameter is always the start location!
    auto presumedLoc = mSourceManager.getPresumedLoc(mStartLocation);
    mStartBuffer << "\"" << presumedLoc.getFilename() << ":" << presumedLoc.getLine() << "\"";
    mStartBuffer << ", ";
  }

  void MacroAdder::commit()
  {
    mEndBuffer << ")";

    mRewriter->InsertText(mStartLocation, mStartBuffer.str());
    mRewriter->InsertTextAfterToken(mEndLocation, mEndBuffer.str());
  }

  void MacroAdder::addParameter(std::string parameter)
  {
    mStartBuffer << parameter << ", ";
  }

  void MacroAdder::addParameterAsString(std::string parameter)
  {
    // TODO: escape!
    mStartBuffer << "\"" << parameter << "\", ";
  }

  llvm::raw_string_ostream& MacroAdder::startBuffer()
  {
    return mStartBuffer;
  }

  llvm::raw_string_ostream& MacroAdder::endBuffer()
  {
    return mEndBuffer;
  }
}