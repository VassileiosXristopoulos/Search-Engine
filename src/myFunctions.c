
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../header/myFunctions.h"
#include "../header/Char_Stack.h"

void ThrowError(char *message){
	printf("%s\n",message);
	exit(0);
}
int getlines(char* docName){
	/*source:
	https://stackoverflow.com/questions/14002954/c-programming-how-to-read-the-whole-file-contents-into-a-buffer*/
	FILE* myfile = fopen(docName, "r");
	int ch, lines = 0;
	do{
	    ch = fgetc(myfile);
	    if(ch == '\n') lines++;
	}while (ch != EOF);
	fclose(myfile);
	return lines;
}

void SaveWords(char ** wordSep,char* doc){
	int i,j=0,ctr=0;
	/*source:
	https://www.w3resource.com/c-programming-exercises/string/c-string-exercise-31.php*/
//the technique i use is i read characters and define a word when 
//whitespace is found, therefore there is the danger of having
//consequtive whitespaces and allocating a word, hence the condition
	for(i=0;i<=strlen(doc);i++){
		if(doc[i]==' '|| doc[i]=='\0'){
			if(doc[i-1]!=' ' && doc[i-1]!='\0'){ //avoid allocating spaces as words
				wordSep[ctr++][j]='\0';
				j=0; //set index 0 for next word
			}
		}
		else{
			wordSep[ctr][j]=doc[i];
			j++;
		}
	}
}
int CountWords(char *text,int len){
	int count=0;
	for(int i=0;i<=len;i++){
		if(text[i]==' '|| text[i]=='\0' || text[i]=='\t'){
			if(text[i-1]!=' ' && text[i-1]!='\0' && text[i-1]!='\t'){
		 		count++;
	 		}
		}
	}
	return count;
}
void CountWordLength(char *text,int*sizes){
	/*source:
	http://www.includehelp.com/c-programs/read-a-string-and-print-the-length-of-the-each-word.aspx*/ 
	/*take a whole text, and count length of each word, so as to allocate the
	proper amount of space of each word*/
	int new_ctr=0,j=0;
	for(int i=0;i<=strlen(text);i++){
		if(text[i]==' '|| text[i]=='\0'){
			if(text[i-1]!=' ' && text[i-1]!='\0'){
				sizes[new_ctr++]=j;
				j=0; 
			}
		}
		else j++;
	}	
}
int isQueryWord(char **query_words,char *text,int len){
	for(int i=0;i<len;i++){
		if(strcmp(query_words[i],text)==0) return 1;
	}
	return 0;
}
void BreakLine(int len){
	printf("\n");
	for(int i=0;i<len;i++) printf(" ");
}

double Get_IDF(int n,int nqi){
	return log((n-nqi+0.5)/(nqi+0.5));
}

int GetOutputLength(int outputNum,int id,double score){
	int ret=0;
	int count=2;
	double temp;
	if(score<0) count++; //+1 character
	temp=id;
	while((temp/10)>=1){
		count++;
		temp/=10;
	}
	temp=outputNum;
	while((temp/10)>=1){
		count++;
		temp/=10;
	}
	temp=fabs(score);
	while((temp/10)>=1){
		count++; //scores digits before
		temp/=10;
	}
	ret+=count;	
	return ret+6; //+6 because we always have 6 decimal digits
}

void myPrint(int ctr,char **query_words,char**wordSep,int numOfwords,int terminalSize,int len){
	int charactersPrinted=len; 
	int arrayPosition=0;
	int start_index=0;
	int end_index=0;
	while(1){
		int lengthFit=0;
		while(1){
			lengthFit+=strlen(wordSep[end_index])+1;//+1 for space afterwards
			if(lengthFit<(terminalSize-charactersPrinted)) end_index++;
			else break;
			if(end_index==ctr) break;
		}
		if(end_index>ctr) end_index=ctr;
		/*i can print from start_index until end_index*/
		/*----print the right amount of words and underline-----*/
		for(int i=start_index;i<end_index;i++)printf("%s ",wordSep[i] );
		printf("\n");
		for(int i=0;i<len;i++) printf(" ");
		for(int i=start_index;i<end_index;i++){
			if(isQueryWord(query_words,wordSep[i],numOfwords)){
				for(int k=0;k<strlen(wordSep[i]);k++) printf("^");
			}
			else{
				for(int k=0;k<strlen(wordSep[i]);k++) printf(" ");
			}
			printf(" ");
		}
		/*----------------------print ended!!----------------------*/

		if(end_index==ctr) return;
		BreakLine(len); //the next part of the content will start at the same
		//position as the previous
		start_index=end_index;
	}
}



