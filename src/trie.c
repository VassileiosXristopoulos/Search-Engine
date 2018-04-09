#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/trie.h"

void Trie_node_Init(Trie_node* node){
	node->isleaf=0;
	node->sibling=NULL;
	node->child=NULL;
	node->value=0;
	node->pl_ptr=NULL;
	node->df=-1;
}
void Trie_Insert(char * word,int doc,Trie_node * Trie_root){
	Trie_node * iterator = Trie_root;
	for(int i=0;i<strlen(word);i++){
		int finished=0;
		char letter=word[i];
		/*--------no child exists--------------*/
		if(iterator->child==NULL){ //insert letter as child!
			Trie_node *newNode;
			newNode=malloc(sizeof(Trie_node));
			Trie_node_Init(newNode);
			newNode->sibling=NULL;
			newNode->child=NULL;
			newNode->value=letter;
			iterator->child=newNode;
			iterator=iterator->child;
		}
		/*------------child exists and is same as letter-----------*/
		else if(iterator->child->value==letter){ //letter is found at child
				iterator=iterator->child;
		}
		/*------------child exists but not same as letter--------*/
		else{ //letter not found at child
			Trie_node *temp_ptr=iterator->child;
			if(temp_ptr->sibling!=NULL){
				int found=0;
				while(temp_ptr->sibling!=NULL){ //search list
					if(temp_ptr->sibling->value==letter){ //if found in list
						found=1;
						iterator=temp_ptr->sibling;
						break;
					} 
					else temp_ptr=temp_ptr->sibling; 
				}
				if(found==0){ /*insert as sibling*/	
					Trie_node *newNode;
					newNode=malloc(sizeof(Trie_node));
					Trie_node_Init(newNode);
					newNode->value=letter;
					newNode->sibling=NULL;
					newNode->child=NULL;
					temp_ptr->sibling=newNode;
					iterator=temp_ptr->sibling;
				}
			}
			else{ //insert letter as sibling!!
				Trie_node *newNode;
				newNode=malloc(sizeof(Trie_node));
				Trie_node_Init(newNode);
				newNode->sibling=NULL;
				newNode->child=NULL;
				newNode->value=letter;
				temp_ptr->sibling=newNode;	
				iterator=temp_ptr->sibling;
			}
		}
	}
	/*iterator points at the last letter of the inserted word*/
	if(iterator->isleaf==0){ //initialize the leaf-info at Trie
		iterator->isleaf=1; 
		iterator->word=malloc((strlen(word)+1)*sizeof(char));
		strcpy(iterator->word,word);
		if(iterator->df==-1) iterator->df=0;
	}
	//postingList_Update returns 1 if new doc is inserted
	iterator->df+=postingList_Update(doc,&(iterator->pl_ptr));
} 

Trie_node* Trie_Search(char *word,Trie_node * Trie_root){
	Trie_node * iterator = Trie_root;
	for(int i=0;i<strlen(word);i++){
		int found=0;
		char letter=word[i];
		if(iterator->child->value==letter){
			iterator=iterator->child;
			found=1;
		}
		else{
			Trie_node *temp_ptr=iterator->child;
			while(temp_ptr->sibling!=NULL){ //search list
					if(temp_ptr->sibling->value==letter){ //if found in list
						iterator=temp_ptr->sibling;
						found=1;
						break; 
					} 
					else temp_ptr=temp_ptr->sibling;
			}
		}
		if(found==0) return NULL; //if i never found a letter of the word
	}
	return iterator;
}


void Trie_Destroy(Trie_node *myNode){
	if(myNode->sibling!=NULL) Trie_Destroy(myNode->sibling);
	if(myNode->child!=NULL) Trie_Destroy(myNode->child);
	if(myNode->isleaf==1){
		postingList_Destroy(&(myNode->pl_ptr));
		free(myNode->word);
	}
	free(myNode);
	return;
}

void Trie_PrintAll(Trie_node* myNode,Char_Stack_Node**head,Char_Stack_Node**top){
	Char_Stack_push(head,top,myNode->value);
	int flag=0;
	if(myNode->child!=NULL){
		Trie_PrintAll(myNode->child,head,top);
	}
	if(myNode->isleaf==1){
		PostingList * list_iterator=myNode->pl_ptr;
		int count=0;
		while(list_iterator!=NULL){
			count+=list_iterator->word_frequency;
			list_iterator=list_iterator->next;
		}
		Char_Stack_Print(head);
		printf(" %d\n",count );
	}
	if(myNode->sibling!=NULL){
		flag=1;
		Char_Stack_pop(top,head);
		Trie_PrintAll(myNode->sibling,head,top);
	}
	
	if(flag==0) Char_Stack_pop(top,head);
	return;
}