#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define word_length 20
#define max_players 10
#define max_words 20
#define max_guesses 26 // Maximum number of different letters (A-Z)

// Function declarations
void printHangman(int tries); //to print Hangman after every wrong attempt
void initializeWord(char *word, const char *chosenWord); //to initialize hidden word as underscores
int checkGuess(char *word, const char *chosenWord, char guess); //Function to check if the letter entered by the user is in the word  
int isWordComplete(const char *word); //Function to check if the word has been fully guessed
void displayWord(const char *word);	//Function to display the current state of the word
int getRandomWordIndex(int wordCount); //Function to get a random word index that will be multiplied with the wordCount to get a random word from the file everytime. 
void Hint(const char *chosenWord, char *word); //function to provide Hint
int extractWord(const char *filename, char words[][word_length]); //loads word from file

int main() 
{
    char players[max_players][word_length];
    int scores[max_players] = {0};
    int numPlayers, tries, maxTries = 6;
    char guess;
    int categoryChoice, i;
    char words[max_words][word_length];  // Array to store words for the chosen category
    int wordCount = 0;
    char guessedLetters[max_guesses] = {0}; // Array to track guessed letters
    
    srand(time(NULL));  // Seed the random number generator
       printf("\033[38;5;214m");
    printf("\n\n");
    printf("\t       **       ** ******** **           *****      *****       ****  ****     ********\t   **********   ****   \n");
    printf("\t       **       ** **       **         **     **  **     **   **    **     **  **      \t       **     **    ** \n");
    printf("\t       **   *   ** **       **         **         **     **   **    **     **  **      \t       **     **    ** \n");
    printf("\t       **  * *  ** ******   **         **         **     **   **    **     **  ******  \t       **     **    ** \n");
    printf("\t       ** *   * ** **       **         **     **  **     **   **    **     **  **      \t       **     **    ** \n");
    printf("\t       ***     *** ******** *********   ******      *****     **    **     **  ********\t       **       ****   \n");
	
	printf("\n\t\t      **    **   *********   **      **     ********     ****  ****    *********  **       **       \n");
    printf("\t\t      **    **   **     **   ***     **    **      **  **    **    **  **     **  ***      **  \n");
    printf("\t\t      **    **   **     **   ** *    **    **          **    **    **  **     **  ** *     **    \n");
    printf("\t\t      **    **   **     **   **  *   **    **          **    **    **  **     **  **  *    **  \n");
    printf("\t\t      ********   *********   **   *  **    **          **    **    **  *********  **   *   **    \n");
    printf("\t\t      **    **   **     **   **    * **    **   *****  **    **    **  **     **  **    *  **  \n");
    printf("\t\t      **    **   **     **   **     ***    **      **  **    **    **  **     **  **     * ** \n");
    printf("\t\t      **    **   **     **   **      **    **      **  **    **    **  **     **  **      ***   \n");
    printf("\t\t      **    **   **     **   **      **     *********  **    **    **  **     **  **       **    \n");
	
	printf("\033[0m");
	printf("\n\n\n");

    
    // Get player names
    printf("Enter the number of players (1-%d): ", max_players);
    scanf("%d", &numPlayers);
    getchar();  // Clear the newline character left by scanf
    for (i = 0; i < numPlayers; i++) {
        printf("Enter name of player %d: ", i + 1);
        fgets(players[i], word_length, stdin);
        players[i][strcspn(players[i], "\n")] = '\0';  // Remove trailing newline
    }

    // Ask the player to choose a category
    printf("\nChoose a category:\n");
    printf("1. Food\n");
    printf("2. Animals\n");
    printf("3. Sports\n");
    printf("4. Places\n");
    printf("5. Programming Terms\n");  // New category option
    printf("Enter your choice (1-5): ");
    scanf("%d", &categoryChoice);

    // Ensure the category choice is valid
    if (categoryChoice < 1 || categoryChoice > 5) {
        printf("Invalid category choice.\n");
        return 1;  // Exit if the choice is invalid
    }

    // Load words from the selected category file
    const char *categoryFiles[] = {"food.txt", "animals.txt", "sports.txt", "places.txt", "programming.txt"};
    wordCount = extractWord(categoryFiles[categoryChoice - 1], words);

    if (wordCount == 0) {
        printf("Error loading words from the file.\n");
        return 1;
    }

    // Game loop for each player
    for (i = 0; i < numPlayers; i++) {
        const char *chosenWord = words[getRandomWordIndex(wordCount)];
        char word[word_length];
        tries = 0;
        int hintFlag = 0;  // Flag to check if hint has been given

        printf("\n\033[1;34m%s's Turn\033[0m\n", players[i]);

        // Initialize the word with underscores
        initializeWord(word, chosenWord);

        // Game loop
        while (tries < maxTries) {
            // Display the word and the hangman
            displayWord(word);
            printHangman(tries);

            // Loop to get a valid guess
            int validInput = 0;
            while (!validInput)
			{
                printf("Enter a Letter to Guess: ");
                scanf(" %c", &guess);
                guess = tolower(guess); // Convert the input character to lowercase

                // To Check if the input is only an alphabetic character
                if (isalpha(guess))
				{
                    // Check if the letter has already been guessed
                    int j, alreadyGuessed = 0;
                    for (j = 0; j < max_guesses; j++)
					{
                        if (guessedLetters[j] == guess)
						{
                            alreadyGuessed = 1;
                            break;
                        }
                    }
                    
                    if (alreadyGuessed) {
                        printf("\033[38;5;214mYou've already guessed the letter '%c'!\033[0m\n", guess);  // Yellow color
                    }
					else
					{
                        guessedLetters[i] = guess;  // Store the guessed letter
                        validInput = 1; // Valid input, break out of the loop
                    }
                }
				else
				{
                    // If input is not a valid letter, print an error message
                    printf("\033[0;31mError: Please enter a valid letter (a-z)!\033[0m\n");  // Red for invalid input
                }
            }

            // Check if the guess is correct
            if (checkGuess(word, chosenWord, guess)) {
                printf("\033[0;32mGood guess!\033[0m\n");  // Green for correct guess
                scores[i] += 10;  // Increment score by 10 for correct guess
            } else {
                printf("\033[0;31mWrong guess!\033[0m\n");  // Red for wrong guess
                tries++;
            }

            // Offer a hint if only 2 tries remain and hint hasn't been given yet
            if (tries == maxTries - 2 && !hintFlag) {
                char response;
                printf("\033[1;34mWould you like to have a hint? (y/n)\033[0m: ");
                scanf(" %c", &response);
                if (tolower(response) == 'y') {
                    Hint(chosenWord, word);  // Provide and update word with the hint
                    hintFlag = 1;  // Mark hint as given
                }
            }

            // Check if the word has been fully guessed
            if (isWordComplete(word)) {
                printf("\033[0;32mCongratulations, %s! You've guessed the word: %s\033[0m\n", players[i], chosenWord);
                break;
            }
        }

        // If the user is out of tries, the GAME is OVER
        if (tries == maxTries) {
            printHangman(tries); // Ensure the final hangman is printed
            printf("\033[0;31mGame over, %s! You've run out of tries. The word was: %s\033[0m\n", players[i], chosenWord);
        }
    }

    // Display the final leaderboard
    printf("\n\033[1;33mFinal Leaderboard:\033[0m\n");  // Yellow for leaderboard
    for (i = 0; i < numPlayers; i++) {
        printf("%d. \033[1;34m%s\033[0m - %d points\n", i + 1, players[i], scores[i]);  // Blue for player names
    }

    // Display the Final Winner
    int winnerIndex = 0;
    for (i = 1; i < numPlayers; i++)
	{
        if (scores[i] > scores[winnerIndex])
		{
            winnerIndex = i;
        }
    }

    printf("\n\033[1;32mWinner: %s with %d points!\033[0m\n", players[winnerIndex], scores[winnerIndex]);  // Green for winner
    return 0;
}

