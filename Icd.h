// ICD = INTEGER CODED DECIMAL
// A class to work with large decimal numbers
// in exact precisions in decimals as opposed to
// the built in datatypes (float, double) that
// do a binary approximation 
//
#pragma once
#include <sqltypes.h>
#include <ostream>

using std::ostream;

static const long  icdBase          = 100000000L; // Base   per element of m_data
static const short icdDigits        = 8;          // Digits per element of m_data
static const short icdLength        = 10;         // Total storage length in m_data
static const short icdPointPosition = 5;          // Implied position of the decimal '.'
static const short icdPrecision     = 38;         // Maximum allowed precision in ODBC
// icd now has the form of: 0 0 0 0 0. 0 0 0 0 0
// Example: 2.5 is stored:  0 0 0 0 5. 2 0 0 0 0

// All internal registers are in 64 bits
#define int64 __int64

class icd
{
public:
  // Default constructor.
  icd();

  // ICD of a long
  icd(const long value, const long remainder = 0);

  // ICD of a 64bits int
  icd(const int64 value,const int64 remainder = 0);

  // Copy constructor.
  icd(const icd& icd);

  // ICD of a double
  icd(const double value);

  // Assignment-constructor from a string.
  icd(const CString& p_string);

  // Assignment constructor from ODBC
  icd(SQL_NUMERIC_STRUCT* p_numeric);

  // Destructor of the icd
  ~icd();

  // CONSTANTS

  static icd PI();     // Ratio between radius and circumference of a circle
  static icd LN2();    // Natural logarithm of 2
  static icd LN10();   // Natural logarithm of 10

  // OPERATORS

  const icd& operator=( const icd& icd);
  const icd& operator=( const long value);
  const icd& operator=( const double value);
  const icd& operator=( const CString& str);
  const icd& operator+=(const icd& icd);
  const icd& operator-=(const icd& icd);
  const icd& operator*=(const icd& icd);
  const icd& operator/=(const icd& icd);
  // Prefix unary minus (negation)
  const icd  operator-() const;
  // Postfix increment
  const icd operator++(int);
  // Prefix increment
  icd& operator++();
  // Postfix decrement
  const icd operator--(int);
  // Prefix decrement
  icd& operator--();

  // Comparison operators
  const bool operator==(const icd& icd) const;
  const bool operator!=(const icd& icd) const;
  const bool operator< (const icd& icd) const;
  const bool operator> (const icd& icd) const;
  const bool operator<=(const icd& icd) const;
  const bool operator>=(const icd& icd) const;

  // standard output operator
  friend ostream& operator<<(ostream& os, const icd& icd);

  // MATHEMATICAL FUNCTIONS

  // Floor function
  icd     Floor() const;
  // Ceiling function
  icd     Ceil() const;
  // Square root
  icd     SquareRoot() const;
  // To the power of 
  icd     Power(const icd& macht) const;
  // Absolute value (ABS)
  icd     AbsoluteValue() const;
  // Reciproke / Inverse = 1/x
  icd     Reciproke() const;
  // Natural logarithm
  icd     Log() const;
  // Exponent e till the power of this number
  icd     Exp() const;
  // Log with base 10
  icd     Log10() const;

  // TRIGONOMETRICAL FUNCTIONS
  
  // Sine of an angle
  icd     Sine() const;
  // Cosine of an angle
  icd     Cosine() const;
  // Tangent of an angle
  icd     Tangent() const;
  // Arcsine (angle) of a Sine ratio
  icd     ArcSine() const;
  // Arccosine (angle) of a Cosine ratio
  icd     ArcCosine() const;
  // Arctangent (angle) of a Tangent ratio
  icd     ArcTangent() const;
  // Angle from 2 points
  icd     ArcTangent2Points(icd p_x) const;

  // REQUEST AS SOMETHING DIFFERENT
  // Part before the decimal point (floor if greater than 0)
  icd     ValueBeforePoint() const;
  // Part behind the decimal point
  icd     ValueBehindPoint() const;
  // Get as a converted double
  double  AsDouble() const;
  // Get as a converted long
  long    AsLong() const;
  // Get as a converted int-64
  int64   AsInt64() const;
  // Get as mathematical string
  CString AsString() const;
  // Get as a SQL string 
  CString AsSQLString(bool p_strip = false) const;
  // Get as a display string (for UI purposes)
  CString AsDisplayString() const;
  // Get as an ODBC SQL NUMERIC
  void    AsNumeric(SQL_NUMERIC_STRUCT* p_numeric) const;

  // CHANGE PRECISION

  // Change length and precision
  void  SetLengthAndPrecision(int length = 80, int precision = 40);
  // Rounding on decimals and precision
  long  RoundDecimals(long decimal,int precsion);
  // Correct mathematical rounding
  void  Rounding(int p_length /*= 80*/,int p_precision /*= 40*/);

  // GETTER FUNCTIES

  // Is the value of the icd exactly 0.0 ?
  // With no distinction between +0 en -0
  bool  IsNull() const; 
  // Getting the sign as 0 (= 0.0), 1 (greater than 0) or -1 (smaller than 0)
  int   GetSign() const;
  // Total length (before and after the decimal .)
  int   Length();
  // Precision after the decimal '.'
  int   Precision();
  // Get the max size of an icd
  static int GetMaxSize(int precision = 0);
  // See if the icd fits into a long
  bool  FitsInLong() const;
  // See if the icd fits into a int64
  bool  FitsInInt64() const;
  // See if the icd has decimals after the '.' (no scalar)
  bool  HasDecimals() const;
  // Getting the exponent of the icd
  int   Exponent() const;
  // Getting the 10th power mantissa of the icd
  icd   Mantissa() const;

  // BASIC OPERATIONS.
  // Has to be public for the operators

