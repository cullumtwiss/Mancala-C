#include <stdio.h>
#include <stdlib.h>

int greetings(void); //function that shows start menu
void initializeBoard(int menuChoice, int pieces[], int *playersTurn);//sets board for all three cases (new game, load game, save game,resume) by calling appropiate functions
void loadGame(int pieces[], int *playersTurn); //loads game and from save file, changes array and playersTurn accordingly
void showInstructions(void);//just prints the instructions
void coinFlip(int *playersTurn);//flips coin to determine who goes first
void drawGame(int pieces[]);//displays game board according to pieces array
int whichPocket(int pieces[], int playersTurn);//asks user for their choice of pocket and returns their choice (asks again if invalid)
int showMenu(int pieces[], int *playersTurn);//shows menu and returns their choice, needs to pass pieces[] and *playersTurn to initializeBoard
void saveGame(int pieces[], int playersTurn);//saves game to file
int deposit(int pieces[], int pocketChoice, int playersTurn);//deposits pebbles and returns the final pocket a pebble is placed in
int endOfTurnSpecifics(int pieces[], int lastPocket, int playersTurn);//decides whose turn it is next and changes array accordingly if lastPocket is empty
int gameOver(int pieces[]);//evaluates if game is over by looking at the array and returns 1 if game is over
void declareWinner(int pieces[]);//tallies up scores and determines the winner, then prints correspending message

int main(void) {
  int pocketChoice = 3; //start pocket choice at three to avoid it randomly choosing 0
  int isGameOver = 0;//isGameOver starts as false (we need this variable because compiler gets confused if we name it gameOver too
  int playersTurn = 90;//initializing playersTurn to 90 to avoid any funny business
  int pieces[14];//array that holds positions of pieces
  int lastPocket;//holds the value of the last pocket that a pebble is placed in for each turn
  int greetingsChoice = greetings();//stores the user choice that was input when greetings menu was shown
  
  initializeBoard(greetingsChoice, pieces, &playersTurn);//takes user input given to greetings and resets board accordingly
  showInstructions();//shows instructions if the user selects that they want to
  
  if(greetingsChoice == 0) {
    coinFlip(&playersTurn);//flips coin and changes playersTurn accordingly if new game is selected
  }
  
  while(isGameOver == 0) {//only draws board while game isn't over
    drawGame(pieces);//draws board after each turn
    pocketChoice = whichPocket(pieces, playersTurn);//sets pocket choice equal to user input taken by whichPocket
    lastPocket = deposit(pieces, pocketChoice, playersTurn);//deposit returns the last pocket so we set lastPocket to the return value
    playersTurn = endOfTurnSpecifics(pieces, lastPocket, playersTurn); //we have to give this function the array, number of the last pocket, and playersTurn
    isGameOver = gameOver(pieces);//sets isGameOver to true or false depending on what gameOver evaluates to
    if(pocketChoice == -1) {//if user inputs -1, menu is shown. showMenu calls initializeBoard and then the loop continues
      
      if(showMenu(pieces, &playersTurn) == 0) { 
      coinFlip(&playersTurn);//flips coin again and changes playersTurn accordingly if new game is selected
     }
     
    }
    
  }
  
  declareWinner(pieces);//declares winner based on array values
  drawGame(pieces);//draws game one last time to show winning board
  
}

int greetings(void) {//asks user what they want to do and returns the number they enter
  int newOrOld = 20;//stores user's input, starts at 20 to avoid any funny business
  printf("Welcome! Would you like to start a new game or load an old one?\n");
  printf("(0)New Game \n(1) Load Game\n");
  scanf("%d", &newOrOld); //saves user input to newOrOld
  
while(newOrOld != 0 && newOrOld != 1) {//if input is invalid it keeps asking until it's valid
    printf("Invalid entry. Try again\n");
    scanf("%d", &newOrOld); 
  }
  return(newOrOld);//returns value to pass to initializeBoard
}

