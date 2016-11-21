#ifndef __ASTAR_HELPER_H__
#define __ASTAR_HELPER_H__

#include <vector>
#include <algorithm>
#include <cmath>

#include "State.h"

template <class TState>
class AStarOnBoardHelper;

template <class TState>
std::vector<TState> possible_states(const AStarOnBoardHelper<TState> &helper, const TState& state);

template <class TState>
int heuristic(const TState &st, const TState &nd);

template <class TState>
class AStarOnBoardHelper
{
public:
  AStarOnBoardHelper(TState start, TState end, int w, int h, short **fields);

  TState get_start() const;

  bool is_end(const TState &state) const;

  std::vector<TState> possible_states(const TState &state) const;

  int heuristic(const TState &state) const;

  int cost(const TState &from, const TState &to) const;

private:
  TState start, end;
  short **fields;
  int w, h;

  bool is_legal_field(int x, int y) const;

  template <class T>
  friend std::vector<T> possible_states(const AStarOnBoardHelper<T> &helper, const T& state);
};

template <class TState>
AStarOnBoardHelper<TState>::AStarOnBoardHelper(TState start, TState end, int w, int h, short **fields):
  start(start), end(end), fields(fields), w(w), h(h)
{
  // empty
}

template <class TState>
TState AStarOnBoardHelper<TState>::get_start() const
{
  return start;
}

template <class TState>
bool AStarOnBoardHelper<TState>::is_end(const TState &state) const
{
  return state == end;
}

template <class TState>
std::vector<TState> AStarOnBoardHelper<TState>::possible_states(const TState &state) const
{
  return ::possible_states<TState>(*this, state);
}

template <class TState>
int AStarOnBoardHelper<TState>::heuristic(const TState &state) const
{
  return ::heuristic<TState>(state, end);
}

template <class TState>
int AStarOnBoardHelper<TState>::cost(const TState &from, const TState &to) const
{
  return 1;
}

template <class TState>
bool AStarOnBoardHelper<TState>::is_legal_field(int x, int y) const
{
  bool on_board = x >= 0 && x < w && y >= 0 && y < h;
  bool not_border = fields[x][y] != 1;
  return on_board && not_border;
}

#endif
