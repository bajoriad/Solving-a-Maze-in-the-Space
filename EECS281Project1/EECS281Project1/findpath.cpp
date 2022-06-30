// 950181F63D0A883F183EC0A5CC67B19928FE896A
//  findpath.cpp
//  EECS281Project1
//
//  Created by DIVYANGA on 13/09/21.
//

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
#include "findpath.h"
#include "xcode_redirect.hpp"
using namespace std;


void findpath::get_options(int argc, char** argv)
{
    bool routingspecify = false;
    int option_index = 0;
    int choice = 0;
    opterr = false;
    option long_options[] = {
        {"stack", no_argument, nullptr, 's'},
        {"queue", no_argument, nullptr , 'q'},
        {"output", required_argument, nullptr, 'o'},
        {"help", no_argument, nullptr, 'h'},
        { nullptr, 0,                 nullptr, '\0' }
    };
    while ((choice = getopt_long(argc, argv, "sqo:h", long_options, &option_index)) != -1)
    {
        switch (choice)
        {
             case 's':
                 if (routingspecify == true)
                 {
                     cerr << "Multiple routing modes specified\n" ;
                     exit(1);
                 }
                 routingtype = "s";
                 routingspecify = true;
                 break;
                 
             case 'q':
                if (routingspecify == true)
                {
                    cerr << "Multiple routing modes specified\n" ;
                    exit(1);
                }
                 routingtype = "q";
                 routingspecify = true;
                 break;
                 
             case 'o':
                 outputmode = optarg;
                 break;
            
             case 'h':
                std::cout << "This program reads the coordinates of a maze from an input\n"
                          << "file and finds a path to reach from the starting point to\n"
                          << "to the hangar in the maze. There may be no solution as well.\n"
                          <<  "Usage: \'./project1\n\t[--stack | -s]\n"
                          <<                      "\t[--queue | -q]\n"
                          <<                      "\t[--output | -o]\n"
                          <<                      "\t[--help | -h]\n";
                exit(0);
                
          }
    }
    
    if(outputmode == "")
    {
        outputmode = "M";
    }
    
    if(routingspecify == false)
    {
        cerr << "No routing mode specified\n" ;
        exit(1);
    }
}


void findpath::read_file()
{
    // eachline for reaching every line of the output
    string eachline = "";
    string copy = "";
    string line_take = "";
    cin >> inputmode;
    cin >> no_levels;
    cin >> no_rows;
    no_cols = no_rows;
    maze.resize(no_levels, vector < vector<positionfeatures> >(no_rows, vector<positionfeatures>(no_cols)));
    if (inputmode == 'M')
    {
        getline(cin,copy);
        uint32_t row_f = 0;
        uint32_t level = 0;
        getline(cin,eachline);
        while(!eachline.empty())
        {
            if(eachline[0] != '/')
               {
                   for (uint32_t i = 0; i < no_cols; i++)
                   {
                       if (eachline[i] != '.' && eachline[i] != '#' && eachline[i] != 'S' && eachline[i] != 'H' && eachline[i] != 'E')
                       {
                           cerr << "Invalid map character\n" ;
                           exit(1);
                       }
                       maze[level][row_f][i].character = eachline[i];
                       if (eachline[i] == 'S')
                       {
                           start_position.level = level;
                           start_position.col = i;
                           start_position.row = row_f;
                       }
                       if (eachline[i] == 'H')
                       {
                           hangar_positon.level = level;
                           hangar_positon.col = i;
                           hangar_positon.row = row_f;
                       }
                   }
                   ++row_f;
                   if (row_f == no_rows)
                   {
                       row_f = 0;
                       ++level;
                   }
                   getline(cin, eachline);
               }
              else
              {
                  row_f = row_f + 0;
                  getline(cin, eachline);
              }
        }
    }
    else
    {
        char a,b,c,d,e;
        char character_input;
        uint32_t row_list, col_list, level_list;
        while(cin >> a)
        {
            if (a == '(')
            {
                cin >> level_list >> b >> row_list >> c >> col_list >> d >>
                character_input >> e;
                if (character_input != '.' && character_input != '#' && character_input != 'S' && character_input != 'H' && character_input != 'E')
                {
                    cerr << "Invalid map character\n" ;
                    exit(1);
                }

                if (level_list >= no_levels)
                {
                    cerr << "Invalid map level\n";
                    exit(1);
                }
                if (row_list >= no_rows)
                {
                    cerr << "Invalid map row\n";
                    exit(1);
                }
                if (col_list >= no_cols)
                {
                    cerr << "Invalid map column\n";
                    exit(1);
                }
                maze[level_list][row_list][col_list].character = character_input;
                if (character_input == 'S')
                {
                    start_position.level = level_list;
                    start_position.col = col_list;
                    start_position.row = row_list;
                }
                if (character_input == 'H')
                {
                    hangar_positon.level = level_list;
                    hangar_positon.col = col_list;
                    hangar_positon.row = row_list;
                }
            }
            else if (a == '/')
            {
                getline(cin, line_take);
            }
        }
    }
}


