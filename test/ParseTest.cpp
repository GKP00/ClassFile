#include <gtest/gtest.h>

#include <ClassFile/ClassFile.hpp>
#include <ClassFile/Parser.hpp>
#include <ClassFile/Error.hpp>

#include <fstream>
#include <iostream>

class FileParseTest : public ::testing::Test
{
  protected:
    ClassFile::ErrorOr<ClassFile::ClassFile> errOrSimpleClass{
      ClassFile::Error{"ErrOr uninitialized in test"}};

    void SetUp() override
    {
      #ifdef RES_DIR
      const char* filePath = RES_DIR"/HelloWorld.class";
      #else
      const char* filePath = "./res/HelloWorld.class";
      #endif

      try
      {
        std::ifstream is{filePath};
        is.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        errOrSimpleClass = std::move(ClassFile::Parser::ParseClassFile(is));
      }
      catch(std::exception& e)
      {
        FAIL() << "stream threw while reading from \"" 
               << filePath
               << "\" (probably failed to open). Exception.what(): " 
               << e.what();
      }
    }

};

TEST_F(FileParseTest, SuccessfulParse)
{
  ASSERT_TRUE( !errOrSimpleClass.IsError() )
    << "Parsing failed for simple valid class, error:\n" 
    << errOrSimpleClass.GetError().What;
}
