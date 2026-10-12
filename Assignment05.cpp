#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

// 1. Array-based stack. The top is stored at index size - 1.
template <typename T>
class ArrayStack {
    T* data;
    size_t capacity;
    size_t count;
    void grow() {
        T* next = new T[capacity * 2];
        for (size_t i = 0; i < count; ++i) next[i] = data[i];
        delete[] data;
        data = next;
        capacity *= 2;
    }
public:
    explicit ArrayStack(size_t initial = 4) : data(new T[initial]), capacity(initial), count(0) {}
    ~ArrayStack() { delete[] data; }
    void push(const T& value) { if (count == capacity) grow(); data[count++] = value; }
    T pop() { if (empty()) throw underflow_error("ArrayStack is empty"); return data[--count]; }
    const T& top() const { if (empty()) throw underflow_error("ArrayStack is empty"); return data[count - 1]; }
    bool empty() const { return count == 0; }
    size_t size() const { return count; }
};

// 2. Linked-list stack. The head is the top of the stack.
template <typename T>
class LinkedStack {
    struct Node { T value; Node* next; Node(const T& v, Node* n) : value(v), next(n) {} };
    Node* head = nullptr;
    size_t count = 0;
public:
    ~LinkedStack() { while (!empty()) pop(); }
    void push(const T& value) { head = new Node(value, head); ++count; }
    T pop() { if (empty()) throw underflow_error("LinkedStack is empty"); Node* old = head; T v = old->value; head = head->next; delete old; --count; return v; }
    const T& top() const { if (empty()) throw underflow_error("LinkedStack is empty"); return head->value; }
    bool empty() const { return head == nullptr; }
    size_t size() const { return count; }
};

// 3. Array-based queue. A circular buffer avoids shifting elements.
template <typename T>
class ArrayQueue {
    T* data;
    size_t capacity, frontIndex = 0, count = 0;
    void grow() {
        T* next = new T[capacity * 2];
        for (size_t i = 0; i < count; ++i) next[i] = data[(frontIndex + i) % capacity];
        delete[] data; data = next; capacity *= 2; frontIndex = 0;
    }
public:
    explicit ArrayQueue(size_t initial = 4) : data(new T[initial]), capacity(initial) {}
    ~ArrayQueue() { delete[] data; }
    void enqueue(const T& value) { if (count == capacity) grow(); data[(frontIndex + count) % capacity] = value; ++count; }
    T dequeue() { if (empty()) throw underflow_error("ArrayQueue is empty"); T v = data[frontIndex]; frontIndex = (frontIndex + 1) % capacity; --count; return v; }
    const T& front() const { if (empty()) throw underflow_error("ArrayQueue is empty"); return data[frontIndex]; }
    bool empty() const { return count == 0; }
    size_t size() const { return count; }
};

// 4. Linked-list queue. Tail insertion and head removal are constant time.
template <typename T>
class LinkedQueue {
    struct Node { T value; Node* next = nullptr; explicit Node(const T& v) : value(v) {} };
    Node* head = nullptr; Node* tail = nullptr; size_t count = 0;
public:
    ~LinkedQueue() { while (!empty()) dequeue(); }
    void enqueue(const T& value) { Node* n = new Node(value); if (tail) tail->next = n; else head = n; tail = n; ++count; }
    T dequeue() { if (empty()) throw underflow_error("LinkedQueue is empty"); Node* old = head; T v = old->value; head = head->next; if (!head) tail = nullptr; delete old; --count; return v; }
    const T& front() const { if (empty()) throw underflow_error("LinkedQueue is empty"); return head->value; }
    bool empty() const { return head == nullptr; }
    size_t size() const { return count; }
};

// 5. Array-based deque. A circular buffer supports both ends.
template <typename T>
class ArrayDeque {
    T* data; size_t capacity, frontIndex = 0, count = 0;
    void grow() {
        T* next = new T[capacity * 2];
        for (size_t i = 0; i < count; ++i) next[i] = data[(frontIndex + i) % capacity];
        delete[] data; data = next; capacity *= 2; frontIndex = 0;
    }
public:
    explicit ArrayDeque(size_t initial = 4) : data(new T[initial]), capacity(initial) {}
    ~ArrayDeque() { delete[] data; }
    void pushFront(const T& value) { if (count == capacity) grow(); frontIndex = (frontIndex + capacity - 1) % capacity; data[frontIndex] = value; ++count; }
    void pushBack(const T& value) { if (count == capacity) grow(); data[(frontIndex + count) % capacity] = value; ++count; }
    T popFront() { if (empty()) throw underflow_error("ArrayDeque is empty"); T v = data[frontIndex]; frontIndex = (frontIndex + 1) % capacity; --count; return v; }
    T popBack() { if (empty()) throw underflow_error("ArrayDeque is empty"); size_t i = (frontIndex + count - 1) % capacity; T v = data[i]; --count; return v; }
    bool empty() const { return count == 0; }
    size_t size() const { return count; }
};

