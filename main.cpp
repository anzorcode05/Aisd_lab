#include <iostream>
#include <random>
#include <vector>
#include <functional>

using namespace std;

struct stats {
    size_t comparisonCount = 0;
    size_t copyCount = 0;
};

ostream& operator<<(ostream& os, const vector<int>& arr) {
    for (auto el : arr) {
        os<< el << ' ';
    }
    return os << '\n';
}

stats insertionSort(std::vector<int>& arr) {
    stats ans;
    for (size_t i = 1; i < arr.size(); ++i) {
        int value = arr[i];
        ++ans.copyCount; // Копируем число во временную переменную value

        int j = i - 1;
        while (j >= 0 && arr[j] > value) {
            ++ans.comparisonCount; // Считаем сравнение arr[j] > value
            arr[j + 1] = arr[j];
            ++ans.copyCount; // Считаем копирование arr[j + 1] = arr[j]
            --j;
        }
        ++ans.comparisonCount; // Считаем последнее сравнение arr[j] > value, после которого вышли
        arr[j + 1] = value; // Копируем value в правильную позицию
        ++ans.copyCount; // Учитываем это копирование
    }
    return ans;
}

stats shakerSort(vector<int>& arr) {
    stats ans;
    if (arr.empty()) {
        return ans;
    }

    int left = 0;
    int right = arr.size() - 1;
    while (left <= right) {
        for (int i = right; i > left; --i) {
            ++ans.comparisonCount;
            if (arr[i - 1] > arr[i]) {
                swap(arr[i - 1], arr[i]);
                ans.copyCount += 3;
            }
        }
        ++left;
        for (int i = left; i < right; ++i) {
            ++ans.comparisonCount;
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                ans.copyCount += 3;
            }
        }
        --right;
    }
    return ans;
}

int getRandomPivot(vector <int>& v, int left, int right) {
    mt19937 gen(random_device{}());
    uniform_int_distribution <int> range(left, right);
    return v[range(gen)];
}

struct Partition_points {
    int for_left_part;
    int for_right_part;
};

Partition_points partition(vector <int>& v, int left, int right, int pivot, stats& ans) {
    int less = left;
    for (int i = left; i <= right; ++i) {
        ++ans.comparisonCount;
        if (v[i] < pivot) {
            swap(v[less], v[i]);
            ans.copyCount += 3;
            less++;
        }
    }
    int greater = right;
    for (int i = right; i >= less; --i) {
        ++ans.comparisonCount;
        if (v[i] > pivot) {
            swap(v[greater], v[i]);
            ans.copyCount += 3;
            greater--;
        }
    }

    return { less - 1, greater + 1 };
}

void recursiveQuickSortImpl(vector <int>& v, int left, int right, stats& ans) {
    if (left < right) {
        int pivot = getRandomPivot(v, left, right);
        Partition_points p = partition(v, left, right, pivot, ans);
        recursiveQuickSortImpl(v, left, p.for_left_part, ans);
        recursiveQuickSortImpl(v, p.for_right_part, right, ans);
    }
}

stats quickSort(vector <int>& v) {
    stats ans;
    recursiveQuickSortImpl(v, 0, v.size() - 1, ans);
    return ans;
}

vector<int> createRandomArr(size_t size) {
    vector<int> arr(size);
    static mt19937 gen(random_device{}());
    uniform_int_distribution<int> range(0, 999);

    for (size_t i = 0; i < size; ++i) {
        arr[i] = range(gen);
    }

    return arr;
}

vector<int> createSortedArr(size_t size) {
    vector<int> arr(size);

    for (size_t i = 0; i < size; ++i) {
        arr[i] = (int)i;
    }

    return arr;
}

vector<int> createReverseArr(size_t size) {
    vector<int> arr(size);

    for (size_t i = 0; i < size; ++i) {
        arr[i] = (int)(size - i);
    }

    return arr;
}

void calcAvgStatsRandom() {
    cout << "////////////////Random Arrays////////////////" << endl << endl;

    stats s, total;

    vector<size_t> sizes = { 1000, 2000 , 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000,25000, 50000, 100000  };

    for (size_t size : sizes) {
        cout << "-------------------------------" << endl;
        cout << "\nSize of array: " << size << endl << endl;

        total.comparisonCount = 0;
        total.copyCount = 0;

        for (size_t i = 0; i < 100; ++i) {
            vector<int> arr = createRandomArr(size);

            s = insertionSort(arr);

            total.comparisonCount += s.comparisonCount;
            total.copyCount += s.copyCount;
        }

        cout << "-----insertion Sort-----" << '\n';
        cout << "Avg comparison count: " << total.comparisonCount / 100 << '\n';
        cout << "Avg copy count: " << total.copyCount / 100 << "\n\n";

        total.comparisonCount = 0;
        total.copyCount = 0;

        for (size_t i = 0; i < 100; ++i) {
            vector<int> arr = createRandomArr(size);

            s = shakerSort(arr);

            total.comparisonCount += s.comparisonCount;
            total.copyCount += s.copyCount;
        }

        cout << "-----Shaker Sort-----" << '\n';
        cout << "Avg comparison count: " << total.comparisonCount / 100 << '\n';
        cout << "Avg copy count: " << total.copyCount / 100 << "\n\n";

        total.comparisonCount = 0;
        total.copyCount = 0;

        for (size_t i = 0; i < 100; ++i) {
            vector<int> arr = createRandomArr(size);

            s = quickSort(arr);

            total.comparisonCount += s.comparisonCount;
            total.copyCount += s.copyCount;
        }

        cout << "-----Quick Sort-----" << '\n';
        cout << "Avg comparison count: " << total.comparisonCount / 100 << '\n';
        cout << "Avg copy count: " << total.copyCount / 100 << "\n\n";
    }
}

