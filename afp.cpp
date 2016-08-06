//  AFP
//  Floating Point Precision Number class (Binary Coded Decimal)
//  An Arbitrary float always has the format [sign][digit][.[digit]*][E[sign][digits]+] where sign is either '+' or '-'
//  And is always stored in normalized mode after an operation or conversion
//  The length or the representation is always >= 2
//  A null string is considered as an error and an exception is thrown
//  Floating Point Numbers is stored in BASE 10 or BASE 256 depends of the F_RADIX setting
//
// Improvement of the float_precision library of Henrik Vestermark (hve@hvks.com). See www.hvks.com
//
#include "Stdafx.h"
#include "afp.h"
#include <math.h>

// Core Supporting functions. Works directly on string class
// All forward declarations to statics in this implementation file
static int  _afp_normalize( std::string * );
static int  _afp_rounding(std::string* m, int sign, unsigned int precision, enum round_mode mode );
static void _afp_strip_leading_zeros( std::string * );
static void _afp_strip_trailing_zeros( std::string * );
static void _afp_right_shift( std::string *, int );
static void _afp_left_shift( std::string *, int );
static int  _afp_compare( std::string *, std::string * );
static void _int_real_fourier( double data[], unsigned int n, int isign );
static void _int_fourier( std::complex<double> data[], unsigned int n, int isign );
static void _int_reverse_binary( std::complex<double> data[], unsigned int n );

static std::string _afp_uadd_short( std::string *, unsigned int );
static std::string _afp_uadd( std::string *, std::string * );
static std::string _afp_usub_short( int *, std::string *, unsigned int );
static std::string _afp_usub( int *, std::string *, std::string * );
static std::string _afp_umul_short( std::string *, unsigned int );
static std::string _afp_umul( std::string *, std::string * );
static std::string _afp_umul_fourier( std::string *, std::string * );
static std::string _afp_udiv_short( unsigned int *, std::string *, unsigned int );

// Inline functions for the internals of the AFP
inline unsigned char FDIGIT(unsigned char x)  
{
#if (F_RADIX == BASE_10)
  return (unsigned char) (x - '0');
#else
  return (unsigned char) x;
#endif
}

inline unsigned char FCHARACTER(unsigned char x ) 
{
#if (F_RADIX == BASE_10)
  return (unsigned char)(x + '0');
#else
  return (unsigned char) x;
#endif
}

inline unsigned char FCHARACTER10(unsigned char x)
{ 
  return (unsigned char)( x + '0'); 
}

inline int FCARRY( unsigned int x )       { return x / F_RADIX; }
inline int FSINGLE( unsigned int x )      { return x % F_RADIX; }
inline std::string SIGN_STRING( int x )   { return x >=0 ? "+" : "-" ; }
inline int CHAR_SIGN( char x )            { return x == '-' ? -1 : 1; }

//////////////////////////////////////////////////////////////////////////////////////
///
/// CONSTRUCTORS
///
//////////////////////////////////////////////////////////////////////////////////////


// Description:
//   Constructor through a char number
//   Validate and initilize with a character
//   Input Always in BASE_10
//
afp::afp(const char p_number, unsigned int p_precision, enum round_mode p_mode)
{
  int i;

  if( p_number < '0' || p_number > '9' )
  {
    throw bad_int_syntax(); 
  }
  else
  { 
    i = p_number - '0';  // Convert to integer
    m_number  = "+";
    m_number += FCHARACTER( FSINGLE( i ) );
    m_expo    = 0;
    m_rmode   = p_mode;
    m_prec    = p_precision;
  }
}

// Description:
//   Constructor through an integer
//   Just convert integer to string representation in BASE RADIX
//   The input integer is always BASE_10
//   Only use core base functions to create multi precision numbers
//
afp::afp(int p_number, unsigned int p_precision, enum round_mode p_mode)
{
  int sign = 1;
  std::string number;

  m_rmode = p_mode;
  m_prec = p_precision;
  m_expo = 0;

  // Optimize null
  if(p_number == 0)
  {
    m_number = "+";
    m_number.append( 1, FCHARACTER( 0 ) );
    return;
  }
  // Take care of sign
  if(p_number < 0 ) 
  { 
    p_number = -p_number;
    sign = -1;
  }

  if(F_RADIX == BASE_256)  // Fast BASE_256 conversion
  {
    int j;
    unsigned char *p = (unsigned char *)&p_number;

    for( j = sizeof( int ); j > 0; j-- )
    {
      if( p[j-1] != 0 )  // Strip leading zeros
      {
        break;
      }
    }
    for( ; j > 0; j-- )
    {
      number.append( 1, p[j-1] );
    }
  }
  else
  {
    // All other BASE
    for( ; p_number != 0; p_number /= F_RADIX )
    {
      number.insert((std::string::size_type)0, 1, FCHARACTER(p_number % F_RADIX));
    }
  }
  _afp_strip_leading_zeros( &number );                       // First strip for leading zeros
  m_expo = (int)number.length() -1;                               // Always one digit before the dot
  _afp_strip_trailing_zeros( &number );                      // Get rid of trailing non-significant zeros
  m_expo += _afp_rounding( &number, sign, m_prec, m_rmode ); // Perform rounding
  m_number = SIGN_STRING( sign ) + number;                   // Build number
}

// Description:
//   Constructor trough a character string
//   Validate input and convert to internal representation
//   Always add sign if not specified 
//   Only use core base functions to create multi precision numbers
//   The float can be any integer or decimal float representation
//
afp::afp(const char *p_number, unsigned int p_precision, enum round_mode p_mode)
{
  if(p_number == NULL || *p_number== '\0' )
  { 
    throw bad_int_syntax(); 
  }
  m_rmode = p_mode;
  m_prec  = p_precision;
  *this   = _afp_atof(p_number,p_precision,p_mode);
}

// Description:
//   Constructor from a double
//   Validate input and convert to internal representation
//   Always add sign if not specified 
//   Only use core base functions to create multi precision numbers
//   The float can be any integer or decimal float representation
//
afp::afp(double p_number, unsigned int p_precision, enum round_mode p_mode)
{
  m_rmode = p_mode;
  m_prec  = p_precision;
  *this   = _afp_dtof(p_number,p_precision,p_mode);
}

// When initialized through another afp
afp::afp(const afp& p_number)
    :m_number(p_number.m_number)
    ,m_rmode (p_number.m_rmode)
    ,m_prec  (p_number.m_prec)
    ,m_expo  (p_number.m_expo) 
{
 // Simply copy constructor. Nothing more to do
}

//////////////////////////////////////////////////////////////////////////////////////
///
/// END AFP CONSTRUCTORS
///
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// AFP PUBLIC FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

unsigned 
afp::precision(unsigned int p_precision)         
{ 
  int sign; 
  std::string m;
  
  sign   = CHAR_SIGN(m_number[0]);
  m_prec = p_precision > 0 ? p_precision : PRECISION;
  // [HVKS] Adjust rounding only works in base_10 mode of the class
  if(F_RADIX == BASE_10)
  {
    m = m_number.substr(1); // Bypass sign
    m_expo += _afp_rounding(&m, sign, m_prec, m_rmode);
    m_number = SIGN_STRING(sign) + m;
  }
  return m_prec;
}

// Assign another AFP to this AFP
// Works like the copy constructor, but can always be called as in
// afp number;
// afp another;
// number.assign(another)
afp
afp::assign(afp& p_another)
{ 
  m_rmode  = p_another.m_rmode; 
  m_prec   = p_another.m_prec; 
  m_expo   = p_another.m_expo; 
  m_number = p_another.m_number; 
  
  return *this; 
}

// Change the sign of the AFP
// and returns the new sign
int 
afp::change_sign()   
{ 
  // Change and return sign   
  if(m_number.length() != 2 || FDIGIT(m_number[1]) != 0 ) // Don't change sign for +0!
  {
    if(m_number[0] == '+') 
    {
      m_number[0] = '-'; 
    }
    else 
    {
      m_number[0] = '+';
    }
  }
  return CHAR_SIGN(m_number[0]);
}

// Return the sign of the number
int 
 afp::sign() const
{ 
  return CHAR_SIGN( m_number[0] ); 
}

// Cast to an int
afp::operator int()     
{
  // Conversion to int
  std::string s = _afp_ftoa(*this); 
  return atoi( s.c_str() ); 
} 

// Cast to a double
afp::operator double()  
{
  // Conversion to double
  std::string s = _afp_ftoa(*this); 
  return atof( s.c_str() ); 
} 

//////////////////////////////////////////////////////////////////////////////////////
///
/// AFP OPERATORS
///
//////////////////////////////////////////////////////////////////////////////////////

// Description:
//   Assign operator
//   Round it to precision and mode of the left hand side
//   Only the exponent and mantissa is assigned
//   Mode and precision is not affected by the assignment
//
afp& 
afp::operator=(const afp& p_other)
{
  int sign;

  m_expo   = p_other.m_expo;
  sign     = p_other.sign();
  m_number = p_other.m_number.substr(1);
  // [HVKS] In 10 base, see to rounding of
  if(F_RADIX == BASE_10)
  {
    if(_afp_rounding(&m_number,sign,m_prec,m_rmode) != 0 )  
    {
      // Round back to left hand side precision
      m_expo++;
    }
  }
  m_number.insert((std::string::size_type)0,SIGN_STRING(sign));

  return *this;
}

// Description:
//   The essential += operator
//   1) Align to same exponent
//   2) Align to same precision
//   3) Add Mantissa
//   4) Add carry to exponent
//   4) Normalize 
//   5) Rounding to precision 
//   Early out algorithm. i.e.
//      - x+=0 return x
//      - x+=a where x is 0 return a
//
//	@todo   Still missing code for x += a where add make sense. fx. if a is so small it does 
//          not affect the result within the given precision is should ignored. same is true
//          if x is insignificant compare to a the just assign a to x
//
afp& 
afp::operator+=(const afp& p_other)
{
  int sign, sign1, sign2, wrap;
  int expo_max, digits_max;
  unsigned int precision_max;
  std::string s, s1, s2;

  if(p_other.m_number.length() == 2 && FDIGIT(p_other.m_number[1]) == 0)  // Add zero
  {
    return *this;
  }
  if(m_number.length() == 2 && FDIGIT(m_number[1]) == 0)      // Add a (not zero) to *this (is zero) Same as *this = a;
  {
     return *this = p_other;
  }
  // extract sign and unsigned portion of number
  sign1 = p_other.sign();
  s1    = p_other.m_number.substr(1); // Extract Mantissa
  sign2 = CHAR_SIGN(m_number[0]);
  s2    = m_number.substr( 1 );   // Extract Mantissa
  expo_max = MAX(m_expo, p_other.m_expo);
  precision_max = MAX(m_prec,p_other.precision());

  // Check if add makes sense. Still missing

  // Right shift (padd leading zeros) to the smallest number
  if(p_other.m_expo != expo_max )
  {
    _afp_right_shift(&s1, expo_max - p_other.m_expo);
  }
  if(m_expo != expo_max) 
  {
    _afp_right_shift(&s2,expo_max - m_expo);
  }
  // Round to same precision 
  if( _afp_rounding(&s1,sign1,precision_max,p_other.mode()) != 0) // If carry when rounding up then one right shift
  {
    _afp_right_shift( &s1, 1 );
  }
  if( _afp_rounding(&s2,sign2,precision_max,m_rmode) != 0) // If carry when rounding up then one right shift 
  {
    _afp_right_shift( &s2, 1 );
  }
  // Alignment to same number of digits, so add can be perfomed as integer add
  digits_max = (int) MAX( s1.length(), s2.length() );
  if( s1.length() != digits_max )
  {
    _afp_left_shift( &s1, digits_max - (int)s1.length());
  }
  if( s2.length() != digits_max ) 
  {
    _afp_left_shift( &s2, digits_max - (int)s2.length() );
  }
  // Now s1 and s2 is aligned to the same exponent. The biggest of the two
  if( sign1 == sign2 )
  {
    s = _afp_uadd( &s1, &s2 );
    if( s.length() > s1.length() ) // One more digit
    {
      expo_max++;
    }
    sign = sign1;
  }
  else
  {
    int cmp = _afp_compare( &s1, &s2 );
    if( cmp > 0 ) // Since we subctract less the wrap indicater need not to be checked
    {
      s = _afp_usub( &wrap, &s1, &s2 );
      sign = sign1;
    }
    else
    {
      if( cmp < 0 )
      {
        s = _afp_usub( &wrap, &s2, &s1 );
        sign = sign2;
      }
      else
      {  // Result zero
        sign = 1;
        s = "0"; s[0] = FCHARACTER( 0 );
        expo_max = 0;
      }
    }
  }

  expo_max += _afp_normalize( &s );            // Normalize the number
  if( _afp_rounding( &s, sign, m_prec, m_rmode ) != 0 )  // Round back left hand side precision
  {
    expo_max++;
  }
  m_number = SIGN_STRING( sign ) + s;
  m_expo = expo_max;

  return *this;
}