void loadGame(int pieces[], int *playersTurn) {
  int i;//counter to read file with
  FILE *savedGame;
  savedGame = fopen("loadGame.txt", "r");//open file for reading
  
  if (savedGame == NULL) {//if file doesn't exist, prints that
        printf("File not found!! Exiting.\n");
    } else {
       for(i = 0; i < 14; i++) {
        fscanf(savedGame, "%d", &pieces[i]);//reads all values of pockets
       }
      //15th number in file is always playersTurn, so read that and save it to playersTurn
      fscanf(savedGame, "%d", playersTurn);
      fclose(savedGame);//close file, we done
    }
    
  }
  
void saveGame(int pieces[], int playersTurn) {
  int i;
  FILE *savedGame;
  savedGame = fopen("loadGame.txt", "w");//open file for writing
  
  if (savedGame == NULL) {//if file doesn't exist it does nothing
        printf("File not found!! Exiting.\n");
    }else {
       for(i = 0; i < 14; i++) {
       fprintf(savedGame, "%d ", *(pieces + i));
       } 
    }  
    
  fclose(savedGame);//close file since we're done writing pieces[] to file
  
  savedGame = fopen("loadGame.txt", "a");//adds players turn to the end of the file so that it is always the 15th integer in file
  fprintf(savedGame, "%d", playersTurn);
  fclose(savedGame);
  
}

void initializeBoard(int menuChoice, int pieces[], int *playersTurn) {//resets array based on returned value of greetings()
  int i;
  
  if(menuChoice == 0) {
     for(i = 0; i < 14; i++) { //set all pocket values to four
        pieces[i] = 4;
    }
      pieces[0] = 0;//then just change the endzones to zero (pieces[0] is player 2's endzone and pieces[7] is player 1's endzone)
      pieces[7] = 0;
      printf("Blank board set. Starting new game...\n");
  } else if(menuChoice == 1) {//if they choose load game, call load game
    loadGame(pieces, playersTurn);
    printf("Game loaded from save file. Let's begin!\n");
  } else if(menuChoice == 2) {//if they choose save game, call save game
    int playersTurnCopy = *playersTurn;//had to make a copy of playersTurn to fix a weird and convuluted bug with the pointer value
    saveGame(pieces, playersTurnCopy);
    printf("Game saved.\n");
  } else {//if they choose resume game, does nothing
    printf("You've chosen to resume the game. Resuming...\n");
  } 
}

void showInstructions(void) {
  int yesOrNo; //stores value for user input of yes or no to seeing the instructions
  printf("Would you like to see the rules of the game?\n" 
  "(0) No \n(1) Yes\n"); 
  scanf("%d", &yesOrNo); 
  
   while(yesOrNo != 0 && yesOrNo != 1) {//if they choose a number not on the menu it keeps asking
    printf("Invalid entry. Try again\n");
    scanf("%d", &yesOrNo); 
  }
  
  if(yesOrNo) {//these instructions were taken from assignment instructions (approved by TA's)
    printf("The object of the game is to collect the most pieces by the end of the game."
   "\n•The Mancala Board consists of two rows, each with six pockets. An “endzone” pocket sits on either end of the rectangular board."
   "\n•There are 48 stones total in the game."
   "\n•The game is set up with 4 stones in each of the 12 pockets and each player claims one side of the board and the endzone to the right of their side (or counterclockwise)."
   "\n•To begin, Player 1 chooses one pocket on their side and picks up all of the stonesfrom this pocket. Moving in a counterclockwise direction, starting with the next pocket, Player 1 deposits a stone in each pocket, until all of the picked-up stones have been deposited."
   "\n•Player 2 then chooses a pocket on their side, picks up all the stones in it and deposits them in a counterclockwise direction until allstones from that pocket are used up."
   "\n•If a player passes their own endzone, they deposit a stone there also. BUT as theypass the opponent’s endzone, they skip it."
   "\n•If a player’s last stone lands in their own endzone, they get to take another turn."
   "\n•If a player’s last stone lands in an empty pocket (not including an endzone), they may capture that stone AND any stones in the pocket directly across. They move all of these stones to their own endzone."
   "\n•The game endswhen all six pockets on one side of the board are empty. The player who still has any stones left in pockets on their own side may claim them."
   "\n•The player with the most stones in their endzone by the end of the game wins."
   "\n•First we will flip a coin to determine who goes first"
   "\n•Then you will enter the number of the pocket you want to move the stones from"
   "\nYou can enter -1 at any time to view the menu.\n");
  } else {
    printf("Alright then let's get started. We will now flip a coin to determine who goes first.\n");
  }
}

