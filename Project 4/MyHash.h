// MyHash.h

// Skeleton for the MyHash class template.  You must implement the first seven
// member functions; we have implemented the eighth.

#include <iostream>
using namespace std;
template<typename KeyType, typename ValueType>
class MyHash
{
public:
    MyHash(double maxLoadFactor = 0.5);
    ~MyHash();
    void reset();
    void associate(const KeyType& key, const ValueType& value);
    int getNumItems() const;
    double getLoadFactor() const;
    
    // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;
    
    // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
    }
    
    // C++11 syntax for preventing copying and assignment
    MyHash(const MyHash&) = delete;
    MyHash& operator=(const MyHash&) = delete;
    
private:
    double max_load_factor;
    int items;
    int buckets;
    
    struct node {
        KeyType first;
        ValueType second;
        node* next;
    };
    
    node* *bucketArr;
    
    void resizeBucketArray();
};

template<typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::MyHash(double maxLoadFactor) {
    
    /*
        Constructor: Initialize an array of 100 buckets and make them all
        point to the nullptr
    */
    
    if (maxLoadFactor <= 0)
        max_load_factor = 0.5;
    
    else if (maxLoadFactor > 2)
        max_load_factor = 2.0;
    
    else
        max_load_factor = maxLoadFactor;
    
    buckets = 100;
    bucketArr = new node*[buckets];
    
    //  All pointers should initially point to the nullptr
    for (int i = 0; i < buckets; i++) {
        bucketArr[i] = nullptr;
    }
}

template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::reset() {
    
    /*
        The function deletes the current hash table
        and resets it to its original state i.e.
        the one with 0 items and 100 buckets.
     */
    
    
    //  Loop over all buckets and delete the entire linked list
    for (int i = 0; i < buckets; i++) {
        if (bucketArr[i] == nullptr)
            continue;
        node* temp = bucketArr[i];
        while (bucketArr[i] != nullptr) {
            temp = bucketArr[i];
            bucketArr[i] = bucketArr[i]->next;
            delete temp;
        }
    }
    
    //  Delete the current dynamically allocated array of buckets
    //  and create a new one.
    
    delete [] bucketArr;
    buckets = 100;
    items = 0;
    
    bucketArr = new node*[buckets];
    
    //  All pointers should initially point to the nullptr
    for (int i = 0; i < buckets; i++) {
        bucketArr[i] = nullptr;
    }
}

template<typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::~MyHash() {
    
    /*
        Destructor: Destruct all dynamically allocated objects including
        the array and all linked lits.
    */
    
    //  Loop over all buckets and delete the entire linked list
    for (int i = 0; i < buckets; i++) {
        if (bucketArr[i] == nullptr)
            continue;
        node* temp = bucketArr[i];
        while (bucketArr[i] != nullptr) {
            temp = bucketArr[i];
            bucketArr[i] = bucketArr[i]->next;
            delete temp;
        }
    }
    
    delete [] bucketArr;
}

template<typename KeyType, typename ValueType>
int MyHash<KeyType, ValueType>::getNumItems() const {
    return items;
}

template<typename KeyType, typename ValueType>
double MyHash<KeyType, ValueType>::getLoadFactor() const{
    
    double currentLoadFactor = ((double)items)/buckets;
    return currentLoadFactor;
    
}

template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value) {
    
    /*
        Either inserts a new Key-Value pair or modifies an
        exisiting one.
     */
    
    unsigned int hash(const KeyType& k);
    unsigned int h = hash(key)%buckets;
    
    node* temp = bucketArr[h];
    
    //  Check to see if the key already exists
    /*
    while (temp != nullptr) {
        if (temp->first == key) {
            for (int i = 0; i < temp->second.size(); i++) {
                if (temp->second[i] == value[0])
                    return;
            }
            temp->second.push_back(value[0]);
            return;
        }
        temp = temp->next;
    }
    */
    
    while (temp != nullptr) {
        if (temp->first == key) {
            temp->second = value;
            return;
        }
        temp = temp->next;
    }
    
    items++;
    
    if (getLoadFactor() > max_load_factor) {
        resizeBucketArray();
        h = hash(key)%buckets;
    }
    
    node* newNode = new node;
    newNode->first = key;
    newNode->second = value;
    newNode->next = bucketArr[h];
    bucketArr[h] = newNode;
    
}

template<typename KeyType, typename ValueType>
const ValueType* MyHash<KeyType, ValueType>::find(const KeyType& key) const {
    
    /*
        The function finds the bucket in O(1) time using the hash function
        and it then loops over the linked list to find the required value.
        If it is not found, it return the nullptr.
    */
    
    unsigned int hash(const KeyType& k);
    unsigned int h = hash(key)%buckets;
    
    node* temp = bucketArr[h];
    
    while (temp != nullptr) {
        if (temp->first == key)
            return &temp->second;
        else
            temp = temp->next;
    }
    
    return nullptr;
}

template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::resizeBucketArray() {
    
    /*
        The function doubles the size of the previous dnyamically allocated
        array of buckets. It copies and recomputes the hash of each key-value
        pair.
     */
    
    int prevBuckets = buckets;
    buckets *= 2;
    
    //  Initialize the new array
    node* *tempArray = new node*[buckets];
    
    for (int i = 0; i < buckets; i++)
        tempArray[i] = nullptr;
    
    //  Copy key-value pairs from previous array to new array
    for (int i = 0; i < prevBuckets; i++) {
        
        if (bucketArr[i] == nullptr)
            continue;
        
        node* temp = bucketArr[i];
        while (temp != nullptr) {
            
            unsigned int hash(const KeyType& k);
            unsigned int h = hash(temp->first)%buckets;
            
            node* newNode = new node;
            newNode->first = temp->first;
            newNode->second = temp->second;
            newNode->next = tempArray[h];
            tempArray[h] = newNode;
            
            temp = temp->next;
        }
        
    }
    
    //  Delete content of the previous array
    for (int i = 0; i < prevBuckets; i++) {
        if (bucketArr[i] == nullptr)
            continue;
        
        node* temp = bucketArr[i];
        while (bucketArr[i] != nullptr) {
            temp = bucketArr[i];
            bucketArr[i] = bucketArr[i]->next;
            delete temp;
        }
    }
    
    delete [] bucketArr;
    
    bucketArr = tempArray;
}



