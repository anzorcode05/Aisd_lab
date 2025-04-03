#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <set>
#include <windows.h>

using namespace std;

size_t lcg() {
    static size_t x = 0;
    x = (1021 * x + 24631) % 116640;
    return x;
}

struct Node {
    int val;
    Node* left;
    Node* right;
    Node(int val) : val(val), left(nullptr), right(nullptr) {}
};

class BST {
private:
    Node* root;

    void clear(Node* node) {
        if (node == nullptr)
            return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

    Node* copy(Node* node) const {
        if (node == nullptr) {
            return nullptr;
        }
        Node* new_node = new Node(node->val);
        new_node->left = copy(node->left);
        new_node->right = copy(node->right);

        return new_node;
    }

    void print(Node* node) const {
        if (node == nullptr)
            return;

        print(node->left);
        cout << node->val << ' ';
        print(node->right);
    }

    bool insert(Node*& node, int val) {
        if (node == nullptr) {
            node = new Node(val);
            return true;
        }
        if (val == node->val) {
            return false;
        }
        if (val < node->val) {
            return insert(node->left, val);
        }
        else {
            return insert(node->right, val);
        }
    }

    bool contains(Node* node, int val) const {
        if (node == nullptr) {
            return false;
        }
        if (val == node->val) {
            return true;
        }
        if (val < node->val) {
            return contains(node->left, val);
        }
        else {
            return contains(node->right, val);
        }
    }

    bool erase(Node*& node, int val) {
        if (node == nullptr) {
            return false;
        }

        if (val < node->val) {
            return erase(node->left, val);
        }
        else if (val > node->val) {
            return erase(node->right, val);
        }
        else { // Нашли узел для удаления
            // Случай 1: У узла нет левого потомка
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                node = temp;
                return true;
            }
            // Случай 2: У узла нет правого потомка
            else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                node = temp;
                return true;
            }
            // Случай 3: У узла два потомка
            else {
                Node* temp = node->right;
                // Ищем наименьший элемент в правом поддереве
                while (temp->left != nullptr) {
                    temp = temp->left;
                }

                node->val = temp->val; // Копируем значение

                // Удаляем наименьший элемент из правого поддерева
                bool success = eraseMin(node->right); // Используем специальную функцию
                return success;
            }
        }
    }

    bool eraseMin(Node*& node) {
        if (node == nullptr) {
            return false; // Не должно произойти
        }

        if (node->left == nullptr) {
            // Это наименьший элемент
            Node* temp = node->right; // Сохраняем правого потомка
            delete node;              // Удаляем узел
            node = temp;              // Обновляем указатель
            return true;              // Успешно удалили
        }
        else {
            // Идем влево
            return eraseMin(node->left);
        }
    }

    void inorderTraversal(Node* node, vector<int>& elements) const { //?
        if (node == nullptr)
            return;
        inorderTraversal(node->left, elements);
        elements.push_back(node->val);
        inorderTraversal(node->right, elements);
    }

    void findIntersection(Node* node, const BST& other,
        vector<int>& result) const { //?
        if (node == nullptr)
            return;

        if (other.contains(node->val)) {
            result.push_back(node->val);
        }

        findIntersection(node->left, other, result);
        findIntersection(node->right, other, result);
    }

public:
    BST() : root(nullptr) {}
    BST(const BST& other) { root = copy(other.root); }
    ~BST() { clear(); }
    BST& operator=(const BST& other) {
        if (this != &other) {
            clear(root);
            root = copy(other.root);
        }
        return *this;
    }

    void print() const {
        print(root);
        cout << endl;
    }

    bool insert(int val) { return insert(root, val); }

    bool contains(int val) const { return contains(root, val); }

    bool erase(int val) { return erase(root, val); }

    void clear() {
        clear(root);
        root = nullptr;
    }

    void inorderTraversal(vector<int>& elements) const {
        inorderTraversal(root, elements);
    }

    void findIntersection(const BST& other, vector<int>& result) const {
        findIntersection(root, other, result);
    }
};

vector<int> getUnion(const BST& tree1, const BST& tree2) {
    vector<int> result;
    tree1.inorderTraversal(result);
    tree2.inorderTraversal(result);

    sort(result.begin(), result.end());
    auto last = unique(result.begin(), result.end());
    result.erase(last, result.end());

    return result;
}

// Функция для пересечения двух деревьев
vector<int> getIntersection(const BST& tree1, const BST& tree2) {
    vector<int> result;
    tree1.findIntersection(tree2, result);
    return result;
}

void FillTreeWithRandom(BST& tree, size_t count) {
    set<int> s;
    size_t cur_size = 0;
    while (cur_size < count) {
        int random_number = lcg();

        assert(tree.contains(random_number) == s.contains(random_number));
        if (tree.contains(random_number))
            continue;

        s.insert(random_number);
        tree.insert(random_number);
        ++cur_size;
    }
}

