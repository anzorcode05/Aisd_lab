
#pragma once

#include <ctime>
#include <iostream>

template<typename K, typename V>
class HashTable {
    enum class SlotState {
        EMPTY,
        FILLED,
        DELETED
    };

    struct Element {
        K key;
        V value;
        SlotState state;

        Element() : key(), value(), state(SlotState::EMPTY) {}
        Element(const K& key, const V& value) : key(key), value(value), state(SlotState::FILLED) {}
    };

    Element* elements;
    size_t capacity;
    size_t size;

    size_t hash(const K& key) const {
        return key % capacity;
    }

    size_t probe(size_t index, size_t i) const {
        return (index + i) % capacity;
    }

public:
    HashTable() : size(0) {
        const size_t CAPACITY = 100'000;
        capacity = CAPACITY;
        elements = new Element[capacity];

        srand(time(0));

        for (size_t i = 0; i < capacity; ++i) {
            insert(rand(), rand());
        }
    }

    HashTable(size_t cap) : capacity(cap), size(0) {
        elements = new Element[capacity];
    }

    HashTable(const HashTable& other) : capacity(other.capacity), size(other.size) {
        elements = new Element[capacity];
        for (size_t i = 0; i < capacity; ++i) {
            elements[i] = other.elements[i];
        }
    }

    HashTable& operator=(const HashTable& other) {
        if (this != &other) {
            HashTable tmp(other);
            std::swap(elements, tmp.elements);
            std::swap(capacity, tmp.capacity);
            std::swap(size, tmp.size);
        }
        return *this;
    }

    ~HashTable() { delete[] elements; }

    bool insert(const K& key, const V& value) {
        size_t index = hash(key);
        for (size_t i = 0; i < capacity; ++i) {
            size_t idx = probe(index, i);
            if (elements[idx].state == SlotState::EMPTY || elements[idx].state == SlotState::DELETED) {
                elements[idx] = Element(key, value);
                elements[idx].state = SlotState::FILLED;
                ++size;
                return true;
            }
            if (elements[idx].state == SlotState::FILLED && elements[idx].key == key) {
                /*
                if (elements[idx].value != value) {
                    elements[idx].value = value;
                    return true;
                }
                */
                return false;
            }
        }
        return false;
    }

    void insertOrAssign(const K& key, const V& value) {
        size_t index = hash(key);
        for (size_t i = 0; i < capacity; ++i) {
            size_t idx = probe(index, i);
            if (elements[idx].state == SlotState::EMPTY || elements[idx].state == SlotState::DELETED) {
                elements[idx] = Element(key, value);
                elements[idx].state = SlotState::FILLED;
                ++size;
                return;
            }
            if (elements[idx].state == SlotState::FILLED && elements[idx].key == key) {
                elements[idx].value = value;
                return;
            }
        }
    }

    V* search(const K& key) {
        size_t index = hash(key);
        for (size_t i = 0; i < capacity; ++i) {
            size_t idx = probe(index, i);
            if (elements[idx].state == SlotState::EMPTY) {
                return nullptr;
            }
            if (elements[idx].state == SlotState::FILLED && elements[idx].key == key) {
                return &elements[idx].value;
            }
        }
        return nullptr;
    }

    bool erase(const K& key) {
        size_t index = hash(key);
        for (size_t i = 0; i < capacity; ++i) {
            size_t idx = probe(index, i);
            if (elements[idx].state == SlotState::EMPTY) {
                return false;
            }
            if (elements[idx].state == SlotState::FILLED && elements[idx].key == key) {
                elements[idx].state = SlotState::DELETED;
                --size;
                return true;
            }
        }
        return false;
    }



        bool contains(const V& value) {
        for (size_t i = 0; i < capacity; ++i) {
            if (elements[i].state == SlotState::FILLED && elements[i].value == value)
                return true;
        }
        return false;
    }

    int count(const K& key) {
        size_t index = hash(key);
        int result = 0;
        for (size_t i = 0; i < capacity; ++i) {
            size_t idx = probe(index, i);
            if (elements[idx].state == SlotState::EMPTY) {
                break;
            }
            if (elements[idx].state == SlotState::FILLED && hash(elements[idx].key) == index) {
                ++result;
            }
        }
        return result;
    }

    void print() {
        for (size_t i = 0; i < capacity; ++i) {
            if (elements[i].state == SlotState::FILLED) {
                std::cout << elements[i].key << ": " << elements[i].value << '\n';
            }
        }
    }

    size_t getSize() const {
        return size;
    }
};