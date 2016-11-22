#ifndef __STATE_2V_H__
#define __STATE_2V_H__

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
};

#endif
