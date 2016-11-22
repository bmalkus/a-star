#include <iostream>
#include <fstream>
#include <algorithm>
#include <set>
#include <vector>
#include <cmath>
#include <queue>
#include <tuple>

#include "AStarOnBoardHelper.h"
#include "AStar.h"
#include "State.h"
#include "State2V.h"

template <class TState> // {{{ create_state
TState create_state(int x, int y);

template <>
State create_state<State>(int x, int y)
{
  return State(x, y, 0);
}

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

using S = State2V;

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cerr << "Filename needed\n";
    return 1;
  }

  bool fancy = false;

  if (argc > 2)
    fancy = true;

  AStarOnBoardHelper<S> helper = read_board<S>(std::fstream(argv[1]));
  AStar<S, int, AStarOnBoardHelper<S>> astar;
  auto track = astar.solve(helper);
  std::cerr << "Track length: " << track.size() << "\n";
  for (auto s : track)
  {
    std::cerr << s.x << " " << s.y << "\n";
    if (helper.fields[s.x][s.y] == 0)
      helper.fields[s.x][s.y] = 4;
  }
  write_board_to_stdout(helper, fancy);
}
