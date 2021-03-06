
#include "BaseExprHandler.h"

namespace typegrind {
BaseExprHandler::BaseExprHandler(clang::Rewriter*& rewriter,
                                 SpecializationHandler& specializationHandler)
    : mRewriter(rewriter), mSpecializationHandler(specializationHandler) {}

clang::StringRef BaseExprHandler::getID() const { return "typegrind"; }

bool BaseExprHandler::processingLocation(clang::SourceLocation loc) {
  if (mAlreadyProcessed.find(loc.getRawEncoding()) != mAlreadyProcessed.end()) return false;
  mAlreadyProcessed.insert(loc.getRawEncoding());
  return true;
}

bool BaseExprHandler::isReallyDependentType(
    clang::QualType const& typeInfo, PointeeConversion convertToPointee /*=KEEP_ORIGINAL_TYPE*/) {
  clang::LangOptions options;
  clang::PrintingPolicy policy(options);
  policy.SuppressUnwrittenScope = true;
  policy.SuppressTagKeyword = true;

  clang::QualType baseType = typeInfo;
  if (convertToPointee == CONVERT_TO_POINTEE && !baseType->getPointeeType().isNull()) {
    baseType = baseType->getPointeeType();
  }
  // TODO: better!
  std::string name = baseType.getAsString(policy);
  return name == "<dependent type>";
}

void BaseExprHandler::addTypeInformationParameters(
    MacroAdder& macroAdder, clang::QualType const& typeInfo, unsigned specificUniqId,
    PointeeConversion convertToPointee /*=KEEP_ORIGINAL_TYPE*/) {
  clang::LangOptions options;
  clang::PrintingPolicy policy(options);
  policy.SuppressUnwrittenScope = true;
  policy.SuppressTagKeyword = true;

  if (typeInfo.getTypePtr()->isInstantiationDependentType()) {
    clang::QualType baseType = typeInfo;
    if (convertToPointee == CONVERT_TO_POINTEE && !baseType->getPointeeType().isNull()) {
      baseType = baseType->getPointeeType();
    }
    macroAdder.addParameter("TYPEGRIND_CANONICAL_TYPE(TYPEGRIND_TYPE(" +
                            baseType.getAsString(policy) + "))");
    macroAdder.addParameter("TYPEGRIND_SPECIFIC_TYPE(TYPEGRIND_TYPE(" +
                            baseType.getAsString(policy) + "), " + std::to_string(specificUniqId) +
                            ")");
  } else {
    clang::QualType baseType = typeInfo;
    if (convertToPointee == CONVERT_TO_POINTEE && !baseType->getPointeeType().isNull()) {
      baseType = baseType->getPointeeType();
    }
    macroAdder.addParameterAsString(baseType.getAsString());
    clang::QualType qt = typeInfo;
    if (!qt.isCanonical()) qt = qt.getCanonicalType();
    if (convertToPointee == CONVERT_TO_POINTEE && !qt->getPointeeType().isNull()) {
      qt = qt->getPointeeType();
    }
    macroAdder.addParameterAsString(qt.getAsString());
  }
}

void BaseExprHandler::handleSpecializedType(
    clang::QualType const& typeInfo, unsigned specificUniqId,
    PointeeConversion convertToPointee /*=KEEP_ORIGINAL_TYPE*/) {
  mSpecializationHandler.handleSpecializedType(typeInfo, specificUniqId, convertToPointee);
}

void BaseExprHandler::addSizeOfParameter(MacroAdder& macroAdder, clang::QualType const& typeInfo) {
  clang::LangOptions options;
  clang::PrintingPolicy policy(options);
  policy.SuppressUnwrittenScope = true;
  policy.SuppressTagKeyword = true;
  macroAdder.endBuffer() << ", sizeof(" << typeInfo.getAsString(policy) << ")";
}
}
