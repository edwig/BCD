// Testing.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Testing.h"
#include "afp.h"
#include "icd.h"
#include "bcd.h"
#include "numeric.h"
#include "HPFCounter.h"

#pragma warning (disable: 4477)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef enum _testoperator
{
  Operator_add
 ,Operator_minus
 ,Operator_mult
 ,Operator_div
 ,Operator_mod
}
TestOperator;

typedef enum _testfunc
{
  Func_sine
 ,Func_cosine
 ,Func_tangent
 ,Func_asin
 ,Func_acos
 ,Func_atan
 ,Func_atan2
 ,Func_floor
 ,Func_ceiling
 ,Func_fabs
 ,Func_sqrt
 ,Func_log10
 ,Func_log
 ,Func_exp
 ,Func_pow
 ,Func_modf
 ,Func_fmod
 ,Func_frexp
 ,Func_ldexp
}
TestFunction;

LPCTSTR operators[] = 
{
  _T("adding")
 ,_T("subtracting")
 ,_T("multiplication")
 ,_T("division")
 ,_T("modulo")
};

LPCTSTR functions[] =
{
  _T("sine")
 ,_T("cosine")
 ,_T("tangent")
 ,_T("arcsine")
 ,_T("arccosine")
 ,_T("arctangent")
 ,_T("tangens2points")
 ,_T("floor")
 ,_T("ceiling")
 ,_T("absolute")
 ,_T("squareroot")
 ,_T("log10")
 ,_T("log")
 ,_T("exp")
 ,_T("power")
 ,_T("splitfloat")
 ,_T("modulo")
 ,_T("mantissa-exponent split")
 ,_T("mult-2-power")
 ,NULL
};

// The one and only application object

CWinApp theApp;

using namespace std;

double delta = 0;


// Calibrating the measurement tolerance (delta)
// Time spent in the loop without doing the actual measurement
void
Calibrate(int p_count)
{
  int p = 0;
  bcd num;
  HPFCounter count;
  for(int x = 0; x < p_count; ++x)
  {
    switch(p)
    {
      case 0: num = 0L; break;
    }
  }
  count.Stop();
  delta = count.GetCounter();
  _tprintf(_T("Calibrating delta = %.6f\n\n"),delta);
}

