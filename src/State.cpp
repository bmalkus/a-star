#include "State.h"

State::State()
{
  // empty
}

State::State(int x, int y, int v):
  x(x), y(y), v(v)
{
  // empty
}

bool State::operator< (const State &nd) const
{
  if (x == nd.x)
  {
    if (y == nd.y)
      return v < nd.v;
    return y < nd.y;
  }
  else
    return x < nd.x;
}

bool State::operator== (const State &nd) const
{
  return x == nd.x && y == nd.y;
}
