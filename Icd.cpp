// ICD = INTEGER CODED DECIMAL
// Een klasse om met grote gebroken getallen te werken
// Gebroken getallen met een exacte nauwkeurigheid in decimalen
// als in tegenstelling tot binaire benaderingen (float,double)
//
#include "Stdafx.h"
#include <math.h>   // Toch wel nodig voor conversies
#include <float.h>  // en voor snelle benaderingen
#include <limits.h> // Voor maximal grootte long en int64
#include "Icd.h"

//////////////////////////////////////////////////////////////////////////
//
//  ICD CONSTRUCTORS / DESTRUCTORS
//
//////////////////////////////////////////////////////////////////////////

// Icd::Icd
// Omschrijving:   default constructor
// Werking:        Zet de waarde van de Icd op nul dmv aanroep MaakLeeg()
//
Icd::Icd()
{
  MaakLeeg();
}

// Icd::Icd
// Omschrijving:   copy constructor
// Parameters:     const Icd& Icd // te kopieeren Icd
// Werking:        copieert alle datamembers van de opgegeven Icd
//
Icd::Icd(const Icd& Icd)
{
  m_teken    = Icd.m_teken;
  m_lengte   = Icd.m_lengte;
  m_precisie = Icd.m_precisie;
  for(int i = 0; i < icdLengte; i++)
  {
    m_data[i] = Icd.m_data[i];
  }
}

// Icd::Icd
//
// Omschrijving:   copy constructor met 1 of 2 longs (32 bit)
// Parameters:     const long waarde     // waarde voor de komma
//                 const long restWaarde // waarde achter de komma
// Werking:        gebruikt ZetWaardeLong om de waarde van de long(s) in
//                 de Icd te zetten. restWaarde is default 0.
//
Icd::Icd(const long waarde, const long restWaarde)
{
  ZetWaardeLong(waarde, restWaarde);
}

// Icd::Icd
// Copy constructor met een int64 (64 bits) en een optionele restwaarde (32 bit)
Icd::Icd(const int64 waarde, const int64 restWaarde)
{
  ZetWaardeInt64(waarde, restWaarde);
}

// Icd::Icd
// Omschrijving:   copy constructor met double. Pas op: door de afrondfouten van een
//                 double is het niet verstandig om deze constructor te gebruiken, aangezien
//                 het waarde van de Icd ook deze afrondfouten kan bevatten!
// Parameters:     const double waarde // te kopieeren waarde als double
// Werking:        gebruikt ZetWaardeDouble om de waarde van de double in de Icd te zetten.
//
Icd::Icd(const double waarde)
{
  ZetWaardeDouble(waarde);
}

// Icd::Icd
// Omschrijving:   copy constructor met string. (String type uit basisklassen!)
// Parameters:     const String& str // te kopieeren waarde als string
//                 bool  fromDB      // Geeft aan of waarde uit een database (ODBC) kwam
// Werking:        gebruikt ZetWaardeString om de waarde van de string in de Icd te zetten.
//
Icd::Icd(const CString& str,bool fromDB)
{
  ZetWaardeString(str,fromDB);
}

// Icd::~Icd
// Omschrijving:   destructor
// Werking:        doet (nog) niks
//
Icd::~Icd()
{
  // niks, aanwezig voor evt. uitbreiding
}

//////////////////////////////////////////////////////////////////////////
//
// EINDE ICD CONSTRUCTORS / DESTRUCTORS
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// ICD CONSTANTS
//
//////////////////////////////////////////////////////////////////////////

// Icd::PI
// Omschrijving: Verhouding tussen straal en omtrek van een cirkel
// Werking:      Natuurconstante die nooit veranderd
Icd
Icd::PI()
{
  Icd pi;

  pi.m_lengte   = icdLengte * icdDigits;
  pi.m_precisie = icdLengte * icdDigits / 2;
  pi.m_teken    = Positief;

  // PI in 40 decimalen: Gebruik indien icdLengte wordt uitgebreid
  // +3.14159265_35897932_38462643_38327950_28841972
  pi.m_data[icdKommaPositie    ] = 3;
  pi.m_data[icdKommaPositie - 1] = 14159265L;
  pi.m_data[icdKommaPositie - 2] = 35897932L;
  pi.m_data[icdKommaPositie - 3] = 38462643L;
  pi.m_data[icdKommaPositie - 4] = 38327950L;
  pi.m_data[icdKommaPositie - 5] = 28841972L;

  return pi;
}

// Icd::LN2
// Omschrijving: Natuurlijke logarithme van twee
// Werking:      Mathematische constante die nooit veranderd
Icd
Icd::LN2()
{
  Icd ln2;

  ln2.m_lengte   = icdLengte * icdDigits;
  ln2.m_precisie = icdLengte * icdDigits / 2;
  ln2.m_teken    = Positief;

  // LN2 in 40 decimalen: Gebruik indien icdLengte wordt uitgebreid
  // +0.69314718_05599453_09417232_12145817_65680756
  ln2.m_data[icdKommaPositie    ] = 0;
  ln2.m_data[icdKommaPositie - 1] = 69314718L;
  ln2.m_data[icdKommaPositie - 2] =  5599453L;
  ln2.m_data[icdKommaPositie - 3] =  9417232L;
  ln2.m_data[icdKommaPositie - 4] = 12145817L;
  ln2.m_data[icdKommaPositie - 5] = 65680756L;

  return ln2;
}

// Icd::LN10
// Omschrijving: Natuurlijke logarithme van tien
// Werking:      Mathematische constante die nooit veranderd
Icd
Icd::LN10()
{
  Icd ln10;

  ln10.m_lengte   = icdLengte * icdDigits;
  ln10.m_precisie = icdLengte * icdDigits / 2;
  ln10.m_teken    = Positief;

  // LN10 in 40 decimalen: Gebruik indien icdLengte wordt uitgebreid
  // +2.30258509_29940456_84017991_45468436_42076019
  ln10.m_data[icdKommaPositie    ] = 2;
  ln10.m_data[icdKommaPositie - 1] = 30258509L;
  ln10.m_data[icdKommaPositie - 2] = 29940456L;
  ln10.m_data[icdKommaPositie - 3] = 84017991L;
  ln10.m_data[icdKommaPositie - 4] = 45468436L;
  ln10.m_data[icdKommaPositie - 5] = 42076019L;

  return ln10;
}

//////////////////////////////////////////////////////////////////////////
//
// EINDE ICD CONSTANTS
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// ICD OPERATOREN
//
//////////////////////////////////////////////////////////////////////////

// Icd::operator=
// Omschrijving:   assignment met Icd
// Parameters:     const Icd& Icd // over te nemen Icd
// Werking:        als niet Icd=Icd, dan alle datamembers gelijk zetten.
//
const Icd&
Icd::operator=(const Icd& Icd)
{
  if (this != &Icd)
  {
    m_teken = Icd.m_teken;
    for (long i = 0; i < icdLengte; i++)
    {
      m_data[i] = Icd.m_data[i];
    }
  }
  return *this;
}

// Icd::operator=
//
// Omschrijving:   assignment met string
// Parameters:     const String& str
// Werking:        gebruikt ZetWaardeString om de waarde van de string in de Icd te zetten.
//
const Icd&
Icd::operator=(const CString& str)
{
  ZetWaardeString(str);
  return *this;
}

// Icd::operator=
// Omschrijving:   assignment met long
// Parameters:     const long waarde
// Werking:        gebruikt ZetWaardeLong om de waarde van de long in de Icd te zetten.
//
const Icd&
Icd::operator=(const long waarde)
{
  ZetWaardeLong(waarde, 0);
  return *this;
}

// Icd::operator=
// Omschrijving:   assignment met double
// Parameters:     const double waarde
// Werking:        gebruikt ZetWaardeDouble om de waarde van de double in de Icd te zetten.
//
const Icd&
Icd::operator=(const double waarde)
{
  ZetWaardeDouble(waarde);
  return *this;
}

// Icd::operator+=
// Omschrijving:   telt het argument op bij de Icd en retourneert het resultaat
// Werking:        gebruikt de + operator en de = operator. (x+=y equivalent aan x=x+y)
//
const Icd&
Icd::operator+=(const Icd& Icd)
{
  // x+=y is gelijk aan x=x+y
  *this = *this + Icd;
  return *this;
}

// Icd::operator-=
// Omschrijving:   trekt het argument af van de Icd en retourneert het resultaat
// Werking:        gebruikt de - operator en de = operator. (x-=y equivalent aan x=x-y)
//
const Icd&
Icd::operator-=(const Icd& Icd)
{
  // x-=y is gelijk aan x=x-y
  *this = *this - Icd;
  return *this;
}

// Icd::operator*=
// Omschrijving:   vermenigvuldigt getal met het argument en retourneert het resultaat
// Werking:        gebruikt de Mul operatie functie
//
const Icd& 
Icd::operator*=(const Icd& p_icd)
{
  *this = Mul(p_icd);
  return *this;
}

// Icd::operator/=
// Omschrijving:   deelt getal door het argument en retourneert het resultaat
// Werking:        gebruikt de Div operatie functie
//
const Icd&
Icd::operator/=(const Icd& p_icd)
{
  *this = Div(p_icd);
  return *this;
}

// Icd::operator- (Unary)
// Omschrijving:   prefix - operator (negatie)
// Werking:        kopieert de waarde van de Icd, om deze vervolgens van
//                 teken te laten wisselen, mits het geheel niet nul is.
//
const Icd
Icd::operator-() const
{
  Icd getal(*this);

  // als nul, dan teken niet veranderen, anders omdraaien
  if (!getal.IsNul())
  {
    if (getal.m_teken == Positief)
    {
      getal.m_teken = Negatief;
    }
    else
    {
      getal.m_teken = Positief;
    }
  }
  return getal;
}

// Icd::operator++
// Omschrijving:   postfix ++ operator
// Werking:        gebruikt de prefix ++ en de copy constructor
//
const Icd
Icd::operator++(int)
{
  // bij postfix ++ operator eerst resultaat retourneren, dan pas optellen
  Icd res(*this);
  ++*this;
  return res;
}

// Icd::operator++
// Omschrijving:   prefix ++ operator
// Werking:        maakt eerst een Icd aan met waarde 1, waarna
//                 deze opgeteld wordt bij this.
//
Icd&
Icd::operator++()
{
  //++x is geljk aan x+=1
  Icd getal_1(1L, 0);
  *this += getal_1;
  return *this;
}

// Icd::operator--
// Omschrijving:   postfix -- operator
// Werking:        gebruikt de prefix -- en de copy constructor
//
const Icd
Icd::operator--(int)
{
  // bij postfix -- operator eerst resultaat retourneren, dan pas aftrekken
  Icd res(*this);
  --*this;
  return res;
}

// Icd::operator--
// Omschrijving:   prefix -- operator
// Werking:        maakt eerst een Icd aan met waarde 1, waarna
//                 deze afgetrokken wordt van this.
//
Icd&
Icd::operator--()
{
  // --x is geljk aan x-=1
  Icd getal_1(1L);
  *this -= getal_1;
  return *this;
}

// Icd::operator==
// Omschrijving:   vergelijkingsoperator is_gelijk
// Werking:        true als teken gelijk en alle m_data gelijk
//
const bool
Icd::operator==(const Icd& Icd) const
{
  // Alleen gelijk als de tekens gelijk zijn
  bool isGelijk = m_teken == Icd.m_teken;

  // Gelijk als alle cijfers gelijk
  for (long i = 0; isGelijk && i < icdLengte; i++)
  {
    isGelijk = m_data[i] == Icd.m_data[i];
  }
  return isGelijk;
}


