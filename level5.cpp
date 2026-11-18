#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <exception>
#include <cstdlib>
#include "level.cpp"

using namespace std;

class MazeError : public std::exception // base exception
{
public:
    virtual const char *what() const noexcept override
    {
        return "Invalid input! Please use W, A, S, or D.";
    }
};

class WallError : public MazeError // derived exception
{
public:
    const char *what() const noexcept override
    {
        return "Ouch! You bumped into a wall.";
    }
};

class level5 : public level
{
private:
    player &p;
    vector<string> maze;
    int playerX;
    int playerY;
    bool gameWon;
    bool skipped;

    string statusMessage; // for exception message
    int moves;

public:
    level5(player &current_player) : level(5, "Maze Game"), p(current_player)
    {
        maze = {
            "#################################################################################",
            "#P        #       #               # #           #       #                   #   #",
            "######### # ####### ##### ####### # # # ####### # ##### # ########### ##### ### #",
            "#       # #     #       #   #   #   # # # #   #   # #   #     #     #   #   #   #",
            "# # ### # # ### # ######### # # ##### # # # # ##### # ####### # ### ### ### # # #",
            "# # #   # # # # #   #   #   # # #     # # # #   #   #       #   #   # #   #   # #",
            "### # ### # # # ### # # # ### # # ##### # # # ### # # ####### ##### # ### ##### #",
            "#   #   # #   #     # # #   # #   #     # # #     # # #       #   #   #   #   # #",
            "# ### ### ### ####### # # # # ##### ##### # ####### # # ####### # ##### ### # # #",
            "# #   #   # #       # # # # # # #   #     # # #   #   #       # #       # # # # #",
            "# ##### ### # ####### # # # # # # ### ### # # # # ########### # ######### # # # #",
            "#       #     #     # # # #     # # # # #     # # #           # #     #     # # #",
            "# ####### ##### ### # # # ####### # # # ####### # # ########### # ##### ##### # #",
            "#       # #     # #   # #     #   #     #     # # #   # #       #     # #   #   #",
            "####### ### ##### ##### ##### # ######### ### # # ### # # ####### ### # # #######",
            "#     #   # #     #   #     # #     #       #   #     # #       # # #   #       #",
            "# ### ### # # ### ### ##### ##### # # ############### # ####### # # ##### ##### #",
            "#   #   # # # #           #     # #   #   #   #     #     #   # #       # #   # #",
            "# ### # # # # ########### ##### ####### # # # # ### ####### # # ####### ### # # #",
            "# #   # #   #       #     # #   #       # # #   # # #       #   #     #   # #   #",
            "# # ############# # # ##### # ### ####### # ##### # # ########### # ##### # ### #",
            "# #             # # # #     # #       # #   #       # #       #   # #     # # # #",
            "# ####### ##### ### # ### # # ### ### # ##### ####### ####### # # ### ##### # # #",
            "# #     # #   #     #   # # #   #   # #     #       # #     # # #     #     #   #",
            "# # ### # # # ######### ### ### ##### # # ######### # # ### # # ####### # ##### #",
            "# # #     # #         #     # #   #   # #     #   # #     # # #       # #     # #",
            "# # ############# ### ##### # ### # ####### # ### # ####### # ####### # ##### ###",
            "# #             #   #     # #   #   #       #   # # #       # #     # #     #   #",
            "# ############# ### ##### # ### ##### # ####### # # # ####### # ### # # ####### #",
            "# # #         # #     #   # #     # # #       #   #   #     #   #   # # #     # #",
            "# # # # ##### # ####### # # # ### # # ####### ######### # ### ### ### # # ### # #",
            "# #   #   #   # #       # #   #     # #     # #         # #     # #   # # #   # #",
            "# ##### # ##### # ######### ####### # # ##### # ######### # ##### # ##### # ### #",
            "# #   # # #     #           #   #   # #   #   # #       #   #   # # #   # # #   #",
            "# # # ### # ############### # # ### # # # # ### # ### # ##### # # # # # # # # # #",
            "# # #     #   #   #   #   # # #   # # # # #     # #   #   #   #   # # #   #   # #",
            "# # ##### ### # # # # # # # # ### # # ### ######### ##### # # ##### # # ####### #",
            "# #     #   #   #   # # # # #   # # #     #       #     # # # #     # # #     # #",
            "# # ### ### ######### # # # ### # ####### ### ### ##### # # # # ##### ### ### ###",
            "# #   #   #   #     #   # # #   #       #   # # #       # # # # #   #   #   #   #",
            "# ### ### ### ##### ##### ### ####### ##### # # ######### # ### ### # # ### ### #",
            "# #   # #   #           # #   #     #     # # #         # #   #   #   #     #   #",
            "# ### # ### ########### # # ### # ####### # # ####### ### ### ### # ######### # #",
            "#   # #   #   #   #     # #   # # #       # #       #       #   # #     #   # # #",
            "### # # # ### # ### ##### ### # # # ##### # ### ### # ######### # ####### # # # #",
            "#   #   #   # #     # #     #   # # #   # #   #   # #   #       #       # #   # #",
            "# ########### # ##### # ### ##### # # # ##### ##### ### # # ########### # ##### #",
            "#   #   #     #   #   #   #     # # # #     # #     #   # # #     #     #   #   #",
            "### # # # ####### ### ### ####### # # ### ### # ##### ### ### ### # ####### # ###",
            "#     #   #             #         #     #       #       #       #           #  E#",
            "#################################################################################"};
        playerX = 1;
        playerY = 1;
        gameWon = false;
        skipped = false;
        statusMessage = "";
        moves = 0;
    }

