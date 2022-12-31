/**
 * \file monster.c
 * \brief Implementation of monster actions
 * \version 0.1
 * 
 * \copyright Aalto University, Department of Communications and Networking (c) 2022
 * 
 */

#include "dungeon.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief A monster type that can be used in the game
 * 
 */
typedef struct {
	char name[20];		 //!< Name of monster
	char sign;			 //!< character to show it on map
	unsigned int hplow;	 //!< lowest possible initial maxhp
	unsigned int hphigh; //!< highest possible initial maxhp
} MonstType;

/**
 * \brief A list of ready-made monster types
 * 
 * \details Defines three monster types to start with.
 * 
 * 
 * \note Feel free to add more, or change the below.
 *       It is up to you to decide whether to use this array from createMonsters.
 *         
 */
const MonstType types[] = {{"Goblin", 'G', 6, 10},
						   {"Rat", 'R', 3, 5},
						   {"Dragon", 'D', 15, 20}};

/**
 * \brief One kind of attack done by monster. The mosters' attack function 
 *        pointer can refer to this.
 * 
 * \param game The game 
 * \param monst The monster performing attack
 */
void attackPunch(Game* game, Creature* monst) {
	printf("%s punches you! ", monst->name);
	int hitprob = 50;
	int maxdam = 4;
	if(rand() % 100 < hitprob) {
		printf("Hit! ");
		int dam = rand() % maxdam + 1;
		printf("Damage: %d ", dam);
		game->hp = game->hp - dam;
		if(game->hp <= 0)
			printf("You died!");
		printf("\n");
	} else {
		printf("Miss!\n");
	}
}



/**
 * \brief Move monster 'monst' towards the player character by one tile.
 * 
 * \details The game logic uses this function to move the monsters, which have 
 *          high enough hitpoints, towards the player. The function must obey 
 *          the following restrictions.
 *
 *          a. Monsters do not move diagonally.
 *
 *          b. If possible, the distance between monster and the character should 
 *             reduce when the function returns.
 *
 *          c. A monster must not move more than one tile on the map.
 *
 *          d. A monster must not move over/into a wall.
 *       
 *          e. A monster must not move to a tile occupied by another monster.
 *       
 *          f. A monster must not move to a tile occupied by the player. 
 *
 *          Besides these restrictions, you are free to choose the details of the 
 *          movement algorithm.
 *
 *        note: 
 *         
 *         1. You can assume that monsters have magical powers to detect the player's 
 *            location irrespective of the walls between them.
 *
 *         2. Do not check the hitpoints of the monster since the game logic 
 *            already does so.
 *
 * \param game The game state
 * \param monst The monster to be moved toward the player
 */
void moveTowards(const Game* game, Creature* monst) {
	// (void) game; 
	// (void) monst; 
	// TODO: implement this function 
	int player_x = game->position.x;
	int player_y = game->position.y;
	int monster_x = monst->pos.x;
	int monster_y = monst->pos.y;

	if ((monster_y > player_y) && (!isBlocked(game, monster_x, monster_y-1)))
		monst->pos.y = monst->pos.y - 1; 
	else if ((monster_y < player_y) && (!isBlocked(game, monster_x, monster_y+1)))
		monst->pos.y = monst->pos.y + 1;
	else if ((monster_x > player_x) && (!isBlocked(game, monster_x-1, monster_y)))
		monst->pos.x = monst->pos.x - 1;
	else if ((monster_x < player_x) && (!isBlocked(game, monster_x+1, monster_y)))
		monst->pos.x = monst->pos.x + 1;   

}

/**
 * \brief Move monster 'monst' away the player character by one tile.
 * 
 * \details The game logic uses this function to move the monsters, which have 
 *          low hitpoints, away from the player. The function must obey 
 *          the following restrictions.
 *
 *          a. Monsters do not move diagonally.
 *
 *          b. If possible, the distance between monster and the character should 
 *             increase when the function returns.
 *
 *          c. A monster must not move more than one tile on the map.
 *
 *          d. A monster must not move over/into a wall.
 *       
 *          e. A monster must not move to a tile occupied by another monster.
 *       
 *          f. A monster must not move to a tile occupied by the player. 
 *
 *          Besides these restrictions, you are free to choose the details of the 
 *          movement algorithm.
 *
 *        note: 
 *         
 *         1. You can assume that monsters have magical powers to detect the player's 
 *            location irrespective of the walls between them.
 *
 *         2. Do not check the hitpoints of the monster since the game logic 
 *            already does so.
 * 
 * \param game The game state
 * \param monst The monster to be moved away the player
 */
