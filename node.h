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