#include "stdafx.h"
#include "CppUnitTest.h"
#include "bcd.h"
#include "HPFCounter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(UnitTestBCD)
	{
	public:
		
    TEST_METHOD(T001_Constructor01)
    {
      Logger::WriteMessage("Construct bcd");
      bcd one;
      Assert::IsTrue(one.IsZero());
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
      bcd  one( expect);
      __int64 result = one.AsInt64();

      Assert::IsTrue(result - expect == 0);
    }

    TEST_METHOD(T007_Constructor07)
    {
      Logger::WriteMessage("Construct a bcd from an unsigned __int64");

      unsigned __int64 expect(76543219876543210LL);
      bcd  one(_T("76543219876543210"));
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

      CString large(_T("76543219876543210"));
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
      CString expect(_T("3.141592653589793238462643383279502884197"));

      Assert::AreEqual(expect.GetString(),pi_string.GetString());
    }

    TEST_METHOD(T012_Constant_LN2)
    {
      Logger::WriteMessage("Test the constant LN2");

      bcd ln2 = bcd::LN2();
      CString ln2_string = ln2.AsString();
      CString expect(_T("0.6931471805599453094172321214581765680755"));

      Assert::AreEqual(expect.GetString(),ln2_string.GetString());
    }

    TEST_METHOD(T013_Constant_LN10)
    {
      Logger::WriteMessage("Test the constant LN10");

      bcd ln10 = bcd::LN10();
      CString ln10_string = ln10.AsString();
      CString expect(_T("2.302585092994045684017991454684364207601"));

      Assert::AreEqual(expect.GetString(),ln10_string.GetString());
    }

    TEST_METHOD(T014_Operator_plus)
    {
      Logger::WriteMessage("Test operator +");

      bcd one(_T("123456.789012345678"));
      bcd two(_T("9988776655.4433221100"));
      bcd result = one + two;
      CString resstring = result.AsString();
      TCHAR* expect = _T("9988900112.232334455678");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T015_Operator_min)
    {
      Logger::WriteMessage("Test operator -");

      // 123456,789012345678
      // 9988776655,4433221100
      bcd one(_T("123456.789012345678"));
      bcd two(_T("9988776655.4433221100"));
      bcd result = one - two;
      CString resstring = result.AsString();
      TCHAR* expect = _T("-9988653198.654309764322");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T016_Operator_multiply)
    {
      Logger::WriteMessage("Test operator *");

      // 123456,789012345678
      // 9988776655,4433221100
      bcd one(_T("123456.789012345678"));
      bcd two(_T("9988776655.4433221100"));
      bcd result = one * two;
      CString resstring = result.AsString();
      TCHAR* expect = _T("1233182292042510.13939532499202034058");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T017_Operator_divide)
    {
      Logger::WriteMessage("Test operator /");

      // 123456,789012345678
      // 9988776655,4433221100
      bcd one(_T("123456.789012345678"));
      bcd two(_T("9988776655.4433221100"));
      bcd result = one / two;
      CString resstring = result.AsString(bcd::Format::Engineering);
      TCHAR* expect = _T("1.235955045056179765144678702196566090151E-5");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T018_Operator_modulo)
    {
      Logger::WriteMessage("Test operator %");

      // 123456,789012345678
      // 9988776655,4433221100
      bcd one(_T("123456.789012345678"));
      bcd two(_T("9988776655.4433221100"));
      bcd result = two % one;
      CString resstring = result.AsString();
      TCHAR* expect = _T("11313.243445648698");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T019_Operator_assign_plus)
    {
      Logger::WriteMessage("Test operator +=");

      bcd one(_T("123456.789012345678"));
      bcd two(_T("9988776655.4433221100"));
      one += two;
      CString resstring = one.AsString();
      TCHAR* expect = _T("9988900112.232334455678");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T020_Operator_assign_min)
    {
      Logger::WriteMessage("Test operator -=");

      // 123456,789012345678
      // 9988776655,4433221100
      bcd one(_T("123456.789012345678"));
      bcd two(_T("9988776655.4433221100"));
      one -= two;
      CString resstring = one.AsString();
      TCHAR* expect = _T("-9988653198.654309764322");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T021_Operator_assign_multiply)
    {
      Logger::WriteMessage("Test operator *=");

      // 123456,789012345678
      // 9988776655,4433221100
      bcd one(_T("123456.789012345678"));
      bcd two(_T("9988776655.4433221100"));
      one *= two;
      CString resstring = one.AsString();
      TCHAR* expect = _T("1233182292042510.13939532499202034058");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }
 
    TEST_METHOD(T022_Operator_assing_divide)
    {
      Logger::WriteMessage("Test operator /=");

      // 123456,789012345678
      // 9988776655,4433221100
      bcd one(_T("123456.789012345678"));
      bcd two(_T("9988776655.4433221100"));
      one /= two;
      CString resstring = one.AsString(bcd::Format::Engineering);
      TCHAR* expect = _T("1.235955045056179765144678702196566090151E-5");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }
 
    TEST_METHOD(T023_Operator_assign_modulo)
    {
      Logger::WriteMessage("Test operator %=");

      // 123456,789012345678
      // 9988776655,4433221100
      bcd one(_T("123456.789012345678"));
      bcd two(_T("9988776655.4433221100"));
      two %= one;
      CString resstring = two.AsString();
      TCHAR* expect = _T("11313.243445648698");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T024_Operator_negate)
    {
      Logger::WriteMessage("Test operator unary - (negate)");

      // 123456,789012345678
      // 9988776655,4433221100
      bcd one(_T("123456.789012345678"));
      bcd two = -one;
      CString resstring = two.AsString();
      TCHAR* expect = _T("-123456.789012345678");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T025_Operator_postfix_increment)
    {
      Logger::WriteMessage("Test operator bcd++ (postfix increment)");

      bcd one(_T("1234567890123456789"));
      bcd two = one++;
      CString resstring1 = one.AsString();
      CString resstring2 = two.AsString();
      TCHAR* expect1 = _T("1234567890123456790.00");
      TCHAR* expect2 = _T("1234567890123456789.00");

      Assert::IsTrue(strcmp(expect1,resstring1.GetString()) == 0);
      Assert::IsTrue(strcmp(expect2,resstring2.GetString()) == 0);
    }

    TEST_METHOD(T026_Operator_prefix_increment)
    {
      Logger::WriteMessage("Test operator ++bcd (prefix increment)");

      bcd one(_T("1234567890123456789"));
      bcd two = ++one;
      CString resstring1 = one.AsString();
      CString resstring2 = two.AsString();
      TCHAR* expect1 = _T("1234567890123456790.00");
      TCHAR* expect2 = _T("1234567890123456790.00");

      Assert::IsTrue(strcmp(expect1,resstring1.GetString()) == 0);
      Assert::IsTrue(strcmp(expect2,resstring2.GetString()) == 0);
    }

    TEST_METHOD(T027_Operator_postfix_decrement)
    {
      Logger::WriteMessage("Test operator bcd-- (postfix decrement)");

      bcd one(_T("1234567890123456790"));
      bcd two = one--;
      CString resstring1 = one.AsString();
      CString resstring2 = two.AsString();
      TCHAR* expect1 = _T("1234567890123456789.00");
      TCHAR* expect2 = _T("1234567890123456790.00");

      Assert::IsTrue(strcmp(expect1,resstring1.GetString()) == 0);
      Assert::IsTrue(strcmp(expect2,resstring2.GetString()) == 0);
    }

    TEST_METHOD(T028_Operator_prefix_decrement)
    {
      Logger::WriteMessage("Test operator bcd-- (postfix decrement)");

      bcd one(_T("1234567890123456790"));
      bcd two = --one;
      CString resstring1 = one.AsString();
      CString resstring2 = two.AsString();
      TCHAR* expect1 = _T("1234567890123456789.00");
      TCHAR* expect2 = _T("1234567890123456789.00");

      Assert::IsTrue(strcmp(expect1,resstring1.GetString()) == 0);
      Assert::IsTrue(strcmp(expect2,resstring2.GetString()) == 0);
    }

    TEST_METHOD(T029_Operator_assign_bcd)
    {
      Logger::WriteMessage("Test operator bcd = bcd (assignment operator)");

      bcd one(_T("123456.789012345678"));
      bcd two = one;
      CString resstring = two.AsString(bcd::Format::Bookkeeping,false,12);
      TCHAR* expect = _T("123456.789012345678");

      Assert::AreEqual(expect,resstring.GetString());
    }

    TEST_METHOD(T030_Operator_assign_long)
    {
      Logger::WriteMessage("Test operator bcd = long (assignment operator)");

      long one = 123456789;
      bcd two(one);
      CString resstring = two.AsString();
      TCHAR* expect = _T("123456789.00");

      Assert::AreEqual(expect,resstring.GetString());
    }

    TEST_METHOD(T031_Operator_assign_double)
    {
      Logger::WriteMessage("Test operator bcd = double (assignment operator)");

      double one = 123456.123456789;
      bcd two(one);
      CString resstring = two.AsString();
      TCHAR* expect = _T("123456.123456789");

      Assert::AreEqual(expect,resstring.GetString());
    }

    TEST_METHOD(T032_Operator_assign_CString)
    {
      Logger::WriteMessage("Test operator bcd = CString (assignment operator)");

      CString one(_T("123456.123456789"));
      bcd two(one);
      CString resstring = two.AsString();
      TCHAR* expect = _T("123456.123456789");

      Assert::AreEqual(expect,resstring.GetString());
    }

    TEST_METHOD(T033_Operator_comparison_equal)
    {
      Logger::WriteMessage("Test comparison bcd == bcd");
      Logger::WriteMessage("Test comparison bcd >= bcd");
      Logger::WriteMessage("Test comparison bcd <= bcd");

      CString number(_T("8765432123456.123456789"));
      bcd one(number);
      bcd two(number);

      Assert::IsTrue(one == two);
      Assert::IsTrue(one <= two);
      Assert::IsTrue(one >= two);
    }

    TEST_METHOD(T034_Operator_comparison_notequal)
    {
      Logger::WriteMessage("Test comparison bcd != bcd");

      CString number1(_T("8765432123456.123456789"));
      CString number2(_T("8765432123456.123476789"));
      bcd one(number1);
      bcd two(number2);

      Assert::IsTrue(one != two);
    }

    TEST_METHOD(T035_Operator_comparison_smaller)
    {
      Logger::WriteMessage("Test comparison bcd < bcd");

      CString number1(_T("8765432123456.123456789"));
      CString number2(_T("8765432123456.123476789"));
      bcd one(number1);
      bcd two(number2);

      Assert::IsTrue(one < two);
    }

    TEST_METHOD(T036_Operator_comparison_smaller)
    {
      Logger::WriteMessage("Test comparison bcd > bcd");

      CString number1(_T("8765432123456.123456789"));
      CString number2(_T("8765432123456.123476789"));
      bcd one(number1);
      bcd two(number2);

      Assert::IsTrue(two > one);
    }

    TEST_METHOD(T037_Make_Zero)
    {
      Logger::WriteMessage("Test set to 0 (zero) with: bcd.Zero()");

      bcd one(_T("12345.898989898342212"));
      bcd zero;
      one.Zero();

      Assert::IsTrue(zero == one);
      Assert::IsTrue(zero.IsZero());
      Assert::IsTrue(one.IsZero());
    }

    TEST_METHOD(T038_Fraction)
    {
      Logger::WriteMessage("Test fraction part with: bcd.Fraction()");

      bcd one(_T("12345.898989898342212"));
      bcd expect (_T("0.898989898342212"));
      bcd result = one.Fraction();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T039_Ceiling)
    {
      Logger::WriteMessage("Test ceiling with: bcd.Ceiling()");

      bcd one(_T("12345.898989898342212"));
      bcd expect(12346);
      bcd result = one.Ceiling();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T040_SquareRoot)
    {
      Logger::WriteMessage("Test root with: bcd.SquareRoot()");

      // 9988776655,4433221100
      bcd one(_T("9988776655.4433221100"));
      bcd expect(_T("99943.86752294170705107883721621278845710"));
      bcd result = one.SquareRoot();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T041_Power)
    {
      Logger::WriteMessage("Test power with: bcd.Power()");
      
      // 9988776655,4433221112
      // 3,78123764321234
      bcd one(_T("9988776655.4433221112"));
      bcd two(_T("3.78123764321234"));
      bcd expect(_T("6.46446118874655302175032245730334730318E+37"));
      bcd result = one.Power(two);
      Logger::WriteMessage(result.AsString()); 

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T042_Absolute)
    {
      Logger::WriteMessage("Test absolute value with: bcd.Absolute()");

      // 9988776655,4433221112
      bcd one  (_T("-9988776655.4433221112"));
      bcd expect(_T("9988776655.4433221112"));
      bcd result = one.AbsoluteValue();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T043_Reciproke)
    {
      Logger::WriteMessage("Test reciproke value with: bcd.Reciproke()");

      // 9988776655,4433221112
      bcd one(_T("-9988776655.4433221112"));
      bcd expect(_T("-1.001123595505617977406695215250599671773E-10"));
      bcd result = one.Reciprocal();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T044_Natural_logarithm)
    {
      Logger::WriteMessage("Test logarithm value with: bcd.Log()");

      // 9988776655,4433221112
      bcd one(_T("9988776655.4433221112"));
      bcd expect(_T("23.02472796519583305299405544350891551931"));

      bcd result = one.Log();

      Logger::WriteMessage(result.AsString());

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T045_10_logarithm)
    {
      Logger::WriteMessage("Test 10 logarithm value with: bcd.Log10()");

      // 9988776655,4433221112
      bcd one(_T("9988776655.4433221112"));
      bcd expect(_T("9.99951230260803799484589365115468478907"));

      bcd result = one.Log10();

      Logger::WriteMessage(result.AsString());

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T046_e_power)
    {
      Logger::WriteMessage("Test e-power value with: bcd.Exp()");

      // 99,887766554433221112
      bcd one(_T("99.887766554433221112"));
      bcd expect(_T("2.402734720884442281355919192444100235161E+43"));
      bcd result = one.Exp();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T047_10th_power)
    {
      Logger::WriteMessage("Test 10th-power value with: bcd.TenPower()");

      // 99,887766554433221112
      bcd one(_T("99.887766554433221112"));
      bcd expect(_T("998877665.54433221112"));
      bcd result = one.TenPower(7);

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T048_Round)
    {
      Logger::WriteMessage("Test rounding of a number with : bcd.Round(5)");
      
      // 9988776655,4433277112
      bcd one(_T("9988776655.4433277112"));
      one.Round(5);
      bcd expect(_T("9988776655.44333"));
      CString test = one.AsString();

      Assert::IsTrue(expect == one);

      one = _T("9988776655.64433277112");
      expect = _T("9988776655.644333");
      one.Round(6);
      Assert::IsTrue(expect == one);

      one = _T("9988776655.64433277112");
      expect = _T("9988776656");
      one.Round(0);
      Assert::IsTrue(expect == one);
    }

    TEST_METHOD(T049_Truncate)
    {
      Logger::WriteMessage("Test truncation of a number with : bcd.Truncate(5)");
      
      // 9988776655,4433277112
      bcd one(_T("9988776655.4433277112"));
      one.Truncate(5);
      bcd expect(_T("9988776655.44332"));

      Assert::IsTrue(expect == one);

      one = _T("9988776655.64433277112");
      expect = _T("9988776655");
      one.Truncate(0);
      Assert::IsTrue(expect == one);
    }

    TEST_METHOD(T050_Sine)
    {
      Logger::WriteMessage("Test Sine function of a radian with : bcd.Sine()");

      // 0,9876543210123456
      TCHAR* a_angle = _T("0.9876543210123456");
      TCHAR* r_sine  = _T("8.347366295099261173476093153791068840984E-1");

      bcd one(a_angle);
      bcd expect(r_sine);
      bcd result = one.Sine();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T051_Cosine)
    {
      Logger::WriteMessage("Test Cosine function of a radian with : bcd.Cosine()");

      // 0,9876543210123456
      TCHAR* a_angle  = _T("0.9876543210123456");
      TCHAR* r_cosine = _T("5.506493978516714425340844117564672940338E-1");

      bcd one(a_angle);
      bcd expect(r_cosine);
      bcd result = one.Cosine();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T052_Tangent)
    {
      Logger::WriteMessage("Test Tangent function of a radian with : bcd.Tangent()");

      // 0,9876543210123456
      TCHAR* a_angle   = _T("0.9876543210123456");
      TCHAR* r_tangent = _T("1.515913088739596368439240774287234302490");

      bcd one(a_angle);
      bcd expect(r_tangent);
      bcd result = one.Tangent();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T053_ArcSine)
    {
      Logger::WriteMessage("Test ArcSine function of a ratio with : bcd.ArcSine()");

      TCHAR* a_ratio = _T("0.765498765404321098765");
      TCHAR* r_asine = _T("8.71816131070559101024946021343034377293E-1");

      bcd one(a_ratio);
      bcd expect(r_asine);
      bcd result = one.ArcSine();

      Logger::WriteMessage(result.AsString());

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T054_ArcCosine)
    {
      Logger::WriteMessage("Test ArcCosine function of a ratio with : bcd.ArcCosine()");

      TCHAR* a_ratio   = _T("0.765498765404321098765");
      TCHAR* r_acosine = _T("6.98980195724337518206375670296717064805E-1");

      bcd one(a_ratio);
      bcd expect(r_acosine);
      bcd result = one.ArcCosine();

      Logger::WriteMessage(result.AsString());

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T055_ArcTangent)
    {
      Logger::WriteMessage("Test ArcTangent function of a ratio with : bcd.ArcTangent()");

      TCHAR* a_ratio    = _T("0.765498765404321098765");
      TCHAR* r_atangent = _T("6.53346752384431270749403109172000942006E-1");

      bcd one(a_ratio);
      bcd expect(r_atangent);
      bcd result = one.ArcTangent();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T056_SQL_NUMERIC_STRUCT)
    {
      Logger::WriteMessage("Test SQL_NUMERIC_STRUCT by assigning and converting to");

      // num = 10.001 (ten and 1 thousandth)
      SQL_NUMERIC_STRUCT num;
      SQL_NUMERIC_STRUCT res;
      memset(&num,0,sizeof(SQL_NUMERIC_STRUCT));
      memset(&res,0,sizeof(SQL_NUMERIC_STRUCT));

      // Filling NUM
      num.sign      = 1; // Positive
      num.precision = 6;
      num.scale     = 4;
      num.val[0] = 0xAA;  // 1010 1010
      num.val[1] = 0x86;  // 1000 0110
      num.val[2] = 0x01;  // 0000 0001

      bcd one(&num);
      one.AsNumeric(&res);

      bcd result(&res);

      Assert::IsTrue(one == result);
      Assert::IsTrue(one == bcd(_T("10.001")));
    }

    TEST_METHOD(T057_AsDouble)
    {
      Logger::WriteMessage("Test as-other-datatype: bcd.AsDouble()");

      bcd one(_T("99887766.554433"));
      double result = one.AsDouble();
      double expect = 99887766.554433;

      Assert::IsTrue(abs(expect - result) < 0.00005);
    }

    TEST_METHOD(T058_AsShort)
    {
      Logger::WriteMessage("Test as-other-datatype: bcd.AsShort()");

      bcd one(_T("4433"));
      short result = one.AsShort();
      short expect = 4433;

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T059_AsUShort)
    {
      Logger::WriteMessage("Test as-other-datatype: bcd.AsUShort()");

      bcd one(_T("54873"));
      unsigned short result = one.AsUShort();
      unsigned short expect = 54873;

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T060_AsLong)
    {
      Logger::WriteMessage("Test as-other-datatype: bcd.AsLong()");

      bcd one(_T("98854873"));
      long result = one.AsLong();
      long expect = 98854873;

      Assert::IsTrue(expect == result);
      Assert::IsTrue(one.GetFitsInLong());
    }

    TEST_METHOD(T061_AsULong)
    {
      Logger::WriteMessage("Test as-other-datatype: bcd.AsULong()");

      bcd one(_T("2974276950"));
      unsigned long result = one.AsULong();
      unsigned long expect = 2974276950;

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T062_AsInt64)
    {
      Logger::WriteMessage("Test as-other-datatype: bcd.AsInt64()");

      bcd one(_T("23154765019"));
      __int64 result = one.AsInt64();
      __int64 expect = 23154765019L;

      Assert::IsTrue(expect == result);
      Assert::IsTrue(one.GetFitsInInt64());
    }

    TEST_METHOD(T063_Operator_assign_long)
    {
      Logger::WriteMessage("Test operator = long");

      // 123456,789012345678
      // 9988776655,4433221100
      long one = 1234567890L;
      bcd result;
      result = one;
      CString resstring = result.AsString();
      TCHAR* expect = _T("1234567890.00");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T064_Operator_assign_double)
    {
      Logger::WriteMessage("Test operator = double");

      // 123456,789012345678
      // 9988776655,4433221100
      double one = 1234567890.5432L;
      bcd result;
      result = one;
      CString resstring = result.AsString();
      TCHAR* expect = _T("1234567890.5432");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T065_ArcTangent2)
    {
      Logger::WriteMessage("Test ArcTangent2Points function of bcd with bcd::ArcTangent2Points");

      // 1.0456788
      // 7.89991232E-1

      bcd one(_T("1.0456788"));
      bcd two(_T("7.89991232E-1"));

      bcd result = one.ArcTangent2Points(two);
      CString resstring = result.AsString();
      TCHAR* expect = _T("4.065388783692102294632898803723974485214");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }
    
    TEST_METHOD(T066_Negate)
    {
      Logger::WriteMessage("Test the Negate method for unary -");

      bcd one(_T("45523.89901"));
      one.Negate();
      CString resstring = one.AsString();
      TCHAR* expect = _T("-45523.89901");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }
    
    TEST_METHOD(T067_IsNull)
    {
      Logger::WriteMessage("Test the status function IsNull");

      bcd one;
      bcd two(12);

      Assert::IsTrue(one.IsZero());
      Assert::IsFalse(two.IsZero());
    }

    TEST_METHOD(T068_GetSign)
    {
      Logger::WriteMessage("Test the status function GetSign");

      bcd zero;
      bcd one(12);
      bcd two(-12);

      Assert::IsTrue(zero.GetSign() ==  0);
      Assert::IsTrue(one.GetSign()  ==  1);
      Assert::IsTrue(two.GetSign()  == -1);
    }

    TEST_METHOD(T069_GetLength)
    {
      Logger::WriteMessage("Test the status function GetLength");

      // 123456,789012345678
      bcd one(_T("123456.789012345678"));
      Assert::IsTrue(one.GetLength() == 18);
    }

    TEST_METHOD(T070_GetPrecision)
    {
      Logger::WriteMessage("Test the status function GetPrecision");

      // 123456,789012345678
      bcd one(_T("123456.789012345678"));
      Assert::IsTrue(one.GetPrecision() == 12);
    }

    TEST_METHOD(T071_GetMaxSize)
    {
      Logger::WriteMessage("Test the status function GetMaxSize");

      // 123456,789012345678
      bcd one(_T("123456.789012345678"));
      Assert::IsTrue(one.GetMaxSize() == 40);
    }

    TEST_METHOD(T072_GetHasDecimals)
    {
      Logger::WriteMessage("Test the status function GetHasDecimals");

      // 123456,789012345678
      bcd one(_T("123456.789012345678"));
      bcd two(_T("877234"));
      bcd three(_T("123.9"));
      Assert::IsTrue  (one.GetHasDecimals());
      Assert::IsFalse (two.GetHasDecimals());
      Assert::IsTrue(three.GetHasDecimals());
    }

    TEST_METHOD(T073_GetExponent)
    {
      Logger::WriteMessage("Test the function GetExponent");

      // 123456,789012345678
      bcd one(_T("123456.789012345678"));
      bcd two(_T("877234"));
      bcd three(_T("123.9"));
      bcd four(_T("0.0066"));

      Assert::IsTrue(one  .GetExponent() == 5);
      Assert::IsTrue(two  .GetExponent() == 5);
      Assert::IsTrue(three.GetExponent() == 2);
      Assert::IsTrue(four .GetExponent() == -3);
    }

    TEST_METHOD(T074_GetMantissa)
    {
      Logger::WriteMessage("Test the function GetMantissa");

      // 123456,789012345678
      bcd one(_T("123456.789012345678"));
      bcd two(_T("123.9"));

      bcd mant1 = one.GetMantissa();
      bcd mant2 = two.GetMantissa();

      CString m1res = mant1.AsString();
      CString m2res = mant2.AsString();

      CString expect1(_T("1.23456789012345678"));
      CString expect2(_T("1.239"));

      Assert::AreEqual(expect1.GetString(),m1res.GetString());
      Assert::AreEqual(expect2.GetString(),m2res.GetString());
    }

    TEST_METHOD(T075_ReadWriteFile)
    {
      Logger::WriteMessage("Test writing to file");

      // 123456,789012345678
      bcd one(_T("123456.789012345678"));
      FILE* file = nullptr;
      _tfopen_s(&file,_T("C:\\TMP\\bcd.test"),_T("wb"));
      if(file)
      {
        one.WriteToFile(file);
        fclose(file);
      }

      bcd result;
      Logger::WriteMessage("Test reading from file");
      file = nullptr;
      _tfopen_s(&file,_T("C:\\TMP\\bcd.test"),_T("rb"));
      if(file)
      {
        result.ReadFromFile(file);
        fclose(file);
      }

      Assert::IsTrue(one == result);
    }

    TEST_METHOD(T076_math_floor)
    {
      Logger::WriteMessage("Test math style floor");

      // 123456,789012345678
      bcd one(_T("123456.789012345678"));
      bcd result = floor(one);
      CString resstring = result.AsString();
      TCHAR* expect(_T("123456.00"));

      Assert::AreEqual(expect,resstring.GetString());
    }

    TEST_METHOD(T077_math_ceil)
    {
      Logger::WriteMessage("Test math style ceil");

      // 123456,789012345678
      bcd one(_T("123456.789012345678"));
      bcd result = ceil(one);
      CString resstring = result.AsString();
      TCHAR* expect(_T("123457.00"));

      Assert::AreEqual(expect,resstring.GetString());
    }

    TEST_METHOD(T078_AsDisplayString)
    {
      Logger::WriteMessage("Test AsDisplayString");

      // BEWARE: I am testing this in the Dutch locale
      // Thousands separator is a decimal point
      // Decimals  separator is a comma

      // 123456,789012345678
      bcd one(_T("123456.789012345678"));
      CString result = one.AsDisplayString();
      TCHAR* expect = _T("123.456,79");
      Assert::AreEqual(expect,result.GetString());

      bcd two(_T("9123456.789012345678"));
      CString result2 = two.AsDisplayString();
      TCHAR* expect2 = _T("9.123.456,79");
      Assert::AreEqual(expect2,result2.GetString());
    }

    TEST_METHOD(T079_Sine_zero)
    {
      Logger::WriteMessage("Test Sine(0.0)");

      // Reputated to enter and endless loop
      bcd zero;
      bcd sineZero = zero.Sine();

      CString zeroResult(_T("0.00"));
      CString zeroSine = sineZero.AsString();

      Assert::AreEqual(zeroSine.GetString(),zeroResult.GetString());
    }

    TEST_METHOD(T080_Cosine_zero)
    {
      Logger::WriteMessage("Test Cosine(0.0)");

      // Reputated to enter and endless loop
      bcd zero;
      bcd cosineZero = zero.Cosine();

      CString zeroResult(_T("1.00"));
      CString zeroCosine = cosineZero.AsString();

      Assert::AreEqual(zeroCosine.GetString(), zeroResult.GetString());
    }

    TEST_METHOD(T081_Exponent_zero)
    {
      Logger::WriteMessage("Test Exp(0.0)");

      // Reputated to enter and endless loop
      bcd zero;
      bcd expZero = zero.Exp();

      CString zeroResult(_T("1.00"));
      CString zeroExponent = expZero.AsString();

      Assert::AreEqual(zeroExponent.GetString(), zeroResult.GetString());
    }

    TEST_METHOD(T082_Sine_pi)
    {
      Logger::WriteMessage("Test Sine(180 degrees)");

      // Reputated to enter and endless loop
      bcd valuepi = bcd::PI();
      bcd sinepi = valuepi.Sine();

      Assert::IsTrue(sinepi.IsNearZero());
    }

    TEST_METHOD(T083_Cosine_pi)
    {
      Logger::WriteMessage("Test Cosine(180 degrees)");

      // Reputated to enter and endless loop
      bcd valuepi = bcd::PI();
      bcd cosinepi = valuepi.Cosine();
      cosinepi += 1;

      Assert::IsTrue(cosinepi.IsNearZero());
    }

    TEST_METHOD(T084_Exponents1)
    {
      Logger::WriteMessage("Test various exponents");

      bcd number(_T("0.01"));
      bcd expnumber = number.Exp();
      expnumber.Round(15);
      double expdouble = ::exp(0.01);
      CString expected(_T("1.010050167084168"));

      CString bcdResult;
      CString dblResult;
      bcdResult = expnumber.AsString();
      dblResult.Format(_T("%0.16g"), expdouble);

      // Be sure to eliminate the locale differences
      bcdResult.Replace(_T(","), _T("."));
      dblResult.Replace(_T(","), _T("."));

      Assert::AreEqual(expected.GetString(),dblResult.GetString());
      Assert::AreEqual(expected.GetString(),bcdResult.GetString());
    }

    TEST_METHOD(T085_Exponents2)
    {
      Logger::WriteMessage("Test various exponents");

      bcd number(_T("0.03"));
      bcd expnumber = number.Exp();
      expnumber.Round(15);
      double expdouble = ::exp(0.03);
      CString expected(_T("1.030454533953517"));

      CString bcdResult;
      CString dblResult;
      bcdResult = expnumber.AsString();
      dblResult.Format(_T("%0.16g"), expdouble);

      // Be sure to eliminate the locale differences
      bcdResult.Replace(_T(","), _T("."));
      dblResult.Replace(_T(","), _T("."));

      Assert::AreEqual(expected.GetString(), dblResult.GetString());
      Assert::AreEqual(expected.GetString(), bcdResult.GetString());
    }

    TEST_METHOD(T086_Divide10)
    {
      Logger::WriteMessage("Test division by 10");

      bcd number(_T("0.1"));
      bcd numresult = number / 10.0;

      CString expected(_T("0.01"));
      CString result = numresult.AsString();
      
      Assert::AreEqual(expected.GetString(),result.GetString());
    }

    TEST_METHOD(T087_Divide10_Wrong)
    {
      Logger::WriteMessage("Test division by 10 wrong?");

      bcd numresult(0.2 / 10.0);

      CString expected(_T("0.02"));
      CString result = numresult.AsString();

#ifdef _DEBUG
      CString mantissa = numresult.DebugPrint("Correct 0.02");
      Logger::WriteMessage(mantissa);
#endif

      Assert::AreEqual(expected.GetString(), result.GetString());
    }

    TEST_METHOD(T088_Exponent_zero)
    {
      Logger::WriteMessage("Test exp(0.0)");

      double expected = ::exp(0.0);
      bcd    result = bcd(0).Exp();

      Assert::AreEqual(expected, result.AsDouble());
    }

    TEST_METHOD(T089_Exponent_negative)
    {
      Logger::WriteMessage("Test exponent of negative number");

      double dblres = ::exp(-2.5);
      bcd    bcdres = bcd(_T("-2.5")).Exp();
      bcdres.Round(16);

      CString expected(_T("0.0820849986238988"));

      CString bcdResult;
      CString dblResult;
      bcdResult = bcdres.AsString();
      dblResult.Format(_T("%0.15g"), dblres);

      // Be sure to eliminate the locale differences
      bcdResult.Replace(_T(","),_T("."));
      dblResult.Replace(_T(","),_T("."));

      Assert::AreEqual(expected.GetString(), dblResult.GetString());
      Assert::AreEqual(expected.GetString(), bcdResult.GetString());
    }

    TEST_METHOD(T090_Adding_negative_small)
    {
      Logger::WriteMessage("Adding small negative number to 0.01");

      bcd tinynum(_T("0.01"));
      bcd number(_T("-0.05"));

      bcd result = tinynum + number;
      CString expected(_T("-0.04"));
      CString resstring = result.AsString();

      Assert::AreEqual(expected.GetString(),resstring.GetString());
    }

    TEST_METHOD(T091_Adding_negative)
    {
      Logger::WriteMessage("Adding small negative number to 0.0");

      // Original values provided by sisirajaya
      bcd tinynum(_T("0.0"));
      bcd number(_T("-0.05"));

      bcd result = tinynum + number;
      CString expected(_T("-0.05"));
      CString resstring = result.AsString();

      Assert::AreEqual(expected.GetString(), resstring.GetString());
    }

    // REMARK
    // Only run this test if you are suspicious about the inner workings of "AsInt64"
    // As this test takes a long time to run
    //
//     TEST_METHOD(T092_IntegerConversion)
//     {
//       Logger::WriteMessage("Converting to and from regular integers");
// 
//       // BCD TEST
//       for(__int64 val = INT_MIN; val <= INT_MAX; ++val)
//       {
//         bcd b = val;
//         __int64 back = b.AsInt64();
//         if(back != val)
//         {
//           Assert::Fail(L"bcd <-> integer64 just broke!");
//         }
// 
//         // Because it runs a long time...
//         if((val % bcdBase) == 0)
//         {
//           CString message;
//           message.Format("100 million mark at: %i",val);
//           Logger::WriteMessage(message);
//         }
//       }
//     }

    TEST_METHOD(T093_Int64Conversion)
    {
      Logger::WriteMessage("Testing at higher ranges.");

      bcd one(_T("94505024884"));
      CString expected(_T("94505024884"));
      UINT64 number = one.AsUInt64();
      CString result;
      result.Format(_T("%I64d"),number);

      Assert::AreEqual(expected.GetString(),result.GetString());
    }

    TEST_METHOD(T094_Int64PlusDecimals)
    {
      Logger::WriteMessage("Testing int64 plus decimals.");

      UINT64 num = 94505024884;
      bcd one (num,64);
      CString expected(_T("94505024884.64"));
      CString result = one.AsString();

      Assert::AreEqual(expected.GetString(),result.GetString());
    }

    TEST_METHOD(T095_CornerCases)
    {
      Logger::WriteMessage("Testing integer corner cases.");

      bcd test1((TCHAR)MININT8);      Assert::AreEqual((int)MININT8,  (int)test1.AsShort());
      bcd test2((TCHAR)MAXINT8);      Assert::AreEqual((int)MAXINT8,  (int)test2.AsShort()); 

      bcd test3((uchar)0);           Assert::AreEqual(0,             (int)test3.AsShort()); 
      bcd test4((uchar)MAXUINT8);    Assert::AreEqual((int)MAXUINT8, (int)test4.AsShort());

      bcd test5((short)MININT16);    Assert::AreEqual((int)MININT16, (int)test5.AsShort()); 
      bcd test6((short)MAXINT16);    Assert::AreEqual((int)MAXINT16, (int)test6.AsShort()); 

      bcd test7((ushort)0);          Assert::AreEqual((int)0,        (int)test7.AsUShort()); 
      bcd test8((ushort)MAXUINT16);  Assert::AreEqual((int)MAXUINT16,(int)test8.AsUShort()); 

      bcd test9((long)MININT32);     Assert::AreEqual((int)MININT32, (int)test9.AsLong());
      bcd test10((long)MAXINT32);    Assert::AreEqual((int)MAXINT32, (int)test10.AsLong()); 

      bcd test11((ulong)0);          Assert::IsTrue(test11.AsULong() == 0); 
      bcd test12((ulong)MAXUINT32);  Assert::IsTrue(test12.AsULong() == MAXUINT32); 

      bcd test13((INT64)MININT64);   Assert::IsTrue(test13.AsInt64() == MININT64); 
      bcd test14((INT64)MAXINT64);   Assert::IsTrue(test14.AsInt64() == MAXINT64); 

      bcd test15((UINT64)0);         Assert::IsTrue(test15.AsUInt64() == 0L); 
      bcd test16((UINT64)MAXUINT64); Assert::IsTrue(test16.AsUInt64() == MAXUINT64);
    }

    TEST_METHOD(T096_TestNaturalLog)
    {
      Logger::WriteMessage("Testing BCD Natural Log 0.5");

      bcd value(_T("0.5"));
      bcd nlog = value.Log();

      CString expected = _T("-0.693147180559945309417232121458176568023");
      CString testval  = nlog.AsString(bcd::Format::Bookkeeping, false, 30);

      Assert::AreEqual(expected.GetString(), testval.GetString());
    }

    TEST_METHOD(T097_CalculatePI)
    {
      Logger::WriteMessage("Testing BCD calculating PI");
      bcd value("-1");
      bcd pi = value.ArcCosine();
      bcd internalPi = bcd::PI();

      CString disp = pi.AsString(bcd::Format::Engineering);
      CString intr = internalPi.AsString(bcd::Format::Engineering);

      bcd diff = pi - internalPi;
      Assert::IsTrue(diff.IsNearZero());
    }

    TEST_METHOD(T098_NaturalLog2)
    {
      Logger::WriteMessage("Testing BCD Natural LN 2");
      bcd value("2");
      bcd ln2 = value.Log();
      bcd internal = bcd::LN2();

      bcd diff = ln2 - internal;
      Assert::IsTrue(diff.IsNearZero());
    }

    TEST_METHOD(T099_NaturalLog10)
    {
      Logger::WriteMessage("Testing BCD Natural Log 10");
      bcd value("10");
      bcd log = value.Log();
      bcd internal = bcd::LN10();

      log.Round(35);
      internal.Round(35);

      CString reslog = log.AsString();
      CString resint = internal.AsString();
      Assert::IsTrue(reslog == resint);
    }

    // USE
    // #define ALT_SQUAREROOT
    // To test the alternate algorithm provided by blackjetrock
    // 
    TEST_METHOD(T100_Root10thousands)
    {
      Logger::WriteMessage("Testing BCD square root(0.0001)");
      bcd value("0.0001");
      bcd sqroot;
      
      HPFCounter count;
      for (int ind = 0; ind < 100; ++ind)
      {
        sqroot = value.SquareRoot();
      }
      count.Stop();

      sqroot.Round(20);
      CString result = sqroot.AsString();
      CString expected("0.01");

      Assert::IsTrue(result == expected);

      CString timing;
      timing.Format("Total calculation time: %f", count.GetCounter() / 100);
      Logger::WriteMessage(timing);
      // Unmodified: 4 nano-sec
      // Total calculation time: 0.000004
      // Modified algorithm blackjetrock: 56 nano-sec
      // Total calculation time: 0.000056
    }
  };
}