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
  int playerEstatesPre = 0;
  for (size_t i = 0; i < post->handCount[player]; i++) if (post->hand[player][i] == estate) playerEstatesPre++;
  // printf("PRE: \n");
  // printf("player: %i/%i\n", player, pre->numPlayers);
  // printf("\tchoice1: %i\n", choice1);
  // printf("\tnumBuys: %i\n", pre->numBuys);
  // printf("\tplayerEstates: %i\n", playerEstatesPre);
  // printf("\tnumCoins: %i\n", pre->coins);
  // printf("\testateSupply: %i\n", pre->supplyCount[estate]);
  // printf("\tG->deckCount[player]: %i\n", pre->deckCount[player]);
  handleBaron(player, choice1, post);
  int playerEstatesPost = 0;
  for (size_t i = 0; i < post->handCount[player]; i++) if (post->hand[player][i] == estate) playerEstatesPost++;
  // printf("POST: \n");
  // printf("player: %i/%i\n", player, post->numPlayers);
  // printf("\tnumBuys: %i\n", post->numBuys);
  // printf("\tplayerEstates: %i\n", playerEstatesPost);
  // printf("\tnumCoins: %i\n", post->coins);
  // printf("\testateSupply: %i\n", post->supplyCount[estate]);
  // printf("\tG.deckCount[player]: %i\n", post->deckCount[player]);

  printf("player should gain a buy upon Baron play\n");
  asserttrue(post->numBuys, pre->numBuys, "numBuys", pre->numBuys);
  if (choice1) {                       // player choosing to discard an estate
    printf("choice1 (discard an estate) with ");
    if (playerEstatesPre == 0) {       // player has no estates to discard
      printf("no estates to discard:\n");// choice1 should behave like choice2
      asserttrue(playerEstatesPost, playerEstatesPre+1, "playerEstates", playerEstatesPre);
    } else {
      printf("1 or more estates in deck to discard:\n");
      // player should gain four coins
      asserttrue(post->coins, pre->coins+4, "numCoins", pre->coins);
      // player deck should lose an estate
      asserttrue(playerEstatesPost, playerEstatesPre-1, "playerEstates", playerEstatesPre);
      // player deck count should be -1
      asserttrue(post->deckCount[player], pre->deckCount[player]-1, "post->deckCount[player]", pre->deckCount[player]);
    }
  } else {                             // player gaining an estate (choice2)
    printf("choice2 (gain an estate) with ");
    if (pre->supplyCount[estate] == 0) { // no estates left to gain
      printf("no estates left in supply\n");
      /* code */
    } else {
      printf("1 or more estates left in supply\n");
      // supply estate count should be -1
      asserttrue(post->supplyCount[estate], pre->supplyCount[estate]-1, "post->supplyCount[estate]", pre->supplyCount[estate]);
      // player should gain an estate
      asserttrue(playerEstatesPost, playerEstatesPre+1, "playerEstates", playerEstatesPre);
    }
  }

  // // test where player has no estates in hand
  // printf("Testing player with no estates in hand\n");
  // handleBaron(PLAYER, 1, &G);
  //
  // // check that an estate has been added to player's deck
  // for (size_t i = 0; i < G.deckCount[PLAYER]; i++) {
  //   if (G.deck[PLAYER][i] == estate) numEstates++;
  // }
  // printf("\texpect player to have one estate in deck\n");
  // asserttrue(numEstates, 1, "numEstates", 0);
  // printf("\texpect player to have +1 card in deck\n");
  // asserttrue(G.deckCount[PLAYER], deckCount + 1, "G.deckCount[PLAYER]", deckCount);
  // // check that player has gained a buy by playing Baron
  // printf("\texpect player to have +1 buy\n");
  // asserttrue(G.numBuys, numBuys + 1, "G.numBuys", numBuys);
  // // check that player has not gained 4 coins
  // printf("\texpect player to have same number of coins\n");
  // asserttrue(G.coins, numCoins, "G.coins", numCoins);
  // // check that number of estates available in Victory pile has decreased by one
  // printf("\texpect Victory pile to have one fewer Estate available\n");
  // asserttrue(G.supplyCount[estate], estateSupply - 1, "G.supplyCount[estate]", estateSupply);
  //
  // // test where player has estates in hand
  // G.hand[PLAYER][3] = estate;
  // G.hand[PLAYER][4] = estate;
  // numBuys = G.numBuys;
  // numEstates = 0;
  // numCoins = G.coins;
  // printf("Testing player with estates in hand\n");
  // handleBaron(PLAYER, 1, &G);
  //
  // // choice1 --  discard estate to gain 4 coins
  // // check that an estate has been removed from player's hand
  // numEstates = 0;
  // for (size_t i = 0; i < G.handCount[PLAYER]; i++) {
  //   if (G.hand[PLAYER][i] == estate) numEstates++;
  // }
  // printf("\texpect player to have one fewer estate in hand\n");
  // asserttrue(numEstates, 1, "numEstates", 2);
  // // check that player has gained a buy by playing Baron
  // printf("\texpect player to have +1 buy\n");
  // asserttrue(G.numBuys, numBuys + 2, "G.numBuys", numBuys);
  // // check that player has gained 4 coins by playing Baron
  // printf("\texpect player to have +4 coins\n");
  // asserttrue(G.coins, numCoins + 4, "G.coins", numCoins);
  //
  // // test no estates left to gain
  // printf("Testing no estates left to gain from Supply\n");
  // numEstates = 0;
  // for (size_t i = 0; i < G.deckCount[PLAYER]; i++) {
  //   if (G.deck[PLAYER][i] == estate) numEstates++;
  // }
  // G.supplyCount[estate] = 0;
  // handleBaron(PLAYER, 0, &G); // choice1 = gain estate
  // int afterEstates = 0;
  // for (size_t i = 0; i < G.deckCount[PLAYER]; i++) {
  //   if (G.deck[PLAYER][i] == estate) afterEstates++;
  // }
  // // check that player has gained a buy by playing Baron
  // printf("\texpect player to have +1 buy\n");
  // asserttrue(G.numBuys, numBuys + 3, "G.numBuys", numBuys);
  // // check that a player has not gained an estate
  // printf("\texpect player to have same number of estates in deck\n");
  // asserttrue(numEstates, afterEstates, "numEstates", numEstates);
  //
  // // test no state change to other player
  //
  // // test no state change to supply pile, trash pile, kingdom card piles
  return 0;
}

int main() {
  int player;
  int choice1;

  printf ("TESTING handleBaron():\n");
  for (size_t i = 0; i < 2; i++) {
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
