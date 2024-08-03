#include "listnode.h"
#include "vector.h"
#include "testing/SimpleTest.h"
#include "sorting.h"
using namespace std;

// Based on linked lists, this function divides the input list into sublists that are less than,
// equal to, or greater than a chosen pivot. After recursively sorting the sublists, they are
// joined in sequence.
void quickSort(ListNode*& front) {
    if (front != nullptr && front->next != nullptr) {
        ListNode *lesser = nullptr, *equal = nullptr, *greater = nullptr;

        // Divide into three sublists, less than or equal to, and greater than a pivot
        partition(front, lesser, equal, greater);

        quickSort(lesser);
        quickSort(greater);

        // Join three lists into one
        front = concatenate(lesser, equal, greater);
    }
}

/*
 * Choose the first element of the list as the pivot. Divide the elements into three sublists:
 * those elements that are strictly less than the pivot, those that are equal to the pivot,
 * and those that are strictly greater.
 */
void partition(ListNode*& front, ListNode*& lesser, ListNode*& equal, ListNode*& greater) {
    ListNode* cur = front->next;
    int pivot = front->data;
    equal = front;
    equal->next = nullptr;	// Disconnect this list from other nodes

    while (cur != nullptr) {
        ListNode* next = cur->next;	// Save the next node
        if (cur->data < pivot) {
            cur->next = lesser;
            lesser = cur;
        } else if (cur->data > pivot) {
            cur->next = greater;
            greater = cur;
        } else {
            cur->next = equal;
            equal = cur;
        }
        cur = next;
    }
}

/*
 * This helper function is given three lists to join into one combined by adding the links in between.
 */
ListNode* concatenate(ListNode*& lesser, ListNode*& equal, ListNode*& greater) {
    ListNode *front = nullptr, *tail = nullptr;

    // Helper lambda to append a list to the concatenated result
    auto appendList = [&](ListNode*& list) {
        if (list != nullptr) {
            // the previous sublists are empty
            if (front == nullptr) {
                front = tail = list;
            } else {
                tail->next = list;
            }
            while (tail->next != nullptr) {
                tail = tail->next;
            }
        }
    };

    appendList(lesser);
    appendList(equal);
    appendList(greater);

    return front;
}


/* * * * * * Test Code Below This Point * * * * * */

/* TODO: Write your own student tests below to test your sorting helper
 * functions and overall sorting algorithm correctness. We have provided
 * the skeleton of some tests below to help you get started with this
 * process.
 */

/*
 * We have provided this utility function as a convenience. This
 * function traverses the linked list represented by the provided
 * front pointer and prints out the data stored in the list along
 * the way in a convenient format. You may find it to be helpful to
 * use this function as a debugging aid.
 */
void printList(ListNode* front) {
   cout << "{";
   for (ListNode *cur = front; cur != nullptr; cur = cur->next) {
       cout << cur->data;
       if (cur->next != nullptr){
           cout << ", ";
       }
   }
   cout << "}" << endl;
}

/*
 * This utility function deallocates the memory for all the nodes in a
 * given linked list. It can be used to recycle the memory allocated
 * during a test case and avoid memory leaks.
 */
void deallocateList(ListNode* front) {
    while (front != nullptr) {
        ListNode* temp = front;
        front = front->next;
        delete temp;
    }
}

/*
 * This utility function is given a vector of values and constructs
 * an equivalent linked list containing the same values in the same order.
 * It can be used to construct a linked list to use as an input to
 * a function being tested.
 */
ListNode* createList(Vector<int> values){
    ListNode* list = nullptr;
    for (int i = values.size() - 1; i >= 0; i--) {
        list = new ListNode(values[i], list);
    }
    return list;
}

/*
 * This utility function compares a linked list and a vector for
 * equivalence and returns true if both contain the same values in the
 * same order. It can be used to confirm a linked list's contents match
 * the expected.
 */
bool areEquivalent(ListNode* front, Vector<int> v){
    int i = 0;
    while (front != nullptr && i < v.size()) {
        if (front->data != v[i]) {
            return false;
        }
        front = front->next;
        i++;
    }

    // Both should be null/complete if they are equivalent
    return front == nullptr && i == v.size();
}

