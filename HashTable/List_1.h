#pragma once

#include <stdio.h>



typedef char* Elem_t;

extern const Elem_t POISON_ELEM;

enum listErr {
    OK = 0, LIST_EMPTY = 1, NEGATIVE_SIZE = 2, EXTERNAL_INTRUSION = 3, FULL = 4
};


extern const char *errMsg[];

#pragma pack(1)
struct List_t {
    unsigned long long canaryBegin;
	char *name;
    size_t size;
	size_t maxSize;
	int free;
	int head;
	int tail;
    Elem_t *data;
    int *next;
    int *prev;
	listErr error;
    unsigned long long canaryEnd;
};
#pragma pack()

void tellMeEverythingYouKnow( char *name, List_t *this_, char *reason, const char *func, int line, char *file );

#define LIST_DUMP(list, reason) tellMeEverythingYouKnow( #list, list, reason, __func__, __LINE__, __FILE__ );

void clearLog();

void listInit( List_t *this_, const char *name = "Default name" );

void listDestruct( List_t *this_ );

int insertAfter( List_t *this_, int index, Elem_t val );

int insertBegin( List_t *this_, Elem_t val );

int insertBefore( List_t *this_, int index, Elem_t val );

void delElem( List_t *this_, int index );

Elem_t getValueByLogicIndexVerySlowlyWorkingDontUseMe( List_t *this_, int logicalAddress );

int getLogicalIndexVerySlowlyWorkingDontUseMe( List_t *this_, Elem_t val, int *out = nullptr, size_t maxSize = 1 );

listErr listOK( List_t *this_ );

void sortList( List_t *this_ );

void printSortedList( List_t *this_ );

void printListSlowly( List_t *this_ );
