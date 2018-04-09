#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/Char_Stack.h"


Char_Stack_Node * Char_Stack_Node_Init(char value){
	Char_Stack_Node* myNode=malloc(sizeof(Char_Stack_Node));
	myNode->value=value;
	myNode->next=NULL;
	return myNode;
}


int Char_Stack_push(Char_Stack_Node **head,Char_Stack_Node **top,char letter){
	int _ret=0;
	if(isEmpty(head)){
		(*head)=Char_Stack_Node_Init(letter);
		(*top)=(*head);
		(*head)->previous=NULL;
		_ret=1;
	}
	else{
		Char_Stack_Node *temp=Char_Stack_Node_Init(letter);
		(*top)->next=temp;
		temp->previous=(*top);
		(*top)=temp;
		_ret=1;
	}
	return _ret;
}

int Char_Stack_pop(Char_Stack_Node **top,Char_Stack_Node **head){
	int _ret=0;
	if(!isEmpty(head)){
		Char_Stack_Node *temp=(*top);
		if((*top)==(*head)) (*head)=NULL;
		(*top)=temp->previous;
		free(temp);
		if((*top)!=NULL)(*top)->next=NULL;
	}
	return _ret;
}


int isEmpty(Char_Stack_Node**head){	return((*head)==NULL); }

void Char_Stack_Print(Char_Stack_Node **head){
	Char_Stack_Node* iterator=(*head);
	while(iterator!=NULL){
		printf("%c",iterator->value );
		iterator=iterator->next;
	}
}

