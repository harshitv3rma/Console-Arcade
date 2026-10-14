#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <unistd.h> // for usleep()

using namespace std;

// Parent class common for all the games
class level {
protected:
    string level_name;
    int level_number;

public:
    level(int number, string name) {
        level_number = number;
        level_name = name;
    }

    void display() {
        cout << "Level " << level_number << ": "
             << level_name << endl;
    }

    // Derived classes provide their own game.
    virtual void playLevel() = 0;

    virtual ~level() {}
};

// Child class
class level2 : public level {
private:
    int score;

public:
    // Calling the parent constructor.
    level2() : level(2, "Pattern Recognition") {
        score = 0;
    }

    void playLevel() override {
        score = 0;

        display();
        cout << "Remember the digits shown one by one.\n";
        cout << "Enter 'skip' to activate cheat.\n";

        for (int round = 1; round <= 3; round++) {
            int length = round + 2;
            string number = "";

            // Generate a random sequence of digits.
            for (int i = 0; i < length; i++) {
                number += char('0' + rand() % 10);
            }

            cout << "\nRound " << round << endl;
            usleep(1000000); // 1 second

            // Display one digit at a time.
            for (int i = 0; i < length; i++) {
                system("clear"); // clears terminal on macOS/Linux

                cout << "Round " << round << endl;
                cout << "\nRemember: " << number[i] << endl;

                usleep(1000000); // 1 second
            }

            system("clear");

            string answer;
            cout << "Enter the number you remember: ";
            cin >> answer;

            if (answer == "skip") {
                cout << "\nCheat activated!\n";
                cout << "Level 2 skipped.\n";
                return; // Exit this level's function.
            }

            if (answer == number) {
                cout << "Correct!\n";
                score++;
            } else {
                cout << "Wrong!\n";
                cout << "Correct number was: " << number << endl;
                break;
            }
        }

        cout << "\n===== GAME OVER =====\n";
        cout << "Final Score: " << score << "/3\n";

        if (score == 3) {
            cout << "Congratulations! You completed Level 2!\n";
        }
    }
};

int main() {
    srand(static_cast<unsigned int>(time(0)));

    level2 game;      // Creating an object.
    game.playLevel(); // Start the game.

    return 0;
}