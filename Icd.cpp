// ICD = INTEGER CODED DECIMAL
// A class to work with large decimal numbers
// in exact precisions in decimals as opposed to
// the built in data types (float, double) that
// do a binary approximation 
//
#include "Stdafx.h"
#include <math.h>   // Needed for conversions and estimates
#include <float.h>  // Needed for estimates
#include <limits.h> // For max sizes of int, long and int64
#include "Icd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
//
//  ICD CONSTRUCTORS / DESTRUCTORS
//
//////////////////////////////////////////////////////////////////////////

// Icd::Icd
// Description:   default constructor
// What it does:  Initializes the icd to 0.0
//
icd::icd()
{
  MakeEmpty();
}

// Icd::Icd
// Description:   copy constructor
// Parameters:    const icd& 
// What it does:  copies all data members
//
icd::icd(const icd& p_icd)
{
  m_sign      = p_icd.m_sign;
  m_length    = p_icd.m_length;
  m_precision = p_icd.m_precision;
  memcpy_s(m_data,sizeof(long) * icdLength,p_icd.m_data,sizeof(long) * icdLength);
}

// Icd::Icd
//
// Description:   copy constructor with one or two longs (32bits)
// Parameters:    const long value      // value before the '.'
//                const long remainder  // value behind the '.'
// What it does:  Uses SetValue long to initialize the icd
//
icd::icd(const long value, const long remainder)
{
  SetValueLong(value,remainder);
}

// Icd::Icd
// Copy constructor with an int64 (64 bits) and an optional remainder (32 bit)
icd::icd(const int64 value, const int64 remainder)
{
  SetValueInt64(value, remainder);
}

// Icd::Icd
// Description:   copy constructor from a double. Beware for rounding errors
// Parameters:    const double value 
// What it does:  Uses SetValueDouble
//
icd::icd(const double value)
{
  SetValueDouble(value);
}

// Icd::Icd
// Description:   copy constructor from a string
// Parameters:    const String&     // String to be copied
//                bool  fromDB      // True if the value came from a database
// What it does:  Uses SetValueString
//
icd::icd(const CString& str)
{
  SetValueString(str);
}

// Icd::Icd
// Description:  copy constructor from a database NUMERIC
// Parameters    p_numeric   // Pointer to ODBC structure
// 
icd::icd(SQL_NUMERIC_STRUCT* p_numeric)
{
  SetValueNumeric(p_numeric);
}

// Icd::~Icd
// Description:   destructor
// What it does:  Does nothing (yet)
//
icd::~icd()
{
}

//////////////////////////////////////////////////////////////////////////
//
// END ICD CONSTRUCTORS / DESTRUCTORS
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// ICD CONSTANTS
//
//////////////////////////////////////////////////////////////////////////

// Icd::PI
// Description:   Ratio between the radius and the circumference of a circle
// What it does:  Natural constant that never changes
icd
icd::PI()
{
  icd pi;

  pi.m_length    = icdLength * icdDigits;
  pi.m_precision = icdLength * icdDigits / 2;
  pi.m_sign      = Positive;

  // PI in 40 decimals
  // +3.14159265_35897932_38462643_38327950_28841972
  pi.m_data[icdPointPosition    ] = 3;
  pi.m_data[icdPointPosition - 1] = 14159265L;
  pi.m_data[icdPointPosition - 2] = 35897932L;
  pi.m_data[icdPointPosition - 3] = 38462643L;
  pi.m_data[icdPointPosition - 4] = 38327950L;
  pi.m_data[icdPointPosition - 5] = 28841972L;

  return pi;
}

// Icd::LN2
// Description:  Natural logarithm of two
// What it does: Mathematical constant that never changes
icd
icd::LN2()
{
  icd ln2;

  ln2.m_length    = icdLength * icdDigits;
  ln2.m_precision = icdLength * icdDigits / 2;
  ln2.m_sign      = Positive;

  // LN2 in 40 decimals
  // +0.69314718_05599453_09417232_12145817_65680756
  ln2.m_data[icdPointPosition    ] = 0;
  ln2.m_data[icdPointPosition - 1] = 69314718L;
  ln2.m_data[icdPointPosition - 2] =  5599453L;
  ln2.m_data[icdPointPosition - 3] =  9417232L;
  ln2.m_data[icdPointPosition - 4] = 12145817L;
  ln2.m_data[icdPointPosition - 5] = 65680756L;

  return ln2;
}

// Icd::LN10
// Description:  Natural logarithm of ten
// What it does: Mathematical constant that never changes
icd
icd::LN10()
{
  icd ln10;

  ln10.m_length    = icdLength * icdDigits;
  ln10.m_precision = icdLength * icdDigits / 2;
  ln10.m_sign      = Positive;

  // LN10 in 40 decimals
  // +2.30258509_29940456_84017991_45468436_42076019
  ln10.m_data[icdPointPosition    ] = 2;
  ln10.m_data[icdPointPosition - 1] = 30258509L;
  ln10.m_data[icdPointPosition - 2] = 29940456L;
  ln10.m_data[icdPointPosition - 3] = 84017991L;
  ln10.m_data[icdPointPosition - 4] = 45468436L;
  ln10.m_data[icdPointPosition - 5] = 42076019L;

  return ln10;
}

//////////////////////////////////////////////////////////////////////////
//
// END ICD CONSTANTS
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// ICD OPERATORS
//
//////////////////////////////////////////////////////////////////////////

// Icd::operator=
// Description:   assignment from an icd
// Parameters:    const icd& 
// What it does:  If it is not us, copy the icd
//
const icd&
icd::operator=(const icd& p_icd)
{
  if (this != &p_icd)
  {
    m_sign      = p_icd.m_sign;
    m_length    = p_icd.m_length;
    m_precision = p_icd.m_precision;
    for (long i = 0; i < icdLength; i++)
    {
      m_data[i] = p_icd.m_data[i];
    }
  }
  return *this;
}

// Icd::operator=
//
// Description:   assignment from a string
// Parameters:    const String&
// What it does:  Uses SetValueString to get the string in the icd
//
const icd&
icd::operator=(const CString& str)
{
  SetValueString(str);
  return *this;
}

// Icd::operator=
// Description:   assignment from a long
// Parameters:    const long 
// What it does:  Uses SetValueLong to get the long in the icd
//
const icd&
icd::operator=(const long value)
{
  SetValueLong(value, 0);
  return *this;
}

// Icd::operator=
// Description:   assignment from a double
// Parameters:    const double 
// What it does:  Uses SetValueDouble to get the double in the icd
//
const icd&
icd::operator=(const double value)
{
  SetValueDouble(value);
  return *this;
}

// Icd::operator+=
// Description:   Adds the icd and returns the result
// What it does:  Uses the + operator and the = operator. (x+=y equivalent to x=x+y)
//
const icd&
icd::operator+=(const icd& p_icd)
{
  // x+=y is equal to: x=x+y
  *this = *this + p_icd;
  return *this;
}

// Icd::operator-=
// Description:   Subtracts the icd and returns the result
// What it does:  Uses the - operator and the = operator. (x-=y equivalent to x=x-y)
//
const icd&
icd::operator-=(const icd& p_icd)
{
  // x-=y is the same as: x=x-y
  *this = *this - p_icd;
  return *this;
}

// Icd::operator*=
// Description:   Multiplies the icd to this one and returns the result
// What it does:  Uses the Mul operation
//
const icd& 
icd::operator*=(const icd& p_icd)
{
  *this = Mul(p_icd);
  return *this;
}

// Icd::operator/=
// Description:   Divide this icd with the given icd and returns the result
// What it does:  Uses the Div operation
//
const icd&
icd::operator/=(const icd& p_icd)
{
  *this = Div(p_icd);
  return *this;
}

// Icd::operator- (Unary)
// Description:   prefix - operator (negation)
// What it does:  Copies the icd and then negates the result, if not zero
//
const icd
icd::operator-() const
{
  icd number(*this);

  // If null, do not change the sign, otherwise flip it
  if (!number.IsNull())
  {
    number.m_sign = (number.m_sign == Positive) ? Negative : Positive;
  }
  return number;
}

// Icd::operator++
// Description:   postfix ++ operator
// What it does:  Uses the prefix ++ and the copy constructor
//
const icd
icd::operator++(int)
{
  // postfix ++ operator: calculate the result first, do the addition afterward
  icd res(*this);
  ++*this;
  return res;
}

// Icd::operator++
// Description:   prefix ++ operator
// What it does:  Adds 1 (one) and returns the result
//
icd&
icd::operator++()
{
  //++x is equivalent to: x+=1
  icd number_1(1L, 0);
  *this += number_1;
  return *this;
}

// Icd::operator--
// Description:   postfix -- operator
// What it does   Uses the prefix -- and returns the result
//
const icd
icd::operator--(int)
{
  // postfix -- operator: calculate result first, subtract later
  icd res(*this);
  --*this;
  return res;
}

// Icd::operator--
// Description:   prefix -- operator
// What it does:  Subtract 1 (one) and return this
//
icd&
icd::operator--()
{
  // --x is same as x-=1
  icd number_1(1L);
  *this -= number_1;
  return *this;
}

// Icd::operator==
// Description:   comparison operator is-equal
// What it does:  true if sign and all m_data are equal
//
const bool
icd::operator==(const icd& p_icd) const
{
  // Only equal is signs are equal
  bool isSame = m_sign == p_icd.m_sign;

  if(isSame)
  {
    // Same if all m_data are equal
    for(long i = 0; isSame && i < icdLength; i++)
    {
      isSame = m_data[i] == p_icd.m_data[i];
    }
  }
  return isSame;
}

// Icd::operator!=
// Description:   Comparison operator not-equal
// What it does:  Uses the inversion of the equality operator
//
const bool
icd::operator!=(const icd& p_icd) const
{
  // x!=y is equivalent to !(x==y)
  return !(*this == p_icd);
}

