using namespace std;
#include <iostream>
#include "fstream"


class Node {
public:
    int data;
    Node* parent;
    Node* leftSon;
    Node* rightSon;
    bool color; // true - red, false - black;
    Node(int val) {
        data = val; 
        parent = nullptr;
        leftSon = nullptr;
        rightSon = nullptr;
        color = false;
    }
};


void skipSpaces(const string& arr, int& pos) {
    while (pos < arr.size() && isspace(arr[pos])) pos++;
}

Node* parse(const string& arr, int& pos) {
    skipSpaces(arr, pos);
    if (pos >= arr.size() || arr[pos] != '(') return nullptr;
    pos++;
    skipSpaces(arr, pos);

    bool negative = false;
    int val = 0;
    if (arr[pos] == '-') {
        negative = true;
        pos++;
    }
    while (pos < arr.size() && isdigit(arr[pos])) {
        val = val * 10 + (arr[pos] - '0');
        pos++;
    }
    if (negative) val = -val;
    Node* node = new Node(val);
    skipSpaces(arr, pos);
    if (pos < arr.size() && arr[pos] == '(') {
        node -> leftSon = parse(arr, pos);
    }
    skipSpaces(arr, pos);
    if (pos < arr.size() && arr[pos] == '(') {
        node -> rightSon = parse(arr, pos);
    }
    if (pos < arr.size() && arr[pos] == ')') {pos++;}
    return node;
}


class BinaryTree {
public:
    Node* root;
    BinaryTree() {root = nullptr;}

    Node* add(Node* parent, int val, bool left) {
        Node* node = new Node(val);
        node -> parent = parent;
        if (parent) {
            if (left) parent->leftSon = node;
            else parent->rightSon = node;
        }
        else root = node;
        return node;
    }
    void dfs(Node* node) {
        if (!node) return;
        cout << node -> data << " ";
        dfs(node->leftSon);
        dfs(node->rightSon);
    }
};

class RBTree{
public:
    Node* root;
    RBTree() {root = nullptr;}
    

    void transplant(Node* u, Node* v) {
        if (u->parent == nullptr) root = v;
        else if (u == u->parent->leftSon) u->parent->leftSon = v;
        else u->parent->rightSon = v;
        if (v) v->parent = u->parent;
    }
    void leftRotate(Node* x) {
        Node* y = x->rightSon;
        x->rightSon = y->leftSon;
        if (y->leftSon) y->leftSon->parent = x;
        y->parent = x->parent;
        if (x->parent == nullptr) root = y;
        else if (x == x->parent->leftSon) x->parent->leftSon = y;
        else x->parent->rightSon = y;
        y->leftSon = x;
        x->parent = y;
    }
    void rightRotate(Node* x) {
        Node* y = x->leftSon;
        x->leftSon = y->rightSon;
        if (y->rightSon) y->rightSon->parent = x;
        y->parent = x->parent;
        if (x->parent == nullptr) root = y;
        else if (x == x->parent->rightSon) x->parent->rightSon = y;
        else x->parent->leftSon = y;
        y->rightSon = x;
        x->parent = y;
    }
    void insert(int key) {
        Node* t = new Node(key);
        t->color = true;
        t->leftSon = t->rightSon = nullptr;

        if (!root) {
            root = t;
            t -> parent = nullptr;
            t -> color = false;
            return;
        }
        Node *p = root;
        Node *q = nullptr;

        while (p != nullptr) {
            q = p;
            if (t -> data < p -> data) {p = p -> leftSon;}
            else p = p-> rightSon;
        }
        t-> parent = q;
        if (t-> data < q -> data) q -> leftSon = t;
        else q -> rightSon = t;
        fixInsertion(t);
    }
    Node* search(int key) {
        Node* p = root;
        while (p!=nullptr && p -> data != key) {
            if (key < p->data) p=p ->leftSon;
            else p = p->rightSon;
        }
        return p;
    }

