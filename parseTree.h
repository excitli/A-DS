#include <iostream>
using namespace std;
#include "node.h"
#include "dynarr.h"


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