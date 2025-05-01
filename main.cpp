#include <algorithm>
#include <cassert>
#include <ctime>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "HashTable.h"

using namespace std;

void print(const vector<int>& numbers) {
    for (const int& number : numbers) {
        cout << number << ' ';
    }
    cout << '\n';
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(0));
    const int MAX_SIZE = 100;
    const int MAX_VALUE = 10;
    //const size_t size = 1 + rand() % MAX_SIZE;
    const size_t size = 20;

    vector<int> numbers(size);
    for (int& number : numbers) {
        number = rand() % MAX_VALUE;
    }
    unordered_map<int, int> forTest;
    HashTable<int, int> hashTable(100 * size);
    for (const int& number : numbers) {
        hashTable.insert(number, number);
        forTest.insert({ number, number });
    }
    assert(hashTable.getSize() == forTest.size());

    auto uniqueNumbers = numbers;
    sort(uniqueNumbers.begin(), uniqueNumbers.end());
    uniqueNumbers.erase(unique(uniqueNumbers.begin(), uniqueNumbers.end()), uniqueNumbers.end());
    assert(uniqueNumbers.size() == hashTable.getSize());

    cout << "Размер исходного вектора = " << size << '\n';
    cout << "Количество одинаковых чисел в случайно сгенерированном массиве  = " << size - hashTable.getSize() << '\n';
    cout << "Исходный массив: ";
    print(numbers);
    cout << "Уникальные значения из исходного массива: ";
    print(uniqueNumbers);

    cout << "Значения в хеш-таблице:\n";
    hashTable.print();

    return 0;
}
