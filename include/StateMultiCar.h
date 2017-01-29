#ifndef STATEMULTICAR_H
#define STATEMULTICAR_H

#include <vector>

#include "State2V.h"
#include "AStarOnBoardHelper.h"

struct StateMultiCar
{
  std::vector<State2V> _cars;

  StateMultiCar();
  StateMultiCar(const std::vector<State2V> &cars);

  bool operator< (const StateMultiCar &nd) const;
  bool operator== (const StateMultiCar &nd) const;

  friend std::ostream& operator<< (std::ostream &out, const StateMultiCar &state);

  struct Hasher
  {
    size_t operator() (const StateMultiCar &s) const;
  };
};

class AStarMultiCarOnBoardHelper
{
private:
  StateMultiCar _start;
  State2V _end;

public:
  short **fields;
  int w, h;
  mutable int heuristic_calls;

  AStarMultiCarOnBoardHelper(StateMultiCar start, State2V end, int w, int h, short **fields);

  StateMultiCar get_start() const;

  bool is_end(const StateMultiCar &state) const;

  std::vector<StateMultiCar> possible_states(const StateMultiCar &state) const;

  int heuristic(const StateMultiCar &state) const;

  int cost(const StateMultiCar &from, const StateMultiCar &to) const;

  void iter_begin(StateMultiCar &state) const;

private:
  bool has_ended(const State2V &car) const;
  bool is_legal_field(int x, int y) const;

  void r_possible_states(const AStarOnBoardHelper<State2V> &s2V_helper,
                         std::vector<StateMultiCar> &states,
                         std::vector<State2V> &cars,
                         size_t curr_car) const;

  friend std::vector<StateMultiCar> possible_states(const AStarMultiCarOnBoardHelper &helper, const StateMultiCar& state);
};

#endif /* STATEMULTICAR_H */