void
DoFunctionTest(TestFunction p_function
              ,LPCTSTR      p_number1
              ,LPCTSTR      p_number2
              ,LPCTSTR      p_expect
              ,LPCTSTR      p_extra
              ,int          p_count)
{
  bool extraFloat   = p_function == Func_modf  ? true : false;
  bool extraInteger = p_function == Func_frexp ? true : false;

  LPCTSTR name = functions[p_function];
  _tprintf(_T("Testing the function [%s] for a total of [%d] iterations:\n\n"),name,p_count);

  if(_tcscmp(p_number2,_T("0")))
  {
    _tprintf(_T("Input1: %s\n"),p_number1);
    _tprintf(_T("Input2: %s\n"),p_number2);
  }
  else
  {
    _tprintf(_T("Input: %s\n"),p_number1);
  }
  _tprintf(_T("\n"));

  _tprintf(_T("Type         Time Value\n"));
  _tprintf(_T("------ ---------- ------------------------------------------------------\n"));
  _tprintf(_T("calc     0.000000 +%s\n"),p_expect);
  if(extraInteger || extraFloat)
  {
    _tprintf(_T("         0.000000 +%s\n"),p_extra);
  }

  // DOUBLE
  double d_number1 = _ttof(p_number1);
  double d_number2 = _ttof(p_number2);
  double d_result;
  int    exponent  = _ttoi(p_number2);
  HPFCounter counter;
  for(int x = 0; x < p_count; ++x)
  {
    switch(p_function)
    {
      case Func_sine:     d_result = sin  (d_number1);  break;
      case Func_cosine:   d_result = cos  (d_number1);  break;
      case Func_tangent:  d_result = tan  (d_number1);  break;
      case Func_asin:     d_result = asin (d_number1);  break;
      case Func_acos:     d_result = acos (d_number1);  break;
      case Func_atan:     d_result = atan (d_number1);  break;
      case Func_atan2:    d_result = atan2(d_number1,d_number2); break;
      case Func_floor:    d_result = floor(d_number1);  break;
      case Func_ceiling:  d_result = ceil (d_number1);  break;
      case Func_fabs:     d_result = fabs (d_number1);  break;
      case Func_sqrt:     d_result = sqrt (d_number1);  break;
      case Func_log10:    d_result = log10(d_number1);  break;
      case Func_log:      d_result = log  (d_number1);  break;
      case Func_exp:      d_result = exp  (d_number1);  break;
      case Func_pow:      d_result = pow  (d_number1, d_number2);  break;
      case Func_modf:     d_result = modf (d_number1,&d_number2);  break;
      case Func_fmod:     d_result = fmod (d_number1, d_number2);  break;
      case Func_frexp:    d_result = frexp(d_number1,&exponent);   break;
      case Func_ldexp:    d_result = ldexp(d_number1, exponent);   break;
      default:            _tprintf(_T("Unknown function in DoFunctionTest -> double\n"));
                          break;
    }
  }
  counter.Stop();
  _tprintf(_T("double %10.6f +%.15f\n"),counter.GetCounter(),d_result);
  if(extraInteger)
  {
    _tprintf(_T("                  +%d\n"),exponent);
  }
  if(extraFloat)
  {
    _tprintf(_T("                  +%.15f\n"),d_number2);
  }

  // afp
  afp b_number1(p_number1,40);
  afp b_number2(p_number2,40);
  afp b_result(0,40);
  HPFCounter counter2;
  for(int x = 0; x < p_count; ++x)
  {
    switch(p_function)
    {
      case Func_sine:     b_result = sin  (b_number1);  break;
      case Func_cosine:   b_result = cos  (b_number1);  break;
      case Func_tangent:  b_result = tan  (b_number1);  break;
      case Func_asin:     b_result = asin (b_number1);  break;
      case Func_acos:     b_result = acos (b_number1);  break;
      case Func_atan:     b_result = atan (b_number1);  break;
      case Func_atan2:    b_result = atan2(b_number1,b_number2); break;
      case Func_floor:    b_result = floor(b_number1);  break;
      case Func_ceiling:  b_result = ceil (b_number1);  break;
      case Func_fabs:     b_result = fabs (b_number1);  break;
      case Func_sqrt:     b_result = sqrt (b_number1);  break;
      case Func_log10:    b_result = log10(b_number1);  break;
      case Func_log:      b_result = log  (b_number1);  break;
      case Func_exp:      b_result = exp  (b_number1);  break;
      case Func_pow:      b_result = pow  (b_number1, b_number2);  break;
      case Func_modf:     b_result = modf (b_number1,&b_number2);  break;
      case Func_fmod:     b_result = fmod (b_number1, b_number2);  break;
      case Func_frexp:    b_result = frexp(b_number1,&exponent);   break;
      case Func_ldexp:    b_result = ldexp(b_number1, exponent);   break;
      default:            _tprintf(_T("Unknown function in DoFunctionTest -> afp\n"));
                          break;
    }
  }
  counter2.Stop();
  _tprintf(_T("afp    %10.6f %s\n"),counter2.GetCounter(),_afp_ftoa(b_result,40,40).c_str());
  if(extraInteger)
  {
    _tprintf(_T("                  +%d\n"),exponent);
  }
  if(extraFloat)
  {
    _tprintf(_T("                  %s\n"),_afp_ftoa(b_number2,40,40).c_str());
  }

  // ICD8
  icd i_number1(p_number1);
  icd i_number2(p_number2);
  icd i_result;

  HPFCounter counter3;
  for(int x = 0; x < p_count; ++x)
  {
    switch(p_function)
    {
      case Func_sine:     i_result = sin  (i_number1);  break;
      case Func_cosine:   i_result = cos  (i_number1);  break;
      case Func_tangent:  i_result = tan  (i_number1);  break;
      case Func_asin:     i_result = asin (i_number1);  break;
      case Func_acos:     i_result = acos (i_number1);  break;
      case Func_atan:     i_result = atan (i_number1);  break;
      case Func_atan2:    i_result = atan2(i_number1,i_number2); break;
      case Func_floor:    i_result = floor(i_number1);  break;
      case Func_ceiling:  i_result = ceil (i_number1);  break;
      case Func_fabs:     i_result = fabs (i_number1);  break;
      case Func_sqrt:     i_result = sqrt (i_number1);  break;
      case Func_log10:    i_result = log10(i_number1);  break;
      case Func_log:      i_result = log  (i_number1);  break;
      case Func_exp:      i_result = exp  (i_number1);  break;
      case Func_pow:      i_result = pow  (i_number1, i_number2);  break;
      case Func_modf:     i_result = modf (i_number1,&i_number2);  break;
      case Func_fmod:     i_result = fmod (i_number1, i_number2);  break;
      case Func_frexp:    i_result = frexp(i_number1,&exponent);   break;
      case Func_ldexp:    i_result = ldexp(i_number1, exponent);   break;
      default:            _tprintf(_T("Unknown function in DoFunctionTest -> icd\n"));
                          break;
    }
  }
  counter3.Stop();
  _tprintf(_T("icd    %10.6f %s\n"),counter3.GetCounter(),i_result.AsString());
  if(extraInteger)
  {
    _tprintf(_T("                  +%d\n"),exponent);
  }
  if(extraFloat)
  {
    _tprintf(_T("                  %s\n"),i_number2.AsString());
  }


  // bcd
  bcd c_number1(p_number1);
  bcd c_number2(p_number2);
  bcd c_result;
  HPFCounter counter4;
  for(int x = 0; x < p_count; ++x)
  {
    switch(p_function)
    {
      case Func_sine:     c_result = sin  (c_number1);  break;
      case Func_cosine:   c_result = cos  (c_number1);  break;
      case Func_tangent:  c_result = tan  (c_number1);  break;
      case Func_asin:     c_result = asin (c_number1);  break;
      case Func_acos:     c_result = acos (c_number1);  break;
      case Func_atan:     c_result = atan (c_number1);  break;
      case Func_atan2:    c_result = atan2(c_number1,c_number2); break;
      case Func_floor:    c_result = floor(c_number1);  break;
      case Func_ceiling:  c_result = ceil (c_number1);  break;
      case Func_fabs:     c_result = fabs (c_number1);  break;
      case Func_sqrt:     c_result = sqrt (c_number1);  break;
      case Func_log10:    c_result = log10(c_number1);  break;
      case Func_log:      c_result = log  (c_number1);  break;
      case Func_exp:      c_result = exp  (c_number1);  break;
      case Func_pow:      c_result = pow  (c_number1, c_number2);  break;
      case Func_modf:     c_result = modf (c_number1,&c_number2);  break;
      case Func_fmod:     c_result = fmod (c_number1, c_number2);  break;
      case Func_frexp:    c_result = frexp(c_number1,&exponent);   break;
      case Func_ldexp:    c_result = ldexp(c_number1, exponent);   break;
      default:            _tprintf(_T("Unknown function in DoFunctionTest -> bcd\n"));
                          break;
    }
  }
  counter4.Stop();
  _tprintf(_T("bcd    %10.6f %s\n"),counter4.GetCounter(),c_result.AsString(bcd::Format::Bookkeeping,true));
  if(extraInteger)
  {
    _tprintf(_T("                  +%d\n"),exponent);
  }
  if(extraFloat)
  {
    _tprintf(_T("                  %s\n"),c_number2.AsString(bcd::Format::Bookkeeping,true));
  }
  
  _tprintf(_T("\n\n"));
}

