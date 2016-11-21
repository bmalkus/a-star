#include "Utils.h"

template <>
std::vector<State> possible_states<State>(const AStarOnBoardHelper<State> &helper, const State &state)
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
      if (helper.is_legal_field(x, y))
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
        if (helper.is_legal_field(x, y))
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


template <>
int heuristic<State>(const State &st, const State &nd)
{
  int max = std::max(std::abs(st.x - nd.x), std::abs(st.y - nd.y));
  return std::ceil((-2*st.v - 1 + sqrt(4*st.v*st.v + 4*st.v + 1 + 8*max)) / 2);
}
