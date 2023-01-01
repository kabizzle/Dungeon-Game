# Dungeon game 

Programming task for ELEC-A7100 Basic course in C programming (Spring 2022)

The main purpose of a player is to explore a dungeon without getting hit by a monster. The game has a dungeon composed of several rooms, which are connected by corridors. A dungeon map is represented as a two dimensional array of tiles. Each tile can be one of

- open

- wall, or

- room

If a tile is open or room, the player can move to it. If it is a wall, player cannot move to it. The monsters have some movement and attack features. The game is turn-based so that after each player movement, all monsters on the map are moved based on monster specific algorithms. If the monsters come to the next tile of the player, the monsters hit them, causing some amount of damage. If the player moves to the tile of the monster, they attack the monster and cause random damage to them.

The user controls the game by pressing the following keyboard keys.

- n: move north

- s: move south

- e: move east

- w: move west

- q: exit game

Input is line buffered, so the player needs to press "Enter" after the command to observe its impact.
