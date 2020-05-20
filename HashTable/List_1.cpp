#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <typeinfo>

#include "List_1.h"

#ifdef DEBUG
	#define ON_DEBUG(cond) cond;
#else
	#define ON_DEBUG(cond)
#endif

const char* errMsg[]{
	"OK", "LIST_EMPTY", "NEGATIVE_SIZE", "EXTERNAL_INTRUSION", "FULL"
};

const Elem_t POISON_ELEM = nullptr;
const unsigned long long canary = 123456789;

const char separation[] = "\n------------------------------------------------------------------------------------\n";
const char logName[] = "listLog.log";

void clearLog(){
    FILE *out = fopen ( logName, "w" );
    fclose( out );
}

void msgToLog( const char *msg ){
	FILE *out = fopen ( logName, "at" );
	fprintf( out, "%s", msg );
	fprintf( out, "%s", separation );
	fclose( out );
}

void listInit( List_t *this_, char *name ){

	ON_DEBUG( assert( this_ ) );
	if( this_ == nullptr ) {
		msgToLog( "this_ in listInit is a nullptr!" );
		return;
	}
	ON_DEBUG( assert( name ) );
	if( name == nullptr ) {
		msgToLog( "Name in listInit is a nullptr!" );
		return;
	}

	this_-> name = strdup( name );

	this_-> maxSize = 12;
	this_-> data = ( Elem_t* ) calloc ( this_-> maxSize, sizeof( Elem_t ) );
	this_-> next = ( int* ) calloc ( this_-> maxSize, sizeof( int ) );
	this_-> prev = ( int* ) calloc ( this_-> maxSize, sizeof( int ) );

	this_-> size = 0;
	this_-> free = 0;
	this_-> head = -1;
	this_-> tail = -1;

	this_-> canaryBegin = canary;
	this_-> canaryEnd = canary;
	this_-> error = OK;

	for( int i = 0; i < this_-> maxSize; i++ ){
		this_-> data[i] = POISON_ELEM;
		this_-> next[i] = i + 1;
		this_-> prev[i] = i - 1;
	}
	this_-> next[this_-> maxSize - 1] = -1;
}

void listDestruct( List_t *this_ ){
	ON_DEBUG( assert( this_ ) );
	if( this_ == nullptr ) {
		msgToLog( "this_ in listInit is a nullptr!" );
		return;
	}

	free( this_-> name );
	free( this_-> next );
	free( this_-> data );
	free( this_-> prev );
}

listErr listOK( List_t *this_ ){
	assert( this_ );
	if( (this_-> canaryEnd != 123456789) || (this_-> canaryBegin != 123456789) ){
		this_-> error = EXTERNAL_INTRUSION;
		LIST_DUMP( this_, "EXTERNAL_INTRUSION" );
		msgToLog( "EXTERNAL_INTRUSION" );
		return EXTERNAL_INTRUSION;
	}
	if( this_-> size < 0 ){
		LIST_DUMP( this_, "NEGATIVE_SIZE" );
		msgToLog( "NEGATIVE_SIZE" );
		this_-> error = NEGATIVE_SIZE;
		return NEGATIVE_SIZE;
	}
	if( this_-> free > this_-> maxSize ){
		LIST_DUMP( this_, "FULL" );
		msgToLog( "FULL" );
		this_-> error = FULL;
		return FULL;
	}
	return OK;
}

bool canAddElem( List_t *this_ ){
	if( this_-> size >= this_-> maxSize ) return false;
	return true;
}

void listExpand( List_t *this_ ){
	return;
}

int insertBegin( List_t *this_, Elem_t val ){
	assert( listOK( this_ ) == OK );

	if( val == POISON_ELEM ){
		msgToLog( "User pushes a POISON value. Dont do that please (>=<)");
		return -1;
	}

	int newIndex = this_-> free;
	this_-> free = this_-> next[ this_-> free ];

	this_-> data[ newIndex ] = val;
	this_-> prev[ newIndex ] = -1;

	this_-> next[ newIndex ] = this_-> head;
	this_-> head = newIndex;

	if( this_-> size == 0 ) {
		this_-> tail = newIndex;
	} else {
		this_-> prev[ this_-> next[newIndex] ] = newIndex;
	}

	this_-> size++;
	assert( listOK( this_ ) == OK );

	return 0;
}

