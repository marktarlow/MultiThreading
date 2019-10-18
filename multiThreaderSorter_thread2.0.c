#include "multiThreaderSorter_thread.h"
//movie realloc
//key realloc 

int procID;
//int* csvReturn;
char* sort;
char* output;
movie** movieArray;
int mallocSize = 25000; 
int reallocHelper = 0;
int numberOfThreads;       
pthread_t* allPthreads;
int pthreadArrayOffset = 0;
char** dataKeyArray;
int arrayOffset = 0;
int mergeSortNumEntries = 0;
char absoluteStartingPath[FILENAME_MAX];
//Sort Array Pointers
char* headerRow;


pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock4 = PTHREAD_MUTEX_INITIALIZER;                                     

int  main(int argc, char* argv[]){
	//Gathering Input
        char directory[FILENAME_MAX];
	int counter; 
	dataKeyArray = (char**) malloc(mallocSize * sizeof(char*));
	movieArray = (movie**) malloc(mallocSize *sizeof(movie*));
	allPthreads = (pthread_t*)malloc(sizeof(pthread_t) * 100);
	procID = getpid();
	//	csvReturn = (int*)malloc(sizeof(int));
	//csvReturn[0] = 1;
	 
          for(counter = 0; counter < argc; counter++){
		if(strcmp(argv[counter], "-c") == 0){
			if(argc < counter+1){
				printf("No Sort Column, not enough arguments. Exiting.");
				exit(0);
			}
			else{
				sort = argv[counter+1];
				isValidSort(sort);
			}
		}
		if(strcmp(argv[counter], "-d") == 0){
			if(argc < counter+1){
				printf("No Source Directory, not enough arguments. Using default behavior.");
				exit(0);
			}
			else{
				strcpy(directory, argv[counter+1]);
			}
		}
		if(strcmp(argv[counter], "-o") == 0){
			if(argc < counter+1){
				printf("No Output Directory, not enough arguments. Using default behavior.");
			}
			else{
				output = argv[counter+1];
			}
		}
	}

	if(directory[0] == NULL){
		getcwd(directory, FILENAME_MAX);
		getcwd(absoluteStartingPath, FILENAME_MAX);
	}

	//exploreFilesArguments explore = {directory, NULL);
	
	pthread_t thread; 
        if(pthread_create(&thread, NULL, exploreFiles, directory))
	  {
	    printf("Critical Error, Could Not Create Thread\n");
	    exit(-1);
	  }
        pthread_join(thread, NULL);
	  
	  //char* spawnedThreads = (char*)malloc(sizeof(char) * 100);


        int* printOrder = mergeSort(dataKeyArray, mergeSortNumEntries, isNumeric(mergeSortNumEntries));

	char* outputLocation = getOutLocation(sort, output);
        
	printCSV(printOrder, mergeSortNumEntries, headerRow, outputLocation);
        
	free(headerRow);
	

	
	
	return 0;
	
}

