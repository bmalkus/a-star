One of academic projects.

The goal was to provide generic A\* algorithm (`include/AStar.h`) to which
Cost, State and Helper classes are provided.  Algorithm was tested on following
case: car(s) which speed up and slow down in particular (limited) manner are
finding paths from start to finish point on 2D board, avoiding obstacles.
Multiple heuristics for calculating distance were used.

I have also implemented optional Hash template parameter to AStar class, which,
when provided, will cause `std::unordered_set` to be used instead of regular
`std::set`. If Hash class is properly implemented, it may greatly speed up
checking current state against set of already visited.
