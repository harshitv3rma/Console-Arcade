#include "player.cpp"
#include "level.cpp"
#include <iostream>
#include <deque>
#include <utility>
#include <string>
#include <random>
#include <algorithm>
using namespace std;
class Level3 : public level
{
private:
    // Player reference
    player &p;
    // Board properties
    const int boardLength = 15;
    const int boardWidth = 15;
    // Game properties
    const int targetFood = 10;
    int snakeLength;
    int foodCollected;
    // Snake properties
    string snakeDirection;
    deque<pair<int, int>> snakeDeque;
    // Food position
    pair<int, int> foodPosition;
    // Random number generator
    mt19937 rng;

public:
    // Constructor
    Level3(player &current_player)
        : level(3, "Snake Game"),
          p(current_player),
          snakeLength(7),
          foodCollected(0),
          snakeDirection("Right"),
          rng(random_device{}())
    {
        // Initial snake position
        snakeDeque.push_back({6, 8});
        snakeDeque.push_back({5, 8});
        snakeDeque.push_back({4, 8});
        snakeDeque.push_back({3, 8});
        snakeDeque.push_back({2, 8});
        snakeDeque.push_back({1, 8});
        snakeDeque.push_back({0, 8});
        // Initial food position
        foodPosition = {5, 2};
    }
    // Move the snake
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
        // Add the new head
        snakeDeque.push_front(newHead);
        // Check whether food is collected
        if (newHead == foodPosition)
        {
            foodCollected++;
            cout << "\nFood collected!";
            cout << " (" << foodCollected << "/" << targetFood << ")" << endl;
            // Increase snake length
            snakeLength++;
            // Generate new food if target is not reached
            if (foodCollected < targetFood)
            {
                spawnFood();
            }
        }
        else
        {
            // Remove the tail if food is not collected
            snakeDeque.pop_back();
        }
        // Keep length synchronized
        snakeLength = snakeDeque.size();
    }
    // Generate food at a random empty position
    void spawnFood()
    {
        uniform_int_distribution<int> xDistribution(
            0, boardLength - 1);
        uniform_int_distribution<int> yDistribution(
            0, boardWidth - 1);
        while (true)
        {
            int x = xDistribution(rng);
            int y = yDistribution(rng);
            pair<int, int> newFood = {x, y};
            bool occupied = false;
            // Check whether position is occupied by snake
            for (const auto &segment : snakeDeque)
            {
                if (segment == newFood)
                {
                    occupied = true;
                    break;
                }
            }
            if (!occupied)
            {
                foodPosition = newFood;
                return;
            }
        }
    }
    // Check collision with walls or itself
    bool checkCollision()
    {
        pair<int, int> head = snakeDeque.front();
        int x = head.first;
        int y = head.second;
        // Wall collision
        if (x < 0 || x >= boardLength ||
            y < 0 || y >= boardWidth)
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
    // Change snake direction
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
    // Display the game board
    void displayBoard()
    {
        // Clear previous screen
        cout << string(30, '\n');
        cout << "========== LEVEL 3 ==========" << endl;
        cout << "          SNAKE GAME         " << endl;
        cout << "Player: " << p.name << " | Health: " << p.curr_health << "/" << player::max_health << endl;
        cout << "-----------------------------" << endl;
        cout << "Food Collected : " << foodCollected << " / " << targetFood << endl;
        cout << "Snake Length   : " << snakeLength << endl;
        cout << "Direction      : " << snakeDirection << endl;
        cout << endl;
        // Top boundary
        for (int x = 0; x < boardLength; x++)
            cout << "# ";
        cout << endl;
        // Display board
        for (int y = 0; y < boardWidth; y++)
        {
            for (int x = 0; x < boardLength; x++)
            {
                pair<int, int> currentPosition = {x, y};
                bool printed = false;
                // Display snake
                for (size_t i = 0; i < snakeDeque.size(); i++)
                {
                    if (snakeDeque[i] == currentPosition)
                    {
                        if (i == 0)
                            cout << "O "; // Snake head
                        else
                            cout << "o "; // Snake body
                        printed = true;
                        break;
                    }
                }
                // Display food
                if (!printed && currentPosition == foodPosition)
                {
                    cout << "* ";
                    printed = true;
                }
                // Empty position
                if (!printed)
                    cout << ". ";
            }
            cout << endl;
        }
        // Bottom boundary
        for (int x = 0; x < boardLength; x++)
            cout << "# ";
        cout << endl;
    }
    // Main game function
    void playLevel() override
    {
        cout << endl;
        cout << "       LEVEL 3 - SNAKE GAME" << endl;
        cout << "\nGoal: Eat " << targetFood << " food items." << endl;
        cout << "Board: " << boardLength << " x " << boardWidth << endl;
        cout << "\nControls:" << endl;
        cout << "W = Up | S = Down | " << "A = Left | D = Right | Q = Quit" << endl;
        // Main game loop
        while (true)
        {
            displayBoard();
            // Check level completion
            if (foodCollected >= targetFood)
            {
                cout << "\nLEVEL 3 COMPLETED!" << endl;
                cout << "Congratulations, " << p.name << "! You collected all " << targetFood << " foods!" << endl;
                return;
            }
            cout << "\nCurrent direction: " << snakeDirection << endl;
            cout << "Enter direction (W/A/S/D), " << "or Q to quit: ";
            char input;
            cin >> input;
            // Quit game
            if (input == 'q' || input == 'Q')
            {
                cout << "\nLeaving Level 3..." << endl;
                return;
            }
            // Validate input
            if (input == 'w' || input == 'W' ||
                input == 'a' || input == 'A' ||
                input == 's' || input == 'S' ||
                input == 'd' || input == 'D')
            {
                changeDirection(input);
            }
            else
            {
                cout << "\nInvalid input! " << "Continuing in the previous direction." << endl;
            }
            // Move snake
            moveSnake();
            // Check collision
            if (checkCollision())
            {
                displayBoard();
                cout << "\nSTAGE FAILED!" << endl;
                cout << "The snake collided with " << "the wall or itself." << endl;
                // Apply health damage
                int damage = 200;
                p.curr_health -= damage;
                if (p.curr_health < 0)
                    p.curr_health = 0;
                // Check player statistics
                level::checkStats(p);
                if (p.curr_health <= 0)
                {
                    cout << "\nYou died! GAME OVER." << endl;
                }
                return;
            }
        }
    }
};