void
DoOperatorTest(TestOperator p_operator,LPCTSTR p_een,LPCTSTR p_two,LPCTSTR p_expect,int p_count)
{
  LPCTSTR name = operators[p_operator];

  _tprintf(_T("Testing [%s] for a total of [%d] iterations:\n\n"),name,p_count);

  _tprintf(_T("Type         Time Value\n"));
  _tprintf(_T("------ ---------- ------------------------------------------------------\n"));
  _tprintf(_T("calc     0.000000 +%s\n"),p_expect);

  // DOUBLE
  double d_number1 = _ttof(p_een);
  double d_number2 = _ttof(p_two);
  double d_result;

  HPFCounter counter1;
  for(int x = 0;x < p_count; ++x)
  {
    switch(p_operator)
    {
      case Operator_add:   d_result = d_number1 + d_number2; break;
      case Operator_minus: d_result = d_number1 - d_number2; break;
      case Operator_mult:  d_result = d_number1 * d_number2; break;
      case Operator_div:   d_result = d_number1 / d_number2; break;
      case Operator_mod:   d_result = fmod(d_number1,d_number2); break;
    }
  }
  counter1.Stop();
  _tprintf(_T("double %10.6f +%.15f\n"),counter1.GetCounter(),d_result);

  // afp
  afp b_number1(p_een, 60);
  afp b_number2(p_two,60);
  afp b_result(0,60);

  HPFCounter counter2;
  for(int x = 0;x < p_count; ++x)
  {
    switch(p_operator)
    {
      case Operator_add:    b_result = b_number1 + b_number2; break;
      case Operator_minus:  b_result = b_number1 - b_number2; break;
      case Operator_mult:   b_result = b_number1 * b_number2; break;
      case Operator_div:    b_result = b_number1 / b_number2; break;
      case Operator_mod:    b_result = fmod(b_number1,b_number2); break;
    }
  }
  counter2.Stop();
  _tprintf(_T("afp    %10.6f %s\n"),counter2.GetCounter(),_afp_ftoa(b_result,60,60).c_str());

 
  // ICD (Improved)
  icd i_getal1(p_een);
  icd i_getal2(p_two);
  icd i_result;

  HPFCounter counter4;
  for(int x = 0;x < p_count; ++x)
  {
    switch(p_operator)
    {
      case Operator_add:    i_result = i_getal1 + i_getal2; break;
      case Operator_minus:  i_result = i_getal1 - i_getal2; break;
      case Operator_mult:   i_result = i_getal1 * i_getal2; break;
      case Operator_div:    i_result = i_getal1 / i_getal2; break;
      case Operator_mod:    i_result = i_getal1 % i_getal2; break;
    }
  }
  counter4.Stop();
  _tprintf(_T("icd    %10.6f %s\n"),counter4.GetCounter(),i_result.AsString());

  bcd c_number1(p_een);
  bcd c_number2(p_two);
  bcd c_result;

  HPFCounter counter5;
  for(int x = 0;x < p_count; ++x)
  {
    switch(p_operator)
    {
      case Operator_add:    c_result = c_number1 + c_number2; break;
      case Operator_minus:  c_result = c_number1 - c_number2; break;
      case Operator_mult:   c_result = c_number1 * c_number2; break;
      case Operator_div:    c_result = c_number1 / c_number2; break;
      case Operator_mod:    c_result = c_number1 % c_number2; break;
    }
  }
  counter5.Stop();
  _tprintf(_T("bcd    %10.6f %s\n"),counter5.GetCounter(),c_result.AsString(bcd::Format::Bookkeeping,true));


//   numeric n_number1(32,16,p_een);
//   numeric n_number2(32,16,p_two);
//   numeric n_result;
// 
//   HPFCounter counter6;
//   for (int x = 0; x < p_count; ++x)
//   {
//     switch(p_operator)
//     {
//       case Operator_add:    n_result = n_number1 + n_number2; break;
//       case Operator_minus:  n_result = n_number1 - n_number2; break;
//       case Operator_mult:   n_result = n_number1 * n_number2; break;
//       case Operator_div:    n_result = n_number1 / n_number2; break;
//       case Operator_mod:    n_result = n_number1 % n_number2; break;
//     }
//   }
//   counter6.Stop();
//   _tprintf("numeric%10.6f %s\n", counter6.GetCounter(), n_result.AsString(numeric::Format::Bookkeeping,true));

  _tprintf(_T("\n\n"));
}

