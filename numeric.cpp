//////////////////////////////////////////////////////////////////////////
//
// NUMERIC
//
// Floating Point Precision Number class (NUMERIC / DECIMAL)
// Intended for ISO SQL:9085 type databases
// 
// A number always has the format [sign][[digit]*][.[digit]*][E[sign][digits]+] where sign is either '+' or '-'
// Numbers are stored in 256 radix mantissa of SQL_MAX_NUMERIC_LEN length
// The mantissa array exists of a series of unsigned characters
//
// Copyright (c) 1998-2019 ir W. E. Huisman
// Version 0.1
//
#include "stdafx.h"
#include "numeric.h"
#include "StdException.h"

//////////////////////////////////////////////////////////////////////////
//
// NUMERIC: CONSTRUCTORS AND DESTRUCTOR
//
//////////////////////////////////////////////////////////////////////////

// XTOR: Default constructor on 0.0
numeric::numeric()
{
  Zero();
}

// XTOR from an integer
numeric::numeric(int p_precision,int p_scale,int p_value)
{
  SetPrecisionAndScale(p_precision,p_scale);
  SetValueInteger(p_value);
}


// XTOR from a string
numeric::numeric(int p_precision,int p_scale,const char* p_string,bool p_fromDB /*=false*/)
{
  SetPrecisionAndScale(p_precision,p_scale);
  SetValueString(p_string,p_fromDB);
}

// XTOR: Created from a SQL_NUMERIC_STRUCT
numeric::numeric(SQL_NUMERIC_STRUCT* p_numeric)
{
  // Just copy the numeric struct
  memcpy_s(&m_numeric,sizeof(SQL_NUMERIC_STRUCT),p_numeric,sizeof(SQL_NUMERIC_STRUCT));
}

// DTOR: Nothing interesting to do :-)
numeric::~numeric()
{
}

//////////////////////////////////////////////////////////////////////////
//
// END OF: CONSTRUCTORS AND DESTRUCTOR
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// CONSTANTS
//
//////////////////////////////////////////////////////////////////////////

// Circumference/Radius ratio of a circle
numeric 
numeric::PI(int p_precision,int p_scale)
{
  if(p_precision < 3 || p_precision > 2 * SQL_MAX_NUMERIC_LEN ||
     p_scale     < 2 || p_scale >= p_precision)
  {
    throw new StdException(_T("Numeric: precision and scale out of range"));
  }
  numeric pi;
  pi.SetPrecisionAndScale(p_precision,p_scale);
  uint128 pival(_T("31415926535897932384626433832795"));
  uint128 power = pi.TenPowerScale((2 * SQL_MAX_NUMERIC_LEN) - 1 - p_scale);
  pival /= power;
  pival.AsNumeric(&pi.m_numeric);

  return pi;
}

// Natural logarithm of 2
numeric 
numeric::LN2(int p_precision, int p_scale)
{
  if(p_precision < 3 || p_precision > 2 * SQL_MAX_NUMERIC_LEN ||
     p_scale     < 2 || p_scale >= p_precision)
  {
    throw new StdException(_T("Numeric: precision and scale out of range"));
  }
  numeric ln2;

  ln2.SetPrecisionAndScale(p_precision, p_scale);
  uint128 ln2val(_T("69314718055994530941723212145817"));
  uint128 power = ln2.TenPowerScale((2 * SQL_MAX_NUMERIC_LEN) - p_scale);
  ln2val /= power;
  ln2val.AsNumeric(&ln2.m_numeric);

  return ln2;
}

// Natural logarithm of 10
numeric 
numeric::LN10(int p_precision, int p_scale)
{
  if(p_precision < 3 || p_precision > 2 * SQL_MAX_NUMERIC_LEN ||
     p_scale     < 2 || p_scale >= p_precision)
  {
    throw new StdException(_T("Numeric: precision and scale out of range"));
  }
  numeric ln10;
  ln10.SetPrecisionAndScale(p_precision, p_scale);
  uint128 lnval(_T("23025850929940456840179914546843"));
  uint128 power = ln10.TenPowerScale((2 * SQL_MAX_NUMERIC_LEN) - 1 - p_scale);
  lnval /= power;
  lnval.AsNumeric(&ln10.m_numeric);

  return ln10;
}