// Description:
//   The essential -= operator
//   n = n - a is the same as n = n + (-a). so change sign and use the += operator instead
//
afp& 
afp::operator-=(const afp& p_other)
{
  afp b;

  b.precision(p_other.precision()); 
  b.mode(p_other.mode());
  b = p_other;
  b.change_sign();
  *this += b;

  return *this;
}

// Description:
//   The essential *= operator
//   1) Multiply mantissa
//   2) Add exponent
//   3) Normalize
//   4) Rounding to precision
//
afp& 
afp::operator*=(const afp& p_other)
{
  int expo_res;
  int sign, sign1, sign2;
  std::string s, s1, s2;

  // extract sign and unsigned portion of number
  sign1 = p_other.sign();
  s1    = p_other.m_number.substr( 1 );
  sign2 = CHAR_SIGN( m_number[0] );
  s2    = m_number.substr( 1 );

  sign = sign1 * sign2;
  s = _afp_umul_fourier( &s1, &s2 );
  expo_res = m_expo + p_other.m_expo;
  if( s.length() - 1 > s1.length() + s2.length() - 2 ) // A carry 
  {
    expo_res++;
  }
  expo_res += _afp_normalize( &s );            // Normalize the number
  if( _afp_rounding( &s, sign, m_prec, m_rmode ) != 0 )  // Round back left hand side precision
  {
    expo_res++;
  }
  if( sign == -1 && s.length() == 1 && FDIGIT( s[0] ) == 0 )  // Avoid -0 as result +0 is right
  {
    sign = 1; // Change sign
  }
  if( s.length() == 1 && FDIGIT( s[0] ) == 0 )  // Result 0 clear exponent
  {
    expo_res = 0;
  }
  m_expo = expo_res;
  m_number = SIGN_STRING( sign ) + s;

  return *this;
}

// Description:
//   The essential /= operator
//   We do a /= b as a *= (1/b)
// Bug 
//   1/27/2006 Inverse was always done with the precision of a instead of the Max precision of both this & a
//
afp& 
afp::operator/=(const afp& p_other)
{
  if(m_number.length() == 2 && FDIGIT(m_number[1]) == 0) // If divisor is zero the result is zero
  {
    return *this;
  }
  afp c;

  c.precision(p_other.precision());
  if(p_other.precision() < m_prec)
  {
    c.precision(m_prec);
  }
  c = p_other;

  afp inv = _afp_inverse(c);
  afp test = p_other * inv;

  *this *= inv;

  // *this *= _afp_inverse(c); 
  return *this;
}

// Description:
//   Binary add two afp numbers
//   Implementing using the essential += operator
//
afp 
operator+(const afp& p_left, const afp& p_right)
{
  afp result;

  if(p_left.precision() < p_right.precision())
  {
    result.precision(p_right.precision());
  }
  else
  {
    result.precision(p_left.precision());
  }
  result  = p_left;
  result += p_right; 

  return result;
}

// Description:
//   Unary add. Do nothing and return a
//
afp 
operator+(const afp& p_right)
{
  // Otherwise do nothing
  return p_right;
}

// Description:
//   Binary subtract two afp numbers
//   Implementing using the essential -= operator
//
afp 
operator-(const afp& p_left,const afp& p_right)
{
  unsigned int precision;
  afp result;

  precision = p_left.precision();
  if( precision < p_right.precision() )
  {
    precision = p_right.precision();
  }
  result.precision(precision);
  result  = p_left;
  result -= p_right; 

  return result;
}

// Description:
//   Unary hypen: Just change sign
//
afp 
operator-(const afp& p_right)
{
  afp result;

  result.precision(p_right.precision());
  result = p_right;
  result.change_sign();

  return result;
}

// Description:
//   Binary multiplying two afp numbers
//   Implenting using the essential *= operator
//
afp 
operator*(const afp& p_left,const afp& p_right)
{
  unsigned int precision;
  afp result;

  precision = p_left.precision();
  if( precision < p_right.precision() )
  {
    precision = p_right.precision();
  }
  result.precision(precision);

  result  = p_left;
  result *= p_right; 

  return result;
}

// Description:
//   Binary divide two afp numbers
//   Implementing using the essential /= operator
//
afp 
operator/(const afp& p_left,const afp& p_right)
{
  unsigned int precision;
  afp result;

  precision = p_left.precision();
  if(precision < p_right.precision())
  {
    precision = p_right.precision();
  }
  result.precision(precision + 1);

  result  = p_left;
  result /= p_right;

  return result;
}

// Description:
//   If both operands has the same mantissa length and same exponent
//   and if the mantissa is identical then it's the same. 
//   However a special test of +0 == -0 is done
//   Precision and rounding mode does not affect the comparison
//
bool 
operator==(const afp& p_left,const afp& p_right)
{
  if(const_cast<afp&>(p_left).ref_mantissa()->length() != const_cast<afp&>(p_right).ref_mantissa()->length() ||
     p_left.exponent() != p_right.exponent() ) // Different therefore false
  {
    return false;
  }
  else
  {
    if((const_cast<afp&>(p_left).ref_mantissa())->compare(*const_cast<afp&>(p_right).ref_mantissa()) == 0)   // Same return true
    {
      return true;
    }
    else
    {
      if( const_cast<afp&>(p_left) .ref_mantissa()->length() == 2   && 
        (*const_cast<afp&>(p_left) .ref_mantissa())[1] == FDIGIT(0) && 
        (*const_cast<afp&>(p_right).ref_mantissa())[1] == FDIGIT(0) )
      {
        // This conditions is only true if +0 is compare with -0 and therefore true
        return true;  
      }
    }
  }
  return false;
}

// Description:
//   1) Test for both operand is zero and return false if condition is meet
//   2) If signs differs then return the boolean result based on that
//   3) Now if same sign and one operand is zero then return the boolean result
//   4) If same sign and not zero check the exponent
//   5) If same sign and same exponent then check the mantissa for boolean result
//   Precision and rounding mode does not affect the comparison
//
bool 
operator<(const afp& p_left,const afp& p_right)
{
  int  sign1, sign2, cmp;
  bool zero1, zero2;

  sign1 = p_left.sign(); 
  sign2 = p_right.sign(); 

  zero1 = const_cast<afp&>(p_left ).ref_mantissa()->length() == 2 && FDIGIT( ( *const_cast<afp&>(p_left ).ref_mantissa())[1] ) == 0 ? true : false;
  zero2 = const_cast<afp&>(p_right).ref_mantissa()->length() == 2 && FDIGIT( ( *const_cast<afp&>(p_right).ref_mantissa())[1] ) == 0 ? true : false;

  if( zero1 == true && zero2 == true )  // Both zero
  {
    return false;
  }
  // Different signs
  if( sign1 < sign2 )
  {
    return true;
  }
  if( sign1 > sign2 )
  {
    return false;
  }
  // Now a &  b has the same sign
  if( zero1 == true )   // If a is zero and a & b has the same sign and b is not zero then a < b
  {
    return true;
  }
  if( zero2 == true )   // If b is zero and a & b has the same sign and a is not zero then a > b
  {
    return false;
  }
  // Same sign and not zero . Check exponent
  if(p_left.exponent() < p_right.exponent() )
  {
    return sign1 > 0 ? true : false;
  }
  if( p_left.exponent() > p_right.exponent() )
  {
    return sign1 > 0 ? false: true;
  }
  // Same sign & same exponent. Check mantissa
  cmp = (const_cast<afp&>(p_left).ref_mantissa())->compare( *const_cast<afp&>(p_right).ref_mantissa());
  if( cmp < 0 && sign1 == 1 )
  {
    return true;
  }
  else
  {
    if( cmp > 0 && sign1 == -1 )
    {
      return true;
    }
  }
  return false;
}

// Description:
//   implemented negating the == comparison
//
bool 
operator!=(const afp& p_left,const afp& p_right)
{
  return p_left == p_right ? false : true;
}

// Description:
//   Implemented using the equality a>b => b<a
//
bool 
operator>(const afp& p_left,const afp& p_right)
{
  return p_right < p_left ? true : false;
}

// Description:
//   Implemented using the equality a<=b => not b<a
//
bool 
operator<=(const afp& p_left,const afp& p_right)
{
  return p_right < p_left ? false : true;
}

// Description:
//   Implemented using the equality a>=b => not a<b
//
bool 
operator>=(const afp& p_left,const afp& p_right)
{
  return p_left < p_right ? false: true;
}

// The stream << operator
std::ostream& 
operator<<( std::ostream& strm, const afp& d ) 
{
  return strm << _afp_ftoa( const_cast<afp &>(d) ).c_str();
}

std::istream& 
operator>>( std::istream& strm, afp& d )
{ 
  char ch; std::string s; int cnt, exp_cnt;
  strm >> std::ws >> ch; 
  if( ch == '+' || ch == '-' ) 
  { 
    s += ch; strm >> ch; 
  } 
  else 
  {
    s += '+';  // Parse sign
  }
  for( cnt = 0; ch >= '0' && ch <= '9'; cnt++, strm >> ch ) 
  {
    s += ch;  // Parse integer part
  }
  if( ch == '.' )  // Parse fraction
  {
    for( s += '.', strm >> ch; ch >= '0' && ch <= '9'; cnt++, strm >> ch ) 
    {
      s += ch;   // Parse fraction part
    }
  }
  if( ch == 'e' || ch == 'E' )
  {
    s += 'e'; strm >> ch; if( ch == '+' || ch == '-' ) 
    { 
      s += ch; strm >> ch; 
    } 
    else 
    {
      s += '+';  // Parse Expo sign 
    }
    for( exp_cnt =0; ch >= '0' && ch <= '9'; exp_cnt++, strm >> ch ) s += ch;  // Parse expo number
  }
  std::cin.putback( ch );  // ch contains the first character not part of the number, so put it back
  if( !strm.fail() && ( cnt > 0 || exp_cnt > 0 ) )  // Valid number 
  {
    d = afp( const_cast<char *>( s.c_str() ), PRECISION, ROUND_NEAR );
  }
  return strm;
}

