// ICD208 = INTEGER CODED DECIMAL
// A class to work with large decimal numbers
// in exact precisions in decimals as opposed to
// the built in data types (float, double) that
// do a binary approximation 
//
#pragma once
#include <sqltypes.h>
#include <ostream>

using std::ostream;

static const long  icdBase          = 100000000L; // Base   per element of m_data
static const short icdDigits        = 8;          // Digits per element of m_data
static const short icdPrecision     = 38;         // Maximum allowed precision in ODBC
//////////////////////////////////////////////////////////////////////////
// [208]
// ICD200 changes over ICD
static const short icdLength        = 26;         // Total storage length in m_data
static const short icdPointPosition = 13;         // Implied position of the decimal '.'
//////////////////////////////////////////////////////////////////////////
// icd208 now has the form of: 0 0 0 0 0. 0 0 0 0 0
// Example: 2.5 is stored    : 0 0 0 0 5. 2 0 0 0 0

// Handy typedefs of used basic data types
using uchar  = unsigned char;
using ushort = unsigned short;
using ulong  = unsigned long;

// All internal registers are in 64 bits
#define int64 __int64

class icd208
{
public:
  // Default constructor.
  icd208();

  // ICD of a long
  icd208(const long value, const long remainder = 0);

  // ICD of an unsigned long
  icd208(const ulong value, const ulong remainder = 0);

  // ICD of a 64bits int
  icd208(const int64 value,const int64 remainder = 0);

  // ICD of a 64bits int
  icd208(const UINT64 value, const UINT64 remainder = 0);

  // Copy constructor.
  icd208(const icd208& icd208);

  // ICD of a double
  icd208(const double value);

  // Assignment-constructor from a string.
  icd208(const CString& p_string);

  // ICD of a signed byte/char
  icd208(const char value);

  // ICD of a signed byte/char
  icd208(const uchar value);

  // ICD of a short
  icd208(const short value);

  // ICD of an unsigned short
  icd208(const ushort value);

  // Assignment constructor from ODBC
  icd208(SQL_NUMERIC_STRUCT* p_numeric);

  // Destructor of the icd208
  ~icd208();

  // CONSTANTS

  static icd208 PI();     // Ratio between radius and circumference of a circle
  static icd208 LN2();    // Natural logarithm of 2
  static icd208 LN10();   // Natural logarithm of 10

  // OPERATORS

  const icd208& operator=( const icd208& p_icd);
  const icd208& operator=( const long value);
  const icd208& operator=( const double value);
  const icd208& operator=( const CString& str);
  const icd208& operator+=(const icd208& p_icd);
  const icd208& operator-=(const icd208& p_icd);
  const icd208& operator*=(const icd208& p_icd);
  const icd208& operator/=(const icd208& p_icd);
  const icd208& operator%=(const icd208& p_icd);
  // Prefix unary minus (negation)
  const icd208  operator-() const;
  // Postfix increment
  const icd208 operator++(int);
  // Prefix increment
  icd208& operator++();
  // Postfix decrement
  const icd208 operator--(int);
  // Prefix decrement
  icd208& operator--();

  // Comparison operators
  const bool operator==(const icd208& p_icd) const;
  const bool operator!=(const icd208& p_icd) const;
  const bool operator< (const icd208& p_icd) const;
  const bool operator> (const icd208& p_icd) const;
  const bool operator<=(const icd208& p_icd) const;
  const bool operator>=(const icd208& p_icd) const;

  // standard output operator
  friend ostream& operator<<(ostream& os, const icd208& p_icd);

  // MATHEMATICAL FUNCTIONS

  // Floor function
  icd208     Floor() const;
  // Ceiling function
  icd208     Ceil() const;
  // Square root
  icd208     SquareRoot() const;
  // To the power of 
  icd208     Power(const icd208& macht) const;
  // Absolute value (ABS)
  icd208     AbsoluteValue() const;
  // Reciproke / Inverse = 1/x
  icd208     Reciproke() const;
  // Natural logarithm
  icd208     Log() const;
  // Exponent e till the power of this number
  icd208     Exp() const;
  // Log with base 10
  icd208     Log10() const;
  // Ten Power
  icd208     TenPower(int n);

  // TRIGONOMETRICAL FUNCTIONS
  
  // Sine of an angle
  icd208     Sine() const;
  // Cosine of an angle
  icd208     Cosine() const;
  // Tangent of an angle
  icd208     Tangent() const;
  // Arcsine (angle) of a Sine ratio
  icd208     ArcSine() const;
  // ArcCosine (angle) of a Cosine ratio
  icd208     ArcCosine() const;
  // Arctangent (angle) of a Tangent ratio
  icd208     ArcTangent() const;
  // Angle from 2 points
  icd208     ArcTangent2Points(icd208 p_x) const;

