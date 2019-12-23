//////////////////////////////////////////////////////////////////////////
//
// UINT128 : A 128 bit integer type class
// Based on the code of 'Evan Teran'
// as published on "http://codef00.com/code/uint128.h"
// but enhanced for operators and functions
//
// NOTE: This permutation of the code is **NOT** dependent on BOOST
// 
#include "stdafx.h"
#include "uint128.h"
#include "StdException.h"
#include <limits.h>

// Stop complaining about assigning '-1' to an unsigned integer
#pragma warning(disable: 4146)

// Number of bits in the class
static const unsigned int size128 = 128;

// Forward declaration for a division/modulo
static void divide(const uint128& numerator,const uint128& denominator,uint128& quotient,uint128& remainder);

//////////////////////////////////////////////////////////////////////////
//
// UINT128: Constructors
//
//////////////////////////////////////////////////////////////////////////

uint128::uint128()
{
  hi = 0;
  lo = 0;
}

uint128::uint128(char p_value)
{
  hi = 0;
  lo = static_cast<uint64>(p_value);
  if (p_value < 0)
  {
    hi = static_cast<uint64>(-1UI64);
  }
}

uint128::uint128(unsigned char p_value)
{
  hi = 0;
  lo = static_cast<uint64>(p_value);
}

uint128::uint128(short p_value)
{
  hi = 0;
  lo = static_cast<uint64>(p_value);
  if (p_value < 0)
  {
    hi = static_cast<uint64>(-1UI64);
  }
}

uint128::uint128(unsigned short p_value)
{
  hi = 0;
  lo = static_cast<uint64>(p_value);
}

uint128::uint128(int p_value)
{
  hi = 0;
  lo = static_cast<uint64>(p_value);
  if (p_value < 0)
  {
    hi = static_cast<uint64>(-1UI64);
  }
}

uint128::uint128(unsigned int p_value)
{
  hi = 0;
  lo = p_value;
}

uint128::uint128(float p_value)
        :hi(0)
        ,lo(0)
{
  // BEWARE: Only works for this case
  // AND.... many rounding errors can occur
  if (p_value < UINT_MAX)
  {
    lo = static_cast<uint64>(p_value);
  }
}

uint128::uint128(double p_value)
        :hi(0)
        ,lo(0)
{
  // BEWARE: Only works for this case
  // AND.... many rounding errors can occur
  if (p_value < UINT_MAX)
  {
    lo = static_cast<uint64>(p_value);
  }
}

uint128::uint128(uint64 p_low, uint64 p_high /*=0*/)
{
  hi = p_high;
  lo = p_low;
}

uint128::uint128(const uint128& p_value)
{
  hi = p_value.hi;
  lo = p_value.lo;
}

uint128::uint128(const char* p_string)
        :hi(0)
        ,lo(0)
{
  SetString(p_string);
}

uint128::uint128(const SQL_NUMERIC_STRUCT* p_numeric)
{
  SetNumeric(p_numeric);
}

//////////////////////////////////////////////////////////////////////////
//
// OPERATORS
//
//////////////////////////////////////////////////////////////////////////

uint128& 
uint128::operator=(const uint128& p_other) 
{
  if (&p_other != this) 
  {
    lo = p_other.lo;
    hi = p_other.hi;
  }
  return *this;
}

uint128 
uint128::operator-() const 
{
  // standard 2's compliment negation madness here....
  uint128 temp  = ~uint128(*this);
  return ++temp;
}

uint128 
uint128::operator~() const 
{
  uint128 temp(*this);
  temp.lo = ~temp.lo;
  temp.hi = ~temp.hi;
  return temp;
}

bool 
uint128::operator!() const 
{
  return !(hi != 0 || lo != 0);
}

uint128& 
uint128::operator++() 
{
  if (++lo == 0) 
  {
    ++hi;
  }
  return *this;
}

uint128& 
uint128::operator--() 
{
  if (lo-- == 0) 
  {
    --hi;
  }
  return *this;
}

uint128& 
uint128::operator+=(const uint128& p_other) 
{
  const uint64 old_lo = lo;

  lo += p_other.lo;
  hi += p_other.hi;

  if (lo < old_lo) 
  {
    ++hi;
  }
  return *this;
}

