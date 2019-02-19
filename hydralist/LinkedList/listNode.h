#ifndef _LISTNODE_H
#define _LISTNODE_H
#include <mutex>

#define key_t int
#define MAX_ENTRIES 8
class ListNode
{
private:
    key_t min;
    key_t keyArray[MAX_ENTRIES];
    ListNode* next;
    int numEntries;
    std::mutex lock;
    bool deleted;

    void split();
    void merge();
    int getKeyInsertIndex(key_t key);
    int getKeyIndex(key_t key);
    bool insertAtIndex(key_t key, int index);
    bool removeFromIndex(int index);

public:
    ListNode();
    ListNode(key_t min, int numEntries, ListNode* next);
    bool insert(key_t key);
    bool remove(key_t key);
    bool checkRange(key_t key);
    bool try_lock();
    void unlock();
    void setNext(ListNode* next);
    void setMin(key_t key);
    void setNumEntries(int numEntries);
    void setDeleted(bool deleted);
    ListNode* getNext();
    int getNumEntries();
    key_t getMin();
    key_t* getKeyArray();
    bool getDeleted();

};
#endif
