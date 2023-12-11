/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: bcd200.h
//
// BaseLibrary: Indispensable general objects and functions
//
// Copyright (c) 2014-2022 ir. W.E. Huisman
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
// BCD
//
// Floating Point Precision Number class (Binary Coded Decimal)
// A number always has the format [sign][[digit]*][.[digit]*][E[sign][digits]+] where sign is either '+' or '-'
// Numbers are stored in 1E8 based mantissa with a digital . implied at the second position
// The mantissa array exists of a series of integers with 8 functional digits each
//
// Copyright (c) 2014-2022 ir W. E. Huisman
// Version 1.5 of 03-01-2022
//
#pragma once
#include <intsafe.h>    // Min/Max sizes of integer datatypes
#include <sqltypes.h>   // Needed for conversions of SQL_NUMERIC_STRUCT

#define XString CString

// The ODBC standard has a maximum of 38 decimal places
// At least one database (Oracle) implements these numbers
#ifndef SQLNUM_MAX_PREC
#define SQLNUM_MAX_PREC 38
#endif

// Constants that controls the actual precision:
const int bcdBase      = 100000000L; // Base of the numbers in m_mantissa
const int bcdDigits    = 8;          // Number of digits in one mantissa element

//////////////////////////////////////////////////////////////////////////
// [200]
// THE NEXT LINE IS THE ONLY LINE THAT DIFFERS FROM THE bcd.h INTERFACE!!!
const int bcdLength    = 25;         // Number of elements in the mantissa    
//////////////////////////////////////////////////////////////////////////

const int bcdPrecision = bcdDigits * bcdLength;
// When rethinking one of these four constants, 
// be sure to edit the following points in the implementation class
// - The constants:   PI, LN2, LN10
// - The conversions: AsLong, AsInt64, AsNumeric
// - The setters:     SetValueLong, SetValueInt64, SetValueNumeric
// - Some generals:   DebugPrint ("%08d")

// Handy typedefs of used basic datatypes
using uchar  = unsigned char;
using ushort = unsigned short;
using ulong  = unsigned long;

#ifndef int64
using int64  = __int64;
#endif

#ifndef uint64
using uint64 = unsigned __int64;
#endif

// Forward declaration of our class
class bcd200;

// Overloaded standard mathematical functions
bcd200 floor(const bcd200& p_number);
bcd200 ceil (const bcd200& p_number);
bcd200 fabs (const bcd200& p_number);
bcd200 sqrt (const bcd200& p_number);
bcd200 log10(const bcd200& p_number);
bcd200 log  (const bcd200& p_number);
bcd200 exp  (const bcd200& p_number);
bcd200 pow  (const bcd200& p_number,const bcd200& p_power);
bcd200 frexp(const bcd200& p_number,int* p_exponent);
bcd200 ldexp(const bcd200& p_number,int  p_power);
bcd200 modf (const bcd200& p_number,bcd200* p_intpart);
bcd200 fmod (const bcd200& p_number,const bcd200& p_divisor);

// Overloaded standard trigonometric functions on a bcd200 number
bcd200 sin  (const bcd200& p_number);
bcd200 cos  (const bcd200& p_number);
bcd200 tan  (const bcd200& p_number);
bcd200 asin (const bcd200& p_number);
bcd200 acos (const bcd200& p_number);
bcd200 atan (const bcd200& p_number);
bcd200 atan2(const bcd200& p_y,const bcd200& p_x);

// One-time initialization for printing numbers in the current locale
void InitValutaString();

// string format number and money format functions
extern bool  g_locale_valutaInit;
extern TCHAR g_locale_decimalSep[];
extern TCHAR g_locale_thousandSep[];
extern TCHAR g_locale_strCurrency[];
extern int   g_locale_decimalSepLen;
extern int   g_locale_thousandSepLen;
extern int   g_locale_strCurrencyLen;

//////////////////////////////////////////////////////////////////////////
//
// The Binary Coded Decimal class
//
//////////////////////////////////////////////////////////////////////////

class bcd200
{
public:
  // CONSTRUCTORS/DESTRUCTORS

  // Default constructor.
  bcd200();

  // Copy constructor.
  bcd200(const bcd200& icd);

  // BCD from a char value
  explicit bcd200(const TCHAR p_value);

#ifndef UNICODE
  // BCD from an unsigned char value
  explicit bcd200(const _TUCHAR p_value);
#endif
  // BCD from a short value
  explicit bcd200(const short p_value);

  // BCD from an unsigned short value
  explicit bcd200(const unsigned short p_value);

  // BCD from an integer
  explicit bcd200(const int p_value);

  // BCD from an unsigned integer
  explicit bcd200(const unsigned int p_value);

