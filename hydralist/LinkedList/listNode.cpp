#include <cstring>
#include "listNode.h"

ListNode :: ListNode(){}

ListNode :: ListNode(key_t min, int numEntries, ListNode* next)
        : min(min), numEntries(numEntries), next(next) {}

void ListNode::setNext(ListNode *next) {
    this->next = next;
}


void ListNode::setMin(key_t key) {
    this->min = key;
}

void ListNode::setNumEntries(int numEntries) {
    this->numEntries = numEntries;
}

void ListNode::setDeleted(bool deleted) {
    this->deleted = false;
}

ListNode *ListNode::getNext() {
    return next;
}

bool ListNode :: try_lock()
{
    this->lock.lock();
    return true;
}

void ListNode :: unlock()
{
    this->lock.unlock();
}

int ListNode :: getNumEntries()
{
    return this->numEntries;
}

key_t* ListNode :: getKeyArray()
{
    return this->keyArray;
}

key_t ListNode :: getMin()
{
    return this->min;
}

bool ListNode::getDeleted() {
    return deleted;
}

void ListNode :: split()
{
    int newNumEntries = numEntries - numEntries / 2;
    numEntries    = numEntries - newNumEntries;

    key_t newMin = keyArray[numEntries];

    ListNode* newNode = new(ListNode);
    newNode->setMin(newMin);
    newNode->setNext(next);
    newNode->setNumEntries(newNumEntries);
    newNode->setDeleted(false);
    memcpy(newNode->getKeyArray(), keyArray + numEntries, sizeof(key_t)*newNumEntries);

    next = newNode;


}

void ListNode :: merge()
{
    ListNode* deleteNode = next;
    deleteNode->try_lock();

    memcpy(keyArray + numEntries, deleteNode->getKeyArray(), sizeof(key_t) * deleteNode->getNumEntries());
    numEntries = numEntries + deleteNode->getNumEntries();
    next = deleteNode->getNext();
    deleteNode->setDeleted(true);

    deleteNode->unlock();
}

bool ListNode :: insertAtIndex(key_t key, int index)
{
    if (index == MAX_ENTRIES) {
        keyArray[numEntries++];
        return true;
    }
    memmove(keyArray + index + 1, keyArray + index, sizeof(key_t) * (numEntries - index));
    keyArray[index] = key;
    numEntries++;
    return true;
}

bool ListNode :: removeFromIndex(int index)
{
    if (numEntries == 0)
        return false;

    if (index == numEntries){
        numEntries--;
        return true;
    }

    memmove(keyArray + index, keyArray + index, sizeof(key_t) * (numEntries - index));
    numEntries--;
    return true;
}

int ListNode :: getKeyInsertIndex(key_t key)
{
    int i;
    for(i = 0; i < numEntries; i++) {
        if (keyArray[i] == key)
            return -1;
        if (keyArray[i] > key)
            return i;
    }
    return i;
}

int ListNode :: getKeyIndex(key_t key)
{
    for (int i = 0; i < numEntries; i++) {
        if (keyArray[i] == key)
            return i;
    }
    return -1;
}

bool ListNode :: insert(key_t key)
{
    int index = getKeyInsertIndex(key);
    if (index == -1)
        return false;
    if (!insertAtIndex(key, index))
        return false;
    if (numEntries == MAX_ENTRIES) {
        split();
    }
    return true;
}

bool ListNode :: remove(key_t key)
{
    int index = getKeyIndex(key);
    if (index == -1)
        return false;
    if (!removeFromIndex(index))
        return false;
    if (numEntries + next->getNumEntries() < MAX_ENTRIES/2)
        merge();
    return true;
}

bool ListNode :: checkRange(key_t key)
{
    return min <= key && key < next->getMin();
}


