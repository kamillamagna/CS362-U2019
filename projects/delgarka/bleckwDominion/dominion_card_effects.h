#ifndef _DOMINION_CARD_EFFECTS_H
#define _DOMINION_CARD_EFFECTS_H

int _baron( int currentPlayer, int choice1, struct gameState* state);

int _minion(int choice2, int choice1, int handPos, int currentPlayer, struct gameState* state);

int _ambassador(int choice1, int choice2, int handPos, int currentPlayer, struct gameState* state);

int _tribute(int currentPlayer, int nextPlayer, struct gameState* state);

int _mine(int choice1, int choice2, int handPos, int currentPlayer, struct gameState* state);

#endif
