#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

//Prepare stream for necessary files
FILE *infile;
FILE *outfile;
FILE *dict;

//struct for dictionary words. Every dictionary struct contains
//a word and an associated count.
typedef struct dictword {
	char word[100];
	int count;
}dictword;

//create Dict uses words in the dictionary file to create an array of the dictword structs
void createDict(FILE **dictFile, dictword **dictArray, int *numWords); 

int main(int argc, char **argv) {
	//Set up variables for parsing and file read/write
	int fflag = 0;
	char *fvalue = NULL;
	int wflag = 0;
	char *wvalue = NULL;
	char *filein; 
	int nonopt[2];
	nonopt[0] = 0;
	nonopt[1] = 0;
	int index;
	char c;
	dictword *dictArray;
	int nonarg = 0;
	if (argc < 2) {
		printf("More arguments needed\n");
		return 1;
	}
	//Parse command line arguments
	while (1)
	{
		c = getopt(argc, argv, "f:w:");
		if (c == -1) {
			break;
		}
		switch (c)
		{
			case 'f':
				fflag = 1;
				fvalue = optarg;
				break;
			case 'w':
				wflag = 1;
				wvalue = optarg;
				break;
		}
		for (index = optind; index < argc; index++) {
			filein = argv[index];
			nonopt[nonarg] = index;
			nonarg++;
		}
	}
	//Open files for read/write and/or create dictionary array based
	//on command line arguments.
	int num;
	if (fflag) {
		dict = fopen(fvalue,"r");
		if (dict == NULL) {
			printf("Dictionary file not found");
			return 1;
		}
		createDict(&dict, &dictArray, &num);
	} else {
		dictArray = malloc(sizeof(dictword));
		strncpy(dictArray[0].word,argv[1],99);
		dictArray[0].count = 0;
		num = 1;
	}

	//If -w flag then create file for write

	if (wflag) {
		outfile = fopen(wvalue,"w");
	}

	if (!fflag && !wflag) {
		infile = fopen(argv[2],"r");
	} else {
		infile = fopen(filein,"r");
	}
	if (infile == NULL) {
		printf("input file %s not found\n", argv[2]);
		return 1;
	}

	//Use strtok to get individual strings from file

	char line[61];
	char *token;
	char *saveptr;

	int count = 0;
	char *tok = " .’,;?!'—:\"’";
	int i;
	while (fgets(line, 60, infile) != NULL) 
	{
		token = strtok_r(line, tok, &saveptr);
		while (token !=  NULL) 
		{
			int j;
			//Compare string with all words in the dictionary
			for (j=0; j<num; j++) {
				if (!strcmp(dictArray[j].word,token)) {
					dictArray[j].count++;
				}
			}
			token = strtok_r(NULL, tok, &saveptr);
		}
	}
	//Read words and count from the dictionary and 
	//format them properly
	for (i=0; i<num; i++) {
		char out[100];
		strncpy(out,dictArray[i].word,99);
		strcat(out,",");
		char temp[20] = {};
		snprintf(temp, 19, "%u",dictArray[i].count);	
		strcat(out,temp);
		strcat(out,"\n");
		//Write to file if -w else just print to terminal
		if (!wflag) {
			printf("%s",out);
		} else {
			fprintf(outfile, out);
		}
	}

	//Free memory and close files
	free(dictArray);
	fclose(infile);
	if (wflag) {
		fclose(outfile);
	}
}

void createDict(FILE **dictFile, dictword **dictArray, int *numWords) {
	//Limit string length to 100
	char str[100];
	FILE *dict = *dictFile;
	char *tok = " ";
	char *saveptr;
	int num = 0;
	//Create array of dictWords. Size limit is 100 words
	dictword *dictWords = malloc(sizeof(dictword)*100);
	*dictArray = dictWords;
	//Fill the array with words
	while (fgets(str, 99, dict) != NULL) {
		char c;
		int i = 0;
		//Ignore \n character at the end
		while ((c = str[i]) != '\n') {
			dictWords[num].word[i] = c;
			i++;
		}
		//Set word count of each dictionary word to 0
		dictWords[num].count = 0;
		num++;
	}
	*numWords = num;
	fclose(dict);
}
