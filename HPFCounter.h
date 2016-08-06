////////////////////////////////////////////////////////////////////////
//
// $Source: ?/WOCO - BRIEF4all/Brief/BriefServer/Include/HPFCounter.h $
// $Author: ehuisman $
// 
// $log$
// Revision 152685 2011/01/12 10:47:59  ehuisman
//   BRIEF4all -> Suite BRIEF-E omnoemen.
//
// Copyright (c) 1995 - 2011 Centric IT Solutions
// Alle rechten voorbehouden
//
#pragma once

class HPFCounter
{
public:
  HPFCounter();
 ~HPFCounter();

  // Start or restart the timer
  void   Start();
  // Stop the timer
  void   Stop();
  // Get time in miliseconds
  double GetCounter();

private:
  LARGE_INTEGER m_start;
  LARGE_INTEGER m_end;
  LARGE_INTEGER m_frequency;
};