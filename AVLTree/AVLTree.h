#pragma once
#include <utility> //std::move std::pair std::max std::forward
#include <stack>//std::stack

template <typename> class AVLTree;

template <typename T>
class AVLTIterator
{
private:
    
    friend class AVLTree<T>;
    using NodePtr = typename AVLTree<T>::Node*;

    //For the design of this iterator, I chose to use a stack of pointers instead of storing the parent 
    //pointers in each node or having a threaded tree. If the tree was just a BST and not an AVL tree, then
    //I would not do this because I wouldnt want to copy the pointer stack when making copies of
    //the iterator. However this is an iterator to an AVL tree which is balanced to always have
    //a height of log base 2 of N. This means its not terrible to copy the stack because it will
    //in the worst case only have log base 2 of N pointers in the stack to copy instead of a normal BST,
    //which in the worst case would have N pointers in the stack to copy.
    std::stack<NodePtr> m_nodeHistory;

    //this ctor starts the iterator at the first node in an in-order
    //traversal (the left most node in the AVL tree).
    AVLTIterator(NodePtr root) : m_nodeHistory{}
    {
        //if nullptr was passed for the root, then the stack will
        //be empty and *this will be equivalent to end()
        if(!root) return;

        //start the stack off with the root node
        m_nodeHistory.push(root);

        while(m_nodeHistory.top()->m_left)
            m_nodeHistory.push(m_nodeHistory.top()->m_left);
    }

    //this ctor starts the iterator at a specific node
    AVLTIterator(NodePtr root, T const& elementToFind) : m_nodeHistory{}
    {
        if(!root) return;

        auto search = [&elementToFind, this]
        (this auto const& self, NodePtr currNode) -> NodePtr
        {         
            if(elementToFind > currNode->m_element)
            { 
                m_nodeHistory.push(currNode);
                self(currNode->m_right);
            }
            else if(elementToFind < currNode->m_element)
            { 
                m_nodeHistory.push(currNode);
                self(currNode->m_left);
            }

            //found the node to search for
            else if(elementToFind == currNode->m_element)
            {
                m_nodeHistory.push(currNode);
            }
            else return nullptr;
        };

        if(!search(root))
        {
            //the node wasnt found in the tree. clear the stack to return end()
            while(!m_nodeHistory.empty())
                m_nodeHistory.pop();
        }
    }

    //for the avl tree to easily get a pointer to the parent of a node.
    //the iterator is passed by reference as to not copy the pointer stack.
    NodePtr getParentNode()
    {
        //if the iterator points to the root or if the iterator is end()
        if(m_nodeHistory.size() == 1 || m_nodeHistory.empty()) return nullptr;

        //get the parent of the current node
        auto current = getTopAndPop();
        auto parent  = getTopAndPop();

        //put the pointers back on the stack
        m_nodeHistory.push(parent);
        m_nodeHistory.push(current);

        return parent;
    }

    //for the tree to easily get the underlying Node*
    NodePtr getNodePtr()
    {
        return m_nodeHistory.top();
    }

    //get the node off the top of the stack and pop
    NodePtr getTopAndPop()
    {
        auto ret = getNodePtr();
        m_nodeHistory.pop();
        return ret;
    }

public:

    ~AVLTIterator() = default;

    //postfix increment/decrement not supported as to not allow
    //accidental copies of the internal pointer stack.
    //AVLTIterator operator++(int) {}
    //AVLTIterator operator--(int) {}

    //prefix increment
    AVLTIterator& operator++()
    {
        //if the current node has a right sub tree, then its in order
        //successor will be the left most node in that right sub tree
        if(m_nodeHistory.top()->m_right)
        {   
            //step down to the right sub tree
            m_nodeHistory.push(m_nodeHistory.top()->m_right);

            //go down to the left most node
            while(m_nodeHistory.top()->m_left)
                m_nodeHistory.push(m_nodeHistory.top()->m_left);

            return *this;
        }

        //else if the current node doesnt have a right sub tree then
        //its in order successor will be above it in the tree

        //go "up" the tree until we go over a left edge
        auto child = getTopAndPop();
        while(!m_nodeHistory.empty() && m_nodeHistory.top()->m_right == child)
            child = getTopAndPop();

        return *this;
    }

