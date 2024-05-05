#include<stdc++.h>

// List Node
class Node {
    public:
        int key, value;
        Node* prev;
        Node* next;

        Node(int key, int value) {
            this->key = key;
            this->value = value;
            this->prev = nullptr;
            this->next = nullptr;
        }
};

class LRUCache {
    private:
        // Map to store the cache key and value location in the linked list
        unordered_map<int, Node*> mp;
        // Head and tail node of the list
        Node *head, *tail;
        int cap;

    public:
        LRUCache(int capacity) {
            cap = capacity;
            head = new Node(0, 0); // dummy node
            tail = new Node(0, 0); // dummy node
            head->next = tail;
            tail->prev = head;
        }

        void remove(Node* curr) {
            mp.erase(curr->key);

            curr->next->prev = curr->prev;
            curr->prev->next = curr->next;
        }

        void insert(Node* curr) {
            curr->next = head->next;
            head->next->prev = curr;
            head->next = curr;
            curr->prev = head;
            mp[curr->key] = curr;
        }

        int get(int key) {
            if (mp.find(key) == mp.end()) {
                return -1;
            }

            Node* curr = mp[key];
            remove(curr);
            insert(curr);

            return curr->value;
        }

        void put(int key, int value) {
            if (mp.find(key) != mp.end()) {
                remove(mp[key]);
            }

            if (mp.size() == cap) {
                remove(tail->prev);
            }

            insert(new Node(key, value));
        }
};