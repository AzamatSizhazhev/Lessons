#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <functional>
#include <string>

using namespace std;

class Node {
public:
    int value;
    Node* left;
    Node* right;

    Node(int v) : value(v), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
    Node* root;
    mutable mutex mtx;

    Node* insertNode(Node* node, int value) {
        if (node == nullptr)
            return new Node(value);

        if (value < node->value)
            node->left = insertNode(node->left, value);
        else if (value > node->value)
            node->right = insertNode(node->right, value);

        return node;
    }

    Node* findMin(Node* node) {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    Node* removeNode(Node* node, int value) {
        if (node == nullptr)
            return nullptr;

        if (value < node->value) {
            node->left = removeNode(node->left, value);
        } else if (value > node->value) {
            node->right = removeNode(node->right, value);
        } else {
            if (node->left == nullptr) {
                Node* rightChild = node->right;
                delete node;
                return rightChild;
            }
            if (node->right == nullptr) {
                Node* leftChild = node->left;
                delete node;
                return leftChild;
            }
            Node* minRight = findMin(node->right);
            node->value = minRight->value;
            node->right = removeNode(node->right, minRight->value);
        }
        return node;
    }

    void mapNode(Node* node, function<int(int)> op) {
        if (node == nullptr)
            return;

        node->value = op(node->value);
        mapNode(node->left, op);
        mapNode(node->right, op);
    }

    Node* cutSubtreeNode(Node* node, int value) {
        if (node == nullptr)
            return nullptr;

        if (value < node->value) {
            node->left = cutSubtreeNode(node->left, value);
            return node;
        }
        if (value > node->value) {
            node->right = cutSubtreeNode(node->right, value);
            return node;
        }
        destroyTree(node);
        return nullptr;
    }

    void destroyTree(Node* node) {
        if (node == nullptr)
            return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

    void printNode(Node* node, int depth) const {
        if (node == nullptr)
            return;
        printNode(node->right, depth + 1);
        for (int i = 0; i < depth; ++i)
            cout << "    ";
        cout << node->value << endl;
        printNode(node->left, depth + 1);
    }

    bool containsNode(Node* node, int value) const {
        if (node == nullptr)
            return false;
        if (value == node->value)
            return true;
        if (value < node->value)
            return containsNode(node->left, value);
        return containsNode(node->right, value);
    }

public:
    BinarySearchTree() : root(nullptr) {}

    ~BinarySearchTree() {
        destroyTree(root);
    }

    void insert(int value) {
        lock_guard<mutex> lock(mtx);
        root = insertNode(root, value);
    }

    void remove(int value) {
        lock_guard<mutex> lock(mtx);
        root = removeNode(root, value);
    }

    void map(function<int(int)> op) {
        lock_guard<mutex> lock(mtx);
        mapNode(root, op);
    }

    void cut_subtree(int value) {
        lock_guard<mutex> lock(mtx);
        root = cutSubtreeNode(root, value);
    }

    bool contains(int value) const {
        lock_guard<mutex> lock(mtx);
        return containsNode(root, value);
    }

    void print() const {
        lock_guard<mutex> lock(mtx);
        printNode(root, 0);
    }
};

int main() {
    BinarySearchTree tree;

    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);

    cout << "Дерево после вставки" << endl;
    tree.print();

    cout << endl;
    cout << "contains(40): " << (tree.contains(40) ? "true" : "false") << endl;
    cout << "contains(99): " << (tree.contains(99) ? "true" : "false") << endl;

    tree.map([](int x) { return x * 2; });
    cout << endl << "После map(x * 2)" << endl;
    tree.print();

    tree.remove(30);
    cout << endl << "После remove(30)" << endl;
    tree.print();

    tree.cut_subtree(60);
    cout << endl << "После cut_subtree(60)" << endl;
    tree.print();

    cout << endl << "Многопоточный тест" << endl;

    BinarySearchTree mtTree;

    vector<thread> insertThreads;
    for (int t = 0; t < 4; ++t) {
        insertThreads.push_back(thread([&mtTree, t]() {
            for (int i = 0; i < 25; ++i)
                mtTree.insert(t * 25 + i);
        }));
    }
    for (size_t i = 0; i < insertThreads.size(); ++i)
        insertThreads[i].join();

    cout << "После 4 потоков вставки (0..99):" << endl;
    mtTree.print();

    vector<thread> mapThreads;
    mapThreads.push_back(thread([&mtTree]() {
        mtTree.map([](int x) { return x + 100; });
    }));
    mapThreads.push_back(thread([&mtTree]() {
        mtTree.map([](int x) { return x + 100; });
    }));
    for (size_t i = 0; i < mapThreads.size(); ++i)
        mapThreads[i].join();

    cout << endl << "После 2 потоков map(x + 100):" << endl;
    mtTree.print();

    return 0;
}