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

class icd
{
public:
  // Default constructor.
  icd();

  // ICD van een long
  icd(const long waarde, const long restWaarde = 0);

  // ICD van een 64bits int
  icd(const int64 waarde,const int64 restWaarde = 0);

  // Copy constructor.
  icd(const icd& icd);

  // ICD van een double
  icd(const double waarde);

  // Assignment-constructor van de klasse Icd.
  icd(const CString& p_string,bool p_fromDB = false);

  // Destructor van de klasse Icd.
  ~icd();

  // CONSTANTS

  static icd PI();     // Radius verhouding van een cirkel
  static icd LN2();    // Natuurlijke logarithme van 2
  static icd LN10();   // Natuurlijke logarithme van 10

  // OPERATOREN

  const icd& operator=( const icd& icd);
  const icd& operator=( const long waarde);
  const icd& operator=( const double waarde);
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

  // vergelijkingsoperators
  const bool operator==(const icd& icd) const;
  const bool operator!=(const icd& icd) const;
  const bool operator<(const icd& icd) const;
  const bool operator>(const icd& icd) const;
  const bool operator<=(const icd& icd) const;
  const bool operator>=(const icd& icd) const;

  // standaard output operator
  friend ostream& operator<<(ostream& os, const icd& icd);

  // MATHEMATISCHE FUNCTIES

  // Floor functie
  icd     Floor() const;
  // Ceiling functie
  icd     Ceil() const;
  // Vierkants wortel
  icd     VierkantsWortel() const;
  // Verheffen tot een macht.
  icd     Macht(const icd& macht) const;
  // Absolute waarde (ABS)
  icd     AbsoluteWaarde() const;
  // Reciproke / Inverse = 1/x
  icd     Reciproke() const;
  // Natuurlijke logarithme
  icd     Log() const;
  // Exponent e tot de macht
  icd     Exp() const;
  // Log with base 10
  icd     Log10() const;

  // TRIGONOMETRISCHE FUNCTIES
  
  // Sinus van de hoek
  icd     Sinus() const;
  // Cosinus van de hoek
  icd     Cosinus() const;
  // Tangens van de hoek
  icd     Tangent() const;
  // Arcsinus (hoek) van de verhouding
  icd     ArcSinus() const;
  // Arccosinus (hoek) van de herhouding
  icd     ArcCosinus() const;
  // Arctangens (hoek) van de verhouding
  icd     ArcTangens() const;
  // Hoek van twee punten.
  icd     ArcTangens2Punten(icd p_x) const;

  // OPVRAGEN ALS IETS ANDERS
  // Deel voor de komma opvragen (floor indien groter dan 0)
  icd     WaardeVoorKomma() const;
  // Deel achter de komman opvragen
  icd     WaardeAchterKomma() const;
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
  icd   Mantissa() const;

  // BASIS OPERATIES.
  // Moeten public zijn om operatoren mogelijk te maken

  // optel operatie
  icd Add(const icd& icd) const;
  // aftrek operatie
  icd Sub(const icd& icd) const;
  // vermenigvuldiging operatie
  icd Mul(const icd& icd) const;
  // deling operatie
  icd Div(const icd& icd) const;
  // modulus operatie
  icd Mod(const icd& icd) const;

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
  const icd Multi(const long multiplier) const;
  // Grensgeval waarbij door afronding Icd buiten precisie valt
  const bool IsInhoudGrensGeval() const;
  // Bepaalt de tekens voor een basis-operatie
  static void PositioneerArgumenten(icd& arg1,
                                    icd& arg2,
                                    Teken& tekenResultaat,
                                    SoortOperator& soortOperator);
  // Telt twee ICD's bij elkaar op zonder op het teken te letten
  static const icd    TelPositiefOp(const icd& arg1, const icd& arg2);
  // Trekt een ICD van een andere af zonder op het teken te letten
  static const icd    TrekPositiefAf(const icd& arg1, const icd& arg2);
  // Vermenigvuldigt twee ICD's zonder op het teken te letten
  static const icd    VermenigvuldigPositief(const icd& arg1, const icd& arg2);
  // Deelt een ICD door een andere zonder op het teken te letten
  static const icd    DeelPositief(const icd& arg1, const icd& arg2);
  // Bepaalt het teken van een vermenigvuldiging of deling
  static const Teken  BepaalTeken(const icd& arg1, const icd& arg2);
  // Deler en noemer op hetzelfde exponent brengen
  static const icd    TrekGelijk(const icd& arg1, const long verschil);
  // Mantissa door 10 delen (schuifoperatie)
  void Div10();
  // Mantissa met 10 vermenigvuldigen (Schuifoperatie)
  void Mult10();

  // String <-> Long omzetten
  CString LongNaarString(long p_getal) const;
  long    StringNaarLong(CString& p_string) const;

  // Afbreek criterium voor interne iteraties
  icd&    Epsilon(long p_fraction) const;

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
icd::Lengte()
{
  return m_lengte;
}

inline int
icd::Precisie()
{
  return m_precisie;
}

inline int 
icd::GeefMaxGroote(int precisie)
{
  return ((icdLengte * 2) + precisie); 
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
// Overloaded to work with the ICD number class, always yielding a Icd number.

inline icd modf(icd p_number, icd* p_intpart)
{
  *p_intpart = p_number.WaardeVoorKomma();
  return p_number.WaardeAchterKomma();
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
  return p_number.AbsoluteWaarde();
}

inline icd sqrt(icd p_number)
{
  return p_number.VierkantsWortel();
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
  return p_number.Macht(p_power);
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

// Overloaded trigonometric functions on a Icd number

inline icd atan (icd p_number) 
{ 
  return p_number.ArcTangens(); 
}

inline icd atan2(icd p_y,icd p_x)
{
  return p_y.ArcTangens2Punten(p_x);
}

inline icd asin(icd p_number)
{
  return p_number.ArcSinus();
}

inline icd acos(icd p_number)
{
  return p_number.ArcCosinus();
}

inline icd sin(icd p_number)
{
  return p_number.Sinus();
}

inline icd cos(icd p_number)
{
  return p_number.Cosinus();
}

inline icd tan(icd p_number)
{
  return p_number.Tangent();
}

#endif  // Icd_h