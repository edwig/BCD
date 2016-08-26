#ifndef Icd_h
#define Icd_h

#include <ostream>
using std::ostream;

static const long  icdBasis        = 100000000L; // Basis  per element van m_data
static const short icdDigits       = 8;          // Digits per element van m_data
static const short icdLengte       = 10;         // Totale opslag lengte in m_data
static const short icdKommaPositie = 5;          // Geimpliceerde positie van decimale ','
// icd heeft nu als vorm: 0 0 0 0 0, 0 0 0 0 0
// Voorbeeld 2,5 is       0 0 0 0 5, 2 0 0 0 0

// Interne registers worden in 64 bits verwerkt
#define int64 __int64

class Icd
{
public:
  // Default constructor.
  Icd();

  // ICD van een long
  Icd(const long waarde, const long restWaarde = 0);

  // ICD van een 64bits int
  Icd(const int64 waarde,const int64 restWaarde = 0);

  // Copy constructor.
  Icd(const Icd& icd);

  // ICD van een double
  Icd(const double waarde);

  // Assignment-constructor van de klasse Icd.
  Icd(const CString& p_string,bool p_fromDB = false);

  // Destructor van de klasse Icd.
  ~Icd();

  // CONSTANTS

  static Icd PI();     // Radius verhouding van een cirkel
  static Icd LN2();    // Natuurlijke logarithme van 2
  static Icd LN10();   // Natuurlijke logarithme van 10

  // OPERATOREN

  const Icd& operator=( const Icd& icd);
  const Icd& operator=( const long waarde);
  const Icd& operator=( const double waarde);
  const Icd& operator=( const CString& str);
  const Icd& operator+=(const Icd& icd);
  const Icd& operator-=(const Icd& icd);
  const Icd& operator*=(const Icd& icd);
  const Icd& operator/=(const Icd& icd);
  // Prefix unary minus (negation)
  const Icd  operator-() const;
  // Postfix increment
  const Icd operator++(int);
  // Prefix increment
  Icd& operator++();
  // Postfix decrement
  const Icd operator--(int);
  // Prefix decrement
  Icd& operator--();

  // vergelijkingsoperators
  const bool operator==(const Icd& icd) const;
  const bool operator!=(const Icd& icd) const;
  const bool operator<(const Icd& icd) const;
  const bool operator>(const Icd& icd) const;
  const bool operator<=(const Icd& icd) const;
  const bool operator>=(const Icd& icd) const;

  // standaard output operator
  friend ostream& operator<<(ostream& os, const Icd& icd);

  // MATHEMATISCHE FUNCTIES

  // Floor functie
  Icd     Floor() const;
  // Ceiling functie
  Icd     Ceil() const;
  // Vierkants wortel
  Icd     VierkantsWortel() const;
  // Verheffen tot een macht.
  Icd     Macht(const Icd& macht) const;
  // Absolute waarde (ABS)
  Icd     AbsoluteWaarde() const;
  // Reciproke / Inverse = 1/x
  Icd     Reciproke() const;
  // Natuurlijke logarithme
  Icd     Log() const;
  // Exponent e tot de macht
  Icd     Exp() const;
  // Log with base 10
  Icd     Log10() const;

  // TRIGONOMETRISCHE FUNCTIES
  
  // Sinus van de hoek
  Icd     Sinus() const;
  // Cosinus van de hoek
  Icd     Cosinus() const;
  // Tangens van de hoek
  Icd     Tangent() const;
  // Arcsinus (hoek) van de verhouding
  Icd     ArcSinus() const;
  // Arccosinus (hoek) van de herhouding
  Icd     ArcCosinus() const;
  // Arctangens (hoek) van de verhouding
  Icd     ArcTangens() const;
  // Hoek van twee punten.
  Icd     ArcTangens2Punten(Icd p_x) const;

