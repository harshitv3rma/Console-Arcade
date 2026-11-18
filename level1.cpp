#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include "level.cpp"

using namespace std;

// We have defined struct for storing the riddle parameters..All of them are pulic by default
struct Riddle
{
    string question;
    string answer;
    string hint;
};

// This is a pure virtual class and it requires overriding of functions when inherited
class Runnable
{
public:
    virtual void run() = 0;
    // This a virtual destructor for Runnable object
    virtual ~Runnable() {}
};

// Inheriting Runnable in class Thread
class Thread : public Runnable
{
private:                // using encapuslation to limit data to its class and friends
    std::thread worker; // standard thread object
    std::atomic<int> state;

    // This pointer allows different Runnable-derived task objects to be supplied.
    Runnable *target;

public:
    enum State
    {
        NEW,
        RUNNABLE,
        TERMINATED
    };

    // Constructor initializes members using an initializer list
    // Runnable pointer allows the thread to work with different task types and shows polymorphism
    Thread(Runnable *t = nullptr) : state(NEW), target(t) {}

    ~Thread() { join(); }

    // redefining the run which was created in Runnable(pure virtual class) which shows run time polymorphism

    void run() override
    {
        if (target)
            target->run();
    }

    void start()
    {
        state.store(RUNNABLE);
        worker = std::thread([this]()
                             {
            run();
            state.store(TERMINATED); });
    }

    void join()
    {
        if (worker.joinable())
            worker.join();
    }

    static void sleepFor(int ms)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }

    static void yieldNow()
    {
        std::this_thread::yield();
    }

    string getState() const
    {
        string names[] = {"NEW", "RUNNABLE", "TERMINATED"};
        return names[state.load()];
    }
};

// Inheritance: HintThread is a specialized kind of thread
class HintThread : public Thread
{
public:
    // Thread is being initialised by the base class constructor
    HintThread(Runnable *t) : Thread(t) {}

    ~HintThread() { join(); }

    // Overrides Thread::run() to add behavior before running the task
    // runtime polymorphism as there is a function over-riding involved
    void run() override
    {
        Thread::yieldNow();
        Thread::run();
    }
};

// Inheritance: HintTask implements the Runnable interface, so it can be run by thread
class HintTask : public Runnable
{
private:
    // HintTask does not make separate copies of them.
    std::mutex &cout_mtx;
    std::atomic<bool> &answered;
    int delay_seconds;
    string hint;

public:
    HintTask(std::mutex &m, std::atomic<bool> &a, int d, const string &h)
        : cout_mtx(m), answered(a), delay_seconds(d), hint(h) {}

    // Implements the pure virtual Runnable::run() function
    void run() override
    {
        for (int i = 0; i < delay_seconds * 10; ++i)
        {
            if (answered.load())
                return;
            Thread::sleepFor(100);
        }

        if (!answered.load())
        {
            std::lock_guard<std::mutex> lock(cout_mtx);
            cout << "\n\n[Time's ticking! Free Hint: " << hint << "]\n";
            cout << "Your answer: " << flush;
        }
    }
};

// Level 1 is inheriting the common properties of level
class level1 : public level
{
private:
    player &p;
    vector<Riddle> riddle_pool;
    int level_health;

    std::mutex cout_mtx;
    std::atomic<bool> answered;

public:
    // Friend function gave access to the private/protected members of the level class
    friend class level;

    // Constructor initializer list: first initializes the base class 'level', then member p
    // The player is passed by reference so this level updates the original player object
    level1(player &current_player) : level(1, "Riddles"), p(current_player)
    {
        level_health = 100;

        riddle_pool.push_back({"What has keys but cannot open locks?", "piano", "It is a musical instrument."});
        riddle_pool.push_back({"I speak without a mouth and hear without ears. What am I?", "echo", "Sound bouncing back."});
        riddle_pool.push_back({"The more of this there is, the less you see. What is it?", "darkness", "Absence of light."});
        riddle_pool.push_back({"What has a head, a tail, is brown, and has no legs?", "penny", "A type of coin."});
        riddle_pool.push_back({"I have branches, but no fruit, trunk or leaves. What am I?", "bank", "A financial institution."});
        riddle_pool.push_back({"What can you catch, but not throw?", "cold", "A common illness."});
        riddle_pool.push_back({"What gets wet while drying?", "towel", "Used after a shower."});
    }

