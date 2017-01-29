#ifndef __STATE_2V_H__
#define __STATE_2V_H__

#include <ostream>

struct State2V
{
  union
  {
    struct
    {
      int x, y, Vx, Vy;
    };
    int data[4];
  };

  State2V();
  State2V(int x, int y, int Vx, int Vy);

  bool operator< (const State2V &nd) const;
  bool operator== (const State2V &nd) const;

  friend std::ostream& operator<< (std::ostream &out, const State2V &state);

  struct Hasher
  {
    size_t operator() (const State2V &s) const;
  };
};

#endif
