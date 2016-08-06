// Testing.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Testing.h"
#include "afp.h"
#include "icd4.h"
#include "icd.h"
#include "bcd.h"
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

char* operators[] = 
{
  "adding"
 ,"subtracting"
 ,"multiplication"
 ,"division"
 ,"modulo"
};

char* functions[] =
{
  "sine"
 ,"cosine"
 ,"tangent"
 ,"arcsine"
 ,"arccosine"
 ,"arctangent"
 ,"tangens2points"
 ,"floor"
 ,"ceiling"
 ,"absolute"
 ,"squareroot"
 ,"log10"
 ,"log"
 ,"exp"
 ,"power"
 ,"splitfloat"
 ,"modulo"
 ,"mantissa-exponent split"
 ,"mult-2-power"
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
  printf("Calibrating delta = %.6f\n\n",delta);
}

void
DoFunctionTest(TestFunction p_function
              ,char*        p_number1
              ,char*        p_number2
              ,char*        p_expect
              ,char*        p_extra
              ,int          p_count)
{
  bool extraFloat   = p_function == Func_modf  ? true : false;
  bool extraInteger = p_function == Func_frexp ? true : false;

  char* name = functions[p_function];
  printf("Testing the function [%s] for a total of [%d] iterations:\n\n",name,p_count);

  if(strcmp(p_number2,"0"))
  {
    printf("Input1: %s\n",p_number1);
    printf("Input2: %s\n",p_number2);
  }
  else
  {
    printf("Input: %s\n",p_number1);
  }
  printf("\n");

  printf("Type         Time Value\n");
  printf("------ ---------- ------------------------------------------------------\n");
  printf("calc     0.000000 +%s\n",p_expect);
  if(extraInteger || extraFloat)
  {
    printf("         0.000000 +%s\n",p_extra);
  }

  // DOUBLE
  double d_number1 = atof(p_number1);
  double d_number2 = atof(p_number2);
  double d_result;
  int    exponent  = atoi(p_number2);
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
      default:            printf("Unknown function in DoFunctionTest -> double\n");
                          break;
    }
  }
  counter.Stop();
  printf("double %10.6f +%.15f\n",counter.GetCounter(),d_result);
  if(extraInteger)
  {
    printf("                  +%d\n",exponent);
  }
  if(extraFloat)
  {
    printf("                  +%.15f\n",d_number2);
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
      default:            printf("Unknown function in DoFunctionTest -> afp\n");
                          break;
    }
  }
  counter2.Stop();
  printf("afp    %10.6f %s\n",counter2.GetCounter(),_afp_ftoa(b_result,40,40).c_str());
  if(extraInteger)
  {
    printf("                  +%d\n",exponent);
  }
  if(extraFloat)
  {
    printf("                  %s\n",_afp_ftoa(b_number2,40,40).c_str());
  }

  // ICD
  char buf1[200];
  char buf2[200];
  strcpy_s(buf1,200,p_number1);
  strcpy_s(buf2,200,p_number2);
  char* pos1 = strchr(buf1,'.');
  char* pos2 = strchr(buf2,'.');
  if(pos1) *pos1 = ',';
  if(pos2) *pos2 = ',';

  //ICD4
  Icd4 i4_number1(buf1);
  Icd4 i4_number2(buf2);
  Icd4 i4_result;

  HPFCounter counter5;
  for(int x = 0; x < p_count; ++x)
  {
    switch(p_function)
    {
      case Func_sine:     i4_result = sin  (i4_number1);  break;
      case Func_cosine:   i4_result = cos  (i4_number1);  break;
      case Func_tangent:  i4_result = tan  (i4_number1);  break;
      case Func_asin:     i4_result = asin (i4_number1);  break;
      case Func_acos:     i4_result = acos (i4_number1);  break;
      case Func_atan:     i4_result = atan (i4_number1);  break;
      case Func_atan2:    i4_result = atan2(i4_number1,i4_number2); break;
      case Func_floor:    i4_result = floor(i4_number1);  break;
      case Func_ceiling:  i4_result = ceil (i4_number1);  break;  
      case Func_fabs:     i4_result = fabs (i4_number1);  break;
      case Func_sqrt:     i4_result = sqrt (i4_number1);  break;
      case Func_log10:    i4_result = log10(i4_number1);  break;
      case Func_log:      i4_result = log  (i4_number1);  break;
      case Func_exp:      i4_result = exp  (i4_number1);  break;
      case Func_pow:      i4_result = pow  (i4_number1, i4_number2);  break;
      case Func_modf:     i4_result = modf (i4_number1,&i4_number2);  break;
      case Func_fmod:     i4_result = fmod (i4_number1, i4_number2);  break;
      case Func_frexp:    i4_result = frexp(i4_number1,&exponent);    break;
      case Func_ldexp:    i4_result = ldexp(i4_number1, exponent);    break;
      default:            printf("Unknown function in DoFunctionTest -> icd4\n");
                          break;
    }
  }
  counter5.Stop();
  printf("Icd4   %10.6f %s\n",counter5.GetCounter(),i4_result.AlsString());
  if(extraInteger)
  {
    printf("                  +%d\n",exponent);
  }
  if(extraFloat)
  {
    printf("                  %s\n",i4_number2.AlsString());
  }


  // ICD8
  Icd i_number1(buf1);
  Icd i_number2(buf2);
  Icd i_result;

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
      default:            printf("Unknown function in DoFunctionTest -> icd8\n");
                          break;
    }
  }
  counter3.Stop();
  printf("Icd8   %10.6f %s\n",counter3.GetCounter(),i_result.AlsString());
  if(extraInteger)
  {
    printf("                  +%d\n",exponent);
  }
  if(extraFloat)
  {
    printf("                  %s\n",i_number2.AlsString());
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
      default:            printf("Unknown function in DoFunctionTest -> bcd\n");
                          break;
    }
  }
  counter4.Stop();
  printf("bcd    %10.6f %s\n",counter4.GetCounter(),c_result.AsString(bcd::Bookkeeping,true));
  if(extraInteger)
  {
    printf("                  +%d\n",exponent);
  }
  if(extraFloat)
  {
    printf("                  %s\n",c_number2.AsString(bcd::Bookkeeping,true));
  }
  
  printf("\n\n");
}

