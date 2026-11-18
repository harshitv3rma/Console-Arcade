#pragma once
#include "player.cpp"
#include <iostream>
using namespace std;
// Finally aur tmplate use kiya hai to make it general
template <typename F>
class Finally
{
private:
    F action;

public:
    Finally(F a) : action(a) {}        // Deleted copy constructor
    Finally(const Finally &) = delete; // yaha copying disable ki taki koi duplicate na ho
    ~Finally() { action(); }
};
// Encapsulation
class Reward
{
private:
    const int points;
    const string title;

public:
    Reward(int p, string t) : points(p), title(t) {}
    int getPoints() const { return points; } // const  ensures ki method modify na ho
    string getTitle() const { return title; }
    bool operator==(const Reward &other) const // operator overloading taki direc comparison ho paye
    {
        return points == other.points && title == other.title;
    }
};
// yaha base aur abstract class ka implementation hua hai
class level
{
protected:
    string level_name;
    int level_number;
    inline static int levels_created = 0;
    void nextLevel();
    void skipLevel();
    void resetLevel();
    static void checkStats(player p) // Static member funtion,yr belong karta hai class ko naki object ko
    {
        cout << "Player's Current Health : ";
        cout << p.curr_health << endl;
    }

public:
    level() {}
    level(int level_number, string level_name)
    {
        this->level_name = level_name; // THIS keyword  use
        this->level_number = level_number;
        levels_created++;
    }
    // yaha virtual dewtructor use hua hai
    virtual ~level() {}
    static int getLevelsCreated() { return levels_created; }
    void display()
    {
        cout << level_name;
        cout << endl
             << level_number;
    }
    // use of virtual function and abstract class
    virtual void playLevel() = 0;
};
