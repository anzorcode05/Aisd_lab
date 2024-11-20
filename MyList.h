#include <iostream>
#include <stdexcept>

template <typename T>
class Node {
public:
    T data;
    Node* prev;
    Node* next;

    Node(const T& data) : data(data), prev(nullptr), next(nullptr) {}
};

template <typename T>
class MyList {
private:
    Node<T>* head;
    Node<T>* tail;
    size_t size_;

public:
    // ����������� �� ���������
    MyList() : head(nullptr), tail(nullptr), size_(0) {}

    // ����������� �����������
    MyList(const MyList& other) : head(nullptr), tail(nullptr), size_(0) {
        Node<T>* current = other.head;
        while (current != nullptr) {
            push_back(current->data);
            current = current->next;
        }
    }

    // �������� ������������ �����������
    MyList& operator=(const MyList& other) {
        if (this != &other) {
            Clear(); // ������� ������� ������
            Node<T>* current = other.head;
            while (current != nullptr) {
                PushBack(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    // ����������
    ~MyList() {
        Clear();
    }

    // ���������� � ������
    void PushFront(const T& data) {
        Node<T>* newNode = new Node<T>(data);
        if (head == nullptr) {
            head = tail = newNode;
        }
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        size_++;
    }

    // ���������� � �����
    void PushBack(const T& data) {
        Node<T>* newNode = new Node<T>(data);
        if (tail == nullptr) {
            head = tail = newNode;
        }
        else {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }
        size_++;
    }


    // ������ ������
    size_t Size() const {
        return size_;
    }

    // �������� [] ��� ������� �� �������
    T& operator[](size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        Node<T>* current = head;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->data;
    }

    const T& operator[](size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        Node<T>* current = head;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->data;
    }

    // ������� ������
    void Clear() {
        Node<T>* current = head;
        while (current != nullptr) {
            Node<T>* next = current->next;
            delete current;
            current = next;
        }
        head = tail = nullptr;
        size_ = 0;
    }
};