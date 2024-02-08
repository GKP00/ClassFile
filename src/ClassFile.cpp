#include "ClassFile/ClassFile.hpp"

namespace ClassFile
{

const std::unordered_map<FieldMethodInfo::AccessFlag, std::string_view> 
FieldMethodInfo::FlagStrMap = 
{
#define FOO_TUPLE(X) {FieldMethodInfo::AccessFlag::X, #X}
  FOO_TUPLE(PUBLIC),
  FOO_TUPLE(PRIVATE),
  FOO_TUPLE(PROTECTED),
  FOO_TUPLE(STATIC),
  FOO_TUPLE(FINAL),
  FOO_TUPLE(SYNCHRONIZED),
  FOO_TUPLE(BRIDGE),
  FOO_TUPLE(VARARGS),
  FOO_TUPLE(NATIVE),
  FOO_TUPLE(ABSTRACT),
  FOO_TUPLE(STRICT),
  FOO_TUPLE(SYNTHETIC),
#undef FOO_TUPLE
};

ErrorOr<std::string_view> FieldMethodInfo::FlagToStr(U16 flag)
{
  auto it = FieldMethodInfo::FlagStrMap.find(static_cast<AccessFlag>(flag));

  if(it != FieldMethodInfo::FlagStrMap.end())
    return it->second;

  return Error{"FieldMethodInfo::FlagToStr(): undefined flag value."};
}

std::vector<std::string_view> FieldMethodInfo::FlagsToStrs() const
{
  std::vector<std::string_view> flags;

  for(const auto& it : FieldMethodInfo::FlagStrMap)
  {
    if(this->AccessFlags & static_cast<U16>(it.first))
      flags.push_back(it.second);
  }

  return flags;
}

const std::unordered_map<ClassFile::AccessFlag, std::string_view> 
ClassFile::FlagStrMap = 
{
#define FOO_TUPLE(X) {ClassFile::AccessFlag::X, #X}
  FOO_TUPLE(PUBLIC),
  FOO_TUPLE(FINAL),
  FOO_TUPLE(SUPER),
  FOO_TUPLE(INTERFACE),
  FOO_TUPLE(ABSTRACT),
  FOO_TUPLE(SYNTHETIC),
  FOO_TUPLE(ANNOTATION),
  FOO_TUPLE(ENUM),
#undef FOO_TUPLE
};

ErrorOr<std::string_view> ClassFile::FlagToStr(U16 flag)
{
  auto it = ClassFile::FlagStrMap.find(static_cast<AccessFlag>(flag));

  if(it != ClassFile::FlagStrMap.end())
    return it->second;

  return Error{"ClassFile::FlagToStr(): undefined flag value."};
}

std::vector<std::string_view> ClassFile::FlagsToStrs() const
{
  std::vector<std::string_view> flags;

  for(const auto& it : ClassFile::FlagStrMap)
  {
    if(this->AccessFlags & static_cast<U16>(it.first))
      flags.push_back(it.second);
  }

  return flags;
}


} //namespace: ClassFile
