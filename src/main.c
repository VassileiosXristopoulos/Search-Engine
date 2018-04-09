#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/myFunctions.h"

int main(int argv,char*argc[]){
	int resultLimit=10;
	char * docName,*tempPtr;
	long val;
	if(strcmp(argc[1],"-i")==0){
		if(strcmp(argc[3],"-k")!=0) ThrowError("Execution command failed");
		val = strtol(argc[2],&tempPtr,10);
		/*if not string*/
		if(! ((tempPtr == argc[2]) || (*tempPtr != '\0')) ) ThrowError("Invalid file name"); /*not a string*/
		docName= argc[2];
		if(argc[4]!=NULL){ //if limit is given 
			val= strtol(argc[4],&tempPtr,10);
			/*if not number*/
			/*source: 
			https://stackoverflow.com/questions/17292545/how-to-check-if-the-input-is-a-number-or-not-in-c*/
			if ((tempPtr == argc[4]) || (*tempPtr != '\0'))	ThrowError("Output limit invalid");
			resultLimit= atoi(argc[4]);
			if(resultLimit<=0) ThrowError("Result limit must be at least 1");
		}
	}
	else if(strcmp(argc[1],"-k")==0){
		if(strcmp(argc[2],"-i")==0){ //no limit is given, default 10
			val = strtol(argc[3],&tempPtr,10);
			if(! ((tempPtr == argc[3]) || (*tempPtr != '\0')) ) ThrowError("Invalid file name"); /*not a string*/
			docName= argc[3];
		}
		else{ //limit is given
			if(strcmp(argc[3],"-i")!=0) ThrowError("Execution command failed");
			/*check if arguments are ok*/
			val = strtol(argc[4],&tempPtr,10);
			/*if not string*/
			if(! ((tempPtr == argc[4]) || (*tempPtr != '\0')) ) ThrowError("Invalid file name"); /*not a string*/
			docName= argc[4];

			val= strtol(argc[2],&tempPtr,10);
			/*if not number*/
			if ((tempPtr == argc[2]) || (*tempPtr != '\0')) ThrowError("Output limit invalid");
			resultLimit= atoi(argc[2]);
			if(resultLimit<=0) ThrowError("Result limit must be at least 1");
		}

	}
	else ThrowError("Execution command failed");
	/*--------- all input checks confirmed!!! ---------*/
	char ** myMap;
	
	int fileSize=getlines(docName); //how many lines we have
	FILE *filePointer= fopen(docName,"r");
	if(filePointer!=NULL){
		myMap=malloc(fileSize*sizeof(char*));
		int counter=0;
	   	for(int i=0;i<fileSize;i++){ //for each line, separation id-content
	   		char *line=NULL,* content=NULL;
	   		char* tempContent=NULL,*id,* newchar=NULL, *tmpChar=NULL  ;
			size_t len = 0;
			ssize_t read=0;
			/*source:
			https://stackoverflow.com/questions/3501338/c-read-file-line-by-line*/
		    read = getline(&line, &len, filePointer); //get a line
		    int c=0;
		    char *ptr=line; 
		    while((line[c]==' ')||(line[c]=='\t')){ //prevent from tabs-spaces before the id
		    	ptr++;
		    	c++;
		    }
		    tempContent=ptr; //points at the id
		    c=0;
		    while((ptr[c]!=' ') && (ptr[c]!='\t')){  
		    	tempContent++;
		    	c++;
		    } //get pointer right after the id

		    char *content_ptr=tempContent; 
		    c=0;
			while((tempContent[c]==' ')||(tempContent[c]=='\t')){ //prevent from tabs-spaces after the id
		    	content_ptr++;
		    	c++;
		    }
		    /*tem content points right at our text*/
			size_t lengthOfId = content_ptr - ptr;
			id = (char*)malloc((lengthOfId + 1) * sizeof(char));
			/*check for wrong inputs at id's*/
			if(id!=NULL && ptr!= NULL)strncpy(id, ptr, lengthOfId);
			else ThrowError("Text not given properly");
			if((atoi(id))!=counter++)ThrowError("Text not given properly");
			int myid=atoi(id);
			/*content is " sample-text\n" so we remove the additional info*/
			newchar = malloc((strlen(content_ptr)-1)*sizeof(char));
			int word_iterator=0;
			while(content_ptr[word_iterator]!='\0') word_iterator++;
			content_ptr[word_iterator-1]='\0';
			content=content_ptr;
			/*modification completed!*/
			myMap[myid]=malloc((strlen(content)+1)*sizeof(char));
			strcpy(myMap[myid],content);
			free(id);
			free(line);
			free(newchar);
	    }
	}

	/*word separation and store per line */

	Trie_node *Root; //Trie creation
	Root=malloc(sizeof(Trie_node));
	Trie_node_Init(Root);
	int *wordsPerLine=malloc(fileSize*sizeof(int)); //how many words a line has
	for(int k=0;k<fileSize;k++){
		char ** wordSep;
		int j=0,ctr=0;
		/* counting how many words a doc has */
		ctr=CountWords(myMap[k],strlen(myMap[k]));
		wordsPerLine[k]=ctr;
		int *sizes=malloc(ctr*sizeof(int));
		/* counting the length of each word */
		CountWordLength(myMap[k],sizes);
		/*allocating the array on which the words will be stored */
		wordSep=malloc(ctr*sizeof(char*));
		for(int i=0;i<ctr;i++){ 
			wordSep[i]=malloc((sizes[i]+1)*sizeof(char));
		}
		SaveWords(wordSep,myMap[k]); //saving each word to wordSep
		for(int i=0;i<ctr;i++){
			Trie_Insert(wordSep[i],k,Root);
		}
		for(int i=0;i<ctr;i++){
			free(wordSep[i]);
		}
		free(wordSep);
		free(sizes);
		
	}
	printf("give command\n");
	char *command;
	scanf("%ms",&command);
	while(strcmp(command,"/exit")!=0){
		if(strcmp(command,"/search")==0){
			Search_Command(wordsPerLine,fileSize,Root,myMap,resultLimit);
		}
		else if(strcmp(command,"/df")==0){
			int i=0,in=0;
			char *mystring=NULL;
			char ch=getchar();
			while(ch!='\n'){
				in=1;
				scanf("%ms",&mystring);
				df_command(Root,mystring);
				ch=getchar();
			}
			if(in==0) df_command(Root,mystring);
			if(mystring!=NULL) free(mystring);
			
		}
		else if(strcmp(command,"/tf")==0){
			int i=0,command_id;
			char *mystring;		
			scanf("%d",&command_id);
			scanf("%ms",&mystring);
			tf_command(Root,mystring,command_id);
			free(mystring);
		}
		else{
			ThrowError("Action command invalid");
		}
		free(command);
		scanf("%ms",&command);
	}
	free(command);
	Trie_Destroy(Root);

	for(int i=0;i<fileSize;i++) {
		free(myMap[i]);
	}
	free(myMap);
	free(wordsPerLine);
	fclose(filePointer);
}











