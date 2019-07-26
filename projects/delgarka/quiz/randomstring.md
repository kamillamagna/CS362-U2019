Kamille Delgardo
CS 362 Software Engineering II
Wendy Roberts
28 July 2019
###Random Testing Quiz
The `testme()` function works by maintaining an internal state variable which must reach 9 followed by the string "reset" before it will exit. The state variable starts at zero and is incremented by one when the state-th (zero-eth, first, second, etc.) character in the pattern "[({ ax})]" is detected.
In order to test that state would increment as expected, I utilized the fact that ASCII characters are identifiable by their decimal value. I used rand() to find a random value between 0 and 127 (every valid ASCII char).
In order to get the program to exit() I followed the same pattern but instead of returning the char, used it to build up a word of length 5, then returned it as the string to test against "reset".
The time to increment the state variable was negligible, but the time to build a five-letter word out of 26 lowercase letters was considerably greater.
