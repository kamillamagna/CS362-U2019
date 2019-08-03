/******************************************************************************
* Kamille Delgardo
* CS 362 Software Engineering II
* Unit Test 5 -- Mine
* 14 July 2019
******************************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

/* Basic requirements of Mine:
* Mine should only work if player has Copper or Silver in hand
* Mine should not add to hand if no supply available of trade-up treasure
* Player hand should gain silver if they have a copper
* Player hand should gain gold if they have a silver
* Mine should not respond to cards other than Copper or Silver
* Player hand should lose treasure for which they trade up
* Supply should lose an instance of the gained treasure
* Trash should gain treasure that player trades in
* Mine should be discarded from player hand upon use
* No state change should occur for other players
* No state change should occur to victory card piles, kingdom card piles
*/

#define P 0
#define N 4

struct gameState setupMineTest() {
  struct gameState G;
  int seed = 1000;
  int k[10] = {adventurer, minion, tribute, gardens, mine
             , remodel, smithy, ambassador, baron, great_hall};
  initializeGame(N, k, seed, &G); // initialize a new game
  G.hand[P][0] = mine;
  return G;
}

int main() {
  int r, handCount, numActions, discardCount;
  struct gameState G = setupMineTest();

  printf("Testing invalid choice1, choice2\n");
  // choice1 is not a copper or a silver
  G.hand[P][0] = estate;
  printf("\texpect calling mine on Victory card to fail\n");
  r = handleMine(estate, gold, 0, P, &G);
  asserttrue(r, -1, "return value of handleMine()", 0);

  // choice2 is not a valid card
  G.hand[P][0] = copper;
  printf("\texpect calling mine on Victory card to fail\n");
  r = handleMine(copper, treasure_map+1, 0, P, &G);
  asserttrue(r, -1, "return value of handleMine()", 0);

  // player trying to trade for a treasure more than one level up
  G.hand[P][0] = copper;
  printf("\texpect calling mine on Victory card to fail\n");
  r = handleMine(copper, gold, 0, P, &G);
  asserttrue(r, -1, "return value of handleMine()", 0);

  printf("Testing valid call to handleMine()\n");

  // check choice1 added to player's discard pile
  int choice1 = copper;
  int choice2 = silver;
  G.hand[P][0] = choice1;
  discardCount = 0;
  handCount = 0;
  int postDiscard = 0;
  int postHand = 0;
  int bonusPre = 0;
  int bonusPost = 0;
  for (size_t i = 0; i < G.discardCount[P]; i++) if (G.discard[P][i] == choice1) discardCount++;
  for (size_t i = 0; i < G.handCount[P]; i++) {
    if (G.hand[P][i] == choice1) handCount++;
    else if (G.hand[P][i] == choice2) bonusPre++;
  }
  handleMine(choice1, silver, 0, P, &G);
  for (size_t i = 0; i < G.discardCount[P]; i++) if (G.discard[P][i] == choice1) postDiscard++;
  for (size_t i = 0; i < G.handCount[P]; i++) {
    if (G.hand[P][i] == choice1) postHand++;
    else if (G.hand[P][i] == choice2) bonusPost++;
  }
  printf("\texpect discard pile to contain +1 choice1 card\n");
  asserttrue(postDiscard, discardCount + 1, "choice1 discardCount", discardCount);
  // check choice1 removed from player's hand
  printf("\texpect hand to contain -1 choice1 card\n");
  asserttrue(postHand, handCount - 1, "choice1 handCount", handCount);

  // check copper for silver adds a silver to player hand
  printf("\texpect hand to contain +1 choice2 card\n");
  asserttrue(bonusPost, bonusPre + 1, "choice2 handCount", bonusPre);
  return 0;
}
