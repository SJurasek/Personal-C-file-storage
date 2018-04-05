//
// APS105 Lab 8 Lab8.c
//
// This is a program written to maintain a personal music library, 
// using a linked list to hold the songs in the library.
//
// Author: Scott Jurasek
// Student Number: 1004203578
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

// A node in the linked list

// Each string in the node is declared as a character pointer variable,
// so they need to be dynamically allocated using the malloc() function,
// and deallocated using the free() function after use.

typedef struct node {
    char *artist ;
    char *songName ;
    char *genre ;
    struct node *nextNode ;
} Node;

// Declarations of linked list functions

bool isSongInList(Node* list, char *songName);
Node* insertIntoOrderedList(Node* list);
Node* deleteNode(Node* list, char *songName);
Node* deleteList(Node* list);
Node* newNode(Node* nextNode, char* songName, char *artist, char *genre);
Node* findNodeByName(Node *list, char *songName);
void printSearchedSong(Node* list, char *songName);
void printList(Node* list);


// Declarations of support functions
// See below the main function for descriptions of what these functions do

void inputStringFromUser( char prompt[], char s[], int arraySize ) ;
void songNameDuplicate( char songName[] ) ;
void songNameFound( char songName[] ) ;
void songNameNotFound( char songName[] ) ;
void songNameDeleted( char songName[] ) ;
void artistFound( char artist[] ) ;
void artistNotFound( char artist[] ) ;
void printMusicLibraryEmpty( void ) ;
void printMusicLibraryTitle( void ) ;

const int MAX_LENGTH = 1024;

int main( void ) {
    // Declare the head of the linked list.
	
	Node *songList = NULL;
 
    // Announce the start of the program
    printf( "Personal Music Library.\n\n" ) ;
    printf( "%s", "Commands are I (insert), D (delete), S (search by song name),\n"
         "P (print), Q (quit).\n" ) ;
    
    char response ;
    char input[ MAX_LENGTH + 1 ] ;
    do {
        inputStringFromUser( "\nCommand", input, MAX_LENGTH ) ;

        // Response is the first character entered by user.
        // Convert to uppercase to simplify later comparisons.
        response = toupper( input[0] ) ;

        if( response == 'I' ) {
            // Insert a song into the linked list.
            // Maintain the list in alphabetical order by song name.
			
			songList = insertIntoOrderedList(songList);

        }
        else if( response == 'D' ) {
            // Delete a song from the list.

            char *prompt = "\nEnter the name of the song to be deleted" ;

            inputStringFromUser(prompt, input, MAX_LENGTH);
			
			songList = deleteNode(songList, input);
        }
        else if( response == 'S' ) {
            // Search for a song by its name.

            char *prompt = "\nEnter the name of the song to search for" ;

            inputStringFromUser(prompt, input, MAX_LENGTH);
			
			printSearchedSong(songList, input);

        }
        else if( response == 'P' ) {
            // Print the music library.
			printList(songList);
        }
        else if( response == 'Q' ) {
            ; // do nothing, we'll catch this below
        }
        else {
            // do this if no command matched ...
            printf( "\nInvalid command.\n" ) ;
        }
    } while( response != 'Q' ) ;
  
    // Delete the entire linked list.
    songList = deleteList(songList);

    // Print the linked list to confirm deletion.
    printList(songList);

    return 0 ;
}

// Support Function Definitions

// Prompt the user for a string safely, without buffer overflow
void inputStringFromUser(char prompt[], char s[], int maxStrLength) {
    int i = 0;
    char c;
    
    printf( "%s --> ", prompt ) ;
    while( i < maxStrLength && (c = getchar()) != '\n' )
        s[i++] = c ;
    s[i] = '\0' ;
}

// Function to call when the user is trying to insert a song name 
// that is already in the personal music library.
void songNameDuplicate( char songName[] ) {
    printf( "\nA song with the name '%s' is already in the music library.\n"
            "No new song entered.\n", songName);
}

// Function to call when a song name was found in the personal music library.
void songNameFound( char songName[] ) {
    printf( "\nThe song name '%s' was found in the music library.\n",
           songName ) ;
}

// Function to call when a song name was not found in the personal music library.
void songNameNotFound( char songName[] ) {
    printf( "\nThe song name '%s' was not found in the music library.\n",
           songName);
}

// Function to call when a song name that is to be deleted
// was found in the personal music library.
void songNameDeleted( char songName[] ) {
    printf( "\nDeleting a song with name '%s' from the music library.\n",
           songName);
}

// Function to call when printing an empty music library.
void printMusicLibraryEmpty(void) {
    printf( "\nThe music library is empty.\n" ) ;
}

// Function to call to print a title when the entire music library is printed.
void printMusicLibraryTitle(void) {
    printf( "\nMy Personal Music Library: \n" ) ;
}

