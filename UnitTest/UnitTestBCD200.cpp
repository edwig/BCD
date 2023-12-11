#include "stdafx.h"
#include "CppUnitTest.h"
#include "bcd200.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(UnitTestBCD200)
	{
	public:
		
    TEST_METHOD(T001_Constructor01)
    {
      Logger::WriteMessage("Construct bcd200");
      bcd200 one;
      Assert::IsTrue(one.IsZero());
    }

		TEST_METHOD(T002_Constructor02)
		{
      Logger::WriteMessage("Construct bcd200 from an 1 byte number");

      bcd200  one('\n');
      long expect(10);

      Assert::AreEqual(expect,one.AsLong());
		}

    TEST_METHOD(T003_Constructor03)
    {
      Logger::WriteMessage("Construct a bcd200 from a short int");

      short expect(12);
      bcd200   one(expect);

      Assert::AreEqual((long)expect,one.AsLong());
    }

    TEST_METHOD(T004_Constructor04)
    {
      Logger::WriteMessage("Construct a bcd200 from a long");

      long expect(765432198);
      bcd200  one(expect);

      Assert::AreEqual((long)expect,one.AsLong());
    }

    TEST_METHOD(T005_Constructor05)
    {
      Logger::WriteMessage("Construct a bcd200 from a long + remainder");

      long base(765432198);
      long remainder(50000000);
      bcd200  one(base,remainder);
      double expect = 765432198.5;
      double result = one.AsDouble();
      expect = trunc(expect * 10) / 10;
      result = trunc(result * 10) / 10;

      Assert::AreEqual(expect,result);
    }

    TEST_METHOD(T006_Constructor06)
    {
      Logger::WriteMessage("Construct a bcd200 from an __int64");

      __int64 expect(76543219876543210LL);
      bcd200  one( expect);
      __int64 result = one.AsInt64();

      Assert::IsTrue(result - expect == 0);
    }

    TEST_METHOD(T007_Constructor07)
    {
      Logger::WriteMessage("Construct a bcd200 from an unsigned __int64");

      unsigned __int64 expect(76543219876543210LL);
      bcd200  one(_T("76543219876543210"));
      unsigned __int64 result = one.AsUInt64();

      Assert::IsTrue(result - expect == 0);
    }

    TEST_METHOD(T008_Constructor08)
    {
      Logger::WriteMessage("Construct a bcd200 from another bcd200");

      bcd200 one(12345);
      bcd200 two(one);

      Assert::IsTrue(one.AsLong() - two.AsLong() == 0);
    }

    TEST_METHOD(T009_Constructor09)
    {
      Logger::WriteMessage("Construct a bcd200 from a double");

      double expect = 765432198.5;
      bcd200  one(expect);
      double result = one.AsDouble();
      expect = trunc(expect * 10) / 10;
      result = trunc(result * 10) / 10;

      Assert::AreEqual(expect,result);
    }

    TEST_METHOD(T010_Constructor10)
    {
      Logger::WriteMessage("Construct a bcd200 from a CString");

      CString large(_T("76543219876543210"));
      unsigned __int64 expect(76543219876543210LL);
      bcd200  one(large);
      unsigned __int64 result = one.AsUInt64();

      Assert::IsTrue(result - expect == 0);
    }

    TEST_METHOD(T011_Constant_PI)
    {
      Logger::WriteMessage("Test the constant of PI");
      bcd200 pi = bcd200::PI();
      CString pi_string = pi.AsString();
    //CString expect(_T("3.141592653589793238462643383279502884197"));
      CString expect(_T("3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819"));

      Assert::AreEqual(expect.GetString(),pi_string.GetString());
    }

    TEST_METHOD(T012_Constant_LN2)
    {
      Logger::WriteMessage("Test the constant LN2");

      bcd200 ln2 = bcd200::LN2();
      CString ln2_string = ln2.AsString();
    //CString expect(_T("0.6931471805599453094172321214581765680755"));
      CString expect(_T("0.69314718055994530941723212145817656807550013436025525412068000949339362196969471560586332699641868754200148102057068573368552023575813055703267075163507596193072757082837143519030703862389167347111855"));

      Assert::AreEqual(expect.GetString(),ln2_string.GetString());
    }

    TEST_METHOD(T013_Constant_LN10)
    {
      Logger::WriteMessage("Test the constant LN10");

      bcd200 ln10 = bcd200::LN10();
      CString ln10_string = ln10.AsString();
    //CString expect(_T("2.302585092994045684017991454684364207601"));
      CString expect(_T("2.3025850929940456840179914546843642076011014886287729760333279009675726096773524802359972050895982983419677840422862486334095254650828067566662873690987816894829072083255546808437998948262331985283296"));

      Assert::AreEqual(expect.GetString(),ln10_string.GetString());
    }

    TEST_METHOD(T014_Operator_plus)
    {
      Logger::WriteMessage("Test operator +");

      bcd200 one(_T("123456.789012345678"));
      bcd200 two(_T("9988776655.4433221100"));
      bcd200 result = one + two;
      CString resstring = result.AsString();
      TCHAR* expect = _T("9988900112.232334455678");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T015_Operator_min)
    {
      Logger::WriteMessage("Test operator -");

      // 123456,789012345678
      // 9988776655,4433221100
      bcd200 one(_T("123456.789012345678"));
      bcd200 two(_T("9988776655.4433221100"));
      bcd200 result = one - two;
      CString resstring = result.AsString();
      TCHAR* expect = _T("-9988653198.654309764322");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T016_Operator_multiply)
    {
      Logger::WriteMessage("Test operator *");

      // 123456,789012345678
      // 9988776655,4433221100
      bcd200 one(_T("123456.789012345678"));
      bcd200 two(_T("9988776655.4433221100"));
      bcd200 result = one * two;
      CString resstring = result.AsString();
      TCHAR* expect = _T("1233182292042510.13939532499202034058");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T017_Operator_divide)
    {
      Logger::WriteMessage("Test operator /");

      // 123456,789012345678
      // 9988776655,4433221100
      bcd200 one(_T("123456.789012345678"));
      bcd200 two(_T("9988776655.4433221100"));
      bcd200 result = one / two;
      CString resstring = result.AsString(bcd200::Format::Engineering);
    //TCHAR* expect = _T("1.235955045056179765144678702196566090151E-5");
      TCHAR* expect = _T("1.2359550450561797651446787021965660901515228329188802628200544687270574379069437590239021826540371418437933683340965874810743894737819139466504782140744023438801680020066579748945166651662846539607926E-5");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T018_Operator_modulo)
    {
      Logger::WriteMessage("Test operator %");

      // 123456,789012345678
      // 9988776655,4433221100
      bcd200 one(_T("123456.789012345678"));
      bcd200 two(_T("9988776655.4433221100"));
      bcd200 result = two % one;
      CString resstring = result.AsString();
      TCHAR* expect = _T("11313.243445648698");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T019_Operator_assign_plus)
    {
      Logger::WriteMessage("Test operator +=");

      bcd200 one(_T("123456.789012345678"));
      bcd200 two(_T("9988776655.4433221100"));
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
      bcd200 one(_T("123456.789012345678"));
      bcd200 two(_T("9988776655.4433221100"));
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
      bcd200 one(_T("123456.789012345678"));
      bcd200 two(_T("9988776655.4433221100"));
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
      bcd200 one(_T("123456.789012345678"));
      bcd200 two(_T("9988776655.4433221100"));
      one /= two;
      CString resstring = one.AsString(bcd200::Format::Engineering);
    //TCHAR* expect = _T("1.235955045056179765144678702196566090151E-5");
      TCHAR* expect = _T("1.2359550450561797651446787021965660901515228329188802628200544687270574379069437590239021826540371418437933683340965874810743894737819139466504782140744023438801680020066579748945166651662846539607926E-5");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }
 
    TEST_METHOD(T023_Operator_assign_modulo)
    {
      Logger::WriteMessage("Test operator %=");

      // 123456,789012345678
      // 9988776655,4433221100
      bcd200 one(_T("123456.789012345678"));
      bcd200 two(_T("9988776655.4433221100"));
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
      bcd200 one(_T("123456.789012345678"));
      bcd200 two = -one;
      CString resstring = two.AsString();
      TCHAR* expect = _T("-123456.789012345678");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T025_Operator_postfix_increment)
    {
      Logger::WriteMessage("Test operator bcd200++ (postfix increment)");

      bcd200 one(_T("1234567890123456789"));
      bcd200 two = one++;
      CString resstring1 = one.AsString();
      CString resstring2 = two.AsString();
      TCHAR* expect1 = _T("1234567890123456790.00");
      TCHAR* expect2 = _T("1234567890123456789.00");

      Assert::IsTrue(strcmp(expect1,resstring1.GetString()) == 0);
      Assert::IsTrue(strcmp(expect2,resstring2.GetString()) == 0);
    }

    TEST_METHOD(T026_Operator_prefix_increment)
    {
      Logger::WriteMessage("Test operator ++bcd200 (prefix increment)");

      bcd200 one(_T("1234567890123456789"));
      bcd200 two = ++one;
      CString resstring1 = one.AsString();
      CString resstring2 = two.AsString();
      TCHAR* expect1 = _T("1234567890123456790.00");
      TCHAR* expect2 = _T("1234567890123456790.00");

      Assert::IsTrue(strcmp(expect1,resstring1.GetString()) == 0);
      Assert::IsTrue(strcmp(expect2,resstring2.GetString()) == 0);
    }

    TEST_METHOD(T027_Operator_postfix_decrement)
    {
      Logger::WriteMessage("Test operator bcd200-- (postfix decrement)");

      bcd200 one(_T("1234567890123456790"));
      bcd200 two = one--;
      CString resstring1 = one.AsString();
      CString resstring2 = two.AsString();
      TCHAR* expect1 = _T("1234567890123456789.00");
      TCHAR* expect2 = _T("1234567890123456790.00");

      Assert::IsTrue(strcmp(expect1,resstring1.GetString()) == 0);
      Assert::IsTrue(strcmp(expect2,resstring2.GetString()) == 0);
    }

    TEST_METHOD(T028_Operator_prefix_decrement)
    {
      Logger::WriteMessage("Test operator bcd200-- (postfix decrement)");

      bcd200 one(_T("1234567890123456790"));
      bcd200 two = --one;
      CString resstring1 = one.AsString();
      CString resstring2 = two.AsString();
      TCHAR* expect1 = _T("1234567890123456789.00");
      TCHAR* expect2 = _T("1234567890123456789.00");

      Assert::IsTrue(strcmp(expect1,resstring1.GetString()) == 0);
      Assert::IsTrue(strcmp(expect2,resstring2.GetString()) == 0);
    }

    TEST_METHOD(T029_Operator_assign_bcd)
    {
      Logger::WriteMessage("Test operator bcd200 = bcd200 (assignment operator)");

      bcd200 one(_T("123456.789012345678"));
      bcd200 two = one;
      CString resstring = two.AsString(bcd200::Format::Bookkeeping,false,12);
      TCHAR* expect = _T("123456.789012345678");

      Assert::AreEqual(expect,resstring.GetString());
    }

    TEST_METHOD(T030_Operator_assign_long)
    {
      Logger::WriteMessage("Test operator bcd200 = long (assignment operator)");

      long one = 123456789;
      bcd200 two(one);
      CString resstring = two.AsString();
      TCHAR* expect = _T("123456789.00");

      Assert::AreEqual(expect,resstring.GetString());
    }

    TEST_METHOD(T031_Operator_assign_double)
    {
      Logger::WriteMessage("Test operator bcd200 = double (assignment operator)");

      double one = 123456.123456789;
      bcd200 two(one);
      CString resstring = two.AsString();
      TCHAR* expect = _T("123456.123456789");

      Assert::AreEqual(expect,resstring.GetString());
    }

    TEST_METHOD(T032_Operator_assign_CString)
    {
      Logger::WriteMessage("Test operator bcd200 = CString (assignment operator)");

      CString one(_T("123456.123456789"));
      bcd200 two(one);
      CString resstring = two.AsString();
      TCHAR* expect = _T("123456.123456789");

      Assert::AreEqual(expect,resstring.GetString());
    }

    TEST_METHOD(T033_Operator_comparison_equal)
    {
      Logger::WriteMessage("Test comparison bcd200 == bcd200");
      Logger::WriteMessage("Test comparison bcd200 >= bcd200");
      Logger::WriteMessage("Test comparison bcd200 <= bcd200");

      CString number(_T("8765432123456.123456789"));
      bcd200 one(number);
      bcd200 two(number);

      Assert::IsTrue(one == two);
      Assert::IsTrue(one <= two);
      Assert::IsTrue(one >= two);
    }

    TEST_METHOD(T034_Operator_comparison_notequal)
    {
      Logger::WriteMessage("Test comparison bcd200 != bcd200");

      CString number1(_T("8765432123456.123456789"));
      CString number2(_T("8765432123456.123476789"));
      bcd200 one(number1);
      bcd200 two(number2);

      Assert::IsTrue(one != two);
    }

    TEST_METHOD(T035_Operator_comparison_smaller)
    {
      Logger::WriteMessage("Test comparison bcd200 < bcd200");

      CString number1(_T("8765432123456.123456789"));
      CString number2(_T("8765432123456.123476789"));
      bcd200 one(number1);
      bcd200 two(number2);

      Assert::IsTrue(one < two);
    }

    TEST_METHOD(T036_Operator_comparison_smaller)
    {
      Logger::WriteMessage("Test comparison bcd200 > bcd200");

      CString number1(_T("8765432123456.123456789"));
      CString number2(_T("8765432123456.123476789"));
      bcd200 one(number1);
      bcd200 two(number2);

      Assert::IsTrue(two > one);
    }

    TEST_METHOD(T037_Make_Zero)
    {
      Logger::WriteMessage("Test set to 0 (zero) with: bcd200.Zero()");

      bcd200 one(_T("12345.898989898342212"));
      bcd200 zero;
      one.Zero();

      Assert::IsTrue(zero == one);
      Assert::IsTrue(zero.IsZero());
      Assert::IsTrue(one.IsZero());
    }

    TEST_METHOD(T038_Fraction)
    {
      Logger::WriteMessage("Test fraction part with: bcd200.Fraction()");

      bcd200 one(_T("12345.898989898342212"));
      bcd200 expect (_T("0.898989898342212"));
      bcd200 result = one.Fraction();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T039_Ceiling)
    {
      Logger::WriteMessage("Test ceiling with: bcd200.Ceiling()");

      bcd200 one(_T("12345.898989898342212"));
      bcd200 expect(12346);
      bcd200 result = one.Ceiling();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T040_SquareRoot)
    {
      Logger::WriteMessage("Test root with: bcd200.SquareRoot()");

      // 9988776655,4433221100
      bcd200 one(_T("9988776655.4433221100"));
    //bcd200 expect(_T("99943.86752294170705107883721621278845710"));
      bcd200 expect(_T("99943.86752294170705107883721621278845728505305120198590594603923999248060213776260765359932728852982977071848960784809271784113274507829156812495717071028518663266128900612303561676739127744453257189"));
      bcd200 result = one.SquareRoot();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T041_Power)
    {
      Logger::WriteMessage("Test power with: bcd200.Power()");
      
      // 9988776655,4433221112
      // 3,78123764321234
      bcd200 one(_T("9988776655.4433221112"));
      bcd200 two(_T("3.78123764321234"));
    //bcd200 expect(_T("6.46446118874655302175032245730334730318E+37"));
      bcd200 expect(_T("6.4644611887465530217503224573033473303234042053435314439941010745685084786561330381754823529273589697907876993593141826387830776282819191401180394363137438806177372580960520931593965626406832940084236E+37"));
      bcd200 result = one.Power(two);

      CString res = result.AsString(bcd200::Format::Engineering);
      Logger::WriteMessage(res);
      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T042_Absolute)
    {
      Logger::WriteMessage("Test absolute value with: bcd200.Absolute()");

      // 9988776655,4433221112
      bcd200 one  (_T("-9988776655.4433221112"));
      bcd200 expect(_T("9988776655.4433221112"));
      bcd200 result = one.AbsoluteValue();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T043_Reciproke)
    {
      Logger::WriteMessage("Test reciproke value with: bcd200.Reciproke()");

      // 9988776655,4433221112
      bcd200 one(_T("-9988776655.4433221112"));
      //bcd  expect(_T("-1.001123595505617977406695215250599671773E-10"));
      bcd200 expect(_T("-1.0011235955056179774066952152505996717733364633372531274406983470524085870635953195349483879596593181076273819903298498762076999276444973048985000267512425290509594124865234901851915283196116684710271E-10"));
      bcd200 result = one.Reciprocal();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T044_Natural_logarithm)
    {
      Logger::WriteMessage("Test logarithm value with: bcd200.Log()");

      // 9988776655,4433221112
      bcd200 one(_T("9988776655.4433221112"));
      //bcd  expect(_T("23.02472796519583305299405544350891551931"));
      bcd200 expect(_T("23.024727965195833052994055443508915519429216547923180687310367668684725615310028492944404401361257718137186906609063750125558646654287262393879943563443517697796419354297694729255518132260276986244695"));

      bcd200 result = one.Log();

      Logger::WriteMessage(result.AsString());
      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T045_10_logarithm)
    {
      Logger::WriteMessage("Test 10 logarithm value with: bcd200.Log10()");

      // 9988776655,4433221112
      bcd200 one(_T("9988776655.4433221112"));
      // bcd expect(_T("9.99951230260803799484589365115468478907"));
      bcd200 expect(_T("9.999512302608037994845893651154684789121166905989501598174141144472767423906354314074496951958803529396043156171912666827014409480557345562913565514795075663174353450192308334896727819595726205596187"));

      bcd200 result = one.Log10();
      Logger::WriteMessage(result.AsString());
      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T046_e_power)
    {
      Logger::WriteMessage("Test e-power value with: bcd200.Exp()");

      // 99,887766554433221112
      bcd200 one(_T("99.887766554433221112"));
      // bcd expect(_T("2.402734720884442281355919192444100235161E+43"));
      bcd200 expect(_T("2.4027347208844422813559191924441002416077897203416088129830456235044353722032501441219253950736896102538882111290049971443308227274811650975539954866320981084636448708948837592137072936627385738136364E+43"));
      bcd200 result = one.Exp();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T047_10th_power)
    {
      Logger::WriteMessage("Test 10th-power value with: bcd200.TenPower()");

      // 99,887766554433221112
      bcd200 one(_T("99.887766554433221112"));
      bcd200 expect(_T("998877665.54433221112"));
      bcd200 result = one.TenPower(7);

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T048_Round)
    {
      Logger::WriteMessage("Test rounding of a number with : bcd200.Round(5)");
      
      // 9988776655,4433277112
      bcd200 one(_T("9988776655.4433277112"));
      one.Round(5);
      bcd200 expect(_T("9988776655.44333"));
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
      Logger::WriteMessage("Test truncation of a number with : bcd200.Truncate(5)");
      
      // 9988776655,4433277112
      bcd200 one(_T("9988776655.4433277112"));
      one.Truncate(5);
      bcd200 expect(_T("9988776655.44332"));

      Assert::IsTrue(expect == one);

      one = _T("9988776655.64433277112");
      expect = _T("9988776655");
      one.Truncate(0);
      Assert::IsTrue(expect == one);
    }

    TEST_METHOD(T050_Sine)
    {
      Logger::WriteMessage("Test Sine function of a radian with : bcd200.Sine()");

      // 0,9876543210123456
      TCHAR* a_angle = _T("0.9876543210123456");
    //TCHAR* r_sine  = _T("8.347366295099261173476093153791068840984E-1");
      TCHAR* r_sine  = _T("8.3473662950992611734760931537910688408756047599809111875042782561607386790905830568786053258757205938182671211681730536871695308929115141187292042958860261918458846154902597094065887748289278412163839E-1");

      bcd200 one(a_angle);
      bcd200 expect(r_sine);
      bcd200 result = one.Sine();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T051_Cosine)
    {
      Logger::WriteMessage("Test Cosine function of a radian with : bcd200.Cosine()");

      // 0,9876543210123456
      TCHAR* a_angle  = _T("0.9876543210123456");
    //TCHAR* r_cosine = _T("5.506493978516714425340844117564672940338E-1");
      TCHAR* r_cosine = _T("5.5064939785167144253408441175646729403095708912218139415761298064156160855725101375079953739003429964434530245713602015915525129754596525920855981453564775790553063894738103674745387910664599767280971E-1");

      bcd200 one(a_angle);
      bcd200 expect(r_cosine);
      bcd200 result = one.Cosine();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T052_Tangent)
    {
      Logger::WriteMessage("Test Tangent function of a radian with : bcd200.Tangent()");

      // 0,9876543210123456
      TCHAR* a_angle   = _T("0.9876543210123456");
    //TCHAR* r_tangent = _T("1.515913088739596368439240774287234302490");
      TCHAR* r_tangent = _T("1.5159130887395963684392407742872343024187341935250131109552651927915691134759545387515521530550442281041631058027325235923778527508079283345838860820997561343085430350650315585480172879059297371068217");

      bcd200 one(a_angle);
      bcd200 expect(r_tangent);
      bcd200 result = one.Tangent();

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T053_ArcSine)
    {
      Logger::WriteMessage("Test ArcSine function of a ratio with : bcd200.ArcSine()");

      TCHAR* a_ratio = _T("0.765498765404321098765");
    //TCHAR* r_asine = _T("8.71816131070559101024946021343034377293E-1");
      TCHAR* r_asine = _T("8.718161310705591010249460213430343772780510727322269076221581768222516809299039138426179544948246419165205190870002969350813972110542472591961069061175868615003642554566993219186899399910388624925912E-1");

      bcd200 one(a_ratio);
      bcd200 expect(r_asine);
      bcd200 result = one.ArcSine();

      Logger::WriteMessage(result.AsString());
      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T054_ArcCosine)
    {
      Logger::WriteMessage("Test ArcCosine function of a ratio with : bcd200.ArcCosine()");

      TCHAR* a_ratio   = _T("0.765498765404321098765");
    //TCHAR* r_acosine = _T("6.98980195724337518206375670296717064805E-1");
      TCHAR* r_acosine = _T("6.989801957243375182063756702967170648205336269553260028653141193316565222132005854713994581762338920745535241696408563884655250937210438566665727979466536972247777958942699386340898823204356149725997E-1");

      bcd200 one(a_ratio);
      bcd200 expect(r_acosine);
      bcd200 result = one.ArcCosine();

      Logger::WriteMessage(result.AsString());
      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T055_ArcTangent)
    {
      Logger::WriteMessage("Test ArcTangent function of a ratio with : bcd200.ArcTangent()");

      TCHAR* a_ratio    = _T("0.765498765404321098765");
    //TCHAR* r_atangent = _T("6.53346752384431270749403109172000942006E-1");
      TCHAR* r_atangent = _T("6.533467523844312707494031091720009420203307289851202240112116001575224076704283499140086358397604074181618354914359155260497021554984833271295227833039700317016931210335901428034980693894010800950587E-1");

      bcd200 one(a_ratio);
      bcd200 expect(r_atangent);
      bcd200 result = one.ArcTangent();

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

      bcd200 one(&num);
      one.AsNumeric(&res);

      bcd200 result(&res);

      Assert::IsTrue(one == result);
      Assert::IsTrue(one == bcd200(_T("10.001")));
    }

    TEST_METHOD(T057_AsDouble)
    {
      Logger::WriteMessage("Test as-other-datatype: bcd200.AsDouble()");

      bcd200 one(_T("99887766.554433"));
      double result = one.AsDouble();
      double expect = 99887766.554433;

      Assert::IsTrue(abs(expect - result) < 0.00005);
    }

    TEST_METHOD(T058_AsShort)
    {
      Logger::WriteMessage("Test as-other-datatype: bcd200.AsShort()");

      bcd200 one(_T("4433"));
      short result = one.AsShort();
      short expect = 4433;

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T059_AsUShort)
    {
      Logger::WriteMessage("Test as-other-datatype: bcd200.AsUShort()");

      bcd200 one(_T("54873"));
      unsigned short result = one.AsUShort();
      unsigned short expect = 54873;

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T060_AsLong)
    {
      Logger::WriteMessage("Test as-other-datatype: bcd200.AsLong()");

      bcd200 one(_T("98854873"));
      long result = one.AsLong();
      long expect = 98854873;

      Assert::IsTrue(expect == result);
      Assert::IsTrue(one.GetFitsInLong());
    }

    TEST_METHOD(T061_AsULong)
    {
      Logger::WriteMessage("Test as-other-datatype: bcd200.AsULong()");

      bcd200 one(_T("2974276950"));
      unsigned long result = one.AsULong();
      unsigned long expect = 2974276950;

      Assert::IsTrue(expect == result);
    }

    TEST_METHOD(T062_AsInt64)
    {
      Logger::WriteMessage("Test as-other-datatype: bcd200.AsInt64()");

      bcd200 one(_T("23154765019"));
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
      bcd200 result;
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
      bcd200 result;
      result = one;
      CString resstring = result.AsString();
      TCHAR* expect = _T("1234567890.5432");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }

    TEST_METHOD(T065_ArcTangent2)
    {
      Logger::WriteMessage("Test ArcTangent2Points function of bcd200 with bcd200::ArcTangent2Points");

      // 1.0456788
      // 7.89991232E-1

      bcd200 one(_T("1.0456788"));
      bcd200 two(_T("7.89991232E-1"));

      bcd200 result = one.ArcTangent2Points(two);
      CString resstring = result.AsString();
    //TCHAR* expect = _T("4.065388783692102294632898803723974485214");
      TCHAR* expect = _T("4.0653887836921022946328988037239744852614244406154928728422038138928689899362436394182360017777313271594818799035132762840973140168967467029056723865817696443609077737987223137735619386195371474136125");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }
    
    TEST_METHOD(T066_Negate)
    {
      Logger::WriteMessage("Test the Negate method for unary -");

      bcd200 one(_T("45523.89901"));
      one.Negate();
      CString resstring = one.AsString();
      TCHAR* expect = _T("-45523.89901");

      Assert::IsTrue(strcmp(expect,resstring.GetString()) == 0);
    }
    
    TEST_METHOD(T067_IsNull)
    {
      Logger::WriteMessage("Test the status function IsNull");

      bcd200 one;
      bcd200 two(12);

      Assert::IsTrue(one.IsZero());
      Assert::IsFalse(two.IsZero());
    }

    TEST_METHOD(T068_GetSign)
    {
      Logger::WriteMessage("Test the status function GetSign");

      bcd200 zero;
      bcd200 one(12);
      bcd200 two(-12);

      Assert::IsTrue(zero.GetSign() ==  0);
      Assert::IsTrue(one.GetSign()  ==  1);
      Assert::IsTrue(two.GetSign()  == -1);
    }

    TEST_METHOD(T069_GetLength)
    {
      Logger::WriteMessage("Test the status function GetLength");

      // 123456,789012345678
      bcd200 one(_T("123456.789012345678"));
      Assert::IsTrue(one.GetLength() == 18);
    }

    TEST_METHOD(T070_GetPrecision)
    {
      Logger::WriteMessage("Test the status function GetPrecision");

      // 123456,789012345678
      bcd200 one(_T("123456.789012345678"));
      Assert::IsTrue(one.GetPrecision() == 12);
    }

    TEST_METHOD(T071_GetMaxSize)
    {
      Logger::WriteMessage("Test the status function GetMaxSize");

      // 123456,789012345678
      bcd200 one(_T("123456.789012345678"));

      // BEWARE: bcd200 -> 200
      // 40 digits is now 200 digits
      Assert::IsTrue(one.GetMaxSize() == 200);
    }

    TEST_METHOD(T072_GetHasDecimals)
    {
      Logger::WriteMessage("Test the status function GetHasDecimals");

      // 123456,789012345678
      bcd200 one(_T("123456.789012345678"));
      bcd200 two(_T("877234"));
      bcd200 three(_T("123.9"));
      Assert::IsTrue  (one.GetHasDecimals());
      Assert::IsFalse (two.GetHasDecimals());
      Assert::IsTrue(three.GetHasDecimals());
    }

    TEST_METHOD(T073_GetExponent)
    {
      Logger::WriteMessage("Test the function GetExponent");

      // 123456,789012345678
      bcd200 one(_T("123456.789012345678"));
      bcd200 two(_T("877234"));
      bcd200 three(_T("123.9"));
      bcd200 four(_T("0.0066"));

      Assert::IsTrue(one  .GetExponent() == 5);
      Assert::IsTrue(two  .GetExponent() == 5);
      Assert::IsTrue(three.GetExponent() == 2);
      Assert::IsTrue(four .GetExponent() == -3);
    }

    TEST_METHOD(T074_GetMantissa)
    {
      Logger::WriteMessage("Test the function GetMantissa");

      // 123456,789012345678
      bcd200 one(_T("123456.789012345678"));
      bcd200 two(_T("123.9"));

      bcd200 mant1 = one.GetMantissa();
      bcd200 mant2 = two.GetMantissa();

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
      bcd200 one(_T("123456.789012345678"));
      FILE* file = nullptr;
      _tfopen_s(&file,_T("C:\\TMP\\bcd200.test"),_T("wb"));
      if(file)
      {
        one.WriteToFile(file);
        fclose(file);
      }

      bcd200 result;
      Logger::WriteMessage("Test reading from file");
      file = nullptr;
      _tfopen_s(&file,_T("C:\\TMP\\bcd200.test"),_T("rb"));
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
      bcd200 one(_T("123456.789012345678"));
      bcd200 result = floor(one);
      CString resstring = result.AsString();
      TCHAR* expect(_T("123456.00"));

      Assert::AreEqual(expect,resstring.GetString());
    }

    TEST_METHOD(T077_math_ceil)
    {
      Logger::WriteMessage("Test math style ceil");

      // 123456,789012345678
      bcd200 one(_T("123456.789012345678"));
      bcd200 result = ceil(one);
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
      bcd200 one(_T("123456.789012345678"));
      CString result = one.AsDisplayString();
      TCHAR* expect = _T("123.456,79");
      Assert::AreEqual(expect,result.GetString());

      bcd200 two(_T("9123456.789012345678"));
      CString result2 = two.AsDisplayString();
      TCHAR* expect2 = _T("9.123.456,79");
      Assert::AreEqual(expect2,result2.GetString());
    }

    TEST_METHOD(T079_Sine_zero)
    {
      Logger::WriteMessage("Test Sine(0.0)");

      // Reputated to enter and endless loop
      bcd200 zero;
      bcd200 sineZero = zero.Sine();

      CString zeroResult(_T("0.00"));
      CString zeroSine = sineZero.AsString();

      Assert::AreEqual(zeroSine.GetString(),zeroResult.GetString());
    }

    TEST_METHOD(T080_Cosine_zero)
    {
      Logger::WriteMessage("Test Cosine(0.0)");

      // Reputated to enter and endless loop
      bcd200 zero;
      bcd200 cosineZero = zero.Cosine();

      CString zeroResult(_T("1.00"));
      CString zeroCosine = cosineZero.AsString();

      Assert::AreEqual(zeroCosine.GetString(), zeroResult.GetString());
    }

    TEST_METHOD(T081_Exponent_zero)
    {
      Logger::WriteMessage("Test Exp(0.0)");

      // Reputated to enter and endless loop
      bcd200 zero;
      bcd200 expZero = zero.Exp();

      CString zeroResult(_T("1.00"));
      CString zeroExponent = expZero.AsString();

      Assert::AreEqual(zeroExponent.GetString(), zeroResult.GetString());
    }

    TEST_METHOD(T082_Sine_pi)
    {
      Logger::WriteMessage("Test Sine(180 degrees)");

      // Reputationed to enter and endless loop
      bcd200 valuepi = bcd200::PI();
      bcd200 sinepi = valuepi.Sine();

      Assert::IsTrue(sinepi.IsNearZero());
    }

    TEST_METHOD(T083_Cosine_pi)
    {
      Logger::WriteMessage("Test Cosine(180 degrees)");

      // Reputated to enter and endless loop
      bcd200 valuepi = bcd200::PI();
      bcd200 cosinepi = valuepi.Cosine();
      cosinepi += 1;

      Assert::IsTrue(cosinepi.IsNearZero());
    }

    TEST_METHOD(T084_Exponents1)
    {
      Logger::WriteMessage("Test various exponents");

      bcd200 number(_T("0.01"));
      bcd200 expnumber = number.Exp();
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

      bcd200 number(_T("0.03"));
      bcd200 expnumber = number.Exp();
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

      bcd200 number(_T("0.1"));
      bcd200 numresult = number / 10.0;

      CString expected(_T("0.01"));
      CString result = numresult.AsString();
      
      Assert::AreEqual(expected.GetString(),result.GetString());
    }

    TEST_METHOD(T087_Divide10_Wrong)
    {
      Logger::WriteMessage("Test division by 10 wrong?");

      bcd200 numresult(0.2 / 10.0);

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
      bcd200    result = bcd200(0).Exp();

      Assert::AreEqual(expected, result.AsDouble());
    }

    TEST_METHOD(T089_Exponent_negative)
    {
      Logger::WriteMessage("Test exponent of negative number");

      double dblres = ::exp(-2.5);
      bcd200    bcdres = bcd200(_T("-2.5")).Exp();
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

      bcd200 tinynum(_T("0.01"));
      bcd200 number(_T("-0.05"));

      bcd200 result = tinynum + number;
      CString expected(_T("-0.04"));
      CString resstring = result.AsString();

      Assert::AreEqual(expected.GetString(),resstring.GetString());
    }

    TEST_METHOD(T091_Adding_negative)
    {
      Logger::WriteMessage("Adding small negative number to 0.0");

      // Original values provided by sisirajaya
      bcd200 tinynum(_T("0.0"));
      bcd200 number(_T("-0.05"));

      bcd200 result = tinynum + number;
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
//         bcd200 b = val;
//         __int64 back = b.AsInt64();
//         if(back != val)
//         {
//           Assert::Fail(L"bcd200 <-> integer64 just broke!");
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

      bcd200 one(_T("94505024884"));
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
      bcd200 one (num,64);
      CString expected(_T("94505024884.64"));
      CString result = one.AsString();

      Assert::AreEqual(expected.GetString(),result.GetString());
    }

    TEST_METHOD(T095_CornerCases)
    {
      Logger::WriteMessage("Testing integer corner cases.");

      bcd200 test1((TCHAR)MININT8);      Assert::AreEqual((int)MININT8,  (int)test1.AsShort());
      bcd200 test2((TCHAR)MAXINT8);      Assert::AreEqual((int)MAXINT8,  (int)test2.AsShort()); 

      bcd200 test3((uchar)0);           Assert::AreEqual(0,             (int)test3.AsShort()); 
      bcd200 test4((uchar)MAXUINT8);    Assert::AreEqual((int)MAXUINT8, (int)test4.AsShort());

      bcd200 test5((short)MININT16);    Assert::AreEqual((int)MININT16, (int)test5.AsShort()); 
      bcd200 test6((short)MAXINT16);    Assert::AreEqual((int)MAXINT16, (int)test6.AsShort()); 

      bcd200 test7((ushort)0);          Assert::AreEqual((int)0,        (int)test7.AsUShort()); 
      bcd200 test8((ushort)MAXUINT16);  Assert::AreEqual((int)MAXUINT16,(int)test8.AsUShort()); 

      bcd200 test9((long)MININT32);     Assert::AreEqual((int)MININT32, (int)test9.AsLong());
      bcd200 test10((long)MAXINT32);    Assert::AreEqual((int)MAXINT32, (int)test10.AsLong()); 

      bcd200 test11((ulong)0);          Assert::IsTrue(test11.AsULong() == 0); 
      bcd200 test12((ulong)MAXUINT32);  Assert::IsTrue(test12.AsULong() == MAXUINT32); 

      bcd200 test13((INT64)MININT64);   Assert::IsTrue(test13.AsInt64() == MININT64); 
      bcd200 test14((INT64)MAXINT64);   Assert::IsTrue(test14.AsInt64() == MAXINT64); 

      bcd200 test15((UINT64)0);         Assert::IsTrue(test15.AsUInt64() == 0L); 
      bcd200 test16((UINT64)MAXUINT64); Assert::IsTrue(test16.AsUInt64() == MAXUINT64);
    }

    TEST_METHOD(T096_TestNaturalLog)
    {
      Logger::WriteMessage("Testing BCD Natural Log 0.5");

      bcd200 value(_T("0.5"));
      bcd200 nlog = value.Log();

    //CString expected = _T("-0.693147180559945309417232121458176568023");
      CString expected = _T("-0.6931471805599453094172321214581765680755001343602552541206800094933936219696947156058633269964186875420014810205706857336855202357581305570326707516350759619307275708283714351903070386238916734711173");
      CString testval  = nlog.AsString(bcd200::Format::Bookkeeping, false, 30);

      Assert::AreEqual(expected.GetString(), testval.GetString());
    }

    TEST_METHOD(T097_CalculatePI)
    {
      Logger::WriteMessage("Testing BCD calculating PI");
      bcd200 value("-1");
      bcd200 pi = value.ArcCosine();
      bcd200 internalPi = bcd200::PI();

      CString disp = pi.AsString(bcd200::Format::Engineering);
      CString intr = internalPi.AsString(bcd200::Format::Engineering);

      bcd200 diff = pi - internalPi;
      Assert::IsTrue(diff.IsNearZero());
    }

    TEST_METHOD(T098_NaturalLog2)
    {
      Logger::WriteMessage("Testing BCD Natural Log 2");
      bcd200 value("2");
      bcd200 ln2      = value.Log();
      bcd200 internal = bcd200::LN2();

      bcd200 diff = ln2 - internal;
      Assert::IsTrue(diff.IsNearZero());
    }

    TEST_METHOD(T099_NaturalLog10)
    {
      Logger::WriteMessage("Testing BCD Natural Log 10");
      bcd200 value("10");
      bcd200 log = value.Log();
      bcd200 internal = bcd200::LN10();

      bcd200 diff = log - internal;
      Assert::IsTrue(diff.IsNearZero());
    }
  };
}