  // OPVRAGEN ALS IETS ANDERS
  // Deel voor de komma opvragen (floor indien groter dan 0)
  Icd     WaardeVoorKomma() const;
  // Deel achter de komman opvragen
  Icd     WaardeAchterKomma() const;
  // Opvragen geconverteerd als double
  double  AlsDouble() const;
  // Opvragen geconverteerd als long
  long    AlsLong() const;
  // Opvragen geconverteerd als 64 bits long
  int64   AlsInt64() const;
  // Opvragen als mathematische string (met .)
  CString AlsString() const;
  // Opvragen als display string (volgens desktop locale)
  CString AlsDisplayString() const;
  // Hier niet gebruikt. 
//CString AlsSQLString() const;
  
  // PRECISIE WIJZIGEN

  // Verander de lengte en precisie van de ICD
  void  ZetLengteEnPrecisie(int lengte = 80, int precisie = 40);
  // Afronden op decimalen en precisie
  long  RoundDecimal(long decimal,int precsion);

  // GETTER FUNCTIES

  // Is de waarde van de ICD 0.0 ?
  // maakt geen onderscheid tussen +0 en -0
  bool  IsNul() const; 
  // Geeft het teken terug als 0 (= 0.0), 1 (groter dan 0) of -1 (kleiner dan 0)
  int   GeefTeken() const;
  // Totale lengte (voor en na de komma)
  int   Lengte();
  // Precisie na de komma
  int   Precisie();
  // Geeft max grootte van een ICD
  static int GeefMaxGroote(int precisie = 0);
  // Geeft aan of een ICD in een long past qua waarde
  bool  PastInLong() const;
  // Geeft aan of een ICD in een int64 past qua waarde
  bool  PastInInt64() const;
  // Geeft aan of het decimaal deel ongelijk is aan nul (geen scalar)
  bool  HeeftDecimalen() const;
  // Berekent de exponent van het Icd getal
  int   Exponent() const;
  // Berkent de 10macht mantissa
  Icd   Mantissa() const;

  // BASIS OPERATIES.
  // Moeten public zijn om operatoren mogelijk te maken

  // optel operatie
  Icd Add(const Icd& icd) const;
  // aftrek operatie
  Icd Sub(const Icd& icd) const;
  // vermenigvuldiging operatie
  Icd Mul(const Icd& icd) const;
  // deling operatie
  Icd Div(const Icd& icd) const;
  // modulus operatie
  Icd Mod(const Icd& icd) const;

private:
  enum Teken          { Positief, Negatief  };
  enum SoortOperator  { Optellen, Aftrekken };
  
  // Eigen abs(long) functie
  long long_abs(const long waarde) const;
  // Eigen long tot de macht 1,2,3
  long long_pow(long basis,int operand) const;
  // Eigen log10 functie
  long long_log10(long waarde) const;
  
  // Zet een long in de ICD
  void ZetWaardeLong  (const long waarde, const long restWaarde);
  // Zet een double in de ICD
  void ZetWaardeDouble(const double waarde);
  // Zet een string in de ICD
  void ZetWaardeString(CString waarde, bool fromDB = false);
  // Zet een int64 in de ICD
  void ZetWaardeInt64 (const int64 waarde, const int64 restWaarde);

  // Verhef tot een 10e macht
  void PasEFactorToe(int factor);
  // Intern m_data herformatteren zodat ze < icdBasis zijn
  void Herformatteer();
  // Leeg maken naar 0.0
  void MaakLeeg();
  // Vermenigvuldig Icd met 1 (een) multiplier deel
  const Icd Multi(const long multiplier) const;
  // Grensgeval waarbij door afronding Icd buiten precisie valt
  const bool IsInhoudGrensGeval() const;
  // Bepaalt de tekens voor een basis-operatie
  static void PositioneerArgumenten(Icd& arg1,
                                    Icd& arg2,
                                    Teken& tekenResultaat,
                                    SoortOperator& soortOperator);
  // Telt twee ICD's bij elkaar op zonder op het teken te letten
  static const Icd    TelPositiefOp(const Icd& arg1, const Icd& arg2);
  // Trekt een ICD van een andere af zonder op het teken te letten
  static const Icd    TrekPositiefAf(const Icd& arg1, const Icd& arg2);
  // Vermenigvuldigt twee ICD's zonder op het teken te letten
  static const Icd    VermenigvuldigPositief(const Icd& arg1, const Icd& arg2);
  // Deelt een ICD door een andere zonder op het teken te letten
  static const Icd    DeelPositief(const Icd& arg1, const Icd& arg2);
  // Bepaalt het teken van een vermenigvuldiging of deling
  static const Teken  BepaalTeken(const Icd& arg1, const Icd& arg2);
  // Deler en noemer op hetzelfde exponent brengen
  static const Icd    TrekGelijk(const Icd& arg1, const long verschil);
  // Mantissa door 10 delen (schuifoperatie)
  void Div10();
  // Mantissa met 10 vermenigvuldigen (Schuifoperatie)
  void Mult10();

