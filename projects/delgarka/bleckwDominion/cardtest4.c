/******************************************************************************
* Kamille Delgardo
* CS 362 Software Engineering II
* Card Test 4 -- getWinners()
* 14 July 2019
******************************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

/* Basic requirements of getWinners():
* Each player should put all cards into their Deck
* Each player should tally the victory points on all cards in deck
* The player with the most victory points should win
* In case of tie, tied player who has had fewest turns wins
* In case of tie and equal number of turns, all players win
*/

#define N 4

struct gameState setupGetWinnersTest() {
  int seed = 1000;
  int k[10] = {adventurer, council_room, feast, gardens, mine
             , remodel, smithy, village, baron, great_hall};
  struct gameState G;
  initializeGame(N, k, seed, &G);
  return G;
}

int main() {
  printf("TESTING getWinners()\n");
  struct gameState G;
  int players[N];

  printf("\texpect deck to contain all of player's cards\n");
  G = setupGetWinnersTest();
  int handCountPre = G.handCount[0];
  int deckCountPre = G.deckCount[0];
  int discardCountPre = G.discardCount[0];
  getWinners(players, &G);
  asserttrue(G.handCount[0], 0, "G.handCount[0]", handCountPre);
  asserttrue(G.discardCount[0], 0, "G.discardCount[0]", discardCountPre);
  asserttrue(G.deckCount[0], handCountPre + deckCountPre + discardCountPre, "G.deckCount[0]", deckCountPre);

  printf("Testing single player with most victory points\n");
  G = setupGetWinnersTest();
  for (size_t i = 0; i < N; i++) players[i] = -1;
  G.handCount[3]++; // stack player 4's deck
  G.hand[3][G.handCount[3] - 1] = province;
  getWinners(players, &G);
  asserttrue(players[0], 0, "player 1 does not win", 0);
  asserttrue(players[1], 0, "player 2 does not win", 0);
  asserttrue(players[2], 0, "player 3 does not win", 0);
  asserttrue(players[3], 1, "player 4 wins", 0);

  printf("Testing tie between players with most victory points but different numbers of turns\n");
  G = setupGetWinnersTest();
  // Player1 shall have had more turns
  for (size_t i = 0; i < N; i++) {
    players[i] = -1;
    G.handCount[i]++;
    G.hand[i][G.handCount[i] - 1] = province;
  }
  getWinners(players, &G);
  asserttrue(players[0], 1, "player 1 does not win", 0);
  asserttrue(players[1], 0, "player 2 wins", 0);
  asserttrue(players[2], 0, "player 3 wins", 0);
  asserttrue(players[3], 0, "player 4 wins", 0);

  printf("Testing tie between players with most victory points and same numbers of turns\n");
  G = setupGetWinnersTest();
  // Player1 shall have had more turns
  for (size_t i = 0; i < N; i++) {
    players[i] = -1;
    G.handCount[i]++;
    G.hand[i][G.handCount[i] - 1] = province;
  }
  endTurn(&G);
  endTurn(&G);
  endTurn(&G);
  G.whoseTurn = 3;
  getWinners(players, &G);
  asserttrue(players[0], 1, "player 1 wins", 0);
  asserttrue(players[1], 1, "player 2 wins", 0);
  asserttrue(players[2], 1, "player 3 wins", 0);
  asserttrue(players[3], 1, "player 4 wins", 0);
}
