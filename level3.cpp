// #include "player.cpp"
#include "level.cpp"
#include <iostream>
#include <deque>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

class Level3 : public level
{
private:
    // --- PLAYER INTEGRATION: Reference to the player object ---
    player &p;

    int boardLength;
    int boardWidth;
    int targetFood;
    int snakeLength;
    string snakeDirection;

    // deque holds pair<int,int> = (x,y)
    deque<pair<int, int>> snakeDeque;
    pair<int, int> foodPosition;
    bool growthPending;
    int foodCollected;

    // void moveSnake();
    // void spawnFood();
    // bool checkCollision();

public:
    // Constructor now requires a player object reference
    Level3(player &current_player)
        : level(3, "Snake Game"), p(current_player)
    {
        boardLength = 15;
        boardWidth = 15;
        targetFood = 10;
        snakeLength = 7;
        snakeDirection = "Right";
        growthPending = false;
        foodCollected = 0;

        snakeDeque.push_back({6, 8});
        snakeDeque.push_back({5, 8});
        snakeDeque.push_back({4, 8});
        snakeDeque.push_back({3, 8});
        snakeDeque.push_back({2, 8});
        snakeDeque.push_back({1, 8});
        snakeDeque.push_back({0, 8});

        foodPosition = {5, 2};

        // Random number generator
        srand(static_cast<unsigned int>(time(nullptr)));
    }

    // MOVE SNAKE
    void moveSnake()
    {
        pair<int, int> head = snakeDeque.front();
        int newX = head.first;
        int newY = head.second;

        if (snakeDirection == "Up")
            newY--;
        else if (snakeDirection == "Down")
            newY++;
        else if (snakeDirection == "Left")
            newX--;
        else if (snakeDirection == "Right")
            newX++;

        pair<int, int> newHead = {newX, newY};
        snakeDeque.push_front(newHead);

        // Check if food is eaten
        if (newHead == foodPosition)
        {
            foodCollected++;
            snakeLength++;
            growthPending = true;
            cout << "\nFood collected!";
            cout << "  (" << foodCollected << "/" << targetFood << ")" << endl;

            if (foodCollected < targetFood)
            {
                spawnFood();
            }
        }

        // Remove tail if snake didn't eat food
        if (!growthPending)
        {
            snakeDeque.pop_back();
        }
        else
        {
            growthPending = false;
        }
    }

    // SPAWN FOOD
    void spawnFood()
    {
        bool validPosition = false;
        while (!validPosition)
        {
            int x = rand() % boardLength;
            int y = rand() % boardWidth;
            pair<int, int> newFood = {x, y};
            validPosition = true;

            for (const auto &segment : snakeDeque)
            {
                if (segment == newFood)
                {
                    validPosition = false;
                    break;
                }
            }
            if (validPosition)
            {
                foodPosition = newFood;
            }
        }
    }

    // CHECK COLLISION
    bool checkCollision()
    {
        pair<int, int> head = snakeDeque.front();
        int x = head.first;
        int y = head.second;

        // Wall collision
        if (x < 0 || x >= boardLength || y < 0 || y >= boardWidth)
        {
            return true;
        }

        // Self collision
        for (size_t i = 1; i < snakeDeque.size(); i++)
        {
            if (snakeDeque[i] == head)
            {
                return true;
            }
        }
        return false;
    }

    // CHANGE DIRECTION
    void changeDirection(char input)
    {
        if (input == 'w' || input == 'W')
        {
            if (snakeDirection != "Down")
                snakeDirection = "Up";
        }
        else if (input == 's' || input == 'S')
        {
            if (snakeDirection != "Up")
                snakeDirection = "Down";
        }
        else if (input == 'a' || input == 'A')
        {
            if (snakeDirection != "Right")
                snakeDirection = "Left";
        }
        else if (input == 'd' || input == 'D')
        {
            if (snakeDirection != "Left")
                snakeDirection = "Right";
        }
    }

    // DISPLAY BOARD
    void displayBoard()
    {
        cout << string(30, '\n');
        cout << "========== LEVEL 3 ==========" << endl;
        cout << "SNAKE GAME" << endl;

        // --- USING 'p.' TO ACCESS PLAYER VARIABLES ---
        cout << "Player: " << p.name << " | Health: " << p.curr_health << "/" << player::max_health << endl;
        cout << "-----------------------------" << endl;
        cout << "Food Collected : " << foodCollected << " / " << targetFood << endl;
        cout << "Snake Length   : " << snakeLength << endl;
        cout << "Direction      : " << snakeDirection << endl;
        cout << endl;

        // Top border
        for (int x = 0; x < boardLength; x++)
            cout << "# ";
        cout << endl;

        // Board
        for (int y = 0; y < boardWidth; y++)
        {
            for (int x = 0; x < boardLength; x++)
            {
                pair<int, int> currentPosition = {x, y};
                bool printed = false;

                for (size_t i = 0; i < snakeDeque.size(); i++)
                {
                    if (snakeDeque[i] == currentPosition)
                    {
                        if (i == 0)
                            cout << "O "; // Head
                        else
                            cout << "o "; // Body
                        printed = true;
                        break;
                    }
                }

                if (!printed && currentPosition == foodPosition)
                {
                    cout << "* ";
                    printed = true;
                }

                if (!printed)
                    cout << ". ";
            }
            cout << endl;
        }

        // Bottom border
        for (int x = 0; x < boardLength; x++)
            cout << "# ";
        cout << endl;
    }

    // PLAY LEVEL (No override keyword needed here)
    void playLevel()
    {
        cout << endl;
        cout << "          LEVEL 3 - SNAKE GAME" << endl;
        cout << endl;
        cout << "Goal: Eat " << targetFood << " food items." << endl;
        cout << "Board: " << boardLength << " x " << boardWidth << endl;
        cout << endl;
        cout << "Controls:" << endl;
        cout << "W = Up | S = Down | A = Left | D = Right | Q = Quit" << endl;

        while (true)
        {
            displayBoard();

            if (foodCollected >= targetFood)
            {
                cout << "\nLEVEL 3 COMPLETED!" << endl;
                cout << "Congratulations, " << p.name << "! You collected all " << targetFood << " foods!" << endl;
                return;
            }

            cout << "\nCurrent direction: " << snakeDirection << endl;
            cout << "Enter direction (W/A/S/D), or Q to quit: ";

            char input;
            cin >> input;

            if (input == 'q' || input == 'Q')
            {
                cout << "\nLeaving Level 3..." << endl;
                return;
            }

            if (input == 'w' || input == 'W' ||
                input == 'a' || input == 'A' ||
                input == 's' || input == 'S' ||
                input == 'd' || input == 'D')
            {
                changeDirection(input);
            }
            else
            {
                cout << "Invalid input! Continuing in the previous direction." << endl;
            }

            moveSnake();

            if (checkCollision())
            {
                displayBoard();
                cout << "\nSTAGE FAILED!" << endl;
                cout << "The snake collided with the wall or itself." << endl;

                // --- APPLYING DAMAGE TO PLAYER ---
                int damage = 200;
                p.curr_health -= damage;
                if (p.curr_health < 0)
                    p.curr_health = 0;

                // Using the level class static function to show health dynamically
                level::checkStats(p);

                if (p.curr_health <= 0)
                {
                    cout << "You died! GAME OVER.\n";
                }

                return; // Ends the game level
            }
        }
    }
};

int main()
{
    // 1. Initialize the player class properly
    player mainPlayer("Hero", 3);

    // 2. Pass the player object into the Level3 game
    Level3 game(mainPlayer);

    // 3. Start the game
    game.playLevel();

    return 0;
}
