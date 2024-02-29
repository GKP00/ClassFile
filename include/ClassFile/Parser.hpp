#pragma once

#include "ClassFile.hpp"
#include "Error.hpp"

namespace ClassFile
{
namespace Parser
{

ErrorOr<ClassFile> ParseClassFile(std::istream&);
ErrorOr<ConstantPool> ParseConstantPool(std::istream&);
ErrorOr< std::unique_ptr<CPInfo> > ParseConstant(std::istream&);

ErrorOr<FieldMethodInfo> ParseFieldMethodInfo(std::istream&, const ConstantPool&);
ErrorOr< std::unique_ptr<AttributeInfo> > ParseAttribute(std::istream&, const ConstantPool&);

ErrorOr<Instruction> ParseInstruction(std::istream&);

} //namespace Parser
} //namespace ClassFile