// Icd::operator!=
// Omschrijving:   vergelijkingsoperator is_ongelijk
// Werking:        gebruikt == operator (x!=y is equivalent aan !(x==y))
//
const bool
Icd::operator!=(const Icd& Icd) const
{
  // x!=y is gelijk aan !(x==y)
  return !(*this == Icd);
}

// Icd::operator<
// Omschrijving:   vergelijkingsoperator is_kleiner
// Werking:        als het linker argument negatief en rechter positief, dan al kleiner anders:
//                 test vanaf de meest belangrijke m_data of de m_data aan de
//                 linkerkant van de operator groter is, zo ja dan stoppen, anders doorgaan
//                 als blijkt dat de tekens beide negatief zijn, dan resultaat omdraaien
//                 tenzij de beide argumenten gelijk zijn.
//
const bool
Icd::operator<(const Icd& Icd) const
{
  // Alleen gelijk als tekens gelijk zijn
  bool isGelijk = (m_teken == Icd.m_teken);

  // Kleiner als teken links negatief en teken rechts positief
  bool isKleiner = (m_teken == Negatief && Icd.m_teken == Positief);

  // Kleiner zolang cijfer links kleiner dan cijfer rechts
  for (long i = icdLengte - 1; i >= 0 && isGelijk; i--)
  {
    if (m_data[i] != Icd.m_data[i])
    {
      isGelijk  = false;
      isKleiner = m_data[i] < Icd.m_data[i];
    }
  }
  // Als beide tekens negatief, dan resultaat omdraaien, tenzij de
  // argumenten gelijk zijn.
  // Voorbeeld: -5<-3 is gelijk aan 5>3
  //            -5<-5 is gelijk aan 5<5
  if (m_teken == Negatief && Icd.m_teken == Negatief && !isGelijk)
  {
    isKleiner =! isKleiner;
  }
  return isKleiner;
}

// Icd::operator>
// Omschrijving:   vergelijkingsoperator is_groter
// Werking:        als het linker argument positief en rechter negatief, dan al groter anders:
//                 test vanaf de meest belangrijke m_data of de m_data aan de
//                 linkerkant van de operator kleiner is, zo ja dan stoppen, anders doorgaan
//                 als blijkt dat de tekens beide negatief zijn, dan resultaat omdraaien
//                 tenzij de beide argumenten gelijk zijn.
//
const bool
Icd::operator>(const Icd& Icd) const
{
  // Alleen gelijk als tekens gelijk zijn
  bool isGelijk = (m_teken == Icd.m_teken);

  // Groter als teken links positief en teken rechts negatief
  bool isGroter = (m_teken == Positief && Icd.m_teken == Negatief);

  // Groter zolang cijfer links groter dan cijfer rechts
  for (long i = icdLengte - 1; i >= 0 && isGelijk; i--)
  {
    if (m_data[i] != Icd.m_data[i])
    {
      isGelijk = false;
      isGroter = m_data[i] > Icd.m_data[i];
    }
  }

  // Als beide tekens negatief, dan resultaat omdraaien, tenzij de
  // argumenten gelijk zijn.
  // Voorbeeld: -5>-3 is gelijk aan 5<3
  //            -5>-5 is gelijk aan 5>5
  if (m_teken == Negatief && Icd.m_teken == Negatief && !isGelijk)
  {
    isGroter=!isGroter;
  }
  return isGroter;
}

// Icd::operator<=
// Omschrijving:   vergelijkingsoperator is_kleiner_of_gelijk
// Werking:        gebruikt > operator (x<=y is equivalent aan !(x>y))
//
const bool
Icd::operator<=(const Icd& Icd) const
{
  // x<=y is gelijk aan !(x>y)
  return !(*this>Icd);
}

// Icd::operator>=
// Omschrijving:   vergelijkingsoperator is_groter_of_gelijk
// Werking:        gebruikt < operator (x>=y is equivalent aan !(x<y))
//
const bool
Icd::operator>=(const Icd& Icd) const
{
  // x>=y is gelijk aan !(x<y)
  return !(*this<Icd);
}

// operator<<
// Omschrijving:   standaard output operator
// Werking:        gebruikt Icd.WaardeAlsString om waarde om te zetten naar String
//                 waarna de << operator op een string wordt uitgevoerd.
//
ostream& operator<<(ostream& os, const Icd& Icd)
{
  // os<<Icd is gelijk aan os<<Icd.WaardeAlsString()
  os << Icd.AlsString().operator const char*();
  return os;
}

//////////////////////////////////////////////////////////////////////////
//
// EINDE ICD OPERATOREN
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// MATHEMATISCHE ICD FUNCTIES
//
//////////////////////////////////////////////////////////////////////////

// Icd::Floor
// Omschrijving: Eerste gehele getal kleiner dan de input
// Werking:      Laat decimaal deel vervallen en controleer op negatief getal
//               Bij negatief getal er 1 aftrekken
Icd
Icd::Floor() const
{  
  Icd res;
  res.m_teken = m_teken;
  for (int n = icdKommaPositie; n < icdLengte; n++)
  {
    res.m_data[n] = m_data[n];
  }
  if (m_teken == Negatief)
  {  
    for (int n = 0; n < icdKommaPositie; n++)
    {
      if(m_data[n])
      {
        // ICD was niet nul achter de komma, en dit is een negatief getal, dus de
        // gefloorde versie is de afgekapte versie MIN EEN.
        --res;
        break;
      }
    }
  }
  return res;
}

// Icd::Ceiling
// Omschrijving: Eerste gehele getal groter dan de input
// Werking:      Laat decimaal deel vervallen en controleer op negatief getal
//               Bij negatief getal er 1 bij optellen
Icd
Icd::Ceil() const
{
  Icd res;
  res.m_teken = m_teken;
  for (int n = icdKommaPositie; n < icdLengte; n++)
  {
    res.m_data[n] = m_data[n];
  }
  if (m_teken == Positief)
  {  
    for (int n = 0; n < icdKommaPositie; n++)
    {
      if (m_data[n])
      {
        // ICD was niet nul achter de komma, en dit is een positief getal, dus de
        // geceilde versie is de afgekapte versie PLUS EEN.
        ++res;
        break;
      }
    }
  }
  return res;
}

// Icd::sqrt
// Omschrijving: Vierkantswortel van het getal
// Werking:      Maak eerste benadering aan de hand van een double
//               Doe daarna de Newton's benadering van de wortel
Icd
Icd::VierkantsWortel() const
{
  Icd getal(0L,0L);
  Icd half("0,5");
  Icd twee(2L);
  Icd drie(3L);

  // Optimalisatie: wortel(0) = 0
  if(IsNul())
  {
    return getal;
  }
  // Bereken afbreekcriterium epsilon
  Icd epsilon = Epsilon(10);

  getal = *this; // Getal om wortel uit te trekken
  if(getal.GeefTeken() == -1)
  {
    throw CString("Kan geen wortel uit een negatief getal trekken");
  }
  // Reductie door te delen door een kwadraat van een geheel getal
  // voor de snelheid hier een macht van twee
  Icd reductie(1L);
  Icd honderd(100L);
  while(getal / (reductie * reductie) > honderd)
  {
    reductie *= twee;
  }
  // Reduceren door te delen door het kwadraat van de reductie
  // variabele reductie is feitelijk sqrt(reductie)
  getal /= (reductie * reductie);

  // Eerste benadering
  double benadering1 = getal.AlsDouble();
  double benadering2 = 1 / sqrt(benadering1);
  Icd result(benadering2);
  Icd tussen;

  // Newton's iteratie
  // Un = U(3-VU^2)/2
  while(true)
  {
    tussen  = getal * result * result;  // VU^2
    tussen  = drie - tussen;            // 3-VU^2
    tussen *= half;                     // (3-VU^2)/2

    if(tussen.WaardeAchterKomma() < epsilon)
    {
      break;
    }
    result *= tussen;
  }
  // Eind resultaat berekenen uit getal * 1/wortel
  result *= getal;
  // Reductie toevoegen door enkel met reductie te vermenigvuldigen
  result *= reductie;

  return result;
}

// Icd::Macht
// Omschrijving: Verhef ICD getal tot een macht
// Werking:      x^y = exp(y * ln(x))
Icd
Icd::Macht(const Icd& icd) const
{
  Icd result;

  result = this->Log() * icd;
  result = result.Exp();

  return result;
}

// Icd::Abs
// Omschrijving: Retourneer de absolute waarde
// Werking:      Zet het teken op Teken::Positief
Icd
Icd::AbsoluteWaarde() const
{
  Icd icd2 = *this;
  icd2.m_teken = Positief; 
  return icd2;
}

// Icd::Reciproke
// Omschrijving: Inverse van het getal = 1 / getal
// Werking:      Gebruikt standaard Div
//
Icd
Icd::Reciproke() const
{
  Icd getal(1L);
  getal = getal.Div(*this);
  return getal;
}

// Icd::Log
// Omschrijving: Natuurlijke logarithme
// Werking:      Use a taylor series until their is no more change in the result
//               Equivalent with the same standard C function call
//               ln(x) == 2( z + z^3/3 + z^5/5 ...
//               z = (x-1)/(x+1)
//
Icd 
Icd::Log() const
{
  long k;
  long expo = 0;
  Icd res, getal, z2;
  Icd getal10(10L);
  Icd fast("1,2");
  Icd one(1L);
  Icd epsilon = Epsilon(5);

  if(*this <= Icd(0L)) 
  { 
    throw CString("Kan geen natuurlijke logarithme trekken uit een getal <= 0");
  }
  // Breng getal onder de 10 en bewaar de exponent
  getal = *this;
  while(getal > getal10)
  {
    getal /= getal10;
    ++expo;
  }
  // In order to get a fast Taylor series result we need to get the fraction closer to one
  // The fraction part is [1.xxx-9.999] (base 10) OR [1.xxx-255.xxx] (base 256) at this point
  // Repeat a series of square root until 'getal' < 1.2
  for(k = 0; getal > fast; k++)
  {
    getal = sqrt(getal);
  }
  // Calculate the fraction part now at [1.xxx-1.1999]
  getal = (getal - one) / (getal + one);
  z2    = getal * getal;
  res   = getal;
  // Iterate using taylor series ln(x) == 2( z + z^3/3 + z^5/5 ... )
  Icd tussen;
  for(long stap = 3; ;stap += 2)
  {
    getal *= z2;
    tussen = getal / Icd(stap);
    // Afbreek criterium
    if(tussen < epsilon)
    {
      break;
    }
    res += tussen;
  }
  // Machten van twee weer toevoegen (afkomstig van " < 1.2")
  res *= Icd(pow(2.0,(double)(k + 1)));

  // Exponent weer toevoegen
  if(expo != 0)
  {
    // Ln(x^y) = Ln(x) + Ln(10^y) = Ln(x) + y * ln(10)
    res += Icd(expo) * Icd::LN10();
  }
  return res;
}

