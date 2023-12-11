/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: bcd200.cpp
//
// Copyright (c) 2014-2021 ir. W.E. Huisman
// All rights reserved
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
//////////////////////////////////////////////////////////////////////////
//
//  BCD
// 
//  Floating Point Precision Number class (Binary Coded Decimal)
//  An Arbitrary float always has the format [sign][digit][.[digit]*][E[sign][digits]+] 
//  where sign is either '+' or '-' or is missing ('+' implied)
//  And is always stored in normalized mode after an operation or conversion
//  with an implied decimal point between the first and second position
//
// Copyright (c) 2014-2022 ir W. E. Huisman
// Version 1.5 of 03-01-2022
//
//  Examples:
//  E+03 15456712 45000000 00000000 -> 1545.671245
//  E+01 34125600 00000000 00000000 -> 3.41256
//  E-05 78976543 12388770 00000000 -> 0.0000789765431238877
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"         // Precompiled headers
#include "bcd200.h"         // OUR INTERFACE
#include "StdException.h"   // Exceptions
#include <math.h>           // Still needed for conversions of double
#include <locale.h>
#include <winnls.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Theoretical maximum of numerical separators
#define SEP_LEN 10

// [200]
// THESE GLOBALS ARE ALREADY IN THE bcd CLASS

// string format number and money format functions
// bool  g_locale_valutaInit = false;
// TCHAR g_locale_decimalSep [SEP_LEN + 1];
// TCHAR g_locale_thousandSep[SEP_LEN + 1];
// TCHAR g_locale_strCurrency[SEP_LEN + 1];
// int   g_locale_decimalSepLen   = 0;
// int   g_locale_thousandSepLen  = 0;
// int   g_locale_strCurrencyLen  = 0;
// 
// // Error handling throws or we silently return -INF, INF, NaN
extern bool g_throwing;

// One-time initialization for printing numbers in the current locale
// [200] Name is expanded (global function!)
void 
InitValutaString200()
{
  if(g_locale_valutaInit == false)
  {
    GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL,  g_locale_decimalSep, SEP_LEN);
    GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, g_locale_thousandSep,SEP_LEN);
    GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SCURRENCY, g_locale_strCurrency,SEP_LEN);
    g_locale_decimalSepLen  = (int)_tclen(g_locale_decimalSep);
    g_locale_thousandSepLen = (int)_tclen(g_locale_thousandSep);
    g_locale_strCurrencyLen = (int)_tclen(g_locale_strCurrency);

    g_locale_valutaInit = true;
  }
}

//////////////////////////////////////////////////////////////////////////
//
// CONSTRUCTORS OF BCD
//
//////////////////////////////////////////////////////////////////////////

// bcd200::bcd200
// Description: Default constructor
// Technical:   Initialize the number at zero (0)
bcd200::bcd200()
{
  Zero();
}

// bcd200::bcd200(bcd200& arg)
// Description: Copy constructor of a bcd200
// Technical:   Copies all data members
bcd200::bcd200(const bcd200& p_arg)
{
  m_sign      = p_arg.m_sign;
  m_exponent  = p_arg.m_exponent;
  // Create and copy mantissa
  memcpy(m_mantissa,p_arg.m_mantissa,bcdLength * sizeof(long));
}

// bcd200::bcd200(value)
// Description: BCD from a char value
bcd200::bcd200(const TCHAR p_value)
{
  SetValueInt((int)p_value);
}

#ifndef UNICODE
// bcd200::bcd200(value)
// Description: BCD from an unsigned char value
bcd200::bcd200(const _TUCHAR p_value)
{
  SetValueInt((int)p_value);
}
#endif

// bcd200::bcd200(value)
// Description: BCD from a short value
// 
bcd200::bcd200(const short p_value)
{
  SetValueInt((int)p_value);
}

// bcd200::bcd200(value)
// Description: BCD from an unsigned short value
//
bcd200::bcd200(const unsigned short p_value)
{
  SetValueInt((int)p_value);
}

// bcd200::bcd200(value)
// BCD from an integer
bcd200::bcd200(const int p_value)
{
  SetValueInt(p_value);
}

// bcd200::bcd200(value)
// BCD from an unsigned integer
bcd200::bcd200(const unsigned int p_value)
{
  SetValueInt64((int64)p_value,0);
}

// bcd200::bcd200(value,value)
// Description: Construct a BCD from a long and an option long
// Technical:   See description of SetValueLong
//
bcd200::bcd200(const long p_value, const long p_restValue /*= 0*/)
{
  SetValueLong(p_value,p_restValue);
}

// bcd200::bcd200(value,value)
// Description: Construct a BCD from an unsigned long and an unsigned optional long
// Technical:   See description of SetValueLong
bcd200::bcd200(const unsigned long p_value, const unsigned long p_restValue /*= 0*/)
{
  SetValueInt64((int64)p_value,(int64)p_restValue);
}

// bcd200::bcd200(value,value)
// Description: Construct a BCD from a 64bit long and an optional long
// Technical:   See description of SetValueInt64
//
bcd200::bcd200(const int64 p_value,const int64 p_restvalue /*= 0*/)
{
  SetValueInt64(p_value,p_restvalue);
}

bcd200::bcd200(const uint64 p_value,const int64 p_restvalue)
{
  SetValueUInt64(p_value,p_restvalue);
}

// bcd200::bcd200(float)
// Description: Construct a bcd200 from a float
// 
bcd200::bcd200(const float p_value)
{
  SetValueDouble((double)p_value);
}

// bcd200::bcd200(double)
// Description: Construct a bcd200 from a double
// 
bcd200::bcd200(const double p_value)
{
  SetValueDouble(p_value);
}

// BCD From a character string
// Description: Assignment-constructor from an elementary character data pointer
// Parameters:  p_string -> Input character pointer (containing a number)
//              p_fromDB -> Input comes from a  database (always American format)
bcd200::bcd200(LPCTSTR p_string,bool p_fromDB /*= false*/)
{
  SetValueString(p_string,p_fromDB);
}

// bcd200::bcd200(numeric)
// Description: Assignment-constructor for bcd200 from a SQL NUMERIC
// Parameters:  p_numeric -> Input from a SQL ODBC database
//                           from a NUMERIC field
//
bcd200::bcd200(const SQL_NUMERIC_STRUCT* p_numeric)
{
  SetValueNumeric(p_numeric);
}

// bcd200::bcd200(Sign)
// Description: Construct a bcd200 from a NULL in the database
// Parameters:  p_sign : BUT GETS IGNORED!!
//
bcd200::bcd200(const bcd200::Sign /*p_sign*/)
{
  Zero();
  // We ignore the argument!!
  m_sign = Sign::ISNULL;
}

//////////////////////////////////////////////////////////////////////////
//
// END OF CONSTRUCTORS OF BCD
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// CONSTANTS OF BCD
//
//////////////////////////////////////////////////////////////////////////

// bcd200::PI
// Description: Circumference/Radius ratio of a circle
// Technical:   Nature constant that never changes
bcd200
bcd200::PI()
{
  bcd200 pi;

  // [200]
  // PI in 200 decimals: 
  // According to math.com
  // http://www.math.com/tables/constants/pi.htm
  // 
  //  +31415926 53589793 23846264 33832795 02884197 
  //   16939937 51058209 74944592 30781640 62862089
  //   98628034 82534211 70679821 48086513 28230664
  //   70938446 09550582 23172535 94081284 81117450
  //   28410270 19385211 05559644 62294895 49303819
  //   64428810 97566593 34461284 75648233 78678316

  pi.m_mantissa[ 0] = 31415926L;
  pi.m_mantissa[ 1] = 53589793L;
  pi.m_mantissa[ 2] = 23846264L;
  pi.m_mantissa[ 3] = 33832795L;
  pi.m_mantissa[ 4] =  2884197L;

  pi.m_mantissa[ 5] = 16939937L;
  pi.m_mantissa[ 6] = 51058209L;
  pi.m_mantissa[ 7] = 74944592L;
  pi.m_mantissa[ 8] = 30781640L;
  pi.m_mantissa[ 9] = 62862089L;

  pi.m_mantissa[10] = 98628034L;
  pi.m_mantissa[11] = 82534211L;
  pi.m_mantissa[12] = 70679821L;
  pi.m_mantissa[13] = 48086513L;
  pi.m_mantissa[14] = 28230664L;

  pi.m_mantissa[15] = 70938446L;
  pi.m_mantissa[16] =  9550582L;
  pi.m_mantissa[17] = 23172535L;
  pi.m_mantissa[18] = 94081284L;
  pi.m_mantissa[19] = 81117450L;

  pi.m_mantissa[20] = 28410270L;
  pi.m_mantissa[21] = 19385211L;
  pi.m_mantissa[22] =  5559644L;
  pi.m_mantissa[23] = 62294895L;
  pi.m_mantissa[24] = 49303819L;

  return pi;
}

// bcd200::LN2
// Description: Natural logarithm of two
// Technical:  Mathematical constant that never changes
bcd200
bcd200::LN2()
{
  bcd200 ln2;

  // [200]
  // LN2 in 200 decimals: Use if you expand bcdLength
  // +0.69314718_05599453_09417232_12145817_65680755_
  //    00134360_25525412_06800094_93393621_96969471_
  //    56058633_26996418_68754200_14810205_70685733_
  //    68552023_57581305_57032670_75163507_59619307_
  //    27570828_37143519_03070386_23891673_47111855

  ln2.m_exponent = -1;
  ln2.m_mantissa[ 0] = 69314718L;
  ln2.m_mantissa[ 1] =  5599453L;
  ln2.m_mantissa[ 2] =  9417232L;
  ln2.m_mantissa[ 3] = 12145817L;
  ln2.m_mantissa[ 4] = 65680755L;

  ln2.m_mantissa[ 5] =   134360L;
  ln2.m_mantissa[ 6] = 25525412L;
  ln2.m_mantissa[ 7] =  6800094L;
  ln2.m_mantissa[ 8] = 93393621L;
  ln2.m_mantissa[ 9] = 96969471L;

  ln2.m_mantissa[10] = 56058633L;
  ln2.m_mantissa[11] = 26996418L;
  ln2.m_mantissa[12] = 68754200L;
  ln2.m_mantissa[13] = 14810205L;
  ln2.m_mantissa[14] = 70685733L;

  ln2.m_mantissa[15] = 68552023L;
  ln2.m_mantissa[16] = 57581305L;
  ln2.m_mantissa[17] = 57032670L;
  ln2.m_mantissa[18] = 75163507L;
  ln2.m_mantissa[19] = 59619307L;

  ln2.m_mantissa[20] = 27570828L;
  ln2.m_mantissa[21] = 37143519L;
  ln2.m_mantissa[22] =  3070386L;
  ln2.m_mantissa[23] = 23891673L;
  ln2.m_mantissa[24] = 47111855L;
    
  return ln2;
}

// bcd200::LN10
// Description: Natural logarithm of ten
// Technical:   Mathematical constant that never changes
bcd200
bcd200::LN10()
{
  bcd200 ln10;

  // [200]
  // LN10 in 200 decimals: Use if you expand bcdLength
  // +2.3025850_92994045_68401799_14546843_64207601_
  //   10148862_87729760_33327900_96757260_96773524_
  //   80235997_20508959_82983419_67784042_28624863_
  //   34095254_65082806_75666628_73690987_81689482_
  //   90720832_55546808_43799894_82623319_85283296

  ln10.m_mantissa[ 0] = 23025850L;
  ln10.m_mantissa[ 1] = 92994045L;
  ln10.m_mantissa[ 2] = 68401799L;
  ln10.m_mantissa[ 3] = 14546843L;
  ln10.m_mantissa[ 4] = 64207601L;

  ln10.m_mantissa[ 5] = 10148862L;
  ln10.m_mantissa[ 6] = 87729760L;
  ln10.m_mantissa[ 7] = 33327900L;
  ln10.m_mantissa[ 8] = 96757260L;
  ln10.m_mantissa[ 9] = 96773524L;

  ln10.m_mantissa[10] = 80235997L;
  ln10.m_mantissa[11] = 20508959L;
  ln10.m_mantissa[12] = 82983419L;
  ln10.m_mantissa[13] = 67784042L;
  ln10.m_mantissa[14] = 28624863L;

  ln10.m_mantissa[15] = 34095254L;
  ln10.m_mantissa[16] = 65082806L;
  ln10.m_mantissa[17] = 75666628L;
  ln10.m_mantissa[18] = 73690987L;
  ln10.m_mantissa[19] = 81689482L;

  ln10.m_mantissa[20] = 90720832L;
  ln10.m_mantissa[21] = 55546808L;
  ln10.m_mantissa[22] = 43799894L;
  ln10.m_mantissa[23] = 82623319L;
  ln10.m_mantissa[24] = 85283296L;

  return ln10;
}

//////////////////////////////////////////////////////////////////////////
//
// END OF CONSTANTS OF BCD
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// ERROR HANDLING
//
//////////////////////////////////////////////////////////////////////////

/*static */ void 
bcd200::ErrorThrows(bool p_throws /*= true*/)
{
  g_throwing = p_throws;
}

//////////////////////////////////////////////////////////////////////////
//
// OPERATORS OF BCD
//
//////////////////////////////////////////////////////////////////////////

// bcd200::+
// Description: Addition operator
//
const bcd200
bcd200::operator+(const bcd200& p_value) const
{
  return Add(p_value);
}

const bcd200
bcd200::operator+(const int p_value) const
{
  return Add(bcd200(p_value));
}

const bcd200
bcd200::operator+(const double p_value) const
{
  return Add(bcd200(p_value));
}

const bcd200
bcd200::operator+(LPCTSTR p_value) const
{
  return Add(bcd200(p_value));
}

// bcd200::-
// Description: Subtraction operator
const bcd200
bcd200::operator-(const bcd200& p_value) const
{
  return Sub(p_value);
}

const bcd200
bcd200::operator-(const int p_value) const
{
  return Sub(bcd200(p_value));
}

const bcd200
bcd200::operator-(const double p_value) const
{
  return Sub(bcd200(p_value));
}

const bcd200
bcd200::operator-(LPCTSTR p_value) const
{
  return Sub(bcd200(p_value));
}

// bcd200::*
// Description: Multiplication operator
const bcd200
bcd200::operator*(const bcd200& p_value) const
{
  return Mul(p_value);
}

