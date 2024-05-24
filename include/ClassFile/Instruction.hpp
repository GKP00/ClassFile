#pragma once

#include "OpCodes.hpp"
#include "Error.hpp"

#include <vector>
#include <memory>
#include <functional>
#include <cassert>

namespace ClassFile
{

class Instruction
{
  public:
  static ErrorOr<Instruction> MakeInstruction(OpCode);

  OpCode GetOpCode() const;
  std::string_view GetMnemonic() const;
  size_t GetNOperands() const;
  OperandType GetOperandType(size_t index) const;
  size_t GetOperandSize(size_t index) const;
  size_t GetLength() const;
  bool IsComplex() const;

  template <typename T>
  ErrorOr< std::reference_wrapper<T> > Operand(size_t index)
  {
    ErrorOr<size_t> errOrOffset = this->validateTypeAndGetOffset<T>(index);

    if(errOrOffset.IsError())
      return errOrOffset.GetError();

    T& ref = reinterpret_cast<T&>(this->operandBytes.data()[errOrOffset.Get()]);
    return std::reference_wrapper<T>(ref);
  }

  template <typename T>
  ErrorOr< std::reference_wrapper<const T> > Operand(size_t index) const
  {
    ErrorOr<size_t> errOrOffset = this->validateTypeAndGetOffset<T>(index);

    if(errOrOffset.IsError())
      return errOrOffset.GetError();

    const T& ref = reinterpret_cast<const T&>(this->operandBytes.data()[errOrOffset.Get()]);
    return std::reference_wrapper<const T>(ref);
  }

  ErrorOr<S32> GetOperand(size_t index) const;
  ErrorOr<void> SetOperand(size_t index, S32 value);

  private:
  OpCode op;
  std::vector<U8> operandBytes;

  Instruction() = default;


  template <typename T>
  bool isValidType(size_t index) const
  {
    switch( this->GetOperandType(index) )
    {
      case TypeS32: return {typeid(T) == typeid(S32)}; 
      case TypeS16: return {typeid(T) == typeid(S16)}; 
      case TypeS8:  return {typeid(T) == typeid(S8) }; 
      case TypeU16: return {typeid(T) == typeid(U16)}; 
      case TypeU8:  return {typeid(T) == typeid(U8) }; 
    }

    return false;
  }

  Error oobError(size_t) const;
  Error castError(size_t, std::string_view) const;


  template <typename T>
  ErrorOr<size_t> validateTypeAndGetOffset(size_t index) const
  {
    if(index >= this->GetNOperands())
      return oobError(index);

    if(!isValidType<T>(index))
      return castError(index, typeid(T).name());

    size_t offset{0};
    for(size_t i{0}; i < index; ++i)
      offset += this->GetOperandSize(i);

    assert(operandBytes.size() >= offset + this->GetOperandSize(index));

    return offset;
  }
};

} //namespace ClassFile