  // Add operation
  icd Add(const icd& icd) const;
  // Subtraction operation
  icd Sub(const icd& icd) const;
  // Multiply operation
  icd Mul(const icd& icd) const;
  // Dividing operation
  icd Div(const icd& icd) const;
  // Modulo operation
  icd Mod(const icd& icd) const;

private:
  enum Sign          { Positive, Negative    };
  enum OperatorKind  { Adding,   Subtracting };
  
  // Our own abs(long) function
  long long_abs(const long value) const;
  // Our own long to the power 1,2,3.. function
  long long_pow(long base,int operand) const;
  // Our own log10 function
  long long_log10(long value) const;
  
  // Put a long in the icd
  void SetValueLong  (const long value, const long remainder);
  // Put a double in the icd
  void SetValueDouble(const double value);
  // Convert a string in the icd
  void SetValueString(CString value);
  // Put an int64 in the icd
  void SetValueInt64 (const int64 value, const int64 remainder);
  // Put a SQL_NUMERIC_STRUCT from the ODBC driver in the icd
  void SetValueNumeric(SQL_NUMERIC_STRUCT* p_numeric);
  // To the 10th power
  void CalculateEFactor(int factor);
  // Internal reformatting of m_data so that members are < icdBase
  void Reformat();
  // Empty icd to be 0.0
  void MakeEmpty();
  // Internal multiply icd with one icd m_data member
  const icd Multi(const long multiplier) const;
  // Corner case where the round brings icd outside the precision
  const bool IsCornerCase() const;
  // Getting the sign and alignments for a basic operation
  static void PositionArguments(icd& arg1,
                                icd& arg2,
                                Sign& signResult,
                                OperatorKind& kindOfOperator);
  // Adds two icd's without taking the sign into account
  static const icd  AddPositive(const icd& arg1, const icd& arg2);
  // Subtracts two icd's without taking the sign into account
  static const icd  SubtractPositive(const icd& arg1, const icd& arg2);
  // Multiplies two icd's without taking the sign into account
  static const icd  MultiplyPositive(const icd& arg1, const icd& arg2);
  // Divide an icd by another icd without taking the sign into account
  static const icd  DividePositive(const icd& arg1, const icd& arg2);
  // Getting the sign of a multiplication or a division
  static const Sign CalculateSign(const icd& arg1, const icd& arg2);
  // Bringing sides to the same exponent
  static const icd  BringTogether(const icd& arg1, const long verschil);
  // Divide the mantissa by 10 (shift operation)
  void Div10();
  // Multiply the mantissa by 10 (shift operation)
  void Mult10();

  // String <-> Long conversions
  CString LongToString(long p_number) const;
  long    StringToLong(CString& p_string) const;

  // Breaking criterion for internal iterations
  icd&    Epsilon(long p_fraction) const;

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
icd::Length()
{
  return m_length;
}

inline int
icd::Precision()
{
  return m_precision;
}

inline int 
icd::GetMaxSize(int precisie)
{
  return ((icdLength * 2) + precisie); 
}

// Overloaded basic operators

inline icd operator + (const icd& lhs, const icd& rhs)
{ 
  return lhs.Add(rhs); 
}

inline icd operator - (const icd& lhs, const icd& rhs)
{ 
  return lhs.Sub(rhs); 
}

inline icd operator * (const icd& lhs, const icd& rhs)
{ 
  return lhs.Mul(rhs); 
}

inline icd operator / (const icd& lhs, const icd& rhs)
{ 
  return lhs.Div(rhs); 
}

inline icd operator % (const icd& lhs, const icd& rhs)
{ 
  return lhs.Mod(rhs); 
}

// Overloaded math precision floating point functions equivalent with the std C functions
// Overloaded to work with the ICD number class, always yielding a icd number.

inline icd modf(icd p_number, icd* p_intpart)
{
  *p_intpart = p_number.ValueBeforePoint();
  return p_number.ValueBehindPoint();
}

inline icd fmod(icd p_number,icd p_divisor)
{
  return p_number.Mod(p_divisor);
}

inline icd floor(icd p_number)
{
  return p_number.Floor();
}

inline icd ceil(icd p_number)
{
  return p_number.Ceil();
}

inline icd fabs(icd p_number)
{
  return p_number.AbsoluteValue();
}

inline icd sqrt(icd p_number)
{
  return p_number.SquareRoot();
}

inline icd log10(icd p_number)
{
  return p_number.Log10();
}

inline icd log(icd p_number)
{
  return p_number.Log();
}

inline icd exp(icd p_number)
{
  return p_number.Exp();
}

inline icd pow(icd p_number,icd  p_power)
{
  return p_number.Power(p_power);
}

inline icd frexp(icd p_number,int* p_exponent)
{
  *p_exponent = p_number.Exponent();
  return p_number.Mantissa();
}

inline icd ldexp(icd p_number,int p_power)
{
  if(p_power == 0)
  {
    return p_number;
  }
  if(p_power > 0 && p_power <= 31)
  {
    return p_number * icd((long) (1 << p_power));
  }
  return p_number * pow(icd(2L),icd((long)p_power));
}

// Overloaded trigonometric functions on a icd number

inline icd atan (icd p_number) 
{ 
  return p_number.ArcTangent(); 
}

inline icd atan2(icd p_y,icd p_x)
{
  return p_y.ArcTangent2Points(p_x);
}

inline icd asin(icd p_number)
{
  return p_number.ArcSine();
}

inline icd acos(icd p_number)
{
  return p_number.ArcCosine();
}

inline icd sin(icd p_number)
{
  return p_number.Sine();
}

inline icd cos(icd p_number)
{
  return p_number.Cosine();
}

inline icd tan(icd p_number)
{
  return p_number.Tangent();
}
