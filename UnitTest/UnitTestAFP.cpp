#include "stdafx.h"
#include "CppUnitTest.h"
#include "afp.h"
#include <math.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
  TEST_CLASS(UnitTestAFP)
  {
  public:

    TEST_METHOD(T001_double)
    {
      Logger::WriteMessage("Testing cast to double");

      afp af(30.0);
      double daf = (double)af;

      CString expect(_T("30.0"));
      CString result;
      result.Format(_T("%2.1f"),daf);

      Assert::AreEqual(expect.GetString(),result.GetString());
    }
  };
}