void fillingTask(int size) {
    cout << "Size: " << size << endl;
    double bst_time = 0;
    double vec_time = 0;
    for (int i = 0; i < 100; i++) {
        BST tree;
        auto begin = std::chrono::steady_clock::now();
        FillTreeWithRandom(tree, size);
        auto end = std::chrono::steady_clock::now();
        auto elapsed_ms =
            std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        bst_time += elapsed_ms.count();

        vector<int> vec;
        auto begin1 = std::chrono::steady_clock::now();
        for (int j = 0; j < size; j++) {
            vec.push_back(lcg());
        }
        auto end1 = std::chrono::steady_clock::now();
        auto elapsed_ms1 =
            std::chrono::duration_cast<std::chrono::microseconds>(end1 - begin1);
        vec_time += elapsed_ms1.count();
    }
    cout << "Time to fill the BST tree: " << (bst_time / 100) << " microseconds"
        << endl;
    cout << "Vector filling time: " << (vec_time / 100) << " microseconds"
        << endl;
}

void searchTime(int size) {
    cout << "Size: " << size << endl;
    double bst_time = 0;
    double vec_time = 0;
    for (int i = 0; i < 1000; i++) {
        BST tree;
        FillTreeWithRandom(tree, size);

        int random_number = lcg();
        auto begin = std::chrono::steady_clock::now();
        bool is_in = tree.contains(random_number);
        auto end = std::chrono::steady_clock::now();
        auto elapsed_ms =
            std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        bst_time += elapsed_ms.count();

        vector<int> vec;
        for (int i = 0; i < size; i++) {
            vec.push_back(lcg());
        }
        int random_number1 = lcg();
        auto begin1 = std::chrono::steady_clock::now();
        auto a = find(vec.begin(), vec.end(), random_number1);
        auto end1 = std::chrono::steady_clock::now();
        auto elapsed_ms1 =
            std::chrono::duration_cast<std::chrono::microseconds>(end1 - begin1);
        vec_time += elapsed_ms1.count();
    }
    cout << "BST tree number search time: " << (bst_time / 100) << " microseconds"
        << endl;
    cout << "Vector number search time: " << (vec_time / 100) << " microseconds"
        << endl;
}

void addingAndDeletingTime(int size) {
    cout << "Size: " << size << endl;
    double bst_time = 0;
    double vec_time = 0;
    for (int i = 0; i < 1000; i++) {
        BST tree;
        FillTreeWithRandom(tree, size);
        int rand_to_add = lcg();
        int rand_to_delete = lcg();

        auto begin = std::chrono::steady_clock::now();
        tree.insert(rand_to_add);
        tree.erase(rand_to_delete);
        auto end = std::chrono::steady_clock::now();

        auto elapsed_ms =
            std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        bst_time += elapsed_ms.count();

        vector<int> vec;
        for (int i = 0; i < size; i++) {
            vec.push_back(lcg());
        }
        rand_to_add = lcg();
        rand_to_delete = lcg();

        auto begin1 = std::chrono::steady_clock::now();
        vec.push_back(rand_to_add);
        auto it = find(vec.begin(), vec.end(), rand_to_delete);
        if (it != vec.end()) {
            swap(vec[it - vec.begin()], vec[vec.size() - 1]);
            vec.pop_back();
        }
        auto end1 = std::chrono::steady_clock::now();

        auto elapsed_ms1 =
            std::chrono::duration_cast<std::chrono::microseconds>(end1 - begin1);
        vec_time += elapsed_ms1.count();
    }
    cout << "Time of adding and removing numbers of a BST tree: "
        << (bst_time / 100) << " nanoseconds" << endl;
    cout << "Time to add and remove a vector number: " << (vec_time / 100)
        << " nanoseconds" << endl;
}

int main() {
    srand(time(0));
    // setlocale(LC_ALL, "ru");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    BST tree;
    tree.insert(50);
    tree.insert(30);
    tree.insert(20);
    tree.print();
    tree.insert(20);
    tree.print();
    if (tree.contains(10)) {
        cout << "tree contains 10\n";
    }
    else {
        cout << "tree not contains 10\n";
    }
    tree.erase(20);
    tree.print();
    tree.insert(20);
    tree.print();

    BST tree1;
    tree1.insert(5);
    tree1.insert(10);

    BST tree2;
    tree2.insert(30);
    tree2.insert(20);

    cout << "tree contains\n";
    tree.print();

    cout << "tree1 contains\n";
    tree1.print();

    cout << "tree2 contains\n";
    tree2.print();

    vector<int> v = getUnion(tree, tree1);
    cout << "The union of tree and tree1: " << endl;
    for (int i = 0; i < v.size(); ++i) {
        cout << v[i] << ' ';
    }
    cout << '\n';
    vector<int> v1 = getIntersection(tree, tree2);
    cout << "The intersection of tree and tree2: " << endl;
    for (int i = 0; i < v1.size(); ++i) {
        cout << v1[i] << ' ';
    }
    cout << '\n';

    const vector<int> sizes = { 1000, 10000, 100000 };
    for (int size : sizes) {
        cout << "Size of contanier: " << size << endl;
        fillingTask(size);
        searchTime(size);
        addingAndDeletingTime(size);
        cout << '\n';
    }
}