#include <algorithm>
#include <cmath>

#include "AStarHelper.h"
#include "State.h"

AStarHelper::AStarHelper(State start, State end, int w, int h, short **fields):
  start(start), end(end), fields(fields), w(w), h(h)
{
  // empty
}

State AStarHelper::get_start()
{
  return start;
}

bool AStarHelper::is_end(const State &state)
{
  return state == end;
}

std::vector<State> AStarHelper::possible_states(const State &state)
{
  std::vector<State> ret;
  int faster = state.v + 1;
  for (int i : { 0, -faster, faster })
  {
    for (int j : { 0, -faster, faster })
    {
      if (i == 0 && j == 0 )
        continue;
      int x = state.x + i;
      int y = state.y + j;
      if (is_legal_field(x, y))
      {
        ret.push_back(State(x, y, faster));
      }
    }
  }

  if (state.v > 1)
  {
    int slower = state.v - 1;
    for (int i : { 0, -slower, slower })
    {
      for (int j : { 0, -slower, slower })
      {
        if (i == 0 && j == 0)
          continue;
        int x = state.x + i;
        int y = state.y + j;
        if (is_legal_field(x, y))
          ret.push_back(State(x, y, slower));
      }
    }
  }
  else if (state.v == 1)
  {
    ret.push_back(State(state.x, state.y, 0));
  }
  return ret;
}

int AStarHelper::heuristic(const State &state)
{
  int max = std::max(std::abs(state.x - end.x), std::abs(state.y - end.y));
  return std::ceil((-2*state.v - 1 + sqrt(4*state.v*state.v + 4*state.v + 1 + 8*max)) / 2);
}

int AStarHelper::cost(const State &from, const State &to)
{
  return 1;
}

bool AStarHelper::is_legal_field(int x, int y)
{
  bool on_board = x >= 0 && x < w && y >= 0 && y < h;
  bool not_border = fields[x][y] != 1;
  return on_board && not_border;
}
