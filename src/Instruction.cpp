#include "ClassFile/Instruction.hpp"

#include <fmt/core.h>

#include "Util/IO.hpp"
#include "Util/Error.hpp"

using namespace ClassFile;

ErrorOr<Instruction> Instruction::MakeInstruction(OpCode op)
{
  Instruction instr;
  instr.Op = op;

  size_t totalOperandSize{0};
  for(size_t i{0}; i < ::GetNOperands(op); ++i)
    totalOperandSize += ::GetOperandSize(op, i);

  instr.operandBytes.resize(totalOperandSize);

  return instr;
}

std::string_view Instruction::GetMnemonic() const
{
  return ::GetMnemonic(this->Op);
}

size_t Instruction::GetNOperands() const
{ 
  return ::GetNOperands(this->Op); 
}

OperandType Instruction::GetOperandType(size_t index) const
{
  return ::GetOperandType(this->Op, index);
}

size_t Instruction::GetOperandSize(size_t index) const
{
  return ::GetOperandSize(this->Op, index);
}

size_t Instruction::GetLength() const
{
  return ::GetLength(this->Op);
}

bool Instruction::IsComplex() const
{
  return ::IsComplex(this->Op);
}

template <typename T>
static ErrorOr<S32> verifyGetOpr(size_t index, const Instruction* instr)
{
  auto errOrRef = instr->Operand<T>(index);
  VERIFY(errOrRef);

  return S32{ errOrRef.Get().get() };
}

ErrorOr<S32> Instruction::GetOperand(size_t index) const
{
  switch( this->GetOperandType(index) )
  {
    case 'I': return verifyGetOpr<S32>(index, this);
    case 'S': return verifyGetOpr<S16>(index, this);
    case 'B': return verifyGetOpr<S8 >(index, this);
    case 's': return verifyGetOpr<U16>(index, this);
    case 'b': return verifyGetOpr<U8 >(index, this);

    default: assert(false);
  }
}

template <typename T>
static ErrorOr<void> verifySetOpr(size_t index, S32 value, Instruction* instr)
{
  auto errOrRef = instr->Operand<T>(index);
  VERIFY(errOrRef);

  T& ref = errOrRef.Get().get();
  ref = static_cast<T>(value);

  return NoError{};
}

ErrorOr<void> Instruction::SetOperand(size_t index, S32 value) 
{
  switch( this->GetOperandType(index) )
  {
    case 'I': return verifySetOpr<S32>(index, value, this);
    case 'S': return verifySetOpr<S16>(index, value, this);
    case 'B': return verifySetOpr<S8 >(index, value, this);
    case 's': return verifySetOpr<U16>(index, value, this);
    case 'b': return verifySetOpr<U8 >(index, value, this);

    default: assert(false);
  }
}

Error Instruction::oobError(size_t index) const
{
  return Error{ fmt::format("Instruction: out-of-bounds operand access "
      "at index {}, instruction has {} operand(s).", index, this->GetNOperands())};
}

Error Instruction::castError(size_t index, std::string_view type) const
{
  return Error{ fmt::format("Instruction: invalid operand type cast at index {}, "
      "failed to cast to \"{}\"", index, type)};
}