//////////////////////////////////////////////////////////////////////////
//
// END OF: CONSTANTS
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// OPERATORS
//
//////////////////////////////////////////////////////////////////////////

// UNARY OPERATORS

// prefix unary minus (negation)
//
numeric
numeric::operator-() const
{
  numeric result(*this);

  // Null can never be negative
  if (!result.IsNull())
  {
    // Swap signs
    if (result.m_numeric.sign == (int)Sign::Positive)
    {
      result.m_numeric.sign = (int)Sign::Negative;
    }
    else
    {
      result.m_numeric.sign = (int)Sign::Positive;
    }
  }
  return result;
}

// DYADIC OPERATORS

numeric
numeric::operator+(const numeric& p_rhs) const
{
  return Add(p_rhs);
}

numeric 
numeric::operator-(const numeric& p_rhs) const
{
  return Sub(p_rhs);
}

numeric 
numeric::operator*(const numeric& p_rhs) const
{
  return Mul(p_rhs);
}

numeric 
numeric::operator/(const numeric& p_rhs) const
{
  return Div(p_rhs);
}

numeric 
numeric::operator%(const numeric& p_rhs) const
{
  return Mod(p_rhs);
}

numeric& 
numeric::operator+=(const numeric& p_rhs)
{
  *this = Add(p_rhs);
  return *this;
}

numeric& 
numeric::operator-=(const numeric& p_rhs)
{
  *this = Sub(p_rhs);
  return *this;
}

numeric& 
numeric::operator*=(const numeric& p_rhs)
{
  *this = Mul(p_rhs);
  return *this;
}

numeric& 
numeric::operator/=(const numeric& p_rhs)
{
  *this = Div(p_rhs);
  return *this;
}

numeric& 
numeric::operator%=(const numeric& p_rhs)
{
  *this = Mod(p_rhs);
  return *this;
}

bool    
numeric::operator==(const numeric& p_rhs) const
{
  // Optimize for different signs
  if(m_numeric.sign != p_rhs.m_numeric.sign)
  {
    return false;
  }
  CheckCompatiblePrecisionAndScale(*this, p_rhs);

  int max1 = (m_numeric.precision + 1) / 2;
  int max2 = (p_rhs.m_numeric.precision + 1) / 2;
  int total = max(max1, max2);

  for (int index = total; index >= 0; --index)
  {
    if (m_numeric.val[index] != p_rhs.m_numeric.val[index])
    {
      return false;
    }
  }
  // Equal -> true
  return true;
}

bool
numeric::operator!=(const numeric& p_rhs) const
{
  // Optimize for different signs
  if (m_numeric.sign != p_rhs.m_numeric.sign)
  {
    return true;
  }
  CheckCompatiblePrecisionAndScale(*this, p_rhs);

  int max1 = (m_numeric.precision + 1) / 2;
  int max2 = (p_rhs.m_numeric.precision + 1) / 2;
  int total = max(max1, max2);

  for (int index = total; index >= 0; --index)
  {
    if (m_numeric.val[index] != p_rhs.m_numeric.val[index])
    {
      return true;
    }
  }
  // Equal -> false
  return false;
}

bool
numeric::operator>(const numeric& p_rhs) const
{
  // Optimize for different signs
  if(m_numeric.sign != p_rhs.m_numeric.sign)
  {
    return m_numeric.sign == (int)Sign::Positive;
  }
  CheckCompatiblePrecisionAndScale(*this,p_rhs);

  int max1 = (m_numeric.precision + 1) / 2;
  int max2 = (p_rhs.m_numeric.precision + 1) / 2;
  int total = max(max1,max2);

  for(int index = total; index >= 0; --index)
  {
    if(m_numeric.val[index] > p_rhs.m_numeric.val[index])
    {
      return true;
    }
    if(m_numeric.val[index] < p_rhs.m_numeric.val[index])
    {
      return false;
    }
  }
  // Equal -> false
  return false;
}

