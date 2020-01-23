#include <stddef.h>

typedef struct node {
    Node * pNext;
    Node * pPrev;
    int * pData;
} Node;

Node * createNode() {
    Node * pNode = (Node *) malloc(sizeof(Node));
    if (pNode != NULL) {
        pNode->pData = NULL;
        pNode->pNext = NULL;
        pNode->pPrev = NULL;
    }
    return pNode;
}

int deleteNode(Node * pNode) {
    if (pNode != NULL) {
        if (pNode->pPrev != NULL) {
            pNode->pPrev->pNext = pNode->pNext;
        }

        if (pNode->pNext != NULL) {
            pNode->pNext->pPrev = pNode->pPrev;
        }

        if (pNode->pData != NULL) {
            free(pNode->pData);
        }

        free(pNode);
        return 1;
    }
    return 0;
}

int * getData(Node * pNode) {
    if (pNode != NULL && pNode->pData != NULL) {
        return pNode->pData;
    }
    return NULL;
}

int setData(Node * pNode, int * pData) {
    if (pNode != NULL & pData != NULL) {
        pNode->pData = pData;
        return 1;
    }
    return 0;
}

/*********************LINKED LIST FUNCTIONS & STUFF***********************/
typedef struct linkedlist {
    Node * pHead;
    Node * pTail;
    int length;
} List;

List * createList() {
    List * pList = (List *) malloc(sizeof(List));
    if (pList != NULL) {
        pList->pHead = NULL;
        pList->pTail = NULL;
        pList->length = 0;
    }

    return pList;
}

int deleteList(List * pList) {
    if (pList != NULL){
        while (pList->pHead != NULL) {
            Node * pTemp = pList->pHead;
            pList->pHead = pList->pHead->pNext;
            deleteNode(pTemp);
        }

        free(pList);
        return 1;
    }
    return 0;
}

int push(List * pList, int * pData) {
    Node * pNode = createNode();
    if (pNode != NULL) {
        pNode->pData = pData;
        if (pList->pHead == NULL) {
            pList->pHead == pNode;
        }
        pNode->pPrev = pList->pTail;
        pList->pTail = pNode;
        return 1;
    }
    return 0;
}

int pop(List * pList) {
    if (pList != NULL) {
        Node * pNode = pList->pTail;
        if (pNode != NULL) {
            if (pList->pHead == pNode) {
                pList->pHead = NULL;
            }
            if (pNode->pPrev != NULL) {
                pList->pTail = pNode->pPrev;
            }
            deleteNode(pNode);
            return 1;
        }
    }
    return 0;
}