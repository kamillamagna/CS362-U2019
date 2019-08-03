/******************************************************************************
* Kamille Delgardo
* CS 362 Software Engineering II
* Unit Test 3 -- Ambassador
* 14 July 2019
******************************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

/* Basic requirements of Ambassador:
* Player should reveal a card from their hand
* Player should return 0, 1, or 2 copies of a card from hand to Supply pile
* Each other player should gain a copy of revealed card from Supply
* If Supply has fewer than N-1 (M) copies of the card, the M players to the left should gain a copy of that card to their decks
* After play, Supply should contain M fewer copies of revealed card
* After play, Player's hand should contain 0, 1, or 2 fewer copies of revealed card
* After play, M other players' decks should contain 1 more card
* No state change should occur to trash pile, victory card piles, kingdom card piles, or to other players' hands
*/

#define P 0 // current player index
#define N 4 // num players

struct gameState setupAmbassadorTest(int numGardens) {
  struct gameState G;
  int seed = 1000;
  int r;
  int k[10] = {adventurer, minion, feast, gardens, mine
             , remodel, smithy, ambassador, baron, great_hall};
  r = initializeGame(N, k, seed, &G); // initialize a new game

  G.hand[P][0] = ambassador;
  int numAmbs = G.handCount[P] - numGardens;
  for (size_t i = 0; i < numAmbs; i++) G.hand[P][i] = ambassador;
  for (size_t i = numAmbs; i < G.handCount[P]; i++) G.hand[P][i] = gardens;

  return G;
}

int main() {
  int r, deckCount, discardCount, numGardensPre[N], numGardensPost[N];
  struct gameState G = setupAmbassadorTest(4);
  printf ("TESTING handleAmbassador():\n");

  // testing discard 2 gardens with at least 2 in hand
  printf("Testing choice2 discard 2 with 2 in hand\n");
  deckCount = G.deckCount[1];
  discardCount = G.discardCount[1];
  int gardenSupply = G.supplyCount[gardens];
  for (size_t i = 0; i < N; i++) {
    numGardensPre[i] = 0;
    numGardensPost[i] = 0;
    for (size_t j = 0; j < G.handCount[i]; j++) if (G.hand[i][j] == gardens) numGardensPre[i]++;
  }

  // discard 2 copies of garden card at position 1 in in hand
  handleAmbassador(gardens, 2, 4, P, &G);
  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < G.handCount[i]; j++) if (G.hand[i][j] == gardens) numGardensPost[i]++;
  }
  // check numGardens in players hand decreased by 2
  printf("\texpect player to have 2 fewer gardens in hand\n");
  asserttrue(numGardensPost[P], numGardensPre[P] - 2, "numGardens[0]", numGardensPre[P]);
  // check other players' hands contain at least 1 copy of garden after play
  for (size_t i = 1; i < N; i++) {
    printf("\texpect player%i to have +1 gardens in hand\n", i+1);
    asserttrue(numGardensPost[i], numGardensPre[i] + 1, "numGardens[i]", numGardensPre[i]);
  }
  // check Supply of gardens has been reduced by 1
  printf("\texpect supply pile to have 1 fewer gardens (3 distributed - 2 discarded)\n");
  asserttrue(G.supplyCount[gardens], gardenSupply - 1, "G.supplyCount[gardens]", gardenSupply);

  // testing discard 2 gardens with 1 in hand
  printf("\texpect attempt discard 2 with 1 in hand to fail\n");
  G = setupAmbassadorTest(1);
  r = handleAmbassador(gardens, 2, 4, P, &G);
  asserttrue(r, -1, "handleAmbassador return value", 0);

  // testing attempt to discard -1 gardens
  printf("\texpect attempt discard < 0 to fail\n");
  G = setupAmbassadorTest(4);
  r = handleAmbassador(gardens, -1, 4, P, &G);
  asserttrue(r, -1, "handleAmbassador return value", 0);

  // testing play with 0 gardens available in Supply
  return 0;
}
