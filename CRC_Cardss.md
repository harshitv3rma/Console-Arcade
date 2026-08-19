# CRC Diagrams

## 1. Game Class

| Class | Responsibilities | Collaborators |
|---|---|---|
| **Game** | • `startGame()` – Start the game<br>• `endGame()` – End the game<br>• `playerDetails()` – Manage/display player details<br>• **Tracks the overall score**<br>• **Maintains game statistics**<br>• **Knows the current game index** | **Player**, **Level** |

## 2. Level Class

| Class | Responsibilities | Collaborators |
|---|---|---|
| **Level** | • `nextLevel()` – Move to the next level<br>• `skipLevel()` – Skip the current level<br>• `playLevel()` – Start/play the level<br>• `checkStats()` – Check level/game statistics<br>• `resetLevel()` – Reset the current level<br>• **Tracks level attempts**<br>• **Knows the level goal** | **Game**, **Player** |

## 3. Player Class

| Class | Responsibilities | Collaborators |
|---|---|---|
| **Player** | • **Knows current health**<br>• **Knows the player's name** | **Game**, **Level** |

## 4. Level3 Class

| Class | Responsibilities | Collaborators |
|---|---|---|
| **Level3** | • `moveSnake()` – Calculate next snake position<br>• `spawnFood()` – Generate new food coordinates<br>• `checkCollision()` – Detect wall or self-collision<br>• **Knows board dimensions** (length, width)<br>• **Knows snake state** (length, direction, body segments)<br>• **Knows food position** | **Game**, **Player** |