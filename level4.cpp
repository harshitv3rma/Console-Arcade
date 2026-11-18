#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "level.cpp"
using namespace std;

// Given name to elements ; FIRE=0, WATER=1, ... DARK=5.
enum Element
{
    FIRE,
    WATER,
    EARTH,
    AIR,
    LIGHT,
    DARK
};
string elementName(Element e)
{
    string names[] = {"Fire", "Water", "Earth", "Air", "Light", "Dark"};
    return names[e];
}

// a = attacker's element , d = defender's element
bool beats(Element a, Element d)
{
    return (a == FIRE && d == EARTH) || (a == EARTH && d == WATER) ||
           (a == WATER && d == FIRE) || (a == LIGHT && d == DARK) ||
           (a == DARK && d == LIGHT);
}

double multiplier(Element a, Element d)
{
    if (beats(a, d))
        return 1.5;
    if (beats(d, a))
        return 0.5;
    return 1.0;
}
class Entity
{
protected:
    string name;
    Element element;
    int atk, def, maxHp, hp;

public:
    // PARAMETERIZED CONSTRUCTOR - object bante hi initial stats set hai.
    Entity(string n, Element e, int a, int d, int h)
    {
        name = n;
        element = e;
        atk = a;
        def = d;
        maxHp = h;
        hp = h;
    }

    // Virtual destructor - neccesssary for proper cleanup of derived objects when deleted through a base class pointer.
    virtual ~Entity() {}

    // Getters
    // Supports encapsulation by providing controlled access to private members.
    string getName() { return name; }
    Element getElement() { return element; }
    int getDef() { return def; }
    int getHp() { return hp; }
    int getMaxHp() { return maxHp; }
    bool isAlive() { return hp > 0; }

    // Abstraction helps in case if damage becomes negative (it will be set to 0)
    void takeDamage(int d)
    {
        hp -= d;
        if (hp < 0)
            hp = 0;
    }

    // Healing ke baad HP ko maxHp tak limit karte hain cant go above it
    void heal(int amount)
    {
        hp += amount;
        if (hp > maxHp)
            hp = maxHp;
    }

    // virtual funcation derived class can override this function to provide  damage calculation logic.
    virtual int damageTo(Entity &target)
    {
        // Formula = attack * element multiplier - enemy defence.
        int dmg = atk * multiplier(element, target.getElement()) - target.getDef();
        if (dmg < 1)
            dmg = 1;
        return dmg;
    }

    // Common display function
    virtual void show()
    {
        cout << name << " (" << elementName(element) << ") HP:" << hp << "/" << maxHp;
    }
};

// Entity se Monster, Card, Support ki branches hierarchical inheritance bhi dikhati hain.
class Monster : public Entity
{
public:
    // first it calls enitity constructor to initialize base class members then call monster to initialize derived class members
    //  phir Monster constructor ki body chalegi.
    Monster(string n, Element e, int a, int d, int h) : Entity(n, e, a, d, h) {}
};

// virtual inheritance- Paladin mein Entity ki duplicate copies se bachata hai.
class Card : virtual public Entity
{
protected:
    string role;
    int healPower;

public:
    //  if true halfs the damage on enemy card, if false full damage is done to enemy card
    bool guarding;
    Card(string n, Element e, int a, int d, int h, string r, int hp2 = 0)
        : Entity(n, e, a, d, h)
    {
        role = r;
        healPower = hp2;
        guarding = false;
    }

    int getHealPower() { return healPower; }

    // method overriding - Card ka show() Entity ke show() ko override karta hai
    void show() override
    {
        cout << name << " [" << elementName(element) << "/" << role << "] HP:" << hp
             << "/" << maxHp << " ATK:" << atk << " DEF:" << def;
        if (healPower > 0)
            cout << " HEAL:" << healPower;
    }
};

// inheriting virtuallly from entity to avoud diamond problem
class Support : virtual public Entity
{
protected:
    int shieldPower;

public:
    Support(string n, Element e, int a, int d, int h, int s)
        : Entity(n, e, a, d, h)
    {
        shieldPower = s;
    }

    int getShieldPower() { return shieldPower; }
};

