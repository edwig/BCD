//  AFP
//  Arbitrary Floating Point Precision Number class 
//  An Arbitrary float always has the format [sign][digit][.[digit]*][E[sign][digits]+] where sign is either '+' or '-'
//  And is always stored in normalized mode after an operation or conversion
//  The length or the representation is always >= 2
//  A null string is considered as an error and an exception is thrown
//  Floating Point Numbers is stored in BASE 10 or BASE 256 depends of the F_RADIX setting
//
// Improvement of the float_precision library of Henrik Vestermark (hve@hvks.com). See www.hvks.com
//
#pragma once
#include <string>
#include <complex>
// #include <ostream>
// #include <istream>

// RADIX can either be 2,10 or 256!
static const int BASE_8   = 8;
static const int BASE_10  = 10;
static const int BASE_16  = 16;
static const int BASE_256 = 256;

#define MAX(p_number,y)  ((p_number)>(y)?(p_number):(y))
#define MIN(p_number,y)  ((p_number)>(y)?(y):(p_number))

/// The four different rounding modes
/// # ROUND_NEAR  Rounded result is the closest to the infinitely precise result.
/// # ROUND_DOWN  Rounded result is close to but no greater than the infinitely precise result.
/// # ROUND_UP    Rounded result is close to but no less than the infinitely precise result.
/// # ROUND ZERO  Rounded result is close to but no greater in absolute value than the infinitely precise result.
enum round_mode { ROUND_NEAR, ROUND_UP, ROUND_DOWN, ROUND_ZERO };

// The build in constants!. See the function _float_table()
enum table_type { _LN2, _LN10, _PI };

// Default precision of 20 Radix digits if not specified
static const int PRECISION = 20;

// INTERNAL AFP RADIX
// Can be either BASE_10 or BASE_256
// BASE_10  -> Mantissa is in base 10 and rouding works
// BASE_256 -> Bigger numbers that work faster, but rouding doesn't work
// Only define one of the following two lines !!
#define F_RADIX     BASE_10
//#define F_RADIX     BASE_256

// Forward declaration for operators and functions
class afp;

// Arithmetic
afp operator+(const afp& p_left, const afp& p_right);  // Binary
afp operator+(const afp& p_right);                     // Unary 
afp operator-(const afp& p_left, const afp& p_right);  // Binary
afp operator-(const afp& p_right);                     // Unary
afp operator*(const afp& p_left, const afp& p_right);  // Binary
afp operator/(const afp& p_left, const afp& p_right);  // Binary

// Boolean Comparison Operators
bool operator>( const afp& p_left, const afp& p_right);
bool operator<( const afp& p_left, const afp& p_right);
bool operator==(const afp& p_left, const afp& p_right);
bool operator!=(const afp& p_left, const afp& p_right);
bool operator>=(const afp& p_left, const afp& p_right);
bool operator<=(const afp& p_left, const afp& p_right);

// Overloaded math precision floating point functions equivalent with the std C functions
// Overloaded to work with the AFP number class, always yielding a afp number.
afp modf (afp p_number, afp* intpart);
afp fmod (afp p_number, afp p_devisor);
afp floor(afp p_number);
afp ceil (afp p_number);
afp fabs (afp p_number);
afp sqrt (afp p_number);
afp log10(afp p_number);
afp log  (afp p_number);
afp exp  (afp p_number);
afp pow  (afp p_number,afp p_power);
afp frexp(afp p_number,int* p_exponent);
afp ldexp(afp p_number,int p_power);

// Trigonometric functions on a afp number
afp atan (afp p_number);
afp atan2(afp p_y,afp p_x);
afp asin (afp p_number);
afp acos (afp p_number);
afp sin  (afp p_number);
afp cos  (afp p_number);
afp tan  (afp p_number);

// Support functions. Works on afp 

// Take reciprocal of number
afp _afp_inverse(const afp& p_number);
// Get constant LN2,LN10 or PI in x precision decimals
afp _float_table(enum table_type p_type,unsigned int p_precision);
// Conversion of a AFP back to a string
std::string _afp_ftoa( afp& p_number,unsigned int p_digits = PRECISION,int p_threshold = PRECISION);
// Conversion from a string to a AFP
afp _afp_atof(const char* str, unsigned int p_precision, enum round_mode p_mode);
// Conversion of a double to a AFP
afp _afp_dtof(double p_number, unsigned int p_precision, enum round_mode p_mode);

class afp 
{
public:
  // CONSTRUCTORS

  // AFP initialized through an int
  afp(      int    p_number, unsigned int p_precision = PRECISION, enum round_mode p_mode = ROUND_NEAR);
  // AFP initialized through a double
  afp(      double p_number, unsigned int p_precision = PRECISION, enum round_mode p_mode = ROUND_NEAR);
  // AFP initialized through a char
  afp(const char   p_number, unsigned int p_precision = PRECISION, enum round_mode p_mode = ROUND_NEAR);  
  // AFP initialized through a char string
  afp(const char*  p_number, unsigned int p_precision = PRECISION, enum round_mode p_mode = ROUND_NEAR);    
  // AFP initialized through another afp
  afp(const afp&   p_number = afp(0, PRECISION, ROUND_NEAR));
  
  // Set different precision
  unsigned precision(unsigned int p_precision);
  // Assign another AFP to this AFP
  afp assign(afp& p_another);
  // Change the sign of the AFP, returns new sign
  int change_sign();
  // Return the sign of the number
  int sign() const;

  // Coordinate functions
  std::string get_mantissa() const             { return m_number; };      // Copy of mantissa
  std::string *ref_mantissa()                  { return &m_number; }      // Reference of Mantissa
  enum round_mode mode() const                 { return m_rmode; }        // Get the rounding mode
  enum round_mode mode( enum round_mode m )    { return( m_rmode = m ); } // Set the roudding mode
  int exponent() const                         { return m_expo; };        // Get the exponent
  int exponent( int e )                        { return( m_expo = e ); }  // Set the exponent
  unsigned int precision() const               { return m_prec; }         // Get the precision
  
  // CONVERSION OPERATORS
  // Cast to an int
  operator int();     
  // Cast to a double
  operator double();  

  // Essential operators
  afp& operator=(  const afp& p_other);
  afp& operator+=( const afp& p_other);
  afp& operator-=( const afp& p_other);
  afp& operator*=( const afp& p_other);
  afp& operator/=( const afp& p_other);

  // Specialization
  friend std::ostream& operator<<( std::ostream& strm, const afp& d);
  friend std::istream& operator>>( std::istream& strm, afp& d );

  // Exception classes
  class bad_int_syntax   {}; 
  class bad_float_syntax {};
  class out_of_range     {};
  class divide_by_zero   {};
  class domain_error     {};

public: // Should be private
  // Set the mantissa number
  void set_n( std::string mantissa )   { m_number = mantissa; }   

private:
  enum round_mode   m_rmode;  // Rounding mode. Default Round Nearest
  unsigned int      m_prec;   // Number of decimals in mantissa. Default 20, We make a shot cut by assuming the number of digits can't exceed 2^32-1
  int               m_expo;   // Exponent as a power of RADIX (not 2 as in IEEE 754). We make a short cut here and use a standard int to hold 
                              // the exponent. This will allow us exponent in the range from -RADIX^2^31 to  RADIX^2^31. Which should be enough
  std::string       m_number; // The mantissa any length however the fraction point is always after the first digit and is implied
};