int insertAfter( List_t *this_, int index, Elem_t val ){
	assert( listOK( this_ ) == OK );

	if( this_-> size == 0 ){
		msgToLog( "Failed to insert an element after one which doesn't exist in empty list! Use insertBegin instead!" );
		return -1;
	}
	if( index < 0 ){
		msgToLog( "User tried to insert an element after a negative index. Unsuccessfully." );
		return -1;
	}
	if( index > ( this_-> size - 1 ) ){
		msgToLog( "User tried to insert an element after index which is greater than list size!" );
		return -1;
	}
	if( val == POISON_ELEM ){
		msgToLog( "User pushes a POISON value. Dont do that please (>=<)");
		return -1;
	}


	if( this_-> data[ index ] == POISON_ELEM ){
		msgToLog( "User tried to put a new element after nonexistent element");
		return -1;
	} else {

		if( !canAddElem( this_ ) ) {
			listExpand( this_ );
			assert( listOK( this_ ) == OK );
		}

		int newIndex = this_-> free;
		this_-> data[ newIndex ] = val;

		this_-> free = this_-> next[ this_-> free ];
		this_-> size++;

		if( index != this_-> tail ){
			this_-> prev[ this_-> next[ index ] ] = newIndex;
		} else {
			this_-> tail = newIndex;
		}
		this_-> next[ newIndex ] = this_-> next[ index ];
		this_-> next[ index ] = newIndex;
		this_-> prev[ newIndex ] = index;

		assert( listOK( this_ ) == OK );

		return newIndex;
	}
}

int insertBefore( List_t *this_, int index, Elem_t val ){
	assert( listOK( this_ ) == OK );
	if( this_-> size == 0 ){
		msgToLog( "Failed to add an element before one which doesn't exist in empty list! Use insertBegin instead!" );
		return -1;
	}
	if( index < 0 ){
		msgToLog( "User tried to insert an element before a negative index. Unsuccessfully.");
		return -1;
	}
	if( val == POISON_ELEM ){
		msgToLog( "User pushes a POISON value. Dont do that please (>=<)");
		return -1;
	}
	if( this_-> prev[ index ] == -1 ){
		return insertBegin( this_, val );
	}
	if( this_-> prev[ index ] != -1 ) return insertAfter( this_, this_-> prev[ index ], val );
	return 0;
}

void delElem( List_t *this_, int index ){
	assert( listOK( this_ ) == OK );
	if( index < 0 ){
		msgToLog( "User tried to delete an element with negative index. Unsuccessfully." );
		return;
	}
	if( this_-> size == 0 ) {
		msgToLog( "User tried to delete an element from empty list. Unsuccessfully." );
		return;
	}
	this_-> size--;

	if( this_-> data[index] == POISON_ELEM ) {
		msgToLog( "User tried to delete an element which doesn't exist. Unsuccessfully." );
		return;
	}
	this_-> data[ index ] = POISON_ELEM;

	if( this_-> size == 0 ){
		this_-> head = -1;
		this_-> tail = -1;
		return;
	}
	if( index != this_-> head ){
		this_-> next[ this_-> prev[index] ] = this_-> next[index];
	}
	if( index != this_-> tail ){
		this_-> prev[ this_-> next[index] ] = this_-> prev[index];
	}

	this_-> next[ index ] = this_-> free;
	this_-> prev[ index ] = -1;
	this_-> free = index;

}

