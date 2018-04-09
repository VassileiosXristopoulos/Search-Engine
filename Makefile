all:	
	gcc ./src/main.c ./src/myFunctions.c ./src/trie.c ./src/postingList.c ./src/Stack.c ./src/PriorityQueue.c ./src/Char_Stack.c -g -o  minisearch -lm 
