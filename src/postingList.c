#include <stdio.h>
#include <stdlib.h>
#include "../header/postingList.h"

int postingList_Update(int doc,PostingList* *pl_ptr){
	PostingList* iterator= *pl_ptr;
	if(iterator==NULL){ //list empty
		*pl_ptr=malloc(sizeof(PostingList));
		(*pl_ptr)->article_id=doc;
		(*pl_ptr)->word_frequency=1;
		(*pl_ptr)->next=NULL;
		return 1;
	}
	else{ //list not empty, search for doc
	int found=0; 
	PostingList *tmp;
		while(iterator!=NULL){
			if(iterator->article_id==doc){
				iterator->word_frequency++;
				found=1;
				break;
			}
			else{
				tmp=iterator;
				iterator=iterator->next;
			}
		}
		if(found==0){ //doesn't exist in list
			iterator= malloc(sizeof(PostingList));
			iterator->article_id=doc;
			iterator->word_frequency=1;
			iterator->next=NULL;
			tmp->next=iterator;
			return 1; //new doc inserted!
		}
	}
	return 0;
}

void postingList_Print(PostingList* *pl_ptr,char* word){
	printf("%s --> ",word );
	PostingList *iterator=*pl_ptr;
	while(iterator!=NULL){
		printf("[%d,%d] ",iterator->article_id,iterator->word_frequency );
		iterator=iterator->next;
	}
	printf("\n");
}

void postingList_Destroy(PostingList**pl_ptr){
	PostingList *iterator=*pl_ptr;
	while(iterator->next!=NULL){
		PostingList *tmp=iterator->next;
		free(iterator);
		iterator=tmp;
	}
	free(iterator);
}