// Icd::operator<
// Description:   Comparison operator smaller-than
// What it does:  If the left side is negative, and the right side positive, already true
//                Otherwise: test the m_data for the inequality
//
const bool
icd::operator<(const icd& p_icd) const
{
  // Signs are the same
  bool isSame = (m_sign == p_icd.m_sign);

  // Smaller if the left side is negative, and the right side positive
  bool isSmaller = (m_sign == Negative && p_icd.m_sign == Positive);

  // Smaller as long as number left is smaller than number right
  for (long i = icdLength - 1; i >= 0 && isSame; i--)
  {
    if (m_data[i] != p_icd.m_data[i])
    {
      isSame    = false;
      isSmaller = m_data[i] < p_icd.m_data[i];
      break;
    }
  }
  // If both signs are negative and not the same, invert the result
  // Example: -5<-3 is equal to 5>3
  //          -5<-5 is equal to 5<5
  if (m_sign == Negative && p_icd.m_sign == Negative && !isSame)
  {
    isSmaller = !isSmaller;
  }
  return isSmaller;
}

// Icd::operator>
// Description:    Comparison operator greater-than
// What it does:   Already greater if left side is positive and right side negative
//                 Otherwise test the m_data structure
//
const bool
icd::operator>(const icd& p_icd) const
{
  // Equality of the signs
  bool isSame = (m_sign == p_icd.m_sign);

  // Greater if left side is positive and right side not
  bool isGreater = (m_sign == Positive && p_icd.m_sign == Negative);

  // Test the m_data structure
  for (long i = icdLength - 1; i >= 0 && isSame; i--)
  {
    if (m_data[i] != p_icd.m_data[i])
    {
      isSame    = false;
      isGreater = m_data[i] > p_icd.m_data[i];
      break;
    }
  }

  // If both sides are negative and not the same, invert the result
  // Example: -5>-3 is same as 5<3
  //          -5>-5 is same as 5>5
  if (m_sign == Negative && p_icd.m_sign == Negative && !isSame)
  {
    isGreater = !isGreater;
  }
  return isGreater;
}

// Icd::operator<=
// Description:   Comparison operator smaller-than-or-equal-to
// What it does:  Uses the inversion of the greater-than operator
//
const bool
icd::operator<=(const icd& p_icd) const
{
  // x<=y is equivalent to !(x>y)
  return !(*this>p_icd);
}

// Icd::operator>=
// Description:   Comparison operator greater-than-or-equal-to
// What it does:  Uses the inversion of the smaller-than operator
//
const bool
icd::operator>=(const icd& p_icd) const
{
  // x>=y is equivalent to !(x<y)
  return !(*this < p_icd);
}

// operator<<
// Description:   standard output operator
// What it does:  Uses the string 'AsString' method to serialize an icd
//
ostream& operator<<(ostream& os, const icd& p_icd)
{
  os << p_icd.AsString().operator const char*();
  return os;
}

//////////////////////////////////////////////////////////////////////////
//
// END ICD OPERATORS
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// MATHEMATICAL ICD FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

// Icd::Floor
// Description:   First natural number not greater than the input
// What it does:  Remove the decimal part and subtract 1 for negative numbers
//
icd
icd::Floor() const
{  
  icd res;
  res.m_sign = m_sign;
  for (int n = icdPointPosition; n < icdLength; n++)
  {
    res.m_data[n] = m_data[n];
  }
  if (m_sign == Negative)
  {  
    for (int n = 0; n < icdPointPosition; n++)
    {
      if(m_data[n])
      {
        // ICD not zero behind the decimal point, and it's a negative number
        // so the 'floored' version is the truncation MINUS ONE!
        --res;
        break;
      }
    }
  }
  return res;
}

// Icd::Ceiling
// Description:   First natural number greater than the input
// What it does:  Remove decimal part and add 1 for negative numbers
//
icd
icd::Ceil() const
{
  icd res;
  res.m_sign = m_sign;
  for (int n = icdPointPosition; n < icdLength; n++)
  {
    res.m_data[n] = m_data[n];
  }
  if (m_sign == Positive)
  {  
    for (int n = 0; n < icdPointPosition; n++)
    {
      if (m_data[n])
      {
        // ICD was non-zero behind the decimal point
        // so the 'ceiled' version is truncated PLUS ONE!
        ++res;
        break;
      }
    }
  }
  return res;
}

// Icd::sqrt
// Description:   Square root of the number
// What it does:  Does an estimation through a double and then
//                Uses the Newton estimation to calculate the root
icd
icd::SquareRoot() const
{
  icd number(0L,0L);
  icd half("0.5");
  icd two(2L);
  icd three(3L);

  // Optimization: sqrt(0) = 0
  if(IsNull())
  {
    return number;
  }
  // Getting the breaking criterion
  icd epsilon = Epsilon(10);

  number = *this; // Number to get the square-root from
  if(number.GetSign() == -1)
  {
    throw new CString("Cannot calculate a square-root from a negative number");
  }
  // Reduction by dividing through the square of a whole number
  // For speed we use the powers of two
  icd reduction(1L);
  icd hundred(100L);
  while(number / (reduction * reduction) > hundred)
  {
    reduction *= two;
  }
  // Reduction by dividing through the square of the reduction
  // 'reduction' is in fact sqrt(reduction)
  number /= (reduction * reduction);

  // First estimate
  double estimate1 = number.AsDouble();
  double estimate2 = 1 / sqrt(estimate1);
  icd result(estimate2);
  icd between;

  // Newton's iteration
  // Un = U(3-VU^2)/2
  while(true)
  {
    between  = number * result * result;  // VU^2
    between  = three - between;           // 3-VU^2
    between *= half;                      // (3-VU^2)/2

    if(between.ValueBehindPoint() < epsilon)
    {
      break;
    }
    result *= between;
  }
  // Final result by calculating number * 1/root
  result *= number;
  // Adding the reduction by multiplying
  result *= reduction;

  return result;
}

// Icd::Power
// Description:   icd to the power of another icd
// What it does:  x^y = exp(y * ln(x))
icd
icd::Power(const icd& p_icd) const
{
  icd result;

  result = this->Log() * p_icd;
  result = result.Exp();

  return result;
}

// Icd::Abs
// Description:   Returning the absolute value
// What it does:  Setting the sign to positive
icd
icd::AbsoluteValue() const
{
  icd icd2 = *this;
  icd2.m_sign = Sign::Positive; 
  return icd2;
}

// Icd::Reciproke
// Description:   Inverse of the number = 1 / number
// What it does:  Using the standard Div
//
icd
icd::Reciproke() const
{
  icd number(1L);
  number = number.Div(*this);
  return number;
}

// Icd::Log
// Description:   Natural Logarithm
// What it does:  Use a Taylor series until their is no more change in the result
//                Equivalent with the same standard C function call
//                ln(x) == 2( z + z^3/3 + z^5/5 ...
//                z = (x-1)/(x+1)
//
icd 
icd::Log() const
{
  long k;
  long expo = 0;
  icd res, number, z2;
  icd number10(10L);
  icd fast("1.2");
  icd one(1L);
  icd epsilon = Epsilon(5);

  if(*this <= icd(0L)) 
  { 
    throw new CString("Cannot get a natural logarithm from a number <= 0");
  }
  // Bringing the number under 10 and save the exponent
  number = *this;
  while(number > number10)
  {
    number /= number10;
    ++expo;
  }
  // In order to get a fast Taylor series result we need to get the fraction closer to one
  // The fraction part is [1.xxx-9.999] (base 10) OR [1.xxx-255.xxx] (base 256) at this point
  // Repeat a series of square root until 'number' < 1.2
  for(k = 0; number > fast; k++)
  {
    number = sqrt(number);
  }
  // Calculate the fraction part now at [1.xxx-1.1999]
  number = (number - one) / (number + one);
  z2    = number * number;
  res   = number;
  // Iterate using taylor series ln(x) == 2( z + z^3/3 + z^5/5 ... )
  icd tussen;
  for(long stap = 3; ;stap += 2)
  {
    number *= z2;
    tussen = number / icd(stap);
    // Breaking criterion
    if(tussen < epsilon)
    {
      break;
    }
    res += tussen;
  }
  // Adding the powers of 2 again (came from " < 1.2")
  res *= icd(pow(2.0,(double)(k + 1)));

  // Adding the exponent again
  if(expo != 0)
  {
    // Ln(x^y) = Ln(x) + Ln(10^y) = Ln(x) + y * ln(10)
    res += icd(expo) * icd::LN10();
  }
  return res;
}

// Icd::Exp
// Description:   Exponent E till the power of this icd
// What it does:  Use a Taylor series until their is no more change in the result
//                exp(x) == 1 + x + x^2/2!+x^3/3!+....
//                Equivalent with the same standard C function call
//
icd 
icd::Exp() const
{
  long step, k = 0;
  long expo;
  icd  between, result, number;
  icd  half("0.5");
  icd  ten(10L);
  icd  epsilon = Epsilon(5);

  number = *this;

  if( number.GetSign() < 0 )
  {
    number = -number;;
  }
  for( k = 0; number > half; )
  {
    expo = number.Exponent();
    if( expo > 0 )
    {
      step   = 3 * min( 10, expo );  // 2^3
      result = icd((long) (1 << step) );
      result = result.Reciproke();
      k += step;
    }
    else
    {
      result = half;
      k++;
    }

    number *= result;
  }

  // Do first two iterations
  result  = icd(1L) + number;
  between  = number * number * half;
  result += between;
  // Now iterate 
  for(step = 3; ;step++)
  {
    between *= number / icd(step);
    // Breaking criterion
    if(between < epsilon)
    {
      break;
    }
    result += between;
  }
  // Adding powers of itself again
  for( ; k > 0; k-- )
  {
    result *= result;
  }
  // Correcting the sign
  if(this->GetSign() < 0 )
  {
    result = icd((long)1) / result;
  }
  return result;
}

// Icd::Log10
// Description:   Logarithm on base 10
// What it does:  log10 = ln(x) / ln(10);
icd     
icd::Log10() const
{
  icd res(0L);

  if(GetSign() <= 0) 
  { 
    throw new CString("Cannot calculate a logarithm from a number <= 0");
  }
  res = *this;
  res = res.Log() / LN10();

  return res;
}

