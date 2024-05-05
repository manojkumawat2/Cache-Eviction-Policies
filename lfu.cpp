#include<stdioc++.h>

class Node {
public:
    int key, value, freq;
    Node *prev, *next;

    Node(int _key, int _value) {
        key = _key;
        value = _value;
        freq = 1;
        prev = nullptr;
        next = nullptr;
    }
};

class List {
public:
    int size;
    Node *head, *tail;

    List() {
        size = 0;
        head = new Node(0, 0); // dummy node
        tail = new Node(0, 0); // dummy node
        head->next = tail;
        tail->prev = head;
    }

    void insertAtFront(Node *node) {
        Node* temp = head->next;
        node->next = temp;
        node->prev = head;
        head->next = node;
        temp->prev = node;
        size++;
    }

    void removeNode(Node *node) {
        Node* delprev = node->prev;
        Node* delnext = node->next;
        delprev->next = delnext;
        delnext->prev = delprev;
        size--;
    }
};

class LFUCache {
private:
    int capacity;
    map<int, Node*> keyNode;
    map<int, List*> freqListMap;
    int minFreq;

public:
    LFUCache(int capacity) {
        this->capacity = capacity;
    }

    void updateFreqListMap(Node* node) {
        keyNode.erase(node->key); 
        if (node->freq > 0) {
            freqListMap[node->freq]->removeNode(node);
        }
            
        if (node->freq == minFreq && freqListMap[node->freq]->size == 0) {
            minFreq++;
        }

        List *nextHigherFreqList = new List();
        if (freqListMap.find(node->freq + 1) != freqListMap.end()) {
            nextHigherFreqList = freqListMap[node->freq + 1];
        }

        node->freq += 1;
        nextHigherFreqList->insertAtFront(node);
        freqListMap[node->freq] = nextHigherFreqList;
        keyNode[node->key] = node;
    }
    
    int get(int key) {
        if (keyNode.find(key) == keyNode.end()) {
            return -1;
        }

        Node* node = keyNode[key];
        updateFreqListMap(node);
        cout<<"freq "<<node->freq<<endl;
        return node->value;
    }
    
    void put(int key, int value) {
        if (capacity == 0) {
            return;
        }

        if (keyNode.find(key) != keyNode.end()) {
            Node *node = keyNode[key];
            node->value = value;
            updateFreqListMap(node);
            return;
        }

        if (keyNode.size() == capacity) {
            List *lst = freqListMap[minFreq];
            // cout<<lst->tail->prev->value<<endl;
            keyNode.erase(lst->tail->prev->key);
            freqListMap[minFreq]->removeNode(lst->tail->prev);
        }

        Node* node = new Node(key, value);

        minFreq = 1;
        List* lstFreq = new List();
        if (freqListMap.find(minFreq) != freqListMap.end()) {
            lstFreq = freqListMap[minFreq];
        }
        lstFreq->insertAtFront(node);
        keyNode[key] = node;
        freqListMap[minFreq] = lstFreq;
    }
};