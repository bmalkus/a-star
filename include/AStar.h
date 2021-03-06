#ifndef __ASTAR_H__
#define __ASTAR_H__

#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <queue>
#include <algorithm>

#ifdef PARALLEL_HEURISTIC
#include <omp.h>
#endif

namespace AStarTypeSelector
{

template<class Hash, class TState>
struct Select
{
  typedef std::unordered_set<TState, Hash> Container;
};

struct DummyHash;

template<class TState>
struct Select<DummyHash, TState>
{
  typedef std::set<TState> Container;
};

}

template <
  class TState,
  typename TCost,
  class THelper,
  class Hash = AStarTypeSelector::DummyHash
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
  class THelper,
  class Hash
>
std::vector<TState> AStar<TState, TCost, THelper, Hash>::solve(THelper &helper)
{
  typename AStarTypeSelector::Select<Hash, TState>::Container processed;
  std::vector<StateCost*> to_delete;
  std::priority_queue<StateCost*, std::vector<StateCost*>, CostComparer> to_process;

  TState start = helper.get_start();

  StateCost* curr(new StateCost(start, helper.heuristic(start), 0, nullptr));

  do
  {

#ifndef PARALLEL_HEURISTIC

    helper.iter_begin(curr->state);
    for (auto &state : helper.possible_states(curr->state))
    {
      TCost f = curr->f + helper.cost(curr->state, state);
      to_process.push(new StateCost(state, f + helper.heuristic(state), f, curr));
    }

#else

    helper.iter_begin(curr->state);
    auto possible = helper.possible_states(curr->state);
    int states_num = possible.size();
    std::vector<int> heuristics;
    heuristics.resize(states_num);

#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < states_num; ++i)
    {
      heuristics[i] = helper.heuristic(possible[i]);
    }

    for (int i = 0; i < states_num; ++i)
    {
      TCost f = curr->f + helper.cost(curr->state, possible[i]);
      to_process.push(new StateCost(possible[i], f + heuristics[i], f, curr));
    }

#endif

    processed.insert(curr->state);
    to_delete.push_back(curr);
    do
    {
      if (!to_process.empty())
      {
        curr = to_process.top();
        to_process.pop();
      }
      else
        return std::vector<TState>();

      if (processed.find(curr->state) == processed.end())
        break;
      else
        delete curr;
    } while (true);
  } while(!helper.is_end(curr->state));

  auto curr_orig = curr;

  std::vector<TState> ret;
  do
  {
    ret.push_back(curr->state);
  } while ((curr = curr->prev));

  std::reverse(ret.begin(), ret.end());

  delete curr_orig;
  while (!to_process.empty())
  {
    delete to_process.top();
    to_process.pop();
  }
  for (auto p : to_delete)
    delete p;

  return ret;
}
#endif