void* exploreFiles(void* direct){
	//Initialize Variables
        
       
	DIR* dir;
	struct dirent* entry;
	char* directory = (char*)direct;
     
	

	//Finish Initialization

	//Open Directory
	if(!(dir = opendir(directory))){
		printf("Not a valid directory\n");
		exit(0);
	}
	entry = readdir(dir);
	entry = readdir(dir);
	//Finish Open Directory

	//While loop through directory
	
	while((entry = readdir(dir)) != NULL){
	  	  
		//If regular file
		if(entry->d_type == DT_REG){
		  
		   if(isCSV(entry -> d_name))
				 {
				   //		    pthread_mutex_lock(&lock);      
				   sortCSVArguments*  arguments = (sortCSVArguments*) malloc(sizeof(sortCSVArguments));
				    arguments -> argSort = sort;
				    arguments -> argEntry = entry;
				    arguments -> argDirectory = directory;
				    arguments -> argOutput = output;
				    
				    /* pthread_t thread;
				    if(pthread_create(&thread, NULL, sortCSV, (void*)arguments))
				      {
					
					printf("Critical Error Creating Threads!");
					exit(-1);
				      }
				    pthread_join(thread, NULL);
				    allPthreads[pthreadArrayOffset] = thread;
				    pthreadArrayOffset++;
				    printf("Initial PID %d\n", procID);
				    printf("TIDS of all spawned threads: ");
				    int i;
				    for(i = -1; i < pthreadArrayOffset; i++)
				      {
					printf("%d, ", allPthreads[pthreadArrayOffset]);
				      }
				    printf("\n");
				    printf("Total number of threads: %d\n", pthreadArrayOffset);
 
				     pthread_mutex_unlock(&lock);
				     
				     pthread_exit(NULL);*/
		  sortCSV(arguments);
				  }
				 
			}
		
		//If folder
		else if(entry->d_type == DT_DIR){
		  
		  ///pthread_mutex_lock(&lock2);
				 char* temp =  getFilePath(entry, directory);
				 char* temp1 = (char*) malloc((strlen(temp) + 1) * sizeof(char));
				 memcpy(temp1, temp, strlen(temp) + 1);
				
				 /* pthread_t thread;
				 if(pthread_create(&thread, NULL, exploreFiles, (void*)temp1))
				   {
				     
				     printf("Critical Error Creating Threads!");
				     exit(-1);
				   }
				 pthread_mutex_unlock(&lock2);
					free(temp1);
					pthread_join(thread, NULL);
					pthread_exit(NULL);*/
		  exploreFiles(temp1);
				
		       
			}
		
		}
       

	pthread_exit(NULL);  
}

char* getFilePath(struct dirent* entry, char* directory){

        char* temp = malloc(sizeof(char)*(strlen(entry->d_name)+strlen(directory)+2));
	memset(temp, '\0', (strlen(entry->d_name)+strlen(directory)+2));
	strncpy(temp, directory, strlen(directory));
	strncat(temp, "/", 1);
	strncat(temp, entry->d_name, strlen(entry->d_name));
	strncat(temp, "\0", 1);
        
	return temp;
}

void* sortCSV(void* parameter){
  pthread_mutex_lock(&lock3);
  sortCSVArguments* arguments = (sortCSVArguments*) parameter;
        char* reqSort = arguments -> argSort;
        struct dirent* entry = arguments -> argEntry;
        char* directory = arguments -> argDirectory;
        char* outPath  = arguments -> argOutput;
	
        
	char* filePath = getFilePath(entry, directory);
       	
	char* fileName = entry->d_name;

	char* unsortedText = readFile(filePath);

        
	printf("%s\n", fileName);

        int columnNumber = getSortColumn(reqSort, unsortedText);

	int numEntries = getSortSize(unsortedText);
       
	headerRow = saveHeader(unsortedText);


	if(strstr(headerRow, reqSort) == NULL){
		printf("%s does not contain column %s", filePath, reqSort);
		return;
	}
  

      	int i;
	for(i = 0; i < numEntries; i++){
	  
	  if((reallocHelper + 200) >= mallocSize)
	    {
	      mallocSize *= 1.6;
	      movieArray = (movie**)realloc(movieArray, (sizeof(movie*) * mallocSize));
	      dataKeyArray = (char**)realloc(dataKeyArray, (sizeof(char*) * mallocSize));
	    }
	  movieArray[arrayOffset + i] = (movie*)malloc(sizeof(movie));
          movieArray[arrayOffset + i] = makeMovie(columnNumber, unsortedText);
	  reallocHelper++;
	}

	free(unsortedText);
        
	 

	for (i = 0; i < numEntries; i++){
	 
	  dataKeyArray[arrayOffset + i] = (char*)malloc(strlen(movieArray[arrayOffset + i] -> key) + 1);
	  dataKeyArray[arrayOffset + i] = (movieArray[arrayOffset + i] -> key);
	 
	  
	  } 
	printf("For: %s - mergeSortNumEntries: %d - arrayOffset: %d\n", fileName, mergeSortNumEntries, arrayOffset);
	mergeSortNumEntries = mergeSortNumEntries + numEntries;	
	arrayOffset = arrayOffset + numEntries;

	pthread_mutex_unlock(&lock3);
	
//arrayOffset = arrayOffset + (numEntries + 1);
	/*for(i = 0; i<numEntries; i++){
		free(movieArray[i]->key);
		free(movieArray[i]->movieData);
		free(movieArray[i]);
		}*/
		

	// int tid = assignTid(); 
	
	pthread_exit(NULL);
}


