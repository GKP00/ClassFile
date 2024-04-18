#include "ClassFile/Attribute.hpp"

#include <fmt/core.h>

#include <map>
#include <cassert>

namespace ClassFile
{


static std::map<AttributeInfo::Type, std::string_view> typeNames =
{
  {AttributeInfo::Type::ConstantValue,   "ConstantValue"},
  {AttributeInfo::Type::Code,            "Code"},
  {AttributeInfo::Type::Exceptions,      "Exceptions"},
  {AttributeInfo::Type::SourceFile,      "SourceFile"},
  {AttributeInfo::Type::LineNumberTable, "LineNumberTable"},

  {AttributeInfo::Type::Raw, "_Raw"}
};

std::string_view AttributeInfo::GetTypeName(AttributeInfo::Type type) 
{
  auto itr = typeNames.find(type);

  assert(itr != typeNames.end());
  return std::get<1>(*itr);
}

ErrorOr<AttributeInfo::Type> AttributeInfo::GetType(std::string_view name) 
{
  for (auto itr : typeNames)
  {
    if (name == std::get<1>(itr))
      return std::get<0>(itr);
  }

  return Error{ fmt::format("AttributeInfo::GetType(): requested type for "
      "unknown type name: \"{}\"", name) };
}

std::string_view AttributeInfo::GetName() const
{
  return AttributeInfo::GetTypeName(this->GetType());
}

AttributeInfo::Type AttributeInfo::GetType() const
{
  return m_type;
}


} //namespace ClassFile
