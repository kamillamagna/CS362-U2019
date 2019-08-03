/******************************************************************************
* Kamille Delgardo
* CS 362 Software Engineering II
* Card Test 3 -- endTurn()
* 14 July 2019
******************************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

/* Basic requirements of endTurn():
* Played Action and Treasure cards should be placed in discard pile
* Player should discard remaining cards from hand
* Player should draw five cards for their next hand
* NumActions and numBuys should be reset to 1
* outpostPlayed, phase, and playedCardCount should be reset to 0
* coins should be reset to 3
* Play should pass to the next player in line
*/
#define P 0
#define N 2

struct gameState setupEndTurnTest() {
  int seed = 1000;
  int k[10] = {adventurer, council_room, feast, gardens, mine
             , remodel, smithy, village, baron, great_hall};
  struct gameState G;

  initializeGame(N, k, seed, &G);

  return G;
}

int main() {
  printf("TESTING endTurn()\n");
  struct gameState G;

  printf("\texpect play to move to next player in line (0 -> 1)\n");
  G = setupEndTurnTest();
  int whoseTurnPre = G.whoseTurn;
  endTurn(&G);
  asserttrue(G.whoseTurn, whoseTurnPre + 1, "G.whoseTurn", whoseTurnPre);

  printf("\texpect play to move to next player in line (1 -> 0)\n");
  whoseTurnPre = G.whoseTurn;
  endTurn(&G);
  asserttrue(G.whoseTurn, whoseTurnPre - 1, "G.whoseTurn", whoseTurnPre);

  printf("\texpect discard pile to contain remaining cards from hand\n");

  printf("\texpect player 1 to draw 5 cards for their next hand\n");
  asserttrue(G.handCount[1], 5, "G.handCount[1]", G.handCount[1]);

  printf("\texpect outpostPlayed to be reset to 0\n");
  G = setupEndTurnTest();
  G.outpostPlayed = 99;
  int outpostPre = G.outpostPlayed;
  endTurn(&G);
  asserttrue(G.outpostPlayed, 0, "G.outpostPlayed", outpostPre);

  printf("\texpect coins to be initialized to 3 at start of turn\n");
  G = setupEndTurnTest();
  G.coins = 99;
  int coinsPre = G.coins;
  endTurn(&G);
  asserttrue(G.coins, 3, "G.coins", coinsPre);

  printf("\texpect phase to be reset to 0\n");
  G = setupEndTurnTest();
  G.phase = 1;
  int phasePre = G.phase;
  endTurn(&G);
  asserttrue(G.phase, 0, "G.phase", phasePre);

  printf("\texpect playedCardCount to be reset to 0\n");
  G = setupEndTurnTest();
  G.playedCardCount = 33;
  int playedCardCountPre = G.playedCardCount;
  endTurn(&G);
  asserttrue(G.playedCardCount, 0, "G.playedCardCount", playedCardCountPre);

  printf("\texpect numActions to be reset to 1\n");
  G = setupEndTurnTest();
  G.numActions = 8;
  int numActionsPre = G.numActions;
  endTurn(&G);
  asserttrue(G.numActions, 1, "G.numActions", numActionsPre);

  printf("\texpect numBuys to be reset to 1\n");
  G = setupEndTurnTest();
  G.numBuys = 3;
  int numBuysPre = G.numBuys;
  endTurn(&G);
  asserttrue(G.numBuys, 1, "G.numBuys", numBuysPre);

  return 0;
}
