#include "linked_list.h"
#include "debug.h"


#ifdef LINKED_LIST_DEBUG

class test {
public:
    int a;
    test(int a) {this->a = a;};
};

int main () {
    printf("start\n");
    DoublyLinkedList<int> list;
    list.InsertFront(2);
    list.InsertFront(4);
    list.InsertBack(5);
    list.InsertFront(8);
    
    ListNode<int> *node;
    FOR_EACH(node, list) {
        printf("%d\t", node->dat);
    }
    printf("\n");

    node = list.getFront();
    while(node) {
        if (node->dat == 4) {
            node = list.Delete(node);
        } else {
            node = node->next;
        }
    }
    FOR_EACH(node, list) {
        printf("%d\t", node->dat);
    }
    printf("\n");



    DoublyLinkedList<test> list2;
    list2.InsertFront(test(3));
    list2.InsertFront(test(5));
    list2.InsertFront(test(7));
    list2.InsertFront(test(9));

    ListNode<test> *node2;
    FOR_EACH(node2, list2) {
        printf("%d\t", node2->dat.a);
    }
    printf("\n");


    return 0;
}
#endif