// 6. Linked-list deque. Both head and tail are maintained.
template <typename T>
class LinkedDeque {
    struct Node { T value; Node* prev = nullptr; Node* next = nullptr; explicit Node(const T& v) : value(v) {} };
    Node* head = nullptr; Node* tail = nullptr; size_t count = 0;
public:
    ~LinkedDeque() { while (!empty()) popFront(); }
    void pushFront(const T& value) { Node* n = new Node(value); n->next = head; if (head) head->prev = n; else tail = n; head = n; ++count; }
    void pushBack(const T& value) { Node* n = new Node(value); n->prev = tail; if (tail) tail->next = n; else head = n; tail = n; ++count; }
    T popFront() { if (empty()) throw underflow_error("LinkedDeque is empty"); Node* old = head; T v = old->value; head = head->next; if (head) head->prev = nullptr; else tail = nullptr; delete old; --count; return v; }
    T popBack() { if (empty()) throw underflow_error("LinkedDeque is empty"); Node* old = tail; T v = old->value; tail = tail->prev; if (tail) tail->next = nullptr; else head = nullptr; delete old; --count; return v; }
    bool empty() const { return head == nullptr; }
    size_t size() const { return count; }
};

template <typename F>
void expectEmpty(const string& name, F operation) {
    try { operation(); cout << name << " empty handling: FAIL\n"; }
    catch (const underflow_error& e) { cout << name << " empty handling: PASS (" << e.what() << ")\n"; }
}

int main() {
    cout << "Assignment 05: Stacks, Queues, and Deques\n\n";

    ArrayStack<int> as; for (int i = 1; i <= 5; ++i) as.push(i); cout << "ArrayStack: "; for (int i = 0; i < 5; ++i) cout << as.pop() << ' '; cout << "\n"; expectEmpty("ArrayStack", [&] { as.pop(); });
    LinkedStack<int> ls; for (int i = 1; i <= 5; ++i) ls.push(i); cout << "LinkedStack: "; for (int i = 0; i < 5; ++i) cout << ls.pop() << ' '; cout << "\n"; expectEmpty("LinkedStack", [&] { ls.pop(); });

    ArrayQueue<int> aq; for (int i = 1; i <= 5; ++i) aq.enqueue(i); cout << "ArrayQueue: "; for (int i = 0; i < 5; ++i) cout << aq.dequeue() << ' '; cout << "\n"; expectEmpty("ArrayQueue", [&] { aq.dequeue(); });
    LinkedQueue<int> lq; for (int i = 1; i <= 5; ++i) lq.enqueue(i); cout << "LinkedQueue: "; for (int i = 0; i < 5; ++i) cout << lq.dequeue() << ' '; cout << "\n"; expectEmpty("LinkedQueue", [&] { lq.dequeue(); });

    ArrayDeque<int> ad; ad.pushBack(2); ad.pushFront(1); ad.pushBack(3); ad.pushFront(0); ad.pushBack(4); cout << "ArrayDeque: " << ad.popFront() << ' ' << ad.popBack() << ' ' << ad.popFront() << ' ' << ad.popBack() << ' ' << ad.popFront() << "\n"; expectEmpty("ArrayDeque", [&] { ad.popFront(); });
    LinkedDeque<int> ld; ld.pushBack(2); ld.pushFront(1); ld.pushBack(3); ld.pushFront(0); ld.pushBack(4); cout << "LinkedDeque: " << ld.popFront() << ' ' << ld.popBack() << ' ' << ld.popFront() << ' ' << ld.popBack() << ' ' << ld.popFront() << "\n"; expectEmpty("LinkedDeque", [&] { ld.popBack(); });

    cout << "All six structures completed at least 10 operations each (five insertions and five removals), plus empty-removal tests.\n";
}
