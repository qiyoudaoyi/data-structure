#pragma once

#include "Global.h"

template <class T>
class Node
{
    friend class Polynomial;
    friend class CircularList<T>;
    friend std::ostream &operator<<(std::ostream &out, const Polynomial &p);

    // private:
public:
    T data;
    Node<T> *link = nullptr;
    Node() {}
    Node(T element, Node *next = nullptr) : data(element), link(next) {}
};

template <class T>
class CircularList
{
    friend class Polynomial;
    friend std::ostream &operator<<(std::ostream &out, const Polynomial &p);

public:
    // constructor
    CircularList() { head->link = head; }
    CircularList(Node<T> *f);
    CircularList(const CircularList<T> &a);
    CircularList<T> &operator=(const CircularList<T> &a);
    ~CircularList();

    // CircularList manipulation
    void InsertFront(const T &e);
    void InsertBack(const T &e);
    void DeleteFront();
    void DeleteBack();
    void DeleteOdd();
    const T &Front();
    const T &Back();
    const T &Get(int i);
    void Delete(int i);
    void Insert(int i, const T &e);

    static Node<T> *GetNode();
    static void RetNode(Node<T> *&x);
    void SetAv();

    // HW
    int Length();
    void Patch(int k, const T &e);
    void Clear();

    // Debug
    void Print();

    static Node<T> *av;

private:
    Node<T> *head = new Node<T>(), *last = head;
};

template <class T>
CircularList<T>::CircularList(Node<T> *h) : head(h)
{
    Node<T> *now = head->link;
    while (now->link != head) now = now->link;
    last = now;
}

template <class T>
CircularList<T>::CircularList(const CircularList<T> &a)
{
    if (head != last) {
        last->link = av;
        av = head->link;
        head->link = nullptr;
    }

    if (a.head == a.last) return;

    Node<T> *now = head, *a_now = a.head->link;
    while (a_now != a.head) {
        now = now->link = new Node<T>(a_now->data);
        a_now = a_now->link;
    }
    last = now;
    last->link = head;
}

template <class T>
CircularList<T>::~CircularList()
{
    last->link = av;
    av = head;
}

template <class T>
CircularList<T> &CircularList<T>::operator=(const CircularList<T> &a)
{
    if (this == &a) return *this;
    if (head != last) {
        last->link = av;
        av = head->link;
        head->link = nullptr;
    }

    if (a.head == a.last) return *this;

    Node<T> *now = head, *a_now = a.head->link;
    while (a_now != a.head) {
        now = now->link = new Node<T>(a_now->data);
        a_now = a_now->link;
    }
    last = now;
    last->link = head;

    return *this;
}

template <class T>
void CircularList<T>::InsertFront(const T &e)
{
    // Create the new node
    Node<T> *newNode = new Node<T>(e);
    // the list is empty, last needs to take in consideration
    if (head == last) {
        last = head->link = newNode;
        last->link = head;
    } else {
        // add the node to the front as instructed
        Node<T> *front = head->link;
        newNode->link = front;
        head->link = newNode;
    }
}

template <class T>
void CircularList<T>::InsertBack(const T &e)
{
    // add node after last
    last = last->link = new Node<T>(e, head);
}

template <class T>
void CircularList<T>::DeleteFront()
{
    // The list is empty, do nothing
    if (head == last) return;

    // front to store head element
    Node<T> *front = head->link;
    if (front == last) last = head;
    head->link = front->link;
    delete front;
}

template <class T>
void CircularList<T>::DeleteBack()
{
    // The list is empty, do nothing
    if (head == last) return;

    // back to store last element
    Node<T> *back = last;

    // only one element in the list, assign both head and last to nullptr(0)
    if (head->link == last) {
        last = head->link = head;
    } else {
        // traverse until the element prior to last, and then skip it
        Node<T> *now = head->link;
        while (now->link != last) now = now->link;
        now->link = head;
        last = now;
    }
    delete back;
}

template <class T>
void CircularList<T>::DeleteOdd()
{
    // No elements in the list, do nothing
    if (head == last) return;
    // Only one element in the list, delete it and adjust head and last
    if (head->link == last) {
        delete head->link;
        last = head->link = head;
    }

    // front is the first element in the list
    // even and odd are the looping pointers
    Node<T> *front = head->link;
    Node<T> *even = front->link;
    Node<T> *odd = even->link;

    // delete front since 1 is odd and readjust head
    delete front;
    head->link = even;
    // Traverse through the list with even and odd
    // with odd being ahead of even by 1 node
    while (even != last && even->link != last) {
        // delete current odd and move on
        Node<T> *del = odd;
        even->link = odd->link;
        delete del;
        even = even->link;
        odd = even->link;
    }

    // Cases when the original length is odd
    if (even->link == last) {
        delete even->link;
        last = even;
    }
    // adjust last
    last->link = head;
}