//////////////////////////////////////////////////////////////////////////////////////
///
/// END AFP OPERATORS
///
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// PUBLIC AFP SUPPORT FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

// Description:
//   Reciprocal/Inverse of V
//   Using a Newton iterations Un = U(2-UV)
//   Always return the result with 2 digits higher precision that argument
//   _afp_inverse() return a interim result for a basic operation like /
//
afp 
_afp_inverse(const afp& p_number)
{
  unsigned int precision,i;
  int    expo;
  double fv, fu;
  afp    r, u, v, c2;
  std::string::reverse_iterator rpos;
  std::string::iterator pos;
  std::string *p;

  precision = p_number.precision();  
  v.precision(precision + 2);
  v = p_number;
  p= v.ref_mantissa();
  if( p->length() == 2 && FDIGIT( (*p)[1] ) == 0 )
  { 
    throw afp::divide_by_zero(); 
    return p_number;
  }
  expo = v.exponent();
  v.exponent( 0 );
  r.precision( precision + 2 ); // Do iteration using 2 digits higher precision
  u.precision( precision + 2 );
  c2.precision(precision + 2 );
  c2 = afp( 2, precision + 2 );

  // Get a initial guess using ordinary floating point
  rpos = v.ref_mantissa()->rbegin();
  fv = FDIGIT( (unsigned char)*rpos );
  for( rpos++; rpos+1 != v.ref_mantissa()->rend(); rpos++ )
  {
    fv *= (double)1/(double)F_RADIX;
    fv += FDIGIT( *rpos );
  }
  if( v.sign() < 0 )
  {
    fv = -fv;
  }
  fu = 1 / fv;

  u = afp( fu );

  // Now iterate using Netwon Un=U(2-UV)
  for(;;)
  {
    r = u * v;                 // UV
    r = c2-r;                  // 2-UV
    u *= r;                    // Un=U(2-UV)
    for( pos = r.ref_mantissa()->begin(), pos+=2, i = 0; pos != r.ref_mantissa()->end(); i++, pos++ )
    {
      if( FDIGIT( *pos ) )
      {
        break;
      }
    }
    if( pos == r.ref_mantissa()->end() || i >= precision )
    {
      break;
    }
  }

  u.exponent( u.exponent() - expo );
  u.mode(p_number.mode());

  return u;
}

// Description:
//   sqrt(V)
//   Equivalent with the same standard C function call
//   Seperate exponent. e.g. sqrt(V*10^x)=10^x/2*sqrt(V)
//   Un=0.5U(3-VU^2)
//   Then Un == 1/Sqrt(V). and sqrt(V) = VUn
//
afp 
sqrt(const afp p_number)
{
  unsigned int precision,i;
  int    expo, expo_sq;
  double fv, fu;
  afp    r, u, v;
  afp    c3(3);
  afp    c05(0.5);
  std::string::reverse_iterator rpos;
  std::string::iterator pos;
  std::string *p;

  precision = p_number.precision(); 
  v.precision( precision + 2 );
  v = p_number;
  if( v.sign() < 0 ) 
  { 
    throw afp::domain_error(); 
    return p_number; 
  }

  p= v.ref_mantissa();
  if( p->length() == 2 && FDIGIT( (*p)[1] ) == 0 )  // Sqrt(0) is zero
  { 
    return afp( 0 ); 
  }

  expo = v.exponent();
  expo_sq = expo / 2;
  v.exponent( expo - 2 * expo_sq );
  r.precision( precision + 2 ); // Do iteration using 2 digits higher precision
  u.precision( precision + 2 );

  // Get a initial guess using ordinary floating point
  rpos = v.ref_mantissa()->rbegin();
  fv = FDIGIT( (unsigned char)*rpos );
  for( rpos++; rpos+1 != v.ref_mantissa()->rend(); rpos++ )
  {
    fv *= (double)1/(double)F_RADIX;
    fv += FDIGIT( *rpos );
  }
  if( expo - 2 * expo_sq > 0 )
  {
    fv *= (double)F_RADIX;
  }
  else
  {
    if( expo - 2 * expo_sq < 0 )
    {
      fv /= (double)F_RADIX;
    }
  }
  fu = 1 / sqrt( fv );

  u = afp( fu );

  // Now iterate using Netwon Un=0.5U(3-VU^2)
  for(;;)
  {
    r = v * u * u;             // VU^2
    r = c3-r;                  // 3-VU^2
    r *= c05;                  // (3-VU^2)/2
    u *= r;                    // U=U(3-VU^2)/2

    for( pos = r.ref_mantissa()->begin(), pos+=2, i = 0; pos != r.ref_mantissa()->end(); i++, pos++ )
    {
      if( FDIGIT( *pos ) )
      {
        break;
      }
    }
    if( pos == r.ref_mantissa()->end() || i >= precision )
    {
      break;
    }
  }

  u *= v;
  u.exponent( u.exponent() + expo_sq );

  // Round to same precision as argument and rounding mode
  u.mode( p_number.mode() );
  u.precision( precision );  

  return u;
}

/// Description:
///   Dynamic tables for "fixed" constant like ln(2), ln(10) and PI
///   If a higher precision is requested we create it and return otherwise 
///   we just the "constant" at a higher precision which eventually will be
///   rounded to the destination variables precision 
//
afp 
_float_table( enum table_type p_type, unsigned int p_precision )
{
  static afp ln2 ( 0, 0, ROUND_NEAR );
  static afp ln10( 0, 0, ROUND_NEAR );
  static afp pi  ( 0, 0, ROUND_NEAR );
  afp res(0, p_precision );

  switch( p_type )
  {
  case _LN2:
    if( ln2.precision() >= p_precision )
    {
      res = ln2;
    }
    else
    {
      int j;
      afp z2(0, p_precision + 1 ), u(0, p_precision + 1), r(0, p_precision + 1 );
      // Calculate ln2(2)
      ln2.precision( p_precision );
      ln2 = afp( 2 );
      for( j = 1; j <= 2; j++ )
      {
        // Reduce the number 2 times to get faster taylor iteration
        ln2 = sqrt( ln2 );   
      }
      // Now the number is less than 1.19
      ln2 = ( ln2 - afp( 1 ) ) / ( ln2 + afp( 1 ) );
      z2 = ln2 * ln2;
      u = ln2;
      // Iterate using taylor series ln(x) == 2( z + z^3/3 + z^5/5 ... )
      for( j=3;;j+=2 )
      {
        u *= z2;
        r = u / afp(j);  
        if( ln2 + r == ln2 )
        {
          break;
        }
        ln2 += r;
      }

      ln2 *= afp( 8 ); // 2 * 2^2 == 2^3 == 8
      res = ln2;
    }
    break;
  case _LN10:
    if( ln10.precision() >= p_precision )
    {
      res = ln10;
    }
    else
    {
      int j;
      afp z2(0, p_precision + 1 ), u(0, p_precision + 1), r(0, p_precision + 1 );
      // Calculate ln2(10)
      ln10.precision( p_precision );
      ln10 = afp(10 );
      for( j = 1; j <= 4; j++ )
      {
        ln10 = sqrt( ln10 );   // Reduce the number 4 times to get faster taylor iteration
      }
      // Now the number is less than 1.16
      ln10 = ( ln10 - afp( 1 ) ) / ( ln10 + afp( 1 ) );
      z2 = ln10 * ln10;
      u = ln10;
      // Iterate using taylor series ln(x) == 2( z + z^3/3 + z^5/5 ... )
      for( j=3;;j+=2 )
      {
        u *= z2;
        r = u / afp(j);  
        if( ln10 + r == ln10 )
        {
          break;
        }
        ln10 += r;
      }

      ln10 *= afp( 32 ); // 2 * 2^4 == 2^5 == 32
      res = ln10;
    }
    break;
  case _PI:
    if( pi.precision() > p_precision )
    {
      res = pi;
    }
    else
    {
      int i;
      afp c05( 0.5 ), c1( 1 );
      afp x, y, xsq, xsq_inv, r;
      afp epsilon(5,2);
      epsilon.exponent(-((int)p_precision + 2));

      pi.precision( p_precision + 2 );
      x.precision( p_precision + 2 );
      y.precision( p_precision + 2 );
      xsq.precision( p_precision + 2 );
      xsq_inv.precision( p_precision + 2 );
      r.precision( p_precision + 2 );
      // x0 = sqrt(2), pi = 2 + sqrt(2), y = 2^(1/4)
      x = sqrt( afp( 2, p_precision + 2 ) );
      pi = x + afp( 2 );
      xsq = sqrt( x );
      xsq_inv = _afp_inverse( xsq );
      y = xsq;
      // Iterate. x = 0.5(sqrt(x)+1/sqrt(x)), pi=pi((x+1)/(y+1)), y=(y*sqrt(x)+1/sqrt(x))/(y+1)
      for( i=0;;i++) 
      {
        x = ( xsq + xsq_inv ) * c05;
        r = ( x + c1 ) / ( y + c1 );
        pi *= r;
        xsq = sqrt( x );
        xsq_inv = _afp_inverse( xsq );
        y = ( y * xsq + xsq_inv ) / ( y + c1 );
        // if( r == c1 )
        if(fabs(c1 - r) < epsilon)
        {
          break;
        }
      }
      res = pi;
      // Round to one extra precision and store it
      pi.precision( p_precision + 1 );  
    }
    break;
  }

  return res;
}

// Description:
//   Use a taylor series until their is no more change in the result
//   exp(x) == 1 + x + x^2/2!+x^3/3!+....
//   Equivalent with the same standard C function call
//
afp 
exp(const afp p_number )
{
  unsigned int precision;
  int j, k =0;
  int expo;
  afp r, u, v;
  afp c05(0.5);

  precision = p_number.precision() + 1;  
  v.precision( precision );
  v = p_number;
  r.precision( precision ); // Do iteration using 2 digits higher precision
  u.precision( precision );

  if( v.sign() < 0 )
  {
    v.change_sign();
  }
  for( k = 0; v > c05; )
  {
    expo = v.exponent();
    if( expo > 0 )
    {
      j = 3 * MIN( 10, expo );  // 2^3
      u = _afp_inverse( afp( 1 << j ) );
      k += j;
    }
    else
    {
      u = c05;
      k++;
    }

    v *= u;
  }

  // Do first two iterations
  u = afp(1) + v;
  r = v * v * c05;
  u += r;
  // Now iterate 
  for( j=3;; j++ )
  {
    r *= v / afp(j);
    if( u + r == u )
    {
      break;
    }
    u += r;
  }

  for( ; k > 0; k-- )
  {
    u *= u;
  }
  if( p_number.sign() < 0 )
  {
    u = _afp_inverse( u );
  }
  // Round to same precision as argument and rounding mode
  u.mode( p_number.mode() );
  u.precision( p_number.precision() );  

  return u;
}

