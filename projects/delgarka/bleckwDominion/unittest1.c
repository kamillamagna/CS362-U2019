/******************************************************************************
* Kamille Delgardo
* CS 362 Software Engineering II
* Unit Test 1 -- Baron
* 14 July 2019
******************************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

/* Basic requirements of Baron:
* Player should gain one buy
* Player should have 2 choices
* Player should choose only one choice
* If choice1 Player should have an estate in hand to discard
* If choice1 Player should discard an estate
* If choice1 Player should gain 4 coins
* If choice2 Player should gain an Estate to their deck
* If choice2 but there are no Estates left, Player should not gain an Estate
* If choice2 the number of Estates in victory pile should decrease by 1
* Other players should see no state change
* No state change should occur to supply pile, trash pile, kingdom card piles
*/

int main() {
  int seed = 1000;
  int numPlayer = 2;
  int PLAYER = 0;
  int r;
  int k[10] = {adventurer, council_room, feast, gardens, mine
             , remodel, smithy, village, baron, great_hall};
  int deckCount, numBuys, numEstates, numCoins, estateSupply;
  struct gameState G;

  // set up game and player hand
  printf ("TESTING handleBaron():\n");
  r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
  for (size_t i = 3; i < G.handCount[PLAYER]; i++) {
    G.hand[PLAYER][i] = adventurer;
  }
  G.hand[PLAYER][0] = baron;
  G.hand[PLAYER][1] = baron;
  G.hand[PLAYER][2] = baron;
  deckCount = G.deckCount[PLAYER];
  numBuys = G.numBuys;
  numEstates = 0;
  numCoins = G.coins;
  estateSupply = G.supplyCount[estate];

  // test where player has no estates in hand
  printf("Testing player with no estates in hand\n");
  handleBaron(PLAYER, 1, &G);

  // check that an estate has been added to player's deck
  for (size_t i = 0; i < G.deckCount[PLAYER]; i++) {
    if (G.deck[PLAYER][i] == estate) numEstates++;
  }
  printf("\texpect player to have one estate in deck\n");
  asserttrue(numEstates, 1, "numEstates", 0);
  printf("\texpect player to have +1 card in deck\n");
  asserttrue(G.deckCount[PLAYER], deckCount + 1, "G.deckCount[PLAYER]", deckCount);
  // check that player has gained a buy by playing Baron
  printf("\texpect player to have +1 buy\n");
  asserttrue(G.numBuys, numBuys + 1, "G.numBuys", numBuys);
  // check that player has not gained 4 coins
  printf("\texpect player to have same number of coins\n");
  asserttrue(G.coins, numCoins, "G.coins", numCoins);
  // check that number of estates available in Victory pile has decreased by one
  printf("\texpect Victory pile to have one fewer Estate available\n");
  asserttrue(G.supplyCount[estate], estateSupply - 1, "G.supplyCount[estate]", estateSupply);

  // test where player has estates in hand
  G.hand[PLAYER][3] = estate;
  G.hand[PLAYER][4] = estate;
  numBuys = G.numBuys;
  numEstates = 0;
  numCoins = G.coins;
  printf("Testing player with estates in hand\n");
  handleBaron(PLAYER, 1, &G);

  // choice1 --  discard estate to gain 4 coins
  // check that an estate has been removed from player's hand
  numEstates = 0;
  for (size_t i = 0; i < G.handCount[PLAYER]; i++) {
    if (G.hand[PLAYER][i] == estate) numEstates++;
  }
  printf("\texpect player to have one fewer estate in hand\n");
  asserttrue(numEstates, 1, "numEstates", 2);
  // check that player has gained a buy by playing Baron
  printf("\texpect player to have +1 buy\n");
  asserttrue(G.numBuys, numBuys + 2, "G.numBuys", numBuys);
  // check that player has gained 4 coins by playing Baron
  printf("\texpect player to have +4 coins\n");
  asserttrue(G.coins, numCoins + 4, "G.coins", numCoins);

  // test no estates left to gain
  printf("Testing no estates left to gain from Supply\n");
  numEstates = 0;
  for (size_t i = 0; i < G.deckCount[PLAYER]; i++) {
    if (G.deck[PLAYER][i] == estate) numEstates++;
  }
  G.supplyCount[estate] = 0;
  handleBaron(PLAYER, 0, &G); // choice1 = gain estate
  int afterEstates = 0;
  for (size_t i = 0; i < G.deckCount[PLAYER]; i++) {
    if (G.deck[PLAYER][i] == estate) afterEstates++;
  }
  // check that player has gained a buy by playing Baron
  printf("\texpect player to have +1 buy\n");
  asserttrue(G.numBuys, numBuys + 3, "G.numBuys", numBuys);
  // check that a player has not gained an estate
  printf("\texpect player to have same number of estates in deck\n");
  asserttrue(numEstates, afterEstates, "numEstates", numEstates);

  // test no state change to other player

  // test no state change to supply pile, trash pile, kingdom card piles
  return 0;
}
