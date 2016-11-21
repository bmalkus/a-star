#ifndef __UTILS_H__
#define __UTILS_H__

#include <vector>

#include "AStarOnBoardHelper.h"

template <class TState>
std::vector<TState> possible_states(const AStarOnBoardHelper<TState> &helper, const TState& state);

template <class TState>
int heuristic(const TState &st, const TState &nd);

#endif
