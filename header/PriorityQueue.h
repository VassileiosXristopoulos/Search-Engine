#ifndef PRIORITYQUEUE
#define PRIORITYQUEUE
#include <sys/ioctl.h>
#include <unistd.h>
#include <math.h>
#include "../header/myFunctions.h"
typedef struct PriorityQueue PriorityQueue;
struct PriorityQueue{
	int id;
	double score;
	PriorityQueue * next;
};
void Queue_Print(PriorityQueue*,PriorityQueue**,PriorityQueue**,char**,int,char**,int);
void Queue_Init(PriorityQueue*,PriorityQueue**,PriorityQueue**,int,double);
void Queue_Destroy(PriorityQueue *,PriorityQueue**);
void Queue_Insert(PriorityQueue*,PriorityQueue**,PriorityQueue**,int,double,char**);
int InQueue(PriorityQueue* ,PriorityQueue* *,PriorityQueue* *,int );
#endif