  // BCD from a long
  explicit bcd200(const long p_value, const long p_restValue = 0);

  // BCD from an unsigned long
  explicit bcd200(const unsigned long p_value, const unsigned long p_restValue = 0);

  // BCD from a 64bits int
  explicit bcd200(const int64 p_value,const int64 p_restvalue = 0);

  // BCD from an unsigned 64bits int
  explicit bcd200(const uint64 p_value,const int64 p_restvalue = 0);

  // BCD from a float
  explicit bcd200(const float p_value);

  // BCD from a double
  explicit bcd200(const double p_value);

  // BCD From a character string
  explicit bcd200(PCTSTR p_string,bool p_fromDB = false);

  // BCD from a SQL_NUMERIC_STRUCT
  explicit bcd200(const SQL_NUMERIC_STRUCT* p_numeric);

  // ENUMERATIONS

  // Keep sign status in this order!
  enum class Sign
  {
     Positive  // bcd200 number >= 0
    ,Negative  // bcd200 number  < 0
    ,ISNULL    // bcd200 number is NULL in the database
    ,MIN_INF   // bcd200 number in negative infinity
    ,INF       // bcd200 number in positive infinity
    ,NaN       // Not a Number (e.g. a string)
  };
  // Formatting of a string (As<X>String())
  enum class Format
  {
     Engineering
    ,Bookkeeping
  };
  // For internal processing
  enum class Operator
  {
     Addition
    ,Subtraction
  };

  // BCD constructs as a NULL from the database
  explicit bcd200(const bcd200::Sign p_sign);

  // CONSTANTS

  static bcd200 PI();     // Circumference/Radius ratio of a circle
  static bcd200 LN2();    // Natural logarithm of 2
  static bcd200 LN10();   // Natural logarithm of 10

  // ERROR HANDLING

  // BCD throws on error or sets status (-INF, INF, NAN)
  // BEWARE: Not thread safe to change in flight
  // Applications must use ONE (1) setting at startup
  static void ErrorThrows(bool p_throws = true);

  // OPERATORS

  // Standard mathematical operators
  const bcd200  operator+(const bcd200&   p_value) const;
  const bcd200  operator-(const bcd200&   p_value) const;
  const bcd200  operator*(const bcd200&   p_value) const;
  const bcd200  operator/(const bcd200&   p_value) const;
  const bcd200  operator%(const bcd200&   p_value) const;

  const bcd200  operator+(const int    p_value) const;
  const bcd200  operator-(const int    p_value) const;
  const bcd200  operator*(const int    p_value) const;
  const bcd200  operator/(const int    p_value) const;
  const bcd200  operator%(const int    p_value) const;

  const bcd200  operator+(const double p_value) const;
  const bcd200  operator-(const double p_value) const;
  const bcd200  operator*(const double p_value) const;
  const bcd200  operator/(const double p_value) const;
  const bcd200  operator%(const double p_value) const;

  const bcd200  operator+(LPCTSTR p_value) const;
  const bcd200  operator-(LPCTSTR p_value) const;
  const bcd200  operator*(LPCTSTR p_value) const;
  const bcd200  operator/(LPCTSTR p_value) const;
  const bcd200  operator%(LPCTSTR p_value) const;

  // Standard math/assignment operators
  bcd200& operator+=(const bcd200& p_value);
  bcd200& operator-=(const bcd200& p_value);
  bcd200& operator*=(const bcd200& p_value);
  bcd200& operator/=(const bcd200& p_value);
  bcd200& operator%=(const bcd200& p_value);

  bcd200& operator+=(const int p_value);
  bcd200& operator-=(const int p_value);
  bcd200& operator*=(const int p_value);
  bcd200& operator/=(const int p_value);
  bcd200& operator%=(const int p_value);

  bcd200& operator+=(const double p_value);
  bcd200& operator-=(const double p_value);
  bcd200& operator*=(const double p_value);
  bcd200& operator/=(const double p_value);
  bcd200& operator%=(const double p_value);

  bcd200& operator+=(LPCTSTR p_value);
  bcd200& operator-=(LPCTSTR p_value);
  bcd200& operator*=(LPCTSTR p_value);
  bcd200& operator/=(LPCTSTR p_value);
  bcd200& operator%=(LPCTSTR p_value);

  // Prefix unary minus (negation)
  bcd200  operator-() const;

  // Prefix/Postfix increment/decrement operators
  bcd200  operator++(int);  // Postfix increment
  bcd200& operator++();     // Prefix  increment
  bcd200  operator--(int);  // Postfix decrement
  bcd200& operator--();     // Prefix  decrement