    //prefix decrement
    AVLTIterator& operator--()
    {
        //if the current node has a left sub tree, then its in order
        //predecessor will be the right most node in that left sub tree
        if(m_nodeHistory.top()->m_left)
        {
            //step down to the left sub tree   
            m_nodeHistory.push(m_nodeHistory.top()->m_left);

            //go down to the right most node
            while(m_nodeHistory.top()->m_right)
                m_nodeHistory.push(m_nodeHistory.top()->m_right);

            return *this;
        }

        //else if the current node doesnt have a left sub tree then
        //its in order predecessor will be above it in the tree

        //go "up" the tree until we go over a right edge   
        auto child = getTopAndPop();
        while(!m_nodeHistory.empty() && m_nodeHistory.top()->m_left == child)
            child = getTopAndPop();

        return *this;
    }

    T* operator->()
    {
        return &m_nodeHistory.top()->m_element;
    }

    T& operator*()
    {
        return m_nodeHistory.top()->m_element;
    }

    bool operator==(AVLTIterator const& rhs)
    {
        //make sure containers are not empty before calling top()
        bool thisIsEmpty = m_nodeHistory.empty(), rhsIsEmpty = rhs.m_nodeHistory.empty();
        if(!thisIsEmpty && !rhsIsEmpty) return m_nodeHistory.top() == rhs.m_nodeHistory.top();
        return thisIsEmpty && rhsIsEmpty;
    }

    bool operator!=(AVLTIterator const& rhs)
    {
        return !(*this == rhs);
    }

};

//todo restrict template so that T is has to be comparable for better compiler error messages
template <typename T>
class AVLTree
{
private:

    struct Node
    {
        T m_element;
        Node* m_left = nullptr, *m_right = nullptr;
        int m_balanceFactor = 0, m_height = 0;

        //move or copy an element into a node.
        template<typename Ty>
        Node(Ty&& elem) : m_element{std::forward<Ty>(elem)} {}

        void updateHeightAndBalanceFactor()
        {
            int rightTreeHeight = m_right ? m_right->m_height : -1;
            int leftTreeHeight  = m_left  ? m_left->m_height  : -1;
            m_height = std::max(rightTreeHeight, leftTreeHeight) + 1;
            m_balanceFactor = rightTreeHeight - leftTreeHeight;
        }

        //Should be called after the above method is called, since the balance factor needs to be updated first.
        //parentsChildPtr is the pointer (inside this' parent) that points to this node.
        void reBalance(Node*& parentsChildPtr)
        {   
            if(m_balanceFactor == -2)//this node is left heavy
            {
                //a left-right double rotation is necessary
                if(m_left->m_balanceFactor == 1) 
                    m_left->leftRotate(m_left);

                rightRotate(parentsChildPtr);
            }       
            else if(m_balanceFactor == 2)//this node is right heavy
            {
                //a right-left double rotation is necessary
                if(m_right->m_balanceFactor == -1) 
                    m_right->rightRotate(m_right);
                
                leftRotate(parentsChildPtr);
            }
        }

        //parentsChildPtr is the pointer (inside this' parent) that points to this node.
        void leftRotate(Node*& parentsChildPtr)
        {
            auto oldRightChild = m_right;//save the pointer to the right child (this nodes right child will become this node's new parent)
            m_right = oldRightChild->m_left;//assign a new right child to this node
            oldRightChild->m_left = this;//the new parent of this node is now this node's old right child
            parentsChildPtr = oldRightChild;//the old parent of this node now points to this node's old right child
            updateHeightAndBalanceFactor();//update the height and BF of this node before we update this node's new parent
            oldRightChild->updateHeightAndBalanceFactor();//update this node's new parent
        }

