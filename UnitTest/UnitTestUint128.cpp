#include "stdafx.h"
#include "CppUnitTest.h"
#include "uint128.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
  TEST_CLASS(UnitTestUint128)
  {
  public:

    TEST_METHOD(T001_Constructor01)
    {
      Logger::WriteMessage("Construct uint128");
      uint128 one;
      Assert::IsTrue(one.IsNull());
    }

    TEST_METHOD(T002_Constructor02)
    {
      Logger::WriteMessage("Construct uint128 from an 1 byte number");

      uint128 one('\n');
      int expect(10);

      Assert::AreEqual(expect, one.AsInteger());
    }

    TEST_METHOD(T003_Constructor03)
    {
      Logger::WriteMessage("Construct a uint128 from a short int");

      short expect(12);
      uint128  one(expect);

      Assert::AreEqual((int)expect, one.AsInteger());
    }

    TEST_METHOD(T004_Constructor04)
    {
      Logger::WriteMessage("Construct a uint128 from a long");

      long expect(765432198);
      uint128 one(expect);

      Assert::AreEqual((int)expect, one.AsInteger());
    }

    TEST_METHOD(T005_Constructor05)
    {
      Logger::WriteMessage("Construct a bcd from an uint64");

      uint64 expect(76543219876543210LL);
      uint128  one("76543219876543210");
      uint64 result = one.AsUInt64();

      Assert::IsTrue(result - expect == 0);
    }

    TEST_METHOD(T006_Constructor06)
    {
      Logger::WriteMessage("Construct an uint128 from another uint128");

      uint128 one(12345);
      uint128 two(one);

      Assert::IsTrue(one.AsInteger() - two.AsInteger() == 0);
    }

    TEST_METHOD(T007_Constructor07)
    {
      Logger::WriteMessage("Construct a bcd from a float");

      float expect = static_cast<float>(765432198);
      uint128  one(expect);
      float result = (float) one.AsInteger();
      expect = trunc(expect * 10) / 10;
      result = trunc(result * 10) / 10;

      Assert::AreEqual(expect, result);
    }

    TEST_METHOD(T008_Constructor08)
    {
      Logger::WriteMessage("Construct a bcd from a double");

      double expect = static_cast<double>(765432198);
      uint128  one(expect);
      double result = one.AsInteger();
      expect = trunc(expect * 10) / 10;
      result = trunc(result * 10) / 10;

      Assert::AreEqual(expect, result);
    }

    TEST_METHOD(T009_Constructor09)
    {
      Logger::WriteMessage("Construct a bcd from a std::string");

      CString large("76543219876543210");
      uint64 expect(76543219876543210LL);
      uint128  one(large);
      uint64 result = one.AsUInt64();

      Assert::IsTrue(result - expect == 0);
    }

    TEST_METHOD(T010_Constant_PI)
    {
      Logger::WriteMessage("Test the constant of PI");
      uint128 pi("31415926535897932384626433832795028841");
      std::string pi_string = pi.AsString();
      std::string expect("31415926535897932384626433832795028841");

      Assert::AreEqual(expect.c_str(), pi_string.c_str());
    }

    TEST_METHOD(T011_Operator_plus)
    {
      Logger::WriteMessage("Test operator +");

      uint128 one("5678902345678901123456789023456");
      uint128 two("7766554433220011998877665544311");
      uint128 result(one);
      result += two;
      string resstring = result.AsString();
      char* expect = "13445456778898913122334454567767";

      Assert::IsTrue(strcmp(expect, resstring.c_str()) == 0);
    }
    TEST_METHOD(T012_Operator_min)
    {
      Logger::WriteMessage("Test operator -");

      uint128 one("7766554433220011998877665544311");
      uint128 two("5678902345678901123456789023456");
      uint128 result(one);
      result -= two;
      string resstring = result.AsString();
      char* expect = "2087652087541110875420876520855";

      Assert::IsTrue(strcmp(expect, resstring.c_str()) == 0);
    }

    TEST_METHOD(T013_Operator_multiplication)
    {
      Logger::WriteMessage("Test operator *");

      uint128 one("1234567890123456789");
      uint128 two("9876543210987654321");
      uint128 result(one);
      result *= two;
      string resstring = result.AsString();
      char* expect = "12193263113702179522374638011112635269";

      Assert::IsTrue(strcmp(expect, resstring.c_str()) == 0);
    }

    TEST_METHOD(T014_Operator_division)
    {
      Logger::WriteMessage("Test operator /");

      uint128 one("12193263113702179522374638011112635269");
      uint128 two("9876543210987654321");
      uint128 result(one);
      result /= two;
      string resstring = result.AsString();
      char* expect = "1234567890123456789";

      Assert::IsTrue(strcmp(expect, resstring.c_str()) == 0);
    }

    TEST_METHOD(T015_SQL_NUMERIC_STRUCT)
    {
      Logger::WriteMessage("Test SQL_NUMERIC_STRUCT by assigning and converting to");

      // num = 10.001 (ten and 1 thousandth)
      SQL_NUMERIC_STRUCT num;
      SQL_NUMERIC_STRUCT res;
      memset(&num, 0, sizeof(SQL_NUMERIC_STRUCT));
      memset(&res, 0, sizeof(SQL_NUMERIC_STRUCT));

      // Filling NUM
      num.sign      = 1; // Positive
      num.precision = 6;
      num.scale     = 4;
      num.val[0] = 0xAA;  // 1010 1010
      num.val[1] = 0x86;  // 1000 0110
      num.val[2] = 0x01;  // 0000 0001

      uint128 one(&num);

      // Getting it back
      res.sign      = 1; // Positive
      res.precision = 6;
      res.scale     = 4;
      one.AsNumeric(&res);

      string check1 = one.AsString();
      
      Assert::IsTrue(memcmp(num.val,res.val,SQL_MAX_NUMERIC_LEN) == 0);
      Assert::IsTrue(check1 == "100010");
    }

  };
}