const bcd200
bcd200::operator*(const int p_value) const
{
  return Mul(bcd200(p_value));
}

const bcd200
bcd200::operator*(const double p_value) const
{
  return Mul(bcd200(p_value));
}

const bcd200
bcd200::operator*(LPCTSTR p_value) const
{
  return Mul(bcd200(p_value));
}

// bcd200::/
// Description: Division operator
const bcd200
bcd200::operator/(const bcd200& p_value) const
{
  return Div(p_value);
}

const bcd200
bcd200::operator/(const int p_value) const
{
  return Div(bcd200(p_value));
}

const bcd200
bcd200::operator/(const double p_value) const
{
  return Div(bcd200(p_value));
}

const bcd200
bcd200::operator/(LPCTSTR p_value) const
{
  return Div(bcd200(p_value));
}

// bcd200::%
// Description: Modulo operator
const bcd200
bcd200::operator%(const bcd200& p_value) const
{
  return Mod(p_value);
}

const bcd200  
bcd200::operator%(const int p_value) const
{
  return Mod(bcd200(p_value));
}

const bcd200
bcd200::operator%(const double p_value) const
{
  return Mod(bcd200(p_value));
}

const bcd200
bcd200::operator%(LPCTSTR p_value) const
{
  return Mod(bcd200(p_value));
}

// bcd200::operator +=
// Description: Operator to add a bcd200 to this one
bcd200&
bcd200::operator+=(const bcd200& p_value)
{
  *this = Add(p_value);
  return *this;
}

bcd200&
bcd200::operator+=(const int p_value)
{
  *this = Add(bcd200(p_value));
  return *this;
}

bcd200&
bcd200::operator+=(const double p_value)
{
  *this = Add(bcd200(p_value));
  return *this;
}

bcd200&
bcd200::operator+=(LPCTSTR p_value)
{
  *this = Add(bcd200(p_value));
  return *this;
}

// bcd200::operator -=
// Description: Operator to subtract a bcd200 from this one
bcd200&
bcd200::operator-=(const bcd200& p_value)
{
  *this = Sub(p_value);
  return *this;
}

bcd200& 
bcd200::operator-=(const int p_value)
{
  *this = Sub(bcd200(p_value));
  return *this;
}

bcd200&
bcd200::operator-=(const double p_value)
{
  *this = Sub(bcd200(p_value));
  return *this;
}

bcd200&
bcd200::operator-=(LPCTSTR p_value)
{
  *this = Sub(bcd200(p_value));
  return *this;
}

// bcd200::operator *=
// Description: Operator to multiply a bcd200 with this one
bcd200& 
bcd200::operator*=(const bcd200& p_value)
{
  *this = Mul(p_value);
  return *this;
}

bcd200&
bcd200::operator*=(const int p_value)
{
  *this = Mul(bcd200(p_value));
  return *this;
}

bcd200&
bcd200::operator*=(const double p_value)
{
  *this = Mul(bcd200(p_value));
  return *this;
}

bcd200&
bcd200::operator*=(LPCTSTR p_value)
{
  *this = Mul(bcd200(p_value));
  return *this;
}

// bcd200::operator /=
// Description: Operator to divide a bcd200 with another
bcd200& 
bcd200::operator/=(const bcd200& p_value)
{
  *this = Div(p_value);
  return *this;
}

bcd200&
bcd200::operator/=(const int p_value)
{
  *this = Div(bcd200(p_value));
  return *this;
}

bcd200&
bcd200::operator/=(const double p_value)
{
  *this = Div(bcd200(p_value));
  return *this;
}

bcd200&
bcd200::operator/=(LPCTSTR p_value)
{
  *this = Div(bcd200(p_value));
  return *this;
}

// bcd200::operator %=
// Description: Operator to do a modulo on this one
bcd200& 
bcd200::operator%=(const bcd200& p_value)
{
  *this = Mod(p_value);
  return *this;
}

bcd200&
bcd200::operator%=(const int p_value)
{
  *this = Mod(bcd200(p_value));
  return *this;
}

bcd200&
bcd200::operator%=(const double p_value)
{
  *this = Mod(bcd200(p_value));
  return *this;
}

bcd200&
bcd200::operator%=(LPCTSTR p_value)
{
  *this = Mod(bcd200(p_value));
  return *this;
}

// bd::-
// Description: prefix unary minus (negation)
//
bcd200  
bcd200::operator-() const
{
  bcd200 result(*this);

  // Null can never be negative
  if(!result.IsZero() && result.IsValid() && !result.IsNULL())
  {
    // Swap signs
    if(result.m_sign == Sign::Positive)
    {
      result.m_sign = Sign::Negative;
    }
    else
    {
      result.m_sign = Sign::Positive;
    }
  }
  return result;
}

// bcd200::postfix ++
//
bcd200 
bcd200::operator++(int)
{
  // Return result first, than do the add 1
  bcd200 res(*this);
  ++*this;
  return res;
}

// bcd200::prefix++
bcd200& 
bcd200::operator++()
{
  //++x is equal to x+=1
  bcd200 number_1(1);
  *this += number_1;
  return *this;
}

// bcd200::Postfix decrement
//
bcd200 
bcd200::operator--(int)
{
  // Return result first, than do the subtract
  bcd200 res(*this);
  --*this;
  return res;
}

// bcd200::Prefix  decrement
bcd200& 
bcd200::operator--()
{
  // --x is equal to x-=1
  bcd200 number_1(1);
  *this -= number_1;
  return *this;
}

// bcd200::=
// Description: Assignment operator from another bcd200
bcd200& 
bcd200::operator=(const bcd200& p_value)
{
  if(this != &p_value)
  {
    m_sign      = p_value.m_sign;
    m_exponent  = p_value.m_exponent;
    memcpy(m_mantissa,p_value.m_mantissa,bcdLength * sizeof(long));
  }
  return *this;
}

// bcd200::=
// Description: Assignment operator from a long
bcd200& 
bcd200::operator=(const int p_value)
{
  SetValueLong(p_value,0);
  return *this;
}

// bcd200::=
// Description: Assignment operator from a double
bcd200& 
bcd200::operator=(const double p_value)
{
  SetValueDouble(p_value);
  return *this;
}

// bcd200::=
// Description: Assignment operator from a string
bcd200& 
bcd200::operator=(const PCTSTR p_value)
{
  SetValueString(p_value);
  return *this;
}

// bcd200::=
// Description: Assignment operator from an __int64
bcd200&
bcd200::operator=(const __int64 p_value)
{
  SetValueInt64(p_value,0);
  return *this;
}

// bcd200::operator==
// Description: Equality comparison of two bcd200 numbers
//
bool 
bcd200::operator==(const bcd200& p_value) const
{
  // Shortcut: the same number is equal to itself
  if(this == &p_value)
  {
    return true;
  }
  // Signs must be equal
  if(m_sign != p_value.m_sign)
  {
    return false;
  }
  // Exponents must be equal
  if(m_exponent != p_value.m_exponent)
  {
    return false;
  }
  // Mantissa's must be equal
  for(int ind = 0;ind < bcdLength; ++ind)
  {
    if(m_mantissa[ind] != p_value.m_mantissa[ind])
    {
      return false;
    }
  }
  // Everything is equal
  return true;
}

bool
bcd200::operator==(const int p_value) const
{
  bcd200 value(p_value);
  return *this == value;
}

bool
bcd200::operator==(const double p_value) const
{
  bcd200 value(p_value);
  return *this == value;
}

bool
bcd200::operator==(LPCTSTR p_value) const
{
  bcd200 value(p_value);
  return *this == value;
}

// bcd200::operator!=
// Description: Inequality comparison of two bcd200 numbers
//
bool 
bcd200::operator!=(const bcd200& p_value) const
{
  // (x != y) is equal to !(x == y)
  return !(*this == p_value);
}

bool
bcd200::operator!=(const int p_value) const
{
  bcd200 value(p_value);
  return !(*this == value);
}

bool
bcd200::operator!=(const double p_value) const
{
  bcd200 value(p_value);
  return !(*this == value);
}

bool
bcd200::operator!=(LPCTSTR p_value) const
{
  bcd200 value(p_value);
  return !(*this == value);
}

bool
bcd200::operator<(const bcd200& p_value) const
{
  // Check if we can do a comparison
  // Infinity compares to nothing!!
  if(!IsValid() || !p_value.IsValid() || IsNULL() || p_value.IsNULL())
  {
    return false;
  }

  // Shortcut: Negative numbers are smaller than positive ones
  if(m_sign != p_value.m_sign)
  {
    // Signs are not equal 
    // If this one is negative "smaller than" is true
    // If this one is positive "smaller than" is false
    return (m_sign == Sign::Negative);
  }

  // Issue #2 at github
  // Zero is always smaller than everything else
  if(IsZero() && !p_value.IsZero())
  {
    return (m_sign == Sign::Positive);
  }

  // Shortcut: If the exponent differ, the mantissa's don't matter
  if(m_exponent != p_value.m_exponent)
  {
    if(m_exponent < p_value.m_exponent)
    {
      return (m_sign == Sign::Positive);
    }
    else
    {
      return (m_sign == Sign::Negative);
    }
  }

  // Signs are the same and exponents are the same
  // Now compare the mantissa
  for(int ind = 0;ind < bcdLength; ++ind)
  {
    // Find the first position not equal to the other
    if(m_mantissa[ind] != p_value.m_mantissa[ind])
    {
      // Result by comparing the mantissa positions
      return (m_mantissa[ind] < p_value.m_mantissa[ind]);
    }
  }
  // Numbers are exactly the same
  return false;
}

bool
bcd200::operator<(const int p_value) const
{
  bcd200 value(p_value);
  return *this < value;
}

bool
bcd200::operator<(const double p_value) const
{
  bcd200 value(p_value);
  return *this < value;
}

bool
bcd200::operator<(LPCTSTR p_value) const
{
  bcd200 value(p_value);
  return *this < value;
}

bool
bcd200::operator>(const bcd200& p_value) const
{
  // Check if we can do a comparison
  // Infinity compares to nothing!!
  if(!IsValid() || !p_value.IsValid() || IsNULL() || p_value.IsNULL())
  {
    return false;
  }

  // Shortcut: Negative numbers are smaller than positive ones
  if(m_sign != p_value.m_sign)
  {
    // Signs are not equal. 
    // If this one is positive "greater than" is true
    // If this one is negative "greater than" is false
    return (m_sign == Sign::Positive);
  }
  // Shortcut: if value is zero
  if(IsZero())
  {
    return (p_value.m_sign == Sign::Negative);
  }
  // Shortcut: If the exponent differ, the mantissa's don't matter
  if(m_exponent != p_value.m_exponent)
  {
    if(m_exponent > p_value.m_exponent || p_value.IsZero())
    {
      return (m_sign == Sign::Positive);
    }
    else
    {
      return (m_sign == Sign::Negative);
    }
  }
  // Signs are the same and exponents are the same
  // Now compare the mantissa
  for(int ind = 0;ind < bcdLength; ++ind)
  {
    // Find the first position not equal to the other
    if(m_mantissa[ind] != p_value.m_mantissa[ind])
    {
      // Result by comparing the mantissa positions
      return (m_mantissa[ind] > p_value.m_mantissa[ind]);
    }
  }
  // Numbers are exactly the same
  return false;
}

bool
bcd200::operator>(const int p_value) const
{
  bcd200 value(p_value);
  return *this > value;
}

bool
bcd200::operator>(const double p_value) const
{
  bcd200 value(p_value);
  return *this > value;
}

bool
bcd200::operator>(LPCTSTR p_value) const
{
  bcd200 value(p_value);
  return *this > value;
}

bool
bcd200::operator<=(const bcd200& p_value) const
{
  // (x <= y) equals !(x > y)
  return !(*this > p_value);
}

bool
bcd200::operator<=(const int p_value) const
{
  bcd200 value(p_value);
  return !(*this > value);
}

bool
bcd200::operator<=(const double p_value) const
{
  bcd200 value(p_value);
  return !(*this > value);
}

bool
bcd200::operator<=(LPCTSTR p_value) const
{
  bcd200 value(p_value);
  return !(*this > value);
}

bool
bcd200::operator>=(const bcd200& p_value) const
{
  // (x >= y) equals !(x < y)
  return !(*this < p_value);
}

bool
bcd200::operator>=(const int p_value) const
{
  bcd200 value(p_value);
  return !(*this < value);
}

bool
bcd200::operator>=(const double p_value) const
{
  bcd200 value(p_value);
  return !(*this < value);
}

bool
bcd200::operator>=(LPCTSTR p_value) const
{
  bcd200 value(p_value);
  return !(*this < value);
}

//////////////////////////////////////////////////////////////////////////
//
// END OF OPERATORS OF BCD
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// MAKING AN EXACT NUMERIC
//
//////////////////////////////////////////////////////////////////////////

// bcd200::Zero
// Description: Make empty
// Technical:   Set the mantissa/exponent/sign to the number zero (0)

void
bcd200::Zero()
{
  m_sign = Sign::Positive;
  m_exponent = 0;
  memset(m_mantissa,0,bcdLength * sizeof(long));
}

// Set to database NULL
void
bcd200::SetNULL()
{
  Zero();
  m_sign = Sign::ISNULL;
}

// Round to a specified fraction (decimals behind the .)
void     
bcd200::Round(int p_precision /*=0*/)
{
  // Check if we can do a round
  if(!IsValid() || IsNULL())
  {
    return;
  }

  // Precision is dependent on the exponent
  int precision = p_precision + m_exponent;

  // Quick optimization
  if(precision < 0)
  {
    Zero();
    return;
  }
  if(precision > bcdPrecision)
  {
    // Nothing to be done
    return;
  }

  // Walk the mantissa
  int mant = precision / bcdDigits;
  int pos  = precision % bcdDigits;

  int mm = m_mantissa[mant];

  if(pos == (bcdDigits - 1))
  {
    // Last position in the mantissa part
    if(mant == (bcdLength - 1))
    {
      // Nothing to do. No rounding possible
      return;
    }
    else
    {
      int mn = m_mantissa[mant + 1];
      int digitNext  = mn / (bcdBase / 10);

      if(digitNext >= 5)
      {
        // The rounding in optimized form
        m_mantissa[mant] = ++mm;
      }
    }
  }
  else
  {
    // In-between Optimalization
    int base = bcdBase;
    // Calculate base
    for(int p2 = 0;p2 <= pos; ++p2)
    {
      base /= 10;
    }
    // Rounding this digit
    int mantBefore = mm / base;
    // Next rounding digit
    int digitNext = mm % base;
    digitNext /= (base / 10);

    // Rounding
    if(digitNext >= 5)
    {
      // Calculate new mantissa
      ++mantBefore;
      int newMant = mantBefore * base;
      if(pos >= 0)
      {
        m_mantissa[mant] = newMant;
      }
    }
    // Strip everything in the mantissa behind the position
    m_mantissa[mant] /= base;
    m_mantissa[mant] *= base;
  }

  // Strip the higher mantissa's
  for(int m1 = mant + 1;m1 < bcdLength; ++m1)
  {
    m_mantissa[m1] = 0;
  }
  Normalize();
}