void findpath::search_path()
{
    coordinate location_elevator;
    coordinate location_north;
    coordinate location_south;
    coordinate location_west;
    coordinate location_east;
    search_container.push_back(start_position);
    current_location = start_position;
    maze[start_position.level][start_position.row][start_position.col].discovery = true;
    while (!search_container.empty())
        {
            if (maze[hangar_positon.level][hangar_positon.row][hangar_positon.col].discovery == true)
            {
                return;
            }
            if (routingtype == "q")
             {
                   current_location = search_container.front();
                   search_container.pop_front();
             }
             else
             {
                   current_location = search_container.back();
                   search_container.pop_back();
              
              }
            
            if (current_location.row != 0)
            {
                if(maze[current_location.level][current_location.row - 1][current_location.col].character != '#' && maze[current_location.level][current_location.row - 1][current_location.col].discovery == false)
                {
                    maze[current_location.level][current_location.row - 1][current_location.col].discovery = true;
                    maze[current_location.level][current_location.row - 1][current_location.col].direction = 'n';
                    location_north.level = current_location.level;
                    location_north.row = current_location.row - 1;
                    location_north.col = current_location.col;
                    search_container.push_back(location_north);
                }
            }
            
            if(current_location.col != (no_cols - 1))
            {
                if (maze[hangar_positon.level][hangar_positon.row][hangar_positon.col].discovery == true)
                {
                    return;
                }
                if(maze[current_location.level][current_location.row][current_location.col + 1].character != '#' && maze[current_location.level][current_location.row][current_location.col + 1].discovery == false)
                {
                    maze[current_location.level][current_location.row][current_location.col + 1].discovery = true;
                    maze[current_location.level][current_location.row][current_location.col + 1].direction = 'e';
                    location_east.level = current_location.level;
                    location_east.row = current_location.row;
                    location_east.col = current_location.col + 1;
                    search_container.push_back(location_east);
                }
            }
            
            if(current_location.row != (no_rows - 1))
            {
                if (maze[hangar_positon.level][hangar_positon.row][hangar_positon.col].discovery == true)
                {
                    return;
                }
                if(maze[current_location.level][current_location.row + 1][current_location.col].character != '#' && maze[current_location.level][current_location.row + 1][current_location.col].discovery == false)
                {
                    maze[current_location.level][current_location.row + 1][current_location.col].discovery = true;
                    maze[current_location.level][current_location.row + 1][current_location.col].direction = 's';
                    location_south.level = current_location.level;
                    location_south.row = current_location.row + 1;
                    location_south.col = current_location.col;
                    search_container.push_back(location_south);
                }
            }
            
            if(current_location.col != 0)
            {
                if (maze[hangar_positon.level][hangar_positon.row][hangar_positon.col].discovery == true)
                {
                    return;
                }
                if(maze[current_location.level][current_location.row][current_location.col - 1].character != '#' && maze[current_location.level][current_location.row][current_location.col - 1].discovery == false)
                {
                    maze[current_location.level][current_location.row][current_location.col - 1].discovery = true;
                    maze[current_location.level][current_location.row][current_location.col - 1].direction = 'w';
                    location_west.level = current_location.level;
                    location_west.row = current_location.row;
                    location_west.col = current_location.col - 1;
                    search_container.push_back(location_west);
                }
            }
            
            if(maze[current_location.level][current_location.row][current_location.col].character == 'E')
               {
                   if (maze[hangar_positon.level][hangar_positon.row][hangar_positon.col].discovery == true)
                   {
                       return;
                   }
                   for (uint32_t i = 0; i < no_levels; i++)
                   {
                       if (maze[i][current_location.row][current_location.col].discovery == false && maze[i][current_location.row][current_location.col].character == 'E')
                       {
                           maze[i][current_location.row][current_location.col].discovery = true;
                           maze[i][current_location.row][current_location.col].direction = static_cast<char>(current_location.level + '0');
                           location_elevator.level = i;
                           location_elevator.row = current_location.row;
                           location_elevator.col = current_location.col;
                           search_container.push_back(location_elevator);
                       }
                   }
                   
                }
         }
    }