// Function to print the hangman based on the number of tries
void printHangman(int tries) {
    // Hangman design (same as before)
    switch (tries) {
        case 0:
            printf("\033[1;33m\n  +---+\n  |   |\n      |\n      |\n      |\n      |\n=========\n\033[0m");
            break;
        case 1:
            printf("\033[1;33m\n  +---+\n  |   |\n  O   |\n      |\n      |\n      |\n=========\n\033[0m");
            break;
        case 2:
            printf("\033[1;33m\n  +---+\n  |   |\n  O   |\n  |   |\n      |\n      |\n=========\n\033[0m");
            break;
        case 3:
            printf("\033[1;33m\n  +---+\n  |   |\n  O   |\n /|   |\n      |\n      |\n=========\n\033[0m");
            break;
        case 4:
            printf("\033[1;33m\n  +---+\n  |   |\n  O   |\n /|\\  |\n      |\n      |\n=========\n\033[0m");
            break;
        case 5:
            printf("\033[1;33m\n  +---+\n  |   |\n  O   |\n /|\\  |\n /    |\n      |\n=========\n\033[0m");
            break;
        case 6:
            printf("\033[1;33m\n  +---+\n  |   |\n  O   |\n /|\\  |\n / \\  |\n      |\n=========\n\033[0m");
            break;
    }
}

// Other functions (initializeWord, checkGuess, isWordComplete, displayWord, extractWord, etc.) remain the same
int getRandomWordIndex(int wordCount) {
    return rand() % wordCount;
}
// Function to initialize the word with underscores (same as before)
void initializeWord(char *word, const char *chosenWord) {
    int i, len = strlen(chosenWord);
    for (i = 0; i < len; i++) {
        if (chosenWord[i] == ' ') {
            word[i] = ' ';
        } 
		else 
		{
            word[i] = '_';
        }
    }
    word[len] = '\0';
}

int checkGuess(char *word, const char *chosenWord, char guess) {
    int i, correct = 0;
    for (i = 0; chosenWord[i] != '\0'; i++) {
        // Skip spaces and don't count them
        if (chosenWord[i] == ' ') {
            word[i] = ' ';
            continue;
        }

        if (chosenWord[i] == guess && word[i] == '_') {
            word[i] = guess;
            correct = 1;
        }
    }
    return correct;
}

int isWordComplete(const char *word) {
    while (*word) {
        if (*word == '_') {
            return 0;
        }
        word++;
    }
    return 1;
}

void displayWord(const char *word) {
    printf("Word: ");
    while (*word) {
        printf("%c ", *word);
        word++;
    }
    printf("\n");
}

//Hint will be given on the very first unguessed letter
void Hint(const char *chosenWord, char *word) {
    int i, len = strlen(chosenWord);
    for (i = 0; i < len; i++) {
        if (chosenWord[i] != ' ' && word[i] == '_') {
            word[i] = chosenWord[i];
            break;
        }
    }
}

// Function to get a random word index
int extractWord(const char *filename, char words[][word_length]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open %s\n", filename);
        return 0;
    }

    int wordCount = 0;
    while (fgets(words[wordCount], word_length, file)) {
        words[wordCount][strcspn(words[wordCount], "\n")] = '\0';  // Remove newline
        wordCount++;
    }

    fclose(file);
    return wordCount;
}