  // REQUEST AS SOMETHING DIFFERENT
  // Part before the decimal point (floor if greater than 0)
  icd208     ValueBeforePoint() const;
  // Part behind the decimal point
  icd208     ValueBehindPoint() const;
  // Get as a converted double
  double     AsDouble() const;
  // Get as a short
  short      AsShort() const;
  // Get as an unsigned short
  ushort     AsUShort() const;
  // Get as a converted long
  long       AsLong() const;
  // Get as an unsigned long
  ulong      AsULong() const;
  // Get as a converted int-64
  int64      AsInt64() const;
  // Get as a unsigned int64
  unsigned int64  AsUInt64() const;
  // Get as mathematical string
  CString    AsString() const;
  // Get as a SQL string 
  CString    AsSQLString(bool p_strip = false) const;
  // Get as a display string (for UI purposes)
  CString    AsDisplayString() const;
  // Get as an ODBC SQL NUMERIC
  void       AsNumeric(SQL_NUMERIC_STRUCT* p_numeric) const;

  // CHANGE PRECISION

  // Change length and precision
  void  SetLengthAndPrecision(int length = 200, int precision = 100);
  // Rounding on decimals and precision
  long  RoundDecimals(long decimal,int precsion);
  // Correct mathematical rounding
  void  Rounding(int p_length /*= 80*/,int p_precision /*= 40*/);

  // GETTER FUNCTIES

  // Is the value of the icd208 exactly 0.0 ?
  // With no distinction between +0 en -0
  bool  IsNull() const; 
  // Getting the sign as 0 (= 0.0), 1 (greater than 0) or -1 (smaller than 0)
  int   GetSign() const;
  // Change the sign
  void  Negate();
  // Total length (before and after the decimal .)
  int   Length();
  // Precision after the decimal '.'
  int   Precision();
  // Get the max size of an icd208
  static int GetMaxSize(int precision = 0);
  // See if the icd208 fits into a long
  bool  FitsInLong() const;
  bool  FitsInULong() const;
  // See if the icd208 fits into a int64
  bool  FitsInInt64() const;
  bool  FitsInUInt64() const;
  // See if the icd208 has decimals after the '.' (no scalar)
  bool  HasDecimals() const;
  // Getting the exponent of the icd208
  int   Exponent() const;
  // Getting the 10th power mantissa of the icd208
  icd208 Mantissa() const;
  // Nearly zero or zero
  bool  IsNearZero();

  // BASIC OPERATIONS.
  // Has to be public for the operators

  // Make ZERO
  void   Zero();
  // Add operation
  icd208 Add(const icd208& p_icd) const;
  // Subtraction operation
  icd208 Sub(const icd208& p_icd) const;
  // Multiply operation
  icd208 Mul(const icd208& p_icd) const;
  // Dividing operation
  icd208 Div(const icd208& p_icd) const;
  // Modulo operation
  icd208 Mod(const icd208& p_icd) const;

private:
  enum Sign          { Positive, Negative    };
  enum OperatorKind  { Adding,   Subtracting };
  
  // Our own abs(long) function
  long long_abs (const long  value) const;
  // Our own long to the power 1,2,3.. function
  long long_pow(long base,int operand) const;
  // Our own log10 function
  long long_log10(long value) const;
  
  // Put a long in the icd208
  void SetValueLong  (const long  value,const long  remainder);
  void SetValueULong (const ulong value,const ulong remainder);
  // Put a double in the icd208
  void SetValueDouble(const double value);
  // Convert a string in the icd208
  void SetValueString(CString value);
  // Put an int64 in the icd208
  void SetValueInt64 (const int64  value,const int64  remainder);
  void SetValueUInt64(const UINT64 value,const UINT64 remainder);
  // Put a SQL_NUMERIC_STRUCT from the ODBC driver in the icd208
  void SetValueNumeric(SQL_NUMERIC_STRUCT* p_numeric);
  // To the 10th power
  void CalculateEFactor(int factor);
  // Internal reformatting of m_data so that members are < icdBase
  void Reformat();
  // Empty icd208 to be 0.0
  void MakeEmpty();
  // Internal multiply icd208 with one icd208 m_data member
  const icd208 Multi(const long multiplier) const;
  // Corner case where the round brings icd208 outside the precision
  const bool IsCornerCase() const;
  // Getting the sign and alignments for a basic operation
  static void PositionArguments(icd208& arg1,
                                icd208& arg2,
                                Sign& signResult,
                                OperatorKind& kindOfOperator);
  // Adds two icd208's without taking the sign into account
  static const icd208  AddPositive(const icd208& arg1, const icd208& arg2);
  // Subtracts two icd208's without taking the sign into account
  static const icd208  SubtractPositive(const icd208& arg1, const icd208& arg2);
  // Multiplies two icd208's without taking the sign into account
  static const icd208  MultiplyPositive(const icd208& arg1, const icd208& arg2);
  // Divide an icd208 by another icd208 without taking the sign into account
  static const icd208  DividePositive(const icd208& arg1, const icd208& arg2);
  // Getting the sign of a multiplication or a division
  static const Sign CalculateSign(const icd208& arg1, const icd208& arg2);
  // Bringing sides to the same exponent
  static const icd208  BringTogether(const icd208& arg1, const long verschil);
  // Divide the mantissa by 10 (shift operation)
  void Div10();
  // Multiply the mantissa by 10 (shift operation)
  void Mult10();

