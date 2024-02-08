#pragma once

#include "Defs.hpp"
#include "ConstantPool.hpp"
#include "Attribute.hpp"

namespace ClassFile
{

struct FieldMethodInfo
{
  U16 AccessFlags;
  U16 NameIndex;
  U16 DescriptorIndex;
  std::vector< std::unique_ptr<AttributeInfo> > Attributes;

  enum class AccessFlag : U16
  {
    PUBLIC       = 0x0001,
    PRIVATE      = 0x0002,
    PROTECTED    = 0x0004,
    STATIC       = 0x0008,
    FINAL        = 0x0010,
    SYNCHRONIZED = 0x0020,
    BRIDGE       = 0x0040,
    VARARGS      = 0x0080,
    NATIVE       = 0x0100,
    ABSTRACT     = 0x0400,
    STRICT       = 0x0800,
    SYNTHETIC    = 0x1000,
  };

  static const std::unordered_map<AccessFlag, std::string_view> FlagStrMap;
  static ErrorOr<std::string_view> FlagToStr(U16 flag);
  std::vector<std::string_view> FlagsToStrs() const;
};

struct ClassFile 
{
  U32 Magic;
  U16 MinorVersion;
  U16 MajorVersion;
  ConstantPool ConstPool;
  U16 AccessFlags;
  U16 ThisClass;
  U16 SuperClass;
  std::vector<U16> Interfaces;
  std::vector<FieldMethodInfo> Fields;
  std::vector<FieldMethodInfo> Methods;
  std::vector< std::unique_ptr<AttributeInfo> > Attributes;
};

} //namespace: ClassFile
