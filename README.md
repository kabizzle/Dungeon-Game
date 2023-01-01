# Dungeon game 

Programming task for ELEC-A7100 Basic course in C programming (Spring 2022)

The main purpose of a player is to explore a dungeon without getting hit by a monster. The game has a dungeon composed of several rooms, which are connected by corridors. A dungeon map is represented as a two dimensional array of tiles. Each tile can be one of

open,

wall, or

room.

If a tile is open or room, the player can move to it. If it is a wall, player cannot move to it. The monsters have some movement and attack features. The game is turn-based so that after each player movement, all monsters on the map are moved based on monster specific algorithms. If the monsters come to the next tile of the player, the monsters hit them, causing some amount of damage. If the player moves to the tile of the monster, they attack the monster and cause random damage to them.

In the provided template files, you can find an implementation of simplified version of a dungeon crawling game. As you will notice, it lacks several features compared to its original predecessors, but you are free to enhance the game in various ways if you wish to do so.

The user controls the game by pressing the following keyboard keys.

n: move north

s: move south

e: move east

w: move west

q: exit game

Input is line buffered, so you’ll need to press enter after the command to observe its impact.

Should the functioned described below are implemented correctly, the game would output the following screenshot when a monster 'D' approaching the player '*' from left. It shows the current (and max) hitpoints above the command prompt.