void
DoOperatorTest(TestOperator p_operator,char* p_een,char* p_two,char* p_expect,int p_count)
{
  char* name = operators[p_operator];

  printf("Testing [%s] for a total of [%d] iterations:\n\n",name,p_count);

  printf("Type         Time Value\n");
  printf("------ ---------- ------------------------------------------------------\n");
  printf("calc     0.000000 +%s\n",p_expect);

  // DOUBLE
  double d_number1 = atof(p_een);
  double d_number2 = atof(p_two);
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
  printf("double %10.6f +%.15f\n",counter1.GetCounter(),d_result);

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
  printf("afp    %10.6f %s\n",counter2.GetCounter(),_afp_ftoa(b_result,60,60).c_str());

  // ICD 4 and improved
  char buf1[200];
  char buf2[200];
  strcpy_s(buf1,200,p_een);
  strcpy_s(buf2,200,p_two);
  char* pos1 = strchr(buf1,'.');
  char* pos2 = strchr(buf2,'.');
  if(pos1) *pos1 = ',';
  if(pos2) *pos2 = ',';


  // ICD4
  Icd4 i4_number1(buf1);
  Icd4 i4_number2(buf2);
  Icd4 i4_result;

  HPFCounter counter3;
  for(int x = 0;x < p_count; ++x)
  {
    switch(p_operator)
    {
      case Operator_add:    i4_result = i4_number1 + i4_number2; break;
      case Operator_minus:  i4_result = i4_number1 - i4_number2; break;
      case Operator_mult:   i4_result = i4_number1 * i4_number2; break;
      case Operator_div:    i4_result = i4_number1 / i4_number2; break;
      case Operator_mod:    i4_result = i4_number1 % i4_number2; break;
    }
  }
  counter3.Stop();
  printf("icd4   %10.6f %s\n",counter3.GetCounter(),i4_result.AlsString());


  // ICD (Improved)
  Icd i_getal1(buf1);
  Icd i_getal2(buf2);
  Icd i_result;

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
  printf("icd8   %10.6f %s\n",counter4.GetCounter(),i_result.AlsString());

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
  printf("bcd    %10.6f %s\n",counter5.GetCounter(),c_result.AsString(bcd::Bookkeeping,true));

  printf("\n\n");
}

