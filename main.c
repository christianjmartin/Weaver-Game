/**********************************
Project 2 "Weaver"
Christian Martin
Professor Kidane
03/08/23
**********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>



// struct for linked list
typedef struct Node *NodePtr; // Make:  NodePtr to be a synonym for: struct Node *
	struct Node {
  		char data[81]; // The data stored at the node
  		NodePtr pNext; // Pointer to the next node
};



// loops through list and frees memory, used when applicable
void freeMemory(NodePtr pHead) {
    NodePtr pCurrent = pHead;
    while(pCurrent != NULL) {
        NodePtr pTemp = pCurrent;
        pCurrent = pCurrent->pNext;
        free(pTemp);
    }
}


// checks if in dictionary (called in checkRequirements)
NodePtr checkValidWord(NodePtr pHead, char *word)
{
    NodePtr pCurrent = pHead;
    while (pCurrent != NULL) {
        if (strcmp(pCurrent->data, word) == 0) {
            return pCurrent;
        }
        pCurrent = pCurrent->pNext;
    }
    return NULL;
}



// checks if word is correct size, in the list, or is signifying for random
bool CheckRequirements(char *word, int wordSize, NodePtr pHead, int counter) {
	// if the user enters 'r'
	if ((strlen(word) == 1 && word[0] == 'r')) {
		int randIndex;
		randIndex = rand() % counter;
		NodePtr pCurrent = pHead;
        int index = 0;
        while (pCurrent != NULL) {
            if (index == randIndex) {
                strcpy(word, pCurrent->data);
                return true;
            }
            index++;
            pCurrent = pCurrent->pNext;
        }
	}
	// if the user enters word of incorrect size
	else if (strlen(word) != wordSize) {
		printf("Your word, '%s', is not a %d-letter word. Try again.\n", word, wordSize);
		return false;
	}
	else {
	// if the user enters word that is not in the list
	NodePtr pWord = checkValidWord(pHead, word);
	if (pWord == NULL) {
    	printf("Your word, '%s', is not a valid dictionary word. Try again.\n", word);
		return false;
	}
	}
	return true;
}



// fixes newline issues for different part of the game, while functionality stays same 
bool CheckRequirementsDuringGame(char *word, int wordSize, NodePtr pHead, int counter) {
	// if the user enters 'r'
	if ((strlen(word) == 1 && word[0] == 'r')) {
		int randIndex;
		randIndex = rand() % counter;
		NodePtr pCurrent = pHead;
        int index = 0;
        while (pCurrent != NULL) {
            if (index == randIndex) {
                strcpy(word, pCurrent->data);
                return true;
            }
            index++;
            pCurrent = pCurrent->pNext;
        }
	}
	// if the user enters word of incorrect size
	else if (strlen(word) != wordSize) {
		printf("Your word, '%s', is not a %d-letter word. Try again.", word, wordSize);
		return false;
	}
	else {
	// if the user enters word that is not in the list
		NodePtr pWord = checkValidWord(pHead, word);
		if (pWord == NULL) {
    		printf("Your word, '%s', is not a valid dictionary word. Try again.", word);
			return false;
		}
	}
return true;
}



// checks if the users guess is one letter apart from the previous 
bool oneLetterApart (char *prevWord, char *userGuess, int wordSize) {
	int i;
	int diff = 0;
	for (i = 0; i < wordSize; i++) {
		if (prevWord[i] != userGuess[i]) {
			diff += 1;
		}
	}
	if ((diff > 1) || (diff == 0)) {
		printf("Your word, '%s', is not exactly 1 character different. Try again.", userGuess);
		return false;
	}
	else {
		return true;
	}
}



// checks if the users guess matches the goal word
bool isEndWord (char *prevWord, char *congratsStartWord, char *userGuess, int wordSize, int moveCounter) {
	int i;
	int diff = 0;

	// if previous and guess are exactly same
	if (strcmp(prevWord, userGuess) == 0) {
		printf("Congratulations! You changed '%s' into '%s' in %d moves.", congratsStartWord, userGuess, moveCounter);
		return true;
	}
	else {
		return false;
	}
}



// reads file into a linked list, counts elements of said linked list
void readFile(NodePtr *pHead, int wordSize, int *counter) {
	char filename[] = "words.txt";
	FILE *filePtr  = fopen(filename, "r"); // "r" means we open the file for reading
	
	// Check that the file was able to be opened
	if(filePtr == NULL) {
		printf("Error: could not open %s for reading\n", filename);
		exit(-1);
	}

	// Read each word from file, and print them one by one
	char inputString[ 81];
	  while(fscanf(filePtr, "%s", inputString) != EOF) {
        if(strlen(inputString) == wordSize) {
            NodePtr pNewNode = malloc(sizeof(struct Node));
            strcpy(pNewNode->data, inputString);

			// loop through list to find the correct position to insert the new node
            NodePtr pCurrent = *pHead;
            NodePtr pPrev = NULL;
            while((pCurrent != NULL) && (strcmp(inputString, pCurrent->data) > 0)) {
                pPrev = pCurrent; // previous node
                pCurrent = pCurrent->pNext;
            }
			// if nothing before, insert at beginning
            if(pPrev == NULL) {
                pNewNode->pNext = *pHead;
                *pHead = pNewNode;
            }
			// insert node after pPrev
            else {
                pPrev->pNext = pNewNode; // inserts
                pNewNode->pNext = pCurrent; // update to cont. loop
            }
			(*counter)++;
        }
    }
	fclose(filePtr);
	printf("Number of %d-letter words found: ",wordSize);
	printf ("%d.\n\n", *counter);
	if (*counter == 0) {
		printf("Please try again with new number of letters\n");
		exit(-1);
	}
}






// plays the game 
void playGame(int wordSize, NodePtr pHead, int counter) {
	char startWord[wordSize];
	char endWord[wordSize];
	bool goodStartWord = false;
	bool goodEndWord = false;
	int moveCounter = 1;

	// configure the start and end word for the game
	while ((!goodStartWord) || (!goodEndWord)) {
		printf("Enter starting and ending words, or 'r' for either for a random word: ");
		scanf("%s", startWord);
		scanf("%s", endWord);
		goodStartWord = CheckRequirements(startWord, wordSize, pHead, counter); 
		if (goodStartWord) {
			goodEndWord = CheckRequirements(endWord, wordSize, pHead, counter);
		}
		if ((!goodStartWord) || (!goodEndWord)) {
        	continue;
    	}
		if ((goodStartWord) && (goodEndWord)) {
			break;
		}
	}

	printf("Your starting word is: %s.\n", startWord);
	printf("Your ending word is: %s.\n", endWord);
	printf("\nOn each move enter a word of the same length that is at most 1 character different and is also in the dictionary.\n");
    printf("You may also type in 'q' to quit guessing.\n");

	// the meat of the game begins, user tries to reach goal word from the start wordd
	char userGuess[81] = "";
	char congratsStartWord[wordSize]; 
	strcpy(congratsStartWord, startWord);
	bool goodUserGuess = false;
	bool oneDiff = false;
	bool gameCompleted = false;
	while (strcmp(userGuess, "q") != 0) {
		printf("\n%d. Previous word is '%s'. Goal word is '%s'. Next word: ", moveCounter, startWord, endWord);
		scanf("%s", userGuess);
		if (strcmp(userGuess, "q") != 0) {
			goodUserGuess = CheckRequirementsDuringGame(userGuess, wordSize, pHead, counter);
			if (goodUserGuess) {
				oneDiff = oneLetterApart(startWord, userGuess, wordSize);
				if (oneDiff) {
					gameCompleted = isEndWord(endWord, congratsStartWord, userGuess, wordSize, moveCounter);
					if (gameCompleted) {
						break;
					}
				}
			}
		}
		if ((!goodUserGuess) || (!oneDiff)) {
        	continue;
        }
	moveCounter++;
	strcpy(startWord, userGuess);
	}
	// prints 3 options for the user after a playthrough is complete
	printf("\nEnter: \t1 to play again,\n");
    printf("\t2 to change the number of letters in the words and then play again, or\n");
    printf("\t3 to exit the program.\n");
    printf("Your choice --> ");
	int userChoice;
	scanf("%d", &userChoice);
	if (userChoice == 1) {
		// same memory is used since same list is used
		playGame(wordSize, pHead, counter);
	}
	if (userChoice == 2) {
		freeMemory(pHead); // free memory from linked list

		// new linked list is made for game with new size
		int newWordSize;
		printf("How many letters do you want to have in the words? ");
		scanf("%d", &newWordSize);
		pHead = NULL;
		int newCounter = 0; 
		readFile(&pHead, newWordSize, &newCounter);
		playGame(newWordSize, pHead, newCounter);
	}
	if (userChoice == 3) {
		printf("\nThanks for playing!");
		printf("\nExiting...");
		freeMemory(pHead); // free memory from linked list
		exit(0); // exit program
	}
}



int main() {
	srand(1);

	printf("Weaver is a game where you try to find a way to get from the starting word to the ending word.\n");
	printf("You can change only one letter at a time, and each word along the way must be a valid word.\n");
	printf("Enjoy!\n\n");

	int wordSize;
	printf("How many letters do you want to have in the words? ");
	scanf("%d", &wordSize);

	NodePtr pHead = NULL;
	int counter = 0; // counter to return size of the list
	readFile(&pHead, wordSize, &counter); // read and make linked list
	playGame(wordSize, pHead, counter); // plays game

	// memory is free'd in "playGame" function at all control paths necessary, program exit(0)'s from there
	return 0;
}
