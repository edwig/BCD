#include "stdafx.h"
#include "CppUnitTest.h"
#include "numeric.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
  TEST_CLASS(UnitTestNumeric)
  {
  public:

    TEST_METHOD(T001_Constructor01)
    {
      Logger::WriteMessage("Construct numeric");
      numeric one;
      Assert::IsTrue(one.IsNull());
    }

    TEST_METHOD(T002_Constructor02_string)
    {
      Logger::WriteMessage("Construct numeric from string");
      numeric one(6,4,"10.001");

      SQL_NUMERIC_STRUCT num;
      one.AsNumeric(&num);

      // Check the filling
      Assert::AreEqual(num.sign,     (unsigned char) 1);
      Assert::AreEqual(num.precision,(unsigned char) 6);
      Assert::AreEqual(num.scale,    (SQLSCHAR)      4);
      Assert::AreEqual(num.val[0],   (SQLCHAR)    0xAA);
      Assert::AreEqual(num.val[1],   (SQLCHAR)    0x86);
      Assert::AreEqual(num.val[2],   (SQLCHAR)    0x01);
    }

    TEST_METHOD(T003_Constructor03_string)
    {
      Logger::WriteMessage("Construct numeric from string");
      numeric one(6,4,"10.0000");

      SQL_NUMERIC_STRUCT num;
      one.AsNumeric(&num);

      // Check the filling
      Assert::AreEqual(num.sign,     (unsigned char) 1);
      Assert::AreEqual(num.precision,(unsigned char) 6);
      Assert::AreEqual(num.scale,    (SQLSCHAR)      4);
      Assert::AreEqual(num.val[0],   (SQLCHAR)    0xA0);
      Assert::AreEqual(num.val[1],   (SQLCHAR)    0x86);
      Assert::AreEqual(num.val[2],   (SQLCHAR)    0x01);
    }

    TEST_METHOD(T004_Constructor04_string)
    {
      Logger::WriteMessage("Construct numeric from string");
      numeric one(6,4,"10");

      SQL_NUMERIC_STRUCT num;
      one.AsNumeric(&num);

      // Check the filling
      Assert::AreEqual(num.sign,     (unsigned char) 1);
      Assert::AreEqual(num.precision,(unsigned char) 6);
      Assert::AreEqual(num.scale,    (SQLSCHAR)      4);
      Assert::AreEqual(num.val[0],   (SQLCHAR)    0xA0);
      Assert::AreEqual(num.val[1],   (SQLCHAR)    0x86);
      Assert::AreEqual(num.val[2],   (SQLCHAR)    0x01);
    }

    TEST_METHOD(T005_Constructor05_string)
    {
      Logger::WriteMessage("Construct numeric from string");
      numeric one(6,4,"10.00100000");

      SQL_NUMERIC_STRUCT num;
      one.AsNumeric(&num);

      // Check the filling
      Assert::AreEqual(num.sign,     (unsigned char) 1);
      Assert::AreEqual(num.precision,(unsigned char) 6);
      Assert::AreEqual(num.scale,    (SQLSCHAR)      4);
      Assert::AreEqual(num.val[0],   (SQLCHAR)    0xAA);
      Assert::AreEqual(num.val[1],   (SQLCHAR)    0x86);
      Assert::AreEqual(num.val[2],   (SQLCHAR)    0x01);
    }

    TEST_METHOD(T006_Constructor06_string)
    {
      Logger::WriteMessage("Construct numeric from string");
      numeric one(6, 4, "10.00100000");

      SQL_NUMERIC_STRUCT num;
      one.AsNumeric(&num);

      // Check the filling
      CString check = one.AsString();
      Assert::AreEqual(check, "10.0010");
    }

    TEST_METHOD(T07_Addition)
    {
      Logger::WriteMessage("Adding two numerics");
      numeric one(6, 4, "10.00100000");
      numeric two(6, 4, "20.00300000");

      numeric three = one + two;

      // Check the filling
      CString check = three.AsString();
      Assert::AreEqual(check, "30.0040");
    }

    TEST_METHOD(T08_Constant_PI)
    {
      Logger::WriteMessage("Getting the constant of PI");
      numeric pi = numeric::PI(32,31);
      
      // Check the constant
      // +31415926_53589793_23846264_33832795_02884197
      CString check = pi.AsString();
      Assert::AreEqual(check, "3.1415926535897932384626433832795");

      numeric pi2 = numeric::PI(16,15);
      check = pi2.AsString();
      Assert::AreEqual(check, "3.141592653589793");
    }

    TEST_METHOD(T09_Constant_LN2)
    {
      Logger::WriteMessage("Getting the constant LN2");
      numeric ln2 = numeric::LN2(32, 31);

      // Check the constant
      // +0.69314718_05599453_09417232_12145817_65680755
      CString check = ln2.AsString();
      Assert::AreEqual(check, "0.6931471805599453094172321214581");

      numeric ln22 = numeric::LN2(16, 15);
      check = ln22.AsString();
      Assert::AreEqual(check, "0.693147180559945");
    }

    TEST_METHOD(T10_Constant_LN10)
    {
      Logger::WriteMessage("Getting the constant LN10");
      numeric ln10 = numeric::LN10(32, 31);

      // Check the constant
      // +2.3025850_92994045_68401799_14546843_64207601
      CString check = ln10.AsString();
      Assert::AreEqual(check,"2.3025850929940456840179914546843");

      numeric ln102 = numeric::LN10(16, 15);
      check = ln102.AsString();
      Assert::AreEqual(check, "2.302585092994045");
    }
  };
}