uint128& 
uint128::operator-=(const uint128& b) 
{
  // it happens to be way easier to write it
  // this way instead of make a subtraction algorithm
  return *this += -b;
}

uint128& 
uint128::operator*=(const uint128& p_other) 
{
  // check for multiply by 0
  // result is always 0 :-P
  if (p_other == 0) 
  {
    hi = 0;
    lo = 0;
  }
  else if (p_other != 1) 
  {
    // check we aren't multiplying by 1
    uint128 accu(*this);
    uint128 temp = p_other;

    lo = 0;
    hi = 0;

    for (unsigned int i = 0; i < size128; ++i) 
    {
      if ((temp & 1) != 0) 
      {
        *this += (accu << i);
      }
      temp >>= 1;
    }
  }
  return *this;
}

uint128& 
uint128::operator/=(const uint128& p_other) 
{
  uint128 remainder;
  divide(*this, p_other, *this, remainder);
  return *this;
}

uint128& 
uint128::operator%=(const uint128& p_other) 
{
  uint128 quotient;
  divide(*this, p_other, quotient, *this);
  return *this;
}

uint128
uint128::operator+(const uint128& p_other)
{
  uint128 temp(*this);
  temp += p_other;
  return temp;
}

uint128 
uint128::operator-(const uint128& p_other)
{
  uint128 temp(*this);
  temp += -p_other;
  return temp;
}

uint128 
uint128::operator*(const uint128& p_other)
{
  uint128 temp(*this);
  temp *= p_other;
  return temp;
}

uint128 
uint128::operator/(const uint128& p_other)
{
  uint128 temp;
  uint128 remainder;
  divide(*this,p_other,temp,remainder);
  return temp;
}

uint128 
uint128::operator%(const uint128& p_other)
{
  uint128 temp;
  uint128 remainder;
  divide(*this,p_other,temp,remainder);
  return remainder;
}

// BITWISE OPERATORS

uint128& 
uint128::operator|=(const uint128& p_other) 
{
  hi |= p_other.hi;
  lo |= p_other.lo;
  return *this;
}

uint128& 
uint128::operator&=(const uint128& p_other) 
{
  hi &= p_other.hi;
  lo &= p_other.lo;
  return *this;
}

uint128& 
uint128::operator^=(const uint128& p_other) 
{
  hi ^= p_other.hi;
  lo ^= p_other.lo;
  return *this;
}

uint128  
uint128::operator&(const uint128& p_rhs)
{
  uint128 temp(*this);
  temp.hi &= p_rhs.hi;
  temp.lo &= p_rhs.lo;
  return temp;
}

uint128
uint128::operator|(const uint128& p_rhs)
{
  uint128 temp(*this);
  temp.hi |= p_rhs.hi;
  temp.lo |= p_rhs.lo;
  return temp;
}

uint128
uint128::operator^(const uint128& p_rhs)
{
  uint128 temp(*this);
  temp.hi ^= p_rhs.hi;
  temp.lo ^= p_rhs.lo;
  return temp;
}

// SHIFT operators

uint128& 
uint128::operator<<=(const uint128& rhs) 
{
  unsigned int n = rhs.AsInteger();
  *this = *this << n;
  return *this;
}

uint128  
uint128::operator<<(const uint128& p_rhs)
{
  unsigned int n = p_rhs.AsInteger();
  return *this << n;
}

uint128  
uint128::operator<<(const int p_rhs)
{
  uint128 temp(*this);
  int n(p_rhs);

  if (n >= size128)
  {
    temp.hi = 0;
    temp.lo = 0;
  }
  else
  {
    const unsigned int halfsize = size128 / 2;

    if (n >= halfsize)
    {
      n -= halfsize;
      temp.hi = temp.lo;
      temp.lo = 0;
    }

    if (n != 0)
    {
      // shift high half
      temp.hi <<= n;

      // Create bit mask
      const uint64 mask(~(uint64(-1) >> n));
      // Add to high half
      temp.hi |= (temp.lo & mask) >> (halfsize - n);
      // Finally shift low half
      temp.lo <<= n;
    }
  }
  return temp;
}

uint128& 
uint128::operator>>=(const uint128& rhs) 
{
  unsigned int n = rhs.AsInteger();
  *this = *this >> n;
  return *this;
}

