#pragma once

#include "./Error.hpp"

#include <tuple>
#include <vector>

namespace ClassFile
{

//The below methods, on success, return a struct with the decoded typename
//and amount of characters gone over in the descriptor in order to decode it.
//
//The functions will succeed as long as the first characters of the input are 
//a valid decodeable string. Any following characters are ignored.
//
//This allows you to use the same function to parse a continuous list of encoded
//types, such as you get in a MethodDescriptor parameter list.
//
//To do that call the function, passing your string of continuous encoded type 
//names, then move the strings begin pointer by the amount of NParsed characters 
//as returned by the function on success. 
//Repeat until your string length is 0 or you encounter an error.
//
//If you know the input string should only contain a single encoded type then 
//you should ensure that the returned NParsed equals the input string length.
struct TypeAndNParsed
{
  std::string TypeName;
  size_t NParsed;
};
ErrorOr<TypeAndNParsed> DecodeFirstObjectType(std::string_view) noexcept;
ErrorOr<TypeAndNParsed> DecodeFirstArrayType(std::string_view) noexcept;
ErrorOr<TypeAndNParsed> DecodeFirstFieldDescriptor(std::string_view) noexcept;

//All base types are 1 character encoded so no need to return NParsed
ErrorOr<std::string> DecodeFirstBaseType(std::string_view) noexcept;
ErrorOr<std::string> DecodeBaseType(char c) noexcept;

//Does the same as DecodeFirstFieldDescriptor but ensures that the entire
//descriptor string was parsed. 
ErrorOr<std::string> DecodeFieldDescriptor(std::string_view) noexcept;

//Continuously calls DecodeFirstFieldDescriptor() until the full descriptor list 
//has been parsed or an error gets encountered.
//
//A descriptor string of length 0 is a valid and will return a 0 length vector.
ErrorOr<std::vector<std::string>> DecodeFieldDescriptors(std::string_view) noexcept;

struct MethodDescriptor
{
  std::string ReturnType;
  std::vector<std::string> ParamTypes;
};

ErrorOr<MethodDescriptor> DecodeMethodDescriptor(std::string_view) noexcept;

} //namespace ClassFile
