//
// Created by vagrant on 4/2/16.
//

#ifndef TYPEGRIND_MACROADDER_H
#define TYPEGRIND_MACROADDER_H

#include <clang/Basic/SourceManager.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <llvm/Support/raw_ostream.h>

namespace typegrind {
class MacroAdder {
 public:
  MacroAdder(std::string macroName, clang::SourceLocation startLocation,
             clang::SourceLocation endLocation, clang::Rewriter*& rewriter);

  void commitAroundLocations();

  void commitAfterStartLocation();

  void commitBeforeStartLocation();

  void addParameter(std::string parameter);

  void addParameterAsString(std::string parameter);

  /**
   * Returns the start stream, for low level operations
   */
  llvm::raw_string_ostream& startBuffer();

  /**
   * Returns the end stream, for low level operations
   */
  llvm::raw_string_ostream& endBuffer();

 private:
  clang::Rewriter*& mRewriter;
  clang::SourceManager& mSourceManager;

  clang::SourceLocation mStartLocation;
  clang::SourceLocation mEndLocation;

  std::string mStartBufferInternal;
  std::string mEndBufferInternal;
  llvm::raw_string_ostream mStartBuffer;
  llvm::raw_string_ostream mEndBuffer;
};
}

#endif  // TYPEGRIND_MACROADDER_H