  // Assignment operators
  bcd200& operator=(const bcd200&    p_value);
  bcd200& operator=(const int     p_value);
  bcd200& operator=(const double  p_value);
  bcd200& operator=(const PCTSTR  p_value);
  bcd200& operator=(const __int64 p_value);

  // comparison operators
  bool operator==(const bcd200&   p_value) const;
  bool operator!=(const bcd200&   p_value) const;
  bool operator< (const bcd200&   p_value) const;
  bool operator> (const bcd200&   p_value) const;
  bool operator<=(const bcd200&   p_value) const;
  bool operator>=(const bcd200&   p_value) const;

  bool operator==(const int    p_value) const;
  bool operator!=(const int    p_value) const;
  bool operator< (const int    p_value) const;
  bool operator> (const int    p_value) const;
  bool operator<=(const int    p_value) const;
  bool operator>=(const int    p_value) const;

  bool operator==(const double p_value) const;
  bool operator!=(const double p_value) const;
  bool operator< (const double p_value) const;
  bool operator> (const double p_value) const;
  bool operator<=(const double p_value) const;
  bool operator>=(const double p_value) const;

  bool operator==(LPCTSTR p_value) const;
  bool operator!=(LPCTSTR p_value) const;
  bool operator< (LPCTSTR p_value) const;
  bool operator> (LPCTSTR p_value) const;
  bool operator<=(LPCTSTR p_value) const;
  bool operator>=(LPCTSTR p_value) const;

  // MAKING AN EXACT NUMERIC value
  
  // Set the mantissa/exponent/sign to the number zero (0)
  void    Zero();
  // Set to database NULL
  void    SetNULL();
  // Round to a specified fraction (decimals behind the .)
  void    Round(int p_precision = 0);
  // Truncate to a specified fraction (decimals behind the .)
  void    Truncate(int p_precision = 0);  
  // Change length and precision
  void    SetLengthAndPrecision(int p_precision = bcdPrecision,int p_scale = (bcdPrecision / 2));
  // Change the sign
  void    Negate();
  
  // MATHEMATICAL FUNCTIONS

  // Value before the decimal point
  bcd200     Floor() const;
  // Value behind the decimal point
  bcd200     Fraction() const;
  // Value after the decimal point
  bcd200     Ceiling() const;
  // Square root of the bcd200
  bcd200     SquareRoot() const;
  // This bcd200 to the power x
  bcd200     Power(const bcd200& p_power) const;
  // Absolute value (ABS)
  bcd200     AbsoluteValue() const;
  // Reciproke / Inverse = 1/x
  bcd200     Reciprocal() const;
  // Natural logarithm
  bcd200     Log() const;
  // Exponent e tot the power 'this number'
  bcd200     Exp() const;
  // Log with base 10
  bcd200     Log10() const;
  // Ten Power
  bcd200     TenPower(int n);

  // TRIGONOMETRIC FUNCTIONS

  // Sinus of the angle
  bcd200     Sine() const;
  // Cosine of the angle
  bcd200     Cosine() const;
  // Tangent of the angle
  bcd200     Tangent() const;
  // Arc sines (angle) of the ratio
  bcd200     ArcSine() const;
  // Arc cosine (angle) of the ratio
  bcd200     ArcCosine() const;
  // Arctangent (angle) of the ratio
  bcd200     ArcTangent() const;
  // Angle of two points (x,y)
  bcd200     ArcTangent2Points(const bcd200& p_x) const;

  // GET AS SOMETHING DIFFERENT

  // Get as a double
  double  AsDouble() const;
  // Get as a short
  short   AsShort() const;
  // Get as an unsigned short
  ushort  AsUShort() const;
  // Get as a long
  long    AsLong() const;
  // Get as an unsigned long
  ulong   AsULong() const;
  // Get as a 64bits long
  int64   AsInt64() const;
  // Get as an unsigned 64 bits long
  uint64  AsUInt64() const;
  // Get as a mathematical string
  XString AsString(bcd200::Format p_format = Format::Bookkeeping,bool p_printPositive = false,int p_decimals = 2) const;
  // Get as a display string (by desktop locale)
  XString AsDisplayString(int p_decimals = 2) const;
  // Get as an ODBC SQL NUMERIC(p,s)
  void    AsNumeric(SQL_NUMERIC_STRUCT* p_numeric) const;

  // GETTER FUNCTIES

