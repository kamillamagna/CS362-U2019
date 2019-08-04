/******************************************************************************
* Kamille Delgardo
* CS 362 Software Engineering II
* Random Testing 3 -- Tribute
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

#ifndef RAND_TESTS
#define RAND_TESTS 200
#endif

/* Basic requirements of Tribute:
* Tribute card should apply to player to immediate left
* Target should reveal top 2 cards in their deck
* Target should reveal no more or less than 2 cards
* Target should not reveal cards from any other location but top of deck
* After play, Target's discard pile should contain the two revealed cards
* After play, Target's deck should not contain the two revealed cards
* If revealed cards are the same, player should receive one bonus
* If revealed cards are different, player should receive bonus for each
* If the card is a curse it should produce no bonus
* If the card has two types, it should give both bonuses
* Player should receive +2 actions for an Action card revealed
* Player should receive +2 coins for a Treasure card revealed
* Player should receive +2 cards for a Victory card revealed
* If target has fewer than 2 cards left in their deck, they should reveal all remaining cards in their deck, shuffle their discard pile, then reveal the remainded needed
* If target does not have enough cards to reveal 2, they should reveal what they can
* No state change should occur for players other than Target and Player
* No state change should occur to supply pile, trash pile, victory card piles, kingdom card piles
*/


int testTribute(int player, struct gameState* post, int* tributeRevealedCards) {
  int target = player == post->numPlayers - 1 ? 0 : player+1;
  int curseCount = 0;
  int victoryCount = 0;
  int treasureCount = 0;
  int actionCount = 0;
  struct gameState* pre = malloc(sizeof(struct gameState));
  memcpy (pre, post, sizeof(struct gameState));

  // play Tribute card
  _tribute(player, target, post, tributeRevealedCards);


  // test outcomes
  printf ("TESTING handleTribute():\n");
  printf("Testing target has 1 in deck to reveal\n");

  // check case where player has 1 card in discard to reveal
  printf("Testing target has 1 in discard to reveal\n");

  // check case were target has 0 cards to reveal
  printf("Testing target has no cards to reveal\n");



  if (pre->deckCount[target] < 2) {
    /* code */
  } else { // cases where target has at least 2 cards to reveal
    printf("Testing target has 2 or more in deck to reveal\n");
    int k = 2;
    if (tributeRevealedCards[0] == tributeRevealedCards[1]) k = 1;
    for (size_t i = 0; i < k; i++) {
      if (tributeRevealedCards[i] < curse || tributeRevealedCards[i] > treasure_map) {
        return -1;
      }
      else if (tributeRevealedCards[i] == 0) curseCount++; // curse, gain nothing
      else if (tributeRevealedCards[i] < 4) victoryCount++; // victory, gain +2 cards
      else if (tributeRevealedCards[i] < 7) treasureCount++; // treasure, gain +2 coins
      else actionCount++; // action, gain +2 actions
    }
  }

  if (actionCount && curseCount) { // case action + curse
    printf("\texpect action and curse combo to add +2 actions\n");
    asserttrue(post->numActions, pre->numActions + 2, "G.numActions", pre->numActions);
  }
  else if (actionCount && victoryCount) { // case action + victory
    printf("\texpect action and victory combo to add +2 actions, +2 cards\n");
    asserttrue(post->numActions, pre->numActions + 2, "G.numActions", pre->numActions);
    asserttrue(post->handCount[player], pre->handCount[player] + 2, "post->handCount[player]", pre->handCount[player]);
  }
  else if (actionCount && treasureCount) { // case action + treasure
    printf("\texpect action and treasure combo to add +2 actions, +2 coins\n");
    asserttrue(post->numActions, pre->numActions + 2 , "G.numActions", pre->numActions);
    asserttrue(post->coins, pre->coins + 2, "post->coins", pre->coins);
  }
  else if (curseCount && treasureCount) { // case curse + treasure
    printf("\texpect curse and treasure combo to add +2 coins\n");
    asserttrue(post->coins, pre->coins + 2, "post->coins", pre->coins);
  }
  else if (curseCount && victoryCount) { // case curse + victory
    printf("\texpect curse and victory combo to add +2 cards\n");
    asserttrue(post->handCount[player], pre->handCount[player] + 2, "post->handCount[player]", pre->handCount[player]);
  }
  else if (treasureCount && victoryCount) {  // case treasure + victory
    printf("\texpect treasure and victory combo to add +2 coins and +2 cards\n");
    asserttrue(post->coins, pre->coins + 2 , "post->coins", pre->coins);
    asserttrue(post->handCount[player], pre->handCount[player] + 2, "post->handCount[player]", pre->handCount[player]);
  }
  else if (treasureCount > 1) { // case treasure + treasure
    printf("\texpect 2 different treasures to add +4 coins\n");
    asserttrue(post->coins, pre->coins + 2, "post->coins", pre->coins);
  }
  else if (actionCount > 1) { // case action + action
    printf("\texpect 2 different treasures to add +4 actions\n");
    asserttrue(post->coins, pre->coins + 2, "post->coins", pre->coins);
  }
  else if (victoryCount > 1) { // case victory + victory
    printf("\texpect 2 different victories to add +4 cards\n");
    asserttrue(post->handCount, pre->handCount + 4, "handCount", pre->handCount);
  }
  else if (curseCount && !victoryCount && !treasureCount && !actionCount) {
    printf("\texpect 2 curses to have no bonus\n");
    asserttrue(post->handCount[player], pre->handCount[player], "post->handCount[player]", pre->handCount[player]);
    asserttrue(post->numActions, pre->numActions, "G.numActions", pre->numActions);
    asserttrue(post->coins, pre->coins, "post->coins", pre->coins);
  }
  else if (!curseCount && victoryCount && !treasureCount && !actionCount) {
    printf("\texpect 2 same victories to add +2 cards\n");
    asserttrue(post->handCount[player], pre->handCount[player] + 2, "post->handCount[player]", pre->handCount[player]);
  }
  else if (!curseCount && !victoryCount && treasureCount && !actionCount) {
    printf("\texpect 2 same treasures to add +2 coins\n");
    asserttrue(post->coins, pre->coins + 2, "post->coins", pre->coins);
  }
  else if (!curseCount && !victoryCount && !treasureCount && actionCount) {
    printf("\texpect 2 same actions to add +2 actions\n");
    asserttrue(post->numActions, pre->numActions + 2, "G.numActions", pre->numActions);
  }
  // // test no state change to other player
  //
  // // test no state change to supply pile, trash pile, kingdom card piles
  return 0;
}