void
PrintConstants(int p_count)
{
  afp  b_pi  (0,41);
  afp  b_ln10(0,41);
  afp  b_ln2 (0,41);
  icd  i_pi;
  icd  i_ln10;
  icd  i_ln2;
  bcd  c_pi;
  bcd  c_ln10;
  bcd  c_ln2;
  numeric n_pi;

  _tprintf(_T("Floating point constants in [%d] iterations are:\n\n"),p_count);
  _tprintf(_T("Constant Type   Time     Value\n"));
  _tprintf(_T("-------  ------ -------- ---------------------------------------------\n"));

  // Calculate PI
  HPFCounter count1;
  for(int x = 0;x < p_count; ++x)
  {
    b_pi = _float_table(_PI,41);
  }
  count1.Stop();
  HPFCounter count2;
  for(int x = 0;x < p_count; ++x)
  {
    i_pi = icd::PI();
  }
  HPFCounter count3;
  for(int x = 0;x < p_count; ++x)
  {
    c_pi = bcd::PI();
  }
  count3.Stop();
//   HPFCounter count4;
//   for (int x = 0; x < p_count; ++x)
//   {
//     n_pi = numeric::PI(32,30);
//   }
//   count4.Stop();
// 
//   _tprintf("PI       Calc   0.000000 +3.1415926535897932384626433832795\n");
//   _tprintf("         afp    %0.6f %s\n",count1.GetCounter(),_afp_ftoa(b_pi,41,41).c_str());
//   _tprintf("         icd    %0.6f %s\n",count2.GetCounter(),i_pi.AsString());
//   _tprintf("         bcd    %0.6f %s\n",count3.GetCounter(),c_pi.AsString(bcd::Bookkeeping,true));
//   _tprintf("         numeric%0.6f %s\n",count4.GetCounter(),n_pi.AsString(numeric::Format::Bookkeeping,true));

  // BEREKEN LN(10)
  count1.Start();
  for(int x = 0; x < p_count; ++x)
  {
    b_ln10 = _float_table(_LN10,41);
  }
  count1.Stop();
  count2.Start();
  for(int x = 0; x < p_count; ++x)
  {
    i_ln10 = icd::LN10();
  }
  count2.Stop();
  count3.Start();
  for(int x = 0; x < p_count; ++x)
  {
    c_ln10 = bcd::LN10();
  }
  count3.Stop();

  _tprintf(_T("LN10     Calc   0.000000 +2.3025850929940456840179914546844\n"));
  _tprintf(_T("         afp    %0.6f %s\n"),count1.GetCounter(),_afp_ftoa(b_ln10,41).c_str());
  _tprintf(_T("         icd    %0.6f %s\n"),count2.GetCounter(),i_ln10.AsString());
  _tprintf(_T("         bcd    %0.6f %s\n"),count3.GetCounter(),c_ln10.AsString(bcd::Format::Bookkeeping,true));

  // BEREKEN LN(2)
  count1.Start();
  for(int x = 0;x < p_count; ++x)
  {
    b_ln2 = _float_table(_LN2,41);
  }
  count1.Stop();
  count2.Start();
  for(int x = 0;x < p_count; ++x)
  {
    i_ln2 = icd::LN2();
  }
  count2.Stop();
  count3.Start();
  for(int x = 0;x < p_count; ++x)
  {
    c_ln2 = bcd::LN2();
  }
  count3.Stop();

  _tprintf(_T("LN2      Calc   0.000000 +0.69314718055994530941723212145818\n"));
  _tprintf(_T("         afp    %0.6f %s\n"),count1.GetCounter(),_afp_ftoa(b_ln2,41).c_str());
  _tprintf(_T("         icd    %0.6f %s\n"),count2.GetCounter(),i_ln2.AsString());
  _tprintf(_T("         bcd    %0.6f %s\n"),count3.GetCounter(),c_ln2.AsString(bcd::Format::Bookkeeping,true));

  _tprintf(_T("\n\n"));
}