bool
numeric::operator<(const numeric& p_rhs) const
{
  // Optimize for different signs
  if (m_numeric.sign != p_rhs.m_numeric.sign)
  {
    return m_numeric.sign == (int)Sign::Positive;
  }
  CheckCompatiblePrecisionAndScale(*this, p_rhs);

  int max1 = (m_numeric.precision + 1) / 2;
  int max2 = (p_rhs.m_numeric.precision + 1) / 2;
  int total = max(max1, max2);

  for (int index = total; index >= 0; --index)
  {
    if (m_numeric.val[index] < p_rhs.m_numeric.val[index])
    {
      return true;
    }
    if (m_numeric.val[index] > p_rhs.m_numeric.val[index])
    {
      return true;
    }
  }
  // Equal -> false
  return false;
}

bool
numeric::operator>=(const numeric& p_rhs) const
{
  return !(*this < p_rhs);
}

bool
numeric::operator<=(const numeric& p_rhs) const
{
  return !(*this > p_rhs);
}

//////////////////////////////////////////////////////////////////////////
//
// END OF: OPERATORS
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// MATHEMATICAL FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

numeric 
numeric::Floor()
{
  numeric result;
  numeric minusOne(GetPrecision(),GetScale(),-1);

  // Shortcut: If number is null. Floor is always zero
  if (IsNull())
  {
    return result;
  }
  // Find the floor
  SplitMantissa(&result.m_numeric);

  // Take care of sign
  if (m_numeric.sign == (int)Sign::Negative)
  {
    // Floor is 1 smaller
    result += numeric(GetPrecision(),GetScale(),-1);
  }
  return result;
}

numeric
numeric::Add(const numeric& p_other) const
{
  Sign     signResult;
  Operator operatorKind;
  numeric  arg1(*this);
  numeric  arg2(p_other);

  // Check compatibility. throw if not
  CheckCompatiblePrecisionAndScale(arg1,arg2);
  // Figure out what to do
  PositionArguments(arg1,arg2,signResult,operatorKind);

  if (operatorKind == Operator::Addition)
  {
    arg1 = PositiveAddition(arg1,arg2);
  }
  else
  {
    if (arg1 > arg2)
    {
      arg1 = PositiveSubtraction(arg1,arg2);
    }
    else
    {
      arg1 = PositiveSubtraction(arg2,arg1);
      signResult = Sign::Negative;
    }
  }
  arg1.m_numeric.sign = (int)signResult;

  return arg1;
}

// Subtraction operation
numeric 
numeric::Sub(const numeric& p_number) const
{
  // x-y is equal to  x+(-y)
  return *this + (-p_number);
}

// Multiplication
numeric 
numeric::Mul(const numeric& p_number) const
{
  // Multiplication without signs
  numeric result = PositiveMultiplication(*this, p_number);

  // Take care of the sign
  result.m_numeric.sign = (int) (result.IsNull() ? Sign::Positive : CalculateSign(*this, p_number));

  return result;
}

// Division
numeric 
numeric::Div(const numeric& p_number) const
{
  // If divisor is zero -> ERROR
  if (p_number.IsNull())
  {
    throw new StdException(_T("Numeric: Division by zero."));
  }
  // Shortcut: result is zero if this is zero
  if (IsNull())
  {
    return *this;
  }
  // Division without signs
  numeric arg1(*this);
  numeric arg2(p_number);
  numeric result = PositiveDivision(arg1, arg2);

  // Take care of the sign
  result.m_numeric.sign = (int)(result.IsNull() ? Sign::Positive : CalculateSign(*this, p_number));

  return result;
}

