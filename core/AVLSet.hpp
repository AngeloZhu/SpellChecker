// AVLSet.hpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
//
// An AVLSet is an implementation of a Set that is an AVL tree, which uses
// the algorithms we discussed in lecture to maintain balance every time a
// new element is added to the set.  The balancing is actually optional,
// with a bool parameter able to be passed to the constructor to explicitly
// turn the balancing on or off (on is default).  If the balancing is off,
// the AVL tree acts like a binary search tree (e.g., it will become
// degenerate if elements are added in ascending order).
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to implement your AVL tree
// using your own dynamically-allocated nodes, with pointers connecting them,
// and with your own balancing algorithms used.

#ifndef AVLSET_HPP
#define AVLSET_HPP

#include <functional>
#include "Set.hpp"
#include <algorithm>


template <typename ElementType>
class AVLSet : public Set<ElementType>
{
public:
    // A VisitFunction is a function that takes a reference to a const
    // ElementType and returns no value.
    using VisitFunction = std::function<void(const ElementType&)>;

public:
    // Initializes an AVLSet to be empty, with or without balancing.
    explicit AVLSet(bool shouldBalance = true);

    // Cleans up the AVLSet so that it leaks no memory.
    virtual ~AVLSet() noexcept;

    // Initializes a new AVLSet to be a copy of an existing one.
    AVLSet(const AVLSet& s);

    // Initializes a new AVLSet whose contents are moved from an
    // expiring one.
    AVLSet(AVLSet&& s) noexcept;

    // Assigns an existing AVLSet into another.
    AVLSet& operator=(const AVLSet& s);