// Multilevel inheritance - HealerCard inherits from Card which inherits from Entity
class HealerCard : public Card
{
public:
    // entity is virtual base class of Card and Support, so it is initialized by the most derived class (HealerCard) constructor.
    // on calling the healerCard constructor the Entity constructor is called first, then the Card constructor is called, and finally the HealerCard constructor is executed.
    HealerCard(string n, Element e, int a, int d, int h, int heal)
        : Entity(n, e, a, d, h), Card(n, e, a, d, h, "Healer", heal) {}
    // overrides damageTo function of Entity class to provide specialized damage calculation for HealerCard
    int damageTo(Entity &target) override
    {
        int dmg = atk * 0.5 * multiplier(element, target.getElement()) - target.getDef();
        if (dmg < 1)
            dmg = 1;
        return dmg;
    }
};

// multilevel inheritance
class Paladin : public Card, public Support
{
public:
    // paladin constructor initializes the base class members using initializer list. It first calls the Entity constructor to initialize the common attributes, then it calls the Card constructor to initialize the card-specific attributes, and finally it calls the Support constructor to initialize the support-specific attributes.
    Paladin(string n, Element e, int a, int d, int h, int heal, int s)
        : Entity(n, e, a, d, h), Card(n, e, a, d, h, "Paladin", heal), Support(n, e, a, d, h, s) {}

    // overrides damageto function of entity to provide special damage calculation logic
    void show() override
    {
        // scope resolution (::): calls Card::show() instead of Entity::show() to display the card's attributes.
        Card::show();
        cout << " SHIELD:" << shieldPower;
    }
};

vector<Card *> makeDeck()
{
    vector<Card *> deck;

    deck.push_back(new Card("Blaze Warrior", FIRE, 25, 10, 100, "Attacker"));
    deck.push_back(new Card("Aqua Guardian", WATER, 12, 25, 120, "Defender"));
    deck.push_back(new Card("Stone Sentinel", EARTH, 10, 30, 130, "Defender"));
    deck.push_back(new Card("Gale Striker", AIR, 22, 12, 95, "Attacker"));
    deck.push_back(new HealerCard("Solar Priestess", LIGHT, 8, 10, 90, 20));
    deck.push_back(new Card("Shadow Assassin", DARK, 28, 8, 85, "Attacker"));
    deck.push_back(new HealerCard("Ember Sprite", FIRE, 10, 8, 80, 15));
    deck.push_back(new Card("Tidecaller", WATER, 24, 10, 100, "Attacker"));
    deck.push_back(new Card("Rock Golem", EARTH, 20, 20, 110, "Attacker"));
    deck.push_back(new HealerCard("Wind Sage", AIR, 6, 10, 85, 18));
    deck.push_back(new Paladin("Holy Paladin", LIGHT, 18, 18, 105, 12, 5));
    return deck;
}