int main() {
  int player;
  int choice1;

  printf ("TESTING handleTribute():\n");
  for (size_t i = 0; i < RAND_TESTS; i++) {
    PlantSeeds(-1);                      // seed based on internal clock
    int seed = floor(Random() * MAX_RAND); // seed between 0 and 999999
    SelectStream(seed);                  // select stream from gen'd streams
    PutSeed((long)seed);

    struct gameState G = setupRandomGame();
    int numPlayer = G.numPlayers;
    int tributeRevealedCards[] = {-1, -1};
    player = (int)floor(Random() * numPlayer); // randomize current player
    int target = player == numPlayer - 1 ? 0 : player+1;

    G.hand[player][0] = tribute; // make tribute next-played card

    // randomize # cards in tribute deck, discard
    G.deckCount[target] = (int)floor(Random() * 11);
    G.discardCount[target] = (int)floor(Random() * 11);
    // randomize cards in tribute deck, discard
    for (size_t i = 0; i < G.deckCount[target]; i++) G.deckCount[i] = (int)floor(Random() * treasure_map);
    for (size_t i = 0; i < G.discardCount[target]; i++) G.discardCount[i] = (int)floor(Random() * treasure_map);

    testTribute(player, &G, tributeRevealedCards);
  }
  return 0;
}