// Icd::Exp
// Omschrijving: Exponent e tot de macht
// Werking:      Use a taylor series until their is no more change in the result
//               exp(x) == 1 + x + x^2/2!+x^3/3!+....
//               Equivalent with the same standard C function call
//
Icd 
Icd::Exp() const
{
  long stap, k = 0;
  long expo;
  Icd tussen, result, getal;
  Icd half("0,5");
  Icd ten(10L);
  Icd epsilon = Epsilon(5);

  getal = *this;

  if( getal.GeefTeken() < 0 )
  {
    getal = -getal;;
  }
  for( k = 0; getal > half; )
  {
    expo = getal.Exponent();
    if( expo > 0 )
    {
      stap = 3 * min( 10, expo );  // 2^3
      result = Icd((long) (1 << stap) );
      result = result.Reciproke();
      k += stap;
    }
    else
    {
      result = half;
      k++;
    }

    getal *= result;
  }

  // Do first two iterations
  result  = Icd(1L) + getal;
  tussen  = getal * getal * half;
  result += tussen;
  // Now iterate 
  for(stap = 3; ;stap++)
  {
    tussen *= getal / Icd(stap);
    // Afbreek criterium
    if(tussen < epsilon)
    {
      break;
    }
    result += tussen;
  }
  // Machten van zichzelf toevoegen
  for( ; k > 0; k-- )
  {
    result *= result;
  }
  // Teken goed zetten
  if(this->GeefTeken() < 0 )
  {
    result = -result;
  }
  return result;
}

// Icd::Log10
// Omschrijving: Logarithme van basis 10
// Werking:      log10 = ln(x) / ln(10);
Icd     
Icd::Log10() const
{
  Icd res(0L);

  if(GeefTeken() <= 0) 
  { 
    throw CString("Kan geen logarithme van een getal <= 0 bepalen");
  }
  res = *this;
  res = res.Log() / LN10();

  return res;
}

//////////////////////////////////////////////////////////////////////////
//
// EINDE ICD MATHEMATISCHE FUNCTIES
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// ICD TRIGONOMETRISCHE FUNCTIES
//
//////////////////////////////////////////////////////////////////////////

// Icd::Sin
// Omschrijving: Sinus van de hoek
// Werking:      Gebruik de Taylor serie: Sin(x) = x - x^3/3! + x^5/5! ...
//               1) Reduceer x eerst tussen 0..2*PI 
//               2) Reduceer verder tot x tussen 0..PI door middel van sin(x+PI) = -Sin(x)
//               3) De de Taylor expansie serie
//   De reductie is nodig om de Taylor expansie sneller te laten lopen 
//   en de afrondingsfouten te beperken
//
Icd
Icd::Sinus() const
{
  int sign;
  Icd getal;
  Icd pi,pi2;
  Icd tussen;
  Icd epsilon = Epsilon(3);

  getal = *this;

  sign = getal.GeefTeken();
  if( sign < 0 )
  {
    getal = -getal;
  }
  // Reduceer het arugment tot het tussen 0..2PI ligt
  pi2 = PI() * Icd(2L);
  if(getal > pi2)
  {
    tussen = getal / pi2; 
    tussen = tussen.WaardeVoorKomma();
    getal -= tussen * pi2;
  }
  if(getal < Icd(0L))
  {
    getal += pi2;
  }
  // Reduceer verder tot het tussen 0..PI ligt
  pi = PI();
  if(getal > pi)
  { 
    getal -= pi; 
    sign *= -1; 
  }

  // Nu ittereren we met Taylor expansie
  // Sin(x) = x - x^3/3! + x^5/5! ...
  Icd kwadraat  = getal * getal;
  Icd resultaat = getal;
  tussen = getal;

  for(long stap = 3; ;stap += 2)
  {
    tussen    *= kwadraat;
    tussen    /= Icd(stap) * Icd (stap - 1);
    tussen     = -tussen; // Om de stap van teken wisselen
    resultaat += tussen;

    // DEBUGGING
    // printf("%02d = %40s = %40s\n",stap,tussen.AlsString(),resultaat.AlsString());

    // Controleer afbreekcriterium epsilon
    if(tussen.AbsoluteWaarde() < epsilon)
    {
      break;
    }
  }
  // Goede teken nog toepassen
  if( sign < 0 )
  {
    resultaat = -resultaat;
  }
  return resultaat;
}

// Icd::Cos
// Omschrijving: Cosinus van de hoek
// Werking:      Use the taylor series. Cos(x) = 1 - x^2/2! + x^4/4! - x^6/6! ...
//               1) However first reduce x to between 0..2*PI
//               2) Then reduced it further to between 0..PI using cos(x)=Cos(2PI-x) for x >= PI
//               3) Now use the trisection identity cos(3x)=-3*cos(x)+4*cos(x)^3
//                  until argument is less than 0.5
//               4) Finally use Taylor 
//
Icd
Icd::Cosinus() const
{
  long trisectie, stap;
  Icd tussen, result, getal, getal2;
  Icd c05("0,5"), c1(1L), c2(2L), c3(3L), c4(4L);
  Icd epsilon = Epsilon(2);

  getal = *this;

  // Reduce argument to between 0..2PI
  result  = PI();
  result *= c2;
  if(getal.AbsoluteWaarde() > result )
  {
    tussen = getal / result; 
    tussen = tussen.WaardeVoorKomma();
    getal -= tussen * result;
  }
  if(getal.GeefTeken() < 0)
  {
    getal += result;
  }
  // Reduced it further to between 0..PI. u==2PI
  tussen = PI();
  if( getal > tussen )
  {
    getal = result - getal;
  }
  // Now use the trisection identity cos(3x)=-3*cos(x)+4*cos(x)^3
  // until argument is less than 0.5
  for( trisectie = 0, tussen = c1; getal / tussen > c05; ++trisectie)
  {
    tussen *= c3;
  }
  getal /= tussen;

  // Eerste stap van de itteratie
  getal2 = getal * getal;
  tussen = c1;
  result = tussen;
  // Itereer met Taylor expansie
  for(stap=2; ;stap += 2)
  {
    getal   = getal2; 
    getal  /= Icd(stap);
    getal  /= Icd(stap-1);
    tussen *= getal;
    tussen  = -tussen;  // r.change_sign();
    // Afbreek criterium
    if(tussen.AbsoluteWaarde() < epsilon)
    {
      break;
    }
    result += tussen;
  }

  // Effect van trisectie weer toevoegen
  for( ;trisectie > 0; --trisectie)
  {
    result *= ( c4 * result * result - c3 );
  }
  return result;
}

// Icd::Tan
// Omschrijving: Tangens van de hoek
// Werking:      Use the identity tan(x)=Sin(x)/Sqrt(1-Sin(x)^2)
//               However first reduce x to between 0..2*PI
//
Icd
Icd::Tangent() const
{
  Icd result, tussen, getal;
  Icd twee(2L), drie(3L);;

  getal = *this;

  // Reduce argument to between 0..2PI
  Icd pi     = PI();
  Icd tweepi = twee * pi;
  if(getal.AbsoluteWaarde() > tweepi )
  {
    tussen  = getal / tweepi; 
    tussen  = tussen.WaardeVoorKomma();
    getal  -= tussen * tweepi;
  }

  if(getal.GeefTeken() < 0)
  {
    getal += tweepi;
  }
  Icd halfpi    = pi / twee;
  Icd anderhalf = drie * halfpi;
  if( getal == halfpi || getal == anderhalf)
  { 
    throw CString("Kan geen tangens bereken uit een hoek van 1/2 pi of 3/2 pi");
  }
  // Sin(x)/Sqrt(1-Sin(x)^2)
  result       = getal.Sinus(); 
  Icd kwadraat = result * result;
  Icd deel     = Icd(1L) - kwadraat;
  Icd wortel   = sqrt(deel);
  result      /= wortel;

  // Corrigeren voor teken
  if(getal > halfpi && getal < anderhalf)
  {
    result = -result;
  }
  return result;
}

// Icd::Asin
// Omschrivjing: Arcsinus (hoek) van de verhouding
// Werking:      Use Newton by solving the equation Sin(y)=x. Then y is Arcsin(x)
//               Iterate by Newton y'=y-(sin(y)-x)/cos(y). 
//               With initial guess using standard double precision arithmetic.
//
Icd     
Icd::ArcSinus() const
{
  long stap, reductie, sign;
  double d;
  Icd tussen, getal, result, factor;
  Icd c1(1L), c05("0,5"), c2(2L);
  Icd epsilon = Epsilon(5);

  getal = *this;
  if(getal > c1 || getal < -c1)
  {
    throw CString("Kan geen arcsinus berekenen uit een getal > 1 of < -1");
  }
  // bewaar het teken
  sign = getal.GeefTeken();
  if(sign < 0)
  {
    getal = -getal;
  }
  // Reduce the argument to below 0.5 to make the newton run faster
  for(reductie = 0; getal > c05; ++reductie)
  {
    getal /= sqrt(c2) * sqrt( c1 + sqrt( c1 - getal * getal ));
  }
  // Snelle benadering via een double
  d = asin(getal.AlsDouble());
  result = Icd( d );
  factor = Icd( 1.0 / cos(d) ); // Constant factor 

  // Newton Iteration
  for( stap=0;; stap++)
  {
    tussen = ( result.Sinus() - getal ) * factor;
    // if( y - r == y )
    if(tussen.AbsoluteWaarde() < epsilon)
    {
      break;
    }
    result -= tussen;
  }

  // Reductie terugverwerken in resultaat
  result *= Icd((long) (1 << reductie) );

  if( sign < 0 )
  {
    result = -result;
  }
  return result;

}

// Icd::Acos
// Omschrijving: Arccosinus (hoek) van de herhouding
// Werking:      Use Arccos(x) = PI/2 - Arcsin(x)
Icd     
Icd::ArcCosinus() const
{
  Icd y;

  y  = PI();
  y /= Icd(2L);
  y -= ArcSinus();

  return y;
}

// Icd::Atan
// Omschrijving: Arctangens (hoek) van de verhouding
// Werking:      Use the taylor series. ArcTan(x) = x - x^3/3 + x^5/5 ...
//               However first reduce x to abs(x)< 0.5 to improve taylor series
//               using the identity. ArcTan(x)=2*ArcTan(x/(1+sqrt(1+x^2)))
//
Icd
Icd::ArcTangens() const
{
  Icd  result, kwadraat;
  Icd  tussen1,tussen2;
  Icd  half("0,5");
  Icd  een(1L);
  Icd  epsilon = Epsilon(5);
  long k = 2;

  result   = *this;
  // Transform the solution to ArcTan(x)=2*ArcTan(x/(1+sqrt(1+x^2)))
  result = result / ( een + sqrt( een + (result * result)));
  if( result.AbsoluteWaarde() > half) // if still to big then do it again
  {
    k = 4;
    result = result / (een + sqrt(een + (result * result)));
  }
  kwadraat = result * result;
  tussen1  = result;
  // Now iterate using Taylor expansion
  for(long stap = 3;; stap += 2)
  {
    tussen1 *= kwadraat;
    tussen1  = -tussen1;
    tussen2  = tussen1 / Icd(stap);
    // Afbreek criterium
    if(tussen2.AbsoluteWaarde() < epsilon)
    {
      break;
    }
    result += tussen2;
  }
  result *= Icd(k);
  return result;
}

// Icd::Atan2
// Omschrijving: Hoek van twee punten.
// Werking:      return the angle (in radians) from the X axis to a point (y,x).
//               use atan() to calculate atan2()
//
Icd
Icd::ArcTangens2Punten(Icd p_x) const
{
  Icd result;
  Icd getal = *this;
  Icd nul(0L), c05("0,5");

  if( p_x == nul && getal == nul)
  {
    return nul;
  }
  if( p_x == nul )
  {
    result = PI();
    if( getal < nul )
    {
      result *= -c05;
    }
    else
    {
      result *= c05;
    }
  }
  else
  {
    if( getal == nul )
    {
      if( p_x < nul )
      {
        result = PI();
      }
      else
      {
        result = nul;
      }
    }
    else
    {
      result = Icd( getal / p_x ).ArcTangens();
      if( p_x < nul  && getal < nul )
      {
        result -= PI();
      }
      if( p_x < nul && getal >= nul )
      {
        result += PI();
      }
    }
  }
  return result;
}

