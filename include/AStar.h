#ifndef __ASTAR_H__
#define __ASTAR_H__

#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>

#include <vector>

template <
  class TState,
  typename TCost,
  class THelper
>
class AStar
{

public:
  std::vector<TState> solve(THelper &helper);

private:

  class StateCost
  {
  public:
    TState state;
    TCost fh, f;
    StateCost *prev;

    StateCost(const TState &state, TCost fh, TCost f, StateCost *prev):
      state(state), fh(fh), f(f), prev(prev) { }
  };

  class CostComparer
  {
  public:
    bool operator() (const StateCost *st, const StateCost *nd) const
    {
      return st->fh > nd->fh;
    }
  };

};

template <
  class TState,
  typename TCost,
  class THelper
>
std::vector<TState> AStar<TState, TCost, THelper>::solve(THelper &helper)
{
  std::set<TState> processed;
  std::vector<StateCost*> to_delete;
  std::priority_queue<StateCost*, std::vector<StateCost*>, CostComparer> to_process;

  TState start = helper.get_start();

  StateCost* curr(new StateCost(start, helper.heuristic(start), 0, nullptr));

  do
  {
    for (auto &state : helper.possible_states(curr->state))
      to_process.push(new StateCost(state, curr->f + helper.cost(curr->state, state) + helper.heuristic(state), curr->f + helper.cost(curr->state, state), curr));

    to_delete.push_back(curr);
    do
    {
      if (!to_process.empty())
      {
        curr = to_process.top();
        to_process.pop();
      }
      else
        break;
      if (processed.find(curr->state) == processed.end())
        break;
      else
        delete curr;
    } while (true);
  } while(!helper.is_end(curr->state));

  std::vector<TState> ret;
  do
  {
    ret.push_back(curr->state);
  } while ((curr = curr->prev));

  std::reverse(ret.begin(), ret.end());

  for (auto p : to_delete)
    delete p;

  return ret;
}
#endif
