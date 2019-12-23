//////////////////////////////////////////////////////////////////////////
//
// UINT128 : A 128 bit integer type class
// Based on the code of 'Evan Teran'
// as published on "http://codef00.com/code/uint128.h"
// but enhanced for operators and functions
// 
// NOTE: This permutation of the code is **NOT** dependent on BOOST
// 
#pragma once
#include <intsafe.h>
#include <sqltypes.h>

using std::string;

// Handy definitions
#ifndef int64
using int64 = __int64;
#endif

#ifndef uint64
using uint64 = unsigned __int64;
#endif

//////////////////////////////////////////////////////////////////////////

class uint128
{
public:
  // XTOR's
  uint128();
  uint128(char           p_value);
  uint128(unsigned char  p_value);
  uint128(short          p_value);
  uint128(unsigned short p_value);
  uint128(int            p_value);
  uint128(unsigned int   p_value);
  uint128(float          p_value);
  uint128(double         p_value);
  uint128(const uint128& p_value);
  uint128(const char*    p_string);
  uint128(uint64 p_low,uint64 p_high = 0);
  uint128(const SQL_NUMERIC_STRUCT* p_numeric);

  // OPERATORS

  // Unary operators
  uint128  operator-() const;
  uint128  operator~() const;
  bool     operator!() const;
  uint128& operator++();
  uint128& operator--();

  // Arithmetic operators
  uint128  operator+(const uint128& p_other);
  uint128  operator-(const uint128& p_other);
  uint128  operator*(const uint128& p_other);
  uint128  operator/(const uint128& p_other);
  uint128  operator%(const uint128& p_other);

  // Assignment operators
  uint128& operator =(const uint128& p_other);
  uint128& operator+=(const uint128& p_other);
  uint128& operator-=(const uint128& p_other);
  uint128& operator*=(const uint128& p_other);
  uint128& operator/=(const uint128& p_other);
  uint128& operator%=(const uint128& p_other);

  // Bitwise operators
  uint128& operator|=(const uint128& p_other);
  uint128& operator&=(const uint128& p_other);
  uint128& operator^=(const uint128& p_other);

  uint128  operator&(const uint128& p_rhs);
  uint128  operator|(const uint128& p_rhs);
  uint128  operator^(const uint128& p_rhs);

  // Shift operators
  uint128& operator<<=(const uint128& p_rhs);
  uint128& operator>>=(const uint128& p_rhs);
  uint128  operator<< (const uint128& p_rhs);
  uint128  operator>> (const uint128& p_rhs);
  uint128  operator<< (const int p_rhs);
  uint128  operator>> (const int p_rhs);

  // Comparison operators
  bool operator==(const uint128& p_other) const;
  bool operator!=(const uint128& p_other) const;
  bool operator <(const uint128& p_other) const;
  bool operator >(const uint128& p_other) const;
  bool operator>=(const uint128& p_other) const;
  bool operator<=(const uint128& p_other) const;
  // Needed for internal multiplication/division
  bool operator!=(const int p_other) const;

  // FUNCTIONS

  bool    IsNull() const;    // Is 0.0 ?
  void    Zero();            // Make it 0.0 !

  // GETTERS

  short   AsShort()   const;
  int     AsInteger() const;
  uint64  AsUInt64()  const;
  CString AsString(unsigned int p_radix = 10) const;
  void    AsNumeric(SQL_NUMERIC_STRUCT* p_numeric) const;
  uint64  GetHighPart() const;
  uint64  GetLowPart()  const;

#ifdef _DEBUG
  void    PrintDebug();
#endif

private:
  void    SetString (const char* p_string);
  void    SetNumeric(const SQL_NUMERIC_STRUCT* p_numeric);

  // DATA: Our 128 bits integer
  uint64 hi;  // High 64 bits of 128 in total
  uint64 lo;  // Low  64 bits of 128 in total
};

// For std junkies
typedef uint128 uint128_t;
