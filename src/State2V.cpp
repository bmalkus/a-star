#include <iostream>
#include <functional>

#include "State2V.h"
#include "AStarOnBoardHelper.h"

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

size_t State2V::Hasher::operator() (const State2V &s) const
{
  return 5000*(5000*(100*size_t(s.Vx) + s.Vy) + s.x) + s.y;
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

#if !defined HEURISTIC_1_CAR ||  HEURISTIC_1_CAR == 1

template <>
int heuristic<State2V>(const State2V &st, const State2V &nd)
{
  int diff_x = nd.x - st.x;
  int diff_y = nd.y - st.y;
  double res_x = 0.0, res_y = 0.0;
  int diff_x_pos = (diff_x > 0) * 2 - 1;
  int diff_y_pos = (diff_y > 0) * 2 - 1;
  res_x = (-2*st.Vx + diff_x_pos + std::sqrt(4*st.Vx*st.Vx - diff_x_pos * 4*st.Vx + 1 + diff_x_pos * 8*diff_x)) / 2*diff_x_pos;
  res_y = (-2*st.Vy + diff_y_pos + std::sqrt(4*st.Vy*st.Vy - diff_y_pos * 4*st.Vy + 1 + diff_y_pos * 8*diff_y)) / 2*diff_y_pos;
  return std::max(std::ceil(res_x), std::ceil(res_y));
}

#elif HEURISTIC_1_CAR == 2

template <>
int heuristic<State2V>(const State2V &st, const State2V &nd)
{
  int diff_x = nd.x - st.x;
  int diff_y = nd.y - st.y;
  double res_x = 0.0, res_y = 0.0;
  if (diff_x > 0)
    res_x = (-2*st.Vx + 1 + std::sqrt(4*st.Vx*st.Vx - 4*st.Vx + 1 + 8*diff_x)) / 2;
  else if (diff_x < 0)
    res_x = (-2*st.Vx - 1 + std::sqrt(4*st.Vx*st.Vx + 4*st.Vx + 1 - 8*diff_x)) / -2;
  if (diff_y > 0)
    res_y = (-2*st.Vy + 1 + std::sqrt(4*st.Vy*st.Vy - 4*st.Vy + 1 + 8*diff_y)) / 2;
  else if (diff_y < 0)
    res_y = (-2*st.Vy - 1 + std::sqrt(4*st.Vy*st.Vy + 4*st.Vy + 1 - 8*diff_y)) / -2;
  return std::max(std::ceil(res_x), std::ceil(res_y));
}

#elif HEURISTIC_1_CAR == 3

int v2o(int v)
{
  return (v * (v-1)) / 2;
}

int o2v2oSteps(int x)
{
  return std::ceil(2 * std::sqrt(x));
}

int o2vv2oSteps(int x)
{
  return std::ceil(std::sqrt(1 + 4*x));
}

int vo2v2voSteps(int x, int v0)
{
  return std::ceil(2 * std::sqrt(x + v0*v0) - v0*v0);
}

int vo2vv2voSteps(int x, int v0)
{
  return std::ceil(std::sqrt(1 + 4*(x + v0*v0)) - 2*v0*v0);
}

int distanceWhileStoppingFrom(int v)
{
  return v2o(v);
}

int minStepsToGoThrough(int x)
{
  return std::min(o2v2oSteps(x), o2vv2oSteps(x));
}

int minStepsToGoThroughStartingAtVo(int x, int v0)
{
  return std::min(vo2v2voSteps(x, v0), vo2vv2voSteps(x, v0));
}

int calculateMinNumberOfSteps(int dist, int speedTowardsTarget)
{
  if (speedTowardsTarget < 0)
  {
    int speedAwayFromTarget = -speedTowardsTarget;
    return minStepsToGoThrough(distanceWhileStoppingFrom(speedAwayFromTarget) + dist) + speedAwayFromTarget;
  }
  else
  {
    int distanceWhileStoppingFromStartSpeed = distanceWhileStoppingFrom(speedTowardsTarget);
    if (distanceWhileStoppingFromStartSpeed > dist)
      return minStepsToGoThrough(distanceWhileStoppingFromStartSpeed - dist) + speedTowardsTarget;
    else
      return minStepsToGoThroughStartingAtVo(dist - distanceWhileStoppingFromStartSpeed, speedTowardsTarget) + speedTowardsTarget;
  }
}

int calculateMinNumberOfSteps(int start, int speed, int end)
{
  int dist = std::abs(start - end);
  int speedTowardsTarget = speed;
  if (end < start)
    speedTowardsTarget = -speedTowardsTarget;
  return calculateMinNumberOfSteps(dist, speedTowardsTarget);
}

template <>
int heuristic<State2V>(const State2V &st, const State2V &nd)
{
  int minStepsX = calculateMinNumberOfSteps(st.x, st.Vx, nd.x);
  int minStepsY = calculateMinNumberOfSteps(st.y, st.Vy, nd.y);
  return std::max(minStepsX, minStepsY);
}

#endif

std::ostream& operator<< (std::ostream &out, const State2V &state)
{
  // out << "position = " << state.x << ", " << state.y << "; "
  //   << "speed = " << state.Vx << ", " << state.Vy;
  out << state.x << " " << state.y << " " << state.Vx << " " << state.Vy;
  return out;
}
