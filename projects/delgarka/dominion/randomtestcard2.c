/******************************************************************************
* Kamille Delgardo
* CS 362 Software Engineering II
* Random Testing 2 -- Minion
* 14 July 2019
******************************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
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

int testMinion(int player, int choice1, struct gameState* post) {
  struct gameState* pre = malloc(sizeof(struct gameState));
  memcpy (pre, post, sizeof(struct gameState));

  // play minion card
  handleMinion(0, choice1, player, post);

  // test outcomes
  // check numActions increased by 1
  printf("\texpect player to have +1 actions (+2 from card, -1 from act of playing)\n");
  asserttrue(post->numActions, pre->numActions + 1, "numActions", post->numActions);

  if (choice1) {  // player chooses coin/action increase
    printf("choice1 (gain actions and coins)\n");

    // check numCoins increased by 2
    printf("\texpect player to have +2 coins\n");
    asserttrue(post->coins, post->coins + 2, "post->coins", pre->coins);

    // check no state change to other players
    printf("\texpect other player to have same cards (no state change)\n");
    for (size_t i = 0; i < pre->numPlayers; i++) {
      printf("\t player %li\n", i+1);
      asserttrue(post->handCount[i], pre->handCount[i], "post->handCount[i]", pre->handCount[i]);
      asserttrue(post->deckCount[i], pre->deckCount[i], "post->deckCount[i]", pre->deckCount[i]);
      asserttrue(post->discardCount[i], pre->discardCount[i], "post->discardCount[i]", pre->discardCount[i]);
    }
  } else { // player chooses discard and draw 4
    printf("choice2 (players with >= 5 cards in hand discard hand and draw 4)\n");

    // check numCoins remains the same
    printf("\texpect player to have same number of coins\n");
    asserttrue(post->coins, pre->coins, "numCoins", post->coins);

    // check other player has +4 cards after play
    printf("\texpect other player to have +4 cards in hand\n");
    for (size_t i = 0; i < pre->numPlayers; i++) {
      if (pre->handCount[i] >= 5) {
        asserttrue(post->handCount[i], 4, "post->handCount[i]", pre->handCount[i]);
      } else {
        // check no state change to other players
        printf("\texpect other player to have same cards (no state change)\n");
        for (size_t i = 0; i < pre->numPlayers; i++) {
          printf("\t player %li\n", i+1);
          asserttrue(post->handCount[i], pre->handCount[i], "post->handCount[i]", pre->handCount[i]);
          asserttrue(post->deckCount[i], pre->deckCount[i], "post->deckCount[i]", pre->deckCount[i]);
          asserttrue(post->discardCount[i], pre->discardCount[i], "post->discardCount[i]", pre->discardCount[i]);
        }
      }
    }

  }
  return 0;
}

int main() {
  int player;
  int choice1;

  printf ("TESTING handleMinion():\n");
  for (size_t i = 0; i < RAND_TESTS; i++) {
    struct gameState G = setupRandomGame();
    int numPlayer = G.numPlayers;
    player = (int)floor(Random() * numPlayer); // randomize current player
    choice1 = (int)floor(Random() * 2);// randomize choice1/choice2

    G.hand[player][0] =  minion;

    // randomize # estates left
    G.supplyCount[estate] = (int)floor(Random() * numPlayer * 10);

    testMinion(player, choice1, &G);
  }
  return 0;
}