double GetScore(int doc,double avgdl,char **words,int numOfWords,int N,Trie_node*Root,int D){
	double kl=1.2;
	double b=0.75;
	double score=0;
	int term_frequency;
	for(int i=0;i<numOfWords;i++){//for a certain doc, statistics for every word from query
		Trie_node * iterator=Trie_Search(words[i],Root);
		if(iterator==NULL) continue; //word not found at Trie
		double IDF=Get_IDF(N,iterator->df);
		PostingList * pl_iterator=iterator->pl_ptr;
		while((pl_iterator!=NULL)&&(pl_iterator->article_id!=doc)) pl_iterator=pl_iterator->next;
		if(pl_iterator!=NULL){
			term_frequency=pl_iterator->word_frequency;
			score+=IDF*( ((double)term_frequency*(kl+1)) / (term_frequency+kl*(1-b+b*(abs(D)/avgdl) ) ));
		}
	}
	return score;
}	

void Search_Command(int * wordsPerLine,int fileSize,Trie_node *Root,char **myMap,int resultLimit){
	char* mystring;
	int i=0,cnt=0;
	Stack *mystack=Stack_CreateStack(10); //10 is the maximum amount of elements
	/*at input after given arguments must be NO space */
	if(scanf("%ms",&mystring)!=1) ThrowError("Scanf failed");
	while(i++<10){//if no entries given program keeps waiting 
		Stack_push(mystack,mystring);	
		char ch=getchar();
		free(mystring);
		if(ch=='\n') break;
		if(scanf("%ms",&mystring)!=1) ThrowError("Scanf failed");
	}
	//we dont know how many arguments we are gonna get, so we push them in a stack
	//wordsPerLine contains the num of words per text
	double avgdl=0;
	for(int i=0;i<fileSize;i++) avgdl+=wordsPerLine[i];
	avgdl=avgdl/fileSize; //avg words per line
	
	/*Get query words from stack and place them in array*/
	int numOfWords=Stack_GetSize(mystack)+1; //how many words given to query
	char ** query_words=malloc(numOfWords*sizeof(char*)); //array containing the words
	char * popped;
	while(Stack_pop(mystack,&popped)==1){
		size_t char_len = strlen(popped);
		query_words[cnt]=malloc((strlen(popped)+1)*sizeof(char));
		strcpy(query_words[cnt],popped);
		cnt++;
		free(popped);
	}
	Stack_Destroy(mystack);
	PriorityQueue * myQueue,*start=NULL,*end=NULL;
	int count=0;
	for(int i=0;i<numOfWords;i++){//for each query-word, get df and posting list
		Trie_node * iterator=Trie_Search(query_words[i],Root);
		if(iterator==NULL) continue; //word not found at Trie
		PostingList * list_iterator=iterator->pl_ptr;
		while(list_iterator!=NULL){ //for documents in posting list
			int doc_id=list_iterator->article_id;
			/* for this document, get score for query*/
			if(InQueue(myQueue,&start,&end,doc_id)) break; //if document already computed, ommit it
			double score =  GetScore(doc_id,avgdl,query_words,numOfWords,fileSize,Root,wordsPerLine[doc_id]);
			if(count++==0) Queue_Init(myQueue,&start,&end,doc_id,score);
			else Queue_Insert(myQueue,&start,&end,doc_id,score,myMap);
			list_iterator=list_iterator->next;
		}
	}	

	Queue_Print(myQueue,&start,&end,myMap,resultLimit,query_words,numOfWords);
	Queue_Destroy(myQueue,&start);
	for(int i=0;i<cnt;i++) free(query_words[i]);
	free(query_words);
}



void df_command(Trie_node *Root,char *mystring){
	if(mystring==NULL){//print all words
		Char_Stack_Node *head=NULL;
		Char_Stack_Node *top=NULL;
		Trie_PrintAll(Root->child,&head,&top);
	}
	else{
		Trie_node * iterator=Trie_Search(mystring,Root);
		if(iterator==NULL){
			printf("word not in text\n");
			return;
		}
		printf("%s %d\n",mystring,iterator->df );	
	}
}

void tf_command(Trie_node *Root,char *mystring,int id){
	Trie_node * iterator=Trie_Search(mystring,Root);
	if(iterator==NULL){
			printf("word not in text\n");
			return;
	}
	
	PostingList * list_iterator=iterator->pl_ptr;
	while(list_iterator!=NULL){
		if(list_iterator->article_id==id){
			printf("%d %s %d\n",id,mystring,list_iterator->word_frequency );
			return;
		}
		list_iterator=list_iterator->next;
	}
	printf("%s %d\n",mystring,0 ); //if not in document
	return;
}