  // String <-> Long omzetten
  CString LongNaarString(long p_getal) const;
  long    StringNaarLong(CString& p_string) const;

  // Afbreek criterium voor interne iteraties
  Icd&    Epsilon(long p_fraction) const;

  //
  // Datamembers: Opslag van het getal
  //
  Teken         m_teken;
  int           m_lengte;
  int           m_precisie;
  unsigned long m_data[icdLengte];
};

// Getters

inline int
Icd::Lengte()
{
  return m_lengte;
}

inline int
Icd::Precisie()
{
  return m_precisie;
}

inline int 
Icd::GeefMaxGroote(int precisie)
{
  return ((icdLengte * 2) + precisie); 
}

// Overloaded basic operators

inline Icd operator + (const Icd& lhs, const Icd& rhs)
{ 
  return lhs.Add(rhs); 
}

inline Icd operator - (const Icd& lhs, const Icd& rhs)
{ 
  return lhs.Sub(rhs); 
}

inline Icd operator * (const Icd& lhs, const Icd& rhs)
{ 
  return lhs.Mul(rhs); 
}

inline Icd operator / (const Icd& lhs, const Icd& rhs)
{ 
  return lhs.Div(rhs); 
}

inline Icd operator % (const Icd& lhs, const Icd& rhs)
{ 
  return lhs.Mod(rhs); 
}

// Overloaded math precision floating point functions equivalent with the std C functions
// Overloaded to work with the ICD number class, always yielding a Icd number.

inline Icd modf(Icd p_number, Icd* p_intpart)
{
  *p_intpart = p_number.WaardeVoorKomma();
  return p_number.WaardeAchterKomma();
}

inline Icd fmod(Icd p_number,Icd p_divisor)
{
  return p_number.Mod(p_divisor);
}

inline Icd floor(Icd p_number)
{
  return p_number.Floor();
}

inline Icd ceil(Icd p_number)
{
  return p_number.Ceil();
}

inline Icd fabs(Icd p_number)
{
  return p_number.AbsoluteWaarde();
}

inline Icd sqrt(Icd p_number)
{
  return p_number.VierkantsWortel();
}

inline Icd log10(Icd p_number)
{
  return p_number.Log10();
}

inline Icd log(Icd p_number)
{
  return p_number.Log();
}

inline Icd exp(Icd p_number)
{
  return p_number.Exp();
}

inline Icd pow(Icd p_number,Icd  p_power)
{
  return p_number.Macht(p_power);
}

inline Icd frexp(Icd p_number,int* p_exponent)
{
  *p_exponent = p_number.Exponent();
  return p_number.Mantissa();
}

inline Icd ldexp(Icd p_number,int p_power)
{
  if(p_power == 0)
  {
    return p_number;
  }
  if(p_power > 0 && p_power <= 31)
  {
    return p_number * Icd((long) (1 << p_power));
  }
  return p_number * pow(Icd(2L),Icd((long)p_power));
}

// Overloaded trigonometric functions on a Icd number

inline Icd atan (Icd p_number) 
{ 
  return p_number.ArcTangens(); 
}

inline Icd atan2(Icd p_y,Icd p_x)
{
  return p_y.ArcTangens2Punten(p_x);
}

inline Icd asin(Icd p_number)
{
  return p_number.ArcSinus();
}

inline Icd acos(Icd p_number)
{
  return p_number.ArcCosinus();
}

inline Icd sin(Icd p_number)
{
  return p_number.Sinus();
}

inline Icd cos(Icd p_number)
{
  return p_number.Cosinus();
}

inline Icd tan(Icd p_number)
{
  return p_number.Tangent();
}

#endif  // Icd_h