#ifndef MYFUNCTIONS_H
#define MYFUNCTIONS_H
#include "../header/trie.h"
#include "../header/Stack.h"
#include "../header/PriorityQueue.h"
void ThrowError(char*);
int getlines(char*);
void SaveWords(char**,char*);
double GetScore(int,double,char**,int,int,Trie_node*,int);
double Get_IDF(int,int);
void Search_Command(int*,int,Trie_node*,char**,int);
void df_command(Trie_node *,char *);
void tf_command(Trie_node*,char*,int);
void CountWordLength(char *,int*);
int CountWords(char *,int);
int isQueryWord(char **,char *,int );
void myPrint(int ,char **,char**,int ,int ,int );
int GetOutputLength(int,int ,double);
void breakLine(int);
#endif