//////////////////////////////////////////////////////////////////////////
//
// END ICD MATHEMATICAL FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// ICD TRIGONOMETRIC FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

// Icd::Sine
// Description:  Sine of an angle
// What it does: Use the Taylor series: Sin(x) = x - x^3/3! + x^5/5! ...
//               1) Reduce x first to in between 0..2*PI 
//               2) Reduce further until x between 0..PI by means of sin(x+PI) = -Sin(x)
//               3) Do the Taylor expansion series
//   This reductions are necessary to speed up the Taylor expansion and 
//   keep the rounding errors under control
//
icd
icd::Sine() const
{
  int sign;
  icd number;
  icd pi,pi2;
  icd between;
  icd epsilon = Epsilon(3);

  number = *this;

  sign = number.GetSign();
  if( sign < 0 )
  {
    number = -number;
  }
  // Reduce the argument until it is between 0..2PI
  pi2 = PI() * icd(2L);
  if(number > pi2)
  {
    between = number / pi2; 
    between = between.ValueBeforePoint();
    number -= between * pi2;
  }
  if(number < icd(0L))
  {
    number += pi2;
  }
  // Reduce further until it is between 0..PI 
  pi = PI();
  if(number > pi)
  { 
    number -= pi; 
    sign *= -1; 
  }

  // Now we iterate with the Taylor expansion
  // Sin(x) = x - x^3/3! + x^5/5! ...
  icd kwadraat  = number * number;
  icd resultaat = number;
  between = number;

  for(long stap = 3; ;stap += 2)
  {
    between   *= kwadraat;
    between   /= icd(stap) * icd (stap - 1);
    between    = -between; // switch signs each step!
    resultaat += between;

    // DEBUGGING
    // printf("%02d = %40s = %40s\n",stap,tussen.AlsString(),resultaat.AlsString());

    // Check the breaking criterion
    if(between.AbsoluteValue() < epsilon)
    {
      break;
    }
  }
  // Add the correct sign
  if( sign < 0 )
  {
    resultaat = -resultaat;
  }
  return resultaat;
}

// Icd::Cosine
// Description:  Cosine of an angle
// What it does: Use the Taylor series. Cos(x) = 1 - x^2/2! + x^4/4! - x^6/6! ...
//               1) However first reduce x to between 0..2*PI
//               2) Then reduced it further to between 0..PI using cos(x)=Cos(2PI-x) for x >= PI
//               3) Now use the trisection identity cos(3x)=-3*cos(x)+4*cos(x)^3
//                  until argument is less than 0.5
//               4) Finally use Taylor 
//
icd
icd::Cosine() const
{
  long trisection, step;
  icd between, result, number, number2;
  icd c05("0.5"), c1(1L), c2(2L), c3(3L), c4(4L);
  icd epsilon = Epsilon(2);

  number = *this;

  // Reduce argument to between 0..2PI
  result  = PI();
  result *= c2;
  if(number.AbsoluteValue() > result )
  {
    between = number / result; 
    between = between.ValueBeforePoint();
    number -= between * result;
  }
  if(number.GetSign() < 0)
  {
    number += result;
  }
  // Reduced it further to between 0..PI. u==2PI
  between = PI();
  if( number > between )
  {
    number = result - number;
  }
  // Now use the trisection identity cos(3x)=-3*cos(x)+4*cos(x)^3
  // until argument is less than 0.5
  for( trisection = 0, between = c1; number / between > c05; ++trisection)
  {
    between *= c3;
  }
  number /= between;

  // First step of the iteration
  number2 = number * number;
  between = c1;
  result = between;
  // Iterate with Taylor expansion
  for(step=2; ;step += 2)
  {
    number   = number2; 
    number  /= icd(step);
    number  /= icd(step-1);
    between *= number;
    between  = -between;  // r.change_sign();
    // Breaking criterion
    if(between.AbsoluteValue() < epsilon)
    {
      break;
    }
    result += between;
  }

  // Adding the effect of the tri-section again
  for( ;trisection > 0; --trisection)
  {
    result *= ( c4 * result * result - c3 );
  }
  return result;
}

// Icd::Tangent
// Description:   Tangent of an angle
// What it does:  Use the identity tan(x)=Sin(x)/Sqrt(1-Sin(x)^2)
//                However first reduce x to between 0..2*PI
//
icd
icd::Tangent() const
{
  icd result, between, number;
  icd two(2L),three(3L);;

  number = *this;

  // Reduce argument to between 0..2PI
  icd pi    = PI();
  icd twopi = two * pi;
  if(number.AbsoluteValue() > twopi )
  {
    between  = number / twopi; 
    between  = between.ValueBeforePoint();
    number  -= between * twopi;
  }

  if(number.GetSign() < 0)
  {
    number += twopi;
  }
  icd halfpi    = pi / two;
  icd anderhalf = three * halfpi;
  if( number == halfpi || number == anderhalf)
  { 
    throw new CString("Cannot calculate a tangent from an angle of 1/2 pi or 3/2 pi");
  }
  // Sin(x)/Sqrt(1-Sin(x)^2)
  result     = number.Sine(); 
  icd square = result * result;
  icd part   = icd(1L) - square;
  icd root   = sqrt(part);
  result    /= root;

  // Correct for the sign
  if(number > halfpi && number < anderhalf)
  {
    result = -result;
  }
  return result;
}

// Icd::Asin
// Description:  Arcsine (angle) of a Sine ratio
// What it does: Use Newton by solving the equation Sin(y)=x. Then y is Arcsine(x)
//               Iterate by Newton y'=y-(sin(y)-x)/cos(y). 
//               With initial guess using standard double precision arithmetic.
//
icd     
icd::ArcSine() const
{
  long step, reduction, sign;
  double d;
  icd between, number, result, factor;
  icd c1(1L), c05("0.5"), c2(2L);
  icd epsilon = Epsilon(5);

  number = *this;
  if(number > c1 || number < -c1)
  {
    throw new CString("Cannot calculate an arcsine from a number > 1 or < -1");
  }
  // Save the sign
  sign = number.GetSign();
  if(sign < 0)
  {
    number = -number;
  }
  // Reduce the argument to below 0.5 to make the newton run faster
  for(reduction = 0; number > c05; ++reduction)
  {
    number /= sqrt(c2) * sqrt( c1 + sqrt( c1 - number * number ));
  }
  // Fast estimate from a double
  d = asin(number.AsDouble());
  result = icd( d );
  factor = icd( 1.0 / cos(d) ); // Constant factor 

  // Newton Iteration
  for( step=0;; step++)
  {
    between = ( result.Sine() - number ) * factor;
    // if( y - r == y )
    if(between.AbsoluteValue() < epsilon)
    {
      break;
    }
    result -= between;
  }

  // Set reduction back into the result
  result *= icd((long) (1 << reduction) );

  // Correct the sign
  if( sign < 0 )
  {
    result = -result;
  }
  return result;

}

// Icd::Acos
// Description:   ArcCosine (angle) of a cosine ratio
// What it does:  Use ArcCosine(x) = PI/2 - ArcSine(x)
icd     
icd::ArcCosine() const
{
  icd y;

  y  = PI();
  y /= icd(2L);
  y -= ArcSine();

  return y;
}

// Icd::Atan
// Description:   Arctangent (angle) of a tangent ratio
// What it does:  Use the Taylor series. ArcTan(x) = x - x^3/3 + x^5/5 ...
//                However first reduce x to abs(x)< 0.5 to improve taylor series
//                using the identity. ArcTan(x)=2*ArcTan(x/(1+sqrt(1+x^2)))
//
icd
icd::ArcTangent() const
{
  icd  result, square;
  icd  between1,between2;
  icd  half("0.5");
  icd  one(1L);
  icd  epsilon = Epsilon(5);
  long k = 2;

  result   = *this;
  // Transform the solution to ArcTan(x)=2*ArcTan(x/(1+sqrt(1+x^2)))
  result = result / (one + sqrt( one + (result * result)));
  if( result.AbsoluteValue() > half) // if still to big then do it again
  {
    k = 4;
    result = result / (one + sqrt(one + (result * result)));
  }
  square   = result * result;
  between1 = result;
  // Now iterate using Taylor expansion
  for(long stap = 3;; stap += 2)
  {
    between1 *= square;
    between1  = -between1;
    between2  = between1 / icd(stap);
    // Use the breaking criterion
    if(between2.AbsoluteValue() < epsilon)
    {
      break;
    }
    result += between2;
  }
  result *= icd(k);
  return result;
}

// Icd::Atan2
// Description:  Angle between two points
// What it does: return the angle (in radians) from the X axis to a point (y,x).
//               use atan() to calculate atan2()
//
icd
icd::ArcTangent2Points(icd p_x) const
{
  icd result;
  icd number = *this;
  icd zero(0L), c05("0.5");

  if( p_x == zero && number == zero)
  {
    return zero;
  }
  if( p_x == zero )
  {
    result = PI();
    if( number < zero )
    {
      result *= -c05;
    }
    else
    {
      result *= c05;
    }
  }
  else
  {
    if( number == zero )
    {
      if( p_x < zero )
      {
        result = PI();
      }
      else
      {
        result = zero;
      }
    }
    else
    {
      result = icd( number / p_x ).ArcTangent();
      if( p_x < zero  && number < zero )
      {
        result -= PI();
      }
      if( p_x < zero && number >= zero )
      {
        result += PI();
      }
    }
  }
  return result;
}

//////////////////////////////////////////////////////////////////////////
//
// END ICD TRIGONOMETRIC FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 
// GETTING THE ICD AS SOMETHING ELSE
//
//////////////////////////////////////////////////////////////////////////

// Icd::ValueBeforePoint
// Description:   Getting the part before the decimal point 
// What it does:  Part after the decimal point is set to zero
icd
icd::ValueBeforePoint() const
{
  icd before = *this;
  for(int i = 0; i < icdPointPosition; ++i)
  {
    before.m_data[i] = 0;
  }
  return before;
}