void tellMeEverythingYouKnow( char *name, List_t *this_, char *reason, const char *func, int line, char *file ){
    assert( name );
    assert( this_ );
    assert( reason );
    assert( func );
    assert( file );

    FILE *out = fopen ( logName, "at" );
    fseek( out, 0, SEEK_END );

    fprintf( out, "LIST %s called '%s'. Name in function = %s\n", typeid (this_).name(), this_-> name, name );
    fprintf( out, "%s from function <%s> from file <%s>, line <%d>:\n", reason, func, file, line );

    int size = this_-> size;
    fprintf( out, "list -> size = <%d>\n", size );
    fprintf( out, "list -> maxSize = <%lu>\n", this_-> maxSize );
    fprintf( out, "list -> data = <%p>\n", this_-> data );
    fprintf( out, "list -> free = <%d>\n", this_-> free );
    fprintf( out, "list -> head = <%d>\n", this_-> head );
    fprintf( out, "list -> tail = <%d>\n", this_-> tail );

    for ( int i = 0; i < this_-> maxSize; i++ ){
        if( i < size ) fprintf( out, "\t*" ); else fprintf( out, "\t " );
    fprintf( out, "data[%d] = <%s>;", i, this_-> data[i] );
        fprintf( out, " prev[%d] = <%d>;", i, this_-> prev[i] );
        fprintf( out, " next[%d] = <%d>;", i, this_-> next[i] );
        if ( this_-> data[i] == POISON_ELEM ) fprintf ( out, " [POISON]\n" ); else fprintf ( out, "\n" );
    }

    fprintf( out, "err = %d: %s\n", this_-> error, errMsg[ this_ -> error ]);

    //unsigned long long realHash = countHash( this_, sizeof( Stack_t ), &(this_->hash), sizeof( this_-> hash ) );
    //fprintf( out, "HASH: %llu; SHOULD BE: %llu", this_->hash, realHash);
    //if( realHash == this_-> hash ) fprintf( out, " Hash correct\n"); else fprintf( out, " HASH INCORRECT!\n");
    fprintf( out, "%s", separation );

    fclose( out );
}

Elem_t getValueByLogicIndexVerySlowlyWorkingDontUseMe( List_t *this_, int logicalAddress ){
	assert( listOK( this_ ) == OK );

	if( logicalAddress <= 0 ){
		msgToLog( "Didn't find expected value by non-positive address! Address should be a positive number!");
		return POISON_ELEM;
	}

	if( logicalAddress > this_-> size ){ // address начинается с 1, а не с 0
		msgToLog( "Didn't find expected value by address which is greater that list size!");
		return POISON_ELEM;
	}
	int cur = this_-> head;
	for( int i = 1; i < logicalAddress; i++ ){
		cur = this_-> next[cur];
	}
	return this_-> data[cur];
}

int getLogicalIndexVerySlowlyWorkingDontUseMe( List_t *this_, Elem_t wanted, int *out, size_t maxQty ){
	assert( listOK( this_ ) == OK );

	int cur = this_-> head;
	int ans = 1;

	if( ( maxQty == 1 ) || ( out == nullptr ) ){
		while( cur != -1 ){
			if( this_-> data[cur] == wanted ) return ans;
			cur = this_-> next[cur];
			ans++;
		}
		return -1;
	} else {
		int i = 0;
		while( cur != -1 ){
			if( this_-> data[cur] == wanted ) out[i++] = ans;
			cur = this_-> next[cur];
			ans++;
		}
	}
	return 1;
}

void printSortedList( List_t *this_ ){
	printf( "-1 <--" );
	for( int i = 0; i < this_-> size; i++ ){
		if( i != 0 ) printf( ">" );
		printf( " %s ", this_-> data[i] );
		if( i != this_-> size - 1 ) printf( "<--" ); else printf( "--" );
	}
	printf( "> -1\n%s", separation );
}

void printListSlowly( List_t *this_ ){
	int cur = this_-> head;
	printf( "-1 <--" );
	while( cur != -1 ){
		if( cur != this_-> head ) printf(">");
		printf( " %s ", this_-> data[cur]);
		cur = this_-> next[ cur ];
		if( cur != -1 ) printf( "<--" ); else printf( "--" );
	}
	printf( "> -1\n%s", separation );
}