void
TestOperatoren(int p_count)
{
  LPCTSTR a_big1      = _T("1234567890123456.1234567890123456");
  LPCTSTR a_big2      =                _T("5.1234567890123456");
  LPCTSTR a_big3      =  _T("876543210876543.21087654321087654321");
  LPCTSTR r_big_mult  = _T("6325255238149668.8052126159533604");
  LPCTSTR r_big_div   =  _T("240963853305269.14298709106993387");
  LPCTSTR r_big_mod   =                _T("0.7325881824833792");

  LPCTSTR a_small1    = _T("0.8347366295099261173476093153791068840878");
  LPCTSTR a_small2    = _T("0.5506493978516714425340844117564672940305");
  LPCTSTR r_small_add = _T("2111111100999999.3343333322232221");
  LPCTSTR r_small_div = _T("1.5159130887395963684392407742872"); 
  LPCTSTR r_big_min   = _T("358024679246912.91258024580146906");

  DoOperatorTest(Operator_mult, a_big1,   a_big2,   r_big_mult,   p_count);
  DoOperatorTest(Operator_div,  a_big1,   a_big2,   r_big_div,    p_count);
  DoOperatorTest(Operator_add,  a_big1,   a_big3,   r_small_add,  p_count);
  DoOperatorTest(Operator_minus,a_big1,   a_big3,   r_big_min,    p_count);
  DoOperatorTest(Operator_mod,  a_big1,   a_big2,   r_big_mod,    p_count);
  DoOperatorTest(Operator_div,  a_small1, a_small2, r_small_div,  p_count);
}