//////////////////////////////////////////////////////////////////////////
//
// EINDE ICD TRIGONOMETRISCHE FUNCTIES
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 
// ICD OPVRAGEN ALS IETS ANDERS
//
//////////////////////////////////////////////////////////////////////////

// Icd::WaardeVoorKomma
// Omschrjiving: Deel voor de komma opvragen (floor indien groter dan 0)
// Werking:      Deel na de komma wordt op 0'en gezet
Icd
Icd::WaardeVoorKomma() const
{
  Icd voor = *this;
  for(int i = 0; i < icdKommaPositie; ++i)
  {
    voor.m_data[i] = 0;
  }
  return voor;
}

// Icd:: WaardeAchterKomma
// Omschrijving: Deel achter de komman opvragen
// Werking.      Deel voor de komma wordt op 0'en gezet
Icd
Icd::WaardeAchterKomma() const
{
  Icd achter = *this;
  for(int i = icdKommaPositie; i < icdLengte; ++i)
  {
    achter.m_data[i] = 0;
  }
  return achter;
}

// Icd::AlsDouble
// Omschrijving:   geeft de waarde van de Icd als een double. Pas op: wegens afrondfouten
//                 van de double kan het voorkomen dat de waarde van de double niet precies
//                 gelijk is aan de waarde van de Icd.
// Werking:        berekent eerst het deel voor de komma mbv de pow functie, en daarna
//                 het deel achter de komma.
//
double
Icd::AlsDouble() const
{
  double res = 0.0;

  // Deel voor de komma bepalen
  for (int i = icdKommaPositie; i < icdLengte; i++)
  {
    res += pow(double(icdBasis), i - icdKommaPositie) * m_data[i];
  }
  // Deel na de komma bepalen
  for (int i = 0; i < icdKommaPositie; i++)
  {
    res += (double)m_data[i] / pow(double(icdBasis), icdKommaPositie - i);
  }
  // Teruggeven plus het teken.
  return (m_teken == Positief ? 1 : -1) * res; 
}

// Icd::AlsLong
// Omschrijving: Geeft de waarde van Icd als een long
//               Kan throwen als de ICD te groot is voor een long
// Werking:      Converteert eerste drie delen voor de komma
//
long
Icd::AlsLong() const
{
  if(!PastInLong()) 
  {
    throw CString("Getal: Overflow in ICD");
  }
  // Bereken long
  long result = m_data[icdKommaPositie] + 
                m_data[icdKommaPositie  + 1] * icdBasis;
  if(icdBasis <= 10000)
  {
    result += m_data[icdKommaPositie + 2] * icdBasis * icdBasis;
  }
  if (m_teken == Negatief)
  {
    result = -result;
  }
  return result;
}

// Icd::AlsInt64
// Omschrijving: Geeft de waarde van de Icd als een int64
// Werking:      Converteert het deel voor de komma
//
int64
Icd::AlsInt64() const
{
  int64 result = 0;

  if(!PastInInt64())
  {
    throw CString("Getal: Overflow in ICD");
  }
  for(int i = icdLengte -1; i >= icdKommaPositie; --i)
  {
    result *= icdBasis;
    result += m_data[i];
  }
  if(m_teken == Negatief)
  {
    return -result;
  }
  return result;
}

// Icd::AlsString
// Omschrijving:   Geeft de waarde van de Icd terug als string
// Werking:        Bepaald eerste het teken, waarna voor elke m_data een
//                 conversie naar string volgt. Eventueel worden er nog extra
//                 nullen voor geplaatst (achter de komma altijd): 3 wordt 0003
//
CString
Icd::AlsString() const
{
  // Teken aan het begin, indien negatief
  CString str;
  if (m_teken == Icd::Negatief)
  {
    str += "-";
  }
  else
  {
    str += "+";
  }
  // Als het eerste niet nul cijfer nog niet is gevonden, dan niets doen
  // anders het getal neerzetten, eventueel vooraf laten gaan door nullen
  // Voorbeeld: 0:2:342:2:0 wordt +20342,00020000
  bool nogNul = true;

  for (int i = icdLengte - 1; i >= icdKommaPositie; i--)
  {
    if (!nogNul)
    {
      // Extra nullen plaatsen
      for (long j = 0; j < (icdDigits - 1) - long_log10(m_data[i]); j++)
      {
        str += "0";
      }
    }
    nogNul = (nogNul && (m_data[i] == 0));
    if (!nogNul || i == icdKommaPositie)
    {
      str += LongNaarString(m_data[i]);
    }
  }

  CString strNaKomma;
  for (int k = icdKommaPositie - 1; k >= 0; k--)
  {
    for (long j = 0; j < (icdDigits - 1) - long_log10(m_data[k]); j++)
    {
      strNaKomma += "0";
    }
    strNaKomma += LongNaarString(m_data[k]);
  }
  if (!str)
  {
    str += "0";
  }
  // afronden
  if (m_precisie < 16 && strNaKomma[m_precisie] >= '5') 
  {
    int pos = m_precisie-1;

    while (pos >= 0 && strNaKomma[pos] == '9') 
    {
      strNaKomma.SetAt(pos, '0');
      pos--;
    }
    if (pos >= 0)
    {
      strNaKomma.SetAt(pos, (char)(strNaKomma[pos] + 1)); // rond af
    }
    else
    {
      pos = str.GetLength()-1; // afronding over komma heen
      while (pos >= 0 && str[pos] == '9')
      {
        str.SetAt(pos, '0');
        pos--;
      }
      if (pos >= 0)
      {
        str.SetAt(pos, (char)(str[pos] + 1)); // rond af
      }
      else
      {
        str = "1" + str;
      }
    }
  }
  if (m_precisie > 0)
  {
    str += ","; // sessie->GeefLocale().DecimaalChar();
    str += strNaKomma.Mid(0, m_precisie);
  }
  str.TrimRight('0');
  return str;
}

// Icd::AlsDisplayString
// Omschrijving:   Geeft de waarde van de Icd terug als geformatteerde string
// Werking:        Bepaald eerste het teken, waarna voor elke m_data een
//                 conversie naar string volgt. Eventueel worden er nog extra
//                 nullen voor geplaatst (achter de komma altijd): 3 wordt 0003
//
CString 
Icd::AlsDisplayString() const
{
  CString display = AlsString();
  CString decpart;
  CString intpart;
  CString sgnpart;
  static CString sgn("-+");
  int decpos;
  if((decpos = display.Find(",")) == -1) 
  {
    intpart = display;	
  }
  else
  {
    intpart = display.Left(decpos);
    decpart = display.Mid(decpos + 1);
  }
  while(!intpart.IsEmpty() && sgn.Find(intpart.Left(1)) != -1)
  {
    sgnpart += intpart.Left(1);
    intpart  = intpart.Mid(1);
  }
  display = "";
  while(intpart.GetLength() > 3)
  {
    display = "." + intpart.Right(3) + display;
    intpart = intpart.Left(intpart.GetLength() - 3);
  }
  intpart += display;

  display = sgnpart + intpart;
  if(decpart.GetLength() != 0)
  {
    display += "," + decpart; 
  }
  return display;
}

//////////////////////////////////////////////////////////////////////////
//
// EINDE ICD OPVRAGEN ALS IETS ANDERS
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// ICD WIJZIGEN PRECISIE
//
//////////////////////////////////////////////////////////////////////////

// Icd::ZetLengteEnPrecisie
// Omschrijving: Nieuwe lengte en schaal zetten
// Werking:      Controle van de argumenten
//               Aanpassen members en afronden data
//               
void
Icd::ZetLengteEnPrecisie(int lengte, int precisie)
{
  if (lengte == m_lengte && precisie == m_precisie)
    return;

  if (lengte < 1 || lengte > icdLengte * icdDigits)
  {
    throw CString("Gebroken-getal (ICD): De lengte moet liggen tussen 1 en ") + LongNaarString(icdLengte * icdDigits);
  }
  if (precisie < 0 || precisie > icdKommaPositie * icdDigits)
  {
    throw CString("Gebroken-getal (ICD): De precisie moet liggen tussen 0 en ") + LongNaarString(icdKommaPositie * icdDigits);
  }
  if (lengte < precisie)
  {
    throw CString("Gebroken-getal (ICD): De precisie moet kleiner of gelijk aan de lengte zijn");
  }
  if ((lengte - precisie) > (icdKommaPositie * icdDigits))
  {
    throw CString("Gebroken-getal (ICD): Aantal cijfers voor de komma mag niet groter zijn dan ") + LongNaarString(icdKommaPositie * icdDigits);
  }
  m_precisie = precisie;

  int hoogsteindex = (icdDigits * icdKommaPositie + (lengte - m_precisie) - 1) / icdDigits;

  for (long i = icdLengte - 1; i > hoogsteindex ; i--)
  {
    if (m_data[i] != 0)
    {
      throw CString("Gebroken getal te groot (ICD Overflow)");
    }
  }

  unsigned long maxIndexWaarde = (unsigned long)long_pow(10, (lengte - m_precisie) % icdDigits);
  if (maxIndexWaarde == 1)
  {
    maxIndexWaarde = icdBasis;
  }
  if (m_data[hoogsteindex] >= maxIndexWaarde)
  {
    throw CString(" Gebroken getal te groot (ICD Overflow)");
  }

  m_lengte = lengte;

  // Testen of de inhoud niet de maximale waarde betreft die door afronding te lang 
  // wordt (groter dan de lengte)
  if ((m_precisie != (icdKommaPositie * icdDigits)) && IsInhoudGrensGeval()) 
  {
    throw CString("Gebroken getal te groot (ICD Overflow)");
  }

  // Clearen data achter de komma; deze hoort niet bij onze precisie en moet
  // for all intents and purposes 0 zijn.
  int nietSignificantieDigits = (icdKommaPositie * icdDigits - m_precisie);
  for (int n = 0; n < nietSignificantieDigits / icdDigits; n++)
  {
    m_data[n] = 0;
  }
  // 1 van de elementen is gedeeltelijk wel, gedeeltelijk niet significant. We
  // clearen het niet-significante deel.
  static int significanties[] = {1,10,100,1000,10000,100000,1000000,10000000};
  // Aantal digits aan de rechterkant dat niet significant is
  int significantie = significanties[nietSignificantieDigits % icdDigits];

  ///////////////////////////////////////////////////////////////////////////////////
  ////  Debug

  //m_data[ nietSignificantieDigits / 4] += 100;
  //long rndDecimaal = RoundDecimal(m_data[nietSignificantieDigits / 4], precisie);
  //m_data[ nietSignificantieDigits / 4] = rndDecimaal;

  ////  End Debug
  ///////////////////////////////////////////////////////////////////////////////////

  m_data[ nietSignificantieDigits / icdDigits] /= significantie;
  m_data[ nietSignificantieDigits / icdDigits] *= significantie;

  // [EH] ITIL 130483 RONDAF(-0.01) is fout
  // -0.0 check voor het geval we afronden vanaf negatief > -1
  int ind = 0;
  while((m_data[ind] == 0) && (ind < icdLengte)) ++ind;
  if((m_teken == Negatief) && (ind == icdLengte))
  {
    m_teken = Positief;
  }
}

