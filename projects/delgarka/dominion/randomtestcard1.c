/******************************************************************************
* Kamille Delgardo
* CS 362 Software Engineering II
* Random Testing 1 -- Baron
* 28 July 2019
******************************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
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

int testBaron(int player, int choice1, struct gameState* post) {
  struct gameState* pre = malloc(sizeof(struct gameState));
  memcpy (pre, post, sizeof(struct gameState));

  // count player estates in hand before Baron play
  int playerEstatesPre = 0;
  for (size_t i = 0; i < post->handCount[player]; i++) if (post->hand[player][i] == estate) playerEstatesPre++;

  // play baron card
  handleBaron(player, choice1, post);

  // count player estates in hand after Baron play
  int playerEstatesPost = 0;
  for (size_t i = 0; i < post->handCount[player]; i++) if (post->hand[player][i] == estate) playerEstatesPost++;

  // test outcomes
  printf("\texpect player to have +1 buy\n");
  asserttrue(post->numBuys, pre->numBuys, "numBuys", pre->numBuys);
  if (choice1) {                       // player choosing to discard an estate
    printf("choice1 (discard an estate) with ");
    if (playerEstatesPre == 0) {       // player has no estates to discard
      printf("no estates to discard:\n");// choice1 should behave like choice2

      // supply estate count should be -1
      printf("\texpect Victory pile to have one fewer Estate available\n");
      asserttrue(post->supplyCount[estate], pre->supplyCount[estate]-1, "post->supplyCount[estate]", pre->supplyCount[estate]);

      // player should gain an estate
      printf("\texpect player to have one more estate in deck\n");
      asserttrue(playerEstatesPost, playerEstatesPre+1, "playerEstates", playerEstatesPre);
    } else {
      printf("1 or more estates in deck to discard:\n");

      // player should gain four coins
      printf("\texpect player to have +4 coins\n");
      asserttrue(post->coins, pre->coins+4, "numCoins", pre->coins);

      // player deck should lose an estate
      printf("\texpect player to have one fewer estate in deck\n");
      asserttrue(playerEstatesPost, playerEstatesPre-1, "playerEstates", playerEstatesPre);

      // player deck count should be -1
      printf("\texpect player to have fewer card in deck\n");
      asserttrue(post->deckCount[player], pre->deckCount[player]-1, "post->deckCount[player]", pre->deckCount[player]);
    }
  } else {                             // player gaining an estate (choice2)
    printf("choice2 (gain an estate) with ");
    if (pre->supplyCount[estate] == 0) {
      printf("no estates left in supply\n");

      // no estates left to gain
      printf("\texpect player to have same number of estates in deck\n");
      asserttrue(playerEstatesPost, playerEstatesPre, "playerEstates", playerEstatesPre);
    } else {
      printf("1 or more estates left in supply\n");

      // supply estate count should be -1
      printf("\texpect Victory pile to have one fewer Estate available\n");
      asserttrue(post->supplyCount[estate], pre->supplyCount[estate]-1, "post->supplyCount[estate]", pre->supplyCount[estate]);

      // player should gain an estate
      printf("\texpect player to have one more estate in deck\n");
      asserttrue(playerEstatesPost, playerEstatesPre+1, "playerEstates", playerEstatesPre);

      // player should not gain four coins as in choice1
      printf("\texpect player to have same number of coins\n");
      asserttrue(post->coins, pre->coins, "numCoins", pre->coins);
    }
  }

  // // test no state change to other player
  //
  // // test no state change to supply pile, trash pile, kingdom card piles
  return 0;
}

int main() {
  int player;
  int choice1;

  printf ("TESTING handleBaron():\n");
  for (size_t i = 0; i < RAND_TESTS; i++) {
    struct gameState G = setupRandomGame();
    int numPlayer = G.numPlayers;
    player = (int)floor(Random() * numPlayer); // randomize current player
    choice1 = (int)floor(Random() * 2);// randomize choice1/choice2

    G.hand[player][0] =  baron;

    // randomize # estates left
    G.supplyCount[estate] = (int)floor(Random() * numPlayer * 10);

    testBaron(player, choice1, &G);
  }
  return 0;
}