void
TestFuncties(int p_count)
{
  LPCTSTR a_angle     = _T("0.9876543210123456");
  LPCTSTR r_sine      = _T("0.83473662950992611734760931537911");
  LPCTSTR r_cosine    = _T("0.55064939785167144253408441175647");
  LPCTSTR r_tangent   = _T("1.5159130887395963684392407742872");
  
  LPCTSTR a_ratio     = _T("0.765498765404321098765");
  LPCTSTR r_asine     = _T("0.87181613107055910102494602134303");
  LPCTSTR r_acosine   = _T("0.69898019572433751820637567029672");
  LPCTSTR r_atangent  = _T("0.653346752384431270749403109172");

  LPCTSTR a_big       =  _T("98765432109876543210.123456789012345678901234567890");
  LPCTSTR a_big_n     = _T("-98765432109876543210.123456789012345678901234567890");
  LPCTSTR a_small1    =  _T("26.5566778899001122334455");
  LPCTSTR a_small2    =   _T("7.6655443322110099887766");
  LPCTSTR a_mini      =   _T("0.00000000000000077665544332211998877665544332211");
  LPCTSTR r_sqroot    =  _T("9938079900.558082311745752865316");
  LPCTSTR r_floor     =  _T("98765432109876543210");
  LPCTSTR r_ceiling   =  _T("98765432109876543211");
  LPCTSTR r_log10     =  _T("19.994604968162151965673558368195");
  LPCTSTR r_log       =  _T("46.039279339994856527044707840045");
  LPCTSTR r_exp       =  _T("341521984409.089389680737393624");
  LPCTSTR r_pow       =  _T("82616536947.2042654425347359351");

  LPCTSTR a_big1      = _T("1234567890123456.1234567890123456");
  LPCTSTR a_big2      =                _T("5.1234567890123456");
  LPCTSTR r_big_mod   =                _T("0.7325881824833792");
  LPCTSTR r_modf1     =                _T("0.1234567890123456");
  LPCTSTR r_modf2     = _T("1234567890123456");
  LPCTSTR r_frexp1    = _T("1.2345678901234561234567890123456");
  LPCTSTR r_frexp2    = _T("15");
  LPCTSTR r_frexp3    = _T("7.7665544332211998877665544332211");
  LPCTSTR r_frexp4    = _T("-16");
  LPCTSTR r_ldexp     = _T("3399.254769907214365881024");

  DoFunctionTest(Func_sine,   a_angle,     _T("0"),      r_sine,     _T("0"),         p_count);
  DoFunctionTest(Func_cosine, a_angle,     _T("0"),      r_cosine,   _T("0"),         p_count);
  DoFunctionTest(Func_tangent,a_angle,     _T("0"),      r_tangent,  _T("0"),         p_count);
  DoFunctionTest(Func_asin,   a_ratio,     _T("0"),      r_asine,    _T("0"),         p_count);
  DoFunctionTest(Func_acos,   a_ratio,     _T("0"),      r_acosine,  _T("0"),         p_count);
  DoFunctionTest(Func_atan,   a_ratio,     _T("0"),      r_atangent, _T("0"),         p_count);
  DoFunctionTest(Func_sqrt,   a_big,       _T("0"),      r_sqroot,   _T("0"),         p_count);
  DoFunctionTest(Func_floor,  a_big,       _T("0"),      r_floor,    _T("0"),         p_count);
  DoFunctionTest(Func_ceiling,a_big,       _T("0"),      r_ceiling,  _T("0"),         p_count);
  DoFunctionTest(Func_fabs,   a_big_n,     _T("0"),      a_big,      _T("0"),         p_count);
  DoFunctionTest(Func_log10,  a_big,       _T("0"),      r_log10,    _T("0"),         p_count);
  DoFunctionTest(Func_log,    a_big,       _T("0"),      r_log,      _T("0"),         p_count);
  DoFunctionTest(Func_exp,    a_small1,    _T("0"),      r_exp,      _T("0"),         p_count);
  DoFunctionTest(Func_pow,    a_small1,    a_small2,     r_pow,      _T("0"),         p_count);
  DoFunctionTest(Func_modf,   a_big1,      _T("0"),      r_modf1,    r_modf2,         p_count);
  DoFunctionTest(Func_fmod,   a_big1,      a_big2,       r_big_mod,  _T("0"),         p_count);
  DoFunctionTest(Func_frexp,  a_big1,      _T("0"),      r_frexp1,   r_frexp2,        p_count);
  DoFunctionTest(Func_frexp,  a_big_n,     _T("0"),      r_frexp1,   r_frexp2,        p_count);
  DoFunctionTest(Func_frexp,  a_mini,      _T("0"),      r_frexp3,   r_frexp4,        p_count);
  DoFunctionTest(Func_ldexp,  a_small1,    a_small2,     r_ldexp,    _T("0"),         p_count);
}

