//  _      ___ _____   ____
// | | /| / (_) __(_) / __/__ ____ *
// | |/ |/ / / _// / _\ \/ -_) __/
// |__/|__/_/_/ /_/ /___/\__/\__/
//
// WiFiSec ////// originally created by Alex Haley 2024 (C)
// version: 1.1rc
//
// desc:
//      this is a little cli game where you create room and place routers in it,
//      and then you calculate the power of routers signal.
//
// this app is destributed under GPL License
//
// You need to have wifisec.hpp file in the same library or this program will not work!!!
//

// standart libraries + wifisec library containing function's definisions
#include <algorithm>
#include <cmath>
#include <array>
#include <string>
#include <iostream>
#include <cstdlib>
#include <map>
#include <termios.h>
#include "wifisec.hpp"

// define input keys
#define W_KEY 119
#define A_KEY 97
#define S_KEY 115
#define D_KEY 100
#define ESC_KEY 27
#define ENTER_KEY 13

static struct termios stored_settings;

int m = 17;
int l = 80;

// define room
char cMap[17][80] = {
    "############################################################  WiFiSec          ",
    "#..........................................................#  your coordinates:",
    "#..........................................................#  x: 0",
    "#..........................................................#  y: 0",
    "#..........................................................#",
    "#..........................................................#",
    "#..........................................................#",
    "#..........................................................#",
    "#..........................................................#",
    "#..........................................................#",
    "#..........................................................#",
    "#..........................................................#",
    "#..........................................................#",
    "#..........................................................#",
    "#..........................................................#",
    "############################################################  (C) 2024"
};

// basic value
int x = 1;
int y = 1;
// counting values in string names[10];
int setval = 0;

// creating dictionary to store axis of routers
std::array<int, 2> arr;
std::map<std::string, std::string> dict;
// string array to store routers names
std::string names[10];

int main(void)
{
    int ch;
    while (ch != 4)
    {
        metalrender();

        std::cout << "menu\n";
        std::cout << "1. place router\n";
        std::cout << "2. delete router\n";
        std::cout << "3. clear all\n";
        std::cout << "4. exit\n";
        std::cin >> ch;

        if (ch == 1)
        {
            movement();
        }
        else if (ch == 2)
        {
            std::cout << "placed routers:\n";
            for (auto it : dict)
                std::cout << it.first << ": " << it.second << "\n";
            std::string rnam;
            std::cout << "which router you want to delete?\n(type r to return to the menu) ";
            std::cin >> rnam;
            if (rnam == "r")
            {
              std::cout << "no input provided. aborting\n";
            }
            else {
              delete_router(rnam);
            }
        }
        else if (ch == 3)
        {
            for (int i = 1; i < 15; i++)
            {
                for (int j = 1; j < 59; j++)
                {
                    cMap[i][j] = '.';
                }
            }
            dict.clear();
        }
    }
    return 0;
}

// renders the room
void metalrender()
{
    std::cout << "\n";
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < l; j++)
        {
            std::cout << cMap[i][j];
        }
        std::cout << "\n";
    }
    std::cout << "to exit press esc; to place router press R;\n";
}

// checking router signal
void check_signal(std::string s)
{
    float answer;
    if (setval > 0)
    {
        int xder;
        int yder;
        std::string cut;
        std::string first;
        std::string last;
        cut = dict.at(s);

        if (cut[0] == ' ')
        {
            first = cut[1];
            if (cut[3] == ' ')
                last = cut[4];
            else
            {
                last = cut[3];
                last += cut[4];
            }
        }
        else {
            first = cut[0];
            first += cut[1];
            if (cut[3] == ' ')
                last = cut[4];
            else
            {
                last = cut[3];
                last += cut[4];
            }
        }

        xder = std::stoi(first);
        yder = std::stoi(last);

        int cxper;
        int cyper;

        if (y >= yder)
        {
            if (x <= xder)
            {
                cyper = sqrt(pow(y - yder,2));
                cxper = sqrt(pow(xder - x,2));
            }
            else if (x >= xder)
            {
                cyper = sqrt(pow(y - yder,2));
                cxper = sqrt(pow(x - xder,2));
            }
        }
        else if (y <= yder)
        {
            if (x <= xder)
            {
                cyper = sqrt(pow(yder - y,2));
                cxper = sqrt(pow(xder - x,2));
            }
            else if (x >= xder)
            {
                cyper = sqrt(pow(yder - y,2));
                cxper = sqrt(pow(x - xder,2));
            }
        }

        answer = (1 - (cyper / 8.0)-0.5) + (1 - (cxper / 4.0)-0.5);
        std::cout << answer;
    }
}

