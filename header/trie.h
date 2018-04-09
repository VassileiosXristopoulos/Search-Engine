#ifndef TRIE_H
#define TRIE_H
#include <stdio.h>
#include <stdlib.h>
#include "../header/postingList.h"
#include "../header/Char_Stack.h"
typedef struct Trie_node{
	int isleaf;
	struct Trie_node *sibling;
	struct Trie_node *child;
	char  value;
	int df;
	PostingList *pl_ptr;
	char *word;
}Trie_node;

void Trie_node_Init(Trie_node*);
void Trie_Insert(char*,int,Trie_node*);
Trie_node * Trie_Search(char*,Trie_node*);
void Trie_Destroy(Trie_node*);
void Trie_PrintAll(Trie_node* ,Char_Stack_Node**,Char_Stack_Node**);
#endif