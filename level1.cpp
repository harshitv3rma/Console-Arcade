#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "player.cpp" // Assuming player class is stored here

using namespace std;

// Structure to hold our riddle data cleanly
struct Riddle
{
    string question;
    string answer;
    string hint;
};

class level1 : public player
{
private:
    vector<Riddle> riddle_pool;
    int level_health;

public:
    level1(string playerName) : player(playerName, 1)
    {
        level_health = 100; // Local health pool for this specific level

        // Add multiple riddles to the pool
        riddle_pool.push_back({"What has keys but cannot open locks?", "piano", "It is a musical instrument."});
        riddle_pool.push_back({"I speak without a mouth and hear without ears. What am I?", "echo", "Sound bouncing back."});
        riddle_pool.push_back({"The more of this there is, the less you see. What is it?", "darkness", "Absence of light."});
        riddle_pool.push_back({"What has a head, a tail, is brown, and has no legs?", "penny", "A type of coin."});
        riddle_pool.push_back({"I have branches, but no fruit, trunk or leaves. What am I?", "bank", "A financial institution."});
        riddle_pool.push_back({"What can you catch, but not throw?", "cold", "A common illness."});
        riddle_pool.push_back({"What gets wet while drying?", "towel", "Used after a shower."});
    }

    // Check answer (case-insensitive)
    bool checkAnswer(string userAnswer, string correctAnswer)
    {
        transform(userAnswer.begin(), userAnswer.end(), userAnswer.begin(), [](unsigned char c)
                  { return tolower(c); });
        transform(correctAnswer.begin(), correctAnswer.end(), correctAnswer.begin(), [](unsigned char c)
                  { return tolower(c); });

        return userAnswer == correctAnswer;
    }

    void playLevel()
    {
        // Shuffle the riddles using rand() to ensure random, non-repeating selection
        srand(static_cast<unsigned int>(time(0)));
        for (int i = riddle_pool.size() - 1; i > 0; --i)
        {
            int j = rand() % (i + 1);
            swap(riddle_pool[i], riddle_pool[j]);
        }

        cout << "Welcome, " << name << "! Starting Level " << curr_level << "...\n";
        cout << "You have " << level_health << " Level Health.\n";
        cout << "Answer 4 riddles correctly to win. Wrong answers cost 25 Level Health.\n\n";

        // Loop exactly 4 times for 4 rounds
        for (int round = 1; round <= 4; round++)
        {
            Riddle currentRiddle = riddle_pool[round - 1]; // Pick a riddle from our shuffled pool

            cout << "=== ROUND " << round << " ===\n";
            cout << "Riddle: " << currentRiddle.question << "\n";

            // Keeps asking until they get it right or level health drops to 0
            while (level_health > 0)
            {
                string userAnswer;
                cout << "Your answer: ";
                getline(cin, userAnswer);

                if (checkAnswer(userAnswer, currentRiddle.answer))
                {
                    cout << "Correct!\n\n";
                    break; // Breaks the while-loop to move to the next round
                }
                else
                {
                    level_health -= 25;
                    cout << "Wrong answer!\n";
                    cout << "Hint: " << currentRiddle.hint << "\n";

                    if (level_health <= 0)
                    {
                        cout << "\nYour Level Health has depleted to 0!\n";

                        // --- GLOBAL HEALTH DEDUCTION TRIGGERED ---
                        curr_health -= 100;
                        if (curr_health < 0)
                            curr_health = 0;

                        cout << "Global Player Health decreased by 100!\n";
                        cout << "Current Global Health: " << curr_health << " / " << max_health << "\n";
                        cout << "You failed Level 1. GAME OVER.\n";
                        return; // Exits playLevel() completely
                    }
                    else
                    {
                        cout << "Level Health remaining: " << level_health << "\n";
                        cout << "Try again!\n\n";
                    }
                }
            }
        }

        // If the loop finishes and they still have level health, they win
        if (level_health > 0)
        {
            cout << "Congratulations, " << name << "! You successfully cleared all 4 rounds!\n";
            cout << "Global Health remains intact at: " << curr_health << " / " << max_health << "\n";
        }
    }
};

int main()
{
    // Create game object, passing the player name "Hero"
    level1 game("Hero");

    // Start the multi-round level
    game.playLevel();

    return 0;
}
