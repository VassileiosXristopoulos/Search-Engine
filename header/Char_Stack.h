#ifndef CHARSTACK_H
#define CHARSTACK_H
typedef struct Char_Stack_Node Char_Stack_Node;
struct Char_Stack_Node{
	char value;
	Char_Stack_Node *next;
	Char_Stack_Node *previous;
};
Char_Stack_Node* Char_Stack_Node_Init(char);
int Char_Stack_push(Char_Stack_Node**,Char_Stack_Node**,char);
int Char_Stack_pop(Char_Stack_Node**,Char_Stack_Node**);
void Char_Stack_Print(Char_Stack_Node **);
int isEmpty(Char_Stack_Node**);


#endif 