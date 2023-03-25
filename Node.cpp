#include <iostream>
#include <string>
#include <concepts>
#include <vector>
#include <iomanip>
using namespace std;

// Структура для узла дерева
struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int data) {
        this->data = data;
        left = right = NULL;
    }
};

Node* balance(Node* root);
bool isBalanced(Node* root);

// Функция для добавления узла
// isBalance - балансировать ли дерево. Если true - то дерево будет также сбалансировано
Node* insert(Node* root, int data, bool isBalance) {
    if (root == NULL) {
        root = new Node(data);
    }
    else if (data < root->data) {
        root->left = insert(root->left, data, isBalance);
    }
    else {
        root->right = insert(root->right, data, isBalance);
    }

    if (isBalance) {
        if (!isBalanced(root)) {
            root = balance(root);
        }
    }
    return root;
} 

// Функция для поиска минимального узла
Node* minValueNode(Node* node) {
    Node* current = node;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}

// Функция для удаления узла
// isBalance - балансировать ли дерево. Если true - то дерево будет также сбалансировано 
Node* remove(Node* root, int data, bool isBalance) {
    if (root == NULL) {
        return root;
    }

    if (data < root->data) {
        root->left = remove(root->left, data, isBalance);
    }
    else if (data > root->data) {
        root->right = remove(root->right, data, isBalance);
    }
    else {
        if (root->left == NULL) {
            Node* temp = root->right;
            delete(root);
            return temp;
        }
        else if (root->right == NULL) {
            Node* temp = root->left;
            delete(root);
            return temp;
        }
        Node* temp = minValueNode(root->right);
        root->data = temp->data;
        root->right = remove(root->right, temp->data, isBalance);
    }

    if (isBalance) {
        if (!isBalanced(root)) {
            root = balance(root);
        }
    }

    return root;
}

// Функции для обхода дерева
void inorder(Node* root) {
    if (!root) return;
    inorder(root->left);
    cout << root->data << " ";
    inorder(root->right);
}

void preorder(Node* root) {
    if (!root) return;
    cout << root->data << " ";
    preorder(root->left);
    preorder(root->right);
}

void postorder(Node* root) {
    if (!root) return;
    postorder(root->left);
    postorder(root->right);
    cout << root->data << " ";
}

// Функция для очистки дерева
void deleteTree(Node* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

// Функции для поворота дерева
Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;
    x->right = y;
    y->left = T2;
    return x;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;
    y->left = x;
    x->right = T2;
    return y;
}

// Вычисление высоты дерева
int getHeight(Node* root) {
    if (!root) return 0;
    int left_height = getHeight(root->left);
    int right_height = getHeight(root->right);
    return max(left_height, right_height) + 1;
}


/*
Для балансировки дерева можно использовать алгоритм AVL-дерева.

AVL-дерево – это двоичное дерево поиска, в котором для каждой вершины высота каждого из её поддеревьев различается не более чем на 1.
*/

// Проверяем нарушение сбалансированности
bool isBalanced(Node* root) {
    if (!root) return true;
    int left_height = getHeight(root->left);
    int right_height = getHeight(root->right);
    return abs(left_height - right_height) <= 1 && isBalanced(root->left) && isBalanced(root->right);
}

// балансировка дерева
Node* balance(Node* root) {
    int left_height = getHeight(root->left);
    int right_height = getHeight(root->right);

    if (left_height - right_height > 1) {
        if (getHeight(root->left->left) >= getHeight(root->left->right)) {
            root = rightRotate(root);
        }
        else {
            root->left = leftRotate(root->left);
            root = rightRotate(root);
        }
    }
    else if (right_height - left_height > 1) {
        if (getHeight(root->right->right) >= getHeight(root->right->left)) {
            root = leftRotate(root);
        }
        else {
            root->right = rightRotate(root->right);
            root = leftRotate(root);
        }
    }

    return root;
}

// Возвращает true, если узел с заданным значением присутствует в дереве. Иначе false
bool searchNode(Node* root, int value) {
    if (!root) return false;

    if (root->data == value) return true;

    return searchNode(root->left, value) || searchNode(root->right, value);
}

// Более красивый вывод дерева в консоль
void PrintPostorder(Node* p, int indent)
{
    if (p != NULL) {
        if (p->right) {
            PrintPostorder(p->right, indent + 4);
        }
        if (indent) {
            std::cout << std::setw(indent) << ' ';
        }
        if (p->right) std::cout << " /\n" << std::setw(indent) << ' ';
        std::cout << p->data << "\n ";
        if (p->left) {
            std::cout << std::setw(indent) << ' ' << " \\\n";
            PrintPostorder(p->left, indent + 4);
        }
    }
}