char* getOutLocation(char* reqSort, char* reqOutPath){
                   
	if(reqOutPath != NULL){
	                
			char* temp = malloc(sizeof(char)*(strlen(reqOutPath)+ strlen(reqSort)+ 21));
			memset(temp, '\0', sizeof(temp));
			strncpy(temp, reqOutPath, strlen(reqOutPath));
			strncat(temp, "/AllFiles", 8);
			strncat(temp, "-sorted-", 8);
			strncat(temp, reqSort, strlen(reqSort));
			strncat(temp, ".csv", 4);
			strncat(temp, "\0", 1);
			
			printf("Temp 1 - %s\n", temp);
			return temp;
		}
		else{
		  int tempSize = (strlen(absoluteStartingPath) + strlen(reqSort) + 22);
		  char* temp = malloc(sizeof(char) * tempSize);
		        memset(temp, '\0', tempSize);
			strncpy(temp, absoluteStartingPath, strlen(absoluteStartingPath));
			strncat(temp, "/AllFiles", 9);
			strncat(temp, "-sorted-", 8);
			strncat(temp, reqSort, strlen(reqSort));
			strncat(temp, ".csv", 4);
			strncat(temp, "\0", 1);
		
		        
			return temp;
		}
}

void printCSV(int* order, int numEntries, char* headerRow, char* outPath){
	FILE* fp;
	int i;
	printf("WRITE OUTPATH - %s\n", outPath);
	fp = fopen(outPath, "w");
        printf("OPENED\n");
	printf("arrayOffset - %d\n", arrayOffset);
	fprintf(fp, "%s\n", headerRow);
	printf("Possible Seg Fault 1\n");
	 printf("Num Entries  - %d\n", numEntries);
	printf("Array Offset  - %d\n", arrayOffset);/////////////////////////////////
	for(i = 0; i < numEntries; i++){
	 
	  fprintf(fp, "%s\n", (movieArray[order[i]]  -> movieData));
	 
	  //printf("%s\n", (movieArray[arrayOffset + order[i]]  -> movieData));
		}
	printf("Possible Seg Fault 2\n");
	fclose(fp);

}


int isCSV(char* temp){
	char* type = ".csv";
	printf("%s\n", temp);
	if((strstr((temp+strlen(temp)-4), type) != NULL)){
		return 1;
	}
	else{
		return 0;
	}
}

char* readFile(char* filePath){
       
	int bufferSize= 1024;
   
	char* inputBuffer = (char*)malloc(bufferSize * sizeof(char));
        
	int i = 0;

	FILE* input = fopen(filePath, "r"); 
	while(feof(input)==0){
		*(inputBuffer + i) = fgetc(input);
		i++;
		if(feof(input)){
			break;
		}
		if(i == (bufferSize-1)){
			bufferSize = bufferSize*1.2;
			inputBuffer = (char*)realloc(inputBuffer, sizeof(char)*(bufferSize));
		}
	}
	fclose(input);
	return inputBuffer;
	
}

void isValidSort(char* sortColumn){
	char* head = "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes";
	
	if(strstr(head, sortColumn) != NULL)
	    {
	      return;
	    }
	  else
	    {
	      	printf("Not a valid sort column!\n");
		exit(1);
	    }
	  
	

	
}

