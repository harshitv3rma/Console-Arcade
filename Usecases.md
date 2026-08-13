**Specifications for three cases:**

1**. Use Case 1:**

1. **Case ID**: UC-01
2. **Use case name**: Start the game
3. **Primary actor-Player**
4. **Stakeholder:**
5. **Player**\- The player will run the game from the beginning and follow a linear game journey of different levels and challenges.
6. **System Developer-Uses OOP** (Encapsulation and Array/List of polymorphic objects) to control campaign progression.

**e)** **Preconditions:**

1)The console application should be launched successfully.

1. The initialised memory is the level queue.

**f)** **Postconditions:**

1)System loads level 1 into memory.

1. The campaign journey begins.

**g) Trigger**: Player opens the application and selects "start journey".

1. **MAIN FLOW:**

1) System displays welcome title screen with player options (Start journey \[1\], Quit journey \[2\]).
2) Player inputs 1 to start the journey.
3) System displays the level information of the first level(The riddle).
4) System transfers control to the execution loop of Level 1 (proceeds to UC-02).

### 3**. Alternate / Exceptional Flows**

1. A specific keyword that a user can use to skip a level.

**A2: Exit Application Before Journey Starts**

- - Player selects option 2 at the welcome screen.
    - System prints exit message and gracefully closes the program.

## **Use Case 2: Play Current Level Stage**

### **1\. Specification Details**

- **Use Case ID:** UC-02
- **Use Case Name:** Play Current Level Stage
- **Primary Actor:** Player
- **Stakeholders:**
  - **Player:** Needs clear feedback on moves, constraints, and progress within the active minigame stage.
  - **Level Subclasses:** Encapsulates rules specific to that mini-game.
- **Preconditions:**
  - UC-01 is complete; a specific level is loaded as active.
- **Postconditions:**
  - Level state updates.
  - Game registers whether the stage goal is fulfilled or failed.
- **Trigger:** Console prompts the user for action input during game stage execution.

**2\. Main flow:**

1. System renders current stage display in console and prompts for input.
2. Player inputs action/move command.
3. System validates input against current level's rules and handles the exception cases.
4. System updates internal stage state variables(health,stamina,etc).
5. System checks winning/completion criteria for the current stage.
6. System re-renders updated console view.
7. Stage completion criteria met: System flags stage state as complete and ends active game loop (Proceeds to UC-03).

### **3\. Alternate / Exceptional Flows**

- **A1: Invalid Command Input**
  - **1.** Player inputs out-of-bounds or misformatted data.
  - **2.** System catches error, displays prompt: _"Invalid input for this stage. Try again."_
  - **3.** System re-prompts without advancing turns or penalizing stats (Returns to Step 1).
- **A2: Failure / Game-Over Condition Met**
  - **1.** Player exhausts attempts or loses the current mini-game (e.g., out of lives).
  - **2.** System flags stage state as FAILED.
  - **3.** System displays failure screen (_"Stage Failed!"_) and gives option to \[1\] Retry Level or \[2\] Quit Journey.
  - **4.** If retry chosen, System calls reset() on current Level object and returns to Step 1.

**Use Case 3: Unlock and Progress to Next Level**

**1\. Specification Details**

- **Use Case ID:** UC-03
- **Use Case Name:** Unlock and Progress to Next Level
- **Primary Actor:** Player
- **Stakeholders:**
  - **Player:** Granted permission to the next level and moved directly to the next mini-game in the journey.
  - **Campaign Controller:** Handles progression logic, unlocking the next index in the polymorphism array.
- **Preconditions:**
  - Current Level stage has been successfully cleared (COMPLETED state in UC-02).
- **Postconditions:**
  - Current level memory is cleared, and that memory is used for loading the next level.
  - Next level in queue is set as active, or full campaign victory is declared.
- **Trigger:** Player successfully satisfies completion conditions of current mini-game.

**2\. Main Success Scenario (Main Flow)**

1. System displays Level Clear victory banner and calculates score/bonus for this stage.
2. System updates total player campaign statistics. (Health, Stamina, etc)
3. System checks if additional levels remain in the campaign sequence queue (currentIndex < totalLevels).
4. System increments level index (currentIndex++).
5. System displays transition screen (_"Stage Unlocked! Press ENTER to enter Level \[X\]"_).
6. Player presses ENTER.
7. System loads next Level object polymorphically and transfers control (returns to Step 5 of UC-01).

**3\. Alternate / Exceptional Flows**

- **A1: Final Campaign Level Completed (Journey Victory)**
  - **1.** At Step 3, the system checks the sequence queue and finds no remaining levels (currentIndex == totalLevels).
  - **2.** System triggers Grand Victory Sequence screen.

**3.** The system prompts the player to exit the system or restart the whole campaign journey.