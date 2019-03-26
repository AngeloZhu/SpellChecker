// ListSet.hpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
//
// An implementation of the Set<ElementType> class template, which uses
// a singly-linked list with a head pointer to store its keys.  The keys
// are not sorted in any particular order.

#ifndef LISTSET_HPP
#define LISTSET_HPP

#include <algorithm>
#include "Set.hpp"



template <typename ElementType>
class ListSet : public Set<ElementType>
{
public:
    ListSet() noexcept;
    virtual ~ListSet() noexcept;
    ListSet(const ListSet& s);
    ListSet(ListSet&& s) noexcept;
    ListSet& operator=(const ListSet& s);
    ListSet& operator=(ListSet&& s) noexcept;

    virtual bool isImplemented() const noexcept override;
    virtual void add(const ElementType& element) override;
    virtual bool contains(const ElementType& element) const override;
    virtual unsigned int size() const noexcept override;

private:
    struct Node
    {
        ElementType element;
        Node* next;
    };

    Node* head;

private:
    Node* copyAll(const ListSet& s);
    void destroyAll(Node* head) noexcept;
};



template <typename ElementType>
ListSet<ElementType>::ListSet() noexcept
    : head{nullptr}
{
}


template <typename ElementType>
ListSet<ElementType>::~ListSet() noexcept
{
    destroyAll(head);
}


template <typename ElementType>
ListSet<ElementType>::ListSet(const ListSet& s)
    : head{nullptr}
{
    head = copyAll(s);
}


template <typename ElementType>
ListSet<ElementType>::ListSet(ListSet&& s) noexcept
    : head{nullptr}
{
    std::swap(head, s.head);
}


template <typename ElementType>
ListSet<ElementType>& ListSet<ElementType>::operator=(const ListSet& s)
{
    if (this != &s)
    {
        Node* newHead = copyAll(s);
        destroyAll(head);
        head = newHead;
    }

    return *this;
}


template <typename ElementType>
ListSet<ElementType>& ListSet<ElementType>::operator=(ListSet&& s) noexcept
{
    std::swap(head, s.head);
    return *this;
}


template <typename ElementType>
bool ListSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void ListSet<ElementType>::add(const ElementType& element)
{
    Node* curr = head;

    while (curr != nullptr)
    {
        if (curr->element == element)
        {
            return;
        }

        curr = curr->next;
    }

    head = new Node{element, head};
}


template <typename ElementType>
bool ListSet<ElementType>::contains(const ElementType& element) const
{
    Node* curr = head;

    while (curr != nullptr)
    {
        if (curr->element == element)
        {
            return true;
        }

        curr = curr->next;
    }

    return false;
}


template <typename ElementType>
unsigned int ListSet<ElementType>::size() const noexcept
{
    Node* curr = head;
    unsigned int count = 0;

    while (curr != nullptr)
    {
        ++count;
        curr = curr->next;
    }

    return count;
}


template <typename ElementType>
typename ListSet<ElementType>::Node* ListSet<ElementType>::copyAll(const ListSet& s)
{
    Node* curr = s.head;
    Node* newHead = nullptr;

    try
    {
        while (curr != nullptr)
        {
            newHead = new Node{curr->element, newHead};
            curr = curr->next;
        }

        return newHead;
    }
    catch (...)
    {
        destroyAll(newHead);
        throw;
    }
}


template <typename ElementType>
void ListSet<ElementType>::destroyAll(Node* head) noexcept
{
    Node* curr = head;

    while (curr != nullptr)
    {
        Node* temp = curr;
        curr = curr->next;
        delete temp;
    }
}



#endif // LISTSET_HPP

