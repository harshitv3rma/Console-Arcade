#include <iostream>
using namespace std;
class player{
    public:
        player(){}
        string name;
        int curr_level;
        static const int max_health = 1000; // FINAL keyword replaced by const | STATIC variable
        int curr_health = 1000;
    player(string name,int curr_level){ // CONSTRUCTOR
        this->curr_level = curr_level;
        this->name = name;
    }
};