void calcAvgStatsSorted() {
    cout << "////////////////Sorted Arrays////////////////" << endl << endl;

    stats s, total;

    vector<size_t> sizes = { 1000, 2000 , 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 25000, 50000, 100000 };

    for (size_t size : sizes) {

        cout << "-------------------------------" << endl;
        cout << "\nSize of array: " << size << endl << endl;

        total.comparisonCount = 0;
        total.copyCount = 0;

        vector<int> arr = createSortedArr(size);

        s = insertionSort(arr);

        total.comparisonCount += s.comparisonCount;
        total.copyCount += s.copyCount;

        cout << "-----Insertion Sort-----" << endl;
        cout << "Comparison count: " << total.comparisonCount << endl;
        cout << "Copy count: " << total.copyCount << endl << endl;

        total.comparisonCount = 0;
        total.copyCount = 0;

        s = shakerSort(arr);

        total.comparisonCount += s.comparisonCount;
        total.copyCount += s.copyCount;

        cout << "-----Shake Sort-----" << endl;
        cout << "Comparison count: " << total.comparisonCount << endl;
        cout << "Copy count: " << total.copyCount << endl << endl;

        total.comparisonCount = 0;
        total.copyCount = 0;

        s = quickSort(arr);

        total.comparisonCount += s.comparisonCount;
        total.copyCount += s.copyCount;

        cout << "-----Quick Sort-----" << endl;
        cout << "Comparison count: " << total.comparisonCount << endl;
        cout << "Copy count: " << total.copyCount << endl << endl;
    }
}

void calcAvgStatsReverse() {

    cout << "///////////////Reverse Arrays///////////////" << endl << endl;

    stats s, total;

    vector<size_t> sizes = { 1000, 2000 , 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 25000, 50000, 100000 };

    for (size_t size : sizes) {

        cout << "-------------------------------" << endl;
        cout << "\nSize of array: " << size << endl << endl;

        total.comparisonCount = 0;
        total.copyCount = 0;

        vector<int> arr1 = createReverseArr(size);

        s = insertionSort(arr1);

        total.comparisonCount += s.comparisonCount;
        total.copyCount += s.copyCount;

        cout << "-----Insertion Sort-----" << endl;
        cout << "Comparison count: " << total.comparisonCount << endl;
        cout << "Copy count: " << total.copyCount << endl << endl;

        total.comparisonCount = 0;
        total.copyCount = 0;

        vector<int> arr2 = createReverseArr(size);

        s = shakerSort(arr2);

        total.comparisonCount += s.comparisonCount;
        total.copyCount += s.copyCount;

        cout << "-----Shake Sort-----" << endl;
        cout << "Comparison count: " << total.comparisonCount << endl;
        cout << "Copy count: " << total.copyCount << endl << endl;

        total.comparisonCount = 0;
        total.copyCount = 0;

        vector<int> arr3 = createReverseArr(size);

        s = quickSort(arr3);

        total.comparisonCount += s.comparisonCount;
        total.copyCount += s.copyCount;

        cout << "-----Quick Sort-----" << endl;
        cout << "Comparison count: " << total.comparisonCount << endl;
        cout << "Copy count: " << total.copyCount << endl << endl;
    }
}

int main() {
    const size_t size = 10;
    vector<int> arr1 = createRandomArr(size);
    cout << "----------------------------------\n";
    cout << "Array_1: ";
    cout << arr1;
    cout << "----------------------------------\n\n";

    cout << "-----------------Insertion Sort-----------------\n\n";
    stats iSStats = insertionSort(arr1);
    cout << arr1 << endl;
    cout << "Comparisons count: " << iSStats.comparisonCount << '\n';
    cout << "Copies count: " << iSStats.copyCount << "\n\n";

    vector<int> arr2 = createRandomArr(size);
    cout << "----------------------------------\n";
    cout << "Array_2: ";
    cout << arr2;
    cout << "----------------------------------\n\n";

    cout << "-----------------Shaker Sort-----------------\n\n";
    stats sSStats = shakerSort(arr2);
    cout << arr2 << endl;
    cout << "Comparisons count: " << sSStats.comparisonCount << '\n';
    cout << "Copies count: " << sSStats.copyCount << "\n\n";

    vector<int> arr3 = createRandomArr(size);

    cout << "----------------------------------\n";
    cout << "Array_3: ";
    cout << arr3;
    cout << "----------------------------------\n\n";

    cout << "-----------------Quick Sort-----------------\n\n";
    auto qsStats = quickSort(arr3);
    cout << arr3 << '\n';
    cout << "Comparions count: " << qsStats.comparisonCount << '\n';
    cout << "Copies count: " << qsStats.copyCount << "\n\n";

    cout << "---------------------------------------------------------------------------\n";
    cout << endl << "Second Task:" << endl << endl;

    //calcAvgStatsRandom();
    calcAvgStatsReverse();
    //calcAvgStatsSorted();
}