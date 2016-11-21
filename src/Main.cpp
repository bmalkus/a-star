#include <iostream>
#include <fstream>
#include <algorithm>
#include <set>
#include <vector>
#include <cmath>
#include <queue>
#include <tuple>

#include "AStarHelper.h"
#include "AStar.h"

AStarHelper read_board(std::istream &&in)
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
  return AStarHelper(start, end, w, h, fields);
}

int main()
{
  AStarHelper helper = read_board(std::fstream("../../../a-star-tests/text_in/test03.in"));
  AStar<State, int, AStarHelper> astar;
  auto track = astar.solve(helper);
  std::cout << "Track length: " << track.size() << "\n";
  for (auto s : track)
  {
    std::cout << s.x << " " << s.y << "\n";
  }
}