// Description: Natural logaritme
//   Use a taylor series until their is no more change in the result
//   Equivalent with the same standard C function call
//   ln(x) == 2( z + z^3/3 + z^5/5 ...
//   z = (x-1)/(x+1)
//
afp 
log(const afp p_number)
{
  unsigned int precision;
  int j, k;
  int expo;
  afp res, r, z, z2;

  if( p_number <= afp(0) ) 
  { 
    throw afp::domain_error(); 
    return p_number; 
  }

  precision = p_number.precision() + 1;  
  z.precision( precision ); // Do calc at one higher precision to allow correct rounding of result
  r.precision( precision ); 
  z2.precision( precision );
  res.precision( precision );
  z = p_number;
  expo = z.exponent();
  z.exponent( 0 );

  afp epsilon(5,2);
  epsilon.exponent(-((int)precision + 2));


  // In order to get a fast Taylor series result we need to get the fraction closer to one
  // The fraction part is [1.xxx-9.999] (base 10) OR [1.xxx-255.xxx] (base 256) at this point
  // Repeat a series of square root until z < 1.2
  for( k = 0; z > afp( 1.2 ); k++ )
  {
    z = sqrt(z);
  }
  // Calculate the fraction part now at [1.xxx-1.1999]
  z = ( z - afp(1) ) / ( z + afp(1) );
  z2 = z * z;
  res = z;
  // Iterate using taylor series ln(x) == 2( z + z^3/3 + z^5/5 ... )
  for( j=3;;j+=2 )
  {
    z *= z2;
    r = z/afp(j);
    // [HVKS]
    // if( res + r == res )
    if(r < epsilon)
    {
      break;
    }
    res += r;
  }
  res *= afp( pow( 2.0, (double)( k + 1 ) ) );

  if( expo != 0 )
  {
    if( F_RADIX == BASE_10 )  
    {
      // Ln(x^y) = Ln(x) + Ln(10^y) = Ln(x) + y * ln(10)
      res += afp( expo ) * _float_table( _LN10, precision + 1 );
    }
    else     
    {
      // Ln(x^y) = Ln(x) + Ln(256^y) = Ln(x) + y * ln(256) = Ln(x) + y * ln(2^8) = Ln(x) + y * 8 * ln(2)
      res += afp( expo ) * afp( 8 ) * _float_table( _LN2, precision + 1 );
    }
  }
  // Round to same precision as argument and rounding mode
  res.mode( p_number.mode() );
  res.precision( p_number.precision() );  

  return res;
}

// Description:
//   Log10. Use the equation log10(x)=log(x)/log(10)
//   Equivalent with the same standard C function call
//
afp 
log10(const afp p_number)
{
  unsigned int precision = p_number.precision();  
  afp res( 0, precision + 1 );

  if( p_number <= afp(0) ) 
  { 
    throw afp::domain_error(); 
    return p_number; 
  }

  res = p_number;
  res = log( res ) / _float_table( _LN10, precision + 1 );

  // Round to same precision as argument and rounding mode
  res.mode( p_number.mode() );
  res.precision( p_number.precision() );  

  return res;
}

// Description:
//   x^y == exp( y * ln( x ) ) );
// 
afp 
pow(afp p_number, afp p_power)
{
  afp result;

  result.precision(p_number.precision());
  result = log(p_number) * p_power;
  result = exp(result);

  return result;
}

// Description:
//   float precision. fmod remainder of x/y
//   Equivalent with the standard C function fmod
//   x = i * y + f or f = x - i * y; and i = integer(x/y)
//
afp 
fmod( afp p_number, afp p_devisor )
{
  afp devision, remainder;
  int expo;

  remainder.precision( p_number.precision() );
  devision = p_number / p_devisor;
  expo = devision.exponent();
  if( expo < 0 )
  {
    remainder = p_number;
  }
  else
  {
    devision.mode( ROUND_ZERO);
    devision.precision( 1 + expo );
    devision.mode( ROUND_NEAR );
    remainder = p_number - devision * p_devisor;
  }
  return remainder;
}

// Description:
//   Float Precision floor
//   Equivalent with the same standard C function floor
//
afp 
floor(afp p_number)
{
  afp result;
  unsigned int precision = 1 + p_number.exponent();

  if( precision <= 0 ) // is number less than |1|
  {
    if( p_number.sign() < 0 )
    {
      result = afp( -1, p_number.precision() );
    }
    else
    {
      result = afp( 0, p_number.precision() );
    }
  }
  else
  {
    result.mode( ROUND_DOWN );
    result.precision( precision );
    result = p_number;
    result.precision( p_number.precision() );
    result.mode( ROUND_NEAR );
  }
  return result;
}

// Description:
//   Float Precision ceil
//   Equivalent with the same standard C function ceil
//
afp 
ceil(afp p_number)
{
  afp result;
  unsigned int precision = 1 + p_number.exponent();

  if( precision <= 0 ) // is number less than |1|
  {
    if( p_number.sign() < 0 )
    {
      result = afp( 0, p_number.precision() );
    }
    else
    {
      result = afp( 1, p_number.precision() );
    }
  }
  else
  {
    result.mode( ROUND_UP );
    result.precision( 1 + p_number.exponent() );
    result = p_number;
    result.precision( p_number.precision() );
    result.mode( ROUND_NEAR );
  }
  return result;
}

// Description:
//   Float Precision fmod
//   Split a Floating point number into integer part and fraction
//   Equivalent with the same standard C function call
//
afp 
modf(afp p_number, afp *intptr)
{
  afp f ( 0, p_number.precision() );
  afp c1( 1, p_number.precision() );

  intptr->precision( p_number.precision() );

  f = fmod( p_number, c1 );
  *intptr = p_number - f;

  return f;
}

// Description:
//   Float Precision fabs
//   Equivalent with the same standard C function call
//
afp 
fabs(afp p_number)
{
  afp f(0, p_number.precision() );

  f = p_number;
  if( f.sign() < 0 )
  {
    f.change_sign();
  }
  return f;
}

// Description:
//   The ldexp function returns the value of x * 2^exp
//
afp 
ldexp(afp p_number,int p_power )
{
  if( p_power == 0 )
  {
    return p_number;
  }
  if( p_power > 0 && p_power <= 31 )
  {
    return p_number * afp( 1 << p_power );
  }
  return p_number * pow( afp( 2 ), afp( p_power ) );
}

// Description:
//   The frexp()
//   The frexp function breaks down the floating-point value (x) into a mantissa (m) and an exponent (n), 
//   such that the absolute value of m is greater than or equal to 1/RADIX and less than RADIX, and x = m*Radix^n. 
//   The integer exponent n is stored at the location pointed to by expptr. 
//
afp 
frexp(afp p_number,int *expptr)
{
  if(p_number == afp(0))
  {
    *expptr = 0;
  }
  *expptr = p_number.exponent();
  p_number.exponent(0);

  return p_number;
}

//////////////////////////////////////////////////////////////////////////
//
// END AFP SUPPORT FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
///
/// TRIGONOMETRIC FUNCTIONS
///   atan()
///   atan2()
///   asin()
///   acos()
///   sin()
///   cos()
///   tan()
///
//////////////////////////////////////////////////////////////////////////////////////


// Description:
//   Use the taylor series. ArcTan(x) = x - x^3/3 + x^5/5 ...
//   However first reduce x to abs(x)< 0.5 to improve taylor series
//   using the identity. ArcTan(x)=2*ArcTan(x/(1+sqrt(1+x^2)))
//
afp 
atan(afp p_number)
{
  unsigned int precision;
  int j, k;
  afp r, u, v, v2;
  afp c1(1), c05(0.5);

  precision = p_number.precision();  
  v.precision( precision );
  v = p_number;
  r.precision( precision ); 
  u.precision( precision );

  // Transform the solution to ArcTan(x)=2*ArcTan(x/(1+sqrt(1+x^2)))
  k=2;
  v = v / ( c1 + sqrt( c1 + v * v ) );
  if( fabs( v ) > c05 ) // if still to big then do it again
  {
    k=4;
    v = v / ( c1 + sqrt( c1 + v * v ) );
  }

  v2 = v * v;
  r = v;
  u = v;
  // Now iterate using taylor expansion
  for( j=3;; j+=2 )
  {
    v *= v2;
    v.change_sign();
    r = v / afp(j);;
    if( u + r == u )
    {
      break;
    }
    u += r;
  }
  u *= afp( k );

  // Round to same precision as argument and rounding mode
  u.mode( p_number.mode() );
  u.precision( precision );  

  return u;
}

// Description:
//    return the angle (in radians) from the X axis to a point (y,x).
//   use atan() to calculate atan2()
//
afp 
atan2(afp y,afp x)
{
  unsigned int precision;
  afp u;
  afp c0(0), c05(0.5);

  if( x == c0 && y == c0 )
  {
    return c0;
  }
  precision = x.precision();  
  u.precision( precision );
  if( x == c0 )
  {
    u = _float_table( _PI, precision );
    if( y < c0 )
    {
      u *= -c05;
    }
    else
    {
      u *= c05;
    }
  }
  else
  {
    if( y == c0 )
    {
      if( x < c0 )
      {
        u = _float_table( _PI, precision );
      }
      else
      {
        u = c0;
      }
    }
    else
    {
      u = atan( y / x );
      if( x < c0  && y < c0 )
      {
        u -= _float_table( _PI, precision );
      }
      if( x < c0 && y >= c0 )
      {
        u += _float_table( _PI, precision );
      }
    }
  }
  // Round to same precision as argument and rounding mode
  u.mode( x.mode() );
  u.precision( precision );  

  return u;
}

// Description:
//   Use Newton by solving the equation Sin(y)=x. Then y is Arcsin(x)
//   Iterate by Newton y'=y-(sin(y)-x)/cos(y). With initial guess using 
//   standard double precision arithmetic.
//
afp 
asin(afp p_number)
{
  unsigned int precision;
  int j, k, sign;
  double d;
  afp r, v, y, f;
  afp c1(1), c05(0.5), c2(2);

  if( p_number > c1 || p_number < -c1 )
  { 
    throw afp::domain_error(); 
    return p_number; 
  }

  precision = p_number.precision();  
  y.precision( precision );
  r.precision( precision ); 
  v.precision( precision ); 
  f.precision( precision ); 

  v = p_number;
  sign = v.sign();
  if( sign < 0 )
  {
    v.change_sign();
  }
  // Reduce the argument to below 0.5 to make the newton run faster
  for( k = 0; v > c05; k++ )
  {
    v /= sqrt( c2 ) * sqrt( c1 + sqrt( c1 - v * v ) );
  }
  d = asin( (double)v );       // Quick estimate 
  y = afp( d );
  f = afp( 1 / cos( d ) );  // Constant factor 

  // Newton Iteration
  for( j=0;; j++)
  {
    r = ( sin( y ) - v ) * f;
    if( y - r == y )
    {
      break;
    }
    y -=r;
  }

  y *= afp( 1 << k );

  // Round to same precision as argument and rounding mode
  y.mode( p_number.mode() );
  y.precision( precision );  

  if( sign < 0 )
  {
    y.change_sign();
  }
  return y;
}

// Description:
//    Use Arccos(x)=PI/2 - Arcsin(x)
//
afp 
acos(afp p_number)
{
  unsigned int precision;
  afp y;

  precision = p_number.precision();  
  y = _float_table( _PI, precision );
  y /= afp( 2 );
  y -= asin( p_number );

  // Round to same precision as argument and rounding mode
  y.mode( p_number.mode() );
  y.precision( precision );  

  return y;
}