template <class T>
const T &CircularList<T>::Front()
{
    if (head == nullptr) throw "The list is empty.";

    return head->data;
}

template <class T>
const T &CircularList<T>::Back()
{
    if (head == nullptr) throw "The list is empty.";

    return last->data;
}

template <class T>
const T &CircularList<T>::Get(int i)
{
    if (head == nullptr) throw "The list is empty.";
    if (i <= 0) throw "i must be a positive integer.";

    Node<T> *now = head;
    i--;
    while (now != last && i) {
        now = now->link;
        i--;
    }
    if (now == last && i > 0) throw "out of range.";

    return now->data;
}

template <class T>
void CircularList<T>::Delete(int i)
{
    if (head == nullptr) throw "The list is empty.";
    if (i <= 0) throw "i must be a positive integer.";
    if (i == 1) {
        DeleteFront();
        return;
    }

    Node<T> *now = head, *prev;
    i--;
    while (now != last && i) {
        prev = now;
        now = now->link;
        i--;
    }

    if (now == last && i > 0) throw "out of range.";

    prev->link = now->link;
    delete now;
}

template <class T>
void CircularList<T>::Insert(int i, const T &e)
{
    // Exception Cases
    if (head == last)
        if (i == 1) InsertFront(e);
        else throw "The list is empty.";

    if (i <= 0) throw "i must be a positive integer.";
    if (i > Length()) throw "out of range.";

    // When i = 1, InsertFront will do
    if (i == 1) {
        InsertFront(e);
        return;
    }

    // Traverse through the CircularList with now and prev
    Node<T> *now = head->link, *prev;

    // i-- at the begining since we want i-th.
    // Therefore, we only need to loop from i-1 to 0
    // Traverse through every node until i-th node
    for (i--; i; i--) {
        prev = now;
        now = now->link;
    }

    // Insert the node before i-th element
    // Note that the new node's link is pointing to the original i-th element
    prev->link = new Node<T>(e, now);
}

template <class T>
Node<T> *CircularList<T>::GetNode()
{
    if (av == nullptr) return new Node<T>();
    Node<T> *x = av;
    av = av->link;
    return x;
}

template <class T>
void CircularList<T>::RetNode(Node<T> *&x)
{
    x->link = av;
    av = x;
    x = nullptr;
}

template <class T>
int CircularList<T>::Length()
{
    // the list is empty, return 0
    if (head == last) return 0;
    // the length to count the number of nodes
    int l;
    // initialize now as the head node
    Node<T> *now = head->link;
    // Traverse through every node and increment l until nullptr(0)
    for (l = 0; now != head; l++) now = now->link;
    return l;
}

template <class T>
void CircularList<T>::Patch(int k, const T &e)
{
    // Exception Cases
    if (head == last) throw "The list is empty.";
    if (k <= 0) throw "k must be a positive integer.";
    if (k > Length()) throw "out of range.";

    // initialize now as the first node
    Node<T> *now = head->link;

    // k-- at the begining since we want k-th.
    // Therefore, we only need to loop from k-1 to 0.
    // Traverse through every node until k-th node.
    for (k--; k; k--) now = now->link;
    // the data field we want to change
    now->data = e;
}

template <class T>
void CircularList<T>::Clear()
{
    // Exception Case
    if (head == nullptr) throw "The list is empty.";

    // Traverse through the CircularList with now and prev.
    Node<T> *now;
    Node<T> *prev;

    // Traverse through every node and delete all of them until nullptr(0).
    for (now = head; now != nullptr;) {
        prev = now;
        now = now->link;
        delete prev;
    }

    // Re-assignment of head and last as nullptr(0).
    head = last = nullptr;
}

template <class T>
void CircularList<T>::Print()
{
    if (head == last) {
        std::cout << "The list is empty.\n";
        return;
    };

    Node<T> *now = head->link;
    std::cout << "first = " << now->data << ", last = " << last->data << '\n';
    for (; now != head; now = now->link)
        std::cout << now->data << " \n"[now == last ? 1 : 0];
}

template <class T>
Node<T> *CircularList<T>::av = nullptr;
