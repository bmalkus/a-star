#ifndef __STATE_H__
#define __STATE_H__

struct State
{
  int x, y, v;

  State();
  State(int x, int y, int v);

  bool operator< (const State &nd) const;
  bool operator== (const State &nd) const;
};

#endif