uint128  
uint128::operator>>(const uint128& p_rhs)
{
  unsigned int n = p_rhs.AsInteger();
  return *this >> n;
}

uint128  
uint128::operator>>(const int p_rhs)
{
  uint128 temp(*this);
  int n(p_rhs);

  if(n >= size128)
  {
    temp.hi = 0;
    temp.lo = 0;
  }
  else
  {
    const unsigned int halfsize = size128 / 2;

    if(n >= halfsize)
    {
      n -= halfsize;
      temp.lo = temp.hi;
      temp.hi = 0;
    }

    if (n != 0)
    {
      // shift low half
      temp.lo >>= n;

      // Get lower N bits of high half
      const uint64 mask(~(uint64(-1) << n));
      // And add them to low word
      temp.lo |= (temp.hi & mask) << (halfsize - n);
      // Shift high half
      temp.hi >>= n;
    }
  }
  return temp;
}

// COMPARISON operators

bool 
uint128::operator==(const uint128& p_other) const 
{
  return hi == p_other.hi && lo == p_other.lo;
}

bool
uint128::operator!=(const uint128& p_other) const
{
  return hi != p_other.hi || lo != p_other.lo;
}

bool
uint128::operator<(const uint128& p_other) const 
{
  return (hi == p_other.hi) ? lo < p_other.lo : hi < p_other.hi;
}

bool
uint128::operator>(const uint128& p_other) const
{
  return (hi == p_other.hi) ? lo > p_other.lo : hi > p_other.hi;
}

bool
uint128::operator>=(const uint128& p_other) const
{
  return !(*this < p_other);
}

bool
uint128::operator<=(const uint128& p_other) const
{
  return !(*this > p_other);
}

// Mostly used internal in this class...
bool
uint128::operator!=(const int p_other) const
{
  if(hi != 0)
  {
    return true;
  }
  return lo != static_cast<uint64>(p_other);
}

//////////////////////////////////////////////////////////////////////////
//
// PUBLIC METHODS
//
//////////////////////////////////////////////////////////////////////////

short
uint128::AsShort() const
{
  if (hi || lo & 0xFFFFFFFFFFFF0000UI64)
  {
    throw new StdException("Number to big");
  }
  return static_cast<short>(lo);
}

int
uint128::AsInteger() const 
{
  if(hi || lo & 0xFFFFFFFF00000000UI64)
  {
    throw new StdException("Number to big");
  }
  return static_cast<int>(lo);
}

uint64 
uint128::AsUInt64() const 
{
  if(hi)
  {
    throw new StdException("Number to big");
  }
  return lo;
}

CString
uint128::AsString(unsigned int radix /*= 10*/) const 
{
  // Optimize for zero
  if(*this == 0) 
  {
    return "0";
  }

  if (radix < 2 || radix > 37) 
  {
    throw new StdException("invalid radix");
  }

  // at worst it will be size digits (base 2) so make our buffer
  // that plus room for null terminator
  static char sz[size128 + 1];
  sz[sizeof(sz) - 1] = '\0';

  uint128 ii(*this);
  int i = size128 - 1;

  while (ii != 0 && i) 
  {
    uint128 remainder;
    divide(ii, uint128(radix), ii, remainder);
    sz[--i] = "0123456789abcdefghijklmnopqrstuvwxyz"[remainder.AsInteger()];
  }
  return &sz[i];
}

bool
uint128::IsNull() const
{
  return (hi | lo) ? false : true;
}

void
uint128::Zero()
{
  hi = 0;
  lo = 0;
}

uint64 
uint128::GetHighPart() const
{
  return hi;
}

uint64 
uint128::GetLowPart() const
{
  return lo;
}

