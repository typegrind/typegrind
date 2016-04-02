
#include "BaseExprHandler.h"

namespace typegrind
{
  BaseExprHandler::BaseExprHandler(clang::Rewriter*& rewriter)
          : mRewriter(rewriter)
  {
  }

  clang::StringRef BaseExprHandler::getID() const
  {
    return "typegrind";
  }

  bool BaseExprHandler::processingLocation(clang::SourceLocation loc)
  {
    if(mAlreadyProcessed.find(loc.getRawEncoding()) != mAlreadyProcessed.end()) return false;
    mAlreadyProcessed.insert(loc.getRawEncoding());
    return true;
  }

  void BaseExprHandler::addTypeInformationParameters(MacroAdder& macroAdder, clang::QualType const& typeInfo)
  {
    // Currently it is the concrete name if we can place it in the source without generating a template specialization
    // otherwise it's based on typeid, so we are missing a demangle function ...
    if (typeInfo.getTypePtr()->isTemplateTypeParmType())
    {
      // TODO: generate a (manual) specialization for this function (using marker templates?)
      // Let's just use type_info::name(), and demangle it somehow runtime...
      // or maybe create an initialization code dumping the mangled names for all affected specializations? (the ones skipped by the previous return)
      macroAdder.addParameter("TYPEGRIND_DEMANGLE(typeid(" + typeInfo.getAsString() + ").name())");
      macroAdder.addParameter("\"TODO\"");
    }
    else
    {
      macroAdder.addParameterAsString(typeInfo.getAsString());
      clang::QualType qt = typeInfo;
      if (!qt.isCanonical()) qt = qt.getCanonicalType();
      if (!qt->getPointeeType().isNull())
      {
        qt = qt->getPointeeType();
      }
      macroAdder.addParameterAsString(qt.getAsString());
    }
  }

}