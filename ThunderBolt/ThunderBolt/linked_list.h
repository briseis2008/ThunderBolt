#ifndef THUNDERBOLT_LINKED_LIST_H
#define THUNDERBOLT_LINKED_LIST_H

#include <stdlib.h>
#include <assert.h>

template <class T>
class ListNode {
public:
    T dat;
    ListNode<T> *next;
    ListNode<T> *prev;

public:
    ListNode() : next(NULL), prev(NULL) {};
    ListNode(T d) : next(NULL), prev(NULL), dat(d) {};
};

template <class T>
class DoublyLinkedList {
    ListNode<T> *head;
    ListNode<T> *tail;

public:
    DoublyLinkedList() : head(NULL), tail(NULL) {};
    ~DoublyLinkedList() {CleanUp();};

    ListNode<T>* getFront();
    ListNode<T>* getBack();

    ListNode<T>* InsertFront(const T &dat);
    ListNode<T>* InsertBack(const T &dat);

    /* remove current node, and return original next node */
    ListNode<T>* Remove(ListNode<T> *node);
    /* delete current node, which also free node->dat. Should use this instead
     * of Remove() if your node->dat points to dynamically allocated memory */
    ListNode<T>* Delete(ListNode<T> *node);
    void CleanUp();
};


/* macro for list traverse */
#define FOR_EACH(node, list)\
    for ((node) = (list).getFront(); (node) != NULL; (node) = (node)->next)


/* template functions definition */
template <class T>
ListNode<T>* DoublyLinkedList<T>::getFront() {
    return head;
}

template <class T>
ListNode<T>* DoublyLinkedList<T>::getBack() {
    return tail;
}

template <class T>
ListNode<T>* DoublyLinkedList<T>::InsertFront(const T &dat) {
    /* allocate new node */
    ListNode<T> *newNode = new ListNode<T>(dat);
    assert(newNode);

    if (head == NULL) {
        head = newNode;
        tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    
    return newNode;
}

template <class T>
ListNode<T>* DoublyLinkedList<T>::InsertBack(const T &dat) {
    /* allocate new node */
    ListNode<T> *newNode = new ListNode<T>(dat);
    assert(newNode);

    if (tail == NULL) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    
    return newNode;
}

template <class T>
ListNode<T>* DoublyLinkedList<T>::Remove(ListNode<T> *node) {
    /* you got be kidding me, right? :) */
    if (!node) return NULL;

    ListNode<T> *next = node->next;

    /* re-link previous and next */
    if (node->next) node->next->prev = node->prev;
    if (node->prev) node->prev->next = node->next;

    /* update head and tail */
    if (node == head) head = node->next;
    if (node == tail) tail = node->prev;

    delete node;
    return next;
}


/* Only use this version instead of Remove() if your node->dat contain
 * dynamically allocated memory */
template <class T>
ListNode<T>* DoublyLinkedList<T>::Delete(ListNode<T> *node) {
    delete node->dat;
    return Remove(node);
}


template <class T>
void DoublyLinkedList<T>::CleanUp() {
    ListNode<T> *tmp, *next;
    tmp = head;

    while (tmp) {
        next = tmp->next;
        delete tmp;
        tmp = next;
    }

    head = NULL;
    tail = NULL;
}


#endif
