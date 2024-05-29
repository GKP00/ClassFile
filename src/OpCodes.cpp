#include "ClassFile/OpCodes.hpp"

#include <array>
#include <tuple>
#include <cassert>

using namespace ClassFile;

struct opInfo
{
  const std::string_view Mnemonic;
  const std::string_view OperandFormat;
  const std::string_view WideOperandFormat;

  constexpr opInfo(const std::string_view mnemonic, 
                   const std::string_view fmt  = "",
                   const std::string_view wFmt = "")
    : Mnemonic{mnemonic}, OperandFormat{fmt}, WideOperandFormat{wFmt} {}
};

constexpr std::array<opInfo, ClassFile::OpCode::_N> infoTable = 
{{
  {"nop"},
  {"aconst_null"},
  {"iconst_m1"},
  {"iconst_0"},{"iconst_1"},{"iconst_2"},{"iconst_3"},{"iconst_4"},{"iconst_5"},
  {"lconst_0"},{"lconst_1"},
  {"fconst_0"},{"fconst_1"},{"fconst_2"},
  {"dconst_0"},{"dconst_1"},

  {"bipush", "B"},
  {"sipush", "S"},
  {"ldc",    "b"},
  {"ldc_w",  "s"},
  {"ldc2_w", "s"},
  {"iload",  "b", "s"},
  {"lload",  "b", "s"},
  {"fload",  "b", "s"},
  {"dload",  "b", "s"},
  {"aload",  "b", "s"},

  {"iload_0"},{"iload_1"},{"iload_2"},{"iload_3"},
  {"lload_0"},{"lload_1"},{"lload_2"},{"lload_3"},
  {"fload_0"}, {"fload_1"},{"fload_2"},{"fload_3"},
  {"dload_0"},{"dload_1"},{"dload_2"},{"dload_3"},
  {"aload_0"},{"aload_1"},{"aload_2"},{"aload_3"},
  {"iaload"},{"laload"},{"faload"},{"daload"},{"aaload"},{"baload"},{"caload"},{"saload"},

  {"istore", "b", "s"},
  {"lstore", "b", "s"},
  {"fstore", "b", "s"},
  {"dstore", "b", "s"},
  {"astore", "b", "s"},

  {"istore_0"},{"istore_1"},{"istore_2"},{"istore_3"},
  {"lstore_0"},{"lstore_1"},{"lstore_2"},{"lstore_3"},
  {"fstore_0"},{"fstore_1"},{"fstore_2"},{"fstore_3"},
  {"dstore_0"},{"dstore_1"},{"dstore_2"},{"dstore_3"},
  {"astore_0"},{"astore_1"},{"astore_2"},{"astore_3"},
  {"iastore"},{"lastore"},{"fastore"},{"dastore"},{"aastore"},{"bastore"},{"castore"},{"sastore"},

  {"pop"},{"pop2"},
  {"dup"},{"dup_x1"},{"dup_x2"},{"dup2"},{"dup2_x1"},{"dup2_x2"},

  {"swap"},

  {"iadd"},{"ladd"},{"fadd"},{"dadd"},
  {"isub"},{"lsub"},{"fsub"},{"dsub"},
  {"imul"},{"lmul"},{"fmul"},{"dmul"},
  {"idiv"},{"ldiv"},{"fdiv"},{"ddiv"},
  {"irem"},{"lrem"},{"frem"},{"drem"},
  {"ineg"},{"lneg"},{"fneg"},{"dneg"},

  {"ishl"},{"lshl"},
  {"ishr"},{"lshr"},
  {"iushr"},{"lushr"},
  {"iand"},{"land"},
  {"ior"},{"lor"},
  {"ixor"},{"lxor"},

  {"iinc", "bB", "sS"},

  {"i2l"},{"i2f"},{"i2d"},
  {"l2i"},{"l2f"},{"l2d"},
  {"f2i"},{"f2l"},{"f2d"},
  {"d2i"},{"d2l"},{"d2f"},
  {"i2b"},{"i2c"},{"i2s"},

  {"lcmp"},{"fcmpl"},{"fcmpg"},
  {"dcmpl"},{"dcmpg"},

  {"ifeq", "S"},
  {"ifne", "S"},
  {"iflt", "S"},
  {"ifge", "S"},
  {"ifgt", "S"},
  {"ifle", "S"},

  {"if_icmpeq", "S"},
  {"if_icmpne", "S"},
  {"if_icmplt", "S"},
  {"if_icmpge", "S"},
  {"if_icmpgt", "S"},
  {"if_icmple", "S"},
  {"if_acmpeq", "S"},
  {"if_acmpne", "S"},

  {"goto", "S"},
  {"jsr",  "S"},
  {"ret",  "b", "s"},

  {"tableswitch",  "c"}, //TODO
  {"lookupswitch", "c"}, //TODO

  {"ireturn"},{"lreturn"},{"freturn"},{"dreturn"},{"areturn"},{"return"},

  {"getstatic", "s"},
  {"putstatic", "s"},
  {"getfield",  "s"},
  {"putfield",  "s"},

  {"invokevirtual", "s"},
  {"invokespecial", "s"},
  {"invokestatic",  "s"},
  {"invokeinterface","sbb"},
  {"invokedynamic",  "sbb"},

  {"new",       "s"},
  {"newarray",  "b"},
  {"anewarray", "s"},
  {"arraylength"},

  {"athrow"},

  {"checkcast",  "s"},
  {"instanceof", "s"},

  {"monitorenter"},
  {"monitorexit"},

  {"wide", ""}, 

  {"multianewarray", "sb"},
  {"ifnull",    "s"},
  {"ifnonnull", "s"},
  {"goto_w", "I"},
  {"jsr_w",  "I"},
  {"breakpoint"},
 }};