void coinFlip(int *playersTurn) {//have to pass the pointer of playersTurn so that it can change that value
  int coinSide = 20;//coinSide starts at 20 to avoid any funny business
  
  printf("Player 1, choose heads(0) or tails(1): ");//takes user input
  scanf("%d", &coinSide); 
  
  while(coinSide != 0 && coinSide != 1) {//keeps asking for input until choice is valid
    printf("Invalid entry. Try again\n");
    scanf("%d", &coinSide); 
  }
  
  printf("Flipping...\n");//totally fake, computer has already chosen a number by now. It's about creating a sense of wonder
  int coinResult = rand() % 2;//rand chooses any number greater than zero, so we do %2 to get a 1 or 2 from that number
  //credit to: https://www.geeksforgeeks.org/generating-random-number-range-c/ which is where I found info on rand()
  
  if(coinResult == 0 && coinSide == 0) {//decides who goes first based on if the random number matches player 1's choice
    printf("Result was heads. Player 1 goes first.\n");
    *playersTurn = 1;
  } else if(coinResult == 0 && coinSide == 1) {
    printf("Result was heads. Player 2 goes first.\n");
    *playersTurn = 2;
  } else if(coinResult == 1 && coinSide == 1) {
    printf("Result was tails. Player 1 goes first.\n");
    *playersTurn = 1;
  }else if(coinResult == 1 && coinSide == 0) {
    printf("Result was tails. Player 2 goes first.\n");
    *playersTurn = 2;
  }
  
}

void drawGame(int pieces[]) {//credit to assignment instructions for providing a good template to base this on
  printf("                                              ~~~~~~~~~~~~\n"
         "             6     5     4     3     2     1  | Player 2 |\n");
  printf("             |     |     |     |     |     |  ~~~~~~~~~~~~\n"
         "           ------------------------------------\n");
  printf("     -----| %2d  | %2d  | %2d  | %2d  | %2d  | %2d  |-----\n", pieces[13], pieces[12],pieces[11],pieces[10],pieces[9],pieces[8]);
  printf("    | %2d  |-----|-----|-----|-----|-----|-----| %2d  |\n", pieces[0], pieces[7]);
  printf("     -----| %2d  | %2d  | %2d  | %2d  | %2d  | %2d  |-----\n", pieces[1], pieces[2], pieces[3], pieces[4], pieces[5], pieces[6]);
  printf("           ------------------------------------\n");
  printf("~~~~~~~~~~~~ |     |     |     |     |     | \n");
  printf("| Player 1 | 1     2     3     4     5     6\n");
  printf("~~~~~~~~~~~~\n");
}

