#ifndef POSTINGLIST_H
#define POSTINGLIST_H
typedef struct PostingList PostingList;
struct PostingList{
	int article_id;
	int word_frequency;
	PostingList *next;
};
int postingList_Update(int,PostingList**);
void postingList_Print(PostingList**,char*);
void postingList_Destroy(PostingList**);
#endif