#ifndef Icd4_h
#define Icd4_h

#include <ostream>
using std::ostream;

static const long  Basis           = 10000L; // Basis  per element van m_data
static const short Digits          = 4;      // Digits per element van m_data
static const short Lengte          = 20;      // Totale opslag lengte in m_data
static const short KommaPositie    = 10;      // Geimpliceerde positie van decimale ','
// icd heeft nu als vorm: 0 0 0 0 0, 0 0 0 0 0
// Voorbeeld 2,5 is       0 0 0 0 5, 2 0 0 0 0

// Interne registers worden in 64 bits verwerkt
#define int64 __int64

class Icd4
{
public:
  // Default constructor.
  Icd4();

  // ICD van een long
  Icd4(const long waarde, const long restWaarde = 0);

  // ICD van een 64bits int
  Icd4(const int64 waarde,const int64 restWaarde = 0);

  // Copy constructor.
  Icd4(const Icd4& icd);

  // ICD van een double
  Icd4(const double waarde);

  // Assignment-constructor van de klasse Icd4.
  Icd4(const CString& p_string,bool p_fromDB = false);

  // Destructor van de klasse Icd4.
  ~Icd4();

  // CONSTANTS

  static Icd4 PI();     // Radius verhouding van een cirkel
  static Icd4 LN2();    // Natuurlijke logarithme van 2
  static Icd4 LN10();   // Natuurlijke logarithme van 10

  // OPERATOREN

  const Icd4& operator=( const Icd4& icd);
  const Icd4& operator=( const long waarde);
  const Icd4& operator=( const double waarde);
  const Icd4& operator=( const CString& str);
  const Icd4& operator+=(const Icd4& icd);
  const Icd4& operator-=(const Icd4& icd);
  const Icd4& operator*=(const Icd4& icd);
  const Icd4& operator/=(const Icd4& icd);
  // Prefix unary minus (negation)
  const Icd4  operator-() const;
  // Postfix increment
  const Icd4 operator++(int);
  // Prefix increment
  Icd4& operator++();
  // Postfix decrement
  const Icd4 operator--(int);
  // Prefix decrement
  Icd4& operator--();

  // vergelijkingsoperators
  const bool operator==(const Icd4& icd) const;
  const bool operator!=(const Icd4& icd) const;
  const bool operator<(const Icd4& icd) const;
  const bool operator>(const Icd4& icd) const;
  const bool operator<=(const Icd4& icd) const;
  const bool operator>=(const Icd4& icd) const;

  // standaard output operator
  friend ostream& operator<<(ostream& os, const Icd4& icd);

  // MATHEMATISCHE FUNCTIES

  // Floor functie
  Icd4     Floor() const;
  // Ceiling functie
  Icd4     Ceil() const;
  // Vierkants wortel
  Icd4     VierkantsWortel() const;
  // Verheffen tot een macht.
  Icd4     Macht(const Icd4& macht) const;
  // Absolute waarde (ABS)
  Icd4     AbsoluteWaarde() const;
  // Reciproke / Inverse = 1/x
  Icd4     Reciproke() const;
  // Natuurlijke logarithme
  Icd4     Log() const;
  // Exponent e tot de macht
  Icd4     Exp() const;
  // Log with base 10
  Icd4     Log10() const;

  // TRIGONOMETRISCHE FUNCTIES
  
  // Sinus van de hoek
  Icd4     Sinus() const;
  // Cosinus van de hoek
  Icd4     Cosinus() const;
  // Tangens van de hoek
  Icd4     Tangent() const;
  // Arcsinus (hoek) van de verhouding
  Icd4     ArcSinus() const;
  // Arccosinus (hoek) van de herhouding
  Icd4     ArcCosinus() const;
  // Arctangens (hoek) van de verhouding
  Icd4     ArcTangens() const;
  // Hoek van twee punten.
  Icd4     ArcTangens2Punten(Icd4 p_x) const;

  // OPVRAGEN ALS IETS ANDERS
  // Deel voor de komma opvragen (floor indien groter dan 0)
  Icd4     WaardeVoorKomma() const;
  // Deel achter de komman opvragen
  Icd4     WaardeAchterKomma() const;
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
  int   GeefLengte();
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
  // Berekent de exponent van het Icd4 getal
  int   Exponent() const;
  // Berkent de 10macht mantissa
  Icd4   Mantissa() const;

  // BASIS OPERATIES.
  // Moeten public zijn om operatoren mogelijk te maken