// Icd::RoundDecimal
// Omschrijving: Afronden op aantal decimalen
// Werking:
//
long 
Icd::RoundDecimal(long decimal, int precsion)
{
  CString strDecimal = "";
  CString strNum     = "";

  strDecimal = LongNaarString(decimal);

  if (!strDecimal.IsEmpty())
  {
    int intLenght = strDecimal.GetLength() - 1;
    for (int idx = 0; idx <= intLenght; idx++)
    {
      if (idx == precsion-1)
      {
        if (idx+1 <= intLenght)
        {
          CString strDigit  = "";
          CString strTarget = "";

          strDigit += strDecimal.GetAt(idx+1);
          int digit = StringNaarLong(strDigit);

          strTarget += strDecimal.GetAt(idx);
          int Target = StringNaarLong(strTarget);

          if (digit >= 5)
          { 
            Target++;
            strTarget = "";
            strNum += LongNaarString(Target);
            return StringNaarLong(strNum);
          }
          else
          {
            strNum += strDecimal.GetAt(idx);
          }
        }
        else
        {
          return  decimal;
        }
      }
      else
      {
        strNum += strDecimal.GetAt(idx);
      }
    }
  }
  return StringNaarLong(strNum);
}

//////////////////////////////////////////////////////////////////////////
//
// EINDE ICD PRECISIE FUNCTIES
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// ICD GETTERS
//
//////////////////////////////////////////////////////////////////////////

// Icd::IsNul
// Omschrijving:   geeft true indien de Icd gelijk is aan 0, anders false
// Werking:        Loopt de m_data af, totdat deze niet meer nul is. Indien de
//                 laatste ook 0 is, dan true opleveren, anders false.
//
bool
Icd::IsNul() const
{
  // IsNul geeft waar als alle datamembers gelijk zijn aan nul
  bool isNul = true;
  long i = 0;
  while (isNul && i < icdLengte)
  {
    if (m_data[i] != 0)
    {
      isNul = false;
      break;
    }
    i++;
  }
  return isNul;
}

// Retourneert 0  -> Inhoud is 0.0
// Retourneert 1  -> Inhoud > 0.0
// Retourneert -1 -> Inhoud < 0.0
int 
Icd::GeefTeken() const
{
  if (IsNul()) 
  {
    return 0;
  }
  return m_teken == Positief ? 1 : -1;
}

// Icd::PastInLong
// Omschrijving: Retourneert true als getal in een long past
// Werking:      Controleert de inhoud van de eerste twee decimalen
//
bool 
Icd::PastInLong() const
{
  int64 result = m_data[icdKommaPositie    ] + 
                 m_data[icdKommaPositie + 1] * icdBasis;

  // Implementatie is nu simpel geworden door grote icdBasis (10E+8)
  if(m_teken == Positief)
  {
    if(result > LONG_MAX)
    {
      return false;
    }
  }
  else
  {
    if(-result < LONG_MIN)
    {
      return false;
    }
  }
  // Kijken of het stiekum niet een nog groter getal was
  for(int i = icdKommaPositie + 2; i < icdLengte; ++i)
  {
    if(m_data[i])
    {
      return false;
    }
  }
  return true;
}

// Icd::PastInInt64
// Omschrijving: Retourneert true als getal in een int64 past
// Werking:      Controleert de inhoud van de eerste drie decimalen
bool
Icd::PastInInt64() const
{
  int64 result = m_data[icdKommaPositie    ] +
                 m_data[icdKommaPositie + 1] * icdBasis;
  if(m_teken == Positief)
  {
    int64 restruimte = LLONG_MAX - ((int64)icdBasis * (int64)icdBasis);
    if(m_data[icdKommaPositie + 2] > restruimte)
    {
      return false;
    }
  }
  else
  {
    int64 restruimte = -(LLONG_MIN + ((int64)icdBasis * (int64)icdBasis));
    if(m_data[icdKommaPositie + 2] > restruimte)
    {
      return false;
    }
  }
  // Kijken of het stiekum niet een nog groter getal was
  for(int i = icdKommaPositie + 3; i < icdLengte; ++i)
  {
    if(m_data[i])
    {
      return false;
    }
  }
  return true;
}

// Icd::HeeftDecimalen
// Omschrijving: Retourneert true als getal decimalen achter de komma heeft
// Werking:      Controleert decimalen posities
bool 
Icd::HeeftDecimalen() const
{
  for(int x = 0; x < icdKommaPositie; ++x)
  {
    if(m_data[x])
    {
      return true;
    }
  }
  return false;
}

// Icd::Exponent
// Omschrijving: Retourneert de basis 10 exponent van het ICD getal
// Werking:      Deelt/vermenigvuldigt net zo lang tot getal in range 0-10 ligt
//
int
Icd::Exponent() const
{
  int exponent = icdKommaPositie * icdDigits - 1;
  for(int ind = icdLengte - 1; ind >= 0; --ind)
  {
    long deler = icdBasis / 10;

    for(int num = icdDigits; num > 0; --num)
    {
      long test = m_data[ind] / deler;
      if(test)
      { 
        return exponent;
      }
      --exponent;
      deler /= 10;
    }
  }
  return 0;
}

// Icd::Mantissa
// Omschrijving: Retourneert de basis 10 mantissa van het getal
Icd
Icd::Mantissa() const
{
  long expo = Exponent();

  if(expo == 0)
  {
    return *this;
  }
  long schuif = expo / icdDigits;
  long rest   = expo % icdDigits;
  Icd  getal  = *this;
  int  teken  = getal.GeefTeken();

  getal = getal.TrekGelijk(getal,-schuif);
  if(expo > 0)
  {
    while(rest--)
    {
      getal.Div10();
    }
  }
  else
  {
    while(rest++)
    {
      getal.Mult10();
    }
  }
  // Teken weer toevoegen.
  if(teken < 0)
  {
    getal = -getal;
  }
  return getal;
}

//////////////////////////////////////////////////////////////////////////
//
// EINDE ICD GETTERS
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// ICD BASIC OPERATIES
//
//////////////////////////////////////////////////////////////////////////

// Icd::Add
// Omschrijving:   + operator, optelling van twee Icd's
// Parameters:     const Icd& Icd
// Werking:        eerst wordt het teken van het eindresultaat bepaald en
//                 wordt gekeken of er positief opgeteld of afgetrokken moet
//                 worden, hierna wordt 1 van deze twee acties uitgevoerd, waarna
//                 het resultaat wordt teruggegeven.
//
Icd
Icd::Add(const Icd& icd) const
{
  // Bepalen of opgeteld of afgetrokken moet worden, eventueel
  // argumenten omdraaien.
  // (+x) + (+y) -> optellen , resultaat positief, niet omdraaien
  // (+x) + (-y) -> aftrekken, resultaat positief, niet omdraaien
  // (-x) + (+y) -> aftrekken, resultaat positief, omdraaien
  // (-x) + (-y) -> optellen,  resultaat negatief, niet omdraaien
  Teken tekenResultaat;
  SoortOperator soortOperator;
  Icd arg1(*this);
  Icd arg2(icd);
  PositioneerArgumenten(arg1, arg2, tekenResultaat, soortOperator);

  if (soortOperator == Optellen)
  {
    arg1 = TelPositiefOp(arg1, arg2);
  }
  else
  {
    arg1 = TrekPositiefAf(arg1, arg2);
    //als teken negatief geworden tijdens aftrekken, dan teken omdraaien
    if (arg1.m_teken == Negatief)
    {
      if (tekenResultaat == Positief)
      {
        tekenResultaat = Negatief;
      }
      else
      {
        tekenResultaat = Positief;
      }
    }
  }
  arg1.m_teken = tekenResultaat;

  return arg1;
}

// Icd::Sub
// Omschrijving:   - operator, aftrekken van twee Icd's
// Werking:        gebruikt de + operator en de prefix - (x-y is equivalent aan x+(-y))
//
Icd
Icd::Sub(const Icd& Icd) const
{
  // x-y is gelijk aan x+(-y)
  return *this + (-Icd);
}

// Icd::Mul
// Omschrijving:   * operator, vermenigvuldiging van twee Icd's
//
Icd
Icd::Mul(const Icd& icd) const
{
  // Vermenigvuldigen zonder rekening te houden met het teken
  Icd res = VermenigvuldigPositief(*this, icd);

  // Teken bepalen
  res.m_teken = res.IsNul() ? Positief : BepaalTeken(*this, icd);

  return res;
}

// Icd::Div
// Omschrijving:   / operator, deling van twee Icd's
// Werking:        als noemer ongelijk nul, dan positief delen en daarna
//                 het teken bepalen.
//
Icd
Icd::Div(const Icd& icd) const
{
  // Als noemer nul, dan exceptie
  if (icd.IsNul())
  {
    throw CString("Gebroken getal (ICD): Delen door nul");
  }
  // Delen zonder rekening te houden met het teken
  Icd res = DeelPositief(*this, icd);

  // Teken bepalen
  res.m_teken = res.IsNul() ? Positief : BepaalTeken(*this, icd);

  return res;
}

// Icd::Mod
// Omschrijving: Modulo operator
// Werking:      Modulo berekening in icd zelf
Icd
Icd::Mod(const Icd& icd) const
{
  Icd aantal = ((*this) / icd).WaardeVoorKomma();
  Icd mod((*this) - (aantal * icd));
  if (m_teken == Negatief)
  {
    -mod;
  }
  return mod;
}

//////////////////////////////////////////////////////////////////////////
//
// EINDE ICD BASIC OPERATIES
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// ICD PRIVATE FUNCTIES
// ALLE INTERNE FUNCTIES OM ICD MOGELIJK TE MAKEN
//
//////////////////////////////////////////////////////////////////////////

// Neem de absolute waarde van een long
// Deze functie gaat om de <math> bibliotheek en algemene macro's heen
// Nodig omdat diverse versies van std::abs verkeerd linkten
long 
Icd::long_abs(const long waarde) const
{
  if(waarde < 0)
  {
    return -waarde;
  }
  return waarde;
}

// Geeft de 10 of 10000 macht van een getal
// Wordt gebruikt voor snelle shift operaties in een ICD
// (operand ligt altijd tussen 0 en 4)
long 
Icd::long_pow(long basis,int operand) const
{
  long result = 1;
  if(operand > 0)
  {
    for(short i=0;i<operand;++i)
    {
      result *= basis;
    }
    return result;
  }
  // Ga er van uit dat basis tot de macht 0 = 1;
  return 1;
}

// Retourneert de floor(log10(waarde)) 
// Nodig voor snelle shift operaties
// Result wordt maximaal 3.
long
Icd::long_log10(long waarde) const
{
  int result = 0;

  while(waarde >= 10)
  {
    ++result;
    waarde /= 10;
  }
  return result;
}

// Icd::ZetWaardeLong
// Omschrijving:   Geeft de Icd de waarde van twee longs. De eerste geeft het deel
//                 voor de komma weer, de tweede het deel na de komma. Het teken wordt
//                 bepaald door de eerste long, tenzij deze gelijk is aan nul. De tweede
//                 long mag niet groter zijn dan de basis (=10000)
// Parameters:     const long waarde     //waarde voor komma
//                 const long restWaarde //waarde na de komma (precies 4 cijfers)
// Werking:        Bepaald het teken door het teken van waarde te nemen,
//                 tenzij waarde=0, dan wordt het teken van restWaarde genomen.
//                 Vervolgens wordt de eerste m_data voor de komma gelijk aan
//                 waarde, en de eerste m_data na de komma gelijk aan restWaarde,
//                 waarbij restWaarde kleiner dan IcdBasis wordt gemaakt dmv MOD operatie
//                 Hierna volgt een herformatteer om de Icd valide te maken.
//
void
Icd::ZetWaardeLong(const long waarde, const long restWaarde)
{
  // Default leeg
  MaakLeeg();

  // Bepaal het teken.
  if (waarde == 0)
  {
    m_teken = (restWaarde < 0) ? Negatief : Positief;
  }
  else
  {
    m_teken = (waarde < 0) ? Negatief : Positief;
  }
  // Vul de cijfertjes.
  m_data[icdKommaPositie-1] = long_abs(restWaarde % icdBasis);
  m_data[icdKommaPositie  ] = long_abs(waarde);
  Herformatteer();
}

