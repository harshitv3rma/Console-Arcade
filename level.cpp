// #include <bits/stdc++.h>
#include "player.cpp"
#include <iostream>
using namespace std;

class level
{
protected:
    string level_name;
    int level_number;
    void nextLevel();
    void skipLevel();
    void playLevel();
    void resetLevel();
    static void checkStats(player p)
    { // STATIC method
        cout << "Player's Current Health : ";
        cout << p.curr_health << endl;
    }

public:
    level() {}
    level(int level_number, string level_name)
    {                                  // CONSTRUCTOR
        this->level_name = level_name; // THIS keyword
        this->level_number = level_number;
    }
    void display()
    {
        cout << level_name;
        cout << endl
             << level_number;
    }
};