int getSortColumn(char* sortColumn, char* inputText){
        int i = 0;
        char* tempString = (char*)malloc(sizeof(char)*(strlen(inputText) + 1));
	char* part = tempString;
	strncpy(part, inputText, strlen(inputText) + 1);
	part = strtok(part, "\n");
	part = strtok(part, ",");
	while(strncmp(part, sortColumn, strlen(sortColumn)) != 0){
		part = strtok(NULL, ",");
		i++;
	}
	free(tempString);
	return i;
}

int getSortSize(char* inputText){

	char* counterText = inputText;
	int lines = 0;
	do{
		if (*counterText == '\n'){
			lines++;
		}
	} while (*(counterText++));
	return lines-1;
}

char* saveHeader(char* inputFile){
        	
  inputFile = strtok(inputFile, "\n");
  char* headerRow = malloc(sizeof(char)*strlen(inputFile));
	strcpy(headerRow, inputFile);
	return headerRow;
	}


char* findKey(char* rowText, int columnNumber){
	char tempArray[strlen(rowText)];
	int commasCounted = 0;
	int i = 0;

	for(i = 0; i < strlen(rowText); i++){
		if(commasCounted == columnNumber){
			if(rowText[i] == ','){
				return "0000";
			}
			break;
		}

		if(rowText[i] == '\"'){
			i++;
			while(rowText[i] != '\"'){
				i++;
			}
		}

		if(rowText[i] == ','){
			commasCounted++;
		}
	}

	int j = 0;

	for(i = i; i < strlen(rowText); i++){
		if(rowText[i] == '\"'){
			i++;
			while(rowText[i] != '\"'){
				tempArray[j] = rowText[i];
				i++;
				j++;
			}
			break;
		}
		else if (rowText[i] == ','){
			break;
		}
		else {
			tempArray[j] = rowText[i];
			j++;
		}
	}
	tempArray[j] = '\0';
	char* tempString = removeSpaces(tempArray);
	return tempString;
}

char* removeSpaces(char* stringToTrim){
	char *rightEnd;

	while (isspace((unsigned char)*stringToTrim)){
		stringToTrim++;
	}

	if (*stringToTrim == 0){
		return stringToTrim;
	}

	rightEnd = stringToTrim + strlen(stringToTrim) - 1;

	while (rightEnd > stringToTrim && isspace((unsigned char)*rightEnd)) {
		rightEnd--;
	}
	
	rightEnd[1] = '\0';

	return stringToTrim;
}

movie* makeMovie (int columnNumber, char* inputText){
	inputText = strtok(NULL, "\n");
	movie* currentMovie = malloc(sizeof(movie));
	char* currentRow = malloc(sizeof(char)*strlen(inputText)+1);
	strncpy(currentRow, inputText, strlen(inputText)+1);
	currentMovie->movieData = currentRow;
	char* tempKey = findKey(inputText, columnNumber);
	char* currentKey = malloc(sizeof(char)*strlen(tempKey)+1);
	strncpy(currentKey, tempKey, strlen(tempKey)+1);
	currentMovie->key = currentKey;
	
	return currentMovie;

}

int isNumeric(int numEntries){
	srand(time(NULL));
	int testKey = rand() % numEntries;
	if(isalpha(*movieArray[testKey]->key)){
	  
		return 0;
	} 
	else{
	  
		return 1;
	}
	
}

 

/*void cdStruct(int param)
{
  //pthreadStruct type
  pthread_mutex_lock(&lock2);
  

  pthread_mutex_unlock(&lock2);
}*/

/*void offsetChange(int stat)
{

  pthread_mutex_lock(&lock2);
  if(stat == 1)
    {
      pthreadIDs[pthreadOffset] = pthreadOffset + 1;
      pthreadOffset++;
    }
  else
    {
      pthreadIDs[pthreadOffset] = 0;
      pthreadOffset--;
    }
  printf("OffsetChangeEnd\n");
  pthread_mutex_unlock(&lock2);
  }*/

/*int assignTid()
{
  pthread_mutex_lock(&lock2);
  int retVal;
  retVal = pthreadID;
  pthreadID ++;
  pthread_mutex_unlock(&lock2);
  return retVal;
  
}*/