void
PrintConstants(int p_count)
{
  afp  b_pi  (0,41);
  afp  b_ln10(0,41);
  afp  b_ln2 (0,41);
  Icd4 i4_pi;
  Icd4 i4_ln10;
  Icd4 i4_ln2;
  Icd  i_pi;
  Icd  i_ln10;
  Icd  i_ln2;
  bcd  c_pi;
  bcd  c_ln10;
  bcd  c_ln2;

  printf("Floating point constants in [%d] iterations are:\n\n",p_count);
  printf("Constant Type   Time     Value\n");
  printf("-------  ------ -------- ---------------------------------------------\n");

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
    i_pi = Icd::PI();
  }
  HPFCounter count3;
  for(int x = 0;x < p_count; ++x)
  {
    c_pi = bcd::PI();
  }
  count3.Stop();
  HPFCounter count4;
  for(int x = 0;x < p_count; ++x)
  {
    i4_pi = Icd4::PI();
  }
  count4.Stop();
  printf("PI       Calc   0.000000 +3,1415926535897932384626433832795\n");
  printf("         afp    %0.6f %s\n",count1.GetCounter(),_afp_ftoa(b_pi,41,41).c_str());
  printf("         Icd4   %0.6f %s\n",count4.GetCounter(),i4_pi.AlsString());
  printf("         Icd8   %0.6f %s\n",count2.GetCounter(),i_pi.AlsString());
  printf("         bcd    %0.6f %s\n",count3.GetCounter(),c_pi.AsString(bcd::Bookkeeping,true));

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
    i_ln10 = Icd::LN10();
  }
  count2.Stop();
  count3.Start();
  for(int x = 0; x < p_count; ++x)
  {
    c_ln10 = bcd::LN10();
  }
  count3.Stop();
  count4.Start();
  for(int x = 0; x < p_count; ++x)
  {
    i4_ln10 = Icd4::LN10();
  }
  count4.Stop();
  printf("LN10     Calc   0.000000 +2,3025850929940456840179914546844\n");
  printf("         afp    %0.6f %s\n",count1.GetCounter(),_afp_ftoa(b_ln10,41).c_str());
  printf("         Icd4   %0.6f %s\n",count4.GetCounter(),i4_ln10.AlsString());
  printf("         Icd8   %0.6f %s\n",count2.GetCounter(),i_ln10.AlsString());
  printf("         bcd    %0.6f %s\n",count3.GetCounter(),c_ln10.AsString(bcd::Bookkeeping,true));

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
    i_ln2 = Icd::LN2();
  }
  count2.Stop();
  count3.Start();
  for(int x = 0;x < p_count; ++x)
  {
    c_ln2 = bcd::LN2();
  }
  count3.Stop();
  count4.Start();
  for(int x = 0;x < p_count; ++x)
  {
    i4_ln2 = Icd4::LN2();
  }
  count4.Stop();
  printf("LN2      Calc   0.000000 +0,69314718055994530941723212145818\n");
  printf("         afp    %0.6f %s\n",count1.GetCounter(),_afp_ftoa(b_ln2,41).c_str());
  printf("         Icd4   %0.6f %s\n",count4.GetCounter(),i4_ln2.AlsString());
  printf("         Icd8   %0.6f %s\n",count2.GetCounter(),i_ln2.AlsString());
  printf("         bcd    %0.6f %s\n",count3.GetCounter(),c_ln2.AsString(bcd::Bookkeeping,true));

  printf("\n\n");
}