// Description:
//   Use the taylor series. Sin(x) = x - x^3/3! + x^5/5! ...
//   1) However first reduce x to between 0..2*PI 
//   2) Then reduced further to between 0..PI using sin(x+PI)=-Sin(x)
//   3) Finally reduced it to below 0.5 using the trisection identity
//         sin(3x)=3*sin(x)-4*sin(x)^3
//   4) Then do the taylor. 
//   The argument reduction is used to reduced the number of taylor iteration 
//   and to minimize round off erros and calculation time
//
afp 
sin(afp p_number)
{
  unsigned int precision;
  int k, j, sign;
  afp r, u, v, v2;
  afp c05(0.5), c3(3), c4(4);

  precision = p_number.precision() + 1;  
  v.precision( precision );
  v = p_number;
  r.precision( precision ); 
  u.precision( precision );
  v2.precision( precision );

  sign = v.sign();
  if( sign < 0 )
  {
    v.change_sign();
  }
  // Reduce argument to between 0..2PI
  u = _float_table( _PI, precision );
  u *= afp( 2 );
  if( fabs( v ) > u )
  {
    r = v / u; 
    (void)modf( r, &r ); 
    v -= r * u;
  }
  if( v < afp( 0 ) )
  {
    v += u;
  }
  // Reduced it further to between 0..PI
  u = _float_table( _PI, precision );
  if( v > u )
  { 
    v -= u; 
    sign *= -1; 
  }

  // Now use the trisection identity sin(3x)=3*sin(x)-4*sin(x)^3
  // until argument is less than 0.5
  for( k = 0, r = afp(1); v / r > c05; k++ )
  {
    r *= c3;
  }
  v /= r;

  v2 = v * v;
  r = v;
  u = v;
  // Now iterate using taylor expansion
  for( j=3;; j+=2 )
  {
    v = v2 / ( afp(j) * afp( j-1) );
    r *= v;
    r.change_sign();
    if( u + r == u )
    {
      break;
    }
    u += r;
  }

  for( ; k > 0 ; k-- )
  {
    u *= ( c3 - c4 * u * u );
  }
  // Round to same precision as argument and rounding mode
  u.mode( p_number.mode() );
  u.precision( p_number.precision() );  

  if( sign < 0 )
  {
    u.change_sign();
  }
  return u;
}

// Description:
//   Use the taylor series. Cos(x) = 1 - x^2/2! + x^4/4! - x^6/6! ...
//   1) However first reduce x to between 0..2*PI
//   2) Then reduced it further to between 0..PI using cos(x)=Cos(2PI-x) for x >= PI
//   3) Now use the trisection identity cos(3x)=-3*cos(x)+4*cos(x)^3
//      until argument is less than 0.5
//   4) Finally use Taylor 
//
afp 
cos(afp p_number)
{
  unsigned int precision;
  int k, j;
  afp r, u, v, v2;
  afp c05(0.5), c1(1), c2(2), c3(3), c4(4);

  precision = p_number.precision() + 1;  
  v.precision( precision );
  v = p_number;
  r.precision( precision ); 
  u.precision( precision );
  v2.precision( precision );

  // Reduce argument to between 0..2PI
  u = _float_table( _PI, precision );
  u *= c2;
  if( fabs( v ) > u )
  {
    r = v / u; 
    (void)modf( r, &r ); 
    v -= r * u;
  }
  if( v < afp( 0 ) )
  {
    v += u;
  }
  // Reduced it further to between 0..PI. u==2PI
  r = _float_table( _PI, precision );
  if( v > r )
  {
    v = u - v;
  }
  // Now use the trisection identity cos(3x)=-3*cos(x)+4*cos(x)^3
  // until argument is less than 0.5
  for( k = 0, r = c1; v / r > c05; k++ )
  {
    r *= c3;
  }
  v /= r;

  v2 = v * v;
  r = c1;
  u = r;
  // Now iterate using taylor expansion
  for( j=2;; j+=2 )
  {
    v = v2 / ( afp(j) * afp( j-1) );
    r *= v;
    r.change_sign();
    if( u + r == u )
    {
      break;
    }
    u += r;
  }

  for( ; k > 0 ; k-- )
  {
    u *= ( c4 * u * u - c3 );
  }
  // Round to same precision as argument and rounding mode
  u.mode( p_number.mode() );
  u.precision( p_number.precision() );  

  return u;
}

// Description:
//   Use the identity tan(x)=Sin(x)/Sqrt(1-Sin(x)^2)
//   1) However first reduce x to between 0..2*PI
//   2) Use taylot
//
afp 
tan(afp p_number)
{
  unsigned int precision;
  afp u, r, v, p;
  afp c2(2), c3(3);;

  precision = p_number.precision() + 1;  
  u.precision( precision );
  v.precision( precision );
  p.precision( precision );
  v = p_number;

  // Reduce argument to between 0..2PI
  p = _float_table( _PI, precision );
  u = c2 * p;
  if( fabs( v ) > u )
  {
    r = v / u; 
    (void)modf( r, &r ); 
    v -= r * u;
  }

  if( v < afp( 0 ) )
  {
    v += u;
  }
  p /= c2;
  if( v == p || v ==  p * c3 )
  { throw afp::domain_error(); return p_number; }

  u = sin( v ); 
  if( v < p || v > p * c3 ) 
  {
    u /= sqrt( afp( 1 ) - u * u );
  }
  else
  {
    u /= -sqrt( afp( 1 ) - u * u );
  }
  // Round to same precision as argument and rounding mode
  u.mode( p_number.mode() );
  u.precision( p_number.precision() );  

  return u;
}

//////////////////////////////////////////////////////////////////////////////////////
///
/// END TRIGONOMETRIC FUNCTIONS
///
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
///
/// CONVERT AFP to and from ascii representation
///   _afp_atof()
///   _afp_ftoa()
///   _afp_dtof()
///
//////////////////////////////////////////////////////////////////////////////////////

/// Description:
///   Convert afp numbers into string (decimal representation)
//    Digits is the number of decimal digits to print
//    Threshold is the number of decimals. Above +thresold and below -threshold 
//              the scientific E+/-XXX form is used. Otherwise floating point is used
//
std::string 
_afp_ftoa(afp& p_number,unsigned int p_digits /*=PRECISION*/,int p_threshold /*= PRECISION*/)
{
  int rem;
  char buf[ 33 ];
  std::string s;
  std::string src;
  afp r256;
  int expo10, expo256;

  // Test for digits and threshold
  // Otherwise we could never print large numbers!!
  if((int)p_digits < p_threshold)
  {
    p_threshold = p_digits;
  }

  r256.precision( p_number.precision() );
  r256 = p_number;
  s.erase();

  // Initial rounding of the number
  if(p_digits < p_number.precision())
  {
    _afp_rounding(r256.ref_mantissa(),r256.sign(),p_digits,ROUND_NEAR);
  }


  if( F_RADIX == BASE_256 )  // 
  {
    afp frac, ipart;
    bool no_integer;

    expo10=0;
    no_integer = false;
    // Convert Integer and fraction part
    frac = modf( r256, &ipart );
    if( ipart == afp(0) )
    {
      no_integer = true;
      if( r256.sign() < 0 )
      {
        s = "-0";
      }
      else
      {
        s = "+0";
      }
    }
    else
    {
      std::string::reverse_iterator rpos;
      expo256 = ipart.exponent();
      src = ipart.get_mantissa();
      if( (int)src.length() - 1 <= expo256 )
      {
        src.append( (std::string::size_type)( expo256-src.length()+2 ), (char)0 );
      }
      if( F_RADIX == BASE_10 )
      {
        s = src;
      }
      else
      {
        std::string c0;

        c0.insert( (std::string::size_type)0, 1, FCHARACTER( 0 ) );
        s.append( src, 0, 1 );     // Copy sign
        src.erase( src.begin() );  // Erase sign
        for( ; _afp_compare( &src, &c0 ) != 0;  )
        {
          src = _afp_udiv_short( (unsigned int *)&rem, &src, BASE_10 );
          _afp_strip_leading_zeros( &src );
          s.insert( (std::string::size_type)1, 1, (char)FCHARACTER10( rem ) );  
        }
      }
    }

    if( frac != afp(0) ) 
    {
      std::string::reverse_iterator rpos;
      std::string::iterator pos;
      int count;
      bool leading_zero = true;

      s.append( "." );
      src = frac.get_mantissa(); 
      rem = p_digits;
      while(rem > 0)
      {
        frac *= afp( 10 );
        frac = modf( frac, &ipart );
        src = ipart.get_mantissa();
        s.append( (std::string::size_type)1, (char)FCHARACTER10( (unsigned char)src[1] % 10 ) );
        if( frac == afp( 0 ) )
        {
          break;
        }
        --rem;
      }

      // Remove trailing zeros
      // Strip trailing zeros
      for( count = 0, rpos = s.rbegin(); rpos != s.rend() && *rpos == '0'; rpos++ )
      {
        count++;
      }
      s.erase( s.length() - count, count );
      if( no_integer == true )
      {
        s.erase( 2, 1 );  // Remove the dot
        // Count leading zeros
        for( expo10 = 0, pos = s.begin(), pos++; pos != s.end() && *pos == '0'; pos++ )
        {
          expo10--;
        }
        if( expo10 < 0 )      
        {
          s.erase( 1, -expo10 );        // Remove leading zeros
        }
        if( s.length() > 2 )
        {
          s.insert( 2, "." );           // Insert dot again
        }
      }
      else
      {
        std::string::size_type nidx;

        nidx = s.find_first_of( '.' );
        if( nidx > 2 )
        {
          s.erase( nidx, 1 );     // Erase dot
          s.insert( 2, "." );     // and move it to after the first digit
          expo10 += (int)nidx - 2;     // increase the exponent by number os positions moved
        }
      }
    }
    else
    {
      if( no_integer == false )
      {
        if( s.length() > 2 )
        {
          expo10 += (int)s.length() - 2;
          s.insert( (std::string::size_type)2, "." );
        }
      }
    }
  }
  else
  {
    s = r256.get_mantissa();
    if( s.length() > 2 ) 
    {
      s.insert( (std::string::size_type)2, "." );
    }
    expo10 = p_number.exponent();
  }

  // Do the exponent part of the number
  if((expo10 >  p_threshold) ||
     (expo10 < -p_threshold) )
  {
    // Has te be an E+/-xxx form
    // Because normalized form would be too long
    s += "E";
    _itoa_s( expo10, buf, 32, 10 );
    s += buf;
  }
  else
  {
    // No E-form needed. Decimal point to the left
    if(expo10 > 0)
    {
      unsigned int point_pos = 2;
      while(expo10)
      {
        if(s.length() < point_pos + 2)
        {
          s.append(1,'0');
        }
        // next
        ++point_pos;
        --expo10;
      }
      s.erase(2,1);
      if(point_pos < s.length())
      {
        s.insert(point_pos,".");
      }
    }
    else
    {
      // EXPO10 below zero
      // No E-Form needed. Decimal point to the right
      s.erase(2,1);
      s.insert((std::string::size_type)1,(std::string::size_type)-expo10,'0'); 
      s.insert(2,".");
    }
  }
  return s;
}

