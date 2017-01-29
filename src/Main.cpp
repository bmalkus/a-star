#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <set>
#include <vector>
#include <cmath>
#include <queue>
#include <tuple>
#include <chrono>

#include "AStarOnBoardHelper.h"
#include "AStar.h"
#include "State2V.h"
#include "StateMultiCar.h"

template <class TState> // {{{ create_state
TState create_state(int x, int y);

template <>
State2V create_state<State2V>(int x, int y)
{
  return State2V(x, y, 0, 0);
}
// }}}

template <class TState> // {{{ read_board
AStarOnBoardHelper<TState> read_board(std::istream &&in)
{
  int w, h;
  short **fields;
  TState start, end;

  in >> w >> h;

  fields = new short*[w];
  for (int i = 0; i < w; ++i)
    fields[i] = new short[h];

  for (int j = 0; j < h; ++j)
  {
    for (int i = 0; i < w; ++i)
    {
      in >> fields[i][j];
      if (fields[i][j] == 2)
        start = create_state<TState>(i, j);
      if (fields[i][j] == 3)
        end = create_state<TState>(i, j);
    }
  }
  return AStarOnBoardHelper<TState>(start, end, w, h, fields);
}

AStarMultiCarOnBoardHelper read_board(std::istream &&in)
{
  int w, h;
  short **fields;
  StateMultiCar start;
  State2V end;

  in >> w >> h;

  fields = new short*[w];
  for (int i = 0; i < w; ++i)
    fields[i] = new short[h];

  std::vector<State2V> cars;

  for (int j = 0; j < h; ++j)
  {
    for (int i = 0; i < w; ++i)
    {
      in >> fields[i][j];
      if (fields[i][j] == 2)
        cars.emplace_back(create_state<State2V>(i, j));
      if (fields[i][j] == 3)
        end = create_state<State2V>(i, j);
    }
  }

  return AStarMultiCarOnBoardHelper(StateMultiCar(std::move(cars)),
                                    end, w, h, fields);
}
// }}}

const char char_map[] = ".@BEX";

template <class TState> // {{{ write_board_to_stdout
void write_board_to_stdout(AStarOnBoardHelper<TState> &helper, bool fancy = false)
{
  std::cout << helper.w << " " << helper.h << "\n";

  for (int j = 0; j < helper.h; ++j)
  {
    for (int i = 0; i < helper.w; ++i)
    {
      if (fancy)
        std::cout << char_map[helper.fields[i][j]] << " ";
      else
        std::cout << helper.fields[i][j] << " ";
    }
    std::cout << "\n";
  }
}
// }}}

template <class TState> // {{{ print_solution
void print_solution(std::string fname,
                    double time,
                    std::vector<TState> solution,
                    AStarMultiCarOnBoardHelper &helper)
{
  std::cerr << "==============================================\n"
    << "File name: " << fname.substr(fname.rfind('/') + 1) << "\n"
    << "Execution time: " << std::fixed << std::setprecision(6) << time << " s\n"
    << "Heuristic function calls: " << helper.heuristic_calls << "\n"
    << "Number of steps: " << solution.size() - 1 << "\n"
    << "Number of cars: " << solution[0]._cars.size() << "\n";
  std::cerr << "Solution:\n";
  std::cout << solution.size() << " " << solution[0]._cars.size() << "\n";
  for (auto s : solution)
    std::cout << s << "\n";
  std::cerr << "==============================================\n";
}
// }}}

using std::chrono::steady_clock;
using std::chrono::duration;
using std::chrono::duration_cast;

using S = StateMultiCar;
using Helper = AStarMultiCarOnBoardHelper;

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cerr << "Filename needed\n";
    return 1;
  }

  Helper helper = read_board(std::fstream(argv[1]));
  AStar<S, int, Helper> astar;

  auto t1 = steady_clock::now();

  auto track = astar.solve(helper);

  auto d = duration_cast<duration<double>>(steady_clock::now() - t1);

  print_solution(argv[1], d.count(), track, helper);

  for (int i = 0; i < helper.w; ++i)
    delete[] helper.fields[i];
  delete[] helper.fields;
}