// Modulo
numeric 
numeric::Mod(const numeric& p_number) const
{
  numeric count = ((*this) / p_number).Floor();

  //   part  = (*this) / p_number;
  //   floor = part.Floor();
  //   subst = floor * p_number;
  //   answ  = *this - subst;
  // 
  //   part .DebugPrint("Divisor");
  //   floor.DebugPrint("Divisor-floor");
  //   subst.DebugPrint("Subtrahend");
  //   answ .DebugPrint("Answer");

  // Divisor:     240963853305269,14298709106993387
  // Floor:       240963853305269,0
  // Subtrahend: 1234567890123455,3908686065289664
  // Answer:                    0,7325881824833792

  numeric mod((*this) - (count * p_number));
  if (m_numeric.sign == (int)Sign::Negative)
  {
    mod = -mod;
  }
  return mod;
}

//////////////////////////////////////////////////////////////////////////
//
// END OF: MATHEMATICAL FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// MAKING AN EXACT VALUE
//
//////////////////////////////////////////////////////////////////////////

// Setting the number to zero (0.0)
// With a default precision/scale of 32.16
void
numeric::Zero()
{
  // Set default precision and scale
  m_numeric.precision = 2 * SQL_MAX_NUMERIC_LEN;
  m_numeric.scale     =     SQL_MAX_NUMERIC_LEN;
  // Reset val array and sign
  Reset();
}

//////////////////////////////////////////////////////////////////////////
//
// END OF: MAKING AN EXACT VALUE
//
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//
// GET AS SOMETHING DIFFERENT
//
//////////////////////////////////////////////////////////////////////////

// Get as a mathematical string
CString
numeric::AsString(numeric::Format p_format /*= Bookkeeping*/,bool p_printPositive /*= false*/) const
{
  CString thestring;

  if(IsNull())
  {
    thestring = _T("0");
  }
  else
  {
    // Here is the conversion
    uint128 val(&m_numeric);
    thestring = val.AsString();

    // Adjust for scale
    if(m_numeric.scale)
    {
      while(thestring.GetLength() < ((size_t)m_numeric.scale + 1))
      {
        thestring.Insert(0,'0');
      }
      thestring.Insert(thestring.GetLength() - m_numeric.scale,'.');
    }
  }
  // Trim of excessive zeros
  // thestring = thestring.TrimRight('0');

  // Prepare for sign
  if (m_numeric.sign == 0)
  {
    thestring.Insert(0,'-');
  }
  else if (p_printPositive)
  {
    thestring.Insert(0,'+');
  }
  return thestring;
}

// Get as a numeric struct
void
numeric::AsNumeric(SQL_NUMERIC_STRUCT* p_numeric)
{
  if(p_numeric)
  {
    memcpy_s(p_numeric,sizeof(SQL_NUMERIC_STRUCT),&m_numeric,sizeof(SQL_NUMERIC_STRUCT));
  }
}

//////////////////////////////////////////////////////////////////////////
//
// END OF: GET AS SOMETHING DIFFERENT
//
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//
// GETTER FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

// Is exactly 0.0?
bool    
numeric::IsNull() const
{
  // If a negative number or has an exponent: it cannot be zero
  if(m_numeric.sign == 0)
  {
    return false;
  }
  // If one of the values within the precision is 'filled'
  // it cannot be a zero
  int size = (m_numeric.precision + 1 ) / 2;
  for(int index = 0; index < size; ++index)
  {
    if (m_numeric.val[index])
    {
      return false;
    }
  }
  return true;
}

// Getting the precision
int
numeric::GetPrecision() const
{
  return m_numeric.precision;
}

// Getting the scale
int
numeric::GetScale() const
{
  return m_numeric.scale;
}

// Getting the sign
numeric::Sign
numeric::GetSign() const
{
  return (numeric::Sign)m_numeric.sign;
}

//////////////////////////////////////////////////////////////////////////
//
// GETTER FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//
// END OF: GETTER FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// SETTER FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////