    bool checkAnswer(string userAnswer, string correctAnswer)
    {
        transform(userAnswer.begin(), userAnswer.end(), userAnswer.begin(), [](unsigned char c)
                  { return tolower(c); });
        transform(correctAnswer.begin(), correctAnswer.end(), correctAnswer.begin(), [](unsigned char c)
                  { return tolower(c); });

        return userAnswer == correctAnswer;
    }

    // Overrides the virtual function from base class 'level'
    // Calling playLevel through a level pointer/reference can select this implementation at runtime
    void playLevel() override
    {
        srand(static_cast<unsigned int>(time(0)));
        for (int i = riddle_pool.size() - 1; i > 0; --i)
        {
            int j = rand() % (i + 1);
            swap(riddle_pool[i], riddle_pool[j]);
        }

        {
            std::lock_guard<std::mutex> lock(cout_mtx);
            cout << "Welcome, " << p.name << "! Starting Level " << p.curr_level << "...\n";
            cout << "You have " << level_health << " Level Health.\n";
            cout << "Answer 4 riddles correctly to win. You have 10 seconds per riddle before a hint appears!\n\n";
        }

        for (int round = 1; round <= 4; round++)
        {
            Riddle currentRiddle = riddle_pool[round - 1];

            {
                std::lock_guard<std::mutex> lock(cout_mtx);
                cout << "=== ROUND " << round << " ===\n";
                cout << "Riddle: " << currentRiddle.question << "\n";
            }

            while (level_health > 0)
            {
                answered.store(false);

                {
                    std::lock_guard<std::mutex> lock(cout_mtx);
                    cout << "Your answer: " << flush;
                }

                HintTask task(cout_mtx, answered, 10, currentRiddle.hint);
                HintThread hint_thread(&task);
                hint_thread.start();

                string userAnswer;

                // Check for invalid input buffer states
                if (!getline(cin, userAnswer))
                {
                    cin.clear();
                    cout << "\n[Warning] Invalid input format! Try again.\n";
                    userAnswer = "";
                }

                answered.store(true);
                hint_thread.join();

                {
                    std::lock_guard<std::mutex> lock(cout_mtx);
                    cout << "[Hint thread " << hint_thread.getState() << "]\n";
                }

                if (userAnswer.empty())
                {
                    continue;
                }

                if (checkAnswer(userAnswer, currentRiddle.answer))
                {
                    std::lock_guard<std::mutex> lock(cout_mtx);
                    cout << "Correct!\n\n";
                    break;
                }
                else
                {
                    level_health -= 25;

                    std::lock_guard<std::mutex> lock(cout_mtx);
                    cout << "Wrong answer!\n";

                    if (level_health <= 0)
                    {
                        cout << "\nYour Level Health has depleted to 0!\n";

                        // Updated the penalty to 250 here
                        p.curr_health -= 250;
                        if (p.curr_health < 0)
                            p.curr_health = 0;

                        cout << "Global Player Health decreased by 250!\n";
                        cout << "Current Global Health: " << p.curr_health << " / " << player::max_health << "\n";
                        cout << "You failed Level 1. GAME OVER.\n";
                        return;
                    }
                    else
                    {
                        cout << "Level Health remaining: " << level_health << "\n";
                        cout << "Try again!\n\n";
                    }
                }
            }
        }

        if (level_health > 0)
        {
            std::lock_guard<std::mutex> lock(cout_mtx);
            cout << "Congratulations, " << p.name << "! You successfully cleared all 4 rounds!\n";
            cout << "Global Health remains intact at: " << p.curr_health << " / " << player::max_health << "\n";
        }
    }
};