int readInt(int low, int high)
{
    int x;
    while (!(cin >> x) || x < low || x > high)
    {
        cout << "Enter a number between " << low << " and " << high << ": ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    return x;
}
bool teamAlive(vector<Card *> &team)
{
    for (int i = 0; i < team.size(); i++)
        if (team[i]->isAlive())
            return true;
    return false;
}
class level4 : public level
{
private:
    player &p;

public:
    level4(player &current_player) : level(4, "Card Battle"), p(current_player) {}

    void playLevel() override
    {
        // decides the seed for random number generation based on the current time, ensuring different random sequences on each run.
        srand(time(0));
        vector<Card *> deck = makeDeck();
        // lambda function cleanup is used to delete the dynamically allocated Card objects in the deck vector when the function scope ends
        Finally cleanup([&deck]()
                        {
            for (int i = 0; i < deck.size(); i++)
                delete deck[i]; });
        vector<Card *> team;

        cout << "\n\n===== LEVEL 4: Card Battle =====\n";
        cout << "Player: " << p.name << " | Health: " << p.curr_health << "/" << player::max_health << "\n\n";
        cout << "===== YOUR DECK =====\n";
        for (int i = 0; i < deck.size(); i++)
        {
            cout << i + 1 << ". ";
            deck[i]->show();
            cout << "\n";
        }

        cout << "\nPick 3 different cards by number:\n";
        while (team.size() < 3)
        {

            Card *c = deck[readInt(1, deck.size()) - 1];
            bool already = false;
            for (int i = 0; i < team.size(); i++)
                if (team[i] == c)
                    already = true;

            if (already)
                cout << "Already picked. Choose another.\n";
            else
                team.push_back(c);
        }

        cout << "\nTeam preview:\n";
        for (int i = 0; i < team.size(); i++)
        {
            // used onject slicing here as team[i] is a pointer to Card and we want to create a copy of the object it points to but we want to treat it as an Entity for the purpose of displaying its information. This allows us to call the show() method of the derived class (Card or HealerCard) while storing it in a base class (Entity) variable.
            Entity preview = *team[i];
            preview.show();
            cout << "\n";
        }

        Monster monsters[] = {
            Monster("Goblin Grunt", EARTH, 15, 5, 80),
            Monster("Fire Imp", FIRE, 18, 4, 70),
            Monster("Water Serpent", WATER, 16, 6, 90),
            Monster("Storm Harpy", AIR, 20, 4, 75),
            Monster("Shadow Wraith", DARK, 22, 5, 85),
            Monster("Crystal Golem", EARTH, 12, 15, 150)};
        Monster enemy = monsters[rand() % 6];

        cout << "\nA wild ";
        enemy.show();
        cout << " appears!\n";

        // Battle dont stop untill either monster dies or all 3 cards are dead
        while (enemy.isAlive() && teamAlive(team))
        {
            cout << "\n--- Status ---\nEnemy: ";
            enemy.show();
            cout << "\n";
            for (int i = 0; i < team.size(); i++)
            {
                team[i]->guarding = false;
                cout << i + 1 << ". ";
                team[i]->show();
                if (!team[i]->isAlive())
                    cout << " (KO)";
                cout << "\n";
            }

            cout << "Choose a card to act: ";
            int pick;
            // used do while loop to ensure that the player selects a card that is still alive. If the player selects a card that has been defeated (KO), the loop will prompt them to choose again until a valid selection is made.
            do
            {
                pick = readInt(1, 3) - 1;
            } while (!team[pick]->isAlive());
            Card *actor = team[pick];

            cout << "1. Attack\n2. Guard\n";
            int maxChoice = 2;
            if (actor->getHealPower() > 0)
            {
                cout << "3. Heal weakest ally\n";
                maxChoice = 3;
            }
            int action = readInt(1, maxChoice);

            if (action == 1)
            {
                int dmg = actor->damageTo(enemy);
                enemy.takeDamage(dmg);
                cout << actor->getName() << " hits for " << dmg << ". Enemy HP: " << enemy.getHp() << "\n";
            }
            else if (action == 2)
            {
                actor->guarding = true;
                cout << actor->getName() << " is guarding.\n";
            }
            else
            {
                // Find the weakest living ally to heal
                Card *weakest = actor;
                for (int i = 0; i < team.size(); i++)
                    if (team[i]->isAlive() && team[i]->getHp() < weakest->getHp())
                        weakest = team[i];
                weakest->heal(actor->getHealPower());
                cout << actor->getName() << " heals " << weakest->getName() << ".\n";
            }

            // if enemy defeated then break the loop and player wins else enemy attacks a random living teammate
            if (!enemy.isAlive())
                break;
            Card *target;
            do
            {
                target = team[rand() % 3];
            } while (!target->isAlive());

            // target pointer is used to select a random living teammate for the enemy to attack. The damage calculation takes into account whether the target is guarding, halving the damage if they are. The minimum damage is set to 1 to ensure that every attack has some effect. After calculating the damage, the target's health is reduced accordingly, and a message is displayed indicating the attack and the resulting damage.
            int dmg = enemy.damageTo(*target);
            if (target->guarding)
                dmg = dmg / 2;
            if (dmg < 1)
                dmg = 1;
            target->takeDamage(dmg);
            cout << enemy.getName() << " attacks " << target->getName() << " for " << dmg << ".\n";
        }

        if (enemy.isAlive())
        {
            cout << "\nYour team fainted. You lost the battle.\n";
            p -= 300; // used operator overloading to deduct 300 health from main player
            cout << "Global Player Health decreased by 300!\n";
        }
        else
        {
            cout << "\nYou defeated " << enemy.getName() << "! You win!\n";
        }
    }
};

// int main()
// {
// }