// BEWARE: Precision must be set to get performance
void   
uint128::AsNumeric(SQL_NUMERIC_STRUCT* p_numeric) const
{
  // Check for precision
  if(p_numeric->precision <= 0 || p_numeric->precision > (2 * SQL_MAX_NUMERIC_LEN))
  {
    p_numeric->precision = 2 * SQL_MAX_NUMERIC_LEN;
  }

  // Amount in 'val' array to do
  int num   = (p_numeric->precision + 1) / 2;
  int lower = min(num, SQL_MAX_NUMERIC_LEN / 2);

  // Do the lower word
  uint64 lowval = lo;
  for(int index = 0; index < lower; ++index)
  {
    p_numeric->val[index] = static_cast<unsigned char>(lowval & 0x0FF);
    lowval >>= 8;
  }

  // See if we must do the high word
  if(num >= SQL_MAX_NUMERIC_LEN / 2)
  {
    uint64 higher = hi;
    for(int index = SQL_MAX_NUMERIC_LEN / 2; index < num; ++index)
    {
      p_numeric->val[index] = static_cast<unsigned char>(higher & 0xFF);
      higher >>= 8;
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE
//
//////////////////////////////////////////////////////////////////////////

void
uint128::SetString(const char* p_string)
{
  // do we have at least one character?
  if(*p_string == 0)
  {
    return;
  }

  // make some reasonable assumptions
  int  radix = 10;
  bool minus = false;

  const char* i = p_string;

  // check for minus sign, i suppose technically this should only apply
  // to base 10, but who says that -0x1 should be invalid?
  if (*i == '-') 
  {
    ++i;
    minus = true;
  }

  // check if there is radix changing prefix (0 or 0x)
  if(*i) 
  {
    if (*i == '0') 
    {
      radix = 8;
      ++i;
      if(*i) 
      {
        if(*i == 'x') 
        {
          radix = 16;
          ++i;
        }
      }
    }

    while (*i) 
    {
      unsigned int n;
      const char ch = *i;

      if (ch >= 'A' && ch <= 'Z') 
      {
        if (((ch - 'A') + 10) < radix) 
        {
          n = (ch - 'A') + 10;
        }
        else 
        {
          break;
        }
      }
      else if (ch >= 'a' && ch <= 'z') 
      {
        if (((ch - 'a') + 10) < radix) 
        {
          n = (ch - 'a') + 10;
        }
        else 
        {
          break;
        }
      }
      else if (ch >= '0' && ch <= '9') 
      {
        if ((ch - '0') < radix) 
        {
          n = (ch - '0');
        }
        else 
        {
          break;
        }
      }
      else 
      {
        /* completely invalid character */
        break;
      }

      (*this) *= radix;
      (*this) += n;
      ++i;
    }
  }

  // if this was a negative number, do that two's compliment madness :-P
  if(minus) 
  {
    *this = -(*this);
  }
}

void   
uint128::SetNumeric(const SQL_NUMERIC_STRUCT* p_numeric)
{
  // Restart at zero
  Zero();

  // Amount in 'val' array to do
  int num   = (p_numeric->precision + 1) / 2;
  int lower = min(num, SQL_MAX_NUMERIC_LEN / 2);
  static const unsigned int radix = 8;

  // Do the lower word
  for(int index = lower - 1; index >= 0; --index)
  {
    lo <<= radix;
    lo |= p_numeric->val[index];
  }

  // See if we must do the high word
  if(num >= SQL_MAX_NUMERIC_LEN / 2)
  {
    for(int index = num - 1; index >= SQL_MAX_NUMERIC_LEN / 2; --index)
    {
      hi <<= radix;
      hi |= p_numeric->val[index];
    }
  }
}

void   
uint128::PrintDebug()
{
  // TRACE("UINT128 hi:%llX lo:%llX\n", hi, lo);
}

//////////////////////////////////////////////////////////////////////////
//
// Static functions
//
//////////////////////////////////////////////////////////////////////////

void divide(const uint128& numerator
           ,const uint128& denominator
           ,      uint128& quotient
           ,      uint128& remainder)
{
  if (denominator == 0) 
  {
    throw new StdException("divide by zero");
  }
  else 
  {
    uint128 n(numerator);
    uint128 d(denominator);
    uint128 x(1);
    uint128 answer = 0;

    while ((n >= d) && (((d >> (size128 - 1)) & 1) == 0)) 
    {
      x <<= 1;
      d <<= 1;
    }

    while (x != 0) 
    {
      if (n >= d) 
      {
        n -= d;
        answer |= x;
      }

      x >>= 1;
      d >>= 1;
    }

    quotient  = answer;
    remainder = n;
  }
}
