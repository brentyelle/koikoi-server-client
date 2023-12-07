# koikoi-server-client
A server &amp; client pair for playing the hanafuda game Koi-Koi (text only).

## HOW TO COMPILE & RUN

On a Linux terminal, running the following commands will re-compile the code:

    $ make clean
    $ make

The server can be run by doing:

    $ ./hserver.out [portname]

And the client can be run by doing:

    $ ./hclient.out [hostname] [portname]

Presumably, the hostname will be `localhost`.

## HANAFUDA CARDS

Koi-Koi is a 2-player game played with a hanafuda deck, which consists of a deck with 12 suits (one for each month) of 4 cards each, for a total of 48 cards. The cards in each month can be LIGHT, SEED, RIBBON, or CHAFF type, and each suit has a different assortment of each. In total, the deck has 5 Lights, 9 Seeds, 10 ribbons, and 24 chaff.

## GAME RULES

### SETUP:
 1. The dealer is chosen: The first dealer is chosen randomly, whereas in all subsequent rounds, the winner of the previous round becomes the dealer for the next round.
 2. The dealer gives 8 cards to the other player, then 8 face-up to the table, then finally 8 to himself. (Players hide their hands from each other.)
 3. The remaining 24 cards are set face-down in a draw pile on the table.
 4. The players check their hands and the table for any instant-win conditions:
     - having all 4 cards of a suit
     - having 4 pairs of cards of matching suits
    If the table has either of these, then it is considered a misdeal, and the dealing is redone.
    If a player has either of these, then that player instantly wins the round and scores 6 points.
    (In the improbable case where multiple instant-win triggers happen simultaneously, the player(s) still score their points.)
 5. If no instant-win condition was met, then the round begins.
    Play generally proceeds for a predetermined number of rounds, often up to 12.

### TURNS:
 1. The player tries to match a card from his hand to a face-up card on the table.
     -  If he can, then both cards are placed in his score pile (face-up in front of himself).
     -  If he cannot, then he takes a card from his hand and adds it (face-up) to the table's cards.
 2. The player then draws a card from the deck and sees if it matches a face-up card on the table.
     -  If he can, then both cards are placed in his score pile.
     -  If he cannot, then the card from the deck is added to the table's cards.
 3. The player then checks his score pile to see if any new Combos (see below) have been made this turn. If so, then he MUST make a choice:
     A. He can end the round, cashing out the combo(s) in his score pile for points.
     B. He can continue the round by saying "Koi-Koi".
 4. If the round has not been ended, then play proceeds to the other player.

### MATCHING:
 -  Two cards are considered to "match" if they have the same month.
 -  The Lightning card (November Chaff) is "wild", meaning it can be used from the hand or deck to match any card on the table.
 -  However, if the Lightning card is on the table, it can only be matched normally, i.e., by other November cards.
 -  If three cards of a Month are present on the table, all three can be matched at once by the fourth card.

### SCORING & COMBOS:
 -  If a player's score pile has 7 or more points' worth in it, then if that player wins the round, he doubles his final score.
 -  If a player wins after his opponent has called Koi-Koi at least once, then he doubles his final score.
 -  (The two conditions can both stack, meaning that the final score can be quadrupled.)
-   The Light Combos (marked with * above) are mutually exclusive, meaning that a player can only score one of them for a round. All other Combos above can overlap freely.
-   If a player upgrades from one Light Combo to another, or if they increase the value of their Many Seeds / Many Ribbons / Many Chaff combo with another card, the player is still required to decide between ending the round or calling Koi-Koi.

#### LIGHT COMBOS
    *   Five Lights         15      all five Lights (Crane, Curtain, Moon, Rain Man, Phoenix)
    *   Dry Four Lights     8       four Lights, where none of the Lights is Rain Man
    *   Rainy Four Lights   7       four Lights, where one of the Lights is Rain Man
    *   Three Lights        6       any three Lights

#### SEED COMBOS
    -   Ino-Shika-Cho           5       Boar, Deer, and Butterfly
    -   Many Seeds              1+      1 point for 5 seeds, plus 1 additional point for every additional seed

#### RIBBON COMBOS
    -   Poetry Ribbons          5       all three Poetry Ribbons
    -   Blue Ribbons            5       all three Blue Ribbons
    -   Many Ribbons            1+      1 point for 5 ribbons, plus 1 additional point for every additional ribbon

#### CHAFF COMBOS
    -   Many Chaff              1+      1 point for 10 chaff, plus 1 additional point for every additional chaff
    -   No Chaff                10      score pile does not have even a single chaff card

#### OTHER COMBOS
    -   Sakura Viewing          5       Sake Cup & Curtain, but cannot have a "Rainy" card (Rain Man or Lightning) in the score pile
    -   Moon Viewing            5       Sake Cup & Moon, but cannot have a "Rainy" card (Rain Man or Lightning) in the score pile
    -   Four-of-a-Kind          4       all four cards of the same month
