#include "State2V.h"
#include "Utils.h"
#include "AStarOnBoardHelper.h"
#include <iostream>

State2V::State2V()
{
  // empty
}

State2V::State2V(int x, int y, int Vx, int Vy):
  x(x), y(y), Vx(Vx), Vy(Vy)
{
  // empty
}

bool State2V::operator< (const State2V &nd) const
{
  for (int i = 0; i < 4; ++i)
  {
    if (data[i] != nd.data[i])
      return data[i] < nd.data[i];
  }
  return false;
}

bool State2V::operator== (const State2V &nd) const
{
  return x == nd.x && y == nd.y
    && Vx == nd.Vx && Vy == nd.Vy;
}

template <>
std::vector<State2V> possible_states<State2V>(const AStarOnBoardHelper<State2V> &helper, const State2V &state)
{
  std::vector<State2V> ret;
  for (int i : { -1, 0, 1 })
  {
    for (int j : { -1, 0, 1 })
    {
      if (i == 0 && j == 0 && state.Vx == 0 && state.Vy == 0)
        continue;
      int Vx = state.Vx + i;
      int Vy = state.Vy + j;
      int x = state.x + Vx;
      int y = state.y + Vy;
      if (helper.is_legal_field(x, y))
        ret.push_back(State2V(x, y, Vx, Vy));
    }
  }
  return ret;
}

template <>
int heuristic<State2V>(const State2V &st, const State2V &nd)
{
  int diff_x = nd.x - st.x;
  int diff_y = nd.y - st.y;
  double res_x = 0.0, res_y = 0.0;
  if (diff_x > 0)
    res_x = (-2*st.Vx + 1 + sqrt(4*st.Vx*st.Vx - 4*st.Vx + 1 + 8*diff_x)) / 2;
  else if (diff_x < 0)
    res_x = (-2*st.Vx - 1 + sqrt(4*st.Vx*st.Vx + 4*st.Vx + 1 - 8*diff_x)) / -2;
  if (diff_y > 0)
    res_y = (-2*st.Vy + 1 + sqrt(4*st.Vy*st.Vy - 4*st.Vy + 1 + 8*diff_y)) / 2;
  else if (diff_y < 0)
    res_y = (-2*st.Vy - 1 + sqrt(4*st.Vy*st.Vy + 4*st.Vy + 1 - 8*diff_y)) / -2;
  return std::max(std::ceil(res_x), std::ceil(res_y));

  // int speed = st.Vx + st.Vy;

  // int distance = diff_x + diff_y;
  // int distanceInSteps = speed;
  // int steps = 1;

  // while (true)
  // {
    // if (distance <= distanceInSteps)
    // {
      // return steps;
    // }
    // distanceInSteps = distanceInSteps + steps * 2 + speed;
    // steps = steps + 1;
  // }
}

std::ostream& operator<< (std::ostream &out, State2V &state)
{
  out << "position = " << state.x << ", " << state.y << "; "
    << "speed = " << state.Vx << ", " << state.Vy;
  return out;
}