void
TestOperatoren(int p_count)
{
  char* a_big1      = "1234567890123456.1234567890123456";
  char* a_big2      =                "5.1234567890123456";
  char* a_big3      =  "876543210876543.21087654321087654321";
  char* r_big_mult  = "6325255238149668.8052126159533604";
  char* r_big_div   =  "240963853305269.14298709106993387";
  char* r_big_mod   =                "0.7325881824833792";

  char* a_small1    = "0.8347366295099261173476093153791068840878";
  char* a_small2    = "0.5506493978516714425340844117564672940305";
  char* r_small_add = "1.5159130887395963684392407742872";
  char* r_small_div = "2111111100999999,3343333322232221";
  char* r_big_min   =  "358024679246912,91258024580146906";

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
  char* a_angle     = "0.9876543210123456";
  char* r_sine      = "0,83473662950992611734760931537911";
  char* r_cosine    = "0,55064939785167144253408441175647";
  char* r_tangent   = "1,5159130887395963684392407742872";
  
  char* a_ratio     =  "0.765498765404321098765";
  char* r_asine     = "0,87181613107055910102494602134303";
  char* r_acosine   = "0,69898019572433751820637567029672";
  char* r_atangent  = "0,653346752384431270749403109172";

  char* a_big       =  "98765432109876543210.123456789012345678901234567890";
  char* a_big_n     = "-98765432109876543210.123456789012345678901234567890";
  char* a_small1    =  "26.5566778899001122334455";
  char* a_small2    =   "7.6655443322110099887766";
  char* a_mini      =   "0.00000000000000077665544332211998877665544332211";
  char* r_sqroot    =  "9938079900.558082311745752865316";
  char* r_floor     =  "98765432109876543210";
  char* r_ceiling   =  "98765432109876543211";
  char* r_log10     =  "19,994604968162151965673558368195";
  char* r_log       =  "46,039279339994856527044707840045";
  char* r_exp       =  "341521984409,089389680737393624";
  char* r_pow       =  "82616536947,2042654425347359351";

  char* a_big1      = "1234567890123456.1234567890123456";
  char* a_big2      =                "5.1234567890123456";
  char* r_big_mod   =                "0.7325881824833792";
  char* r_modf1     =                "0.1234567890123456";
  char* r_modf2     = "1234567890123456";
  char* r_frexp1    = "1.2345678901234561234567890123456";
  char* r_frexp2    = "15";
  char* r_frexp3    = "7,7665544332211998877665544332211";
  char* r_frexp4    = "-16";
  char* r_ldexp     = "3399.254769907214365881024";

  DoFunctionTest(Func_sine,   a_angle,     "0",      r_sine,     "0",         p_count);
  DoFunctionTest(Func_cosine, a_angle,     "0",      r_cosine,   "0",         p_count);
  DoFunctionTest(Func_tangent,a_angle,     "0",      r_tangent,  "0",         p_count);
  DoFunctionTest(Func_asin,   a_ratio,     "0",      r_asine,    "0",         p_count);
  DoFunctionTest(Func_acos,   a_ratio,     "0",      r_acosine,  "0",         p_count);
  DoFunctionTest(Func_atan,   a_ratio,     "0",      r_atangent, "0",         p_count);
  DoFunctionTest(Func_sqrt,   a_big,       "0",      r_sqroot,   "0",         p_count);
  DoFunctionTest(Func_floor,  a_big,       "0",      r_floor,    "0",         p_count);
  DoFunctionTest(Func_ceiling,a_big,       "0",      r_ceiling,  "0",         p_count);
  DoFunctionTest(Func_fabs,   a_big_n,     "0",      a_big,      "0",         p_count);
  DoFunctionTest(Func_log10,  a_big,       "0",      r_log10,    "0",         p_count);
  DoFunctionTest(Func_log,    a_big,       "0",      r_log,      "0",         p_count);
  DoFunctionTest(Func_exp,    a_small1,    "0",      r_exp,      "0",         p_count);
  DoFunctionTest(Func_pow,    a_small1,    a_small2, r_pow,      "0",         p_count);
  DoFunctionTest(Func_modf,   a_big1,      "0",      r_modf1,    r_modf2,     p_count);
  DoFunctionTest(Func_fmod,   a_big1,      a_big2,   r_big_mod,  "0",         p_count);
  DoFunctionTest(Func_frexp,  a_big1,      "0",      r_frexp1,   r_frexp2,    p_count);
  DoFunctionTest(Func_frexp,  a_big_n,     "0",      r_frexp1,   r_frexp2,    p_count);
  DoFunctionTest(Func_frexp,  a_mini,      "0",      r_frexp3,   r_frexp4,    p_count);
  DoFunctionTest(Func_ldexp,  a_small1,    a_small2, r_ldexp,    "0",         p_count);
}

int
Test()
{
  double test = 0.123456789012345678901234567890;
  printf("Test: %.30lf\n\n",(test * 432));
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
  printf("TESTPROGRAM EXACT NUMERICS WITH LARGE PRECISION\n");
  printf("===============================================\n");
  printf("\n");
  printf("Legenda:\n");
  printf("--------\n");
  printf("calc   -> Calculations in MS-Calc (standard calculator)\n");
  printf("double -> Build in C++ datatype 'double'\n");
  printf("afp    -> Datatype 'Arbitrairy Floating Point' of Henrik Vestermark\n");
  printf("Icd4   -> Datatype 'Integer Coded Decimal' of the Pronto(tm) project\n");
  printf("Icd8   -> Datatype 'Integer Coded Decimal' (improved by EH)\n");
  printf("bcd    -> Datatype 'Binairy Coded Decimal' of Edwig Huisman\n");
  printf("\n");

#ifdef DEBUG
  int count = 1;      // Quicker in debug mode 
#else
  int count = 1000;   // The real deal!!!
#endif
  Calibrate(count);

  // Small test program
  Test();

  // Constants
  PrintConstants(count);

  // Operators
  TestOperatoren(count);

  // Functions
  TestFuncties(count);

  // Wait until user has seen the result
  printf("Seen the output? ");
  char buffer[1024];
  gets_s(buffer,1024);

  return 0;
}


