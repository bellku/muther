/***********************************************************
*  Program:        Project 2 (Go Fish)                     *
*  Author:         Kurtis Bell                             *
*                                                          *
*  Description:    Text based Go Fish card game            *
*                                                          *
*  Input:          Card requests, menu options, etc        *
*  Output:         Wins, points, game statistics, etc      *
***********************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>

#define handLimit 52
#define dealLimit 10
#define deckLimit 52
#define cardLimit 13 // (i.e. 13 face values)
#define SHUF 100000  // Shuffling constant

int deck[deckLimit];      // > The deck array
int userHand[handLimit];  // > The User's Hand
int userPairs[cardLimit]; // > userPair Pile
int compHand[handLimit];  // > The Computer's Hand
int compPairs[cardLimit]; // > compPair Pile
int currentWins[2];       // > Tracks current wins
int turnFlag[2];          // > Tracks which turn it is
int deckCount = 0;        // > Tracks the numbers of cards in the deck

int userStats[10];  // [0] = points
                    // [1] = Number of cards in a hand
                    // [9] = Remembers the suit of a matched card
                    // [8] = flag used if a pair is made after the player draws a card
                    // [3] = Misc. flag
                    // [5] = Only used in compStats -- > Counts cards in memory
int compStats[10];

// > compAI() Tools:

int memory[handLimit]; // The computer's memory stores what cards the user has in his hand.
int memCheck;          // Stores the face value of a user's paired card
int difficulty;        // This value is compared with values in memory[]
int cheatCode = 0;     // The cheat code identifier

// Function Prototypes

void popDeck(void); // > Populates the Deck of Cards
void shuffle(void); // > Shuffles the deck
void pause(void);   // > Pauses game -- waits for user input
int getCard(void);  // > Retrieves the top card off the deck
void deal(void);    // > Deals, in an oscillatory manner, the cards to the players
void printNumber(int);  // > Prints the face value of the card
void printSuit (int);   // > Prints the suit of the card
void displayHand(int hand[], int player, int stats[]);  // > Displays a player's hand
void removePairs(int hand[], int stats[], int pos);     // > Removes pairs from hand
void findPairs(int hand[], int stats[], int pairs[]);   // > Finds pairs in hand
void userTurn(void);    // > the user's entire turn
void printPoints(int player); // > prints out the points for a player
void compTurn(void);          // > the computer's entire turn
void emptyArray(int array[], int playFlag);  // > BLANK becomes every value in array
void gameOver(int status);    // > Detirmines the winner
int gameStatus(void);         // > Flag type function returns if the game is over
void displayWins(void);       // > Displays the current and previous wins
void trackWins(void);         // > Records and updates wins in a text file
void newGame(void);           // > Starts a new game
int next(void);               // > Asks users if they want to save and quit or continue
void playAgain(void);         // > Decides if another game will execute
void saveGame(void);          // > Saves all information in one text file
void loadGame(void);          // > Loads all information from a text file
void updateMemory(int check); // > Updates the computer's AI memory
int compAI(void);             // > AI logic for the computer
void cheatMode(void);         // > Executes cheat mode if the code is entered
void code(void);              // > Contains the section for which cheat code is entered
void endCheats(void);         // > Detirmines if the cheat mode is active or inactive
void printMemory(void);       // > Prints what the AI memory contains
int memoryFix(int card);      // > Refreshes the memory if particular events occur

const int BLANK = -1;         // > Occupies an emptied array slot

int main()
{
    srand((int)time(NULL) + 1);

    int opt;  // option flag
    int error; // error checking flag

    printf("\n****************************\n");
    printf("          Go Fish            ");
    printf("\n****************************\n\n\n");

    printf("1 - {New Game}\n");   // > MENU
    printf("2 - {Load Game}\n");
    printf("3 - {Exit Game}\n\n");
    PlaySound("e:\\GoFish\\dp_bomberman.wav", NULL, SND_SYNC);   // Opening Theme Music

    do{
    printf("Option: ");   // > Do-while Loop is for error checking the option input
    scanf("%d", &opt);

    if((opt != 1) && (opt != 2) && (opt != 3))
    {
        error = 1;
        printf("\n\n*** Error!  Invalid Option! ***\n\n");   // Error Checking input
    }
    else
        error = 0;
    }while(error == 1);

    if(opt == 1)   // > Begins new game
        newGame();
    if(opt == 2)   // > Loads previous game data
        loadGame();
    if(opt == 3)
    {
        PlaySound("e:\\GoFish\\shut_off.wav", NULL, SND_SYNC);  // > Exit music
        exit(0);  // Exits the program
    }
    return 0;
}

void popDeck()  // Populates the deck
{
    int i;

    for(i = 0; i < deckLimit; i++)
    {
        deck[i] = i;  // cards 0 through 51 (i.e. 52 cards)
        deckCount++;  // counts the number of cards in the deck
    }
}

void shuffle()  // Shuffles the deck
{
    int i, temp, pos1, pos2;

    for(i = 0; i < SHUF; i++)  // Switches two random cards 'SHUF' times
    {
        pos1 = (rand() % deckLimit);
        pos2 = (rand() % deckLimit);

        temp = deck[pos1];     // > Position switching
        deck[pos1] = deck[pos2];
        deck[pos2] = temp;
    }
}

int getCard()  // Retrieves the top card off the deck
{
    int i;
    int card;

    card = deck[0];  // Top card

    for(i = 0; i < deckCount; i++)  // shifts every card up one slot in array
        deck[i] = deck[i + 1];

    deck[51] = BLANK; // replaces the last card as a BLANK card

    deckCount--;   // decrements the the number of cards in deck by 1

    return card;  // returns the card
}

void deal()  // deals out the cards to each player
{
    int i;

    for(i = 0; i < dealLimit; i++)
    {
        userHand[i] = getCard();  // gives card to user
        userStats[1]++;    // counts the number of cards in user hand
        compHand[i] = getCard(); // gives card to the computer
        compStats[1]++;   // counts the number of cards in comp hand
    }
}

void printNumber(int card)  // prints the number for a card
{
    switch (card % 13)
    {
    case 0:
        printf ("King");
        break;
    case 1:
        printf ("Ace");
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        printf ("%d", card % 13);
        break;
    case 11:
        printf ("Jack");
        break;
    case 12:
        printf ("Queen");
        break;
    }
}

void printSuit (int card)  // Detirmines the suit of the card
{
    if (card / 13 == 0)
        printf ("%c\n", 6);
    if (card / 13 == 1)
        printf ("%c\n", 5);
    if (card / 13 == 2)
        printf ("%c\n", 4);
    if (card / 13 == 3)
        printf ("%c\n", 3);
}

void displayHand(int hand[], int player, int stats[]) // Displays a player's hand
{
    int i;

    if(player == 1)  // player 1 is the user
    {
        printf("\n**********************\n");
        printf("     USER's Hand");
        printf("\n**********************\n");
    }
    if(player == 2)  // player 2 is the computer
    {
        printf("\n**********************\n");
        printf("   COMPUTER's Hand");
        printf("\n**********************\n");
    }

    for(i = 0; i < stats[1]; i++)
    {
        if(hand[i] == BLANK)  // Skips BLANK slots
            continue;

        printNumber(hand[i]); // prints face
        printf(" of ");
        printSuit(hand[i]);   // prints the suit
    }
}

void findPairs(int hand[], int stats[], int pairs[]) // Finds pairs within a player's hand
{
    int i, k;
    int pairFound = 0;  // pair found flag

    for(i = 9; i >= 0; i--)
    {                                 // double for loop compares face values for matches
        for(k = 0; k < stats[1]; k++)
            if((hand[i] % 13 == hand[k] % 13) && (hand[i] != hand[k]) && (pairFound != 2))
            {
                pairs[hand[i] % 13]++;         // > Put in Pair Pile
                stats[9] = hand[k];            // > For printing a suit in turn

                if((turnFlag[0] == 1) && (difficulty == 1))
                {
                    memCheck = (hand[i] % 13);  // Records paired cards if AI is active
                    updateMemory(memCheck);    //  Refreshes the AI memory bank
                }

                removePairs(hand, stats, i);   // > Removes paired cards from hand
                removePairs(hand, stats, k);
                stats[0]++;                    // > Adds a point
                pairFound++;
            }
        pairFound = 0; // resets the pair flag
    }
}

void removePairs(int hand[], int stats[], int pos)  // removes the pairs from the hand
{
    int j;

    for(j = pos; j < (stats[1]); j++)  // shifts every card in the hand
    {                                  // up one slot starting from the position
        hand[j] = hand[j + 1];         // of the removed card
    }
    hand[(stats[1] - 1)] = BLANK;  // assigns the BLANK position
    stats[1] -= 1;                 // Updates number of cards in a player's hand
}

void pause()  // 'Pauses' game play
{
    double pause;

    printf("\n\n**********************************");
    printf("\nPress any number to continue: ");
    scanf("%lf", &pause);    // waits for arbitrary (number) user input
    printf("**********************************\n\n");
}

void userTurn()  // The user's entire turn executes in the following
{
    int card, error, repeat; // card, and flags
    int k;

    printf("\n\n\nIt is the USER's Turn: \n");

    if(userStats[1] == 0) // i.e. if there are no cards in the hand, the draw a card
    {
        if(deckCount == 0) // if there are no cards in the deck, detirmine if there is a winner
            gameOver(gameStatus());
        else
        {
            userHand[0] = getCard();  // Else, retrieve a card
            userStats[1]++;  // Incrementing the number of cards in a hand
        }
    }

    displayHand(userHand, 1, userStats);  // Displaying the user hand
    pause();   //  Pauses the game temporarily


    do
    {
        printf("\nUSER:  Do you have a ... :\n\n");
        printf("Press 0 - {King}, 1 - {Ace}\n");
        printf("     2 through 10 - {2 ... 10}\n");
        printf("     11 - {Jack}, 12 -{Queen} :   ");
        scanf("%d", &card);

        error = 1;
        for(k = 0; k < userStats[1]; k++)  // Error checking
        {
            if((card % 13) != (userHand[k] % 13))
                continue;
            else
            {
                error = 0;
                k = userStats[1];
            }
        }
        if (error == 1)
        {
            printf("\n\n*** Pick a card you can match with! ***\n");  // error message
            repeat = 1;
        }
        else
            repeat = 0;
    }
    while(repeat == 1);

    // > Now the COMP checks if it has that card...

    printf("\n\nCOMPUTER:  Commencing search algorithm. . .\n\n");

    int y;
    int match = 0;
    int temp;
    int pos;
    for(y = 0; y < compStats[1]; y++)          // Computer looks for a match in his hand
        if(card % 13 == compHand[y] % 13)
        {
            match = 1;     // match flag
            temp = compHand[y];  // records the card
            pos = y;             // recrods position in the hand
            break;   // Exits the loop
        }
    printf("\nCOMPUTER:  Search complete.");
    pause();

    // > Match = 1 ?  >> YES! >>> Match = 0 ? >> GO FISH!!

    if(match == 0)
    {
        if((difficulty == 1) && (cheatCode != -1234)) // If AI is active then the computer
        {                                             // remembers the card the user "fished" on
            for(y = 0; y < handLimit; y++)
            {
                if(memory[y] != BLANK)  // Finds an empty spot in the memory bank
                    continue;
                else
                {
                    if(memoryFix(card) != 1)  // records the memorized card
                    {
                        memory[y] = card;
                        compStats[5]++;
                        break;
                    }
                    else
                        continue;
                }
             }
        }

        printf("\n\n");
        printf("COMPUTER: GO FISH!\n");
        PlaySound("e:\\GoFish\\splash-02.wav", NULL, SND_SYNC);  // Spalsh noise

        printf("\nDraw a card!");
        pause();


        userHand[userStats[1]] = getCard();  // User draws a card

        printf("\n\nCard Drawn : ");
        printNumber(userHand[userStats[1]]);  // prints the card out
        printf(" of ");
        printSuit(userHand[userStats[1]]);

        userStats[1]++;   // increments the number of cards in the hand
        displayHand(userHand, 1, userStats);  // displays the hand
        findPairs(userHand, userStats, userPairs);  // finds pairs in the hand

        if(userStats[1] == 0)
        {
            if(deckCount == 0)
                gameOver(gameStatus());  //  Detirmines if the game is about to end
            else
            {
                userHand[0] = getCard();   // if there are no cards then draw a card
                userStats[1]++;   // increments the number of cards in hand
                printf("\nMatch Made!!\nUSER draws a card. . .\n");
                printf("\n\nCard Drawn : ");
                printNumber(userHand[0]); // prints the matched cards if needed
                printf(" of ");
                printSuit(userHand[0]);
            }
        }

        printPoints(1);  // prints out the user's points

        pause();

    }
    if(match == 1)  // If a match is made
    {
        printf("\n\n");
        printf("COMPUTER: Hmm... It appears that I do have a ");
        printNumber(card);
        printf(".\n");

        removePairs(compHand, compStats, pos); // removes that card from the computer
        userHand[userStats[1]] = temp;    // gives the card to the user
        userStats[1]++;  // increments the number of user cards
        findPairs(userHand, userStats, userPairs); // makes the match and removes the card

        printf("\nGOOD JOB!!\n\n");
        printf("\nMatch made: ");
        printNumber(temp);
        printf(" of ");
        printSuit(temp);
        printf("            ");
        printNumber(card);
        printf(" of ");
        printSuit(userStats[9]);
        pause();

// > if userStats[1] == 0 then draw a card!!!!
        if(userStats[1] == 0)
        {
            printf("\nYou have no cards left!\n"); // check if deckCount == 0
            if(deckCount == 0)
                gameOver(gameStatus());  // game status is called if there are no cards
            else
            {
                printf("\n\nDraw a card.");
                pause();

                userHand[userStats[1]] = getCard(); // retrieves a card if there are cards
                printf("\n\nCard Drawn : ");
                printNumber(userHand[userStats[1]]);
                printf(" of ");
                printSuit(userHand[userStats[1]]);
                userStats[1]++;
            }
        }
        printf("\n  Updated USER Hand: \n");  // Updated user hand at the end of turn
        printf("***********************\n\n");
        displayHand(userHand, 0, userStats);
        printPoints(1);  // prints the points
        pause();
    }

    // End of USER turn
    turnFlag[0] = 2; // > Flag says it is Computer's Turn
}

void printPoints(int player) // Prints out the points for a player
{
    if(player == 1)
    {
        printf("\n\nUSER's Points: %d\n", userStats[0]);
    }
    if(player == 2)
    {
        printf("\n\nCOMPUTER's Points: %d\n", compStats[0]);
    }
}

void compTurn()  // The computer's entire turn
{
    int j;
    int card;

    printf("\n\nIt's the COMPUTER'S turn.\n");

    if(compStats[1] == 0)  // draws a card if there aren't any in hand
    {
        if(deckCount == 0)
            gameOver(gameStatus());
        else
        {
            compHand[0] = getCard();
            compStats[1]++;
        }
    }
    #ifdef DISPLAY_COMP_HAND
    displayHand(compHand, 2, compStats);  // displays hand at beginning of turn
    #endif
    if(difficulty == 0)  // if AI is not active
        pause();
    if(difficulty == 1 && (cheatCode != -1234))  // If AI is active and cheat mode isn't
    {
        code(); // code entry screen
        if(cheatCode == -1234)  // if cheat code is entered...
        {
            printf("\n\n*** Commencing Superman Vision ***\n\n");
            PlaySound("e:\\GoFish\\laugh_x.wav", NULL, SND_SYNC);
            cheatMode();  // enters the cheat mode and the computer peaks at user hand
        }
        if(memory[0] != BLANK)
        {
            printMemory();  // prints what the computer remembers
            pause();
        }
    }
    else
        if(difficulty == 1 && (cheatCode == -1234)) // if the cheats are on then....
        {
            endCheats();        // > Exits cheatMode if the code is entered in again.
            if(cheatCode == -1234)
            {
                cheatMode();     // > Cheat Mode Continues if cheatCode is still active
                printMemory();   // > after endCheats() is called
            }
        }

    card = compAI();  // AI detirmines which card the computer will request

    printf("\nCOMPUTER:  Do you have a ");
    printNumber(card);   // asks for requested card
    printf("?");

    int match = 0;
    int pos;
    int temp;
    for(j = 0; j < userStats[1]; j++)  // user looks for that card in his hand
    {
        if(card % 13 == userHand[j] % 13)
        {
            match = 1;
            pos = j;            // recording position of matched card and its type
            temp = userHand[j];
            break;
        }
    }

    pause();

    if(match == 0)   // If no match exists, then computer "fishes"
    {
        printf("\n\n");
        printf("USER:  GO FISH!!\n\n");
        PlaySound("e:\\GoFish\\splash-02.wav", NULL, SND_SYNC);
        printf("\nThe COMPUTER draws a card.\n");
        pause();

        compHand[compStats[1]] = getCard();   // Retrieves a card
        #ifdef DISPLAY_COMP_HAND
        printf("\nCard Drawn: ");
        printNumber(compHand[compStats[1]]);
        printf(" of ");
        printSuit(compHand[compStats[1]]);
        printf("\n\n");
        #endif

        compStats[1]++;                          // Updates number of cards in hand
        #ifdef DISPLAY_COMP_HAND
        displayHand(compHand, 2, compStats);    // displays hand and find any pairs
        #endif
        findPairs(compHand, compStats, compPairs);

        if(compStats[1] == 0)            // if there are no cards then the comp draws one
        {
            printf("\n\nThe computer ran out of cards.\n");
            if(deckCount == 0)
                gameOver(gameStatus());
            else
            {
                compHand[0] = getCard();
                compStats[1]++;
                #ifdef DISPLAY_COMP_HAND
                printf("Card Drawn: ");
                printNumber(compHand[0]);
                printf(" of ");
                printSuit(compHand[0]);
                #else
                printf("\nThe computer drew a card.\n");
                #endif
            }
        }

        printPoints(2);  // displays the computer's points

        pause();
    }
    if(match == 1)  // If a match is made
    {
        printf("\n\n");
        printf("USER: Dag-Nabit!  I do have a ");   // prints the matched card
        printNumber(card);
        printf(".\n\n");

        removePairs(userHand, userStats, pos);  // user removes card
        compHand[compStats[1]] = temp;       // the computer stores that card
        compStats[1]++;                      // Adds one to number of cards in hand
        findPairs(compHand, compStats, compPairs);  // matches them up
        updateMemory(card);                  // updates the memory if needed

        printf("\nCOMPUTER'S Match made: ");
        printNumber(temp);
        printf(" of ");
        printSuit(temp);
        printf("                       ");
        printNumber(card);
        printf(" of ");
        printSuit(compStats[9]);
        pause();

        printPoints(2);  // displays the updated computer points

        if(compStats[1] == 0)  // if there are no cards in the hand
        {
            printf("\nThe computer ran out of cards.\n");
            if(deckCount == 0)
                gameOver(gameStatus());  // detirmines if the game is over
            else
            {
                compHand[0] = getCard();  // retrieves a card
                compStats[1]++;
                #ifdef DISPLAY_COMP_HAND
                printf("Card Drawn: ");
                printNumber(compHand[0]);
                printf(" of ");
                printSuit(compHand[0]);
                #else
                printf("\nThe computer drew a card\n");
                #endif
            }
        }
        pause();
    }

    turnFlag[0] = 1; // > Flag says it is the USER's turn
}

void emptyArray(int array[], int playFlag)  // empties out arrays
{
    int i;

    if(playFlag == 0)     // empties hands and memory arrays
        for(i = 0; i < handLimit; i++)
            array[i] = BLANK;
    if(playFlag == 1)    // empties the deck
        for(i = 0; i < dealLimit; i++)
            array[i] = 0;
    if(playFlag == 2)    // empties the pair piles
        for(i = 0; i < cardLimit; i++)
            array[i] = 0;
}

int gameStatus()  // detirmines if all the pairs have been made
{
    int sum = 0;
    int status = 0;

    if(deckCount == 0)
    {
        sum = (userStats[0] + compStats[0]);

        if(sum == 26)
            status = 1;
    }

    return status;  // returns status flag
}

void gameOver(int status)
{
    int play;
    int error = 1;

    if(status == 1) // Means the game is over
    {
        printPoints(1);
        printPoints(2);

        if(userStats[0] > compStats[0])  // user wins
        {
            printf("\n\nCongratulations!  You've won the game!\n\n");
            currentWins[0]++; // > Recroding wins since execution
            trackWins();
            PlaySound("e:\\GoFish\\dp_galaga88_bonus.wav", NULL, SND_SYNC);
        }
        if(compStats[0] > userStats[0]) // comp wins
        {
            printf("\n\nLOSER!!\n\n");
            currentWins[1]++; // > Recording wins since execution
            trackWins();
            PlaySound("e:\\GoFish\\bubbling1.wav", NULL, SND_SYNC);
        }
        if(compStats[0] == userStats[0]) // tie game
            printf("\n\nTie game!\n\n");

        printf("*** End of Game ***\n\n");

        while(error == 1)
        {
            printf("\n\n1 - {Play Again}   0 - {Exit Game} : "); // play again?
            scanf("%d", &play);

            if((play == 1) || (play == 0))
                error = 0;
            if((play != 1) && (play != 0))
                printf("\nERROR!  Invalid entry!\n\n");  // eror checking
        }

        if(play == 0)
        {
            PlaySound("e:\\GoFish\\bubbling1.wav", NULL, SND_SYNC);
            exit(0);
        }
        if(play == 1)
            playAgain();  // calls play again
    }
}

void displayWins()  // displays previus and current wins
{
    FILE* rp;
    int status;
    rp = fopen("e:\\GoFish\\prevWins.txt", "r");  // reads in past wins
    status = fscanf(rp, "%d", &userStats[3]);
    while(status != EOF)
    {
        status = fscanf(rp, "%d", &compStats[3]);
    }

    printf("\n*********************************\n");
    printf("   Wins since Current Execution");
    printf("\n*********************************\n\n");

    printf("  USER Wins:  %d\n", currentWins[0]);
    printf("  COMP Wins:  %d\n", currentWins[1]);

    printf("\n*********************************\n");
    printf("  Wins from Previous Executions");
    printf("\n*********************************\n\n");

    printf("  USER Wins:  %d\n", userStats[3]);
    printf("  COMP Wins:  %d\n", compStats[3]);

    fclose(rp);
}

void trackWins()  // writes to the prev wins file
{
    FILE *fp;
    FILE *up;
    int upUser = 0;
    int upComp = 0;
    int status;

    up = fopen("e:\\GoFish\\prevWins.txt", "r");
    status = fscanf(up, "%d", &upUser);
    while(status != EOF)
        status = fscanf(up, "%d", &upComp);
    fclose(up);

    upUser += currentWins[0];  // adding new win to prevWins
    upComp += currentWins[1];

    fp = fopen("e:\\GoFish\\prevWins.txt", "w");
    fprintf(fp, "%d %d", upUser, upComp);
    fclose(fp);
}

void newGame() // New Game begins
{
    displayWins();
    pause();

    if(turnFlag[1] == 0)
    {
        int error = 1;
        while(error == 1)
        {
            printf("\n\nSelect Difficulty: \n");
            printf("0 - {Normal}   1 - {Hard - AI} : ");
            scanf("%d", &difficulty);

            if((difficulty != 0) && (difficulty != 1))  // Difficulty is selected
            {
                printf("\n\nError!  Invalid input!\n\n");
                error = 1;
            }
            else
                error = 0;
        }

        turnFlag[0] = 1; // > Says that the USER begins a New Game
        emptyArray(userHand, 0);  // initializing the arrays
        emptyArray(compHand, 0);
        emptyArray(memory, 0);

        popDeck();
        shuffle();
        deal();
        printf("\nThe deck has been shuffled!\n");
        printf("The cards have been dealt to each player!\n\n");
        pause();

        displayHand(userHand, 1, userStats);
        printf("\n\n");
        pause();
        #ifdef DISPLAY_COMP_HAND
        displayHand(compHand, 2, compStats);
        pause();
        #endif

        findPairs(userHand, userStats, userPairs);
        findPairs(compHand, compStats, compPairs);
        printf("\nRemove any pairs from your hands!\n");
        pause();
        printf("\n  Updated USER Hand: \n");
        printf("***********************\n\n");
        displayHand(userHand, -1, userStats);
        pause();
        #ifdef DISPLAY_COMP_HAND
        printf("\n  Updated COMP Hand: \n");
        printf("***********************\n\n");
        displayHand(compHand, -1, compStats);
        #endif
    }
    if(turnFlag[1] == 1)  // if the game executed was loaded
    {

        printf("\n**************************\n");
        printf("     Loaded Game Data");
        printf("\n**************************\n");
        #ifdef DISPLAY_COMP_HAND
        displayHand(compHand, 2, compStats);
        pause();
        #endif
        displayHand(userHand, 1, userStats);
    }
    printPoints(1);
    printPoints(2);
    pause();

    int again = 0;
    while(again == 0)   // The game flow loop
    {
        if(turnFlag[0] == 1)
        {
            userTurn();  // > Calling the user's turn

            again = next();  // continue or save?
            if(again == 1)
                saveGame();
        }
        if(turnFlag[0] == 2) // Computer turn flag
        {
            compTurn();

            again = next(); // continue or exit?
            if(again == 1)
                saveGame();
        }
    }
}

void saveGame()  // Saves all game data to one file
{
    FILE *sp;
    int i;

    sp = fopen("e:\\GoFish\\savedGames.txt", "w");

    for(i = 0; i < handLimit; i++)
        fprintf(sp, "%d ", userHand[i]);
    fprintf(sp, "\n\n");
    for(i = 0; i < dealLimit; i++)
        fprintf(sp, "%d ", userStats[i]);
    fprintf(sp, "\n\n");
    for(i = 0; i < cardLimit; i++)
        fprintf(sp, "%d ", userPairs[i]);
    fprintf(sp, "\n\n");

    fprintf(sp, "%d\n", currentWins[0]);
    fprintf(sp, "\n\n");

    for(i = 0; i < handLimit; i++)
        fprintf(sp, "%d ", compHand[i]);
    fprintf(sp, "\n\n");
    for(i = 0; i < dealLimit; i++)
        fprintf(sp, "%d ", compStats[i]);
    fprintf(sp, "\n\n");
    for(i = 0; i < cardLimit; i++)
        fprintf(sp, "%d ", compPairs[i]);
    fprintf(sp, "\n\n");

    fprintf(sp, "%d\n", currentWins[1]);
    fprintf(sp, "\n\n");

    fprintf(sp, "\n\n");
    for(i = 0; i < deckLimit; i++)
        fprintf(sp, "%d ", deck[i]);

    fprintf(sp, "\n\n");

    for(i = 0; i < 2; i++)
        fprintf(sp, "%d ", turnFlag[i]);

    fprintf(sp, "\n\n");
    fprintf(sp, "%d", deckCount);
    fprintf(sp, "\n\n");
    fprintf(sp, "%d", difficulty);
    fprintf(sp, "\n\n");
    fprintf(sp, "%d", memCheck);
    fprintf(sp, "\n\n");
    fprintf(sp, "%d", cheatCode);
    fprintf(sp, "\n\n");

    for(i = 0; i < handLimit; i++)
        fprintf(sp, "%d ", memory[i]);

    fclose(sp);
    exit(0);
}

void loadGame()  // Loads all game data from one text file
{
    FILE *lp;
    int i;

    lp = fopen("e:\\GoFish\\savedGames.txt", "r");

    for(i = 0; i < handLimit; i++)
        fscanf(lp, "%d", &userHand[i]);

    for(i = 0; i < dealLimit; i++)
        fscanf(lp, "%d", &userStats[i]);

    for(i = 0; i < cardLimit; i++)
        fscanf(lp, "%d", &userPairs[i]);

    fscanf(lp, "%d", &currentWins[0]);

    for(i = 0; i < handLimit; i++)
        fscanf(lp, "%d", &compHand[i]);

    for(i = 0; i < dealLimit; i++)
        fscanf(lp, "%d", &compStats[i]);

    for(i = 0; i < cardLimit; i++)
        fscanf(lp, "%d", &compPairs[i]);

    fscanf(lp, "%d", &currentWins[1]);

    for(i = 0; i < deckLimit; i++)
        fscanf(lp, "%d ", &deck[i]);

    for(i = 0; i < 2; i++)
        fscanf(lp, "%d", &turnFlag[i]);

    fscanf(lp, "%d", &deckCount);
    fscanf(lp, "%d", &difficulty);
    fscanf(lp, "%d", &memCheck);
    fscanf(lp, "%d", &cheatCode);

    for(i = 0; i < handLimit; i++)
        fscanf(lp, "%d", &memory[i]);

    turnFlag[1] = 1;  // flags new game that game was loaded
    fclose(lp);

    newGame();  // begins the loaded game with above data
}

int next()  // decides if the game will continue or save and exit
{
    int ans;
    int error = 1;

    while(error == 1)
    {
        printf("\n1 - Save and Quit    0 - Continue Game : ");
        scanf("%d", &ans);

        if((ans == 1) || (ans == 0))    // error checking input
            error = 0;
        if(ans == 1)
            PlaySound("e:\\GoFish\\bubbling1.wav", NULL, SND_SYNC);

        if((ans != 1) && (ans != 0))
            printf("\nERROR!  Invalid entry!\n\n");
    }
    return ans;
}

void playAgain()   // if the game replays then all arrays are reinitialized
{
    emptyArray(userStats, 1);
    emptyArray(compStats, 1);
    emptyArray(userPairs, 2);
    emptyArray(compPairs, 2);
    emptyArray(memory, 0);
    turnFlag[1] = 0;
    newGame();
}

int compAI()   // Computer Intelligence Function
{
    int flag = 0;
    int temp = 0;
    int hold;
    int pos;
    int i, j, k;
    int card;

    if(difficulty == 1)      // Difficulty:   HARD  (AI Active)
    {
        for(i = 9; i >= 0; i--)
        {
            for(k = 0; k < compStats[5]; k++)   // Searches for memorized user cards in compHand
            {
                if((compHand[i] % 13) == memory[k] && (memory[k] != BLANK))
                {
                    card = memory[k];
                    temp = 1;   // If such a card exists, then request that card
                    pos = k;
                    break;
                }
            }
            if(temp == 1)
                break;
        }
        if(temp == 1)
        {
            for(j = pos; j < compStats[5]; j++)   // Refreshes the memory
                                                 //if the card exists in hand
                memory[i] = memory[i + 1];
            compStats[5]--;
        }

        if(temp == 0)   // If such a card does not exist, then pick a random card from hand
        {
            while(flag == 0)
            {
                hold = rand() % 13;
                for(j = 0; j < compStats[1]; j++)
                {
                    if(hold % 13 == compHand[j] % 13)
                    {
                        card = compHand[j];
                        flag = 1;
                    }
                }
            }
        }
    }


    if(difficulty == 0)     // Difficulty:   EASY
    {
        while(flag == 0)   // Picks a random card from the compHand
        {
            hold = rand() % 13;  // Doesn't memorize the user's "fished" cards
            for(j = 0; j < compStats[1]; j++)
            {
                if(hold % 13 == compHand[j] % 13)
                {
                    card = compHand[j];
                    flag = 1;
                }
            }
        }
    }
    return card;
}

void printMemory()  // Prints out stored comp memory or cheat memory
{
    int i;

    if(cheatCode != -1234)
    {
        printf("\n**********************\n");
        printf("   COMPUTER Memory");
        printf("\n**********************\n");
    }
    else
    {
        printf("\n**********************\n");  // Cheated "memory"
        printf("    Superman Vision");
        printf("\n**********************\n");
    }


    for(i = 0; i <= compStats[5]; i++)  // Printing loop
    {
        printNumber(memory[i]);
        printf("\n");
    }
}

void updateMemory(int check)  // Updates the comp memory if user makes a pair with
{                             // a memorized card
    int i;
    int temp;
    int flag = 0;

    for(i = 0; i < compStats[5]; i++)
    {
        if(check == memory[i])  // Finds the memorized card that has been matched
        {
            temp = i;
            flag = 1;
            compStats[5]--;  // updates the number of cards in memory
            break;
        }
        if(flag == 1)
            break;
    }

    if(flag == 1)
    {
        for(i = temp; i < handLimit; i++)
            memory[i] = memory[i + 1];

        memory[51] = BLANK;
    }
}

int memoryFix(int card)  // fixes the memory slots if the user
{                        // makes a pair with a memorized card at the
    int fix = 0;         // end of a turn
    int i;

    for(i = 0; i < handLimit; i++)
        if(card == memory[i])
            fix = 1;
    return fix;
}

void cheatMode()  // The computer peaks at the user's hand
{
    int i;

    compStats[5] = 0;    // These cards get recorded in AI memory[]

    for(i = (userStats[1] - 1); i >= 0; i--)
    {
        memory[i] = (userHand[i] % 13);
        compStats[5]++;
    }

}

void code()  // Hidden cheat code message cue
{
    printf("\n\n**********************************");
    printf("\nPress any number to continue:: ");
    scanf("%d", &cheatCode);
    printf("**********************************\n\n");
}

void endCheats()  // Detirmines if the cheat mode should continue or not
{
    int temp;

    printf("\n\n**********************************");
    printf("\nPress any number to continue:: ");
    scanf("%d", &temp);
    printf("**********************************\n\n");

    if(temp == -1234)
    {
        emptyArray(memory, 0);
        cheatCode = 0;
        printf("\n\n*** CHEAT MODE EXITED ***\n\n");
    }


}











