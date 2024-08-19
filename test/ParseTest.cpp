#include <gtest/gtest.h>

#include <ClassFile/ClassFile.hpp>
#include <ClassFile/Parser.hpp>
#include <ClassFile/Error.hpp>

#include <fstream>
#include <iostream>

#ifndef RES_DIR
  #define RES_DIR "res"
#endif

class FileParseTest : public ::testing::Test
{
  protected:
    ClassFile::ErrorOr<ClassFile::ClassFile> errOrSimpleClass{
      ClassFile::Error{"ErrOr uninitialized in test"}};

    ClassFile::ErrorOr<ClassFile::ClassFile> errOrComplexClass{
      ClassFile::Error{"ErrOr uninitialized in test"}};

    ClassFile::ErrorOr<ClassFile::ClassFile> errOrWideClass{
      ClassFile::Error{"ErrOr uninitialized in test"}};

    void SetUp() override
    {
      try
      {
        std::ifstream is{RES_DIR"/HelloWorld.class"};
        is.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        errOrSimpleClass = std::move(ClassFile::Parser::ParseClassFile(is));
        is.close();

        is.open(RES_DIR"/Complex.class");
        errOrComplexClass = std::move(ClassFile::Parser::ParseClassFile(is));
        is.close();

        is.open(RES_DIR"/Wide.class");
        errOrWideClass = std::move(ClassFile::Parser::ParseClassFile(is));
        is.close();
      }
      catch(std::exception& e)
      {
        FAIL() << "Setup failed: failed to read test files: \n"
               << e.what();
      }
    }

};

TEST_F(FileParseTest, SuccessfulParseWideInstr)
{
  ASSERT_TRUE( !errOrWideClass.IsError() )
    << "Parsing failed for class with wide instr, error:\n" 
    << errOrWideClass.GetError().What;

  ClassFile::ClassFile cf = errOrWideClass.Release();

  //find code
  ClassFile::CodeAttribute* pCode;
  for(const auto& method : cf.Methods)
  {
    auto codeItr = 
      std::find_if(method.Attributes.begin(), method.Attributes.end(), 
          [](const auto& pAttr) 
          { return pAttr->GetType() == ClassFile::AttributeInfo::Type::Code;});

    pCode = codeItr == method.Attributes.end() ? nullptr :
      static_cast<ClassFile::CodeAttribute*>((*codeItr).get());
  }

  ASSERT_TRUE(pCode != nullptr);

  //find the wide instr
  auto wideInstr = 
    std::find_if(pCode->Code.begin(), pCode->Code.end(), 
        [](const auto& instr){ return instr.IsWide(); });

  ASSERT_TRUE(wideInstr != pCode->Code.end());

  //test the increment constant was successfully parsed as 128
  auto constant = (*wideInstr).GetOperand(1).GetOrElse(0);
  ASSERT_TRUE(constant == 128);
}

TEST_F(FileParseTest, SuccessfulParseSimple)
{
  ASSERT_TRUE( !errOrSimpleClass.IsError() )
    << "Parsing failed for simple valid class, error:\n" 
    << errOrSimpleClass.GetError().What;
}

TEST_F(FileParseTest, SuccessfulParseComplex)
{
  ASSERT_TRUE( !errOrComplexClass.IsError() )
    << "Parsing failed for complex valid class, error:\n" 
    << errOrComplexClass.GetError().What;
}