STUDENT_TEST("This is a skeleton to demonstrate an end-to-end student test."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {1, 2, 3, 4};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("Test sorting an empty list") {
    ListNode* list = nullptr;
    quickSort(list);
    EXPECT(list == nullptr);
}

STUDENT_TEST("Test sorting a single-element list") {
    Vector<int> values = {42};
    ListNode* list = createList(values);
    quickSort(list);
    EXPECT(areEquivalent(list, values));
    deallocateList(list);
}

STUDENT_TEST("Test sorting a two-element list") {
    Vector<int> values = {2, 1};
    ListNode* list = createList(values);
    quickSort(list);
    values.sort();
    EXPECT(areEquivalent(list, values));
    deallocateList(list);
}

STUDENT_TEST("Test sorting a reverse-ordered list") {
    Vector<int> values = {5, 4, 3, 2, 1};
    ListNode* list = createList(values);
    quickSort(list);
    values.sort();
    EXPECT(areEquivalent(list, values));
    deallocateList(list);
}

STUDENT_TEST("Test sorting an already sorted list") {
    Vector<int> values = {1, 2, 3, 4, 5};
    ListNode* list = createList(values);
    quickSort(list);
    values.sort();
    EXPECT(areEquivalent(list, values));
    deallocateList(list);
}

STUDENT_TEST("Test sorting a list with duplicate elements") {
    Vector<int> values = {3, 1, 2, 3, 1, 2};
    ListNode* list = createList(values);
    quickSort(list);
    values.sort();
    EXPECT(areEquivalent(list, values));
    deallocateList(list);
}

STUDENT_TEST("Test sorting a large list") {
    Vector<int> values;
    for (int i = 1000; i >= 1; --i) {
        values.add(i);
    }
    ListNode* list = createList(values);
    quickSort(list);
    values.sort();
    EXPECT(areEquivalent(list, values));
    deallocateList(list);
}

STUDENT_TEST("Test sorting a randomly ordered list") {
    Vector<int> values = {7, 2, 5, 3, 8, 1, 4, 6};
    ListNode* list = createList(values);
    quickSort(list);
    values.sort();
    EXPECT(areEquivalent(list, values));
    deallocateList(list);
}

STUDENT_TEST("Test sorting a list with negative numbers and zeros") {
    Vector<int> values = {-3, 0, -1, 4, 2, -2, 1, 0};
    ListNode* list = createList(values);
    quickSort(list);
    values.sort();
    EXPECT(areEquivalent(list, values));
    deallocateList(list);
}

STUDENT_TEST("Test sorting a list with all identical elements") {
    Vector<int> values = {42, 42, 42, 42};
    ListNode* list = createList(values);
    quickSort(list);
    values.sort();
    EXPECT(areEquivalent(list, values));
    deallocateList(list);
}


PROVIDED_TEST("This tests some of the testing helper functions you are recommended to implement."){
    /* Creates a small test list containing the values 1->2->3. */
    ListNode* testList = nullptr;
    testList = new ListNode(3, testList);
    testList = new ListNode(2, testList);
    testList = new ListNode(1, testList);

    /* Test equality comparison function. */
    EXPECT(areEquivalent(testList, {1, 2, 3}));
    EXPECT(!areEquivalent(testList, {1, 2}));
    EXPECT(!areEquivalent(testList, {1, 2, 3, 4}));
    EXPECT(!areEquivalent(testList, {1, 3, 2}));

    /* Test create list function. */
    ListNode* studentList = createList({1, 2, 3, 4});
    printList(studentList);
    ListNode* cur = studentList;
    for (int i = 1; i <= 4; i++){
        EXPECT(cur != nullptr);
        EXPECT_EQUAL(cur->data, i);
        cur = cur->next;
    }

    /* Test deallocate list functions. There should be no memory leaks from this test. */
    deallocateList(studentList);
    deallocateList(testList);
}

/*
 * The provided test case demonstrates a comparsion between the same
 * algorithm (quicksort) operating on a linked list versus a vector.
 */
PROVIDED_TEST("Time linked list quicksort vs vector quicksort") {
    int startSize = 50000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, v.sort());        /* Standard vector sort operation is backed
                                               with quicksort algorithm. */

        deallocateList(list);
    }
}