    Node* minElem(Node* node) {
        while (node && node -> leftSon) {
            node = node -> leftSon;
        }
        return node;
    }

    void remove(int key) {
        Node* p = search(key);
        if (!p) return;
    
        Node* y = p;
        Node* q = nullptr;         
        bool yOriginalColor = y->color;
    
        if (p->leftSon == nullptr) {
            q = p->rightSon;
            transplant(p, p->rightSon);
        }

        else if (p->rightSon == nullptr) {
            q = p->leftSon;
            transplant(p, p->leftSon);
        }
        else {
            y = minElem(p->rightSon);
            yOriginalColor = y->color;
            q = y->rightSon;
    
            if (y->parent == p) {
                if (q) q->parent = y;
            } else {
                transplant(y, y->rightSon);
                y->rightSon = p->rightSon;
                y->rightSon->parent = y;
            }
    
            transplant(p, y);
            y->leftSon = p->leftSon;
            y->leftSon->parent = y;
            y->color = p->color;
        }
    
        delete p;
    
        if (yOriginalColor == false && q != nullptr)
            fixDelete(q);
    }

    void fixDelete(Node* p) {
        while (p != root && (p == nullptr || p->color == false)) {
            if (p == p->parent->leftSon) {
                Node* brother = p->parent->rightSon;
    
                if (brother && brother->color == true) {
                    brother->color = false;
                    p->parent->color = true;
                    leftRotate(p->parent);
                    brother = p->parent->rightSon;
                }
    
                if ((!brother->leftSon  || brother->leftSon->color == false) &&
                    (!brother->rightSon || brother->rightSon->color == false)) {
                    if (brother) brother->color = true;
                    p = p->parent;
                } else {
                    if (!brother->rightSon || brother->rightSon->color == false) {
                        if (brother->leftSon)
                            brother->leftSon->color = false;
                        brother->color = true;
                        rightRotate(brother);
                        brother = p->parent->rightSon;
                    }
                    brother->color = p->parent->color;
                    p->parent->color = false;
                    if (brother->rightSon)
                        brother->rightSon->color = false;
                    leftRotate(p->parent);
                    p = root;
                }
            } 
            else {
                Node* brother = p->parent->leftSon;
    
                if (brother && brother->color == true) {
                    brother->color = false;
                    p->parent->color = true;
                    rightRotate(p->parent);
                    brother = p->parent->leftSon;
                }

                if ((!brother->rightSon || brother->rightSon->color == false) &&
                    (!brother->leftSon  || brother->leftSon->color == false)) {
                    if (brother) brother->color = true;
                    p = p->parent;
                } else {
                    if (!brother->leftSon || brother->leftSon->color == false) {
                        if (brother->rightSon)
                            brother->rightSon->color = false;
                        brother->color = true;
                        leftRotate(brother);
                        brother = p->parent->leftSon;
                    }
                    brother->color = p->parent->color;
                    p->parent->color = false;
                    if (brother->leftSon)
                        brother->leftSon->color = false;
                    rightRotate(p->parent);
                    p = root;
                }
            }
        }
        if (p)
            p->color = false;
        root->color = false;
    }


    void fixInsertion(Node* t) {
        while (t != root && t->parent->color == true) { 
            Node* parent = t->parent;
            Node* grandparent = parent->parent;
    
            if (parent == grandparent->leftSon) { 
                Node* uncle = grandparent->rightSon;
    
                if (uncle && uncle->color == true) { 
                    parent->color = false;
                    uncle->color = false;
                    grandparent->color = true;
                    t = grandparent;
                } else {
                    if (t == parent->rightSon) { 
                        t = parent;
                        leftRotate(t);
                        parent = t->parent;
                        grandparent = parent->parent;
                    }
                    parent->color = false;
                    grandparent->color = true;
                    rightRotate(grandparent);
                }
            } else {
                Node* uncle = grandparent->leftSon;
    
                if (uncle && uncle->color == true) { 
                    parent->color = false;
                    uncle->color = false;
                    grandparent->color = true;
                    t = grandparent;
                } else {
                    if (t == parent->leftSon) {
                        t = parent;
                        rightRotate(t);
                        parent = t->parent;
                        grandparent = parent->parent;
                    }
                    parent->color = false;
                    grandparent->color = true;
                    leftRotate(grandparent);
                }
            }
        }
        root->color = false; 
    }
    
