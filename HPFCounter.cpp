////////////////////////////////////////////////////////////////////////
//
// $Source: ?/WOCO - BRIEF4all/Brief/BriefServer/HPFCounter.cpp $
// $Author: ehuisman $
// 
// $log$
// Revision 152685 2011/01/12 10:47:58  ehuisman
//   BRIEF4all -> Suite BRIEF-E omnoemen.
//
// Copyright (c) 1995 - 2011 Centric IT Solutions
// Alle rechten voorbehouden
//
#include "stdafx.h"
#include "HPFCounter.h"


HPFCounter::HPFCounter()
{
  m_end.QuadPart = 0L;
  QueryPerformanceFrequency(&m_frequency);
  QueryPerformanceCounter(&m_start);
}

HPFCounter::~HPFCounter()
{
}

void
HPFCounter::Start()
{
  m_end.QuadPart = 0L;
  QueryPerformanceCounter(&m_start);
}

void
HPFCounter::Stop()
{
  QueryPerformanceCounter(&m_end);
}

// Get time in miliseconds
double 
HPFCounter::GetCounter()
{
  if(m_end.QuadPart == 0L)
  {
    Stop();
  }
  return ((double)(m_end.QuadPart - m_start.QuadPart)) / (double)m_frequency.QuadPart;
}
