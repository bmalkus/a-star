#include <iostream>
#include <algorithm>
#include <set>
#include <vector>
#include <cmath>
#include <queue>
#include <tuple>

struct State;

using vec_state = std::vector<State>;

struct State
{
  int x, y, v;

  State(int x, int y, int v);

  bool operator< (const State &nd) const;
  bool operator!= (const State &nd) const { return x != nd.x || y != nd.y; }
};

struct StateCost
{
  State state;
  int fh, f;

  StateCost(const State &state, int fh, int f): state(state), fh(fh), f(f) { }
};

struct CostComparer
{
  bool operator() (const StateCost &st, const StateCost &nd) const
  {
    return st.fh > nd.fh;
  }
};

int h(const State &curr, const State &end);

vec_state possible_states(const State &curr)
{
  vec_state ret;

  int faster = curr.v + 1;
  for (int i = -faster; i <= faster; i += faster)
  {
    for (int j = -faster; j <= faster; j += faster)
    {
      if (i == 0 && j == 0)
        continue;
      ret.push_back(State(curr.x + i, curr.y + j, faster));
    }
  }

  if (curr.v > 1)
  {
    int slower = curr.v - 1;
    for (int i = -slower; i <= slower; i += slower)
    {
      for (int j = -slower; j <= slower; j += slower)
      {
        if (i == 0 && j == 0)
          continue;
        ret.push_back(State(curr.x + i, curr.y + j, slower));
      }
    }
  }
  else if (curr.v == 1)
  {
    ret.push_back(State(curr.x, curr.y, 0));
  }

  return ret;
}

void astar(const State &start, const State &end)
{
  std::set<State> processed;
  std::priority_queue<StateCost, std::vector<StateCost>, CostComparer> to_process;

  StateCost curr(start, h(start, end), 0);

  // std::cout << curr.state.x << " " << curr.state.y << " " << curr.fh << " " << curr.f << "\n";
  do
  {
    for (auto &state : possible_states(curr.state))
      to_process.push(StateCost(state, curr.f + 1 + h(state, end), curr.f + 1));

    do
    {
      curr = to_process.top();
      to_process.pop();
    } while (processed.find(curr.state) != processed.end());
    // std::cout << curr.state.x << " " << curr.state.y << " " << curr.fh << " " << curr.f << "\n";

  } while(curr.state != end);
  std::cout << "Total cost: " << curr.f << "\n";
}

int main()
{
  State start(0, 0, 0);
  State end(367, 456, 0);
  astar(start, end);
}

int h(const State &curr, const State &end)
{
  int max = std::max(std::abs(curr.x - end.x), std::abs(curr.y - end.y));
  return std::ceil((-2*curr.v - 1 + sqrt(4*curr.v*curr.v + 4*curr.v + 1 + 8*max)) / 2);
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