std::string_view ClassFile::ToString(OperandType opType)
{
  switch(opType)
  {
    case TypeS32: return "S32";
    case TypeS16: return "S16";
    case TypeS8:  return  "S8";
    case TypeU16: return "U16";
    case TypeU8:  return  "U8";
  }

  assert(false);
  return {};
}

std::ostream& ClassFile::operator<<(std::ostream& s, OperandType type)
{
  s << ToString(type);
  return s;
}

static void assertValidFormatType(const char c)
{
  assert( c == 'I' ||
          c == 'S' ||
          c == 'B' ||
          c == 's' ||
          c == 'b'   );
}

static void assertValidFormatIndex(std::string_view format, size_t index)
{
  assert(!format.empty());
  assert(index < format.size());

  assertValidFormatType(format[index]);
}

static constexpr std::string_view mnemonic(OpCode op)
{
  return infoTable[op].Mnemonic;
}

static constexpr std::string_view format(OpCode op)
{
  return infoTable[op].OperandFormat;
}

static constexpr std::string_view wideformat(OpCode op)
{
  return infoTable[op].WideOperandFormat;
}


std::string_view ClassFile::GetMnemonic(OpCode op)
{
  return mnemonic(op);
}

size_t ClassFile::GetNOperands(OpCode op)
{
  return format(op).size();
}

OperandType ClassFile::GetOperandType(OpCode op, size_t index) 
{
  assertValidFormatIndex(wideformat(op), index);
  return static_cast<OperandType>(format(op)[index]);
}

size_t ClassFile::GetOperandSize(OpCode op, size_t index) 
{
  return GetOperandSize(GetOperandType(op, index));
}

size_t ClassFile::GetOperandSize(OperandType type)
{
  switch( type )
  {
    case 'I': return sizeof(S32); 
    case 'S': return sizeof(S16);
    case 'B': return sizeof(S8);
    case 's': return sizeof(U16);
    case 'b': return sizeof(U8);
  }

  assert(false);
  return {};
}

size_t ClassFile::GetLength(OpCode op)
{
  assert(!IsComplex(op));

  //opcode is 1 byte
  size_t len{1};

  for(size_t i{0}; i < GetNOperands(op); ++i)
    len += GetOperandSize(op, i);

  return len;
}

OperandType ClassFile::GetWideOperandType(OpCode op, size_t index) 
{
  assertValidFormatIndex(wideformat(op), index);
  return static_cast<OperandType>(wideformat(op)[index]);
}

size_t ClassFile::GetWideOperandSize(OpCode op, size_t index) 
{
  return GetOperandSize( GetWideOperandType(op, index) );
}

size_t ClassFile::GetWideLength(OpCode op)
{
  assert(!IsComplex(op));

  //opcode + wide opcode are 2 bytes
  size_t len{2};

  for(size_t i{0}; i < GetNOperands(op); ++i)
    len += GetWideOperandSize(op, i);

  return len;
}

bool ClassFile::IsComplex(OpCode op) 
{
  return format(op)[0] == 'c';
}


