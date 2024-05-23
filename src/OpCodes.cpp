#include "ClassFile/OpCodes.hpp"

#include <array>
#include <tuple>
#include <cassert>

using namespace ClassFile;

//                                    mnemonic           format
using opInfo = const std::tuple<std::string_view, std::string_view>;
constexpr std::array<opInfo, ClassFile::OpCode::_N> infoTable = 
{{
  {"nop",            ""},
  {"aconst_null",    ""},
  {"iconst_m1",      ""},
  {"iconst_0",       ""},
  {"iconst_1",       ""},
  {"iconst_2",       ""},
  {"iconst_3",       ""},
  {"iconst_4",       ""},
  {"iconst_5",       ""},
  {"lconst_0",       ""},
  {"lconst_1",       ""},
  {"fconst_0",       ""},
  {"fconst_1",       ""},
  {"fconst_2",       ""},
  {"dconst_0",       ""},
  {"dconst_1",       ""},
  {"bipush",         "B"},
  {"sipush",         "S"},
  {"ldc",            "b"},
  {"ldc_w",          "s"},
  {"ldc2_w",         "s"},
  {"iload",          "b"},
  {"lload",          "b"},
  {"fload",          "b"},
  {"dload",          "b"},
  {"aload",          "b"},
  {"iload_0",        ""},
  {"iload_1",        ""},
  {"iload_2",        ""},
  {"iload_3",        ""},
  {"lload_0",        ""},
  {"lload_1",        ""},
  {"lload_2",        ""},
  {"lload_3",        ""},
  {"fload_0",        ""},
  {"fload_1",        ""},
  {"fload_2",        ""},
  {"fload_3",        ""},
  {"dload_0",        ""},
  {"dload_1",        ""},
  {"dload_2",        ""},
  {"dload_3",        ""},
  {"aload_0",        ""},
  {"aload_1",        ""},
  {"aload_2",        ""},
  {"aload_3",        ""},
  {"iaload",         ""},
  {"laload",         ""},
  {"faload",         ""},
  {"daload",         ""},
  {"aaload",         ""},
  {"baload",         ""},
  {"caload",         ""},
  {"saload",         ""},
  {"istore",         "b"},
  {"lstore",         "b"},
  {"fstore",         "b"},
  {"dstore",         "b"},
  {"astore",         "b"},
  {"istore_0",       ""},
  {"istore_1",       ""},
  {"istore_2",       ""},
  {"istore_3",       ""},
  {"lstore_0",       ""},
  {"lstore_1",       ""},
  {"lstore_2",       ""},
  {"lstore_3",       ""},
  {"fstore_0",       ""},
  {"fstore_1",       ""},
  {"fstore_2",       ""},
  {"fstore_3",       ""},
  {"dstore_0",       ""},
  {"dstore_1",       ""},
  {"dstore_2",       ""},
  {"dstore_3",       ""},
  {"astore_0",       ""},
  {"astore_1",       ""},
  {"astore_2",       ""},
  {"astore_3",       ""},
  {"iastore",        ""},
  {"lastore",        ""},
  {"fastore",        ""},
  {"dastore",        ""},
  {"aastore",        ""},
  {"bastore",        ""},
  {"castore",        ""},
  {"sastore",        ""},
  {"pop",            ""},
  {"pop2",           ""},
  {"dup",            ""},
  {"dup_x1",         ""},
  {"dup_x2",         ""},
  {"dup2",           ""},
  {"dup2_x1",        ""},
  {"dup2_x2",        ""},
  {"swap",           ""},
  {"iadd",           ""},
  {"ladd",           ""},
  {"fadd",           ""},
  {"dadd",           ""},
  {"isub",           ""},
  {"lsub",           ""},
  {"fsub",           ""},
  {"dsub",           ""},
  {"imul",           ""},
  {"lmul",           ""},
  {"fmul",           ""},
  {"dmul",           ""},
  {"idiv",           ""},
  {"ldiv",           ""},
  {"fdiv",           ""},
  {"ddiv",           ""},
  {"irem",           ""},
  {"lrem",           ""},
  {"frem",           ""},
  {"drem",           ""},
  {"ineg",           ""},
  {"lneg",           ""},
  {"fneg",           ""},
  {"dneg",           ""},
  {"ishl",           ""},
  {"lshl",           ""},
  {"ishr",           ""},
  {"lshr",           ""},
  {"iushr",          ""},
  {"lushr",          ""},
  {"iand",           ""},
  {"land",           ""},
  {"ior",            ""},
  {"lor",            ""},
  {"ixor",           ""},
  {"lxor",           ""},
  {"iinc",           "sS"},
  {"i2l",            ""},
  {"i2f",            ""},
  {"i2d",            ""},
  {"l2i",            ""},
  {"l2f",            ""},
  {"l2d",            ""},
  {"f2i",            ""},
  {"f2l",            ""},
  {"f2d",            ""},
  {"d2i",            ""},
  {"d2l",            ""},
  {"d2f",            ""},
  {"i2b",            ""},
  {"i2c",            ""},
  {"i2s",            ""},
  {"lcmp",           ""},
  {"fcmpl",          ""},
  {"fcmpg",          ""},
  {"dcmpl",          ""},
  {"dcmpg",          ""},
  {"ifeq",           "S"},
  {"ifne",           "S"},
  {"iflt",           "S"},
  {"ifge",           "S"},
  {"ifgt",           "S"},
  {"ifle",           "S"},
  {"if_icmpeq",      "S"},
  {"if_icmpne",      "S"},
  {"if_icmplt",      "S"},
  {"if_icmpge",      "S"},
  {"if_icmpgt",      "S"},
  {"if_icmple",      "S"},
  {"if_acmpeq",      "S"},
  {"if_acmpne",      "S"},
  {"goto",           "S"},
  {"jsr",            "S"},
  {"ret",            "b"},
  {"tableswitch",    "c"}, //complex
  {"lookupswitch",   "c"}, //complex
  {"ireturn",        ""},
  {"lreturn",        ""},
  {"freturn",        ""},
  {"dreturn",        ""},
  {"areturn",        ""},
  {"return",         ""},
  {"getstatic",      "s"},
  {"putstatic",      "s"},
  {"getfield",       "s"},
  {"putfield",       "s"},
  {"invokevirtual",  "s"},
  {"invokespecial",  "s"},
  {"invokestatic",   "s"},
  {"invokeinterface","sbb"},
  {"invokedynamic",  "sbb"},
  {"new",            "s"},
  {"newarray",       "b"},
  {"anewarray",      "s"},
  {"arraylength",    ""},
  {"athrow",         ""},
  {"checkcast",      "s"},
  {"instanceof",     "s"},
  {"monitorenter",   ""},
  {"monitorexit",    ""},
  {"wide",           "c"}, //TODO
  {"multianewarray", "sb"},
  {"ifnull",         "s"},
  {"ifnonnull",      "s"},
  {"goto_w",         "I"},
  {"jsr_w",          "I"},
  {"breakpoint",     ""},
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

static constexpr std::string_view mnemonic(OpCode op)
{
  assert(static_cast<int>(op) < OpCode::_N);
  return std::get<0>(infoTable[op]);
}

static constexpr std::string_view format(OpCode op)
{
  assert(static_cast<int>(op) < OpCode::_N);
  return std::get<1>(infoTable[op]);
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
  assert( format(op).empty()       ||
          format(op)[index] == 'I' ||
          format(op)[index] == 'S' ||
          format(op)[index] == 'B' ||
          format(op)[index] == 's' ||
          format(op)[index] == 'b'   );

  return static_cast<OperandType>(format(op)[index]);
}

size_t ClassFile::GetOperandSize(OpCode op, size_t index) 
{
  size_t size;

  switch( GetOperandType(op, index) )
  {
    case 'I': size = sizeof(S32); break;
    case 'S': size = sizeof(S16); break;
    case 'B': size = sizeof(S8);  break;
    case 's': size = sizeof(U16); break;
    case 'b': size = sizeof(U8);  break;
  }

  return size;
}

size_t ClassFile::GetLength(OpCode op)
{
  assert(!IsComplex(op));

  //opcode is 1 byte
  size_t len{1};

  for(size_t i{0}; i < GetNOperands(op); ++i)
  {
    switch( GetOperandType(op, i) )
    {
      case 'I': len+=sizeof(S32); break;
      case 'S': len+=sizeof(S16); break;
      case 'B': len+=sizeof(S8);  break;
      case 's': len+=sizeof(U16); break;
      case 'b': len+=sizeof(U8);  break;
    }
  }

  return len;
}

bool ClassFile::IsComplex(OpCode op) 
{
  return format(op)[0] == 'c';
}


