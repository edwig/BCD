#include "stdafx.h"
#include "CppUnitTest.h"
#include "bcd.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
    TEST_METHOD(T001_Constructor01)
    {
      Logger::WriteMessage("Construct bcd");
      bcd one;
      Assert::IsTrue(one.IsNull());
    }

		TEST_METHOD(T002_Constructor02)
		{
      Logger::WriteMessage("Construct bcd from an 1 byte number");

      bcd  one('\n');
      long expect(10);

      Assert::AreEqual(expect,one.AsLong());
		}

    TEST_METHOD(T003_Constructor03)
    {
      Logger::WriteMessage("Construct a bcd from a short int");

      short expect(12);
      bcd   one(expect);

      Assert::AreEqual((long)expect,one.AsLong());
    }

    TEST_METHOD(T004_Constructor04)
    {
      Logger::WriteMessage("Construct a bcd from a long");

      long expect(765432198);
      bcd  one(expect);

      Assert::AreEqual((long)expect,one.AsLong());
    }

    TEST_METHOD(T005_Constructor05)
    {
      Logger::WriteMessage("Construct a bcd from a long + remainder");

      long base(765432198);
      long remainder(50000000);
      bcd  one(base,remainder);
      double expect = 765432198.5;
      double result = one.AsDouble();
      expect = trunc(expect * 10) / 10;
      result = trunc(result * 10) / 10;

      Assert::AreEqual(expect,result);
    }

    TEST_METHOD(T006_Constructor06)
    {
      Logger::WriteMessage("Construct a bcd from an __int64");

      __int64 expect(76543219876543210LL);
      bcd  one(expect);
      __int64 result = one.AsInt64();

      Assert::IsTrue(result - expect == 0);
    }

    TEST_METHOD(T007_Constructor07)
    {
      Logger::WriteMessage("Construct a bcd from an unsigned __int64");

      unsigned __int64 expect(76543219876543210LL);
      bcd  one("76543219876543210");
      unsigned __int64 result = one.AsUInt64();

      Assert::IsTrue(result - expect == 0);
    }

    TEST_METHOD(T008_Constructor08)
    {
      Logger::WriteMessage("Construct a bcd from another bcd");

      bcd one(12345);
      bcd two(one);

      Assert::IsTrue(one.AsLong() - two.AsLong() == 0);
    }

    TEST_METHOD(T009_Constructor09)
    {
      Logger::WriteMessage("Construct a bcd from a double");

      double expect = 765432198.5;
      bcd  one(expect);
      double result = one.AsDouble();
      expect = trunc(expect * 10) / 10;
      result = trunc(result * 10) / 10;

      Assert::AreEqual(expect,result);
    }

    TEST_METHOD(T010_Constructor10)
    {
      Logger::WriteMessage("Construct a bcd from a CString");

      CString large("76543219876543210");
      unsigned __int64 expect(76543219876543210LL);
      bcd  one(large);
      unsigned __int64 result = one.AsUInt64();

      Assert::IsTrue(result - expect == 0);
    }

    TEST_METHOD(T011_Constant_PI)
    {
      Logger::WriteMessage("Test the constant of PI");
      bcd pi = bcd::PI();
      CString pi_string = pi.AsString();
      CString expect("3.141592653589793238462643383279502884197");

      Assert::AreEqual(expect.GetString(),pi_string.GetString());
    }

    TEST_METHOD(T012_Constant_LN2)
    {
      Logger::WriteMessage("Test the constant LN2");

      bcd ln2 = bcd::LN2();
      CString ln2_string = ln2.AsString();
      CString expect("0.6931471805599453094172321214581765680755");

      Assert::AreEqual(expect.GetString(),ln2_string.GetString());
    }

    TEST_METHOD(T013_Constant_LN10)
    {
      Logger::WriteMessage("Test the constant LN10");

      bcd ln10 = bcd::LN10();
      CString ln10_string = ln10.AsString();
      CString expect("2.302585092994045684017991454684364207601");

      Assert::AreEqual(expect.GetString(),ln10_string.GetString());
    }

    TEST_METHOD(T014_Operator_plus)
    {
      Logger::WriteMessage("Test operator +");

      bcd one("123456.789012345678");
      bcd two("9988776655.4433221100");
      bcd result = one + two;
      CString resstring = result.AsString();
      char* expect = "9988900112.232334455678";

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T015_Operator_min)
    {
      Logger::WriteMessage("Test operator -");

      // 123456,789012345678
      // 9988776655,4433221100
      bcd one("123456.789012345678");
      bcd two("9988776655.4433221100");
      bcd result = one - two;
      CString resstring = result.AsString();
      char* expect = "-9988653198.654309764322";

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T016_Operator_multiply)
    {
      Logger::WriteMessage("Test operator *");

      // 123456,789012345678
      // 9988776655,4433221100
      bcd one("123456.789012345678");
      bcd two("9988776655.4433221100");
      bcd result = one * two;
      CString resstring = result.AsString();
      char* expect = "1233182292042510.13939532499202034058";

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T017_Operator_divide)
    {
      Logger::WriteMessage("Test operator /");

      // 123456,789012345678
      // 9988776655,4433221100
      bcd one("123456.789012345678");
      bcd two("9988776655.4433221100");
      bcd result = one / two;
      CString resstring = result.AsString(bcd::Format::Engineering);
      char* expect = "1.235955045056179765144678702196566090151E-5";

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T018_Operator_modulo)
    {
      Logger::WriteMessage("Test operator %");

      // 123456,789012345678
      // 9988776655,4433221100
      bcd one("123456.789012345678");
      bcd two("9988776655.4433221100");
      bcd result = two % one;
      CString resstring = result.AsString();
      char* expect = "11313.243445648698";

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T019_Operator_assign_plus)
    {
      Logger::WriteMessage("Test operator +=");

      bcd one("123456.789012345678");
      bcd two("9988776655.4433221100");
      one += two;
      CString resstring = one.AsString();
      char* expect = "9988900112.232334455678";

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T020_Operator_assign_min)
    {
      Logger::WriteMessage("Test operator -=");

      // 123456,789012345678
      // 9988776655,4433221100
      bcd one("123456.789012345678");
      bcd two("9988776655.4433221100");
      one -= two;
      CString resstring = one.AsString();
      char* expect = "-9988653198.654309764322";

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T021_Operator_assign_multiply)
    {
      Logger::WriteMessage("Test operator *=");

      // 123456,789012345678
      // 9988776655,4433221100
      bcd one("123456.789012345678");
      bcd two("9988776655.4433221100");
      one *= two;
      CString resstring = one.AsString();
      char* expect = "1233182292042510.13939532499202034058";

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }
 
    TEST_METHOD(T022_Operator_assing_divide)
    {
      Logger::WriteMessage("Test operator /=");

      // 123456,789012345678
      // 9988776655,4433221100
      bcd one("123456.789012345678");
      bcd two("9988776655.4433221100");
      one /= two;
      CString resstring = one.AsString(bcd::Format::Engineering);
      char* expect = "1.235955045056179765144678702196566090151E-5";

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }
 
    TEST_METHOD(T023_Operator_assign_modulo)
    {
      Logger::WriteMessage("Test operator %=");

      // 123456,789012345678
      // 9988776655,4433221100
      bcd one("123456.789012345678");
      bcd two("9988776655.4433221100");
      two %= one;
      CString resstring = two.AsString();
      char* expect = "11313.243445648698";

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T024_Operator_negate)
    {
      Logger::WriteMessage("Test operator unary - (negate)");

      // 123456,789012345678
      // 9988776655,4433221100
      bcd one("123456.789012345678");
      bcd two = -one;
      CString resstring = two.AsString();
      char* expect = "-123456.789012345678";

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T025_Operator_postfix_increment)
    {
      Logger::WriteMessage("Test operator bcd++ (postfix increment)");

      bcd one("1234567890123456789");
      bcd two = one++;
      CString resstring1 = one.AsString();
      CString resstring2 = two.AsString();
      char* expect1 = "1234567890123456790";
      char* expect2 = "1234567890123456789";

      Assert::IsTrue(strcmp(expect1,resstring1.GetString()) == 0);
      Assert::IsTrue(strcmp(expect2,resstring2.GetString()) == 0);
    }

    TEST_METHOD(T026_Operator_prefix_increment)
    {
      Logger::WriteMessage("Test operator ++bcd (prefix increment)");

      bcd one("1234567890123456789");
      bcd two = ++one;
      CString resstring1 = one.AsString();
      CString resstring2 = two.AsString();
      char* expect1 = "1234567890123456790";
      char* expect2 = "1234567890123456790";

      Assert::IsTrue(strcmp(expect1,resstring1.GetString()) == 0);
      Assert::IsTrue(strcmp(expect2,resstring2.GetString()) == 0);
    }

    TEST_METHOD(T027_Operator_postfix_decrement)
    {
      Logger::WriteMessage("Test operator bcd-- (postfix decrement)");

      bcd one("1234567890123456790");
      bcd two = one--;
      CString resstring1 = one.AsString();
      CString resstring2 = two.AsString();
      char* expect1 = "1234567890123456789";
      char* expect2 = "1234567890123456790";

      Assert::IsTrue(strcmp(expect1,resstring1.GetString()) == 0);
      Assert::IsTrue(strcmp(expect2,resstring2.GetString()) == 0);
    }

    TEST_METHOD(T028_Operator_prefix_decrement)
    {
      Logger::WriteMessage("Test operator bcd-- (postfix decrement)");

      bcd one("1234567890123456790");
      bcd two = --one;
      CString resstring1 = one.AsString();
      CString resstring2 = two.AsString();
      char* expect1 = "1234567890123456789";
      char* expect2 = "1234567890123456789";

      Assert::IsTrue(strcmp(expect1,resstring1.GetString()) == 0);
      Assert::IsTrue(strcmp(expect2,resstring2.GetString()) == 0);
    }

    TEST_METHOD(T029_Operator_assign_bcd)
    {
      Logger::WriteMessage("Test operator bcd = bcd (assignment operator)");

      bcd one("123456.789012345678");
      bcd two = one;
      CString resstring = two.AsString();
      char* expect = "123456.789012345678";

      Assert::AreEqual(expect,resstring.GetString());
    }

    TEST_METHOD(T030_Operator_assign_long)
    {
      Logger::WriteMessage("Test operator bcd = long (assignment operator)");

      long one = 123456789;
      bcd two = one;
      CString resstring = two.AsString();
      char* expect = "123456789";

      Assert::AreEqual(expect,resstring.GetString());
    }

    TEST_METHOD(T031_Operator_assign_double)
    {
      Logger::WriteMessage("Test operator bcd = double (assignment operator)");

      double one = 123456.123456789;
      bcd two = one;
      CString resstring = two.AsString();
      char* expect = "123456.123456789";

      Assert::AreEqual(expect,resstring.GetString());
    }

    TEST_METHOD(T032_Operator_assign_CString)
    {
      Logger::WriteMessage("Test operator bcd = CString (assignment operator)");

      CString one("123456.123456789");
      bcd two = one;
      CString resstring = two.AsString();
      char* expect = "123456.123456789";

      Assert::AreEqual(expect,resstring.GetString());
    }

    TEST_METHOD(T033_Operator_comparison_equal)
    {
      Logger::WriteMessage("Test comparison bcd == bcd");
      Logger::WriteMessage("Test comparison bcd >= bcd");
      Logger::WriteMessage("Test comparison bcd <= bcd");

      CString number("8765432123456.123456789");
      bcd one(number);
      bcd two(number);

      Assert::IsTrue(one == two);
      Assert::IsTrue(one <= two);
      Assert::IsTrue(one >= two);
    }

    TEST_METHOD(T034_Operator_comparison_notequal)
    {
      Logger::WriteMessage("Test comparison bcd != bcd");

      CString number1("8765432123456.123456789");
      CString number2("8765432123456.123476789");
      bcd one(number1);
      bcd two(number2);

      Assert::IsTrue(one != two);
    }

    TEST_METHOD(T035_Operator_comparison_smaller)
    {
      Logger::WriteMessage("Test comparison bcd < bcd");

      CString number1("8765432123456.123456789");
      CString number2("8765432123456.123476789");
      bcd one(number1);
      bcd two(number2);

      Assert::IsTrue(one < two);
    }

    TEST_METHOD(T036_Operator_comparison_smaller)
    {
      Logger::WriteMessage("Test comparison bcd > bcd");

      CString number1("8765432123456.123456789");
      CString number2("8765432123456.123476789");
      bcd one(number1);
      bcd two(number2);

      Assert::IsTrue(two > one);
    }

    TEST_METHOD(T037_Make_Zero)
    {
      Logger::WriteMessage("Test set to 0 (zero) with: bcd.Zero()");

      bcd one("12345.898989898342212");
      bcd zero;
      one.Zero();

      Assert::IsTrue(zero == one);
      Assert::IsTrue(zero.IsNull());
      Assert::IsTrue(one.IsNull());
    }

    TEST_METHOD(T038_Fraction)
    {
      Logger::WriteMessage("Test fraction part with: bcd.Fraction()");

      bcd one("12345.898989898342212");
      bcd expect ("0.898989898342212");
      bcd result = one.Fraction();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T039_Ceiling)
    {
      Logger::WriteMessage("Test ceiling with: bcd.Ceiling()");

      bcd one("12345.898989898342212");
      bcd expect = 12346;
      bcd result = one.Ceiling();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T040_SquareRoot)
    {
      Logger::WriteMessage("Test root with: bcd.SquareRoot()");

      // 9988776655,4433221100
      bcd one("9988776655.4433221100");
      bcd expect("99943.86752294170705107883721621278845710");
      bcd result = one.SquareRoot();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T041_Power)
    {
      Logger::WriteMessage("Test power with: bcd.Power()");

      // 9988776655,4433221112
      // 3,78123764321234
      bcd one("9988776655.4433221112");
      bcd two("3.78123764321234");
      bcd expect("6.464461188746553021750322457303347299096E+37");
      bcd result = one.Power(two);

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T042_Absolute)
    {
      Logger::WriteMessage("Test absolute value with: bcd.Absolute()");

      // 9988776655,4433221112
      bcd one  ("-9988776655.4433221112");
      bcd expect("9988776655.4433221112");
      bcd result = one.AbsoluteValue();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T043_Reciproke)
    {
      Logger::WriteMessage("Test reciproke value with: bcd.Reciproke()");

      // 9988776655,4433221112
      bcd one("-9988776655.4433221112");
      bcd expect("-1.001123595505617977406695215250599671773E-10");
      bcd result = one.Reciproke();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T044_Natural_logarithm)
    {
      Logger::WriteMessage("Test logarithm value with: bcd.Log()");

      // 9988776655,4433221112
      bcd one("9988776655.4433221112");
      bcd expect("23.02472796519583305299405544350891551917");
      bcd result = one.Log();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T045_10_logarithm)
    {
      Logger::WriteMessage("Test 10 logarithm value with: bcd.Log10()");

      // 9988776655,4433221112
      bcd one("9988776655.4433221112");
      bcd expect("9.999512302608037994845893651154684789013");
      bcd result = one.Log10();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T046_e_power)
    {
      Logger::WriteMessage("Test e-power value with: bcd.Exp()");

      // 99,887766554433221112
      bcd one("99.887766554433221112");
      bcd expect("2.402734720884442281355919192444100235161E+43");
      bcd result = one.Exp();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T047_10th_power)
    {
      Logger::WriteMessage("Test 10th-power value with: bcd.TenPower()");

      // 99,887766554433221112
      bcd one("99.887766554433221112");
      bcd expect("998877665.54433221112");
      bcd result = one.TenPower(7);

      Assert::IsTrue(expect == result);
    }
  };
}