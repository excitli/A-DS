using namespace std;



class Node {
    int data;
    Node* parent;
    Node* leftSon;
    Node* rightSon;
    Node(int val) {data = val, parent = nullptr, leftSon = nullptr, rightSon = nullptr;}
};