// Setting the correct precision and scale (with checks)
void
numeric::SetPrecisionAndScale(int p_precision, int p_scale)
{
  if(p_precision > 0 && p_precision < (2 * SQL_MAX_NUMERIC_LEN))
  {
    m_numeric.precision = p_precision;
  }
  if(p_scale >= 0 && p_scale <= m_numeric.precision)
  {
    m_numeric.scale = p_scale;
  }
}

//////////////////////////////////////////////////////////////////////////
//
// END OF: SETTER FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 
// PRIVATE FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

// Reset the val array, leaving precision and scale intact
void   
numeric::Reset()
{
  m_numeric.sign = 1;  // Positive
  memset(m_numeric.val, 0, SQL_MAX_NUMERIC_LEN);
}

// Sets the value from a string
void
numeric::SetValueString(const char* p_string, bool p_fromDB /*= false*/)
{
  CString string(p_string);

  string = string.TrimLeft(_T(" \t\n\r"));

  // Finding the sign
  m_numeric.sign = 1; // positive
  if(string.GetAt(0) == '-')
  {
    m_numeric.sign = 0; // negative
    string = string.Mid(1);
  }
  if(string.GetAt(0) == '+')
  {
    string = string.Mid(1);
  }

  // Build a uint128 string
  int pos = string.Find('.');
  if(pos >= 0)
  {
    // 1234.123  pos = 4, len = 8
    int found = string.GetLength() - pos - 1;
    int toadd = m_numeric.scale - found;
    if (toadd > 0)
    {
      // Add extra zeros
      string.Append(_T("0"),toadd);
    }
    else if (toadd < 0)
    {
      // Remove excessive zeros
      string = string.Left(string.GetLength() + toadd);
    }
    string.Remove('.');
  }
  else if(m_numeric.scale > 0)
  {
    // No decimal point found
    for(int ind = 0; ind < m_numeric.scale; ++ind)
    {
      string.Append(_T("0"));
    }
  }

  // Convert to SQL_NUMERIC_STRUCT
  uint128 value(string.GetString());
  value.AsNumeric(&m_numeric);
}

// Sets the value from an integer
void    
numeric::SetValueInteger(int p_integer)
{
  // Take care of the sign
  if (p_integer < 0)
  {
    p_integer = -p_integer;
    m_numeric.sign = (int)Sign::Negative;

  }
  else
  {
    m_numeric.sign = (int)Sign::Positive;
  }
  // Positive 128-bits value
  uint128 val(p_integer);

  // Take care of the scale
  val *= TenPowerScale(GetScale());

  // Convert to val array
  val.AsNumeric(&m_numeric);
}

// Check if we can operate on these two numerics
void    
numeric::CheckCompatiblePrecisionAndScale(const numeric& p_lhs,const numeric& p_rhs) const
{
  if(p_lhs.m_numeric.scale != p_rhs.m_numeric.scale)
  {
    throw new StdException(_T("Cannot operate on numerics of different scale"));
  }
  if(p_lhs.m_numeric.precision < p_rhs.m_numeric.precision)
  {
    throw new StdException(_T("Cannot operate on numerics with a greater precision on the right-hand-side"));
  }
}

// Position arguments and signs for the next operation
void 
numeric::PositionArguments(numeric& p_arg1
                          ,numeric& p_arg2
                          ,Sign&    p_signResult
                          ,Operator& p_operatorKind) const
{
  // Get the resulting kind of operator and sign
  // if (-x) + y then turnaround, so x + (-y), becomes x - y
  if(p_arg1.m_numeric.sign == (int)Sign::Positive)
  {
    if(p_arg2.m_numeric.sign == (int)Sign::Positive)
    {
      p_signResult   = Sign::Positive;
      p_operatorKind = Operator::Addition;
    }
    else
    {
      p_signResult   = Sign::Positive;
      p_operatorKind = Operator::Subtraction;
    }
  }
  else
  {
    if(p_arg2.m_numeric.sign == (int)Sign::Positive)
    {
      p_signResult   = Sign::Positive;
      p_operatorKind = Operator::Subtraction;
      // Swap arguments
      numeric dummy(p_arg1);
      p_arg1 = p_arg2;
      p_arg2 = dummy;
    }
    else
    {
      p_signResult   = Sign::Negative;
      p_operatorKind = Operator::Addition;
    }
  }
  // Now the rest is a positive operation
  p_arg1.m_numeric.sign = (int)Sign::Positive;
  p_arg2.m_numeric.sign = (int)Sign::Positive;
}

