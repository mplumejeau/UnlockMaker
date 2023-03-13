/*
// Created Thomas on 07/03/23.
*/

#include <stdio.h>
#include <stdlib.h>
#include "EdgeList.h"


/**
 * Initialized an EdgeList.
 * @param el the address of the EdgeList to initialize
 */
void initEdgeList(EdgeList *el){

}

/**
 * Tests if it is an empty EdgeList.
 * @param el the address of the EdgeList to be tested
 * @return int 1 if empty, 0 otherwise
 */
int isEdgeListEmpty(EdgeList *el){


    return 1;
}

/**
 * Tests if the current Edge is the first one.
 * @param el the address of the EdgeList to be tested
 * @return int 1 if the current Edge is the first one, 0 otherwise
 */
int isFirstEdge(EdgeList *el){


    return 1;
}

/**
 * Tests if the current Edge is the last one.
 * @param el the address of the EdgeList to be tested
 * @return int 1 if the current Edge is the last one, 0 otherwise
 */
int isLastEdge(EdgeList *el){


    return 1;
}

/**
 * Tests if the current Edge not valid (outside the EdgeList).
 * @param el the address of the EdgeList to be tested
 * @return int 1 if the current Edge is not valid, 0 otherwise
 */
int isOutOfListEdge(EdgeList *el){


    return 1;
}

/**
 * Set the current Edge on the first one, if the list is empty, it will be positionned on the sentinel at the end.
 * @param el the address of the EdgeList to be modified
 */
void setOnFirstEdge(EdgeList *el){


}

/**
 * Set the current Edge on the last one, if the list is empty, it will be positionned on the sentinel at the beginning.
 * @param el the address of the EdgeList to be modified
 */
void setOnLastEdge(EdgeList *el){


}

/**
 * Set the current Edge on the next one.
 * @param el the address of the EdgeList to be modified
 */
void setOnNextEdge(EdgeList *el){


}

/**
 * Set the current Edge on the previous one.
 * @param el the address of the EdgeList to be modified
 */
void setOnPreviousEdge(EdgeList *el){


}

/**
 * Print the content of the EdgeList.
 * @param el the address of the EdgeList to be displayed
 */
void printEdgeList(EdgeList *el){


}

/**
 * Count the number of elements stored in the EdgeList.
 * @param el the address of the EdgeList to be counted
 * @return In int the number of element in the EdgeList
 */
int countEdgeElements(EdgeList *el){


    return 1;
}

/**
 * Create a new Edge containing an Link given as parameter and insert it at the end of the EdgeList
 * @param el the address of the EdgeList
 * @param l the address of Link
 * @return int 1 if the Edge is valid and correctly insert, 0 otherwise
 */
int insertEdgeLast(EdgeList *el, Link* l){


    return 1;
}

/**
 * Verify if the Link l is stored in the EdgeList el.
 * @param el the address of the EdgeList
 * @param l the address of Link to be found
 * @return int 1 if Link is found, 0 otherwise
 */
int findLink(EdgeList *el, Link* l){


    return 1;
}

/**
 * Delete the Edge containing the Link l in the EdgeList el. It frees the memory of the Edge.
 * @param el the address of the EdgeList
 * @param l the contained in the Edge to be deleted
 */
void deleteEdge(EdgeList *el, Link* l){


}