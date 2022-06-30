//950181F63D0A883F183EC0A5CC67B19928FE896A
//  main.cpp
//  EECS281Project1
// Created by DIVYANGA on 13/09/21.
//

#include <cassert>
#include <iostream>
#include <string>
#include <array>
#include <algorithm>
#include <vector>
#include <deque>
#include <getopt.h>
#include <stack>
#include "xcode_redirect.hpp"
#include "findpath.h"
using namespace std;

int main(int argc, char *argv[])
{
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);
    bool solution_analysis;
    findpath maze_solver;
    maze_solver.get_options(argc, argv);
    maze_solver.read_file();
    maze_solver.search_path();
    solution_analysis = maze_solver.solution_or_not();
    if (solution_analysis == true)
    {
        maze_solver.print_no_solution();
    }
    else
    {
        maze_solver.backtrack_path();
        maze_solver.print_with_solution();
    }
    return 0;
}