// Объединение 2-х деревьев в одно дерево
Node* merge(Node* root1, Node* root2) {
    if (!root1) {
        return root2;
    }
    if (!root2) {
        return root1;
    }
    Node* merged = new Node(root1->data + root2->data);
    merged->left = merge(root1->left, root2->left);
    merged->right = merge(root1->right, root2->right);
    return merged;
}

// Добавить поддерево в дерево
void addSubtree(Node* root, Node* subtree) {
    if (root == NULL || subtree == NULL) {
        return;
    }

    if (root->left == NULL) {
        root->left = subtree;
    }
    else if (root->right == NULL) {
        root->right = subtree;
    }
    else {
        addSubtree(root->left, subtree);
    }
}

// Изменяет значение узла, если оно было найдено
void changeNodeValue(Node* root, int oldValue, int newValue) {
    if (root == NULL) return;
    if (root->data == oldValue) {
        root->data = newValue;
        return;
    }
    changeNodeValue(root->left, oldValue, newValue);
    changeNodeValue(root->right, oldValue, newValue);
}

// ======= ТЕСТЫ ==========
void balance_test();
void search_test();
void merge_test();
void addSubtree_test();
void changeNodeValue_test();

int main()
{
    // balance_test();
    // search_test();
    // merge_test();
    // addSubtree_test();
    changeNodeValue_test();
}

// Просто заполнение дерева данными для тестов
/*
                   2
             1            10
          0    n       3       n
                    n     8
                       n     n

*/
Node* generateTestData() {
    Node* root = new Node(2);
    bool isBalance = false;

    root = insert(root, 1, isBalance);
    root = insert(root, 10, isBalance);
    root = insert(root, 3, isBalance);
    root = insert(root, 8, isBalance);
    root = insert(root, 0, isBalance);
    return root;
}

void changeNodeValue_test() {
    Node* root = new Node(5);
    root->left = new Node(4);
    root->right = new Node(7);
    root->left->left = new Node(1);
    root->left->right = new Node(4);
    root->right->left = new Node(6);
    root->right->right = new Node(8);

    changeNodeValue(root, 4, 2);
    PrintPostorder(root, 0);
}

void addSubtree_test() {
    /*
         3
       /
     1
       \
        2
    */
    Node* root = new Node(1);
    Node* left = new Node(2);
    Node* right = new Node(3);

    addSubtree(root, left); // Поддерево добавилось в левую ветвь
    addSubtree(root, right); // Поддерево добавилось в правую ветвь

    PrintPostorder(root, 0);
}

void merge_test() {
    Node* root1 = generateTestData();
    Node* root2 = new Node(2);
    bool isBalance = false;

    root2 = insert(root2, 3, isBalance);
    root2 = insert(root2, 5, isBalance);
    root2 = insert(root2, -2, isBalance);
    root2 = insert(root2, 8, isBalance);
    root2 = insert(root2, 1, isBalance);

    Node* merged = merge(root1, root2);
    PrintPostorder(merged, 0);
    //PrintPostorder(root2, 0);
}

void search_test() {
    Node* root = generateTestData();
    
    cout << boolalpha << searchNode(root, 1) << endl; // true
    cout << boolalpha << searchNode(root, 200) << endl; // false
    cout << boolalpha << searchNode(root, 3) << endl; // true
    cout << boolalpha << searchNode(root, 0) << endl; // true
    cout << boolalpha << searchNode(root, 65) << endl; // false
    cout << boolalpha << searchNode(root, 8) << endl; // true
}

void balance_test() {
    Node* root = generateTestData();
    
    /*int height = getHeight(root);
    cout << height << endl;*/

    //inorder(root); // 0 1 2 3 8 10
    // //cout << endl;
    //preorder(root); // 2 1 0 10 3 8
    //cout << endl;
    //postorder(root); // 0 1 8 3 10 2

    //deleteTree(root);
    //cout << boolalpha << root->data << endl;
   
    //PrintPostorder(root, 0);
    /* Если isBalance = false, то
    
            10
          \
                8
              /
            3
      /
     2
       \
        1
          \
            0
    */

    /* Если isBalance = true, то
    
                10
          /
        8
          \
            3
      /
     2
       \
        1
          \
            0
    */
}
