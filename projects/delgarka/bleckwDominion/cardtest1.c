/******************************************************************************
* Kamille Delgardo
* CS 362 Software Engineering II
* Card Test 1 -- initializeGame()
* 14 July 2019
******************************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

/* Basic requirements of initializeGame():
* Supply should contain 7 basic supply piles (curse, copper, silver, gold, estate, duchy, province)
* Supply should contain 10 random Kingdom cards
* Supply Victory piles should have 8 cards in a 2-player game
* Supply Victory piles should have 12 in games with > 2 players
* Supply Curse pile should have 10 cards in a 2-player game
* Supply Curse pile should have 20 cards in a 3-player game
* Supply Curse pile should have 30 cards in a 4-player game
* Each player's deck should start with 7 Coppers from the Supply
* Each player's deck should start with 3 Estates not from the Supply
* Trash pile should start out empty
*/

int main() {
  printf("TESTING initializeGame()\n");
  int seed = 1000;
  int k[10] = {adventurer, council_room, feast, gardens, mine
             , remodel, smithy, village, baron, great_hall};
  struct gameState G2, G3, G4;

  printf("Testing 2-player game\n");
  int NUM_PLAYERS = 2;
  initializeGame(NUM_PLAYERS, k, seed, &G2); // initialize a new game

  printf("\texpect Victory piles to have 8 cards\n");
  asserttrue(G2.supplyCount[estate], 8, "G2.supplyCount[estate]", 0);
  asserttrue(G2.supplyCount[duchy], 8, "G2.supplyCount[duchy]", 0);
  asserttrue(G2.supplyCount[province], 8, "G2.supplyCount[province]", 0);

  printf("\texpect Curse pile to have 10 cards\n");
  asserttrue(G2.supplyCount[curse], 10, "G2.supplyCount[curse]", 0);

  printf("\texpect each Player's deck to have 7 Coppers\n");
  int totalCoppers = 0;
  int totalEstates = 0;
  for (size_t i = 0; i < NUM_PLAYERS; i++) {
    for (size_t j = 0; j < G2.deckCount[0]; j++) {
      if (G2.deck[i][j] ==  copper) totalCoppers++;
      if (G2.deck[i][j] ==  estate) totalEstates++;
    }
  }
  asserttrue(totalCoppers, 7 * NUM_PLAYERS, "G2 totalCoppers", 0);
  printf("\texpect each Player's deck to have 3 Estates\n");
  asserttrue(totalEstates, 3 * NUM_PLAYERS, "G2 totalEstates", 0);

  printf("\texpect Trash pile to start out empty\n");
  asserttrue(G2.playedCardCount, 0, "G2.playedCardCount", 0);

  printf("Testing 3-player game\n");
  NUM_PLAYERS = 3;
  initializeGame(NUM_PLAYERS, k, seed, &G3); // initialize a new game

  printf("\texpect Victory piles to have 12 cards\n");
  asserttrue(G3.supplyCount[estate], 12, "G3.supplyCount[estate]", 0);
  asserttrue(G3.supplyCount[duchy], 12, "G3.supplyCount[duchy]", 0);
  asserttrue(G3.supplyCount[province], 12, "G3.supplyCount[province]", 0);

  printf("\texpect Curse pile to have 20 cards\n");
  asserttrue(G3.supplyCount[curse], 20, "G3.supplyCount[curse]", 0);

  printf("Testing 4-player game\n");
  NUM_PLAYERS = 4;
  initializeGame(NUM_PLAYERS, k, seed, &G4); // initialize a new game
  printf("\texpect Curse pile to have 30 cards\n");
  asserttrue(G4.supplyCount[curse], 30, "G4.supplyCount[curse]", 0);
    return 0;
}