int whichPocket(int pieces[], int playersTurn) {
  int pocketChoice;//stores which pocket user chooses
  
  printf("Player %d, which of your pockets do you pick?\n", playersTurn); 
  scanf("%d", &pocketChoice);
  
  if(playersTurn == 1) {//these entries are only invalid if playersTurn is 1
    while(((pocketChoice < 1 || pocketChoice > 6) && pocketChoice != -1) || (pieces[pocketChoice] == 0 && pocketChoice != -1)) {
      //pocketChoice must be in between 0 and 7 and it must not be empty. We also don't say invalid if they enter -1
      printf("Invalid choice. Try again.\n");
      scanf("%d", &pocketChoice);
    }
  } else {//the only other option is that playersTurn is 2, these entries are only invalid if playersTurn is 2
  
      if(pocketChoice != -1) {// if they enter -1 we must leave the value unchanged
        pocketChoice += 7;//we need to add 7 to get the index of the pocket on their side
      }
      
      while(((pocketChoice < 8 || pocketChoice > 13) && pocketChoice != -1) || (pieces[pocketChoice] == 0 && pocketChoice != -1)) {
      //pocketChoice must be in between 7 and 14 and it must not be empty. We also don't say invalid if they enter -1
      //printf("%d has %d\n", pocketChoice, pieces[pocketChoice]);
      printf("Invalid choice. Try again.\n");
      scanf("%d", &pocketChoice);
      if(pocketChoice != -1) {//if they enter an invalid number, we must change their choice again before testing the conditions
        pocketChoice += 7;
       }
      
      } 
    }  
    
  return(pocketChoice);// we need to return the pocket choice (after it's been adjusted) to pass it to deposit()
}

int showMenu(int pieces[], int *playersTurn) {
  int whatWant;//stores user's menu choice
  printf("You've pasued the game. What would you like to do?\n");
  printf("(0)New Game \n(1)Load Game\n(2)Save Game\n(3)Resume Game\n");
  scanf("%d", &whatWant); 
  
while(whatWant != 0 && whatWant != 1 && whatWant != 2 && whatWant != 3) {//keeps asking until input is valid
    printf("Invalid entry. Try again\n");
    scanf("%d", &whatWant); 
  }
  
  initializeBoard(whatWant, pieces, playersTurn);//passes user choice to initializeBoard to change array
  return(whatWant);//returns user choice so that we can check what it is in main
}

int deposit(int pieces[], int pocketChoice, int playersTurn) {
    int lastPocket = 0;//this will store the index of the pocket that the last pebble is dropped in
    int pebblesLeft = pieces[pocketChoice];//starts the amount of pebbles to be the amount in the chosen pocket
    int i = 1; //i starts at one because we start at the pocket one to the right of their choice
    pieces[pocketChoice] = 0; //remove all pebbles from the pocket they chose 
    
    while(pebblesLeft > 0 && pocketChoice != -1) {//while there are still pebbles to distribute and menu user hasn't opened menu
      
        if(pocketChoice + i == 7 && playersTurn == 2) {
          i += 1;//if the pocket that will receive stones next is player 1's endzone, we skip it
        }
        if(pocketChoice + i > 13 && playersTurn == 2) {
          i = -1 * pocketChoice; //if the pocket that will receive stones next is player 2's endzone, we need to add the stone to pieces[0]
          //because pieces[13] is the highest index. we go to pieces[0] by making i -pocketChoice since we add i to pocketChoice later
        }
        if(pocketChoice + i > 13 && playersTurn == 1) {
          i = -1 * (pocketChoice - 1);//if the pocket that will receive stones next is player 2's endzone, we need to add the stone to pieces[1]
          //we do this by making i one greater than -pocketChoice
        }
        
      pieces[pocketChoice + i]++;//add one to whichever pocket we're on, which is determined by the above if statements
      i++; //increase i by one to make sure we move onto the next index
      pebblesLeft--;//remove one from pebblesLeft since we just put a pebble into a pocket

    }
  
     lastPocket = pocketChoice + i - 1; //last pocket is whatever pocket we end on, but since i gets increased an extra time we -1
     return(lastPocket); 
  }
  