    void inorder(Node* node) {
        if (!node) return;
        inorder(node -> leftSon);
        cout << node -> data << (node -> color ? "R" : "B") << " ";
        inorder(node -> rightSon);
    }

    void preorder(Node* node) {
        if (!node) return;
        cout << node -> data << (node -> color? "R" : "B") << " ";
        preorder(node -> leftSon);
        preorder(node -> rightSon);
    }

    void dfs_rb(Node* node) {
        if (!node) return;
        dfs_rb(node -> leftSon);
        dfs_rb(node -> rightSon);
        cout << node -> data << (node -> color ? "R" : "B") << " ";
    }

    void iterativePreorder(Node* root) {
        if (!root) return;
    
        std::stack<Node*> s;
        s.push(root);
    
        while (!s.empty()) {
            Node* node = s.top();
            s.pop();
            std::cout << node->data << (node->color ? "R" : "B") << " ";
            if (node->rightSon) s.push(node->rightSon);
            if (node->leftSon) s.push(node->leftSon);
        }
    }

    void levelOrder() {
        if (!root) return;

        std::queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            Node* current = q.front();
            q.pop();
            std::cout << current->data << (current->color ? "R" : "B") << " ";
            if (current->leftSon)
                q.push(current->leftSon);
            if (current->rightSon)
                q.push(current->rightSon);
        }
        std::cout << std::endl;
    }
};

string readTree() {
    string treeString;
    std::ifstream in("tree.txt");
    if (in.is_open()) {
        getline(in, treeString);
        in.close();
        return treeString;
    }
    cout << "Error while opening file" << endl;
    return ")";
}

void traverseInsert(Node* node, RBTree& rbt) {
    if (!node) return;
    rbt.insert(node -> data);
    traverseInsert(node -> leftSon, rbt);
    traverseInsert(node -> rightSon, rbt);

}


void convertTree() {
    string treeStr = readTree();
    if (treeStr.empty()) {
        cout << "TreeStr string is empty";
        return;
    }
    BinaryTree BT;
    RBTree RBT;
    int pos = 0;
    BT.root = parse(treeStr, pos);

    if (!BT.root) {
        cout << "Invalid skobochnaya form";
        return;
    }

    traverseInsert(BT.root, RBT);

    cout << "Binary tree depth search" << endl;
    BT.dfs(BT.root);
    cout << endl;
    cout << endl;

    cout << "RBT tree (level order)" << endl;
    RBT.levelOrder();
    cout << endl;

    cout << "RBT tree (inorder)" << endl;
    RBT.inorder(RBT.root);
    cout << endl;

    cout << "RBT tree (post order)" << endl;
    RBT.dfs_rb(RBT.root);
    cout << endl;

    cout << "RBT tree (iterative preorder)" << endl;
    RBT.iterativePreorder(RBT.root);
    cout << endl;

    cout << "RBT tree (preorder)" << endl;
    RBT.preorder(RBT.root);
    cout << endl;
}


int main() {
    // BinaryTree tree;
    // string input = "(8 (9 (5)) (1))";
    // int pos = 0;
    // tree.root = parse(input, pos);
    // cout << tree.root -> data;
    // tree.dfs(tree.root);
    // RBTree t;
    // t.insert(10);
    // t.insert(5);
    // t.insert(15);
    // t.insert(3);
    // t.insert(7);
    // t.insert(0);
    // t.insert(12);
    // t.insert(11);
    // t.insert(9);
    // t.insert(6);
    // t.remove(10);
    // t.inorder(t.root);
    // cout << endl;
    // cout << t.root -> data;
    // cout << t.root -> color;
    convertTree();
    

};