// Description:
//   Convert double (IEE754) into a afp numbers 
//   Convert afp numbers into string (decimal representation)
//   Constructor for double floating point 
//
afp 
_afp_dtof(double p_number, unsigned int p_precision, enum round_mode p_mode)
{
  int expo;
  std::string n;
  char buf[ 32 ];
  afp fp(0,p_precision,p_mode);

  if( p_number == 0 )
  {
    return fp;
  }
  expo = 0;

  if( F_RADIX == BASE_10 )
  {
    //_gcvt( d, 18, buf);
    _gcvt_s(buf,p_number,18);
    // sprintf( buf, "%.16e", d );
    fp = _afp_atof( buf, p_precision, p_mode );
  }
  else
  {
    int i;
    afp cpower2(2);

    memcpy( buf, (char *)&p_number, sizeof( double ) );
    n += "+1"; n[1] = FCHARACTER( 1 );
    n += ( ( buf[ 6 ] & 0xf ) << 4 ) + ( ( buf[ 5 ] & 0xf0 ) >> 4 );
    n += ( ( buf[ 5 ] & 0xf ) << 4 ) + ( ( buf[ 4 ] & 0xf0 ) >> 4 );
    n += ( ( buf[ 4 ] & 0xf ) << 4 ) + ( ( buf[ 3 ] & 0xf0 ) >> 4 );
    n += ( ( buf[ 3 ] & 0xf ) << 4 ) + ( ( buf[ 2 ] & 0xf0 ) >> 4 );
    n += ( ( buf[ 2 ] & 0xf ) << 4 ) + ( ( buf[ 1 ] & 0xf0 ) >> 4 );
    n += ( ( buf[ 1 ] & 0xf ) << 4 ) + ( ( buf[ 0 ] & 0xf0 ) >> 4 );
    _afp_strip_trailing_zeros( &n );
    expo = ( ( buf[ 7 ] & 0x7f ) << 4 ) + ( ( buf[ 6 ] & 0xf0 ) >> 4 );
    expo -= 1023;  // unbiased the double exponent
    if( buf[ 7 ] & 0x80 )  // set sign
    {
      n[0] = '-';
    }
    else
    {
      n[0] = '+';
    }
    fp.set_n( n );
    // convert exponent in 2^expo to 256^x. exponent modulo 8 is set straight into expo the remainding 
    // is converted by multiplying repeately with 2 or 0.5
    i = expo % 8;
    expo /= 8;
    fp.exponent( expo );
    if( i > 0 )
    {
      // Create 2^i where i [1..7]
      switch( i )
      {
        case 1: (*cpower2.ref_mantissa())[1] = (char)2; break;
        case 2: (*cpower2.ref_mantissa())[1] = (char)4; break;
        case 3: (*cpower2.ref_mantissa())[1] = (char)8; break;
        case 4: (*cpower2.ref_mantissa())[1] = (char)16; break;
        case 5: (*cpower2.ref_mantissa())[1] = (char)32; break;
        case 6: (*cpower2.ref_mantissa())[1] = (char)64; break;
        case 7: (*cpower2.ref_mantissa())[1] = (char)128; break;
      }
      fp *= cpower2;
    }
    else
    {
      if( i < 0 )
      {
        // std::string s = cpower2.get_mantissa();
        // Create 2^-i where i [-1..-7]
        switch( -i )
        {
          case 1: (*cpower2.ref_mantissa())[1] = (char)0x80; break;
          case 2: (*cpower2.ref_mantissa())[1] = (char)0x40; break;
          case 3: (*cpower2.ref_mantissa())[1] = (char)0x20; break;
          case 4: (*cpower2.ref_mantissa())[1] = (char)0x10; break;
          case 5: (*cpower2.ref_mantissa())[1] = (char)0x8; break;
          case 6: (*cpower2.ref_mantissa())[1] = (char)0x4; break;
          case 7: (*cpower2.ref_mantissa())[1] = (char)0x2; break;
        }
        cpower2.exponent( -1 );
        fp *= cpower2;
      }
    }
  }
  return fp;
}

/// Description:
///   Convert ascii string into a afp numbers 
//    The ascii float format is based on standard C notation
//
afp 
_afp_atof(const char *str, unsigned int p_precision, enum round_mode p_mode)
{
  int sign, sign_expo;
  int expo, expo_radix, expo_e;
  int s_digit, f_digit;
  std::string::size_type nidx, idx;
  unsigned int i;
  std::string s(str);
  std::string::iterator pos;
  std::string number, fraction, exponent;
  afp fp(0,p_precision,p_mode);

  expo = 0;
  idx=0;
  sign = CHAR_SIGN( '+' );
  // Parse leading sign if any
  pos = s.begin();
  if( *pos == '+' || *pos == '-' )  // 
  {
    sign = CHAR_SIGN( *pos );
    pos++;
    idx=1;
    if( pos == s.end() )
    { 
      throw afp::bad_int_syntax(); 
      return fp; 
    }
  }
  // Determine any significant, fraction sign or exponent sign
  nidx = s.find_first_of( ".eE", idx );
  if( nidx == std::string::npos ) // Only digits (INTEGER) if any
  {
    if( *pos == '0' ) // Octal or hex representation
    {
      if( pos+1 != s.end() && tolower( pos[1] ) == 'x' )
      {
        for( pos += 2; pos != s.end(); pos++ )
        {
          if( ( *pos < '0' || *pos > '9' ) && ( tolower( *pos ) < 'a' || tolower( *pos ) > 'f' ) )
          {  
            throw afp::bad_int_syntax(); 
            return fp; 
          }
          else
          {
            char buf[ 16 ];
            std::string tmp;

            int hexvalue = *pos - '0';
            if( hexvalue > 10 )
            {
              hexvalue = tolower( *pos ) - 'a' + 10;
            }
            //itoa( BASE_16, &buf[ 0 ], BASE_10 );
            _itoa_s(BASE_16,&buf[0],16,BASE_10);
            tmp = buf;
            number = _afp_umul_fourier( &number, &tmp );

            _itoa_s( hexvalue, &buf[ 0 ], 16,BASE_10 );
            tmp = buf;
            number = _afp_uadd( &number, &tmp );
          }
        }
      }
      else
      { 
        // Collect octal represenation
        for( ; pos != s.end(); pos++ )
        {
          if( *pos < '0' || *pos > '7' )
          { 
            throw afp::bad_int_syntax(); 
            return fp; 
          }
          else
          {
            number = _afp_umul_short( &number, BASE_8 );
            number = _afp_uadd_short( &number, *pos - '0' );
          }
        }
      }
    }
    else
    { 
      // Collect decimal representation
      for( ; pos != s.end(); pos++ )
      {
        if( *pos < '0' || *pos > '9' )
        {  
          throw afp::bad_int_syntax(); 
          return fp; 
        }
        else
        {
          number = _afp_umul_short( &number, BASE_10 );
          number = _afp_uadd_short( &number, *pos - '0' );
        }
      }
    }

    // This is all integers digits, so exponent is the number of digits
    _afp_strip_leading_zeros( &number );  // First strip for leading zeros
    expo = (int)number.length() -1;            // Always one digit before the dot
    _afp_strip_trailing_zeros( &number ); // Get rid of trailing non-significant zeros
    expo += _afp_rounding( &number, sign, p_precision, p_mode );
    number.insert( (std::string::size_type)0, SIGN_STRING( sign ) ); // Build the complete number
    fp.set_n( number );
    fp.exponent( expo );

    return fp;
  }

  s_digit = 0;
  f_digit = 0;
  // Pick up significant beteen idx and nidx 
  if( nidx > idx ) // Number of digits before the . sign or exponent
  {
    // Strip leading zeros
    for( i = (int)idx; i != (int)nidx; i++ ) 
    {
      if( s[i] != '0' ) 
      {
        break;
      }
    }
    // Collect significant
    for( ; i != nidx; i++ )
    {
      if( s[i] < '0' || s[i] > '9' )
      {  
        throw afp::bad_float_syntax(); 
        return afp(0); 
      }
      else
      {
        number = _afp_umul_short( &number, BASE_10 );
        number = _afp_uadd_short( &number, s[i] - '0' );
        s_digit++;  // Significant digits. leading space are not counted
      }
    }
  }

  // Floating point representation
  if( s[ nidx ] == '.' ) // Any fraction ?
  { 
    idx = nidx + 1;                      // Find start of fraction
    nidx = s.find_first_of( "eE", idx ); // Find end of fraction
    if( nidx == std::string::npos )
    {
      nidx = s.length();
    }
    // Remove trailing zero digits
    for( i = (int)nidx - 1; i >= (int)idx; i--, nidx-- ) 
    {
      if( s[i] != '0' ) 
      {
        break;
      }
    }
    for( i = (int)idx; i < (int)nidx; i++ )
    {
      if( s[i] < '0' || s[i] > '9' )
      {  
        throw afp::bad_float_syntax(); 
        return afp(0); 
      }
      else
      {
        number = _afp_umul_short( &number, BASE_10 );
        number = _afp_uadd_short( &number, s[i] - '0' );
        f_digit++; // fraction digits. trailing zeros are not counted
      }
      nidx = s.find_first_of( "eE", idx );
      if( nidx == std::string::npos )
      {
        nidx = s.length();
      }
    }
  }

  expo_e = 0;
  if( nidx != std::string::npos && 
      nidx < s.length() && 
      ( s[ nidx ] == 'e' || s[ nidx ] == 'E' ) )
  {
    // Parse the exponent 
    idx = nidx + 1;
    nidx = s.length();
    sign_expo = CHAR_SIGN( '+' );;
    if( idx < nidx && ( s[idx] == '+' || s[idx] == '-' ) )  
    {
      sign_expo = CHAR_SIGN( s[idx] );
      idx++;
      if( idx == nidx )
      { 
        // Sign but no number
        throw afp::bad_float_syntax(); 
        return afp(0); 
      }  
    }
    else
    {
      if( idx >= nidx )
      { 
        // E but no number
        throw afp::bad_float_syntax(); 
        return afp(0); 
      }  

      // Collect exponent using base 10
      for( i = (int)idx; i < (int)nidx; i++ )
      {
        if( s[i] < '0' || s[i] > '9' )
        {  
          throw afp::bad_float_syntax(); 
          return afp(0); 
        }
        else
        {
          expo_e *= BASE_10;
          expo_e += s[i] - '0';
        }
        if( sign_expo < 0 )
        {
          expo_e= -expo_e;
        }
        //
        if( number.length() == 0 )
        {
          number += "1";
        }
      }
    }
  }

  expo_radix = (int)number.length() - 1;  // Always one digit before the dot
  if( f_digit > 0 )
  {
    expo_e += -f_digit;  // Adjust for fraction counted as a significant
  }
  if( F_RADIX == BASE_10 )
  {
    expo_radix += expo_e;
  }
  // Put it all together
  // Now the number has everything so get rid of trailing non-significant zeros
  expo_radix += _afp_normalize( &number );                // Normalize
  expo = _afp_rounding( &number, sign, p_precision, p_mode );    // Perform rounding
  expo += expo_radix;
  number.insert( (std::string::size_type)0, SIGN_STRING( sign ) ); // Build the complete number
  fp.set_n( number );
  fp.exponent( expo_radix );

  if( F_RADIX == BASE_256 && expo_e != 0 )
  {
    if( expo_e > 0 )
    {
      afp cpower100(100, p_precision, p_mode ), cpower10(10, p_precision, p_mode );

      for( ; expo_e >= 2; expo_e -= 2 )
      {
        fp *= cpower100;
      }
      for( ; expo_e >= 1; expo_e-- )
      {
        fp *= cpower10;
      }
    }
    else
    {
      if( expo_e < 0 )
      {
        afp cpower01(10, p_precision+1, p_mode ), cpower001(100, p_precision+1, p_mode );

        cpower01 = _afp_inverse( cpower01 );   // Hand craf it to the correct precision
        cpower001 = _afp_inverse( cpower001 ); // Hand craf it to the correct precision

        for( ; expo_e < -1; expo_e += 2 )
        {
          fp *= cpower001;
        }
        for( ; expo_e < 0; expo_e++ )
        {
          fp *= cpower01;
        }
      }
    }
  }
  return fp;
}