int endOfTurnSpecifics(int pieces[], int lastPocket, int playersTurn) {
  int turnChanged = 0; //flag variable to use for later 
  int across;//stores the index of the pocket across from lastPocket
  if(pieces[lastPocket] == 1 && playersTurn == 1 && lastPocket != 0 && lastPocket != 7) {//if lastPocket has zero pebbles originally, it will always have one after deposit()
    //we also make sure that they didn't land in an endzone
    if( 0 < lastPocket && lastPocket < 7) { //if lastPocket is on player 1's side, we use this algothorimn to find across
    across = lastPocket + (12 - (2 * (lastPocket - 1)));//(which was probably not worth the effort of figuring out instead of using a switch)
  
      if(pieces[across] != 0) {//we only change these values if across has pebbles in it
        pieces[lastPocket] = 0;//we remove the pebble from lastPocket
        pieces[7] += pieces[across] + 1;//then we add it and the pebbles in the pocket across to player 1's endzone
        pieces[across] = 0;//now we remove pebbles across from lastPocket
      }
      
    } 
    
    if( 7 < lastPocket && lastPocket < 14){//if lastPocket is on player 2's side, we use this algothorimn to find across
       across = lastPocket - (2 * (lastPocket - 7));
       
       if(pieces[across] != 0) {//we only change these values if across has pebbles in it
        pieces[7] += pieces[across] + 1; //same process but across is found differently
        pieces[across] = 0;
      }
       
    }
    
  }
  
  
  if(pieces[lastPocket] == 1 && playersTurn == 2 && lastPocket != 0 && lastPocket != 7) {//if lastPocket has zero pebbles originally, it will always have one after deposit()
    //we also make sure that they didn't land in an endzone (this is the same process as above but for player 2)

    if( 0 < lastPocket && lastPocket < 7) {
     across = lastPocket + (12 - (2 * (lastPocket - 1)));
     
     if(pieces[across] != 0) {
       pieces[lastPocket] = 0;
       pieces[0] += pieces[across] + 1;
       pieces[across] = 0;
     }
     
    } 
    
    if( 7 < lastPocket && lastPocket < 14){
       across = lastPocket - (2 * (lastPocket - 7));
         if(pieces[across] != 0) {
           pieces[lastPocket] = 0;
           pieces[0] += pieces[across] + 1;
           pieces[across] = 0;
         }
    }
  }
  
  if(playersTurn == 1 && lastPocket != 7) {//if playersTurn is one and they don't land in their endzone we switch to player 2
    playersTurn = 2;
    turnChanged = 1;//change flag because otherwise the next if statement would see that it's player 2's turn and change it back to player 1
  }
  
  if(playersTurn == 2 && lastPocket != 0 && turnChanged == 0) {//if playersTurn is two and they don't land in their endzone and playersTurn hasn't been changed yet
    playersTurn = 1;
  }
  
 return(playersTurn);//return playersTurn to pass know which player to ask for their choice (by calling whichPocket)
}

int gameOver(int pieces[]) {
  int gameOver = 0;
  if((pieces[1] == 0 && pieces[2] == 0 && pieces[3] == 0 && pieces[4] == 0 && pieces[5] == 0 && pieces[6] == 0) || (pieces[8] == 0 && pieces[9] == 0 && pieces[10] == 0 && pieces[11] == 0 && pieces[12] == 0 && pieces[13] == 0)) {
    gameOver = 1;//if all the pieces on one side are gone then game is over
  }
return(gameOver);//return if game is over so while loop in main can check its condition
}

void declareWinner(int pieces[]) {
  int i;
  int sum1 = 0; //sum of pieces on player 1's side
  int sum2 = 0;//sum of pieces on player 2's side
    
    for(i = 1; i < 7; i++) {//adds all pieces on player one's side to sum
      sum1 += pieces[i];
      pieces[i] = 0;//then sets them to zero to 'move' them to endzone
    }
    
    pieces[7] += sum1;//add all the pieces on this side to the endzone
    
    for(i = 8; i < 14; i++) {//same process for player 2's side
      sum2 += pieces[i];
      pieces[i] = 0;
    }
    
    pieces[0] += sum2;
    
    if(pieces[7] > pieces[0]) {//prints winner by comparing how many pebbles are in each endzone
      printf("Nice job Player 1, you win! Here's the winning board\n");
    } else if(pieces[7] < pieces[0]) {
      printf("Nice job Player 2, you win! Here's the winning board\n");
    } else {
      printf("Wow, you somehow ended in a draw! Nice job?\n");
    }
}