// Truncate to a specified fraction (decimals behind the .)
void
bcd200::Truncate(int p_precision /*=0*/)
{
  // Check if we can do a truncate
  if(!IsValid() || IsNULL())
  {
    return;
  }

  // Precision is dependent on the exponent
  int precision = p_precision + m_exponent;

  // Quick optimization
  if(precision <= 0)
  {
    // Number totally truncated
    Zero();
    return;
  }
  if(precision > bcdPrecision)
  {
    // Nothing to truncate
    return;
  }

  // Walk the mantissa
  int mant = precision / bcdDigits;
  int pos  = precision % bcdDigits;

  // Strip everything in the mantissa behind the position
  if(pos < (bcdDigits - 1))
  {
    // Calculate base
    int base = bcdBase;
    for(int p2 = 0;p2 <= pos; ++p2)
    {
      base /= 10;
    }

    m_mantissa[mant] /= base;
    m_mantissa[mant] *= base;
  }

  // Strip the higher mantissa's
  if(mant < (bcdLength - 1))
  {
    for(int m1 = mant + 1;m1 < bcdLength; ++m1)
    {
      m_mantissa[m1] = 0;
    }
  }
}

// Change the sign
void
bcd200::Negate()
{
  // Check if we can do a negation
  if(!IsValid() || IsNULL())
  {
    return;
  }

  if(IsZero())
  {
    m_sign = Sign::Positive;
  }
  else
  {
    m_sign = (m_sign == Sign::Positive) ? Sign::Negative : Sign::Positive;
  }
}

// Change length and precision
void
bcd200::SetLengthAndPrecision(int p_precision /*= bcdPrecision*/,int p_scale /*= (bcdPrecision / 2)*/)
{
  // Check if we can set these
  if(!IsValid() || IsNULL())
  {
    return;
  }

  if(IsZero())
  {
    // Optimize for NULL situation
    return;
  }

  if(m_exponent > p_precision)
  {
    XString error;
    error.Format(_T("Overflow in BCD at set precision and scale as NUMERIC(%d,%d)"),p_precision,p_scale);
    *this = SetInfinity(error);
    return;
  }

  // Calculate the mantissa position to truncate
  int mantpos = m_exponent + p_scale + 1;

  // Truncating to zero?
  if(mantpos <= 0)
  {
    m_exponent = 0;
    memset(m_mantissa,0,sizeof(long) * bcdLength);
    return;
  }
  int mant = mantpos / bcdDigits;
  int mpos = mantpos % bcdDigits;

  // Strip this on mantissa part
  if(mpos)
  {
    static const int significations[] = {1,10000000,1000000,100000,10000,1000,100,10,1};
    int  significant = significations[mpos];
    int64       accu = m_mantissa[mant] / significant;
    m_mantissa[mant] = (long) (accu * significant);
  }
  // Strip the rest of the mantissa
  for(int index = mant + 1;index < bcdLength; ++index)
  {
    m_mantissa[index] = 0;
  }
}

//////////////////////////////////////////////////////////////////////////
//
// END OF MAKING AN EXACT NUMERIC
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// MATHEMATICAL FUNCTIONS FOR BCD
//
//////////////////////////////////////////////////////////////////////////

// bcd200::Floor
// Value before the decimal point
bcd200
bcd200::Floor() const
{
  bcd200 result;
  bcd200 minusOne(-1L);

  // Check if we can do a floor
  if(IsNULL())
  {
    return bcd200(Sign::ISNULL);
  }
  if(!IsValid())
  {
    return SetInfinity(_T("BCD: Infinity doesn't have a floor"));
  }

  // Shortcut: If number is null. Floor is always zero
  if(IsZero())
  {
    return result;
  }
  // Shortcut: if number is a fraction: floor is always zero
  if(m_exponent < 0)
  {
    return m_sign == Sign::Positive ? result : minusOne;
  }
  // Shortcut: If number is too big, it's just this number
  if(m_exponent > bcdDigits * bcdLength)
  {
    return *this;
  }
  // Find the floor
  result = SplitMantissa();

  // Take care of sign
  if(m_sign == Sign::Negative)
  {
    // Floor is 1 smaller
    result -= bcd200(1L);
  }
  return result;
}

// bcd200::Fraction
// Description: Value behind the decimal point
bcd200     
bcd200::Fraction() const
{
  return (*this) - Floor();
}

// Value after the decimal point
bcd200     
bcd200::Ceiling() const
{
  bcd200 result;
  bcd200 one(1);

  // Check if we can do a ceiling
  if(IsNULL())
  {
    return bcd200(Sign::ISNULL);
  }
  if(!IsValid())
  {
    return SetInfinity(_T("BCD: Infinity does not have a ceiling."));
  }

  // Shortcut: If number is null. Ceiling is always zero
  if(IsZero())
  {
    return result;
  }
  // Shortcut: if number is a fraction: ceiling is one or zero
  if(m_exponent < 0)
  {
    return m_sign == Sign::Positive ? one : result;
  }
  // Shortcut: If number is too big, it's just this number
  if(m_exponent > bcdDigits * bcdLength)
  {
    return *this;
  }
  // Find the floor
  result = SplitMantissa();

  // Take care of sign
  if(m_sign == Sign::Positive)
  {
    // Floor is 1 smaller
    result += one;
  }
  return result;
}

// bcd200::SquareRoot
// Description: Do the square root of the bcd200
// Technical:   Do first approximation by sqrt(double)
//              Then use Newton's equation
bcd200
bcd200::SquareRoot() const
{
  bcd200 number;
  bcd200 half(_T("0.5"));
  bcd200 two(2);
  bcd200 three(3);

  // Check if we can do this
  if(IsNULL())
  {
    return bcd200(Sign::ISNULL);
  }
  if(!IsValid())
  {
    return SetInfinity(_T("BCD: Infinity does not have a square root!"));
  }

  // Optimalization sqrt(0) = 0
  if(IsZero())
  {
    return number;
  }
  // Tolerance criterion epsilon
  bcd200 epsilon = Epsilon(10);

  number = *this; // Number to get the root from
  if(number.GetSign() == -1)
  {
    return SetInfinity(_T("BCD: Cannot get a square root from a negative number."));
  }
  // Reduction by dividing through square of a whole number
  // for speed a power of two
  bcd200 reduction(1L);
  bcd200 hunderd(100L);
  while(number / (reduction * reduction) > hunderd)
  {
    reduction *= two;
  }
  // Reduce by dividing by the square of the reduction
  // (reduction is really sqrt(reduction)
  number /= (reduction * reduction);

  // First quick guess
  double approximation1 = number.AsDouble();
  double approximation2 = 1 / ::sqrt(approximation1);
  bcd200    result(approximation2);
  bcd200    between;

  // Newton's iteration
  // U(n) = U(3-VU^2)/2
  while(true)
  {
    between  = number * result * result;  // VU^2
    between  = three - between;           // 3-VU^2
    between *= half;                      // (3-VU^2)/2

    if(between.Fraction() < epsilon)
    {
      break;
    }
    result *= between;
  }
  // End result from number * 1/sqrt
  result *= number;
  // Reapply reduction by multiplying to the result
  result *= reduction;

  return result;
}

// bcd200::Power
// Description: Get BCD number to a power
// Technical:   x^y = exp(y * ln(x))
bcd200
bcd200::Power(const bcd200& p_power) const
{
  bcd200 result;

  // Check if we can do this
  if(IsNULL())
  {
    return bcd200(Sign::ISNULL);
  }
  if(!IsValid())
  {
    return SetInfinity(_T("BCD: Can not take a power of infinity!"));
  }

  result = this->Log() * p_power;
  result = result.Exp();

  return result;
}

// bcd200::AbsoluteValue
// Description: Return the absolute value
bcd200
bcd200::AbsoluteValue() const
{
  // Check if we can do this
  if(IsNULL())
  {
    return bcd200(Sign::ISNULL);
  }
  if(!IsValid())
  {
    return SetInfinity(_T("BCD: Can change the sign of infinity!"));
  }
  bcd200 result(*this);
  result.m_sign = Sign::Positive;
  return result;
}

// bcd200::Reciprocal
// Description: Reciprocal / Inverse = 1/x
bcd200     
bcd200::Reciprocal() const
{
  // Check if we can do this
  if(IsNULL())
  {
    return bcd200(Sign::ISNULL);
  }
  if(!IsValid())
  {
    return SetInfinity(_T("BCD: Can do the reciprocal of infinity!"));
  }
  bcd200 result = bcd200(1) / *this;
  return result;
}

// bcd200::Log
// Description: Natural logarithm
// Technical:   Use a Taylor series until their is no more change in the result
//              Equivalent with the same standard C function call
//              ln(x) == 2( z + z^3/3 + z^5/5 ...
//              z = (x-1)/(x+1)
bcd200     
bcd200::Log() const
{
  long k;
  long expo = 0;
  bcd200 res, number, z2;
  bcd200 number10(10L);
  bcd200 fast(_T("1.2"));
  bcd200 one(1L);
  bcd200 epsilon = Epsilon(5);

  // Check if we can do this
  if(IsNULL())
  {
    return bcd200(Sign::ISNULL);
  }
  if((GetSign() == -1) || !IsValid())
  { 
    return SetInfinity(_T("BCD: Cannot calculate a natural logarithm of a number <= 0"));
  }
  // Bring number under 10 and save exponent
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
    number = number.SquareRoot();
  }
  // Calculate the fraction part now at [1.xxx-1.1999]
  number = (number - one) / (number + one);
  z2     = number * number;
  res    = number;
  // Iterate using Taylor series ln(x) == 2( z + z^3/3 + z^5/5 ... )
  bcd200 between;
  for(long stap = 3; ;stap += 2)
  {
    number *= z2;
    between = number / bcd200(stap);
    // Tolerance criterion
    if(between.AbsoluteValue() < epsilon)
    {
      break;
    }
    res += between;
  }
  // Re-add powers of two (comes from  " < 1.2")
  res *= bcd200(::pow(2.0,(double)++k));

  // Re-apply the exponent
  if(expo != 0)
  {
    // Ln(x^y) = Ln(x) + Ln(10^y) = Ln(x) + y * ln(10)
    res += bcd200(expo) * bcd200::LN10();
  }
  return res;
}

