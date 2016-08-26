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
