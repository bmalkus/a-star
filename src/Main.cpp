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

AStarOnBoardHelper<State> read_board(std::istream &&in)
{
  int w, h;
  short **fields;
  State start, end;

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
        start = State(i, j, 0);
      if (fields[i][j] == 3)
        end = State(i, j, 0);
    }
  }
  return AStarOnBoardHelper<State>(start, end, w, h, fields);
}

AStarOnBoardHelper<State2V> read_board_2V(std::istream &&in)
{
  int w, h;
  short **fields;
  State2V start, end;

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
        start = State2V(i, j, 0, 0);
      if (fields[i][j] == 3)
        end = State2V(i, j, 0, 0);
    }
  }
  return AStarOnBoardHelper<State2V>(start, end, w, h, fields);
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cerr << "Filename needed\n";
    return 1;
  }

  AStarOnBoardHelper<State2V> helper = read_board_2V(std::fstream(argv[1]));
  AStar<State2V, int, AStarOnBoardHelper<State2V>> astar;
  auto track = astar.solve(helper);
  std::cerr << "Track length: " << track.size() << "\n";
  for (auto s : track)
  {
    std::cerr << s.x << " " << s.y << "\n";
    if (helper.fields[s.x][s.y] == 0)
    helper.fields[s.x][s.y] = 4;
  }
  std::cout << helper.w << " " << helper.h << "\n";

  for (int j = 0; j < helper.h; ++j)
  {
    for (int i = 0; i < helper.w; ++i)
    {
      std::cout << helper.fields[i][j] << " ";
    }
    std::cout << "\n";
  }
}