//////////////////////////////////////////////////////////////////////////////////////
///
/// END CONVERT AFP to and from ascii representation
///
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
///
/// AFP CORE FUNCTIONS
///
///   _afp_strip_leading_zeros
///   _afp_strip_trailing_zeros
///   _afp_normalize
///   _afp_rounding
///   _afp_right_shift
///   _afp_left_shift
///   _afp_compare
///   _afp_uadd_short
///   _afp_uadd
///   _afp_usub_short
///   _afp_usub
///   _afp_umul_short
///   _afp_umul
///   _afp_umul_fourier
///   _afp_udiv_short
///
///   Works Directly on the string class of the AFP
///
//////////////////////////////////////////////////////////////////////////////////////

// Description:
//   Remove leading nosignificant zeros
//
static void 
_afp_strip_leading_zeros( std::string *s )
{
  std::string::iterator pos;

  // Strip leading zeros
  for( pos = s->begin(); pos != s->end() && FDIGIT( *pos ) == 0; )
  {
    s->erase( pos );
  }
  if( s->length() == 0 )
  {
    *s = FCHARACTER(0);
  }
  return;
}

// Description:
//   Remove trailing nosignificant zeros
//
static void 
_afp_strip_trailing_zeros( std::string *s )
{
  std::string::reverse_iterator pos;
  int count;

  // Strip trailing zeros
  for( count = 0, pos = s->rbegin(); pos != s->rend() && FDIGIT( *pos ) == 0; pos++ )
  {
    count++;
  }
  s->erase( s->length() - count, count );
  if( s->length() == 0 )
  {
    *s = FCHARACTER(0);
  }
  return;
}

// Description:
//   Right shift number x decimals by inserting 0 in front of the number
//
static void 
_afp_right_shift( std::string *s, int shift )
{
  s->insert( (std::string::size_type)0, shift, FCHARACTER( 0 ) );
}

// Description:
//   Left shift number x decimals by appending 0 in the back of the number
//
static void 
_afp_left_shift( std::string *s, int shift )
{
  s->append( shift, FCHARACTER( 0 ) );
}

// Description:
//   Normalize the mantissa
//   1) If a number does not have a leading digit != 0 then left shift until 
//   it has and adjust the exponent accordingly and return it.
//   2) Then remove trailing zeros
//   3) The mantissa NEVER contain a leading sign
//
static int 
_afp_normalize( std::string *m )
{
  int expo = 0;
  std::string::iterator pos;

  // Left shift until a digit is not 0
  for( pos = m->begin(); pos != m->end() && FDIGIT( *pos ) == 0; )
  {
    m->erase( pos );
    expo--;
  }
  if( m->length() == 0 ) // If all zero the number is zero
  {
    *m = FCHARACTER(0);
    return 0;
  }
  _afp_strip_trailing_zeros( m );

  return expo;
}

// Description:
//   Rounding control
//   Round the fraction to the number of precision based on the round mode 
//   Note that the mantissa number has ALWAYS been normalize prior to rounding
//   The mantissa NEVER contain a leading sign
//   Rounding Mode Positive numnber   Result    
//   Rounding to nearest              +�   
//   Rounding toward zero (Truncate)  Maximum, positive finite value   
//   Rounding up (toward +�)          +�   
//   Rounding down) (toward -�)       Maximum, positive finite value   
//
//   Rounding Mode Negative number    Result    
//   Rounding to nearest              -�   
//   Rounding toward zero (Truncate)  Maximum, negative finite value   
//   Rounding up (toward +�)          Maximum, negative finite value   
//   Rounding down) (toward -�)       -�   
//
static int 
_afp_rounding(std::string* m, int sign, unsigned int precision, enum round_mode mode )
{
  enum round_mode rm = mode;

  if( m->length() > precision )  // More digits than we need 
  {
    if( rm == ROUND_NEAR )
    {
      if( 2 * FDIGIT( (*m)[ precision ] ) >= F_RADIX )
      {
        //sign < 0 ? ROUND_DOWN : ROUND_UP;
        rm = ROUND_UP; 
      }
      else
      {
        // sign < 0 ? ROUND_UP : ROUND_DOWN;
        rm = ROUND_DOWN; 
      }
    }
    else
    {
      if( rm == ROUND_UP && sign < 0 )
      {
        rm = ROUND_DOWN;
      }
      else
      {
        if( rm == ROUND_DOWN && sign < 0 )
        {
          rm = ROUND_UP;
        }
      }
    }
    // Chuck excessive digits
    m->erase( (std::string::size_type)precision, m->length() - precision );

    if( rm == ROUND_UP ) 
    {
      std::string::size_type before;

      before = m->length();
      *m = _afp_uadd_short( m, 1 );
      if( m->length() > before )
      {
        if( m->length() > precision )
        {
          m->erase( (std::string::size_type)precision, m->length() - precision );
        }
        _afp_strip_trailing_zeros( m );            
        return 1;
      }
    }
  }
  _afp_strip_trailing_zeros( m );            
  return 0;
}

// Description:
//   Compare two unsigned decimal string 
//   and return 0 is equal, 1 if s1 > s2 otherwise -1
//   Optimized check length first and determine 1 or -1 if equal
//   compare the strings
//
static int 
_afp_compare( std::string *s1, std::string *s2 )
{
  int cmp;

  if( s1->length() > s2->length() )
  {
    cmp = 1;
  }
  else
  {
    if( s1->length() < s2->length() )
    {
      cmp = -1;
    }
    else
    {
      cmp = s1->compare( *s2 );
    }
  }
  return cmp;
}

// Description:
//   add a short integer to a floating point string (mantissa)
//   Short float Add: The digit d [0..F_RADIX] is added to the unsigned fraction string
//   Optimized 0 add or early out add is implemented
//
static std::string 
_afp_uadd_short( std::string *src1, unsigned int d )
{
  unsigned short ireg;
  std::string::reverse_iterator r1_pos, rd_pos;
  std::string des1;

  if( d > F_RADIX )
  {
    throw afp::out_of_range();
    des1.insert( (std::string::size_type)0, 1, FCHARACTER(0) );
    return des1;
  }

  if( d == 0 )   // Zero add
  {
    return *src1;
  }
  ireg = F_RADIX * d;
  des1 = *src1;
  rd_pos = des1.rbegin();
  r1_pos = src1->rbegin();

  for(; r1_pos != src1->rend(); r1_pos++, rd_pos++ )
  {
    ireg = FDIGIT( *r1_pos ) + FCARRY( ireg ); 
    *rd_pos = FCHARACTER( FSINGLE( ireg ) );
    if( FCARRY( ireg ) == 0 ) // Early out add
    {
      break;
    }
  }
  if( FCARRY( ireg ) != 0 )  // Insert the carry in the front of the number
  {
    des1.insert( (std::string::size_type)0, 1, FCHARACTER( FCARRY( ireg ) ) );
  }
  return des1;
}

// Description:
//   Add two unsigned decimal strings
//   Optimized: Used early out add
//
static std::string 
_afp_uadd( std::string *src1, std::string *src2 )
{
  unsigned short ireg = 0;
  std::string des1;
  std::string::reverse_iterator r_pos, r_end, rd_pos;

  if( src1->length() >= src2->length() )
  {
    des1 = *src1; 
    r_pos = src2->rbegin();
    r_end = src2->rend();
  }
  else
  {
    des1 = *src2;
    r_pos = src1->rbegin();
    r_end = src1->rend();
  }
  rd_pos = des1.rbegin();

  for(; r_pos != r_end;)
  { 
    // Adding element by element for the two numbers
    ireg = FDIGIT( *r_pos ) + FDIGIT( *rd_pos ) + FCARRY( ireg );
    *rd_pos = FCHARACTER( FSINGLE( ireg ) );
    r_pos++;
    rd_pos++;
  }

  // Exhaust the smalles of the number, so only the carry can changes the uppper radix digits
  for( ; FCARRY( ireg ) != 0 && rd_pos != des1.rend(); )
  {
    ireg = FDIGIT( *rd_pos ) + FCARRY( ireg );
    *rd_pos = FCHARACTER( FSINGLE( ireg ) );
    rd_pos++;
  }

  // No more carry or end of upper radix number. 
  if( FCARRY( ireg ) != 0 ) // If carry add the carry as a extra radix digit to the front of the number
  {
    des1.insert( (std::string::size_type)0, 1, FCHARACTER( FCARRY( ireg ) ) );
  }
  return des1;
}

// Description:
//   subtract a short integer from a floating point string (mantissa)
//   Short Subtract: The digit d [0..F_RADIX] is subtracted from a unsigned decimal string
//   if src1 < src2 return -1 (wrap around) otherwise return 0 (no wrap around)
//
static std::string 
_afp_usub_short( int *result, std::string *src1, unsigned int d )
{
  unsigned short ireg = F_RADIX;
  std::string::reverse_iterator r1_pos;
  std::string::iterator d_pos;
  std::string des1;

  if( d > F_RADIX )
  {
    throw afp::out_of_range();
    des1.insert( (std::string::size_type)0, 1, FCHARACTER(0) );
    return des1;
  }

  if( d == 0 ) // Nothing to subtract
  {
    *result = 0;
    return *src1;
  }

  des1.erase();
  d_pos = des1.begin();
  r1_pos = src1->rbegin();

  ireg = F_RADIX - 1 + FDIGIT( *r1_pos ) - d + FCARRY( ireg );
  d_pos = des1.insert( d_pos, FCHARACTER( FSINGLE( ireg ) ) );
  for( r1_pos++; FCARRY( ireg ) && r1_pos != src1->rend(); r1_pos++ )
  {
    ireg = F_RADIX - 1 + FDIGIT( *r1_pos ) + FCARRY( ireg );
    d_pos = des1.insert( d_pos, FCHARACTER( FSINGLE( ireg ) ) );
  }

  *result = FCARRY( ireg ) - 1;
  return des1;
}