// Calculate the sign for multiplication or division
numeric::Sign    
numeric::CalculateSign(const numeric& p_arg1, const numeric& p_arg2) const
{
  // Find the sign for multiplication / division
  // (+x) * (+y) -> positive
  // (-x) * (+y) -> negative
  // (-x) * (-y) -> positive
  // (+x) * (-y) -> negative
  if (p_arg1.IsNull() || p_arg2.IsNull())
  {
    return Sign::Positive;
  }
  if (p_arg1.m_numeric.sign != p_arg2.m_numeric.sign)
  {
    return Sign::Negative;
  }
  return Sign::Positive;
}

// Addition of two mantissa (no signs/exponents)
numeric 
numeric::PositiveAddition(numeric& arg1, numeric& arg2) const
{
  int ind1 = (arg1.m_numeric.precision + 1) / 2;
  int ind2 = (arg2.m_numeric.precision + 1) / 2;
  int ind  = max(ind1,ind2);

  int accu  = 0;
  int carry = 0;
  const int radix = 256;

  for(int index = 0; index < ind; ++index)
  {
    accu  = arg1.m_numeric.val[index] + arg2.m_numeric.val[index] + carry;
    carry = accu / radix;
    accu %= radix;

    arg1.m_numeric.val[index] = (SQLCHAR) accu;
  }
  return arg1;
}

// Subtraction of two mantissa (no signs/exponents)
numeric 
numeric::PositiveSubtraction(numeric& p_arg1,numeric& p_arg2) const
{
  uint128 arg1(&p_arg1.m_numeric);
  uint128 arg2(&p_arg2.m_numeric);

  arg1 -= arg2;
  numeric result;
  result.SetPrecisionAndScale(p_arg1.m_numeric.precision, p_arg1.m_numeric.scale);
  arg1.AsNumeric(&result.m_numeric);

  return p_arg1;
}

// Multiplication of two mantissa (no signs)
numeric 
numeric::PositiveMultiplication(const numeric& p_arg1, const numeric& p_arg2) const
{
  uint128 arg1(&p_arg1.m_numeric);
  uint128 arg2(&p_arg2.m_numeric);

  arg1 *= arg2;
  numeric result;
  result.SetPrecisionAndScale(p_arg1.m_numeric.precision, p_arg1.m_numeric.scale);
  arg1.AsNumeric(&result.m_numeric);

  return result;
}

// Division of two mantissa (no signs)
numeric 
numeric::PositiveDivision(numeric& p_arg1, numeric& p_arg2) const
{
  uint128 arg1(&p_arg1.m_numeric);
  uint128 arg2(&p_arg2.m_numeric);

  arg1 /= arg2;
  numeric result;
  result.SetPrecisionAndScale(p_arg1.m_numeric.precision, p_arg1.m_numeric.scale);
  arg1.AsNumeric(&result.m_numeric);

  return result;
}

// Split the mantissa for floor/ceiling operations
void
numeric::SplitMantissa(SQL_NUMERIC_STRUCT* p_numeric) const
{
  // Get 128 bits values
  uint128 power = TenPowerScale(GetScale());
  uint128 value(p_numeric);

  // Get rid of the decimal part
  value /= power;
  value *= power;

  value.AsNumeric(p_numeric);
}

// Calculate the 10-power for the scaling
uint128 
numeric::TenPowerScale(int p_scale) const
{
  // Take care of the scale
  uint128 power(1);
  for (int index = 0; index < p_scale; ++index)
  {
    power *= 10;
  }
  return power;
}

//////////////////////////////////////////////////////////////////////////
//
// END OF: PRIVATE FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////
