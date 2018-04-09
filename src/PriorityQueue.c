#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/PriorityQueue.h"
void Queue_Init(PriorityQueue* myQueue,PriorityQueue* *start,PriorityQueue* *end,int id,double score){
	myQueue=malloc(sizeof(PriorityQueue));
	myQueue->id=id;
	myQueue->score=score;
	myQueue->next=NULL;
	*start=myQueue;
	*end=myQueue;
}
int InQueue(PriorityQueue* myQueue,PriorityQueue* *start,PriorityQueue* *end,int id){
	PriorityQueue * temp=*start;
	while(temp!=NULL){ 
		if(temp->id==id) return 1; //if already in queue
		temp=temp->next;
	}
	return 0;
}
void Queue_Insert(PriorityQueue* myQueue,PriorityQueue* *start,PriorityQueue* *end,int id,double score,char**myMap){
	/*if reached here, I am about to place a new document id*/
	if((*start)->score<=score){ //insert at 
		PriorityQueue * newNode=malloc(sizeof(PriorityQueue));
		PriorityQueue *tmp=(*start);
		newNode->id=id;
		newNode->score=score;
		newNode->next=tmp;
		(*start)=newNode;
	}	
	else if((*end)->score>=score){//insert at end
		PriorityQueue * newNode=malloc(sizeof(PriorityQueue));
		newNode->id=id;
		newNode->score=score;
		(*end)->next=newNode;
		(*end)=(*end)->next;
		(*end)->next=NULL;
	}
	else{ //insert somewhere in the middle
		PriorityQueue * iterator=(*start);
		while(score<iterator->next->score) iterator=iterator->next;
		/*iterator points at the node AFTER which we want to place the new one*/
		PriorityQueue * newNode=malloc(sizeof(PriorityQueue));
		newNode->id=id;
		newNode->score=score;
		newNode->next=iterator->next;
		iterator->next=newNode;
	}
}

void Queue_Print(PriorityQueue* myQueue,PriorityQueue* *start,PriorityQueue* *end,char**myMap,int resultLimit,char**query_words,int numOfwords){
	int i=0;
	int len=0; 
	char **wordSep;
	struct winsize ws;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
	int lineSize=ws.ws_col;
	PriorityQueue *iterator=(*start);
	while((iterator!=NULL)&&(i<resultLimit)){
		//parenthesis,dot,squarebrackets,space before article_id,space after score
		len=9+GetOutputLength(i+1,iterator->id,iterator->score);
		if(len>lineSize){
			printf("Too small terminal size!\n");
			exit(0);
		}

		printf("%d.( %d)",i+1,iterator->id );
		printf("[%.*lf] ",6, iterator->score); //always print 6 decimal digits
		int ctr=CountWords(myMap[iterator->id],strlen(myMap[iterator->id])); //how many words this line has
		wordSep=malloc(ctr*(sizeof(char*)));
		int *sizes=malloc(ctr*sizeof(int));
		CountWordLength(myMap[iterator->id],sizes); //length of each word
		for(int i=0;i<ctr;i++)	wordSep[i]=malloc((sizes[i]+1)*sizeof(char));
		SaveWords(wordSep,myMap[iterator->id]); //get each word separately
		myPrint(ctr,query_words,wordSep,numOfwords,lineSize,len);
		printf("\n");
		iterator=iterator->next;
		i++;
		for(int j=0;j<ctr;j++)free(wordSep[j]);
		free(wordSep);
		free(sizes);
	}
}
void Queue_Destroy(PriorityQueue* myQueue,PriorityQueue* *start){
	PriorityQueue *iterator=(*start);
	PriorityQueue *temp;
	while(iterator!=NULL){
		temp=iterator->next;
		free(iterator);
		iterator=temp;
	}
	free(iterator);
}


