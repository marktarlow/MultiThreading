#include "multiThreaderSorter_thread.h"

//Pthread count is size of pthreads array
//PTHREAD COUNT OUTPUT
char* sort;
char* output;
movie* movieArray;
int movieOffset = 0;
int movieArraySize = 400;
int movieArraySizeCount = 0;
int pthreadOffset = 1;
int* pthreads;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char* argv[]){
	//Gathering Input 
        char directory[FILENAME_MAX];        
        movieArray = (movie*) malloc(sizeof(movie) * movieArraySize);  
       
        pthreads = (int*) malloc(sizeof(int) * 100);
	int counter;

	for(counter = 0; counter < argc; counter++){
		if(strcmp(argv[counter], "-c") == 0){
			if(argc < counter+1){
				printf("No Sort Column, not enough arguments. Exiting.");
				exit(0);
			}
			else{
				sort = argv[counter+1];
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
	}
	//Finish Gathering Input



	printf("Initial Process ID: %d\n", getpid());
	printf("PIDs of Spawned Threads:\n");

	pthread_t thread;
        offsetChange(1);
      	if(pthread_create(&thread, NULL, exploreFiles, directory))
	{
	  offsetChange(-1);
	  printf("Critical Error While Creating Thread!\n");
	  exit(-1);
	}
       
	
	
	 
	    
	      pthread_join(thread, NULL);
	    
	    pthread_exit(NULL);
	return 0;
}

void* exploreFiles(void* direct){
  
	//Initialize Variable
        DIR* dir;
	struct dirent* entry;
	char* directory = (char*)direct;
	
	
	if(!(dir = opendir(directory))){
	     	printf("Not a valid directory\n");
		exit(0);
	}
	entry = readdir(dir);
	entry = readdir(dir);
	//Finish Open Directory

	//While loop through directory
	while((entry = readdir(dir)) != NULL){
	 
		if(entry->d_type == DT_REG){
		 	
		  
		                  if(isCSV(entry -> d_name))
				  {
				    sortCSVArguments* arguments = (sortCSVArguments*) malloc(sizeof(sortCSVArguments));
				    arguments -> argSort = sort;
				    arguments -> argEntry = entry;
				    arguments -> argDirectory = directory;
				    arguments -> argOutput = output;
				    
				    pthread_t thread;
				    offsetChange(1);
				    printf("Debugger 2\n");
				    if(pthread_create(&thread, NULL, sortCSV, arguments))
					   {
					     offsetChange(-1);
					     printf("Critical Error While Creating Thread!\n");
					     exit(-1);
					   }
				  
				      
				    
				      }

				 
				
			
		}
		//If folder
		else if(entry->d_type == DT_DIR){
		                
		  
		  //char* temp =  getFilePath(entry, directory);
		                 char* temp =  getFilePath(entry, directory);
				 char* temp1 = (char*) malloc((strlen(temp) + 1) * sizeof(char));
				 memcpy(temp1, temp, strlen(temp) + 1);
				 
				 pthread_t thread;
				 offsetChange(1);
				 printf("%s\n", temp1);
				 printf("Debugger 1\n");
			       	if(pthread_create(&thread, NULL, exploreFiles, temp1))
				  {
				    offsetChange(-1);
				    printf("Critical Error While Creating thread!\n");
				    exit(-1);
				  } 
				
				free(temp); 
				pthread_exit(NULL);
			
		}
	}
	//End While Loop

	//Wait loop for process counter
//	while(wait(&status) != -1){
//	count += WEXITSTATUS(status);
//	}
 
	
}

char* getFilePath(struct dirent* entry, char* directory){
	char* temp = malloc(sizeof(char)*(strlen(entry->d_name)+strlen(directory)+2));

	strncpy(temp, directory, strlen(directory));
	strncat(temp, "/", 1);
	strncat(temp, entry->d_name, strlen(entry->d_name));
	strncat(temp, "\0", 1);

	return temp;
}

void* sortCSV(void* parameter){
        printf("Debugger 3\n");
        sortCSVArguments* arguments = (sortCSVArguments*) parameter;
        char* reqSort = arguments -> argSort;
        struct dirent* entry = arguments -> argEntry;
        char* directory = arguments -> argDirectory;
        char* outPath  = arguments -> argOutput;
	free(arguments);

	char* filePath = getFilePath(entry, directory);
	
	char* fileName = entry->d_name;
	printf("%s 123\n", filePath);
	char* unsortedText = readFile(filePath);
	printf("Debugger 4\n");
	int columnNumber = getSortColumn(reqSort, unsortedText); //SEGFAULT
	printf("%d\n", columnNumber);
	printf("%s 124\n", filePath);
	int numEntries = getSortSize(unsortedText);

	char* headerRow = saveHeader(unsortedText); //might have to edit this
	
	if(strstr(headerRow, reqSort) == NULL){
		printf("%s does not contain column %s", filePath, reqSort);
		return;
	}
	
	
	 if(pthread_mutex_trylock(&lock))
	    {
	      printf("Mutex Could Not Lock!\n");
	    }
	  
	 if(((movieArraySizeCount + 5) >= movieArraySize) || ((movieArraySize - movieArraySizeCount) < numEntries))
	  {
	    movieArraySize = movieArraySize * 2;
	    movieArray = realloc(movieArray, (sizeof(movie) * movieArraySize));
	    }

	
	int i;
	for(i = 0; i < numEntries; i++){
	 
	  makeMovie(columnNumber, unsortedText);
	  movieArraySizeCount++;
	}
	
	 free(unsortedText);
	
	char* dataKeyArray[numEntries];
	for (i = 0; i < numEntries; i++){
	  dataKeyArray[i] = (movieArray + i) ->key;
	}
	
	pthread_mutex_unlock(&lock);

	//int* printOrder = mergeSort(dataKeyArray, numEntries, isNumeric(movieArray, numEntries));
	
	

	//char* outputLocation = getOutLocation(reqSort, filePath, outPath);
	
	printf("%s\n", fileName);
	
	//printCSV(printOrder, movieArray, numEntries, headerRow, outputLocation);
	
	free(headerRow);
	/*for(i = 0; i<numEntries; i++){
	        free((movieArray + i)->key);
		free((movieArray + i)->movieData);
		
	}
	free(movieArray);*/
	pthread_exit(NULL);
}

char* getOutLocation(char* reqSort, char* filePath, char* reqOutPath){
	if(reqOutPath != NULL){
	                char* fileName = "AllFiles";
			char* temp = malloc(sizeof(char)*(strlen(reqOutPath)+strlen(fileName)+strlen(reqSort)+9));
			strncpy(temp, reqOutPath, strlen(reqOutPath));
			memset(temp, '\0', sizeof(temp));
			strncat(temp, fileName, (strlen(fileName)-4));
			strncat(temp, "-sorted-", 8);
			strncat(temp, reqSort, strlen(reqSort));
			strncat(temp, ".csv", 4);
			strncat(temp, "\0", 1);
			
			return temp;
		}
		else{
		        char* fileName = "AllFiles";
			char* temp = malloc(sizeof(char)*(strlen(fileName)+strlen(reqSort)+9));
			memset(temp, '\0', sizeof(temp));
			strncpy(temp, fileName, strlen(fileName));
			strcat(temp, "-sorted-");
			strcat(temp, reqSort);
			strcat(temp, ".csv");
		
			return temp;
		}
}


void printCSV(int* order, movie* array, int numEntries, char* headerRow, char* outPath){
	FILE* fp;
	int i;
	fp = fopen(outPath, "w");
	fprintf(fp, "%s\n", headerRow);
	for(i = 0; i < numEntries; i++){
	fprintf(fp, "%s\n", (array[order[i]] -> movieData)); 
		}
	
	fclose(fp);
}


int isCSV(char* temp){
 
        char* type = ".csv";
	if(strstr((temp+strlen(temp)-4), type) != NULL){
		return 1;
	}
	else{
		return 0;
	}
}

char* readFile(char* fileName){
	int bufferSize= 1024;
	char* inputBuffer = malloc(sizeof(char)*(bufferSize));
	int i = 0;
	FILE* input = fopen(fileName, "r");

	while(feof(input)==0){
		*(inputBuffer + i) = fgetc(input);
		i++;
		if(feof(input)){
			break;
		}
		if(i == (bufferSize-1)){
			bufferSize = bufferSize*1.2;
			inputBuffer = realloc(inputBuffer, sizeof(char)*(bufferSize));
		}
	}
	return inputBuffer;
}

void isValidSort(char* sortColumn){
	char* headerRow = "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes";
	if(strstr(headerRow, sortColumn) != NULL){
		return;
	} 
	else{
		printf("Not a valid sort column!\n");
		exit(1);
	}
}

int getSortColumn(char* sortColumn, char* inputText){
	int i = 0;
	char* tempString = malloc(sizeof(char)*(strlen(inputText) + 1));
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
	char* tempString = strtok(inputFile, "\n");
	char* headerRow = malloc(sizeof(char)*strlen(tempString));
	strcpy(headerRow, tempString);
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

void makeMovie (int columnNumber, char* inputText){
  
	inputText = strtok(NULL, "\n");
	char* currentRow = malloc(sizeof(char)*strlen(inputText)+1);
	strncpy(currentRow, inputText, strlen(inputText)+1);
	(movieArray + movieOffset)->movieData = currentRow;
	char* tempKey = findKey(inputText, columnNumber);
	char* currentKey = malloc(sizeof(char)*strlen(tempKey)+1);
	strncpy(currentKey, tempKey, strlen(tempKey)+1);
	(movieArray + movieOffset)->key = currentKey;
	movieOffset++;
}

int isNumeric(movie* movieArray, int numEntries){
	srand(time(NULL));
	int testKey = rand() % numEntries;
	if(isalpha((movieArray + testKey)->key)){
		return 0;
	} else{
		return 1;
	}
} 

void offsetChange(int stat)
{
  pthread_mutex_lock(&lock2);
  if(stat == 1)
    {
      *(pthreads + pthreadOffset) = pthreadOffset;
      pthreadOffset++;
    }
  else
    {
      *(pthreads + pthreadOffset) = 0;
      pthreadOffset--;
    }
  pthread_mutex_unlock(&lock2);
}