// Icd:: ValueBehindPoint
// Description:  Value behind the decimal point
// What it does: Part before the decimal point is set to zero
icd
icd::ValueBehindPoint() const
{
  icd behind = *this;
  for(int i = icdPointPosition; i < icdLength; ++i)
  {
    behind.m_data[i] = 0;
  }
  return behind;
}

// Icd::AsDouble
// Description:    Value of the icd as a standard double
//                 Beware for rounding errors!
// What it does:   Calculate the parts before and after the decimal point
//                 seperetly by means of the pow function
//
double
icd::AsDouble() const
{
  double res = 0.0;

  // Part before the decimal point
  for (int i = icdPointPosition; i < icdLength; i++)
  {
    res += pow(double(icdBase), i - icdPointPosition) * m_data[i];
  }
  // Part after the decimal point
  for (int i = 0; i < icdPointPosition; i++)
  {
    res += (double)m_data[i] / pow(double(icdBase), icdPointPosition - i);
  }
  // Correct for the sign
  return (m_sign == Positive ? 1 : -1) * res; 
}

// Icd::AsLong
// Description:   Returns the value of the ICD as a long
//                Can throw if the icd does not fit into a long
// What it does:  Converts the first three parts before the decimal point
//
long
icd::AsLong() const
{
  if(!FitsInLong()) 
  {
    throw new CString("Number: Overflow in icd");
  }
  // Calculate long
  long result = m_data[icdPointPosition] + 
                m_data[icdPointPosition  + 1] * icdBase;
  if(icdBase <= 10000)
  {
    result += m_data[icdPointPosition + 2] * icdBase * icdBase;
  }
  if (m_sign == Negative)
  {
    result = -result;
  }
  return result;
}

// Icd::AsInt64
// Description:   Gets the value of an icd as an int64
// What it does:  Converts the part before the decimal point
//
int64
icd::AsInt64() const
{
  int64 result = 0;

  if(!FitsInInt64())
  {
    throw new CString("Number: Overflow in icd");
  }
  for(int i = icdLength -1; i >= icdPointPosition; --i)
  {
    result *= icdBase;
    result += m_data[i];
  }
  if(m_sign == Negative)
  {
    return -result;
  }
  return result;
}

// Icd::AsString
// Description:    Gets the value of the icd as a string
// What it does:   Gets the sign first, and then runs through the m_data parts
//
CString
icd::AsString() const
{
  // Sign at the beginning. Always add a sign
  CString str;
  if (m_sign == icd::Negative)
  {
    str += "-";
  }
  else
  {
    str += "+";
  }
  // If the first non-null number has not be found: do nothing
  // Otherwise prefix the number with enough zeros
  // Example: 0:0:2:342:2:0 becomes +200000342,00000002
  bool stillZero = true;

  for (int i = icdLength - 1; i >= icdPointPosition; i--)
  {
    if (!stillZero)
    {
      // Place extra zero's
      for (long j = 0; j < (icdDigits - 1) - long_log10(m_data[i]); j++)
      {
        str += "0";
      }
    }
    stillZero = (stillZero && (m_data[i] == 0));
    if (!stillZero || i == icdPointPosition)
    {
      str += LongToString(m_data[i]);
    }
  }

  CString strAfterPoint;
  for (int k = icdPointPosition - 1; k >= 0; k--)
  {
    for (long j = 0; j < (icdDigits - 1) - long_log10(m_data[k]); j++)
    {
      strAfterPoint += "0";
    }
    strAfterPoint += LongToString(m_data[k]);
  }
  if (!str)
  {
    str += "0";
  }
  // Rounding
  if (m_precision < 16 && strAfterPoint[m_precision] >= '5') 
  {
    int pos = m_precision-1;

    while (pos >= 0 && strAfterPoint[pos] == '9') 
    {
      strAfterPoint.SetAt(pos, '0');
      pos--;
    }
    if (pos >= 0)
    {
      strAfterPoint.SetAt(pos, (char)(strAfterPoint[pos] + 1)); // rounding
    }
    else
    {
      pos = str.GetLength()-1; // Rounding past the decimal point
      while (pos >= 0 && str[pos] == '9')
      {
        str.SetAt(pos, '0');
        pos--;
      }
      if (pos >= 0)
      {
        str.SetAt(pos, (char)(str[pos] + 1)); // Rounding
      }
      else
      {
        str = "1" + str;
      }
    }
  }
  if (m_precision > 0)
  {
    str += "."; // Use locale?
    str += strAfterPoint.Mid(0, m_precision);
  }
  str.TrimRight(".0");
  return str;
}

// Get as a SQL string 
CString 
icd::AsSQLString(bool p_strip /*=false*/) const
{
  CString str = AsString();
  if(p_strip)
  {
    // Strip mechanism is used to reduce the extra trailing
    // zero's in the representation to save spacae if large
    // amounts of numbers need to be used
    char ch = '.';
    int ind = str.Find(ch);
    if(ind < 0)
    {
      ch = ','; // sessie->GeefLocale().DecimaalChar();
      ind = str.Find(ch);
    }
    if(ind >= 0)
    {
      int pos = str.GetLength() - 1;
      while(pos && str.GetAt(pos) == '0')
      {
        --pos;
      }
      if(pos && str.GetAt(pos) == ch)
      {
        --pos;
      }
      str = str.Left(pos + 1);
      if(str.IsEmpty())
      {
        str = "0";
      }
    }
  }
  char decimaal = ','; // sessie->GeefLocale().DecimaalChar();
  if(decimaal != '.')
  {
    str.Replace(decimaal,'.');
  }
  // SQL-Server cannot use decimals (before and after) the decimal point
  // longer than the maximum allowed precision.
  // The maximum length is therefore the precision + (1 for the decimal point) + optional minus sign
  int maxprec = icdPrecision + 1 + ((m_sign == Sign::Negative) ? 1 : 0);
  if(str.GetLength() > maxprec)
  {
    str = str.Left(maxprec);
  }
  return str;
}

// Icd::AsDisplayString
// Description:    Gets the value of a icd as a formatted string
// What it does:   Gets the string first and then adds the thousands separators
//
CString 
icd::AsDisplayString() const
{
  CString display = AsString();
  CString decpart;
  CString intpart;
  CString sgnpart;
  static CString sgn("-+");
  int decpos;
  if((decpos = display.Find(".")) == -1) 
  {
    intpart = display;	
  }
  else
  {
    intpart = display.Left(decpos);
    decpart = display.Mid(decpos + 1);
  }
  while(!intpart.IsEmpty() && sgn.Find(intpart.Left(1)) != -1)
  {
    sgnpart += intpart.Left(1);
    intpart  = intpart.Mid(1);
  }
  display = "";
  while(intpart.GetLength() > 3)
  {
    display = "." + intpart.Right(3) + display;
    intpart = intpart.Left(intpart.GetLength() - 3);
  }
  intpart += display;

  display = sgnpart + intpart;
  if(decpart.GetLength() != 0)
  {
    display += "." + decpart; 
  }
  return display;
}

// Get as an ODBC SQL NUMERIC
void
icd::AsNumeric(SQL_NUMERIC_STRUCT* p_numeric) const
{
  // Init the value array
  memset(p_numeric->val,0,SQL_MAX_NUMERIC_LEN);

  // Register the resulting sign, precision and scale
  p_numeric->sign      = (m_sign == Sign::Positive) ? 1 : 0;
  p_numeric->precision = (SQLCHAR) m_length;
  p_numeric->scale     = (SQLSCHAR) m_precision;

  // Special case for 0.0 
  if(IsNull())
  {
    return;
  }

  // Converting the value array
  icd one(1L);
  icd radix(256L);
  icd accu(*this);
  int index = 0;

  // Here is the big trick: positive calculations
  accu.m_sign = Sign::Positive;

  // Adjust decimals after the comma
  for(int mul = 0;mul < m_precision;++mul)
  {
    accu.Mult10();
  }

  while(true)
  {
    // Getting the next val array value, relying on the modulo
    // Mag hier met een long, omdat (icdBasis % 256 == 0)
    long val = accu.m_data[icdPointPosition] % 256;
    p_numeric->val[index++] = (SQLCHAR) val;

    // Adjust the intermediate accu
    accu -= val;
    accu = accu.Div(radix);

    // Breaking criterion: nothing left
    if(accu < one)
    {
      break;
    }

    // Breaking criterion on overflow
    // Check as last, number could fit exactly!
    if(index >= SQL_MAX_NUMERIC_LEN)
    {
      break;
    }
  }
  if(accu >= one)
  {
    throw CString("ICD to SQL_NUMERIC overflow!");
  }
}

//////////////////////////////////////////////////////////////////////////
//
// END GETTING AN ICD AS SOMETHING DIFFERENT
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// ICD CHANGE PRECISION
//
//////////////////////////////////////////////////////////////////////////

