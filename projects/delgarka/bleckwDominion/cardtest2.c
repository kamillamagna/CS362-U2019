/******************************************************************************
* Kamille Delgardo
* CS 362 Software Engineering II
* Card Test 2 -- shuffle()
* 14 July 2019
******************************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

/* Basic requirements of shuffle():
* Cards should not be in same order in deck after shuffle()
*/

int main() {
  int seed = 1000;
  int numPlayer = 2;
  int k[10] = {adventurer, minion, feast, gardens, mine
             , remodel, smithy, village, baron, great_hall};

  struct gameState G, G2;
  printf ("TESTING shuffle():\n");
  printf("Testing shuffle does not result in same deck pre and post\n");

  initializeGame(numPlayer, k, seed, &G); // initialize a new game
  memcpy (&G2, &G, sizeof(struct gameState));
  shuffle(0, G.deck[0]);

  int same = memcmp(&G, &G2, sizeof(struct gameState));
  printf("\texpect memory of pre and post shuffle of player 1 deck to be different\n");
  asserttrue(same, 0, "memory comparison pre and post", 1);
}