bool findpath::solution_or_not()
{
    if(search_container.empty())
    {
        return true;
    }
    else
    {
        return false;
    }
}
    
void findpath::backtrack_path()
{
    coordinate path_tracker = hangar_positon;
    coordinate previous_position = path_tracker;
    
    while(maze[path_tracker.level][path_tracker.row][path_tracker.col].direction != 'f')
    {
        if (maze[path_tracker.level][path_tracker.row][path_tracker.col].direction == 's')
        {
            maze[path_tracker.level][path_tracker.row - 1][path_tracker.col].character = 's';
            previous_position = path_tracker;
            path_tracker.row = path_tracker.row - 1;
            list_ouput.push_front(path_tracker);
        }
        else if (maze[path_tracker.level][path_tracker.row][path_tracker.col].direction == 'e')
        {
            maze[path_tracker.level][path_tracker.row][path_tracker.col - 1].character = 'e';
            previous_position = path_tracker;
            path_tracker.col = path_tracker.col - 1;
            list_ouput.push_front(path_tracker);
        }
        else if (maze[path_tracker.level][path_tracker.row][path_tracker.col].direction == 'w')
        {
            maze[path_tracker.level][path_tracker.row][path_tracker.col + 1].character = 'w';
            previous_position = path_tracker;
            path_tracker.col = path_tracker.col + 1;
            list_ouput.push_front(path_tracker);
        }
        else if (maze[path_tracker.level][path_tracker.row][path_tracker.col].direction == 'n')
        {
            maze[path_tracker.level][path_tracker.row + 1][path_tracker.col].character = 'n';
            previous_position = path_tracker;
            path_tracker.row = path_tracker.row + 1;
            list_ouput.push_front(path_tracker);
        }
        else
        {
            uint32_t elevator_level_direction = static_cast<uint32_t>(maze[path_tracker.level][path_tracker.row][path_tracker.col].direction - '0');
            maze[elevator_level_direction][path_tracker.row][path_tracker.col].character = static_cast<char>(path_tracker.level + '0');
            previous_position = path_tracker;
            path_tracker.level = elevator_level_direction;
            list_ouput.push_front(path_tracker);
        }
    }
    maze[path_tracker.level][path_tracker.row][path_tracker.col].character = maze[previous_position.level][previous_position.row][previous_position.col].direction;
    list_ouput.push_front(path_tracker);
}

void findpath::print_no_solution()
{
    if (outputmode == "M")
    {
        cout << "Start in level " << start_position.level << ", row " << start_position.row << ", column " << start_position.col << '\n';
        for (uint32_t i = 0; i < no_levels; ++i)
        {
            cout << "//level " << i << '\n';
            for (uint32_t j = 0; j < no_rows; ++j)
            {
                for (uint32_t k = 0; k < no_cols; ++k)
                {
                    cout << maze[i][j][k].character;
                }
                cout << '\n';
             }
        }
    }
    else
    {
        cout << "//path taken\n";
    }
}

void findpath::print_with_solution()
{
    if (outputmode == "M")
    {
        cout << "Start in level " << start_position.level << ", row " << start_position.row << ", column " << start_position.col << '\n';
        for (uint32_t i = 0; i < no_levels; ++i)
        {
            cout << "//level " << i << '\n';
            for (uint32_t j = 0; j < no_rows; ++j)
            {
                for (uint32_t k = 0; k < no_cols; ++k)
                {
                    cout << maze[i][j][k].character;
                }
                cout << '\n';
             }
        }
    }
    else
    {
        cout << "//path taken\n";
        for (uint32_t i = 1; i < list_ouput.size(); ++i)
        {
            cout << "(" << list_ouput[i].level << "," << list_ouput[i].row << "," << list_ouput[i].col << "," << maze[list_ouput[i].level][list_ouput[i].row][list_ouput[i].col].character << ")\n";
        }
    }
}

