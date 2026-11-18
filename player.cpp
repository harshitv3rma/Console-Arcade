#pragma once
#include <iostream>
#include <string>
#include <limits>
using namespace std;

// This is a template which allows us to use a function over different data types
template <typename T>
T clampValue(T value, T low, T high)
{
    if (value < low)
        return low;
    if (value > high)
        return high;
    return value;
}

class level;
class level1;
class Level3;
class level4;
class level5;

//Class contains data members and the functions on which the data members will be used and shows encapsulation
class player
{
private:
    // priavte members only accessible within class and friend scope(or using getter, setter methods)
    string name;
    int curr_level;
    int curr_health;

public:
   // One copy of max_health which works for all the levels
    inline static int max_health = 1000;

    
   // constructoe for player
    player(string name, int curr_level)
    {
        this->name = name;
        this->curr_level = curr_level;
        this->curr_health = max_health;
    }

    // getter and setter for accessing the private members of class player
    string getName() const { return name; }
    int getLevel() const { return curr_level; }
    int getHealth() const { return curr_health; }
    void setLevel(int l) { curr_level = l; }

    // Declaration of friend classes 
    friend class level;
    friend class level1;
    friend class Level3;
    friend class level4;
    friend class level5;
   
    // operator overloading used with friend function for accessing private members of class
    friend player &operator-=(player &p, int damage);

    // Friend function: overloads << so a player can be printed with cout << playerObject
    friend ostream &operator<<(ostream &out, const player &p);

    // Friend function: overloads >> so input can be read directly into a player object
    friend istream &operator>>(istream &in, player &p);
};

// overloaidn -= for the global health deduction logic
player &operator-=(player &p, int damage)
{
    p.curr_health = clampValue(p.curr_health - damage, 0, player::max_health);
    return p;
}

// overloading << for output 
ostream &operator<<(ostream &out, const player &p)
{
    out << p.name << " | Level: " << p.curr_level << " | Health: " << p.curr_health << "/" << player::max_health;
    return out;
}

// overloading >> for input 
istream &operator>>(istream &in, player &p) 
{
    // Added safety check for bad input stream
    if (in >> p.name) {
        in.ignore(numeric_limits<streamsize>::max(), '\n');
    } else {
        in.clear();
        in.ignore(numeric_limits<streamsize>::max(), '\n');
        p.name = "Hero"; // Default fallback for the user
    }
    return in;
}