int
TestNumeric(int p_count)
{
  // Header
  _tprintf(_T("Testing SQL_NUMERIC_STRUCT for a total of [%d] iterations\n\n"),p_count);

  // num = 10.001 (ten and 1 thousandth)
  SQL_NUMERIC_STRUCT num;
  SQL_NUMERIC_STRUCT res;
  memset(&num,0,sizeof(SQL_NUMERIC_STRUCT));
  memset(&res,0,sizeof(SQL_NUMERIC_STRUCT));
  num.sign = 1; // Positive
  num.precision = 6;
  num.scale     = 4;

  num.val[0] = 0xAA;
  num.val[1] = 0x86;
  num.val[2] = 0x01;

  HPFCounter counter;
  for(int x = 0; x < p_count; ++x)
  {
    bcd ten(&num);
  }
  counter.Stop();
  bcd ten(&num);
  _tprintf(_T("SQL_NUMERIC_STRUCT -> bcd %10.6f : %s\n"),counter.GetCounter(),ten.AsString());

  // Now back again to a SQL_NUMERIC_STRUCT
  HPFCounter cnt2;
  for(int x = 0; x < p_count; ++x)
  {
    ten.AsNumeric(&res);
  }
  cnt2.Stop();


//     _tprintf("Precision: %d\n",num.precision);
//     _tprintf("Scale    : %d\n",num.scale);
//     _tprintf("Sign     : %d\n",num.sign);
// 
//     for(unsigned ind = 0;ind < SQL_MAX_NUMERIC_LEN; ++ind)
//     {
//       _tprintf("Numeric mantissa [%d:%02.2X]\n",ind,num.val[ind]);
//     }

  bcd check(&num);
  _tprintf(_T("bcd -> SQL_NUMERIC_STRUCT %10.6f : %s\n"),cnt2.GetCounter(),check.AsString());

  _tprintf(_T("\n"));
  return 0;
}

int 
_tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule == NULL)
  {
    // TODO: change error code to suit your needs
    _tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
    return 1;
  }
	// initialize MFC and print and error on failure
	if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		return 1;
	}
  _tprintf(_T("TESTPROGRAM EXACT NUMERICS WITH LARGE PRECISION\n"));
  _tprintf(_T("===============================================\n"));
  _tprintf(_T("\n"));
  _tprintf(_T("Legenda:\n"));
  _tprintf(_T("--------\n"));
  _tprintf(_T("calc    -> Calculations in MS-Calc (standard calculator)\n"));
  _tprintf(_T("double  -> Build in C++ datatype 'double'\n"));
  _tprintf(_T("afp     -> Datatype 'Arbitrairy Floating Point' of Henrik Vestermark\n"));
  _tprintf(_T("icd     -> Datatype 'Integer Coded Decimal'\n"));
  _tprintf(_T("bcd     -> Datatype 'Binairy Coded Decimal'\n"));
//_tprintf("numeric -> Datatype 'Numeric / Decimal'\n");
  _tprintf(_T("\n"));

#ifdef DEBUG
  int count = 10;     // Quicker in debug mode 
#else
  int count = 1000;   // The real deal!!!
#endif
  Calibrate(count);

  // Constants
  PrintConstants(count);

  // Operators
  TestOperatoren(count);

  // Functions
  TestFuncties(count);

  // SQL DECIMAL/NUMERIC conversions
  TestNumeric(count);

  // Wait until user has seen the result
  _tprintf(_T("Seen the output? "));
  char buffer[1024];
  gets_s(buffer,1024);

  return 0;
}


