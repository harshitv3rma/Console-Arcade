#pragma once
#include <iostream>
#include <string>
#include <cstdlib> // for random() and system()
#include <ctime>
#include <thread>
#include <chrono>
#include "level.cpp"

using namespace std;

#ifdef _WIN32 // Conditionl compilation taki macOS or windows dono pe chale
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

// yaha multithreading ke aplication kiya hai
//  Countdown create kiya hai
void countdown()
{
    for (int i = 3; i >= 1; i--)
    {
        cout << "Starting in " << i << "...\n";
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}
// Inheritance use kiy hai
class level2 : public level
{
private:
    player &p;
    int score;
    // yaha nested class aur sath me encapsulation implement kara hai
    // gnerator class banayi hai taki pattern generate ho paye
    class Generator
    {
    public:
        string make(int length)
        {
            string digits = "";
            for (int i = 0; i < length; i++)
            {
                digits += char('0' + rand() % 10); // random single number ko string me add kiya hai
            }
            return digits;
        }
    };

public:
    // constructor apply kiya hai
    level2(player &current_player) : level(2, "Pattern Recognition"), p(current_player)
    {
        score = 0;
    }
    // Polymorphism and method overriding ka application kiya hai taki level ke virtual ko defination de saku
    void playLevel() override
    {
        score = 0;

        display();
        cout << "\nRemember the digits shown one by one.\n";
        cout << "Cheat code is skip\n";

        for (int round = 1; round <= 3; round++)
        {
            int length = round + 2;
            Generator generator;
            string number = generator.make(length);

            cout << "\nRound " << round << endl;

            // Multithreading ka application kiya hai taki counter aur number generation ek sath ho
            thread t(countdown); // t object is created for completeing execution of countdown
            cout << "Generating your pattern in ...\n";
            t.join(); // thread synchronization

            for (int i = 0; i < length; i++)
            {
                system(CLEAR_SCREEN);

                cout << "Round " << round << endl;
                cout << "\nPattern: " << number[i] << endl;

                this_thread::sleep_for(chrono::milliseconds(1000));
            }

            system(CLEAR_SCREEN);

            string answer;
            cout << "Enter the number: ";

            if (!(cin >> answer))
            {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "\nWarning invalid input stream! Treating as incorrect.\n";
                answer = "INVALID";
            }
            // Lvel skip karne ke liye cheat code dala hai
            if (answer == "skip")
            {
                cout << "\nCheat activated!\n";
                cout << "Level 2 skipped.\n";
                return;
            }

            if (answer == number)
            {
                cout << "Correct!\n";
                score++;
            }
            else
            {
                cout << "Wrong!\n";
                cout << "Correct number was: " << number << endl;
                // yaha operator overloading kiya hai taki global player health kaat saku
                p -= 200;

                cout << "Global Player Health decreased by 200!\n";
                cout << "Current Global Health: " << p.getHealth() << " / " << player::max_health << "\n";

                if (p.getHealth() <= 0)
                {
                    cout << "Your health has depleted to 0! GAME OVER.\n";
                    return;
                }

                cout << "Moving to the next round...\n";
                this_thread::sleep_for(chrono::milliseconds(1500));
            }
        }

        cout << "\n===== LEVEL 2 COMPLETE =====\n";
        cout << "Final Score: " << score << "/3\n";

        if (score == 3)
        {
            cout << "Congratulations! You completed Level 2.Now you can move to next level!\n";
        }
        else
        {
            cout << "You survived Level 2!\n";
        }
    }
};