// invoke cheking for different routers (up to 4)
void invoke_checking()
{
    if (setval == 1)
    {
        std::cout << "power of router " + names[0] + ": ";
        check_signal(names[0]);
    }
    else if (setval == 2)
    {
        std::cout << "power of router " + names[0] + ": ";
        check_signal(names[0]);
        std::cout << "\npower of router " + names[1] + ": ";
        check_signal(names[1]);
    }
    else if (setval == 3)
    {
        std::cout << "power of router " + names[0] + ": ";
        check_signal(names[0]);
        std::cout << "\npower of router " + names[1] + ": ";
        check_signal(names[1]);
        std::cout << "\npower of router " + names[2] + ": ";
        check_signal(names[2]);
    }
    else if (setval == 4)
    {
        std::cout << "power of router " + names[0] + ": ";
        check_signal(names[0]);
        std::cout << "\npower of router " + names[1] + ": ";
        check_signal(names[1]);
        std::cout << "\npower of router " + names[2] + ": ";
        check_signal(names[2]);
        std::cout << "\npower of router " + names[3] + ": ";
        check_signal(names[3]);
    }
}

// move around room
void movement()
{
    set_keypress();
    char ch;
    if (cMap[x][y] == '.')
    {
        cMap[x][y] = '~';
    }
    metalrender();

    while (ch != (char)ESC_KEY)
    {
        if (ch == (char)W_KEY)
        {
            if (cMap[x][y] == '.' || cMap[x][y] == '~')
            {
                cMap[x][y] = '.';
            }
            x -= 1;
            if (x == 0)
            {
                x += 1;
                cMap[x][y] = '~';
                calc_coordinates();
                metalrender();
                invoke_checking();
            }
            else
            {
                if (cMap[x][y] == '.')
                {
                    cMap[x][y] = '~';
                }
                calc_coordinates();
                metalrender();
                invoke_checking();
            }
        }
        if (ch == (char)A_KEY)
        {
            if (cMap[x][y] == '.' || cMap[x][y] == '~')
            {
                cMap[x][y] = '.';
            }
            y -= 1;
            if (y == 0)
            {
                y += 1;
                cMap[x][y] = '~';
                calc_coordinates();
                metalrender();
                invoke_checking();
            }
            else
            {
                if (cMap[x][y] == '.')
                {
                    cMap[x][y] = '~';
                }
                calc_coordinates();
                metalrender();
                invoke_checking();
            }
        }
        if (ch == (char)S_KEY)
        {
            if (cMap[x][y] == '.' || cMap[x][y] == '~')
            {
                cMap[x][y] = '.';
            }
            x += 1;
            if (x == 15)
            {
                x -= 1;
                cMap[x][y] = '~';
                calc_coordinates();
                metalrender();
                invoke_checking();
            }
            else
            {
                if (cMap[x][y] == '.')
                {
                    cMap[x][y] = '~';
                }
                calc_coordinates();
                metalrender();
                invoke_checking();
            }
        }
        if (ch == (char)D_KEY)
        {
            if (cMap[x][y] == '.' || cMap[x][y] == '~')
            {
                cMap[x][y] = '.';
            }
            y += 1;
            if (y == 59)
            {
                y -= 1;
                cMap[x][y] = '~';
                calc_coordinates();
                metalrender();
                invoke_checking();
            }
            else
            {
                if (cMap[x][y] == '.')
                {
                    cMap[x][y] = '~';
                }
                calc_coordinates();
                metalrender();
                invoke_checking();
            }
        }
        if (ch == 'r')
        {
            create_router();
            metalrender();
        }
        std::cin >> ch;
    }
}

// calculating coordinates of player pointer on the map
void calc_coordinates()
{
    cMap[2][65] = y / 10 + 48;
    cMap[2][66] = y % 10 + 48;
    cMap[3][65] = x / 10 + 48;
    cMap[3][66] = x % 10 + 48;
}

