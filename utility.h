#ifndef utility_h
#define utility_h

#include <iostream>
#include <map>
#include <vector>
#include <limits.h>
using namespace std;

class Node
{
public:
    Node *previous, *next;
    int value, key, count;
    
    Node(){};
    
    Node(int key, int value, int count)
    {
        this->key = key; this->value = value; this->count = count;
    }
    
};

class DoublyLinkedList
{
private:
    Node *head = nullptr;
    Node *tail = nullptr;
    int size{0};
public:
    
    void push_back(Node *node)
    {
        if(head == nullptr)
        {
            head = node;
            tail = node;
        }
        else
        {
            tail->next = node;
            node->previous = tail;
            node->next = nullptr;
            tail = node;
        }
        size++;
    }
    void remove(Node *node)
    {
        if(size == 0)
        {
            cout << "list empty "<<endl;
            return;
        }
        else if(size == 1)
        {
            delete head;
            head = nullptr;
            tail = nullptr;
        }
        // There are two cases when either previous node is null or next node is nullptr.
        else if(node == head)
        {
            head = head->next;
            delete node;
            head->previous = nullptr;
        }
        else if(node == tail)
        {
            tail = tail->previous;
            delete node;
            tail->next = nullptr;
        }
        // The general case where previous and next nodes are not null
        else
        {
            node->previous->next = node->next;
            node->next->previous = node->previous;
            delete node;
        }
        size --;
    }
    Node* getFirstNode()
    {
        return head;
    }
    
    int getSize()
    {
        return this->size;
    }
    
};

class NFUCache
{
private:
    int size{0};
    map<int, Node*> cache;
    map<int, DoublyLinkedList*> fMap;
    
public:
    int hits = 0, miss = 0;
    int get(int key)
    {
        if(cache.find(key) == cache.end())
            return -1;
        
        Node *node = cache.at(key);
        int ret = node->value;
        int c = node->count;
        fMap.at(c)->remove(node);
        if(fMap.at(c)->getSize() == 0)
        {
            delete fMap.at(c);
            fMap.erase(c);
        }
        if(fMap.find(c+1) == fMap.end()) // freq key does not exist, put a list at that location
        {
            DoublyLinkedList *list = new DoublyLinkedList;
            fMap[c+1] = list;
        }
        node = new Node(key, ret, c+1);
        fMap.at(c+1)->push_back(node);
        
        //Update cache with new address of that node for current key
        cache.at(key) = node;
        
        return ret;
    }
    
    bool put(int key, int value)
    {
        if(this->size == 0)
            return false;
        if(cache.find(key) != cache.end()) // update the existing key
        {
            Node *node = cache.at(key);
            int c = node->count;
            fMap.at(c)->remove(node);
            if(fMap.at(c)->getSize() == 0)
            {
                delete fMap.at(c);
                fMap.erase(c);
            }
            if(fMap.find(c+1) == fMap.end()) // freq key does not exist, put a list at that location
            {
                DoublyLinkedList *list = new DoublyLinkedList;
                fMap[c+1] = list;
            }
            node = new Node(key, value, c+1);
            fMap[c+1]->push_back(node);
            cache.at(key) = node;
        }
        else if(cache.size() < size) // space available
        {
            Node *node = new Node(key, value, 1);
            cache[key] = node;
            
            if(fMap.find(1) == fMap.end()) // freq key does not exist, put a list at that location
            {
                DoublyLinkedList *list = new DoublyLinkedList;
                fMap[1] = list;
            }
            fMap.at(1)->push_back(node);
        }
        else // cache full, need to remove items
        {
            DoublyLinkedList *list = fMap.begin()->second;
            Node *nodeToRemove = list->getFirstNode();
            int c = fMap.begin()->first;
            
            cache.erase(nodeToRemove->key);
            list->remove(nodeToRemove);
            nodeToRemove = nullptr;
            if(fMap.at(c)->getSize() == 0)
            {
                delete fMap.at(c);
                fMap.erase(c);
            }
            Node *node = new Node(key, value, 1);
            cache[key] = node;
            if(fMap.find(1) == fMap.end())
            {
                DoublyLinkedList * list = new DoublyLinkedList;
                fMap[1] = list;
            }
            fMap.at(1)->push_back(node);
        }
        return true;
    }
    
    NFUCache(int size = 0)
    {
        this->size = size;
    }
};

class LRUCache
{
    int size{0};
    map<int, Node*> cache;
    DoublyLinkedList lruList;
    
public:
    int hits{}, miss{};
    int get(int key)
    {
        if(cache.count(key) == 0)
        {
            // cout << "key not found" << endl;
            return -1;
        }
        Node *node = cache.at(key);
        int ret = node->value;
        lruList.remove(node);
        node = new Node;
        node->value = ret;
        node->key = key;
        lruList.push_back(node);
        cache.at(key) = node;
        return ret;
    }
    
    bool put(int key, int value)
    {
        if(cache.count(key) > 0)
        {
            Node *node = cache.at(key);
            lruList.remove(node);
            node = new Node;
            node->value = value;
            node->key = key;
            lruList.push_back(node);
            cache.at(key) = node;
            return true;
        }
        else // key not present
        {
            if(cache.size() < size) // cahce has empty space
            {
                Node *node = new Node;
                node->value = value;
                node->key = key;
                cache[key] = node;
                lruList.push_back(node);
                return true;
            }
            else if(cache.size() == size) // cache is full
            {
                Node *node = lruList.getFirstNode();
                cache.erase(node->key);
                lruList.remove(node);
                node = new Node;
                node->key = key;
                node->value = value;
                lruList.push_back(node);
                cache[key] = node;
            }
        }
        return false;
    }
    
    LRUCache(int capacity)
    {
        this->size = capacity;
    }
    
};

class AegingCache
{
public:
    map<int, int> cache; // (page number, age_para)
    int size{};
    
public:
    int hits{}, miss{};
    AegingCache(int size)
    {
        this->size = size;
    }
    void decrementAgePara()
    {
        for(auto itr = cache.begin(); itr != cache.end(); itr++)
            itr->second--;
    }
    void eraseOldest()
    {
        int min_para = INT_MAX;
        int oldest_key = -1;
        for(auto itr = cache.begin(); itr != cache.end(); itr++)
        {
            if(itr->second < min_para)
            {
                min_para = itr->second;
                oldest_key = itr->first;
            }
        }
        cache.erase(oldest_key);
    }
    void put(int key)
    {
        if(cache.count(key) != 0)
        {
            cache.at(key) = INT_MAX;
        }
        
        else if(cache.size() < size) // key not present, but cache has space left
        {
            cache[key] = INT_MAX;
        }
        else // key not present, and cache is also full
        {
            eraseOldest();
            cache[key] = INT_MAX;
        }
        
    }
    
    int get(int key) // returns frequency if page is abscent, else its age_para
    {
        if(cache.count(key) == 0)
            return -1;
        return cache.at(key) = INT_MAX;
    }
};


#endif /* utility_h */
