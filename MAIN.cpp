#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include "player.cpp"
#include "level1.cpp"
#include "level2.cpp"
#include "level3.cpp"
#include "level4.cpp"
#include "level5.cpp"

using namespace std;

class LevelFactory
{
public:
    static unique_ptr<level> create(int number, player &p)
    { // unique_ptr is special pointer which frees memory it self
        switch (number)
        {
        case 1:
            return make_unique<level1>(p);
        case 2:
            return make_unique<level2>(p);
        case 3:
            return make_unique<Level3>(p);
        case 4:
            return make_unique<level4>(p);
        default:
            return make_unique<level5>(p);
        }
    }
};

void saveScore(const player &p, int total)
{
    ofstream file("scores.txt", ios::app);
    if (file.is_open())
    {
        file << p << " | Total Reward: " << total << "\n";
        file.close();
    }
}

int main()
{
    cout << "\n\n   Welcome to the Console Arcade    \n\n\n";
    cout << "Developed by: Harshit, Mayank, Pranshu, Madhav, and Heri\n\n";

    cout << "Enter Player Name: ";
    player mainPlayer("", 1);
    cin >> mainPlayer;

    cout << "\nGood luck, " << mainPlayer.getName() << "! Let the games begin.\n\n";

    // Polymorphism via base class pointer
    vector<unique_ptr<level>> arcadeLevels;
    vector<Reward> rewards;
    int totalReward = 0;

    for (int n = 1; n <= 5; n++)
    {
        arcadeLevels.push_back(LevelFactory::create(n, mainPlayer));
    }
    cout << "Levels loaded: " << level::getLevelsCreated() << "\n";

    for (size_t i = 0; i < arcadeLevels.size(); i++)
    { // CONTROLS level progression and keeps levels in sync
        mainPlayer.setLevel(i + 1);

        cout << "\n--- LOADING LEVEL " << mainPlayer.getLevel() << " ---\n";

        // calls the overridden playLevel() for the current level object
        arcadeLevels[i]->playLevel();

        // checks if player survived the stage
        if (mainPlayer.getHealth() <= 0)
        {
            cout << "\n\nGame Over !  Your health dropped to 0.\n";
            cout << "Thanks for playing the CS201 Console Arcade !\n";
            saveScore(mainPlayer, totalReward);
            return 0;
        }

        Reward earned(100 * mainPlayer.getLevel(), "Stage Clear");
        bool known = false;
        for (size_t j = 0; j < rewards.size(); j++)
        {
            if (rewards[j] == earned)
                known = true;
        }
        if (!known)
        {
            rewards.push_back(earned);
            totalReward += earned.getPoints();
        }

        cout << "\nStage Cleared! Moving to the next challenge...\n";
        cout << "Reward: " << earned.getTitle() << " (" << earned.getPoints() << " pts)\n";
    }

    cout << "\n\n CONGRATULATIONS " << mainPlayer.getName() << "! You conquered the arcade! \n";
    cout << "Total Reward Points: " << totalReward << "\n";
    saveScore(mainPlayer, totalReward);

    return 0;
}