    // Assigns an expiring AVLSet into another.
    AVLSet& operator=(AVLSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement an AVLSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function always runs in O(log n) time
    // when there are n elements in the AVL tree.
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function always runs in O(log n) time when
    // there are n elements in the AVL tree.
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // height() returns the height of the AVL tree.  Note that, by definition,
    // the height of an empty tree is -1.
    int height() const;


    // preorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a preorder traversal of the AVL
    // tree.
    void preorder(VisitFunction visit) const;


    // inorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by an inorder traversal of the AVL
    // tree.
    void inorder(VisitFunction visit) const;


    // postorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a postorder traversal of the AVL
    // tree.
    void postorder(VisitFunction visit) const;


private:
    // You'll no doubt want to add member variables and "helper" member
    // functions here.
    struct Node
    {
        ElementType value;
        Node* left;
        Node* right;
        int node_height = 1;

    };

    Node* root;
    bool balance;
    int current_size;

    void copyAVLSet(Node* const &source, Node* &target) const;
    bool containsHelper(Node* const &node, const ElementType& element) const;
    void destroyTree(Node* &node) const;
    int findHeight(Node* const &node) const;
    int get_height(Node* node) const;
    void insert(const ElementType& element, Node* &node) const;
    void preorderHelper(Node* const &node, VisitFunction& visit) const;
    void inorderHelper(Node* const &node, VisitFunction& visit) const;
    void postorderHelper(Node* const &node, VisitFunction& visit) const;

    void rightRotate(Node* &node) const;
    void leftRotate(Node* &node) const;


};


template <typename ElementType>
AVLSet<ElementType>::AVLSet(bool shouldBalance)
    : root{nullptr}, balance{shouldBalance}, current_size{0}
{
}


template <typename ElementType>
AVLSet<ElementType>::~AVLSet() noexcept
{
    destroyTree(root);
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(const AVLSet& s)
{
    copyAVLSet(s.root, root);
    balance = s.balance;
    current_size = s.current_size;
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(AVLSet&& s) noexcept
{
    root = nullptr;
    std::swap(root, s.root);
    std::swap(balance, s.balance);
    std::swap(current_size, s.current_size);
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(const AVLSet& s)
{
    if (this != &s)
    {
        balance = s.balance;
        current_size = s.current_size;

        destroyTree(root);
        copyAVLSet(s.root, root);
    }
    return *this;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(AVLSet&& s) noexcept
{
    std::swap(balance, s.balance);
    std::swap(current_size, s.current_size);
    std::swap(root, s.root);

    return *this;
}


template <typename ElementType>
bool AVLSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void AVLSet<ElementType>::add(const ElementType& element)
{
    insert(element, root);
    current_size++;
}


template <typename ElementType>
bool AVLSet<ElementType>::contains(const ElementType& element) const
{
    return containsHelper(root, element);
}


template <typename ElementType>
unsigned int AVLSet<ElementType>::size() const noexcept
{
    return current_size;
}


template <typename ElementType>
int AVLSet<ElementType>::height() const
{
    return findHeight(root);
}


template <typename ElementType>
void AVLSet<ElementType>::preorder(VisitFunction visit) const
{
    preorderHelper(root, visit);
}


template <typename ElementType>
void AVLSet<ElementType>::inorder(VisitFunction visit) const
{
    inorderHelper(root, visit);
}

template <typename ElementType>
void AVLSet<ElementType>::postorder(VisitFunction visit) const
{
    postorderHelper(root, visit);
}

template<typename ElementType>
void AVLSet<ElementType>::copyAVLSet(Node* const &source, Node* &target) const
{
    if (source == nullptr)
    {
        target = nullptr;
    }
    else
    {
        target = new Node{source->value, nullptr, nullptr, 1};
        copyAVLSet(source->left, target->left);
        copyAVLSet(source->right, target->right);
    }
}

template <typename ElementType>
bool AVLSet<ElementType>::containsHelper(Node* const &node, const ElementType& element) const
{
    if (node == nullptr)
    {
        return false;
    }
    else if (node->value == element)
    {
        return true;
    }
    return containsHelper(node->left, element) || containsHelper(node->right, element);
}

template <typename ElementType>
void AVLSet<ElementType>::destroyTree(Node* &node) const
{
    if (node != nullptr)
    {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
    node = nullptr;
}

/*template <typename ElementType>
int AVLSet<ElementType>::findHeight(Node* const &node) const
{
    if (node == nullptr)
    {
        return -1;
    }
    node->node_height = 1 + std::max(findHeight(node->left), findHeight(node->right));
    return node->node_height;
}*/
// height running in o(n)?
template <typename ElementType>
int AVLSet<ElementType>::findHeight(Node* const &node) const
{
    if (node == nullptr)
    {
        return -1;
    }
    node->node_height = 1 + std::max(findHeight(node->left), findHeight(node->right));
    return node->node_height;
}

template <typename ElementType>
int AVLSet<ElementType>::get_height(Node* node) const
{
    if (node == nullptr)
    {
        return 0;
    }
    return node->node_height;
}


template <typename ElementType>
void AVLSet<ElementType>::insert(const ElementType& element, Node* &node) const
{
    if (!contains(element))
    {
        if (node == nullptr)
        {
            node = new Node{element, nullptr, nullptr, 1};
        }
        else if (node->value < element)
        {
            insert(element, node->right);
        }
        else if (node->value > element)
        {
            insert(element, node->left);
        }

        if (balance)
        {
            node->node_height = findHeight(node);
            int height_difference = findHeight(node->left) - findHeight(node->right);

            if (height_difference > 1 && element < node->left->value)
            {
                rightRotate(node);
            }
            if (height_difference < -1 && element > node->right->value)
            {
                leftRotate(node);
            }
            if (height_difference > 1 && element > node->left->value)
            {
                leftRotate(node->left);
                rightRotate(node);
            }
            if (height_difference < -1 && element < node->right->value)
            {
                rightRotate(node->right);
                leftRotate(node);
            }
        }
    }

}

template <typename ElementType>
void AVLSet<ElementType>::preorderHelper(Node* const &node, VisitFunction& visit) const 
{
    if (node != nullptr)
    {
        visit(node->value);
        preorderHelper(node->left, visit);
        preorderHelper(node->right, visit);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::inorderHelper(Node* const &node, VisitFunction& visit) const
{
    if (node != nullptr)
    {
        inorderHelper(node->left, visit);
        visit(node->value);
        inorderHelper(node->right, visit);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::postorderHelper(Node* const &node, VisitFunction& visit) const
{
    if (node != nullptr)
    {
        postorderHelper(node->left, visit);
        postorderHelper(node->right, visit);
        visit(node->value);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::rightRotate(Node* &node) const
{
    Node* x = node->left;

    node->left = x->right;
    x->right = node;

    node->node_height = std::max(get_height(node->left), get_height(node->right)) + 1;
    x->node_height = std::max(get_height(x->left), get_height(x->right)) + 1;

    node = x;

}

template <typename ElementType>
void AVLSet<ElementType>::leftRotate(Node* &node) const
{
    Node* x = node->right;

    node->right = x->left;
    x->left = node;

    node->node_height = std::max(get_height(node->left), get_height(node->right)) + 1;
    x->node_height = std::max(get_height(x->left), get_height(x->right)) + 1;

    node = x;
}


#endif // AVLSET_HPP

