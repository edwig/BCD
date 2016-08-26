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