  // String <-> Long conversions
  CString LongToString(long p_number) const;
  long    StringToLong(CString& p_string) const;

  // Breaking criterion for internal iterations
  icd208&    Epsilon(long p_fraction) const;
  //
  // Data members: Storing the number
  //
  Sign          m_sign;
  int           m_length;
  int           m_precision;
  unsigned long m_data[icdLength];
};

// Getters

inline int
icd208::Length()
{
  return m_length;
}

inline int
icd208::Precision()
{
  return m_precision;
}

inline int 
icd208::GetMaxSize(int precisie)
{
  return (icdLength * icdDigits); 
}

// Overloaded basic operators

inline icd208 operator + (const icd208& lhs, const icd208& rhs)
{ 
  return lhs.Add(rhs); 
}

inline icd208 operator - (const icd208& lhs, const icd208& rhs)
{ 
  return lhs.Sub(rhs); 
}

inline icd208 operator * (const icd208& lhs, const icd208& rhs)
{ 
  return lhs.Mul(rhs); 
}

inline icd208 operator / (const icd208& lhs, const icd208& rhs)
{ 
  return lhs.Div(rhs); 
}

inline icd208 operator % (const icd208& lhs, const icd208& rhs)
{ 
  return lhs.Mod(rhs); 
}

// Overloaded math precision floating point functions equivalent with the std C functions
// Overloaded to work with the ICD number class, always yielding a icd208 number.

inline icd208 modf(icd208 p_number, icd208* p_intpart)
{
  *p_intpart = p_number.ValueBeforePoint();
  return p_number.ValueBehindPoint();
}

inline icd208 fmod(icd208 p_number,icd208 p_divisor)
{
  return p_number.Mod(p_divisor);
}

inline icd208 floor(icd208 p_number)
{
  return p_number.Floor();
}

inline icd208 ceil(icd208 p_number)
{
  return p_number.Ceil();
}

inline icd208 fabs(icd208 p_number)
{
  return p_number.AbsoluteValue();
}

inline icd208 sqrt(icd208 p_number)
{
  return p_number.SquareRoot();
}

inline icd208 log10(icd208 p_number)
{
  return p_number.Log10();
}

inline icd208 log(icd208 p_number)
{
  return p_number.Log();
}

inline icd208 exp(icd208 p_number)
{
  return p_number.Exp();
}

inline icd208 pow(icd208 p_number,icd208  p_power)
{
  return p_number.Power(p_power);
}

inline icd208 frexp(icd208 p_number,int* p_exponent)
{
  *p_exponent = p_number.Exponent();
  return p_number.Mantissa();
}

inline icd208 ldexp(icd208 p_number,int p_power)
{
  if(p_power == 0)
  {
    return p_number;
  }
  if(p_power > 0 && p_power <= 31)
  {
    return p_number * icd208((long) (1 << p_power));
  }
  return p_number * pow(icd208(2L),icd208((long)p_power));
}

// Overloaded trigonometric functions on a icd208 number

inline icd208 atan (icd208 p_number) 
{ 
  return p_number.ArcTangent(); 
}

inline icd208 atan2(icd208 p_y,icd208 p_x)
{
  return p_y.ArcTangent2Points(p_x);
}

inline icd208 asin(icd208 p_number)
{
  return p_number.ArcSine();
}

inline icd208 acos(icd208 p_number)
{
  return p_number.ArcCosine();
}

inline icd208 sin(icd208 p_number)
{
  return p_number.Sine();
}

inline icd208 cos(icd208 p_number)
{
  return p_number.Cosine();
}

inline icd208 tan(icd208 p_number)
{
  return p_number.Tangent();
}
