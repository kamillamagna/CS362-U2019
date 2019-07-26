/******************************************************************************
* Kamille Delgardo
* CS 362 Software Engineering II
* Unit Test 2 -- Minion
* 14 July 2019
******************************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

/* Basic requirements of Minion:
* Player should gain one action
* Player should have two options
* Player should gain two coins if choice1
* Player and all players with >= 5 cards in hand should discard their hand and draw 4 cards if choice2
* If choice2 player should end up with 4 cards in hand
* If choice2 players with < 5 cards in hand should not see state change
* If choice1 other players should see no state change
* No state change should occur to supply pile, trash pile, victory card piles, kingdom card piles
*/

int main() {
  int seed = 1000;
  int numPlayer = 2;
  int PLAYER = 0;
  int r;
  int k[10] = {adventurer, minion, feast, gardens, mine
             , remodel, smithy, village, baron, great_hall};
  int deckCount, numCoins, handCount, numActions, discardCount;
  struct gameState G;
  printf ("TESTING handleMinion():\n");
  r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
  for (size_t i = 0; i < G.handCount[PLAYER]; i++) {
    G.hand[PLAYER][i] = minion;
  }

  // test choice1 activated -- +2 coins
  printf("Testing choice1 activated\n");
  numActions = G.numActions;
  numCoins = G.coins;
  handCount = G.handCount[1];
  deckCount = G.deckCount[1];
  discardCount = G.discardCount[1];
  handleMinion(0, 1, 0, PLAYER, &G);
  // check numActions increased by 1
  printf("\texpect player to have +1 actions (+2 from card, -1 from act of playing)\n");
  asserttrue(G.numActions, numActions + 1, "G.numActions", numActions);
  // check numCoins increased by 2
  printf("\texpect player to have +2 coins\n");
  asserttrue(G.coins, numCoins + 2, "G.coins", numCoins);
  // check no state change to other players
  printf("\texpect other player to have cards (no state change)\n");
  asserttrue(G.handCount[1], handCount, "G.handCount[1]", handCount);
  asserttrue(G.deckCount[1], deckCount, "G.deckCount[1]", deckCount);
  asserttrue(G.discardCount[1], discardCount, "G.discardCount[1]", discardCount);

  // test choice2 activated -- discard hand and draw 4 cards
  handleMinion(0, 0, 1, PLAYER, &G);
  // check numActions increased by 1
  printf("\texpect player to have +1 actions (+2 from card, -1 from act of playing)\n");
  asserttrue(G.numActions, numActions + 2, "G.numActions", numActions);
  // check numCoins remains the same
  printf("\texpect player to have same number of coins\n");
  asserttrue(G.coins, numCoins, "G.coins", numCoins);
  // check other player has 4 cards after play
  handCount = G.handCount[1];
  printf("\texpect other player to have 4 cards in hand\n");
  asserttrue(G.handCount[1], 4, "G.handCount[1]", handCount);
  return 0;
}