// Icd::ZetWaardeDouble
//
// Deze versie van ZetWaardeDouble gebruikt dus géén sprintf meer, maar
// berekent de inhoud van het m_data array vanuit de double waarde.
// De sprintf maakt namelijk zijn eigen afrondings fouten.
// Wij maken graag hier onze eigen bugs!
void
Icd::ZetWaardeDouble(const double waarde)
{
  // Maak de ICD initieel leeg
  MaakLeeg();

  // Bepaal het teken.
  double tmpwaarde = fabs(waarde);
  m_teken = (waarde < 0.0) ? Negatief : Positief;

  // Bepaal deel voor en na de komma
  double deel = floor(tmpwaarde);
  double frac = tmpwaarde - deel;

  // Vul eerst het deel voor de komma
  // 5E-9 = 5E(icdDigits + 1)
  int idx = 0;
  while(deel && idx<=icdKommaPositie)
  {
    tmpwaarde = deel / icdBasis;
    deel      = tmpwaarde - floor(tmpwaarde);
    m_data[icdKommaPositie+idx] = (unsigned long)floor((deel * icdBasis) + 5E-9);
    deel      = floor(tmpwaarde);
    ++idx;
  }
  // Vul nu het deel na de komma 
  idx = icdKommaPositie - 1;
  while(frac && idx >= (icdKommaPositie - 2))
  {
    frac       *= icdBasis;
    tmpwaarde   = floor(frac);
    m_data[idx] = (unsigned long) (tmpwaarde + 0.5);
    frac       -= tmpwaarde;
    --idx;
  }
  // Afronden op 14 posities
  // Afhankelijk van icdDigits !!!
  if(idx == (icdKommaPositie - 2))
  {
    if(m_data[idx])
    {
      m_data[idx] = 100 * (m_data[idx] / 100);
    }
  }
  Herformatteer();
}

// Icd::ZetWaardeString
// Omschrijving:   Zet de waarde van een string in een Icd. Tekens zonder betekenis
//								 worden genegeerd. Conversie geschiedt op basis van locale
// Informix gaat eromheen -> fromDB is lezen uit de database altijd met '.'
//
void
Icd::ZetWaardeString(CString waarde, bool fromDB)
{
  // Maak de ICD leeg
  MaakLeeg();

  //	Lege string
  if(!waarde.GetLength())
  {
    return; // Eigenlijk throwen?!!
  }

  // Bepaal decimaal- en duizendscheider
  char dec[2] = { '.', 0 };
  char dui[2] = { ',', 0 };
  if(!fromDB)
  {
    dec[0] = ',';
    dui[0] = '.';
  }

  // Verwijder duizendscheider
  waarde.Remove(dui[0]);

  // Vervang decimaalscheider
  if(dec[0] != '.')
  {
    waarde.Replace(dec[0], '.');
  }

  // Vervang PI / LN2 / LN10
  if(waarde.CompareNoCase("PI") == 0)
  {
    *this = PI();
    return;
  }
  if(waarde.CompareNoCase("LN2") == 0)
  {
    *this = LN2();
    return;
  }
  if(waarde.CompareNoCase("LN10") == 0)
  {
    *this = LN10();
    return;
  }

  // Begin van waarde
  char const* ptr = waarde;

  // Bepaal sign
  m_teken = Positief;
  if(*ptr == '-')
  {
    m_teken = Negatief;
    ++ptr;
  }
  else if(*ptr == '+')
  {
    m_teken = Positief;
    ++ptr;
  }

  // Bepaal de lengte van het integere deel
  char const* from = ptr;
  char const* next = ptr;
  while(isdigit(*next))
  {
    ++next;
  }

  // Verwerk het integere deel
  long power = 1;
  long index = icdKommaPositie;
  for(ptr = next - 1; index < icdLengte, ptr >= from; --ptr)
  {
    if(index > icdLengte - 1)
    {
      // ICD Overflow
      throw CString("ICD Overflow");
    }
    m_data[index] += power * (long)(*ptr - '0');
    if((power *= 10) >= icdBasis)
    {
      power = 1;
      ++index;
    }
  }

  // Ga verder na het integere deel
  ptr = next;

  // Verwerk decimale deel
  if(*ptr == '.')
  {
    power = icdBasis / 10;
    index = icdKommaPositie - 1;
    for (++ptr; *ptr >= '0' && *ptr <= '9'; ++ptr)
    {
      m_data[index] += power * (long)(*ptr - '0');
      if((power /= 10) == 0)
      {
        power = icdBasis / 10;
        if(--index < 0)
        {
          // Negeer overgebleven decimalen
          while(*ptr >= '0' && *ptr <= '9')
          {
            ++ptr;
          }
          break;
        }
      }
    }
  }

  // Bepaal exponent
  int exponent = 0;
  if(*ptr == 'e' || *ptr == 'E')
  {
    ++ptr;
    char const* sign = ptr;
    if(*ptr == '+' || *ptr == '-')
    {
      ++ptr;
    }
    while(*ptr >= '0' && *ptr <= '9')
    {
      exponent *= 10;
      exponent += *ptr - '0';
      ++ptr;
    }
    if(*sign == '-')
    {
      exponent = -exponent;
    }
  }
  // Pas exponent toe
  if(exponent)
  {
    PasEFactorToe(exponent);
  }

  // String moet nu leeg zijn
  if(*ptr)
  {
    // Kan niet converteren naar Icd
    throw CString("Kan niet converteren naar ICD");
  }
}

// Icd::ZetWaardeInt64
// Omschrijving: Zet een int64 getal in de ICD
//               En optioneel de restwaarde achter de komma
void
Icd::ZetWaardeInt64(const int64 waarde, const int64 restWaarde)
{
  int64 rest = 0;

  // Default leeg
  MaakLeeg();

  // Bepaal het teken.
  if (waarde == 0)
  {
    m_teken = (restWaarde < 0) ? Negatief : Positief;
  }
  else
  {
    m_teken = (waarde < 0) ? Negatief : Positief;
    rest    = (waarde < 0) ? -waarde : waarde;
  }

  // Vul de waarde in
  for(int ind = icdKommaPositie; rest > 0 && ind < icdLengte; ++ind)
  {
    m_data[ind] = rest % icdBasis;
    rest /= icdBasis;
  }
  // Vul de restwaarde in
  int64 deel = restWaarde < 0 ? -restWaarde : restWaarde;
  for(int ind = icdKommaPositie -1; deel > 0 && ind >= 0; --ind)
  {
    m_data[ind] = deel % icdBasis;
    deel /= icdBasis;
  }
  Herformatteer();
}

// Icd::PasEFactorToe
// Omschrijving: Icd tot een 10e macht verheffen
void
Icd::PasEFactorToe(int factor)
{
  static const int positiesVoorDeKomma = icdKommaPositie * icdDigits;
  static const int positiesAchterDeKomma = (icdLengte - icdKommaPositie) * icdDigits;
  static const Icd basisICD((long)icdBasis);

  // Eerst zorgen dat de factor binnen de range [-9,+9] ligt.
  while (factor > 9)
  {
    *this = Mul(1000000000L /* 9 posities */);
    factor -= 9;
  }
  while (factor < -9)
  {
    *this = Div(1000000000L /* 9 posities */);
    factor += 9;
  }
  // En dan der rest van de verschuiving doen met een Icd-vermenigvuldiging. De
  // long_pow kan nu, omdat 10^factor binnen de grenzen van een 32-bits long ligt.
  if (factor > 0)
  {
    *this = Mul(long_pow(10,factor));
  }
  else if (factor < 0)
  {
    *this = Div(long_pow(10,-factor));
  }
}

// Icd::Herformatteer
// Omschrijving:   Zorgt ervoor dat alle m_data members weer correct zijn (<IcdBasis)
// Excepties:      Als de Icd te groot is geworden, dan volgt een BKIcdOverflow exceptie
// Werking:        van de laagste m_data totaan de hoogste wordt de waarde van m_data
//                 afgekapt, zodat m_data<IcdBasis. Het restant wordt opgeteld bij de
//                 volgende m_data. (IcdOverflow als deze er niet meer is)
//
void
Icd::Herformatteer()
{
  // Doorschuiven waarden boven basis naar hogere m_data
  long rest = 0;
  for (long i = 0; i < icdLengte; i++)
  {
    m_data[i] += rest;
    rest       = m_data[i] / icdBasis;
    m_data[i]  = m_data[i] % icdBasis;
  }
  if (rest > 0)
    // Als op het einde nog een restant, dan overflow
  {
    throw CString("Gebroken getal te groot (ICD Overflow)");
  }
}

// Icd::MaakLeeg
// Omschrijving:   Icd gelijk maken aan 0
//
void
Icd::MaakLeeg()
{
  // data members initialiseren op +0000...0000,0000...0000
  m_teken    = Positief;
  m_lengte   = icdLengte * icdDigits;
  m_precisie = m_lengte / 2;
  for (long i = 0; i < icdLengte; ++i)
  {
    m_data[i] = 0;
  }
}

// Icd::Multi
// Omschrijving:   Vermenigvuldigt een (positieve) Icd met een (positieve) long.
//                 Indien deze Icd te groot wordt, dan zal het teken op negatief
//                 worden gezet.
// Parameters:     const long multiplier //aantal keren te vermenigvuldigen
// Retourneert:    this*multiplier
// Preconditie:    m_teken is positief, de parameter is positief
// Werking:        Vermenigvuldigt alle afzonderlijke m_data met de parameter
//                 waarna een herformatteer actie volgt. Deze wordt hier uitgevoerd,
//                 omdat er geen exceptie mag komen, maar bij een overflow het teken
//                 negatief wordt gemaakt. (Overloading van de m_teken member!!!)
//
const Icd
Icd::Multi(const long multiplier) const
{
  int64 tussen = 0;
  int64 rest   = 0;
  Icd res(*this);

  // Vermenigvuldigen
  for(long i = 0; i < icdLengte; ++i)
  {
    tussen         = (int64)res.m_data[i] * (int64)multiplier;
    tussen        += rest;
    res.m_data[i]  = (long) (tussen % icdBasis);
    rest           = tussen / icdBasis;
  }
  if(rest > 0)
  {
    // Als overflow, dan teken negatief zodat dit later nog herkenbaar is.
    res.m_teken = Negatief;
  }
  return res;
}