// creating router, adding its name and coordinates to the dictionary
void create_router()
{
    cMap[x][y] = 'R';
    std::string rname;
    std::cout << "router name: ";
    std::cin >> rname;

    names[setval] = rname;

    if (x < 10)
    {
        dict[rname] += " ";
    }
    dict[rname] += std::to_string(x);
    dict[rname] += " ";
    if (y < 10)
    {
        dict[rname] += " ";
    }
    dict[rname] += std::to_string(y);

    if (cMap[x-1][y] == '.')
        cMap[x-1][y] = '-';

    if (cMap[x][y-1] == '.')
        cMap[x][y-1] = '|';

    if (cMap[x+1][y] == '.')
        cMap[x+1][y] = '-';

    if (cMap[x][y+1] == '.')
        cMap[x][y+1] = '|';

    if (cMap[x][y+8] == '.')
        cMap[x][y+8] = '|';

    if (cMap[x][y-8] == '.')
        cMap[x][y-8] = '|';

    if (cMap[x+4][y] == '.')
        cMap[x+4][y] = '-';

    if (cMap[x-4][y] == '.')
        cMap[x-4][y] = '-';

    if (cMap[x+3][y-1] == '.')
        cMap[x+3][y-1] = '_';
    if (cMap[x+3][y-2] == '.')
        cMap[x+3][y-2] = '_';
    if (cMap[x+3][y-3] == '.')
        cMap[x+3][y-3] = '\\';
    if (cMap[x+2][y-4] == '.')
        cMap[x+2][y-4] = '_';
    if (cMap[x+2][y-5] == '.')
        cMap[x+2][y-5] = '_';
    if (cMap[x+2][y-6] == '.')
        cMap[x+2][y-6] = '\\';
    if (cMap[x+1][y-7] == '.')
        cMap[x+1][y-7] = '_';
    if (cMap[x+1][y-8] == '.')
        cMap[x+1][y-8] = '\\';
    if (cMap[x+3][y+1] == '.')
        cMap[x+3][y+1] = '_';
    if (cMap[x+3][y+2] == '.')
        cMap[x+3][y+2] = '_';
    if (cMap[x+3][y+3] == '.')
        cMap[x+3][y+3] = '/';
    if (cMap[x+2][y+4] == '.')
        cMap[x+2][y+4] = '_';
    if (cMap[x+2][y+5] == '.')
        cMap[x+2][y+5] = '_';
    if (cMap[x+2][y+6] == '.')
        cMap[x+2][y+6] = '/';
    if (cMap[x+1][y+7] == '.')
        cMap[x+1][y+7] = '_';
    if (cMap[x+1][y+8] == '.')
        cMap[x+1][y+8] = '/';

    if (cMap[x-4][y-1] == '.')
        cMap[x-4][y-1] = '_';
    if (cMap[x-4][y-2] == '.')
        cMap[x-4][y-2] = '_';
    if (cMap[x-3][y-3] == '.')
        cMap[x-3][y-3] = '/';
    if (cMap[x-3][y-4] == '.')
        cMap[x-3][y-4] = '_';
    if (cMap[x-3][y-5] == '.')
        cMap[x-3][y-5] = '_';
    if (cMap[x-2][y-6] == '.')
        cMap[x-2][y-6] = '/';
    if (cMap[x-2][y-7] == '.')
        cMap[x-2][y-7] = '_';
    if (cMap[x-1][y-8] == '.')
        cMap[x-1][y-8] = '/';
    if (cMap[x-4][y+1] == '.')
        cMap[x-4][y+1] = '_';
    if (cMap[x-4][y+2] == '.')
        cMap[x-4][y+2] = '_';
    if (cMap[x-3][y+3] == '.')
        cMap[x-3][y+3] = '\\';
    if (cMap[x-3][y+4] == '.')
        cMap[x-3][y+4] = '_';
    if (cMap[x-3][y+5] == '.')
        cMap[x-3][y+5] = '_';
    if (cMap[x-2][y+6] == '.')
        cMap[x-2][y+6] = '\\';
    if (cMap[x-2][y+7] == '.')
        cMap[x-2][y+7] = '_';
    if (cMap[x-1][y+8] == '.')
        cMap[x-1][y+8] = '\\';

    setval += 1;
}

