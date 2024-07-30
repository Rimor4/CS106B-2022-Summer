#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;
const int NONE = -1; // used as sentinel index

/*
 * The constructor initializes all of the member variables needed for
 * an instance of the PQHeap class. The allocated capacity
 * is initialized to a starting constant and a dynamic array of that
 * size is allocated. The number of filled slots is initially zero.
 */
PQHeap::PQHeap() {
    _numAllocated = INITIAL_CAPACITY;
    _numFilled = 0;
    _elements = new DataPoint[_numAllocated]();
}

/* The destructor is responsible for cleaning up any resources
 * used by this instance of the PQHeap class. The array
 * memory that was allocated for the PQArray is deleted here.
 */
PQHeap::~PQHeap() {
    delete[] _elements;
}

/*
 * Adds a new element into the heap while maintaining the property of the minHeap.
 */
void PQHeap::enqueue(DataPoint elem) {
    // Check whether the queue needs to be extended
    if (_numAllocated < _numFilled + 1) {
        // By default, it expands to twice the original size
        extend(2 * _numAllocated);
    }

    // Insert the new element at the end and move it up to its correct position.
    _elements[_numFilled] = elem;
    bubbleUp(_numFilled);
    _numFilled++;
}

/*
 * Returns, but does not remove, the element that is frontmost.
 */
DataPoint PQHeap::peek() const {
    if (isEmpty()) error("PQHeap is empty!");
    return _elements[0];
}

/*
 * Removes and returns the element that is frontmost in this priority queue.
 */
DataPoint PQHeap::dequeue() {
    DataPoint front = peek();

    // Move the last element to the first and move it down to its correct position.
    _elements[0] = _elements[size() - 1];
    bubbleDown(0);
    _numFilled--;
    return front;
}

/*
 * Returns true if no elements in the queue, false otherwise
 */
bool PQHeap::isEmpty() const {
    return size() == 0;
}

/*
 * The count of enqueued elements is tracked in the
 * member variable _numFilled.
 */
int PQHeap::size() const {
    return _numFilled;
}

/*
 * Updates internal state to reflect that the queue is empty.
 */
void PQHeap::clear() {
    _numFilled = 0;
}

// Moves the element at the given index up to its correct position.
void PQHeap::bubbleUp(int index) {
    int parent;
    while ((parent = getParentIndex(index)) != NONE) {
        if (_elements[parent].priority <= _elements[index].priority) break;
        swap(parent, index);
        index = parent;
    }
}

// Moves the element at the given index down to its correct position.
void PQHeap::bubbleDown(int index) {
    int left, right, child;
    while (true) {
        left = getLeftChildIndex(index);
        right = getRightChildIndex(index);
        if (left == NONE) break;
        child = (right != NONE && _elements[right].priority < _elements[left].priority) ? right : left;
        if (_elements[child].priority >= _elements[index].priority) break;
        swap(index, child);
        index = child;
    }
}

/*
 * Private member function. This helper exchanges the element at
 * indexA with the element at indexB.
 */
void PQHeap::swap(int indexA, int indexB) {
    DataPoint tmp = _elements[indexA];
    _elements[indexA] = _elements[indexB];
    _elements[indexB] = tmp;
}

/*
 * This private function create a new, larger array and copies the original
 * elements into the new one, to extend the original array.
 */
void PQHeap::extend(size_t size) {
    if (size <= _numAllocated) {
        error("The extension size must be larger than the original size");
    }
    size_t old_size = PQHeap::size();
    DataPoint* n_elements = new DataPoint[size]();

    for (int i = 0; i < old_size; i++) {
        n_elements[i] = _elements[i];
    }

    delete[] _elements;
    _elements = n_elements;
    _numAllocated = size;
}

/*
 * Prints the contents of internal array for debugging purposes.
 */
void PQHeap::printDebugInfo(string msg) const {
    cout << msg << endl;
    for (int i = 0; i < size(); i++) {
        cout << "[" << i << "] = " << _elements[i] << endl;
    }
}

/*
 * We strongly recommend implementing this helper function, which
 * should traverse the heap array and ensure that the heap property
 * holds for all elements in the array. If elements are found that
 * violate the heap property, an error should be thrown.
 */
