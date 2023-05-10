/*
// Created Thomas on 07/03/23.
*/

#include <stdio.h>
#include <stdlib.h>

#include "EdgeList.h"
#include "Edge.h"
#include "Link.h"


/**
 * initialize an empty list of Edge and set the current pointer on sentinelFirst
 * @param el the list to be initialized
 */
void initEmptyEdgeList(EdgeList *el){
    if (el != NULL){
        el->sentinelFirst.link = NULL;
        el->sentinelFirst.next = &el->sentinelLast;
        el->sentinelFirst.previous = NULL;

        el->current = &el->sentinelFirst;

        el->sentinelLast.link = NULL;
        el->sentinelLast.next = NULL;
        el->sentinelLast.previous = &el->sentinelFirst;
    } else {
        fprintf(stderr, "error : EdgeList bad allocation\n");
    }
}

/**
 * return a boolean about the emptyness of a list of Links
 * @param el the list of Links
 * @return 1 if the list is empty and 0 if the list isn't empty
 */
int isEdgeListEmpty(EdgeList *el){
    if (el != NULL){
        return el->sentinelFirst.next == &el->sentinelLast;
    } else {
        fprintf(stderr, "error : EdgeList bad allocation\n");
        return -1;
    }
}

/**
 * return a boolean about the position of the current pointer in a list of Links
 * @param el the list of Links
 * @return 1 if the current pointer is on the first element of the list and 0 if it isn't
 */
int isFirstEdge(EdgeList *el){
    if (el != NULL){
        return el->current == el->sentinelFirst.next;
    } else {
        fprintf(stderr, "error : EdgeList bad allocation\n");
        return -1;
    }
}

/**
 * return a boolean about the position of the current pointer in a list of Links
 * @param el the list of Links
 * @return 1 if the current pointer is on the last element of the list and 0 if it isn't
 */
int isLastEdge(EdgeList *el){
    if (el != NULL){
        return el->current == el->sentinelLast.previous;
    } else {
        fprintf(stderr, "error : EdgeList bad allocation\n");
        return -1;
    }
}

/**
 * return a boolean about the position of the current pointer in a list of Links
 * @param el the list of Links
 * @return 1 if the current pointer is on one of both sentinels of the list and 0 if it isn't
 */
int isOutOfListEdge(EdgeList *el){
    if (el != NULL){
        return el->current == &el->sentinelLast || el->current == &el->sentinelFirst;
    } else {
        fprintf(stderr, "error : EdgeList bad allocation\n");
        return -1;
    }
}

/**
 * set the current pointer of a list of Links on the first element of the list
 * @param el the list of Links
 */
void setOnFirstEdge(EdgeList *el){
    if (el != NULL){
        el->current = el->sentinelFirst.next;
    } else {
        fprintf(stderr, "error : EdgeList bad allocation\n");
    }
}

/**
 * set the current pointer of a list of Links on the last element of the list
 * @param el the list of Links
 */
void setOnLastEdge(EdgeList *el){
    if (el != NULL){
        el->current = el->sentinelLast.previous;
    } else {
        fprintf(stderr, "error : EdgeList bad allocation\n");
    }
}

/**
 * set the current pointer of a list of Links on the next element of the list
 * @param el the list of Links
 */
void setOnNextEdge(EdgeList *el){
    if (el != NULL && el->current != NULL){
        el->current = el->current->next;
    } else {
        fprintf(stderr, "error : EdgeList bad allocation or current pointer NULL\n");
    }
}

/**
 * set the current pointer of a list of Links on the previous element of the list
 * @param el the list of Links
 */
void setOnPreviousEdge(EdgeList *el){
    if (el != NULL && el->current != NULL){
        el->current = el->current->previous;
    } else {
        fprintf(stderr, "error : EdgeList bad allocation or current pointer NULL\n");
    }
}

/**
 * print the content of a list of Links
 * @param el the list of Links
 */
 /*
void printEdgeList(EdgeList *el){
    Link* l;
    if (el != NULL){
        printf("Edge list :\n\n");
        setOnFirstEdge(el);
        while(!isOutOfListEdge(el)){
            l = getEdgeLink(el->current);
            printLink(l);
            setOnNextEdge(el);
        }
        printf("\n");
    } else {
        fprintf(stderr, "error : EdgeList bad allocation\n");
    }
}
*/
/**
 * return the number of element of a list of Links
 * @param el the list of Links
 * @return the number of element of a list of Links
 */
int countEdgeElements(EdgeList *el){
    int res = 0;
    if (el != NULL){
        setOnFirstEdge(el);
        while(!isOutOfListEdge(el)){
            res ++;
            setOnNextEdge(el);
        }
        return res;
    } else {
        fprintf(stderr, "error : EdgeList bad allocation\n");
        return -1;
    }
}

/**
 * create a new Edge pointing a given Link and insert it at the end of a list of Links
 * @param el the list of Links
 * @param l the Link to add in the list
 * @return 0 if it is a success, -1 if it isn't
 */
int insertEdgeLast(EdgeList *el, Link* l){
    Edge* e = NULL;
    if (el != NULL){
        e = allocEdge();
        initEdge(e,l,NULL,NULL);
        if(isEdgeListEmpty(el)){
            el->sentinelFirst.next = e;
            e->previous = &el->sentinelFirst;
        } else {
            setOnLastEdge(el);
            el->current->next = e;
            e->previous = el->current;
        }
        el->sentinelLast.previous = e;
        e->next = &el->sentinelLast;
        return 0;
    } else {
        fprintf(stderr, "error : EdgeList bad allocation\n");
        return -1;
    }
}

/**
 * seek a given Link in a list of Links
 * @param el the list of Links
 * @param l the Link to seek
 * @return 1 if the Link is in the list, 0 if not
 */
int findLink(EdgeList *el, Link* l){
    if (el != NULL){
        setOnFirstEdge(el);
        while(!isOutOfListEdge(el)){
            if(el->current != NULL && el->current->link == l){
                return 1;
            }
            setOnNextEdge(el);
        }
        return 0;
    } else {
        fprintf(stderr, "error : EdgeList bad allocation\n");
        return -1;
    }
}

/**
 * delete a Edge pointing a given Link from a list of Links if it exists and free the Edge deleted memory
 * @param el the list of Links
 * @param l the Link to be deleted
 * @return 0 if it's a success, -1 if not
 */
int deleteEdge(EdgeList* el, Link* l){
    if (el != NULL){
        if(findLink(el,l) == 1){
            el->current->previous->next = el->current->next;
            el->current->next->previous = el->current->previous;
            freeEdge(el->current);
            setOnPreviousEdge(el);
            return 0;
        }
        return -1;
    } else {
        fprintf(stderr, "error : EdgeList bad allocation\n");
        return -1;
    }
}