// deletes the router and its key-value pair from dict
void delete_router(std::string rnam)
{
    int x;
    int y;
    std::string cut;
    std::string first;
    std::string last;
    cut = dict.at(rnam);

    if (cut[0] == ' ')
    {
        first = cut[1];
        if (cut[3] == ' ')
            last = cut[4];
        else
        {
            last = cut[3];
            last += cut[4];
        }
    }
    else {
        first = cut[0];
        first += cut[1];
        if (cut[3] == ' ')
            last = cut[4];
        else
        {
            last = cut[3];
            last += cut[4];
        }
    }

    std::cout << cut << "\n";
    x = std::stoi(first);
    y = std::stoi(last);
    std::cout << x << "\n";
    std::cout << y << "\n";

    cMap[x][y] = '.';
    if (cMap[x-1][y] == '-')
        cMap[x-1][y] = '.';

    if (cMap[x][y-1] == '|')
        cMap[x][y-1] = '.';

    if (cMap[x+1][y] == '-')
        cMap[x+1][y] = '.';

    if (cMap[x][y+1] == '|')
        cMap[x][y+1] = '.';

    if (cMap[x][y+8] == '|')
        cMap[x][y+8] = '.';

    if (cMap[x][y-8] == '|')
        cMap[x][y-8] = '.';

    if (cMap[x+4][y] == '-')
        cMap[x+4][y] = '.';

    if (cMap[x-4][y] == '-')
        cMap[x-4][y] = '.';

    if (cMap[x+3][y-1] == '_')
        cMap[x+3][y-1] = '.';
    if (cMap[x+3][y-2] == '_')
        cMap[x+3][y-2] = '.';
    if (cMap[x+3][y-3] == '\\')
        cMap[x+3][y-3] = '.';
    if (cMap[x+2][y-4] == '_')
        cMap[x+2][y-4] = '.';
    if (cMap[x+2][y-5] == '_')
        cMap[x+2][y-5] = '.';
    if (cMap[x+2][y-6] == '\\')
        cMap[x+2][y-6] = '.';
    if (cMap[x+1][y-7] == '_')
        cMap[x+1][y-7] = '.';
    if (cMap[x+1][y-8] == '\\')
        cMap[x+1][y-8] = '.';
    if (cMap[x+3][y+1] == '_')
        cMap[x+3][y+1] = '.';
    if (cMap[x+3][y+2] == '_')
        cMap[x+3][y+2] = '.';
    if (cMap[x+3][y+3] == '/')
        cMap[x+3][y+3] = '.';
    if (cMap[x+2][y+4] == '_')
        cMap[x+2][y+4] = '.';
    if (cMap[x+2][y+5] == '_')
        cMap[x+2][y+5] = '.';
    if (cMap[x+2][y+6] == '/')
        cMap[x+2][y+6] = '.';
    if (cMap[x+1][y+7] == '_')
        cMap[x+1][y+7] = '.';
    if (cMap[x+1][y+8] == '/')
        cMap[x+1][y+8] = '.';

    if (cMap[x-4][y-1] == '_')
        cMap[x-4][y-1] = '.';
    if (cMap[x-4][y-2] == '_')
        cMap[x-4][y-2] = '.';
    if (cMap[x-3][y-3] == '/')
        cMap[x-3][y-3] = '.';
    if (cMap[x-3][y-4] == '_')
        cMap[x-3][y-4] = '.';
    if (cMap[x-3][y-5] == '_')
        cMap[x-3][y-5] = '.';
    if (cMap[x-2][y-6] == '/')
        cMap[x-2][y-6] = '.';
    if (cMap[x-2][y-7] == '_')
        cMap[x-2][y-7] = '.';
    if (cMap[x-1][y-8] == '/')
        cMap[x-1][y-8] = '.';
    if (cMap[x-4][y+1] == '_')
        cMap[x-4][y+1] = '.';
    if (cMap[x-4][y+2] == '_')
        cMap[x-4][y+2] = '.';
    if (cMap[x-3][y+3] == '\\')
        cMap[x-3][y+3] = '.';
    if (cMap[x-3][y+4] == '_')
        cMap[x-3][y+4] = '.';
    if (cMap[x-3][y+5] == '_')
        cMap[x-3][y+5] = '.';
    if (cMap[x-2][y+6] == '\\')
        cMap[x-2][y+6] = '.';
    if (cMap[x-2][y+7] == '_')
        cMap[x-2][y+7] = '.';
    if (cMap[x-1][y+8] == '\\')
        cMap[x-1][y+8] = '.';

    auto end = std::remove(names, names + setval, rnam);

    auto it = dict.find(rnam);
    if (it != dict.end())
        dict.erase(rnam);

    setval -= 1;
}

// function for editing terminal mode for getch()
// cause on linux theres no getch() function
void set_keypress(void)
{
    struct termios new_settings;
    tcgetattr(0,&stored_settings);
    new_settings = stored_settings;

    // disable canonical mode and set buffer to 1 byte
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_cc[VTIME] = 0;
    new_settings.c_cc[VMIN] = 1;

    tcsetattr(0,TCSANOW,&new_settings);
    return;
}