    void draw()
    {
        cout << "\033[2J\033[1;1H"; // Clears the terminal screen

        cout << "          MAZE GAME          \n";
        cout << "Player: " << p.name << " | Health: " << p.curr_health << "/" << player::max_health << " | Moves: " << moves << "\n";

        for (const string &row : maze)
        {
            for (char c : row)
            {
                if (c == '#')
                {
                    cout << "#";
                }
                else
                {
                    cout << c;
                }
            }
            cout << "\n";
        }

        if (!statusMessage.empty())
        {
            cout << "\n[!] " << statusMessage << "\n";
            statusMessage = "";
        }

        if (!gameWon && !skipped)
            cout << "\nMove (W/A/S/D) or type 'skip' (-300 HP), and press Enter: ";
    }

    void movePlayer(int dx, int dy)
    {
        int nextX = playerX + dx;
        int nextY = playerY + dy;

        if (maze[nextY][nextX] == 'E')
        {
            gameWon = true;
            maze[playerY][playerX] = ' ';
            maze[nextY][nextX] = 'P';
        }
        else if (maze[nextY][nextX] == '#')
        {
            throw WallError();
        }
        else
        {
            maze[playerY][playerX] = ' ';
            playerX = nextX;
            playerY = nextY;
            maze[playerY][playerX] = 'P';
        }
    }

    void processInput()
    {
        string inStr; // input string
        if (!(cin >> inStr))
        { // reads data and true if input fails
            cin.clear();
            cin.ignore(10000, '\n');
            statusMessage = "Invalid input stream detected!";
            return;
        }

        // for skip
        string lowerInput = inStr;
        for (char &c : lowerInput)
        {
            c = tolower(c);
        }

        // for skip
        if (lowerInput == "skip")
        {
            skipped = true;
            return;
        }

        char input = lowerInput[0];

        Finally counter([this]()
                        { moves++; }); // lambda function, move is counted even if exception comes

        try
        {
            if (input == 'w')
                movePlayer(0, -1);
            else if (input == 's')
                movePlayer(0, 1);
            else if (input == 'a')
                movePlayer(-1, 0);
            else if (input == 'd')
                movePlayer(1, 0);
            else
            {
                throw MazeError();
            }
        }
        catch (const WallError &e) // catch derived class first
        {
            statusMessage = e.what();
        }
        catch (const MazeError &e) // catch base class second
        {
            statusMessage = e.what();
        }
    }

    bool isOver()
    {
        return gameWon || skipped;
    }

    void playLevel() override
    {
        level5 *ptr = this;

        while (!ptr->isOver())
        {
            ptr->draw();
            ptr->processInput();

            // deduct health if skipped
            if (ptr->skipped)
            {
                cout << "\n[SKIP ACTIVATED] You chose to bypass the maze!\n";

                p -= 300;

                cout << "Global Player Health decreased by 300!\n";
                cout << "Current Global Health: " << p.curr_health << " / " << player::max_health << "\n";
                return;
            }
        }

        // Only prints if the player successfully finishes the maze
        if (ptr->gameWon)
        {
            ptr->draw();
            cout << "\nCongratulations, " << p.name << "! You escaped the maze!\n";
        }
    }
};