void PQHeap::validateInternalState() const {
    /*
     * If there are more elements than spots in the array, we have a problem.
     */
    if (_numFilled > _numAllocated) error("Too many elements in not enough sapce!");

    /*
     * Ensure that the heap property is maintained: the priority value of each parent
     * node isn't more than its two child node.
     */
    for (int i = 0; i < _numFilled; i++) {
        int left = getLeftChildIndex(i);
        int right = getRightChildIndex(i);
        if (left != NONE && _elements[left].priority < _elements[i].priority) {
            printDebugInfo("validateInternalState");
            error("Heap property violated at index " + integerToString(i));
        }
        if (right != NONE && _elements[right].priority < _elements[i].priority) {
            printDebugInfo("validateInternalState");
            error("Heap property violated at index " + integerToString(i));
        }
    }
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the parent of the
 * specified child index. If this child has no parent, return
 * the sentinel value NONE.
 */
int PQHeap::getParentIndex(int child) const {
    if (child == 0) return NONE;
    return (child - 1) / 2;
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the left child of the
 * specified parent index. If this parent has no left child, return
 * the sentinel value NONE.
 */
int PQHeap::getLeftChildIndex(int parent) const {
    int left = 2 * parent + 1;
    return left < size() ? left : NONE;
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the right child of the
 * specified parent index. If this parent has no right child, return
 * the sentinel value NONE.
 */
int PQHeap::getRightChildIndex(int parent) const {
    int right = 2 * parent + 2;
    return right < size() ? right : NONE;
}

/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Add your own custom tests here! */
STUDENT_TEST("Test for enqueue and dequeue operations") {
    PQHeap pq;
    DataPoint dp1 = { "task1", 3 };
    DataPoint dp2 = { "task2", 1 };
    DataPoint dp3 = { "task3", 2 };

    pq.enqueue(dp1);
    pq.validateInternalState();
    pq.enqueue(dp2);
    pq.validateInternalState();
    pq.enqueue(dp3);
    pq.validateInternalState();

    EXPECT_EQUAL(pq.dequeue().label, "task2");
    pq.validateInternalState();
    EXPECT_EQUAL(pq.dequeue().label, "task3");
    pq.validateInternalState();
    EXPECT_EQUAL(pq.dequeue().label, "task1");
    pq.validateInternalState();
    EXPECT(pq.isEmpty());
}

STUDENT_TEST("Test for isEmpty and size operations") {
    PQHeap pq;
    EXPECT(pq.isEmpty());
    EXPECT_EQUAL(pq.size(), 0);

    pq.enqueue({ "task1", 1 });
    EXPECT(!pq.isEmpty());
    EXPECT_EQUAL(pq.size(), 1);

    pq.enqueue({ "task2", 2 });
    EXPECT_EQUAL(pq.size(), 2);

    pq.dequeue();
    EXPECT_EQUAL(pq.size(), 1);

    pq.dequeue();
    EXPECT(pq.isEmpty());
    EXPECT_EQUAL(pq.size(), 0);
}

STUDENT_TEST("Test for clear operation") {
    PQHeap pq;
    pq.enqueue({ "task1", 1 });
    pq.enqueue({ "task2", 2 });
    pq.enqueue({ "task3", 3 });

    pq.clear();
    EXPECT(pq.isEmpty());
    EXPECT_EQUAL(pq.size(), 0);
}

STUDENT_TEST("Test for peek operation") {
    PQHeap pq;
    pq.enqueue({ "task1", 3 });
    pq.enqueue({ "task2", 1 });
    pq.enqueue({ "task3", 2 });

    EXPECT_EQUAL(pq.peek().label, "task2");
    pq.dequeue();
    EXPECT_EQUAL(pq.peek().label, "task3");
}

STUDENT_TEST("Test for resizing") {
    PQHeap pq;
    for (int i = 0; i < 100; i++) {
        pq.enqueue({ "task" + integerToString(i), static_cast<double>(randomInteger(1, 100)) });
    }
    EXPECT_EQUAL(pq.size(), 100);
}

STUDENT_TEST("Test with duplicate priorities") {
    PQHeap pq;
    pq.enqueue({ "task1", 1 });
    pq.enqueue({ "task2", 1 });
    pq.enqueue({ "task3", 1 });

    EXPECT_EQUAL(pq.dequeue().label, "task1");
    EXPECT_EQUAL(pq.dequeue().label, "task3");
    EXPECT_EQUAL(pq.dequeue().label, "task2");
    EXPECT(pq.isEmpty());
}

STUDENT_TEST("Stress test with a large number of elements") {
    PQHeap pq;
    const int numElements = 10000;
    Vector<DataPoint> elements;

    for (int i = 0; i < numElements; i++) {
        elements.add({ "task" + integerToString(i), static_cast<double>(randomInteger(1, 100000)) });
    }

    for (DataPoint dp : elements) {
        pq.enqueue(dp);
    }

    EXPECT_EQUAL(pq.size(), numElements);

    int previousPriority = pq.dequeue().priority;
    for (int i = 1; i < numElements; i++) {
        int currentPriority = pq.dequeue().priority;
        EXPECT(previousPriority <= currentPriority);
        previousPriority = currentPriority;
    }

    EXPECT(pq.isEmpty());
}


/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQHeap example from writeup, validate each step") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();
    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }
    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
}