// Icd::IsInhoudGrensgeval
// Omschrijving: Test of Icd te groot wordt voor precisie
//
const bool
Icd::IsInhoudGrensGeval() const
{
  // testen op een grensgeval waarbij door afronding de lengte te groot wordt.
  // bijv: 9,995 met lengte = 3 en precisie = 2, wordt afgerond 10,00 (lengte = icdDigits!)

  int hoogsteIndex = (icdDigits * icdKommaPositie + (m_lengte - m_precisie) - 1) / icdDigits;
  int laagsteIndex = (icdDigits * icdKommaPositie - m_precisie) / icdDigits;


  int index = hoogsteIndex;

  while (index >= laagsteIndex)
  {
    unsigned long dataWaarde = m_data[index];
    unsigned long maxWaarde;

    if (index == hoogsteIndex)
    {
      maxWaarde = (unsigned long) (long_pow(10, (m_lengte - m_precisie) % icdDigits) - 1);
      if (maxWaarde == 0)
      {
        maxWaarde = 9999;
      }
      if (dataWaarde != maxWaarde)
      {
        if (index != laagsteIndex)
        {
          return false;
        }
      }
    }
    if (index == laagsteIndex)
    {
      int orde = (icdDigits - (m_precisie % icdDigits)) % icdDigits;
      unsigned long relevantDeel = dataWaarde / (unsigned long)(long_pow(10, orde));
      maxWaarde = (unsigned long) (long_pow(10, (icdDigits - orde)) - 1);  

      if (relevantDeel == maxWaarde)
      {
        // bepalen van het getal dat voor de afronding zorgt. (eerste getal na precisie)
        int getal;
        if (orde > 0)
        {
          getal = ((dataWaarde % (unsigned long) long_pow(10, orde)) / (unsigned long) long_pow(10, orde - 1));
        }
        else
        {
          // getal zit in het volgende datablok.
          getal = m_data[index - 1] / 1000;
        }

        if (getal >= 5)
        {
          return true;
        }
        else 
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
    if (!(((index != hoogsteIndex) || (index != laagsteIndex)) && (dataWaarde == 9999)))
    {
      return false;
    }
    --index;
  }
  return false;
}

// Icd::PositioneerArgumenten
//
// Omschrijving:   Positioneert twee argumenten op basis van hun teken
//                 voor optel en aftrek operaties:
//                 (+x) + (+y) -> optellen , resultaat positief, niet omdraaien
//                 (+x) + (-y) -> aftrekken, resultaat positief, niet omdraaien
//                 (-x) + (+y) -> aftrekken, resultaat positief, omdraaien
//                 (-x) + (-y) -> optellen,  resultaat negatief, niet omdraaien
//
// Parameters:     Icd& arg1 //argument aan de linkerkant
//                 Icd& arg2 //argument aan de rechterkant
//                 Teken& tekenResultaat //uiteindelijk teken
//                 SoortOperator& soortOperator //uit te voeren operator (+ of -)
//
// Werking:        Op basis van de tekens wordt bepaald wat het teken wordt van
//                 het eindresultaat, en welke operator moet worden uitgevoerd.
//                 eventueel worden de argumenten verwisseld.
//                 Als laatste worden de tekens van de argumenten positief gemaakt.
//
void
Icd::PositioneerArgumenten(Icd& arg1,
                           Icd& arg2,
                           Teken& tekenResultaat,
                           SoortOperator& soortOperator)
{
  // Bepaal resultaat teken en soort operator
  // Indien (-x) + y dan omdraaien, dus x + (-y), wordt x - y
  if (arg1.m_teken == Positief)
  {
    if (arg2.m_teken == Positief)
    {
      tekenResultaat = Positief;
      soortOperator = Optellen;
    }
    else
    {
      tekenResultaat = Positief;
      soortOperator = Aftrekken;
    }
  }
  else
  {
    if (arg2.m_teken == Positief)
    {
      tekenResultaat = Positief;
      soortOperator = Aftrekken;
      Icd dummy(arg1);
      arg1 = arg2;
      arg2 = dummy;
    }
    else
    {
      tekenResultaat = Negatief;
      soortOperator = Optellen;
    }
  }
  //Positief doorwerken, dus teken argumenten zetten
  arg1.m_teken = Positief;
  arg2.m_teken = Positief;
}

// Icd::TelPositiefOp
// Omschrijving:   Telt twee Icd's bij elkaar op zonder rekening te houden met
//                 het teken.
// Parameters:     const Icd& arg1 //argument aan de linkerkant
//                 const Icd& arg2 //argument aan de rechterkant
// Retourneert:    arg1+arg2 als Icd
// Preconditie:    arg1.m_teken=positief, arg2.m_teken=positief
// Excepties:      BKIcdOverflow indien het resultaat niet in een Icd past
// Werking:        telt alle m_data van het eerste argument 1-aan-1 op bij
//                 de m_data van het twee argument. Hierna volgt een herformatteer
//                 actie.
//
const Icd
Icd::TelPositiefOp(const Icd& arg1, const Icd& arg2)
{
  // Datamembers bij elkaar optellen
  Icd res(arg1);
  for (long i = 0; i < icdLengte; i++)
  {
    res.m_data[i] += arg2.m_data[i];
  }
  // Herformatteer actie
  res.Herformatteer();
  return res;
}

// Icd::TrekPositiefAf
// Omschrijving:   Trekt twee Icd's van elkaar af zonder rekening te houden met
//                 het teken.
// Parameters:     const Icd& arg1 // argument aan de linkerkant
//                 const Icd& arg2 // argument aan de rechterkant
// Retourneert:    arg1-arg2 als Icd
// Preconditie:    arg1.m_teken=positief, arg2.m_teken=positief
// Excepties:      BKIcdOverflow indien het resultaat te klein.
// Werking:        Voor alle m_data van argument 1 en 2 wordt
//                 m_data[i]-m_data[i] uitgevoerd. Indien de linker m_data
//                 kleiner dan de rechter, dan wordt icdBasis bij de linker opgeteld.
//                 vervolgens wordt 1 bij de volgende rechter m_data opgeteld, tenzij
//                 deze niet bestaat, dan volgt een tekenwissel.
//                 Indien op het einde blijkt dat er een tekenwissel is opgetreden,
//                 dan moet het resultaat nog van IcdMax+1 afgetrokken.
//
const Icd
Icd::TrekPositiefAf(const Icd& arg1, const Icd& arg2)
{
  // Tijdelijke Icd's aanmaken
  Icd res(arg1);
  Icd mina(arg2);

  // res = res - mina
  for (long i = 0; i < icdLengte; i++)
  {
    if (res.m_data[i] < mina.m_data[i])
    {
      // kleiner, dus lenen van buurman
      res.m_data[i] += icdBasis - mina.m_data[i];
      if (i < icdLengte - 1)
      {
        // Let op: We doen hier dus niet: res.m_data[i+1]--
        // zoals we bij een het lenen zouden doen,
        // omdat deze member op nul zou kunnen staan!!
        mina.m_data[i+1]++;
      }
      else
      { 
        //tekenwissel!
        res.m_teken = Negatief;
      }
    }
    else
    {
      res.m_data[i] -= mina.m_data[i];
    }
  }
  if (res.m_teken == Negatief)
  {
    // tekenwissel, dan klopt het resultaat nog niet, maar
    // moet nog van maxIcd afgetrokken worden.
    // N.B.: recursieve aanroep van TrekPositiefAf!
    Icd maxIcd;
    maxIcd.m_data[icdLengte-1] = icdBasis;
    res = -(maxIcd + res);
  }
  return res;
}

// Icd::VermenigvuldigPositief
// Omschrijving:   Vermenigvuldigt twee Icd's zonder rekening te houden met het teken
// Parameters:     Icd& arg1 // argument aan de linkerkant
//                 Icd& arg2 // argument aan de rechterkant
// Retourneert:    arg1*arg2
// Preconditie:    arg1.m_teken=positief, arg2.m_teken=positief
// Excepties:      BKIcdOverflow als het resultaat te groot
// Werking:        Vermenigvuldigt alle afzonderlijke m_data's volgens de
//                 'klassieke' handmatige vermenigvuldiging. Het resultaat komt
//                 in een aantal longs (2x zoveel als in een normale Icd). Hierdoor
//                 kunnen alle waarden opgeslagen worden. Als het resultaat (na het
//                 verplaatsen van de komma) toch niet past in een Icd, dan volgt een
//                 BKIcdOverflow exceptie. Uiteindelijk volgt nog een herformatteer om
//                 de Icd weer correct te krijgen.
//
const Icd
Icd::VermenigvuldigPositief(const Icd& arg1, const Icd& arg2)
{
  // tijdelijke m_data's in nieuwe structuur opslaan, hierdoor
  // geen langere Icd's noodzakelijk.
  // alle onderdelen op 0 zetten.
  int64 res[icdLengte * 2 + 1] = {0};
  int64 tussen = 0;

  // vermenigvuldigen, tussenresultaat opslaan in res
  for (int i = 0; i < icdLengte; i++)
  {
    for (int j = 0; j < icdLengte; j++)
    {
      tussen      = (int64)arg1.m_data[i] * (int64)arg2.m_data[j];
      res[i+j]   += (long) (tussen % icdBasis);
      res[i+j+1] += (long) (tussen / icdBasis);

      // Check op overflow
      if (((i+j) >= (icdLengte + icdKommaPositie)) && (res[i+j] != 0))
      {
        // als iets opgeslagen boven de lengte en de kommapositie, dan overflow
        throw CString("Gebroken getal te groot (ICD Overflow)");
      }
    }
  }
  // Herformatteer
  int64 rest = 0;
  for (long i = 0; i < icdLengte * 2; i++)
  {
    res[i] += rest;
    rest    = res[i] / icdBasis;
    res[i]  = res[i] % icdBasis;
  }
  // Als op het einde nog een restant, dan overflow
  if (rest > 0)
  {
    throw CString("Gebroken getal te groot (ICD Overflow)");
  }


  // resultaat in Icd zetten, hierbij rekening houden met de
  // nieuwe positie van de komma
  Icd Icdres;
  for (int i = 0; i < icdLengte; i++)
  {
    Icdres.m_data[i] = (long) res[i+icdKommaPositie];
  }
  return Icdres;
}

// Icd::DeelPositief
// Omschrijving:   Deelt twee Icd's zonder rekening te houden met het teken
// Parameters:     Icd& arg1 // teller
//                 Icd& arg2 // noemer (ongelijk aan nul)
// Retourneert:    arg1/arg2
// Preconditie:    arg1.m_teken=positief, arg2.m_teken=positief, arg2!=0
// Excepties:      BKIcdOverflow als het resultaat te groot
// Werking:        Eerst wordt bepaald wat de belangrijkste m_data!=0 is, voor zowel
//                 arg1 als arg2. Vervolgens wordt de (uiteindelijke) positie van de
//                 komma bepaald op basis van deze gegevens. Hierna worden de m_data in
//                 het resultaat bepaald. Dit gaat volgens de 'klassieke' staartdeling.
//                 Om een getal te vinden, worden de twee meest belangrijke m_data's op
//                 elkaar gedeeld. Hiermee krijg je de hoogst mogelijke waarde van het
//                 quotient om de laagst mogelijke waarde van het quotient te vinden,
//                 deel je de meest belangrijke m_data van de teller door de meest
//                 belangrijke m_data van de noemer PLUS 1. Hierna kun je mbv een binary-
//                 search algorithme achter de werkelijke waarde van het quotient komen.
//                 Hiervoor wordt nl. de noemer met het quotient vermenigvuldigd. Dit
//                 quotient is nu valide indien de teller groter of gelijk is aan dit produkt.
//
const Icd
Icd::DeelPositief(const Icd& arg1, const Icd& arg2)
{
  // teller wordt argument 1
  // noemer wordt argument 2
  // res    wordt het uiteindelijke resultaat.
  // teller wordt ook gebruikt om nog te delen gedeelte van
  // de teller in op te slaan.
  Icd teller(arg1);
  Icd noemer(arg2);
  Icd res;

  // Alleen positief delen, later pas teken bepalen
  teller.m_teken = Positief;
  noemer.m_teken = Positief;

  // index1 is hoogste teller.m_data die ongelijk is aan nul
  // index2 is hoogste noemer.m_data die ongelijk is aan nul
  long index1 = icdLengte - 1;
  long index2 = icdLengte - 1;
  while (index1 > 0 && teller.m_data[index1] == 0)
  {
    index1--;
  }
  while (index2 > 0 && noemer.m_data[index2] == 0)
  {
    index2--;
  }

  // resindex bepaald de positie van waaraf het resultaat wordt
  // ingevuld. Hierdoor wordt de kommapositie vooraf al goed gezet.
  long resindex = index1 - index2 + icdKommaPositie;

  // quotient is het 1 resultaat-getalletje van een stap in het deelproces
  // (vergelijk de staart-deling)
  // noemerGelijk is de noemer gelijkgetrokken met de teller (dwz:
  // het meest belangrijke cijfer van de noemer staat op dezelfde plaats
  // als de meest belangrijke cijfer van de teller.
  // Voorbeeld: als teller is 12:32:43 en noemer 32:43, dan wordt de
  //            noemer 32:43:00.
  // multires is de te testen waarde (=quotient*noemer, gelijkgetrokken met
  // de teller)
  Icd multires;
  Icd noemerGelijk;

  // feitelijke deelactie, doorgaan totdat het einde van het resultaat bereikt is
  // (omdat doorgaan toch niet relevant is, dan worden slechts getallen verder dan
  // 8 cijfers achter de komma bepaald.

  while (resindex >= 0 && index1 >= 0)
  {
    // quotient_test is gelijk aan het meest belangrijke cijfer uit de teller
    // gedeeld door het meest belangrijke getal uit de noemer. Indien er nog
    // een rest was van de vorige deling, dan wordt dit ook meegenomen
    // Voor optimalisatie wordt een binary search algorithme toegepast.
    // Hierbij wordt aangenomen dat a/b een te groot quotient kan opleveren en
    // a/(b+1) een te klein quotient. Hierdoor is een bovengrens en een ondergrens
    // te bepalen. quotient_test zit nu altijd precies tussen de grenzen.
    // quotient_onder_oud is de hoogste ondergrens die nog kon.
    int64 quotient_boven = ((int64)teller.m_data[index1] + ((index1 == icdLengte-1) ? 0 : (int64)teller.m_data[index1+1] * icdBasis)) / (int64)noemer.m_data[index2];
    int64 quotient_onder = ((int64)teller.m_data[index1] + ((index1 == icdLengte-1) ? 0 : (int64)teller.m_data[index1+1] * icdBasis)) / (int64)(noemer.m_data[index2] + 1);
    int64 quotient_onder_oud = 0;
    int64 quotient_test = (quotient_boven + quotient_onder) / 2;

    // bepalen van multires. Als de vermenigvuldiging een overflow veroorzaakt, dan
    // wordt het teken negatief, en moet het quotient (dus) kleiner worden
    noemerGelijk = TrekGelijk(noemer, index1 - index2);
    bool teGroot;
    multires = noemerGelijk.Multi((long)quotient_test);
    teGroot = multires.m_teken == Negatief;

    // doorgaan zolang ondergrens niet gelijk aan de bovengrens
    while (quotient_boven != quotient_onder)
    {
      if (teller < multires || teGroot)
      {
        quotient_boven = quotient_test;
      }
      else
      {
        // valide ondergrens bewaren!
        quotient_onder_oud = quotient_test;
        if (((quotient_onder + quotient_boven) % 2) == 1)
        {
          // Bij afrondfouten de ondergrens 1 hoger kiezen
          quotient_onder = quotient_test + 1;
        }
        else
        {
          quotient_onder = quotient_test;
        }
      }
      quotient_test = (quotient_boven + quotient_onder) / 2;
      noemerGelijk = TrekGelijk(noemer, index1 - index2);
      multires = noemerGelijk.Multi((long)quotient_test);
      teGroot = multires.m_teken == Negatief;
    }
    // Als quotient toch nog te hoog, dan oude ondergrens pakken
    if (teller < multires || teGroot)
    {
      quotient_test = quotient_onder_oud;
      noemerGelijk = TrekGelijk(noemer, index1 - index2);
      multires = noemerGelijk.Multi((long)quotient_test);
    }
    // Quotient toekennen, indien ongelijk aan nul
    if (quotient_test != 0)
    {
      if (resindex >= icdLengte)
      {
        // overflow bij te groot getal in resultaat
        throw CString("Gebroken getal te groot (ICD Overflow)");
      }
      else
      {
        res.m_data[resindex] = (long)quotient_test;
      }
    }
    // teller verkleinen. Indien er nu een positie aan de linkerkant vrij gekomen is,
    // dan de teller doorschuiven (hierdoor voorkom je dat je later aan de rechterkant
    // posities te weinig hebt. (het zgn. 0 bijplakken bij een staart-deling)
    teller = teller - multires;
    if (teller.m_data[icdLengte-1] == 0)
    {
      for (long i = icdLengte - 1; i > 0; i--)
      {
        teller.m_data[i] = teller.m_data[i-1];
      }
      teller.m_data[0] = 0;
    }
    else
    {
      --index1;
    }
    --resindex;
  }
  return res;
}

// Icd::BepaalTeken
// Omschrijving:   Bepaalt het teken van een vermenigvuldiging of deling:
//                 x of y is nul -> positief
//                 (+x)*(+y) -> positief
//                 (+x)*(-y) -> negatief
//                 (-x)*(+y) -> negatief
//                 (-x)*(-y) -> positief
//
// Parameters:     const Icd& arg1 // 1e argument
//                 const Icd& arg2 // 2e argument
//
// Werking:        Kijkt eerst of 1 van de argumenten gelijk is aan 0, anders
//                 wordt het nieuwe teken bepaald uit de tekens van de argumenten.
//
const Icd::Teken
Icd::BepaalTeken(const Icd& arg1, const Icd& arg2)
{
  // Teken bepalen bij vermenigvuldiging en delen
  // - 1 van beide argumenten 0 -> positief
  // (+x) * (+y) -> positief
  // (-x) * (+y) -> negatief
  // (-x) * (-y) -> positief
  // (+x) * (-y) -> negatief
  if (arg1.IsNul() || arg2.IsNul())
  {
    return Positief;
  }
  if (arg1.m_teken == Positief)
  {
    if (arg2.m_teken == Positief)
    {
      return Positief;
    }
    else
    {
      return Negatief;
    }
  }
  else
  {
    if (arg2.m_teken == Negatief)
    {
      return Positief;
    }
    else
    {
      return Negatief;
    }
  }
}

// Icd::TrekGelijk
// Omschrijving:   Schuift een Icd een aantal plaatsen naar links of naar rechts,
//                 afhankelijk van het tweede argumenten.
// Parameters:     const Icd& arg1 // gelijk te trekken Icd
//                 const long verschil // verschil in grootte tussen twee Icd's
// Retourneert:    opgeschoven Icd
// Werking:        wordt gebruikt bij deling om noemer en teller op hetzelfde niveau
//                 te brengen. Afhankelijk van het teken van het twee argument worden
//                 de m_data's verschoven.
//
const Icd
Icd::TrekGelijk(const Icd& arg1, const long verschil)
{
  Icd res;

  // Geen verschil, dan niks doen.
  if (verschil == 0)
  {
    res = arg1;
  }
  else
  {
    if (verschil < 0)
    {
      // noemer groter dan teller, dan noemer naar rechts opschuiven
      // voorbeeld: 31:12:00 wordt 00:31:12 bij verschil van -1
      for (long i = 0; i < icdLengte + verschil; i++)
      {
        res.m_data[i] = arg1.m_data[i-verschil];
      }
    }
    else
    {
      // noemer kleiner dan teller, dan noemer naar links opschuiven
      // voorbeeld: 00:31:12 wordt 31:12:00 bij verschil van 1
      for (long i = icdLengte - 1; i >= verschil; i--)
      {
        res.m_data[i] = arg1.m_data[i-verschil];
      }
    }
  }
  return res;
}

// Icd::Div10
// Omschrijving: Deel mantissa (m_data) door 10
// Werking:      Puur schuifregister om door 10 te delen
//               Functioneel een deel van "TrekGelijk", maar dan met 
//               een verschuiving kleiner dan 'icdBasis'.
void
Icd::Div10()
{
  for(int ind = icdLengte - 1;ind >= 0; --ind)
  {
    long tussen = m_data[ind] / 10;
    if(ind > 0)
    {
      long extra = m_data[ind] % 10;
      m_data[ind - 1] = (extra * icdBasis) + m_data[ind - 1];
    }
    m_data[ind] = tussen;
  }
}

// Icd::Mult10
// Omschrijving: Vermenigvuldig mantissa (m_data) met 10
// Werking:      Puur schuifregister om met 10 te vermenigvuldigen
//               Functioneel een deel van "TrekGelijk", maar dan met 
//               een verschuiving kleiner dan 'icdBasis'.
void
Icd::Mult10()
{
  long carry = 0;
  for(int ind = 0; ind < icdLengte; ++ind)
  {
    long tussen = m_data[ind] * 10 + carry;
    m_data[ind] = tussen % icdBasis;
    carry       = tussen / icdBasis;
  }
  if(carry)
  {
    // Zou niet op mogen treden, omdat Mult10 alleen 'save' mag worden aangeroepen
    throw CString("Internal: ICD Overflow in suboperation");
  }
}

// Icd::LongNaarString
// Omschrijving: Long naar een string omzetten in radix 10
CString
Icd::LongNaarString(long p_getal) const
{
  char buffer[20];
  _itoa_s(p_getal,buffer,20,10);
  return CString(buffer);
}

// Icd::StringNaarLong
// Omschrijving: String naar een long omzetten
long
Icd::StringNaarLong(CString& p_string) const
{
  return atoi(p_string);
}

// Icd::Epsilon
// Omschrijving: Afbreek criterium voor iteraties
// Werking:      Transleert de fractie naar de laagste decimaal positie
//               10 -> 0,00000000000000000010
//                5 -> 0,00000000000000000005
Icd&
Icd::Epsilon(long p_fraction) const
{
  // Bereken afbreekcriterium epsilon
  static Icd epsilon;
  epsilon.m_data[0] = p_fraction;
  return epsilon;
}

//////////////////////////////////////////////////////////////////////////////
//
// Icd::AlsSQLString
//
// Omschrijving:   Geeft de waarde van de Icd terug als string met punt
//                 voor decimaalscheiding.
//
// CString
// Icd::AlsSQLString(const DBInfo *,bool strip /*=false*/) const
// {
//   String str = AlsString();
//   if(strip)
//   {
//     // Strip mechanisme wordt alleen gebruikt in de CBS om een
//     // patch op schijf te zetten en het schijfbeslag in te korten
//     char ch = '.';
//     int ind = str.Find(ch);
//     if(ind < 0) 
//     {
//       ch  = sessie->GeefLocale().DecimaalChar();
//       ind = str.Find(ch);
//     }
//     if(ind >=0)
//     {
//       int pos = str.GetLength() - 1;
//       while(pos && str.GetAt(pos) == '0')
//       {
//         --pos;
//       }
//       if(pos && str.GetAt(pos) == ch)
//       {
//         --pos;
//       }
//       str = str.Left(pos+1);
//       if(str.IsEmpty())
//       {
//         str = "0";
//       }
//     }
//   }
//   char decimaal = sessie->GeefLocale().DecimaalChar();
//   if(decimaal != '.')
//   {
//     str.Replace(decimaal, '.');
//   }
//   return str;
// }









