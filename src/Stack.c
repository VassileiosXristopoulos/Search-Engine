#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/Stack.h"

Stack* Stack_CreateStack(int capacity){
	Stack *stack=malloc(sizeof(Stack));
	stack->capacity=capacity;
	stack->top=-1;
	stack->items=malloc(capacity*sizeof(char*));
	for(int i=0;i<capacity;i++) stack->items[i]=NULL;
	return stack;
}

int Stack_isFull(Stack * stack){
	return stack->top==stack->capacity-1;
}

int Stack_isEmpty(Stack*stack){
	return stack->top==-1;
}

void Stack_push(Stack * stack,char *word){
	if(Stack_isFull(stack)) return;
	if(inStack(word,stack)) return;
	stack->top++;
	stack->items[stack->top]=malloc((strlen(word)+1)*sizeof(char));
	strcpy(stack->items[stack->top],word);
	return;
}

int Stack_pop(Stack *stack,char **mychar){
	if(Stack_isEmpty(stack)) return 0;
	char *tmp=stack->items[stack->top];
	*mychar=malloc((strlen(tmp)+1)*sizeof(char));
	strcpy(*mychar,tmp);
	free(stack->items[stack->top]);
	stack->top--;
	return 1;
}
char ** Stack_Peek(Stack *stack){ //see top element of stack, without moving it
	if(stack->top==-1) return NULL;
	return &(stack->items[stack->top]);
}
void Stack_Destroy(Stack *stack){
	while(stack->top >=0){
		free(stack->items[stack->top]);
		stack->top--;
	}
	free(stack->items);
	free(stack);
}
int Stack_GetSize(Stack *stack){
	return stack->top;
}
int inStack(char *word,Stack *stack){
	int counter=stack->top;
	while(counter>-1){
		if(strcmp(stack->items[counter],word)==0) return 1;
		counter--;
	}
	return 0;
}