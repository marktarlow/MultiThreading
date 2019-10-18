#ifndef A0_CSV_MOVIE_H
#define A0_CSV_MOVIE_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <pthread.h>

struct _movie{
	char* key;
	char* movieData;
};
typedef struct _movie movie;

struct _sortCSVArguments
{
	char* argSort;
	struct dirent* argEntry;
	char* argDirectory;
	char* argOutput;
        
};
typedef struct _sortCSVArguments sortCSVArguments;


//Main Process Functions
void* exploreFiles(void* direct);
void* sortCSV(void* parameter);


//Sub Functions
char* getFilePath(struct dirent* entry, char* directory);
char* getOutLocation(char* reqSort, char* reqOutPath);
void printCSV(int order[], int numEntries, char* headerRow, char* outPath);

int isCSV(char* temp);
char* readFile(char* fileName);
void isValidSort(char* sortColumn);
int getSortColumn(char* sortColumn, char* inputText);
int getSortSize(char* inputText);
movie* makeMovie (int columnNumber, char* inputText);
char* saveHeader(char* inputFile);
char* findKey(char* rowText, int columnNumber);
char* removeSpaces(char* stringToTrim);
int isNumeric(int numEntries);
void incereaseMovie();
void offsetChange(int stat);
//Merge Sort Functions
int* mergeSort(char** dataKeyArray, int numEntries, int numeric);
int sortHelper(char* arrayToSort[], int lPointer, int rPointer, int numeric, char* auxiliary[]);
int merge(char* arrayToMerge[], int lPointer, int middlePointer, int rPointer, int numeric, char* auxiliary[]);

#endif
