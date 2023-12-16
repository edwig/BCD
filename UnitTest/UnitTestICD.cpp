#include "stdafx.h"
#include "CppUnitTest.h"
#include "icd.h"
#include "HPFCounter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(UnitTestICD)
	{
	public:
		
    TEST_METHOD(T001_Constructor01)
    {
      Logger::WriteMessage("Construct icd");
      icd one;
      Assert::IsTrue(one.IsNull());
    }

		TEST_METHOD(T002_Constructor02)
		{
      Logger::WriteMessage("Construct icd from an 1 byte number");

      icd  one((TCHAR)'\n');
      long expect(10);

      Assert::AreEqual(expect,one.AsLong());
		}

    TEST_METHOD(T003_Constructor03)
    {
      Logger::WriteMessage("Construct a icd from a short int");

      short expect(12);
      icd   one(expect);

      Assert::AreEqual((long)expect,one.AsLong());
    }

    TEST_METHOD(T004_Constructor04)
    {
      Logger::WriteMessage("Construct a icd from a long");

      long expect(765432198);
      icd  one(expect);

      Assert::AreEqual((long)expect,one.AsLong());
    }

    TEST_METHOD(T005_Constructor05)
    {
      Logger::WriteMessage("Construct a icd from a long + remainder");

      long base(765432198);
      long remainder(50000000);
      icd  one(base,remainder);
      double expect = 765432198.5;
      double result = one.AsDouble();
      expect = trunc(expect * 10) / 10;
      result = trunc(result * 10) / 10;

      Assert::AreEqual(expect,result);
    }

    TEST_METHOD(T006_Constructor06)
    {
      Logger::WriteMessage("Construct a icd from an __int64");

      __int64 expect(76543219876543210LL);
      icd  one( expect);
      __int64 result = one.AsInt64();

      Assert::IsTrue(result - expect == 0);
    }

    TEST_METHOD(T007_Constructor07)
    {
      Logger::WriteMessage("Construct a icd from an unsigned __int64");

      unsigned __int64 expect(76543219876543210LL);
      icd  one(_T("76543219876543210"));
      unsigned __int64 result = one.AsUInt64();

      Assert::IsTrue(result - expect == 0);
    }

    TEST_METHOD(T008_Constructor08)
    {
      Logger::WriteMessage("Construct a icd from another icd");

      icd one(12345L);
      icd two(one);

      Assert::IsTrue(one.AsLong() - two.AsLong() == 0);
    }

    TEST_METHOD(T009_Constructor09)
    {
      Logger::WriteMessage("Construct a icd from a double");

      double expect = 765432198.5;
      icd  one(expect);
      double result = one.AsDouble();
      expect = trunc(expect * 10) / 10;
      result = trunc(result * 10) / 10;

      Assert::AreEqual(expect,result);
    }

    TEST_METHOD(T010_Constructor10)
    {
      Logger::WriteMessage("Construct a icd from a CString");

      CString large(_T("76543219876543210"));
      unsigned __int64 expect(76543219876543210LL);
      icd  one(large);
      unsigned __int64 result = one.AsUInt64();

      Assert::IsTrue(result - expect == 0);
    }

    TEST_METHOD(T011_Constant_PI)
    {
      Logger::WriteMessage("Test the constant of PI");
      icd pi = icd::PI();
      CString pi_string = pi.AsString();
      // First 104 decimals of PI
      CString expect(_T("+3.1415926535897932384626433832795028841972"));

      Assert::AreEqual(expect.GetString(),pi_string.GetString());
    }

    TEST_METHOD(T012_Constant_LN2)
    {
      Logger::WriteMessage("Test the constant LN2");

      icd ln2 = icd::LN2();
      CString ln2_string = ln2.AsString();
      // LN2 in 104 decimals
      CString expect(_T("+0.6931471805599453094172321214581765680756"));

      Assert::AreEqual(expect.GetString(),ln2_string.GetString());
    }

    TEST_METHOD(T013_Constant_LN10)
    {
      Logger::WriteMessage("Test the constant LN10");

      icd ln10 = icd::LN10();
      CString ln10_string = ln10.AsString();
      // LN10 in 104 decimals
      CString expect(_T("+2.3025850929940456840179914546843642076019"));

      Assert::AreEqual(expect.GetString(),ln10_string.GetString());
    }

    TEST_METHOD(T014_Operator_plus)
    {
      Logger::WriteMessage("Test operator +");

      icd one(_T("123456.789012345678"));
      icd two(_T("9988776655.4433221100"));
      icd result = one + two;
      result.SetLengthAndPrecision(40,12);
      CString resstring = result.AsString();
      TCHAR* expect = _T("+9988900112.232334455678");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T015_Operator_min)
    {
      Logger::WriteMessage("Test operator -");

      // 123456,789012345678
      // 9988776655,4433221100
      icd one(_T("123456.789012345678"));
      icd two(_T("9988776655.4433221100"));
      icd result = one - two;
      result.SetLengthAndPrecision(40,12);
      CString resstring = result.AsString();
      TCHAR* expect = _T("-9988653198.654309764322");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T016_Operator_multiply)
    {
      Logger::WriteMessage("Test operator *");

      // 123456,789012345678
      // 9988776655,4433221100
      icd one(_T("123456.789012345678"));
      icd two(_T("9988776655.4433221100"));
      icd result = one * two;
      result.SetLengthAndPrecision(40,20);
      CString resstring = result.AsString();
      TCHAR* expect = _T("+1233182292042510.13939532499202034058");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T017_Operator_divide)
    {
      Logger::WriteMessage("Test operator /");

      // 123456,789012345678
      // 9988776655,4433221100
      icd one(_T("123456.789012345678"));
      icd two(_T("9988776655.4433221100"));
      icd result = one / two;
      result.SetLengthAndPrecision(80,40);
      CString resstring = result.AsString();
      TCHAR* expect = _T("+0.0000123595504505617976514467870219656609");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T018_Operator_modulo)
    {
      Logger::WriteMessage("Test operator %");

      // 123456,789012345678
      // 9988776655,4433221100
      icd one(_T("123456.789012345678"));
      icd two(_T("9988776655.4433221100"));
      icd result = two % one;
      result.SetLengthAndPrecision(40,12);
      CString resstring = result.AsString();
      TCHAR* expect = _T("+11313.243445648698");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T019_Operator_assign_plus)
    {
      Logger::WriteMessage("Test operator +=");

      icd one(_T("123456.789012345678"));
      icd two(_T("9988776655.4433221100"));
      one += two;

      one.SetLengthAndPrecision(40,12);
      CString resstring = one.AsString();
      TCHAR* expect = _T("+9988900112.232334455678");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T020_Operator_assign_min)
    {
      Logger::WriteMessage("Test operator -=");

      // 123456,789012345678
      // 9988776655,4433221100
      icd one(_T("123456.789012345678"));
      icd two(_T("9988776655.4433221100"));
      one -= two;

      one.SetLengthAndPrecision(40,12);
      CString resstring = one.AsString();
      TCHAR* expect = _T("-9988653198.654309764322");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T021_Operator_assign_multiply)
    {
      Logger::WriteMessage("Test operator *=");

      // 123456,789012345678
      // 9988776655,4433221100
      icd one(_T("123456.789012345678"));
      icd two(_T("9988776655.4433221100"));
      one *= two;

      one.SetLengthAndPrecision(40,20);
      CString resstring = one.AsString();
      TCHAR* expect = _T("+1233182292042510.13939532499202034058");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }
 
    TEST_METHOD(T022_Operator_assing_divide)
    {
      Logger::WriteMessage("Test operator /=");

      // 123456,789012345678
      // 9988776655,4433221100
      icd one(_T("123456.789012345678"));
      icd two(_T("9988776655.4433221100"));
      one /= two;

      one.SetLengthAndPrecision(80,40);
      CString resstring = one.AsString();
      TCHAR* expect = _T("+0.0000123595504505617976514467870219656609");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }
 
    TEST_METHOD(T023_Operator_assign_modulo)
    {
      Logger::WriteMessage("Test operator %=");

      // 123456,789012345678
      // 9988776655,4433221100
      icd one(_T("123456.789012345678"));
      icd two(_T("9988776655.4433221100"));
      two %= one;

      two.SetLengthAndPrecision(40,12);
      CString resstring = two.AsString();
      TCHAR* expect = _T("+11313.243445648698");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T024_Operator_negate)
    {
      Logger::WriteMessage("Test operator unary - (negate)");

      // 123456,789012345678
      // 9988776655,4433221100
      icd one(_T("123456.789012345678"));
      icd two = -one;

      two.SetLengthAndPrecision(40,12);
      CString resstring = two.AsString();
      TCHAR* expect = _T("-123456.789012345678");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T025_Operator_postfix_increment)
    {
      Logger::WriteMessage("Test operator icd++ (postfix increment)");

      icd one(_T("1234567890123456789"));
      icd two = one++;
      CString resstring1 = one.AsString();
      CString resstring2 = two.AsString();
      TCHAR* expect1 = _T("+1234567890123456790");
      TCHAR* expect2 = _T("+1234567890123456789");

      Assert::IsTrue(strcmp(expect1,resstring1.GetString()) == 0);
      Assert::IsTrue(strcmp(expect2,resstring2.GetString()) == 0);
    }

    TEST_METHOD(T026_Operator_prefix_increment)
    {
      Logger::WriteMessage("Test operator ++icd (prefix increment)");

      icd one(_T("1234567890123456789"));
      icd two = ++one;
      CString resstring1 = one.AsString();
      CString resstring2 = two.AsString();
      TCHAR* expect1 = _T("+1234567890123456790");
      TCHAR* expect2 = _T("+1234567890123456790");

      Assert::IsTrue(strcmp(expect1,resstring1.GetString()) == 0);
      Assert::IsTrue(strcmp(expect2,resstring2.GetString()) == 0);
    }

    TEST_METHOD(T027_Operator_postfix_decrement)
    {
      Logger::WriteMessage("Test operator icd-- (postfix decrement)");

      icd one(_T("1234567890123456790"));
      icd two = one--;
      CString resstring1 = one.AsString();
      CString resstring2 = two.AsString();
      TCHAR* expect1 = _T("+1234567890123456789");
      TCHAR* expect2 = _T("+1234567890123456790");

      Assert::IsTrue(strcmp(expect1,resstring1.GetString()) == 0);
      Assert::IsTrue(strcmp(expect2,resstring2.GetString()) == 0);
    }

    TEST_METHOD(T028_Operator_prefix_decrement)
    {
      Logger::WriteMessage("Test operator icd-- (postfix decrement)");

      icd one(_T("1234567890123456790"));
      icd two = --one;
      CString resstring1 = one.AsString();
      CString resstring2 = two.AsString();
      TCHAR* expect1 = _T("+1234567890123456789");
      TCHAR* expect2 = _T("+1234567890123456789");

      Assert::IsTrue(strcmp(expect1,resstring1.GetString()) == 0);
      Assert::IsTrue(strcmp(expect2,resstring2.GetString()) == 0);
    }

    TEST_METHOD(T029_Operator_assign_icd)
    {
      Logger::WriteMessage("Test operator icd = icd (assignment operator)");

      icd one(_T("123456.789012345678"));
      icd two = one;

      two.SetLengthAndPrecision(40,12);
      CString resstring = two.AsString();
      TCHAR* expect = _T("+123456.789012345678");

      Assert::AreEqual(expect,resstring.GetString());
    }

    TEST_METHOD(T030_Operator_assign_long)
    {
      Logger::WriteMessage("Test operator icd = long (assignment operator)");

      long one = 123456789;
      icd two(one);
      CString resstring = two.AsString();
      TCHAR* expect = _T("+123456789");

      Assert::AreEqual(expect,resstring.GetString());
    }

    TEST_METHOD(T031_Operator_assign_double)
    {
      Logger::WriteMessage("Test operator icd = double (assignment operator)");

      double one = 123456.123456789;
      icd two(one);

      two.SetLengthAndPrecision(40,9);
      CString resstring = two.AsString();
      TCHAR* expect = _T("+123456.123456789");

      Assert::AreEqual(expect,resstring.GetString());
    }

    TEST_METHOD(T032_Operator_assign_CString)
    {
      Logger::WriteMessage("Test operator icd = CString (assignment operator)");

      CString one(_T("123456.123456789"));
      icd two(one);

      two.SetLengthAndPrecision(40,9);
      CString resstring = two.AsString();
      TCHAR* expect = _T("+123456.123456789");

      Assert::AreEqual(expect,resstring.GetString());
    }

    TEST_METHOD(T033_Operator_comparison_equal)
    {
      Logger::WriteMessage("Test comparison icd == icd");
      Logger::WriteMessage("Test comparison icd >= icd");
      Logger::WriteMessage("Test comparison icd <= icd");

      CString number(_T("8765432123456.123456789"));
      icd one(number);
      icd two(number);

      Assert::IsTrue(one == two);
      Assert::IsTrue(one <= two);
      Assert::IsTrue(one >= two);
    }

    TEST_METHOD(T034_Operator_comparison_notequal)
    {
      Logger::WriteMessage("Test comparison icd != icd");

      CString number1(_T("8765432123456.123456789"));
      CString number2(_T("8765432123456.123476789"));
      icd one(number1);
      icd two(number2);

      Assert::IsTrue(one != two);
    }

    TEST_METHOD(T035_Operator_comparison_smaller)
    {
      Logger::WriteMessage("Test comparison icd < icd");

      CString number1(_T("8765432123456.123456789"));
      CString number2(_T("8765432123456.123476789"));
      icd one(number1);
      icd two(number2);

      Assert::IsTrue(one < two);
    }

    TEST_METHOD(T036_Operator_comparison_smaller)
    {
      Logger::WriteMessage("Test comparison icd > icd");

      CString number1(_T("8765432123456.123456789"));
      CString number2(_T("8765432123456.123476789"));
      icd one(number1);
      icd two(number2);

      Assert::IsTrue(two > one);
    }

    TEST_METHOD(T037_Make_Zero)
    {
      Logger::WriteMessage("Test set to 0 (zero) with: icd.Zero()");

      icd one(_T("12345.898989898342212"));
      icd zero;
      one.Zero();

      Assert::IsTrue(zero == one);
      Assert::IsTrue(zero.IsNull());
      Assert::IsTrue(one.IsNull());
    }

    TEST_METHOD(T038_Fraction)
    {
      Logger::WriteMessage("Test fraction part with: icd.Fraction()");

      icd one(_T("12345.898989898342212"));
      icd expect (_T("0.898989898342212"));
      icd result = one - one.Floor();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T039_Ceiling)
    {
      Logger::WriteMessage("Test ceiling with: icd.Ceiling()");

      icd one(_T("12345.898989898342212"));
      icd expect(12346L);
      icd result = one.Ceil();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T040_SquareRoot)
    {
      Logger::WriteMessage("Test root with: icd.SquareRoot()");

      // 9988776655,4433221100
      icd one(_T("9988776655.4433221100"));
      // SQRT in 40 decimals
      icd expect(_T("+99943.8675229417070510788372162127884572770304"));
      icd result;

      HPFCounter count;
      for(int ind = 0; ind < 1000; ++ind)
      {
        result = one.SquareRoot();
      }
      count.Stop();
      CString speed;
      speed.Format(_T("Speed of the SQUARE ROOT calculation = %.6f"),count.GetCounter());
      Logger::WriteMessage(speed);

      // Original code  : Speed of the SQUARE ROOT calculation = 0.161931
      // FAST SQUAREROOT: Speed of the SQUARE ROOT calculation = 0.641662

      result.SetLengthAndPrecision(80,40);
      CString show = result.AsString();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T040_SquareRoot_0001)
    {
      Logger::WriteMessage("Test root with: icd.SquareRoot()");

      icd one(_T("0.0001"));
      // SQRT in 100 decimals
      icd expect(_T("+0.01"));
      icd result;

      HPFCounter count;
      for (int ind = 0; ind < 1000; ++ind)
      {
        result = one.SquareRoot();
      }
      count.Stop();
      CString speed;
      speed.Format(_T("Speed of the SQUARE ROOT calculation = %.6f"), count.GetCounter());
      Logger::WriteMessage(speed);

      // Original code  : Speed of the SQUARE ROOT calculation = 0.011214
      // FAST SQUAREROOT: Speed of the SQUARE ROOT calculation = 0.262210

      result.SetLengthAndPrecision(80,40);
      CString show = result.AsString();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T041_Power)
    {
      Logger::WriteMessage("Test power with: icd.Power()");
      
      // 9988776655,4433221112
      // 3,78123764321234
      icd one(_T("9988776655.4433221112"));
      icd two(_T("3.78123764321234"));
      // Power in 100 decimals
      icd expect(_T("+64644611887465530217503224573035829885.4640759949343857000872841844317017116778"));
      icd result = one.Power(two);

      result.SetLengthAndPrecision(80,40);
      CString res = result.AsString();
      Logger::WriteMessage(res);
      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T042_Absolute)
    {
      Logger::WriteMessage("Test absolute value with: icd.Absolute()");

      // 9988776655,4433221112
      icd one   (_T("-9988776655.4433221112"));
      icd expect(_T("+9988776655.4433221112"));
      icd result = one.AbsoluteValue();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T043_Reciproke)
    {
      Logger::WriteMessage("Test reciproke value with: icd.Reciproke()");

      // 9988776655,4433221112
      icd one(1L);
      icd value(_T("-9988776655.4433221112"));
      // Reciproke in 100 decimals
      icd expect(_T("-0.000000000100112359550561797740669521525"));
      icd result = one / value;

      result.SetLengthAndPrecision(80,40);
      CString show = result.AsString();
      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T044_Natural_logarithm)
    {
      Logger::WriteMessage("Test logarithm value with: icd.Log()");

      // 9988776655,4433221112
      icd one(_T("9988776655.4433221112"));
      // Log in 100 decimals
      icd expect(_T("+23.0247279651958330529940554435089251603595"));

      icd result = one.Log();

      result.SetLengthAndPrecision(80,40);
      CString show = result.AsString();
      Logger::WriteMessage(show);
      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T045_10_logarithm)
    {
      Logger::WriteMessage("Test 10 logarithm value with: icd.Log10()");

      // 9988776655,4433221112
      icd one(_T("9988776655.4433221112"));
      // Log10 in 100 decimals
      icd expect(_T("+9.9995123026080379948458936511546889761205"));

      icd result = one.Log10();
      result.SetLengthAndPrecision(80,40);
      CString show = result.AsString();
      Logger::WriteMessage(show);
      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T046_e_power)
    {
      Logger::WriteMessage("Test e-power value with: icd.Exp()");

      // 99,887766554433221112
      icd one(_T("9.9887766554433221112"));
      // Exp in 100 decimals
      icd expect(_T("+21780.6372693926259028515018562364753145706029"));
      
      icd result = one.Exp();
      result.SetLengthAndPrecision(80,40);
      CString show = result.AsString();
      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T047_10th_power)
    {
      Logger::WriteMessage("Test 10th-power value with: icd.TenPower()");

      // 99,887766554433221112
      icd one(_T("99.887766554433221112"));
      icd expect(_T("998877665.54433221112"));
      icd result = one.TenPower(7);

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T048_Round)
    {
      Logger::WriteMessage("Test rounding of a number with : icd.Round(5)");
      
      // 9988776655,4433277112
      icd one(_T("9988776655.4433277112"));
      one.Rounding(20,5);
      icd expect(_T("+9988776655.44333"));
      CString test = one.AsString();

      Assert::IsTrue(expect == one);

      one = _T("9988776655.64433277112");
      expect = _T("+9988776655.644333");
      one.Rounding(20,6);
      Assert::IsTrue(expect == one);

      one = _T("9988776655.64433277112");
      expect = _T("+9988776656");
      one.Rounding(20,0);
      Assert::IsTrue(expect == one);
    }

    TEST_METHOD(T049_Truncate)
    {
      Logger::WriteMessage("Test truncation of a number with : icd.Truncate(5)");
      
      // 9988776655,4433277112
      icd one(_T("9988776655.4433277112"));
      one.SetLengthAndPrecision(20,5);
      icd expect(_T("9988776655.44332"));

      Assert::IsTrue(expect == one);

      one = _T("9988776655.64433277112");
      expect = _T("9988776655");
      one.SetLengthAndPrecision(20,0);
      Assert::IsTrue(expect == one);
    }

    TEST_METHOD(T050_Sine)
    {
      Logger::WriteMessage("Test Sine function of a radian with : icd.Sine()");

      // 0,9876543210123456
      TCHAR* a_angle = _T("0.9876543210123456");
      // Sine in 100 decimals
      TCHAR* r_sine  = _T("+0.8347366295099261173476093153791068840878");

      icd one(a_angle);
      icd expect(r_sine);
      icd result = one.Sine();

      result.SetLengthAndPrecision(80,40);
      CString show = result.AsString();
      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T051_Cosine)
    {
      Logger::WriteMessage("Test Cosine function of a radian with : icd.Cosine()");

      // 0,9876543210123456
      TCHAR* a_angle  = _T("0.9876543210123456");
      // Cosine in 100 decimals
      TCHAR* r_cosine = _T("+0.5506493978516714425340844117564672940295");

      icd one(a_angle);
      icd expect(r_cosine);
      icd result = one.Cosine();

      result.SetLengthAndPrecision(80,40);
      CString show = result.AsString();
      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T052_Tangent)
    {
      Logger::WriteMessage("Test Tangent function of a radian with : icd.Tangent()");

      // 0,9876543210123456
      TCHAR* a_angle   = _T("0.9876543210123456");
      // Tangent in 100 decimals
      TCHAR* r_tangent = _T("+1.5159130887395963684392407742872343024204");

      icd one(a_angle);
      icd expect(r_tangent);
      icd result = one.Tangent();

      result.SetLengthAndPrecision(80,40);
      CString show = result.AsString();
      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T053_ArcSine)
    {
      Logger::WriteMessage("Test ArcSine function of a ratio with : icd.ArcSine()");

      TCHAR* a_ratio = _T("0.765498765404321098765");
      // ArcSine in 100 decimals
      TCHAR* r_asine = _T("+0.8718161310705591010249460213430371705786");

      icd one(a_ratio);
      icd expect(r_asine);
      icd result = one.ArcSine();

      result.SetLengthAndPrecision(80,40);
      CString show = result.AsString();
      Logger::WriteMessage(show);
      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T054_ArcCosine)
    {
      Logger::WriteMessage("Test ArcCosine function of a ratio with : icd.ArcCosine()");

      TCHAR* a_ratio   = _T("0.765498765404321098765");
      // ArcCosine in 100 decimals
      TCHAR* r_acosine = _T("+0.69898019572433751820637567029671427152");

      icd one(a_ratio);
      icd expect(r_acosine);
      icd result = one.ArcCosine();

      result.SetLengthAndPrecision(80,40);
      CString show = result.AsString();
      Logger::WriteMessage(show);
      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T055_ArcTangent)
    {
      Logger::WriteMessage("Test ArcTangent function of a ratio with : icd.ArcTangent()");

      TCHAR* a_ratio    = _T("0.765498765404321098765");
      // ArcTangent in 100 decimals
      TCHAR* r_atangent = _T("+0.6533467523844312707494031091720016391906");

      icd one(a_ratio);
      icd expect(r_atangent);
      icd result = one.ArcTangent();

      result.SetLengthAndPrecision(80,40);
      CString show = result.AsString();
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

      icd one(&num);
      one.AsNumeric(&res);

      icd result(&res);

      Assert::IsTrue(one == result);
      Assert::IsTrue(one == icd(_T("10.001")));
    }

    TEST_METHOD(T057_AsDouble)
    {
      Logger::WriteMessage("Test as-other-datatype: icd.AsDouble()");

      icd one(_T("99887766.554433"));
      double result = one.AsDouble();
      double expect = 99887766.554433;

      Assert::IsTrue(abs(expect - result) < 0.00005);
    }

    TEST_METHOD(T058_AsShort)
    {
      Logger::WriteMessage("Test as-other-datatype: icd.AsShort()");

      icd one(_T("4433"));
      short result = one.AsShort();
      short expect = 4433;

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T059_AsUShort)
    {
      Logger::WriteMessage("Test as-other-datatype: icd.AsUShort()");

      icd one(_T("54873"));
      unsigned short result = one.AsUShort();
      unsigned short expect = 54873;

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T060_AsLong)
    {
      Logger::WriteMessage("Test as-other-datatype: icd.AsLong()");

      icd one(_T("98854873"));
      long result = one.AsLong();
      long expect = 98854873;

      Assert::IsTrue(expect == result);
      Assert::IsTrue(one.FitsInLong());
    }

    TEST_METHOD(T061_AsULong)
    {
      Logger::WriteMessage("Test as-other-datatype: icd.AsULong()");

      icd one(_T("2974276950"));
      unsigned long result = one.AsULong();
      unsigned long expect = 2974276950;

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T062_AsInt64)
    {
      Logger::WriteMessage("Test as-other-datatype: icd.AsInt64()");

      icd one(_T("23154765019"));
      __int64 result = one.AsInt64();
      __int64 expect = 23154765019L;

      Assert::IsTrue(expect == result);
      Assert::IsTrue(one.FitsInInt64());
    }

    TEST_METHOD(T063_Operator_assign_long)
    {
      Logger::WriteMessage("Test operator = long");

      // 123456,789012345678
      // 9988776655,4433221100
      long one = 1234567890L;
      icd result;
      result = one;
      CString resstring = result.AsString();
      TCHAR* expect = _T("+1234567890");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T064_Operator_assign_double)
    {
      Logger::WriteMessage("Test operator = double");

      // 123456,789012345678
      // 9988776655,4433221100
      double one = 1234567890.5432L;
      icd result;
      result = one;

      result.SetLengthAndPrecision(20,4);
      CString resstring = result.AsString();
      TCHAR* expect = _T("+1234567890.5432");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

//     TEST_METHOD(T065_ArcTangent2)
//     {
//       Logger::WriteMessage("Test ArcTangent2Points function of icd with icd::ArcTangent2Points");
// 
//       // 1.0456788
//       // 7.89991232E-1
// 
//       icd one(_T("1.0456788"));
//       icd two(_T("7.89991232E-1"));
// 
//       icd result = one.ArcTangent2Points(two);
//       CString resstring = result.AsString();
//       // ArcTangent2 in 100 decimals // 0.647000196692588
//       TCHAR* expect = _T("+0.6470001966925876");
// 
//       result.SetLengthAndPrecision(200,100);
//       CString show = result.AsString();
//       Assert::IsTrue(strcmp(expect,show) == 0);
//     }
    
    TEST_METHOD(T066_Negate)
    {
      Logger::WriteMessage("Test the Negate method for unary -");

      icd one(_T("45523.89901"));
      one.Negate();

      one.SetLengthAndPrecision(20,5);
      CString resstring = one.AsString();
      TCHAR* expect = _T("-45523.89901");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }
    
    TEST_METHOD(T067_IsNull)
    {
      Logger::WriteMessage("Test the status function IsNull");

      icd one;
      icd two(12L);

      Assert::IsTrue(one.IsNull());
      Assert::IsFalse(two.IsNull());
    }

    TEST_METHOD(T068_GetSign)
    {
      Logger::WriteMessage("Test the status function GetSign");

      icd zero;
      icd one(12L);
      icd two(-12L);

      Assert::IsTrue(zero.GetSign() ==  0);
      Assert::IsTrue(one.GetSign()  ==  1);
      Assert::IsTrue(two.GetSign()  == -1);
    }

    TEST_METHOD(T069_GetLength)
    {
      Logger::WriteMessage("Test the status function GetLength");

      // 123456,789012345678
      icd one(_T("+123456.789012345678"));
      one.SetLengthAndPrecision(21, 12);
      Assert::IsTrue(one.Length() == 21);
    }

    TEST_METHOD(T070_GetPrecision)
    {
      Logger::WriteMessage("Test the status function GetPrecision");

      // 123456,789012345678
      icd one(_T("123456.789012345678"));
      one.SetLengthAndPrecision(21, 12);
      Assert::IsTrue(one.Precision() == 12);
    }

    TEST_METHOD(T071_GetMaxSize)
    {
      Logger::WriteMessage("Test the status function GetMaxSize");

      // 123456,789012345678
      icd one(_T("123456.789012345678"));
      Assert::IsTrue(one.GetMaxSize() == 80);
    }

    TEST_METHOD(T072_GetHasDecimals)
    {
      Logger::WriteMessage("Test the status function GetHasDecimals");

      // 123456,789012345678
      icd one(_T("123456.789012345678"));
      icd two(_T("877234"));
      icd three(_T("123.9"));
      Assert::IsTrue  (one.HasDecimals());
      Assert::IsFalse (two.HasDecimals());
      Assert::IsTrue(three.HasDecimals());
    }

    TEST_METHOD(T073_GetExponent)
    {
      Logger::WriteMessage("Test the function GetExponent");

      // 123456,789012345678
      icd one(_T("123456.789012345678"));
      icd two(_T("877234"));
      icd three(_T("123.9"));
      icd four(_T("0.0066"));

      Assert::IsTrue(one  .Exponent() == 5);
      Assert::IsTrue(two  .Exponent() == 5);
      Assert::IsTrue(three.Exponent() == 2);
      Assert::IsTrue(four .Exponent() == -3);
    }

    TEST_METHOD(T074_GetMantissa)
    {
      Logger::WriteMessage("Test the function GetMantissa");

      // 123456,789012345678
      icd one(_T("123456.789012345678"));
      icd two(_T("123.9"));

      icd mant1 = one.Mantissa();
      icd mant2 = two.Mantissa();

      mant1.SetLengthAndPrecision(20,17);
      mant2.SetLengthAndPrecision(20,3);

      CString m1res = mant1.AsString();
      CString m2res = mant2.AsString();

      CString expect1(_T("+1.23456789012345678"));
      CString expect2(_T("+1.239"));

      Assert::AreEqual(expect1.GetString(),m1res.GetString());
      Assert::AreEqual(expect2.GetString(),m2res.GetString());
    }

//     TEST_METHOD(T075_ReadWriteFile)
//     {
//       Logger::WriteMessage("Test writing to file");
// 
//       // 123456,789012345678
//       icd one(_T("123456.789012345678"));
//       FILE* file = nullptr;
//       _tfopen_s(&file,_T("C:\\TMP\\icd.test"),_T("wb"));
//       if(file)
//       {
//         one.WriteToFile(file);
//         fclose(file);
//       }
// 
//       icd result;
//       Logger::WriteMessage("Test reading from file");
//       file = nullptr;
//       _tfopen_s(&file,_T("C:\\TMP\\icd.test"),_T("rb"));
//       if(file)
//       {
//         result.ReadFromFile(file);
//         fclose(file);
//       }
// 
//       Assert::IsTrue(one == result);
//     }

    TEST_METHOD(T076_math_floor)
    {
      Logger::WriteMessage("Test math style floor");

      // 123456,789012345678
      icd one(_T("123456.789012345678"));
      icd result = floor(one);
      result.SetLengthAndPrecision(20,2);
      CString resstring = result.AsString();
      TCHAR* expect(_T("+123456"));

      Assert::AreEqual(expect,resstring.GetString());
    }

    TEST_METHOD(T077_math_ceil)
    {
      Logger::WriteMessage("Test math style ceil");

      // 123456,789012345678
      icd one(_T("123456.789012345678"));
      icd result = ceil(one);
      CString resstring = result.AsString();
      TCHAR* expect(_T("+123457"));

      Assert::AreEqual(expect,resstring.GetString());
    }

    TEST_METHOD(T078_AsDisplayString)
    {
      Logger::WriteMessage("Test AsDisplayString");

      // BEWARE: I am testing this in the Dutch locale
      // Thousands separator is a decimal point
      // Decimals  separator is a comma

      // 123456,789012345678
      icd one(_T("123456.789012345678"));
      one.Rounding(8, 2);
      CString result = one.AsDisplayString();
      TCHAR* expect = _T("+123.456,79");
      Assert::AreEqual(expect,result.GetString());

      icd two(_T("9123456.789012345678"));
      two.Rounding(9,2);
      CString result2 = two.AsDisplayString();
      TCHAR* expect2 = _T("+9.123.456,79");
      Assert::AreEqual(expect2,result2.GetString());
    }

    TEST_METHOD(T079_Sine_zero)
    {
      Logger::WriteMessage("Test Sine(0.0)");

      // Reputationed to enter and endless loop
      icd zero;
      icd sineZero = zero.Sine();

      CString zeroResult(_T("+0"));
      CString zeroSine = sineZero.AsString();

      Assert::AreEqual(zeroSine.GetString(),zeroResult.GetString());
    }

    TEST_METHOD(T080_Cosine_zero)
    {
      Logger::WriteMessage("Test Cosine(0.0)");

      // Reputationed to enter and endless loop
      icd zero;
      icd cosineZero = zero.Cosine();

      CString zeroResult(_T("+1"));
      CString zeroCosine = cosineZero.AsString();

      Assert::AreEqual(zeroCosine.GetString(), zeroResult.GetString());
    }

    TEST_METHOD(T081_Exponent_zero)
    {
      Logger::WriteMessage("Test Exp(0.0)");

      // Reputationed to enter and endless loop
      icd zero;
      icd expZero = zero.Exp();

      CString zeroResult(_T("+1"));
      CString zeroExponent = expZero.AsString();

      Assert::AreEqual(zeroExponent.GetString(), zeroResult.GetString());
    }

    TEST_METHOD(T082_Sine_pi)
    {
      Logger::WriteMessage("Test Sine(180 degrees)");

      // Reputationed to enter and endless loop
      icd valuepi = icd::PI();
      icd sinepi = valuepi.Sine();

      Assert::IsTrue(sinepi.IsNearZero());
    }

    TEST_METHOD(T083_Cosine_pi)
    {
      Logger::WriteMessage("Test Cosine(180 degrees)");

      // Reputationed to enter and endless loop
      icd valuepi = icd::PI();
      icd cosinepi = valuepi.Cosine();
      cosinepi += icd(1L);

      Assert::IsTrue(cosinepi.IsNearZero());
    }

    TEST_METHOD(T084_Exponents1)
    {
      Logger::WriteMessage("Test various exponents");

      icd number(_T("0.01"));
      icd expnumber = number.Exp();
      expnumber.Rounding(16,15);
      double expdouble = ::exp(0.01);
      CString expected(_T("+1.010050167084168"));

      CString icdResult;
      CString dblResult;
      icdResult = expnumber.AsString();
      dblResult.Format(_T("+%0.16g"), expdouble);

      // Be sure to eliminate the locale differences
      icdResult.Replace(_T(","), _T("."));
      dblResult.Replace(_T(","), _T("."));

      Assert::AreEqual(expected.GetString(),dblResult.GetString());
      Assert::AreEqual(expected.GetString(),icdResult.GetString());
    }

    TEST_METHOD(T085_Exponents2)
    {
      Logger::WriteMessage("Test various exponents");

      icd number(_T("0.03"));
      icd expnumber = number.Exp();
      expnumber.Rounding(16,15);
      double expdouble = ::exp(0.03);
      CString expected(_T("+1.030454533953517"));

      CString bcdResult;
      CString dblResult;
      bcdResult = expnumber.AsString();
      dblResult.Format(_T("+%0.16g"), expdouble);

      // Be sure to eliminate the locale differences
      bcdResult.Replace(_T(","), _T("."));
      dblResult.Replace(_T(","), _T("."));

      Assert::AreEqual(expected.GetString(), dblResult.GetString());
      Assert::AreEqual(expected.GetString(), bcdResult.GetString());
    }

    TEST_METHOD(T086_Divide10)
    {
      Logger::WriteMessage("Test division by 10");

      icd number(_T("0.1"));
      icd numresult = number / 10.0;

      numresult.SetLengthAndPrecision(3, 2);
      CString expected(_T("+0.01"));
      CString result = numresult.AsString();
      
      Assert::AreEqual(expected.GetString(),result.GetString());
    }

    TEST_METHOD(T087_Divide10_Wrong)
    {
      Logger::WriteMessage("Test division by 10 wrong?");

      icd numresult(0.2 / 10.0);

      numresult.SetLengthAndPrecision(3,2);
      CString expected(_T("+0.02"));
      CString result = numresult.AsString();

#ifdef _DEBUG
//       CString mantissa = numresult.DebugPrint("Correct 0.02");
//       Logger::WriteMessage(mantissa);
#endif

      Assert::AreEqual(expected.GetString(), result.GetString());
    }

    TEST_METHOD(T088_Exponent_zero)
    {
      Logger::WriteMessage("Test exp(0.0)");

      double expected = ::exp(0.0);
      icd    result = icd(0L).Exp();

      Assert::AreEqual(expected, result.AsDouble());
    }

    TEST_METHOD(T089_Exponent_negative)
    {
      Logger::WriteMessage("Test exponent of negative number");

      double dblres = ::exp(-2.5);
      icd icdres = icd(_T("-2.5")).Exp();
      icdres.Rounding(20,16);

      CString expected(_T("+0.0820849986238988"));

      CString bcdResult;
      CString dblResult;
      bcdResult = icdres.AsString();
      dblResult.Format(_T("+%0.15g"), dblres);

      // Be sure to eliminate the locale differences
      bcdResult.Replace(_T(","),_T("."));
      dblResult.Replace(_T(","),_T("."));

      Assert::AreEqual(expected.GetString(), dblResult.GetString());
      Assert::AreEqual(expected.GetString(), bcdResult.GetString());
    }

    TEST_METHOD(T090_Adding_negative_small)
    {
      Logger::WriteMessage("Adding small negative number to 0.01");

      icd tinynum(_T("0.01"));
      icd number(_T("-0.05"));

      icd result = tinynum + number;
      result.SetLengthAndPrecision(3,2);
      CString expected(_T("-0.04"));
      CString resstring = result.AsString();

      Assert::AreEqual(expected.GetString(),resstring.GetString());
    }

    TEST_METHOD(T091_Adding_negative)
    {
      Logger::WriteMessage("Adding small negative number to 0.0");

      // Original values provided by sisirajaya
      icd tinynum(_T("0.0"));
      icd number(_T("-0.05"));

      icd result = tinynum + number;
      result.SetLengthAndPrecision(3,2);
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
//         icd b = val;
//         __int64 back = b.AsInt64();
//         if(back != val)
//         {
//           Assert::Fail(L"icd <-> integer64 just broke!");
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

      icd one(_T("94505024884"));
      CString expected(_T("+94505024884"));
      UINT64 number = one.AsUInt64();
      CString result;
      result.Format(_T("+%I64d"),number);

      Assert::AreEqual(expected.GetString(),result.GetString());
    }

    TEST_METHOD(T094_Int64PlusDecimals)
    {
      Logger::WriteMessage("Testing int64 plus decimals.");

      UINT64 num = 94505024884;
      icd one (num,64000000);
      CString expected(_T("+94505024884.64"));
      one.SetLengthAndPrecision(20,2);
      CString result = one.AsString();

      Assert::AreEqual(expected.GetString(),result.GetString());
    }

    TEST_METHOD(T095_CornerCases)
    {
      Logger::WriteMessage("Testing integer corner cases.");

      icd test1((char)MININT8);      Assert::AreEqual((int)MININT8,  (int)test1.AsShort());
      icd test2((char)MAXINT8);      Assert::AreEqual((int)MAXINT8,  (int)test2.AsShort()); 

      icd test3((uchar)0);           Assert::AreEqual(0,             (int)test3.AsShort()); 
      icd test4((uchar)MAXUINT8);    Assert::AreEqual((int)MAXUINT8, (int)test4.AsShort());

      icd test5((short)MININT16);    Assert::AreEqual((int)MININT16, (int)test5.AsShort()); 
      icd test6((short)MAXINT16);    Assert::AreEqual((int)MAXINT16, (int)test6.AsShort()); 

      icd test7((ushort)0);          Assert::AreEqual((int)0,        (int)test7.AsUShort()); 
      icd test8((ushort)MAXUINT16);  Assert::AreEqual((int)MAXUINT16,(int)test8.AsUShort()); 

      icd test9((long)MININT32);     Assert::AreEqual((int)MININT32, (int)test9.AsLong());
      icd test10((long)MAXINT32);    Assert::AreEqual((int)MAXINT32, (int)test10.AsLong()); 

      icd test11((ulong)0);          Assert::IsTrue(test11.AsULong() == 0); 
      icd test12((ulong)MAXUINT32);  Assert::IsTrue(test12.AsULong() == MAXUINT32); 

      icd test13((INT64)MININT64+1); Assert::IsTrue(test13.AsInt64() == (MININT64 + 1)); 
      icd test14((INT64)MAXINT64);   Assert::IsTrue(test14.AsInt64() == MAXINT64); 

      icd test15((UINT64)0);         Assert::IsTrue(test15.AsUInt64() == 0L); 
      icd test16((UINT64)MAXUINT64); Assert::IsTrue(test16.AsUInt64() == MAXUINT64);
    }

    TEST_METHOD(T096_TestNaturalLog)
    {
      Logger::WriteMessage("Testing BCD Natural Log 0.5");

      icd value(_T("0.5"));
      icd nlog = value.Log();

      CString expected = _T("-0.6931471805599453094172321214581765680714");
      nlog.SetLengthAndPrecision(80,40);
      CString testval  = nlog.AsString();

      Assert::AreEqual(expected.GetString(), testval.GetString());
    }

    TEST_METHOD(T097_CalculatePI)
    {
      Logger::WriteMessage("Testing BCD calculating PI");
      icd value("-1");
      icd pi = value.ArcCosine();
      icd internalPi = icd::PI();

      pi.RoundDecimals(80,32);
      internalPi.RoundDecimals(80,32);

      pi.SetLengthAndPrecision(40,32);
      internalPi.SetLengthAndPrecision(40,32);

      CString disp = pi.AsString();
      CString intr = internalPi.AsString();

      icd diff = pi - internalPi;
      Assert::IsTrue(diff.IsNearZero());
    }

    TEST_METHOD(T098_NaturalLog2)
    {
      Logger::WriteMessage("Testing BCD Natural Log 2");
      icd value("2");
      icd ln2      = value.Log();
      icd internal = icd::LN2();

      ln2.Rounding(40, 32);
      internal.Rounding(40,32);

      icd diff = ln2 - internal;
      Assert::IsTrue(diff.IsNearZero());
    }

    TEST_METHOD(T099_NaturalLog10)
    {
      Logger::WriteMessage("Testing ICD Natural Log 10");
      icd value("10");
      icd log = value.Log();
      icd internal = icd::LN10();

      log.Rounding(40,30);
      internal.Rounding(40,30);

      icd diff = log - internal;
      Assert::IsTrue(diff.IsNearZero());
    }
  };
}