  // Is bcd200 exactly 0.0? (used to be called IsNull)
  bool    IsZero() const; 
  // Is bcd200 a database NULL
  bool    IsNULL() const;
  // Not an (-)INF or a NAN
  bool    IsValid() const;
  // Is bcd200 nearly 0.0 (smaller than epsilon)
  bool    IsNearZero();
  // Gets the sign 0 (= 0.0), 1 (greater than 0) of -1 (smaller than 0)
  int     GetSign() const;
  // Gets Signed status Positive, Negative, -INF, INF, NaN
  Sign    GetStatus() const;
  // Total length (before and after decimal point)
  int     GetLength() const;
  // Total precision (length after the decimal point)
  int     GetPrecision() const;
  // Get the max size of a bcd200
  static int GetMaxSize(int p_precision = 0);
  // Gets the fact that it fits in a long
  bool    GetFitsInLong() const;
  // Gets the fact that it fits in an int64
  bool    GetFitsInInt64() const;
  // Decimal part (behind the decimal point) is not "000" (zeros)
  bool    GetHasDecimals() const;
  // Gets the exponent
  int     GetExponent() const;
  // Gets the mantissa
  bcd200     GetMantissa() const;

  // FILE STREAM FUNCTIONS
  bool    WriteToFile (FILE* p_fp);
  bool    ReadFromFile(FILE* p_fp);

#ifdef _DEBUG
  // Debug print of the mantissa
  XString DebugPrint(PTCHAR p_name);
#endif

private:

  // INTERNALS

  // Set infinity for overflows
  bcd200     SetInfinity(XString p_reason = _T("")) const;
  // Sets one integer in this bcd200 number
  void    SetValueInt(const int p_value);
  // Sets one or two longs in this bcd200 number
  void    SetValueLong(const long p_value, const long p_restValue);
  // Sets one or two longs in this bcd200 number
  void    SetValueInt64 (const  int64 p_value,const int64 p_restValue);
  void    SetValueUInt64(const uint64 p_value,const int64 p_restValue);
  // Sets the value from a double
  void    SetValueDouble(const double p_value);
  // Sets the value from a string
  void    SetValueString(LPCTSTR p_string,bool p_fromDB = false);
  // Sets the value from a SQL NUMERIC
  void    SetValueNumeric(const SQL_NUMERIC_STRUCT* p_numeric);
  // Take the absolute value of a long
  long    long_abs(const long p_value) const;
  // Normalize the mantissa/exponent
  void    Normalize(int p_startExponent = 0);
  // Multiply the mantissa by 10
  void    Mult10(int p_times = 1);
  // Divide the mantissa by 10
  void    Div10(int p_times = 1);
  // Shift mantissa 1 position right
  void    ShiftRight();
  // Shift mantissa 1 position left
  void    ShiftLeft();
  // Convert a string to a single long value
  long    StringToLong(LPCTSTR p_string) const;
  // Convert a long to a string
  XString LongToString(long p_value) const;
  // Split the mantissa for floor/ceiling operations
  bcd200     SplitMantissa() const;
  // Compare two mantissa
  int     CompareMantissa(const bcd200& p_value) const;
  // Calculate the precision and scale for a SQL_NUMERIC
  void    CalculatePrecisionAndScale(SQLCHAR& p_precision,SQLCHAR& p_scale) const;
  // Stopping criterion for internal iterations
  bcd200&    Epsilon(long p_fraction) const;

  // BASIC OPERATIONS

  // Addition operation
  bcd200 Add(const bcd200& p_number) const;
  // Subtraction operation
  bcd200 Sub(const bcd200& p_number) const;
  // Multiplication
  bcd200 Mul(const bcd200& p_number) const;
  // Division
  bcd200 Div(const bcd200& p_number) const;
  // Modulo
  bcd200 Mod(const bcd200& p_number) const;

  // Helpers for the basic operations

  // Position arguments and signs for the next operation
  void PositionArguments(bcd200& arg1,bcd200& arg2,Sign& signResult,Operator& operatorKind) const;
  // Calculate the sign for multiplication or division
  Sign CalculateSign(const bcd200& p_arg1, const bcd200& p_arg2) const;
  // Addition of two mantissa (no signs/exponents)
  bcd200  PositiveAddition(bcd200& arg1,bcd200& arg2) const;
  // Subtraction of two mantissa (no signs/exponents)
  bcd200  PositiveSubtraction(bcd200& arg1,bcd200& arg2) const;
  // Multiplication of two mantissa (no signs)
  bcd200  PositiveMultiplication(const bcd200& p_arg1,const bcd200& p_arg2) const;
  // Division of two mantissa (no signs)
  bcd200  PositiveDivision(bcd200& p_arg1,bcd200& p_arg2) const;

  // STORAGE OF THE NUMBER
  Sign          m_sign;                // 0 = Positive, 1 = Negative (INF, NaN)
  short         m_exponent;            // +/- 10E32767
  long          m_mantissa[bcdLength]; // Up to (bcdDigits * bcdLength) digits
};