// Description:
//   Subtract two unsigned decimal strings
//   if src1 < src2 return -1 (wrap around) otherwise return 0 (no wrap around)
//
static std::string 
_afp_usub( int *result, std::string *src1, std::string *src2 )
{
  unsigned short ireg = F_RADIX;
  std::string::reverse_iterator r1_pos, r2_pos;
  std::string::iterator d_pos;
  std::string des1;

  des1.erase();
  d_pos  = des1.begin();
  r1_pos = src1->rbegin();
  r2_pos = src2->rbegin();

  for(; r1_pos != src1->rend() || r2_pos != src2->rend();)
  {
    if( r1_pos != src1->rend() && r2_pos != src2->rend() )
    { 
      ireg = F_RADIX - 1 + FDIGIT( *r1_pos ) - FDIGIT( *r2_pos ) + FCARRY( ireg ); 
      r1_pos++;
      r2_pos++; 
    }
    else
    {
      if( r1_pos != src1->rend() )
      { 
        ireg = F_RADIX - 1 + FDIGIT( *r1_pos ) + FCARRY( ireg ); 
        r1_pos++; 
      }
      else
      { 
        ireg = F_RADIX - 1 - FDIGIT( *r2_pos ) + FCARRY( ireg ); 
        r2_pos++; 
      }
    }
    d_pos = des1.insert( d_pos, FCHARACTER( FSINGLE( ireg ) ) );
  }

  *result = FCARRY( ireg ) - 1;
  return des1;
}

// Description:
//   multiply a short integer to a floating point string (mantissa)
//   Short float Multiplication: The unsigned digit d [0..F_RADIX] is multiplied to the unsigned fraction 
//   Optimize: Multiply with zero yields zero;
//
static std::string 
_afp_umul_short( std::string *src1, unsigned int d )
{
  unsigned short ireg = 0;
  std::string::reverse_iterator r1_pos;
  std::string::iterator d_pos;
  std::string des1;

  if( d > F_RADIX )
  {
    throw afp::out_of_range();
    des1.insert( (std::string::size_type)0, 1, ( FCHARACTER(0) ) );
    return des1;
  }

  if( d == 0 )
  {
    des1.insert( (std::string::size_type)0, 1, ( FCHARACTER(0) ) );
    return des1;
  }

  if( d == 1 )
  {
    des1 = *src1;
    return des1;
  }

  if( d == F_RADIX )  
  {
    des1.insert( (std::string::size_type)0, 1, ( FCHARACTER(0) ) );
    des1 = *src1 + des1;
    _afp_strip_leading_zeros( &des1 );
    return des1;
  }

  des1.erase();             
  d_pos = des1.begin();
  r1_pos = src1->rbegin();

  for( ; r1_pos != src1->rend(); r1_pos++ )
  {
    ireg = FDIGIT(  *r1_pos ) * d + FCARRY( ireg );
    d_pos = des1.insert( d_pos, FCHARACTER( FSINGLE( ireg ) ) );
  }

  if( FCARRY( ireg ) != 0 )
  {
    d_pos = des1.insert( d_pos, FCHARACTER( FCARRY( ireg ) ) );
  }
  return des1;
}

// Description:
//   Multiply two unsigned decimal strings
//   Optimized: Used early out add and multiplication w. zero
//   NO LONGER IN USE. Replaced by _afp_umul_fourier
//
static std::string 
_afp_umul( std::string *src1, std::string *src2 )
{
  unsigned short ireg = 0;
  int disp;
  std::string des1, tmp;
  std::string::reverse_iterator r_pos2;

  r_pos2 = src2->rbegin();
  des1 = _afp_umul_short( src1, FDIGIT( *r_pos2 ) );
  for( r_pos2++, disp = 1; r_pos2 != src2->rend(); disp++, r_pos2++ )
  {
    if( FDIGIT( *r_pos2 ) != 0 )
    {
      tmp = _afp_umul_short( src1, FDIGIT( *r_pos2 ) );
      tmp.append( disp, FCHARACTER( 0 ) );
      des1 = _afp_uadd( &des1, &tmp );
    }
  }
  _afp_strip_leading_zeros( &des1 ); 

  return des1;
}

// Description:
//   Multiply two unsigned decimal strings
//   Optimized: Used early out add and multiplication w. zero
//   This is considerable faster than the previous methode and is used now
//
static std::string 
_afp_umul_fourier( std::string *src1, std::string *src2 )
{
  unsigned short ireg = 0;
  std::string des1;
  std::string::iterator pos;
  unsigned int n, l, l1, l2;
  int j;
  double *a, *b, cy;

  l1 = (unsigned int) src1->length();
  l2 = (unsigned int) src2->length();
  l = l1 < l2 ? l2 : l1;
  for( n = 1; n < l; n <<= 1 ) ;
  n <<= 1;
  a = new double [n];
  b = new double [n];
  for( l=0, pos = src1->begin(); pos != src1->end(); pos++ ) 
  {
    a[l++] = (double)FDIGIT(*pos);
  }
  for( ; l < n; ) 
  {
    a[l++] = (double)0;
  }
  for( l=0, pos = src2->begin(); pos != src2->end(); pos++ ) 
  {
    b[l++] = (double)FDIGIT(*pos);
  }
  for( ; l < n; ) 
  {
    b[l++] = (double)0;
  }
  _int_real_fourier( a, n, 1 );
  _int_real_fourier( b, n, 1 );
  b[0] *= a[0];
  b[1] *= a[1];
  for( j = 2; j < (int)n; j += 2 )
  {
    double t;
    b[j]=(t=b[j])*a[j]-b[j+1]*a[j+1];
    b[j+1]=t*a[j+1]+b[j+1]*a[j];
  }
  _int_real_fourier( b, n, -1 );
  for( cy=0, j= (int)n - 1; j >= 0; j-- )
  {
    double t;
    t=b[j]/(n>>1)+cy+0.5;
    cy=(unsigned long)( t/ F_RADIX );
    b[j]=t-cy*F_RADIX;
  }

  ireg = (unsigned short)cy;
  if( ireg != 0 )
  {
    des1.append( 1, FCHARACTER((unsigned char) ireg ) );
  }
  for( j = 0; j < (int)(l1 + l2 - 1); j++ )
  {
    des1.append( 1, FCHARACTER((unsigned char) b[ j ] ) );
  }
  _afp_strip_leading_zeros( &des1 );
  delete [] a;
  delete [] b;

  return des1;
}

// Description:
//   divide a short integer into a floating point string (mantissa)
//   Short Division: The digit d [1..F_RADIX] is divide up into the unsigned decimal string
//
static std::string 
_afp_udiv_short( unsigned int *remaind, std::string *src1, unsigned int d )
{
  int i, ir;
  std::string::iterator s1_pos;
  std::string des1;

  if( d > F_RADIX )
  {
    throw afp::out_of_range();
    des1.insert( (std::string::size_type)0, 1, FCHARACTER(0) );
    return des1;
  }

  if( d == 0 )
  {
    throw afp::divide_by_zero();
    des1.insert( (std::string::size_type)0, 1, FCHARACTER(0) );
    return des1;
  }

  des1.erase();
  s1_pos = src1->begin();

  ir = 0;
  for(; s1_pos != src1->end(); s1_pos++ )
  {
    i = F_RADIX * ir + FDIGIT( *s1_pos );
    des1 += FCHARACTER( (unsigned char)( i / d ) );
    ir = i % d;
  }

  *remaind = ir;
  return des1;
}

// Description:
//   Convert n discrete double data into a fourier transform data set
//   n must be a power of 2
//  isign must be 1 (in) or -1 (out)
//
static void 
_int_real_fourier( double data[], unsigned int n, int isign )
{
  unsigned int i;
  double theta, c1 = 0.5, c2;
  std::complex<double> w, wp, h1, h2;

  theta = 3.14159265358979323846264 / (double)( n >> 1 );
  if( isign == 1 )
  {
    c2 = -c1;
    _int_fourier( (std::complex<double> *)data, n >> 1, 1 );
  }
  else
  {
    c2 = c1;
    theta = -theta;
  }
  wp = std::complex<double>( -2.0 * sin( 0.5 * theta ) * sin( 0.5 * theta ), sin( theta ) );
  w = std::complex<double> ( 1 + wp.real(), wp.imag() );
  for( i = 1; i < (n>>2); i++ )
  {
    int i1, i2, i3, i4;
    std::complex<double> tc;

    i1 = i + i;
    i2 = i1 + 1;
    i3 = n + 1 - i2;
    i4 = i3 + 1;
    h1 = std::complex<double> ( c1 * ( data[i1] + data[i3] ), c1 * ( data[i2]-data[i4]));
    h2 = std::complex<double> ( -c2 * ( data[i2]+data[i4] ), c2 * ( data[i1]-data[i3]));
    tc = w * h2;
    data[i1]=h1.real()+tc.real();
    data[i2]=h1.imag()+tc.imag();
    data[i3]=h1.real() - tc.real();
    data[i4]=-h1.imag() + tc.imag();
    w *= ( std::complex<double>(1) + wp );
  }
  if( isign == 1 )
  {
    double t;
    data[0] = (t=data[0]) + data[1];
    data[1] = t - data[1];
  }
  else
  {
    double t;
    data[0]=c1*((t=data[0])+data[1]);
    data[1]=c1*(t-data[1]);
    _int_fourier( (std::complex<double> *)data, n>>1, -1 );
  }
}


// Description:
//   _int_fourier do the fourier transformation
//   Wk=exp(2* PI *i *j )  j=0..n/2-1
//   exp( k * i *j ) => exp( k * i * (j-1) + k * i ) => exp( t + o ) for each new step
//   exp( t + o ) => exp(t)-exp(t)*( 1 - cos(o) -isin(o) ) => exp(t)-exp(t)*(a-ib)
//   => exp(t)+exp(t)*(-a+ib) => exp(t)( 1 + (-a+b) )
//   sin(t+o)=sin(t)+[-a*sin(t)+b*cos(t)]
//   a=2sin^2(o/2), b=sin(o)
//   n must be a power of 2
//
static void 
_int_fourier( std::complex<double> data[], unsigned int n, int isign )
{
  double theta;
  std::complex<double> w, wp;
  unsigned long mh, m, r, j, i;

  _int_reverse_binary( data, n );

  for( m = 2; n >= m; m <<= 1 )
  {
    theta = isign * 2 * 3.14159265358979323846264 / m;
    wp = std::complex<double>( -2.0 * sin( 0.5 * theta ) * sin( 0.5 * theta ), sin( theta ) );
    w = std::complex<double> ( 1, 0 ); // exp(0) == exp( isign*2*PI*i/mmax * m-1 )
    mh = m >> 1;

    for( j = 0; j < mh; j++ )      // m/2 iteration
    {
      for( r = 0; r <= n - m; r += m )
      {
        std::complex<double> tempc;
        i = r + j;
        tempc = w * data[ i + mh ];              // u=data[i]; v=data[j]*w; data[i]=u+v;data[j]=u-v;
        data[ i + mh ] = data[ i ] - tempc;
        data[ i ] += tempc;
      }

      w =  w * wp + w;  // w = w(1+wp) ==> w *=1+wp;
    }
  }
}

// Description:
//   Reverse binary permute
//   n must be a power of 2
//
static void 
_int_reverse_binary( std::complex<double> data[], unsigned int n )
{
  unsigned int i, j, m;

  if( n <=2 ) return;

  j = 1;
  for( i = 1; i < n; i++ )
  {
    if( j > i ) 
    {
      std::swap( data[ j - 1 ], data[ i - 1 ] );
    }
    for( m = n >> 1; m >= 2 && j > m; m >>= 1 )
    {
      j -= m;
    }
    j += m;
  }
}

//////////////////////////////////////////////////////////////////////////////////////
///
/// END FLOATING POINT CORE FUNCTIONS
///
///
//////////////////////////////////////////////////////////////////////////////////////