// Icd::SetLengthAndPrecision
// Description:   Setting a new length and precision scale
// What it does:  Checking of all the arguments and rounding
//               
void
icd::SetLengthAndPrecision(int length, int precision)
{
  if(length == m_length && precision == m_precision)
  {
    return;
  }

  if (length < 1 || length > icdLength * icdDigits)
  {
    throw CString("Decimal number (icd): The length must be between 1 and ") + LongToString(icdLength * icdDigits);
  }
  if (precision < 0 || precision > icdPointPosition * icdDigits)
  {
    throw CString("Decimal number (icd): The precision must be between 0 and ") + LongToString(icdPointPosition * icdDigits);
  }
  if (length < precision)
  {
    throw new CString("Decimal number (icd): The precision must be smaller than or equal to the length");
  }
  if ((length - precision) > (icdPointPosition * icdDigits))
  {
    throw CString("Decimal number (icd): Number of digits before the decimal point cannot be greater than ") + LongToString(icdPointPosition * icdDigits);
  }
  m_precision = precision;

  int highestIndex = (icdDigits * icdPointPosition + (length - m_precision) - 1) / icdDigits;

  for (long i = icdLength - 1; i > highestIndex ; i--)
  {
    if (m_data[i] != 0)
    {
      throw new CString("Decimal number too big (ICD Overflow)");
    }
  }

  unsigned long maxIndexValue = (unsigned long)long_pow(10, (length - m_precision) % icdDigits);
  if (maxIndexValue == 1)
  {
    maxIndexValue = icdBase;
  }
  if (m_data[highestIndex] >= maxIndexValue)
  {
    throw new CString("Decimal number too big (ICD Overflow)");
  }

  m_length = length;

  // Test if the contents does not have a maximal value outside range after rounding
  if ((m_precision != (icdPointPosition * icdDigits)) && IsCornerCase()) 
  {
    throw new CString("Decimal number too big(ICD Overflow)");
  }

  // Clearing of data behind the decimal mpint; does not belong to our precision and must
  // be for all intents and purposes 0.
  int nonSignificantDigits = (icdPointPosition * icdDigits - m_precision);
  for (int n = 0; n < nonSignificantDigits / icdDigits; n++)
  {
    m_data[n] = 0;
  }
  // 1 of the elements is partly significant and partly not
  // we clear the non-significant part
  static int significations[] = {1,10,100,1000,10000,100000,1000000,10000000};
  // Number of digits on the right side that is non-significant
  int significans = significations[nonSignificantDigits % icdDigits];

  ///////////////////////////////////////////////////////////////////////////////////
  ////  Debug

  //m_data[ nietSignificantieDigits / 4] += 100;
  //long rndDecimaal = RoundDecimal(m_data[nietSignificantieDigits / 4], precisie);
  //m_data[ nietSignificantieDigits / 4] = rndDecimaal;

  ////  End Debug
  ///////////////////////////////////////////////////////////////////////////////////

  m_data[ nonSignificantDigits / icdDigits] /= significans;
  m_data[ nonSignificantDigits / icdDigits] *= significans;

  // RoundDecimals(-0.01) is an error
  // -0.0 check for the case we round a negative number > -1
  int ind = 0;
  while((m_data[ind] == 0) && (ind < icdLength)) ++ind;
  if((m_sign == Negative) && (ind == icdLength))
  {
    m_sign = Positive;
  }
}

// Icd::RoundDecimals
// Description:   Rounding on a whole number of decimals
// What it does:
//
long 
icd::RoundDecimals(long decimal, int precsion)
{
  CString strDecimal = "";
  CString strNum     = "";

  strDecimal = LongToString(decimal);

  if (!strDecimal.IsEmpty())
  {
    int intLength = strDecimal.GetLength() - 1;
    for (int idx = 0; idx <= intLength; idx++)
    {
      if (idx == precsion-1)
      {
        if (idx+1 <= intLength)
        {
          CString strDigit  = "";
          CString strTarget = "";

          strDigit += strDecimal.GetAt(idx+1);
          int digit = StringToLong(strDigit);

          strTarget += strDecimal.GetAt(idx);
          int Target = StringToLong(strTarget);

          if (digit >= 5)
          { 
            Target++;
            strTarget = "";
            strNum += LongToString(Target);
            return StringToLong(strNum);
          }
          else
          {
            strNum += strDecimal.GetAt(idx);
          }
        }
        else
        {
          return  decimal;
        }
      }
      else
      {
        strNum += strDecimal.GetAt(idx);
      }
    }
  }
  return StringToLong(strNum);
}

// Correct mathematical rounding
void
icd::Rounding(int p_length /*= 80*/,int p_precision /*= 40*/)
{
  icd factor(pow(0.1,p_precision));
  factor = (GetSign() == 1) ? (factor * 0.5) : (factor * -0.5);
  *this = Add(factor);
  SetLengthAndPrecision(p_length,p_precision);
}
//////////////////////////////////////////////////////////////////////////
//
// END ICD PRECISION FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// ICD GETTERS
//
//////////////////////////////////////////////////////////////////////////

// Icd::IsNull
// Description:    Returns true if icd is zero, otherwise false
// What it does:   Walks m_data to see if it is all zero
//
bool
icd::IsNull() const
{
  for(int ind = 0;ind < icdLength; ++ind)
  {
    if(m_data[ind])
    {
      return false;
    }
  }
  return true;
}

// Returns 0  -> Contents are 0.0
// Returns 1  -> Contents are > 0.0
// Returns -1 -> Contents are < 0.0
int 
icd::GetSign() const
{
  if (IsNull())
  {
    return 0;
  }
  return m_sign == Positive ? 1 : -1;
}

// Icd::FitsInLong
// Description:   Returns true if the icd fits into a basic 'long'
// What it does:  Checks the first two data members
//
bool 
icd::FitsInLong() const
{
  int64 result = m_data[icdPointPosition    ] + 
                 m_data[icdPointPosition + 1] * icdBase;

  // Implementation is now simple through bigger icdBase (10E+8)
  if(m_sign == Positive)
  {
    if(result > LONG_MAX)
    {
      return false;
    }
  }
  else
  {
    if(-result < LONG_MIN)
    {
      return false;
    }
  }
  // Check if it was still bigger
  for(int i = icdPointPosition + 2; i < icdLength; ++i)
  {
    if(m_data[i])
    {
      return false;
    }
  }
  return true;
}

// Icd::FitsInInt64
// Description:   Returns true if the icd fits into a __int64
// What it does:  Checks the first m_data members
bool
icd::FitsInInt64() const
{
  int64 result = m_data[icdPointPosition    ] +
                 m_data[icdPointPosition + 1] * icdBase;
  if(m_sign == Positive)
  {
    int64 testspace = LLONG_MAX - ((int64)icdBase * (int64)icdBase);
    if(m_data[icdPointPosition + 2] > testspace)
    {
      return false;
    }
  }
  else
  {
    int64 testspace = -(LLONG_MIN + ((int64)icdBase * (int64)icdBase));
    if(m_data[icdPointPosition + 2] > testspace)
    {
      return false;
    }
  }
  // Check is still bigger?
  for(int i = icdPointPosition + 3; i < icdLength; ++i)
  {
    if(m_data[i])
    {
      return false;
    }
  }
  return true;
}

// Icd::HasDecimals
// Description:   Returns true if the part behind the decimal point is filled
// What it does:  Checks m_data
bool 
icd::HasDecimals() const
{
  for(int x = 0; x < icdPointPosition; ++x)
  {
    if(m_data[x])
    {
      return true;
    }
  }
  return false;
}

// Icd::Exponent
// Description:   Returns the 10 base exponent of the icd number
// What it does:  Divides/multiplies until the icd is in the 0-10 range
//
int
icd::Exponent() const
{
  int exponent = icdPointPosition * icdDigits - 1;
  for(int ind = icdLength - 1; ind >= 0; --ind)
  {
    long denominator = icdBase / 10;

    for(int num = icdDigits; num > 0; --num)
    {
      long test = m_data[ind] / denominator;
      if(test)
      { 
        return exponent;
      }
      --exponent;
      denominator /= 10;
    }
  }
  return 0;
}

// Icd::Mantissa
// Description: Returns the 10-base mantissa of the icd number
icd
icd::Mantissa() const
{
  long expo = Exponent();

  if(expo == 0)
  {
    return *this;
  }
  long shift  = expo / icdDigits;
  long remain = expo % icdDigits;
  icd  number = *this;
  int  sign   = number.GetSign();

  number = number.BringTogether(number,-shift);
  if(expo > 0)
  {
    while(remain--)
    {
      number.Div10();
    }
  }
  else
  {
    while(remain++)
    {
      number.Mult10();
    }
  }
  // Adding the sign again
  if(sign < 0)
  {
    number = -number;
  }
  return number;
}

//////////////////////////////////////////////////////////////////////////
//
// END ICD GETTERS
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// ICD BASIC OPERATIONS
//
//////////////////////////////////////////////////////////////////////////

// Icd::Add
// Description:   + operator, Adding of two icd's
// Parameters:     const icd&
// What it does:   Getting the sign of the end result first
//                 Then see if we must use positive addition or subtraction
//
icd
icd::Add(const icd& p_icd) const
{
  // See if we must add or subtract in positive form
  // (+x) + (+y) -> Adding,    result positive, do not turn around
  // (+x) + (-y) -> Subtract,  result positive, do not turn around
  // (-x) + (+y) -> Subtract,  result positive, turn around
  // (-x) + (-y) -> Adding,    result negative, do not turn around
  Sign signResult;
  OperatorKind kindOperator;
  icd arg1(*this);
  icd arg2(p_icd);
  PositionArguments(arg1, arg2, signResult, kindOperator);

  if (kindOperator == Adding)
  {
    arg1 = AddPositive(arg1, arg2);
  }
  else
  {
    arg1 = SubtractPositive(arg1, arg2);
    // If sign became negative, than turn sign around
    if (arg1.m_sign == Negative)
    {
      if (signResult == Positive)
      {
        signResult = Negative;
      }
      else
      {
        signResult = Positive;
      }
    }
  }
  arg1.m_sign = signResult;

  return arg1;
}

// Icd::Sub
// Description:   - operator, Subtracting two icds
// What it does:  Uses the + operator and the prefix - (x-y is equivalent to x+(-y))
//
icd
icd::Sub(const icd& p_icd) const
{
  // x-y is equivalent to: x+(-y)
  return *this + (-p_icd);
}

// Icd::Mul
// Description:   * operator, Multiplies two icd's
//
icd
icd::Mul(const icd& p_icd) const
{
  // Multiply without taking signs into account
  icd res = MultiplyPositive(*this,p_icd);

  // Getting the sign
  res.m_sign = res.IsNull() ? Positive : CalculateSign(*this,p_icd);

  return res;
}

// Icd::Div
// Description:   / operator, Dividing of two icd's
// What it does:  if denominator is zero then an exception
//                Using positive dividing en then getting the sign
//
icd
icd::Div(const icd& p_icd) const
{
  // if denominator is zero -> error
  if (p_icd.IsNull())
  {
    throw new CString("Decimal number (icd): Division by zero");
  }
  // Divide without taking the sign into account
  icd res = DividePositive(*this,p_icd);

  // Getting the sign
  res.m_sign = res.IsNull() ? Positive : CalculateSign(*this,p_icd);

  return res;
}