// Add your functions below this line.

bool isSongInList(Node* list, char *songName){
	Node* currentNode = list;
	while(currentNode != NULL){
		if(strcmp(songName, list->songName) == 0)
			return true;
		currentNode = currentNode->nextNode;
	}
	return false;
}

/**
 * @brief Inserts a valid song into a linkedlist according to alphanumeric order
 * @param list
 * @param songName
 * @param artist
 * @param genre
 * @return 
 */
Node* insertIntoOrderedList(Node* list){
	Node *newSong = (Node *)malloc(sizeof(Node));
	char *promptName = "Song name" ;
	char *promptArtist =  "Artist" ;
	char *promptGenre = "Genre" ;
	
	char tempString[MAX_LENGTH+1];
	inputStringFromUser("Song name", tempString, MAX_LENGTH);
	newSong->songName = (char *)malloc(sizeof(char) * (strlen(tempString)+1) );
	strcpy(newSong->songName, tempString);
	
	inputStringFromUser("Artist", tempString, MAX_LENGTH);
	newSong->artist = (char *)malloc(sizeof(char) * (strlen(tempString)+1) );
	strcpy(newSong->artist, tempString);
	
	inputStringFromUser("Genre", tempString, MAX_LENGTH);
	newSong->genre = (char *)malloc(sizeof(char) * (strlen(tempString)+1) );
	strcpy(newSong->genre, tempString);
	
	newSong->nextNode = NULL;
	
	if(isSongInList(list, newSong->songName)){
		songNameDuplicate(newSong->songName);
		free(newSong->songName);
		free(newSong->artist);
		free(newSong->genre);
		free(newSong);
		return list;
	}
	
	if(list == NULL){
		return newSong;
	}else if(list->nextNode == NULL){
		if(strcmp(newSong->songName, list->songName) < 0){
			newSong->nextNode = list;
			return newSong;
		}else{
			list->nextNode = newSong;
			return list;
		}
	} else {
		Node *temp = list;
		while(temp->nextNode != NULL) {
			if(strcmp(newSong->songName, temp->nextNode->songName) < 0){
				newSong->nextNode = temp->nextNode;
				temp->nextNode = newSong;
				return list;
			}
			temp = temp->nextNode;
		}
		temp->nextNode = newSong;
		return list;
	}
}


/**
 * @brief Deletes the node with songName in the list and frees all data in it.
 * @param list
 * @param songName
 */
Node* deleteNode(Node* list, char *songName){
	Node *currentNode = list;
	Node *previous = NULL;
	
	while(currentNode != NULL) {
		if(strcmp(currentNode->songName, songName) == 0){
			songNameDeleted(currentNode->songName);
			free(currentNode->songName);
			free(currentNode->artist);
			free(currentNode->genre);
			if(previous != NULL){
				previous->nextNode = currentNode->nextNode;
				free(currentNode);
			}else{
				free(currentNode);
			}
			return list;
		}
		previous = currentNode;
		currentNode = currentNode->nextNode;
	}
	songNameNotFound(songName);
	return list;
}

Node* deleteList(Node* list){
	Node* currentNode = list;
	
	while(currentNode != NULL) {
		songNameDeleted(currentNode->songName);
		free(currentNode->songName);
		free(currentNode->artist);
		free(currentNode->genre);
		currentNode = currentNode->nextNode;
		free(list);
		list = currentNode;
	}
	return list;
}

Node* newNode(Node* nextNode, char *songName, char *artist, char *genre){
	Node *newSong = (Node *)malloc(sizeof(Node));
	if(newSong == NULL){
		printf("Error - out of memory\n");
		exit(EXIT_FAILURE);
	}
	newSong->songName = songName;
	newSong->artist = artist;
	newSong->genre = genre;
	newSong->nextNode = nextNode;
	return newSong;
}

Node* findNodeByName(Node *list, char *songName){
	Node* currentNode = list;
	while(currentNode != NULL){
		if(strcmp(currentNode->songName, songName) == 0)
			return currentNode;
		currentNode = currentNode->nextNode;
	}
	return currentNode; // points to NULL if it reaches this point
}

void printSearchedSong(Node* list, char *songName){
	Node *song = findNodeByName(list, songName);
	if(song != NULL){
		songNameFound(songName);
		printf("\n%s\n%s\n%s\n", song->songName, song->artist, song->genre);
	}else{
		songNameNotFound(songName);
	}
}

void printList(Node* list){
	if(list != NULL){
		printMusicLibraryTitle();
		Node *currentNode = list;
		
		while(currentNode != NULL){
			printf("\n%s\n%s\n%s\n", currentNode->songName, currentNode->artist, currentNode->genre);
			currentNode = currentNode->nextNode;
		}
	} else {
		printMusicLibraryEmpty();
	}
}