// bcd200::exp
// Description: Exponent e tot the power 'this number'
// Technical:   Use a Taylor series until their is no more change in the result
//              exp(x) == 1 + x + x^2/2!+x^3/3!+....
//              Equivalent with the same standard C function call
//
bcd200
bcd200::Exp() const
{
  long step, k = 0;
  bcd200 between, result, number;
  bcd200 half(_T("0.5"));
  bcd200 epsilon = Epsilon(5);

  // Check if we can do this
  if(IsNULL())
  {
    return bcd200(Sign::ISNULL);
  }
  if(!IsValid())
  {
    return SetInfinity(_T("BCD: Cannot take the exponent of infinity!"));
  }
  number = *this;

  // Can not calculate: will always be one!
  if(number.IsZero())
  {
    return bcd200(1);
  }

  if(number.GetSign () < 0 )
  {
    number = -number;;
  }
  for( k = 0; number > half; )
  {
    long expo = number.GetExponent();
    if( expo > 0 )
    {
      step   = 3 * min( 10, expo );  // 2^3
      result = bcd200((long) (1 << step) );
      result = result.Reciprocal();
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
  result  = bcd200(1L) + number;
  between  = number * number * half;
  result += between;
  // Now iterate 
  for(step = 3; ;step++)
  {
    between *= number / bcd200(step);
    // Tolerance criterion
    if(between < epsilon)
    {
      break;
    }
    result += between;
  }
  // Re-add powers of myself
  for( ; k > 0; k-- )
  {
    result *= result;
  }
  // Take care of the sign
  if(this->GetSign() < 0 )
  {
    result = bcd200(1) / result;
  }
  return result;
}

// bcd200::Log10
// Description: Logarithm in base 10
// Technical:   log10 = ln(x) / ln(10);
bcd200     
bcd200::Log10() const
{
  bcd200 res;

  // Check if we can do a LOG10
  if(IsNULL())
  {
    return bcd200(Sign::ISNULL);
  }
  if(!IsValid())
  {
    return SetInfinity(_T("BCD: Cannot take the Log10 of infinity!"));
  }
  if(GetSign() <= 0) 
  { 
    return SetInfinity(_T("BCD: Cannot get a 10-logarithm of a number <= 0"));
  }
  res = *this;
  res = res.Log() / LN10();

  return res;
}

// Ten Power
// Description: bcd200 . 10^n
// Technical:   add n to the exponent of the number
bcd200
bcd200::TenPower(int n)
{
  // Check if we can do this
  if(IsNULL())
  {
    return bcd200(Sign::ISNULL);
  }
  if(!IsValid())
  {
    return SetInfinity(_T("BCD: Cannot take the 10th power of infinity!"));
  }
  bcd200 res = *this;
  res.m_exponent += (short)n;
  res.Normalize();
  
  return res;
}

//////////////////////////////////////////////////////////////////////////
//
// END OF MATHEMATICAL FUNCTIONS FOR BCD
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// BCD TRIGONOMETRIC FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

// bcd200::Sine
// Description: Sine of the angle
// Technical:   Use the Taylor series: Sin(x) = x - x^3/3! + x^5/5! ...
//              1) Reduce x to between 0..2*PI 
//              2) Reduce further until x between 0..PI by means of sin(x+PI) = -Sin(x)
//              3) Then do the Taylor expansion series
// Reduction is needed to speed up the Taylor expansion and reduce rounding errors
//
bcd200
bcd200::Sine() const
{
  int sign;
  bcd200 number;
  bcd200 pi,pi2;
  bcd200 between;
  bcd200 epsilon = Epsilon(3);

  // Check if we can do this
  if(IsNULL())
  {
    return bcd200(Sign::ISNULL);
  }
  if(!IsValid())
  {
    return SetInfinity(_T("BCD: Cannot take the sine of infinity!"));
  }

  number = *this;
  sign = number.GetSign();
  if( sign < 0 )
  {
    number = -number;
  }
  // Reduce the argument until it is between 0..2PI 
  pi2 = PI() * bcd200(2);
  if(number > pi2)
  {
    between = number / pi2; 
    between = between.Floor();
    number -= between * pi2;
  }
  if(number < bcd200(0))
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

  // Now iterate with Taylor expansion
  // Sin(x) = x - x^3/3! + x^5/5! ...
  bcd200 square = number * number;
  bcd200 result = number;
  between    = number;

  for(long step = 3; ;step += 2)
  {
    between *= square;
    between /= bcd200(step) * bcd200(step - 1);
    between  = -between; // Switch sign each step

//     // DEBUGGING
//     printf("Step: %d\n",step);
//     result.DebugPrint("Result");
//     between.DebugPrint("Between");

    result  += between;

//     // DEBUGGING
//     result.DebugPrint("Between+");

//     // DEBUGGING
//     printf("%02d = %40s = %40s\n",step,between.AsString(),result.AsString());

    // Check tolerance criterion
    if(between.AbsoluteValue() < epsilon)
    {
      break;
    }
  }
  // Reapply the right sign
  if(sign < 0)
  {
    result = -result;
  }
  return result;
}

// bcd200::Cosine
// Description: Cosine of the angle
// Technical:   Use the Taylor series. Cos(x) = 1 - x^2/2! + x^4/4! - x^6/6! ...
//              1) However first reduce x to between 0..2*PI
//              2) Then reduced it further to between 0..PI using cos(x)=Cos(2PI-x) for x >= PI
//              3) Now use the trisection identity cos(3x)=-3*cos(x)+4*cos(x)^3
//                 until argument is less than 0.5
//              4) Finally use Taylor 
//
bcd200
bcd200::Cosine() const
{
  long trisection, step;
  bcd200 between, result, number, number2;
  bcd200 c05(_T("0.5")), c1(1), c2(2), c3(3), c4(4);
  bcd200 epsilon = Epsilon(2);

  // Check if we can do this
  if(IsNULL())
  {
    return bcd200(Sign::ISNULL);
  }
  if(!IsValid())
  {
    return SetInfinity(_T("BCD: Cannot take the cosine of infinity!"));
  }

  number = *this;

  // Reduce argument to between 0..2PI
  result  = PI();
  result *= c2;
  if(number.AbsoluteValue() > result )
  {
    between = number / result; 
    between = between.Floor();
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

  // Breaking criterion, cosine close to 1 if number is close to zero
  if(number.AbsoluteValue() < epsilon)
  {
    return (result = 1);
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
    number  /= bcd200(step);
    number  /= bcd200(step-1);
    between *= number;
    between  = -between;  // r.change_sign();
    // Tolerance criterion
    if(between.AbsoluteValue() < epsilon)
    {
      break;
    }
    result += between;
  }

  // Reapply the effects of the trisection again
  for( ;trisection > 0; --trisection)
  {
    result *= ( c4 * result * result - c3 );
  }
  return result;
}

// bcd200::Tangent
// Description: Tangent of the angle
// Technical:   Use the identity tan(x)=Sin(x)/Sqrt(1-Sin(x)^2)
//              However first reduce x to between 0..2*PI
//
bcd200
bcd200::Tangent() const
{
  bcd200 result, between, number;
  bcd200 two(2), three(3);

  // Check if we can do this
  if(IsNULL())
  {
    return bcd200(Sign::ISNULL);
  }
  if(!IsValid())
  {
    return SetInfinity(_T("BCD: Cannot take the tangent of infinity!"));
  }

  number = *this;

  // Reduce argument to between 0..2PI
  bcd200 pi     = PI();
  bcd200 twopi  = two * pi;
  if(number.AbsoluteValue() > twopi )
  {
    between  = number / twopi; 
    between  = between.Floor();
    number  -= between * twopi;
  }

  if(number.GetSign() < 0)
  {
    number += twopi;
  }
  bcd200 halfpi     = pi / two;
  bcd200 oneandhalf = three * halfpi;
  if( number == halfpi || number == oneandhalf)
  { 
    return SetInfinity(_T("BCD: Cannot calculate a tangent from a angle of 1/2 pi or 3/2 pi"));
  }
  // Sin(x)/Sqrt(1-Sin(x)^2)
  result     = number.Sine(); 
  bcd200 square = result * result;
  bcd200 divide = bcd200(1) - square;
  bcd200 root   = sqrt(divide);
  result    /= root;

  // Correct for the sign
  if(number > halfpi && number < oneandhalf)
  {
    result = -result;
  }
  return result;
}

// bcd200::Asine
// Description: ArcSine (angle) of the ratio
// Technical:   Use Newton by solving the equation Sin(y)=x. Then y is Arcsine(x)
//              Iterate by Newton y'=y-(sin(y)-x)/cos(y). 
//              With initial guess using standard double precision arithmetic.
// 
bcd200     
bcd200::ArcSine() const
{
  long step, reduction, sign;
  double d;
  bcd200 between, number, result, factor;
  bcd200 c1(1);
  bcd200 c2(2);
  bcd200 c05(_T("0.5"));
  bcd200 epsilon = Epsilon(5);

  // Check if we can do this
  if(IsNULL())
  {
    return bcd200(Sign::ISNULL);
  }
  if(!IsValid())
  {
    return SetInfinity(_T("BCD: Cannot take the arcsine of infinity!"));
  }

  number = *this;
  if(number > c1 || number < -c1)
  {
    return SetInfinity(_T("BCD: Cannot calculate an arcsine from a number > 1 or < -1"));
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
  // Quick approximation of the asin
  d = ::asin(number.AsDouble());
  result = bcd200( d );
  factor = bcd200( 1.0 / ::cos(d)); // Constant factor 

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

  // Repair the reduction in the result
  result *= bcd200((long) (1 << reduction) );

  // Take care of sign
  if( sign < 0 )
  {
    result = -result;
  }

  // this is the result
  return result;
}

// bcd200::ArcCosine
// Description: ArcCosine (angle) of the ratio
// Technical:   Use ArcCosine(x) = PI/2 - ArcSine(x)
bcd200     
bcd200::ArcCosine() const
{
  bcd200 y;

  // Check if we can do this
  if(IsNULL())
  {
    return bcd200(Sign::ISNULL);
  }
  if(!IsValid())
  {
    return SetInfinity(_T("BCD: Cannot take the arc-cosine of infinity!"));
  }

  y  = PI();
  y /= bcd200(2L);
  y -= ArcSine();

  return y;
}

// bcd200::ArcTangent
// Description: ArcTangent (angle) of the ratio
// Technical:   Use the Taylor series. ArcTan(x) = x - x^3/3 + x^5/5 ...
//              However first reduce x to abs(x)< 0.5 to improve taylor series
//              using the identity. ArcTan(x)=2*ArcTan(x/(1+sqrt(1+x^2)))
//
bcd200
bcd200::ArcTangent() const
{
  bcd200  result, square;
  bcd200  between1,between2;
  bcd200  half(_T("0.5"));
  bcd200  one(1);
  bcd200  epsilon = Epsilon(5);
  long k = 2;

  // Check if we can do this
  if(IsNULL())
  {
    return bcd200(Sign::ISNULL);
  }
  if(!IsValid())
  {
    return SetInfinity(_T("BCD: Cannot take the arc-tangent of infinity!"));
  }

  result   = *this;
  // Transform the solution to ArcTan(x)=2*ArcTan(x/(1+sqrt(1+x^2)))
  result = result / ( one + sqrt( one + (result * result)));
  if( result.AbsoluteValue() > half) // if still to big then do it again
  {
    k = 4;
    result = result / (one + sqrt(one + (result * result)));
  }
  square = result * result;
  between1  = result;
  // Now iterate using Taylor expansion
  for(long step = 3;; step += 2)
  {
    between1 *= square;
    between1  = -between1;
    between2  = between1 / bcd200(step);
    // Tolerance criterion
    if(between2.AbsoluteValue() < epsilon)
    {
      break;
    }
    result += between2;
  }

  // Reapply the reduction/transformation
  result *= bcd200(k);

  // this is the result
  return result;
}

// bcd200::Atan2
// Description: Angle of two points
// Technical:   return the angle (in radians) from the X axis to a point (y,x).
//              use atan() to calculate atan2()
//
bcd200
bcd200::ArcTangent2Points(const bcd200& p_x) const
{
  bcd200 result;
  bcd200 number = *this;
  bcd200 nul, c05(_T("0.5"));

  // Check if we can do this
  if(IsNULL())
  {
    return bcd200(Sign::ISNULL);
  }
  if(!IsValid() || !p_x.IsValid())
  {
    return SetInfinity(_T("BCD: Cannot take the arc-tangent-2 of infinity!"));
  }

  if( p_x == nul && number == nul)
  {
    return nul;
  }
  if( p_x == nul )
  {
    result = PI();
    if( number < nul )
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
    if( number == nul )
    {
      if( p_x < nul )
      {
        result = PI();
      }
      else
      {
        result = nul;
      }
    }
    else
    {
      result = bcd200( number / p_x ).ArcTangent();
      if( p_x < nul  && number < nul )
      {
        result -= PI();
      }
      if( p_x < nul && number >= nul )
      {
        result += PI();
      }
    }
  }
  return result;
}

//////////////////////////////////////////////////////////////////////////
//
// END OF BCD TRIGONOMETRIC FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// GET AS SOMETHING DIFFERENT THAN THE BCD
//
//////////////////////////////////////////////////////////////////////////

// bcd200::AsDouble
// Description: Get as a double
//
double  
bcd200::AsDouble() const
{
  double result = 0.0;

  // Check if we have a result
  if(!IsValid() || IsNULL())
  {
    return result;
  }

  if(bcdDigits >= 8)
  {
    // SHORTCUT FOR PERFORMANCE: 
    // A double cannot have more than 16 digits
    // so everything over 16 digits gets discarded
    result  = ((double)m_mantissa[0]) * 10.0 / (double) bcdBase;
    result += ((double)m_mantissa[1]) * 10.0 / (double) bcdBase / (double) bcdBase;
    //result += ((double)m_mantissa[2]) / bcdBase / bcdBase / bcdBase;
  }
  else
  {
    // Works for ALL implementations of bcdDigits  and bcdLength
    // Get the mantissa into the result
    double factor = 1.0;
    for(int ind = 0; ind < bcdLength; ++ind)
    {
      long base    = bcdBase / 10;
      long between = m_mantissa[ind];

      for(int pos = bcdDigits; pos > 0; --pos)
      {
        // Get next number
        long number = between / base;
        between %= base;
        base /= 10;

        // Set in the result
        result += (double)number * factor;
        factor /= 10;
      }
    }
  }

  // Take care of exponent
  if(m_exponent)
  {
    result *= ::pow(10.0,m_exponent);
  }
  // Take care of the sign
  if(m_sign == Sign::Negative)
  {
    result = -result;
  }
  return result;
}

// bcd200::AsShort
// Description: Get as a short
short   
bcd200::AsShort() const
{
  // Check if we have a result
  if(!IsValid() || IsNULL())
  {
    return 0;
  }

  // Quick check for zero
  if(m_exponent < 0)
  {
    return 0;
  }
  int exponent = bcdDigits - m_exponent - 1;

  // Get from the mantissa
  int result = m_mantissa[0];

  // Adjust to exponent
  while(exponent--)
  {
    result /= 10;
  }

  // Take care of sign and over/under flows
  if(m_sign == Sign::Positive)
  {
    if(result > SHORT_MAX)
    {
      throw StdException(_T("BCD: Overflow in conversion to short number."));
    }
  }
  else
  {
    if(result < SHORT_MIN)
    {
      throw StdException(_T("BCD: Underflow in conversion to short number."));
    }
    result = -result;
  }
  return (short) result;
}

// bcd200::AsUShort
// Description: Get as an unsigned short
ushort  
bcd200::AsUShort() const
{
  // Check if we have a result
  if(!IsValid() || IsNULL())
  {
    return 0;
  }

  // Check for unsigned
  if(m_sign == Sign::Negative)
  {
    throw StdException(_T("BCD: Cannot convert a negative number to an unsigned short number."));
  }
  // Quick check for zero
  if(m_exponent < 0)
  {
    return 0;
  }

  // Get from the mantissa
  int result = m_mantissa[0];

  // Adjust to exponent
  int exponent = bcdDigits - m_exponent - 1;
  while(exponent--)
  {
    result /= 10;
  }

  // Take care of overflow
  if(result > USHORT_MAX)
  {
    throw StdException(_T("BCD: Overflow in conversion to unsigned short number."));
  }

  return (short)result;
}

// bcd200::AsLong
// Description: Get as a long
long    
bcd200::AsLong() const
{
  // Check if we have a result
  if(!IsValid() || IsNULL())
  {
    return 0;
  }

  // Quick optimization for really small numbers
  if(m_exponent < 0)
  {
    return 0L;
  }

  // Get from the mantissa
  int64 result = ((int64)m_mantissa[0] * bcdBase) + ((int64)m_mantissa[1]);

  // Adjust to exponent
  int exponent = 2 * bcdDigits - m_exponent - 1;
  while(exponent--)
  {
    result /= 10;
  }

  // Take care of sign and over/under flows
  if(m_sign == Sign::Positive)
  {
    if(result > LONG_MAX)
    {
      throw StdException(_T("BCD: Overflow in conversion to integer number."));
    }
  }
  else
  {
    if(result < LONG_MIN)
    {
      throw StdException(_T("BCD: Underflow in conversion to integer number."));
    }
    result = -result;
  }
  return (long) result;
}

// bcd200::AsULong
// Get as an unsigned long
ulong   
bcd200::AsULong() const
{
  // Check if we have a result
  if(!IsValid() || IsNULL())
  {
    return 0;
  }

  // Check for unsigned
  if(m_sign == Sign::Negative)
  {
    throw StdException(_T("BCD: Cannot convert a negative number to an unsigned long."));
  }

  // Quick optimization for really small numbers
  if(m_exponent < 0)
  {
    return 0L;
  }

  // Get from the mantissa
  int64 result = ((int64)m_mantissa[0] * bcdBase) + ((int64)m_mantissa[1]);

  // Adjust to exponent
  int exponent = 2 * bcdDigits - m_exponent - 1;
  while(exponent--)
  {
    result /= 10;
  }

  // Take care of overflow
  if(result > ULONG_MAX)
  {
    throw StdException(_T("BCD: Overflow in conversion to unsigned long integer."));
  }
  return (long)result;
}

// bcd200::AsInt64
// Description: Get as a 64 bits long number
//
int64
bcd200::AsInt64() const
{
  // Check if we have a result
  if(!IsValid() || IsNULL())
  {
    return 0;
  }

  // Quick optimization for really small numbers
  if(m_exponent < 0)
  {
    return 0L;
  }
  int64 result1 = 0L;
  int64 result2 = 0L;
  int exponent  = 4 * bcdDigits - m_exponent - 1;
  int64 base2   = (int64)bcdBase * ((int64)bcdBase);
  int64 base    = base2 / 10;

  // Get from the mantissa
  result1 = ((int64)m_mantissa[0] * bcdBase) + ((int64)m_mantissa[1]);
  result2 = ((int64)m_mantissa[2] * bcdBase) + ((int64)m_mantissa[3]);

  // Adjust to exponent
  while(exponent--)
  {
    int64 carry = result1 %10;
    result1 /= 10;
    result2 /= 10;
    result2 += carry * base;
  }

  // Take care of overflow
  if(result1 > (LLONG_MAX / base2))
  {
    throw StdException(_T("BCD: Overflow in conversion to 64 bits integer number."));
  }
  result2 += (result1 * base2);

  // Take care of sign 
  if(m_sign == Sign::Negative)
  {
    result2 = -result2;
  }
  return result2;
}

// Get as an unsigned 64 bits long
uint64  
bcd200::AsUInt64() const
{
  // Check if we have a result
  if(!IsValid() || IsNULL())
  {
    return 0;
  }

  // Check for negative
  if(m_sign == Sign::Negative)
  {
    throw StdException(_T("BCD: Cannot convert a negative number to an unsigned 64 bits integer"));
  }
  // Quick optimization for really small numbers
  if(m_exponent < 0)
  {
    return 0L;
  }
  uint64 result1 = 0L;
  uint64 result2 = 0L;
  int exponent   = 4 * bcdDigits - m_exponent - 1;
  uint64 base2   = (uint64)bcdBase * ((uint64)bcdBase);
  uint64 base    = base2 / 10;

  // Get from the mantissa
  result1 = ((uint64)m_mantissa[0] * bcdBase) + ((uint64)m_mantissa[1]);
  result2 = ((uint64)m_mantissa[2] * bcdBase) + ((uint64)m_mantissa[3]);

  // Adjust to exponent
  while(exponent--)
  {
    uint64 carry    = result1 %10;
    result1 /= 10;
    result2 /= 10;
    result2 += carry * base;
  }

  // Take care of overflow
  if(result1 > (ULLONG_MAX / base2))
  {
    throw StdException(_T("BCD: Overflow in conversion to 64 bits unsigned integer number."));
  }
  result2 += (result1 * base2);

  return result2;
}

// bcd200::AsString
// Description: Get as a mathematical string
//
// Engineering format: Convert back to "[sign][digit][.[digit]*][E[sign][digits]+]"
// Bookkeeping format: Convert back to "[sign]9[digit]*][.[digit]*]"
// Optionally also print the positive '+' ('-' negative sign is always printed!)
// Optionally get with fixed decimals, default = 2 decimals (most common default for bookkeeping purposes)
// Optionally get as much as needed decimals with "p_decimals = 0"
XString 
bcd200::AsString(Format p_format /*=Bookkeeping*/,bool p_printPositive /*=false*/,int p_decimals /*=2*/) const
{
  XString result;
  int expo   = m_exponent;
  int prec   = bcdDigits * bcdLength;

  // Shortcut for infinity and not-a-number
  switch(m_sign)
  {
    case Sign::NaN:     return  _T("NaN");
    case Sign::INF:     return  _T("INF");
    case Sign::MIN_INF: return _T("-INF");
    case Sign::ISNULL:  return _T("NULL");
  }

  // Check format possibilities
  if(expo < -(prec/2) || expo > (prec/2))
  {
    p_format = Format::Engineering;
  }

  // Construct the mantissa string
  for(int mantpos = 0; mantpos < bcdLength; ++mantpos)
  {
    long number = m_mantissa[mantpos];
    long base   = bcdBase / 10;
    for(int digitpos = bcdDigits; digitpos > 0; --digitpos)
    {
      long num = number / base;
      number   = number % base;
      TCHAR c  = (TCHAR)num + '0';
      base    /= 10;

      result += c;
    }
  }
  // Stripping trailing zeros.
  result = result.TrimRight('0');

  if(p_format == Format::Engineering)
  {
    XString left = result.Left(1);
    result = left + XString(_T(".")) + result.Mid(1) + XString(_T("E"));
    result += LongToString(expo);
  }
  else // Bookkeeping
  {
    if(m_exponent < 0)
    {
      XString left(_T("0."));
      for(int ind = -1; ind > m_exponent; --ind)
      {
        left += _T("0");
      }
      result = left + result;
    }
    if(m_exponent >= 0)
    {
      int pos = 1 + m_exponent;
      XString before = result.Left(pos);
      XString behind = result.Mid(pos);
      while(before.GetLength() < pos)
      {
        before += _T("0");
      }
      result = before;
      while(p_decimals > 0 && behind.GetLength() < p_decimals)
      {
        behind += '0';
      }
      if(!behind.IsEmpty())
      {
        result += XString(_T(".")) + behind;
      }
    }
  }

  // Take care of the sign
  // result = ((m_sign == Positive ) ? "+" : "-") + result;
  if(m_sign == Sign::Positive)
  {
    if(p_printPositive)
    {
      result = _T("+") + result;
    }
  }
  else
  {
    result = _T("-") + result;
  }

  // Ready
  return result;
}

// Display strings are always in Format::Bookkeeping
// as most users find mathematical exponential notation hard to read.
XString 
bcd200::AsDisplayString(int p_decimals /*=2*/) const
{
  // Shortcut for infinity and not-a-number
  switch(m_sign)
  {
    case Sign::NaN:     return  _T("NaN");
    case Sign::INF:     return  _T("INF");
    case Sign::MIN_INF: return _T("-INF");
    case Sign::ISNULL:  return _T("NULL");
  }

  // Initialize locale strings
  InitValutaString200();

  // Not in the bookkeeping range
  if(m_exponent > 12 || m_exponent < -2)
  {
    return AsString(bcd200::Format::Engineering,false,p_decimals);
  }
  bcd200 number(*this);
  number.Round(p_decimals);

  XString str = number.AsString(Format::Bookkeeping,false,p_decimals);
  int pos = str.Find('.');
  if(pos >= 0)
  {
    str.Replace(_T("."),g_locale_decimalSep);
  }

  // Apply thousand separators in first part of the number
  if((pos > 0) || (pos == -1 && str.GetLength() > 3))
  {
    XString result = pos > 0 ? str.Mid(pos) : XString();
    str = pos > 0 ? str.Left(pos) : str;
    pos = result.GetLength();

    while(str.GetLength() > 3)
    {
      result = XString(g_locale_thousandSep) + str.Right(3) + result;
      str = str.Left(str.GetLength() - 3);
    }
    str += result;
  }
  
  // Extra zero's for the decimals?
  if((pos <= 0 && p_decimals > 0) || (0 < pos && pos <= p_decimals))
  {
    // Round on this number of decimals
    int decimals(p_decimals);
    if(pos < 0)
    {
      str += g_locale_decimalSep;
    }
    else
    {
      decimals = p_decimals - pos + 1;
    }
    for(int index = 0;index < decimals; ++index)
    {
      str += _T("0");
    }
  }
  return str;
}

// Get as an ODBC SQL NUMERIC
void
bcd200::AsNumeric(SQL_NUMERIC_STRUCT* p_numeric) const
{
  // Init the value array
  memset(p_numeric->val,0,SQL_MAX_NUMERIC_LEN);

  // Check if we have a result
  if(!IsValid() || IsNULL())
  {
    return;
  }

  // Special case for 0.0 or smaller than can be contained (1.0E-38)
  if(IsZero() || m_exponent < -SQLNUM_MAX_PREC)
  {
    return;
  }

  // Check for overflow. Cannot be greater than 9.999999999E+37
  if(m_exponent >= SQLNUM_MAX_PREC)
  {
    throw StdException(_T("BCD: Overflow in converting bcd200 to SQL NUMERIC/DECIMAL"));
  }

  SQLCHAR precision = 0;
  SQLCHAR scale = 0;
  CalculatePrecisionAndScale(precision,scale);

  // Setting the sign, precision and scale
  p_numeric->sign      = (m_sign == Sign::Positive) ? 1 : 0;
  p_numeric->precision = precision;
  p_numeric->scale     = scale;

  // Converting the value array
  bcd200 one(1);
  bcd200 radix(256);
  bcd200 accu(*this);
  int index = 0;

  // Here is the big trick: use the exponent to scale up the number
  // Adjusting m_exponent to positive scaled integer result
  accu.m_exponent += (short)scale;
  accu.m_sign      = Sign::Positive;

  while(true)
  {
    // Getting the next val array value, relying on the bcd200::modulo
    bcd200 val = accu.Mod(radix);
    p_numeric->val[index++] = (SQLCHAR) val.AsLong();

    // Adjust the intermediate accu
    accu -= val;
    accu  = accu.Div(radix);

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
}

//////////////////////////////////////////////////////////////////////////
//
// END OF "GET AS SOMETHING DIFFERENT"
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// GETTERS of BCD
//
//////////////////////////////////////////////////////////////////////////

// bcd200::IsNull
// Description: Gets the fact that bcd200 is exactly 0.0
bool  
bcd200::IsZero() const
{
  // Shortcut test
  if(m_sign == Sign::Negative || m_exponent != 0)
  {
    return false;
  }
  // The normalized mantissa's first part must be a number
  // otherwise the total of the bcd200 is NULL!!
  // So: No need to scan the whole mantissa!!
  if(m_mantissa[0])
  {
    // Also works for (-)INF, NaN and NULL
    return false;
  }
  return true;
}

// Is bcd200 a database NULL
bool
bcd200::IsNULL() const
{
  return m_sign == Sign::ISNULL;
}

// bcd200::IsNearZero
// Description: Nearly zero or zero
bool
bcd200::IsNearZero()
{
  // NULL, NaN or (-)INF is never 'near zero'
  if(m_sign > Sign::Negative)
  {
    return false;
  }

  // [200]
  // BEWARE: bcd200 -> Epsilon is two times higher!!
  bcd200 epsilon = Epsilon(10);
  return AbsoluteValue() < epsilon;
}

// Not an (-)INF or a NaN
bool
bcd200::IsValid() const
{
  return m_sign <= Sign::Negative;
}

// bcd200::GetSign
// Description: Gets the sign
// Technical:   Returns -1 (negative), 0 or 1 (Positive)
// Beware;      NaN, (-)INF also returns a 0
int   
bcd200::GetSign() const
{
  // Negative number returns -1
  if(m_sign == Sign::Negative)
  {
    return -1;
  }
  // Positive number returns 1
  if(m_mantissa[0] || m_exponent)
  {
    return 1;
  }
  // Number is NULL. Sign = 0, or (-)INF or NaN
  return 0;
}

// Gets Signed status Positive, Negative, -INF, INF, NaN
bcd200::Sign
bcd200::GetStatus() const
{
  return m_sign;
}

// bcd200::GetLength
// Description: Total length (before and after decimal point)
// Technical:   Returns the actual length (not the precision)
int   
bcd200::GetLength() const
{
  int length  = 0;
  int counter = 0;

  // Length of the display string
  switch(m_sign)
  {
    case Sign::NaN:     return 3;
    case Sign::INF:     return 3;
    case Sign::MIN_INF: return 4;
    case Sign::ISNULL:  return 4;
  }

  // Quick optimization
  if(IsZero())
  {
    // Zero (0) has length of 1
    return 1;
  }

  // Walk the mantissa
  for(int pos = 0;pos < bcdLength; ++pos)
  {
    long number = m_mantissa[pos];
    long base   = bcdBase / 10;

    for(int dig = bcdDigits; dig > 0; --dig)
    {
      // Actual position
      ++counter;

      // See if there is a digit here
      long num = number / base;
      number  %= base;
      base    /= 10;
      if(num)
      {
        length = counter;
      }
    }
  }
  return length;
}

// bcd200::GetPrecision
// Description: Total precision (length after the decimal point)
int   
bcd200::GetPrecision() const
{
  // Quick optimization
  if(IsZero() || !IsValid() || IsNULL())
  {
    return 0;
  }

  // Default max precision
  int precision = bcdPrecision;

  for(int posLength = bcdLength - 1; posLength >= 0; --posLength)
  {
    int mant = m_mantissa[posLength];
    for(int posDigits =  bcdDigits - 1;posDigits >= 0; --posDigits)
    {
      int last = mant % 10;
      mant /= 10;       // Next pos
      if(last == 0)
      {
        --precision;
      }
      else
      {
        // Break on first non-zero 
        // by terminating both loops
        posDigits = -1;
        posLength = -1;
      }
    }
  }
  // Precision is 1 based (1 position for the decimal point implied)
  precision -= 1;
  // Precision is inverse to the exponent (higher exponent, means less behind the decimal point)
  precision -= m_exponent;
  // Precision cannot be less than zero
  if(precision < 0)
  {
    precision = 0;
  }
  // Return the result
  return precision;
}

// bcd200::GetMaxSize
// Description: Get the max size of a bcd200
int 
bcd200::GetMaxSize(int /* precision /*= 0*/)
{
  // int size = bcdDigits * bcdLength;
  return bcdPrecision;
}

// bcd200::GetFitsInLong
// Gets the fact that it fits in a long
bool  
bcd200::GetFitsInLong() const
{
  // Infinity does not fit in a long :-)
  if(!IsValid() || IsNULL())
  {
    return false;
  }

  try
  {
    AsLong();
  }
  catch(StdException& ex)
  {
    ReThrowSafeException(ex);
    return false;
  }
  return true;
}

// bcd200::GetFitsInInt64
// Description: Gets the fact that it fits in an int64
//
bool  
bcd200::GetFitsInInt64() const
{
  // Infinity does not fit in an int64 :-)
  if(!IsValid() || IsNULL())
  {
    return false;
  }

  try
  {
    AsInt64();
  }
  catch(StdException& ex)
  {
    ReThrowSafeException(ex);
    return false;
  }
  return true;
}

// bcd200::GetHasDecimals
// Description: Decimal part (behind the decimal point) is not "000" (zeros)
bool  
bcd200::GetHasDecimals() const
{
  // Shortcut for ZERO
  if(IsZero() || !IsValid() || IsNULL())
  {
    return false;
  }
  // Shortcut: some decimals must exist
  if(m_exponent < 0)
  {
    return true;
  }

  // Compare the length with the exponent.
  // If exponent smaller, the rest is behind the decimal point
  int length = GetLength();
  if(m_exponent < (length - 1))
  {
    return true;
  }
  return false;
}

// bd::GetExponent
// Description: Gets the 10-based exponent
int   
bcd200::GetExponent() const
{
  // Infinity has no exponent
  if(!IsValid() || IsNULL())
  {
    return 0;
  }
  return m_exponent;
}

// bcd200::GetMantissa
// Description: Gets the mantissa
bcd200   
bcd200::GetMantissa() const
{
  if(!IsValid() || IsNULL())
  {
    return SetInfinity(_T("BCD: Infinity cannot give a mantissa."));
  }

  bcd200 number(*this);

  number.m_sign     = Sign::Positive;
  number.m_exponent = 0;
  return number;
}

//////////////////////////////////////////////////////////////////////////
//
// END OF GETTERS OF BCD
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// INTERNALS OF BCD
//
//////////////////////////////////////////////////////////////////////////

// Take the absolute value of a long
// This method is taken outside the <math> library or other macro's.
// It was needed because some versions of std linked wrongly
long 
bcd200::long_abs(const long p_value) const
{
  if(p_value < 0)
  {
    return -p_value;
  }
  return p_value;
}

// bcd200::SetValueInt
// Description: Sets one integer in this bcd200 number
void  
bcd200::SetValueInt(const int p_value)
{
  Zero();

  // Shortcut if value is zero
  if(p_value == 0)
  {
    return;
  }
  // Take care of sign
  m_sign = (p_value < 0) ? Sign::Negative : Sign::Positive;
  // Place in mantissa
  m_mantissa[0] = long_abs(p_value);
  // And normalize
  Normalize(bcdDigits - 1);
}

// bcd200::SetValueLong
// Description: Set the bcd200 to the value of two longs
//              Numbers can be set as bcd200(12345,4567) -> +1.23454567E+4 = 12,345.4567
//              Works up to 8 positions before and after the decimal point
// Parameters:  const long value     // value before the decimal point
//              const long restValue // Optional value behind the decimal point
//
void
bcd200::SetValueLong(const long p_value, const long p_restValue)
{
  Zero();

  if(p_value == 0 && p_restValue == 0)
  {
    // Nothing more to do. We are zero
    return;
  }
  // Get the sign
  if (p_value == 0)
  {
    m_sign = (p_restValue < 0) ? Sign::Negative : Sign::Positive;
  }
  else
  {
    m_sign = (p_value < 0) ? Sign::Negative : Sign::Positive;
  }
  // Fill in mantissa. restValue first
  int norm = 0;

  if(p_restValue)
  {
    m_mantissa[0] = long_abs(p_restValue % bcdBase);
    norm = -1;

    if(p_restValue / bcdBase)
    {
      ShiftRight();
      m_mantissa[0] = long_abs(p_restValue / bcdBase);
      norm -= bcdDigits;
    }
    Normalize(norm);
    norm = 0;
    if(p_value)
    {
      ShiftRight();
    }
  }

  if(p_value % bcdBase)
  {
    m_mantissa[0] = long_abs(p_value % bcdBase);
    norm = bcdDigits - 1;
  }
  if(p_value / bcdBase)
  {
    ShiftRight();
    m_mantissa[0] = long_abs(p_value / bcdBase);
    norm = 2 * bcdDigits - 1;
  }
  Normalize(norm);
}

// bcd200::SetValueint64
// Description: Set the bcd200 to the value of two longs
//              Numbers can be set as bcd200(12345,4567) -> +1.23454567E+4 = 12,345.4567
//              Works up to all 19 positions before and after the decimal point
// Parameters:  const long value     // value before the decimal point
//              const long restValue // Optional value behind the decimal point
// Be advised:  The restValues behave as digits left shifted to the decimal marker
//              5      becomes 0.5
//              15     becomes 0.15
//              2376   becomes 0.2376
void  
bcd200::SetValueInt64(const int64 p_value, const int64 p_restValue)
{
  Zero();

  int64 dblBcdDigits = (int64)bcdBase * (int64)bcdBase;

  if(p_value == 0L && p_restValue == 0L)
  {
    // Nothing more to do. We are zero
    return;
  }
  // Get the sign
  if(p_value == 0L)
  {
    m_sign = (p_restValue < 0L) ? Sign::Negative : Sign::Positive;
  }
  else
  {
    m_sign = (p_value < 0L) ? Sign::Negative : Sign::Positive;
  }
  // Fill in mantissa
  int norm = 0;

  // Part after the decimal point first
  if(p_restValue % bcdBase)
  {
    m_mantissa[0] = long_abs(p_restValue % bcdBase);
    norm = bcdDigits - 1;
  }
  if(p_restValue / bcdBase)
  {
    ShiftRight();
    m_mantissa[0] = long_abs((long)((p_restValue / bcdBase) % bcdBase));
    norm = 2 * bcdDigits - 1;
  }
  if(p_restValue / dblBcdDigits)
  {
    ShiftRight();
    m_mantissa[0] = long_abs((long)(p_restValue / dblBcdDigits));
    norm = 3 * bcdDigits - 1;
  }

  if(p_restValue)
  {
    // Normalize the rest value to be left shifted
    Normalize(norm);
    norm = 0; // reset !!
  }

  // Part before the decimal point
  if(p_value % bcdBase)
  {
    ShiftRight();
    m_mantissa[0] = long_abs((long)(p_value % bcdBase));
    norm = bcdDigits - 1;
  }
  if(p_value / bcdBase)
  {
    ShiftRight();
    m_mantissa[0] = long_abs((long)((p_value / bcdBase) % bcdBase));
    norm = 2 * bcdDigits - 1;
  }
  if(p_value / dblBcdDigits)
  {
    ShiftRight();
    m_mantissa[0] = long_abs((long)(p_value / dblBcdDigits));
    norm = 3 * bcdDigits -1;
  }

  // Normalize the whole result
  Normalize(norm);
}

void    
bcd200::SetValueUInt64(const uint64 p_value,const int64 p_restValue)
{
  uint64 value(p_value);
  bool extra = false;
  if(p_value > LONGLONG_MAX)
  {
    extra  = true;
    value -= LONGLONG_MAX;
    value -= 1;
  }
  SetValueInt64(value,p_restValue);
  if(extra)
  {
    *this += bcd200(LONGLONG_MAX);
    *this += bcd200(1);
  }
  Normalize();
}

// bcd200::SetValueDouble
// Description: Sets the value from a double
void  
bcd200::SetValueDouble(const double p_value)
{
  // Make empty
  Zero();

  // Save in between value
  double between = p_value;
  double notused;

  // Take care of sign
  if(p_value < 0.0)
  {
    m_sign  = Sign::Negative;
    between = -between;
  }
  // Take care of exponent
  m_exponent = (short)::log10(between);

  // Normalize
  between *= ::pow(10.0,(int)-m_exponent);

  // Set the double mantissa into the m_mantissa
  // A double has 16 digits
  for(int ind = 0;ind < (16 / bcdDigits); ++ind)
  {
    long base = bcdBase / 10;
    for(int pos = 0; pos < bcdDigits; ++pos)
    {
      long res = (long) ::fmod(between,10.0);
      between  = ::modf(between,&notused) * 10;

      m_mantissa[ind] += res * base;
      base /= 10;
    }
  }
  Normalize();
}

// bcd200::SetValueString
// Description: Set the value of the bcd200 from a string
// Technical:   Scans [sign][digit][.[digit]*][E[sign][digits]+]
//              part =       1        2          3    
void
bcd200::SetValueString(LPCTSTR p_string,bool /*p_fromDB*/)
{
  // Zero out this number
  Zero();

  bool spacing   = true;
  bool signing   = true;
  int  mantpos   = 0;
  int  digitpos  = bcdDigits;
  int  base      = bcdBase / 10;
  int  part      = 1;
  int  exp_extra = 0;
  Sign exp_sign  = Sign::Positive;
  
  // For normalized numbers without a first part
  m_exponent = -1;

  // Check special cases
  if(_tcscmp(p_string,_T("INF")) == 0)
  {
    m_sign = Sign::INF;
    return;
  }
  if(_tcscmp(p_string,_T("-INF")) == 0)
  {
    m_sign = Sign::MIN_INF;
    return;
  }
  if(_tcscmp(p_string,_T("NaN")) == 0)
  {
    m_sign = Sign::NaN;
    return;
  }
  if(_tcscmp(p_string,_T("NULL")) == 0)
  {
    m_sign = Sign::ISNULL;
    return;
  }

  // Scan the entire string
  for(LPCTSTR pos = p_string; *pos; ++pos)
  {
    // Get a char at the next position
    _TUCHAR c = *pos;

    // Skip whitespace at the beginning
    if(spacing)
    {
      if(isspace(c))
      {
        continue;
      }
      spacing = false;
    }
    // See if a sign is found
    if(signing && part == 1)
    {
      if(c == '-')
      {
        m_sign  = Sign::Negative;
        signing = false;
        continue;
      }
      if(c == '+')
      {
        m_sign  = Sign::Positive;
        signing = false;
        continue;
      }
    }
    // Get type of next character
    switch(c)
    {
      case '.': part = 2; 
                break;
      case 'e': // Fall through
      case 'E': part = 3; 
                break;
      case '-': exp_sign = Sign::Negative;
                break;
      case '+': exp_sign = Sign::Positive;
                break;
      default:  // Now must be a digit. No other chars allowed
                if(isdigit(c) == false)
                {
                  Zero();
                  m_sign = Sign::NaN;
                  if(g_throwing)
                  {
                    throw StdException(_T("BCD: Conversion from string. Bad format in decimal number"));
                  }
                  return;
                }
                break;
    }
    // Decimal point found. Just continue
    if(part == 2 && c == '.')
    {
      continue;
    }
    // Exponent just found. Just continue;
    if(part == 3 && (c == 'e' || c == 'E'))
    {
      continue;
    }
    // Exponent sign just found. Just continue
    if(c == '-' || c == '+')
    {
      continue;
    }
    // Get next char pos
    int number = c - '0';
    // Building the mantissa
    if(part == 1 || part == 2)
    {
      // Adjusting exponent
      if(part == 1)
      {
        // Before the decimal point, the exponent grows
        ++m_exponent;
      }
      // Set in the mantissa
      if(mantpos < bcdLength)
      {
        m_mantissa[mantpos] += number * base;
        base /= 10;

        // Check for next position in the mantissa
        if(--digitpos == 0)
        {
          // Reset for the next long in the mantissa
          base     = bcdBase / 10;
          digitpos = bcdDigits;

          // Next position
          ++mantpos;
        }
      }
      // else skip the latter part of the mantissa
    }
    // Building the exponent
    if(part == 3)
    {
      exp_extra *= 10;
      exp_extra += number;
    }
  }
  // Ready with the string

  // Adjust the exponent from saved variables
  if(exp_extra)
  {
    if(exp_sign == Sign::Negative)
    {
      exp_extra = -exp_extra;
    }
    m_exponent += (short) exp_extra;
  }
  // Normalize for "0.xxx" case
  Normalize();
}

// Sets the value from a SQL NUMERIC
void  
bcd200::SetValueNumeric(const SQL_NUMERIC_STRUCT* p_numeric)
{
  int maxval = SQL_MAX_NUMERIC_LEN - 1;

  // Start at zero
  Zero();

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
  if(ind < 0)
  {
    return;
  }

  // Compute the value array to the bcd200-mantissa
  bcd200 radix(1);
  for(ind = 0;ind <= maxval; ++ind)
  {
    bcd200 val = radix * bcd200(p_numeric->val[ind]);
    *this   = Add(val);
    radix  *= 256;  // Value array is in 256 radix
  }

  // Compute the exponent from the precision and scale
  m_exponent -= p_numeric->scale;

  // Adjust the sign
  m_sign      = (p_numeric->sign == 1) ? Sign::Positive : Sign::Negative;
}

// bcd200::Normalize
// Description: Normalize the exponent. 
//              Always up to first position with implied decimal point
void
bcd200::Normalize(int p_startExponent /*=0*/)
{
  // Set starting exponent
  if(p_startExponent)
  {
    m_exponent = (short)p_startExponent;
  }
  // Check for zero first
  bool zero = true;
  for(int ind = 0; ind < bcdLength; ++ind)
  {
    if(m_mantissa[ind])
    {
      zero = false;
    }
  }
  // Zero mantissa found
  if(zero)
  {
    m_sign     = Sign::Positive;
    m_exponent = 0;
    return;
  }
  // See to it that the mantissa is normalized
  int shift = 0;
  while(((m_mantissa[0] * 10) / bcdBase) == 0)
  {
    ++shift;
    Mult10();
  }
  // Calculate exponent from number of shifts
  m_exponent -= (short) shift;
}

// bcd200::Mult10
// Description: Multiply the mantissa by 10
// Technical:   Optimize by doing shifts
//              Pure internal operation for manipulating the mantissa
//
void
bcd200::Mult10(int p_times /* = 1 */)
{
  // If the number of times is bigger than bcdDigits
  // Optimize by doing shifts instead of MULT
  if(p_times / bcdDigits)
  {
    int shifts = p_times / bcdDigits;
    p_times   %= bcdDigits;
    while(shifts--)
    {
      ShiftLeft();
    }
  }
  while(p_times--)
  {
    long carry   = 0;

    // Multiply all positions by 10
    for(int ind = bcdLength -1; ind >= 0; --ind)
    {
      long between    = m_mantissa[ind] * 10 + carry;
      m_mantissa[ind] = between % bcdBase;
      carry           = between / bcdBase;
    }
  }
}

// bcd200::Div10
// Description: Divide the mantissa by 10
// Technical:   Optimize by doing shifts
//              Pure internal operation for manipulating the mantissa
void
bcd200::Div10(int p_times /*=1*/)
{
  // if the number of times is bigger than bcdDigits
  // optimize by doing shifts instead of divs
  if(p_times / bcdDigits)
  {
    int shifts = p_times / bcdDigits;
    p_times   %= bcdDigits;
    while(shifts--)
    {
      ShiftRight();
    }
  }
  while(p_times--)
  {
    long carry   = 0;

    for(int ind = 0; ind < bcdLength; ++ind)
    {
      long between    = m_mantissa[ind] + (carry * bcdBase);
      carry           = between % 10;
      m_mantissa[ind] = between / 10;
    }
  }
}

// bcd200::ShiftRight
// Description: Shift the mantissa members one position right
void
bcd200::ShiftRight()
{
  for(int ind = bcdLength - 1; ind > 0; --ind)
  {
    m_mantissa[ind] = m_mantissa[ind - 1];
  }
  m_mantissa[0] = 0;
}

// bcd200::ShiftLeft
// Description: Shift the mantissa members one position left
void
bcd200::ShiftLeft()
{
  for(int ind = 0; ind < bcdLength - 1; ++ind)
  {
    m_mantissa[ind] = m_mantissa[ind + 1];
  }
  m_mantissa[bcdLength - 1] = 0;
}

// bcd200::LongNaarString
XString
bcd200::LongToString(long p_value) const
{
  TCHAR buffer[20];
  _itot_s(p_value,buffer,20,10);
  return XString(buffer);
}

// bcd200::StringNaarLong
// Description: Convert a string to a single long value
long
bcd200::StringToLong(LPCTSTR p_string) const
{
  return _ttoi(p_string);
}

// bcd200::SplitMantissa
// Description: Split the mantissa for floor/ceiling operations
bcd200
bcd200::SplitMantissa() const
{
  bcd200 result = *this;

  // Splitting position is 1 more than the exponent
  // because of the implied first position
  int position = m_exponent + 1;

  for(int mantpos = 0;mantpos < bcdLength; ++mantpos)
  {
    if(position <= 0)
    {
      // Result after this position will be zero
      result.m_mantissa[mantpos] = 0;
    }
    else if(position >= bcdDigits)
    {
      // Keep these numbers in the mantissa
      position -= bcdDigits;
    }
    else
    {
      // Here we will split the bcdDigits somewhere in the middle
      long base   = bcdBase / 10;
      long number = result.m_mantissa[mantpos];

      for(int pos = bcdDigits; pos > 0; --pos)
      {
        // Find next number
        long num = number / base;
        number  %= base;

        // zero out ?
        if(position <= 0)
        {
          result.m_mantissa[mantpos] -= (num * base);
        }
        // go to the next position
        --position;
        base /= 10;
      }
    }
  }
  return result;
}

// bcd200::CompareMantissa
// Description: Compare two mantissa
// Technical:   returns the following values
//              -1  p_value is bigger
//              0   mantissa are equal
//              1   this is is bigger
int   
bcd200::CompareMantissa(const bcd200& p_value) const
{
  // Now compare the mantissa
  for(int ind = 0;ind < bcdLength; ++ind)
  {
    // Find the first position not equal to the other
    if(m_mantissa[ind] != p_value.m_mantissa[ind])
    {
      // Result by comparing the mantissa positions
      return (m_mantissa[ind] > p_value.m_mantissa[ind]) ? 1 : -1;
    }
  }
  // Mantissa are equal
  return 0;
}

#ifdef _DEBUG
// Debug print of the mantissa
XString
bcd200::DebugPrint(PTCHAR p_name)
{
  XString debug;

  // Print the debug name
  debug.Format(_T("%-14s "),p_name);

  // Print the sign
  debug.AppendFormat(_T("%c "),m_sign == Sign::Positive ? '+' : '-');

  // Print the exponent
  debug.AppendFormat(_T("E%+d "),m_exponent);

  // Print the mantissa in special format
  for(int ind = 0;ind < bcdLength; ++ind)
  {
    // Text "%08ld" dependent on bcdDigits
    debug.AppendFormat(_T(" %08ld"),m_mantissa[ind]);
  }
  debug += _T("\n");

  return debug;
}
#endif

// bcd200::Epsilon
// Description: Stopping criterion for iterations
// Technical:   Translates fraction to lowest decimal position
//               10 -> 0.0000000000000000000000000000000000000010
//                5 -> 0.0000000000000000000000000000000000000005
bcd200&
bcd200::Epsilon(long p_fraction) const
{
  // Calculate stop criterion epsilon
  static bcd200 epsilon;
  epsilon.m_mantissa[0] = p_fraction * bcdBase / 10;
  epsilon.m_exponent    = 2 - bcdPrecision;
  return epsilon;
}

// Calculate the precision and scale for a SQL_NUMERIC
// Highly optimized version as we do this a lot when
// streaming bcd200 numbers to the database
void
bcd200::CalculatePrecisionAndScale(SQLCHAR& p_precision,SQLCHAR& p_scale) const
{
  // Default max values
  p_precision = bcdDigits * bcdLength;
  p_scale     = 0;

  // Quick check on zero
  if(IsZero())
  {
    p_precision = 1;
    return;
  }

  int index;
  // Find the first non-zero mantissa digit
  for(index = bcdLength - 1;index >= 0; --index)
  {
    if(m_mantissa[index] == 0)
    {
      p_precision -= bcdDigits;
    }
    else break;
  }
  if(index < 0)
  {
    return;
  }
  // Find the number of digits in this mantissa
  // Change this optimization when changing bcdDigits or bcdLength !!
  if(m_mantissa[index] % 10000)
  {
    // Lower half filled
    if(m_mantissa[index] % 100)
    {
      // 7 or 8 digits
      p_precision -= (m_mantissa[index] % 10) ? 0 : 1;
    }
    else
    {
      // 5 or 6 digits
      p_precision -= (m_mantissa[index] % 1000) ? 2 : 3;
    }
  }
  else
  {
    // Lower half is empty
    if(m_mantissa[index] % 1000000)
    {
      // 3 or 4 digits
      p_precision -= (m_mantissa[index] % 100000) ? 4 : 5;
    }
    else
    {
      // just two digits
      p_precision -= (m_mantissa[index] % 10000000) ? 6 : 7;
    }
  }
  // Final check on maximum precision
  // Cannot exceed SQLNUM_MAX_PREC as otherwise 
  // we **will** crash on certain RDBMS (MS SQL-Server)
  if(p_precision > SQLNUM_MAX_PREC)
  {
    p_precision = SQLNUM_MAX_PREC;
  }

  // Now adjust the scale to accommodate precision and exponent
  // m_exponent always below (-)SQLNUM_MAX_PREC, so this is safe
  if(m_exponent < 0)
  {
    p_scale      = p_precision - (SQLCHAR) m_exponent - 1;
    p_precision -= (SQLCHAR) m_exponent;
  }
  else
  {
    if((p_precision - 1) > (SQLCHAR) m_exponent)
    {
      p_scale = p_precision - (SQLCHAR) m_exponent - 1;
    }
    if(p_precision <= m_exponent)
    {
      p_precision = (SQLCHAR) m_exponent + 1;
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
// END OF INTERNALS OF BCD
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// BASIC OPERATIONS OF BCD
//
//////////////////////////////////////////////////////////////////////////

// Addition operation
bcd200 
bcd200::Add(const bcd200& p_number) const 
{
  // Check if we can add
  if(!IsValid() || !p_number.IsValid())
  {
    return SetInfinity(_T("Cannot add to INFINITY"));
  }
  // NULL always yield a NULL
  if(IsNULL() || p_number.IsNULL())
  {
    return bcd200(Sign::ISNULL);
  }
  // See if we must do addition or subtraction
  // Probably we need to swap the arguments....
  // (+x) + (+y) -> Addition,    result positive, Do not swap
  // (+x) + (-y) -> Subtraction, result pos/neg,  Possibly swap
  // (-x) + (+y) -> Subtraction, result pos/neg,  Possibly swap
  // (-x) + (-y) -> Addition,    result negative, Do not swap
  Sign     signResult   = Sign::Positive;
  Operator operatorKind = Operator::Addition;
  bcd200      arg1(*this);
  bcd200      arg2(p_number);
  PositionArguments(arg1, arg2, signResult, operatorKind);

  if (operatorKind == Operator::Addition)
  {
    arg1 = PositiveAddition(arg1, arg2);
  }
  else
  {
    if(arg1 > arg2)
    {
      arg1 = PositiveSubtraction(arg1, arg2);
    }
    else
    {
      arg1 = PositiveSubtraction(arg2, arg1);
    }
  }
  arg1.m_sign = signResult;

  return arg1;
}

// Subtraction operation
bcd200 
bcd200::Sub(const bcd200& p_number) const 
{
  // Check if we can subtract
  if(!IsValid() || !p_number.IsValid())
  {
    return SetInfinity(_T("Cannot subtract with INFINITY"));
  }
  // NULL always yield a NULL
  if(IsNULL() || p_number.IsNULL())
  {
    return bcd200(Sign::ISNULL);
  }
  // x-y is equal to  x+(-y)
  return *this + (-p_number);
}

// Multiplication
bcd200 
bcd200::Mul(const bcd200& p_number) const 
{
  // Check if we can multiply
  if(!IsValid() || !p_number.IsValid())
  {
    return SetInfinity(_T("Cannot multiply with INFINITY"));
  }
  // NULL always yield a NULL
  if(IsNULL() || p_number.IsNULL())
  {
    return bcd200(Sign::ISNULL);
  }
  // Multiplication without signs
  bcd200 result = PositiveMultiplication(*this,p_number);

  // Take care of the sign
  result.m_sign = result.IsZero() ? Sign::Positive : CalculateSign(*this, p_number);

  return result;
}

// Division
bcd200 
bcd200::Div(const bcd200& p_number) const 
{
  // Check if we can divide
  if(!IsValid() || !p_number.IsValid())
  {
    return SetInfinity(_T("Cannot divide with INFINITY"));
  }
  // NULL always yield a NULL
  if(IsNULL() || p_number.IsNULL())
  {
    return bcd200(Sign::ISNULL);
  }
  // If divisor is zero -> ERROR
  if(p_number.IsZero())
  {
    return SetInfinity(_T("BCD: Division by zero."));
  }
  // Shortcut: result is zero if this is zero
  if(IsZero())
  {
    return *this;
  }
  // Division without signs
  bcd200 arg1(*this);
  bcd200 arg2(p_number);
  bcd200 result = PositiveDivision(arg1,arg2);

  // Take care of the sign
  result.m_sign = result.IsZero() ? Sign::Positive : CalculateSign(*this, p_number);

  return result;
}

// Modulo
bcd200 
bcd200::Mod(const bcd200& p_number) const 
{
  // Check if we can do a modulo
  if(!IsValid() || !p_number.IsValid())
  {
    return SetInfinity(_T("Cannot do a modulo with INFINITY"));
  }
  // NULL always yield a NULL
  if(IsNULL() || p_number.IsNULL())
  {
    return bcd200(Sign::ISNULL);
  }
  bcd200 count = ((*this) / p_number).Floor();
  bcd200 mod((*this) - (count * p_number));

  if (m_sign == Sign::Negative)
  {
    mod = -mod;
  }
  return mod;
}

// Position the arguments for a positive addition or subtraction
// Only called from within Add()
void
bcd200::PositionArguments(bcd200&       arg1,
                       bcd200&       arg2,
                       Sign&      signResult,
                       Operator&  operatorKind) const
{
  // Get the resulting kind of operator and sign
  // if (-x) + y then turnaround, so x + (-y), becomes x - y
  if (arg1.m_sign == Sign::Positive)
  {
    if(arg2.m_sign == Sign::Positive)
    {
      // Both are positive
      signResult   = Sign::Positive;
      operatorKind = Operator::Addition;
    }
    else
    {
      // Arg2 is negative
      // Now the rest is positive
      arg1.m_sign  = Sign::Positive;
      arg2.m_sign  = Sign::Positive;
      operatorKind = Operator::Subtraction;
      // Sign depends on the size
      signResult   = (arg1 >= arg2) ? Sign::Positive : Sign::Negative;
    }
  }
  else // arg1.m_sign == Negative
  {
    if(arg2.m_sign == Sign::Negative)
    {
      // Both are negative
      arg1.m_sign  = Sign::Positive;
      arg2.m_sign  = Sign::Positive;
      signResult   = Sign::Negative;
      operatorKind = Operator::Addition;
    }
    else
    {
      // arg2 is positive
      // Now the rest is positive
      arg1.m_sign = Sign::Positive;
      arg2.m_sign = Sign::Positive;
      operatorKind = Operator::Subtraction;
      // Sign depends on the size
      signResult  = (arg2 >= arg1) ? Sign::Positive : Sign::Negative;
    }
  }
}

bcd200::Sign
bcd200::CalculateSign(const bcd200& p_arg1, const bcd200& p_arg2) const
{
  // Find the sign for multiplication / division
  // (+x) * (+y) -> positive
  // (-x) * (+y) -> negative
  // (-x) * (-y) -> positive
  // (+x) * (-y) -> negative
  if (p_arg1.IsZero() || p_arg2.IsZero())
  {
    return Sign::Positive;
  }
  if(p_arg1.m_sign != p_arg2.m_sign)
  {
    return Sign::Negative;
  }
  return Sign::Positive;
}

// Addition of two mantissa (no signs/exponents)
bcd200
bcd200::PositiveAddition(bcd200& arg1,bcd200& arg2) const
{
  // Take care of the exponents
  if(arg1.m_exponent != arg2.m_exponent)
  {
    // If numbers differ more than this, an addition is useless
    int border = bcdDigits * bcdLength;

    if(arg1.m_exponent > arg2.m_exponent)
    {
      int shift = arg1.m_exponent - arg2.m_exponent;
      if(shift > border)
      {
        // Adding arg2 will not result in a difference
        return arg1;
      }
      // Shift arg2 to the right;
      arg2.Div10(shift);
    }
    else
    {
      int shift = arg2.m_exponent - arg1.m_exponent;
      if(shift > border)
      {
        // Adding arg1 will not result in a difference
        return arg2;
      }
      // Shift arg1 to the right
      arg1.Div10(shift);
      arg1.m_exponent += (short) shift;
    }
  }
  // Do the addition of the mantissa
  int64 carry = 0L;
  for(int ind = bcdLength - 1;ind >= 0; --ind)
  {
    int64 reg = ((int64)arg1.m_mantissa[ind]) + ((int64)arg2.m_mantissa[ind]) + carry;
    carry = reg / bcdBase;
    arg1.m_mantissa[ind] = reg % bcdBase;
  }
  // Take care of carry
  if(carry)
  {
    arg1.Div10();
    arg2.Div10();
    arg1.m_exponent++;
    arg1.m_mantissa[0] += (long)(carry * (bcdBase / 10));
  }
  return arg1;
}

// Subtraction of two mantissa (no signs/exponents)
// Precondition arg1 > arg2
bcd200
bcd200::PositiveSubtraction(bcd200& arg1,bcd200& arg2) const
{
  // Take care of the exponents
  if(arg1.m_exponent != arg2.m_exponent)
  {
    // If numbers differ more than this, an addition is useless
    int border = bcdDigits * bcdLength;

    int shift = arg1.m_exponent - arg2.m_exponent;
    if(shift > border)
    {
      // Adding arg2 will not result in a difference
      return arg1;
    }
    if (shift > 0)
    {
      // Shift arg2 to the right;
      arg2.Div10(shift);
    }
    else
    {
      arg2.Mult10(-shift);
    }
  }
  // Do the subtraction of the mantissa
  for(int ind = bcdLength - 1;ind >= 0; --ind)
  {
    if(arg1.m_mantissa[ind] >= arg2.m_mantissa[ind])
    {
      arg1.m_mantissa[ind] -= arg2.m_mantissa[ind];
    }
    else
    {
      int64 reg = ((int64)bcdBase + arg1.m_mantissa[ind]) - arg2.m_mantissa[ind];
      arg1.m_mantissa[ind] = (long) reg;
      // Take care of carry
      if(ind > 0)
      {
        arg1.m_mantissa[ind - 1] -= 1;
      }
    }
  }
  // Normalize the mantissa/exponent
  arg1.Normalize();

  // Return the result
  return arg1;
}

// bcd200::PositiveMultiplication
// Description: Multiplication of two mantissa (no signs)
// Technical:   1) addition of the exponents
//              2) multiplication of the mantissa
//              3) take-in carry and normalize
bcd200
bcd200::PositiveMultiplication(const bcd200& p_arg1,const bcd200& p_arg2) const
{
  bcd200 result;
  int64 res[2 * bcdLength] = { 0 };

  // Multiplication of the mantissa
  for(int i = bcdLength - 1; i >= 0; --i)
  {
    for(int j = bcdLength - 1; j >= 0; --j)
    {
      int64 between = (int64)p_arg1.m_mantissa[i] * (int64)p_arg2.m_mantissa[j];
      res[i + j + 1] += between % bcdBase; // result
      res[i + j    ] += between / bcdBase; // carry
    }
  }

  // Normalize resulting mantissa to bcdBase
  int64 carry   = 0;
  for(int ind = (2 * bcdLength) - 1;ind >= 0; --ind)
  {
    res[ind] += carry;
    carry     = res[ind] / bcdBase;
    res[ind] %= bcdBase;
  }

  // Possibly perform rounding of res[bcdLength] -> res[bcdLength-1]

  // Put the resulting mantissa's in the result
  for(int ind = 0; ind < bcdLength; ++ind)
  {
    result.m_mantissa[ind] = (long)res[ind];
  }

  // Take care of exponents
  // Multiplication is the addition of the exponents
  // Add 1 for the implied 1 position of the second argument
  result.m_exponent = p_arg1.m_exponent + p_arg2.m_exponent + 1;

  // Normalize to implied dec.point position
  result.Normalize();

  return result;
}

// bcd200::PositiveDivision
// Description: Division of two mantissa (no signs)
// Technical:   Do traditional 'long division' of two bcd200's
bcd200
bcd200::PositiveDivision(bcd200& p_arg1,bcd200& p_arg2) const
{
  bcd200   result;
  short result_mantissa[bcdPrecision + 1] = { 0 };
  bcd200   subtrahend;

  // Division of the mantissa
  long dividend  = 0;
  long divisor   = 0; 
  long quotient  = 0;
  int  guess     = 2;

  // Grade down arg2 one position
  p_arg2.Div10();

  for(int ind = 1;ind <= bcdPrecision; ++ind)
  {
//     printf("Iteration: %d\n",ind);
//     p_arg1.DebugPrint("argument1");
//     p_arg2.DebugPrint("argument2");

    // Check for intermediate of zero. Arg1 != zero, so it must end!!
    bool zero = true;
    for(int x = 0;x < bcdLength; ++x)
    {
      if(p_arg1.m_mantissa[x]) 
      {
        zero = false;
        break;
      }
    }
    // arg1 has become zero. Nothing more to do
    if(zero)
    {
      break;
    }
    // If not at implied position: Mantissa can shift 1 position up
    if((p_arg1.m_mantissa[0] * 10 / bcdBase) == 0)
    {
      p_arg1.Mult10();

      // If still not at implied position, we may do an extra shifts 
      // inserting zero's in the result if there are more
      if((p_arg1.m_mantissa[0] * 10 / bcdBase) == 0)
      {
        do
        {
          result_mantissa[ind++] = 0;
          p_arg1.Mult10();
          if(ind > bcdPrecision)
          {
            // Escape from double loop (for/while)
            goto end;
          }
        }
        while((p_arg1.m_mantissa[0] * 10 / bcdBase) == 0);
      }
    }

//     // Resulting mantissa's 
//     p_arg1.DebugPrint("argument1-2");
//     p_arg2.DebugPrint("argument2-2");

    // get quotient
    dividend = p_arg1.m_mantissa[0];
    divisor  = p_arg2.m_mantissa[0];
    quotient = dividend / divisor;

    // Try to get a subtrahend
    // Could go wrong because the total of quotient * arg2.mantissa can be > than arg1.mantissa
    guess = 2;
    while(guess--)
    {
      // quotient * p_arg2 -> subtrahend
      int64 carry  = 0;
      for(int pos = bcdLength - 1; pos >= 0; --pos)
      {
        int64 number = (int64)quotient * (int64)p_arg2.m_mantissa[pos] + carry;
        subtrahend.m_mantissa[pos] = number % bcdBase;
        carry = number / bcdBase;
      }
//    // Effective subtrahend
//    subtrahend.DebugPrint("subtrahend");

      // if(p_arg1 < subtrahend)
      if(p_arg1.CompareMantissa(subtrahend) == -1)
      {
        --quotient;
      }
      else
      {
        // Effective quotient found for this 'ind' step
//      printf("%02d quotient: %d\n",ind,quotient);

        // Ready guessing: quotient is OK
        guess = 0;
        result_mantissa[ind] = (short)quotient;
        // arg1 = arg1 - subtrahend
        for(int pos = bcdLength - 1;pos >= 0; --pos)
        {
          if(p_arg1.m_mantissa[pos] >= subtrahend.m_mantissa[pos])
          {
            p_arg1.m_mantissa[pos] -= subtrahend.m_mantissa[pos];
          }
          else
          {
            int64 number = ((int64)bcdBase + p_arg1.m_mantissa[pos]) - subtrahend.m_mantissa[pos];
            p_arg1.m_mantissa[pos] = (long)number;
            if(pos > 0)
            {
              // Do the borrow
              p_arg1.m_mantissa[pos - 1] -= 1;
            }
          }
        }
      }
//    // Effective difference (minuend - subtrahend)
//    p_arg1.DebugPrint("argument1-3");
    }
  }
end:
  // Normalize result
  short carry = 0;
  for(int i = bcdPrecision; i > 0; --i)
  {
    result_mantissa[i] += carry;
    carry               = result_mantissa[i] / 10;
    result_mantissa[i] %= 10;
  }
  result_mantissa[0] = carry;

  // Get starting position of the answer
  int respos = (carry == 0) ? 1 : 0;

  // Set the result_mantissa in the result number
  for(int mantpos = 0;mantpos < bcdLength; ++mantpos)
  {
    long number = 0;
    long base   = bcdBase / 10;

    for(int ind = 0; ind < bcdDigits; ++ind)
    {
      number += result_mantissa[respos++] * base;
      base   /= 10;
    }
    result.m_mantissa[mantpos] = number;
  }

  // Subtraction of the exponents
  // Subtract 1 for initial Div10 of the second argument
  result.m_exponent = p_arg1.m_exponent - p_arg2.m_exponent - 1;

  // If we had a carry, the exponent will be 1 higher
  if(carry)
  {
    result.m_exponent++;
  }

  // Return the result
  return result;
}

// On overflow we set negative or positive infinity
bcd200
bcd200::SetInfinity(XString p_reason /*= ""*/) const
{
  if(g_throwing)
  {
    throw StdException(p_reason);
  }
  // NaN AND previous infinity is set to positive infinity !!
  bcd200 inf;
  inf.m_sign = (m_sign == Sign::Negative) ? Sign::MIN_INF : Sign::INF;
  return inf;
}

//////////////////////////////////////////////////////////////////////////
//
// END OF BASIC OPERATIONS OF BCD
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// OVERLOADED MATH FUNCTIONS (Everybody has come to expect)
//
//////////////////////////////////////////////////////////////////////////

// Overloaded math precision floating point functions equivalent with the std C functions
// Overloaded to work with the BCD number class, always yielding a bcd200 number.

bcd200 modf(const bcd200& p_number, bcd200* p_intpart)
{
  *p_intpart = p_number.Floor();
  return p_number.Fraction();
}

bcd200 fmod(const bcd200& p_number,const bcd200& p_divisor)
{
  return p_number % p_divisor;
}

bcd200 floor(const bcd200& p_number)
{
  return p_number.Floor();
}

bcd200 ceil(const bcd200& p_number)
{
  return p_number.Ceiling();
}

bcd200 fabs(const bcd200& p_number)
{
  return p_number.AbsoluteValue();
}

bcd200 sqrt(const bcd200& p_number)
{
  return p_number.SquareRoot();
}

bcd200 log10(const bcd200& p_number)
{
  return p_number.Log10();
}

bcd200 log(const bcd200& p_number)
{
  return p_number.Log();
}

bcd200 exp(const bcd200& p_number)
{
  return p_number.Exp();
}

bcd200 pow(const bcd200& p_number,const bcd200& p_power)
{
  return p_number.Power(p_power);
}

bcd200 frexp(const bcd200& p_number,int* p_exponent)
{
  *p_exponent = p_number.GetExponent();
  return p_number.GetMantissa();
}

bcd200 ldexp(const bcd200& p_number,int p_power)
{
  if(p_power == 0)
  {
    return p_number;
  }
  if(p_power > 0 && p_power <= 31)
  {
    return p_number * bcd200((long) (((unsigned)1) << p_power));
  }
  return p_number * pow(bcd200(2L),bcd200((long)p_power));
}

// Overloaded trigonometric functions on a bcd200 number

bcd200 atan (const bcd200& p_number) 
{ 
  return p_number.ArcTangent(); 
}

bcd200 atan2(const bcd200& p_y,const bcd200& p_x)
{
  return p_y.ArcTangent2Points(p_x);
}

bcd200 asin(const bcd200& p_number)
{
  return p_number.ArcSine();
}

bcd200 acos(const bcd200& p_number)
{
  return p_number.ArcCosine();
}

bcd200 sin(const bcd200& p_number)
{
  return p_number.Sine();
}

bcd200 cos(const bcd200& p_number)
{
  return p_number.Cosine();
}

bcd200 tan(const bcd200& p_number)
{
  return p_number.Tangent();
}

//////////////////////////////////////////////////////////////////////////
//
// END OF OVERLOADED MATH FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// FILE STREAM FUNCTIONS
// Used for binary storage in a binary-mode file stream!
//
//////////////////////////////////////////////////////////////////////////

bool
bcd200::WriteToFile (FILE* p_fp)
{
  // Write out the sign
  if(putc((char)m_sign,p_fp)      == EOF) return false;
  // Write out the exponent (little endian)
  if(putc(m_exponent >> 8,  p_fp) == EOF) return false;
  if(putc(m_exponent & 0xFF,p_fp) == EOF) return false;
  // Write out the mantissa (little endian)
  for(unsigned int ind = 0;ind < bcdLength; ++ind)
  {
    ulong num = (ulong) m_mantissa[ind];
    if(putc((num & 0xFF000000) >> 24,p_fp) == EOF) return false;
    if(putc((num & 0x00FF0000) >> 16,p_fp) == EOF) return false;
    if(putc((num & 0x0000FF00) >>  8,p_fp) == EOF) return false;
    if(putc((num & 0x000000FF)      ,p_fp) == EOF) return false;
  }
  return true;
}

bool
bcd200::ReadFromFile(FILE* p_fp)
{
  int ch = 0;

  // Read in the sign
  m_sign = (Sign) getc(p_fp);
  // Read in the exponent
  ch = getc(p_fp);
  m_exponent = (short) (ch << 8);
  ch = getc(p_fp);
  m_exponent += (short) ch;

  // Read in the mantissa
  for(unsigned int ind = 0; ind < bcdLength; ++ind)
  {
    ulong num = 0L;
    ch = getc(p_fp); num += ((ulong)ch) << 24;
    ch = getc(p_fp); num += ((ulong)ch) << 16;
    ch = getc(p_fp); num += ((ulong)ch) <<  8;
    ch = getc(p_fp); num +=  (ulong)ch;
    m_mantissa[ind] = num;
  }

  // BCD is invalid in case of file error
  if(ferror(p_fp))
  {
    Zero();
    return false;
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
// END OF FILE STREAM FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// END OF BCD IMPLEMENTATION
//
//////////////////////////////////////////////////////////////////////////
