#ifndef __ASTAR_HELPER_H__
#define __ASTAR_HELPER_H__

#include <vector>
#include <algorithm>
#include <cmath>

template <class TState>
class AStarOnBoardHelper;

template <class TState>
std::vector<TState> possible_states(const AStarOnBoardHelper<TState> &helper, const TState& state);

template <class TState>
int heuristic(const TState &st, const TState &nd);

template <class TState>
class AStarOnBoardHelper
{
private:
  TState start, end;

public:
  short **fields;
  int w, h;
  mutable int heuristic_calls;

  AStarOnBoardHelper(TState start, TState end, int w, int h, short **fields);
  AStarOnBoardHelper(int w, int h, short **fields);

  TState get_start() const;

  bool is_end(const TState &state) const;

  std::vector<TState> possible_states(const TState &state) const;

  int heuristic(const TState &state) const;

  int cost(const TState &from, const TState &to) const;

  void iter_begin(TState &/*state*/) const {}

private:
  bool is_legal_field(int x, int y) const;

  template <class T>
  friend std::vector<T> possible_states(const AStarOnBoardHelper<T> &helper, const T& state);
};

template <class TState>
AStarOnBoardHelper<TState>::AStarOnBoardHelper(TState start, TState end, int w, int h, short **fields):
  start(start), end(end), fields(fields), w(w), h(h), heuristic_calls(0)
{
  // empty
}

template <class TState>
AStarOnBoardHelper<TState>::AStarOnBoardHelper(int w, int h, short **fields):
  fields(fields), w(w), h(h), heuristic_calls(0)
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
  ++heuristic_calls;
  return ::heuristic<TState>(state, end);
}

template <class TState>
int AStarOnBoardHelper<TState>::cost(const TState &/*from*/, const TState &/*to*/) const
{
  return 1;
}

template <class TState>
bool AStarOnBoardHelper<TState>::is_legal_field(int x, int y) const
{
  bool on_board = x >= 0 && x < w && y >= 0 && y < h;
  if (!on_board)
    return false;
  return fields[x][y] != 1;
}

#endif
