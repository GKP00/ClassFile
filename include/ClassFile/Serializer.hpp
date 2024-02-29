#pragma once

#include "ClassFile.hpp"
#include "Error.hpp"

namespace ClassFile
{
namespace Serializer
{

ErrorOr<void> SerializeClassFile(std::ostream&, const ClassFile&);
ErrorOr<void> SerializeConstantPool(std::ostream&, const ConstantPool&);
ErrorOr<void> SerializeConstant(std::ostream&, const CPInfo&);

ErrorOr<void> SerializeFieldMethod(std::ostream&, const FieldMethodInfo&);
ErrorOr<void> SerializeAttribute(std::ostream&, const AttributeInfo&);

ErrorOr<void> SerializeInstruction(std::ostream&, const Instruction&);

} //namespace Serializer
} //namespace ClassFile

