#include "ClassFile/Misc.hpp"

#include <fmt/core.h>

namespace ClassFile
{

//ObjectType grammar (spaces not included): 
//  L typename ;
ErrorOr<TypeAndNParsed> DecodeFirstObjectType(std::string_view desc) noexcept
{
  if(desc.length() < 3)
  {
    return Error{fmt::format(
        "DecodeFirstObjectType(\"{}\") failed: "
        "input string length is less than the minimum possible 3 characters "
        "for an object type descriptor", 
        desc)};
  }

  if(desc[0] != 'L')
  {
    return Error{fmt::format(
        "DecodeFirstObjectType(\"{}\") failed: "
        "expected descriptor to start with 'L' instead '{}' was encountered",
        desc, desc[0])};
  }

  std::string typeName;

  for(size_t i = 1; i < desc.length(); ++i)
  {
    if(desc[i] == ';')
      return TypeAndNParsed{typeName, i+1};

    typeName += desc[i];
  }

  return Error{fmt::format(
      "DecodeFirstObjectType(\"{}\") failed: "
      "never encountereed terminating ';' char",
      desc)};
}

//ArrayType grammar (spaces not included): 
//  [ FieldDescriptor
ErrorOr<TypeAndNParsed> DecodeFirstArrayType(std::string_view desc) noexcept
{
  if(desc.length() < 2)
  {
    return Error{fmt::format(
        "DecodeFirstArrayType(\"{}\") failed: "
        "input string length is less than the minimum possible 2 characters"
        "for an array type descriptor", 
        desc)};
  }

  if(desc[0] != '[')
  {
    return Error{fmt::format(
        "DecodeFirstArrayType(\"{}\") failed: "
        "expected descriptor to start with '[' instead '{}' was encountered",
        desc, desc[0])};
  }

  auto errOrDecoded = DecodeFirstFieldDescriptor(desc.substr(1, desc.npos));

  if(errOrDecoded.IsError())
  {
    return Error{fmt::format(
        "DecodeFirstArrayType(\"{}\") failed: "
        "error encountered while decoding the field descriptor\n  {}",
        desc, errOrDecoded.GetError().What)};

  }

  auto decoded = errOrDecoded.Get();
  std::string typeName = std::move(decoded.TypeName);
  typeName += "[]";

  return TypeAndNParsed{ std::move(typeName), decoded.NParsed+1};
}

//FieldDescriptor grammar:
//  ObjectType
//  ArrayType
//  BasicType
ErrorOr<TypeAndNParsed> DecodeFirstFieldDescriptor(std::string_view desc) noexcept
{
  if(desc.empty())
  {
    return Error{fmt::format(
        "DecodeFirstFieldDescriptor(\"{}\") failed: "
        "empty descriptor string is invalid",
        desc)};
  }

  auto errOrObjType = DecodeFirstObjectType(desc);

  if(!errOrObjType.IsError())
    return errOrObjType.Release();

  auto errOrArrType = DecodeFirstArrayType(desc);

  if(!errOrArrType.IsError())
    return errOrArrType.Release();

  auto errOrBasType = DecodeFirstBaseType(desc);

  if(!errOrBasType.IsError())
    return TypeAndNParsed{errOrBasType.Release(), 1};

  return Error{fmt::format(
      "DecodeFirstFieldDescriptor(\"{}\") failed: \n"
      "  - Couldn't decode as base type: {}\n"
      "  - Couldn't decode as object type: {}\n"
      "  - Couldn't decode as array type: {}",
      desc, 
      errOrBasType.GetError().What,
      errOrObjType.GetError().What,
      errOrArrType.GetError().What)};
}

ErrorOr<std::string> DecodeFirstBaseType(std::string_view desc) noexcept
{
  if(desc.empty())
  {
    return Error{fmt::format(
        "DecodeFirstBaseType(\"{}\") failed: "
        "empty descriptor string is invalid",
        desc)};
  }

  static std::tuple<char, std::string_view> baseTypes[] = 
  {
    {'B', "byte"},
    {'C', "char"},
    {'D', "double"},
    {'F', "float"},
    {'I', "int"},
    {'J', "long"},
    {'S', "short"},
    {'Z', "boolean"},
  };

  //iterating a small list should be faster than some kind of map lookup
  for(auto baseType: baseTypes)
  {
    if(desc[0] == std::get<0>(baseType))
      return std::string{std::get<1>(baseType)};
  }

  return Error{fmt::format(
      "DecodeFirstBaseType(\"{}\") failed: "
      "'{}' does not correspond to a valid base type",
      desc, desc[0])};
}

ErrorOr<std::string> DecodeBaseType(char c) noexcept 
{ 
  auto errOrType = DecodeFirstBaseType(std::string_view{&c, 1});

  if(errOrType.IsError())
    return errOrType.GetError();

  return errOrType.Release();
}

ErrorOr<std::string> DecodeFieldDescriptor(std::string_view desc) noexcept
{
  auto errOrDecoded = DecodeFirstFieldDescriptor(desc);

  if(errOrDecoded.IsError())
    return errOrDecoded.GetError();

  auto decoded = errOrDecoded.Get();

  if(decoded.NParsed != desc.length())
  {
    return Error{fmt::format(
        "DecodeFieldDescriptor(\"{}\") failed: "
        "decoded part of descriptor as \"{}\" but then encountered unexpected "
        "character(s) \"{}\" at string offset {}",
        desc, decoded.TypeName, desc.substr(decoded.NParsed, desc.npos), decoded.NParsed)};
  }

  return std::string{ std::move(decoded.TypeName) };
}

ErrorOr<std::vector<std::string>> DecodeFieldDescriptors(std::string_view desc) noexcept
{
  std::vector<std::string> decodedTypeNames;

  while(!desc.empty())
  {
    auto errOrDecoded = DecodeFirstFieldDescriptor(desc);

    if(errOrDecoded.IsError())
      return errOrDecoded.GetError();

    auto decoded = errOrDecoded.Get();

    decodedTypeNames.emplace_back( std::move(decoded.TypeName) );
    desc = desc.substr(decoded.NParsed, desc.npos);
  }

  return decodedTypeNames;
}

//MethodDescriptor grammar (spaces not included):
//  ( FieldDescriptor* ) ReturnDescriptor
//
//ReturnDescriptor:
//  FieldDescriptor
//  V 
//
//'V' is a basetype that is only valid in method descriptors and it indicates
//"void" type
ErrorOr<MethodDescriptor> DecodeMethodDescriptor(std::string_view desc) noexcept
{
  if(desc.length() < 3)
  {
    return Error{fmt::format(
        "DecodeMethodDescriptor(\"{}\") failed: "
        "input string length is less than the minimum possible 3 characters "
        "for a valid method descriptor", 
        desc)};
  }

  if(desc[0] != '(')
  {
    return Error{fmt::format(
        "DecodeMethodDescriptor(\"{}\") failed: "
        "expected descriptor to start with '(' instead '{}' was encountered",
        desc, desc[0])};
  }

  auto endOfParams = desc.find(')');

  if(endOfParams == desc.npos)
  {
    return Error{fmt::format(
        "DecodeMethodDescriptor(\"{}\") failed: "
        "unable to find expected character ')'",
        desc)};
  }

  std::string_view parameterDescs = desc.substr(1, endOfParams-1);
  auto errOrParameterTypeList = DecodeFieldDescriptors(parameterDescs);

  if(errOrParameterTypeList.IsError())
  {
    return Error{fmt::format(
        "DecodeMethodDescriptor(\"{}\") failed: "
        "unable to decode a parameter in parameter list:\n  - {}",
        desc, errOrParameterTypeList.GetError().What)};
  }

  std::string_view returnDesc = desc.substr(endOfParams+1, desc.npos);

  if(returnDesc.empty())
  {
    return Error{fmt::format(
        "DecodeMethodDescriptor(\"{}\") failed: "
        "empty return type field descriptor",
        desc)};
  }

  std::string returnType;

  if(returnDesc[0] == 'V')
  {
    returnType = "void";
  }
  else
  {
    auto errOrRetType = DecodeFieldDescriptor(returnDesc);

    if(errOrRetType.IsError())
    {
      return Error{fmt::format(
          "DecodeMethodDescriptor(\"{}\") failed: "
          "invalid return type field descriptor: \n  {}",
          desc, errOrRetType.GetError().What)};
    }

    returnType = errOrRetType.Release();
  }

  return MethodDescriptor{returnType, errOrParameterTypeList.Release()};
}

} //namespace ClassFile
