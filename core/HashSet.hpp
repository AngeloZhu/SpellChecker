// HashSet.hpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
//
// A HashSet is an implementation of a Set that is a separately-chained
// hash table, implemented as a dynamically-allocated array of linked
// lists.  At any given time, the HashSet has a "size" indicating
// how many elements are stored within it, along with a "capacity"
// indicating the size of the array.
//
// As elements are added to the HashSet and the proportion of the HashSet's
// size to its capacity exceeds 0.8 (i.e., there are more than 80% as many
// elements as there are array cells), the HashSet should be resized so
// that it is twice as large as it was before.
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to use a dynamically-
// allocated array and your own linked list implemenation; the linked list
// doesn't have to be its own class, though you can do that, if you'd like.

#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"



template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction is a function that takes a reference to a const
    // ElementType and returns an unsigned int.
    using HashFunction = std::function<unsigned int(const ElementType&)>;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    explicit HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    virtual ~HashSet() noexcept;

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s) noexcept;

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8.  In the case
    // where the array is resized, this function runs in linear time (with
    // respect to the number of elements, assuming a good hash function);
    // otherwise, it runs in constant time (again, assuming a good hash
    // function).
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // elementsAtIndex() returns the number of elements that hashed to a
    // particular index in the array.  If the index is out of the boundaries
    // of the array, this function returns 0.
    unsigned int elementsAtIndex(unsigned int index) const;


    // isElementAtIndex() returns true if the given element hashed to a
    // particular index in the array, false otherwise.  If the index is
    // out of the boundaries of the array, this functions returns 0.
    bool isElementAtIndex(const ElementType& element, unsigned int index) const;



private:
    struct Node
    {
        ElementType value;
        Node* next;
    };

    HashFunction hashFunction;
    Node** hash_set;
    int max_capacity;
    int current_size;

    void copyHashSet(Node** const &source, Node** target, const unsigned int max_size) const;
    void initializeHashSet(Node** hs, const unsigned int max_size) const;
    void addToHashSet(Node* &first_pointer, Node* &old_node) const;

};



namespace impl_
{
    template <typename ElementType>
    unsigned int HashSet__undefinedHashFunction(const ElementType& element)
    {
        return 0;
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashFunction hashFunction)
    : hashFunction{hashFunction}, hash_set{new Node*[DEFAULT_CAPACITY]}, max_capacity{DEFAULT_CAPACITY}, current_size{0}
{
    initializeHashSet(hash_set, max_capacity);
}


template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{
    for (int i = 0; i < max_capacity; ++i)
    {
        Node* node = hash_set[i];
        while(node != nullptr)
        {
            Node* temp = node;
            node = node->next;
            delete temp;
        }
    }
    delete[] hash_set;
}


template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
    : hashFunction{s.hashFunction}, hash_set{new Node*[DEFAULT_CAPACITY]}, max_capacity{s.max_capacity}, current_size{s.current_size}
{
    initializeHashSet(hash_set, max_capacity);
    copyHashSet(s.hash_set, hash_set, s.max_capacity);
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
{

    max_capacity = s.max_capacity;
    hash_set = new Node*[DEFAULT_CAPACITY];
    hashFunction = impl_::HashSet__undefinedHashFunction<ElementType>;
    current_size = 0;

    initializeHashSet(hash_set, max_capacity);
    std::swap(max_capacity, s.max_capacity);
    std::swap(hash_set, s.hash_set);
    std::swap(hashFunction, s.hashFunction);
    std::swap(current_size, s.current_size);
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
    if (this != &s)
    {
        hashFunction = s.hashFunction;
        max_capacity = s.max_capacity;
        current_size = s.current_size;
        this->~HashSet();

        hash_set = new Node*[max_capacity];
        initializeHashSet(hash_set, max_capacity);
        copyHashSet(s.hash_set, hash_set, max_capacity);
    }
    return *this;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
    if (this != &s)
    {
        std::swap(hashFunction, s.hashFunction);
        std::swap(max_capacity, s.max_capacity);
        std::swap(current_size, s.current_size);
        std::swap(hash_set, s.hash_set);  
    }
    return *this;
}


template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
    return true;
}

template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
    if (!contains(element))
    {
        if ((double)(current_size + 1) > max_capacity)
        {
            int new_max_capacity = max_capacity * 2;
            Node** new_hash_set = new Node*[new_max_capacity];
            initializeHashSet(new_hash_set, new_max_capacity);

            for (int i = 0; i < max_capacity; ++i)
            {
                Node* node = hash_set[i];

                while (node != nullptr)
                {
                    Node* temp = node;
                    unsigned int hash_index = hashFunction(node->value) % new_max_capacity;
                    node = node->next;
                    addToHashSet(new_hash_set[hash_index], temp);
                }
            }
            delete[] hash_set;
            max_capacity = new_max_capacity;
            hash_set = new_hash_set;
        }

        unsigned int h_index = hashFunction(element) % max_capacity;
        Node* new_node = new Node{element, hash_set[h_index]};
        hash_set[h_index] = new_node;
        current_size++;
    }
}


template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
    unsigned int hash_index = hashFunction(element) % max_capacity;
    Node* node = hash_set[hash_index];

    while(node != nullptr)
    {
        if (node->value == element)
        {
            return true;
        }
        node = node->next;
    }
    return false;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
    return current_size;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
    unsigned int count = 0;

    if (index >= max_capacity)
    {
        return count;
    }

    Node* node = hash_set[index];
    while (node != nullptr)
    {
        count++;
        node = node->next;
    }
    return count;
}


template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{
    if (index >= max_capacity)
    {
        return false;
    }

    Node* node = hash_set[index];
    while (node != nullptr)
    {
        if (node->value == element)
        {
            return true;
        }
        node = node->next;
    }
    return false;
}

template <typename ElementType>
void HashSet<ElementType>::initializeHashSet(Node** hs, const unsigned int max_size) const
{
    for (int i = 0; i < max_size; ++i)
    {
        hs[i] = nullptr;
    }
}


template <typename ElementType>
void HashSet<ElementType>::copyHashSet(Node** const &source, Node** target, const unsigned int max_size) const
{
    for (int i = 0; i < max_size; ++i)
    {
        Node* copied_pointer = source[i];
        Node* target_pointer = target[i];
        if (copied_pointer == nullptr)
        {
            target_pointer = nullptr;
        }
        while (copied_pointer != nullptr)
        {
            target_pointer->value = copied_pointer->value;
            target_pointer->next = copied_pointer->next;
            copied_pointer = copied_pointer->next;
            target_pointer = target_pointer->next;
        }
    }
}

template <typename ElementType>
void HashSet<ElementType>::addToHashSet(Node* &first_pointer, Node* &old_node) const
{
    Node* new_node = new Node{old_node->value, first_pointer};
    first_pointer = new_node;
    delete old_node;
}


#endif // HASHSET_HPP

