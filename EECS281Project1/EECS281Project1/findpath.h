// 950181F63D0A883F183EC0A5CC67B19928FE896A
//  findpath.h
//  EECS281Project1
// Created by DIVYANGA on 13/09/21.

#ifndef findpath_h
#define findpath_h

#include <stdio.h>
#include <vector>
#include <deque>
#include <iostream>
#include <cassert>
#include <string>
#include <array>
#include <algorithm>
#include <stack>
#include <getopt.h>
#include "xcode_redirect.hpp"
using namespace std;


class findpath
{
   private:
    
    uint32_t no_levels;
    uint32_t no_rows;
    uint32_t no_cols;
    
    struct positionfeatures
    {
        char character = '.';
        // direction given by default is f
        char direction = 'f';
        bool discovery = false;
    };
    
    vector < vector < vector<positionfeatures> > > maze;
    
    struct coordinate
    {
        // level of the coordinate list
        uint32_t level;
        // row of the coordinate list
        uint32_t row;
        // column of the coordinate list
        uint32_t col;
    };
    
    deque<coordinate> search_container;
    deque<coordinate> list_ouput;
    coordinate start_position;
    coordinate hangar_positon;
    coordinate current_location;
    string outputmode;
    char inputmode;
    string routingtype = "";
    
    public:
    
    void get_options(int agrc, char **argv);
    void read_file();
    void search_path();
    bool solution_or_not();
    void backtrack_path();
    void print_no_solution();
    void print_with_solution();

};
#endif /* findpath_h */