// Icd::Mod
// Description:   Modulo operator
// What it does:  Modulo calculation in icd calculations
icd
icd::Mod(const icd& p_icd) const
{
  icd number = ((*this) / p_icd).ValueBeforePoint();
  icd mod((*this) - (number * p_icd));
  if (m_sign == Negative)
  {
    -mod;
  }
  return mod;
}

//////////////////////////////////////////////////////////////////////////
//
// END ICD BASIC OPERATIONS
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// ICD PRIVATE FUNCTIONS
// ALL INTERNAL FUNCTIONS TO MAKE ICD POSSIBLE
//
//////////////////////////////////////////////////////////////////////////

// Getting the absolute value of a long
// This function bypasses all <math> library and general macros
// Needed because various versions of std:abs linked wrongly in larger apps
long 
icd::long_abs(const long waarde) const
{
  if(waarde < 0)
  {
    return -waarde;
  }
  return waarde;
}

// Gives the 10 or 10000 power of a number
// Used for quick shift operations in an icd
// (operand always between 0 en icdDigits (currently 8))
long 
icd::long_pow(long base,int operand) const
{
  long result = 1;
  if(operand > 0)
  {
    for(short i=0;i<operand;++i)
    {
      result *= base;
    }
    return result;
  }
  // number to the 0th power = 1;
  return 1;
}

// Returns floor(log10(value)) 
// Needed for quick shift operations
// Result is at maximum (icdDigits - 1)
long
icd::long_log10(long value) const
{
  int result = 0;

  while(value >= 10)
  {
    ++result;
    value /= 10;
  }
  return result;
}

// Icd::SetValueLong
// Description:    Setting the icd through two long values
//                 First value is the number before the decimal point
//                 The second number cannot be larger than icdBase (1000000)
// Parameters:     const long value     // value before decimal point
//                 const long remainder // value after decimal point (at max 8 (icdDigits) long)
// What it does:   1) Gets the sign from the value, unless value is zero
//                 2) gets the sign from the remainder if value == 0
//                 3) Sets the first m_data before the decimal point
//                 4) Sets the first m_data after the decimal point
//                 5) Do a reformat to make the icd valid
//
void
icd::SetValueLong(const long p_value, const long p_remainder)
{
  // Empty the number
  MakeEmpty();

  // Getting the sign
  if (p_value == 0)
  {
    m_sign = (p_remainder < 0) ? Negative : Positive;
  }
  else
  {
    m_sign = (p_value < 0) ? Negative : Positive;
  }
  // Fill the digits
  m_data[icdPointPosition-1] = long_abs(p_remainder % icdBase);
  m_data[icdPointPosition  ] = long_abs(p_value);
  Reformat();

  // Initial size and scale
  m_length    = 20; // 2 * length of a long
  m_precision = 10; // laength of a long
}

// Icd::SetValueDouble
//
// Setting the value of the icd from a double
void
icd::SetValueDouble(const double waarde)
{
  // Make icd initially empty
  MakeEmpty();

  // Getting the sign
  double tmpwaarde = fabs(waarde);
  m_sign = (waarde < 0.0) ? Negative : Positive;

  // Getting the part before and after the decimal point
  double part = floor(tmpwaarde);
  double frac = tmpwaarde - part;

  // Fill in the part before the decimal point
  // 5E-9 = 5E(icdDigits + 1)
  int idx = 0;
  while(part && idx<=icdPointPosition)
  {
    tmpwaarde = part / icdBase;
    part      = tmpwaarde - floor(tmpwaarde);
    m_data[icdPointPosition+idx] = (unsigned long)floor((part * icdBase) + 5E-9);
    part      = floor(tmpwaarde);
    ++idx;
  }
  // Fill in the part after the decimal point
  idx = icdPointPosition - 1;
  while(frac && idx >= (icdPointPosition - 2))
  {
    frac       *= icdBase;
    tmpwaarde   = floor(frac);
    m_data[idx] = (unsigned long) (tmpwaarde + 0.5);
    frac       -= tmpwaarde;
    --idx;
  }
  // Round at 14 decimals (max precision of a double)
  // But dependent on icdDigits !!!!
  if(idx == (icdPointPosition - 2))
  {
    if(m_data[idx])
    {
      m_data[idx] = 100 * (m_data[idx] / 100);
    }
  }
  Reformat();
}

// Icd::SetValueString
// Description:    Setting a value in an icd from a string
//								 Characters without meaning will be ignored like in an 'atoi' conversion
//
void
icd::SetValueString(CString p_value)
{
  // Begin with empty icd
  MakeEmpty();

  // Optimization: Ready on empty string
  if(!p_value.GetLength())
  {
    return;
  }

  // Remove the thousands separator
  p_value.Remove(',');

  // Special cases: looking for our own constants: PI / LN2 / LN10
  if(p_value.CompareNoCase("PI") == 0)
  {
    *this = PI();
    return;
  }
  if(p_value.CompareNoCase("LN2") == 0)
  {
    *this = LN2();
    return;
  }
  if(p_value.CompareNoCase("LN10") == 0)
  {
    *this = LN10();
    return;
  }

  // Beginning of the value
  char const* ptr = p_value;

  // Getting the sign
  m_sign = Positive;
  if(*ptr == '-')
  {
    m_sign = Negative;
    ++ptr;
  }
  else if(*ptr == '+')
  {
    m_sign = Positive;
    ++ptr;
  }

  // Getting the length of the integer part
  char const* from = ptr;
  char const* next = ptr;
  while(isdigit(*next))
  {
    ++next;
  }

  // Process the integer part
  long power = 1;
  long index = icdPointPosition;
  for(ptr = next - 1; index < icdLength, ptr >= from; --ptr)
  {
    if(index > icdLength - 1)
    {
      // ICD Overflow
      throw new CString("ICD Overflow");
    }
    m_data[index] += power * (long)(*ptr - '0');
    if((power *= 10) >= icdBase)
    {
      power = 1;
      ++index;
    }
  }

  // Process next part
  ptr = next;

  // Process part after the decimal point
  if(*ptr == '.')
  {
    power = icdBase / 10;
    index = icdPointPosition - 1;
    for (++ptr; *ptr >= '0' && *ptr <= '9'; ++ptr)
    {
      m_data[index] += power * (long)(*ptr - '0');
      if((power /= 10) == 0)
      {
        power = icdBase / 10;
        if(--index < 0)
        {
          // Ignore left over decimals
          while(*ptr >= '0' && *ptr <= '9')
          {
            ++ptr;
          }
          break;
        }
      }
    }
  }

  // Getting the optional exponent
  int exponent = 0;
  if(*ptr == 'e' || *ptr == 'E')
  {
    ++ptr;
    char const* sign = ptr;
    if(*ptr == '+' || *ptr == '-')
    {
      ++ptr;
    }
    while(*ptr >= '0' && *ptr <= '9')
    {
      exponent *= 10;
      exponent += *ptr - '0';
      ++ptr;
    }
    if(*sign == '-')
    {
      exponent = -exponent;
    }
  }
  // Calculate the effect of the exponent
  if(exponent)
  {
    CalculateEFactor(exponent);
  }

  // String must now be empty
//   if(*ptr)
//   {
//     // Cannot convert
//     throw new CString("Cannot convert to icd");
//   }
}

// Icd::SetValueInt64
// Description:  Set the icd to an int64 value
//               and optionally a remainder after the decimal point
void
icd::SetValueInt64(const int64 p_value, const int64 p_remainder)
{
  int64 rest = 0;

  // Start with an empty icd
  MakeEmpty();

  // Getting the sign
  if (p_value == 0)
  {
    m_sign = (p_remainder < 0) ? Negative : Positive;
  }
  else
  {
    m_sign = (p_value < 0) ? Negative : Positive;
    rest   = (p_value < 0) ? -p_value : p_value;
  }

  // Fill in the value
  for(int ind = icdPointPosition; rest > 0 && ind < icdLength; ++ind)
  {
    m_data[ind] = rest % icdBase;
    rest /= icdBase;
  }
  // Fill in the remainder
  int64 deel = p_remainder < 0 ? -p_remainder : p_remainder;
  for(int ind = icdPointPosition -1; deel > 0 && ind >= 0; --ind)
  {
    m_data[ind] = deel % icdBase;
    deel /= icdBase;
  }
  Reformat();
}

// Put a SQL_NUMERIC_STRUCT from the ODBC driver in the icd
void
icd::SetValueNumeric(SQL_NUMERIC_STRUCT* p_numeric)
{
  MakeEmpty();

  int maxval = SQL_MAX_NUMERIC_LEN - 1;


  // Find the last value in the numeric
  int ind = maxval;
  for(;ind >= 0; --ind)
  {
    if(p_numeric->val[ind])
    {
      maxval = ind;
      break;
    }
  }

  // Special case: NUMERIC = zero
  if(ind >= 0)
  {
    // Compute the value array to the bcd-mantissa
    icd radix(1L);
    for(ind = 0;ind <= maxval; ++ind)
    {
      icd val = radix * icd((long) (p_numeric->val[ind]));
      *this = Add(val);
      radix = radix.Multi(256);  // Value array is in 256 radix
    }
  }
  // Use internal registrations
  m_sign      = p_numeric->sign ? Sign::Positive : Sign::Negative;
  m_precision = p_numeric->scale;
  m_length    = p_numeric->precision;

  // Adjust for precision
  for(int divide = 0;divide < m_precision; ++divide)
  {
    Div10();
  }
}

