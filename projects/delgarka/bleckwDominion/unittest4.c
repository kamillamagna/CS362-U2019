/******************************************************************************
* Kamille Delgardo
* CS 362 Software Engineering II
* Unit Test 4 -- Tribute
* 14 July 2019
******************************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

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

#define P 0
#define Q 1
#define N 4

struct gameState setupTributeTest(int* tributeRevealedCards) {
  struct gameState G;
  int seed = 1000;
  int k[10] = {adventurer, minion, tribute, gardens, mine
             , remodel, smithy, ambassador, baron, great_hall};
  initializeGame(N, k, seed, &G); // initialize a new game
  tributeRevealedCards[0] = -1;
  tributeRevealedCards[1] = -1;

  G.hand[P][0] = tribute;
  return G;
}

int main() {
  int tributeRevealedCards[2] = {-1, -1};
  struct gameState G = setupTributeTest(&tributeRevealedCards);
  int actionsPre = G.numActions;
  int coinsPre = G.coins;
  int cardsPre = G.discardCount[P];
  printf ("TESTING handleAmbassador():\n");
  printf("Testing target has 1 in deck to reveal\n");
  // check case where player has 1 card in deck to reveal
  G.deck[Q][0] = copper;
  handleTribute(Q, P, &G, &tributeRevealedCards);

  // check case where player has 1 card in discard to reveal
  printf("Testing target has 1 in discard to reveal\n");

  // check case were target has 0 cards to reveal
  printf("Testing target has no cards to reveal\n");


  // cases where target has at least 2 cards to reveal
  printf("Testing target has 2 or more in deck to reveal\n");

  // case action + curse
  printf("\texpect action and curse combo to add +2 actions\n");
  G = setupTributeTest(&tributeRevealedCards);
  tributeRevealedCards[0] = -1;
  tributeRevealedCards[1] = -1;
  actionsPre = G.numActions;
  coinsPre = G.coins;
  cardsPre = G.handCount[P];
  G.deck[Q][0] = mine;
  G.deck[Q][1] = curse;
  handleTribute(Q, P, &G, &tributeRevealedCards);
  asserttrue(G.numActions, actionsPre + 2, "G.numActions", actionsPre);

  // case action + victory
  printf("\texpect action and victory combo to add +2 actions, +2 cards\n");
  G = setupTributeTest(&tributeRevealedCards);
  tributeRevealedCards[0] = -1;
  tributeRevealedCards[1] = -1;
  actionsPre = G.numActions;
  coinsPre = G.coins;
  cardsPre = G.handCount[P];
  G.deck[Q][0] = steward;
  G.deck[Q][1] = duchy;
  handleTribute(Q, P, &G, &tributeRevealedCards);
  asserttrue(G.numActions, actionsPre + 2, "G.numActions", actionsPre);
  asserttrue(G.handCount[P], cardsPre + 2, "G.handCount[P]", cardsPre);

  // case action + treasure
  printf("\texpect action and treasure combo to add +2 actions, +2 coins\n");
  G = setupTributeTest(&tributeRevealedCards);
  tributeRevealedCards[0] = -1;
  tributeRevealedCards[1] = -1;
  actionsPre = G.numActions;
  coinsPre = G.coins;
  cardsPre = G.handCount[P];
  G.deck[Q][0] = steward;
  G.deck[Q][1] = duchy;
  handleTribute(Q, P, &G, &tributeRevealedCards);
  asserttrue(G.numActions, actionsPre + 2 , "G.numActions", actionsPre);
  asserttrue(G.coins, coinsPre + 2, "G.coins", coinsPre);

  // case curse + treasure
  printf("\texpect curse and treasure combo to add +2 coins\n");
  G = setupTributeTest(&tributeRevealedCards);
  tributeRevealedCards[0] = -1;
  tributeRevealedCards[1] = -1;
  actionsPre = G.numActions;
  coinsPre = G.coins;
  cardsPre = G.handCount[P];
  G.deck[Q][0] = curse;
  G.deck[Q][1] = gold;
  handleTribute(Q, P, &G, &tributeRevealedCards);
  asserttrue(G.coins, coinsPre + 2, "G.coins", coinsPre);

  // case curse + victory
  printf("\texpect curse and victory combo to add +2 cards\n");
  G = setupTributeTest(&tributeRevealedCards);
  tributeRevealedCards[0] = -1;
  tributeRevealedCards[1] = -1;
  actionsPre = G.numActions;
  coinsPre = G.coins;
  cardsPre = G.handCount[P];
  G.deck[Q][0] = curse;
  G.deck[Q][1] = duchy;
  handleTribute(Q, P, &G, &tributeRevealedCards);
  asserttrue(G.handCount[P], cardsPre + 2, "G.handCount[P]", cardsPre);

  // case treasure + victory
  printf("\texpect treasure and victory combo to add +2 coins and +2 cards\n");
  G = setupTributeTest(&tributeRevealedCards);
  tributeRevealedCards[0] = -1;
  tributeRevealedCards[1] = -1;
  actionsPre = G.numActions;
  coinsPre = G.coins;
  cardsPre = G.handCount[P];
  G.deck[Q][0] = silver;
  G.deck[Q][1] = estate;
  handleTribute(Q, P, &G, &tributeRevealedCards);
  asserttrue(G.coins, coinsPre + 2 , "G.coins", coinsPre);
  asserttrue(G.handCount[P], cardsPre + 2, "G.handCount[P]", cardsPre);

  // case treasure + treasure
  printf("\texpect 2 same treasures to add +2 coins\n");
  G = setupTributeTest(&tributeRevealedCards);
  tributeRevealedCards[0] = -1;
  tributeRevealedCards[1] = -1;
  actionsPre = G.numActions;
  coinsPre = G.coins;
  cardsPre = G.handCount[P];
  G.deck[Q][0] = silver;
  G.deck[Q][1] = silver;
  handleTribute(Q, P, &G, &tributeRevealedCards);
  asserttrue(G.coins, coinsPre + 2, "G.coins", coinsPre);

  // case action + action
  printf("\texpect 2 same actions to add +2 actions\n");
  G = setupTributeTest(&tributeRevealedCards);
  tributeRevealedCards[0] = -1;
  tributeRevealedCards[1] = -1;
  actionsPre = G.numActions;
  coinsPre = G.coins;
  cardsPre = G.handCount[P];
  G.deck[Q][0] = minion;
  G.deck[Q][1] = minion;
  handleTribute(Q, P, &G, &tributeRevealedCards);
  asserttrue(G.numActions, actionsPre + 2, "G.numActions", actionsPre);

  // case victory + victory
  printf("\texpect 2 same victories to add +2 cards\n");
  G = setupTributeTest(&tributeRevealedCards);
  tributeRevealedCards[0] = -1;
  tributeRevealedCards[1] = -1;
  actionsPre = G.numActions;
  coinsPre = G.coins;
  cardsPre = G.handCount[P];
  G.deck[Q][0] = duchy;
  G.deck[Q][1] = duchy;
  handleTribute(Q, P, &G, &tributeRevealedCards);
  asserttrue(G.handCount[P], cardsPre + 2, "G.handCount[P]", cardsPre);

  // case curse + curse
  printf("\texpect 2 curses to have no bonus\n");
  G = setupTributeTest(&tributeRevealedCards);
  tributeRevealedCards[0] = -1;
  tributeRevealedCards[1] = -1;
  actionsPre = G.numActions;
  coinsPre = G.coins;
  cardsPre = G.handCount[P];
  G.deck[Q][0] = curse;
  G.deck[Q][1] = duchy;
  handleTribute(Q, P, &G, &tributeRevealedCards);
  asserttrue(G.handCount[P], cardsPre, "G.handCount[P]", cardsPre);
  asserttrue(G.numActions, actionsPre, "G.numActions", actionsPre);
  asserttrue(G.coins, coinsPre, "G.coins", coinsPre);
  return 0;
}