void moveAway(const Game* game, Creature* monst) {
	// (void) game; 
	// (void) monst; 
	// TODO: implement this function 
	int player_x = game->position.x;
	int player_y = game->position.y;
	int monster_x = monst->pos.x;
	int monster_y = monst->pos.y;

	if ((monster_y > player_y) && (!isBlocked(game, monster_x, monster_y+1)))
		monst->pos.y = monster_x + 1; 
	else if ((monster_y < player_y) && (!isBlocked(game, monster_x, monster_y-1)))
		monst->pos.y = monster_y - 1;
	else if ((monster_x > player_x) && (!isBlocked(game, monster_x+1, monster_y)))
		monst->pos.x = monster_x + 1;
	else if ((monster_x < player_x) && (!isBlocked(game, monster_x-1, monster_y)))
		monst->pos.x = monster_x - 1;  
}



/**
 * \brief  Takes action on each monster (that is alive) in 'monsters' array.
 * 
 * \details For each monster, the function should 
 *       
 *       a. if a monster is in an adjacent location to the player, 
 *          it should try to attack the player. 
 *       
 *       b. Otherwise, the monster should invoke the movement action. 
 *          
 *       c. If monster is dead (with hitpoints zero or less), no action is taken.
 *
 *       note:: 
 *         A. Function pointers attack and move in Creature
 *            structure determine the action to be taken. You are required 
 *            to call these function pointers to invoke either of 
 *            monster actions. 
 *         
 *            If either of the function pointers is NULL, the respective 
 *            action is ignored. 
 *         
 *         B. You should specify valid attack and movement behaviors (i.e., 
 *            function pointers) when creating monsters. The game template 
 *            provides one attack behavior, but you are free to define more.
 *
 * 
 * \param game The game
 */
void monsterAction(Game* game) {
	// (void) game; 
	// TODO: implement this function 
	int num_monsters = game->numMonsters;
	int i;
	Creature *monster;
	int y_distance, x_distance;

	for (i = 0; i < num_monsters; i++){
		monster = (Creature*)&game->monsters[i];
		if (game->monsters[i].hp != 0){
			y_distance = game->position.y - game->monsters[i].pos.y;
			x_distance = game->position.x - game->monsters[i].pos.x;
			if (game->position.x == game->monsters[i].pos.x && abs(y_distance)==1){
				game->monsters[i].attack(game, monster);
			}
			else if (game->position.y == game->monsters[i].pos.y && abs(x_distance)==1){
				game->monsters[i].attack(game, monster);
			}
			else {
				game->monsters[i].move(game, monster);
			}
		}

	}
}



/**
 * \brief  Create opts.numMonsters monsters and position them on valid positions 
 *         in the the game map. The moster data (hitpoints, name, map sign) 
 *         should be set appropriately.
 * 
 * \details The function should 
 *       
 *       1. create ``opts.numMonsters`` number of monsters.
 *
 *       2. allocate memory for each monster in ``monsters`` array of the ``game`` 
 *          object (i.e. ).
 *    
 *       3. place them on random free positions on the map that are 
 *          
 *          a. not wall tiles, and 
 *          b. not occupied by other monsters, and
 *          v. inside the map area. 
 *
 *          hint: 
 *            You can use isBlocked function to check if a monster can be 
 *            placed at a random position.
 *
 *       4. initializes other monster details, such as name, the map sign shown 
 *          on the user interface, hitpoints, and so on.
 *
 *          a. Each monster must have more than ``0`` hit points, and initial 
 *             hit points should be set to the maximum HP.
 *          
 *          b. The monster name must be properly initialized.
 *          
 *          c. The monster's map sign must be an alphabetic character.
 *          
 *          d. The other fields can be initialized freely.
 * 
 * \param game The game
 */
void createMonsters(Game* game) {
	// (void) game; 
	int num_monsters = game->opts.numMonsters;
	game->numMonsters = num_monsters;

	int map_width = game->opts.mapWidth;
	int map_height = game->opts.mapHeight;
	int i;
	game->monsters = calloc(sizeof(Creature), num_monsters);
	char sign;
	int x, y, hp, rand_num;

	for (i = 0; i < num_monsters; i++){
		x = rand() % map_width;
		y = rand() % map_height;
		while (isBlocked(game, x, y)){
			x = rand() % game->opts.mapWidth;
			y = rand() % game->opts.mapHeight;
		}
		game->monsters[i].pos.x = x;
		game->monsters[i].pos.y = y;
		
		rand_num = rand() % 3;
		// monster_name = types[rand_num].name;
		hp = types[rand_num].hphigh;
		sign = types[rand_num].sign;

		strcpy(game->monsters[i].name, types[rand_num].name);
		game->monsters[i].maxhp = hp;
		game->monsters[i].hp = hp;
		game->monsters[i].sign = sign;
	}
	
}

/**
 * \brief Determine whether the monsters move towards or away from the 
 * player character.
 * 
 * \param game The game
 */
void checkIntent(Game* game) {
	for(unsigned int i = 0; i < game->numMonsters; i++) {
		Creature* m = &game->monsters[i];
		if(m->hp <= 2) {
			m->move = moveAway;
		} else {
			m->move = moveTowards;
		}
		if(m->hp < m->maxhp)
			m->hp = m->hp + 0.1; // heals a bit every turn
	}
}