// Icd::CalculateEFactor
// Description: icd to the 10th power of a factor
void
icd::CalculateEFactor(int factor)
{
  static const int positionsBeforePoint = icdPointPosition * icdDigits;
  static const int positionsAfterPoint = (icdLength - icdPointPosition) * icdDigits;
  static const icd basisICD((long)icdBase);

  // See to it that the factor is in the range [-9,+9].
  while (factor > 9)
  {
    *this = Mul(1000000000L /* 9 positions */);
    factor -= 9;
  }
  while (factor < -9)
  {
    *this = Div(1000000000L /* 9 positions */);
    factor += 9;
  }
  // Shift the rest by doing an icd-multiply
  // the long-pow is now possible because 10^factor is within range of a 32-bits long
  if (factor > 0)
  {
    *this = Mul(long_pow(10,factor));
  }
  else if (factor < 0)
  {
    *this = Div(long_pow(10,-factor));
  }
}

// Icd::Reformat
// Description:    See to it that all m_data members are correct again (<IcdBasis)
// Excepties:      If icd has become too big after reformatting  we do an Overflow exception
// What it does:   Truncate the m_data values from highest till lowest value
//                 so that m_data[i] < IcdBase. The remainder will be added to the next 
//                 m_data member. (IcdOverflow if no more m_data member available)
//
void
icd::Reformat()
{
  // Shift values to higher members of m_data
  long remainder = 0;
  for (long i = 0; i < icdLength; i++)
  {
    m_data[i] += remainder;
    remainder  = m_data[i] / icdBase;
    m_data[i]  = m_data[i] % icdBase;
  }
  // Still a remainder at the end? 
  if (remainder > 0)
  {
    throw new CString("Decimal number too big (ICD Overflow)");
  }
}

// Icd::MakeEmpty
// Description:   Make icd zero
//
void
icd::MakeEmpty()
{
  // initialize data members to +0000...0000,0000...0000
  m_sign      = Positive;
  m_length    = 0; // icdLength * icdDigits;
  m_precision = 0;
  memset(m_data,0,sizeof(unsigned long) * icdLength);
}

// Icd::Multi
// Description:    Multiplies a (positive) icd with a positive long
//                 If the icd becomes to big, it's set to negative
// Parameters:     const long multiplier // number to multiply with
// Retourneert:    this * multiplier
// Precondition:   Sign is positive, the parameter is positive
// What it does:   Multiplies every member of m_data with the parameter
//                 After this you need to do a 'Reformat'
//
const icd
icd::Multi(const long multiplier) const
{
  int64 between = 0;
  int64 remain  = 0;
  icd res(*this);

  // Multiplying
  for(long i = 0; i < icdLength; ++i)
  {
    between        = (int64)res.m_data[i] * (int64)multiplier;
    between       += remain;
    res.m_data[i]  = (long) (between % icdBase);
    remain         = between / icdBase;
  }
  if(remain > 0)
  {
    // If overflow, make sign negative, so we can detect the overflow
    res.m_sign = Negative;
  }
  return res;
}