        //parentsChildPtr is the pointer (inside this' parent) that points to this node.
        void rightRotate(Node*& parentsChildPtr)
        {
            auto oldLeftChld = m_left;//save the pointer to the left child (this nodes left child will become this node's new parent)
            m_left = oldLeftChld->m_right;//assign a new left child to this node
            oldLeftChld->m_right = this;//the new parent of this node is now this node's old left child
            parentsChildPtr = oldLeftChld;//the old parent of this node now points to this node's old left child
            updateHeightAndBalanceFactor();//update the height and BF of this node before we update this node's new parent
            oldLeftChld->updateHeightAndBalanceFactor();//update this node's new parent
        }
    };

    Node* m_root;

    Node* minNode(Node* node)
    {
        if(!node) return nullptr;
        while(node->m_left) node = node->m_left;
        return node;
    }

    Node* maxNode(Node* node)
    {
        if(!node) return nullptr;
        while(node->m_right) node = node->m_right;
        return node;
    }

    //private recursive insert that takes a forwarding reference.
    //used by the copy and move public insert methods below
    template <typename Ty>
    Node* insert(Ty&& toInsert, Node*& node)
    {
        if(!node)//base case: end of tree reached make a new node and return
            node = new Node(std::forward<Ty>(toInsert));
        else if(toInsert > node->m_element)//the element to insert needs to go to the right
            insert(std::forward<Ty>(toInsert), node->m_right);
        else if(toInsert < node->m_element)//the element to insert needs to go to the left
            insert(std::forward<Ty>(toInsert), node->m_left);

        node->updateHeightAndBalanceFactor();
        node->reBalance(node);
        return node;
    }

public:

    AVLTree() : m_root{nullptr} {}

    //to follow RAII free the tree automatically at the end of its scope
    //or if an exception is thrown before the end of its scope
    ~AVLTree() 
    {
        clearTree();
    }

    using Iterator = AVLTIterator<T>;
    friend class Iterator;
    Iterator begin() {return Iterator(m_root);}  //get iterator to the first inorder node
    Iterator end()   {return Iterator(nullptr);} //get iterator "past" the last inorder node

    bool isEmpty() const 
    {
        return m_root;
    }

    //The Iterator ctor will do a binary search on the tree and return
    //an iterator to that node or and end() iterator if it cannot find the node.
    Iterator binarySearch(T const& toSearchFor) const
    {
        return Iterator(m_root, toSearchFor);
    }

    //copy an element into the tree if that element doenst already exist
    void insert(T const& toInsert)
    {
        insert(toInsert, m_root);
    }

    //move an element into the tree if that element doenst already exist
    void insert(T&& toInsert)
    {
        insert(std::forward<T>(toInsert), m_root);
    }

    //Returns true or false indicating whether or not toErase was found and deleted.
    bool erase(T const& toErase)
    {
        bool wasErased = false;
        auto deleteNode = [&wasErased, this](this auto const& self, Node*& node, T const& toErase) -> void
        {
            if(!node) return;
            if(toErase < node->m_element) self(node->m_left, toErase);
            else if(toErase > node->m_element) self(node->m_right, toErase);
            else if(node->m_left && node->m_right)//if two child nodes replace toErase with its in order predecessor
            {
                auto predecessor = maxNode(node->m_left);
                node->m_element = std::move(predecessor->m_element);//try to move the contents over
                self(node->m_left, node->m_element);//now delete the predecessor
            }
            else//if one or zero child nodes
            {
                auto nodeToDelete = node;

                //make sure the parent points to the child of the node we are deleting (if any)
                node = node->m_left ? node->m_left : node->m_right;

                delete nodeToDelete;
                wasErased = true;
                return;
            }

            node->updateHeightAndBalanceFactor();
            node->reBalance(node);
            return;
        };

        deleteNode(m_root, toErase);
        return wasErased;
    }

    //clear all the nodes in a post order recursive fashion
    void clearTree()
    {
        auto postOrderClear = [](this auto const& self, Node* node) -> void
        {
            if(node->m_left) self(node->m_left);
            if(node->m_right) self(node->m_right);
            delete node;
        };

        if(!m_root) return;
        postOrderClear(m_root);
        m_root = nullptr;
    }
};