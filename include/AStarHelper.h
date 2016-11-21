#ifndef __ASTAR_HELPER_H__
#define __ASTAR_HELPER_H__

#include <vector>

#include "State.h"

class AStarHelper
{
public:
  AStarHelper(State start, State end, int w, int h, short **fields);

  State get_start();

  bool is_end(const State &state);

  std::vector<State> possible_states(const State &state);

  int heuristic(const State &state);

  int cost(const State &from, const State &to);

private:
  State start, end;
  short **fields;
  int w, h;

  bool is_legal_field(int x, int y);
};

#endif
