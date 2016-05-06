
#include "BaseExprHandler.h"

namespace typegrind
{
  BaseExprHandler::BaseExprHandler(clang::Rewriter*& rewriter, SpecializationHandler& specializationHandler)
          : mRewriter(rewriter)
          , mSpecializationHandler(specializationHandler)
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

  void BaseExprHandler::addTypeInformationParameters(MacroAdder& macroAdder, clang::QualType const& typeInfo, unsigned specificUniqId, PointeeConversion convertToPointee/*=KEEP_ORIGINAL_TYPE*/)
  {
    clang::LangOptions options;
    clang::PrintingPolicy policy(options);
    policy.SuppressUnwrittenScope = true;
    policy.SuppressTagKeyword = true;

    if (typeInfo.getTypePtr()->isInstantiationDependentType())
    {
      macroAdder.addParameter("TYPEGRIND_CANONICAL_TYPE((" + typeInfo.getAsString(policy) + "))");
      macroAdder.addParameter("TYPEGRIND_SPECIFIC_TYPE((" + typeInfo.getAsString(policy) + "), " + std::to_string(specificUniqId) + ")");
    }
    else
    {
      clang::QualType baseType = typeInfo;
      if (convertToPointee == CONVERT_TO_POINTEE && !baseType->getPointeeType().isNull())
      {
        baseType = baseType->getPointeeType();
      }
      macroAdder.addParameterAsString(baseType.getAsString());
      clang::QualType qt = typeInfo;
      if (!qt.isCanonical()) qt = qt.getCanonicalType();
      if (convertToPointee == CONVERT_TO_POINTEE && !qt->getPointeeType().isNull())
      {
        qt = qt->getPointeeType();
      }
      macroAdder.addParameterAsString(qt.getAsString());
    }

  }

  void BaseExprHandler::handleSpecializedType(clang::QualType const& typeInfo, unsigned specificUniqId, PointeeConversion convertToPointee/*=KEEP_ORIGINAL_TYPE*/)
  {
    mSpecializationHandler.handleSpecializedType(typeInfo, specificUniqId, convertToPointee);
  }

  void BaseExprHandler::addSizeOfParameter(MacroAdder& macroAdder, clang::QualType const& typeInfo)
  {
    clang::LangOptions options;
    clang::PrintingPolicy policy(options);
    policy.SuppressUnwrittenScope = true;
    policy.SuppressTagKeyword = true;
    macroAdder.startBuffer() << "sizeof(" << typeInfo.getAsString(policy) << "), ";
  }


}