// Icd::IsCornerCase
// Description: Test if icd has become to big for the precision
//
const bool
icd::IsCornerCase() const
{
  // Test if a corner case where the round makes the length to long
  // E.g.: 9,995 with length = 3 and precision = 2, will be rounded to 10,00 (length = icdDigits!)

  int highestIndex = (icdDigits * icdPointPosition + (m_length - m_precision) - 1) / icdDigits;
  int lowestIndex  = (icdDigits * icdPointPosition - m_precision) / icdDigits;


  int index = highestIndex;

  while (index >= lowestIndex)
  {
    unsigned long dataValue = m_data[index];
    unsigned long maxValue;

    if (index == highestIndex)
    {
      maxValue = (unsigned long) (long_pow(10, (m_length - m_precision) % icdDigits) - 1);
      if (maxValue == 0)
      {
        maxValue = 9999;
      }
      if (dataValue != maxValue)
      {
        if (index != lowestIndex)
        {
          return false;
        }
      }
    }
    if (index == lowestIndex)
    {
      int orde = (icdDigits - (m_precision % icdDigits)) % icdDigits;
      unsigned long relevantPart = dataValue / (unsigned long)(long_pow(10, orde));
      maxValue = (unsigned long) (long_pow(10, (icdDigits - orde)) - 1);  

      if (relevantPart == maxValue)
      {
        // Getting the number that makes the rounding (first after the precision)
        int number;
        if (orde > 0)
        {
          number = ((dataValue % (unsigned long) long_pow(10, orde)) / (unsigned long) long_pow(10, orde - 1));
        }
        else
        {
          // Number is in next data block
          number = m_data[index - 1] / icdBase; // 1000;
        }

        if (number >= (icdDigits + 1)) // 5
        {
          return true;
        }
        else 
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
    if (!(((index != highestIndex) || (index != lowestIndex)) && (dataValue == (icdBase - 1)))) // 9999
    {
      return false;
    }
    --index;
  }
  return false;
}

// Icd::PositionArguments
//
// Description:    Positions two arguments for adding and subtracting operations
//                 (+x) + (+y) -> Adding,   result positive, flip arguments
//                 (+x) + (-y) -> Subtract, result positive, do NOT flip arguments
//                 (-x) + (+y) -> Subtract, result positive, flip arguments
//                 (-x) + (-y) -> Adding,   result negative, do NOT flip arguments
//
// Parameters:     icd& arg1        // argument on the left side
//                 icd& arg2        // argument on the right side
//                 Sign& signResult // Total sign result
//                 OperatorKind& kind // Operator to do (+ or -)
//
// What it does:   On the basis of the signs we find the sign of the result
//                 and the type of operation to perform
//
void
icd::PositionArguments(icd&  arg1,
                       icd&  arg2,
                       Sign& signResult,
                       OperatorKind& kindOperator)
{
  // Getting the result sign
  // if (-x) + y then flip, so: x + (-y), becomes: x - y
  if (arg1.m_sign == Positive)
  {
    if (arg2.m_sign == Positive)
    {
      signResult   = Positive;
      kindOperator = Adding;
    }
    else
    {
      signResult   = Positive;
      kindOperator = Subtracting;
    }
  }
  else
  {
    if (arg2.m_sign == Positive)
    {
      signResult = Positive;
      kindOperator = Subtracting;
      // Flip the arguments
      icd dummy(arg1);
      arg1 = arg2;
      arg2 = dummy;
    }
    else
    {
      signResult   = Negative;
      kindOperator = Adding;
    }
  }
  // Carry on in positive in both sides
  arg1.m_sign = Positive;
  arg2.m_sign = Positive;
}

// Icd::AddPositive
// Description:    Adds two icd's without taking the sign into account
// Parameters:     const icd& arg1 // argument on the left side
//                 const icd& arg2 // argument on the right side
// Returns:        arg1+arg2 as an icd
// Precondition:   arg1.m_sign = positive, arg2.m_teken = positive
// Exceptions:     IcdOverflow if the result does not fit into an icd
// What it does:   Adds all m_data members one-by-one after which a reformat occurs
//
const icd
icd::AddPositive(const icd& arg1, const icd& arg2)
{
  // Add all data members
  icd res(arg1);
  for (long i = 0; i < icdLength; i++)
  {
    res.m_data[i] += arg2.m_data[i];
  }
  // Reformat action
  res.Reformat();
  return res;
}

// Icd::SubtractPositive
// Description:    Subtracts two icd's without taking the sign into account
// Parameters:     const icd& arg1 // argument on the left side
//                 const icd& arg2 // argument on the right side
// Returns:        arg1-arg2 as an icd
// Precondition:   arg1.m_sign = positive, arg2.m_sign = positive
// What it does:   We do a subtract of all m_Data members
//                 if the left data is smaller than the right data
//                 we doe a borrow from the next data member on the left hand side
//                 unless it does not exist, so we do a switch of the sign
//                 to make the underflow detectable outside the function
//
const icd
icd::SubtractPositive(const icd& arg1, const icd& arg2)
{
  // Temporary icd's
  icd res(arg1);
  icd mina(arg2);

  // res = res - mina
  for (long i = 0; i < icdLength; i++)
  {
    if (res.m_data[i] < mina.m_data[i])
    {
      // Smaller, so do  borrow from our neighbor
      res.m_data[i] += icdBase - mina.m_data[i];
      if (i < icdLength - 1)
      {
        // Beware: we do not do a: res.m_data[i+1]--
        // as in a regular borrow, because the member
        // could become below zero
        mina.m_data[i+1]++;
      }
      else
      { 
        // Switch signs to flag underflow
        res.m_sign = Negative;
      }
    }
    else
    {
      res.m_data[i] -= mina.m_data[i];
    }
  }
  if (res.m_sign == Negative)
  {
    // Switched signs, the result must be corrected
    // by subtracting the result from the theoretical maximum
    // N.B.: It's a recursive call of SubtractPositive!
    icd maxIcd;
    maxIcd.m_data[icdLength-1] = icdBase;
    res = -(maxIcd + res);
  }
  return res;
}

// Icd::MultiplyPositive
// Description:    Multiply two icd's without taking the sign into account
// Parameters:     icd& arg1 // argument on the left side
//                 icd& arg2 // argument on the righ tside
// Returns:        arg1*arg2
// Precondition:   arg1.m_sign = positive, arg2.m_sign = positive
// Exceptions:     BKIcdOverflow if the result becomes too big
// What it does:   Multiplies each m_data member in the classical way
//                 The result is saved in an array that's two times bigger
//                 then the regular m_data array. This is done so all values
//                 can be stored. If the result doesn't fit into a regular
//                 m_data array at the end, an ICD overflow exception will follow
//                 As a last step we doe a 'Reformat' to get everything in line
//
const icd
icd::MultiplyPositive(const icd& arg1, const icd& arg2)
{
  // temporary m_data's so we do not need any longer icd structures elsewhere
  int64 res[icdLength * 2 + 1] = {0};
  int64 between = 0;

  // Multiply and store the result in the 'res' array
  for (int i = 0; i < icdLength; i++)
  {
    for (int j = 0; j < icdLength; j++)
    {
      between      = (int64)arg1.m_data[i] * (int64)arg2.m_data[j];
      res[i+j]   += (long) (between % icdBase);
      res[i+j+1] += (long) (between / icdBase);

      // Check on overflow
      if (((i+j) >= (icdLength + icdPointPosition)) && (res[i+j] != 0))
      {
        // If something has been stored above the length and pointposition -> overflow
        throw new CString("Decimal number too big (ICD Overflow)");
      }
    }
  }
  // Reformat
  int64 remain = 0;
  for (long i = 0; i < icdLength * 2; i++)
  {
    res[i] += remain;
    remain  = res[i] / icdBase;
    res[i]  = res[i] % icdBase;
  }
  // Still a remainder?
  if (remain > 0)
  {
    throw new CString("Decimal number too big (ICD Overflow)");
  }

  // Setting the result in an icd, taking the decimal point position into account
  icd Icdres;
  for (int i = 0; i < icdLength; i++)
  {
    Icdres.m_data[i] = (long) res[i+icdPointPosition];
  }
  return Icdres;
}

// Icd::DividePositive
// Description:    Divides two icd's without taking the sign into account
// Parameters:     icd& arg1 // numerator
//                 icd& arg2 // denominator (non zero)
// Returns:        arg1 / arg2
// Precondition:   arg1.m_teken=positief, arg2.m_teken=positief, arg2!=0
// Exceptions:     IcdOverflow if the result becomes too big
// What it does:   First we look for the most significant m_data member that is non zero
//                 Then we determine the placing of the decimal point.
//                 After this we do a 'classical tail-division'
//                 To find a next number, we divide the most significant data members
//                 So we find the highest and lowest possible values for the quotient
//                 Then we do a binary search to find the 'real' quotient.
//
const icd
icd::DividePositive(const icd& arg1, const icd& arg2)
{
  // numerator   is argument 1
  // denominator is argument 2
  // res         Will be the result
  // The numerator will also be used to store parts of the numerator
  icd numerator(arg1);
  icd denominator(arg2);
  icd res;

  // Only positive dividing, sign will be set later
  numerator.m_sign   = Positive;
  denominator.m_sign = Positive;

  // Index1 is the highest numerator.m_data that is not null
  // index2 is the highest denominator.m_data that is not null
  long index1 = icdLength - 1;
  long index2 = icdLength - 1;
  while (index1 > 0 && numerator.m_data[index1] == 0)
  {
    index1--;
  }
  while (index2 > 0 && denominator.m_data[index2] == 0)
  {
    index2--;
  }

  // resindex is the position in the array where the result will set
  // the decimal point position is defined in this way beforehand
  long resindex = index1 - index2 + icdPointPosition;

  // multires is the result number from a step in the dividing process
  // denominatorSame is the denominator that has been aligned with the numerator
  // The most-significant number of the denominator is on the same place as the
  // most-significant number of the numerator
  // Example: numerator is 12:32:43 and denominator is 32:43, then the
  // denominator will be shifted to "32:43:00"
  icd multires;
  icd denominatorSame;

  // The actual dividing action, go on until the number of digits is reached
  while (resindex >= 0 && index1 >= 0)
  {
    // quotient_test is equal to the most-significant number from the numerator
    // divided by the most-significant number from the denominator.
    // If there was a remainder from the last division, than take that into account
    // For optimalisation we do a binary search.
    // We assume that a/b is can give a quotient that is too big and 
    // (a/(b+1) a quotient that is too small. In this way we find an upper and lower
    // border. quotient_test will be between these borders and quotient_below_old
    // was the lowest border that is still possible
    int64 quotient_above = ((int64)numerator.m_data[index1] + ((index1 == icdLength-1) ? 0 : (int64)numerator.m_data[index1+1] * icdBase)) / (int64)denominator.m_data[index2];
    int64 quotient_below = ((int64)numerator.m_data[index1] + ((index1 == icdLength-1) ? 0 : (int64)numerator.m_data[index1+1] * icdBase)) / (int64)(denominator.m_data[index2] + 1);
    int64 quotient_below_old = 0;
    int64 quotient_test = (quotient_above + quotient_below) / 2;

    // Betting multires. If the multiplication gets into overflow, 
    // the sign will be negative, so the quotient must be smaller!
    denominatorSame = BringTogether(denominator, index1 - index2);
    bool tooBig;
    multires = denominatorSame.Multi((long)quotient_test);
    tooBig   = multires.m_sign == Negative;

    // Continue until we reach the lower border not equal to the upperborder
    while (quotient_above != quotient_below)
    {
      if (numerator < multires || tooBig)
      {
        quotient_above = quotient_test;
      }
      else
      {
        // Save valid lower border!
        quotient_below_old = quotient_test;
        if (((quotient_below + quotient_above) % 2) == 1)
        {
          // If we get into a rounding error, choose the lower border 1 higher
          quotient_below = quotient_test + 1;
        }
        else
        {
          quotient_below = quotient_test;
        }
      }
      quotient_test = (quotient_above + quotient_below) / 2;
      denominatorSame = BringTogether(denominator, index1 - index2);
      multires = denominatorSame.Multi((long)quotient_test);
      tooBig = multires.m_sign == Negative;
    }
    // If the quotient is still too high, use old lower border
    if (numerator < multires || tooBig)
    {
      quotient_test   = quotient_below_old;
      denominatorSame = BringTogether(denominator, index1 - index2);
      multires = denominatorSame.Multi((long)quotient_test);
    }
    // If non zero, use the quotient 
    if (quotient_test != 0)
    {
      if (resindex >= icdLength)
      {
        // Overflow if it became too big
        throw new CString("Decimal number too big (ICD Overflow)");
      }
      else
      {
        res.m_data[resindex] = (long)quotient_test;
      }
    }
    // Subtract from the numerator. If a position on the left hand side became free
    // then shift the numerator, so we do not lack on the right hand side
    // (equivalent to getting and extra '0' in a tail division)
    numerator = numerator - multires;
    if (numerator.m_data[icdLength-1] == 0)
    {
      for (long i = icdLength - 1; i > 0; i--)
      {
        numerator.m_data[i] = numerator.m_data[i-1];
      }
      numerator.m_data[0] = 0;
    }
    else
    {
      --index1;
    }
    --resindex;
  }
  return res;
}

// Icd::CalculateSign
// Description:    Calculates the sign for a multiplication or a division
// Parameters:     const icd& arg1 // 1th argument
//                 const icd& arg2 // 2th argument
// What it does:   First see if one of both arguments is zero
//                 Then calculates the sign
//
const icd::Sign
icd::CalculateSign(const icd& arg1, const icd& arg2)
{
  // Getting the sign
  // If both arguments are 0 -> positive
  // (+x) * (+y) -> positive
  // (-x) * (+y) -> negative
  // (-x) * (-y) -> positive
  // (+x) * (-y) -> negative
  if (arg1.IsNull() || arg2.IsNull())
  {
    return Positive;
  }
  if (arg1.m_sign == Positive)
  {
    if (arg2.m_sign == Positive)
    {
      return Positive;
    }
    else
    {
      return Negative;
    }
  }
  else
  {
    if (arg2.m_sign == Negative)
    {
      return Positive;
    }
    else
    {
      return Negative;
    }
  }
}

// Icd::BringTogether
// Description:    Shifts the icd a number of places left or right
//                 dependent on the second argument
// Parameters:     const icd& arg1   // icd to shift
//                 const long places // places to shift
// Returns:        Shifted icd
// What it does:   Used to shift an icd to bring it in line with another one
//                 second argument shifts right (negative) or left (positive)
//
const icd
icd::BringTogether(const icd& arg1, const long places)
{
  icd res;

  // if no displacement: do nothing
  if (places == 0)
  {
    res = arg1;
  }
  else
  {
    if (places < 0)
    {
      // denominator is bigger than the numerator, shift the denominator to the right
      // example: 31:12:00 becomes 00:31:12 at a shift of -1
      for (long i = 0; i < icdLength + places; i++)
      {
        res.m_data[i] = arg1.m_data[i-places];
      }
    }
    else
    {
      // denominator is smaller than the numerator, shift the denominator to the left
      // example: 00:31:12 becomes 31:12:00 at a shift of 1
      for (long i = icdLength - 1; i >= places; i--)
      {
        res.m_data[i] = arg1.m_data[i-places];
      }
    }
  }
  return res;
}

// Icd::Div10 
// Description:  Divide mantissa (m_data) by 10
// What it does: Purely a shift register operation by dividing everything by 10
void
icd::Div10()
{
  for(int ind = icdLength - 1;ind >= 0; --ind)
  {
    long tussen = m_data[ind] / 10;
    if(ind > 0)
    {
      long extra = m_data[ind] % 10;
      m_data[ind - 1] = (extra * icdBase) + m_data[ind - 1];
    }
    m_data[ind] = tussen;
  }
}

// Icd::Mult10
// Description:   Multiplies mantissa (m_data) with 10
// What it does:  Purely a shift register operation by multiplying everything by 10
void
icd::Mult10()
{
  long carry = 0;
  for(int ind = 0; ind < icdLength; ++ind)
  {
    long tussen = m_data[ind] * 10 + carry;
    m_data[ind] = tussen % icdBase;
    carry       = tussen / icdBase;
  }
  if(carry)
  {
    // Should not occur: Mult10 should be safely called internally!
    throw new CString("Internal: ICD Overflow in sub operation");
  }
}

// Icd::LongToString
// Description: Long to a string in radix 10
CString
icd::LongToString(long p_getal) const
{
  char buffer[20];
  _itoa_s(p_getal,buffer,20,10);
  return CString(buffer);
}

// Icd::StringToLong
// Description: String to a long
long
icd::StringToLong(CString& p_string) const
{
  return atoi(p_string);
}

// Icd::Epsilon
// Description:  Getting the breaking criterion epsilon
// What it does: Transposes the fraction to the lowest m_data position
//               10 -> 0.0000000000000000000000000000000000000010
//                5 -> 0.0000000000000000000000000000000000000005
icd&
icd::Epsilon(long p_fraction) const
{
  static icd epsilon;
  epsilon.m_data[0] = p_fraction;
  return epsilon;
}
