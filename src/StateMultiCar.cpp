#include <iostream>

#include "StateMultiCar.h"
#include "AStarOnBoardHelper.h"

StateMultiCar::StateMultiCar()
{
  // empty
}

StateMultiCar::StateMultiCar(const std::vector<State2V> &cars):
  _cars(cars)
{
  // empty
}

bool StateMultiCar::operator< (const StateMultiCar &nd) const
{
  int cars_num = _cars.size();
  for (int i = 0; i < cars_num; ++i)
    if (!(_cars[i] == nd._cars[i]))
      return _cars[i] < nd._cars[i];
  return false;
}

bool StateMultiCar::operator== (const StateMultiCar &nd) const
{
  int cars_num = _cars.size();
  for (int i = 0; i < cars_num; ++i)
    if (!(_cars[i] == nd._cars[i]))
      return false;
  return true;
}

size_t StateMultiCar::Hasher::operator() (const StateMultiCar &state) const
{
  static State2V::Hasher s2V_hasher;
  size_t h = 0;
  int cars_num = state._cars.size();
  for (int i = 0; i < cars_num; ++i)
    h ^= s2V_hasher(state._cars[i]);
  return h;
}

std::ostream& operator<< (std::ostream &out, const StateMultiCar &state)
{
  int cars_num = state._cars.size();
  out << state._cars[0];
  for (int i = 1; i < cars_num; ++i)
    out << std::endl << state._cars[i];
  return out;
}

AStarMultiCarOnBoardHelper::AStarMultiCarOnBoardHelper(StateMultiCar start, State2V end, int w, int h, short **fields):
  _start(start), _end(end), fields(fields), w(w), h(h), heuristic_calls(0)
{
  // empty
}

StateMultiCar AStarMultiCarOnBoardHelper::get_start() const
{
  return _start;
}

bool AStarMultiCarOnBoardHelper::is_end(const StateMultiCar &state) const
{
  int cars_num = state._cars.size();
  for (int i = 0; i < cars_num; ++i)
  {
    const State2V &car = state._cars[i];
    if (car.x == -1 && car.y == -1)
      continue;
    if (!(car == _end))
      return false;
  }
  return true;
}

void AStarMultiCarOnBoardHelper::r_possible_states(
  const AStarOnBoardHelper<State2V> &s2V_helper,
  std::vector<StateMultiCar> &states,
  std::vector<State2V> &cars,
  size_t curr_car) const
{
  if (curr_car == cars.size())
    states.emplace_back(cars);
  else
  {
    if (has_ended(cars[curr_car]))
      r_possible_states(s2V_helper, states, cars, curr_car + 1);
    else
    {
      for (State2V &possible : ::possible_states(s2V_helper, cars[curr_car]))
      {
        bool collision = false;
        for (size_t i = 0; i < curr_car; ++i)
        {
          if (cars[i].x == possible.x && cars[i].y == possible.y)
          {
            collision = true;
            break;
          }
        }
        if (collision)
          continue;

        std::swap(possible, cars[curr_car]);
        r_possible_states(s2V_helper, states, cars, curr_car + 1);
        std::swap(possible, cars[curr_car]);
      }
    }
  }
}

std::vector<StateMultiCar> AStarMultiCarOnBoardHelper::possible_states(const StateMultiCar &state) const
{
  static AStarOnBoardHelper<State2V> s2V_helper(w, h, fields);

  std::vector<StateMultiCar> ret;
  std::vector<State2V> cars_copy(state._cars);

  r_possible_states(s2V_helper, ret, cars_copy, 0);

  return ret;
}

#define HEURISTIC 1

#if HEURISTIC == 1

int AStarMultiCarOnBoardHelper::heuristic(const StateMultiCar &state) const
{
  ++heuristic_calls;
  int res = 0;
  for (const State2V &car : state._cars)
    if (!has_ended(car))
      res = std::max(res, ::heuristic(car, _end));
  return res;
}

#elif HEURISTIC == 2

int AStarMultiCarOnBoardHelper::heuristic(const StateMultiCar &state) const
{
  ++heuristic_calls;
  int worst = 0;
  AStar<State2V, int, AStarOnBoardHelper<State2V>> astar;
  for (const State2V &car : state._cars)
  {
    if (!has_ended(car))
    {
      AStarOnBoardHelper<State2V> helper = AStarOnBoardHelper<State2V>(
        car, _end, w, h, fields);
      int res = astar.solve(helper).size();
      if (res)
        worst = std::max(worst, res);
    }
  }
  return worst;
}

#endif

#undef HEURISTIC

int AStarMultiCarOnBoardHelper::cost(const StateMultiCar &/*from*/, const StateMultiCar &/*to*/) const
{
  return 1;
}

void AStarMultiCarOnBoardHelper::iter_begin(StateMultiCar &state) const
{
  for (State2V &car : state._cars)
    if (car == _end)
      car = State2V(-1, -1, 0, 0);
}

bool AStarMultiCarOnBoardHelper::has_ended(const State2V &car) const
{
  return car.x == -1 && car.y == -1;
}

bool AStarMultiCarOnBoardHelper::is_legal_field(int x, int y) const
{
  bool on_board = x >= 0 && x < w && y >= 0 && y < h;
  if (!on_board)
    return false;
  return fields[x][y] != 1;
}
