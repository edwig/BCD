#include "stdafx.h"
#include "CppUnitTest.h"
#include "icd208.h"
#include "HPFCounter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(UnitTestICD208)
	{
	public:
		
    TEST_METHOD(T001_Constructor01)
    {
      Logger::WriteMessage("Construct icd208");
      icd208 one;
      Assert::IsTrue(one.IsNull());
    }

		TEST_METHOD(T002_Constructor02)
		{
      Logger::WriteMessage("Construct icd208 from an 1 byte number");

      icd208  one((TCHAR)'\n');
      long expect(10);

      Assert::AreEqual(expect,one.AsLong());
		}

    TEST_METHOD(T003_Constructor03)
    {
      Logger::WriteMessage("Construct a icd208 from a short int");

      short expect(12);
      icd208   one(expect);

      Assert::AreEqual((long)expect,one.AsLong());
    }

    TEST_METHOD(T004_Constructor04)
    {
      Logger::WriteMessage("Construct a icd208 from a long");

      long expect(765432198);
      icd208  one(expect);

      Assert::AreEqual((long)expect,one.AsLong());
    }

    TEST_METHOD(T005_Constructor05)
    {
      Logger::WriteMessage("Construct a icd208 from a long + remainder");

      long base(765432198);
      long remainder(50000000);
      icd208  one(base,remainder);
      double expect = 765432198.5;
      double result = one.AsDouble();
      expect = trunc(expect * 10) / 10;
      result = trunc(result * 10) / 10;

      Assert::AreEqual(expect,result);
    }

    TEST_METHOD(T006_Constructor06)
    {
      Logger::WriteMessage("Construct a icd208 from an __int64");

      __int64 expect(76543219876543210LL);
      icd208  one( expect);
      __int64 result = one.AsInt64();

      Assert::IsTrue(result - expect == 0);
    }

    TEST_METHOD(T007_Constructor07)
    {
      Logger::WriteMessage("Construct a icd208 from an unsigned __int64");

      unsigned __int64 expect(76543219876543210LL);
      icd208  one(_T("76543219876543210"));
      unsigned __int64 result = one.AsUInt64();

      Assert::IsTrue(result - expect == 0);
    }

    TEST_METHOD(T008_Constructor08)
    {
      Logger::WriteMessage("Construct a icd208 from another icd208");

      icd208 one(12345L);
      icd208 two(one);

      Assert::IsTrue(one.AsLong() - two.AsLong() == 0);
    }

    TEST_METHOD(T009_Constructor09)
    {
      Logger::WriteMessage("Construct a icd208 from a double");

      double expect = 765432198.5;
      icd208  one(expect);
      double result = one.AsDouble();
      expect = trunc(expect * 10) / 10;
      result = trunc(result * 10) / 10;

      Assert::AreEqual(expect,result);
    }

    TEST_METHOD(T010_Constructor10)
    {
      Logger::WriteMessage("Construct a icd208 from a CString");

      CString large(_T("76543219876543210"));
      unsigned __int64 expect(76543219876543210LL);
      icd208  one(large);
      unsigned __int64 result = one.AsUInt64();

      Assert::IsTrue(result - expect == 0);
    }

    TEST_METHOD(T011_Constant_PI)
    {
      Logger::WriteMessage("Test the constant of PI");
      icd208 pi = icd208::PI();
      CString pi_string = pi.AsString();
      // First 104 decimals of PI
      CString expect(_T("+3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798215"));

      Assert::AreEqual(expect.GetString(),pi_string.GetString());
    }

    TEST_METHOD(T012_Constant_LN2)
    {
      Logger::WriteMessage("Test the constant LN2");

      icd208 ln2 = icd208::LN2();
      CString ln2_string = ln2.AsString();
      // LN2 in 104 decimals
      CString expect(_T("+0.693147180559945309417232121458176568075500134360255254120680009493393621969694715605863326996418687542"));

      Assert::AreEqual(expect.GetString(),ln2_string.GetString());
    }

    TEST_METHOD(T013_Constant_LN10)
    {
      Logger::WriteMessage("Test the constant LN10");

      icd208 ln10 = icd208::LN10();
      CString ln10_string = ln10.AsString();
      // LN10 in 104 decimals
      CString expect(_T("+2.30258509299404568401799145468436420760110148862877297603332790096757260967735248023599720508959829834197"));

      Assert::AreEqual(expect.GetString(),ln10_string.GetString());
    }

    TEST_METHOD(T014_Operator_plus)
    {
      Logger::WriteMessage("Test operator +");

      icd208 one(_T("123456.789012345678"));
      icd208 two(_T("9988776655.4433221100"));
      icd208 result = one + two;
      result.SetLengthAndPrecision(100, 12);
      CString resstring = result.AsString();
      TCHAR* expect = _T("+9988900112.232334455678");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T015_Operator_min)
    {
      Logger::WriteMessage("Test operator -");

      // 123456,789012345678
      // 9988776655,4433221100
      icd208 one(_T("123456.789012345678"));
      icd208 two(_T("9988776655.4433221100"));
      icd208 result = one - two;
      result.SetLengthAndPrecision(100, 12);
      CString resstring = result.AsString();
      TCHAR* expect = _T("-9988653198.654309764322");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T016_Operator_multiply)
    {
      Logger::WriteMessage("Test operator *");

      // 123456,789012345678
      // 9988776655,4433221100
      icd208 one(_T("123456.789012345678"));
      icd208 two(_T("9988776655.4433221100"));
      icd208 result = one * two;
      result.SetLengthAndPrecision(100,20);
      CString resstring = result.AsString();
      TCHAR* expect = _T("+1233182292042510.13939532499202034058");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T017_Operator_divide)
    {
      Logger::WriteMessage("Test operator /");

      // 123456,789012345678
      // 9988776655,4433221100
      icd208 one(_T("123456.789012345678"));
      icd208 two(_T("9988776655.4433221100"));
      icd208 result = one / two;
      result.SetLengthAndPrecision(100, 100);
      CString resstring = result.AsString();
      TCHAR* expect = _T("+0.0000123595504505617976514467870219656609015152283291888026282005446872705743790694375902390218265403");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T018_Operator_modulo)
    {
      Logger::WriteMessage("Test operator %");

      // 123456,789012345678
      // 9988776655,4433221100
      icd208 one(_T("123456.789012345678"));
      icd208 two(_T("9988776655.4433221100"));
      icd208 result = two % one;
      result.SetLengthAndPrecision(100,12);
      CString resstring = result.AsString();
      TCHAR* expect = _T("+11313.243445648698");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T019_Operator_assign_plus)
    {
      Logger::WriteMessage("Test operator +=");

      icd208 one(_T("123456.789012345678"));
      icd208 two(_T("9988776655.4433221100"));
      one += two;

      one.SetLengthAndPrecision(100,12);
      CString resstring = one.AsString();
      TCHAR* expect = _T("+9988900112.232334455678");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T020_Operator_assign_min)
    {
      Logger::WriteMessage("Test operator -=");

      // 123456,789012345678
      // 9988776655,4433221100
      icd208 one(_T("123456.789012345678"));
      icd208 two(_T("9988776655.4433221100"));
      one -= two;

      one.SetLengthAndPrecision(100,12);
      CString resstring = one.AsString();
      TCHAR* expect = _T("-9988653198.654309764322");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T021_Operator_assign_multiply)
    {
      Logger::WriteMessage("Test operator *=");

      // 123456,789012345678
      // 9988776655,4433221100
      icd208 one(_T("123456.789012345678"));
      icd208 two(_T("9988776655.4433221100"));
      one *= two;

      one.SetLengthAndPrecision(100,20);
      CString resstring = one.AsString();
      TCHAR* expect = _T("+1233182292042510.13939532499202034058");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }
 
    TEST_METHOD(T022_Operator_assing_divide)
    {
      Logger::WriteMessage("Test operator /=");

      // 123456,789012345678
      // 9988776655,4433221100
      icd208 one(_T("123456.789012345678"));
      icd208 two(_T("9988776655.4433221100"));
      one /= two;

      one.SetLengthAndPrecision(100, 100);
      CString resstring = one.AsString();
      TCHAR* expect = _T("+0.0000123595504505617976514467870219656609015152283291888026282005446872705743790694375902390218265403");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }
 
    TEST_METHOD(T023_Operator_assign_modulo)
    {
      Logger::WriteMessage("Test operator %=");

      // 123456,789012345678
      // 9988776655,4433221100
      icd208 one(_T("123456.789012345678"));
      icd208 two(_T("9988776655.4433221100"));
      two %= one;

      two.SetLengthAndPrecision(100,12);
      CString resstring = two.AsString();
      TCHAR* expect = _T("+11313.243445648698");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T024_Operator_negate)
    {
      Logger::WriteMessage("Test operator unary - (negate)");

      // 123456,789012345678
      // 9988776655,4433221100
      icd208 one(_T("123456.789012345678"));
      icd208 two = -one;

      two.SetLengthAndPrecision(100,12);
      CString resstring = two.AsString();
      TCHAR* expect = _T("-123456.789012345678");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T025_Operator_postfix_increment)
    {
      Logger::WriteMessage("Test operator icd208++ (postfix increment)");

      icd208 one(_T("1234567890123456789"));
      icd208 two = one++;
      CString resstring1 = one.AsString();
      CString resstring2 = two.AsString();
      TCHAR* expect1 = _T("+1234567890123456790");
      TCHAR* expect2 = _T("+1234567890123456789");

      Assert::IsTrue(strcmp(expect1,resstring1.GetString()) == 0);
      Assert::IsTrue(strcmp(expect2,resstring2.GetString()) == 0);
    }

    TEST_METHOD(T026_Operator_prefix_increment)
    {
      Logger::WriteMessage("Test operator ++icd208 (prefix increment)");

      icd208 one(_T("1234567890123456789"));
      icd208 two = ++one;
      CString resstring1 = one.AsString();
      CString resstring2 = two.AsString();
      TCHAR* expect1 = _T("+1234567890123456790");
      TCHAR* expect2 = _T("+1234567890123456790");

      Assert::IsTrue(strcmp(expect1,resstring1.GetString()) == 0);
      Assert::IsTrue(strcmp(expect2,resstring2.GetString()) == 0);
    }

    TEST_METHOD(T027_Operator_postfix_decrement)
    {
      Logger::WriteMessage("Test operator icd208-- (postfix decrement)");

      icd208 one(_T("1234567890123456790"));
      icd208 two = one--;
      CString resstring1 = one.AsString();
      CString resstring2 = two.AsString();
      TCHAR* expect1 = _T("+1234567890123456789");
      TCHAR* expect2 = _T("+1234567890123456790");

      Assert::IsTrue(strcmp(expect1,resstring1.GetString()) == 0);
      Assert::IsTrue(strcmp(expect2,resstring2.GetString()) == 0);
    }

    TEST_METHOD(T028_Operator_prefix_decrement)
    {
      Logger::WriteMessage("Test operator icd208-- (postfix decrement)");

      icd208 one(_T("1234567890123456790"));
      icd208 two = --one;
      CString resstring1 = one.AsString();
      CString resstring2 = two.AsString();
      TCHAR* expect1 = _T("+1234567890123456789");
      TCHAR* expect2 = _T("+1234567890123456789");

      Assert::IsTrue(strcmp(expect1,resstring1.GetString()) == 0);
      Assert::IsTrue(strcmp(expect2,resstring2.GetString()) == 0);
    }

    TEST_METHOD(T029_Operator_assign_bcd)
    {
      Logger::WriteMessage("Test operator icd208 = icd208 (assignment operator)");

      icd208 one(_T("123456.789012345678"));
      icd208 two = one;

      two.SetLengthAndPrecision(100,12);
      CString resstring = two.AsString();
      TCHAR* expect = _T("+123456.789012345678");

      Assert::AreEqual(expect,resstring.GetString());
    }

    TEST_METHOD(T030_Operator_assign_long)
    {
      Logger::WriteMessage("Test operator icd208 = long (assignment operator)");

      long one = 123456789;
      icd208 two(one);
      CString resstring = two.AsString();
      TCHAR* expect = _T("+123456789");

      Assert::AreEqual(expect,resstring.GetString());
    }

    TEST_METHOD(T031_Operator_assign_double)
    {
      Logger::WriteMessage("Test operator icd208 = double (assignment operator)");

      double one = 123456.123456789;
      icd208 two(one);

      two.SetLengthAndPrecision(100,9);
      CString resstring = two.AsString();
      TCHAR* expect = _T("+123456.123456789");

      Assert::AreEqual(expect,resstring.GetString());
    }

    TEST_METHOD(T032_Operator_assign_CString)
    {
      Logger::WriteMessage("Test operator icd208 = CString (assignment operator)");

      CString one(_T("123456.123456789"));
      icd208 two(one);

      two.SetLengthAndPrecision(100,9);
      CString resstring = two.AsString();
      TCHAR* expect = _T("+123456.123456789");

      Assert::AreEqual(expect,resstring.GetString());
    }

    TEST_METHOD(T033_Operator_comparison_equal)
    {
      Logger::WriteMessage("Test comparison icd208 == icd208");
      Logger::WriteMessage("Test comparison icd208 >= icd208");
      Logger::WriteMessage("Test comparison icd208 <= icd208");

      CString number(_T("8765432123456.123456789"));
      icd208 one(number);
      icd208 two(number);

      Assert::IsTrue(one == two);
      Assert::IsTrue(one <= two);
      Assert::IsTrue(one >= two);
    }

    TEST_METHOD(T034_Operator_comparison_notequal)
    {
      Logger::WriteMessage("Test comparison icd208 != icd208");

      CString number1(_T("8765432123456.123456789"));
      CString number2(_T("8765432123456.123476789"));
      icd208 one(number1);
      icd208 two(number2);

      Assert::IsTrue(one != two);
    }

    TEST_METHOD(T035_Operator_comparison_smaller)
    {
      Logger::WriteMessage("Test comparison icd208 < icd208");

      CString number1(_T("8765432123456.123456789"));
      CString number2(_T("8765432123456.123476789"));
      icd208 one(number1);
      icd208 two(number2);

      Assert::IsTrue(one < two);
    }

    TEST_METHOD(T036_Operator_comparison_smaller)
    {
      Logger::WriteMessage("Test comparison icd208 > icd208");

      CString number1(_T("8765432123456.123456789"));
      CString number2(_T("8765432123456.123476789"));
      icd208 one(number1);
      icd208 two(number2);

      Assert::IsTrue(two > one);
    }

    TEST_METHOD(T037_Make_Zero)
    {
      Logger::WriteMessage("Test set to 0 (zero) with: icd208.Zero()");

      icd208 one(_T("12345.898989898342212"));
      icd208 zero;
      one.Zero();

      Assert::IsTrue(zero == one);
      Assert::IsTrue(zero.IsNull());
      Assert::IsTrue(one.IsNull());
    }

    TEST_METHOD(T038_Fraction)
    {
      Logger::WriteMessage("Test fraction part with: icd208.Fraction()");

      icd208 one(_T("12345.898989898342212"));
      icd208 expect (_T("0.898989898342212"));
      icd208 result = one - one.Floor();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T039_Ceiling)
    {
      Logger::WriteMessage("Test ceiling with: icd208.Ceiling()");

      icd208 one(_T("12345.898989898342212"));
      icd208 expect(12346L);
      icd208 result = one.Ceil();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T040_SquareRoot)
    {
      Logger::WriteMessage("Test root with: icd208.SquareRoot()");

      // 9988776655,4433221100
      icd208 one(_T("9988776655.4433221100"));
      // SQRT in 100 decimals
      icd208 expect(_T("+99943.8675229417070510788372162127884572850530512019859059460392399924806021377626076535993272885298297669"));
      icd208 result;

      HPFCounter count;
      for(int ind = 0; ind < 1000; ++ind)
      {
        result = one.SquareRoot();
      }
      count.Stop();
      CString speed;
      speed.Format(_T("Speed of the SQUARE ROOT calculation = %.6f"),count.GetCounter());
      Logger::WriteMessage(speed);

      // Original code  : Speed of the SQUARE ROOT calculation = 0.464387
      // FAST SQUAREROOT: Speed of the SQUARE ROOT calculation = 3.015204

      result.SetLengthAndPrecision(200,100);
      CString show = result.AsString();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T040_SquareRoot_0001)
    {
      Logger::WriteMessage("Test root with: icd208.SquareRoot()");

      icd208 one(_T("0.0001"));
      // SQRT in 100 decimals
      icd208 expect(_T("+0.01"));
      icd208 result;

      HPFCounter count;
      for (int ind = 0; ind < 1000; ++ind)
      {
        result = one.SquareRoot();
      }
      count.Stop();
      CString speed;
      speed.Format(_T("Speed of the SQUARE ROOT calculation = %.6f"), count.GetCounter());
      Logger::WriteMessage(speed);

      // Original code  : Speed of the SQUARE ROOT calculation = 0.041188
      // FAST SQUAREROOT: Speed of the SQUARE ROOT calculation = 1.370566

      result.SetLengthAndPrecision(200, 100);
      CString show = result.AsString();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T041_Power)
    {
      Logger::WriteMessage("Test power with: icd208.Power()");
      
      // 9988776655,4433221112
      // 3,78123764321234
      icd208 one(_T("9988776655.4433221112"));
      icd208 two(_T("3.78123764321234"));
      // Power in 100 decimals
      icd208 expect(_T("+64644611887465530217503224573033473303.2340420534353144399410107456850847865613303817548235292736286208199744319579190697139903189651615793"));
      icd208 result = one.Power(two);

      result.SetLengthAndPrecision(200,100);
      CString res = result.AsString();
      Logger::WriteMessage(res);
      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T042_Absolute)
    {
      Logger::WriteMessage("Test absolute value with: icd208.Absolute()");

      // 9988776655,4433221112
      icd208 one   (_T("-9988776655.4433221112"));
      icd208 expect(_T("+9988776655.4433221112"));
      icd208 result = one.AbsoluteValue();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T043_Reciproke)
    {
      Logger::WriteMessage("Test reciproke value with: icd208.Reciproke()");

      // 9988776655,4433221112
      icd208 one(1L);
      icd208 value(_T("-9988776655.4433221112"));
      // Reciproke in 100 decimals
      icd208 expect(_T("-0.0000000001001123595505617977406695215250599671773336463337253127440698347052408587063595319534948387"));
      icd208 result = one / value;

      result.SetLengthAndPrecision(200,100);
      CString show = result.AsString();
      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T044_Natural_logarithm)
    {
      Logger::WriteMessage("Test logarithm value with: icd208.Log()");

      // 9988776655,4433221112
      icd208 one(_T("9988776655.4433221112"));
      // Log in 100 decimals
      icd208 expect(_T("+23.0247279651958330529940554435089155194292165479231806873103676686847256153100284929444044013612578773"));

      icd208 result = one.Log();

      result.SetLengthAndPrecision(200,100);
      CString show = result.AsString();
      Logger::WriteMessage(show);
      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T045_10_logarithm)
    {
      Logger::WriteMessage("Test 10 logarithm value with: icd208.Log10()");

      // 9988776655,4433221112
      icd208 one(_T("9988776655.4433221112"));
      // Log10 in 100 decimals
      icd208 expect(_T("+9.9995123026080379948458936511546847891211669059895015981741411444727674239063543140744969519588035985"));

      icd208 result = one.Log10();
      result.SetLengthAndPrecision(200, 100);
      CString show = result.AsString();
      Logger::WriteMessage(show);
      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T046_e_power)
    {
      Logger::WriteMessage("Test e-power value with: icd208.Exp()");

      // 99,887766554433221112
      icd208 one(_T("99.887766554433221112"));
      // Exp in 100 decimals
      icd208 expect(_T("+24027347208844422813559191924441002416077897.2034160881298304562350443537220325014412192539507368961012541815789058820564415669365869200990033257"));
      
      icd208 result = one.Exp();
      result.SetLengthAndPrecision(200,100);
      CString show = result.AsString();
      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T047_10th_power)
    {
      Logger::WriteMessage("Test 10th-power value with: icd208.TenPower()");

      // 99,887766554433221112
      icd208 one(_T("99.887766554433221112"));
      icd208 expect(_T("998877665.54433221112"));
      icd208 result = one.TenPower(7);

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T048_Round)
    {
      Logger::WriteMessage("Test rounding of a number with : icd208.Round(5)");
      
      // 9988776655,4433277112
      icd208 one(_T("9988776655.4433277112"));
      one.Rounding(100,5);
      icd208 expect(_T("+9988776655.44333"));
      CString test = one.AsString();

      Assert::IsTrue(expect == one);

      one = _T("9988776655.64433277112");
      expect = _T("+9988776655.644333");
      one.Rounding(100,6);
      Assert::IsTrue(expect == one);

      one = _T("9988776655.64433277112");
      expect = _T("+9988776656");
      one.Rounding(100,0);
      Assert::IsTrue(expect == one);
    }

    TEST_METHOD(T049_Truncate)
    {
      Logger::WriteMessage("Test truncation of a number with : icd208.Truncate(5)");
      
      // 9988776655,4433277112
      icd208 one(_T("9988776655.4433277112"));
      one.SetLengthAndPrecision(100,5);
      icd208 expect(_T("9988776655.44332"));

      Assert::IsTrue(expect == one);

      one = _T("9988776655.64433277112");
      expect = _T("9988776655");
      one.SetLengthAndPrecision(100,0);
      Assert::IsTrue(expect == one);
    }

    TEST_METHOD(T050_Sine)
    {
      Logger::WriteMessage("Test Sine function of a radian with : icd208.Sine()");

      // 0,9876543210123456
      TCHAR* a_angle = _T("0.9876543210123456");
      // Sine in 100 decimals
      TCHAR* r_sine  = _T("+0.8347366295099261173476093153791068840875604759980911187504278256160738679090583056878605325875720593");

      icd208 one(a_angle);
      icd208 expect(r_sine);
      icd208 result = one.Sine();

      result.SetLengthAndPrecision(200,100);
      CString show = result.AsString();
      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T051_Cosine)
    {
      Logger::WriteMessage("Test Cosine function of a radian with : icd208.Cosine()");

      // 0,9876543210123456
      TCHAR* a_angle  = _T("0.9876543210123456");
      // Cosine in 100 decimals
      TCHAR* r_cosine = _T("+0.5506493978516714425340844117564672940309570891221813941576129806415616085572510137507995373900342996");

      icd208 one(a_angle);
      icd208 expect(r_cosine);
      icd208 result = one.Cosine();

      result.SetLengthAndPrecision(200, 100);
      CString show = result.AsString();
      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T052_Tangent)
    {
      Logger::WriteMessage("Test Tangent function of a radian with : icd208.Tangent()");

      // 0,9876543210123456
      TCHAR* a_angle   = _T("0.9876543210123456");
      // Tangent in 100 decimals
      TCHAR* r_tangent = _T("+1.5159130887395963684392407742872343024187341935250131109552651927915691134759545387515521530550442281");

      icd208 one(a_angle);
      icd208 expect(r_tangent);
      icd208 result = one.Tangent();

      result.SetLengthAndPrecision(200, 100);
      CString show = result.AsString();
      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T053_ArcSine)
    {
      Logger::WriteMessage("Test ArcSine function of a ratio with : icd208.ArcSine()");

      TCHAR* a_ratio = _T("0.765498765404321098765");
      // ArcSine in 100 decimals
      TCHAR* r_asine = _T("+0.8718161310705591010249460213430343772780510727322269076221581768222516809299039138426179544948247522");

      icd208 one(a_ratio);
      icd208 expect(r_asine);
      icd208 result = one.ArcSine();

      result.SetLengthAndPrecision(200, 100);
      CString show = result.AsString();
      Logger::WriteMessage(show);
      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T054_ArcCosine)
    {
      Logger::WriteMessage("Test ArcCosine function of a ratio with : icd208.ArcCosine()");

      TCHAR* a_ratio   = _T("0.765498765404321098765");
      // ArcCosine in 100 decimals
      TCHAR* r_acosine = _T("+0.6989801957243375182063756702967170648205336269553260028653141193316565222132005854713994581762337817");

      icd208 one(a_ratio);
      icd208 expect(r_acosine);
      icd208 result = one.ArcCosine();

      result.SetLengthAndPrecision(200, 100);
      CString show = result.AsString();
      Logger::WriteMessage(show);
      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T055_ArcTangent)
    {
      Logger::WriteMessage("Test ArcTangent function of a ratio with : icd208.ArcTangent()");

      TCHAR* a_ratio    = _T("0.765498765404321098765");
      // ArcTangent in 100 decimals
      TCHAR* r_atangent = _T("+0.6533467523844312707494031091720009420203307289851202240112116001575224076704283499140086358397604846");

      icd208 one(a_ratio);
      icd208 expect(r_atangent);
      icd208 result = one.ArcTangent();

      result.SetLengthAndPrecision(200, 100);
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

      icd208 one(&num);
      one.AsNumeric(&res);

      icd208 result(&res);

      Assert::IsTrue(one == result);
      Assert::IsTrue(one == icd208(_T("10.001")));
    }

    TEST_METHOD(T057_AsDouble)
    {
      Logger::WriteMessage("Test as-other-datatype: icd208.AsDouble()");

      icd208 one(_T("99887766.554433"));
      double result = one.AsDouble();
      double expect = 99887766.554433;

      Assert::IsTrue(abs(expect - result) < 0.00005);
    }

    TEST_METHOD(T058_AsShort)
    {
      Logger::WriteMessage("Test as-other-datatype: icd208.AsShort()");

      icd208 one(_T("4433"));
      short result = one.AsShort();
      short expect = 4433;

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T059_AsUShort)
    {
      Logger::WriteMessage("Test as-other-datatype: icd208.AsUShort()");

      icd208 one(_T("54873"));
      unsigned short result = one.AsUShort();
      unsigned short expect = 54873;

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T060_AsLong)
    {
      Logger::WriteMessage("Test as-other-datatype: icd208.AsLong()");

      icd208 one(_T("98854873"));
      long result = one.AsLong();
      long expect = 98854873;

      Assert::IsTrue(expect == result);
      Assert::IsTrue(one.FitsInLong());
    }

    TEST_METHOD(T061_AsULong)
    {
      Logger::WriteMessage("Test as-other-datatype: icd208.AsULong()");

      icd208 one(_T("2974276950"));
      unsigned long result = one.AsULong();
      unsigned long expect = 2974276950;

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T062_AsInt64)
    {
      Logger::WriteMessage("Test as-other-datatype: icd208.AsInt64()");

      icd208 one(_T("23154765019"));
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
      icd208 result;
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
      icd208 result;
      result = one;

      result.SetLengthAndPrecision(100,4);
      CString resstring = result.AsString();
      TCHAR* expect = _T("+1234567890.5432");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

//     TEST_METHOD(T065_ArcTangent2)
//     {
//       Logger::WriteMessage("Test ArcTangent2Points function of icd208 with icd208::ArcTangent2Points");
// 
//       // 1.0456788
//       // 7.89991232E-1
// 
//       icd208 one(_T("1.0456788"));
//       icd208 two(_T("7.89991232E-1"));
// 
//       icd208 result = one.ArcTangent2Points(two);
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

      icd208 one(_T("45523.89901"));
      one.Negate();

      one.SetLengthAndPrecision(100,5);
      CString resstring = one.AsString();
      TCHAR* expect = _T("-45523.89901");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }
    
    TEST_METHOD(T067_IsNull)
    {
      Logger::WriteMessage("Test the status function IsNull");

      icd208 one;
      icd208 two(12L);

      Assert::IsTrue(one.IsNull());
      Assert::IsFalse(two.IsNull());
    }

    TEST_METHOD(T068_GetSign)
    {
      Logger::WriteMessage("Test the status function GetSign");

      icd208 zero;
      icd208 one(12L);
      icd208 two(-12L);

      Assert::IsTrue(zero.GetSign() ==  0);
      Assert::IsTrue(one.GetSign()  ==  1);
      Assert::IsTrue(two.GetSign()  == -1);
    }

    TEST_METHOD(T069_GetLength)
    {
      Logger::WriteMessage("Test the status function GetLength");

      // 123456,789012345678
      icd208 one(_T("+123456.789012345678"));
      one.SetLengthAndPrecision(21, 12);
      Assert::IsTrue(one.Length() == 21);
    }

    TEST_METHOD(T070_GetPrecision)
    {
      Logger::WriteMessage("Test the status function GetPrecision");

      // 123456,789012345678
      icd208 one(_T("123456.789012345678"));
      one.SetLengthAndPrecision(21, 12);
      Assert::IsTrue(one.Precision() == 12);
    }

    TEST_METHOD(T071_GetMaxSize)
    {
      Logger::WriteMessage("Test the status function GetMaxSize");

      // 123456,789012345678
      icd208 one(_T("123456.789012345678"));

      // BEWARE: icd208 -> 200
      // 40 digits is now 208 digits
      Assert::IsTrue(one.GetMaxSize() == 208);
    }

    TEST_METHOD(T072_GetHasDecimals)
    {
      Logger::WriteMessage("Test the status function GetHasDecimals");

      // 123456,789012345678
      icd208 one(_T("123456.789012345678"));
      icd208 two(_T("877234"));
      icd208 three(_T("123.9"));
      Assert::IsTrue  (one.HasDecimals());
      Assert::IsFalse (two.HasDecimals());
      Assert::IsTrue(three.HasDecimals());
    }

    TEST_METHOD(T073_GetExponent)
    {
      Logger::WriteMessage("Test the function GetExponent");

      // 123456,789012345678
      icd208 one(_T("123456.789012345678"));
      icd208 two(_T("877234"));
      icd208 three(_T("123.9"));
      icd208 four(_T("0.0066"));

      Assert::IsTrue(one  .Exponent() == 5);
      Assert::IsTrue(two  .Exponent() == 5);
      Assert::IsTrue(three.Exponent() == 2);
      Assert::IsTrue(four .Exponent() == -3);
    }

    TEST_METHOD(T074_GetMantissa)
    {
      Logger::WriteMessage("Test the function GetMantissa");

      // 123456,789012345678
      icd208 one(_T("123456.789012345678"));
      icd208 two(_T("123.9"));

      icd208 mant1 = one.Mantissa();
      icd208 mant2 = two.Mantissa();

      mant1.SetLengthAndPrecision(100,17);
      mant2.SetLengthAndPrecision(100,3);

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
//       icd208 one(_T("123456.789012345678"));
//       FILE* file = nullptr;
//       _tfopen_s(&file,_T("C:\\TMP\\icd208.test"),_T("wb"));
//       if(file)
//       {
//         one.WriteToFile(file);
//         fclose(file);
//       }
// 
//       icd208 result;
//       Logger::WriteMessage("Test reading from file");
//       file = nullptr;
//       _tfopen_s(&file,_T("C:\\TMP\\icd208.test"),_T("rb"));
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
      icd208 one(_T("123456.789012345678"));
      icd208 result = floor(one);
      result.SetLengthAndPrecision(20,2);
      CString resstring = result.AsString();
      TCHAR* expect(_T("+123456"));

      Assert::AreEqual(expect,resstring.GetString());
    }

    TEST_METHOD(T077_math_ceil)
    {
      Logger::WriteMessage("Test math style ceil");

      // 123456,789012345678
      icd208 one(_T("123456.789012345678"));
      icd208 result = ceil(one);
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
      icd208 one(_T("123456.789012345678"));
      one.Rounding(8, 2);
      CString result = one.AsDisplayString();
      TCHAR* expect = _T("+123.456,79");
      Assert::AreEqual(expect,result.GetString());

      icd208 two(_T("9123456.789012345678"));
      two.Rounding(9,2);
      CString result2 = two.AsDisplayString();
      TCHAR* expect2 = _T("+9.123.456,79");
      Assert::AreEqual(expect2,result2.GetString());
    }

    TEST_METHOD(T079_Sine_zero)
    {
      Logger::WriteMessage("Test Sine(0.0)");

      // Reputationed to enter and endless loop
      icd208 zero;
      icd208 sineZero = zero.Sine();

      CString zeroResult(_T("+0"));
      CString zeroSine = sineZero.AsString();

      Assert::AreEqual(zeroSine.GetString(),zeroResult.GetString());
    }

    TEST_METHOD(T080_Cosine_zero)
    {
      Logger::WriteMessage("Test Cosine(0.0)");

      // Reputationed to enter and endless loop
      icd208 zero;
      icd208 cosineZero = zero.Cosine();

      CString zeroResult(_T("+1"));
      CString zeroCosine = cosineZero.AsString();

      Assert::AreEqual(zeroCosine.GetString(), zeroResult.GetString());
    }

    TEST_METHOD(T081_Exponent_zero)
    {
      Logger::WriteMessage("Test Exp(0.0)");

      // Reputationed to enter and endless loop
      icd208 zero;
      icd208 expZero = zero.Exp();

      CString zeroResult(_T("+1"));
      CString zeroExponent = expZero.AsString();

      Assert::AreEqual(zeroExponent.GetString(), zeroResult.GetString());
    }

    TEST_METHOD(T082_Sine_pi)
    {
      Logger::WriteMessage("Test Sine(180 degrees)");

      // Reputationed to enter and endless loop
      icd208 valuepi = icd208::PI();
      icd208 sinepi = valuepi.Sine();

      Assert::IsTrue(sinepi.IsNearZero());
    }

    TEST_METHOD(T083_Cosine_pi)
    {
      Logger::WriteMessage("Test Cosine(180 degrees)");

      // Reputationed to enter and endless loop
      icd208 valuepi = icd208::PI();
      icd208 cosinepi = valuepi.Cosine();
      cosinepi += icd208(1L);

      Assert::IsTrue(cosinepi.IsNearZero());
    }

    TEST_METHOD(T084_Exponents1)
    {
      Logger::WriteMessage("Test various exponents");

      icd208 number(_T("0.01"));
      icd208 expnumber = number.Exp();
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

      icd208 number(_T("0.03"));
      icd208 expnumber = number.Exp();
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

      icd208 number(_T("0.1"));
      icd208 numresult = number / 10.0;

      numresult.SetLengthAndPrecision(3, 2);
      CString expected(_T("+0.01"));
      CString result = numresult.AsString();
      
      Assert::AreEqual(expected.GetString(),result.GetString());
    }

    TEST_METHOD(T087_Divide10_Wrong)
    {
      Logger::WriteMessage("Test division by 10 wrong?");

      icd208 numresult(0.2 / 10.0);

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
      icd208    result = icd208(0L).Exp();

      Assert::AreEqual(expected, result.AsDouble());
    }

    TEST_METHOD(T089_Exponent_negative)
    {
      Logger::WriteMessage("Test exponent of negative number");

      double dblres = ::exp(-2.5);
      icd208 icdres = icd208(_T("-2.5")).Exp();
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

      icd208 tinynum(_T("0.01"));
      icd208 number(_T("-0.05"));

      icd208 result = tinynum + number;
      result.SetLengthAndPrecision(3,2);
      CString expected(_T("-0.04"));
      CString resstring = result.AsString();

      Assert::AreEqual(expected.GetString(),resstring.GetString());
    }

    TEST_METHOD(T091_Adding_negative)
    {
      Logger::WriteMessage("Adding small negative number to 0.0");

      // Original values provided by sisirajaya
      icd208 tinynum(_T("0.0"));
      icd208 number(_T("-0.05"));

      icd208 result = tinynum + number;
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
//         icd208 b = val;
//         __int64 back = b.AsInt64();
//         if(back != val)
//         {
//           Assert::Fail(L"icd208 <-> integer64 just broke!");
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

      icd208 one(_T("94505024884"));
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
      icd208 one (num,64000000);
      CString expected(_T("+94505024884.64"));
      one.SetLengthAndPrecision(80,2);
      CString result = one.AsString();

      Assert::AreEqual(expected.GetString(),result.GetString());
    }

    TEST_METHOD(T095_CornerCases)
    {
      Logger::WriteMessage("Testing integer corner cases.");

      icd208 test1((char)MININT8);      Assert::AreEqual((int)MININT8,  (int)test1.AsShort());
      icd208 test2((char)MAXINT8);      Assert::AreEqual((int)MAXINT8,  (int)test2.AsShort()); 

      icd208 test3((uchar)0);           Assert::AreEqual(0,             (int)test3.AsShort()); 
      icd208 test4((uchar)MAXUINT8);    Assert::AreEqual((int)MAXUINT8, (int)test4.AsShort());

      icd208 test5((short)MININT16);    Assert::AreEqual((int)MININT16, (int)test5.AsShort()); 
      icd208 test6((short)MAXINT16);    Assert::AreEqual((int)MAXINT16, (int)test6.AsShort()); 

      icd208 test7((ushort)0);          Assert::AreEqual((int)0,        (int)test7.AsUShort()); 
      icd208 test8((ushort)MAXUINT16);  Assert::AreEqual((int)MAXUINT16,(int)test8.AsUShort()); 

      icd208 test9((long)MININT32);     Assert::AreEqual((int)MININT32, (int)test9.AsLong());
      icd208 test10((long)MAXINT32);    Assert::AreEqual((int)MAXINT32, (int)test10.AsLong()); 

      icd208 test11((ulong)0);          Assert::IsTrue(test11.AsULong() == 0); 
      icd208 test12((ulong)MAXUINT32);  Assert::IsTrue(test12.AsULong() == MAXUINT32); 

      icd208 test13((INT64)MININT64+1); Assert::IsTrue(test13.AsInt64() == (MININT64 + 1)); 
      icd208 test14((INT64)MAXINT64);   Assert::IsTrue(test14.AsInt64() == MAXINT64); 

      icd208 test15((UINT64)0);         Assert::IsTrue(test15.AsUInt64() == 0L); 
      icd208 test16((UINT64)MAXUINT64); Assert::IsTrue(test16.AsUInt64() == MAXUINT64);
    }

    TEST_METHOD(T096_TestNaturalLog)
    {
      Logger::WriteMessage("Testing BCD Natural Log 0.5");

      icd208 value(_T("0.5"));
      icd208 nlog = value.Log();

      CString expected = _T("-0.69314718055994530941723212145817656807550013436025525412068000949339362196969471560586332699641868754102");
      nlog.SetLengthAndPrecision(208,104);
      CString testval  = nlog.AsString();

      Assert::AreEqual(expected.GetString(), testval.GetString());
    }

    TEST_METHOD(T097_CalculatePI)
    {
      Logger::WriteMessage("Testing BCD calculating PI");
      icd208 value("-1");
      icd208 pi = value.ArcCosine();
      icd208 internalPi = icd208::PI();

      pi.Rounding(192,96);
      internalPi.Rounding(192,96);

      pi.SetLengthAndPrecision(208,104);
      internalPi.SetLengthAndPrecision(208,104);

      CString disp = pi.AsString();
      CString intr = internalPi.AsString();
      // In reality: So rounding to 96 significant decimals needed
      // +3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211724032095
      // +3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798215
      icd208 diff = pi - internalPi;
      Assert::IsTrue(diff.IsNearZero());
    }

    TEST_METHOD(T098_NaturalLog2)
    {
      Logger::WriteMessage("Testing BCD Natural Log 2");
      icd208 value("2");
      icd208 ln2      = value.Log();
      icd208 internal = icd208::LN2();

      ln2.Rounding(192, 96);
      internal.Rounding(192,96);

      icd208 diff = ln2 - internal;
      Assert::IsTrue(diff.IsNearZero());
    }

    TEST_METHOD(T099_NaturalLog10)
    {
      Logger::WriteMessage("Testing ICD Natural Log 10");
      icd208 value("10");
      icd208 log = value.Log();
      icd208 internal = icd208::LN10();

      log.Rounding(192, 90);
      internal.Rounding(192, 90);

      icd208 diff = log - internal;
      Assert::IsTrue(diff.IsNearZero());
    }
  };
}
