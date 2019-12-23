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
#pragma once
#include "uint128.h"
#include <sqltypes.h>

using std::string;

// The NUMERIC_STRUCT makes 38 decimal places possible
#ifndef SQLNUM_MAX_PREC
#define SQLNUM_MAX_PREC 38
#endif

// Handy typedefs of used basic datatypes
using uchar  = unsigned char;
using ushort = unsigned short;
using ulong  = unsigned long;

#ifndef int64
using int64 = __int64;
#endif

#ifndef uint64
using uint64 = unsigned __int64;
#endif

//////////////////////////////////////////////////////////////////////////

class numeric
{
public:
  enum class Format   { Engineering,  Bookkeeping  };
  enum class Sign     { Negative = 0, Positive = 1 };
  enum class Operator { Addition,     Subtraction  };

  // CONSTRUCTORS/DESTRUCTORS

  // Default constructor.
  numeric();
  // Constructor from an integer
  numeric(int p_precision,int p_scale,int p_value);
  // Constructor from a string
  numeric(int p_precision,int p_scale,const char* p_string,bool p_fromDB = false);
  // Constructor from an SQL_NUMERIC_STRUCT
  numeric(SQL_NUMERIC_STRUCT* p_numeric);
  // Default destructor
  ~numeric();

  // CONSTANTS

  static numeric PI  (int p_precision,int p_scale); // Circumference/Radius ratio of a circle
  static numeric LN2 (int p_precision,int p_scale); // Natural logarithm of 2
  static numeric LN10(int p_precision,int p_scale); // Natural logarithm of 10

  // OPERATORS

  // Unary operators
  numeric operator-() const;

  // Standard arithmetic operators
  numeric  operator+(const numeric& p_rhs) const;
  numeric  operator-(const numeric& p_rhs) const;
  numeric  operator*(const numeric& p_rhs) const;
  numeric  operator/(const numeric& p_rhs) const;
  numeric  operator%(const numeric& p_rhs) const;

  numeric& operator+=(const numeric& p_rhs);
  numeric& operator-=(const numeric& p_rhs);
  numeric& operator*=(const numeric& p_rhs);
  numeric& operator/=(const numeric& p_rhs);
  numeric& operator%=(const numeric& p_rhs);

  bool     operator >(const numeric& p_rhs) const;
  bool     operator <(const numeric& p_rhs) const;
  bool     operator>=(const numeric& p_rhs) const;
  bool     operator<=(const numeric& p_rhs) const;
  bool     operator==(const numeric& p_rhs) const;
  bool     operator!=(const numeric& p_rhs) const;

  // MATHEMATICAL FUNCTIONS

  numeric Floor();

  // MAKING AN EXACT NUMERIC value

  // Set the mantissa/exponent/sign to the number zero (0)
  void    Zero();

  // GET AS SOMETHING DIFFERENT

  // Get as a mathematical string
  CString AsString(Format p_format = Format::Bookkeeping, bool p_printPositive = false) const;
  // Get as a numeric struct
  void    AsNumeric(SQL_NUMERIC_STRUCT* p_numeric);

  // GETTER FUNCTIONS

  // Is exactly 0.0?
  bool    IsNull() const;
  // Getting the precision
  int     GetPrecision() const;
  // Getting the scale
  int     GetScale() const;
  // Getting the sign
  Sign    GetSign() const;

  // SETTER FUNCTIONS

  // Setting the correct precision and scale (with checks)
  void    SetPrecisionAndScale(int p_precision,int p_scale);

private:
  // Sets the value from a string
  void    SetValueString(const char* p_string, bool p_fromDB = false);
  // Sets the value from an integer
  void    SetValueInteger(int p_integer);
  // Reset the val array, leaving precision and scale intact
  void    Reset();
  // Check if we can operate on these two numerics
  void    CheckCompatiblePrecisionAndScale(const numeric& p_lhs,const numeric& p_rhs) const;
  // Split the mantissa for floor/ceiling operations
  void    SplitMantissa(SQL_NUMERIC_STRUCT* p_numeric) const;
  // Calculate the 10-power for the scaling
  uint128 TenPowerScale(int p_scale) const;

  // BASIC OPERATIONS

  // Add two numerics together
  numeric Add(const numeric& p_number) const;
  // Subtraction operation
  numeric Sub(const numeric& p_number) const;
  // Multiplication
  numeric Mul(const numeric& p_number) const;
  // Division
  numeric Div(const numeric& p_number) const;
  // Modulo
  numeric Mod(const numeric& p_number) const;

  // Helpers for the basic operations

  // Position arguments and signs for the next operation
  void    PositionArguments(numeric& p_arg1,numeric& p_arg2,Sign& p_signResult,Operator& p_operatorKind) const;
  // Calculate the sign for multiplication or division
  Sign    CalculateSign(const numeric& p_arg1, const numeric& p_arg2) const;
  // Addition of two mantissa (no signs/exponents)
  numeric PositiveAddition(numeric& arg1, numeric& arg2) const;
  // Subtraction of two mantissa (no signs/exponents)
  numeric PositiveSubtraction(numeric& arg1,numeric& arg2) const;
  // Multiplication of two mantissa (no signs)
  numeric PositiveMultiplication(const numeric& p_arg1,const numeric& p_arg2) const;
  // Division of two mantissa (no signs)
  numeric PositiveDivision(numeric& p_arg1,numeric& p_arg2) const;

  // DATA

  // The one and only value 
  SQL_NUMERIC_STRUCT m_numeric;
};
