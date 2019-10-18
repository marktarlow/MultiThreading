#include "multiThreaderSorter_thread.h"

int* mergeSort(char** dataKeyArray, int numEntries, int numeric){

        char** beforeSort = malloc(sizeof(char*) * numEntries);
	int* printingSequence = malloc(sizeof(int)*numEntries);
	char* auxiliary[numEntries];
	int lPointer, rPointer;
	

	int i;
	for (i = 0; i < numEntries; i++){
	  	beforeSort[i] = malloc(strlen(dataKeyArray[i]) + 10);
	  	strcpy(beforeSort[i], dataKeyArray[i]);
	        
        
	}
        

	lPointer = 0;
	rPointer = numEntries - 1;
	int sortCheck = sortHelper(dataKeyArray, lPointer, rPointer, numeric, auxiliary);
	if (sortCheck != 1) {printf("Error: Merge sort failed!\n"); return NULL;}

	int j = 0, k = 0;
	for (i = 0; i < numEntries; i++){
		for (j = 0; j < numEntries; j++){
			if (strcmp(dataKeyArray[i], beforeSort[j]) == 0){
				char index[15];
				sprintf(index, "%d", j);
				beforeSort[j] = strcat(beforeSort[j], index);
				printingSequence[k] = j;
				k++;
				break;
			}
		}
	}

	for (i = 0; i < numEntries; i++){
		free(beforeSort[i]);
	}
	free(beforeSort);

	return printingSequence;
}

int sortHelper(char* arrayToSort[], int lPointer, int rPointer, int numeric, char* auxiliary[]){
	int middlePointer;

	if (lPointer < rPointer){
		middlePointer = (lPointer + rPointer) / 2;

		sortHelper(arrayToSort, lPointer, middlePointer, numeric, auxiliary);
		sortHelper(arrayToSort, middlePointer + 1, rPointer, numeric, auxiliary);

		merge(arrayToSort, lPointer, middlePointer, rPointer, numeric, auxiliary);

	} else {
		return 1;
	}
	return 1;
}

int merge(char* arrayToMerge[], int lPointer, int middlePointer, int rPointer, int numeric, char* auxiliary[]){
	int i, leftScan, rightScan;

	for (leftScan = lPointer, rightScan = middlePointer + 1, i = lPointer; leftScan <= middlePointer && rightScan <= rPointer; i++){
		if (numeric){
			if (atof(arrayToMerge[leftScan]) <= atof(arrayToMerge[rightScan])){
				auxiliary[i] = arrayToMerge[leftScan++];
			} else {
				auxiliary[i] = arrayToMerge[rightScan++];
			}
		} else {
			if (strcmp(arrayToMerge[leftScan], arrayToMerge[rightScan]) <= 0){
				auxiliary[i] = arrayToMerge[leftScan++];
			} else {
				auxiliary[i] = arrayToMerge[rightScan++];
			}
		}
	}

	while (leftScan <= middlePointer){
		auxiliary[i++] = arrayToMerge[leftScan++];
	}

	while (rightScan <= rPointer){
		auxiliary[i++] = arrayToMerge[rightScan++];
	}

	for (i = lPointer; i <= rPointer; i++){
		arrayToMerge[i] = auxiliary[i];
	}

	return 1;
}