  // optel operatie
  Icd4 Add(const Icd4& icd) const;
  // aftrek operatie
  Icd4 Sub(const Icd4& icd) const;
  // vermenigvuldiging operatie
  Icd4 Mul(const Icd4& icd) const;
  // deling operatie
  Icd4 Div(const Icd4& icd) const;
  // modulus operatie
  Icd4 Mod(const Icd4& icd) const;

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
  // Intern m_data herformatteren zodat ze < Basis zijn
  void Herformatteer();
  // Leeg maken naar 0.0
  void MaakLeeg();
  // Vermenigvuldig Icd4 met 1 (een) multiplier deel
  const Icd4 Multi(const long multiplier) const;
  // Grensgeval waarbij door afronding Icd4 buiten precisie valt
  const bool IsInhoudGrensGeval() const;
  // Bepaalt de tekens voor een basis-operatie
  static void PositioneerArgumenten(Icd4& arg1,
                                    Icd4& arg2,
                                    Teken& tekenResultaat,
                                    SoortOperator& soortOperator);
  // Telt twee ICD's bij elkaar op zonder op het teken te letten
  static const Icd4    TelPositiefOp(const Icd4& arg1, const Icd4& arg2);
  // Trekt een ICD van een andere af zonder op het teken te letten
  static const Icd4    TrekPositiefAf(const Icd4& arg1, const Icd4& arg2);
  // Vermenigvuldigt twee ICD's zonder op het teken te letten
  static const Icd4    VermenigvuldigPositief(const Icd4& arg1, const Icd4& arg2);
  // Deelt een ICD door een andere zonder op het teken te letten
  static const Icd4    DeelPositief(const Icd4& arg1, const Icd4& arg2);
  // Bepaalt het teken van een vermenigvuldiging of deling
  static const Teken  BepaalTeken(const Icd4& arg1, const Icd4& arg2);
  // Deler en noemer op hetzelfde exponent brengen
  static const Icd4    TrekGelijk(const Icd4& arg1, const long verschil);
  // Mantissa door 10 delen (schuifoperatie)
  void Div10();
  // Mantissa met 10 vermenigvuldigen (Schuifoperatie)
  void Mult10();

  // String <-> Long omzetten
  CString LongNaarString(long p_getal) const;
  long    StringNaarLong(CString& p_string) const;

  // Afbreek criterium voor interne iteraties
  Icd4&    Epsilon(long p_fraction) const;

  //
  // Datamembers: Opslag van het getal
  //
  Teken         m_teken;
  int           m_lengte;
  int           m_precisie;
  unsigned long m_data[Lengte];
};

// Getters

inline int
Icd4::GeefLengte()
{
  return m_lengte;
}

inline int
Icd4::Precisie()
{
  return m_precisie;
}

inline int 
Icd4::GeefMaxGroote(int precisie)
{
  return ((Lengte * 2) + precisie); 
}

// Overloaded basic operators

inline Icd4 operator + (const Icd4& lhs, const Icd4& rhs)
{ 
  return lhs.Add(rhs); 
}

inline Icd4 operator - (const Icd4& lhs, const Icd4& rhs)
{ 
  return lhs.Sub(rhs); 
}

inline Icd4 operator * (const Icd4& lhs, const Icd4& rhs)
{ 
  return lhs.Mul(rhs); 
}

inline Icd4 operator / (const Icd4& lhs, const Icd4& rhs)
{ 
  return lhs.Div(rhs); 
}

inline Icd4 operator % (const Icd4& lhs, const Icd4& rhs)
{ 
  return lhs.Mod(rhs); 
}

// Overloaded math precision floating point functions equivalent with the std C functions
// Overloaded to work with the ICD number class, always yielding a Icd4 number.

inline Icd4 modf(Icd4 p_number, Icd4* p_intpart)
{
  *p_intpart = p_number.WaardeVoorKomma();
  return p_number.WaardeAchterKomma();
}

inline Icd4 fmod(Icd4 p_number,Icd4 p_divisor)
{
  return p_number.Mod(p_divisor);
}

inline Icd4 floor(Icd4 p_number)
{
  return p_number.Floor();
}

inline Icd4 ceil(Icd4 p_number)
{
  return p_number.Ceil();
}

inline Icd4 fabs(Icd4 p_number)
{
  return p_number.AbsoluteWaarde();
}

inline Icd4 sqrt(Icd4 p_number)
{
  return p_number.VierkantsWortel();
}

inline Icd4 log10(Icd4 p_number)
{
  return p_number.Log10();
}

inline Icd4 log(Icd4 p_number)
{
  return p_number.Log();
}

inline Icd4 exp(Icd4 p_number)
{
  return p_number.Exp();
}

inline Icd4 pow(Icd4 p_number,Icd4  p_power)
{
  return p_number.Macht(p_power);
}

inline Icd4 frexp(Icd4 p_number,int* p_exponent)
{
  *p_exponent = p_number.Exponent();
  return p_number.Mantissa();
}

inline Icd4 ldexp(Icd4 p_number,int p_power)
{
  if(p_power == 0)
  {
    return p_number;
  }
  if(p_power > 0 && p_power <= 31)
  {
    return p_number * Icd4((long) (1 << p_power));
  }
  return p_number * pow(Icd4(2L),Icd4((long)p_power));
}

// Overloaded trigonometric functions on a Icd4 number

inline Icd4 atan (Icd4 p_number) 
{ 
  return p_number.ArcTangens(); 
}

inline Icd4 atan2(Icd4 p_y,Icd4 p_x)
{
  return p_y.ArcTangens2Punten(p_x);
}

inline Icd4 asin(Icd4 p_number)
{
  return p_number.ArcSinus();
}

inline Icd4 acos(Icd4 p_number)
{
  return p_number.ArcCosinus();
}

inline Icd4 sin(Icd4 p_number)
{
  return p_number.Sinus();
}

inline Icd4 cos(Icd4 p_number)
{
  return p_number.Cosinus();
}

inline Icd4 tan(Icd4 p_number)
{
  return p_number.Tangent();
}

#endif  // Icd4_h